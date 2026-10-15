/* --gax-trace FILE: GAX2 on its own, driven by a trace of the calls the
 * game made on the GBA, compared with what the GBA mixed.
 *
 * The trace (platform/pc/tools/mgba_gax_trace.c, the ROM in libmgba with
 * breakpoints on GAX2's API, run by tools/boottest.py `gax-trace`) is a
 * line per call, in the order they came:
 *
 *     init LAYOUT SFX MIXRATE FILTER FLAGS NUMSFX VOLUME SHOWERRORS
 *                              GAX2_init: the parameter block as the game
 *                              filled it (LAYOUT and SFX are offsets in
 *                              gGaxMusicData, SFX -1 for none)
 *     fx_ex INSTR CH PRIO NOTE, fx INSTR, fx_note CH NOTE, stop_fx CH,
 *     music_vol CH VOL, fx_vol CH VOL, stop, pause, resume, jingle LAYOUT
 *     play VOLUME FILTER       GAX_play, with the parameter block's master
 *                              volume and filter then (AudioContext::Update
 *                              writes the volume into it for the fades)
 *     irq                      GAX_irq
 *     block RATE N HASH        the block the GBA mixed last: its rate,
 *                              length and FNV-1a hash
 *
 * Replayed here through the platform's GAX2, each block is the GBA's or
 * not: the run prints `gax-trace BLOCKS EXACT` and the first block that
 * differs, and fails if one does. It takes the game's timing out of the
 * comparison (the host's frames take no time, so a load takes fewer
 * VBlanks than on the GBA, and the music and the sound effects meet at
 * other points): what is left is the engine, sample for sample.
 *
 * --gax-log FILE writes the same trace of the game's own calls on the
 * host: the link wraps GAX2's API (platform.mk, --wrap) with the
 * functions below, which log each call and then make it. Its calls
 * (everything but play, irq and block, which follow the VBlanks) are the
 * GBA's, in the same order and with the same arguments, when the game
 * drives the audio as it does on the GBA (tools/boottest.py
 * `gax-calls`). */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gax_host.h"
#include "host_platform.h"

extern const u8 gGaxMusicData[];

static struct GaxSongHeader sHeader;
static u8 sWork[0x2000];

static u32 Fnv(const s8 *buf, u32 n)
{
    u32 h = 2166136261u;
    u32 i;

    for (i = 0; i < n; i++) {
        h = (h ^ (u8)buf[i]) * 16777619u;
    }
    return h;
}

/* --gax-log: the wrapped API. */
static FILE *sLog;

static int Logging(void)
{
    if (sLog == NULL && gHostOptions.gaxLog != NULL) {
        sLog = fopen(gHostOptions.gaxLog, "w");
        if (sLog == NULL) {
            perror(gHostOptions.gaxLog);
            exit(2);
        }
    }
    return sLog != NULL;
}

/* The block GAX_play mixed last, if it ran since the last GAX_irq: what
 * mgba_gax_trace.c reads from the GBA's player state at the same calls. */
static void LogBlock(void)
{
    struct GaxPlayerState *st = gGaxPlayerState;
    u32 n;

    if (st == NULL || st->magic != 0x47415832 || !st->playDone || st->state == 0) {
        return;
    }
    n = st->format->frames;
    fprintf(sLog, "block %u %u %08x\n", st->format->mixRate, n,
            Fnv(st->outBuf + n * (st->outHalf ^ 1), n));
}

u8 __real_GAX2_init(struct GaxSongHeader *params);
s32 __real_GAX_fx_ex(u32 instrument, s32 channel, s32 priority, s32 pitch);
s32 __real_GAX_fx(u32 instrument);
void __real_GAX_fx_note(s32 channel, u32 period);
void __real_GAX_stop_fx(s32 channel);
void __real_GAX_set_music_volume(s32 channel, u32 volume);
void __real_GAX_set_fx_volume(s32 channel, u32 volume);
void __real_GAX_stop(void);
void __real_GAX_pause(void);
void __real_GAX_resume(void);
void __real_GAX_play(void);
void __real_GAX_irq(void);
u32 __real_GAX2_jingle(struct GaxHandlerLayout *layout);

u8 __wrap_GAX2_init(struct GaxSongHeader *p)
{
    if (Logging()) {
        fprintf(sLog, "init %d %d %u %u %u %u %u %u # vblank %u frame %u\n",
                (int)((const u8 *)p->layout - gGaxMusicData),
                p->sfxTypes != NULL ? (int)((const u8 *)p->sfxTypes - gGaxMusicData) : -1,
                p->mixRate, p->filter, p->flags, p->numSfx, p->volume, p->showErrors, gHostVBlanks,
                HostFrame());
    }
    return __real_GAX2_init(p);
}

s32 __wrap_GAX_fx_ex(u32 instrument, s32 channel, s32 priority, s32 pitch)
{
    if (Logging()) {
        fprintf(sLog, "fx_ex %u %d %d %d\n", instrument, channel, priority, pitch);
    }
    return __real_GAX_fx_ex(instrument, channel, priority, pitch);
}

s32 __wrap_GAX_fx(u32 instrument)
{
    if (Logging()) {
        fprintf(sLog, "fx %u\n", instrument);
    }
    return __real_GAX_fx(instrument);
}

void __wrap_GAX_fx_note(s32 channel, u32 period)
{
    if (Logging()) {
        fprintf(sLog, "fx_note %d %u\n", channel, period);
    }
    __real_GAX_fx_note(channel, period);
}

void __wrap_GAX_stop_fx(s32 channel)
{
    if (Logging()) {
        fprintf(sLog, "stop_fx %d\n", channel);
    }
    __real_GAX_stop_fx(channel);
}

void __wrap_GAX_set_music_volume(s32 channel, u32 volume)
{
    if (Logging()) {
        fprintf(sLog, "music_vol %d %u\n", channel, volume);
    }
    __real_GAX_set_music_volume(channel, volume);
}

void __wrap_GAX_set_fx_volume(s32 channel, u32 volume)
{
    if (Logging()) {
        fprintf(sLog, "fx_vol %d %u\n", channel, volume);
    }
    __real_GAX_set_fx_volume(channel, volume);
}

void __wrap_GAX_stop(void)
{
    if (Logging()) {
        fprintf(sLog, "stop\n");
    }
    __real_GAX_stop();
}

void __wrap_GAX_pause(void)
{
    if (Logging()) {
        fprintf(sLog, "pause\n");
    }
    __real_GAX_pause();
}

void __wrap_GAX_resume(void)
{
    if (Logging()) {
        fprintf(sLog, "resume\n");
    }
    __real_GAX_resume();
}

u32 __wrap_GAX2_jingle(struct GaxHandlerLayout *layout)
{
    if (Logging()) {
        fprintf(sLog, "jingle %d\n", (int)((const u8 *)layout - gGaxMusicData));
    }
    return __real_GAX2_jingle(layout);
}

void __wrap_GAX_play(void)
{
    if (Logging()) {
        LogBlock();
        if (gGaxPlayerState != NULL && gGaxPlayerState->magic == 0x47415832) {
            fprintf(sLog, "play %u %u\n", GAX_SONG()->volume, GAX_SONG()->filter);
        } else {
            fprintf(sLog, "play - -\n");
        }
    }
    __real_GAX_play();
}

void __wrap_GAX_irq(void)
{
    if (Logging()) {
        LogBlock();
        fprintf(sLog, "irq\n");
    }
    __real_GAX_irq();
}

void HostGaxLogEnd(void)
{
    if (sLog != NULL) {
        fclose(sLog);
        sLog = NULL;
    }
}

void HostGaxTrace(const char *path)
{
    FILE *f = fopen(path, "r");
    char line[256];
    u32 blocks = 0;
    u32 exact = 0;
    u32 unmixed = 0;
    u32 lineNo = 0;
    u32 first = 0;

    if (f == NULL) {
        perror(path);
        exit(2);
    }
    while (fgets(line, sizeof(line), f) != NULL) {
        char op[16];
        int a[8];
        unsigned hash;
        int n;

        lineNo++;
        n = sscanf(line, "%15s %d %d %d %d %d %d %d %d", op, &a[0], &a[1], &a[2], &a[3], &a[4],
                   &a[5], &a[6], &a[7]);
        if (n < 1) {
            continue;
        }
        if (strcmp(op, "init") == 0 && n >= 9) {
            GAX2_new(&sHeader);
            sHeader.workBuf = sWork;
            sHeader.workSize = sizeof(sWork);
            sHeader.layout = (struct GaxHandlerLayout *)(gGaxMusicData + a[0]);
            sHeader.sfxTypes = a[1] < 0 ? NULL : (struct GaxHandlerType **)(gGaxMusicData + a[1]);
            sHeader.mixRate = a[2];
            sHeader.filter = a[3];
            sHeader.flags = a[4];
            sHeader.numSfx = a[5];
            sHeader.volume = a[6];
            sHeader.showErrors = a[7];
            GAX2_init(&sHeader);
        } else if (strcmp(op, "fx_ex") == 0 && n >= 5) {
            GAX_fx_ex(a[0], a[1], a[2], a[3]);
        } else if (strcmp(op, "fx") == 0 && n >= 2) {
            GAX_fx(a[0]);
        } else if (strcmp(op, "fx_note") == 0 && n >= 3) {
            GAX_fx_note(a[0], a[1]);
        } else if (strcmp(op, "stop_fx") == 0 && n >= 2) {
            GAX_stop_fx(a[0]);
        } else if (strcmp(op, "music_vol") == 0 && n >= 3) {
            GAX_set_music_volume(a[0], a[1]);
        } else if (strcmp(op, "fx_vol") == 0 && n >= 3) {
            GAX_set_fx_volume(a[0], a[1]);
        } else if (strcmp(op, "stop") == 0) {
            GAX_stop();
        } else if (strcmp(op, "pause") == 0) {
            GAX_pause();
        } else if (strcmp(op, "resume") == 0) {
            GAX_resume();
        } else if (strcmp(op, "jingle") == 0 && n >= 2) {
            GAX2_jingle((struct GaxHandlerLayout *)(gGaxMusicData + a[0]));
        } else if (strcmp(op, "play") == 0) {
            if (n >= 3) {
                sHeader.volume = a[0];
                sHeader.filter = a[1];
            }
            GAX_play();
        } else if (strcmp(op, "irq") == 0) {
            GAX_irq();
        } else if (strcmp(op, "block") == 0 &&
                   sscanf(line, "block %d %d %x", &a[0], &a[1], &hash) == 3) {
            const s8 *buf;
            u32 len;
            u32 rate;

            if (!HostAudioTakeBlock(&buf, &len, &rate)) {
                /* The GBA's state said a block was mixed (GAX2_init's
                 * playDone, before its first GAX_play): nothing to compare. */
                unmixed++;
                continue;
            }
            blocks++;
            if (rate == (u32)a[0] && len == (u32)a[1] && Fnv(buf, len) == hash) {
                exact++;
            } else if (first == 0) {
                first = lineNo;
                printf(
                    "gax-trace: block %u (line %u) differs: %u Hz, %u samples, hash %08x; the GBA's %d Hz, %d "
                    "samples, %08x\n",
                    blocks, lineNo, rate, len, Fnv(buf, len), a[0], a[1], hash);
            }
        }
    }
    fclose(f);
    printf("gax-trace %u %u (%u without a block)\n", blocks, exact, unmixed);
    fflush(stdout);
    exit(blocks != 0 && exact == blocks ? 0 : 1);
}
