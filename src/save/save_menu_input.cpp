#include "save_menu.hpp"
#include "audio.hpp"
#include "link_session.hpp"

extern "C" {
#include "core.h"
#include "text.h"
#include "link.h"
#include "save.h"
#include "util.h"
#include "system.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
}

/* SaveMenu's run loop, constructor, destructor and input handlers
 * (include/save_menu.hpp, #664's final cleanup), with the save
 * transfer's three accessors and RunSaveMenu (C linkage). The C needed
 * nine register pins in the constructor and destructor and the
 * hand-written slot calls of the slot list's icons; DrawMain was two
 * inline-asm transcriptions of the ROM (docs/matching/archive/
 * issue-5-overlay-ui-sync.md). The C++ needs none of them, and matches
 * under both compilers. */

void SaveTransfer::SetRecord(SaveData *record)
{
    tmpl = record;
    cursor = (u8 *)record;
}

void *SaveTransfer::GetData()
{
    return data;
}

void SaveTransfer::Reset()
{
    remaining = sizeof(data);
    totalReceived = 0;
    cursor = (u8 *)tmpl;
    writePtr = data;
    sendDone = 0;
    receiveDone = 0;
    settleTimer = 0;
}

/* gKeys is a plain u32 elsewhere (e.g.
 * src/save/save_menu_draw.cpp's LinkExchange) but this call site reads
 * only its upper 16 bits (the "newly pressed" half of a held/pressed
 * input pair) - matching the ROM's own `ldrh r1,[r0,#2]` (a runtime
 * +2 byte offset on the reloaded base address) requires a real field
 * access here rather than `(u8*)&gKeys + 2`, which the
 * compiler folds into the linker-relocated constant instead. */

/* The save menu's modal loop (C linkage, for game_frame.cpp): sets up
 * gSaveMenu's `state`/`cursor`/`flags`/`done`/`gameLoaded`, then
 * repeatedly dispatches input (Input) through the draw state machine
 * (Draw), VBlank-waits and commits the frame (CommitFrame) until `done`
 * (set by one of the state handlers below) requests an exit. Returns
 * `gameLoaded` (a game was loaded). */
u8 RunSaveMenu(u32 state, u32 cursor)
{
    SaveMenu **selfAddr = &gSaveMenu;
    SaveMenu *self;

    self = *selfAddr;
    self->state = state;
    self->cursor = cursor;
    self->flags = 0;
    self->done = 0;
    (*selfAddr)->gameLoaded = 0;

    goto dispatch;
    for (;;) {
        u16 keys;

        UpdateKeys(gInput);
        keys = gKeys.half.pressed;
        (*selfAddr)->Input(keys);
    dispatch:
        (*selfAddr)->Draw();
        WaitForVBlank();
        (*selfAddr)->CommitFrame();
        if ((*selfAddr)->done != 0) {
            break;
        }
    }
    return gSaveMenu->gameLoaded;
}

/* OpenSaveMenu's `new SaveMenu` (InitSaveMenu): `new`s and resets both
 * saves, sets up the graphics (InitIcons, LoadBg), plays the warp room
 * song, loads the cartridge save (LoadData), summarizes the current game
 * and the cartridge's slots, and builds the link session (gLinkSession,
 * in IWRAM). Taking the two saves' addresses up front keeps them in r9
 * and r8, as in the ROM. */
SaveMenu::SaveMenu()
{
    SaveData **cartSaveAddr = &cartSave;
    SaveData **linkSaveAddr;
    SaveData *save;

    save = new SaveData;
    save->Reset();
    *cartSaveAddr = save;

    linkSaveAddr = &linkSave;
    save = new SaveData;
    save->Reset();
    *linkSaveAddr = save;

    gPaletteCache->FreeUnlockedSlots();
    InitIcons();
    LoadBg();
    gAudioContext->PlaySong(SONG_WARP_ROOM);
    LoadData();
    SummarizeProgress(&currentStats, PackSaveData(gLevelState));
    RefreshSlotSummaries(*cartSaveAddr);

    {
        LinkSession **sessionAddr = &gLinkSession;

        *sessionAddr = new LinkSession;
    }
    FadeBrightness(0x80, 1, 0);
    gameLoaded = 0;
}

/* CloseSaveMenu's `delete gSaveMenu` (DestroySaveMenu): destroys the link
 * session (`delete gLinkSession`: a null test and DestroyLinkSession with
 * 3), deletes the two saves and the slot list's 15 icons (through their
 * virtual destructors, slot 10). */
SaveMenu::~SaveMenu()
{
    s32 i;

    delete gLinkSession;

    delete linkSave;
    delete cartSave;

    for (i = 0; i <= 4; i++) {
        delete rowObjA[i];
        delete rowObjB[i];
        delete rowObjC[i];
    }
}

/* Per-frame input dispatch: updates the slot list's 15 icons (their
 * virtual Update, slot 3), dispatches `keys` to the current `state`'s
 * handler, and advances `flags` (a wrapping 0-0xff frame counter) and
 * `frame`. */
void SaveMenu::Input(u32 keys)
{
    s32 i;

    for (i = 0; i <= 4; i++) {
        rowObjA[i]->Update();
        rowObjB[i]->Update();
        rowObjC[i]->Update();
    }

    if ((u32)state <= 0xa) {
        switch (state) {
        case 0:
            MainInput(keys);
            break;
        case 1:
            LoadInput(keys, cartSave);
            break;
        case 2:
            LoadInput(keys, linkSave);
            break;
        case 3:
            LinkInput();
            break;
        case 4:
            MessageInput(keys);
            break;
        case 5:
            SaveInput(keys);
            break;
        case 6:
            DeleteInput(keys);
            break;
        case 9:
            OverwriteInput(keys);
            break;
        case 8:
            break;
        case 7:
            ConfirmDeleteInput(keys);
            break;
        case 10:
            break;
        }
    }

    flags = (flags + 1) & 0xff;
    frame += 1;
}

/* State 0's input handler (`keys`: the newly pressed keys): B or START
 * requests an exit; A advances through this state's own little
 * sub-menu (`cursor` 0-4, mirroring the DrawSaveMenu states each
 * selects); up/down move the `cursor` cursor with wraparound. */
void SaveMenu::MainInput(u32 keys)
{
    if (keys & (B_BUTTON | START_BUTTON)) {
        gAudioContext->PlaySfx(SFX_MENU_BACK, 0x100);
        done = 1;
        return;
    }
    if (keys & A_BUTTON) {
        gAudioContext->PlaySfx(SFX_MENU_SELECT, 0x100);
        switch (cursor) {
        case 0:
            state = 1;
            cursor = 0;
            RefreshSlotSummaries(cartSave);
            break;
        case 1:
            state = 3;
            cursor = 0;
            messageLine1 = GetUiText(0x2b);
            messageLine2 = GetUiText(0x2d);
            BeginLinkTransfer();
            break;
        case 2:
            state = 5;
            cursor = 0;
            RefreshSlotSummaries(cartSave);
            break;
        case 3:
            state = 6;
            cursor = 0;
            RefreshSlotSummaries(cartSave);
            break;
        case 4:
            done = 1;
            break;
        }
        return;
    }
    if (keys & DPAD_UP) {
        gAudioContext->PlaySfx(SFX_MENU_MOVE, 0x100);
        cursor -= 1;
        if (cursor < 0) {
            cursor = 4;
        }
    } else if (keys & DPAD_DOWN) {
        gAudioContext->PlaySfx(SFX_MENU_MOVE, 0x100);
        cursor += 1;
        if (cursor > 4) {
            cursor = 0;
        }
    }
}

/* D-pad-only row-cursor mover shared by several of this screen's other
 * states (called directly by several handlers below when their own
 * confirm/cancel keys aren't pressed): up/down step through the rows,
 * left/right swap columns (`cursor ^ 2`) except on "cancel" (4). */
void SaveMenu::MoveCursor(u32 keys)
{
    if (keys & DPAD_UP) {
        gAudioContext->PlaySfx(SFX_MENU_MOVE, 0x100);
        if ((u32)cursor <= 4) {
            switch (cursor) {
            case 0:
            case 2:
                cursor = 4;
                break;
            case 1:
            case 3:
                cursor = cursor - 1;
                break;
            case 4:
                cursor = 1;
                break;
            }
        }
        return;
    }
    if (keys & DPAD_DOWN) {
        gAudioContext->PlaySfx(SFX_MENU_MOVE, 0x100);
        if ((u32)cursor <= 4) {
            switch (cursor) {
            case 0:
            case 2:
                cursor = cursor + 1;
                break;
            case 1:
            case 3:
                cursor = 4;
                break;
            case 4:
                cursor = 0;
                break;
            }
        }
        return;
    }
    if (keys & DPAD_SIDEWAYS) {
        if (cursor != 4) {
            gAudioContext->PlaySfx(SFX_MENU_MOVE, 0x100);
            cursor ^= 2;
        }
    }
}

/* States 1/2's input handler ("load game" from `handle` = cartSave or
 * linkSave): confirm (A or START) on "cancel" (cursor 4) goes back to state 0; on an
 * empty slot (SaveData::IsSlotEmpty) it only plays an error sound; otherwise it
 * reads the slot (SaveData::ReadSlot), unpacks it into gLevelState with its
 * level and volumes, summarizes it into `currentStats` and exits with
 * `gameLoaded` set. Cancel (B) goes back to state 0; otherwise falls
 * through to the shared cursor mover. */
void SaveMenu::LoadInput(u32 keys, SaveData *handle)
{
    struct save_slot buf;

    if (keys & A_BUTTON) {
        goto confirm;
    }
    if (keys & START_BUTTON) {
    confirm:
        if (cursor == 4) {
            gAudioContext->PlaySfx(SFX_MENU_SELECT, 0x100);
            state = 0;
            cursor = 0;
            return;
        }
        if (handle->IsSlotEmpty(cursor)) {
            gAudioContext->PlaySfx(SFX_MENU_ERROR, 0x100);
            return;
        }
        gAudioContext->PlaySfx(SFX_MENU_SELECT, 0x100);
        handle->ReadSlot(cursor, &buf);
        UnpackSaveData(gLevelState, &buf.progress);
        SetCurrentLevel(gLevelState, buf.level);
        gAudioContext->SetSfxVolume(buf.sfxVolume);
        gAudioContext->SetMusicVolume(buf.musicVolume);
        SummarizeProgress(&currentStats, PackSaveData(gLevelState));
        gameLoaded = 1;
        done = 1;
        return;
    }
    if (keys & B_BUTTON) {
        gAudioContext->PlaySfx(SFX_MENU_BACK, 0x100);
        state = 0;
        cursor = 0;
        return;
    }
    MoveCursor(keys);
}

/* State 3's input handler: polls the SIO-handshake spinner
 * (LinkExchangeSaveData, parked). Timeout/cancel -> settle back to state 0;
 * error/checksum-mismatch -> a "connection failed" message (state 4);
 * otherwise, if both sides agree on the checksummed record
 * (SaveData::GetGameId's version nibble), accept it (state 2); if they don't,
 * inspect the remote's nibble to merge either flag 2 or flag 4 into our
 * own record and show a matching "conflict" message. */
void SaveMenu::LinkInput()
{
    s32 result = LinkExchange();

    EndLinkTransfer();

    if (result == 3) {
        state = 0;
        cursor = 1;
        gAudioContext->PlaySfx(SFX_MENU_BACK, 0x100);
        return;
    }

    if (result == 2 || !(u8)linkSave->CheckChecksum()) {
        state = 4;
        messageLine1 = GetUiText(0x2c);
        messageLine2 = GetUiText(0x2e);
        return;
    }

    if (cartSave->GetGameId() == linkSave->GetGameId()) {
        state = 2;
        cursor = 0;
        RefreshSlotSummaries(linkSave);
        return;
    }

    switch (linkSave->GetGameId()) {
    case 2:
        cartSave->SetFlags(2);
        cartSave->Store();
        state = 4;
        messageLine1 = (u32)gCrash2LinkTextPtr;
        break;
    case 3:
        cartSave->SetFlags(4);
        cartSave->Store();
        state = 4;
        messageLine1 = (u32)gCrash3LinkTextPtr;
        break;
    default:
        state = 0;
        cursor = 1;
        return;
    }
    messageLine2 = GetUiText(0x2e);
}

/* Shared "commit or refresh row `rowIndex`" step used by states 5-9
 * below: pulls the row's stats/name/icon scratch data, feeds it through
 * `cartSave`'s pending-edit slot, and either finalises the edit
 * (SaveData::EraseSlot, when it wasn't already selected) or just refreshes the
 * row's aggregate stats. */
void SaveMenu::SaveToSlot(s32 rowIndex)
{
    struct save_slot buf[2]; /* [0] the slot's old contents, [1] the new */
    SaveData **handleAddr = &cartSave;
    SaveData **handleAddr2;
    u32 wasSelected;
    struct level_state **c0Addr;
    AudioContext **bcAddr;

    if (!(*handleAddr)->IsSlotEmpty(rowIndex)) {
        (*handleAddr)->ReadSlot(rowIndex, buf);
        wasSelected = 0;
    } else {
        wasSelected = 1;
    }

    c0Addr = &gLevelState;
    {
        /* The ROM evaluates PackSaveData()'s result before computing
         * `&buf[1]` (the ROM's own callee-arg setup order for
         * MemCopy32, not the other way around) - a plain nested call
         * expression here lets this compiler compute the pointer
         * argument first instead. */
        struct game_progress *result = PackSaveData(*c0Addr);
        MemCopy32(&buf[1].progress, result, sizeof(struct game_progress));
    }
    buf[1].level = (u8)GetCurrentLevel(*c0Addr);

    bcAddr = &gAudioContext;
    buf[1].sfxVolume = (u16)(*bcAddr)->GetSfxVolume();
    buf[1].musicVolume = (u16)(*bcAddr)->GetMusicVolume();

    /* The ROM recomputes `cartSave`'s address a second time here
     * (a fresh `adds r4, r7, #0` / `adds r4, #0x8c` pair) rather than
     * reusing the register the first computation above left live -
     * mirror that with a second local instead of reusing `handleAddr`,
     * matching the technique noted in docs/matching.md for this class
     * of gap. */
    handleAddr2 = &cartSave;
    (*handleAddr2)->WriteSlot(rowIndex, &buf[1]);
    if ((*handleAddr2)->Store()) {
        if (wasSelected) {
            (*handleAddr2)->EraseSlot(rowIndex);
        } else {
            (*handleAddr2)->WriteSlot(rowIndex, &buf[0]);
        }
    } else {
        SummarizeProgress(&rowStats[rowIndex], PackSaveData(*c0Addr));
    }
}

/* State 7's input handler: confirm (A or START) commits row `pendingSlot`
 * (SaveToSlot) and returns to state 0
 * if it was already the "current" row (`cursor==0`), else re-enters
 * state 5 to reselect; cancel (B) re-enters state 5 too; up/down toggle
 * `cursor` between 0/1. */
void SaveMenu::OverwriteInput(u32 keys)
{
    if (keys & A_BUTTON) {
        goto confirm;
    }
    if (keys & START_BUTTON) {
    confirm:
        if (cursor == 0) {
            SaveToSlot(pendingSlot);
            state = 0;
            cursor = 4;
        } else {
            state = 5;
            cursor = pendingSlot;
            gAudioContext->PlaySfx(SFX_MENU_SELECT, 0x100);
        }
        return;
    }
    if (keys & B_BUTTON) {
        state = 5;
        cursor = pendingSlot;
        gAudioContext->PlaySfx(SFX_MENU_BACK, 0x100);
        return;
    }
    if (keys & DPAD_UP) {
        if (cursor == 1) {
            cursor = 0;
            gAudioContext->PlaySfx(SFX_MENU_MOVE, 0x100);
        }
        return;
    }
    if (keys & DPAD_DOWN) {
        if (cursor == 0) {
            cursor = 1;
            gAudioContext->PlaySfx(SFX_MENU_MOVE, 0x100);
        }
    }
}

/* State 5's input handler: confirm (A or START) either resets to state
 * 0 (maxed out) or, if row `cursor` isn't already selected
 * (SaveData::IsSlotEmpty), enters state 9 to edit it, else commits it directly
 * (SaveGameToSlot) and returns to state 0; cancel (B) resets to state
 * 0; otherwise falls through to the shared d-pad cursor mover. */
void SaveMenu::SaveInput(u32 keys)
{
    if (keys & A_BUTTON) {
        goto confirm;
    }
    if (keys & START_BUTTON) {
    confirm:
        if (cursor == 4) {
            gAudioContext->PlaySfx(SFX_MENU_SELECT, 0x100);
            state = 0;
            cursor = 2;
            return;
        }
        gAudioContext->PlaySfx(SFX_MENU_SELECT, 0x100);
        if (!cartSave->IsSlotEmpty(cursor)) {
            state = 9;
            pendingSlot = cursor;
            cursor = 0;
        } else {
            SaveToSlot(cursor);
            state = 0;
            cursor = 4;
        }
        return;
    }
    if (keys & B_BUTTON) {
        gAudioContext->PlaySfx(SFX_MENU_BACK, 0x100);
        state = 0;
        cursor = 2;
        return;
    }
    MoveCursor(keys);
}

/* State 6's input handler - same shape as SaveMenuSaveInput above, a
 * different row-selection sub-menu (state 7 on confirm-when-unselected,
 * cursor target value 3 rather than 2). */
void SaveMenu::DeleteInput(u32 keys)
{
    if (keys & A_BUTTON) {
        goto confirm;
    }
    if (keys & START_BUTTON) {
    confirm:
        if (cursor == 4) {
            gAudioContext->PlaySfx(SFX_MENU_SELECT, 0x100);
            state = 0;
            cursor = 3;
            return;
        }
        if (cartSave->IsSlotEmpty(cursor)) {
            gAudioContext->PlaySfx(SFX_MENU_ERROR, 0x100);
            return;
        }
        gAudioContext->PlaySfx(SFX_MENU_SELECT, 0x100);
        state = 7;
        pendingSlot = cursor;
        cursor = 0;
        return;
    }
    if (keys & B_BUTTON) {
        gAudioContext->PlaySfx(SFX_MENU_BACK, 0x100);
        state = 0;
        cursor = 3;
        return;
    }
    MoveCursor(keys);
}

/* State 9's input handler: confirm (A or START) commits row `pendingSlot`
 * unconditionally (SaveData::ReadSlot+SaveData::EraseSlot+optional SaveData::WriteSlot) then
 * settles at state 0; cancel (B) re-enters state 6; up/down toggle
 * `cursor` between 0/1. */
void SaveMenu::ConfirmDeleteInput(u32 keys)
{
    u8 buf[0x70];

    if (keys & A_BUTTON) {
        goto confirm;
    }
    if (keys & START_BUTTON) {
    confirm:
        if (cursor == 0) {
            s32 rowIndex = pendingSlot;
            SaveData *handle = cartSave;

            handle->ReadSlot(rowIndex, buf);
            handle = cartSave;
            handle->EraseSlot(rowIndex);
            handle = cartSave;
            if (handle->Store()) {
                handle = cartSave;
                handle->WriteSlot(rowIndex, buf);
            }
            state = 0;
            cursor = 4;
        } else {
            state = 6;
            cursor = pendingSlot;
            gAudioContext->PlaySfx(SFX_MENU_SELECT, 0x100);
        }
        return;
    }
    if (keys & B_BUTTON) {
        state = 6;
        cursor = pendingSlot;
        gAudioContext->PlaySfx(SFX_MENU_BACK, 0x100);
        return;
    }
    if (keys & DPAD_UP) {
        if (cursor == 1) {
            cursor = 0;
            gAudioContext->PlaySfx(SFX_MENU_MOVE, 0x100);
        }
        return;
    }
    if (keys & DPAD_DOWN) {
        if (cursor == 0) {
            cursor = 1;
            gAudioContext->PlaySfx(SFX_MENU_MOVE, 0x100);
        }
    }
}

/* State 0's draw: the five main options (gSaveMenuOptions) centred in
 * gSmallFont from Y=0x64, the one under the cursor in the blink palette,
 * then the current game's summary (DrawSlotStats with summary 0,
 * unhighlighted). The C was two inline-asm transcriptions of the ROM; the
 * C++ only needs the option's label loaded inside the MeasureText call
 * (after the vtable lookup, as in the ROM) and the stack-passed `flag`
 * set before the loop, which leaves the 0 to be rematerialized after the
 * argument slot's address, as the ROM does. */
void SaveMenu::DrawMain()
{
    s32 y = 0x64;
    s32 i;
    struct byte_arg flag;

    flag.v = 0;

    for (i = 0; i <= 4; i++) {
        s32 label;
        s32 w;

        if (i == cursor)
            gSmallFont->SetPalette(GetBlinkPalette());
        else
            gSmallFont->SetPalette(0);
        w = gSmallFont->MeasureText((u8 *)GetUiText(label = gSaveMenuOptions[i]));
        gSmallFont->SetPos((0xf0 - w) >> 1, y);
        gSmallFont->DrawText((u8 *)GetUiText(label));
        y += 0xa;
    }

    DrawSlotStats(0x5a, 0x21, 0, flag);
}
