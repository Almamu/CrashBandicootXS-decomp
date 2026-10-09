#include "menus.hpp"
#include "font.hpp"

extern "C" {
#include "core.h"
#include "match.h"
#include "system.h"
#include "text.h"
#include "util.h"
#include <libgcc.h>
#include "gfx.h"
#include "globals.h"
}

/* PauseMenu's Animate, Draw and DrawRows (menus.hpp; C++ since the #664
 * cleanup), between Loop (pause_menu_loop.cpp) and DrawPowersPage
 * (pause_menu_powers.cpp) in the ROM. See
 * docs/matching/archive/issue-7-0x08004d74-overlay-ui.md. */

/* Animates the info page shown (`page`, 0-4: each of its icons'
 * AdvanceAnim), moves on to the next page every 180 frames, and blinks
 * the eyelids (`blinkEyes`) over the background's eyes: while
 * `blinkTimer` counts down nothing shows; at 0 the blink plays, and once
 * it is done it is rewound and a new countdown (0x78-0xef) starts. */
void PauseMenu::Animate()
{
    switch (page) {
    case 0:
        crystalIcon->AdvanceAnim();
        break;
    case 1:
        {
            UiSprite **p = powerIcons;
            s32 i;
            for (i = 3; i >= 0; i--) {
                (*p)->AdvanceAnim();
                p++;
            }
            break;
        }
    case 2:
        {
            UiSprite **p = gemIcons;
            s32 i;
            for (i = 4; i >= 0; i--) {
                (*p)->AdvanceAnim();
                p++;
            }
            break;
        }
    case 3:
        {
            UiSprite **p = relicIcons;
            s32 i;
            for (i = 2; i >= 0; i--) {
                (*p)->AdvanceAnim();
                p++;
            }
            break;
        }
    case 4:
        trialIcon->AdvanceAnim();
        break;
    }

    pageTimer--;
    if (pageTimer == 0) {
        page++;
        page = __modsi3(page, 5);
        pageTimer = 0xb4;
    }

    {
        s32 *countAddr = &blinkTimer;
        s32 result;

        if (*countAddr != 0) {
            goto decrement;
        }
        {
            UiSprite *icon = blinkEyes;

            if (icon->animDone != 0) {
                icon->tag = 0;
                icon->ResetFrameTimer();
                icon->ResetFrameIndex();
                icon->SetAnimDone(0);
                result = (u16)RandRange(0x78) + 0x78;
                goto store;
            } else {
                icon->AdvanceAnim();
                goto done;
            }
        }
    decrement:
        result = *countAddr - 1;
    store:
        *countAddr = result;
    done:;
    }
}

/* Draws the frame: the level's name centred at the top, "LEVEL N" under
 * it for the numbered levels, the completion percentage right-aligned,
 * the rows, the info page's title and the page itself, and the eyelids
 * while they blink. */
void PauseMenu::Draw()
{
    void *label;
    u32 width;

    gOamBuffer->Reset();
    gObjVramCursor->Rewind();
    {
        u32 w = gLargeFont->MeasureText((u8 *)levelName);
        u32 x, t;
        MATCH_HOLD_REG(s32, hold, r2);

        t = 0xf0 - w;
        x = t >> 1;
        /* Extra reference (no code): keeps `t` (r1) from being tied
         * to `x` (r3). */
        MATCH_USE(t);
        /* Hard-register hold (no code): `hold`, never assigned, keeps r2
         * live from its declaration to here, so y takes it afterwards.
         * #662 round 2: SetPos with the expressions as arguments or an
         * `x` local give x r1 and y r3 (direct posX/posY stores are
         * further off), and the permuter found nothing in 43k
         * iterations.
         * #662 round 3: local-alloc. Plain, the shift result is tied to
         * the dying subtraction (r1) and, at the same priority as
         * SetPos's inlined `y` parameter (2 refs over 5 insns each), is
         * allocated first because it is born first; `y` then gets r3.
         * The ROM allocates `y` first (r2) and leaves x untied (r3).
         * Tried: centring/right-align inline helpers, `y` and the 0xf0
         * as locals in either order, s32/u32 SetPos parameters, old_agbcp
         * and every -fno-* flag family. */
        MATCH_USE(hold);
        gLargeFont->SetPos(x, 0xe);
    }
    gLargeFont->DrawText((u8 *)levelName);
    label = levelLabel;
    if (label != NULL) {
        gLargeFont->SetPos(0x20, 0x26);
        gLargeFont->DrawText((u8 *)label);
        gLargeFont->DrawText(levelNumber);
    }
    width = gLargeFont->MeasureText(percentText);
    {
        u32 x, t;
        MATCH_HOLD_REG(s32, hold, r2);

        t = 0x8c;
        x = t - width;
        /* Extra references (no code): neither the 0x8c (r1) nor
         * `width` (r0) is tied to `x` (r3). */
        MATCH_USE2(t, width);
        /* Hard-register hold (no code), as above: the same local-alloc
         * tie, x tied to the dying 0x8c (round 3: `t`/`x`/`y` locals in
         * either order, `-width + 0x8c`, a RightX helper don't help). */
        MATCH_USE(hold);
        gLargeFont->SetPos(x, 0x88);
    }
    gLargeFont->DrawText(percentText);
    DrawRows();
    DrawPageTitle();
    switch (page) {
    case 0:
        DrawCrystalsPage();
        break;
    case 1:
        DrawPowersPage();
        break;
    case 2:
        DrawGemsPage();
        break;
    case 3:
        DrawRelicsPage();
        break;
    case 4:
        DrawTimeTrialPage();
        break;
    }
    if (blinkTimer == 0)
        blinkEyes->DrawWithOffset(0, 0);
    gOamBuffer->HideUnused();
}

/* Draws the rows (`rows`, `rowCount` of them) from y = 0x4a,
 * `rowSpacing` apart, each centred on x = 0x32, the selected one
 * (`cursor`) in palette 15. The music and sound rows (types 4 and 5)
 * are followed by their " <NN%>" volume on the same line, the pair
 * centred together. */
void PauseMenu::DrawRows()
{
    s32 y = 0x4a;
    s32 i;

    for (i = 0; i < rowCount; i++) {
        u8 *label;
        s32 x;

        if (i == cursor)
            gSmallFont->SetPalette(0xf);
        else
            gSmallFont->ResetPalette();
        label = (u8 *)GetUiText(rows[i].labelId);
        x = 0x32 - ((u32)gSmallFont->MeasureText(label) >> 1);
        switch (rows[i].type) {
        case 4:
            x -= (u32)gSmallFont->MeasureText(musicVolumeText) >> 1;
            gSmallFont->SetPos(x, y);
            gSmallFont->DrawText(label);
            gSmallFont->DrawText(musicVolumeText);
            break;
        case 5:
            x -= (u32)gSmallFont->MeasureText(soundVolumeText) >> 1;
            gSmallFont->SetPos(x, y);
            gSmallFont->DrawText(label);
            gSmallFont->DrawText(soundVolumeText);
            break;
        default:
            gSmallFont->SetPos(x, y);
            gSmallFont->DrawText(label);
            break;
        }
        y += rowSpacing;
    }
    gSmallFont->ResetPalette();
}
