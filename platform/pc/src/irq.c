/* The pc platform's frame loop and interrupts, in place of the GBA's
 * IntrMain (platform/gba/asm/intr_main.s) and the BIOS's VBlankIntrWait.
 *
 * The game runs on the main thread and takes no time: the display's time
 * passes only inside VBlankIntrWait, the one place the game waits for it
 * (FrameLimiter::WaitForVBlank, irq.cpp). Each call runs the display's
 * scanlines from where the last one stopped (line 160) up to the next
 * VBlank: line 161 to 227, then 0 to 160, one frame. Per line it sets
 * VCOUNT and DISPSTAT's flags as the GBA's display does, and raises in IF
 * the interrupts DISPSTAT enables: VCount at the line in DISPSTAT's top
 * byte (the music fade's 0x35, audio.cpp), VBlank at line 160. Each
 * visible line is drawn (ppu.c) once its IRQs have run, and at line 160
 * the frame is the finished picture, which the window shows (sdl.c),
 * waiting there for the frame's time to pass. HBlank is
 * never raised (the game enables no HBlank IRQ). A raised interrupt that
 * IE enables runs the game's handler from gIntrTable (Irq::Setup) right
 * away, as IntrMain does; VBlankIntrWait returns once the VBlank's has
 * run (INTR_CHECK, as on the GBA).
 *
 * Timers and the serial port raise nothing: the EEPROM (eeprom.c, a RAM
 * buffer) needs no Timer2 timeout, and the link (Timer3, Serial) never
 * connects. */

#include <stdio.h>

#include "gba/gba.h"
#include <agb_syscall.h>
#include "engine/sys/irq.h"
#include "host_platform.h"

#define LINES_PER_FRAME 228
#define VBLANK_LINE     160

u32 gHostVBlanks;

/* What Irq::Setup stores in INTR_VECTOR: on the GBA the IWRAM copy of
 * IntrMain. Here only its address is used, as the vector: the platform
 * calls HostIntrMain itself. */
u32 IntrMain_Buffer;

/* The raised interrupts. IF is a register the game acknowledges by
 * writing 1s to it (the link's `REG_IF |= INTR_FLAG_SERIAL`), which host
 * memory can't model, so the platform keeps IF here and copies it to
 * REG_IF for the game to read. */
static u16 sRaised;

/* The current scanline: the GBA's display is somewhere in line 0 when
 * the game starts. */
static u32 sLine;

/* IntrMain's job: while IME is on, run the handler of the lowest
 * interrupt that is both enabled (IE) and raised, acknowledging it first,
 * and for a VBlank set the BIOS's IntrCheck flag, which VBlankIntrWait
 * waits for. One at a time, as the GBA (IntrMain doesn't nest). */
void HostIntrMain(void)
{
    extern irq_handler_t gIntrTable[14];

    while (REG_IME & 1) {
        u16 pending = REG_IE & sRaised;
        s32 i;

        if (pending == 0) {
            break;
        }
        for (i = 0; !(pending & (1 << i)); i++) {
        }
        sRaised &= ~(1 << i);
        REG_IF = sRaised;
        if (i == INTR_INDEX_VBLANK) {
            INTR_CHECK = INTR_FLAG_VBLANK;
        }
        gIntrTable[i]();
    }
}

static void RaiseIrq(u16 flag)
{
    sRaised |= flag;
    REG_IF = sRaised;
    HostIntrMain();
}

/* The display moves on one scanline. */
static void NextScanline(void)
{
    u16 dispstat;

    sLine = (sLine + 1) % LINES_PER_FRAME;
    dispstat = REG_DISPSTAT & ~(DISPSTAT_VBLANK | DISPSTAT_HBLANK | DISPSTAT_VCOUNT);
    /* The VBlank flag is set from line 160 to 226, not in the last line. */
    if (sLine >= VBLANK_LINE && sLine < LINES_PER_FRAME - 1) {
        dispstat |= DISPSTAT_VBLANK;
    }
    if (sLine == (u32)(dispstat >> 8)) {
        dispstat |= DISPSTAT_VCOUNT;
    }
    REG_VCOUNT = sLine;
    REG_DISPSTAT = dispstat;

    if (sLine == VBLANK_LINE) {
        gHostVBlanks++;
        /* The picture is finished: shown (the window), and the time a
         * frame takes waited for, before the game's VBlank handler. */
        HostPpuFrame();
        HostSaveFlush();
        if (!gHostOptions.headless) {
            HostFrontendVBlank();
        }
        if (gHostOptions.vblanks != 0 && gHostVBlanks > gHostOptions.vblanks) {
            HostExit(0, "vblanks");
        }
        if (dispstat & DISPSTAT_VBLANK_INTR) {
            RaiseIrq(INTR_FLAG_VBLANK);
        }
    }
    if ((dispstat & (DISPSTAT_VCOUNT | DISPSTAT_VCOUNT_INTR)) ==
        (DISPSTAT_VCOUNT | DISPSTAT_VCOUNT_INTR)) {
        RaiseIrq(INTR_FLAG_VCOUNT);
    }
    /* The line is drawn after its VCount IRQ, as the GBA draws it during
     * the line's H-draw, after the IRQ at its start. */
    if (sLine < VBLANK_LINE) {
        HostPpuLine(sLine);
    }
}

/* BIOS VBlankIntrWait (IntrWait(1, INTR_FLAG_VBLANK)): turns IME on,
 * clears IntrCheck's VBlank flag and lets the display run until the
 * VBlank handler sets it again. On the GBA a VBlank IRQ that is never
 * enabled hangs here; the host stops with an error instead. */
void VBlankIntrWait(void)
{
    u32 lines = 0;

    REG_IME = 1;
    INTR_CHECK &= ~INTR_FLAG_VBLANK;
    while (!(INTR_CHECK & INTR_FLAG_VBLANK)) {
        NextScanline();
        if (++lines > 4 * LINES_PER_FRAME) {
            HostExit(2, "VBlankIntrWait with the VBlank IRQ disabled");
        }
    }
}
