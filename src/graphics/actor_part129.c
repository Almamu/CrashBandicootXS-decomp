#include "core.h"

/* First half of the `0x08031A6C`-`0x08032858` remainder issue #59's
 * foundational pass (docs/matching/issue-59-0x08031784-actor.md) left
 * for "Phase 2" - the first 30 of the 60 still-raw functions in
 * `asm/code_3_2_20_28568_c99c_31784_31a6c.s`, `sub_8031A6C` through
 * `sub_8032688` inclusive. Same shared "self" object family documented
 * for the boss-weapon cluster (issues #58/#62) and confirmed again on
 * first read here: state at `self+0x28`, table-index/"kind" at
 * `self+0xc`, an anim-frame halfword/byte pair at `self+0x10`/
 * `self+0x12`, an accumulator at `self+8`, a "part table" pointer at
 * `self+0`, and an event/trampoline table pointer at `self+0x50`.
 *
 * `sub_8031B0C`/`sub_8031C0C`/`sub_8031D04`/`sub_8031D7C`/`sub_8031E80`
 * are the "type-byte event dispatch" family already characterized by
 * `docs/rom_map.md`: a proximity check (`sub_802A6EC`) or a countdown
 * timer at `self+0x54` gates the transition, `PlaySfx(3, 0x100)` always
 * plays first, then a `self+0x30`-relative type byte selects between
 * `sub_8022EA8`/`sub_802F540` calls - written as `goto`-chained `if`
 * blocks (not a plain `switch`) to match this family's already-matched
 * sibling `sub_802C540` (`actor_part19g.c`), whose last case does
 * something structurally different from the others and resists a plain
 * `switch`'s uniform codegen.
 *
 * `sub_8032480` is the already-flagged orbital-motion consumer of the
 * shared trig table `gStaticData_0816A820`; `sub_8032290` turned out to
 * be a second, closely-related consumer of the same table feeding the
 * same `self+0x1c`/`self+0x20` position pair.
 *
 * `sub_8031A6C`/`sub_80322F4` are near-duplicate keyframe-table-relative
 * dispatch helpers, structurally identical to the already-parked
 * `sub_8031A08` (issue #59 Phase 1, `actor_part125.c`) - same
 * `r7`-as-table-base-pin gap, so both are transcribed NAKED here too
 * rather than re-attempting a reconstruction already shown not to work
 * for this exact shape. */

extern void *gUnknown_030012BC;
extern void *gUnknown_030012C0;
extern void *gUnknown_03000884;

extern u8 sub_802A6EC(void *self);
extern void sub_802A7B8(void *self);
extern void sub_8022FEC(void *self);
extern void sub_8022EA8(void *arg0, s32 arg1);
extern void sub_8022D50(void *arg0);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void sub_802F50C(void *selfArg, s32 delta);
extern void sub_802F540(void *selfArg, s32 delta);
extern void sub_802F164(void *selfArg, s32 x, s32 y);
extern void sub_802AAB4(void *selfArg);
extern s32 sub_8023464(void *self);
extern void sub_80318B4(void *selfArg);
extern void sub_80318D0(void *selfArg, s32 a, s32 b, s32 c);
extern void InitActorPart(void *self, s32 a, s32 b, s32 c, s32 d);
extern s32 sub_8000E1C(s32 max);
extern s32 sub_802E4B8(s32 kind, s32 a1, s32 a2, s32 a3, void *selfArg);
extern s32 sub_803ADB4(s32 a, s32 b);
extern s32 sub_803AD80(void *arg0, s32 arg1, void *arg2);
extern void mem_free(void *ptr);

extern u8 gStaticData_0816A820[];
extern u8 gStaticData_0817C444[];
extern u8 gStaticData_087E4DF4[];
extern u8 gStaticData_087E52CC[];
extern u8 gStaticData_087E530C[];
extern u8 gStaticData_087E534C[];
extern u8 gStaticData_087E538C[];
extern u8 gStaticData_087E53CC[];
extern u8 gStaticData_087E5404[];

/* Anonymous 12-byte (3-word) copy unit - see actor_part19g.c's own copy
 * of this comment for why this shape (rather than three separate `s32`
 * field copies) is needed to reproduce the ROM's `ldm`/`stm` lowering. */
struct vec3_words {
    s32 a, b, c;
};

void sub_803256C(void *selfArg);

/* Keyframe-table-relative dispatch helper: indexes `gStaticData_0817C42C`
 * by `self+0x28` (stride 8), and - when the indexed entry's own `+2`
 * halfword is positive - reads a second, `+4`-offset-relative table
 * entry's `+0`/`+4` fields, otherwise falls back to the direct `+4`
 * entry (same shape as `sub_80322F4` below and the already-parked
 * `sub_8031A08`, issue #59 Phase 1). Once the resulting pointer is
 * flushed through `sub_803AD84`, additionally fires a second
 * `self+0x50`-table trampoline call while state 1 (health-timer running
 * past `0xe100`) or state 2 with `self+0x12` set, falling back to
 * `sub_802A7B8` otherwise. Resists a byte-exact plain-C reconstruction
 * of the ROM's specific `r7`-as-table-base-pin choice - transcribed
 * NAKED, byte-verified against the original disassembly. */
NAKED void sub_8031A6C(void *selfArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r4, r0, #0\n\t"
        "ldr r1, 1f\n\t"
        "ldr r0, [r4, #0x28]\n\t"
        "lsl r3, r0, #3\n\t"
        "add r0, r3, r1\n\t"
        "mov r7, #2\n\t"
        "ldrsh r2, [r0, r7]\n\t"
        "add r7, r1, #0\n\t"
        "cmp r2, #0\n\t"
        "ble 2f\n\t"
        "mov r1, #4\n\t"
        "ldrsh r0, [r0, r1]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r0]\n\t"
        "lsl r0, r2, #3\n\t"
        "add r0, r0, r1\n\t"
        "sub r0, #8\n\t"
        "ldr r5, [r0]\n\t"
        "ldr r6, [r0, #4]\n\t"
        "add r3, r6, #0\n\t"
        "b 3f\n\t"
        ".align 2, 0\n"
    "1: .4byte gStaticData_0817C42C\n"
    "2:\n\t"
        "add r0, r7, #4\n\t"
        "add r0, r3, r0\n\t"
        "ldr r3, [r0]\n\t"
    "3:\n\t"
        "ldr r0, [r4, #0x28]\n\t"
        "lsl r0, r0, #3\n\t"
        "add r0, r0, r7\n\t"
        "mov r7, #0\n\t"
        "ldrsh r1, [r0, r7]\n\t"
        "cmp r2, #0\n\t"
        "ble 4f\n\t"
        "lsl r0, r5, #0x10\n\t"
        "asr r0, r0, #0x10\n\t"
        "add r0, r0, r1\n\t"
        "b 5f\n\t"
    "4:\n\t"
        "add r0, r1, #0\n\t"
    "5:\n\t"
        "add r0, r4, r0\n\t"
        "bl sub_803AD84\n\t"
        "ldr r0, [r4, #0x28]\n\t"
        "cmp r0, #1\n\t"
        "bne 6f\n\t"
        "ldr r1, [r4, #0x20]\n\t"
        "mov r0, #0xe1\n\t"
        "lsl r0, r0, #8\n\t"
        "cmp r1, r0\n\t"
        "ble 6f\n\t"
        "cmp r4, #0\n\t"
        "beq 9f\n\t"
        "ldr r1, [r4, #0x50]\n\t"
        "mov r2, #8\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "b 8f\n\t"
    "6:\n\t"
        "ldr r0, [r4, #0x28]\n\t"
        "cmp r0, #2\n\t"
        "bne 7f\n\t"
        "ldrb r0, [r4, #0x12]\n\t"
        "cmp r0, #0\n\t"
        "beq 7f\n\t"
        "cmp r4, #0\n\t"
        "beq 9f\n\t"
        "ldr r1, [r4, #0x50]\n\t"
        "mov r7, #8\n\t"
        "ldrsh r0, [r1, r7]\n\t"
    "8:\n\t"
        "add r0, r4, r0\n\t"
        "ldr r2, [r1, #0xc]\n\t"
        "mov r1, #3\n\t"
        "bl sub_803AD80\n\t"
        "b 9f\n\t"
    "7:\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_802A7B8\n\t"
    "9:\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    );
}

/* Proximity-triggered member of the shared "type-byte event dispatch"
 * family: on trigger, transitions to state 2/table-index 1 (anim frame
 * from `self`'s own part table at `+0xc`), then dispatches on a
 * `self+0x30` type byte (`0x14`-`0x16` into `sub_802F540` at
 * increasing tiers, `0x17` into a fixed sound cue plus
 * `sub_802AAB4`/`sub_8023464`), flushes a pending trampoline call at
 * `self+0x58`, marks `self+0x5c`, and tail-calls `sub_8031A6C`. */
void sub_8031B0C(void *selfArg)
{
    u8 *self = selfArg;
    s32 kind = *(s32 *)(self + 0xc);

    if (kind == 0 && sub_802A6EC(self)) {
        register s32 state asm("r0") = 2;
        register s32 one asm("r1") = 1;
        s32 typeByte;

        *(s32 *)(self + 0x28) = state;
        *(s32 *)(self + 0x44) = kind;
        *(s32 *)(self + 0xc) = one;
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0xc);
            register u8 zero1 asm("r1") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero1;
        }
        *(s32 *)(self + 8) = kind;

        typeByte = *(u8 *)(*(u8 **)(self + 0x30));

        if (typeByte == 0x15) {
            goto case_15;
        }
        if (typeByte > 0x15) {
            goto gt_15;
        }
        if (typeByte == 0x14) {
            goto case_14;
        }
        goto after_dispatch;

    gt_15:
        if (typeByte == 0x16) {
            goto case_16;
        }
        if (typeByte == 0x17) {
            goto case_17;
        }
        goto after_dispatch;

    case_14:
        PlaySfx(gUnknown_030012BC, 3, 0x100);
        sub_802F540(gUnknown_03000884, 1);
        goto after_dispatch;

    case_15:
        PlaySfx(gUnknown_030012BC, 3, 0x100);
        sub_802F540(gUnknown_03000884, 3);
        goto after_dispatch;

    case_16:
        PlaySfx(gUnknown_030012BC, 3, 0x100);
        sub_802F540(gUnknown_03000884, 5);
        goto after_dispatch;

    case_17:
        PlaySfx(gUnknown_030012BC, 7, 0x100);
        sub_802AAB4(*(void **)(self + 0x70));
        sub_8023464(gUnknown_030012C0);

    after_dispatch:
        if (*(s32 *)(self + 0x58) != 0) {
            sub_8022FEC(gUnknown_030012C0);
            sub_80318B4(*(void **)(self + 0x58));
            *(s32 *)(self + 0x58) = 0;
        }
        self[0x5c] = 1;
    }

    sub_8031A6C(self);
}

/* Countdown twin of `sub_8031B0C`: gated by `self+0x54`'s health-style
 * timer instead of proximity, same type-byte dispatch, no tail call
 * (the caller drives whatever comes after directly). */
void sub_8031C0C(void *selfArg, s32 delta)
{
    u8 *self = selfArg;
    s32 health = *(s32 *)(self + 0x54) - delta;
    s32 typeByte;

    *(s32 *)(self + 0x54) = health;
    if (health > 0) {
        return;
    }

    {
        register s32 state asm("r0") = 2;
        register s32 one asm("r1") = 1;

        *(s32 *)(self + 0x28) = state;
        {
            register s32 zero2 asm("r2") = 0;

            *(s32 *)(self + 0x44) = zero2;
            *(s32 *)(self + 0xc) = one;
            {
                register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0xc);
                register u8 zero1 asm("r1") = 0;

                *(u16 *)(self + 0x10) = anim;
                self[0x12] = zero1;
            }
            *(s32 *)(self + 8) = zero2;
        }
    }

    typeByte = *(u8 *)(*(u8 **)(self + 0x30));

    if (typeByte == 0x15) {
        goto case_15;
    }
    if (typeByte > 0x15) {
        goto gt_15;
    }
    if (typeByte == 0x14) {
        goto case_14;
    }
    goto after_dispatch;

gt_15:
    if (typeByte == 0x16) {
        goto case_16;
    }
    if (typeByte == 0x17) {
        goto case_17;
    }
    goto after_dispatch;

case_14:
    PlaySfx(gUnknown_030012BC, 3, 0x100);
    sub_802F540(gUnknown_03000884, 1);
    goto after_dispatch;

case_15:
    PlaySfx(gUnknown_030012BC, 3, 0x100);
    sub_802F540(gUnknown_03000884, 3);
    goto after_dispatch;

case_16:
    PlaySfx(gUnknown_030012BC, 3, 0x100);
    sub_802F540(gUnknown_03000884, 5);
    goto after_dispatch;

case_17:
    PlaySfx(gUnknown_030012BC, 7, 0x100);
    sub_802AAB4(*(void **)(self + 0x70));
    sub_8023464(gUnknown_030012C0);

after_dispatch:
    if (*(s32 *)(self + 0x58) != 0) {
        sub_8022FEC(gUnknown_030012C0);
        sub_80318B4(*(void **)(self + 0x58));
        *(s32 *)(self + 0x58) = 0;
    }
    self[0x5c] = 1;
}

/* Proximity-triggered transition with a single fixed downstream call
 * (`sub_802F50C(player, 0x14)`) rather than a type-byte dispatch, then
 * flushes `self+0x58` and tail-calls `sub_8031A6C`. */
void sub_8031D04(void *selfArg)
{
    u8 *self = selfArg;
    s32 kind = *(s32 *)(self + 0xc);

    if (kind == 0 && sub_802A6EC(self)) {
        register s32 state asm("r0") = 2;
        register s32 one asm("r6") = 1;

        *(s32 *)(self + 0x28) = state;
        *(s32 *)(self + 0x44) = kind;
        *(s32 *)(self + 0xc) = one;
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0xc);
            register u8 zero1 asm("r1") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero1;
        }
        *(s32 *)(self + 8) = kind;

        sub_802F50C(gUnknown_03000884, 0x14);
        PlaySfx(gUnknown_030012BC, 3, 0x100);

        if (*(s32 *)(self + 0x58) != 0) {
            sub_8022FEC(gUnknown_030012C0);
            sub_80318B4(*(void **)(self + 0x58));
            *(s32 *)(self + 0x58) = kind;
        }
        self[0x5c] = one;
    }

    sub_8031A6C(self);
}

/* Proximity-triggered member of the `sub_8022EA8` half of the type-byte
 * dispatch family (values `0x18`/`0x19`/`0x1a`/`0x1d`); the `0x1d` case
 * plays a different cue and calls `sub_8022D50` instead, and the
 * trailing flush re-reads the type byte fresh to skip the lap-counter
 * tie (`sub_8022FEC`) specifically for that case. Tail-calls
 * `sub_8031A6C`. */
void sub_8031D7C(void *selfArg)
{
    u8 *self = selfArg;
    s32 kind = *(s32 *)(self + 0xc);

    if (kind == 0 && sub_802A6EC(self)) {
        register s32 state asm("r0") = 2;
        register s32 one asm("r1") = 1;
        s32 typeByte;

        *(s32 *)(self + 0x28) = state;
        *(s32 *)(self + 0x44) = kind;
        *(s32 *)(self + 0xc) = one;
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0xc);
            register u8 zero1 asm("r1") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero1;
        }
        *(s32 *)(self + 8) = kind;

        typeByte = *(u8 *)(*(u8 **)(self + 0x30));

        if (typeByte == 0x19) {
            goto case_19;
        }
        if (typeByte > 0x19) {
            goto gt_19;
        }
        if (typeByte == 0x18) {
            goto case_18;
        }
        goto after_dispatch;

    gt_19:
        if (typeByte == 0x1a) {
            goto case_1a;
        }
        if (typeByte == 0x1d) {
            goto case_1d;
        }
        goto after_dispatch;

    case_18:
        PlaySfx(gUnknown_030012BC, 3, 0x100);
        sub_8022EA8(gUnknown_030012C0, 1);
        goto after_dispatch;

    case_19:
        PlaySfx(gUnknown_030012BC, 3, 0x100);
        sub_8022EA8(gUnknown_030012C0, 2);
        goto after_dispatch;

    case_1a:
        PlaySfx(gUnknown_030012BC, 3, 0x100);
        sub_8022EA8(gUnknown_030012C0, 3);
        goto after_dispatch;

    case_1d:
        PlaySfx(gUnknown_030012BC, 0x18, 0x100);
        sub_8022D50(gUnknown_030012C0);

    after_dispatch:
        if (*(s32 *)(self + 0x58) != 0) {
            if (*(u8 *)(*(u8 **)(self + 0x30)) != 0x1d) {
                sub_8022FEC(gUnknown_030012C0);
            }
            sub_80318B4(*(void **)(self + 0x58));
            *(s32 *)(self + 0x58) = 0;
        }
        self[0x5c] = 1;
    }

    sub_8031A6C(self);
}

/* Countdown twin of `sub_8031D7C`: gated by `self+0x54`'s timer instead
 * of proximity, same `sub_8022EA8` dispatch, no tail call. */
void sub_8031E80(void *selfArg, s32 delta)
{
    u8 *self = selfArg;
    s32 health = *(s32 *)(self + 0x54) - delta;
    s32 typeByte;

    *(s32 *)(self + 0x54) = health;
    if (health > 0) {
        return;
    }

    {
        register s32 state asm("r0") = 2;
        register s32 one asm("r1") = 1;

        *(s32 *)(self + 0x28) = state;
        {
            register s32 zero2 asm("r2") = 0;

            *(s32 *)(self + 0x44) = zero2;
            *(s32 *)(self + 0xc) = one;
            {
                register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0xc);
                register u8 zero1 asm("r1") = 0;

                *(u16 *)(self + 0x10) = anim;
                self[0x12] = zero1;
            }
            *(s32 *)(self + 8) = zero2;
        }
    }

    typeByte = *(u8 *)(*(u8 **)(self + 0x30));

    if (typeByte == 0x19) {
        goto case_19;
    }
    if (typeByte > 0x19) {
        goto gt_19;
    }
    if (typeByte == 0x18) {
        goto case_18;
    }
    goto after_dispatch;

gt_19:
    if (typeByte == 0x1a) {
        goto case_1a;
    }
    if (typeByte == 0x1d) {
        goto case_1d;
    }
    goto after_dispatch;

case_18:
    PlaySfx(gUnknown_030012BC, 3, 0x100);
    sub_8022EA8(gUnknown_030012C0, 1);
    goto after_dispatch;

case_19:
    PlaySfx(gUnknown_030012BC, 3, 0x100);
    sub_8022EA8(gUnknown_030012C0, 2);
    goto after_dispatch;

case_1a:
    PlaySfx(gUnknown_030012BC, 3, 0x100);
    sub_8022EA8(gUnknown_030012C0, 3);
    goto after_dispatch;

case_1d:
    PlaySfx(gUnknown_030012BC, 0x18, 0x100);
    sub_8022D50(gUnknown_030012C0);

after_dispatch:
    if (*(s32 *)(self + 0x58) != 0) {
        if (*(u8 *)(*(u8 **)(self + 0x30)) != 0x1d) {
            sub_8022FEC(gUnknown_030012C0);
        }
        sub_80318B4(*(void **)(self + 0x58));
        *(s32 *)(self + 0x58) = 0;
    }
    self[0x5c] = 1;
}

/* `InitActorPart`-based constructor: forwards `a`/`b`/`c`/`d` straight
 * through, marks health `2`, stashes `b`/`c` into `self+0x60`/`0x64`, a
 * random 16-bit seed into `self+0x68`, then forwards to `sub_802E4B8`
 * (kind `0x28`) with `c` biased by `-15798` - one of the "spawn effect
 * type N" family's own per-kind constructors (docs/rom_map.md). */
void *sub_8031F78(void *selfArg, s32 a, s32 b, s32 c, s32 d)
{
    u8 *self = selfArg;
    register s32 health asm("r8") = 2;

    InitActorPart(self, a, b, c, d);
    *(s32 *)(self + 0x54) = health;
    *(void **)(self + 0x50) = gStaticData_087E538C;
    self[0x5c] = 0;
    *(s32 *)(self + 0x60) = b;
    *(s32 *)(self + 0x64) = c;
    *(s32 *)(self + 0x68) = (u16)sub_8000E1C(0xff);

    *(s32 *)(self + 0x58) = sub_802E4B8(0x28, b, c + (s32)0xFFFFC24A, d, self);
    *(void **)(self + 0x50) = gStaticData_087E530C;

    return self;
}

/* Countdown twin of `sub_8031C0C`'s shape applied to a fixed-cue,
 * single-downstream-call proximity/countdown transition (same body as
 * `sub_8031FE8` below except gated by `self+0x54`, see there). */
void sub_8031FE8(void *selfArg, s32 delta)
{
    u8 *self = selfArg;
    s32 health = *(s32 *)(self + 0x54) - delta;

    *(s32 *)(self + 0x54) = health;
    if (health > 0) {
        return;
    }

    {
        register s32 state asm("r0") = 2;
        register s32 one asm("r6") = 1;

        *(s32 *)(self + 0x28) = state;
        {
            register s32 zero2 asm("r5") = 0;

            *(s32 *)(self + 0x44) = zero2;
            *(s32 *)(self + 0xc) = one;
            {
                register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0xc);
                register u8 zero1 asm("r1") = 0;

                *(u16 *)(self + 0x10) = anim;
                self[0x12] = zero1;
            }
            *(s32 *)(self + 8) = zero2;

            sub_802F50C(gUnknown_03000884, 0x14);
            PlaySfx(gUnknown_030012BC, 3, 0x100);

            if (*(s32 *)(self + 0x58) != 0) {
                sub_8022FEC(gUnknown_030012C0);
                sub_80318B4(*(void **)(self + 0x58));
                *(s32 *)(self + 0x58) = zero2;
            }
            self[0x5c] = one;
        }
    }
}

/* Same `sub_802E4B8`-based constructor shape as `sub_8031F78`, kind
 * `0x2a`, final event table `gStaticData_087E52CC`. */
void *sub_8032054(void *selfArg, s32 a, s32 b, s32 c, s32 d)
{
    u8 *self = selfArg;
    register s32 health asm("r8") = 2;

    InitActorPart(self, a, b, c, d);
    *(s32 *)(self + 0x54) = health;
    *(void **)(self + 0x50) = gStaticData_087E538C;
    self[0x5c] = 0;
    *(s32 *)(self + 0x60) = b;
    *(s32 *)(self + 0x64) = c;
    *(s32 *)(self + 0x68) = (u16)sub_8000E1C(0xff);

    *(s32 *)(self + 0x58) = sub_802E4B8(0x2a, b, c + (s32)0xFFFFC24A, d, self);
    *(void **)(self + 0x50) = gStaticData_087E52CC;

    return self;
}

/* Same `sub_802E4B8`-based constructor shape again, kind `0x29`, final
 * event table `gStaticData_087E534C`, plus a 6th argument stashed
 * verbatim into `self+0x70`. */
void *sub_80320C4(void *selfArg, s32 a, s32 b, s32 c, s32 d, s32 e)
{
    u8 *self = selfArg;
    register s32 health asm("r8") = 2;

    InitActorPart(self, a, b, c, d);
    *(s32 *)(self + 0x54) = health;
    *(void **)(self + 0x50) = gStaticData_087E538C;
    self[0x5c] = 0;
    *(s32 *)(self + 0x60) = b;
    *(s32 *)(self + 0x64) = c;
    *(s32 *)(self + 0x68) = (u16)sub_8000E1C(0xff);

    *(s32 *)(self + 0x58) = sub_802E4B8(0x29, b, c + (s32)0xFFFFC24A, d, self);
    *(void **)(self + 0x50) = gStaticData_087E534C;
    *(s32 *)(self + 0x70) = e;

    return self;
}

/* Trivial `self+0x58` clearing setter. */
void sub_8032138(void *selfArg)
{
    u8 *self = selfArg;
    *(s32 *)(self + 0x58) = 0;
}

/* Full reset idiom variant: `self+0x6c`/`0x44`/`0xc`/`8` cleared, state
 * set to 1, anim frame re-synced from `self`'s own part table at `+0`
 * (not `+0xc`, unlike the boss cluster's usual reset block), lap-counter
 * tie (`sub_8022FEC`), `self+0x58` cleared. */
void sub_8032140(void *selfArg)
{
    u8 *self = selfArg;
    register s32 zero asm("r5") = 0;

    *(s32 *)(self + 0x6c) = zero;
    *(s32 *)(self + 0x28) = 1;
    *(s32 *)(self + 0x44) = zero;
    *(s32 *)(self + 0xc) = zero;
    {
        register u16 anim asm("r0") = *(u16 *)(*(u8 **)self);
        register u8 zero1 asm("r1") = 0;

        *(u16 *)(self + 0x10) = anim;
        self[0x12] = zero1;
    }
    *(s32 *)(self + 8) = zero;
    sub_8022FEC(gUnknown_030012C0);
    *(s32 *)(self + 0x58) = zero;
}

/* Countdown-gated state-2 transition with a `self+0x58` trampoline
 * flush (sound cue plus lap-counter tie only fire when there's a
 * pending object to flush), no type-byte dispatch. */
void sub_8032170(void *selfArg, s32 delta)
{
    u8 *self = selfArg;
    s32 health = *(s32 *)(self + 0x54) - delta;

    *(s32 *)(self + 0x54) = health;
    if (health > 0) {
        return;
    }

    {
        register s32 state asm("r0") = 2;
        register s32 one asm("r6") = 1;

        *(s32 *)(self + 0x28) = state;
        {
            register s32 zero2 asm("r5") = 0;

            *(s32 *)(self + 0x44) = zero2;
            *(s32 *)(self + 0xc) = one;
            {
                register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0xc);
                register u8 zero1 asm("r1") = 0;

                *(u16 *)(self + 0x10) = anim;
                self[0x12] = zero1;
            }
            *(s32 *)(self + 8) = zero2;

            if (*(s32 *)(self + 0x58) != 0) {
                PlaySfx(gUnknown_030012BC, 3, 0x100);
                sub_8022FEC(gUnknown_030012C0);
                sub_80318B4(*(void **)(self + 0x58));
                *(s32 *)(self + 0x58) = zero2;
            }
            self[0x5c] = one;
        }
    }
}

/* Doubly-linked-list unlink (`self+0x48`=prev, `self+0x4c`=next, cross-
 * links `next->prev`/`prev->next` around `self`), resets `self+0x50`'s
 * event table to `gStaticData_087E4DF4`, then conditionally `mem_free`s
 * `self` if the caller's flag bit 0 is set - a destructor/detach helper
 * for this object family. */
void sub_80321D0(void *selfArg, s32 flags)
{
    u8 *self = selfArg;
    u8 *next;
    u8 *prev;

    *(void **)(self + 0x50) = gStaticData_087E4DF4;

    {
        register u8 *nextReg asm("r2") = *(u8 **)(self + 0x4c);
        register u8 *prevReg asm("r0") = *(u8 **)(self + 0x48);

        *(u8 **)(nextReg + 0x48) = prevReg;
    }

    prev = *(u8 **)(self + 0x48);
    next = *(u8 **)(self + 0x4c);
    *(u8 **)(prev + 0x4c) = next;

    if ((flags & 1) != 0) {
        mem_free(self);
    }
}

/* Same `sub_802E4B8`-based constructor shape as `sub_8031F78`, but
 * fully parameterized: the "kind" (`0x28`/`0x29`/`0x2a`/etc there) is a
 * 6th caller-supplied byte argument here rather than a fixed literal,
 * and this one doesn't reassign `self+0x50`'s event table afterward. */
/* Semantics understood as part of the shared "spawn effect type N"
 * constructor family (`sub_8031F78`/`sub_8032054`/`sub_80320C4` above):
 * the "kind" is a 6th caller-supplied byte argument here instead of a
 * fixed literal, and this one doesn't reassign `self+0x50`'s event
 * table afterward. Resists a byte-exact plain-C reconstruction: this
 * compiler always re-materializes the incoming `c` argument register
 * for the `InitActorPart` call from its own cached copy (`r6`) instead
 * of leaving the ROM's original parameter register (`r3`) untouched
 * until the call, and separately defers the `kind` byte truncation to
 * its point of use rather than the ROM's eager truncation right after
 * loading it from the stack - transcribed NAKED, byte-verified against
 * the original disassembly. */
NAKED void *sub_80321FC(void *selfArg, s32 a, s32 b, s32 c, s32 d, s32 kind)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "sub sp, #4\n\t"
        "add r4, r0, #0\n\t"
        "mov sb, r2\n\t"
        "add r6, r3, #0\n\t"
        "ldr r7, [sp, #0x20]\n\t"
        "ldr r5, [sp, #0x24]\n\t"
        "lsl r5, r5, #0x18\n\t"
        "lsr r5, r5, #0x18\n\t"
        "mov r0, #2\n\t"
        "mov r8, r0\n\t"
        "str r7, [sp]\n\t"
        "add r0, r4, #0\n\t"
        "bl InitActorPart\n\t"
        "mov r0, r8\n\t"
        "str r0, [r4, #0x54]\n\t"
        "ldr r0, 1f\n\t"
        "str r0, [r4, #0x50]\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x5c\n\t"
        "mov r0, #0\n\t"
        "strb r0, [r1]\n\t"
        "mov r0, sb\n\t"
        "str r0, [r4, #0x60]\n\t"
        "str r6, [r4, #0x64]\n\t"
        "mov r0, #0xff\n\t"
        "bl sub_8000E1C\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "str r0, [r4, #0x68]\n\t"
        "ldr r0, 2f\n\t"
        "add r6, r6, r0\n\t"
        "str r4, [sp]\n\t"
        "add r0, r5, #0\n\t"
        "mov r1, sb\n\t"
        "add r2, r6, #0\n\t"
        "add r3, r7, #0\n\t"
        "bl sub_802E4B8\n\t"
        "str r0, [r4, #0x58]\n\t"
        "add r0, r4, #0\n\t"
        "add sp, #4\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    "1: .4byte gStaticData_087E538C\n"
    "2: .4byte 0xFFFFC24A\n"
    );
}

void nullsub_33(void *selfArg)
{
}

/* Trivial accumulator: `self+0x20` advances by `self+0x6c`'s current
 * step, then the step itself advances by `0x12`/frame, clamped to
 * `0x4c0`. */
void sub_8032274(void *selfArg)
{
    u8 *self = selfArg;
    s32 pos = *(s32 *)(self + 0x20);
    s32 delta = *(s32 *)(self + 0x6c);

    *(s32 *)(self + 0x20) = pos + delta;
    delta += 0x12;
    *(s32 *)(self + 0x6c) = delta;
    if (delta <= 0x4c0) {
        return;
    }
    *(s32 *)(self + 0x6c) = 0x4c0;
}

/* A second, independent consumer of the shared trig table
 * `gStaticData_0816A820` (the orbital-motion convention already
 * documented for `sub_8032480`): computes an `self+0x1c`/`self+0x20`
 * position pair from two phase-shifted table lookups around
 * `self+0x68 + self+0x44`, then - while `self+0x58` holds another
 * object - forwards the result into that object's own anim-frame-
 * advance-and-clamp step (`sub_80318D0`). */
void sub_8032290(void *selfArg)
{
    u8 *self = selfArg;
    s16 *trig = (s16 *)gStaticData_0816A820;
    s32 phase = *(s32 *)(self + 0x68) + *(s32 *)(self + 0x44);
    s32 idx1 = ((phase * 5) >> 4) & 0xff;
    s32 v1 = trig[idx1];
    s32 x = *(s32 *)(self + 0x60) + v1 * 17;
    s32 idx2;
    s32 v2;
    s32 y;

    *(s32 *)(self + 0x1c) = x;

    idx2 = ((phase * 8) >> 4) & 0xff;
    v2 = trig[idx2];
    y = *(s32 *)(self + 0x64) + v2 * 30;
    *(s32 *)(self + 0x20) = y;

    if (*(s32 *)(self + 0x58) != 0) {
        sub_80318D0(*(void **)(self + 0x58), x, y + (s32)0xFFFFC24A, *(s32 *)(self + 0x24));
    }
}

/* Same keyframe-table-relative dispatch core as `sub_8031A6C` above
 * (identical body, minus that function's extra trailing
 * state/trampoline logic) - same `r7`-as-table-base-pin gap,
 * transcribed NAKED, byte-verified. */
NAKED void sub_80322F4(void *selfArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r4, r0, #0\n\t"
        "ldr r1, 1f\n\t"
        "ldr r0, [r4, #0x28]\n\t"
        "lsl r3, r0, #3\n\t"
        "add r0, r3, r1\n\t"
        "mov r7, #2\n\t"
        "ldrsh r2, [r0, r7]\n\t"
        "add r7, r1, #0\n\t"
        "cmp r2, #0\n\t"
        "ble 2f\n\t"
        "mov r1, #4\n\t"
        "ldrsh r0, [r0, r1]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r0]\n\t"
        "lsl r0, r2, #3\n\t"
        "add r0, r0, r1\n\t"
        "sub r0, #8\n\t"
        "ldr r5, [r0]\n\t"
        "ldr r6, [r0, #4]\n\t"
        "add r3, r6, #0\n\t"
        "b 3f\n\t"
        ".align 2, 0\n"
    "1: .4byte gStaticData_0817C42C\n"
    "2:\n\t"
        "add r0, r7, #4\n\t"
        "add r0, r3, r0\n\t"
        "ldr r3, [r0]\n\t"
    "3:\n\t"
        "ldr r0, [r4, #0x28]\n\t"
        "lsl r0, r0, #3\n\t"
        "add r0, r0, r7\n\t"
        "mov r7, #0\n\t"
        "ldrsh r1, [r0, r7]\n\t"
        "cmp r2, #0\n\t"
        "ble 4f\n\t"
        "lsl r0, r5, #0x10\n\t"
        "asr r0, r0, #0x10\n\t"
        "add r0, r0, r1\n\t"
        "b 5f\n\t"
    "4:\n\t"
        "add r0, r1, #0\n\t"
    "5:\n\t"
        "add r0, r4, r0\n\t"
        "bl sub_803AD84\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    );
}

/* Trivial `self+0x5c` byte getter. */
u8 sub_8032350(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x5c];
}

/* State-1 trampoline flush, or (otherwise) a proximity-triggered
 * transition that fires an event-table call on the *player* object
 * (`gUnknown_03000884`) before its own state-1/table-index-1
 * transition; either way clamps `self+0x20` forward by `0x140` once it
 * falls behind `self+0x5c`, then tail-calls `sub_802A7B8`. */
void sub_8032358(void *selfArg)
{
    u8 *self = selfArg;

    if (*(s32 *)(self + 0xc) == 1) {
        if (self[0x12] == 0) {
            goto tail;
        }
        if (self != 0) {
            u8 *table = *(u8 **)(self + 0x50);
            sub_803AD80(self + *(s16 *)(table + 8), 3, *(void **)(table + 0xc));
        }
        return;
    }

    if (sub_802A6EC(self)) {
        u8 *player = gUnknown_03000884;
        u8 *ptable = *(u8 **)(player + 0x50);

        sub_803AD80(player + *(s16 *)(ptable + 0x20), 0x14, *(void **)(ptable + 0x24));
        sub_8022FEC(gUnknown_030012C0);
        PlaySfx(gUnknown_030012BC, 4, 0x100);
        *(s32 *)(self + 0xc) = 1;
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0xc);
            register u8 zero1 asm("r1") = 0;
            register s32 zero2 asm("r2") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero1;
            *(s32 *)(self + 8) = zero2;
        }
        self[0x58] = 1;
    }

    if (*(s32 *)(self + 0x20) < *(s32 *)(self + 0x5c)) {
        *(s32 *)(self + 0x20) += 0x140;
    }

tail:
    sub_802A7B8(self);
}

/* Countdown-gated `self+0x58` byte transition into state 1 (anim frame
 * from `self`'s own part table at `+0xc`), then ties the lap counter. */
void sub_80323F4(void *selfArg, s32 delta)
{
    register u8 *self asm("r6") = selfArg;
    s32 health = *(s32 *)(self + 0x54) - delta;

    *(s32 *)(self + 0x54) = health;
    if (health > 0) {
        return;
    }

    {
        u8 *deathPtr = self + 0x58;
        register s32 zero2 asm("r5") = 0;
        register s32 one asm("r4") = 1;

        *deathPtr = one;
        PlaySfx(gUnknown_030012BC, 4, 0x100);
        *(s32 *)(self + 0xc) = one;
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0xc);
            register u8 zero1 asm("r1") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero1;
        }
        *(s32 *)(self + 8) = zero2;
        sub_8022FEC(gUnknown_030012C0);
    }
}

/* Thin `InitActorPart`-based constructor: forwards `a`/`b` straight
 * through but replaces its own `c` with a fixed bias constant
 * (`0xFFFF0600`) for `InitActorPart`'s own 4th argument, stashing the
 * caller's real `c` into `self+0x5c` instead; `d` still forwards to
 * `InitActorPart` untouched. Semantics fully understood and every
 * real-C attempt reproduced the ROM's exact register choices, but this
 * compiler's own independent-instruction scheduler always groups the
 * two pure register loads (`d` off the stack, the `0xFFFF0600`
 * constant) together regardless of source order, while the ROM's own
 * build interleaves them with the `str`/`adds` steps in between -
 * transcribed NAKED, byte-verified. */
NAKED void *sub_8032440(void *selfArg, s32 a, s32 b, s32 c, s32 d)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "sub sp, #4\n\t"
        "add r4, r0, #0\n\t"
        "add r6, r3, #0\n\t"
        "ldr r0, [sp, #0x14]\n\t"
        "mov r5, #2\n\t"
        "str r0, [sp]\n\t"
        "add r0, r4, #0\n\t"
        "ldr r3, 1f\n\t"
        "bl InitActorPart\n\t"
        "str r5, [r4, #0x54]\n\t"
        "ldr r0, 2f\n\t"
        "str r0, [r4, #0x50]\n\t"
        "str r6, [r4, #0x5c]\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x58\n\t"
        "mov r0, #0\n\t"
        "strb r0, [r1]\n\t"
        "add r0, r4, #0\n\t"
        "add sp, #4\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    "1: .4byte 0xFFFF0600\n"
    "2: .4byte gStaticData_087E53CC\n"
    );
}

/* Trivial `self+0x58` byte getter. */
u8 sub_8032478(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x58];
}

/* The already-flagged orbital-motion consumer of the shared trig table
 * `gStaticData_0816A820` (docs/rom_map.md): while idle (state 0),
 * checks proximity to fire an event-table call on the player plus a
 * state transition through `sub_803256C`, then drives the orbit itself
 * (`self+0x1c`) and either lets `self+0x20` coast forward by
 * `self+0x60` or, once it catches up to `self+0x5c`, re-seeds
 * `self+0x38`'s 3-word block from `gStaticData_0817C444` and re-fires
 * `sub_803256C`. Once no longer idle, either flushes a pending
 * `self+0x50` trampoline call (state-1/table-index-1 shape) or repeats
 * the same player-proximity event once (latched via `self+0x65`).
 * Falls back to `sub_802A7B8` in both non-idle paths. */
void sub_8032480(void *selfArg)
{
    u8 *self = selfArg;

    if (*(s32 *)(self + 0xc) != 0) {
        goto state_nonzero;
    }

    if (sub_802A6EC(self)) {
        u8 *player = gUnknown_03000884;
        u8 *ptable = *(u8 **)(player + 0x50);

        sub_803AD80(player + *(s16 *)(ptable + 0x20), 0xe, *(void **)(ptable + 0x24));
        self[0x65] = 1;
        sub_803256C(self);
    }

    /* `sub_803256C` may have just transitioned the state away from 0 -
     * the ROM re-checks and, if so, joins the state-nonzero handling
     * below instead of running the orbital-motion step on stale state. */
    if (*(s32 *)(self + 0xc) != 0) {
        goto state_nonzero;
    }

    {
        s16 *trig = (s16 *)gStaticData_0816A820;
        s32 idx = (*(s32 *)(self + 0x44)) << 6;
        s32 v;

        idx = ((idx >> 4) & 0xff) + 0x40;
        idx &= 0xff;
        v = trig[idx];

        *(s32 *)(self + 0x1c) = *(s32 *)(self + 0x58) + v * 16;

        if (*(s32 *)(self + 0x20) > *(s32 *)(self + 0x5c)) {
            *(s32 *)(self + 0x20) += *(s32 *)(self + 0x60);
        } else {
            *(struct vec3_words *)(self + 0x38) = *(struct vec3_words *)gStaticData_0817C444;
            sub_803256C(self);
        }
    }

    goto tail;

state_nonzero:
    if (self[0x12] != 0) {
        if (self != 0) {
            u8 *table = *(u8 **)(self + 0x50);
            sub_803AD80(self + *(s16 *)(table + 8), 3, *(void **)(table + 0xc));
        }
        return;
    }

    if (self[0x65] == 0 && sub_802A6EC(self)) {
        u8 *player = gUnknown_03000884;
        u8 *ptable = *(u8 **)(player + 0x50);

        sub_803AD80(player + *(s16 *)(ptable + 0x20), 0xe, *(void **)(ptable + 0x24));
        self[0x65] = 1;
    }

tail:
    sub_802A7B8(self);
}

/* State transition setter: marks `self+0x64`, plays a fixed cue, sets
 * `self+0x18`, and the usual state-1/anim-reset block (anim frame from
 * `self`'s own part table at `+0xc`). */
void sub_803256C(void *selfArg)
{
    u8 *self = selfArg;
    u8 *statePtr = self + 0x64;
    register s32 zero asm("r6") = 0;
    register s32 one asm("r5") = 1;

    *statePtr = one;
    PlaySfx(gUnknown_030012BC, 4, 0x100);
    *(s32 *)(self + 0x18) = 7;
    *(s32 *)(self + 0xc) = one;
    {
        register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0xc);
        register u8 zero1 asm("r1") = 0;

        *(u16 *)(self + 0x10) = anim;
        self[0x12] = zero1;
    }
    *(s32 *)(self + 8) = zero;
}

/* Countdown-gated double-byte state transition (`self+0x64`/`0x65`),
 * anim frame taken from `self`'s own part table at `+0x18` this time
 * (not the usual `+0xc`). */
void sub_80325A4(void *selfArg, s32 delta)
{
    register u8 *self asm("r5") = selfArg;
    s32 health = *(s32 *)(self + 0x54) - delta;

    *(s32 *)(self + 0x54) = health;
    if (health > 0) {
        return;
    }

    {
        register u8 *statePtr asm("r1") = self + 0x64;
        register s32 zero asm("r4") = 0;
        register s32 one asm("r0") = 1;

        *statePtr = one;
        asm volatile("add %0, %0, #1" : "+r"(statePtr));
        *statePtr = one;
        PlaySfx(gUnknown_030012BC, 4, 0x100);
        *(s32 *)(self + 0x18) = 4;
        *(s32 *)(self + 0xc) = 2;
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0x18);
            register u8 zero1 asm("r1") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero1;
        }
        *(s32 *)(self + 8) = zero;
    }
}

/* `InitActorPart`-based constructor (kind `1`, `InitActorPart`'s own 4th
 * argument replaced with a fixed `0xfa00` bias); clamps the caller's
 * `c` into `self+0x5c` (+-0x3f00), mirrors a clamped `self+0x1c` into
 * `self+0x58` (+-0x8000), and derives `self+0x60` from
 * `sub_803ADB4(self+0x5c - 0xfa00, 0xc6)`. Semantics fully understood
 * and every branch/store confirmed correct in a real-C attempt, but
 * this compiler couldn't be steered into the ROM's exact register
 * choreography for the `d` argument (transiently held in `r0`, pushed
 * to the outgoing stack slot, then `r0` reused for `self`) simultaneous
 * with `health` (`1`) needing to survive in `r4` across the
 * `InitActorPart` call - transcribed NAKED, byte-verified. */
NAKED void *sub_80325EC(void *selfArg, s32 a, s32 b, s32 c, s32 d)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "sub sp, #4\n\t"
        "add r5, r0, #0\n\t"
        "add r6, r3, #0\n\t"
        "ldr r0, [sp, #0x14]\n\t"
        "mov r4, #1\n\t"
        "str r0, [sp]\n\t"
        "add r0, r5, #0\n\t"
        "mov r3, #0xfa\n\t"
        "lsl r3, r3, #8\n\t"
        "bl InitActorPart\n\t"
        "str r4, [r5, #0x54]\n\t"
        "ldr r0, 1f\n\t"
        "str r0, [r5, #0x50]\n\t"
        "mov r0, #0xfc\n\t"
        "lsl r0, r0, #6\n\t"
        "cmp r6, r0\n\t"
        "ble 2f\n\t"
        "add r6, r0, #0\n\t"
    "2:\n\t"
        "ldr r0, 3f\n\t"
        "cmp r6, r0\n\t"
        "bge 4f\n\t"
        "add r6, r0, #0\n\t"
    "4:\n\t"
        "str r6, [r5, #0x5c]\n\t"
        "ldr r0, [r5, #0x1c]\n\t"
        "mov r1, #0x80\n\t"
        "lsl r1, r1, #8\n\t"
        "cmp r0, r1\n\t"
        "ble 5f\n\t"
        "str r1, [r5, #0x1c]\n\t"
    "5:\n\t"
        "ldr r0, [r5, #0x1c]\n\t"
        "ldr r1, 6f\n\t"
        "cmp r0, r1\n\t"
        "bge 7f\n\t"
        "str r1, [r5, #0x1c]\n\t"
    "7:\n\t"
        "ldr r0, [r5, #0x1c]\n\t"
        "str r0, [r5, #0x58]\n\t"
        "ldr r0, [r5, #0x5c]\n\t"
        "ldr r1, 8f\n\t"
        "add r0, r0, r1\n\t"
        "mov r1, #0xc6\n\t"
        "bl sub_803ADB4\n\t"
        "str r0, [r5, #0x60]\n\t"
        "add r0, r5, #0\n\t"
        "add r0, #0x65\n\t"
        "mov r1, #0\n\t"
        "strb r1, [r0]\n\t"
        "sub r0, #1\n\t"
        "strb r1, [r0]\n\t"
        "ldr r0, 9f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #0x2d\n\t"
        "bl PlaySfx\n\t"
        "add r0, r5, #0\n\t"
        "add sp, #4\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    "1: .4byte gStaticData_087E5404\n"
    "3: .4byte 0xFFFFC100\n"
    "6: .4byte 0xFFFF8000\n"
    "8: .4byte 0xFFFF0600\n"
    "9: .4byte gUnknown_030012BC\n"
    );
}

/* Trivial `self+0x64` byte getter. */
u8 sub_8032680(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x64];
}

/* Type-byte-gated (`self+0x30`'s type byte `== 0x1f`) proximity check:
 * on trigger, feeds the offset between `self+0x1c` and the type-byte
 * table's own `+0x20` field, plus `self+0x20`, into `sub_802F164`, then
 * latches a one-shot cue via `self+0x58`. Tail-calls `sub_802A7B8`
 * unconditionally. */
void sub_8032688(void *selfArg)
{
    u8 *self = selfArg;

    if (*(u8 *)(*(u8 **)(self + 0x30)) == 0x1f && sub_802A6EC(self)) {
        u8 *player = gUnknown_03000884;
        u8 *ptable = *(u8 **)(self + 0x30);
        s32 x = *(s32 *)(self + 0x1c) - *(s32 *)(ptable + 0x20);
        s32 y = *(s32 *)(self + 0x20);

        sub_802F164(player, x, y);

        if (self[0x58] == 0) {
            self[0x58] = 1;
            PlaySfx(gUnknown_030012BC, 0x3e, 0x100);
        }
    }

    sub_802A7B8(self);
}
