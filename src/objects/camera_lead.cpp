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

extern "C" {
#include "match.h"
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
    /* One pin kept (the C had five): the ROM tests `blink` with a 1 of its
     * own in r1 and loads another for ToggleHidden's; unpinned, gcc shares
     * one constant between the two, with every spelling of the test and
     * the toggle tried (u8/s32/bool tests, a switch, `^ 1`, `== 0`; in
     * round 2 also IsHidden-style inline helpers returning s32 or bool,
     * the bitfield test and `blink = 1`).
     * #662 round 3, from the -da dumps: the toggle's 1 is a QImode
     * pseudo of its own (the bitfield store), so nothing shares it; the
     * test's 1 dies at the `and`, and regmove/reload then build the
     * `and` in its register (`movs r0, #1; ands r0, r1`). The ROM's
     * `adds r0, r2, #0; ands r0, r1` is the `and` built in a copy of `v`,
     * which reload only does when the 1 is still live after it. A literal
     * 1 is folded into `(v ^ 1) & 1` and shared with the toggle; u8, u16,
     * s8, s16 and bool for `one` or `v`, a `hidden = v & 1` local and the
     * flag sweep (-fno-regmove included) don't give the copy. */
    MATCH_HOLD_REG(u32, one, r1) = 1;

    if (!(v & one))
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
