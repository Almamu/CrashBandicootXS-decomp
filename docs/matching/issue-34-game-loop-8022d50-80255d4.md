# GitHub issue #34 follow-up: `sub_8022D50` (parked, NAKED) / `sub_80255D4` (left raw)

Picked up the two remaining raw functions the `game_loop` category's
`docs/status/game_loop.md` still listed under the `UpdateGameFrame`-
`MainLoop` cluster: `sub_8022D50` and `sub_80255D4`.

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

## `sub_80255D4` - left raw

Re-read against the ROM disassembly with several callees now
characterized that weren't when this was last looked at (docs/matching.md's
GitHub issue #40 entry): `self` here is `*gUnknown_030012B4` (the same
collision-bitmap base `sub_8025944`/`sub_8025968`/`sub_802599C`,
game_loop12.c, and `sub_8025A0C`, game_loop13.c, already operate on).

The first half is now solid: if `arg1` (a new "list" pointer) differs
from `self`'s own cached copy at `+0`, `self+8`/`self+0x208` (the first
two of the three overlapping collision-bitmap arrays that family
already documents) get DMA-zero-filled (64 bytes each, matching a plain
`DmaFill32(3, 0, dest, 64)`), then unconditionally `self+8`→`self+0x108`
and `self+0x208`→`self+0x308` get `CpuSet`-copied (the same
`sub_803A94C(src, dst, 0x04000040)` idiom `sub_8022CA0`, game_loop.c,
already documents in the opposite direction). `self+4` is set from
`arg3 >> 8`. Then `arg1` itself is walked as a `{count:u16@2,
groups:ptr@4}` header (the same shape `sub_8025894`'s own `#if
NON_MATCHING` reconstruction, game_loop12.c, documents for a sibling
list) over `{count:u16@2, items:ptr@4}` 8-byte group records, each
holding `{tableIdx:u16, p1:u16, p2:u16, p3:u16}` 8-byte item records; for
each item not already flagged in the `self+8` bit-grid (`sub_8025968`),
`sub_8025D28` (the table-indexed interworking-trampoline dispatcher,
game_loop14.c) fires with a running, never-reset-per-group counter as
its own `self` argument.

The second half (everything gated on `arg2`, a count-prefixed
`{u32, u32}` array) is where confidence runs out: for each
`gUnknown_0300130C` (`struct actor_list`) entry in reverse, its `+8` id
is looked up in `arg2`'s array; a match's paired value is used to
search `gUnknown_0300130C` *again* for an entry with that id, linking
the two via `sub_8010714`/`sub_8010710` (the neighbor-list set-next/
set-prev pair, game_loop23.c) on success. On failure the code chases a
*second* lookup back into `arg2` itself (treating it as an id→id
redirect table) and retries the actor-list search with the new id -
apparently a "follow a redirect chain until something in the live actor
list matches" loop. The exact real-world meaning of `arg2`'s two `u32`
fields (what relationship is being re-linked, and why a chase through
`arg2` itself is needed when the direct actor-list search fails) isn't
pinned down with the confidence a byte-exact reconstruction attempt
needs, and the function's overall register pressure (`r8`/`sb`/`sl`/`ip`
all live simultaneously across nested loops) matches the shape already
flagged elsewhere in this ROM region as resistant to this compiler's
allocator even once matching. Left completely untouched rather than
force a low-confidence reconstruction or an under-understood NAKED
transcription - real bytes stay in `asm/code_3_2_17_255d4.s`, byte-for-byte
unchanged.
