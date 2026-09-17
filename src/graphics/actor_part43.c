#include "core.h"

/* Same large per-instance "self" object family as actor_part17.c/
 * actor_part18.c/actor_part19.c/actor_part20.c (state at `self+0x28`,
 * table-index at `self+0xc`, an anim-frame halfword/byte pair at
 * `self+0x10`/`self+0x12`, an accumulator at `self+8`, a "part table"
 * pointer at `self+0`), part of a second boss-weapon "spawn/pre-
 * attack" singleton whose own flags/counters live at
 * `gUnknown_030014E0`-`gUnknown_03001518` - a different singleton
 * cluster than issue #58's `gUnknown_03001534` one and issue #62's
 * `gUnknown_030015AC` one. See docs/matching/issue-56-0x0802f0dc-actor.md
 * and docs/status/actor.md. */

extern void sub_8029BAC(s32 arg0);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void sub_8022EA8(void *arg0, s32 arg1);
extern void *gUnknown_030012BC;
extern void *gUnknown_030012C0;
extern u8 gUnknown_03001504;
extern u8 gUnknown_03001506;
extern u8 gUnknown_03001507;
extern s32 gUnknown_03001508;
extern s32 gUnknown_0300150C;

/* Constructor/reset: while the singleton flag (`gUnknown_03001506`) is
 * off, resets `self` to state 5/table-index 4 (idle-ish), plays a cue,
 * and - only if the current game-mode flag at `gUnknown_030012C0+0x8c`
 * is set - fires an extra one-shot effect via `sub_8022EA8`. */
void sub_802F0DC(void *selfArg)
{
    register u8 *self asm("r4") = selfArg;
    register s32 zero asm("r5") = gUnknown_03001506;

    if (zero == 0) {
        gUnknown_03001507 = zero;
        gUnknown_03001504 = 1;
        gUnknown_03001506 = 1;
        sub_8029BAC(0x3c);
        gUnknown_03001508 = zero;
        gUnknown_0300150C = zero;
        {
            register s32 five asm("r0") = 5;
            register s32 four asm("r1") = 4;

            *(s32 *)(self + 0x28) = five;
            *(s32 *)(self + 0x44) = zero;
            *(s32 *)(self + 0xc) = four;
        }
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0x30);
            register u8 zero2 asm("r1") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero2;
        }
        *(s32 *)(self + 8) = zero;
        PlaySfx(gUnknown_030012BC, 0x3b, 0x100);
        if (*((u8 *)gUnknown_030012C0 + 0x8c) != 0) {
            sub_8022EA8(gUnknown_030012C0, 0x2710);
        }
    }
}
