#include "core.h"
#include "action_obj.h"

/* Part of GitHub issue #16's remainder (0x08011BD4-0x08012D24): two
 * gStaticData_0816BF20 action-table helpers for the player/action object
 * (include/action_obj.h, the same object issue #17's actor_part_12fbc.c
 * handlers use). Not ROM-adjacent to actor_part80.c's matched
 * `sub_8012A7C` (this file starts right where that one ends, at
 * 0x08012AF4).
 *
 * Built with old_agbcc (Makefile OLD_AGBCC_OBJS) since the issue #15/#16
 * NAKED retry (docs/matching/issue-15-16-naked-retry.md): `sub_8012D24`
 * matches as plain C under it. `sub_8012AF4` is still a NAKED
 * transcription; its C draft sits under NON_MATCHING. */

asm(".set _call_via_r2, sub_803AD80\n"
    ".set _call_via_r3, sub_803AD84\n");

/* One 12-byte gStaticData_0816B304 animation parameter record. */
struct anim_rec
{
    s32 a;
    s32 b;
    s32 c;
};

extern u32 gUnknown_030007E0;
extern void *gUnknown_030012BC;
extern void *gUnknown_030012C0;
extern struct act_part *gUnknown_030012D8;
extern void *gUnknown_03001304;
extern struct anim_rec gStaticData_0816B304[];
extern void PlaySfx(void *ctx, s32 sfxId, s32 volume);
extern void sub_80019A8(void *ctx, u32 id);
extern s32 sub_80008FC(s32 a, s32 b);
extern u8 sub_8000760(void *pad);
extern void sub_8009EA8(struct act_part *p, s32 x, s32 y);
extern u8 sub_8012A7C(struct act *self);
extern void sub_80122CC(struct act *self);
extern void sub_8015398(struct act *self);
extern void sub_8015460(struct act *self);
extern u8 sub_80231C4(void *self);

/* Trio stores as in actor_part_12fbc.c: as inline parameters, old_agbcc
 * materializes the values before the stores. */
static inline void ActTrio27(struct act *self, s32 cur, s32 flag, s32 next)
{
    self->next31 = cur;
    self->flag2F = flag;
    self->next27 = next;
}

static inline void ActHold27P(struct act *self, u8 *slot, s32 next)
{
    self->next31 = 1;
    self->flag2F = 1;
    *slot = next;
}

/* `part->unk_100` read through an offset parameter: that keeps old_agbcc
 * from reusing an earlier 0x100 constant for the field address, which
 * the ROM rematerializes. */
static inline u8 PartByte(struct act_part *part, s32 offset)
{
    return *((u8 *)part + offset);
}

/* Runs a "part" OAM-visibility/priority housekeeping pass. If the
 * player's `+0x103` flag or `+0x102` byte just changed and `part+0x68`
 * is busy (`==8`), nudges the player's saved-position word by a fixed
 * delta and calls `sub_8009EA8`. If `self+8==0`, and the player isn't
 * already in a matching state, plays a fixed sound and fires the
 * `+0x50`/`+0x54` trampoline pair with id `0x12`. Then, keyed on
 * `self+0x2f`'s value (whether `1` or something else), either sets it
 * from the player's `+0x60`/`+0x100` state or reads it as-is; when it's
 * `1`, looks up a per-tag record in `gStaticData_0816B304` (indexed
 * `(*(self+4))[tag]`, `tag = self+0x27`), copies a 12-byte stretch of
 * it onto the stack, optionally rescales two of its three fields
 * (halving one, doubling the other) via `sub_80008FC` when the part is
 * busy and the player is active, special-cases tag `0x1e` to zero the
 * record and rescale differently, then fires a `sub_803AD84` trampoline
 * (one of two field-pairs depending on `self+0x30`) with the stack
 * record as its payload and clears the flag. Finally, on `self+0x30==1`,
 * repeats a near-identical stack-record/rescale/trampoline sequence
 * keyed on `self+0x28`'s tag against the same table, then clears
 * `self+0x30`. */
#if NON_MATCHING
/* Near miss under old_agbcc: same instructions and control flow, but the
 * register allocation differs throughout (the ROM puts short-lived
 * constants in r5/r6 and keeps the +0x27 slot in r8). */
void sub_8012AF4(struct act *self)
{
    struct anim_rec rec;
    struct act_part *p;
    u8 *flag;

    p = gUnknown_030012D8;
    if (p->unk_102 == 0) {
        if (p->unk_103 == 0)
            goto skip;
    }
    {
        if (self->part->contact == 8) {
            if (p->unk_102)
                p->x -= 0x100;
            else if (p->unk_103)
                p->x += 0x100;
            sub_8009EA8(gUnknown_030012D8, gUnknown_030012D8->x, gUnknown_030012D8->y);
        }
    }
skip:
    if (self->state == 0) {
        p = gUnknown_030012D8;
        if (p->unk_60 == 0 && p->bank->unk_0A != 0x12 && self->unk_33 == 0) {
            sub_80019A8(gUnknown_030012BC, 0x36);
            ACT_CALL2(self, m50, gUnknown_030012D8, 0x12);
        }
    }
    if (self->state == 0 || self->state == 0x11) {
        p = gUnknown_030012D8;
        flag = &self->flag2F;
        if (p->unk_60 != 0 && p->unk_100 == 0) {
            self->next31 = 0;
            *flag = 1;
            self->next27 = 0;
        }
    } else {
        flag = &self->flag2F;
    }
    {
        u8 f = *flag;

        if (f == 1) {
            u8 *tag = &self->next27;

            rec = gStaticData_0816B304[(*self->anims)[*tag].first];
            p = gUnknown_030012D8;
            if (p->unk_100 && self->part->contact == 8 && p->unk_60 != 0) {
                self->next31 = f;
                rec.c = sub_80008FC(rec.c, 0x180);
                rec.b /= 2;
            }
            if (*tag == 0x1E) {
                self->next31 = 0;
                if (gUnknown_030012D8->unk_100) {
                    rec.b = sub_80008FC(rec.b, 0x200);
                    rec.a = sub_80008FC(rec.a, 0x180);
                }
            }
            if (self->next31)
                ACT_CALL2(self, m38, self->part, &rec);
            else
                ACT_CALL2(self, m28, self->part, &rec);
            *flag = 0;
        }
    }
    {
        u8 *f30 = &self->flag30;

        if (*f30 == 1) {
            rec = gStaticData_0816B304[(*self->anims)[self->next28].second];
            if (self->next32)
                ACT_CALL2(self, m40, self->part, &rec);
            else
                ACT_CALL2(self, m30, self->part, &rec);
            *f30 = 0;
        }
    }
}
#else
NAKED void sub_8012AF4(void *selfArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "sub sp, #0xc\n\t"
        "add r4, r0, #0\n\t"
        "ldr r0, 40f\n\t"
        "ldr r2, [r0]\n\t"
        "mov r3, #0x81\n\t"
        "lsl r3, r3, #1\n\t"
        "add r1, r2, r3\n\t"
        "ldrb r1, [r1]\n\t"
        "add r3, r0, #0\n\t"
        "cmp r1, #0\n\t"
        "bne 1f\n\t"
        "ldr r5, 41f\n\t"
        "add r0, r2, r5\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 5f\n\t"
    "1:\n\t"
        "ldr r0, [r4, #0x10]\n\t"
        "add r0, #0x68\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #8\n\t"
        "bne 5f\n\t"
        "cmp r1, #0\n\t"
        "beq 2f\n\t"
        "ldr r0, [r2]\n\t"
        "ldr r6, 42f\n\t"
        "add r0, r0, r6\n\t"
        "b 3f\n\t"
        ".align 2, 0\n"
    "40: .4byte gUnknown_030012D8\n"
    "41: .4byte 0x00000103\n"
    "42: .4byte 0xFFFFFF00\n"
    "2:\n\t"
        "ldr r1, 43f\n\t"
        "add r0, r2, r1\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 4f\n\t"
        "ldr r0, [r2]\n\t"
        "mov r5, #0x80\n\t"
        "lsl r5, r5, #1\n\t"
        "add r0, r0, r5\n\t"
    "3:\n\t"
        "str r0, [r2]\n\t"
    "4:\n\t"
        "ldr r0, [r3]\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r2, [r0, #4]\n\t"
        "bl sub_8009EA8\n\t"
    "5:\n\t"
        "ldr r0, [r4, #8]\n\t"
        "cmp r0, #0\n\t"
        "bne 7f\n\t"
        "ldr r5, 44f\n\t"
        "ldr r1, [r5]\n\t"
        "ldr r0, [r1, #0x60]\n\t"
        "cmp r0, #0\n\t"
        "bne 6f\n\t"
        "ldr r0, [r1, #0x20]\n\t"
        "ldrh r0, [r0, #0xa]\n\t"
        "cmp r0, #0x12\n\t"
        "beq 6f\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x33\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 6f\n\t"
        "ldr r0, 45f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #0x36\n\t"
        "bl sub_80019A8\n\t"
        "ldr r2, [r4, #0xc]\n\t"
        "add r2, #0x50\n\t"
        "mov r6, #0\n\t"
        "ldrsh r0, [r2, r6]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r5]\n\t"
        "ldr r3, [r2, #4]\n\t"
        "mov r2, #0x12\n\t"
        "bl sub_803AD84\n\t"
    "6:\n\t"
        "ldr r0, [r4, #8]\n\t"
        "cmp r0, #0\n\t"
        "beq 8f\n\t"
    "7:\n\t"
        "add r7, r4, #0\n\t"
        "add r7, #0x2f\n\t"
        "cmp r0, #0x11\n\t"
        "bne 9f\n\t"
    "8:\n\t"
        "ldr r0, 44f\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r0, [r1, #0x60]\n\t"
        "add r7, r4, #0\n\t"
        "add r7, #0x2f\n\t"
        "cmp r0, #0\n\t"
        "beq 9f\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "add r0, r1, r2\n\t"
        "ldrb r1, [r0]\n\t"
        "cmp r1, #0\n\t"
        "bne 9f\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x31\n\t"
        "strb r1, [r0]\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r7]\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x27\n\t"
        "strb r1, [r0]\n\t"
    "9:\n\t"
        "ldrb r3, [r7]\n\t"
        "mov ip, r3\n\t"
        "cmp r3, #1\n\t"
        "bne 14f\n\t"
        "ldr r0, [r4, #4]\n\t"
        "add r3, r4, #0\n\t"
        "add r3, #0x27\n\t"
        "ldr r1, [r0]\n\t"
        "ldrb r5, [r3]\n\t"
        "lsl r0, r5, #3\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, [r0]\n\t"
        "lsl r0, r1, #1\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r2, 46f\n\t"
        "mov r1, sp\n\t"
        "add r0, r0, r2\n\t"
        "ldm r0!, {r2, r5, r6}\n\t"
        "stm r1!, {r2, r5, r6}\n\t"
        "ldr r0, 44f\n\t"
        "ldr r1, [r0]\n\t"
        "mov r6, #0x80\n\t"
        "lsl r6, r6, #1\n\t"
        "add r0, r1, r6\n\t"
        "ldrb r0, [r0]\n\t"
        "add r5, r4, #0\n\t"
        "add r5, #0x31\n\t"
        "mov r8, r3\n\t"
        "cmp r0, #0\n\t"
        "beq 10f\n\t"
        "ldr r0, [r4, #0x10]\n\t"
        "add r0, #0x68\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #8\n\t"
        "bne 10f\n\t"
        "ldr r0, [r1, #0x60]\n\t"
        "cmp r0, #0\n\t"
        "beq 10f\n\t"
        "mov r0, ip\n\t"
        "strb r0, [r5]\n\t"
        "ldr r0, [sp, #8]\n\t"
        "mov r1, #0xc0\n\t"
        "lsl r1, r1, #1\n\t"
        "bl sub_80008FC\n\t"
        "str r0, [sp, #8]\n\t"
        "ldr r0, [sp, #4]\n\t"
        "lsr r1, r0, #0x1f\n\t"
        "add r0, r0, r1\n\t"
        "asr r0, r0, #1\n\t"
        "str r0, [sp, #4]\n\t"
    "10:\n\t"
        "mov r1, r8\n\t"
        "ldrb r1, [r1]\n\t"
        "cmp r1, #0x1e\n\t"
        "bne 11f\n\t"
        "mov r0, #0\n\t"
        "strb r0, [r5]\n\t"
        "ldr r0, 44f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "add r0, r0, r2\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 11f\n\t"
        "ldr r0, [sp, #4]\n\t"
        "mov r1, #0x80\n\t"
        "lsl r1, r1, #2\n\t"
        "bl sub_80008FC\n\t"
        "str r0, [sp, #4]\n\t"
        "ldr r0, [sp]\n\t"
        "mov r1, #0xc0\n\t"
        "lsl r1, r1, #1\n\t"
        "bl sub_80008FC\n\t"
        "str r0, [sp]\n\t"
    "11:\n\t"
        "ldrb r0, [r5]\n\t"
        "cmp r0, #0\n\t"
        "beq 12f\n\t"
        "ldr r2, [r4, #0xc]\n\t"
        "mov r3, #0x38\n\t"
        "ldrsh r0, [r2, r3]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r4, #0x10]\n\t"
        "ldr r3, [r2, #0x3c]\n\t"
        "mov r2, sp\n\t"
        "bl sub_803AD84\n\t"
        "b 13f\n\t"
        ".align 2, 0\n"
    "43: .4byte 0x00000103\n"
    "44: .4byte gUnknown_030012D8\n"
    "45: .4byte gUnknown_030012BC\n"
    "46: .4byte gStaticData_0816B304\n"
    "12:\n\t"
        "ldr r2, [r4, #0xc]\n\t"
        "mov r5, #0x28\n\t"
        "ldrsh r0, [r2, r5]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r4, #0x10]\n\t"
        "ldr r3, [r2, #0x2c]\n\t"
        "mov r2, sp\n\t"
        "bl sub_803AD84\n\t"
    "13:\n\t"
        "mov r0, #0\n\t"
        "strb r0, [r7]\n\t"
    "14:\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x30\n\t"
        "add r5, r0, #0\n\t"
        "ldrb r6, [r5]\n\t"
        "cmp r6, #1\n\t"
        "bne 17f\n\t"
        "ldr r0, [r4, #4]\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x28\n\t"
        "ldr r2, [r0]\n\t"
        "ldrb r1, [r1]\n\t"
        "lsl r0, r1, #3\n\t"
        "add r0, r0, r2\n\t"
        "ldr r1, [r0, #4]\n\t"
        "lsl r0, r1, #1\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r2, 47f\n\t"
        "mov r1, sp\n\t"
        "add r0, r0, r2\n\t"
        "ldm r0!, {r2, r3, r6}\n\t"
        "stm r1!, {r2, r3, r6}\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x32\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 15f\n\t"
        "ldr r2, [r4, #0xc]\n\t"
        "add r2, #0x40\n\t"
        "mov r1, #0\n\t"
        "ldrsh r0, [r2, r1]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r4, #0x10]\n\t"
        "ldr r3, [r2, #4]\n\t"
        "mov r2, sp\n\t"
        "bl sub_803AD84\n\t"
        "b 16f\n\t"
        ".align 2, 0\n"
    "47: .4byte gStaticData_0816B304\n"
    "15:\n\t"
        "ldr r2, [r4, #0xc]\n\t"
        "mov r3, #0x30\n\t"
        "ldrsh r0, [r2, r3]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r4, #0x10]\n\t"
        "ldr r3, [r2, #0x34]\n\t"
        "mov r2, sp\n\t"
        "bl sub_803AD84\n\t"
    "16:\n\t"
        "mov r0, #0\n\t"
        "strb r0, [r5]\n\t"
    "17:\n\t"
        "add sp, #0xc\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0"
    );
}
#endif

/* A further sibling/callee of the same action-table family. Reads the
 * D-pad (`sub_8000760`) and ticks `self+0x25` down on release. If
 * `part+0x38` is set, fires the `+0x50`/`+0x54` trampoline (id `0x12`)
 * and clears `part+0x33`. Bumps `self+0x1c`'s frame counter; while the
 * player's type is `0x12` and `+0x30 == 0`, once the counter passes one
 * of two thresholds (`0x708`, then a further gated pair keyed on a
 * `self+4`-relative negative-distance test), fires further `+0x50`/
 * `+0x54` trampoline calls with escalating ids and resets the counter,
 * setting `self+0x33`. Bails early if `sub_8012A7C(self)` reports busy.
 * Otherwise dispatches the input snapshot's low bits: bit 0 plays a
 * fixed sound and fires two trampoline pairs, bit 1 tail-calls
 * `sub_8015398`, bit `0x80` (high byte) fires a different trampoline
 * pair - all converging on `sub_80122CC`. A further branch (input byte
 * unset, `self+0x25==0`) reads `self+8`'s snapshot value against `2`/
 * `8`-range checks to gate a `sub_80231C4`-confirmed trampoline call
 * (id `4`/`0x18`) or fall through to `sub_8015460`/a final `+0x20`/
 * `+0x24` trampoline pair, each path ending in `sub_80122CC`.
 *
 * Matched under old_agbcc. The pad object is loaded before the input word
 * is spilled (its argument is read first); the held-0x100 test's result
 * is what +0x29 is cleared with; the D-pad `else` part sits after the
 * first sub_80122CC tail, reached by a goto, as in the ROM's layout; and
 * the 3..8 range case comes before case 2. */
void sub_8012D24(struct act *self)
{
    void *pad = gUnknown_03001304;
    u32 in = gUnknown_030007E0;
    u8 dir = sub_8000760(pad);
    s32 frames;
    struct act_part *part;

    if (self->unk_24[1] != 0) {
        self->unk_24[1]--;
        if (dir == 0)
            self->unk_24[1] = dir;
    }
    part = self->part;
    if (part->animDone) {
        ACT_CALL2(self, m50, part, 0x12);
        self->unk_33 = 0;
    }
    frames = ++self->frames;
    part = self->part;
    if (part->tag == 0x12 && part->frame == 0) {
        if (frames > 0x708) {
            ACT_CALL2(self, m50, part, 0x1A);
            self->frames = 0;
        } else if (frames >= 0x49D && frames <= 0x4C3) {
            ACT_CALL2(self, m50, part, 5);
            self->frames = 0x4C4;
        } else if (frames >= 0x1E1 && frames <= 0x207) {
            ACT_CALL2(self, m50, part, 0xE);
            self->frames = 0x208;
        } else {
            goto skip;
        }
        self->unk_33 = 1;
    }
skip:
    {
        u8 busy = sub_8012A7C(self);
        u16 held;

        if (busy)
            return;
        if (INPUT_PRESSED(in) & 1) {
            PlaySfx(gUnknown_030012BC, 0xD, 0x100);
            ACT_CALL1(self, m20, 5);
            ACT_CALL2(self, m50, self->part, 0x13);
            self->frame = busy;
            ActSetNext(self, 7);
        } else {
            u16 alt = INPUT_PRESSED(in) & 2;

            if (alt) {
                sub_8015398(self);
            } else {
                if ((held = INPUT_HELD(in) & 0x100) == 0)
                    goto other;
                ACT_CALL1(self, m20, 0x10);
                ACT_CALL2(self, m50, self->part, 3);
                self->frames = alt;
            }
        }
        sub_80122CC(self);
        return;
    other:
        self->unk_29 = held;
        if (dir == 0) {
            struct act_part *p = self->part;
            u8 *slot;

            if (PartByte(p, 0x100) && *(slot = &self->next27) != 0x1F && p->unk_60 != 0)
                ActHold27P(self, slot, 0x1F);
        } else {
            u8 wait = self->unk_24[1];

            if (wait == 0) {
                switch (dir) {
                case 3 ... 8:
                    if ((INPUT_HELD(in) & 0x200) && sub_80231C4(gUnknown_030012C0)) {
                        self->unk_29 = 1;
                        ACT_CALL1(self, m20, 4);
                        ACT_CALL2(self, m50, self->part, 0x18);
                        ActTrio27(self, wait, 1, 0x1B);
                    } else {
                        sub_8015460(self);
                    }
                    break;
                case 2:
                    ACT_CALL1(self, m20, 0x10);
                    ACT_CALL2(self, m50, self->part, 3);
                    self->frames = wait;
                    break;
                }
            }
        }
        sub_80122CC(self);
    }
}
/* Trailing byte count isn't a multiple of 4 - pad with zeros, not a nop. */
asm(".align 2, 0");
