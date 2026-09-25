#include "core.h"

/* GitHub issues #9/#10/#41's shared cross-reference: `sub_800A884`
 * (`src/graphics/actor_part78.c`, still `NON_MATCHING`/parked) flags
 * this as "still raw, only its return code's meaning as an opaque
 * 'hit' test against the constant 6 is used" from its own camera-
 * probe tail; `sub_8007634`'s original write-up
 * (`docs/matching/issue-9-0x08007634-actor.md`, line ~214) separately
 * flags it as "the unexamined `sub_8026BC0`" from a jump-table
 * dispatch context. Neither caller needed anything more than the
 * return value, so it was never examined on its own until now.
 *
 * `sub_8026BC0(arg0, x, y)` is a small wrapper around the already-
 * matched terrain-tile-cache lookup `sub_8025460` (`src/system/
 * game_loop4.c`, GitHub issue #40): it takes `arg0+0x20`'s pointed-to
 * `struct tile_cache`, converts `x`/`y` into that cache's own lookup
 * units via a plain `>>3` (clamped to a minimum of 0 on each axis
 * independently - `sub_8026628`'s own bounds-clamp neighbors use the
 * same "clamp each axis, don't just floor a negative tile index"
 * shape), and calls `sub_8025460(cache, x>>3, y>>3, &flags, &hi)`.
 * `flags` and `hi` are both zero-initialized locals whose addresses
 * are handed to `sub_8025460` purely as out-parameters - `hi` (the
 * decoded cell's top nibble, per `sub_8025460`'s own doc comment) is
 * never read back here, exactly the "discarded outValue" idiom
 * `sub_8026628`'s own `sub_8026AE8`/`sub_8026A18` calls already
 * established for this ROM neighborhood. Only `flags` (the u8 return
 * value `sub_8025460` also returns directly) is returned - matching
 * both known call sites, which only ever compare the return code
 * against a small constant.
 *
 * `arg0` is always `gUnknown_03001308` at both known call sites
 * (`actor_part78.c`'s camera-probe tail); `arg0+0x20` is that same
 * global struct's tile-cache-pointer field, the same offset
 * `sub_800A884`'s own doc already established other fields of
 * (`+0x29`/`+0x2a`) for - not given its own named struct here since
 * this function only ever touches the one field, following the same
 * "duplicate only what's needed, no shared header" precedent
 * `struct tile_cache` itself already set between `game_loop3.c`/
 * `game_loop4.c`.
 *
 * Matched as real C on the first isolated-compile attempt - no
 * register pins or opaque asm needed, following `sub_8026628`'s own
 * "matched on the first attempt" precedent right next door. Confirmed
 * byte-identical to the ROM's own instructions (register for
 * register, operand for operand) via the isolated `cpp`/`agbcc`/`as`
 * + `objcopy`/`cmp` pipeline against `baserom.gba`'s raw bytes at
 * `0x08026BC0`-`0x08026BF8` (the only difference being the `bl
 * sub_8025460` relocation site, which resolves correctly once
 * linked), plus a full clean `rm -rf build && make NON_MATCHING=1
 * report` (no warnings) and `rm -rf build crashbandicootxs.elf
 * crashbandicootxs.gba crashbandicootxs.map && make compare`
 * (`crashbandicootxs.gba: La suma coincide`). Already flush to a
 * 4-byte boundary (56 bytes total) - no trailing `.align 2, 0` gap,
 * unlike `sub_8026628`'s own end-of-function padding quirk.
 *
 * See docs/matching/issue-9-10-0x0800a884-graphics.md for the full
 * write-up of this closure, appended to the section that originally
 * flagged this function raw. */

extern u16 sub_8025460(void *self, s32 x, s32 y, u8 *flagsOut, s32 *hiOut);

s32 sub_8026BC0(void *arg0, s32 x, s32 y)
{
    u8 flagsOut = 0;
    s32 hiOut = 0;
    s32 tileX = x >> 3;
    s32 tileY = y >> 3;

    if (tileX < 0)
        tileX = 0;
    if (tileY < 0)
        tileY = 0;

    sub_8025460(*(void **)((u8 *)arg0 + 0x20), tileX, tileY, &flagsOut, &hiOut);

    return flagsOut;
}
