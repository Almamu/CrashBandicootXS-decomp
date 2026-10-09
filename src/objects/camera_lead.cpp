/* The camera lead (CameraLead, gCameraLeadVtable; include/level_select.hpp):
 * the 0x80-byte moving sprite InputCtrl::StateStart (input_ctrl.cpp) spawns
 * for the input controller. It trails the player at a horizontal offset
 * that eases 2 px per frame toward a clamped target, and is gCamera's
 * follow target while alive. Its key method is here, so g++ emits its
 * vtable here (ldscript.txt places it).
 *
 * UNUSED - no `bl`/`.4byte` reference in asm/, expected/ or src/: SetUnk32,
 * SetOffset and GetOffset. Matched anyway.
 *
 * Split from the start of menus/level_select.cpp (GitHub issue #26) in
 * #767, with its flags: old_agbcp (Makefile OLD_AGBCC_OBJS), as its C was
 * old_agbcc, and -fno-implement-inlines (NO_IMPLEMENT_INLINES_OBJS). */

#include "level_select.hpp"
#include "audio.hpp"
#include "level_state.hpp"
#include "camera.hpp"

extern "C" {
#include <agb_syscall.h>
#include <libgcc.h>
#include "text.h"
#include "level.h"
#include "math_util.h"
}

/* The motion axes and the probe's hit axes, through an inline: both
 * values come first, and the second address is the first one's plus
 * 0x44, as in the ROM. */
static inline void SetAxes(MovingSprite *s, u8 dir, u8 hit)
{
    s->dir = dir;
    s->hitAxes = hit;
}

/* Sprite::ToggleHidden, inlined. */
static inline void ToggleHiddenNow(Sprite *s)
{
    s->f.b.blink = !s->f.b.blink;
}

/* UNUSED - no caller anywhere in the ROM (checked asm/, data/, src/ and a
 * whole-ROM Thumb-pointer scan). Sets the byte at 0x32, the third byte of
 * Sprite::frame, which nothing else touches on its own, so its meaning
 * (and this method's name) is open. */
void CameraLead::SetUnk32()
{
    ((u8 *)&frame)[2] = 1;
}

/* (Re)initializes the lead: makes it visible, registers it as gCamera's
 * follow target and snaps it 0x1E00 (30 px, Q8) to the player's right with
 * both offsets reset. */
void CameraLead::Reset()
{
    u32 v = f.bytes.flags2 >> 2;
    /* A u16 flag, as TryDoubleJump's `pressed`. Its AND is expanded as
     * a failed HImode AND: the 1 (r1) is an HImode pseudo the SImode
     * `and` reads through a subreg, so nothing ties the result to it and
     * the `and` is built in a copy of `v`; the toggle's bitfield store
     * keeps a QImode 1 of its own (`movs r0, #1`), as in the ROM. An
     * s32 or u8 flag shares one 1 between the two (#662 round 7, -da
     * dumps). */
    u16 hidden = v & 1;

    if (!hidden)
        ToggleHiddenNow(this);
    gCamera->target = this;
    {
        Player *p = gPlayer;
        s32 px = p->x;
        s32 py = p->y;
        s32 off = 0x1E00;

        x = px + off;
        y = py;
        f.b.active = 1;
        targetOffset = off;
        offset = off;
    }
}

/* Slot 3: the moving sprite's update, then eases `offset` toward
 * `targetOffset` by 0x200 per frame and follows the player. */
void CameraLead::Update()
{
    s32 cur, tgt;

    MovingSprite::Update();
    SetAxes(this, 1, 0);
    cur = offset;
    tgt = targetOffset;
    if (cur < tgt) {
        cur += 0x200;
        if (cur > tgt)
            offset = tgt;
        else
            offset = cur;
    } else if (cur > tgt) {
        cur -= 0x200;
        if (cur < tgt)
            offset = tgt;
        else
            offset = cur;
    }
    {
        Player *p = gPlayer;
        s32 px = p->x;
        s32 py = p->y;

        x = px + offset;
        y = py;
    }
    speedX = gPlayer->speedX;
}

/* Slot 10: hands gCamera's follow target back to the player. */
CameraLead::~CameraLead()
{
    gCamera->target = gPlayer;
}

/* The constructor, called from InputCtrl::StateStart (input_ctrl.cpp). */
CameraLead::CameraLead()
{
    Reset();
}

/* UNUSED - no caller anywhere in the ROM (checked asm/, data/, src/ and a
 * whole-ROM Thumb-pointer scan). Sets the target offset, clamped to
 * 0xA00-0x3200. */
void CameraLead::SetOffset(s32 v)
{
    if (v > 0x3200)
        v = 0x3200;
    else if (v <= 0x9FF)
        v = 0xA00;
    targetOffset = v;
}

/* UNUSED - no caller anywhere in the ROM (checked asm/, data/, src/ and a
 * whole-ROM Thumb-pointer scan). Returns the target offset. */
s32 CameraLead::GetOffset()
{
    return targetOffset;
}
