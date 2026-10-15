/* The pc platform's startup, in place of the GBA's crt0.s
 * (platform/gba/asm/).
 *
 * crt0's job on the GBA is the CPU modes and stacks, the IWRAM image's
 * copy and AgbMain. On the host the image's code and data are ordinary
 * host code and data, and the host memory (memory.S) starts zeroed, so
 * main reads the command line and runs AgbMain.
 *
 *     crashbandicootxs_pc [--frames N] [--vblanks N] [--keys FILE]
 *                         [--checkpoint-every N] [--dump DIR] [--quiet]
 *
 * Headless: the game runs, nothing is shown (input.c has the key file
 * and the checkpoints, irq.c the frame loop). */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gba/gba.h"
#include "engine/sys/system.h"
#include "host_platform.h"

struct HostOptions gHostOptions = { 0, 0, 60, NULL, NULL, 0 };

void HostExit(int status, const char *how)
{
    printf("end %s at frame %u (VBlank %u)\n", how, HostFrame(), gHostVBlanks);
    fflush(stdout);
    exit(status);
}

static void Usage(const char *argv0)
{
    fprintf(stderr,
            "usage: %s [--frames N] [--vblanks N] [--keys FILE] [--checkpoint-every N]\n"
            "       [--dump DIR] [--quiet]\n"
            "  --frames N            stop after N game frames (KeyInput::Update calls)\n"
            "  --vblanks N           stop after N VBlanks\n"
            "  --keys FILE           scripted input: lines `FRAME KEYS` (input.c)\n"
            "  --checkpoint-every N  hash palette RAM, VRAM and OAM every N frames (60; 0: never)\n"
            "  --dump DIR            write each checkpoint's palette RAM, VRAM and OAM to DIR\n"
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
        } else {
            Usage(argv[0]);
        }
        i++;
    }
    HostInputInit();

    if (AgbMain() != 0) {
        HostExit(1, "AgbMain failed");
    }
    HostExit(0, "AgbMain returned");
    return 0;
}
