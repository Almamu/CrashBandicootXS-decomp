extern "C" {
#include "core.h"
#include "system.h"
#include "globals.h"
}

/* Sits between FormatCentiseconds (ROM 0x0800106C, in src/util/time_format.cpp) and
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
 * `src/system/irq.cpp`) fresh each poll (no caching across polls, since
 * `UpdateKeys`'s call in between could change it). `UpdateKeys`
 * takes the input object (`gInput`, in r0 at every call) but never
 * reads it (system.h). Built with old_agbcc: agbcc loads `pressed`
 * straight into r1 and ANDs `mask` into it, where the ROM (and
 * old_agbcc) copies `mask` to r1 first (`adds r1, r7, #0`).
 *
 * The block order comes from the C front end's loop rotation
 * (expand_end_loop): it moves the counted loop's code from the top up to
 * its last exit (`break` on a confirm press) behind the body, so the
 * cancel test, which leaves through `goto fail` rather than the loop's
 * end, stays first and the confirm test ends in `beq cancel; b done`.
 * The `(u16)` on `mask` (the key bits are a halfword) puts the flag in
 * r4 and `i` in r5 as in the ROM; with a plain `mask &` old_agbcc swaps
 * them (#662 round 2, found by the permuter; until then a goto
 * transcription with `checkButtons` pinned to r4). */
s32 WaitForKeyPress(s32 count, u8 checkButtons, s32 mask)
{
    s32 result = 1;
    s32 i;
    s32 keys;

    if (count != 0) {
        for (i = 0; i < count; i++) {
            WaitForVBlank();
            UpdateKeys(gInput);
            keys = (u16)mask & gKeys.half.pressed;
            if (checkButtons) {
                if (keys & 1) {
                    break;
                }
                if (keys & 8) {
                    goto fail;
                }
            }
        }
    } else if (checkButtons) {
        for (;;) {
            WaitForVBlank();
            UpdateKeys(gInput);
            keys = (u16)mask & gKeys.half.pressed;
            if (keys & 1) {
                break;
            }
            if (keys & 8) {
            fail:
                result = 0;
                break;
            }
        }
    }
    return result;
}
