# `sub_8008F20`/`sub_8009914`: NAKED asm transcription (byte-exact, tracked as parked)

**Tracking note**: same convention as `naked-spatial-grid-tail.md` -
these two are byte-exact (confirmed by a full clean `make compare`)
but, being `NAKED` transcriptions of the ROM's own disassembly rather
than real decompiled C, they count as **parked**, not matched, in this
project's tracking. `tools/report_units.py`'s `UNITS` list keeps each
one's own address range at `base_object: None`, and
`docs/status/actor.md` files them under the same NAKED-transcription
write-up as `sub_80096C0`/`sub_80099F0` (the "### NAKED transcription"
list under "## Parked (`NON_MATCHING`, not yet byte-exact)").

Both were previously `#if NON_MATCHING` C reconstructions in
`actor_part11.c` (see `docs/matching.md`, "Parked, not matched:
`sub_8008F20`"/"`sub_8009914`", the original write-ups, for the full
semantic breakdown - unchanged here). They share the exact same
free-list-build loop shape (`sub_8009914`'s own tail is a byte-for-byte
copy of `sub_8008F20`'s), and hit the identical compiler gap, so both
close via the same technique in one pass.

## What each function does

- **`sub_8008F20`** - a fixed-slot object-pool initializer: sets
  `activeCount = 0`, `capacity = count`, allocates `slotArray`
  (`count` pointers, zero-filled), `nodeArray` (`count` 0x14-byte
  nodes), and `freeListArray` (`count` 8-byte `{node, next}` pairs) via
  `sub_8026EC0`, zeros the two 256-word `gridHead`/`gridTail` tables,
  then builds a singly-linked free list threading every `nodeArray`
  entry through its own `freeListArray` wrapper slot.
- **`sub_8009914`** - resets a pool manager to empty: tears down every
  active object (`slotArray[0..activeCount)`, firing each one's
  `table+0x50/0x54` "destroy" trampoline via `sub_803AD80` if
  non-`NULL`, then clearing the slot), resets `activeCount` to 0, zeros
  the grid tables, and rebuilds the free list from scratch over the
  existing `nodeArray` - identical tail logic to `sub_8008F20`.

## Why plain C didn't converge

Both were fully semantically understood (every load, store, field
offset, and branch traced against the ROM disassembly - see each
function's own now-removed doc comment, preserved in git history and
in `docs/matching.md`'s original entries) before falling back to a
transcription, per `docs/workflow.md`'s step 3.

The shared free-list-build loop keeps four persistent cross-iteration
values alive - the item count, the `+0x810`/`+0x814` field addresses
(cached once before the loop, `mov sb`/`mov sl`), the loop index
(reused as the zero-fill counter that precedes it), and a running
"next" byte offset (kept in `r8`, incremented by 8 each iteration
alongside a parallel `+0x14`-per-iteration node-offset counter in a
plain scratch register) - across `r3`/`sb`/`sl`/`r4`/`r8` in one
specific combination. Every isolated-compile attempt produced the
right *shape* (same branches, same use of `ip`/`r8`, even a matching
high-register save/restore prologue/epilogue) but a different concrete
register assignment for several of the four long-lived scalars - the
same "gcc allocates a many-live-value loop differently than the ROM"
gap already catalogued for this cluster's siblings
(`sub_8009008`/`sub_80091D4`, `naked-spatial-grid-tail.md`).

## The fix

Both converted to `NAKED` functions whose body is a single `asm()`
block transcribing the real ROM disassembly instruction-for-instruction,
following the exact same conventions established earlier this session
(`sub_8010B6C`, `game_loop28.c`; `naked-oam-actor-part-batch.md`):
GNU-as local numeric labels in ROM order (`Nf`/`Nb`, reusable since
each number is only ever defined once per function), the ROM's
suffixed Thumb mnemonics (`movs`/`adds`/`subs`/`lsls`) written in their
suffix-less forms (`mov`/`add`/`sub`/`lsl`) - this project's assembler
invocation (divided syntax, no `.syntax unified`) accepts these
identically to the suffixed originals, and a trailing `.align 2, 0`
reproduced at the ROM's own position (mid-function, right after
`sub_8008F20`/`sub_8009914`'s shared `b 7f`/`b 8f` branch, holding the
`0x814` field-offset literal) plus after the function body itself.

Each transcription was verified in isolation before touching the real
tree: assembled with `arm-none-eabi-as -mcpu=arm7tdmi -mthumb-interwork
-I asminclude --defsym NON_MATCHING=0`, compared byte-for-byte against
the same real toolchain's assembly of the original raw `.if
NON_MATCHING == 0` guard bodies (`asm/code_3_2_13.s`/
`asm/code_3_2_13_9914.s`, both retired now that their contents are
NAKED C instead) - identical for both.

## File/link-order changes

`sub_8008F20` sat inside `asm/code_3_2_13.s`, the sole remaining
content of that file (already trimmed down to just this one guard by
an earlier batch, see `naked-spatial-grid-tail.md`). Since it's now a
real (if `NAKED`) function, it no longer needs a guard at all - it's
appended directly to `actor_part11.c`, right after the truly-matched
`sub_8008DC0`-`sub_8008EE4` functions and before the still-`#if
NON_MATCHING`-guarded `sub_8009150` (owned by a parallel effort, left
untouched). `asm/code_3_2_13.s` is retired entirely, and its
`ldscript.txt` line removed - `actor_part11.o`'s own compiled output
now ends exactly where that file used to begin, so no other reordering
is needed.

`sub_8009914` sat inside `asm/code_3_2_13_9914.s` (also already trimmed
to just this one guard by the same earlier batch). Unlike `sub_8008F20`,
its real ROM address (`0x08009914`) does **not** sit adjacent to
`actor_part11.c`'s own functions - `sub_8009150`, `sub_8009008`,
`sub_80091D4`, `sub_800944C`, `sub_8009528`, `sub_80096C0`, and
`sub_8009868` all sit between them in ROM order, each already living in
its own translation unit. Per `docs/workflow.md` step 4 ("a function
whose real address isn't adjacent to an existing matched file's
functions needs its own new `.c` file"), `sub_8009914` moved to a new
`src/graphics/actor_part11i.c` instead - the same reasoning that gave
`sub_80096C0` its own `actor_part11e.c` earlier, `sub_8009528` its own
`actor_part11f.c`, `sub_8009150` its own `actor_part11g.c`, and
`sub_800944C` its own `actor_part11h.c` - `sub_8009914` landed on "i"
purely because all three of those letters were already claimed, by
three separate parallel PRs, by the time this branch rebased onto them
(three times, in fact - this file was renamed during each rebase:
f -> g -> h -> i).
`asm/code_3_2_13_9914.s` is retired entirely, and `ldscript.txt`'s
corresponding line now points at the new `actor_part11i.o`, in the
exact same link-order slot the old guard file occupied (between
`actor_part11d.o` and `actor_part12.o`).

`tools/report_units.py` gained two new `base_object: None` entries at
`0x08008F20` and `0x08009914` (splitting `actor_part11.o`'s own entry
right before the first, and `actor_part11d.o`'s neighbor `sub_8009868`
entry right before the second) - the same "NAKED function embedded in
an otherwise-matched file still gets its own address-boundary report
entry" convention already established for `sub_8008044`
(`actor_part3.c`)/`sub_8033B44`(`actor_part31.c`)-style cases.

Full clean `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` confirms `crashbandicootxs.gba:
La suma coincide` after this change, alongside `make NON_MATCHING=1
report` + `objdiff-cli report generate` (both functions report as
their own `raw_0800XXXX` units, matching the established parked-NAKED
convention - `actor_part11`'s own unit is 100% matched across its 6
remaining real functions).
