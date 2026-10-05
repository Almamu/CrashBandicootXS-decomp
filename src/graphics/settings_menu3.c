#include "core.h"
#include "save_menu.h"

/* Bit-2 flag test repeated throughout this chunk's functions - matches
 * GetSaveMenuBlinkPalette's own trivial body: caller-visible "1" (set) vs "2"
 * (clear). */
s32 GetSaveMenuBlinkPalette(struct save_menu *self)
{
    if ((self->flags >> 2) & 1) {
        return 1;
    }
    return 2;
}

extern void ResetLinkSession(void *arg0);
extern void *gLinkSession;

void EndLinkSaveTransfer(void)
{
    void *p = gLinkSession;
    ResetLinkSession(p);
    *((u8 *)p + 5) = 0;
}

extern void ResetSaveData(void *arg0);

void BeginLinkSaveTransfer(struct save_menu *self)
{
    ResetLinkSession(gLinkSession);
    *((u8 *)gLinkSession + 5) = 1;
    ResetSaveData(self->field_90);
}

extern void DrawSaveMenuTitle(struct save_menu *self, s32 labelIndex);
extern void DrawSaveSlots(struct save_menu *self, void *handle, s32 arg2);
extern void DrawYesNoPrompt(struct save_menu *self, s32 labelIndex);
extern void DrawSaveMenuCancel(struct save_menu *self, u8 highlight);
extern void DrawSaveMenuMessageLines(struct save_menu *self, s32 label1, s32 label2);

void DrawSaveMenuConfirmDelete(struct save_menu *self)
{
    DrawSaveMenuTitle(self, 0x1d);
    DrawSaveSlots(self, self->field_8c, self->field_24);
    DrawYesNoPrompt(self, 0x26);
}

void DrawSaveMenuDelete(struct save_menu *self)
{
    DrawSaveMenuTitle(self, 0x1d);
    DrawSaveSlots(self, self->field_8c, self->field_10);
    DrawSaveMenuCancel(self, self->field_10 == 4);
}

void DrawSaveMenuOverwrite(struct save_menu *self)
{
    DrawSaveMenuTitle(self, 0x1e);
    DrawSaveSlots(self, self->field_8c, self->field_24);
    DrawYesNoPrompt(self, 0x27);
}

void DrawSaveMenuSave(struct save_menu *self)
{
    DrawSaveMenuTitle(self, 0x1e);
    DrawSaveSlots(self, self->field_8c, self->field_10);
    DrawSaveMenuCancel(self, self->field_10 == 4);
}

void DrawSaveMenuMessage(struct save_menu *self)
{
    DrawSaveMenuTitle(self, 0x1c);
    DrawSaveMenuMessageLines(self, self->field_14, self->field_18);
}

void DrawSaveMenuLoadLink(struct save_menu *self)
{
    DrawSaveMenuTitle(self, 0x1c);
    DrawSaveSlots(self, self->field_90, self->field_10);
    DrawSaveMenuCancel(self, self->field_10 == 4);
}

void DrawSaveMenuLoad(struct save_menu *self)
{
    DrawSaveMenuTitle(self, 0x1b);
    DrawSaveSlots(self, self->field_8c, self->field_10);
    DrawSaveMenuCancel(self, self->field_10 == 4);
}

extern void ResetOamBuffer(void *arg0);
extern void HideUnusedOamEntries(void *arg0);
extern void RewindObjVram(void *arg0);
extern void DrawSaveMenuMain(struct save_menu *self);
extern void *gOamBuffer;
extern void *gObjVramCursor;

void DrawSaveMenu(struct save_menu *self)
{
    ResetOamBuffer(gOamBuffer);
    RewindObjVram(gObjVramCursor);
    if ((u32)self->state <= 0xa) {
        switch (self->state) {
        case 0:
            DrawSaveMenuMain(self);
            break;
        case 1:
            DrawSaveMenuLoad(self);
            break;
        case 2:
            DrawSaveMenuLoadLink(self);
            break;
        case 3:
        case 4:
            DrawSaveMenuMessage(self);
            break;
        case 5:
            DrawSaveMenuSave(self);
            break;
        case 6:
            DrawSaveMenuDelete(self);
            break;
        case 9:
            DrawSaveMenuOverwrite(self);
            break;
        case 7:
            DrawSaveMenuConfirmDelete(self);
            break;
        case 8:
            break;
        case 10:
            break;
        default:
            break;
        }
    }
    HideUnusedOamEntries(gOamBuffer);
}

extern void ReadSaveSlot(void *handle, s32 rowIndex, void *buf);
extern void EraseSaveSlot(void *arg0, s32 arg1);
extern s32 StoreSaveData(void *arg0);
extern void WriteSaveSlot(void *arg0, s32 arg1, void *buf);

void DeleteSaveSlot(struct save_menu *self, s32 arg1)
{
    u8 buf[0x70];

    ReadSaveSlot(self->field_8c, arg1, buf);
    EraseSaveSlot(self->field_8c, arg1);
    if (StoreSaveData(self->field_8c)) {
        WriteSaveSlot(self->field_8c, arg1, buf);
    }
}
asm(".align 2, 0");
