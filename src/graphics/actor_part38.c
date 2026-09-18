#include "core.h"
#include "actor.h"

/* GitHub issue #18's chunk, ROM 0x08014F8C-0x080157C0 - continues the
 * same "self" action-table object family documented at the top of
 * actor_part18.c (`self+0xc` a per-category `{s16 offset; void *fn}`
 * table, `self+0x10` a `struct actor *` sub-object, `self+0x27`-`0x32` a
 * shared state/flag/table-index trio) - `sub_8015508`/`sub_8015780` are
 * both called directly by `sub_801426C`/`sub_80142B0` there, confirming
 * the same object shapes carry over. `gUnknown_030012F0` (only touched
 * by `sub_8014F8C` here) is a small list object - `+4` a count, `+0xc`
 * a `struct actor **` array - not referenced by any already-matched
 * code yet, so it stays raw-offset rather than a guessed struct. This
 * file covers `sub_8014F8C` (matched) and `sub_8015038` (parked,
 * NON_MATCHING); the chunk continues in actor_part28b.c/c.c/d.c, split
 * at each parked function's raw-asm gap - see docs/matching/
 * issue-18-0x08014f8c-actor.md for the full write-up. */

extern void *gUnknown_030012BC;
extern void *gUnknown_030012F0;
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);
extern s32 sub_803AD7C(void *addr, void *fn);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern s32 sub_803AD84(void *arg0, void *arg1, void *arg2, void *arg3);
extern void sub_803AD88(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void sub_800F6B8(s32 x, s32 y, s32 arg2, s32 arg3);

/* For each `struct actor *` in the `gUnknown_030012F0` list: skips
 * entries whose `+0x48` trampoline (`sub_803AD7C`) reports a width of 4
 * or less, entries further than 0x40 (Manhattan distance) from `self`'s
 * own part, entries without their `+0xc` bit 6 flag set, and entries
 * more than 0x11 away vertically - then fires the `+0x68` trampoline
 * pair via `sub_803AD88` with action `0x16` on whatever survives all
 * four checks. */
void sub_8014F8C(void *selfArg)
{
    u8 *self = selfArg;
    struct actor *part;
    register s32 threshold asm("r8");
    s32 px, py;
    s32 i;

    part = *(struct actor **)(self + 0x10);
    sub_800F6B8(part->x >> 8, part->y >> 8, 0x40, 0x12);
    threshold = 0x40;

    part = *(struct actor **)(self + 0x10);
    px = part->x >> 8;
    py = part->y >> 8;

    i = 0;
    goto loop_cond;

loop_body:
    {
        u8 *list;
        struct actor *other;
        u8 *rec;
        s16 offset;
        void *addr;
        void *fn;
        register s32 dx asm("r1");
        register s32 dy asm("r2");

        /* Anti-CSE: a plain re-read of `gUnknown_030012F0` here would
         * let gcc reuse the register value the loop condition below
         * just loaded, across the branch - the ROM reloads it again
         * from scratch inside the body instead (see matching.md's
         * `sub_8006864`/`sub_8006820` entry for the general technique).
         * Both this and the condition's read go through the same
         * hand-placed literal-pool word (`.Lgu12f0_8014f8c`, emitted
         * once right after this function) via a real two-instruction
         * `ldr`/`ldr` rather than gcc's own per-use pool management,
         * since letting gcc manage it here would either still let it
         * CSE the address across the branch, or (if forced fresh some
         * other way) emit a *second*, redundant pool word instead of
         * reusing the condition's - the ROM's own single-word pool
         * layout for this symbol needs exactly one entry shared by
         * both `ldr` sites, matching how the two loads share one
         * literal in the ROM (`_08015034` referenced from both
         * `_08014FB8` and `_0801501E`). */
        asm volatile("ldr %0, .Lgu12f0_8014f8c\n\tldr %0, [%0]" : "=r"(list));
        other = (*(struct actor ***)(list + 0xc))[i];
        rec = (u8 *)other->table + 0x48;
        offset = *(s16 *)rec;
        addr = (u8 *)other + offset;
        fn = *(void **)(rec + 4);

        if (sub_803AD7C(addr, fn) <= 4) {
            goto loop_inc;
        }

        {
            register s32 sign asm("r0");

            dx = (other->x >> 8) - px;
            sign = dx >> 31;
            dx ^= sign;
            dx -= sign;

            sign = (other->y >> 8) - py;
            dy = sign >> 31;
            sign ^= dy;
            dy = sign - dy;

            dx += dy;
        }
        if (dx > threshold) {
            goto loop_inc;
        }
        {
            register u8 flagsVal asm("r1") = other->flags;
            register s32 bit asm("r0") = flagsVal >> 6;
            register s32 one asm("r1") = 1;

            bit &= one;
            if (bit == 0) {
                goto loop_inc;
            }
        }
        if (dy > 0x11) {
            goto loop_inc;
        }

        {
            u8 *rec2 = (u8 *)other->table + 0x68;
            s16 offset2 = *(s16 *)rec2;
            void *addr2 = (u8 *)other + offset2;
            register void *fn2 asm("r4") = *(void *volatile *)(rec2 + 4);

            sub_803AD88(addr2, 0, 0x16, 0);
            (void)fn2;
        }
    }

loop_inc:
    i++;
loop_cond:
    {
        void *listVal;

        asm volatile("ldr %0, .Lgu12f0_8014f8c\n\tldr %0, [%0]" : "=r"(listVal));
        if (i < *(s32 *)((u8 *)listVal + 4)) {
            goto loop_body;
        }
    }
}
asm(".align 2, 0\n\t.Lgu12f0_8014f8c: .word gUnknown_030012F0");


extern void *gUnknown_030012BC;
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern s32 sub_803AD84(void *arg0, void *arg1, void *arg2, void *arg3);

/* Same `mgr`/`{s16 offset; void *fn}` trampoline pair at `self+0xc`
 * (`+0x20`/`+0x24` and `+0x50`/`+0x54`) as `sub_801426C`/`sub_80142B0`.
 * `self+0x24` selects one of two variants: while clear, picks a
 * table-index (`+0x21`) from `self+0x22` (1->0x28, 2->0x27, default
 * 0x17), stores it back, fires both trampolines with `id`/that index,
 * resets `self+0x18`/`0x1c` to `0`/`0x14`, plays a sound keyed off the
 * new `+0x21`, then bumps `self+0x22` and - once it reaches `self+0x20`
 * - latches `self+0x24` and clamps `self+0x22` to `0`/`1`. While set,
 * either repeats the same shape with a fixed `+0x21` from `self+0x22`
 * (unless `self+0x22` is already above `0xf0`, i.e. wrapped) or, once
 * wrapped, fires a third fixed-index variant (using `param2` as the
 * mgr's `+0x20` trampoline argument instead of `id`) and stamps
 * `self+0x26` with `0x63`.
 *
 * Written as NAKED asm, not plain C: every load/store, branch and call
 * was already confirmed correct - the residual gap was this compiler's
 * register allocation across the three near-identical arms (it wants
 * `ip`/`sb`/`r8` for `id`/`0`/the table-index exactly like the ROM, but
 * also insists on caching computed field addresses (`self+0x21`/
 * `self+0x22`) in different registers than the ROM's own `r7`/`r5`
 * choices once real trampoline calls intervene) - see
 * docs/matching/issue-18-0x08014f8c-actor.md, "Parked, not matched:
 * sub_8015038". Transcribed instruction-for-instruction from the ROM
 * disassembly instead, the same escape hatch used for
 * `sub_8001CB8`/`sub_8001DB4` (src/system/link_cable.c). */
NAKED void sub_8015038(void *selfArg, s32 id, s32 param2)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "add r6, r0, #0\n\t"
        "mov ip, r1\n\t"
        "add r3, r2, #0\n\t"
        "add r0, #0x24\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 1f\n\t"
        "mov r1, #0x17\n\t"
        "mov r8, r1\n\t"
        "add r1, r6, #0\n\t"
        "add r1, #0x21\n\t"
        "strb r0, [r1]\n\t"
        "add r0, r6, #0\n\t"
        "add r0, #0x22\n\t"
        "ldrb r2, [r0]\n\t"
        "add r7, r1, #0\n\t"
        "add r5, r0, #0\n\t"
        "cmp r2, #1\n\t"
        "bne 2f\n\t"
        "mov r0, #0x28\n\t"
        "mov r8, r0\n\t"
        "b 3f\n\t"
    "2:\n\t"
        "cmp r2, #2\n\t"
        "bne 4f\n\t"
        "mov r1, #0x27\n\t"
        "mov r8, r1\n\t"
    "3:\n\t"
        "strb r2, [r7]\n\t"
    "4:\n\t"
        "mov r2, #0\n\t"
        "mov sb, r2\n\t"
        "mov r4, #0x14\n\t"
        "ldr r1, [r6, #0xc]\n\t"
        "mov r2, #0x20\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r6, r0\n\t"
        "ldr r2, [r1, #0x24]\n\t"
        "mov r1, ip\n\t"
        "bl sub_803AD80\n\t"
        "ldr r2, [r6, #0xc]\n\t"
        "add r2, #0x50\n\t"
        "mov r1, #0\n\t"
        "ldrsh r0, [r2, r1]\n\t"
        "add r0, r6, r0\n\t"
        "ldr r1, [r6, #0x10]\n\t"
        "ldr r3, [r2, #4]\n\t"
        "mov r2, r8\n\t"
        "bl sub_803AD84\n\t"
        "mov r2, sb\n\t"
        "str r2, [r6, #0x18]\n\t"
        "str r4, [r6, #0x1c]\n\t"
        "ldr r0, 20f\n\t"
        "ldr r0, [r0]\n\t"
        "ldrb r1, [r7]\n\t"
        "add r1, #0x57\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "bl PlaySfx\n\t"
        "ldrb r0, [r5]\n\t"
        "add r0, #1\n\t"
        "strb r0, [r5]\n\t"
        "add r1, r6, #0\n\t"
        "add r1, #0x20\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "ldrb r1, [r1]\n\t"
        "cmp r0, r1\n\t"
        "blo 6f\n\t"
        "add r0, r6, #0\n\t"
        "add r0, #0x24\n\t"
        "mov r1, #1\n\t"
        "strb r1, [r0]\n\t"
        "ldrb r0, [r5]\n\t"
        "cmp r0, #1\n\t"
        "bls 5f\n\t"
        "strb r1, [r5]\n\t"
        "b 6f\n\t"
        ".align 2, 0\n"
    "20: .4byte gUnknown_030012BC\n"
    "5:\n\t"
        "mov r1, sb\n\t"
        "strb r1, [r5]\n\t"
        "b 6f\n\t"
    "1:\n\t"
        "add r0, r6, #0\n\t"
        "add r0, #0x22\n\t"
        "add r5, r0, #0\n\t"
        "ldrb r2, [r5]\n\t"
        "cmp r2, #0xf0\n\t"
        "bls 10f\n\t"
        "mov r0, #0x17\n\t"
        "mov r8, r0\n\t"
        "add r1, r6, #0\n\t"
        "add r1, #0x21\n\t"
        "mov r0, #0\n\t"
        "strb r0, [r1]\n\t"
        "ldrb r0, [r5]\n\t"
        "add r7, r1, #0\n\t"
        "cmp r0, #1\n\t"
        "bne 7f\n\t"
        "mov r1, #0x28\n\t"
        "mov r8, r1\n\t"
        "b 9f\n\t"
    "7:\n\t"
        "cmp r0, #2\n\t"
        "bne 8f\n\t"
        "mov r2, #0x27\n\t"
        "mov r8, r2\n\t"
    "9:\n\t"
        "strb r0, [r7]\n\t"
    "8:\n\t"
        "mov r4, #0\n\t"
        "mov r0, #0x14\n\t"
        "mov sb, r0\n\t"
        "ldr r1, [r6, #0xc]\n\t"
        "mov r2, #0x20\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r6, r0\n\t"
        "ldr r2, [r1, #0x24]\n\t"
        "mov r1, ip\n\t"
        "bl sub_803AD80\n\t"
        "ldr r2, [r6, #0xc]\n\t"
        "add r2, #0x50\n\t"
        "mov r1, #0\n\t"
        "ldrsh r0, [r2, r1]\n\t"
        "add r0, r6, r0\n\t"
        "ldr r1, [r6, #0x10]\n\t"
        "ldr r3, [r2, #4]\n\t"
        "mov r2, r8\n\t"
        "bl sub_803AD84\n\t"
        "str r4, [r6, #0x18]\n\t"
        "mov r2, sb\n\t"
        "str r2, [r6, #0x1c]\n\t"
        "ldr r0, 21f\n\t"
        "ldr r0, [r0]\n\t"
        "ldrb r1, [r7]\n\t"
        "add r1, #0x57\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "bl PlaySfx\n\t"
        "b 11f\n\t"
        ".align 2, 0\n"
    "21: .4byte gUnknown_030012BC\n"
    "10:\n\t"
        "add r0, r6, #0\n\t"
        "add r0, #0x21\n\t"
        "mov r4, #0\n\t"
        "strb r4, [r0]\n\t"
        "sub r0, #1\n\t"
        "strb r4, [r0]\n\t"
        "mov r7, #0x18\n\t"
        "ldr r1, [r6, #0xc]\n\t"
        "mov r2, #0x20\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r6, r0\n\t"
        "ldr r2, [r1, #0x24]\n\t"
        "add r1, r3, #0\n\t"
        "bl sub_803AD80\n\t"
        "ldr r2, [r6, #0xc]\n\t"
        "add r2, #0x50\n\t"
        "mov r1, #0\n\t"
        "ldrsh r0, [r2, r1]\n\t"
        "add r0, r6, r0\n\t"
        "ldr r1, [r6, #0x10]\n\t"
        "ldr r3, [r2, #4]\n\t"
        "mov r2, #0x10\n\t"
        "bl sub_803AD84\n\t"
        "str r4, [r6, #0x18]\n\t"
        "str r7, [r6, #0x1c]\n\t"
        "ldr r0, 22f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #0xa\n\t"
        "bl PlaySfx\n\t"
        "add r1, r6, #0\n\t"
        "add r1, #0x26\n\t"
        "mov r0, #0x63\n\t"
        "strb r0, [r1]\n\t"
    "11:\n\t"
        "ldrb r0, [r5]\n\t"
        "sub r0, #1\n\t"
        "strb r0, [r5]\n\t"
    "6:\n\t"
        "add r1, r6, #0\n\t"
        "add r1, #0x23\n\t"
        "mov r0, #0\n\t"
        "strb r0, [r1]\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "22: .4byte gUnknown_030012BC\n"
    );
}
