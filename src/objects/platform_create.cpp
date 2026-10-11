#include "platform.hpp"
#include "level_state.hpp"
#include "spawners.hpp"

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
    const struct entity_params *rec;
    u32 type;
    PlatformMover *m;

    obj->id = id;
    obj->x = INT_TO_Q8(x);
    obj->y = INT_TO_Q8(y);
    rec = EntityParams(index);
    type = rec->p.platform.type;
    switch (kind) {
    case PLATFORM_KIND_EXIT_PAD:
        type = PLATFORM_TYPE_WARP_EXIT;
        break;
    case PLATFORM_KIND_BONUS:
        type = PLATFORM_TYPE_WARP_BONUS;
        break;
    case PLATFORM_KIND_ROCK:
        type = PLATFORM_TYPE_ROCK;
        break;
    case PLATFORM_KIND_GEM_YELLOW:
    case PLATFORM_KIND_GEM_BLUE:
    case PLATFORM_KIND_GEM_GREEN:
    case PLATFORM_KIND_GEM_RED:
    case PLATFORM_KIND_GEM_PATH_DONE:
        type = PLATFORM_TYPE_WARP_GEM_PATH;
        break;
    case PLATFORM_KIND_FLIP:
        type = PLATFORM_TYPE_FLIP;
        break;
    }
    switch (type) {
    case PLATFORM_TYPE_STILL:
        obj->type = PLATFORM_TYPE_STILL;
        break;
    case PLATFORM_TYPE_MOVING:
        obj->type = PLATFORM_TYPE_MOVING;
        m = new PlatformMover(rec->p.platform.distX, rec->p.platform.distY,
                              rec->p.platform.dirX != 0, rec->p.platform.dirY != 0,
                              PLATFORM_TYPE_MOVING);
        obj->mover = m;
        m->Attach(obj);
        if (rec->p.platform.flag)
            obj->SetAlwaysActive();
        break;
    case PLATFORM_TYPE_WARP_EXIT:
        obj->type = PLATFORM_TYPE_WARP_EXIT;
        break;
    case PLATFORM_TYPE_WARP_BONUS:
        obj->type = PLATFORM_TYPE_WARP_BONUS;
        obj->SetExitMirror(rec->flags & ENTITY_PARAMS_FLAG_EXIT_MIRROR);
        break;
    case PLATFORM_TYPE_WARP_GEM_PATH:
        obj->type = PLATFORM_TYPE_WARP_GEM_PATH;
        break;
    case PLATFORM_TYPE_FALLING:
        obj->type = PLATFORM_TYPE_FALLING;
        m = new PlatformMover(0, 0, false, false, PLATFORM_TYPE_FALLING);
        obj->mover = m;
        m->Attach(obj);
        break;
    case PLATFORM_TYPE_FLIP:
        obj->type = PLATFORM_TYPE_FLIP;
        if (gLevelState->GetBossIndex() != BOSS_NEO_CORTEX)
            m = new PlatformMover(0, 0, false, false, PLATFORM_TYPE_FLIP);
        else
            m = new CortexBossPlatformMover;
        obj->mover = m;
        m->Attach(obj);
        break;
    case PLATFORM_TYPE_ROCK:
        obj->type = PLATFORM_TYPE_ROCK;
        m = new PlatformMover(0, 0, false, false, PLATFORM_TYPE_ROCK);
        obj->mover = m;
        m->Attach(obj);
        break;
    }
    TouchableList()->Add(obj);
    obj->bank = gSpriteBankSet->Bank(39);
    obj->tag = kind;
    obj->ResetFrameTimer();
    obj->ResetFrameIndex();
    obj->SetAnimDone(0);
    obj->mirrorFlags.mirrorX = 0;
    obj->mirrorFlags.mirrorY = 0;
    {
        const struct sprite_anim *r = obj->bank->anims;
        u32 slot;

        r += obj->tag;
        slot = gPaletteCache->GetSlot(r->paletteId);

        obj->palette = slot;
    }
    return obj;
}
