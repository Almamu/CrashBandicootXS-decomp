#include "core.h"

/* GitHub issue #9/#10: single-point terrain-height ("floor") probes,
 * split out of `docs/matching/issue-9-0x0800a178-graphics.md`'s existing
 * write-up - both callers (`sub_800A178`/`sub_800A420`, GitHub issue
 * #9/#10, `src/graphics/actor_part110.c`) already fully placed their
 * argument roles: `s32 fn(void *player, struct probe_pos *pos, s32
 * *outValue)`, computing `pos->x >> 3`/`pos->y >> 3` tile coords from
 * `player+0x20`'s terrain-data pointer (the same `struct tile_cache *`
 * field `sub_8026BC0` (`game_loop44.c`) already established that offset
 * for on the same `player`/`arg0` global, `gUnknown_03001308`).
 *
 * `sub_8026BF8` looks the tile row up via the already-matched
 * `sub_80250BC` ("the raw terrain streamer" - `game_loop3.c`, GitHub
 * issue #40), returning a row pointer or `NULL` on a miss. On a hit,
 * reads a **signed byte** height sample at `row[pos->x & 7]`, computes
 * `((pos->y >> 3) << 3) + heightByte - pos->y`, shifts to Q8, and
 * accumulates it into `*outValue`. Returns `1` on a row hit, `0` if
 * `sub_80250BC` returned `NULL`.
 *
 * `sub_8026C3C` is the exact same shape, but the height byte comes from
 * the already-matched `sub_8025228(terrainPtr, tileX, tileY, 0,
 * &scratch)` instead - the "CheckTerrainFlag"-style API
 * `sub_8026A18`/`sub_8026AE8` already use via their own `sub_8025130`
 * calls (`game_loop3.c`, same issue #40). `scratch` is a caller-local
 * flag-nibble out-parameter nothing here ever reads back, the same
 * "discarded outValue" idiom `game_loop3.c`'s own siblings already
 * established. Returns `0` if the returned signed byte is negative, `1`
 * otherwise, with the same `(tileY<<3)+byte-pos->y` delta accumulation.
 *
 * Both are Y-axis (floor-height) probes - matching how `sub_800A178`
 * only ever uses them against `self.y`/`self->y`, never `self.x`.
 * `struct probe_pos` reuses `game_loop43.c`'s own plain-int (not Q8)
 * probe-position layout unchanged (same "duplicate only what's needed,
 * no shared header" precedent `struct tile_cache` itself already set
 * between `game_loop3.c`/`game_loop4.c`).
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
 * for `sub_8026BF8`, whose value depends on the row lookup; `(tileY <<
 * 3) + height - y` for `sub_8026C3C`, whose height is already available
 * before the branch, forcing the same left-to-right emission order
 * either way) rather than however the multiplication naturally reads.
 *
 * `sub_8026C3C` matched on the first isolated-compile attempt, no
 * register pins needed. `sub_8026BF8` needed one targeted fix: this
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
 * is now trimmed to begin at `sub_8026C90`). */

struct probe_pos
{
    s32 x;
    s32 y;
};

struct tile_cache;

extern void *sub_80250BC(struct tile_cache *self, s32 x, s32 y);
extern s8 sub_8025228(struct tile_cache *self, s32 x, s32 y, s32 mode, u8 *flagsOut);

s32 sub_8026BF8(void *player, struct probe_pos *pos, s32 *outValue)
{
    s32 tileX = pos->x >> 3;
    s32 tileY = pos->y >> 3;
    s8 *row = (s8 *)sub_80250BC(*(struct tile_cache **)((u8 *)player + 0x20), tileX, tileY);

    if (row != NULL)
    {
        s32 y = pos->y;
        register s8 *addr asm("r0") = row + (pos->x & 7);
        register s32 height asm("r1") = 0;

        asm("ldrsb %0, [%1, %0]" : "+r"(height) : "r"(addr));

        *outValue += ((tileY << 3) + height - y) << 8;
        return 1;
    }
    return 0;
}

s32 sub_8026C3C(void *player, struct probe_pos *pos, s32 *outValue)
{
    u8 scratch;
    s32 tileX = pos->x >> 3;
    s32 tileY = pos->y >> 3;
    s8 height = sub_8025228(*(struct tile_cache **)((u8 *)player + 0x20), tileX, tileY, 0, &scratch);

    if (height >= 0)
    {
        s32 y = pos->y;

        *outValue += ((tileY << 3) + height - y) << 8;
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
