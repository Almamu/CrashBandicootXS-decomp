#include "core.h"
#include "actor.h"
#include "pause_screen_results.h"

/* Built with old_agbcc (Makefile OLD_AGBCC_OBJS): the four icon-group
 * constructors below only match under it, and InitPauseCrystalsPage compiles
 * identically under both compilers. */

extern void *OperatorNew(s32 size);
extern struct actor *InitUiSpriteObj(struct actor *part);
extern void SetEntityPixelPos(struct actor *self, s32 arg1, s32 arg2);
extern s32 CountCrystals(void *arg0);
extern s32 FormatDecimal(s32 value, void *dest);
extern void ***gSpriteBankSet;
extern struct icon_pos gStaticData_0816B1E4;

/* Constructs the single icon at `field_88`: positions it from the fixed
 * `gStaticData_0816B1E4` pair, picks its starting keyframe-table entry
 * from a shared table (`gSpriteBankSet`'s triple-indirected base,
 * offset `0xde<<1`), and formats two small numbers - a row-stats
 * derived count into `buf2c` and the constant `0x14` into `buf46` -
 * as decimal strings. */
void InitPauseCrystalsPage(struct pause_screen_results *self)
{
    struct settings_icon_actor **dest = &self->field_88;

    *dest = (struct settings_icon_actor *)InitUiSpriteObj((struct actor *)OperatorNew(0x40));
    (*dest)->field_20 = (void **)((u8 *)(**gSpriteBankSet) + (0xde << 1));
    SetEntityPixelPos(&(*dest)->base, gStaticData_0816B1E4.x, gStaticData_0816B1E4.y);
    UPDATE_ICON_FRAME_NIBBLE(*dest);

    FormatDecimal(CountCrystals(self->field_10), self->buf2c);
    FormatDecimal(0x14, (u8 *)self + 0x46);
}

extern void ResetSpriteFrameTimer(struct actor *part);
extern void ResetSpriteFrameIndex(struct actor *part);
extern void SetSpriteAnimDone(struct actor *part, u8 val);

/* `field_29` viewed as the nibble pair it is: the low nibble is the
 * GetSpriteAnimPaletteSlot-derived frame bits (same byte as level_menu.h's
 * `struct sprite` `palette:4`). Assigning the bitfield gives the ROM's
 * `and #0xf / mov #0x10; neg / and / orr` sequence with no pins. */
struct icon_frame_nibble {
    u8 lo:4;
    u8 hi:4;
};

#define SET_ICON_FRAME_NIBBLE(iconExpr) \
    (((struct icon_frame_nibble *)&(iconExpr)->field_29)->lo = GetSpriteAnimPaletteSlot(&(iconExpr)->base))

/* Allocates and constructs a fresh 0x40-byte icon into `icon`, yielding
 * it. Used as the right-hand side of the `self->iconsXX[i] = ...`
 * stores below: because that right-hand side is a comma expression
 * rather than a bare call, gcc computes the array slot's address first
 * (before the two calls), which is also what keeps the loop from being
 * strength-reduced - both exactly as the ROM has it. */
#define NEW_ICON(icon) \
    ((icon) = (struct settings_icon_actor *)InitUiSpriteObj((struct actor *)OperatorNew(0x40)), (icon))

static inline void set_icon_pos(struct actor *a, struct icon_pos *p)
{
    SetEntityPixelPos(a, p->x, p->y);
}

extern u32 gStaticData_0816B20C[];
extern struct icon_pos gStaticData_0816B1EC[];

/* Builds the 4-icon array at `icons8c`: one per `gStaticData_0816B1EC`
 * position entry, keyframe-table base `0xe4<<1` off the same shared
 * table `InitPauseCrystalsPage` uses, frame index from `gStaticData_0816B20C`,
 * then the standard sub-counter/frame-counter/"done"-flag reset trio. */
void InitPausePowersPage(struct pause_screen_results *self)
{
    s32 i;

    for (i = 0; i <= 3; i++) {
        struct settings_icon_actor *icon;

        self->icons8c[i] = NEW_ICON(icon);
        icon->field_20 = (void **)((u8 *)(**gSpriteBankSet) + (0xe4 << 1));
        icon->frameIndex = gStaticData_0816B20C[i];
        ResetSpriteFrameTimer(&icon->base);
        ResetSpriteFrameIndex(&icon->base);
        SetSpriteAnimDone(&icon->base, 0);
        set_icon_pos(&self->icons8c[i]->base, &gStaticData_0816B1EC[i]);
        SET_ICON_FRAME_NIBBLE(self->icons8c[i]);
    }
}

extern s32 CountClearGems(void *arg0);
extern s32 CountGems(void *arg0);
extern u32 gStaticData_0816B244[];
extern struct icon_pos gStaticData_0816B21C[];

/* Same shape as InitPausePowersPage above for the 5-icon array at `icons9c`
 * (keyframe-table base `0xc0<<1`, positions/frame indices from
 * `gStaticData_0816B21C`/`gStaticData_0816B244`), plus each icon's
 * `field_3c = 0x80`. After the loop, formats two more row-stats
 * derived numbers (`CountClearGems`/`CountGems` on `field_10`) into
 * `buf2f`/`buf32`, and the constant `0x1c` into `buf49`. */
void InitPauseGemsPage(struct pause_screen_results *self)
{
    s32 i;
    s32 a, b;

    for (i = 0; i <= 4; i++) {
        struct settings_icon_actor *icon;

        self->icons9c[i] = NEW_ICON(icon);
        icon->field_20 = (void **)((u8 *)(**gSpriteBankSet) + (0xc0 << 1));
        icon->frameIndex = gStaticData_0816B244[i];
        ResetSpriteFrameTimer(&icon->base);
        ResetSpriteFrameIndex(&icon->base);
        SetSpriteAnimDone(&icon->base, 0);
        set_icon_pos(&self->icons9c[i]->base, &gStaticData_0816B21C[i]);
        SET_ICON_FRAME_NIBBLE(self->icons9c[i]);
        self->icons9c[i]->field_3c = 0x80;
    }

    a = CountClearGems(self->field_10);
    b = CountGems(self->field_10);
    FormatDecimal(a, self->buf2f);
    FormatDecimal(b, self->buf32);
    FormatDecimal(0x1c, self->buf49);
}

extern s32 CountSapphireRelics(void *arg0);
extern s32 CountGoldRelics(void *arg0);
extern s32 CountPlatinumRelics(void *arg0);
extern s32 CountRelics(void *arg0);
extern u32 gStaticData_0816B270[];
extern struct icon_pos gStaticData_0816B258[];

/* Same shape as InitPausePowersPage/InitPauseGemsPage above for the 3-icon array at
 * `iconsB0` (keyframe-table base `0xc6<<1`, positions/frame indices
 * from `gStaticData_0816B258`/`gStaticData_0816B270`), each icon's
 * `field_3c = 0x80`. After the loop, formats four category counts
 * (`CountSapphireRelics`/`CountGoldRelics`/`CountPlatinumRelics`/`CountRelics` on
 * `field_10` - the same four functions src/graphics/oam_count.c
 * documents) into `buf38`/`buf3b`/`buf3e`/`buf35`, and the constant
 * `0x14` into `buf4c`. */
void InitPauseRelicsPage(struct pause_screen_results *self)
{
    s32 i;

    for (i = 0; i <= 2; i++) {
        struct settings_icon_actor *icon;

        self->iconsB0[i] = NEW_ICON(icon);
        icon->field_20 = (void **)((u8 *)(**gSpriteBankSet) + (0xc6 << 1));
        icon->frameIndex = gStaticData_0816B270[i];
        ResetSpriteFrameTimer(&icon->base);
        ResetSpriteFrameIndex(&icon->base);
        SetSpriteAnimDone(&icon->base, 0);
        set_icon_pos(&self->iconsB0[i]->base, &gStaticData_0816B258[i]);
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
extern s32 GetCurrentLevel(void *arg0);
extern void FormatCentiseconds(s32 value, u8 *buf);

/* Same per-level bronze/silver/gold threshold table src/graphics/oam_count.c's
 * `struct threshold_table_entry`/`gLevelTable` already document -
 * duplicated locally (rather than shared via a header) per that file's
 * own comment on the type, matching this project's minimal-local-type
 * convention. */
struct threshold_table_entry {
    u8 unused_00[8];
    u32 threshold_08;
    u32 threshold_0C;
    u32 threshold_10;
    u8 unused_14[0x24 - 0x14];
};
COMPILE_TIME_ASSERT(sizeof(struct threshold_table_entry) == 0x24);

extern struct threshold_table_entry gLevelTable[];
extern struct icon_pos gStaticData_0816B27C;

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
 * medal (from `gStaticData_0816B270`, the same table InitPauseRelicsPage uses)
 * whose threshold was met. `field_6c` is an "earned" flag. */
void InitPauseTimeTrialPage(struct pause_screen_results *self)
{
    s32 levelIdx;
    u32 time;
    struct threshold_table_entry *entry;
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
    if (time != 0 && time <= entry->threshold_08)
        earned = 1;
    self->field_6c = earned;

    slot = &self->field_bc;
    *slot = (struct settings_icon_actor *)InitUiSpriteObj((struct actor *)OperatorNew(0x40));
    (*slot)->field_20 = (void **)((u8 *)(**gSpriteBankSet) + (0xc6 << 1));
    set_icon_pos(&(*slot)->base, &gStaticData_0816B27C);

    if (time != 0) {
        if (time <= entry->threshold_08)
            set_icon_frame(*slot, gStaticData_0816B270[2]);
        if (time <= entry->threshold_0C)
            set_icon_frame(*slot, gStaticData_0816B270[1]);
        if (time <= entry->threshold_10)
            set_icon_frame(*slot, gStaticData_0816B270[0]);
        SET_ICON_FRAME_NIBBLE(*slot);
    }
}
