#include "core.h"
#include "match.h"
#include "actor.h"
#include "level_data.h"
#include "crates.h"
#include "player.h"
#include "gfx.h"
#include "objects.h"
#include "memory.h"
#include "level.h"
#include "globals.h"

extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);

/* The HUD widget's method table: a gcc 2.x {this-adjust, fn} record at
 * +0x18, called with the player as its argument. */
struct widget_vtable {
    u8 unk_00[0x18];
    struct actor_method attach; // 0x18
};

struct widget {
    u8 unk_00[0xC];
    struct widget_vtable *vtable; // 0x0C
};

/* Level-start dispatcher, called once from `UpdateGameFrame` when the
 * level object's own `+0xdc->+8` state field is `2` (see
 * `asm/code_3_2_17_225a0.s`). Allocates the whole per-level widget set
 * (ring-buffer/pool object families already matched in
 * `part_list.c`/`crate_list.c`: `gUpdateOnlyPartList`, `gUnknown_030012EC`, `gCollidableList`,
 * `gDecorationList` and `gUnknown_030012F4` are
 * `dual_array_manager`s, `gCrateList` a `pool_manager`), the
 * player actor itself (`gPlayer`, `InitPlayer`), and the
 * text-box singleton (`gLevelLayers`, `GetLevelLayers`). Dispatches on
 * the current room's (`self->cat`) `kind`
 * to construct one of three HUD counter/ring-buffer widgets
 * (`gActionCtrlMotionSet`/`0816B934`/`0816B93C`, still-uncharacterized
 * per-widget action tables), then unconditionally hands off to
 * `RunRoom` and tears the per-frame update queues back down before
 * returning its status code. */
s32 PlayRoom(struct level_progress *selfArg)
{
    /* `self` is pinned to r8 for the whole function, matching the ROM:
     * it has to survive dozens of `bl`s while r4-r7 are already busy
     * with other live locals, so this compiler (like the ROM) needs a
     * `mov` through a low register before every field access - each
     * such access below is its own small register-pinned block for
     * that reason. */
    MATCH_HOLD_REG(struct level_progress *, self, r8) = selfArg;
    struct player **d8;
    s32 mode;
    s32 result;

    CreateEntitySpawner();
    ClearRoomExit();

    {
        struct part_list **slot = &gUpdateOnlyPartList;
        *slot = InitPartList(OperatorNew(0x14), 0x20);
    }
    {
        struct part_list **slot = &gUnknown_030012EC;
        *slot = InitPartList(OperatorNew(0x14), 0xc0);
    }
    {
        struct pool_manager **slot = &gCrateList;
        *slot = InitCrateList(OperatorNew(0x818), 0xc0);
    }
    {
        struct part_list **slot = &gCollidableList;
        *slot = InitPartList(OperatorNew(0x14), 0x80);
    }
    {
        struct part_list **slot = &gDecorationList;
        *slot = InitPartList(OperatorNew(0x14), 0x40);
    }
    {
        struct part_list **slot = &gUnknown_030012F4;
        *slot = InitPartList(OperatorNew(0x14), 0x40);
    }
    {
        struct camera **slot = &gCamera;
        *slot = OperatorNew(0x18);
    }

    gLevelLayers = GetLevelLayers();

    d8 = &gPlayer;
    *d8 = InitPlayer(OperatorNew(0x350), 0xffff, 0, 0, 0);
    {
        struct level_progress *p = self;
        SetEntityPos((struct actor *)*d8, p->checkpointX, p->checkpointY);
    }

    /* Register-pinned (rather than a plain `*p |= 0x10`) so the mask
     * value is loaded before the pointer's current byte, matching the
     * ROM's own operand-evaluation order for this store - a plain
     * compound assignment here evaluates the load first instead. */
    {
        MATCH_HOLD_REG(u8 *, p, r1) = (u8 *)*d8;
        MATCH_HOLD_REG(u8, val, r0) = 0x10;
        MATCH_HOLD_REG(u8, cur, r3) = p[0xc];
        MATCH_HOLD_REG(u8, result, r0) = val | cur;
        p[0xc] = result;
    }
    {
        MATCH_HOLD_REG(u8 *, p, r2) = (u8 *)*d8 + 0x28;
        MATCH_HOLD_REG(u32, one, r1) = 1;
        MATCH_HOLD_REG(struct level_progress *, sp, r4) = self;
        MATCH_HOLD_REG(u8, rawbit, r4) = sp->flags;
        MATCH_HOLD_REG(u32, bit, r1) = (one & rawbit) << 4;
        /* Register-pinned negative-constant mask (`-0x11`, not `~0x10`)
         * so this compiler emits the ROM's own `movs r0, #0x11 / rsbs
         * r0, r0, #0` runtime mask computation instead of
         * constant-folding it to a single immediate load - the
         * "negative-constant bit-clear idiom" documented in
         * docs/matching.md (see `ClearSpriteObjFlag5` in ground_sprite.c for the
         * established `register ... = -N` shape this mirrors). */
        MATCH_HOLD_REG(s32, mask, r0) = -0x11;
        MATCH_HOLD_REG(u8, cur, r3) = *p;
        MATCH_HOLD_REG(s32, result, r0) = (mask & cur) | bit;
        *p = result;
    }

    /* The ROM re-derives `self` from `r8` into `r4` again here (a
     * redundant `mov r4, r8` this compiler's own value tracking would
     * otherwise elide, since r4 still holds that exact value from the
     * block above) - the barrier below forces the reload to keep the
     * instruction count matching. */
    MATCH_CLOBBER_VOLATILE(r4);
    {
        MATCH_HOLD_REG(struct level_progress *, p, r4) = self;
        mode = p->cat->kind;
    }

    switch (mode) {
    case 0:
        {
            u8 *widget = (u8 *)InitActionCtrl(OperatorNew(0x38));

            SetCtrlAnimSet(widget, (s32)&gActionCtrlMotionSet);

            (*d8)->ctrlMode = mode;
            {
                void *val = SPRITE_BANK_BASE;
                struct player *pl = *d8;
                struct widget_vtable *w1c;
                s32 off;

                pl->anim = val;
                pl->ctrl = widget;

                w1c = ((struct widget *)widget)->vtable;
                off = w1c->attach.thisOffset;
                widget += off;
                _call_via_r2(widget, pl, w1c->attach.fn);
            }
            break;
        }
    case 1:
        {
            void *w;

            {
                void **slot = &gPlayerCtrl;
                *slot = InitPlayerCtrl(OperatorNew(0x30));
            }
            SetCtrlAnimSet(gPlayerCtrl, (s32)&gPlayerCtrlMotionSet);

            (*d8)->ctrlMode = mode;
            {
                void *val = SPRITE_BANK_BASE + 0xc;
                struct player *pl = *d8;
                pl->anim = val;
                {
                    u8 v = 0x1f;
                    pl->tag = v;
                }
                ResetSpriteFrameTimer(pl);
                ResetSpriteFrameIndex(pl);
                SetSpriteAnimDone(pl, 0);
            }
            {
                struct player *pl = *d8;
                struct widget_vtable *w1c;
                s32 off;

                w = gPlayerCtrl;
                pl->ctrl = w;
                w1c = ((struct widget *)w)->vtable;
                off = w1c->attach.thisOffset;
                w = (u8 *)w + off;
                _call_via_r2(w, pl, w1c->attach.fn);
            }
            break;
        }
    case 2:
        {
            u8 *widget = (u8 *)CreateInputCtrl(OperatorNew(0x28));

            SetCtrlAnimSet(widget, (s32)&gInputCtrlMotionSet);

            {
                struct player *pl = *d8;
                u8 v = 3;
                pl->ctrlMode = v;
            }
            {
                void *val = SPRITE_BANK_BASE + 0x18;
                struct player *pl = *d8;
                struct widget_vtable *w1c;
                s32 off;

                pl->anim = val;
                pl->ctrl = widget;

                w1c = ((struct widget *)widget)->vtable;
                off = w1c->attach.thisOffset;
                widget += off;
                _call_via_r2(widget, pl, w1c->attach.fn);
            }
            break;
        }
    }

    result = RunRoom(self);

    if (gLevelLayers != NULL) {
        DestroyLevelLayers(gLevelLayers, 3);
    }
    OperatorDelete(gCamera);

    if (gPlayer != NULL) {
        const struct actor_method *p = &gPlayer->vtable->destroy;
        s32 off = p->thisOffset;

        _call_via_r2((u8 *)gPlayer + off, (void *)3, p->fn);
    }

    if (gUnknown_030012F4 != NULL) {
        DestroyPartList(gUnknown_030012F4, 3);
    }
    if (gDecorationList != NULL) {
        DestroyPartList((struct part_list *)gDecorationList, 3);
    }
    if (gCollidableList != NULL) {
        DestroyPartList(gCollidableList, 3);
    }
    if (gCrateList != NULL) {
        DestroyCrateList(gCrateList, 3);
    }
    if (gUnknown_030012EC != NULL) {
        DestroyPartList(gUnknown_030012EC, 3);
    }
    if (gUpdateOnlyPartList != NULL) {
        DestroyPartList((struct part_list *)gUpdateOnlyPartList, 3);
    }

    DestroyEntitySpawner();

    return result;
}
