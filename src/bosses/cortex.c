#include "core.h"
#include "match.h"
#include "mover_new.h"
#include "gfx_part.h"
#include "util.h"
#include <libgcc.h>
#include "audio.h"
#include "player.h"
#include "bosses.h"
#include "objects.h"
#include "memory.h"
#include "level.h"
#include "box_part.h"
#include "globals.h"

/* GitHub issue #23: 0x080188D0-0x0801967C, formerly
 * asm/code_3_2_17_188d0.s (details in docs/matching/archive/issue-23-graphics.md).
 *
 * Small method-table ("vtable" at self+0x0C) objects of the same C++-style
 * family as input_ctrl.c/input_ctrl_queue.c/mega_mix.c: each class here is a
 * constructor (base InitCtrl/CreateBossCtrl/CreatePlatformMover, then its own
 * table pointer) plus a destructor (table pointer, then the base
 * destructor), and a handful of per-frame update methods that drive one
 * "part" - a CreateMovingSprite-built on-screen object (struct gfx_part below)
 * whose animation tag/frame, mirror bit and flags they set
 * (include/gfx_part.h). Virtual calls
 * go through the _call_via_r2/AD84/AD88 call-via-register trampolines with
 * gcc 2.x's {this-adjust, fn} method entries.
 *
 * Most of these classes are pieces of the Neo Cortex fight (sprite bank
 * 53: the Tesla cannon at three angles in anims 3-5, the green/red
 * crosshairs in 0xF/0x10/0x12, the shots in 0xE/0x11 and the shrinking
 * gems in 9-0xD): the cannon (SpawnCortexCannon) aims at a crosshair
 * (SpawnCortexTarget, UpdateCortexTarget) that hops across the level and
 * then chases the player, firing a shot at each stop (FireCortexShot,
 * UpdateCortexShot). In this fight the red/green/yellow gem spawners
 * hand over to SpawnCortexBossGem (bank 32's gems), which a ring shot
 * shrinks away.
 *
 * - UpdateCortexBoss/SpawnCortexCannon/SpawnCortexTarget: a two-part effect that spawns two
 *   child parts, aims them at each other and sinks off the bottom of the
 *   level.
 * - SpawnCortexBossGem/UpdateCortexTarget/SetCortexTargetState/FireCortexShot: a "mover" that glides
 *   its part between targets (SetCortexTargetDest sets the target, UpdateCortexTarget
 *   interpolates it), bouncing across the level in a height pattern chosen
 *   by the level config (gCortexTargetHopSteps-0816C362 per-config timings),
 *   and spawns hit effects (FireCortexShot).
 * - UpdateCortexShot: hit test of a part against the player and the
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
 * docs/matching/archive/old-agbcc-retry.md), under which UpdateCortexBoss and
 * CreateCortexBossPlatformMover match as C and several of the workarounds were dropped.
 *
 * UNUSED - no caller anywhere in the ROM (checked the asm/ and expected/
 * sources, every .c file under src/, and every word-aligned Thumb pointer
 * in baserom.gba): CreateUnusedOneShotAnimCtrl (the gUnusedOneShotAnimCtrlVtable class's
 * constructor). Matched anyway. */

/* `struct gfx_ctrl` and its `struct gfx_vtable` are in bosses.h. */

/* CreateTiny/DestroyTiny (vtable gTinyVtable) */
struct gfx_squares {
    u8 unk_00[0xC];
    struct gfx_vtable *vtable; // 0x0C
    u8 unk_10[0x14];
    s32 stomped; // 0x24 - struct tiny_tiger's `stomped`
    u8 unk_28[0x20];
    s16 *squares; // 0x48
};

/* UpdateCortexBoss */
struct gfx_pair_ctrl {
    u8 unk_00[8];
    s32 state;                 // 0x08
    struct gfx_vtable *vtable; // 0x0C
    s32 counter;               // 0x10
    u8 unk_14[8];
    struct gfx_part *childA; // 0x1C
    struct gfx_part *childB; // 0x20
};

/* StartTinyHop */
struct gfx_offset_ctrl {
    u8 unk_00[0x30];
    s32 x;     // 0x30
    s32 y;     // 0x34
    s32 steps; // 0x38 - struct tiny_tiger's `steps`/`total`
    s32 total; // 0x3C
    s32 dy;    // 0x40
    s32 dx;    // 0x44
};

struct gfx_level_cfg {
    u8 unk_00[0x10];
    s32 index; // 0x10
};

/* UpdateCortexTarget/SetCortexTargetState/FireCortexShot */
struct gfx_mover {
    u8 unk_00[8];
    s32 state;                 // 0x08
    struct gfx_vtable *vtable; // 0x0C
    u8 dirLeft;                // 0x10
    u8 high;                   // 0x11
    u8 top;                    // 0x12
    u8 unk_13;
    s32 targetX;   // 0x14
    s32 targetY;   // 0x18
    s32 deltaX;    // 0x1C
    s32 deltaY;    // 0x20
    s32 stepsLeft; // 0x24
    s32 steps;     // 0x28
    s32 nextState; // 0x2C
    s32 timer;     // 0x30
    s32 blink;     // 0x34
    u8 blinking;   // 0x38
    u8 unk_39[3];
    struct gfx_level_cfg *cfg; // 0x3C
};

/* UpdateCortexShot */
struct gfx_hit_ctrl {
    u8 unk_00[0x10];
    u8 enabled; // 0x10
    u8 unk_11[3];
    struct gfx_ctrl *owner; // 0x14
};

/* UpdateCortexBossGem */
struct gfx_kind_ctrl {
    u8 unk_00[8];
    s32 state;                 // 0x08
    struct gfx_vtable *vtable; // 0x0C
    s32 kind;                  // 0x10
};

extern s32 _call_via_r2(void *self, s32 arg, void *fn);
extern s32 _call_via_r3(void *self, void *arg1, s32 arg2, void *fn);
extern void _call_via_r4(void *arg0, s32 arg1, s32 arg2, s32 arg3);

#define CALL2(obj, m, a)                                                       \
    do                                                                         \
    {                                                                          \
        struct actor_method *_m = &(obj)->vtable->m;                             \
        _call_via_r2((u8 *)(obj) + _m->thisOffset, (a), _m->fn);                \
    } while (0)
#define CALL3(obj, m, a, b)                                                    \
    do                                                                         \
    {                                                                          \
        struct actor_method *_m = &(obj)->vtable->m;                             \
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
    MATCH_HOLD_REG(s32, val, r0) = v;
    MATCH_HOLD_REG(u8 *, p, r2) = (u8 *)part + 0x29;
    MATCH_HOLD_REG(s32, m, r1);
    MATCH_HOLD_REG(s32, b, r3);

    val &= mask;
    asm volatile("mov %0, #0x10\n\tneg %0, %0" : "=r"(m));
    b = *p;
    m &= b;
    m |= val;
    *p = m;
}

static inline void SetFrameNibble(struct gfx_part *part, s32 v)
{
    MATCH_HOLD_REG(s32, val, r0) = v;
    MATCH_HOLD_REG(u8 *, p, r2) = (u8 *)part + 0x29;
    MATCH_HOLD_REG(s32, m, r1);
    MATCH_HOLD_REG(s32, b, r3);

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
    MATCH_HOLD_REG(u8 *, dp, r2) = (u8 *)dst + 0x28;
    MATCH_HOLD_REG(s32, bit, r1) = 0x10;
    MATCH_HOLD_REG(s32, m, r0);
    MATCH_HOLD_REG(s32, b, r3);

    MATCH_KEEP(bit);
    sv = *(u8 *)sv;
    bit &= sv;
    m = -0x11;
    MATCH_KEEP(m);
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
        MATCH_HOLD_REG(u8 *, _tagp, r2) = &(part)->tag;                        \
        MATCH_HOLD_REG(struct anim_record *, _records, r1) = _bank->records;   \
        register u32 _tag asm(R_TAG) = *_tagp;                                 \
        s32 _count = _records[_tag].frameCount;                                \
                                                                               \
        if (_frame >= _count)                                                  \
            _frame = _count - 1;                                               \
        (part)->frame = _frame;                                                \
    } while (0)

/* "Mark part gone": set flags bit 0, then unless its id is 0xFFFF set the
 * id's bit in the gEntityFlags+0x108 bitmap - the same sequence as
 * MarkEntityGone (graphics.c) and InputCtrlStateDead (input_ctrl.c), inlined.
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
            MATCH_HOLD_REG(s32, _v, r0) = 1;                                   \
            register s32 _f asm(R_FLAGS) = PART_FLAGS(t);                      \
                                                                               \
            _v |= _f;                                                          \
            PART_FLAGS(t) = _v;                                                \
        }                                                                      \
        MARK_GONE_BITMAP(t, R_CUR, R_BASE);                                    \
    } while (0)

#define GONE_SLOT(slot, base) slot = (u32 *)((base) + 0x108)
/* UpdateCortexShot: the ROM holds the 0x108 bitmap offset in r4, where the
 * allocator would otherwise pick r5 */
#define GONE_SLOT_R4(slot, base)                                               \
    {                                                                          \
        MATCH_HOLD_REG(s32, _k, r4) = 0x108;                                   \
                                                                               \
        MATCH_KEEP(_k);                                                        \
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
            MATCH_HOLD_REG(s32, _none, r0) = 0xFFFF;                           \
            register u32 _cur asm(R_CUR) = (t)->id;                            \
                                                                               \
            if (_cur != _none)                                                 \
            {                                                                  \
                MATCH_HOLD_REG(s32, _id, r3) = *(vu16 *)&(t)->id;              \
                register u8 *_base asm(R_BASE) = (u8 *)gEntityFlags;     \
                MATCH_HOLD_REG(s32, _word, r0) = _id;                          \
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

void *CreateOneShotAnimCtrl(struct gfx_ctrl *self)
{
    InitCtrl(self);
    self->vtable = (struct gfx_vtable *)gOneShotAnimCtrlVtable;
    return self;
}

void DestroyOneShotAnimCtrl(struct gfx_ctrl *self, s32 flags)
{
    self->vtable = (struct gfx_vtable *)gOneShotAnimCtrlVtable;
    DestroyCtrl(self, flags);
}

/* gUnusedOneShotAnimCtrlVtable's class does what gOneShotAnimCtrlVtable's
 * does (UpdateOneShotAnimCtrl, tiny_hop_pad.c): once the part's animation
 * is done, mark it gone. Its constructor has no caller, so it is never
 * instantiated. */
void UpdateUnusedOneShotAnimCtrl(struct gfx_ctrl *self, struct gfx_part *part)
{
    if (part->animDone)
        MARK_GONE(part, "r2", "r4", "r2");
}

/* UNUSED - see the top-of-file comment. */
void *CreateUnusedOneShotAnimCtrl(struct gfx_ctrl *self)
{
    InitCtrl(self);
    self->vtable = (struct gfx_vtable *)gUnusedOneShotAnimCtrlVtable;
    return self;
}

void DestroyUnusedOneShotAnimCtrl(struct gfx_ctrl *self, s32 flags)
{
    self->vtable = (struct gfx_vtable *)gUnusedOneShotAnimCtrlVtable;
    DestroyCtrl(self, flags);
}

/* Empty. SetTinyState's case 9 (tiny_update.c) calls it directly, between
 * the hop set-up and the anim-7 call, with the same (self, part) arguments
 * as StartTinyHop below; it is in no method table, so nothing shows what
 * it was for. */
void nullsub_19(void *self, void *part)
{
}

void StartTinyHop(struct gfx_offset_ctrl *self, struct gfx_part *part)
{
    s32 px = part->pos.x;

    if (self->x <= px) {
        u8 *p = (u8 *)part + 0x28;
        s32 m = -0x11;

        m &= *p;
        m |= 0x10;
        *p = m;
    } else {
        u8 *p = (u8 *)part + 0x28;
        s32 m = -0x11;

        m &= *p;
        *p = m;
    }
    self->steps = 0x1A;
    self->total = 0x1A;
    self->dy = part->pos.y - self->y;
    self->dx = part->pos.x - self->x;
}

void DestroyTiny(struct gfx_squares *self, s32 flags)
{
    self->vtable = (struct gfx_vtable *)gTinyVtable;
    if (self->squares != NULL)
        OperatorDeleteArray(self->squares);
    DestroyBossCtrl((struct boss_ctrl *)self, flags);
}

void *CreateTiny(struct gfx_squares *self)
{
    s32 i;

    CreateBossCtrl((struct boss_ctrl *)self);
    self->vtable = (struct gfx_vtable *)gTinyVtable;
    self->stomped = -1;
    self->squares = OperatorNewArray(0x202);
    for (i = 0; i <= 0x100; i++)
        self->squares[i] = (i * i) >> 8;
    return self;
}

/* State machine for the two-part effect built by SpawnCortexCannon/SpawnCortexTarget:
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
    switch (self->state) {
    case 0:
        SpawnCortexCannon(self, part);
        SpawnCortexTarget(self, part);
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
                s32 w = gLevelLayers->layer0->widthPx << 8;

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
        if (++self->counter > 2) {
            SetCortexBossState((struct obj_476c *)self, (s32)part, 3);
            break;
        }
    mode1:
        SetCortexBossState((struct obj_476c *)self, (s32)part, 1);
        break;
    case 4:
        break;
    case 3:
        self->childA->pos.y += 0x80;
        part->pos.y += 0x80;
        if (part->pos.y >= (gLevelLayers->layer0->heightPx << 8) + 0x4000) {
            if ((u8)HasTurboRun(gLevelState))
                RequestRoomExit();
            SetCortexBossState((struct obj_476c *)self, (s32)part, 4);
        }
        break;
    }
}

void SpawnCortexCannon(struct gfx_pair_ctrl *self, struct gfx_part *part)
{
    struct gfx_part *c = CreateMovingSprite(0xFFFF, 0, 0, 0);
    struct gfx_ctrl *ctrl;

    c->bank = (struct anim_bank *)(SPRITE_BANK_BASE + 0x27C);
    SetTag(c, 3);
    c->animating = 0;
    ctrl = (struct gfx_ctrl *)CreateCortexCannonCtrl(OperatorNew(0x10));
    SetFrameNibble(c, GetSpriteAnimPaletteSlot((struct actor *)c));
    c->ctrl = ctrl;
    _call_via_r2((u8 *)ctrl + ctrl->vtable->method_18.thisOffset, (s32)c,
                 ctrl->vtable->method_18.fn);
    c->pos = part->pos;
    CopyFlipX(c, part);
    OrFlags(c, 0x10);
    AddToPartList(gForegroundList, c);
    self->childA = c;
}

void SpawnCortexTarget(struct gfx_pair_ctrl *self, struct gfx_part *part)
{
    struct gfx_part *c = CreateMovingSprite(0xFFFF, 0, 0, 0);
    struct gfx_ctrl *ctrl;
    s32 x, y;

    c->bank = (struct anim_bank *)(SPRITE_BANK_BASE + 0x27C);
    {
        /* the ROM keeps 0xF in r5 across the calls and reuses it as the
         * frame-nibble mask below */
        MATCH_HOLD_REG(s32, t, r0) = 0xF;
        MATCH_HOLD_REG(s32, k, r5);

        MATCH_KEEP(t);
        {
            u8 *p = &c->tag;

            k = 0xF;
            MATCH_KEEP(k);
            *p = t;
        }
        ResetSpriteFrameTimer(c);
        ResetSpriteFrameIndex(c);
        SetSpriteAnimDone(c, 0);
        SetFrameNibbleM(c, GetSpriteAnimPaletteSlot((struct actor *)c), k);
    }
    ctrl = (struct gfx_ctrl *)CreateCortexTargetCtrl(OperatorNew(0x40), self);
    c->ctrl = ctrl;
    _call_via_r2((u8 *)ctrl + ctrl->vtable->method_18.thisOffset, (s32)c,
                 ctrl->vtable->method_18.fn);
    x = part->pos.x;
    y = part->pos.y;
    x += 0x2000;
    y -= 0x4000;
    c->pos.x = x;
    c->pos.y = y;
    OrFlags(c, 0x10);
    AddToPartList(gForegroundList, c);
    {
        MATCH_HOLD_REG(struct gfx_pair_ctrl *, s, r2) = self;

        MATCH_KEEP(s);
        s->childB = c;
    }
}

void SpawnCortexBossGem(u32 a0, u16 a1, u16 a2, u16 a3, s32 kind)
{
    struct gfx_part *c = CreateMovingSprite(a0, a1, a2, a3);
    struct gfx_ctrl *ctrl;

    c->bank = (struct anim_bank *)(SPRITE_BANK_BASE + 0x180);
    switch (kind) {
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
    SetFrameNibble(c, GetSpriteAnimPaletteSlot((struct actor *)c));
    ctrl = CreateCortexBossGemCtrl(OperatorNew(0x14), kind);
    c->ctrl = ctrl;
    _call_via_r2((u8 *)ctrl + ctrl->vtable->method_18.thisOffset, (s32)c,
                 ctrl->vtable->method_18.fn);
    c->kind = 0;
    {
        s32 m = -5;
        m &= PART_FLAGS(c);
        PART_FLAGS(c) = m | 0x10;
    }
    AddToPartList(gCollidableList, c);
}

void UpdateCortexTarget(struct gfx_mover *self, struct gfx_part *partArg)
{
    /* pinned so `self` is left the ROM's r7 */
    MATCH_HOLD_REG(struct gfx_part *, part, r6) = partArg;
    MATCH_HOLD_REG(s32, n, r5);

    if ((n = self->stepsLeft) != 0) {
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

    switch (self->state) {
    case 0:
        {
            MATCH_HOLD_REG(s32, m, r0) = -5;
            MATCH_HOLD_REG(s32, b, r2);
            b = PART_FLAGS(part);
            m &= b;
            PART_FLAGS(part) = m;
            SetCortexTargetState(self, part, 1);
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
        SetCortexTargetState(self, part, 7);
        break;
    case 4:
        FireCortexShot(self, part, 0);
        SetCortexTargetState(self, part, 2);
        CALL3(self, method_50, part, 0xF);
        break;
    case 7:
        if (part->animDone) {
        next:
            SetCortexTargetState(self, part, self->nextState);
        }
        break;
    case 5:
        {
            MATCH_HOLD_REG(u8 *, blinking, r5);
            s32 left;

            {
                MATCH_HOLD_REG(u8 *, bp, r0) = &self->blinking;
                MATCH_HOLD_REG(u32, on, r1) = *bp;

                asm("mov %0, %1" : "=l"(blinking) : "l"(bp));
                if (on && ++self->blink > 9) {
                    self->blink = 0;
                    SET_FRAME_R(part, part->frame ^ 1, "r3", "r4");
                }
            }
            if ((left = self->stepsLeft) != 0)
                break;
            if (--self->timer == 0) {
                SetCortexTargetState(self, part, 1);
                FireCortexShot(self, part, 1);
                break;
            }
            if (self->timer == gCortexTargetBlinkStartTimes[self->cfg->index]) {
                PlaySfx(gAudioContext, 0x5C, 0x100);
                part->animating = left;
                CALL3(self, method_50, part, 0x10);
                *blinking = 1;
                self->blink = left;
            }
            if (self->timer == gCortexTargetBlinkStopTimes[self->cfg->index]) {
                part->animating = left;
                CALL3(self, method_50, part, 0x10);
                *blinking = left;
                SET_FRAME_R(part, 1, "r3", "r4");
            }
            SetCortexTargetDest((struct obj_4704 *)self, (s32 *)part, gPlayer->x,
                                gPlayer->y - 0xA00);
            {
                s32 i = self->cfg->index;

                self->stepsLeft = self->steps = gCortexTargetChaseSteps[i];
            }
            break;
        }
    case 8:
        self->nextState = 10;
        SetCortexTargetState(self, part, 6);
        break;
    case 9:
        SetCortexTargetState(self, part, 8);
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
    MATCH_HOLD_REG(s32, v, r0) = pattern;
    MATCH_HOLD_REG(s32, p, r1);

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
        MATCH_HOLD_REG(s32, one, r0) = 1;
        MATCH_HOLD_REG(s32, b, r1);

        MATCH_KEEP(one);
        b = self->high;
        one ^= b;
        self->high = one;
    }
    return;
toggleTop:
    {
        MATCH_HOLD_REG(s32, one, r3) = 1;
        s32 h;

        MATCH_KEEP(one);
        h = self->high ^ one;
        self->high = h;
        if (h == 0)
            self->top ^= one;
    }
}

void SetCortexTargetState(struct gfx_mover *self, struct gfx_part *part, s32 mode)
{
    switch (mode) {
    case 8:
        SetCortexTargetDest((struct obj_4704 *)self, (s32 *)part,
                            (u32)(gLevelLayers->layer0->widthPx << 8) >> 1,
                            (gLevelLayers->layer0->heightPx << 8) + 0x2000);
        break;
    case 1:
        {
            s32 zero;

            SetCortexPlatformsKind(self, 0);
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
        SetCortexTargetDest((struct obj_4704 *)self, (s32 *)part,
                            (gLevelLayers->layer0->widthPx << 8) - 0x400, 0x9800);
        self->nextState = 2;
        break;
    case 2:
        {
            s32 x, y;

            self->nextState = 3;
            x = self->targetX;
            if (self->dirLeft) {
                if (x - 0x1800 <= 0x400)
                    self->dirLeft = 0;
            } else if (x + 0x1C00 >= gLevelLayers->layer0->widthPx << 8) {
                self->nextState = 5;
            }
            StepHeight(self, self->cfg->index);
            if (self->dirLeft)
                x -= 0x1800;
            else
                x += 0x1800;
            if (self->high) {
                u8 top = self->top;

                y = 0x9800;
                if (top)
                    y = 0x3E00;
            } else {
                y = 0x8200;
            }
            SetCortexTargetDest((struct obj_4704 *)self, (s32 *)part, x, y);
            break;
        }
    case 5:
        self->blinking = 0;
        SetCortexPlatformsKind(self, 1);
        self->timer = 0x14;
        break;
    }
    CALL2(self, method_20, mode);
}

void FireCortexShot(struct gfx_mover *self, struct gfx_part *partArg, s32 kindArg)
{
    /* pinned so `self` is left the ROM's r7 (see the file comment) */
    MATCH_HOLD_REG(struct gfx_part *, part, r6) = partArg;
    MATCH_HOLD_REG(s32, kind, r5) = kindArg;
    struct gfx_part *c = CreateMovingSprite(0xFFFF, 0, 0, 0);
    struct {
        u8 unk_00[0xC];
        struct gfx_vtable *vtable;
        u8 fast;
    } *ctrl;

    c->bank = (struct anim_bank *)(SPRITE_BANK_BASE + 0x27C);
    switch (kind) {
    case 0:
        SetTag(c, 0xE);
        break;
    case 1:
        SetTag(c, 0x11);
        break;
    }
    SetFrameNibble(c, GetSpriteAnimPaletteSlot((struct actor *)c));
    ctrl = CreateCortexShotCtrl(OperatorNew(0x18), self->cfg);
    ctrl->fast = kind == 1;
    c->ctrl = ctrl;
    _call_via_r2((u8 *)ctrl + ctrl->vtable->method_18.thisOffset, (s32)c,
                 ctrl->vtable->method_18.fn);
    c->pos = part->pos;
    {
        MATCH_HOLD_REG(s32, m, r0) = -5;
        MATCH_HOLD_REG(s32, b, r1);
        b = PART_FLAGS(c);
        m &= b;
        b = 1;
        c->kind = b;
        b = 0x10;
        m |= b;
        PART_FLAGS(c) = m;
    }
    AddToPartList(gForegroundList, c);
    if (kind == 1)
        PlaySfx(gAudioContext, 0x31, 0x100);
    else
        PlaySfx(gAudioContext, 0x32, 0x100);
}

void UpdateCortexShot(struct gfx_hit_ctrl *self, struct gfx_part *partArg)
{
    struct gfx_part *part = partArg;
    struct aabb a;
    struct aabb b;
    struct aabb c;

    if (part->kind == 1) {
        GetSpriteBodyBox(&a, gPlayer);
        if (a.w == 0) {
            GetSpriteAttackBox(&b, gPlayer);
            a = b;
        }
        GetSpriteAttackBox(&b, part);
        if (AabbOverlaps(&a, &b)) {
            struct player *p = gPlayer;

            if (p->dead == 0) {
                /* _call_via_r4 calls through r4: the method's function
                 * pointer is loaded there but never passed in r0-r3 (same
                 * idiom as player_collide.c's _call_via_r4 calls) */
                const struct actor_method *tbl = &p->vtable->handleEvent;
                void *thisp = (u8 *)p + tbl->thisOffset;
                MATCH_HOLD_REG(void *, fn, r4) = *(void *const volatile *)&tbl->fn;

                (void)fn;
                _call_via_r4(thisp, 0, 9, 0);
            }
            {
                MATCH_HOLD_REG(s32, zero, r0) = 0;
                MATCH_HOLD_REG(struct gfx_part *, q, r4) = part;

                MATCH_KEEP(q);
                q->kind = zero;
            }
        } else if (self->enabled) {
            s32 n = gCollidableList->count;
            MATCH_HOLD_REG(s32, i, r5);

            for (i = 0; i < n; i++) {
                struct gfx_part *e = (struct gfx_part *)gCollidableList->items[i];

                c = GetSpriteHitbox((struct box_part *)e);
                if (AabbOverlaps(&c, &b)) {
                    CALL2(self->owner, method_20, 2);
                    e->kind = 1;
                    {
                        MATCH_HOLD_REG(s32, zero, r2) = 0;
                        MATCH_HOLD_REG(struct gfx_part *, q, r1) = part;

                        MATCH_KEEP(q);
                        q->kind = zero;
                    }
                }
            }
        }
    }
    if (part->animDone) {
        /* MARK_GONE, but `part` lives in r8 here: the flags byte is read
         * through an r3 copy and everything after through an r4 copy */
        MATCH_HOLD_REG(s32, v, r0) = 1;
        MATCH_HOLD_REG(struct gfx_part *, t, r4);

        {
            MATCH_HOLD_REG(u32, q, r3) = (u32)part;

            MATCH_KEEP(q);
            q = PART_FLAGS((struct gfx_part *)q);
            v |= q;
        }
        t = part;
        MATCH_KEEP(t);
        PART_FLAGS(t) = v;
        MARK_GONE_BITMAP_R4(t, "r5", "r2");
    }
}

void UpdateCortexBossPlatformMover(struct gfx_ctrl *self, struct gfx_part *part)
{
    s32 target;

    if (self->state == 0) {
        SET_FRAME_R(part, 0x1A, "r4", "r6");
        self->state = 1;
    }
    target = 0x1A;
    if (part->kind == 1)
        target = 10;
    if (part->frame == target) {
        part->animating = 0;
    } else {
        MATCH_HOLD_REG(s32, one, r1) = 1;
        MATCH_HOLD_REG(u8 *, p, r0) = &part->animating;

        *p = one;
        p += 0x38 - 0x2C;
        MATCH_KEEP(p);
        if (*p)
            SET_FRAME_R(part, 0, "r4", "r5");
    }
}

void UpdateCortexBossGem(struct gfx_kind_ctrl *self, struct gfx_part *partArg)
{
    struct gfx_part *part = partArg;

    switch (self->state) {
    case 0:
        if (part->kind == 1) {
            part->bank = (struct anim_bank *)(SPRITE_BANK_BASE + 0x27C);
            switch (self->kind) {
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
            SetFrameNibble(part, GetSpriteAnimPaletteSlot((struct actor *)part));
            CALL2(self, method_20, 1);
        }
        break;
    case 1:
        if (part->animDone)
            MARK_GONE(part, "r1", "r2", "r1");
        break;
    }
}

void DestroyCortexBossGemCtrl(struct gfx_ctrl *self, s32 flags)
{
    self->vtable = (struct gfx_vtable *)gCortexBossGemVtable;
    DestroyCtrl(self, flags);
}

void *CreateCortexBossGemCtrl(void *selfArg, s32 kind)
{
    struct gfx_kind_ctrl *self = selfArg;

    InitCtrl(self);
    self->vtable = (struct gfx_vtable *)gCortexBossGemVtable;
    self->kind = kind;
    return self;
}

void DestroyCortexBossPlatformMover(struct gfx_ctrl *self, s32 flags)
{
    self->vtable = (struct gfx_vtable *)gCortexBossPlatformMoverVtable;
    DestroyPlatformMover((struct mover *)self, flags);
}

/* Constructor: base-constructs through CreatePlatformMover(self, 0, 0, 0, {0}, 6)
 * and points the method table at gCortexBossPlatformMoverVtable.
 *
 * The fifth argument is a byte the ROM stores with `strb` into its
 * outgoing stack slot; like CreatePlatform (platform_create.c), the call
 * writes both stack slots itself through MOVER_NEW's 4-argument view
 * (include/mover_new.h), which also gives the ROM's `mov r1, sp` before
 * the 0. */
void *CreateCortexBossPlatformMover(struct gfx_ctrl *self)
{
    struct mover_stack_args args;

    *(volatile u8 *)&args.dirY = 0;
    *(volatile s32 *)&args.kind = 6;
    MOVER_NEW(self, 0, 0, 0);
    self->vtable = (struct gfx_vtable *)gCortexBossPlatformMoverVtable;
    return self;
}

void DestroyCortexShotCtrl(struct gfx_ctrl *self, s32 flags)
{
    self->vtable = (struct gfx_vtable *)gCortexShotVtable;
    DestroyCtrl(self, flags);
}

void *CreateCortexShotCtrl(void *selfArg, void *cfg)
{
    struct {
        u8 unk_00[0xC];
        struct gfx_vtable *vtable;
        u8 unk_10[4];
        void *cfg;
    } *self = selfArg;

    InitCtrl(self);
    self->vtable = (struct gfx_vtable *)gCortexShotVtable;
    self->cfg = cfg;
    return self;
}
