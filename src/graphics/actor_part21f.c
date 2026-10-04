#include "core.h"
#include "actor_self.h"

/* Same boss-weapon "self"/tracker object family as actor_part20.c/
 * actor_part21c.c/actor_part21d.c/actor_part21e.c - see
 * actor_part20.c's header comment and
 * docs/matching/issue-58-0x08030334-actor.md.
 *
 * Large weapon-kind projectile spawner: advances the position
 * accumulators (`gUnknown_03001540`/`gUnknown_03001544`/
 * `gUnknown_03001548`), clears `gUnknown_03001578`'s DMA-refresh
 * counter, and derives two base screen coordinates from a fixed
 * keyframe-table box (`gStaticData_0817C3D8`, `>>8`) offset by the
 * accumulators. Dispatches on `gUnknown_0300153C` (a frame/flags
 * counter, the same one `sub_8031504`'s palette fade reads) through
 * five weapon-kind cases (0xa/0x32/0x50/0x6e/0xaa), each clearing one
 * BG palette bank-1 slot then spawning 1-3 sub-projectiles via
 * `sub_8000E1C` (a per-axis jitter/randomizer) and `sub_802E420` (the
 * actual spawn call, `(x, y, z)`); the 0xaa case instead
 * fires the state-5/table-index-1 transition on the tracker object,
 * plays a sound, and - gated by a lock byte
 * (`gUnknown_030012C0+0x8c`) and a spawn-budget counter
 * (`gUnknown_0300157C`) - spawns a homing/seek effect via
 * `sub_802F4AC`/`sub_802E3CC`.
 *
 * Matching notes: the box table is `const` (so its jitter ranges stay
 * CSE'd in registers across the spawn calls), the palette base pointer
 * is assigned right where the ROM materializes it (declared-and-
 * initialized at the top it gets hoisted into a callee-saved register),
 * the RNG `sub_8000E1C` is read back as a `u16` here (the ROM zero-
 * extends its result), and the seek spawn takes `&gUnknown_03000884`
 * before the last lock check, as the ROM loads that address early. */
extern s32 gUnknown_03001540;
extern s32 gUnknown_03001558;
extern s32 gUnknown_03001544;
extern s32 gUnknown_0300155C;
extern s32 gUnknown_03001548;
extern s32 gUnknown_03001560;
extern s32 gUnknown_03001578;
extern const s16 gStaticData_0817C3D8[];
extern s32 gUnknown_0300153C;
extern u16 sub_8000E1C(s32 max);
extern void sub_802E420(s32 x, s32 y, s32 z);
extern void sub_802A4EC(void);
extern s32 gUnknown_03001538;
extern struct actor_self *gUnknown_03001534;
extern s32 GetAnimFrameBaseOffset(void *self);
extern void *gUnknown_030012BC;
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern u8 *gUnknown_030012C0;
extern s32 gUnknown_0300157C;
extern void *gUnknown_03000884;
extern u8 gUnknown_03001506;
extern s32 sub_802F4AC(void *arg0);
extern void sub_802E3CC(void);

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

/* One sub-projectile, jittered around (x, y) by the box's own +-range. */
#define SPAWN(x, y) sub_802E420((x) + sub_8000E1C(gStaticData_0817C3D8[3] << 8),  \
                                (y) + sub_8000E1C(gStaticData_0817C3D8[4] << 8),  \
                                gUnknown_03001548 - 0x100)

void sub_80309B4(void)
{
    s32 x, y;
    u16 *pal;

    gUnknown_03001540 += gUnknown_03001558;
    gUnknown_03001544 += gUnknown_0300155C;
    gUnknown_03001548 += gUnknown_03001560;
    gUnknown_03001578 = 0;
    pal = (u16 *)(BG_PLTT + 0x20);
    x = gUnknown_03001540 + (gStaticData_0817C3D8[0] << 8);
    y = gUnknown_03001544 + (gStaticData_0817C3D8[1] << 8);

    if (gUnknown_0300153C == 0xa) {
        pal[15] = 0;
        SPAWN(x, y);
    } else if (gUnknown_0300153C == 0x32) {
        pal[1] = 0;
        SPAWN(x, y);
        SPAWN(x, y);
    } else if (gUnknown_0300153C == 0x50) {
        pal[4] = 0;
        SPAWN(x, y);
        SPAWN(x, y);
        SPAWN(x, y);
    } else if (gUnknown_0300153C == 0x6e) {
        pal[8] = 0;
        SPAWN(x, y);
        SPAWN(x, y);
        SPAWN(x, y);
        SPAWN(x, y);
    } else if (gUnknown_0300153C == 0xaa) {
        sub_802A4EC();
        BossSetState(5, 1);
        PlaySfx(gUnknown_030012BC, 0x42, 0x100);
        gUnknown_03001560 = 0x9d;
        if (gUnknown_030012C0[0x8c] == 0 && gUnknown_0300157C <= 1) {
            void **pl = &gUnknown_03000884;
            if (gUnknown_03001506 == 0) {
                sub_802F4AC(*pl);
                sub_802E3CC();
                gUnknown_0300157C++;
            }
        }
    }
}
