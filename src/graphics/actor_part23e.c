#include "core.h"
#include "actor_self.h"

/* Same boss-weapon "self"/tracker object family as actor_part20.c/
 * actor_part23.c - see actor_part20.c's header comment and
 * docs/matching/issue-58-0x08030334-actor.md.
 *
 * A large "spawn/arm this weapon-kind instance" setup routine: resets
 * the ramp/velocity globals, fires the tracker object's state-1/
 * table-index-0 transition, seeds the position accumulators
 * (`gAirshipX`/`gAirshipY`/`gAirshipZ`) from
 * its own three arguments, looks up a per-kind keyframe-table record
 * (`gAirshipAttacks`, indexed by both `gAirshipLevel` - the
 * level index `CreateAirship` stashed - and this function's own first
 * argument) and copies several of its fields into
 * `gAirshipFireTimer`/`gAirshipHp`, resets the DMA-refresh/
 * palette-strip counters, recomputes the BG2 zoom scale/offset via
 * `sub_8029B2C`/`__divsi3`/`sub_8029E34`, blits the tracker's
 * current keyframe-table box via `DrawAirshipMap`, sets DISPCNT's bit10,
 * recomputes the BG2 affine matrix (`UpdateAirshipBg2`), and finally queues
 * a palette-strip DMA transfer (`QueueVramDmaTransfer`).
 *
 * Matching notes: the zoom divide is an explicit `__divsi3` call
 * (the ROM reloads `gAirshipDistance` after it, which `/`'s const
 * libcall wouldn't force) and the record lookup is written `a - -b` (see
 * below). `gAirshipLevel` is the level index `CreateAirship` caches,
 * not an object pointer. */
extern s32 gAirshipVelZ;
extern s32 gAirshipState;
extern s32 gAirshipStateTimer;
extern struct actor_self *gAirship;
extern s32 GetAnimFrameBaseOffset(void *self);
extern s32 gAirshipX;
extern s32 gAirshipY;
extern s32 gAirshipZ;
/* One 28-byte per-kind weapon record (`gAirshipAttacks`); only the
 * fields this file reads are meaningful names-wise. */
struct weapon_kind {
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
    s32 unk_18;
};
extern struct weapon_kind *gAirshipAttack;
extern s32 gAirshipLevel;
extern struct weapon_kind gAirshipAttacks[];
extern s32 gAirshipFireTimer;
extern s32 gAirshipHp;
extern s32 gAirshipVolleyCount;
extern u8 gAirshipBg2PageFlip;
extern s32 gAirshipBg2Page;
extern s32 gAirshipDistance;
extern s32 sub_8029B2C(void);
extern s32 gAirshipScreenX;
extern s32 gAirshipScreenY;
extern void sub_8029E34(s32 arg0);
extern void DrawAirshipMap(u16 *src);
extern void UpdateAirshipBg2(void);
extern s32 gAirshipHitFlashTimer;
extern u8 gAirshipHitFlashPalettes[];
extern s32 QueueVramDmaTransfer(void *arg0, void *arg1, u16 arg2, u16 arg3);

extern s32 __divsi3(s32 num, s32 den);

static inline void BossSetState(s32 st, s32 idx)
{
    struct actor_self *self;
    gAirshipState = st;
    gAirshipStateTimer = 0;
    self = gAirship;
    self->animIndex = idx;
    self->animTimer = self->anims[idx].duration;
    self->animDone = 0;
    if (GetAnimFrameBaseOffset(self) >= self->anims[self->animIndex].loopThreshold)
        self->animTime = 0;
}

void SpawnAirship(s32 kind, s32 x, s32 y, s32 z)
{
    s32 scale;
    struct actor_self *self;

    gAirshipVelZ = 0x66;
    BossSetState(1, 0);
    gAirshipX = x * 5;
    gAirshipY = y * 2;
    gAirshipZ = z + 0xA000;
    /* `a - -b` rather than `a + b`: the latter lets fold reassociate the
     * constant table base out of `&table[kind]`, while the ROM adds the
     * level offset to the finished record address. */
    gAirshipAttack = (struct weapon_kind *)(gAirshipLevel * (s32)sizeof(struct weapon_kind) - -(s32)&gAirshipAttacks[kind]);
    gAirshipFireTimer = gAirshipAttack->unk_0C;
    gAirshipHp = gAirshipAttack->unk_00;
    gAirshipVolleyCount = 0;
    gAirshipBg2PageFlip = 1;
    gAirshipBg2Page = 0;
    gAirshipDistance = gAirshipZ - (sub_8029B2C() << 8);
    scale = __divsi3(0x1C00000, gAirshipDistance);
    gAirshipScreenX = (gAirshipX * scale) >> 12;
    gAirshipScreenY = (scale * gAirshipY) >> 12;
    sub_8029E34(gAirshipDistance);
    self = gAirship;
    {
        s32 t = self->animTime >> 8;
        DrawAirshipMap((u16 *)self->frameOffsets[self->anims[self->animIndex].frameIndex + t]);
    }
    REG_DISPCNT |= 0x400;
    UpdateAirshipBg2();
    gAirshipHitFlashTimer = 0;
    QueueVramDmaTransfer(gAirshipHitFlashPalettes, (void *)(BG_PLTT + 0x20), 0x20, 0x10);
}
