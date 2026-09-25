# Issue #68: `0x0803985C`-`0x08039FFC` (audio, channel bind/envelope/note)

Continues issue #68's pass past `sub_8039818`-`sub_803A22C` (see
[`issue-68-0x08039818-audio.md`](./issue-68-0x08039818-audio.md)). This
pass covers the `0x0803985C`-`0x08039FFC` cluster: 1 of 6 functions
matched as real C, 4 parked as byte-verified NAKED transcriptions, and
one entangled pair (`sub_8039B44`/`sub_8039E50`) left raw.

## Matched

None of this cluster's functions landed as plain C byte-exact against the
real full-ROM link this pass - see "Parked" below for the closest
attempts and why each still needed a NAKED fallback.

## Parked - NAKED asm transcription

- **`sub_803985C`** (`src/audio/gax_channel_bind_instrument.c`) - binds a
  new instrument entry (`table->0x10[cmd]`) to a per-channel voice object
  and resets its envelope/state fields, clearing the binding back out if
  the entry's own first byte flags it invalid, then records `cmd` into
  the current song's per-slot table. A real C reconstruction reproduced
  every field write and even the "two named zero temps" split already
  established for `sub_803A104` (`gax_channel_init.c`), but the ROM's
  `self` register choreography - reloaded fresh from `ip` into a rotating
  r0/r1/r3 cast exactly when each group of field writes needs it, with
  `r3` itself later mutated in place (`adds r3, #0x23`) - always needed
  one extra callee-saved register (`r5`) that the ROM's version doesn't
  spend. This exact function was already set aside for the same reason in
  a prior pass (see `issue-68-0x08039818-audio.md`'s `sub_803985C`
  entry) - this pass's fresh attempt, informed by the redundant-re-fetch
  technique that closed `sub_8038FD0`'s cluster
  (`issue-67-channel-mute-volume-dma-stop.md`), still didn't close it,
  confirming it as a genuine register-choreography gap rather than an
  easy miss.
- **`sub_80398DC`** (`src/audio/gax_channel_note_scheduler.c`) - the
  per-tick pattern-note/priority-steal scheduler with a 15-way command
  jump table. Not attempted as real C this pass - keeps `r8`/`sb` live as
  genuine scratch (a running "steal" candidate index/slot-array base
  pair) across the whole priority-steal block and the jump table, the
  same many-register gcc-2.9 allocation ceiling already documented
  throughout this ROM region.
- **`sub_8039AA4`** (`src/audio/gax_channel_envelope_tick.c`) - per-tick
  envelope/portamento-pitch update. This one got very close: register-
  pinning `self` to `r4` and the two envelope-clamp temporaries landed
  everything except a handful of `ldrsh`-with-non-immediate-offset reads
  in the portamento tail (the same "materialize the field offset into a
  scratch register first" gotcha already documented for `sub_803943C` in
  `gax_sound_handler_info.c` - Thumb's `ldrsh` has no immediate-offset
  encoding). Each individual read could be forced to the ROM's exact
  register via a tiny fixed-register `asm` block, but doing so for all of
  them together kept perturbing an *earlier*, already-correct clamp
  block's register choice - gcc-2.9's hard-register variable reservations
  turned out not to be scoped as tightly as their enclosing C block, so a
  later read's `asm` clobber list changed unrelated, already-matching
  codegen upstream of it. Parked as a byte-verified NAKED transcription
  rather than chase that ripple further within this pass's budget - a
  reasonable next target for a future pass with more room to iterate
  register-by-register.
- **`sub_8039F30`** (`src/audio/gax_note_lookup.c`) - resolves a
  pattern-note index into an interpolated pitch/volume byte from a sorted
  breakpoint table (called by `sub_8039AA4`). A real C reconstruction
  matched this function's full control flow (every branch and
  computation, confirmed by isolated compile), but the ROM's specific
  register choices - `self` kept in `r5`, `table` kept in `r3` for the
  entire function (never the parameter's own `r1`), and a
  `lsls/lsrs #0x10` zero-extension dance for the note-index parameter -
  didn't come out byte-identical from the C forms tried in the time
  available this pass. Parked as a byte-verified NAKED transcription;
  register-pinning every one of these to force the exact rotation (the
  same technique that worked for `sub_8038FD0`'s cluster) is a reasonable
  next step for a future pass.

All four NAKED transcriptions above were verified byte-exact two ways:
directly against `baserom.gba` via an isolated-compile-then-objcopy byte
comparison (skipping only the a few bytes genuinely dependent on final
link addresses - `bl` call-site relocations and a jump table's absolute
address entries), and then again via this pass's full clean
`make compare` pass (`La suma coincide`).

## Left raw

- **`sub_8039B44`/`sub_8039E50`** - one logical note-trigger routine
  split by a manual return-address trampoline: `sub_8039B44`'s tail
  computes a return address into `lr` and `bx`-jumps into `sub_8039E50`
  (which starts with a `nop`/`mov r8, r8` alignment pad, not a real
  instruction), the same entangled-function idiom already left raw for
  `sub_803A278`/`sub_803A318` in the first `0x08039818` pass. Beyond the
  trampoline itself, `sub_8039B44` builds a large stack-resident struct
  (a `sp+0x28`-based, 0x28-byte argument block) passed to `sub_800014C`
  and calls into `sub_8037ECC` (both callees still raw/unnamed
  elsewhere), and reads several parallel per-song-slot tables
  (`gStaticData_0803A874`/`gStaticData_0803A818`/`gStaticData_0803A884`/
  `gStaticData_0803A8B4`/`gStaticData_0803A8C4`) whose shapes aren't
  modeled. Not attempted this pass given its size (over 250 combined
  lines of disassembly) and the established precedent that this manual-
  trampoline shape doesn't factor cleanly into two independent C
  functions - a reasonable next target would be a dedicated NAKED
  transcription pass, given both halves are otherwise fully understood.

See `docs/status/audio.md` for the updated matched/parked/left-raw
summary and `tools/report_units.py`'s `UNITS` table for the exact current
file boundaries.

## Update: `sub_803985C` closed as real C

A later pass closed `sub_803985C` (`src/audio/gax_channel_bind_instrument.c`)
using the same "`self` lives in `ip` for the whole function, never spilled
to a callee-saved register" idiom already established for `sub_80259D4`
(`src/system/game_loop13.c`, see
[naked-sub_80259d4-matched.md](./naked-sub_80259d4-matched.md)):

```c
register void *selfIP asm("ip");
register s32 n asm("r4");
asm volatile("mov %0, %2\n\tadd %1, %3, #0" : "=r"(selfIP), "=r"(n) : "r"(self), "r"(cmd));
```

Every one of the ROM's `mov rX, ip` re-derivations of `self` (one per
group of field writes) is reproduced as its own `register T x asm("rN")
= (T)selfIP;` local, matching the ROM's own rotation through
r0/r1/r3 exactly - this immediately closed the "extra callee-saved
register" gap the two prior passes both hit, since `self` never occupies
a persistent register the allocator has to spend a `push`/`pop` slot on.
The one extra trick needed beyond that: the final `str r2, [r3, #0x3c]`
(clearing the binding back out) reuses the register already holding a
just-materialized `zero16 = 0` constant, but plain C referencing that
same pinned local (`*(s32 *)(...) = zero16;`) let `-O2`'s constant
propagation flatten it back into a fresh literal load in a different
scratch register instead of reusing r2 - a single opaque
`asm volatile("str %1, [%0, #0x3c]" :: "r"(s3c), "r"(zero16))` anchor
(the same "opaque store reusing an already-pinned zero" technique
documented in the register-pinning memory notes) fixed it. Full clean
`rm -rf build && make NON_MATCHING=1 report` (no warnings) and
`rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` both pass (`La suma coincide`).
`tools/report_units.py`'s entry for `0x0803985C` now points at
`src/audio/gax_channel_bind_instrument.o`.

## Update: `sub_8039F30` narrowed to a single instruction, still parked

The same pass made substantial progress on `sub_8039F30`
(`src/audio/gax_note_lookup.c`) without fully closing it - a near-
matching C reconstruction is now kept in-tree under `#if NON_MATCHING`
(the doc comment above the function has the full derivation). Every one
of the techniques already established in this project closed a specific
sub-gap:

- **`self` in r5, `table` in r3 for the whole function**: plain
  register-pinned locals (`register u8 *selfR asm("r5") = self;` etc.)
  work immediately, no special tricks needed - this part was never
  actually the hard part.
- **The "zero-extension dance"**: the ROM's `lsl r0, r0, #0x10` /
  `lsr r4, r0, #0x10` pair (turning `*out`'s old value into a clean
  32-bit `idx`) only reproduces exactly from a single opaque
  `asm volatile("lsl %0, %0, #0x10\n\tlsr %1, %0, #0x10" : "+r"(rawOut),
  "=r"(idx));` - letting plain C do the truncation (`idx = (u16)tmp;`)
  either omits the dance entirely (when `idx` is declared `u16`, since
  `ldrh` already zero-extends and the compiler doesn't re-derive it) or
  reproduces it at the *wrong* point in the function (when `idx` is
  narrower than a full register and read back later, the compiler
  re-inserts a defensive truncate/extend at the *read* site instead of
  trusting the original materialization - the same "hard-register pins
  aren't trusted as clean across statements" pattern noted below).
  Declaring `idx` as `u32` (not `u16`/`s32`) once materialized this way
  avoids every later defensive re-extension.
- **Unsigned vs. signed branches** (`blo`/`bhs`/`bcc`/`bcs` vs.
  `blt`/`bge`): `idx` must be `u32`, not `s32` - a `u16` operand
  promotes to plain (signed) `int` in C regardless of the other
  operand's own signedness, so only declaring the *hard-register*
  side of the comparison as truly unsigned forces the unsigned branch
  encoding.
- **Destructive in-place pointer mutation on a second use**: the ROM
  reads `self->0x22` twice (once early, once again for the loop-point
  remap), recomputing `self + 0x22` into `r0` *both* times without ever
  touching `r5`/`self`'s own register. Plain C's second
  `*(u8 *)(selfR + 0x22)` instead let the compiler decide `selfR` was
  dead afterward and mutate it in place (`adds r5, #0x22`) - fixed by
  forcing the second read through its own fresh
  `register u8 *s0 asm("r0") = selfR + 0x22;` local, the same fix
  already used inside `sub_803985C` above.
- **A cached vs. redundantly-re-read table byte**: the ROM re-reads
  `table[2]` from memory twice (once for the `!= 0xff` guard, once again
  later for the final index computation) rather than caching it in a
  register across the whole block, even though nothing else clobbers
  that register in between - the same "redundant re-fetch" idiom
  documented for `sub_8038FD0`'s cluster. Caching it into one C local
  reused twice saves an instruction the ROM doesn't save, producing a
  4-byte-smaller function and cascading branch-offset mismatches
  throughout the rest of the function.
- **Register-pinned loop counter/scan-pointer pair**: the breakpoint
  scan loop matches by pinning `i` to `r1` and the scan pointer to `r2`
  (matching the ROM's own choice, not gcc's natural r2/r1 swap).
- **A sign-extended `s16` used directly in a later multiply**: the ROM's
  `ldrsh r2, [r0, r5]` (the interpolation slope, materialized via a
  `register u8 six asm("r5") = 6; asm volatile("ldrsh ...")` anchor,
  the same "materialize the offset register first" fix already
  documented for `sub_8039AA4`'s portamento tail) must be declared
  `s32`, not `s16`, once captured this way - otherwise the later
  `slope * delta` multiply re-inserts a redundant `lsl #16`/`asr #16`
  sign-extension pair the ROM doesn't have, for the same "hard-register
  pins aren't trusted as clean" reason as the zero-extension dance
  above.

**What's still open (one instruction)**: the ROM arms the `0x8AD0` "no
note" sentinel via `ldr r0, <pool>` - the constant loads *directly* into
r0, the same register `override` (dead by this point) just occupied.
Every C phrasing tried (a plain literal store, a named local, an
explicitly `r0`-pinned local, an opaque `asm volatile("ldr %0,
=0x8ad0")`, forcing `override` itself to an explicit `r0` pin, adding an
`asm volatile("" ::: "r2")` clobber hint, materializing `last` via its
own opaque `asm volatile` to change its pseudo-numbering) instead makes
this compiler's own constant-pool materialization pass pick `r2` (the
same register already holding the unrelated, already-dead `last`
local), needing an extra `adds r0, r2, #0` copy to reach the ROM's
actual destination register - confirmed at the level of gcc's own
generated intermediate assembly (`ldr r2, .L24` literally appears in the
`.s` output before final assembly), not just the final object bytes, so
this is a genuine gcc-2.9 constant-materialization register-choice
limitation rather than an unexplored C-level phrasing. A structurally
simpler function in the same file family (`sub_803A104` /
`gax_channel_init.c`, already matched) loads the *same* `0x8AD0`
constant directly into `r0` from an unpinned plain C local with no
trouble at all - the difference appears to be the amount of competing
register pressure (`self`/`table`/`out`/`idx` all pinned + `lastVal`
pinned + `last` and `override` both naturally occupying r0-r2) at the
point of materialization in `sub_8039F30`, which this compiler's
constant-pool allocator doesn't handle via true per-register liveness
the way normal register allocation does elsewhere in the same function.
Separately, note that literal-pool *placement* (not just its register)
is itself a distinct, already-solved sub-problem: a plain C literal
store lets gcc's own pool-flush heuristic defer the constant to the
function's own final unconditional branch (reusing the same "b" the
interpolation-vs-exact-match control flow already needs), exactly
matching the ROM's layout; an explicit inline-asm "materialize the
literal locally with its own branch-over" trick gets the *register*
right but costs 4 extra bytes since it can't share that later branch -
so any future attempt at the remaining register gap should keep the
plain-C store (not inline asm) for the literal itself.

Verification: the `#if NON_MATCHING` reconstruction compiles cleanly
under `rm -rf build && make NON_MATCHING=1 report` (no warnings) and is
204 ROM bytes' worth of logic laid out in 208 bytes (one extra 2-byte
copy instruction, cascading into a further 2 bytes of shifted branch
offsets). The default (NAKED) build's full clean `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare` still passes (`La suma coincide`), unchanged from before this
investigation; `tools/report_units.py`'s entry for `0x08039F30` stays
parked (`base_object=None`).
