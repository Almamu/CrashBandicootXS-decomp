#include "core.h"
#include "icon_manager.h"

/* NOT YET BYTE-MATCHING - see docs/matching/issue-46-hud-icon-widget.md
 * for the full account; compiled only under `make NON_MATCHING=1`, the
 * checked-in assembly (asm/code_3_2_20_85c4.s) is used otherwise. */
#if NON_MATCHING

extern s32 sub_803AD80(void *arg0, s32 arg1, void *arg2);
extern void sub_803A94C(void *src, void *dst, s32 control);
extern void sub_8006AC8(void *arg0, void *arg1);
extern struct oam_shadow_buffer *gUnknown_03001300;
extern u8 gStaticData_08174D84[];
extern u8 gStaticData_08174DD4[];
extern u8 gStaticData_087E4DAC[];
extern u8 gStaticData_087E4D64[];
extern u8 gStaticData_087E4D1C[];
extern u8 gStaticData_085A4E70[];
extern u8 gStaticData_085A551C[];
extern u8 gStaticData_08175188[];
extern u8 gStaticData_081751D4[];

/* Builds one glyph's OAM-scratch draw request (`self->oam_scratch`) from
 * `self->glyphRecords[glyphIndex]` and the current cursor position, hands
 * it to `sub_8006AC8` to actually draw, then advances `posX` by the
 * glyph's width. `charByte` is looked up through `charLookup` first -
 * callers pass a raw character byte, not a glyph index.
 *
 * Residual gap: `self` is kept in `ip`/`r12` here instead of the ROM's
 * `r3` - this function has too many simultaneously-live values (the
 * glyph index, three re-derived `rec` pointers, `self` itself across the
 * `sub_8006AC8` call) for this compiler to fit into r4-r7 the way the
 * ROM does; tried caching `&self->posX`/`&self->glyphRecords` into
 * explicit locals (matching the ROM's own address-caching shape) and
 * plain repeated field access, neither changed the register choice. */
void sub_80285C4(struct icon_manager *self, u8 charByte)
{
    u8 glyphIndex = self->charLookup[charByte];
    struct icon_glyph_metrics *rec;

    *(u16 *)((u8 *)self + 2) = (*(u16 *)((u8 *)self + 2) & 0xFE00) | (self->posX & 0x1FF);

    rec = (struct icon_glyph_metrics *)((u8 *)self->glyphRecords + glyphIndex * 12);
    self->oam_scratch[0] = rec->field_8 + *((u8 *)&self->posY);

    rec = (struct icon_glyph_metrics *)((u8 *)self->glyphRecords + glyphIndex * 12);
    self->oam_scratch[1] = (self->oam_scratch[1] & 0x3F) | (u8)(rec->field_4 << 6);

    *(u16 *)((u8 *)self + 4) = (*(u16 *)((u8 *)self + 4) & 0xFC00)
        | ((self->field_108 + glyphIndex * self->field_124) & 0x3FF);

    sub_8006AC8(gUnknown_03001300, self);

    rec = (struct icon_glyph_metrics *)((u8 *)self->glyphRecords + glyphIndex * 12);
    self->posX += rec->width;
}

/* Constructs a `struct icon_manager` for the "A" icon/text widget
 * family: zeroes the leading OAM-scratch pair of words, resets cursor
 * position and left margin, points `record` at `gStaticData_087E4D64`
 * (the first store to `gStaticData_087E4DAC` is a genuinely dead write
 * that's really in the ROM - see actor_aabb_setup.c's
 * sub_803AFF0/sub_803B024 for the identical documented pattern),
 * configures the line-height/space-width/glyph-stride fields, and
 * builds `charLookup` from the `gStaticData_08174D84` font-glyph-order
 * table (see include/icon_manager.h).
 *
 * Residual gap: the `posX`/`posY` zero-init pair - the ROM computes both
 * field addresses in ascending-offset order (sharing the +4 constant
 * delta between them) but stores through them in the opposite order;
 * every C phrasing tried (struct-field order both ways, raw offset
 * casts, a shared base pointer) reproduced one half of that shape but
 * not both together. */
struct icon_manager *InitHudIconWidgetA(struct icon_manager *self)
{
    s32 zero;
    s32 i;
    u8 count;

    self->record = (struct icon_record *)gStaticData_087E4DAC;
    self->posX = 0;
    self->posY = 0;
    self->field_118 = 0;
    self->field_12c = 0;
    zero = 0;
    sub_803A94C(&zero, self, 0x05000002);

    self->record = (struct icon_record *)gStaticData_087E4D64;
    self->field_11c = 9;
    self->spaceWidth = 4;
    self->field_128 = gStaticData_085A4E70;
    self->field_124 = 2;
    self->glyphRecords = (struct icon_glyph_metrics *)gStaticData_08174DD4;

    count = gStaticData_08174D84[0];
    for (i = 0; i <= 0xFF; i++) {
        u8 j;

        self->charLookup[i] = 0;
        if (count == i) {
            continue;
        }
        for (j = 1; j <= 0x4F; j++) {
            if (gStaticData_08174D84[j] == i) {
                self->charLookup[i] = j;
                break;
            }
        }
    }
    return self;
}

/* Same shape as InitHudIconWidgetA above, "B" icon/text widget family -
 * different data tables and line-height/glyph-stride constants. Same
 * residual `posX`/`posY` gap as InitHudIconWidgetA. */
struct icon_manager *InitHudIconWidgetB(struct icon_manager *self)
{
    s32 zero = 0;
    s32 i;
    u8 count;

    self->record = (struct icon_record *)gStaticData_087E4DAC;
    self->posX = 0;
    self->posY = 0;
    self->field_118 = 0;
    self->field_12c = 0;
    sub_803A94C(&zero, self, 0x05000002);

    self->record = (struct icon_record *)gStaticData_087E4D1C;
    self->field_11c = 0x10;
    self->spaceWidth = 6;
    self->field_124 = 4;
    *((u8 *)self + 3) = (*((u8 *)self + 3) & 0x3F) | 0x40;
    self->field_128 = gStaticData_081751D4;
    self->glyphRecords = (struct icon_glyph_metrics *)gStaticData_085A551C;

    count = gStaticData_08175188[0];
    for (i = 0; i <= 0xFF; i++) {
        u8 j;

        self->charLookup[i] = 0;
        if (count == i) {
            continue;
        }
        for (j = 1; j <= 0x4B; j++) {
            if (gStaticData_08175188[j] == i) {
                self->charLookup[i] = j;
                break;
            }
        }
    }
    return self;
}

/* Per-character dispatcher used while drawing/measuring one glyph at a
 * time: newline resets `posX` to the left margin and advances `posY` by
 * one line height; space just advances `posX` by `spaceWidth`; anything
 * else is forwarded to `record`'s slot-4 trampoline (the glyph-draw
 * callee, `sub_80285C4` per the widget's own vtable) via `sub_803AD80`.
 *
 * Residual gap: the ROM lowers this 3-way `if`/`else if`/`else` so the
 * *middle* arm (the space case) ends up inline and the other two become
 * jumped-to blocks in test order, with a shared two-instruction tail
 * ("dest += self->offset") folded out of the newline/space arms; tried
 * a version with that tail hoisted into explicit `destAddr`/`offset`
 * locals (does trigger the tail-merge, confirmed) and a `switch`
 * (produces a different, also-wrong block order) - neither reproduced
 * the exact block layout. */
void sub_8028808(struct icon_manager *self, u8 charByte)
{
    if (charByte == '\n') {
        self->posX = self->field_118;
        self->posY += self->field_11c;
    } else if (charByte == ' ') {
        self->posX += self->spaceWidth;
    } else {
        struct icon_slot *slot = &self->record->slots[4];
        sub_803AD80((u8 *)self + slot->offset, charByte, slot->ptr);
    }
}

#endif /* NON_MATCHING */
