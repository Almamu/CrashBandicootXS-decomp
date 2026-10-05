#include "core.h"
#include "actor.h"
#include "level_data.h"

extern void CreateEntitySpawner(void);
extern void ClearRoomExit(void);
extern void *OperatorNew(s32 size);
extern struct dual_array_manager *InitPartList(struct dual_array_manager *manager, s32 count);
extern struct pool_manager *InitCrateList(struct pool_manager *manager, s32 count);
extern void *GetLevelLayers(void);
extern void *InitPlayer();
extern void SetEntityPos(struct actor *self, s32 arg1, s32 arg2);
extern void OperatorDelete(void *self);
extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern void *InitActionCtrl(void *selfArg);
extern void SetCtrlAnimSet(void *selfArg, s32 val);
extern void *InitPlayerCtrl(void *arg0);
extern void *CreateInputCtrl(void *arg0);
extern void ResetSpriteFrameTimer(void *part);
extern void ResetSpriteFrameIndex(void *part);
extern void SetSpriteAnimDone(void *part, u8 val);
extern s32 RunRoom(void *self);
extern void DestroyLevelLayers(void *self, s32 flag);
extern void DestroyPartList(struct dual_array_manager *manager, s32 flags);
extern void DestroyCrateList(struct pool_manager *manager, s32 flags);
extern void DestroyEntitySpawner(void);

extern struct dual_array_manager *gUnknown_030012E8;
extern struct dual_array_manager *gUnknown_030012EC;
extern struct pool_manager *gCrateList;
extern struct dual_array_manager *gCollidableList;
extern struct dual_array_manager *gDecorationList;
extern struct dual_array_manager *gUnknown_030012F4;
extern void *gCamera;
extern void *gLevelLayers;
extern void *gPlayer;
extern void *gPlayerCtrl;
extern void ***gSpriteBankSet;
extern u8 gStaticData_0816B92C[];
extern u8 gStaticData_0816B934[];
extern u8 gStaticData_0816B93C[];

/* PlayRoom's argument (game_loop55.c passes `&self->level`; the same
 * record game_loop56.c's RunRoom reads as `struct gl_self`). */
struct level_start_args {
    s32 level;                      // 0x00
    u8 unk_04[0xC];
    s32 spawnX;                     // 0x10 - the player's start position
    s32 spawnY;                     // 0x14
    const struct level_room *room;  // 0x18
    u8 flags;                       // 0x1C - bit 0: start X-mirrored
};

/* The HUD widget's method table: a gcc 2.x {this-adjust, fn} record at
 * +0x18, called with the player as its argument. */
struct widget_method {
    s16 thisOffset;
    u8 unk_02[2];
    void *fn;
};

struct widget_vtable {
    u8 unk_00[0x18];
    struct widget_method attach;    // 0x18
};

struct widget {
    u8 unk_00[0xC];
    struct widget_vtable *vtable;   // 0x0C
};

/* Level-start dispatcher, called once from `UpdateGameFrame` when the
 * level object's own `+0xdc->+8` state field is `2` (see
 * `asm/code_3_2_17_225a0.s`). Allocates the whole per-level widget set
 * (ring-buffer/pool object families already matched in
 * `actor_part11.c`/`actor_part12.c`: `gUnknown_030012E8/EC/F0/F8/F4` are
 * `dual_array_manager`s, `gCrateList` a `pool_manager`), the
 * player actor itself (`gPlayer`, `InitPlayer`), and the
 * text-box singleton (`gLevelLayers`, `GetLevelLayers`). Dispatches on
 * the level-state record's (`self->0x18`) own `+8` "widget kind" field
 * to construct one of three HUD counter/ring-buffer widgets
 * (`gStaticData_0816B92C`/`0816B934`/`0816B93C`, still-uncharacterized
 * per-widget action tables), then unconditionally hands off to
 * `RunRoom` and tears the per-frame update queues back down before
 * returning its status code. */
s32 PlayRoom(void *selfArg)
{
    /* `self` is pinned to r8 for the whole function, matching the ROM:
     * it has to survive dozens of `bl`s while r4-r7 are already busy
     * with other live locals, so this compiler (like the ROM) needs a
     * `mov` through a low register before every field access - each
     * such access below is its own small register-pinned block for
     * that reason. */
    register struct level_start_args *self asm("r8") = selfArg;
    void **d8;
    s32 mode;
    s32 result;

    CreateEntitySpawner();
    ClearRoomExit();

    {
        struct dual_array_manager **slot = &gUnknown_030012E8;
        *slot = InitPartList(OperatorNew(0x14), 0x20);
    }
    {
        struct dual_array_manager **slot = &gUnknown_030012EC;
        *slot = InitPartList(OperatorNew(0x14), 0xc0);
    }
    {
        struct pool_manager **slot = &gCrateList;
        *slot = InitCrateList(OperatorNew(0x818), 0xc0);
    }
    {
        struct dual_array_manager **slot = &gCollidableList;
        *slot = InitPartList(OperatorNew(0x14), 0x80);
    }
    {
        struct dual_array_manager **slot = &gDecorationList;
        *slot = InitPartList(OperatorNew(0x14), 0x40);
    }
    {
        struct dual_array_manager **slot = &gUnknown_030012F4;
        *slot = InitPartList(OperatorNew(0x14), 0x40);
    }
    {
        void **slot = &gCamera;
        *slot = OperatorNew(0x18);
    }

    gLevelLayers = GetLevelLayers();

    d8 = &gPlayer;
    *d8 = InitPlayer(OperatorNew(0x350), 0xffff, 0, 0, 0);
    {
        struct level_start_args *p = self;
        SetEntityPos((struct actor *)*d8, p->spawnX, p->spawnY);
    }

    /* Register-pinned (rather than a plain `*p |= 0x10`) so the mask
     * value is loaded before the pointer's current byte, matching the
     * ROM's own operand-evaluation order for this store - a plain
     * compound assignment here evaluates the load first instead. */
    {
        register u8 *p asm("r1") = (u8 *)*d8;
        register u8 val asm("r0") = 0x10;
        register u8 cur asm("r3") = p[0xc];
        register u8 result asm("r0") = val | cur;
        p[0xc] = result;
    }
    {
        register u8 *p asm("r2") = (u8 *)*d8 + 0x28;
        register u32 one asm("r1") = 1;
        register struct level_start_args *sp asm("r4") = self;
        register u8 rawbit asm("r4") = sp->flags;
        register u32 bit asm("r1") = (one & rawbit) << 4;
        /* Register-pinned negative-constant mask (`-0x11`, not `~0x10`)
         * so this compiler emits the ROM's own `movs r0, #0x11 / rsbs
         * r0, r0, #0` runtime mask computation instead of
         * constant-folding it to a single immediate load - the
         * "negative-constant bit-clear idiom" documented in
         * docs/matching.md (see `sub_800A70C` in actor_part14.c for the
         * established `register ... = -N` shape this mirrors). */
        register s32 mask asm("r0") = -0x11;
        register u8 cur asm("r3") = *p;
        register s32 result asm("r0") = (mask & cur) | bit;
        *p = result;
    }

    /* The ROM re-derives `self` from `r8` into `r4` again here (a
     * redundant `mov r4, r8` this compiler's own value tracking would
     * otherwise elide, since r4 still holds that exact value from the
     * block above) - the barrier below forces the reload to keep the
     * instruction count matching. */
    asm volatile("" ::: "r4");
    {
        register struct level_start_args *p asm("r4") = self;
        mode = p->room->kind;
    }

    switch (mode) {
    case 0: {
        u8 *widget = InitActionCtrl(OperatorNew(0x38));

        SetCtrlAnimSet(widget, (s32)gStaticData_0816B92C);

        *((u8 *)*d8 + 0x88) = mode;
        {
            void *val = **gSpriteBankSet;
            u8 *pl = *d8;
            struct widget_vtable *w1c;
            s32 off;

            *(void **)(pl + 0x20) = val;
            *(void **)(pl + 0x44) = widget;

            w1c = ((struct widget *)widget)->vtable;
            off = w1c->attach.thisOffset;
            widget += off;
            _call_via_r2(widget, pl, w1c->attach.fn);
        }
        break;
    }
    case 1: {
        void *w;

        {
            void **slot = &gPlayerCtrl;
            *slot = InitPlayerCtrl(OperatorNew(0x30));
        }
        SetCtrlAnimSet(gPlayerCtrl, (s32)gStaticData_0816B934);

        *((u8 *)*d8 + 0x88) = mode;
        {
            void *val = (u8 *)**gSpriteBankSet + 0xc;
            u8 *pl = *d8;
            *(void **)(pl + 0x20) = val;
            {
                u8 v = 0x1f;
                pl[0x2d] = v;
            }
            ResetSpriteFrameTimer(pl);
            ResetSpriteFrameIndex(pl);
            SetSpriteAnimDone(pl, 0);
        }
        {
            u8 *pl = *d8;
            struct widget_vtable *w1c;
            s32 off;

            w = gPlayerCtrl;
            *(void **)(pl + 0x44) = w;
            w1c = ((struct widget *)w)->vtable;
            off = w1c->attach.thisOffset;
            w = (u8 *)w + off;
            _call_via_r2(w, pl, w1c->attach.fn);
        }
        break;
    }
    case 2: {
        u8 *widget = CreateInputCtrl(OperatorNew(0x28));

        SetCtrlAnimSet(widget, (s32)gStaticData_0816B93C);

        {
            u8 *pl = *d8;
            u8 v = 3;
            pl[0x88] = v;
        }
        {
            void *val = (u8 *)**gSpriteBankSet + 0x18;
            u8 *pl = *d8;
            struct widget_vtable *w1c;
            s32 off;

            *(void **)(pl + 0x20) = val;
            *(void **)(pl + 0x44) = widget;

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
        u8 *p = *(u8 **)((u8 *)gPlayer + 0x18) + 0x50;
        s32 off = *(s16 *)p;

        _call_via_r2((u8 *)gPlayer + off, (void *)3, *(void **)(p + 4));
    }

    if (gUnknown_030012F4 != NULL) {
        DestroyPartList(gUnknown_030012F4, 3);
    }
    if (gDecorationList != NULL) {
        DestroyPartList(gDecorationList, 3);
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
    if (gUnknown_030012E8 != NULL) {
        DestroyPartList(gUnknown_030012E8, 3);
    }

    DestroyEntitySpawner();

    return result;
}
