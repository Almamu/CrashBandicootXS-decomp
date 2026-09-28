#include "core.h"

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see game_loop17.c's header comment and
 * docs/matching/issue-13-graphics-fc70.md). This is the last still-raw
 * function from that pass's leftover cluster - see
 * docs/matching/issue-13-fc70-second-continuation.md's "Still left raw"
 * section - now closed. Real bytes formerly the entirety of
 * asm/code_3_2_17_e560_ff0c.s (now deleted).
 *
 * **The `sub_800FF0C` entity-constructor trampoline family.** Two whole
 * files exist purely to call this one function with a fixed constant
 * `type` (the 5th, stack-passed argument): every `type` from `0` to
 * `0x12` (18) is externally confirmed by a caller -
 * src/graphics/graphics_loading_21bfc.c (`type` 0-7, issue #33) and
 * src/graphics/graphics_loading_21668.c (`type` 7-0x12, issue #31) -
 * matching this function's own internal 19-entry jump table
 * (case IDs 0-0x12) exactly. See docs/matching/issue-13-0x0800ff0c-
 * graphics.md for the full type-code-to-behavior table this cross-
 * reference makes possible, plus the register-allocation reasons this
 * stayed `NAKED`.
 *
 * Allocates a 0x64 (100)-byte object via `sub_8026EDC`, sets
 * `self+0x18` to `&gStaticData_087E4074` (a real address inside the
 * documented 93-entry `gStaticData_087Exxx` vtable family, but at a
 * `+0x18` offset - every other constructor this project has matched so
 * far uses `+0xC` for this same table-pointer convention; this function
 * is the first confirmed `+0x18` outlier), then runs
 * `sub_800FEB0(self)` and stores `arg0` at `self+8`.
 *
 * `type == 9` gets one early special-case: if `arg0 != 0xFFFF` and
 * `sub_802599C(*gUnknown_030012B4, arg0)` is true, `type` is forced to
 * `0`.
 *
 * Unless `gUnknown_030012C0+0x8c` is set, or the level's "how many of
 * this entity kind currently exist" counters (`sub_80232E0`/
 * `sub_8023128`, both called on `gUnknown_030012C0`) show the pool
 * isn't already at/over capacity, `type == 0xb` or `type == 0xf` gets
 * demoted via a placement-record flags byte (the `gUnknown_030012B4 ->
 * *P -> {+8 array, +0xc base}` indexed-by-`arg3<<1` convention
 * `sub_8021D04`, graphics_loading_21bfc.c, already established): flag
 * `0x40` forces `type = 2`, flag `0x80` forces `type = 1`, and (for the
 * `0xf` path only) placement-record byte `+1` bit `0x1` forces
 * `type = 9`.
 *
 * Sets `self+0x20` to `***gUnknown_030012D0 + 0x174` (the same
 * `self+0x20`-pointer-to-manager/`self+0x2d`-tag/0x1c-stride hitbox-
 * record table every sibling in this subsystem uses).
 *
 * **First jump table** (index `type - 1`, valid for `type` 1-15,
 * anything else skips straight to the second table): marks a "treat
 * this placement record as pre-flagged" bit (a local flag, call it
 * `special`) for `type` in `{1, 9, 11, 12, 15}`; for `type == 3`, if
 * the same placement-record lookup shows the entity is already present
 * (`sub_802599C(*gUnknown_030012B4, arg0)`), escalates `type` to `7`
 * outright, running the rest of the function as if `type` had been `7`
 * to begin with. Every other `type` in range is a no-op here.
 *
 * **Merge block**: looks up the placement record again; if `special`
 * is set, or the record's own flags byte has bit `0x20` set, sets a
 * second local flag (call it `flagged`) and computes `self+0x54` from
 * the record's `+4` halfword (`0x15` if it equals `0x1b`, else the raw
 * signed value) - then, if `gUnknown_030012C0+0x8c` is set, overwrites
 * `type` with `self->0x54 - 0x15`.
 *
 * **Second jump table** (index `type`, 0-0x12/18 - the one the
 * trampoline family's caller-side `type` constants directly select).
 * See docs/matching/issue-13-0x0800ff0c-graphics.md for the full case
 * table (which tag each case writes to `self+0x2d`, and what extra
 * fields each initializes); a couple of cases worth calling out here:
 * - `type == 5`: sets `self+0x50`/`+0x51`/`+0x48` from the placement
 *   record, tags `self+0x2d = 0x15`; later, if the placement record
 *   confirms the entity is present, calls `sub_800F5B8(self)` (matched
 *   as `NAKED` in game_loop49.c, issue #12) - a direct, concrete tie
 *   between this constructor family and that subsystem.
 * - `type == 0xf` (case `_0801028E`): the largest single case - looks
 *   up a tile/graphics asset via `sub_8006DF8`, masks `self+0x48`,
 *   tags `self+0x2d = 7`, looks up `gStaticData_0816BB94[(self->0x48 &
 *   0x38) >> 3]` for `self+0x4f`, and folds three placement-record
 *   `+1` flag bits (`0x2`/`0x4`/`0x8`) into `self+0x50`.
 *
 * **Common tail**: clears `self+0x28` bits `0x10`/`0x20`, folds
 * `sub_800815C(self)`'s low nibble into `self+0x29`'s low nibble,
 * writes `self+0/+4` (position) from `arg1<<8`/`arg2<<8`. If the
 * placement record confirms presence and `type` was `0xb` or `0xf`,
 * checks placement-record flag `0x80` to force `type = 1`. If `type ==
 * 1` and `arg0 != 0xFFFF` and the placement record confirms presence,
 * re-tags `self+0x2d = 0x1b` (27), reruns the sprite/animation trio,
 * and initializes `self+0x30` from the 28-byte-stride hitbox-record
 * table's own `+0x16` count (the same convention `sub_8010480`,
 * game_loop35.c, already established) minus one; also folds `type`'s
 * low bit into `self+0x4d` bit 0 (keeping bit 7). Writes
 * `self+0x4e = type` unconditionally. If `type == 5` and the placement
 * record confirms presence, calls `sub_800F5B8(self)` (see above).
 * Finally registers `self` via `sub_8009B70(gUnknown_0300130C, self)`
 * and returns `self`.
 *
 * Built with old_agbcc (the file is on the Makefile's OLD_AGBCC_OBJS;
 * under agbcc it is 8 bytes long and ~400 halfwords off). Was NAKED; the
 * later passes' notes are in docs/matching/issue-13-0x0800ff0c-graphics.md
 * and docs/matching/size2-naked-retry.md. The pieces that matter:
 * - The ROM copies the placement-record pointer in the 0xb pre-check,
 *   the `flagged` block and case 15 (`add r2,r0,#0` / `add r3,r1,#0` /
 *   `mov sl,r6`). Those are the copy from an inline function's return
 *   value: the record is read through `Placement(slot)` there, and
 *   through the plain `PLACEMENT(slot)` macro everywhere else.
 * - Case 15 masks `u48` with two `&=` statements (the ROM keeps both
 *   ands), passes the anim record through a pointer local (the ROM
 *   computes its address before loading gUnknown_030012B8) and indexes
 *   gStaticData_0816BB94 through an `idx` local (index before table).
 * - Case 11 builds its tag 0 with the constant-init asm so the `movs`
 *   comes before the tag address, as in the ROM.
 * - Three extra references to `type` at the end give it r7 and slot*2
 *   r8. */
#include "phys_obj.h"
extern void *sub_8026EDC(u32 size);
extern void sub_80084A4(void *self);
extern void sub_800FEB0(struct phys_obj *self);
extern u8 sub_802599C(void *level, s32 id);
extern s32 sub_80232E0(void *self);
extern s32 sub_8023128(void *self);
extern s32 sub_800815C(struct phys_obj *self);
extern void sub_8009B70(void *list, struct phys_obj *self);
extern void sub_800F5B8(struct phys_obj *self);
extern struct phys_obj_vtable gStaticData_087E4074;
extern u8 gStaticData_0816BB94[];
extern void *gUnknown_0300130C;

struct placement_level
{
    u8 unk_00[8];
    u16 *offsets;       // 0x08
    u8 *records;        // 0x0C
};

#define PLACEMENT_LEVEL (*(struct placement_level **)gUnknown_030012B4)
#define PLACEMENT(i) (PLACEMENT_LEVEL->records + PLACEMENT_LEVEL->offsets[i])
/* PLACEMENT as an inline: its return value is copied, which gives the
 * ROM's pointer copies (see the note above) */
static inline u8 *Placement(u16 i)
{
    return PLACEMENT(i);
}
void *sub_800FF0C(u16 id, u16 x, u16 y, u16 slot, u8 type)
{
    struct phys_obj *self;
    s32 special;
    s32 flagged;

    {
        void *obj = sub_8026EDC(0x64);

        sub_80084A4(obj);
        ((struct phys_obj *)obj)->vtable = &gStaticData_087E4074;
        ((struct phys_obj *)obj)->unk_59 = 0;
        sub_800FEB0(obj);
        self = obj;
    }
    self->id = id;
    if (type == 9 && id != 0xffff && sub_802599C(gUnknown_030012B4, id))
        type = 0;
    if (*((u8 *)gUnknown_030012C0 + 0x8c) == 0
        && sub_80232E0(gUnknown_030012C0) >= sub_8023128(gUnknown_030012C0))
    {
        if (type == 0xb)
        {
            u8 *rec = Placement(slot);

            if (rec[0] & 0x40)
                type = 2;
            else if (rec[0] & 0x80)
                type = 1;
            else if (rec[1] & 1)
                type = 9;
        }
        else if (type == 0xf)
        {
            u8 *rec = PLACEMENT(slot);

            if (rec[0] & 0x40)
                type = 2;
            else if (rec[0] & 0x80)
                type = 1;
            else if (rec[1] & 1)
                type = 9;
        }
    }
    special = 0;
    self->anim = (struct anim_table *)(**gUnknown_030012D0 + 0x174);
    switch (type)
    {
    case 1:
    case 9:
    case 11:
    case 12:
    case 15:
        special = 1;
        break;
    case 3:
        if (sub_802599C(gUnknown_030012B4, id))
            type = 7;
        break;
    }
    flagged = 0;
    {
        u8 *rec = Placement(slot);

        if (special || (rec[0] & 0x20))
        {
            flagged = 1;
            if (*(u16 *)(rec + 4) == 0x1b)
                self->unk_54 = 0x15;
            else
                self->unk_54 = *(s16 *)(rec + 4);
            if (*((u8 *)gUnknown_030012C0 + 0x8c))
                type = self->unk_54 - 0x15;
        }
    }
    switch (type)
    {
    case 0:
        PhysSetTag(self, 0x1f);
        break;
    case 1:
        self->unk_50 = (u32)(PLACEMENT(slot)[0] << 25) >> 31;
        PhysSetTag(self, 0x1a);
        break;
    case 2:
        PhysSetTag(self, 0x17);
        break;
    case 3:
        {
            u8 *rec = PLACEMENT(slot);

            PhysSetTag(self, 3);
            self->unk_50 = rec[6];
            self->unk_51 = rec[7];
            self->unk_4C = rec[8];
        }
        break;
    case 4:
        PhysSetTag(self, 0x18);
        break;
    case 5:
        {
            u8 *rec = PLACEMENT(slot);

            PhysSetTag(self, 0x15);
            self->unk_50 = rec[6];
            self->unk_51 = rec[7];
            self->u48.n = *(s16 *)(rec + 8);
        }
        break;
    case 6:
        PhysSetTag(self, 4);
        break;
    case 7:
        PhysSetTag(self, 0x20);
        break;
    case 8:
        PhysSetTag(self, 2);
        break;
    case 9:
        PhysSetTag(self, 0x1c);
        if (!flagged)
        {
            self->unk_54 = 0x15;
            if (*((u8 *)gUnknown_030012C0 + 0x8c))
                type = 0;
        }
        break;
    case 10:
        PhysSetTag(self, 5);
        break;
    case 11:
        {
            u8 *rec = PLACEMENT(slot);

            {
                u8 zero;

                /* constant-init: the 0 is loaded before the tag address */
                asm("" : "=r"(zero) : "0"(0));
                PhysSetTag(self, zero);
            }
            self->unk_51 = rec[6];
        }
        break;
    case 12:
        self->u48.n = -0x2a;
        PhysSetTag(self, 0x19);
        break;
    case 13:
        PhysSetTag(self, 6);
        break;
    case 14:
        PhysSetTag(self, 0x11);
        break;
    case 15:
        {
            u8 *rec = Placement(slot);

            {
                struct anim_rec *ar = &self->anim->records[8];

                sub_8006DF8(gUnknown_030012B8, ar->unk_14);
            }
            self->u48.n &= 0x3f;
            self->u48.n &= 0xf8;
            PhysSetTag(self, 7);
            {
                u32 idx = (u32)(self->u48.n & 0x38) >> 3;

                self->timer = gStaticData_0816BB94[idx];
            }
            self->unk_51 = rec[6];
            self->unk_50 = 0;
            if (rec[1] & 2)
                self->unk_50 = 1;
            if (rec[1] & 4)
                self->unk_50 |= 2;
            if (rec[1] & 8)
                self->unk_50 |= 4;
        }
        break;
    case 16:
        PhysSetTag(self, 0xe);
        break;
    case 17:
        PhysSetTag(self, 0xf);
        break;
    case 18:
        PhysSetTag(self, 0x10);
        break;
    }
    self->flipX = 0;
    self->flipY = 0;
    self->slot = sub_800815C(self);
    self->x = x << 8;
    self->y = y << 8;
    if (sub_802599C(gUnknown_030012B4, id) && (type == 0xb || type == 0xf)
        && (PLACEMENT(slot)[0] & 0x80))
        type = 1;
    if (type == 1 && id != 0xffff && sub_802599C(gUnknown_030012B4, id))
    {
        PhysSetTag(self, 0x1b);
        self->frame = self->anim->records[self->tag].frames - 1;
        self->state = (self->state & 0x80) | type;
    }
    self->kind = type;
    /* three extra references to `type`: raise its allocation priority so
     * it gets r7 and slot*2 gets r8, as in the ROM (one or two are not
     * enough) */
    asm("" : : "r"(type));
    asm("" : : "r"(type));
    asm("" : : "r"(type));
    if (type == 5 && sub_802599C(gUnknown_030012B4, id))
        sub_800F5B8(self);
    sub_8009B70(gUnknown_0300130C, self);
    return self;
}
