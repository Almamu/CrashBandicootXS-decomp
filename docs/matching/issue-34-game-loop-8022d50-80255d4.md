# GitHub issue #34 follow-up: `sub_8022D50` (parked, NAKED) / `sub_80255D4` (parked, NAKED)

Picked up the two remaining raw functions the `game_loop` category's
`docs/status/game_loop.md` still listed under the `UpdateGameFrame`-
`MainLoop` cluster: `sub_8022D50` and `sub_80255D4`. `sub_80255D4` was
initially left raw in a first pass (see below), then picked up again in
a second follow-up pass once its second half was fully traced - see the
"NAKED transcription" section below.

## `sub_8022D50` - NAKED transcription, `src/system/game_loop40.c`

Fully traced against the ROM: `self` (every caller passes
`*gUnknown_030012C0`, the same per-level state object
`sub_8022BF0`/`sub_8022CA0`, game_loop.c, and the `self+0x80`/`0x84`/...
accessor family, game_loop2.c, operate on) gets two fields cleared
(`self+0x8c` as a byte, `self+0x90`-`0xa0` as five zeroed words), then -
unless `self+0xdc`'s level object is already in state 3 - the two actor
slots at `self+0x1b8`/`0x1bc` are torn down (`sub_80087C0`/
`sub_80087B4`/`sub_800872C(..., 0)`, the same OAM-trio teardown
`sub_802375C`, game_loop39.c, already uses) when non-null. `self+0x1bc`'s
actor additionally feeds its own `+0x20`-table/`+0x2d`-tag hitbox record
(the same convention `sub_8010480`, game_loop35.c, and `sub_8010674`,
game_loop23.c, already document) into `sub_8006D08` (the tile-asset-cache
slot loader) - `self+0x29`'s low nibble is the cache slot, and the
record's own `+0x14` byte is the asset id. Finally `gUnknown_030012EC`
(a `dual_array_manager`, per `actor_part11.c`'s canonical definition) is
walked: each entry fires its own `+0x18`-table's `+0x48`/`0x4c`
`sub_803AD7C` trampoline, and a result of `2` fires the `+0x28`/`0x2c`
trampoline too - a nonzero low byte there flags the entry for despawn
(`sub_8011448(entry, 1)`), otherwise the entry is marked "seen" (`+0xc`
bit 0) and, unless its `+8` id is the `0xFFFF` sentinel, its bit gets set
in the `gUnknown_030012B4+0x108` collision bitmap - the exact same
inline idiom `sub_80072D8` (graphics.c) uses on a `struct actor`.

Every one of those pieces matches byte-for-byte in isolation with a
register pin or a small anchored `asm volatile` block: `self` pinned to
r5 for its whole life (the natural allocator otherwise spills a
redundant second copy), the `self+0x8c`/`self+0x90` zero-fill-through-
`self+0xdc` read anchored (this compiler either CSEs the two addresses
together or peephole-merges the unrolled zero-fill into `stmia`, neither
of which the ROM does), several `movs <const>` instructions anchored
ahead of their address computation (the ROM materializes constants
before addresses; a plain C store evaluates the address first), the
`part->table+0x48` record pointer anchored so it's advanced in place and
reused for `fn` (a plain rewrite either keeps `table` live across the
first `sub_803AD7C` call, which the ROM never does, or re-derives
`part->table` a second time for `fn`, which the ROM also never does),
the `flags |= 1` store anchored to match `sub_80072D8`'s own
constant-first idiom, and `self+8` read twice - once for the sentinel
compare, again for the bitmap math - since a shared load collapses both
into one.

The one piece that never converged: the loop's own `0xffff` sentinel has
to live in r7 for the *entire* array walk. A `register s32 sentinel
asm("r7")` pin hits a confirmed toolchain bug - this compiler never adds
an inline-asm-clobbered r7 to the function's own push/pop list (the same
gap already parked for `sub_8010674`'s `success` local, game_loop23.c,
and `sub_8025A64`, game_loop29.c - see `docs/status/game_loop.md`). A
*plain* (non-pinned) `s32 sentinel = 0xffff;` local dodges the bug and
gets tracked correctly, but only in isolation - once every other quirk
above is also anchored (needed to get the rest of the loop byte-exact),
register pressure shifts enough that the natural allocator stops landing
`sentinel` on r7 at all (it moves to r8, or gets displaced entirely by a
newly-hoisted `&gUnknown_030012EC` cache landing on r7 instead - a
correctness bug, not just a mismatch, since the sentinel compare would
then silently compare against the wrong value). No source phrasing
tried here reproduces the ROM's simultaneous "sentinel pinned to r7,
push/pop tracked correctly, nothing else hoisted into r7" state at once.
Transcribed straight from the confirmed-correct ROM disassembly instead.

Full clean `make compare` (`La suma coincide`) confirms the NAKED
transcription byte-exact.

## `sub_80255D4` - NAKED transcription, `src/system/game_loop41.c`

Follow-up pass on the entry directly above: the full instruction-level
trace was finished this time (every field, offset, branch and call
argument pinned down against the ROM), so this is now understood with
byte-exact-reconstruction confidence rather than left raw. `self` here
is `*gUnknown_030012B4` (the same collision-bitmap base
`sub_8025944`/`sub_8025968`/`sub_802599C`, game_loop12.c, and
`sub_8025A0C`, game_loop13.c, already operate on).

**First half** (DMA/`CpuSet` refresh + group/item walk): if `list` (a
new "list" pointer) differs from `self`'s own cached copy at `+0`,
`self+8`/`self+0x208` (the first two of the three overlapping
collision-bitmap arrays that family already documents) get
DMA-zero-filled (64 bytes each, matching a plain `DmaFill32(3, 0, dest,
64)`), then unconditionally `self+8`→`self+0x108` and
`self+0x208`→`self+0x308` get `CpuSet`-copied (the same
`sub_803A94C(src, dst, 0x04000040)` idiom `sub_8022CA0`, game_loop.c,
already documents in the opposite direction). `self+4` is set from
`posArg >> 8`. Then `list` itself is walked as a `{count:u16@2,
groups:ptr@4}` header (the same shape `sub_8025894`'s own matched
reconstruction, game_loop12.c, documents for a sibling
list) over `{count:u16@2, items:ptr@4}` 8-byte group records, each
holding `{tableIdx:u16, p1:u16, p2:u16, p3:u16}` 8-byte item records; for
each item not already flagged in the `self+8` bit-grid (`sub_8025968`),
`sub_8025D28` (the table-indexed interworking-trampoline dispatcher,
game_loop14.c) fires with a running, never-reset-per-group counter as
its own `self` argument, indexing `gUnknown_030012E4`'s table.

**Second half** (everything gated on `redirectInfo`, a count-prefixed
`{u32, u32}` array - null skips it entirely, otherwise the first word
is the count and the array starts right after): for each
`gUnknown_0300130C` (`struct actor_list`) entry in reverse, its `+8` id
is looked up in `redirectInfo`'s array (`.a` field, read as a full
`u32`); a match's paired `.b` value is used to search
`gUnknown_0300130C` *again*, in reverse, for an entry with that id,
linking the two via `sub_8010714`/`sub_8010710` (the neighbor-list
set-next/set-prev pair, game_loop23.c) on success. On failure the code
chases a *second* lookup back into `redirectInfo`'s array itself
(treating it as an id→id redirect table, `.a`→`.b` again) and retries
the actor-list search with the new id - a "follow a redirect chain
until something in the live actor list matches" loop, with one
asymmetry transcribed verbatim from the ROM: a redirect hit at array
index 0 specifically is treated as "give up" (stop chasing, move to the
next actor-list entry) rather than "keep chasing with the new id" the
way every other index is. A second, independent forward pass then walks
`redirectInfo`'s array by index and looks each entry's `.a` id (this
time read as a `u16`, not the first pass's `u32` - a genuinely different
load width for the exact same field, confirmed against the ROM and
transcribed as-is rather than "cleaned up" to one consistent width) up
in `gUnknown_0300130C` directly (forward this time); on a miss it
chases the same kind of `u16`-width id→id redirect chain through the
array until a match is found or the chain runs out. Once a match is
found (either pass), its `+0x18`-table's `+0x10`/`+0x14` `sub_803AD7C`
trampoline record's returned `+5` byte becomes a `(byte+1)<<8` Q8 delta
added to the matched entry's own `+4` field, then every entry in its
`sub_801070C` ("get next") neighbor chain has
`sub_8007398(entry, entry+0, entry+4+delta)` fired on it in turn - a
position-resync pass over whatever got linked.

The real-world *meaning* of `redirectInfo`'s two `u32`/`u16` fields
(what relationship is being re-linked, why entry 0 is special-cased, why
a second, differently-widthed pass repeats similar logic) still isn't
named - nothing in the ROM gives that away without wider context this
pass didn't chase down - but that's no longer what's blocking a
byte-exact reconstruction; the control flow itself is fully pinned down.

**NAKED, not plain C**: a real C reconstruction was attempted and got
the *entire* first half byte-for-byte identical to the ROM once `self`
and the group-loop's running counter were pinned to their ROM registers
(`register void *self asm("r6")`, `register s32 counter asm("r7")`,
each scoped to its own block so the pin doesn't outlive the ROM's own
use of that register) - confirmed via an isolated compile diffed
instruction-for-instruction against the ROM disassembly. The second
half is where it breaks down: the ROM keeps `redirectInfo`'s
`{count, array}` decomposition split across `r8` (count) and `sb`/`r9`
(array base) for the *entire* second half - `r8`/`sb` pins reproduce
that much cleanly - but the ROM never caches the array base in a single
low register the way a normal C local would. `sb` is a high register,
which Thumb's 3-operand `add`/indexed-load encodings restrict; at every
individual use site that needs the array base combined with an index,
the ROM re-issues a fresh `mov rX, sb` into whichever low register
happens to be free at that exact point - `r1` in one place, `r4` in
another, `r6` in a third, never the same choice twice in a row. A plain
C pointer local (even given its own register pin) gets allocated to
*one* register for its entire lifetime instead, which is a real,
different, and smaller register footprint than the ROM's own
repeated-rematerialization pattern. Reproducing it exactly would mean
hand-placing a distinct inline-asm anchor at every one of the dozen-plus
individual use sites - at which point it is no longer a C
reconstruction in any meaningful sense, just NAKED asm wearing a C
function signature. This is the same family of gcc-2.9
high-register/3-operand-add materialization gap already parked
elsewhere in this ROM region for similarly register-heavy functions
(`sub_8025B0C`/`sub_8025BAC`/`sub_8025CA4`, `sub_8025E98`/`sub_8025F3C`
above, both keeping `r8` live across most of their bodies). Transcribed
straight from the confirmed-correct ROM disassembly instead - every
label, branch and literal-pool placement (including the ROM's four
redundant re-loads of `&gUnknown_0300130C` into separate nearby literal
pools, one per Thumb `ldr`-range-limited region) carried over
unmodified.

Full clean `make compare` (`La suma coincide`) confirms the NAKED
transcription byte-exact. `sub_8025894`, which used to share
`asm/code_3_2_17_255d4.s` with `sub_80255D4`, was unaffected by this
pass and stayed parked `NON_MATCHING` in `src/system/game_loop12.c` at
the time - it has since been matched as real C (the whole raw file is
now gone) - see
[docs/matching/issue-41-game-loop-25894.md](issue-41-game-loop-25894.md).

## `UpdateGameFrame` - NAKED transcription, `src/system/game_loop55.c`

Picked up the last big raw piece of the `UpdateGameFrame`-`MainLoop`
cluster: `UpdateGameFrame` itself (ROM `0x080225A0`-`0x08022BF0`, ~730
instructions), called once a frame from `MainLoop`
(`src/system/main_loop.c`) with `self` = `gUnknown_030012C0` - the same
per-level state object `sub_8022BF0`/`sub_8022CA0` (`game_loop.c`) and
the `self+0x80`-`0xc4`/`+2` accessor family (`game_loop2.c`) already
operate on. `docs/matching.md`'s original entry for this chunk (search
"GitHub issue #34: `0x080225A0`-`0x080231C4`") sketched the shape but
flagged its own transition-target list as possibly loose transcription
and left the end-of-frame stack slots uncharacterized; this pass traced
every branch against the raw ROM bytes directly and confirms/corrects
both.

**Confirmed 5-case player-state dispatch map.** The switch key is
`self->0xc4 - 0x14` (`bhi`-gated to 0-4; anything else falls through to
a separate `self->0xdc`-level-object-state-3` OR-set branch, see
below). Crucially, `self->0xc4` is *not* a separate "player state"
field distinct from the retry-loop's frame-tick counter documented
below - it is the exact same field, doing double duty: clamped to
`<=0x17` (23) and fed to `sub_801BAF0` every retry-loop pass, *and*
used as the switch key here. So the 5 special states are literally
`self->0xc4` values `0x14`-`0x18` (20-24). Corrected transition-target
map (the original doc's `sub_8023190`/`8184`/`819C`/`80231A8`
ordering was slightly off against the real per-case targets):

| case | gate (skip transition if true) | transition callee | extra work | `sub_8022468` mode |
|---|---|---|---|---|
| 0 | `sub_80231BC` | `sub_8023190` | `sub_801D41C`, `sub_80067D4` | 4 |
| 1 | `sub_80231CC` | `sub_80231A8` | `sub_801D41C`, `sub_80067C4` | 5 |
| 2 | `sub_80231B4` | `sub_8023184` | `sub_801D41C`, `sub_80067B4` | 6 |
| 3 | `sub_80231C4` | `sub_802319C` | `sub_801D41C`, `sub_80067A4`, then unconditionally: `sub_800697C(self) > 0x63` frames increments `self->0xc4` (advances to the next state) and zeroes `*(self+0xc8)`, mode 8, `goto` the post-category-reset block directly; otherwise mode 0xa | 8 or 0xa |
| 4 | (none - unconditional) | (none) | `sub_80354BC` | 9 |

Values >4 (i.e. `self->0xc4` outside `0x14`-`0x18`) instead check
`self->0xdc`'s level object's `+8` state field; if it's `3`,
OR-sets bit 0 on `sub_8023404(self)`'s returned byte pointer (a flags
byte on a per-frame sub-object). All 5 gate functions return "still
in this state, do nothing more" when true; the transition callee only
fires when the gate says "no longer in this state."

**Level-load loop** (function entry, before the state dispatch):
allocates a `0x220`-byte scratch buffer (`sub_8026EDC`, matches
`src/graphics/level_graphics.c`'s own doc comment for this exact
allocation) and hands it straight to `LoadLevelGraphics`, then polls
`sub_8035E14`; while it returns `2` ("still loading") the loop calls
`sub_80354BC` (map/progress-screen trigger) and repeats. Once
`sub_8035E14` returns something else: `0` triggers
`sub_8022468(*gUnknown_030012C0, 2)`, anything nonzero triggers a
`sub_8004D4C`/`sub_800300C(1,0)`/`sub_8004D20` input-poll bracket
(purpose not chased further, out of scope for this pass).

**End-of-frame stack-slot semantics** (the doc's "8 still-
uncharacterized SP-relative locals" - there are 7 word-slots plus the
one scratch halfword at `sp+0`, all now traced):

- `sp+0x0` (halfword): a single zeroed halfword, used once as the DMA3
  fixed-source operand for the entry-time zero-fill below; dead after.
- `sp+0x4`: a snapshot of `self->0x78` taken once, at the start of each
  fresh per-level dispatch round (`self->0x78` is a progress/lives-style
  counter judging by `game_loop.c`'s `sub_8022BF0`). Read back exactly
  once, at the *top* of the outer state-dispatch loop (label reached
  only via the loop-back branches at the very end of the function):
  when the just-finished category loop's status flag (`r8`) was `2`
  ("done"), `self->0x78` gets clamped to not exceed this saved value -
  a rollback guard preventing the counter from advancing past what it
  was when the current dispatch round started.
- `sp+0x8` = `&self->0xac`.
- `sp+0xc` = `&self->0xbc`.
- `sp+0x10` = `&self->0xcc`.
- `sp+0x14` = `&self->0xe0` (byte field).
- `sp+0x18` = `&self->0xe4`.

`self->0xe4` and `self->0x14c` are a **double-buffered `0x68`-byte
snapshot pair** of `self`'s own first `0x68` bytes (`0xe4 + 0x68 ==
0x14c`, confirmed adjacent) - DMA3-zeroed via the classic fixed-source
trick at function entry (`self[0:0x68)` zeroed by 52 fixed-source
16-bit DMA3 writes, control word `0x81000034`: bits 24-23 = `10`
source-fixed, bit 26 = 0 sixteen-bit unit, count = `0x34` = 52
halfwords = `0x68` bytes), then copied to `self+0x14c` as the initial
backup. Every pass through the retry loop (top label reached both from
function entry and via `beq`-back-to-self) restores `self[0:0x68)`
*from* `self+0x14c` (undo whatever the previous attempt did), then
re-snapshots the freshly-restored bytes into `self+0xe4` (a second,
independent backup) before deciding via `sub_801BAF0(&self->0xc4)`
whether to proceed (byte result `0`) or poll input and possibly clear
`self->0xe0` and retry (nonzero result).

**Category-processing loop** (the function's other major loop,
entered once the retry loop and state dispatch above both settle):
walks `self->0xdc`-style "current category" objects, gating everything
on an `r8`-resident status flag persisted across iterations - `0`
means "keep going," `1` means "check `sub_803AFEC(self) < 0` for an
early exit," `2` means "stop the whole per-category loop now." Inside
each iteration: `sub_8024404`/`sub_80232B8` gate one
`gUnknown_030012B4` bitmap flush+ping-pong-to-`self+0x1b4` cycle into
`sub_8022BF0`; `sub_80243E0`/`sub_8023290` gate a parallel second
cycle into `sub_80235E4` (same ping-pong shape, different consumer -
apparently two independent bitmap "channels"). The loop's tail
(`sub_80232B8`/`sub_8023290` again) decides between two closing
branches that both refresh the HUD icon via `sub_8024464` +
`sub_8028568`: the "true" branch also refills `self+0xb0`/`0xb8`/`0xb4`
(`sub_802325C`/`sub_803AFEC`/`sub_8023414`) via `sub_8024540`; the
"false" branch only refills `self+0xb4` via `sub_8024524`. Either way
the loop re-enters at its own top unless `sub_802455C(&self->0xc4)`
says otherwise, at which point control falls to the end-of-frame block
that (if `gUnknown_03001318`, the HUD object, is non-null) calls
`sub_8028574(hud, 3)`, then decides whether to loop all the way back to
the outer state-dispatch entry (`sub_803AFEC(self) >= 0`, or
`sub_8034CB0()` true after also re-running `sub_80231E4(self)`) or
finally return to `MainLoop` - meaning a single `UpdateGameFrame` call
from `MainLoop` can internally re-run its entire state-dispatch +
category-loop body multiple times before actually returning.

**NAKED, not plain C.** At this instruction count with three registers
persisted across the *entire* function body (`r7` = `&self->0xc4`, `sl`
= `&self->0xc8`, `r8`/`sb` alternating as a category-status flag and
`&gUnknown_030012B4`) plus the six SP-relative field-address slots
above all live simultaneously across two nested nine-way-branch loops,
this is well past the register-pressure range this project's C
reconstruction toolbox (register pins, opaque `asm volatile`
materialization, `goto`-based restructuring) has closed in one pass -
no attempt was made to force a plain-C reconstruction of the whole
730-instruction body; the semantic trace above stands on its own.
Transcribed mechanically from the confirmed-traced ROM disassembly
(previously `asm/code_3_2_17_225a0.s`, now retired) - every mnemonic
converted to this project's suffix-less-Thumb-mnemonic convention and
every named label renumbered to GNU local numeric labels, with no
semantic changes. Full clean `make compare` (`La suma coincide`)
confirms the transcription byte-exact; `make NON_MATCHING=1 report`
compiles the new `src/system/game_loop55.c` warning-free.
