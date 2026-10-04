#include "core.h"
#include "pause_options_screen.h"

/* Bit-2 flag test repeated throughout this chunk's functions - matches
 * sub_8004A50's own trivial body: caller-visible "1" (set) vs "2"
 * (clear). */
s32 sub_8004A50(struct pause_options_screen *self)
{
    if ((self->flags >> 2) & 1) {
        return 1;
    }
    return 2;
}

extern void sub_8002798(void *arg0);
extern void *gLinkSession;

void sub_8004A64(void)
{
    void *p = gLinkSession;
    sub_8002798(p);
    *((u8 *)p + 5) = 0;
}

extern void ResetSaveData(void *arg0);

void sub_8004A80(struct pause_options_screen *self)
{
    sub_8002798(gLinkSession);
    *((u8 *)gLinkSession + 5) = 1;
    ResetSaveData(self->field_90);
}

extern void sub_80049CC(struct pause_options_screen *self, s32 labelIndex);
extern void sub_80041BC(struct pause_options_screen *self, void *handle, s32 arg2);
extern void sub_8003D3C(struct pause_options_screen *self, s32 labelIndex);
extern void sub_8003C90(struct pause_options_screen *self, u8 highlight);
extern void sub_8003BDC(struct pause_options_screen *self, s32 label1, s32 label2);

void DrawSaveMenuConfirmDelete(struct pause_options_screen *self)
{
    sub_80049CC(self, 0x1d);
    sub_80041BC(self, self->field_8c, self->field_24);
    sub_8003D3C(self, 0x26);
}

void DrawSaveMenuDelete(struct pause_options_screen *self)
{
    sub_80049CC(self, 0x1d);
    sub_80041BC(self, self->field_8c, self->field_10);
    sub_8003C90(self, self->field_10 == 4);
}

void DrawSaveMenuOverwrite(struct pause_options_screen *self)
{
    sub_80049CC(self, 0x1e);
    sub_80041BC(self, self->field_8c, self->field_24);
    sub_8003D3C(self, 0x27);
}

void DrawSaveMenuSave(struct pause_options_screen *self)
{
    sub_80049CC(self, 0x1e);
    sub_80041BC(self, self->field_8c, self->field_10);
    sub_8003C90(self, self->field_10 == 4);
}

void DrawSaveMenuMessage(struct pause_options_screen *self)
{
    sub_80049CC(self, 0x1c);
    sub_8003BDC(self, self->field_14, self->field_18);
}

void DrawSaveMenuLoadLink(struct pause_options_screen *self)
{
    sub_80049CC(self, 0x1c);
    sub_80041BC(self, self->field_90, self->field_10);
    sub_8003C90(self, self->field_10 == 4);
}

void DrawSaveMenuLoad(struct pause_options_screen *self)
{
    sub_80049CC(self, 0x1b);
    sub_80041BC(self, self->field_8c, self->field_10);
    sub_8003C90(self, self->field_10 == 4);
}

extern void ResetOamBuffer(void *arg0);
extern void HideUnusedOamEntries(void *arg0);
extern void RewindObjVram(void *arg0);
extern void DrawSaveMenuMain(struct pause_options_screen *self);
extern void *gOamBuffer;
extern void *gObjVramCursor;

void DrawSaveMenu(struct pause_options_screen *self)
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

void DeleteSaveSlot(struct pause_options_screen *self, s32 arg1)
{
    u8 buf[0x70];

    ReadSaveSlot(self->field_8c, arg1, buf);
    EraseSaveSlot(self->field_8c, arg1);
    if (StoreSaveData(self->field_8c)) {
        WriteSaveSlot(self->field_8c, arg1, buf);
    }
}
asm(".align 2, 0");
