#ifndef GUARD_PC_HOST_PLATFORM_H
#define GUARD_PC_HOST_PLATFORM_H

/* The pc platform library's own interface, between its units
 * (platform/pc/src/): what the command line asks for (main.c), the frame
 * loop and the IRQs (irq.c), the scripted input and the checkpoints
 * (input.c). Not for the game: the game sees the GBA's interface only. */

#include "gba/gba.h"

/* The command line (main.c). */
struct HostOptions {
    u32 frames;          /* --frames N: stop after N game frames (0: never) */
    u32 vblanks;         /* --vblanks N: stop after N VBlanks (0: never) */
    u32 checkpointEvery; /* --checkpoint-every N (default 60) */
    const char *keys;    /* --keys FILE: the scripted input */
    const char *dumpDir; /* --dump DIR: the checkpoints' memory, raw */
    int quiet;           /* --quiet: no checkpoint lines */
};

extern struct HostOptions gHostOptions;

/* The VBlanks so far (irq.c). */
extern u32 gHostVBlanks;

/* The scripted input (input.c): reads the key file. */
void HostInputInit(void);

/* The game frame: how many times the game has read REG_KEYINPUT. */
u32 HostFrame(void);

/* Ends the run: prints how it ended and exits with `status`. */
void HostExit(int status, const char *how);

/* IntrMain's dispatch (irq.c). */
void HostIntrMain(void);

/* SHA-1 of `n` bytes, continuing `state` (sha1.c). */
struct HostSha1 {
    u32 h[5];
    u64 length;
    u8 block[64];
    u32 used;
};

void HostSha1Init(struct HostSha1 *s);
void HostSha1Update(struct HostSha1 *s, const void *data, u32 n);
void HostSha1Final(struct HostSha1 *s, u8 digest[20]);

#endif /* GUARD_PC_HOST_PLATFORM_H */
