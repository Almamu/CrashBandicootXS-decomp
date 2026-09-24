# Issue #47: 0x08028BA0-0x080291A4, graphics_loading

Seventeen functions covering three related subsystems that sit back to back
in ROM order: the dynamic OBJ-tile VRAM allocator, the per-frame overflow
OAM/affine queue built on top of it, and the sprite-frame VRAM cache that
uses both. All three were already named/understood at a high level in
`docs/rom_map.md` before this pass; the work here was turning that
understanding into byte-exact C.

## The OBJ-tile VRAM allocator (`src/graphics/sprite_frame_pool.c`,
`src/graphics/sprite_frame_queue.c`)

A small doubly-linked, address-sorted free-block allocator - structurally
the same design as `mem_alloc`/`mem_free` in `src/system/memory.c` (next-fit
search with a rover cursor, two-sided coalesce-on-free) but with metadata
kept in an external, fixed-size pool of `struct vram_tile_block` records
(16 bytes: `addr`, `size`, `status`, `next`, `prev`) instead of an
embedded-in-buffer header, since the memory being managed is real OBJ tile
VRAM (0x06010000-0x06018000) that can't hold a software header mixed in
with pixel data. A 1024-byte lookup table (`gUnknown_03001340`, one byte
per possible 32-byte tile slot) maps a bare VRAM address back to its
owning record's pool index for `FreeVramTileBlock`.

- **`InitObjTileFreeList`** - matched. Allocates the 128-record pool and
  the lookup table, DMA3-zero-fills the whole VRAM range, and seeds the
  free-block list with one record spanning the entire range plus a
  singly-linked LIFO stack of the 127 remaining spare records.
- **`FreeVramTileBlock`** (was `sub_8028C48`) - matched. The `mem_free`-shaped
  two-sided coalesce.
- **`AllocVramTileBlock`** (was `sub_8028CD4`) - **matched** (moved to
  `src/graphics/sprite_frame_queue.c`, real bytes in
  `asm/code_3_2_20_8b7c_cd4.s` deleted). The `mem_alloc`-shaped next-fit
  search, splitting off a spare record when there's leftover space,
  failing the allocation outright if the spare-record stack is empty even
  though a big enough block exists. Every operation, field access and
  register in a plain-C reconstruction matched the ROM exactly *except*
  the search loop's entry shape: the ROM's own compiled output checks the
  starting rover once, as a standalone, differently-registered copy of the
  free+size check (`bge`/`blt` polarity), and only falls into the shared
  advance/re-check block on failure - but this compiler's cross-jump/
  tail-merging pass at `-O2` collapses every C phrasing tried (plain
  while/for, do-while-with-a-guard-if, explicit `goto found` with a
  `for(;;)`, and manually duplicating the check text with swapped operand
  order specifically to defeat the block-merge) back down to one shared
  check block reached via a leading unconditional branch - the exact
  `bcc`-shaped, singly-deduplicated loop shape `mem_alloc` itself
  legitimately compiles to (confirmed by objdumping `mem_alloc`'s own real
  ROM bytes for contrast - it never had the ROM's two-copy shape to begin
  with). Closed by giving up on plain C for this region and writing one
  continuous `asm volatile` island (register-pinned `roverSlot`/`cur`/
  `requestedSize` to r6/r3/r4) reproducing the ROM's loop-entry-plus-loop
  instructions literally, with real `.L`-prefixed named labels (the same
  "opaque asm reaching a named landing point defined by a later, ordinary
  C statement's own `asm volatile(".Lname:")` marker" idiom
  `sub_802AB58` in `src/graphics/actor_part53.c` established) standing in
  for the two ROM-shared merge points a plain C `if`/`return` can't be
  aimed at a chosen physical address: the "not found" early return, and
  the free-list-split/no-split rejoin. The same asm island also had to
  swallow the free-list-split logic and a `gUnknown_0300133C` literal
  pool right after the "not found" trampoline - both cases of this
  compiler's own CSE/pool-placement choices (reusing an already-loaded
  register instead of ROM's redundant reload; deferring the pool to the
  function's end instead of the ROM's mid-function group) that plain C
  couldn't be steered around either. See that function's own comment in
  `src/graphics/sprite_frame_queue.c` for the full instruction-by-
  instruction breakdown.
- **`sub_8028D6C`** - matched, **UNUSED** (no caller anywhere in the ROM,
  checked every `asm/*.s`, `expected/*.s` and `src/**/*.c` for the address
  and a `bl`/`.4byte` reference). Walks the spare-record stack to its end,
  then the free-block list all the way around, discarding both results -
  the same "list-walk with the result never stored" optimizer-leftover
  shape already documented for `sub_800039C` in `src/system/memory.c`.
- **`sub_8028D94`** - matched, **UNUSED**. The original disassembly never
  gave this address its own `thumb_func_start`; it's a genuinely separate
  function starting right where `sub_8028D6C`'s real body ends (confirmed
  by there being no caller for the combined "one function" reading and by
  this half being a clean, self-contained "sum every free block's size"
  routine). Added a `split 0x08028D94 sub_8028D94` correction, the same
  pattern `sub_800039C` already established.

## The overflow OAM/affine queue (`src/graphics/sprite_frame_queue.c`)

A small per-frame buffer that `SetupSpriteFrameOam` appends into instead
of writing straight to the real hardware-shaped OAM shadow buffer, later
committed in bulk by `FlushSpriteFrameOamQueue`.

- **`QueueSpriteFrameOam`** (was `sub_8028DD8`) - matched. Appends one
  `{attr01, attr2}` entry; when the caller's ATTR0 affine bit is set, first
  resolves an affine-parameter group (a signed-per-axis scale pair derived
  from `priority`, deduped against the immediately-preceding entry, with
  the winning index packed into ATTR1's real affine-select bits).
- **`FreeSpriteFrameOamQueue`** (was `sub_8028E88`), **`InitSpriteFrameOamQueue`**
  (was `sub_8028EF0`) - matched. Init allocates the two overflow arrays and
  DMA3-fills real hardware OAM with `0x0200` (every OBJ disabled) as this
  system's startup state - the same "DMA a fixed halfword across a region"
  trick `InitObjTileFreeList` uses for its VRAM clear.
- **`FlushSpriteFrameOamQueue`** (was `sub_8028EA8`) - matched. Commits the
  queue into the real `struct oam_shadow_buffer` and resets both counts.

## The sprite-frame VRAM cache (`src/graphics/sprite_frame_queue.c`)

A small two-generation clock cache (`struct sprite_frame_cache_node`,
matched to the same 128-record-pool-plus-spare-stack shape as the tile
allocator above) tracking which animation frames currently have their
pixel data resident in VRAM, so repeated frames within a level don't
re-DMA the same tiles every call.

- **`LoadSpriteFrameTiles`** (0x08028F58) - matched. First tries an
  optional override hook (`gUnknown_03000870`, called through the
  `sub_803AD7C` register-trampoline convention); on a genuine cache miss,
  inserts a fresh node at the head of the "this frame" MRU list and
  `AllocVramTileBlock`s the frame's payload, evicting the tail of the
  "last frame" list (freeing its VRAM block, recycling its node) and
  retrying until an allocation succeeds.
- **`SetupSpriteFrameOam`** (0x08028FF8) - matched. Decodes the frame's
  `w`/`h` header into the hardware OAM shape/size encoding (the exact same
  logic `GetSpriteShapeSizeBits` below implements, but inlined here rather
  than called - the ROM genuinely compiles two separate copies, not one
  shared call), then hands off to `LoadSpriteFrameTiles` and
  `QueueSpriteFrameOam`.
- **`FreeSpriteFrameCache`** (was `sub_802907C`), **`InitSpriteFrameCache`**
  (was `sub_80290BC`) - matched.
- **`AgeSpriteFrameCache`** (was `sub_8029090`) - matched. Splices the
  entire "this frame" list onto the front of the "last frame" list as one
  block (preserving order), then empties "this frame" - the actual
  generation-advance step of the clock cache.
- **`GetSpriteShapeSizeBits`** (was `sub_8029108`) - matched. The standalone
  twin of `SetupSpriteFrameOam`'s inlined shape/size logic. Needed
  `result`/`diff`/`mask` pinned to r2/r0/r1 (this compiler otherwise picks
  r3 for `result` and spills the abs-value sign mask through an extra
  register-to-register copy instead of reusing `w`/`h`'s own registers in
  place) and the usual trailing `asm(".align 2, 0")` at file scope after
  the function (no literal pool of its own, so nothing else forces the
  4-byte pad the ROM has before the next function).
- **`FreeCategorySpriteSheet`** (was `sub_8029168`), **`DecompressCategorySpriteSheet`**
  (0x0802917C) - matched. The decompress side is `InitActorCategory`'s
  reader for a category descriptor's `+0x1C` LZ77 sheet pointer (see
  `docs/graphics.md`, "Identified the two giant LZ77 sheets").

## Gotchas worth recording for future functions in this cluster

- **Cross-jump/tail-merging at `-O2` can't be defeated from C alone** once
  gcc-2.9 recognizes two basic blocks are equal - see `AllocVramTileBlock`
  above. Recognize this pattern early (ROM has two differently-registered
  copies of the same check, one reached by fallthrough and one by a
  leading `b`) rather than burning time trying every loop-phrasing
  variant - go straight to a hand-written `asm volatile` island
  reproducing the ROM bytes literally, landing on named `.L`-prefixed
  labels a later plain-C statement's own no-op `asm volatile(".Lname:")`
  marker defines (see `sub_802AB58` in `actor_part53.c` for the idiom
  this project already established, and `AllocVramTileBlock`'s own
  comment for a second worked example spanning a loop, a literal pool
  split, and a free-list-split's CSE quirks all at once).
- **The "value or its negation" ternary always canonicalizes** to
  "assign the positive value, then conditionally negate in place",
  discarding the ROM's two-independent-branches shape - fixable with
  `register` pins on the destination plus splitting the truncate/shift
  into their own statements (see `QueueSpriteFrameOam`'s affine-scale
  pair), unlike the loop-merging issue above.
- **Commutative-operand order (`cmp`/`add`) is not reliably controlled by
  C statement order** - this compiler sometimes canonicalizes regardless
  of which operand is written first. An `asm volatile("add %0, %0, %1" :
  "+r"(x) : "r"(y))`-style single-instruction island pinned exactly on the
  two operands is the reliable fix when plain reordering doesn't stick
  (confirmed needed in both `sub_8028D94`'s sum and one of
  `QueueSpriteFrameOam`'s pointer-arithmetic adds; a few other spots that
  looked identical happened to already match with plain C, so try the
  cheap fix first).
- **Struct-field partial-store order can also get reordered independent
  of C statement order** for two writes to the same base pointer with no
  data dependency between them (seen initializing `struct
  sprite_frame_cache_node`'s `next`/`prev` sentinel self-links) - in this
  case plain C in the right order turned out to already work once
  isolated correctly; a stray `volatile`/pointer-cast "fix" attempted
  first actually broke the match by changing the codegen shape entirely,
  so verify with a fresh isolated compile before trusting any manual
  disassembly transcription here - `arm-none-eabi-objdump` output can
  look inconsistent between runs if you're not careful about which
  address range/file you're actually looking at; a direct Python byte
  read of the exact file offset is the reliable tie-breaker.
