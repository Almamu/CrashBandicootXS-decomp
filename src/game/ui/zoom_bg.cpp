/* ZoomBg (include/game/ui/level_select.hpp): the level picture on the affine BG2
 * layer (`LevelSelect::bg2`). Its constructor clears its screen block,
 * writes an 8x4 tile block and spawns four corner sprites ("twinkles",
 * mirrored per corner). It zooms out to swap in the selected level's
 * picture (states 1 -> 2 -> 0), wobbles on the sine table while shown
 * (state 3) and zooms away on exit (states 4 -> 5). Each twinkle shows a
 * random frame for a random time and blinks along with the wobble.
 *
 * GitHub issues #27 (the constructor) and #28. The constructor ended
 * level_select_pages.cpp and the rest started level_select_widgets.cpp
 * until #767; all old_agbcc (Makefile OLD_AGBCC_OBJS). */

#include "game/ui/level_select.hpp"
#include "engine/audio/audio.hpp"

extern "C" {
#include <agb_syscall.h>
#include "engine/util/math_util.h"
#include "constants/themes.h"
}

using Game::Entities::Sprite;
using Game::Entities::UiSprite;
using Game::Ui::ZoomBg;

/* Setters as the original's inline member functions: storing a parameter
 * (rather than a literal) into the bitfield keeps the ROM's full
 * clear-then-or sequence, and lets the loop hoist the `1` for SetMode
 * into a register. */
static inline void SetMode(Sprite *s, s32 mode)
{
    s->mirrorBits.gfxMode = mode;
}

static inline void SetPalette(Sprite *s, s32 pal)
{
    s->palette = pal;
}

/* BG2's constructor: the BGCNT fields (priority 1, 256 colours), a cleared
 * screen block with an 8x4 block of tile entries at its top left, and four
 * corner sprites (animation bank 50 of the level graphics, mirrored
 * per corner) positioned around (x, y) = (DISPLAY_CENTER_X, 0x35) by
 * gZoomBgSlotOffsets. */
ZoomBg::ZoomBg(s32 charBlock, s32 screenBlock)
{
    s32 i;

    x = DISPLAY_CENTER_X;
    y = DISPLAY_ART_Y + 0x35;
    bgcnt.raw = 0;
    bgcnt.bits.priority = 1;
    this->charBlock = charBlock;
    bgcnt.bits.charBase = charBlock;
    bgcnt.bits.screenSize = 0;
    this->screenBlock = screenBlock;
    bgcnt.bits.screenBase = screenBlock;
    bgcnt.bits.color256 = 1;

    u16 *dst = (u16 *)(VRAM + (this->screenBlock << 11));
    u32 v;
    s32 row;

    DmaFill16(3, 0, dst, 0x100);
    v = 0x100;
    for (row = 0; row <= 7; row++) {
        s32 j;

        for (j = 0; j <= 3; j++) {
            dst[j] = v;
            v += 0x202;
        }
        dst += 8;
    }
    image = ZOOM_BG_NO_PICTURE;
    state = ZOOM_BG_STATE_WAIT;
    scale = LEVEL_SELECT_ZOOM_MIN;
    dx = dy = phase = 0;
    /* the centre of the 64x64 picture */
    affine.texY = affine.texX = INT_TO_Q8(32);
    affine.scrX = x;
    affine.scrY = y;
    affine.angle = 0;
    for (i = 0; i <= 3; i++) {
        twinkles[i].part = new UiSprite;
        SetBankNow(twinkles[i].part, gSpriteBankSet->Bank(50));
        SetMode(twinkles[i].part, OAM_OBJ_MODE_SEMI_TRANSPARENT);
        twinkles[i].part->SetPixelPos(x + gZoomBgSlotOffsets[i].x, y + gZoomBgSlotOffsets[i].y);
        twinkles[i].part->SetPriority(1);
        SetPalette(twinkles[i].part, twinkles[0].part->GetAnimPaletteSlot());
        RandomizeTwinkle(&twinkles[i]);
    }
    gPaletteCache->Lock(twinkles[0].part->bank->anims[twinkles[0].part->tag].paletteId);
    twinkles[1].part->SetFlipX(1);
    twinkles[2].part->SetFlipY(1);
    twinkles[3].part->SetFlipX(1);
    twinkles[3].part->SetFlipY(1);
}

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
    case ZOOM_BG_STATE_ZOOM_IN:
        if (scale <= INT_TO_Q8(1) - 1) {
            scale += ZOOM_BG_ZOOM_STEP;
            break;
        }
        scale = INT_TO_Q8(1);
        state = ZOOM_BG_STATE_SHOWN;
        break;
    case ZOOM_BG_STATE_ZOOM_OUT:
        if (scale > LEVEL_SELECT_ZOOM_MIN) {
            scale -= ZOOM_BG_ZOOM_STEP;
            break;
        }
        scale = LEVEL_SELECT_ZOOM_MIN;
        state = ZOOM_BG_STATE_WAIT;
        break;
    case ZOOM_BG_STATE_SHOWN:
        phase = (phase + 1) & 0xFF;
        /* `* 4 >> 8`, not `>> 6`: gcc's combiner turns the latter into
         * ldrh+lsl+asr instead of the ROM's ldrsh+asr. */
        dx = Q8_MUL(SIN_Q8(phase), 4);
        dy = Q8_MUL(SIN_Q8(phase << 1), 4);
        break;
    case ZOOM_BG_STATE_WAIT:
        if (image == ZOOM_BG_NO_PICTURE)
            break;
        gAudioContext->PlaySfx(SFX_ZOOM_BG_IN, 0x100);
        LoadTaggedAsset(gLevelSelectPictures[image].palette, buf);
        DmaCopy16(3, buf, BG_PLTT, 2 * PALETTE_SIZE_16);
        LoadTaggedAsset(gLevelSelectPictures[image].tiles, (void *)(BG_VRAM + (charBlock << 14)));
        state = ZOOM_BG_STATE_ZOOM_IN;
        break;
    case ZOOM_BG_STATE_EXIT:
        if (scale > LEVEL_SELECT_ZOOM_MIN) {
            scale -= ZOOM_BG_ZOOM_STEP;
            affine.angle += 0x100;
            break;
        }
        scale = LEVEL_SELECT_ZOOM_MIN;
        state = ZOOM_BG_STATE_GONE;
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
    case ZOOM_BG_STATE_ZOOM_IN ... ZOOM_BG_STATE_SHOWN:
        {
            u16 s;

            affine.scrX = x + dx;
            affine.scrY = y + dy;
            /* the BG's step: 1 / scale */
            s = Q8_DIV(INT_TO_Q8(1), scale);
            affine.sx = s;
            affine.sy = s;
            break;
        }
    case ZOOM_BG_STATE_EXIT ... ZOOM_BG_STATE_GONE:
        affine.sx = scale;
        affine.sy = scale;
        break;
    }
    BgAffineSet(&affine, &pa, 1);
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
    return state == ZOOM_BG_STATE_EXIT;
}

/* Gone (state 5). */
u8 ZoomBg::IsGone()
{
    return state == ZOOM_BG_STATE_GONE;
}

/* Shown (state 3). */
u8 ZoomBg::IsShown()
{
    return state == ZOOM_BG_STATE_SHOWN;
}

/* Waiting for a picture (state 2). */
u8 ZoomBg::IsWaiting()
{
    return state == ZOOM_BG_STATE_WAIT;
}

/* Zooming out (state 1). */
u8 ZoomBg::IsZoomingOut()
{
    return state == ZOOM_BG_STATE_ZOOM_OUT;
}

/* Starts the exit zoom, with BG2 at the front. */
void ZoomBg::StartExit()
{
    bgcnt.bits16.priority = 0;
    state = ZOOM_BG_STATE_EXIT;
}

/* Zooms the picture out with no follow-up picture (page turn). */
void ZoomBg::ClearPicture()
{
    gAudioContext->PlaySfx(SFX_ZOOM_BG_OUT, 0x100);
    state = ZOOM_BG_STATE_ZOOM_OUT;
    image = ZOOM_BG_NO_PICTURE;
}

/* Requests picture `image`, taken once the zoom-out finishes. */
void ZoomBg::SetPicture(s32 image)
{
    switch (state) {
    case ZOOM_BG_STATE_ZOOM_OUT ... ZOOM_BG_STATE_WAIT:
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
    ShowFrame(t->part, RandRange(8));
    t->timer = RandRange(0x3C);
    if (t->timer != 0)
        t->blink = RandRange(t->timer / 2 + 1);
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
