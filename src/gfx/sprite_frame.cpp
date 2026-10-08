#include "sprite_obj.hpp"

extern "C" {
#include "core.h"
#include "gfx.h"
#include "system.h"
#include "actor.h"
#include "globals.h"
#include "math_util.h"
}

/* A meta-node for a doubly-linked, address-ordered free-block list that
 * tracks allocations inside the OBJ tile VRAM pool (OBJ_VRAM0,
 * OBJ_VRAM0_SIZE bytes) InitObjTileFreeList sets up. Metadata lives in
 * a separate fixed pool of these structs (never embedded in VRAM
 * itself, since VRAM holds real tile pixel data), looked up via a
 * byte-per-tile index table (`gVramTileBlockIndex`) - see
 * InitObjTileFreeList/AllocVramTileBlock/FreeVramTileBlock below.
 * `status` mirrors `struct mem_block`'s same-shaped field (0 = free),
 * but this allocator's own "in use" value (2) doesn't match
 * MEMORY_STATUS_USED (1) from memory.h, so it gets its own constant
 * rather than reusing that one. */
struct vram_tile_block {
    void *addr;                   // 0x00
    u16 size;                     // 0x04
    u16 status;                   // 0x06
    struct vram_tile_block *next; // 0x08
    struct vram_tile_block *prev; // 0x0C
};

#define VRAM_TILE_BLOCK_FREE 0
#define VRAM_TILE_BLOCK_USED 2

/* 128 preallocated `vram_tile_block` records; InitObjTileFreeList seeds
 * `gVramTileBlockSpares` with a singly-linked (via `next`) LIFO stack of
 * 127 of them (indices 0-126) as "spare" records available whenever
 * AllocVramTileBlock/FreeVramTileBlock need to split or merge a block,
 * and uses the 128th (index 127) as the pool's initial single free
 * block, spanning the whole range. */
#define VRAM_TILE_BLOCK_POOL_COUNT 128

/* ROM 0x08028BA0 - sets up the dynamic OBJ-tile VRAM allocator: two
 * EWRAM buffers (the 128-record node pool above, and the tile-index
 * lookup table), a DMA3 zero-fill of the whole `base`..OBJ_VRAM0+
 * OBJ_VRAM0_SIZE range, and the free-block list's initial state (one
 * free block spanning the entire range, plus the spare-record stack -
 * see the struct's own comment above). `base` is always OBJ_VRAM0 at
 * this ROM's one real call site, but the free-block list's own address
 * bookkeeping is relative to whatever's passed in. */
void InitObjTileFreeList(void *base)
{
    struct vram_tile_block *node;
    u8 **lookupSlot;
    s32 len;
    s32 i;

    gVramTileBlockPool = (struct vram_tile_block *)mem_alloc(
        sizeof(struct vram_tile_block) * VRAM_TILE_BLOCK_POOL_COUNT, MEM_HEAP_EWRAM);
    lookupSlot = &gVramTileBlockIndex;
    *lookupSlot = (u8 *)mem_alloc(TOTAL_OBJ_TILE_COUNT, MEM_HEAP_EWRAM);

    len = (s32)((u8 *)OBJ_VRAM0 + OBJ_VRAM0_SIZE - (u8 *)base);

    DmaFill16(3, 0, base, len);

    gVramTileBlockSpares = gVramTileBlockPool;
    node = gVramTileBlockPool;
    i = VRAM_TILE_BLOCK_POOL_COUNT - 3;
    do {
        struct vram_tile_block *next = node + 1;
        node->next = next;
        node = next;
    } while (--i >= 0);
    node->next = 0;
    node++;

    gVramTileBlockList.next = node;
    gVramTileBlockList.prev = node;
    gVramTileBlockList.status = 1;
    gVramTileBlockList.size = 0;
    gVramTileBlockList.addr = (u8 *)OBJ_VRAM0 + OBJ_VRAM0_SIZE;

    node->status = VRAM_TILE_BLOCK_FREE;
    node->prev = &gVramTileBlockList;
    node->next = &gVramTileBlockList;
    node->size = (u16)len;
    node->addr = base;
    gVramTileBlockRover = node;
}

/* ROM 0x08028C48 - frees a block previously returned by
 * AllocVramTileBlock, coalescing with its address-order neighbors when
 * they're also free (the same two-sided merge `mem_free` in
 * src/system/memory.cpp does, but against this allocator's own external
 * node pool instead of an embedded-in-buffer header - see the struct's
 * comment above). Reclaimed node records go back onto the
 * `gVramTileBlockSpares` spare stack. */
void FreeVramTileBlock(void *addr)
{
    struct vram_tile_block *node;
    struct vram_tile_block *adj;
    struct vram_tile_block *next;
    u32 tile;

    if (addr == 0) {
        return;
    }

    tile = GET_TILE_NUM(addr);
    node = &gVramTileBlockPool[gVramTileBlockIndex[tile]];
    node->status = VRAM_TILE_BLOCK_FREE;

    adj = node->prev;
    if (adj->status == VRAM_TILE_BLOCK_FREE) {
        adj->size += node->size;
        next = node->next;
        adj->next = next;
        next->prev = adj;
        if (node == gVramTileBlockRover) {
            gVramTileBlockRover = adj;
        }
        node->next = gVramTileBlockSpares;
        gVramTileBlockSpares = node;
        node = adj;
    }

    adj = node->next;
    if (adj->status == VRAM_TILE_BLOCK_FREE) {
        node->size += adj->size;
        next = adj->next;
        node->next = next;
        next->prev = node;
        if (adj == gVramTileBlockRover) {
            gVramTileBlockRover = node;
        }
        adj->next = gVramTileBlockSpares;
        gVramTileBlockSpares = adj;
    }
}

/* ROM 0x08028CD4 - next-fit search (starting from and updating the
 * `gVramTileBlockRover` rover, the same strategy `mem_alloc` uses over
 * its own free list) for a free block at least `size` bytes,
 * splitting the remainder off into a spare record when there's enough
 * left over to bother (unlike `mem_alloc`'s in-place split, this one
 * needs a free node record to represent the split-off piece - if the
 * spare stack is empty, the allocation fails even though a big enough
 * block exists, rather than allocate without being able to track the
 * leftover). Records the winning block's own pool-record index into
 * the `gVramTileBlockIndex` tile lookup table so FreeVramTileBlock can
 * find it again from a bare VRAM address.
 *
 * Was one `asm volatile` island (#662). Under old_agbcc the plain loop
 * matches, with `rover` as its own local: the ROM's copy of the search
 * test before the loop uses other registers than the one at its bottom,
 * so cross-jumping doesn't merge them. */
void *AllocVramTileBlock(s32 size)
{
    struct vram_tile_block *cur;
    struct vram_tile_block *last;
    struct vram_tile_block *rover;
    s32 rest;

    rover = gVramTileBlockRover;
    last = rover->prev;
    cur = rover;
    while (cur->status != VRAM_TILE_BLOCK_FREE || cur->size < size) {
        if (cur == last) {
            return 0;
        }
        cur = cur->next;
    }

    rest = cur->size - size;
    if (rest != 0) {
        struct vram_tile_block *spare = gVramTileBlockSpares;
        struct vram_tile_block *next;

        if (spare == 0) {
            return 0;
        }
        gVramTileBlockSpares = spare->next;
        spare->size = rest;
        spare->addr = (u8 *)cur->addr + size;
        spare->status = VRAM_TILE_BLOCK_FREE;
        spare->prev = cur;
        next = cur->next;
        spare->next = next;
        next->prev = spare;
        cur->next = spare;
        cur->size = size;
    }

    cur->status = VRAM_TILE_BLOCK_USED;
    gVramTileBlockRover = cur->next;
    gVramTileBlockIndex[GET_TILE_NUM(cur->addr)] = cur - gVramTileBlockPool;
    return cur->addr;
}

/* UNUSED - ROM 0x08028D6C, no caller anywhere in the ROM (checked
 * every asm file, expected disassembly and src tree for this address
 * and for a `bl`/`.4byte` reference to it). Walks the
 * `gVramTileBlockSpares` spare-node stack to its end, then the
 * `gVramTileBlockList` free-block list all the way around, discarding
 * both results - the same "list-walk with the result never stored"
 * optimizer-leftover shape already documented for `mem_walk_heaps` in
 * src/system/memory.cpp (see that function's comment). */
void WalkVramTileBlocks(void)
{
    struct vram_tile_block *p;
    struct vram_tile_block *q;

    for (p = gVramTileBlockSpares; p != 0; p = p->next) {
    }

    for (q = gVramTileBlockList.next; q != &gVramTileBlockList; q = q->next) {
    }
}

/* ROM 0x08028D94 - the original disassembly never gave this address its
 * own function label; it's a separate function from `WalkVramTileBlocks`
 * above (see `expected/corrections.txt`'s `split` entry), also with no
 * caller anywhere in the ROM. Sums the `size` of every FREE block
 * currently in the `gVramTileBlockList` list - a "how many free VRAM
 * tile bytes are left" query the rest of this cluster never calls. */
s32 GetFreeVramTileBytes(void)
{
    struct vram_tile_block *node;
    s32 total;

    total = 0;
    for (node = gVramTileBlockList.next; node != &gVramTileBlockList; node = node->next) {
        if (node->status == VRAM_TILE_BLOCK_FREE) {
            total += node->size;
        }
    }
    return total;
}

/* ROM 0x08028DB8 - `InitObjTileFreeList`'s destructor: frees the two
 * EWRAM buffers it allocated. */
void FreeObjTileFreeList(void)
{
    mem_free((u8 *)gVramTileBlockIndex);
    mem_free((u8 *)gVramTileBlockPool);
}

/* One queued OAM entry the overflow arrays below buffer up during a
 * frame - `attr01` packs real hardware ATTR0 in the low 16 bits and
 * ATTR1 in the high 16 (the same convention `SetupSpriteFrameOam`'s
 * shape/size bits below get OR'd into), `attr2` is the real hardware
 * ATTR2 (tile index/priority/palette). `QueueSpriteFrameOam` appends
 * these; `FlushSpriteFrameOamQueue` commits them into the real
 * OAM shadow buffer (OamBuffer, sprite_obj.hpp). */
struct queued_oam_entry {
    u32 attr01;
    u16 attr2;
};

/* attr01 bits QueueSpriteFrameOam tests. The two negate flags sit in
 * ATTR1's affine-index field until it is written. */
#define QUEUED_OAM_AFFINE   0x100      // ATTR0 bit 8: rotation/scaling
#define QUEUED_OAM_NEGATE_X 0x10000000 // negate the affine x scale
#define QUEUED_OAM_NEGATE_Y 0x20000000 // negate the affine y scale

/* An affine x/y scale pair as `gSpriteAffineQueue` stores it: x in the
 * low halfword, y in the high one. The ROM builds it in one register,
 * inserting each half into whatever the register held. */
union affine_scale {
    s32 raw;
    struct {
        u32 x:16;
        u32 y:16;
    } xy;
};

/* ROM 0x08028DD8 - appends one OAM entry (`attr01`/`attr2`, hardware
 * ATTR0|ATTR1<<16 and ATTR2) to the `gSpriteOamQueue` overflow queue
 * `FlushSpriteFrameOamQueue` later commits. When ATTR0 bit 8 (the
 * hardware "affine" flag) is set, first resolves an affine-parameter
 * group: computes a (x,y) scale pair from `priority` (negated per
 * ATTR1 bits 9/10, already packed into `attr01`'s bits 28/29 by the
 * caller), reuses the previous `gSpriteAffineQueue` entry if it's
 * identical (a cheap run-length dedup - adjacent sprites sharing a
 * scale are extremely common), otherwise appends a new one, then
 * writes that entry's index into `attr01` bits 25-29 (ATTR1's real
 * affine-index field) after clearing the two negate-flag bits that
 * used to live there. */
void QueueSpriteFrameOam(u32 attr01, u16 attr2, s32 priority)
{
    if (attr01 & QUEUED_OAM_AFFINE) {
        union affine_scale scale;

        scale.xy.x = (attr01 & QUEUED_OAM_NEGATE_X) ? -priority : priority;
        scale.xy.y = (attr01 & QUEUED_OAM_NEGATE_Y) ? -priority : priority;
        attr01 &= ~(QUEUED_OAM_NEGATE_X | QUEUED_OAM_NEGATE_Y);

        if (gSpriteAffineQueueCount == 0 ||
            scale.raw != gSpriteAffineQueue[gSpriteAffineQueueCount - 1]) {
            gSpriteAffineQueue[gSpriteAffineQueueCount] = scale.raw;
            gSpriteAffineQueueCount++;
        }
        attr01 |= (gSpriteAffineQueueCount - 1) << 25;
    }

    gSpriteOamQueue[gSpriteOamQueueCount].attr01 = attr01;
    gSpriteOamQueue[gSpriteOamQueueCount].attr2 = attr2;
    gSpriteOamQueueCount++;
}

/* ROM 0x08028E88 - frees the two EWRAM buffers `InitSpriteFrameOamQueue`
 * allocated. */
void FreeSpriteFrameOamQueue(void)
{
    mem_free((u8 *)gSpriteAffineQueue);
    mem_free((u8 *)gSpriteOamQueue);
}

/* ROM 0x08028EA8 - commits this frame's overflow OAM queue into the
 * real hardware-shaped OAM shadow buffer: appends the queued entries
 * (`AppendOamEntries`), hides whatever hardware slots are still unused
 * (`HideUnusedOamEntries`), pads the affine-parameter table out with the last
 * entry repeated (`SetOamAffineScales`, the same "hide unused affine groups"
 * idiom OamBuffer's own comment describes), then
 * resets both overflow counts for the next frame. */
void FlushSpriteFrameOamQueue(void)
{
    gOamBuffer->Append(gSpriteOamQueue, gSpriteOamQueueCount);
    gOamBuffer->HideUnused();
    gOamBuffer->SetAffineScales((u16 *)gSpriteAffineQueue, gSpriteAffineQueueCount);
    gSpriteOamQueueCount = 0;
    gSpriteAffineQueueCount = 0;
}

/* ROM 0x08028EF0 - allocates the two overflow buffers above and DMA3-
 * fills the whole real hardware OAM with `0x0200` (ATTR0's "disable"
 * bit with every other field zeroed) - the same "DMA a single fixed
 * halfword across a whole region" trick `InitObjTileFreeList` uses,
 * here hiding every hardware sprite as this system's startup state. */
void InitSpriteFrameOamQueue(void)
{
    gSpriteOamQueue = (struct queued_oam_entry *)mem_alloc(OAM_ENTRY_COUNT * 8, MEM_HEAP_EWRAM);
    gSpriteAffineQueue = (s32 *)mem_alloc(32 * 4, MEM_HEAP_EWRAM);
    gSpriteOamQueueCount = 0;
    gSpriteAffineQueueCount = 0;
    DmaFill16(3, 0x200, OAM, OAM_ENTRY_COUNT * 8);
}

/* A doubly-linked frame-cache entry: `frame` is the source animation-
 * frame record (see docs/graphics.md's `table_B` description) currently
 * resident in VRAM, and `vramAddr` is the `AllocVramTileBlock` result
 * its pixel data was DMA'd into. Nodes live in a fixed pool
 * (`gSpriteFrameCacheSpares`, seeded by InitSpriteFrameCache) and move
 * between two ring lists as they age: `gSpriteFrameCacheCurrent` (this
 * frame's in-use entries, newest at the head) and `gSpriteFrameCachePrevious`
 * (last frame's entries, oldest at the tail) - see
 * AgeSpriteFrameCache/LoadSpriteFrameTiles. */
#define SPRITE_FRAME_CACHE_POOL_COUNT 128

/* ROM 0x08028F58 - resolves one animation frame's tile data into VRAM,
 * returning its OBJ tile index (`GET_TILE_NUM`-shaped, ready to OR
 * into an OAM attr2). First tries an optional override hook
 * (`gLookupSpriteFrameCacheFunc`, called through the `_call_via_r1` trampoline
 * convention - see lib/libgcc/lib1funcs.s); if that returns
 * anything other than -1, that's used directly. Otherwise inserts a
 * fresh cache node at the head of the "this frame" MRU list
 * (`gSpriteFrameCacheCurrent`) and tries to `AllocVramTileBlock` the frame's
 * `w*h*32`-byte payload, evicting the least-recently-used entry from
 * the "last frame" list (`gSpriteFrameCachePrevious`, freeing its VRAM block
 * and recycling its node back onto the spare stack) and retrying until
 * an allocation succeeds. The frame's pixel data (skipping its 4-byte
 * `{w,h,0x30,0x00}` header) is then DMA-queued into the allocated
 * block. */
s32 LoadSpriteFrameTiles(u8 *frame)
{
    struct sprite_frame_cache_node *node;
    struct sprite_frame_cache_node *oldFirst;
    s32 byteCount;
    s32 result;

    result = _call_via_r1(frame, gLookupSpriteFrameCacheFunc);
    if (result != -1) {
        return result;
    }

    node = gSpriteFrameCacheSpares;
    gSpriteFrameCacheSpares = node->next;
    node->frame = frame;
    node->prev = &gSpriteFrameCacheCurrent;
    oldFirst = gSpriteFrameCacheCurrent.next;
    node->next = oldFirst;
    gSpriteFrameCacheCurrent.next->prev = node;
    gSpriteFrameCacheCurrent.next = node;

    byteCount = frame[1] * frame[0] * 32;

    while ((node->vramAddr = AllocVramTileBlock(byteCount)) == 0) {
        struct sprite_frame_cache_node *victim = gSpriteFrameCachePrevious.prev;

        FreeVramTileBlock(victim->vramAddr);
        victim->prev->next = victim->next;
        victim->next->prev = victim->prev;
        victim->next = gSpriteFrameCacheSpares;
        gSpriteFrameCacheSpares = victim;
    }

    QueueVramDmaTransfer(node->frame + 4, node->vramAddr, (u16)byteCount, 0x10);
    return GET_TILE_NUM(node->vramAddr);
}

/* ROM 0x08028FF8 - builds and queues one sprite frame's OAM entry:
 * decodes the frame's shape/size bits (identical logic to
 * `GetSpriteShapeSizeBits` above, inlined here rather than called -
 * see that function's own comment), ORs them into the caller's own
 * `attr01` bits, resolves the frame's VRAM tile index via
 * `LoadSpriteFrameTiles` and ORs that into the caller's `arg2` (the
 * attr2-to-be), then hands both off to `QueueSpriteFrameOam` alongside
 * `priority` (used only for the affine-scale path). */
void SetupSpriteFrameOam(u8 *frame, u32 attr01, u32 arg2, s32 priority)
{
    u32 shapeBits;
    u16 packed = (u16)arg2;
    s32 w = frame[0];
    s32 h = frame[1];
    s32 tileIdx;

    if (w == h) {
        shapeBits = 0;
        if (w == 8) {
            shapeBits = 0xC0 << 24;
        } else if (w == 4) {
            shapeBits = 0x80 << 24;
        } else if (w == 2) {
            shapeBits = 0x80 << 23;
        }
    } else {
        s32 diff;

        shapeBits = 0x80 << 7;
        if (w < h) {
            shapeBits = 0x80 << 8;
        }

        diff = ABS_BRANCHLESS(w - h);
        if (diff == 4) {
            shapeBits |= 0xC0 << 24;
        } else if (diff == 2) {
            shapeBits |= 0x80 << 24;
        } else if (diff == 3) {
            shapeBits |= 0x80 << 23;
        }
    }
    attr01 |= shapeBits;

    tileIdx = LoadSpriteFrameTiles(frame);
    packed |= (u16)tileIdx;

    QueueSpriteFrameOam(attr01, packed, priority);
}

/* ROM 0x0802907C - frees the pool `InitSpriteFrameCache` allocated. */
void FreeSpriteFrameCache(void)
{
    mem_free((u8 *)gSpriteFrameCachePool);
}

/* ROM 0x08029090 - ages the frame cache one generation: splices every
 * node currently in `gSpriteFrameCacheCurrent` (this frame's entries, MRU-
 * first) onto the *front* of `gSpriteFrameCachePrevious` (last frame's
 * entries) as one block, preserving relative order, then empties
 * `gSpriteFrameCacheCurrent`. The freshest entries from the finished
 * generation become the least-likely-to-be-evicted end of the
 * eviction list (LoadSpriteFrameTiles evicts from its tail), while
 * anything already in `gSpriteFrameCachePrevious` before this call gets pushed
 * closer to eviction - a simple two-generation clock cache. */
void AgeSpriteFrameCache(void)
{
    struct sprite_frame_cache_node *head;

    head = gSpriteFrameCacheCurrent.next;
    if (head == &gSpriteFrameCacheCurrent) {
        return;
    }

    head->prev = &gSpriteFrameCachePrevious;
    gSpriteFrameCacheCurrent.prev->next = gSpriteFrameCachePrevious.next;
    gSpriteFrameCachePrevious.next->prev = gSpriteFrameCacheCurrent.prev;
    gSpriteFrameCachePrevious.next = gSpriteFrameCacheCurrent.next;

    gSpriteFrameCacheCurrent.prev = &gSpriteFrameCacheCurrent;
    gSpriteFrameCacheCurrent.next = &gSpriteFrameCacheCurrent;
}

/* ROM 0x080290BC - allocates the 128-record node pool and seeds
 * `gSpriteFrameCacheSpares` with a singly-linked (via `next`) LIFO stack of
 * all 128 of them, resetting both ring-list sentinels to empty. */
void InitSpriteFrameCache(void)
{
    struct sprite_frame_cache_node *node;
    s32 i;

    gSpriteFrameCachePool = (struct sprite_frame_cache_node *)mem_alloc(
        sizeof(struct sprite_frame_cache_node) * SPRITE_FRAME_CACHE_POOL_COUNT, MEM_HEAP_IWRAM);

    gSpriteFrameCacheCurrent.prev = &gSpriteFrameCacheCurrent;
    gSpriteFrameCacheCurrent.next = &gSpriteFrameCacheCurrent;
    gSpriteFrameCachePrevious.prev = &gSpriteFrameCachePrevious;
    gSpriteFrameCachePrevious.next = &gSpriteFrameCachePrevious;

    gSpriteFrameCacheSpares = gSpriteFrameCachePool;
    node = gSpriteFrameCachePool;
    i = SPRITE_FRAME_CACHE_POOL_COUNT - 2;
    do {
        struct sprite_frame_cache_node *next = node + 1;
        node->next = next;
        node = next;
    } while (--i >= 0);
    node->next = 0;
}

/* ROM 0x08029108 - decodes a frame record's `w_tiles`/`h_tiles` header
 * bytes into the hardware OAM shape+size encoding (square/wide/tall,
 * size class 1/2/4/8 tiles) - see docs/graphics.md's `table_B` write-up
 * for the record format. Identical logic is also inlined directly into
 * `SetupSpriteFrameOam` below (the ROM compiles that copy separately
 * rather than calling this one - see that function's own comment).
 * The ROM's abs is branchless, and written as one expression it gets the
 * ROM's registers (a separate sign-mask local doesn't). */
u32 GetSpriteShapeSizeBits(u8 *frame)
{
    u32 result;
    s32 diff;
    s32 w = frame[0];
    s32 h = frame[1];

    if (w == h) {
        result = 0;
        if (w == 8) {
            result = 0xC0 << 24;
        } else if (w == 4) {
            result = 0x80 << 24;
        } else if (w == 2) {
            result = 0x80 << 23;
        }
        goto done;
    }

    result = 0x80 << 7;
    if (w < h) {
        result = 0x80 << 8;
    }

    diff = ABS_BRANCHLESS(w - h);
    if (diff == 4) {
        result |= 0xC0 << 24;
    } else if (diff == 2) {
        result |= 0x80 << 24;
    } else if (diff == 3) {
        result |= 0x80 << 23;
    }
done:
    return result;
}

/* ROM 0x08029168 - frees the buffer `DecompressCategorySpriteSheet`
 * allocated. */
void FreeCategorySpriteSheet(void)
{
    mem_free((u8 *)gCategorySpriteSheet);
}

/* ROM 0x0802917C - `InitActorCategory`'s reader for a category
 * descriptor's `+0x1C` sheet pointer (see docs/graphics.md, "Identified
 * the two giant LZ77 sheets"): allocates a buffer for the declared
 * decompressed size (the tag+size header's top 24 bits) and
 * decompresses into it via the shared `LoadTaggedAsset` dispatcher,
 * stashing the result in `gCategorySpriteSheet` - the same global
 * `GetAnimFrameData` adds to a raw `table_B` pointer for the
 * "decompressed sheet, relative addressing" animation records. */
void DecompressCategorySpriteSheet(const void *sheet)
{
    u32 size = *(const u32 *)sheet >> 8;
    void *buf = mem_alloc(size, MEM_HEAP_EWRAM);

    gCategorySpriteSheet = buf;
    LoadTaggedAsset(sheet, buf);
}
