#include "core.h"
#include "actor.h"
#include "pause_screen_results.h"

/* Built with old_agbcc (Makefile OLD_AGBCC_OBJS): the four icon-group
 * constructors below only match under it, and sub_8005A78 compiles
 * identically under both compilers. */

extern void *sub_8026EDC(s32 size);
extern struct actor *sub_8008904(struct actor *part);
extern void sub_800737C(struct actor *self, s32 arg1, s32 arg2);
extern s32 sub_800695C(void *arg0);
extern s32 sub_80060AC(s32 value, void *dest);
extern void ***gUnknown_030012D0;
extern struct icon_pos gStaticData_0816B1E4;

/* Constructs the single icon at `field_88`: positions it from the fixed
 * `gStaticData_0816B1E4` pair, picks its starting keyframe-table entry
 * from a shared table (`gUnknown_030012D0`'s triple-indirected base,
 * offset `0xde<<1`), and formats two small numbers - a row-stats
 * derived count into `buf2c` and the constant `0x14` into `buf46` -
 * as decimal strings. */
void sub_8005A78(struct pause_screen_results *self)
{
    struct settings_icon_actor **dest = &self->field_88;

    *dest = (struct settings_icon_actor *)sub_8008904((struct actor *)sub_8026EDC(0x40));
    (*dest)->field_20 = (void **)((u8 *)(**gUnknown_030012D0) + (0xde << 1));
    sub_800737C(&(*dest)->base, gStaticData_0816B1E4.x, gStaticData_0816B1E4.y);
    UPDATE_ICON_FRAME_NIBBLE(*dest);

    sub_80060AC(sub_800695C(self->field_10), self->buf2c);
    sub_80060AC(0x14, (u8 *)self + 0x46);
}

extern void sub_80087C0(struct actor *part);
extern void sub_80087B4(struct actor *part);
extern void sub_800872C(struct actor *part, u8 val);

/* `field_29` viewed as the nibble pair it is: the low nibble is the
 * sub_800815C-derived frame bits (same byte as level_menu.h's
 * `struct sprite` `palette:4`). Assigning the bitfield gives the ROM's
 * `and #0xf / mov #0x10; neg / and / orr` sequence with no pins. */
struct icon_frame_nibble {
    u8 lo:4;
    u8 hi:4;
};

#define SET_ICON_FRAME_NIBBLE(iconExpr) \
    (((struct icon_frame_nibble *)&(iconExpr)->field_29)->lo = sub_800815C(&(iconExpr)->base))

/* Allocates and constructs a fresh 0x40-byte icon into `icon`, yielding
 * it. Used as the right-hand side of the `self->iconsXX[i] = ...`
 * stores below: because that right-hand side is a comma expression
 * rather than a bare call, gcc computes the array slot's address first
 * (before the two calls), which is also what keeps the loop from being
 * strength-reduced - both exactly as the ROM has it. */
#define NEW_ICON(icon) \
    ((icon) = (struct settings_icon_actor *)sub_8008904((struct actor *)sub_8026EDC(0x40)), (icon))

static inline void set_icon_pos(struct actor *a, struct icon_pos *p)
{
    sub_800737C(a, p->x, p->y);
}

extern u32 gStaticData_0816B20C[];
extern struct icon_pos gStaticData_0816B1EC[];

/* Builds the 4-icon array at `icons8c`: one per `gStaticData_0816B1EC`
 * position entry, keyframe-table base `0xe4<<1` off the same shared
 * table `sub_8005A78` uses, frame index from `gStaticData_0816B20C`,
 * then the standard sub-counter/frame-counter/"done"-flag reset trio. */
void sub_8005AE8(struct pause_screen_results *self)
{
    s32 i;

    for (i = 0; i <= 3; i++) {
        struct settings_icon_actor *icon;

        self->icons8c[i] = NEW_ICON(icon);
        icon->field_20 = (void **)((u8 *)(**gUnknown_030012D0) + (0xe4 << 1));
        icon->frameIndex = gStaticData_0816B20C[i];
        sub_80087C0(&icon->base);
        sub_80087B4(&icon->base);
        sub_800872C(&icon->base, 0);
        set_icon_pos(&self->icons8c[i]->base, &gStaticData_0816B1EC[i]);
        SET_ICON_FRAME_NIBBLE(self->icons8c[i]);
    }
}

extern s32 sub_8006920(void *arg0);
extern s32 sub_80068CC(void *arg0);
extern u32 gStaticData_0816B244[];
extern struct icon_pos gStaticData_0816B21C[];

/* Same shape as sub_8005AE8 above for the 5-icon array at `icons9c`
 * (keyframe-table base `0xc0<<1`, positions/frame indices from
 * `gStaticData_0816B21C`/`gStaticData_0816B244`), plus each icon's
 * `field_3c = 0x80`. After the loop, formats two more row-stats
 * derived numbers (`sub_8006920`/`sub_80068CC` on `field_10`) into
 * `buf2f`/`buf32`, and the constant `0x1c` into `buf49`. */
void sub_8005B80(struct pause_screen_results *self)
{
    s32 i;
    s32 a, b;

    for (i = 0; i <= 4; i++) {
        struct settings_icon_actor *icon;

        self->icons9c[i] = NEW_ICON(icon);
        icon->field_20 = (void **)((u8 *)(**gUnknown_030012D0) + (0xc0 << 1));
        icon->frameIndex = gStaticData_0816B244[i];
        sub_80087C0(&icon->base);
        sub_80087B4(&icon->base);
        sub_800872C(&icon->base, 0);
        set_icon_pos(&self->icons9c[i]->base, &gStaticData_0816B21C[i]);
        SET_ICON_FRAME_NIBBLE(self->icons9c[i]);
        self->icons9c[i]->field_3c = 0x80;
    }

    a = sub_8006920(self->field_10);
    b = sub_80068CC(self->field_10);
    sub_80060AC(a, self->buf2f);
    sub_80060AC(b, self->buf32);
    sub_80060AC(0x1c, self->buf49);
}

extern s32 sub_8006864(void *arg0);
extern s32 sub_8006820(void *arg0);
extern s32 sub_80067EC(void *arg0);
extern s32 sub_80068A8(void *arg0);
extern u32 gStaticData_0816B270[];
extern struct icon_pos gStaticData_0816B258[];

/* Same shape as sub_8005AE8/B80 above for the 3-icon array at
 * `iconsB0` (keyframe-table base `0xc6<<1`, positions/frame indices
 * from `gStaticData_0816B258`/`gStaticData_0816B270`), each icon's
 * `field_3c = 0x80`. After the loop, formats four category counts
 * (`sub_8006864`/`sub_8006820`/`sub_80067EC`/`sub_80068A8` on
 * `field_10` - the same four functions src/graphics/oam_count.c
 * documents) into `buf38`/`buf3b`/`buf3e`/`buf35`, and the constant
 * `0x14` into `buf4c`. */
void sub_8005C58(struct pause_screen_results *self)
{
    s32 i;

    for (i = 0; i <= 2; i++) {
        struct settings_icon_actor *icon;

        self->iconsB0[i] = NEW_ICON(icon);
        icon->field_20 = (void **)((u8 *)(**gUnknown_030012D0) + (0xc6 << 1));
        icon->frameIndex = gStaticData_0816B270[i];
        sub_80087C0(&icon->base);
        sub_80087B4(&icon->base);
        sub_800872C(&icon->base, 0);
        set_icon_pos(&self->iconsB0[i]->base, &gStaticData_0816B258[i]);
        SET_ICON_FRAME_NIBBLE(self->iconsB0[i]);
        self->iconsB0[i]->field_3c = 0x80;
    }

    sub_80060AC(sub_8006864(self->field_10), self->buf38);
    sub_80060AC(sub_8006820(self->field_10), self->buf3b);
    sub_80060AC(sub_80067EC(self->field_10), self->buf3e);
    sub_80060AC(sub_80068A8(self->field_10), self->buf35);
    sub_80060AC(0x14, self->buf4c);
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
    sub_80087C0(&icon->base);
    sub_80087B4(&icon->base);
    sub_800872C(&icon->base, 0);
}

/* The medal/rank award widget (see docs/rom_map.md's overlay_ui
 * "Correction" section): reads the current level's completion time
 * (bits 3-15 of the save block's per-level record word at
 * `field_10 + 4 + levelIdx * 4` - level_menu.h's `struct level_save`
 * `time:13`), formats it via FormatCentiseconds, then compares it
 * against `gLevelTable[levelIdx]`'s bronze/silver/gold
 * thresholds and constructs the icon at `field_bc`, tagged with each
 * medal (from `gStaticData_0816B270`, the same table sub_8005C58 uses)
 * whose threshold was met. `field_6c` is an "earned" flag. */
void sub_8005D44(struct pause_screen_results *self)
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
    *slot = (struct settings_icon_actor *)sub_8008904((struct actor *)sub_8026EDC(0x40));
    (*slot)->field_20 = (void **)((u8 *)(**gUnknown_030012D0) + (0xc6 << 1));
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
