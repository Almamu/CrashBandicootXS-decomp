#include "core.h"
#include "hud.h"
#include "gfx.h"
#include "objects.h"
#include "memory.h"
#include "level.h"
#include "globals.h"

/* Built with old_agbcc - see docs/matching/archive/game-loop-old-agbcc.md. */

#define HUD_ANIM(offset) ((struct hud_anim_data *)(SPRITE_BANK_BASE + (offset)))
#define SLOT_RECORD(s) ((s)->anim_data->records[(s)->anim_index])

static inline void RestartSlot(struct hud_digit_part *slot)
{
    ResetSpriteFrameTimer(slot);
    ResetSpriteFrameIndex(slot);
    SetSpriteAnimDone(slot, 0);
}

/* Shows animation frame `frame`, clamped to the animation's last one. */
static inline void SetSlotFrame(struct hud_digit_part *slot, s32 frame)
{
    s32 n = SLOT_RECORD(slot).frame_count;
    if (frame >= n)
        frame = n - 1;
    slot->frame_index = frame;
}

static inline void SetSlotPos(struct hud_digit_part *slot, const struct hud_pos *pos)
{
    SetEntityPixelPos((struct actor *)slot, pos->x, pos->y);
}

/* Builds self->parts: a counted array of 35 HUD slots, each given the
 * shared HUD animation table, its frame (slot 22 shows the current mode's
 * life icon) and its position. Slot 13 gets the second table plus its
 * tile record's palette, slots 16/19/21 their starting frames, and
 * ConfigureHudParts does the rest. */
struct hud_counter *InitHud(struct hud_counter *self)
{
    s32 i;

    {
        s32 *mem = OperatorNewArray(0x8C4);
        struct hud_digit_part *slots;
        struct hud_digit_part *slot;
        s32 n;

        *mem++ = 0x23;
        slots = (struct hud_digit_part *)mem;
        for (slot = slots, n = 0x22; n != -1; slot++, n--)
            InitHudPart(slot);
        self->parts = slots;
    }
    self->livesSlide = 0;
    self->crateSlide = 0;
    self->wumpaSlide = 0;
    self->livesSlideTimer = 0;
    self->crateSlideTimer = 0;
    self->wumpaSlideTimer = 0;

    for (i = 0; i <= 0x22; i++)
    {
        struct hud_digit_part *slot;

        SetSpritePriority(&self->parts[i], 0);
        {
            struct hud_anim_data *anim = HUD_ANIM(0x234);

            slot = (struct hud_digit_part *)(i * sizeof(struct hud_digit_part) + (u32)self->parts);
            slot->anim_data = anim;
        }
        if (i == 0x16)
        {
            s32 life = GetBossIndex(gLevelState);

            slot = &self->parts[i];
            self->parts[0x16].anim_index = life + 6;
            RestartSlot(slot);
        }
        else
        {
            slot->anim_index = gHudPartAnims[i];
            RestartSlot(slot);
        }
        SetSlotPos(&self->parts[i], &gHudPartPositions[i]);
    }

    {
        struct hud_anim_data *anim;
        struct hud_digit_part *slot;

        i = 13;
        anim = HUD_ANIM(0x1A4);
        slot = &self->parts[i];
        slot->anim_data = anim;
        self->parts[13].anim_index = gHudPartAnims[13];
        RestartSlot(slot);
    }
    {
        struct hud_anim_record *records = self->parts[13].anim_data->records;
        struct hud_anim_record *rec = &records[self->parts[13].anim_index];
        s32 palette = GetPaletteSlot((struct palette_cache *)gPaletteCache, rec->tile_record);

        self->parts[13].palette = palette;
    }

    SetSlotFrame(&self->parts[16], 10);
    SetSlotFrame(&self->parts[19], 11);
    SetSlotFrame(&self->parts[21], 0);
    ConfigureHudParts(self, 0);
    return self;
}

/* `InitHud`'s own tail: stores `iconFlag` into `self->icon_flag`,
 * finishes the two slots `InitHud` set up part of already (a
 * position/frame-index pair from a shared table, then the same
 * `field_29`-low-nibble update `pause_menu_pages_init.c`'s
 * `UPDATE_ICON_FRAME_NIBBLE` macro names for the unrelated
 * `struct settings_icon_actor` family - `GetSpriteAnimPaletteSlot`'s result feeds the
 * same low-nibble-preserving update here too), then loops over the
 * remaining slots (index 0-34 again) repositioning/re-clamping a
 * handful of specific ones (13, 22, 29 - byte offsets `0x340`/`0x580`/
 * `0x740` off `self->parts`) depending on the current level/game-mode
 * (`GetBossIndex`) and `self->icon_flag`, before DMA-filling nine words
 * at `self+0x40` with `-1` (a raw `REG_DMA3SAD`/`DAD`/`CNT` poke, the
 * same low-level idiom `save_data.c`'s `ValidateSaveData` and
 * `link_handshake.c` already document for this ROM).
 *
 * Matching notes (old_agbcc): the two inner palette stores go through
 * the `SetPal` inline so the ROM's three separate nibble-insert copies
 * survive cross-jumping; the 22/23/29 and 14..21 tests are `switch`es
 * (compare-tree order); slot 13 is indexed through a local `k`, and the
 * position table through a `tbl` local, to settle register ties. */
static inline void SetPal(struct hud_digit_part *slot, s32 v)
{
    slot->palette = v;
}

void ConfigureHudParts(struct hud_counter *self, u8 iconFlag)
{
    s32 base;
    s32 i;

    self->icon_flag = iconFlag;
    {
        struct hud_anim_data *anim;
        struct hud_digit_part *slot;
        s32 k = 13;

        anim = HUD_ANIM(0x1A4);
        slot = &self->parts[k];

        slot->anim_data = anim;
        self->parts[13].anim_index = gHudPartAnims[13];
        RestartSlot(slot);
    }
    base = GetSpriteAnimPaletteSlot((struct actor *)&self->parts[13]);
    self->parts[13].palette = base;

    for (i = 0; i <= 0x22; i++)
    {
        s32 frame;

        if (i == 0x16)
        {
            s32 life = GetBossIndex(gLevelState);
            struct hud_digit_part *slot = &self->parts[i];

            self->parts[0x16].anim_index = life + 6;
            RestartSlot(slot);
        }
        frame = 0;
        switch (i)
        {
        case 0x16:
        case 0x17:
            if (GetBossIndex(gLevelState) == -1)
                break;
            goto get;
        case 0x1D:
            if (!self->icon_flag)
                break;
            frame = GetSpriteAnimPaletteSlot((struct actor *)&self->parts[i]);
            break;
        default:
        get:
            frame = GetSpriteAnimPaletteSlot((struct actor *)&self->parts[i]);
            break;
        }

        if (GetBossIndex(gLevelState) == -1 && self->icon_flag)
        {
            switch (i)
            {
            case 0xE ... 0x15:
            {
                const struct hud_pos *tbl = gHudPartPositions;
                const struct hud_pos *pos = tbl + i;

                self->parts[i].x = pos->x << 8;
                self->parts[i].y = 0x1400;
                SetPal(&self->parts[i], frame);
                break;
            }
            default:
                if (frame == base)
                    SetPal(&self->parts[i], 10);
                else
                    SetPal(&self->parts[i], frame);
                break;
            }
        }
        else
            self->parts[i].palette = frame;
    }
    DmaFill32(3, -1, &self->shownLives, 9 * 4);
}
