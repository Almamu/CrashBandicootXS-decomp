#include "core.h"
#include "actor_self.h"

/* Same boss-weapon "self"/tracker object family as actor_part20.c/
 * actor_part23.c - see actor_part20.c's header comment and
 * docs/matching/issue-58-0x08030334-actor.md.
 *
 * A large per-frame "advance this weapon-kind instance" driver: fires
 * a stride-4 trampoline (`gAirshipStateFuncs`, indexed by the
 * tracker's own state global `gAirshipState`) via `_call_via_r0`,
 * refreshes the palette-strip animation (`AnimateAirshipPalette`), and advances
 * `gAirshipStateTimer`'s frame counter. While the tracker's state is
 * nonzero: advances its own anim-frame accumulator (`+8`, by its part-
 * table's `+0x10` halfword) and, once `GetAnimFrameBaseOffset` crosses
 * the current keyframe-table entry's threshold, both re-arms the
 * accumulator against the *next* entry's own delta and sets the "loop"
 * flag (`+0x12`). Always recomputes the BG2 zoom scale/offset the same
 * way `SpawnAirship` (actor_part23e.c) does (`GetCellAnimDistance`/
 * `__divsi3`/`sub_8029E34`), and - only when the tracker's
 * accumulator (`+8`, `>>8`) actually crossed to a new keyframe-table
 * index this frame - re-blits its box via `DrawAirshipMap` and re-arms
 * the "apply now" latch (`gAirshipBg2PageFlip`).
 *
 * Matching notes: the zoom divide is a plain call to `__divsi3`
 * (not `/`, whose libcall the compiler would treat as not clobbering
 * memory - the ROM reloads `gAirshipDistance` after it), and the
 * re-blit tail reads the tracker through a fresh local (a separate
 * pseudo from the head's own `self`). */
extern struct actor_self *gAirship;
extern void *gAirshipStateFuncs[];
extern s32 gAirshipState;
extern s32 _call_via_r0(void *fn);
extern void AnimateAirshipPalette(void);
extern s32 gAirshipStateTimer;
extern s32 GetAnimFrameBaseOffset(void *self);
extern s32 gAirshipDistance;
extern s32 GetCellAnimDistance(void);
extern s32 gAirshipZ;
extern s32 __divsi3(s32 arg0, s32 arg1);
extern s32 gAirshipScreenX;
extern s32 gAirshipX;
extern s32 gAirshipScreenY;
extern s32 gAirshipY;
extern void sub_8029E34(s32 arg0);
extern void DrawAirshipMap(u16 *src);
extern u8 gAirshipBg2PageFlip;

void UpdateAirship(void)
{
    s32 prev = gAirship->animTime >> 8;
    struct actor_self *self;

    _call_via_r0(gAirshipStateFuncs[gAirshipState]);
    AnimateAirshipPalette();
    gAirshipStateTimer++;
    if (gAirshipState != 0) {
        s32 scale;

        self = gAirship;
        self->animTime += (s16)self->animTimer;
        self->animDone = 0;
        if (GetAnimFrameBaseOffset(self) >= self->anims[self->animIndex].loopThreshold) {
            self->animTime -= (self->anims[self->animIndex].loopThreshold - self->anims[self->animIndex].loopBase) << 8;
            self->animDone = 1;
        }
        gAirshipDistance = gAirshipZ - (GetCellAnimDistance() << 8);
        scale = __divsi3(0x1C00000, gAirshipDistance);
        gAirshipScreenX = (gAirshipX * scale) >> 12;
        gAirshipScreenY = (scale * gAirshipY) >> 12;
        sub_8029E34(gAirshipDistance);
        {
            struct actor_self *cur = gAirship;
            s32 t = cur->animTime >> 8;
            if (prev != t) {
                DrawAirshipMap((u16 *)cur->frameOffsets[cur->anims[cur->animIndex].frameIndex + t]);
                gAirshipBg2PageFlip = 1;
            }
        }
    }
}
