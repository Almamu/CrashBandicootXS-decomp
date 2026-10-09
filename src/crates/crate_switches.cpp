#include "crate.hpp"
#include "spawners.hpp"
#include "crate_list.hpp"
#include "part_list.hpp"
#include "pickups.hpp"
#include "player.hpp"
#include "hud.hpp"
#include "audio.hpp"
#include "level_state.hpp"

extern "C" {
#include "match.h"
#include "pickups.h"
#include "util.h"
#include "player.h"
#include "memory.h"
#include "level.h"
#include "globals.h"
#include "entity_bits.h"
#include "sprite_bank.h"
}

/* The crate list's update pass (UpdateCrates), the nitro detonation, the
 * nitro and iron switches, the outline crates and BreakCratesInArea
 * (#664, include/crate.hpp, part 7g), ROM 0x0800F1B8-0x0800F798: the
 * middle of crate_break.cpp's "physics/collision" cluster, split out of
 * it (#768). Built with old_agbcp, as the C was with old_agbcc. */

/* The player's `busy` latch set: the player is loaded before the 1, as
 * in SetCrateBusy. */
static inline void SetPlayerBusy(void)
{
    Player *p = gPlayer;
    u8 one = 1;

    p->busy = one;
}

/* The crate list's update pass (run_room.cpp): DetonateNitroCrates, then
 * each crate of the list is updated, and a crate that is gone is removed
 * and deleted; all of it again while the list keeps changing. */
void UpdateCrates(void)
{
    s32 i;

    DetonateNitroCrates();
    do {
        gCrateListChanged = 0;
        for (i = 0; i < gCrateList->count; i++) {
            Entity *o = gCrateList->slots[i];

            if (o->GetClassId() == 3) {
                if (o->f.flags & 1) {
                    Crates()->RemoveAt(i);
                    delete o;
                    i--;
                } else {
                    o->Update();
                }
            }
        }
    } while (gCrateListChanged);
}

/* Every idle nitro crate of the crate list explodes. */
void DetonateNitroCrates(void)
{
    s32 i = 0;

    if (i < Crates()->count) {
        do {
            Crate *o = Crates()->slots[i];

            if (o->GetClassId() == 3 && o->kind == CRATE_KIND_NITRO) {
                if ((o->state & CRATE_STATE_MASK) == 0)
                    o->Explode(0);
            }
            i++;
        } while (i < Crates()->count);
    }
}

/* The nitro switch crate, once: its pressed animation and palette, every
 * nitro crate in the room explodes, the crate counter shows, and the
 * switch counts as pressed. */
void Crate::ActivateNitroSwitch()
{
    if (pressed == 0) {
        s32 one;
        const struct sprite_anim *anims;
        const struct sprite_anim *a;
        u32 slot;

        state |= CRATE_STATE_BUSY;
        {
            Player *player = gPlayer;
            one = 1;
            player->busy = one;
        }
        SetTag(0x23);
        anims = bank->anims;
        a = &anims[tag];
        slot = gPaletteCache->GetSlot(a->paletteId);
        palette = slot;
        DetonateNitroCrates();
        gHud->ShowCrates();
        gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
        pressed = 1;
        gLevelState->PressSwitchCrate();
    }
}

/* The iron switch crate, once: its pressed animation and palette, its
 * entity marked activated, and the group of the room's idle outline
 * crates with its group id (paramA) collected (up to 32; each marked
 * activated), which SolidifyOutlineCrates then turns solid one step at a
 * time. */
void Crate::ActivateIronSwitch()
{
    Crate *found[32];
    s32 n = 0;
    s32 i;

    if (group == PHYS_NO_GROUP)
        return;
    if (group != 0)
        return;

    f.flags |= 0x10;
    Crates()->LinkActive(this);
    state |= CRATE_STATE_BUSY;
    SetPlayerBusy();
    SetTag(0x22);
    {
        const struct sprite_anim *anims = bank->anims;
        const struct sprite_anim *a = &anims[tag];
        u32 slot = gPaletteCache->GetSlot(a->paletteId);

        palette = slot;
    }
    MarkEntityIdActivated(gEntityFlags, id);

    i = 0;
    if (i < Crates()->count) {
        do {
            Crate *o = Crates()->slots[i];

            if (o->GetClassId() == 3 && (o->state & CRATE_STATE_MASK) == 0) {
                if (o->kind == CRATE_KIND_OUTLINE && o->paramA == paramA) {
                    found[n] = o;
                    n++;
                    n &= 0x1f;
                    MarkEntityIdActivated(gEntityFlags, o->id);
                }
            }
            i++;
        } while (i < Crates()->count);
    }

    if (n != 0) {
        struct crate_group *g = (struct crate_group *)OperatorNewArray((n + 1) * 4);

        groupAllocated = 1;
        g->count = n;
        /* Stored through the block as an array of words (`g[i + 1]`):
         * written as `g->items[i]`, the loop's code differs from the ROM's. */
        for (i = 0; i < n; i++)
            ((Crate **)g)[i + 1] = found[i];
        group = g;
    } else {
        group = PHYS_NO_GROUP;
    }
    paramA = 0;
    timer = fallSpeed;
}

/* One step of an activated iron switch crate (when its timer runs out):
 * the step counter (paramA) goes up, and the outline crates of its group
 * whose step (paramB) it has reached turn solid, with one sound. After
 * the last step the group is freed and the switch becomes an iron crate. */
void Crate::SolidifyOutlines()
{
    if (timer != 0)
        return;

    if (++paramA >= paramB) {
        struct crate_group *g = group;

        if (PHYS_HAS_GROUP(g)) {
            if (g != 0)
                OperatorDeleteArray(g);
            groupAllocated = 0;
        }
        group = PHYS_NO_GROUP;
        {
            u8 k = CRATE_KIND_IRON;
            kind = k;
        }
    } else {
        struct crate_group *g = group;

        if (PHYS_HAS_GROUP(g)) {
            s32 i;
            s32 n = g->count;
            Crate **items = g->items;
            s32 played = FALSE;

            for (i = 0; i < n; i++) {
                Crate *o = items[i];

                if (o->kind == CRATE_KIND_OUTLINE && paramA >= o->paramB) {
                    o->SolidifyOutline();
                    if (!played) {
                        gAudioContext->PlaySfx(SFX_OUTLINE_CRATES_SOLIDIFY, 0x100);
                        played = TRUE;
                    }
                }
            }
        }
        timer = fallSpeed;
    }
}

/* An outline crate turns into the crate it outlines (`solidKind`, an
 * entity type): its kind, its animation and its palette. */
void Crate::SolidifyOutline()
{
    kind = solidKind - ENTITY_BASIC_CRATE;
    switch (kind) {
    case CRATE_KIND_BASIC:
        SetTag(0x1f);
        break;
    case CRATE_KIND_CHECKPOINT:
        SetTag(0x1a);
        break;
    case CRATE_KIND_AKU_AKU:
        SetTag(0x17);
        break;
    case CRATE_KIND_ARROW:
        SetTag(0x18);
        break;
    case CRATE_KIND_NITRO_SWITCH:
        SetTag(4);
        break;
    case CRATE_KIND_IRON:
        SetTag(0x20);
        break;
    case CRATE_KIND_IRON_ARROW:
        SetTag(2);
        break;
    case CRATE_KIND_NITRO:
        SetTag(5);
        break;
    case CRATE_KIND_BOUNCY_WUMPA:
        bounceTimer = -0x2a;
        SetTag(0x19);
        break;
    case CRATE_KIND_REINFORCED:
        SetTag(6);
        break;
    case CRATE_KIND_TNT:
        SetTag(0x11);
        break;
    case CRATE_KIND_TIME_1:
        SetTag(0xe);
        break;
    case CRATE_KIND_TIME_2:
        SetTag(0xf);
        break;
    case CRATE_KIND_TIME_3:
        SetTag(0x10);
        break;
    }
    palette = GetAnimPaletteSlot();
}

/* Every idle crate of the crate list within `dist` pixels (|dx| + |dy|)
 * of (x, y), and less than `height` pixels above or below it, explodes,
 * if it is explosive, or opens or breaks, if it is breakable (the
 * player's super body slam, action_ctrl_moves.cpp). */
void BreakCratesInArea(s32 x, s32 y, s32 dist, s32 height)
{
    s32 i = 0;

    if (i < gCrateList->count) {
        const u8 *commit = gCrateKindExplosive;

        do {
            Crate *o = gCrateList->slots[i];

            if (o->GetClassId() == 3) {
                s32 t1 = Q8_TO_INT(o->x) - x;
                s32 dx = ABS_BRANCHLESS(t1);
                s32 t2 = Q8_TO_INT(o->y) - y;
                s32 dy = ABS_BRANCHLESS(t2);

                if (dx + dy <= dist && dy < height && (o->state & CRATE_STATE_MASK) == 0) {
                    /* `kind + table`, as in BlastNearbyCrates */
                    if (*(u8 *)(o->kind + (u32)commit))
                        o->Explode(0);
                    else if (gCrateKindBreakable[o->kind]) {
                        if (o->kind == CRATE_KIND_CHECKPOINT)
                            o->OpenCheckpoint();
                        else
                            o->BreakInStack(0, 0, 0);
                    }
                }
            }
            i++;
        } while (i < gCrateList->count);
    }
}
