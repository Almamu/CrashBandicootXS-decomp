#include "core.h"
#include "actor.h"

/* GitHub issue #12: 0x0800D040-0x0800FC70, the physics/collision
 * subsystem documented in docs/rom_map.md ("Confirmed: a shared
 * physics/collision subsystem, entered from multiple different entity
 * types"). This first function of that subsystem sits right after
 * already-matched `game_loop` code - `sub_0800D18C` and `sub_800E08C`
 * immediately after it are two of the subsystem's largest, most
 * tangled functions and are left untouched for now; see
 * docs/matching/issue-12-physics-collision.md. */

struct aabb {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_c;
};

extern void sub_803AFE4(void *buf, s32 arg1, s32 arg2);
extern void sub_803AFDC(void *buf, s32 arg1, s32 arg2);
extern u8 sub_8001688(void *buf1, void *buf2);
extern void *gUnknown_030012D8;
extern u8 gStaticData_0816BBC4[];
extern void sub_800EEF0(void *self, u8 arg1);
extern void sub_800E7A8(void *self, u8 arg1, u8 arg2, u8 arg3);

#if NON_MATCHING
/* Builds two AABBs - one for `self`, one for the player
 * (`gUnknown_030012D8`) - from the shared "keyframe/hitbox record"
 * table convention already established by `sub_8007B00`/`sub_8007B98`
 * in actor_part.c (`self+0x20` -> a pointer-to-table, indexed by
 * `self+0x2d` at 0x1c/28-byte stride; here the {s16 xOff, s16 yOff, u8
 * w, u8 h} quad sits at the record's `+4`/`+6`/`+8`/`+9` instead of
 * `+0xc`/`+0xe`/`+0x10`/`+0x11`, the same "differently laid out"
 * variance `sub_8007B98`'s doc comment already flags). `self+0x28`
 * bits 4/5 mirror each box horizontally/vertically around its own
 * object's position, exactly like the `actor_part.c` pair. If the two
 * boxes overlap (`sub_8001688`), dispatches to `sub_800EEF0` or
 * `sub_800E7A8` depending on a per-state-id lookup in
 * `gStaticData_0816BBC4`.
 *
 * Early-outs entirely when `self+0x4d & 0x7f == 1`.
 *
 * PARKED, NOT BYTE-MATCHING: this is the same AABB-build primitive as
 * the already-parked `sub_8007B98` (see actor_part.c), just inlined
 * twice (once for `self`, once for the player) instead of called as a
 * subroutine, plus the overlap dispatch tail. `sub_8007B98`'s own doc
 * comment already documents this exact shape resisting byte-exact
 * register allocation even in isolation ("about 10 of ~73
 * instructions... which anonymous scratch register" gaps); doing it
 * twice in a row compounds the problem rather than cancelling it out.
 * Concretely: the ROM keeps exactly two extra callee-saved registers
 * live across both AABB builds (`r8` and `sb`, the latter holding
 * `&gUnknown_030012D8` so the player pointer can be cheaply reloaded
 * after the `sub_803AFE4`/`sub_803AFDC` calls clobber it), and reuses
 * `r7`/`r8` for the X/Y "shift" values across *both* the self-block and
 * the player-block. Every reconstruction tried here (explicit
 * `xShift`/`yShift` locals reused across both blocks, a `vu8` volatile
 * cast on the second `self+0x28` bit-test in each block to block gcc's
 * CSE the same way `sub_8007B98` needed it, hoisting/flattening the
 * player-box locals in and out of a nested scope) always lands on
 * *three* extra callee-saved registers (`r8`/`r9`/`sl` in every variant
 * tried) instead of the ROM's two, and/or moves `self` itself out of
 * `r6` into `r8`. Parked rather than keep chasing individual register
 * letters - see docs/matching/issue-12-physics-collision.md. */
void sub_800D040(void *self)
{
    struct aabb buf_;
    s32 *buf = (s32 *)&buf_;
    void *table;
    u8 idx;
    void *rec;
    s16 offX, offY;
    u8 w, h;
    s32 x, y;
    s32 xShift, yShift;

    if ((*((u8 *)self + 0x4d) & 0x7f) == 1) {
        return;
    }

    {
        void **tablePtr = *(void ***)((u8 *)self + 0x20);
        s32 offset;

        idx = *((u8 *)self + 0x2d);
        offset = idx * 0x1c;
        table = *tablePtr;
        rec = (u8 *)table + offset;
    }
    offX = *(s16 *)((u8 *)rec + 4);
    offY = *(s16 *)((u8 *)rec + 6);
    w = *((u8 *)rec + 8);
    h = *((u8 *)rec + 9);

    xShift = *(s32 *)self >> 8;
    yShift = *(s32 *)((u8 *)self + 4) >> 8;

    x = offX + xShift;
    y = offY + yShift;
    sub_803AFE4(buf, x, y);
    sub_803AFDC(buf, w, h);

    {
        u8 flags = *((u8 *)self + 0x28);
        if ((s32)(flags << 27) < 0) {
            buf[0] = xShift * 2 - (buf[0] + buf[2]);
        }
    }
    {
        u8 flags = *(vu8 *)((u8 *)self + 0x28);
        if ((s32)(flags << 26) < 0) {
            buf[1] = yShift * 2 - (buf[1] + buf[3]);
        }
    }

    {
        void *player = gUnknown_030012D8;
        struct aabb buf2_;
        s32 *buf2 = (s32 *)&buf2_;

        {
            void **tablePtr = *(void ***)((u8 *)player + 0x20);
            s32 offset;

            idx = *((u8 *)player + 0x2d);
            offset = idx * 0x1c;
            table = *tablePtr;
            rec = (u8 *)table + offset;
        }
        offX = *(s16 *)((u8 *)rec + 4);
        offY = *(s16 *)((u8 *)rec + 6);
        w = *((u8 *)rec + 8);
        h = *((u8 *)rec + 9);

        xShift = *(s32 *)player >> 8;
        yShift = *(s32 *)((u8 *)player + 4) >> 8;

        x = offX + xShift;
        y = offY + yShift;
        sub_803AFE4(buf2, x, y);
        sub_803AFDC(buf2, w, h);

        {
            u8 flags = *((u8 *)gUnknown_030012D8 + 0x28);
            if ((s32)(flags << 27) < 0) {
                buf2[0] = xShift * 2 - (buf2[0] + buf2[2]);
            }
        }
        {
            u8 flags = *(vu8 *)((u8 *)gUnknown_030012D8 + 0x28);
            if ((s32)(flags << 26) < 0) {
                buf2[1] = yShift * 2 - (buf2[1] + buf2[3]);
            }
        }

        if (sub_8001688(buf, buf2)) {
            if (gStaticData_0816BBC4[*((u8 *)self + 0x4e)] == 1) {
                sub_800EEF0(self, 1);
            } else {
                sub_800E7A8(self, 0, 0, 0);
            }
        }
    }
}
#endif /* NON_MATCHING */
asm(".align 2, 0");
