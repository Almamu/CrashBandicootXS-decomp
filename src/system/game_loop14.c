#include "core.h"
#include "actor.h"
#include "orbit_part.h"

/* Built with old_agbcc - see docs/matching/game-loop-old-agbcc.md. */

struct level_layer
{
    u8 unk_00[0x10];
    u32 width;                  // 0x10 - extent in the low 24 bits
    u32 height;                 // 0x14 - extent in the low 24 bits
};

struct level_info
{
    u8 unk_00[0x10];
    struct level_layer *layer;  // 0x10
};

struct method
{
    s16 thisOffset;
    u8 unk_2[2];
    void *fn;
};

struct manager
{
    u8 unk_00[0xC];
    struct { u8 unk_00[0x18]; struct method attach; } *vtable; // 0x0C
};

struct fx_part
{
    struct actor base;          // 0x00
    u8 unk_1C[4];
    void *anim;                 // 0x20
    u8 unk_24[4];
    u32 unk_28_0:4;             // 0x28
    u32 flipX:1;
    u32 unk_28_5:3;
    u8 frameNibble:4;           // 0x29
    u8 unk_29_4:4;
    u8 unk_2A[3];
    u8 tag;                     // 0x2D
    u8 unk_2E[0x16];
    struct manager *mgr;        // 0x44
    s32 unk_48;                 // 0x48 - velocity fields seeded by sub_8025B0C
    s32 unk_4C;                 // 0x4C
    s32 unk_50;                 // 0x50
    u8 unk_54[0xC];
    s32 unk_60;                 // 0x60
};

struct fx_box
{
    s32 x, y;
    s32 w;                      // 0x08
    s32 h;
};

struct actor_flag_bits
{
    u8 unk_0:1;
    u8 bit1:1;
    u8 bit2:1;
    u8 unk_3:5;
};

#define ACTOR_FLAG_BITS(a) ((struct actor_flag_bits *)&(a)->flags)

extern struct level_info *gLevelLayers;
extern void ***gUnknown_030012D0;
extern void *gUnknown_030012F0;

extern struct fx_part *sub_8009ED0(u16 arg0, u16 x, u16 y, u16 arg3);
extern void sub_80087C0(struct fx_part *part);
extern void sub_80087B4(struct fx_part *part);
extern void sub_800872C(struct fx_part *part, s32 val);
extern s32 sub_800815C(struct fx_part *part);
extern void *sub_8026EDC(s32 size);
extern struct manager *sub_800CCE0(void);
extern s32 _call_via_r2(void *self, void *arg, void *fn);
extern void sub_8008E94(void *manager, void *value);

/* Spawns a `sub_8025BAC` part next to `src` (at `src`'s tile X/Y, facing
 * its way), places it beside `src` by their two `sub_8007B98` AABBs'
 * half-widths plus `margin`, offsets its Y by `z`, and seeds its
 * velocity fields (`+0x60`/`+0x48`/`+0x4c`/`+0x50`) from `speed`,
 * negated when `src` is mirrored.
 *
 * Matched (old_agbcc) with three source-shape changes, no pins:
 * - the two AABBs live in one frame struct `f`, so the ROM re-adds
 *   `sp,#N` for the second box instead of holding its address in a
 *   callee-saved register (which had pushed `speed` out to the stack);
 * - the spawn arguments go through locals `x0`/`y0`/`m`, which computes
 *   all three before either stack store, as the ROM does;
 * - the velocity seed goes through the `SetVel` inline, whose arguments
 *   (`-speed`, 0x40) are expanded before the stores, and the X offset is
 *   a `?:` so the flip byte is tested before `ox + dist`. */
struct fx_part *sub_8025BAC(void *unused0, s32 anim, s32 tag, s32 x, s32 y, s32 mirror);
extern void sub_8007B98(struct fx_box *dest, void *obj);

static inline void SetVel(struct fx_part *p, s32 v, s32 k)
{
    p->unk_60 = v;
    p->unk_48 = v;
    p->unk_4C = k;
    p->unk_50 = v;
}

struct fx_part *sub_8025B0C(void *pool, s32 arg1, s32 kind, s32 margin, s32 z, s32 speed, struct fx_part *src)
{
    struct fx_part *part;
    s32 w1, w2, dist, x;

    {
        s32 x0 = src->base.x >> 8;
        s32 y0 = src->base.y >> 8;
        s32 m = src->flipX;

        part = sub_8025BAC(pool, arg1, kind, x0, y0, m);
    }
    {
        struct { struct fx_box a, b; } f;

        sub_8007B98(&f.a, part);
        w1 = f.a.w;
        sub_8007B98(&f.b, src);
        w2 = f.b.w;
    }
    dist = w1 / 2 + w2 / 2 + margin;
    {
        s32 ox = part->base.x >> 8;
        s32 y;

        x = part->flipX ? ox - dist : ox + dist;
        y = (part->base.y >> 8) + z;
        part->base.x = x << 8;
        part->base.y = y << 8;
    }
    if (part->flipX)
        SetVel(part, -speed, 0x40);
    else
        SetVel(part, speed, 0x40);
    return part;
}

/* Spawns a sub_8009ED0 effect part at (x, y) clamped into the current
 * level's bounds, facing left when `mirror` is set, with animation
 * record `anim` (12-byte stride) and tag `tag`. Attaches it to a fresh
 * sub_800CCE0 manager and registers it with gUnknown_030012F0. */
struct fx_part *sub_8025BAC(void *unused0, s32 anim, s32 tag, s32 x, s32 y, s32 mirror)
{
    struct fx_part *part;
    struct level_layer *layer;
    struct manager *mgr;

    if (x < 0)
        x = 0;
    layer = gLevelLayers->layer;
    /* Compared sign-extended from 24 bits, clamped zero-extended. */
    if (x >= (s32)(layer->width << 8) >> 8)
        x = (layer->width << 8 >> 8) - 1;
    if (y < 0)
        y = 0;
    if (y >= (s32)(layer->height << 8) >> 8)
        y = (layer->height << 8 >> 8) - 1;
    part = sub_8009ED0(0xffff, x, y, 0);
    part->flipX = mirror != 0;
    part->anim = (u8 *)**gUnknown_030012D0 + anim * 12;
    part->tag = tag;
    sub_80087C0(part);
    sub_80087B4(part);
    sub_800872C(part, 0);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x10);
    mgr = sub_800CCE0();
    part->mgr = mgr;
    _call_via_r2((u8 *)mgr + mgr->vtable->attach.thisOffset, part, mgr->vtable->attach.fn);
    ACTOR_FLAG_BITS(&part->base)->bit2 = 0;
    ACTOR_FLAG_BITS(&part->base)->bit1 = 0;
    sub_8008E94(gUnknown_030012F0, part);
    return part;
}

/* Same early-out and `+0x49`/`+0x4a`/`+0x4b` tagging shape as
 * `sub_8025A64` (game_loop29.c), but spawns via `sub_801173C` with a
 * "special" 4th argument (`0xFFFF` when `p5` is set or `p4 == 0xff`,
 * `0` otherwise) and fires `sub_801191C`/`sub_8011870` instead of
 * `sub_80111B8`.
 *
 * `p5` is read as the low byte of its stack word (the ROM's ldrb), and
 * `p4` as the full word (its `cmp r5,#0xff` has no truncation).
 *
 * Matched (old_agbcc): the three tag bytes are written through a pointer
 * `t`, and the +0x4B zero is an opaque `zero`, so the `movs r0,#0` lands
 * after the +0x49 address instead of being hoisted above it. */
extern struct level_state14 { u8 unk_00[0x8C]; u8 unk_8C; } *gLevelState;
extern struct orbit_part *sub_801173C(u16 id, u16 x, u16 y, u16 special);
extern void sub_801191C(struct orbit_part *self);
extern void sub_8011870(struct orbit_part *self);

struct orbit_part *sub_8025CA4(void *unused0, u32 x, u32 y, u32 p3, u32 p4, u32 flag5)
{
    u8 p5 = *(u8 *)&flag5;
    struct orbit_part *part = NULL;

    if (gLevelState->unk_8C == 0)
    {
        if (p5 || p4 == 0xff)
            part = sub_801173C(0xffff, x, y, 0xffff);
        else
            part = sub_801173C(0xffff, x, y, 0);
        part->base.flags |= 0x10;
        {
            u8 *t = &part->counter;
            u8 zero = 0;

            /* opaque 0: keeps the +0x4B `movs r0,#0` after the +0x49
             * address rather than scheduled ahead of it */
            asm("" : "+r"(zero));
            *t++ = p3;
            *t++ = p4;
            *t = zero;
        }
        if (p4 == 0xff)
            sub_801191C(part);
        if (p5)
            sub_8011870(part);
    }
    return part;
}

/* Loads a `{tableIdx:u16, p1:u16, p2:u16, p3:u16}` record from `rec`,
 * indexes `*table` by `tableIdx` (4-byte stride) to get a function
 * pointer, and tail-calls it as `fn(self, p1, p2, p3)`. On real
 * hardware this indirect call has to go through one of this ROM's
 * fixed per-register interworking trampolines
 * (`src/system/reg_trampolines.c`) rather than a direct `blx` - which
 * specific trampoline (here, `_call_via_r5`/"bx r5") depends purely on
 * which register this compiler's allocator happens to land the
 * function pointer in, hence the `register ... asm("r5")` pin plus the
 * empty-asm "keep this value live" barrier right before the call. */
extern void _call_via_r5(void *a0, u16 a1, u16 a2, u16 a3);

void SpawnEntity(void **table, void *self, u16 *rec)
{
    register void *tablePtr asm("r1") = *table;
    register u16 idx asm("r3") = rec[0];
    register s32 shifted asm("r0") = idx << 2;
    void *entry = (u8 *)shifted + (s32)tablePtr;
    u16 p1 = rec[1];
    u16 p2 = rec[2];
    u16 p3 = rec[3];
    register void *fn asm("r5") = *(void **)entry;

    asm("" :: "r"(fn));
    _call_via_r5(self, p1, p2, p3);
}

/* Stores `{a, b}` into the two Q8 words at `self+0`/`self+4`. */
void sub_8025D4C(void *self, s32 a, s32 b)
{
    *(s32 *)((u8 *)self + 4) = b;
    *(s32 *)self = a;
}

extern void sub_8026ED0(void *self);

/* If bit 0 of `flags` is set, forwards to `sub_8026ED0` - identical
 * body to `sub_8025A44` above (a second copy at a different ROM
 * address, same as `sub_8025A5C`/`sub_8025D6C` below). */
void sub_8025D54(void *self, s32 flags)
{
    if (flags & 1) {
        sub_8026ED0(self);
    }
}

/* Zeroes the two Q8 position words at `self+0`/`self+4` - identical
 * body to `sub_8025A5C` above. */
void sub_8025D6C(void *self)
{
    *(s32 *)self = 0;
    *(s32 *)((u8 *)self + 4) = 0;
}
