# `sub_8020E84`/`sub_8020F7C`/`sub_802107C`/`sub_802117C` (issue #31) - a near-miss pass

Follow-up to the third pass recorded in
[issue-31-graphics-loading.md](./issue-31-graphics-loading.md) ("the
'trigger effect type N' twin family matched via NAKED transcription"),
which converted all four `src/graphics/trigger_effect.c` functions to
`NAKED` after concluding plain-C restructuring couldn't reproduce two
register-allocation spots. This pass revisits `sub_8020E84` with
techniques established by *later* passes in this same document (the
`asm volatile`-anchored idioms, the `goto`-based branch/fallthrough
restructuring, the r7-pin categorical bug write-up) that postdate the
third pass's own attempt - none of that had been tried on this specific
function before.

## Result: 11 bytes short of byte-exact, kept in-tree via `#if NON_MATCHING`

`sub_8020E84` got extremely close - every load/store, branch and call
is confirmed correct, and the reconstruction is 237 of 248 bytes
byte-identical to the ROM - but a handful of pure register-choice/
scheduling details resisted every technique tried this pass. Per this
project's own established convention for a near-miss C reconstruction
(`docs/workflow.md` step 3, the `matching_decomp_non_matching_toggle`
memory note), it's kept in-tree under `#if NON_MATCHING` rather than
discarded - the default build (`NON_MATCHING` unset/0) still uses the
original byte-correct `NAKED` transcription (`#else` branch, unchanged
from the third pass), verified via a full clean `make compare` ("La
suma coincide"). `make NON_MATCHING=1 report` also builds clean with no
warnings for this file. The other three siblings
(`sub_8020F7C`/`sub_802107C`/`sub_802117C`) are untouched - still plain
`NAKED`, no `#if` toggle - since this pass only worked through
`sub_8020E84` itself; the same techniques should transfer directly
(same shape, different bit-test mask/sound ids/tag value) to whoever
picks up the other three next.

## What got fixed this pass (new techniques, not tried in the third pass)

- **The `gUnknown_030012C0` address survives across the `sub_8023278`
  call for free once pinned to `r9` via a *plain C* `register void
  *self asm("r0") = (void *)&gUnknown_030012C0;` initializer** - letting
  the compiler's own address-of codegen manage the literal-pool
  placement (landing it in the function's single combined pool,
  matching every other reference in this file) rather than a hand-
  written `ldr r0, =symbol` pseudo-op inside `asm volatile`. This is a
  **new, previously-undocumented gotcha** for this project's established
  "spell out the literal load in an asm island" idiom: a hand-written
  `ldr r0, =gUnknown_030012C0` inside `asm volatile` pools at the *far
  end of the whole translation unit* (GAS's default deferred-pool
  behavior for the `=expr` pseudo-op, since nothing in this file forces
  an early `.ltorg`), not at the natural per-function position the ROM
  and this compiler's own hex-asm output both use - shifting every
  literal-pool-dependent byte in the function and even corrupting *other*
  functions' apparent addresses when measured via `nm` before the fix
  was found. Once the address-of moved to plain C, the pool landed
  exactly where the ROM's own does.
- **The mask-then-load evaluation order for the bit test** (`bit =
  ((u8*)self)[2] & 1`) needs a small `asm volatile` island - this
  compiler otherwise schedules the `1` mask constant *after* the byte
  load, while the ROM materializes it first. Confirmed by testing the
  boolean condition through a *separate* `s32`-typed register variable
  (`bitLo`) bound to the same `r2` the mask computation lands in -
  testing the `u8`-typed pinned result directly triggers an unwanted
  zero/sign-extension widening (`lsl r2,r2,#24` before the `cmp`) that
  the ROM's own `if` test doesn't have.
- **The `sub_8023278(...) || ...+0x8c` branch needs explicit `goto`s,
  not a plain `||` expression.** A plain `||` compiles both operands as
  independent "if true, branch to the shared target" tests; the ROM's
  second operand (`gUnknown_030012C0+0x8c` test) is inverted and
  *falls through* into the first operand's target instead of branching
  to it (saving one instruction). Restructuring as `if (A) goto
  id_c; if (!B) goto id_b;` (fallthrough to `id_c`) reproduces the
  ROM's exact branch/fallthrough shape.
- **The `id`/`(u16)arg0`-truncation pair needs to be duplicated per arm**
  (written once in each of `id_c:`/`id_b:`, not computed once after an
  `if`/`else` merge) - this compiler's own cross-branch tail merging
  then reunites the *shared* call-marshalling tail (the stack-arg store
  plus `arg1`/`arg2`/`a3` register moves) on its own once the source is
  shaped this way, matching the ROM's single `bl sub_801A878` call site
  exactly (not two separate calls).
- **The `+0x8c` re-check after the `sub_8023278` call re-derefs
  `gUnknown_030012C0` through the `r9`-pinned address**, not by reusing
  `self` (whose own register, `r0`, was clobbered by the call) - a
  fresh `register void **tmp asm("r3") = pAddr; register u8 val
  asm("r0") = *((u8 *)*tmp + 0x8c);` pair reproduces the ROM's exact
  `mov r3, r9` / `ldr r0, [r3]` sequence.
- **The closing `part->flags &= -5;` needs the operands swapped**
  (`negFive &= part->flags; part->flags = negFive;`, not `part->flags
  &= negFive;`) to land the AND's result in the same register (`r0`)
  the ROM's own `ands r0, r1` does - the mask-materialization instruction
  pair itself (`mov r0, #5` / `neg r0, r0`) is the same established
  negative-constant idiom `sub_801FDEC` uses elsewhere in this cluster.

## What's still unmatched (11 bytes)

- **Argument-marshalling order for the `sub_801A878`/`sub_8008434`
  calls.** The ROM fills the stack slot (`id`) and `r1`/`r2`
  (`arg1`/`arg2`) *before* `r3` (the hard-pinned `a3`), but this
  compiler always moves `a3`'s already-pinned register into its call
  slot first, ahead of the (unpinned) `arg1`/`arg2` - confirmed
  present regardless of source-level argument order, an intervening
  empty `asm volatile` barrier touching `arg1`/`arg2` first, or wrapping
  `arg1`/`arg2` in their own freshly-declared locals right before the
  call. A hand-spelled `asm volatile` block covering the whole
  marshal-through-call sequence *does* reproduce the ROM's order
  correctly - but only by also hard-pinning `arg1`/`arg2` (so the raw
  asm text's register names resolve to something valid), which cascades
  into the next point.
- **Pinning `arg2` to `r7` explicitly reproduces the confirmed
  categorical r7-pin toolchain bug** this project has already documented
  for `sub_8007114`/`sub_802190C`/`sub_8021280`/`sub_8021480`
  (`docs/status/graphics_loading.md`): the compiler silently drops
  `arg2`'s own truncation code *and* `r7`'s save/restore from the
  prologue's push/pop set entirely once it's explicitly pinned, leaving
  the function reading an uninitialized `r7` - not just a mismatch, an
  outright miscompile. (`arg1`, pinned to `r6`, has no such problem -
  the bug is specific to `r7`.) This confirms the bug is genuinely
  categorical and not limited to the earlier-documented functions.
  Reverting to unpinned `arg1`/`arg2` (natural allocation correctly
  reaches `r6`/`r7` on its own, just not in the ROM's *call-marshalling*
  order) is what this pass shipped instead.
- **The `+0x2d` tag store's source register for the final `strb`**
  (`r1` here vs. the ROM's `r2`) - a single-instruction, same-length
  cosmetic register choice. Forcing a separate `register u8
  asm("r2")` temporary for just this store back-fired: the compiler
  then sees the `r9`-pinned `tag` variable has no other reader and
  re-materializes the `7` constant directly into `r2` instead, which
  drops the `mov r9, r0` / `movs r0, #7` pair that otherwise matches the
  ROM one instruction earlier. No technique tried this pass reproduced
  both the `r9` hold-through and the `r2` reload target at once.

## Dead ends specific to this pass (beyond the two above)

- **Reordering the `part->0x20`/`part->0x2d` writes** to influence which
  register is "freshly freed" for the tag store: not attempted in full
  (would have broken the *already-matching* `+0x20` instruction
  sequence, which depends on the current order).
- **An explicit `register s32 idOrRet asm("r1")` reusing one pinned
  register for both the `id` (call input) and the return value (call
  output)** - matching the ROM's own register reuse exactly in
  principle - destabilized the `(u16)arg0` truncation's per-arm
  duplication (regressed back to a single shared computation, undoing
  the `goto` fix above) for reasons not fully root-caused; reverted in
  favor of two separate plain locals (`id`, `ret`), which coexists
  correctly with the `goto` restructuring.
- **A full hand-spelled `asm volatile` marshal-through-call block**
  (mentioned above) was the only technique that fixed the r3-ordering
  issue, but only by also forcing `arg1`/`arg2` into hard registers -
  which, for `arg2`/`r7` specifically, hits the categorical bug above.
  Tried once with `arg1`/`arg2` left unpinned and referenced via `%N`
  asm-operand substitution instead of hardcoded register names (to
  sidestep needing a hard pin) - this still didn't help, because without
  a hard pin nothing constrains the *scheduling* of when `arg1`/`arg2`
  get materialized, and the underlying "pinned operand hoisted first"
  behavior persisted regardless of whether the raw text spelled out
  `r6`/`r7` literally or used `%N` substitution.

## Verification

Full clean `make compare` (`rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare`) confirms
`La suma coincide` with the `#if NON_MATCHING` toggle in place (default
build unaffected, still byte-exact via the `NAKED` `#else` branch).
`rm -rf build && make NON_MATCHING=1 report` also builds clean with no
new warnings. No entries changed in `tools/report_units.py` - all four
functions remain tracked exactly as the third pass left them (`NAKED`,
parked, `base_object = None`), since none of them actually closed this
pass.
