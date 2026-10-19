/* The reference pictures for the pc platform's PPU (platform/pc/src/ppu.c):
 * the ROM run in libmgba with a key file, writing what mGBA's own renderer
 * put on the screen. tools/boottest.py's `screens` builds and runs it
 * (docs/platforms.md); it is not part of any build.
 *
 *     mgba_screens ROM KEYS FRAMES EVERY DIR [--save FILE]
 *                  [--level ADDRESS N [--room ADDRESS R | --extra-room ADDRESS RETURN ENTER]]
 *
 * KEYS is a key file expanded to lines `FRAME KEYS` (KEYS a number, the
 * active-high bits of REG_KEYINPUT), as boottest.py reads platform/pc's.
 * A game frame is one read of REG_KEYINPUT (KeyInput::Update reads it once
 * a frame), as in boottest's `run` and in the host executable: mGBA's
 * keysRead callback counts the reads and sets that frame's keys before
 * the read returns them. Every EVERY frames, at that frame's read, the
 * picture mGBA finished last (videoFrameEnded) is written to DIR/FRAME.png
 * (unless DIR is "") and `screen FRAME HASH VBLANKS` printed: the SHA-1 of
 * its RGB bytes (the first 16 hex digits, as the host's checkpoints print
 * its SCREEN) and how many frames mGBA has drawn. After FRAMES frames it
 * stops. No BIOS file (mGBA's HLE BIOS) and no save, as boottest's runs;
 * with --save, mGBA keeps the EEPROM in FILE (erased first), the image the
 * host's --save writes.
 *
 * --level and --room are boottest's level scripts' overrides, as the
 * host's --level and --room (platform/pc/src/level_pick.c): at the first
 * stop at ADDRESS (boottest gives the return address of RunLevelSelect's
 * call of LevelSelectLoop) r0, the level chosen, becomes N; at the first
 * stop at the room's ADDRESS (SelectRoom) after that, the roomIndex at r0
 * + 4 becomes R. They are breakpoints of mGBA's debugger, which runs the
 * CPU an instruction at a time, more slowly.
 *
 * --extra-room is the host's --room bonus and --room gem-path: the first
 * SelectRoom (ADDRESS) after the level is picked runs as the game's, and
 * when it returns to UpdateGameFrame (RETURN, boottest gives the address
 * after its call) the room block it was called on (its r0, kept at
 * ADDRESS) goes to ENTER (EnterBonusRoom or EnterGemPathRoom), which
 * returns to RETURN again with SelectRoom's result put back in r0: the
 * call level_pick.c's wrapper makes after SelectRoom's. */

#include <fcntl.h>
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
#include <mgba/internal/arm/isa-inlines.h>
#include <mgba-util/vfs.h>

#define WIDTH  240
#define HEIGHT 160

typedef unsigned char u8;
typedef unsigned int u32;
typedef int s32;
typedef unsigned long long u64;

/* platform/pc/src/sha1.c and png.c, linked in. */
struct HostSha1 {
    u32 h[5];
    u64 length;
    u8 block[64];
    u32 used;
};

void HostSha1Init(struct HostSha1 *s);
void HostSha1Update(struct HostSha1 *s, const void *data, u32 n);
void HostSha1Final(struct HostSha1 *s, u8 digest[20]);
int HostWritePng(const char *path, const u32 *xrgb, u32 width, u32 height);

struct Run {
    u32 frame;
    u32 keys;
};

static struct mCore *sCore;
static color_t sVideo[WIDTH * HEIGHT];
static u32 sLast[WIDTH * HEIGHT];
static struct Run *sRuns;
static u32 sRunCount;
static u32 sRun;
static u32 sFrame;
static u32 sFrames;
static u32 sEvery;
static const char *sDir;
static int sDone;

/* --level and --room. */
static struct mDebugger sDebugger;
static u32 sLevelAddress;
static s32 sLevel = -1;
static u32 sRoomAddress;
static s32 sRoom = -1;
static int sLevelPicked;
static int sRoomPicked;

/* --extra-room: 1 once SelectRoom is entered, 2 while ENTER runs, 3 done. */
static u32 sExtraReturn;
static u32 sExtraEnter;
static int sExtraState;
static u32 sExtraSelf;
static u32 sExtraResult;

/* mGBA's log (the BIOS calls, ...) isn't wanted. */
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

/* mGBA's 32-bit colour is XBGR8, the host's pictures XRGB8. */
static void FrameEnded(void *context)
{
    u32 i;

    (void)context;
    for (i = 0; i < WIDTH * HEIGHT; i++) {
        u32 c = sVideo[i];

        sLast[i] = (c & 0xFF) << 16 | (c & 0xFF00) | ((c >> 16) & 0xFF);
    }
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
    if (sFrame % sEvery == 0) {
        struct HostSha1 sha;
        u8 rgb[WIDTH * HEIGHT * 3];
        u8 digest[20];
        char path[4096];
        u32 i;

        for (i = 0; i < WIDTH * HEIGHT; i++) {
            rgb[3 * i] = sLast[i] >> 16;
            rgb[3 * i + 1] = sLast[i] >> 8;
            rgb[3 * i + 2] = sLast[i];
        }
        HostSha1Init(&sha);
        HostSha1Update(&sha, rgb, sizeof(rgb));
        HostSha1Final(&sha, digest);
        printf("screen %u ", sFrame);
        for (i = 0; i < 8; i++) {
            printf("%02x", digest[i]);
        }
        printf(" %u\n", sCore->frameCounter(sCore));
        if (sDir[0] != '\0') {
            snprintf(path, sizeof(path), "%s/%05u.png", sDir, sFrame);
            if (!HostWritePng(path, sLast, WIDTH, HEIGHT)) {
                fprintf(stderr, "%s: can't write\n", path);
                exit(2);
            }
        }
    }
}

static void Entered(struct mDebugger *debugger, enum mDebuggerEntryReason reason,
                    struct mDebuggerEntryInfo *info)
{
    struct ARMCore *cpu = sCore->cpu;

    if (reason == DEBUGGER_ENTER_BREAKPOINT) {
        if (info->address == sLevelAddress && !sLevelPicked) {
            sLevelPicked = 1;
            cpu->gprs[0] = sLevel;
        } else if (info->address == sRoomAddress && sLevelPicked && !sRoomPicked) {
            sRoomPicked = 1;
            if (sExtraEnter != 0) {
                sExtraState = 1;
                sExtraSelf = cpu->gprs[0];
            } else {
                sCore->busWrite32(sCore, cpu->gprs[0] + 4, sRoom);
            }
        } else if (info->address == sExtraReturn && sExtraState == 1) {
            /* SelectRoom returned: call ENTER(self), back to RETURN (Thumb) */
            sExtraState = 2;
            sExtraResult = cpu->gprs[0];
            cpu->gprs[0] = sExtraSelf;
            cpu->gprs[ARM_LR] = sExtraReturn | 1;
            cpu->gprs[ARM_PC] = sExtraEnter;
            ThumbWritePC(cpu);
        } else if (info->address == sExtraReturn && sExtraState == 2) {
            sExtraState = 3;
            cpu->gprs[0] = sExtraResult;
        }
    }
    debugger->state = DEBUGGER_RUNNING;
}

static void Break(u32 address)
{
    struct mBreakpoint bp;

    memset(&bp, 0, sizeof(bp));
    bp.address = address;
    bp.segment = -1;
    bp.type = BREAKPOINT_HARDWARE;
    sDebugger.platform->setBreakpoint(sDebugger.platform, &bp);
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
    const char *save = NULL;
    u32 vblanks = 0;
    int i;

    for (i = 6; i < argc; i++) {
        if (strcmp(argv[i], "--save") == 0 && i + 1 < argc) {
            save = argv[++i];
        } else if (strcmp(argv[i], "--level") == 0 && i + 2 < argc) {
            sLevelAddress = strtoul(argv[i + 1], NULL, 0);
            sLevel = strtol(argv[i + 2], NULL, 0);
            i += 2;
        } else if (strcmp(argv[i], "--room") == 0 && i + 2 < argc) {
            sRoomAddress = strtoul(argv[i + 1], NULL, 0);
            sRoom = strtol(argv[i + 2], NULL, 0);
            i += 2;
        } else if (strcmp(argv[i], "--extra-room") == 0 && i + 3 < argc) {
            sRoomAddress = strtoul(argv[i + 1], NULL, 0);
            sExtraReturn = strtoul(argv[i + 2], NULL, 0);
            sExtraEnter = strtoul(argv[i + 3], NULL, 0);
            sRoom = 0;
            i += 3;
        } else {
            argc = 0;
        }
    }
    if (argc < 6) {
        fprintf(
            stderr,
            "usage: %s ROM KEYS FRAMES EVERY DIR [--save FILE]\n"
            "       [--level ADDRESS N [--room ADDRESS R | --extra-room ADDRESS RETURN ENTER]]\n",
            argv[0]);
        return 2;
    }
    mLogSetDefaultLogger(&sLogger);
    ReadKeys(argv[2]);
    sFrames = strtoul(argv[3], NULL, 0);
    sEvery = strtoul(argv[4], NULL, 0);
    sDir = argv[5];
    if (sEvery == 0) {
        sEvery = 1;
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
    /* The defaults, not a user's config file. Opposing directions are
     * passed on, as boottest's `run` (which sets the read's register)
     * passes them. */
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
    /* A save file mGBA keeps the EEPROM in (written as it changes). */
    if (save != NULL) {
        struct VFile *vf = VFileOpen(save, O_CREAT | O_TRUNC | O_RDWR);

        if (vf == NULL || !sCore->loadSave(sCore, vf)) {
            fprintf(stderr, "%s: can't open\n", save);
            return 2;
        }
    }
    sCore->reset(sCore);
    if (sLevel >= 0) {
        sDebugger.entered = Entered;
        mDebuggerAttach(&sDebugger, sCore);
        Break(sLevelAddress);
        if (sRoom >= 0) {
            Break(sRoomAddress);
        }
        if (sExtraEnter != 0) {
            Break(sExtraReturn);
        }
        sDebugger.state = DEBUGGER_RUNNING;
    }

    while (!sDone) {
        if (sLevel >= 0) {
            mDebuggerRunFrame(&sDebugger);
        } else {
            sCore->runFrame(sCore);
        }
        if (++vblanks > 20 * (sFrames + 600)) {
            fprintf(stderr, "no key reads: the game hung at frame %u\n", sFrame);
            return 1;
        }
    }
    printf("end done at frame %u (VBlank %u)\n", sFrame, vblanks);
    sCore->deinit(sCore);
    return 0;
}
