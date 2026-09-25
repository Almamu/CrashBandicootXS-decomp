#include "core.h"

/* Dedicated deep investigation (GitHub issue #9/#10,
 * docs/matching/issue-9-0x0800a178-graphics.md): `sub_800A178`/
 * `sub_800A420`, the last two functions in the `sub_800A0FC`-through-
 * `sub_800A420` still-raw span `tools/report_units.py` tracked as
 * parked. `sub_800A0FC` itself (the caller of `sub_800A178`, see
 * `asm/code_3_2_11.s`) stays raw/unexamined - its own gate logic still
 * depends on the also-still-raw `sub_8009BE0` (parked NAKED,
 * `actor_part12b.c`), so closing `sub_800A178`/`sub_800A420` alone
 * doesn't unblock it.
 *
 * `docs/rom_map.md` (line ~2624) had already partially flagged
 * `sub_800A178`: "mid-function, unconditionally zeroes `self+0x74` -
 * the same field `UpdateGameFrame`'s level-load branch sets once from
 * `sub_8035E14`'s return value ... consistent with 'total for this
 * level' being cleared and presumably recomputed under some condition,
 * not fully traced here." Reading the real bytes confirms the zeroing
 * itself exactly (unconditional once both leading gate checks pass -
 * see below) and traces the "recompute" side all the way through: the
 * function's second half OR's `mode` bits (`self+0x24 & 0x3`/`0xc`)
 * into `self+0x74` on each successful `sub_8026628` axis-probe hit -
 * i.e. `self+0x74` really is "which movement axes/directions collided
 * this call", zeroed up front and rebuilt bit by bit as each probe
 * fires. `docs/matching/issue-9-0x08007634-actor.md` (line ~206) had
 * also already flagged both functions as built on `sub_8008200`/
 * `sub_8026628`/`sub_8026C3C`/`sub_8026BF8` - two of those four
 * (`sub_8008200`, `sub_8026628`) are already matched this session
 * (`src/graphics/actor_part4.c`, `src/system/game_loop43.c`); this
 * session additionally reads `sub_8026C3C`/`sub_8026BF8` (still raw,
 * `asm/code_3_2_17_266bc.s`) far enough to place them precisely.
 *
 * ## What `sub_800A178`/`sub_800A420` actually do
 *
 * Both operate on the same "hitbox quad" pointer - `self->table[0x10]`/
 * `[0x14]`'s own `sub_803AD7C(self + addr, fn)` trampoline result,
 * i.e. a `{s16 xOff, s16 yOff, u8 w, u8 h}` record straight from this
 * ROM region's already-established convention (`game_loop42.c`'s
 * `struct hitbox_quad`, `actor_part109.c`'s AABB builds) - and share
 * the same "adjust the object's own Q8 position based on where a
 * collision probe says the ground/wall actually is" shape:
 *
 * - **`sub_800A420(self, quad, u8 *outFlag)`**: a single Y-axis
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
 * - **`sub_800A178(self)`**: the larger orchestrator. Gated on two
 *   guard checks (a `sub_803AD7C(self->table[0x38]/[0x3c])`
 *   trampoline truthiness test, then `self+0xc` bit 7) - either
 *   failing returns `0` immediately with no other effect. Once past
 *   both gates: unconditionally zeroes `self+0x74` (the `rom_map.md`
 *   field, confirmed - see above), computes a starting result mask
 *   (`self+0xd` bit 0 expanded to `8`, but only when `self+0x24 &
 *   0xc` is clear), then:
 *   1. If `self+0xd` bit 0 is set, calls `sub_800A420` once
 *      (arg `&flagByte`, a stack-local initialized `0`) and keeps its
 *      boolean result as a "found ground already" flag.
 *   2. If that flag is set, and the result mask is still `0`, forces
 *      it to `8`.
 *   3. If the `sub_800A420` out-flag came back `1`: builds an int
 *      position at the *bottom* of the quad (mirrored the same way),
 *      probes it via `sub_8026C3C(player, pos, &origY)` (see below,
 *      `player` = `gUnknown_03001308` dereferenced) - on a hit, snaps
 *      `self->y` to the probed value and nudges `self->x` by ±1 pixel
 *      depending on `self+0x24 & 3`; on a miss, nudges `self->y` down
 *      one pixel instead. Either way calls `sub_800A420` again
 *      afterward to refresh the "found ground" flag.
 *   4. Three more blocks, each gated on `self+0x24`'s own 2-bit field
 *      (`& 3` for X-axis modes 1/2, `& 0xc` for Y-axis modes 4/8) and
 *      on the "found ground" flag still being `0`: build a plain int
 *      `{x, y}` position via `sub_8008278(dest, mode, quad)`, probe it
 *      through the shared `sub_8026628(player, mode, pos, span,
 *      outValue)` API (already matched, `game_loop43.c`) with a
 *      `span` derived from the quad's own `h`/`w` byte (`h - 16` for
 *      the first X-axis block, `w` for the Y-axis block, `h` for the
 *      second X-axis block - three deliberately different probe
 *      geometries, not a shared constant), and on a hit OR's `mode`
 *      into both `self+0x74` and the running result mask, restoring
 *      the axis coordinate `sub_8026628` didn't touch. Returns the
 *      final result mask.
 *
 * Read together: this is the part-object movement/collision
 * *resolution* step - once some other function (still-raw
 * `sub_800A0FC`) has decided a part object needs a physics update,
 * `sub_800A178` runs a layered probe (fast quad-based floor/wall test
 * first via `sub_800A420`/`sub_8026C3C`, then falling back to the
 * general tile-scan `sub_8026628` API per axis) and snaps the object's
 * position to whatever solid surface each probe finds, recording which
 * axes/directions actually resolved in `self+0x74` for whatever caller
 * reads it next (`UpdateGameFrame`'s own level-load branch is the only
 * other confirmed writer, per `rom_map.md`).
 *
 * ## `sub_8026C3C`/`sub_8026BF8` (still raw, understood only)
 *
 * Both are single-point collision-test siblings of the already-matched
 * `sub_8026A18`/`sub_8026AE8` pair (`game_loop43.c`'s own "what's still
 * open" section already predicted this) - `rom_map.md`'s "15 more
 * reads" pass (line ~2581) had already placed them as "single-point
 * collision-test siblings ... one via the raw terrain streamer and one
 * via the CheckTerrainFlag API"; reading their bytes directly confirms
 * that and pins down the exact shape, both `s32 fn(void *player, struct
 * probe_pos *pos, s32 *outValue)`:
 *
 * - **`sub_8026BF8`**: `player->0x20`'s terrain-data pointer, `pos->x
 *   >> 3`/`pos->y >> 3` tile coords, looked up via `sub_80250BC`
 *   ("the raw terrain streamer" - returns a row pointer or `NULL`).
 *   On a hit, reads a **signed byte** height sample at
 *   `row[pos->x & 7]`, computes `((pos->y >> 3) << 3) + heightByte -
 *   pos->y`, shifts to Q8, and accumulates it into `*outValue`.
 *   Returns `1` on a row hit, `0` if `sub_80250BC` returned `NULL`.
 * - **`sub_8026C3C`**: the exact same shape, but the height byte comes
 *   from `sub_8025228(terrainPtr, tileX, tileY, 0, &scratch)` instead
 *   of a direct row-pointer byte read - the "CheckTerrainFlag"-style
 *   API `sub_8026A18`/`sub_8026AE8` already use via their own
 *   `sub_8025130` calls (same argument shape: base pointer, tile
 *   coords, a submode, an out-parameter). Returns `0` if the returned
 *   signed byte is negative, `1` otherwise, with the same
 *   `(tileY<<3)+byte-pos->y` delta accumulation.
 *
 * Both are Y-axis (floor-height) probes, matching how `sub_800A178`
 * uses them (against `self.y`/`self->y`, never `self.x`). Full byte-
 * exact matching for either wasn't attempted this session - see
 * docs/matching/issue-9-0x0800a178-graphics.md for why (same
 * resistant multi-high-register shape this immediate ROM neighborhood
 * has already hit four times: `sub_8009BE0`, `sub_800CD00`,
 * `sub_800CEAC`, `sub_800CF70`).
 *
 * ## Matching
 *
 * Both `sub_800A178`/`sub_800A420` keep `sb`/`sl`/`r8` (and, for
 * `sub_800A178`, `r7` too) live simultaneously across several `bl`
 * calls and reused for genuinely different values block to block
 * (e.g. `sub_800A420`'s `r8` holds `&gUnknown_03001308` across two
 * separate `sub_8026BF8` calls rather than re-deriving it each time;
 * `sub_800A178`'s `sb` accumulates a result bitmask across five
 * different probe blocks while `sl` independently tracks a "found
 * ground" boolean and `r8` holds the quad pointer for the whole
 * function) - the exact `r7`/`r8`/`sb` cross-block register-reuse
 * shape this ROM neighborhood's own precedent (`sub_800CD00`,
 * `sub_800CEAC`, `sub_800CF70`, all documented in
 * docs/matching/issue-9-10-0x0800aaec-graphics.md and
 * issue-9-10-0x0800ceac-graphics.md) already established resists gcc
 * 2.9 C reconstruction. A single honest isolated-compile attempt
 * against `sub_800A420` (the smaller of the two, with the cleanest
 * worked-out C-level logic of the pair) confirmed the same resistance
 * directly: this compiler's natural register allocation used no high
 * registers at all (`r4`-`r7` sufficed), diverging immediately from
 * the ROM's own deliberate `sb`/`r8` choice - not a near-miss
 * needing one or two register pins, but a structurally different
 * solution. Recognizing the established pattern rather than
 * re-litigating it from scratch, both are transcribed directly as
 * byte-exact NAKED asm instead: the ROM disassembly translated
 * instruction-for-instruction, unified-syntax mnemonics converted to
 * this project's plain/divided-syntax NAKED convention (`adds`->`add`,
 * `movs`->`mov`, `ands`->`and`, `lsls`/`lsrs`->`lsl`/`lsr`,
 * `asrs`->`asr`, `rsbs`->`neg`), with the original `_08XXXXXX:` labels
 * renumbered to GNU-as local numeric labels.
 *
 * Verified byte-exact via the isolated `cpp`/`agbcc`/`as` +
 * `objcopy`/`cmp` pipeline against `baserom.gba`'s own bytes at
 * `0x0800A178`-`0x0800A528` (944 bytes total): the only differing
 * bytes fell into exactly the expected relocation-site set - 16 `bl`
 * calls (`sub_803AD7C`x2, `sub_800A420`x3, `sub_8008200`x2,
 * `sub_8026C3C`x1, `sub_8026628`x3, `sub_8008278`x3, `sub_8026BF8`x2)
 * plus 3 `.4byte gUnknown_03001308` literal-pool words - which resolve
 * correctly once linked. */

/* Follow-up (same investigation): `sub_800A0FC`, the only caller of
 * `sub_800A178` (both live right next to each other, `sub_800A0FC`
 * immediately before `sub_800A178` in ROM and formerly the sole
 * remaining content of `asm/code_3_2_11.s`). It stayed raw the first
 * pass through this cluster because its own gate logic calls
 * `sub_8009BE0` (parked NAKED, `src/graphics/actor_part12b.c`, see
 * `docs/matching/naked-spatial-grid-tail.md`) - at the time that
 * function's semantics were still unresolved. `sub_8009BE0` is now
 * fully understood (a physics/collision step-probe: converts `self`'s
 * position to plain ints via `sub_8008278`, probes it through
 * `sub_8026628` with `mode` as the axis selector, retrying up to 3
 * more times on a miss by nudging Y down), which is enough to close
 * this function's own dispatch logic as real, byte-exact matched C:
 *
 * `sub_800A0FC(self)` returns early with `self+0x68` unchanged unless
 * `self+0xc` bit 7 is set (the same gate `sub_800A178` itself re-checks
 * internally). If set: calls `sub_800A178(self)` and OR's its result
 * bitmask into `self+0x68` (a persistent, cumulative per-object
 * collision-axis mask - distinct from `self+0x74`'s own per-call
 * scratch mask `sub_800A178` zeroes and rebuilds every call), then
 * unconditionally fires `sub_800A050(self)` (the already-matched
 * `self->table+0x70/0x74` trampoline, `src/graphics/actor_part9.c` -
 * a side-effect-only call, its always-`0` return discarded). If
 * `self+0x68` bit 3 (the "Y-axis/mode-8" collision bit `sub_800A178`
 * just OR'd in, if it hit) is now set: clears `self+0xc` bits 0 and 5,
 * then - unless `self+0xd` bit 1 is already set (ground already
 * snapped this call, `sub_800A420`'s own convention) - fires the same
 * `self->table+0x10/0x14` "hitbox quad" trampoline `sub_800A178`
 * itself uses, and runs a `mode == 8` (Y-axis/floor) step-probe via
 * `sub_8009BE0(self, 8, quad)`. If that step-probe does *not* report
 * immediate success (either a full miss, or only succeeding via one of
 * its internal retries - see `sub_8009BE0`'s own doc comment), sets
 * `self+0xc` bit 5 and clears `self+0x68` bit 3 back out - rolling
 * back the "Y axis resolved" bit `sub_800A178`'s cheaper probe had
 * just set, since the more thorough step-probe didn't confirm it
 * cleanly. Returns the (possibly rolled-back) `self+0x68` byte either
 * way.
 *
 * Read together with `sub_800A178`/`sub_800A420`: this is the
 * part-object physics dispatcher - `sub_800A0FC` is the entry point
 * (called by `sub_800A884`'s per-frame reentrancy-guarded wrapper,
 * `docs/rom_map.md` line ~1835), `sub_800A178` does the actual
 * layered collision resolution and reports which axes it resolved,
 * and `sub_800A0FC`'s own tail cross-checks the Y-axis result against
 * a second, independent step-probe (`sub_8009BE0`) before trusting it
 * enough to leave the bit set in the persistent `self+0x68` mask.
 *
 * Matched as real C with no `NON_MATCHING` gap, via direct register
 * pinning to reproduce the ROM's exact register choices at several
 * points where this compiler's natural allocation otherwise diverged:
 * a `flags`(r1)/`bit7`(r0) pair for the leading `self+0xc >> 7` gate
 * test (this compiler naturally reused one register for both the load
 * and the shift result; the ROM keeps them separate), the established
 * "negative-constant register-pinned mask" idiom (`register s32 mask
 * asm("r0") = -0x21`, matching `sub_800A734`'s own precedent,
 * `actor_part48.c`) for the `self+0xc &= ~0x21` clear, a
 * `dByte`(r2)/`shifted`(r0)/`one`(r1)/`bit1`(r0) chain for the
 * `(self+0xd >> 1) & 1` gate (the same bit-1 accessor shape as
 * `sub_800A6C4`, `actor_part14.c`, but needing explicit pinning here
 * since it's inlined alongside other already-pinned locals rather than
 * standing alone), the same `addr`(r0)/`fn`(r1) "compute the trampoline
 * address before loading the function pointer" ordering `sub_800A050`
 * already established for its own `self->table+0x70/0x74` trampoline
 * (here reused for `self->table+0x10/0x14`, the "hitbox quad"
 * accessor - the natural, unpinned C already matched the ROM's
 * register-offset `ldrsh` addressing for the `+0x10` field, since
 * Thumb's `LDRSH` has no immediate-offset encoding and must always
 * materialize the offset into a register), and two more
 * `mask`(r0)/`byte`(r1 or r2)/`result`(r0) pairs (mirroring the
 * `~0x21` clear's own idiom) for the final `self+0xc |= 0x20` and
 * `self+0x68 &= 7` writes, both of which this compiler naturally
 * ordered constant-then-byte in the opposite register slots from the
 * ROM's own choice.
 *
 * Verified via the isolated `cpp`/`agbcc`/`as` + `objcopy`/`cmp`
 * pipeline against `baserom.gba`'s own bytes at `0x0800A0FC`-
 * `0x0800A178` (124 bytes) before integration, then confirmed again
 * via a full clean `make compare` after linking - `sub_800A0FC` is
 * genuinely byte-exact, register-for-register, not just
 * behaviorally equivalent. */

extern s32 sub_803AD7C(void *addr, void *fn);
extern u8 sub_800A178(void *self);
extern s32 sub_800A050(void *self);
extern u8 sub_8009BE0(void *self, s32 mode, void *quad);

u8 sub_800A0FC(void *self)
{
    u8 *p = (u8 *)self + 0x68;
    u8 val = *p;
    register u8 flags asm("r1") = *(u8 *)((u8 *)self + 0xc);
    register u32 bit7 asm("r0");

    bit7 = flags >> 7;
    if (bit7)
    {
        val |= sub_800A178(self);
        *p = val;

        sub_800A050(self);

        {
        register s32 mask asm("r0") = 8;
        register s32 byte asm("r2") = *p;
        register s32 result asm("r0");

        result = mask & byte;
        if (result)
        {
            {
                register s32 mask asm("r0") = -0x21;
                register s32 byte asm("r1") = *(u8 *)((u8 *)self + 0xc);
                register s32 result asm("r0");

                result = mask & byte;
                *(u8 *)((u8 *)self + 0xc) = result;
            }

            {
                register u8 dByte asm("r2") = *(u8 *)((u8 *)self + 0xd);
                register u32 shifted asm("r0");
                register u32 one asm("r1");
                register u32 bit1 asm("r0");

                shifted = dByte >> 1;
                one = 1;
                bit1 = shifted & one;
                if (!bit1)
                {
                    void *quad;
                    void *table = *(void **)((u8 *)self + 0x18);
                    register void *addr asm("r0");
                    register void *fn asm("r1");

                    addr = (u8 *)self + *(s16 *)((u8 *)table + 0x10);
                    fn = *(void **)((u8 *)table + 0x14);
                    quad = (void *)sub_803AD7C(addr, fn);

                    if (!sub_8009BE0(self, 8, quad))
                    {
                        {
                            register s32 mask asm("r0") = 0x20;
                            register s32 byte asm("r1") = *(u8 *)((u8 *)self + 0xc);
                            register s32 result asm("r0");

                            result = mask | byte;
                            *(u8 *)((u8 *)self + 0xc) = result;
                        }
                        {
                            register s32 mask asm("r0") = 7;
                            register s32 byte asm("r2") = *p;
                            register s32 result asm("r0");

                            result = mask & byte;
                            *p = result;
                        }
                    }
                }
            }
        }
        }
    }

    return *(u8 *)((u8 *)self + 0x68);
}

extern s32 sub_803AD7C(void *addr, void *fn);
extern s32 sub_8008200(void *dest, s32 kind, void *rec);
extern s32 sub_8008278(void *dest, s32 kind, void *rec);
extern s32 sub_8026628(void *player, s32 mode, void *pos, s32 span, void *outValue);
extern s32 sub_8026C3C(void *player, void *pos, void *outValue);
extern s32 sub_8026BF8(void *player, void *pos, void *outValue);
extern void *gUnknown_03001308;

/* See the file-level header comment above for the full account of this
 * function's semantics and why it's a NAKED transcription rather than
 * real C. `self`'s only argument; returns the accumulated result
 * bitmask (`self+0x24`'s per-axis mode bits, OR'd in as each
 * `sub_8026628` probe reports a hit). */
NAKED u8 sub_800A178(void *self)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0x1c\n\t"
        "add r6, r0, #0\n\t"
        "mov r0, #0\n\t"
        "mov sb, r0\n\t"
        "add r0, sp, #4\n\t"
        "mov r1, sb\n\t"
        "strb r1, [r0]\n\t"
        "mov r2, #0\n\t"
        "mov sl, r2\n\t"
        "ldr r1, [r6, #0x18]\n\t"
        "mov r2, #0x38\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r6, r0\n\t"
        "ldr r1, [r1, #0x3c]\n\t"
        "bl sub_803AD7C\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne 1f\n\t"
        "b 15f\n\t"
    "1:\n\t"
        "ldrb r1, [r6, #0xc]\n\t"
        "lsr r0, r1, #7\n\t"
        "cmp r0, #0\n\t"
        "bne 2f\n\t"
        "b 15f\n\t"
    "2:\n\t"
        "add r1, r6, #0\n\t"
        "add r1, #0x24\n\t"
        "mov r0, #0xc\n\t"
        "ldrb r2, [r1]\n\t"
        "and r0, r2\n\t"
        "str r1, [sp, #0x18]\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
        "mov r0, #1\n\t"
        "ldrb r1, [r6, #0xd]\n\t"
        "and r0, r1\n\t"
        "neg r1, r0\n\t"
        "orr r1, r0\n\t"
        "asr r1, r1, #0x1f\n\t"
        "mov sb, r1\n\t"
        "mov r0, #8\n\t"
        "mov r2, sb\n\t"
        "and r2, r0\n\t"
        "mov sb, r2\n\t"
    "3:\n\t"
        "mov r0, #0\n\t"
        "str r0, [r6, #0x74]\n\t"
        "ldr r1, [r6, #0x18]\n\t"
        "mov r2, #0x10\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r6, r0\n\t"
        "ldr r1, [r1, #0x14]\n\t"
        "bl sub_803AD7C\n\t"
        "mov r8, r0\n\t"
        "mov r0, #1\n\t"
        "ldrb r1, [r6, #0xd]\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "beq 4f\n\t"
        "add r0, r6, #0\n\t"
        "mov r1, r8\n\t"
        "add r2, sp, #4\n\t"
        "bl sub_800A420\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "mov sl, r0\n\t"
    "4:\n\t"
        "mov r2, sl\n\t"
        "cmp r2, #0\n\t"
        "beq 5f\n\t"
        "mov r0, sb\n\t"
        "cmp r0, #0\n\t"
        "bne 5f\n\t"
        "mov r1, #8\n\t"
        "mov sb, r1\n\t"
    "5:\n\t"
        "add r0, sp, #4\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #1\n\t"
        "bne 12f\n\t"
        "ldr r0, [r6, #4]\n\t"
        "str r0, [sp, #0x10]\n\t"
        "ldr r0, [r6]\n\t"
        "ldr r1, [r6, #4]\n\t"
        "str r0, [sp, #8]\n\t"
        "str r1, [sp, #0xc]\n\t"
        "add r4, sp, #8\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #8\n\t"
        "mov r2, r8\n\t"
        "bl sub_8008200\n\t"
        "ldr r0, [sp, #8]\n\t"
        "asr r0, r0, #8\n\t"
        "str r0, [sp, #8]\n\t"
        "ldr r0, [r4, #4]\n\t"
        "asr r0, r0, #8\n\t"
        "str r0, [r4, #4]\n\t"
        "add r0, r6, #0\n\t"
        "add r0, #0x28\n\t"
        "ldrb r0, [r0]\n\t"
        "lsl r0, r0, #0x1b\n\t"
        "cmp r0, #0\n\t"
        "bge 6f\n\t"
        "mov r2, r8\n\t"
        "ldrb r2, [r2, #4]\n\t"
        "lsr r1, r2, #1\n\t"
        "ldr r0, [sp, #8]\n\t"
        "sub r0, r0, r1\n\t"
        "b 7f\n\t"
    "6:\n\t"
        "mov r0, r8\n\t"
        "ldrb r0, [r0, #4]\n\t"
        "lsr r1, r0, #1\n\t"
        "ldr r0, [sp, #8]\n\t"
        "add r0, r0, r1\n\t"
    "7:\n\t"
        "str r0, [sp, #8]\n\t"
        "ldr r0, 8f\n\t"
        "ldr r0, [r0]\n\t"
        "add r2, sp, #0x10\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8026C3C\n\t"
        "lsl r0, r0, #0x18\n\t"
        "mov r2, sp\n\t"
        "add r2, #5\n\t"
        "mov r1, #0\n\t"
        "strb r1, [r2]\n\t"
        "cmp r0, #0\n\t"
        "beq 11f\n\t"
        "ldr r0, [sp, #0x10]\n\t"
        "ldr r4, 9f\n\t"
        "and r0, r4\n\t"
        "str r0, [r6, #4]\n\t"
        "add r0, r6, #0\n\t"
        "mov r1, r8\n\t"
        "bl sub_800A420\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "mov sl, r0\n\t"
        "mov r0, #3\n\t"
        "ldr r1, [sp, #0x18]\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "beq 13f\n\t"
        "cmp r0, #2\n\t"
        "bne 10f\n\t"
        "ldr r0, [r6]\n\t"
        "add r0, r0, r4\n\t"
        "str r0, [r6]\n\t"
        "b 12f\n\t"
        ".align 2, 0\n"
    "8: .4byte gUnknown_03001308\n"
    "9: .4byte 0xFFFFFF00\n"
    "10:\n\t"
        "ldr r0, [r6]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "add r0, r0, r2\n\t"
        "str r0, [r6]\n\t"
        "b 12f\n\t"
    "11:\n\t"
        "ldr r0, [r6, #4]\n\t"
        "mov r1, #0x80\n\t"
        "lsl r1, r1, #1\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r6, #4]\n\t"
        "add r0, r6, #0\n\t"
        "mov r1, r8\n\t"
        "bl sub_800A420\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "mov sl, r0\n\t"
    "12:\n\t"
        "mov r7, #3\n\t"
        "ldr r2, [sp, #0x18]\n\t"
        "ldrb r2, [r2]\n\t"
        "and r7, r2\n\t"
        "cmp r7, #0\n\t"
        "beq 13f\n\t"
        "mov r0, sl\n\t"
        "cmp r0, #0\n\t"
        "bne 13f\n\t"
        "ldr r0, [r6]\n\t"
        "ldr r1, [r6, #4]\n\t"
        "str r0, [sp, #8]\n\t"
        "str r1, [sp, #0xc]\n\t"
        "mov r1, r8\n\t"
        "ldrb r5, [r1, #5]\n\t"
        "sub r5, #0x10\n\t"
        "ldr r0, [r6]\n\t"
        "str r0, [sp, #0x14]\n\t"
        "add r4, sp, #8\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r7, #0\n\t"
        "mov r2, r8\n\t"
        "bl sub_8008278\n\t"
        "ldr r0, [sp, #8]\n\t"
        "asr r0, r0, #8\n\t"
        "str r0, [sp, #8]\n\t"
        "ldr r0, [r4, #4]\n\t"
        "asr r0, r0, #8\n\t"
        "add r0, #8\n\t"
        "str r0, [r4, #4]\n\t"
        "ldr r0, 16f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, sp, #0x14\n\t"
        "str r1, [sp]\n\t"
        "add r1, r7, #0\n\t"
        "add r2, r4, #0\n\t"
        "add r3, r5, #0\n\t"
        "bl sub_8026628\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq 13f\n\t"
        "ldr r0, [r6, #0x74]\n\t"
        "orr r0, r7\n\t"
        "str r0, [r6, #0x74]\n\t"
        "mov r2, sb\n\t"
        "orr r2, r7\n\t"
        "mov sb, r2\n\t"
        "ldr r0, [sp, #0x14]\n\t"
        "str r0, [r6]\n\t"
    "13:\n\t"
        "mov r7, #0xc\n\t"
        "ldr r0, [sp, #0x18]\n\t"
        "ldrb r0, [r0]\n\t"
        "and r7, r0\n\t"
        "cmp r7, #0\n\t"
        "beq 14f\n\t"
        "mov r1, sl\n\t"
        "cmp r1, #0\n\t"
        "bne 14f\n\t"
        "ldr r0, [r6]\n\t"
        "ldr r1, [r6, #4]\n\t"
        "str r0, [sp, #8]\n\t"
        "str r1, [sp, #0xc]\n\t"
        "mov r2, r8\n\t"
        "ldrb r5, [r2, #4]\n\t"
        "ldr r0, [r6]\n\t"
        "str r0, [sp, #0x14]\n\t"
        "ldr r0, [r6, #4]\n\t"
        "str r0, [sp, #0x10]\n\t"
        "add r4, sp, #8\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r7, #0\n\t"
        "bl sub_8008278\n\t"
        "ldr r0, [sp, #8]\n\t"
        "asr r0, r0, #8\n\t"
        "str r0, [sp, #8]\n\t"
        "ldr r0, [r4, #4]\n\t"
        "asr r0, r0, #8\n\t"
        "str r0, [r4, #4]\n\t"
        "ldr r0, 16f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, sp, #0x10\n\t"
        "str r1, [sp]\n\t"
        "add r1, r7, #0\n\t"
        "add r2, r4, #0\n\t"
        "add r3, r5, #0\n\t"
        "bl sub_8026628\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq 14f\n\t"
        "mov r0, sb\n\t"
        "orr r0, r7\n\t"
        "mov sb, r0\n\t"
        "ldr r0, [r6, #0x74]\n\t"
        "orr r0, r7\n\t"
        "str r0, [r6, #0x74]\n\t"
        "ldr r0, [sp, #0x10]\n\t"
        "str r0, [r6, #4]\n\t"
    "14:\n\t"
        "mov r7, #3\n\t"
        "ldr r1, [sp, #0x18]\n\t"
        "ldrb r1, [r1]\n\t"
        "and r7, r1\n\t"
        "cmp r7, #0\n\t"
        "beq 15f\n\t"
        "mov r2, sl\n\t"
        "cmp r2, #0\n\t"
        "bne 15f\n\t"
        "ldr r0, [r6]\n\t"
        "ldr r1, [r6, #4]\n\t"
        "str r0, [sp, #8]\n\t"
        "str r1, [sp, #0xc]\n\t"
        "mov r0, r8\n\t"
        "ldrb r5, [r0, #5]\n\t"
        "ldr r0, [r6]\n\t"
        "str r0, [sp, #0x14]\n\t"
        "add r4, sp, #8\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r7, #0\n\t"
        "mov r2, r8\n\t"
        "bl sub_8008278\n\t"
        "ldr r0, [sp, #8]\n\t"
        "asr r0, r0, #8\n\t"
        "str r0, [sp, #8]\n\t"
        "ldr r0, [r4, #4]\n\t"
        "asr r0, r0, #8\n\t"
        "str r0, [r4, #4]\n\t"
        "ldr r0, 16f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, sp, #0x14\n\t"
        "str r1, [sp]\n\t"
        "add r1, r7, #0\n\t"
        "add r2, r4, #0\n\t"
        "add r3, r5, #0\n\t"
        "bl sub_8026628\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq 15f\n\t"
        "ldr r0, [r6, #0x74]\n\t"
        "orr r0, r7\n\t"
        "str r0, [r6, #0x74]\n\t"
        "mov r1, sb\n\t"
        "orr r1, r7\n\t"
        "mov sb, r1\n\t"
        "ldr r0, [sp, #0x14]\n\t"
        "str r0, [r6]\n\t"
    "15:\n\t"
        "mov r0, sb\n\t"
        "add sp, #0x1c\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    "16: .4byte gUnknown_03001308\n"
    );
}

/* See the file-level header comment above for the full account of this
 * function's semantics and why it's a NAKED transcription rather than
 * real C. `self`'s a part object, `quad` its `{s16 xOff, s16 yOff, u8
 * w, u8 h}` hitbox quad pointer (the `self->table[0x10]/[0x14]`
 * trampoline result `sub_800A178` also uses), `outFlag` a caller-owned
 * byte set to `1` only on the specific all-miss-with-bit-1-already-set
 * path described above. Returns the final probe's hit boolean. */
NAKED u8 sub_800A420(void *selfArg, void *quad, u8 *outFlag)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "sub sp, #0xc\n\t"
        "add r4, r0, #0\n\t"
        "add r5, r1, #0\n\t"
        "mov sb, r2\n\t"
        "ldr r0, [r4, #4]\n\t"
        "str r0, [sp, #8]\n\t"
        "ldr r0, [r4]\n\t"
        "ldr r1, [r4, #4]\n\t"
        "str r0, [sp]\n\t"
        "str r1, [sp, #4]\n\t"
        "mov r0, sp\n\t"
        "mov r1, #8\n\t"
        "add r2, r5, #0\n\t"
        "bl sub_8008200\n\t"
        "ldr r0, [sp]\n\t"
        "asr r0, r0, #8\n\t"
        "str r0, [sp]\n\t"
        "ldr r0, [sp, #4]\n\t"
        "asr r2, r0, #8\n\t"
        "str r2, [sp, #4]\n\t"
        "ldrb r1, [r4, #0xd]\n\t"
        "lsr r0, r1, #1\n\t"
        "mov r1, #1\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "bne 1f\n\t"
        "sub r0, r2, #1\n\t"
        "str r0, [sp, #4]\n\t"
    "1:\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x28\n\t"
        "ldrb r0, [r0]\n\t"
        "lsl r0, r0, #0x1b\n\t"
        "cmp r0, #0\n\t"
        "bge 2f\n\t"
        "ldrb r5, [r5, #4]\n\t"
        "lsr r1, r5, #1\n\t"
        "ldr r0, [sp]\n\t"
        "sub r0, r0, r1\n\t"
        "b 3f\n\t"
    "2:\n\t"
        "ldrb r5, [r5, #4]\n\t"
        "lsr r1, r5, #1\n\t"
        "ldr r0, [sp]\n\t"
        "add r0, r0, r1\n\t"
    "3:\n\t"
        "str r0, [sp]\n\t"
        "ldr r2, 5f\n\t"
        "mov r8, r2\n\t"
        "ldr r0, [r2]\n\t"
        "add r6, sp, #8\n\t"
        "mov r1, sp\n\t"
        "add r2, r6, #0\n\t"
        "bl sub_8026BF8\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r5, r0, #0x18\n\t"
        "cmp r5, #0\n\t"
        "beq 7f\n\t"
        "ldr r3, [sp, #8]\n\t"
        "ldr r0, 6f\n\t"
        "and r3, r0\n\t"
        "str r3, [r4, #4]\n\t"
        "ldrb r2, [r4, #0xd]\n\t"
        "lsr r0, r2, #1\n\t"
        "mov r1, #1\n\t"
        "and r0, r1\n\t"
        "add r1, r2, #0\n\t"
        "cmp r0, #0\n\t"
        "bne 4f\n\t"
        "ldr r2, 6f\n\t"
        "add r0, r3, r2\n\t"
        "str r0, [r4, #4]\n\t"
    "4:\n\t"
        "mov r0, #2\n\t"
        "orr r0, r1\n\t"
        "b 12f\n\t"
        ".align 2, 0\n"
    "5: .4byte gUnknown_03001308\n"
    "6: .4byte 0xFFFFFF00\n"
    "7:\n\t"
        "ldrb r1, [r4, #0xd]\n\t"
        "lsr r0, r1, #1\n\t"
        "mov r7, #1\n\t"
        "and r0, r7\n\t"
        "cmp r0, #0\n\t"
        "bne 10f\n\t"
        "ldr r0, [sp, #4]\n\t"
        "add r0, #1\n\t"
        "str r0, [sp, #4]\n\t"
        "mov r1, r8\n\t"
        "ldr r0, [r1]\n\t"
        "mov r1, sp\n\t"
        "add r2, r6, #0\n\t"
        "bl sub_8026BF8\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r5, r0, #0x18\n\t"
        "cmp r5, #0\n\t"
        "beq 9f\n\t"
        "ldr r0, [sp, #8]\n\t"
        "ldr r1, 8f\n\t"
        "and r0, r1\n\t"
        "str r0, [r4, #4]\n\t"
        "mov r0, #2\n\t"
        "ldrb r2, [r4, #0xd]\n\t"
        "orr r0, r2\n\t"
        "b 12f\n\t"
        ".align 2, 0\n"
    "8: .4byte 0xFFFFFF00\n"
    "9:\n\t"
        "ldrb r1, [r4, #0xd]\n\t"
        "lsr r0, r1, #1\n\t"
        "and r0, r7\n\t"
        "cmp r0, #0\n\t"
        "beq 11f\n\t"
    "10:\n\t"
        "mov r0, #1\n\t"
        "mov r2, sb\n\t"
        "strb r0, [r2]\n\t"
    "11:\n\t"
        "mov r0, #3\n\t"
        "neg r0, r0\n\t"
        "and r0, r1\n\t"
    "12:\n\t"
        "strb r0, [r4, #0xd]\n\t"
        "add r0, r5, #0\n\t"
        "add sp, #0xc\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
    );
}
