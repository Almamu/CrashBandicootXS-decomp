#include "core.h"
#include "audio.h"

/* GAX2's work-RAM size estimator (GitHub issue #66's range, formerly the
 * raw asm/code_3_2_20c.s): computes how many bytes `sub_8038538`
 * (gax_playstart.c) will carve out of the caller's work buffer for song
 * header `p` and stores the requirement in `p->workSize`. It resolves the
 * same defaults `sub_8038538` does (default handler layout, the song's
 * default mix rate and SFX-voice count, no SFX voices without SFX types),
 * then adds up the player state (0x18c, +0xf0 with flags bit 2), the
 * handler array and format, every layout handler's header, instance and
 * children (`sub_8038240`'s carving, skipping slot 2, which holds the
 * list of alternative layouts), the largest alternative layout (unless
 * flags bit 4), 0x58 per SFX voice, the echo buffer for the highest DSP
 * tap rate across the layouts (`rate * mixRate / 1000` samples), the
 * mixer code (0x130 or 0xdc bytes) and two frame-sized mix buffers
 * (`mixRate * 1000 / 59727` samples each).
 *
 * NAKED transcription for now. The draft below (old_agbcc; agbcc is
 * further off) has the ROM's control flow and instruction selection for
 * most blocks, but ~240 halfwords still differ, all allocation: the ROM
 * keeps `flags` as a halfword stack slot (`strh`/`ldrh [sp, #0x18]`),
 * orders the stack slots rate/maxRate/max/layout differently, doesn't
 * strength-reduce the first carving loop (re-reading `layout->count`
 * each iteration, which a `goto` loop reproduces but then loses `size`'s
 * r7), and re-derives `layout->types[0]` for the tap scan where the
 * draft hoists it. See docs/matching/late-rom-naked-retry.md. */
struct RateEntry {
    u32 rate;
    u32 timer;
};

/* `layout->types[2]`'s slot holds a list of alternative layouts. */
struct GaxLayoutList {
    u32 count;
    struct GaxHandlerLayout *layouts[1];
};

extern struct GaxHandlerLayout gStaticData_085A4C5C;
extern struct RateEntry gStaticData_085A6150[];
extern s32 sub_8037FA0(u32 rate);
extern s32 sub_8037E54(s32 value, s32 divisor);

#if NON_MATCHING
void sub_8037FC0(struct GaxSongHeader *p)
{
    u32 size = 0;
    u32 maxRate = 0;
    u32 mixRate;
    u32 numSfx;
    u32 rate;
    u16 flags;
    struct GaxHandlerLayout *layout;
    u32 i;

    if (p->layout == NULL)
        p->layout = &gStaticData_085A4C5C;
    if (p->mixRate == 0xffff)
        mixRate = p->layout->types[1]->data.song->mixRate;
    else
        mixRate = p->mixRate;
    if (p->numSfx == 0xffff)
        numSfx = p->layout->types[1]->data.song->numSfx;
    else
        numSfx = p->numSfx;
    if (p->sfxTypes == NULL)
        p->numSfx = numSfx = 0;
    rate = gStaticData_085A6150[sub_8037FA0(mixRate)].rate;
    flags = p->flags;
    if (flags & 4)
        size += 0xf0;
    size += 0x18c;
    layout = p->layout;
    size += (layout->count + numSfx) * 4;
    size += 8;
    for (i = 0; i < layout->count; i++) {
        struct GaxHandlerType *t = layout->types[i];

        if (i != 2) {
            size = size + 0xc + t->instanceSize + t->childCount * 4;
            if (i == 0)
                size += numSfx * 4;
        }
    }
    if (!(flags & 0x10)) {
        struct GaxLayoutList *list = (struct GaxLayoutList *)layout->types[2];

        if (list != NULL) {
            s32 max = 0;
            s32 k;

            for (k = 0; k < (s32)list->count; k++) {
                struct GaxHandlerLayout *l = list->layouts[k];
                s32 need = (l->count + numSfx) * 4;
                u32 j;

                for (j = 0; j < l->count; j++) {
                    struct GaxHandlerType *t = l->types[j];

                    if (j != 2) {
                        need = need + 0xc + t->instanceSize + t->childCount * 4;
                        if (j == 0)
                            need += numSfx * 4;
                    }
                }
                if (need > max)
                    max = need;
            }
            size += max;
        }
    }
    for (i = 0; i < numSfx; i++)
        size += 0x58;
    {
        struct GaxDspTap *tap = layout->types[0]->data.dsp->taps;

        for (i = 0; i <= 2; i++) {
            if (tap->rate > maxRate)
                maxRate = tap->rate;
            tap++;
        }
    }
    if (!(flags & 0x10)) {
        struct GaxLayoutList *list = (struct GaxLayoutList *)layout->types[2];

        if (list != NULL) {
            s32 k;

            for (k = 0; k < (s32)list->count; k++) {
                struct GaxDspTap *tap = list->layouts[k]->types[0]->data.dsp->taps;

                for (i = 0; i <= 2; i++) {
                    if (tap->rate > maxRate)
                        maxRate = tap->rate;
                    tap++;
                }
            }
        }
    }
    if (maxRate != 0)
        size = size + sub_8037E54(maxRate * rate, 1000) * 2 + 0x18;
    if (layout->types[1]->data.song->field_1b != 0 || (flags & 0x20))
        size += 0x130;
    else
        size += 0xdc;
    {
        s32 frames = sub_8037E54(rate * 1000, 0xe94f);

        size += frames * 2;
        size += frames * 2;
    }
    p->workSize = size + 0x20;
}
#else
NAKED void sub_8037FC0(struct GaxSongHeader *p)
{
    asm(".syntax unified\n"
        "\tpush {r4, r5, r6, r7, lr}\n"
        "\tmov r7, sl\n"
        "\tmov r6, sb\n"
        "\tmov r5, r8\n"
        "\tpush {r5, r6, r7}\n"
        "\tsub sp, #0x1c\n"
        "\tstr r0, [sp]\n"
        "\tmovs r7, #0\n"
        "\tmovs r0, #0\n"
        "\tstr r0, [sp, #8]\n"
        "\tldr r1, [sp]\n"
        "\tldr r0, [r1, #0x30]\n"
        "\tcmp r0, #0\n"
        "\tbne _08037FE0\n"
        "\tldr r0, _08037FF4\n"
        "\tstr r0, [r1, #0x30]\n"
        "_08037FE0:\n"
        "\tldr r2, [sp]\n"
        "\tldrh r1, [r2, #8]\n"
        "\tldr r0, _08037FF8\n"
        "\tcmp r1, r0\n"
        "\tbne _08037FFC\n"
        "\tldr r0, [r2, #0x30]\n"
        "\tldr r0, [r0, #8]\n"
        "\tldr r0, [r0, #0x18]\n"
        "\tldrh r2, [r0, #0x18]\n"
        "\tb _08038000\n"
        "\t.align 2, 0\n"
        "_08037FF4:\n"
        "\t.4byte gStaticData_085A4C5C\n"
        "_08037FF8:\n"
        "\t.4byte 0x0000FFFF\n"
        "_08037FFC:\n"
        "\tldr r4, [sp]\n"
        "\tldrh r2, [r4, #8]\n"
        "_08038000:\n"
        "\tldr r6, [sp]\n"
        "\tldrh r1, [r6, #0xe]\n"
        "\tldr r0, _08038014\n"
        "\tcmp r1, r0\n"
        "\tbne _08038018\n"
        "\tldr r0, [r6, #0x30]\n"
        "\tldr r0, [r0, #8]\n"
        "\tldr r0, [r0, #0x18]\n"
        "\tldrb r0, [r0, #0x1a]\n"
        "\tb _0803801C\n"
        "\t.align 2, 0\n"
        "_08038014:\n"
        "\t.4byte 0x0000FFFF\n"
        "_08038018:\n"
        "\tldr r0, [sp]\n"
        "\tldrh r0, [r0, #0xe]\n"
        "_0803801C:\n"
        "\tmov r8, r0\n"
        "\tldr r1, [sp]\n"
        "\tldr r0, [r1, #0x2c]\n"
        "\tcmp r0, #0\n"
        "\tbne _0803802E\n"
        "\tmovs r4, #0\n"
        "\tmov r8, r4\n"
        "\tmov r6, r8\n"
        "\tstrh r6, [r1, #0xe]\n"
        "_0803802E:\n"
        "\tldr r4, _080381C0\n"
        "\tadds r0, r2, #0\n"
        "\tbl sub_8037FA0\n"
        "\tlsls r0, r0, #3\n"
        "\tadds r0, r0, r4\n"
        "\tldr r0, [r0]\n"
        "\tstr r0, [sp, #4]\n"
        "\tldr r0, [sp]\n"
        "\tldrh r3, [r0, #0xc]\n"
        "\tmovs r0, #4\n"
        "\tands r0, r3\n"
        "\tcmp r0, #0\n"
        "\tbeq _0803804C\n"
        "\tadds r7, #0xf0\n"
        "_0803804C:\n"
        "\tmovs r1, #0xc6\n"
        "\tlsls r1, r1, #1\n"
        "\tadds r7, r7, r1\n"
        "\tldr r4, [sp]\n"
        "\tldr r2, [r4, #0x30]\n"
        "\tldr r1, [r2]\n"
        "\tmov r6, r8\n"
        "\tadds r0, r1, r6\n"
        "\tlsls r0, r0, #2\n"
        "\tadds r7, r7, r0\n"
        "\tadds r7, #8\n"
        "\tmovs r4, #0\n"
        "\tstr r2, [sp, #0x10]\n"
        "\tmov r0, sp\n"
        "\tstrh r3, [r0, #0x18]\n"
        "\tldr r6, [sp, #4]\n"
        "\tlsls r6, r6, #5\n"
        "\tstr r6, [sp, #0x14]\n"
        "\tcmp r4, r1\n"
        "\tbhs _080380A0\n"
        "\tmov r0, r8\n"
        "\tlsls r3, r0, #2\n"
        "_08038078:\n"
        "\tlsls r0, r4, #2\n"
        "\tadds r0, r0, r2\n"
        "\tldr r2, [r0, #4]\n"
        "\tcmp r4, #2\n"
        "\tbeq _08038096\n"
        "\tadds r1, r7, #0\n"
        "\tadds r1, #0xc\n"
        "\tldr r0, [r2, #0x14]\n"
        "\tadds r7, r1, r0\n"
        "\tldr r0, [r2, #0xc]\n"
        "\tlsls r0, r0, #2\n"
        "\tadds r7, r7, r0\n"
        "\tcmp r4, #0\n"
        "\tbne _08038096\n"
        "\tadds r7, r7, r3\n"
        "_08038096:\n"
        "\tadds r4, #1\n"
        "\tldr r2, [sp, #0x10]\n"
        "\tldr r0, [r2]\n"
        "\tcmp r4, r0\n"
        "\tblo _08038078\n"
        "_080380A0:\n"
        "\tmovs r0, #0x10\n"
        "\tmov r1, sp\n"
        "\tldrh r1, [r1, #0x18]\n"
        "\tands r0, r1\n"
        "\tcmp r0, #0\n"
        "\tbne _08038114\n"
        "\tldr r2, [sp, #0x10]\n"
        "\tldr r2, [r2, #0xc]\n"
        "\tmov sb, r2\n"
        "\tcmp r2, #0\n"
        "\tbeq _08038114\n"
        "\tmovs r4, #0\n"
        "\tstr r4, [sp, #0xc]\n"
        "\tmovs r5, #0\n"
        "\tldr r6, [r2]\n"
        "\tmov sl, r6\n"
        "\tcmp r4, sl\n"
        "\tbge _08038110\n"
        "_080380C4:\n"
        "\tlsls r0, r5, #2\n"
        "\tadd r0, sb\n"
        "\tldr r3, [r0, #4]\n"
        "\tldr r2, [r3]\n"
        "\tmov r1, r8\n"
        "\tadds r0, r2, r1\n"
        "\tlsls r1, r0, #2\n"
        "\tmovs r4, #0\n"
        "\tadds r5, #1\n"
        "\tmov ip, r5\n"
        "\tcmp r4, r2\n"
        "\tbhs _08038102\n"
        "\tmov r0, r8\n"
        "\tlsls r6, r0, #2\n"
        "\tadds r5, r2, #0\n"
        "_080380E2:\n"
        "\tldr r2, [r3, #4]\n"
        "\tcmp r4, #2\n"
        "\tbeq _080380FA\n"
        "\tadds r1, #0xc\n"
        "\tldr r0, [r2, #0x14]\n"
        "\tadds r1, r1, r0\n"
        "\tldr r0, [r2, #0xc]\n"
        "\tlsls r0, r0, #2\n"
        "\tadds r1, r1, r0\n"
        "\tcmp r4, #0\n"
        "\tbne _080380FA\n"
        "\tadds r1, r1, r6\n"
        "_080380FA:\n"
        "\tadds r3, #4\n"
        "\tadds r4, #1\n"
        "\tcmp r4, r5\n"
        "\tblo _080380E2\n"
        "_08038102:\n"
        "\tldr r2, [sp, #0xc]\n"
        "\tcmp r1, r2\n"
        "\tble _0803810A\n"
        "\tstr r1, [sp, #0xc]\n"
        "_0803810A:\n"
        "\tmov r5, ip\n"
        "\tcmp r5, sl\n"
        "\tblt _080380C4\n"
        "_08038110:\n"
        "\tldr r4, [sp, #0xc]\n"
        "\tadds r7, r7, r4\n"
        "_08038114:\n"
        "\tmovs r4, #0\n"
        "\tcmp r4, r8\n"
        "\tbhs _08038122\n"
        "_0803811A:\n"
        "\tadds r7, #0x58\n"
        "\tadds r4, #1\n"
        "\tcmp r4, r8\n"
        "\tblo _0803811A\n"
        "_08038122:\n"
        "\tmovs r4, #0\n"
        "\tldr r6, [sp, #0x10]\n"
        "\tldr r0, [r6, #4]\n"
        "\tldr r1, [r0, #0x18]\n"
        "_0803812A:\n"
        "\tldr r0, [r1, #4]\n"
        "\tldr r2, [sp, #8]\n"
        "\tcmp r0, r2\n"
        "\tbls _08038134\n"
        "\tstr r0, [sp, #8]\n"
        "_08038134:\n"
        "\tadds r1, #8\n"
        "\tadds r4, #1\n"
        "\tcmp r4, #2\n"
        "\tbls _0803812A\n"
        "\tmovs r0, #0x10\n"
        "\tmov r4, sp\n"
        "\tldrh r4, [r4, #0x18]\n"
        "\tands r0, r4\n"
        "\tcmp r0, #0\n"
        "\tbne _08038182\n"
        "\tldr r6, [sp, #0x10]\n"
        "\tldr r0, [r6, #0xc]\n"
        "\tcmp r0, #0\n"
        "\tbeq _08038182\n"
        "\tadds r3, r0, #0\n"
        "\tmovs r1, #0\n"
        "\tldr r0, [r3]\n"
        "\tcmp r1, r0\n"
        "\tbge _08038182\n"
        "\tadds r5, r0, #0\n"
        "_0803815C:\n"
        "\tlsls r0, r1, #2\n"
        "\tadds r0, r0, r3\n"
        "\tldr r0, [r0, #4]\n"
        "\tmovs r4, #0\n"
        "\tadds r2, r1, #1\n"
        "\tldr r0, [r0, #4]\n"
        "\tldr r1, [r0, #0x18]\n"
        "_0803816A:\n"
        "\tldr r0, [r1, #4]\n"
        "\tldr r6, [sp, #8]\n"
        "\tcmp r0, r6\n"
        "\tbls _08038174\n"
        "\tstr r0, [sp, #8]\n"
        "_08038174:\n"
        "\tadds r1, #8\n"
        "\tadds r4, #1\n"
        "\tcmp r4, #2\n"
        "\tbls _0803816A\n"
        "\tadds r1, r2, #0\n"
        "\tcmp r1, r5\n"
        "\tblt _0803815C\n"
        "_08038182:\n"
        "\tldr r0, [sp, #8]\n"
        "\tcmp r0, #0\n"
        "\tbeq _0803819E\n"
        "\tadds r1, r0, #0\n"
        "\tldr r2, [sp, #4]\n"
        "\tadds r0, r1, #0\n"
        "\tmuls r0, r2, r0\n"
        "\tmovs r1, #0xfa\n"
        "\tlsls r1, r1, #2\n"
        "\tbl sub_8037E54\n"
        "\tlsls r0, r0, #1\n"
        "\tadds r7, r7, r0\n"
        "\tadds r7, #0x18\n"
        "_0803819E:\n"
        "\tldr r4, [sp, #0x10]\n"
        "\tldr r0, [r4, #8]\n"
        "\tldr r0, [r0, #0x18]\n"
        "\tldrb r0, [r0, #0x1b]\n"
        "\tcmp r0, #0\n"
        "\tbne _080381B6\n"
        "\tmovs r0, #0x20\n"
        "\tmov r6, sp\n"
        "\tldrh r6, [r6, #0x18]\n"
        "\tands r0, r6\n"
        "\tcmp r0, #0\n"
        "\tbeq _080381C4\n"
        "_080381B6:\n"
        "\tmovs r0, #0x98\n"
        "\tlsls r0, r0, #1\n"
        "\tadds r7, r7, r0\n"
        "\tb _080381C6\n"
        "\t.align 2, 0\n"
        "_080381C0:\n"
        "\t.4byte gStaticData_085A6150\n"
        "_080381C4:\n"
        "\tadds r7, #0xdc\n"
        "_080381C6:\n"
        "\tldr r1, [sp, #0x14]\n"
        "\tldr r2, [sp, #4]\n"
        "\tsubs r0, r1, r2\n"
        "\tlsls r0, r0, #2\n"
        "\tadds r0, r0, r2\n"
        "\tlsls r0, r0, #3\n"
        "\tldr r1, _080381F8\n"
        "\tbl sub_8037E54\n"
        "\tlsls r0, r0, #1\n"
        "\tadds r7, r7, r0\n"
        "\tadds r7, r7, r0\n"
        "\tadds r0, r7, #0\n"
        "\tadds r0, #0x20\n"
        "\tldr r4, [sp]\n"
        "\tstr r0, [r4, #4]\n"
        "\tadd sp, #0x1c\n"
        "\tpop {r3, r4, r5}\n"
        "\tmov r8, r3\n"
        "\tmov sb, r4\n"
        "\tmov sl, r5\n"
        "\tpop {r4, r5, r6, r7}\n"
        "\tpop {r0}\n"
        "\tbx r0\n"
        "\t.align 2, 0\n"
        "_080381F8:\n"
        "\t.4byte 0x0000E94F\n"
        ".syntax divided\n");
}
#endif
