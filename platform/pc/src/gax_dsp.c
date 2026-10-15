/* GAX2's ARM DSP routines (lib/gax/asm/gax_arm_dsp.s) in C, for the pc
 * platform's GAX2 (gax_host.h): the resampler, the downmix to 8 bits, the
 * echo and the low-pass filter. Each does what the ARM code does,
 * instruction for instruction where it shows in the output: 32-bit
 * products, arithmetic shifts, the halfword stores' truncation, the
 * clamps' bounds, the loops' entry at their test (or not), and the
 * half-rate loop's word accesses, which the ARM7TDMI makes on the aligned
 * word with an unaligned address (a load rotated, a store aligned down).
 * So the mix comes out as the GBA's, byte for byte. */

#include "gax_host.h"

/* ARM's `mul`: the low 32 bits of the product. */
static s32 Mul(s32 a, s32 b)
{
    return (s32)((u32)a * (u32)b);
}

/* The ARM7TDMI's `ldr` and `str` of a word at any address. */
static u32 LoadWord(uintptr_t addr)
{
    u32 v = *(const u32 *)(addr & ~(uintptr_t)3);
    u32 rot = (addr & 3) * 8;

    return rot ? (v >> rot) | (v << (32 - rot)) : v;
}

static void StoreWord(uintptr_t addr, u32 v)
{
    *(u32 *)(addr & ~(uintptr_t)3) = v;
}

/* gGaxArmResample: mode 0 stores (the first channel mixed), 1 adds, 2
 * adds at half rate (each sample into two outputs, stepping twice as
 * far); positions are Q11. The store and add loops step and test the end
 * forward or backward as GaxChannelMix patched them; the half-rate loop
 * is never patched. Each loop is entered at its test. */
void HostGaxResample(struct HostGaxMixItem *item, int storeBackward, int mixBackward)
{
    s32 volume = item->volume;
    s32 step = item->step;
    u8 *buf = item->buf;
    u8 *outEnd = buf + item->frames * 2;
    u8 *out = buf + item->done * 2;
    s32 pos = item->pos;
    s32 end = item->end;
    s32 loopLen = item->loopLen;
    const s8 *src = (const s8 *)item->src;

    switch (item->mode) {
    case 0:
        for (;;) {
            if (out >= outEnd) {
                break;
            }
            if (storeBackward ? !(pos > end) : !(pos < end)) {
                if (loopLen == 0) {
                    break;
                }
                pos -= loopLen;
            }
            *(s16 *)out = (s16)(Mul(src[pos >> 11], volume) >> 8);
            out += 2;
            pos = storeBackward ? pos - step : pos + step;
        }
        break;
    case 1:
        for (;;) {
            if (out >= outEnd) {
                break;
            }
            if (mixBackward ? !(pos > end) : !(pos < end)) {
                if (loopLen == 0) {
                    break;
                }
                pos -= loopLen;
            }
            *(s16 *)out = (s16)((Mul(src[pos >> 11], volume) >> 8) + *(s16 *)out);
            out += 2;
            pos = mixBackward ? pos - step : pos + step;
        }
        break;
    case 2:
        while (pos < end && out < outEnd) {
            s32 s = Mul(src[pos >> 11], volume) >> 8;
            u32 both = ((u32)s << 16) | ((u32)s & 0xFFFF);

            StoreWord((uintptr_t)out, both + LoadWord((uintptr_t)out));
            out += 4;
            pos += (s32)((u32)step << 1);
        }
        /* overshot: back off one output and one step */
        if (pos > end || out > outEnd) {
            out -= 2;
            pos -= step;
        }
        break;
    }
    item->pos = pos;
    item->done = (u32)((uintptr_t)out - (uintptr_t)buf) >> 1;
}

/* gGaxArmDownmix: dst[i] = (src[i] * gain) >> 10, clamped to a byte. The
 * loop runs at least once. */
void HostGaxDownmix(const s16 *src, s8 *dst, u32 samples, u32 gain)
{
    s8 *end = dst + samples;

    do {
        s32 v = Mul(*src++, gain) >> 10;

        if (v < -128) {
            v = -128;
        }
        if (v > 127) {
            v = 127;
        }
        *dst++ = v;
    } while (dst < end);
}

/* The echo's delay-line position and the filter's two stages, which the
 * ARM routines keep inside their own code (.Lecho_pos, .Lfilter_state):
 * every GAX2_init copies the code again, with zeros there. */
static u32 sEchoPos;
static s32 sFilterStage1;
static s32 sFilterStage2;

void HostGaxDspReset(void)
{
    sEchoPos = 0;
    sFilterStage1 = 0;
    sFilterStage2 = 0;
}

static s32 EchoTap(const u8 *echo, u32 pos, u32 len, u32 delay)
{
    s32 at = (s32)(pos + len - delay);

    if (at >= (s32)len) {
        at -= len;
    }
    return *(const s16 *)(echo + at);
}

/* gGaxArmEcho: up to three taps of the circular delay line `echoBuf`
 * (`echoLen` bytes) mixed into `buf` in place, each output sample written
 * back into the line. taps = three {delay in bytes, gain} pairs; a zero
 * delay ends the list after the first; the third tap's gain is never
 * used (its sample is added >> 4). The loop runs at least once. */
void HostGaxEcho(u32 bytes, s16 *buf, s16 *echoBuf, u32 echoLen, const u32 *taps)
{
    const u8 *end = (const u8 *)buf + bytes;
    u8 *echo = (u8 *)echoBuf;
    u32 pos = sEchoPos;

    do {
        s32 acc;
        s32 v;

        if (pos == echoLen) {
            pos = 0;
        }
        acc = Mul(EchoTap(echo, pos, echoLen, taps[0]), taps[1]) >> 4;
        if (taps[2] != 0) {
            acc += Mul(EchoTap(echo, pos, echoLen, taps[2]), taps[3]) >> 4;
            if (taps[4] != 0) {
                acc += EchoTap(echo, pos, echoLen, taps[4]) >> 4;
            }
        }
        v = *buf + acc;
        *buf = (s16)v;
        *(s16 *)(echo + pos) = (s16)v;
        buf++;
        pos += 2;
    } while ((const u8 *)buf < end);
    sEchoPos = pos;
}

/* The filter's clamp: below -limit it is ~limit (`mvn`), one less. */
static s32 FilterClamp(s32 v, s32 limit)
{
    if ((s64)v + limit < 0) {
        v = ~limit;
    }
    if (v > limit) {
        v = limit;
    }
    return v;
}

/* gGaxArmFilter: two cascaded one-pole low-pass stages over `buf` in
 * place, coefficient `amount * 0x334 >> 8`, every stage clamped to
 * +-(0x7F00 * channels). Runs at least once. (The game never turns the
 * filter on: GaxSongHeader.flags bit 2.) */
void HostGaxFilter(s16 *buf, u32 bytes, u32 amount, u32 channels)
{
    s32 coef = Mul(amount, 0x334) >> 8;
    s32 limit = Mul(channels, 0x7F00);
    s32 left = bytes;
    s32 s1 = sFilterStage1;
    s32 s2 = sFilterStage2;

    do {
        s32 v = FilterClamp((s32)((u32)*buf << 8) - s1 - s2, limit);

        s1 = FilterClamp(s1 + (Mul(v, coef) >> 8), limit);
        s2 = FilterClamp(s2 + (Mul(s1, coef) >> 8), limit);
        *buf++ = (s16)(s2 >> 8);
        left -= 2;
    } while (left > 0);
    sFilterStage1 = s1;
    sFilterStage2 = s2;
}
