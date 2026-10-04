#include "core.h"
#include "actor_self.h"

/* Same boss-weapon "self"/tracker object family as actor_part20.c/
 * actor_part23.c - see actor_part20.c's header comment and
 * docs/matching/issue-58-0x08030334-actor.md.
 *
 * A large "spawn/arm this weapon-kind instance" setup routine: resets
 * the ramp/velocity globals, fires the tracker object's state-1/
 * table-index-0 transition, seeds the position accumulators
 * (`gUnknown_03001540`/`gUnknown_03001544`/`gUnknown_03001548`) from
 * its own three arguments, looks up a per-kind keyframe-table record
 * (`gStaticData_0817C2D0`, indexed by both `gUnknown_03001564` - the
 * level index `sub_8030F88` stashed - and this function's own first
 * argument) and copies several of its fields into
 * `gUnknown_03001570`/`gUnknown_0300156C`, resets the DMA-refresh/
 * palette-strip counters, recomputes the BG2 zoom scale/offset via
 * `sub_8029B2C`/`sub_803ADB4`/`sub_8029E34`, blits the tracker's
 * current keyframe-table box via `sub_8030D48`, sets DISPCNT's bit10,
 * recomputes the BG2 affine matrix (`sub_80312C4`), and finally queues
 * a palette-strip DMA transfer (`QueueVramDmaTransfer`).
 *
 * Matching notes: the zoom divide is an explicit `sub_803ADB4` call
 * (the ROM reloads `gUnknown_03001554` after it, which `/`'s const
 * libcall wouldn't force) and the record lookup is written `a - -b` (see
 * below). `gUnknown_03001564` is the level index `sub_8030F88` caches,
 * not an object pointer. */
extern s32 gUnknown_03001560;
extern s32 gUnknown_03001538;
extern s32 gUnknown_0300153C;
extern struct actor_self *gUnknown_03001534;
extern s32 GetAnimFrameBaseOffset(void *self);
extern s32 gUnknown_03001540;
extern s32 gUnknown_03001544;
extern s32 gUnknown_03001548;
/* One 28-byte per-kind weapon record (`gStaticData_0817C2D0`); only the
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
extern struct weapon_kind *gUnknown_03001568;
extern s32 gUnknown_03001564;
extern struct weapon_kind gStaticData_0817C2D0[];
extern s32 gUnknown_03001570;
extern s32 gUnknown_0300156C;
extern s32 gUnknown_03001574;
extern u8 gUnknown_03001524;
extern s32 gUnknown_03001520;
extern s32 gUnknown_03001554;
extern s32 sub_8029B2C(void);
extern s32 gUnknown_0300154C;
extern s32 gUnknown_03001550;
extern void sub_8029E34(s32 arg0);
extern void sub_8030D48(u16 *src);
extern void sub_80312C4(void);
extern s32 gUnknown_03001578;
extern u8 gStaticData_0817C378[];
extern s32 QueueVramDmaTransfer(void *arg0, void *arg1, u16 arg2, u16 arg3);

extern s32 sub_803ADB4(s32 num, s32 den);

static inline void BossSetState(s32 st, s32 idx)
{
    struct actor_self *self;
    gUnknown_03001538 = st;
    gUnknown_0300153C = 0;
    self = gUnknown_03001534;
    self->animIndex = idx;
    self->animTimer = self->anims[idx].duration;
    self->animDone = 0;
    if (GetAnimFrameBaseOffset(self) >= self->anims[self->animIndex].loopThreshold)
        self->animTime = 0;
}

void sub_8031040(s32 kind, s32 x, s32 y, s32 z)
{
    s32 scale;
    struct actor_self *self;

    gUnknown_03001560 = 0x66;
    BossSetState(1, 0);
    gUnknown_03001540 = x * 5;
    gUnknown_03001544 = y * 2;
    gUnknown_03001548 = z + 0xA000;
    /* `a - -b` rather than `a + b`: the latter lets fold reassociate the
     * constant table base out of `&table[kind]`, while the ROM adds the
     * level offset to the finished record address. */
    gUnknown_03001568 = (struct weapon_kind *)(gUnknown_03001564 * (s32)sizeof(struct weapon_kind) - -(s32)&gStaticData_0817C2D0[kind]);
    gUnknown_03001570 = gUnknown_03001568->unk_0C;
    gUnknown_0300156C = gUnknown_03001568->unk_00;
    gUnknown_03001574 = 0;
    gUnknown_03001524 = 1;
    gUnknown_03001520 = 0;
    gUnknown_03001554 = gUnknown_03001548 - (sub_8029B2C() << 8);
    scale = sub_803ADB4(0x1C00000, gUnknown_03001554);
    gUnknown_0300154C = (gUnknown_03001540 * scale) >> 12;
    gUnknown_03001550 = (scale * gUnknown_03001544) >> 12;
    sub_8029E34(gUnknown_03001554);
    self = gUnknown_03001534;
    {
        s32 t = self->animTime >> 8;
        sub_8030D48((u16 *)self->frameOffsets[self->anims[self->animIndex].frameIndex + t]);
    }
    REG_DISPCNT |= 0x400;
    sub_80312C4();
    gUnknown_03001578 = 0;
    QueueVramDmaTransfer(gStaticData_0817C378, (void *)(BG_PLTT + 0x20), 0x20, 0x10);
}
