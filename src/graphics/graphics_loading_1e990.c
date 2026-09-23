#include "core.h"
#include "actor.h"

extern void *gUnknown_030012C0;
extern void *gUnknown_030012B4;
extern struct actor *gUnknown_030012D8;
extern void *gUnknown_030012BC;

extern u8 sub_80232F4(void *self);
extern s32 sub_80232E0(void *self);
extern s32 sub_8023130(void *self);
extern s32 sub_803AFEC(void *self);
extern u8 sub_80232B8(void *self);
extern void sub_803AD88(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void PlaySfx(void *bank, s32 arg1, s32 sfxId);

/* Sound-trigger dispatch/position writer - the last of the
 * `LoadGraphicsPackage` cluster's scratch-buffer-style helper family
 * (issue #30). Two independent, unrelated halves:
 *
 * 1. If `sub_80232F4(gUnknown_030012C0)` (the player's `+0xa8` flag)
 *    is set: looks up a per-`z` flags byte via the same
 *    `gUnknown_030012B4 -> *rec -> {+8 offsets[], +0xc base}` table
 *    `sub_8021D04` (graphics_loading_21bfc.c) already reads, folds
 *    its bit 1 into the player's `+0x28` bitfield's bit 4, then
 *    unconditionally writes the incoming `x`/`y` (Q8.8, shifted from
 *    the raw `u16` args) into the player's own `x`/`y` fields - the
 *    same unconditional write `sub_80221A4`/`sub_80221D4`
 *    (graphics_loading_21d80.c) already do elsewhere in this cluster.
 *
 * 2. Unless the player's `+0x8c` "paused" flag is set: fires the
 *    player's `table+0x68` trampoline (via `sub_803AD88`, action
 *    `0x1a`) and plays SFX `0x100` through `gUnknown_030012BC`,
 *    unless a budget/reentrancy guard trips first - either the
 *    player's spawn counter (`sub_80232E0`, `+0x7c`) has room against
 *    its cap (`sub_8023130`, `+0x84`), or (when it doesn't) all three
 *    of `sub_803AFEC` (`+0x74`), `sub_80232B8` (`+0xa4`) and the
 *    player's `+0x78` mode field agree it's still safe to fire.
 *
 * Written as NAKED asm, not plain C: a 96.8%-matching C reconstruction
 * is kept below under `#if NON_MATCHING` - see
 * docs/matching/naked-sub_801e990-matched.md for the derivation and
 * the residual register-choice gap. Mechanical, byte-verified
 * transcription of the ROM's own instructions, not an inferred
 * control-flow guess. */
#if NON_MATCHING
/* NOT YET BYTE-MATCHING - 96.8% instruction match (see the doc comment
 * above and docs/matching/naked-sub_801e990-matched.md); compiled only
 * under `make NON_MATCHING=1`, the NAKED version below is used
 * otherwise. */
void sub_801E990(u32 arg0, u16 x, u16 y, u16 z)
{
    if (sub_80232F4(gUnknown_030012C0)) {
        register u8 *rec asm("r2");
        register u16 *arrayBase asm("r0");
        register s32 addr asm("r1");
        register u8 *tmp asm("r0");
        register u8 *d8addr asm("r3");

        rec = *(u8 **)gUnknown_030012B4;
        arrayBase = *(u16 **)(rec + 8);
        addr = (z << 1) + (s32)arrayBase;
        {
            s32 base = *(s32 *)(rec + 0xc);
            addr = *(u16 *)addr;
            tmp = (u8 *)(addr + base);
        }

        {
            register u8 byte asm("r0") = *tmp;
            register s32 shiftedByte asm("r2");
            register s32 one asm("r0");
            register u8 *addr28 asm("r1");
            register s32 mask asm("r0");
            register u8 byte2 asm("r4");

            shiftedByte = byte >> 1;
            one = 1;
            d8addr = (u8 *)gUnknown_030012D8;
            addr28 = d8addr + 0x28;
            shiftedByte &= one;
            shiftedByte <<= 4;
            asm volatile("sub %0, %0, #0x12" : "+r"(one));
            mask = one;
            byte2 = *addr28;
            mask &= byte2;
            mask |= shiftedByte;
            *addr28 = mask;
        }

        {
            register u8 *obj2 asm("r1") = d8addr;
            *(u32 *)obj2 = x << 8;
            *(u32 *)(obj2 + 4) = y << 8;
        }
    }

    if (*(u8 *)((u8 *)gUnknown_030012C0 + 0x8c) != 0) {
        goto end;
    }
    {
        s32 spawnCount = sub_80232E0(gUnknown_030012C0);
        s32 cap = sub_8023130(gUnknown_030012C0);
        if (spawnCount >= cap) {
            goto fire;
        }
        if (sub_803AFEC(gUnknown_030012C0) != 0) {
            goto end;
        }
        if (sub_80232B8(gUnknown_030012C0) != 0) {
            goto end;
        }
        if (*(s32 *)((u8 *)gUnknown_030012C0 + 0x78) != 0) {
            goto end;
        }
    }
fire:
    {
        register u8 *d8obj asm("r0");
        register u8 *entry asm("r1");
        register s32 fnOffset asm("r2");
        register void *fn asm("r0");
        register u32 dead asm("r4");

        d8obj = (u8 *)gUnknown_030012D8;
        entry = *(u8 **)(d8obj + 0x18);
        entry = entry + 0x68;
        asm volatile("mov r3, #0\n\tldrsh %0, [%1, r3]" : "=r"(fnOffset) : "r"(entry) : "r3");
        fn = d8obj + fnOffset;
        dead = *(u32 volatile *)(entry + 4);
        (void)dead;
        sub_803AD88(fn, 0, 0x1a, 0);
        PlaySfx(gUnknown_030012BC, 1, 0x100);
    }
end:;
}
#else /* !NON_MATCHING */
NAKED void sub_801E990(u32 arg0, u16 x, u16 y, u16 z)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r6, r1, #0x10\n\t"
        "lsl r2, r2, #0x10\n\t"
        "lsr r7, r2, #0x10\n\t"
        "lsl r3, r3, #0x10\n\t"
        "lsr r4, r3, #0x10\n\t"
        "ldr r5, 10f\n\t"
        "ldr r0, [r5]\n\t"
        "bl sub_80232F4\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq 1f\n\t"
        "ldr r0, 11f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r2, [r0]\n\t"
        "ldr r0, [r2, #8]\n\t"
        "lsl r1, r4, #1\n\t"
        "add r1, r1, r0\n\t"
        "ldr r0, [r2, #0xc]\n\t"
        "ldrh r1, [r1]\n\t"
        "add r0, r1, r0\n\t"
        "ldrb r0, [r0]\n\t"
        "lsr r2, r0, #1\n\t"
        "mov r0, #1\n\t"
        "ldr r3, 12f\n\t"
        "ldr r1, [r3]\n\t"
        "add r1, #0x28\n\t"
        "and r2, r0\n\t"
        "lsl r2, r2, #4\n\t"
        "sub r0, #0x12\n\t"
        "ldrb r4, [r1]\n\t"
        "and r0, r4\n\t"
        "orr r0, r2\n\t"
        "strb r0, [r1]\n\t"
        "ldr r1, [r3]\n\t"
        "lsl r0, r6, #8\n\t"
        "str r0, [r1]\n\t"
        "lsl r0, r7, #8\n\t"
        "str r0, [r1, #4]\n\t"
    "1:\n\t"
        "ldr r1, [r5]\n\t"
        "add r0, r1, #0\n\t"
        "add r0, #0x8c\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
        "add r0, r1, #0\n\t"
        "bl sub_80232E0\n\t"
        "add r4, r0, #0\n\t"
        "ldr r0, [r5]\n\t"
        "bl sub_8023130\n\t"
        "cmp r4, r0\n\t"
        "bge 2f\n\t"
        "ldr r0, [r5]\n\t"
        "bl sub_803AFEC\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
        "ldr r0, [r5]\n\t"
        "bl sub_80232B8\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
        "ldr r0, [r5]\n\t"
        "ldr r0, [r0, #0x78]\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
    "2:\n\t"
        "ldr r0, 12f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0, #0x18]\n\t"
        "add r1, #0x68\n\t"
        "mov r3, #0\n\t"
        "ldrsh r2, [r1, r3]\n\t"
        "add r0, r0, r2\n\t"
        "ldr r4, [r1, #4]\n\t"
        "mov r1, #0\n\t"
        "mov r2, #0x1a\n\t"
        "mov r3, #0\n\t"
        "bl sub_803AD88\n\t"
        "ldr r0, 13f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #1\n\t"
        "bl PlaySfx\n\t"
    "3:\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "10: .4byte gUnknown_030012C0\n"
    "11: .4byte gUnknown_030012B4\n"
    "12: .4byte gUnknown_030012D8\n"
    "13: .4byte gUnknown_030012BC\n"
    );
}
#endif /* NON_MATCHING */
