#include "core.h"

/* GitHub issues #9/#10/#41's shared cross-reference: `sub_8009BE0`'s
 * physics/collision step-probe (`src/graphics/actor_part12b.c`, see
 * `docs/matching/naked-spatial-grid-tail.md`) and `sub_800AAEC`'s
 * input-action-check gate (`src/graphics/actor_part108.c`, see
 * `docs/matching/issue-9-10-0x0800aaec-graphics.md`) both flagged this
 * function as "still unexamined" from their own call sites.
 * `docs/rom_map.md` (line ~2472) had already sketched it as "an
 * umbrella 5-mode dispatcher unifying the already-documented
 * `sub_8026AE8`/`sub_8026A18` collision resolvers under one API" - this
 * pass confirms that shape exactly (modulo it being 4 real dispatch
 * arms, not 5 - see below) and closes it as real matched C.
 *
 * `mode` selects one of two axis resolvers and, for three of the four
 * modes, a cheap out-of-range short-circuit that skips the tile scan
 * entirely:
 *
 *   - `mode == 2`: if `pos->x < 0`, clamps `*outValue = 0` and reports
 *     a hit immediately (the probe position is already off the left
 *     edge of the loaded region - nothing to scan). Otherwise calls
 *     `sub_8026AE8(self, pos, span, outValue, 3)`.
 *   - `mode == 1`: reads `self+0x10`'s pointed-to record's own `+0x10`
 *     field (`bounds->maxX` below - a right/loaded-region edge, plain
 *     ints, not Q8, matching `pos`'s own units). If `pos->x` is past
 *     it, clamps `*outValue = bound << 8` and reports a hit without
 *     scanning. Otherwise calls `sub_8026AE8(self, pos, span,
 *     outValue, 1)`.
 *   - `mode == 4`: always calls `sub_8026A18(self, pos, span,
 *     outValue, 2)` - no short-circuit for this arm.
 *   - `mode == 8`: same shape as `mode == 1` but on the Y axis
 *     (`bounds->maxY`, `pos->y`), calling `sub_8026A18(self, pos,
 *     span, outValue, 0)` when not already past the bound.
 *   - any other `mode` (in practice just `0`): returns `0` with no
 *     side effects.
 *
 * `sub_8026AE8`/`sub_8026A18` (both still raw, `asm/code_3_2_17_266bc.s`)
 * are `docs/rom_map.md`'s already-documented "horizontal/vertical
 * collision-resolver pair": each iterates a run of tiles along one axis
 * via `sub_8025130` (matched, `src/system/game_loop3.c`) and, on a
 * solid hit, accumulates a `±(tile_edge_distance << 8)` push-out delta
 * into `*outValue`. Reading their own raw bytes alongside this
 * function confirms which axis each one actually resolves:
 * `sub_8026A18` scans `pos->x` over `[x, x+span-1]>>3` at a fixed
 * `pos->y>>3` row (a horizontal tile-row scan - floor/ceiling
 * detection, i.e. the *vertical*-position resolver, matching this
 * function's own `mode == 4`/`8` -> Y-axis pairing above), while
 * `sub_8026AE8` scans `pos->y` over the same span at a fixed
 * `pos->x>>3` column (a vertical tile-column scan - wall detection,
 * the *horizontal*-position resolver, matching `mode == 1`/`2` ->
 * X-axis above). The `submode` passed as each resolver's 5th
 * (stack) argument selects which of the two push directions along
 * that axis to apply on a hit (`sub_8026AE8`: `1` = push left/negative
 * X, `3` = push right/positive X + a 1-unit epsilon; `sub_8026A18`:
 * `0`/`2` are its own push-up/push-down pair, mirroring the same
 * idiom on the Y axis) - `mode`'s four values (`1`/`2`/`4`/`8`) map to
 * `submode` `1`/`3`/`2`/`0` respectively, a non-monotonic mapping this
 * function's own dispatch just hard-codes per arm rather than deriving
 * arithmetically.
 *
 * Confirmed against both flagged call sites: `sub_8009BE0` passes a
 * plain-int (not Q8) `{x, y}` position it just computed via
 * `sub_8008278`, matching `pos`'s units here; `sub_800AAEC` passes
 * `self+0x28` bit 4 (mirror flag) as a `1`/`2` selector - exactly this
 * function's `mode` values `1`/`2` (the X-axis/`sub_8026AE8` arms) -
 * confirming `mode` really is a small enumerated selector, not a
 * literal bitmask combined at runtime (`mode`'s four handled values
 * happen to be powers of two, but no caller ever ORs two together;
 * they're four mutually exclusive probe directions, not flag bits).
 *
 * Real bytes formerly the middle of `asm/code_3_2_17_25fc8.s` (that
 * file is now trimmed to end right after `sub_8026618`); the
 * remainder from `sub_80266BC` onward (still raw, unexamined this
 * session, including `sub_8026AE8`/`sub_8026A18` themselves) moved to
 * the new `asm/code_3_2_17_266bc.s`. */

struct probe_pos
{
    s32 x;
    s32 y;
};

/* `self+0x10` points at a per-object bounds record; only the two
 * fields this function itself reads are confirmed (a right/lower
 * streamed-region edge pair, compared directly against the caller's
 * plain-int probe position, not Q8). */
struct sub_8026628_bounds
{
    u8 unk0[0x10];
    s32 maxX; /* +0x10 */
    s32 maxY; /* +0x14 */
};

extern s32 sub_8026AE8(void *self, struct probe_pos *pos, s32 span, s32 *outValue, s32 submode);
extern s32 sub_8026A18(void *self, struct probe_pos *pos, s32 span, s32 *outValue, s32 submode);

s32 sub_8026628(void *self, s32 mode, struct probe_pos *pos, s32 span, s32 *outValue)
{
    s32 hit = 0;

    switch (mode)
    {
    case 2:
        if (pos->x < 0)
        {
            *outValue = 0;
            hit = 1;
        }
        else if ((u8)sub_8026AE8(self, pos, span, outValue, 3) != 0)
        {
            hit = 1;
        }
        break;
    case 1:
    {
        s32 bound = ((struct sub_8026628_bounds *)(*(void **)((u8 *)self + 0x10)))->maxX;
        if (pos->x > bound)
        {
            *outValue = bound << 8;
            hit = 1;
        }
        else if ((u8)sub_8026AE8(self, pos, span, outValue, mode) != 0)
        {
            hit = 1;
        }
        break;
    }
    case 4:
        if ((u8)sub_8026A18(self, pos, span, outValue, 2) != 0)
        {
            hit = 1;
        }
        break;
    case 8:
    {
        s32 bound = ((struct sub_8026628_bounds *)(*(void **)((u8 *)self + 0x10)))->maxY;
        if (pos->y > bound)
        {
            *outValue = bound << 8;
            hit = 1;
        }
        else if ((u8)sub_8026A18(self, pos, span, outValue, 0) != 0)
        {
            hit = 1;
        }
        break;
    }
    default:
        break;
    }

    return hit;
}
asm(".align 2, 0");
