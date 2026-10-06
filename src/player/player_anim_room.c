#include "core.h"
#include "box_part.h"
#include "crates.h"
#include "player.h"
#include "level.h"
#include "globals.h"

/* GitHub issue #9/#10: 0x0800AAEC, the input-action-check function the
 * 42-slot `gActionCtrlStateTable` action-dispatch table's own entries
 * (`ActionCtrlStateSlide` etc.) call for their action codes `0xB`/`0x10` (see
 * docs/rom_map.md). Iterates the `gCrateList` object list (the
 * same count-prefixed `{count, unused_4, items}` layout already
 * established in `src/crates/crate_time_trial.c`'s `ConvertCratesForTimeTrial`), testing
 * each entry's own `+0x18`-table `+0x48` trampoline via `_call_via_r1`
 * (matched elsewhere) and, on a hit (state `3`), calling `PlayerAnimWouldTouchCrate`
 * (this function's own companion, see `crate_touch.c`) with that
 * entry and `x`.
 *
 * Before the loop, gates the whole call on a `ProbeTerrain` proximity/
 * position probe against the player (`gLevelLayers`): builds an
 * integer `{x, y}` position from `self`'s own Q8 position plus the
 * target action `x`'s own keyframe record's box `offY` (added in Q8
 * space before truncating), passes `self`'s horizontal mirror bit as a
 * `1`/`2` direction selector, the box's `h` byte as a third scalar, and
 * a pointer to `self`'s original (untruncated) Q8 `y` for the callee to
 * restore/report through.
 *
 * Matching notes (see docs/matching/archive/issue-9-naked-retry.md): the list
 * walk is the guarded do-while shape (not a `for`), which is what gives
 * the ROM's per-iteration `&gCrateList` literal reload; the
 * position is read as one struct copy (both words loaded, then both
 * stored, then `y` reloaded for `origY`); and the record pointer's `+4`
 * is a separate `rec += 4` step (the ROM's `adds r1, #4; adds r4, r1, #0`
 * pair). Compiles identically under agbcc and old_agbcc. */

struct pos {
    s32 x;
    s32 y;
};

extern s32 _call_via_r1(void *addr, void *fn);

u8 PlayerHasRoomForAnim(struct box_part *self, s32 x)
{
    u8 *rec;
    struct hitbox_quad *box;
    u8 h;
    struct pos pos;
    s32 origY;
    s32 dir;
    s32 i;

    rec = (u8 *)&(*self->keyframes)[x];
    rec += 4;
    box = (struct hitbox_quad *)rec;
    h = box->h;
    pos = *(struct pos *)self;
    origY = self->y;
    if (self->mirrorX)
        dir = 2;
    else
        dir = 1;
    pos.y = pos.y + (box->offY << 8);
    pos.x >>= 8;
    pos.y >>= 8;
    if ((u8)ProbeTerrain(gLevelLayers, dir, (struct probe_pos *)&pos, h, &origY))
        return 0;

    i = 0;
    if (i < gCrateList->activeCount)
        do {
            u8 *entry = (u8 *)gCrateList->slotArray[i];
            u8 *method = *(u8 **)(entry + 0x18) + 0x48;

            if (_call_via_r1(entry + *(s16 *)method, *(void **)(method + 4)) == 3) {
                if (PlayerAnimWouldTouchCrate((struct box_part *)entry, x) == 1)
                    return 0;
            }
            i++;
        } while (i < gCrateList->activeCount);
    return 1;
}
