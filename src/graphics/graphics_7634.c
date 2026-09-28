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
 * NAKED: the NON_MATCHING draft below follows the ROM block for block,
 * but its register allocation and stack frame differ (468 halfwords off;
 * 1032 bytes under agbcc and 1024 under old_agbcc, against the ROM's
 * 1044). The ROM keeps nearly every local in a 0x48-byte frame. A
 * single frame struct at the ROM's offsets was tried and is worse (481
 * under old_agbcc): the ROM's per-use reloads are spilled pseudos, not
 * memory locals (docs/matching/last-four-naked-retry.md). */

#if NON_MATCHING
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

struct oam_attr2 {
    u32 tile:10;
    u32 priority:2;
    u32 palette:4;
    u32 unused:16;
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
    u32 packed;                 // 0x08 - low 24 bits: VRAM source offset
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
    u8 flags;                   // 0x28 - bits 0-1 gfx mode, 2 mosaic, 3 color, 4 mirror X, 5 mirror Y
    u8 palette;                 // 0x29 - low 4 bits
    u8 unk_2A[3];
    u8 frame;                   // 0x2D
    u8 unk_2E[2];
    s32 tick;                   // 0x30
    u8 unk_34[8];
    u16 scale;                  // 0x3C
};

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
    u16 param;
    s32 idx;
    struct oam_buffer *buf;
    u8 *flagsPtr;
    s32 baseX = 0, baseY = 0, halfW = 0, halfH = 0, pullX = 0, pullY = 0;
    s32 i;

    if (scale < 0x40)
        scale = 0x40;
    param = sub_800090C((s16)scale);
    if (scale <= 0x100)
        oam.a.objMode = 1;
    else
        oam.a.objMode = 3;
    buf = gUnknown_03001300;
    idx = buf->count++;
    oam.a.matrix = idx;
    oam.a.matrixHi = idx >> 3;
    oam.a.matrixTop = idx >> 4;
    buf->oam[idx * 4].param = param;
    buf->oam[idx * 4 + 1].param = 0;
    buf->oam[idx * 4 + 2].param = 0;
    buf->oam[idx * 4 + 3].param = param;
    flagsPtr = &part->flags;
    oam.a.gfxMode = *flagsPtr;
    oam.a.mosaic = *flagsPtr >> 2;
    oam.a.colorMode = *flagsPtr >> 3;
    {
        struct part_method7634 *m = (struct part_method7634 *)(part->vtable + 0x58);

        oam.b.priority = (u16)sub_803AD7C((u8 *)part + m->thisOffset, m->fn);
    }
    oam.b.palette = part->palette;

    for (i = 0; i != ((u8 *)info)[0xb]; i++) {
        s32 id = info->ids[i] & 0xf;
        s32 w = gStaticData_0816B2E0[id];
        s32 h = gStaticData_0816B2EC[id];
        s32 ox = info->offsets[i].x;
        s32 oy = info->offsets[i].y;
        s32 tiles = (w >> 3) * (h >> 3);
        s32 sw = ((w << 8) * scale) >> 16;
        s32 sh = ((h << 8) * scale) >> 16;
        s32 x, y;

        if (*flagsPtr & 0x20)
            y = pos[1] - sh - oy;
        else
            y = pos[1] + oy;
        if (y + sh > 0 && y <= 0x9f) {
            if (*flagsPtr & 0x10)
                x = pos[0] - sw - ox;
            else
                x = pos[0] + ox;
            if (x + sw > 0 && x <= 0xef) {
                s32 saved = part->tick;
                struct piece_info *first;
                s32 cx, cy;
                s32 id0;

                ClampTick7634(part, 0);
                first = sub_80083B8(part);
                ClampTick7634(part, saved);
                cx = first->offsets[0].x + pos[0];
                cy = first->offsets[0].y + pos[1];
                id0 = *first->ids & 0xf;
                cx += gStaticData_0816B2E0[id0] >> 1;
                cy += gStaticData_0816B2EC[id0] >> 1;
                if (i == 0) {
                    s32 dx = x + (gStaticData_0816B2E0[id] >> 1) - cx;
                    s32 dy = y + (gStaticData_0816B2EC[id] >> 1) - cy;

                    pullX = -(((dx << 8) * scale) >> 16);
                    pullY = -(((dy << 8) * scale) >> 16);
                    x += pullX;
                    y += pullY;
                    baseX = x;
                    baseY = y;
                    halfW = sw >> 1;
                    halfH = sh >> 1;
                } else {
                    s32 dx, dy;

                    x += pullX;
                    y += pullY;
                    dx = (((x - baseX) << 8) * scale >> 16) + halfW - (sw >> 1);
                    dy = (((y - baseY) << 8) * scale >> 16) + halfH - (sh >> 1);
                    x = baseX + dx;
                    y = baseY + dy;
                }
                oam.a.y = y;
                oam.a.shape = id >> 2;
                oam.a.x = x;
                oam.a.size = id;
                oam.b.tile = tile;
                sub_8006AC8(gUnknown_03001300, &oam);
            }
        }
        if (*flagsPtr & 8)
            tiles *= 2;
        tile += tiles;
        total += tiles << 5;
    }
    sub_8006C84(gUnknown_030012FC, sub_80083A8(part) + (info->packed & 0xffffff), total);
}
#else
struct affine_part;

NAKED void sub_8007634(void *unused, struct affine_part *part, s32 *pos)
{
    asm(".syntax unified\n"
        "\tpush {r4, r5, r6, r7, lr}\n"
        "\tmov r7, sl\n"
        "\tmov r6, sb\n"
        "\tmov r5, r8\n"
        "\tpush {r5, r6, r7}\n"
        "\tsub sp, #0x48\n"
        "\tstr r1, [sp, #8]\n"
        "\tstr r2, [sp, #0xc]\n"
        "\tmovs r0, #0\n"
        "\tstr r0, [sp, #0x10]\n"
        "\tadds r0, r1, #0\n"
        "\tbl sub_80083B8\n"
        "\tstr r0, [sp, #0x14]\n"
        "\tldr r0, _0800768C\n"
        "\tldr r0, [r0]\n"
        "\tbl sub_8006C44\n"
        "\tstr r0, [sp, #0x18]\n"
        "\tldr r1, [sp, #8]\n"
        "\tldrh r1, [r1, #0x3c]\n"
        "\tstr r1, [sp, #0x1c]\n"
        "\tcmp r1, #0x3f\n"
        "\tbgt _08007668\n"
        "\tmovs r2, #0x40\n"
        "\tstr r2, [sp, #0x1c]\n"
        "_08007668:\n"
        "\tldr r3, [sp, #0x1c]\n"
        "\tlsls r0, r3, #0x10\n"
        "\tasrs r0, r0, #0x10\n"
        "\tbl sub_800090C\n"
        "\tlsls r0, r0, #0x10\n"
        "\tlsrs r6, r0, #0x10\n"
        "\tadds r7, r6, #0\n"
        "\tmovs r2, #0x80\n"
        "\tlsls r2, r2, #1\n"
        "\tldr r4, [sp, #0x1c]\n"
        "\tcmp r4, r2\n"
        "\tbgt _08007694\n"
        "\tldr r1, _08007690\n"
        "\tldr r0, [sp]\n"
        "\tands r0, r1\n"
        "\torrs r0, r2\n"
        "\tb _0800769C\n"
        "\t.align 2, 0\n"
        "_0800768C: .4byte gUnknown_030012FC\n"
        "_08007690: .4byte 0xFFFFFCFF\n"
        "_08007694:\n"
        "\tmovs r1, #0xc0\n"
        "\tlsls r1, r1, #2\n"
        "\tldr r0, [sp]\n"
        "\torrs r0, r1\n"
        "_0800769C:\n"
        "\tstr r0, [sp]\n"
        "\tldr r0, _080077F0\n"
        "\tldr r4, [r0]\n"
        "\tldr r0, [r4, #8]\n"
        "\tadds r3, r0, #0\n"
        "\tadds r0, #1\n"
        "\tstr r0, [r4, #8]\n"
        "\tmovs r1, #7\n"
        "\tands r1, r3\n"
        "\tlsls r1, r1, #0x19\n"
        "\tldr r2, _080077F4\n"
        "\tldr r0, [sp]\n"
        "\tands r0, r2\n"
        "\torrs r0, r1\n"
        "\tasrs r1, r3, #3\n"
        "\tmovs r5, #1\n"
        "\tands r1, r5\n"
        "\tlsls r1, r1, #0x1c\n"
        "\tldr r2, _080077F8\n"
        "\tands r0, r2\n"
        "\torrs r0, r1\n"
        "\tasrs r1, r3, #4\n"
        "\tands r1, r5\n"
        "\tlsls r1, r1, #0x1d\n"
        "\tldr r2, _080077FC\n"
        "\tands r0, r2\n"
        "\torrs r0, r1\n"
        "\tstr r0, [sp]\n"
        "\tlsls r1, r3, #2\n"
        "\tlsls r3, r3, #5\n"
        "\tadds r3, r4, r3\n"
        "\tmovs r2, #0\n"
        "\tstrh r6, [r3, #0x12]\n"
        "\tadds r0, r1, #1\n"
        "\tlsls r0, r0, #3\n"
        "\tadds r0, r4, r0\n"
        "\tstrh r2, [r0, #0x12]\n"
        "\tadds r0, r1, #2\n"
        "\tlsls r0, r0, #3\n"
        "\tadds r0, r4, r0\n"
        "\tstrh r2, [r0, #0x12]\n"
        "\tadds r1, #3\n"
        "\tlsls r1, r1, #3\n"
        "\tadds r4, r4, r1\n"
        "\tstrh r7, [r4, #0x12]\n"
        "\tldr r7, [sp, #8]\n"
        "\tadds r7, #0x28\n"
        "\tmov r8, r7\n"
        "\tldrb r3, [r7]\n"
        "\tlsls r0, r3, #0x1e\n"
        "\tmovs r6, #3\n"
        "\tlsrs r0, r0, #0x14\n"
        "\tldr r4, _08007800\n"
        "\tldr r1, [sp]\n"
        "\tands r1, r4\n"
        "\torrs r1, r0\n"
        "\tlsls r0, r3, #0x1d\n"
        "\tlsrs r0, r0, #0x1f\n"
        "\tands r0, r5\n"
        "\tlsls r0, r0, #0xc\n"
        "\tldr r2, _08007804\n"
        "\tands r1, r2\n"
        "\torrs r1, r0\n"
        "\tlsls r3, r3, #0x1c\n"
        "\tlsrs r3, r3, #0x1f\n"
        "\tands r3, r5\n"
        "\tlsls r3, r3, #0xd\n"
        "\tldr r0, _08007808\n"
        "\tands r1, r0\n"
        "\torrs r1, r3\n"
        "\tstr r1, [sp]\n"
        "\tldr r0, [sp, #8]\n"
        "\tldr r1, [r0, #0x18]\n"
        "\tadds r1, #0x58\n"
        "\tmovs r2, #0\n"
        "\tldrsh r0, [r1, r2]\n"
        "\tldr r3, [sp, #8]\n"
        "\tadds r0, r3, r0\n"
        "\tldr r1, [r1, #4]\n"
        "\tbl sub_803AD7C\n"
        "\tlsls r0, r0, #0x10\n"
        "\tlsrs r0, r0, #0x10\n"
        "\tands r0, r6\n"
        "\tlsls r0, r0, #0xa\n"
        "\tldr r1, [sp, #4]\n"
        "\tands r1, r4\n"
        "\torrs r1, r0\n"
        "\tldr r0, [sp, #8]\n"
        "\tadds r0, #0x29\n"
        "\tldrb r0, [r0]\n"
        "\tlsls r0, r0, #0x1c\n"
        "\tlsrs r0, r0, #0x10\n"
        "\tldr r2, _0800780C\n"
        "\tands r1, r2\n"
        "\torrs r1, r0\n"
        "\tstr r1, [sp, #4]\n"
        "\tmovs r4, #0\n"
        "\tstr r4, [sp, #0x20]\n"
        "\tmovs r7, #0\n"
        "\tstr r7, [sp, #0x24]\n"
        "\tmovs r0, #0\n"
        "\tstr r0, [sp, #0x28]\n"
        "\tmovs r1, #0\n"
        "\tstr r1, [sp, #0x2c]\n"
        "\tmovs r2, #0\n"
        "\tstr r2, [sp, #0x30]\n"
        "\tmovs r3, #0\n"
        "\tstr r3, [sp, #0x34]\n"
        "\tstr r4, [sp, #0x38]\n"
        "\tmov r7, r8\n"
        "\tstr r7, [sp, #0x44]\n"
        "\tldr r0, [sp, #0x14]\n"
        "\tldrb r0, [r0, #0xb]\n"
        "\tcmp r4, r0\n"
        "\tbne _08007786\n"
        "\tb _080079F4\n"
        "_08007786:\n"
        "\tldr r1, [sp, #0x14]\n"
        "\tldr r0, [r1, #4]\n"
        "\tldr r2, [sp, #0x38]\n"
        "\tadds r0, r0, r2\n"
        "\tmovs r3, #0xf\n"
        "\tldrb r0, [r0]\n"
        "\tands r0, r3\n"
        "\tstr r0, [sp, #0x3c]\n"
        "\tldr r0, _08007810\n"
        "\tldr r4, [sp, #0x3c]\n"
        "\tadds r0, r4, r0\n"
        "\tldrb r0, [r0]\n"
        "\tmov r8, r0\n"
        "\tldr r0, _08007814\n"
        "\tadds r0, r4, r0\n"
        "\tldrb r0, [r0]\n"
        "\tmov sb, r0\n"
        "\tldr r1, [r1]\n"
        "\tlsls r0, r2, #2\n"
        "\tadds r0, r0, r1\n"
        "\tmovs r7, #0\n"
        "\tldrsh r2, [r0, r7]\n"
        "\tmovs r1, #2\n"
        "\tldrsh r3, [r0, r1]\n"
        "\tmov r4, r8\n"
        "\tasrs r1, r4, #3\n"
        "\tmov r7, sb\n"
        "\tasrs r0, r7, #3\n"
        "\tadds r4, r1, #0\n"
        "\tmuls r4, r0, r4\n"
        "\tstr r4, [sp, #0x40]\n"
        "\tmov r7, r8\n"
        "\tlsls r0, r7, #8\n"
        "\tldr r1, [sp, #0x1c]\n"
        "\tmuls r0, r1, r0\n"
        "\tasrs r0, r0, #0x10\n"
        "\tmov r8, r0\n"
        "\tmov r4, sb\n"
        "\tlsls r0, r4, #8\n"
        "\tmuls r0, r1, r0\n"
        "\tasrs r0, r0, #0x10\n"
        "\tmov sb, r0\n"
        "\tldr r7, [sp, #0x44]\n"
        "\tldrb r7, [r7]\n"
        "\tlsls r0, r7, #0x1a\n"
        "\tcmp r0, #0\n"
        "\tbge _08007818\n"
        "\tldr r1, [sp, #0xc]\n"
        "\tldr r0, [r1, #4]\n"
        "\tmov r4, sb\n"
        "\tsubs r0, r0, r4\n"
        "\tsubs r6, r0, r3\n"
        "\tb _0800781E\n"
        "\t.align 2, 0\n"
        "_080077F0: .4byte gUnknown_03001300\n"
        "_080077F4: .4byte 0xF1FFFFFF\n"
        "_080077F8: .4byte 0xEFFFFFFF\n"
        "_080077FC: .4byte 0xDFFFFFFF\n"
        "_08007800: .4byte 0xFFFFF3FF\n"
        "_08007804: .4byte 0xFFFFEFFF\n"
        "_08007808: .4byte 0xFFFFDFFF\n"
        "_0800780C: .4byte 0xFFFF0FFF\n"
        "_08007810: .4byte gStaticData_0816B2E0\n"
        "_08007814: .4byte gStaticData_0816B2EC\n"
        "_08007818:\n"
        "\tldr r7, [sp, #0xc]\n"
        "\tldr r0, [r7, #4]\n"
        "\tadds r6, r0, r3\n"
        "_0800781E:\n"
        "\tmov r1, sb\n"
        "\tadds r0, r6, r1\n"
        "\tcmp r0, #0\n"
        "\tbgt _08007828\n"
        "\tb _080079C4\n"
        "_08007828:\n"
        "\tcmp r6, #0x9f\n"
        "\tble _0800782E\n"
        "\tb _080079C4\n"
        "_0800782E:\n"
        "\tldr r3, [sp, #0x44]\n"
        "\tldrb r3, [r3]\n"
        "\tlsls r0, r3, #0x1b\n"
        "\tcmp r0, #0\n"
        "\tbge _08007844\n"
        "\tldr r4, [sp, #0xc]\n"
        "\tldr r0, [r4]\n"
        "\tmov r7, r8\n"
        "\tsubs r0, r0, r7\n"
        "\tsubs r5, r0, r2\n"
        "\tb _0800784A\n"
        "_08007844:\n"
        "\tldr r1, [sp, #0xc]\n"
        "\tldr r0, [r1]\n"
        "\tadds r5, r0, r2\n"
        "_0800784A:\n"
        "\tmov r2, r8\n"
        "\tadds r0, r5, r2\n"
        "\tcmp r0, #0\n"
        "\tbgt _08007854\n"
        "\tb _080079C4\n"
        "_08007854:\n"
        "\tcmp r5, #0xef\n"
        "\tble _0800785A\n"
        "\tb _080079C4\n"
        "_0800785A:\n"
        "\tldr r3, [sp, #8]\n"
        "\tldr r3, [r3, #0x30]\n"
        "\tmov sl, r3\n"
        "\tmovs r2, #0\n"
        "\tldr r4, [sp, #8]\n"
        "\tldr r0, [r4, #0x20]\n"
        "\tadds r4, #0x2d\n"
        "\tldr r1, [r0]\n"
        "\tldrb r7, [r4]\n"
        "\tlsls r0, r7, #3\n"
        "\tadds r3, r7, #0\n"
        "\tsubs r0, r0, r3\n"
        "\tlsls r0, r0, #2\n"
        "\tadds r0, r0, r1\n"
        "\tldrb r0, [r0, #0x16]\n"
        "\tcmp r2, r0\n"
        "\tblt _0800787E\n"
        "\tsubs r2, r0, #1\n"
        "_0800787E:\n"
        "\tldr r7, [sp, #8]\n"
        "\tstr r2, [r7, #0x30]\n"
        "\tldr r0, [sp, #8]\n"
        "\tbl sub_80083B8\n"
        "\tadds r7, r0, #0\n"
        "\tmov r2, sl\n"
        "\tldr r1, [sp, #8]\n"
        "\tldr r0, [r1, #0x20]\n"
        "\tldr r1, [r0]\n"
        "\tldrb r3, [r4]\n"
        "\tlsls r0, r3, #3\n"
        "\tsubs r0, r0, r3\n"
        "\tlsls r0, r0, #2\n"
        "\tadds r0, r0, r1\n"
        "\tldrb r0, [r0, #0x16]\n"
        "\tcmp r2, r0\n"
        "\tblt _080078A4\n"
        "\tsubs r2, r0, #1\n"
        "_080078A4:\n"
        "\tldr r4, [sp, #8]\n"
        "\tstr r2, [r4, #0x30]\n"
        "\tldr r0, [r7]\n"
        "\tmovs r1, #0\n"
        "\tldrsh r3, [r0, r1]\n"
        "\tmovs r2, #2\n"
        "\tldrsh r4, [r0, r2]\n"
        "\tldr r0, [r7, #4]\n"
        "\tmovs r1, #0xf\n"
        "\tldrb r0, [r0]\n"
        "\tands r1, r0\n"
        "\tldr r7, _08007934\n"
        "\tmov ip, r7\n"
        "\tmov r0, ip\n"
        "\tadds r2, r1, r0\n"
        "\tldr r7, _08007938\n"
        "\tmov sl, r7\n"
        "\tadd r1, sl\n"
        "\tldr r7, [sp, #0xc]\n"
        "\tldr r0, [r7]\n"
        "\tadds r3, r3, r0\n"
        "\tldr r0, [r7, #4]\n"
        "\tadds r4, r4, r0\n"
        "\tldrb r2, [r2]\n"
        "\tlsrs r0, r2, #1\n"
        "\tadds r3, r3, r0\n"
        "\tldrb r1, [r1]\n"
        "\tlsrs r0, r1, #1\n"
        "\tadds r4, r4, r0\n"
        "\tldr r0, [sp, #0x38]\n"
        "\tcmp r0, #0\n"
        "\tbne _0800793C\n"
        "\tldr r0, [sp, #0x3c]\n"
        "\tadd r0, ip\n"
        "\tldrb r0, [r0]\n"
        "\tlsrs r0, r0, #1\n"
        "\tadds r0, r5, r0\n"
        "\tstr r0, [sp, #0x30]\n"
        "\tldr r0, [sp, #0x3c]\n"
        "\tadd r0, sl\n"
        "\tldrb r0, [r0]\n"
        "\tlsrs r0, r0, #1\n"
        "\tadds r0, r6, r0\n"
        "\tldr r1, [sp, #0x30]\n"
        "\tsubs r1, r1, r3\n"
        "\tsubs r0, r0, r4\n"
        "\tstr r0, [sp, #0x34]\n"
        "\tlsls r0, r1, #8\n"
        "\tldr r2, [sp, #0x1c]\n"
        "\tmuls r0, r2, r0\n"
        "\tasrs r0, r0, #0x10\n"
        "\tstr r0, [sp, #0x30]\n"
        "\tldr r3, [sp, #0x34]\n"
        "\tlsls r0, r3, #8\n"
        "\tmuls r0, r2, r0\n"
        "\tasrs r0, r0, #0x10\n"
        "\tldr r4, [sp, #0x30]\n"
        "\trsbs r4, r4, #0\n"
        "\tstr r4, [sp, #0x30]\n"
        "\trsbs r0, r0, #0\n"
        "\tstr r0, [sp, #0x34]\n"
        "\tadds r5, r5, r4\n"
        "\tadds r6, r6, r0\n"
        "\tstr r5, [sp, #0x20]\n"
        "\tstr r6, [sp, #0x24]\n"
        "\tmov r7, r8\n"
        "\tasrs r7, r7, #1\n"
        "\tstr r7, [sp, #0x28]\n"
        "\tmov r0, sb\n"
        "\tasrs r0, r0, #1\n"
        "\tstr r0, [sp, #0x2c]\n"
        "\tb _08007976\n"
        "\t.align 2, 0\n"
        "_08007934: .4byte gStaticData_0816B2E0\n"
        "_08007938: .4byte gStaticData_0816B2EC\n"
        "_0800793C:\n"
        "\tldr r1, [sp, #0x30]\n"
        "\tadds r5, r5, r1\n"
        "\tldr r2, [sp, #0x34]\n"
        "\tadds r6, r6, r2\n"
        "\tldr r3, [sp, #0x20]\n"
        "\tsubs r2, r5, r3\n"
        "\tldr r4, [sp, #0x24]\n"
        "\tsubs r1, r6, r4\n"
        "\tlsls r0, r2, #8\n"
        "\tldr r7, [sp, #0x1c]\n"
        "\tmuls r0, r7, r0\n"
        "\tasrs r2, r0, #0x10\n"
        "\tlsls r0, r1, #8\n"
        "\tmuls r0, r7, r0\n"
        "\tasrs r1, r0, #0x10\n"
        "\tldr r0, [sp, #0x28]\n"
        "\tadds r2, r2, r0\n"
        "\tldr r3, [sp, #0x2c]\n"
        "\tadds r1, r1, r3\n"
        "\tmov r4, r8\n"
        "\tasrs r0, r4, #1\n"
        "\tsubs r2, r2, r0\n"
        "\tmov r7, sb\n"
        "\tasrs r0, r7, #1\n"
        "\tsubs r1, r1, r0\n"
        "\tldr r0, [sp, #0x20]\n"
        "\tadds r5, r0, r2\n"
        "\tldr r2, [sp, #0x24]\n"
        "\tadds r6, r2, r1\n"
        "_08007976:\n"
        "\tlsls r1, r6, #0x18\n"
        "\tlsrs r1, r1, #0x18\n"
        "\tldr r2, _08007A24\n"
        "\tldr r0, [sp]\n"
        "\tands r0, r2\n"
        "\torrs r0, r1\n"
        "\tldr r3, [sp, #0x3c]\n"
        "\tasrs r2, r3, #2\n"
        "\tmovs r4, #3\n"
        "\tands r2, r4\n"
        "\tlsls r2, r2, #0xe\n"
        "\tldr r1, _08007A28\n"
        "\tands r0, r1\n"
        "\torrs r0, r2\n"
        "\tldr r1, _08007A2C\n"
        "\tands r5, r1\n"
        "\tlsls r2, r5, #0x10\n"
        "\tldr r1, _08007A30\n"
        "\tands r0, r1\n"
        "\torrs r0, r2\n"
        "\tands r3, r4\n"
        "\tlsls r2, r3, #0x1e\n"
        "\tldr r1, _08007A34\n"
        "\tands r0, r1\n"
        "\torrs r0, r2\n"
        "\tstr r0, [sp]\n"
        "\tldr r7, [sp, #0x18]\n"
        "\tlsls r1, r7, #0x16\n"
        "\tlsrs r1, r1, #0x16\n"
        "\tldr r2, _08007A38\n"
        "\tldr r0, [sp, #4]\n"
        "\tands r0, r2\n"
        "\torrs r0, r1\n"
        "\tstr r0, [sp, #4]\n"
        "\tldr r0, _08007A3C\n"
        "\tldr r0, [r0]\n"
        "\tmov r1, sp\n"
        "\tbl sub_8006AC8\n"
        "_080079C4:\n"
        "\tldr r1, [sp, #0x44]\n"
        "\tldrb r1, [r1]\n"
        "\tlsls r0, r1, #0x1c\n"
        "\tcmp r0, #0\n"
        "\tbge _080079D4\n"
        "\tldr r2, [sp, #0x40]\n"
        "\tlsls r2, r2, #1\n"
        "\tstr r2, [sp, #0x40]\n"
        "_080079D4:\n"
        "\tldr r3, [sp, #0x18]\n"
        "\tldr r4, [sp, #0x40]\n"
        "\tadds r3, r3, r4\n"
        "\tstr r3, [sp, #0x18]\n"
        "\tlsls r0, r4, #5\n"
        "\tldr r7, [sp, #0x10]\n"
        "\tadds r7, r7, r0\n"
        "\tstr r7, [sp, #0x10]\n"
        "\tldr r0, [sp, #0x38]\n"
        "\tadds r0, #1\n"
        "\tstr r0, [sp, #0x38]\n"
        "\tldr r1, [sp, #0x14]\n"
        "\tldrb r1, [r1, #0xb]\n"
        "\tcmp r0, r1\n"
        "\tbeq _080079F4\n"
        "\tb _08007786\n"
        "_080079F4:\n"
        "\tldr r0, _08007A40\n"
        "\tldr r4, [r0]\n"
        "\tldr r0, [sp, #8]\n"
        "\tbl sub_80083A8\n"
        "\tadds r1, r0, #0\n"
        "\tldr r2, [sp, #0x14]\n"
        "\tldr r0, [r2, #8]\n"
        "\tldr r2, _08007A44\n"
        "\tands r0, r2\n"
        "\tadds r1, r1, r0\n"
        "\tadds r0, r4, #0\n"
        "\tldr r2, [sp, #0x10]\n"
        "\tbl sub_8006C84\n"
        "\tadd sp, #0x48\n"
        "\tpop {r3, r4, r5}\n"
        "\tmov r8, r3\n"
        "\tmov sb, r4\n"
        "\tmov sl, r5\n"
        "\tpop {r4, r5, r6, r7}\n"
        "\tpop {r0}\n"
        "\tbx r0\n"
        "\t.align 2, 0\n"
        "_08007A24: .4byte 0xFFFFFF00\n"
        "_08007A28: .4byte 0xFFFF3FFF\n"
        "_08007A2C: .4byte 0x000001FF\n"
        "_08007A30: .4byte 0xFE00FFFF\n"
        "_08007A34: .4byte 0x3FFFFFFF\n"
        "_08007A38: .4byte 0xFFFFFC00\n"
        "_08007A3C: .4byte gUnknown_03001300\n"
        "_08007A40: .4byte gUnknown_030012FC\n"
        "_08007A44: .4byte 0x00FFFFFF\n"
        ".syntax divided\n");
}

#endif
