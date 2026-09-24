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
