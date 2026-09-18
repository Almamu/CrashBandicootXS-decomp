#ifndef __HUD_H__
#define __HUD_H__

/* Shared shapes for the HUD stat-widget/icon-slot family documented in
 * docs/rom_map.md's "hud" investigation (the dispatcher at sub_80274EC
 * and its digit/icon-counter callees). Only `sub_8027838`
 * (src/graphics/hud_counter.c) and the icon-slot constructor family in
 * src/graphics/hud_icon_slot.c use these so far - the rest of the
 * family (sub_80274EC, sub_802757C, sub_802763C, sub_8027940,
 * sub_8027D5C, sub_8027E88) is still raw asm. */

struct hud_anim_record {
    u8 unknown_00[0x16];
    u8 frame_count;
    u8 unknown_17[5];
};

struct hud_anim_data {
    struct hud_anim_record *records;
};

/* A single HUD digit/icon slot. Its first 0x18 bytes plus the `table`
 * field at +0x18 match `struct actor` (include/actor.h) byte for byte -
 * sub_802710C/sub_8027120 (src/graphics/hud_icon_slot.c) construct each
 * slot by calling the same generic `struct actor`-based table-swap
 * helpers (sub_80088F0/sub_8008904) already used by the actor/part
 * system, treating this object as one. The rest of the fields
 * (animation state) are specific to this widget family. */
struct hud_digit_part {
    u8 unknown_00[0x18];
    void *table;           /* +0x18 - see `struct actor.table` */
    u8 unknown_1c[4];
    struct hud_anim_data *anim_data;
    u8 unknown_24[9];
    u8 anim_index;
    u8 unknown_2E[2];
    s32 frame_index;
    u8 unknown_34[0xC];
};

struct hud_counter {
    s32 mode;
    s32 layout_value;
    s32 field_08;           /* +0x08 - read by sub_80274EC (gated with
                              * field_00 against 0 to decide whether to
                              * call sub_802763C); meaning not established. */
    u8 unknown_0c[0xC];      /* +0x0c */
    u8 icon_flag;            /* +0x18 - sub_80274EC's dispatcher gate for
                               * sub_8027E88 (percentage counter); also set
                               * from sub_802732C's second argument while
                               * the OAM slot array is being built. */
    u8 unknown_19[3];        /* +0x19 */
    s32 value;               /* +0x1c */
    u8 unknown_20[0xC];      /* +0x20 */
    s32 sync_value_a;        /* +0x2c - sub_802763C's change-detection
                               * cache for `sub_8023270`'s value. */
    s32 sync_value_b;        /* +0x30 - same, for `sub_8023268`. */
    s32 sync_value_c;        /* +0x34 - same, for `sub_8023260`. */
    u8 unknown_38[8];        /* +0x38 */
    s32 previous_value;      /* +0x40 */
    u8 unknown_44[0x20];     /* +0x44 */
    struct hud_digit_part *parts; /* +0x64 */
};

COMPILE_TIME_ASSERT(sizeof(struct hud_anim_record) == 0x1C);
COMPILE_TIME_ASSERT(sizeof(struct hud_digit_part) == 0x40);
COMPILE_TIME_ASSERT(sizeof(struct hud_counter) == 0x68);

#endif /* !__HUD_H__ */
