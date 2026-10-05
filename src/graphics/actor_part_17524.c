#include "core.h"

/* GitHub issue #21: 0x08017524-0x08017A44, the whole tail of the former
 * asm/code_3_2_17_16048.s.
 *
 * The first six functions are byte accessors for an object not otherwise
 * characterized here (`+0x2C`/`+0x2D` "has value" flags for the bytes at
 * `+0x24`/`+0x25`).
 *
 * The other 19 are one self-contained actor-part subclass,
 * `struct input_ctrl`, whose method table is `gInputCtrlVtable`
 * (constructor `CreateInputCtrl` - called from game_loop39.c - destructor
 * `DestroyInputCtrl`; every other slot it calls through is a base-class
 * `sub_800B6xx`/`sub_800B8xx` function). Each frame (`UpdateInputCtrl`, table
 * slot +0x0C) it reads the held D-pad bits from `gKeys` and
 * picks animation pairs for its target (`+0x10`) through
 * `gInputCtrlMotionRecords`'s 12-byte records: up/down select one channel
 * (`animB`), left/right the other (`animA`), which also sets a speed-like
 * value at the child object's `+0x78` (`+0x1C`, spawned on demand by
 * `sub_8017600`). Once the target passes the level's right edge
 * (`gLevelLayers`'s layer 0 width, less 0xA00) the child is marked
 * gone and `RequestRoomExit` is signalled. It then dispatches the current
 * `state` through `gInputCtrlStateFuncs`, a table of gcc 2.x
 * pointer-to-member-functions: state 0 `sub_8017600`, 1 `sub_801796C`,
 * 2 `sub_801793C`, 3 `sub_80178EC`. That call sequence - and the
 * `_call_via_r1`/`AD80`/`AD84` "call via r1/r2/r3" trampolines used for
 * every virtual call - is what gcc's C++ front end emits, so this object
 * was very likely written in C++.
 *
 * UNUSED - no `bl`/`.4byte` reference in the asm/ sources or any .c file under
 * src/, and no Thumb pointer anywhere in the ROM: `sub_8017524`,
 * `sub_801752C`, `sub_8017534`, `sub_801753C`, `sub_8017544`,
 * `sub_8017554`, `sub_8017A20`, `sub_8017A28`, `sub_8017A30`,
 * `sub_8017A38`, `sub_8017A40`. Matched anyway.
 *
 * Built with the older compiler, tools/agbcc/bin/old_agbcc (the Makefile's
 * OLD_AGBCC_OBJS), like actor_part_16048.c before it. Under old_agbcc the
 * whole file is plain C: no register pins, `volatile` re-reads or empty
 * `asm` barriers (those were needed to imitate old_agbcc's output with the
 * current agbcc).
 *
 * Matching notes (details in docs/matching/issue-21-input-ctrl.md): the
 * virtual-call macros take the method-table entry's address once; the
 * `SetAnimA`/`SetAnimB`/`SetChildSpeed` inline helpers reproduce the ROM
 * evaluating the stored constant before the store's own loads; the
 * "mark actor gone" bitmap sequence (MarkEntityGone's, inlined twice) is
 * MARK_GONE, actor_part_16048.c's MarkGone - its word index comes from a signed
 * division of the zero-extended id. */

struct flag_pair_owner
{
    u8 unk_00[0x24];
    u8 valueA;   // 0x24
    u8 valueB;   // 0x25
    u8 unk_26[6];
    u8 hasA;     // 0x2C
    u8 hasB;     // 0x2D
};

struct ctrl_method
{
    s16 thisOffset;
    u8 unk_2[2];
    void *fn;
};

struct ctrl_vtable
{
    u8 unk_00[0x20];
    struct ctrl_method method_20; // 0x20
    struct ctrl_method method_28; // 0x28
    struct ctrl_method method_30; // 0x30
    struct ctrl_method method_38; // 0x38
    struct ctrl_method method_40; // 0x40
    u8 unk_48[8];
    struct ctrl_method method_50; // 0x50
};

struct ctrl_target
{
    s32 x;            // 0x00
    s32 y;            // 0x04
    u16 field_08;     // 0x08 - bitmap id (see MarkEntityGone)
    u8 unk_0A[2];
    u8 gone:1;        // 0x0C - bit 0: removed (see MarkEntityGone)
    u8 unk_0C_1:5;
    u8 flag6:1;
    u8 flag7:1;
    u8 unk_0D[0x13];
    struct { u8 *records; } *table; // 0x20
    u8 unk_24[5];
    u8 slot:4;        // 0x29 - low nibble
    u8 unk_29_4:4;
    u8 unk_2A[3];
    u8 tag;           // 0x2D
    u8 unk_2E[0xA];
    u8 unk_38;        // 0x38
    u8 unk_39[0xCB];
    u8 dead;           // 0x104
};

struct ctrl_child
{
    s32 x;            // 0x00
    s32 y;            // 0x04
    u16 field_08;     // 0x08 - bitmap id
    u8 unk_0A[2];
    u8 gone:1;        // 0x0C - bit 0: removed
    u8 unk_0C_1:7;
    u8 unk_0D[0x6B];
    s32 unk_78;       // 0x78
};

struct anim_pair
{
    u32 a;
    u32 b;
};

struct input_ctrl
{
    u8 unk_00[4];
    struct { struct anim_pair *entries; } *animSet; // 0x04
    s32 state;                   // 0x08
    struct ctrl_vtable *vtable;  // 0x0C
    struct ctrl_target *target;  // 0x10
    u8 animA;                    // 0x14
    u8 animB;                    // 0x15
    u8 dirState;                 // 0x16
    u8 dirtyA;                   // 0x17
    u8 dirtyB;                   // 0x18
    u8 altA;                     // 0x19
    u8 altB;                     // 0x1A
    u8 unk_1B;
    struct ctrl_child *child;    // 0x1C
    u8 flag20;                   // 0x20
    u8 unk_21[3];
    s32 timer;                   // 0x24
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

extern void *gAudioContext;
extern void *gLevelState;
extern void *gPaletteCache;
extern void *gEntityFlags;
extern void *gCollidableList;
extern u32 gKeys; /* low half: held keys */
extern struct { u8 unk_00[0x10]; struct { u8 unk_00[0x10]; s32 width; } *layer0; } *gLevelLayers;
extern struct pmf gInputCtrlStateFuncs[];
extern u8 gInputCtrlMotionRecords[];
extern u8 gInputCtrlVtable[];

extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern s32 _call_via_r2(void *self, s32 arg, void *fn);
extern s32 _call_via_r3(void *self, void *arg1, void *arg2, void *fn);
extern void LoseLife(void *arg0);
extern void LoadPaletteSlot(void *self, s32 slot, s32 recordId);
extern void *OperatorNew(u32 size);
extern struct ctrl_child *CreateCameraLead(void *mem);
extern void ResetCameraLead(struct ctrl_child *child);
extern void AddToPartList(void *manager, void *value);
extern void RequestRoomExit(void);
extern void DestroyCtrl(void *self, s32 flags);
extern void InitCtrl(void *self);

/* A virtual call as gcc 2.x lowers it: take the method-table entry's
 * address once, then read its `this` adjustment and function from it.
 * `if (1) { } else (void)0` rather than `do { } while (0)`, whose loop
 * notes are not neutral under this compiler (include/actor_self.h). */
#define CTRL_CALL2(obj, m, a)                                                  \
    if (1) {                                                                   \
        struct ctrl_method *_m = &(obj)->vtable->m;                            \
        _call_via_r2((u8 *)(obj) + _m->thisOffset, (a), _m->fn);                \
    } else (void)0
#define CTRL_CALL3(obj, m, a, b)                                               \
    if (1) {                                                                   \
        struct ctrl_method *_m = &(obj)->vtable->m;                            \
        _call_via_r3((u8 *)(obj) + _m->thisOffset, (a), (b), _m->fn);           \
    } else (void)0

/* MarkEntityGone's "set the id's bit in the gEntityFlags+0x108 bitmap"
 * (see actor_part_16048.c: the do/while(0) loop notes are what reproduce
 * the id reload after the 0xFFFF test) */
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

/* "Mark gone": MarkEntityGone's sequence (graphics.c), inlined */
#define MARK_GONE(t)                                                           \
    {                                                                          \
        (t)->gone = 1;                                                         \
        if ((t)->field_08 != 0xFFFF)                                           \
            SET_ID_BIT((t)->field_08);                                         \
    }

static inline void SetChildSpeed(struct input_ctrl *self, s32 speed)
{
    self->child->unk_78 = speed;
}

static inline void SetAnimA(struct input_ctrl *self, u8 anim)
{
    self->dirtyA = 1;
    self->animA = anim;
}

static inline void SetAnimB(struct input_ctrl *self, u8 anim)
{
    self->dirtyB = 1;
    self->animB = anim;
}

void sub_80178BC(struct input_ctrl *self, s32 mode, void *arg, s32 unused3, s32 unused4);
void sub_8017808(struct input_ctrl *self);
void ResetInputCtrl(struct input_ctrl *self);

void sub_8017524(struct flag_pair_owner *self)
{
    self->hasB = 0;
}

void sub_801752C(struct flag_pair_owner *self)
{
    self->hasA = 0;
}

u8 sub_8017534(struct flag_pair_owner *self)
{
    return self->hasB;
}

u8 sub_801753C(struct flag_pair_owner *self)
{
    return self->hasA;
}

void sub_8017544(struct flag_pair_owner *self, u8 value)
{
    self->hasB = 1;
    self->valueB = value;
}

void sub_8017554(struct flag_pair_owner *self, u8 value)
{
    self->hasA = 1;
    self->valueA = value;
}


void InputCtrlKillPlayer(struct input_ctrl *self, void *arg)
{
    PlaySfx(gAudioContext, 0x1B, 0x100);
    CTRL_CALL2(self, method_20, 3);
    CTRL_CALL3(self, method_50, self->target, arg);
    self->target->flag7 = 0;
    self->target->flag6 = 0;
    self->target->dead = 1;
    LoseLife(gLevelState);
    {
        void *cache = gPaletteCache;
        struct ctrl_target *t = self->target;

        LoadPaletteSlot(cache, t->slot, t->table->records[t->tag * 28 + 0x14]);
    }
}

void sub_8017600(struct input_ctrl *self)
{
    sub_80178BC(self, 1, NULL, 0, 0);
    self->dirtyA = 1;
    self->animA = 1;
    self->dirtyB = 1;
    self->animB = 0;
    self->dirState = 0;
    if (self->child == NULL)
    {
        self->child = CreateCameraLead(OperatorNew(0x80));
        AddToPartList(gCollidableList, self->child);
    }
    ResetCameraLead(self->child);
}


void UpdateInputCtrl(struct input_ctrl *self)
{
    if (self->state != 3)
    {
        u32 keys;
        s32 x = self->target->x;

        if (x > (gLevelLayers->layer0->width << 8) - 0xA00)
        {
            {
                struct ctrl_child *c = self->child;

                MARK_GONE(c);
            }
            self->child = NULL;
            RequestRoomExit();
        }

        keys = gKeys;
        if ((keys & DPAD_UP) && self->dirState != 1)
        {
            SetAnimB(self, 3);
            self->dirState = 1;
        }
        else
        {
            if ((keys & DPAD_DOWN) && self->dirState != 2)
            {
                SetAnimB(self, 5);
                self->dirState = 2;
            }
            else if (!(keys & (DPAD_UP | DPAD_DOWN)))
            {
                SetAnimB(self, 0);
                self->dirState = 0;
            }
        }

        if ((keys & DPAD_LEFT) && self->flag20)
        {
            SetAnimA(self, 7);
            SetChildSpeed(self, 0x3200);
            if (++self->timer > 30)
            {
                self->flag20 = 0;
                self->timer = 10;
            }
        }
        else if (keys & DPAD_RIGHT)
        {
            SetAnimA(self, 8);
            SetChildSpeed(self, 0xA00);
        }
        else if (!(keys & DPAD_SIDEWAYS) || ((keys & DPAD_LEFT) && !self->flag20))
        {
            SetChildSpeed(self, 0x1E00);
            SetAnimA(self, 1);
        }

        if (!self->flag20 && --self->timer < 0)
        {
            self->timer = 0;
            if (!(keys & DPAD_LEFT))
                self->flag20 = 1;
        }
    }

    {
        s32 idx = gInputCtrlStateFuncs[self->state].index;
        struct pmf_entry e;
        void *fn;

        if (idx > 0)
        {
            e = (*(struct pmf_entry **)((u8 *)self + gInputCtrlStateFuncs[self->state].u.vtableOffset))[idx - 1];
            fn = e.fn;
        }
        else
        {
            fn = gInputCtrlStateFuncs[self->state].u.fn;
        }
        {
            s32 d = gInputCtrlStateFuncs[self->state].delta;
            s32 adj;

            if (idx > 0)
                adj = e.delta + d;
            else
                adj = d;
            _call_via_r2((u8 *)self + adj, d, fn);
        }
    }
    sub_8017808(self);
}

void sub_8017808(struct input_ctrl *self)
{
    if (self->dirtyA == 1)
    {
        u8 *rec = gInputCtrlMotionRecords + self->animSet->entries[self->animA].a * 12;

        if (self->altA)
            CTRL_CALL3(self, method_38, self->target, rec);
        else
            CTRL_CALL3(self, method_28, self->target, rec);
        self->dirtyA = 0;
        self->altA = 0;
    }
    if (self->dirtyB == 1)
    {
        u8 *rec = gInputCtrlMotionRecords + self->animSet->entries[self->animB].b * 12;

        if (self->altB)
            CTRL_CALL3(self, method_40, self->target, rec);
        else
            CTRL_CALL3(self, method_30, self->target, rec);
        self->dirtyB = 0;
        self->altB = 0;
    }
}

void sub_80178BC(struct input_ctrl *self, s32 mode, void *arg, s32 unused3, s32 unused4)
{
    CTRL_CALL2(self, method_20, mode);
    CTRL_CALL3(self, method_50, self->target, arg);
}

void sub_80178EC(struct input_ctrl *self)
{
    struct ctrl_target *t = self->target;

    if (t->unk_38)
        MARK_GONE(t);
}

void sub_801793C(struct input_ctrl *self)
{
    if (self->target->unk_38)
    {
        sub_80178BC(self, 1, NULL, 0, 0);
        SetAnimA(self, 2);
    }
}

void sub_801796C(struct input_ctrl *self)
{
    if (self->target->unk_38)
        sub_80178BC(self, 2, NULL, 0, 0);
}

void sub_8017994(struct input_ctrl *self)
{
    sub_80178BC(self, 0, NULL, 0, 0);
    self->dirtyA = 1;
    self->animA = 0;
    self->dirtyB = 1;
    self->animB = 0;
}

void ResetInputCtrl(struct input_ctrl *self)
{
    self->state = 1;
    self->animA = 0;
    self->animB = 0;
    self->dirtyA = 1;
    self->dirtyB = 1;
    self->target = NULL;
    self->child = NULL;
    self->flag20 = 0;
}

void sub_80179D4(struct input_ctrl *self, s32 arg1, s32 arg2)
{
    /* a non-literal lower bound keeps gcc from folding `>= 1` into
     * `> 0` (the ROM compares against 1) and from merging the two tests
     * into one unsigned range check */
    s32 lo = 1;

    if (arg2 >= lo && arg2 <= 4)
        InputCtrlKillPlayer(self, (void *)1);
}

void AttachInputCtrl(struct input_ctrl *self, struct ctrl_target *target)
{
    self->target = target;
}

void DestroyInputCtrl(struct input_ctrl *self, s32 flags)
{
    self->vtable = (struct ctrl_vtable *)gInputCtrlVtable;
    DestroyCtrl(self, flags);
}

struct input_ctrl *CreateInputCtrl(struct input_ctrl *self)
{
    InitCtrl(self);
    self->vtable = (struct ctrl_vtable *)gInputCtrlVtable;
    ResetInputCtrl(self);
    return self;
}

void sub_8017A20(struct input_ctrl *self)
{
    self->dirtyB = 1;
}

void sub_8017A28(struct input_ctrl *self)
{
    self->dirtyA = 1;
}

void sub_8017A30(struct input_ctrl *self)
{
    self->dirtyB = 0;
    self->altB = 0;
}

void sub_8017A38(struct input_ctrl *self)
{
    self->dirtyA = 0;
    self->altA = 0;
}

u8 sub_8017A40(struct input_ctrl *self)
{
    return self->dirtyB;
}
