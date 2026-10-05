#include "core.h"
#include "actor.h"

/* Built with old_agbcc - see docs/matching/game-loop-old-agbcc.md. */

struct spawn_part
{
    struct actor base;          // 0x00
    u8 unk_1C[4];
    void *anim;                 // 0x20
    u8 unk_24[5];
    u8 frameNibble:4;           // 0x29
    u8 unk_29_4:4;
    u8 unk_2A[3];
    u8 tag;                     // 0x2D
    u8 unk_2E[0x1B];
    u8 unk_49;                  // 0x49
    u8 unk_4A;                  // 0x4A
    u8 unk_4B;                  // 0x4B
};

struct level_state
{
    u8 unk_00[0x8C];
    u8 timeTrial;                  // 0x8C
};

extern struct level_state *gLevelState;
extern void ***gUnknown_030012D0;

extern struct spawn_part *CreateExtraLife(u16 arg0, u16 arg1, u16 arg2, s32 arg3);
extern void ResetSpriteFrameTimer(struct spawn_part *part);
extern void ResetSpriteFrameIndex(struct spawn_part *part);
extern void SetSpriteAnimDone(struct spawn_part *part, u8 val);
extern s32 GetSpriteAnimPaletteSlot(struct spawn_part *part);
extern void SendExtraLifeToHud(struct spawn_part *part);

static inline void SetPartTag(struct spawn_part *part, s32 tag)
{
    part->tag = tag;
}

/* Spawns a CreateExtraLife part at (x, y) unless gLevelState's +0x8c
 * flag is set (then returns NULL). Tags it with p3/p5/the flag, gives it
 * animation slot 0x8d and tag 0xa, and runs SendExtraLifeToHud on it when
 * `flag6` is set. */
struct spawn_part *DropExtraLife(void *unused0, u32 x, u32 y, u32 p3, u32 p5, u32 flag6)
{
    /* The ROM reads the flag as the stack word's low byte (ldrb). */
    u8 f = *(u8 *)&flag6;
    struct spawn_part *part = NULL;
    u8 state = gLevelState->timeTrial;

    if (state == 0)
    {
        part = CreateExtraLife(0xffff, x, y, 0);
        part->base.flags |= 0x10;
        part->unk_49 = p3;
        part->unk_4A = p5;
        part->unk_4B = state;
        part->anim = (u8 *)**gUnknown_030012D0 + 0x8d * 4;
        SetPartTag(part, 0xa);
        ResetSpriteFrameTimer(part);
        ResetSpriteFrameIndex(part);
        SetSpriteAnimDone(part, 0);
        part->frameNibble = GetSpriteAnimPaletteSlot(part);
        if (f)
            SendExtraLifeToHud(part);
    }
    return part;
}
