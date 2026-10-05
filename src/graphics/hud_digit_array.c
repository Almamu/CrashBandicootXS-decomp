#include "core.h"
#include "hud.h"

/* Built with old_agbcc - see docs/matching/game-loop-old-agbcc.md. */

/* Local views of the slot and record fields hud.h doesn't name yet. */
struct hud_slot
{
    s32 x;                            // 0x00
    s32 y;                            // 0x04
    u8 unk_08[0x18];
    struct hud_anim_data *anim_data;  // 0x20
    u8 unk_24[5];
    u8 palette:4;                     // 0x29
    u8 unk_29_4:4;
    u8 unk_2A[3];
    u8 anim_index;                    // 0x2D
    u8 unk_2E[2];
    s32 frame_index;                  // 0x30
    u8 unk_34[0xC];
};

struct hud_record
{
    u8 unk_00[0x14];
    u8 tile_record;                   // 0x14
    u8 unk_15;
    u8 frame_count;                   // 0x16
    u8 unk_17[5];
};

struct hud_pos
{
    s32 x;
    s32 y;
};

extern void ***gUnknown_030012D0;
extern void *gLevelState;
extern u8 *gPaletteCache;
extern u32 gHudPartAnims[];
extern struct hud_pos gHudPartPositions[];

extern void *OperatorNewArray(s32 size);
extern void InitHudPart(struct hud_slot *slot);
extern void SetSpritePriority(struct hud_slot *slot, s32 value);
extern s32 sub_80233B4(void *self);
extern void ResetSpriteFrameTimer(struct hud_slot *slot);
extern void ResetSpriteFrameIndex(struct hud_slot *slot);
extern void SetSpriteAnimDone(struct hud_slot *slot, s32 arg);
extern void sub_800737C(struct hud_slot *slot, s32 x, s32 y);
extern u8 GetPaletteSlot(u8 *cache, s32 recordId);
extern void sub_802732C(struct hud_counter *self, u8 iconFlag);
extern s32 GetSpriteAnimPaletteSlot(struct hud_slot *slot);

#define HUD_ANIM(offset) ((struct hud_anim_data *)((u8 *)**gUnknown_030012D0 + (offset)))
#define SLOT_RECORD(s) (((struct hud_record *)(s)->anim_data->records)[(s)->anim_index])

static inline void RestartSlot(struct hud_slot *slot)
{
    ResetSpriteFrameTimer(slot);
    ResetSpriteFrameIndex(slot);
    SetSpriteAnimDone(slot, 0);
}

/* Shows animation frame `frame`, clamped to the animation's last one. */
static inline void SetSlotFrame(struct hud_slot *slot, s32 frame)
{
    s32 n = SLOT_RECORD(slot).frame_count;
    if (frame >= n)
        frame = n - 1;
    slot->frame_index = frame;
}

#define SLOTS(self) ((struct hud_slot *)(self)->parts)

static inline void SetSlotPos(struct hud_slot *slot, struct hud_pos *pos)
{
    sub_800737C(slot, pos->x, pos->y);
}

/* Builds self->parts: a counted array of 35 HUD slots, each given the
 * shared HUD animation table, its frame (slot 22 shows the current mode's
 * life icon) and its position. Slot 13 gets the second table plus its
 * tile record's palette, slots 16/19/21 their starting frames, and
 * sub_802732C does the rest. */
struct hud_counter *InitHud(struct hud_counter *self)
{
    s32 i;

    {
        s32 *mem = OperatorNewArray(0x8C4);
        struct hud_slot *slots;
        struct hud_slot *slot;
        s32 n;

        *mem++ = 0x23;
        slots = (struct hud_slot *)mem;
        for (slot = slots, n = 0x22; n != -1; slot++, n--)
            InitHudPart(slot);
        self->parts = (struct hud_digit_part *)slots;
    }
    self->livesSlide = 0;
    *(s32 *)&self->unknown_0c[4] = 0;
    self->wumpaSlide = 0;
    self->livesSlideTimer = 0;
    *(s32 *)&self->unknown_0c[8] = 0;
    *(s32 *)&self->unknown_0c[0] = 0;

    for (i = 0; i <= 0x22; i++)
    {
        struct hud_slot *slot;

        SetSpritePriority(&SLOTS(self)[i], 0);
        {
            struct hud_anim_data *anim = HUD_ANIM(0x234);

            slot = (struct hud_slot *)(i * sizeof(struct hud_slot) + (u32)SLOTS(self));
            slot->anim_data = anim;
        }
        if (i == 0x16)
        {
            s32 life = sub_80233B4(gLevelState);

            slot = &SLOTS(self)[i];
            SLOTS(self)[0x16].anim_index = life + 6;
            RestartSlot(slot);
        }
        else
        {
            slot->anim_index = gHudPartAnims[i];
            RestartSlot(slot);
        }
        SetSlotPos(&SLOTS(self)[i], &gHudPartPositions[i]);
    }

    {
        struct hud_anim_data *anim;
        struct hud_slot *slot;

        i = 13;
        anim = HUD_ANIM(0x1A4);
        slot = &SLOTS(self)[i];
        slot->anim_data = anim;
        SLOTS(self)[13].anim_index = gHudPartAnims[13];
        RestartSlot(slot);
    }
    {
        struct hud_record *records = (struct hud_record *)SLOTS(self)[13].anim_data->records;
        struct hud_record *rec = &records[SLOTS(self)[13].anim_index];
        s32 palette = GetPaletteSlot(gPaletteCache, rec->tile_record);

        SLOTS(self)[13].palette = palette;
    }

    SetSlotFrame(&SLOTS(self)[16], 10);
    SetSlotFrame(&SLOTS(self)[19], 11);
    SetSlotFrame(&SLOTS(self)[21], 0);
    sub_802732C(self, 0);
    return self;
}

/* `InitHud`'s own tail: stores `iconFlag` into `self->icon_flag`,
 * finishes the two slots `InitHud` set up part of already (a
 * position/frame-index pair from a shared table, then the same
 * `field_29`-low-nibble update `settings_menu6.c`'s
 * `UPDATE_ICON_FRAME_NIBBLE` macro names for the unrelated
 * `struct settings_icon_actor` family - `GetSpriteAnimPaletteSlot`'s result feeds the
 * same low-nibble-preserving update here too), then loops over the
 * remaining slots (index 0-34 again) repositioning/re-clamping a
 * handful of specific ones (13, 22, 29 - byte offsets `0x340`/`0x580`/
 * `0x740` off `self->parts`) depending on the current level/game-mode
 * (`sub_80233B4`) and `self->icon_flag`, before DMA-filling nine words
 * at `self+0x40` with `-1` (a raw `REG_DMA3SAD`/`DAD`/`CNT` poke, the
 * same low-level idiom `settings_menu8e.c`'s `ValidateSaveData` and
 * `link_cable.c` already document for this ROM).
 *
 * Matching notes (old_agbcc): the two inner palette stores go through
 * the `SetPal` inline so the ROM's three separate nibble-insert copies
 * survive cross-jumping; the 22/23/29 and 14..21 tests are `switch`es
 * (compare-tree order); slot 13 is indexed through a local `k`, and the
 * position table through a `tbl` local, to settle register ties. */
static inline void SetPal(struct hud_slot *slot, s32 v)
{
    slot->palette = v;
}

void sub_802732C(struct hud_counter *self, u8 iconFlag)
{
    s32 base;
    s32 i;

    self->icon_flag = iconFlag;
    {
        struct hud_anim_data *anim;
        struct hud_slot *slot;
        s32 k = 13;

        anim = HUD_ANIM(0x1A4);
        slot = &SLOTS(self)[k];

        slot->anim_data = anim;
        SLOTS(self)[13].anim_index = gHudPartAnims[13];
        RestartSlot(slot);
    }
    base = GetSpriteAnimPaletteSlot(&SLOTS(self)[13]);
    SLOTS(self)[13].palette = base;

    for (i = 0; i <= 0x22; i++)
    {
        s32 frame;

        if (i == 0x16)
        {
            s32 life = sub_80233B4(gLevelState);
            struct hud_slot *slot = &SLOTS(self)[i];

            SLOTS(self)[0x16].anim_index = life + 6;
            RestartSlot(slot);
        }
        frame = 0;
        switch (i)
        {
        case 0x16:
        case 0x17:
            if (sub_80233B4(gLevelState) == -1)
                break;
            goto get;
        case 0x1D:
            if (!self->icon_flag)
                break;
            frame = GetSpriteAnimPaletteSlot(&SLOTS(self)[i]);
            break;
        default:
        get:
            frame = GetSpriteAnimPaletteSlot(&SLOTS(self)[i]);
            break;
        }

        if (sub_80233B4(gLevelState) == -1 && self->icon_flag)
        {
            switch (i)
            {
            case 0xE ... 0x15:
            {
                struct hud_pos *tbl = gHudPartPositions;
                struct hud_pos *pos = tbl + i;

                SLOTS(self)[i].x = pos->x << 8;
                SLOTS(self)[i].y = 0x1400;
                SetPal(&SLOTS(self)[i], frame);
                break;
            }
            default:
                if (frame == base)
                    SetPal(&SLOTS(self)[i], 10);
                else
                    SetPal(&SLOTS(self)[i], frame);
                break;
            }
        }
        else
            SLOTS(self)[i].palette = frame;
    }
    DmaFill32(3, -1, &self->shownLives, 9 * 4);
}
