#include "core.h"
#include "match.h"
#include "system.h"
#include "globals.h"

/* Sits between FormatCentiseconds (ROM 0x0800106C, in src/util/time_format.c) and
 * LoadTaggedAsset (still raw in asm/code_3_1_5.s). */

/* Polls input (via WaitForVBlank/UpdateKeys, the same VBlank-wait-then-
 * update-keys pair used elsewhere) until a button matching `mask`'s bit
 * 0 (confirm) or bit 3 (cancel) is newly pressed, or (if `count != 0`)
 * until `count` polls have elapsed. Returns 0 only if a cancel (bit 3)
 * press was seen; every other exit path (confirm press, or hitting the
 * poll-count limit) returns 1. `checkButtons` (nonzero) enables the
 * per-poll button checks at all - with it clear and a nonzero `count`,
 * this is just a `count`-poll delay that always returns 1; with it
 * clear and `count == 0`, it returns 1 immediately without polling at
 * all. Reads the "newly pressed this frame" keys (`gKeys`'s
 * companion u16 at +2, see `UpdateKeys`'s own notes in
 * `src/system/irq.c`) fresh each poll (no caching across polls, since
 * `UpdateKeys`'s call in between could change it). `UpdateKeys`
 * takes the input object (`gInput`, in r0 at every call) but never
 * reads it (system.h). Built with old_agbcc: agbcc loads `pressed`
 * straight into r1 and ANDs `mask` into it, where the ROM (and
 * old_agbcc) copies `mask` to r1 first (`adds r1, r7, #0`).
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
s32 WaitForKeyPress(s32 count, u8 checkButtons, s32 mask)
{
    s32 result;
    s32 i;
    /* The ROM has the flag in r4 and `i` in r5; old_agbcc swaps them
     * without the pin (every loop shape tried). */
    MATCH_HOLD_REG(u8, flagR, r4);
    s32 keys;
    s32 confirm;
    struct held_pressed_pair *addr;

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
    WaitForVBlank();
    UpdateKeys(gInput);
    addr = &gKeys.half;
    keys = mask & addr->pressed;
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
    WaitForVBlank();
    UpdateKeys(gInput);
    addr = &gKeys.half;
    keys = mask & addr->pressed;
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
