#ifndef GUARD_PLAYER_CTRL_H
#define GUARD_PLAYER_CTRL_H

/* The player-input controller object of src/graphics/actor_part_16048.c
 * (GitHub issues #19/#20, ROM 0x08016048-0x08017524): a C++-style class
 * with gcc 2.x method table gPlayerCtrlVtable (+0x0C UpdatePlayerCtrl
 * per-frame update, +0x14 PlayerCtrlHandleEvent message handler, +0x1C AttachPlayerCtrl
 * set target, +0x4C DestroyPlayerCtrl destructor; the rest are base-class
 * sub_800B6xx/sub_800B8xx functions). Constructor InitPlayerCtrl (called from
 * game_loop39.c), whose field reset is actor_part57.c's ResetPlayerCtrl.
 * The dispatchers sub_80159F8/sub_8015C6C/sub_8015DF8 (actor_part86.c/
 * actor_part86b.c) and sub_8015FDC (actor_part57b.c) are methods of the
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

/* one gStaticData_0816B61C record */
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
    u16 id;             // 0x08 - bitmap id (see sub_80072D8)
    u8 kind;            // 0x0A - object kind passed to the hit handlers (0x13 while attacking, else 1)
    u8 unk_0B;
    u8 gone:1;          // 0x0C - bit 0: removed (see sub_80072D8)
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
    s32 velAX;          // 0x48 - struct gobj.velA
    s32 velAY;          // 0x4C
    s32 velAZ;          // 0x50
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
    s32 state;                    // 0x08 - index into gStaticData_0816C250
    struct pctrl_vtable *vtable;  // 0x0C
    struct pctrl_target *target;  // 0x10
    s32 unk_14;                   // 0x14
    s32 timer;                    // 0x18
    s32 timerMax;                 // 0x1C
    u8 repeat;                    // 0x20 - D-pad auto-repeat countdown
    u8 level;                     // 0x21 - 0..12, column of gStaticData_0816C070
    u8 mode;                      // 0x22 - row of gStaticData_0816C070
    u8 cooldown;                  // 0x23
    u8 valueA;                    // 0x24
    u8 valueB;                    // 0x25
    u8 unk_26;                    // 0x26
    u8 counter;                   // 0x27
    u32 deadline;                 // 0x28 - gRoomFrameCount + 16 (sub_80159F8); the state waits until it passes or the anim ends
    u8 hasA;                      // 0x2C
    u8 hasB;                      // 0x2D
};

#endif /* GUARD_PLAYER_CTRL_H */
