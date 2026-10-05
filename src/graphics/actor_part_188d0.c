#include "core.h"
#include "mover_new.h"
#include "gfx_part.h"

/* GitHub issue #23: 0x080188D0-0x0801967C, formerly
 * asm/code_3_2_17_188d0.s (details in docs/matching/issue-23-graphics.md).
 *
 * Small method-table ("vtable" at self+0x0C) objects of the same C++-style
 * family as actor_part_17524.c/actor_part27*.c: each class here is a
 * constructor (base InitCtrl/sub_8017A8C/CreatePlatformMover, then its own
 * table pointer) plus a destructor (table pointer, then the base
 * destructor), and a handful of per-frame update methods that drive one
 * "part" - a CreateMovingSprite-built on-screen object (struct gfx_part below)
 * whose animation tag/frame, mirror bit and flags they set
 * (include/gfx_part.h). Virtual calls
 * go through the _call_via_r2/AD84/AD88 call-via-register trampolines with
 * gcc 2.x's {this-adjust, fn} method entries.
 *
 * - UpdateCortexBoss/sub_8018BDC/sub_8018CB0: a two-part effect that spawns two
 *   child parts, aims them at each other and sinks off the bottom of the
 *   level.
 * - sub_8018D70/sub_8018E4C/sub_8019094/sub_8019214: a "mover" that glides
 *   its part between targets (sub_80196B8 sets the target, sub_8018E4C
 *   interpolates it), bouncing across the level in a height pattern chosen
 *   by the level config (gStaticData_0816C358-0816C362 per-config timings),
 *   and spawns hit effects (sub_8019214).
 * - sub_8019324: hit test of a part against the player and the
 *   gCollidableList list (sub_8007xxx boxes, AabbOverlaps overlap).
 * - CreateTiny: allocates a 257-entry table of i*i>>8 squares.
 *
 * Matching notes: this code materializes a byte-RMW's constant/mask
 * before loading the byte, and computes stored values before their
 * addresses, where plain C does the opposite - hence the small
 * barrier-carrying helpers (SetFrameNibble, CopyFlipX, ...), pins, and a
 * couple of per-site macros. Those were written against the current
 * agbcc; the ROM was built with the older compiler, and this file is
 * built with old_agbcc (Makefile OLD_AGBCC_OBJS,
 * docs/matching/old-agbcc-retry.md), under which UpdateCortexBoss and
 * sub_801961C match as C and several of the workarounds were dropped.
 *
 * UNUSED - no caller anywhere in the ROM (checked the asm/ and expected/
 * sources, every .c file under src/, and every word-aligned Thumb pointer
 * in baserom.gba): sub_8018948 (the gStaticData_087E44FC class's
 * constructor). Matched anyway. */

struct gfx_method
{
    s16 thisOffset;
    u8 unk_2[2];
    void *fn;
};

struct gfx_vtable
{
    u8 unk_00[0x18];
    struct gfx_method method_18; // 0x18 - "attach to part"
    struct gfx_method method_20; // 0x20 - "set state"
    u8 unk_28[0x28];
    struct gfx_method method_50; // 0x50
};


struct gfx_ctrl
{
    u8 unk_00[8];
    s32 state;                  // 0x08
    struct gfx_vtable *vtable;  // 0x0C
};

/* CreateTiny/DestroyTiny (vtable gTinyVtable) */
struct gfx_squares
{
    u8 unk_00[0xC];
    struct gfx_vtable *vtable;  // 0x0C
    u8 unk_10[0x14];
    s32 unk_24;                 // 0x24
    u8 unk_28[0x20];
    s16 *squares;               // 0x48
};

/* UpdateCortexBoss */
struct gfx_pair_ctrl
{
    u8 unk_00[8];
    s32 state;                  // 0x08
    struct gfx_vtable *vtable;  // 0x0C
    s32 counter;                // 0x10
    u8 unk_14[8];
    struct gfx_part *childA;    // 0x1C
    struct gfx_part *childB;    // 0x20
};

/* sub_8018978 */
struct gfx_offset_ctrl
{
    u8 unk_00[0x30];
    s32 x;      // 0x30
    s32 y;      // 0x34
    s32 unk_38; // 0x38
    s32 unk_3C; // 0x3C
    s32 dy;     // 0x40
    s32 dx;     // 0x44
};

struct gfx_level_cfg
{
    u8 unk_00[0x10];
    s32 index;  // 0x10
};

/* sub_8018E4C/sub_8019094/sub_8019214 */
struct gfx_mover
{
    u8 unk_00[8];
    s32 state;                  // 0x08
    struct gfx_vtable *vtable;  // 0x0C
    u8 dirLeft;                 // 0x10
    u8 high;                    // 0x11
    u8 top;                     // 0x12
    u8 unk_13;
    s32 targetX;                // 0x14
    s32 targetY;                // 0x18
    s32 deltaX;                 // 0x1C
    s32 deltaY;                 // 0x20
    s32 stepsLeft;              // 0x24
    s32 steps;                  // 0x28
    s32 nextState;              // 0x2C
    s32 timer;                  // 0x30
    s32 blink;                  // 0x34
    u8 blinking;                // 0x38
    u8 unk_39[3];
    struct gfx_level_cfg *cfg;  // 0x3C
};

/* sub_8019324 */
struct gfx_hit_ctrl
{
    u8 unk_00[0x10];
    u8 enabled;                 // 0x10
    u8 unk_11[3];
    struct gfx_ctrl *owner;     // 0x14
};

/* sub_80194E0 */
struct gfx_kind_ctrl
{
    u8 unk_00[8];
    s32 state;                  // 0x08
    struct gfx_vtable *vtable;  // 0x0C
    s32 kind;                   // 0x10
};

struct gfx_box
{
    s32 x;
    s32 y;
    s32 w;
    s32 h;
};

struct gfx_player
{
    s32 x;                      // 0x00
    s32 y;                      // 0x04
    u8 unk_08[0x10];
    u8 *vtable;                 // 0x18
    u8 unk_1C[0xE8];
    u8 dead;                     // 0x104
};

struct gfx_list
{
    u8 unk_00[4];
    s32 count;                  // 0x04
    u8 unk_08[4];
    struct gfx_part **items;    // 0x0C
};

struct gfx_level
{
    u8 unk_00[0x10];
    struct { u8 unk_00[0x10]; s32 width; s32 height; } *layer0;
};

extern void *gEntityFlags;
extern void *gAudioContext;
extern void *gLevelState;
extern u8 ***gSpriteBankSet;
extern struct gfx_player *gPlayer;
extern struct gfx_list *gCollidableList;
extern void *gUnknown_030012F4;
extern struct gfx_level *gLevelLayers;
extern u8 gStaticData_087E4494[];
extern u8 gStaticData_087E44FC[];
extern u8 gTinyVtable[];
extern u8 gStaticData_087E45CC[];
extern u8 gStaticData_087E4634[];
extern u8 gStaticData_087E469C[];
extern u8 gStaticData_0816C35C[];
extern u8 gStaticData_0816C35F[];
extern u8 gStaticData_0816C362[];

extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void DestroyCtrl(void *self, s32 flags);
extern void InitCtrl(void *self);
extern void sub_8017A78(void *self, s32 flags);
extern void *sub_8017A8C(void *self);
extern void OperatorDeleteArray(void *ptr);
extern void *OperatorNewArray(u32 size);
extern void *OperatorNew(u32 size);
extern struct gfx_part *CreateMovingSprite(u16 arg0, u16 arg1, u16 arg2, u16 arg3);
extern void ResetSpriteFrameTimer(void *part);
extern void ResetSpriteFrameIndex(void *part);
extern void SetSpriteAnimDone(void *part, u8 val);
extern s32 GetSpriteAnimPaletteSlot(void *part);
extern void AddToPartList(void *manager, void *value);
extern s32 _call_via_r2(void *self, s32 arg, void *fn);
extern s32 _call_via_r3(void *self, void *arg1, s32 arg2, void *fn);
extern void _call_via_r4(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern s32 __divsi3(s32 dividend, s32 divisor);
extern s32 __udivsi3(s32 value, s32 divisor);
extern u8 HasTurboRun(void *self);
extern void RequestRoomExit(void);
extern void *GetSpriteBodyBox(void *dest, void *pt);
extern void *GetSpriteAttackBox(void *dest, void *pt);
extern void *GetSpriteHitbox(void *dest, void *pt);
extern u8 AabbOverlaps(void *buf1, void *buf2);
extern void DestroyPlatformMover(void *self, s32 flags);
extern void *sub_8019758(void *mem);
extern void *sub_80196F8(void *mem, void *owner);
extern void sub_80196B8(void *self, struct gfx_part *part, s32 x, s32 y);
extern void sub_801967C(void *self, u8 flag);
extern void sub_8019770(void *self, struct gfx_part *part, s32 mode);

void sub_8018BDC(struct gfx_pair_ctrl *self, struct gfx_part *part);
void sub_8018CB0(struct gfx_pair_ctrl *self, struct gfx_part *part);
void sub_8019094(struct gfx_mover *self, struct gfx_part *part, s32 mode);
void sub_8019214(struct gfx_mover *self, struct gfx_part *part, s32 kind);
void *sub_80195EC(void *self, s32 kind);
void *sub_8019660(void *self, void *cfg);

#define CALL2(obj, m, a)                                                       \
    do                                                                         \
    {                                                                          \
        struct gfx_method *_m = &(obj)->vtable->m;                             \
        _call_via_r2((u8 *)(obj) + _m->thisOffset, (a), _m->fn);                \
    } while (0)
#define CALL3(obj, m, a, b)                                                    \
    do                                                                         \
    {                                                                          \
        struct gfx_method *_m = &(obj)->vtable->m;                             \
        _call_via_r3((u8 *)(obj) + _m->thisOffset, (a), (b), _m->fn);           \
    } while (0)

static inline void SetTag(struct gfx_part *part, s32 tag)
{
    part->tag = tag;
    ResetSpriteFrameTimer(part);
    ResetSpriteFrameIndex(part);
    SetSpriteAnimDone(part, 0);
}

/* Read-modify-write helpers for the byte-wide bitfields at +0x0C/+0x28/
 * +0x29. The ROM always materializes the mask/constant *before* loading
 * the byte it applies to. For the flags byte an inline helper taking the
 * constant as an `s32` parameter is enough under old_agbcc (the same
 * statement written in place loads the byte first); the nibble/flip
 * helpers still spell out their registers. */
static inline void AndFlags(struct gfx_part *part, s32 mask)
{
    PART_FLAGS(part) &= mask;
}

static inline void OrFlags(struct gfx_part *part, s32 bits)
{
    PART_FLAGS(part) |= bits;
}

static inline void SetFrameNibbleM(struct gfx_part *part, s32 v, s32 mask)
{
    register s32 val asm("r0") = v;
    register u8 *p asm("r2") = (u8 *)part + 0x29;
    register s32 m asm("r1");
    register s32 b asm("r3");

    val &= mask;
    asm volatile("mov %0, #0x10\n\tneg %0, %0" : "=r"(m));
    b = *p;
    m &= b;
    m |= val;
    *p = m;
}

static inline void SetFrameNibble(struct gfx_part *part, s32 v)
{
    register s32 val asm("r0") = v;
    register u8 *p asm("r2") = (u8 *)part + 0x29;
    register s32 m asm("r1");
    register s32 b asm("r3");

    val &= 0xF;
    asm volatile("mov %0, #0x10\n\tneg %0, %0" : "=r"(m));
    b = *p;
    m &= b;
    m |= val;
    *p = m;
}

static inline void CopyFlipX(struct gfx_part *dst, struct gfx_part *src)
{
    u32 sv = (u32)src + 0x28;
    register u8 *dp asm("r2") = (u8 *)dst + 0x28;
    register s32 bit asm("r1") = 0x10;
    register s32 m asm("r0");
    register s32 b asm("r3");

    asm("" : "+r"(bit));
    sv = *(u8 *)sv;
    bit &= sv;
    m = -0x11;
    asm("" : "+r"(m));
    b = *dp;
    m &= b;
    m |= bit;
    *dp = m;
}



static inline s32 Abs(s32 v)
{
    s32 sign = v >> 31;

    return (v ^ sign) - sign;
}

static inline void SetFrame(struct gfx_part *part, s32 frame)
{
    struct anim_bank *bank = part->bank;
    u8 *tag = &part->tag;
    struct anim_record *records = bank->records;
    s32 count = records[*tag].frameCount;

    if (frame >= count)
        frame = count - 1;
    part->frame = frame;
}

/* SetFrame with the ROM's register choice spelled out: the record table in
 * r1, the tag's address in r2 and the tag itself in R_TAG (a callee-saved
 * register the allocator reaches for because r0-r3 are all busy at that
 * point in the ROM's allocation). */
#define SET_FRAME_R(part, frameExpr, R_FRAME, R_TAG)                           \
    do                                                                         \
    {                                                                          \
        register s32 _frame asm(R_FRAME) = (frameExpr);                        \
        struct anim_bank *_bank = (part)->bank;                                \
        register u8 *_tagp asm("r2") = &(part)->tag;                           \
        register struct anim_record *_records asm("r1") = _bank->records;      \
        register u32 _tag asm(R_TAG) = *_tagp;                                 \
        s32 _count = _records[_tag].frameCount;                                \
                                                                               \
        if (_frame >= _count)                                                  \
            _frame = _count - 1;                                               \
        (part)->frame = _frame;                                                \
    } while (0)

/* "Mark part gone": set flags bit 0, then unless its id is 0xFFFF set the
 * id's bit in the gEntityFlags+0x108 bitmap - the same sequence as
 * MarkEntityGone (graphics.c) and sub_80178EC (actor_part_17524.c), inlined.
 * The id is re-read (`volatile`) after the 0xFFFF test, and the word index
 * is a *signed* division of that zero-extended value, which is what gives
 * the ROM's copy + `asr #5` + subtract. The register pins are
 * load-bearing (docs/workflow.md step 7) and differ per call site, so they
 * are macro parameters: the flags scratch register, the register the
 * first id read lands in, and the bitmap base. */
#define MARK_GONE(t, R_FLAGS, R_CUR, R_BASE)                                   \
    do                                                                         \
    {                                                                          \
        {                                                                      \
            register s32 _v asm("r0") = 1;                                     \
            register s32 _f asm(R_FLAGS) = PART_FLAGS(t);                      \
                                                                               \
            _v |= _f;                                                          \
            PART_FLAGS(t) = _v;                                                \
        }                                                                      \
        MARK_GONE_BITMAP(t, R_CUR, R_BASE);                                    \
    } while (0)

#define GONE_SLOT(slot, base) slot = (u32 *)((base) + 0x108)
/* sub_8019324: the ROM holds the 0x108 bitmap offset in r4, where the
 * allocator would otherwise pick r5 */
#define GONE_SLOT_R4(slot, base)                                               \
    {                                                                          \
        register s32 _k asm("r4") = 0x108;                                     \
                                                                               \
        asm("" : "+r"(_k));                                                    \
        slot = (u32 *)((base) + _k);                                           \
    }

#define MARK_GONE_BITMAP(t, R_CUR, R_BASE)                                     \
    MARK_GONE_BITMAP_OFF(t, R_CUR, R_BASE, GONE_SLOT)
#define MARK_GONE_BITMAP_R4(t, R_CUR, R_BASE)                                  \
    MARK_GONE_BITMAP_OFF(t, R_CUR, R_BASE, GONE_SLOT_R4)

#define MARK_GONE_BITMAP_OFF(t, R_CUR, R_BASE, OFFSET_STMT)                    \
    do                                                                         \
    {                                                                          \
        {                                                                      \
            register s32 _none asm("r0") = 0xFFFF;                             \
            register u32 _cur asm(R_CUR) = (t)->id;                            \
                                                                               \
            if (_cur != _none)                                                 \
            {                                                                  \
                register s32 _id asm("r3") = *(vu16 *)&(t)->id;                \
                register u8 *_base asm(R_BASE) = gEntityFlags;            \
                register s32 _word asm("r0") = _id;                            \
                s32 _off;                                                      \
                u32 *_slot;                                                    \
                                                                               \
                _word /= 32;                                                   \
                _off = _word * 4;                                              \
                OFFSET_STMT(_slot, _base);                                     \
                _slot = (u32 *)((u8 *)_slot + _off);                           \
                _word = _id - _word * 32;                                      \
                *_slot |= 1 << _word;                                          \
            }                                                                  \
        }                                                                      \
    } while (0)

void *sub_80188D0(struct gfx_ctrl *self)
{
    InitCtrl(self);
    self->vtable = (struct gfx_vtable *)gStaticData_087E4494;
    return self;
}

void sub_80188E8(struct gfx_ctrl *self, s32 flags)
{
    self->vtable = (struct gfx_vtable *)gStaticData_087E4494;
    DestroyCtrl(self, flags);
}

void sub_80188FC(struct gfx_ctrl *self, struct gfx_part *part)
{
    if (part->animDone)
        MARK_GONE(part, "r2", "r4", "r2");
}

/* UNUSED - see the top-of-file comment. */
void *sub_8018948(struct gfx_ctrl *self)
{
    InitCtrl(self);
    self->vtable = (struct gfx_vtable *)gStaticData_087E44FC;
    return self;
}

void sub_8018960(struct gfx_ctrl *self, s32 flags)
{
    self->vtable = (struct gfx_vtable *)gStaticData_087E44FC;
    DestroyCtrl(self, flags);
}

void nullsub_19(void)
{
}

void sub_8018978(struct gfx_offset_ctrl *self, struct gfx_part *part)
{
    s32 px = part->pos.x;

    if (self->x <= px)
    {
        u8 *p = (u8 *)part + 0x28;
        s32 m = -0x11;

        m &= *p;
        m |= 0x10;
        *p = m;
    }
    else
    {
        u8 *p = (u8 *)part + 0x28;
        s32 m = -0x11;

        m &= *p;
        *p = m;
    }
    self->unk_38 = 0x1A;
    self->unk_3C = 0x1A;
    self->dy = part->pos.y - self->y;
    self->dx = part->pos.x - self->x;
}

void DestroyTiny(struct gfx_squares *self, s32 flags)
{
    self->vtable = (struct gfx_vtable *)gTinyVtable;
    if (self->squares != NULL)
        OperatorDeleteArray(self->squares);
    sub_8017A78(self, flags);
}

void *CreateTiny(struct gfx_squares *self)
{
    s32 i;

    sub_8017A8C(self);
    self->vtable = (struct gfx_vtable *)gTinyVtable;
    self->unk_24 = -1;
    self->squares = OperatorNewArray(0x202);
    for (i = 0; i <= 0x100; i++)
        self->squares[i] = (i * i) >> 8;
    return self;
}

/* State machine for the two-part effect built by sub_8018BDC/sub_8018CB0:
 * state 0 spawns both children and moves to state 1; state 1 picks the
 * first child's animation tag from the second child's height, mirrors
 * both parts towards the second child and sets both parts' frame from
 * the horizontal distance (0-5, scaled by the level width); state 2
 * counts to 3 before moving on; state 3 sinks everything 0x80 per frame
 * until it leaves the bottom of the level, then signals RequestRoomExit.
 *
 * Parked as NAKED under the current agbcc (it put `self`/`part` in r6/r7
 * where the ROM uses r7 as scratch); matches unchanged under old_agbcc. */
void UpdateCortexBoss(struct gfx_pair_ctrl *self, struct gfx_part *part)
{
    switch (self->state)
    {
    case 0:
        sub_8018BDC(self, part);
        sub_8018CB0(self, part);
        AndFlags(part, -5);
        goto mode1;
    case 1:
    {
        s32 y = self->childB->pos.y;
        s32 n;

        if (y <= 0x5000)
            SetTag(self->childA, 5);
        else if (y <= 0x7800)
            SetTag(self->childA, 4);
        else
            SetTag(self->childA, 3);

        n = self->childB->pos.x - part->pos.x;
        part->flipX = n >= 0;
        self->childA->flipX = n >= 0;
        {
            s32 w = gLevelLayers->layer0->width << 8;

            n = __udivsi3(Abs(n) * 12, w);
        }
        if (n > 5)
            n = 5;
        n = 5 - n;
        SetFrame(part, n);
        SetFrame(self->childA, n);
        break;
    }
    case 2:
        if (++self->counter > 2)
        {
            sub_8019770(self, part, 3);
            break;
        }
    mode1:
        sub_8019770(self, part, 1);
        break;
    case 4:
        break;
    case 3:
        self->childA->pos.y += 0x80;
        part->pos.y += 0x80;
        if (part->pos.y >= (gLevelLayers->layer0->height << 8) + 0x4000)
        {
            if (HasTurboRun(gLevelState))
                RequestRoomExit();
            sub_8019770(self, part, 4);
        }
        break;
    }
}

void sub_8018BDC(struct gfx_pair_ctrl *self, struct gfx_part *part)
{
    struct gfx_part *c = CreateMovingSprite(0xFFFF, 0, 0, 0);
    struct gfx_ctrl *ctrl;

    c->bank = (struct anim_bank *)(**gSpriteBankSet + 0x27C);
    SetTag(c, 3);
    c->animating = 0;
    ctrl = sub_8019758(OperatorNew(0x10));
    SetFrameNibble(c, GetSpriteAnimPaletteSlot(c));
    c->ctrl = ctrl;
    _call_via_r2((u8 *)ctrl + ctrl->vtable->method_18.thisOffset, (s32)c, ctrl->vtable->method_18.fn);
    c->pos = part->pos;
    CopyFlipX(c, part);
    OrFlags(c, 0x10);
    AddToPartList(gUnknown_030012F4, c);
    self->childA = c;
}

void sub_8018CB0(struct gfx_pair_ctrl *self, struct gfx_part *part)
{
    struct gfx_part *c = CreateMovingSprite(0xFFFF, 0, 0, 0);
    struct gfx_ctrl *ctrl;
    s32 x, y;

    c->bank = (struct anim_bank *)(**gSpriteBankSet + 0x27C);
    {
        /* the ROM keeps 0xF in r5 across the calls and reuses it as the
         * frame-nibble mask below */
        register s32 t asm("r0") = 0xF;
        register s32 k asm("r5");

        asm("" : "+r"(t));
        {
            u8 *p = &c->tag;

            k = 0xF;
            asm("" : "+r"(k));
            *p = t;
        }
        ResetSpriteFrameTimer(c);
        ResetSpriteFrameIndex(c);
        SetSpriteAnimDone(c, 0);
        SetFrameNibbleM(c, GetSpriteAnimPaletteSlot(c), k);
    }
    ctrl = sub_80196F8(OperatorNew(0x40), self);
    c->ctrl = ctrl;
    _call_via_r2((u8 *)ctrl + ctrl->vtable->method_18.thisOffset, (s32)c, ctrl->vtable->method_18.fn);
    x = part->pos.x;
    y = part->pos.y;
    x += 0x2000;
    y -= 0x4000;
    c->pos.x = x;
    c->pos.y = y;
    OrFlags(c, 0x10);
    AddToPartList(gUnknown_030012F4, c);
    {
        register struct gfx_pair_ctrl *s asm("r2") = self;

        asm("" : "+r"(s));
        s->childB = c;
    }
}

void sub_8018D70(u32 a0, u16 a1, u16 a2, u16 a3, s32 kind)
{
    struct gfx_part *c = CreateMovingSprite(a0, a1, a2, a3);
    struct gfx_ctrl *ctrl;

    c->bank = (struct anim_bank *)(**gSpriteBankSet + 0x180);
    switch (kind)
    {
    case 0:
        SetTag(c, 3);
        break;
    case 1:
        SetTag(c, 2);
        break;
    case 2:
        SetTag(c, 0);
        break;
    }
    SetFrameNibble(c, GetSpriteAnimPaletteSlot(c));
    ctrl = sub_80195EC(OperatorNew(0x14), kind);
    c->ctrl = ctrl;
    _call_via_r2((u8 *)ctrl + ctrl->vtable->method_18.thisOffset, (s32)c, ctrl->vtable->method_18.fn);
    c->kind = 0;
    {
        s32 m = -5;
        m &= PART_FLAGS(c);
        PART_FLAGS(c) = m | 0x10;
    }
    AddToPartList(gCollidableList, c);
}

void sub_8018E4C(struct gfx_mover *self, struct gfx_part *partArg)
{
    /* pinned so `self` is left the ROM's r7 */
    register struct gfx_part *part asm("r6") = partArg;
    register s32 n asm("r5");

    if ((n = self->stepsLeft) != 0)
    {
        s32 steps;
        s32 t;
        s32 x, y;

        self->stepsLeft = --n;
        t = self->deltaX * n;
        steps = self->steps;
        x = self->targetX - __divsi3(t, steps);
        y = self->targetY - __divsi3(self->deltaY * n, steps);
        part->pos.x = x;
        part->pos.y = y;
    }

    switch (self->state)
    {
    case 0:
    {
        register s32 m asm("r0") = -5;
        register s32 b asm("r2");
        b = PART_FLAGS(part);
        m &= b;
        PART_FLAGS(part) = m;
        sub_8019094(self, part, 1);
        break;
    }
    case 1:
    case 2:
    case 6:
        if (self->stepsLeft != 0)
            break;
        goto next;
    case 3:
        self->nextState = 4;
        CALL3(self, method_50, part, 0x12);
        sub_8019094(self, part, 7);
        break;
    case 4:
        sub_8019214(self, part, 0);
        sub_8019094(self, part, 2);
        CALL3(self, method_50, part, 0xF);
        break;
    case 7:
        if (part->animDone)
        {
        next:
            sub_8019094(self, part, self->nextState);
        }
        break;
    case 5:
    {
        register u8 *blinking asm("r5");
        s32 left;

        {
            register u8 *bp asm("r0") = &self->blinking;
            register u32 on asm("r1") = *bp;

            asm("mov %0, %1" : "=l"(blinking) : "l"(bp));
            if (on && ++self->blink > 9)
            {
                self->blink = 0;
                SET_FRAME_R(part, part->frame ^ 1, "r3", "r4");
            }
        }
        if ((left = self->stepsLeft) != 0)
            break;
        if (--self->timer == 0)
        {
            sub_8019094(self, part, 1);
            sub_8019214(self, part, 1);
            break;
        }
        if (self->timer == gStaticData_0816C35F[self->cfg->index])
        {
            PlaySfx(gAudioContext, 0x5C, 0x100);
            part->animating = left;
            CALL3(self, method_50, part, 0x10);
            *blinking = 1;
            self->blink = left;
        }
        if (self->timer == gStaticData_0816C362[self->cfg->index])
        {
            part->animating = left;
            CALL3(self, method_50, part, 0x10);
            *blinking = left;
            SET_FRAME_R(part, 1, "r3", "r4");
        }
        sub_80196B8(self, part, gPlayer->x, gPlayer->y - 0xA00);
        {
            s32 i = self->cfg->index;

            self->stepsLeft = self->steps = gStaticData_0816C35C[i];
        }
        break;
    }
    case 8:
        self->nextState = 10;
        sub_8019094(self, part, 6);
        break;
    case 9:
        sub_8019094(self, part, 8);
        break;
    case 10:
        break;
    }
}

/* Advance the mover's height pattern for the level config's `index`:
 * 0 - high mirrors the horizontal direction, 1 - alternate high/low,
 * 2 - alternate high/low and flip `top` every second step.
 *
 * Written as the switch's compare tree by hand: the ROM loads the index
 * into r0, copies it to r1, runs the first three compares on r0 and the
 * `== 2` one on r1 - the shape an inlined call's parameter copy leaves -
 * which no plain `switch` reproduced (docs/workflow.md step 7). The
 * `1` constants are materialized before their byte loads, as everywhere
 * in this file. */
static inline void StepHeight(struct gfx_mover *self, s32 pattern)
{
    register s32 v asm("r0") = pattern;
    register s32 p asm("r1");

    asm("mov %0, %1" : "=l"(p) : "l"(v));
    if (v == 1)
        goto toggle;
    if (v > 1)
        goto above1;
    if (v == 0)
        goto mirror;
    return;
above1:
    if (p == 2)
        goto toggleTop;
    return;
mirror:
    self->high = self->dirLeft;
    return;
toggle:
    {
        register s32 one asm("r0") = 1;
        register s32 b asm("r1");

        asm("" : "+r"(one));
        b = self->high;
        one ^= b;
        self->high = one;
    }
    return;
toggleTop:
    {
        register s32 one asm("r3") = 1;
        s32 h;

        asm("" : "+r"(one));
        h = self->high ^ one;
        self->high = h;
        if (h == 0)
            self->top ^= one;
    }
}

void sub_8019094(struct gfx_mover *self, struct gfx_part *part, s32 mode)
{
    switch (mode)
    {
    case 8:
        sub_80196B8(self, part, (u32)(gLevelLayers->layer0->width << 8) >> 1,
                    (gLevelLayers->layer0->height << 8) + 0x2000);
        break;
    case 1:
    {
        s32 zero;

        sub_801967C(self, 0);
        {
            u8 *p = &part->animating;

            zero = 0;
            *p = mode;
        }
        CALL3(self, method_50, part, 0xF);
        self->dirLeft = mode;
        self->high = mode;
        self->top = zero;
    }
        sub_80196B8(self, part, (gLevelLayers->layer0->width << 8) - 0x400, 0x9800);
        self->nextState = 2;
        break;
    case 2:
    {
        s32 x, y;

        self->nextState = 3;
        x = self->targetX;
        if (self->dirLeft)
        {
            if (x - 0x1800 <= 0x400)
                self->dirLeft = 0;
        }
        else if (x + 0x1C00 >= gLevelLayers->layer0->width << 8)
        {
            self->nextState = 5;
        }
        StepHeight(self, self->cfg->index);
        if (self->dirLeft)
            x -= 0x1800;
        else
            x += 0x1800;
        if (self->high)
        {
            u8 top = self->top;

            y = 0x9800;
            if (top)
                y = 0x3E00;
        }
        else
        {
            y = 0x8200;
        }
        sub_80196B8(self, part, x, y);
        break;
    }
    case 5:
        self->blinking = 0;
        sub_801967C(self, 1);
        self->timer = 0x14;
        break;
    }
    CALL2(self, method_20, mode);
}

void sub_8019214(struct gfx_mover *self, struct gfx_part *partArg, s32 kindArg)
{
    /* pinned so `self` is left the ROM's r7 (see the file comment) */
    register struct gfx_part *part asm("r6") = partArg;
    register s32 kind asm("r5") = kindArg;
    struct gfx_part *c = CreateMovingSprite(0xFFFF, 0, 0, 0);
    struct { u8 unk_00[0xC]; struct gfx_vtable *vtable; u8 fast; } *ctrl;

    c->bank = (struct anim_bank *)(**gSpriteBankSet + 0x27C);
    switch (kind)
    {
    case 0:
        SetTag(c, 0xE);
        break;
    case 1:
        SetTag(c, 0x11);
        break;
    }
    SetFrameNibble(c, GetSpriteAnimPaletteSlot(c));
    ctrl = sub_8019660(OperatorNew(0x18), self->cfg);
    ctrl->fast = kind == 1;
    c->ctrl = ctrl;
    _call_via_r2((u8 *)ctrl + ctrl->vtable->method_18.thisOffset, (s32)c, ctrl->vtable->method_18.fn);
    c->pos = part->pos;
    {
        register s32 m asm("r0") = -5;
        register s32 b asm("r1");
        b = PART_FLAGS(c);
        m &= b;
        b = 1;
        c->kind = b;
        b = 0x10;
        m |= b;
        PART_FLAGS(c) = m;
    }
    AddToPartList(gUnknown_030012F4, c);
    if (kind == 1)
        PlaySfx(gAudioContext, 0x31, 0x100);
    else
        PlaySfx(gAudioContext, 0x32, 0x100);
}

void sub_8019324(struct gfx_hit_ctrl *self, struct gfx_part *partArg)
{
    struct gfx_part *part = partArg;
    struct gfx_box a;
    struct gfx_box b;
    struct gfx_box c;

    if (part->kind == 1)
    {
        GetSpriteBodyBox(&a, gPlayer);
        if (a.w == 0)
        {
            GetSpriteAttackBox(&b, gPlayer);
            a = b;
        }
        GetSpriteAttackBox(&b, part);
        if (AabbOverlaps(&a, &b))
        {
            struct gfx_player *p = gPlayer;

            if (p->dead == 0)
            {
                /* _call_via_r4 calls through r4: the method's function
                 * pointer is loaded there but never passed in r0-r3 (same
                 * idiom as actor_part78.c's _call_via_r4 calls) */
                u8 *tbl = p->vtable + 0x68;
                void *thisp = (u8 *)p + *(s16 *)tbl;
                register void *fn asm("r4") = *(void *volatile *)(tbl + 4);

                (void)fn;
                _call_via_r4(thisp, 0, 9, 0);
            }
            {
                register s32 zero asm("r0") = 0;
                register struct gfx_part *q asm("r4") = part;

                asm("" : "+r"(q));
                q->kind = zero;
            }
        }
        else if (self->enabled)
        {
            s32 n = gCollidableList->count;
            register s32 i asm("r5");

            for (i = 0; i < n; i++)
            {
                struct gfx_part *e = gCollidableList->items[i];

                GetSpriteHitbox(&c, e);
                if (AabbOverlaps(&c, &b))
                {
                    CALL2(self->owner, method_20, 2);
                    e->kind = 1;
                    {
                        register s32 zero asm("r2") = 0;
                        register struct gfx_part *q asm("r1") = part;

                        asm("" : "+r"(q));
                        q->kind = zero;
                    }
                }
            }
        }
    }
    if (part->animDone)
    {
        /* MARK_GONE, but `part` lives in r8 here: the flags byte is read
         * through an r3 copy and everything after through an r4 copy */
        register s32 v asm("r0") = 1;
        register struct gfx_part *t asm("r4");

        {
            register u32 q asm("r3") = (u32)part;

            asm("" : "+r"(q));
            q = PART_FLAGS((struct gfx_part *)q);
            v |= q;
        }
        t = part;
        asm("" : "+r"(t));
        PART_FLAGS(t) = v;
        MARK_GONE_BITMAP_R4(t, "r5", "r2");
    }
}

void sub_8019464(struct gfx_ctrl *self, struct gfx_part *part)
{
    s32 target;

    if (self->state == 0)
    {
        SET_FRAME_R(part, 0x1A, "r4", "r6");
        self->state = 1;
    }
    target = 0x1A;
    if (part->kind == 1)
        target = 10;
    if (part->frame == target)
    {
        part->animating = 0;
    }
    else
    {
        register s32 one asm("r1") = 1;
        register u8 *p asm("r0") = &part->animating;

        *p = one;
        p += 0x38 - 0x2C;
        asm("" : "+r"(p));
        if (*p)
            SET_FRAME_R(part, 0, "r4", "r5");
    }
}

void sub_80194E0(struct gfx_kind_ctrl *self, struct gfx_part *partArg)
{
    struct gfx_part *part = partArg;

    switch (self->state)
    {
    case 0:
        if (part->kind == 1)
        {
            part->bank = (struct anim_bank *)(**gSpriteBankSet + 0x27C);
            switch (self->kind)
            {
            case 0:
                CALL3(self, method_50, part, 0xC);
                break;
            case 1:
                CALL3(self, method_50, part, 0xB);
                break;
            case 2:
                CALL3(self, method_50, part, 0xD);
                break;
            }
            SetFrameNibble(part, GetSpriteAnimPaletteSlot(part));
            CALL2(self, method_20, 1);
        }
        break;
    case 1:
        if (part->animDone)
            MARK_GONE(part, "r1", "r2", "r1");
        break;
    }
}

void sub_80195D8(struct gfx_ctrl *self, s32 flags)
{
    self->vtable = (struct gfx_vtable *)gStaticData_087E45CC;
    DestroyCtrl(self, flags);
}

void *sub_80195EC(void *selfArg, s32 kind)
{
    struct gfx_kind_ctrl *self = selfArg;

    InitCtrl(self);
    self->vtable = (struct gfx_vtable *)gStaticData_087E45CC;
    self->kind = kind;
    return self;
}

void sub_8019608(struct gfx_ctrl *self, s32 flags)
{
    self->vtable = (struct gfx_vtable *)gStaticData_087E4634;
    DestroyPlatformMover(self, flags);
}

/* Constructor: base-constructs through CreatePlatformMover(self, 0, 0, 0, {0}, 6)
 * and points the method table at gStaticData_087E4634.
 *
 * The fifth argument is a byte the ROM stores with `strb` into its
 * outgoing stack slot; like CreatePlatform (actor_part_1a878.c), the call
 * writes both stack slots itself through MOVER_NEW's 4-argument view
 * (include/mover_new.h), which also gives the ROM's `mov r1, sp` before
 * the 0. */
void *sub_801961C(struct gfx_ctrl *self)
{
    struct mover_stack_args args;

    *(volatile u8 *)&args.dirY = 0;
    *(volatile s32 *)&args.kind = 6;
    MOVER_NEW(self, 0, 0, 0);
    self->vtable = (struct gfx_vtable *)gStaticData_087E4634;
    return self;
}

void sub_801964C(struct gfx_ctrl *self, s32 flags)
{
    self->vtable = (struct gfx_vtable *)gStaticData_087E469C;
    DestroyCtrl(self, flags);
}

void *sub_8019660(void *selfArg, void *cfg)
{
    struct { u8 unk_00[0xC]; struct gfx_vtable *vtable; u8 unk_10[4]; void *cfg; } *self = selfArg;

    InitCtrl(self);
    self->vtable = (struct gfx_vtable *)gStaticData_087E469C;
    self->cfg = cfg;
    return self;
}
