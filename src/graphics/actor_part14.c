#include "core.h"
#include "actor.h"

extern s32 sub_8008350(void *arg0);
extern void *sub_8026EDC(s32 size);
extern struct actor *sub_8009F90(struct actor *part);
extern void sub_8009F1C(struct actor *self, u32 arg1);
extern u8 gStaticData_087E3D8C[];
extern void sub_800A664(void *selfArg);

/* Void tail-call wrapper around the already-matched `sub_8008350`. */
void sub_800A5F4(void *arg0)
{
    sub_8008350(arg0);
}

/* Constant-6 stub. */
s32 sub_800A600(void)
{
    return 6;
}

/* Same shape as `sub_8009ED0`/etc.: allocates a bigger (0x80-byte)
 * part-object, re-initializes it via `sub_8009F90`, overwrites its
 * table with `gStaticData_087E3D8C`, clears it via `sub_800A664`
 * below, then sets `field_08` and the Q8 `x`/`y` position from the
 * three `u16` arguments. */
struct actor *sub_800A604(u16 arg0, u16 arg1, u16 arg2)
{
    struct actor *part = sub_8026EDC(0x80);

    sub_8009F90(part);
    part->table = gStaticData_087E3D8C;
    sub_800A664(part);
    part->field_08 = arg0;
    part->x = (s32)arg1 << 8;
    part->y = (s32)arg2 << 8;
    return part;
}

/* Overwrites `self->table` with `gStaticData_087E3D8C`, then tail-
 * calls `sub_8009F1C` - which unconditionally overwrites `table`
 * again with `gStaticData_087E3D14` and fires its own trampoline, so
 * this function's own table write only matters transiently (read by
 * nothing before `sub_8009F1C` clobbers it). `unusedArg` is passed
 * straight through to `sub_8009F1C`'s own second parameter without
 * this function ever touching it itself - the ROM leaves it in
 * whatever register its own caller happened to leave it in. */
void sub_800A650(struct actor *self, u32 unusedArg)
{
    self->table = gStaticData_087E3D8C;
    sub_8009F1C(self, unusedArg);
}

/* Part-object field clearer/initializer, the `gStaticData_087E3D8C`-
 * table sibling of `sub_8009F50`'s own `gStaticData_087E3D14`-table
 * clearer: sets `flags` bits 6/7, zeroes the same velocity/accel/
 * max-velocity fields `sub_8009DF4` consumes (`+0x60`/`+0x64`/`+0x48`/
 * `+0x4c`/`+0x50`/`+0x54`/`+0x58`/`+0x5c`) plus `+0x24`/`+0x44`/
 * `+0x78`/`+0x1c`, sets `+0x68` to 8, and (unlike `sub_8009F50`) sets
 * `+0xd` bit 0 instead of clearing bit 3. */
void sub_800A664(void *selfArg)
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

/* Same `sub_8009F90`/table-swap/clearer shape as `sub_800A604` above,
 * but re-initializes an existing `self` instead of allocating a new
 * one - the same relationship `sub_8009F90` itself has to
 * `sub_800A604`. */
struct actor *sub_800A6A4(struct actor *self)
{
    sub_8009F90(self);
    self->table = gStaticData_087E3D8C;
    sub_800A664(self);
    return self;
}

/* `self+0xd` bit 1 get/set/clear accessors. */
u8 sub_800A6C4(void *selfArg)
{
    u8 *self = selfArg;
    return (self[0xd] >> 1) & 1;
}

void sub_800A6D0(void *selfArg)
{
    u8 *self = selfArg;
    register s32 mask asm("r1") = -3;
    register s32 byte asm("r2") = self[0xd];
    register s32 result asm("r1");

    result = mask & byte;
    self[0xd] = result;
}

void sub_800A6DC(void *selfArg)
{
    u8 *self = selfArg;
    register s32 mask asm("r1") = 2;
    register s32 byte asm("r2") = self[0xd];
    register s32 result asm("r1");

    result = mask | byte;
    self[0xd] = result;
}

/* `self+0xd` bit 0 get/set/clear accessors. */
u8 sub_800A6E8(void *selfArg)
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

void sub_800A6F4(void *selfArg)
{
    u8 *self = selfArg;
    register s32 mask asm("r1") = -2;
    register s32 byte asm("r2") = self[0xd];
    register s32 result asm("r1");

    result = mask & byte;
    self[0xd] = result;
}

void sub_800A700(void *selfArg)
{
    u8 *self = selfArg;
    register s32 mask asm("r1") = 1;
    register s32 byte asm("r2") = self[0xd];
    register s32 result asm("r1");

    result = mask | byte;
    self[0xd] = result;
}

/* `flags` bit 5 clear/set/get accessors. */
void sub_800A70C(void *selfArg)
{
    u8 *self = selfArg;
    register s32 mask asm("r1") = -0x21;
    register s32 byte asm("r2") = self[0xc];
    register s32 result asm("r1");

    result = mask & byte;
    self[0xc] = result;
}

void sub_800A718(void *selfArg)
{
    u8 *self = selfArg;
    register s32 mask asm("r1") = 0x20;
    register s32 byte asm("r2") = self[0xc];
    register s32 result asm("r1");

    result = mask | byte;
    self[0xc] = result;
}

u8 sub_800A724(void *selfArg)
{
    u8 *self = selfArg;
    return (self[0xc] >> 5) & 1;
}

/* `self+0x44` getter (the same "record" field `sub_8009F1C`/
 * `sub_8009FB0` fire their trampolines through). */
s32 sub_800A730(void *selfArg)
{
    return *(s32 *)((u8 *)selfArg + 0x44);
}
asm(".align 2, 0");
