# Late-ROM NAKED retry (0x0802F7B0-0x08039B44, issues #56/#58/#61/#63/#66/#67/#68)

A retry pass over the NAKED functions left in the late ROM, plus the one
function there still linked from raw asm (`sub_8037FC0`). Every
function was tried under both agbcc and old_agbcc.

## Closed (2)

| Function | File | Compiler | What it took |
|---|---|---|---|
| `sub_8037388` | `src/audio/counter_selector_icons.c` | both | The icon-manager steps as `static inline` helpers taking the manager (`IconSetBase`/`IconReserveVram`, the idiom from `sub_8034CEC` in actor_part131.c), plus one `u32 zero` local shared by the `field_8` and `field_108` stores. |
| `sub_802F8E8` | `src/graphics/actor_part45d.c` | old_agbcc (object joined `OLD_AGBCC_OBJS`) | Map entry read as `v = *map; v += base;`, high nibble masked as `(*nib >> 4) & 0xf`, and the function declared `inline` ahead of `sub_802F7B0`. |

### `sub_8037388`

The old note said the ROM rematerializes the `0x108`/`0x12c`/`0x130`
field offsets after every call while the compiler CSEs them into
callee-saved registers. Separate inline expansions give each call site
its own offsets, as in actor_part131.c. That left one difference: the
ROM keeps a 0 in r8 and uses it for both the cursor's `field_8 = 0` and
`IconSetBase(DC, 0)`. A `u32 zero = 0;` local passed to both reproduces
it.

### `sub_802F8E8` (and why 7B0 inlines it)

`sub_802F7B0`'s loop has two separately strength-reduced store pointers
(`dest[c]` and `dest[c + 0x3e0]`, stepped side by side). Written
standalone, gcc combines them into one pointer plus `0x7c0`, which
matches 8E8 but not 7B0. An inlined copy of the same loop keeps them
apart. 8E8 follows 7B0 in the ROM because gcc defers emitting an
inlinable function until the end of the file. So 8E8 is declared
`inline` before 7B0 and 7B0 calls it. With 7B0 NAKED, the object is
unchanged.

The two register swaps the old note blamed on the allocator both come
down to spelling:

- map pointer vs `dest` (r5/r6): in global allocation the tile value `v`
  outranked the map pointer. Splitting the read into `v = *map;` then
  `v += base;` fixes it.
- next-row pointer vs row+1 (ip/r9): these are two GCSE-created
  temporaries whose creation order depends on hash order. Masking the
  high nibble, `(*nib >> 4) & 0xf`, flips it. (Declaring 6-20 unused
  locals between `odd` and `r` also flips it; the mask is the real
  spelling.)

## Not closed

- **`sub_802F7B0`** (actor_part45d.c, #56). The draft now inlines 8E8
  and has the ROM's exact instruction stream, but two priority ties go
  the other way (32 halfwords): the nibble pointer (13 refs over 56
  insns) narrowly outranks `dest` (9 over 40) for r5, and `cols`/row+1
  tie for r8/sl. Nothing tried moved either one: 7B0 local
  declarations and types, argument spellings, pseudo-number shifts,
  or nine nibble-read spellings in the inline body (all of which keep
  8E8 matching).
- **`sub_8034994`** (actor_part89.c, #63). The new draft is 2 halfwords
  off under old_agbcc. It needed:
  - the input word read as a struct copy, so each test does a word load
    and `lsrs #16`, and the 0x80 test does a fresh `ldrh` after the
    calls;
  - a separate `u16` for the first test, otherwise CSE shares its shift
    with the `& 8` test;
  - `while (dir >= 0)` instead of `while (1)`, otherwise jump.c moves
    the return-value block up to the `break`;
  - the pair counter pinned to r8.

  What's left: in the first test the ROM ties the `& 1` result to the
  constant's register (`movs r0, #1; ands r0, r1`) where we tie it to
  `pressed`'s. Variants of the mask, pins, locals, inline accessors,
  unions/bitfields, and separate or `do {} while (0)`-wrapped tests
  either keep this tie or let CSE merge the shifts.
- **`sub_8031604`** (actor_part26c.c, #58) and **`sub_80336CC`**
  (actor_part130.c, #61), the fill-level meter twins, are still about 95
  and 105 halfwords off. The first loop in the ROM spills each height
  and reloads it (`ldm r1!`) after the row-pointer store, because all 8
  low registers are taken. That includes `&gUnknown_03001530`, which is
  hoisted into r7, where the drafts give it r8 and keep the height
  in a register. None of these reproduced it: `sum += heights[k] = ...`,
  walking-pointer forms, `volatile` on one access, a volatile row store,
  or an inline helper with the N-row count as a parameter.
- **`sub_8037E54`** (`__udivsi3`, math_div64_util.c, #66). This is
  lib1funcs.asm's hand-written routine, not compiler output. It stays a
  NAKED transcription (a final state, as already recorded).
- **`sub_8037FC0`** (#66). It was raw `asm/code_3_2_20c.s`. It is now
  `src/audio/gax_work_size.c`, a NAKED transcription with a documented C
  draft under `#if NON_MATCHING`: the ROM's control flow and most
  blocks, about 240 halfwords off. Reaching even that needed:
  - the two frame-buffer adds as separate statements (`size +=
    frames * 2;` twice);
  - walking `tap` pointers for the DSP rate scans;
  - a signed `max` for the alternative-layout scan.

  What's left:
  - the ROM keeps `flags` in a halfword stack slot (`strh`/`ldrh`);
  - it orders the rate/maxRate/max/layout slots differently;
  - it doesn't strength-reduce the first carving loop and re-reads
    `layout->count` on every iteration. A `goto` loop reproduces that
    loop, but then `size` loses r7;
  - it re-derives `layout->types[0]` for the tap scan, where the draft
    hoists it.
- **`sub_8038240`**, **`sub_8038538`**, **`sub_8039B44`** (#66/#67/#68).
  These are large GAX2 functions. The existing drafts stay 289/422/438
  halfwords off, which is register allocation throughout. One finding
  for `sub_8038538`: its copy loops compare signed (`ble`), so they use
  an `s32` counter separate from the unsigned tap-scan index. That
  alone doesn't change the count; the r8/r9 swap of the max tap rate
  and the format pointer still cascades (the max rate's initial 0 is
  also the CSE'd zero stored to `numSfx` and the state fields).
