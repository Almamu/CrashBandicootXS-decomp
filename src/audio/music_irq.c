#include "core.h"
#include "irq.h"
#include "audio.h"

extern void sub_80005A0(s32 interruptIndex, irq_handler_t *fn);
extern struct AudioContext *gUnknown_030012BC;
extern void sub_80016EC(struct AudioContext *self);

void sub_8001CA4(void);

/* Installs `sub_8001CA4` as the VCount-IRQ handler and arms VCount IRQs
 * with a fixed trigger line (`0x35`) - the music player's per-tick fade
 * update (`sub_80016EC`, `src/audio/music_player.c`) runs off this
 * VCount interrupt rather than VBlank. See that file's header comment,
 * which already anticipated this function. */
void sub_8001C80(void)
{
    register vu8 *p asm("r1");
    register u8 v asm("r0");
    register u8 loaded asm("r2");

    sub_80005A0(INTR_INDEX_VCOUNT, sub_8001CA4);
    p = (vu8 *)REG_ADDR_DISPSTAT;
    p[1] = 0x35;
    v = DISPSTAT_VCOUNT_INTR;
    loaded = *p;
    v |= loaded;
    *p = v;
}

/* The VCount-IRQ handler installed by `sub_8001C80` above: just forwards
 * into the music player's per-tick fade update. */
void sub_8001CA4(void)
{
    sub_80016EC(gUnknown_030012BC);
}
