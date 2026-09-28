#include "core.h"
#include "action_obj.h"

/* Part of GitHub issue #16's remainder (0x08011BD4-0x08012D24): three
 * gStaticData_0816BF20 action-table helpers for the player/action object
 * (include/action_obj.h). Not ROM-adjacent to actor_part79.c/
 * actor_part80.c (the still-NAKED `sub_8011BD4` sits before it,
 * `sub_8012AF4` after) - see docs/matching/issue-16-actor-12420.md.
 *
 * Built with old_agbcc (Makefile OLD_AGBCC_OBJS) since the issue #15/#16
 * NAKED retry (docs/matching/issue-15-16-naked-retry.md): `sub_801283C`
 * matches as plain C under it. `sub_8012420` and `sub_8012694` are still
 * NAKED transcriptions with their C drafts under NON_MATCHING. */

asm(".set _call_via_r2, sub_803AD80\n"
    ".set _call_via_r3, sub_803AD84\n"
    ".set _call_via_r4, sub_803AD88\n");

/* A gcc 2.x pointer-to-member-function record (gStaticData_0816BF20's
 * per-state handlers): `index > 0` selects virtual slot `index - 1` of the
 * method table at `this + vtableOffset`, otherwise `fn` is called. */
struct act_pmf
{
    s16 thisOffset;
    s16 index;
    union {
        s16 vtableOffset;
        void *fn;
    } u;
};

struct cam_target
{
    u8 unk_00[0x14];
    s32 y;                 // 0x14 (Q0)
};

struct cam
{
    u8 unk_00[0x10];
    struct cam_target *target; // 0x10
};

typedef void (*act_fn3)(void *self, s32 a, s32 b, s32 c);

extern u32 gUnknown_030007E0;
extern void *gUnknown_030012BC;
extern void *gUnknown_030012C0;
extern struct act_part *gUnknown_030012D8;
extern void *gUnknown_03001304;
extern struct cam *gUnknown_03001308;
extern struct act_pmf gStaticData_0816BF20[];
extern void PlaySfx(void *ctx, s32 sfxId, s32 volume);
extern u8 sub_8000760(void *pad);
extern void sub_8012238(struct act *self);
extern void sub_8012AF4(struct act *self);
extern void sub_80138E8(struct act *self);
extern void sub_80151C8(struct act *self);
extern u8 sub_80231CC(void *self);
extern void sub_80231EC(void *self, s32 arg);

/* Trio stores as in actor_part_12fbc.c: as inline parameters, old_agbcc
 * materializes the values before the stores. */
static inline void ActTrio27(struct act *self, s32 cur, s32 flag, s32 next)
{
    self->next31 = cur;
    self->flag2F = flag;
    self->next27 = next;
}

static inline void ActTrio28(struct act *self, s32 cur, s32 flag, s32 next)
{
    self->next32 = cur;
    self->flag30 = flag;
    self->next28 = next;
}

static inline void ActQueue27(struct act *self, s32 cur, s32 next)
{
    self->next31 = cur;
    self->flag2F = 1;
    self->next27 = next;
}

static inline void ActSet27(struct act *self, s32 next)
{
    self->next31 = 0;
    self->flag2F = 1;
    self->next27 = next;
}

static inline void ActSet28(struct act *self, s32 next)
{
    self->next32 = 0;
    self->flag30 = 1;
    self->next28 = next;
}

static inline void ActSetNext27P(struct act *self, u8 *slot, s32 next)
{
    self->next31 = 0;
    self->flag2F = 1;
    *slot = next;
}

static inline void ActHold27P(struct act *self, u8 *slot, s32 next)
{
    self->next31 = 1;
    self->flag2F = 1;
    *slot = next;
}

/* Byte fields of the part read/written through an offset parameter, which
 * keeps old_agbcc from reusing an earlier 0x100 constant for the address
 * (the ROM rematerializes it). */
static inline u8 PartByte(struct act_part *part, s32 offset)
{
    return *((u8 *)part + offset);
}

static inline u8 *PartBytePtr(struct act_part *part, s32 offset)
{
    return (u8 *)part + offset;
}

/* Clears `self+0x34`'s reentrancy flag once `gUnknown_030007E0`'s bit
 * `0x100` clears. If `part+0x100` (the part's own "active" flag)
 * changed since last frame, re-runs `sub_8012238`. Then, using
 * `gUnknown_03001308`'s sub-object's `+0x14` Q8 field as a screen-space
 * anchor, checks `part->field_04` against two thresholds: past the near
 * one, resets `part`'s `+0x48`/`+0x4c`/`+0x50` velocity-target fields
 * (and `+0x60` unless still active); past the far one, additionally
 * clears `part+0x8c` and fires a state-close call (`sub_80231EC`) plus
 * `sub_803AD88` through the `self+0xc` manager's `+0x10`/`+0x14`
 * trampoline slot. Decrements `self+0x26` if set. While `self+0x2b`'s
 * countdown is running and `part+0x94 <= 1`, ticks it down and, on
 * reaching zero, resets the trio's first half (`+0x31`/`+0x2f`/`+0x27`/
 * `+0x2c`) if `+0x27` was already clear, then always clears the
 * player's `+0x90` byte. Looks up `self+8`'s type in
 * `gStaticData_0816BF20`'s 8-byte-per-slot table - a `{s16 baseOffset;
 * s16 count; s16 recordOffset; s32 fallback}` record - to build the
 * arguments for one `sub_803AD84` trampoline call. If `part+0x68` bit 3
 * got cleared this call and `self+0x28` is `4`/`5`, resets the trio's
 * second half; either way calls `sub_8012AF4`, then (unless `part+0xc`
 * bit 7 is set) resets `part`'s `+0x48`/`+0x4c`/`+0x50`/`+0x60` fields
 * again. Finally writes a small fixed value into `part+0xa` from a
 * second, 22-case jump table on the same type. */
#if NON_MATCHING
/* Near miss: same control flow and calls (including the gcc 2.x PMF call
 * through gStaticData_0816BF20), but the ROM keeps the method record in an
 * 8-byte stack slot and pushes an unused r7 (a DImode pair spilled after
 * allocation), and its temporaries land in r6 where old_agbcc picks
 * r0-r3. */
void sub_8012420(struct act *self)
{
    u32 in = gUnknown_030007E0;

    if (self->unk_34 != 0) {
        u16 held = in & 0x100;

        if (held == 0)
            self->unk_34 = held;
    }
    if (self->unk_2A[4] != PartByte(self->part, 0x100))
        sub_8012238(self);
    self->unk_2A[4] = PartByte(self->part, 0x100);
    {
        struct act_part *part = self->part;

        if (part->y > (gUnknown_03001308->target->y << 8) - 0x1400) {
            part->flags0C &= 0x7F;
            part = self->part;
            if (PartByte(part, 0x100) == 0)
                part->unk_60 = 0;
            part->unk_48 = 0;
            part->unk_4C = 0;
            part->unk_50 = 0;
            part = self->part;
            if (part->y > (gUnknown_03001308->target->y << 8) + 0x1400) {
                part->unk_8C = 0;
                sub_80231EC(gUnknown_030012C0, 0);
                {
                    struct act_method *m = &self->vt->m10;

                    ((act_fn3)m->fn)((u8 *)self + m->thisOffset, 0, 1, 0);
                }
            }
        }
    }
    if (self->unk_26)
        self->unk_26--;
    {
        u8 *timer = &self->unk_2A[1];
        u8 t = *timer;

        if (t != 0 && self->part->unk_94 <= 1) {
            u8 left = --*timer;

            if (left == 0) {
                u8 *slot = &self->next27;

                if (*slot == 0) {
                    u8 *queued = &self->unk_2A[2];

                    self->next31 = left;
                    self->flag2F = 1;
                    *slot = *queued;
                    *queued = left;
                }
                gUnknown_030012D8->unk_90 = left;
            }
        }
    }
    {
        struct act_method m;
        void (*fn)(void *);
        s32 index = gStaticData_0816BF20[self->state].index;
        s32 off;

        if (index > 0) {
            m = (*(struct act_method **)((u8 *)self + gStaticData_0816BF20[self->state].u.vtableOffset))[index - 1];
            fn = m.fn;
        } else {
            fn = gStaticData_0816BF20[self->state].u.fn;
        }
        off = gStaticData_0816BF20[self->state].thisOffset;
        {
            s32 d;

            if (index > 0)
                d = m.thisOffset + off;
            else
                d = off;
            fn((u8 *)self + d);
        }
    }
    self->part->contact &= 8;
    if (self->part->contact == 8) {
        u8 *slot = &self->next28;

        if (*slot == 4 || *slot == 5) {
            self->next32 = 0;
            self->flag30 = 1;
            *slot = 0;
        }
    }
    sub_8012AF4(self);
    {
        struct act_part *part = self->part;
        u32 top = part->flags0C >> 7;

        if (top == 0) {
            if (PartByte(part, 0x100) == 0)
                part->unk_60 = top;
            part->unk_48 = top;
            part->unk_4C = top;
            part->unk_50 = top;
        }
    }
    switch (self->state) {
    case 0xC:
        self->part->unk_0A[0] = 0x14;
        break;
    case 0xD:
    case 0xE:
    case 0xF:
    case 0x21:
        self->part->unk_0A[0] = 0x13;
        break;
    case 0x18:
        self->part->unk_0A[0] = 0x15;
        break;
    case 0x19:
        self->part->unk_0A[0] = 0x16;
        break;
    default:
        self->part->unk_0A[0] = 1;
        break;
    }
}
#else
NAKED void sub_8012420(void *selfArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "sub sp, #8\n\t"
        "add r5, r0, #0\n\t"
        "ldr r0, 40f\n\t"
        "ldr r1, [r0]\n\t"
        "add r2, r5, #0\n\t"
        "add r2, r2, #0x34\n\t"
        "ldrb r0, [r2]\n\t"
        "cmp r0, #0\n\t"
        "beq 1f\n\t"
        "mov r0, #0x80\n\t"
        "lsl r0, r0, #1\n\t"
        "and r1, r0\n\t"
        "lsl r0, r1, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "cmp r0, #0\n\t"
        "bne 1f\n\t"
        "strb r0, [r2]\n\t"
    "1:\n\t"
        "add r4, r5, #0\n\t"
        "add r4, r4, #0x2e\n\t"
        "ldr r0, [r5, #0x10]\n\t"
        "mov r1, #0x80\n\t"
        "lsl r1, r1, #1\n\t"
        "add r0, r0, r1\n\t"
        "ldrb r2, [r4]\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r2, r0\n\t"
        "beq 2f\n\t"
        "add r0, r5, #0\n\t"
        "bl sub_8012238\n\t"
    "2:\n\t"
        "ldr r0, [r5, #0x10]\n\t"
        "mov r3, #0x80\n\t"
        "lsl r3, r3, #1\n\t"
        "add r0, r0, r3\n\t"
        "ldrb r0, [r0]\n\t"
        "mov r3, #0\n\t"
        "strb r0, [r4]\n\t"
        "ldr r2, [r5, #0x10]\n\t"
        "ldr r1, [r2, #4]\n\t"
        "ldr r4, 41f\n\t"
        "ldr r0, [r4]\n\t"
        "ldr r0, [r0, #0x10]\n\t"
        "ldr r0, [r0, #0x14]\n\t"
        "lsl r0, r0, #8\n\t"
        "ldr r6, 42f\n\t"
        "add r0, r0, r6\n\t"
        "cmp r1, r0\n\t"
        "ble 4f\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r1, [r2, #0xc]\n\t"
        "and r0, r1\n\t"
        "strb r0, [r2, #0xc]\n\t"
        "ldr r1, [r5, #0x10]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "add r0, r1, r2\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
        "str r3, [r1, #0x60]\n\t"
    "3:\n\t"
        "str r3, [r1, #0x48]\n\t"
        "str r3, [r1, #0x4c]\n\t"
        "str r3, [r1, #0x50]\n\t"
        "ldr r2, [r5, #0x10]\n\t"
        "ldr r1, [r2, #4]\n\t"
        "ldr r0, [r4]\n\t"
        "ldr r0, [r0, #0x10]\n\t"
        "ldr r0, [r0, #0x14]\n\t"
        "lsl r0, r0, #8\n\t"
        "mov r4, #0xa0\n\t"
        "lsl r4, r4, #5\n\t"
        "add r0, r0, r4\n\t"
        "cmp r1, r0\n\t"
        "ble 4f\n\t"
        "add r0, r2, #0\n\t"
        "add r0, r0, #0x8c\n\t"
        "str r3, [r0]\n\t"
        "ldr r0, 43f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #0\n\t"
        "bl sub_80231EC\n\t"
        "ldr r1, [r5, #0xc]\n\t"
        "mov r6, #0x10\n\t"
        "ldrsh r0, [r1, r6]\n\t"
        "add r0, r5, r0\n\t"
        "ldr r4, [r1, #0x14]\n\t"
        "mov r1, #0\n\t"
        "mov r2, #1\n\t"
        "mov r3, #0\n\t"
        "bl sub_803AD88\n\t"
    "4:\n\t"
        "add r1, r5, #0\n\t"
        "add r1, r1, #0x26\n\t"
        "ldrb r0, [r1]\n\t"
        "cmp r0, #0\n\t"
        "beq 5f\n\t"
        "sub r0, #1\n\t"
        "strb r0, [r1]\n\t"
    "5:\n\t"
        "add r2, r5, #0\n\t"
        "add r2, r2, #0x2b\n\t"
        "ldrb r1, [r2]\n\t"
        "cmp r1, #0\n\t"
        "beq 7f\n\t"
        "ldr r0, [r5, #0x10]\n\t"
        "add r0, r0, #0x94\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #1\n\t"
        "bgt 7f\n\t"
        "sub r0, r1, #1\n\t"
        "strb r0, [r2]\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r4, r0, #0x18\n\t"
        "cmp r4, #0\n\t"
        "bne 7f\n\t"
        "mov r0, #0x27\n\t"
        "add r0, r0, r5\n\t"
        "mov ip, r0\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 6f\n\t"
        "add r0, r5, #0\n\t"
        "add r0, r0, #0x2c\n\t"
        "ldrb r2, [r0]\n\t"
        "add r1, r5, #0\n\t"
        "add r1, r1, #0x31\n\t"
        "strb r4, [r1]\n\t"
        "add r3, r5, #0\n\t"
        "add r3, r3, #0x2f\n\t"
        "mov r1, #1\n\t"
        "strb r1, [r3]\n\t"
        "mov r1, ip\n\t"
        "strb r2, [r1]\n\t"
        "strb r4, [r0]\n\t"
    "6:\n\t"
        "ldr r0, 44f\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, r0, #0x90\n\t"
        "strb r4, [r0]\n\t"
    "7:\n\t"
        "ldr r1, 45f\n\t"
        "ldr r0, [r5, #8]\n\t"
        "lsl r3, r0, #3\n\t"
        "add r0, r3, r1\n\t"
        "mov r4, #2\n\t"
        "ldrsh r2, [r0, r4]\n\t"
        "add r4, r1, #0\n\t"
        "cmp r2, #0\n\t"
        "ble 9f\n\t"
        "mov r6, #4\n\t"
        "ldrsh r0, [r0, r6]\n\t"
        "add r0, r5, r0\n\t"
        "ldr r1, [r0]\n\t"
        "lsl r0, r2, #3\n\t"
        "add r0, r0, r1\n\t"
        "add r3, r0, #0\n\t"
        "sub r3, #8\n\t"
        "ldr r0, [r3]\n\t"
        "ldr r1, [r3, #4]\n\t"
        "str r0, [sp]\n\t"
        "str r1, [sp, #4]\n\t"
        "ldr r3, [sp, #4]\n\t"
        "b 10f\n\t"
        ".align 2, 0\n"
    "40: .4byte gUnknown_030007E0\n"
    "41: .4byte gUnknown_03001308\n"
    "42: .4byte 0xFFFFEC00\n"
    "43: .4byte gUnknown_030012C0\n"
    "44: .4byte gUnknown_030012D8\n"
    "45: .4byte gStaticData_0816BF20\n"
    "9:\n\t"
        "add r0, r4, #4\n\t"
        "add r0, r3, r0\n\t"
        "ldr r3, [r0]\n\t"
    "10:\n\t"
        "ldr r0, [r5, #8]\n\t"
        "lsl r0, r0, #3\n\t"
        "add r0, r0, r4\n\t"
        "mov r4, #0\n\t"
        "ldrsh r1, [r0, r4]\n\t"
        "cmp r2, #0\n\t"
        "ble 12f\n\t"
        "ldr r6, [sp]\n\t"
        "lsl r0, r6, #0x10\n\t"
        "asr r0, r0, #0x10\n\t"
        "add r0, r0, r1\n\t"
        "b 13f\n\t"
    "12:\n\t"
        "add r0, r1, #0\n\t"
    "13:\n\t"
        "add r0, r5, r0\n\t"
        "bl sub_803AD84\n\t"
        "ldr r1, [r5, #0x10]\n\t"
        "add r1, r1, #0x68\n\t"
        "mov r0, #8\n\t"
        "ldrb r2, [r1]\n\t"
        "and r0, r2\n\t"
        "mov r3, #0\n\t"
        "strb r0, [r1]\n\t"
        "ldr r0, [r5, #0x10]\n\t"
        "add r0, r0, #0x68\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #8\n\t"
        "bne 14f\n\t"
        "add r2, r5, #0\n\t"
        "add r2, r2, #0x28\n\t"
        "ldrb r0, [r2]\n\t"
        "sub r0, #4\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "cmp r0, #1\n\t"
        "bhi 14f\n\t"
        "add r0, r5, #0\n\t"
        "add r0, r0, #0x32\n\t"
        "strb r3, [r0]\n\t"
        "add r1, r5, #0\n\t"
        "add r1, r1, #0x30\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r1]\n\t"
        "strb r3, [r2]\n\t"
    "14:\n\t"
        "add r0, r5, #0\n\t"
        "bl sub_8012AF4\n\t"
        "ldr r2, [r5, #0x10]\n\t"
        "ldrb r3, [r2, #0xc]\n\t"
        "lsr r1, r3, #7\n\t"
        "cmp r1, #0\n\t"
        "bne 16f\n\t"
        "mov r4, #0x80\n\t"
        "lsl r4, r4, #1\n\t"
        "add r0, r2, r4\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 15f\n\t"
        "str r1, [r2, #0x60]\n\t"
    "15:\n\t"
        "str r1, [r2, #0x48]\n\t"
        "str r1, [r2, #0x4c]\n\t"
        "str r1, [r2, #0x50]\n\t"
    "16:\n\t"
        "ldr r0, [r5, #8]\n\t"
        "sub r0, #0xc\n\t"
        "cmp r0, #0x15\n\t"
        "bhi 18f\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r1, 46f\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "mov pc, r0\n\t"
        ".align 2, 0\n"
    "46: .4byte 22f\n"
    "22:\n"
        ".4byte 19f\n"
        ".4byte 17f\n"
        ".4byte 17f\n"
        ".4byte 17f\n"
        ".4byte 18f\n"
        ".4byte 18f\n"
        ".4byte 18f\n"
        ".4byte 18f\n"
        ".4byte 18f\n"
        ".4byte 18f\n"
        ".4byte 18f\n"
        ".4byte 18f\n"
        ".4byte 20f\n"
        ".4byte 21f\n"
        ".4byte 18f\n"
        ".4byte 18f\n"
        ".4byte 18f\n"
        ".4byte 18f\n"
        ".4byte 18f\n"
        ".4byte 18f\n"
        ".4byte 18f\n"
        ".4byte 17f\n"
    "17:\n\t"
        "ldr r1, [r5, #0x10]\n\t"
        "mov r0, #0x13\n\t"
        "b 23f\n\t"
    "19:\n\t"
        "ldr r1, [r5, #0x10]\n\t"
        "mov r0, #0x14\n\t"
        "b 23f\n\t"
    "20:\n\t"
        "ldr r1, [r5, #0x10]\n\t"
        "mov r0, #0x15\n\t"
        "b 23f\n\t"
    "21:\n\t"
        "ldr r1, [r5, #0x10]\n\t"
        "mov r0, #0x16\n\t"
        "b 23f\n\t"
    "18:\n\t"
        "ldr r1, [r5, #0x10]\n\t"
        "mov r0, #1\n\t"
    "23:\n\t"
        "strb r0, [r1, #0xa]\n\t"
        "add sp, #8\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0"
    );
}

#endif

/* A helper of `sub_801283C` (below): if the input snapshot's D-pad bit
 * `1` is set, `self+0x18`'s counter is 0, `gUnknown_030012C0` passes
 * `sub_80231CC`, and a sub-object type of `6`/`0xb`/`0xc` (each with its
 * own extra `+0x30 >= 0` gate) matches, bumps `self+0x18`, fires the
 * `+0x50`/`+0x54` and `+0x20`/`+0x24` trampoline pairs with type-keyed
 * ids, resets the state/flag/table-index trio to a type-keyed value,
 * plays a fixed sound, and returns 1; otherwise returns 0. */
#if NON_MATCHING
/* Near miss: everything matches except register choice in the tag tests -
 * the ROM loads `self->part` into r0, keeps the copy in r2 and recomputes
 * the +0x2D address for each test; old_agbcc keeps the part in r1 and
 * reuses the address. */
u8 sub_8012694(struct act *self)
{
    u32 in = gUnknown_030007E0;
    u16 pressed;
    s32 one;

    if (self->state == 0xE)
        return 0;
    pressed = INPUT_PRESSED(in);
    one = 1;
    if (pressed & 1) {
        s32 frame = self->frame;

        if (frame == 0 && sub_80231CC(gUnknown_030012C0)) {
            struct act_part *part = self->part;

            if (part->tag == 6 && part->frame >= 0) {
                self->frame++;
                *PartBytePtr(gUnknown_030012D8, 0x100) = frame;
                ACT_CALL2(self, m50, self->part, 0x12);
                ACT_CALL1(self, m20, 9);
                ACT_CALL2(self, m50, self->part, 6);
                ActTrio27(self, frame, one, 0xD);
                ActTrio28(self, frame, one, 0xD);
                PlaySfx(gUnknown_030012BC, 0xC, 0x100);
                return 1;
            } else if (part->tag == 0xB && part->frame >= 0) {
                self->frame++;
                ACT_CALL1(self, m20, 0xB);
                ACT_CALL2(self, m50, self->part, 0xA);
                ActSet27(self, 0xE);
                ActSet28(self, 0xE);
                PlaySfx(gUnknown_030012BC, 0xC, 0x100);
                return 1;
            } else if (part->tag == 0xC) {
                self->frame++;
                ACT_CALL1(self, m20, 0xB);
                ACT_CALL2(self, m50, self->part, 0xA);
                ActSet27(self, 0xC);
                ActSet28(self, 0xC);
                PlaySfx(gUnknown_030012BC, 0xC, 0x100);
                return 1;
            }
        }
    }
    return 0;
}
#else
NAKED u8 sub_8012694(struct act *self)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "sub sp, #4\n\t"
        "add r4, r0, #0\n\t"
        "ldr r0, 40f\n\t"
        "ldr r0, [r0]\n\t"
        "str r0, [sp]\n\t"
        "ldr r0, [r4, #8]\n\t"
        "cmp r0, #0xe\n\t"
        "bne 1f\n\t"
        "b 9f\n\t"
    "1:\n\t"
        "mov r0, sp\n\t"
        "ldrh r1, [r0, #2]\n\t"
        "mov r6, #1\n\t"
        "mov r0, #1\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "bne 2f\n\t"
        "b 9f\n\t"
    "2:\n\t"
        "ldr r5, [r4, #0x18]\n\t"
        "cmp r5, #0\n\t"
        "beq 3f\n\t"
        "b 9f\n\t"
    "3:\n\t"
        "ldr r0, 41f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_80231CC\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne 4f\n\t"
        "b 9f\n\t"
    "4:\n\t"
        "ldr r0, [r4, #0x10]\n\t"
        "add r1, r0, #0\n\t"
        "add r1, #0x2d\n\t"
        "add r2, r0, #0\n\t"
        "ldrb r1, [r1]\n\t"
        "cmp r1, #6\n\t"
        "bne 5f\n\t"
        "ldr r0, [r2, #0x30]\n\t"
        "cmp r0, #0\n\t"
        "blt 5f\n\t"
        "ldr r0, [r4, #0x18]\n\t"
        "add r0, #1\n\t"
        "str r0, [r4, #0x18]\n\t"
        "ldr r0, 42f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #0x80\n\t"
        "lsl r1, r1, #1\n\t"
        "add r0, r0, r1\n\t"
        "strb r5, [r0]\n\t"
        "ldr r2, [r4, #0xc]\n\t"
        "add r2, #0x50\n\t"
        "mov r1, #0\n\t"
        "ldrsh r0, [r2, r1]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r4, #0x10]\n\t"
        "ldr r3, [r2, #4]\n\t"
        "mov r2, #0x12\n\t"
        "bl sub_803AD84\n\t"
        "ldr r1, [r4, #0xc]\n\t"
        "mov r2, #0x20\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r2, [r1, #0x24]\n\t"
        "mov r1, #9\n\t"
        "bl sub_803AD80\n\t"
        "ldr r2, [r4, #0xc]\n\t"
        "add r2, #0x50\n\t"
        "mov r1, #0\n\t"
        "ldrsh r0, [r2, r1]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r4, #0x10]\n\t"
        "ldr r3, [r2, #4]\n\t"
        "mov r2, #6\n\t"
        "bl sub_803AD84\n\t"
        "mov r1, #0xd\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x31\n\t"
        "strb r5, [r0]\n\t"
        "sub r0, #2\n\t"
        "strb r6, [r0]\n\t"
        "sub r0, #8\n\t"
        "strb r1, [r0]\n\t"
        "add r0, #0xb\n\t"
        "strb r5, [r0]\n\t"
        "sub r0, #2\n\t"
        "strb r6, [r0]\n\t"
        "sub r0, #8\n\t"
        "strb r1, [r0]\n\t"
        "b 7f\n\t"
        ".align 2, 0\n"
    "40: .4byte gUnknown_030007E0\n"
    "41: .4byte gUnknown_030012C0\n"
    "42: .4byte gUnknown_030012D8\n"
    "5:\n\t"
        "add r0, r2, #0\n\t"
        "add r0, #0x2d\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0xb\n\t"
        "bne 6f\n\t"
        "ldr r0, [r2, #0x30]\n\t"
        "cmp r0, #0\n\t"
        "blt 6f\n\t"
        "ldr r0, [r4, #0x18]\n\t"
        "add r0, #1\n\t"
        "str r0, [r4, #0x18]\n\t"
        "ldr r1, [r4, #0xc]\n\t"
        "mov r2, #0x20\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r2, [r1, #0x24]\n\t"
        "mov r1, #0xb\n\t"
        "bl sub_803AD80\n\t"
        "ldr r2, [r4, #0xc]\n\t"
        "add r2, #0x50\n\t"
        "mov r1, #0\n\t"
        "ldrsh r0, [r2, r1]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r4, #0x10]\n\t"
        "ldr r3, [r2, #4]\n\t"
        "mov r2, #0xa\n\t"
        "bl sub_803AD84\n\t"
        "mov r2, #0xe\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x31\n\t"
        "mov r0, #0\n\t"
        "strb r0, [r1]\n\t"
        "add r3, r4, #0\n\t"
        "add r3, #0x2f\n\t"
        "mov r1, #1\n\t"
        "strb r1, [r3]\n\t"
        "sub r3, #8\n\t"
        "strb r2, [r3]\n\t"
        "add r3, #0xb\n\t"
        "strb r0, [r3]\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x30\n\t"
        "strb r1, [r0]\n\t"
        "sub r0, #8\n\t"
        "strb r2, [r0]\n\t"
        "ldr r0, 43f\n\t"
        "ldr r0, [r0]\n\t"
        "add r2, #0xf2\n\t"
        "b 8f\n\t"
        ".align 2, 0\n"
    "43: .4byte gUnknown_030012BC\n"
    "6:\n\t"
        "add r0, r2, #0\n\t"
        "add r0, #0x2d\n\t"
        "ldrb r5, [r0]\n\t"
        "cmp r5, #0xc\n\t"
        "bne 9f\n\t"
        "ldr r0, [r4, #0x18]\n\t"
        "add r0, #1\n\t"
        "str r0, [r4, #0x18]\n\t"
        "ldr r1, [r4, #0xc]\n\t"
        "mov r2, #0x20\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r2, [r1, #0x24]\n\t"
        "mov r1, #0xb\n\t"
        "bl sub_803AD80\n\t"
        "ldr r2, [r4, #0xc]\n\t"
        "add r2, #0x50\n\t"
        "mov r1, #0\n\t"
        "ldrsh r0, [r2, r1]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r4, #0x10]\n\t"
        "ldr r3, [r2, #4]\n\t"
        "mov r2, #0xa\n\t"
        "bl sub_803AD84\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x31\n\t"
        "mov r0, #0\n\t"
        "strb r0, [r1]\n\t"
        "add r2, r4, #0\n\t"
        "add r2, #0x2f\n\t"
        "mov r1, #1\n\t"
        "strb r1, [r2]\n\t"
        "sub r2, #8\n\t"
        "strb r5, [r2]\n\t"
        "add r2, #0xb\n\t"
        "strb r0, [r2]\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x30\n\t"
        "strb r1, [r0]\n\t"
        "sub r0, #8\n\t"
        "strb r5, [r0]\n\t"
    "7:\n\t"
        "ldr r0, 44f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
    "8:\n\t"
        "mov r1, #0xc\n\t"
        "bl PlaySfx\n\t"
        "mov r0, #1\n\t"
        "b 10f\n\t"
        ".align 2, 0\n"
    "44: .4byte gUnknown_030012BC\n"
    "9:\n\t"
        "mov r0, #0\n\t"
    "10:\n\t"
        "add sp, #4\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r1}\n\t"
        "bx r1"
    );
}

#endif

/* A proximity-triggered indicator: if `part->field_0x64` is within
 * `0x27f` (or, failing that, `sub_8012694` fires), dispatches on
 * `self+8`'s type (`7`/`9`/`0xb`/`0xe`, each with its own distance
 * threshold against `part->field_0x64`/its negation) to set `part+0xd`
 * bit 0 and fire the `+0x20`/`+0x24` trampoline with a fixed id
 * (`0x1a`), or (type `0xe`) tail-call `sub_80151C8`. Then, unless the
 * type is `7`/`9`/`0xb`/`0xe`/`0x1a`, reads the D-pad and remaps
 * `self+0x27`'s table-index byte through a further small dispatch
 * (types `9`/`0x1c`-`0x1d` fire `sub_803AD80`/`sub_80138E8` variants,
 * type `7` fires a `sub_803AD84` pair) before a shared tail that sets
 * `self+0x31` when the player's `+0x100` flag is set.
 *
 * Matched under old_agbcc. The distance tests are two separate `if`s (one
 * `||` gets folded into a single compare); the trio stores mix literal
 * stores with the parameter-passing inlines exactly where the ROM
 * materializes the constants early. */
void sub_801283C(struct act *self)
{
    u8 near = 0;
    u32 in;

    if (self->part->unk_64 <= 0x27F) {
        near = 1;
        if (sub_8012694(self))
            return;
    }
    in = gUnknown_030007E0;
    {
        struct act_part *part;

        if (self->state == 7) {
            part = self->part;
            if (-part->unk_64 > 0x1BF)
                goto done;
            if (-part->unk_64 > 0x17F)
                goto done;
            goto hit;
        } else if (self->state == 9) {
            part = self->part;
            if (-part->unk_64 > 0x1BF)
                goto done;
            if (-part->unk_64 > 0x7F)
                goto done;
            goto hit;
        } else if (self->state == 0xB) {
            part = self->part;
            if (-part->unk_64 > 0xFF)
                goto done;
            if (-part->unk_64 > 0x1F)
                goto done;
        hit:
            ActOrFlags0D(part, 1);
            ACT_CALL1(self, m20, 0x1A);
        } else if (self->state == 0xE) {
            if (self->unk_22) {
                s32 x;

                part = self->part;
                x = part->unk_64;
                if (-x <= 0x7F)
                    ActOrFlags0D(part, 1);
                if (x > 0)
                    sub_80151C8(self);
            }
        }
    }
done:
    if (self->state != 0xE && self->state != 0xB && near && (in & 0x100)) {
        u8 busy = self->unk_34;

        if (busy == 0) {
            u8 tag;

            ACT_PART_FLAGS0D(self->part) |= 1;
            tag = self->unk_2A[3];
            if (tag == 9 || self->state == 9) {
                ACT_CALL1(self, m20, 0xA);
                self->next31 = busy;
                self->flag2F = 1;
                self->next27 = busy;
                ActTrio28(self, busy, 1, 0x16);
                sub_80138E8(self);
                self->unk_2A[0] = busy;
                return;
            }
            if (tag == 7) {
                ACT_CALL1(self, m20, 8);
                ACT_CALL2(self, m50, self->part, 0x19);
                self->next31 = busy;
                self->flag2F = 1;
                self->next27 = busy;
                ActTrio28(self, busy, 1, 0x15);
            }
        }
    }
    if (self->state == 7 || self->state == 9 || self->state == 0xB || self->state == 0xE || self->state == 0x1A) {
        if (sub_8000760(gUnknown_03001304) <= 2) {
            ActQueue27(self, 0, 0);
        } else {
            u8 *slot = &self->next27;

            if (*slot == 0x1B || *slot == 0x1C)
                ActHold27P(self, slot, 0x1C);
            else if (self->state == 0xE)
                ActSetNext27P(self, slot, 7);
            else if (self->frame != 0) {
                if (*slot != 0xD)
                    ActSetNext27P(self, slot, 0xD);
            } else
                ActSetNext27P(self, slot, 7);
        }
        if (gUnknown_030012D8->unk_100)
            self->next31 = 1;
    }
}
/* Trailing byte count isn't a multiple of 4 - pad with zeros, not a nop. */
asm(".align 2, 0");
