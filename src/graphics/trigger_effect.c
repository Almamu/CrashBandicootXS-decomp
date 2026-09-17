#include "core.h"
#include "actor.h"

#if NON_MATCHING
extern void *gUnknown_030012C0;
extern void *gUnknown_030012D0;
extern void *gUnknown_030012EC;

extern u8 sub_8023278(void *self);
extern s32 sub_801A878(u16 x, u16 y, u16 w, u16 h, s32 id);
extern s32 sub_80234E8(void *self, s32 handle);
extern struct actor *sub_8008434(u16 arg0, u16 arg1, u16 arg2, u16 arg3);
extern void sub_80087C0(void *part);
extern void sub_80087B4(void *part);
extern void sub_800872C(void *part, u8 val);
extern s32 sub_800815C(struct actor *part);
extern void sub_8008E94(void *manager, void *value);

/* One of the four "trigger effect type N" slots in the 15-slot dispatch
 * table at `gStaticData_0816C7D8` (docs/rom_map.md, "A family of
 * 'trigger effect type N' functions"). Tests bit 0 of
 * `gUnknown_030012C0+2`: if set, plays a sound only (id `0xB`, or the
 * shared fallback `0xC` if either `sub_8023278(gUnknown_030012C0)` is
 * true or `gUnknown_030012C0+0x8c` is nonzero) via `sub_801A878` +
 * `sub_80234E8`. Otherwise spawns a full visual effect: allocates a
 * part-object (`sub_8008434`), points its `+0x20` table pointer at
 * `gStaticData_084A5600`'s own first field (reached through
 * `gUnknown_030012D0`'s pointer-to-pointer, same idiom as
 * `sub_80083A8` in actor_part5.c) plus a fixed `0x180` offset, tags it
 * (`+0x2d = 7`), builds it via the standard `sub_80087C0`/
 * `sub_80087B4`/`sub_800872C` OAM trio, sets its `+0x29` bitfield via
 * `sub_800815C`, sets `field_0A` to the (always-zero, since this is
 * the "bit clear" branch) tested bit, registers it into
 * `gUnknown_030012EC`'s manager via `sub_8008E94`, and clears flag bit
 * 2 (`& -5`, the negative-constant bit-clear idiom, not `& ~5`).
 * `field_0x20`/`field_0x2d` are kept as raw offsets - they sit well
 * past `struct actor`'s documented 0x1c bytes, in the same not-yet-
 * understood per-spawn-variant tail `sub_8009F1C`/`sub_8009FB0`
 * (actor_part8.c) already leave as raw offsets.
 *
 * NOT YET BYTE-MATCHING - semantics fully understood and every
 * instruction's operation matches the ROM (confirmed via isolated
 * compile + manual instruction diff), but register allocation drifts
 * in two spots that resisted every restructuring tried:
 *   1. The `arg0`/`arg1`/`arg2`/`arg3` parameters land in a rotated
 *      register set (`r5`/`r6`/`r7` here vs. the ROM's `r6`/`r7`/`r5`
 *      for `arg1`/`arg2`/`arg3`) from function entry onward. Declaring
 *      `arg0` as `u32` (matching the ROM's deferred per-call-site
 *      truncation instead of an upfront one - the real fix for that
 *      part) and using `void` instead of `s32` as the return type
 *      (matching the ROM's `pop {r0}; bx r0` epilogue instead of the
 *      `r1` the compiler otherwise reaches for once a call's result
 *      looks "live") both got confirmed and kept, but no ordering of
 *      the four parameter reads changes gcc's own r5/r6/r7 pick to
 *      match the ROM's r6/r7/r5.
 *   2. The `gUnknown_030012C0` bit-test/`sub_8023278` call at function
 *      entry: the ROM computes the global's address once into `sb`
 *      (r9), dereferences straight into `r0`, and reuses that same
 *      `r0` for the `sub_8023278` argument without an intervening
 *      move; every source shape tried here (bare global reference, a
 *      local `void *`/`u8 *` alias, explicit register pins on `r0`/
 *      `r9`) instead routes the loaded value through `r1` and adds an
 *      extra `adds r0,r1,#0` before the call, or - when the alias is
 *      kept live across the call instead of reloaded via the cached
 *      address - drops the ROM's reload-after-call instructions
 *      entirely.
 * The negative-constant mask idiom (`& -0x10`, `& -5`) and the
 * post-call-then-address-then-mask statement order for the `+0x29`
 * bitfield update did get pinned down exactly (see
 * `src/audio/counter_selector_setup.c`'s `sub_80374D0` for the same
 * idiom) - the `.L3`/spawn-branch tail is instruction-for-instruction
 * identical to the ROM except for the register numbers themselves.
 * Parked here rather than keep guessing register-allocation orderings
 * with no more structural clues to try. */
void sub_8020E84(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    u8 bit = *((u8 *)gUnknown_030012C0 + 2) & 1;

    if (bit) {
        s32 handle;

        if (sub_8023278(gUnknown_030012C0) || *((u8 *)gUnknown_030012C0 + 0x8c) != 0) {
            handle = sub_801A878(arg0, arg1, arg2, arg3, 0xC);
        } else {
            handle = sub_801A878(arg0, arg1, arg2, arg3, 0xB);
        }
        sub_80234E8(gUnknown_030012C0, handle);
    } else {
        struct actor *part = sub_8008434(arg0, arg1, arg2, arg3);
        void *p2 = *(void **)gUnknown_030012D0;
        void *field0 = *(void **)p2;
        s32 mask;
        u8 *bitfield;
        s32 old;
        s32 flags;

        *(void **)((u8 *)part + 0x20) = (u8 *)field0 + 0x180;
        *((u8 *)part + 0x2d) = 7;
        sub_80087C0(part);
        sub_80087B4(part);
        sub_800872C(part, 0);
        {
            s32 result = sub_800815C(part);

            bitfield = (u8 *)part + 0x29;
            mask = result & 0xf;
        }
        old = *bitfield;
        old &= -0x10;
        old |= mask;
        *bitfield = old;
        part->field_0A = bit;
        sub_8008E94(gUnknown_030012EC, part);
        flags = part->flags;
        flags &= -5;
        part->flags = flags;
    }
}

/* Same shape as `sub_8020E84` above (twin sibling, tests bit 1 of
 * `gUnknown_030012C0+2` instead of bit 0), sound ids `3`/`0xC`, tag
 * value `5`. Same not-yet-byte-matching register-allocation gap. */
void sub_8020F7C(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    u8 bit = *((u8 *)gUnknown_030012C0 + 2) & 2;

    if (bit) {
        s32 handle;

        if (sub_8023278(gUnknown_030012C0) || *((u8 *)gUnknown_030012C0 + 0x8c) != 0) {
            handle = sub_801A878(arg0, arg1, arg2, arg3, 0xC);
        } else {
            handle = sub_801A878(arg0, arg1, arg2, arg3, 3);
        }
        sub_80234E8(gUnknown_030012C0, handle);
    } else {
        struct actor *part = sub_8008434(arg0, arg1, arg2, arg3);
        void *p2 = *(void **)gUnknown_030012D0;
        void *field0 = *(void **)p2;
        s32 mask;
        u8 *bitfield;
        s32 old;
        s32 flags;

        *(void **)((u8 *)part + 0x20) = (u8 *)field0 + 0x180;
        *((u8 *)part + 0x2d) = 5;
        sub_80087C0(part);
        sub_80087B4(part);
        sub_800872C(part, 0);
        {
            s32 result = sub_800815C(part);

            bitfield = (u8 *)part + 0x29;
            mask = result & 0xf;
        }
        old = *bitfield;
        old &= -0x10;
        old |= mask;
        *bitfield = old;
        part->field_0A = bit;
        sub_8008E94(gUnknown_030012EC, part);
        flags = part->flags;
        flags &= -5;
        part->flags = flags;
    }
}

/* Same shape as `sub_8020E84` above (twin sibling, tests bit 2 of
 * `gUnknown_030012C0+2`), sound ids `0xA`/`0xC`, tag value `6`. Same
 * not-yet-byte-matching register-allocation gap. */
void sub_802107C(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    u8 bit = *((u8 *)gUnknown_030012C0 + 2) & 4;

    if (bit) {
        s32 handle;

        if (sub_8023278(gUnknown_030012C0) || *((u8 *)gUnknown_030012C0 + 0x8c) != 0) {
            handle = sub_801A878(arg0, arg1, arg2, arg3, 0xC);
        } else {
            handle = sub_801A878(arg0, arg1, arg2, arg3, 0xA);
        }
        sub_80234E8(gUnknown_030012C0, handle);
    } else {
        struct actor *part = sub_8008434(arg0, arg1, arg2, arg3);
        void *p2 = *(void **)gUnknown_030012D0;
        void *field0 = *(void **)p2;
        s32 mask;
        u8 *bitfield;
        s32 old;
        s32 flags;

        *(void **)((u8 *)part + 0x20) = (u8 *)field0 + 0x180;
        *((u8 *)part + 0x2d) = 6;
        sub_80087C0(part);
        sub_80087B4(part);
        sub_800872C(part, 0);
        {
            s32 result = sub_800815C(part);

            bitfield = (u8 *)part + 0x29;
            mask = result & 0xf;
        }
        old = *bitfield;
        old &= -0x10;
        old |= mask;
        *bitfield = old;
        part->field_0A = bit;
        sub_8008E94(gUnknown_030012EC, part);
        flags = part->flags;
        flags &= -5;
        part->flags = flags;
    }
}

/* Same shape as `sub_8020E84` above (twin sibling, tests bit 3 of
 * `gUnknown_030012C0+2`), sound ids `9`/`0xC`, tag value `8`. Same
 * not-yet-byte-matching register-allocation gap. */
void sub_802117C(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    u8 bit = *((u8 *)gUnknown_030012C0 + 2) & 8;

    if (bit) {
        s32 handle;

        if (sub_8023278(gUnknown_030012C0) || *((u8 *)gUnknown_030012C0 + 0x8c) != 0) {
            handle = sub_801A878(arg0, arg1, arg2, arg3, 0xC);
        } else {
            handle = sub_801A878(arg0, arg1, arg2, arg3, 9);
        }
        sub_80234E8(gUnknown_030012C0, handle);
    } else {
        struct actor *part = sub_8008434(arg0, arg1, arg2, arg3);
        void *p2 = *(void **)gUnknown_030012D0;
        void *field0 = *(void **)p2;
        s32 mask;
        u8 *bitfield;
        s32 old;
        s32 flags;

        *(void **)((u8 *)part + 0x20) = (u8 *)field0 + 0x180;
        *((u8 *)part + 0x2d) = 8;
        sub_80087C0(part);
        sub_80087B4(part);
        sub_800872C(part, 0);
        {
            s32 result = sub_800815C(part);

            bitfield = (u8 *)part + 0x29;
            mask = result & 0xf;
        }
        old = *bitfield;
        old &= -0x10;
        old |= mask;
        *bitfield = old;
        part->field_0A = bit;
        sub_8008E94(gUnknown_030012EC, part);
        flags = part->flags;
        flags &= -5;
        part->flags = flags;
    }
}
asm(".align 2, 0");
#endif /* NON_MATCHING */
