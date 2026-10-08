#include "level_select.hpp"

extern "C" {
#include "audio.h"
#include "math_util.h"
}

/* GitHub issue #27: 0x0801CEE0-0x0801DA38. The rest of the level-select
 * screen (LevelSelect, level_select.cpp; #664, include/level_select.hpp)
 * and its two background layers:
 *
 * - LevelSelect::TurnPage-ReloadPalette: the page-turn animation (TurnPage,
 *   driven by the Down/Up handlers PrevWorld/NextWorld), the frame loops of
 *   the A (Confirm) and Start (Exit) exits, the "previous/next page open"
 *   tests, and the per-page entry refresh (PlaceEntries/LoadEntries/
 *   SetEntryBoxes, also inlined into TurnPage).
 * - LevelSelectPageBg: BG1, the page strip whose vertical scroll eases 8
 *   per frame toward a Q8 target (0x100 = one page).
 * - ZoomBg's constructor, BG2: clears its screen block, writes an 8x4 tile
 *   block and spawns four corner sprites (mirrored per corner).
 *
 * UNUSED - no `bl`/`.4byte` reference in asm/, data/ or src/ and no Thumb
 * pointer anywhere in the ROM: LevelSelect::CommitFrame. Matched anyway.
 *
 * old_agbcp (Makefile OLD_AGBCC_OBJS), as its C was old_agbcc: it builds
 * the constant operand of a byte read-modify-write before the load, as
 * the ROM does. */

/* Runs the page-turn animation: steps BG1's scroll toward its target one
 * frame at a time, and halfway through (scroll 0xA0) swaps the page's
 * entries over to the new page. */
void LevelSelect::TurnPage()
{
    while (!bg1->IsSettled()) {
        BeginFrame();
        bg1->Scroll();
        panel->Update();
        if ((bg1->GetScroll() & 0xFF) == 0xA0) {
            LoadItems();
            PlaceItems();
            SkinItems();
        }
    }
}

/* Runs frames until the cursor settles. */
void LevelSelect::WaitCursor()
{
    while (!panel->HasArrived()) {
        BeginFrame();
        panel->Update();
        bg2->Update();
    }
}

/* A pressed on an open entry: sound 0x52, move the cursor to the middle,
 * let the picture play its selection, then fade out. */
void LevelSelect::Confirm()
{
    s32 t;

    PlaySfx(gAudioContext, SFX_LEVEL_SELECT_CONFIRM, 0x100);
    items[index]->SetSelected(0);
    panel->Move(0x78, 0x35);
    panel->Hide();
    WaitCursor();
    while (!bg2->IsShown()) {
        BeginFrame();
        panel->Update();
        bg2->Update();
    }
    blend.bits.effect = 3;
    blend.bits.bdFirst = 1;
    blend.bits.bg0First = 1;
    blend.bits.bg1First = 1;
    blend.bits.bg2First = 1;
    blend.bits.bg3First = 1;
    blend.bits.objFirst = 1;
    bldy.evy = 0;
    t = 0;
    bg2->StartExit();
    while (!bg2->IsGone()) {
        BeginFrame();
        panel->Update();
        bg2->Update();
        t++;
        bldy.evy = t / 2;
    }
}

/* Start: sound 0x49, fade to black, and leave with `result` 1. */
void LevelSelect::Exit()
{
    s32 t;

    PlaySfx(gAudioContext, SFX_MENU_SELECT, 0x100);
    blend.bits.effect = 3;
    blend.bits.bdFirst = 1;
    blend.bits.bg0First = 1;
    blend.bits.bg1First = 1;
    blend.bits.bg2First = 1;
    blend.bits.bg3First = 1;
    blend.bits.objFirst = 1;
    bldy.evy = 0;
    t = 0;
    bg2->ClearPicture();
    while (!bg2->IsWaiting()) {
        BeginFrame();
        panel->Update();
        bg2->Update();
        t++;
        bldy.evy = t / 2;
    }
    result = 1;
}

void SetNewWorldOpened(void)
{
    gNewWorldOpened = 1;
}

/* Whether there is a previous page (`world != 0`, spelled as the ROM's
 * branchless neg/orr/lsr; a plain comparison compiles to a branch). */
u8 LevelSelect::HasPrevWorld()
{
    u32 w = world;

    return (-w | w) >> 31;
}

/* Whether the next page has been opened (save byte 2, bits 5/7/6). */
u8 LevelSelect::IsNextWorldOpen()
{
    u8 r = 0;

    switch (world) {
    case 0:
        r = (save->flags >> 5) & 1;
        break;
    case 1:
        r = (save->flags >> 7) & 1;
        break;
    case 2:
        r = (save->flags >> 6) & 1;
        break;
    }
    return r;
}

/* Page changed: switch the page title sprite's animation and put the
 * cursor back on the (clamped) entry. */
void LevelSelect::RefreshPage()
{
    StartAnim(sprites[0], gLevelSelectWorldAnims[world]);
    LIMIT_MAX(index, lastIndex);
    {
        const struct xy_pair *pos = &positions[index];

        panel->Move(pos->x, pos->y - 0x18);
    }
}

/* Down: turn back one page (repeating while Down is held); sound 0x48 on
 * the first page. The loop is written with gotos for the ROM's block
 * order (the test after the body, entered by a jump): a `while` gets its
 * test copied in front of the loop. */
void LevelSelect::PrevWorld()
{
    if (HasPrevWorld()) {
        SettlePage();
        PlaySfx(gAudioContext, SFX_LEVEL_SELECT_PREV_WORLD, 0x100);
        goto check;
    loop:
        world--;
        bg1->TurnBack();
        TurnPage();
        UpdateKeys(gInput);
        if (!(gKeys.all & DPAD_DOWN))
            goto done;
    check:
        if (HasPrevWorld())
            goto loop;
    done:
        RefreshPage();
    } else {
        PlaySfx(gAudioContext, SFX_MENU_ERROR, 0x100);
    }
}

/* Up: turn forward one page (repeating while Up is held) while the next
 * page is open; sound 0x48 otherwise. */
void LevelSelect::NextWorld()
{
    if (IsNextWorldOpen()) {
        SettlePage();
        PlaySfx(gAudioContext, SFX_LEVEL_SELECT_NEXT_WORLD, 0x100);
        goto check;
    loop:
        world++;
        bg1->TurnForward();
        TurnPage();
        UpdateKeys(gInput);
        if (!(gKeys.all & DPAD_UP))
            goto done;
    check:
        if (IsNextWorldOpen())
            goto loop;
    done:
        RefreshPage();
    } else {
        PlaySfx(gAudioContext, SFX_MENU_ERROR, 0x100);
    }
}

void LevelSelect::PlaceEntries()
{
    PlaceItems();
}

void LevelSelect::LoadEntries()
{
    LoadItems();
}

void LevelSelect::SetEntryBoxes()
{
    SkinItems();
}

/* UNUSED - no caller anywhere in the ROM. One frame of the screen
 * without the menu's own update. */
void LevelSelect::CommitFrame()
{
    WaitForVBlank();
    gPaletteCache->Upload();
    gOamBuffer->Commit();
    CommitDisplay();
}

/* Reloads the palette and re-applies it to the eight sprites and the
 * page entries. */
void LevelSelect::ReloadPalette()
{
    s32 i;

    gPaletteCache->ClaimSlot(0xF);
    for (i = 0; i <= 7; i++)
        sprites[i]->palette = sprites[i]->GetAnimPaletteSlot();
    SetEntryBoxes();
}

s32 LevelSelectPageBg::GetScroll()
{
    return scroll;
}

u8 LevelSelectPageBg::IsSettled()
{
    u8 r = 0;

    if (scroll == target)
        r = 1;
    return r;
}

void LevelSelectPageBg::TurnBack()
{
    target += 0x100;
}

void LevelSelectPageBg::TurnForward()
{
    target -= 0x100;
}

/* Eases `scroll` 8 per frame toward `target` and scrolls BG1 with it. */
void LevelSelectPageBg::Scroll()
{
    if (scroll < target)
        scroll += 8;
    if (scroll > target)
        scroll -= 8;
    vofs = scroll;
}

/* BG1HOFS and BG1VOFS as one word. */
u32 LevelSelectPageBg::GetOffsets()
{
    return *(u32 *)&hofs;
}

void LevelSelectPageBg::SetOffsets()
{
    hofs = 8;
    vofs = scroll + 0x30;
}

LevelSelectPageBg::~LevelSelectPageBg()
{
}

LevelSelectPageBg::LevelSelectPageBg(s32 charBlock, s32 screenBlock)
{
    InitBgSetup(&bg, charBlock, screenBlock, 0, 2);
    scroll = target = 0x300;
    LoadGraphicsPackage(&bg, &gLevelSelectPageBg);
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
    texY = texX = 0x2000;
    x16 = x;
    y16 = y;
    alpha = 0;
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
