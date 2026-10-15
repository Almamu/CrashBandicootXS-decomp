#include "game/ui/level_select.hpp"
#include "engine/audio/audio.hpp"
#include "game/level/level_state.hpp"
#include "engine/sys/key_input.hpp"
#include "engine/sys/frame_limiter.hpp"

extern "C" {
#include <agb_syscall.h>
#include <libgcc.h>
#include "engine/text/text.h"
#include "game/level/level.h"
#include "engine/util/math_util.h"
}

using Engine::Gfx::BgSetup;
using Engine::Gfx::PaletteCache;
using Engine::Sys::FrameLimiter;
using Engine::Text::Font;
using Game::Entities::UiSprite;
using Game::Ui::LevelSelect;
using Game::Ui::LevelSelectCursor;
using Game::Ui::LevelSelectEntry;
using Game::Ui::LevelSelectPageBg;
using Game::Ui::ZoomBg;

/* GitHub issue #26: 0x0801B85C-0x0801CEE0 (engine/include/game/ui/level_select.hpp).
 * LevelSelect: the paged level-select screen. LevelSelect::Run is the whole
 * modal screen: it builds the menu, runs it (Loop) and returns the chosen
 * level through `*level`. Five levels per page (`level / 5`, `level % 5`;
 * fewer on a partial page, level_select_worlds.h); each
 * level's fixed record (name text, three time-trial thresholds) is
 * gLevelTable, its saved record a bitfield word (cleared flag, two more
 * flags, best time). The rest of its methods are in
 * level_select_pages.cpp. The file started with CameraLead and LaunchPad,
 * objects/camera_lead.cpp and objects/launch_pad.cpp since #767.
 *
 * old_agbcp (Makefile OLD_AGBCC_OBJS), as its C was old_agbcc. */

/* codegen: gLevelSelectGemPos and gLevelSelectTrialIconPos are const
 * (menus.hpp), but the constructor reads each one twice, across calls, and
 * the ROM loads it again each time: through the const object gcc keeps the
 * first loads in registers. docs/headers.md */
extern "C" struct vec2 gLevelSelectGemPos_rw asm("gLevelSelectGemPos");
extern "C" struct vec2 gLevelSelectTrialIconPos_rw asm("gLevelSelectTrialIconPos");

/* The level-select screen, modal (called from game_frame.cpp): resets the
 * display, palette, VRAM cursor and both fonts (the same setup as
 * ShowPowerDialog), builds the menu for level `*level`, runs it, stores the
 * chosen level back through `level`, tears the menu down and returns its
 * `result` byte. The font steps are inline functions (Font::SetTileBase
 * and IconReserve): the ROM recomputes every field address after each
 * call instead of keeping the offsets in registers, which is what
 * separate inlined expansions give. */
static inline void IconReserve(Font **m)
{
    gObjVramCursor->Reserve((*m)->tileCount << 5);
}

static inline void LoadMenuPalette(PaletteCache *cache)
{
    CpuSet(gLevelSelectPalette, cache->slots[15], sizeof(gLevelSelectPalette) / sizeof(u16));
}

s32 LevelSelect::Run(s32 *level)
{
    LevelSelect *menu;
    /* `result` is needed: inlined, the instructions are reordered */
    u8 result;
    s32 heaps = 0xC0000000;

    mem_free_bytes(heaps);
    FrameLimiter::WaitForVBlank();
    BACKDROP_COLOR = 0;
    *(vu16 *)REG_ADDR_DISPCNT = 0;
    gPaletteCache->FreeUnlockedSlots();
    gPaletteCache->ClaimSlot(0xF);
    LoadMenuPalette(gPaletteCache);
    gObjVramCursor->baseTile = 0;
    gObjVramCursor->Reset();
    gObjVramCursor->Reset();
    gSmallFont->SetTileBase(0);
    IconReserve(&gSmallFont);

    /* `v` is needed: inlined, the instructions are reordered */
    u32 v = gSmallFont->tileCount;

    gLargeFont->SetTileBase(v);
    IconReserve(&gLargeFont);
    gObjVramCursor->Mark();
    gAudioContext->PlaySong(SCREEN_SONG_LEVEL_SELECT);

    LevelSelect **menuAddr = &instance;

    *menuAddr = new LevelSelect(*level);
    *level = (*menuAddr)->Loop();
    menu = *menuAddr;
    result = menu->result;
    delete menu;
    *menuAddr = 0;
    gPaletteCache->FreeUnlockedSlots();
    mem_free_bytes(heaps);
    return result;
}

/* The constructor: blend/display shadow registers (alpha blend, evy 16,
 * mode 1, BG0/BG1/OBJ), page/cursor from `level` (its world and entry; a
 * boss is its page's entry after the levels), the save block, both BG layers, the cursor, six level
 * entries and ten sprites, then the cursor position and the BG registers.
 * The six-entry loop needs its own counter and the sprite loop's 0x80 a
 * variable set with the counter, for the ROM's register choice and hoisted
 * constant. */
LevelSelect::LevelSelect(s32 level)
{
    s32 i;
    UiSprite *s;

    blend.raw = 0;
    blend.bits.effect = 3;
    blend.bits.bdFirst = 1;
    blend.bits.bg0First = 1;
    blend.bits.bg1First = 1;
    blend.bits.bg2First = 1;
    blend.bits.bg3First = 1;
    blend.bits.objFirst = 1;
    bldy.evy = 16;
    *(vu32 *)REG_ADDR_BLDCNT = blend.raw;
    *(vu16 *)REG_ADDR_BLDY = bldy.evy;
    dispcnt.raw = 0;
    dispcnt.bits.objMap1D = 1;
    dispcnt.bits.mode = DISPCNT_MODE_1;
    dispcnt.bits.bg0 = 1;
    dispcnt.bits.bg1 = 1;
    dispcnt.bits.obj = 1;
    if (level <= LEVEL_LAST_NUMBERED) {
        world = LEVEL_SELECT_WORLD_OF_LEVEL(level);
        index = LEVEL_SELECT_INDEX_OF_LEVEL(level, world);
    } else {
        world = LEVEL_SELECT_WORLD_OF_BOSS(level);
        index = LEVEL_SELECT_WORLD_LEVELS(world);
    }
    nameText = 0;
    save = gLevelState->PackSaveData();
    result = 0;
    bg1 = new LevelSelectPageBg(0, 0x1D);
    BgSetup bg0cnt(2, 0x1E, 2, 3);
    bg0cnt.Load(&gMenuSkyBg);
    scroll = 0;
    panel = new LevelSelectCursor;
    bg2 = new ZoomBg(3, 0x1F);

    s32 j;

    for (j = 0; j < LEVEL_SELECT_ENTRIES; j++)
        items[j] = new LevelSelectEntry;
    LoadEntries();
    PlaceEntries();
    SetEntryBoxes();

    s32 v;

    /* the sprites past the first two at half size */
    for (i = 0, v = Q8(0.5); i < 8; i++) {
        s = new UiSprite;
        sprites[i] = s;
        s->SetPriority(1);
        if (i > 1)
            sprites[i]->affine = v;
    }
    SetBankNow(sprites[0], gSpriteBankSet->Bank(47));
    sprites[0]->StartAnim(gLevelSelectWorldAnims[world]);
    SetEntityPixelPos(sprites[0], gLevelSelectWorldPos.x, gLevelSelectWorldPos.y);
    SetBankNow(sprites[1], gSpriteBankSet->Bank(47));
    sprites[1]->StartAnim(10);
    SetEntityPixelPos(sprites[1], gLevelSelectCrashIconPos.x, gLevelSelectCrashIconPos.y);
    SetBankNow(sprites[2], gSpriteBankSet->Bank(37));
    SetEntityPixelPos(sprites[2], gLevelSelectCrystalPos.x, gLevelSelectCrystalPos.y);
    SetBankNow(sprites[3], gSpriteBankSet->Bank(32));
    sprites[3]->StartAnim(1);
    SetEntityPixelPos(sprites[3], gLevelSelectGemPos_rw.x, gLevelSelectGemPos_rw.y);
    SetBankNow(sprites[4], gSpriteBankSet->Bank(32));
    sprites[4]->StartAnim(1);
    SetEntityPixelPos(sprites[4], gLevelSelectGemPos_rw.x, gLevelSelectGemPos_rw.y);
    SetBankNow(sprites[5], gSpriteBankSet->Bank(33));
    SetEntityPixelPos(sprites[5], gLevelSelectTrialIconPos_rw.x, gLevelSelectTrialIconPos_rw.y);
    SetBankNow(sprites[6], gSpriteBankSet->Bank(33));
    SetEntityPixelPos(sprites[6], gLevelSelectTrialIconPos_rw.x, gLevelSelectTrialIconPos_rw.y);
    SetBankNow(sprites[7], gSpriteBankSet->Bank(33));
    SetEntityPixelPos(sprites[7], gLevelSelectTimePos.x, gLevelSelectTimePos.y);
    s = new UiSprite;
    sprites[8] = s;
    s->SetPriority(1);
    SetBankNow(sprites[8], gSpriteBankSet->Bank(52));
    sprites[8]->StartAnim(1);
    SetEntityPixelPos(sprites[8], gLevelSelectNextWorldArrowPos.x, gLevelSelectNextWorldArrowPos.y);
    s = new UiSprite;
    sprites[9] = s;
    s->SetPriority(1);
    SetBankNow(sprites[9], gSpriteBankSet->Bank(52));
    sprites[9]->StartAnim(0);
    SetEntityPixelPos(sprites[9], gLevelSelectPrevWorldArrowPos.x, gLevelSelectPrevWorldArrowPos.y);
    if (newWorldOpened && IsNextWorldOpen()) {
        panel->Park();
    } else {
        const struct vec2 *pos = &positions[index];

        panel->Move(pos->x, pos->y - 0x18);
    }
    *(vu32 *)REG_ADDR_BG0HOFS = 0;
    *(vu32 *)REG_ADDR_BG1HOFS = bg1->GetOffsets();
    *(vu16 *)REG_ADDR_BG0CNT = bg0cnt.GetControl();
    *(vu16 *)REG_ADDR_BG1CNT = bg1->bg.GetControl();
    *(vu16 *)REG_ADDR_BG2CNT = bg2->GetControl();
}

/* The destructor: deletes the sprites, the cursor, the BG layers and the
 * level entries. */
LevelSelect::~LevelSelect()
{
    s32 i;

    delete sprites[9];
    delete sprites[8];
    for (i = 0; i < 8; i++)
        delete sprites[i];
    delete panel;
    delete bg2;
    for (i = 0; i < LEVEL_SELECT_ENTRIES; i++)
        delete items[i];
    delete bg1;
}

/* Per-frame update: draws the selected level's name centred at the top
 * (gLargeFont) and its record panel, updates every entry, and once the BG1
 * page has settled draws text 0x2F centred at y=0x96 (the first time only,
 * with the page arrows) and steps BG2; BG2's DISPCNT enable bit follows
 * ZoomBg::IsWaiting. */
void LevelSelect::Update()
{
    s32 i;

    gOamBuffer->Reset();
    gObjVramCursor->Rewind();
    panel->Draw();
    if (bg2->IsShown() && items[index]->IsSelected()) {
        screen_ux_t x = (u32)(DISPLAY_WIDTH - gLargeFont->MeasureText(nameText)) >> 1;

        gLargeFont->SetPos(x, -panelSlideX + (DISPLAY_ART_Y + 2));
        gLargeFont->DrawText(nameText);
        if (index < LEVEL_SELECT_WORLD_LEVELS(world))
            DrawRecord();
    }
    bg1->SetOffsets();
    for (i = 0; i <= lastIndex; i++)
        items[i]->Animate(bg1->GetScroll());
    if (bg1->IsSettled()) {
        if (!bg2->IsExiting()) {
            const u8 *text = GetUiText(0x2F);
            screen_ux_t x = (u32)(DISPLAY_WIDTH - gSmallFont->MeasureText(text)) >> 1;

            gSmallFont->SetPos(x, DISPLAY_ART_Y + 0x96);
            gSmallFont->SetPalette(0xF);
            gSmallFont->DrawText(text);
            UpdatePageArrows();
        }
        bg2->Draw();
    }
    if (bg2->IsWaiting())
        dispcnt.bits.bg2 = 0;
    else
        dispcnt.bits.bg2 = 1;
    gOamBuffer->HideUnused();
}

/* Updates the two page-arrow sprites (8/9): palettes from
 * GetAnimPaletteSlot, frame 0/1 by whether the previous/next page is
 * open. The next-page arrow's ShowFrame is in both arms of the `if`, as
 * the ROM's two `ldr r1, [r6, #0x60]` show: the two copies share
 * everything after their frame constants (#662 round 2; the C pinned
 * the arrow to r1 and `&tag` to r3 for this). */
void LevelSelect::UpdatePageArrows()
{
    sprites[8]->palette = sprites[8]->GetAnimPaletteSlot();
    sprites[9]->palette = sprites[9]->GetAnimPaletteSlot();
    if (world < WORLD_COUNT - 1) {
        if (IsNextWorldOpen())
            ShowFrame(sprites[8], 0);
        else
            ShowFrame(sprites[8], 1);
        sprites[8]->DrawWithOffset(0, 0);
    }
    if (HasPrevWorld()) {
        UiSprite *s = sprites[9];

        ShowFrame(s, 0);
        s->DrawWithOffset(0, 0);
    }
}

/* Draws the record panel sprites 0-4 at their per-row offsets and, if the
 * level is cleared, its time readout (DrawTime). */
void LevelSelect::DrawRecord()
{
    sprites[0]->DrawWithOffset(-panelSlideX, 0);
    sprites[1]->DrawWithOffset(-panelSlideX, 0);
    sprites[2]->DrawWithOffset(-panelSlideX, clearedIconY);
    sprites[3]->DrawWithOffset(-panelSlideX, flag1IconY);
    if (rank != 5)
        sprites[4]->DrawWithOffset(-panelSlideX, gemIconY);

    struct level_save_h *sv = &save->levels[levelId].h;

    if (sv->cleared)
        DrawTime(sv->time);
}

/* Draws the time readout: just the best time if it beats the tightest
 * threshold (time2), otherwise sprite 7, the next threshold to beat and
 * the best time. */
void LevelSelect::DrawTime(u32 time)
{
    /* `info` is needed: inlined, the code is 2 instructions longer */
    const struct level_info *info;

    sprites[5]->DrawWithOffset(-panelSlideX, trialIconY);
    sprites[6]->DrawWithOffset(-panelSlideX, trialIcon2Y);
    info = &gLevelTable[levelId];
    if (time != 0 && time <= info->times[2]) {
        gLargeFont->SetPos(panelSlideX + gLevelSelectTimePos.x + 10, gLevelSelectTimePos.y - 8);
        gLargeFont->DrawText(timeText);
    } else {
        sprites[7]->DrawWithOffset(panelSlideX, 0);
        gLargeFont->SetPalette(sprites[7]->palette);
        gLargeFont->SetPos(panelSlideX + gLevelSelectTimePos.x + 10, gLevelSelectTimePos.y - 8);
        gLargeFont->DrawText(recordText);
        gLargeFont->ResetPalette();
        gLargeFont->SetPos(panelSlideX + gLevelSelectTimePos.x + 10, gLevelSelectTimePos.y + 8);
        gLargeFont->DrawText(timeText);
    }
}

/* Per-frame draw step: once the BG1 page has settled and the cursor has
 * arrived on a new entry, selects it and loads that level's name and
 * record (LoadRecord); then draws the six entries and steps record sprites
 * 2-7. */
void LevelSelect::Draw()
{
    LevelSelectEntry **it;
    /* `sp` is needed: inlined, the code is 1 instruction shorter */
    UiSprite **sp;
    s32 i;

    bg1->Scroll();
    panel->Update();
    if (!bg1->IsSettled())
        return;
    if (panel->HasArrived() && !items[index]->IsSelected()) {
        LevelSelectEntry *e = items[index];

        e->SetSelected(1);
        if (!bg2->IsShown()) {
            const struct level_info *info;

            levelId = e->GetLevel();
            info = &gLevelTable[levelId];
            bg2->SetPicture(info->theme);
            nameText = GetUiText(info->nameText);
        }
        panelSlideX = 0;
        if (index < LEVEL_SELECT_WORLD_LEVELS(world)) {
            LoadRecord();
            gPaletteCache->FreeUnlockedSlots();
            ReloadPalette();
        }
        gLargeFont->ResetPalette();
    }
    it = items;
    sp = sprites;
    for (i = LEVEL_SELECT_ENTRIES - 1; i >= 0; i--)
        (*it++)->Draw();
    bg2->Update();

    UiSprite **p = sp + 2;

    for (i = 5; i >= 0; i--)
        (*p++)->AdvanceAnim();
}

/* Loads the selected level's record into the panel: its `rank` (first of
 * five save predicates that holds, 5 if none), which flag icons to show (y
 * offsets 0 or 0x1C), and the time-trial texts/sprites. The ROM spills a
 * second copy of the record pointer to sp+0 and reloads it for the `time0`
 * test: that is the separate `entry` local, which `info` copies. */
void LevelSelect::LoadRecord()
{
    s32 *r = &rank;
    struct level_save_h *sv;

    *r = 5;
    if (gLevelState->LevelHasGemPathGem(levelId))
        *r = 0;
    if (gLevelState->LevelHasRedGem(levelId))
        *r = 1;
    if (gLevelState->LevelHasGreenGem(levelId))
        *r = 2;
    if (gLevelState->LevelHasBlueGem(levelId))
        *r = 3;
    if (gLevelState->LevelHasYellowGem(levelId))
        *r = 4;
    clearedIconY = 0;
    flag1IconY = 0;
    gemIconY = 0;
    trialIconY = 0;
    trialIcon2Y = 0;
    sv = &save->levels[levelId].h;
    if (sv->cleared)
        clearedIconY = 0x1C;
    if (sv->flag1)
        flag1IconY = 0x1C;
    switch (*r) {
    case 0:
        if (sv->flag2)
            gemIconY = 0x1C;
        break;
    case 1:
        if (save->flags & PROGRESS_FLAG_RED_GEM)
            gemIconY = 0x1C;
        break;
    case 2:
        if (save->flags & PROGRESS_FLAG_GREEN_GEM)
            gemIconY = 0x1C;
        break;
    case 3:
        if (save->flags & PROGRESS_FLAG_BLUE_GEM)
            gemIconY = 0x1C;
        break;
    case 4:
        if (save->flags & PROGRESS_FLAG_YELLOW_GEM)
            gemIconY = 0x1C;
        break;
    case 5:
        break;
    default:
        goto set_rank_icon;
    }
    if (rank != 5) {
    set_rank_icon:
        sprites[4]->StartAnim(gLevelSelectRankAnims[rank]);
        if (flag1IconY == gemIconY) {
            flag1IconY -= 6;
            gemIconY += 6;
        }
    }
    if (sv->cleared) {
        const struct level_info *entry = &gLevelTable[levelId];
        const struct level_info *info = entry;

        FormatCentiseconds(info->times[0], recordText);
        FormatCentiseconds(sv->time, timeText);
        sprites[5]->StartAnim(0);
        sprites[6]->StartAnim(0);
        sprites[7]->StartAnim(0);
        if (sv->time != 0) {
            if (sv->time <= info->times[2]) {
                trialIcon2Y = 0x1C;
                trialIconY = 0x1C;
                sprites[5]->StartAnim(1);
                sprites[6]->StartAnim(1);
            } else if (sv->time <= info->times[1]) {
                FormatCentiseconds(info->times[2], recordText);
                trialIconY = 0x1C;
                sprites[5]->StartAnim(2);
                sprites[6]->StartAnim(1);
                sprites[7]->StartAnim(1);
            } else if (sv->time <= entry->times[0]) {
                FormatCentiseconds(info->times[1], recordText);
                trialIconY = 0x1C;
                sprites[5]->StartAnim(0);
                sprites[6]->StartAnim(2);
                sprites[7]->StartAnim(2);
            }
        }
    }
}

/* The key state's halfwords, from a copy of the word passed by value
 * (LevelSelect::Loop, below). */
static inline struct held_pressed_pair KeyHalf(union key_state k)
{
    return k.half;
}

/* The menu loop: fades in (BLDY), then runs frames until A is pressed on
 * an open entry (Confirm) or Start exits (Exit), dispatching Up/Down page
 * turns (NextWorld/PrevWorld) and Left/Right cursor moves
 * (CursorLeft/CursorRight); returns the selected entry's level.
 *
 * The direction tests read the keys through KeyHalf, which takes the
 * key word by value (#662 round 4; a code-free asm kept a copy of the
 * word before). The inline's parameter and return value are each a
 * union in an ADDRESSOF pseudo, so cse1 sees the copies into and out of
 * them as memory moves; they only become register copies in the
 * addressof pass after it, and one of them survives the later passes:
 * the ROM's `adds r1, r2, #0` between the 0x80 test's `ands` and `cmp`,
 * which the 0x20 test reads. */
s32 LevelSelect::Loop()
{
    const struct level_info *info;
    /* `a` is needed: inlined, the instructions differ */
    u32 a;

    result = 0;
    levelId = items[index]->GetLevel();
    info = &gLevelTable[levelId];
    bg2->SetPicture(info->theme);
    nameText = GetUiText(info->nameText);
    while (!bg2->IsShown()) {
        if (bldy.evy != 0)
            bldy.evy--;
        BeginFrame();
        bg2->Update();
    }
    gAudioContext->PlaySfx(SFX_ZOOM_BG_SHOWN, 0x100);
    blend.raw = 0;
    blend.bits.bg0Second = 1;
    blend.bits.bg1Second = 1;
    blend.bits.bg2Second = 1;
    blend.bits.bg3Second = 1;
    blend.bits.bdSecond = 1;
    blend.bits.eva = 0x10;
    blend.bits.evb = 0x10;
    if (newWorldOpened && IsNextWorldOpen()) {
        index = 0;
        NextWorld();
    }
    newWorldOpened = 0;
    goto loop;

check_exit:
    if (gKeys.half.pressed & START_BUTTON) {
        Exit();
        goto end;
    }
loop:
    BeginFrame();
    Draw();
    if (!bg1->IsSettled())
        goto loop;
    if (!panel->HasArrived())
        goto loop;
    gInput->Update();
    if (KeyHalf(gKeys).pressed & DPAD_UP)
        NextWorld();
    else if (KeyHalf(gKeys).pressed & DPAD_DOWN)
        PrevWorld();
    else if (KeyHalf(gKeys).pressed & DPAD_LEFT)
        CursorLeft();
    else if (KeyHalf(gKeys).pressed & DPAD_RIGHT)
        CursorRight();
    a = gKeys.half.pressed & A_BUTTON;

    if (!a)
        goto check_exit;
    if (!items[index]->IsSelected())
        goto check_exit;
    if (!bg2->IsShown())
        goto check_exit;
    Confirm();
end:
    dispcnt.raw = 0;
    dispcnt.bits.objMap1D = 1;
    FrameLimiter::WaitForVBlank();
    gPaletteCache->Upload();
    gOamBuffer->Commit();
    CommitDisplay();
    return items[index]->GetLevel();
}

/* Deselects the current entry and runs frames until BG2 and the cursor
 * settle. Called by the page-turn handlers (PrevWorld/NextWorld). */
void LevelSelect::SettlePage()
{
    items[index]->SetSelected(0);
    bg2->ClearPicture();
    panel->Park();
    while (bg2->IsZoomingOut() || !panel->HasArrived()) {
        BeginFrame();
        panel->Update();
        bg2->Update();
    }
    gPaletteCache->FreeUnlockedSlots();
}

/* Moves the cursor left, repeating while Left is held; sound 0x48 at the
 * first entry. */
void LevelSelect::CursorLeft()
{
    if (index == 0) {
        gAudioContext->PlaySfx(SFX_MENU_ERROR, 0x100);
        return;
    }
    items[index]->SetSelected(0);
    bg2->ClearPicture();
    while (index != 0) {
        const struct vec2 *pos;

        index--;
        pos = &positions[index];
        panel->Move(pos->x, pos->y - 0x18);
        WaitCursor();
        gInput->Update();
        if (!(gKeys.all & DPAD_LEFT))
            return;
    }
}

/* Moves the cursor right, repeating while Right is held; sound 0x48 at the
 * last entry. */
void LevelSelect::CursorRight()
{
    if (index == lastIndex) {
        gAudioContext->PlaySfx(SFX_MENU_ERROR, 0x100);
        return;
    }
    items[index]->SetSelected(0);
    bg2->ClearPicture();
    while (index < lastIndex) {
        const struct vec2 *pos;

        index++;
        pos = &positions[index];
        panel->Move(pos->x, pos->y - 0x18);
        WaitCursor();
        gInput->Update();
        if (!(gKeys.all & DPAD_RIGHT))
            return;
    }
}
