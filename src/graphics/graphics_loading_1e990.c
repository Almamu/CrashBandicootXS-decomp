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
 * Was a NAKED asm transcription for a long time - see
 * docs/matching/naked-sub_801e990-matched.md for the full derivation
 * history, including the register-choice gap that blocked a real match
 * (the `+0x28` write's address/value register split) and how it closed:
 * the r3-pinned local had to model the *address of the global*
 * (`&gUnknown_030012D8`, a `struct actor **`) with `+0x28` computed as
 * a single dereference-and-add into r1, rather than modeling the
 * *dereferenced value* itself and copying it into r1 afterward - the
 * latter is semantically equivalent but makes gcc materialize the
 * value in a different temp register first, needing an extra `mov`
 * the ROM doesn't have. */
void sub_801E990(u32 arg0, u16 x, u16 y, u16 z)
{
    if (sub_80232F4(gUnknown_030012C0)) {
        register u8 *rec asm("r2");
        register u16 *arrayBase asm("r0");
        register s32 addr asm("r1");
        register u8 *tmp asm("r0");
        register struct actor **d8ptr asm("r3");

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
            d8ptr = &gUnknown_030012D8;
            addr28 = (u8 *)*d8ptr + 0x28;
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
            register u8 *obj2 asm("r1") = (u8 *)*d8ptr;
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
