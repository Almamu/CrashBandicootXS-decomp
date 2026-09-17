#include "core.h"
#include "actor.h"

/* A small `struct actor`-derived on-screen icon: the first 0x1c bytes
 * are a plain `struct actor` (see actor.h), then a second keyframe-
 * table pointer at +0x20 and a frame index at +0x2d - both already
 * established by the already-matched sub_800815C/sub_80080C0
 * (src/graphics/actor_part4.c), which read this exact same object
 * through raw offsets. +0x29's low nibble and +0x3c are new fields this
 * chunk's functions write but don't otherwise interpret. Allocated with
 * `sub_8026EDC(0x40)` - bigger than plain `struct actor` (0x1c), so it
 * has more trailing fields this chunk's functions never touch. */
struct settings_icon_actor {
    struct actor base;    /* 0x00-0x1b */
    u8 unused_1c[0x20 - 0x1c];
    void **field_20;        /* 0x20 - keyframe-table pointer, see sub_800815C */
    u8 unused_24[0x29 - 0x24];
    u8 field_29;               /* 0x29 - low nibble set from sub_800815C's result */
    u8 unused_2a[0x2d - 0x2a];
    u8 frameIndex;                /* 0x2d - current keyframe index, see sub_800815C */
    u8 unused_2e[0x3c - 0x2e];
    u16 field_3c;                   /* 0x3c - sub_8005B80/sub_8005C58 only, set to 0x80 */
};

/* A fixed {x, y} screen-position pair, as consumed by sub_800737C. */
struct icon_pos {
    s32 x;
    s32 y;
};

/* The composite pause/options screen's "results" sub-region: five
 * settings-row icon widgets (a single one at field_88, then three small
 * arrays at 0x8c/0x9c/0xb0, then one more at field_bc) plus their
 * shared scratch buffers/counters. This may be the same underlying
 * object pause_options_screen.h documents (built by the still-raw
 * sub_8004D74/sub_8004EC0/sub_800599C/sub_8005100), just through a
 * wider set of offsets that chunk's functions never touched - not
 * reconciled with that struct yet (its rowObjA/B/C arrays at
 * 0xa8-0xe3 don't line up with the 0x8c/0x9c/0xb0/0xbc arrays actually
 * written here), so kept as its own locally-scoped type per
 * docs/workflow.md step 7's carve-out for genuine uncertainty. Only
 * the fields sub_8005A78/AE8/B80/C58/D44 touch are named here -
 * src/graphics/settings_menu7.c's `struct pause_screen_row_counts` is
 * a second, non-overlapping partial view of the same object for
 * sub_8005EF4/FBC's fields (0x14/0x18/0x4f/0x57/0x60/0x64). */
struct pause_screen_results {
    u8 unused_00[0x10];
    void *field_10;          /* 0x10 - a row-stats handle, passed to sub_8006920/64/68A8/etc and read via gUnknown_030012C0's per-level index table in sub_8005D44 */
    u8 unused_14[0x2c - 0x14];
    u8 buf2c[3];    /* sub_8005A78 */
    u8 buf2f[3];    /* sub_8005B80 */
    u8 buf32[3];    /* sub_8005B80 */
    u8 buf35[3];    /* sub_8005C58 */
    u8 buf38[3];    /* sub_8005C58 */
    u8 buf3b[3];    /* sub_8005C58 */
    u8 buf3e[3];    /* sub_8005C58 */
    u8 unused_41[5]; /* used by sub_800599C, still raw */
    u8 buf46[3];    /* sub_8005A78 */
    u8 buf49[3];    /* sub_8005B80 */
    u8 buf4c[3];    /* sub_8005C58 */
    u8 unused_4f[0x6c - 0x4f];
    u8 field_6c;          /* sub_8005D44 */
    u8 unused_6d[0x7c - 0x6d];
    u8 timeBuf[0xc];         /* 0x7c - sub_8005D44, FormatCentiseconds dest */
    struct settings_icon_actor *field_88;      /* sub_8005A78 */
    struct settings_icon_actor *icons8c[4];      /* sub_8005AE8 */
    struct settings_icon_actor *icons9c[5];        /* sub_8005B80 */
    struct settings_icon_actor *iconsB0[3];          /* sub_8005C58 */
    struct settings_icon_actor *field_bc;              /* sub_8005D44 */
};

extern void *sub_8026EDC(s32 size);
extern struct actor *sub_8008904(struct actor *part);
extern void sub_800737C(struct actor *self, s32 arg1, s32 arg2);
extern s32 sub_800815C(struct actor *part);
extern s32 sub_800695C(void *arg0);
extern s32 sub_80060AC(s32 value, void *dest);
extern void ***gUnknown_030012D0;
extern struct icon_pos gStaticData_0816B1E4;

/* Sets `field_29`'s low nibble from sub_800815C's result, keeping the
 * high nibble - the recurring last step of every icon constructor in
 * this file. Written with explicit register pins (matching the
 * SUB_8006600_* macros in src/graphics/oam_count.c) because gcc's
 * constant-propagation otherwise folds the ROM's two-instruction
 * "movs r1,#0x10 / rsbs r1,r1,#0" -0x10 load into a single `sub`
 * relative to the just-used 0xf mask, which the ROM never does. */
#define UPDATE_ICON_FRAME_NIBBLE(iconExpr) \
    do { \
        register s32 _ret asm("r0") = sub_800815C(&(iconExpr)->base); \
        register u8 *_addr asm("r2") = &(iconExpr)->field_29; \
        register s32 _mask asm("r1"); \
        register u8 _byte asm("r3"); \
        _mask = 0xf; \
        _ret &= _mask; \
        asm volatile("mov %0, #0x10\n\tneg %0, %0" : "=r" (_mask)); \
        _byte = *_addr; \
        _mask &= _byte; \
        _mask |= _ret; \
        *_addr = _mask; \
    } while (0)

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

#if NON_MATCHING
/* The four functions below (sub_8005AE8, sub_8005B80, sub_8005C58,
 * sub_8005D44 - plus sub_8005EF4/FBC, src/graphics/settings_menu7.c,
 * same object) are reconstructed (semantics understood, cross-checked
 * against docs/rom_map.md's "Correction: overlay_ui is a small family
 * of screens" writeup) but NOT YET BYTE-MATCHING - parked here the
 * same way sub_8006600 (src/graphics/oam_count.c) is. All four hit the
 * same class of gcc-2.9 register-allocation difficulty already
 * documented for sub_8006600 and sub_80374D0's neighbor sub_8037388
 * (src/audio/counter_selector_setup.c): the loop/self pointer never
 * ends up in `r8`/`sb` here the way the ROM's does, no matter how the
 * source is rephrased, and closing that gap would need the same kind
 * of heavy per-call-site SUB_8006600_*-style register-pin macros this
 * pass didn't have budget for across four near-identical functions (the
 * `field_29`-update tail every one of them shares - see
 * UPDATE_ICON_FRAME_NIBBLE below - *did* get pinned down exactly this
 * way, and matches sub_8005A78's real bytes byte-for-byte; it's only
 * the surrounding loop/branch scaffolding that doesn't). */

extern void sub_80087C0(struct actor *part);
extern void sub_80087B4(struct actor *part);
extern void sub_800872C(struct actor *part, u8 val);
extern u32 gStaticData_0816B20C[];
extern struct icon_pos gStaticData_0816B1EC[];

/* Builds the 4-icon array at `icons8c`: one per `gStaticData_0816B1EC`
 * position entry, keyframe-table base `0xe4<<1` off the same shared
 * table `sub_8005A78` uses, frame index from `gStaticData_0816B20C`,
 * then the standard sub-counter/frame-counter/"done"-flag reset trio. */
void sub_8005AE8(struct pause_screen_results *self)
{
    s32 i;
    struct settings_icon_actor *icon;

    for (i = 0; i <= 3; i++) {
        icon = (struct settings_icon_actor *)sub_8008904((struct actor *)sub_8026EDC(0x40));
        self->icons8c[i] = icon;
        icon->field_20 = (void **)((u8 *)(**gUnknown_030012D0) + (0xe4 << 1));
        icon->frameIndex = (u8)gStaticData_0816B20C[i];
        sub_80087C0(&icon->base);
        sub_80087B4(&icon->base);
        sub_800872C(&icon->base, 0);
        sub_800737C(&icon->base, gStaticData_0816B1EC[i].x, gStaticData_0816B1EC[i].y);
        UPDATE_ICON_FRAME_NIBBLE(icon);
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
    struct settings_icon_actor *icon;
    s32 a, b;

    for (i = 0; i <= 4; i++) {
        icon = (struct settings_icon_actor *)sub_8008904((struct actor *)sub_8026EDC(0x40));
        self->icons9c[i] = icon;
        icon->field_20 = (void **)((u8 *)(**gUnknown_030012D0) + (0xc0 << 1));
        icon->frameIndex = (u8)gStaticData_0816B244[i];
        sub_80087C0(&icon->base);
        sub_80087B4(&icon->base);
        sub_800872C(&icon->base, 0);
        sub_800737C(&icon->base, gStaticData_0816B21C[i].x, gStaticData_0816B21C[i].y);
        UPDATE_ICON_FRAME_NIBBLE(icon);
        icon->field_3c = 0x80;
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
    struct settings_icon_actor *icon;

    for (i = 0; i <= 2; i++) {
        icon = (struct settings_icon_actor *)sub_8008904((struct actor *)sub_8026EDC(0x40));
        self->iconsB0[i] = icon;
        icon->field_20 = (void **)((u8 *)(**gUnknown_030012D0) + (0xc6 << 1));
        icon->frameIndex = (u8)gStaticData_0816B270[i];
        sub_80087C0(&icon->base);
        sub_80087B4(&icon->base);
        sub_800872C(&icon->base, 0);
        sub_800737C(&icon->base, gStaticData_0816B258[i].x, gStaticData_0816B258[i].y);
        UPDATE_ICON_FRAME_NIBBLE(icon);
        icon->field_3c = 0x80;
    }

    sub_80060AC(sub_8006864(self->field_10), self->buf38);
    sub_80060AC(sub_8006820(self->field_10), self->buf3b);
    sub_80060AC(sub_80067EC(self->field_10), self->buf3e);
    sub_80060AC(sub_80068A8(self->field_10), self->buf35);
    sub_80060AC(0x14, self->buf4c);
}

extern void *gUnknown_030012C0;
extern s32 sub_802332C(void *arg0);
extern void FormatCentiseconds(s32 value, u8 *buf);

/* Same per-level bronze/silver/gold threshold table src/graphics/oam_count.c's
 * `struct threshold_table_entry`/`gStaticData_0816C86C` already document -
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

extern struct threshold_table_entry gStaticData_0816C86C[];
extern struct icon_pos gStaticData_0816B27C;

/* The medal/rank award widget (see docs/rom_map.md's overlay_ui
 * "Correction" section): reads the current level's completion time
 * (`self->field_10[levelIdx+1]`, a raw record whose low bits are a
 * centiseconds time), formats it via FormatCentiseconds, then compares
 * it against `gStaticData_0816C86C[levelIdx]`'s bronze/silver/gold
 * thresholds and constructs the icon at `field_bc` tagged with the
 * matching medal (from `gStaticData_0816B270`, the same table
 * sub_8005C58 uses) only if a threshold was actually met - otherwise
 * leaves `field_6c` (an "earned" flag) clear and the icon untagged. */
void sub_8005D44(struct pause_screen_results *self)
{
    s32 levelIdx;
    s32 raw;
    s32 time;
    struct threshold_table_entry *entry;
    struct settings_icon_actor *icon;
    u8 earned;

    levelIdx = sub_802332C(gUnknown_030012C0);
    raw = *((s32 *)self->field_10 + levelIdx + 1);
    time = (s32)((u32)raw << 16) >> 19;
    FormatCentiseconds(time, self->timeBuf);

    entry = &gStaticData_0816C86C[levelIdx];
    earned = 0;
    if (time != 0 && time <= entry->threshold_08) {
        earned = 1;
    }
    self->field_6c = earned;

    icon = (struct settings_icon_actor *)sub_8008904((struct actor *)sub_8026EDC(0x40));
    self->field_bc = icon;
    icon->field_20 = (void **)((u8 *)(**gUnknown_030012D0) + (0xc6 << 1));
    sub_800737C(&icon->base, gStaticData_0816B27C.x, gStaticData_0816B27C.y);

    if (time != 0) {
        if (time <= entry->threshold_08) {
            icon->frameIndex = (u8)gStaticData_0816B270[2];
            sub_80087C0(&icon->base);
            sub_80087B4(&icon->base);
            sub_800872C(&icon->base, 0);
        }
        if (time <= entry->threshold_0C) {
            icon->frameIndex = (u8)gStaticData_0816B270[1];
            sub_80087C0(&icon->base);
            sub_80087B4(&icon->base);
            sub_800872C(&icon->base, 0);
        }
        if (time <= entry->threshold_10) {
            icon->frameIndex = (u8)gStaticData_0816B270[0];
            sub_80087C0(&icon->base);
            sub_80087B4(&icon->base);
            sub_800872C(&icon->base, 0);
        }
        UPDATE_ICON_FRAME_NIBBLE(icon);
    }
}
#endif /* NON_MATCHING */
