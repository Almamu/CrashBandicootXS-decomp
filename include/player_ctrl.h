#ifndef GUARD_PLAYER_CTRL_H
#define GUARD_PLAYER_CTRL_H

/* The player-input controller object of src/player/swim_ctrl.c
 * (GitHub issues #19/#20, ROM 0x08016048-0x08017524): the underwater
 * (scuba-diving) controller game_loop39.c attaches in room kind 1, with
 * sprite bank 1 (Crash in an air tank and flippers). It is a C++-style class
 * with gcc 2.x method table gPlayerCtrlVtable (+0x0C UpdatePlayerCtrl
 * per-frame update, +0x14 PlayerCtrlHandleEvent message handler, +0x1C AttachPlayerCtrl
 * set target, +0x4C DestroyPlayerCtrl destructor; the rest are base-class
 * sub_800B6xx/sub_800B8xx functions). Constructor InitPlayerCtrl (called from
 * game_loop39.c), whose field reset is action_ctrl.c's ResetPlayerCtrl.
 * The dispatchers StartPlayerCtrlStroke/StartPlayerCtrlSpin/ApplyPlayerCtrlSwimDrift (swim_ctrl_stroke.c) and SetPlayerSwimDriftY (swim_ctrl_drift.c) are methods of the
 * same class. */

struct pctrl_method
{
    s16 thisOffset;
    u8 unk_2[2];
    void *fn;
};

struct pctrl_vtable
{
    u8 unk_00[0x20];
    struct pctrl_method setMode;  // 0x20
    u8 unk_28[0x28];
    struct pctrl_method setAnim;  // 0x50
};

/* one gPlayerCtrlMotionRecords record */
struct pctrl_anim
{
    s32 a;
    s32 b;
    s32 c;
};

struct pctrl_anim_pair
{
    u32 a;
    u32 b;
};

/* 28-byte animation record (same layout as gobj_1a794.h's anim_rec) */
struct pctrl_anim_rec
{
    u8 unk_00[0x14];
    u8 paletteId;   // 0x14 - LoadPaletteSlot record id
    u8 unk_15;
    u8 frames;      // 0x16
    u8 unk_17[5];
};

/* The bitfield byte at the target's +0x28 (same layout as
 * actor_part_1967c.c's `struct part_f28`). `flipX` is a signed field: the
 * ROM tests it with `lsl #27` / sign branch. */
struct pctrl_f28
{
    u8 unk_0:4;
    u32 flipX:1;
    u32 flipY:1;
    u8 unk_6:2;
} __attribute__((packed));

/* The controlled object (the player, gPlayer). Same layout as
 * gobj_1a794.h's `struct gobj` for the fields both read. */
struct pctrl_target
{
    s32 x;              // 0x00
    s32 y;              // 0x04
    u16 id;             // 0x08 - bitmap id (see MarkEntityGone)
    u8 kind;            // 0x0A - object kind passed to the hit handlers (0x13 while attacking, else 1)
    u8 unk_0B;
    u8 gone:1;          // 0x0C - bit 0: removed (see MarkEntityGone)
    u8 unk_0C_1:5;
    u8 flag6:1;
    u8 flag7:1;
    u8 unk_0D[0x13];
    struct { struct pctrl_anim_rec *records; } *anim; // 0x20
    u8 unk_24[4];
    struct pctrl_f28 f28; // 0x28
    u8 slot:4;          // 0x29 - low nibble
    u8 unk_29_4:4;
    u8 unk_2A[3];
    u8 tag;             // 0x2D
    u8 unk_2E[2];
    s32 frame;          // 0x30
    s32 stepTimer;      // 0x34 - ticks spent on the current step
    u8 animDone;        // 0x38 - set once a non-looping animation ends
    u8 unk_39[0xF];
    s32 rampXStart;     // 0x48 - struct gobj.rampX (start, step, target)
    s32 rampXStep;      // 0x4C
    s32 rampXTarget;    // 0x50
    u8 unk_54[0xC];
    s32 speedX;         // 0x60
    s32 speedY;         // 0x64
    u8 hitAxes;         // 0x68 - collision axes the terrain probe resolved
    u8 unk_69[0xB];
    s32 hitMask;        // 0x74 - probe axes hit this frame (bits 0-1: X, 2-3: Y)
    u8 unk_78[0x1A];
    u8 unk_92;          // 0x92
    u8 unk_93[0x71];
    u8 dead;             // 0x104
};

struct player_ctrl
{
    u8 unk_00[4];
    struct { struct pctrl_anim_pair *entries; } *animSet; // 0x04
    s32 state;                    // 0x08 - index into gPlayerCtrlStateFuncs
    struct pctrl_vtable *vtable;  // 0x0C
    struct pctrl_target *target;  // 0x10
    s32 unk_14;                   // 0x14
    s32 timer;                    // 0x18
    s32 timerMax;                 // 0x1C
    u8 repeat;                    // 0x20 - D-pad auto-repeat countdown
    u8 tilt;                      // 0x21 - swim direction, 0 (up) .. 6 (level) .. 12 (down); column of gPlayerCtrlModeAnimRows
    u8 mode;                      // 0x22 - row of gPlayerCtrlModeAnimRows (0 idle, 1 swim, 2 stroke, 3 spin, 4-7 turn)
    u8 spinCooldown;              // 0x23 - frames until StartPlayerCtrlSpin is allowed again (set to 12)
    u8 motionX;                   // 0x24 - queued X motion entry (animSet->entries[].a)
    u8 motionY;                   // 0x25 - queued Y motion entry (animSet->entries[].b)
    u8 unk_26;                    // 0x26
    u8 idleTimer;                 // 0x27 - PlayerCtrlStateIdle's bob timer (Y motion 1 at 30, 2 at 60)
    u32 deadline;                 // 0x28 - gRoomFrameCount + 16 (StartPlayerCtrlStroke); the state waits until it passes or the anim ends
    u8 motionXPending;            // 0x2C - motionX is queued
    u8 motionYPending;            // 0x2D - motionY is queued
};

#endif /* GUARD_PLAYER_CTRL_H */
