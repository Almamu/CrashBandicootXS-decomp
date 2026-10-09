#include "sprite_obj.hpp"

extern "C" {
#include "core.h"
#include "math_util.h"
#include "sprite_bank.h"
#include "util.h"
#include "gfx.h"
#include "gfx_part.h"
#include "objects.h"
#include "globals.h"
}

/* The sprite renderer (gSpriteRenderer; SpriteRenderer, include/sprite_obj.hpp):
 * DrawPieces, DrawAffinePieces, DrawAt, Draw and the empty constructor and
 * destructor. An old_agbcp object (OLD_AGBCC_OBJS). */

/* SpriteRenderer::DrawPieces (DrawSpritePieces, 0x080073DC-0x08007634;
 * #664 cleanup, include/sprite_obj.hpp), the plain (non-affine) sibling of
 * DrawAffinePieces (below). Split out of graphics.c
 * (now graphics.cpp) when that was still built with agbcc: it needs
 * old_agbcc, and old_agbcp as C++ (Makefile OLD_AGBCC_OBJS) - the ROM loads the piece id's
 * `0xf` mask before the `ldrb` it is combined with. graphics.cpp was an
 * old_agbcp object too since #664 (split into entity.cpp and the gfx/
 * manager files in #767; entity.o is the object before this one).
 *
 * Queues one OAM entry per visible sub-piece of an animated `part`
 * (mirrored per the part's flag bits) and sends the part's whole tile
 * total to `UploadObjVram` once after the loop. */

static inline s32 PieceSize73DC(s32 id)
{
    return id & 3;
}

static inline s32 PieceShape73DC(s32 id)
{
    return (id >> 2) & 3;
}

void SpriteRenderer::DrawPieces(Sprite *part, s32 *pos)
{
    struct oam_attrs oam;
    s32 total = 0;
    struct piece_info *info = (struct piece_info *)part->GetFrame();
    s32 tile = gObjVramCursor->GetTile();
    s32 i;

    oam.affineMode = 0;
    oam.objMode = part->mirrorFlags.gfxMode;
    oam.mosaic = part->mirrorFlags.mosaic;
    oam.bpp = part->mirrorFlags.colorMode;
    oam.priority = part->GetPriority();
    oam.palette = part->palette;
    /* Not affine: matrixBit3/matrixBit4 are the h/v flip. */
    if (PART_FLAG_SET(part, 27))
        oam.matrixBit3 = 1;
    else
        oam.matrixBit3 = 0;
    if (PART_FLAG_SET(part, 26))
        oam.matrixBit4 = 1;
    else
        oam.matrixBit4 = 0;

    for (i = 0; i != info->u.b.count; i++) {
        s32 id = info->ids[i] & 0xf;
        s32 w = gObjPieceWidths[id];
        s32 h = gObjPieceHeights[id];
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
                oam.y = y;
                oam.shape = PieceShape73DC(id);
                oam.x = x;
                oam.size = PieceSize73DC(id);
                oam.tileNum = tile;
                gOamBuffer->Add(&oam);
            }
        }
        tiles = (w >> 3) * (h >> 3);
        if (PART_FLAG_SET(part, 28)) // 8bpp: twice the tiles
            tiles *= 2;
        tile += tiles;
        total += tiles << 5;
    }
    gObjVramCursor->Upload((void *)(part->GetTileBase() + (info->u.packed & 0xffffff)), total);
}

/* GitHub issue #9: SpriteRenderer::DrawAffinePieces (DrawAffineSpritePieces,
 * 0x08007634-0x08007A48; #664 cleanup, include/sprite_obj.hpp), the affine
 * (rotation/scaling) sibling of DrawPieces (above) - see
 * docs/matching/archive/issue-9-0x08007634-actor.md. Moved here from
 * asm/code_3_2.s in the issue #9 raw-asm pass.
 *
 * Queues one affine OAM entry per visible sub-piece of an animated
 * `part`, scaled by its `affine` halfword (Q8, at least 0x40). It allocates an
 * affine matrix slot from `gOamBuffer->matrixCount` (written as a plain
 * scale into the slot's pa/pd, via `FixedInverse16`), selects OBJ mode 1
 * (affine) or 3 (double size, scale > 0x100), and pulls every piece
 * towards the first piece's centre by the scale factor. To find that
 * centre it probes keyframe 0 through `GetFrame` with `part->frame`
 * temporarily clamped to 0. The tile total goes to `UploadObjVram` once
 * after the loop.
 *
 * Built with old_agbcp, as the C was with old_agbcc (Makefile
 * OLD_AGBCC_OBJS): the ROM loads the 0xf
 * mask before the `ldrb` it is combined with. The ROM keeps nearly every
 * local in a 0x48-byte frame; those are register-allocator spills, and
 * the source shapes below are what reproduce them. See
 * docs/matching/archive/graphics-7634-retry.md. */

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

static inline void ClampTick7634(Sprite *part, s32 t)
{
    s32 n = part->bank->anims[part->tag].frameCount;

    CLAMP_INDEX(t, n);
    part->frame = t;
}

void SpriteRenderer::DrawAffinePieces(Sprite *part, s32 *pos)
{
    struct oam_attrs oam;
    s32 total = 0;
    struct piece_info *info = (struct piece_info *)part->GetFrame();
    s32 tile = gObjVramCursor->GetTile();
    s32 scale = part->affine;
    s16 pa; // the matrix scales, signed 8.8
    s16 pd;
    s32 idx;
    s32 k;
    OamBuffer *buf;
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
    oam.objMode = part->mirrorFlags.gfxMode;
    oam.mosaic = part->mirrorFlags.mosaic;
    oam.bpp = part->mirrorFlags.colorMode;
    oam.priority = part->GetPriority();
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
                s32 saved = part->frame;
                struct piece_info *first;
                s32 cx, cy;
                s32 id0;
                const u8 *pw, *ph;

                ClampTick7634(part, 0);
                first = (struct piece_info *)part->GetFrame();
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
                gOamBuffer->Add(&oam);
            }
        }
        if (PART_FLAG_SET(part, 28)) // 8bpp: twice the tiles
            tiles *= 2;
        tile += tiles;
        total += tiles << 5;
    }
    gObjVramCursor->Upload((void *)(part->GetTileBase() + (info->u.packed & 0xffffff)), total);
}

/* `screenSpace` says whether (x, y) are already screen coordinates;
 * otherwise WorldToScreen makes them camera-relative. DrawPieces
 * builds and queues the part's OAM entries at the resolved position. */
void SpriteRenderer::DrawAt(Sprite *part, s32 x, s32 y)
{
    s32 pos[2];

    if (part->screenSpace == 0) {
        WorldToScreen(part, x, y, &pos[0], &pos[1]);
    } else {
        pos[0] = x;
        pos[1] = y;
    }
    DrawPieces(part, pos);
}

/* Draws `part` at its own position. */
void SpriteRenderer::Draw(Sprite *part)
{
    DrawAt(part, Q8_TO_INT(part->x), Q8_TO_INT(part->y));
}

SpriteRenderer::~SpriteRenderer()
{
}

SpriteRenderer::SpriteRenderer()
{
}
