# Graphics

## Background/tileset extraction (done)

Most of the ROM's background and tileset graphics have been identified and
split out of the raw `baserom.gba` incbins in `data/data.s`, under
`graphics/<region>/`:

- `graphics/intro/`, `graphics/tileset1/` — tile graphics (4bpp) and
  palettes for the regions scanned so far.
- `graphics/unknown/` — LZ77-compressed blocks that decompress fine but
  don't produce a coherent image as tile data (wrong width guessed, a
  non-tile asset, or genuinely something else) - kept as raw binary rather
  than forced into a misleading PNG.

### How blocks were found

GBA BIOS decompression calls are `svc`/`swi` instructions with a fixed
number: `0x12` = `LZ77UnCompVram`, `0x15` = `RLUnCompVram`, `0x13` =
`HuffUnComp`. Wrapper functions around these were found by searching
`asm/code_3.s` for the raw byte pattern (e.g. `\x12\xdf` for `svc 0x12`),
then tracing what data address each wrapper was called with.

Compressed blocks (`LZ77`, tag byte `0x10`) were located by brute-force
scanning still-untyped `baserom.gba` regions for the tag byte followed by a
structurally valid compressed stream (strict decompressor that verifies
back-references stay in bounds and the stream produces exactly the declared
decompressed size — this rejects the many incidental `0x10` bytes that
don't start a real compressed block). **This only finds LZ77 data** — see
"Sprites" below for why this approach comes up empty in large parts of the
ROM.

### GBA Mode 4 bitmaps

Full-screen 240x160 linear (non-tiled) framebuffers needed a dedicated tool,
`tools/linear_gfx.py` — `gbagfx` always arranges pixel data into 8x8 tiles,
which is the wrong layout for these.

### Picking a legible tile width

For tile graphics, the raw dimensions aren't stored anywhere explicit; a
Total Variation (TV) minimization heuristic over candidate widths (only
exact integer-tile-count divisors are safe, to keep the byte-exact
round-trip) was used to pick a width that produces a visually coherent image
rather than a "garbled" one.

## Build pipeline

`graphics.mk` + `Makefile` (`GRAPHICS_BUILDDIR`) convert `graphics/*/*.png`
+ `*.pal` + `*.bin` sources into the original compressed bytes, output to
`build/crashbandicootxs/graphics/` (mirroring the source subdirectory
layout) — `graphics/` itself only ever holds editable sources, never build
products. `tools/gbagfx` (vendored from the sibling `sa2` decompilation,
same Dimps-engine lineage) does the PNG<->4bpp/8bpp/gbapal conversion and
LZ77 compress/decompress.

## Sprites (in progress)

As of this writing, none of the ROM's sprite graphics (player, enemies,
objects) have been located and extracted. This section documents what's
been established while looking for them - it is **not** a finished picture
of the engine's rendering/actor system.

### The two largest untyped ROM regions

- `gStaticData_0817E78C` — ~3.2 MB (ROM `0x0817E78C`-`0x084A5600`)
- `gStaticData_084A5600` — ~747 KB

Together these are over half the ROM and by far the largest remaining
un-split data. A full LZ77 signature scan across both found **nothing**
(a handful of trivial 1-13 byte "matches" that are just noise). RL
(`0x30` tag) scanning likewise only produced garbage decode lengths (into
the millions of bytes - clearly false positives, not real compressed
blocks). **Conclusion: whatever lives in these two regions is not LZ77 or
RL compressed** - it's either stored raw/uncompressed, or isn't graphics at
all (e.g. level/tilemap data).

### The VRAM upload dispatcher

`sub_8001174` (ROM `0x08001174`) is a generic "decompress-or-copy asset"
helper, used for sprite tile uploads among other things. It reads a tag
byte from the *first byte* of its source and dispatches:

| tag (top nibble of byte 0) | meaning | handler |
|---|---|---|
| `0x0` | raw, uncompressed | direct DMA3 copy (source+4, skipping the 4-byte tag+size header) |
| `0x1` | LZ77 | `sub_803A950` (`svc 0x12`) |
| `0x3` | RL | `sub_803A958` (`svc 0x15`) |

This is the same tag-byte convention used by the already-identified LZ77
graphics blocks elsewhere in the ROM (tag `0x10` = `0x1` in the top nibble +
zero low nibble). **If sprite tiles are stored with tag `0x0` (raw), no
signature scan will ever find them** - this is the leading theory for why
the two large regions above didn't turn up anything.

`sub_8028A00` calls this dispatcher with a destination computed as
`0x06010000 + tile_index * 32` - i.e. a real GBA OBJ tile VRAM address
(`0x06010000` is the OBJ character base in tile modes 0-2, 32 bytes per
4bpp tile). This confirms the dispatcher is genuinely used for sprite tile
streaming, not just background graphics.

### A dynamic VRAM tile allocator exists

`sub_8028BA0` builds a **free list of 125 x 16-byte tile slots** starting at
VRAM `0x06018000` (also OBJ tile memory), via `mem_alloc` + a manually
constructed linked list (each node's `+8` field points at the next node,
16 bytes further on). This strongly suggests sprite tiles for whichever
actors are actually on screen get streamed into a shared VRAM pool
dynamically, rather than every actor having a fixed permanent VRAM
assignment - consistent with a platformer needing far more total sprite
data (player + many enemy/object types) than fits in VRAM at once.

### An actor "vtable" system

Around ROM `0x087E3BEC` onward sits a run of ~93 already-boundary-split
(but not yet named/labeled) blocks, sizes `0x20`-`0x78` bytes (always a
multiple of 8), each a sequence of `{0, function_pointer}` 8-byte pairs -
i.e. a per-actor-type table of up to 9 "virtual method" slots, some of
which can be entirely absent (`{0, 0}`) if that actor type doesn't use that
hook. The constant `0` half of every pair is always zero in every record
seen so far; its purpose isn't understood (padding, or an unused parameter
slot in a generic "call this with a stored argument" convention). These
look like a per-actor-type behavior dispatch table (message handlers /
virtual methods), not graphics data:

- Several consecutive records share the exact same first-slot pointer
  (`sub_8028A00`, the VRAM-tile-upload function above) while differing in
  other slots - consistent with many actor types sharing a common
  "upload my current sprite frame" hook but having distinct
  behavior/update functions.
- A runtime actor struct has (at least) two fields that get set to point at
  one of these tables: offset `+0x50` and offset `+0x130`. Many actor types'
  init code just points one of these fields at a **shared default table**
  (`gStaticData_087E4DF4`, 32 bytes) rather than a unique one - so the
  presence of a pointer here doesn't by itself mean "this actor is unique",
  most just fall back to shared boilerplate.
- The second pair (word index 2-3, i.e. the slot right after the always-`{0,0}`
  first pair) is consistently one of ~36 near-identical tiny "constructor"
  stubs found immediately past `0x0803B0C4` (see below) - each one just
  writes its own hardcoded vtable-record pointer into the new actor
  instance and, for some types, calls one more common init function. This
  is the per-actor-type "vtable selection" hook, not a graphics pointer.

### Extra code past the previously-known code region (now disassembled)

While tracing the actor vtables, several entries pointed at addresses
**at and past `0x0803B0C4`** - previously assumed to be the exact end of
all disassembled code (`asm/code_3.s` covered `0x0`-`0x3B0C4`) with
everything after treated as data (`gStaticData_0803B0C4`, labeled "padding/
unidentified data between assets"). It's real, valid Thumb code -
`0x0803B0C4`-`0x0803B8B0` (0x7EC = 2028 bytes, 41 functions) has since been
properly disassembled and merged into the end of `asm/code_3.s`, verified
byte-for-byte via a full `make compare`. `data/data.s`'s padding block now
correctly starts at `0x0803B8B0` instead.

Of the 41 functions, ~36 are near-identical tiny per-actor-type "reset"
stubs (one compiled separately per actor type rather than deduplicated by
the toolchain): each writes its own hardcoded vtable-record pointer into
the new actor instance's `+0x50` field and, if a flag bit is set, calls
**`mem_free(self)`** - i.e. these are a constructor/reset path that also
conditionally frees a previously-allocated per-instance override, not
anything graphics-related. The remaining handful are small standalone
helpers (an on-screen/bounds-check-looking function among them) called
from these stubs or from the vtables.

This does not affect the two already-extracted graphics blocks further
into the (still unidentified) region beyond it
(`graphics/unknown/00_0b2120.bin` at `0x080B2120` and `01_14174c.bin` at
`0x08151AC2` sit well past this code).

### Open questions / next steps

- Where the *actual* per-actor-type graphics pointer lives is still
  unknown. The two struct fields traced so far (`+0x50`, `+0x130`) are
  behavior vtables, not graphics. The real sprite data pointer is likely
  set from a separate "actor spawn descriptor" table (mapping a type ID to
  `{vtable pointer, graphics pointer, stats, ...}`) that hasn't been located
  yet.
- Given the raw-tag (`0x0`) upload path exists and signature scanning can't
  find uncompressed data, the fastest path to *locating* sprite pixels may
  be empirical: render chunks of the two large untyped regions as raw
  (uncompressed) 4bpp tile data at various candidate widths and look for a
  recognizable character, the same way the original background/tileset
  scan worked - rather than continuing to trace the actor system function
  by function.
