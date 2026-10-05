#include "core.h"
#include "actor.h"

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see game_loop17.c's header comment and
 * docs/matching/issue-13-graphics-fc70.md). `GetTopCrate`/
 * `GetBottomCrate`/`CollideCrateWithPlayer` right after this function are matched in
 * crate_stack.c. */

extern void *gAudioContext;
extern void *gEntityFlags;
extern void *gEntitySpawner;
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);
extern s32 sub_802599C(void *self, s32 n);
extern void sub_80259D4(void *self, s32 n);
/* DropExtraLife is parked (NON_MATCHING) as of game_loop14.c. This call
 * site's own arguments are spelled out entirely in inline asm below -
 * see the comment right above that block for why. */
extern struct actor *DropExtraLife(void *unused0, s32 x, s32 y, u8 p3, u32 p5, u8 flag6);

/* Plays cue-3 SFX, then - unless `self->field_08` is the sentinel
 * `0xffff` - consumes a slot from the per-record bit-grid
 * (`gEntityFlags`, the same `sub_802599C`/`sub_80259D4` accessor
 * pair game_loop12.c/game_loop13.c already establish) keyed by
 * `self->field_08`, setting the bit only if it wasn't already set.
 * Finally spawns a part object (`DropExtraLife`) three tiles below
 * `self`'s own position, tagged with the caller's own byte argument. */
void OpenLifeCrate(struct actor *self, u32 arg1)
{
    u8 flag6 = (u8)arg1;

    PlaySfx(gAudioContext, 3, 0x100);

    if (self->field_08 != 0xFFFF) {
        if ((u8)sub_802599C(gEntityFlags, self->field_08) == 0) {
            sub_80259D4(gEntityFlags, self->field_08);
        }
    }

    /* The ROM stores this call's stack-passed 6th argument (`flag6`, a
     * plain u8) through a computed `add r3, sp, #4`/`strb r6, [r3]`
     * pair - Thumb1 has no sp-relative byte-store encoding, so the
     * byte has to go through a register base - whereas this compiler
     * always emits a direct word-sized `str` for a stack argument
     * regardless of the parameter's declared width (same gap already
     * closed for `DrawSaveMenuMain`'s own `DrawSaveSlotStats` call in
     * save_menu_input.c - see docs/matching/issue-5-overlay-ui-sync.md).
     * The whole call is spelled out in asm to match; a dummy 2-word
     * local's address is taken as an unused input operand purely to
     * make this compiler reserve the same 8-byte outgoing-argument
     * stack slot pair (5th arg `p5`'s word, 6th arg `flag6`'s byte)
     * the ROM's own `sub sp, #8`/`add sp, #8` frame does. */
    {
        u32 dummy[2];
        asm volatile(
            "ldr r1, [%0]\n"
            "asr r1, r1, #8\n"
            "ldr r2, [%0, #4]\n"
            "asr r2, r2, #8\n"
            "add r2, r2, #3\n"
            "ldr r0, =gEntitySpawner\n"
            "ldr r0, [r0]\n"
            "mov r3, #3\n"
            "str r3, [sp]\n"
            "add r3, sp, #4\n"
            "strb %1, [r3]\n"
            "mov r3, #0\n"
            "bl DropExtraLife\n"
            :
            : "r" (self), "r" (flag6), "r" (dummy)
            : "r0", "r1", "r2", "r3", "r12", "lr", "cc", "memory"
        );
    }
}

asm(".pool");

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see game_loop17.c's header comment and
 * docs/matching/issue-13-graphics-fc70.md). `OpenLifeCrate` right
 * before this function is left untouched raw. */

extern u8 gCrateKindBreakable[];

/* Trivial byte-table lookup: `gCrateKindBreakable[idx]`. The first
 * parameter is unused in the ROM. */
u8 IsCrateKindBreakable(void *arg0, u32 idx)
{
    return gCrateKindBreakable[idx];
}

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see game_loop17.c's header comment and
 * docs/matching/issue-13-graphics-fc70.md). `GetSlotCrateStage` right after
 * this file is already matched in slot_crate.c. `self` throughout
 * this file is the same actor/"collision box" object every other
 * function in this subsystem operates on - offsets `0`/`4`/`0xc` here
 * line up with `struct actor`'s own `x`/`y`/`flags` fields
 * (include/actor.h), kept as raw offsets rather than that struct type
 * to stay consistent with every already-matched sibling in this file
 * family (crate_reset.c-game_loop29.c), which do the same. */

extern void *GetCrateBelow(void *selfArg);
extern void *GetCrateAbove(void *selfArg);
extern void QueueCratePlayerCollision(void *selfArg);

/* Walks the "get prev" neighbor-list chain (`GetCrateAbove`) starting at
 * `self`, returning the furthest node reachable while every node
 * visited (other than `self` itself) has a `+0x4d & 0x7f` state != 1 -
 * the walk stops (returning the last accepted node) as soon as it
 * would step onto a node in state 1, or runs out of neighbors. If
 * `self` has no "prev" neighbor at all, or that first neighbor is
 * already in state 1, falls back to returning `self` itself:
 *
 *   cur = GetCrateAbove(self);
 *   if (cur == NULL) return self;
 *   if ((cur[0x4d] & 0x7f) == 1) return self;
 *   for (;;) {
 *       next = GetCrateAbove(cur);
 *       if (next == NULL) return cur;
 *       if ((next[0x4d] & 0x7f) == 1) return cur;
 *       cur = next;
 *   }
 *
 * Was NAKED asm, not plain C - see
 * docs/matching/naked-GetTopCrate-matched.md for the derivation. Two
 * gaps: this compiler's cross-jump pass merges the loop's two
 * `return cur;` sites into a single shared tail positioned right
 * before the epilogue (saving a branch the ROM's own, separately-kept
 * copy still pays for), and the mask check normally computes the
 * struct-field address and the `0x7f` mask constant in the opposite
 * order from the ROM. Both closed below: `goto`s placing the shared
 * `returnCur:` block *before* the loop body in source order (matching
 * the ROM's own physical layout, so the loop's exit checks become
 * backward branches into it, keeping the ROM's separate-copy byte
 * count), and an inline-asm-materialized mask check matching the
 * ROM's own instruction order (address-then-mask-then-load, not
 * load-then-mask). */
void *GetTopCrate(void *selfArg)
{
    void *self = selfArg;
    void *cur;
    void *next;
    register u32 masked asm("r0");

    cur = GetCrateAbove(self);
    if (cur == NULL) {
        goto returnSelf;
    }
    asm volatile(
        "add r1, %1, #0\n\t"
        "add r1, r1, #0x4d\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r0, r1\n\t"
        : "=r"(masked)
        : "r"(cur)
        : "r1", "cc"
    );
    if (masked != 1) {
        goto loop;
    }

returnSelf:
    return self;

returnCur:
    return cur;

loop:
    next = GetCrateAbove(cur);
    if (next == NULL) {
        goto returnCur;
    }
    asm volatile(
        "add r1, %1, #0\n\t"
        "add r1, r1, #0x4d\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r0, r1\n\t"
        : "=r"(masked)
        : "r"(next)
        : "r1", "cc"
    );
    if (masked == 1) {
        goto returnCur;
    }
    cur = next;
    goto loop;
}

/* Same walk as `GetTopCrate`, but over the "get next" chain
 * (`GetCrateBelow`) instead of "get prev". Same matching technique as
 * that function - see docs/matching/naked-GetTopCrate-matched.md. */
void *GetBottomCrate(void *selfArg)
{
    void *self = selfArg;
    void *cur;
    void *next;
    register u32 masked asm("r0");

    cur = GetCrateBelow(self);
    if (cur == NULL) {
        goto returnSelf;
    }
    asm volatile(
        "add r1, %1, #0\n\t"
        "add r1, r1, #0x4d\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r0, r1\n\t"
        : "=r"(masked)
        : "r"(cur)
        : "r1", "cc"
    );
    if (masked != 1) {
        goto loop;
    }

returnSelf:
    return self;

returnCur:
    return cur;

loop:
    next = GetCrateBelow(cur);
    if (next == NULL) {
        goto returnCur;
    }
    asm volatile(
        "add r1, %1, #0\n\t"
        "add r1, r1, #0x4d\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r0, r1\n\t"
        : "=r"(masked)
        : "r"(next)
        : "r1", "cc"
    );
    if (masked == 1) {
        goto returnCur;
    }
    cur = next;
    goto loop;
}

/* Unless `self`'s own `+0x4d & 0x7f` state is 1, and `testX`/`testY`
 * (both raw, same Q8 scale as `self`'s own `+0`/`+4` position pair)
 * are both within `0x3fff` of `self`'s position, and `self`'s `+0x4e`
 * byte isn't `5`, fires `QueueCratePlayerCollision(self)` - the physics/collision
 * subsystem's own collision-response commit
 * (docs/matching/issue-12-physics-collision.md). Always clears
 * `self`'s own `+0xc` flags bit 3 before returning, unconditionally. */
s32 CollideCrateWithPlayer(void *selfArg, u32 unused1, s32 testX, s32 testY)
{
    /* Pinned to r4: the ROM keeps `self` in r4 for the whole function
     * (only the transient mask-check scratch below uses r5/r6/ip), and
     * this compiler's own unforced allocator drifts it onto r6 instead
     * once the tail's `loaded asm("r6")` pin is in scope. */
    register u8 *self asm("r4") = selfArg;
    u32 masked;

    asm volatile(
        "mov r0, #0x4d\n\t"
        "add r0, r0, %1\n\t"
        "mov ip, r0\n\t"
        "mov r0, #0x7f\n\t"
        "mov r5, ip\n\t"
        "ldrb r5, [r5]\n\t"
        "and r0, r0, r5\n\t"
        : "=r"(masked)
        : "r"(self)
        : "r5", "ip", "cc"
    );

    if (masked != 1) {
        s32 dx = *(s32 *)self - testX;
        if (dx < 0) {
            dx = -dx;
        }
        /* Register-pinned, and declared only here (not at the top of
         * the enclosing block): the ROM loads the literal-pool
         * `0x3fff` bound into r2 only *after* `testX` (the incoming
         * parameter register, also r2) is already consumed by the dx
         * computation above, then reuses that same r2 for both the dx
         * and dy comparisons. A pin declared any earlier claims r2 for
         * its whole lexical scope, forcing this compiler to relocate
         * `testX` out of r2 into r1 pre-emptively instead. */
        {
            register s32 limit asm("r2") = 0x3FFF;
            if (dx <= limit) {
                s32 dy = *(s32 *)(self + 4) - testY;
                if (dy < 0) {
                    dy = -dy;
                }
                if (dy <= limit) {
                    if (self[0x4e] != 5) {
                        QueueCratePlayerCollision(self);
                    }
                }
            }
        }
    }

    /* Inline-asm-anchored: the ROM computes the `~8` clear-mask at
     * runtime (`movs r0,#9; rsbs r0,r0,#0`, the negative-constant
     * register-pinned mask idiom - matching_decomp_register_pinning)
     * rather than folding it into an 8-bit AND immediate, and keeps
     * the loaded byte in r6, AND-ing into r0 (not r6). A plain
     * `register u8 loaded asm("r6") = self[0xc];` gets optimized away
     * (its single use inlines straight into the AND, dropping the r6
     * pin entirely) - anchored as one literal block instead, since
     * `self` is already known to sit in r4 throughout this function. */
    asm volatile(
        "mov r0, #0x9\n\t"
        "neg r0, r0\n\t"
        "ldrb r6, [r4, #0xc]\n\t"
        "and r0, r0, r6\n\t"
        "strb r0, [r4, #0xc]\n\t"
        :
        :
        : "r0", "r6", "cc", "memory"
    );
    return 0;
}
