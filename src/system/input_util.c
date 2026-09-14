#include "core.h"

/* Sits between sub_800106C (ROM 0x0800106C, in src/util/time_util.c) and
 * LoadTaggedAsset (still raw in asm/code_3_1_5.s). */

#if NON_MATCHING
/* NOT YET BYTE-MATCHING - see docs/matching.md, "Parked, not matched:
 * sub_80010E0" for the full account; compiled only under
 * `make NON_MATCHING=1`, the checked-in assembly (asm/code_3_1_5.s) is
 * used otherwise. Logic and every register/instruction match the ROM
 * except a single 4-byte residual: one bit-test (`if (keys & 1)` inside
 * the count-limited loop) compiles here as `bne done; b continue`,
 * while the ROM has the opposite sense, `beq continue; b done` - the
 * same two instructions, same total size, just the inverted condition
 * first. Every rephrasing tried (positive/negative sense, explicit
 * goto-only chains, nested if/else, reordering this block before or
 * after the unlimited-loop variant in the source) produced the same
 * output; the identical check in the *unlimited*-loop variant (right
 * below) already compiles in the ROM's sense with no special handling,
 * so this looks like a fixed gcc-2.9 canonicalization for this specific
 * shape rather than anything under this file's control. */
extern void sub_80006A8(void);
extern s32 sub_80007AC(void *arg0);
extern void *gUnknown_03001304;
extern u16 gUnknown_030007E0;

/* Polls input (via sub_80006A8/sub_80007AC, the same VBlank-wait-then-
 * update-keys pair used elsewhere) until a button matching `mask`'s bit
 * 0 (confirm) or bit 3 (cancel) is newly pressed, or (if `count != 0`)
 * until `count` polls have elapsed. Returns 0 only if a cancel (bit 3)
 * press was seen; every other exit path (confirm press, or hitting the
 * poll-count limit) returns 1. `checkButtons` (nonzero) enables the
 * per-poll button checks at all - with it clear and a nonzero `count`,
 * this is just a `count`-poll delay that always returns 1; with it
 * clear and `count == 0`, it returns 1 immediately without polling at
 * all. Reads the "newly pressed this frame" keys (`gUnknown_030007E0`'s
 * companion u16 at +2, see `sub_80007AC`'s own notes in `src/system/irq.c`) via
 * a fresh `u16 *` each time, matching the ROM (no caching across polls,
 * since sub_80007AC's call in between could change it). The
 * `sub_80007AC(gUnknown_03001304)` calls pass an argument the real,
 * already-matched `sub_80007AC(void)` (in `src/system/irq.c`) never reads -
 * same "ROM sets up an arg the callee ignores" shape as `sub_80006A8`
 * itself; declared here with a dummy `void *` parameter purely so this
 * call site's leftover r0 setup matches the ROM's bytes. `keys` is
 * pinned to r1 and set via an inline-asm copy of `mask` (rather than a
 * plain `keys = mask & ...`) to reproduce the ROM's redundant
 * `adds r1, mask, #0` before the load - gcc's own codegen for the
 * combined expression instead loads straight into r1 and ANDs with the
 * mask register in place, one instruction shorter (but not what the
 * ROM does). The `keys &= 8;` bit-8 checks are similarly written as an
 * in-place AND (instead of `if ((keys & 8) != 0)`) so the result lands
 * back in `keys`'s own register (matching the ROM's `ands r1, r0`)
 * rather than a fresh scratch register. */
s32 sub_80010E0(s32 count, u8 checkButtons, s32 mask)
{
    s32 result;
    register s32 i asm("r5");
    register u8 flagR asm("r4");
    register s32 keys asm("r1");
    u16 *addr;

    flagR = checkButtons;
    result = 1;

    if (count == 0) {
        goto noLimit;
    }
    i = 0;
    goto checkCount;

loopCount:
    sub_80006A8();
    sub_80007AC(gUnknown_03001304);
    addr = &gUnknown_030007E0;
    asm volatile("add %0, %1, #0" : "=r"(keys) : "r"(mask));
    keys &= *(u16 *)((u8 *)addr + 2);
    if (flagR != 0) {
        if ((keys & 1) != 0) {
            goto done;
        }
        keys &= 8;
        if (keys != 0) {
            goto fail;
        }
    }
    i++;
checkCount:
    if (i >= count) {
        goto done;
    }
    goto loopCount;

noLimit:
    if (flagR == 0) {
        goto done;
    }
loopNoLimit:
    sub_80006A8();
    sub_80007AC(gUnknown_03001304);
    addr = &gUnknown_030007E0;
    asm volatile("add %0, %1, #0" : "=r"(keys) : "r"(mask));
    keys &= *(u16 *)((u8 *)addr + 2);
    if ((keys & 1) != 0) {
        goto done;
    }
    keys &= 8;
    if (keys == 0) {
        goto loopNoLimit;
    }

fail:
    result = 0;

done:
    return result;
}
#endif /* NON_MATCHING */
