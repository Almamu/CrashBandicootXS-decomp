#include "platform.hpp"

extern "C" {
#include "level.h"
#include "globals.h"
}

/* Platform::Create (#664, include/platform.hpp), ROM 0x0801A878-0x0801AB34:
 * CreatePlatform, the level spawners' constructor (src/level/). */

/* The constructor, inlined into `new Platform` here; platform.cpp has its
 * out-of-line copy (InitPlatform). */
inline Platform::Platform()
{
    ClearVulnerable();
}

/* A platform at (x, y) (pixels), its parameter record entry `index`. The
 * type is the record's, or forced by the spawn kind; a moving one (type
 * 1) gets a PlatformMover over the record's distances, and types 5, 6 and
 * 7 one of their own kind (the Neo Cortex fight's type 6 the fight's own,
 * CortexBossPlatformMover, src/bosses/cortex.cpp). The bonus platform (3) takes its exit
 * facing from the record. The platform goes into the touchable list, with
 * animation `kind` of sprite bank 0x1D4. */
Platform *Platform::Create(u16 id, u16 x, u16 y, u16 index, s32 kind)
{
    Platform *obj = new Platform;
    struct spawn_rec *rec;
    u32 type;
    PlatformMover *m;

    obj->id = id;
    obj->x = INT_TO_Q8(x);
    obj->y = INT_TO_Q8(y);
    {
        const struct level_entity_list *lvl = gEntityFlags->list;
        const u16 *offsets = lvl->paramOffsets;

        rec = (struct spawn_rec *)((const u8 *)lvl->params + offsets[index]);
        type = rec->type;
    }
    switch (kind) {
    case 4:
        type = 2;
        break;
    case 5:
        type = 3;
        break;
    case 8:
        type = 7;
        break;
    case 3:
    case 9:
    case 10:
    case 11:
    case 12:
        type = 4;
        break;
    case 6:
        type = 6;
        break;
    }
    switch (type) {
    case 0:
        obj->type = 0;
        break;
    case 1:
        obj->type = 1;
        m = new PlatformMover(rec->distX, rec->distY, rec->dirX != 0, rec->dirY != 0, 1);
        obj->mover = m;
        m->Attach(obj);
        if (rec->flag)
            obj->SetAlwaysActive();
        break;
    case 2:
        obj->type = 2;
        break;
    case 3:
        obj->type = 3;
        obj->SetExitMirror(rec->flags & 1);
        break;
    case 4:
        obj->type = 4;
        break;
    case 5:
        obj->type = 5;
        m = new PlatformMover(0, 0, false, false, 5);
        obj->mover = m;
        m->Attach(obj);
        break;
    case 6:
        obj->type = 6;
        if (GetBossIndex(gLevelState) != BOSS_NEO_CORTEX)
            m = new PlatformMover(0, 0, false, false, 6);
        else
            m = new CortexBossPlatformMover;
        obj->mover = m;
        m->Attach(obj);
        break;
    case 7:
        obj->type = 7;
        m = new PlatformMover(0, 0, false, false, 7);
        obj->mover = m;
        m->Attach(obj);
        break;
    }
    TouchableList()->Add(obj);
    obj->anim = (struct anim_table *)(SPRITE_BANK_BASE + 0x1D4);
    obj->tag = kind;
    obj->ResetFrameTimer();
    obj->ResetFrameIndex();
    obj->SetAnimDone(0);
    obj->mirrorFlags.mirrorX = 0;
    obj->mirrorFlags.mirrorY = 0;
    {
        struct anim_rec *r = obj->anim->records;
        u32 slot;

        r += obj->tag;
        slot = GetPaletteSlot(gPaletteCache, r->paletteId);

        obj->palette = slot;
    }
    return obj;
}
