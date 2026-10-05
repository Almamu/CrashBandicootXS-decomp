#include "core.h"
#include "actor.h"

/* GitHub issue #9/#10, ROM 0x0800AB9C-0x0800AC2C (details in
 * docs/matching/issue-9-10-0x0800ab9c-graphics.md). Built with old_agbcc
 * (Makefile OLD_AGBCC_OBJS).
 *
 * A two-flag-gated teardown/notification step on the still-unnamed "big
 * object" (at least 0x110 bytes) actor_part15.c/actor_part77.c work on,
 * guarded by its +0x105 "torn down already" latch. */

struct aabb
{
    s32 x;
    s32 y;
    s32 w;
    s32 h;
};

/* sub_8007C30's result and the copy of it that goes to CollidePartList. As two
 * members of one frame object, the copy is addressed as a frame offset, so
 * its by-value words load straight from sp (a separate `struct aabb`
 * local's address is kept in a callee-saved register instead). */
struct aabb_copy
{
    struct aabb src;
    struct aabb copy;
};

struct ab9c_link
{
    u32 unk_0;
    u8 unk_4;
};

struct ab9c_obj
{
    u8 unk_00[0xC];
    u8 flags0C;            // 0x0C - bit 1: report the box, bit 7: tear down
    u8 unk_0D[0x17];
    u8 unk_24;             // 0x24
    u8 unk_25[0xE0];
    u8 cleared;            // 0x105
    u8 unk_106[2];
    struct ab9c_link link; // 0x108
};

extern void sub_8007C30(struct aabb *dest, void *obj);
extern void *MemCopy32(void *dest, void *src, s32 size);
extern void CollidePartList(void *manager, struct aabb box, s32 unused, void *compareViewport);
extern void CollidePlayerWithCrates(void *manager, s32 arg1);
extern void sub_8008D30(void *manager, s32 arg1);
extern void ResolvePlayerCollisions(void);
extern void *gCollidableList;
extern void *gCrateList;
extern void *gUnknown_030012EC;

/* Bit 1 of +0x0C: builds the object's AABB (sub_8007C30), copies it
 * (MemCopy32, a CpuSet memcpy) and hands the copy to CollidePartList by value
 * - three words in r1-r3, the fourth on the stack, which is what gives the
 * ROM's stack-argument order (6th, 7th, then the box's last word). Bit 7:
 * clears +0x108/+0x10C with the latch's 0 and fires three teardown
 * notifications (CollidePlayerWithCrates never reads its second argument; the ROM
 * still loads it). */
void CollidePlayerWithObjects(struct ab9c_obj *self)
{
    u32 cleared = self->cleared;

    if (cleared != 0)
        return;

    if ((self->flags0C >> 1) & 1)
    {
        struct aabb_copy b;
        void *manager;

        sub_8007C30(&b.src, self);
        manager = gCollidableList;
        MemCopy32(&b.copy, &b.src, sizeof(b.src));
        CollidePartList(manager, b.copy, self->unk_24, self);
    }

    if (self->flags0C >> 7)
    {
        struct ab9c_link *link = &self->link;

        link->unk_0 = cleared;
        link->unk_4 = cleared;
        CollidePlayerWithCrates(gCrateList, 3);
        sub_8008D30(gUnknown_030012EC, 4);
        ResolvePlayerCollisions();
    }
}
asm(".align 2, 0");
