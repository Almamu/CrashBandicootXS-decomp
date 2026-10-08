#ifndef GUARD_GOBJ_1A794_H
#define GUARD_GOBJ_1A794_H

#include <libgcc.h>
#include "util.h"
#include "crates.h"
#include "player.h"
#include "bosses.h"
#include "gfx.h"
#include "objects.h"
#include "memory.h"
#include "level.h"
#include "globals.h"
#include "match.h"

/* What is left of the platforms' C views (GitHub issue #25, ROM
 * 0x0801A794-0x0801B85C): the platforms are C++ (include/platform.hpp's
 * Platform and PlatformMover, src/objects/platform*.cpp), and `struct
 * gobj`, their and the ground sprites' C view, went with its last user
 * (#656), as did the sprite bank's copies (`struct anim_table` and its
 * `struct anim_rec` records: Sprite's `bank` is sprite_bank.h's `struct
 * sprite_bank`). Kept here: the platform collision's position pair and
 * the platform spawn record (CreatePlatform). */

struct spawn_rec {
    u8 flags; // 0x00
    u8 unk_01[3];
    u32 type;  // 0x04
    s32 distX; // 0x08
    s32 distY; // 0x0C
    s16 dirX;  // 0x10
    s16 dirY;  // 0x12
    s16 flag;  // 0x14
};

extern s32 _call_via_r1(void *self, void *fn);
extern s32 _call_via_r2(void *self, void *arg, void *fn);
extern s32 _call_via_r3(void *self, void *arg1, s32 arg2, void *fn);
extern void _call_via_r4(void *self, s32 a, s32 b, s32 c);

#endif /* GUARD_GOBJ_1A794_H */
