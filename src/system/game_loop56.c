#include "core.h"

/* GitHub issue #37 follow-up to `docs/matching/issue-37-game-loop-2375c.md`
 * (which matched this function's only caller, `sub_802375C`, in
 * `game_loop39.c`, but left this one "not yet confidently understood
 * branch-by-branch"). `self` (r7) is the level object `sub_802375C`
 * itself received; `gUnknown_030012C0` is the separate "level" object
 * most of its own callees take. `gStaticData_0816C86C` is the confirmed
 * 36-slot, 0x24-byte-stride per-level master table (see
 * `settings_menu19.c`/`oam_count.c`/`game_loop17.c`'s own struct views
 * of it) - here indexed by `self+0`, reading its `+0x1c` "already
 * initialized" guard byte (calls `sub_8023484` once if clear), then
 * `+0x14`/`+0x18` (fed straight through to `sub_8023118`/`sub_8023110`)
 * and finally `+4`, the **state field the 6-case jump table below
 * dispatches on**.
 *
 * **The 6-case dispatch** (`state-1` clamped to `[0,5]`, anything else -
 * `state==0` or `state>6` - taking the `default` path below):
 *
 * - **state 1 or 6** (cases 0 and 5 share one code block): resets the
 *   `gUnknown_030012C8` "fx queue" (`sub_8027088`, the `hud_fx_queue`
 *   struct `hud_icon_slot.c` documents) then fires it **twice** via
 *   `sub_8027018(queue, (u16 *)0x05000000, gStaticData_0816C81E, 0x10,
 *   9, 0)` and `sub_8027018(queue, (u16 *)0x05000000, gStaticData_
 *   0816C830, 0x14, 9, 0)` - `(u16 *)0x05000000` is GBA palette RAM
 *   itself passed as the queue's `targets` argument, so this is a
 *   **palette color-cycle animation**, not the HUD-digit rotation
 *   `sub_8026F54`/`sub_8027018`'s other call sites drive. The second
 *   call's setup (`angle=0x14`, `direction=0`) is byte-identical to
 *   state 5's own call below and the two share its tail (`_08023BA6`)
 *   in the ROM itself - not a coincidence this transcription
 *   reproduces, see the NAKED note below.
 * - **state 2**: single call, `sub_8027018(queue, (u16 *)0x05000000,
 *   gStaticData_0816C814, 6, 5, 1)` (`direction=1`, the only case that
 *   sets it).
 * - **state 3**: single call, `sub_8027018(queue, (u16 *)0x05000000,
 *   gStaticData_0816C842, 0xa, 0x10, 0)`.
 * - **state 5**: single call, `sub_8027018(queue, (u16 *)0x05000000,
 *   gStaticData_0816C862, 0x14, 5, 0)` - shares its `angle=0x14; bl
 *   sub_8027018` tail instruction-for-instruction with state 1/6's
 *   second call, per the note above.
 * - **default** (state 0, state 4, or anything `>6`): skips the reset
 *   and the `sub_8027018` call entirely, just clears the queue's own
 *   `active` byte directly.
 *
 * All five non-default cases fall into one shared tail. **This closes
 * the "`gStaticData_0816C81E`/`0816C830`/`0816C842`/`0816C862`" open
 * question from the issue doc**: these (plus `gStaticData_0816C814`,
 * a fifth table the same neighborhood the jump-table trace hadn't
 * previously reached) are not per-level records with their own shape -
 * they're plain, tightly-packed `u16[]` "permutation index list"
 * arguments straight to `sub_8027018`'s own `lists` parameter, each
 * one exactly `list_count * 2` bytes long and back-to-back with the
 * next table in ROM (`0816C814`: 5 entries/0xA bytes -> `0816C81E`;
 * `0816C81E`: 9/0x12 -> `0816C830`; `0816C830`: 9/0x12 -> `0816C842`;
 * `0816C842`: 0x10/0x20 -> `0816C862`; `0816C862`: 5 entries), matching
 * every call site's own `list_count` argument exactly. No further
 * struct needed.
 *
 * **Shared tail**: calls `sub_80240E4(self)`/`sub_802423C()` (the
 * latter already matched in `game_loop9.c`), then re-reads the
 * level-state record's (`self->0x18`) own `+8` "widget kind" field
 * (the same field `sub_802375C` dispatched its own widget-construction
 * switch on) - if it's `1`, re-stamps the player's `+0x2d` byte to
 * `0x1f`, refreshes its OAM entry (`sub_80087C0`/`sub_80087B4`/
 * `sub_800872C`), and sets the 0x18-byte scratch block's `+0x14` to
 * `2`. Either way, recomputes the player's `+0x29` low nibble from
 * `sub_800815C(player)` (the "negative-constant bit-clear idiom",
 * `docs/matching.md`) and fires `sub_8006D08` against the tile-asset
 * cache using a `player+0x20`-table lookup indexed by `player+0x2d*7`
 * (0x1c-byte stride), flushes the scratch block (`sub_8026DFC`) and
 * text-box singleton (`sub_8026984`).
 *
 * If the widget kind is `0`: probes `sub_80232B8`/`sub_8024404` or
 * `sub_8023290`/`sub_80243E0` (level-object and self-based readiness
 * checks); on success, clears the player's busy bit 7 (`+0xc &=
 * 0x7f`), re-stamps `+0x2d` to `0x29`, refreshes the OAM entry again,
 * plays a sound effect (`gUnknown_030012BC` as the sample id, priority
 * `0x2c`) via `PlaySfx`, fires the `player+0x44`-table's trampoline
 * (`sub_803AD80`, mode `0x29`), repeats the same `sub_8006D08` tile-
 * cache call, and pings `gUnknown_03001318` (`sub_8028504`).
 *
 * Either way, this converges on flushing the four HUD ring-buffer
 * managers (`sub_8008C80` on `030012F4`/`EC`/`F0`/`F8`), a
 * `sub_802400C(self)` VRAM/OAM refresh, and the fade-cluster
 * `sub_8001524(0)`/`sub_80015E0`/`sub_8001614`/`sub_8001624` reset
 * quartet, landing at the **wait loop** (`_08023E5A`/`_08023D7C`,
 * `docs/rom_map.md`'s "Traced the fade-to-black's trigger" section):
 * poll `sub_80241B0` (the `gUnknown_03000830` readiness flag,
 * `game_loop9.c`) each iteration; while not ready and the player's
 * `+0xc` bit 0 is clear, run one more "outstanding work" pass
 * (`sub_802423C`/`sub_802400C`, a `sub_8004D74` input-driven mini-
 * dispatch that can early-exit this whole function with return value
 * `1` or `2` via `sub_80241BC`'s level-end teardown, a `gUnknown_
 * 030007E0` input-flag-gated `sub_8028504` ping, `sub_800891C` on
 * three ring-buffer managers, `sub_803AD7C` trampoline probes against
 * the player's own `+0x18`/`+0x38`-`/+0x18` tables, `sub_80091D4` on
 * `gUnknown_0300130C`, `sub_8028400`, and a `gUnknown_030012C0+0x8c`-
 * gated `sub_8022F2C` call) before looping back. Once ready, fires the
 * fade (`sub_80014A4`) - the concrete trigger `rom_map.md` originally
 * traced this function down to find.
 *
 * **Post-fade** (`_08023E82` onward, converging at `_08023F92`): sets
 * the return value to `0`, then tries two `sub_802356C` "spawn"
 * dispatches gated by `sub_8024404`/`sub_80232B8`/`sub_8023104` or
 * `sub_80243E0`/`sub_8023290` (both skip straight to the flush tail on
 * failure); falling through both, loops `gUnknown_0300130C` counting
 * entries whose `sub_803AD7C` trampoline probe returns `3` *and* whose
 * own `+0x4e` tag is `0xa` (the same physics-subsystem state tag
 * `gStaticData_0816BC98` indexes, `docs/matching/
 * issue-12-physics-collision.md`), then calls `sub_8023140(gUnknown_
 * 030012C0, count)`. **Final tail** (`_08023F92`, also every early-out
 * above): flushes all five hot IWRAM widget-manager globals
 * (`sub_8008CEC` on `030012E8`/`EC`/`F0`/`F8`/`F4`, `sub_8009914` on
 * `0300130C`), resets the fade cluster's own bitfield accessors
 * (`sub_8001578`/`sub_8001564`/`sub_8001550`/`sub_800153C`/
 * `sub_800158C`/`sub_80006A8`/`sub_8001614`), and returns whatever
 * `sl` was left holding (`1` by default, `2` from the wait-loop's
 * `sub_8004D74`-driven early exit, or `0` once the post-fade branch
 * was reached) - the value `sub_802375C` itself stashes and returns.
 *
 * Was a NAKED transcription (from `asm/code_3_2_17_23a1c.s`); matches
 * as plain C under old_agbcc since the hard-register hold pass
 * (docs/matching/hard-register-hold-retry.md), so this object is on the
 * Makefile's OLD_AGBCC_OBJS list (it is the file's only function;
 * agbcc is 42 halfwords off). The shared `_08023BA6` tail is ordinary
 * cross-jumping. The one-byte `direction` stack argument is a BLKmode
 * struct (see `struct fx_direction`), and the post-fade player-position
 * copy holds r0/r1 while the player pointer is loaded. */
struct gl_point
{
    s32 x;
    s32 y;
};

struct gl_method
{
    s16 delta;
    u8 unk_02[2];
    void *fn;
};

struct gl_vtable
{
    u8 unk_00[0x18];
    struct gl_method m18;           /* +0x18 */
    u8 unk_20[0x18];
    struct gl_method m38;           /* +0x38 */
    u8 unk_40[8];
    struct gl_method m48;           /* +0x48 */
};

struct gl_attach_vtable
{
    u8 unk_00[0x20];
    struct gl_method attach;        /* +0x20 */
};

struct gl_attach
{
    u8 unk_00[0xC];
    struct gl_attach_vtable *vtable; /* +0x0C */
};

struct gl_anim_record
{
    u8 unk_00[0x14];
    u8 tileRecord;                  /* +0x14 */
    u8 unk_15[7];
};

struct gl_player
{
    struct gl_point pos;            /* +0x00 */
    u8 unk_08[4];
    u8 flags;                       /* +0x0C */
    u8 unk_0D[0xB];
    struct gl_vtable *vtable;       /* +0x18 */
    u8 unk_1C[4];
    struct gl_anim_record **anim;   /* +0x20 */
    u8 unk_24[5];
    u8 frameNibble:4;               /* +0x29 */
    u8 unk_29_4:4;
    u8 unk_2A[3];
    u8 animIndex;                   /* +0x2D */
    u8 unk_2E[0x16];
    struct gl_attach *attach;       /* +0x44 */
    u8 unk_48[0x3C];
    u8 unk_84[0x80];
    u8 inputLock;                   /* +0x104 */
};

struct gl_entity
{
    u8 unk_00[0x18];
    struct gl_vtable *vtable;       /* +0x18 */
    u8 unk_1C[0x32];
    u8 tag;                         /* +0x4E */
};

struct gl_entity_list
{
    s32 count;
    u8 unk_04[4];
    struct gl_entity **items;       /* +0x08 */
};

struct gl_level_entry
{
    s32 unk_00;
    s32 state;                      /* +0x04 */
    u8 unk_08[0xC];
    s32 unk_14;
    s32 unk_18;
    u8 initialized;                 /* +0x1C */
    u8 unk_1D[7];
};

struct gl_widget_kind
{
    u8 unk_00[8];
    s32 kind;                       /* +0x08 */
};

struct gl_self
{
    s32 level;                      /* +0x00 */
    u8 unk_04[0x14];
    struct gl_widget_kind *widget;  /* +0x18 */
};

struct gl_scratch
{
    u8 unk_00[0x10];
    void *player;                   /* +0x10 */
    s32 unk_14;                     /* +0x14 */
};

struct gl_level
{
    u8 unk_00[0x8C];
    u8 busy;                        /* +0x8C */
};

union gl_input
{
    u32 held;
    struct
    {
        u16 lo;
        u16 hi;
    } half;
};

extern struct gl_player *gUnknown_030012D8;
extern struct gl_scratch *gUnknown_030012D4;
extern void *gUnknown_03001308;
extern struct gl_level *gUnknown_030012C0;
extern u8 *gUnknown_030012C8;
extern void *gUnknown_030012B8;
extern void *gUnknown_030012BC;
extern void *gUnknown_03001318;
extern void *gUnknown_030012F4;
extern void *gUnknown_030012EC;
extern void *gUnknown_030012F0;
extern void *gUnknown_030012F8;
extern void *gUnknown_030012E8;
extern void *gUnknown_03001304;
extern struct gl_entity_list *gUnknown_0300130C;
extern s32 gUnknown_0300082C;
extern union gl_input gUnknown_030007E0;
extern struct gl_level_entry gStaticData_0816C86C[];
extern u16 gStaticData_0816C814[];
extern u16 gStaticData_0816C81E[];
extern u16 gStaticData_0816C830[];
extern u16 gStaticData_0816C842[];
extern u16 gStaticData_0816C862[];

extern void sub_800A810(void *player);
extern void sub_80266BC(void *box, void *widget);
extern void sub_8023484(void *level);
extern u8 sub_80232C8(void *level);
extern void sub_800F1B8(void);
extern void sub_8023118(void *level, s32 value);
extern void sub_8023110(void *level, s32 value);
extern void sub_8027088(void *queue);
/* The direction flag travels as a one-byte struct by value - the ROM
 * stores it into its stack slot with `strb`. The zero-length `pad`
 * makes the struct BLKmode, so the compound literal is stored straight
 * into the outgoing slot: the slot address (`add rN, sp, #4`) comes
 * before the constant, as in the ROM. As a QImode struct the value was
 * built in a register first. */
struct fx_direction
{
    u8 value;
    u8 pad[0];
} __attribute__((packed));

extern void sub_8027018(void *queue, u16 *targets, u16 *lists, s32 angle, s32 count, struct fx_direction direction);

#define FX_CYCLE(lists, angle, count, dir) \
    sub_8027018(gUnknown_030012C8, PAL_RAM, (lists), (angle), (count), \
                (struct fx_direction){ (dir) })
extern void sub_80240E4(struct gl_self *self);
extern void sub_802423C(void);
extern void sub_80087C0(void *part);
extern void sub_80087B4(void *part);
extern void sub_800872C(void *part, s32 arg);
extern s32 sub_800815C(void *part);
extern void sub_8006D08(void *cache, s32 slot, s32 recordId);
extern void sub_8026DFC(void *scratch);
extern void sub_8026984(void *box);
extern u8 sub_80232B8(void *level);
extern u8 sub_8023290(void *level);
extern u8 sub_8024404(struct gl_self *self);
extern u8 sub_80243E0(struct gl_self *self);
extern void PlaySfx(void *ctx, s32 sfxId, s32 volume);
extern s32 sub_803AD80(void *self, s32 arg, void *fn);
extern s32 sub_803AD7C(void *self, void *fn);
extern void sub_8028504(void *arg);
extern void sub_8008C80(void *mgr);
extern void sub_802400C(struct gl_self *self);
extern void sub_8001524(s32 arg);
extern void sub_8001604(void);
extern void sub_80015E0(void);
extern void sub_8001614(void);
extern void sub_8001624(void);
extern void sub_80007AC(void *arg);
extern s32 sub_8004D74(void);
extern void sub_80241BC(struct gl_self *self);
extern void sub_800891C(void *mgr);
extern void sub_80091D4(void *list);
extern void sub_8028400(void *arg);
extern void sub_8022F2C(struct gl_level *level);
extern u8 sub_80241B0(void);
extern void sub_80014A4(void);
extern s32 *sub_8023104(void *level);
extern s32 sub_801B29C(s32 *arg);
extern void sub_802356C(void *level, s32 arg, s32 *point);
extern void sub_8023140(void *level, s32 count);
extern void sub_8008CEC(void *mgr);
extern void sub_8009914(void *list);
extern void sub_8001578(void);
extern void sub_8001564(void);
extern void sub_8001550(void);
extern void sub_800153C(void);
extern void sub_800158C(void);
extern void sub_80006A8(void);

#define PAL_RAM ((u16 *)PLTT)

/* obj->vtable->slot(obj), through `_call_via_r1` (sub_803AD7C). */
#define PMF_CALL(obj, slot)                                                    \
    ({                                                                         \
        struct gl_method *_m = &(obj)->vtable->slot;                           \
        sub_803AD7C((u8 *)(obj) + _m->delta, _m->fn);                          \
    })

static inline void SetPoint(struct gl_point *point, s32 x, s32 y)
{
    point->x = x;
    point->y = y;
}

static inline void SpawnNearPlayer(s32 x, s32 y)
{
    struct gl_point point;
    struct gl_level *level;

    point.x = x;
    point.y = y;
    level = gUnknown_030012C0;
    sub_802356C(level, sub_801B29C(sub_8023104(level)), &point.x);
}

static inline void RestartPlayerAnim(struct gl_player *p, s32 anim)
{
    p->animIndex = anim;
    sub_80087C0(p);
    sub_80087B4(p);
    sub_800872C(p, 0);
}

static inline void RefreshPlayerTiles(void)
{
    void *cache = gUnknown_030012B8;
    struct gl_player *p = gUnknown_030012D8;

    sub_8006D08(cache, p->frameNibble, (*p->anim)[p->animIndex].tileRecord);
}

s32 sub_8023A1C(struct gl_self *self)
{
    s32 ret = 1;
    s32 i;

    sub_800A810(gUnknown_030012D8);
    gUnknown_030012D4->player = gUnknown_030012D8;
    gUnknown_030012D4->unk_14 = ret;
    sub_80266BC(gUnknown_03001308, self->widget);
    if (!gStaticData_0816C86C[self->level].initialized)
        sub_8023484(gUnknown_030012C0);
    if (sub_80232C8(gUnknown_030012C0))
        sub_800F1B8();
    sub_8023118(gUnknown_030012C0, gStaticData_0816C86C[self->level].unk_14);
    sub_8023110(gUnknown_030012C0, gStaticData_0816C86C[self->level].unk_18);

    switch (gStaticData_0816C86C[self->level].state)
    {
    case 2:
        sub_8027088(gUnknown_030012C8);
        FX_CYCLE(gStaticData_0816C814, 6, 5, 1);
        break;
    case 1:
    case 6:
        sub_8027088(gUnknown_030012C8);
        FX_CYCLE(gStaticData_0816C81E, 0x10, 9, 0);
        FX_CYCLE(gStaticData_0816C830, 0x14, 9, 0);
        break;
    case 3:
        sub_8027088(gUnknown_030012C8);
        FX_CYCLE(gStaticData_0816C842, 0xA, 0x10, 0);
        break;
    case 5:
        sub_8027088(gUnknown_030012C8);
        FX_CYCLE(gStaticData_0816C862, 0x14, 5, 0);
        break;
    default:
        *gUnknown_030012C8 = 0;
        break;
    }

    sub_80240E4(self);
    sub_802423C();
    if (self->widget->kind == 1)
    {
        RestartPlayerAnim(gUnknown_030012D8, 0x1F);
        gUnknown_030012D4->unk_14 = 2;
    }
    gUnknown_030012D8->frameNibble = sub_800815C(gUnknown_030012D8);
    RefreshPlayerTiles();
    sub_8026DFC(gUnknown_030012D4);
    sub_8026984(gUnknown_03001308);

    if (self->widget->kind == 0)
    {
        if ((sub_80232B8(gUnknown_030012C0) && sub_8024404(self))
            || (sub_8023290(gUnknown_030012C0) && sub_80243E0(self)))
        {
            struct gl_attach *a;

            gUnknown_030012D8->flags &= 0x7F;
            RestartPlayerAnim(gUnknown_030012D8, 0x29);
            PlaySfx(gUnknown_030012BC, 0x2C, 0x100);
            a = gUnknown_030012D8->attach;
            sub_803AD80((u8 *)a + a->vtable->attach.delta, 0x29, a->vtable->attach.fn);
            RefreshPlayerTiles();
            sub_8028504(gUnknown_03001318);
        }
    }
    sub_8008C80(gUnknown_030012F4);
    sub_8008C80(gUnknown_030012EC);
    sub_8008C80(gUnknown_030012F0);
    sub_8008C80(gUnknown_030012F8);
    sub_802400C(self);
    sub_8001524(0);
    sub_8001604();
    sub_80015E0();
    sub_8001614();
    sub_8001624();

    while (!sub_80241B0() && !(gUnknown_030012D8->flags & 1))
    {
        struct gl_player *p;

        sub_802423C();
        sub_802400C(self);
        sub_80007AC(gUnknown_03001304);
        if (!gUnknown_030012D8->inputLock && (gUnknown_030007E0.half.hi & 8))
        {
            s32 r = sub_8004D74();

            if (r == 0)
            {
                sub_80241BC(self);
                sub_80007AC(gUnknown_03001304);
            }
            if (r == 1)
            {
                ret = 1;
                goto fade;
            }
            if (r == 2)
            {
                ret = 2;
                goto fade;
            }
        }
        if (gUnknown_030007E0.held & 4)
            sub_8028504(gUnknown_03001318);
        sub_800891C(gUnknown_030012F4);
        sub_800891C(gUnknown_030012E8);
        if ((u8)PMF_CALL(gUnknown_030012D8, m38))
            PMF_CALL(gUnknown_030012D8, m18);
        sub_80091D4(gUnknown_0300130C);
        sub_800891C(gUnknown_030012EC);
        sub_800891C(gUnknown_030012F0);
        sub_800891C(gUnknown_030012F8);
        sub_8028400(gUnknown_03001318);
        if (gUnknown_030012C0->busy)
            sub_8022F2C(gUnknown_030012C0);
        gUnknown_0300082C++;
    }
fade:
    sub_80014A4();
    if (sub_80241B0())
    {
        ret = 0;
        if (!sub_8024404(self) && sub_80232B8(gUnknown_030012C0))
        {
            struct gl_point point;
            s32 x;

            x = *sub_8023104(gUnknown_030012C0) + -0x1E00;
            i = gUnknown_030012D8->pos.y + 0x1200;
            point.x = x;
            point.y = i;
            x = (s32)gUnknown_030012C0;
            sub_802356C((void *)x, sub_801B29C(sub_8023104((void *)x)), &point.x);
        }
        else if (!sub_80243E0(self) && sub_8023290(gUnknown_030012C0))
        {
            struct gl_point point;
            struct gl_player *pl;
            register s32 hold asm("r0");
            register s32 hold1 asm("r1");

            /* Hard-register hold (no code): with r0 and r1 live, the
             * global's address and the player pointer both land in r2,
             * as in the ROM. */
            asm("" : "=r"(hold));
            asm("" : "=r"(hold1));
            pl = gUnknown_030012D8;
            /* End of the hold. */
            asm("" : : "r"(hold));
            asm("" : : "r"(hold1));
            point = pl->pos;
            sub_802356C(gUnknown_030012C0, 0, &point.x);
        }
        else
        {
            s32 count = 0;
            struct gl_entity_list **list;

            i = 0;
            if (count < gUnknown_0300130C->count)
            {
                list = &gUnknown_0300130C;
                do
                {
                    struct gl_entity *e = (*list)->items[i];

                    if (PMF_CALL(e, m48) == 3 && e->tag == 0xA)
                        count++;
                    i++;
                } while (i < (*list)->count);
            }
            sub_8023140(gUnknown_030012C0, count);
        }
    }
    sub_8008CEC(gUnknown_030012E8);
    sub_8009914(gUnknown_0300130C);
    sub_8008CEC(gUnknown_030012EC);
    sub_8008CEC(gUnknown_030012F0);
    sub_8008CEC(gUnknown_030012F8);
    sub_8008CEC(gUnknown_030012F4);
    sub_8001578();
    sub_8001564();
    sub_8001550();
    sub_800153C();
    sub_800158C();
    sub_80006A8();
    sub_8001614();
    return ret;
}
