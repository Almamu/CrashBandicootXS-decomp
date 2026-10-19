/* The pc platform's audio output: what GAX2 mixes (gax_host.h), played
 * as the GBA plays it.
 *
 * On the GBA, GAX_play (from AudioContext::Update, the VCount IRQ at line
 * 0x35) mixes a frame's samples into one half of its 8-bit output buffer,
 * and DMA1 feeds the halves to Direct Sound A's FIFO at the mix rate
 * (Timer0), re-armed by GAX_irq at every other VBlank: the GBA plays the
 * blocks in the order they were mixed, one a frame. Here GaxMixFrame
 * hands each block to HostAudioMixed, and at each VBlank (irq.c, line
 * 160) HostAudioVBlank passes that frame's block on, or silence of the
 * last block's length when nothing was mixed (no song, or paused: GAX2's
 * output is off then):
 * - to the window's audio device (sdl.c), unless headless;
 * - to --wav FILE: the samples as they are, 8-bit mono at the mix rate of
 *   the first block (a song at another rate is resampled to it, nearest
 *   sample); the file starts at the first block;
 * - to a SHA-1 of every block's bytes and rate, which HostAudioEnd prints
 *   with the count (`audio BLOCKS HASH`): the headless runs' check.
 * --gax-trace has no VBlanks: HostAudioWavBlock writes each block GAX_play
 * mixed to --wav's file, as it comes. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gax_host.h"
#include "host_platform.h"

#define MAX_BLOCK 2048

static s8 sBlock[MAX_BLOCK];
static u32 sBlockLen;
static u32 sRate;
static int sMixed;
static int sFresh; /* mixed since HostAudioTakeBlock */
static u32 sBlocks;
static struct HostSha1 sHash;

static FILE *sWav;
static u32 sWavRate;
static u32 sWavBytes;
static u64 sWavPhase; /* for resampling: input samples * sWavRate */

void HostAudioMixed(const s8 *buf, u32 samples, u32 rate)
{
    u8 tag[4];

    if (samples > MAX_BLOCK) {
        samples = MAX_BLOCK;
    }
    memcpy(sBlock, buf, samples);
    sBlockLen = samples;
    sRate = rate;
    sMixed = 1;
    sFresh = 1;
    if (sBlocks++ == 0) {
        HostSha1Init(&sHash);
    }
    tag[0] = rate;
    tag[1] = rate >> 8;
    tag[2] = rate >> 16;
    tag[3] = rate >> 24;
    HostSha1Update(&sHash, tag, sizeof(tag));
    HostSha1Update(&sHash, buf, samples);
}

static void WavHeader(void)
{
    u8 h[44];
    u32 n = sWavBytes;

    memcpy(h, "RIFF", 4);
    h[4] = (36 + n);
    h[5] = (36 + n) >> 8;
    h[6] = (36 + n) >> 16;
    h[7] = (36 + n) >> 24;
    memcpy(h + 8, "WAVEfmt ", 8);
    memcpy(h + 16, "\x10\0\0\0\x01\0\x01\0", 8); /* PCM, mono */
    h[24] = sWavRate;
    h[25] = sWavRate >> 8;
    h[26] = sWavRate >> 16;
    h[27] = sWavRate >> 24;
    h[28] = sWavRate; /* bytes a second */
    h[29] = sWavRate >> 8;
    h[30] = sWavRate >> 16;
    h[31] = sWavRate >> 24;
    memcpy(h + 32, "\x01\0\x08\0", 4); /* 1-byte frames, 8 bits */
    memcpy(h + 36, "data", 4);
    h[40] = n;
    h[41] = n >> 8;
    h[42] = n >> 16;
    h[43] = n >> 24;
    fseek(sWav, 0, SEEK_SET);
    fwrite(h, 1, sizeof(h), sWav);
    fseek(sWav, 0, SEEK_END);
}

/* A block to the WAV file: 8-bit PCM is unsigned. */
static void WavWrite(const s8 *buf, u32 n, u32 rate)
{
    u8 out[MAX_BLOCK * 8];
    u32 m = 0;
    u32 i;

    if (sWav == NULL) {
        sWav = fopen(gHostOptions.wav, "wb");
        if (sWav == NULL) {
            perror(gHostOptions.wav);
            exit(2);
        }
        sWavRate = rate;
        WavHeader();
    }
    if (rate == sWavRate) {
        for (i = 0; i < n; i++) {
            out[m++] = (u8)(buf[i] + 128);
        }
    } else {
        /* nearest sample: output sample k shows input sample k * rate / sWavRate */
        u64 total = (u64)n * sWavRate;

        while (sWavPhase < total && m < sizeof(out)) {
            out[m++] = (u8)(buf[sWavPhase / sWavRate] + 128);
            sWavPhase += rate;
        }
        sWavPhase -= total;
    }
    fwrite(out, 1, m, sWav);
    sWavBytes += m;
}

void HostAudioVBlank(void)
{
    if (!sMixed) {
        if (sBlockLen == 0) {
            return;
        }
        memset(sBlock, 0, sBlockLen);
    }
    sMixed = 0;
    if (gHostOptions.wav != NULL) {
        WavWrite(sBlock, sBlockLen, sRate);
    }
    if (!gHostOptions.headless) {
        HostFrontendAudio(sBlock, sBlockLen, sRate);
    }
}

void HostAudioWavBlock(void)
{
    if (sMixed && gHostOptions.wav != NULL) {
        WavWrite(sBlock, sBlockLen, sRate);
    }
    sMixed = 0;
}

int HostAudioTakeBlock(const s8 **buf, u32 *n, u32 *rate)
{
    if (!sFresh) {
        return 0;
    }
    sFresh = 0;
    *buf = sBlock;
    *n = sBlockLen;
    *rate = sRate;
    return 1;
}

void HostAudioEnd(void)
{
    u8 digest[20];
    u32 i;

    if (sWav != NULL) {
        WavHeader();
        fclose(sWav);
        sWav = NULL;
    }
    if (sBlocks == 0) {
        HostSha1Init(&sHash);
    }
    HostSha1Final(&sHash, digest);
    if (gHostOptions.quiet) {
        return;
    }
    printf("audio %u ", sBlocks);
    for (i = 0; i < 8; i++) {
        printf("%02x", digest[i]);
    }
    printf("\n");
}
