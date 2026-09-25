# Issue #9/#10: `sub_800A178`/`sub_800A420` (graphics)

Dedicated deep-investigation session against the last two functions in
the `tools/report_units.py`-tracked `sub_800A0FC`-through-`sub_800A420`
still-raw span (`base_object=None`): `sub_800A178` (ROM `0x0800A178`,
680 bytes) and `sub_800A420` (ROM `0x0800A420`, 264 bytes).
`sub_800A0FC` itself, the only caller of `sub_800A178` (both live in
`asm/code_3_2_11.s`), stays raw - its own gate logic depends on
`sub_8009BE0` (parked NAKED, `actor_part12b.c`,
[naked-spatial-grid-tail.md](./naked-spatial-grid-tail.md)), so closing
`sub_800A178`/`sub_800A420` alone doesn't unblock it.

## Starting point

`docs/rom_map.md` (line ~2624) had already partially flagged
`sub_800A178`: "mid-function, unconditionally zeroes `self+0x74` - the
same field `UpdateGameFrame`'s level-load branch sets once from
`sub_8035E14`'s return value ... consistent with 'total for this level'
being cleared and presumably recomputed under some condition, not fully
traced here." `docs/matching/issue-9-0x08007634-actor.md` (line ~206)
had also already flagged both functions as built on `sub_8008200`/
`sub_8026628`/`sub_8026C3C`/`sub_8026BF8` - two of those four
(`sub_8008200`, `sub_8026628`) are already matched this session
(`src/graphics/actor_part4.c`, `src/system/game_loop43.c`), leaving
only `sub_8026C3C`/`sub_8026BF8` genuinely unexamined.

## Reading the real bytes

Both functions sit in `asm/code_3_2_11.s`, immediately after the
still-raw `sub_800A0FC` and immediately before the already-matched
`sub_800A528`/`sub_800A590` (`src/graphics/actor_part47.c`, which
itself already notes "still-raw `sub_800A0FC`/`sub_800A178`/
`sub_800A420`" as its own neighbors). `sub_8026C3C`/`sub_8026BF8`
themselves live in `asm/code_3_2_17_266bc.s` (the file `sub_8026628`
was split out of, right after it).

### `sub_8026C3C`/`sub_8026BF8`: single-point terrain-height probes

`docs/rom_map.md`'s "15 more reads" pass (line ~2581) had already
placed these as "single-point collision-test siblings of
`sub_8026A18`/`sub_8026AE8` [the axis resolvers `sub_8026628` already
dispatches to], one via the raw terrain streamer and one via the
`CheckTerrainFlag` API." Reading their bytes directly confirms and
sharpens that: both are `s32 fn(void *player, struct probe_pos *pos,
s32 *outValue)`, computing `pos->x >> 3`/`pos->y >> 3` tile coords from
`player->0x20`'s terrain-data pointer:

- **`sub_8026BF8`** looks the tile row up via `sub_80250BC` ("the raw
  terrain streamer" - returns a row pointer, or `NULL` on a miss).
  On a hit, reads a **signed byte** height sample at
  `row[pos->x & 7]`, computes `((pos->y >> 3) << 3) + heightByte -
  pos->y`, shifts to Q8, and accumulates it into `*outValue`. Returns
  `1` on a row hit, `0` if `sub_80250BC` returned `NULL`.
- **`sub_8026C3C`** is the exact same shape, but the height byte comes
  from `sub_8025228(terrainPtr, tileX, tileY, 0, &scratch)` instead of
  a direct row-pointer byte read - the "CheckTerrainFlag"-style API
  `sub_8026A18`/`sub_8026AE8` already use via their own `sub_8025130`
  calls (same argument shape: base pointer, tile coords, a submode, an
  out-parameter - see [issue-9-10-41-0x08026628-game-loop.md](./issue-9-10-41-0x08026628-game-loop.md)).
  Returns `0` if the returned signed byte is negative, `1` otherwise,
  with the same `(tileY<<3)+byte-pos->y` delta accumulation.

Both are Y-axis (floor-height) probes - matching how `sub_800A178`
only ever uses them against `self.y`/`self->y`, never `self.x`. Neither
was attempted as a byte-exact C match this session (see "Matching"
below); this account is only deep enough to place both callers'
argument roles precisely.

### `sub_800A420(void *self, struct hitbox_quad *quad, u8 *outFlag)`

A single Y-axis "floor" probe. Builds an int `{x, y}` position at the
*bottom* of `quad` (`self.x`/`self.y + (quad->yOff + quad->h) << 8`,
via the already-matched `sub_8008200(dest, 8, quad)`, nudged left/right
by half the quad's width depending on `self+0x28` bit 4's mirror flag -
the same established convention `game_loop43.c`/`actor_part109.c`
document), then probes it via `sub_8026BF8(*gUnknown_03001308, &pos,
&origY)` where `origY` is `self.y`'s own original (unmodified) Q8
value, kept aside as the probe's out-parameter target.

- On a hit: snaps `self->y` to the probed value (`origY & ~0xFF`),
  additionally moving it one pixel higher (`-= 0x100`) if `self+0xd`
  bit 1 was clear on entry, then sets `self+0xd` bit 1.
- On a miss: if `self+0xd` bit 1 was already set, skips straight to the
  tail (no retry). Otherwise retries one integer pixel lower
  (`pos.y += 1`) via a second `sub_8026BF8` call:
  - if *that* hits, snaps `self->y` to its own `origY` and sets
    `self+0xd` bit 1 (same as the first-hit path);
  - if it also misses, `*outFlag` is set to `1` only on the path where
    bit 1 was set from the very start (i.e. never on this specific
    retry-then-miss path) - `self+0xd` bit 1 always ends up cleared on
    any all-miss outcome, regardless of which sub-path was taken.

Returns whichever `sub_8026BF8` call's hit boolean was computed last.
(The `outFlag`/bit-1 gating looks like a redundant double-test of the
same bit at the disassembly level - `sub_800A4C4`'s own bit-1 check,
then a second bit-1 check at `sub_800A500` that's always false whenever
control actually reaches it - but it reconstructs cleanly as two
sequential `if (!(flags & 2))` checks in the natural C source shape,
the second one simply re-testing a flag that didn't change in between,
not a compiler artifact.)

### `sub_800A178(void *self)`

The larger orchestrator, gated on two checks - a
`sub_803AD7C(self->table[0x38]/[0x3c])` trampoline truthiness test,
then `self+0xc` bit 7 - either failing returns `0` immediately with no
other effect. Once past both:

1. **Zeroes `self+0x74` unconditionally** - confirming
   `docs/rom_map.md`'s partial note in full: this is a genuine,
   unconditional write (once the two gates pass), and reading the rest
   of the function completes the "recomputed under some condition"
   half that note left open - see "Confirming `self+0x74`" below.
2. Computes a starting result mask: `self+0xd` bit 0 expanded to `8`,
   but only when `self+0x24 & 0xc` is clear (otherwise the mask starts
   at `0`).
3. If `self+0xd` bit 0 is set, calls `sub_800A420` once (with a
   stack-local `u8` initialized `0` as `outFlag`) and keeps its return
   value as a "found ground already" flag (`sl`). If that flag is set
   and the result mask is still `0`, forces the mask to `8`.
4. If the `sub_800A420` call's `outFlag` came back `1`: builds an int
   position at the bottom of the quad (same mirrored-half-width
   adjustment as `sub_800A420`'s own), probes it via
   `sub_8026C3C(player, pos, &origY)`. On a hit: snaps `self->y` to the
   probed value and nudges `self->x` by ±1 pixel depending on
   `self+0x24 & 3` (`2` -> left, any other nonzero -> right, `0` -> no
   nudge). On a miss: nudges `self->y` down one pixel instead. Either
   way, calls `sub_800A420` again afterward to refresh the "found
   ground" flag (`sl`).
5. Three more blocks, each gated on `self+0x24`'s own 2-bit sub-fields
   (`& 3` for the X-axis modes `1`/`2`, `& 0xc` for the Y-axis modes
   `4`/`8`) and on `sl` still being `0`: build a plain int `{x, y}`
   position via the already-matched `sub_8008278(dest, mode, quad)`,
   probe it through the already-matched `sub_8026628(player, mode,
   pos, span, outValue)` tile-scan API, with `span` taken from the
   quad's own `h`/`w` byte - **three deliberately different probe
   geometries**, not a shared constant: `quad->h - 16` for the first
   X-axis block, `quad->w` for the Y-axis block, `quad->h` (full) for
   the second X-axis block. On a hit, OR's `mode` into both
   `self+0x74` and the running result mask, and restores whichever
   coordinate `sub_8026628` didn't touch (`sub_8026628`'s own
   `outValue` only ever carries the probed axis; the other axis is
   restored from a stack-cached original).
6. Returns the final result mask.

### Confirming `self+0x74`

`self+0x74` starts this call zeroed (step 1), then only ever gets
OR'd with a `mode` value (`1`/`2`/`4`/`8`, `self+0x24`'s own per-axis
bits) at each of the three `sub_8026628` probe blocks in step 5, each
time gated on that probe actually reporting a hit. So `self+0x74` is a
**per-call bitmask of which movement axes/directions actually resolved
a collision this call** - zeroed at the top of every `sub_800A178`
invocation and rebuilt bit by bit as each axis probe fires, exactly
matching `docs/rom_map.md`'s "'total for this level' being cleared and
presumably recomputed" framing in spirit (the "presumably recomputed"
half is now fully traced: it's a per-axis hit mask, not a level-wide
total in the way the first flagged write site - `UpdateGameFrame`'s
level-load branch - might have suggested; the two write sites share a
field offset but serve different purposes, one per-level-load, one
per-physics-call).

## Matching

Neither function was attempted as a byte-exact C reconstruction.
Both keep `sb`/`sl`/`r8` (and, for `sub_800A178`, `r7` too) live
simultaneously across many `bl` calls, reused for genuinely different
values block to block:

- `sub_800A420`'s `r8` holds `&gUnknown_03001308` across two separate
  `sub_8026BF8` calls (a conservative re-derive-via-cached-address
  idiom, not a straight cached value) rather than re-fetching the
  literal pool address each time; `sb` holds `outFlag` for the whole
  function despite only being dereferenced once, right at the end.
- `sub_800A178`'s `sb` accumulates a result bitmask across five
  different probe blocks while `sl` independently tracks the "found
  ground" boolean and `r8` holds the quad pointer for the entire
  function; `r7` gets reused three separate times for three different
  `self+0x24`-derived mode values in unrelated blocks.

This is the exact `r7`/`r8`/`sb` cross-block register-reuse shape this
immediate ROM neighborhood has already independently established as
resistant to gcc 2.9 C reconstruction, four times over: `sub_8009BE0`
([naked-spatial-grid-tail.md](./naked-spatial-grid-tail.md)),
`sub_800CD00`, `sub_800CEAC`, `sub_800CF70`
([issue-9-10-0x0800aaec-graphics.md](./issue-9-10-0x0800aaec-graphics.md),
[issue-9-10-0x0800ceac-graphics.md](./issue-9-10-0x0800ceac-graphics.md)).
Rather than re-litigating that from scratch across two more, larger
functions, this session ran a single honest isolated-compile attempt
against `sub_800A420` (the smaller of the two, with the cleanest
worked-out C-level logic of the pair - see the `outFlag`/bit-1 aside
above) as a direct confirmation: the straightforward C reconstruction
compiled cleanly, but this compiler's natural register allocation used
**no high registers at all** (`r4`-`r7` sufficed for every value,
including the `gUnknown_03001308` address and `outFlag`) - a
structurally different register-allocation solution from the ROM's own
deliberate `sb`/`r8` choice, not a near-miss fixable with one or two
register pins. Recognizing the established pattern, both functions
were transcribed directly as byte-exact NAKED asm instead: the ROM
disassembly translated instruction-for-instruction, unified-syntax
mnemonics converted to this project's plain/divided-syntax NAKED
convention (`adds`->`add`, `movs`->`mov`, `ands`->`and`,
`lsls`/`lsrs`->`lsl`/`lsr`, `asrs`->`asr`, `rsbs`->`neg`), with the
original `_08XXXXXX:` labels renumbered to GNU-as local numeric labels.

Verified byte-exact via the isolated `cpp`/`agbcc`/`as` +
`objcopy`/`cmp` pipeline against `baserom.gba`'s own bytes at
`0x0800A178`-`0x0800A528` (944 bytes, both functions together): the
only differing bytes fell into exactly the expected relocation-site
set - 16 `bl` calls (`sub_803AD7C` x2, `sub_800A420` x3, `sub_8008200`
x2, `sub_8026C3C` x1, `sub_8026628` x3, `sub_8008278` x3, `sub_8026BF8`
x2) plus 3 `.4byte gUnknown_03001308` literal-pool words, all of which
resolve correctly once linked.

## Build layout

New object `src/graphics/actor_part110.c` holds both functions,
inserted in `ldscript.txt` exactly where `sub_800A178`/`sub_800A420`'s
real bytes used to sit: between the trimmed `asm/code_3_2_11.o` (now
ending right after `sub_800A0FC`'s own trailing `bx r1`) and the
already-matched `src/graphics/actor_part47.o` (`sub_800A528` onward).
`asm/code_3_2_11.s` is trimmed accordingly - `sub_800A0FC` unchanged,
everything after it removed.

## Full-ROM verification

`rm -rf build && make NON_MATCHING=1 report` - clean, no warnings from
the new file. `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` - `crashbandicootxs.gba: La suma
coincide` (checksum matches).

## Techniques used

- NAKED transcription as the deliberate, documented default given this
  ROM neighborhood's own established `r7`/`r8`/`sb` cross-block
  register-reuse resistance (four prior confirmed instances) -
  confirmed directly rather than assumed, via one isolated-compile
  attempt against the smaller of the two functions before committing
  to the NAKED path for both.
- Reading `sub_8026C3C`/`sub_8026BF8`'s own raw bytes (not attempting
  to match them) just far enough to pin down both callers' argument
  roles precisely, the same "read the callee enough to place the
  caller" approach `issue-9-10-41-0x08026628-game-loop.md` used for
  `sub_8026AE8`/`sub_8026A18`.
- Cross-referencing `docs/rom_map.md`'s own partial `self+0x74` note
  against the fully-read function body to confirm and complete it,
  rather than leaving the "presumably recomputed" half unresolved.

## Cross-references

- `docs/status/actor.md` - `sub_800A178`/`sub_800A420` moved from
  "Left raw" into "Parked - NAKED transcription"; the `sub_800A0FC`
  "Left raw" entry narrowed to cover only itself.
- `tools/report_units.py` - new unit at `0x0800A178` (`base_object:
  None`, category `graphics`); the `0x0800A0FC` unit's comment
  narrowed to describe only `sub_800A0FC` itself.
- `docs/matching/issue-9-0x08007634-actor.md` - the original "Left
  untouched (raw)" entries for both functions (left as-is, per this
  project's append-only convention for per-investigation docs already
  published - this doc supersedes them going forward).

## Follow-up: `sub_800A0FC`, closing the whole span (issue #9/#10)

A second dedicated session against the immediate follow-up: `sub_800A0FC`
(ROM `0x0800A0FC`, 124 bytes), `sub_800A178`'s only caller and, until
now, the sole remaining content of `asm/code_3_2_11.s`. It stayed raw
the first pass through this cluster because its own gate logic calls
`sub_8009BE0` (parked NAKED, `src/graphics/actor_part12b.c`, see
[naked-spatial-grid-tail.md](./naked-spatial-grid-tail.md)) - at the
time that function's own semantics were still unresolved, so closing
`sub_800A178`/`sub_800A420` alone didn't unblock this one. Both facts
changed since: `sub_8009BE0` is now fully understood (a physics/
collision step-probe, confirmed above and in its own doc), and
`sub_800A178` itself is now fully understood too - together, that's
enough to close `sub_800A0FC`'s own dispatch logic.

### Reading the real bytes

`sub_800A0FC` is short enough to read in full directly from
`asm/code_3_2_11.s` (now deleted - it held only this function):

```
push {r4, r5, r6, lr}
adds r4, r0, #0
adds r6, r4, #0
adds r6, #0x68
ldrb r5, [r6]
ldrb r1, [r4, #0xc]
lsrs r0, r1, #7
cmp r0, #0
beq _0800A16C
adds r0, r4, #0
bl sub_800A178
orrs r5, r0
strb r5, [r6]
adds r0, r4, #0
bl sub_800A050
movs r0, #8
ldrb r2, [r6]
ands r0, r2
cmp r0, #0
beq _0800A16C
movs r0, #0x21
rsbs r0, r0, #0
ldrb r1, [r4, #0xc]
ands r0, r1
strb r0, [r4, #0xc]
ldrb r2, [r4, #0xd]
lsrs r0, r2, #1
movs r1, #1
ands r0, r1
cmp r0, #0
bne _0800A16C
ldr r1, [r4, #0x18]
movs r2, #0x10
ldrsh r0, [r1, r2]
adds r0, r4, r0
ldr r1, [r1, #0x14]
bl sub_803AD7C
adds r2, r0, #0
adds r0, r4, #0
movs r1, #8
bl sub_8009BE0
lsls r0, r0, #0x18
cmp r0, #0
bne _0800A16C
movs r0, #0x20
ldrb r1, [r4, #0xc]
orrs r0, r1
strb r0, [r4, #0xc]
movs r0, #7
ldrb r2, [r6]
ands r0, r2
strb r0, [r6]
_0800A16C:
adds r0, r4, #0
adds r0, #0x68
ldrb r0, [r0]
pop {r4, r5, r6}
pop {r1}
bx r1
```

There's a third callee alongside the two flagged in the task ticket:
`sub_800A050` (already matched, `src/graphics/actor_part9.c`) - a
fire-and-forget `self->table+0x70/0x74` trampoline call, its always-`0`
return discarded. It sits between the `sub_800A178` call and the
`self+0x68` bit-3 recheck, with no other effect on this function's
control flow.

### Semantics

`sub_800A0FC(self)` returns `self+0x68` (a byte) unchanged unless
`self+0xc` bit 7 is set - the same gate `sub_800A178` itself
re-checks internally as its own second gate. Once past it:

1. Calls `sub_800A178(self)` and OR's its result bitmask into
   `self+0x68`. This is a **persistent, cumulative per-object
   collision-axis mask** - distinct from `self+0x74`'s own per-call
   scratch mask that `sub_800A178` zeroes and rebuilds every call (see
   above). `self+0x68` just accumulates whatever axis bits
   `sub_800A178` reports, call after call, with nothing in this
   function ever clearing it back out except the one narrow rollback
   in step 3 below.
2. Fires `sub_800A050(self)` unconditionally - a side-effect-only call,
   its return value never used.
3. If `self+0x68` bit 3 (the Y-axis/"mode 8" bit `sub_800A178` just
   OR'd in, if its own probes hit) is now set: clears `self+0xc` bits 0
   and 5, then - unless `self+0xd` bit 1 is already set (ground
   already snapped this call, `sub_800A420`'s own convention, see
   above) - fires the *same* `self->table+0x10/0x14` "hitbox quad"
   trampoline `sub_800A178` itself uses (confirmed identical: table
   pointer read, signed-halfword offset at `+0x10`, function pointer at
   `+0x14`, `sub_803AD7C(self+offset, fn)`), and runs a `mode == 8`
   (Y-axis/floor, confirmed by `game_loop43.c`'s own `sub_8026628` mode
   table) step-probe via `sub_8009BE0(self, 8, quad)`. If that
   step-probe does *not* report immediate success (either a full miss,
   or only succeeding via one of its own internal retries - see
   `sub_8009BE0`'s doc comment), sets `self+0xc` bit 5 and clears
   `self+0x68` bit 3 back out - **rolling back the "Y axis resolved"
   bit `sub_800A178`'s own probes had just set**, since the more
   thorough, independent step-probe didn't confirm it cleanly.
4. Returns the (possibly rolled-back) `self+0x68` byte either way.

Read together with `sub_800A178`/`sub_800A420`: this is the
part-object physics dispatcher. `sub_800A0FC` is the entry point
(called by `sub_800A884`'s per-frame reentrancy-guarded wrapper,
`docs/rom_map.md` line ~1835 - `sub_800A884` itself fires its own
`self->table+0x70` trampoline before calling in, a *different* `self`
than `sub_800A050`'s own table+0x70/0x74 call operates on, so these
aren't the same trampoline invocation despite sharing an offset
convention). `sub_800A178` does the actual layered collision
resolution and reports which axes it resolved this call; `sub_800A0FC`
cross-checks the Y-axis result specifically against a second,
independent step-probe (`sub_8009BE0`) before trusting it enough to
leave the bit set in the persistent `self+0x68` mask - a "cheap probe,
then confirm" two-stage design for the one axis (gravity/floor) that
matters most for basing the object.

### Matching

Attempted, and closed, as real C - no `NON_MATCHING` fallback needed.
The straightforward translation compiled to the right *shape*
immediately (same branches, same calls, same bit tests), but diverged
from the ROM's own register choices at several small, specific points;
each was closed with a targeted, narrow fix rather than a blanket
NAKED transcription, following the project's usual "isolated-compile
first, only fall back if it's a genuine structural gap" order:

- **Leading `self+0xc >> 7` gate**: this compiler's natural allocation
  reused one register for both the `ldrb` and the `lsr` result; the
  ROM keeps them in separate registers (`ldrb r1, ...` then
  `lsrs r0, r1, #7`). Closed with a `flags`(`r1`)/`bit7`(`r0`)
  register-variable pair - same "pin source and destination to
  different explicit registers" technique already used throughout this
  ROM neighborhood.
- **`self+0xc &= ~0x21`**: the established "negative-constant
  register-pinned mask" idiom already confirmed for `sub_800A734`
  (`register s32 mask asm("r0") = -0x21`, `actor_part48.c`) reused
  unchanged here - the ROM materializes `-0x21` via `movs`+`rsbs`
  rather than folding the AND mask to a literal, since Thumb's `ANDS`
  has no immediate form.
- **`self+0x68 & 8` test**: same class of gap as the leading gate, but
  needing a full `mask`(`r0`=8)/`byte`(`r2`=`*p`)/`result`(`r0`) triple
  to match the ROM's specific `movs r0,#8` / `ldrb r2,[r6]` /
  `ands r0,r2` order and register choice (this compiler's unpinned
  natural order loaded the byte into `r1` first, then the constant into
  `r0` - operand order reversed from the ROM's constant-first choice).
  This one only showed up as a real mismatch during the full-ROM
  `make compare` (the isolated single-function byte compare against
  just `0x0800A0FC`-`0x0800A178` caught it directly too, once checked
  carefully - see "Verification" below).
- **`(self+0xd >> 1) & 1` gate**: the same bit-1 accessor shape as the
  already-matched `sub_800A6C4` (`actor_part14.c`, `return
  (self[0xd]>>1)&1;`, no pinning needed there since it's a standalone
  function) - but inlined here alongside other already-pinned locals,
  this compiler's allocator picked different registers than the ROM at
  every step. Closed with a 4-register chain (`dByte` r2, `shifted` r0,
  `one` r1, `bit1` r0) reproducing the ROM's exact
  `ldrb r2,.../lsrs r0,r2,#1/movs r1,#1/ands r0,r1` sequence.
- **`self->table+0x10/0x14` trampoline**: reused `sub_800A050`'s own
  already-established "compute the trampoline address before loading
  the function pointer" register-pinned ordering (`addr` r0 computed
  first, `fn` r1 loaded second, reusing the dying `table` pointer
  register) - the same idiom, just retargeted from offset `0x70/0x74`
  to `0x10/0x14`. The signed-halfword offset load itself
  (`*(s16*)(table+0x10)`) needed no pinning at all: Thumb's `LDRSH` has
  no immediate-offset encoding, so the compiler is forced to
  materialize the `0x10` constant into a register and emit a
  register-offset `ldrsh` regardless of source phrasing, which already
  matches the ROM's own `movs r2,#0x10`/`ldrsh r0,[r1,r2]` shape
  unpinned.
- **`sub_8009BE0`'s return value truthy test**: the ROM narrows the
  return value via `lsls r0,r0,#0x18` before the zero test (Thumb has
  no `AND #0xff` immediate form, and a sub-word return value isn't
  guaranteed clean in the upper bits at the call site). This fell out
  automatically once `sub_8009BE0` was locally declared returning `u8`
  (matching how `sub_800A420` is itself declared `u8` despite the same
  narrowing dance appearing at *its* own call sites in `sub_800A178`) -
  declaring it `s32` instead skipped the narrowing entirely and
  produced a plain `cmp r0,#0` with no `lsl`, an immediate byte
  mismatch.
- **Trailing `self+0xc |= 0x20` and `self+0x68 &= 7` writes**: both
  needed the same `mask`(`r0`)/`byte`(`r1`-or-`r2`)/`result`(`r0`)
  register-pinned pattern as the `~0x21` clear above, since this
  compiler's unpinned natural order for both was byte-then-constant,
  opposite the ROM's constant-then-byte choice; the second one also
  needed the byte register specifically pinned to `r2` (not `r1`) to
  match the ROM's own choice there.

No genuinely resistant register-allocation gap turned up anywhere in
this function - every divergence was a "compiler's unpinned natural
choice differs from the ROM's own, but both are valid allocations"
case, closed with a direct, targeted register pin rather than an
opaque `asm volatile` block or a NAKED fallback.

### Verification

Isolated `cpp`/`agbcc`/`as` + `objcopy`/`cmp` pipeline against
`baserom.gba`'s own bytes at `0x0800A0FC`-`0x0800A178` (124 bytes)
confirmed byte-exact before integration - this caught the `self+0x68 &
8` test's constant-vs-byte register-order gap directly (a 5-byte
mismatch at `0x0800A11E`-`0x0800A122` on the first full-ROM `make
compare` attempt, traced back to that one test not yet being
register-pinned) once compared carefully against the ROM's own
disassembly rather than skimmed. After the fix: full clean `rm -rf
build && make NON_MATCHING=1 report` (no warnings from the new
function), then `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` - `crashbandicootxs.gba: La suma
coincide` (checksum matches).

### Build layout

`sub_800A0FC` was added directly to `src/graphics/actor_part110.c`
(prepended before `sub_800A178`), the same translation unit as the
NAKED `sub_800A178`/`sub_800A420` - the same "NAKED function sharing a
file with matched ones" precedent already established for `sub_8008044`/
`actor_part3.c`. `asm/code_3_2_11.s` is retired entirely (it held only
`sub_800A0FC`) and removed from `ldscript.txt`; `actor_part110.o` now
sits directly between the trimmed `actor_part9.o` and `actor_part47.o`
in link order, with no raw `.s` gap between `actor_part9.o` and
`actor_part110.o` any more.

### Cross-references (follow-up)

- `docs/status/actor.md` - new `src/graphics/actor_part110.c` bullet
  in "Matched" for `sub_800A0FC`; the stale "Left raw" entry for it
  removed; the `actor_part14.c` bullet's "large raw span" note
  corrected (that span was never fully raw - `sub_800A528`/
  `sub_800A590` were already matched in `actor_part47.c`).
- `tools/report_units.py` - the `0x0800A0FC` unit's `base_object`
  changed from `None` to `"src/graphics/actor_part110.o"` (matched);
  the `0x0800A178` unit's comment updated to note it now shares that
  object with the matched `sub_800A0FC`.
- `ldscript.txt` - `asm/code_3_2_11.o` line removed.

## Second follow-up: `sub_8026BF8`/`sub_8026C3C`, closing the last two
## flagged callees (issue #9/#10)

A third dedicated pass, this time a matching-only session against the
two callees this doc's own "`sub_8026C3C`/`sub_8026BF8`: single-point
terrain-height probes" section (above) had already fully worked out
semantically but explicitly left unattempted as C reconstructions
("Neither was attempted as a byte-exact C match this session"). Nothing
about their semantics changed from the account above - `s32 fn(void
*player, struct probe_pos *pos, s32 *outValue)`, `player+0x20`'s
terrain-data pointer, `pos->x>>3`/`pos->y>>3` tile coords, a signed
height-byte lookup (`sub_80250BC` row read for `sub_8026BF8`,
`sub_8025228` for `sub_8026C3C`), `((tileY<<3)+height-pos->y)<<8`
accumulated into `*outValue` - this pass just closes them as real C.

### Matching

Both matched as real C, no `NON_MATCHING` gap and no NAKED fallback
needed - the earlier "same resistant multi-high-register shape this ROM
neighborhood already hit four times" caution (about `sub_800A178`/
`sub_800A420` themselves) turned out not to apply to these two smaller
leaf functions, which only ever need `r0`-`r6`, matching the ROM's own
register choices directly once the right C shape was found:

- Branch polarity: the natural `if (row == NULL) return 0; ... hit
  code ...` phrasing compiles to the *opposite* branch polarity from the
  ROM (this compiler tests the null case and skips forward over the hit
  code; the ROM tests the hit case and skips forward over the `return
  0`). Rewriting as `if (row != NULL) { ... hit code ...; return 1; }
  return 0;` (and the equivalent `if (height >= 0) { ... } return 0;`
  for `sub_8026C3C`) reproduces the ROM's own `bne`-to-hit-code shape
  exactly.
- Load-order: the ROM reloads `pos->y` (and, for `sub_8026BF8`,
  `pos->x`) from memory a second time inside the hit block, in a
  specific order (`y` before `x`), even though the pre-shifted `tileY`
  is still live in a callee-saved register from the top of the
  function. Matched by hoisting `s32 y = pos->y;` as its own local
  right at the top of the hit block (mirroring the ROM's own early
  reload) and writing the final accumulation with the row-byte/height
  operand evaluated first - `height + (tileY << 3) - y` for
  `sub_8026BF8` (whose height depends on the row lookup, so evaluating
  it first also keeps the byte-load and its address computation
  contiguous, avoiding the compiler's scheduler hoisting the unrelated
  `tileY << 3` computation in between - the same "fold into a single
  expression / keep the two loads in the ROM's actual order" scheduler
  workaround `docs/matching/issue-68-0x08039818-audio.md` already
  documented for an unrelated function), `(tileY << 3) + height - y` for
  `sub_8026C3C` (whose `height` is already available before the branch,
  via the call's own return value, so no equivalent hoist risk exists
  there).
- `sub_8026BF8`'s signed-byte row read: this specific agbcc build never
  emits a Thumb `LDRSB` (register-offset signed-byte load) from *any*
  C-level signed-byte array/pointer read, confirmed categorically with a
  minimal standalone `s32 f(s8 *arr, s32 i) { return arr[i]; }` test,
  which still lowers to `ldrb` + `lsl #24` + `asr #24` regardless of how
  the array access is phrased (a separate `s8 height = row[idx];` local
  vs. inlined into the final expression made no difference). Thumb's
  `LDRSB` has no immediate-offset encoding at all (unlike `LDRB`), so
  once the base+index add is folded into a single address register this
  compiler's cost model apparently never reconsiders the register-offset
  signed load - it always prefers the unsigned-byte-load-plus-shift
  fallback. Closed with a narrow, targeted inline-asm materialization of
  the exact `mov r1,#0`/`ldrsb r1,[r0,r1]` instruction pair the ROM
  itself uses - `register s8 *addr asm("r0")` pinned to `row + (pos->x &
  7)`, `register s32 height asm("r1")` initialized to `0` and used as
  both the register-offset input and the load's destination
  (`"+r"(height)`), not a blanket opaque block - the one instruction
  class this compiler categorically can't select on its own.

Confirmed byte-exact via the isolated `cpp`/`agbcc`/`as` +
`objcopy`/`cmp` pipeline against `baserom.gba`'s own bytes at
`0x08026BF8`-`0x08026C80` (136 bytes, both functions) - the only
differing bytes are the two expected `bl` relocation sites (`sub_80250BC`,
`sub_8025228`), both resolving correctly once linked. Full clean `rm -rf
build && make NON_MATCHING=1 report` (no warnings) and `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare` - `crashbandicootxs.gba: La suma coincide`.

One documentation-writing pitfall worth recording: this agbcc build's
`cpp` will occasionally warn about "missing terminating `'`/`\"`
character" for ordinary English apostrophes/quotes inside `/* ... */`
doc comments (an old-cpp lexer quirk scanning raw comment text for quote
balance) - harmless to compilation but a real warning that breaks the
"no warnings" full-report requirement, so doc comments in this codebase
should stick to backticks for code references and avoid stray straight
apostrophes/double-quoted phrases spanning multiple comment lines.
Separately, and more seriously: the glob-style path `src/**/*.c`
(intended to mean "every `.c` file under `src/`") literally contains the
C block-comment closer `*/` as a substring, which prematurely ends the
enclosing `/* ... */` doc comment and turns the rest of the comment into
real (broken) code - avoid that exact character sequence in comments
project-wide; spell it out in prose instead (e.g. "every `.c` file under
`src/`").

### Bonus: `sub_8026C80`/`sub_8026C8C`, two adjacent UNUSED stubs

The same survey that originally flagged `sub_8026C3C`/`sub_8026BF8`
("15 more reads" pass, `docs/rom_map.md` line ~2581) also flagged the
two tiny functions immediately following them in the same file as
"possibly trampolines/stubs". Read directly and matched alongside the
main pair, since both turned out to be trivially small (10 and 4 bytes)
and matched on the first isolated-compile attempt with zero register
pins:

- **`sub_8026C80(void *arg0, s32 arg1, s32 *arg2)`**: `arg0` is never
  touched (dead parameter). If `arg1` is nonzero, dereferences `arg2`
  and discards the result - a real load with no observable effect,
  needing `arg2` typed `volatile` to survive optimization (matching the
  ROM's own unconditional `ldr r0, [r2]`, whose loaded value is
  immediately clobbered by the trailing `movs r0, #0`). Always returns
  `0` regardless of which path was taken.
- **`sub_8026C8C(void)`**: unconditionally returns `0`, touching no
  parameters at all. Not given the `nullsub_N` name (see
  `docs/naming.md`) since that convention is reserved for a genuinely
  *empty* function body (`bx lr` alone), not a one-instruction
  always-returns-a-constant stub; left as `sub_8026C8C` per the
  project's own "when in doubt, leave it `sub_XXXXXXXX`" guidance.

Both are **UNUSED** - checked every `asm/*.s`, `expected/*.s` (aside
from their own definitions there), and every `.c` file under `src/` for
a `bl`/`.4byte` reference to either symbol, none found. Matched anyway
per this project's standing "genuinely dead code still goes through the
full workflow" convention (`docs/naming.md`) rather than skipped.
Confirmed byte-exact the same way as the main pair, against
`baserom.gba`'s bytes at `0x08026C80`-`0x08026C90` (16 bytes including
the inter-function alignment pad) - zero relocation sites, so this slice
is byte-identical with no exceptions.

### Build layout (second follow-up)

New object `src/system/game_loop45.c` holds all four functions
(`sub_8026BF8`, `sub_8026C3C`, `sub_8026C80`, `sub_8026C8C`), following
the `game_loop43.c`/`game_loop44.c` naming precedent already established
for this exact ROM neighborhood (both matched in the same immediate
area, both reusing `struct probe_pos`/`struct tile_cache` conventions
this new file also reuses rather than redefining differently).
`asm/code_3_2_17_26bf8.s` is trimmed to begin at `sub_8026C90` (the next
still-raw function); `game_loop45.o` is inserted into `ldscript.txt`
directly between `game_loop44.o` and the trimmed `code_3_2_17_26bf8.o`,
exactly where these four functions' real bytes already sat.

### Cross-references (second follow-up)

- `docs/status/game_loop.md` - new bullet in "Matched" for
  `sub_8026BF8`/`sub_8026C3C`/`sub_8026C80`/`sub_8026C8C`
  (`src/system/game_loop45.c`).
- `tools/report_units.py` - new unit at `0x08026BF8`
  (`"src/system/game_loop45.o"`, category `game_loop`); the `0x0800A178`
  unit's comment updated to note all four of `sub_8008200`/
  `sub_8026628`/`sub_8026C3C`/`sub_8026BF8` are now matched, not just
  the first two.
- `ldscript.txt` - `build/crashbandicootxs/src/system/game_loop45.o(.text);`
  line added, between `game_loop44.o` and `code_3_2_17_26bf8.o`.
