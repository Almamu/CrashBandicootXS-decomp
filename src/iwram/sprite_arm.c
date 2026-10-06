#include "core.h"
#include "match.h"
#include "iwram.h"
#include "gfx.h"
#include "actor_self.h"

/*
 * IWRAM 0x0300024C-0x030007CC (stored in ROM at 0x087E5830): the ARM
 * graphics routines of the IWRAM image crt0 copies to 0x03000000 at boot.
 * The Thumb code calls them through the function pointers in
 * src/iwram/iwram_data.c (gUnpackNibbleTilesFunc,
 * gDrawMirroredTilemapFunc, gHeapSortActorsByKeyFunc,
 * gUnpackRleSpriteFrameFunc and gLookupSpriteFrameCacheFunc).
 *
 * Built as ARM code with agbcc_arm (Makefile ARM_OBJS). UnpackNibbleTiles,
 * DrawMirroredTilemap and UnpackRleSpriteFrame match as plain C, and
 * HeapSortActorsByKey as C with two empty-asm barriers.
 * LookupSpriteFrameCache is parked: the ROM was built by an ARM gcc whose
 * output differs from agbcc_arm's, and for it that difference is provably
 * out of C's reach. See its comment and docs/matching/iwram-image.md.
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
 * 15 (colour 0 stays 0). Called by yeti_update.c/yeti_graphics.c. */
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
 * first. Called by cell_anim.c. */
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

/* a[i]->key > a[j]->key as a u8 1/0. The two results come in as
 * arguments: the ROM keeps 1 and 0 in two registers loaded before each
 * sift loop (`mov r3, one; movls r3, zero; cmp r3, #0`), and it only gets
 * them there when the caller's u8 locals `one`/`zero` are copied into
 * this function's u8 parameters; literal 0/1 returns fold to immediates.
 * Indices rather than pointers: each inlined `child + 1` argument is its
 * own pseudo, which is why the ROM reloads a[child + 1] for the second
 * test instead of reusing it. */
static inline u8 KeyGreater(struct sort_entry **a, s32 i, s32 j, u8 one, u8 zero)
{
    if (a[i]->key <= a[j]->key)
        return zero;
    return one;
}

/* gHeapSortActorsByKeyFunc(n, list): heapsorts `n` actor pointers into
 * ascending order of the u32 at +0x14. Called by actor_category_frame.c.
 * Both phases spell out the sift-down loop on the shared `root`/`child`
 * (an inline sift function allocates them to other registers). See
 * docs/matching/iwram-image.md, "Fourth pass". */
void HeapSortActorsByKey(s32 n, struct actor_self **list)
{
    struct sort_entry **a = (struct sort_entry **)list;
    s32 i, root, child;
    struct sort_entry *t;

    for (i = n / 2; i > 0;) {
        i--;
        for (root = i; (child = root * 2 + 1) < n; root = child) {
            /* Loop-scoped so loop.c hoists them to this loop's preheader. */
            u8 one = 1, zero = 0;

            if (child + 1 < n && KeyGreater(a, child + 1, child, one, zero) &&
                KeyGreater(a, child + 1, root, one, zero)) {
                child++;
            } else if (!KeyGreater(a, child, root, one, zero)) {
                break;
            }
            t = a[root];
            a[root] = a[child];
            a[child] = t;
        }
    }
    /* This and the barrier at the end of the loop body keep jump2 from
     * cross-jumping the second loop's entry test (`cmp r7, #1; ble`) and
     * its bottom test into each other; the ROM keeps both. */
    MATCH_BARRIER();
    while (n > 1) {
        n--;
        t = a[0];
        a[0] = a[n];
        a[n] = t;
        for (root = 0; (child = root * 2 + 1) < n; root = child) {
            u8 one = 1, zero = 0;

            if (child + 1 < n && KeyGreater(a, child + 1, child, one, zero) &&
                KeyGreater(a, child + 1, root, one, zero)) {
                child++;
            } else if (!KeyGreater(a, child, root, one, zero)) {
                break;
            }
            t = a[root];
            a[root] = a[child];
            a[child] = t;
        }
        MATCH_BARRIER();
    }
}

/* gUnpackRleSpriteFrameFunc(dst, frame): unpacks a frame's w*h tiles into `dst`,
 * zero runs with a DMA3 fill and literal runs with a DMA3 copy. Called
 * by polar_player.c, jetpack_spawn.c and company_logos.c. */
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

#if NON_MATCHING
/* (vramAddr - OBJ_VRAM0) >> 5, the OBJ tile index of a VRAM address.
 * OBJ_VRAM0 (0x06010000) is subtracted in two statements, each a valid
 * ARM immediate. Written as one expression, fold-const keeps the
 * -0x06010000 whole, addsi3 has arm_split_constant build it in a
 * register (with -fexpensive-optimizations preserve_subexpressions_p()
 * is always true), and loop.c hoists that register out of both loops.
 * As two statements combine merges them into one `plus` that
 * *addsi3_insn accepts ("?n"), and the post-reload split prints it in
 * place as the ROM's `add #0xF9000000; add #0xFF0000`. */
static inline s32 ObjTileIndex(u32 vramAddr)
{
    vramAddr -= 0x07000000;
    vramAddr += 0xFF0000;
    return vramAddr >> 5;
}

/* gLookupSpriteFrameCacheFunc(frame), LoadSpriteFrameTiles's override hook (see
 * sprite_frame.c): returns the OBJ tile index of `frame` if it is
 * already in VRAM, or -1. Hits in this frame's list are returned as they
 * are; a hit in last frame's list is moved to the head of this frame's
 * list first.
 *
 * Parked, six instructions short (fifth pass). Everything but the
 * returns matches under the object's flags: 44 of the 50 instructions,
 * same registers, schedule and literal pool. The ROM returns with
 * `ldmfd sp!, {lr}; bx lr` three times; agbcc_arm prints
 * `ldmfd sp!, {ip}; bx ip` at each. Three copies of the exit sequence
 * mean three `return` insns (the text epilogue, which does print
 * `ldmfd sp!, {lr}; bx lr`, is printed once per function), and
 * agbcc_arm's output_return_instruction pops an interworking return
 * without a frame pointer into ip (arm.c: `if (TARGET_THUMB_INTERWORK
 * && really_return) strcat (instr, reg_names[12])`, then
 * `bx ... ip`). See docs/matching/iwram-image.md. */
s32 LookupSpriteFrameCache(u8 *frame)
{
    struct sprite_frame_cache_node *node;

    for (node = gSpriteFrameCacheCurrent.next; node != &gSpriteFrameCacheCurrent;
         node = node->next) {
        if (node->frame == frame)
            return ObjTileIndex((u32)node->vramAddr);
    }
    for (node = gSpriteFrameCachePrevious.next; node != &gSpriteFrameCachePrevious;
         node = node->next) {
        if (node->frame == frame) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            node->prev = &gSpriteFrameCacheCurrent;
            node->next = gSpriteFrameCacheCurrent.next;
            gSpriteFrameCacheCurrent.next->prev = node;
            gSpriteFrameCacheCurrent.next = node;
            return ObjTileIndex((u32)node->vramAddr);
        }
    }
    return -1;
}
#else
NAKED s32 LookupSpriteFrameCache(u8 *frame)
{
    // clang-format off
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
    // clang-format on
}
#endif
