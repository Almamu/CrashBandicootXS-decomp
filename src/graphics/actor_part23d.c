#include "core.h"
#include "memory.h"
#include "actor_self.h"

/* Same boss-weapon "self"/tracker object family as actor_part20.c/
 * actor_part23.c - see actor_part20.c's header comment and
 * docs/matching/issue-58-0x08030334-actor.md.
 *
 * Constructor for the small tracker object (`gAirship`):
 * stashes its level-index argument into `gUnknown_03001564`, and - if
 * `GetActorCheckpoint` (a level-index/mode query) returns zero - clears
 * `gUnknown_0300157C`'s spawn-budget counter. Seeds the row/column
 * dimensions (`gUnknown_03001528`/`gUnknown_0300152C`) from
 * `gAirshipPicture`'s first two halfwords, allocates the 0x1c-byte
 * tracker object, wires its event table (`gStaticData_0817C3E4`) and
 * part table (`gUnknown_03001580`) pointers plus a fixed `+0x18` flag,
 * registers it via `SetActorAnim`, and stores it into
 * `gAirship`. Resets both boss-weapon state globals
 * (`gAirshipState`/`gUnknown_0300153C`) and fires the tracker's own
 * state-0/table-index-0 transition (anim frame from its own part-table
 * pointer at `+0`). Finishes by running `LoadAirshipGraphics` once (the DMA/
 * tile-cache setup + palette fade, actor_part26b.c) and clearing
 * `gUnknown_03001524`'s "apply now" latch.
 *
 * Matched as the inlined C++ `gAirship = new Tracker(...)`:
 * the destination's address is taken before the allocation, the size
 * goes through an `operator new`-style inline wrapper (materialized
 * before the heap flags), and the part-table setup is an inlined
 * constructor taking its values as arguments (all loaded before the
 * stores). */
extern s32 gUnknown_0300157C;
extern s32 gUnknown_03001528;
extern s32 gUnknown_0300152C;
extern struct actor_self *gAirship;
extern s32 gAirshipState;
extern s32 gUnknown_0300153C;
extern u8 gUnknown_03001524;
extern s32 gUnknown_03001564;
extern s32 GetActorCheckpoint(void);
extern void SetActorAnim(void *self, s32 idx);
extern s32 GetAnimFrameBaseOffset(void *self);
extern void LoadAirshipGraphics(void);
extern const s16 gAirshipPicture[];
extern struct anim_frame_record gStaticData_0817C3E4[];
extern u32 gUnknown_03001580[];

static inline void BossSetState(s32 st, s32 idx)
{
    struct actor_self *self;
    gAirshipState = st;
    gUnknown_0300153C = 0;
    self = gAirship;
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
    self->palette = flag;
    SetActorAnim(self, 0);
}

void CreateAirship(s32 level)
{
    struct actor_self *t;
    struct actor_self **slot;

    gUnknown_03001564 = level;
    if (GetActorCheckpoint() == 0)
        gUnknown_0300157C = 0;
    gUnknown_03001528 = gAirshipPicture[0];
    gUnknown_0300152C = gAirshipPicture[1];
    slot = &gAirship;
    t = AllocActor(0x1c);
    InitAnimPart(t, gStaticData_0817C3E4, gUnknown_03001580, 1);
    *slot = t;
    BossSetState(0, 0);
    LoadAirshipGraphics();
    gUnknown_03001524 = 0;
}
