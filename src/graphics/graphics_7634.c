#include "core.h"

/* GitHub issue #9: sub_8007634 (0x08007634-0x08007A48), the affine
 * (rotation/scaling) sibling of `sub_80073DC` (graphics.c) - see
 * docs/matching/issue-9-0x08007634-actor.md. Moved here from
 * asm/code_3_2.s in the issue #9 raw-asm pass.
 *
 * Queues one affine OAM entry per visible sub-piece of an animated
 * `part`, scaled by `part+0x3c` (Q8, at least 0x40). It allocates an
 * affine matrix slot from `gUnknown_03001300->count` (written as a plain
 * scale into the slot's pa/pd, via `sub_800090C`), selects OBJ mode 1
 * (affine) or 3 (double size, scale > 0x100), and pulls every piece
 * towards the first piece's centre by the scale factor. To find that
 * centre it probes keyframe 0 through `sub_80083B8` with `part->tick`
 * temporarily clamped to 0. The tile total goes to `sub_8006C84` once
 * after the loop.
 *
 * Built with old_agbcc (Makefile OLD_AGBCC_OBJS): the ROM loads the 0xf
 * mask before the `ldrb` it is combined with. The ROM keeps nearly every
 * local in a 0x48-byte frame; those are register-allocator spills, and
 * the source shapes below are what reproduce them. See
 * docs/matching/graphics-7634-retry.md. */

struct oam_attr01 {
    u32 y:8;
    u32 objMode:2;
    u32 gfxMode:2;
    u32 mosaic:1;
    u32 colorMode:1;
    u32 shape:2;
    u32 x:9;
    u32 matrix:3;
    u32 matrixHi:1;
    u32 matrixTop:1;
    u32 size:2;
};

/* u16 fields: the tile store then masks with lsl/lsr #22 as in the ROM
 * (a u32 field loads a 0x3ff constant instead). */
struct oam_attr2 {
    u16 tile:10;
    u16 priority:2;
    u16 palette:4;
    u16 unused:16;
};

struct oam_pair {
    struct oam_attr01 a;
    struct oam_attr2 b;
};

struct affine_oam {
    u16 attr[3];
    s16 param;
};

struct oam_buffer {
    u8 unk_00[8];
    s32 count;                  // 0x08 - next free affine matrix
    struct affine_oam oam[128]; // 0x0C - param of entry n at +0x12 + 8n
};

struct piece_offset {
    s16 x;
    s16 y;
};

struct piece_info {
    struct piece_offset *offsets; // 0x00
    u8 *ids;                    // 0x04 - low 4 bits: shape/size index
    union {
        u32 packed;             // 0x08 - low 24 bits: VRAM source offset
        struct {
            u8 src[3];
            u8 count;           // 0x0B - piece count
        } b;
    } u;
};

struct kf_record {
    u8 unk_00[0x16];
    u8 steps;                   // 0x16
    u8 unk_17[5];
};

struct part_method7634 {
    s16 thisOffset;
    u8 unk_02[2];
    void *fn;
};

struct affine_part {
    u8 unk_00[0x18];
    u8 *vtable;                 // 0x18
    u8 unk_1C[4];
    struct kf_record **keyframes; // 0x20
    u8 unk_24[4];
    u8 gfxMode:2;               // 0x28
    u8 mosaic:1;
    u8 colorMode:1;
    u8 mirrorX:1;
    u8 mirrorY:1;
    u8 flagsHi:2;
    u8 palette:4;               // 0x29
    u8 paletteHi:4;
    u8 unk_2A[3];
    u8 frame;                   // 0x2D
    u8 unk_2E[2];
    s32 tick;                   // 0x30
    u8 unk_34[8];
    u16 scale;                  // 0x3C
};

/* The loop tests the 0x28 flag bits as sign tests (`lsl #26; cmp #0;
 * bge`); a 1-bit field test compiles to `movs #0x20; ands` instead. */
#define PART_FLAG_SET(part, shift) ((s32)(*((u8 *)(part) + 0x28) << (shift)) < 0)

extern struct piece_info *sub_80083B8(void *part);
extern s32 sub_80083A8(void *part);
extern s32 sub_8006C44(void *cursor);
extern s32 sub_8006C84(void *cursor, s32 src, s32 size);
extern void sub_8006AC8(void *buffer, void *record);
extern s32 sub_800090C(s32 scale);
extern s32 sub_803AD7C(void *self, void *fn);
extern void *gUnknown_030012FC;
extern struct oam_buffer *gUnknown_03001300;
extern u8 gStaticData_0816B2E0[];
extern u8 gStaticData_0816B2EC[];

/* Shape/size index -> OBJ shape and size. As inline helpers the `& 3`
 * masks survive tree folding and share one `movs #3`, which is what
 * keeps the ROM's `asrs; ands` and `ands; lsls #30`. */
static inline s32 PieceSize7634(s32 id)
{
    return id & 3;
}

static inline s32 PieceShape7634(s32 id)
{
    return (id >> 2) & 3;
}

static inline void ClampTick7634(struct affine_part *part, s32 t)
{
    s32 n = (*part->keyframes)[part->frame].steps;

    if (t >= n)
        t = n - 1;
    part->tick = t;
}

void sub_8007634(void *unused, struct affine_part *part, s32 *pos)
{
    struct oam_pair oam;
    s32 total = 0;
    struct piece_info *info = sub_80083B8(part);
    s32 tile = sub_8006C44(gUnknown_030012FC);
    s32 scale = part->scale;
    u16 pa;
    u16 pd;
    s32 idx;
    s32 k;
    struct oam_buffer *buf;
    s32 baseX, baseY, halfW, halfH, pullX, pullY;
    s32 i;

    if (scale < 0x40)
        scale = 0x40;
    pa = sub_800090C((s16)scale);
    pd = pa;
    if (scale <= 0x100)
        oam.a.objMode = 1;
    else
        oam.a.objMode = 3;
    buf = gUnknown_03001300;
    idx = buf->count++;
    oam.a.matrix = idx;
    oam.a.matrixHi = idx >> 3;
    oam.a.matrixTop = idx >> 4;
    k = idx * 4;
    buf->oam[k].param = pa;
    buf->oam[k + 1].param = 0;
    buf->oam[k + 2].param = 0;
    buf->oam[k + 3].param = pd;
    /* Extra reference, no code: keeps `pa` live past `pd`, so cse2 leaves
     * the call result in `pa` (r6) and `pd` as the copy (r7), as in the
     * ROM. Without it the two registers swap. */
    asm("" : : "r"(pa));
    oam.a.gfxMode = part->gfxMode;
    oam.a.mosaic = part->mosaic;
    oam.a.colorMode = part->colorMode;
    {
        struct part_method7634 *m = (struct part_method7634 *)(part->vtable + 0x58);

        oam.b.priority = (u16)sub_803AD7C((u8 *)part + m->thisOffset, m->fn);
    }
    oam.b.palette = part->palette;

    baseX = 0;
    baseY = 0;
    halfW = 0;
    halfH = 0;
    pullX = 0;
    pullY = 0;
    for (i = 0; i != info->u.b.count; i++) {
        s32 id = info->ids[i] & 0xf;
        s32 w = gStaticData_0816B2E0[id];
        s32 h = gStaticData_0816B2EC[id];
        s32 ox = info->offsets[i].x;
        s32 oy = info->offsets[i].y;
        s32 tiles = (w >> 3) * (h >> 3);
        s32 x, y;

        /* Rescaled in place, so w/h and their scaled sizes share r8/sb. */
        w = ((w << 8) * scale) >> 16;
        h = ((h << 8) * scale) >> 16;
        if (PART_FLAG_SET(part, 26)) // mirror Y
            y = pos[1] - h - oy;
        else
            y = pos[1] + oy;
        if (y + h > 0 && y <= 0x9f) {
            if (PART_FLAG_SET(part, 27)) // mirror X
                x = pos[0] - w - ox;
            else
                x = pos[0] + ox;
            if (x + w > 0 && x <= 0xef) {
                s32 saved = part->tick;
                struct piece_info *first;
                s32 cx, cy;
                s32 id0;
                u8 *pw, *ph;

                ClampTick7634(part, 0);
                first = sub_80083B8(part);
                ClampTick7634(part, saved);
                /* The size-table addresses are taken before `pos` is read;
                 * that keeps r0-r4 busy at the `pos` reload, which makes
                 * reload spill r7 and sets the ROM's reload-register
                 * rotation for the whole function. */
                cx = first->offsets[0].x;
                cy = first->offsets[0].y;
                id0 = *first->ids & 0xf;
                pw = &gStaticData_0816B2E0[id0];
                ph = &gStaticData_0816B2EC[id0];
                cx += pos[0];
                cy += pos[1];
                cx += *pw >> 1;
                cy += *ph >> 1;
                if (i == 0) {
                    pullX = x + (gStaticData_0816B2E0[id] >> 1);
                    pullY = y + (gStaticData_0816B2EC[id] >> 1);
                    pullX -= cx;
                    pullY -= cy;
                    pullX = ((pullX << 8) * scale) >> 16;
                    pullY = ((pullY << 8) * scale) >> 16;
                    pullX = -pullX;
                    pullY = -pullY;
                    x += pullX;
                    y += pullY;
                    baseX = x;
                    baseY = y;
                    halfW = w >> 1;
                    halfH = h >> 1;
                } else {
                    s32 dx, dy;

                    x += pullX;
                    y += pullY;
                    dx = x - baseX;
                    dy = y - baseY;
                    dx = ((dx << 8) * scale) >> 16;
                    dy = ((dy << 8) * scale) >> 16;
                    dx += halfW;
                    dy += halfH;
                    dx -= w >> 1;
                    dy -= h >> 1;
                    x = baseX + dx;
                    y = baseY + dy;
                }
                oam.a.y = y;
                oam.a.shape = PieceShape7634(id);
                oam.a.x = x;
                oam.a.size = PieceSize7634(id);
                oam.b.tile = tile;
                sub_8006AC8(gUnknown_03001300, &oam);
            }
        }
        if (PART_FLAG_SET(part, 28)) // 8bpp: twice the tiles
            tiles *= 2;
        tile += tiles;
        total += tiles << 5;
    }
    sub_8006C84(gUnknown_030012FC, sub_80083A8(part) + (info->u.packed & 0xffffff), total);
}
