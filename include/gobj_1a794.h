#ifndef GUARD_GOBJ_1A794_H
#define GUARD_GOBJ_1A794_H

/* Shared by src/graphics/actor_part_1a794.c/_1a878.c/_1ab34.c/_1ab98.c/
 * _1b208.c (GitHub issue #25, ROM 0x0801A794-0x0801B85C).
 *
 * Two C++-style classes (gcc 2.x method tables of {s16 this-adjust; fn}
 * pairs, called through the sub_803AD7C/AD80/AD84/AD88 "call via
 * r1/r2/r3/r4" thunks):
 *
 * - `struct gobj`, a 0x80-byte level object built by sub_801A878 (method
 *   table gStaticData_087E49DC: +0x0C sub_801AB34 player-contact test,
 *   +0x1C sub_801B208 per-frame/destroy step, +0x4C sub_801B2C0,
 *   +0x54 sub_801B2C4 destructor). Its `type` (+0x78) comes from the
 *   level's spawn record or is forced by the spawn kind; types 1/5/6/7
 *   get a `struct mover` attached at +0x44. The player object
 *   (gUnknown_030012D8) uses the same layout for the fields read here,
 *   and `carried` (+0xAC) is the object the player is standing on.
 * - `struct mover`, a 0x38-byte helper (method table gStaticData_087E4A54:
 *   +0x0C sub_801B304 per-frame move, +0x4C sub_801B7C4 destructor,
 *   +0x5C sub_801B7A0 / +0x64 sub_801B77C velocity setters) that
 *   oscillates its owner back and forth over `rangeX`/`rangeY` pixels
 *   using the 12-byte velocity records of gStaticData_0816C460, and drags
 *   the player along while it is `active` (sub_801B624). */

struct vec3
{
    s32 x;
    s32 y;
    s32 z;
};

struct vec_pair
{
    u32 a;
    u32 b;
};

struct method
{
    s16 thisOffset;
    u8 unk_2[2];
    void *fn;
};

/* anim_rec's 0x04-0x09 collision box, accessed through a pointer to it
 * (not embedded: this compiler pads every struct to a word multiple) */
struct anim_box
{
    s16 offX;   // 0x00
    s16 offY;   // 0x02
    u8 padX;    // 0x04
    u8 padY;    // 0x05
};

struct anim_rec
{
    u8 unk_00[4];
    s16 offX;   // 0x04 - struct anim_box
    s16 offY;   // 0x06
    u8 padX;    // 0x08
    u8 padY;    // 0x09
    u8 unk_0A[0xA];
    u8 unk_14;  // 0x14
    u8 unk_15;
    u8 frames;  // 0x16
    u8 unk_17[5];
};

struct anim_table
{
    struct anim_rec *records;
};

struct gobj_vtable
{
    u8 unk_00[0x38];
    struct method m38; // 0x38
    u8 unk_40[0x20];
    struct method m60; // 0x60
    struct method m68; // 0x68
};

struct mover;

struct gobj
{
    s32 x;              // 0x00
    s32 y;              // 0x04
    u16 id;             // 0x08
    u8 unk_0A[2];
    u8 flags;           // 0x0C
    u8 flags2;          // 0x0D
    u8 unk_0E[0xA];
    struct gobj_vtable *vtable; // 0x18
    u8 unk_1C[4];
    struct anim_table *anim; // 0x20
    u8 dir;             // 0x24
    u8 unk_25[3];
    u8 mirror;          // 0x28 - bit 4: X mirrored, bit 5: Y mirrored
    u8 slot;            // 0x29 - low nibble: palette/tile slot
    u8 unk_2A[2];
    u8 unk_2C;
    u8 tag;             // 0x2D
    u8 unk_2E[2];
    s32 frame;          // 0x30
    u8 unk_34[4];
    u8 unk_38;          // 0x38
    u8 unk_39[0xB];
    struct mover *mover; // 0x44
    struct vec3 velA;   // 0x48
    struct vec3 velB;   // 0x54
    s32 speedX;         // 0x60
    s32 speedY;         // 0x64
    u8 unk_68;          // 0x68
    u8 unk_69[0xB];
    s32 unk_74;         // 0x74
    s32 type;           // 0x78
    u8 unk_7C[0x30];
    struct gobj *carried; // 0xAC
};

struct mover_vtable
{
    u8 unk_00[8];
    struct method m08; // 0x08
    struct method m10; // 0x10
    struct method m18; // 0x18
    u8 unk_20[0x40];
    struct method m60; // 0x60
};

struct mover
{
    u8 unk_00[4];
    struct { struct vec_pair *entries; } *set; // 0x04
    u8 unk_08[4];
    struct mover_vtable *vtable; // 0x0C
    s32 kind;           // 0x10
    s32 timer;          // 0x14
    s32 distX;          // 0x18
    s32 distY;          // 0x1C
    s32 lastX;          // 0x20
    s32 lastY;          // 0x24
    s32 rangeX;         // 0x28
    s32 rangeY;         // 0x2C
    u8 dirX;            // 0x30
    u8 dirY;            // 0x31
    u8 active;          // 0x32
    u8 unk_33;
    u32 time;           // 0x34
};

struct pos2
{
    s32 x;
    s32 y;
};

struct aabb
{
    s32 x;
    s32 y;
    s32 w;
    s32 h;
};

struct spawn_rec
{
    u8 flags;           // 0x00
    u8 unk_01[3];
    u32 type;           // 0x04
    s32 distX;          // 0x08
    s32 distY;          // 0x0C
    s16 dirX;           // 0x10
    s16 dirY;           // 0x12
    s16 flag;           // 0x14
};

extern struct gobj *gUnknown_030012D8;
extern void *gUnknown_030012C0;
extern void *gUnknown_030012B8;
extern void *gUnknown_030012EC;
extern u8 *gUnknown_030012B4;
extern u8 ***gUnknown_030012D0;
extern u32 gUnknown_0300082C;
extern struct vec_pair gStaticData_0816C418[];
extern struct vec3 gStaticData_0816C3B8[];
extern struct vec3 gStaticData_0816C460[];
extern u8 gStaticData_0816C458[];
extern u8 gStaticData_087E490C[];
extern u8 gStaticData_087E4974[];
extern u8 gStaticData_087E49DC[];
extern u8 gStaticData_087E4A54[];

extern void *sub_8017A8C(void *self);
extern void sub_8017A78(void *self, s32 flags);
extern void sub_8019EBC(void *self, s32 a, u16 b, u16 c, s32 d);
extern void *sub_8026EDC(u32 size);
extern void sub_8009F90(void *self);
extern void sub_8009F1C(void *self, s32 flags);
extern s32 sub_803AD7C(void *self, void *fn);
extern s32 sub_803AD80(void *self, void *arg, void *fn);
extern s32 sub_803AD84(void *self, void *arg1, s32 arg2, void *fn);
extern void sub_803AD88(void *self, s32 a, s32 b, s32 c);
extern u32 sub_803AF1C(u32 a, u32 b);
extern s32 sub_80233B4(void *arg);
extern u8 sub_80232A0(void *arg);
extern u8 sub_8023278(void *arg);
extern struct mover *sub_801961C(void *mem);
extern void sub_8008E94(void *manager, void *value);
extern void sub_80087C0(void *self);
extern void sub_80087B4(void *self);
extern void sub_800872C(void *self, s32 a);
extern u8 sub_8006DF8(void *cache, u8 id);
extern void sub_8007B98(struct aabb *dest, struct gobj *obj);
extern u8 sub_8001688(struct aabb *a, struct aabb *b);
extern s32 sub_8009EBC(struct gobj *obj);
extern s32 sub_8009EC4(struct gobj *obj);
extern s32 sub_800FDC8(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void sub_8007398(struct gobj *obj, s32 x, s32 y);
extern void sub_8008044(struct gobj *obj);
extern void sub_8009EA8(struct gobj *obj);
extern void sub_800B6D0(void *self, void *part, struct vec3 *vec);
extern void sub_800B7B0(void *self, void *part, struct vec3 *vec);
extern void sub_800B8A8(void *self, s32 flags);
extern void sub_800B8C8(void *self);

void sub_801B2D8(struct gobj *self);

/* The object's constructor body (sub_801B2E4), which sub_801A878 inlines
 * into its `new`. */
static inline struct gobj *GobjInit(struct gobj *self)
{
    sub_8009F90(self);
    self->vtable = (struct gobj_vtable *)gStaticData_087E49DC;
    sub_801B2D8(self);
    return self;
}
void sub_801B2A8(struct gobj *self, u8 value);
struct mover *sub_801B7D8(struct mover *self, s32 distX, s32 distY, u32 dirX, u8 dirY, s32 kind);
void sub_801AB98(struct gobj *self, void *unused);
void sub_801B624(struct mover *self, struct gobj *obj);

/* sub_801B7D8's 5th/6th arguments are passed on the stack, the 5th as a
 * genuine byte (`strb`); this compiler always widens a stack-passed
 * argument to a word `str`, so the caller writes both slots itself into
 * `args` (the only thing in its frame, i.e. at sp+0/sp+4 - exactly the
 * outgoing-argument area) and calls through a 4-argument view. */
struct mover_stack_args
{
    u8 dirY;
    u8 unk_1[3];
    s32 kind;
};

typedef struct mover *(*MoverCtor4)(void *mem, s32 distX, s32 distY, u32 dirX);
#define MOVER_NEW(mem, dX, dY, fX) ((MoverCtor4)sub_801B7D8)((mem), (dX), (dY), (fX))

/* Branchless absolute value, updating `x` in place (same helper as
 * actor_part50.c) - the ROM's asr/eor/sub sequence. */
#define ABS32(x, sign) do { (sign) = (x) >> 0x1f; (x) ^= (sign); (x) -= (sign); } while (0)

#define MOVER_CALL3(obj, m, a, b)                                              \
    do                                                                         \
    {                                                                          \
        struct method *_m = &(obj)->vtable->m;                                 \
        sub_803AD84((u8 *)(obj) + _m->thisOffset, (a), (b), _m->fn);           \
    } while (0)

/* sub_803AD88 calls the function in r4 */
#define OBJ_CALL68(obj, a, b, c)                                               \
    do                                                                         \
    {                                                                          \
        struct method *_m = &(obj)->vtable->m68;                               \
        void *_this = (u8 *)(obj) + _m->thisOffset;                            \
        register void *_fn asm("r4") = _m->fn;                                 \
                                                                               \
        asm volatile("" : : "r"(_fn));                                         \
        sub_803AD88(_this, (a), (b), (c));                                     \
    } while (0)

#define OBJ_CALL1(obj, m)                                                      \
    do                                                                         \
    {                                                                          \
        struct method *_m = &(obj)->vtable->m;                                 \
        sub_803AD7C((u8 *)(obj) + _m->thisOffset, _m->fn);                     \
    } while (0)

#define MOVER_CALL2(obj, m, a)                                                 \
    do                                                                         \
    {                                                                          \
        struct method *_m = &(obj)->vtable->m;                                 \
        sub_803AD80((u8 *)(obj) + _m->thisOffset, (a), _m->fn);                \
    } while (0)

#endif /* GUARD_GOBJ_1A794_H */
