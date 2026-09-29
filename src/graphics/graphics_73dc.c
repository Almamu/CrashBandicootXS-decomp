#include "core.h"

/* sub_80073DC (0x080073DC-0x08007634), the plain (non-affine) sibling of
 * `sub_8007634` (graphics_7634.c). Split out of graphics.c: it is the
 * last function there, and it needs old_agbcc (Makefile OLD_AGBCC_OBJS)
 * while the rest of graphics.c is built with agbcc - the ROM loads the
 * piece id's `0xf` mask before the `ldrb` it is combined with.
 *
 * Queues one OAM entry per visible sub-piece of an animated `part`
 * (mirrored per the part's flag bits) and sends the part's whole tile
 * total to `sub_8006C84` once after the loop. */

struct oam_attr01 {
    u32 y:8;
    u32 objMode:2;
    u32 gfxMode:2;
    u32 mosaic:1;
    u32 colorMode:1;
    u32 shape:2;
    u32 x:9;
    u32 unused:3;
    u32 hflip:1;
    u32 vflip:1;
    u32 size:2;
};

/* u16 fields: the tile store then masks with lsl/lsr #22 as in the ROM. */
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

struct part_method73dc {
    s16 thisOffset;
    u8 unk_02[2];
    void *fn;
};

struct oam_part {
    u8 unk_00[0x18];
    u8 *vtable;                 // 0x18
    u8 unk_1C[0xC];
    u8 gfxMode:2;               // 0x28
    u8 mosaic:1;
    u8 colorMode:1;
    u8 mirrorX:1;
    u8 mirrorY:1;
    u8 flagsHi:2;
    u8 palette:4;               // 0x29
    u8 paletteHi:4;
};

/* The 0x28 flag bits tested as sign tests (`lsl #N; cmp #0; bge`). */
#define PART_FLAG_SET(part, shift) ((s32)(*((u8 *)(part) + 0x28) << (shift)) < 0)

extern struct piece_info *sub_80083B8(void *part);
extern s32 sub_80083A8(void *part);
extern s32 sub_8006C44(void *cursor);
extern s32 sub_8006C84(void *cursor, s32 src, s32 size);
extern void sub_8006AC8(void *buffer, void *record);
extern s32 sub_803AD7C(void *self, void *fn);
extern void *gUnknown_030012FC;
extern void *gUnknown_03001300;
extern u8 gStaticData_0816B2E0[];
extern u8 gStaticData_0816B2EC[];

static inline s32 PieceSize73DC(s32 id)
{
    return id & 3;
}

static inline s32 PieceShape73DC(s32 id)
{
    return (id >> 2) & 3;
}

void sub_80073DC(void *unused, struct oam_part *part, s32 *pos)
{
    struct oam_pair oam;
    s32 total = 0;
    struct piece_info *info = sub_80083B8(part);
    s32 tile = sub_8006C44(gUnknown_030012FC);
    s32 i;

    oam.a.objMode = 0;
    oam.a.gfxMode = part->gfxMode;
    oam.a.mosaic = part->mosaic;
    oam.a.colorMode = part->colorMode;
    {
        struct part_method73dc *m = (struct part_method73dc *)(part->vtable + 0x58);

        oam.b.priority = (u16)sub_803AD7C((u8 *)part + m->thisOffset, m->fn);
    }
    oam.b.palette = part->palette;
    if (PART_FLAG_SET(part, 27))
        oam.a.hflip = 1;
    else
        oam.a.hflip = 0;
    if (PART_FLAG_SET(part, 26))
        oam.a.vflip = 1;
    else
        oam.a.vflip = 0;

    for (i = 0; i != info->u.b.count; i++) {
        s32 id = info->ids[i] & 0xf;
        s32 w = gStaticData_0816B2E0[id];
        s32 h = gStaticData_0816B2EC[id];
        s32 x, y;
        s32 tiles;

        if (PART_FLAG_SET(part, 26)) // mirror Y
            y = pos[1] - h - info->offsets[i].y;
        else
            y = pos[1] + info->offsets[i].y;
        if (y + h > 0 && y <= 0x9f) {
            if (PART_FLAG_SET(part, 27)) // mirror X
                x = pos[0] - w - info->offsets[i].x;
            else
                x = pos[0] + info->offsets[i].x;
            if (x + w > 0 && x <= 0xef) {
                oam.a.y = y;
                oam.a.shape = PieceShape73DC(id);
                oam.a.x = x;
                oam.a.size = PieceSize73DC(id);
                oam.b.tile = tile;
                sub_8006AC8(gUnknown_03001300, &oam);
            }
        }
        tiles = (w >> 3) * (h >> 3);
        if (PART_FLAG_SET(part, 28)) // 8bpp: twice the tiles
            tiles *= 2;
        tile += tiles;
        total += tiles << 5;
    }
    sub_8006C84(gUnknown_030012FC, sub_80083A8(part) + (info->u.packed & 0xffffff), total);
}
