#include "crate.hpp"
#include "crate_list.hpp"

extern "C" {
#include "match.h"
#include "objects.h"
#include "memory.h"
#include "level.h"
#include "globals.h"
}

/* Crate::Create (#664, include/crate.hpp): CreateCrate, the constructor
 * the level spawners call with each crate kind (src/level/spawn_crates.c,
 * spawn_objects.c). */

/* The room's entity parameter records (struct level_entity_list). */
#define PLACEMENT_LEVEL (gEntityFlags->list)
#define PLACEMENT(i)                                                           \
    ((struct crate_placement *)((u8 *)PLACEMENT_LEVEL->params + PLACEMENT_LEVEL->paramOffsets[i]))
/* PLACEMENT as an inline: its return value is copied, which gives the
 * ROM's pointer copies (the 0xB pre-check, the time-trial block and the
 * slot crate's case: `add r2,r0,#0` / `add r3,r1,#0` / `mov sl,r6`). */
static inline struct crate_placement *Placement(u16 i)
{
    return PLACEMENT(i);
}

/* The constructor, inlined into `new Crate` here; crate.cpp has its
 * out-of-line copy (InitCrate). */
inline Crate::Crate()
{
    groupAllocated = 0;
    Reset();
}

/* A crate of kind `type` (CRATE_KIND_*) at (x, y) (pixels), its
 * placement record entry `slot`. An activated life crate (its entity id
 * set) is a basic one, and an activated iron switch crate an iron one.
 * With crate assistance (enough deaths, not in a time trial) a "?" or
 * slot crate turns into an Aku Aku, checkpoint or life crate as its
 * record says. A crate with a time-trial kind keeps it in `trialKind`,
 * and in a time trial is created as that kind. Each kind then gets its
 * animation and parameters from the record; an activated "?" or slot
 * crate of a checkpoint record, and an activated checkpoint crate, show
 * the opened checkpoint, and an activated outline crate is solid. The
 * crate goes into the crate list. */
Crate *Crate::Create(u16 id, u16 x, u16 y, u16 slot, u8 type)
{
    Crate *self = new Crate;
    s32 special;
    s32 flagged;

    self->id = id;
    if (type == CRATE_KIND_LIFE && id != 0xffff && (u8)IsEntityIdActivated(gEntityFlags, id))
        type = CRATE_KIND_BASIC;
    if (gLevelState->timeTrial == 0 &&
        GetDeaths(gLevelState) >= GetCrateAssistDeaths(gLevelState)) {
        if (type == CRATE_KIND_MYSTERY) {
            struct crate_placement *rec = Placement(slot);

            if (rec->flags & CRATE_PLACEMENT_FLAG_ASSIST_AKU_AKU)
                type = CRATE_KIND_AKU_AKU;
            else if (rec->flags & CRATE_PLACEMENT_FLAG_ASSIST_CHECKPOINT)
                type = CRATE_KIND_CHECKPOINT;
            else if (rec->options & CRATE_PLACEMENT_OPTION_ASSIST_LIFE)
                type = CRATE_KIND_LIFE;
        } else if (type == CRATE_KIND_SLOT) {
            struct crate_placement *rec = PLACEMENT(slot);

            if (rec->flags & CRATE_PLACEMENT_FLAG_ASSIST_AKU_AKU)
                type = CRATE_KIND_AKU_AKU;
            else if (rec->flags & CRATE_PLACEMENT_FLAG_ASSIST_CHECKPOINT)
                type = CRATE_KIND_CHECKPOINT;
            else if (rec->options & CRATE_PLACEMENT_OPTION_ASSIST_LIFE)
                type = CRATE_KIND_LIFE;
        }
    }
    special = 0;
    self->anim = (struct anim_table *)(SPRITE_BANK_BASE + 0x174);
    switch (type) {
    case CRATE_KIND_CHECKPOINT:
    case CRATE_KIND_LIFE:
    case CRATE_KIND_MYSTERY:
    case CRATE_KIND_BOUNCY_WUMPA:
    case CRATE_KIND_SLOT:
        special = 1;
        break;
    case CRATE_KIND_IRON_SWITCH:
        if ((u8)IsEntityIdActivated(gEntityFlags, id))
            type = CRATE_KIND_IRON;
        break;
    }
    flagged = 0;
    {
        struct crate_placement *rec = Placement(slot);

        if (special || (rec->flags & CRATE_PLACEMENT_FLAG_TRIAL_KIND)) {
            flagged = 1;
            if ((u16)rec->trialKind == ENTITY_NITRO_SWITCH_CRATE)
                self->trialKind = ENTITY_BASIC_CRATE;
            else
                self->trialKind = rec->trialKind;
            if (gLevelState->timeTrial)
                type = self->trialKind - ENTITY_BASIC_CRATE;
        }
    }
    switch (type) {
    case CRATE_KIND_BASIC:
        self->SetTag(0x1f);
        break;
    case CRATE_KIND_CHECKPOINT:
        self->paramA = (u32)(PLACEMENT(slot)->flags << 25) >> 31;
        self->SetTag(0x1a);
        break;
    case CRATE_KIND_AKU_AKU:
        self->SetTag(0x17);
        break;
    case CRATE_KIND_IRON_SWITCH:
        {
            struct crate_placement *rec = PLACEMENT(slot);

            self->SetTag(3);
            self->paramA = rec->param6;
            self->paramB = rec->param7;
            self->fallSpeed = rec->u08.stepDelay;
        }
        break;
    case CRATE_KIND_ARROW:
        self->SetTag(0x18);
        break;
    case CRATE_KIND_OUTLINE:
        {
            struct crate_placement *rec = PLACEMENT(slot);

            self->SetTag(0x15);
            self->paramA = rec->param6;
            self->paramB = rec->param7;
            self->solidKind = rec->u08.solidKind;
        }
        break;
    case CRATE_KIND_NITRO_SWITCH:
        self->SetTag(4);
        break;
    case CRATE_KIND_IRON:
        self->SetTag(0x20);
        break;
    case CRATE_KIND_IRON_ARROW:
        self->SetTag(2);
        break;
    case CRATE_KIND_LIFE:
        self->SetTag(0x1c);
        if (!flagged) {
            self->trialKind = ENTITY_BASIC_CRATE;
            if (gLevelState->timeTrial)
                type = CRATE_KIND_BASIC;
        }
        break;
    case CRATE_KIND_NITRO:
        self->SetTag(5);
        break;
    case CRATE_KIND_MYSTERY:
        {
            struct crate_placement *rec = PLACEMENT(slot);
            u8 zero;

            /* constant-init: the ROM loads the 0 before the tag's address,
             * as for the other tags; a plain 0 is loaded after it */
            MATCH_CONST(zero, 0);
            self->SetTag(zero);
            self->paramB = rec->param6;
        }
        break;
    case CRATE_KIND_BOUNCY_WUMPA:
        self->bounceTimer = -0x2a;
        self->SetTag(0x19);
        break;
    case CRATE_KIND_REINFORCED:
        self->SetTag(6);
        break;
    case CRATE_KIND_TNT:
        self->SetTag(0x11);
        break;
    case CRATE_KIND_SLOT:
        {
            struct crate_placement *rec = Placement(slot);

            {
                const struct sprite_anim *anim = &self->bank->anims[8];

                GetPaletteSlot(gPaletteCache, anim->paletteId);
            }
            self->slotState &= CRATE_SLOT_CLEAR_STAGE;
            self->slotState &= CRATE_SLOT_CLEAR_PHASE;
            self->SetTag(7);
            {
                u32 idx = (u32)(self->slotState & CRATE_SLOT_SPINS_MASK) >> CRATE_SLOT_SPINS_SHIFT;

                self->timer = gSlotCrateTimers[idx];
            }
            self->paramB = rec->param6;
            self->paramA = 0;
            if (rec->options & 2)
                self->paramA = 1;
            if (rec->options & 4)
                self->paramA |= 2;
            if (rec->options & 8)
                self->paramA |= 4;
        }
        break;
    case CRATE_KIND_TIME_1:
        self->SetTag(0xe);
        break;
    case CRATE_KIND_TIME_2:
        self->SetTag(0xf);
        break;
    case CRATE_KIND_TIME_3:
        self->SetTag(0x10);
        break;
    }
    self->mirrorBits.flipX = 0;
    self->mirrorBits.flipY = 0;
    self->palette = self->GetAnimPaletteSlot();
    self->x = INT_TO_Q8(x);
    self->y = INT_TO_Q8(y);
    if ((u8)IsEntityIdActivated(gEntityFlags, id) &&
        (type == CRATE_KIND_MYSTERY || type == CRATE_KIND_SLOT) &&
        (PLACEMENT(slot)->flags & CRATE_PLACEMENT_FLAG_ASSIST_CHECKPOINT))
        type = CRATE_KIND_CHECKPOINT;
    if (type == CRATE_KIND_CHECKPOINT && id != 0xffff &&
        (u8)IsEntityIdActivated(gEntityFlags, id)) {
        self->SetTag(0x1b);
        self->frame = self->bank->anims[self->tag].frameCount - 1;
        self->state = (self->state & CRATE_STATE_BUSY) | type;
    }
    self->kind = type;
    if (type == CRATE_KIND_OUTLINE && (u8)IsEntityIdActivated(gEntityFlags, id))
        self->SolidifyOutline();
    Crates()->Add(self);
    return self;
}
