# Issue #13, third pass: `sub_8010480`/`sub_8010674`

GitHub issue #13 (`0x0800FC70-0x08010A0C`, physics/collision subsystem,
`game_loop` category - see
[docs/matching/issue-13-graphics-fc70.md](issue-13-graphics-fc70.md) for
the first pass and
[docs/matching/issue-13-fc70-continuation.md](issue-13-fc70-continuation.md)
for the second) had one raw cluster left:
`sub_800FF0C`/`sub_8010480`/`sub_80104E4`/`sub_8010674`
(`asm/code_3_2_17_e560_ff0c.s`, `0x0800FF0C`-`0x080106DC`). The second
pass's write-up already notes `sub_8010480`/`sub_8010674` were read and
understood, but a plain-C attempt spread more live values across r0-r7
than the ROM's own tighter allocation used, so both stayed raw. This
third pass picks that back up with more targeted register pinning and
matches both.

## Matched (2 of 4 remaining functions)

- **`sub_8010480`** (`src/system/game_loop35.c`, new file - it sits
  between the still-raw `sub_800FF0C` and `sub_80104E4`, so it can't
  join either neighbor's file) - unless `self`'s own `+0x4d` state byte
  has bit 7 set or its low 7 bits are already nonzero, resets
  `self+0x38` to 0 and clamps `self+0x30`'s index to the
  `self+0x20`-pointer-to-manager/`self+0x2d`-tag/0x1c-stride
  hitbox-record's own `+0x16` count (the same table-lookup convention
  `sub_800D040`, game_loop6.c, establishes). Always tail-fires
  `sub_8007A84(gUnknown_030012CC, self)`, then - only if `self+0x38`
  ended up nonzero - clears `self+0xc` bit 3. Two gotchas, both
  register-pinning:
  - The `self+0x20`-pointer-to-manager/`self+0x2d`-tag record lookup
    (`table + tag*0x1c`) needed every intermediate value pinned to the
    ROM's own register (`p`/`record` in r0, `tagAddr` in r2, `table` in
    r1, `tag` in r5, `idx` in r3), nested in blocks that close each
    pin's scope right as its value dies so a later value can safely
    reuse the same register - the same "which anonymous scratch
    register" gap that forced `sub_8007B00`/`sub_8007B98`
    (`actor_part.c`) fully NAKED, but tractable here with explicit
    pins since this function's register pressure is much lower (no
    `sub_803AFE4`/`sub_803AFDC` calls in the middle). The final
    add (`record = tag*0x1c + table`) also needed the offset written as
    the *left* operand - the ROM's `adds r0,r0,r1` keeps the
    offset's own register (r0) as the destination, which only a
    `tag*0x1c + (s32)table` expression (not `table + tag*0x1c`)
    reproduced.
  - The trailing `self+0xc &= ~8` clear needed the full inline-asm
    anchor (`self` passed as an unused input purely to mark it live -
    without that, this compiler reused r4 in place for the `self[0x38]
    != 0` check just above it, corrupting the address this block
    reads/writes).
- **`sub_8010674`** (`src/system/game_loop23.c`, prepended ahead of the
  already-matched `sub_80106DC` run - it's immediately ROM-adjacent, so
  it joins that file rather than getting its own) - an AABB-overlap
  test between `self`'s own table-driven half-width/half-height box
  (built via the same `sub_803AD7C` table-trampoline convention
  `sub_8007048`/`sub_80070D4`, `graphics.c`, already establish) and a
  caller-supplied `struct aabb *`. Short-circuits true when `self+0xc`
  bit 4 is set or `self+0x44` is nonzero. Several gotchas:
  - **Narrow register-return zero-extend**: returning a `register u8 x
    asm("r1")` variable always compiles to an extra `lsls
    r0,r0,#24; lsrs r0,r0,#24` zero-extend dance this compiler doesn't
    otherwise need - confirmed with a standalone repro (`return`ing a
    register-pinned `u8` unconditionally adds these two instructions
    regardless of context). Declaring the pinned return-path variable
    (and the function's own return type) `u32` instead of `u8` avoids
    it entirely - the ROM's own callers only ever read the low byte, so
    this changes nothing observable while producing byte-identical
    code.
  - **Confirmed r7-never-saved toolchain bug**: the ROM's own
    overlap-test accumulator lives in r7 (`movs r7,#0` / `movs r7,#1` /
    `adds r1,r7,#0`). Pinning it as a `register u32 success asm("r7")`
    - or even just listing `"r7"` in an inline asm's clobber list -
    never gets added to this function's own push/pop list, the same
    confirmed bug already parking `sub_8025A64` NAKED (see
    docs/status/game_loop.md, GitHub issue #41). The fix: leave
    `success` a **plain**, unpinned local. Ordinary if/else control
    flow (`success = 0; ...; success = 1;`) happens to let this
    compiler's own allocator land it in r7 anyway (matching the ROM
    exactly), and - unlike the pinned/clobbered cases - a *naturally*
    allocated register-resident variable *does* get correctly tracked
    for save/restore.
  - **AABB-build "which anonymous scratch register" gap**: the same
    class of gap `sub_8007B00`/`sub_8007B98` (`actor_part.c`) went
    NAKED over. Anchored as one literal `asm volatile` block (self/rec
    passed as inputs purely to mark them live; left/right/top/bottom as
    fixed-register outputs) rather than plain C, which kept letting
    this compiler's scheduler hoist the `self->y` load ahead of the
    still-pending `rec[5] << 7` shift, stealing r3/r0 from each other.
  - **Dest-vs-first-operand split for `box->field_0 + box->field_8`
    (and the `+4`/`+c` pair)**: the ROM computes this sum with the
    *running* edge value (r2) as the first source operand but still
    lands the result in the *freshly-loaded* field's own register (r0)
    - `adds r0,r2,r0`. No source-expression reordering reproduced that
    (this compiler always picks the first-written operand's own
    register as the destination), so it's anchored too, in two small
    `asm volatile` blocks (one per edge pair) sandwiched between plain
    C comparisons/branches - the box-field reads and their reuse via r2
    stayed plain C (`register s32 boxX asm("r2") = box->field_0;`),
    only the sum itself needed anchoring.

## Still left raw - 2 functions

- **`sub_800FF0C`** (`asm/code_3_2_17_e560_ff0c.s`, now trimmed to just
  this one function) - a large (~660-instruction) projectile/
  hazard-spawn dispatcher with two big jump tables and packed bitfield
  arguments; still out of scope for a single pass.
- **`sub_80104E4`** (new `asm/code_3_2_17_e560_104e4.s`) - a large
  (~195-instruction) state dispatcher calling several still-raw
  siblings; not attempted.

## Cross-references

- `docs/status/game_loop.md` - matched list updated for this pass.
- `tools/report_units.py` - the old single `0x0800FF0C` unit (covering
  the whole `sub_800FF0C`-`sub_8010674` span as raw) split into four:
  `0x0800FF0C` (still raw, trimmed `asm/code_3_2_17_e560_ff0c.o`),
  `0x08010480` (new `src/system/game_loop35.o`), `0x080104E4` (still
  raw, new `asm/code_3_2_17_e560_104e4.o`), and `0x08010674` (now the
  start address of `src/system/game_loop23.o`, moved from its old
  `0x080106DC`).
- `ldscript.txt` - `game_loop35.o` and `code_3_2_17_e560_104e4.o`
  inserted between the trimmed `code_3_2_17_e560_ff0c.o` and
  `game_loop23.o`, preserving ROM link order.
