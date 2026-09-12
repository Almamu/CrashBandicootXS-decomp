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
  than forced into a misleading PNG. Two of these (`00_0b2120`,
  `01_14174c`) are actually directories of named, per-record editable
  PNGs (a real sprite sheet, not raw binary) - see "Splitting the two
  giant sprite sheets" below for why and how the build reassembles them.

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

Around ROM `0x087E3BEC` onward sits a run of 93 already-boundary-split
(but not yet named/labeled) blocks, sizes `0x20`-`0x78` bytes (always a
multiple of 8). Each is an array of 8-byte `{0, function_pointer}` pairs
(4-9 pairs depending on size) - a per-actor-type table of "virtual method"
slots, where a shorter record simply doesn't implement the higher-numbered
slots. **Pair index 0 is `{0, 0}` in every single record seen** (an
always-unimplemented slot 0); real behavior starts at pair index 1. These
are a per-actor-type message/behavior dispatch table, not graphics data:

- Only **3 of the 93 records** (`gStaticData_087E4D1C`, `_087E4D64`,
  `_087E4DAC`) use `sub_8028A00` (the VRAM-tile-upload function above) -
  confirmed by searching the whole ROM for that function's address as a raw
  pointer, so this is exhaustive, not a sample. All three share every pair
  *except* pair 1 (their constructor, see below) - i.e. they're the same
  "renderable sprite" base chassis with different construction. Of the
  three, `_087E4DAC`'s constructor (`sub_8028B7C`) is confirmed to belong to
  a **text-rendering actor** (see next section) - the other two
  (`_087E4D1C`, `_087E4D64`) are still unidentified and are the most
  promising remaining candidates for "an actual sprite", including
  possibly the player.
- A runtime actor struct has (at least) two fields that get pointed at one
  of these tables: offset `+0x50` and offset `+0x130`. Many actor types'
  init code just points one of these fields at a **shared default table**
  (`gStaticData_087E4DF4`, 32 bytes, only 4 pairs - no render slot at all)
  rather than a unique one, so most actors are *not* part of the
  renderable-sprite family above; they're presumably non-visual logic
  objects (triggers, timers, etc).
- Pair 1 (the slot right after the always-`{0,0}` pair 0) is the
  "constructor": one of the near-identical tiny stubs found past
  `0x0803B0C4` and `0x0803AFF0` (see below) - it writes its own hardcoded
  vtable-record pointer into the new actor instance and, if a flag bit is
  set, conditionally `mem_free`s the instance. This is a per-actor-type
  "which vtable do I use" hook, not a graphics pointer.
- Checked whether a separate "type ID -> vtable record" lookup table exists
  anywhere in the ROM (scanned for 4+ consecutive pointers all landing
  inside the 93-record region) - **none found**. Actor construction is not
  driven by a simple integer type ID indexing a table; whatever decides
  "spawn an actor using record X" must embed record X's address (or its
  constructor's address) directly, most likely from level/spawn data that
  hasn't been identified yet (see "Open questions").

### The render-vtable family is a HUD text/counter system, not player sprites

All 3 records that use `sub_8028A00` are now identified, and **none of them
is a game-world sprite** - they're all part of an on-screen text/counter
HUD widget:

- `_087E4DAC`'s constructor (`sub_8028B7C`) leads to a pure text-measurement
  routine (`sub_8028994`): walks a null-terminated string byte by byte,
  special-casing `'\n'`/`' '`, looking up per-character width from tables
  hung off the actor - classic text layout, no icon/number.
- `_087E4D64` and `_087E4D1C` are set up by two near-identical functions
  (`sub_802866C`, `sub_8028734`) that **both** start by measuring a string
  via the same low-level routine (`sub_803A94C`, called with a
  `0x05000002` constant - `0x05000000` is GBA Palette RAM, so this looks
  like a palette-aware text draw), then overwrite the actor's vtable to
  `_087E4D64`/`_087E4D1C` and set a graphics-package pointer at a struct
  offset that varies by variant:
  - `sub_802866C` -> `_087E4D64`: sets `self+0x128 = gStaticData_085A4E70`
    (an *already-extracted* graphics block, `graphics/intro/00_5a4e70_tiles.png`
    - 5056 bytes decompressed, only 16px wide).
  - `sub_8028734` -> `_087E4D1C`: sets `self+0x110 = gStaticData_085A551C`
    (also already extracted, `graphics/intro/00_5a551c_tiles.png` - 9600
    bytes decompressed) and flips a bit in a `+3` flags byte
    (`& 0x3F | 0x40`), which is presumably what tells the shared rendering
    code which struct offset to treat as the graphics pointer for this
    variant - i.e. this region of the struct (`0x108`-`0x130`ish) is a
    small tagged union whose layout depends on that flag, not a fixed
    layout across all render-capable actors.

Put together: this whole family (all 3 records, the only ones that ever
touch `sub_8028A00`) is a HUD element that measures a string then renders
an icon/number combo (think a lives-or-fruit counter: icon + digits) or
plain text next to it. Both graphics pointers found this way point at
small, already-extracted icon-sized assets from the very first extraction
pass, not anything sprite-sheet sized.

**This rules out the vtable/`sub_8028A00` path as the mechanism for
regular game-world sprites (player, enemies, objects).** Whatever renders
those must go through a different function entirely - `sub_8028A00`'s
callers are exhaustively these 3 HUD records and nothing else (confirmed
via full-ROM pointer scan for `sub_8028A00`'s address).

The single most shared function across all 93 records, for reference, is
`sub_802A88C` (used by 40 of the 93) - but it contains no
VRAM/OAM/DMA/`mem_alloc` references at all, so it reads as a generic
per-frame update (physics/timer-style), not a renderer. No other function
comes close to being used broadly enough to be "the" sprite-render hook
for regular actors, which suggests player/enemy rendering may not go
through this vtable system in the same way HUD elements do.

### Extra code recovered from two spots that weren't actually data

While tracing the actor vtables, two separate pockets of genuine Thumb
code turned up in places that weren't previously recognized as code:

1. **`0x0803B0C4`-`0x0803B8B0`** (2028 bytes, 41 functions) - previously
   assumed to be the exact end of all disassembled code (`asm/code_3.s`
   covered `0x0`-`0x3B0C4`) with everything after treated as data
   (`gStaticData_0803B0C4`, labeled "padding/unidentified data between
   assets"). `data/data.s`'s padding block now correctly starts at
   `0x0803B8B0` instead.
2. **`0x0803AFF0`-`0x0803B058`** (104 bytes, 2 functions) - this one was
   *inside* the already-disassembled `asm/code_3.s`, but the original
   disassembly gave up on it and dumped it as a raw `.byte` blob under a
   bare `_0803AFF0:` label rather than recognizing it as code. Worth
   keeping in mind: **there are ~179 other `.byte` blobs remaining in
   `asm/code_3.s`** - most are probably legitimate non-code data (literal
   pools, jump tables) but this is a confirmed instance of one actually
   being unrecognized code, so it's not a safe assumption that all the
   others are data without checking.

Both were disassembled with a small purpose-built converter (resolves
branch/literal-pool targets against the linked ELF's full symbol table,
so it correctly pulls in C-compiled names like `mem_free`) and merged in,
verified byte-for-byte via a full `make compare` after each.

Of the ~43 functions recovered across both spots, the large majority are
near-identical tiny per-actor-type "reset" stubs (one compiled separately
per actor type rather than deduplicated by the toolchain): each writes its
own hardcoded vtable-record pointer into the new actor instance's `+0x50`
or `+0x130` field and, if a flag bit is set, calls **`mem_free(self)`** -
i.e. a constructor/reset path that also conditionally frees a previously-
allocated per-instance override, not anything graphics-related. One
constructor (`sub_803AFF0`) oddly writes two *different* record pointers
to the same field back-to-back (the second overwrites the first) - reads
as a dead store; not yet understood why the compiled code has it. The
remaining handful are small standalone helpers (an on-screen/bounds-check-
looking function among them) called from these stubs or from the vtables.

This does not affect the two already-extracted graphics blocks further
into the (still unidentified) region beyond it
(`graphics/unknown/00_0b2120.bin` at `0x080B2120` and `01_14174c.bin` at
`0x08151AC2` sit well past this code).

### Found the real sprite-loading path (not through the HUD vtable system)

Traced from the other direction as planned: `AgbMain` (`src/main.c`) calls
`sub_8026EEC`, which contains the game's true main loop (an unconditional
`b` back to itself, calling `sub_80225A0` every iteration - this never
returns during normal play, which is why `AgbMain`'s post-loop cleanup
code is dead in practice). `sub_80225A0` has its own inner loop that reads
level data and, per iteration, calls `sub_80354E0` - and **that** is where
real sprite/tile loading happens:

- `sub_80354E0` DMAs three **16-color palettes** directly into **OBJ
  palette RAM banks 13-15** (`0x050003A0`-`0x050003FF`) from
  `gStaticData_0817D034`, `_0817D054`, `_0817D074` (32/32/112 bytes -
  uncompressed, no tag+size header, straight RGB555 arrays). **The first
  two are a clear match for Crash's color scheme** - transparent green at
  index 0, then a run of oranges/tans/reds/browns (`rgb(248,168,64)`,
  `rgb(248,80,56)`, `rgb(216,32,24)`, etc.) exactly like his fur/shorts.
  The third bank is black/white and is presumably a UI or flash-effect
  palette rather than part of his normal look.
- It then calls `sub_80355E0` (loads a background onto BG2 via the same
  tag+size `sub_8001174` dispatcher, from a package struct
  `gStaticData_0817D0E4` with fields `{width, height, palette_ptr,
  tile_ptr, tilemap_ptr}` at offsets `0, 4, 8, 0xC, 0x10`) and then
  `sub_8035684`.
- `sub_8035684` is the real **OBJ sprite tile loader**: it walks an array
  of 4 pointers (`gUnknown_030008BC`, confirmed 16 bytes = 4 pointers via
  the linked ELF's symbol table) to package structs of that same
  `{w, h, palette_ptr, tile_ptr, remap_ptr}` shape. For each of the 4: DMA
  the palette into sequential OBJ palette banks (`0x05000200 + 0x20*i`),
  decompress the tile sheet into a temp buffer, decompress a remap table,
  then for each entry in the remap table, copy exactly one 32-byte tile
  from the decompressed sheet (`remap[i] & 0xFF`, `* 32` offset) into the
  **next sequential OBJ VRAM slot starting at `0x06010000`**. I.e. it
  compacts a handful of actually-used tiles out of a larger sheet into a
  tight run of live sprite VRAM - a real "load this frame/animation's
  tiles" routine, structurally nothing like the HUD vtable system.

This is almost certainly the real player/enemy/object sprite loading
mechanism. What's still missing: **where `gUnknown_030008BC`'s 4 entries
get populated**. It's never written anywhere else in the disassembled
code (checked exhaustively by text search) - `sub_8035684` is the only
place that even reads it. Likely explanations, in rough order of
likelihood: it's filled in from a still-undisassembled code pocket (see
the two we already found - there may be more `.byte`-dumped fragments
among the ~179 remaining); it's filled from level data read earlier in
`sub_80225A0`'s loop (the `sub_80354BC`/`sub_8035E14`/`sub_8036154`
"stream reader" functions glimpsed there haven't been traced yet); or it's
written through a raw computed address rather than the symbol textually
(harder to grep for).

**Next step:** find what writes `gUnknown_030008BC` (and its 4 individual
struct-pointer values) - that will either directly hand us a ROM address
holding a real animation-frame tile sheet, or point at the level-data
parser that ultimately supplies one.

Progress on that, now conclusive enough to stop and report rather than
keep guessing: this has been checked about as thoroughly as static
analysis allows, and the write site still can't be found.

- Ruled out `sub_8035E14` (called right alongside `sub_80354E0` in the
  main loop) - it's player input/collision/SFX handling (calls
  `sub_8001854`, the confirmed `PlaySfx` function, with real SFX IDs like
  `0x49`/`0x46` gated on input bitflags), not graphics setup.
- Traced `sub_80354BC`/`sub_8034CEC`/`sub_8034E2C` (the other branch of the
  main loop's state machine) - these turned out to be level-transition
  fade effects and a level-title-card display, not sprite-package setup
  either.
- Checked the IWRAM linker symbol map (`sym_iwram.txt`) - confirms
  `gUnknown_030008BC` is a genuine standalone 16-byte variable, and that
  the whole IWRAM/EWRAM region is linked `(NOLOAD)` - i.e. **there is no
  ROM-side initializer copied in at boot**; every byte of it must be
  written by executed code, not preset data.
- Searched the *entire 8MB ROM* for the raw 4-byte value `0x030008BC` -
  **exactly one hit**: the literal pool entry inside `sub_8035684` itself
  (the reader). No other instruction anywhere in the ROM - disassembled
  or not - loads this address as a constant.
- Checked the two nearby variables that do get directly referenced
  elsewhere (`gUnknown_030008B4`, `_030008B8`, used by an unrelated
  screen-shake/camera function) for any write loop that might overflow
  into `_BC` - both are pure reads there, not part of a sequential write.

Conclusion: whatever writes this array does so via a computed address
(arithmetic from some other base) rather than a direct literal - which is
invisible to literal/text search - or the write is inside a still-missing
code fragment small enough, or accessed indirectly enough, that this
pass didn't surface it.

### The `.byte` blob sweep (done)

Did the systematic pass anyway, since it's independently valuable for the
decompilation regardless of whether it found the writer. `code_3.s` had 38
distinct `.byte`-dumped regions (not ~179 - that figure was literal line
count, not distinct blobs; multiple `.byte` lines can belong to one blob).
Each was individually disassembled and checked for whether it was really
code or really data (clean-decoding Thumb is necessary but **not**
sufficient evidence - see below), then integrated and verified byte-exact.

**27 were genuine missed code**, mostly small leaf functions (helpers like
string length/copy/concat, vector negation-and-store pairs, simple
getters) sitting between two named functions with no `push {...,lr}`
prologue - which is exactly why the original disassembly's "does it start
with a push" heuristic skipped them. Two were more involved: a 12-case
switch/jump-table dispatcher (`sub_8008188`, using the classic agbcc
`mov pc, r0` indexed-jump pattern) and a pair of near-identical
vector-transform functions (`sub_801B6EC`/`sub_801B734`). One
(`_08039E9C`) turned out to be a **false function boundary** - it's
reached only via a mid-function `ble`, not a call, so it was relabeled as
a plain local branch target instead of a fake `sub_` function. A few
others needed local labels added at addresses that only became real
branch targets *because of* the newly-added code (e.g. `_08039CE6`,
`_08039EA4`/`_08039EAC`/`_08039EC0` - existing instructions that nothing
had ever pointed at before).

**11 were confirmed to be genuine data, correctly left alone** - this
needed real verification, not just "did it fail to decode cleanly":
- A cluster of 8 blocks (`gStaticData_0803A630` through `_0803A8C4`,
  ~720 bytes total, sitting between GAX2 audio functions) decode as
  syntactically valid Thumb *by coincidence*, but every actual use of
  them is either a `ldm`-based bulk-copy loop (`memcpy`-style, copying
  them into a runtime struct as template/default values) or pointer
  *subtraction* to compute a table index/size - never a `bl` or a branch.
  One of these blocks' bytes even decode suggestively as **ARM (32-bit)
  instructions** when read 4 bytes at a time (`E9 2D 00 60` etc. -
  `stmdb sp!, {r5,r6}`), which raises the possibility this is actually an
  ARM-mode routine (likely part of the GAX2 mixer, copied to IWRAM and
  executed there for speed) rather than a plain data table - interesting,
  but a separate investigation from this sweep.
- `_0803A628` (8 zero bytes) sits immediately before that cluster - just
  padding/a leading zero entry, not code.
- `_0803A9AC`/`gStaticData_0803A9AD` (36 bytes) - decisive proof this is
  data: `gStaticData_0803A9AD` starts at an **odd address**
  (`0x0803A9AD`), which is structurally impossible for real Thumb code
  (2-byte alignment is mandatory). It only *looks* like plausible
  mnemonics when misread as code.

Verified after every single change (not just at the end) via a full clean
`make compare`, including rebuilding `gbagfx` from scratch each time - all
27 integrations are byte-exact. `gUnknown_030008BC` is still referenced
nowhere but its own reader even after this exhaustive pass, so the
negative result from the previous section stands: it is not written
anywhere in `code_3.s`, `crt0.s`, or the compiled C sources. Making
further progress on that specific question would need runtime tracing
(an emulator with a debugger/watchpoint), which isn't available in this
environment.

### Found and fixed: some "unclear" graphics were actually 8bpp, not 4bpp

The background-loading trace above (`sub_80355E0`) led to a graphics
package struct, `gStaticData_0817D0E4`, whose tile pointer turned out to be
`gStaticData_0862E3B0` - one of the blocks from the original "garbled
mess" pass that got reclassified as raw binary because it looked like
noise as 4bpp tile data at every width tried. Decompressing it, its
declared palette (`gStaticData_0861C224`) and its tilemap
(`gStaticData_0863183C`) together and compositing them properly produced
a perfectly clean image: **the "Crash Bandicoot XS" title/logo art**. The
reason it looked garbled before: it isn't 4bpp at all, **it's 8bpp**
(64 bytes/tile, not 32) - the original extraction pass assumed 4bpp
uniformly and never tried 8bpp.

Checked the other blocks from that same reclassification batch for the
same mistake by looking at whether their size divides evenly by 64
(8bpp tile size) as well as 32: `38_62556c`, `41_62b34c`, and `46_62d0cc`
all do, and rendering them as raw 8bpp tiles (no palette, just grayscale)
immediately showed **recognizable art** - a boat/raft silhouette, and what
looks like a legal-information/credits text screen, respectively (the
third's content wasn't identified beyond "clearly not noise"). All three
were reclassified from raw `.bin` to proper `.8bpp` sources (with a
`_8bpp_tiles.png` naming convention so the build knows to run them through
gbagfx's 8bpp path instead of 4bpp), verified byte-exact.

One gbagfx gotcha hit while doing this: **8bpp source PNGs must be
palette/indexed mode (`P`), not grayscale (`L`)** - grayscale round-tripped
wrong (fine for the existing 4bpp files, which are `L` mode, but not for
8bpp). Also, tile counts that don't evenly factor into a reasonable
rectangle still need an exact-divisor width to stay byte-exact (same rule
as 4bpp) - `38_62556c` has a prime tile count (317), so it's stored as a
1-tile-tall strip; not pretty, but correct.

### Swept the remaining raw `.bin` files for the same mistake

Checked all 72 remaining raw `.bin` blocks (every one in `graphics/unknown/`,
`graphics/tileset1/`, and the leftover ones in `graphics/intro/`) for the
same 4bpp/8bpp mixup: 25 divide evenly by 64 (8bpp tile size) and are
candidates; rendered all 25 as a contact sheet to review at once. Result:

- **`graphics/tileset1/01`-`10` (10 files) were all real 8bpp graphics** -
  once rendered with the palette sitting right next to them in ROM
  (`gStaticData_0863CF98`, already-extracted as `11_63cf98.pal`), they turned
  out to be **circular level-select map icons** - a waterfall, a ship/dock
  scene, and others, each clearly a distinct level's thumbnail. All 10
  converted to proper `_8bpp_tiles.png` sources using that real palette and
  verified byte-exact.
  - **Correction (fixed):** rendering all 10 with the single shared
    `gStaticData_0863CF98` palette was wrong for 9 of them. An array at
    `gStaticData_0816C5A0` (10 `{palette_ptr, tile_ptr}` 8-byte pairs, read
    by `sub_801DB6C`) gives each icon its own dedicated 256-color palette -
    only icon `01_637a70`'s pairing with `0863CF98` was actually correct;
    icons `02`-`10` each pair with one of the `tileset1/12`-`20` blocks
    below instead (`02`->`12`, `03`->`13`, `04`->`14`, `05`->`15`,
    `06`->`16`, `07`->`17`, `08`->`18`, `09`->`19`, `10`->`20`). Re-rendered
    with their real palettes: `02` is a jungle/waterfall scene, `06` an
    icy/snow scene, `09` a submarine in water, etc. Fixed by regenerating
    each PNG's color table from its real palette (the pixel *index* data,
    and therefore the built ROM bytes, are untouched by this - it only
    affects what the source PNG looks like when viewed/edited) and
    correcting `data/data.s`'s per-file comments; verified byte-exact.
- **`graphics/tileset1/12`-`20` (9 files, 512 bytes each) are not tile
  graphics at all - they're almost certainly 256-color palettes** that just
  happen to be the same byte size as 8 tiles' worth of 8bpp pixels (both are
  512 bytes, hence they passed the "divides evenly by 64" filter by
  coincidence). Confirmed by decoding them as RGB555 and getting plausible,
  varied colors rather than noise. **Could not convert them to `.pal`
  sources though** - `gbagfx`'s `.pal` round-trip only reconstructs bits
  0-14 of each RGB555 entry, and several entries in these files have a
  stray bit 15 set that gets lost, breaking byte-exact rebuilding. Left as
  raw `.bin` with an explanatory comment rather than forcing a lossy format.
- Similarly, 3 more small 512-byte blocks in `graphics/intro/` (`25_61bb04`,
  `26_61bd48`, `29_61bf80`) look plausibly palette-like (varied RGB555-ish
  values) but are unconfirmed and hit the same bit-15 round-trip problem -
  left as raw `.bin` with a similar comment. **Update:** all 3 are now
  confirmed (not just "plausible") - see "Swept `sub_801E578`'s package
  structs" below.
- **One more, `graphics/intro/35_61c224`, is a confirmed palette** - it's
  `gStaticData_0817D0E4`'s (the XS logo package's) own palette pointer,
  found while tracing that struct. Same bit-15 issue prevents converting it
  to `.pal`, so it stays raw `.bin` too, but the comment now states this
  with confidence instead of "unidentified".
- The remaining 2 candidates (`tileset1/55_6e2214`, `tileset1/57_6e4750`)
  were tried at several widths, both grayscale and with the real palette
  above - neither produced anything resembling a coherent image. Left
  unchanged; genuinely still unidentified.

The other 47 raw blocks don't divide evenly by 64 at all, so they can't be
straightforward 8bpp tile data - not swept further here.

### Swept `sub_801E578`'s package structs (done)

`sub_801E578` (ROM `0x0801E578`) is a generic "load a `{w, h, palette_ptr,
tile_ptr, tilemap_ptr}` graphics package" helper (the same 5-word package
shape used elsewhere in this doc, e.g. `gStaticData_0817D0E4` for the XS
logo) - found from 9 call sites, resolving to 6 distinct package structs.
All 6 already had their tile graphics extracted from earlier passes, but
several were misclassified, and none had their real palette/tilemap
identified. Decompressing each package's tile+palette+tilemap together and
compositing them gives a clean, confirmed picture in every case:

| package | tile data | content |
|---|---|---|
| `gStaticData_0816C484` | `36_61c30c` (4bpp) | sky/clouds background |
| `gStaticData_0816B284` | `37_61e5f8` (8bpp) | Crash's face in a blue badge, metallic warp-room background |
| `gStaticData_0816C58C` | `38_62556c` (8bpp) | level-select platform icon: blue gem pool, palm trees, small ruins, magenta transparent bg |
| `gStaticData_0817C594` | `39_628c50` (4bpp) | red/fiery smoke texture |
| `gStaticData_0817C5A8` | `40_62a958` (4bpp) | fire/aura glow effect: green transparent bg, orange/red/magenta outline |
| `gStaticData_0817C5BC` | `41_62b34c` (8bpp) | Uka Uka's mask |

Fixes applied (all verified byte-exact via a full clean rebuild):

- **`36_61c30c` and `40_62a958`** were left as raw `.bin` ("not clearly
  identifiable as pixel graphics") - true of a bare tileset without its
  tilemap, but both decode cleanly as 4bpp once paired with their real
  tilemap. Converted to proper `_tiles.4bpp` PNG sources (prime tile
  counts - 509 and 167 - so both stored as 1-tile-tall strips, matching
  the existing convention for `38_62556c`'s 317-tile case).
- **`37_61e5f8`** was classified as a Mode 4 (linear) bitmap - it happens
  to be exactly 240x160 like a real one, but its decompressed size
  (38400 bytes) divides evenly by 64 into exactly 600 tiles (an exact
  30x20 screen, matching its package's `w,h` with no tile reuse), and it
  needs a tilemap to make sense - genuine tiled+tilemapped 8bpp BG
  graphics, not a linear bitmap. Reconverted through `gbagfx`'s normal
  8bpp path.
- **Three 32-byte blocks miscategorized as one-tile 4bpp graphics**
  (`24_61badc`, `27_61bf30`, `28_61bf58` - the same "32 bytes divides
  evenly by the 4bpp tile size" coincidence hit earlier for `34_61c1fc`)
  are actually the 16-color palettes for `36_61c30c`, `39_628c50`, and
  `40_62a958` respectively. None have the stray-bit-15 problem, so all
  converted cleanly to `.pal` sources.
- **Two blocks miscategorized as 4bpp tile graphics** (`48_62fb24`,
  1280 bytes; `50_63053c`, 2048 bytes) are actually the tilemaps for
  `36_61c30c` and `38_62556c` (both byte counts divide evenly by 32, the
  4bpp tile size, purely by coincidence - same trap as the palettes
  above). Every tilemap entry's tile index is in-bounds for its
  tileset's real tile count, confirming this rather than assuming it.
  Converted back to raw `.bin` (tilemaps aren't tile pixel data, so they
  don't go through `gbagfx` at all - same as every other already-known
  tilemap in this file).
- **The 3 previously "plausible but unconfirmed" 256-color palette
  blocks** (`25_61bb04`, `26_61bd48`, `29_61bf80`) are now confirmed as
  `37_61e5f8`'s, `38_62556c`'s, and `41_62b34c`'s real palettes
  respectively - comments updated from "very likely"/"plausible" to
  stating the pairing directly. Still raw `.bin` (all 3 still hit the
  stray-bit-15 `.pal` round-trip problem).
- **`38_62556c` and `41_62b34c`** were already correctly 4bpp/8bpp from
  earlier passes - only their `data.s` comments were missing the real
  identity/palette/tilemap pairing, now added.

One gotcha hit while fixing `37_61e5f8`: hand-building an indexed (`P`
mode) PNG in Pillow from the working grayscale source (loading it,
`putdata`-ing the same index values into a fresh `P`-mode image, then
`putpalette`) silently broke the round-trip through `gbagfx`, even though
the pixel index values were byte-identical going in - so the fix keeps
this one file as grayscale (verified byte-exact) rather than forcing it
to match the indexed-PNG style most other 8bpp sources use. Root cause
not investigated further; worth being cautious about if this comes up
again.

### Found the real per-actor animation-frame system

This is the "actual per-frame rendering loop" the previous section's open
question was looking for - found by chasing call sites of the generic
"load OBJ graphics package" helper `sub_801E578` and the package structs it
was called with, which led away from the HUD vtable system entirely into a
separate, much more elaborate chain:

- **`gStaticData_08175558`** - a per-actor-*category* descriptor array,
  `0x34` (52-byte) stride, 7 valid entries. Selected via
  `sub_8029ED0(category, ...)`, which computes
  `gStaticData_081756C4 + category*0x34` (see next) and stores it as the
  active vtable, then calls vtable slot 0 (the constructor) passing the
  descriptor's `+0x18` field. Key fields (word offsets): `+0x10` = a raw
  16-color OBJ palette pointer (DMA'd to `0x05000200`), `+0x18` = the
  **animation table base** for this category-family
  (`gStaticData_081796CC` for categories 0-2, `gStaticData_0817B2A4` for
  3-6), `+0x1C` = a big LZ77 sprite-sheet pointer (`0x080B2120` for
  categories 0-2, `0x0814174C` for 3-6 - these are the same two giant
  blocks as `graphics/unknown/00_0b2120.bin`/`01_14174c.bin` from the very
  first extraction pass). **Note:** despite living in the same descriptor
  record, this `+0x1C` sheet is *not* the data source for the animation
  system below - see the callout at the end of this section.
- **`gStaticData_081756C4`** - the category vtable array, same `0x34`
  stride but interpreted as **13 plain function pointers** (not the
  `{0, ptr}` pair convention the HUD vtable system uses - a different,
  unrelated convention that happens to reuse the same struct-offset idea).
- **`sub_802A700`** - constructs one *part* instance: `r1` (the per-part
  descriptor) is computed at call sites as
  `gUnknown_0300147C[0] + index*0x28` (40-byte stride) - i.e. a single
  category can spawn several independently-animated parts (limbs on a
  shared body, most likely), each picking its own row out of the
  animation table.
- **The animation table** (`gStaticData_081796CC` / `_0817B2A4`), `0x28`
  (40-byte) stride records: `{index, table_A_ptr, table_B_ptr, header_byte,
  ...}`. Multiple records can share the same `table_A` (the timing/keyframe
  driver) while pointing at *different* `table_B`s (the actual pixel data)
  - seen directly in `gStaticData_081796CC` records 5-9, which all reuse
  `table_A = 0x817a2b8` with 5 distinct `table_B`s - consistent with
  several body parts animating in lockstep off one shared timing table.
- **`table_A`** - 12 bytes/entry; the signed halfword at `+2` is a
  `table_B` index. A record's full `table_A` is the keyframe sequence for
  one animation clip (e.g. record 0's 13-entry cycle:
  `[0,20,59,79,88,97,113,121,132,133,117,60,0]` - a closed loop back to 0).
- **`table_B`** - flat array of 4-byte raw ROM pointers, read by
  **`sub_803B074`** (`table_A[keyframe].halfword_at_2` indexes into it).
  Confirmed via disassembly of `sub_8028FF8`/`sub_8028F58` (the functions
  that consume the returned pointer) that each entry points at a tiny
  self-contained record: `{w_tiles, h_tiles, 0x30, 0x00}` (4 bytes) followed
  by exactly `w_tiles*h_tiles*32` bytes of standard swizzled 4bpp tile data
  (every sampled entry across both animal families is `w=8,h=8` = a 64x64
  OBJ). `sub_8028FF8` decodes the `w`/`h` bytes using the exact GBA OAM
  shape/size encoding (square/wide/tall + size class 1/2/4/8 tiles) to
  build the sprite's attribute bits - this is unambiguous confirmation
  it's genuine OBJ sprite data, not coincidence.
- The actual VRAM upload is a **queued DMA**, not an inline copy:
  `sub_8028F58` computes the byte count and calls `sub_8006B94`, which just
  appends `{dest, src, size}` into a ring buffer
  (`gUnknown_03001290`, up to 768 entries) rather than copying immediately.
  The flush happens in `sub_8006B1C`, confirmed by disassembly to write
  directly to **`0x040000D4`/`0x040000D8`** - the GBA's real DMA3
  source/destination registers - and start the transfer. **No
  reformatting happens anywhere in this path**: ROM bytes reach VRAM
  completely unmodified, so the frame-record format above is exactly what
  the hardware sees.

**The `table_B` "sliding window" discovery:** consecutive `table_B`
entries looked suspiciously close together in ROM (only ~600-880 bytes
apart) for records that each claim a 2048-byte payload (64 tiles). Checked
directly at the byte level: `payload(table_B[i])[delta:]` is **byte-for-
byte identical** to `payload(table_B[i+1])[:len]`, where `delta` is the
address gap between the two entries - not approximately similar, an exact
match, confirmed across dozens of consecutive index pairs in both animal
families. Some indices even alias the exact same address entirely (e.g.
category 0-2's `table_B[113] == table_B[20]`, bit-for-bit the same
pointer). This is a ROM-space-saving trick for pre-rendered 3D rotation
animation: since neighboring rotation angles of a spinning object share
almost all their pixels, the asset pipeline stores one long overlapped
byte stream per rotating object and lets different keyframe indices just
pick different windows (or literally the same window, when a pose
repeats) into it, instead of storing N independent full frames.

Rendering frames evenly spaced across one confirmed contiguous run (e.g.
indices 0, 12, 24, ... 95 of the categories 0-2 pool) shows a *stable*
overall composition frame to frame - green (transparent) background, a
pale rounded mass with a dark-blue crescent shape, red/orange mass lower
down - exactly what a rotating 3D-rendered object looks like under a fixed
camera (silhouette/position holds steady, surface shading drifts). Given
the palette (cream/white + red/orange + dark blue, index 0 = transparent)
matches the already-identified Uka Uka mask family from the
`sub_801E578` sweep, this is most likely a **spinning Aku Aku mask**
(Uka Uka's "good" counterpart) - not yet confirmed beyond the palette/shape
match, since no in-game screenshot or emulator run was available to check
against.

**Important correction/clarification:** this whole `table_A`/`table_B`
system reads its frame data directly from fixed ROM addresses (confirmed:
`sub_803B074` does add a RAM-buffer-base global, `gUnknown_0300137C`, but
it reads back as `0` along this path, making the add a no-op) - it is
**raw, uncompressed data sitting in ROM**, physically near but *not inside*
the two giant LZ77 sheets (`0x080B2120`/`0x0814174C`) referenced by the
category descriptor's `+0x1C` field. Those two sheets are loaded via the
ordinary tag+size dispatcher elsewhere and their actual contents/purpose
remain unidentified - don't assume they're pre-decompressed into the
buffer this animation system reads from.

Not yet resolved: the descriptor's `+0x14` field (unique per entry,
passed into `sub_8029ED0`) and the `+0x20`-`+0x30` small integers; the
animation table record's `header_byte` (copied into the runtime instance
at `+0x18` by `sub_802A700`, role not traced further).

### Identified the two giant LZ77 sheets: they're the actual sprite art

Found the reader: `sub_802928C` (per-category init, called with the
category number, stored in `gUnknown_03001380`) reads the descriptor's
`+0x1C` sheet pointer and calls `sub_802917C(sheet_ptr)`, which reads the
tag+size header, `mem_alloc`s a buffer of the declared decompressed size,
stores it in **`gUnknown_0300137C`**, and decompresses into it via
`sub_8001174` - **this is the same global `sub_803B074` adds to a
`table_B` value** (see above). So there are genuinely two different
addressing modes in play for animation-table records, both already
present in the code, and confirmed by directly decompressing
`0x080B2120` (a standard LZ77 stream, tag `0x10`, declared size 213064 -
matches `graphics/unknown/00_0b2120.bin` exactly) and checking real
records against it:

- **Category 0-2's record 0** (the "mask", `table_B = gStaticData_0817941C`)
  holds full absolute ROM addresses (`0x080Cxxxx`) and reads real
  uncompressed ROM data directly - `gUnknown_0300137C` is unset/0 for
  this path, making the add in `sub_803B074` a no-op. This is the
  overlapping/deduplicated "rotation strip" scheme described above.
- **Records 1, 2, 4** (`table_B` at `0x0817a130`, `0x081796a4`,
  `0x0817a250`) hold **small byte offsets into the decompressed
  `0x080B2120` buffer** instead (e.g. record 1: `0xfdf8, 0xfffc, 0x10200,
  ...`, a constant stride of exactly `0x204` = `4 + 4*4*32` bytes - a
  plain non-overlapping sequential frame array, no dedup trick at all).
  Decompressing `0x080B2120` in Python and rendering these offsets with
  the same category palette (`gStaticData_08178F80`) gives **completely
  clean, unambiguous art**:
  - Record 1 (`w=4,h=4`, 32x32 frames): a **rotating TNT crate** (the
    classic X-braced wooden crate), full clean rotation sequence.
  - Record 4 (`w=4,h=4`, 32x32 frames): a **rotating Nitro crate** - the
    "NITRO" text label is legible in multiple frames.
  - Record 2 (`w=8,h=8`, 64x64 frames, stride `0x804` = `4+8*8*32`, also
    no dedup): a fluffy white/gray creature face (round ears, snout -
    plausibly Crash's polar-bear companion, unconfirmed) for the first
    several frames, transitioning to what looks like a red-armored
    enemy figure with a chain/weapon prop later in the sequence (the
    frame count used to sample this one was a guess, not derived from a
    real per-record count, so the transition point may just be where
    record 2's real data ends and record 3's begins - not necessarily
    one single animated object).

This conclusively confirms `graphics/unknown/00_0b2120.bin` is genuine
**rotating pickup/hazard sprite art** (crates confirmed, a
companion/enemy likely) - not a background or something unrelated.

**The second sheet, `01_14174c.bin` (categories 3-6), decompresses and
checks out the same way** - `0x0814174C`, tag `0x10`, declared size
207124, matches the LZ77 stream exactly. Its animation table
(`gStaticData_0817B2A4`) uses the identical two addressing modes: record
0 is the absolute-ROM-address/overlap-dedup scheme (like categories 0-2's
"mask"), while records 1 and 2 are small offsets into this sheet's own
decompressed buffer (stride `0x804` = `4+8*8*32` for record 1, `0x84` =
`4+2*2*32` for record 2 - both plain non-overlapping arrays). Rendered
with this category's palette (`gStaticData_0817AAA4`):

- Record 1 (`w=8,h=8`, 64x64 frames): a rotating **mechanical/winged
  creature** - a central body with a beaked/helmeted head, wide
  outstretched wings (dotted/riveted pattern), and what look like
  wheels or turbines on some angles - reads as a small flying
  enemy/vehicle rather than a crate; exact identity unconfirmed.
- Record 2 (`w=2,h=2`, 16x16 frames, only 4 valid entries before the
  data stops making sense as this record - matches the raw `table_B`
  values going erratic after index 2): three small circular
  medallion/badge icons (red, dark maroon, and a third variant, tan
  border) plus one small icon of the winged creature above - likely UI
  icons (checkpoint/gem/difficulty badges) rather than animation
  frames.

**Records 5-9 of the categories 0-2 animation table** (`gStaticData_081796CC`,
all sharing `table_A = 0x817a2b8`) turned out not to be body parts of one
character as first guessed - each is an **independent decorated crate
variant**: rendering frame 0 of all five (all `w=4,h=4`, plain non-
overlapping `0x204`-stride arrays like the TNT/Nitro crates) shows records
5-7 as a red/dark-blue crate border with a small gray icon/figure inside,
and records 8-9 as a tan/pale crate border with a different red-orange
icon inside - reads as different "special crate" types (e.g. checkpoint,
extra-life, ? crate - exact icon meanings unconfirmed) that all happen to
reuse the same generic rotation timing table, which is exactly why they
share one `table_A`.

Categories 0-2's record 3 (`table_B = 0x0817a5dc`, mostly `0x204`-stride
like the crates) is another `w=4,h=4` decorated-drum/barrel-looking
object (a red/tan spiral or coil pattern on a cylindrical shape) - yet
another distinct pickup/hazard prop, not yet identified beyond "clearly
not noise, clearly a barrel/drum-shaped object". Categories 3-6's own
records 3-4 follow the same pattern (an 8x8 badge icon and a 32x32 prop
resembling part of the winged creature/vehicle from record 1) - records
5-9 in that table are identical repeats of record 4, i.e. this
particular table's genuinely-used range is only records 0-4, not the
full 10 slots read for categories 0-2's table.

### Splitting the two giant sprite sheets (done, superseded)

**This section's file layout (one grid PNG + `.frames` sidecar per
record) no longer exists** - see "Re-splitting into one file per entity"
and "One PNG per frame, no sidecar" further down for the current layout
(one folder per entity, one PNG per frame, no sidecar). Kept as-is since
the investigation described here (how the record boundaries were found)
is what made both later passes possible.

Both sheets are now split into per-record source files instead of one
opaque flat `.bin`. Each decompressed sheet turned out to be, with zero
gaps and zero leftover bytes, a back-to-back sequence of
`{w_tiles, h_tiles, pad, 0}` + `w*h*32`-byte frame records from start to
finish - confirmed by a greedy parser that walks the whole buffer
re-parsing a frame header at every position it lands on and never fails
to find a valid one anywhere in either 213064 or 207124 bytes. That made
the record boundaries found while identifying content (the `table_B`
start offsets for each record) exact and exhaustive: sorting all known
record-start offsets and using each as a cut point accounts for every
single byte in both sheets.

- `graphics/unknown/00_0b2120/` (categories 0-2's sheet, in ROM order):
  9 files at this point - 5 decorated crate variants, a Nitro crate, a
  TNT crate, a barrel, and one 92-frame mixed-content pool (see
  "Cataloguing the `unidentified.png` pools" below - **this initial
  9-way split was later replaced entirely**, see "Re-splitting into one
  file per entity" further down for the current, final layout).
- `graphics/unknown/01_14174c/` (categories 3-6's sheet, in ROM order):
  4 files at this point - badge icons, a winged creature, one small
  mixed pool, one large 176-frame mixed pool (see below - **also later
  replaced entirely**, see "Re-splitting into one file per entity").
- Categories 0-2/3-6's record 0 (the mask-like object using the separate
  absolute-ROM-address/overlap-dedup scheme, not this buffer) is **not**
  part of either split - it was never inside these LZ77 streams to begin
  with, see the addressing-mode callout above.

**These are real, viewable/editable indexed PNGs, not raw passthrough
binaries** - each frame's actual pixels (after its 4-byte header) are
standard swizzled 4bpp tile data, exactly like every other 4bpp asset in
this project, just interleaved with non-pixel header bytes and (within a
single record) sometimes varying frame sizes, which is what stops plain
`gbagfx` from handling them directly. A new tool, `tools/framed_gfx.py`
(same idea as the existing `linear_gfx.py` for Mode 4 bitmaps - bypass
`gbagfx`, do the pixel conversion directly), strips the per-frame headers
and lays every frame out on a grid (cells sized to the record's largest
frame, each frame's real pixels anchored top-left, unused cell space
filled with palette index 0 - already the transparent/background index
everywhere in this data) to produce one indexed PNG per record, plus a
`.frames` sidecar text file (grid column count, then each frame's
`w_tiles h_tiles` in original order - the only information a flat PNG
can't self-describe, since frame sizes vary within a record). The
per-frame pad bytes are always `{0x10, 0x00}` in both sheets (verified
across every single frame via the same greedy parser), so the tool
hardcodes them rather than storing them per frame.

**Why directories of PNGs instead of separate top-level assets:** the
original ROM data is one single LZ77-compressed stream per sheet, so the
records can't be compressed independently and still round-trip byte-
exact - compression exploits cross-record byte patterns. `graphics.mk`
gained a generic `%.bin: %.png %.frames` rule (runs `framed_gfx.py`) plus
two explicit rules that `cat` each directory's *built* per-record `.bin`
fragments back together in filename order (numeric prefixes preserve
original ROM order, computed via `$(patsubst ...,$(sort $(wildcard
graphics/unknown/<name>/*.png)))` so new/removed records stay in sync
automatically) before the existing generic `.bin.lz` compression rule
runs - `Makefile`'s `GRAPHICS_BUILT` lists the two composite `.bin.lz`
targets explicitly since the flat `graphics/*/*.bin`/`*.png` wildcards
don't reach 3 directory levels deep. Verified byte-exact via a full clean
`make compare` (including rebuilding `gbagfx`) after the change.

Not done: further breaking down the two `unidentified*.png` catch-all
fragments, and figuring out frame *counts* actually used per object (vs.
how many exist physically - some records, e.g. category 0-2's record 2,
may bundle more than one logical animation).

### Cataloguing the `unidentified.png` pools

Viewing the split PNGs whole (they're small enough) rather than one
record at a time turns up clearly recognizable content in both -
"unidentified" only meant "not yet looked at", not "noise".

**Superseded by the re-split below** - `08_unidentified.png` no longer
exists as a single file; every object identified in this section now has
its own file (`16_guard_barrier.png`, `17_wumpa_fruit.png`,
`19_checkpoint_text.png`, etc.) - but the identification work described
here is what made that re-split possible, so it's kept as-is.

**`graphics/unknown/00_0b2120/08_unidentified.png`** (92 frames, the byte
range covered by categories 0-2's record 2) is genuinely several
different objects sharing one physical frame pool - and unlike the rest
of this section, this one **is** code-verified: `gStaticData_081796CC`
(the animation table) turned out to have far more than the 10 records
originally catalogued - it runs to at least 41 (many are duplicate
aliases of the same handful of `table_A`/`table_B` pairs, and indices
32-34 are a zeroed gap). Cross-referencing each *distinct* record's own
`table_B[0]` offset against this pool's byte range, then rendering that
record's own frames (not just eyeballing the pool as a whole), gives
actual per-object confirmation:
- **Record 2** (`table_A 0x817968C`, `table_B 0x81796A4`) - the small
  gray/white rounded creature, a 10-frame idle/rotation set. Identity
  beyond shape/color still unconfirmed.
- **Record 11** (`table_A 0x8179D34`, `table_B 0x8179D40`) - a clean
  rotation set that renders as a real **Wumpa Fruit**, confirming the
  visual ID directly at the record level, not just by proximity.
- **Record 22** (`table_A 0x8179D78`, `table_B 0x8179D90`) - a red/orange
  spiky-based object with a white/gray cap that grows then peels away
  over its 11 frames - reads as some creature or item emerging from
  under a shell/cover. Not confidently identified beyond that.
- **Record 23** (`table_A 0x8179E24`, `table_B 0x8179E3C`) - not one
  guard repeated, but **two** guards facing each other, each holding one
  end of a shared horizontal chain/spear prop between them - a barrier
  formed by a pair of guards, confirmed by rendering record 23's own
  frame 0 cleanly.
- **Record 40** (`table_A 0x817A6A0`, `table_B 0x817A6AC`) - a `w=8,h=4`
  frame that renders as the literal text **"CHECK POINT"**, confirmed
  crystal-clear at native resolution. This sits right near the end of
  the pool's byte range.

So the "guard shattering into debris" and the loose fragments seen when
scanning the whole pool visually are real (there's clearly debris-style
content between the guard-barrier frames and the checkpoint text), but
which specific record(s) they belong to hasn't been pinned down the same
rigorous way - only records 2, 11, 22, 23, and 40 have been matched to
actual animation-table entries so far. The overall reading still stands
and is now partially code-confirmed rather than purely visual: a
two-guard barrier (record 23) guards a checkpoint, and defeating it
reveals "CHECK POINT" (record 40), with a Wumpa Fruit (record 11), a
small creature (record 2), and an unidentified emerging object (record
22) sharing the same underlying pixel pool incidentally rather than as
part of that sequence.

**Superseded by the re-split below** - `03_unidentified.png` no longer
exists as a single file either; each object below now has its own file
(`19_treasure_chest.png`, `13_parachute_crate.png`, `17_clock.png`, the
3 balloon files, etc.).

**`graphics/unknown/01_14174c/03_unidentified.png`** (176 frames,
categories 3-6's record 3) got the same code-level treatment:
`gStaticData_0817B2A4` also runs far past the 3 records originally
catalogued - valid entries go up to at least record 46 (again with many
duplicate aliases; records 49+ read back the same garbage offset and are
past the real end). Rendering each distinct record's own frame 0 gives a
rich, individually-confirmed catalogue:

| record | table_A / table_B | content |
|---|---|---|
| 12 | `0x817bdf0` / `0x817be20` | a **treasure chest** opening (matches the "chest" guess from the whole-pool scan, now tied to a specific record) |
| 19 | `0x817bedc` / `0x817bef4` | a wooden crate with a **"?" mark** - a mystery/question-mark crate |
| 24 | `0x817bedc` / `0x817bf18` | a wooden crate with a **"1"** |
| 25 | `0x817bedc` / `0x817bf3c` | a wooden crate with a **"2"** |
| 26 | `0x817bedc` / `0x817bf60` | a wooden crate with a **"3"**-like digit |
| 27 | `0x817bf84` / `0x817bf9c` | the **parachute-dropped crate** (confirmed) |
| 29 | `0x817c070` / `0x817c088` | a round **clock face** with visible hands |
| 31 | `0x817c100` / `0x817c118` | the crescent/arch-shaped striped object (still just a shape match, not identified beyond that - sits right next to the clock, so plausibly a related prop) |
| 40, 41, 42 | `0x817c0a0` / `0x817c0b8`, `0x817c0e8`, `0x817c0d0` | three **balloon color variants** - plain red/yellow, orange/red/blue, and a yellow/blue one with a cross/plus mark on it |
| 46 | `0x817c1a8` / `0x817c1b4` | **"CHECK POINT"** text again, confirmed crystal-clear, independently of the first pool's copy |

(Records 3, 10, 13, 14, 28, 38, 44, 45 are smaller badge/icon/fragment
frames, consistent with the debris/sparkle content seen scanning the pool
as a whole, but weren't individually identified beyond "a small icon or
debris chunk".)

This upgrades the whole-pool visual scan from a guess to a confirmed
catalogue: numbered crates (including a "?" mystery crate - a real,
distinct Crash Bandicoot gameplay object), a parachute crate, a treasure
chest, a clock, and 3 balloon variants really are separate animation-
table entries sharing this one physical byte pool, each independently
renderable from its own record. The "container breaks open into CHECK
POINT" narrative from the whole-pool scan is still not proven as a
*single connected sequence* (no code trace linking e.g. record 27's
parachute crate to record 46's text) - what's now confirmed is that both
the container objects and the checkpoint text are real, distinct,
individually-addressable records in this table, which was the main open
question.

### Bundling everything into `entities.json` (done)

All of the structural data recovered above - the 7 category descriptors
and both animation tables' full record lists (`gStaticData_081796CC`:
41 slots, `gStaticData_0817B2A4`: 47 slots), keyframe sequences, and a
resolved reference to the exact split PNG frame each keyframe uses - is
now bundled into two generated files:

- `graphics/unknown/00_0b2120/entities.json`
- `graphics/unknown/01_14174c/entities.json`

Generated by `tools/dump_entities.py` (re-run any time - it only reads
`baserom.gba` and whatever entity folders/frame PNGs currently exist
under `graphics/unknown/*/`, never writes to them). Purpose: once the
actor/entity C code behind this system is reversed, this is the *data*
half ready to go - real ROM addresses, per-record keyframe->frame
mappings, and a direct `asset_file` reference straight to each keyframe's
own frame PNG - so whoever writes the matching structs doesn't have to
re-derive any of this from raw hex again.

Schema per file: `categories` (this sheet's 2-4 category descriptors,
every `gStaticData_08175558` field, named where its role is known -
`type_00`, `family_const_04/08`, `conditional_ptr_0C`, `palette`,
`sub_effect_table_14`, `anim_table_base_18`, `sprite_sheet_1C`,
`threshold_24`, `flag_2C` - and left as `unknown_XX` otherwise, see
"Found the real per-actor animation-frame system" above for what's known
about each); `records` (one entry per animation-table slot, `valid:
false` for empty/garbage slots) each with `table_A_address`,
`table_B_address`, `header_byte`, `addressing_mode` (`pool_offset` vs the
mask-style `absolute_rom`), `duplicate_of_slot` (many slots are aliases
of an earlier slot's exact same `table_A`/`table_B` pair), `keyframes`
(every table_A entry actually used, its raw 12 bytes for the fields still
unexplained, and which `table_B` index it selects), and `frames` (each
used `table_B` index resolved to either `{asset_file, w_tiles, h_tiles}`
- `asset_file` pointing straight at that one frame's own PNG - for
`pool_offset` records, or `{rom_address, w_tiles, h_tiles}` for
`absolute_rom` ones). Confirmed content from this and the
previous two sections is carried over as `label`/`identified` on the
matching record.

Two heuristics worth knowing if this is regenerated/extended:
- A record's real `table_B` length (needed to know where its keyframes
  stop being legitimate indices) is found by walking forward from
  `table_B[0]` and re-validating an actual `{w,h,0x10,0x00}` frame header
  at each step against the freshly-LZ77-decompressed sheet (not just
  "is this a small number", which produced false-positive resolutions
  during development - e.g. resolving into the middle of an unrelated
  frame). Every current record resolves cleanly with this check (no
  `note` fields on any `frames` entry).
- `keyframes` parsing is capped at 256 entries per record purely as a
  safety net (`keyframes_truncated_at_cap` flags it if hit) - real
  sequences observed run up to ~200 (e.g. the Nitro crate has 113,
  Wumpa Fruit 104), and a long run of small in-bounds indices can't be
  distinguished from "the table just ends here" by index-bounds alone,
  so this is a pragmatic cutoff, not a proof the true sequence never
  runs longer.

### Re-splitting into one file per entity (done)

The original split (above) was done by byte-range before the full extent
of the animation tables was known, so several files bundled multiple
unrelated objects together - most obviously the two `*unidentified*.png`
catch-all pools, but also `06_tnt_crate.png` (which turned out to also
contain 4 other small objects after it in ROM) and `07_barrel.png`
(2 more). That's the opposite of what you want if you're trying to
tweak one sprite or feed one struct's worth of data to reversed code
without wading through everything else sharing its old byte-range file.

Fixed by `tools/split_entities.py`: rather than anchoring on the handful
of records identified so far, it enumerates **every** valid slot in both
animation tables, dedupes by `table_B` address (the thing that actually
determines a distinct piece of frame data - multiple slots/keyframe-
timings can legitimately share one `table_B`, e.g. the 3 balloon colors
each have their own frames but reuse one shared timing table), and sorts
the results by each entity's starting offset in the decompressed sheet.
This gives a complete, verified-gapless, non-overlapping partition of
each sheet - 20 distinct entities for categories 0-2, 23 for categories
3-6 - which is a lot more granular than the 9/4 files from the first
pass once every duplicate/shared-frame-set slot is accounted for.

Each entity gets its own numbered **folder** (`NN_<name>/`), named from
the identification work in the sections above where available
(`17_wumpa_fruit/`, `19_checkpoint_text/`, `13_parachute_crate/`, ...)
and `record_slotNN/` (using the lowest animation-table slot that
references it) where not yet identified - still individually editable
and traceable back to a specific slot even without a friendly name.

### One PNG per frame, no sidecar (done)

Taken a step further: each entity folder holds one indexed PNG per
*frame* (`00.png`, `01.png`, ...), sized to exactly that frame's own
`w_tiles*8 x h_tiles*8` pixels - no grid, no padding cells, no separate
`.frames` metadata file, since a frame's tile dimensions are just its own
PNG size / 8. Editing or replacing one frame of one animation now means
opening exactly one small image, nothing else.

`tools/framed_gfx.py` was reworked around this: `to-frames IN.bin OUT_DIR
PAL.bin` explodes a raw entity into `OUT_DIR/00.png, 01.png, ...`, and
`to-bin-folder IN_DIR OUT.bin` reads a folder's PNGs back in filename
order, infers each one's `w_tiles`/`h_tiles` from its own dimensions,
and reassembles the `{w,h,0x10,0x00}`-header+tile-data stream exactly as
originally found. `tools/split_entities.py` calls the former once per
entity; the numeric `NN_` folder prefixes and `NN.png` frame filenames
are both load-bearing - sort order is the only thing that reconstructs
original ROM byte order, for entities within a sheet and frames within
an entity alike.

`graphics.mk` gained explicit (non-pattern) rules generated once per
entity folder found under each sheet directory (via `$(foreach)`+`$(eval)`
at parse time), each depending on `$(wildcard <entity_dir>/*.png)` - so
adding, removing, or editing a frame PNG is tracked like any other
prerequisite, not just "the folder's mtime changed" (which wouldn't catch
in-place edits to an existing frame). The per-sheet concatenation rule
now sums each entity's own built `.bin` in entity-folder-name order
instead of concatenating raw PNG-derived bytes directly.

Order of operations when regenerating: `tools/split_entities.py` first
(writes each entity's per-frame PNGs, deleting whatever was there
before), then `tools/dump_entities.py` (reads whatever entity folders and
frame PNGs exist to resolve keyframes to `asset_file`, so it always
reflects the current split - `asset_file` now points straight at one
frame's own PNG, no `frame_index` field needed anymore either). Verified
byte-exact via a full clean `make compare` after each step.

Not-yet-identified entities are still real, individually-editable folders
(e.g. `03_record_slot28/`, `18_record_slot10/`) - "not identified" no
longer means "bundled in with everything else nearby", just "nobody's
looked at this specific folder yet".

### Open questions / next steps

- The `0x087E3BEC`-onward vtable system, its constructors, and
  `sub_8028A00` are now a dead end for finding player/enemy sprites - fully
  traced and conclusively shown to be a HUD text/counter system instead.
  Don't re-investigate this path without new evidence.
- There is no simple "type ID -> descriptor" lookup table anywhere in the
  ROM for this vtable system (checked exhaustively) - actor construction
  embeds a record's address directly in code (as seen in `sub_802866C`/
  `sub_8028734`) rather than going through a numeric index. Whatever the
  equivalent mechanism is for game-world sprites hasn't been located.
- **The per-frame rendering loop has been found** (see the new section
  above) - it's the category-descriptor -> vtable -> animation-table ->
  `table_A`/`table_B` chain, not `sub_8028A00`. What's still open there:
  identifying which game object(s) categories 0-2 and 3-6 actually are
  (working theory: a spinning Aku Aku mask for 0-2, unconfirmed), and the
  still-undecoded descriptor/record fields listed at the end of that
  section.
- The two giant LZ77 sheets are now identified and split into per-record,
  editable PNG sources under `graphics/unknown/00_0b2120/`, `01_14174c/`
  (a TNT crate, a Nitro crate, 5 decorated crate variants, a winged
  creature, badge icons, a barrel, all confirmed; two large
  `unidentified.png` catch-all pools remain uncatalogued). See "Identified
  the two giant LZ77 sheets" / "Splitting the two giant sprite sheets"
  above. Not done: cataloguing the two `unidentified.png` pools further.
- Given the raw-tag (`0x0`) upload path exists (see the VRAM upload
  dispatcher above) and signature scanning can't find uncompressed data,
  the fastest path to *locating* more sprite pixels may still be empirical:
  render chunks of the two large untyped regions as raw (uncompressed)
  4bpp tile data at various candidate widths and look for a recognizable
  character, the same way the original background/tileset scan worked -
  independent of and complementary to the code-tracing approach above.
