#include "core.h"
#include "audio.h"

/* GAX2's play-start/init entry point (per docs/audio.md): initializes the
 * runtime player-state object at gUnknown_03001630 (magic, songPtr,
 * curChannelIdx/channels/0x24/0x41/0x43 defaults), validates the caller's
 * item count against a 0x18B (395) sanity maximum, fills in default fields
 * (instrument-bank pointer gStaticData_085A4C5C, default volume 0xFF) when
 * the caller left them zero, looks up a starting song-slot index
 * (sub_8037FA0) and computes its fixed-point tempo (sub_803ADB4), copies
 * two whole gStaticData_0803A630/0803A73C/0803A818 initializer blocks into
 * the player state (0x48+/0x9c+, 21 and 56 words), computes the sanity-
 * checked buffer bounds for channel/pattern data, arms the loop/priority
 * defaults, and finally allocates the channel table (sub_8038240) and
 * re-arms the hardware DMA1/SOUNDCNT output path (sub_80384DC, matched
 * above in gax_hw_reset.c) - showing GAX2's fatal-error screen
 * (sub_80392E0) on any sanity-check failure along the way instead of
 * returning normally.
 *
 * Matched in GAX retry 5 (docs/matching/gax-naked-retry-5.md) after four
 * earlier passes (gax-toolchain-retry.md, gax-naked-retry-2/3/4.md). The
 * last pieces were: indexed copies of the constant ARM-code tables (GCSE
 * hoists their addresses to the first block in the ROM's order), a
 * separate counter for the dspFn17c copy, `layout` copied from a
 * block-local read after the first types[] load, and three no-code
 * register nudges (commented at each use). */
asm(".set _call_via_r1, sub_803AD7C\n.set __divsi3, sub_803ADB4\n.set __udivsi3, sub_8037E54\n");

struct RateEntry { u32 rate; u32 timer; };
struct GaxLayoutList { u32 count; struct GaxHandlerLayout *layouts[1]; };
extern struct GaxHandlerLayout gStaticData_085A4C5C;
extern struct RateEntry gStaticData_085A6150[];
extern const u8 *gStaticData_085A614C;
extern const u32 gStaticData_0803A630[];
extern const u32 gStaticData_0803A67C[];
extern const u32 gStaticData_0803A73C[];
extern const u32 gStaticData_0803A818[];
extern const char gStaticData_085A61D0[];
extern const char gStaticData_085A61DC[];
extern void sub_80392E0(const char *a, const char *b);
extern void sub_8037F3C(void *dest, s32 count);
extern s32 sub_8037FA0(u32 rate);
extern s32 sub_8037E54(s32 a, s32 b);
extern void sub_80384DC(void);
extern u8 sub_8038240(struct GaxHandlerLayout *layout, struct GaxHandlerType **sfx, u32 numSfx, u8 **bufp,
                      u32 *sizep);

#define ALIGN4(buf, size)                                  \
    {                                                      \
        u32 pad_ = (((u32)(buf) + 4) & ~3) - (u32)(buf);   \
        (buf) += pad_;                                     \
        (size) -= pad_;                                    \
    }

u32 sub_8038538(struct GaxSongHeader *p)
{
    struct GaxChannelFormat *fmt;
    u32 maxRate = 0;
    u8 *buf = p->workBuf;
    u32 size = p->workSize;
    u32 n;
    u32 i;
    s32 idx;
    s32 k;
    struct GaxHandlerLayout *layout;
    /* no-code hold: the ROM leaves r3 unused while `layout` is live
     * between the first tap scan and its `types[2]` test, so `layout`
     * lands in r4 */
    register u32 hold asm("r3");

    if (size <= 0x18b)
        goto fail;
    gUnknown_03001630 = (struct GaxPlayerState *)buf;
    buf += 0x18c;
    size -= 0x18c;
    if (p->layout == NULL)
        p->layout = &gStaticData_085A4C5C;
    if (p->sfxTypes == NULL)
        p->numSfx = 0;
    if (p->mixRate == 0xffff)
        p->mixRate = p->layout->types[1]->data.song->mixRate;
    if (p->numSfx == 0xffff)
        p->numSfx = p->layout->types[1]->data.song->numSfx;
    if (p->field_10 == 0xffff)
        p->field_10 = 0xff;
    gUnknown_03001630->magic = 0x47415832;
    gUnknown_03001630->songPtr = p;
    gUnknown_03001630->state = 0;
    gUnknown_03001630->curChannelIdx = 0;
    gUnknown_03001630->field_24 = 0;
    gUnknown_03001630->field_41 = 0;
    gUnknown_03001630->field_43 = 1;
    n = p->layout->count;
    if (p->sfxTypes != NULL)
        n += p->numSfx;
    gUnknown_03001630->channels[gUnknown_03001630->curChannelIdx] = buf;
    size = size - n * 4;
    fmt = (struct GaxChannelFormat *)(buf + n * 4);
    buf = (u8 *)fmt + 8;
    size -= 8;
    gUnknown_03001630->format = fmt;
    idx = sub_8037FA0(p->mixRate);
    fmt->field_00 = 8;
    fmt->channels = 1;
    fmt->mixRate = gStaticData_085A6150[idx].rate;
    fmt->frames = fmt->mixRate * 1000 / 0xe94f;
    gUnknown_03001630->field_34 = gStaticData_085A6150[idx].timer;
    gUnknown_03001630->field_40 = 0;
    if (gStaticData_085A614C[2] != 'X' || gStaticData_085A614C[1] != 'A' || gStaticData_085A614C[0] != 'G')
        gUnknown_03001630->field_34 <<= 1;
    if (size < (gUnknown_03001630->format->frames + 4) * 2)
        goto fail;
    gUnknown_03001630->field_1c = (u32)buf;
    buf += (gUnknown_03001630->format->frames + 4) * 2;
    size -= (gUnknown_03001630->format->frames + 4) * 2;
    ALIGN4(buf, size);
    /* no code: an extra reference that lifts the aligned size over the
     * format pointer in global.c's priority order (ROM: r3/r4) */
    asm("" : : "r"(size));
    gUnknown_03001630->field_2c = 0;
    if (size < gUnknown_03001630->format->frames * 2)
        return 0;
    gUnknown_03001630->field_18 = (u32)buf;
    buf += gUnknown_03001630->format->frames * 2;
    size -= gUnknown_03001630->format->frames * 2;
    sub_8037F3C((void *)gUnknown_03001630->field_18, gUnknown_03001630->format->frames * 2);
    ALIGN4(buf, size);
    {
        struct GaxDspTap *tap;
        struct GaxHandlerLayout *l;
        struct GaxHandlerType *t0;

        /* `layout` is copied from a block-local read after the first
         * types[] load, as in the ROM (`adds r4, r0, #0` between them) */
        i = 0;
        l = p->layout;
        t0 = l->types[0];
        asm("" : "=r"(hold));
        layout = l;
        tap = t0->data.dsp->taps;
        for (; i <= 2; i++) {
            if (tap->rate > maxRate)
                maxRate = tap->rate;
            /* no code: an extra reference that lifts `maxRate` over
             * `fmt` in global.c's priority order (ROM: r8/r9) */
            asm("" : : "r"(maxRate));
            tap++;
        }
    }
    asm("" : : "r"(hold));
    if (!(p->flags & 0x10) && layout->types[2] != NULL) {
        struct GaxLayoutList *subs = (struct GaxLayoutList *)layout->types[2];

        {
            s32 j;
            s32 next;

            for (j = 0; j < (s32)subs->count; j = next) {
                struct GaxDspTap *tap;
                struct GaxHandlerLayout *l = *(j + subs->layouts);

                i = 0;
                next = j + 1;
                tap = l->types[0]->data.dsp->taps;
                for (; i <= 2; i++) {
                    if (tap->rate > maxRate)
                        maxRate = tap->rate;
                    tap++;
                }
            }
        }
    }
    if (maxRate != 0) {
        struct GaxPlayerState *g;
        u8 *echo;
        u32 left;
        u32 len;

        ALIGN4(buf, size);
        if (size <= 23)
            goto fail;
        g = gUnknown_03001630;
        g->field_24 = (u32)buf;
        echo = buf + 24;
        buf = echo;
        left = size - 24;
        size = left;
        len = maxRate * fmt->mixRate / 1000 * 2;
        if (left < len)
            goto fail;
        g->field_20 = (u32)echo;
        g->field_28 = len;
        buf = echo + len;
        size = left - len;
        ALIGN4(buf, size);
        sub_8037F3C(echo, len);
    }
    {
    /* Indexed copies of the constant tables: GCSE's PRE hoists the
     * `&gUnknown_03001630`, `p->layout`, 0803A73C and 0803A818 loads to
     * the end of this first block, in the ROM's order, and loop.c
     * strength-reduces each index into the `ldmia` pointer. */
    const u32 *src;

    for (k = 0; k <= 20; k++)
        gUnknown_03001630->dspCode48[k] = gStaticData_0803A630[k];
    src = gStaticData_0803A73C;
    for (k = 0; k <= 55; k++)
        gUnknown_03001630->dspCode9c[k] = src[k];
    src = gStaticData_0803A818;
    {
        s32 words;
        if (p->layout->types[1]->data.song->field_1b != 0 || (u16)(p->flags & 0x20)) {
            gUnknown_03001630->field_42 = 1;
            words = 76;
        } else {
            gUnknown_03001630->field_42 = 0;
            words = 55;
        }
        if (size < words * 4)
            goto fail;
        gUnknown_03001630->field_44 = buf;
        buf += words * 4;
        size -= words * 4;
        for (k = 0; (s32)k < words; k++)
            ((u32 *)gUnknown_03001630->field_44)[k] = src[k];
    }
    }
    if ((u16)(p->flags & 4)) {
        if (size <= 239)
            goto fail;
        gUnknown_03001630->dspFn17c = buf;
        buf += 240;
        size -= 240;
        {
            /* its own counter: sharing `k` makes it conflict with the
             * 380 offset constant and pushes `k` out of r2 above */
            s32 m;

            for (m = 0; m <= 59; m++)
                ((u32 *)gUnknown_03001630->dspFn17c)[m] = gStaticData_0803A67C[m];
        }
    } else {
        gUnknown_03001630->dspFn17c = NULL;
    }
    {
        struct GaxHandlerLayout *l = p->layout;

        gUnknown_03001630->field_180 = 0;
        /* no code: an extra reference that puts the (PRE-hoisted)
         * `p->layout` argument first in global.c's order, so it takes r4
         * and the other arguments sb/r6 as in the ROM */
        asm("" : : "r"(l));
        if (!sub_8038240(l, p->sfxTypes, p->numSfx, &buf, &size))
            goto fail;
    }
    ALIGN4(buf, size);
    GAX_MIXER()->extraChildren = p->numSfx;
    GAX_MIXER()->field_10 = gUnknown_03001630->field_1c;
    GAX_MIXER()->type->init(GAX_MIXER());
    gUnknown_03001630->workBuf = buf;
    gUnknown_03001630->workSize = size;
    sub_80384DC();
    gUnknown_03001630->state = 1;
    GAX_INFO()->field_20 = (p->flags >> 3) & 1;
    GAX_SONG()->field_39 = 0;
    GAX_SONG()->field_3a = 0;
    /* nested, not `&&`: keeps the ROM's u16 test and its two
     * `field_40 = 0` stores sharing the tested zero */
    if ((u16)(p->flags & 2)) {
        if (((u32 *)GAX_MIXER()->type->data.dsp)[1] != 0)
            gUnknown_03001630->field_40 = 1;
        else
            gUnknown_03001630->field_40 = 0;
    } else
        gUnknown_03001630->field_40 = 0;
    return 1;
fail:
    if (p->showErrors)
        sub_80392E0(gStaticData_085A61D0, gStaticData_085A61DC);
    return 0;
}
