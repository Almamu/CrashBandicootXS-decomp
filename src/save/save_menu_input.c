#include "core.h"
#include "match.h"
#include "box_part.h"
#include "bitmap_font.h"
#include "text.h"
#include "link.h"
#include "save.h"
#include "util.h"
#include "system.h"
#include "audio.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"

void SetSaveTransferRecord(struct settings_sync_pump *self, struct save_data *tmpl)
{
    self->tmpl = tmpl;
    self->cursor = (u8 *)tmpl;
}

void *GetSaveTransferData(struct settings_sync_pump *self)
{
    return self->data;
}

void ResetSaveTransfer(struct settings_sync_pump *self)
{
    self->remaining = sizeof(self->data);
    self->totalReceived = 0;
    self->cursor = (u8 *)self->tmpl;
    self->writePtr = self->data;
    self->sendDone = 0;
    self->receiveDone = 0;
    self->settleTimer = 0;
}

/* gKeys is a plain u32 elsewhere (e.g.
 * src/save/save_menu_draw.c's LinkExchangeSaveData) but this call site reads
 * only its upper 16 bits (the "newly pressed" half of a held/pressed
 * input pair) - matching the ROM's own `ldrh r1,[r0,#2]` (a runtime
 * +2 byte offset on the reloaded base address) requires a real field
 * access here rather than `(u8*)&gKeys + 2`, which the
 * compiler folds into the linker-relocated constant instead. */

/* The "connecting..." spinner dialog's blocking modal loop: sets up
 * `gSaveMenu`'s `state`/`cursor`/`flags`/`done`/`gameLoaded`,
 * then repeatedly dispatches input (SaveMenuInput) through
 * DrawSaveMenu's state machine, VBlank-waits, and restores display
 * registers (CommitSaveMenuFrame) until `done` (set by one of the state
 * handlers below) requests an exit. Returns `gameLoaded` (a game was
 * loaded). */
u8 RunSaveMenu(u32 state, u32 cursor)
{
    struct save_menu **selfAddr = (struct save_menu **)&gSaveMenu;
    struct save_menu *self;

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
        SaveMenuInput(*selfAddr, keys);
    dispatch:
        DrawSaveMenu(*selfAddr);
        WaitForVBlank();
        CommitSaveMenuFrame(*selfAddr);
        if ((*selfAddr)->done != 0) {
            break;
        }
    }
    return ((struct save_menu *)gSaveMenu)->gameLoaded;
}

/* The composite pause/options screen's (and the spinner dialog's, via
 * InitSaveMenu above) `cartSave`/`linkSave` constructor: allocates and
 * initialises both save_data instances (ResetSaveData), does
 * the screen's tile/BG/list setup (InitSaveMenuIcons/LoadSaveMenuBg/
 * LoadSaveMenuData, still raw), fills `currentStats` and the first
 * `rowStats` entry, then allocates and stashes the global SIO session
 * object (`gLinkSession`, a `struct link_session` - see
 * SendSaveTransferChunk/ReceiveSaveTransferChunk, src/save/save_transfer.c) and kicks off
 * a VBlank IRQ request. */
struct save_menu *InitSaveMenu(struct save_menu *arg0)
{
    MATCH_HOLD_REG(struct save_menu *, self, r5) = arg0;
    MATCH_HOLD_REG(void **, cartSaveAddr, r9) = &self->cartSave;
    MATCH_HOLD_REG(void **, linkSaveAddr, r8);
    MATCH_HOLD_REG(s32, size, r6) = 0x200;
    MATCH_HOLD_REG(void *, obj, r4);

    obj = OperatorNew(size);
    ResetSaveData(obj);
    *cartSaveAddr = obj;

    linkSaveAddr = &self->linkSave;
    obj = OperatorNew(size);
    ResetSaveData(obj);
    *linkSaveAddr = obj;

    FreeUnlockedPaletteSlots(gPaletteCache);
    InitSaveMenuIcons(self);
    LoadSaveMenuBg(self);
    PlaySong(gAudioContext, SONG_WARP_ROOM);
    LoadSaveMenuData(self);
    SummarizeProgress(self, &self->currentStats, PackSaveData(gLevelState));
    RefreshSaveSlotSummaries(self, *cartSaveAddr);

    {
        MATCH_HOLD_REG(struct link_session **, sessionAddr, r4) = &gLinkSession;
        *sessionAddr = InitLinkSession(IwramAlloc(0x408));
    }
    FadeBrightness(0x80, 1, 0);
    self->gameLoaded = 0;
    return self;
}

extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);

/* Tears down the composite screen's (or spinner dialog's) cartSave/
 * linkSave pair, cancels the global SIO session object if one's still
 * active, erases each of the five settings-row icon widgets
 * (rowObjA/B/C, drawing a "blank" glyph via _call_via_r2's arg1=3), and
 * - only when `flags` bit 0 is set - destroys `self` itself. */
void DestroySaveMenu(struct save_menu *self, u32 flags)
{
    MATCH_HOLD_REG(void **, c, r6);
    MATCH_HOLD_REG(void **, b, r5);
    MATCH_HOLD_REG(void **, a, r4);
    MATCH_HOLD_REG(s32, n, r8);

    if (gLinkSession != NULL) {
        DestroyLinkSession(gLinkSession, 3);
    }

    OperatorDelete(self->linkSave);
    OperatorDelete(self->cartSave);

    c = (void **)self->rowObjC;
    b = (void **)self->rowObjB;
    a = (void **)self->rowObjA;

    n = 4;
    do {
        void *obj;
        MATCH_HOLD_REG(struct part_method *, p, r1);

        /* rowObjA/B/C[i]'s icon descriptor at +0x18 holds a {s16 offset,
         * u8 pad[2], void *fn} record at +0x50 - the same (offset, fn)
         * shape bitmap_font's own record slots use elsewhere in this
         * screen, just inside a different, still-uncharacterized
         * container type. */
        obj = *a;
        if (obj != NULL) {
            p = PART_METHOD((struct box_part *)obj, 0x50);
            _call_via_r2((u8 *)obj + p->thisOffset, (void *)3, p->fn);
        }
        obj = *b;
        if (obj != NULL) {
            p = PART_METHOD((struct box_part *)obj, 0x50);
            _call_via_r2((u8 *)obj + p->thisOffset, (void *)3, p->fn);
        }
        obj = *c;
        if (obj != NULL) {
            p = PART_METHOD((struct box_part *)obj, 0x50);
            _call_via_r2((u8 *)obj + p->thisOffset, (void *)3, p->fn);
        }
        c++;
        b++;
        a++;
        n--;
    } while (n >= 0);

    if (flags & 1) {
        OperatorDelete(self);
    }
}

extern void _call_via_r1(void *arg0, void *fn);

/* Per-frame input dispatch for the composite screen (or, via
 * RunSaveMenu above, the spinner dialog sharing the same struct shape):
 * redraws all 15 settings-row icon widgets (rowObjA/B/C[0..4] - same
 * still-uncharacterized descriptor shape as DestroySaveMenu above), then
 * dispatches `keys` to whichever per-`state` handler is active, and
 * finally advances `flags` (as a wrapping 0-0xff per-frame counter) and
 * `frame` (as a plain per-frame tick). */
void SaveMenuInput(struct save_menu *self, u32 keys)
{
    s32 i;

    for (i = 0; i <= 4; i++) {
        struct part_method *p;
        s16 off;

        p = PART_METHOD((struct box_part *)self->rowObjA[i], 0x18);
        off = p->thisOffset;
        _call_via_r1((u8 *)self->rowObjA[i] + off, p->fn);

        p = PART_METHOD((struct box_part *)self->rowObjB[i], 0x18);
        off = p->thisOffset;
        _call_via_r1((u8 *)self->rowObjB[i] + off, p->fn);

        p = PART_METHOD((struct box_part *)self->rowObjC[i], 0x18);
        off = p->thisOffset;
        _call_via_r1((u8 *)self->rowObjC[i] + off, p->fn);
    }

    if ((u32)self->state <= 0xa) {
        switch (self->state) {
        case 0:
            SaveMenuMainInput(self, keys);
            break;
        case 1:
            SaveMenuLoadInput(self, keys, self->cartSave);
            break;
        case 2:
            SaveMenuLoadInput(self, keys, self->linkSave);
            break;
        case 3:
            SaveMenuLinkInput(self);
            break;
        case 4:
            SaveMenuMessageInput(self, keys);
            break;
        case 5:
            SaveMenuSaveInput(self, keys);
            break;
        case 6:
            SaveMenuDeleteInput(self, keys);
            break;
        case 9:
            SaveMenuOverwriteInput(self, keys);
            break;
        case 8:
            break;
        case 7:
            SaveMenuConfirmDeleteInput(self, keys);
            break;
        case 10:
            break;
        }
    }

    self->flags = (self->flags + 1) & 0xff;
    self->frame += 1;
}

/* State 0's input handler: cancel/confirm-combo (bits 1/3) requests an
 * exit; confirm (bit 0) advances through this state's own little
 * sub-menu (`cursor` 0-4, mirroring the DrawSaveMenu states each
 * selects); L/R (bits 6/7) move the `cursor` cursor with wraparound. */
void SaveMenuMainInput(struct save_menu *self, u32 flags)
{
    if (flags & 0xa) {
        PlaySfx(gAudioContext, SFX_MENU_BACK, 0x100);
        self->done = 1;
        return;
    }
    if (flags & 1) {
        PlaySfx(gAudioContext, SFX_MENU_SELECT, 0x100);
        switch (self->cursor) {
        case 0:
            self->state = 1;
            self->cursor = 0;
            RefreshSaveSlotSummaries(self, self->cartSave);
            break;
        case 1:
            self->state = 3;
            self->cursor = 0;
            self->messageLine1 = GetUiText(0x2b);
            self->messageLine2 = GetUiText(0x2d);
            BeginLinkSaveTransfer(self);
            break;
        case 2:
            self->state = 5;
            self->cursor = 0;
            RefreshSaveSlotSummaries(self, self->cartSave);
            break;
        case 3:
            self->state = 6;
            self->cursor = 0;
            RefreshSaveSlotSummaries(self, self->cartSave);
            break;
        case 4:
            self->done = 1;
            break;
        }
        return;
    }
    if (flags & 0x40) {
        PlaySfx(gAudioContext, SFX_MENU_MOVE, 0x100);
        self->cursor -= 1;
        if (self->cursor < 0) {
            self->cursor = 4;
        }
    } else if (flags & 0x80) {
        PlaySfx(gAudioContext, SFX_MENU_MOVE, 0x100);
        self->cursor += 1;
        if (self->cursor > 4) {
            self->cursor = 0;
        }
    }
}

/* L/R-only row-cursor mover shared by several of this screen's other
 * states (called directly by several handlers below when their own
 * confirm/cancel bits are clear). */
void SaveMenuMoveCursor(struct save_menu *self, u32 flags)
{
    if (flags & 0x40) {
        PlaySfx(gAudioContext, SFX_MENU_MOVE, 0x100);
        if ((u32)self->cursor <= 4) {
            switch (self->cursor) {
            case 0:
            case 2:
                self->cursor = 4;
                break;
            case 1:
            case 3:
                self->cursor = self->cursor - 1;
                break;
            case 4:
                self->cursor = 1;
                break;
            }
        }
        return;
    }
    if (flags & 0x80) {
        PlaySfx(gAudioContext, SFX_MENU_MOVE, 0x100);
        if ((u32)self->cursor <= 4) {
            switch (self->cursor) {
            case 0:
            case 2:
                self->cursor = self->cursor + 1;
                break;
            case 1:
            case 3:
                self->cursor = 4;
                break;
            case 4:
                self->cursor = 0;
                break;
            }
        }
        return;
    }
    if (flags & 0x30) {
        if (self->cursor != 4) {
            PlaySfx(gAudioContext, SFX_MENU_MOVE, 0x100);
            self->cursor ^= 2;
        }
    }
}

/* States 1/2's input handler ("load game" from `handle` = cartSave or
 * linkSave): confirm on "cancel" (cursor 4) goes back to state 0; on an
 * empty slot (IsSaveSlotEmpty) it only plays an error sound; otherwise it
 * reads the slot (ReadSaveSlot), unpacks it into gLevelState with its
 * level and volumes, summarizes it into `currentStats` and exits with
 * `gameLoaded` set. Cancel (bit 1) goes back to state 0; otherwise falls
 * through to the shared cursor mover. */
void SaveMenuLoadInput(struct save_menu *self, u32 flags, void *handle)
{
    struct save_slot buf;

    if (flags & 1) {
        goto confirm;
    }
    if (flags & 8) {
    confirm:
        if (self->cursor == 4) {
            PlaySfx(gAudioContext, SFX_MENU_SELECT, 0x100);
            self->state = 0;
            self->cursor = 0;
            return;
        }
        if (IsSaveSlotEmpty(handle, self->cursor)) {
            PlaySfx(gAudioContext, SFX_MENU_ERROR, 0x100);
            return;
        }
        PlaySfx(gAudioContext, SFX_MENU_SELECT, 0x100);
        ReadSaveSlot(handle, self->cursor, &buf);
        UnpackSaveData(gLevelState, &buf);
        SetCurrentLevel(gLevelState, buf.level);
        SetSfxVolume(gAudioContext, buf.sfxVolume);
        SetMusicVolume(gAudioContext, buf.musicVolume);
        SummarizeProgress(self, &self->currentStats, PackSaveData(gLevelState));
        self->gameLoaded = 1;
        self->done = 1;
        return;
    }
    if (flags & 2) {
        PlaySfx(gAudioContext, SFX_MENU_BACK, 0x100);
        self->state = 0;
        self->cursor = 0;
        return;
    }
    SaveMenuMoveCursor(self, flags);
}

/* State 3's input handler: polls the SIO-handshake spinner
 * (LinkExchangeSaveData, parked). Timeout/cancel -> settle back to state 0;
 * error/checksum-mismatch -> a "connection failed" message (state 4);
 * otherwise, if both sides agree on the checksummed record
 * (GetSaveGameId's version nibble), accept it (state 2); if they don't,
 * inspect the remote's nibble to merge either flag 2 or flag 4 into our
 * own record and show a matching "conflict" message. */
void SaveMenuLinkInput(struct save_menu *self)
{
    s32 state = LinkExchangeSaveData(self);

    EndLinkSaveTransfer(self);

    if (state == 3) {
        self->state = 0;
        self->cursor = 1;
        PlaySfx(gAudioContext, SFX_MENU_BACK, 0x100);
        return;
    }

    if (state == 2 || !(u8)CheckSaveChecksum(self->linkSave)) {
        self->state = 4;
        self->messageLine1 = GetUiText(0x2c);
        self->messageLine2 = GetUiText(0x2e);
        return;
    }

    if (GetSaveGameId(self->cartSave) == GetSaveGameId(self->linkSave)) {
        self->state = 2;
        self->cursor = 0;
        RefreshSaveSlotSummaries(self, self->linkSave);
        return;
    }

    switch (GetSaveGameId(self->linkSave)) {
    case 2:
        SetSaveFlags(self->cartSave, 2);
        StoreSaveData(self->cartSave);
        self->state = 4;
        self->messageLine1 = (u32)gCrash2LinkTextPtr;
        break;
    case 3:
        SetSaveFlags(self->cartSave, 4);
        StoreSaveData(self->cartSave);
        self->state = 4;
        self->messageLine1 = (u32)gCrash3LinkTextPtr;
        break;
    default:
        self->state = 0;
        self->cursor = 1;
        return;
    }
    self->messageLine2 = GetUiText(0x2e);
}

/* Shared "commit or refresh row `rowIndex`" step used by states 5-9
 * below: pulls the row's stats/name/icon scratch data, feeds it through
 * `cartSave`'s pending-edit slot, and either finalises the edit
 * (EraseSaveSlot, when it wasn't already selected) or just refreshes the
 * row's aggregate stats. */
void SaveGameToSlot(struct save_menu *self, s32 rowIndex)
{
    struct save_slot buf[2]; /* [0] the slot's old contents, [1] the new */
    void **handleAddr = &self->cartSave;
    void **handleAddr2;
    u32 wasSelected;
    struct level_state **c0Addr;
    struct AudioContext **bcAddr;

    if (!IsSaveSlotEmpty(*handleAddr, rowIndex)) {
        ReadSaveSlot(*handleAddr, rowIndex, buf);
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
        void *result = PackSaveData(*c0Addr);
        MemCopy32(&buf[1], result, 0x68);
    }
    buf[1].level = (u8)GetCurrentLevel(*c0Addr);

    bcAddr = &gAudioContext;
    buf[1].sfxVolume = (u16)GetSfxVolume(*bcAddr);
    buf[1].musicVolume = (u16)GetMusicVolume(*bcAddr);

    /* The ROM recomputes `self->cartSave`'s address a second time here
     * (a fresh `adds r4, r7, #0` / `adds r4, #0x8c` pair) rather than
     * reusing the register the first computation above left live -
     * mirror that with a second local instead of reusing `handleAddr`,
     * matching the technique noted in docs/matching.md for this class
     * of gap. */
    handleAddr2 = &self->cartSave;
    WriteSaveSlot(*handleAddr2, rowIndex, &buf[1]);
    if (StoreSaveData(*handleAddr2)) {
        if (wasSelected) {
            EraseSaveSlot(*handleAddr2, rowIndex);
        } else {
            WriteSaveSlot(*handleAddr2, rowIndex, &buf[0]);
        }
    } else {
        SummarizeProgress(self, &self->rowStats[rowIndex], PackSaveData(*c0Addr));
    }
}

/* State 7's input handler: confirm/cancel-combo commits row `pendingSlot`
 * (SaveGameToSlot, src/save/save_menu_input.c) and returns to state 0
 * if it was already the "current" row (`cursor==0`), else re-enters
 * state 5 to reselect; cancel (bit 1) re-enters state 5 too; L/R toggle
 * `cursor` between 0/1. */
void SaveMenuOverwriteInput(struct save_menu *self, u32 flags)
{
    if (flags & 1) {
        goto confirm;
    }
    if (flags & 8) {
    confirm:
        if (self->cursor == 0) {
            SaveGameToSlot(self, self->pendingSlot);
            self->state = 0;
            self->cursor = 4;
        } else {
            self->state = 5;
            self->cursor = self->pendingSlot;
            PlaySfx(gAudioContext, SFX_MENU_SELECT, 0x100);
        }
        return;
    }
    if (flags & 2) {
        self->state = 5;
        self->cursor = self->pendingSlot;
        PlaySfx(gAudioContext, SFX_MENU_BACK, 0x100);
        return;
    }
    if (flags & 0x40) {
        if (self->cursor == 1) {
            self->cursor = 0;
            PlaySfx(gAudioContext, SFX_MENU_MOVE, 0x100);
        }
        return;
    }
    if (flags & 0x80) {
        if (self->cursor == 0) {
            self->cursor = 1;
            PlaySfx(gAudioContext, SFX_MENU_MOVE, 0x100);
        }
    }
}

/* State 5's input handler: confirm/cancel-combo either resets to state
 * 0 (maxed out) or, if row `cursor` isn't already selected
 * (IsSaveSlotEmpty), enters state 9 to edit it, else commits it directly
 * (SaveGameToSlot) and returns to state 0; cancel (bit 1) resets to state
 * 0; otherwise falls through to the shared L/R cursor mover. */
void SaveMenuSaveInput(struct save_menu *self, u32 flags)
{
    if (flags & 1) {
        goto confirm;
    }
    if (flags & 8) {
    confirm:
        if (self->cursor == 4) {
            PlaySfx(gAudioContext, SFX_MENU_SELECT, 0x100);
            self->state = 0;
            self->cursor = 2;
            return;
        }
        PlaySfx(gAudioContext, SFX_MENU_SELECT, 0x100);
        if (!IsSaveSlotEmpty(self->cartSave, self->cursor)) {
            self->state = 9;
            self->pendingSlot = self->cursor;
            self->cursor = 0;
        } else {
            SaveGameToSlot(self, self->cursor);
            self->state = 0;
            self->cursor = 4;
        }
        return;
    }
    if (flags & 2) {
        PlaySfx(gAudioContext, SFX_MENU_BACK, 0x100);
        self->state = 0;
        self->cursor = 2;
        return;
    }
    SaveMenuMoveCursor(self, flags);
}

/* State 6's input handler - same shape as SaveMenuSaveInput above, a
 * different row-selection sub-menu (state 7 on confirm-when-unselected,
 * cursor target value 3 rather than 2). */
void SaveMenuDeleteInput(struct save_menu *self, u32 flags)
{
    if (flags & 1) {
        goto confirm;
    }
    if (flags & 8) {
    confirm:
        if (self->cursor == 4) {
            PlaySfx(gAudioContext, SFX_MENU_SELECT, 0x100);
            self->state = 0;
            self->cursor = 3;
            return;
        }
        if (IsSaveSlotEmpty(self->cartSave, self->cursor)) {
            PlaySfx(gAudioContext, SFX_MENU_ERROR, 0x100);
            return;
        }
        PlaySfx(gAudioContext, SFX_MENU_SELECT, 0x100);
        self->state = 7;
        self->pendingSlot = self->cursor;
        self->cursor = 0;
        return;
    }
    if (flags & 2) {
        PlaySfx(gAudioContext, SFX_MENU_BACK, 0x100);
        self->state = 0;
        self->cursor = 3;
        return;
    }
    SaveMenuMoveCursor(self, flags);
}

/* State 9's input handler: confirm/cancel-combo commits row `pendingSlot`
 * unconditionally (ReadSaveSlot+EraseSaveSlot+optional WriteSaveSlot) then
 * settles at state 0; cancel (bit 1) re-enters state 6; L/R toggle
 * `cursor` between 0/1. */
void SaveMenuConfirmDeleteInput(struct save_menu *self, u32 flags)
{
    u8 buf[0x70];

    if (flags & 1) {
        goto confirm;
    }
    if (flags & 8) {
    confirm:
        if (self->cursor == 0) {
            s32 rowIndex = self->pendingSlot;
            void *handle = self->cartSave;

            ReadSaveSlot(handle, rowIndex, buf);
            handle = self->cartSave;
            EraseSaveSlot(handle, rowIndex);
            handle = self->cartSave;
            if (StoreSaveData(handle)) {
                handle = self->cartSave;
                WriteSaveSlot(handle, rowIndex, buf);
            }
            self->state = 0;
            self->cursor = 4;
        } else {
            self->state = 6;
            self->cursor = self->pendingSlot;
            PlaySfx(gAudioContext, SFX_MENU_SELECT, 0x100);
        }
        return;
    }
    if (flags & 2) {
        self->state = 6;
        self->cursor = self->pendingSlot;
        PlaySfx(gAudioContext, SFX_MENU_BACK, 0x100);
        return;
    }
    if (flags & 0x40) {
        if (self->cursor == 1) {
            self->cursor = 0;
            PlaySfx(gAudioContext, SFX_MENU_MOVE, 0x100);
        }
        return;
    }
    if (flags & 0x80) {
        if (self->cursor == 0) {
            self->cursor = 1;
            PlaySfx(gAudioContext, SFX_MENU_MOVE, 0x100);
        }
    }
}

/* Draws the 5-entry state-select sub-menu label list (SaveMenuMainInput's
 * `cursor` states, gSaveMenuOptions's label table) into
 * gSmallFont, highlighting whichever row matches `cursor`,
 * then draws a final fixed label via the still-raw DrawSaveSlotStats. Same
 * measure-then-draw icon shape as DrawSaveMenuTitle
 * (src/save/save_menu_draw.c, parked) - see that function's doc
 * comment for the class of gcc register-allocation quirk this may hit
 * too. */
/* This compiler's automatic register allocation cannot reproduce the
 * ROM's exact shape here even with the individual-variable register
 * pins that work elsewhere in this chunk (InitSaveMenu/DestroySaveMenu/
 * SaveMenuInput): a loop-invariant constant (`mgr->record`'s 0x130 field
 * offset, and separately `&gSaveMenuOptions[0]`) repeatedly gets
 * hoisted out of the loop into whichever register looks free at that
 * program point, which lands on r7 - the pinned loop counter itself,
 * silently corrupting it - because nothing textually mentions `i` in
 * between, and even where a per-value register pin sidesteps that, the
 * ROM's specific choice of scratch register per access still differs
 * (e.g. reusing r1's already-computed 0x114 via a plain `+0x1c` for the
 * second `mgr->record` fetch, instead of resynthesizing 0x98<<1). The
 * per-iteration body below is therefore spelled out as one inline-asm
 * transcription of the ROM's own instruction sequence, operating on the
 * same pinned C locals (`self`/`mgrAddr`/`y`/`i`/`label`/`mgr`) the rest
 * of this file's register-pinned functions use - see
 * docs/matching/archive/issue-5-overlay-ui-sync.md for the write-up. */
void DrawSaveMenuMain(struct save_menu *self)
{
    MATCH_HOLD_REG(struct save_menu *, selfReg, r9) = self;
    MATCH_HOLD_REG(s32, y, sl) = 0x64;
    s32 i = 0;
    /* `mgrAddr`'s init is deliberately kept last (right before the loop
     * body) - the ROM computes it right there too, not up front with
     * `self`. It's also set via its own tiny asm statement, rather than
     * a plain C initializer, because this compiler's own literal-pool
     * placement for a compiler-managed `ldr =symbol` always defers to
     * the function's tail (it never looks inside a raw asm statement's
     * text for an earlier flush point, even one right there in the very
     * next statement) - the matching `.pool` directive placed inside
     * the loop body's first asm block below, right after its own
     * unconditional `b`, is what actually lands this literal in the
     * ROM's exact early slot. */
    MATCH_HOLD_REG(struct bitmap_font **, mgrAddr, r8);
    MATCH_HOLD_REG(s32, label, r6);
    MATCH_HOLD_REG(struct bitmap_font *, mgr, r4);

    // clang-format off
    asm volatile(
        "ldr r1, =gSmallFont\n"
        "mov %0, r1\n"
        : "=r" (mgrAddr)
        :
        : "r1"
    );
    // clang-format on

    /* `i`/`y` are kept as genuinely-used C locals (the `for` loop's own
     * compare/increment) rather than folded into the asm text below -
     * a register pin that's *only* ever touched from inside an asm
     * operand/clobber list, with no real (non-asm) RTL use, doesn't get
     * the usual callee-save push/pop from this compiler (it silently
     * drops the save of that hardware register instead of erroring),
     * so the loop control has to stay in plain C to keep r7 (and sl)
     * properly preserved. This build of agbcc also predates GCC's
     * `%[name]` symbolic asm-operand syntax, so operands are referenced
     * positionally below. */
    for (; i <= 4; i++) {
        /* %0 = mgr, %1 = i, %2 = self, %3 = mgrAddr */
        // clang-format off
        asm volatile(
            "mov r2, %2\n"
            "ldr r0, [r2, #0x10]\n"
            "cmp %1, r0\n"
            "bne 1f\n"
            "mov r0, %3\n"
            "ldr %0, [r0]\n"
            "mov r0, %2\n"
            "bl GetSaveMenuBlinkPalette\n"
            "add r1, r0, #0\n"
            "lsl r1, r1, #0x18\n"
            "lsr r1, r1, #0x18\n"
            "add r0, %0, #0\n"
            "bl FontSetPalette\n"
            "b 2f\n"
            ".pool\n"
            "1:\n"
            "mov r1, %3\n"
            "ldr r0, [r1]\n"
            "movs r1, #0\n"
            "bl FontSetPalette\n"
            "2:\n"
            : "=r" (mgr)
            : "r" (i), "r" (selfReg), "r" (mgrAddr)
            : "r0", "r1", "r2", "r3", "r12", "lr", "cc", "memory"
        );
        // clang-format on

        /* %0 = mgr, %1 = label, %2 = y, %3 = i, %4 = mgrAddr */
        // clang-format off
        asm volatile(
            "mov r2, %4\n"
            "ldr %0, [r2]\n"
            "movs r1, #0x98\n"
            "lsl r1, r1, #1\n"
            "add r0, %0, r1\n"
            "ldr r0, [r0]\n"
            "add r5, r0, #0\n"
            "add r5, #0x10\n"
            "movs r2, #0x10\n"
            "ldrsh r0, [r0, r2]\n"
            "add %0, %0, r0\n"
            "ldr r1, =gSaveMenuOptions\n"
            "lsl r0, %3, #2\n"
            "add r0, r0, r1\n"
            "ldr %1, [r0]\n"
            "add r0, %1, #0\n"
            "bl GetUiText\n"
            "add r1, r0, #0\n"
            "ldr r2, [r5, #4]\n"
            "add r0, %0, #0\n"
            "bl _call_via_r2\n"
            "movs r1, #0xf0\n"
            "sub r1, r1, r0\n"
            "asr r1, r1, #1\n"
            "mov r0, %4\n"
            "ldr %0, [r0]\n"
            "movs r2, #0x88\n"
            "lsl r2, r2, #1\n"
            "add r0, %0, r2\n"
            "str r1, [r0]\n"
            "movs r1, #0x8a\n"
            "lsl r1, r1, #1\n"
            "add r0, %0, r1\n"
            "mov r2, %2\n"
            "str r2, [r0]\n"
            "add r1, #0x1c\n"
            "add r0, %0, r1\n"
            "ldr r0, [r0]\n"
            "add r5, r0, #0\n"
            "add r5, #0x20\n"
            "movs r2, #0x20\n"
            "ldrsh r0, [r0, r2]\n"
            "add %0, %0, r0\n"
            "add r0, %1, #0\n"
            "bl GetUiText\n"
            "add r1, r0, #0\n"
            "ldr r2, [r5, #4]\n"
            "add r0, %0, #0\n"
            "bl _call_via_r2\n"
            : "+r" (mgr), "=r" (label)
            : "r" (y), "r" (i), "r" (mgrAddr)
            : "r0", "r1", "r2", "r3", "r5", "r12", "lr", "cc", "memory"
        );
        // clang-format on

        y += 0xa;
    }

    /* The ROM stores this call's stack-passed 5th argument (`flag`, a
     * plain u8 0) through a computed `mov r1, sp` pointer and a `strb`
     * - Thumb1 has no sp-relative byte-store encoding, so the byte has
     * to go through a register base - whereas this compiler always
     * emits a direct word-sized `str r0, [sp]` for a stack argument
     * regardless of the parameter's declared width. Spelled out in asm
     * to match; `flag`'s address is still taken (as an unused input
     * operand) purely to make this compiler reserve the same 4-byte
     * stack slot the ROM's own `sub sp, #4`/`add sp, #4` frame does. */
    {
        u8 flag;
        // clang-format off
        asm volatile(
            "mov r1, sp\n"
            "movs r0, #0\n"
            "strb r0, [r1]\n"
            "mov r0, %0\n"
            "movs r1, #0x5a\n"
            "movs r2, #0x21\n"
            "movs r3, #0\n"
            "bl DrawSaveSlotStats\n"
            :
            : "r" (selfReg), "r" (&flag)
            : "r0", "r1", "r2", "r3", "r12", "lr", "cc", "memory"
        );
        // clang-format on
    }
}
