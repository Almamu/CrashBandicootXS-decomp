#include "core.h"
#include "box_part.h"
#include "objects.h"
#include "level.h"
#include "globals.h"

/* Dedicated deep investigation (GitHub issue #9/#10,
 * docs/matching/archive/issue-9-0x0800a178-graphics.md): `ProbeGroundSpriteTerrain`/
 * `ProbeGroundSpriteFloor`, the last two functions in the `CollideGroundSprite`-through-
 * `ProbeGroundSpriteFloor` still-raw span `tools/report_units.py` tracked as
 * parked. `CollideGroundSprite` itself (the caller of `ProbeGroundSpriteTerrain`, see
 * `asm/code_3_2_11.s`) stays raw/unexamined - its own gate logic still
 * depends on the also-still-raw `sub_8009BE0` (parked NAKED,
 * `step_probe.c`), so closing `ProbeGroundSpriteTerrain`/`ProbeGroundSpriteFloor` alone
 * doesn't unblock it.
 *
 * `docs/rom_map.md` (line ~2624) had already partially flagged
 * `ProbeGroundSpriteTerrain`: "mid-function, unconditionally zeroes `self+0x74` -
 * the same field `UpdateGameFrame`'s level-load branch sets once from
 * `RunTitleScreen`'s return value ... consistent with 'total for this
 * level' being cleared and presumably recomputed under some condition,
 * not fully traced here." Reading the real bytes confirms the zeroing
 * itself exactly (unconditional once both leading gate checks pass -
 * see below) and traces the "recompute" side all the way through: the
 * function's second half OR's `mode` bits (`self+0x24 & 0x3`/`0xc`)
 * into `self+0x74` on each successful `ProbeTerrain` axis-probe hit -
 * i.e. `self+0x74` really is "which movement axes/directions collided
 * this call", zeroed up front and rebuilt bit by bit as each probe
 * fires. `docs/matching/archive/issue-9-0x08007634-actor.md` (line ~206) had
 * also already flagged both functions as built on `sub_8008200`/
 * `ProbeTerrain`/`sub_8026C3C`/`sub_8026BF8` - two of those four
 * (`sub_8008200`, `ProbeTerrain`) are already matched this session
 * (`src/objects/sprite_obj.c`, `src/level/terrain_probe.c`); this
 * session additionally reads `sub_8026C3C`/`sub_8026BF8` (still raw,
 * `asm/code_3_2_17_266bc.s`) far enough to place them precisely.
 *
 * ## What `ProbeGroundSpriteTerrain`/`ProbeGroundSpriteFloor` actually do
 *
 * Both operate on the same "hitbox quad" pointer - `self->table[0x10]`/
 * `[0x14]`'s own `_call_via_r1(self + addr, fn)` trampoline result,
 * i.e. a `{s16 xOff, s16 yOff, u8 w, u8 h}` record straight from this
 * ROM region's already-established convention (`crate_hit.c`'s
 * `struct hitbox_quad`, `crate_touch.c`'s AABB builds) - and share
 * the same "adjust the object's own Q8 position based on where a
 * collision probe says the ground/wall actually is" shape:
 *
 * - **`ProbeGroundSpriteFloor(self, quad, u8 *outFlag)`**: a single Y-axis
 *   ("floor") probe. Builds an int `{x, y}` position at the *bottom*
 *   of the quad (`self.x`/`self.y + (quad->yOff + quad->h) << 8`,
 *   via `sub_8008200(dest, 8, quad)`, nudged left/right by half the
 *   quad's width depending on `self+0x28` bit 4's mirror flag), then
 *   probes it via `sub_8026BF8` (see below). On a hit, snaps
 *   `self->y` to the probed height (optionally one pixel higher, if
 *   `self+0xd` bit 1 is clear) and sets `self+0xd` bit 1. On a miss,
 *   if bit 1 was already clear it retries one pixel lower via a
 *   second `sub_8026BF8` call; if that also misses, `*outFlag` is set
 *   only when bit 1 was set from the very start (skipping the retry
 *   entirely) - `self+0xd` bit 1 always ends up cleared on any
 *   all-miss path. Returns whichever probe's hit boolean was last
 *   computed.
 * - **`ProbeGroundSpriteTerrain(self)`**: the larger orchestrator. Gated on two
 *   guard checks (a `_call_via_r1(self->table[0x38]/[0x3c])`
 *   trampoline truthiness test, then `self+0xc` bit 7) - either
 *   failing returns `0` immediately with no other effect. Once past
 *   both gates: unconditionally zeroes `self+0x74` (the `rom_map.md`
 *   field, confirmed - see above), computes a starting result mask
 *   (`self+0xd` bit 0 expanded to `8`, but only when `self+0x24 &
 *   0xc` is clear), then:
 *   1. If `self+0xd` bit 0 is set, calls `ProbeGroundSpriteFloor` once
 *      (arg `&flagByte`, a stack-local initialized `0`) and keeps its
 *      boolean result as a "found ground already" flag.
 *   2. If that flag is set, and the result mask is still `0`, forces
 *      it to `8`.
 *   3. If the `ProbeGroundSpriteFloor` out-flag came back `1`: builds an int
 *      position at the *bottom* of the quad (mirrored the same way),
 *      probes it via `sub_8026C3C(player, pos, &origY)` (see below,
 *      `player` = `gLevelLayers` dereferenced) - on a hit, snaps
 *      `self->y` to the probed value and nudges `self->x` by ±1 pixel
 *      depending on `self+0x24 & 3`; on a miss, nudges `self->y` down
 *      one pixel instead. Either way calls `ProbeGroundSpriteFloor` again
 *      afterward to refresh the "found ground" flag.
 *   4. Three more blocks, each gated on `self+0x24`'s own 2-bit field
 *      (`& 3` for X-axis modes 1/2, `& 0xc` for Y-axis modes 4/8) and
 *      on the "found ground" flag still being `0`: build a plain int
 *      `{x, y}` position via `sub_8008278(dest, mode, quad)`, probe it
 *      through the shared `ProbeTerrain(player, mode, pos, span,
 *      outValue)` API (already matched, `terrain_probe.c`) with a
 *      `span` derived from the quad's own `h`/`w` byte (`h - 16` for
 *      the first X-axis block, `w` for the Y-axis block, `h` for the
 *      second X-axis block - three deliberately different probe
 *      geometries, not a shared constant), and on a hit OR's `mode`
 *      into both `self+0x74` and the running result mask, restoring
 *      the axis coordinate `ProbeTerrain` didn't touch. Returns the
 *      final result mask.
 *
 * Read together: this is the part-object movement/collision
 * *resolution* step - once some other function (still-raw
 * `CollideGroundSprite`) has decided a part object needs a physics update,
 * `ProbeGroundSpriteTerrain` runs a layered probe (fast quad-based floor/wall test
 * first via `ProbeGroundSpriteFloor`/`sub_8026C3C`, then falling back to the
 * general tile-scan `ProbeTerrain` API per axis) and snaps the object's
 * position to whatever solid surface each probe finds, recording which
 * axes/directions actually resolved in `self+0x74` for whatever caller
 * reads it next (`UpdateGameFrame`'s own level-load branch is the only
 * other confirmed writer, per `rom_map.md`).
 *
 * ## `sub_8026C3C`/`sub_8026BF8` (still raw, understood only)
 *
 * Both are single-point collision-test siblings of the already-matched
 * `ProbeTerrainY`/`ProbeTerrainX` pair (`terrain_probe.c`'s own "what's still
 * open" section already predicted this) - `rom_map.md`'s "15 more
 * reads" pass (line ~2581) had already placed them as "single-point
 * collision-test siblings ... one via the raw terrain streamer and one
 * via the CheckTerrainFlag API"; reading their bytes directly confirms
 * that and pins down the exact shape, both `s32 fn(void *player, struct
 * probe_pos *pos, s32 *outValue)`:
 *
 * - **`sub_8026BF8`**: `player->0x20`'s terrain-data pointer, `pos->x
 *   >> 3`/`pos->y >> 3` tile coords, looked up via `GetTerrainHeights`
 *   ("the raw terrain streamer" - returns a row pointer or `NULL`).
 *   On a hit, reads a **signed byte** height sample at
 *   `row[pos->x & 7]`, computes `((pos->y >> 3) << 3) + heightByte -
 *   pos->y`, shifts to Q8, and accumulates it into `*outValue`.
 *   Returns `1` on a row hit, `0` if `GetTerrainHeights` returned `NULL`.
 * - **`sub_8026C3C`**: the exact same shape, but the height byte comes
 *   from `sub_8025228(terrainPtr, tileX, tileY, 0, &scratch)` instead
 *   of a direct row-pointer byte read - the "CheckTerrainFlag"-style
 *   API `ProbeTerrainY`/`ProbeTerrainX` already use via their own
 *   `GetSolidTerrainHeights` calls (same argument shape: base pointer, tile
 *   coords, a submode, an out-parameter). Returns `0` if the returned
 *   signed byte is negative, `1` otherwise, with the same
 *   `(tileY<<3)+byte-pos->y` delta accumulation.
 *
 * Both are Y-axis (floor-height) probes, matching how `ProbeGroundSpriteTerrain`
 * uses them (against `self.y`/`self->y`, never `self.x`). Full byte-
 * exact matching for either wasn't attempted this session - see
 * docs/matching/archive/issue-9-0x0800a178-graphics.md for why (same
 * resistant multi-high-register shape this immediate ROM neighborhood
 * has already hit four times: `sub_8009BE0`, `PlayerAnimWouldTouchCrate`,
 * `sub_800CEAC`, `sub_800CF70`).
 *
 * ## Matching
 *
 * All three functions in this file are real C built with old_agbcc
 * (Makefile OLD_AGBCC_OBJS; issue #9-#11 NAKED retry,
 * docs/matching/archive/issue-9-11-box-naked-retry.md). They were first parked
 * as NAKED transcriptions on the theory that the ROM's `sb`/`sl`/`r8`
 * cross-block register reuse resists gcc 2.9 - under the right
 * compiler it falls out of plain C; the few source-shape details that
 * mattered are noted next to each function. */

/* Follow-up (same investigation): `CollideGroundSprite`, the only caller of
 * `ProbeGroundSpriteTerrain` (both live right next to each other, `CollideGroundSprite`
 * immediately before `ProbeGroundSpriteTerrain` in ROM and formerly the sole
 * remaining content of `asm/code_3_2_11.s`). It stayed raw the first
 * pass through this cluster because its own gate logic calls
 * `sub_8009BE0` (parked NAKED, `src/objects/step_probe.c`, see
 * `docs/matching/archive/naked-spatial-grid-tail.md`) - at the time that
 * function's semantics were still unresolved. `sub_8009BE0` is now
 * fully understood (a physics/collision step-probe: converts `self`'s
 * position to plain ints via `sub_8008278`, probes it through
 * `ProbeTerrain` with `mode` as the axis selector, retrying up to 3
 * more times on a miss by nudging Y down), which is enough to close
 * this function's own dispatch logic as real, byte-exact matched C:
 *
 * `CollideGroundSprite(self)` returns early with `self+0x68` unchanged unless
 * `self+0xc` bit 7 is set (the same gate `ProbeGroundSpriteTerrain` itself re-checks
 * internally). If set: calls `ProbeGroundSpriteTerrain(self)` and OR's its result
 * bitmask into `self+0x68` (a persistent, cumulative per-object
 * collision-axis mask - distinct from `self+0x74`'s own per-call
 * scratch mask `ProbeGroundSpriteTerrain` zeroes and rebuilds every call), then
 * unconditionally fires `CollideMovingSprite(self)` (the already-matched
 * `self->table+0x70/0x74` trampoline, `src/objects/moving_sprite_collide.c` -
 * a side-effect-only call, its always-`0` return discarded). If
 * `self+0x68` bit 3 (the "Y-axis/mode-8" collision bit `ProbeGroundSpriteTerrain`
 * just OR'd in, if it hit) is now set: clears `self+0xc` bits 0 and 5,
 * then - unless `self+0xd` bit 1 is already set (ground already
 * snapped this call, `ProbeGroundSpriteFloor`'s own convention) - fires the same
 * `self->table+0x10/0x14` "hitbox quad" trampoline `ProbeGroundSpriteTerrain`
 * itself uses, and runs a `mode == 8` (Y-axis/floor) step-probe via
 * `sub_8009BE0(self, 8, quad)`. If that step-probe does *not* report
 * immediate success (either a full miss, or only succeeding via one of
 * its internal retries - see `sub_8009BE0`'s own doc comment), sets
 * `self+0xc` bit 5 and clears `self+0x68` bit 3 back out - rolling
 * back the "Y axis resolved" bit `ProbeGroundSpriteTerrain`'s cheaper probe had
 * just set, since the more thorough step-probe didn't confirm it
 * cleanly. Returns the (possibly rolled-back) `self+0x68` byte either
 * way.
 *
 * Read together with `ProbeGroundSpriteTerrain`/`ProbeGroundSpriteFloor`: this is the
 * part-object physics dispatcher - `CollideGroundSprite` is the entry point
 * (called by `CollidePlayer`'s per-frame reentrancy-guarded wrapper,
 * `docs/rom_map.md` line ~1835), `ProbeGroundSpriteTerrain` does the actual
 * layered collision resolution and reports which axes it resolved,
 * and `CollideGroundSprite`'s own tail cross-checks the Y-axis result against
 * a second, independent step-probe (`sub_8009BE0`) before trusting it
 * enough to leave the bit set in the persistent `self+0x68` mask.
 *
 * Real C, built with old_agbcc (issue #9-#11 NAKED retry: the file
 * moved to OLD_AGBCC_OBJS for `ProbeGroundSpriteFloor`). Under old_agbcc the
 * register pins this function needed under the current compiler are
 * gone; the one shape left is the `self+0xc` clear, which goes through
 * an `s32` local so the mask stays the SImode `-0x21` (`movs #0x21;
 * negs`) instead of a folded QImode `0xDF`. The mask clears bit 5 only
 * (`~0x20`), not `~0x21` as the older notes had it. */

extern s32 _call_via_r1(void *addr, void *fn);

u8 CollideGroundSprite(struct box_part *self)
{
    u8 *p = &self->hitAxes;
    u8 val = *p;

    if (self->flags >> 7) {
        val |= ProbeGroundSpriteTerrain(self);
        *p = val;
        CollideMovingSprite((struct gobj *)self);
        if (*p & 8) {
            {
                s32 f = self->flags & ~0x20;

                self->flags = f;
            }
            if (!((self->flags2 >> 1) & 1)) {
                struct part_method *m = PART_METHOD(self, 0x10);
                void *quad = (void *)_call_via_r1((u8 *)self + m->thisOffset, m->fn);

                if (!(u8)sub_8009BE0(self, 8, quad)) {
                    self->flags |= 0x20;
                    *p &= 7;
                }
            }
        }
    }
    return self->hitAxes;
}

/* See the file-level header comment above for this function's
 * semantics. `self`'s only argument; returns the accumulated result
 * bitmask (`self+0x24`'s per-axis mode bits, OR'd in as each
 * `ProbeTerrain` probe reports a hit).
 *
 * Source-shape details that matter: the result is an `s32` set by a
 * `? 8 : result` conditional (expanded as `-(x != 0)` into the result
 * register, then `&= 8` - the ROM's `mov r2, sb; and r2, r0; mov sb, r2`
 * reload); the `self+0x24` pointer is taken after its `& 0xc` test
 * value (so its spill store follows the `and`); the two out-bytes of
 * ProbeGroundSpriteFloor are separate `u8` locals (sp+4 and sp+5, the second
 * addressed as `sp + 5`, not `&arr[1]`). The Y-axis block stores
 * `origX` too although it probes with `&origY`, as the ROM does. */
s32 ProbeGroundSpriteTerrain(struct box_part *self)
{
    s32 origX;
    s32 origY;
    struct probe_pos pos;
    u8 unused;
    u8 floorMiss;
    s32 result;
    u8 hit;
    struct hitbox_quad *quad;
    u8 *axes;
    struct part_method *m;
    s32 mode;

    result = 0;
    floorMiss = result;
    hit = 0;
    m = PART_METHOD(self, 0x38);
    if (!(u8)_call_via_r1((u8 *)self + m->thisOffset, m->fn))
        goto done;
    if (!(self->flags >> 7))
        goto done;
    {
        u32 t = self->moveAxes & 0xc;

        axes = &self->moveAxes;
        if (!t)
            result = (self->flags2 & 1) ? 8 : result;
    }
    self->hitMask = 0;
    m = PART_METHOD(self, 0x10);
    quad = (struct hitbox_quad *)_call_via_r1((u8 *)self + m->thisOffset, m->fn);
    if (self->flags2 & 1)
        hit = ProbeGroundSpriteFloor(self, quad, &floorMiss);
    if (hit && result == 0)
        result = 8;
    if (floorMiss == 1) {
        u8 c;

        origY = self->y;
        pos = *(struct probe_pos *)self;
        sub_8008200(&pos, 8, quad);
        pos.x >>= 8;
        pos.y >>= 8;
        if (self->mirrorX)
            pos.x -= quad->w >> 1;
        else
            pos.x += quad->w >> 1;
        c = sub_8026C3C(gLevelLayers, &pos, &origY);
        unused = 0;
        if (c) {
            self->y = origY & 0xFFFFFF00;
            hit = ProbeGroundSpriteFloor(self, quad, &unused);
            {
                u32 xm = *axes & 3;

                if (xm == 0)
                    goto y_probe;
                if (xm == 2)
                    self->x += (s32)0xFFFFFF00;
                else
                    self->x += 0x100;
            }
        } else {
            self->y += 0x100;
            hit = ProbeGroundSpriteFloor(self, quad, &unused);
        }
    }
    mode = *axes & 3;
    if (mode && !hit) {
        s32 span;

        pos = *(struct probe_pos *)self;
        span = quad->h - 0x10;
        origX = self->x;
        sub_8008278(&pos, mode, quad);
        pos.x >>= 8;
        pos.y = (pos.y >> 8) + 8;
        if ((u8)ProbeTerrain(gLevelLayers, mode, &pos, span, &origX)) {
            self->hitMask |= mode;
            result |= mode;
            self->x = origX;
        }
    }
y_probe:
    mode = *axes & 0xc;
    if (mode && !hit) {
        s32 span;

        pos = *(struct probe_pos *)self;
        span = quad->w;
        origX = self->x;
        origY = self->y;
        sub_8008278(&pos, mode, quad);
        pos.x >>= 8;
        pos.y >>= 8;
        if ((u8)ProbeTerrain(gLevelLayers, mode, &pos, span, &origY)) {
            result |= mode;
            self->hitMask |= mode;
            self->y = origY;
        }
    }
    mode = *axes & 3;
    if (mode && !hit) {
        s32 span;

        pos = *(struct probe_pos *)self;
        span = quad->h;
        origX = self->x;
        sub_8008278(&pos, mode, quad);
        pos.x >>= 8;
        pos.y >>= 8;
        if ((u8)ProbeTerrain(gLevelLayers, mode, &pos, span, &origX)) {
            self->hitMask |= mode;
            result |= mode;
            self->x = origX;
        }
    }
done:
    return result;
}

/* See the file-level header comment above for this function's
 * semantics. `self`'s a part object, `quad` its `{s16 xOff, s16 yOff,
 * u8 w, u8 h}` hitbox quad pointer (the `self->table[0x10]/[0x14]`
 * trampoline result `ProbeGroundSpriteTerrain` also uses), `outFlag` a caller-owned
 * byte set to `1` only on the specific all-miss-with-bit-1-already-set
 * path described above. Returns the final probe's hit boolean.
 *
 * Real C under old_agbcc (issue #9-#11 NAKED retry). The first probe's
 * hit path computes the bit-1 test into its own local before copying
 * the flags byte to `v`, with `f` pinned to r2 and `v` to r1: that is
 * the ROM's `lsrs; movs #1; ands; adds r1, r2, #0` order, and it frees
 * r2 for the `0xFFFFFF00` literal. The shared final `strb` to +0xd is a
 * common store (`val`) the three exits jump to. */

u8 ProbeGroundSpriteFloor(struct box_part *self, struct hitbox_quad *quad, u8 *outFlag)
{
    s32 origY = self->y;
    struct probe_pos pos;
    u8 hit;
    s32 val;

    pos = *(struct probe_pos *)self;
    sub_8008200(&pos, 8, quad);
    pos.x >>= 8;
    pos.y >>= 8;
    if (!((self->flags2 >> 1) & 1))
        pos.y--;
    if (self->mirrorX)
        pos.x -= quad->w >> 1;
    else
        pos.x += quad->w >> 1;
    hit = sub_8026BF8(gLevelLayers, &pos, &origY);
    if (hit) {
        s32 y;
        register u8 f asm("r2");
        u32 t;
        register s32 v asm("r1");

        self->y = y = origY & 0xFFFFFF00;
        f = self->flags2;
        t = (f >> 1) & 1;
        v = f;
        if (!t)
            self->y = y + (s32)0xFFFFFF00;
        val = 2 | v;
    } else {
        u8 f = self->flags2;
        if (!((f >> 1) & 1)) {
            pos.y++;
            hit = sub_8026BF8(gLevelLayers, &pos, &origY);
            if (hit) {
                self->y = origY & 0xFFFFFF00;
                val = 2 | self->flags2;
                goto store;
            }
            f = self->flags2;
            if (!((f >> 1) & 1))
                goto clear;
        }
        *outFlag = 1;
    clear:
        val = f & ~2;
    }
store:
    self->flags2 = val;
    return hit;
}
