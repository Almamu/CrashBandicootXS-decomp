#ifndef GUARD_LEVEL_STATE_H
#define GUARD_LEVEL_STATE_H

/*
 * The level/session state object `gUnknown_030012C0` points at (0x1CC
 * bytes). Its accessor family is src/system/game_loop2.c
 * (sub_8022EA8-sub_8023484); game_loop55.c's level loop still carries
 * its own copy of the same layout (`struct level_state` there).
 *
 * The first 0x68 bytes are the per-attempt block the frame loop
 * snapshots into `snapE4`/`snap14C` and restores from (game_loop55.c).
 */

/* The level's category record (level_state.cat) - one of the medal
 * table's list items (game_loop18.c's `struct MedalListItem`). */
struct level_category
{
    u8 unk_00[8];
    s32 kind;                       // 0x08 - 0-2 plain level, 3 actor-category level
    u8 unk_0c[4];
    u16 category;                   // 0x10
};

struct level_state_1c8;

struct level_state
{
    u8 unk_00[2];                   // 0x000 - packed lives (bits 0-6), wumpa (9-15) and maskLevel (7-8) (sub_80236AC/sub_80236EC)
    u8 flags;                       // 0x002 - bits 4-7: sub_8023168..sub_80231CC
    u8 unk_03;
    u32 levelFlags[0x19];           // 0x004 - one word per level, indexed by `level` (sub_80233FC)
    s32 unk_68;                     // 0x068
    s32 wumpa;                      // 0x06C - at 100 it wraps and adds a life (sub_8023430)
    s32 unk_70;                     // 0x070 - counter; reaching `unk_bc` sets the level's bit 1 (sub_8022FEC)
    s32 lives;                      // 0x074 - 5 at the start (sub_80231E4), capped at 99
    s32 maskLevel;                  // 0x078 - 0-3; 3 plays the invincibility jingle (sub_80231EC)
    s32 unk_7c;                     // 0x07C - free-running counter (sub_80232E0/sub_80232E4)
    s32 unk_80;                     // 0x080
    s32 unk_84;                     // 0x084 - the cap unk_7c is checked against (graphics_loading_1e990.c)
    s32 unk_88;                     // 0x088
    u8 timeTrial;                   // 0x08C - nonzero: no lives lost, the clock runs
    u8 unk_8d[3];
    s32 minutes;                    // 0x090 - the time-trial clock (sub_8022F2C), capped at 99
    s32 seconds;                    // 0x094
    s32 tenths;                     // 0x098
    s32 frames;                     // 0x09C - 0-5, one tenth every 6 frames
    s32 countdown;                  // 0x0A0 - frames the clock stays frozen (sub_8022EA8 adds seconds * 60)
    u8 unk_a4;                      // 0x0A4 - status flags, get/clear (and some set) accessors each
    u8 unk_a5;                      // 0x0A5
    u8 unk_a6;                      // 0x0A6
    u8 unk_a7;                      // 0x0A7
    u8 unk_a8;                      // 0x0A8
    u8 unk_a9;                      // 0x0A9
    u8 unk_aa[2];
    s32 unk_ac;                     // 0x0AC - amount sub_802306C adds to unk_70
    s32 unk_b0;                     // 0x0B0
    s32 unk_b4;                     // 0x0B4 - takes unk_ac instead while unk_a4 is set
    s32 unk_b8;                     // 0x0B8
    s32 unk_bc;                     // 0x0BC - unk_70's target
    s32 unk_c0;                     // 0x0C0 - bit mask (sub_802314C/sub_8023158)
    s32 level;                      // 0x0C4 - also the head of the progress record (game_loop18.c's struct level_progress)
    s32 unk_c8;                     // 0x0C8
    s32 unk_cc;                     // 0x0CC - checkpoint copy of unk_70 (sub_802356C/sub_8023548)
    u8 unk_d0;                      // 0x0D0 - checkpoint copy of unk_a9
    u8 unk_d1[3];
    s32 checkpointX;                // 0x0D4 - the player's position at the checkpoint
    s32 checkpointY;                // 0x0D8
    struct level_category *cat;     // 0x0DC - level_progress.item
    u8 unk_e0;                      // 0x0E0 - checkpoint flag (sub_802356C)
    u8 unk_e1[3];
    u8 snapE4[0x68];                // 0x0E4 - copies of the first 0x68 bytes
    u8 snap14C[0x68];               // 0x14C
    void *savedBitmap;              // 0x1B4
    s32 unk_1b8;                    // 0x1B8
    s32 unk_1bc;                    // 0x1BC
    s32 unk_1c0;                    // 0x1C0 - a point (sub_8023500); its low halves go to sub_801EB04 when unk_70 hits unk_bc
    s32 unk_1c4;                    // 0x1C4
    struct level_state_1c8 *unk_1c8; // 0x1C8 - sub_8023318
};

COMPILE_TIME_ASSERT(sizeof(struct level_state) == 0x1CC);

/* The record `level_state.unk_1c8` points at (sub_8023378). */
struct level_state_1c8
{
    u8 unk_00[0x10];
    s32 unk_10;                     // 0x10 - sub_8023378 returns 3 minus this
};

#endif /* GUARD_LEVEL_STATE_H */
