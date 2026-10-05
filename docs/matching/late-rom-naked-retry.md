# Late-ROM NAKED retry (0x0802F7B0-0x08039B44, issues #56/#58/#61/#63/#66/#67/#68)

A retry pass over the NAKED functions left in the late ROM, plus the one
function there still linked from raw asm (`GAX2_estimate`). Every
function was tried under both agbcc and old_agbcc.

## Closed (2)

| Function | File | Compiler | What it took |
|---|---|---|---|
| `InitLanguageSelectGraphics` | `src/frontend/language_select.c` | both | The icon-manager steps as `static inline` helpers taking the manager (`IconSetBase`/`IconReserveVram`, the idiom from `InitCredits` in credits.c), plus one `u32 zero` local shared by the `field_8` and `field_108` stores. |
| `FillBgPictureMap` | `src/actor/bg_picture.c` | old_agbcc (object joined `OLD_AGBCC_OBJS`) | Map entry read as `v = *map; v += base;`, high nibble masked as `(*nib >> 4) & 0xf`, and the function declared `inline` ahead of `LoadBgPicture`. |

### `InitLanguageSelectGraphics`

The old note said the ROM rematerializes the `0x108`/`0x12c`/`0x130`
field offsets after every call while the compiler CSEs them into
callee-saved registers. Separate inline expansions give each call site
its own offsets, as in credits.c. That left one difference: the
ROM keeps a 0 in r8 and uses it for both the cursor's `field_8 = 0` and
`IconSetBase(DC, 0)`. A `u32 zero = 0;` local passed to both reproduces
it.

### `FillBgPictureMap` (and why 7B0 inlines it)

`LoadBgPicture`'s loop has two separately strength-reduced store pointers
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

- **`LoadBgPicture`** (bg_picture.c, #56). The draft now inlines 8E8
  and has the ROM's exact instruction stream, but two priority ties go
  the other way (32 halfwords): the nibble pointer (13 refs over 56
  insns) narrowly outranks `dest` (9 over 40) for r5, and `cols`/row+1
  tie for r8/sl. Nothing tried moved either one: 7B0 local
  declarations and types, argument spellings, pseudo-number shifts,
  or nine nibble-read spellings in the inline body (all of which keep
  8E8 matching).
- **`ContinuePromptLoop`** (continue_prompt.c, #63). The new draft is 2 halfwords
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
- **`ConvertAirshipTiles`** (airship_graphics.c, #58) and **`ConvertHovercraftTiles`**
  (hovercraft.c, #61), the fill-level meter twins, are still about 95
  and 105 halfwords off. The first loop in the ROM spills each height
  and reloads it (`ldm r1!`) after the row-pointer store, because all 8
  low registers are taken. That includes `&gAirshipMapTileBase`, which is
  hoisted into r7, where the drafts give it r8 and keep the height
  in a register. None of these reproduced it: `sum += heights[k] = ...`,
  walking-pointer forms, `volatile` on one access, a volatile row store,
  or an inline helper with the N-row count as a parameter.
- **`__udivsi3`** (libgcc2.c, #66). This is
  lib1funcs.asm's hand-written routine, not compiler output. It stays a
  NAKED transcription (a final state, as already recorded).
- **`GAX2_estimate`** (#66). It was raw `asm/code_3_2_20c.s`. It is now
  `lib/gax/src/gax_work_size.c`, a NAKED transcription with a documented C
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
- **`GaxCreateHandlers`**, **`GAX2_init`**, **`GaxChannelMix`** (#66/#67/#68).
  These are large GAX2 functions. The existing drafts stay 289/422/438
  halfwords off, which is register allocation throughout. One finding
  for `GAX2_init`: its copy loops compare signed (`ble`), so they use
  an `s32` counter separate from the unsigned tap-scan index. That
  alone doesn't change the count; the r8/r9 swap of the max tap rate
  and the format pointer still cascades (the max rate's initial 0 is
  also the CSE'd zero stored to `numSfx` and the state fields).

## Second pass: the near-misses (2 of 4 closed)

A follow-up aimed at the four drafts above that were closest, with the
extra-reference nudge from #468: `asm("" : : "r"(x))` emits no code but
adds one reference to `x`, which raises its global-allocation priority.
A brute-force variant runner found both matches.

| Function | File | Compiler | What it took |
|---|---|---|---|
| `ContinuePromptLoop` | `continue_prompt.c` (object joined `OLD_AGBCC_OBJS`) | old_agbcc | `asm("" : "+r"(k))` on the input copy between the `& 1` and `& 8` tests, and one extra reference to `audio` at the top of the loop in place of the r8 pin. |
| `LoadBgPicture` | `bg_picture.c` | old_agbcc | The DMA width fixed, one extra reference to `dest` after the loop, two to `cols` before the call, and 7B0 inlining its own `static inline` copy of the loop. |

### `ContinuePromptLoop`

The first test's r0/r1 swap came from the separate `u16 p` local that
stopped CSE sharing the shift with the `& 8` test. Testing
`k.pressed & 1` directly and putting `asm("" : "+r"(k))` inside the
second operand of the `||` (as a statement expression) keeps the two
shifts apart and gives the ROM's registers. Without the pin, the pair
counter took r7 from `audio`. One extra reference to `audio` at the top
of the loop restores the ROM's order (`audio` r7, counter r8). The same
reference before the loop does nothing.

### `LoadBgPicture`

- The draft copied the palette with `DmaCopy32(3, pic, PLTT, 0x400)`,
  which gives control word 0x84000100. The ROM's is 0x80000100,
  `DmaCopy16(3, pic, PLTT, 0x200)`. That accounted for one of the 32
  "register" halfwords.
- Two `asm("" : : "r"(cols))` before the call settle the `cols`/row+1
  tie (r8/sl).
- One `asm("" : : "r"(dest))` after the loop lifts `dest` over the
  nibble pointer (r5/r6). A reference at the top of the row loop also
  works. The call argument is spelled `tileData + tiles * 32`, which
  gives the ROM's `adds r6, r1, r5` operand order.
- Any `dest` reference in the shared body breaks `FillBgPictureMap`, and no
  placement satisfies both. So 7B0 inlines a `static inline MapFill`
  copy that has the reference, and 8E8 is the plain loop written out
  after 7B0. A wrapper 8E8 that inlines `MapFill` doesn't work: the
  inlined copy has 7B0's two separate store pointers.

### Not closed: `ConvertAirshipTiles` / `ConvertHovercraftTiles` (#58/#61)

Both drafts improved (95 to 56, and 105 to 29 halfwords):

- **Height re-read.** `asm("" : "+m"(heights[k]))` right after the
  row-pointer store makes gcc reload the height, which is the ROM's
  `ldm r1!`. `&gAirshipMapTileBase` then lands in r7, as in the ROM.
  Walking pointers, `s32 x` temporaries and struct forms didn't do it.
- **`dst` split.** The ROM keeps `dst` in r3 between rows and in sb
  inside the row. A copy `u32 *d = dst;` for the inner loop, then
  `dst = d;`, reproduces that.
- **Left (both):** in the nibble expansion the ROM copies the hoisted
  0xf and ANDs the byte into it (`adds r4, r6, #0; ands r4, r0`). gcc
  copies the byte instead (`adds r4, r0, #0; ands r4, r6`), and that
  permutes the other temporaries. None of these changed it: `0xf & b`,
  a mask variable (function scope or loop scope, any type),
  `p = 0xf; p &= b;` (right order, but then the 0xf isn't hoisted),
  in-place `b >>= 4`, u8/u32/s32 variants of `MeterPx`, or
  `src++`/`*src++` placement.
- **Left (80336CC only):** the ROM starts the reversed row counter
  from `sum`'s zero register (`adds r2, r5, #0`), and orders the
  second loop's header loads slightly differently.
- **Spellings.** The twin's draft uses the "nibble into a variable,
  then `MeterPx(p)`, `src++` after the high nibble" spelling, which has
  the ROM's instruction order. In 8031604 the same spelling costs 4
  bytes in the outer loop header (a `mov rX, sp` copy), so that draft
  keeps the `*src++` spelling.

## Later pass: GAX NAKED retry 2

`GAX2_estimate` and `GaxCreateHandlers` are now real C. Every open point listed
above for `GAX2_estimate` came down to source shape:
- `p->layout`/`p->flags` re-read at every use, so GCSE makes the halfword
  flags slot and the unreduced carving loop;
- `/` for both divisions;
- one shared loop counter;
- four documented no-code `asm("")` that set GCSE's hash-table size,
  and with it the stack-slot order.

See [gax-naked-retry-2.md](./gax-naked-retry-2.md).
