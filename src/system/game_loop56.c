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
 * Written as a NAKED, instruction-for-instruction transcription rather
 * than real C, matching this session's established fallback for
 * `game_loop`-neighborhood functions this size with a raw jump table
 * (`sub_8017AB0` in `actor_part27a.c`, `UpdateGameFrame` in
 * `game_loop55.c`): beyond the sheer instruction count (~650), the
 * state 1/6 and state 5 cases converging on one shared physical tail
 * block (`_08023BA6`) mid-case-body - not at a case's start or end,
 * but after state 1/6's *second* `sub_8027018` call has already begun
 * loading that call's own arguments - is exactly the kind of
 * compiler-internal basic-block-sharing decision this project's own
 * `goto`-based restructuring technique targets *within* a single
 * `switch`, not *across* two different jump-table targets; forcing it
 * from plain C here would mean fighting the compiler's own switch
 * lowering rather than working with it. Transcribed from `asm/
 * code_3_2_17_23a1c.s` (now retired - its only function moves here),
 * keeping the ROM's own `_0XXXXXXX` hex-address labels verbatim as
 * plain, file-local asm symbols. Verified structurally byte-exact via
 * an isolated `cpp`/`agbcc`/`arm-none-eabi-as` + `objcopy` comparison
 * against the ROM's own raw bytes, and via a full clean `make compare`
 * once wired into the real build.
 *
 * Later pass (#37 retry): the `#if NON_MATCHING` draft below is 15
 * halfwords off under old_agbcc (same size; the shared `_08023BA6` tail
 * is ordinary cross-jumping and comes out on its own). What is left: the
 * one-byte `direction` stack argument - passing it as a packed one-byte
 * struct gets the ROM's `strb` into the outgoing slot, but the ROM
 * computes the slot address (`add rN, sp, #4`) before materializing
 * the constant, which every C spelling tried (compound literal, local,
 * union cast, inline wrapper) reverses - plus one register choice in the
 * post-fade player-position copy.
 * Mix-6 pass: also no effect or worse - `"=r"/"0"` and `"+r"` escapes
 * on the value, `u8`/`u8[1]`/`u8:8` field, a `u8` prototype (loses the
 * ROM's sl register, 447 hw), a struct-by-value inline wrapper, and
 * dropping the `r2` pin (17 hw). The file builds with agbcc (57 hw
 * there), so a close would also need a split to old_agbcc. */
#if NON_MATCHING
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
 * stores it into its stack slot with `strb`. */
struct fx_direction
{
    u8 value;
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

#define PAL_RAM ((u16 *)0x05000000)

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
            register struct gl_player *pl asm("r2") = gUnknown_030012D8;

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
#else
NAKED s32 sub_8023A1C(void *selfArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0x18\n\t"
        "add r7, r0, #0\n\t"
        "mov r0, #1\n\t"
        "mov sl, r0\n\t"
        "ldr r4, _08023AC4\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_800A810\n\t"
        "ldr r0, _08023AC8\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r0, [r4]\n\t"
        "str r0, [r1, #0x10]\n\t"
        "mov r2, sl\n\t"
        "str r2, [r1, #0x14]\n\t"
        "ldr r0, _08023ACC\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r7, #0x18]\n\t"
        "bl sub_80266BC\n\t"
        "ldr r5, _08023AD0\n\t"
        "ldr r1, [r7]\n\t"
        "lsl r0, r1, #3\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r5\n\t"
        "ldrb r0, [r0, #0x1c]\n\t"
        "cmp r0, #0\n\t"
        "bne _08023A66\n\t"
        "ldr r0, _08023AD4\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8023484\n\t"
    "_08023A66:\n"
        "ldr r4, _08023AD4\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_80232C8\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq _08023A78\n\t"
        "bl sub_800F1B8\n\t"
    "_08023A78:\n"
        "ldr r0, [r4]\n\t"
        "ldr r2, [r7]\n\t"
        "lsl r1, r2, #3\n\t"
        "add r1, r1, r2\n\t"
        "lsl r1, r1, #2\n\t"
        "add r2, r5, #0\n\t"
        "add r2, #0x14\n\t"
        "add r1, r1, r2\n\t"
        "ldr r1, [r1]\n\t"
        "bl sub_8023118\n\t"
        "ldr r0, [r4]\n\t"
        "ldr r2, [r7]\n\t"
        "lsl r1, r2, #3\n\t"
        "add r1, r1, r2\n\t"
        "lsl r1, r1, #2\n\t"
        "add r2, r5, #0\n\t"
        "add r2, #0x18\n\t"
        "add r1, r1, r2\n\t"
        "ldr r1, [r1]\n\t"
        "bl sub_8023110\n\t"
        "ldr r1, [r7]\n\t"
        "lsl r0, r1, #3\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "add r1, r5, #4\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "sub r0, #1\n\t"
        "cmp r0, #5\n\t"
        "bls _08023ABA\n\t"
        "b _08023BC4\n\t"
    "_08023ABA:\n"
        "lsl r0, r0, #2\n\t"
        "ldr r1, _08023AD8\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "mov pc, r0\n\t"
        ".align 2, 0\n"
    "_08023AC4: .4byte gUnknown_030012D8\n"
    "_08023AC8: .4byte gUnknown_030012D4\n"
    "_08023ACC: .4byte gUnknown_03001308\n"
    "_08023AD0: .4byte gStaticData_0816C86C\n"
    "_08023AD4: .4byte gUnknown_030012C0\n"
    "_08023AD8: .4byte _08023ADC\n"
    "_08023ADC:\n"
        ".4byte _08023B20\n\t"
        ".4byte _08023AF4\n\t"
        ".4byte _08023B60\n\t"
        ".4byte _08023BC4\n\t"
        ".4byte _08023B8C\n\t"
        ".4byte _08023B20\n\t"
    "_08023AF4:\n"
        "ldr r4, _08023B18\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8027088\n\t"
        "ldr r0, [r4]\n\t"
        "mov r1, #0xa0\n\t"
        "lsl r1, r1, #0x13\n\t"
        "ldr r2, _08023B1C\n\t"
        "mov r3, #5\n\t"
        "str r3, [sp]\n\t"
        "add r4, sp, #4\n\t"
        "mov r3, #1\n\t"
        "strb r3, [r4]\n\t"
        "mov r3, #6\n\t"
        "bl sub_8027018\n\t"
        "b _08023BCC\n\t"
        ".align 2, 0\n"
    "_08023B18: .4byte gUnknown_030012C8\n"
    "_08023B1C: .4byte gStaticData_0816C814\n"
    "_08023B20:\n"
        "ldr r4, _08023B54\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8027088\n\t"
        "ldr r0, [r4]\n\t"
        "mov r3, #0xa0\n\t"
        "lsl r3, r3, #0x13\n\t"
        "mov sb, r3\n\t"
        "ldr r2, _08023B58\n\t"
        "mov r5, #9\n\t"
        "mov r8, r5\n\t"
        "str r5, [sp]\n\t"
        "add r6, sp, #4\n\t"
        "mov r5, #0\n\t"
        "strb r5, [r6]\n\t"
        "mov r1, sb\n\t"
        "mov r3, #0x10\n\t"
        "bl sub_8027018\n\t"
        "ldr r0, [r4]\n\t"
        "ldr r2, _08023B5C\n\t"
        "mov r1, r8\n\t"
        "str r1, [sp]\n\t"
        "strb r5, [r6]\n\t"
        "mov r1, sb\n\t"
        "b _08023BA6\n\t"
        ".align 2, 0\n"
    "_08023B54: .4byte gUnknown_030012C8\n"
    "_08023B58: .4byte gStaticData_0816C81E\n"
    "_08023B5C: .4byte gStaticData_0816C830\n"
    "_08023B60:\n"
        "ldr r4, _08023B84\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8027088\n\t"
        "ldr r0, [r4]\n\t"
        "mov r1, #0xa0\n\t"
        "lsl r1, r1, #0x13\n\t"
        "ldr r2, _08023B88\n\t"
        "mov r3, #0x10\n\t"
        "str r3, [sp]\n\t"
        "add r4, sp, #4\n\t"
        "mov r3, #0\n\t"
        "strb r3, [r4]\n\t"
        "mov r3, #0xa\n\t"
        "bl sub_8027018\n\t"
        "b _08023BCC\n\t"
        ".align 2, 0\n"
    "_08023B84: .4byte gUnknown_030012C8\n"
    "_08023B88: .4byte gStaticData_0816C842\n"
    "_08023B8C:\n"
        "ldr r4, _08023BB0\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8027088\n\t"
        "ldr r0, [r4]\n\t"
        "mov r1, #0xa0\n\t"
        "lsl r1, r1, #0x13\n\t"
        "ldr r2, _08023BB4\n\t"
        "mov r3, #5\n\t"
        "str r3, [sp]\n\t"
        "add r4, sp, #4\n\t"
        "mov r3, #0\n\t"
        "strb r3, [r4]\n\t"
    "_08023BA6:\n"
        "mov r3, #0x14\n\t"
        "bl sub_8027018\n\t"
        "b _08023BCC\n\t"
        ".align 2, 0\n"
    "_08023BB0: .4byte gUnknown_030012C8\n"
    "_08023BB4: .4byte gStaticData_0816C862\n"
    "_08023BB8:\n"
        "mov r2, #1\n\t"
        "mov sl, r2\n\t"
        "b _08023E72\n\t"
    "_08023BBE:\n"
        "mov r3, #2\n\t"
        "mov sl, r3\n\t"
        "b _08023E72\n\t"
    "_08023BC4:\n"
        "ldr r0, _08023D4C\n\t"
        "ldr r1, [r0]\n\t"
        "mov r0, #0\n\t"
        "strb r0, [r1]\n\t"
    "_08023BCC:\n"
        "add r0, r7, #0\n\t"
        "bl sub_80240E4\n\t"
        "bl sub_802423C\n\t"
        "ldr r0, [r7, #0x18]\n\t"
        "ldr r0, [r0, #8]\n\t"
        "cmp r0, #1\n\t"
        "bne _08023C06\n\t"
        "ldr r0, _08023D50\n\t"
        "ldr r4, [r0]\n\t"
        "mov r0, #0x1f\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x2d\n\t"
        "strb r0, [r1]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087C0\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087B4\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800872C\n\t"
        "ldr r0, _08023D54\n\t"
        "ldr r1, [r0]\n\t"
        "mov r0, #2\n\t"
        "str r0, [r1, #0x14]\n\t"
    "_08023C06:\n"
        "ldr r4, _08023D50\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_800815C\n\t"
        "ldr r2, [r4]\n\t"
        "add r2, #0x29\n\t"
        "mov r1, #0xf\n\t"
        "and r0, r1\n\t"
        "mov r1, #0x10\n\t"
        "neg r1, r1\n\t"
        "ldrb r5, [r2]\n\t"
        "and r1, r5\n\t"
        "orr r1, r0\n\t"
        "strb r1, [r2]\n\t"
        "ldr r0, _08023D58\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r3, [r4]\n\t"
        "add r1, r3, #0\n\t"
        "add r1, #0x29\n\t"
        "ldrb r1, [r1]\n\t"
        "lsl r1, r1, #0x1c\n\t"
        "lsr r1, r1, #0x1c\n\t"
        "ldr r2, [r3, #0x20]\n\t"
        "add r3, #0x2d\n\t"
        "ldr r4, [r2]\n\t"
        "ldrb r5, [r3]\n\t"
        "lsl r2, r5, #3\n\t"
        "sub r2, r2, r5\n\t"
        "lsl r2, r2, #2\n\t"
        "add r2, r2, r4\n\t"
        "ldrb r2, [r2, #0x14]\n\t"
        "bl sub_8006D08\n\t"
        "ldr r0, _08023D54\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8026DFC\n\t"
        "ldr r0, _08023D5C\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8026984\n\t"
        "ldr r0, [r7, #0x18]\n\t"
        "ldr r0, [r0, #8]\n\t"
        "cmp r0, #0\n\t"
        "bne _08023D0C\n\t"
        "ldr r4, _08023D60\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_80232B8\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq _08023C7A\n\t"
        "add r0, r7, #0\n\t"
        "bl sub_8024404\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne _08023C92\n\t"
    "_08023C7A:\n"
        "ldr r0, [r4]\n\t"
        "bl sub_8023290\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq _08023D0C\n\t"
        "add r0, r7, #0\n\t"
        "bl sub_80243E0\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq _08023D0C\n\t"
    "_08023C92:\n"
        "ldr r5, _08023D50\n\t"
        "ldr r1, [r5]\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r2, [r1, #0xc]\n\t"
        "and r0, r2\n\t"
        "strb r0, [r1, #0xc]\n\t"
        "ldr r4, [r5]\n\t"
        "mov r0, #0x29\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x2d\n\t"
        "strb r0, [r1]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087C0\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087B4\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800872C\n\t"
        "ldr r0, _08023D64\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #0x2c\n\t"
        "bl PlaySfx\n\t"
        "ldr r0, [r5]\n\t"
        "ldr r0, [r0, #0x44]\n\t"
        "ldr r2, [r0, #0xc]\n\t"
        "mov r3, #0x20\n\t"
        "ldrsh r1, [r2, r3]\n\t"
        "add r0, r0, r1\n\t"
        "ldr r2, [r2, #0x24]\n\t"
        "mov r1, #0x29\n\t"
        "bl sub_803AD80\n\t"
        "ldr r0, _08023D58\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r3, [r5]\n\t"
        "add r1, r3, #0\n\t"
        "add r1, #0x29\n\t"
        "ldrb r1, [r1]\n\t"
        "lsl r1, r1, #0x1c\n\t"
        "lsr r1, r1, #0x1c\n\t"
        "ldr r2, [r3, #0x20]\n\t"
        "add r3, #0x2d\n\t"
        "ldr r4, [r2]\n\t"
        "ldrb r5, [r3]\n\t"
        "lsl r2, r5, #3\n\t"
        "sub r2, r2, r5\n\t"
        "lsl r2, r2, #2\n\t"
        "add r2, r2, r4\n\t"
        "ldrb r2, [r2, #0x14]\n\t"
        "bl sub_8006D08\n\t"
        "ldr r0, _08023D68\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8028504\n\t"
    "_08023D0C:\n"
        "ldr r0, _08023D6C\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8008C80\n\t"
        "ldr r0, _08023D70\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8008C80\n\t"
        "ldr r0, _08023D74\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8008C80\n\t"
        "ldr r0, _08023D78\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8008C80\n\t"
        "add r0, r7, #0\n\t"
        "bl sub_802400C\n\t"
        "mov r0, #0\n\t"
        "bl sub_8001524\n\t"
        "bl sub_8001604\n\t"
        "bl sub_80015E0\n\t"
        "bl sub_8001614\n\t"
        "bl sub_8001624\n\t"
        "b _08023E5A\n\t"
        ".align 2, 0\n"
    "_08023D4C: .4byte gUnknown_030012C8\n"
    "_08023D50: .4byte gUnknown_030012D8\n"
    "_08023D54: .4byte gUnknown_030012D4\n"
    "_08023D58: .4byte gUnknown_030012B8\n"
    "_08023D5C: .4byte gUnknown_03001308\n"
    "_08023D60: .4byte gUnknown_030012C0\n"
    "_08023D64: .4byte gUnknown_030012BC\n"
    "_08023D68: .4byte gUnknown_03001318\n"
    "_08023D6C: .4byte gUnknown_030012F4\n"
    "_08023D70: .4byte gUnknown_030012EC\n"
    "_08023D74: .4byte gUnknown_030012F0\n"
    "_08023D78: .4byte gUnknown_030012F8\n"
    "_08023D7C:\n"
        "bl sub_802423C\n\t"
        "add r0, r7, #0\n\t"
        "bl sub_802400C\n\t"
        "ldr r5, _08023ED4\n\t"
        "ldr r0, [r5]\n\t"
        "bl sub_80007AC\n\t"
        "ldr r0, [r4]\n\t"
        "mov r1, #0x82\n\t"
        "lsl r1, r1, #1\n\t"
        "add r0, r0, r1\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne _08023DCA\n\t"
        "ldr r1, _08023ED8\n\t"
        "mov r0, #8\n\t"
        "ldrh r1, [r1, #2]\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "beq _08023DCA\n\t"
        "bl sub_8004D74\n\t"
        "add r4, r0, #0\n\t"
        "cmp r4, #0\n\t"
        "bne _08023DBE\n\t"
        "add r0, r7, #0\n\t"
        "bl sub_80241BC\n\t"
        "ldr r0, [r5]\n\t"
        "bl sub_80007AC\n\t"
    "_08023DBE:\n"
        "cmp r4, #1\n\t"
        "bne _08023DC4\n\t"
        "b _08023BB8\n\t"
    "_08023DC4:\n"
        "cmp r4, #2\n\t"
        "bne _08023DCA\n\t"
        "b _08023BBE\n\t"
    "_08023DCA:\n"
        "ldr r0, _08023ED8\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #4\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "beq _08023DDE\n\t"
        "ldr r0, _08023EDC\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8028504\n\t"
    "_08023DDE:\n"
        "ldr r0, _08023EE0\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_800891C\n\t"
        "ldr r0, _08023EE4\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_800891C\n\t"
        "ldr r4, _08023EE8\n\t"
        "ldr r0, [r4]\n\t"
        "ldr r2, [r0, #0x18]\n\t"
        "mov r3, #0x38\n\t"
        "ldrsh r1, [r2, r3]\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, [r2, #0x3c]\n\t"
        "bl sub_803AD7C\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq _08023E16\n\t"
        "ldr r0, [r4]\n\t"
        "ldr r2, [r0, #0x18]\n\t"
        "mov r5, #0x18\n\t"
        "ldrsh r1, [r2, r5]\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, [r2, #0x1c]\n\t"
        "bl sub_803AD7C\n\t"
    "_08023E16:\n"
        "ldr r0, _08023EEC\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_80091D4\n\t"
        "ldr r0, _08023EF0\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_800891C\n\t"
        "ldr r0, _08023EF4\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_800891C\n\t"
        "ldr r0, _08023EF8\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_800891C\n\t"
        "ldr r0, _08023EDC\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8028400\n\t"
        "ldr r0, _08023EFC\n\t"
        "ldr r1, [r0]\n\t"
        "add r0, r1, #0\n\t"
        "add r0, #0x8c\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq _08023E52\n\t"
        "add r0, r1, #0\n\t"
        "bl sub_8022F2C\n\t"
    "_08023E52:\n"
        "ldr r1, _08023F00\n\t"
        "ldr r0, [r1]\n\t"
        "add r0, #1\n\t"
        "str r0, [r1]\n\t"
    "_08023E5A:\n"
        "bl sub_80241B0\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne _08023E72\n\t"
        "ldr r4, _08023EE8\n\t"
        "ldr r1, [r4]\n\t"
        "mov r0, #1\n\t"
        "ldrb r1, [r1, #0xc]\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "beq _08023D7C\n\t"
    "_08023E72:\n"
        "bl sub_80014A4\n\t"
        "bl sub_80241B0\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne _08023E82\n\t"
        "b _08023F92\n\t"
    "_08023E82:\n"
        "mov r0, #0\n\t"
        "mov sl, r0\n\t"
        "add r0, r7, #0\n\t"
        "bl sub_8024404\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne _08023F08\n\t"
        "ldr r6, _08023EFC\n\t"
        "ldr r0, [r6]\n\t"
        "bl sub_80232B8\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq _08023F08\n\t"
        "ldr r0, [r6]\n\t"
        "bl sub_8023104\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, _08023F04\n\t"
        "add r4, r0, r1\n\t"
        "ldr r0, _08023EE8\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0, #4]\n\t"
        "mov r2, #0x90\n\t"
        "lsl r2, r2, #5\n\t"
        "add r5, r0, r2\n\t"
        "str r4, [sp, #8]\n\t"
        "str r5, [sp, #0xc]\n\t"
        "ldr r4, [r6]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8023104\n\t"
        "bl sub_801B29C\n\t"
        "add r1, r0, #0\n\t"
        "add r0, r4, #0\n\t"
        "add r2, sp, #8\n\t"
        "bl sub_802356C\n\t"
        "b _08023F92\n\t"
        ".align 2, 0\n"
    "_08023ED4: .4byte gUnknown_03001304\n"
    "_08023ED8: .4byte gUnknown_030007E0\n"
    "_08023EDC: .4byte gUnknown_03001318\n"
    "_08023EE0: .4byte gUnknown_030012F4\n"
    "_08023EE4: .4byte gUnknown_030012E8\n"
    "_08023EE8: .4byte gUnknown_030012D8\n"
    "_08023EEC: .4byte gUnknown_0300130C\n"
    "_08023EF0: .4byte gUnknown_030012EC\n"
    "_08023EF4: .4byte gUnknown_030012F0\n"
    "_08023EF8: .4byte gUnknown_030012F8\n"
    "_08023EFC: .4byte gUnknown_030012C0\n"
    "_08023F00: .4byte gUnknown_0300082C\n"
    "_08023F04: .4byte 0xFFFFE200\n"
    "_08023F08:\n"
        "add r0, r7, #0\n\t"
        "bl sub_80243E0\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne _08023F44\n\t"
        "ldr r4, _08023F3C\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8023290\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq _08023F44\n\t"
        "ldr r2, _08023F40\n\t"
        "ldr r2, [r2]\n\t"
        "ldr r0, [r2]\n\t"
        "ldr r1, [r2, #4]\n\t"
        "str r0, [sp, #0x10]\n\t"
        "str r1, [sp, #0x14]\n\t"
        "ldr r0, [r4]\n\t"
        "add r2, sp, #0x10\n\t"
        "mov r1, #0\n\t"
        "bl sub_802356C\n\t"
        "b _08023F92\n\t"
        ".align 2, 0\n"
    "_08023F3C: .4byte gUnknown_030012C0\n"
    "_08023F40: .4byte gUnknown_030012D8\n"
    "_08023F44:\n"
        "mov r7, #0\n\t"
        "mov r5, #0\n\t"
        "ldr r1, _08023FF0\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r7, r0\n\t"
        "bge _08023F88\n\t"
        "add r6, r1, #0\n\t"
    "_08023F54:\n"
        "ldr r0, [r6]\n\t"
        "ldr r1, [r0, #8]\n\t"
        "lsl r0, r5, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r4, [r0]\n\t"
        "ldr r1, [r4, #0x18]\n\t"
        "add r1, #0x48\n\t"
        "mov r3, #0\n\t"
        "ldrsh r0, [r1, r3]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r1, #4]\n\t"
        "bl sub_803AD7C\n\t"
        "cmp r0, #3\n\t"
        "bne _08023F7E\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x4e\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0xa\n\t"
        "bne _08023F7E\n\t"
        "add r7, #1\n\t"
    "_08023F7E:\n"
        "add r5, #1\n\t"
        "ldr r0, [r6]\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r5, r0\n\t"
        "blt _08023F54\n\t"
    "_08023F88:\n"
        "ldr r0, _08023FF4\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r7, #0\n\t"
        "bl sub_8023140\n\t"
    "_08023F92:\n"
        "ldr r0, _08023FF8\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8008CEC\n\t"
        "ldr r0, _08023FF0\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8009914\n\t"
        "ldr r0, _08023FFC\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8008CEC\n\t"
        "ldr r0, _08024000\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8008CEC\n\t"
        "ldr r0, _08024004\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8008CEC\n\t"
        "ldr r0, _08024008\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8008CEC\n\t"
        "bl sub_8001578\n\t"
        "bl sub_8001564\n\t"
        "bl sub_8001550\n\t"
        "bl sub_800153C\n\t"
        "bl sub_800158C\n\t"
        "bl sub_80006A8\n\t"
        "bl sub_8001614\n\t"
        "mov r0, sl\n\t"
        "add sp, #0x18\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    "_08023FF0: .4byte gUnknown_0300130C\n"
    "_08023FF4: .4byte gUnknown_030012C0\n"
    "_08023FF8: .4byte gUnknown_030012E8\n"
    "_08023FFC: .4byte gUnknown_030012EC\n"
    "_08024000: .4byte gUnknown_030012F0\n"
    "_08024004: .4byte gUnknown_030012F8\n"
    "_08024008: .4byte gUnknown_030012F4\n"
    );
}
#endif
