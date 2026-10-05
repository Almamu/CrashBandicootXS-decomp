#include "core.h"
#include "actor.h"
#include "gfx_part.h"
#include "objects.h"
#include "memory.h"

/* Void tail-call wrapper around the already-matched `DrawSpriteObj`. */
void DrawGroundSprite(void *arg0)
{
    DrawSpriteObj(arg0);
}

/* Constant-6 stub. */
s32 GetGroundSpriteClassId(void)
{
    return 6;
}

/* Same shape as `CreateMovingSprite`/etc.: allocates a bigger (0x80-byte)
 * part-object, re-initializes it via `InitMovingSprite`, overwrites its
 * table with `gGroundSpriteVtable`, clears it via `ResetGroundSprite`
 * below, then sets `field_08` and the Q8 `x`/`y` position from the
 * three `u16` arguments. */
void *CreateGroundSprite(u16 arg0, u16 arg1, u16 arg2, u16 unused)
{
    struct actor *part = OperatorNew(0x80);

    InitMovingSprite(part);
    part->table = (void *)gGroundSpriteVtable;
    ResetGroundSprite(part);
    part->field_08 = arg0;
    part->x = (s32)arg1 << 8;
    part->y = (s32)arg2 << 8;
    return part;
}

/* Overwrites `self->table` with `gGroundSpriteVtable`, then tail-
 * calls `DestroyMovingSprite` - which unconditionally overwrites `table`
 * again with `gMovingSpriteVtable` and fires its own trampoline, so
 * this function's own table write only matters transiently (read by
 * nothing before `DestroyMovingSprite` clobbers it). `unusedArg` is passed
 * straight through to `DestroyMovingSprite`'s own second parameter without
 * this function ever touching it itself - the ROM leaves it in
 * whatever register its own caller happened to leave it in. */
void DestroyGroundSprite(struct actor *self, u32 unusedArg)
{
    self->table = (void *)gGroundSpriteVtable;
    DestroyMovingSprite(self, unusedArg);
}

/* Part-object field clearer/initializer, the `gGroundSpriteVtable`-
 * table sibling of `ResetMovingSprite`'s own `gMovingSpriteVtable`-table
 * clearer: sets `flags` bits 6/7, zeroes the same velocity/accel/
 * max-velocity fields `ApplySpriteVelocity` consumes (`+0x60`/`+0x64`/`+0x48`/
 * `+0x4c`/`+0x50`/`+0x54`/`+0x58`/`+0x5c`) plus `+0x24`/`+0x44`/
 * `+0x78`/`+0x1c`, sets `+0x68` to 8, and (unlike `ResetMovingSprite`) sets
 * `+0xd` bit 0 instead of clearing bit 3. */
void ResetGroundSprite(void *selfArg)
{
    u8 *self = selfArg;

    {
        register s32 mask1 asm("r0") = 0x80;
        register s32 curFlags asm("r1") = self[0xc];
        register s32 combined asm("r0");

        combined = mask1 | curFlags;
        {
            register s32 mask2 asm("r1") = 0x40;
            register s32 result asm("r0");

            result = combined | mask2;
            self[0xc] = result;
        }
    }
    {
        register s32 zero asm("r0") = 0;

        *(s32 *)(self + 0x60) = zero;
        *(s32 *)(self + 0x64) = zero;
        *(s32 *)(self + 0x48) = zero;
        *(s32 *)(self + 0x4c) = zero;
        *(s32 *)(self + 0x50) = zero;
        *(s32 *)(self + 0x54) = zero;
        *(s32 *)(self + 0x58) = zero;
        *(s32 *)(self + 0x5c) = zero;
        {
            register u8 *addr68 asm("r2") = self + 0x68;
            register s32 eight asm("r1") = 8;

            *addr68 = eight;
        }
        {
            register u8 *addr24 asm("r1") = self + 0x24;

            *addr24 = zero;
        }
        *(s32 *)(self + 0x44) = zero;
        *(s32 *)(self + 0x78) = zero;
        *(s32 *)(self + 0x1c) = zero;
    }
    {
        register s32 mask asm("r0") = 1;
        register s32 byte asm("r1") = self[0xd];
        register s32 result asm("r0");

        result = mask | byte;
        self[0xd] = result;
    }
}

/* Same `InitMovingSprite`/table-swap/clearer shape as `CreateGroundSprite` above,
 * but re-initializes an existing `self` instead of allocating a new
 * one - the same relationship `InitMovingSprite` itself has to
 * `CreateGroundSprite`. */
struct actor *InitGroundSprite(struct actor *self)
{
    InitMovingSprite(self);
    self->table = (void *)gGroundSpriteVtable;
    ResetGroundSprite(self);
    return self;
}

/* `self+0xd` bit 1 get/set/clear accessors. */
u8 IsGroundSpriteGrounded(void *selfArg)
{
    u8 *self = selfArg;
    return (self[0xd] >> 1) & 1;
}

void ClearGroundSpriteGrounded(void *selfArg)
{
    u8 *self = selfArg;
    register s32 mask asm("r1") = -3;
    register s32 byte asm("r2") = self[0xd];
    register s32 result asm("r1");

    result = mask & byte;
    self[0xd] = result;
}

void SetGroundSpriteGrounded(void *selfArg)
{
    u8 *self = selfArg;
    register s32 mask asm("r1") = 2;
    register s32 byte asm("r2") = self[0xd];
    register s32 result asm("r1");

    result = mask | byte;
    self[0xd] = result;
}

/* `self+0xd` bit 0 get/set/clear accessors. */
u8 IsGroundSpriteFloorProbeEnabled(void *selfArg)
{
    register u8 *self asm("r1");
    register s32 mask asm("r0") = 1;
    register s32 byte asm("r1");
    register s32 result asm("r0");

    self = selfArg;
    byte = self[0xd];
    result = mask & byte;
    return result;
}

void DisableGroundSpriteFloorProbe(void *selfArg)
{
    u8 *self = selfArg;
    register s32 mask asm("r1") = -2;
    register s32 byte asm("r2") = self[0xd];
    register s32 result asm("r1");

    result = mask & byte;
    self[0xd] = result;
}

void EnableGroundSpriteFloorProbe(void *selfArg)
{
    u8 *self = selfArg;
    register s32 mask asm("r1") = 1;
    register s32 byte asm("r2") = self[0xd];
    register s32 result asm("r1");

    result = mask | byte;
    self[0xd] = result;
}

/* `flags` bit 5 clear/set/get accessors. */
void ClearSpriteObjFlag5(void *selfArg)
{
    u8 *self = selfArg;
    register s32 mask asm("r1") = -0x21;
    register s32 byte asm("r2") = self[0xc];
    register s32 result asm("r1");

    result = mask & byte;
    self[0xc] = result;
}

void SetSpriteObjFlag5(void *selfArg)
{
    u8 *self = selfArg;
    register s32 mask asm("r1") = 0x20;
    register s32 byte asm("r2") = self[0xc];
    register s32 result asm("r1");

    result = mask | byte;
    self[0xc] = result;
}

u8 GetSpriteObjFlag5(void *selfArg)
{
    u8 *self = selfArg;
    return (self[0xc] >> 5) & 1;
}

/* `ctrl` getter (the same "record" field `DestroyMovingSprite`/`UpdateMovingSprite`
 * fire their trampolines through). */
s32 GetMovingSpriteCtrl(void *selfArg)
{
    return (s32)((struct gfx_part *)selfArg)->ctrl;
}
asm(".align 2, 0");
