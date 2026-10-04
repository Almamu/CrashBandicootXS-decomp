#include "core.h"
#include "gba/io_reg.h"
#include "audio.h"

extern struct GaxPlayerState *gGaxPlayerState;

/* Direct Sound A output "stop": clears the GAX2 player-state's `state`
 * once it's non-zero and disables the DMA1 sound-A output pair in
 * SOUNDCNT_H (bits 8/9). Mirror of GAX_resume below. */
void GAX_pause(void)
{
    struct GaxPlayerState *p = gGaxPlayerState;

    if (p->state != 0) {
        p->state = 0;
        REG_SOUNDCNT_H &= 0xFCFF;
    }
}

/* Direct Sound A output "start": flushes FIFO_A (8 zero halfwords) and
 * enables the DMA1 sound-A output pair once `state` is still zero
 * (mirrors GAX_pause's teardown). */
void GAX_resume(void)
{
    struct GaxPlayerState *p = gGaxPlayerState;
    vu16 *fifo;
    u16 zero;
    s32 i;

    if (p->state == 0) {
        p->state = 1;
        fifo = (vu16 *)REG_ADDR_FIFO_A;
        zero = 0;
        for (i = 7; i >= 0; i--) {
            *fifo = zero;
        }
        REG_SOUNDCNT_H |= 0xc0 << 2;
    }
}
