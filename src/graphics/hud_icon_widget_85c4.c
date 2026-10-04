#include "core.h"
#include "icon_manager.h"

/* GitHub issue #46: the HUD icon/text widget's glyph drawer and its two
 * constructors. Built with old_agbcc: under agbcc, FontDrawGlyph derives
 * its bitfield masks differently. */
extern s32 _call_via_r2(void *arg0, s32 arg1, void *arg2);

extern void CpuSet(void *src, void *dst, s32 control);
extern void sub_8006AC8(void *arg0, void *arg1);
extern struct oam_shadow_buffer *gUnknown_03001300;
extern u8 gSmallFontChars[];
extern u8 gSmallFontGlyphs[];
extern u8 gFontVtable[];
extern u8 gSmallFontVtable[];
extern u8 gLargeFontVtable[];
extern u8 gSmallFontTiles[];
extern u8 gLargeFontTiles[];
extern u8 gLargeFontChars[];
extern u8 gLargeFontGlyphs[];

/* `icon_manager.oam_scratch` viewed as the OAM-shaped draw request
 * sub_8006AC8 consumes: attr0's Y byte and 2-bit shape, attr1's 9-bit X
 * and 2-bit size, attr2's 10-bit tile number. */
struct glyph_oam
{
    u8 y;
    u8 unk_1:6;
    u8 shape:2;
    u16 x:9;
    u16 unk_2:5;
    u16 size:2;
    u16 tile:10;
    u16 unk_4:6;
};

/* The value arrives as a parameter so old_agbcc loads the 0x1ff mask
 * from the literal pool, as the ROM does. */
static inline void SetGlyphX(struct glyph_oam *oam, s32 x)
{
    oam->x = x;
}

/* Builds one glyph's draw request in `self->oam_scratch` from
 * `glyphRecords[charLookup[charByte]]` and the cursor, draws it with
 * sub_8006AC8, then advances `posX` by the glyph's width. */
void FontDrawGlyph(struct icon_manager *self, u8 charByte)
{
    struct glyph_oam *oam = (struct glyph_oam *)self->oam_scratch;
    u8 glyph = self->charLookup[charByte];

    SetGlyphX(oam, self->posX);
    {
        u8 *posY = (u8 *)&self->posY;

        oam->y = self->glyphRecords[glyph].yOffset + *posY;
    }
    oam->shape = self->glyphRecords[glyph].shape;
    oam->tile = self->tileBase + glyph * self->glyphTileStride;
    sub_8006AC8(gUnknown_03001300, self);
    self->posX += self->glyphRecords[glyph].width;
}

/* The part both widget constructors share: resets the cursor, left
 * margin and tileCount, zeroes the OAM scratch buffer, and points
 * `record` at gFontVtable (which the constructors then
 * overwrite). */
static inline void InitIconManager(struct icon_manager *self)
{
    u32 zero;

    self->record = (struct icon_record *)gFontVtable;
    self->posX = self->posY = 0;
    self->marginX = 0;
    self->tileCount = 0;
    zero = 0;
    CpuSet(&zero, self, CPU_SET_32BIT | CPU_SET_SRC_FIXED | 2);
}

/* Constructs the "A" widget: 9-pixel lines, 4-pixel spaces, glyph
 * stride 2, and a charLookup built from the gSmallFontChars font
 * order table (see include/icon_manager.h). */
struct icon_manager *InitSmallFont(struct icon_manager *self)
{
    u32 i;
    u32 j;

    InitIconManager(self);
    self->record = (struct icon_record *)gSmallFontVtable;
    self->lineHeight = 9;
    self->spaceWidth = 4;
    self->tiles = gSmallFontTiles;
    self->glyphTileStride = 2;
    self->glyphRecords = (struct icon_glyph_metrics *)gSmallFontGlyphs;
    for (i = 0; i <= 0xff; i++)
    {
        self->charLookup[i] = 0;
        for (j = 0; j <= 0x4f; j++)
        {
            if (gSmallFontChars[j] == i)
            {
                self->charLookup[i] = j;
                break;
            }
        }
    }
    return self;
}

/* Constructs the "B" widget: 16-pixel lines, 6-pixel spaces, glyph
 * stride 4, OBJ size 1, and a charLookup built from the
 * gLargeFontChars font order table. */
struct icon_manager *InitLargeFont(struct icon_manager *self)
{
    u32 i;
    u32 j;

    InitIconManager(self);
    self->record = (struct icon_record *)gLargeFontVtable;
    self->lineHeight = 0x10;
    self->spaceWidth = 6;
    self->glyphTileStride = 4;
    self->glyphRecords = (struct icon_glyph_metrics *)gLargeFontGlyphs;
    self->tiles = gLargeFontTiles;
    ((struct glyph_oam *)self->oam_scratch)->size = 1;
    for (i = 0; i <= 0xff; i++)
    {
        self->charLookup[i] = 0;
        for (j = 0; j <= 0x4b; j++)
        {
            if (gLargeFontChars[j] == i)
            {
                self->charLookup[i] = j;
                break;
            }
        }
    }
    return self;
}

/* FontPutChar is matched, byte-exact.
 *
 * Per-character dispatcher used while drawing/measuring one glyph at a
 * time: newline resets `posX` to the left margin and advances `posY` by
 * one line height; space just advances `posX` by `spaceWidth`; anything
 * else is forwarded to `record`'s slot-4 trampoline (the glyph-draw
 * callee, `FontDrawGlyph` per the widget's own vtable) via `_call_via_r2`.
 *
 * The 3-way `if`/`else if`/`else` is written as explicit `goto`s so the
 * *middle* arm (the space case) ends up inline and the other two become
 * jumped-to blocks in test order, with the shared two-instruction tail
 * ("dest += self->offset") reached via a `destAddr`/`offset` pair - this
 * reproduces the ROM's exact block layout.
 *
 * The `self`/`charByte` prologue needed a literal inline-asm block: this
 * compiler always widens a `u8` parameter (`charByte`) to its
 * zero-extended byte value before doing anything else, regardless of
 * where that's first used in the C source, while a pointer parameter
 * (`self`) is only materialized into its own register lazily, at first
 * use - so plain C, however reordered or register-pinned, only ever
 * produced the widen-then-copy order, never the ROM's copy-then-widen.
 * Taking `charByte` as a raw `u32` (avoiding the implicit byte-promotion
 * invariant entirely) and spelling out all three prologue instructions
 * as one asm block - self-copy first, then the in-place `lsl`/`lsr`
 * widen matching the ROM's own register reuse (`lsl r1,r1,#0x18` in
 * place, not into a fresh register) - fixed it. */
void FontPutChar(struct icon_manager *self, u32 charByte)
{
    register u32 raw asm("r1") = charByte;
    register struct icon_manager *s asm("r3");
    register u32 c asm("r4");
    register u32 *destAddr asm("r2");
    register u32 offset asm("r0");

    asm volatile(
        "add %0, %3, #0\n\t"
        "lsl %2, %2, #0x18\n\t"
        "lsr %1, %2, #0x18"
        : "=r"(s), "=r"(c), "+r"(raw)
        : "r"(self)
    );
    if (c == '\n')
        goto newline;
    if (c != ' ')
        goto dispatch;
    destAddr = &s->posX;
    offset = (u8 *)&s->spaceWidth - (u8 *)s;
    goto tail;
newline:
    /* Chained address anchor: the ROM computes `&marginX` as
     * `&posX + 8` (sharing the `0x88<<1` offset register), not as two
     * independent field-offset computations. */
    {
        register u32 *posXAddr asm("r1");
        register u32 *field118Addr asm("r0");

        asm volatile(
            "mov r2, #0x88\n\tlsl r2, r2, #1\n\tadd %0, %2, r2\n\t"
            "add r2, r2, #8\n\tadd %1, %2, r2"
            : "=r"(posXAddr), "=r"(field118Addr)
            : "r"(s)
            : "r2"
        );
        *posXAddr = *field118Addr;
    }
    asm volatile(
        "mov r0, #0x8a\n\tlsl r0, r0, #1\n\tadd %0, %2, r0\n\t"
        "add r0, r0, #8"
        : "=r"(destAddr), "=r"(offset)
        : "r"(s)
    );
tail:
    {
        register u32 *fieldAddr asm("r1") = (u32 *)((u8 *)s + offset);
        *destAddr += *fieldAddr;
    }
    return;
dispatch:
    {
        struct icon_slot *slot = &s->record->slots[4];
        _call_via_r2((u8 *)s + slot->offset, c, slot->ptr);
    }
}
