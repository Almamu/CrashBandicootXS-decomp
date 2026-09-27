#include "core.h"

/* GitHub issue #21: 0x08017524-0x08017A44, the whole tail of the former
 * asm/code_3_2_17_16048.s.
 *
 * The first six functions are byte accessors for an object not otherwise
 * characterized here (`+0x2C`/`+0x2D` "has value" flags for the bytes at
 * `+0x24`/`+0x25`).
 *
 * The other 19 are one self-contained actor-part subclass,
 * `struct input_ctrl`, whose method table is `gStaticData_087E42F4`
 * (constructor `sub_8017A00` - called from game_loop39.c - destructor
 * `sub_80179EC`; every other slot it calls through is a base-class
 * `sub_800B6xx`/`sub_800B8xx` function). Each frame (`sub_8017650`, table
 * slot +0x0C) it reads the held D-pad bits from `gUnknown_030007E0` and
 * picks animation pairs for its target (`+0x10`) through
 * `gStaticData_0816B8C0`'s 12-byte records: up/down select one channel
 * (`animB`), left/right the other (`animA`), which also sets a speed-like
 * value at the child object's `+0x78` (`+0x1C`, spawned on demand by
 * `sub_8017600`). Once the target passes the level's right edge
 * (`gUnknown_03001308`'s layer 0 width, less 0xA00) the child is marked
 * gone and `sub_80241A4` is signalled. It then dispatches the current
 * `state` through `gStaticData_0816C290`, a table of gcc 2.x
 * pointer-to-member-functions: state 0 `sub_8017600`, 1 `sub_801796C`,
 * 2 `sub_801793C`, 3 `sub_80178EC`. That call sequence - and the
 * `sub_803AD7C`/`AD80`/`AD84` "call via r1/r2/r3" trampolines used for
 * every virtual call - is what gcc's C++ front end emits, so this object
 * was very likely written in C++.
 *
 * UNUSED - no `bl`/`.4byte` reference in the asm/ sources or any .c file under
 * src/, and no Thumb pointer anywhere in the ROM: `sub_8017524`,
 * `sub_801752C`, `sub_8017534`, `sub_801753C`, `sub_8017544`,
 * `sub_8017554`, `sub_8017A20`, `sub_8017A28`, `sub_8017A30`,
 * `sub_8017A38`, `sub_8017A40`. Matched anyway.
 *
 * Matching notes (details in docs/matching/issue-21-input-ctrl.md): the
 * virtual-call macros take the method-table entry's address once; the
 * `SetAnimA`/`SetAnimB`/`SetChildSpeed` inline helpers reproduce the ROM
 * evaluating the stored constant before the store's own loads; the
 * "mark actor gone" bitmap sequence (sub_80072D8's, inlined twice) needs
 * register pins but no inline asm - its word index comes from a signed
 * division of the zero-extended id; a few other spots are register-pinned
 * where noted. */

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
    u16 field_08;     // 0x08 - bitmap id (see sub_80072D8)
    u8 unk_0A[2];
    u8 flags;         // 0x0C
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
    u8 unk_104;       // 0x104
};

struct ctrl_child
{
    s32 x;            // 0x00
    s32 y;            // 0x04
    u16 field_08;     // 0x08
    u8 unk_0A[2];
    u8 flags;         // 0x0C
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

extern void *gUnknown_030012BC;
extern void *gUnknown_030012C0;
extern void *gUnknown_030012B8;
extern void *gUnknown_030012B4;
extern void *gUnknown_030012F0;
extern u32 gUnknown_030007E0; /* low half: held keys */
extern struct { u8 unk_00[0x10]; struct { u8 unk_00[0x10]; s32 width; } *layer0; } *gUnknown_03001308;
extern struct pmf gStaticData_0816C290[];
extern u8 gStaticData_0816B8C0[];
extern u8 gStaticData_087E42F4[];

extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern s32 sub_803AD80(void *self, s32 arg, void *fn);
extern s32 sub_803AD84(void *self, void *arg1, void *arg2, void *fn);
extern void sub_8023234(void *arg0);
extern void sub_8006D08(void *self, s32 slot, s32 recordId);
extern void *sub_8026EDC(u32 size);
extern struct ctrl_child *sub_801B940(void *mem);
extern void sub_801B864(struct ctrl_child *child);
extern void sub_8008E94(void *manager, void *value);
extern void sub_80241A4(void);
extern void sub_800B8A8(void *self, s32 flags);
extern void sub_800B8C8(void *self);

/* A virtual call as gcc 2.x lowers it: take the method-table entry's
 * address once, then read its `this` adjustment and function from it. */
#define CTRL_CALL2(obj, m, a)                                                  \
    do                                                                         \
    {                                                                          \
        struct ctrl_method *_m = &(obj)->vtable->m;                            \
        sub_803AD80((u8 *)(obj) + _m->thisOffset, (a), _m->fn);                \
    } while (0)
#define CTRL_CALL3(obj, m, a, b)                                               \
    do                                                                         \
    {                                                                          \
        struct ctrl_method *_m = &(obj)->vtable->m;                            \
        sub_803AD84((u8 *)(obj) + _m->thisOffset, (a), (b), _m->fn);           \
    } while (0)

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
void sub_80179BC(struct input_ctrl *self);

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


void sub_8017564(struct input_ctrl *self, void *arg)
{
    PlaySfx(gUnknown_030012BC, 0x1B, 0x100);
    CTRL_CALL2(self, method_20, 3);
    CTRL_CALL3(self, method_50, self->target, arg);
    {
        register struct ctrl_target *t asm("r1") = self->target;
        register s32 m asm("r0") = 0x7F;
        register s32 f asm("r2") = t->flags;

        m &= f;
        t->flags = m;
    }
    {
        register struct ctrl_target *t asm("r1") = self->target;
        register s32 m asm("r0") = -0x41;
        register s32 f asm("r5") = t->flags;

        m &= f;
        t->flags = m;
    }
    {
        register u8 *p asm("r0") = (u8 *)self->target;
        register s32 off asm("r1") = 0x104;

        asm("" : "+r"(off));
        p += off;
        *p = 1;
    }
    sub_8023234(gUnknown_030012C0);
    {
        void *cache = gUnknown_030012B8;
        register struct ctrl_target *t asm("r3") = self->target;

        u32 slot = t->slot;
        register u8 *records asm("r4");
        register u32 tag asm("r5");

        {
            void *table = t->table;
            u8 *tagp = &t->tag;

            records = ((struct { u8 *records; } *)table)->records;
            tag = *tagp;
        }
        asm("" : "+r"(records), "+r"(tag));
        sub_8006D08(cache, slot, records[tag * 28 + 0x14]);
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
        self->child = sub_801B940(sub_8026EDC(0x80));
        sub_8008E94(gUnknown_030012F0, self->child);
    }
    sub_801B864(self->child);
}


void sub_8017650(struct input_ctrl *self)
{
    if (self->state != 3)
    {
        u32 keys;
        s32 x = self->target->x;

        if (x > (gUnknown_03001308->layer0->width << 8) - 0xA00)
        {
            register struct ctrl_child *c asm("r1") = self->child;

            {
                register s32 v asm("r0") = 1;
                register s32 f asm("r3") = c->flags;

                v |= f;
                c->flags = v;
            }
            {
                register s32 none asm("r0") = 0xFFFF;
                register u32 cur asm("r5") = c->field_08;

                if (cur != none)
                {
                    register s32 id asm("r3") = *(vu16 *)&c->field_08;
                    u8 *base = gUnknown_030012B4;
                    register s32 word asm("r0") = id;
                    s32 off;
                    register u32 *slot asm("r2");

                    word /= 32;
                    off = word * 4;
                    {
                        register s32 k asm("r6") = 0x108;

                        asm("" : "+r"(k));
                        slot = (u32 *)(base + k);
                    }
                    slot = (u32 *)((u8 *)slot + off);
                    word = id - word * 32;
                    *slot |= 1 << word;
                }
            }
            self->child = NULL;
            sub_80241A4();
        }

        keys = gUnknown_030007E0;
        if ((keys & DPAD_UP) && self->dirState != 1)
        {
            SetAnimB(self, 3);
            self->dirState = 1;
        }
        else
        {
            if (keys & DPAD_DOWN)
            {
                register u32 d asm("r1") = self->dirState;

                if (d != 2)
                {
                    SetAnimB(self, 5);
                    self->dirState = 2;
                    goto dir_done;
                }
            }
            if (!(keys & (DPAD_UP | DPAD_DOWN)))
            {
                SetAnimB(self, 0);
                self->dirState = 0;
            }
        }
    dir_done:

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
        s32 idx = gStaticData_0816C290[self->state].index;
        struct pmf_entry e;
        void *fn;

        if (idx > 0)
        {
            e = (*(struct pmf_entry **)((u8 *)self + gStaticData_0816C290[self->state].u.vtableOffset))[idx - 1];
            fn = e.fn;
        }
        else
        {
            fn = gStaticData_0816C290[self->state].u.fn;
        }
        {
            s32 d = gStaticData_0816C290[self->state].delta;
            s32 adj;

            if (idx > 0)
                adj = e.delta + d;
            else
                adj = d;
            sub_803AD80((u8 *)self + adj, d, fn);
        }
    }
    sub_8017808(self);
}

/* The register pins below are load-bearing (docs/workflow.md step 7):
 * the ROM keeps the table base in r1, the index byte in r2 and the
 * scaled sum in r0 (`off + base` order), and re-reads `dirtyB` into r2;
 * every unpinned arrangement tried lands these in other registers. */
void sub_8017808(struct input_ctrl *self)
{
    if (self->dirtyA == 1)
    {
        register struct anim_pair *e asm("r1") = self->animSet->entries;
        register u32 idx asm("r2") = self->animA;
        register struct anim_pair *entry asm("r0");
        u8 *rec;

        asm("" : "+r"(idx));
        entry = (struct anim_pair *)((idx << 3) + (u32)e);
        rec = gStaticData_0816B8C0 + entry->a * 12;
        if (self->altA)
            CTRL_CALL3(self, method_38, self->target, rec);
        else
            CTRL_CALL3(self, method_28, self->target, rec);
        self->dirtyA = 0;
        self->altA = 0;
    }
    {
        register u32 dirty asm("r2") = self->dirtyB;
        if (dirty == 1)
        {
            register struct anim_pair *e asm("r1") = self->animSet->entries;
            register u32 idx asm("r2") = self->animB;
            register struct anim_pair *entry asm("r0");
            u8 *rec;

            asm("" : "+r"(idx));
            entry = (struct anim_pair *)((idx << 3) + (u32)e);
            rec = gStaticData_0816B8C0 + entry->b * 12;
            if (self->altB)
                CTRL_CALL3(self, method_40, self->target, rec);
            else
                CTRL_CALL3(self, method_30, self->target, rec);
            self->dirtyB = 0;
            self->altB = 0;
        }
    }
}

void sub_80178BC(struct input_ctrl *self, s32 mode, void *arg, s32 unused3, s32 unused4)
{
    CTRL_CALL2(self, method_20, mode);
    CTRL_CALL3(self, method_50, self->target, arg);
}

/* The same "mark actor gone" sequence as sub_80072D8 (graphics.c), inlined:
 * set flags bit 0, then unless the id is 0xFFFF set its bit in the
 * gUnknown_030012B4+0x108 bitmap. Register pins are load-bearing
 * (docs/workflow.md step 7) - they reproduce the ROM's allocation,
 * including its use of callee-saved r4 in a leaf function. The id is
 * re-read (`volatile`) after the 0xFFFF test, and the word index comes
 * from a *signed* division of that zero-extended value, which is what
 * produces the ROM's copy + `asr #5` + subtract (no inline asm needed). */
void sub_80178EC(struct input_ctrl *self)
{
    register struct ctrl_target *t asm("r1") = self->target;

    if (t->unk_38)
    {
        {
            register s32 v asm("r0") = 1;
            register s32 f asm("r2") = t->flags;

            v |= f;
            t->flags = v;
        }
        {
            register s32 none asm("r0") = 0xFFFF;
            register u32 cur asm("r4") = t->field_08;

            if (cur != none)
            {
                register s32 id asm("r3") = *(vu16 *)&t->field_08;
                u8 *base = gUnknown_030012B4;
                register s32 word asm("r0") = id;
                s32 off;
                u32 *slot;

                word /= 32;
                off = word * 4;

                slot = (u32 *)(base + 0x108);
                slot = (u32 *)((u8 *)slot + off);
                word = id - word * 32;
                *slot |= 1 << word;
            }
        }
    }
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

void sub_80179BC(struct input_ctrl *self)
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
        sub_8017564(self, (void *)1);
}

void sub_80179E8(struct input_ctrl *self, struct ctrl_target *target)
{
    self->target = target;
}

void sub_80179EC(struct input_ctrl *self, s32 flags)
{
    self->vtable = (struct ctrl_vtable *)gStaticData_087E42F4;
    sub_800B8A8(self, flags);
}

struct input_ctrl *sub_8017A00(struct input_ctrl *self)
{
    sub_800B8C8(self);
    self->vtable = (struct ctrl_vtable *)gStaticData_087E42F4;
    sub_80179BC(self);
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
