#include "player_ctrl.hpp"

extern "C" {
#include "globals.h"
}

/* GitHub issue #19: continuation of action_ctrl.cpp's chunk
 * (0x08015840-0x08016128), non-adjacent since
 * `StartStroke`/`StartSpin`/`ApplySwimDrift` (swim_ctrl_stroke.cpp)
 * sit between them - see docs/matching/archive/issue-19-0x08015840-actor.md.
 * This is the chunk's last matched function; `sub_8016046` right after
 * it in the ROM is disassembler-rendered padding (a zero halfword
 * between this function's 106-byte body and the next 4-byte-aligned
 * function, `CheckTurn` in swim_ctrl.cpp) - not real code, has no
 * caller or symbol reference anywhere in the tree, and is reproduced
 * by the build's end-of-object zero fill (the Makefile's ZERO_PAD_TEXT)
 * without needing its own C function (see docs/matching.md's
 * `GetEntityPixelY` entry for the established precedent: a disassembler-
 * rendered `movs r0, r0` at a function gap is usually just a zero-fill
 * halfword, not a literal instruction). The name survives only as the
 * frozen disassembly's label, dropped by expected/corrections.txt's
 * `unlabel sub_8016046`. */

/* The player's Y drift ramp (`rampY`), like SetDriftX (swim_ctrl.cpp) for
 * X: the step is `(speedY^2 / 0x4000 + 4) * 3 / 2` when the player is
 * faster than `target`, `step` plus that when it moves against `target`,
 * and `step` otherwise. */
void PlayerCtrl::SetDriftY(s32 start, s32 step, s32 target)
{
    struct player *p = gPlayer;
    s32 v = p->speedY;
    s32 t = (v * v / 0x4000 + 4) * 3 / 2;
    s32 signV;
    s32 signC;
    s32 absV;
    s32 absC;

    signV = v >> 31;
    absV = (v ^ signV) - signV;
    signC = target >> 31;
    absC = (target ^ signC) - signC;
    if (absV > absC) {
        p->rampY.start = start;
        p->rampY.step = t;
    } else if (v * target < 0) {
        s32 sum = t + step;

        p->rampY.start = start;
        p->rampY.step = sum;
    } else {
        p->rampY.start = start;
        p->rampY.step = step;
    }
    p->rampY.target = target;
}
