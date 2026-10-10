#include "gax_internal.h"
#include "match.h"

/* GAX2 per-channel mixer (issue #68): renders channel `self` into
 * `buf` through the ARM resampling routine `gGaxPlayerState->mixCode`
 * points at (an IWRAM copy of the hand-written ARM code at
 * `gGaxArmResample`, lib/gax/asm/gax_arm_dsp.s). Called by the Channel
 * play_fns `GaxChannelPlay`/`GaxFxChannelPlay` as `GaxChannelMix(self, info, buf,
 * arg, type data, flag)`.
 *
 * Bails out (returns 0) when no instrument is bound, no note is playing
 * (`0x8AD0`), `row` is out of range or the row's wave is empty.
 * Otherwise it looks up the sample step for `note + vibratoOffset` (+ pitch
 * slide and, for pattern channels, the order's transpose) + the row's
 * tune in the period table `gGaxPeriodTable` (capped at 0xEF3),
 * scaled by the mix-rate reciprocal `gGaxMixRateReciprocal` (`__muldi3`),
 * chains the envelope/volume/Info/song volumes, builds a 10-word work
 * item on the stack, then loops calling the ARM routine - patching two
 * of its instructions first to pick forward/backward stepping - and
 * handles each return: buffer full, ping-pong bounce, sweep/loop wrap,
 * or end of sample (clears the rest of the buffer and arms the "no
 * note" state). Returns 1.
 *
 * `sub_8039E50` in the ROM disassembly is not a function: it's the
 * `nop` the ARM call returns to (see `GAX_CALL_ARM_R`, gax_internal.h),
 * never called from anywhere. Like gax_sound_handler_mixer_play.c's return points,
 * it no longer has a label of its own.
 *
 * Matched (plain agbcc) in GAX retry 6 (docs/matching/archive/gax-naked-retry-6.md)
 * after five register-allocation passes (docs/matching/archive/gax-toolchain-retry.md,
 * mix-naked-retry-5.md, gax-naked-retry-3.md to -5.md). */

/* MemCopy32 is this ROM's memcpy (the work item's initializer). The
 * 64-bit multiply is a `__muldi3` libcall, which does not clobber
 * memory (see docs/matching/archive/gax-naked-retry-2.md). */
asm(".set memcpy, MemCopy32");

struct GaxMixItem {
    u8 *src;
    void *buf;
    s32 pos; /* Q11 sample position */
    s32 end; /* Q11 end/turn-around position */
    u32 frames;
    /* samples written so far: the ARM routine advances it behind gcc's
     * back, and the ROM reads it again at every use (GaxZeroFill's two
     * arguments load it twice in a row, which only a volatile access
     * does). #662 round 4: as a volatile field the rest of the item is
     * plain, and the item itself needs no volatile qualifier. */
    volatile u32 done;
    u32 volume;
    u32 step; /* Q11 per output sample */
    u32 mode;
    s32 loopLen; /* Q11 sweep loop length, 0 = none */
};

/* Rewrites the halfword at `label` in the IWRAM copy of the ARM mixer. */
#define GAX_PATCH_MIXER(label, value) \
    (((u16 *)gGaxPlayerState->mixCode)[((const u8 *)(label) - (const u8 *)gGaxArmResample + 2) / 2] = (value))

u32 GaxChannelMix(struct GaxChannelState *self, struct GaxInfoHandler *info, void *buf, u32 arg,
                  struct GaxSongData *song, u8 flag)
{
    struct GaxChannelInstrument *inst;
    struct GaxWave *wave;
    s32 pitch;
    u32 idx;
    u32 period;
    u32 vol;
    u32 step;
    u32 len;
    u32 pingpong;

    if (self->instrument == NULL || self->note == (s16)0x8ad0 || self->row > 3)
        return 0;
    wave = &song->waves[self->instrument->waveIdx[self->row]];
    if (wave->data == NULL)
        return 0;

    pitch = self->note + self->vibratoOffset;
    if (self->fixedPitch == 0) {
        pitch += self->pitch;
        if (flag == 0)
            pitch += self->type->data.orders[info->orderPos].transpose << 5;
    }
    {
        /* The tune: the ROM loads `self->instrument` into r0, adds the
         * table after the tune load, loads 0xef3, and only then copies the
         * pointer to r8 (`inst`) right before the clamp's `cmp`. */
        struct GaxChannelInstrument *ip = self->instrument;
        const u32 *tab;
        s32 t;
        u32 m;

        t = ip->rows[self->row].tune;
        tab = gGaxPeriodTable;
        idx = pitch + t;
        m = 0xef3;
        inst = ip;
        /* A two-armed clamp. Its join label ends cse's extended basic
         * block, so the ping-pong test below computes `row * 28` again,
         * as the ROM does, instead of reusing the tune's product. The
         * no-code escape stops cse rewriting `tab[idx]` as `tab[m]` in the
         * first arm, so both arms end in the same insns, and
         * cross-jumping (jump2, after reload) merges them into the ROM's
         * one-armed clamp.
         *
         * What the ROM needs (#662 round 11, from -dc/-dt dumps): a block
         * end between the tune and the ping-pong test in cse1 *and* cse2
         * that is gone by the final code. Where it is doesn't matter: an
         * `else MATCH_KEEP` arm at the clamp, at the vol17 step or at the
         * flag volume step leaves only its own jump (9 instructions off),
         * and `else if ((s32)idx < 0) idx = 0;` (Shin'en's if/else-if
         * clamp) gives the ROM except for that test's 4 instructions. A
         * break that cse1 sees but the jump pass after it folds back
         * (an inline `GaxScaleVolume(vol, v)` with an early return) is
         * not enough: gcse turns both row loads into copies of one
         * register and cse2 then reuses the product. A one-armed `if` is
         * skipped by cse's skip-blocks rule, and its 7 branches before
         * the test don't reach the path limit (PATHLENGTH 10).
         *
         * What doesn't work: the one-armed `if` (71 instructions off; with
         * -fno-cse-skip-blocks 79); every single-load spelling, which
         * jump.c lowers to the one-armed `if`; two loads with no escape
         * (`if (idx > m) period = tab[m]; else period = tab[idx];` and 7
         * other spellings, 27-59 off), because cse makes the first arm's
         * index `m`, not `idx`, and cross-jumping only merges the load;
         * if/else and `?:` forms of the volume steps, which jump1
         * converts to one-armed before cse1; a clamp inline (one or both
         * bounds, 64-72); a separate final volume variable or a
         * GaxChannelVolume() inline with an early return (the break
         * survives to reload, but the two values get different
         * registers, 31-63); the row in a local read before the
         * transposition (14-32: cse no longer reuses the product, but
         * the local and gcse's copy of the row are two registers where
         * the ROM has one, r9). Rounds 3-9 (one-armed forms, inlines,
         * natural_enum.py over 103 edits, every same-size retype) are in
         * docs/matching_techniques.md. */
        if (idx > m) {
            idx = m;
            MATCH_KEEP(idx);
            period = tab[idx];
        } else
            period = tab[idx];
    }
    vol = self->envOut != 0xff ? self->envOut : 0x100;
    if (self->vol17 != 0xff)
        vol = vol * self->vol17 >> 8;
    if (self->vol15 != 0xff)
        vol = vol * self->vol15 >> 8;
    if ((u8)self->volume != 0xff)
        vol = vol * (u8)self->volume >> 8;
    if (info->volume != 0xff)
        vol = vol * info->volume >> 8;
    if (flag == 0)
        vol = vol * info->type->data.song->volume >> 8;
    {
        /* one DImode variable for operand and result: it overlaps the
         * libcall's r0-r3 setup, so it takes r4:r5 (first in global's
         * order) and keeps `self` out of them, as in the ROM */
        s64 prod = (s32)period;

        prod = prod * gGaxMixRateReciprocal >> 32;
        step = prod;
    }
    len = wave->length;
    pingpong = 0;
    if (inst->rows[self->row].sweep == 0 &&
        inst->rows[self->row].sweepMin < inst->rows[self->row].sweepMax)
        pingpong = 1;
    {
        /* the ROM loads these three before the first store to the item */
        u32 frames = self->format->frames;
        u8 *data = wave->data;
        s32 pos = self->samplePos;
        /* The ARM routine updates the item through the pointer the call
         * passes; only `done` is read back where gcc can't know it changed
         * (see GaxMixItem).
         *
         * The loop length reads `self->instrument` again through a
         * volatile lvalue: the ROM loads it fresh, which a gcc 2.9 gcse
         * bug decides (#662 round 11, from an instrumented agbcc). The
         * item initializer emits `(clobber (mem:BLK item))`, and
         * compute_transp passes that CLOBBER rtx, not its MEM, to
         * true_dependence. MEM_ALIAS_SET then reads the word after the
         * clobber, which is the header of the insn that holds it: rtx
         * code 31 (INSN). So the clobber kills every load whose alias set
         * is 0 or 31 (the row loads after it are fresh, as in the ROM),
         * and the plain `self->instrument` read, alias set 9 here (a
         * `struct GaxChannelInstrument *`, the 9th type this file gives an
         * alias set), survives it: PRE turns it into a copy of a load it
         * inserts at the end of the tune's block, which reload spills (15
         * instructions off). Declaring 22 more pointer types ahead of the
         * function puts that type in set 31, and the plain read then
         * compiles to the ROM's code, so Shin'en's file numbered about
         * 22 more types first. None of the GAX2's other real globals adds
         * an alias set (char and u8 arrays are set 0, the rest reuse
         * existing sets), and the whole GAX library in one file, in ROM
         * order, gets this type only to set 21. A union field (set 0)
         * would change the data layout.
         *
         * What else was tried (rounds 4-9): retyped item fields, an
         * explicit `__muldi3()` call through libgcc.h's prototype (the
         * load is right, the rest 84 lines off), the -f flags (with
         * -fno-strict-aliasing the read is right but the mixer patches
         * reload gGaxPlayerState, 30 off), inlines for the sweep length,
         * and every same-size retype of the structs involved. */
        // clang-format off
        struct GaxMixItem item = {
            data, buf, pos, len << 11, frames, 0, vol, step, 0,
            self->sweepOn
                ? (*(struct GaxChannelInstrument *volatile *)&self->instrument)
                          ->rows[self->row].sweepLen << 11
                : 0,
        };
        // clang-format on

        while (item.done < self->format->frames) {
            if (self->direction > 0) {
                if (pingpong)
                    item.end = self->instrument->rows[self->row].sweepMax << 11;
                else if (self->sweepOn)
                    item.end = (self->sweepPos + self->instrument->rows[self->row].sweepLen) << 11;
                else
                    item.end = wave->length << 11;
                if (self->isFirst) {
                    item.mode = 0;
                    GAX_PATCH_MIXER(gGaxArmResampleStoreStep, 0xe082);
                    GAX_PATCH_MIXER(gGaxArmResampleStoreEndTest, 0xbaff);
                } else {
                    item.mode = self->mixMode;
                    GAX_PATCH_MIXER(gGaxArmResampleMixStep, 0xe082);
                    GAX_PATCH_MIXER(gGaxArmResampleMixEndTest, 0xbaff);
                }
            } else {
                /* Reuses `len`: as one pseudo with the wave length above, it
                 * gets r3 from global-alloc (the ROM's `ldr r3; lsl r0, r3`). */
                len = self->instrument->rows[self->row].sweepMin;
                item.end = len << 11;
                if (self->isFirst) {
                    item.mode = 0;
                    GAX_PATCH_MIXER(gGaxArmResampleStoreStep, 0xe042);
                    GAX_PATCH_MIXER(gGaxArmResampleStoreEndTest, 0xcaff);
                } else {
                    item.mode = 1;
                    GAX_PATCH_MIXER(gGaxArmResampleMixStep, 0xe042);
                    GAX_PATCH_MIXER(gGaxArmResampleMixEndTest, 0xcaff);
                }
            }
            GAX_CALL_ARM_R(gGaxPlayerState->mixCode, &item);
            if (item.done == self->format->frames)
                break;
            if (pingpong) {
                if (self->instrument->rows[self->row].pingPong != 0) {
                    if (self->direction > 0)
                        item.pos -= step * 2;
                    else
                        item.pos += step * 2;
                    self->direction = ~self->direction;
                } else {
                    // clang-format off
                    item.pos -= (self->instrument->rows[self->row].sweepMax -
                                 self->instrument->rows[self->row].sweepMin) << 11;
                    // clang-format on
                }
            } else if (self->sweepOn) {
                item.pos -= self->instrument->rows[self->row].sweepLen << 11;
            } else {
                if (self->isFirst)
                    GaxZeroFill((u16 *)buf + item.done, (self->format->frames - item.done + 1) * 2);
                self->note = 0x8ad0;
                self->noteStep = 0;
                self->priority = 0x80000000;
                break;
            }
        }
        self->samplePos = item.pos;
    }
    return 1;
}
