#include "game/ui/save_menu.hpp"
#include "engine/audio/audio.hpp"
#include "game/level/level_state.hpp"
#include "engine/link/link_session.hpp"
#include "engine/sys/key_input.hpp"
#include "engine/sys/frame_limiter.hpp"
#include "engine/gfx/fade.hpp"

extern "C" {
#include "core.h"
#include "engine/text/text.h"
#include "engine/link/link.h"
#include "engine/save/save.h"
#include "engine/util/util.h"
#include "engine/sys/system.h"
#include "engine/gfx/gfx.h"
#include "game/level/level.h"
#include "globals.h"
}

using Engine::Audio::AudioContext;
using Engine::Gfx::BrightnessFade;
using Engine::Link::LinkSession;
using Engine::Save::SaveData;
using Engine::Sys::FrameLimiter;
using Game::Level::GameProgress;
using Game::Level::LevelState;
using Game::Ui::SaveMenu;

/* SaveMenu's run loop, constructor, destructor and input handlers
 * (engine/include/game/ui/save_menu.hpp, #664's final cleanup), with the static Run
 * (#915); the save transfer's three accessors that started the file
 * are at the end of save_transfer_state.cpp since #767. The C needed
 * nine register pins in the constructor and destructor and the
 * hand-written slot calls of the slot list's icons; DrawMain was two
 * inline-asm transcriptions of the ROM (docs/matching/archive/
 * issue-5-overlay-ui-sync.md). The C++ needs none of them, and matches
 * under both compilers. */

/* gKeys is a plain u32 elsewhere (e.g.
 * engine/src/game/ui/save_menu_draw.cpp's LinkExchange) but this call site reads
 * only its upper 16 bits (the "newly pressed" half of a held/pressed
 * input pair) - matching the ROM's own `ldrh r1,[r0,#2]` (a runtime
 * +2 byte offset on the reloaded base address) requires a real field
 * access here rather than `(u8*)&gKeys + 2`, which the
 * compiler folds into the linker-relocated constant instead. */

/* The save menu's modal loop (for game_frame.cpp): sets up
 * `instance`'s `state`/`cursor`/`flags`/`done`/`gameLoaded`, then
 * repeatedly dispatches input (Input) through the draw state machine
 * (Draw), VBlank-waits and commits the frame (CommitFrame) until `done`
 * (set by one of the state handlers below) requests an exit. Returns
 * `gameLoaded` (a game was loaded). */
u8 SaveMenu::Run(u32 state, u32 cursor)
{
    SaveMenu **selfAddr = &instance;
    SaveMenu *self;

    self = *selfAddr;
    self->state = state;
    self->cursor = cursor;
    self->flags = 0;
    self->done = 0;
    (*selfAddr)->gameLoaded = 0;

    goto dispatch;
    for (;;) {
        /* `keys` is needed: inlined, the instructions are reordered */
        u16 keys;

        gInput->Update();
        keys = gKeys.half.pressed;
        (*selfAddr)->Input(keys);
    dispatch:
        (*selfAddr)->Draw();
        FrameLimiter::WaitForVBlank();
        (*selfAddr)->CommitFrame();
        if ((*selfAddr)->done != 0) {
            break;
        }
    }
    return instance->gameLoaded;
}

/* Open's `new SaveMenu` (InitSaveMenu): `new`s and resets both
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
    gAudioContext->PlaySong(SCREEN_SONG_SAVE_MENU);
    LoadData();
    SummarizeProgress(&currentStats, gLevelState->PackSaveData());
    RefreshSlotSummaries(*cartSaveAddr);

    LinkSession **sessionAddr = &gLinkSession;

    *sessionAddr = new LinkSession;
    BrightnessFade::Start(0x80, 1, 0);
    gameLoaded = 0;
}

/* Close's `delete instance` (DestroySaveMenu): destroys the link
 * session (`delete gLinkSession`: a null test and DestroyLinkSession with
 * 3), deletes the two saves and the slot list's 15 icons (through their
 * virtual destructors, slot 10). */
SaveMenu::~SaveMenu()
{
    s32 i;

    delete gLinkSession;

    delete linkSave;
    delete cartSave;

    for (i = 0; i < (s32)ARRAY_COUNT(rowObjA); i++) {
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

    for (i = 0; i < (s32)ARRAY_COUNT(rowObjA); i++) {
        rowObjA[i]->Update();
        rowObjB[i]->Update();
        rowObjC[i]->Update();
    }

    if (state <= SAVE_MENU_STATE_UNK10) {
        switch (state) {
        case SAVE_MENU_STATE_MAIN:
            MainInput(keys);
            break;
        case SAVE_MENU_STATE_LOAD:
            LoadInput(keys, cartSave);
            break;
        case SAVE_MENU_STATE_LOAD_LINK:
            LoadInput(keys, linkSave);
            break;
        case SAVE_MENU_STATE_LINK_TRANSFER:
            LinkInput();
            break;
        case SAVE_MENU_STATE_MESSAGE:
            MessageInput(keys);
            break;
        case SAVE_MENU_STATE_SAVE:
            SaveInput(keys);
            break;
        case SAVE_MENU_STATE_DELETE:
            DeleteInput(keys);
            break;
        case SAVE_MENU_STATE_CONFIRM_OVERWRITE:
            OverwriteInput(keys);
            break;
        case SAVE_MENU_STATE_CONFIRM_DELETE:
            ConfirmDeleteInput(keys);
            break;
        case SAVE_MENU_STATE_UNK8:
        case SAVE_MENU_STATE_UNK10:
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
            state = SAVE_MENU_STATE_LOAD;
            cursor = 0;
            RefreshSlotSummaries(cartSave);
            break;
        case 1:
            state = SAVE_MENU_STATE_LINK_TRANSFER;
            cursor = 0;
            messageLine1 = GetUiText(0x2b);
            messageLine2 = GetUiText(0x2d);
            BeginLinkTransfer();
            break;
        case 2:
            state = SAVE_MENU_STATE_SAVE;
            cursor = 0;
            RefreshSlotSummaries(cartSave);
            break;
        case 3:
            state = SAVE_MENU_STATE_DELETE;
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
    if ((keys & DPAD_SIDEWAYS) && cursor != 4) {
        gAudioContext->PlaySfx(SFX_MENU_MOVE, 0x100);
        cursor ^= 2;
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
            state = SAVE_MENU_STATE_MAIN;
            cursor = 0;
            return;
        }
        if (handle->IsSlotEmpty(cursor)) {
            gAudioContext->PlaySfx(SFX_MENU_ERROR, 0x100);
            return;
        }
        gAudioContext->PlaySfx(SFX_MENU_SELECT, 0x100);
        handle->ReadSlot(cursor, &buf);
        gLevelState->UnpackSaveData(&buf.progress);
        gLevelState->SetCurrentLevel(buf.level);
        gAudioContext->SetSfxVolume(buf.sfxVolume);
        gAudioContext->SetMusicVolume(buf.musicVolume);
        SummarizeProgress(&currentStats, gLevelState->PackSaveData());
        gameLoaded = 1;
        done = 1;
        return;
    }
    if (keys & B_BUTTON) {
        gAudioContext->PlaySfx(SFX_MENU_BACK, 0x100);
        state = SAVE_MENU_STATE_MAIN;
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
        state = SAVE_MENU_STATE_MAIN;
        cursor = 1;
        gAudioContext->PlaySfx(SFX_MENU_BACK, 0x100);
        return;
    }

    if (result == 2 || !linkSave->CheckChecksum()) {
        state = SAVE_MENU_STATE_MESSAGE;
        messageLine1 = GetUiText(0x2c);
        messageLine2 = GetUiText(0x2e);
        return;
    }

    if (cartSave->GetGameId() == linkSave->GetGameId()) {
        state = SAVE_MENU_STATE_LOAD_LINK;
        cursor = 0;
        RefreshSlotSummaries(linkSave);
        return;
    }

    switch (linkSave->GetGameId()) {
    case 2:
        cartSave->SetFlags(2);
        cartSave->Store();
        state = SAVE_MENU_STATE_MESSAGE;
        messageLine1 = gCrash2LinkTextPtr;
        break;
    case 3:
        cartSave->SetFlags(4);
        cartSave->Store();
        state = SAVE_MENU_STATE_MESSAGE;
        messageLine1 = gCrash3LinkTextPtr;
        break;
    default:
        state = SAVE_MENU_STATE_MAIN;
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
    LevelState **c0Addr;
    AudioContext **bcAddr;

    if (!(*handleAddr)->IsSlotEmpty(rowIndex)) {
        (*handleAddr)->ReadSlot(rowIndex, buf);
        wasSelected = 0;
    } else {
        wasSelected = 1;
    }

    c0Addr = &gLevelState;

    /* The ROM evaluates PackSaveData()'s result before computing
     * `&buf[1]` (the ROM's own callee-arg setup order for
     * MemCopy32, not the other way around) - a plain nested call
     * expression here lets this compiler compute the pointer
     * argument first instead. */
    GameProgress *result = (*c0Addr)->PackSaveData();
    MemCopy32(&buf[1].progress, result, sizeof(GameProgress));
    buf[1].level = (u8)(*c0Addr)->GetCurrentLevel();

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
        SummarizeProgress(&rowStats[rowIndex], (*c0Addr)->PackSaveData());
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
            state = SAVE_MENU_STATE_MAIN;
            cursor = 4;
        } else {
            state = SAVE_MENU_STATE_SAVE;
            cursor = pendingSlot;
            gAudioContext->PlaySfx(SFX_MENU_SELECT, 0x100);
        }
        return;
    }
    if (keys & B_BUTTON) {
        state = SAVE_MENU_STATE_SAVE;
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
    if ((keys & DPAD_DOWN) && cursor == 0) {
        cursor = 1;
        gAudioContext->PlaySfx(SFX_MENU_MOVE, 0x100);
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
            state = SAVE_MENU_STATE_MAIN;
            cursor = 2;
            return;
        }
        gAudioContext->PlaySfx(SFX_MENU_SELECT, 0x100);
        if (!cartSave->IsSlotEmpty(cursor)) {
            state = SAVE_MENU_STATE_CONFIRM_OVERWRITE;
            pendingSlot = cursor;
            cursor = 0;
        } else {
            SaveToSlot(cursor);
            state = SAVE_MENU_STATE_MAIN;
            cursor = 4;
        }
        return;
    }
    if (keys & B_BUTTON) {
        gAudioContext->PlaySfx(SFX_MENU_BACK, 0x100);
        state = SAVE_MENU_STATE_MAIN;
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
            state = SAVE_MENU_STATE_MAIN;
            cursor = 3;
            return;
        }
        if (cartSave->IsSlotEmpty(cursor)) {
            gAudioContext->PlaySfx(SFX_MENU_ERROR, 0x100);
            return;
        }
        gAudioContext->PlaySfx(SFX_MENU_SELECT, 0x100);
        state = SAVE_MENU_STATE_CONFIRM_DELETE;
        pendingSlot = cursor;
        cursor = 0;
        return;
    }
    if (keys & B_BUTTON) {
        gAudioContext->PlaySfx(SFX_MENU_BACK, 0x100);
        state = SAVE_MENU_STATE_MAIN;
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
    u8 buf[sizeof(struct save_slot)];

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
            state = SAVE_MENU_STATE_MAIN;
            cursor = 4;
        } else {
            state = SAVE_MENU_STATE_DELETE;
            cursor = pendingSlot;
            gAudioContext->PlaySfx(SFX_MENU_SELECT, 0x100);
        }
        return;
    }
    if (keys & B_BUTTON) {
        state = SAVE_MENU_STATE_DELETE;
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
    if ((keys & DPAD_DOWN) && cursor == 0) {
        cursor = 1;
        gAudioContext->PlaySfx(SFX_MENU_MOVE, 0x100);
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
    screen_y_t y = DISPLAY_ART_Y + 0x64;
    s32 i;
    struct byte_arg flag;

    flag.v = 0;

    for (i = 0; i < (s32)ARRAY_COUNT(gSaveMenuOptions); i++) {
        s32 label;
        screen_w_t w;

        if (i == cursor)
            gSmallFont->SetPalette(GetBlinkPalette());
        else
            gSmallFont->SetPalette(0);
        w = gSmallFont->MeasureText(GetUiText(label = gSaveMenuOptions[i]));
        gSmallFont->SetPos((DISPLAY_WIDTH - w) >> 1, y);
        gSmallFont->DrawText(GetUiText(label));
        y += 0xa;
    }

    DrawSlotStats(DISPLAY_ART_X + 0x5a, DISPLAY_ART_Y + 0x21, 0, flag);
}
