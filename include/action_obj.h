#ifndef GUARD_ACTION_OBJ_H
#define GUARD_ACTION_OBJ_H

/* The player/action object behind the gActionCtrlStateTable 42-slot action
 * table (docs/rom_map.md), as far as src/graphics/actor_part_13c60.c and
 * actor_part_14674.c (GitHub issue #17, both built with old_agbcc) use it.
 * actor_part18.c/actor_part_138e8.c and friends reach the same fields
 * through raw offsets.
 *
 * `vt` is a gcc 2.x method table ({s16 this-adjust; fn} entries, called
 * through libgcc's _call_via_r2/_call_via_r3 trampolines), `part` the on-screen object it animates, and the
 * +0x27..+0x32 bytes two "next action" trios (+0x31/+0x2F/+0x27 and
 * +0x32/+0x30/+0x28) the table's dispatcher consumes. */

struct act_method
{
    s16 thisOffset;
    u8 unk_2[2];
    void *fn;
};

struct act_vtable
{
    u8 unk_00[0x10];
    struct act_method m10; // 0x10
    u8 unk_18[8];
    struct act_method m20; // 0x20 - "set animation"
    struct act_method m28; // 0x28
    struct act_method m30; // 0x30
    struct act_method m38; // 0x38
    struct act_method m40; // 0x40
    struct act_method m48; // 0x48
    struct act_method m50; // 0x50 - "set part animation"
};

struct act_anim_record
{
    u8 unk_00[0x14];
    u8 paletteId;          // 0x14 - LoadPaletteSlot/GetPaletteSlot record id
    u8 unk_15;
    u8 frameCount;         // 0x16
    u8 unk_17[5];
};

struct act_anim_bank
{
    struct act_anim_record *records;
    u8 unk_04[6];
    u16 unk_0A;            // 0x0A
};

struct act_part
{
    s32 x;                 // 0x00 (Q8)
    s32 y;                 // 0x04 (Q8)
    u16 id;                // 0x08
    u8 kind;               // 0x0A - object kind passed to the hit handlers (0x13: player;
                           //        0x14-0x16 during some attack actions)
    u8 unk_0B;
    u8 flags0C;            // 0x0C
    u8 flags0D;            // 0x0D
    u8 unk_0E[0x12];
    struct act_anim_bank *bank; // 0x20
    u8 unk_24[4];
    u8 flags28;            // 0x28 - bit 4: X mirror
    u8 slotNibble:4;       // 0x29 - palette slot
    u8 unk_29_4:4;
    u8 unk_2A[3];
    u8 tag;                // 0x2D
    u8 unk_2E[2];
    s32 frame;             // 0x30
    s32 stepTimer;         // 0x34 - ticks spent on the current step
    u8 animDone;           // 0x38
    u8 unk_39[0xF];
    s32 velAX;             // 0x48 - struct gobj's velA/velB/speedX/speedY, as
    s32 velAY;             // 0x4C   separate words
    s32 velAZ;             // 0x50
    s32 velBX;             // 0x54
    s32 velBY;             // 0x58
    s32 velBZ;             // 0x5C
    s32 speedX;            // 0x60
    s32 speedY;            // 0x64
    u8 contact;            // 0x68
    u8 unk_69[0x23];
    s32 deadline;          // 0x8C - struct gobj.deadline
    u8 unk_90;             // 0x90
    u8 unk_91[3];
    u8 listCount;          // 0x94 - struct gobj.listCount
    u8 unk_95[0x6B];
    u8 unk_100;            // 0x100
    u8 unk_101;            // 0x101
    u8 pushLeft;           // 0x102 - nonzero: moves the standing player 1px left per frame
    u8 pushRight;          // 0x103 - nonzero: moves the standing player 1px right per frame
};

/* One entry of the per-object table `act.anims` points at: indices into
 * gStaticData_0816B304's 12-byte records for the +0x27 and +0x28 actions. */
struct act_anim_pair
{
    s32 first;
    s32 second;
};

struct act
{
    u8 unk_00[4];
    struct act_anim_pair **anims; // 0x04
    s32 state;             // 0x08
    struct act_vtable *vt; // 0x0C
    struct act_part *part; // 0x10
    u8 unk_14[4];
    s32 frame;             // 0x18
    s32 frames;            // 0x1C
    u8 charge;             // 0x20
    u8 unk_21;
    u8 unk_22;             // 0x22
    u8 unk_23;             // 0x23
    u8 unk_24[2];
    u8 spinCooldown;       // 0x26 - frames until the next spin is allowed (set to 12, counts down)
    u8 next27;             // 0x27
    u8 next28;             // 0x28
    u8 unk_29;             // 0x29
    u8 unk_2A[5];
    u8 flag2F;             // 0x2F
    u8 flag30;             // 0x30
    u8 next31;             // 0x31
    u8 next32;             // 0x32
    u8 unk_33;
    u8 unk_34;             // 0x34
};

typedef void (*act_fn1)(void *self, s32 a);
typedef void (*act_fn2)(void *self, void *a, s32 b);

#define ACT_VCALL1(obj, m, a)                                                  \
    do                                                                         \
    {                                                                          \
        struct act_method *_m = &(obj)->vt->m;                                 \
        ((act_fn1)_m->fn)((u8 *)(obj) + _m->thisOffset, (s32)(a));             \
    } while (0)
#define ACT_VCALL2(obj, m, a, b)                                               \
    do                                                                         \
    {                                                                          \
        struct act_method *_m = &(obj)->vt->m;                                 \
        ((act_fn2)_m->fn)((u8 *)(obj) + _m->thisOffset, (void *)(a), (s32)(b)); \
    } while (0)

/* The same calls wrapped in `if (1) { ... } else (void)0` instead of
 * `do { ... } while (0)` (include/actor_self.h explains the difference).
 * agbcc treats the `do`/`while` as a loop, which keeps CSE from carrying
 * a constant from before the call to a store after it. Most handlers match
 * either way; sub_8013D94 needs the loop form and sub_8014D18 (and the
 * other handlers that keep a 1 in a callee-saved register across the
 * calls) needs this one. */
#define ACT_CALL1(obj, m, a)                                                   \
    if (1)                                                                     \
    {                                                                          \
        struct act_method *_m = &(obj)->vt->m;                                 \
        ((act_fn1)_m->fn)((u8 *)(obj) + _m->thisOffset, (s32)(a));             \
    } else (void)0
#define ACT_CALL2(obj, m, a, b)                                                \
    if (1)                                                                     \
    {                                                                          \
        struct act_method *_m = &(obj)->vt->m;                                 \
        ((act_fn2)_m->fn)((u8 *)(obj) + _m->thisOffset, (void *)(a), (s32)(b)); \
    } else (void)0

/* gKeys is the input word: low half held, high half newly
 * pressed. Handlers copy it to a stack slot and read the halves back from
 * there; the halves go through the local's address (a union or struct
 * member read is folded into a halfword load of the global itself). */
#define INPUT_HELD(in) (*(u16 *)&(in))
#define INPUT_PRESSED(in) (*(u16 *)((u8 *)&(in) + 2))

/* Byte read-modify-writes of part+0x0D, through a plain byte pointer: as
 * a struct member store, gcc's expansion leaves a dead `& 0` whose 0 CSE
 * then reuses for later zero stores, moving them (see
 * actor_part_18008.c). The mask arrives as an `s32` parameter so
 * old_agbcc materializes it before the load. */
#define ACT_PART_FLAGS0D(p) (*((u8 *)(p) + 0xD))

static inline void ActAndFlags0D(struct act_part *part, s32 mask)
{
    ACT_PART_FLAGS0D(part) &= mask;
}

static inline void ActOrFlags0D(struct act_part *part, s32 bits)
{
    ACT_PART_FLAGS0D(part) |= bits;
}

/* Queues action `next` on the +0x32/+0x30/+0x28 trio. As an inline
 * parameter, old_agbcc materializes `next` before the three stores, as
 * the ROM does. */
static inline void ActSetNext(struct act *self, s32 next)
{
    self->next32 = 0;
    self->flag30 = 1;
    self->next28 = next;
}

#endif // GUARD_ACTION_OBJ_H
