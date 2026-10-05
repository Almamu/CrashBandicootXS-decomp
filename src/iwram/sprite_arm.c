#include "core.h"

/*
 * IWRAM 0x0300024C-0x030007CC (stored in ROM at 0x087E5830): the ARM
 * graphics routines of the IWRAM image crt0 copies to 0x03000000 at boot.
 * The Thumb code calls them through the function pointers in
 * src/iwram/iwram_data.c (gUnpackNibbleTilesFunc,
 * gDrawMirroredTilemapFunc, gHeapSortActorsByKeyFunc,
 * gUnpackRleSpriteFrameFunc and gLookupSpriteFrameCacheFunc).
 *
 * Built as ARM code with agbcc_arm (Makefile ARM_OBJS). UnpackNibbleTiles,
 * DrawMirroredTilemap and UnpackRleSpriteFrame match as plain C.
 * HeapSortActorsByKey and LookupSpriteFrameCache are parked. The ROM was
 * built by an ARM gcc whose output differs from agbcc_arm's. For
 * LookupSpriteFrameCache that difference is provably out of C's reach;
 * for HeapSortActorsByKey no C form found so far reaches it. See their
 * comments and docs/matching/iwram-image.md.
 */

static inline u32 ExpandNibble(u32 nibble)
{
    return nibble ? nibble | 0xF0 : 0;
}

/* gUnpackNibbleTilesFunc(src, lowBlock): decodes a zero-run-compressed 4bpp
 * picture into 8bpp tiles in BG VRAM (0x06008000 when `lowBlock`, else
 * 0x0600A000; 0x1900 bytes). The stream alternates a u16 count of zero
 * words, filled by DMA3, and a u16 count of literal halfwords, each
 * holding four 4bpp pixels that become four 8bpp pixels in palette row
 * 15 (colour 0 stays 0). Called by actor_part74.c/actor_part75.c. */
void UnpackNibbleTiles(u16 *src, s32 lowBlock)
{
    u32 *dst = lowBlock ? (u32 *)(BG_VRAM + 0x8000) : (u32 *)(BG_VRAM + 0xA000);
    u32 *end = (u32 *)((u8 *)dst + 0x1900);

    while (dst != end) {
        u32 n = *src++;
        DmaFill16(3, 0, dst, n * 4);
        dst += n;
        if (dst == end)
            break;
        n = *src++;
        if (n != 0) {
            u32 i = n;
            do {
                u32 v = *src++;
                u32 p0 = ExpandNibble(v & 15);
                u32 p1 = ExpandNibble((u16)v >> 4 & 15);
                u32 p2 = ExpandNibble((u16)v >> 8 & 15);
                u32 p3 = ExpandNibble((u16)v >> 12);
                *dst++ = p0 | (p1 << 8) | (p2 << 16) | (p3 << 24);
            } while (--i != 0);
        }
    }
}

/* gDrawMirroredTilemapFunc(pal, lowBlock, w, h): fills a w x h block of tilemap
 * entries in screen block 30 (`lowBlock`, tiles from 0) or 31 (tiles
 * from 0x100) and mirrors it three times: flipped vertically below it,
 * horizontally to its right (running into the next screen block past
 * column 31) and both ways diagonally. Tile numbers count up from the
 * start; `pal` holds one 4-bit palette number per tile, low nibble
 * first. Called by actor_part95.c. */
void DrawMirroredTilemap(u8 *pal, s32 lowBlock, s32 w, s32 h)
{
    u16 *top, *bottom;
    s32 x, y, tile;

    top = lowBlock ? (u16 *)BG_SCREEN_ADDR(28) : (u16 *)BG_SCREEN_ADDR(30);
    tile = lowBlock ? 0 : 0x100;
    bottom = top + (h * 64 - 32);

    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            s32 entry, mirrorX;

            if (tile & 1)
                entry = *pal++ >> 4;
            else
                entry = *pal & 15;
            entry = tile | (entry << 12);
            tile++;
            top[x] = entry;
            bottom[x] = entry | 0x800;
            mirrorX = 2 * w - 1 - x;
            if (mirrorX > 31)
                mirrorX += 992;
            top[mirrorX] = entry | 0x400;
            bottom[mirrorX] = entry | 0xC00;
        }
        top += 32;
        bottom -= 32;
    }
}

/* The only field the sort looks at. */
struct sort_entry {
    u8 unk_00[0x14];
    u32 key;
};

#if NON_MATCHING
static inline u8 KeyGreater(struct sort_entry *a, struct sort_entry *b)
{
    if (a->key <= b->key)
        return 0;
    return 1;
}

static inline void SiftDown(struct sort_entry **a, s32 root, s32 n)
{
    s32 child, right;
    struct sort_entry *t;

    child = root * 2 + 1;
    while (child < n) {
        right = child + 1;
        if (right < n && KeyGreater(a[right], a[child]) && KeyGreater(a[right], a[root])) {
            child = right;
        } else if (!KeyGreater(a[child], a[root])) {
            break;
        }
        t = a[root];
        a[root] = a[child];
        a[child] = t;
        root = child;
        child = root * 2 + 1;
    }
}

/* gHeapSortActorsByKeyFunc(n, list): heapsorts `n` actor pointers into
 * ascending order of the u32 at +0x14. Called by actor_part103.c.
 *
 * Parked. Same control flow and the same loads, but the ROM tests each
 * comparison as a materialized 1/0 held in two registers hoisted out of
 * the sift loop (`mov r3, r9; movls r3, sl; cmp r3, #0`, the constants
 * in r9/sl, or in phase two in registers CSE found already holding 1
 * and 0), and reloads a[root]/a[right] across the tests. agbcc_arm either
 * folds the flag into the branches (an int result) or keeps it but with
 * immediates (`mov r3, #1; movls r3, #0`, a u8 result, as here); no
 * return type, flag variable or cast tried reproduces the hoisted
 * constants. A second pass also tried `one`/`zero` locals, which jump.c
 * turns into a conditional move with register arms. Only the arm that
 * stays a register is hoisted; jump.c's if-conversion folds the other
 * to an immediate. `asm("" : "=r"(one) : "0"(1))` constants (all
 * three tests materialized as `movls rX, zero; movhi rX, one`, but not
 * hoisted and in the wrong order) and sweeps of -O1/-O2/-O3 with ~50
 * single flags also failed. The same compiler built itoa_arm and
 * LookupSpriteFrameCache, which provably aren't agbcc_arm output. */
void HeapSortActorsByKey(s32 n, struct sort_entry **a)
{
    s32 i;
    struct sort_entry *t;

    for (i = n / 2; i > 0; i--)
        SiftDown(a, i - 1, n);
    while (n > 1) {
        n--;
        t = a[0];
        a[0] = a[n];
        a[n] = t;
        SiftDown(a, 0, n);
    }
}
#else
NAKED void HeapSortActorsByKey(s32 n, struct sort_entry **a)
{
    asm(".syntax unified\n"
        "\tpush {r4, r5, r6, r7, r8, r9, r10, lr}\n"
        "\tmov r7, r0\n"
        "\tadd r3, r7, r7, lsr #31\n"
        "\tasr r8, r3, #1\n"
        "\tcmp r8, #0\n"
        "\tmov lr, r1\n"
        "\tble .L03000554\n"
        ".L03000490:\n"
        "\tsub r4, r8, #1\n"
        "\tmov r8, r4\n"
        "\tlsl r3, r4, #1\n"
        "\tadd r12, r3, #1\n"
        "\tcmp r12, r7\n"
        "\tbge .L0300054C\n"
        "\tmov r9, #1\n"
        "\tmov r10, #0\n"
        ".L030004B0:\n"
        "\tadd r3, r12, #1\n"
        "\tldr r6, [lr, r12, lsl #2]\n"
        "\tcmp r3, r7\n"
        "\tldr r5, [lr, r4, lsl #2]\n"
        "\tmov r0, r3\n"
        "\tbge .L03000510\n"
        "\tldr r3, [lr, r0, lsl #2]\n"
        "\tldr r2, [r6, #20]\n"
        "\tldr r1, [r3, #20]\n"
        "\tcmp r1, r2\n"
        "\tmov r3, r9\n"
        "\tmovls r3, r10\n"
        "\tldr r5, [lr, r4, lsl #2]\n"
        "\tcmp r3, #0\n"
        "\tbeq .L03000510\n"
        "\tldr r3, [lr, r0, lsl #2]\n"
        "\tldr r2, [r5, #20]\n"
        "\tldr r1, [r3, #20]\n"
        "\tcmp r1, r2\n"
        "\tmov r3, r9\n"
        "\tmovls r3, r10\n"
        "\tcmp r3, #0\n"
        "\tmovne r12, r0\n"
        "\tbne .L0300052C\n"
        ".L03000510:\n"
        "\tldr r2, [r6, #20]\n"
        "\tldr r3, [r5, #20]\n"
        "\tcmp r2, r3\n"
        "\tmov r3, r9\n"
        "\tmovls r3, r10\n"
        "\tcmp r3, #0\n"
        "\tbeq .L0300054C\n"
        ".L0300052C:\n"
        "\tldr r3, [lr, r12, lsl #2]\n"
        "\tstr r3, [lr, r4, lsl #2]\n"
        "\tmov r4, r12\n"
        "\tstr r5, [lr, r12, lsl #2]\n"
        "\tlsl r3, r4, #1\n"
        "\tadd r12, r3, #1\n"
        "\tcmp r12, r7\n"
        "\tblt .L030004B0\n"
        ".L0300054C:\n"
        "\tcmp r8, #0\n"
        "\tbgt .L03000490\n"
        ".L03000554:\n"
        "\tcmp r7, #1\n"
        "\tble .L0300062C\n"
        ".L0300055C:\n"
        "\tldr r2, [lr]\n"
        "\tsub r7, r7, #1\n"
        "\tldr r3, [lr, r7, lsl #2]\n"
        "\tmov r4, #0\n"
        "\tstr r3, [lr]\n"
        "\tadd r12, r4, #1\n"
        "\tstr r2, [lr, r7, lsl #2]\n"
        "\tcmp r12, r7\n"
        "\tbge .L03000624\n"
        "\tmov r10, r12\n"
        "\tmov r8, r4\n"
        ".L03000588:\n"
        "\tadd r3, r12, #1\n"
        "\tldr r6, [lr, r12, lsl #2]\n"
        "\tcmp r3, r7\n"
        "\tldr r5, [lr, r4, lsl #2]\n"
        "\tmov r0, r3\n"
        "\tbge .L030005E8\n"
        "\tldr r3, [lr, r0, lsl #2]\n"
        "\tldr r2, [r6, #20]\n"
        "\tldr r1, [r3, #20]\n"
        "\tcmp r1, r2\n"
        "\tmov r3, r10\n"
        "\tmovls r3, r8\n"
        "\tldr r5, [lr, r4, lsl #2]\n"
        "\tcmp r3, #0\n"
        "\tbeq .L030005E8\n"
        "\tldr r3, [lr, r0, lsl #2]\n"
        "\tldr r2, [r5, #20]\n"
        "\tldr r1, [r3, #20]\n"
        "\tcmp r1, r2\n"
        "\tmov r3, r10\n"
        "\tmovls r3, r8\n"
        "\tcmp r3, #0\n"
        "\tmovne r12, r0\n"
        "\tbne .L03000604\n"
        ".L030005E8:\n"
        "\tldr r2, [r6, #20]\n"
        "\tldr r3, [r5, #20]\n"
        "\tcmp r2, r3\n"
        "\tmov r3, r10\n"
        "\tmovls r3, r8\n"
        "\tcmp r3, #0\n"
        "\tbeq .L03000624\n"
        ".L03000604:\n"
        "\tldr r3, [lr, r12, lsl #2]\n"
        "\tstr r3, [lr, r4, lsl #2]\n"
        "\tmov r4, r12\n"
        "\tstr r5, [lr, r12, lsl #2]\n"
        "\tlsl r3, r4, #1\n"
        "\tadd r12, r3, #1\n"
        "\tcmp r12, r7\n"
        "\tblt .L03000588\n"
        ".L03000624:\n"
        "\tcmp r7, #1\n"
        "\tbgt .L0300055C\n"
        ".L0300062C:\n"
        "\tpop {r4, r5, r6, r7, r8, r9, r10, lr}\n"
        "\tbx lr\n"
        ".syntax divided\n");
}
#endif

/* A zero-run-compressed OBJ frame (docs/data.md "Compressed sprite
 * frames"): the size in tiles, then u16 counts alternating between a run
 * of zero halfwords and a run of literal halfwords that follow it. */
struct rle_frame {
    u8 w;
    u8 h;
    u8 unk_2;
    u8 unk_3;
    u16 data[0];
};

/* gUnpackRleSpriteFrameFunc(dst, frame): unpacks a frame's w*h tiles into `dst`,
 * zero runs with a DMA3 fill and literal runs with a DMA3 copy. Called
 * by actor_part127.c, actor_part128.c and graphics_loading_3686c.c. */
void UnpackRleSpriteFrame(u16 *dst, struct rle_frame *frame)
{
    u16 *end = dst + frame->h * frame->w * 16;
    u16 *src = frame->data;

    while (dst != end) {
        u32 n = *src++;
        DmaFill16(3, 0, dst, n * 2);
        dst += n;
        if (dst == end)
            break;
        n = *src++;
        DmaCopy16(3, src, dst, n * 2);
        src += n;
        dst += n;
    }
}

/* Same layout as sprite_frame.c's copy. */
struct sprite_frame_cache_node {
    struct sprite_frame_cache_node *next;
    struct sprite_frame_cache_node *prev;
    u8 *frame;
    void *vramAddr;
};

extern struct sprite_frame_cache_node gSpriteFrameCacheCurrent;
extern struct sprite_frame_cache_node gSpriteFrameCachePrevious;

#define OBJ_TILE_INDEX(addr) (((u32)(addr) - (u32)OBJ_VRAM0) >> 5)

#if NON_MATCHING
/* gLookupSpriteFrameCacheFunc(frame), LoadSpriteFrameTiles's override hook (see
 * sprite_frame.c): returns the OBJ tile index of `frame` if it is
 * already in VRAM, or -1. Hits in this frame's list are returned as they
 * are; a hit in last frame's list is moved to the head of this frame's
 * list first.
 *
 * Parked. The ROM computes the tile index in place at each return
 * (`add #0xF9000000; add #0xFF0000`) and returns with
 * `ldmfd sp!, {lr}; bx lr` three times. agbcc_arm hoists the
 * -0x06010000 into a register before the first loop (with
 * -fno-expensive-optimizations only the second loop stays in place).
 * The returns are out of its reach. Three copies of the exit sequence
 * mean three `return` insns (the text epilogue is printed once), and
 * agbcc_arm's output_return_instruction pops an interworking return into
 * ip: with only lr saved it prints `ldmfd sp!, {ip}; bx ip`
 * (`ldmeqfd`/`bxeq` when conditional). It never prints
 * `ldmfd sp!, {lr}; bx lr`. Checked by compiling this draft: with
 * -fno-expensive-optimizations it saves only lr and prints exactly
 * that. */
s32 LookupSpriteFrameCache(u8 *frame)
{
    struct sprite_frame_cache_node *node;

    for (node = gSpriteFrameCacheCurrent.next; node != &gSpriteFrameCacheCurrent; node = node->next) {
        if (node->frame == frame)
            return OBJ_TILE_INDEX(node->vramAddr);
    }
    for (node = gSpriteFrameCachePrevious.next; node != &gSpriteFrameCachePrevious; node = node->next) {
        if (node->frame == frame) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            node->prev = &gSpriteFrameCacheCurrent;
            node->next = gSpriteFrameCacheCurrent.next;
            gSpriteFrameCacheCurrent.next->prev = node;
            gSpriteFrameCacheCurrent.next = node;
            return OBJ_TILE_INDEX(node->vramAddr);
        }
    }
    return -1;
}
#else
NAKED s32 LookupSpriteFrameCache(u8 *frame)
{
    asm(".syntax unified\n"
        "\tstmfd sp!, {lr}\n"
        "\tldr r3, .L030007C4 @ =gSpriteFrameCacheCurrent\n"
        "\tldr r12, [r3]\n"
        "\tcmp r12, r3\n"
        "\tbeq .L03000744\n"
        "\tmov r2, r3\n"
        ".L03000714:\n"
        "\tldr r3, [r12, #8]\n"
        "\tcmp r3, r0\n"
        "\tbne .L03000738\n"
        "\tldr r0, [r12, #12]\n"
        "\tadd r0, r0, #-117440512\n"
        "\tadd r0, r0, #16711680\n"
        "\tlsr r0, r0, #5\n"
        "\tldmfd sp!, {lr}\n"
        "\tbx lr\n"
        ".L03000738:\n"
        "\tldr r12, [r12]\n"
        "\tcmp r12, r2\n"
        "\tbne .L03000714\n"
        ".L03000744:\n"
        "\tldr r3, .L030007C8 @ =gSpriteFrameCachePrevious\n"
        "\tldr r12, [r3]\n"
        "\tcmp r12, r3\n"
        "\tbeq .L030007B8\n"
        "\tmov r2, r3\n"
        "\tldr lr, .L030007C4 @ =gSpriteFrameCacheCurrent\n"
        ".L0300075C:\n"
        "\tldr r3, [r12, #8]\n"
        "\tcmp r3, r0\n"
        "\tbne .L030007AC\n"
        "\tldr r2, [r12, #4]\n"
        "\tldr r3, [r12]\n"
        "\tldr r0, [r12, #12]\n"
        "\tstr r3, [r2]\n"
        "\tldm r12, {r1, r3}\n"
        "\tstr r3, [r1, #4]\n"
        "\tstr lr, [r12, #4]\n"
        "\tldr r3, [lr]\n"
        "\tstr r3, [r12]\n"
        "\tldr r3, [lr]\n"
        "\tadd r0, r0, #-117440512\n"
        "\tstr r12, [r3, #4]\n"
        "\tadd r0, r0, #16711680\n"
        "\tstr r12, [lr]\n"
        "\tlsr r0, r0, #5\n"
        "\tldmfd sp!, {lr}\n"
        "\tbx lr\n"
        ".L030007AC:\n"
        "\tldr r12, [r12]\n"
        "\tcmp r12, r2\n"
        "\tbne .L0300075C\n"
        ".L030007B8:\n"
        "\tmvn r0, #0\n"
        "\tldmfd sp!, {lr}\n"
        "\tbx lr\n"
        ".L030007C4: .4byte gSpriteFrameCacheCurrent\n"
        ".L030007C8: .4byte gSpriteFrameCachePrevious\n"
        ".syntax divided\n");
}
#endif
