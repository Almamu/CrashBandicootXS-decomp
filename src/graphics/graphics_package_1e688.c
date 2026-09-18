#include "core.h"

/* Same 0x40C-byte OAM shadow buffer `src/graphics/graphics.c` already
 * names `struct oam_shadow_buffer` (redeclared locally per this
 * project's minimal-local-type convention - see docs/naming.md). */
struct oam_shadow_buffer {
    s32 count;
    s32 field_04;
    s32 field_08;
    u8 table[0x400];
};

extern struct oam_shadow_buffer *gUnknown_03001300;
extern void sub_8006AC8(struct oam_shadow_buffer *arg0, u32 *arg1);
extern s32 sub_803ADB4(s32 a, s32 b);

/* The same 91-entry-family, 12-entry "box preset" pair
 * `docs/rom_map.md` ties to `sub_801E788`'s centering math - see that
 * function's comment below for what's confirmed about them. */
extern s32 gStaticData_0816C644[12];
extern s32 gStaticData_0816C674[12];

/* GitHub issue #30's `sub_801E688`/`sub_801E788` - `LoadGraphicsPackage`'s
 * tile-cell-selection and viewport-centering helpers, already
 * characterized (but not carried to C) in docs/rom_map.md. Both operate
 * on the same "self" object `sub_801E640`/`sub_801E644`/`sub_801E8F8`/
 * `sub_801E964`/`sub_801E96C` (graphics_package_1e640.c/_1e8f8.c/_1e964.c)
 * build up - NOT the small 0x10-byte `LoadGraphicsPackage` scratch
 * buffer itself (that buffer never grows past +0xd), but a *larger*
 * caller-owned object that embeds it: every caller of these two
 * functions allocates something bigger (`u8 buf[0x70]`/`buf[0xe0]` in
 * the `settings_menu*.c` family, never the plain `buf[0x10]` the
 * `LoadGraphicsPackage`-only callers use) with room for the extra
 * fields these two touch (+0x10 through +0x24) - a hardware-OAM-attribute
 * template (+0x10..+0x17: attr0/attr1/attr2/filler) plus a handful of
 * scratch fields private to this pair. */

/* Picks the smallest-area entry from the shared 12-slot
 * `gStaticData_0816C644`/`674` "box preset" table (width/height pairs)
 * that's still large enough to hold a `width`x`height` box (each table
 * entry must be at least half of the requested dimension, i.e. the
 * request must fit at "50% zoom or better") - a best-fit box-size
 * selector. Stores the winning table index at `self+0x18` (split
 * across `self+0x13` bits 6-7 = index bits 0-1, `self+0x11` bits 6-7 =
 * index bits 2-3 - the same byte-packed shape `sub_801E788` below
 * unpacks), a "tile index" derived from the winning box's area at
 * `self+0x14` bits 0-9 (`0x400 - area/32`, clamped to 10 bits - reused
 * as the OAM `attr2` tile-index field by `sub_801E788`'s
 * `sub_8006AC8` insertion), and two Q8.8 fixed-point scale factors
 * (`self+0x20`/`self+0x24`, `sub_803ADB4`-divided: box-dimension<<8
 * over the requested dimension) that `sub_801E788` writes into the
 * shadow OAM buffer's affine-parameter overlay as a pure-scale
 * (no-rotation) 2x2 matrix. The trailing scale-classification write to
 * `self+0x11` bits 0-1 picks between "needs the scaled/clamped OAM
 * path" (3, when either scale factor undershoots 0x100 i.e. shrinks),
 * "oversized on X" (1), "exact/undersized on Y only" (0, when X is
 * exactly 1.0 and Y doesn't exceed 1.0), or leaves the two bits
 * untouched (X exactly 1.0, Y over 1.0) - `sub_801E96C`
 * (graphics_package_1e964.c) is the "reset before rebuild" pair that
 * clears this same byte's bits 4-9 beforehand, so a caller can rebuild
 * it field-by-field across several of these helpers.
 *
 * Parked (`NON_MATCHING`), not matched: every operation here is
 * confirmed against the ROM (cross-checked byte-for-byte against
 * `asm/code_3_2_17_1e644.s`'s disassembly), but this is a
 * register-starved ~110-instruction function using all of r0-r8/sb/sl/
 * ip simultaneously in its search loop (the same "every register
 * committed at once" shape as `LoadGraphicsPackage`/`sub_801E644`
 * elsewhere in this cluster) - not attempted byte-exact this pass; see
 * docs/matching/issue-30-graphics-loading.md's "Fourth pass" for what
 * was tried on this function's sibling below and why the same class of
 * gcc-2.9 register-allocation gap is expected here too. */
#if NON_MATCHING
void sub_801E688(u8 *self, s32 arg1, s32 arg2)
{
    s32 bestArea = 0x1000;
    s32 bestIdx = 0;
    s32 i;
    s32 scaleX, scaleY;

    *(u32 *)(self + 8) = arg1;
    *(u32 *)(self + 0xc) = arg2;

    for (i = 0; i <= 0xb; i++) {
        s32 cellWidth = gStaticData_0816C644[i];
        s32 cellHeight;
        s32 area;

        if (arg1 > cellWidth * 2) {
            continue;
        }
        cellHeight = gStaticData_0816C674[i];
        if (arg2 > cellHeight * 2) {
            continue;
        }
        area = cellWidth * cellHeight;
        if (area >= bestArea) {
            continue;
        }
        bestArea = area;
        bestIdx = i;
        *(u32 *)(self + 0x18) = bestIdx;
    }

    self[0x13] = (self[0x13] & 0x3f) | ((bestIdx & 3) << 6);
    self[0x11] = (self[0x11] & 0x3f) | (((bestIdx >> 2) & 3) << 6);

    {
        /* `0x400 - bestArea/32`, rounded toward zero, clamped to 10
         * bits - becomes the shadow-OAM `attr2` tile-index field
         * `sub_801E788` inserts below. */
        s32 v = bestArea;

        if (v < 0) {
            v += 0x1f;
        }
        v >>= 5;
        v = 0x400 - v;
        *(u16 *)(self + 0x14) = (*(u16 *)(self + 0x14) & 0xFC00) | (v & 0x3ff);
    }

    scaleX = sub_803ADB4(gStaticData_0816C644[bestIdx] << 8, arg1);
    *(u32 *)(self + 0x20) = scaleX;
    scaleY = sub_803ADB4(gStaticData_0816C674[bestIdx] << 8, arg2);
    *(u32 *)(self + 0x24) = scaleY;

    if (scaleX <= 0xff || scaleY <= 0xff) {
        self[0x11] = (self[0x11] & ~3) | 3;
    } else if (scaleX > 0x100) {
        self[0x11] = (self[0x11] & ~3) | 1;
    } else if (scaleY <= 0x100) {
        self[0x11] = self[0x11] & ~3;
    }
}
#endif /* NON_MATCHING */

/* Computes `self`'s on-screen position from one of four modes packed
 * into `self+0x11` bits 0-1 (mode 2 is a no-op: no position math at
 * all), then unconditionally inserts `self`'s pre-built OAM attribute
 * template (`self+0x10..+0x17`) into the shadow OAM buffer
 * (`gUnknown_03001300`, `sub_8006AC8`) - and, unless mode was 0
 * (raw/no-centering, which just clears the affine-enable bits at
 * `self+0x13` bits 4-5), also allocates one affine-parameter group
 * from the shadow buffer's `field_08` counter and writes a pure-scale
 * (no rotation) 2x2 affine matrix into it from `sub_801E688`'s
 * `self+0x20`/`self+0x24` Q8.8 factors:
 *  - Mode 0: `self+0x12` (the OAM `attr1` field's low 9 bits, X
 *    position) = `self+0x00` as-is; `self+0x10` (`attr0` low byte, Y
 *    position) = `self+0x04` as-is - no centering, position copied
 *    straight through.
 *  - Mode 1: centers within table entry `self+0x18`'s box: X position
 *    = `self+0x00` minus half of `(gStaticData_0816C644[idx] -
 *    self+0x08)`; Y position = `self+0x04` minus half of
 *    `(gStaticData_0816C674[idx] - self+0x0c)`.
 *  - Mode 3: the mirror of mode 1 - X position = `self+0x00` plus half
 *    of `self+0x08`, minus the table's own X reference; Y position =
 *    `self+0x04` plus half of `self+0x0c`, minus the table's Y
 *    reference.
 *
 * The affine-group allocation resolves a real open question flagged by
 * this issue's third pass: the writes through `gUnknown_03001300` at
 * offsets that looked like they didn't fit the shadow buffer's 8-byte
 * hardware-OAM-entry stride actually do - `field_08 * 0x20 + 0x12`
 * (and its three siblings 8/16/24 bytes further on) is exactly
 * `(field_08 * 4 + 0/1/2/3) * 8 + 6`, i.e. the *filler* halfword (byte
 * offset +6) of four consecutive shadow-OAM entries starting at
 * `field_08 * 4` - real hardware overlays the OBJ affine-parameter
 * memory (PA/PB/PC/PD) on exactly that halfword of every 4th OAM
 * entry. The four writes here (scaleX, 0, 0, scaleY) are PA/PB/PC/PD
 * of a diagonal (no-rotation) scale-only matrix, and `self+0x13` bits
 * 1-5 (packed alongside the position bits `self+0x12`'s high byte
 * already holds) become that affine group's 5-bit selector index in
 * the sprite's own `attr1` field - a coherent, understood mechanism,
 * not a layout mismatch.
 *
 * Parked (`NON_MATCHING`), not matched: semantics and control flow
 * (branch order, mask constants, the `field_08`-delta-materialized
 * third clear-mask) all confirmed instruction-for-instruction against
 * `asm/code_3_2_17_1e644.s`, and heavy iteration (the negative-constant
 * clear-mask idiom, a `struct oam_shadow_buffer **addr = &gUnknown_03001300`
 * address cache matching `graphics_loading_21d80.c`'s established
 * pattern, explicit register pins for the loop-scoped `slot` value)
 * closed every gap but one: the ROM keeps `self` in `r7` for the whole
 * function (matching its 4-register `push {r4-r7}` list), but this
 * compiler only reaches that same total register count - and, more
 * importantly, only keeps `self+offset` dereferences compiled as a
 * single `ldrb/ldrh/ldr rX,[r7,#imm]` instruction - when `self` is an
 * *ordinary* (non-`register`) local. The moment `self` is pinned to a
 * specific hard register via `register u8 *self asm("r7")` (confirmed
 * with a minimal one-line repro: `return self[0x11];` alone), this
 * compiler stops folding the offset into the load/store's immediate
 * field and instead always emits a separate `add rX, rX, #imm` before
 * a zero-offset dereference - a real, reproducible gcc-2.9/agbcc
 * limitation for asm-register-pinned pointer locals, not something any
 * tried C-level restructuring routes around. Since the ROM's own bytes
 * hard-require `self` to be exactly `r7` (every dereference's encoding
 * depends on the register number), and reaching `r7` here is only
 * possible through that same pin, this function's `self`-register
 * placement is unreachable from portable C under this compiler - the
 * same category of first-pass-vs-second-pass register-pressure
 * artifact already documented for `LoadGraphicsPackage`/`sub_801E644`
 * elsewhere in this cluster, just manifesting through the addressing
 * mode instead of a dropped push/pop pair this time. Worth recording
 * as a new, previously-undocumented flavor of the gotcha for whoever
 * hits it next. */
#if NON_MATCHING
void sub_801E788(u8 *selfArg)
{
    u8 *self = selfArg;
    s32 mode = self[0x11] & 3;

    if (mode == 1) {
        s32 idx = *(u32 *)(self + 0x18);
        s32 a = gStaticData_0816C644[idx] - *(u32 *)(self + 8);
        a = (a + (s32)((u32)a >> 31)) >> 1;
        {
            s32 pos = *(u32 *)(self + 0) - a;

            pos &= 0x1ff;
            *(u16 *)(self + 0x12) = (*(u16 *)(self + 0x12) & 0xFFFFFE00) | pos;
        }
        {
            s32 b = gStaticData_0816C674[idx] - *(u32 *)(self + 0xc);
            b = (b + (s32)((u32)b >> 31)) >> 1;
            self[0x10] = *(u8 *)(self + 4) - b;
        }
    } else if (mode == 0) {
        {
            s32 pos = *(u32 *)(self + 0) & 0x1ff;

            *(u16 *)(self + 0x12) = (*(u16 *)(self + 0x12) & 0xFFFFFE00) | pos;
        }
        self[0x10] = *(u32 *)(self + 4);
    } else if (mode == 3) {
        s32 idx = *(u32 *)(self + 0x18);
        s32 a = *(u32 *)(self + 8);
        a = (a + (s32)((u32)a >> 31)) >> 1;
        {
            s32 pos = ((*(u32 *)(self + 0) + a) - gStaticData_0816C644[idx]) & 0x1ff;

            *(u16 *)(self + 0x12) = (*(u16 *)(self + 0x12) & 0xFFFFFE00) | pos;
        }
        {
            s32 c = *(u32 *)(self + 0xc);
            c = (c + (s32)((u32)c >> 31)) >> 1;
            self[0x10] = (*(u32 *)(self + 4) + c) - *(u8 *)&gStaticData_0816C674[idx];
        }
    }

    {
        struct oam_shadow_buffer **addr = &gUnknown_03001300;

        if ((self[0x11] & 3) == 0) {
            self[0x13] = self[0x13] & -0x11 & -0x21;
        } else {
            struct oam_shadow_buffer *base = *addr;
            s32 slot = base->field_08;

            base->field_08 = slot + 1;
            {
                s32 result = (-0xf & self[0x13]) | ((slot & 7) << 1);

                result = (result & -0x11) | (((slot >> 3) & 1) << 4);
                result = (result & -0x21) | (((slot >> 4) & 1) << 5);
                self[0x13] = result;
            }

            {
                u8 *entry = (u8 *)base;

                *(u16 *)(entry + slot * 0x20 + 0x12) = *(u16 *)(self + 0x20);
                *(u16 *)(entry + (slot * 4 + 1) * 8 + 0x12) = 0;
                *(u16 *)(entry + (slot * 4 + 2) * 8 + 0x12) = 0;
                entry += (slot * 4 + 3) * 8;
                *(u16 *)(entry + 0x12) = *(u16 *)(self + 0x24);
            }
        }

        sub_8006AC8(*addr, (u32 *)(self + 0x10));
    }
}
#endif /* NON_MATCHING */
