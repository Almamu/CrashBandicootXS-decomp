# Issues #9/#10: `sub_800AAEC`/`sub_800CD00` (graphics)

Dedicated deep-investigation session against two functions flagged in
`tools/report_units.py` as parked (`base_object=None`, still raw):
`sub_800AAEC` (0x0800AAEC, [issue-9-10-0x0800a884-graphics.md](./issue-9-10-0x0800a884-graphics.md)'s
own summary already called this "mechanically clear ... blocked on
[its callee]") and `sub_800CD00` (0x0800CD00, inside the still-raw
`sub_800B8DC` onward span), which `sub_800AAEC` is the only caller of.
`docs/rom_map.md`'s "A companion function" passage (its own reads
around line 2782) had already worked out `sub_800CD00`'s field-offset
shape in a later pass, but that finding was never carried into a
`docs/matching.md`/`docs/matching/*.md` entry until now - this session
reconciled that prose against the actual ROM bytes (confirmed correct,
see below) and closed both functions.

## Semantics

### `sub_800AAEC(void *self, s32 x)` - `src/graphics/actor_part108.c`

The input-action-check function the 42-slot `gStaticData_0816BF20`
action-dispatch table's own entries (`sub_8013994` etc.) call for
their action codes `0xB`/`0x10` (`docs/rom_map.md` line 1713).

1. **Gate**: builds an integer `{x, y}` probe position from `self`'s
   own Q8 `x`/`y` plus the target action `x`'s own `self+0x20`-table
   (a pointer-to-table, indexed by `x` at 28-byte stride - the same
   "keyframe/hitbox record" convention `game_loop6.c`'s `sub_800D040`
   documents) record's `+6` (s16) vertical offset, added in Q8 space
   before truncating to match the ROM's exact rounding. Passes
   `self+0x28` bit 4 (the mirror-flag bit `actor_part16.c`/
   `actor_part17.c` already read) as a `1`/`2` selector, the record's
   own `+9` byte as a third scalar, and a pointer to `self`'s original
   (untruncated) Q8 `y` for the callee to restore/report through, to
   `sub_8026628(player, arg1, posInt, arg3, outY)` (prototype already
   established from its two other NAKED call sites,
   `actor_part12b.c`/`game_loop6.c`... actually `actor_part12b.c`'s
   `sub_8009BE0`). If the low byte of the result is nonzero, returns
   `0` immediately.
2. **Loop**: otherwise walks `gUnknown_0300130C` (a `struct actor_list
   { s32 count; s32 unused_4; void **items; }`, the exact layout
   `src/system/game_loop24.c`'s `sub_8010804` already established) -
   for each `entry = items[i]`, tests `entry`'s own `+0x18`-table
   `+0x48` trampoline via `sub_803AD7C(entry + *(s16*)(table+0x48),
   *(void**)(table+0x48+4))` (matched elsewhere). On state `3`, calls
   `sub_800CD00(entry, x)`; if that returns `1`, returns `0`
   immediately. If the loop runs to completion, returns `1`.

### `sub_800CD00(void *self, s32 x)` - `src/graphics/actor_part109.c`

`sub_800AAEC`'s only callee, called once per list entry whose own
`+0x18`-table trampoline reports state `3`.

Early-outs (returns `0`) when `self+0x4e` (a state/type byte) is `5`
or `0xa`. Otherwise builds **three** AABBs via the shared
`sub_803AFE4`(set-pos)/`sub_803AFDC`(set-size) primitive (`struct
aabb` from `actor_part.c`/`game_loop6.c`), all from the same
`self+0x20`-table-at-28-byte-stride convention `sub_800AAEC` above
also uses (confirming `docs/rom_map.md`'s own cross-reference: "the
exact field `gStaticData_0816BC98`, the physics subsystem's 22-row
table, indexes by ... matching `gStaticData_0816BC98`'s stride
exactly, but clearly a different table instance" - reinforcing the
project's established "shared convention, not shared struct" reading),
with the record's own `{s16 offX, s16 offY, u8 w, u8 h}` quad at
`+4`/`+6`/`+8`/`+9` (yet another layout variant of the convention
alongside `actor_part.c`'s `+0xc`/`+0xe`/`+0x10`/`+0x11` and
`game_loop6.c`'s own `+4`/`+6`/`+8`/`+9`, which this function's first
two AABBs match exactly). Each AABB is mirrored horizontally/
vertically around its own object's integer position when that
object's own `+0x28` bits 4/5 are set (same mirror-flag convention):

- **AABB1**: from `self`'s own `+0x20`-table, indexed by `self`'s own
  `+0x2d` tag - `self`'s current hitbox.
- **AABB2**: from the player's (`gUnknown_030012D8`) own `+0x20`-
  table, indexed by the PLAYER's own `+0x2d` tag - the player's
  current hitbox.
- **AABB3** (reuses AABB2's stack slot): from the player's `+0x20`-
  table again, but indexed by `x` (this function's own second
  argument, the caller's target action index) instead of the player's
  `+0x2d` - the player's hitbox *for the target action*.

If AABB1 overlaps AABB2 (`sub_8001640`, the inclusive/touching-counts
variant, `src/graphics/aabb_util.c`), bails out and returns `0`
immediately - `self`'s hitbox already overlaps the player's *current*
hitbox, so this isn't a fresh trigger. Otherwise returns `1` only if
AABB1 overlaps AABB3 - `self`'s hitbox overlaps the player's hitbox
for the action `x` being tested.

Read together, this is: "would performing action `x` right now hit
`self`, given the player isn't already touching it in its current
pose" - a melee/interaction-trigger gate, consistent with
`sub_800AAEC`'s own role gating the action-dispatch table's action
codes `0xB`/`0x10`. The prior `docs/rom_map.md` prose was confirmed
correct against the raw disassembly in full; nothing needed
correcting.

## Matching

### `sub_800AAEC` - PARKED (`NON_MATCHING`), not yet byte-exact

The `#if NON_MATCHING` branch gets every instruction byte-exact except
one 5-instruction pair: the `gUnknown_0300130C` list-walk's loop-
condition-check/loop-entry transition. The ROM re-loads
`&gUnknown_0300130C` from the literal pool fresh on *every* iteration
(a conservative reload, since the intervening `sub_803AD7C`/
`sub_800CD00` calls alias-escape the global) and lands it in `r0`,
then the loop body's own first instruction (`ldr r0, [r0]`) turns that
same register from "address" into "value" in place, reusing it rather
than a second register. No C-level phrasing reproduced that exact
per-iteration re-materialize-into-r0-then-alias-in-place shape:

- A plain `for (i = 0; i < gUnknown_0300130C->count; i++)` matches
  register roles for every OTHER value in the function (getting the
  whole function down to the ROM's exact 3 callee-saved registers,
  `r4`/`r5`/`r6`) but lets gcc correctly recognize `&gUnknown_0300130C`
  as loop-invariant and hoist it - landing the address in `r1` and the
  freshly-dereferenced value/`->count` in `r0` (the ROM's opposite
  choice).
- The `sub_8010804`-style "cache `&var` in a local declared inside an
  `if` guard, then `do`/`while`" idiom (`game_loop24.c`'s own proven
  pattern for the identical `gUnknown_0300130C` list shape) re-adds a
  4th callee-saved register (`r6` for the cached address, on top of
  `r4`/`r5`/`r7`) here - it doesn't reproduce the ROM's shape either,
  just trades one mismatch for a worse one.
- An explicit `goto`-based loop with the address and the dereferenced
  value each pinned to fixed registers (`register struct actor_list
  **listAddr asm("r0")`/`register struct actor_list *list asm("r1")`)
  gets the *check* block byte-exact (confirmed: `ldr r1,[r0]; ldr
  r1,[r1]; cmp r5,r1; blt`, matching the ROM register-for-register) but
  the loop *body* still diverges, because the C-level `listAddr` is
  never reassigned inside the loop (so gcc keeps it live/invariant
  across iterations) where the ROM's compiler re-derives it from
  scratch each time - the two compilers are solving genuinely
  different problems at that point (hoist vs. no-hoist), not just
  picking different registers for the same one.

Every other block matches byte-for-byte: the `self+0x20`/`x*28`-
indexed table lookup (needed the same "materialize the `+4` in two
separate instructions" opaque-asm anchor - `add %1,%1,#4 / add
%0,%1,#0` - that `sub_8007B00`/`sub_800D040` already needed for their
own record-pointer builds, since a bare `rec + 4` otherwise folds into
a single `add r4,r1,#4`), the `recByte9`/`flagArg` register pins
(`r3`/`r2` respectively, matching the ROM's own choices, with
`flagArg`'s default-then-override assignment statement positioned
*after* the `self+0x28` bit-test load rather than at declaration, to
match the ROM's own instruction order), the `self.x`/`self.y` Q8 loads
(needed a scoped `register s32 selfY asm("r2")` plus batching both
loads before either store, to reproduce the ROM's load-load-store-
store interleaving instead of gcc's natural load-store-load-store),
the `volatile` re-read for the original untruncated `y` (the ROM
issues a genuinely redundant second `ldr` the natural CSE would
otherwise eliminate), the Q8-to-int conversion's exact "compute the Y
sum first, finish X, then finish Y" reordering, and the `(u8)` result-
truncation casts on both the `sub_8026628` and `sub_800CD00` call
results (the ROM explicitly narrows to a byte via `lsl r0,r0,#0x18` /
`lsl+lsr` before each comparison; a bare `!= 0`/`== 1` on the full
`s32` drops those shifts entirely even though it's behaviorally
equivalent).

The default (matching) build uses the `#else` branch: a byte-exact
NAKED transcription of the ROM's own confirmed-correct instructions
(verified via isolated `cpp`/`agbcc`/`as` + `objcopy`/`cmp` against
`baserom.gba`'s own bytes at `0x0800AAEC`-`0x0800AB9C` - the only
byte differences were at the three `bl`/two `.word` relocation sites,
which resolve correctly once linked). Real bytes formerly in
`asm/code_3_2_16.s` (now removed entirely - `actor_part108.o` replaces
it in link order between the trimmed `asm/code_3_2_16_a884.o` and
`actor_part81.o`).

### `sub_800CD00` - NAKED transcription, not real C

Not attempted as a C reconstruction at all: this is the same AABB-
build primitive `game_loop6.c`'s `sub_800D040` already documents at
length (inlined twice there, three times here), and that function's
own header comment already records the *simpler* two-AABB version as
resistant to gcc 2.9 C reconstruction - "the ROM keeps exactly two
extra callee-saved registers live across both AABB builds (`r8` and
`sb`) ... and reuses `r7`/`r8` for the X/Y 'shift' values across
*both* the self-block and the player-block - no C reconstruction
tried reproduced that with gcc 2.9". `sub_800CD00` is a strict
superset of that same shape (three AABB builds instead of two,
`r7`/`r8`/`sb` kept live across all three plus an extra `sl`-held
argument), so re-attempting a C reconstruction already known to fail
on the simpler case wasn't a good use of this session's remaining
time - it was transcribed directly as byte-exact NAKED asm instead,
verified instruction-for-instruction against the ROM disassembly and
confirmed byte-exact via the same isolated `cpp`/`agbcc`/`as` +
`objcopy`/`cmp` pipeline against `baserom.gba`'s bytes at
`0x0800CD00`-`0x0800CEAC` (again, the only differences were at
relocation sites - eight `bl` calls and one `.word` literal).

Real bytes formerly in `asm/code_3_2_17.s`'s middle (that fragment is
now trimmed to end right before `sub_800CD00`); the remainder from
`sub_800CEAC` onward (still raw, unexamined this session) moved to the
new `asm/code_3_2_17_ceac.s`. `actor_part109.o` sits between the two
in link order.

## Full-ROM verification

`rm -rf build && make NON_MATCHING=1 report` - clean, no warnings from
either new file. `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` - `crashbandicootxs.gba: La suma
coincide` (checksum matches).

## Techniques used

- Opaque two-instruction `asm volatile` materialization to stop a
  `ptr + N` from folding into the next store/copy's addressing mode
  (established technique, `actor_part48.c`/`sub_8007B00`).
- Scoped `register` pins (`r2`/`r3`/`r4`) for values that need to
  outlive several intervening statements in a *specific* hardware
  register, plus deliberately reordering the C statements that produce
  and consume them to match the ROM's own instruction order (moving an
  assignment later, or grouping two loads before their stores) rather
  than relying on gcc's natural scheduling.
- A `volatile`-qualified struct field to force a redundant reload the
  ROM's own compiler issues but modern CSE would otherwise fold away.
- `(u8)` truncation casts on `s32`-returning helper calls to
  reproduce the ROM's own explicit byte-narrowing shift pairs before a
  boolean comparison.
- NAKED transcription as the deliberate, documented default for an
  AABB-build shape this project has *already* proven (via
  `sub_800D040`) resists gcc 2.9 C reconstruction even in its simplest
  two-block form - not attempting a C draft here was a judgment call
  informed by that existing precedent, not a shortcut.
