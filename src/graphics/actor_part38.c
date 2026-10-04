#include "core.h"
#include "actor.h"
#include "vtable.h"

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
 * file covers `sub_8014F8C` and `sub_8015038` (both matched); the chunk continues in actor_part28b.c/c.c/d.c, split
 * at each parked function's raw-asm gap - see docs/matching/
 * issue-18-0x08014f8c-actor.md for the full write-up. */

extern void *gAudioContext;
extern void *gUnknown_030012F0;
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);
extern s32 _call_via_r1(void *addr, void *fn);
extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern s32 _call_via_r3(void *arg0, void *arg1, void *arg2, void *arg3);
extern void _call_via_r4(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void sub_800F6B8(s32 x, s32 y, s32 arg2, s32 arg3);

/* For each `struct actor *` in the `gUnknown_030012F0` list: skips
 * entries whose `+0x48` trampoline (`_call_via_r1`) reports a width of 4
 * or less, entries further than 0x40 (Manhattan distance) from `self`'s
 * own part, entries without their `+0xc` bit 6 flag set, and entries
 * more than 0x11 away vertically - then fires the `+0x68` trampoline
 * pair via `_call_via_r4` with action `0x16` on whatever survives all
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
         * `CountSapphireRelics`/`CountGoldRelics` entry for the general technique).
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

        if (_call_via_r1(addr, fn) <= 4) {
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

            _call_via_r4(addr2, 0, 0x16, 0);
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


extern void *gAudioContext;
extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern s32 _call_via_r3(void *arg0, void *arg1, void *arg2, void *arg3);

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
 * Built with old_agbcc (the file is on OLD_AGBCC_OBJS). The
 * `self+0x24 != 0` arm tests `self[0x22]` directly rather than through a
 * `u8` local: the byte load then comes after the point where
 * old_agbcc's GCSE inserts its copy of `self + 0x22` (end of the block,
 * before the compare), so the load goes through the copy in r5
 * (`adds r5, r0, #0; ldrb r2, [r5]`) as in the ROM. */
void sub_8015038(u8 *self, s32 id, s32 param2)
{
    struct vtable_slot *mgr;
    u8 *off;

    if (self[0x24] == 0) {
        s32 idx = 0x17;
        s32 zero;
        s32 wait;

        self[0x21] = 0;
        if (self[0x22] == 1) {
            idx = 0x28;
            self[0x21] = 1;
        } else if (self[0x22] == 2) {
            idx = 0x27;
            self[0x21] = 2;
        }
        zero = 0;
        wait = 0x14;
        mgr = *(struct vtable_slot **)(self + 0xc);
        _call_via_r2(self + mgr[4].delta, (void *)id, mgr[4].fn);
        off = *(u8 **)(self + 0xc);
        off += 0x50;
        _call_via_r3(self + *(s16 *)off, *(void **)(self + 0x10), (void *)idx,
                    *(void **)(off + 4));
        *(s32 *)(self + 0x18) = zero;
        *(s32 *)(self + 0x1c) = wait;
        PlaySfx(gAudioContext, self[0x21] + 0x57, 0x100);
        if (++self[0x22] >= self[0x20]) {
            self[0x24] = 1;
            if (self[0x22] > 1)
                self[0x22] = 1;
            else
                self[0x22] = zero;
        }
    } else {
        register s32 hold1 asm("r1");

        /* No code: keeps r1 live across the `self[0x22]` test so the
         * byte loads into r2 and `id` stays in ip, as in the ROM. */
        asm("" : "=r"(hold1));
        if (self[0x22] > 0xf0) {
            s32 idx;
            s32 zero;
            s32 wait;

            asm("" : : "r"(hold1)); /* end of the r1 hold (no code) */
            idx = 0x17;
            self[0x21] = 0;
            if (self[0x22] == 1) {
                idx = 0x28;
                self[0x21] = 1;
            } else if (self[0x22] == 2) {
                idx = 0x27;
                self[0x21] = 2;
            }
            zero = 0;
            wait = 0x14;
            mgr = *(struct vtable_slot **)(self + 0xc);
            _call_via_r2(self + mgr[4].delta, (void *)id, mgr[4].fn);
            off = *(u8 **)(self + 0xc);
            off += 0x50;
            _call_via_r3(self + *(s16 *)off, *(void **)(self + 0x10), (void *)idx,
                        *(void **)(off + 4));
            *(s32 *)(self + 0x18) = zero;
            *(s32 *)(self + 0x1c) = wait;
            PlaySfx(gAudioContext, self[0x21] + 0x57, 0x100);
        } else {
            u8 *p21 = self + 0x21;
            s32 zero = 0;
            s32 wait;

            *p21 = zero;
            self[0x20] = zero;
            wait = 0x18;
            mgr = *(struct vtable_slot **)(self + 0xc);
            _call_via_r2(self + mgr[4].delta, (void *)param2, mgr[4].fn);
            off = *(u8 **)(self + 0xc);
            off += 0x50;
            _call_via_r3(self + *(s16 *)off, *(void **)(self + 0x10), (void *)0x10,
                        *(void **)(off + 4));
            *(s32 *)(self + 0x18) = zero;
            *(s32 *)(self + 0x1c) = wait;
            PlaySfx(gAudioContext, 0xa, 0x100);
            self[0x26] = 0x63;
        }
        self[0x22]--;
    }
    self[0x23] = 0;
}
