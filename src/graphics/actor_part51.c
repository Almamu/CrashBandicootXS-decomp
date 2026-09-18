#include "core.h"

/* Same "self" object family as actor_part39.c/actor_part41.c - see
 * actor_part39.c's header comment and
 * docs/matching/issue-50-actor-2a69c.md. */

/* A 12-byte little vector block: copies `self+0x38..0x44` into `*out`,
 * integrating a per-axis velocity (`self+0x1c`/`0x20`/`0x24`, each
 * `>>8`'d to a whole-unit delta) into the block's first three `s16`
 * slots along the way; the remaining three `s16` slots pass through
 * unchanged. See docs/matching/issue-50-actor-2a69c.md for the small
 * inline-asm islands this needed (the compiler's own list scheduler
 * reorders the three per-axis load/shift pairs and the RMW halfword
 * updates' register reuse differently from the ROM's literal order no
 * matter how the surrounding C is phrased). Declared returning `void *`
 * (not `void`) because the ROM's own epilogue physically returns
 * `outArg` unchanged: `r0` (the parameter) is never touched again after
 * being copied to `r2` for the final block copy, and the ROM's real
 * compiler used that still-live value to pick `r1` (not `r0`) for its
 * "pop a register, branch to it" epilogue step - the same
 * return-type-shapes-epilogue-register-choice gotcha already documented
 * for `sub_800697C`/`sub_8001214` in docs/matching.md. No caller of
 * this function has been matched yet to say whether the return value is
 * actually used. */
void *sub_802AA0C(void *outArg, void *selfArg)
{
    struct blob0xc { u32 w0, w1, w2; };

    u8 *self = selfArg;
    struct blob0xc buf = *(struct blob0xc *)(self + 0x38);
    register s32 d0 asm("r3");
    register s32 d1 asm("r5");
    register s32 d2 asm("r4");

    /* The ROM computes all three per-axis deltas up front (a strict
     * load/shift, load/shift, load/shift run) before touching any of
     * `buf`'s halfwords; this compiler's own instruction scheduler
     * always interleaves the three loads/shifts differently (hoisting
     * loads ahead of unrelated shifts to hide latency) regardless of
     * how the source groups or pins them - confirmed with several
     * different phrasings. A small inline-asm island, matching the
     * ROM's literal instruction order, sidesteps the scheduler for just
     * this one sequence while leaving the rest of the function real C. */
    asm("ldr %0, [%3, #0x1c]\n"
        "asr %0, %0, #8\n"
        "ldr %1, [%3, #0x20]\n"
        "asr %1, %1, #8\n"
        "ldr %2, [%3, #0x24]\n"
        "asr %2, %2, #8"
        : "=r" (d0), "=r" (d1), "=r" (d2)
        : "r" (self));

    {
        /* The ROM's three read-modify-write halfword updates reuse
         * registers in an ad hoc pattern that plain C never reproduces:
         * the first and third overwrite the just-used delta's own
         * register (r3, r4) with the sum, while the middle one leaves
         * the sum in the freshly loaded value's register (r1) instead -
         * not a consistent rule a normal expression/assignment phrasing
         * can steer this compiler's allocator into. Thumb `ldrh`/`strh`
         * also have no SP-relative form, so the ROM materializes `sp`
         * into a plain register (r2) first - a bare "r" input constraint
         * here would let gcc hand back `sp` itself as that register
         * (address-legal for a `ldr`, but out of range for `ldrh`'s
         * 3-bit base-register field), so this pins the copy to r2
         * explicitly instead of trusting the constraint to materialize
         * one. */
        register s16 *sp2 asm("r2") = (s16 *)&buf;

        asm volatile(
            "ldrh r1, [r2]\n"
            "add r3, r1, r3\n"
            "strh r3, [r2]\n"
            "ldrh r1, [r2, #2]\n"
            "add r1, r1, r5\n"
            "strh r1, [r2, #2]\n"
            "ldrh r3, [r2, #4]\n"
            "add r4, r3, r4\n"
            "strh r4, [r2, #4]"
            :
            : "r" (sp2), "r" (d1), "r" (d0), "r" (d2)
            : "r1", "r3", "r4", "memory");
    }

    {
        /* Plain `*(struct blob0xc *)outArg = buf;` picks a different
         * scratch-register triple here than the ROM's own re-use of
         * r4-r6 (the same registers the initial self->buf copy used,
         * long dead by this point) - register-pinning both ends forces
         * the same choice back. */
        register void *dst asm("r2") = outArg;
        register s16 *src asm("r1") = (s16 *)&buf;

        asm volatile(
            "ldmia r1!, {r4, r5, r6}\n"
            "stmia r2!, {r4, r5, r6}"
            :
            : "r" (dst), "r" (src)
            : "r4", "r5", "r6", "memory");
    }

    return outArg;
}
/* Trailing zero-fill padding to the next 4-byte boundary, matching the
 * ROM's own (the assembler's default NOP pad - "mov r8, r8" - mismatches
 * here; see the matching_decomp_alignment_fix precedent). */
asm(".align 2, 0");
