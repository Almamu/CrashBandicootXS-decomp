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
 * returning normally. Object shape not confidently modeled - kept as raw
 * offsets throughout, same as the other GAX2_SoundHandler functions in
 * this cluster.
 *
 * Written as NAKED asm, not plain C: this function's prologue
 * (`push {r4-r7,lr}; mov r7,sl; mov r6,sb; mov r5,r8; push {r5,r6,r7}`)
 * keeps r8/sb/sl live as genuine scratch across the whole function (a
 * running priority-maximum accumulator in r8, spanning two nested voice-
 * scan loops; sb/sl holding intermediate bank/table pointers across
 * several calls) - the same many-register gcc-2.9 allocation ceiling
 * already documented throughout this ROM region for sub_8006600/
 * sub_80372BC and this cluster's other functions (docs/status/audio.md),
 * which a NAKED function sidesteps entirely since nothing asks gcc's
 * allocator to decide anything. Mechanical, byte-verified transcription of
 * the ROM's own instructions (translated from the disassembler's unified
 * syntax to this project's established NAKED plain/divided syntax, local
 * labels renumbered per docs/matching/issue-4-sio-settings-sync.md's
 * convention), not an inferred control-flow guess. */
/* Later pass (docs/matching/gax-toolchain-retry.md): the "r8/sb/sl
 * ceiling" note above is not the real obstacle - a plain draft against
 * the structs in include/audio.h (below) reproduces the ROM's control
 * flow, buffer carving, literal pool and most instruction selection
 * (the tap-table scans need a walking `tap` pointer, the ARM-code copies
 * pre-loaded source pointers). What's left is register choice: agbcc
 * gives the format pointer r8 and `maxRate` r9 where the ROM has them
 * the other way round, which cascades through the rest of the function.
 * Still NAKED.
 * GAX NAKED retry 2 (docs/matching/gax-naked-retry-2.md): draft now
 * ~294 halfwords off by alignment-insensitive count (was ~364): `/` for
 * the echo length (sub_8037E54 is `__udivsi3`), the tap/alternative-
 * layout scans in sub_8037FC0's matched shape, an s32 copy counter, and
 * a no-code `maxRate` reference that gives it r8 and `fmt` r9 like the
 * ROM. Left: register choice in the ALIGN4 after `field_1c` (new size in
 * r3), the first tap scan's `layout` copy, the order of the constants
 * hoisted before the ARM-code copy loops, and the tail. */
#if NON_MATCHING
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

        i = 0;
        layout = p->layout;
        tap = layout->types[0]->data.dsp->taps;
        for (; i <= 2; i++) {
            if (tap->rate > maxRate)
                maxRate = tap->rate;
            /* no code: an extra reference that lifts `maxRate` over
             * `fmt` in global.c's priority order (ROM: r8/r9) */
            asm("" : : "r"(maxRate));
            tap++;
        }
    }
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
    struct GaxHandlerLayout *layout;
    const u32 *a73c;
    const u32 *a818;
    const u32 *src;

    k = 0;
    layout = p->layout;
    a73c = gStaticData_0803A73C;
    a818 = gStaticData_0803A818;
    src = gStaticData_0803A630;
    for (; k <= 20; k++)
        gUnknown_03001630->dspCode48[k] = *src++;
    src = a73c;
    for (k = 0; k <= 55; k++)
        gUnknown_03001630->dspCode9c[k] = *src++;
    {
        s32 words;
        if (layout->types[1]->data.song->field_1b != 0 || (u16)(p->flags & 0x20)) {
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
        src = a818;
        for (k = 0; (s32)k < words; k++)
            ((u32 *)gUnknown_03001630->field_44)[k] = *src++;
    }
    }
    if ((u16)(p->flags & 4)) {
        if (size <= 239)
            goto fail;
        gUnknown_03001630->dspFn17c = buf;
        buf += 240;
        size -= 240;
        for (k = 0; k <= 59; k++)
            ((u32 *)gUnknown_03001630->dspFn17c)[k] = gStaticData_0803A67C[k];
    } else {
        gUnknown_03001630->dspFn17c = NULL;
    }
    gUnknown_03001630->field_180 = 0;
    if (!sub_8038240(p->layout, p->sfxTypes, p->numSfx, &buf, &size))
        goto fail;
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
    if ((p->flags & 2) && ((u32 *)GAX_MIXER()->type->data.dsp)[1] != 0)
        gUnknown_03001630->field_40 = 1;
    else
        gUnknown_03001630->field_40 = 0;
    return 1;
fail:
    if (p->showErrors)
        sub_80392E0(gStaticData_085A61D0, gStaticData_085A61DC);
    return 0;
}
#else /* !NON_MATCHING */
NAKED u32 sub_8038538(void *gaxState)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0xc\n\t"
        "add r7, r0, #0\n\t"
        "mov r0, #0\n\t"
        "mov r8, r0\n\t"
        "ldr r1, [r7]\n\t"
        "str r1, [sp, #4]\n\t"
        "ldr r2, [r7, #4]\n\t"
        "str r2, [sp, #8]\n\t"
        "ldr r0, L8538_39\n\t"
        "cmp r2, r0\n\t"
        "bhi L8538_0\n\t"
        "b L8538_36\n\t"
        "L8538_0:\n\t"
        "ldr r3, L8538_40\n\t"
        "mov sl, r3\n\t"
        "str r1, [r3]\n\t"
        "mov r4, #0xc6\n\t"
        "lsl r4, r4, #1\n\t"
        "add r0, r1, r4\n\t"
        "str r0, [sp, #4]\n\t"
        "ldr r1, L8538_41\n\t"
        "add r0, r2, r1\n\t"
        "str r0, [sp, #8]\n\t"
        "ldr r0, [r7, #0x30]\n\t"
        "cmp r0, #0\n\t"
        "bne L8538_1\n\t"
        "ldr r0, L8538_42\n\t"
        "str r0, [r7, #0x30]\n\t"
        "L8538_1:\n\t"
        "ldr r0, [r7, #0x2c]\n\t"
        "cmp r0, #0\n\t"
        "bne L8538_2\n\t"
        "mov r2, r8\n\t"
        "strh r2, [r7, #0xe]\n\t"
        "L8538_2:\n\t"
        "ldrh r0, [r7, #8]\n\t"
        "ldr r1, L8538_43\n\t"
        "cmp r0, r1\n\t"
        "bne L8538_3\n\t"
        "ldr r0, [r7, #0x30]\n\t"
        "ldr r0, [r0, #8]\n\t"
        "ldr r0, [r0, #0x18]\n\t"
        "ldrh r0, [r0, #0x18]\n\t"
        "strh r0, [r7, #8]\n\t"
        "L8538_3:\n\t"
        "ldrh r0, [r7, #0xe]\n\t"
        "cmp r0, r1\n\t"
        "bne L8538_4\n\t"
        "ldr r0, [r7, #0x30]\n\t"
        "ldr r0, [r0, #8]\n\t"
        "ldr r0, [r0, #0x18]\n\t"
        "ldrb r0, [r0, #0x1a]\n\t"
        "strh r0, [r7, #0xe]\n\t"
        "L8538_4:\n\t"
        "ldrh r0, [r7, #0x10]\n\t"
        "cmp r0, r1\n\t"
        "bne L8538_5\n\t"
        "mov r0, #0xff\n\t"
        "strh r0, [r7, #0x10]\n\t"
        "L8538_5:\n\t"
        "mov r3, sl\n\t"
        "ldr r0, [r3]\n\t"
        "ldr r1, L8538_44\n\t"
        "str r1, [r0]\n\t"
        "str r7, [r0, #4]\n\t"
        "mov r4, r8\n\t"
        "str r4, [r0, #0x30]\n\t"
        "str r4, [r0, #0x10]\n\t"
        "str r4, [r0, #0x24]\n\t"
        "add r0, #0x41\n\t"
        "strb r4, [r0]\n\t"
        "ldr r0, [r3]\n\t"
        "add r0, #0x43\n\t"
        "mov r5, #1\n\t"
        "strb r5, [r0]\n\t"
        "ldr r0, [r7, #0x30]\n\t"
        "ldr r4, [r0]\n\t"
        "ldr r0, [r7, #0x2c]\n\t"
        "cmp r0, #0\n\t"
        "beq L8538_6\n\t"
        "ldrh r0, [r7, #0xe]\n\t"
        "add r4, r4, r0\n\t"
        "L8538_6:\n\t"
        "mov r0, sl\n\t"
        "ldr r3, [r0]\n\t"
        "ldr r0, [r3, #0x10]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r1, r3, #0\n\t"
        "add r1, #8\n\t"
        "add r1, r1, r0\n\t"
        "ldr r2, [sp, #4]\n\t"
        "str r2, [r1]\n\t"
        "lsl r1, r4, #2\n\t"
        "ldr r0, [sp, #8]\n\t"
        "sub r0, r0, r1\n\t"
        "add r2, r2, r1\n\t"
        "mov sb, r2\n\t"
        "mov r1, sb\n\t"
        "add r1, #8\n\t"
        "str r1, [sp, #4]\n\t"
        "sub r0, #8\n\t"
        "str r0, [sp, #8]\n\t"
        "mov r1, sb\n\t"
        "str r1, [r3, #0x14]\n\t"
        "ldrh r0, [r7, #8]\n\t"
        "bl sub_8037FA0\n\t"
        "add r4, r0, #0\n\t"
        "mov r0, #8\n\t"
        "mov r2, sb\n\t"
        "strb r0, [r2]\n\t"
        "strb r5, [r2, #1]\n\t"
        "ldr r5, L8538_45\n\t"
        "lsl r4, r4, #3\n\t"
        "add r0, r4, r5\n\t"
        "ldr r0, [r0]\n\t"
        "mov r6, #0\n\t"
        "strh r0, [r2, #2]\n\t"
        "ldrh r1, [r2, #2]\n\t"
        "lsl r0, r1, #5\n\t"
        "sub r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #3\n\t"
        "ldr r1, L8538_46\n\t"
        "bl sub_803ADB4\n\t"
        "mov r3, sb\n\t"
        "strh r0, [r3, #4]\n\t"
        "mov r0, sl\n\t"
        "ldr r1, [r0]\n\t"
        "add r5, #4\n\t"
        "add r4, r4, r5\n\t"
        "ldr r0, [r4]\n\t"
        "str r0, [r1, #0x34]\n\t"
        "add r1, #0x40\n\t"
        "strb r6, [r1]\n\t"
        "ldr r0, L8538_47\n\t"
        "ldr r1, [r0]\n\t"
        "ldrb r0, [r1, #2]\n\t"
        "cmp r0, #0x58\n\t"
        "bne L8538_7\n\t"
        "ldrb r0, [r1, #1]\n\t"
        "cmp r0, #0x41\n\t"
        "bne L8538_7\n\t"
        "ldrb r0, [r1]\n\t"
        "cmp r0, #0x47\n\t"
        "beq L8538_8\n\t"
        "L8538_7:\n\t"
        "ldr r2, L8538_40\n\t"
        "ldr r1, [r2]\n\t"
        "ldr r0, [r1, #0x34]\n\t"
        "lsl r0, r0, #1\n\t"
        "str r0, [r1, #0x34]\n\t"
        "mov sl, r2\n\t"
        "L8538_8:\n\t"
        "mov r2, sl\n\t"
        "ldr r1, [r2]\n\t"
        "ldr r3, [r1, #0x14]\n\t"
        "ldrh r0, [r3, #4]\n\t"
        "add r0, #4\n\t"
        "lsl r0, r0, #1\n\t"
        "ldr r4, [sp, #8]\n\t"
        "cmp r4, r0\n\t"
        "bhs L8538_9\n\t"
        "b L8538_36\n\t"
        "L8538_9:\n\t"
        "ldr r2, [sp, #4]\n\t"
        "str r2, [r1, #0x1c]\n\t"
        "ldrh r0, [r3, #4]\n\t"
        "add r0, #4\n\t"
        "lsl r0, r0, #1\n\t"
        "add r2, r2, r0\n\t"
        "str r2, [sp, #4]\n\t"
        "mov r3, sl\n\t"
        "ldr r0, [r3]\n\t"
        "ldr r0, [r0, #0x14]\n\t"
        "ldrh r0, [r0, #4]\n\t"
        "add r0, #4\n\t"
        "lsl r0, r0, #1\n\t"
        "sub r0, r4, r0\n\t"
        "add r1, r2, #4\n\t"
        "mov r5, #4\n\t"
        "neg r5, r5\n\t"
        "and r1, r5\n\t"
        "sub r1, r1, r2\n\t"
        "add r2, r2, r1\n\t"
        "str r2, [sp, #4]\n\t"
        "sub r3, r0, r1\n\t"
        "str r3, [sp, #8]\n\t"
        "mov r4, sl\n\t"
        "ldr r1, [r4]\n\t"
        "mov r0, #0\n\t"
        "str r0, [r1, #0x2c]\n\t"
        "ldr r4, [r1, #0x14]\n\t"
        "ldrh r0, [r4, #4]\n\t"
        "lsl r0, r0, #1\n\t"
        "cmp r3, r0\n\t"
        "bhs L8538_10\n\t"
        "b L8538_37\n\t"
        "L8538_10:\n\t"
        "str r2, [r1, #0x18]\n\t"
        "ldrh r0, [r4, #4]\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r2, r0\n\t"
        "str r0, [sp, #4]\n\t"
        "mov r1, sl\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r0, [r0, #0x14]\n\t"
        "ldrh r0, [r0, #4]\n\t"
        "lsl r0, r0, #1\n\t"
        "sub r0, r3, r0\n\t"
        "str r0, [sp, #8]\n\t"
        "ldr r1, [r1]\n\t"
        "ldr r0, [r1, #0x18]\n\t"
        "ldr r1, [r1, #0x14]\n\t"
        "ldrh r1, [r1, #4]\n\t"
        "lsl r1, r1, #1\n\t"
        "bl sub_8037F3C\n\t"
        "ldr r1, [sp, #4]\n\t"
        "add r0, r1, #4\n\t"
        "and r0, r5\n\t"
        "sub r1, r0, r1\n\t"
        "str r0, [sp, #4]\n\t"
        "ldr r0, [sp, #8]\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [sp, #8]\n\t"
        "mov r2, #0\n\t"
        "ldr r0, [r7, #0x30]\n\t"
        "ldr r1, [r0, #4]\n\t"
        "add r4, r0, #0\n\t"
        "ldr r1, [r1, #0x18]\n\t"
        "L8538_11:\n\t"
        "ldr r0, [r1, #4]\n\t"
        "cmp r0, r8\n\t"
        "bls L8538_12\n\t"
        "mov r8, r0\n\t"
        "L8538_12:\n\t"
        "add r1, #8\n\t"
        "add r2, #1\n\t"
        "cmp r2, #2\n\t"
        "bls L8538_11\n\t"
        "ldrh r1, [r7, #0xc]\n\t"
        "mov r0, #0x10\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "bne L8538_16\n\t"
        "ldr r0, [r4, #0xc]\n\t"
        "cmp r0, #0\n\t"
        "beq L8538_16\n\t"
        "add r4, r0, #0\n\t"
        "mov r1, #0\n\t"
        "ldr r0, [r4]\n\t"
        "cmp r1, r0\n\t"
        "bge L8538_16\n\t"
        "add r5, r0, #0\n\t"
        "L8538_13:\n\t"
        "lsl r0, r1, #2\n\t"
        "add r0, r0, r4\n\t"
        "ldr r0, [r0, #4]\n\t"
        "mov r2, #0\n\t"
        "add r3, r1, #1\n\t"
        "ldr r0, [r0, #4]\n\t"
        "ldr r1, [r0, #0x18]\n\t"
        "L8538_14:\n\t"
        "ldr r0, [r1, #4]\n\t"
        "cmp r0, r8\n\t"
        "bls L8538_15\n\t"
        "mov r8, r0\n\t"
        "L8538_15:\n\t"
        "add r1, #8\n\t"
        "add r2, #1\n\t"
        "cmp r2, #2\n\t"
        "bls L8538_14\n\t"
        "add r1, r3, #0\n\t"
        "cmp r1, r5\n\t"
        "blt L8538_13\n\t"
        "L8538_16:\n\t"
        "mov r2, r8\n\t"
        "cmp r2, #0\n\t"
        "beq L8538_19\n\t"
        "ldr r0, [sp, #4]\n\t"
        "add r1, r0, #4\n\t"
        "mov r3, #4\n\t"
        "neg r3, r3\n\t"
        "mov sl, r3\n\t"
        "and r1, r3\n\t"
        "sub r1, r1, r0\n\t"
        "add r3, r0, r1\n\t"
        "str r3, [sp, #4]\n\t"
        "ldr r0, [sp, #8]\n\t"
        "sub r2, r0, r1\n\t"
        "str r2, [sp, #8]\n\t"
        "cmp r2, #0x17\n\t"
        "bhi L8538_17\n\t"
        "b L8538_36\n\t"
        "L8538_17:\n\t"
        "ldr r0, L8538_40\n\t"
        "ldr r5, [r0]\n\t"
        "str r3, [r5, #0x24]\n\t"
        "add r6, r3, #0\n\t"
        "add r6, #0x18\n\t"
        "str r6, [sp, #4]\n\t"
        "add r4, r2, #0\n\t"
        "sub r4, #0x18\n\t"
        "str r4, [sp, #8]\n\t"
        "mov r1, sb\n\t"
        "ldrh r0, [r1, #2]\n\t"
        "mov r2, r8\n\t"
        "mul r2, r0, r2\n\t"
        "add r0, r2, #0\n\t"
        "mov r1, #0xfa\n\t"
        "lsl r1, r1, #2\n\t"
        "bl sub_8037E54\n\t"
        "lsl r3, r0, #1\n\t"
        "cmp r4, r3\n\t"
        "bhs L8538_18\n\t"
        "b L8538_36\n\t"
        "L8538_18:\n\t"
        "str r6, [r5, #0x20]\n\t"
        "str r3, [r5, #0x28]\n\t"
        "add r0, r6, r3\n\t"
        "sub r2, r4, r3\n\t"
        "add r1, r0, #4\n\t"
        "mov r4, sl\n\t"
        "and r1, r4\n\t"
        "sub r0, r1, r0\n\t"
        "str r1, [sp, #4]\n\t"
        "sub r2, r2, r0\n\t"
        "str r2, [sp, #8]\n\t"
        "add r0, r6, #0\n\t"
        "add r1, r3, #0\n\t"
        "bl sub_8037F3C\n\t"
        "L8538_19:\n\t"
        "mov r2, #0\n\t"
        "ldr r0, L8538_40\n\t"
        "mov sl, r0\n\t"
        "ldr r4, [r7, #0x30]\n\t"
        "ldr r6, L8538_48\n\t"
        "ldr r1, L8538_49\n\t"
        "mov r8, r1\n\t"
        "mov r3, sl\n\t"
        "ldr r5, L8538_50\n\t"
        "L8538_20:\n\t"
        "ldr r0, [r3]\n\t"
        "lsl r1, r2, #2\n\t"
        "add r0, #0x48\n\t"
        "add r0, r0, r1\n\t"
        "ldm r5!, {r1}\n\t"
        "str r1, [r0]\n\t"
        "add r2, #1\n\t"
        "cmp r2, #0x14\n\t"
        "ble L8538_20\n\t"
        "mov r2, #0\n\t"
        "ldr r3, L8538_40\n\t"
        "add r5, r6, #0\n\t"
        "L8538_21:\n\t"
        "ldr r0, [r3]\n\t"
        "lsl r1, r2, #2\n\t"
        "add r0, #0x9c\n\t"
        "add r0, r0, r1\n\t"
        "ldm r5!, {r1}\n\t"
        "str r1, [r0]\n\t"
        "add r2, #1\n\t"
        "cmp r2, #0x37\n\t"
        "ble L8538_21\n\t"
        "mov r6, r8\n\t"
        "ldr r0, [r4, #8]\n\t"
        "ldr r0, [r0, #0x18]\n\t"
        "ldrb r0, [r0, #0x1b]\n\t"
        "cmp r0, #0\n\t"
        "bne L8538_22\n\t"
        "ldrh r1, [r7, #0xc]\n\t"
        "mov r0, #0x20\n\t"
        "and r0, r1\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r1, r0, #0x10\n\t"
        "cmp r1, #0\n\t"
        "beq L8538_23\n\t"
        "L8538_22:\n\t"
        "mov r2, sl\n\t"
        "ldr r0, [r2]\n\t"
        "add r0, #0x42\n\t"
        "mov r1, #1\n\t"
        "strb r1, [r0]\n\t"
        "mov r4, #0x4c\n\t"
        "b L8538_24\n\t"
        ".align 2, 0\n\t"
        "L8538_39: .4byte 0x0000018B\n\t"
        "L8538_40: .4byte gUnknown_03001630\n\t"
        "L8538_41: .4byte 0xFFFFFE74\n\t"
        "L8538_42: .4byte gStaticData_085A4C5C\n\t"
        "L8538_43: .4byte 0x0000FFFF\n\t"
        "L8538_44: .4byte 0x47415832\n\t"
        "L8538_45: .4byte gStaticData_085A6150\n\t"
        "L8538_46: .4byte 0x0000E94F\n\t"
        "L8538_47: .4byte gStaticData_085A614C\n\t"
        "L8538_48: .4byte gStaticData_0803A73C\n\t"
        "L8538_49: .4byte gStaticData_0803A818\n\t"
        "L8538_50: .4byte gStaticData_0803A630\n\t"
        "L8538_23:\n\t"
        "mov r3, sl\n\t"
        "ldr r0, [r3]\n\t"
        "add r0, #0x42\n\t"
        "strb r1, [r0]\n\t"
        "mov r4, #0x37\n\t"
        "L8538_24:\n\t"
        "lsl r2, r4, #2\n\t"
        "ldr r3, [sp, #8]\n\t"
        "cmp r3, r2\n\t"
        "bhs L8538_25\n\t"
        "b L8538_36\n\t"
        "L8538_25:\n\t"
        "mov r1, sl\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r1, [sp, #4]\n\t"
        "str r1, [r0, #0x44]\n\t"
        "add r1, r1, r2\n\t"
        "str r1, [sp, #4]\n\t"
        "sub r0, r3, r2\n\t"
        "str r0, [sp, #8]\n\t"
        "mov r2, #0\n\t"
        "cmp r2, r4\n\t"
        "bge L8538_27\n\t"
        "mov r5, sl\n\t"
        "add r3, r6, #0\n\t"
        "L8538_26:\n\t"
        "ldr r0, [r5]\n\t"
        "ldr r1, [r0, #0x44]\n\t"
        "lsl r0, r2, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldm r3!, {r1}\n\t"
        "str r1, [r0]\n\t"
        "add r2, #1\n\t"
        "cmp r2, r4\n\t"
        "blt L8538_26\n\t"
        "L8538_27:\n\t"
        "ldrh r1, [r7, #0xc]\n\t"
        "mov r0, #4\n\t"
        "and r0, r1\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r1, r0, #0x10\n\t"
        "cmp r1, #0\n\t"
        "beq L8538_30\n\t"
        "ldr r3, [sp, #8]\n\t"
        "cmp r3, #0xef\n\t"
        "bhi L8538_28\n\t"
        "b L8538_36\n\t"
        "L8538_28:\n\t"
        "mov r2, sl\n\t"
        "ldr r1, [r2]\n\t"
        "mov r2, #0xbe\n\t"
        "lsl r2, r2, #1\n\t"
        "add r1, r1, r2\n\t"
        "ldr r0, [sp, #4]\n\t"
        "str r0, [r1]\n\t"
        "add r0, #0xf0\n\t"
        "str r0, [sp, #4]\n\t"
        "add r0, r3, #0\n\t"
        "sub r0, #0xf0\n\t"
        "str r0, [sp, #8]\n\t"
        "mov r3, #0\n\t"
        "ldr r4, [r7, #0x30]\n\t"
        "ldr r0, [r7, #0x2c]\n\t"
        "mov sb, r0\n\t"
        "add r6, sp, #8\n\t"
        "mov r8, sl\n\t"
        "ldr r5, L8538_51\n\t"
        "L8538_29:\n\t"
        "mov r1, r8\n\t"
        "ldr r0, [r1]\n\t"
        "add r0, r0, r2\n\t"
        "ldr r1, [r0]\n\t"
        "lsl r0, r3, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldm r5!, {r1}\n\t"
        "str r1, [r0]\n\t"
        "add r3, #1\n\t"
        "cmp r3, #0x3b\n\t"
        "ble L8538_29\n\t"
        "b L8538_31\n\t"
        ".align 2, 0\n\t"
        "L8538_51: .4byte gStaticData_0803A67C\n\t"
        "L8538_30:\n\t"
        "mov r2, sl\n\t"
        "ldr r0, [r2]\n\t"
        "mov r3, #0xbe\n\t"
        "lsl r3, r3, #1\n\t"
        "add r0, r0, r3\n\t"
        "str r1, [r0]\n\t"
        "ldr r4, [r7, #0x30]\n\t"
        "ldr r0, [r7, #0x2c]\n\t"
        "mov sb, r0\n\t"
        "add r6, sp, #8\n\t"
        "L8538_31:\n\t"
        "mov r5, sl\n\t"
        "ldr r0, [r5]\n\t"
        "mov r1, #0xc0\n\t"
        "lsl r1, r1, #1\n\t"
        "add r0, r0, r1\n\t"
        "mov r2, #0\n\t"
        "mov r8, r2\n\t"
        "str r2, [r0]\n\t"
        "ldrh r2, [r7, #0xe]\n\t"
        "str r6, [sp]\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, sb\n\t"
        "add r3, sp, #4\n\t"
        "bl sub_8038240\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq L8538_36\n\t"
        "ldr r2, [sp, #4]\n\t"
        "add r0, r2, #4\n\t"
        "mov r1, #4\n\t"
        "neg r1, r1\n\t"
        "and r0, r1\n\t"
        "sub r2, r0, r2\n\t"
        "str r0, [sp, #4]\n\t"
        "ldr r0, [sp, #8]\n\t"
        "sub r0, r0, r2\n\t"
        "str r0, [sp, #8]\n\t"
        "ldr r2, [r5]\n\t"
        "ldr r0, [r2, #0x10]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r3, r2, #0\n\t"
        "add r3, #8\n\t"
        "add r0, r3, r0\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0]\n\t"
        "ldrh r0, [r7, #0xe]\n\t"
        "str r0, [r1, #0x14]\n\t"
        "ldr r0, [r2, #0x10]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r3, r0\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r0, [r2, #0x1c]\n\t"
        "str r0, [r1, #0x10]\n\t"
        "ldr r0, [r2, #0x10]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r3, r3, r0\n\t"
        "ldr r0, [r3]\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r1, [r1]\n\t"
        "bl sub_803AD7C\n\t"
        "ldr r2, [r5]\n\t"
        "mov r3, #0xc2\n\t"
        "lsl r3, r3, #1\n\t"
        "add r1, r2, r3\n\t"
        "ldr r0, [sp, #4]\n\t"
        "str r0, [r1]\n\t"
        "mov r4, #0xc4\n\t"
        "lsl r4, r4, #1\n\t"
        "add r1, r2, r4\n\t"
        "ldr r0, [sp, #8]\n\t"
        "str r0, [r1]\n\t"
        "bl sub_80384DC\n\t"
        "ldr r0, [r5]\n\t"
        "mov r3, #1\n\t"
        "str r3, [r0, #0x30]\n\t"
        "ldr r1, [r0, #0x10]\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, #8\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0, #4]\n\t"
        "ldrh r0, [r7, #0xc]\n\t"
        "lsr r0, r0, #3\n\t"
        "and r0, r3\n\t"
        "add r1, #0x20\n\t"
        "strb r0, [r1]\n\t"
        "ldr r0, [r5]\n\t"
        "ldr r0, [r0, #4]\n\t"
        "add r0, #0x39\n\t"
        "mov r1, r8\n\t"
        "strb r1, [r0]\n\t"
        "ldr r0, [r5]\n\t"
        "ldr r0, [r0, #4]\n\t"
        "add r0, #0x3a\n\t"
        "strb r1, [r0]\n\t"
        "ldrh r1, [r7, #0xc]\n\t"
        "mov r0, #2\n\t"
        "and r0, r1\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r1, r0, #0x10\n\t"
        "cmp r1, #0\n\t"
        "beq L8538_33\n\t"
        "ldr r2, [r5]\n\t"
        "ldr r1, [r2, #0x10]\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, r2, #0\n\t"
        "add r0, #8\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0, #0x18]\n\t"
        "ldr r1, [r0, #4]\n\t"
        "cmp r1, #0\n\t"
        "beq L8538_32\n\t"
        "add r0, r2, #0\n\t"
        "add r0, #0x40\n\t"
        "strb r3, [r0]\n\t"
        "b L8538_35\n\t"
        "L8538_32:\n\t"
        "add r0, r2, #0\n\t"
        "b L8538_34\n\t"
        "L8538_33:\n\t"
        "mov r2, sl\n\t"
        "ldr r0, [r2]\n\t"
        "L8538_34:\n\t"
        "add r0, #0x40\n\t"
        "strb r1, [r0]\n\t"
        "L8538_35:\n\t"
        "mov r0, #1\n\t"
        "b L8538_38\n\t"
        "L8538_36:\n\t"
        "add r0, r7, #0\n\t"
        "add r0, #0x38\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq L8538_37\n\t"
        "ldr r0, L8538_52\n\t"
        "ldr r1, L8538_53\n\t"
        "bl sub_80392E0\n\t"
        "L8538_37:\n\t"
        "mov r0, #0\n\t"
        "L8538_38:\n\t"
        "add sp, #0xc\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n\t"
        "L8538_52: .4byte gStaticData_085A61D0\n\t"
        "L8538_53: .4byte gStaticData_085A61DC\n\t"
    );
}
#endif /* NON_MATCHING */
