# Issue #13, fourth pass: `sub_800FF0C`

GitHub issue #13 (`0x0800FC70-0x08010A0C`, physics/collision subsystem,
`game_loop` category - see
[docs/matching/issue-13-graphics-fc70.md](issue-13-graphics-fc70.md) for
the first pass,
[docs/matching/issue-13-fc70-continuation.md](issue-13-fc70-continuation.md)
for the second, and
[docs/matching/issue-13-fc70-second-continuation.md](issue-13-fc70-second-continuation.md)
for the third) had one function left: **`sub_800FF0C`**
(`asm/code_3_2_17_e560_ff0c.s`, ROM `0x0800FF0C`, 1396 B / ~660
instructions), the last still-raw entry in this issue's original span.
This pass closes it.

## The trampoline family, read first

Two entire files exist purely to call `sub_800FF0C` with a fixed
constant `type` (the 5th, stack-passed argument) and nothing else:

- `src/graphics/graphics_loading_21bfc.c` (GitHub issue #33) - `type`
  `0` through `7`. `type == 0`'s caller (`sub_8021D04`) does extra
  post-processing: it re-derives the same `gUnknown_030012B4 -> *P ->
  {+8 array, +0xc base}` placement-record lookup `sub_800FF0C` itself
  uses internally (indexed by `arg3<<1`), and folds two of the
  record's own flags-byte bits (`0x2`/`0x4`) into the constructed
  object's `+0x28` bitfield *after* `sub_800FF0C` returns - overriding
  the bits `sub_800FF0C`'s own common tail had just cleared.
  `sub_8021BFC` picks `type` `7` or `6` depending on
  `sub_80232C8(gUnknown_030012C0)`.
- `src/graphics/graphics_loading_21668.c` (GitHub issue #31) - `type`
  `0x12` down to `7` (overlapping `graphics_loading_21bfc.c` at `7`
  through a second, independent trampoline `sub_8021BD8`).

Together every `type` from `0` to `0x12` (18) - all 19 values - is
externally confirmed by a real caller. This matches `sub_800FF0C`'s own
internal **second** jump table exactly (19 cases, case IDs `0`-`0x12`),
which is the strongest single piece of evidence that the second table
is indexed directly by this same `type` value (after the possible
overrides described below).

## Function overview

`void *sub_800FF0C(u16 arg0, u16 arg1, u16 arg2, u16 arg3, u8 type)`

1. Allocates a `0x64` (100)-byte object via `sub_8026EDC(0x64)`
   (`self`), zero-initializes `self+0x59`, then calls
   `sub_800FEB0(self)` (already matched, game_loop22.c - clears the
   collision-response state/countdown/neighbor-list-pointer block).
   Sets `self+0x18 = &gStaticData_087E4074` - a real address inside the
   documented 93-entry `gStaticData_087Exxx` vtable family, but at a
   **`+0x18` offset** rather than the `+0xC` convention every other
   constructor this project has matched uses for the same table-pointer
   slot. This function is the first confirmed `+0x18` outlier of that
   family - worth keeping in mind if a future pass finds more `+0x18`
   sites (it may be a second, distinct vtable-pointer slot on this
   object type, not a mistake in the established `+0xC` convention).
   Stores `arg0` at `self+8`.
2. **`type == 9` early special-case**: if `arg0 != 0xFFFF` and
   `sub_802599C(*gUnknown_030012B4, arg0)` (the same "is this placement
   record slot occupied/confirmed" check used throughout) is true,
   `type` is forced to `0`.
3. **Resource-pressure demotion**: unless `gUnknown_030012C0+0x8c` is
   set, or `sub_80232E0(gUnknown_030012C0) >= sub_8023128(gUnknown_030012C0)`
   (both take the same argument - read as "how many of this entity
   kind currently exist" vs. some capacity/threshold, i.e. the pool is
   already at or over capacity), `type == 0xb` or `type == 0xf` gets
   demoted via the placement record's own flags byte (indexed by
   `arg3<<1`, same convention `sub_8021D04` above uses): flag `0x40`
   forces `type = 2`, flag `0x80` forces `type = 1`, and - `0xf` path
   only - placement-record byte `+1` bit `0x1` forces `type = 9`.
4. Sets `self+0x20 = ***gUnknown_030012D0 + 0x174` - the
   `self+0x20`-pointer-to-manager/`self+0x2d`-tag/0x1c-stride
   hitbox-record table every sibling in this subsystem
   (`sub_0800D18C`, `sub_8010480`, etc.) already establishes.
5. **First jump table** (index `type - 1`, valid for `type` `1`-`15`;
   any other `type` skips straight to step 6): marks a "treat this
   placement record as pre-flagged" local flag (`special`) for `type`
   in `{1, 9, 11, 12, 15}`. For `type == 3`, if the placement record is
   confirmed present (`sub_802599C`), escalates `type` to `7` outright
   - the rest of the function then runs exactly as if `type` had been
   `7` from the start. Every other in-range `type` is a no-op here.
6. **Merge block**: looks up the placement record again. If `special`
   is set, or the record's own flags byte has bit `0x20` set, sets a
   second local flag (`flagged`) and computes `self+0x54` from the
   record's `+4` halfword (`0x15` if it equals `0x1b`, else the raw
   signed value) - then, if `gUnknown_030012C0+0x8c` is set, overwrites
   `type` with `self->0x54 - 0x15`.
7. **Second jump table** (index `type`, `0`-`0x12`) - see the table
   below.
8. **Common tail**: clears `self+0x28` bits `0x10`/`0x20`, folds
   `sub_800815C(self)`'s low nibble into `self+0x29`'s low nibble,
   writes `self+0`/`+4` (position) from `arg1<<8`/`arg2<<8`. If the
   placement record confirms presence and `type` was `0xb` or `0xf`,
   checks placement-record flag `0x80` to force `type = 1`. If
   `type == 1` and `arg0 != 0xFFFF` and the placement record confirms
   presence, re-tags `self+0x2d = 0x1b` (27), reruns the
   `sub_80087C0`/`sub_80087B4`/`sub_800872C` sprite/animation trio, and
   initializes `self+0x30` from the 28-byte-stride hitbox-record
   table's own `+0x16` count minus one; also folds `type`'s low bit
   into `self+0x4d` bit 0 (keeping bit 7). Writes `self+0x4e = type`
   unconditionally. If `type == 5` and the placement record confirms
   presence, calls `sub_800F5B8(self)` - see below. Finally registers
   `self` via `sub_8009B70(*gUnknown_0300130C, self)` and returns
   `self`.

## Type-code table

| `type` | caller(s) | `self+0x2d` tag | notes |
|---|---|---|---|
| `0` | `sub_8021D04` (21bfc.c) | `0x1f` (31) | caller does extra `+0x28` flag post-processing |
| `1` | `sub_8021CE0` | `0x1a` (26) | `self+0x50` = bit 6 of placement-record flags; table-1 `special` |
| `2` | `sub_8021CBC` | `0x17` (23) | |
| `3` | `sub_8021C98` | `3` (or escalates to `7`) | `self+0x50/0x51/0x4c` = record `[6]/[7]/[8]`; escalates to `type 7` if record present |
| `4` | `sub_8021C74` | `0x18` (24) | |
| `5` | `sub_8021C50` | `0x15` (21) | `self+0x50/0x51` = record `[6]/[7]`, `self+0x48` = signed record `[8:9]`; post-tail calls `sub_800F5B8(self)` (game_loop49.c, issue #12) if record confirmed |
| `6` | `sub_8021BFC` (else branch) | `4` | |
| `7` | `sub_8021BFC` (if branch), `sub_8021BD8` | `0x20` (32) | also the `type 3` escalation target |
| `8` | `sub_8021BB4` | `2` | |
| `9` | `sub_8021B90`, also the `type 9` early-case and the `0xb`/`0xf` demotion targets | `0x1c` (28) | table-1 `special`; if not `flagged`, `self->0x54 = 0x15` and (if busy-flag set) `type` bookkeeping resets to `0` post-dispatch |
| `0xa` | `sub_8021B6C` | `5` | |
| `0xb` | `sub_8021B48` | `0` | `self+0x51` = record `[6]`; table-1 `special`; also a demotion source (step 3) |
| `0xc` | `sub_8021B24` | `0x19` (25) | `self+0x48 = -42`; table-1 `special` |
| `0xd` | `sub_8021B00` | `6` | |
| `0xe` | `sub_8021ADC` | `0x11` (17) | |
| `0xf` | `sub_8021AB8` | `7` | largest single case - see below; table-1 `special`; also a demotion source (step 3) |
| `0x10` | `sub_8021A94` | `0xe` (14) | |
| `0x11` | `sub_8021A70` | `0xf` (15) | |
| `0x12` | `sub_8021A4C` | `0x10` (16) | only case whose tag-write isn't reached via the shared jump-to-`0x2d`-write tail (falls straight through - already-optimal in the ROM's own codegen) |

`type == 0xf`'s case (`_0801028E` in the original disassembly, the
largest of the 19) also: looks up a tile/graphics asset via
`sub_8006DF8(*gUnknown_030012B8, byte)`, masks `self+0x48` down to its
`0x38` bits, looks up `gStaticData_0816BB94[(self->0x48 & 0x38) >> 3]`
for `self+0x4f`, writes `self+0x51` from the placement record's `[6]`
byte, and folds three placement-record `+1` flag bits
(`0x2`/`0x4`/`0x8`) into `self+0x50`.

## The `sub_800F5B8` cross-tie

`type == 5`'s post-tail call to `sub_800F5B8(self)` is a direct,
concrete link between this constructor family and the issue #12
physics/collision cluster: `sub_800F5B8` is matched as `NAKED` in
`src/system/game_loop49.c` (issue #12 Phase 2). This confirms `type 5`
spawns an entity that immediately participates in that cluster's own
collision-response state machine.

## Matching notes

Written as `NAKED` asm, not plain C. Two nested jump tables (15 and 19
cases), three extended registers (`r8`/`sb`/`sl`) live across the whole
function (`r8` = `arg3 << 1`, an index reused throughout every
placement-record lookup; `sb` = `arg0`; `sl` = a placement-record
pointer, live only across `type == 0xf`'s case), and the same
`self+0x20`/`self+0x2d`/28-byte-stride hitbox-record register shape
this project's issue #12/#13 write-ups already established as a
confirmed gcc-2.9-resistant allocation (see `sub_0800D18C`/
`sub_800E08C`, game_loop47.c/game_loop48.c) made a plain-C attempt a
poor bet given the size (~660 instructions, roughly four times any
function this project has successfully matched as real C). Transcribed
instruction-for-instruction from the ROM disassembly (cross-checked
against `arm-none-eabi-objdump -D -b binary -m arm
--adjust-vma=0x08000000 -M force-thumb` on `baserom.gba` directly,
which matched the pre-existing `asm/code_3_2_17_e560_ff0c.s` text
exactly - no transcription drift found), mnemonics translated to the
plain/divided syntax this project's other `NAKED` functions use
(`adds`->`add`, `movs`->`mov`, `ands`->`and`, `lsls`/`lsrs`->
`lsl`/`lsr`, `orrs`->`orr`, `subs`->`sub`, `rsbs rX,rX,#0`->
`neg rX,rX`), with the original `_08XXXXXX:` labels renumbered to
GNU-as local numeric labels (first-definition order).

**One real gotcha**: the first attempt forgot the `NAKED` attribute on
the function declaration. Since the body is still a single `asm(...)`
statement, `agbcc` happily compiled it - but without `naked`, the
compiler still appends its own default epilogue after the inline asm
(`bx lr` + `nop`, 4 bytes), even though the asm block itself already
ends with the ROM's own real `pop {r1}; bx r1` return sequence. This
produced a `.text` section 4 bytes too long (`0x578` instead of the
expected `0x574` = 1396 B), which cascaded into a ROM-wide checksum
mismatch (`make compare` reported "La suma no coincide", and the built
`.gba` was 4 bytes larger than `baserom.gba`, with the first divergent
byte deep in the header from a shifted pointer constant - a useful
signature for "some object grew/shrank by N bytes" style problems in
general on this project). Adding `NAKED` to the declaration and
rebuilding fixed it - `arm-none-eabi-objcopy -O binary
--only-section=.text` on the two `.o` files then diffed byte-identical.

Verified byte-exact via a full clean `make compare`
("La suma coincide").

## Cross-references

- `docs/status/game_loop.md` - matched list updated for this pass;
  `sub_800FF0C` moved out of "Still raw, category-mapped".
- `tools/report_units.py` - the `0x0800FF0C` unit now points at the new
  `src/system/game_loop36.o` instead of `None`.
- `ldscript.txt` - `game_loop36.o` replaces the deleted
  `asm/code_3_2_17_e560_ff0c.o` in link order (immediately before
  `game_loop35.o`).
- `asm/code_3_2_17_e560_ff0c.s` - deleted, fully consumed.
