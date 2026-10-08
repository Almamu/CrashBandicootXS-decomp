#include "level_select.hpp"
#include "audio.hpp"

extern "C" {
#include "match.h"
#include <agb_syscall.h>
#include <libgcc.h>
#include "text.h"
#include "level.h"
#include "math_util.h"
}

/* GitHub issue #26: 0x0801B85C-0x0801CEE0. Three classes (#664,
 * include/level_select.hpp):
 *
 * - CameraLead (gCameraLeadVtable): the 0x80-byte moving sprite
 *   InputCtrl::StateStart (input_ctrl.cpp) spawns for the input controller.
 *   It trails the player at a horizontal offset that eases 2 px per frame
 *   toward a clamped target, and is gCamera's follow target while alive.
 * - LaunchPad (gLaunchPadVtable): a 0x78-byte moving sprite with a factory,
 *   a player-contact check that sends the player EVENT_LAUNCH_PAD, a
 *   constructor and a destructor: the green pad of entity type 0x3D
 *   (sprite bank 28). The event makes the action controller launch Crash
 *   upward in an air spin with a full tornado charge (Y motion 0x13, or
 *   0x14 with A held).
 * - LevelSelect: the paged level-select screen. RunLevelSelect is the whole
 *   modal screen: it builds the menu, runs it (Loop) and returns the chosen
 *   level through `*arg`. Five levels per page (`arg / 5`, `arg % 5`); each
 *   level's fixed record (name text, three time-trial thresholds) is
 *   gLevelTable, its saved record a bitfield word (cleared flag, two more
 *   flags, best time). The rest of its methods are in
 *   level_select_pages.cpp.
 *
 * UNUSED - no `bl`/`.4byte` reference in asm/, expected/ or src/:
 * CameraLead::SetUnk32, SetOffset, GetOffset and LaunchPad's out-of-line
 * constructor (Spawn has it inlined). Matched anyway.
 *
 * old_agbcp (Makefile OLD_AGBCC_OBJS), as its C was old_agbcc. */

/* codegen: gLevelSelectGemPos and gLevelSelectTrialIconPos are const
 * (menus.h), but the constructor reads each one twice, across calls, and
 * the ROM loads it again each time: through the const object gcc keeps the
 * first loads in registers. docs/headers_plan.md */
extern "C" struct vec2 gLevelSelectGemPos_rw asm("gLevelSelectGemPos");
extern "C" struct vec2 gLevelSelectTrialIconPos_rw asm("gLevelSelectTrialIconPos");

/* The motion axes and the probe's hit axes, through an inline: both
 * values come first, and the second address is the first one's plus
 * 0x44, as in the ROM. */
static inline void SetAxes(MovingSprite *s, u8 dir, u8 hit)
{
    s->dir = dir;
    s->hitAxes = hit;
}

/* Sprite::ToggleHidden, inlined. */
static inline void ToggleHiddenNow(Sprite *s)
{
    s->f.b.blink = !s->f.b.blink;
}

/* UNUSED - no caller anywhere in the ROM (checked asm/, data/, src/ and a
 * whole-ROM Thumb-pointer scan). Sets the byte at 0x32, the third byte of
 * Sprite::frame, which nothing else touches on its own, so its meaning
 * (and this method's name) is open. */
void CameraLead::SetUnk32()
{
    ((u8 *)&frame)[2] = 1;
}

/* (Re)initializes the lead: makes it visible, registers it as gCamera's
 * follow target and snaps it 0x1E00 (30 px, Q8) to the player's right with
 * both offsets reset. */
void CameraLead::Reset()
{
    u32 v = f.bytes.flags2 >> 2;
    /* One pin kept (the C had five): the ROM tests `blink` with a 1 of its
     * own in r1 and loads another for ToggleHidden's; unpinned, gcc shares
     * one constant between the two, with every spelling of the test and
     * the toggle tried (u8/s32/bool tests, a switch, `^ 1`, `== 0`). */
    MATCH_HOLD_REG(u32, one, r1) = 1;

    if (!(v & one))
        ToggleHiddenNow(this);
    gCamera->target = (struct camera_target *)this;
    {
        Player *p = gPlayer;
        s32 px = p->x;
        s32 py = p->y;
        s32 off = 0x1E00;

        x = px + off;
        y = py;
        f.b.active = 1;
        targetOffset = off;
        offset = off;
    }
}

/* Slot 3: the moving sprite's update, then eases `offset` toward
 * `targetOffset` by 0x200 per frame and follows the player. */
void CameraLead::Update()
{
    s32 cur, tgt;

    MovingSprite::Update();
    SetAxes(this, 1, 0);
    cur = offset;
    tgt = targetOffset;
    if (cur < tgt) {
        cur += 0x200;
        if (cur > tgt)
            offset = tgt;
        else
            offset = cur;
    } else if (cur > tgt) {
        cur -= 0x200;
        if (cur < tgt)
            offset = tgt;
        else
            offset = cur;
    }
    {
        Player *p = gPlayer;
        s32 px = p->x;
        s32 py = p->y;

        x = px + offset;
        y = py;
    }
    speedX = gPlayer->speedX;
}

/* Slot 10: hands gCamera's follow target back to the player. */
CameraLead::~CameraLead()
{
    gCamera->target = (struct camera_target *)gPlayer;
}

/* The constructor, called from InputCtrl::StateStart (input_ctrl.cpp). */
CameraLead::CameraLead()
{
    Reset();
}

/* UNUSED - no caller anywhere in the ROM (checked asm/, data/, src/ and a
 * whole-ROM Thumb-pointer scan). Sets the target offset, clamped to
 * 0xA00-0x3200. */
void CameraLead::SetOffset(s32 v)
{
    if (v > 0x3200)
        v = 0x3200;
    else if (v <= 0x9FF)
        v = 0xA00;
    targetOffset = v;
}

/* UNUSED - no caller anywhere in the ROM (checked asm/, data/, src/ and a
 * whole-ROM Thumb-pointer scan). Returns the target offset. */
s32 CameraLead::GetOffset()
{
    return targetOffset;
}

/* The factory (the constructor inlined): places the pad at (x, y) pixels
 * with record id `id`, adds it to the collidable list, and starts
 * animation 0 of the set at `**gSpriteBankSet + 0x150`, unmirrored. Called
 * from the level spawners (spawn_objects.c's SpawnLaunchPadEntity). */
LaunchPad *LaunchPad::Spawn(u16 id, u16 x, u16 y, u16 unused)
{
    LaunchPad *obj = new LaunchPad(id, x, y);

    CollidableList()->Add(obj);
    obj->bank = AnimTable(0x150);
    StartAnim(obj, 0);
    obj->mirrorFlags.mirrorX = 0;
    obj->mirrorFlags.mirrorY = 0;
    {
        const struct sprite_anim *anims = obj->bank->anims;
        const struct sprite_anim *rec = &anims[obj->tag];
        u32 slot = gPaletteCache->GetSlot(rec->paletteId);

        obj->palette = slot;
    }
    return obj;
}

/* Slot 14: if the player collides (flags bit 7) and overlaps the pad's
 * hitbox, sends the player EVENT_LAUNCH_PAD. */
void LaunchPad::TouchPlayer()
{
    if (gPlayer->f.flags >> 7) {
        struct aabb box = GetAnimHitbox();

        if (box.w != 0 && gPlayer->TouchesBox(&box))
            gPlayer->HandleEvent(0, EVENT_LAUNCH_PAD, 0);
    }
}

/* Slot 10. */
LaunchPad::~LaunchPad()
{
}

/* Clears flags bit 6, the "vulnerable" bit (the launch pad's own
 * out-of-line copy of Sprite::ClearVulnerable; the constructor calls it). */
void LaunchPad::ClearVulnerable()
{
    f.b.vulnerable = 0;
}

/* UNUSED - no caller anywhere in the ROM (checked asm/, data/, src/ and a
 * whole-ROM Thumb-pointer scan; Spawn inlines it instead). */
LaunchPad::LaunchPad()
{
    ClearVulnerable();
}

/* The level-select screen, modal (called from game_frame.cpp): resets the
 * display, palette, VRAM cursor and both fonts (the same setup as
 * ShowPowerDialog), builds the menu for level `*arg`, runs it, stores the
 * chosen level back through `arg`, tears the menu down and returns its
 * `result` byte. The font steps are inline functions (Font::SetTileBase
 * and IconReserve): the ROM recomputes every field address after each
 * call instead of keeping the offsets in registers, which is what
 * separate inlined expansions give. */
static inline void IconReserve(Font **m)
{
    ObjVramCursor *c = gObjVramCursor;

    c->Reserve((*m)->tileCount << 5);
}

static inline void LoadMenuPalette(PaletteCache *cache)
{
    CpuSet(gLevelSelectPalette, cache->slots[15], 0x10);
}

s32 RunLevelSelect(s32 *arg)
{
    LevelSelect *menu;
    u8 result;
    s32 heaps = 0xC0000000;

    mem_free_bytes(heaps);
    WaitForVBlank();
    *(vu16 *)PLTT = 0;
    *(vu16 *)REG_ADDR_DISPCNT = 0;
    gPaletteCache->FreeUnlockedSlots();
    gPaletteCache->ClaimSlot(0xF);
    LoadMenuPalette(gPaletteCache);
    gObjVramCursor->baseTile = 0;
    gObjVramCursor->Reset();
    gObjVramCursor->Reset();
    gSmallFont->SetTileBase(0);
    IconReserve(&gSmallFont);
    {
        u32 v = gSmallFont->tileCount;

        gLargeFont->SetTileBase(v);
    }
    IconReserve(&gLargeFont);
    gObjVramCursor->Mark();
    gAudioContext->PlaySong(SONG_WARP_ROOM);
    {
        LevelSelect **menuAddr = &gLevelSelect;

        *menuAddr = new LevelSelect(*arg);
        *arg = (*menuAddr)->Loop();
        menu = *menuAddr;
        result = menu->result;
        delete menu;
        *menuAddr = 0;
    }
    gPaletteCache->FreeUnlockedSlots();
    mem_free_bytes(heaps);
    return result;
}

/* The constructor: blend/display shadow registers (alpha blend, evy 16,
 * mode 1, BG0/BG1/OBJ), page/cursor from `arg` (5 levels per page, 20+ is
 * the last page), the save block, both BG layers, the cursor, six level
 * entries and ten sprites, then the cursor position and the BG registers.
 * The six-entry loop needs its own counter and the sprite loop's 0x80 a
 * variable set with the counter, for the ROM's register choice and hoisted
 * constant. */
LevelSelect::LevelSelect(s32 arg)
{
    struct bg_setup bg0cnt;
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
    dispcnt.bits.mode = 1;
    dispcnt.bits.bg0 = 1;
    dispcnt.bits.bg1 = 1;
    dispcnt.bits.obj = 1;
    if (arg <= LEVEL_LAST_NUMBERED) {
        world = __divsi3(arg, LEVELS_PER_WORLD);
        index = __modsi3(arg, LEVELS_PER_WORLD);
    } else {
        world = arg - LEVEL_FIRST_BOSS;
        index = 5;
    }
    nameText = 0;
    save = PackSaveData(gLevelState);
    result = 0;
    bg1 = new LevelSelectPageBg(0, 0x1D);
    InitBgSetup(&bg0cnt, 2, 0x1E, 2, 3);
    LoadGraphicsPackage(&bg0cnt, &gMenuSkyBg);
    scroll = 0;
    panel = new LevelSelectCursor;
    bg2 = new ZoomBg(3, 0x1F);
    {
        s32 j;

        for (j = 0; j < 6; j++)
            items[j] = new LevelSelectEntry;
    }
    LoadEntries();
    PlaceEntries();
    SetEntryBoxes();
    {
        s32 v;

        for (i = 0, v = 0x80; i < 8; i++) {
            s = new UiSprite;
            sprites[i] = s;
            s->SetPriority(1);
            if (i > 1)
                sprites[i]->affine = v;
        }
    }
    SetBankNow(sprites[0], AnimTable(0x234));
    StartAnim(sprites[0], gLevelSelectWorldAnims[world]);
    SetEntityPixelPos(sprites[0], gLevelSelectWorldPos.x, gLevelSelectWorldPos.y);
    SetBankNow(sprites[1], AnimTable(0x234));
    StartAnim(sprites[1], 10);
    SetEntityPixelPos(sprites[1], gLevelSelectCrashIconPos.x, gLevelSelectCrashIconPos.y);
    SetBankNow(sprites[2], AnimTable(0x1BC));
    SetEntityPixelPos(sprites[2], gLevelSelectCrystalPos.x, gLevelSelectCrystalPos.y);
    SetBankNow(sprites[3], AnimTable(0x180));
    StartAnim(sprites[3], 1);
    SetEntityPixelPos(sprites[3], gLevelSelectGemPos_rw.x, gLevelSelectGemPos_rw.y);
    SetBankNow(sprites[4], AnimTable(0x180));
    StartAnim(sprites[4], 1);
    SetEntityPixelPos(sprites[4], gLevelSelectGemPos_rw.x, gLevelSelectGemPos_rw.y);
    SetBankNow(sprites[5], AnimTable(0x18C));
    SetEntityPixelPos(sprites[5], gLevelSelectTrialIconPos_rw.x, gLevelSelectTrialIconPos_rw.y);
    SetBankNow(sprites[6], AnimTable(0x18C));
    SetEntityPixelPos(sprites[6], gLevelSelectTrialIconPos_rw.x, gLevelSelectTrialIconPos_rw.y);
    SetBankNow(sprites[7], AnimTable(0x18C));
    SetEntityPixelPos(sprites[7], gLevelSelectTimePos.x, gLevelSelectTimePos.y);
    s = new UiSprite;
    sprites[8] = s;
    s->SetPriority(1);
    SetBankNow(sprites[8], AnimTable(0x270));
    StartAnim(sprites[8], 1);
    SetEntityPixelPos(sprites[8], gLevelSelectNextWorldArrowPos.x, gLevelSelectNextWorldArrowPos.y);
    s = new UiSprite;
    sprites[9] = s;
    s->SetPriority(1);
    SetBankNow(sprites[9], AnimTable(0x270));
    StartAnim(sprites[9], 0);
    SetEntityPixelPos(sprites[9], gLevelSelectPrevWorldArrowPos.x, gLevelSelectPrevWorldArrowPos.y);
    if (gNewWorldOpened && IsNextWorldOpen()) {
        panel->Park();
    } else {
        const struct vec2 *pos = &positions[index];

        panel->Move(pos->x, pos->y - 0x18);
    }
    *(vu32 *)REG_ADDR_BG0HOFS = 0;
    *(vu32 *)REG_ADDR_BG1HOFS = bg1->GetOffsets();
    *(vu16 *)REG_ADDR_BG0CNT = GetBgSetupControl(&bg0cnt);
    *(vu16 *)REG_ADDR_BG1CNT = GetBgSetupControl(&bg1->bg);
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
    for (i = 0; i < 6; i++)
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
        u32 x = (u32)(0xF0 - gLargeFont->MeasureText((u8 *)nameText)) >> 1;

        gLargeFont->SetPos(x, -panelSlideX + 2);
        gLargeFont->DrawText((u8 *)nameText);
        if (index <= 4)
            DrawRecord();
    }
    bg1->SetOffsets();
    for (i = 0; i <= lastIndex; i++)
        items[i]->Animate(bg1->GetScroll());
    if (bg1->IsSettled()) {
        if (!bg2->IsExiting()) {
            s32 text = GetUiText(0x2F);
            u32 x = (u32)(0xF0 - gSmallFont->MeasureText((u8 *)text)) >> 1;

            gSmallFont->SetPos(x, 0x96);
            gSmallFont->SetPalette(0xF);
            gSmallFont->DrawText((u8 *)text);
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
 * open. */
void LevelSelect::UpdatePageArrows()
{
    sprites[8]->palette = sprites[8]->GetAnimPaletteSlot();
    sprites[9]->palette = sprites[9]->GetAnimPaletteSlot();
    if (world <= 2) {
        /* Two pins kept (the C had 24 and a keep): the ROM has the arrow
         * in r1 and `&tag` in r3; unpinned, old_agbcp (and agbcp) give the
         * arrow r3 or swap `&tag` with the animation table's r2. The copy
         * `t` is the register the ROM's frame store and draw call use. */
        MATCH_HOLD_REG(UiSprite *, s, r1);
        s32 f;

        if (IsNextWorldOpen()) {
            s = sprites[8];
            f = 0;
        } else {
            s = sprites[8];
            f = 1;
        }
        {
            const struct sprite_bank *b = s->bank;
            MATCH_HOLD_REG(u8 *, tag, r3) = &s->tag;
            s32 n = b->anims[*tag].frameCount;
            UiSprite *t = s;

            CLAMP_INDEX(f, n);
            t->frame = f;
            t->DrawWithOffset(0, 0);
        }
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
    {
        struct level_save_h *sv = &save->levels[levelId].h;

        if (sv->cleared)
            DrawTime(sv->time);
    }
}

/* Draws the time readout: just the best time if it beats the tightest
 * threshold (time2), otherwise sprite 7, the next threshold to beat and
 * the best time. */
void LevelSelect::DrawTime(u32 time)
{
    const struct level_info *info;

    sprites[5]->DrawWithOffset(-panelSlideX, trialIconY);
    sprites[6]->DrawWithOffset(-panelSlideX, trialIcon2Y);
    info = &gLevelTable[levelId];
    if (time != 0 && time <= info->times[2]) {
        gLargeFont->SetPos(panelSlideX + gLevelSelectTimePos.x + 10, gLevelSelectTimePos.y - 8);
        gLargeFont->DrawText((u8 *)timeText);
    } else {
        sprites[7]->DrawWithOffset(panelSlideX, 0);
        gLargeFont->SetPalette(sprites[7]->palette);
        gLargeFont->SetPos(panelSlideX + gLevelSelectTimePos.x + 10, gLevelSelectTimePos.y - 8);
        gLargeFont->DrawText((u8 *)recordText);
        gLargeFont->ResetPalette();
        gLargeFont->SetPos(panelSlideX + gLevelSelectTimePos.x + 10, gLevelSelectTimePos.y + 8);
        gLargeFont->DrawText((u8 *)timeText);
    }
}

/* Per-frame draw step: once the BG1 page has settled and the cursor has
 * arrived on a new entry, selects it and loads that level's name and
 * record (LoadRecord); then draws the six entries and steps record sprites
 * 2-7. */
void LevelSelect::Draw()
{
    LevelSelectEntry **it;
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
        if (index <= 4) {
            LoadRecord();
            gPaletteCache->FreeUnlockedSlots();
            ReloadPalette();
        }
        gLargeFont->ResetPalette();
    }
    it = items;
    sp = sprites;
    for (i = 5; i >= 0; i--)
        (*it++)->Draw();
    bg2->Update();
    {
        UiSprite **p = sp + 2;

        for (i = 5; i >= 0; i--)
            (*p++)->AdvanceAnim();
    }
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
    if ((u8)LevelHasGemPathGem(gLevelState, levelId))
        *r = 0;
    if ((u8)LevelHasRedGem(gLevelState, levelId))
        *r = 1;
    if ((u8)LevelHasGreenGem(gLevelState, levelId))
        *r = 2;
    if ((u8)LevelHasBlueGem(gLevelState, levelId))
        *r = 3;
    if ((u8)LevelHasYellowGem(gLevelState, levelId))
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
        if (save->flags & 1)
            gemIconY = 0x1C;
        break;
    case 2:
        if (save->flags & 4)
            gemIconY = 0x1C;
        break;
    case 3:
        if (save->flags & 8)
            gemIconY = 0x1C;
        break;
    case 4:
        if (save->flags & 2)
            gemIconY = 0x1C;
        break;
    case 5:
        break;
    default:
        goto set_rank_icon;
    }
    if (rank != 5) {
    set_rank_icon:
        StartAnim(sprites[4], gLevelSelectRankAnims[rank]);
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
        StartAnim(sprites[5], 0);
        StartAnim(sprites[6], 0);
        StartAnim(sprites[7], 0);
        if (sv->time != 0) {
            if (sv->time <= info->times[2]) {
                trialIcon2Y = 0x1C;
                trialIconY = 0x1C;
                StartAnim(sprites[5], 1);
                StartAnim(sprites[6], 1);
            } else if (sv->time <= info->times[1]) {
                FormatCentiseconds(info->times[2], recordText);
                trialIconY = 0x1C;
                StartAnim(sprites[5], 2);
                StartAnim(sprites[6], 1);
                StartAnim(sprites[7], 1);
            } else if (sv->time <= entry->times[0]) {
                FormatCentiseconds(info->times[1], recordText);
                trialIconY = 0x1C;
                StartAnim(sprites[5], 0);
                StartAnim(sprites[6], 2);
                StartAnim(sprites[7], 2);
            }
        }
    }
}

/* The menu loop: fades in (BLDY), then runs frames until A is pressed on
 * an open entry (Confirm) or Start exits (Exit), dispatching Up/Down page
 * turns (NextWorld/PrevWorld) and Left/Right cursor moves
 * (CursorLeft/CursorRight); returns the selected entry's level. */
s32 LevelSelect::Loop()
{
    const struct level_info *info;

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
    if (gNewWorldOpened && IsNextWorldOpen()) {
        index = 0;
        NextWorld();
    }
    gNewWorldOpened = 0;
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
    UpdateKeys(gInput);
    {
        union key_state k;
        union key_state keys = gKeys;

        if (keys.half.pressed & DPAD_UP)
            NextWorld();
        /* The ROM copies the key word between the 0x80 test's `ands`
         * and its `cmp`, and tests 0x20 on the copy; gcc merges a plain
         * copy, so the (code-free) asm keeps `k` a separate value. */
        else if (({
                     u32 hit = keys.half.pressed & DPAD_DOWN;

                     k.all = keys.all;
                     MATCH_KEEP(k.all);
                     hit;
                 }))
            PrevWorld();
        else {
            if (k.half.pressed & DPAD_LEFT)
                CursorLeft();
            else if (keys.half.pressed & DPAD_RIGHT)
                CursorRight();
        }
    }
    {
        u32 a = gKeys.half.pressed & A_BUTTON;

        if (!a)
            goto check_exit;
    }
    if (!items[index]->IsSelected())
        goto check_exit;
    if (!bg2->IsShown())
        goto check_exit;
    Confirm();
end:
    dispcnt.raw = 0;
    dispcnt.bits.objMap1D = 1;
    WaitForVBlank();
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
        UpdateKeys(gInput);
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
        UpdateKeys(gInput);
        if (!(gKeys.all & DPAD_RIGHT))
            return;
    }
}
