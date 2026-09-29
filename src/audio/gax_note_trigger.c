#include "core.h"
#include "audio.h"

/* GAX2 per-channel mixer (issue #68): renders channel `self` into
 * `buf` through the ARM resampling routine `gUnknown_03001630->field_44`
 * points at (an IWRAM copy of the raw ARM code at
 * `gStaticData_0803A818`, gax_unknownc_play.c). Called by the Channel
 * play_fns `sub_80395A4`/`sub_803A158` as `sub_8039B44(self, info, buf,
 * arg, type data, flag)`.
 *
 * Bails out (returns 0) when no instrument is bound, no note is playing
 * (`0x8AD0`), `row` is out of range or the row's wave is empty.
 * Otherwise it looks up the sample step for `note + field_2e` (+ pitch
 * slide and, for pattern channels, the order's transpose) + the row's
 * tune in the period table `gStaticData_085A62DC` (capped at 0xEF3),
 * scaled by the mix-rate reciprocal `gUnknown_03001618` (`__muldi3`),
 * chains the envelope/volume/Info/song volumes, builds a 10-word work
 * item on the stack, then loops calling the ARM routine - patching two
 * of its instructions first to pick forward/backward stepping - and
 * handles each return: buffer full, ping-pong bounce, sweep/loop wrap,
 * or end of sample (clears the rest of the buffer and arms the "no
 * note" state). Returns 1.
 *
 * `sub_8039E50` in the ROM disassembly is not a function: it's the
 * `nop` the ARM call returns to (see `GAX_CALL_ARM_R`, include/audio.h),
 * never called from anywhere; the NAKED body keeps the label so the
 * address still carries its name.
 *
 * Still NAKED. The ARM call itself is no longer a blocker - it's the
 * engine's own inline asm idiom, `GAX_CALL_ARM_R` (register-operand
 * variant of the `GAX_CALL_ARM` that closed the four
 * gax_unknownc_play.c functions). The draft below is a complete
 * reconstruction with the ROM's control flow, work-item layout,
 * instruction patches and tail merges (same size to within 12 bytes),
 * but its register allocation differs throughout: the ROM keeps
 * `self`/`info`/`flag`/`vol` in r6/r4/r5/r7, `row` in sb, the step in
 * sl and the wave pointer spilled to the stack, where agbcc puts `self`
 * in r5, `info` in ip and the wave in sl - see
 * docs/matching/gax-toolchain-retry.md.
 * Mix retry 5 (docs/matching/mix-naked-retry-5.md): the step is a plain
 * 64-bit `*` through a `__muldi3` alias and the ping-pong test re-reads
 * `self->instrument`/`self->row`; ~202 halfwords off by the
 * alignment-insensitive count (was ~237). Left: `self`/`info`/`flag`
 * still land in r5/r7/r9 instead of r6/r4/r5, which cascades.
 * GAX retry 3 (docs/matching/gax-naked-retry-3.md): ~46 off (was ~202).
 * `self`/`info`/`flag`/`vol` now get r6/r4/r5/r7 and the mixer loop
 * matches. Left: the tune reads `row` from a copy made at the top of
 * its block where the ROM copies sb right at the use, the sweep-length
 * initializer and the backward end (`sweepMin`) reuse registers where the
 * ROM reloads into r3, and literal-pool placement follows from those.
 * GAX retry 4 (docs/matching/gax-naked-retry-4.md): ~43 (was ~46; the
 * rest of that score is relocation noise in the literal pools). The row
 * copy now sits at the tune and the sweep-length initializer reloads
 * `self->instrument`. Left: the ROM loads `self->instrument` into r0 for
 * the tune and copies it to r8 only just before the clamp's `cmp` (the
 * draft loads straight into r8), and the backward end still loads
 * `sweepMin` into r0 where the ROM uses r3.
 * GAX retry 5 (docs/matching/gax-naked-retry-5.md): no change; that doc
 * has a variant with the ROM's exact tune instruction order, which moves
 * registers elsewhere. */
#if NON_MATCHING
/* sub_800014C is this ROM's memcpy (the work item's initializer) and
 * sub_8037ECC is `__muldi3`: as a libcall the 64-bit multiply does not
 * clobber memory (see docs/matching/gax-naked-retry-2.md). */
asm(".set memcpy, sub_800014C\n.set __muldi3, sub_8037ECC\n");

struct GaxMixItem {
    u8 *src;
    void *buf;
    s32 pos;     /* Q11 sample position */
    s32 end;     /* Q11 end/turn-around position */
    u32 frames;
    u32 done;    /* samples written so far */
    u32 volume;
    u32 step;    /* Q11 per output sample */
    u32 mode;
    s32 loopLen; /* Q11 sweep loop length, 0 = none */
};

extern u64 gUnknown_03001618;
extern const u32 gStaticData_085A62DC[];
extern u8 gStaticData_0803A818[];
extern u8 gStaticData_0803A874[];
extern u8 gStaticData_0803A884[];
extern u8 gStaticData_0803A8B4[];
extern u8 gStaticData_0803A8C4[];
extern void sub_8037F3C(void *dest, s32 count);

/* Rewrites the halfword at `label` in the IWRAM copy of the ARM mixer. */
#define GAX_PATCH_MIXER(label, value) \
    (((u16 *)gUnknown_03001630->field_44)[((label) - gStaticData_0803A818 + 2) / 2] = (value))

u32 sub_8039B44(struct GaxChannelState *self, struct GaxInfoHandler *info, void *buf, u32 arg,
                struct GaxSongData *song, u8 flag)
{
    struct GaxChannelInstrument *inst;
    struct GaxWave *wave;
    u32 row;
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

    pitch = self->note + self->field_2e;
    if (self->field_21 == 0) {
        pitch += self->pitch;
        if (flag == 0)
            pitch += self->type->data.orders[info->orderPos].transpose << 5;
    }
    inst = self->instrument;
    row = self->row;
    /* The ROM copies GCSE's row register (sb) right here, and computes
     * `row * 28` again for the ping-pong test below. The volatile escape
     * keeps CSE/GCSE from reusing this product there. */
    asm volatile("" : "+r"(row));
    idx = pitch + inst->rows[row].tune;
    period = gStaticData_085A62DC[idx > 0xef3 ? 0xef3 : idx];
    vol = self->envOut != 0xff ? self->envOut : 0x100;
    if (self->vol17 != 0xff)
        vol = vol * self->vol17 >> 8;
    if (self->vol15 != 0xff)
        vol = vol * self->vol15 >> 8;
    if ((u8)self->field_18 != 0xff)
        vol = vol * (u8)self->field_18 >> 8;
    if (info->field_1f != 0xff)
        vol = vol * info->field_1f >> 8;
    if (flag == 0)
        vol = vol * info->type->data.song->volume >> 8;
    {
        /* one DImode variable for operand and result: it overlaps the
         * libcall's r0-r3 setup, so it takes r4:r5 (first in global's
         * order) and keeps `self` out of them, as in the ROM */
        s64 prod = (s32)period;

        prod = prod * gUnknown_03001618 >> 32;
        step = prod;
    }
    len = wave->length;
    pingpong = 0;
    if (inst->rows[self->row].field_00 == 0
        && inst->rows[self->row].sweepMin < inst->rows[self->row].sweepMax)
        pingpong = 1;
    {
        /* the ROM loads these three before the first store to the item */
        u32 frames = self->format->frames;
        u8 *data = wave->data;
        s32 pos = self->samplePos;
        /* volatile: the ARM routine updates it behind gcc's back, and the
         * ROM re-reads `item.done` at every use. The sweep length re-reads
         * `self->instrument` (volatile read) where GCSE would reuse `inst`. */
        volatile struct GaxMixItem item = {
            data, buf, pos, len << 11, frames, 0, vol, step, 0,
            self->sweepOn
                ? (*(struct GaxChannelInstrument *volatile *)&self->instrument)
                          ->rows[self->row].sweepLen << 11
                : 0,
        };

        while (item.done < self->format->frames) {
            if (self->field_11 > 0) {
                if (pingpong)
                    item.end = self->instrument->rows[self->row].sweepMax << 11;
                else if (self->sweepOn)
                    item.end = (self->sweepPos + self->instrument->rows[self->row].sweepLen) << 11;
                else
                    item.end = wave->length << 11;
                if (self->field_0d) {
                    item.mode = 0;
                    GAX_PATCH_MIXER(gStaticData_0803A874, 0xe082);
                    GAX_PATCH_MIXER(gStaticData_0803A884, 0xbaff);
                } else {
                    item.mode = self->field_52;
                    GAX_PATCH_MIXER(gStaticData_0803A8B4, 0xe082);
                    GAX_PATCH_MIXER(gStaticData_0803A8C4, 0xbaff);
                }
            } else {
                item.end = self->instrument->rows[self->row].sweepMin << 11;
                if (self->field_0d) {
                    item.mode = 0;
                    GAX_PATCH_MIXER(gStaticData_0803A874, 0xe042);
                    GAX_PATCH_MIXER(gStaticData_0803A884, 0xcaff);
                } else {
                    item.mode = 1;
                    GAX_PATCH_MIXER(gStaticData_0803A8B4, 0xe042);
                    GAX_PATCH_MIXER(gStaticData_0803A8C4, 0xcaff);
                }
            }
            GAX_CALL_ARM_R(gUnknown_03001630, &item);
            if (item.done == self->format->frames)
                break;
            if (pingpong) {
                if (self->instrument->rows[self->row].pingPong != 0) {
                    if (self->field_11 > 0)
                        item.pos -= step * 2;
                    else
                        item.pos += step * 2;
                    self->field_11 = ~self->field_11;
                } else {
                    item.pos -= (self->instrument->rows[self->row].sweepMax
                                 - self->instrument->rows[self->row].sweepMin) << 11;
                }
            } else if (self->sweepOn) {
                item.pos -= self->instrument->rows[self->row].sweepLen << 11;
            } else {
                if (self->field_0d)
                    sub_8037F3C((u16 *)buf + item.done, (self->format->frames - item.done + 1) * 2);
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
#else /* !NON_MATCHING */
NAKED u32 sub_8039B44(void *self, void *info, u32 arg1, u32 arg2, u32 arg5, u32 flag)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0x5c\n\t"
        "add r6, r0, #0\n\t"
        "add r4, r1, #0\n\t"
        "str r2, [sp, #0x50]\n\t"
        "ldr r0, [sp, #0x80]\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r5, r0, #0x18\n\t"
        "ldr r3, [r6, #0x3c]\n\t"
        "cmp r3, #0\n\t"
        "beq 1f\n\t"
        "mov r0, #0x2a\n\t"
        "ldrsh r1, [r6, r0]\n\t"
        "ldr r0, 2f\n\t"
        "cmp r1, r0\n\t"
        "beq 1f\n\t"
        "ldrb r0, [r6, #0x10]\n\t"
        "cmp r0, #3\n\t"
        "bhi 1f\n\t"
        "ldrb r2, [r6, #0x10]\n\t"
        "add r0, r3, #1\n\t"
        "add r0, r0, r2\n\t"
        "ldrb r0, [r0]\n\t"
        "lsl r0, r0, #3\n\t"
        "ldr r1, [sp, #0x7c]\n\t"
        "ldr r1, [r1, #0x14]\n\t"
        "add r1, r1, r0\n\t"
        "str r1, [sp, #0x54]\n\t"
        "ldr r0, [r1]\n\t"
        "mov sb, r2\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
        "1:\n\t"
        "mov r0, #0\n\t"
        "b 57f\n\t"
        ".align 2, 0\n\t"
        "2: .4byte 0xFFFF8AD0\n\t"
        "3:\n\t"
        "mov r1, #0x2a\n\t"
        "ldrsh r0, [r6, r1]\n\t"
        "mov r2, #0x2e\n\t"
        "ldrsh r1, [r6, r2]\n\t"
        "add r2, r0, r1\n\t"
        "add r0, r6, #0\n\t"
        "add r0, #0x21\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 4f\n\t"
        "mov r3, #0x26\n\t"
        "ldrsh r0, [r6, r3]\n\t"
        "add r2, r2, r0\n\t"
        "cmp r5, #0\n\t"
        "bne 4f\n\t"
        "ldr r1, [r6]\n\t"
        "mov r3, #0x14\n\t"
        "ldrsh r0, [r4, r3]\n\t"
        "ldr r1, [r1, #0x18]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldrb r0, [r0, #2]\n\t"
        "lsl r0, r0, #0x18\n\t"
        "asr r0, r0, #0x18\n\t"
        "lsl r0, r0, #5\n\t"
        "add r2, r2, r0\n\t"
        "4:\n\t"
        "ldr r0, [r6, #0x3c]\n\t"
        "mov r3, sb\n\t"
        "lsl r1, r3, #3\n\t"
        "sub r1, r1, r3\n\t"
        "lsl r1, r1, #2\n\t"
        "add r1, r0, r1\n\t"
        "mov r3, #0x26\n\t"
        "ldrsh r1, [r1, r3]\n\t"
        "ldr r3, 13f\n\t"
        "add r1, r2, r1\n\t"
        "ldr r2, 14f\n\t"
        "mov r8, r0\n\t"
        "cmp r1, r2\n\t"
        "bls 5f\n\t"
        "add r1, r2, #0\n\t"
        "5:\n\t"
        "lsl r0, r1, #2\n\t"
        "add r0, r0, r3\n\t"
        "ldr r1, [r0]\n\t"
        "ldrb r0, [r6, #0x16]\n\t"
        "mov r7, #0x80\n\t"
        "lsl r7, r7, #1\n\t"
        "cmp r0, #0xff\n\t"
        "beq 6f\n\t"
        "add r7, r0, #0\n\t"
        "6:\n\t"
        "ldrb r0, [r6, #0x17]\n\t"
        "cmp r0, #0xff\n\t"
        "beq 7f\n\t"
        "mul r0, r7, r0\n\t"
        "lsr r7, r0, #8\n\t"
        "7:\n\t"
        "ldrb r0, [r6, #0x15]\n\t"
        "cmp r0, #0xff\n\t"
        "beq 8f\n\t"
        "mul r0, r7, r0\n\t"
        "lsr r7, r0, #8\n\t"
        "8:\n\t"
        "ldrb r0, [r6, #0x18]\n\t"
        "cmp r0, #0xff\n\t"
        "beq 9f\n\t"
        "mul r0, r7, r0\n\t"
        "lsr r7, r0, #8\n\t"
        "9:\n\t"
        "ldrb r0, [r4, #0x1f]\n\t"
        "cmp r0, #0xff\n\t"
        "beq 10f\n\t"
        "mul r0, r7, r0\n\t"
        "lsr r7, r0, #8\n\t"
        "10:\n\t"
        "cmp r5, #0\n\t"
        "bne 11f\n\t"
        "ldr r0, [r4]\n\t"
        "ldr r0, [r0, #0x18]\n\t"
        "ldrh r0, [r0, #8]\n\t"
        "mul r0, r7, r0\n\t"
        "lsr r7, r0, #8\n\t"
        "11:\n\t"
        "add r4, r1, #0\n\t"
        "asr r5, r1, #0x1f\n\t"
        "ldr r0, 15f\n\t"
        "ldr r2, [r0]\n\t"
        "ldr r3, [r0, #4]\n\t"
        "add r1, r5, #0\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8037ECC\n\t"
        "add r4, r1, #0\n\t"
        "mov sl, r4\n\t"
        "ldr r4, [sp, #0x54]\n\t"
        "ldr r3, [r4, #4]\n\t"
        "mov r0, #0\n\t"
        "str r0, [sp, #0x58]\n\t"
        "mov r1, sb\n\t"
        "lsl r0, r1, #3\n\t"
        "sub r0, r0, r1\n\t"
        "lsl r2, r0, #2\n\t"
        "mov r4, r8\n\t"
        "add r0, r4, r2\n\t"
        "ldrb r0, [r0, #0xc]\n\t"
        "cmp r0, #0\n\t"
        "bne 12f\n\t"
        "mov r1, r8\n\t"
        "add r1, #0x14\n\t"
        "add r1, r1, r2\n\t"
        "mov r0, r8\n\t"
        "add r0, #0x18\n\t"
        "add r0, r0, r2\n\t"
        "ldr r1, [r1]\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r1, r0\n\t"
        "bge 12f\n\t"
        "mov r0, #1\n\t"
        "str r0, [sp, #0x58]\n\t"
        "12:\n\t"
        "ldr r0, [r6, #4]\n\t"
        "ldrh r2, [r0, #4]\n\t"
        "ldr r1, [sp, #0x54]\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r1, [r6, #0x44]\n\t"
        "mov r4, sp\n\t"
        "str r0, [sp, #0x28]\n\t"
        "ldr r0, [sp, #0x50]\n\t"
        "str r0, [sp, #0x2c]\n\t"
        "str r1, [sp, #0x30]\n\t"
        "lsl r0, r3, #0xb\n\t"
        "str r0, [sp, #0x34]\n\t"
        "str r2, [sp, #0x38]\n\t"
        "mov r0, #0\n\t"
        "str r0, [sp, #0x3c]\n\t"
        "str r7, [sp, #0x40]\n\t"
        "mov r1, sl\n\t"
        "str r1, [sp, #0x44]\n\t"
        "str r0, [sp, #0x48]\n\t"
        "ldrb r0, [r6, #0x12]\n\t"
        "cmp r0, #0\n\t"
        "beq 16f\n\t"
        "ldr r1, [r6, #0x3c]\n\t"
        "ldrb r2, [r6, #0x10]\n\t"
        "lsl r0, r2, #3\n\t"
        "sub r0, r0, r2\n\t"
        "lsl r0, r0, #2\n\t"
        "add r1, #0x1c\n\t"
        "add r1, r1, r0\n\t"
        "ldr r0, [r1]\n\t"
        "lsl r0, r0, #0xb\n\t"
        "b 17f\n\t"
        ".align 2, 0\n\t"
        "13: .4byte gStaticData_085A62DC\n\t"
        "14: .4byte 0x00000EF3\n\t"
        "15: .4byte gUnknown_03001618\n\t"
        "16:\n\t"
        "mov r0, #0\n\t"
        "17:\n\t"
        "str r0, [sp, #0x4c]\n\t"
        "add r1, sp, #0x28\n\t"
        "add r0, r4, #0\n\t"
        "mov r2, #0x28\n\t"
        "bl sub_800014C\n\t"
        "ldr r1, [r6, #4]\n\t"
        "ldr r0, [sp, #0x14]\n\t"
        "ldrh r1, [r1, #4]\n\t"
        "cmp r0, r1\n\t"
        "blo 18f\n\t"
        "b 56f\n\t"
        "18:\n\t"
        "ldr r0, 20f\n\t"
        "ldr r7, 21f\n\t"
        "sub r0, r0, r7\n\t"
        "add r5, r0, #2\n\t"
        "19:\n\t"
        "mov r0, #0x11\n\t"
        "ldrsb r0, [r6, r0]\n\t"
        "cmp r0, #0\n\t"
        "ble 35f\n\t"
        "ldr r2, [sp, #0x58]\n\t"
        "cmp r2, #0\n\t"
        "beq 22f\n\t"
        "ldr r2, [r6, #0x3c]\n\t"
        "ldrb r1, [r6, #0x10]\n\t"
        "lsl r0, r1, #3\n\t"
        "sub r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "add r2, #0x18\n\t"
        "add r2, r2, r0\n\t"
        "ldr r0, [r2]\n\t"
        "b 24f\n\t"
        ".align 2, 0\n\t"
        "20: .4byte gStaticData_0803A874\n\t"
        "21: .4byte gStaticData_0803A818\n\t"
        "22:\n\t"
        "ldrb r0, [r6, #0x12]\n\t"
        "cmp r0, #0\n\t"
        "beq 23f\n\t"
        "ldr r2, [r6, #0x3c]\n\t"
        "ldrb r1, [r6, #0x10]\n\t"
        "lsl r0, r1, #3\n\t"
        "sub r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "add r2, #0x1c\n\t"
        "add r2, r2, r0\n\t"
        "ldr r0, [r6, #0x48]\n\t"
        "ldr r1, [r2]\n\t"
        "add r0, r0, r1\n\t"
        "b 24f\n\t"
        "23:\n\t"
        "ldr r3, [sp, #0x54]\n\t"
        "ldr r0, [r3, #4]\n\t"
        "24:\n\t"
        "lsl r0, r0, #0xb\n\t"
        "str r0, [sp, #0xc]\n\t"
        "ldrb r0, [r6, #0xd]\n\t"
        "cmp r0, #0\n\t"
        "beq 29f\n\t"
        "mov r0, #0\n\t"
        "str r0, [sp, #0x20]\n\t"
        "ldr r3, 25f\n\t"
        "ldr r1, [r3]\n\t"
        "lsr r0, r5, #0x1f\n\t"
        "add r0, r5, r0\n\t"
        "asr r0, r0, #1\n\t"
        "ldr r2, [r1, #0x44]\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r0, r2\n\t"
        "ldr r4, 26f\n\t"
        "add r1, r4, #0\n\t"
        "strh r1, [r0]\n\t"
        "ldr r0, 27f\n\t"
        "sub r0, r0, r7\n\t"
        "add r0, #2\n\t"
        "lsr r1, r0, #0x1f\n\t"
        "add r0, r0, r1\n\t"
        "asr r0, r0, #1\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r0, r2\n\t"
        "ldr r2, 28f\n\t"
        "b 41f\n\t"
        ".align 2, 0\n\t"
        "25: .4byte gUnknown_03001630\n\t"
        "26: .4byte 0x0000E082\n\t"
        "27: .4byte gStaticData_0803A884\n\t"
        "28: .4byte 0x0000BAFF\n\t"
        "29:\n\t"
        "add r0, r6, #0\n\t"
        "add r0, #0x52\n\t"
        "ldrb r0, [r0]\n\t"
        "str r0, [sp, #0x20]\n\t"
        "ldr r3, 30f\n\t"
        "ldr r2, [r3]\n\t"
        "ldr r0, 31f\n\t"
        "sub r0, r0, r7\n\t"
        "add r0, #2\n\t"
        "lsr r1, r0, #0x1f\n\t"
        "add r0, r0, r1\n\t"
        "asr r0, r0, #1\n\t"
        "ldr r2, [r2, #0x44]\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r0, r2\n\t"
        "ldr r4, 32f\n\t"
        "add r1, r4, #0\n\t"
        "strh r1, [r0]\n\t"
        "ldr r0, 33f\n\t"
        "sub r0, r0, r7\n\t"
        "add r0, #2\n\t"
        "lsr r1, r0, #0x1f\n\t"
        "add r0, r0, r1\n\t"
        "asr r0, r0, #1\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r0, r2\n\t"
        "ldr r2, 34f\n\t"
        "b 41f\n\t"
        ".align 2, 0\n\t"
        "30: .4byte gUnknown_03001630\n\t"
        "31: .4byte gStaticData_0803A8B4\n\t"
        "32: .4byte 0x0000E082\n\t"
        "33: .4byte gStaticData_0803A8C4\n\t"
        "34: .4byte 0x0000BAFF\n\t"
        "35:\n\t"
        "ldr r2, [r6, #0x3c]\n\t"
        "ldrb r1, [r6, #0x10]\n\t"
        "lsl r0, r1, #3\n\t"
        "sub r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "add r2, #0x14\n\t"
        "add r2, r2, r0\n\t"
        "ldr r3, [r2]\n\t"
        "lsl r0, r3, #0xb\n\t"
        "str r0, [sp, #0xc]\n\t"
        "ldrb r0, [r6, #0xd]\n\t"
        "cmp r0, #0\n\t"
        "beq 39f\n\t"
        "mov r0, #0\n\t"
        "str r0, [sp, #0x20]\n\t"
        "ldr r3, 36f\n\t"
        "ldr r1, [r3]\n\t"
        "lsr r0, r5, #0x1f\n\t"
        "add r0, r5, r0\n\t"
        "asr r0, r0, #1\n\t"
        "ldr r2, [r1, #0x44]\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r0, r2\n\t"
        "ldr r4, 37f\n\t"
        "add r1, r4, #0\n\t"
        "strh r1, [r0]\n\t"
        "ldr r0, 38f\n\t"
        "b 40f\n\t"
        ".align 2, 0\n\t"
        "36: .4byte gUnknown_03001630\n\t"
        "37: .4byte 0x0000E042\n\t"
        "38: .4byte gStaticData_0803A884\n\t"
        "39:\n\t"
        "mov r0, #1\n\t"
        "str r0, [sp, #0x20]\n\t"
        "ldr r3, 42f\n\t"
        "ldr r2, [r3]\n\t"
        "ldr r0, 43f\n\t"
        "sub r0, r0, r7\n\t"
        "add r0, #2\n\t"
        "lsr r1, r0, #0x1f\n\t"
        "add r0, r0, r1\n\t"
        "asr r0, r0, #1\n\t"
        "ldr r2, [r2, #0x44]\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r0, r2\n\t"
        "ldr r4, 44f\n\t"
        "add r1, r4, #0\n\t"
        "strh r1, [r0]\n\t"
        "ldr r0, 45f\n\t"
        "40:\n\t"
        "sub r0, r0, r7\n\t"
        "add r0, #2\n\t"
        "lsr r1, r0, #0x1f\n\t"
        "add r0, r0, r1\n\t"
        "asr r0, r0, #1\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r0, r2\n\t"
        "ldr r2, 46f\n\t"
        "41:\n\t"
        "add r1, r2, #0\n\t"
        "strh r1, [r0]\n\t"
        "mov r4, sp\n\t"
        "ldr r3, [r3]\n\t"
        "ldr r3, [r3, #0x44]\n\t"
        "add r1, r3, #0\n\t"
        "add r0, r4, #0\n\t"
        "mov r2, pc\n\t"
        "add r2, #5\n\t"
        "mov lr, r2\n\t"
        "bx r1\n\t"
        ".thumb_func\n\t"
        ".global sub_8039E50\n\t"
        "sub_8039E50:\n\t"
        "nop\n\t"
        "ldr r0, [r6, #4]\n\t"
        "ldrh r2, [r0, #4]\n\t"
        "ldr r0, [sp, #20]\n\t"
        "cmp r0, r2\n\t"
        "beq 56f\n\t"
        "ldr r3, [sp, #88]\n\t"
        "cmp r3, #0\n\t"
        "beq 50f\n\t"
        "ldr r3, [r6, #60]\n\t"
        "ldrb r1, [r6, #16]\n\t"
        "lsl r0, r1, #3\n\t"
        "sub r0, r0, r1\n\t"
        "lsl r2, r0, #2\n\t"
        "add r0, r3, r2\n\t"
        "ldrb r0, [r0, #13]\n\t"
        "cmp r0, #0\n\t"
        "beq 49f\n\t"
        "mov r0, #17\n\t"
        "ldrsb r0, [r6, r0]\n\t"
        "ldrb r2, [r6, #17]\n\t"
        "cmp r0, #0\n\t"
        "ble 47f\n\t"
        "mov r4, sl\n\t"
        "lsl r1, r4, #1\n\t"
        "ldr r0, [sp, #8]\n\t"
        "sub r0, r0, r1\n\t"
        "b 48f\n\t"
        "42: .4byte gUnknown_03001630\n\t"
        "43: .4byte gStaticData_0803A8B4\n\t"
        "44: .4byte 0x0000E042\n\t"
        "45: .4byte gStaticData_0803A8C4\n\t"
        "46: .4byte 0x0000CAFF\n\t"
        "47:\n\t"
        "mov r0, sl\n\t"
        "lsl r1, r0, #1\n\t"
        "ldr r0, [sp, #8]\n\t"
        "add r0, r0, r1\n\t"
        "48:\n\t"
        "str r0, [sp, #8]\n\t"
        "mvn r0, r2\n\t"
        "strb r0, [r6, #17]\n\t"
        "b 55f\n\t"
        "49:\n\t"
        "add r1, r3, #0\n\t"
        "add r1, #24\n\t"
        "add r1, r1, r2\n\t"
        "add r0, r3, #0\n\t"
        "add r0, #20\n\t"
        "add r0, r0, r2\n\t"
        "ldr r1, [r1, #0]\n\t"
        "ldr r0, [r0, #0]\n\t"
        "sub r1, r1, r0\n\t"
        "b 51f\n\t"
        "50:\n\t"
        "ldrb r4, [r6, #18]\n\t"
        "cmp r4, #0\n\t"
        "beq 52f\n\t"
        "ldr r2, [r6, #60]\n\t"
        "ldrb r1, [r6, #16]\n\t"
        "lsl r0, r1, #3\n\t"
        "sub r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "add r2, #28\n\t"
        "add r2, r2, r0\n\t"
        "ldr r1, [r2, #0]\n\t"
        "51:\n\t"
        "lsl r1, r1, #11\n\t"
        "ldr r0, [sp, #8]\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [sp, #8]\n\t"
        "b 55f\n\t"
        "52:\n\t"
        "ldrb r0, [r6, #13]\n\t"
        "cmp r0, #0\n\t"
        "beq 53f\n\t"
        "ldr r0, [sp, #20]\n\t"
        "lsl r0, r0, #1\n\t"
        "ldr r1, [sp, #80]\n\t"
        "add r0, r1, r0\n\t"
        "ldr r1, [sp, #20]\n\t"
        "sub r1, r2, r1\n\t"
        "add r1, #1\n\t"
        "lsl r1, r1, #1\n\t"
        "bl sub_8037F3C\n\t"
        "53:\n\t"
        "ldr r0, 54f\n\t"
        "strh r0, [r6, #42]\n\t"
        "strh r4, [r6, #44]\n\t"
        "mov r0, #128\n\t"
        "lsl r0, r0, #24\n\t"
        "str r0, [r6, #76]\n\t"
        "b 56f\n\t"
        "54: .4byte 0x8ad0\n\t"
        "55:\n\t"
        "ldr r1, [r6, #4]\n\t"
        "ldr r0, [sp, #20]\n\t"
        "ldrh r1, [r1, #4]\n\t"
        "cmp r0, r1\n\t"
        "bcs 56f\n\t"
        "b 19b\n\t"
        "56:\n\t"
        "ldr r0, [sp, #8]\n\t"
        "str r0, [r6, #0x44]\n\t"
        "mov r0, #1\n\t"
        "57:\n\t"
        "add sp, #0x5c\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n\t"
    );
}
#endif /* NON_MATCHING */
