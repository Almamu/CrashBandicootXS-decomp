#ifndef __IRQ_H__
#define __IRQ_H__

/* The interrupt table's types (src/engine/sys/irq.cpp). The table, the VBlank
 * callbacks and their functions are class Irq's (irq.hpp); IntrMain
 * (platform/gba/asm/intr_main.s) dispatches each IRQ through Irq::table (gIntrTable). */

#include "core.h"

typedef void (*irq_handler_t)(void);

/* The VBlank callback slots: Irq::AddVBlankCallback() fills a free one,
 * Irq::VBlankHandler() calls every non-zero one each VBlank. */
struct vblank_callbacks {
    irq_handler_t funcs[8];
};

/* gIntrTable[INTR_INDEX_TIMER2] under its own games/xs/sym_iwram.txt name: the
 * slot SetEepromTimerIntr() installs the EEPROM timer handler in
 * (save_data.cpp). */
extern void (*gIntrTableTimer2)(void);
/* IntrMain's IWRAM copy (platform/gba/asm/intr_main.s), what INTR_VECTOR points at. */
extern u32 IntrMain_Buffer;

#endif /* __IRQ_H__ */
