#include "core.h"
#include "vtable.h"
#include "text.h"
#include "pickups.h"
#include "enemies.h"
#include "frontend.h"
#include "system.h"
#include "menus.h"
#include "crates.h"
#include "player.h"
#include "actor.h"
#include "bosses.h"
#include "vehicle.h"
#include "gfx.h"
#include "objects.h"
#include "level.h"
#include "globals.h"

/*
 * ROM 0x087E3BEC-0x087E55E4: the virtual tables of the game's C++
 * object classes (docs/rom_map.md's "93 entity vtables") that g++
 * doesn't emit yet, in ROM order. The others are emitted by g++ in their
 * class's key-method object (docs/cplusplus.md, "Emitting the
 * vtables"). Each table is in a section of its own (VTABLE_SECTION) so
 * that ldscript.txt can place it at its ROM address, between the emitted
 * ones. Constructors store one at the object's method-table pointer; the
 * code reads the slots through `struct actor_method` (actor_self.h).
 * Tables of the same class family share their leading slots, the ROM's
 * own inheritance. See docs/data.md.
 */

/* Used by aabb_setup.c, font_glyph.c (FontDrawGlyph). */
const struct vtable_slot gLargeFontVtable[9] VTABLE_SECTION(gLargeFontVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyLargeFont),
    VTABLE_SLOT(FontMeasureText),
    VTABLE_SLOT(FontMeasureChars),
    VTABLE_SLOT(FontDrawText),
    VTABLE_SLOT(FontDrawChars),
    VTABLE_SLOT(FontDrawGlyph),
    VTABLE_SLOT(FontPutChar),
    VTABLE_SLOT(FontUploadTiles),
};

/* Used by aabb_setup.c, font_glyph.c (FontDrawGlyph). */
const struct vtable_slot gSmallFontVtable[9] VTABLE_SECTION(gSmallFontVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroySmallFont),
    VTABLE_SLOT(FontMeasureText),
    VTABLE_SLOT(FontMeasureChars),
    VTABLE_SLOT(FontDrawText),
    VTABLE_SLOT(FontDrawChars),
    VTABLE_SLOT(FontDrawGlyph),
    VTABLE_SLOT(FontPutChar),
    VTABLE_SLOT(FontUploadTiles),
};

/* Used by aabb_setup.c, font.c (DestroyFont),
 * font_glyph.c (FontDrawGlyph), font.c. */
const struct vtable_slot gFontVtable[9] VTABLE_SECTION(gFontVtable) = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyFont),
    VTABLE_SLOT(FontMeasureText),
    VTABLE_SLOT(FontMeasureChars),
    VTABLE_SLOT(FontDrawText),
    VTABLE_SLOT(FontDrawChars),
    VTABLE_SLOT(FontDrawGlyph),
    VTABLE_SLOT(FontPutChar),
    VTABLE_SLOT(FontUploadTiles),
};
