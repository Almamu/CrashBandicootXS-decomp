#include "level_select.hpp"
#include "audio.hpp"

extern "C" {
#include <agb_syscall.h>
#include "math_util.h"
}

/* GitHub issue #28: 0x0801DA38-0x0801DFEC. Two of the level-select
 * screen's (LevelSelect, level_select.cpp) sub-objects (#664,
 * include/level_select.hpp):
 *
 * - ZoomBg: the level picture on the affine BG2 layer (`LevelSelect::bg2`;
 *   its constructor is in level_select_pages.cpp). It zooms out to swap in
 *   the selected level's picture (states 1 -> 2 -> 0), wobbles on the sine
 *   table while shown (state 3) and zooms away on exit (states 4 -> 5).
 *   Four sprite parts ("twinkles") sit on top of it, each showing a random
 *   frame for a random time and blinking along with the wobble.
 * - LevelSelectEntry's methods (gLevelSelectEntryVtable: 1 Animate, the
 *   bob; 2 SetLevel; 3 SetPos; 4 Draw; 5 the destructor; the constructor
 *   starts issue #29).
 *
 * GitHub issue #29: 0x0801DFEC-0x0801E578: LevelSelectEntry's constructor,
 * then LevelSelectCursor, the screen's cursor (`LevelSelect::panel`). It
 * glides between entries along a Bresenham line (two steps per frame),
 * plays an idle animation cycle at random intervals, and grows in (state
 * 4) / shrinks away (state 5) as an affine OBJ drawn straight into the
 * OAM shadow buffer.
 *
 * UNUSED - no `bl`/`.4byte` reference in asm/, data/ or src/, and no
 * Thumb pointer anywhere in the ROM: LevelSelectCursor::IsHidden,
 * IsGrowing and MoveTo (Park has it inlined). Matched anyway.
 *
 * old_agbcp (Makefile OLD_AGBCC_OBJS), as its C was old_agbcc. */

/* Destructor (`LevelSelect::bg2`). */
ZoomBg::~ZoomBg()
{
    gPaletteCache->Unlock(twinkles[0].part->bank->anims[twinkles[0].part->tag].paletteId);
    delete twinkles[3].part;
    delete twinkles[2].part;
    delete twinkles[1].part;
    delete twinkles[0].part;
}

/* Per-frame state machine:
 * 0 zoom in to 1:1, then 3; 1 zoom out, then 2; 2 load the requested
 * picture (if any) and go to 0; 3 shown, wobbling; 4 zoom out while
 * rotating, then 5 (gone). The twinkles advance in every state. */
void ZoomBg::Update()
{
    u8 buf[0x200];

    switch (state) {
    case 0:
        if (scale <= 0xFF) {
            scale += 8;
            break;
        }
        scale = 0x100;
        state = 3;
        break;
    case 1:
        if (scale > 8) {
            scale -= 8;
            break;
        }
        scale = 8;
        state = 2;
        break;
    case 3:
        phase = (phase + 1) & 0xFF;
        /* `* 4 >> 8`, not `>> 6`: gcc's combiner turns the latter into
         * ldrh+lsl+asr instead of the ROM's ldrsh+asr. */
        dx = Q8_MUL(SIN_Q8(phase), 4);
        dy = Q8_MUL(SIN_Q8(phase << 1), 4);
        break;
    case 2:
        if (image == 11)
            break;
        gAudioContext->PlaySfx(SFX_ZOOM_BG_IN, 0x100);
        LoadTaggedAsset(gLevelSelectPictures[image].palette, buf);
        DmaCopy16(3, buf, BG_PLTT, 0x40);
        LoadTaggedAsset(gLevelSelectPictures[image].tiles, (void *)(BG_VRAM + (charBlock << 14)));
        state = 0;
        break;
    case 4:
        if (scale > 8) {
            scale -= 8;
            alpha += 0x100;
            break;
        }
        scale = 8;
        state = 5;
        break;
    }
    TickTwinkle(&twinkles[0]);
    TickTwinkle(&twinkles[1]);
    TickTwinkle(&twinkles[2]);
    TickTwinkle(&twinkles[3]);
}

/* Draw step: moves the twinkles with the wobble, then rebuilds the BG2
 * affine matrix from the current zoom (rotation only in states 4-5). */
void ZoomBg::Draw()
{
    if (IsShown()) {
        MoveTwinkle(&twinkles[0]);
        MoveTwinkle(&twinkles[1]);
        MoveTwinkle(&twinkles[2]);
        MoveTwinkle(&twinkles[3]);
    }
    switch (state) {
    case 0 ... 3:
        {
            u16 s;

            x16 = x + dx;
            y16 = y + dy;
            s = 0x10000 / scale;
            sx = s;
            sy = s;
            break;
        }
    case 4 ... 5:
        sx = scale;
        sy = scale;
        break;
    }
    BgAffineSet(&texX, &pa, 1);
}

/* Commits the affine matrix to BG2PA-BG2Y. */
void ZoomBg::Commit()
{
    REG_BG2PA = pa;
    REG_BG2PB = pb;
    REG_BG2PC = pc;
    REG_BG2PD = pd;
    REG_BG2X = bgx;
    REG_BG2Y = bgy;
}

/* Zooming away (state 4). */
u8 ZoomBg::IsExiting()
{
    return state == 4;
}

/* Gone (state 5). */
u8 ZoomBg::IsGone()
{
    return state == 5;
}

/* Shown (state 3). */
u8 ZoomBg::IsShown()
{
    return state == 3;
}

/* Waiting for a picture (state 2). */
u8 ZoomBg::IsWaiting()
{
    return state == 2;
}

/* Zooming out (state 1). */
u8 ZoomBg::IsZoomingOut()
{
    return state == 1;
}

/* Starts the exit zoom, with BG2 at the front. */
void ZoomBg::StartExit()
{
    bgcnt.bits16.priority = 0;
    state = 4;
}

/* Zooms the picture out with no follow-up picture (page turn). */
void ZoomBg::ClearPicture()
{
    gAudioContext->PlaySfx(SFX_ZOOM_BG_OUT, 0x100);
    state = 1;
    image = 11;
}

/* Requests picture `image`, taken once the zoom-out finishes. */
void ZoomBg::SetPicture(s32 image)
{
    switch (state) {
    case 1 ... 2:
        this->image = image;
        break;
    }
}

/* Moves a blinking twinkle by the wobble offset. */
void ZoomBg::MoveTwinkle(Twinkle *t)
{
    if (t->blink > 0 && (t->timer & 4))
        t->part->DrawWithOffset(dx, dy);
}

/* Picks a new random frame, duration and blink window for a twinkle. */
void ZoomBg::RandomizeTwinkle(Twinkle *t)
{
    ShowFrame(t->part, (u16)RandRange(8));
    t->timer = (u16)RandRange(0x3C);
    if (t->timer != 0)
        t->blink = (u16)RandRange(t->timer / 2 + 1);
}

void ZoomBg::TickTwinkle(Twinkle *t)
{
    if (t->timer != 0) {
        t->timer--;
        t->blink--;
    } else {
        RandomizeTwinkle(t);
    }
}

u16 ZoomBg::GetControl()
{
    return bgcnt.raw;
}

u8 LevelSelectEntry::IsSelected()
{
    return selected;
}

s32 LevelSelectEntry::GetLevel()
{
    return id;
}

/* Slot 1: bobs the entry by `phase` (a byte; 0xA0-0xFF wrap to a small
 * upward offset), the icon 2 px lower while selected, and shows the box's
 * selected/unselected frame. */
void LevelSelectEntry::Animate(s32 phase)
{
    s32 dy;

    phase &= 0xFF;
    if (phase <= 0x9F)
        dy = -phase;
    else
        dy = 0x100 - phase;
    if (selected)
        icon->DrawWithOffset(0, dy + 2);
    else
        icon->DrawWithOffset(0, dy);
    if (selected)
        ShowFrame(frame, 1);
    else
        ShowFrame(frame, 0);
    frame->DrawWithOffset(0, dy);
}

void LevelSelectEntry::SetSelected(u8 value)
{
    selected = value;
}

/* Slot 2: entry `index` of world `world`. Indices 0-4 are levels (icon
 * frame = level id); anything past that is the world's extra entry (level
 * id 0x14 + world, its own icon animation). */
void LevelSelectEntry::SetLevel(s32 world, s32 index)
{
    if (index <= 4) {
        id = world * LEVELS_PER_WORLD + index;
        ShowFrame(icon, id);
    } else {
        id = world + LEVEL_FIRST_BOSS;
        StartAnim(icon, gLevelSelectEntryWorldAnims[world]);
    }
}

/* Sets the box's animation (gLevelSelectEntryBoxAnims[kind]) and refreshes
 * both parts' palettes. */
void LevelSelectEntry::SetBox(s32 kind)
{
    StartAnim(frame, gLevelSelectEntryBoxAnims[kind]);
    frame->palette = frame->GetAnimPaletteSlot();
    icon->palette = icon->GetAnimPaletteSlot();
}

/* Slot 3: places the entry at pixel `pos`, the icon 3 px higher. The
 * frame's position is SetEntityPixelPos out of line, as in the ROM. */
void LevelSelectEntry::SetPos(const struct vec2 *pos)
{
    icon->SetPixelPos(pos->x, pos->y - 3);
    SetEntityPixelPos(frame, pos->x, pos->y);
}

/* Slot 4: the per-frame draw LevelSelect::Draw calls on every entry. Empty
 * for this class. */
void LevelSelectEntry::Draw()
{
}

/* Slot 5: the destructor. */
LevelSelectEntry::~LevelSelectEntry()
{
    delete icon;
    delete frame;
}

/* One affine parameter; OBJ matrix `m` is entries 4m..4m+3. An inline so
 * that the value is loaded before the address is computed. */
static inline void SetAffineParam(OamBuffer *buf, s32 n, u16 v)
{
    buf->table[n].attr[3] = v;
}

LevelSelectEntry::LevelSelectEntry()
{
    selected = 0;
    frame = new UiSprite;
    SetBankNow(frame, AnimTable(0x24C));
    frame->SetPriority(1);
    icon = new UiSprite;
    SetBankNow(icon, AnimTable(0x264));
    icon->SetPriority(1);
}

LevelSelectCursor::LevelSelectCursor()
{
    UiSprite *p = new UiSprite;

    part = p;
    p->bank = AnimTable(0x240);
    StartAnim(p, 0);
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
    angle = 0;
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
            StartAnim(part, gLevelSelectCursorAnims[1]);
        }
        break;
    case 1:
        part->AdvanceAnim();
        if (part->animDone) {
            state = 2;
            StartAnim(part, gLevelSelectCursorAnims[2]);
        }
        break;
    case 2:
        part->AdvanceAnim();
        if (part->animDone) {
            state = 3;
            StartAnim(part, gLevelSelectCursorAnims[3]);
        }
        break;
    case 3:
        part->AdvanceAnim();
        if (part->animDone) {
            state = 0;
            StartAnim(part, gLevelSelectCursorAnims[0]);
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

    sx = s;
    sy = s;
    ObjAffineSet(&sx, matrix, 1, 2);
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
