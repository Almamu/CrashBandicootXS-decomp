#include "core.h"
#include "settings_sync.h"
#include "save_menu.h"
#include "box_part.h"
#include "bitmap_font.h"

extern void ReadSaveSlot(void *handle, s32 rowIndex, void *buf);
extern void WriteSaveSlot(void *handle, s32 rowIndex, void *buf);
extern void EraseSaveSlot(void *handle, s32 rowIndex);

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

extern void *gInput;
/* gKeys is a plain u32 elsewhere (e.g.
 * src/save/save_menu_draw.c's LinkExchangeSaveData) but this call site reads
 * only its upper 16 bits (the "newly pressed" half of a held/pressed
 * input pair) - matching the ROM's own `ldrh r1,[r0,#2]` (a runtime
 * +2 byte offset on the reloaded base address) requires a real field
 * access here rather than `(u8*)&gKeys + 2`, which the
 * compiler folds into the linker-relocated constant instead. */
struct held_pressed_pair {
    u16 held;
    u16 pressed;
};
extern struct held_pressed_pair gKeys;
extern void UpdateKeys(void *arg0);
extern void DrawSaveMenu(struct save_menu *self);
extern void WaitForVBlank(void);
extern void CommitSaveMenuFrame(struct save_menu *self);
extern void *gSaveMenu;
extern void SaveMenuInput(struct save_menu *self, u32 keys);

/* The "connecting..." spinner dialog's blocking modal loop: sets up
 * `gSaveMenu`'s `state`/`field_10`/`flags`/`field_8`/`field_20`,
 * then repeatedly dispatches input (SaveMenuInput) through
 * DrawSaveMenu's state machine, VBlank-waits, and restores display
 * registers (CommitSaveMenuFrame) until `field_8` (set by one of the state
 * handlers below) requests an exit. Returns `field_20`, the handlers'
 * "result ready" flag. */
u8 RunSaveMenu(u32 state, u32 field10)
{
    struct save_menu **selfAddr = (struct save_menu **)&gSaveMenu;
    struct save_menu *self;

    self = *selfAddr;
    self->state = state;
    self->field_10 = field10;
    self->flags = 0;
    self->field_8 = 0;
    (*selfAddr)->field_20 = 0;

    goto dispatch;
    for (;;) {
        u16 keys;

        UpdateKeys(gInput);
        keys = gKeys.pressed;
        SaveMenuInput(*selfAddr, keys);
    dispatch:
        DrawSaveMenu(*selfAddr);
        WaitForVBlank();
        CommitSaveMenuFrame(*selfAddr);
        if ((*selfAddr)->field_8 != 0) {
            break;
        }
    }
    return ((struct save_menu *)gSaveMenu)->field_20;
}

extern void *OperatorNew(s32 size);
extern void *gAudioContext;
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);
extern u8 CheckSaveChecksum(void *arg0);
extern struct palette_cache *gPaletteCache;
extern void FreeUnlockedPaletteSlots(struct palette_cache *self);
extern void InitSaveMenuIcons(struct save_menu *self);
extern void LoadSaveMenuBg(struct save_menu *self);
extern void PlaySong(void *arg0, s32 arg1);
extern void LoadSaveMenuData(struct save_menu *self);
extern void *gLevelState;
extern void *PackSaveData(void *arg0);
extern void SummarizeProgress(void *self, struct settings_row_stats *dest, void *src);
extern void RefreshSaveSlotSummaries(struct save_menu *self, void *handle);
extern void *IwramAlloc(s32 size);
extern void *InitLinkSession(void *arg0);
extern void FadeBrightness(u8 flags, s32 frameDelay, u8 sync);
extern void ResetSaveData(struct save_data *self);

/* The composite pause/options screen's (and the spinner dialog's, via
 * InitSaveMenu above) `field_8c`/`field_90` constructor: allocates and
 * initialises both save_data instances (ResetSaveData), does
 * the screen's tile/BG/list setup (InitSaveMenuIcons/LoadSaveMenuBg/
 * LoadSaveMenuData, still raw), fills `currentStats` and the first
 * `rowStats` entry, then allocates and stashes the global SIO session
 * object (`gLinkSession`, still uncharacterized - see
 * SendSaveTransferChunk/ReceiveSaveTransferChunk, src/save/save_data.c) and kicks off
 * a VBlank IRQ request. */
struct save_menu *InitSaveMenu(struct save_menu *arg0)
{
    register struct save_menu *self asm("r5") = arg0;
    register void **field8cAddr asm("r9") = &self->field_8c;
    register void **field90Addr asm("r8");
    register s32 size asm("r6") = 0x200;
    register void *obj asm("r4");
    extern void *gLinkSession;

    obj = OperatorNew(size);
    ResetSaveData(obj);
    *field8cAddr = obj;

    field90Addr = &self->field_90;
    obj = OperatorNew(size);
    ResetSaveData(obj);
    *field90Addr = obj;

    FreeUnlockedPaletteSlots(gPaletteCache);
    InitSaveMenuIcons(self);
    LoadSaveMenuBg(self);
    PlaySong(gAudioContext, 0x10);
    LoadSaveMenuData(self);
    SummarizeProgress(self, &self->currentStats, PackSaveData(gLevelState));
    RefreshSaveSlotSummaries(self, *field8cAddr);

    {
        register void **sessionAddr asm("r4") = &gLinkSession;
        *sessionAddr = InitLinkSession(IwramAlloc(0x408));
    }
    FadeBrightness(0x80, 1, 0);
    self->field_20 = 0;
    return self;
}

extern void DestroyLinkSession(void *arg0, s32 arg1);
extern void OperatorDelete(void *arg0);
extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);

/* Tears down the composite screen's (or spinner dialog's) field_8c/
 * field_90 pair, cancels the global SIO session object if one's still
 * active, erases each of the five settings-row icon widgets
 * (rowObjA/B/C, drawing a "blank" glyph via _call_via_r2's arg1=3), and
 * - only when `flags` bit 0 is set - destroys `self` itself. */
void DestroySaveMenu(struct save_menu *self, u32 flags)
{
    register void **c asm("r6");
    register void **b asm("r5");
    register void **a asm("r4");
    register s32 n asm("r8");
    extern void *gLinkSession;

    if (gLinkSession != NULL) {
        DestroyLinkSession(gLinkSession, 3);
    }

    OperatorDelete(*(void **)((u8 *)self + 0x90));
    OperatorDelete(*(void **)((u8 *)self + 0x8c));

    c = (void **)self->rowObjC;
    b = (void **)self->rowObjB;
    a = (void **)self->rowObjA;

    n = 4;
    do {
        void *obj;
        register struct part_method *p asm("r1");

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
extern void SaveMenuMainInput(struct save_menu *self, u32 keys);
extern void SaveMenuLoadInput(struct save_menu *self, u32 keys, void *handle);
extern void SaveMenuLinkInput(struct save_menu *self);
extern void SaveMenuMessageInput(struct save_menu *self, u32 keys);
extern void SaveMenuSaveInput(struct save_menu *self, u32 keys);
extern void SaveMenuDeleteInput(struct save_menu *self, u32 keys);
extern void SaveMenuOverwriteInput(struct save_menu *self, u32 keys);
extern void SaveMenuConfirmDeleteInput(struct save_menu *self, u32 keys);

/* Per-frame input dispatch for the composite screen (or, via
 * RunSaveMenu above, the spinner dialog sharing the same struct shape):
 * redraws all 15 settings-row icon widgets (rowObjA/B/C[0..4] - same
 * still-uncharacterized descriptor shape as DestroySaveMenu above), then
 * dispatches `keys` to whichever per-`state` handler is active, and
 * finally advances `flags` (as a wrapping 0-0xff per-frame counter) and
 * `field_0` (as a plain per-frame tick). */
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
            SaveMenuLoadInput(self, keys, self->field_8c);
            break;
        case 2:
            SaveMenuLoadInput(self, keys, self->field_90);
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
    self->field_0 += 1;
}

extern s32 GetUiText(s32 arg0);
extern void BeginLinkSaveTransfer(struct save_menu *self);

/* State 0's input handler: cancel/confirm-combo (bits 1/3) requests an
 * exit; confirm (bit 0) advances through this state's own little
 * sub-menu (`field_10` 0-4, mirroring the DrawSaveMenu states each
 * selects); L/R (bits 6/7) move the `field_10` cursor with wraparound. */
void SaveMenuMainInput(struct save_menu *self, u32 flags)
{
    if (flags & 0xa) {
        PlaySfx(gAudioContext, 0x47, 0x100);
        self->field_8 = 1;
        return;
    }
    if (flags & 1) {
        PlaySfx(gAudioContext, 0x49, 0x100);
        switch (self->field_10) {
        case 0:
            self->state = 1;
            self->field_10 = 0;
            RefreshSaveSlotSummaries(self, self->field_8c);
            break;
        case 1:
            self->state = 3;
            self->field_10 = 0;
            self->field_14 = GetUiText(0x2b);
            self->field_18 = GetUiText(0x2d);
            BeginLinkSaveTransfer(self);
            break;
        case 2:
            self->state = 5;
            self->field_10 = 0;
            RefreshSaveSlotSummaries(self, self->field_8c);
            break;
        case 3:
            self->state = 6;
            self->field_10 = 0;
            RefreshSaveSlotSummaries(self, self->field_8c);
            break;
        case 4:
            self->field_8 = 1;
            break;
        }
        return;
    }
    if (flags & 0x40) {
        PlaySfx(gAudioContext, 0x46, 0x100);
        self->field_10 -= 1;
        if (self->field_10 < 0) {
            self->field_10 = 4;
        }
    } else if (flags & 0x80) {
        PlaySfx(gAudioContext, 0x46, 0x100);
        self->field_10 += 1;
        if (self->field_10 > 4) {
            self->field_10 = 0;
        }
    }
}

/* L/R-only row-cursor mover shared by several of this screen's other
 * states (called directly by several handlers below when their own
 * confirm/cancel bits are clear). */
void SaveMenuMoveCursor(struct save_menu *self, u32 flags)
{
    if (flags & 0x40) {
        PlaySfx(gAudioContext, 0x46, 0x100);
        if ((u32)self->field_10 <= 4) {
            switch (self->field_10) {
            case 0:
            case 2:
                self->field_10 = 4;
                break;
            case 1:
            case 3:
                self->field_10 = self->field_10 - 1;
                break;
            case 4:
                self->field_10 = 1;
                break;
            }
        }
        return;
    }
    if (flags & 0x80) {
        PlaySfx(gAudioContext, 0x46, 0x100);
        if ((u32)self->field_10 <= 4) {
            switch (self->field_10) {
            case 0:
            case 2:
                self->field_10 = self->field_10 + 1;
                break;
            case 1:
            case 3:
                self->field_10 = 4;
                break;
            case 4:
                self->field_10 = 0;
                break;
            }
        }
        return;
    }
    if (flags & 0x30) {
        if (self->field_10 != 4) {
            PlaySfx(gAudioContext, 0x46, 0x100);
            self->field_10 ^= 2;
        }
    }
}

extern void UnpackSaveData(void *cache, void *buf);
extern void SetCurrentLevel(void *cache, u8 arg1);
extern void SetSfxVolume(void *arg0, u16 arg1);
extern void SetMusicVolume(void *arg0, u16 arg1);

/* States 1/2's input handler (the two icon slider rows, `handle` =
 * field_8c/field_90 respectively): confirm/cancel-combo either resets
 * to state 0 (if maxed out) or, if the currently-highlighted row isn't
 * already selected (IsSaveSlotEmpty), toggles it on and pulls its stats
 * into the current-selection scratch fields; cancel (bit 1) resets to
 * state 0; otherwise falls through to the shared L/R cursor mover. */
void SaveMenuLoadInput(struct save_menu *self, u32 flags, void *handle)
{
    u8 buf[0x70];
    extern u8 IsSaveSlotEmpty(void *handle, s32 rowIndex);

    if (flags & 1) {
        goto confirm;
    }
    if (flags & 8) {
    confirm:
        if (self->field_10 == 4) {
            PlaySfx(gAudioContext, 0x49, 0x100);
            self->state = 0;
            self->field_10 = 0;
            return;
        }
        if (IsSaveSlotEmpty(handle, self->field_10)) {
            PlaySfx(gAudioContext, 0x48, 0x100);
            return;
        }
        PlaySfx(gAudioContext, 0x49, 0x100);
        ReadSaveSlot(handle, self->field_10, buf);
        UnpackSaveData(gLevelState, buf);
        SetCurrentLevel(gLevelState, buf[0x68]);
        SetSfxVolume(gAudioContext, *(u16 *)&buf[0x6a]);
        SetMusicVolume(gAudioContext, *(u16 *)&buf[0x6c]);
        SummarizeProgress(self, &self->currentStats, PackSaveData(gLevelState));
        self->field_20 = 1;
        self->field_8 = 1;
        return;
    }
    if (flags & 2) {
        PlaySfx(gAudioContext, 0x47, 0x100);
        self->state = 0;
        self->field_10 = 0;
        return;
    }
    SaveMenuMoveCursor(self, flags);
}

extern s32 LinkExchangeSaveData(struct save_menu *self);
/* Matches EndLinkSaveTransfer's real (void)-taking, unused-argument prototype
 * from src/save/save_menu_ui.c - this call site passes `self`
 * anyway (the ROM's caller sets it up in r0 even though the callee
 * never reads it), so it's declared here as taking one ignored
 * argument to reproduce that dead register setup. */
extern void EndLinkSaveTransfer(void *arg0);
extern s32 gCrash2LinkTextPtr;
extern s32 gCrash3LinkTextPtr;

/* State 3's input handler: polls the SIO-handshake spinner
 * (LinkExchangeSaveData, parked). Timeout/cancel -> settle back to state 0;
 * error/checksum-mismatch -> a "connection failed" message (state 4);
 * otherwise, if both sides agree on the checksummed record
 * (GetSaveGameId's version nibble), accept it (state 2); if they don't,
 * inspect the remote's nibble to merge either flag 2 or flag 4 into our
 * own record and show a matching "conflict" message. */
void SaveMenuLinkInput(struct save_menu *self)
{
    extern u32 GetSaveGameId(void *arg0);
    extern s32 StoreSaveData(void *arg0);
    extern void SetSaveFlags(void *handle, u8 flags);
    s32 state = LinkExchangeSaveData(self);

    EndLinkSaveTransfer(self);

    if (state == 3) {
        self->state = 0;
        self->field_10 = 1;
        PlaySfx(gAudioContext, 0x47, 0x100);
        return;
    }

    if (state == 2 || !CheckSaveChecksum(self->field_90)) {
        self->state = 4;
        self->field_14 = GetUiText(0x2c);
        self->field_18 = GetUiText(0x2e);
        return;
    }

    if (GetSaveGameId(self->field_8c) == GetSaveGameId(self->field_90)) {
        self->state = 2;
        self->field_10 = 0;
        RefreshSaveSlotSummaries(self, self->field_90);
        return;
    }

    switch (GetSaveGameId(self->field_90)) {
    case 2:
        SetSaveFlags(self->field_8c, 2);
        StoreSaveData(self->field_8c);
        self->state = 4;
        self->field_14 = gCrash2LinkTextPtr;
        break;
    case 3:
        SetSaveFlags(self->field_8c, 4);
        StoreSaveData(self->field_8c);
        self->state = 4;
        self->field_14 = gCrash3LinkTextPtr;
        break;
    default:
        self->state = 0;
        self->field_10 = 1;
        return;
    }
    self->field_18 = GetUiText(0x2e);
}

extern u8 IsSaveSlotEmpty(void *handle, s32 rowIndex);
extern void MemCopy32(void *dst, const void *src, s32 size);
extern s32 GetCurrentLevel(void *arg0);
extern s32 GetSfxVolume(void *arg0);
extern s32 GetMusicVolume(void *arg0);
extern s32 StoreSaveData(void *arg0);

/* Shared "commit or refresh row `rowIndex`" step used by states 5-9
 * below: pulls the row's stats/name/icon scratch data, feeds it through
 * `field_8c`'s pending-edit slot, and either finalises the edit
 * (EraseSaveSlot, when it wasn't already selected) or just refreshes the
 * row's aggregate stats. */
void SaveGameToSlot(struct save_menu *self, s32 rowIndex)
{
    u8 buf[0xe0];
    void **handleAddr = &self->field_8c;
    void **handleAddr2;
    u32 wasSelected;
    void **c0Addr;
    void **bcAddr;

    if (!IsSaveSlotEmpty(*handleAddr, rowIndex)) {
        ReadSaveSlot(*handleAddr, rowIndex, buf);
        wasSelected = 0;
    } else {
        wasSelected = 1;
    }

    c0Addr = &gLevelState;
    {
        /* The ROM evaluates PackSaveData()'s result before computing
         * `buf + 0x70` (the ROM's own callee-arg setup order for
         * MemCopy32, not the other way around) - a plain nested call
         * expression here lets this compiler compute the pointer
         * argument first instead. */
        void *result = PackSaveData(*c0Addr);
        MemCopy32(buf + 0x70, result, 0x68);
    }
    *(u8 *)(buf + 0xd8) = (u8)GetCurrentLevel(*c0Addr);

    bcAddr = &gAudioContext;
    *(u16 *)(buf + 0xda) = (u16)GetSfxVolume(*bcAddr);
    *(u16 *)(buf + 0xdc) = (u16)GetMusicVolume(*bcAddr);

    /* The ROM recomputes `self->field_8c`'s address a second time here
     * (a fresh `adds r4, r7, #0` / `adds r4, #0x8c` pair) rather than
     * reusing the register the first computation above left live -
     * mirror that with a second local instead of reusing `handleAddr`,
     * matching the technique noted in docs/matching.md for this class
     * of gap. */
    handleAddr2 = &self->field_8c;
    WriteSaveSlot(*handleAddr2, rowIndex, buf + 0x70);
    if (StoreSaveData(*handleAddr2)) {
        if (wasSelected) {
            EraseSaveSlot(*handleAddr2, rowIndex);
        } else {
            WriteSaveSlot(*handleAddr2, rowIndex, buf);
        }
    } else {
        SummarizeProgress(self, &self->rowStats[rowIndex], PackSaveData(*c0Addr));
    }
}
/* Trailing byte-padding mismatch fix: GAS's default Thumb code
 * alignment filler is the `mov r8, r8` NOP (0x46c0), but the ROM pads
 * this function's tail with a zero halfword instead (see
 * docs/matching.md's alignment-padding gotcha / the
 * matching_decomp_alignment_fix convention). */
asm(".align 2, 0");

extern void SaveGameToSlot(struct save_menu *self, s32 rowIndex);
extern void SaveMenuMoveCursor(struct save_menu *self, u32 flags);

/* State 7's input handler: confirm/cancel-combo commits row `field_24`
 * (SaveGameToSlot, src/save/save_menu_input.c) and returns to state 0
 * if it was already the "current" row (`field_10==0`), else re-enters
 * state 5 to reselect; cancel (bit 1) re-enters state 5 too; L/R toggle
 * `field_10` between 0/1. */
void SaveMenuOverwriteInput(struct save_menu *self, u32 flags)
{
    if (flags & 1) {
        goto confirm;
    }
    if (flags & 8) {
    confirm:
        if (self->field_10 == 0) {
            SaveGameToSlot(self, self->field_24);
            self->state = 0;
            self->field_10 = 4;
        } else {
            self->state = 5;
            self->field_10 = self->field_24;
            PlaySfx(gAudioContext, 0x49, 0x100);
        }
        return;
    }
    if (flags & 2) {
        self->state = 5;
        self->field_10 = self->field_24;
        PlaySfx(gAudioContext, 0x47, 0x100);
        return;
    }
    if (flags & 0x40) {
        if (self->field_10 == 1) {
            self->field_10 = 0;
            PlaySfx(gAudioContext, 0x46, 0x100);
        }
        return;
    }
    if (flags & 0x80) {
        if (self->field_10 == 0) {
            self->field_10 = 1;
            PlaySfx(gAudioContext, 0x46, 0x100);
        }
    }
}

/* State 5's input handler: confirm/cancel-combo either resets to state
 * 0 (maxed out) or, if row `field_10` isn't already selected
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
        if (self->field_10 == 4) {
            PlaySfx(gAudioContext, 0x49, 0x100);
            self->state = 0;
            self->field_10 = 2;
            return;
        }
        PlaySfx(gAudioContext, 0x49, 0x100);
        if (!IsSaveSlotEmpty(self->field_8c, self->field_10)) {
            self->state = 9;
            self->field_24 = self->field_10;
            self->field_10 = 0;
        } else {
            SaveGameToSlot(self, self->field_10);
            self->state = 0;
            self->field_10 = 4;
        }
        return;
    }
    if (flags & 2) {
        PlaySfx(gAudioContext, 0x47, 0x100);
        self->state = 0;
        self->field_10 = 2;
        return;
    }
    SaveMenuMoveCursor(self, flags);
}

/* State 6's input handler - same shape as SaveMenuSaveInput above, a
 * different row-selection sub-menu (state 7 on confirm-when-unselected,
 * field_10 target value 3 rather than 2). */
void SaveMenuDeleteInput(struct save_menu *self, u32 flags)
{
    if (flags & 1) {
        goto confirm;
    }
    if (flags & 8) {
    confirm:
        if (self->field_10 == 4) {
            PlaySfx(gAudioContext, 0x49, 0x100);
            self->state = 0;
            self->field_10 = 3;
            return;
        }
        if (IsSaveSlotEmpty(self->field_8c, self->field_10)) {
            PlaySfx(gAudioContext, 0x48, 0x100);
            return;
        }
        PlaySfx(gAudioContext, 0x49, 0x100);
        self->state = 7;
        self->field_24 = self->field_10;
        self->field_10 = 0;
        return;
    }
    if (flags & 2) {
        PlaySfx(gAudioContext, 0x47, 0x100);
        self->state = 0;
        self->field_10 = 3;
        return;
    }
    SaveMenuMoveCursor(self, flags);
}

/* State 9's input handler: confirm/cancel-combo commits row `field_24`
 * unconditionally (ReadSaveSlot+EraseSaveSlot+optional WriteSaveSlot) then
 * settles at state 0; cancel (bit 1) re-enters state 6; L/R toggle
 * `field_10` between 0/1. */
void SaveMenuConfirmDeleteInput(struct save_menu *self, u32 flags)
{
    u8 buf[0x70];

    if (flags & 1) {
        goto confirm;
    }
    if (flags & 8) {
    confirm:
        if (self->field_10 == 0) {
            s32 rowIndex = self->field_24;
            void *handle = self->field_8c;

            ReadSaveSlot(handle, rowIndex, buf);
            handle = self->field_8c;
            EraseSaveSlot(handle, rowIndex);
            handle = self->field_8c;
            if (StoreSaveData(handle)) {
                handle = self->field_8c;
                WriteSaveSlot(handle, rowIndex, buf);
            }
            self->state = 0;
            self->field_10 = 4;
        } else {
            self->state = 6;
            self->field_10 = self->field_24;
            PlaySfx(gAudioContext, 0x49, 0x100);
        }
        return;
    }
    if (flags & 2) {
        self->state = 6;
        self->field_10 = self->field_24;
        PlaySfx(gAudioContext, 0x47, 0x100);
        return;
    }
    if (flags & 0x40) {
        if (self->field_10 == 1) {
            self->field_10 = 0;
            PlaySfx(gAudioContext, 0x46, 0x100);
        }
        return;
    }
    if (flags & 0x80) {
        if (self->field_10 == 0) {
            self->field_10 = 1;
            PlaySfx(gAudioContext, 0x46, 0x100);
        }
    }
}

extern struct bitmap_font *gSmallFont;
extern s32 FontSetPalette(void *mgr, s32 arg1);
extern s32 GetSaveMenuBlinkPalette(struct save_menu *self);
extern void DrawSaveSlotStats(struct save_menu *self, s32 label1, s32 label2, s32 rowIdx, u8 flag);
extern s32 gSaveMenuOptions[];

/* Draws the 5-entry state-select sub-menu label list (SaveMenuMainInput's
 * `field_10` states, gSaveMenuOptions's label table) into
 * gSmallFont, highlighting whichever row matches `field_10`,
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
 * docs/matching/issue-5-overlay-ui-sync.md for the write-up. */
void DrawSaveMenuMain(struct save_menu *self)
{
    register struct save_menu *selfReg asm("r9") = self;
    register s32 y asm("sl") = 0x64;
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
    register struct bitmap_font **mgrAddr asm("r8");
    register s32 label asm("r6");
    register struct bitmap_font *mgr asm("r4");

    asm volatile(
        "ldr r1, =gSmallFont\n"
        "mov %0, r1\n"
        : "=r" (mgrAddr)
        :
        : "r1"
    );

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

        /* %0 = mgr, %1 = label, %2 = y, %3 = i, %4 = mgrAddr */
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
            : "+r" (mgr), "+r" (label)
            : "r" (y), "r" (i), "r" (mgrAddr)
            : "r0", "r1", "r2", "r3", "r5", "r12", "lr", "cc", "memory"
        );

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
    }
}
