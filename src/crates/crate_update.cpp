#include "crate.hpp"
#include "player.hpp"

extern "C" {
#include "globals.h"
#include "player.h"
}

/* Crate::Update (#664, include/crate.hpp). */

/* The per-frame tick. While `timer` runs, it counts down, and on each
 * tick a lit TNT crate counts down (and the crate list is marked
 * changed), an idle slot crate spins, a bouncy wumpa crate's bounce ends
 * when the timer does, and an iron switch crate solidifies its outline
 * crates step by step. Then the bounce timer, the fall, and the end of a
 * hit's animation: the busy bit and the player's `busy` latch clear, and
 * a nitro switch crate turns into an iron one (an iron switch crate
 * shows its pressed animation). A committed crate (state 1) finishes
 * breaking instead. Last, the animation and the velocity.
 *
 * The shapes the ROM needs (docs/matching/archive/issue-12-13-25-naked-retry.md):
 * the 0x13-0x15 range test is two nested `if`s on an `s32` copy of `kind`
 * (one `&&` is folded into an unsigned subtract-and-compare); the slot
 * test and its `state` test are nested too (one `&&` merges the two byte
 * compares into a word compare); and the palette record is indexed from
 * a local copy of the records pointer, which loads the table before the
 * tag. */
void Crate::Update()
{
    if (timer != 0) {
        timer--;
        {
            s32 k = kind;

            if (k > CRATE_KIND_TIME_3) {
                if (k <= CRATE_KIND_TNT_LIT_3) {
                    UpdateTntCountdown();
                    gCrateListChanged = 1;
                    goto done;
                }
            }
        }
        if (kind == CRATE_KIND_SLOT) {
            if ((state & CRATE_STATE_MASK) == 0) {
                UpdateSlot();
                goto done;
            }
        }
        if (kind == CRATE_KIND_BOUNCY_WUMPA) {
            if (timer == 0)
                paramA = 0;
        } else if (kind == CRATE_KIND_IRON_SWITCH)
            SolidifyOutlines();
    done:;
    }
    if (kind == CRATE_KIND_BOUNCY_WUMPA && bounceTimer > 0)
        bounceTimer--;
    UpdateFall();
    if (state & CRATE_STATE_BUSY) {
        if (animDone != 0) {
            ClampFrame(0);
            animDone = 0;
            state &= CRATE_STATE_MASK;
            gPlayer->busy = 0;
            if (kind == CRATE_KIND_NITRO_SWITCH) {
                const struct sprite_anim *anims;
                const struct sprite_anim *anim;
                u32 slot; // a u32: the ROM zero-extends the u8 result before the 4-bit store

                kind = CRATE_KIND_IRON;
                SetTag(0x20);
                anims = bank->anims;
                anim = &anims[tag];
                slot = GetPaletteSlot(gPaletteCache, anim->paletteId);
                palette = slot;
            } else if (kind == CRATE_KIND_IRON_SWITCH) {
                SetTag(0x20);
            }
        }
    } else if ((state & CRATE_STATE_MASK) == 1)
        FinishBroken();
    AdvanceAnim();
    ApplyVelocity();
}
