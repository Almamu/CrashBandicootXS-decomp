/* The reference for the pc platform's GAX2 (platform/pc/src/gax_trace.c):
 * the ROM run in libmgba with a key file, logging every call the game
 * makes into GAX2's API and every block GAX2 mixes. tools/boottest.py's
 * `gax-trace` builds and runs it (docs/platforms.md); it is not part of
 * any build.
 *
 *     mgba_gax_trace ROM KEYS FRAMES OUT SYMBOL=ADDRESS...
 *
 * KEYS is a key file expanded to lines `FRAME KEYS`, as mgba_screens.c
 * reads it, and a game frame is one read of REG_KEYINPUT. The symbols
 * (boottest gives them from the ROM's ELF) are GAX2's API functions,
 * gGaxPlayerState and gGaxMusicData, and optionally `level` (the return
 * address of RunLevelSelect's call of LevelSelectLoop) with `levelN`, the
 * level to pick there, as mgba_screens.c's --level.
 *
 * A breakpoint at each API function's entry logs the call with its
 * arguments (gax_trace.c has the format), GAX2_init's from the parameter
 * block r0 points at, GAX_play's the block's master volume and filter.
 * At GAX_irq's and GAX_play's entries, if GAX_play ran since the last
 * GAX_irq (the player state's playDone) and the player isn't stopped, the
 * block it mixed (the half of outBuf before outHalf) is logged by its
 * rate, length and FNV-1a hash: each block once, after it is finished.
 * The game's other code runs unchanged; the breakpoints make mGBA's
 * debugger run the CPU an instruction at a time, more slowly. */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mgba/core/config.h>
#include <mgba/core/core.h>
#include <mgba/core/interface.h>
#include <mgba/core/log.h>
#include <mgba/debugger/debugger.h>
#include <mgba/internal/arm/arm.h>

#define WIDTH  240
#define HEIGHT 160

typedef unsigned char u8;
typedef unsigned int u32;
typedef int s32;

/* GAX2's player state and parameter block on the GBA (lib/gax/src/
 * gax_internal.h, <gax.h>): the offsets read. */
#define GAX_MAGIC         0x47415832
#define STATE_SONG        0x04
#define STATE_FORMAT      0x14
#define STATE_OUT_BUF     0x18
#define STATE_OUT_HALF    0x2c
#define STATE_STATE       0x30
#define STATE_PLAY_DONE   0x43
#define FORMAT_MIX_RATE   0x02
#define FORMAT_FRAMES     0x04
#define PARAMS_MIX_RATE   0x08
#define PARAMS_FILTER     0x0a
#define PARAMS_FLAGS      0x0c
#define PARAMS_NUM_SFX    0x0e
#define PARAMS_VOLUME     0x10
#define PARAMS_SFX_TYPES  0x2c
#define PARAMS_LAYOUT     0x30
#define PARAMS_SHOW_ERROR 0x38

struct Run {
    u32 frame;
    u32 keys;
};

enum {
    FN_INIT,
    FN_FX_EX,
    FN_FX,
    FN_FX_NOTE,
    FN_STOP_FX,
    FN_MUSIC_VOLUME,
    FN_FX_VOLUME,
    FN_STOP,
    FN_PAUSE,
    FN_RESUME,
    FN_PLAY,
    FN_IRQ,
    FN_JINGLE,
    FN_LEVEL,
    FN_COUNT
};

static const char *const sNames[FN_COUNT] = {
    "GAX2_init",         "GAX_fx_ex",   "GAX_fx",
    "GAX_fx_note",       "GAX_stop_fx", "GAX_set_music_volume",
    "GAX_set_fx_volume", "GAX_stop",    "GAX_pause",
    "GAX_resume",        "GAX_play",    "GAX_irq",
    "GAX2_jingle",       "level",
};

static struct mCore *sCore;
static color_t sVideo[WIDTH * HEIGHT];
static struct Run *sRuns;
static u32 sRunCount;
static u32 sRun;
static u32 sFrame;
static u32 sFrames;
static u32 sVBlanks;
static int sDone;
static FILE *sOut;
static struct mDebugger sDebugger;
static u32 sAddress[FN_COUNT];
static u32 sPlayerState;
static u32 sMusicData;
static s32 sLevel = -1;
static int sLevelPicked;

static void Log(struct mLogger *logger, int category, enum mLogLevel level, const char *format,
                va_list args)
{
    (void)logger;
    (void)category;
    (void)level;
    (void)format;
    (void)args;
}

static struct mLogger sLogger = { Log, NULL };

static u32 Read32(u32 address)
{
    return sCore->rawRead32(sCore, address, -1);
}

static u32 Read16(u32 address)
{
    return sCore->rawRead16(sCore, address, -1);
}

static u32 Read8(u32 address)
{
    return sCore->rawRead8(sCore, address, -1);
}

static void FrameEnded(void *context)
{
    (void)context;
    sVBlanks++;
}

static void KeysRead(void *context)
{
    u32 keys = 0;

    (void)context;
    if (sDone) {
        return;
    }
    sFrame++;
    if (sFrame > sFrames) {
        sDone = 1;
        return;
    }
    while (sRun + 1 < sRunCount && sRuns[sRun + 1].frame <= sFrame) {
        sRun++;
    }
    if (sRunCount != 0 && sRuns[sRun].frame <= sFrame) {
        keys = sRuns[sRun].keys;
    }
    sCore->setKeys(sCore, keys);
}

/* The player state, if GAX2 has one. */
static u32 PlayerState(void)
{
    u32 st = Read32(sPlayerState);

    return st != 0 && Read32(st) == GAX_MAGIC ? st : 0;
}

/* The block GAX_play mixed last, if it ran since the last GAX_irq. */
static void LogBlock(void)
{
    u32 st = PlayerState();
    u32 format;
    u32 n;
    u32 out;
    u32 hash = 2166136261u;
    u32 i;

    if (st == 0 || !Read8(st + STATE_PLAY_DONE) || Read32(st + STATE_STATE) == 0) {
        return;
    }
    format = Read32(st + STATE_FORMAT);
    n = Read16(format + FORMAT_FRAMES);
    out = Read32(st + STATE_OUT_BUF) + n * (Read32(st + STATE_OUT_HALF) ^ 1);
    for (i = 0; i < n; i++) {
        hash = (hash ^ Read8(out + i)) * 16777619u;
    }
    fprintf(sOut, "block %u %u %08x\n", Read16(format + FORMAT_MIX_RATE), n, hash);
}

static void Entered(struct mDebugger *debugger, enum mDebuggerEntryReason reason,
                    struct mDebuggerEntryInfo *info)
{
    struct ARMCore *cpu = sCore->cpu;
    u32 r0 = cpu->gprs[0];
    u32 r1 = cpu->gprs[1];
    s32 fn;

    debugger->state = DEBUGGER_RUNNING;
    if (reason != DEBUGGER_ENTER_BREAKPOINT) {
        return;
    }
    for (fn = 0; fn < FN_COUNT && sAddress[fn] != info->address; fn++) {
    }
    switch (fn) {
    case FN_LEVEL:
        if (!sLevelPicked) {
            sLevelPicked = 1;
            cpu->gprs[0] = sLevel;
        }
        break;
    case FN_INIT:
        {
            u32 sfx = Read32(r0 + PARAMS_SFX_TYPES);

            fprintf(sOut, "init %d %d %u %u %u %u %u %u # vblank %u frame %u\n",
                    (s32)(Read32(r0 + PARAMS_LAYOUT) - sMusicData),
                    sfx != 0 ? (s32)(sfx - sMusicData) : -1, Read16(r0 + PARAMS_MIX_RATE),
                    Read16(r0 + PARAMS_FILTER), Read16(r0 + PARAMS_FLAGS),
                    Read16(r0 + PARAMS_NUM_SFX), Read16(r0 + PARAMS_VOLUME),
                    Read8(r0 + PARAMS_SHOW_ERROR), sVBlanks, sFrame);
            break;
        }
    case FN_FX_EX:
        fprintf(sOut, "fx_ex %u %d %d %d\n", r0, (s32)r1, (s32)cpu->gprs[2], (s32)cpu->gprs[3]);
        break;
    case FN_FX:
        fprintf(sOut, "fx %u\n", r0);
        break;
    case FN_FX_NOTE:
        fprintf(sOut, "fx_note %d %u\n", (s32)r0, r1);
        break;
    case FN_STOP_FX:
        fprintf(sOut, "stop_fx %d\n", (s32)r0);
        break;
    case FN_MUSIC_VOLUME:
        fprintf(sOut, "music_vol %d %u\n", (s32)r0, r1);
        break;
    case FN_FX_VOLUME:
        fprintf(sOut, "fx_vol %d %u\n", (s32)r0, r1);
        break;
    case FN_STOP:
        fprintf(sOut, "stop\n");
        break;
    case FN_PAUSE:
        fprintf(sOut, "pause\n");
        break;
    case FN_RESUME:
        fprintf(sOut, "resume\n");
        break;
    case FN_JINGLE:
        fprintf(sOut, "jingle %d\n", (s32)(r0 - sMusicData));
        break;
    case FN_PLAY:
        {
            u32 st = PlayerState();

            LogBlock();
            if (st != 0) {
                u32 params = Read32(st + STATE_SONG);

                fprintf(sOut, "play %u %u\n", Read16(params + PARAMS_VOLUME),
                        Read16(params + PARAMS_FILTER));
            } else {
                fprintf(sOut, "play - -\n");
            }
            break;
        }
    case FN_IRQ:
        LogBlock();
        fprintf(sOut, "irq\n");
        break;
    }
}

static void ReadKeys(const char *path)
{
    FILE *f = fopen(path, "r");
    u32 cap = 0;
    unsigned frame;
    unsigned keys;

    if (f == NULL) {
        perror(path);
        exit(2);
    }
    while (fscanf(f, "%u %u", &frame, &keys) == 2) {
        if (sRunCount == cap) {
            cap = cap ? 2 * cap : 64;
            sRuns = realloc(sRuns, cap * sizeof(*sRuns));
        }
        sRuns[sRunCount].frame = frame;
        sRuns[sRunCount].keys = keys;
        sRunCount++;
    }
    fclose(f);
}

int main(int argc, char **argv)
{
    struct mCoreCallbacks callbacks;
    struct mCoreOptions opts;
    u32 vblanks = 0;
    s32 fn;
    int i;

    if (argc < 5) {
        fprintf(stderr, "usage: %s ROM KEYS FRAMES OUT SYMBOL=ADDRESS...\n", argv[0]);
        return 2;
    }
    for (i = 5; i < argc; i++) {
        char *eq = strchr(argv[i], '=');
        u32 value;

        if (eq == NULL) {
            fprintf(stderr, "%s: not SYMBOL=ADDRESS\n", argv[i]);
            return 2;
        }
        *eq = '\0';
        value = strtoul(eq + 1, NULL, 0);
        if (strcmp(argv[i], "gGaxPlayerState") == 0) {
            sPlayerState = value;
        } else if (strcmp(argv[i], "gGaxMusicData") == 0) {
            sMusicData = value;
        } else if (strcmp(argv[i], "levelN") == 0) {
            sLevel = (s32)value;
        } else {
            for (fn = 0; fn < FN_COUNT && strcmp(argv[i], sNames[fn]) != 0; fn++) {
            }
            if (fn == FN_COUNT) {
                fprintf(stderr, "%s: no such symbol\n", argv[i]);
                return 2;
            }
            sAddress[fn] = value & ~1u;
        }
    }
    mLogSetDefaultLogger(&sLogger);
    ReadKeys(argv[2]);
    sFrames = strtoul(argv[3], NULL, 0);
    sOut = fopen(argv[4], "w");
    if (sOut == NULL) {
        perror(argv[4]);
        return 2;
    }

    sCore = mCoreFind(argv[1]);
    if (sCore == NULL || !sCore->init(sCore)) {
        fprintf(stderr, "%s: not a ROM mGBA runs\n", argv[1]);
        return 2;
    }
    sCore->setVideoBuffer(sCore, sVideo, WIDTH);
    if (!mCoreLoadFile(sCore, argv[1])) {
        fprintf(stderr, "%s: can't load\n", argv[1]);
        return 2;
    }
    mCoreConfigInit(&sCore->config, NULL);
    memset(&opts, 0, sizeof(opts));
    opts.useBios = false;
    mCoreConfigLoadDefaults(&sCore->config, &opts);
    mCoreConfigSetOverrideIntValue(&sCore->config, "allowOpposingDirections", 1);
    mCoreLoadConfig(sCore);

    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.videoFrameEnded = FrameEnded;
    callbacks.keysRead = KeysRead;
    sCore->addCoreCallbacks(sCore, &callbacks);
    sCore->reset(sCore);
    sDebugger.entered = Entered;
    mDebuggerAttach(&sDebugger, sCore);
    for (fn = 0; fn < FN_COUNT; fn++) {
        struct mBreakpoint bp;

        if (sAddress[fn] == 0 || (fn == FN_LEVEL && sLevel < 0)) {
            continue;
        }
        memset(&bp, 0, sizeof(bp));
        bp.address = sAddress[fn];
        bp.segment = -1;
        bp.type = BREAKPOINT_HARDWARE;
        sDebugger.platform->setBreakpoint(sDebugger.platform, &bp);
    }
    sDebugger.state = DEBUGGER_RUNNING;

    while (!sDone) {
        mDebuggerRunFrame(&sDebugger);
        if (++vblanks > 20 * (sFrames + 600)) {
            fprintf(stderr, "no key reads: the game hung at frame %u\n", sFrame);
            return 1;
        }
    }
    fclose(sOut);
    printf("end done at frame %u (VBlank %u)\n", sFrame, sVBlanks);
    sCore->deinit(sCore);
    return 0;
}
