/* The pc platform's startup, in place of the GBA's crt0.s
 * (platform/gba/asm/).
 *
 * crt0's job on the GBA is the CPU modes and stacks, the IWRAM image's
 * copy and AgbMain. On the host the image's code and data are ordinary
 * host code and data, and the host memory (memory.S) starts zeroed, so
 * main reads the command line and runs AgbMain.
 *
 *     crashbandicootxs_pc [--headless] [--scale N] [--save FILE] [--screenshot-at N]
 *                         [--frames N] [--vblanks N] [--keys FILE]
 *                         [--checkpoint-every N] [--dump DIR]
 *                         [--screens DIR] [--ppu-stats] [--quiet]
 *
 * In a window (sdl.c), at the GBA's 59.73 frames a second, played with the
 * keyboard or a gamepad. --headless (and every run of an executable built
 * without SDL) shows nothing and takes no time: the frames run as fast as
 * the host goes, for tests (tools/boottest.py `host`). input.c has the key
 * file and the checkpoints, irq.c the frame loop, ppu.c the picture,
 * eeprom.c the save. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gba/gba.h"
#include "engine/sys/system.h"
#include "host_platform.h"

struct HostOptions gHostOptions = { 0, 0, 60, NULL, NULL, NULL, NULL, 3, 0, -1, -1, 0, 0, 0 };

void HostExit(int status, const char *how)
{
    HostSaveFlush();
    if (gHostOptions.ppuStats) {
        HostPpuPrintStats();
    }
    printf("end %s at frame %u (VBlank %u)\n", how, HostFrame(), gHostVBlanks);
    fflush(stdout);
    exit(status);
}

static void Usage(const char *argv0)
{
    fprintf(
        stderr,
        "usage: %s [--headless] [--scale N] [--save FILE] [--frames N] [--vblanks N]\n"
        "       [--keys FILE] [--checkpoint-every N] [--dump DIR] [--screens DIR]\n"
        "       [--ppu-stats] [--quiet]\n"
        "  --headless            no window, no frame pacing (always, built without SDL)\n"
        "  --scale N             the window's size, N times the GBA's (3)\n"
        "  --save FILE           keep the EEPROM in FILE (read at start, written as it changes)\n"
        "  --screenshot-at N     in the window, take F12's screenshot at game frame N\n"
        "  --level N             the level select picks level N (gLevelTable's index), once\n"
        "  --room R              with --level: the level starts in its room R\n"
        "  --frames N            stop after N game frames (KeyInput::Update calls)\n"
        "  --vblanks N           stop after N VBlanks\n"
        "  --keys FILE           scripted input: lines `FRAME KEYS` (input.c)\n"
        "  --checkpoint-every N  hash palette RAM, VRAM and OAM every N frames (60; 0: never)\n"
        "  --dump DIR            write each checkpoint's palette RAM, VRAM and OAM to DIR\n"
        "  --screens DIR         write each checkpoint's picture to DIR/FRAME.png\n"
        "  --ppu-stats           count the lines that use each PPU feature, print at the end\n"
        "  --quiet               don't print the checkpoints\n",
        argv0);
    exit(2);
}

static u32 Number(const char *argv0, const char *text)
{
    char *end;
    unsigned long v = strtoul(text, &end, 0);

    if (*text == '\0' || *end != '\0') {
        Usage(argv0);
    }
    return (u32)v;
}

int main(int argc, char **argv)
{
    int i;

    for (i = 1; i < argc; i++) {
        const char *arg = argv[i];
        const char *value = i + 1 < argc ? argv[i + 1] : NULL;

        if (strcmp(arg, "--quiet") == 0) {
            gHostOptions.quiet = 1;
            continue;
        }
        if (strcmp(arg, "--headless") == 0) {
            gHostOptions.headless = 1;
            continue;
        }
        if (strcmp(arg, "--ppu-stats") == 0) {
            gHostOptions.ppuStats = 1;
            continue;
        }
        if (value == NULL) {
            Usage(argv[0]);
        }
        if (strcmp(arg, "--frames") == 0) {
            gHostOptions.frames = Number(argv[0], value);
        } else if (strcmp(arg, "--vblanks") == 0) {
            gHostOptions.vblanks = Number(argv[0], value);
        } else if (strcmp(arg, "--checkpoint-every") == 0) {
            gHostOptions.checkpointEvery = Number(argv[0], value);
        } else if (strcmp(arg, "--keys") == 0) {
            gHostOptions.keys = value;
        } else if (strcmp(arg, "--dump") == 0) {
            gHostOptions.dumpDir = value;
        } else if (strcmp(arg, "--screens") == 0) {
            gHostOptions.screenDir = value;
        } else if (strcmp(arg, "--save") == 0) {
            gHostOptions.save = value;
        } else if (strcmp(arg, "--level") == 0) {
            gHostOptions.level = (s32)Number(argv[0], value);
        } else if (strcmp(arg, "--room") == 0) {
            gHostOptions.room = (s32)Number(argv[0], value);
        } else if (strcmp(arg, "--screenshot-at") == 0) {
            gHostOptions.screenshotAt = Number(argv[0], value);
        } else if (strcmp(arg, "--scale") == 0) {
            gHostOptions.scale = Number(argv[0], value);
            if (gHostOptions.scale == 0) {
                Usage(argv[0]);
            }
        } else {
            Usage(argv[0]);
        }
        i++;
    }
    HostInputInit();
    if (!HostFrontendAvailable()) {
        gHostOptions.headless = 1;
    }
    if (!gHostOptions.headless) {
        HostFrontendInit();
    }

    if (AgbMain() != 0) {
        HostExit(1, "AgbMain failed");
    }
    HostExit(0, "AgbMain returned");
    return 0;
}
