#include "hud.hpp"
#include "level_state.hpp"

extern "C" {
#include "core.h"
#include "gfx.h"
#include "memory.h"
#include "level.h"
#include "globals.h"
}

/* Hud's constructor and ConfigureParts (#664 cleanup, include/hud.hpp).
 * Built with old_agbcp, as the C was with old_agbcc - see
 * docs/matching/archive/game-loop-old-agbcc.md. */

#define HUD_ANIM(offset) ((const struct sprite_bank *)(SPRITE_BANK_BASE + (offset)))
#define SLOT_RECORD(s) ((s)->bank->anims[(s)->tag])

static inline void RestartSlot(HudPart *slot)
{
    slot->ResetFrameTimer();
    slot->ResetFrameIndex();
    slot->SetAnimDone(0);
}

/* Shows animation frame `frame`, clamped to the animation's last one. */
static inline void SetSlotFrame(HudPart *slot, s32 frame)
{
    s32 n = SLOT_RECORD(slot).frameCount;
    CLAMP_INDEX(frame, n);
    slot->frame = frame;
}

static inline void SetSlotPos(HudPart *slot, const struct vec2 *pos)
{
    SetEntityPixelPos(slot, pos->x, pos->y);
}

/* InitHud. Builds `parts` (`new HudPart[35]`), each given the
 * shared HUD animation table, its frame (slot 22 shows the current mode's
 * life icon) and its position. Slot 13 gets the second table plus its
 * tile record's palette, slots 16/19/21 their starting frames, and
 * ConfigureParts does the rest. */
Hud::Hud()
{
    s32 i;

    parts = new HudPart[35];
    livesSlide = 0;
    crateSlide = 0;
    wumpaSlide = 0;
    livesSlideTimer = 0;
    crateSlideTimer = 0;
    wumpaSlideTimer = 0;

    for (i = 0; i <= 0x22; i++) {
        HudPart *slot;

        parts[i].SetPriority(0);
        {
            const struct sprite_bank *anim = HUD_ANIM(0x234);

            slot = (HudPart *)(i * sizeof(HudPart) + (u32)parts);
            SET_PART_BANK(slot, anim);
        }
        if (i == 0x16) {
            s32 life = gLevelState->GetBossIndex();

            slot = &parts[i];
            parts[0x16].tag = life + BOSS_HUD_ANIM_BASE;
            RestartSlot(slot);
        } else {
            slot->tag = gHudPartAnims[i];
            RestartSlot(slot);
        }
        SetSlotPos(&parts[i], &gHudPartPositions[i]);
    }

    {
        const struct sprite_bank *anim;
        HudPart *slot;

        i = 13;
        anim = HUD_ANIM(0x1A4);
        slot = &parts[i];
        SET_PART_BANK(slot, anim);
        parts[13].tag = gHudPartAnims[13];
        RestartSlot(slot);
    }
    {
        const struct sprite_anim *records = parts[13].bank->anims;
        const struct sprite_anim *rec = &records[parts[13].tag];
        s32 palette = gPaletteCache->GetSlot(rec->paletteId);

        parts[13].palette = palette;
    }

    SetSlotFrame(&parts[16], 10);
    SetSlotFrame(&parts[19], 11);
    SetSlotFrame(&parts[21], 0);
    ConfigureParts(0);
}

/* ConfigureHudParts, the constructor's own tail: stores `iconFlag` into `icon_flag`,
 * finishes the two slots `InitHud` set up part of already (a
 * position/frame-index pair from a shared table, then the same
 * `field_29`-low-nibble update the pause menu's icons do
 * (Sprite::palette, pause_menu_pages_init.cpp) for their UI sprites -
 * `GetSpriteAnimPaletteSlot`'s result feeds the
 * same low-nibble-preserving update here too), then loops over the
 * remaining slots (index 0-34 again) repositioning/re-clamping a
 * handful of specific ones (13, 22, 29 - byte offsets `0x340`/`0x580`/
 * `0x740` off `parts`) depending on the current level/game-mode
 * (`GetBossIndex`) and `icon_flag`, before DMA-filling nine words
 * at `this+0x40` (`shownLives`...) with `-1` (a raw `REG_DMA3SAD`/`DAD`/`CNT` poke, the
 * same low-level idiom `save_data.cpp`'s `ValidateSaveData` and
 * `link_handshake.cpp` already document for this ROM).
 *
 * Matching notes (old_agbcp, as the C under old_agbcc): the two inner palette stores go through
 * the `SetPal` inline so the ROM's three separate nibble-insert copies
 * survive cross-jumping; the 22/23/29 and 14..21 tests are `switch`es
 * (compare-tree order); slot 13 is indexed through a local `k`, and the
 * position table through a `tbl` local, to settle register ties. */
static inline void SetPal(HudPart *slot, s32 v)
{
    slot->palette = v;
}

void Hud::ConfigureParts(u8 iconFlag)
{
    s32 base;
    s32 i;

    icon_flag = iconFlag;
    {
        const struct sprite_bank *anim;
        HudPart *slot;
        s32 k = 13;

        anim = HUD_ANIM(0x1A4);
        slot = &parts[k];

        SET_PART_BANK(slot, anim);
        parts[13].tag = gHudPartAnims[13];
        RestartSlot(slot);
    }
    base = parts[13].GetAnimPaletteSlot();
    parts[13].palette = base;

    for (i = 0; i <= 0x22; i++) {
        s32 frame;

        if (i == 0x16) {
            s32 life = gLevelState->GetBossIndex();
            HudPart *slot = &parts[i];

            parts[0x16].tag = life + BOSS_HUD_ANIM_BASE;
            RestartSlot(slot);
        }
        frame = 0;
        switch (i) {
        case 0x16:
        case 0x17:
            if (gLevelState->GetBossIndex() == BOSS_NONE)
                break;
            goto get;
        case 0x1D:
            if (!icon_flag)
                break;
            frame = parts[i].GetAnimPaletteSlot();
            break;
        default:
        get:
            frame = parts[i].GetAnimPaletteSlot();
            break;
        }

        if (gLevelState->GetBossIndex() == BOSS_NONE && icon_flag) {
            switch (i) {
            case 0xE ... 0x15:
                {
                    const struct vec2 *tbl = gHudPartPositions;
                    const struct vec2 *pos = tbl + i;

                    parts[i].x = INT_TO_Q8(pos->x);
                    parts[i].y = 0x1400;
                    SetPal(&parts[i], frame);
                    break;
                }
            default:
                if (frame == base)
                    SetPal(&parts[i], 10);
                else
                    SetPal(&parts[i], frame);
                break;
            }
        } else
            parts[i].palette = frame;
    }
    DmaFill32(3, -1, &shownLives, 9 * 4);
}
