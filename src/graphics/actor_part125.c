#include "core.h"

/* Start of the boss-weapon/singleton-object cluster's next raw range
 * (issue #58/#62's shared "self" object family continues here - state
 * at `self+0x28`, table-index/"kind" at `self+0xc`, anim-frame
 * halfword/byte pair at `self+0x10`/`self+0x12`, an accumulator at
 * `self+8`, a "part table" pointer at `self+0`, and an event/trampoline
 * table pointer at `self+0x50`). See docs/matching/issue-58-0x08030334-actor.md,
 * docs/matching/issue-62-0x08033804-actor.md and this range's own
 * write-up in docs/matching/. */

extern s32 sub_803ADB4(s32 a, s32 b);
extern void mem_free(void *ptr);
extern void InitActorPart(void *self, s32 a, s32 b, s32 c, s32 d);
extern s32 GetAnimFrameBaseOffset(void *self);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern s32 sub_803AD7C(void *addr, void *fn);
extern s32 sub_803AD80(void *arg0, s32 arg1, void *arg2);
extern s32 sub_803AD84(void *arg0, void *arg1);
extern void sub_802A980(void *self);
extern void sub_8032138(s32 arg0);
extern void sub_8031A08(void *self);

extern s32 gUnknown_03001538;
extern s32 gUnknown_0300156C;
extern void *gUnknown_03001568;
extern void *gUnknown_03001534;
extern s32 gUnknown_030013C0;
extern void *gUnknown_030012BC;
extern u8 gStaticData_087E5294[];
extern u8 gStaticData_0817C414[];

/* Gates the boss-weapon tracker's own "ready" check: while the tracker
 * is inactive (`gUnknown_03001538 == 0`), reports "not ready" (-1).
 * Otherwise scales the countdown `gUnknown_0300156C` by 100 through
 * `sub_803ADB4` against the weapon table's own first field
 * (`*gUnknown_03001568`, a `void *` pointing at the small weapon-kind
 * table already characterized in actor_part21d.c), and reports "ready"
 * (1) once that scaled ratio is exactly zero and the countdown is still
 * running (`> 0`); otherwise passes the scaled ratio straight through. */
s32 sub_8031784(void)
{
    register s32 countdown asm("r4");
    s32 result;

    if (gUnknown_03001538 == 0) {
        return -1;
    }

    countdown = gUnknown_0300156C;
    result = sub_803ADB4(countdown * 100, *(s32 *)gUnknown_03001568);
    if (result == 0 && countdown > 0) {
        result = 1;
    }
    return result;
}

/* Destructor for the small tracker object (`gUnknown_03001534`) -
 * `mem_free`'s it directly, the counterpart to its constructor
 * `sub_8030F88` (actor_part23d.c). */
void sub_80317C4(void)
{
    mem_free(gUnknown_03001534);
}

void nullsub_30(void)
{
}

void nullsub_31(void)
{
}

/* Semantics understood (proximity-gated event trigger: syncs via
 * `sub_802A980`, checks a camera-relative bound against `self+0x34`,
 * flushes a pending trampoline call at `self+0x58` via `sub_8032138`,
 * checks the shared `self+0x28`/`self+0x12`/`self+0x20` state gate, and
 * either draws a text popup through the event table at `self+0x50` or
 * falls back to `sub_8031A08`), but resists a byte-exact plain-C
 * reconstruction in isolation (this compiler doesn't reach for `r4` as
 * the whole-function `self` pin the ROM uses without also perturbing
 * the branch layout) - transcribed NAKED, byte-verified against the
 * original disassembly. */
NAKED void sub_80317E0(void *selfArg)
{
    asm(
        "push {r4, lr}\n\t"
        "add r4, r0, #0\n\t"
        "bl sub_802A980\n\t"
        "ldr r0, 1f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, 2f\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, [r4, #0x34]\n\t"
        "cmp r1, r0\n\t"
        "bge 3f\n\t"
        "ldr r0, [r4, #0x58]\n\t"
        "cmp r0, #0\n\t"
        "beq 5f\n\t"
        "bl sub_8032138\n\t"
        "mov r0, #0\n\t"
        "str r0, [r4, #0x58]\n\t"
        "b 5f\n\t"
        ".align 2, 0\n"
    "1: .4byte gUnknown_030013C0\n"
    "2: .4byte 0xFFFFFE00\n"
    "3:\n\t"
        "ldr r1, [r4, #0x28]\n\t"
        "cmp r1, #2\n\t"
        "bne 4f\n\t"
        "ldrb r0, [r4, #0x12]\n\t"
        "cmp r0, #0\n\t"
        "bne 5f\n\t"
    "4:\n\t"
        "cmp r1, #1\n\t"
        "bne 7f\n\t"
        "ldr r1, [r4, #0x20]\n\t"
        "ldr r0, 6f\n\t"
        "cmp r1, r0\n\t"
        "bge 7f\n\t"
    "5:\n\t"
        "cmp r4, #0\n\t"
        "beq 8f\n\t"
        "ldr r1, [r4, #0x50]\n\t"
        "mov r2, #8\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r2, [r1, #0xc]\n\t"
        "mov r1, #3\n\t"
        "bl sub_803AD80\n\t"
        "b 8f\n\t"
        ".align 2, 0\n"
    "6: .4byte 0xFFFF1F00\n"
    "7:\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8031A08\n\t"
    "8:\n\t"
        "pop {r4}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    );
}

/* Trivial `self+0x58` clearing setter. */
void sub_8031850(void *selfArg)
{
    u8 *self = selfArg;
    *(s32 *)(self + 0x58) = 0;
}

/* Health/damage-countdown transition at `self+0x54`: once it expires,
 * marks the death byte at `self+0x5c`, flushes a pending trampoline
 * call at `self+0x58` (`sub_803AD7C` on the event table's `+0x50`/
 * `+0x3c` fields), plays a fixed death sound cue, and fires the
 * state-2/table-index-1 transition (anim frame from `self`'s own part
 * table at `+0xc`) - same overall shape as the boss cluster's
 * `sub_8030530` (actor_part20.c). Transcribed NAKED: the death-byte
 * store's `1`/`0` constants (`r5`/`r6`) need to stay live and shared
 * across both the early flush branch and the later state-transition
 * block in the ROM's own register choice, in a way a plain-C
 * reconstruction's register pins couldn't reproduce without changing
 * the branch shape. */
NAKED void sub_8031858(void *selfArg, s32 delta)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "add r4, r0, #0\n\t"
        "ldr r0, [r4, #0x54]\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [r4, #0x54]\n\t"
        "cmp r0, #0\n\t"
        "bgt 2f\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x5c\n\t"
        "mov r5, #0\n\t"
        "mov r6, #1\n\t"
        "strb r6, [r0]\n\t"
        "ldr r2, [r4, #0x58]\n\t"
        "cmp r2, #0\n\t"
        "beq 1f\n\t"
        "ldr r1, [r2, #0x50]\n\t"
        "mov r3, #0x38\n\t"
        "ldrsh r0, [r1, r3]\n\t"
        "add r0, r2, r0\n\t"
        "ldr r1, [r1, #0x3c]\n\t"
        "bl sub_803AD7C\n\t"
        "str r5, [r4, #0x58]\n\t"
    "1:\n\t"
        "ldr r0, 3f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #0x2e\n\t"
        "bl PlaySfx\n\t"
        "mov r0, #2\n\t"
        "str r0, [r4, #0x28]\n\t"
        "str r5, [r4, #0x44]\n\t"
        "str r6, [r4, #0xc]\n\t"
        "ldr r0, [r4]\n\t"
        "ldrh r0, [r0, #0xc]\n\t"
        "mov r1, #0\n\t"
        "strh r0, [r4, #0x10]\n\t"
        "strb r1, [r4, #0x12]\n\t"
        "str r5, [r4, #8]\n\t"
    "2:\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "3: .4byte gUnknown_030012BC\n"
    );
}

/* Full reset idiom (state=1, counter/accumulator/table-index cleared,
 * anim frame re-synced from `self`'s own part table) - same shape as
 * the boss cluster's established reset blocks (`sub_80306AC`,
 * actor_part21c.c). */
void sub_80318B4(void *selfArg)
{
    u8 *self = selfArg;
    register s32 zero asm("r2") = 0;

    *(s32 *)(self + 0x58) = zero;
    *(s32 *)(self + 0x60) = zero;
    {
        register s32 one asm("r1") = 1;
        *(s32 *)(self + 0x28) = one;
    }
    *(s32 *)(self + 0x44) = zero;
    *(s32 *)(self + 0xc) = zero;
    {
        register u16 anim asm("r1") = *(u16 *)(*(u8 **)self);
        register u8 zero3 asm("r3") = 0;

        *(u16 *)(self + 0x10) = anim;
        self[0x12] = zero3;
    }
    *(s32 *)(self + 8) = zero;
}

/* Semantics understood (stashes 3 args into `self+0x1c`/`0x20`/`0x24`,
 * then runs the shared anim-frame-advance-and-clamp idiom below), but
 * this specific compiler always schedules the two `ldrsh`/subtract
 * constant loads (`#4`/`#6`) one instruction earlier than the ROM's own
 * build - transcribed NAKED, byte-verified. Same shared tail as
 * `sub_8031954`/`sub_80319A0` below. */
NAKED void sub_80318D0(void *selfArg, s32 a, s32 b, s32 c)
{
    asm(
        "push {r4, lr}\n\t"
        "add r4, r0, #0\n\t"
        "str r1, [r4, #0x1c]\n\t"
        "str r2, [r4, #0x20]\n\t"
        "str r3, [r4, #0x24]\n\t"
        "ldr r0, [r4, #0x44]\n\t"
        "add r0, #1\n\t"
        "str r0, [r4, #0x44]\n\t"
        "mov r0, #0x10\n\t"
        "ldrsh r1, [r4, r0]\n\t"
        "ldr r0, [r4, #8]\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r4, #8]\n\t"
        "mov r0, #0\n\t"
        "strb r0, [r4, #0x12]\n\t"
        "add r0, r4, #0\n\t"
        "bl GetAnimFrameBaseOffset\n\t"
        "ldr r2, [r4, #0xc]\n\t"
        "ldr r3, [r4]\n\t"
        "lsl r1, r2, #1\n\t"
        "add r1, r1, r2\n\t"
        "lsl r1, r1, #2\n\t"
        "add r1, r1, r3\n\t"
        "mov r3, #4\n\t"
        "ldrsh r2, [r1, r3]\n\t"
        "cmp r0, r2\n\t"
        "blt 1f\n\t"
        "mov r3, #6\n\t"
        "ldrsh r0, [r1, r3]\n\t"
        "sub r0, r2, r0\n\t"
        "lsl r0, r0, #8\n\t"
        "ldr r1, [r4, #8]\n\t"
        "sub r1, r1, r0\n\t"
        "str r1, [r4, #8]\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r4, #0x12]\n\t"
    "1:\n\t"
        "pop {r4}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    );
}

/* An `InitActorPart`-based constructor: forwards its first 4 real
 * arguments straight to `InitActorPart` (the last, `d`, stack-passed),
 * then marks `self+0x54 = 2`, sets `self+0x50`'s event/trampoline table
 * to `gStaticData_087E5294`, stashes a 6th argument (`e`, also
 * stack-passed) into `self+0x58`, and clears `self+0x5c` (byte). Same
 * shape as the already-matched `sub_80305F8` (actor_part20d.c), except
 * with a 6th argument instead of a second stash of `c`. */
void *sub_8031920(void *selfArg, s32 a, s32 b, s32 c, s32 d, s32 e)
{
    u8 *self = selfArg;
    register s32 eReg asm("r6") = e;
    register s32 health asm("r5") = 2;

    InitActorPart(self, a, b, c, d);
    *(s32 *)(self + 0x54) = health;
    *(void **)(self + 0x50) = gStaticData_087E5294;
    *(s32 *)(self + 0x58) = eReg;
    self[0x5c] = 0;

    return self;
}

/* Same shared anim-frame-advance-and-clamp idiom as `sub_80318D0`, no
 * incoming-argument stashes. Transcribed NAKED for the same scheduling
 * gap. */
NAKED void sub_8031954(void *selfArg)
{
    asm(
        "push {r4, lr}\n\t"
        "add r4, r0, #0\n\t"
        "ldr r0, [r4, #0x44]\n\t"
        "add r0, #1\n\t"
        "str r0, [r4, #0x44]\n\t"
        "mov r0, #0x10\n\t"
        "ldrsh r1, [r4, r0]\n\t"
        "ldr r0, [r4, #8]\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r4, #8]\n\t"
        "mov r0, #0\n\t"
        "strb r0, [r4, #0x12]\n\t"
        "add r0, r4, #0\n\t"
        "bl GetAnimFrameBaseOffset\n\t"
        "ldr r2, [r4, #0xc]\n\t"
        "ldr r3, [r4]\n\t"
        "lsl r1, r2, #1\n\t"
        "add r1, r1, r2\n\t"
        "lsl r1, r1, #2\n\t"
        "add r1, r1, r3\n\t"
        "mov r3, #4\n\t"
        "ldrsh r2, [r1, r3]\n\t"
        "cmp r0, r2\n\t"
        "blt 1f\n\t"
        "mov r3, #6\n\t"
        "ldrsh r0, [r1, r3]\n\t"
        "sub r0, r2, r0\n\t"
        "lsl r0, r0, #8\n\t"
        "ldr r1, [r4, #8]\n\t"
        "sub r1, r1, r0\n\t"
        "str r1, [r4, #8]\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r4, #0x12]\n\t"
    "1:\n\t"
        "pop {r4}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    );
}

/* An oscillation drive (`self+0x20 += self+0x60`, decaying `self+0x60`
 * by 6/frame floored at `-0x12c`) feeding the same shared anim-frame-
 * advance-and-clamp tail as `sub_8031954`/`sub_80318D0`. Transcribed
 * NAKED for the same scheduling gap. */
NAKED void sub_80319A0(void *selfArg)
{
    asm(
        "push {r4, lr}\n\t"
        "add r4, r0, #0\n\t"
        "ldr r0, [r4, #0x20]\n\t"
        "ldr r1, [r4, #0x60]\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r4, #0x20]\n\t"
        "sub r1, #6\n\t"
        "str r1, [r4, #0x60]\n\t"
        "ldr r0, 2f\n\t"
        "cmp r1, r0\n\t"
        "ble 1f\n\t"
        "str r0, [r4, #0x60]\n\t"
    "1:\n\t"
        "ldr r0, [r4, #0x44]\n\t"
        "add r0, #1\n\t"
        "str r0, [r4, #0x44]\n\t"
        "mov r0, #0x10\n\t"
        "ldrsh r1, [r4, r0]\n\t"
        "ldr r0, [r4, #8]\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r4, #8]\n\t"
        "mov r0, #0\n\t"
        "strb r0, [r4, #0x12]\n\t"
        "add r0, r4, #0\n\t"
        "bl GetAnimFrameBaseOffset\n\t"
        "ldr r2, [r4, #0xc]\n\t"
        "ldr r3, [r4]\n\t"
        "lsl r1, r2, #1\n\t"
        "add r1, r1, r2\n\t"
        "lsl r1, r1, #2\n\t"
        "add r1, r1, r3\n\t"
        "mov r3, #4\n\t"
        "ldrsh r2, [r1, r3]\n\t"
        "cmp r0, r2\n\t"
        "blt 3f\n\t"
        "mov r3, #6\n\t"
        "ldrsh r0, [r1, r3]\n\t"
        "sub r0, r2, r0\n\t"
        "lsl r0, r0, #8\n\t"
        "ldr r1, [r4, #8]\n\t"
        "sub r1, r1, r0\n\t"
        "str r1, [r4, #8]\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r4, #0x12]\n\t"
    "3:\n\t"
        "pop {r4}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "2: .4byte 0xFFFFFED4\n"
    );
}

void nullsub_32(void)
{
}

/* Draws the keyframe-table-relative text popup: indexes
 * `gStaticData_0817C414` by `self+0x28` (stride 8), and - when the
 * indexed entry's own `+2` halfword is positive - reads a *second*,
 * `+4`-offset-relative table entry's `+0`/`+4` fields (a 12-byte-stride
 * indirect record), otherwise falls back to the direct `+4` entry.
 * Structurally close to the already-documented `sub_8031A6C`/keyframe-
 * table family (`docs/rom_map.md`) but resists a byte-exact plain-C
 * reconstruction of the ROM's specific `r7`-as-table-base-pin choice -
 * transcribed NAKED, byte-verified. */
NAKED void sub_8031A08(void *selfArg)
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
    "1: .4byte gStaticData_0817C414\n"
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

/* Trivial `self+0x5c` byte getter. Needs a trailing `asm(".align 2, 0")`
 * - the lone-function-at-end-of-translation-unit padding gap already
 * documented for `sub_80306A4`/`sub_8033CF0` (issues #58/#62). */
u8 sub_8031A64(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x5c];
}

asm(".align 2, 0");
