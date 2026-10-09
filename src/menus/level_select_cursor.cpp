/* LevelSelectCursor (include/level_select.hpp), the level-select screen's
 * cursor (`LevelSelect::panel`). It glides between entries along a
 * Bresenham line (two steps per frame), plays an idle animation cycle at
 * random intervals, and grows in (state 4) / shrinks away (state 5) as an
 * affine OBJ drawn straight into the OAM shadow buffer.
 *
 * UNUSED - no `bl`/`.4byte` reference in asm/, data/ or src/, and no
 * Thumb pointer anywhere in the ROM: IsHidden, IsGrowing and MoveTo (Park
 * has it inlined). Matched anyway.
 *
 * GitHub issue #29; split from level_select_widgets.cpp (#767), same flags
 * (old_agbcc). */

#include "level_select.hpp"
#include "audio.hpp"

extern "C" {
#include <agb_syscall.h>
#include "math_util.h"
}

/* One affine parameter; OBJ matrix `m` is entries 4m..4m+3. An inline so
 * that the value is loaded before the address is computed. */
static inline void SetAffineParam(OamBuffer *buf, s32 n, u16 v)
{
    buf->table[n].attr[3] = v;
}

LevelSelectCursor::LevelSelectCursor()
{
    UiSprite *p = new UiSprite;

    part = p;
    p->bank = AnimTable(0x240);
    p->StartAnim(0);
    part->palette = part->GetAnimPaletteSlot();
    gPaletteCache->Lock(part->bank->anims[part->tag].paletteId);
    SetPos(0x78, 0x35);
    oam.y = line.y0 - 0x20;
    oam.affineMode = 1;
    oam.objMode = 0;
    oam.mosaic = 0;
    oam.bpp = 0;
    oam.shape = 0;
    oam.x = line.x0 - 0x20;
    oam.matrixLo = 0;
    oam.matrixBit3 = 0;
    oam.matrixBit4 = 0;
    oam.size = 3;
    oam.tileNum = 0x3C0;
    oam.priority = 0;
    oam.palette = part->palette;
    LoadTaggedAsset(gLevelSelectCursorZoomTiles, OBJ_VRAM0 + 0x7800);
    affine.angle = 0;
    scale = 8;
    state = 4;
}

/* Per-frame update. Steps the glide; states 0-3 are the idle animation
 * cycle (0 waits for `timer`, then plays gLevelSelectCursorAnims[1..3] and
 * back to [0]), 4 grows the cursor to 1:1, 5 shrinks it away. */
void LevelSelectCursor::Update()
{
    Glide();
    switch (state) {
    case 0:
        part->AdvanceAnim();
        if (timer != 0) {
            timer--;
            break;
        }
        if (part->frame == 0) {
            state = 1;
            part->StartAnim(gLevelSelectCursorAnims[1]);
        }
        break;
    case 1:
        part->AdvanceAnim();
        if (part->animDone) {
            state = 2;
            part->StartAnim(gLevelSelectCursorAnims[2]);
        }
        break;
    case 2:
        part->AdvanceAnim();
        if (part->animDone) {
            state = 3;
            part->StartAnim(gLevelSelectCursorAnims[3]);
        }
        break;
    case 3:
        part->AdvanceAnim();
        if (part->animDone) {
            state = 0;
            part->StartAnim(gLevelSelectCursorAnims[0]);
            ResetIdleTimer();
        }
        break;
    case 4:
        if (scale <= 0xFF) {
            scale += speed;
        } else {
            scale = 0x100;
            state = 0;
            ResetIdleTimer();
        }
        break;
    case 5:
        if (scale > 8)
            scale -= speed;
        else
            scale = 8;
        break;
    }
}

/* Draw step. While growing/shrinking the cursor is drawn by hand as an
 * affine OBJ with its own matrix; otherwise the sprite part draws it. */
void LevelSelectCursor::Draw()
{
    SetEntityPixelPos(part, line.x0, line.y0);
    switch (state) {
    case 4 ... 5:
        if (scale > 8) {
            s32 idx;

            oam.x = line.x0 - 0x20;
            oam.y = line.y0 - 0x20;
            idx = gOamBuffer->matrixCount++;
            oam.matrixLo = idx;
            oam.matrixBit3 = (idx >> 3) & 1;
            oam.matrixBit4 = (idx >> 4) & 1;
            SetMatrix();
            SetAffineParam(gOamBuffer, idx * 4 + 0, matrix[0]);
            SetAffineParam(gOamBuffer, idx * 4 + 1, matrix[1]);
            SetAffineParam(gOamBuffer, idx * 4 + 2, matrix[2]);
            SetAffineParam(gOamBuffer, idx * 4 + 3, matrix[3]);
            gOamBuffer->Add(&oam);
        }
        break;
    default:
        part->DrawWithOffset(0, 0);
        break;
    }
}

/* Builds the OBJ affine matrix for the current zoom. */
void LevelSelectCursor::SetMatrix()
{
    u16 s = 0x10000 / scale;

    affine.sx = s;
    affine.sy = s;
    ObjAffineSet(&affine, matrix, 1, 2);
}

/* UNUSED. Shrunk away (state 5). */
u8 LevelSelectCursor::IsHidden()
{
    return state == 5;
}

/* UNUSED. Growing (state 4). */
u8 LevelSelectCursor::IsGrowing()
{
    return state == 4;
}

/* Starts the shrink-away. */
void LevelSelectCursor::Hide()
{
    state = 5;
    scale = 0x100;
    Update();
}

/* Parks the cursor at the bottom-left or bottom-right corner, whichever
 * side of the screen it is on (page turn). */
void LevelSelectCursor::Park()
{
    struct vec2 pos;

    if (Q8_TO_INT(part->x) <= 0x78)
        pos.x = 0x14;
    else
        pos.x = 0xDC;
    pos.y = 0x88;
    MoveToNow(&pos);
    timer = 0;
}

/* Glide: two Bresenham steps per frame towards (x1, y1). */
void LevelSelectCursor::Glide()
{
    s32 i;

    for (i = 1; i >= 0; i--) {
        if (line.x0 != line.x1 || line.y0 != line.y1)
            StepBresenhamLine(&line);
    }
}

/* Arrived at the glide target. */
u8 LevelSelectCursor::HasArrived()
{
    u8 done = FALSE;

    if (line.x0 == line.x1 && line.y0 == line.y1)
        done = TRUE;
    return done;
}

/* Starts a glide to (x, y). The zoom step is derived from half the
 * distance along the major axis. */
void LevelSelectCursor::Move(s32 x, s32 y)
{
    line.x1 = x;
    line.y1 = y;
    if (line.x0 != x || line.y0 != y) {
        s32 d;

        InitBresenhamLine(&line);
        if (line.flag)
            d = (line.x1 - line.x0) / 2;
        else
            d = (line.y1 - line.y0) / 2;
        MAKE_ABS(d);
        speed = 0xF8 / d;
    }
    if (state == 0)
        ResetIdleTimerNow();
}

/* UNUSED (inlined into Park). */
void LevelSelectCursor::MoveTo(struct vec2 *pos)
{
    MoveToNow(pos);
}

/* Places the cursor at (x, y) at once. */
void LevelSelectCursor::SetPos(s32 x, s32 y)
{
    line.x0 = x;
    line.y0 = y;
    SetEntityPixelPos(part, x, y);
}

void LevelSelectCursor::ResetIdleTimer()
{
    ResetIdleTimerNow();
}

/* Destructor (`LevelSelect::panel`). */
LevelSelectCursor::~LevelSelectCursor()
{
    gPaletteCache->Unlock(part->bank->anims[part->tag].paletteId);
    delete part;
}
