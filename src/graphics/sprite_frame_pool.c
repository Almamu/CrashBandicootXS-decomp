#include "core.h"
#include "memory.h"

extern void *gCategorySpriteSheet; /* decompressed category sprite sheet buffer */
extern void LoadTaggedAsset(void *asset, void *dest);
extern s32 QueueVramDmaTransfer(void *src, void *dest, u16 size, u16 unit);

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
    void *addr;                    // 0x00
    u16 size;                       // 0x04
    u16 status;                      // 0x06
    struct vram_tile_block *next;      // 0x08
    struct vram_tile_block *prev;       // 0x0C
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

extern struct vram_tile_block *gVramTileBlockPool; /* pool base */
extern struct vram_tile_block gVramTileBlockList;  /* address-sorted free-block list sentinel */
extern struct vram_tile_block *gVramTileBlockRover; /* next-fit search cursor ("rover") */
extern struct vram_tile_block *gVramTileBlockSpares; /* spare-record stack head */
extern u8 *gVramTileBlockIndex;                     /* tile-index -> pool-record-index lookup table, TOTAL_OBJ_TILE_COUNT bytes */

#define DMA3 (*(struct dma_regs *)REG_ADDR_DMA3SAD)

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
    u16 zero;

    gVramTileBlockPool = (struct vram_tile_block *)mem_alloc(
        sizeof(struct vram_tile_block) * VRAM_TILE_BLOCK_POOL_COUNT, MEM_HEAP_EWRAM);
    lookupSlot = &gVramTileBlockIndex;
    *lookupSlot = (u8 *)mem_alloc(TOTAL_OBJ_TILE_COUNT, MEM_HEAP_EWRAM);

    len = (s32)((u8 *)OBJ_VRAM0 + OBJ_VRAM0_SIZE - (u8 *)base);

    {
        register u16 *addr asm("r1");
        register u16 val asm("r0");

        addr = &zero;
        val = 0;
        *addr = val;
        DMA3.src = addr;
    }
    DMA3.dst = base;
    DMA3.cnt = (len / 2) | ((DMA_ENABLE | DMA_SRC_FIXED) << 16);
    (void)DMA3.cnt;

    gVramTileBlockSpares = gVramTileBlockPool;
    node = gVramTileBlockPool;
    i = VRAM_TILE_BLOCK_POOL_COUNT - 3;
    do {
        struct vram_tile_block *next = node + 1;
        node->next = next;
        node = next;
    } while (--i >= 0);
    node->next = NULL;
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
 * src/system/memory.c does, but against this allocator's own external
 * node pool instead of an embedded-in-buffer header - see the struct's
 * comment above). Reclaimed node records go back onto the
 * `gVramTileBlockSpares` spare stack. */
void FreeVramTileBlock(void *addr)
{
    struct vram_tile_block *node;
    struct vram_tile_block *adj;
    struct vram_tile_block **poolSlot;
    u32 index;
    register u8 byteVal asm("r0");
    register u32 shifted asm("r1");
    register u16 target_size asm("r4");
    register u16 other_size asm("r1");
    register u16 sum asm("r0");

    if (addr == NULL) {
        return;
    }

    index = GET_TILE_NUM(addr);
    poolSlot = &gVramTileBlockPool;
    byteVal = gVramTileBlockIndex[index];
    shifted = byteVal << 4;
    node = (struct vram_tile_block *)((u8 *)*poolSlot + shifted);
    node->status = VRAM_TILE_BLOCK_FREE;

    adj = node->prev;
    if (adj->status == VRAM_TILE_BLOCK_FREE) {
        struct vram_tile_block *tmp;

        target_size = adj->size;
        other_size = node->size;
        sum = target_size + other_size;
        adj->size = sum;
        tmp = node->next;
        adj->next = tmp;
        tmp->prev = adj;
        if (node == gVramTileBlockRover) {
            gVramTileBlockRover = adj;
        }
        node->next = gVramTileBlockSpares;
        gVramTileBlockSpares = node;
        node = adj;
    }

    adj = node->next;
    if (adj->status == VRAM_TILE_BLOCK_FREE) {
        struct vram_tile_block *tmp;

        target_size = node->size;
        other_size = adj->size;
        sum = target_size + other_size;
        node->size = sum;
        tmp = adj->next;
        node->next = tmp;
        tmp->prev = node;
        if (adj == gVramTileBlockRover) {
            gVramTileBlockRover = node;
        }
        adj->next = gVramTileBlockSpares;
        gVramTileBlockSpares = adj;
    }
}
