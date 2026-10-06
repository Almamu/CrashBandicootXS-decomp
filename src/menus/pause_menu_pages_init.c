#include "core.h"
#include "actor.h"
#include "pause_menu.h"
#include "util.h"
#include "menus.h"
#include "gfx.h"
#include "objects.h"
#include "memory.h"
#include "level.h"

/* Built with old_agbcc (Makefile OLD_AGBCC_OBJS): the four icon-group
 * constructors below only match under it, and InitPauseCrystalsPage compiles
 * identically under both compilers. */

extern void ***gSpriteBankSet;

/* Constructs the single icon at `field_88`: positions it from the fixed
 * `gPauseCrystalIconPos` pair, picks its starting keyframe-table entry
 * from a shared table (`gSpriteBankSet`'s triple-indirected base,
 * offset `0xde<<1`), and formats two small numbers - a row-stats
 * derived count into `buf2c` and the constant `0x14` into `buf46` -
 * as decimal strings. */
void InitPauseCrystalsPage(struct pause_menu *self)
{
    struct settings_icon_actor **dest = &self->field_88;

    *dest = (struct settings_icon_actor *)InitUiSpriteObj((struct actor *)OperatorNew(0x40));
    (*dest)->field_20 = (void **)((u8 *)(**gSpriteBankSet) + (0xde << 1));
    SetEntityPixelPos(&(*dest)->base, gPauseCrystalIconPos.x, gPauseCrystalIconPos.y);
    UPDATE_ICON_FRAME_NIBBLE(*dest);

    FormatDecimal(CountCrystals(self->field_10), self->buf2c);
    FormatDecimal(0x14, (u8 *)self + 0x46);
}

/* Allocates and constructs a fresh 0x40-byte icon into `icon`, yielding
 * it. Used as the right-hand side of the `self->iconsXX[i] = ...`
 * stores below: because that right-hand side is a comma expression
 * rather than a bare call, gcc computes the array slot's address first
 * (before the two calls), which is also what keeps the loop from being
 * strength-reduced - both exactly as the ROM has it. */
#define NEW_ICON(icon) \
    ((icon) = (struct settings_icon_actor *)InitUiSpriteObj((struct actor *)OperatorNew(0x40)), (icon))

static inline void set_icon_pos(struct actor *a, const struct icon_pos *p)
{
    SetEntityPixelPos(a, p->x, p->y);
}

/* Builds the 4-icon array at `icons8c`: one per `gPausePowerIconPos`
 * position entry, keyframe-table base `0xe4<<1` off the same shared
 * table `InitPauseCrystalsPage` uses, frame index from `gPausePowerIconFrames`,
 * then the standard sub-counter/frame-counter/"done"-flag reset trio. */
void InitPausePowersPage(struct pause_menu *self)
{
    s32 i;

    for (i = 0; i <= 3; i++) {
        struct settings_icon_actor *icon;

        self->icons8c[i] = NEW_ICON(icon);
        icon->field_20 = (void **)((u8 *)(**gSpriteBankSet) + (0xe4 << 1));
        icon->frameIndex = gPausePowerIconFrames[i];
        ResetSpriteFrameTimer(&icon->base);
        ResetSpriteFrameIndex(&icon->base);
        SetSpriteAnimDone(&icon->base, 0);
        set_icon_pos(&self->icons8c[i]->base, &gPausePowerIconPos[i]);
        SET_ICON_FRAME_NIBBLE(self->icons8c[i]);
    }
}

/* Same shape as InitPausePowersPage above for the 5-icon array at `icons9c`
 * (keyframe-table base `0xc0<<1`, positions/frame indices from
 * `gPauseGemIconPos`/`gPauseGemIconFrames`), plus each icon's
 * `field_3c = 0x80`. After the loop, formats two more row-stats
 * derived numbers (`CountClearGems`/`CountGems` on `field_10`) into
 * `buf2f`/`buf32`, and the constant `0x1c` into `buf49`. */
void InitPauseGemsPage(struct pause_menu *self)
{
    s32 i;
    s32 a, b;

    for (i = 0; i <= 4; i++) {
        struct settings_icon_actor *icon;

        self->icons9c[i] = NEW_ICON(icon);
        icon->field_20 = (void **)((u8 *)(**gSpriteBankSet) + (0xc0 << 1));
        icon->frameIndex = gPauseGemIconFrames[i];
        ResetSpriteFrameTimer(&icon->base);
        ResetSpriteFrameIndex(&icon->base);
        SetSpriteAnimDone(&icon->base, 0);
        set_icon_pos(&self->icons9c[i]->base, &gPauseGemIconPos[i]);
        SET_ICON_FRAME_NIBBLE(self->icons9c[i]);
        self->icons9c[i]->field_3c = 0x80;
    }

    a = CountClearGems(self->field_10);
    b = CountGems(self->field_10);
    FormatDecimal(a, self->buf2f);
    FormatDecimal(b, self->buf32);
    FormatDecimal(0x1c, self->buf49);
}

/* Same shape as InitPausePowersPage/InitPauseGemsPage above for the 3-icon array at
 * `iconsB0` (keyframe-table base `0xc6<<1`, positions/frame indices
 * from `gPauseRelicIconPos`/`gPauseRelicIconFrames`), each icon's
 * `field_3c = 0x80`. After the loop, formats four category counts
 * (`CountSapphireRelics`/`CountGoldRelics`/`CountPlatinumRelics`/`CountRelics` on
 * `field_10` - the same four functions src/menus/power_dialog_draw.c
 * documents) into `buf38`/`buf3b`/`buf3e`/`buf35`, and the constant
 * `0x14` into `buf4c`. */
void InitPauseRelicsPage(struct pause_menu *self)
{
    s32 i;

    for (i = 0; i <= 2; i++) {
        struct settings_icon_actor *icon;

        self->iconsB0[i] = NEW_ICON(icon);
        icon->field_20 = (void **)((u8 *)(**gSpriteBankSet) + (0xc6 << 1));
        icon->frameIndex = gPauseRelicIconFrames[i];
        ResetSpriteFrameTimer(&icon->base);
        ResetSpriteFrameIndex(&icon->base);
        SetSpriteAnimDone(&icon->base, 0);
        set_icon_pos(&self->iconsB0[i]->base, &gPauseRelicIconPos[i]);
        SET_ICON_FRAME_NIBBLE(self->iconsB0[i]);
        self->iconsB0[i]->field_3c = 0x80;
    }

    FormatDecimal(CountSapphireRelics(self->field_10), self->buf38);
    FormatDecimal(CountGoldRelics(self->field_10), self->buf3b);
    FormatDecimal(CountPlatinumRelics(self->field_10), self->buf3e);
    FormatDecimal(CountRelics(self->field_10), self->buf35);
    FormatDecimal(0x14, self->buf4c);
}

extern void *gLevelState;

/* Tags `icon` with medal frame `frame` and restarts its animation. The
 * frame is a word parameter (not u8) so the table word is loaded after
 * the icon pointer, as in the ROM. */
static inline void set_icon_frame(struct settings_icon_actor *icon, u32 frame)
{
    icon->frameIndex = frame;
    ResetSpriteFrameTimer(&icon->base);
    ResetSpriteFrameIndex(&icon->base);
    SetSpriteAnimDone(&icon->base, 0);
}

/* The medal/rank award widget (see docs/rom_map.md's overlay_ui
 * "Correction" section): reads the current level's completion time
 * (bits 3-15 of the save block's per-level record word at
 * `field_10 + 4 + levelIdx * 4` - level_menu.h's `struct level_save`
 * `time:13`), formats it via FormatCentiseconds, then compares it
 * against `gLevelTable[levelIdx]`'s bronze/silver/gold
 * thresholds and constructs the icon at `field_bc`, tagged with each
 * medal (from `gPauseRelicIconFrames`, the same table InitPauseRelicsPage uses)
 * whose threshold was met. `field_6c` is an "earned" flag. */
void InitPauseTimeTrialPage(struct pause_menu *self)
{
    s32 levelIdx;
    u32 time;
    const struct level_info *entry;
    struct settings_icon_actor **slot;
    u8 earned;

    levelIdx = GetCurrentLevel(gLevelState);
    {
        /* A byte offset, not an index: keeps the ROM's `idx*4 + 4`
         * computed before the base is loaded. */
        s32 off = levelIdx * 4 + 4;

        time = (u16)*(u32 *)((u8 *)self->field_10 + off) >> 3;
    }
    FormatCentiseconds(time, self->timeBuf);
    entry = &gLevelTable[levelIdx];
    earned = 0;
    if (time != 0 && time <= entry->times[0])
        earned = 1;
    self->field_6c = earned;

    slot = &self->field_bc;
    *slot = (struct settings_icon_actor *)InitUiSpriteObj((struct actor *)OperatorNew(0x40));
    (*slot)->field_20 = (void **)((u8 *)(**gSpriteBankSet) + (0xc6 << 1));
    set_icon_pos(&(*slot)->base, &gPauseTimeTrialIconPos);

    if (time != 0) {
        if (time <= entry->times[0])
            set_icon_frame(*slot, gPauseRelicIconFrames[2]);
        if (time <= entry->times[1])
            set_icon_frame(*slot, gPauseRelicIconFrames[1]);
        if (time <= entry->times[2])
            set_icon_frame(*slot, gPauseRelicIconFrames[0]);
        SET_ICON_FRAME_NIBBLE(*slot);
    }
}
