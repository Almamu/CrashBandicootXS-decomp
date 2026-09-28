#include "core.h"
#include "memory.h"
#include "actor_self.h"

/* Same boss-weapon "self"/tracker object family as actor_part20.c/
 * actor_part23.c - see actor_part20.c's header comment and
 * docs/matching/issue-58-0x08030334-actor.md.
 *
 * Constructor for the small tracker object (`gUnknown_03001534`):
 * stashes its level-index argument into `gUnknown_03001564`, and - if
 * `sub_802973C` (a level-index/mode query) returns zero - clears
 * `gUnknown_0300157C`'s spawn-budget counter. Seeds the row/column
 * dimensions (`gUnknown_03001528`/`gUnknown_0300152C`) from
 * `gStaticData_08167CD4`'s first two halfwords, allocates the 0x1c-byte
 * tracker object, wires its event table (`gStaticData_0817C3E4`) and
 * part table (`gUnknown_03001580`) pointers plus a fixed `+0x18` flag,
 * registers it via `sub_803B0A8`, and stores it into
 * `gUnknown_03001534`. Resets both boss-weapon state globals
 * (`gUnknown_03001538`/`gUnknown_0300153C`) and fires the tracker's own
 * state-0/table-index-0 transition (anim frame from its own part-table
 * pointer at `+0`). Finishes by running `sub_8031504` once (the DMA/
 * tile-cache setup + palette fade, actor_part26b.c) and clearing
 * `gUnknown_03001524`'s "apply now" latch.
 *
 * Matched as the inlined C++ `gUnknown_03001534 = new Tracker(...)`:
 * the destination's address is taken before the allocation, the size
 * goes through an `operator new`-style inline wrapper (materialized
 * before the heap flags), and the part-table setup is an inlined
 * constructor taking its values as arguments (all loaded before the
 * stores). */
extern s32 gUnknown_0300157C;
extern s32 gUnknown_03001528;
extern s32 gUnknown_0300152C;
extern struct actor_self *gUnknown_03001534;
extern s32 gUnknown_03001538;
extern s32 gUnknown_0300153C;
extern u8 gUnknown_03001524;
extern s32 gUnknown_03001564;
extern s32 sub_802973C(void);
extern void sub_803B0A8(void *self, s32 idx);
extern s32 GetAnimFrameBaseOffset(void *self);
extern void sub_8031504(void);
extern const s16 gStaticData_08167CD4[];
extern struct anim_frame_record gStaticData_0817C3E4[];
extern u32 gUnknown_03001580[];

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

static inline struct actor_self *AllocActor(u32 size)
{
    return (struct actor_self *)mem_alloc(size, MEM_HEAP_IWRAM);
}

static inline void InitAnimPart(struct actor_self *self, struct anim_frame_record *anims, u32 *offsets, s32 flag)
{
    self->anims = anims;
    self->frameOffsets = offsets;
    self->unk_18 = flag;
    sub_803B0A8(self, 0);
}

void sub_8030F88(s32 level)
{
    struct actor_self *t;
    struct actor_self **slot;

    gUnknown_03001564 = level;
    if (sub_802973C() == 0)
        gUnknown_0300157C = 0;
    gUnknown_03001528 = gStaticData_08167CD4[0];
    gUnknown_0300152C = gStaticData_08167CD4[1];
    slot = &gUnknown_03001534;
    t = AllocActor(0x1c);
    InitAnimPart(t, gStaticData_0817C3E4, gUnknown_03001580, 1);
    *slot = t;
    BossSetState(0, 0);
    sub_8031504();
    gUnknown_03001524 = 0;
}
