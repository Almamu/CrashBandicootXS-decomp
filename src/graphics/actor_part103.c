#include "core.h"
#include "actor_self.h"

ACTOR_CALL_VIA_ALIASES

/* This cluster (`sub_802A018`, `sub_802A110`, `sub_802A208`, `sub_802A3AC`,
 * ROM 0x0802A018-0x0802A4D4) sits inside the "actor" chunk starting at
 * `SetupActorVramPool` (0x080291A4). `sub_802A018`/`sub_802A110` are
 * near-identical: translate `gUnknown_03000884` (the player/list-sentinel
 * object)'s and `self`'s own 12-byte `{s16 x,y,z,sizeX,sizeY,sizeZ}` AABB
 * record (`self+0x38`, world-translated by `self+0x1c/0x20/0x24 >>8`) into
 * two stack scratch boxes via `sub_800014C` (a real, byte-verified
 * `memcpy(dst,dst,0xc)` self-copy - see src/graphics/actor_part74.c's own
 * definition/doc comment), then run the same 3-axis (Z,Y,X order) overlap
 * test already established throughout this ROM
 * (sub_802D7B0/sub_802DD9C/sub_802C7A8/sub_8031378 etc - see
 * docs/matching/issue-53-actor-c7a8.md, issue-54-actor-d3a8.md,
 * issue-58-0x08030574-actor.md). `sub_802A3AC` is the same test wrapped in
 * an outer walk of the whole `gUnknown_03000884`-rooted circular list
 * (`self+0x4c`), gated by a `sub_803AD7C` per-node visibility check first
 * (same shape as `sub_802C7A8`, actor_part19h.c).
 *
 * All three share the `ActorsOverlap` inline below. Its three boxes are
 * members of one frame struct (the actor_part74.c/actor_part81.c
 * pattern), so every box address is a fresh `add rX, sp, #off`; only
 * the pointer to the middle box stays live across both `sub_800014C`
 * calls (that is the ROM's `r4`, or `r7` once `sub_802A3AC`'s loop
 * hoists it). The first actor's position is read into locals before
 * that pointer is taken, which puts its `add r4, sp, #0xc` after the
 * three loads. The old note blamed an unreachable `r7`; the file
 * simply needs old_agbcc (current agbcc schedules the `asr`s
 * differently, 24-26 halfwords off). `sub_802A208` is NAKED and
 * assembles the same under either compiler. */

extern struct actor_self *gUnknown_03000884;
extern u8 gUnknown_030014A0;
extern u8 gUnknown_03001506;

extern void *sub_800014C(void *dst, const void *src, u32 byteCount);

/* Method slot 0x28 of the actor method table (`self+0x50`), which
 * `struct actor_vtable` still lumps into padding. */
struct actor_methods {
    u8 unk_00[0x28];
    struct actor_method m28; // "skip in overlap scans" query
};

typedef u8 (*actor_query_fn)(void *self);

/* `+0x4c`: next node of the circular actor list rooted at
 * `gUnknown_03000884`. */
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
    sub_800014C(&f.a, &f.a, sizeof(f.a));
    f.s = *(struct box16 *)self->unk_38;
    BoxMove(&f.s, self->x >> 8, self->y >> 8, self->z >> 8);
    *t = f.s;
    sub_800014C(t, t, sizeof(*t));
    return BoxOverlap(&f.a, t);
}

s32 sub_802A018(struct actor_self *self)
{
    struct actor_self **plAddr = &gUnknown_03000884;

    if (gUnknown_030014A0 != 0)
        return 0;
    return ActorsOverlap(*plAddr, self);
}

s32 sub_802A110(struct actor_self *self)
{
    struct actor_self **plAddr = &gUnknown_03000884;

    if (gUnknown_03001506 != 0)
        return 0;
    return ActorsOverlap(*plAddr, self);
}

/* `sub_802A208`: fires a "scroll enter/exit" trampoline pair off
 * `gUnknown_03001418->fn[3]`/`fn[0xa]` (the selected category's vtable,
 * `struct category_vtable`), drives a `gUnknown_03001400`-rooted
 * sub-effect-table draw loop (`sub_803AD84`, same record family as
 * `sub_802A504`/`sub_802A51C`/`sub_802A540`/`sub_802A558`/`sub_802A570`),
 * then walks the whole `gUnknown_03000884`-rooted circular actor list
 * twice: once unconditionally (drawing each node's own `self+0x50`
 * trampoline-record icon via `sub_803AD7C`), once collecting every node
 * with `self+0x2c` set into `gUnknown_03001408` (drawing that filtered
 * set through `sub_803AD80` then a second `sub_803AD7C` pass on a
 * different trampoline-record offset).
 *
 * A careful multi-iteration plain-C reconstruction reproduced the exact
 * same control flow, literal pool contents, and instruction shapes but
 * consistently needed one extra high register (`r9` on top of the ROM's
 * own single `r8`) to hold the function-lifetime
 * `gUnknown_03001418`/`gUnknown_03001400` addresses and the cached
 * `scroll` value simultaneously - the same "more live values than the
 * ROM's own build had to keep" register-pressure gap this project's
 * matching.md catalogs throughout (every attempt at re-deriving *which*
 * value the ROM's C source kept in a true local vs. re-read fresh from
 * the global at each use converged on the same 6-register shape, one
 * over the ROM's 5). NAKED per docs/workflow.md's escape hatch; every
 * instruction below is transcribed directly from the ROM disassembly
 * (`asm/code_3_2_20_8b7c.s`) and verified byte-for-byte against
 * `baserom.gba`, not inferred. */

extern void *gUnknown_03001408;
extern void *gUnknown_03000880;

NAKED s32 sub_802A208(void)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "ldr r7, 11f\n\t"
        "ldr r0, [r7]\n\t"
        "ldr r0, [r0, #0xc]\n\t"
        "cmp r0, #0\n\t"
        "beq 1f\n\t"
        "bl sub_803AD78\n\t"
        "1:\n\t"
        "ldr r1, 12f\n\t"
        "mov r0, #0\n\t"
        "str r0, [r1]\n\t"
        "bl sub_8029B2C\n\t"
        "add r6, r0, #0\n\t"
        "ldr r5, 13f\n\t"
        "ldr r1, [r5]\n\t"
        "sub r1, r6, r1\n\t"
        "ldr r4, 14f\n\t"
        "ldr r0, [r4]\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r1, r0\n\t"
        "ble 2f\n\t"
        "ldr r0, [r7]\n\t"
        "ldr r0, [r0, #0x28]\n\t"
        "bl sub_803AD78\n\t"
        "2:\n\t"
        "ldr r0, 15f\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 3f\n\t"
        "bl sub_8029B8C\n\t"
        "ldr r1, [r5]\n\t"
        "add r1, r1, r0\n\t"
        "str r1, [r5]\n\t"
        "b 5f\n\t"
        ".align 2, 0\n"
        "11: .4byte gUnknown_03001418\n"
        "12: .4byte gUnknown_03001410\n"
        "13: .4byte gUnknown_03001420\n"
        "14: .4byte gUnknown_03001400\n"
        "15: .4byte gUnknown_0300141C\n"
        "3:\n\t"
        "ldr r0, 16f\n\t"
        "mov ip, r0\n\t"
        "ldr r3, [r4]\n\t"
        "ldr r2, [r0]\n\t"
        "ldr r0, [r3, #4]\n\t"
        "cmp r2, r0\n\t"
        "bge 5f\n\t"
        "lsl r1, r2, #2\n\t"
        "add r1, r1, r2\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, r3, #0\n\t"
        "add r0, #0x14\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r0, [r5]\n\t"
        "add r1, r1, r0\n\t"
        "ldr r0, [r7]\n\t"
        "ldr r0, [r0, #0x1c]\n\t"
        "add r0, r6, r0\n\t"
        "cmp r1, r0\n\t"
        "bgt 5f\n\t"
        "mov r8, r7\n\t"
        "add r7, r4, #0\n\t"
        "mov r4, ip\n\t"
        "4:\n\t"
        "mov r2, r8\n\t"
        "ldr r3, [r2]\n\t"
        "ldr r0, [r4]\n\t"
        "lsl r1, r0, #2\n\t"
        "add r1, r1, r0\n\t"
        "lsl r1, r1, #2\n\t"
        "add r1, #8\n\t"
        "ldr r0, [r7]\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, 17f\n\t"
        "ldrb r1, [r1]\n\t"
        "ldr r2, [r5]\n\t"
        "lsl r2, r2, #8\n\t"
        "ldr r3, [r3, #4]\n\t"
        "bl sub_803AD84\n\t"
        "ldr r0, [r4]\n\t"
        "add r2, r0, #1\n\t"
        "str r2, [r4]\n\t"
        "ldr r3, [r7]\n\t"
        "ldr r0, [r3, #4]\n\t"
        "cmp r2, r0\n\t"
        "bge 5f\n\t"
        "lsl r1, r2, #2\n\t"
        "add r1, r1, r2\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, r3, #0\n\t"
        "add r0, #0x14\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r0, [r5]\n\t"
        "add r1, r1, r0\n\t"
        "mov r3, r8\n\t"
        "ldr r0, [r3]\n\t"
        "ldr r0, [r0, #0x1c]\n\t"
        "add r0, r6, r0\n\t"
        "cmp r1, r0\n\t"
        "ble 4b\n\t"
        "5:\n\t"
        "ldr r0, 18f\n\t"
        "ldr r3, [r0]\n\t"
        "add r5, r0, #0\n\t"
        "6:\n\t"
        "ldr r4, [r3, #0x4c]\n\t"
        "ldr r1, [r3, #0x50]\n\t"
        "mov r2, #0x10\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r3, r0\n\t"
        "ldr r1, [r1, #0x14]\n\t"
        "bl sub_803AD7C\n\t"
        "add r3, r4, #0\n\t"
        "ldr r0, [r5]\n\t"
        "cmp r3, r0\n\t"
        "bne 6b\n\t"
        "ldr r0, 19f\n\t"
        "mov r1, #0\n\t"
        "str r1, [r0]\n\t"
        "ldr r1, 18f\n\t"
        "ldr r3, [r1]\n\t"
        "add r5, r0, #0\n\t"
        "ldr r0, 20f\n\t"
        "mov r8, r0\n\t"
        "ldr r2, 21f\n\t"
        "mov ip, r2\n\t"
        "add r4, r5, #0\n\t"
        "mov r7, r8\n\t"
        "add r6, r1, #0\n\t"
        "7:\n\t"
        "add r0, r3, #0\n\t"
        "add r0, #0x2c\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 8f\n\t"
        "ldr r0, [r4]\n\t"
        "ldr r2, [r7]\n\t"
        "lsl r1, r0, #2\n\t"
        "add r1, r1, r2\n\t"
        "str r3, [r1]\n\t"
        "add r0, #1\n\t"
        "str r0, [r4]\n\t"
        "8:\n\t"
        "ldr r3, [r3, #0x4c]\n\t"
        "ldr r0, [r6]\n\t"
        "cmp r3, r0\n\t"
        "bne 7b\n\t"
        "ldr r0, [r5]\n\t"
        "mov r3, r8\n\t"
        "ldr r1, [r3]\n\t"
        "mov r3, ip\n\t"
        "ldr r2, [r3]\n\t"
        "bl sub_803AD80\n\t"
        "mov r4, #0\n\t"
        "ldr r0, [r5]\n\t"
        "cmp r4, r0\n\t"
        "bge 10f\n\t"
        "9:\n\t"
        "ldr r0, 20f\n\t"
        "ldr r1, [r0]\n\t"
        "lsl r0, r4, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r2, [r0, #0x50]\n\t"
        "mov r3, #0x18\n\t"
        "ldrsh r1, [r2, r3]\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, [r2, #0x1c]\n\t"
        "bl sub_803AD7C\n\t"
        "add r4, #1\n\t"
        "ldr r0, 19f\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r4, r0\n\t"
        "blt 9b\n\t"
        "10:\n\t"
        "ldr r1, 22f\n\t"
        "ldr r0, [r1]\n\t"
        "add r0, #1\n\t"
        "str r0, [r1]\n\t"
        "ldr r0, 23f\n\t"
        "ldr r0, [r0]\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
        "16: .4byte gUnknown_03001404\n"
        "17: .4byte gUnknown_03001414\n"
        "18: .4byte gUnknown_03000884\n"
        "19: .4byte gUnknown_0300140C\n"
        "20: .4byte gUnknown_03001408\n"
        "21: .4byte gUnknown_03000880\n"
        "22: .4byte gUnknown_03001424\n"
        "23: .4byte gUnknown_03001410\n"
    );
}

/* `sub_802A3AC`: walks the whole `gUnknown_03000884`-rooted circular
 * actor list (`self+0x4c`) looking for the first OTHER node
 * (`self`'s own arg0, held live in `r8` for the whole function) that
 * passes its method-table slot 0x28 query (false = not skipped) and
 * overlaps `self`'s translated 12-byte AABB, via the same
 * `ActorsOverlap` inline as `sub_802A018`. Inside the loop gcc hoists
 * the third box's `sp+0x18` address into `r7` by itself. */
void *sub_802A3AC(struct actor_self *self)
{
    struct actor_self *n = ACTOR_NEXT(gUnknown_03000884);

    do {
        if (n != self) {
            struct actor_methods *vt = (struct actor_methods *)n->vtable;

            if (((actor_query_fn)vt->m28.fn)((u8 *)n + vt->m28.thisOffset) == 0
                && ActorsOverlap(self, n))
                return n;
        }
        n = ACTOR_NEXT(n);
    } while (n != gUnknown_03000884);
    return 0;
}

asm(".align 2, 0");
