# Issue #16, final remainder: 0x08011BD4-0x08012FBC (6 functions)

Continuation of
[issue-16-actor-12160.md](./issue-16-actor-12160.md), which matched
four more members of the `gStaticData_0816BF20` 42-slot action-dispatch
table and left the hardest six of that pass's ten-function remainder
untouched: `sub_8011BD4`, `sub_8012420`, `sub_8012694`, `sub_801283C`,
`sub_8012AF4`, `sub_8012D24`. This pass closes out all six - but as
NAKED transcriptions, not real decompiled C (see "Why NAKED" below).

## New files

`asm/code_3_2_17_11bd4.s` (just `sub_8011BD4`, 0x08011BD4-0x08012160)
is removed entirely - its function moves to a new
`src/graphics/actor_part82.c`. `asm/code_3_2_17_12420.s`
(`sub_8012420`/`sub_8012694`/`sub_801283C`, 0x08012420-0x08012A7C) is
also removed entirely, moving to a new `src/graphics/actor_part84.c`.
`asm/code_3_2_17_12af4.s` is trimmed to drop its leading
`sub_8012AF4`/`sub_8012D24` span (0x08012AF4-0x08012FBC, which moves to
a new `src/graphics/actor_part83.c`), now starting at `sub_8012FBC`
(0x08012FBC) - the rest of that file was already out of this issue's
scope per the prior pass's writeup. `ldscript.txt` and
`tools/report_units.py`'s `UNITS` list were updated to place all three
new files in their correct link order (`actor_part82.c` before the
already-matched `actor_part79.c`, `actor_part84.c` between
`actor_part79.c` and `actor_part80.c`, `actor_part83.c` right after
`actor_part80.c` and before the trimmed `code_3_2_17_12af4.s`).

## Semantics (all six)

- **`sub_8011BD4`** (1420 B, `actor_part82.c`) - bails unless
  `self+8 == 0x1d` (the same type gate `sub_8016288`, still raw,
  checks). Otherwise dispatches its third argument (`arg2 - 1`, range
  `0..0x18`) through a 25-case jump table: several cases are thin
  `sub_8012160` wrappers with a fixed id; case 22 and case 23 each run
  a shared 7-case inner dispatch on `sub_80083B8(part)`'s nibble result
  to compute a Q8 position delta from a `gStaticData_0816B300` record
  (or the object's own `+0x24`/`+0x14` fields as a nibble-1-5
  fallback), then reset the state/flag/table-index trio via
  `sub_8015780`; case 24 plays sound(s) gated on `gUnknown_030007E0`
  bits and either resets three `self+0x22..0x24` bytes plus calls
  `sub_8012AF4`, or chains through `sub_8001AC4`/a
  `sub_8006D08`-fed 28-byte-record lookup; case 11 resets a child
  object and pool-releases it via one of two dereference-chain-computed
  slots depending on the player's D-pad remap state; cases 9/10 gate
  `sub_8015558` behind `gUnknown_030012D8+0x68`/`sub_800AAEC` checks.
- **`sub_8012420`** (628 B, `actor_part84.c`) - a `part`-visibility/OAM-
  priority housekeeping pass: re-runs `sub_8012238` on an activity-flag
  change, resets velocity/target fields past two `gUnknown_03001308`-
  anchored screen-space thresholds (the far one also firing
  `sub_80231EC`/`sub_803AD88`), ticks a couple of counters, looks up
  `self+8`'s type in `gStaticData_0816BF20` to fire one `sub_803AD84`
  trampoline call, then writes a small fixed value into `part+0xa` from
  a second, 22-case jump table on the same type.
- **`sub_8012694`** (424 B, `actor_part84.c`) - a helper of
  `sub_801283C`: gated on the D-pad snapshot's bit 1, `self+0x18`'s
  counter being 0, `gUnknown_030012C0` passing `sub_80231CC`, and a
  sub-object type of `6`/`0xb`/`0xc` (each with its own `+0x30 >= 0`
  distance-style gate), bumps `self+0x18`, fires the `+0x50`/`+0x54`
  and `+0x20`/`+0x24` trampoline pairs with type-keyed ids, resets the
  trio to a type-keyed value, plays a fixed sound, and returns 1;
  otherwise returns 0.
- **`sub_801283C`** (576 B, `actor_part84.c`) - a proximity-triggered
  indicator: dispatches `self+8`'s type (`7`/`9`/`0xb`/`0xe`) against
  per-type distance thresholds on `part->field_0x64` (falling back to
  `sub_8012694` first when within a `0x27f` threshold), setting
  `part+0xd` bit 0 and firing the `+0x20`/`+0x24` trampoline (id
  `0x1a`), or tail-calling `sub_80151C8` for type `0xe`. Then, unless
  the type is one of the five gate values, reads the D-pad and remaps
  `self+0x27`'s table-index byte through a further small dispatch
  before a shared tail arming `self+0x31` when the player's `+0x100`
  flag is set.
- **`sub_8012AF4`** (560 B, `actor_part83.c`) - an OAM-visibility/
  priority pass: nudges the player's saved-position word by a fixed
  delta and calls `sub_8009EA8` when `part+0x68` is busy and a flag
  just changed; plays a sound and fires the `+0x50`/`+0x54` trampoline
  (id `0x12`) when `self+8==0`; then, keyed on `self+0x2f`, looks up a
  per-tag `gStaticData_0816B304` record (`(*(self+4))[tag]`), copies 12
  bytes of it to the stack, optionally rescales two fields via
  `sub_80008FC` (busy part + active player), special-cases tag `0x1e`,
  fires one of two `sub_803AD84` trampoline calls with the stack record
  as payload, and clears the flag; repeats a near-identical sequence
  keyed on `self+0x30`/`self+0x28` against the same table.
- **`sub_8012D24`** (664 B, `actor_part83.c`) - a further sibling:
  reads the D-pad and ticks `self+0x25` down on release; fires the
  `+0x50`/`+0x54` trampoline (id `0x12`) and clears `part+0x33` when
  `part+0x38` is set; bumps `self+0x1c`'s frame counter and, while the
  player's type is `0x12` and `+0x30==0`, fires escalating-id trampoline
  calls once the counter crosses one of two thresholds; bails early if
  `sub_8012A7C(self)` reports busy; otherwise dispatches the input
  snapshot's low bits (sound + two trampoline pairs, or a
  `sub_8015398` tail-call, or a `sub_80231C4`-gated trampoline call) -
  every path converging on `sub_80122CC`.

## Why NAKED, not real C

A first plain-C attempt at `sub_8012420` (the smallest, best-documented
of the six) compiled logically-equivalent code - every load, store,
branch, and call matched the ROM's own - but diverged at the prologue:
the ROM reserves an unused 8-byte stack slot and pushes a 5th callee-
saved register (`r7`) that the straightforward C translation never
needed (4 registers, no stack). This is the same unexplained-frame-
shape/register-budget gap this exact table family (`gStaticData_0816BF20`)
already hits repeatedly throughout `docs/status/actor.md`'s "Parked -
NAKED transcription" section (`sub_801434C`, `sub_80145E4`,
`sub_8015038`, `sub_8015238`, `sub_80152F0`, `sub_80156EC`,
`sub_80157C4`, and the whole `sub_8030574`-`sub_8031604` boss-weapon
cluster) - a confirmed categorical difficulty for this class of
function, not a one-off. Given all six of this remainder's functions
share the same field-offset/trampoline conventions and jump-table-heavy
dispatch shape as those already-NAKED siblings (and `sub_8011BD4` in
particular is the single widest jump-table dispatcher attempted in this
codebase so far - 25 outer cases plus two independent 7-case inner
tables), every one was transcribed instruction-for-instruction from the
ROM disassembly instead, the same escape hatch used for
`sub_8001CB8`/`sub_8001DB4` (`src/system/link_cable.c`) and
`sub_801434C` (`actor_part18.c`).

To keep a function this size transcription-error-free, `sub_8011BD4`
was transcribed mechanically (a small Python pass converting each ROM
instruction's unified-syntax mnemonic to its divided-syntax form -
`adds`/`movs`/`subs`/`ands`/`orrs`/`lsls`/`lsrs`/`asrs` drop the
trailing `s`, `rsbs Rd, Rs, #0` becomes `neg Rd, Rs` - and wrapping each
line for the `asm()` string) rather than hand-renumbered local labels;
the ROM's own `_0XXXXXXX` hex-address label names were kept verbatim as
plain, file-local asm symbols (safe here since each NAKED function's
`asm()` block assembles into its own object, so these never collide
with any other translation unit) instead of GNU-as numeric local
labels, eliminating the forward/backward-reference bookkeeping a
hand-renumbering pass of this size would otherwise risk getting wrong.

Every function was verified byte-for-byte against `baserom.gba` via an
isolated `arm-none-eabi-as` assemble + `objcopy`/`objdump` comparison
before being wired into the real build (all differences before linking
were relocation placeholders - `bl` targets and literal-pool addresses
showing as zero/section-relative offsets, resolving correctly once
linked - exactly the expected shape for an unlinked single-object
test), and a full clean `make compare` (`rm -rf build && make
NON_MATCHING=1 report`, then `rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare`) confirms
`La suma coincide` against the real ROM.

## Status

All six of this remainder's functions are now byte-exact, but as NAKED
transcriptions - parked, not matched, per project policy. Every
function GitHub issue #16 originally scoped (this remainder plus the
four matched in `issue-16-actor-12160.md` and the earlier
`sub_8012160`/`sub_8012238`/`sub_80122CC`/`sub_8012A7C` matches) is now
either real C or a verified NAKED transcription - nothing from this
issue's original scope is left raw - but since six functions are NAKED
rather than real decompiled C, the issue itself stays open per this
project's "NAKED doesn't count toward closing" convention (see
CONTRIBUTING.md's "Opening the PR").
