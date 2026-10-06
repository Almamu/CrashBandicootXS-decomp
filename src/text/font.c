#include "core.h"
#include "match.h"
#include "text.h"
#include <agb_syscall.h>
#include <libgcc.h>
#include "system.h"
#include "gfx.h"
#include "crates.h"
#include "globals.h"

/* Sits between FontMeasureText (src/text/font_measure.c) and
 * InitFont (src/text/font.c) - FontUploadTiles/
 * FontSetPalette/FontResetPalette, GitHub issue #46. Same `struct bitmap_font`
 * as hud_slide.c and the other src/text/font*.c files. */

/* codegen: GetPaletteSlot returns u8 (gfx.h); with the u8 return
 * FontUploadTiles adds `lsl #0x18; lsr #0x14` where the ROM has one
 * `lsl #4`. docs/headers_plan.md */
extern s32 GetPaletteSlot_s32(u8 *cache, s32 recordId) asm("GetPaletteSlot");

/* Uploads `tiles`'s referenced tile data to the OBJ VRAM slot
 * selected by `tileBase`, recording the resulting tile-count-derived
 * shift (`>>13` of the asset's own header word) into `tileCount`. */
void FontUploadTiles(struct bitmap_font *self)
{
    void *asset = self->tiles;

    self->tileCount = *(u32 *)asset >> 13;
    LoadTaggedAsset(asset, (void *)(0x06010000 + (self->tileBase << 5)));
}

/* Sets the low nibble of `oam_scratch[5]` from `val`'s low byte - a
 * priority/attribute nibble selector, exact meaning not established. */
void FontSetPalette(struct bitmap_font *self, u8 val)
{
    u32 shifted;
    MATCH_HOLD_REG(u8, mask, r2);
    MATCH_HOLD_REG(u8, field, r3);

    shifted = val << 4;
    mask = 0xF;
    MATCH_KEEP_VOLATILE(mask);
    field = self->oam_scratch[5];
    mask &= field;
    mask |= shifted;
    self->oam_scratch[5] = mask;
}

/* Looks up a tile-cache slot for the byte at
 * `(**gSpriteBankSet)[0x1A4]`'s own `+0x14` field (see
 * docs/rom_map.md's `gSpriteBankTable` investigation) via
 * `GetPaletteSlot`, and folds the result into the same `oam_scratch[5]`
 * nibble FontSetPalette sets above. */
void FontResetPalette(struct bitmap_font *self)
{
    u8 *cache = (u8 *)gPaletteCache;
    void *rec = *(void **)(SPRITE_BANK_BASE + (0xD2 << 1));
    u8 field = ((u8 *)rec)[0x14];
    s32 slot = GetPaletteSlot_s32(cache, field);
    u32 shifted = slot << 4;
    MATCH_HOLD_REG(u8, mask, r1);
    MATCH_HOLD_REG(u8, b, r2);

    mask = 0xF;
    MATCH_KEEP_VOLATILE(mask);
    b = self->oam_scratch[5];
    mask &= b;
    mask |= shifted;
    self->oam_scratch[5] = mask;
}

/* Constructor variant used for a widget that's never assigned its own
 * data tables past `record` (the line-height/space-width/glyph fields
 * stay whatever the caller already set) - just the OAM-scratch zero and
 * cursor/margin reset shared with InitSmallFont/B
 * (src/text/font_glyph.c).
 *
 * The `posX`/`posY`/`marginX`/`tileCount` zero-init needed the same
 * inline-asm address anchor as InitSmallFont/B - see that
 * function's own comment for the full account of why. Unlike A/B, there
 * is no charLookup-building loop here, so this one reaches a full
 * byte-exact match. */
struct bitmap_font *InitFont(struct bitmap_font *selfArg)
{
    MATCH_HOLD_REG(struct bitmap_font *, self, r4) = selfArg;
    s32 zero;
    struct icon_record **recordAddr;

    // clang-format off
    asm volatile("mov r0, #0x98\n\tlsl r0, r0, #1\n\tadd %0, %1, r0" : "=r"(recordAddr) : "r"(self) : "r0");
    // clang-format on
    *recordAddr = (struct icon_record *)gFontVtable;

    /* `zero`'s own store (`str r1, [sp]` right before the call) reuses
     * this same zeroed r1 too, instead of materializing a fresh 0 -
     * folded into this same asm block so the compiler can't tell them
     * apart and reload. */
    // clang-format off
    asm volatile(
        "mov r1, #0x88\n\tlsl r1, r1, #1\n\tadd r2, %1, r1\n\t"
        "add r1, r1, #4\n\tadd r0, %1, r1\n\t"
        "mov r1, #0\n\tstr r1, [r0]\n\tstr r1, [r2]\n\t"
        "mov r2, #0x8c\n\tlsl r2, r2, #1\n\tadd r0, %1, r2\n\t"
        "str r1, [r0]\n\t"
        "add r2, r2, #0x14\n\tadd r0, %1, r2\n\t"
        "str r1, [r0]\n\t"
        "str r1, %0"
        : "=m"(zero)
        : "r"(self)
        : "r0", "r1", "r2", "memory"
    );
    // clang-format on

    CpuSet(&zero, self, CPU_SET_SRC_FIXED | CPU_SET_32BIT | 2);
    return self;
}

/* Sits between the parked InitFont (asm/code_3_2_20_8a78.s) and the
 * rest of the still-raw HUD text/icon-widget driver code
 * (asm/code_3_2_20_8b7c.s, out of GitHub issue #46's chunk scope) -
 * FontHeightToLines through FontSetTileBase, GitHub issue #46. Same
 * `struct bitmap_font` as hud_slide.c and the other
 * src/text/font*.c files.
 *
 * DestroyFont (0x08028B7C, no tracked issue - just the next
 * function in ROM order, immediately after FontSetTileBase above) is
 * folded in here too rather than getting its own object file, per
 * docs/workflow.md's "one .c file per contiguous ROM region"
 * convention: same `struct bitmap_font`/`record` field this whole
 * file already documents. It is the base font's destructor (vtable
 * slot 1 of gFontVtable; it was once read as a constructor):
 * (`record = &gFontVtable`, then the same conditional
 * `OperatorDelete(self)` teardown-registration idiom
 * DestroyLargeFont/DestroySmallFont (src/util/aabb_setup.c) and
 * InitFont above use elsewhere for the same table). Matched
 * byte-exact via plain struct field access - unlike InitFont's own
 * record write, this one didn't need the inline-asm address anchor
 * those functions use (confirmed by a full clean `make compare`): with
 * only one address computation total in the whole function (nothing
 * else contends for it), gcc's own codegen already lands the `self+
 * offsetof(record)` add in r2 exactly like the ROM. */

extern void *_call_via_r1(void *arg0, void *arg1);

/* Divides `value` by the widget's own line height (`lineHeight`) - see
 * src/text/text_box.c's DrawWrappedTextInBox, which uses this same field as a
 * divisor for a line-count limit. */
s32 FontHeightToLines(struct bitmap_font *self, s32 value)
{
    return __udivsi3(value, self->lineHeight);
}

/* Trivial getter/setter pairs around `struct bitmap_font`'s fields -
 * used by callers elsewhere in the still-raw HUD text/icon-widget
 * driver code. */
u32 FontGetTileCount(struct bitmap_font *self)
{
    return self->tileCount;
}

void FontSetPos(struct bitmap_font *self, u32 x, u32 y)
{
    self->posX = x;
    self->posY = y;
}

void FontNewLineAt(struct bitmap_font *self, u32 y)
{
    self->posX = self->marginX;
    self->posY = y;
}

u32 FontGetMargin(struct bitmap_font *self)
{
    return self->marginX;
}

void FontSetMargin(struct bitmap_font *self, u32 val)
{
    self->marginX = val;
}

u32 FontGetY(struct bitmap_font *self)
{
    return self->posY;
}

u32 FontGetX(struct bitmap_font *self)
{
    return self->posX;
}

/* Sets `tileBase`, then forwards to `record`'s slot-6 trampoline (see
 * include/bitmap_font.h's `struct icon_record`) via `_call_via_r1`,
 * discarding its result. */
void FontSetTileBase(struct bitmap_font *self, u32 val)
{
    struct icon_slot *slot;

    self->tileBase = val;
    slot = &self->record->slots[6];
    _call_via_r1((u8 *)self + slot->offset, slot->ptr);
}

void DestroyFont(struct bitmap_font *self, u32 flags)
{
    self->record = (struct icon_record *)gFontVtable;
    if (flags & 1) {
        OperatorDelete(self);
    }
}
