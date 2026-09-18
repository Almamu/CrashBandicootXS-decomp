# Issue #3: `0x080016EC`-`0x08001C80` (second pass)

This chunk's category label is `overlay_ui`, but the actual functions
are the `AudioContext` wrapper layer covered by
[docs/status/audio.md](../status/audio.md), not the pause/options-menu
widgets `docs/status/overlay_ui.md` tracks - the chunk generator's
category guess was wrong here, as `.claude/skills/match-chunk/SKILL.md`
warns can happen. All 23 non-parked functions in this range were
already matched in the first pass (PR #194, GitHub issue #2, written up
in `docs/matching.md`'s "`0x080016EC`-`0x08001C80`: the `AudioContext`
wrapper layer" entry before that file was frozen). This second pass
(issue #3) revisits the two still-parked functions, `PlaySfx` and
`sub_80019F8`, with fresh attempts at closing their documented gaps.

## `PlaySfx` - no progress

`PlaySfx`'s single remaining gap is a prologue register-save
*scheduling* difference: the ROM does `mov sb, r0` (cache `self`)
immediately after the callee-saved-register pushes, before `mov sl, r1`
(cache `id`); this compiler always defers the `self` save until just
before `self->state` is first dereferenced, regardless of how the
`register ... asm("r9") = self;` binding is declared or ordered.

Tried this pass, beyond what's already logged in `docs/matching.md`:

- An inline-asm anchor replacing the register-variable initializer with
  a literal `asm volatile ("mov %0, %1" : "=r" (pself) : "r" (self));`
  as the very first statement, hoping an explicit instruction (rather
  than an implicit gcc-scheduled reload) would force it to the top.
  This did move the copy earlier than before, but gcc still scheduled
  `mov sl, r1` and `str r2, [sp]` (the `volumeParam` stack spill)
  *ahead* of it, and - worse - it left the original `r0` treated as
  dead after the inline-asm's `"r"` input, forcing an extra `mov r0, r9`
  reload before the `self->state` dereference that isn't in the ROM at
  all. Net effect: an extra instruction, not a reordering win. Reverted
  - no net change from the prior pass's parked state.

This is the same class of gap as `sub_80014A4`'s loop-invariant-hoisting
entry and `sub_80305F8`'s stack-argument-fetch-ordering entry elsewhere
in this project (see `docs/matching/issue-58-0x08030334-actor.md`) -
gcc 2.9's own instruction scheduler picks a fixed policy for *when* to
materialize a cached register that doesn't appear to be steerable from
C source at all, only from the RTL passes themselves.

## `sub_80019F8` - one of two gaps closed

Previously parked with two gaps (see the old entry in
`docs/status/audio.md` and the original `docs/matching.md` writeup).

**Gap 1, CLOSED**: the ROM reads the 5th (stack-passed) parameter,
`forceFlag` (`u8`), via `add rX, sp, #0x14` (compute the stack slot's
address) then a genuine `ldrb rX, [rX]` (byte load) - 3 instructions
including the register-8 move that follows. This compiler's default
codegen for a `u8` stack argument instead reads the full word via
`ldr` and narrows it with `lsls #24; lsrs #24` - 4 instructions, and
scheduled *after* the `tableBase`/`handle` table lookup rather than
right at the top like the ROM.

Neither a plain `u8 force = forceFlag;` local nor a
`*(u8 *)&forceFlag` cast changed the instruction shape (the pointer
cast even forced a genuine stack-local copy-and-reload, worse than
before). What worked: bypass gcc's own parameter-read codegen entirely
with a literal inline-asm anchor reproducing the ROM's exact
instruction pair,

```c
register u32 force asm("ip");
register struct SfxTableEntry *tableBase asm("r5");
register u32 handle asm("r2");
register u32 t asm("r0");
s32 volume;

asm volatile ("add %0, sp, #0x14\n\tldrb %0, [%0]" : "=r" (t));
force = t;
tableBase = gStaticData_0816AA6C;
handle = tableBase[id].slotId;
```

placed as the *first* statements in the function (all three
`register ... = ...;` initializers that used to run this table lookup
up front were rewritten as plain declarations followed by assignment
statements in this order, since C90 doesn't allow a declaration after
a statement in the same block) - this both reproduces the ROM's exact
byte-load pair (rather than the word-load-and-mask sequence) *and*
keeps it scheduled first, matching the ROM's instruction order.
Verified with an isolated per-function compile against the real
`0x080019F8` ROM bytes: this portion is now instruction-for-instruction
identical.

**Gap 2, STILL OPEN**: the ROM recomputes
`&gStaticData_0816AA6C[id].baseVolume` fully from `tableBase`+offset+8
(`adds r0, r5, #0; adds r0, #8; adds r0, r1, r0; ldr r0, [r0]` - 4
instructions), even though the identical address was already computed
for the `slotId` read a few instructions earlier and its offset
component (`id * sizeof(struct SfxTableEntry)`) is still live in a
register; this compiler's CSE always reuses that live value instead
(2 instructions: `add r5, r0, #0; ldr r0, [r5, #0x8]`), and once the
address is folded that way, the immediately following
`baseVolume * volumeMul` multiply's register-copy step also lands on
this compiler's generic `mov Rd, Rs` encoding (`0x46xx`, the
hi-register-capable form) instead of the ROM's `adds Rd, Rs, #0`
encoding (`0x1Cxx`) - hand-assembling both forms with this project's
own `arm-none-eabi-as` confirms these are genuinely different bytes,
not merely a `-fhex-asm` text-dump cosmetic difference (see
`docs/matching.md`'s note near its "mov r0, r1 vs. adds r0, r1, #0"
discussion, which was about a *different*, confirmed-cosmetic case -
this one isn't).

Retried this pass, none of which closed it:

- Raw-offset casts instead of struct-field access (already tried in
  the prior pass too).
- An `asm volatile ("" ::: "memory")` clobber-barrier between the
  `slotId` and `baseVolume` reads, hoping to force the address to be
  recomputed rather than reused. No effect - the cached value in the
  live register is a *computed address* (arithmetic on other
  registers), not something loaded from memory, so a memory clobber
  doesn't invalidate it.
- Adding `"r0"`/`"r1"` register clobbers to that same barrier, to force
  the specific registers holding the cached sub-expression to be
  spilled. This didn't force a recompute either - it just pushed
  register pressure elsewhere (an extra `r8` push/pop appeared to hold
  `volumeMul` across the barrier), making the function further from
  the ROM, not closer. Reverted.
- Swapping the multiplication's source operand order
  (`baseVolume * volumeMul` vs `volumeMul * baseVolume`) - this does
  fix the `muls` instruction's own register-field encoding to match
  the ROM (confirmed by hand-assembling both variants), but the
  preceding register-copy instruction still comes out as `mov` instead
  of `adds ..., #0` regardless of which register is the source,
  suggesting this specific "materialize a loaded value into a fixed
  target register mid-function" case just isn't `adds`-eligible in
  this compiler the way an incoming-parameter home is.
- Pinning the loaded value directly into `r1` via
  `register s32 baseVolume asm("r1") = tableBase[id].baseVolume;`
  instead of loading into a scratch register and copying. This
  actually *removes* the copy instruction entirely (gcc loads straight
  into the target register), which is fewer instructions than the ROM,
  not more - moving further from a match, not closer.

Both gaps are the same general class already cataloged elsewhere in
this project (`sub_80014A4`, `sub_8030574`/`sub_8030648` in
`docs/matching/issue-58-0x08030334-actor.md`): register-allocation and
CSE decisions this specific gcc 2.9 build makes internally, that don't
appear to be reachable from C source no matter how the expressions or
register pins are phrased. `sub_80019F8` stays parked under
`NON_MATCHING` in `asm/code_3_1_10_2.s`, with the understood
reconstruction (now closer, one gap down) in `src/audio/audio_context.c`.

## Verification

Full clean rebuild (`rm -rf build && make NON_MATCHING=1 report`, then
`rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare`) passes - `La suma coincide` -
confirming neither function's still-open gap regressed anything
already matched, and that both remain correctly guarded by
`NON_MATCHING` rather than silently wrong.

## Outcome

No functions closed this pass (both `PlaySfx` and `sub_80019F8` stay
parked), so issue #3 stays open - a parked function doesn't count
toward `Closes #3` even when it's demonstrably closer than before.
`sub_80019F8`'s reconstruction is measurably closer to byte-exact (one
of its two gaps closed); `PlaySfx` is unchanged from the prior pass
after a genuine fresh attempt that didn't pan out.
