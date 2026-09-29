#ifndef GUARD_GFX_PART_H
#define GUARD_GFX_PART_H

/* A sub_8009ED0/sub_8008434-built on-screen "part" object and its
 * animation bank, as driven by the per-frame helpers
 * sub_80087C0/sub_80087B4/sub_800872C/sub_800815C. First written for
 * src/graphics/actor_part_188d0.c (issue #23); also used by the
 * "trigger effect type N" spawners in
 * src/graphics/graphics_loading_1ea5c.c. The same object is described
 * under other local names elsewhere (`struct gobj` in
 * include/gobj_1a794.h, `struct settings_icon_actor` in
 * include/pause_screen_results.h) - not merged yet. */

struct anim_record
{
    u8 unk_00[0x16];
    u8 frameCount; // 0x16
    u8 unk_17[5];
};

struct anim_bank
{
    struct anim_record *records;
};

struct gfx_vec
{
    s32 x;
    s32 y;
};

struct gfx_part
{
    struct gfx_vec pos;     // 0x00
    u16 id;                 // 0x08
    u8 unk_0A;              // 0x0A
    u8 unk_0B;
    u8 gone:1;              // 0x0C bit 0
    u8 flags_1:1;
    u8 hidden:1;            // 0x0C bit 2
    u8 flags_3:1;
    u8 active:1;            // 0x0C bit 4
    u8 flags_5:3;
    u8 unk_0D[0x13];
    struct anim_bank *bank; // 0x20
    u8 unk_24[4];
    u8 unk_28_0:2;          // 0x28
    u8 unk_28_2:2;
    u8 flipX:1;
    u8 unk_28_5:3;
    u8 frameNibble:4;       // 0x29
    u8 unk_29_4:4;
    u8 unk_2A[2];
    u8 unk_2C;              // 0x2C
    u8 tag;                 // 0x2D
    u8 unk_2E[2];
    s32 frame;              // 0x30 - step within the animation
    s32 stepTimer;          // 0x34 - reset to the animation's `duration` (sub_80083B8)
    u8 animDone;            // 0x38
    u8 unk_39[0xB];
    void *ctrl;             // 0x44
    u8 unk_48[0x24];
    s32 prevX;              // 0x6C - previous position (Q8), cached by sub_8009DF4
    s32 prevY;              // 0x70
};

/* The whole flags byte at +0x0C, for the spots that update it as one
 * byte through register pins (see sub_80188FC). */
#define PART_FLAGS(p) (*((u8 *)(p) + 0xC))

#endif /* GUARD_GFX_PART_H */
