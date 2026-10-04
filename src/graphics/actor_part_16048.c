#include "core.h"
#include "player_ctrl.h"

/* GitHub issues #19 (its last raw function, sub_8016048) and #20
 * (0x08016128-0x08017524): the player-input controller class of
 * include/player_ctrl.h, method table gStaticData_087E428C.
 *
 * Built with the older compiler, tools/agbcc/bin/old_agbcc (the Makefile's
 * OLD_AGBCC_OBJS) - see docs/matching/issue-20-player-ctrl.md.
 *
 * - sub_8016288 (table slot +0x0C) is the per-frame update: D-pad
 *   up/down with auto-repeat (`repeat`) steps `level` (0..12) and
 *   re-applies the target's animation (ApplyLevel, whose out-of-line copy
 *   is sub_8017348), then runs the per-state handler through
 *   gStaticData_0816C250, a table of gcc 2.x pointer-to-member-functions:
 *   states 0..7 are sub_8016B1C, sub_8016C08, sub_8016C94, sub_8016D5C,
 *   sub_8016DDC, sub_80170EC, sub_8017044, sub_8017184.
 * - sub_8016128 (+0x14) is the message handler, sub_8017218 (+0x1C) sets
 *   the target, sub_80174D8 (+0x4C) is the destructor and sub_80174EC the
 *   constructor (called from game_loop39.c).
 * - sub_8017264 sets the mode through the method table (+0x20/+0x50,
 *   called through the _call_via_r2/_call_via_r3 `_call_via_rN` thunks) and
 *   picks the animation from gStaticData_0816C070[mode][level].
 * - sub_80172D0 writes the player's +0x48/+0x4C/+0x50 record from its
 *   speed, like actor_part57b.c's sub_8015FDC does for +0x54..+0x5C.
 *
 * Several functions are inline helpers in the original (C++ inline
 * methods): SetState/sub_8017264, ResetMode/sub_80174BC,
 * SetPlayerRecord/sub_80172D0 and ApplyLevel/sub_8017348 are each inlined
 * at some call sites and also emitted out of line.
 *
 * UNUSED - no reference in asm/, src/ or data/ and no Thumb pointer in the
 * ROM: sub_8016AB0, sub_801721C, sub_8017240, sub_8017330, sub_8017348,
 * sub_80174BC, sub_801750C, sub_8017514, sub_801751C. Matched anyway. */

/* The held/pressed key words of gKeys. The zero-length array
 * makes the struct BLKmode, so a local copy lives on the stack (the ROM
 * reads `pressed` back with `ldrh [sp, #2]`); without it gcc keeps the
 * copy in a register. */
struct keys
{
    u16 held;
    u16 pressed;
    u8 pad[0];
};

struct pmf_entry
{
    s16 delta;
    s16 index;
    void *fn;
};

struct pmf
{
    s16 delta;
    s16 index;
    union
    {
        void *fn;
        s16 vtableOffset;
    } u;
};

extern void *gUnknown_03001304;
extern struct keys gKeys;
extern u32 gUnknown_0300082C;
extern void *gUnknown_030012BC;
extern void *gLevelState;
extern void *gUnknown_030012B8;
extern u8 *gEntityFlags;
extern struct pctrl_target *gUnknown_030012D8;
extern struct pmf gStaticData_0816C250[];
struct level_anim
{
    u8 anim;
    u8 unk_1[3];
};

extern struct level_anim *gStaticData_0816C070[];
extern struct pctrl_anim gStaticData_0816B61C[];
extern u8 gStaticData_087E428C[];

extern u8 GetDpadDirection(void *arg);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void LoseLife(void *arg0);
extern void sub_8006D08(void *cache, s32 slot, s32 recordId);
extern void sub_80087C0(struct pctrl_target *t);
extern void sub_80087B4(struct pctrl_target *t);
extern void sub_800872C(struct pctrl_target *t, s32 a);
extern void sub_800B6D0(void *self, struct pctrl_target *t, struct pctrl_anim *rec);
extern void sub_800B7B0(void *self, struct pctrl_target *t, struct pctrl_anim *rec);
extern void sub_800B8A8(void *self, s32 flags);
extern void sub_800B8C8(void *self);
extern void sub_8015958(struct player_ctrl *self);
extern void sub_80159F8(struct player_ctrl *self);
extern void sub_8015C6C(struct player_ctrl *self);
extern void sub_8015DF8(struct player_ctrl *self);
extern void sub_8015FDC(s32 a, s32 b, s32 c);

typedef void (*pctrl_fn1)(void *self, s32 a);
typedef void (*pctrl_fn2)(void *self, struct pctrl_target *t, s32 a);
typedef void (*pctrl_fn0)(void *self);

/* Virtual calls. Plain-brace macros on purpose: a do/while(0) wrapper
 * emits loop notes that change what CSE and cross-jumping do, and a
 * ({ }) statement expression leaves a USE insn that blocks cross-jumping
 * (see docs/matching/issue-20-player-ctrl.md). */
#define SET_MODE(obj, a)                                                        \
    {                                                                          \
        struct pctrl_method *_m = &(obj)->vtable->setMode;                     \
        ((pctrl_fn1)_m->fn)((u8 *)(obj) + _m->thisOffset, (a));                \
    }
#define SET_ANIM(obj, t, a)                                                    \
    {                                                                          \
        struct pctrl_method *_m = &(obj)->vtable->setAnim;                     \
        ((pctrl_fn2)_m->fn)((u8 *)(obj) + _m->thisOffset, (t), (a));           \
    }

#define KEEP 0x7FFFFFFF

void sub_8017264(struct player_ctrl *self, s32 a, s32 mode, s32 timer, s32 timerMax);
void sub_80161EC(struct player_ctrl *self, s32 anim);

static inline u8 LevelAnim(struct player_ctrl *self)
{
    return gStaticData_0816C070[self->mode][self->level].anim;
}

/* the out-of-line copy is sub_8017264 */
static inline void SetState(struct player_ctrl *self, s32 a, s32 mode, s32 timer, s32 timerMax)
{
    SET_MODE(self, a);
    self->mode = mode;
    SET_ANIM(self, self->target, LevelAnim(self));
    if (timer != KEEP)
        self->timer = timer;
    if (timerMax != KEEP)
        self->timerMax = timerMax;
}

/* the out-of-line copy is sub_80174BC */
static inline void ResetMode(struct player_ctrl *self)
{
    sub_8017264(self, 1, 1, KEEP, 0);
}

/* The value arrives as a (constant-propagated) inline parameter, so the
 * bitfield store is the generic clear-then-or, as in the ROM. */
static inline void SetFlipX(struct pctrl_target *t, u32 value)
{
    t->f28.flipX = value;
}

static inline void SetUnk68(struct pctrl_target *t, s32 value)
{
    t->unk_68 = value;
}

static inline void SetA(struct player_ctrl *self, s32 value)
{
    self->hasA = 1;
    self->valueA = value;
}

static inline void SetB(struct player_ctrl *self, s32 value)
{
    self->hasB = 1;
    self->valueB = value;
}

void sub_8016048(struct player_ctrl *self)
{
    u8 dir = GetDpadDirection(gUnknown_03001304);

    switch (self->state)
    {
    case 0 ... 3:
    case 5 ... 6:
        self->target->f28.flipY = 0;
        if (self->target->f28.flipX && (dir == 4 || dir == 6 || dir == 8))
        {
            self->target->f28.flipX = 0;
            if (self->state != 3)
            {
                sub_8017264(self, 4, 4, KEEP, KEEP);
                self->timerMax = 0;
            }
            else
            {
                sub_8017264(self, 4, 5, KEEP, KEEP);
            }
            self->unk_26 = 0;
        }
        else if (!(self->target->f28.flipX & 1) && (dir == 3 || dir == 5 || dir == 7))
        {
            if (self->state != 3)
            {
                sub_8017264(self, 4, 6, KEEP, KEEP);
                self->timerMax = 0;
            }
            else
            {
                sub_8017264(self, 4, 7, KEEP, KEEP);
            }
            self->unk_26 = 0;
        }
        break;
    }
}

void sub_8016128(struct player_ctrl *self, s32 unused, s32 msg, s32 arg)
{
    switch (msg)
    {
    case 12:
    {
        s32 side = arg & 3;

        if (side == 2)
        {
            if (self->target->f28.flipX)
                SetA(self, 0);
        }
        else if (side == 1)
        {
            if (!self->target->f28.flipX)
                SetA(self, 0);
        }
        else
        {
            break;
        }
        self->target->speedX = 0;
        break;
    }
    case 5:
        sub_80161EC(self, 0x2D);
        break;
    case 3:
        sub_80161EC(self, 0x2B);
        break;
    case 4:
        sub_80161EC(self, 0x2C);
        break;
    case 1:
    case 6:
    case 10:
        sub_80161EC(self, 0x2E);
        break;
    case 13:
        break;
    }
}

void sub_80161EC(struct player_ctrl *self, s32 anim)
{
    PlaySfx(gUnknown_030012BC, 0x1B, 0x100);
    SET_MODE(self, 7);
    SET_ANIM(self, self->target, anim);
    self->target->flag7 = 0;
    self->target->flag6 = 0;
    self->target->unk_104 = 1;
    LoseLife(gLevelState);
    sub_8006D08(gUnknown_030012B8, self->target->slot,
                self->target->anim->records[self->target->tag].unk_14);
}

/* Runs the pointer-to-member handler for the current state. */
#define PMF_DISPATCH(self)                                                      \
    {                                                                          \
        s32 idx = gStaticData_0816C250[(self)->state].index;                   \
        struct pmf_entry e;                                                    \
        void *fn;                                                              \
        s32 d;                                                                 \
        s32 adj;                                                               \
                                                                               \
        if (idx > 0)                                                           \
        {                                                                      \
            e = (*(struct pmf_entry **)((u8 *)(self) + gStaticData_0816C250[(self)->state].u.vtableOffset))[idx - 1]; \
            fn = e.fn;                                                         \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            fn = gStaticData_0816C250[(self)->state].u.fn;                     \
        }                                                                      \
        d = gStaticData_0816C250[(self)->state].delta;                         \
        if (idx > 0)                                                           \
            adj = e.delta + d;                                                 \
        else                                                                   \
            adj = d;                                                           \
        ((pctrl_fn0)fn)((u8 *)(self) + adj);                                   \
    }

/* the out-of-line copy is sub_80172D0 */
static inline void SetPlayerRecord(s32 a, s32 b, s32 c)
{
    struct pctrl_target *p = gUnknown_030012D8;
    s32 v = p->speedX;
    s32 t = v * v / 0x4000 + 4;
    s32 signV;
    s32 signC;
    s32 absV;
    s32 absC;

    signV = v >> 31;
    absV = (v ^ signV) - signV;
    signC = c >> 31;
    absC = (c ^ signC) - signC;
    if (absV > absC)
    {
        p->unk_48 = a;
        p->unk_4C = t;
    }
    else if (v * c < 0)
    {
        s32 sum = t + b;

        p->unk_48 = a;
        p->unk_4C = sum;
    }
    else
    {
        p->unk_48 = a;
        p->unk_4C = b;
    }
    p->unk_50 = c;
}

/* Sets bit `id` of the gEntityFlags+0x108 bitmap. Kept a
 * do/while(0) macro: its loop notes stop CSE from reusing the id already
 * loaded for the caller's 0xFFFF test, so the ROM's reload comes out
 * naturally, and the word index is gcc's signed division of it. */
#define SET_ID_BIT(idExpr)                                                     \
    do                                                                         \
    {                                                                          \
        s32 _id = (idExpr);                                                    \
        u8 *_base = gEntityFlags;                                         \
        s32 _word = _id / 32;                                                  \
        s32 _off = _word * 4;                                                  \
        u32 *_slot = (u32 *)(_base + 0x108);                                   \
                                                                               \
        _slot = (u32 *)((u8 *)_slot + _off);                                   \
        *_slot |= 1 << (_id - _word * 32);                                     \
    } while (0)

/* "Mark gone": sub_80072D8's sequence (graphics.c), inlined - set flags
 * bit 0, then unless the id is 0xFFFF set its bit in the bitmap. */
static inline void MarkGone(struct pctrl_target *t)
{
    t->gone = 1;
    if (t->id != 0xFFFF)
        SET_ID_BIT(t->id);
}

static inline void ClampFrame(struct pctrl_target *t, s32 frame)
{
    s32 n = t->anim->records[t->tag].frames;

    if (frame >= n)
        frame = n - 1;
    t->frame = frame;
}

static inline void RestoreFrame(struct pctrl_target *t, s32 frame, s32 f34)
{
    ClampFrame(t, frame);
    t->unk_34 = f34;
}

/* Re-applies the animation for the current mode/level; the out-of-line
 * copy is sub_8017348. */
static inline void ApplyLevel(struct player_ctrl *self)
{
    struct pctrl_target *t = self->target;
    u8 *tag = &t->tag;

    if (*tag != 0x20 && *tag != 0x1D && *tag != 0x1F)
    {
        s32 frame = t->frame;
        s32 f34 = t->unk_34;

        *tag = gStaticData_0816C070[self->mode][self->level].anim;
        sub_80087C0(t);
        sub_80087B4(t);
        sub_800872C(t, 0);
        RestoreFrame(self->target, frame, f34);
    }
    else
    {
        switch (self->mode)
        {
        case 7:
        case 14 ... 18:
        case 23:
        case 33 ... 37:
        case 41:
        {
            s32 frame = self->target->frame;
            s32 f34 = self->target->unk_34;

            ResetMode(self);
            RestoreFrame(self->target, frame, f34);
            break;
        }
        default:
            ResetMode(self);
            break;
        }
    }
}

void sub_8016288(struct player_ctrl *self)
{
    if (self->state == 7)
    {
        PMF_DISPATCH(self);
    }
    else
    {
        u32 keys;
        u8 dir;
        void *inp;

        if (self->cooldown)
            self->cooldown--;
        inp = gUnknown_03001304;
        keys = *(u32 *)&gKeys; /* the whole word, held keys low */
        dir = GetDpadDirection(inp);

        if (!(keys & (DPAD_UP | DPAD_DOWN)) && self->state != 2)
        {
            if (self->repeat && self->level != 6)
                self->repeat = self->repeat - 1;
            else
                self->repeat = 3;
            if (self->repeat == 0 && self->level != 6)
            {
                if (self->level <= 5)
                    self->level++;
                if (self->level > 6)
                    self->level--;
                if (self->level != 6)
                    self->repeat = 3;
                ApplyLevel(self);
            }
        }
        else if (self->state != 2 || dir != 0)
        {
            if (self->repeat == 0 || --self->repeat == 0)
            {
                self->repeat = 3;
                if (keys & DPAD_UP)
                {
                    if (keys & DPAD_SIDEWAYS)
                    {
                        if (self->level > 3)
                        {
                            self->level--;
                            ApplyLevel(self);
                        }
                        else if (self->level <= 2)
                        {
                            self->level++;
                            ApplyLevel(self);
                        }
                    }
                    else if (self->level != 0)
                    {
                        self->level--;
                        ApplyLevel(self);
                    }
                }
                else if (keys & DPAD_DOWN)
                {
                    if (keys & DPAD_SIDEWAYS)
                    {
                        if (self->level <= 8)
                        {
                            self->level++;
                            ApplyLevel(self);
                        }
                        else if (self->level > 9)
                        {
                            self->level--;
                            ApplyLevel(self);
                        }
                    }
                    else if (self->level <= 11)
                    {
                        self->level++;
                        ApplyLevel(self);
                    }
                }
            }
        }

        PMF_DISPATCH(self);
        if (self->target->unk_74 & 3)
            self->target->speedX = 0;
        if (self->target->unk_74 & 0xC)
            self->target->speedY = 0;
        SetUnk68(self->target, 0);
        sub_8015DF8(self);
    }

    if (self->state == 3 || self->mode == 5 || self->mode == 7)
        self->target->unk_0A = 0x13;
    else
        self->target->unk_0A = 1;
}

/* The register pins are load-bearing (docs/workflow.md step 7): the ROM
 * loads the record index into r1 and scales it into r0 before adding the
 * table base into r2; every unpinned form tried scales straight into r2. */
/* UNUSED */
void sub_8016AB0(struct player_ctrl *self)
{
    if (self->hasA == 1)
    {
        struct pctrl_anim *rec;

        self->hasA = 0;
        {
            register u32 i asm("r1") = self->animSet->entries[self->valueA].a;
            register u32 off asm("r0") = i * sizeof(struct pctrl_anim);

            rec = (struct pctrl_anim *)(off + (u32)gStaticData_0816B61C);
        }
        sub_800B7B0(self, self->target, rec);
    }
    if (self->hasB == 1)
    {
        struct pctrl_anim *rec;

        self->hasB = 0;
        {
            register u32 i asm("r1") = self->animSet->entries[self->valueB].b;
            register u32 off asm("r0") = i * sizeof(struct pctrl_anim);

            rec = (struct pctrl_anim *)(off + (u32)gStaticData_0816B61C);
        }
        sub_800B6D0(self, self->target, rec);
    }
}

void sub_8016B1C(struct player_ctrl *self)
{
    struct keys k;
    u8 dir;
    u8 count;
    void *inp = gUnknown_03001304;

    k = gKeys;
    dir = GetDpadDirection(inp);
    count = ++self->counter;
    if (count == 30)
    {
        SetB(self, 1);
    }
    else if (count > 59)
    {
        SetB(self, 2);
        self->counter = 0;
    }
    if (self->target->unk_38)
        SET_ANIM(self, self->target, 0x1F);
    if (k.pressed & A_BUTTON)
    {
        sub_80159F8(self);
    }
    else if (k.pressed & (B_BUTTON | R_BUTTON))
    {
        sub_8015C6C(self);
    }
    else if (dir)
    {
        SET_MODE(self, 6);
        SET_ANIM(self, self->target, 0x1D);
        SetA(self, 0xC);
    }
    self->unk_26 = 0;
    sub_8016048(self);
}

void sub_8016C08(struct player_ctrl *self)
{
    struct keys k;
    u8 dir = GetDpadDirection(gUnknown_03001304);

    k = gKeys;
    if (k.pressed & A_BUTTON)
    {
        sub_80159F8(self);
        return;
    }
    if (k.pressed & (B_BUTTON | R_BUTTON))
        sub_8015C6C(self);
    if (dir == 0 && self->level == 6)
    {
        SET_MODE(self, 5);
        SET_ANIM(self, self->target, 0x20);
        self->mode = dir;
    }
    sub_8016048(self);
}

void sub_8016C94(struct player_ctrl *self)
{
    void *inp = gUnknown_03001304;
    struct keys k = gKeys;
    struct keys *kp = &k;

    if (kp->pressed & B_BUTTON)
    {
        sub_8015C6C(self);
        return;
    }
    if (self->target->unk_38 || gUnknown_0300082C > self->unk_28)
    {
        u8 dir = GetDpadDirection(inp);

        if (kp->pressed & A_BUTTON)
            sub_80159F8(self);
        else if (dir)
            ResetMode(self);
        else if (self->level == 6)
        {
            SET_MODE(self, 5);
            SET_ANIM(self, self->target, 0x20);
        }
        else
            ResetMode(self);
    }
    sub_8016048(self);
}

void sub_8016D5C(struct player_ctrl *self)
{
    u8 dir = GetDpadDirection(gUnknown_03001304);

    if (++self->timer >= self->timerMax || self->target->unk_38)
    {
        gUnknown_030012D8->unk_92 = 0;
        self->cooldown = 0xC;
        if (dir == 0)
        {
            ResetMode(self);
            self->timerMax = 1;
        }
        else
        {
            ResetMode(self);
        }
    }
    sub_8016048(self);
}

void sub_8016DDC(struct player_ctrl *self)
{
    struct keys k = gKeys;
    s32 frame;

    if (self->timerMax != 0 && ++self->timer >= self->timerMax)
    {
        self->cooldown = 0xC;
        self->timerMax = 0;
        switch (self->mode)
        {
        case 7:
            frame = self->target->frame;
            self->mode = 6;
            SET_ANIM(self, self->target, gStaticData_0816C070[6][self->level].anim);
            ClampFrame(self->target, frame);
            break;
        case 5:
            frame = self->target->frame;
            self->mode = 4;
            SET_ANIM(self, self->target, gStaticData_0816C070[4][self->level].anim);
            ClampFrame(self->target, frame);
            break;
        }
    }
    if (k.pressed & (B_BUTTON | R_BUTTON))
    {
        if (self->cooldown != 0 || self->timerMax != 0)
            return;
        self->timer = 0;
        self->timerMax = 0x18;
        frame = self->target->frame;
        if (self->mode == 4)
            self->mode = 5;
        else
            self->mode = 7;
        SET_ANIM(self, self->target, gStaticData_0816C070[self->mode][self->level].anim);
        sub_8015C6C(self);
        ClampFrame(self->target, frame);
        return;
    }
    if (k.pressed & A_BUTTON)
        sub_80159F8(self);
    if (self->target->unk_38 == 0)
        return;
    switch (self->mode)
    {
    case 4:
        if (self->target->f28.flipX)
            self->target->f28.flipX = 0;
        ResetMode(self);
        break;
    case 6:
        SetFlipX(self->target, 1);
        ResetMode(self);
        break;
    case 5:
        if (self->target->f28.flipX)
            self->target->f28.flipX = 0;
        SetState(self, 3, 3, KEEP, KEEP);
        break;
    case 7:
        SetFlipX(self->target, 1);
        SetState(self, 3, 3, KEEP, KEEP);
        break;
    }
    self->hasA = 1;
}

void sub_8017044(struct player_ctrl *self)
{
    void *inp = gUnknown_03001304;
    struct keys k = gKeys;
    struct keys *kp = &k;

    if (kp->pressed & B_BUTTON)
    {
        sub_8015C6C(self);
        return;
    }
    if (self->target->unk_38)
    {
        u8 dir = GetDpadDirection(inp);

        if (kp->pressed & A_BUTTON)
            sub_80159F8(self);
        else if (dir)
            ResetMode(self);
        else if (self->level == 6)
        {
            SET_MODE(self, 5);
            SET_ANIM(self, self->target, 0x20);
        }
    }
    sub_8016048(self);
}

void sub_80170EC(struct player_ctrl *self)
{
    struct keys k = gKeys;

    if (k.pressed & A_BUTTON)
    {
        sub_80159F8(self);
        return;
    }
    if (k.pressed & (B_BUTTON | R_BUTTON))
    {
        sub_8015C6C(self);
        return;
    }
    if (self->target->unk_38)
    {
        self->mode = 0;
        SetState(self, 0, 0, 0, 0);
    }
    sub_8016048(self);
}

void sub_8017184(struct player_ctrl *self)
{
    sub_8015FDC(0, 5, 0);
    SetPlayerRecord(0, 5, 0);
    if (self->target->unk_38)
        MarkGone(self->target);
}

void sub_8017218(struct player_ctrl *self, struct pctrl_target *target)
{
    self->target = target;
}

/* UNUSED */
void sub_801721C(struct player_ctrl *self, struct pctrl_target *target, s32 idx)
{
    sub_800B6D0(self, target, &gStaticData_0816B61C[self->animSet->entries[idx].b]);
}

/* UNUSED */
void sub_8017240(struct player_ctrl *self, struct pctrl_target *target, s32 idx)
{
    sub_800B7B0(self, target, &gStaticData_0816B61C[self->animSet->entries[idx].a]);
}

void sub_8017264(struct player_ctrl *self, s32 a, s32 mode, s32 timer, s32 timerMax)
{
    SetState(self, a, mode, timer, timerMax);
}

void sub_80172D0(s32 a, s32 b, s32 c)
{
    SetPlayerRecord(a, b, c);
}

/* UNUSED */
s32 sub_8017330(s32 v)
{
    return v * v / 0x4000 + 4;
}

/* UNUSED */
void sub_8017348(struct player_ctrl *self)
{
    ApplyLevel(self);
}

/* UNUSED */
void sub_80174BC(struct player_ctrl *self)
{
    sub_8017264(self, 1, 1, KEEP, 0);
}

void sub_80174D8(struct player_ctrl *self, s32 flags)
{
    self->vtable = (struct pctrl_vtable *)gStaticData_087E428C;
    sub_800B8A8(self, flags);
}

struct player_ctrl *sub_80174EC(struct player_ctrl *self)
{
    sub_800B8C8(self);
    self->vtable = (struct pctrl_vtable *)gStaticData_087E428C;
    sub_8015958(self);
    return self;
}

/* UNUSED */
void sub_801750C(struct player_ctrl *self)
{
    self->unk_14 = 0;
}

/* UNUSED */
void sub_8017514(struct player_ctrl *self)
{
    self->hasB = 1;
}

/* UNUSED */
void sub_801751C(struct player_ctrl *self)
{
    self->hasA = 1;
}
