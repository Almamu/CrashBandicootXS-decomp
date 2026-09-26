# Issue #59/#60 Phase 2, part 1: 0x08031A6C-0x08032688 (actor)

First 30 of the 60 still-raw functions left by
[issue-59-0x08031784-actor.md](issue-59-0x08031784-actor.md)'s Phase 1
pass (`sub_8031A6C`-`sub_8032688`, out of the full `0x08031A6C`-
`0x08033804` "Phase 2" range covering the rest of issue #59 plus all of
issues #60/#61). A sibling pass covers the second half
(`sub_80326E4`-`nullsub_35`) in parallel; see that half's own write-up
for its findings. Same shared "self" object family documented for the
boss-weapon/singleton cluster throughout issues #58/#59/#62: state at
`self+0x28`, table-index/"kind" at `self+0xc`, an anim-frame halfword/
byte pair at `self+0x10`/`self+0x12`, an accumulator at `self+8`, a
"part table" pointer at `self+0`, and an event/trampoline table pointer
at `self+0x50`.

New file: `src/graphics/actor_part129.c` (checked both the highest
`actor_part<N>.c` on disk, sorted numerically, and the highest
`actor_part<N>.o` referenced in `ldscript.txt` before picking `129` -
both agreed the highest in-use number was `128`).

The raw source file `asm/code_3_2_20_28568_c99c_31784_31a6c.s` has been
split at this pass's boundary: this pass's matched/parked functions are
cut out entirely, and the remaining 30 (`sub_80326E4`-`nullsub_35`, the
sibling pass's scope) now live in a new fragment,
`asm/code_3_2_20_28568_c99c_31784_326e4.s`, named by the lower 5 hex
digits of its own first function's address, following this project's
established "cut at the boundary" convention.

## Matched (25 of 30 functions, real C)

- **`sub_8031B0C`/`sub_8031C0C`/`sub_8031D04`/`sub_8031D7C`/
  `sub_8031E80`** - the "type-byte event dispatch" family
  `docs/rom_map.md` had already characterized: a proximity check
  (`sub_802A6EC`) or a countdown timer at `self+0x54` gates the
  transition, `PlaySfx(3, 0x100)` always plays first, then a
  `self+0x30`-relative type byte selects a downstream call
  (`sub_802F540` for `sub_8031B0C`/`sub_8031C0C`'s `0x14`-`0x17` range,
  `sub_8022EA8` for `sub_8031D7C`/`sub_8031E80`'s `0x18`-`0x1a`/`0x1d`
  range). Written with explicit `goto`-chained `if` blocks (not a plain
  `switch`) to match this family's already-matched sibling
  `sub_802C540` (`actor_part19g.c`) - its last case does something
  structurally different from the uniform ones, which a plain `switch`
  doesn't reproduce byte-for-byte here. The state/health-transition
  block at the top of every member of this family needed the ROM's
  exact "compute every constant used by the next few stores up front,
  in one register each, before any of the stores happen" idiom
  reproduced with a `register s32 x asm("rN") = literal;` per constant,
  all declared together in the order the ROM loads them (`state=2`,
  `one=1`, and for the countdown variants a third `zero2=0` reused
  across two separate later stores) - see "A note on isolated-compile
  confidence" below for how this was actually found.
- **`sub_8031F78`/`sub_8032054`/`sub_80320C4`** - `InitActorPart`-based
  constructors forwarding straight through then calling
  `sub_802E4B8` (kind `0x28`/`0x2a`/`0x29` respectively) with a
  `-15798`-biased position argument - three more members of the
  "spawn effect type N" family (`sub_802E504`-family siblings,
  `actor_part128.c`). All three needed the literal health constant `2`
  pinned to `r8` and kept alive across the `InitActorPart` call,
  matching the established `sub_8033BB8` gap (issue #62) where this
  compiler's own allocator always prefers low registers unless forced.
  `sub_80320C4` additionally stashes a 6th argument into `self+0x70`
  after the constructor proper.
- **`sub_8032138`** - trivial `self+0x58` clearing setter.
- **`sub_8032140`** - full reset idiom (state=1, `self+0x44`/`0xc`/`8`/
  `0x6c` cleared, anim frame re-synced from `self`'s own part table at
  `+0` rather than the usual `+0xc`), plus a lap-counter tie and
  `self+0x58` clear. Needed the shared `zero` constant (`self+0x6c`/
  `0x44`/`0xc`/`8`/`0x58`, five separate stores) pinned to a single
  register (`r5`) and reused across all five.
- **`sub_8032170`** - countdown-gated state-2 transition with a
  `self+0x58` trampoline flush (sound cue and lap-counter tie only fire
  when there's a pending object to flush).
- **`sub_80321D0`** - doubly-linked-list unlink (`self+0x48`=prev,
  `self+0x4c`=next, cross-links `next->prev`/`prev->next` around
  `self`), resets `self+0x50`'s event table, then conditionally
  `mem_free`s `self` gated on the caller's flag bit 0 - a
  destructor/detach helper for this object family. The first
  cross-link needed `next`/`prev` explicitly pinned to `r2`/`r0`
  (matching the ROM's exact choice) since a plain top-to-bottom
  translation put them in the opposite registers - same total
  instruction count, but two swapped operand-register encodings.
- **`nullsub_33`** - no-op stub.
- **`sub_8032274`** - trivial `self+0x20`/`self+0x6c` accumulator,
  clamped to `0x4c0`. Needed `self+0x20`'s own prior value read into a
  local *before* `self+0x6c`'s delta (matching the ROM's load order),
  and the delta variable incremented in place (`delta += 0x12;`) rather
  than assigned to a second `next` variable, so the compiler reuses the
  same register the ROM does instead of allocating a second one.
- **`sub_8032290`** - a *second*, independent consumer of the shared
  orbital-motion trig table `gStaticData_0816A820` (alongside the
  already-flagged `sub_8032480`): computes an `self+0x1c`/`self+0x20`
  position pair from two phase-shifted table lookups, then forwards
  the result into another object's (`self+0x58`) anim-frame-advance
  step (`sub_80318D0`). Needed the `self+0x1c` store moved to sit
  immediately after computing it (matching the ROM's own instruction
  order) rather than batched together with the `self+0x20` store at
  the end.
- **`sub_8032350`/`sub_8032478`/`sub_8032680`** - trivial `self+0x5c`/
  `0x58`/`0x64` byte getters.
- **`sub_8032358`** - state-1 trampoline-flush, or (otherwise) a
  proximity-triggered transition firing an event-table call on the
  *player* object before its own state-1 transition; clamps
  `self+0x20` forward by `0x140` once it falls behind `self+0x5c`, then
  tail-calls `sub_802A7B8`. The state-1/`self+0x12`-set branch needed
  an explicit `if (self != 0) { ...trampoline... } return;` (matching
  the established idiom already used for `sub_802C4C8`,
  `actor_part19g.c`); separately, the state-1/`self+0x12`-clear branch
  turned out to jump *directly* to the shared tail call
  (`sub_802A7B8`), **skipping** the `self+0x20` clamp check entirely -
  a real control-flow detail a first pass got backwards (see below).
- **`sub_80323F4`/`sub_80325A4`** - more countdown-gated state
  transitions (`self+0x58`/`self+0x64`+`self+0x65` byte flags,
  anim frame taken from `self`'s own part table at `+0xc`/`+0x18`
  respectively). Both needed their target-field *address* computed
  before the constants used to fill it (`self+0x58`'s address before
  `zero`/`one`; `self+0x64`'s address before `zero`/`one` too), and
  `sub_80325A4`'s second byte store (`self+0x65}) needed the pointer
  explicitly incremented in place (`register u8 *statePtr asm("r1")`
  plus an inline-asm `add %0, %0, #1` barrier) rather than a second
  array-indexed store, which this compiler otherwise folds back into a
  single offset-addressed instruction.
- **`sub_8032480`** - the already-flagged orbital-motion consumer of
  `gStaticData_0816A820`. The real gap here was a control-flow one, not
  a register one: the ROM re-checks `self+0xc`'s state *after* the
  initial proximity-triggered `sub_803256C` call fires (since that call
  can itself transition the state away from 0) and, if so, joins the
  state-nonzero handling below instead of running the orbital-motion
  step on stale state - reproduced with explicit `goto`s mirroring the
  ROM's two separate jumps into the same `_08032510`-equivalent label.
  The state-nonzero branch's `self+0x12 != 0` case also needed the same
  `if (self != 0) { ... } return;` dead-guard-plus-early-skip idiom as
  `sub_8032358` above.
- **`sub_803256C`** - state-transition setter (`self+0x64` byte,
  `self+0x18`, `self+0xc`, anim reset). Needed the target-field
  *address* (`self+0x64`) computed before the constants `0`/`1`
  (pinned to `r6`/`r5` and reused across all three of their stores -
  `self+0x64`, `self+0xc`, `self+8`) to match the ROM's own 4-register
  `push {r4, r5, r6, lr}`.
- **`sub_8032688`** - type-byte-gated (`self+0x30`'s type byte
  `== 0x1f`) proximity check feeding `sub_802F164`, with a one-shot cue
  latched via `self+0x58`; tail-calls `sub_802A7B8` unconditionally.

## Parked - NAKED transcription (5 of 30 functions, byte-correct but not counted as matched)

- **`sub_8031A6C`**, **`sub_80322F4`** - near-duplicate keyframe-table-
  relative dispatch helpers indexing `gStaticData_0817C42C` (stride 8)
  by `self+0x28`, structurally identical in their core to the
  already-parked `sub_8031A08` (issue #59 Phase 1, same table-family
  shape, different table). Resist a byte-exact reproduction of the
  ROM's specific `r7`-as-table-base-pin choice, the same gap that
  parked `sub_8031A08` - transcribed NAKED, byte-verified against the
  original disassembly rather than re-attempting a reconstruction
  already shown not to converge for this exact shape.
  `sub_8031A6C` additionally has trailing state/trampoline logic (fires
  a second `self+0x50`-table call while state 1 with a running
  health-style timer past `0xe100`, or state 2 with `self+0x12` set,
  falling back to `sub_802A7B8` otherwise) that `sub_80322F4` doesn't.
- **`sub_80321FC`** - a parameterized `sub_802E4B8`-based constructor,
  same shape as `sub_8031F78`/`sub_8032054`/`sub_80320C4` above except
  the "kind" is a 6th caller-supplied byte argument instead of a fixed
  literal. Two distinct gaps: this compiler always re-materializes the
  incoming `c` argument register from its own cached copy (`r6`) for
  the `InitActorPart` call, rather than leaving the ROM's original
  parameter register (`r3`) untouched until the call; and this compiler
  defers the "kind" byte truncation to its actual point of use (right
  before the `sub_802E4B8` call) rather than the ROM's eager truncation
  immediately after loading the argument from the stack. An
  `asm volatile` compiler barrier forced the truncation to happen early
  but didn't reproduce the `c`-register gap - transcribed NAKED
  instead, byte-verified.
- **`sub_8032440`** - `InitActorPart`-based constructor forcing a fixed
  `0xFFFF0600` bias for its own 4th argument, stashing the caller's
  real `c` into `self+0x5c`. This compiler's independent-instruction
  scheduler always groups the two pure register loads (the `d`
  argument off the incoming stack, the `0xFFFF0600` constant) together
  right after each other regardless of source order or explicit
  register pins, while the ROM's own build interleaves them with the
  `str`/`adds` steps in between - transcribed NAKED after several
  reordering attempts didn't change the scheduler's grouping.
- **`sub_80325EC`** - clamping `InitActorPart`-based constructor (kind
  `1`, `InitActorPart`'s own 4th argument forced to `0xfa00`): clamps
  the caller's `c` into `self+0x5c` (±`0x3f00`), mirrors a clamped
  `self+0x1c` into `self+0x58` (±`0x8000`), and derives `self+0x60`
  from `sub_803ADB4`. The ROM keeps the `d` argument transiently in
  `r0` (loaded from the stack, immediately pushed to the outgoing call
  frame), then reuses `r0` for `self` right after, *simultaneous* with
  the health literal `1` needing to survive in `r4` across the same
  `InitActorPart` call - every combination of register pins and
  `asm volatile` barriers tried either dropped the `health`-in-`r4`
  persistence or reintroduced an extra register (`r7`) for `d` that the
  ROM's build never needed - transcribed NAKED instead.

## A note on isolated-compile confidence (again)

This pass is a strong, concrete reminder of `docs/workflow.md`'s
warning that an isolated per-file compile's *aggregate* function size
matching the ROM's is not proof of a byte-exact match - several bugs
here produced byte-for-byte-wrong code whose total size still matched
the ROM exactly, and were only caught by the full clean `make compare`
pipeline's checksum failure plus a direct byte-diff against the retail
ROM (`cmp`/a small Python byte-scanner, then `objdump -D -b binary` on
the surrounding bytes from both ROMs) - not by re-reading the isolated
compile's disassembly more carefully:

- `sub_8031B0C` (and every other member of the type-byte-dispatch
  family) computed its `state=2`/`one=1` constants as two *separate*
  literal materializations, one per store, instead of the ROM's
  "compute both up front in their own registers, then do both stores"
  idiom - same total instruction count, so the isolated compile's size
  matched, but the actual bytes at each address didn't.
- `sub_8032358` and `sub_8032480` both, on a first attempt, folded
  their "`self+0x12 != 0`" branch into an unconditional early `return`
  instead of the ROM's `if (self != 0) { trampoline } return;` idiom
  that still lets the *other* branch (`self+0x12 == 0`) fall through to
  the shared tail - `sub_8032358`'s case is especially subtle: the ROM
  actually branches `self+0x12 == 0` **directly to the tail call**,
  skipping the `self+0x20` clamp check that the *other* branch
  (proximity failed) still runs - the opposite of what a first,
  naive reading of the two labels (`_080323CC`/`_080323DC`) suggested.
  The total object size for both functions matched the ROM's exactly on
  the first attempt (a coincidental byte-for-byte cancellation between
  the missing dead-`self`-check/wrong-branch-target and the compiler's
  own register-choice differences elsewhere), and only fell apart under
  a direct disassembly diff.
- Several functions (`sub_8032274`, `sub_80323F4`, `sub_80325A4`,
  `sub_803256C`) needed a target field's *address* computed strictly
  before the constant(s) being stored there, matching the ROM's own
  instruction schedule - a plain top-to-bottom C translation let this
  compiler group the address calculation and the constant
  materializations in either order depending on incidental source
  layout, with no size difference either way.

Every entry in the "Matched" list above was ultimately confirmed via
this same process: `rm -rf build && make NON_MATCHING=1 report` (no
compile warnings), then a full clean `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare`, iterating on each checksum failure by finding the first
differing byte between `baserom.gba` and the freshly-built
`crashbandicootxs.gba`, mapping it back to a ROM address and function,
and comparing raw disassembly on both sides - until `make compare`
finally printed `crashbandicootxs.gba: OK` (this build's locale-neutral
spelling of "La suma coincide").

See [docs/status/actor.md](../status/actor.md) for the running
matched/parked list this entry feeds into, and
[docs/matching/issue-59-0x08031784-actor.md](issue-59-0x08031784-actor.md)
for the Phase 1 pass and full 60-function Phase 2 scope this pass is
the first half of.
