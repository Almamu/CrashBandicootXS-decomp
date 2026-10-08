extern "C" {
#include "core.h"
#include "math_util.h"
#include "level.h"
}

/* GitHub issues #9/#10/#41's shared cross-reference: `CollidePlayer`
 * (`src/player/player_collide.c`, still `NON_MATCHING`/parked) flags
 * this as "still raw, only its return code's meaning as an opaque
 * 'hit' test against the constant 6 is used" from its own camera-
 * probe tail; `DrawAffineSpritePieces`'s original write-up
 * (`docs/matching/archive/issue-9-0x08007634-actor.md`, line ~214) separately
 * flags it as "the unexamined `GetTerrainFlagsAt`" from a jump-table
 * dispatch context. Neither caller needed anything more than the
 * return value, so it was never examined on its own until now.
 *
 * `GetTerrainFlagsAt(arg0, x, y)` is a small wrapper around the already-
 * matched terrain-tile-cache lookup `GetTerrainType` (`src/level/
 * tile_cache.cpp`, GitHub issue #40): it takes `arg0+0x20`'s pointed-to
 * `struct tile_cache`, converts `x`/`y` into that cache's own lookup
 * units via a plain `>>3` (clamped to a minimum of 0 on each axis
 * independently - `ProbeTerrain`'s own bounds-clamp neighbors use the
 * same "clamp each axis, don't just floor a negative tile index"
 * shape), and calls `GetTerrainType(cache, x>>3, y>>3, &flags, &hi)`.
 * `flags` and `hi` are both zero-initialized locals whose addresses
 * are handed to `GetTerrainType` purely as out-parameters - `hi` (the
 * decoded cell's top nibble, per `GetTerrainType`'s own doc comment) is
 * never read back here, exactly the "discarded outValue" idiom
 * `ProbeTerrain`'s own `ProbeTerrainX`/`ProbeTerrainY` calls already
 * established for this ROM neighborhood. Only `flags` (the u8 return
 * value `GetTerrainType` also returns directly) is returned - matching
 * both known call sites, which only ever compare the return code
 * against a small constant.
 *
 * `arg0` is always `gLevelLayers` at both known call sites
 * (`player_collide.c`'s camera-probe tail); `arg0+0x20` is that same
 * global struct's tile-cache-pointer field, the same offset
 * `CollidePlayer`'s own doc already established other fields of
 * (`+0x29`/`+0x2a`) for - not given its own named struct here since
 * this function only ever touches the one field, following the same
 * "duplicate only what's needed, no shared header" precedent
 * `struct tile_cache` itself already set between `bg_layer_base.cpp`/
 * `tile_cache.cpp`.
 *
 * Matched as real C on the first isolated-compile attempt - no
 * register pins or opaque asm needed, following `ProbeTerrain`'s own
 * "matched on the first attempt" precedent right next door. Confirmed
 * byte-identical to the ROM's own instructions (register for
 * register, operand for operand) via the isolated `cpp`/`agbcc`/`as`
 * + `objcopy`/`cmp` pipeline against `baserom.gba`'s raw bytes at
 * `0x08026BC0`-`0x08026BF8` (the only difference being the `bl
 * GetTerrainType` relocation site, which resolves correctly once
 * linked), plus a full clean `rm -rf build && make NON_MATCHING=1
 * report` (no warnings) and `rm -rf build crashbandicootxs.elf
 * crashbandicootxs.gba crashbandicootxs.map && make compare`
 * (`crashbandicootxs.gba: La suma coincide`).
 *
 * See docs/matching/archive/issue-9-10-0x0800a884-graphics.md for the full
 * write-up of this closure, appended to the section that originally
 * flagged this function raw. */

s32 GetTerrainFlagsAt(void *arg0, s32 x, s32 y)
{
    u8 flagsOut = 0;
    s32 hiOut = 0;
    s32 tileX = x >> 3;
    s32 tileY = y >> 3;

    LIMIT_MIN(tileX, 0);
    LIMIT_MIN(tileY, 0);

    GetTerrainType(((struct level_layers *)arg0)->tiles, tileX, tileY, &flagsOut, &hiOut);

    return flagsOut;
}

/* GitHub issue #9/#10: single-point terrain-height ("floor") probes,
 * split out of `docs/matching/archive/issue-9-0x0800a178-graphics.md`'s existing
 * write-up - both callers (`ProbeGroundSpriteTerrain`/`ProbeGroundSpriteFloor`, GitHub issue
 * #9/#10, `src/objects/ground_sprite_collide.cpp`) already fully placed their
 * argument roles: `s32 fn(void *player, struct vec2 *pos, s32
 * *outValue)`, computing `pos->x >> 3`/`pos->y >> 3` tile coords from
 * `player+0x20`'s terrain-data pointer (the same `struct tile_cache *`
 * field `GetTerrainFlagsAt` (above) already established that offset
 * for on the same `player`/`arg0` global, `gLevelLayers`).
 *
 * `ProbeFloorHeight` looks the tile row up via the already-matched
 * `GetTerrainHeights` ("the raw terrain streamer" - `bg_layer_base.cpp`, GitHub
 * issue #40), returning a row pointer or `NULL` on a miss. On a hit,
 * reads a **signed byte** height sample at `row[pos->x & 7]`, computes
 * `((pos->y >> 3) << 3) + heightByte - pos->y`, shifts to Q8, and
 * accumulates it into `*outValue`. Returns `1` on a row hit, `0` if
 * `GetTerrainHeights` returned `NULL`.
 *
 * `ProbeSolidFloorHeight` is the exact same shape, but the height byte comes from
 * the already-matched `GetSolidTerrainModeValue(terrainPtr, tileX, tileY, 0,
 * &scratch)` instead - the "CheckTerrainFlag"-style API
 * `ProbeTerrainY`/`ProbeTerrainX` already use via their own `GetSolidTerrainHeights`
 * calls (`bg_layer_base.cpp`, same issue #40). `scratch` is a caller-local
 * flag-nibble out-parameter nothing here ever reads back, the same
 * "discarded outValue" idiom `bg_layer_base.cpp`'s own siblings already
 * established. Returns `0` if the returned signed byte is negative, `1`
 * otherwise, with the same `(tileY<<3)+byte-pos->y` delta accumulation.
 *
 * Both are Y-axis (floor-height) probes - matching how `ProbeGroundSpriteTerrain`
 * only ever uses them against `self.y`/`self->y`, never `self.x`.
 * `struct vec2` reuses `terrain_probe.cpp`'s own plain-int (not Q8)
 * probe-position layout unchanged (same "duplicate only what's needed,
 * no shared header" precedent `struct tile_cache` itself already set
 * between `bg_layer_base.cpp`/`tile_cache.cpp`).
 *
 * Both reload `pos->x`/`pos->y` a second time from memory after their
 * respective lookup call rather than keeping the pre-shifted tile
 * coordinate live across it (the ROM's own `ldr`s confirm this -
 * `tileX` itself is never reused after being passed to the lookup call,
 * and `pos->y` is read fresh for the final subtraction even though its
 * shifted form, `tileY`, is still live in a callee-saved register) -
 * this falls out naturally once `y` is hoisted into its own local
 * (matching the ROM's own early reload) and the accumulation is written
 * with the row-byte/height operand first (`height + (tileY << 3) - y`
 * for `ProbeFloorHeight`, whose value depends on the row lookup; `(tileY <<
 * 3) + height - y` for `ProbeSolidFloorHeight`, whose height is already available
 * before the branch, forcing the same left-to-right emission order
 * either way) rather than however the multiplication naturally reads.
 *
 * `ProbeSolidFloorHeight` matched on the first isolated-compile attempt, no
 * register pins needed. `ProbeFloorHeight` needed one targeted fix: this
 * agbcc build never emits a Thumb `LDRSB` (register-offset signed-byte
 * load) from *any* C-level signed-byte array/pointer read - confirmed
 * categorically with a minimal standalone `s32 f(s8 *arr, s32 i) {
 * return arr[i]; }` test, which still lowers to `ldrb` + `lsl #24` +
 * `asr #24` (Thumb's `LDRSB` has no immediate-offset encoding at all,
 * unlike `LDRB`, so this compiler's cost model apparently never
 * considers it once the base+index add has already been folded into a
 * single address register). Closed with a narrow inline-asm
 * materialization of the exact `mov r1,#0`/`ldrsb r1,[r0,r1]` pair the
 * ROM itself uses, register-pinned to reproduce the ROM's own register
 * choices (`addr` r0 = `row + (pos->x & 7)`, `height` r1 = `0` going in,
 * the loaded byte coming back out) - not a blanket opaque block, just
 * the one instruction class this compiler categorically can't select.
 *
 * Confirmed byte-exact via the isolated `cpp`/`agbcc`/`as` +
 * `objcopy`/`cmp` pipeline against `baserom.gba`'s own bytes at
 * `0x08026BF8`-`0x08026C80` (136 bytes total, both functions), plus a
 * full clean `rm -rf build && make NON_MATCHING=1 report` (no warnings)
 * and `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
 * crashbandicootxs.map && make compare` (`crashbandicootxs.gba: La suma
 * coincide`).
 *
 * Bonus pass over the two tiny functions immediately following in the
 * same file: sub_8026C80 (10 bytes) and sub_8026C8C (4 bytes), both
 * flush byte-exact on the first isolated-compile attempt (no relocation
 * sites, no register pins). Neither has any caller anywhere in the ROM,
 * checked every asm/*.s, expected/*.s (aside from their own definitions
 * there), and every .c file under src/ for a bl/.4byte reference to
 * either symbol, none found, so both are UNUSED, matched anyway following this
 * project usual practice of still routing genuinely dead code through
 * the normal matching workflow:
 *
 * sub_8026C80(void *arg0, s32 arg1, s32 *arg2): arg0 is never touched
 * (dead parameter). If arg1 is nonzero, dereferences arg2 and discards
 * the result, a real load with no observable effect, needing arg2 typed
 * volatile to survive optimization (matching the ROM own unconditional
 * ldr r0, [r2], whose result is immediately clobbered by the trailing
 * movs r0, #0). Always returns 0 regardless of which path was taken.
 * Shape-wise this reads like a stripped-down conditional accessor or
 * validator whose real work was optimized away upstream of the ROM
 * build, but nothing here confirms that; the load purpose, if it ever
 * had an observable one, is not recoverable from this function alone
 * with no caller to cross-check against.
 *
 * sub_8026C8C(void): unconditionally returns 0, touching no parameters
 * at all. Not given the nullsub_N name since that naming convention (see
 * docs/naming.md) is reserved for a genuinely empty function body (bx lr
 * alone), not a one-instruction always-returns-a-constant stub; left as
 * sub_8026C8C per docs/naming.md own when-in-doubt-leave-it-sub_XXXXXXXX
 * guidance.
 *
 * Real bytes formerly the start of `asm/code_3_2_17_26bf8.s` (that file
 * is now trimmed to begin at `StepCameraDirectional`). */

struct tile_cache;

/* The floor under pixel `pos` from the sloped terrain types (1-0x23,
 * GetTerrainHeights): adds the distance from `pos->y` to that column's
 * surface, Q8, to `*outValue` and returns 1, or returns 0 when the cell has
 * none (ProbeGroundSpriteFloor). */
s32 ProbeFloorHeight(void *player, struct vec2 *pos, s32 *outValue)
{
    s32 tileX = pos->x >> 3;
    s32 tileY = pos->y >> 3;
    s8 *row = (s8 *)GetTerrainHeights(((struct level_layers *)player)->tiles, tileX, tileY);

    if (row != NULL) {
        s32 y = pos->y;
        s32 height = row[pos->x & 7];

        *outValue += INT_TO_Q8((tileY << 3) + height - y);
        return 1;
    }
    return 0;
}

/* The same for the solid terrain types (0x24 and above), whose surface is
 * the type's mode-0 value (GetSolidTerrainModeValue) rather than a height
 * per column (ProbeGroundSpriteTerrain). */
s32 ProbeSolidFloorHeight(void *player, struct vec2 *pos, s32 *outValue)
{
    u8 scratch;
    s32 tileX = pos->x >> 3;
    s32 tileY = pos->y >> 3;
    s8 height =
        GetSolidTerrainModeValue(((struct level_layers *)player)->tiles, tileX, tileY, 0, &scratch);

    if (height >= 0) {
        s32 y = pos->y;

        *outValue += INT_TO_Q8((tileY << 3) + height - y);
        return 1;
    }
    return 0;
}

/* UNUSED - no caller anywhere in the ROM (checked every asm/*.s,
 * expected/*.s, and every .c file under src/ for a bl/.4byte reference).
 * See the file-level comment above. */
s32 sub_8026C80(void *arg0, s32 arg1, s32 *arg2)
{
    if (arg1 != 0)
        (void)*(volatile s32 *)arg2;
    return 0;
}

/* UNUSED - no caller anywhere in the ROM (checked every asm/*.s,
 * expected/*.s, and every .c file under src/ for a bl/.4byte reference).
 * See the file-level comment above. */
s32 sub_8026C8C(void)
{
    return 0;
}
