#ifndef GUARD_BOX_PART_H
#define GUARD_BOX_PART_H

/* The collision/animation view of a `part` object (the same object as
 * include/gfx_part.h's `struct gfx_part` / include/gobj_1a794.h's
 * `struct gobj` / include/actor.h's `struct actor`), as read by the
 * early actor/collision core in src/graphics/actor_part.c,
 * actor_part3.c, actor_part7.c, actor_part7b.c and actor_part108.c.
 * Only the fields those functions touch are named.
 *
 * The mirror bits at 0x28 are `u32` bitfields on purpose: that is what
 * makes the compiler test them with `lsl #27`/`lsl #26` + a sign test,
 * as the ROM does (a `u8` container gives `movs #0x10; ands`). */

/* One KEYFRAME_SIZE-byte keyframe record. The {offX, offY, w, h}
 * collision box (struct part_box below) sits at +0x4 or +0xc depending
 * on which table the part uses. */
struct keyframe {
    u8 unk_00[0x15];
    u8 duration;        // 0x15 - ticks per step
    u8 steps;           // 0x16 - steps before the keyframe ends
    u8 flags;           // 0x17 - bit 1: loops (animDone is not set)
    u8 unk_18[4];
};

#define KEYFRAME_SIZE 0x1c

/* A gcc 2.x method-table record: `this` adjustment plus code pointer.
 * A part's method table (`vtable`) is indexed by byte offset - see
 * PART_METHOD. */
struct part_method {
    s16 thisOffset;
    u8 unk_02[2];
    void *fn;
};

#define PART_METHOD(obj, off) ((struct part_method *)((obj)->vtable + (off)))

struct box_part {
    s32 x;              // 0x00 - Q8 fixed-point
    s32 y;              // 0x04 - Q8 fixed-point
    u16 id;             // 0x08 - bit index in the "gone" bitmap, 0xFFFF for none
    u8 kind;            // 0x0A
    u8 unk_0B;
    u8 flags;           // 0x0C - bit 0 gone, bit 2 visible, bit 3 hit
    u8 flags2;          // 0x0D - bit 3: solid (pushes the player out)
    u8 unk_0E[0xa];
    u8 *vtable;         // 0x18 - method table, see PART_METHOD
    u8 unk_1C[4];
    struct keyframe **keyframes; // 0x20
    u8 unk_24[4];
    u32 unk_28_0:4;     // 0x28
    u32 mirrorX:1;
    u32 mirrorY:1;
    u32 unk_28_6:2;
    u8 unk_29[3];
    u8 animating;       // 0x2C - nonzero while the keyframe timer runs
    u8 frame;           // 0x2D - current keyframe index
    u8 unk_2E[2];
    s32 tick;           // 0x30 - per-keyframe step counter
    s32 timer;          // 0x34 - ticks spent on the current step
    u8 animDone;        // 0x38
    u8 unk_39[0x2b];
    s32 unk_64;         // 0x64
};

/* A keyframe record's {offX, offY, w, h} collision box, reached through
 * a pointer to it (this compiler pads every struct to a word multiple,
 * so it can't be embedded in the record). Sits at record+0xc for the
 * table sub_8007B00 reads and at record+0x4 for the one sub_8007B98/
 * sub_800AAEC read (include/gobj_1a794.h's `struct anim_box`). */
struct part_box {
    s16 offX;
    s16 offY;
    u8 w;
    u8 h;
};

/* The {x, y, w, h} box the collision functions build and pass around,
 * mostly by value (the same layout as `struct aabb` elsewhere). */
struct part_aabb {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
};

/* The part list the per-frame collision passes walk (sub_800891C
 * compacts `items` and fills `visible`; sub_8008A40 walks `visible`). */
struct part_list {
    s32 capacity;       // 0x00
    s32 count;          // 0x04
    s32 visibleCount;   // 0x08
    struct box_part **items;   // 0x0C
    struct box_part **visible; // 0x10
};

#endif /* GUARD_BOX_PART_H */
