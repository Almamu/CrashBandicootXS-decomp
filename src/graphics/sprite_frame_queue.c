#include "core.h"
#include "memory.h"

extern void *gUnknown_0300137C; /* decompressed category sprite sheet buffer */
extern void LoadTaggedAsset(void *asset, void *dest);
extern s32 QueueVramDmaTransfer(void *src, void *dest, u16 size, u16 unit);

/* Same `vram_tile_block` layout src/graphics/sprite_frame_pool.c's own
 * copy documents in full (redeclared locally per this project's
 * minimal-local-type convention - see docs/naming.md) - this file only
 * needs `AllocVramTileBlock`'s NON_MATCHING C reconstruction and the
 * two dead-code list-walkers below to see it. */
struct vram_tile_block {
    void *addr;
    u16 size;
    u16 status;
    struct vram_tile_block *next;
    struct vram_tile_block *prev;
};

#define VRAM_TILE_BLOCK_FREE 0
#define VRAM_TILE_BLOCK_USED 2

extern struct vram_tile_block *gUnknown_03001320;
extern struct vram_tile_block gUnknown_03001328;
extern struct vram_tile_block *gUnknown_03001338;
extern struct vram_tile_block *gUnknown_0300133C;
extern u8 *gUnknown_03001340;

extern void *AllocVramTileBlock(s32 requestedSize);
extern void FreeVramTileBlock(void *addr);

#define DMA3 (*(struct dma_regs *)REG_ADDR_DMA3SAD)

/* ROM 0x08028CD4 - next-fit search (starting from and updating the
 * `gUnknown_03001338` rover, the same strategy `mem_alloc` uses over
 * its own free list) for a free block at least `requestedSize` bytes,
 * splitting the remainder off into a spare record when there's enough
 * left over to bother (unlike `mem_alloc`'s in-place split, this one
 * needs a free node record to represent the split-off piece - if the
 * spare stack is empty, the allocation fails even though a big enough
 * block exists, rather than allocate without being able to track the
 * leftover). Records the winning block's own pool-record index into
 * the `gUnknown_03001340` tile lookup table so FreeVramTileBlock can
 * find it again from a bare VRAM address.
 *
 * NON_MATCHING: every operation/field/register in the body below
 * matches the ROM exactly (confirmed via isolated compiles) except the
 * search loop's entry shape. The ROM's own compiled output checks the
 * starting rover once (a standalone, differently-registered copy of
 * the free+size check, using a signed `bge`/`blt` polarity) and only
 * falls into the shared advance/re-check block on failure; this
 * compiler's cross-jump/tail-merging pass at -O2 collapses every C
 * phrasing tried (plain while/for, do-while-with-guard, explicit
 * `goto found`, and manually duplicating the check text with swapped
 * operand order to defeat the merge) back down to one shared check
 * block reached via a leading unconditional branch - the same
 * `bcc`-shaped, singly-deduplicated loop `mem_alloc` itself compiles to
 * (see the real `mem_alloc`/`FreeVramTileBlock` ROM bytes for the
 * contrast). Parked per docs/workflow.md's NON_MATCHING escape hatch
 * rather than an inline-asm island, since every other line already
 * matches and the semantics are fully understood - real bytes are in
 * asm/code_3_2_20_8b7c_cd4.s, guarded by `.if NON_MATCHING == 0`. */
#if NON_MATCHING
void *AllocVramTileBlock(s32 requestedSize)
{
    struct vram_tile_block **roverSlot;
    struct vram_tile_block *cur;
    struct vram_tile_block *end;
    struct vram_tile_block *spare;
    u16 remaining;

    roverSlot = &gUnknown_03001338;
    cur = *roverSlot;
    end = cur->prev;
    for (; cur->status != VRAM_TILE_BLOCK_FREE || cur->size < requestedSize; cur = cur->next) {
        if (cur == end) {
            return NULL;
        }
    }

    remaining = cur->size - requestedSize;
    if (remaining != 0) {
        spare = gUnknown_0300133C;
        if (spare == NULL) {
            return NULL;
        }
        gUnknown_0300133C = spare->next;
        spare->size = remaining;
        spare->addr = (u8 *)cur->addr + requestedSize;
        spare->status = VRAM_TILE_BLOCK_FREE;
        spare->prev = cur;
        spare->next = cur->next;
        cur->next->prev = spare;
        cur->next = spare;
        cur->size = requestedSize;
    }

    cur->status = VRAM_TILE_BLOCK_USED;
    *roverSlot = cur->next;
    gUnknown_03001340[GET_TILE_NUM(cur->addr)] = (u8)(cur - gUnknown_03001320);
    return cur->addr;
}
#endif

/* ROM 0x08028D6C - dead code, no caller anywhere in the ROM (checked
 * every asm file, expected disassembly and src tree for this address
 * and for a `bl sub_8028D6C`/`.4byte sub_8028D6C` reference). Walks the
 * `gUnknown_0300133C` spare-node stack to its end, then the
 * `gUnknown_03001328` free-block list all the way around, discarding
 * both results - the same "list-walk with the result never stored"
 * optimizer-leftover shape already documented for `sub_800039C` in
 * src/system/memory.c (see that function's comment). */
void sub_8028D6C(void)
{
    struct vram_tile_block *p;
    struct vram_tile_block *q;

    for (p = gUnknown_0300133C; p != NULL; p = p->next) {
    }

    for (q = gUnknown_03001328.next; q != &gUnknown_03001328; q = q->next) {
    }
}

/* ROM 0x08028D94 - the original disassembly never gave this address its
 * own function label; it's a separate function from `sub_8028D6C`
 * above (see `expected/corrections.txt`'s `split` entry), also with no
 * caller anywhere in the ROM. Sums the `size` of every FREE block
 * currently in the `gUnknown_03001328` list - a "how many free VRAM
 * tile bytes are left" query the rest of this cluster never calls. */
s32 sub_8028D94(void)
{
    struct vram_tile_block *node;
    s32 total;

    total = 0;
    node = gUnknown_03001328.next;
    if (node != &gUnknown_03001328) {
        do {
            if (node->status == VRAM_TILE_BLOCK_FREE) {
                {
                    s32 size = node->size;
                    asm volatile("add %0, %1, %0" : "+r"(total) : "r"(size));
                }
            }
            node = node->next;
        } while (node != &gUnknown_03001328);
    }
    return total;
}

/* ROM 0x08028DB8 - `InitObjTileFreeList`'s destructor: frees the two
 * EWRAM buffers it allocated. */
void FreeObjTileFreeList(void)
{
    mem_free((u8 *)gUnknown_03001340);
    mem_free((u8 *)gUnknown_03001320);
}

/* One queued OAM entry the overflow arrays below buffer up during a
 * frame - `attr01` packs real hardware ATTR0 in the low 16 bits and
 * ATTR1 in the high 16 (the same convention `SetupSpriteFrameOam`'s
 * shape/size bits below get OR'd into), `attr2` is the real hardware
 * ATTR2 (tile index/priority/palette). `QueueSpriteFrameOam` appends
 * these; `FlushSpriteFrameOamQueue` commits them into the real
 * `struct oam_shadow_buffer`. */
struct queued_oam_entry {
    u32 attr01;
    u16 attr2;
    u8 pad_06[2];
};

extern struct queued_oam_entry *gUnknown_03001344; /* queued OAM entries, OAM_ENTRY_COUNT max */
extern s32 *gUnknown_03001348;                     /* queued affine (x,y) pairs, packed one s16 each into a u32, deduped */
extern s32 gUnknown_0300134C;                      /* gUnknown_03001344 count */
extern s32 gUnknown_03001350;                      /* gUnknown_03001348 count */

/* ROM 0x08028DD8 - appends one OAM entry (`attr01`/`attr2`, hardware
 * ATTR0|ATTR1<<16 and ATTR2) to the `gUnknown_03001344` overflow queue
 * `FlushSpriteFrameOamQueue` later commits. When ATTR0 bit 8 (the
 * hardware "affine" flag) is set, first resolves an affine-parameter
 * group: computes a (x,y) scale pair from `priority` (negated per
 * ATTR1 bits 9/10, already packed into `attr01`'s bits 28/29 by the
 * caller), reuses the previous `gUnknown_03001348` entry if it's
 * identical (a cheap run-length dedup - adjacent sprites sharing a
 * scale are extremely common), otherwise appends a new one, then
 * writes that entry's index into `attr01` bits 25-29 (ATTR1's real
 * affine-index field) after clearing the two negate-flag bits that
 * used to live there. `x`/`y` are pinned to r0/r1 and the two
 * truncate/shift steps split into their own statements - without
 * those this compiler folds the "value or its negation" ternary into
 * a single assign-then-conditionally-negate sequence instead of the
 * ROM's two independent branches into the same register, and reorders
 * the mask/shift pair relative to the ROM (see docs/workflow.md step 7,
 * confirmed by rebuilding without them). */
void QueueSpriteFrameOam(u32 attr01, u16 attr2, s32 priority)
{
    struct queued_oam_entry *entry;

    if (attr01 & 0x100) {
        register s32 x asm("r0");
        register s32 y asm("r1");
        s32 combined;

        if (attr01 & 0x10000000) {
            x = -priority;
        } else {
            x = priority;
        }
        x = (u16)x;
        combined = (combined & 0xFFFF0000) | x;
        if (attr01 & 0x20000000) {
            y = -priority;
        } else {
            y = priority;
        }
        y = y << 16;
        combined = (combined & 0x0000FFFF) | y;
        attr01 &= 0xCFFFFFFF;

        if (gUnknown_03001350 == 0 || combined != gUnknown_03001348[gUnknown_03001350 - 1]) {
            gUnknown_03001348[gUnknown_03001350] = combined;
            gUnknown_03001350++;
        }
        attr01 |= (gUnknown_03001350 - 1) << 25;
    }

    {
        register s32 count asm("r1");
        register struct queued_oam_entry *base asm("r2");
        register s32 offset asm("r0");

        count = gUnknown_0300134C;
        base = gUnknown_03001344;
        offset = count << 3;
        asm volatile("add %0, %0, %1" : "+r"(offset) : "r"(base));

        entry = (struct queued_oam_entry *)offset;
        entry->attr01 = attr01;
        entry->attr2 = attr2;
        gUnknown_0300134C = count + 1;
    }
}

/* ROM 0x08028E88 - frees the two EWRAM buffers `InitSpriteFrameOamQueue`
 * allocated. */
void FreeSpriteFrameOamQueue(void)
{
    mem_free((u8 *)gUnknown_03001348);
    mem_free((u8 *)gUnknown_03001344);
}

/* Same 0x40C-byte OAM shadow buffer src/graphics/graphics.c already
 * names `struct oam_shadow_buffer` (redeclared locally per this
 * project's minimal-local-type convention - see docs/naming.md). */
struct oam_shadow_buffer {
    s32 count;
    s32 field_04;
    s32 field_08;
    u8 table[0x400];
};

extern struct oam_shadow_buffer *gUnknown_03001300;
extern void sub_8006A14(struct oam_shadow_buffer *arg0, void *arg1, s32 arg2);
extern void sub_8006A48(struct oam_shadow_buffer *arg0);
extern void sub_80069E8(void *arg0, u16 *arg1, s32 arg2);

/* ROM 0x08028EA8 - commits this frame's overflow OAM queue into the
 * real hardware-shaped OAM shadow buffer: appends the queued entries
 * (`sub_8006A14`), hides whatever hardware slots are still unused
 * (`sub_8006A48`), pads the affine-parameter table out with the last
 * entry repeated (`sub_80069E8`, the same "hide unused affine groups"
 * idiom `struct oam_shadow_buffer`'s own comment describes), then
 * resets both overflow counts for the next frame. */
void FlushSpriteFrameOamQueue(void)
{
    sub_8006A14(gUnknown_03001300, gUnknown_03001344, gUnknown_0300134C);
    sub_8006A48(gUnknown_03001300);
    sub_80069E8(gUnknown_03001300, (u16 *)gUnknown_03001348, gUnknown_03001350);
    gUnknown_0300134C = 0;
    gUnknown_03001350 = 0;
}

/* ROM 0x08028EF0 - allocates the two overflow buffers above and DMA3-
 * fills the whole real hardware OAM with `0x0200` (ATTR0's "disable"
 * bit with every other field zeroed) - the same "DMA a single fixed
 * halfword across a whole region" trick `InitObjTileFreeList` uses,
 * here hiding every hardware sprite as this system's startup state. */
void InitSpriteFrameOamQueue(void)
{
    u16 hideValue;

    gUnknown_03001344 = (struct queued_oam_entry *)mem_alloc(OAM_ENTRY_COUNT * 8, MEM_HEAP_EWRAM);
    gUnknown_03001348 = (s32 *)mem_alloc(32 * 4, MEM_HEAP_EWRAM);
    gUnknown_0300134C = 0;
    gUnknown_03001350 = 0;

    {
        register u16 *addr asm("r1");
        register u16 tmp asm("r2");
        register u16 val asm("r0");

        addr = &hideValue;
        tmp = 0x80 << 2;
        val = tmp;
        *addr = val;
    }
    DMA3.src = &hideValue;
    DMA3.dst = (void *)OAM;
    DMA3.cnt = (OAM_ENTRY_COUNT * 8 / 2) | ((DMA_ENABLE | DMA_SRC_FIXED) << 16);
    (void)DMA3.cnt;
}

/* A doubly-linked frame-cache entry: `frame` is the source animation-
 * frame record (see docs/graphics.md's `table_B` description) currently
 * resident in VRAM, and `vramAddr` is the `AllocVramTileBlock` result
 * its pixel data was DMA'd into. Nodes live in a fixed pool
 * (`gUnknown_03001374`, seeded by InitSpriteFrameCache) and move
 * between two ring lists as they age: `gUnknown_03001354` (this
 * frame's in-use entries, newest at the head) and `gUnknown_03001364`
 * (last frame's entries, oldest at the tail) - see
 * AgeSpriteFrameCache/LoadSpriteFrameTiles. */
struct sprite_frame_cache_node {
    struct sprite_frame_cache_node *next; // 0x00
    struct sprite_frame_cache_node *prev; // 0x04
    u8 *frame;                             // 0x08
    void *vramAddr;                         // 0x0C
};

#define SPRITE_FRAME_CACHE_POOL_COUNT 128

extern struct sprite_frame_cache_node gUnknown_03001354; /* "this frame" MRU list sentinel */
extern struct sprite_frame_cache_node gUnknown_03001364; /* "last frame" eviction list sentinel */
extern struct sprite_frame_cache_node *gUnknown_03001374; /* spare-record stack head */
extern struct sprite_frame_cache_node *gUnknown_03001378; /* pool base */

extern void *gUnknown_03000870; /* optional frame-source override hook (called via sub_803AD7C) */
extern void *sub_803AD7C(void *arg0, void *fn);

/* ROM 0x08028F58 - resolves one animation frame's tile data into VRAM,
 * returning its OBJ tile index (`GET_TILE_NUM`-shaped, ready to OR
 * into an OAM attr2). First tries an optional override hook
 * (`gUnknown_03000870`, called through the `sub_803AD7C` trampoline
 * convention - see src/system/reg_trampolines.c); if that returns
 * anything other than -1, that's used directly. Otherwise inserts a
 * fresh cache node at the head of the "this frame" MRU list
 * (`gUnknown_03001354`) and tries to `AllocVramTileBlock` the frame's
 * `w*h*32`-byte payload, evicting the least-recently-used entry from
 * the "last frame" list (`gUnknown_03001364`, freeing its VRAM block
 * and recycling its node back onto the spare stack) and retrying until
 * an allocation succeeds. The frame's pixel data (skipping its 4-byte
 * `{w,h,0x30,0x00}` header) is then DMA-queued into the allocated
 * block. */
s32 LoadSpriteFrameTiles(u8 *frame)
{
    struct sprite_frame_cache_node **spareSlot;
    struct sprite_frame_cache_node *node;
    struct sprite_frame_cache_node *oldFirst;
    s32 byteCount;
    s32 result;

    result = (s32)sub_803AD7C(frame, gUnknown_03000870);
    if (result != -1) {
        return result;
    }

    node = gUnknown_03001374;
    gUnknown_03001374 = node->next;
    node->frame = frame;
    node->prev = &gUnknown_03001354;
    oldFirst = gUnknown_03001354.next;
    node->next = oldFirst;
    gUnknown_03001354.next->prev = node;
    gUnknown_03001354.next = node;

    {
        register u32 w asm("r1");
        register u32 h asm("r3");
        register u32 product asm("r0");

        w = frame[0];
        h = frame[1];
        asm volatile("mov %0, %1" : "=r"(product) : "r"(w));
        product *= h;
        byteCount = product << 5;
    }

    spareSlot = &gUnknown_03001374;

    {
        register void *vramAddr asm("r1");

        vramAddr = AllocVramTileBlock(byteCount);
        node->vramAddr = vramAddr;
        while (vramAddr == NULL) {
            struct sprite_frame_cache_node *victim = gUnknown_03001364.prev;

            FreeVramTileBlock(victim->vramAddr);
            victim->prev->next = victim->next;
            victim->next->prev = victim->prev;
            victim->next = *spareSlot;
            *spareSlot = victim;

            vramAddr = AllocVramTileBlock(byteCount);
            node->vramAddr = vramAddr;
        }

        QueueVramDmaTransfer(node->frame + 4, vramAddr, (u16)byteCount, 0x10);
        return GET_TILE_NUM(node->vramAddr);
    }
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
    register s32 w asm("r1");
    register s32 h asm("r0");
    register u32 shapeBits asm("r2");
    u16 packed;
    s32 tileIdx;

    packed = (u16)arg2;

    w = frame[0];
    h = frame[1];
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
        register s32 diff asm("r0");

        shapeBits = 0x80 << 7;
        if (w < h) {
            shapeBits = 0x80 << 8;
        }

        diff = w - h;
        {
            register s32 mask asm("r1");
            mask = diff >> 31;
            diff = (diff ^ mask) - mask;
        }
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
    mem_free((u8 *)gUnknown_03001378);
}

/* ROM 0x08029090 - ages the frame cache one generation: splices every
 * node currently in `gUnknown_03001354` (this frame's entries, MRU-
 * first) onto the *front* of `gUnknown_03001364` (last frame's
 * entries) as one block, preserving relative order, then empties
 * `gUnknown_03001354`. The freshest entries from the finished
 * generation become the least-likely-to-be-evicted end of the
 * eviction list (LoadSpriteFrameTiles evicts from its tail), while
 * anything already in `gUnknown_03001364` before this call gets pushed
 * closer to eviction - a simple two-generation clock cache. */
void AgeSpriteFrameCache(void)
{
    struct sprite_frame_cache_node *head;

    head = gUnknown_03001354.next;
    if (head == &gUnknown_03001354) {
        return;
    }

    head->prev = &gUnknown_03001364;
    gUnknown_03001354.prev->next = gUnknown_03001364.next;
    gUnknown_03001364.next->prev = gUnknown_03001354.prev;
    gUnknown_03001364.next = gUnknown_03001354.next;

    gUnknown_03001354.prev = &gUnknown_03001354;
    gUnknown_03001354.next = &gUnknown_03001354;
}

/* ROM 0x080290BC - allocates the 128-record node pool and seeds
 * `gUnknown_03001374` with a singly-linked (via `next`) LIFO stack of
 * all 128 of them, resetting both ring-list sentinels to empty. */
void InitSpriteFrameCache(void)
{
    struct sprite_frame_cache_node *node;
    s32 i;

    gUnknown_03001378 = (struct sprite_frame_cache_node *)mem_alloc(
        sizeof(struct sprite_frame_cache_node) * SPRITE_FRAME_CACHE_POOL_COUNT, MEM_HEAP_IWRAM);

    gUnknown_03001354.prev = &gUnknown_03001354;
    gUnknown_03001354.next = &gUnknown_03001354;
    gUnknown_03001364.prev = &gUnknown_03001364;
    gUnknown_03001364.next = &gUnknown_03001364;

    gUnknown_03001374 = gUnknown_03001378;
    node = gUnknown_03001378;
    i = SPRITE_FRAME_CACHE_POOL_COUNT - 2;
    do {
        struct sprite_frame_cache_node *next = node + 1;
        node->next = next;
        node = next;
    } while (--i >= 0);
    node->next = NULL;
}

/* ROM 0x08029108 - decodes a frame record's `w_tiles`/`h_tiles` header
 * bytes into the hardware OAM shape+size encoding (square/wide/tall,
 * size class 1/2/4/8 tiles) - see docs/graphics.md's `table_B` write-up
 * for the record format. Identical logic is also inlined directly into
 * `SetupSpriteFrameOam` below (the ROM compiles that copy separately
 * rather than calling this one - see that function's own comment).
 * `result`/`diff`/`mask` are pinned to r2/r0/r1 - without them this
 * compiler picks a different register for `result` (r3, not r2) and
 * spills the abs-value sign mask through an extra register-to-register
 * copy instead of reusing `w`/`h`'s own registers in place, the same
 * kind of register-shuffle gap tracked on `AllocVramTileBlock`/
 * `QueueSpriteFrameOam` above - confirmed by rebuilding without the
 * pins (docs/workflow.md step 7). */
u32 GetSpriteShapeSizeBits(u8 *frame)
{
    register u32 result asm("r2");
    register s32 diff asm("r0");
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

    {
        register s32 mask asm("r1");

        diff = w - h;
        mask = diff >> 31;
        diff = (diff ^ mask) - mask;
    }
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
asm(".align 2, 0");

/* ROM 0x08029168 - frees the buffer `DecompressCategorySpriteSheet`
 * allocated. */
void FreeCategorySpriteSheet(void)
{
    mem_free((u8 *)gUnknown_0300137C);
}

/* ROM 0x0802917C - `InitActorCategory`'s reader for a category
 * descriptor's `+0x1C` sheet pointer (see docs/graphics.md, "Identified
 * the two giant LZ77 sheets"): allocates a buffer for the declared
 * decompressed size (the tag+size header's top 24 bits) and
 * decompresses into it via the shared `LoadTaggedAsset` dispatcher,
 * stashing the result in `gUnknown_0300137C` - the same global
 * `GetAnimFrameData` adds to a raw `table_B` pointer for the
 * "decompressed sheet, relative addressing" animation records. */
void DecompressCategorySpriteSheet(void *sheet)
{
    u32 size = *(u32 *)sheet >> 8;
    void *buf = mem_alloc(size, MEM_HEAP_EWRAM);

    gUnknown_0300137C = buf;
    LoadTaggedAsset(sheet, buf);
}
