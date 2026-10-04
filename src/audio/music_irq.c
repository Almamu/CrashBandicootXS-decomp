#include "core.h"
#include "irq.h"
#include "audio.h"

extern void IrqSetHandler(s32 interruptIndex, irq_handler_t *fn);
extern struct AudioContext *gAudioContext;
extern void UpdateAudio(struct AudioContext *self);

void MusicVCountIrqHandler(void);

/* Installs `MusicVCountIrqHandler` as the VCount-IRQ handler and arms VCount IRQs
 * with a fixed trigger line (`0x35`) - the music player's per-tick fade
 * update (`UpdateAudio`, `src/audio/music_player.c`) runs off this
 * VCount interrupt rather than VBlank. See that file's header comment,
 * which already anticipated this function. */
void EnableMusicVCountIrq(void)
{
    register vu8 *p asm("r1");
    register u8 v asm("r0");
    register u8 loaded asm("r2");

    IrqSetHandler(INTR_INDEX_VCOUNT, MusicVCountIrqHandler);
    p = (vu8 *)REG_ADDR_DISPSTAT;
    p[1] = 0x35;
    v = DISPSTAT_VCOUNT_INTR;
    loaded = *p;
    v |= loaded;
    *p = v;
}

/* The VCount-IRQ handler installed by `EnableMusicVCountIrq` above: just forwards
 * into the music player's per-tick fade update. */
void MusicVCountIrqHandler(void)
{
    UpdateAudio(gAudioContext);
}
