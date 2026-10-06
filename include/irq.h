#ifndef __IRQ_H__
#define __IRQ_H__

/* The interrupt table and the VBlank callbacks (src/system/irq.c).
 * IntrMain (asm/intr_main.s) dispatches each IRQ through gIntrTable. */

#include "core.h"

typedef void (*irq_handler_t)(void);

/* The VBlank callback slots: AddVBlankCallback() fills a free one,
 * VBlankHandler() calls every non-zero one each VBlank. */
struct vblank_callbacks {
    s32 funcs[8];
    char pad2[48];
}; // 0x80

/* gIntrTable is the per-IRQ handler table IntrMain dispatches through,
 * one entry per REG_IF bit (IrqSetup fills all 14); gPrevIntrTable holds
 * the handler each IrqSetHandler() call replaced, for
 * IrqRestoreHandler(). Both are in sym_iwram.txt, 0x38 bytes apart. */
extern irq_handler_t gIntrTable[14];
extern irq_handler_t gPrevIntrTable[14];
/* gIntrTable[INTR_INDEX_TIMER2] under its own sym_iwram.txt name: the
 * slot SetEepromTimerIntr() installs the EEPROM timer handler in
 * (save_data.c). */
extern void (*gIntrTableTimer2)(void);
extern struct vblank_callbacks gVBlankCallbacks;
/* IntrMain's IWRAM copy (asm/intr_main.s), what INTR_VECTOR points at. */
extern u32 IntrMain_Buffer;

extern void IrqClearHandler(s32 interruptIndex);
extern void IrqRestoreHandler(s32 interruptIndex);
extern void IrqSetHandler(s32 interruptIndex, irq_handler_t fn);
extern void IrqDisable(void);
extern u32 IrqSetup(void);
extern void IrqEmptyHandler(void);
extern void EnableVBlankHandler(void);
extern void DisableVBlankHandler(void);
extern void RemoveVBlankCallback(s32 index);
extern s32 AddVBlankCallback(void (*fn)(void));
extern void VBlankHandler(void);

#endif /* __IRQ_H__ */
