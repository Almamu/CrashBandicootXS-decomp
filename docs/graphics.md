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
  left as raw `.bin` with a similar comment.
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
- Next real lead: find the *actual* per-frame rendering loop - something
  that iterates active game-world actors (not HUD widgets) and uploads
  their current animation frame to VRAM. This is almost certainly a
  different function from `sub_8028A00`, since that one's callers are
  exhaustively the 3 HUD records. Tracing from the main game loop /
  VBlank handler downward (rather than from individual actor vtables
  upward) may be more productive than continuing to inspect individual
  vtable slots one at a time.
- Given the raw-tag (`0x0`) upload path exists (see the VRAM upload
  dispatcher above) and signature scanning can't find uncompressed data,
  the fastest path to *locating* sprite pixels may still be empirical:
  render chunks of the two large untyped regions as raw (uncompressed)
  4bpp tile data at various candidate widths and look for a recognizable
  character, the same way the original background/tileset scan worked -
  independent of and complementary to the code-tracing approach above.
