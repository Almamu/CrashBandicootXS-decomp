#include "core.h"
#include "actor.h"
#include "box_part.h"

/* GitHub issue #12: 0x0800D040-0x0800FC70, the physics/collision
 * subsystem documented in docs/rom_map.md ("Confirmed: a shared
 * physics/collision subsystem, entered from multiple different entity
 * types"). This first function of that subsystem sits right after
 * already-matched `game_loop` code - `sub_0800D18C` and `sub_800E08C`
 * immediately after it are two of the subsystem's largest, most
 * tangled functions and are left untouched for now; see
 * docs/matching/issue-12-physics-collision.md. */

extern void SetAabbPos(void *buf, s32 arg1, s32 arg2);
extern void SetAabbSize(void *buf, s32 arg1, s32 arg2);
extern u8 sub_8001688(void *buf1, void *buf2);
extern void *gPlayer;
#define gPlayerPart (*(struct box_part **)&gPlayer)
extern u8 gCrateKindExplosive[];
extern void ExplodeCrate(void *self, u8 arg1);
extern void BreakCrateInStack(void *self, u8 arg1, u8 arg2, u8 arg3);

/* Builds two AABBs - one for `self`, one for the player
 * (`gPlayer`) - from the shared "keyframe/hitbox record"
 * table convention already established by `sub_8007B00`/`sub_8007B98`
 * in actor_part.c (`self+0x20` -> a pointer-to-table, indexed by
 * `self+0x2d` at 0x1c/28-byte stride; here the {s16 xOff, s16 yOff, u8
 * w, u8 h} quad sits at the record's `+4`/`+6`/`+8`/`+9` instead of
 * `+0xc`/`+0xe`/`+0x10`/`+0x11`, the same "differently laid out"
 * variance `sub_8007B98`'s doc comment already flags). `self+0x28`
 * bits 4/5 mirror each box horizontally/vertically around its own
 * object's position, exactly like the `actor_part.c` pair. If the two
 * boxes overlap (`sub_8001688`), dispatches to `ExplodeCrate` or
 * `BreakCrateInStack` depending on a per-state-id lookup in
 * `gCrateKindExplosive`.
 *
 * Early-outs entirely when `self+0x4d & 0x7f == 1`.
 *
 * Real C (issue #12 NAKED retry, old_agbcc - this object is on the
 * Makefile's OLD_AGBCC_OBJS). The two boxes are one frame struct, and
 * `px`/`py` are shared by both blocks (the ROM keeps them in r7/r8 for
 * both). The ROM recomputes the player box's address (`add r0/r1, sp,
 * #16`) at each of its three uses; with plain `&f.b`, cse and gcse turn
 * them into one pseudo held in a callee-saved register across both
 * builder calls, which shifts px/py/&gPlayer up a register.
 * `BOX_ADDR` passes each use through an empty `asm("" : "+r")`, which
 * hides the value from cse, so each is its own single-use pseudo that
 * combine folds into the `add` right before the call. The first build's
 * x/y are computed first so that the `add r0, sp, #16` comes after them.
 * The same fix closed `sub_800CD00` (actor_part109.c); see
 * docs/matching/sp-box-retry.md. */

/* `a` through a copy that an empty asm claims to modify (emits nothing):
 * it hides the copy's value from cse, so each use of a stack box address
 * is its own pseudo instead of one held across calls. */
#define BOX_ADDR(a) ({ struct part_aabb *_p = (a); asm("" : "+r"(_p)); _p; })

void sub_800D040(struct box_part *self)
{
    struct {
        struct part_aabb a;
        struct part_aabb b;
    } f;
    s32 px;
    s32 py;

    if ((self->physMode & 0x7f) == 1)
        return;
    {
        u8 *rec;
        struct part_box *pb;
        s32 offX;
        s32 offY;
        u8 w;
        u8 h;

        rec = (u8 *)&(*self->keyframes)[self->frame];
        pb = (struct part_box *)(rec + 4);
        px = self->x >> 8;
        py = self->y >> 8;
        offX = pb->offX;
        offY = pb->offY;
        w = pb->w;
        h = pb->h;
        SetAabbPos(&f.a, offX + px, offY + py);
        SetAabbSize(&f.a, w, h);
        if (self->mirrorX)
            f.a.x = px * 2 - (f.a.x + f.a.w);
        if (self->mirrorY)
            f.a.y = py * 2 - (f.a.y + f.a.h);
    }
    {
        u8 *rec;
        struct part_box *pb;
        s32 offX;
        s32 offY;
        u8 w;
        u8 h;

        px = gPlayerPart->x >> 8;
        py = gPlayerPart->y >> 8;
        rec = (u8 *)&(*gPlayerPart->keyframes)[gPlayerPart->frame];
        pb = (struct part_box *)(rec + 4);
        offX = pb->offX;
        offY = pb->offY;
        w = pb->w;
        h = pb->h;
        {
            s32 x = offX + px, y = offY + py;
            SetAabbPos(BOX_ADDR(&f.b), x, y);
        }
        SetAabbSize(BOX_ADDR(&f.b), w, h);
        if (gPlayerPart->mirrorX)
            f.b.x = px * 2 - (f.b.x + f.b.w);
        if (gPlayerPart->mirrorY)
            f.b.y = py * 2 - (f.b.y + f.b.h);
    }
    if (sub_8001688(&f.a, BOX_ADDR(&f.b)))
    {
        if (gCrateKindExplosive[self->state] == 1)
            ExplodeCrate(self, 1);
        else
            BreakCrateInStack(self, 0, 0, 0);
    }
}

/* The object ends word-aligned with zero fill, as the ROM does. */
asm(".align 2, 0");
