/* ZoomBg (include/level_select.hpp): the level picture on the affine BG2
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

#include "level_select.hpp"
#include "audio.hpp"

extern "C" {
#include <agb_syscall.h>
#include "math_util.h"
}

/* Setters as the original's inline member functions: storing a parameter
 * (rather than a literal) into the bitfield keeps the ROM's full
 * clear-then-or sequence, and lets the loop hoist the `1` for SetMode
 * into a register. */
static inline void SetMode(Sprite *s, s32 mode)
{
    s->mirrorBits.gfxMode = mode;
}

static inline void SetFlipX(Sprite *s, u8 on)
{
    s->mirrorFlags.mirrorX = on;
}

static inline void SetFlipY(Sprite *s, u8 on)
{
    s->mirrorFlags.mirrorY = on;
}

static inline void SetPalette(Sprite *s, s32 pal)
{
    s->palette = pal;
}

/* BG2's constructor: the BGCNT fields (priority 1, 256 colours), a cleared
 * screen block with an 8x4 block of tile entries at its top left, and four
 * corner sprites (animation bank `+0x258` of the level graphics, mirrored
 * per corner) positioned around (x, y) = (0x78, 0x35) by
 * gZoomBgSlotOffsets. */
ZoomBg::ZoomBg(s32 charBlock, s32 screenBlock)
{
    s32 i;

    x = 0x78;
    y = 0x35;
    bgcnt.raw = 0;
    bgcnt.bits.priority = 1;
    this->charBlock = charBlock;
    bgcnt.bits.charBase = charBlock;
    bgcnt.bits.screenSize = 0;
    this->screenBlock = screenBlock;
    bgcnt.bits.screenBase = screenBlock;
    bgcnt.bits.color256 = 1;
    {
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
    }
    image = 0xB;
    state = 2;
    scale = 8;
    dx = dy = phase = 0;
    affine.texY = affine.texX = 0x2000;
    affine.scrX = x;
    affine.scrY = y;
    affine.angle = 0;
    for (i = 0; i <= 3; i++) {
        twinkles[i].part = new UiSprite;
        SetBankNow(twinkles[i].part, AnimTable(0x258));
        SetMode(twinkles[i].part, 1);
        twinkles[i].part->SetPixelPos(x + gZoomBgSlotOffsets[i].x, y + gZoomBgSlotOffsets[i].y);
        twinkles[i].part->SetPriority(1);
        SetPalette(twinkles[i].part, twinkles[0].part->GetAnimPaletteSlot());
        RandomizeTwinkle(&twinkles[i]);
    }
    gPaletteCache->Lock(twinkles[0].part->bank->anims[twinkles[0].part->tag].paletteId);
    SetFlipX(twinkles[1].part, 1);
    SetFlipY(twinkles[2].part, 1);
    SetFlipX(twinkles[3].part, 1);
    SetFlipY(twinkles[3].part, 1);
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
            affine.angle += 0x100;
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

            affine.scrX = x + dx;
            affine.scrY = y + dy;
            s = 0x10000 / scale;
            affine.sx = s;
            affine.sy = s;
            break;
        }
    case 4 ... 5:
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
