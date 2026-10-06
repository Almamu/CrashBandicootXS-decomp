#include "core.h"
#include "actor.h"
#include "pickups.h"
#include "objects.h"
#include "level.h"

/* Built with old_agbcc - see docs/matching/game-loop-old-agbcc.md. */

extern struct level_state *gLevelState;
extern void ***gSpriteBankSet;

static inline void SetPartTag(struct orbit_part *part, s32 tag)
{
    part->tag = tag;
}

/* Spawns a CreateExtraLife part at (x, y) unless gLevelState's +0x8c
 * flag is set (then returns NULL). Tags it with p3/p5/the flag, gives it
 * animation slot 0x8d and tag 0xa, and runs SendExtraLifeToHud on it when
 * `flag6` is set. */
struct orbit_part *DropExtraLife(void *unused0, u32 x, u32 y, u32 p3, u32 p5, u32 flag6)
{
    /* The ROM reads the flag as the stack word's low byte (ldrb). */
    u8 f = *(u8 *)&flag6;
    struct orbit_part *part = NULL;
    u8 state = gLevelState->timeTrial;

    if (state == 0)
    {
        part = CreateExtraLife(0xffff, x, y, 0);
        part->base.flags |= 0x10;
        part->counter = p3;
        part->mode = p5;
        part->phase = state;
        part->bank = (struct act_anim_bank *)((u8 *)**gSpriteBankSet + 0x8d * 4);
        SetPartTag(part, 0xa);
        ResetSpriteFrameTimer(part);
        ResetSpriteFrameIndex(part);
        SetSpriteAnimDone(part, 0);
        part->slotNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
        if (f)
            SendExtraLifeToHud(part);
    }
    return part;
}
