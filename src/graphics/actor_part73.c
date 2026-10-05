#include "core.h"

/* Same "self" object family as actor_part61.c/actor_part66.c - see
 * docs/matching/issue-63-0x08033ef4-actor.md. */

/* The particle-trail BG0 object (actor_part85.c's `struct particle_bg`). */
struct particle_bg {
    u32 tileVramBase;
    u32 mapVramBase;
    void *particles;
    s32 count;                  // 0x0c - active particles, 0-0x80
    void *tileBuffer;
};

extern void DrawStarfield(void *mgr);
extern void SpawnStar(void *mgr, s32 idx);

/* Calls `DrawStarfield(mgr)` (the OAM/tile-scan update this object's part
 * table drives), then - while the particle `count` is still under
 * 0x80 - spawns up to 8 more particles via `SpawnStar`, incrementing
 * `count` for each one spawned. */
void UpdateStarfield(void *mgrArg)
{
    struct particle_bg *mgr = mgrArg;

    DrawStarfield(mgr);

    if (mgr->count <= 0x7f) {
        s32 count = 0x80 - mgr->count;

        if (count > 8) {
            count = 8;
        }
        count -= 1;

        if (count != -1) {
            s32 end = -1;

            do {
                register s32 idxR0 asm("r0") = mgr->count;
                register s32 idx asm("r1") = idxR0;

                mgr->count = idxR0 + 1;
                SpawnStar(mgr, idx);
                count -= 1;
            } while (count != end);
        }
    }
}

extern void WaitForVBlank(void);
extern void *gInput;
extern void UpdateKeys(void *arg);
extern u16 gKeys[];

/* Busy-waits (yielding a frame via `WaitForVBlank`/`UpdateStarfield` each
 * time) until the input-poll result from `UpdateKeys(gInput)`
 * has either of bits 0/3 set in `gKeys`'s `+2` halfword. */
void StarfieldWaitForButton(void *mgrArg)
{
    u8 *mgr = mgrArg;
    s32 result;

    goto check;
body:
    WaitForVBlank();
    UpdateStarfield(mgr);
check:
    UpdateKeys(gInput);
    {
        register u8 *addr asm("r1") = (u8 *)gKeys;
        register s32 nine asm("r0") = 9;
        register s32 flag asm("r1");
        register s32 r asm("r0");

        flag = *(u16 *)(addr + 2);
        r = nine & flag;
        result = r;
    }
    if (result == 0) {
        goto body;
    }
}

extern void OperatorDeleteArray(void *ptr);
extern void OperatorDelete(void *self);

/* Releases `self+0x10`/`self+8`'s dynamically-allocated buffers (each,
 * if non-NULL, via `OperatorDeleteArray`) and, if bit 0 of `flags` is set, also
 * releases `self` itself via `OperatorDelete`. */
void DestroyStarfield(void *selfArg, s32 flags)
{
    u8 *self = selfArg;

    if (*(void **)(self + 0x10) != NULL) {
        OperatorDeleteArray(*(void **)(self + 0x10));
    }
    if (*(void **)(self + 8) != NULL) {
        OperatorDeleteArray(*(void **)(self + 8));
    }
    if ((flags & 1) != 0) {
        OperatorDelete(self);
    }
}

asm(".align 2, 0");
