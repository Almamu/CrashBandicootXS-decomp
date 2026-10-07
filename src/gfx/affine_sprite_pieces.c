#include "core.h"
#include "math_util.h"
#include "match.h"
#include "sprite_bank.h"
#include "util.h"
#include "gfx.h"
#include "vtable.h"
#include "objects.h"
#include "globals.h"

/* GitHub issue #9: DrawAffineSpritePieces (0x08007634-0x08007A48), the affine
 * (rotation/scaling) sibling of `DrawSpritePieces` (graphics.c) - see
 * docs/matching/archive/issue-9-0x08007634-actor.md. Moved here from
 * asm/code_3_2.s in the issue #9 raw-asm pass.
 *
 * Queues one affine OAM entry per visible sub-piece of an animated
 * `part`, scaled by `part+0x3c` (Q8, at least 0x40). It allocates an
 * affine matrix slot from `gOamBuffer->matrixCount` (written as a plain
 * scale into the slot's pa/pd, via `FixedInverse16`), selects OBJ mode 1
 * (affine) or 3 (double size, scale > 0x100), and pulls every piece
 * towards the first piece's centre by the scale factor. To find that
 * centre it probes keyframe 0 through `GetSpriteFrame` with `part->tick`
 * temporarily clamped to 0. The tile total goes to `UploadObjVram` once
 * after the loop.
 *
 * Built with old_agbcc (Makefile OLD_AGBCC_OBJS): the ROM loads the 0xf
 * mask before the `ldrb` it is combined with. The ROM keeps nearly every
 * local in a 0x48-byte frame; those are register-allocator spills, and
 * the source shapes below are what reproduce them. See
 * docs/matching/archive/graphics-7634-retry.md. */

struct affine_part {
    u8 unk_00[0x18];
    u8 *vtable; // 0x18
    u8 unk_1C[4];
    const struct sprite_bank *keyframes; // 0x20 - was a `struct kf_record **` view
    u8 unk_24[4];
    u8 gfxMode:2; // 0x28
    u8 mosaic:1;
    u8 colorMode:1;
    u8 mirrorX:1;
    u8 mirrorY:1;
    u8 flagsHi:2;
    u8 palette:4; // 0x29
    u8 paletteHi:4;
    u8 unk_2A[3];
    u8 frame; // 0x2D
    u8 unk_2E[2];
    s32 tick; // 0x30
    u8 unk_34[8];
    u16 scale; // 0x3C
};

/* The loop tests the 0x28 flag bits as sign tests (`lsl #26; cmp #0;
 * bge`); a 1-bit field test compiles to `movs #0x20; ands` instead. */
#define PART_FLAG_SET(part, shift) ((s32)(*((u8 *)(part) + 0x28) << (shift)) < 0)

extern s32 _call_via_r1(void *self, void *fn);

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
    s32 n = part->keyframes->anims[part->frame].frameCount;

    CLAMP_INDEX(t, n);
    part->tick = t;
}

void DrawAffineSpritePieces(void *unused, struct affine_part *part, s32 *pos)
{
    struct oam_attrs oam;
    s32 total = 0;
    struct piece_info *info = GetSpriteFrame((struct gfx_part *)part);
    s32 tile = GetObjVramTile(gObjVramCursor);
    s32 scale = part->scale;
    u16 pa;
    u16 pd;
    s32 idx;
    s32 k;
    struct oam_shadow_buffer *buf;
    s32 baseX, baseY, halfW, halfH, pullX, pullY;
    s32 i;

    LIMIT_MIN(scale, 0x40);
    pa = FixedInverse16((s16)scale);
    pd = pa;
    if (scale <= 0x100)
        oam.affineMode = 1;
    else
        oam.affineMode = 3;
    buf = gOamBuffer;
    idx = buf->matrixCount++;
    oam.matrixLo = idx;
    oam.matrixBit3 = idx >> 3;
    oam.matrixBit4 = idx >> 4;
    k = idx * 4;
    buf->table[k].attr[3] = pa;
    buf->table[k + 1].attr[3] = 0;
    buf->table[k + 2].attr[3] = 0;
    buf->table[k + 3].attr[3] = pd;
    /* Extra reference, no code: keeps `pa` live past `pd`, so cse2 leaves
     * the call result in `pa` (r6) and `pd` as the copy (r7), as in the
     * ROM. Without it the two registers swap. */
    MATCH_USE(pa);
    oam.objMode = part->gfxMode;
    oam.mosaic = part->mosaic;
    oam.bpp = part->colorMode;
    {
        struct vtable_slot *m = (struct vtable_slot *)(part->vtable + 0x58);

        oam.priority = (u16)_call_via_r1((u8 *)part + m->delta, m->fn);
    }
    oam.palette = part->palette;

    baseX = 0;
    baseY = 0;
    halfW = 0;
    halfH = 0;
    pullX = 0;
    pullY = 0;
    for (i = 0; i != info->u.b.count; i++) {
        s32 id = info->ids[i] & 0xf;
        s32 w = gObjPieceWidths[id];
        s32 h = gObjPieceHeights[id];
        s32 ox = info->offsets[i].x;
        s32 oy = info->offsets[i].y;
        s32 tiles = (w >> 3) * (h >> 3);
        s32 x, y;

        /* Rescaled in place, so w/h and their scaled sizes share r8/sb. */
        w = Q16_TO_INT(INT_TO_Q8(w) * scale);
        h = Q16_TO_INT(INT_TO_Q8(h) * scale);
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
                const u8 *pw, *ph;

                ClampTick7634(part, 0);
                first = GetSpriteFrame((struct gfx_part *)part);
                ClampTick7634(part, saved);
                /* The size-table addresses are taken before `pos` is read;
                 * that keeps r0-r4 busy at the `pos` reload, which makes
                 * reload spill r7 and sets the ROM's reload-register
                 * rotation for the whole function. */
                cx = first->offsets[0].x;
                cy = first->offsets[0].y;
                id0 = *first->ids & 0xf;
                pw = &gObjPieceWidths[id0];
                ph = &gObjPieceHeights[id0];
                cx += pos[0];
                cy += pos[1];
                cx += *pw >> 1;
                cy += *ph >> 1;
                if (i == 0) {
                    pullX = x + (gObjPieceWidths[id] >> 1);
                    pullY = y + (gObjPieceHeights[id] >> 1);
                    pullX -= cx;
                    pullY -= cy;
                    pullX = Q16_TO_INT(INT_TO_Q8(pullX) * scale);
                    pullY = Q16_TO_INT(INT_TO_Q8(pullY) * scale);
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
                    dx = Q16_TO_INT(INT_TO_Q8(dx) * scale);
                    dy = Q16_TO_INT(INT_TO_Q8(dy) * scale);
                    dx += halfW;
                    dy += halfH;
                    dx -= w >> 1;
                    dy -= h >> 1;
                    x = baseX + dx;
                    y = baseY + dy;
                }
                oam.y = y;
                oam.shape = PieceShape7634(id);
                oam.x = x;
                oam.size = PieceSize7634(id);
                oam.tileNum = tile;
                AddOamEntry(gOamBuffer, &oam);
            }
        }
        if (PART_FLAG_SET(part, 28)) // 8bpp: twice the tiles
            tiles *= 2;
        tile += tiles;
        total += tiles << 5;
    }
    UploadObjVram(gObjVramCursor, (void *)(GetSpriteTileBase(part) + (info->u.packed & 0xffffff)),
                  total);
}
