#include "sprite_obj.hpp"

extern "C" {
#include "core.h"
#include "gfx.h"
#include "gfx_part.h"
#include "objects.h"
#include "globals.h"
}

/* SpriteRenderer::DrawPieces (DrawSpritePieces, 0x080073DC-0x08007634;
 * #664 cleanup, include/sprite_obj.hpp), the plain (non-affine) sibling of
 * DrawAffinePieces (affine_sprite_pieces.cpp). Split out of graphics.c
 * (now graphics.cpp) when that was still built with agbcc: it needs
 * old_agbcc, and old_agbcp as C++ (Makefile OLD_AGBCC_OBJS) - the ROM loads the piece id's
 * `0xf` mask before the `ldrb` it is combined with. graphics.cpp is an
 * old_agbcp object too since #664.
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
