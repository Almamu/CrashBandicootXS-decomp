#include "core.h"
#include "level_state.h"

/* Same `struct aabb` shape as src/util/aabb.c/actor_part*.c -
 * duplicated here rather than shared, matching this project's existing
 * per-file convention for this struct (see docs/workflow.md and
 * sprite.c etc). */
struct aabb {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_c;
};

/* Set-size primitive - already referenced by name from several other
 * files (sprite.c/power_dialog_draw.c's DrawPowerDialog) as the
 * shared `SetAabbPos`(set-position)/`SetAabbSize`(set-size) pair. */
void SetAabbSize(struct aabb *dest, s32 w, s32 h)
{
    dest->field_8 = w;
    dest->field_c = h;
}
asm(".align 2, 0");

/* Set-position primitive, see SetAabbSize above. */
void SetAabbPos(struct aabb *dest, s32 x, s32 y)
{
    dest->field_0 = x;
    dest->field_4 = y;
}
asm(".align 2, 0");

/* Lives getter of the level state (`gLevelState`; read by
 * game_loop55.c, hud_lives.c, actor_part101.c and
 * graphics_loading_1e990.c). */
s32 GetLives(struct level_state *self)
{
    return self->lives;
}

extern u8 gLargeFontVtable[];
extern u8 gSmallFontVtable[];
extern u8 gFontVtable[];
extern void OperatorDelete(void *self);

/* Both DestroyLargeFont/DestroySmallFont below are per-type descriptor
 * constructors - the same "set one field of a passed-in struct to a
 * ROM data pointer, then conditionally call OperatorDelete based on a bit
 * in the second argument" shape documented at length in docs/rom_map.md
 * for the ~93-entry gEntityVtable-family table (these three
 * entries - gLargeFontVtable/4D64/4DAC, each 0x48 bytes - are
 * further members of that same family). Unusually, each writes to
 * `self+0x130` TWICE in a row with two DIFFERENT table pointers, the
 * second immediately clobbering the first - a genuinely dead first
 * store that's really in the ROM (confirmed: the two address
 * computations and both stores are distinct instructions, not a
 * disassembly artifact). The address is recomputed fresh for each
 * store via an inline-asm anchor (matching power_dialog_draw.c's established
 * technique for stopping gcc from CSE-ing/dead-store-eliminating a
 * repeated address expression, see docs/matching.md, "Matching
 * decompilation") - plain double `struct` field assignment collapses
 * to a single store no matter how it's phrased, tried first and
 * confirmed to regress before reaching for inline asm. */
void DestroyLargeFont(void *self, u32 flags)
{
    void **addr;

    asm volatile("mov r0, #0x98\n\tlsl r0, r0, #1\n\tadd %0, %1, r0" : "=r"(addr) : "r"(self) : "r0");
    *addr = gLargeFontVtable;
    asm volatile("mov r0, #0x98\n\tlsl r0, r0, #1\n\tadd %0, %1, r0" : "=r"(addr) : "r"(self) : "r0");
    *addr = gFontVtable;
    if (flags & 1) {
        OperatorDelete(self);
    }
}

/* Same shape as DestroyLargeFont above, different first table pointer. */
void DestroySmallFont(void *self, u32 flags)
{
    void **addr;

    asm volatile("mov r0, #0x98\n\tlsl r0, r0, #1\n\tadd %0, %1, r0" : "=r"(addr) : "r"(self) : "r0");
    *addr = gSmallFontVtable;
    asm volatile("mov r0, #0x98\n\tlsl r0, r0, #1\n\tadd %0, %1, r0" : "=r"(addr) : "r"(self) : "r0");
    *addr = gFontVtable;
    if (flags & 1) {
        OperatorDelete(self);
    }
}
