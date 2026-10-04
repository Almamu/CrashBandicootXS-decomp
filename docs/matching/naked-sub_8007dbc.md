# `sub_8007DBC`: matched via NAKED asm transcription

`sub_8007DBC` (ROM `0x08007DBC`, `src/graphics/actor_part2.c`) is the
`part`-vs-player collision handler documented at length in
`docs/matching.md`'s "Parked, not matched: `sub_8007DBC`" entry: two
`part->flags` bit tests gate an AABB collision test against the player
global `gPlayer` (via the already-matched `sub_8007B98`/
`sub_8001688`), a hit plays a sound (the `sub_8007048`-style
`table+0x68` offset/`table+4` dead-read idiom keyed off
`part->field_0A`), marks itself in the `gEntityFlags` bitmap at
`+0x108` (`sub_80072D8`'s convention), and finally
`part->field_0A - 0x1b` selects one of six "kind" values passed to
`sub_8025BAC(gEntitySpawner, 0x2b, kind, part->x>>8, part->y>>8, 0)`
to spawn an object at `part`'s position.

The plain-C reconstruction (still readable in git history) got every
operation, operand, and instruction order right except one systematic
register letter: the cached `&gPlayer` address landed in
`r6` instead of the ROM's `r7`. Since that value is read from
repeatedly across several basic blocks (both `sub_8007B98` calls, the
`_call_via_r4` sound-position lookup), the single-register mismatch
cascaded into nearly every later instruction's register numbering.
Pinning the cached-address local directly to `r7` didn't just fail to
schedule correctly (the usual "pin silently dropped from the
push/pop list" failure mode already documented at
`matching_decomp_register_pinning` memory point 10) - it crashed the
compiler outright with `internal error--unrecognizable insn`. This is
the categorical r7-pin limitation cross-referenced from several other
files in this ROM region (`sub_8007B00`, `sub_802D3A8`,
`sub_8002D44`/`sub_8002E20`, `sub_8009FD4`'s neighborhood): this
agbcc build cannot be made to keep a real, cross-block-live value in
`r7` no matter how it's coaxed.

Rather than keep chasing one register, `sub_8007DBC` was converted to
a `NAKED` function with its body written as a single `asm()` block
transcribing the real ROM disassembly (`asm/code_3_2_3.s`)
instruction-for-instruction, the same technique already used for
several functions in `src/system/link_cable.c` and
`src/audio/gax_swi.c`'s `GaxHuffUnComp`. The transcription:

- Uses GNU-as local numeric labels (`1:`/`1f`/`1b`, ...) in the exact
  order the ROM's own branch targets and literal-pool entries appear,
  rather than named labels, since the whole function is one `asm()`
  string.
- Reproduces the ROM's hand-placed literal pools (`gPlayer`,
  the `0xFFFF` sentinel, `gEntityFlags`, and four separate
  `gEntitySpawner` pool copies feeding the six-case spawn switch's
  jump table) at their exact original positions, including the
  jump-table's own indirection through a pool-held table-base address
  (`mov pc, r0` off a hand-built 8-entry `.4byte` table).
- Translates the disassembler's unified-syntax mnemonics to the
  divided syntax this project's other `asm()` blocks use (`adds`→`add`,
  `ands`→`and`, `orrs`→`orr`, `lsls`/`lsrs`/`asrs`→`lsl`/`lsr`/`asr`,
  `subs`→`sub`, `movs`→`mov`, `rsbs rX, rX, #0`→`neg rX, rX`).

Since this is a literal byte-for-byte transcription (not a derived
reconstruction), it reproduces the ROM's own `r7` usage directly and
needs no register-allocation coaxing at all.

Matching this function also retired the `asm/code_3_2_3.s` split: it
held only `sub_8007DBC`, immediately followed by `sub_8007F78`
(already matched in `actor_part3.c`), so moving `sub_8007DBC` into
`actor_part2.c` closes the gap completely - `asm/code_3_2_3.s` was
deleted and its `ldscript.txt` line removed, the same "retire an
emptied split" convention as `asm/code_3_1.s`/`sub_80007EC` and
`asm/code_3_1_697c.o`/`GetCompletionPercent` before it (see `docs/matching.md`).

## Later pass (issue #9 NAKED retry)

Under old_agbcc the global lands in r7 by itself, with no pins. A C
draft (kept under `NON_MATCHING`) is 42 halfwords off in two places:
where the gone-bit OR gets its constant 1, and the spawned part's mode
update. See [issue-9-naked-retry.md](./issue-9-naked-retry.md) for details.

## Later pass (issue #9-#11 NAKED retry)

`sub_8007DBC` is real C under old_agbcc now; `actor_part2.o` is in `OLD_AGBCC_OBJS`. The r7 player-global problem was the compiler, not the source. See [issue-9-11-box-naked-retry.md](issue-9-11-box-naked-retry.md).
