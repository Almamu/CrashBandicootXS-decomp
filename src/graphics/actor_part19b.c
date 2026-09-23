#include "core.h"

/* Screen-space visibility test and OAM setup for one sprite frame:
 * derives the top-left corner from `self+0x1c`/`self+0x20` minus half
 * the frame's tile size, culls if fully off-screen, then builds the
 * OAM attribute words (position, `sub_803B060`'s flag byte, and a
 * priority/palette nibble from `self+0x18`/`self+0x14`) and calls
 * `SetupSpriteFrameOam`.
 *
 * The dead `flag = 0` initializer (materialized by the ROM as `movs
 * r0, #0` / `mov r8, r0` right after `frame` is obtained, even though
 * every reachable path to `flag`'s use overwrites it with `0x100`
 * first) closes via an opaque `asm volatile("mov r0, #0\n\tmov %0,
 * r0" : "=r"(flag) :: "r0")` two-instruction materialization - a
 * single-instruction `mov r8, #0` isn't valid Thumb (only lo registers
 * take an immediate `mov`), caught by the mandatory
 * `arm-none-eabi-as` assemble-verification step. Every other register
 * choice (the `w`/`wShift`/`h`/`hShift` load/shift order, the
 * `x`/`y`-position-word pack, the `self+0x18` priority-nibble unpack)
 * matches the ROM's own register roles exactly once pinned to match. */
extern u8 *GetAnimFrameData(void *self);
extern void SetupSpriteFrameOam(u8 *frame, u32 arg1, u32 arg2, s32 priority);
extern s32 sub_803B060(void *self);

void sub_802C2FC(void *selfArg)
{
    register u8 *self asm("r6") = selfArg;
    register s32 rawX asm("r0") = *(s32 *)(self + 0x1c);
    register s32 rawY asm("r1") = *(s32 *)(self + 0x20);
    register s32 x asm("r4") = rawX >> 8;
    register s32 y asm("r5") = rawY >> 8;
    u8 *frame;
    register u32 flag asm("r8");
    register u32 packed asm("r3");

    frame = GetAnimFrameData(self);
    asm volatile("mov r0, #0\n\tmov %0, r0" : "=r"(flag) :: "r0");

    {
        register s32 w asm("r0");
        register s32 wShift asm("r2");
        register s32 h asm("r1");
        register s32 hShift asm("r0");

        w = frame[0];
        wShift = w << 2;
        h = frame[1];
        hShift = h << 2;
        x -= wShift;
        y -= hShift;

        if (y > 0x9f) return;
        {
            register s32 hCheck asm("r0") = h << 3;
            if (y + hCheck < 0) return;
        }
        if (x > 0xef) return;
        {
            register s32 wCheck asm("r0") = wShift << 1;
            if (x + wCheck < 0) return;
        }

        flag = 0x100;
        {
            register s32 attrFlag asm("r0") = sub_803B060(self);
            register s32 a0 asm("r3") = 0xff;
            register s32 xm asm("r4") = x;

            a0 &= y;
            {
                register s32 mask asm("r1") = 0x1ff;
                register s32 shifted asm("r1");

                xm &= mask;
                shifted = xm << 16;
                a0 |= shifted;
            }
            a0 |= attrFlag;
            a0 |= flag;
            packed = a0;
        }
    }

    {
        register s32 field24 asm("r4") = *(s32 *)(self + 0x18);
        s32 a2 = field24 << 12;
        s32 field20 = *(s32 *)(self + 0x14);
        register u32 attr2 asm("r2");

        if (field20 & 0x8000) {
            a2 |= 0x800;
            {
                register s32 shifted asm("r0") = a2 << 16;
                attr2 = (u32)shifted >> 16;
            }
        } else {
            register s32 shifted asm("r0") = field24 << 28;
            attr2 = (u32)shifted >> 16;
        }

        {
            register u8 *argFrame asm("r0") = frame;
            register u32 argPacked asm("r1") = packed;
            register s32 argPriority asm("r3") = 0x140;

            SetupSpriteFrameOam(argFrame, argPacked, attr2, argPriority);
        }
    }
}

asm(".align 2, 0");
