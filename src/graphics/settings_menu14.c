#include "core.h"
#include "icon_manager.h"
#include "vram_pool.h"
#include "memory.h"

/* `ShowPowerDialog` (GitHub issue #8) - the higher-level dialog spawner:
 * resets palette color 0 and `REG_DISPCNT`, re-initializes the popup-
 * text system's `gSmallFont`/`030012E0` icon managers (the same
 * `FontResetPalette` init call the between-level map screen's `InitCredits`
 * uses), resets the shared VRAM upload cursor `gObjVramCursor`,
 * fires each icon manager's `record->slots[6]` trampoline via
 * `_call_via_r1` and reserves its `tileCount<<5` bytes of VRAM via
 * `ReserveObjVram` (copying `gSmallFont`'s `tileCount` into
 * `gLargeFont`'s `tileBase` in between - meaning not otherwise
 * established), then allocates the dialog object (`OperatorNew(0x2c)`,
 * exactly `src/graphics/settings_menu13.c`'s `struct
 * sub_8006700_actor`'s own size) and builds it via `InitPowerDialog`
 * (matched, same file) before running its fade-in/wait-for-confirm/
 * fade-out lifecycle via `PowerDialogLoop` and, if the confirm button was
 * pressed (a non-NULL result), `DestroyPowerDialog(dialog, 3)`.
 *
 * Has no `bl` caller in raw asm - it's called from the four
 * already-matched `ShowTurboRunDialog`...`ShowSuperBodySlamDialog` wrappers in
 * `src/graphics/oam_count.c`, each with a fixed `(label1, label2,
 * type)` triple.
 *
 * Once a NAKED transcription. It matches as plain C under both
 * compilers: the icon-manager set-up is the same `IconSetup`/
 * `IconReserve` inline-helper sequence `RunLevelSelect`
 * (src/graphics/actor_part_1b85c.c) uses, and `FontResetPalette` takes one
 * argument. See docs/matching/issue-4-6-8-naked-retry.md. */

/* Same struct sub_8006700_actor shape src/graphics/settings_menu13.c
 * documents (redeclared locally per this project's convention). */
struct sub_8006700_actor {
    u8 unused_00[0x10];
    s32 field_10;
    void *field_14;
    void *field_18;
    u32 field_1c;
    u32 field_20;
    u8 field_24;
    u8 unused_25[3];
    u16 field_28;
};

extern void *OperatorNew(s32 size);
extern s32 GetUiText(s32 arg0);
extern void WaitForVBlank(void);
extern s32 mem_free_bytes(s32 flags);
extern void FreeUnlockedPaletteSlots(struct palette_cache *self);
extern void FontResetPalette(struct icon_manager *self);
extern void ResetObjVram(struct vram_upload_cursor *self);
extern s32 ReserveObjVram(struct vram_upload_cursor *self, s32 size);
extern void MarkObjVram(struct vram_upload_cursor *self);
extern void _call_via_r1(void *addr, void *fn);
extern void DestroyPowerDialog(struct sub_8006700_actor *self, u32 flags);
extern void PowerDialogLoop(struct sub_8006700_actor *self);
extern struct sub_8006700_actor *InitPowerDialog(struct sub_8006700_actor *self, s32 label1, s32 label2, s32 type);

extern struct palette_cache *gPaletteCache;
extern struct vram_upload_cursor *gObjVramCursor;
extern struct icon_manager *gSmallFont;
extern struct icon_manager *gLargeFont;

static inline void IconSetup(struct icon_manager *m, u32 v)
{
    struct icon_slot *slot;

    m->tileBase = v;
    slot = &m->record->slots[6];
    _call_via_r1((u8 *)m + slot->offset, slot->ptr);
}

static inline void IconReserve(struct icon_manager **m)
{
    struct vram_upload_cursor *c = gObjVramCursor;

    ReserveObjVram(c, (*m)->tileCount << 5);
}

void ShowPowerDialog(s32 label1, s32 label2, s32 type)
{
    struct sub_8006700_actor *dialog;

    mem_free_bytes(0xC0000000);
    WaitForVBlank();
    *(vu16 *)PLTT = 0;
    *(vu16 *)REG_ADDR_DISPCNT = 0;
    FreeUnlockedPaletteSlots(gPaletteCache);
    FontResetPalette(gSmallFont);
    FontResetPalette(gLargeFont);
    gObjVramCursor->baseTile = 0;
    ResetObjVram(gObjVramCursor);
    ResetObjVram(gObjVramCursor);
    IconSetup(gSmallFont, 0);
    IconReserve(&gSmallFont);
    {
        u32 v = gSmallFont->tileCount;

        IconSetup(gLargeFont, v);
    }
    IconReserve(&gLargeFont);
    MarkObjVram(gObjVramCursor);
    dialog = InitPowerDialog(OperatorNew(0x2c), GetUiText(label1), GetUiText(label2), type);
    PowerDialogLoop(dialog);
    if (dialog != NULL)
        DestroyPowerDialog(dialog, 3);
    FreeUnlockedPaletteSlots(gPaletteCache);
    mem_free_bytes(0xC0000000);
}
