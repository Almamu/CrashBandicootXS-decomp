#ifndef GUARD_ORBIT_PART_H
#define GUARD_ORBIT_PART_H

#include "actor.h"
#include "action_obj.h"

/* The 0x54-byte "orbiting hazard / collectible" part object spawned by
 * CreateExtraLife (extra_life.c) and CreateWumpa (wumpa_update.c) and driven
 * by the per-frame updaters UpdateExtraLife/UpdateWumpa and the orbit helpers
 * UpdateExtraLifeHop/UpdateWumpaHop (GitHub issues #14/#15). It starts with the
 * shared `struct actor` header; `bank` is the same animation-record bank
 * `struct player` (player.h) points at (records are 0x1C bytes,
 * `frameCount` at +0x16). */
struct orbit_vec {
    s32 x;
    s32 y;
};

struct orbit_part {
    struct actor base; // 0x00 - x/y (Q8), id at +0x08, flags at +0x0C
    u8 unk_1C[4];
    struct act_anim_bank *bank; // 0x20
    u8 unk_24;
    u8 screenSpace; // 0x25 - 1: x/y are screen coordinates (struct gobj.screenSpace); set when
                    //        the wumpa flies to the HUD (SendWumpaToHud)
    u8 unk_26[2];
    u32 unk_28_0:4; // 0x28 (same bit layout as struct crate)
    s32 flipX:1;    //      bit 4: X mirrored
    s32 flipY:1;    //      bit 5: Y mirrored
    u32 unk_28_6:2;
    u32 slotNibble:4; // 0x29 - palette slot
    u32 unk_29_4:4;
    u32 unk_2A:16;
    u8 animating; // 0x2C - nonzero while the keyframe timer runs
    u8 tag;       // 0x2D - index into bank->records
    u8 unk_2E[2];
    s32 frame; // 0x30
    u8 unk_34[4];
    u8 animDone; // 0x38 - set once a non-looping animation ends (SetSpriteAnimDone)
    u8 unk_39[3];
    u16 timer; // 0x3C
    u8 unk_3E[2];
    s32 velX;                // 0x40 (Q8 per frame)
    s32 velY;                // 0x44
    u8 state;                // 0x48 - 0 idle/orbiting, 1/2 flying off, 3 parked
    u8 counter;              // 0x49
    u8 mode;                 // 0x4A - orbit mode (1: x - offset, 2: x + offset)
    u8 phase;                // 0x4B - index into gSineTable
    struct orbit_vec anchor; // 0x4C - orbit centre / home position (Q8)
};

/* The x/y pair at the head of the object, as the struct the spawners copy
 * into `anchor` in one go (the ROM's paired `ldr; ldr; str; str`). */
#define ORBIT_POS(self) (*(struct orbit_vec *)&(self)->base.x)

COMPILE_TIME_ASSERT(orbit_part_h, sizeof(struct orbit_part) == 0x54);

#endif // GUARD_ORBIT_PART_H
