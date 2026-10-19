#ifndef GUARD_PC_HOST_PLATFORM_H
#define GUARD_PC_HOST_PLATFORM_H

/* The pc platform library's own interface, between its units
 * (platform/pc/src/): what the command line asks for (main.c), the frame
 * loop and the IRQs (irq.c), the scripted input and the checkpoints
 * (input.c). Not for the game: the game sees the GBA's interface only. */

#include "gba/gba.h"

/* The command line (main.c). */
struct HostOptions {
    u32 frames;            /* --frames N: stop after N game frames (0: never) */
    u32 vblanks;           /* --vblanks N: stop after N VBlanks (0: never) */
    u32 checkpointEvery;   /* --checkpoint-every N (60; 0 in the window) */
    const char *keys;      /* --keys FILE: the scripted input */
    const char *dumpDir;   /* --dump DIR: the checkpoints' memory, raw */
    const char *screenDir; /* --screens DIR: the checkpoints' pictures, PNG */
    const char *save;      /* --save FILE: the EEPROM's image */
    const char *wav;       /* --wav FILE: the audio output (audio.c) */
    const char *gaxTrace;  /* --gax-trace FILE: replay GAX2 calls (gax_trace.c) */
    const char *gaxLog;    /* --gax-log FILE: log the game's GAX2 calls (gax_trace.c) */
    u32 scale;             /* --scale N: the window's size (default 3) */
    u32 screenshotAt;      /* --screenshot-at N: F12 at game frame N */
    s32 level;             /* --level N: the level select picks level N (-1: no) */
    s32 room;              /* --room R: and the level starts in room R (-1: no) */
    int quiet;             /* --quiet: no checkpoint lines */
    int headless;          /* --headless: no window (always, without SDL) */
    int ppuStats;          /* --ppu-stats: what the picture used, at the end */
    s32 cutscene;          /* --cutscene N: the boot plays cutscene N (-1: no) */
    s32 language;          /* --language L: in language L (gLanguage; -1: as it is) */
    s32 extraRoom;         /* --room bonus|gem-path: HOST_EXTRA_ROOM_* (0: no) */
};

/* --room bonus and --room gem-path (level_pick.c): the level's room list's
 * extra1 (EnterBonusRoom's) or extra2 (EnterGemPathRoom's) room. */
enum {
    HOST_EXTRA_ROOM_NONE,
    HOST_EXTRA_ROOM_BONUS,
    HOST_EXTRA_ROOM_GEM_PATH,
};

extern struct HostOptions gHostOptions;

/* The VBlanks so far (irq.c). */
extern u32 gHostVBlanks;

/* The PPU (ppu.c). irq.c's frame loop draws each visible line as the
 * display reaches it (HostPpuLine) and, at line 160, makes the frame the
 * finished picture (HostPpuFrame): gHostScreen, 8-bit XRGB. */
#define HOST_SCREEN_WIDTH  240
#define HOST_SCREEN_HEIGHT 160

extern u32 gHostScreen[HOST_SCREEN_WIDTH * HOST_SCREEN_HEIGHT];

void HostPpuLine(u32 y);
void HostPpuFrame(void);
void HostPpuPrintStats(void);

/* --ppu-stats: the lines that used each feature. */
enum {
    HOST_PPU_MODE0,
    HOST_PPU_FORCED_BLANK = 6,
    HOST_PPU_TEXT_4BPP,
    HOST_PPU_TEXT_8BPP,
    HOST_PPU_TEXT_SIZE0,
    HOST_PPU_AFFINE_WRAP = HOST_PPU_TEXT_SIZE0 + 4,
    HOST_PPU_AFFINE_CLIP,
    HOST_PPU_BG_MOSAIC,
    HOST_PPU_WIN0,
    HOST_PPU_WIN1,
    HOST_PPU_OBJWIN,
    HOST_PPU_BLEND_ALPHA,
    HOST_PPU_BRIGHTEN,
    HOST_PPU_DARKEN,
    HOST_PPU_OBJ_1D,
    HOST_PPU_OBJ_2D,
    HOST_PPU_OBJ_REGULAR,
    HOST_PPU_OBJ_AFFINE,
    HOST_PPU_OBJ_DOUBLE,
    HOST_PPU_OBJ_SEMI,
    HOST_PPU_OBJ_WINDOW,
    HOST_PPU_OBJ_MOSAIC,
    HOST_PPU_OBJ_8BPP,
    HOST_PPU_STAT_COUNT
};

#define HOST_PPU_BIT(n) (1u << (n))

/* The front end (sdl.c): the window, the live keys and the frame pacing.
 * Without SDL (HOST_SDL=0 in platform.mk) only HostFrontendAvailable
 * exists, and says no. */
int HostFrontendAvailable(void);
void HostFrontendInit(void);
/* At each VBlank: shows gHostScreen, reads the keyboard and gamepads,
 * waits for the frame's time. */
void HostFrontendVBlank(void);
/* The keys held now (active high). */
u16 HostFrontendKeys(void);

/* The audio output (audio.c): at each VBlank, the block GAX2 mixed in
 * that frame (or silence) to the window's audio device, --wav's file and
 * the audio hash; at the end, `audio BLOCKS HASH`. */
void HostAudioVBlank(void);
void HostAudioEnd(void);
/* --gax-trace's --wav: the block GAX_play just mixed to the file (gax_trace.c). */
void HostAudioWavBlock(void);
/* The block mixed last, if one was since the last call (gax_trace.c). */
int HostAudioTakeBlock(const s8 **buf, u32 *n, u32 *rate);
/* --gax-trace FILE: GAX2 replays the GBA's calls and compares its blocks
 * with the GBA's (gax_trace.c). Doesn't return. */
void HostGaxTrace(const char *path);
/* --gax-log's file, closed at the end. */
void HostGaxLogEnd(void);
/* The window's audio device (sdl.c): `n` signed 8-bit mono samples at
 * `rate` Hz. */
void HostFrontendAudio(const s8 *buf, u32 n, u32 rate);

/* The EEPROM's image in --save FILE (eeprom.c): written back at a VBlank
 * after the game changed it, and at the end. */
void HostSaveFlush(void);

/* A PNG of 8-bit XRGB pixels (png.c). Returns 0 on failure. */
int HostWritePng(const char *path, const u32 *xrgb, u32 width, u32 height);

/* The scripted input (input.c): reads the key file. */
void HostInputInit(void);

/* The empty sprite frames' reads past their header are 0 wide (sprite_boxes.c). */
void HostCheckEmptyFrames(void);

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
