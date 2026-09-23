#include "core.h"

/* Sits between FormatCentiseconds (ROM 0x0800106C, in src/util/time_util.c) and
 * LoadTaggedAsset (still raw in asm/code_3_1_5.s). */

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
 * companion u16 at +2, see `sub_80007AC`'s own notes in
 * `src/system/irq.c`) fresh each poll (no caching across polls, since
 * `sub_80007AC`'s call in between could change it). The
 * `sub_80007AC(gUnknown_03001304)` calls pass an argument the real,
 * already-matched `sub_80007AC(void)` (in `src/system/irq.c`) never
 * reads - same "ROM sets up an arg the callee ignores" shape as
 * `sub_80006A8` itself; declared here with a dummy `void *` parameter
 * purely so this call site's leftover r0 setup matches the ROM's
 * bytes. `keys` is pinned to r1 and set via an inline-asm copy of
 * `mask` (rather than a plain `keys = mask & ...`) to reproduce the
 * ROM's redundant `adds r1, mask, #0` before the load - gcc's own
 * codegen for the combined expression instead loads straight into r1
 * and ANDs with the mask register in place, one instruction shorter
 * (but not what the ROM does).
 *
 * The count-limited loop's cancel-bit check (`keys &= 8; if (keys) goto
 * fail;`) is written textually *before* the increment/poll code, with
 * the poll code branching backward into it (`goto cancelCheck`) on a
 * confirm-bit miss, rather than the more natural top-to-bottom order
 * (poll, check confirm, check cancel, increment). The ROM's own basic
 * block layout puts the cancel-check first too - matching that layout
 * (not just the branch *sense*) is what gets gcc to emit the ROM's
 * exact `beq cancelCheck; b done` pair for the confirm-bit test,
 * instead of the `bne done; b cancelCheck` pair gcc always produces
 * for the equivalent code in natural top-to-bottom order (confirmed:
 * every rephrasing that kept the cancel-check *after* the confirm-check
 * textually - positive/negative sense, if/else, nested vs flat -
 * produced the same `bne`-first output regardless of source-level
 * phrasing, since it's the source's block *order*, not its condition
 * polarity, driving this compiler's branch layout here). The identical
 * check in the *unlimited*-loop variant right below needs no such
 * reordering since it has no separate cancel-check block to place. */
s32 sub_80010E0(s32 count, u8 checkButtons, s32 mask)
{
    s32 result;
    register s32 i asm("r5");
    register u8 flagR asm("r4");
    register s32 keys asm("r1");
    s32 confirm;
    u16 *addr;

    flagR = checkButtons;
    result = 1;

    if (count == 0) {
        goto noLimit;
    }
    i = 0;
    goto checkCount;

cancelCheck:
    keys &= 8;
    if (keys != 0) {
        goto fail;
    }
increment:
    i++;
checkCount:
    if (i >= count) {
        goto done;
    }
    sub_80006A8();
    sub_80007AC(gUnknown_03001304);
    addr = &gUnknown_030007E0;
    asm volatile("add %0, %1, #0" : "=r"(keys) : "r"(mask));
    keys &= *(u16 *)((u8 *)addr + 2);
    if (flagR == 0) {
        goto increment;
    }
    confirm = keys & 1;
    if (confirm == 0) {
        goto cancelCheck;
    }
    goto done;

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
