#include "core.h"
#include "graphics_package.h"

/* GitHub issue #30. Both built with old_agbcc - see
 * docs/matching/issue-30-old-agbcc.md. */

/* The BG control value InitBgSetup built, for REG_BGnCNT. */
u16 GetBgSetupControl(struct bg_setup *self)
{
    return self->ctrl.raw;
}

/* Fills the BG setup buffer: char block, screen block and palette bank
 * verbatim, and a control value with priority `priority`, char base
 * `charBlock`, screen base `screenBlock` and size 0. */
struct bg_setup *InitBgSetup(struct bg_setup *self, u32 charBlock, u32 screenBlock, u32 paletteBank, u32 priority)
{
    self->ctrl.raw = 0;
    self->ctrl.bits.priority = priority;
    self->charBlock = charBlock;
    self->ctrl.bits.charBase = charBlock;
    self->ctrl.bits.size = 0;
    self->screenBlock = screenBlock;
    self->ctrl.bits.screenBase = screenBlock;
    self->paletteBank = paletteBank;
    return self;
}
/* Zero-fill the trailing halfword, as the ROM does. */
asm(".align 2, 0");
