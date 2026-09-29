# Data: turning ROM data into source

Every compiled function in the ROM is C. The data after the code
(`0x0803B8B0` to the end of the ROM) is still mostly `.incbin
"baserom.gba", ...` in `data/data.s`. This document is the convention for
turning it into real source, starting with pointer tables. For how data
progress is measured, see [decomp_dev.md](./decomp_dev.md), "Data
progress".

## The convention: C `const` arrays in `src/data/`

A converted table is a C `const` object in a `src/data/*.c` file, with a
real element type (from a shared header when the consuming code has one)
and real symbol references for every pointer. The linker script places
the file's `.rodata` at the table's ROM address, between the raw parts of
`data/data.s`.

- **One file per contiguous run of converted tables.** Name it
  `<what>_<ROM offset>.c`, the offset in lowercase hex without the
  `0x08` (`action_table_16bf20.c` is at `0x0816BF20`), the way
  `src/graphics/actor_part_16048.c` is named. Two C files can sit next to
  each other when their tables are unrelated (`bg_package_16c58c.c`,
  then `image_table_16c5a0.c`).
- **Symbol names don't change.** The code references
  `gStaticData_XXXXXXXX`, and so does the frozen `expected/` assembly the
  code report diffs against, so a converted table keeps its label. The
  meaning goes into the type and a comment naming the consumer. A new
  label is fine where a table had none (`gStaticData_0816C2B0`), and a
  label nothing references can be dropped when a C table spans it (see
  `actor_category_175558.c`).
- **Types come from the code's headers** when there is one:
  `struct actor_pmf` (`actor_self.h`), `struct category_descriptor` /
  `struct category_vtable` (`actor_anim.h`), `struct bg_package`
  (`graphics_package.h`), and the new `struct vtable_slot` (`vtable.h`).
  When the only view is a struct local to one `.c` file, the data file
  uses a plain type or a small local struct and says whose view it
  mirrors. Moving such structs into headers is a separate cleanup.

### Layout: `data/data.s` sections and `ldscript.txt`

`data/data.s` is split into sections at every C object. Each section is
named after the ROM address of its first byte, and a comment marks the
hole:

```
gStaticData_0816BF14:
	.incbin "baserom.gba", 0x0016BF14, 0x0000000C

@ gStaticData_0816BF20..gStaticData_0816C070: src/data/action_table_16bf20.c

.section .rodata.0816C090

.global gStaticData_0816C090
gStaticData_0816C090:
	.incbin "baserom.gba", 0x0016C090, 0x000001C0
```

The `/* Data */` block of `ldscript.txt` lists every piece in ROM order:

```
build/crashbandicootxs/data/data.o(.rodata);
build/crashbandicootxs/src/data/action_table_16bf20.o(.rodata);
build/crashbandicootxs/data/data.o(.rodata.0816C090);
...
```

An input-section name in the linker script matches exactly, so
`data.o(.rodata)` never pulls in `.rodata.0816C090`. The first section is
plain `.rodata` (from `0x0803B8B0`). This scales to any number of C
objects, generated ones included: each one is one more line pair.
`src/data/*.c` is already covered by the Makefile's `src/*/*.c` wildcard;
nothing else needs registering. Everything the linker script doesn't
name is discarded (`/DISCARD/`), so a C data file's empty `.text` is
harmless.

### Converting a table

1. **Find it and decode it.** Dump the blob's words. `0x08xxxxxx` values
   are ROM pointers. Odd ones that land on a function symbol (`nm
   crashbandicootxs.elf`) are Thumb function pointers, and even ones point
   at data. Then read how the C uses it: `grep -rn gStaticData_XXXXXXXX
   src include`. The patterns found so far:
   - `{0xFFFF0000, fn}` pairs: a gcc 2.x pointer-to-member-function
     (thisOffset 0, index -1 = non-virtual, then the code address), the
     tables behind `ACTOR_PMF_CALL`. Initialize them with
     `ACTOR_PMF(fn)`.
   - `{0, fn}` pairs after an all-zero first pair: a gcc 2.x virtual
     table (`struct vtable_slot`, `VTABLE_SLOT(fn)`).
   - Plain words that are all odd code addresses: a function-pointer
     array.
   - Records with pointers among scalars: find the struct the consumer
     reads them through (`struct bg_package` is `{w, h, palette, tiles,
     map}`).
2. **Write the C.** One `const` object per label, in ROM order. Declare
   each function it references as `extern void sub_XXXX();` (unprototyped,
   so it can't clash with the real signature) and each data label as
   `extern const u8 gStaticData_XXXX[];`. A pointer into the middle of a
   still-raw blob is `gStaticData_XXXX + 0x20`. Put a comment on each
   table naming the functions that read it.
3. **Cut it out of `data/data.s`.** Delete the labels' blocks, leave the
   `@ first..last: src/data/file.c` comment, and start a new
   `.section .rodata.<ADDR>` at the next label, `<ADDR>` being that
   label's ROM address. (When the next thing is already another C
   object, no new section is needed.)
4. **Add it to `ldscript.txt`:** the object's `(.rodata)` line and the new
   `data.o(.rodata.<ADDR>)` line, right after the `data.o` section the
   table was cut from.
5. **Check.** `make compare` must print `crashbandicootxs.gba: OK`. Then
   `rm -rf build objdiff.json && make NON_MATCHING=1 report &&
   objdiff-cli report generate -o report.json`: the table shows up as a
   `data_src_data_<file>_<ADDR>` unit at 100%, and `matched_data` grows by
   its size. `tools/report_units.py` stops with an error if the layout
   doesn't add up (a table of the wrong size shifts every raw blob after
   it off its incbin offset).

### What agbcc does with `const` data

Checked by compiling test tables and by the conversions themselves:

- A `const` object goes to `.section .rodata`, in definition order,
  each preceded by `.align` for its type (`.align 2, 0` for anything with
  a word or pointer), with zero fill. No hidden padding between objects
  beyond that alignment.
- A function reference is emitted as `.word sub_XXXX`. The linker sets
  the Thumb bit itself: the R_ARM_ABS32 relocation against a Thumb
  function symbol yields the odd address, so never write `sub_XXXX + 1`.
- Designated initializers work (`{ .fn = sub_XXXX }`), which a union
  whose first member isn't the pointer needs (`struct actor_pmf`'s
  `u.vtableOffset`/`u.fn`).
- Integer constants cast to pointers work in initializers
  (`(void (*)(void))0xffffffef` for the non-code slots of
  `struct category_vtable`).

### Pitfalls

- **Forgetting `const`.** A non-`const` initialized object goes to
  `.data`, which the linker script discards: the bytes vanish from the
  ROM (or the link fails if the code references the symbol). A pointer
  array needs the `const` on the array, not the pointee:
  `const u8 *const tbl[]`, `void (*const tbl[])()`.
- **Clashing declarations.** Consumers declare these tables `extern`
  with their own (often non-`const`, often `u8[]`) types, each in its own
  translation unit, which is harmless. But a data file can't include a
  header that declares the same symbol without `const`
  (`conflicting type qualifiers`). Either make the header's declaration
  `const` or don't include it. Making a header declaration `const` can in
  principle change the consumers' code (gcc treats loads from `const`
  objects as unchanging), so rebuild everything that includes it and run
  `make compare` - `actor_anim.h`'s two tables became `const` this way
  with no code change. Don't retype the consumers' own local `extern`s
  as part of a data conversion for the same reason.
- **Struct padding.** agbcc pads every struct to a multiple of 4 bytes,
  so an array of small structs (say, three `u8`s) is not the ROM's
  layout. Use a plain array for such records, or check `sizeof` with
  `COMPILE_TIME_ASSERT`.
- **Alignment.** A C table starts at its type's alignment. Only convert a
  table whose ROM address already has that alignment (all pointer tables
  do); `tools/report_units.py` refuses a misaligned one rather than let
  the linker insert fill.
- **Pointers into a built blob.** A table pointing into data that is
  itself built from an editable source (the GAX2 audio, a sprite sheet)
  must not hard-code an offset into it, since editing the source moves
  things. `gStaticData_0816AA20`, the 19-entry song table, points into
  the rebuilt GAX2 blob, and stays raw until `tools/gax_audio.py` exports
  per-song symbols. Offsets into still-raw blobs are fine: the raw bytes
  can't move.

### Fallback: typed directives in `.s`

The other route is to write the table in assembly: `.4byte sub_XXXX` (the
linker adds the Thumb bit here too), `.2byte`, `.byte`, in `data/data.s`
or a `data/*.s` file linked the same way. The C route has matched
byte-for-byte on every table so far, so this is only for a table C
provably can't express. Nothing uses it yet. If something does,
`tools/report_units.py` needs to learn to size and classify directive
blobs (today it only understands `.incbin` lines in `data/data.s`).

## What has been converted

The first batch (all pointer tables, all byte-exact):

| File | ROM | Contents |
|---|---|---|
| `bg_package_16b284.c` | `0x0816B284` | 1 `struct bg_package` |
| `entry_set_16b92c.c` | `0x0816B92C` | 2 `{entries, 0x100}` sets |
| `action_table_16bf20.c` | `0x0816BF20` | 42-slot player action PMF table, per-mode animation row pointers |
| `player_pmf_16c250.c` | `0x0816C250` | 2 player-controller PMF tables, an entry table and its set |
| `entry_set_16c418.c` | `0x0816C418` | an entry table and its set |
| `bg_package_16c484.c` | `0x0816C484` | 1 `struct bg_package` |
| `bg_package_16c58c.c` | `0x0816C58C` | 1 `struct bg_package` |
| `image_table_16c5a0.c` | `0x0816C5A0` | 10 `{palette, tiles}` asset pairs |
| `dispatch_table_16c6a4.c` | `0x0816C6A4` | the unified 92-slot function-pointer dispatch array |
| `actor_category_175558.c` | `0x08175558` | 7 `struct category_descriptor`, 3 `struct category_vtable` |
| `actor_pmf_17a6b8.c` | `0x0817A6B8` | 1 actor PMF table |
| `actor_state_fn_17a840.c` | `0x0817A840` | 4 state functions |
| `frame_table_17a880.c` | `0x0817A880` | 123 frame pointers |
| `actor_pmf_17c1c0.c` | `0x0817C1C0` | 1 actor PMF table |
| `actor_pmf_17c260.c` | `0x0817C260` | 3 actor PMF tables |
| `actor_state_17c3fc.c` | `0x0817C3FC` | 1 function table, 2 actor PMF tables |
| `actor_pmf_17c450.c` | `0x0817C450` | 1 actor PMF table |
| `actor_state_17c4c8.c` | `0x0817C4C8` | 1 function table, 2 actor PMF tables |
| `bg_package_17c594.c` | `0x0817C594` | 3 `struct bg_package` |
| `popup_glyphs_17cf40.c` | `0x0817CF40` | 5 glyph packages, 9 slot seeds |
| `slot_seeds_17d6c0.c` | `0x0817D6C0` | 20 slot seeds, 3 `struct bg_package` |
| `digit_glyphs_17e714.c` | `0x0817E714` | 6 glyph pointers |
| `entity_vtables_7e3bec.c` | `0x087E3BEC` | the 93 entity virtual tables |

Still raw and worth doing next: the per-level record table
`gStaticData_0816C86C` (self-referencing records, used all over the
game loop), `gStaticData_0816CD80`, and the animation tables inside
`gStaticData_08178F80`/`gStaticData_0817AA98` (`actor_anim.h`'s
`gStaticData_081796CC`/`gStaticData_0817B2A4`, not labeled yet).

## Resources (grit-style)

The plan for graphics, and other assets built from files, is the same
layout: the build turns each asset into a generated C file holding it as
a `const` array, the way grit does, and the linker script places that
file's `.rodata` like any other `src/data` object. Nothing below is built
yet.

- **Where it goes.** A generator (a grit-like tool, or a mode of
  `tools/gbagfx`) writes `build/crashbandicootxs/graphics/<dir>/<name>.c`
  and a matching `.h`, from `graphics/<dir>/<name>.png` (plus `.pal`
  and the conversion options that `graphics.mk` holds today). Compiled
  like any C file, `build/.../graphics/<dir>/<name>.o(.rodata)` goes into
  the `/* Data */` block at the asset's address, replacing the asset's
  current `.incbin` line in `data/data.s` (a new `.section` starts after
  it, as for a hand-written table). Generated sources live under
  `build/`; only the PNG and its options are checked in. Neighbouring
  assets from one directory can share one generated file to keep the
  linker script short, as long as they are contiguous in the ROM.
- **Compressed assets.** An LZ77-compressed asset stays one array of the
  compressed bytes, the equivalent of grit's `-gzl`: `const u32
  gStaticData_XXXX[] = { ... }` holding the BIOS header word and stream
  exactly as the ROM has them. The code hands that pointer to
  `LZ77UnCompVram`/`LoadTaggedAsset` unchanged. Where `data/data.s`
  currently trims the compressor's output (`.incbin "...lz", 0, 0x1E6`),
  the generator must emit exactly that many bytes, and anything the ROM
  keeps after the stream (padding, a stray byte) belongs to the next
  label, not the array. Typed element arrays (`u32` for tiles, `u16` for
  palettes) are only possible where the asset's ROM address has that
  alignment; several current labels sit at odd addresses and need `u8`.
- **Names.** The symbol stays the current label (`gStaticData_XXXX`)
  until the code gives the asset a real name, so the generator takes the
  symbol name from the asset's options rather than from the file name.
  The header declares `extern const u32 gStaticData_XXXX[];` (or `u16`,
  `u8`), and `sizeof` can't be relied on across files, so emit a
  `#define gStaticData_XXXX_Size` next to it if the code needs the
  length.
- **The report.** `tools/report_units.py` counts a `src/data` object's
  tables as built. A generated object under `build/.../graphics/` would
  need the same treatment: `parse_data()` accepts only `data/data.o` and
  `src/data/*.o` in the `/* Data */` block today, and it would group a
  generated file's blobs by its asset directory, like the `.incbin`
  assets now.
