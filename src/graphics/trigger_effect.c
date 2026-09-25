#include "core.h"
#include "actor.h"

extern void *gUnknown_030012C0;
extern void *gUnknown_030012D0;
extern void *gUnknown_030012EC;

extern u8 sub_8023278(void *self);
extern s32 sub_801A878(u16 x, u16 y, u16 w, u16 h, s32 id);
extern s32 sub_80234E8(void *self, s32 handle);
extern struct actor *sub_8008434(u16 arg0, u16 arg1, u16 arg2, u16 arg3);
extern void sub_80087C0(void *part);
extern void sub_80087B4(void *part);
extern void sub_800872C(void *part, u8 val);
extern s32 sub_800815C(struct actor *part);
extern void sub_8008E94(void *manager, void *value);

/* One of the four "trigger effect type N" slots in the 15-slot dispatch
 * table at `gStaticData_0816C7D8` (docs/rom_map.md, "A family of
 * 'trigger effect type N' functions"). Tests bit 0 of
 * `gUnknown_030012C0+2`: if set, plays a sound only (id `0xB`, or the
 * shared fallback `0xC` if either `sub_8023278(gUnknown_030012C0)` is
 * true or `gUnknown_030012C0+0x8c` is nonzero) via `sub_801A878` +
 * `sub_80234E8`. Otherwise spawns a full visual effect: allocates a
 * part-object (`sub_8008434`), points its `+0x20` table pointer at
 * `gStaticData_084A5600`'s own first field (reached through
 * `gUnknown_030012D0`'s double pointer-to-pointer, same idiom as
 * `sub_801FDEC` in graphics_loading_1fdec.c) plus a fixed `0x180`
 * offset, tags it (`+0x2d = 7`), builds it via the standard
 * `sub_80087C0`/`sub_80087B4`/`sub_800872C` OAM trio, sets its `+0x29`
 * bitfield via `sub_800815C`, sets `field_0A` to the (always-zero,
 * since this is the "bit clear" branch) tested bit, registers it into
 * `gUnknown_030012EC`'s manager via `sub_8008E94`, and clears flag bit
 * 2 (`& -5`, the negative-constant bit-clear idiom, not `& ~5`).
 * `field_0x20`/`field_0x2d` are kept as raw offsets - they sit well
 * past `struct actor`'s documented 0x1c bytes, in the same not-yet-
 * understood per-spawn-variant tail `sub_8009F1C`/`sub_8009FB0`
 * (actor_part8.c) already leave as raw offsets.
 *
 * NOT YET BYTE-MATCHING - see docs/matching/issue-31-trigger-effect-
 * type-n.md for the full account; compiled only under
 * `make NON_MATCHING=1`, the NAKED transcription below (`#else`) is
 * used otherwise. This reconstruction gets within 11 bytes (out of
 * 248) of the ROM - every load/store, branch and call is confirmed
 * correct, only a handful of pure register-choice/scheduling details
 * remain unmatched:
 *   - The `sub_801A878`/`sub_8008434` calls' argument-marshalling
 *     order: the ROM fills the stack slot (`id`) and r1/r2
 *     (arg1/arg2) before r3 (the hard-pinned `a3`), but this compiler
 *     always moves a3's already-pinned register into its call slot
 *     first, ahead of the (unpinned) arg1/arg2 - confirmed via several
 *     restructuring attempts (plain aliasing locals, an empty
 *     `asm volatile` barrier touching arg1/arg2 first, hand-spelled
 *     `asm volatile` call blocks); the last of these gets the ordering
 *     right but only by hard-pinning arg1/arg2 too, which cascades
 *     into much deeper register-pressure breakage elsewhere (see next
 *     point).
 *   - Pinning `arg2` to `r7` explicitly (needed to make a hand-spelled
 *     asm call block's raw register references resolve correctly)
 *     reproduces the confirmed categorical r7-pin toolchain bug this
 *     project has hit before (docs/matching.md,
 *     sub_8007114/sub_802190C/sub_8021280/sub_8021480): the compiler
 *     silently drops `arg2`'s own truncation code and `r7`'s
 *     save/restore from the prologue's push/pop set entirely, leaving
 *     the function reading a garbage/uninitialized `r7`.
 *   - The `+0x2d` tag store's source register for the final `strb`
 *     (`r1` here vs. the ROM's `r2`) - a single-instruction, same-
 *     length cosmetic register choice that did not respond to any
 *     restructuring tried (reordering the two `part+0x20`/`part+0x2d`
 *     writes, forcing a separate `register u8 asm("r2")` temporary -
 *     the latter made the compiler skip the `r9`-held `tag` value
 *     entirely and just re-materialize the `7` constant directly into
 *     `r2`, which drops the `mov r9, r0` that otherwise matches the
 *     ROM one instruction earlier).
 *
 * Confirmed working techniques this reconstruction does establish
 * (kept in case a future pass wants to push the last few bytes
 * through, or reuse them for the three still-NAKED siblings below):
 * pinning the `gUnknown_030012C0` address (not its dereferenced value)
 * to `r9` via a plain `register void *self asm("r0") = (void
 * *)&gUnknown_030012C0;` initializer lets the compiler's own address-
 * of codegen manage the literal-pool placement correctly (a hand-
 * written `ldr r0, =symbol` pseudo-op inside `asm volatile` instead
 * pools at the far end of the whole translation unit, shared across
 * every sibling function's identical symbol reference - a real,
 * previously-undocumented gotcha for this project's established
 * "spell out the literal load in an asm island" idiom); the
 * `sub_8023278(...) || ...+0x8c` branch needs explicit `goto`s (not a
 * plain `||` expression) to reproduce the ROM's one-branch-plus-
 * fallthrough shape instead of two explicit branches; and the
 * `id`/`arg0`-truncation pair naturally duplicates per `goto`-reached
 * arm while the shared call marshalling re-merges on its own, matching
 * the ROM's single `bl sub_801A878` site, once written this way. */
#if NON_MATCHING
void sub_8020E84(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    /* arg1/arg2 are deliberately left unpinned - natural allocation
     * reaches r6/r7 on its own (matching the ROM), and an explicit
     * `register ... asm("r7")` pin for arg2 hits the confirmed
     * categorical r7-pin toolchain bug (silently dropped from the
     * prologue's push/pop set and its own truncation code, not just
     * mismatched) documented elsewhere in this project (docs/
     * matching.md, sub_8007114/sub_802190C/sub_8021280/sub_8021480). */
    register u16 a3 asm("r5") = arg3;
    register void *self asm("r0") = (void *)&gUnknown_030012C0;
    register void **pAddr asm("r9");
    register u8 bit asm("r8");
    register s32 bitLo asm("r2");

    /* self = gUnknown_030012C0; bit = ((u8 *)self)[2] & 1;
     * pAddr = &gUnknown_030012C0; - the address-of is a plain C
     * expression (so the compiler manages its own literal-pool
     * placement, landing it in the function's single combined pool
     * like every other reference in this file), but the rest is
     * spelled out because this compiler otherwise schedules the `1`
     * mask constant after the byte load (the ROM materializes it
     * first). */
    asm volatile(
        "mov r9, r0\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #1\n\t"
        "ldrb r2, [r0, #2]\n\t"
        "and r2, r1\n\t"
        "mov r8, r2\n\t"
        : "+r" (self), "=r" (bit), "=r" (pAddr), "=r" (bitLo)
        :
        : "r1");

    if (bitLo) {
        u16 a0;
        s32 id;
        struct actor *ret;

        if (sub_8023278(self))
            goto id_c;
        {
            /* Re-dereferences gUnknown_030012C0 through pAddr (r9)
             * rather than reusing `self` - self's own register (r0)
             * was clobbered by the sub_8023278 call above, matching
             * the ROM's own reload-after-call. */
            register void **tmp asm("r3") = pAddr;
            register u8 val asm("r0") = *((u8 *)*tmp + 0x8c);
            if (!val)
                goto id_b;
        }

    id_c:
        a0 = (u16)arg0;
        id = 0xc;
        goto do_call;

    id_b:
        a0 = (u16)arg0;
        id = 0xb;

    do_call:
        {
            u16 a1 = arg1;
            u16 a2 = arg2;
            ret = (struct actor *)sub_801A878(a0, a1, a2, a3, id);
        }
        sub_80234E8(gUnknown_030012C0, (s32)ret);
    } else {
        register u8 tag asm("r9") = 7;
        struct actor *part;
        {
            u16 a1 = arg1;
            u16 a2 = arg2;
            part = sub_8008434((u16)arg0, a1, a2, a3);
        }

        *(void **)((u8 *)part + 0x20) =
            (u8 *)*(void **)*(void **)gUnknown_030012D0 + 0x180;
        *((u8 *)part + 0x2d) = tag;

        sub_80087C0(part);
        sub_80087B4(part);
        sub_800872C(part, 0);

        {
            register s32 result asm("r0") = sub_800815C(part);
            register u8 *addr asm("r2") = (u8 *)part + 0x29;
            register s32 acc asm("r1");

            result &= 0xf;
            /* acc = -0x10; - see sub_801FDEC's own version of this
             * fix (graphics_loading_1fdec.c): this compiler otherwise
             * synthesizes -0x10 from the still-live r1=0xf mask
             * constant instead of the ROM's fresh mov/neg pair. */
            asm volatile("mov r1, #0x10\n\tneg r1, r1\n\t" : "=r" (acc));
            acc &= *addr;
            acc |= result;
            *addr = acc;
        }

        *((u8 *)part + 0xa) = bit;

        sub_8008E94(gUnknown_030012EC, part);
        {
            register s32 negFive asm("r0");

            /* part->flags &= -5; - the negative-constant bit-clear
             * idiom: plain C's `& ~4` folds to a single AND-immediate,
             * but the ROM materializes -5 via a fresh mov/neg pair
             * first. */
            asm volatile("mov r0, #5\n\tneg r0, r0\n\t" : "=r" (negFive));
            negFive &= part->flags;
            part->flags = negFive;
        }
    }
}
#else
/* Written as NAKED asm, not plain C: semantics fully understood and
 * every instruction's operation matches the ROM, but register
 * allocation drifted in two spots that resisted every restructuring
 * tried (the `arg0`/`arg1`/`arg2`/`arg3` parameter-home registers, and
 * the `gUnknown_030012C0` bit-test/`sub_8023278` call at function
 * entry) - see docs/rom_map.md and the full account this doc comment
 * used to carry, now folded into docs/matching/issue-33-trigger-
 * effect.md. Transcribed instruction-for-instruction from the ROM
 * disassembly instead, the same escape hatch used for
 * `sub_8001CB8`/`sub_8001DB4` (src/system/link_cable.c) - all four
 * "trigger effect type N" siblings below share this exact shape, just
 * different bit-test masks, sound ids and tag values. */
NAKED void sub_8020E84(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "sub sp, #4\n\t"
        "add r4, r0, #0\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r6, r1, #0x10\n\t"
        "lsl r2, r2, #0x10\n\t"
        "lsr r7, r2, #0x10\n\t"
        "lsl r3, r3, #0x10\n\t"
        "lsr r5, r3, #0x10\n\t"
        "ldr r0, 20f\n\t"
        "mov sb, r0\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #1\n\t"
        "ldrb r2, [r0, #2]\n\t"
        "and r2, r1\n\t"
        "mov r8, r2\n\t"
        "cmp r2, #0\n\t"
        "beq 4f\n\t"
        "bl sub_8023278\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne 1f\n\t"
        "mov r3, sb\n\t"
        "ldr r0, [r3]\n\t"
        "add r0, #0x8c\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 2f\n\t"
    "1:\n\t"
        "lsl r0, r4, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "mov r1, #0xc\n\t"
        "b 3f\n\t"
        ".align 2, 0\n"
    "20: .4byte gUnknown_030012C0\n"
    "2:\n\t"
        "lsl r0, r4, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "mov r1, #0xb\n\t"
    "3:\n\t"
        "str r1, [sp]\n\t"
        "add r1, r6, #0\n\t"
        "add r2, r7, #0\n\t"
        "add r3, r5, #0\n\t"
        "bl sub_801A878\n\t"
        "add r1, r0, #0\n\t"
        "ldr r0, 21f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_80234E8\n\t"
        "b 5f\n\t"
        ".align 2, 0\n"
    "21: .4byte gUnknown_030012C0\n"
    "4:\n\t"
        "mov r0, #7\n\t"
        "mov sb, r0\n\t"
        "lsl r0, r4, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "add r1, r6, #0\n\t"
        "add r2, r7, #0\n\t"
        "add r3, r5, #0\n\t"
        "bl sub_8008434\n\t"
        "add r4, r0, #0\n\t"
        "ldr r0, 22f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #0xc0\n\t"
        "lsl r1, r1, #1\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r4, #0x20]\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x2d\n\t"
        "mov r2, sb\n\t"
        "strb r2, [r0]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087C0\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087B4\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800872C\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_800815C\n\t"
        "add r2, r4, #0\n\t"
        "add r2, #0x29\n\t"
        "mov r1, #0xf\n\t"
        "and r0, r1\n\t"
        "mov r1, #0x10\n\t"
        "neg r1, r1\n\t"
        "ldrb r3, [r2]\n\t"
        "and r1, r3\n\t"
        "orr r1, r0\n\t"
        "strb r1, [r2]\n\t"
        "mov r0, r8\n\t"
        "strb r0, [r4, #0xa]\n\t"
        "ldr r0, 23f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8008E94\n\t"
        "mov r0, #5\n\t"
        "neg r0, r0\n\t"
        "ldrb r1, [r4, #0xc]\n\t"
        "and r0, r1\n\t"
        "strb r0, [r4, #0xc]\n\t"
    "5:\n\t"
        "add sp, #4\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "22: .4byte gUnknown_030012D0\n"
    "23: .4byte gUnknown_030012EC\n"
    );
}
#endif /* NON_MATCHING */

/* Same shape as `sub_8020E84` above (twin sibling, tests bit 1 of
 * `gUnknown_030012C0+2` instead of bit 0), sound ids `3`/`0xC`, tag
 * value `5`. Same NAKED-transcription rationale as `sub_8020E84`. */
NAKED void sub_8020F7C(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "sub sp, #4\n\t"
        "add r4, r0, #0\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r6, r1, #0x10\n\t"
        "lsl r2, r2, #0x10\n\t"
        "lsr r7, r2, #0x10\n\t"
        "lsl r3, r3, #0x10\n\t"
        "lsr r5, r3, #0x10\n\t"
        "ldr r0, 20f\n\t"
        "mov r8, r0\n\t"
        "ldr r1, [r0]\n\t"
        "mov r0, #2\n\t"
        "ldrb r2, [r1, #2]\n\t"
        "and r0, r2\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "mov sb, r0\n\t"
        "cmp r0, #0\n\t"
        "beq 4f\n\t"
        "add r0, r1, #0\n\t"
        "bl sub_8023278\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne 1f\n\t"
        "mov r3, r8\n\t"
        "ldr r0, [r3]\n\t"
        "add r0, #0x8c\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 2f\n\t"
    "1:\n\t"
        "lsl r0, r4, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "mov r1, #0xc\n\t"
        "b 3f\n\t"
        ".align 2, 0\n"
    "20: .4byte gUnknown_030012C0\n"
    "2:\n\t"
        "lsl r0, r4, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "mov r1, #3\n\t"
    "3:\n\t"
        "str r1, [sp]\n\t"
        "add r1, r6, #0\n\t"
        "add r2, r7, #0\n\t"
        "add r3, r5, #0\n\t"
        "bl sub_801A878\n\t"
        "add r1, r0, #0\n\t"
        "ldr r0, 21f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_80234E8\n\t"
        "b 5f\n\t"
        ".align 2, 0\n"
    "21: .4byte gUnknown_030012C0\n"
    "4:\n\t"
        "mov r0, #5\n\t"
        "mov r8, r0\n\t"
        "lsl r0, r4, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "add r1, r6, #0\n\t"
        "add r2, r7, #0\n\t"
        "add r3, r5, #0\n\t"
        "bl sub_8008434\n\t"
        "add r4, r0, #0\n\t"
        "ldr r0, 22f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #0xc0\n\t"
        "lsl r1, r1, #1\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r4, #0x20]\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x2d\n\t"
        "mov r2, r8\n\t"
        "strb r2, [r0]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087C0\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087B4\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800872C\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_800815C\n\t"
        "add r2, r4, #0\n\t"
        "add r2, #0x29\n\t"
        "mov r1, #0xf\n\t"
        "and r0, r1\n\t"
        "mov r1, #0x10\n\t"
        "neg r1, r1\n\t"
        "ldrb r3, [r2]\n\t"
        "and r1, r3\n\t"
        "orr r1, r0\n\t"
        "strb r1, [r2]\n\t"
        "mov r0, sb\n\t"
        "strb r0, [r4, #0xa]\n\t"
        "ldr r0, 23f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8008E94\n\t"
        "mov r0, #5\n\t"
        "neg r0, r0\n\t"
        "ldrb r1, [r4, #0xc]\n\t"
        "and r0, r1\n\t"
        "strb r0, [r4, #0xc]\n\t"
    "5:\n\t"
        "add sp, #4\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "22: .4byte gUnknown_030012D0\n"
    "23: .4byte gUnknown_030012EC\n"
    );
}

/* Same shape as `sub_8020E84` above (twin sibling, tests bit 2 of
 * `gUnknown_030012C0+2`), sound ids `0xA`/`0xC`, tag value `6`. Same
 * NAKED-transcription rationale as `sub_8020E84`. */
NAKED void sub_802107C(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "sub sp, #4\n\t"
        "add r4, r0, #0\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r6, r1, #0x10\n\t"
        "lsl r2, r2, #0x10\n\t"
        "lsr r7, r2, #0x10\n\t"
        "lsl r3, r3, #0x10\n\t"
        "lsr r5, r3, #0x10\n\t"
        "ldr r0, 20f\n\t"
        "mov r8, r0\n\t"
        "ldr r1, [r0]\n\t"
        "mov r0, #4\n\t"
        "ldrb r2, [r1, #2]\n\t"
        "and r0, r2\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "mov sb, r0\n\t"
        "cmp r0, #0\n\t"
        "beq 4f\n\t"
        "add r0, r1, #0\n\t"
        "bl sub_8023278\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne 1f\n\t"
        "mov r3, r8\n\t"
        "ldr r0, [r3]\n\t"
        "add r0, #0x8c\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 2f\n\t"
    "1:\n\t"
        "lsl r0, r4, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "mov r1, #0xc\n\t"
        "b 3f\n\t"
        ".align 2, 0\n"
    "20: .4byte gUnknown_030012C0\n"
    "2:\n\t"
        "lsl r0, r4, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "mov r1, #0xa\n\t"
    "3:\n\t"
        "str r1, [sp]\n\t"
        "add r1, r6, #0\n\t"
        "add r2, r7, #0\n\t"
        "add r3, r5, #0\n\t"
        "bl sub_801A878\n\t"
        "add r1, r0, #0\n\t"
        "ldr r0, 21f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_80234E8\n\t"
        "b 5f\n\t"
        ".align 2, 0\n"
    "21: .4byte gUnknown_030012C0\n"
    "4:\n\t"
        "mov r0, #6\n\t"
        "mov r8, r0\n\t"
        "lsl r0, r4, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "add r1, r6, #0\n\t"
        "add r2, r7, #0\n\t"
        "add r3, r5, #0\n\t"
        "bl sub_8008434\n\t"
        "add r4, r0, #0\n\t"
        "ldr r0, 22f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #0xc0\n\t"
        "lsl r1, r1, #1\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r4, #0x20]\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x2d\n\t"
        "mov r2, r8\n\t"
        "strb r2, [r0]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087C0\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087B4\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800872C\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_800815C\n\t"
        "add r2, r4, #0\n\t"
        "add r2, #0x29\n\t"
        "mov r1, #0xf\n\t"
        "and r0, r1\n\t"
        "mov r1, #0x10\n\t"
        "neg r1, r1\n\t"
        "ldrb r3, [r2]\n\t"
        "and r1, r3\n\t"
        "orr r1, r0\n\t"
        "strb r1, [r2]\n\t"
        "mov r0, sb\n\t"
        "strb r0, [r4, #0xa]\n\t"
        "ldr r0, 23f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8008E94\n\t"
        "mov r0, #5\n\t"
        "neg r0, r0\n\t"
        "ldrb r1, [r4, #0xc]\n\t"
        "and r0, r1\n\t"
        "strb r0, [r4, #0xc]\n\t"
    "5:\n\t"
        "add sp, #4\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "22: .4byte gUnknown_030012D0\n"
    "23: .4byte gUnknown_030012EC\n"
    );
}

/* Same shape as `sub_8020E84` above (twin sibling, tests bit 3 of
 * `gUnknown_030012C0+2`), sound ids `9`/`0xC`, tag value `8`. Same
 * NAKED-transcription rationale as `sub_8020E84` - this sibling needs
 * a third extra callee-saved register (`sl`/r10, holding the `8` tag
 * constant across the whole function) since gcc never reproduced this
 * one's shifted `r5`/`r6`/`r7` dx/dy/dz register roles either. */
NAKED void sub_802117C(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #4\n\t"
        "add r4, r0, #0\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r5, r1, #0x10\n\t"
        "lsl r2, r2, #0x10\n\t"
        "lsr r6, r2, #0x10\n\t"
        "lsl r3, r3, #0x10\n\t"
        "lsr r7, r3, #0x10\n\t"
        "ldr r0, 20f\n\t"
        "mov r8, r0\n\t"
        "ldr r1, [r0]\n\t"
        "mov r2, #8\n\t"
        "mov sl, r2\n\t"
        "mov r0, sl\n\t"
        "ldrb r3, [r1, #2]\n\t"
        "and r0, r3\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "mov sb, r0\n\t"
        "cmp r0, #0\n\t"
        "beq 4f\n\t"
        "add r0, r1, #0\n\t"
        "bl sub_8023278\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne 1f\n\t"
        "mov r1, r8\n\t"
        "ldr r0, [r1]\n\t"
        "add r0, #0x8c\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 2f\n\t"
    "1:\n\t"
        "lsl r0, r4, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "mov r1, #0xc\n\t"
        "b 3f\n\t"
        ".align 2, 0\n"
    "20: .4byte gUnknown_030012C0\n"
    "2:\n\t"
        "lsl r0, r4, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "mov r1, #9\n\t"
    "3:\n\t"
        "str r1, [sp]\n\t"
        "add r1, r5, #0\n\t"
        "add r2, r6, #0\n\t"
        "add r3, r7, #0\n\t"
        "bl sub_801A878\n\t"
        "add r1, r0, #0\n\t"
        "ldr r0, 21f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_80234E8\n\t"
        "b 5f\n\t"
        ".align 2, 0\n"
    "21: .4byte gUnknown_030012C0\n"
    "4:\n\t"
        "lsl r0, r4, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "add r1, r5, #0\n\t"
        "add r2, r6, #0\n\t"
        "add r3, r7, #0\n\t"
        "bl sub_8008434\n\t"
        "add r4, r0, #0\n\t"
        "ldr r0, 22f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0xc0\n\t"
        "lsl r2, r2, #1\n\t"
        "add r0, r0, r2\n\t"
        "str r0, [r4, #0x20]\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x2d\n\t"
        "mov r3, sl\n\t"
        "strb r3, [r0]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087C0\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087B4\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800872C\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_800815C\n\t"
        "add r2, r4, #0\n\t"
        "add r2, #0x29\n\t"
        "mov r1, #0xf\n\t"
        "and r0, r1\n\t"
        "mov r1, #0x10\n\t"
        "neg r1, r1\n\t"
        "ldrb r3, [r2]\n\t"
        "and r1, r3\n\t"
        "orr r1, r0\n\t"
        "strb r1, [r2]\n\t"
        "mov r0, sb\n\t"
        "strb r0, [r4, #0xa]\n\t"
        "ldr r0, 23f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8008E94\n\t"
        "mov r0, #5\n\t"
        "neg r0, r0\n\t"
        "ldrb r1, [r4, #0xc]\n\t"
        "and r0, r1\n\t"
        "strb r0, [r4, #0xc]\n\t"
    "5:\n\t"
        "add sp, #4\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "22: .4byte gUnknown_030012D0\n"
    "23: .4byte gUnknown_030012EC\n"
    );
}
