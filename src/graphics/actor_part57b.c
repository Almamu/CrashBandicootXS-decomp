#include "core.h"

/* GitHub issue #19: continuation of actor_part57.c's chunk
 * (0x08015840-0x08016128), non-adjacent since the left-raw
 * `sub_80159F8`/`sub_8015C6C`/`sub_8015DF8` (asm/code_3_2_17_159f8.s)
 * sit between them - see docs/matching/issue-19-0x08015840-actor.md.
 * This is the chunk's last matched function; `sub_8016046` right after
 * it in the ROM is disassembler-rendered padding (a zero halfword
 * between this function's 106-byte body and the next 4-byte-aligned
 * function, `sub_8016048`, itself left raw) - not real code, has no
 * caller or symbol reference anywhere in the tree, and is reproduced
 * automatically by this file's own trailing `asm(".align 2, 0")`
 * without needing its own C function (see docs/matching.md's
 * `sub_8007364` entry for the established precedent: a disassembler-
 * rendered `movs r0, r0` at a function gap is usually just a zero-fill
 * halfword, not a literal instruction). `sub_8016048` continues in
 * asm/code_3_2_17_16048.s. */

extern void *gUnknown_030012D8;

/* Player-velocity-relative "record" writer: computes a Q14-ish rounded
 * `((player->0x64^2 / 0x4000) + 4) * 3 / 2` timing value, then compares
 * `|player->0x64|` against `|arg2|` to decide whether the current
 * `gUnknown_030012D8` record (`+0x54`/`+0x58`/`+0x5c`) gets the computed
 * value or a product-sign-selected combination of `arg1`/the computed
 * value. */
void sub_8015FDC(s32 arg0, s32 arg1arg, s32 arg2arg)
{
    register s32 self asm("r6") = arg0;
    register s32 arg1 asm("ip") = arg1arg;
    register s32 arg2 asm("r5") = arg2arg;
    register u8 *player asm("r3") = gUnknown_030012D8;
    register s32 vel asm("r4") = *(s32 *)(player + 0x64);
    register s32 sq asm("r1") = vel;
    s32 result;

    sq = vel * sq;
    if (sq < 0) {
        register s32 bias asm("r0") = 0x3FFF;
        sq += bias;
    }
    {
        register s32 tmp asm("r0");

        sq >>= 0xe;
        sq += 4;
        tmp = (sq << 1) + sq;
        sq = (u32)tmp >> 0x1f;
        tmp += sq;
        result = tmp >> 1;
    }

    {
        register s32 signVel asm("r0") = vel >> 0x1f;
        register s32 absVel asm("r1") = vel;

        absVel = (absVel ^ signVel) - signVel;
        {
            register s32 signArg2 asm("r2") = arg2 >> 0x1f;
            register s32 absArg2 asm("r0") = arg2;

            absArg2 = (absArg2 ^ signArg2) - signArg2;

            if (absVel > absArg2) {
                *(s32 *)(player + 0x54) = self;
                *(s32 *)(player + 0x58) = result;
            } else {
                register s32 prod asm("r0") = vel;

                prod *= arg2;
                if (prod < 0) {
                    s32 sum = result + arg1;

                    *(s32 *)(player + 0x54) = self;
                    *(s32 *)(player + 0x58) = sum;
                } else {
                    *(s32 *)(player + 0x54) = self;
                    *(s32 *)(player + 0x58) = arg1;
                }
            }
        }
    }

    *(s32 *)(player + 0x5c) = arg2;
}
asm(".align 2, 0");
