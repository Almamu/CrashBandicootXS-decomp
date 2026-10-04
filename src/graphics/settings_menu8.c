#include "core.h"
#include "gba/dma_macros.h"
#include "settings_sync.h"

extern void UpdateSaveChecksum(void *arg0);
extern void EraseSaveSlot(void *handle, s32 rowIndex);

/* UpdateSaveChecksum recomputes and stores this record's additive checksum
 * over its first 0x1f8 bytes (a plain word-sum loop, still raw) - every
 * function below that mutates `flags`/`slotEmpty` calls it afterward
 * to keep the checksum in sync. */

/* Zeroes the whole 0x200-byte record via a DMA16 fill, marks every row
 * "selected" (EraseSaveSlot, still raw), stamps the two fixed marker
 * bytes, clears `flags`/`field_1fb`, then refreshes the checksum. */
void ResetSaveData(struct settings_sync_record *self)
{
    s32 i;

    DmaFill16(3, 0, self, sizeof(*self));
    for (i = 0; i <= 3; i++) {
        EraseSaveSlot(self, i);
    }
    self->magic = 0x43;
    self->versionNibble = 0x12;
    self->flags = 0;
    self->field_1fb = 0;
    UpdateSaveChecksum(self);
}

u8 IsSaveSlotEmpty(struct settings_sync_record *self, s32 rowIndex)
{
    return self->slotEmpty[rowIndex];
}

u8 TestSaveFlags(struct settings_sync_record *self, u8 flags)
{
    register u8 v asm("r1");
    u8 result;

    v = flags & self->flags;
    result = v;
    if (v != 0) {
        result = 1;
    }
    return result;
}

void ClearSaveFlags(struct settings_sync_record *self, u8 flags)
{
    register u8 loaded asm("r3");
    register u8 v asm("r1");

    loaded = self->flags;
    loaded &= ~flags;
    asm volatile("add %0, %1, #0" : "=r"(v) : "r"(loaded));
    self->flags = v;
    UpdateSaveChecksum(self);
}
