#include "core.h"
#include "actor_self.h"
#include "actor_anim.h"

/* This cluster (`PolarIsTouchingPlayer`, `JetpackIsTouchingPlayer`, `RunActorCategoryFrame`, `FindShotTarget`,
 * ROM 0x0802A018-0x0802A4D4) sits inside the "actor" chunk starting at
 * `SetupActorVramPool` (0x080291A4). `PolarIsTouchingPlayer`/`JetpackIsTouchingPlayer` are
 * near-identical: translate `gActorList` (the player/list-sentinel
 * object)'s and `self`'s own 12-byte `{s16 x,y,z,sizeX,sizeY,sizeZ}` AABB
 * record (`self+0x38`, world-translated by `self+0x1c/0x20/0x24 >>8`) into
 * two stack scratch boxes via `MemCopy32` (a real, byte-verified
 * `memcpy(dst,dst,0xc)` self-copy - see src/graphics/actor_part74.c's own
 * definition/doc comment), then run the same 3-axis (Z,Y,X order) overlap
 * test already established throughout this ROM
 * (UpdateYeti/sub_802DD9C/DetonateNearbyPolarNitros/IsTouchingAirship etc - see
 * docs/matching/issue-53-actor-c7a8.md, issue-54-actor-d3a8.md,
 * issue-58-0x08030574-actor.md). `FindShotTarget` is the same test wrapped in
 * an outer walk of the whole `gActorList`-rooted circular list
 * (`self+0x4c`), gated by a `_call_via_r1` per-node visibility check first
 * (same shape as `DetonateNearbyPolarNitros`, actor_part19h.c).
 *
 * All three share the `ActorsOverlap` inline below. Its three boxes are
 * members of one frame struct (the actor_part74.c/actor_part81.c
 * pattern), so every box address is a fresh `add rX, sp, #off`; only
 * the pointer to the middle box stays live across both `MemCopy32`
 * calls (that is the ROM's `r4`, or `r7` once `FindShotTarget`'s loop
 * hoists it). The first actor's position is read into locals before
 * that pointer is taken, which puts its `add r4, sp, #0xc` after the
 * three loads. The old note blamed an unreachable `r7`; the file
 * simply needs old_agbcc (current agbcc schedules the `asr`s
 * differently, 24-26 halfwords off). `RunActorCategoryFrame` matches under
 * old_agbcc as well. */

extern struct actor_self *gActorList;
extern u8 gUnknown_030014A0;
extern u8 gUnknown_03001506;

extern void *MemCopy32(void *dst, const void *src, u32 byteCount);

/* Method slot 0x28 of the actor method table (`self+0x50`), which
 * `struct actor_vtable` still lumps into padding. */
struct actor_methods {
    u8 unk_00[0x28];
    struct actor_method m28; // "skip in overlap scans" query
};

typedef u8 (*actor_query_fn)(void *self);

/* `+0x4c`: next node of the circular actor list rooted at
 * `gActorList`. */
#define ACTOR_NEXT(a) (*(struct actor_self **)&(a)->unk_48[4])

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
    MemCopy32(&f.a, &f.a, sizeof(f.a));
    f.s = *(struct box16 *)self->unk_38;
    BoxMove(&f.s, self->x >> 8, self->y >> 8, self->z >> 8);
    *t = f.s;
    MemCopy32(t, t, sizeof(*t));
    return BoxOverlap(&f.a, t);
}

s32 PolarIsTouchingPlayer(struct actor_self *self)
{
    struct actor_self **plAddr = &gActorList;

    if (gUnknown_030014A0 != 0)
        return 0;
    return ActorsOverlap(*plAddr, self);
}

s32 JetpackIsTouchingPlayer(struct actor_self *self)
{
    struct actor_self **plAddr = &gActorList;

    if (gUnknown_03001506 != 0)
        return 0;
    return ActorsOverlap(*plAddr, self);
}

/* `RunActorCategoryFrame`: fires a "scroll enter/exit" trampoline pair off
 * `gActorCategoryVtable->fn[3]`/`fn[0xa]` (the selected category's vtable,
 * `struct category_vtable`), drives a `gActorSpawnTable`-rooted
 * sub-effect-table draw loop (`_call_via_r3`, same record family as
 * `sub_802A504`/`sub_802A51C`/`sub_802A540`/`sub_802A558`/`sub_802A570`),
 * then walks the whole `gActorList`-rooted circular actor list
 * twice: once unconditionally (drawing each node's own `self+0x50`
 * trampoline-record icon via `_call_via_r1`), once collecting every node
 * with `self+0x2c` set into `gActorDrawList` (drawing that filtered
 * set through `_call_via_r2` then a second `_call_via_r1` pass on a
 * different trampoline-record offset).
 *
 * The old note blamed a missing high register (a C draft needed `r9`).
 * That draft cached values across the sub-effect loop; the ROM is a
 * plain `while` whose exit test gcc copies ahead of the loop
 * (jump.c's duplicate_loop_exit_test), so the loop body starts at a
 * label and re-reads every global. The test must stay free of inline
 * functions (their block notes stop the copy), hence the macro, and
 * builds the "next record" address in the `sub_802A51C` order
 * (`off`, then `base + 0x14`, then the sum) through two locals.
 * Matches under old_agbcc, this file's compiler. */

extern struct category_vtable *gActorCategoryVtable;
extern s32 gUnknown_03001410;
extern s32 gUnknown_03001420;
extern struct sub_effect_record *gActorSpawnTable;
extern u8 gUnknown_0300141C;
extern s32 gActorSpawnIndex;
extern u8 gUnknown_03001414;
extern struct actor_self **gActorDrawList;
extern s32 gActorDrawCount;
extern void (*gHeapSortActorsByKeyFunc)(s32 count, struct actor_self **list);
extern s32 gUnknown_03001424;

extern void _call_via_r0(void *fn);
extern s32 sub_8029B2C(void);
extern s32 sub_8029B8C(void);

/* Method slots 0x10 ("draw") and 0x18 ("draw overlay") of the actor
 * method table. */
struct actor_draw_methods {
    u8 unk_00[0x10];
    struct actor_method m10;
    struct actor_method m18;
};

typedef void (*actor_draw_fn)(void *self);

/* "The next sub-effect record's threshold has scrolled into view":
 * `gActorSpawnTable[idx + 1].field_00 + gUnknown_03001420 <= scroll +
 * vtable slot 7` (read as a value), bounded by record 0's entry count. */
#define SUB_EFFECT_DUE()                                                       \
    (gActorSpawnIndex < gActorSpawnTable->field_04                           \
     && (off = gActorSpawnIndex * 0x14, tb = (u8 *)gActorSpawnTable + 0x14, \
         *(s32 *)(tb + off)) + gUnknown_03001420                              \
            <= scroll + (s32)gActorCategoryVtable->fn[7])

s32 RunActorCategoryFrame(void)
{
    s32 scroll;
    struct actor_self *n;
    s32 i;
    u8 *tb;
    s32 off;

    if (gActorCategoryVtable->fn[3] != NULL)
        _call_via_r0(gActorCategoryVtable->fn[3]);
    gUnknown_03001410 = 0;
    scroll = sub_8029B2C();
    if (scroll - gUnknown_03001420 > gActorSpawnTable->field_00)
        _call_via_r0(gActorCategoryVtable->fn[10]);
    if (gUnknown_0300141C != 0) {
        gUnknown_03001420 += sub_8029B8C();
    } else {
        while (SUB_EFFECT_DUE()) {
            ((void (*)(void *, s32, s32))gActorCategoryVtable->fn[1])(
                (u8 *)gActorSpawnTable + (gActorSpawnIndex * 0x14 + 8),
                gUnknown_03001414, gUnknown_03001420 << 8);
            gActorSpawnIndex++;
        }
    }

    n = gActorList;
    do {
        struct actor_self *next = ACTOR_NEXT(n);
        struct actor_draw_methods *vt = (struct actor_draw_methods *)n->vtable;

        ((actor_draw_fn)vt->m10.fn)((u8 *)n + vt->m10.thisOffset);
        n = next;
    } while (n != gActorList);

    gActorDrawCount = 0;
    n = gActorList;
    do {
        if (n->unk_2C[0] != 0)
            gActorDrawList[gActorDrawCount++] = n;
        n = ACTOR_NEXT(n);
    } while (n != gActorList);
    gHeapSortActorsByKeyFunc(gActorDrawCount, gActorDrawList);

    for (i = 0; i < gActorDrawCount; i++) {
        struct actor_self *a = gActorDrawList[i];
        struct actor_draw_methods *vt = (struct actor_draw_methods *)a->vtable;

        ((actor_draw_fn)vt->m18.fn)((u8 *)a + vt->m18.thisOffset);
    }
    gUnknown_03001424++;
    return gUnknown_03001410;
}

/* `FindShotTarget`: walks the whole `gActorList`-rooted circular
 * actor list (`self+0x4c`) looking for the first OTHER node
 * (`self`'s own arg0, held live in `r8` for the whole function) that
 * passes its method-table slot 0x28 query (false = not skipped) and
 * overlaps `self`'s translated 12-byte AABB, via the same
 * `ActorsOverlap` inline as `PolarIsTouchingPlayer`. Inside the loop gcc hoists
 * the third box's `sp+0x18` address into `r7` by itself. */
void *FindShotTarget(struct actor_self *self)
{
    struct actor_self *n = ACTOR_NEXT(gActorList);

    do {
        if (n != self) {
            struct actor_methods *vt = (struct actor_methods *)n->vtable;

            if (((actor_query_fn)vt->m28.fn)((u8 *)n + vt->m28.thisOffset) == 0
                && ActorsOverlap(self, n))
                return n;
        }
        n = ACTOR_NEXT(n);
    } while (n != gActorList);
    return 0;
}

asm(".align 2, 0");
