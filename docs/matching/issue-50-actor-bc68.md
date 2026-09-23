# Issue #50's leftover tail: 0x0802BC68-0x0802BED8 (actor)

[Issue #50](https://github.com/Almamu/CrashBandicootXS-decomp/issues/50)
("Match 0x0802A69C-0x0802AC28, actor category") itself was already
fully matched - see
[docs/matching/issue-50-actor-2a69c.md](./issue-50-actor-2a69c.md) and
`docs/status/actor.md`'s "Matched" list, all 25 functions real C. But
`tools/report_units.py` still carried a leftover entry,
`(0x0802AC28, None, "actor")`, commented "sub_802AC28 onward - left raw,
out of GitHub issue #50's chunk scope" - the raw `.s` file issue #50's
chunk was carved out of (`asm/code_3_2_20_8b7c_ac28.s`) runs well past
the chunk's own upper bound, all the way to 0x0802BED8 where the
already-matched `actor_part19.c` (issue #52) begins. This entry covers
this raw file's own literal tail: `sub_802BC68` through `sub_802BE80`,
the last 7 functions in the file (everything from that file's very end
backwards to the start of `sub_802BC68`), the cleanest possible cut
since the raw file's last byte and this chunk's last byte are the same
byte.

## What's still raw

Everything from `sub_802AC28` (the giant kind-dispatch actor-part-
factory constructor with its own 39-case jump table, ~560 lines of
disassembly by itself) through `sub_802BBE4` stays raw - the actor-
part-factory dispatcher itself and the run of animation-table-state/
actor-part-factory functions between it and this chunk
(`sub_802B12C`-`sub_802BBE4`, including `ConstructAnimTableState`/
`ConstructActorPart` and the large `sub_802B364`/`sub_802B5B4` state
machines). None of that is in scope here; `tools/report_units.py`'s
`(0x0802AC28, None, "actor")` entry still covers it, with an updated
comment pointing at this entry for the now-matched tail.

## Matched

All 7 functions in this file share the same `self` object and global
cluster (`gUnknown_0300148x`/`gUnknown_030014Ax`) already established
by `actor_part19.c` (issue #52) and `actor_part44.c` (issue #56) - see
those files' own header comments for the wider family. `docs/rom_map.md`
had already read part of this cluster from disassembly alone (its "boss's
BG2 spin/zoom effect..." section); this pass confirms those reads with
real matched C.

- **`sub_802BC68`** - accumulator-drain/reward-dispenser for
  `gUnknown_03001488` (filled by `sub_802C078`, still raw): while the
  "locked" flag `gUnknown_030014A0` is set, fully drains it via repeated
  `sub_8023430` calls without spawning anything; otherwise, once the
  `gUnknown_03001484` cooldown elapses, dispenses one of four tiers of
  reward (via `sub_802B174`, itself still raw but confirmed by
  docs/rom_map.md as a "spawn effect type N" family member) sized by the
  accumulator's own magnitude, and plays a cue. Docs/rom_map.md already
  read this as a structural twin of `actor_part44.c`'s `sub_802F3BC` -
  confirmed exactly: same `register u8 *self asm("r1")` pin, same
  branch/threshold shape, just a different accumulator/cooldown global
  pair.
- **`sub_802BD18`** - trivial byte getter (`gUnknown_03001480`). Called
  by the NAKED `sub_802A688` trampoline in `actor_part94.c`.
- **`sub_802BD24`** - frame-counter-threshold (`self+0x44 > 0x13`)
  state-transition: latches `gUnknown_030014A3`, clears the hazard lock
  `gUnknown_030014A0`, and resets `self` to state 1/table-index 0 via
  the same state/table-index/anim-frame reset idiom already documented
  for the boss cluster's `sub_8030530`/`sub_8030C98` and this family's
  own `sub_802C14C` (`actor_part19.c`), then fires `sub_8029BAC(0x24)`.
- **`sub_802BD64`**/**`sub_802BDD0`** - a per-axis hazard-threshold pair:
  drains a shared "camera catch-up" budget (`gUnknown_030014A4`) into
  `self+0x20`, advances `self+0x24` by a fixed step, and derives a
  camera-relative depth (`self+0x34`, via `sub_8029B2C`) - the same
  shape as `actor_part44.c`'s `sub_802F5E4`/`sub_802F640`. Once that
  depth drops to/below a far threshold (`0x16FF`), triggers a one-shot
  screen-flash (`sub_800132C(0, 2, 1)`, latched via `gUnknown_030014A2`)
  - `sub_802BD64` additionally latches its own one-shot flag
    (`gUnknown_03001480`, the same global `sub_802BD18` reads);
    `sub_802BDD0` doesn't touch it. Once the depth drops to/below a near
    threshold (`0x3FF`), arms hazard direction 1 (`sub_802BD64`) or 2
    (`sub_802BDD0`) via `sub_802A668`.
- **`sub_802BE34`** - the third axis of the same hazard-threshold
  family, but driven directly off `self+0x20` (a fixed `-0x100`
  decrement per call, no shared accumulator, no `self+0x24`/`self+0x34`
  derivation) and arming hazard direction 3.
- **`sub_802BE80`** - frame-counter-threshold (`self+0x44 == 0x1e`)
  state-transition, structural twin of `sub_802BD24`: latches
  `gUnknown_030014A3`, then either (input bit 1 of `gUnknown_030007E0`
  clear) resets `self` to state 1/table-index 0 via the same reset
  idiom and fires `sub_8029BAC(0x24)`, or (bit set) transitions to
  state 2 and fires `sub_8029BAC(0x38)` instead.

## Compiler-quirk notes

- **`sub_802BE80`'s boolean-truthy zero-extension idiom.** The ROM
  computes `gUnknown_030007E0 & 2` into a register, then zero-extends it
  through an explicit `lsls #0x10`/`lsrs #0x10` pair before comparing
  against 0 and branching - not the plain `ands`/`cmp #0`/`bne` a direct
  `if ((gUnknown_030007E0 & 2) == 0)` produces. The zero-extended
  register (`r3`) is then reused as a general-purpose "0" for several of
  the reset idiom's own field stores inside the `if`-branch, instead of
  materializing a fresh `movs r3, #0`. Reproduced by making the masked
  value a real `u16 bit = gUnknown_030007E0 & 2;` local (the 16-bit
  truncation is what triggers the shift-pair zero-extension at `-O2` on
  this compiler) and then using `bit` itself, not a literal `0`, for
  every one of those stores inside the `if (bit == 0)` branch - gcc then
  reuses the same register for both roles, matching the ROM exactly.
  The `else`-branch's `self+0x28 = 2` store needed the same treatment
  from the other direction: gcc had already loaded the AND mask constant
  `2` into a register for the `ands` instruction above, and without a
  register pin it CSE'd that live `2` into the store instead of
  reloading a fresh literal - the ROM does reload a fresh `movs r0, #2`
  there, so `state` (the store value) needed its own explicit
  `register s32 state asm("r0") = 2;` to force a fresh load and block the
  CSE.
- **Comparison-polarity gotcha in `sub_802BD64`/`sub_802BDD0`** (a real
  bug this pass caught only via the full-link `make compare`, not the
  isolated compile - see "A note on isolated-compile confidence" below).
  The ROM's screen-flash trigger fires when the derived depth
  (`self+0x34`) drops *to or below* `0x16FF` (`cmp r1, r0; bgt <skip>` -
  branch away when *greater*, i.e. execute when `<=`), not *above* it.
  An initial reading of the raw `cmp`/`bgt` pair as "value greater than
  threshold fires the block" was backwards; the ROM's `bgt` is the
  *skip* branch, not the *take* branch, so the actual C condition is
  `*(s32 *)(self + 0x34) <= 0x16FF`. The isolated per-function compile
  didn't catch this because the wrong condition (`> 0x16FF`) still
  compiles to a *valid*, byte-plausible-looking `ble`/`bgt` pair (just
  the opposite one from the ROM) - only diffing the real linked ROM
  against `baserom.gba` (`cmp baserom.gba crashbandicootxs.gba`, first
  mismatch at byte 0x2BD96 = ROM address 0x0802BD96, landing squarely on
  this branch's `movs r0, #0` inside `sub_802BD64`) surfaced it. The
  second threshold in the same two functions (`self+0x34 <= 0x3FF` for
  the `sub_802A668` hazard-direction arm) was read correctly from the
  start.

## A note on isolated-compile confidence

This chunk is a second confirmed instance (after issue #50's own
`sub_802AA80`/`sub_802AAB4` register-choice regression, see
issue-50-actor-2a69c.md) of docs/workflow.md's warning that an isolated
per-function compile is a diagnostic tool, never proof of a match - but
this time the gap wasn't a context-dependent register-allocation choice,
it was a plain misreading of a `cmp`/`bgt` pair's branch polarity that
happened to still compile to *some* plausible-looking branch either way.
The isolated compile of `sub_802BD64`/`sub_802BDD0` with the wrong
(`> 0x16FF`) condition produced clean, well-formed Thumb code - nothing
about it looked broken in isolation, and a fast visual scan of the
generated `.s` output initially misread its `ble` as matching the ROM's
`bgt`. Only the real full `rm -rf build`/`make compare` cycle against
`baserom.gba`'s checksum caught it, confirming again that step 6 is not
optional busywork even for "obviously simple" branch-threshold
functions.

See [docs/status/actor.md](../status/actor.md) for the running
matched/parked list this entry feeds into.
