#include "sprite_obj.hpp"
#include "frontend.hpp"

extern "C" {
#include "match.h"
#include "text.h"
#include "system.h"
#include "audio.h"
#include "globals.h"
}

/* The continue prompt's graphics (ContinuePrompt, frontend.hpp; called by
 * the constructor, continue_prompt_init.cpp): resets the OBJ VRAM cursor,
 * sets up gSmallFont (its tiles uploaded, no margin, its tiles reserved),
 * claims palette slots 0-3 and fills them from gContinuePromptPalette0-3,
 * turns the OBJs on and clears OAM.
 *
 * The palette loop is indexed through `i`: gcc strength-reduces every
 * access into its own pointer but keeps `i` as the up-counting trip
 * counter (r7) the ROM has. Written with explicit pointer increments, `i`
 * has nothing left to do but count, and gcc reverses it into a
 * down-counter. */
void ContinuePrompt::InitGraphics()
{
    PaletteCache *cache;
    s32 i;

    gObjVramCursor->baseTile = 0;
    gObjVramCursor->Reset();
    gObjVramCursor->Reset();

    icons = gSmallFont;
    icons->SetTileBase(0);
    icons->SetMargin(0);
    gObjVramCursor->Reserve(icons->tileCount << 5);
    gObjVramCursor->Mark();

    gPaletteCache->FreeUnlockedSlots();
    gPaletteCache->ClaimSlot(0);
    gPaletteCache->ClaimSlot(1);
    gPaletteCache->ClaimSlot(2);
    gPaletteCache->ClaimSlot(3);

    cache = gPaletteCache;
    {
        u16 *destA = (u16 *)cache->slots[0];
        u16 *destB = (u16 *)cache->slots[2];

        for (i = 0; i < 16; i++) {
            destA[i] = gContinuePromptPalette0[i];
            destA[i + 0x10] = gContinuePromptPalette1[i];
            destB[i] = gContinuePromptPalette2[i];
            destB[i + 0x10] = gContinuePromptPalette3[i];
        }
    }
    gPaletteCache->Upload();

    dispcnt.bits.obj = 1;

    gOamBuffer->Reset();
    gOamBuffer->HideUnused();
    WaitForVBlank();
    gOamBuffer->Commit();
}

/* The prompt's loop: up and down move the cursor between Yes (0) and No
 * (1), A or START confirms; every second frame BLDALPHA's EVA steps
 * down to 0 and back up to 16. Returns 1 for Yes.
 *
 * From the late-ROM retry passes (docs/matching/archive/late-rom-naked-retry.md):
 * `k` is a struct copy of the input word (the ROM's word load and `lsrs
 * #16` per test, then a fresh `ldrh` for the DPAD_DOWN test after the
 * calls), and the loop is `while (dir >= 0)` (with `while (1)` jump.c
 * moves the return computation to the `break`). Two empty asm statements,
 * which emit no code, are still needed in C++:
 *  - `MATCH_USE(audio)` at the top of the loop adds one reference to
 *    `audio`, so it outranks the pair counter for r7 (the counter lands in
 *    r8 without a pin);
 *  - `MATCH_KEEP(k)` between the A and START tests makes the second
 *    test's shift a fresh value, so CSE doesn't share it with the first,
 *    and the first test keeps the ROM's `movs r0, #1; ands r0, r1`
 *    register choice. The START test's `u16` result is the 0 the ROM
 *    then stores into `selection`. */
s32 ContinuePrompt::Loop()
{
    s32 dir = 1;
    s32 i = 0;
    s32 level = blend.bits.eva;
    struct held_pressed_pair *input = &gKeys.half;
    struct AudioContext **audio = &gAudioContext;

    while (dir >= 0) {
        MATCH_USE(audio); /* extra reference: audio outranks i for r7 */
        UpdateKeys(gInput);
        {
            struct held_pressed_pair k = *input;

            /* The MATCH_KEEP keeps the & 8 test's shift separate from the
             * & 1 test's. */
            // clang-format off
            if ((k.pressed & A_BUTTON) || ({ MATCH_KEEP(k); (u16)(k.pressed & START_BUTTON); })) {
                // clang-format on
                PlaySfx(*audio, SFX_MENU_SELECT, 0x100);
                break;
            }
            if ((k.pressed & DPAD_UP) && selection == 1) {
                PlaySfx(*audio, SFX_MENU_MOVE, 0x100);
                selection = 0;
            }
        }
        if ((input->pressed & DPAD_DOWN) && selection == 0) {
            PlaySfx(*audio, SFX_MENU_MOVE, 0x100);
            selection = 1;
        }
        Draw();
        CommitFrame();
        if (++i > 1) {
            i = 0;
            if (dir) {
                if (--level <= 0)
                    dir = 0;
            } else {
                if (++level > 15)
                    dir = 1;
            }
            blend.bits.eva = level;
            *(vu32 *)REG_ADDR_BLDCNT = blend.raw;
        }
    }
    return selection == 0;
}
