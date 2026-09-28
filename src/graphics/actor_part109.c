#include "core.h"

/* GitHub issue #9/#10: 0x0800CD00, `sub_800AAEC`'s (`actor_part108.c`)
 * only caller/callee companion - `sub_800AAEC` calls this once per
 * `gUnknown_0300130C` list entry whose own `+0x18`-table `+0x48`
 * trampoline (`sub_803AD7C`) reports state `3`, passing that entry as
 * `self` and its own `x` (the target "action" index) straight through.
 *
 * Early-outs (returns `0`) when `self+0x4e` (a state/type byte, the
 * same offset `docs/rom_map.md`'s "eight more core reads" investigation
 * already reads as an object's own state byte elsewhere in this ROM
 * region) is `5` or `0xa`.
 *
 * Otherwise builds THREE AABBs via the shared `sub_803AFE4`(set-pos)/
 * `sub_803AFDC`(set-size) primitive (`struct aabb` from
 * `actor_part.c`/`src/system/game_loop6.c`), all from the same
 * "keyframe/hitbox record" table convention documented at length in
 * `game_loop6.c`'s own `sub_800D040` header comment: `+0x20` is a
 * pointer-to-table, indexed by a `+0x2d` tag byte at 28-byte stride
 * (`docs/rom_map.md`'s own cross-reference from this exact function:
 * "matching `gStaticData_0816BC98`'s stride exactly, but clearly a
 * different table instance" - reinforcing the project's established
 * "shared convention, not shared struct" reading, since `self` here is
 * a plain `gUnknown_0300130C` list entry, not the physics subsystem's
 * own object type), with the record's own `{s16 offX, s16 offY, u8 w,
 * u8 h}` quad at `+4`/`+6`/`+8`/`+9` this time (yet another layout
 * variant of the same convention, alongside `actor_part.c`'s
 * `+0xc`/`+0xe`/`+0x10`/`+0x11` and `game_loop6.c`'s own `+4`/`+6`/
 * `+8`/`+9`, which this function's first two AABBs match exactly).
 * Each AABB is mirrored horizontally/vertically around its own
 * object's integer position when that object's own `+0x28` bits 4/5
 * (the mirror-flag convention `actor_part16.c`/`actor_part17.c`/
 * `game_loop6.c` all already read) are set:
 *
 *   - AABB1: from `self`'s own `+0x20`-table, indexed by `self`'s own
 *     `+0x2d` tag - `self`'s current hitbox.
 *   - AABB2: from the player's (`gUnknown_030012D8`) own `+0x20`-table,
 *     indexed by the PLAYER's own `+0x2d` tag - the player's current
 *     hitbox.
 *   - AABB3 (reuses AABB2's stack slot): from the player's `+0x20`-
 *     table again, but indexed by `x` (this function's own second
 *     argument, the caller's target action index) instead of the
 *     player's `+0x2d` - the player's hitbox FOR the target action.
 *
 * If AABB1 overlaps AABB2 (`sub_8001640`, the inclusive/touching-counts
 * variant - `src/graphics/aabb_util.c`), bails out and returns `0`
 * immediately: `self`'s hitbox already overlaps the player's CURRENT
 * hitbox, so this is not a fresh trigger. Otherwise, returns `1` only
 * if AABB1 overlaps AABB3 - `self`'s hitbox overlaps the player's
 * hitbox for the action `x` the caller is testing. Read together with
 * `sub_800AAEC`, this is a "would performing action `x` right now hit
 * `self`, given the player isn't already touching it in its current
 * pose" gate - consistent with `sub_800AAEC`'s own role gating the
 * 42-slot action-dispatch table's action codes `0xB`/`0x10`.
 *
 * Real C (issue #11 NAKED retry, old_agbcc - so this object is on the
 * Makefile's OLD_AGBCC_OBJS). The two stack boxes are one frame struct.
 * The ROM recomputes the player box's address (`add r0, sp, #16`) for
 * each of its first two builder calls and only holds it in r6 from the
 * first overlap test on. With plain `&f.b` everywhere, cse and gcse
 * turn every `&f.b` into one pseudo that lives in r6 from the first
 * build on. `BOX_ADDR` below passes each of those three addresses
 * through an empty `asm("" : "+r")`: the asm "modifies" the copy, so cse
 * drops its equivalence with `sp + 16` and the next `&f.b` gets a new
 * pseudo. A pseudo used once as a call argument is folded into the
 * `add r0, sp, #16` right before the `bl`, and `pb` is born at the
 * overlap test as in the ROM. The first build's x/y are computed before
 * the call so that the `add r0, sp, #16` comes after them. `rec` is
 * shared by the first two blocks: a function-scope `rec` is not
 * block-local, so local-alloc can't tie it to the record base and it
 * lands in r1 as in the ROM. See docs/matching/sp-box-retry.md. */
#include "box_part.h"

extern void sub_803AFE4(struct part_aabb *buf, s32 x, s32 y);
extern void sub_803AFDC(struct part_aabb *buf, s32 w, s32 h);
extern u8 sub_8001640(struct part_aabb *a, struct part_aabb *b);
extern struct box_part *gUnknown_030012D8;

/* `a` through a copy that an empty asm claims to modify (emits nothing):
 * it hides the copy's value from cse, so each use of a stack box address
 * is its own pseudo instead of one held across calls. */
#define BOX_ADDR(a) ({ struct part_aabb *_p = (a); asm("" : "+r"(_p)); _p; })

u8 sub_800CD00(struct box_part *self, s32 action)
{
    struct {
        struct part_aabb a;
        struct part_aabb b;
    } f;
    struct part_aabb *pb;
    s32 px, py;
    u8 *rec;
    u8 state = self->state;

    if (state == 5 || state == 0xa)
        return 0;
    {
        struct part_box *q;
        s32 offX, offY;
        u8 w, h;

        rec = (u8 *)&(*self->keyframes)[self->frame];
        q = (struct part_box *)(rec + 4);

        px = self->x >> 8;
        py = self->y >> 8;
        offX = q->offX;
        offY = q->offY;
        w = q->w;
        h = q->h;
        sub_803AFE4(&f.a, offX + px, offY + py);
        sub_803AFDC(&f.a, w, h);
        if (self->mirrorX)
            f.a.x = px * 2 - (f.a.x + f.a.w);
        if (self->mirrorY)
            f.a.y = py * 2 - (f.a.y + f.a.h);
    }
    {
        struct box_part *pl = gUnknown_030012D8;
        struct part_box *q;
        s32 offX, offY;
        u8 w, h;

        px = pl->x >> 8;
        py = pl->y >> 8;
        rec = (u8 *)&(*pl->keyframes)[pl->frame];
        q = (struct part_box *)(rec + 4);
        offX = q->offX;
        offY = q->offY;
        w = q->w;
        h = q->h;
        {
            s32 x = offX + px, y = offY + py;
            sub_803AFE4(BOX_ADDR(&f.b), x, y);
        }
        sub_803AFDC(BOX_ADDR(&f.b), w, h);
        if (gUnknown_030012D8->mirrorX)
            f.b.x = px * 2 - (f.b.x + f.b.w);
        if (gUnknown_030012D8->mirrorY)
            f.b.y = py * 2 - (f.b.y + f.b.h);
    }
    pb = BOX_ADDR(&f.b);
    if (sub_8001640(&f.a, pb))
        return 0;
    {
        u8 *rec = (u8 *)&(*gUnknown_030012D8->keyframes)[action];
        struct part_box *q = (struct part_box *)(rec + 4);
        s32 offX, offY;
        u8 w, h;

        offX = q->offX;
        offY = q->offY;
        w = q->w;
        h = q->h;
        sub_803AFE4(pb, offX + px, offY + py);
        sub_803AFDC(pb, w, h);
        if (gUnknown_030012D8->mirrorX)
            f.b.x = px * 2 - (f.b.x + f.b.w);
        if (gUnknown_030012D8->mirrorY)
            f.b.y = py * 2 - (f.b.y + f.b.h);
    }
    if (sub_8001640(&f.a, pb) != 1)
        return 0;
    return 1;
}

/* The object ends word-aligned with zero fill, as the ROM does. */
asm(".align 2, 0");
