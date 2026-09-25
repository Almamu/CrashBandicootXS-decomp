# Issue #13 continued: 0x0800FC70-0x08010A0C

GitHub issue #13 (`0x0800FC70-0x08010A0C`, physics/collision subsystem,
`game_loop` category - see
[docs/matching/issue-13-graphics-fc70.md](issue-13-graphics-fc70.md) for
the first pass's write-up and the category-correction rationale) had
five units left raw after that first pass. This is the write-up for a
second pass against those five.

## Matched (7 of 12 remaining functions)

- **`sub_801089C`** (`src/system/game_loop34.c`, new file) - plays
  cue-3 SFX, then - unless `self->field_08` is the sentinel `0xffff` -
  consumes a slot from the per-record bit-grid (`gUnknown_030012B4`,
  the same `sub_802599C`/`sub_80259D4` accessor pair game_loop12.c/
  game_loop13.c already establish) keyed by `self->field_08`, setting
  the bit only if it wasn't already set. Finally spawns a part object
  (`sub_8025A64`, itself still parked as of game_loop14.c) three tiles
  below `self`'s own position, tagged with the caller's own byte
  argument. `self` here is a `struct actor *` - `self->x`/`self->y`/
  `self->field_08` (include/actor.h) match the record's `+0`/`+4`/`+8`
  fields exactly, and `field_08`'s own doc comment already calls out
  its use "as a 32-bit-word bitmap index", confirmed here by the
  `sub_802599C`/`sub_80259D4` calls. Two gotchas:
  - `sub_8025A64`'s `x`/`y` arguments need wider `s32` types in this
    call site's own local extern declaration than the `u16 x, u16 y`
    prototype game_loop14.c's (still-parked) definition uses - this
    call site's own ROM bytes never truncate the computed Q8-to-tile
    values to 16 bits.
  - The 6th (stack-passed, `u8`) `flag6` argument needs the whole call
    spelled out in inline asm, not just a register pin: Thumb1 has no
    sp-relative byte-store encoding, so the ROM computes the address
    (`add r3, sp, #4`) then a genuine `strb`, while this compiler
    always emits a direct word-sized `str` for a stack-passed byte
    argument regardless of the parameter's declared width - the same
    gap already closed for `sub_8003A60`'s own `sub_8003F30` call in
    `settings_menu8c.c` (see
    [docs/matching/issue-5-overlay-ui-sync.md](issue-5-overlay-ui-sync.md)).
    A dummy 2-word local's address is passed as an unused input operand
    purely to make this compiler reserve the same 8-byte outgoing-
    argument stack slot pair the ROM's own `sub sp, #8`/`add sp, #8`
    frame does.
- **`sub_801071C`/`sub_801075C`** (`src/system/game_loop31.c`, new
  file) - a part-object table-set/tail-call-`sub_8008484` helper (same
  shape as `sub_80119D8`, `actor_part39.c`) that additionally frees
  `self+0x48` (unless it's the sentinel `-1` or already `NULL`) and
  clears `self+0x59` when `self`'s own `+0x4e` state byte is 3, and a
  second helper that re-initializes `self` via `sub_80084A4`, resets
  its table/`+0x59` flag, then resets its own collision-response state
  via `sub_800FEB0` (`game_loop22.c`). Both take/return `struct actor *`
  (matching `sub_8008484`/`sub_80084A4`'s own already-matched
  prototypes in `actor_part6.c`).
- **`sub_8010784`/`sub_80107C4`** (`src/system/game_loop31.c`) - two
  fixed single-octant variants of the Bresenham-line-style stepper
  `sub_800FDC8` (below) implements in full - each walks a fixed number
  of steps along one axis (the 3rd/4th parameters, doubled into the
  classic Bresenham error term), stepping the 1st parameter (`y`) by
  the 5th parameter (`yStep`) whenever the error term overflows (or
  every step, for the other function), returning the accumulated count
  the moment `y` reaches the 6th parameter (`bound`), or `-1` if the
  walk completes without ever reaching it. Both matched with no
  register-allocation gotchas - straightforward translations of the
  ROM's own loop shape.
- **`sub_80109A4`** (`src/system/game_loop30.c`, new file) - unless
  `self`'s own `+0x4d & 0x7f` state is 1, and `testX`/`testY` are both
  within `0x3fff` of `self`'s own `+0`/`+4` position, and `self`'s
  `+0x4e` byte isn't `5`, fires `sub_0800D18C(self)` (the subsystem's
  collision-response commit) - then always clears `self`'s own `+0xc`
  flags bit 3. Two register-pinning gotchas:
  - The initial `self+0x4d & 0x7f` check needs a full inline-asm
    anchor: the ROM computes the field address into r0 first
    (`movs r0,#0x4d; adds r0,r0,r4`), stashes it via `mov ip,r0` to
    free r0 for the `0x7f` mask, then retrieves it into r5
    (`mov r5,ip; ldrb r5,[r5]`) for the final `ands r0,r5` - an
    `ip`/r5 relay this compiler's own allocator never reaches
    unprompted. Clobbering r5/`ip` in the anchor is what makes this
    compiler's prologue include r5 in the callee-saved push list to
    begin with (a hardware register clobbered by inline asm gets
    saved/restored even with no other "real" use in the function).
  - The `0x3fff` distance-bound literal needs `register s32 limit
    asm("r2")`, matching the ROM's own re-use of r2 for both the dx and
    dy comparisons - but only declared *after* `dx`'s own computation
    (which still needs r2 for the incoming `testX` parameter), not at
    the top of the enclosing block; a pin claims its register for its
    whole *lexical* scope, so declaring it any earlier forces this
    compiler to relocate `testX` out of r2 pre-emptively instead.
  - The trailing `self+0xc &= ~8` clear needs the same inline-asm
    anchor and negative-constant idiom (`matching_decomp_register_pinning`)
    already used throughout this subsystem, keeping the loaded byte in
    r6 specifically (a plain `register u8 loaded asm("r6") = self[0xc];`
    gets optimized away - its single use inlines straight into the AND,
    dropping the r6 pin entirely - so it's anchored as one literal
    block instead).

## Parked (NAKED transcription) - 4 functions

- **`sub_800FC70`** (`src/system/game_loop32.c`, new file) - a
  per-frame position-wrap advance keeping `sb`/`r8` live as two extra
  callee-saved accumulators throughout - the same "two extra
  high-register accumulators live throughout" gap already parked (and
  NAKED-transcribed) for `sub_800D040` (`game_loop6.c`,
  [docs/matching/issue-12-physics-collision.md](issue-12-physics-collision.md)).
  Needed a trailing `asm(".align 2, 0")` too - the function body is 342
  bytes (not 4-aligned), and the ROM pads the 2-byte gap before
  `sub_800FDC8` with a zero halfword rather than the assembler's
  default `nop` (`matching_decomp_alignment_fix`).
- **`sub_800FDC8`** (`src/system/game_loop33.c`, new file) - the full
  4-octant Bresenham-line-style line-stepper `sub_8010784`/
  `sub_80107C4` above are fixed single-octant variants of. Every
  branch/field/octant-selection is understood and was written as plain
  C first (all four octant cases individually matched byte-for-byte in
  isolation) - the sole remaining gap is that this compiler's
  cross-jump pass notices the X-major-increasing octant's own
  early-return (`adds r0,r1,#0; b <exit>`) is byte-identical to the
  shared early-return the other three octants legitimately share in
  the ROM too, and folds *all four* into one shared tail instead of
  the ROM's two (one solo, one shared by three) - 4 bytes short. An
  `asm volatile("" : "+r"(count))` barrier at that one return site (the
  established fix for a *value*-driven cross-jump, see
  `docs/matching.md`'s `text_layout.c` entry) has no effect here since
  this merge is purely code-shape-driven, not value-driven - the
  barrier's own zero real instructions vanish before this late-stage
  pass runs. No combination of restructuring tried kept that one
  return physically separate without changing the byte count
  elsewhere, so it's transcribed instead; the full octant-by-octant C
  reconstruction (verified matching in isolation before this gap
  surfaced) is preserved in the function's doc comment for reference.
- **`sub_8010914`/`sub_801095C`** (`src/system/game_loop30.c`) - two
  "get prev"/"get next" neighbor-list-walk-and-filter helpers: each
  walks its own list direction, returning the furthest node reachable
  while every node visited has a `+0x4d & 0x7f` state != 1, falling
  back to `self` if there's no usable neighbor at all. Hits the exact
  same cross-jump-over-merge gap as `sub_800FDC8` above - the loop's
  two `return cur;` sites are byte-identical (`adds r0,r4,#0; b
  <exit>`) and the ROM keeps them as separate physical copies, this
  compiler merges them into one (4 bytes short each). Transcribed for
  the same reason. **Since matched as real C** - see
  [naked-sub_8010914-matched.md](./naked-sub_8010914-matched.md); this
  entry is left as-is since it's a frozen historical record of why the
  function was originally parked.

## Still left raw - 4 functions

- **`sub_800FF0C`** (`asm/code_3_2_17_e560_ff0c.s`) - a large
  (~660-instruction) projectile/hazard-spawn dispatcher with two big
  jump tables and packed bitfield arguments; still out of scope for a
  single pass.
- **`sub_80104E4`** (`asm/code_3_2_17_e560_ff0c.s`) - a large
  (~195-instruction) state dispatcher calling several still-raw
  siblings; not attempted.
- **`sub_8010480`/`sub_8010674`** (`asm/code_3_2_17_e560_ff0c.s`) -
  both read and their semantics are understood (`sub_8010480`: a
  `self+0x4d`-gated reset of `self+0x30`/`self+0x38` via the
  `self+0x20`-pointer-to-manager/`self+0x2d`-tag/0x1c-stride hitbox-
  record convention `sub_800D040` also uses, then a tail call to
  `sub_8007A84`; `sub_8010674`: an AABB-overlap test between `self`'s
  own table-driven half-width/half-height box and a caller-supplied
  box, short-circuiting true when `self+0xc` bit 4 is set or
  `self+0x44` is 0) - but this compiler's own register allocation for
  both spreads more live values across r0-r7 than the ROM's build
  needs (an extra `r8`/`ip` pair appears in `sub_8010674` where the
  ROM stays within r0-r7 entirely, by reusing `self`'s own register for
  a late scratch value once it's no longer needed - the ROM's own
  `self+0x20`/`self+0x2d` field-address computation and the AABB
  compare both keep tighter register pressure than any C phrasing
  tried here reproduced). Left raw rather than force a low-confidence
  register-pin attempt; a future pass matching `sub_800D040`'s own
  hitbox-record register shape first may make these more tractable.

## Cross-references

- `docs/status/game_loop.md` - matched/parked/raw lists updated for
  this pass.
- `tools/report_units.py` - `UNITS` list updated: `0x0800FC70`/
  `0x0800FDC8` split into their own new-object entries (one NAKED-
  parked, one matched initially then reclassified NAKED-parked once
  the cross-jump gap surfaced), `0x0801071C`/`0x0801089C`/
  `0x08010914` all now point at their new `.o` files.
- `docs/matching/issue-13-graphics-fc70.md` - the first pass against
  this same issue, left these five units raw.

## Update: `sub_800FDC8` since matched as real C

`sub_800FDC8`, parked above as a NAKED transcription, is now matched
as real decompiled C - see
[naked-sub_800fdc8-matched.md](./naked-sub_800fdc8-matched.md). The
`goto`-to-a-physically-earlier-label technique that closed
`sub_8010914`/`sub_801095C` above was necessary but not sufficient
here: this function's ROM layout keeps *two* genuinely separate
physical copies of the shared-tail-shaped return (one solo, one shared
by three), where `sub_8010914`'s gap was a single shared copy in the
*wrong position*. Closing the anti-merge (not just placement) needed
the solo copy's return materialized as an opaque `asm volatile` block
- gcc's cross-jump pass still unified plain-C-goto-placed identical
instruction sequences by content regardless of source position, but
never unifies an inline-asm block with a compiler-generated one. This
entry is left as-is above (a frozen historical record of why the
function was originally parked); `tools/report_units.py`'s
`0x0800FDC8` entry now points at `src/system/game_loop33.o` instead of
`None`, and `docs/status/game_loop.md`'s parked-list entry for it was
removed in favor of a matched-list entry.

## Update: `sub_80104E4` matched (NAKED transcription)

`sub_80104E4`, parked above as "not attempted", is now matched. By the
time this pass picked it up, every callee it flagged as a "still-raw
sibling" (`sub_800F8E0`/`sub_800F990`/`sub_800F4F4`/`sub_800F798`,
`sub_800FC70`, `sub_80087C0`/`sub_80087B4`/`sub_800872C`,
`sub_8006DF8`, `sub_8008044`, `sub_803AD7C`) had already been matched
by earlier passes in this same session (issue #12's cluster and this
issue's own `sub_800FC70`/`game_loop32.c`), which is what made this
function tractable at all - it's a pure dispatcher/glue function over
already-understood pieces, not new semantic territory.

**What it does**: a per-frame state-machine tick on the same
"collision box" `self` object this whole file family operates on.
While `self+0x4f` (a per-object throttle counter, matching the
`sub_800F368`-seeded throttle byte game_loop49.c's own header comments
describe) is nonzero, decrements it and, only for the frame it reaches
zero-triggering, dispatches once more on `self+0x4e` (the settle-state
byte): `0x13`-`0x15` re-enters the edge-settle chain (`sub_800F8E0`)
and arms the global one-shot rescan flag `gUnknown_030012B0`; `0xf`
re-triggers `sub_800F990` when `self+0x4d`'s low 7 bits are already 0;
`0xc`, once the throttle has reached 0 this frame, clears `self+0x50`;
`3` re-triggers `sub_800F4F4`. Unconditionally afterwards: while
`self+0x4e == 0xc`, counts `self+0x48` down toward 0; always calls
`sub_800FC70` (the position-wrap advance, `game_loop32.c`). Then, if
`self+0x4d`'s bit 7 is set and `self+0x38` is nonzero, re-derives
`self+0x30`'s index via the exact same `self+0x20`-pointer-to-manager/
`self+0x2d`-tag/0x1c-stride hitbox-record clamp `sub_8010480`
(`game_loop35.c`) uses, clears `self+0x38` and `self+0x4d`'s bit 7, and
clears the "recently touched" object's (`gUnknown_030012D8`) own
`+0x80` byte - then, depending on `self+0x4e`: state 6 settles to
state 7, tags `self+0x2d = 0x20`, runs the
`sub_80087C0`/`sub_80087B4`/`sub_800872C` triplet (the same one
`sub_800F8E0`'s own settle paths use), then folds the low nibble of a
`sub_8006DF8` tile-cache lookup (keyed by the freshly-retagged hitbox
record's own `+0x14`) into `self+0x29`; state 3 just tags `0x20` and
runs the same triplet. If bit 7 was clear instead, `self+0x4d`'s low 7
bits == 1 triggers `sub_800F798`. Finally, unconditionally, calls
`sub_8008044` and hands `self+0x18`'s table's own `+0x60`/`+0x64`
offset/function-pointer pair off to the `sub_803AD7C` table-trampoline
- the same convention `sub_8007048`/`sub_80070D4` (`graphics.c`)
establish, confirming `self+0x18` is this object's `struct actor.table`
field even though the rest of `self` is far larger than `struct
actor`'s own `0x1c` bytes (consistent with every sibling in this file
family: offsets kept raw, no named struct).

**Why NAKED, not plain C**: an initial plain-C attempt, following
`sub_8010480`'s own successful register-pin-per-nested-scope technique
for its near-identical hitbox-record clamp block, matched the function's
first ~10 instructions exactly (`self+0x4f`'s address computed once
into r1 and reused for both its nonzero check and decrement store;
`self+0x4e`'s address computed into r0, loaded, then copied to r6 for
reuse by the immediately-following `0xf` re-check; a `>0x12 && <=0x15`
range test written with `goto` rather than `&&` to avoid this
compiler's subtract-and-unsigned-compare fold, plus an `s32` local
instead of comparing the raw `u8` field in place to get the ROM's own
signed `ble`/`bgt` mnemonics instead of unsigned `bls`/`bhi`). It
diverged as soon as a *second* field address needed the same
"computed once, copied to a callee-saved register, reused later"
shape: register-pinning `self+0x4e`'s temporary (`r0`) and its copy
(`r6`) explicitly (mirroring the exact r0/r1/r6 pins that fixed the
first divergence) produced an unexplained extra dead copy into `r7`
that the ROM never makes, seemingly because this compiler's liveness
tracking for register-`asm`-pinned locals doesn't fully believe the
temporary dies at the end of its own nested scope. Beyond that point,
the function keeps threading field *addresses* (not just values)
through r0/r1/r6/r2/r5/r8/ip across many intervening `bl` calls, with
an inconsistent reuse pattern the ROM itself only sometimes applies
(e.g. `self+0x4e`'s address is recomputed fresh for the `0xc`/`3`
checks a few instructions later, rather than reusing the r6 copy the
immediately-preceding `0xf` check just made) - a finer-grained, more
pervasive version of the same "which anonymous scratch register" gap
that already forced `sub_8007B00`/`sub_8007B98` (`actor_part.c`) and
`sub_8010B6C` (`game_loop28.c`) fully NAKED. Given the depth of
precise, non-uniform register control needed across the *entire*
~195-instruction function (not just one isolated block), it was
transcribed byte-exact instead - every instruction checked directly
against the ROM's raw bytes, confirmed via a full clean
`make NON_MATCHING=1 report` (no warnings) and `make compare`
("La suma coincide") from scratch. Lives in new file
`src/system/game_loop50.c` (the next free `game_loopNN` slot in this
file family; it sits between the still-differently-addressed
`game_loop35.o` and `game_loop23.o` in ROM order, so - like
`sub_8010480` before it - it needs its own file rather than joining
either neighbor).
