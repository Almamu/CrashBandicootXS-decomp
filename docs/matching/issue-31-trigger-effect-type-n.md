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

## Round 4: the three remaining siblings, also near-misses

Follow-up pass working through the three siblings the previous pass
left untouched: `sub_8020F7C`, `sub_802107C`, `sub_802117C`. All three
now also have a `#if NON_MATCHING` real-C reconstruction in-tree,
alongside the original byte-correct `NAKED` `#else` transcription
(unchanged, still what the default build uses). None of the three
closed either - all three landed as near-misses, same as
`sub_8020E84`. `tools/report_units.py` is unchanged (all four functions
still tracked exactly as before: `NAKED`, parked). Verification: full
clean `rm -rf build && make NON_MATCHING=1 report` builds with no new
warnings, and full clean `rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare` confirms `La
suma coincide` (default build fully unaffected).

### `sub_8020F7C`/`sub_802107C`: the exact same two blockers, transferring unchanged

These two share one register shape, distinct from `sub_8020E84`'s own:
`self` lives in r1 (not r0), the address-of `gUnknown_030012C0` stays
pinned to r8 (not r9) as `pAddr`, and the bit-test result lives in r9
(not r8). The mask constant is still materialized *before* the byte
load (same gotcha as `sub_8020E84`), but here the AND's destination is
the mask register itself, needing an extra explicit byte truncation
afterward that `sub_8020E84`'s narrower mask (`1`) didn't need. Once
this shape is spelled out via one `asm volatile` island (mirroring
`sub_8020E84`'s own bit-test island, just with the register roles
swapped) and the rest of the function follows `sub_8020E84`'s template
verbatim (address-of via plain C `&global` initializer, `goto`-based
`id_c`/`id_b` branch restructuring, the `+0x8c` re-check through
`pAddr`, the two `mov #imm`/`neg` idioms for the flags/bitfield
clears), **every single instruction matches the ROM except the exact
same two residual gaps `sub_8020E84` documented**, reproducing
identically in both functions (confirmed via isolated-compile
`objdump` diff against each function's own `NAKED` block, which is
already the byte-correct ground truth disassembly - direct raw-byte
`cmp` against the ROM is not useful for an isolated compile, since
`bl`/`ldr =symbol` relocations are unresolved and show as placeholder
bytes regardless of correctness, matching this project's own documented
"isolated compile is a diagnostic tool, never proof of a match"
caveat):

1. **Argument-marshalling order** for *both* call sites now
   (`sub_801A878` **and** `sub_8008434` - `sub_8020E84` only has the one
   `sub_801A878` call site affected, since its spawn branch doesn't pass
   a `a3`-shaped hard-pinned register through the same marshal pattern
   in a way that exposes it... actually it does, structurally identical
   call shape, so this is really the same blocker just visible at two
   call sites instead of one for these two siblings). The pinned `a3`
   (r5) always gets moved into the call's r3 slot first, ahead of the
   stack-arg store and the unpinned `arg1`/`arg2` moves - unresolved,
   exact same root cause `sub_8020E84` already ruled out every attempted
   fix for.
2. **The `+0x2d` tag store's source register** - `r1` in this
   reconstruction vs. the ROM's `r2`, still a single-instruction
   cosmetic register choice, same as `sub_8020E84`'s own residual.

Confirms the doc's own prediction that "the same techniques should
transfer directly" - they do, byte-for-byte identically, including the
*unresolved* parts. No new technique was needed or found for these two;
this is a straight, mechanical reapplication of `sub_8020E84`'s
template with the mask/self/pAddr/bit register roles and the sound
id/tag constants substituted in.

### `sub_802117C`: a genuinely harder case, worse near-miss than the other three

This sibling's bit-test mask (`8`) and tag value (`8`) are the *same*
literal, and the ROM exploits this: it materializes `8` once into a
third callee-saved register (`sl`/r10) up front (via `mov r2,#8` / `mov
sl,r2`, immediately after the address-of/self-deref but before the
mask-vs-byte AND), then reuses that single register unconditionally for
both the bit test *and*, later, the `+0x2d` tag store - unlike the
other three siblings, which each re-derive the tag as a fresh constant
inside the spawn branch. Reproducing this exactly (`register u8 maskTag
asm("r10")`, materialized inside the same `asm volatile` island as the
address-of/self-deref/bit-test, not as a separate plain-C initializer -
same "wrong scheduling position" gotcha `sub_8020E84`'s doc comment
already flags for a naive `register T x asm(reg) = value;` at the top
of the function) works cleanly on its own.

The much harder problem is this sibling's `arg1`/`arg2`/`arg3` ->
r5/r6/r7 mapping, which is *shifted* from the other three siblings' own
r6/r7/r5 mapping - already flagged as unreproduced by the pre-existing
third-pass doc comment in the source ("gcc never reproduced this one's
shifted r5/r6/r7 dx/dy/dz register roles either"), predating even this
issue's round-3/round-4 passes. `arg3`'s target register, r7, is the
project's confirmed-categorically-buggy pin target
(`docs/matching.md`, `sub_8007114`/`sub_802190C`/`sub_8021280`/
`sub_8021480`) - explicitly off-limits per this project's standing
rule, so `arg3` is left unpinned (a plain `u16 a3 = arg3;` local) while
`arg1`/`arg2` are pinned to r5/r6 (neither is r7, both safe). Several
register-pressure configurations were tried empirically (isolated
compiles only, not committed) to find one where natural allocation
alone reaches r7 for `a3` without spilling:

- **`a1`+`a2` both pinned (r5/r6), `maskTag` pinned (r10), `a3`
  unpinned** (the version actually committed): natural allocation puts
  `a3` in r7 *for the spawn branch* (matches ROM, `add r3, r7, #0`
  before `bl sub_8008434`), but for the `if`-branch's `do_call` path
  (which crosses the `sub_8023278` call), the allocator doesn't trust
  r7 survives the call on its own and spills `a3` to a stack slot
  instead (`sub sp, #8` instead of the ROM's `sub sp, #4`, plus a `str
  r7, [sp, #4]` right after the truncation and a `ldr r3, [sp, #4]`
  right before the `sub_801A878` call) - even though r7 is
  callee-saved and any correctly-behaving external function *must*
  preserve it across a call. This makes the reconstruction 4 bytes
  longer than the ROM (264 vs. 260), on top of the same
  argument-marshal-order and `+0x2d`-register blockers the other
  siblings hit, plus the whole top-of-function `lsl`/`lsr` truncation
  block batching into two groups (all three `lsl`s, then all three
  `lsr`s) instead of the ROM's per-variable interleaved pairs, since two
  simultaneous `register T x asm(reg) = value;` initializers get
  scheduled together by this compiler rather than preserving source
  order.
- **All three of `a1`/`a2`/`a3` left unpinned** (with `maskTag`/r10
  still pinned): *all three* spill to the stack (`sub sp, #16`, three
  `str`s) - worse, not better; pinning at least some of them is
  necessary to keep register pressure low enough for the allocator to
  keep anything resident.
- **Only `a1` pinned (r5), `a2`/`a3` unpinned, `maskTag`/r10 pinned**:
  `a2` ends up living in *different* registers in the two branches (r7
  in the `if`-branch's `do_call`, r6 in the spawn branch) via an extra
  redundant `adds r7, r6, #0` copy, and `a3` lands in r10 (colliding
  with/replacing `maskTag`'s own role) instead of r7 - worse mismatch
  shape than the committed version, not just a different one.
- **`a1`+`a2` pinned (r5/r6), `maskTag`/r10 dropped entirely** (tag
  re-derived fresh in the spawn branch instead, like the other three
  siblings): `a3` cleanly reaches r7 with zero spill in *both*
  branches - but dropping r10 also drops one whole shadow-register
  save/restore pair from the prologue/epilogue (`push {r5,r6,r7}` /
  `pop {r3,r4,r5}`, 3 registers, mirroring the ROM) down to just 2
  saved registers, a structural instruction-count mismatch that
  desyncs every single byte offset for the rest of the function, not
  just a handful of instructions - strictly worse for byte-matching
  purposes than the single 4-byte spill/reload pair the committed
  version has, even though this shape fixes the argument register
  consistency issue.
- **A fully hand-spelled `asm volatile` marshal-through-call block for
  both `sub_801A878` and `sub_8008434`**, treating r7 purely as
  raw-text-only scratch space (never bound to any C variable via
  `register T x asm("r7")`, specifically to avoid the categorical r7-pin
  bug, only ever named literally inside `asm volatile` text and listed
  in clobber lists) alongside a similarly hand-spelled `asm volatile`
  block for the top-of-function `arg1`/`arg2`/`arg3` truncation (using
  `"r"(arg1)` etc. as unconstrained input operands, not hardcoded source
  registers): **actively broken, not just a worse near-miss** - the
  compiler additionally generated its own separate, redundant
  truncation of `arg1`/`arg2`/`arg3` into whatever registers it
  independently chose for the "r" constraints, on top of the
  hand-spelled block that also truncates them, and dropped the stack
  frame reservation entirely (no `sub sp` before the manual `str r1,
  [sp]` in the hand-spelled call block), i.e. it would corrupt the
  caller's stack red zone if it ran. Confirms hand-spelling a full
  call-marshal block only cooperates correctly with the compiler's own
  codegen when the values involved already have a stable, compiler-known
  home (register-pinned or otherwise); trying to hide a value from the
  compiler entirely by never giving it a C-level name doesn't work when
  other parts of the same function still reference the *parameter* the
  value derives from through ordinary C (here, `arg1`/`arg2`/`arg3`
  are still the plain function parameters, referenced by name elsewhere
  after the hand-spelled truncation block, so the compiler still
  independently manages their "official" storage location too).

The committed version (`a1`/`a2` pinned, `a3` unpinned, `maskTag`
pinned) was kept as the best of these: correct control flow, correct
calls, correct field offsets and struct writes throughout, matching
every single instruction outside of the argument-marshal-order blocker
(shared with all three siblings), the `+0x2d` register blocker (shared
with `sub_8020F7C`/`sub_802107C`), and this sibling's own unique 4-byte
stack-spill/truncation-batching gap - genuinely harder to close than
the other three, consistent with this sibling's register shape having
already been flagged as unreproduced before this pass even started.
