#include "core.h"
#include "phys_obj.h"

/* GitHub issue #12: 0x0800D040-0x0800FC70, the physics/collision
 * subsystem (see game_loop6.c's header comment and
 * docs/matching/issue-12-physics-collision.md). Phase 2, higher-address
 * half: the twelve functions from `sub_800EEF0` through `sub_800F990`
 * (0x0800EEF0-0x0800FC70, the end of this whole cluster), all direct or
 * transitive callees of `sub_0800D18C`'s and `sub_800E08C`'s per-edge
 * jump table (game_loop47.c) - see that issue doc's "Phase 2 grouping
 * hint" for the confirmed dispatch map this group is built from. Real
 * bytes formerly the tail of asm/code_3_2_17_e560.s (from
 * `sub_800EEF0` onward - the head, `sub_800E560` through `sub_800EDBC`,
 * is a sibling pass's territory and untouched here).
 *
 * Compiled with old_agbcc (the Makefile's OLD_AGBCC_OBJS): all twelve
 * are real C (see docs/matching/issue-12-physics-collision.md's
 * NAKED-retry sections). */

extern struct phys_obj_list *gUnknown_0300130C;
extern u8 gUnknown_030012B0;
extern void sub_800EEF0(struct phys_obj *self, u8 arg1);
extern void sub_8009AA0(struct phys_obj_list *list, s32 index);
extern void *gUnknown_030012BC;
extern void *gUnknown_03001318;
extern void PlaySfx(void *ctx, s32 id, s32 volume);
extern void sub_8028474(void *arg);
extern void sub_802306C(void *arg);
extern void sub_800F258(void);
extern void sub_800F5B8(struct phys_obj *self);
extern s32 sub_800815C(void *self);
extern void sub_8026EB4(void *p);
extern u8 gStaticData_0816BBC4[];
extern u8 gStaticData_0816BBAE[];
extern void sub_800E6B0(void *self);
extern void sub_8011448(struct phys_obj *obj, s32 arg);
extern void sub_800F368(struct phys_obj *self);
extern void sub_800F2BC(struct phys_obj *self);
extern void sub_8009150(struct phys_obj_list *list, struct phys_obj *obj);
extern void sub_8025A0C(u8 *bitmap, u16 id);
extern void *sub_8026EC0(u32 size);
extern void *sub_8010708(void *obj);
extern void *sub_801070C(void *obj);
extern void sub_8010710(void *obj, void *prev);
extern void sub_8010714(void *obj, void *next);
extern void sub_800F06C(struct phys_obj *self, s32 dist);
extern u8 gStaticData_0816BB98[];
extern void sub_8022FEC(void *arg);
extern void sub_800EDBC(struct phys_obj *self);
extern void sub_800E7A8(void *self, u32 arg1, u32 arg2, u32 arg3);
extern u32 sub_8010A50(struct phys_obj *self);
extern u8 gStaticData_0816BB94[];


/* Per-edge jump table's **case 4 handler**
 * (`sub_0800D18C(self+0x4d & 0x7f == 0) -> sub_800EEF0(self, 1)`, and
 * `sub_800E08C`'s own case 4, per game_loop47.c's confirmed dispatch
 * map). Also called by several of this file's own sibling functions
 * (`sub_800F06C`, `sub_800F258`, `sub_800F6B8`, `sub_800F8E0`) whenever
 * their own overlap/state checks land on the same "commit an edge
 * collision" outcome, always with `arg1` (a `u8`) as either 0 or 1.
 *
 * Early-outs when `self+0x4d & 0x7f == 1` (already committed). Clears
 * `self+0x4f`, clears `self+0x4d`'s low 7 bits, resets
 * `gUnknown_030012D8+0x80`, sets `self+0xc` bit `0x10` (a "collision
 * response active" render/update flag matched elsewhere in this
 * subsystem), and calls `sub_8009150(gUnknown_0300130C, self)` (adds
 * `self` back onto the shared active-object list). Sets `self+0x4d`'s
 * `0x80` bit unconditionally, then ORs in `arg1` on top of that -
 * `arg1` ends up as the low bit of `self+0x4d`.
 *
 * If `self+0x4e == 0xa` (a specific collision state id), rewinds
 * `self+0x4e` by `0x21` (`0xa -> ... ` - stores through
 * `self+0x4e - 0x21`, i.e. a computed offset elsewhere in `self`'s
 * struct) instead of the usual `self+0x2d = 0x21` tag write, then in
 * either case calls the `sub_80087C0`/`sub_80087B4`/`sub_800872C`
 * triplet (the "set tag, refresh sprite/animation" idiom shared by
 * every state-transition function in this cluster - see
 * `sub_800F2BC`/`sub_800F368`/`sub_800F5B8`/`sub_800F8E0` below for the
 * same three-call pattern).
 *
 * Looks up `gStaticData_0816BB98[self+0x4e]` and, if nonzero, calls
 * `sub_8022FEC(gLevelState)` (an external subsystem, unread -
 * likely a screen-shake/particle trigger). Sets a bit in
 * `gEntityFlags`'s 32x32 collision-cell bitmap from `self+8`'s
 * position (`>>5` row, `&0x1f` column - the same cell-grid convention
 * `sub_0800D18C` itself uses for `gUnknown_030012D8`'s own state), then
 * plays a fixed sound (`gUnknown_030012BC`, id 4). Calls
 * `sub_800EDBC(self)` (already matched elsewhere in this cluster - a
 * sibling's territory).
 *
 * Tail: reads `gUnknown_0300082C`'s `+0xc` byte bit `0x40` (a "combo
 * scoring active" flag elsewhere in the ROM); if set, and
 * `gUnknown_030012D8+0x8c` (a running counter) hasn't exceeded
 * `gUnknown_0300082C`'s threshold, and `self`'s position is within 0x1d
 * px of `gUnknown_030012D8`'s (the player) on both axes (or `arg1`
 * itself is 0), calls `_call_via_r4` (the `bx r4` sound/particle
 * trampoline from `self+0x18+0x68`) with args `(0, 4, 0)`. Finally, if
 * `self+0x4e != 0xa`, forces `self+0x4e = 0x13` (a shared "settle"
 * state most of this cluster's state machines converge on - see
 * `sub_800F8E0` below). */

static inline s32 PhysComboMaxed(struct gobj *player)
{
    s32 maxed = FALSE;

    if (player->deadline > gUnknown_0300082C)
        maxed = TRUE;
    return maxed;
}

void sub_800EEF0(struct phys_obj *self, u8 near)
{
    u8 one;

    if ((self->state & 0x7f) == 1)
        return;

    self->timer = 0;
    self->state &= 0x7f;
    PHYS_PLAYER->busy = 0;
    self->flags |= 0x10;
    sub_8009150(gUnknown_0300130C, self);
    one = 1;
    self->state = (self->state & 0x80) | one;
    if (self->kind == 0xa) {
        self->tag = one;
        sub_80087C0(self);
        sub_80087B4(self);
        sub_800872C(self, 0);
    } else
        PhysSetTag(self, 0x21);
    if (gStaticData_0816BB98[self->kind])
        sub_8022FEC(gLevelState);
    PHYS_SET_ID_BIT(self->id);
    PlaySfx(gUnknown_030012BC, 4, 0x100);
    sub_800EDBC(self);

    if ((gUnknown_030012D8->flags >> 6) & 1 && !PhysComboMaxed(gUnknown_030012D8)) {
        struct gobj *p;
        s32 t1 = (gUnknown_030012D8->x >> 8) - (self->x >> 8);
        s32 dx = (t1 ^ (t1 >> 31)) - (t1 >> 31);

        if (dx <= 0x1d) {
            s32 t2 = (gUnknown_030012D8->y >> 8) - (self->y >> 8);
            s32 dy = (t2 ^ (t2 >> 31)) - (t2 >> 31);

            if (dy <= 0x1d)
                goto call;
        }
        if (near) {
        call:
            p = gUnknown_030012D8;
            PhysCall3(p, &p->vtable->m68, 0, 4, 0);
        }
    }
    if (self->kind != 0xa)
        self->kind = 0x13;
}

/* Called from `sub_800F798` (`sub_800F06C(self, 0x14)` /
 * `sub_800F06C(self, 0x28)`, gated on `self+0x30 == 3` / `== 6`) with
 * `arg1` a small proximity-radius constant (0x14 or 0x28 px). Walks
 * `gUnknown_0300130C`'s whole object list twice:
 *
 * - First pass: for every other object whose `_call_via_r1`
 *   overlap-classification against `self` returns `3` (a "close enough
 *   to interact" code shared with several siblings below) and whose
 *   Chebyshev-ish `|dx|+|dy|` distance to `self` is within `arg1`,
 *   looks up `gStaticData_0816BBC4[other+0x4e]`: if nonzero, calls
 *   `sub_800E7A8(other, 1, 0, 0)`; else if the *other* object's own
 *   `gStaticData_0816BBAE` byte (keyed by that lookup's result) is set,
 *   calls `sub_800E7A8(other, 1, 0, 0)`; else dispatches on that byte's
 *   value (`3` -> `sub_800F368(other)`, `6` -> `sub_800F2BC(other)`) -
 *   but only when `other+0x4d & 0x7f == 0` and it's not already flagged
 *   via `gUnknown_030012EC`. (Every object that passes the overlap
 *   check but isn't otherwise routed still gets `sub_800EEF0(other, 0)`
 *   when `gUnknown_030012EC[other+0x4e]` is nonzero, before falling
 *   into that dispatch.)
 * - Second pass over `gUnknown_030012EC`'s smaller secondary list:
 *   objects with `_call_via_r1 == 2` and the same distance gate get
 *   `sub_8011448(other, 1)` (already matched elsewhere) and an
 *   `other+0xc` bit-`0x10` set (same render/update flag `sub_800EEF0`
 *   sets above).
 *
 * Ends by resetting `self+0x48` to `-1` (0xFFFFFFFF), a sentinel this
 * whole cluster uses for "no pending sub-state timer". */

void sub_800F06C(struct phys_obj *self, s32 dist)
{
    s32 i = 0;

    if (i < gUnknown_0300130C->count) {
        u32 commit = (u32)gStaticData_0816BBC4;

        do {
            struct phys_obj *o = gUnknown_0300130C->items[i];

            if (PHYS_CALL(o, m48) == 3) {
                s32 t1 = (o->x >> 8) - (self->x >> 8);
                s32 dx = (t1 ^ (t1 >> 31)) - (t1 >> 31);
                s32 t2 = (o->y >> 8) - (self->y >> 8);
                s32 dy = (t2 ^ (t2 >> 31)) - (t2 >> 31);

                if (dx + dy <= dist && (o->state & 0x7f) == 0) {
                    u32 kind = o->kind;

                    if (*(u8 *)(kind + commit))
                        sub_800EEF0(o, 0);
                    else if (gStaticData_0816BBAE[kind])
                        sub_800E7A8(o, 1, 0, 0);
                    else if (kind == 3)
                        sub_800F368(o);
                    else if (kind == 6)
                        sub_800F2BC(o);

                }
            }
            i++;
        } while (i < gUnknown_0300130C->count);
    }

    i = 0;
    if (i < ((struct phys_obj_list2 *)gUnknown_030012EC)->count) {
        struct phys_obj_list2 **list = (struct phys_obj_list2 **)&gUnknown_030012EC;

        do {
            struct phys_obj *o = (*list)->items[i];

            if (PHYS_CALL(o, m48) == 2) {
                s32 t1 = (o->x >> 8) - (self->x >> 8);
                s32 dx = (t1 ^ (t1 >> 31)) - (t1 >> 31);
                s32 t2 = (o->y >> 8) - (self->y >> 8);
                s32 dy = (t2 ^ (t2 >> 31)) - (t2 >> 31);

                if (dx + dy <= dist) {
                    sub_8011448(o, 1);
                    o->flags |= 0x10;
                }
            }
            i++;
        } while (i < (*list)->count);
    }
    self->u48.n = 0xff;
}

/* Takes no arguments - a pure `gUnknown_0300130C` list-scan helper,
 * called from the still-raw `sub_80232C8`/`0x08023A1C` caller elsewhere
 * (outside this issue's scope). First calls `sub_800F258` (below) to
 * settle any pending case-`0xa` collisions, then loops
 * `gUnknown_0300130C` up to twice (an outer `do { ... } while
 * (gUnknown_030012B0)` driven by a one-shot re-scan flag stored at
 * `gUnknown_030012B0`): for every object whose `_call_via_r1`
 * classification against `self` is `3` and whose `+0xc` bit `1` is set,
 * calls `sub_8009AA0(list, index)` (an already-elsewhere-matched
 * list-removal helper) and, if that object is still non-NULL
 * afterward, `_call_via_r2(other, 3, ...)` (a variant of the
 * `_call_via_r1` overlap-classifier that also *mutates* state, per the
 * `3` id) - decrementing the loop index to re-visit the same slot next
 * iteration since the list just shrank. Objects that overlap but don't
 * have that `+0xc` flag instead get a plain `_call_via_r1` call against
 * a *different* box (`other+0x18+0x18`/`+0x1c`, not `+0x48`/`+4`) with
 * no further action - just a classification side effect. */

void sub_800F1B8(void)
{
    s32 i;

    sub_800F258();
    do {
        gUnknown_030012B0 = 0;
        for (i = 0; i < gUnknown_0300130C->count; i++) {
            struct phys_obj *o = gUnknown_0300130C->items[i];

            if (PHYS_CALL(o, m48) == 3) {
                if (o->flags & 1) {
                    sub_8009AA0(gUnknown_0300130C, i);
                    if (o != NULL)
                        PHYS_CALL1(o, m50, 3);
                    i--;
                } else {
                    PHYS_CALL(o, m18);
                }
            }
        }
    } while (gUnknown_030012B0);
}

/* Takes no arguments. A short `gUnknown_0300130C` list-scan: for every
 * object whose `_call_via_r1` overlap-classification against `self` is
 * `3`, whose `+0x4e` state is `0xa`, and whose `+0x4d & 0x7f` is clear,
 * calls `sub_800EEF0(other, 0)` - i.e. settles any object still parked
 * in the "pending edge-4 commit, state 0xa" condition `sub_800EEF0`
 * itself creates (see that function's own doc comment above). Called
 * as the first step of both `sub_800F1B8` (above) and `sub_800F2BC`
 * (below), always as a "flush anything left over from a previous
 * frame" pass before running this frame's own dispatch. */

void sub_800F258(void)
{
    s32 i = 0;

    if (i < gUnknown_0300130C->count) {
        struct phys_obj_list **list = &gUnknown_0300130C;

        do {
            struct phys_obj *o = (*list)->items[i];

            if (PHYS_CALL(o, m48) == 3 && o->kind == 0xa) {
                if ((o->state & 0x7f) == 0)
                    sub_800EEF0(o, 0);
            }
            i++;
        } while (i < (*list)->count);
    }
}

/* Per-edge jump table's **case 0/1 handler when the dispatch-id row is
 * 6** (`sub_0800D18C`'s/`sub_800E08C`'s shared case 0/1 target - see
 * game_loop47.c's confirmed dispatch map). Early-outs when `self+0x48`
 * is already nonzero (a pending sub-state timer, same field
 * `sub_800F06C` resets to `-1`).
 *
 * Sets `self+0x4d` bit `0x80`, `gUnknown_030012D8+0x80 = 1`, tags
 * `self+0x2d = 0x23` (a "bounced/deflected" state constant, matching
 * this cluster's numbering - `sub_800F368` below uses `0x22` for a
 * closely related case), then runs the
 * `sub_80087C0`/`sub_80087B4`/`sub_800872C` "set tag, refresh
 * sprite/animation" triplet every state-transition function in this
 * cluster shares. Looks up `self`'s hitbox-record row
 * (`self+0x20`-table/`self+0x2d`-tag/28-byte-stride, this subsystem's
 * standard AABB convention) and calls `sub_8006DF8` with its `+0x14`
 * byte to compute a direction/animation nibble, folded into `self+0x29`
 * (low nibble replaced, high nibble kept - `(x & 0xf) | (old & ~0xf)`).
 * Calls `sub_800F258` (flush any pending case-0xa commits), then
 * `sub_8028474(gUnknown_03001318)` (external, unread - likely a score
 * or combo-counter bump) and plays a fixed sound
 * (`gUnknown_030012BC`, id 4). Sets `self+0x48 = 1` (arms the sub-state
 * timer `sub_800F06C` later drains back to `-1`) and calls
 * `sub_802306C(gLevelState)` (external, unread). */

void sub_800F2BC(struct phys_obj *self)
{
    if (self->u48.n == 0) {
        s32 one;
        struct anim_rec *recs;
        struct anim_rec *rec;

        self->state |= 0x80;
        {
            struct gobj *player = gUnknown_030012D8;
            one = 1;
            player->unk_80 = one;
        }
        PhysSetTag(self, 0x23);
        recs = self->anim->records;
        rec = &recs[self->tag];
        self->slot = sub_8006DF8(gUnknown_030012B8, rec->unk_14);
        sub_800F258();
        sub_8028474(gUnknown_03001318);
        PlaySfx(gUnknown_030012BC, 4, 0x100);
        self->u48.n = one;
        sub_802306C(gLevelState);
    }
}

/* Per-edge jump table's **case 0/1 handler when the dispatch-id row is
 * 3** (the sibling of `sub_800F2BC` above, same dispatch-map entry, and
 * also called directly by `sub_800F06C`'s/`sub_800F6B8`'s/
 * `sub_800D040`'s own dispatch). Early-outs when `self+0x48` is already
 * `-1` or `0` cleared to the "already handled" sentinels (i.e. only
 * proceeds while it's some other in-progress value) - the inverse
 * early-out shape from `sub_800F2BC`'s simple "nonzero" check.
 *
 * Sets `self+0xc` bit `0x10`, calls `sub_8009150(gUnknown_0300130C,
 * self)` (re-adds `self` to the active list, same call `sub_800EEF0`
 * makes), sets `self+0x4d` bit `0x80` and `gUnknown_030012D8+0x80 = 1`,
 * tags `self+0x2d = 0x22` (this case's own state constant), and runs
 * the same `sub_80087C0`/`sub_80087B4`/`sub_800872C` triplet plus the
 * `sub_8006DF8`-driven `self+0x29` nibble update `sub_800F2BC` uses.
 * Calls `sub_8025A0C(gEntityFlags, self+8)` (marks `self`'s
 * position in the same 32x32 collision-cell bitmap `sub_800EEF0`
 * touches).
 *
 * Then walks `gUnknown_0300130C`'s whole list a *second* time (distinct
 * from the `_call_via_r1`-classification passes above): collects up to
 * 0x20 other objects whose `_call_via_r1` result is `3`, `+0x4d & 0x7f
 * == 0`, `+0x4e == 5`, and `+0x50` matches `self+0x50`, into a local
 * stack array, calling `sub_8025A0C` on each of *their* positions too.
 * If any were collected, allocates a heap block sized for the count
 * (`sub_8026EC0`), sets `self+0x59 = 1`, and copies the collected
 * pointer array into it before storing the block at `self+0x48` -
 * building a "linked group of simultaneously-triggered neighbors" list.
 * If none were collected, `self+0x48` gets the `-1` sentinel instead.
 * Tail: clears `self+0x4f` and copies `self+0x4c`'s byte into
 * `self+0x4f` (per-object throttle fields also touched by
 * `sub_800F4F4`/`sub_800F5B8` below). */

void sub_800F368(struct phys_obj *self)
{
    struct phys_obj *found[32];
    s32 n = 0;
    s32 i;

    if (self->u48.group == PHYS_NO_GROUP)
        return;
    if (self->u48.group != NULL)
        return;

    self->flags |= 0x10;
    sub_8009150(gUnknown_0300130C, self);
    self->state |= 0x80;
    {
        struct gobj *player = gUnknown_030012D8;
        u8 one = 1;
        player->unk_80 = one;
    }
    PhysSetTag(self, 0x22);
    {
        struct anim_rec *recs = self->anim->records;
        struct anim_rec *rec = &recs[self->tag];

        self->slot = sub_8006DF8(gUnknown_030012B8, rec->unk_14);
    }
    sub_8025A0C(gEntityFlags, self->id);

    i = 0;
    if (i < gUnknown_0300130C->count) {
        do {
            struct phys_obj *o = gUnknown_0300130C->items[i];

            if (PHYS_CALL(o, m48) == 3 && (o->state & 0x7f) == 0) {
                if (o->kind == 5 && o->unk_50 == self->unk_50) {
                    found[n] = o;
                    n++;
                    n &= 0x1f;
                    sub_8025A0C(gEntityFlags, o->id);
                }
            }
            i++;
        } while (i < gUnknown_0300130C->count);
    }

    if (n != 0) {
        struct phys_group *g = sub_8026EC0((n + 1) * 4);

        self->unk_59 = 1;
        g->count = n;
        for (i = 0; i < n; i++)
            ((struct phys_obj **)g)[i + 1] = found[i];
        self->u48.group = g;
    } else {
        self->u48.group = PHYS_NO_GROUP;
    }
    self->unk_50 = 0;
    self->timer = self->unk_4C;
}

/* Called from `sub_800F5B8`'s own jump-table-driven state machine
 * indirectly via re-entry (see below) and from the still-raw
 * `0x080104E4` continuation (outside this issue's scope) whenever
 * `self+0x4e` is in `0x13`-`0x15`. Bumps `self+0x50` (a per-object
 * "successive triggers" counter) and compares it against `self+0x51`
 * (a per-object cap). Once the cap is reached: if `self+0x48` (the
 * linked-group pointer `sub_800F368` builds) holds more than one
 * element, calls `sub_8026EB4` (frees it, already-elsewhere-matched);
 * resets `self+0x48` to `-1` and `self+0x4e = 7` (a distinct "group
 * exhausted" state).
 *
 * While still under the cap: if `self+0x48`'s group has more than one
 * member, walks every member whose own `+0x4e == 5` and `+0x51 <=
 * self`'s own cached `+0x4c` throttle byte, calling `sub_800F5B8`
 * (below) on each - recursively settling every other object in the
 * same triggered group - and plays a single shared sound
 * (`gUnknown_030012BC`, id 0xf) the first time any member is actually
 * settled this call (a `once`-flag local keeps it from repeating per
 * member). */

void sub_800F4F4(struct phys_obj *self)
{
    if (self->timer != 0)
        return;

    if (++self->unk_50 >= self->unk_51) {
        struct phys_group *g = self->u48.group;

        if (PHYS_HAS_GROUP(g)) {
            if (g != NULL)
                sub_8026EB4(g);
            self->unk_59 = 0;
        }
        self->u48.group = PHYS_NO_GROUP;
        {
            u8 kind = 7;
            self->kind = kind;
        }
    } else {
        struct phys_group *g = self->u48.group;

        if (PHYS_HAS_GROUP(g)) {
            s32 i;
            s32 n = g->count;
            struct phys_obj **items = g->items;
            s32 played = FALSE;

            for (i = 0; i < n; i++) {
                struct phys_obj *o = items[i];

                if (o->kind == 5 && self->unk_50 >= o->unk_51) {
                    sub_800F5B8(o);
                    if (!played) {
                        PlaySfx(gUnknown_030012BC, 0xf, 0x100);
                        played = TRUE;
                    }
                }
            }
        }
        self->timer = self->unk_4C;
    }
}

/* Called by `sub_800F4F4` above (settling every member of a triggered
 * group) and directly from the still-raw `0x080104E4` continuation for
 * `self+0x4e` in `0x13`-`0x15` (outside this issue's scope). Decrements
 * `self+0x48` by `0x15` into `self+0x4e` (reusing the incoming state id
 * as a byte offset into itself - a compact re-dispatch trick this
 * function uses instead of a separate id field), then runs a
 * 0x13-entry jump table (`self+0x4e` post-subtraction, `bhi` gated at
 * `0x12`) that maps each of 15 distinct sub-cases to one of a small set
 * of `self+0x2d` tag constants (`0x1f, 0x1a, 0x17, 0x18, 4, 0x20, 2, 5,
 * 0x19 (with a nested self+0x48 = -0x2a store), 6, 0x11, 0xe, 0xf, 0x10
 * (a distinct "no group" tag)`), each falling into the same shared
 * `sub_80087C0`/`sub_80087B4`/`sub_800872C` triplet, plus 4 cases
 * (`3, 5, 9, 11`, per the raw table's own indices) that skip straight
 * to the tail instead. Tail (`self+0x29` nibble update via
 * `sub_800815C(self)`) matches the same `(x & 0xf) | (old & ~0xf)` fold
 * `sub_800F2BC`/`sub_800F368` use, just via a different lookup helper
 * (`sub_800815C` instead of `sub_8006DF8` directly - presumably an
 * already-classified variant). */

void sub_800F5B8(struct phys_obj *self)
{
    self->kind = self->u48.n - 0x15;
    switch (self->kind) {
    case 0:
        PhysSetTag(self, 0x1f);
        break;
    case 1:
        PhysSetTag(self, 0x1a);
        break;
    case 2:
        PhysSetTag(self, 0x17);
        break;
    case 4:
        PhysSetTag(self, 0x18);
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
    case 10:
        PhysSetTag(self, 5);
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
    self->slot = sub_800815C(self);
}

/* `sub_800F6B8(s32 x, s32 y, s32 arg2, s32 arg3)` - the one function in
 * this group taking a raw position/box instead of a `self` pointer (see
 * `src/graphics/actor_part38.c`'s existing extern: called as
 * `sub_800F6B8(part->x >> 8, part->y >> 8, 0x40, 0x12)`, a fixed
 * 0x40x0x12 probe box around an actor-part's own position). Walks
 * `gUnknown_0300130C`'s whole list: for every object whose
 * `_call_via_r1` classification against the probe box is `3`, whose
 * Chebyshev distance is within `(arg2, arg3)` on X/Y respectively, and
 * whose `+0x4d & 0x7f == 0`, looks up
 * `gStaticData_0816BBC4[other+0x4e]`: if that row's
 * `gStaticData_0816BBAE` byte is set, dispatches `1` ->
 * `sub_800E6B0(other)`, else `sub_800E7A8(other, 0, 0, 0)`; if the row
 * itself is `0`, calls `sub_800EEF0(other, 0)` instead. The same
 * "settle nearby objects against a probe box" shape as
 * `sub_800F06C`/`sub_800F798`, just driven by an explicit box rather
 * than `self`'s own hitbox record. */

void sub_800F6B8(s32 x, s32 y, s32 dist, s32 height)
{
    s32 i = 0;

    if (i < gUnknown_0300130C->count) {
        u8 *commit = gStaticData_0816BBC4;

        do {
            struct phys_obj *o = gUnknown_0300130C->items[i];

            if (PHYS_CALL(o, m48) == 3) {
                s32 t1 = (o->x >> 8) - x;
                s32 dx = (t1 ^ (t1 >> 31)) - (t1 >> 31);
                s32 t2 = (o->y >> 8) - y;
                s32 dy = (t2 ^ (t2 >> 31)) - (t2 >> 31);

                if (dx + dy <= dist && dy < height && (o->state & 0x7f) == 0) {
                    if (*(u8 *)(o->kind + (u32)commit))
                        sub_800EEF0(o, 0);
                    else if (gStaticData_0816BBAE[o->kind]) {
                        if (o->kind == 1)
                            sub_800E6B0(o);
                        else
                            sub_800E7A8(o, 0, 0, 0);
                    }
                }
            }
            i++;
        } while (i < gUnknown_0300130C->count);
    }
}

/* Called from the still-raw `0x080104E4` continuation
 * (`self+0x4d & 0x7f == 1` case, outside this issue's scope) - the
 * per-edge jump table's shared entry point once `self`'s own commit is
 * already underway. Looks up `gStaticData_0816BBC4[self+0x4e]`: if
 * nonzero and `self+0x34 == 0` (no pending sub-effect), dispatches on
 * `self+0x30` (`3` -> `sub_800F06C(self, 0x14)`, `6` ->
 * `sub_800F06C(self, 0x28)` - see that function's own doc comment).
 *
 * If `self+0x38` is set (a "linked to neighbors" flag), re-links
 * `self`'s `sub_8010708`/`sub_801070C` neighbor-list pointers
 * (`sub_8010710`/`sub_8010714`, already-elsewhere-matched splice
 * helpers) to remove `self` from the list. Unless `self+0x4e == 1`,
 * marks `self` "visited this frame" in `gUnknown_030012B0`'s per-cell
 * bitmap (the same 32x32-grid convention `sub_800EEF0`/`sub_800F368`
 * use, here against `gEntityFlags`) and walks
 * `gUnknown_030012D8+0x94`'s "recently touched" ring buffer
 * (`sub_0800D18C`'s own 5-slot buffer, per game_loop47.c's doc comment)
 * clearing each slot's `+0x94` re-visit flag once it matches `self`.
 *
 * If `self+0x38` was clear instead, and `self+0x4e != 1`, sets
 * `gUnknown_030012B0 = 1` (a one-shot "re-scan next pass" flag -
 * `sub_800F1B8`'s own outer loop condition above) unconditionally. */

static inline struct phys_obj *PhysRingAt(struct phys_player *p, s32 i)
{
    if (p->ringLocked == 0 && (i <= 4 || i < p->ringCount))
        return p->ring[i];
    return NULL;
}

void sub_800F798(struct phys_obj *self)
{
    if (gStaticData_0816BBC4[self->kind] && self->unk_34 == 0) {
        if (self->frame == 3)
            sub_800F06C(self, 0x14);
        else if (self->frame == 6)
            sub_800F06C(self, 0x28);
    }

    if (self->unk_38) {
        struct phys_obj *prev = sub_8010708(self);
        struct phys_obj *next = sub_801070C(self);
        s32 i;

        if (prev != NULL && next != NULL) {
            sub_8010710(next, prev);
            sub_8010714(prev, next);
        } else if (next != NULL) {
            sub_8010710(next, NULL);
        } else if (prev != NULL) {
            sub_8010714(prev, NULL);
        }

        if (self->kind == 1)
            return;
        gUnknown_030012B0 = 1;
        PHYS_GONE(self) = 1;
        if (self->id != 0xffff)
            PHYS_SET_ID_BIT(self->id);
        i = 0;
        if (i < PHYS_PLAYER->ringCount) {
            struct phys_player **pp = (struct phys_player **)&gUnknown_030012D8;

            do {
                if (PhysRingAt(*pp, i) == self)
                    (*pp)->ringCount = 0;
                i++;
            } while (i < (*pp)->ringCount);
        }
    } else if (self->kind != 1) {
        gUnknown_030012B0 = 1;
    }
}

/* Called from `sub_800EEF0` indirectly (both converge on
 * `self+0x4e` settling to `0x13`) and reachable from the per-edge
 * dispatch whenever a settled object's state lands in `0x13`-`0x15`.
 * Early-outs when `self+0x4f` (the per-object throttle byte
 * `sub_800F368` seeds from `self+0x4c`) is already nonzero. Otherwise
 * dispatches on `self+0x4e`:
 *
 * - `0x14`: tags `self+0x2d = 0x12`, runs the
 *   `sub_80087C0`/`sub_80087B4`/`sub_800872C` triplet, plays a sound
 *   (`gUnknown_030012BC`, id 0x11), then falls into the shared tail
 *   with `self+0x4e = 0x13`, `self+0x4f = 0x3c` (a ~1-second cooldown
 *   at 60 fps).
 * - `> 0x14` (only `0x15` reaches here, `bgt` from the `0x14` compare):
 *   same triplet + sound + tail, but tags `0x13` first and re-enters
 *   with `self+0x4e = 0x14` instead - a one-step state regression
 *   rather than the terminal settle the `0x14` case takes.
 * - `0x13`: if `self+0x4d & 0x7f == 0`, calls `sub_800EEF0(self, 0)` -
 *   the same "commit the edge collision" call the per-edge dispatch
 *   itself makes, closing the loop back into `sub_800EEF0` above.
 * - anything else: no-op. */

void sub_800F8E0(struct phys_obj *self)
{
    u8 kind;

    if (self->timer != 0)
        return;

    kind = self->kind;
    switch (kind) {
    case 0x15:
        PhysSetTag(self, 0x13);
        PlaySfx(gUnknown_030012BC, 0x11, 0x100);
        self->kind = 0x14;
        self->timer = 0x3c;
        break;
    case 0x14:
        PhysSetTag(self, 0x12);
        PlaySfx(gUnknown_030012BC, 0x11, 0x100);
        self->kind = 0x13;
        self->timer = 0x3c;
        break;
    case 0x13:
        if ((self->state & 0x7f) == 0)
            sub_800EEF0(self, 0);
        break;
    }
}

/* The largest and last function in this cluster
 * (0x0800F990-0x0800FC70, ~736 B). Called from the still-raw
 * `0x080104E4` continuation (`self+0x4e == 0xf`, outside this issue's
 * scope) - a **per-frame position-wrap/edge-scan advance**, structurally
 * similar to the already-parked `sub_800FC70`
 * (docs/matching/issue-13-fc70-continuation.md) that immediately
 * follows this whole cluster.
 *
 * First, unless `self+0x48` already has its `0xc0` high bits set,
 * clamps `self`'s position to within 0x4f/0x3f px of
 * `gUnknown_030012D8` (the player) on X/Y respectively, folding the
 * result into `self+0x48`'s packed byte (`(x & 0x3f) | 0x40`, masked
 * against `0xc7`, then `| 0x10`) - a "snap into range" step. Early-outs
 * entirely (jumps to the tail) once `self+0x4f` is nonzero.
 *
 * The bulk of the function is a small state cycle keyed by
 * `self+0x48 & 7`, advanced via `(x+1) & 3` each call and masked back
 * into `self+0x48`'s low 3 bits - a 4-phase rotation (only entered when
 * `self+0x2d == 8`, a specific tag this cluster's other functions write
 * via the `sub_80087C0` triplet) that dispatches each phase (`0`, `1`,
 * `2`, `3`, sub-split further by the *previous* phase value in a nested
 * compare) into per-phase blocks. These re-tag `self+0x2d`, re-run the
 * `sub_80087C0`/`sub_80087B4`/`sub_800872C` triplet, and (per the
 * `0xc0`-bit branch taken near the top) call `sub_8010A50(self)` -
 * already matched elsewhere (`game_loop30.c` family) - to decide
 * whether the phase cycle continues or the object's position gets
 * finally committed. Given the size and self-contained nature of this
 * state cycle (no calls out to any other function in this cluster), a
 * full branch-by-branch semantic write-up was not attempted for this
 * pass. */

/* Matched under old_agbcc (third near-miss sweep). The phase test
 * (`w1`), the loop (`lw`) and the `0x38` switch (`w2`) each have their
 * own local, and the empty asm below gives `lw` an extra reference so it
 * wins r1 over `nx`. The count update is written as separate in-place
 * steps on a fresh local (`t = (r - 1) << 24; cw &= 0xc7; t >>= 21;
 * cw |= t`), which ties the `& 0xc7` to the reloaded word's register and
 * the shift to `t`'s, as the ROM does; a single `(w & 0xc7) | (t << 3)`
 * expression left 7 halfwords off. */
void sub_800F990(struct phys_obj *self)
{
    s32 w;
    s32 ph0;
    s32 w1;

    w = self->u48.n;
    if (!(w & 0xc0))
    {
        struct gobj *pl = gUnknown_030012D8;
        s32 d;

        d = pl->x >> 8;
        d -= self->x >> 8;
        if (d < 0)
            d = -d;
        if (d <= 0x4f)
        {
            d = pl->y >> 8;
            d -= self->y >> 8;
            if (d < 0)
                d = -d;
            if (d <= 0x3f)
            {
                w &= 0x3f;
                w |= 0x40;
                w &= 0xc7;
                w |= 0x10;
                self->u48.n = w;
            }
        }
    }
    if (self->timer != 0)
        return;
    ph0 = self->u48.n & 7;
    ph0 &= 4;
    w1 = self->u48.n;
    if (ph0 && self->tag == 8)
    {
        s32 done = 0;

        do
        {
            s32 ph;
            s32 nx;
            s32 lw;

            lw = self->u48.n;
            nx = ((lw & 7) + 1) & 3;
            ph = nx;
            lw = (lw & 0xf8) | nx;
            asm("" : : "r"(lw)); /* extra reference: lw wins r1 over nx */
            self->u48.n = lw;
            switch (ph)
            {
            case 0:
                PhysSetTag(self, 7);
                if (self->u48.n & 0xc0)
                {
                    u8 r = sub_8010A50(self);

                    if (r != 0)
                    {
                        u32 t = (r - 1) << 24;
                        s32 cw = self->u48.n;

                        cw &= 0xc7;
                        t >>= 21;
                        cw |= t;
                        self->u48.n = cw;
                    }
                    w = self->u48.n;
                    if (!(w & 0x38))
                    {
                        s32 w2 = (w & 0xc7) | 0x10;

                        self->u48.n = w2;
                        switch ((s32)((u32)(w2 & 0xc0) >> 6))
                        {
                        case 1:
                            self->u48.n = (w2 & 0x3f) | 0x80;
                            break;
                        case 2:
                            self->u48.n = (w2 & 0x3f) | 0xc0;
                            break;
                        case 3:
                            PhysSetTag(self, 0x20);
                            self->kind = 7;
                            break;
                        }
                    }
                }
                goto out;
            case 1:
                if (self->unk_50 & 2)
                {
                    PhysSetTag(self, 9);
                    goto out;
                }
                break;
            case 2:
                if (self->unk_50 & 1)
                {
                    PhysSetTag(self, 0xb);
                    goto out;
                }
                break;
            case 3:
                if (self->unk_50 & 4)
                {
                    PhysSetTag(self, 0xd);
                    done = 1;
                }
                break;
            }
        } while (!done);
    out:
        {
            struct anim_rec *recs = self->anim->records;
            struct anim_rec *rec = &recs[self->tag];

            self->slot = sub_8006DF8(gUnknown_030012B8, rec->unk_14);
        }
        {
            s32 d = (s32)((u32)(self->u48.n & 0xc0) >> 6);

            self->timer = gStaticData_0816BB94[d];
        }
    }
    else
    {
        {
            s32 p = (w1 & 7) | 4;

            w1 = p | (w1 & 0xf8);
        }
        self->u48.n = w1;
        self->timer = 1;
        if (self->tag == 0xc)
            PhysSetTag(self, 0xa);
        else if (self->tag == 0xa)
            PhysSetTag(self, 8);
        else
        {
            switch ((s32)((u32)(self->u48.n & 0xc0) >> 6))
            {
            case 0:
            case 1:
                PhysSetTag(self, 0xc);
                break;
            case 2:
                PhysSetTag(self, 0xa);
                break;
            case 3:
                PhysSetTag(self, 8);
                break;
            }
        }
        {
            struct anim_rec *recs = self->anim->records;
            struct anim_rec *rec = &recs[self->tag];

            self->slot = sub_8006DF8(gUnknown_030012B8, rec->unk_14);
        }
        if (self->u48.n & 0xc0)
            PlaySfx(gUnknown_030012BC, 0x10, 0x100);
    }
}
