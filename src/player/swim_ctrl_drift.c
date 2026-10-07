#include "core.h"
#include "match.h"
#include "gobj_1a794.h"
#include "player.h"

/* GitHub issue #19: continuation of action_ctrl.c's chunk
 * (0x08015840-0x08016128), non-adjacent since
 * `StartPlayerCtrlStroke`/`StartPlayerCtrlSpin`/`ApplyPlayerCtrlSwimDrift` (swim_ctrl_stroke.c)
 * sit between them - see docs/matching/archive/issue-19-0x08015840-actor.md.
 * This is the chunk's last matched function; `sub_8016046` right after
 * it in the ROM is disassembler-rendered padding (a zero halfword
 * between this function's 106-byte body and the next 4-byte-aligned
 * function, `CheckPlayerCtrlTurn` in swim_ctrl.c) - not real code, has no
 * caller or symbol reference anywhere in the tree, and is reproduced
 * by the build's end-of-object zero fill (the Makefile's ZERO_PAD_TEXT)
 * without needing its own C function (see docs/matching.md's
 * `GetEntityPixelY` entry for the established precedent: a disassembler-
 * rendered `movs r0, r0` at a function gap is usually just a zero-fill
 * halfword, not a literal instruction). The name survives only as the
 * frozen disassembly's label, dropped by expected/corrections.txt's
 * `unlabel sub_8016046`. */

/* Player-velocity-relative "record" writer: computes a Q14-ish rounded
 * `((player->speedY^2 / 0x4000) + 4) * 3 / 2` timing value, then compares
 * `|player->speedY|` against `|arg2|` to decide whether the current
 * `gPlayer` record (`rampY`, +0x54/+0x58/+0x5c) gets the computed
 * value or a product-sign-selected combination of `arg1`/the computed
 * value. */
void SetPlayerSwimDriftY(s32 arg0, s32 arg1arg, s32 arg2arg)
{
    MATCH_HOLD_REG(s32, self, r6) = arg0;
    MATCH_HOLD_REG(s32, arg1, ip) = arg1arg;
    MATCH_HOLD_REG(s32, arg2, r5) = arg2arg;
    MATCH_HOLD_REG(struct player *, player, r3) = gPlayer;
    MATCH_HOLD_REG(s32, vel, r4) = player->speedY;
    MATCH_HOLD_REG(s32, sq, r1) = vel;
    s32 result;

    sq = vel * sq;
    if (sq < 0) {
        MATCH_HOLD_REG(s32, bias, r0) = 0x3FFF;
        sq += bias;
    }
    {
        MATCH_HOLD_REG(s32, tmp, r0);

        sq >>= 0xe;
        sq += 4;
        tmp = (sq << 1) + sq;
        sq = (u32)tmp >> 0x1f;
        tmp += sq;
        result = tmp >> 1;
    }

    {
        MATCH_HOLD_REG(s32, signVel, r0) = vel >> 0x1f;
        MATCH_HOLD_REG(s32, absVel, r1) = vel;

        absVel = (absVel ^ signVel) - signVel;
        {
            MATCH_HOLD_REG(s32, signArg2, r2) = arg2 >> 0x1f;
            MATCH_HOLD_REG(s32, absArg2, r0) = arg2;

            absArg2 = (absArg2 ^ signArg2) - signArg2;

            if (absVel > absArg2) {
                player->rampY.start = self;
                player->rampY.step = result;
            } else {
                MATCH_HOLD_REG(s32, prod, r0) = vel;

                prod *= arg2;
                if (prod < 0) {
                    s32 sum = result + arg1;

                    player->rampY.start = self;
                    player->rampY.step = sum;
                } else {
                    player->rampY.start = self;
                    player->rampY.step = arg1;
                }
            }
        }
    }

    player->rampY.target = arg2;
}
