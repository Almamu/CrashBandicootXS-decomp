#include "core.h"

/* Same "self" object family as actor_part59.c - see that file's header
 * comment and docs/matching/issue-63-0x08033ef4-actor.md. */

/* Constructor: health defaults to `0x10`, or `0x18` if the
 * `gUnknown_030015AC` singleton hasn't been constructed yet
 * (`sub_80338DC() == 0`). Forwards to `InitActorPart`, sets the event
 * table (`+0x50=&gStaticData_087E5554`), caches the constructor's 6th
 * (byte, stack-passed) argument at `+0x59`, selects table-index 0 or 1
 * depending on whether that byte is set, resets the usual state/frame-
 * counter/anim fields, clears the death flag (`+0x58=0`), seeds the
 * "spawn/orbit" record (`+0x5c` from the `+0x59` byte, `+0x60=0xa00`,
 * `+0x64=-1`), clears the one-shot flag (`+0x2c=0`), caches the
 * singleton table's `+4` field at `+0x68`, and clears `+0x6c`. Returns
 * `self`.
 *
 * Two gaps closed to get this byte-exact, both `asm volatile` anchors
 * (this compiler's own C-driven codegen can't reproduce either shape):
 * - The 6th (stack-passed, byte-sized) constructor argument: a plain
 *   `u8 eByte` parameter always reads the full word and narrows it
 *   with two shifts, where the ROM computes the stack slot's address
 *   (`add r0, sp, #0x24`) and does a genuine `ldrb` - the same
 *   pattern already closed for other stack-passed byte arguments (see
 *   `docs/matching/issue-5-overlay-ui-sync.md`'s `sub_8003A60` entry
 *   and `issue-45-hud-stat-widget-dispatcher.md`'s "6th-argument"
 *   entry). Materialized via a two-instruction `asm volatile` anchor
 *   into an `"=l"`-constrained (lo-register) temp, then copied into
 *   the `register u32 eByteVal asm("r9")` pin that mirrors the ROM's
 *   own `sb`/r9 cache (needed since it survives the following
 *   `sub_80338DC()` call).
 * - The `+0x5c` spawn-record ternary (`(self[0x59] != 0) ? 0xFFFFBF00
 *   : 0x8400`): the ROM computes this as a genuine two-way diamond (a
 *   forward `beq`/`ldr`/`b` skipping a `movs`/`lsls` false-branch,
 *   with `gStaticData_087E5554`'s pending literal and `0xFFFFBF00`
 *   pooled together right after the skip branch). A plain-C ternary
 *   or if/else always collapses this into an eager "compute one value,
 *   conditionally overwrite" shape instead (4 bytes short - missing
 *   the `b` that skips the false block), and - separately - as soon as
 *   the diamond is forced through any register-pinned if/else, this
 *   compiler's parameter-homing pass stops copying `self` first into
 *   its `r5` home in the prologue (still correct address, but
 *   reordered relative to `part`/`b`/`cParam` - a non-matching
 *   permutation of the same four `adds`/`mov` instructions). Neither
 *   glitch responds to any C-level restructuring tried (ternary
 *   polarity flips, if/else, goto-linearized if/else, precomputed
 *   addresses, or pinning any subset of `self`/`part`/`cParam` - and
 *   pinning `b` itself to its needed `r7` hits the already-documented
 *   r7-pin-hazard, dropping r7 from the prologue's push/pop list
 *   entirely). Fixed by moving the whole diamond into one opaque
 *   `asm volatile` block (referencing `self`'s known `r5` home
 *   directly by name rather than as an operand, which - unlike the
 *   hazard above - does not perturb the surrounding C's own register
 *   allocation) with a real `ldr r0, =0xFFFFBF00` assembler pseudo-op
 *   and a manual `.pool` directive right after the skip branch; the
 *   preceding `gStaticData_087E5554` store (originally plain C) also
 *   had to move into its own tiny `asm volatile` island using the same
 *   `=symbol` pseudo-op, because a real, respected `.pool` split only
 *   works for symbols whose literal load is itself opaque assembler
 *   text - this compiler's own C-driven pool placement always defers
 *   a plain `extern` global access to the function's very end and
 *   ignores an `asm(".pool")` marker around it, exactly the gap
 *   already documented in `actor_part53.c`'s `sub_802AB58`. The
 *   `self[0x28]=0`/self[0x59] reload pair and the `zeroByte`/`zero2`
 *   register splits below needed the same "which register holds
 *   which cached zero" register-pinning treatment, each in its own
 *   narrowly-scoped block so the pin doesn't widen past where the ROM
 *   actually needs that register reserved. */
extern void *sub_80338DC(void);
extern void *InitActorPart(void *selfArg, void *part, s32 b, s32 c, s32 d);
extern u8 gStaticData_087E5554[];
extern void *sub_80338C4(void);

void *sub_8034058(void *selfArg, void *part, s32 b, s32 cParam, s32 d, u8 eByte)
{
    u8 *self;
    register u32 eByteVal asm("r9");
    register s32 health asm("r4");
    s32 idx;
    u8 byte59;
    s32 zero;
    u8 *p59;
    u32 t;

    self = selfArg;
    asm volatile ("add %0, sp, #0x24\n\tldrb %0, [%0]" : "=l" (t));
    eByteVal = t;

    health = (sub_80338DC() == 0) ? 0x18 : 0x10;

    InitActorPart(self, part, b, cParam, d);
    *(s32 *)(self + 0x54) = health;
    asm volatile (
        "ldr r0, =gStaticData_087E5554\n\t"
        "str r0, [r5, #0x50]\n\t"
        :
        :
        : "r0", "memory"
    );
    p59 = self + 0x59;
    zero = 0;
    *p59 = (u8)eByteVal;
    *(s32 *)(self + 0x28) = zero;
    byte59 = *p59;
    idx = 1;
    if (byte59 != 0) {
        idx = 0;
    }
    *(s32 *)(self + 0x28) = zero;
    *(s32 *)(self + 0x44) = zero;
    *(s32 *)(self + 0xc) = idx;
    {
        register u8 zeroByte asm("r1");
        u16 tmp16 = *(u16 *)(*(u8 **)self + idx * 12);
        zeroByte = 0;
        *(u16 *)(self + 0x10) = tmp16;
        self[0x12] = zeroByte;
        *(s32 *)(self + 8) = zero;
        self[0x58] = zeroByte;
    }
    asm volatile (
        "ldrb r0, [%0]\n\t"
        "cmp r0, #0\n\t"
        "beq 1f\n\t"
        "ldr r0, =0xFFFFBF00\n\t"
        "b 3f\n\t"
        ".pool\n"
        "1:\n\t"
        "mov r0, #0x84\n\t"
        "lsl r0, r0, #8\n"
        "3:\n\t"
        "str r0, [r5, #0x5c]\n\t"
        :
        : "r" (p59)
        : "r0", "memory"
    );
    *(s32 *)(self + 0x60) = 0xa00;
    *(s32 *)(self + 0x64) = -1;
    {
        register s32 zero2 asm("r4");
        u8 *p2c = self + 0x2c;
        zero2 = 0;
        *p2c = (u8)zero2;
        *(s32 *)(self + 0x68) = *(s32 *)((u8 *)sub_80338C4() + 4);
        *(s32 *)(self + 0x6c) = zero2;
    }

    return self;
}
