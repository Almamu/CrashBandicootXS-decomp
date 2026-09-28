#include "core.h"
#include "actor_self.h"

/* Sits right after actor_part19g.c's `sub_802C6C0` and before
 * actor_part19d.c's `sub_802C904` - directly adjacent to both now,
 * closing the raw gap issue #53 tracked. Same `self` object and
 * conventions documented in actor_part19g.c/actor_part74.c: the
 * 12-byte `{s16 x, y, z, sizeX, sizeY, sizeZ}` AABB record (per
 * docs/matching/issue-54-actor-d3a8.md's "Pinning down the 12-byte
 * AABB-record layout" section) and the shared "used"-state transition
 * idiom (`+0xc = 0x12`, `+0x10`/`+0x12` anim reset, `+8` accumulator
 * reset). */

extern struct actor_self *gUnknown_03000884;
extern void *gUnknown_030012BC;
extern void *gUnknown_030012C0;
extern void *sub_800014C(void *dst, const void *src, u32 byteCount);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void sub_8022FEC(void *self);

/* The actor_part103.c AABB helpers: the three scratch boxes live in
 * one frame struct so each box address is rematerialized from `sp`
 * (see that file and actor_part74.c). */
struct box16 {
    s16 x, y, z;
    s16 w, h, d;
};

static inline void BoxMove(struct box16 *b, s32 x, s32 y, s32 z)
{
    b->x += x;
    b->y += y;
    b->z += z;
}

static inline u8 BoxOverlap(struct box16 *b, struct box16 *a)
{
    if (b->z < a->z + a->d && b->z + b->d > a->z
        && b->y < a->y + a->h && b->y + b->h > a->y
        && b->x < a->x + a->w && b->x + b->w > a->x)
        goto hit;
    return 0;
hit:
    return 1;
}

static inline u8 ActorsOverlap(struct actor_self *pl, struct actor_self *self)
{
    struct {
        struct box16 a, t, s;
    } f;
    struct box16 *t;
    s32 x, y, z;

    f.t = *(struct box16 *)pl->unk_38;
    x = pl->x >> 8;
    y = pl->y >> 8;
    z = pl->z >> 8;
    t = &f.t;
    BoxMove(t, x, y, z);
    f.a = *t;
    sub_800014C(&f.a, &f.a, sizeof(f.a));
    f.s = *(struct box16 *)self->unk_38;
    BoxMove(&f.s, self->x >> 8, self->y >> 8, self->z >> 8);
    *t = f.s;
    sub_800014C(t, t, sizeof(*t));
    return BoxOverlap(&f.a, t);
}

/* `+0x4c`: next node of the circular actor list rooted at
 * `gUnknown_03000884`; `+0x30`: pointer whose first byte is the type. */
#define ACTOR_NEXT(a) (*(struct actor_self **)&(a)->unk_48[4])
#define ACTOR_TYPE(a) (**(u8 **)&(a)->unk_2C[4])

/* Called from `sub_802C6C0` (actor_part19g.c) once `self` (a "used"
 * pickup, state `0x12`) has stayed used for `self+0x44 == 0x14`
 * frames: walks the whole `self+0x4c`-rooted circular actor list
 * (rooted at `gUnknown_03000884`, the same sentinel-head list every
 * other `self+0x4c`/`self+0x48` teardown/unlink helper in this ROM
 * region walks - `sub_802AA4C`/`sub_802C19C`/`sub_802C394`) looking
 * for every OTHER actor whose type byte (`*(u8*)(*(u8**)(node+0x30))`,
 * the same type-byte indirection `sub_802C540` dispatches on) is `4`
 * and that overlaps `self`'s own translated `self+0x38` AABB (both
 * boxes translated into world space by each object's own `+0x1c`/
 * `+0x20`/`+0x24` `>>8` position, exactly like `sub_802D7B0`/
 * `sub_802DD9C`'s player-overlap test) - a proximity "chain pickup"
 * that fires the shared used-state transition (sound cue `PlaySfx(...,
 * 4, 0x100)`, lap-counter tie `sub_8022FEC`, `+0x44`/`+0x12`/`+8`
 * cleared, `+0xc = 0x12`, anim base reloaded from the node's own part
 * table `+0xd8`) on every type-4 node found overlapping, skipping
 * `self` itself and any node already in the used state. Every box's
 * `sub_800014C` call is the same confirmed no-op `memcpy(dst, dst,
 * 0xc)` self-copy documented in actor_part74.c - kept byte-faithful,
 * not simplified away.
 *
 * The old NAKED note blamed a function-lifetime `r7`; with the boxes
 * in one frame struct gcc hoists `sp+0x18` into `r7` itself. Needs
 * old_agbcc (25 halfwords off under current agbcc, all `asr`
 * scheduling in the box translation). */
void sub_802C7A8(struct actor_self *self)
{
    struct actor_self *n = ACTOR_NEXT(gUnknown_03000884);

    do {
        if (ACTOR_TYPE(n) == 4 && n != self && ActorsOverlap(self, n)
            && n->animIndex != 0x12) {
            PlaySfx(gUnknown_030012BC, 4, 0x100);
            sub_8022FEC(gUnknown_030012C0);
            n->stateTime = 0;
            n->animIndex = 0x12;
            n->animTimer = n->anims[0x12].duration;
            n->animDone = 0;
            n->animTime = 0;
        }
        n = ACTOR_NEXT(n);
    } while (n != gUnknown_03000884);
}

asm(".align 2, 0");
