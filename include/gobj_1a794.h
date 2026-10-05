#ifndef GUARD_GOBJ_1A794_H
#define GUARD_GOBJ_1A794_H

#include "mover_new.h"

/* Shared by src/graphics/actor_part_1a794.c/_1a878.c/_1ab34.c/_1ab98.c/
 * _1b208.c (GitHub issue #25, ROM 0x0801A794-0x0801B85C).
 *
 * Two C++-style classes (gcc 2.x method tables of {s16 this-adjust; fn}
 * pairs, called through the _call_via_r1/AD80/AD84/AD88 "call via
 * r1/r2/r3/r4" thunks):
 *
 * - `struct gobj`, a 0x80-byte level object built by CreatePlatform (method
 *   table gPlatformVtable: +0x0C CheckPlatformContact player-contact test,
 *   +0x1C UpdatePlatform per-frame/destroy step, +0x4C GetPlatformClassId,
 *   +0x54 DestroyPlatform destructor). Its `type` (+0x78) comes from the
 *   level's spawn record or is forced by the spawn kind; types 1/5/6/7
 *   get a `struct mover` attached at +0x44. The player object
 *   (gPlayer) uses the same layout for the fields read here,
 *   and `carried` (+0xAC) is the object the player is standing on.
 * - `struct mover`, a 0x38-byte helper (method table gPlatformMoverVtable:
 *   +0x0C UpdatePlatformMover per-frame move, +0x4C DestroyPlatformMover destructor,
 *   +0x5C StartPlatformMoverMotionXFromSet / +0x64 StartPlatformMoverMotionYFromSet velocity setters) that
 *   oscillates its owner back and forth over `rangeX`/`rangeY` pixels
 *   using the 12-byte velocity records of gStaticData_0816C460, and drags
 *   the player along while it is `active` (MovePlayerWithPlatform). */

struct vec3
{
    s32 x;
    s32 y;
    s32 z;
};

/* A sprite object's per-axis speed ramp (struct gobj.rampX/rampY, the
 * 12-byte motion records of gCtrlMotionRecords and the gStaticData_0816C*
 * entry sets): each frame ApplySpriteVelocity steps speedX/speedY by `step`
 * toward `target` without overshooting. The Start...MotionX/Y setters also
 * load `start` into the speed; the Set... ones keep the current speed. */
struct speed_ramp
{
    s32 start;
    s32 step;
    s32 target;
};

struct vec_pair
{
    u32 a;
    u32 b;
};

struct method
{
    s16 thisOffset;
    u8 unk_2[2];
    void *fn;
};

/* anim_rec's 0x04-0x09 collision box, accessed through a pointer to it
 * (not embedded: this compiler pads every struct to a word multiple) */
struct anim_box
{
    s16 offX;   // 0x00
    s16 offY;   // 0x02
    u8 padX;    // 0x04
    u8 padY;    // 0x05
};

struct anim_rec
{
    u8 unk_00[4];
    s16 offX;   // 0x04 - struct anim_box
    s16 offY;   // 0x06
    u8 padX;    // 0x08
    u8 padY;    // 0x09
    u8 unk_0A[0xA];
    u8 paletteId; // 0x14 - GetPaletteSlot(gPaletteCache, paletteId) gives the OBJ palette slot
    u8 unk_15;
    u8 frames;  // 0x16
    u8 unk_17[5];
};

struct anim_table
{
    struct anim_rec *records;
};

struct gobj_vtable
{
    u8 unk_00[0x10];
    struct method m10; // 0x10 - returns the platform record the object stands on (UpdateGroundSprite)
    u8 unk_18[0x20];
    struct method m38; // 0x38
    u8 unk_40[0x20];
    struct method m60; // 0x60
    struct method m68; // 0x68
};

struct mover;

struct gobj
{
    s32 x;              // 0x00
    s32 y;              // 0x04
    u16 id;             // 0x08
    u8 unk_0A[2];
    u8 flags;           // 0x0C
    u8 flags2;          // 0x0D
    u8 unk_0E[0xA];
    struct gobj_vtable *vtable; // 0x18
    void *platform;     // 0x1C - the last m10 record (UpdateGroundSprite/sub_800A590)
    struct anim_table *anim; // 0x20
    u8 dir;             // 0x24
    u8 screenSpace;     // 0x25 - 1: x/y are screen coordinates (DrawSpriteAt skips WorldToScreen;
                        //        always counts as on screen). GetSpriteScreenSpace/SetSpriteScreenSpace
    u8 unk_26[2];
    u8 mirror;          // 0x28 - bit 4: X mirrored, bit 5: Y mirrored
    u8 slot;            // 0x29 - low nibble: palette/tile slot
    u8 unk_2A[2];
    u8 animating;       // 0x2C - nonzero while the keyframe timer runs (box_part.h)
    u8 tag;             // 0x2D
    u8 unk_2E[2];
    s32 frame;          // 0x30
    s32 stepTimer;      // 0x34 - ticks spent on the current step (ResetSpriteFrameTimer)
    u8 animDone;        // 0x38 - set once a non-looping animation ends (SetSpriteAnimDone)
    u8 unk_39[0xB];
    struct mover *mover; // 0x44
    struct speed_ramp rampX; // 0x48 - speedX's ramp (ApplySpriteVelocity)
    struct speed_ramp rampY; // 0x54 - speedY's ramp
    s32 speedX;         // 0x60
    s32 speedY;         // 0x64
    u8 hitAxes;         // 0x68 - collision axes the terrain probe resolved (8: Y, standing; 4: X)
    u8 probeTries;      // 0x69
    u8 unk_6A[2];
    s32 prevX;          // 0x6C - previous position (Q8), cached by ApplySpriteVelocity
    s32 prevY;          // 0x70
    s32 hitMask;        // 0x74 - probe axes hit this frame (OR-accumulated, see box_part.h)
    s32 type;           // 0x78
    u8 unk_7C[4];
    u8 busy;            // 0x80 - set while a triggered crate animation runs (the crate's state
                        //        bit 7), cleared when it ends; enemies skip the player meanwhile
    u8 unk_81[7];
    u8 ctrlMode;        // 0x88 - player control mode 0-3: picks the `mover` controller update
                        //        (actor_part48.c); nonzero freezes `list` (sub_800B58C/sub_800B650/sub_800B678)
    u8 unk_89[3];
    u32 deadline;       // 0x8C - gRoomFrameCount frame IsPlayerInvulnerable tests against
    u8 bumped;          // 0x90 - set when a crate's side stopped the player's X motion
                        //        (ActionCtrlHandleEvent event 12); cleared when the
                        //        controller's bumpTimer runs out or its mode changes
    u8 countdown;       // 0x91
    u8 unk_92;          // 0x92 - a counter
    u8 unk_93;
    u8 listCount;       // 0x94
    u8 unk_95[3];
    s32 list[5];        // 0x98 - appended to by sub_800B678
    struct gobj *carried; // 0xAC
    /* The rest is only reached by the base-class accessors in
     * actor_part16.c. */
    u8 unk_B0[0x50];
    u8 slippery;        // 0x100 - standing on terrain kind 5 (CollidePlayer): the player keeps
                        //         sliding (speedX isn't zeroed, motion keeps its speed, steps halve)
                        //         and skids (anims 0x25/0x26, sfx 0x36; ActionCtrlSetTargetAnim)
    u8 hanging;         // 0x101 - hanging from hang terrain (code 6): CollidePlayer sends event
                        //         0x17 to grab and 0x18 when it's gone; ActionCtrlHandleEvent sets/clears it
    u8 pushLeft;        // 0x102 - nonzero: moves the standing player 1px left per frame
    u8 pushRight;       // 0x103 - nonzero: moves the standing player 1px right per frame
    u8 dead;            // 0x104 - the player died (KillPlayer and the other controllers' kill handlers); blocks pause and further hits
    u8 unk_105[3];
    u8 collisionQueue[4]; // 0x108 - the embedded collision queue (ResetCollisionQueue/
                          //         DestroyCollisionQueue; GetPlayerCollisionQueue returns its address)
};

struct mover_vtable
{
    u8 unk_00[8];
    struct method m08; // 0x08
    struct method m10; // 0x10
    struct method m18; // 0x18
    u8 unk_20[0x40];
    struct method m60; // 0x60
};

struct mover
{
    u8 unk_00[4];
    struct { struct vec_pair *entries; } *set; // 0x04
    u8 unk_08[4];
    struct mover_vtable *vtable; // 0x0C
    s32 kind;           // 0x10
    s32 timer;          // 0x14
    s32 distX;          // 0x18
    s32 distY;          // 0x1C
    s32 lastX;          // 0x20
    s32 lastY;          // 0x24
    s32 rangeX;         // 0x28
    s32 rangeY;         // 0x2C
    u8 dirX;            // 0x30
    u8 dirY;            // 0x31
    u8 active;          // 0x32
    u8 unk_33;
    u32 time;           // 0x34
};

struct pos2
{
    s32 x;
    s32 y;
};

struct aabb
{
    s32 x;
    s32 y;
    s32 w;
    s32 h;
};

struct spawn_rec
{
    u8 flags;           // 0x00
    u8 unk_01[3];
    u32 type;           // 0x04
    s32 distX;          // 0x08
    s32 distY;          // 0x0C
    s16 dirX;           // 0x10
    s16 dirY;           // 0x12
    s16 flag;           // 0x14
};

extern struct gobj *gPlayer;
extern void *gLevelState;
extern void *gPaletteCache;
extern void *gUnknown_030012EC;
extern u8 *gEntityFlags;
extern u8 ***gSpriteBankSet;
extern u32 gRoomFrameCount;
extern struct vec_pair gStaticData_0816C418[];
extern struct vec3 gStaticData_0816C3B8[];
extern struct vec3 gStaticData_0816C460[];
extern u8 gStaticData_0816C458[];
extern u8 gDingodileShieldVtable[];
extern u8 gDingodileVtable[];
extern u8 gPlatformVtable[];
extern u8 gPlatformMoverVtable[];

extern void *sub_8017A8C(void *self);
extern void sub_8017A78(void *self, s32 flags);
extern void SpawnDingodileShieldOrRocket(void *self, s32 a, u16 b, u16 c, s32 d);
extern void *OperatorNew(u32 size);
extern void InitMovingSprite(void *self);
extern void DestroyMovingSprite(void *self, s32 flags);
extern s32 _call_via_r1(void *self, void *fn);
extern s32 _call_via_r2(void *self, void *arg, void *fn);
extern s32 _call_via_r3(void *self, void *arg1, s32 arg2, void *fn);
extern void _call_via_r4(void *self, s32 a, s32 b, s32 c);
extern u32 __umodsi3(u32 a, u32 b);
extern s32 GetBossIndex(void *arg);
extern u8 IsBonusRoundDone(void *arg);
extern u8 IsGemPathDone(void *arg);
extern struct mover *CreateCortexBossPlatformMover(void *mem);
extern void AddToPartList(void *manager, void *value);
extern void ResetSpriteFrameTimer(void *self);
extern void ResetSpriteFrameIndex(void *self);
extern void SetSpriteAnimDone(void *self, s32 a);
extern u8 GetPaletteSlot(void *cache, u8 id);
extern void GetSpriteHitbox(struct aabb *dest, struct gobj *obj);
extern u8 AabbOverlaps(struct aabb *a, struct aabb *b);
extern s32 GetSpritePrevY(struct gobj *obj);
extern s32 GetSpritePrevX(struct gobj *obj);
extern s32 FindLineCrossing(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void SetEntityPos(struct gobj *obj, s32 x, s32 y);
extern void AdvanceSpriteAnim(struct gobj *obj);
extern void SetSpritePrevPos(struct gobj *obj);
extern void StartCtrlTargetMotionY(void *self, void *part, struct vec3 *vec);
extern void StartCtrlTargetMotionX(void *self, void *part, struct vec3 *vec);
extern void DestroyCtrl(void *self, s32 flags);
extern void InitCtrl(void *self);

void sub_801B2D8(struct gobj *self);

/* The object's constructor body (InitPlatform), which CreatePlatform inlines
 * into its `new`. */
static inline struct gobj *GobjInit(struct gobj *self)
{
    InitMovingSprite(self);
    self->vtable = (struct gobj_vtable *)gPlatformVtable;
    sub_801B2D8(self);
    return self;
}
void sub_801B2A8(struct gobj *self, u8 value);
void ResolvePlatformCollision(struct gobj *self, void *unused);
void MovePlayerWithPlatform(struct mover *self, struct gobj *obj);

/* Branchless absolute value, updating `x` in place (same helper as
 * actor_part50.c) - the ROM's asr/eor/sub sequence. */
#define ABS32(x, sign) do { (sign) = (x) >> 0x1f; (x) ^= (sign); (x) -= (sign); } while (0)

#define MOVER_CALL3(obj, m, a, b)                                              \
    do                                                                         \
    {                                                                          \
        struct method *_m = &(obj)->vtable->m;                                 \
        _call_via_r3((u8 *)(obj) + _m->thisOffset, (a), (b), _m->fn);           \
    } while (0)

/* _call_via_r4 calls the function in r4 */
#define OBJ_CALL68(obj, a, b, c)                                               \
    do                                                                         \
    {                                                                          \
        struct method *_m = &(obj)->vtable->m68;                               \
        void *_this = (u8 *)(obj) + _m->thisOffset;                            \
        register void *_fn asm("r4") = _m->fn;                                 \
                                                                               \
        asm volatile("" : : "r"(_fn));                                         \
        _call_via_r4(_this, (a), (b), (c));                                     \
    } while (0)

#define OBJ_CALL1(obj, m)                                                      \
    do                                                                         \
    {                                                                          \
        struct method *_m = &(obj)->vtable->m;                                 \
        _call_via_r1((u8 *)(obj) + _m->thisOffset, _m->fn);                     \
    } while (0)

#define MOVER_CALL2(obj, m, a)                                                 \
    do                                                                         \
    {                                                                          \
        struct method *_m = &(obj)->vtable->m;                                 \
        _call_via_r2((u8 *)(obj) + _m->thisOffset, (a), _m->fn);                \
    } while (0)

#endif /* GUARD_GOBJ_1A794_H */
