#include "core.h"
#include "math_util.h"
#include "actor_self.h"
#include "actor_anim.h"
#include "system.h"
#include "audio.h"
#include "vehicle.h"
#include "level.h"
#include "globals.h"

/* Sits right after polar_pickups.c's `UpdatePolarNitroCrate` and before
 * polar_crates.c's `UpdatePolarAkuAkuCrate` - directly adjacent to both now,
 * closing the raw gap issue #53 tracked. Same `self` object and
 * conventions documented in polar_pickups.c/yeti_update.c: the
 * 12-byte `{s16 x, y, z, sizeX, sizeY, sizeZ}` AABB record (per
 * docs/matching/archive/issue-54-actor-d3a8.md's "Pinning down the 12-byte
 * AABB-record layout" section) and the shared "used"-state transition
 * idiom (`+0xc = 0x12`, `+0x10`/`+0x12` anim reset, `+8` accumulator
 * reset). */

/* The actor_category_frame.c AABB helpers: the three scratch boxes live in
 * one frame struct so each box address is rematerialized from `sp`
 * (see that file and yeti_update.c). */
static inline void BoxMove(struct anim_box *b, s32 x, s32 y, s32 z)
{
    b->x += x;
    b->y += y;
    b->z += z;
}

static inline u8 BoxOverlap(struct anim_box *b, struct anim_box *a)
{
    if (b->z < a->z + a->d && b->z + b->d > a->z && b->y < a->y + a->h && b->y + b->h > a->y &&
        b->x < a->x + a->w && b->x + b->w > a->x)
        goto hit;
    return 0;
hit:
    return 1;
}

static inline u8 ActorsOverlap(struct actor_self *pl, struct actor_self *self)
{
    struct {
        struct anim_box a, t, s;
    } f;
    struct anim_box *t;
    s32 x, y, z;

    f.t = *(struct anim_box *)pl->box;
    x = Q8_TO_INT(pl->x);
    y = Q8_TO_INT(pl->y);
    z = Q8_TO_INT(pl->z);
    t = &f.t;
    BoxMove(t, x, y, z);
    f.a = *t;
    MemCopy32(&f.a, &f.a, sizeof(f.a));
    f.s = *(struct anim_box *)self->box;
    BoxMove(&f.s, Q8_TO_INT(self->x), Q8_TO_INT(self->y), Q8_TO_INT(self->z));
    *t = f.s;
    MemCopy32(t, t, sizeof(*t));
    return BoxOverlap(&f.a, t);
}

/* `+0x30`: pointer whose first byte is the type. */
#define ACTOR_TYPE(a) (**(u8 **)&(a)->record)

/* Called from `UpdatePolarNitroCrate` (polar_pickups.c) once `self` (a "used"
 * pickup, state `0x12`) has stayed used for `self+0x44 == 0x14`
 * frames: walks the whole `self+0x4c`-rooted circular actor list
 * (rooted at `gActorList`, the same sentinel-head list every
 * other `self+0x4c`/`self+0x48` teardown/unlink helper in this ROM
 * region walks - `IsActorVisible`/`DestroyPolarPlayer`/`DestroyPolarCollectedWumpa`) looking
 * for every OTHER actor whose type byte (`*(u8*)(*(u8**)(node+0x30))`,
 * the same type-byte indirection `UpdatePolarQuestionCrate` dispatches on) is `4`
 * and that overlaps `self`'s own translated `self+0x38` AABB (both
 * boxes translated into world space by each object's own `+0x1c`/
 * `+0x20`/`+0x24` `>>8` position, exactly like `UpdateYeti`/
 * `IsTouchingYeti`'s player-overlap test) - a proximity "chain pickup"
 * that fires the shared used-state transition (sound cue `PlaySfx(...,
 * 4, 0x100)`, lap-counter tie `AddBrokenCrate`, `+0x44`/`+0x12`/`+8`
 * cleared, `+0xc = 0x12`, anim base reloaded from the node's own part
 * table `+0xd8`) on every type-4 node found overlapping, skipping
 * `self` itself and any node already in the used state. Every box's
 * `MemCopy32` call is the same confirmed no-op `memcpy(dst, dst,
 * 0xc)` self-copy documented in yeti_update.c - kept byte-faithful,
 * not simplified away.
 *
 * The old NAKED note blamed a function-lifetime `r7`; with the boxes
 * in one frame struct gcc hoists `sp+0x18` into `r7` itself. Needs
 * old_agbcc (25 halfwords off under current agbcc, all `asr`
 * scheduling in the box translation). */
void DetonateNearbyPolarNitros(struct actor_self *self)
{
    struct actor_self *n = ACTOR_LINK_NEXT(gActorList);

    do {
        if (ACTOR_TYPE(n) == 4 && n != self && ActorsOverlap(self, n) && n->animIndex != 0x12) {
            PlaySfx(gAudioContext, SFX_EXPLOSION, 0x100);
            AddBrokenCrate(gLevelState);
            n->stateTime = 0;
            n->animIndex = 0x12;
            n->animTimer = n->anims[0x12].duration;
            n->animDone = 0;
            n->animTime = 0;
        }
        n = ACTOR_LINK_NEXT(n);
    } while (n != gActorList);
}
