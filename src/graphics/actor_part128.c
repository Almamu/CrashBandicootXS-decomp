#include "core.h"
#include "actor_self.h"

/* Covers the 0x0802E0A4-0x0802F0DC gap between issue #54's chunk
 * (`actor_part61.c`, ending at `nullsub_27`/`sub_802E0A0`) and issue
 * #56's chunk (`actor_part43.c`, starting at `sub_802F0DC`). Two things
 * live here:
 *
 * - The level's spawn dispatcher `sub_802E170` (a 31-case `switch` over
 *   the spawn "kind", indexing the stride-40 per-kind record table
 *   `gUnknown_030014D8`) and its helpers: `sub_802E0CC` picks a spawn
 *   record's kind byte and forwards to it, and the run of small
 *   `new Foo(...)` constructors (`sub_802E3CC`-`sub_802E6CC`) each
 *   allocate one object and hand it a fixed record of the same table.
 * - The player's vehicle object (method table gStaticData_087E5144,
 *   built by `sub_802E710`/`sub_802E740`): its per-frame update
 *   (`sub_802E84C`), sprite draw (`sub_802E9FC`), damage handler
 *   (`sub_802EB78`), d-pad steering (`sub_802EC64`/`sub_802ED10`) and
 *   the per-state input steps (`sub_802EDBC`-`sub_802EFD8`). Its state
 *   lives in the `gUnknown_030014DC`-`gUnknown_03001518` singletons.
 *
 * Built with old_agbcc: `sub_802E9FC` only matches under it (current
 * agbcc loads its `attr` halfword straight into the callee-saved
 * register instead of via r0); every other function here compiles
 * identically under both. */

/* One record of the `gUnknown_030014D8` per-kind table (stride 40). */
struct kind_entry {
    u8 unk_00[0x20];
    s32 dx;         // 0x20 - added to the spawn X
    s32 dy;         // 0x24 - added to the spawn Y
};

/* A level spawn record, as passed to `sub_802E0CC`. */
struct spawn_rec {
    u8 kind[3];     // 0x00 - normal / alternate-mode / `alt`-gated kind
    u8 pad;
    s32 x;          // 0x04 - tile units (<< 8 to Q8)
    s32 y;          // 0x08
    s32 z;          // 0x0C
};

/* `actor_self` plus the hit-point word every class built here keeps at
 * +0x54. */
struct actor_hp {
    struct actor_self base;
    s32 hp;         // 0x54
};

/* The camera-ish object `sub_802E9FC` reads through `self+0x30`. */
struct cam_ref {
    u8 unk_00[0x10];
    s32 depth;      // 0x10 - the depth at which sprites draw unscaled
};

struct keys_pair {
    u16 held;
    u16 pressed;
};

/* A one-byte by-value argument: the ROM stores it into its stack slot
 * with `strb` (a promoted `u8` would be stored with `str`). */
struct byte_arg {
    u8 v;
} __attribute__((packed));

extern u8 *mem_alloc(u32 size, s32 flags);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void InitActorPart(void *self, void *part, s32 b, s32 c, s32 d);
extern s32 GetAnimFrameBaseOffset(void *self);
extern u32 GetSpriteShapeSizeBits(u8 *frame);
extern void QueueSpriteFrameOam(u32 attr01, u16 attr2, s32 priority);
extern void sub_80019F8(void *ctx, s32 id, s32 frame, s32 vol, struct byte_arg force);
extern u8 sub_8023418(void *self);
extern void sub_8023234(void *self);
extern u8 sub_8029794(void);
extern s32 sub_8029B2C(void);
extern void sub_8029BAC(s32 a);
extern void sub_8029D8C(s32 x, s32 y);
extern s32 sub_8029E98(void);
extern s32 sub_8029EB4(void);
extern u8 sub_802AA80(void *spawn);
extern void sub_802F338(void *self);
extern void sub_802F3BC(void *self);
extern void sub_802F4CC(void *self);
extern void sub_8031040(s32 k, s32 x, s32 y, s32 z);
extern void sub_8033264(s32 k, s32 x, s32 y, s32 z);
extern s32 sub_802FD8C(void *obj, struct kind_entry *rec, s32 x, s32 y, s32 z, void *spawn);
extern s32 sub_802FF08(void *obj, struct kind_entry *rec, s32 x, s32 y, s32 z);
extern s32 sub_8032054(void *obj, struct kind_entry *rec, s32 x, s32 y, s32 z);
extern s32 sub_80320C4(void *obj, struct kind_entry *rec, s32 x, s32 y, s32 z, void *spawn);
extern s32 sub_8031F78(void *obj, struct kind_entry *rec, s32 x, s32 y, s32 z);
extern s32 sub_8032440(void *obj, struct kind_entry *rec, s32 x, s32 y, s32 z);
extern s32 sub_80325EC(void *obj, struct kind_entry *rec, s32 x, s32 y, s32 z);
extern s32 sub_80326E4(void *obj, struct kind_entry *rec, s32 x, s32 y, s32 z);
extern s32 sub_8031920(void *obj, struct kind_entry *rec, s32 a, s32 b, s32 c, s32 d);
extern s32 sub_8032890(void *obj, struct kind_entry *rec, s32 a, s32 b, s32 c);
extern s32 sub_80342D4(void *obj, struct kind_entry *rec, s32 a, s32 b, s32 c);
extern void *sub_8034058(void *obj, struct kind_entry *rec, s32 a, s32 b, s32 c, struct byte_arg d);
extern s32 sub_8033EF4(void *obj, struct kind_entry *rec, s32 a, s32 b, s32 c);
extern s32 sub_8033BB8(void *obj, struct kind_entry *rec, s32 a, s32 b, s32 c);
extern void sub_80329D4(void *obj, struct kind_entry *rec, s32 a, s32 b, s32 c);
extern void sub_80305F8(void *obj, struct kind_entry *rec, s32 a, s32 b, s32 c);
extern void sub_8030300(void *obj, struct kind_entry *rec, s32 a, s32 b, s32 c, s32 d, s32 e);
extern void sub_802FA04(void *obj, struct kind_entry *rec, s32 a, s32 b, s32 c, s32 d, s32 e);

extern struct actor_self *gUnknown_03000884;
extern void (*gUnknown_03000874)(void *dst, u8 *frame);
extern struct keys_pair gUnknown_030007E0;
extern void *gUnknown_030012BC;
extern u8 *gUnknown_030012C0;
extern void *gUnknown_030014BC;
extern struct kind_entry *gUnknown_030014D8;
extern s32 gUnknown_030014DC;
extern s32 gUnknown_030014E0;
extern s32 gUnknown_030014E4;
extern u8 gUnknown_030014E8;
extern s32 gUnknown_030014EC;
extern s32 gUnknown_030014F0;
extern s32 gUnknown_030014F4;
extern s32 gUnknown_030014F8;
extern s32 gUnknown_030014FC;
extern s32 gUnknown_03001500;
extern u8 gUnknown_03001504;
extern u8 gUnknown_03001505;
extern u8 gUnknown_03001506;
extern u8 gUnknown_03001507;
extern s32 gUnknown_03001508;
extern s32 gUnknown_0300150C;
extern s32 gUnknown_03001510;
extern u8 *gUnknown_03001514;
extern u8 *gUnknown_03001518[2];
extern struct actor_pmf gStaticData_0817C1C0[];
extern u8 gStaticData_087E50D4[];
extern u8 gStaticData_087E510C[];
extern u8 gStaticData_087E5144[];

ACTOR_CALL_VIA_ALIASES
asm(".set __divsi3, sub_803ADB4");

/* `operator new`: the ROM materializes the size before the heap flags. */
static inline void *AllocActor(u32 size)
{
    return mem_alloc(size, 0x80000000);
}

/* The inlined base constructor of the hit-point classes: the hit-point
 * value is an argument, so it's materialized before the call. */
static inline void InitHpActor(struct actor_hp *obj, struct kind_entry *rec, s32 x, s32 y, s32 z, s32 hp)
{
    InitActorPart(obj, rec, x, y, z);
    obj->hp = hp;
}

/* Branchless `abs()` (`asrs`/`eors`/`subs`), as the ROM computes it. */
static inline s32 Abs(s32 x)
{
    s32 s = x >> 31;

    return (x ^ s) - s;
}

/* Clamps a steering speed to +-0x240, keeping its sign. */
#define CLAMP_SPEED(v)                                                         \
    if (Abs(v) > 0x240)                                                        \
        (v) = (v) < 0 ? -0x240 : ((v) != 0 ? 0x240 : 0);                      \
    else (void)0

/* On the state-3 anim-frame edge, resets `self` (`gUnknown_030014BC`)
 * back to state 3/table-index 0 with a fresh anim frame from `self`'s
 * own part table at `+0x24`. */
void sub_802E0A4(void)
{
    u8 *self = gUnknown_030014BC;

    if (*(s32 *)(self + 0xc) != 3 && self[0x12] != 0) {
        *(s32 *)(self + 0xc) = 3;
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0x24);
            register u8 zero1 asm("r1") = 0;
            register s32 zero2 asm("r2") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero1;
            *(s32 *)(self + 8) = zero2;
        }
    }
}

s32 sub_802E170(u8 kind, s32 x, s32 y, s32 z, void *spawn);

/* Spawns the object a level spawn record describes: its kind comes from
 * byte 0, byte 1 in the alternate game mode (kind 0x17 there becomes
 * 0x14) or byte 2 when `alt` is set. Kind 0x1d only spawns while
 * `sub_8023418` allows it; kinds 0, 0x3e and 0x20-0x25 never do. */
s32 sub_802E0CC(struct spawn_rec *rec, u8 alt, s32 dz)
{
    u8 kind = rec->kind[0];
    s32 x, y, z;

    if (gUnknown_030012C0[0x8c] != 0) {
        kind = rec->kind[1];
        if (kind == 0x17)
            kind = 0x14;
    } else if (alt != 0) {
        kind = rec->kind[2];
    }
    if (kind == 0x1d && !sub_8023418(gUnknown_030012C0))
        return 0;
    if (kind == 0 || kind == 0x3e || (u8)(kind - 0x20) <= 5)
        return 0;
    x = rec->x << 8;
    y = rec->y << 8;
    z = (rec->z << 8) + dz;
    if ((u8)(kind - 0x10) <= 2) {
        sub_8031040(kind - 0x10, x, y, z);
    } else if (kind != 0xa) {
        return sub_802E170(kind, x, y, z, rec);
    } else {
        sub_8033264(0, x, y, z);
    }
    return 0;
}

/* The spawn dispatcher: offsets the position by the kind's record and
 * constructs the kind's object. Kind 23 turns into kind 20's object
 * when `sub_802AA80` says so; kind 31 spawns a kind-43 companion first. */
s32 sub_802E170(u8 kind, s32 x, s32 y, s32 z, void *spawn)
{
    x += gUnknown_030014D8[kind].dx;
    y += gUnknown_030014D8[kind].dy;
    switch (kind) {
    case 1:
        return sub_802FD8C(AllocActor(0x80), &gUnknown_030014D8[kind], x, y, z, spawn);
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        return sub_802FF08(AllocActor(0x64), &gUnknown_030014D8[kind], x, y, z);
    case 19:
        return sub_8032054(AllocActor(0x70), &gUnknown_030014D8[kind], x, y, z);
    case 23:
        if (sub_802AA80(spawn))
            return sub_80320C4(AllocActor(0x74), &gUnknown_030014D8[20], x, y, z, spawn);
        /* fallthrough */
    case 20:
    case 21:
    case 22:
        return sub_80320C4(AllocActor(0x74), &gUnknown_030014D8[kind], x, y, z, spawn);
    case 24:
    case 25:
    case 26:
    case 29:
        return sub_8031F78(AllocActor(0x70), &gUnknown_030014D8[kind], x, y, z);
    case 27:
        return sub_8032440(AllocActor(0x60), &gUnknown_030014D8[kind], x, y, z);
    case 28:
        return sub_80325EC(AllocActor(0x68), &gUnknown_030014D8[kind], x, y, z);
    case 31:
        sub_80326E4(AllocActor(0x5c), &gUnknown_030014D8[43],
                    x - gUnknown_030014D8[kind].dx + gUnknown_030014D8[43].dx, y, z);
        return sub_80326E4(AllocActor(0x5c), &gUnknown_030014D8[kind], x, y, z);
    }
    return 0;
}

/* Plays sfx 0x17 and spawns a 1-hit-point kind-46 object at the origin. */
void sub_802E3CC(void)
{
    struct actor_hp *obj;

    PlaySfx(gUnknown_030012BC, 0x17, 0x100);
    obj = AllocActor(0x58);
    InitHpActor(obj, &gUnknown_030014D8[46], 0, 0, 0, 1);
    obj->base.vtable = (struct actor_vtable *)gStaticData_087E50D4;
}

/* Plays sfx 4 and spawns a 1-hit-point kind-45 object at (x, y, z). */
void sub_802E420(s32 x, s32 y, s32 z)
{
    struct actor_hp *obj;

    PlaySfx(gUnknown_030012BC, 4, 0x100);
    obj = AllocActor(0x58);
    InitHpActor(obj, &gUnknown_030014D8[45], x, y, z, 1);
    obj->base.vtable = (struct actor_vtable *)gStaticData_087E510C;
}

/* Kind-44 constructor. */
void sub_802E484(s32 a, s32 b, s32 c)
{
    sub_8032890(AllocActor(0x64), &gUnknown_030014D8[44], a, b, c);
}

/* `sub_8031920`-class constructor for any kind. */
s32 sub_802E4B8(u8 kind, s32 a, s32 b, s32 c, s32 d)
{
    return sub_8031920(AllocActor(0x64), &gUnknown_030014D8[kind], a, b, c, d);
}

/* Kind-14 constructor. */
void sub_802E504(s32 a, s32 b, s32 c)
{
    sub_80342D4(AllocActor(0x5c), &gUnknown_030014D8[14], a, b, c);
}

/* Kind-13 constructor; the last argument is passed as a single byte. */
void sub_802E538(s32 a, s32 b, s32 c, u8 d)
{
    struct byte_arg arg;

    arg.v = d;
    sub_8034058(AllocActor(0x70), &gUnknown_030014D8[13], a, b, c, arg);
}

/* Kind-12 constructor. */
void sub_802E57C(s32 a, s32 b, s32 c)
{
    sub_8033EF4(AllocActor(0x70), &gUnknown_030014D8[12], a, b, c);
}

/* Kind-11 constructor. */
void sub_802E5B0(s32 a, s32 b, s32 c)
{
    sub_8033BB8(AllocActor(0x70), &gUnknown_030014D8[11], a, b, c);
}

/* Plays sfx 0x38 and spawns a kind-39 object. */
void sub_802E5E4(s32 x, s32 y, s32 z)
{
    PlaySfx(gUnknown_030012BC, 0x38, 0x100);
    sub_80329D4(AllocActor(0x6c), &gUnknown_030014D8[39], x, y, z);
}

/* Plays sfx 0x38 and spawns a kind-38 object. */
void sub_802E62C(s32 x, s32 y, s32 z)
{
    PlaySfx(gUnknown_030012BC, 0x38, 0x100);
    sub_80305F8(AllocActor(0x6c), &gUnknown_030014D8[38], x, y, z);
}

/* Plays sfx 0x30 and spawns a kind-3 object. */
void sub_802E674(s32 a, s32 b, s32 c, s32 d, s32 e)
{
    PlaySfx(gUnknown_030012BC, 0x30, 0x100);
    sub_8030300(AllocActor(0x60), &gUnknown_030014D8[3], a, b, c, d, e);
}

/* Spawns a kind-2 object (the vehicle's shot - see `sub_802EDBC`). */
void sub_802E6CC(s32 a, s32 b, s32 c, s32 d, s32 e)
{
    sub_802FA04(AllocActor(0x60), &gUnknown_030014D8[2], a, b, c, d, e);
}

struct actor_hp *sub_802E740(struct actor_hp *self, struct kind_entry *rec, s32 z);

/* Installs the level's per-kind table and builds the player vehicle
 * from its first record, making it the (self-linked) player object. */
void sub_802E710(struct kind_entry *table, s32 z)
{
    struct actor_hp *p;

    gUnknown_030014D8 = table;
    gUnknown_03000884 = &(p = sub_802E740(AllocActor(0x58), gUnknown_030014D8, z))->base;
    ((void **)p->base.unk_48)[0] = p;
    ((void **)p->base.unk_48)[1] = p;
}

/* The player vehicle's constructor: 100 hit points (0x78 when
 * `sub_8029794` says so), and a reset of all its singleton state. A
 * nonzero start depth starts it in state 7. */
struct actor_hp *sub_802E740(struct actor_hp *self, struct kind_entry *rec, s32 z)
{
    s32 y = 0;

    if (z == 0)
        y = -0x9600;
    InitHpActor(self, rec, 0, y, z, 100);
    self->base.vtable = (struct actor_vtable *)gStaticData_087E5144;
    sub_802F338(self);
    gUnknown_0300150C = 0;
    if (self->base.z != 0) {
        gUnknown_03001508 = 0;
        ACTOR_SET_STATE(&self->base, 7, 0);
        sub_8029BAC(0x28);
    } else {
        sub_8029BAC(0x1e);
        gUnknown_03001508 = 0x180;
    }
    gUnknown_03001507 = 0;
    gUnknown_03001506 = 1;
    gUnknown_03001500 = 0;
    gUnknown_03001505 = 0;
    gUnknown_03001504 = 0;
    gUnknown_030014FC = 0;
    gUnknown_030014F8 = 0;
    gUnknown_030014F4 = 0;
    gUnknown_030014EC = -0xbe;
    gUnknown_030014F0 = 0;
    gUnknown_030014E8 = 0;
    if (sub_8029794())
        self->hp = 0x78;
    gUnknown_030014E4 = self->hp;
    gUnknown_030014E0 = 0;
    gUnknown_030014DC = 0;
    return self;
}

/* The vehicle's per-frame update: engine-sound throttle, fire cooldown,
 * movement by the steering speeds (clamped to the play area unless
 * `gUnknown_03001506` is set), depth, animation, then the current
 * state's handler from gStaticData_0817C1C0. */
void sub_802E84C(struct actor_hp *self)
{
    s32 x, y;

    if (gUnknown_030014E0 != 0) {
        if (gUnknown_030014DC-- <= 0) {
            s32 vol;

            gUnknown_030014DC = 0x16;
            vol = gUnknown_030014E0 * 48;
            if (vol > 0x100)
                vol = 0x100;
            PlaySfx(gUnknown_030012BC, 0x37, vol);
        }
        gUnknown_030014E0 = 0;
    }
    if (gUnknown_03001500 != 0)
        gUnknown_03001500--;
    sub_802F4CC(self);
    sub_802F3BC(self);
    x = self->base.x += gUnknown_0300150C;
    y = self->base.y += gUnknown_03001508;
    if (gUnknown_03001506 == 0) {
        self->base.x = x < -0x8000 ? -0x8000 : x;
        self->base.x = self->base.x > 0x8000 ? 0x8000 : self->base.x;
        self->base.y = y < -0x4b00 ? -0x4b00 : y;
        self->base.y = self->base.y > 0x4b00 ? 0x4b00 : self->base.y;
    }
    if (gUnknown_03001504 == 0) {
        self->base.depth = 0x1c00;
        self->base.z = (sub_8029B2C() << 8) + self->base.depth;
    }
    {
        s32 d = (self->base.depth >> 1) & 0x7f80;

        self->base.visible = d | (((Abs(self->base.y) + Abs(self->base.x)) >> 11) & 0x7f);
    }
    self->base.stateTime++;
    self->base.animTime += *(s16 *)&self->base.animTimer;
    self->base.animDone = 0;
    if (GetAnimFrameBaseOffset(self) >= self->base.anims[self->base.animIndex].loopThreshold) {
        self->base.animTime -= (self->base.anims[self->base.animIndex].loopThreshold
                                - self->base.anims[self->base.animIndex].loopBase) << 8;
        self->base.animDone = 1;
    }
    sub_8029D8C(self->base.x, self->base.y);
    ACTOR_PMF_CALL(&self->base, gStaticData_0817C1C0);
}

asm(".align 2, 0");

/* The anim_part_instance accessors (actor_anim.c), inlined. */
static inline s32 AnimBase(struct actor_self *self)
{
    return self->animTime >> 8;
}

static inline u8 *CurFrame(struct actor_self *self)
{
    s32 base = AnimBase(self);
    s32 idx = self->animIndex;
    struct anim_frame_record *table = self->anims;
    s32 val = table[idx].frameIndex;

    val += base;
    return (u8 *)self->frameOffsets[val];
}

static inline s32 CurAttr(struct actor_self *self)
{
    s32 idx = self->animIndex;
    struct anim_frame_record *table = self->anims;

    return (s32)table[idx].attr << 16;
}

/* The vehicle's sprite draw: projects the position by depth (scaled
 * and double-sized when drawn behind the reference depth), culls
 * against the screen, uploads the frame's tiles into the other of the
 * two VRAM buffers when the frame changed, and queues the OAM entry. */
void sub_802E9FC(struct actor_hp *self)
{
    s32 scale;
    s32 attr1 = 0;
    u8 *frame;
    u32 w, h;
    s32 halfW, halfH;
    s32 sx, sy;

    frame = CurFrame(&self->base);
    w = frame[0];
    halfW = w * 4;
    h = frame[1];
    halfH = h * 4;
    if (self->base.depth == (*(struct cam_ref **)&self->base.unk_2C[4])->depth) {
        scale = 0x100;
        sy = (self->base.y + sub_8029E98()) >> 8;
        sx = (self->base.x + sub_8029EB4()) >> 8;
    } else {
        s32 depth = self->base.depth;
        s32 f;

        scale = (depth << 8) / (*(struct cam_ref **)&self->base.unk_2C[4])->depth;
        f = 0x1c00000 / depth;
        sy = (((self->base.y * f) >> 12) + sub_8029E98()) >> 8;
        sx = (((self->base.x * f) >> 12) + sub_8029EB4()) >> 8;
        attr1 = 0x100;
        if (scale <= 0xff) {
            attr1 |= 0x200;
            halfW = w * 8;
            halfH = h * 8;
        }
    }
    sx -= halfW;
    sy -= halfH;
    if (sy <= 0x9f && sy + halfH * 2 >= 0 && sx <= 0xef && sx + halfW * 2 >= 0) {
        u32 attr = CurAttr(&self->base);

        attr1 |= (sy & 0xff) | ((sx & 0x1ff) << 16) | attr | GetSpriteShapeSizeBits(frame);
        if (frame != gUnknown_03001514) {
            gUnknown_03001510 ^= 1;
            gUnknown_03000874(gUnknown_03001518[gUnknown_03001510], frame);
            gUnknown_03001514 = frame;
        }
        {
            /* the ROM computes the tile number in r0 */
            register u32 tile asm("r0") = GET_TILE_NUM(gUnknown_03001518[gUnknown_03001510]);

            QueueSpriteFrameOam(attr1, tile | (self->base.unk_18 << 12), scale);
        }
    }
}

/* Damage handler: ignored during the first 16 frames of states 2/3.
 * Out of hit points, the vehicle enters state 4 (anim 3), input is
 * locked and the steering speeds are cut; otherwise sfx 0x42 plays. */
void sub_802EB78(struct actor_hp *self, s32 dmg)
{
    if ((u32)(self->base.state - 2) <= 1 && self->base.stateTime <= 0x10)
        return;
    self->hp -= dmg;
    gUnknown_030014F4 = 0x12;
    if (self->hp <= 0) {
        self->hp = 0;
        PlaySfx(gUnknown_030012BC, 0x3a, 0x100);
        ACTOR_SET_STATE(&self->base, 4, 3);
        if (gUnknown_030012C0[0x8c] == 0)
            sub_8023234(gUnknown_030012C0);
        gUnknown_03001507 = 0;
        gUnknown_030014E8 = 1;
        gUnknown_03001506 = 1;
        sub_8029BAC(0x1e);
        gUnknown_03001508 = 0;
        CLAMP_SPEED(gUnknown_0300150C);
        gUnknown_0300150C /= 2;
    } else {
        PlaySfx(gUnknown_030012BC, 0x42, 0x100);
    }
}

/* The key word read as a whole (the ROM does a 32-bit load). */
static inline struct keys_pair ReadKeys(void)
{
    return gUnknown_030007E0;
}

/* Moves a steering speed 0x40 toward zero. */
static inline void DecaySpeed(s32 *p)
{
    s32 v = *p;

    if (v >= 0) {
        if (v != 0)
            v -= 0x40;
    } else {
        v += 0x40;
    }
    *p = v;
}

/* Vertical steering (a C++ method; `this` is unused): up/down change
 * `gUnknown_03001508` by 0x40 while input is enabled, otherwise it
 * decays to zero; clamped to +-0x240. */
void sub_802EC64(void *self)
{
    if (gUnknown_03001507 && (ReadKeys().held & 0x40))
        gUnknown_03001508 -= 0x40;
    else if (gUnknown_03001507 && (ReadKeys().held & 0x80))
        gUnknown_03001508 += 0x40;
    else {
        DecaySpeed(&gUnknown_03001508);
        if (Abs(gUnknown_03001508) <= 0x40)
            gUnknown_03001508 = 0;
    }
    CLAMP_SPEED(gUnknown_03001508);
}

/* Horizontal steering, same shape with left/right and
 * `gUnknown_0300150C`. */
void sub_802ED10(void *self)
{
    if (gUnknown_03001507 && (ReadKeys().held & 0x20))
        gUnknown_0300150C -= 0x40;
    else if (gUnknown_03001507 && (ReadKeys().held & 0x10))
        gUnknown_0300150C += 0x40;
    else {
        DecaySpeed(&gUnknown_0300150C);
        if (Abs(gUnknown_0300150C) <= 0x40)
            gUnknown_0300150C = 0;
    }
    CLAMP_SPEED(gUnknown_0300150C);
}

/* Normal-state step: steering, then R/L enter the roll states 2/3 and
 * A fires a shot (sfx 0x24, `sub_802E6CC`) when the cooldown allows. */
void sub_802EDBC(struct actor_hp *self)
{
    sub_802EC64(self);
    sub_802ED10(self);
    if (gUnknown_03001507) {
        struct keys_pair keys = gUnknown_030007E0;

        if (keys.held & 0x200) {
            gUnknown_030014F4 = 0x12;
            PlaySfx(gUnknown_030012BC, 0xa, 0x100);
            ACTOR_SET_STATE(&self->base, 2, 1);
        } else if (keys.held & 0x100) {
            gUnknown_030014F4 = 0x12;
            PlaySfx(gUnknown_030012BC, 0xa, 0x100);
            ACTOR_SET_STATE(&self->base, 3, 2);
        } else if (gUnknown_03001500 == 0 && (keys.held & 1)) {
            struct byte_arg one;
            s32 x, y;

            gUnknown_03001500 = 0x12;
            one.v = 1;
            sub_80019F8(gUnknown_030012BC, 0x24, 1000, 0xa0, one);
            x = self->base.x + 0x1200;
            y = self->base.y - 0x1800;
            sub_802E6CC(x, y, self->base.z + 10, (x * 0x199) >> 12, (y * 0x199) >> 12);
        }
    }
}

/* Roll state 2: a quick leftward burst for 5 frames, then the horizontal
 * speed recovers; after 0x21 frames R/L may chain another roll, and the
 * animation's end returns to state 1. */
void sub_802EED0(struct actor_hp *self)
{
    sub_802EC64(self);
    if (self->base.stateTime <= 5) {
        gUnknown_0300150C += -0x100;
        if (gUnknown_0300150C < -0x500)
            gUnknown_0300150C = -0x500;
    } else {
        gUnknown_0300150C += 0x2d;
        if (Abs(gUnknown_0300150C) <= 0x2d)
            gUnknown_0300150C = 0;
    }
    if (self->base.stateTime > 0x21) {
        struct keys_pair keys;

        sub_802ED10(self);
        keys = gUnknown_030007E0;
        if (keys.held & 0x200) {
            gUnknown_030014F4 = 0x12;
            PlaySfx(gUnknown_030012BC, 0xa, 0x100);
            ACTOR_SET_STATE(&self->base, 2, 1);
        } else if (keys.held & 0x100) {
            gUnknown_030014F4 = 0x12;
            PlaySfx(gUnknown_030012BC, 0xa, 0x100);
            ACTOR_SET_STATE(&self->base, 3, 2);
        }
    }
    if (self->base.animDone) {
        ACTOR_SET_STATE(&self->base, 1, 0);
    }
}

/* Roll state 3: the rightward mirror of `sub_802EED0`. */
void sub_802EFD8(struct actor_hp *self)
{
    sub_802EC64(self);
    if (self->base.stateTime <= 5) {
        gUnknown_0300150C += 0x100;
        if (gUnknown_0300150C > 0x500)
            gUnknown_0300150C = 0x500;
    } else {
        gUnknown_0300150C -= 0x2d;
        if (Abs(gUnknown_0300150C) <= 0x2d)
            gUnknown_0300150C = 0;
    }
    if (self->base.stateTime > 0x21) {
        struct keys_pair keys;

        sub_802ED10(self);
        keys = gUnknown_030007E0;
        if (keys.held & 0x200) {
            gUnknown_030014F4 = 0x12;
            PlaySfx(gUnknown_030012BC, 0xa, 0x100);
            ACTOR_SET_STATE(&self->base, 2, 1);
        } else if (keys.held & 0x100) {
            gUnknown_030014F4 = 0x12;
            PlaySfx(gUnknown_030012BC, 0xa, 0x100);
            ACTOR_SET_STATE(&self->base, 3, 2);
        }
    }
    if (self->base.animDone) {
        ACTOR_SET_STATE(&self->base, 1, 0);
    }
}

asm(".align 2, 0");
