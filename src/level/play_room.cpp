#include "player.hpp"
#include "action_ctrl.hpp"
#include "player_ctrl.hpp"
#include "input_ctrl.hpp"
#include "crate_list.hpp"
#include "bg_layer.hpp"

extern "C" {
#include "core.h"
#include "actor.h"
#include "level_data.h"
#include "crates.h"
#include "gfx.h"
#include "objects.h"
#include "memory.h"
#include "level.h"
#include "globals.h"
}

/* Level-start dispatcher, called once from `UpdateGameFrame` when the
 * level object's own `+0xdc->+8` state field is `2`. Allocates the whole
 * per-level object set (the part lists `gUpdateOnlyPartList`,
 * `gTouchableList`, `gCollidableList`, `gDecorationList` and
 * `gForegroundList`, the crate list `gCrateList`, the camera), takes the
 * level layers singleton (`LevelLayers::Get`), builds the player
 * (`gPlayer`) at the checkpoint, and gives it the room kind's controller
 * (`self->cat->kind`): the action controller on foot, the swim controller
 * (`gPlayerCtrl`) underwater, the input controller on the hover levels.
 * Then it runs the room (`RunRoom`) and deletes everything again.
 *
 * C++ since the #664 cleanup, built with old_agbcp (agbcc as C, held by
 * 14 pins, a clobber and the `widget` vtable struct; agbcp orders the
 * flag byte's load before the constant). The locals are the ROM's: each
 * controller case reads `gPlayer` once into `pl` after the sprite bank,
 * and the constant stores (`tag`, `ctrlMode` 3) go through a `u8` local
 * so the constant is loaded before the field's address. */
s32 PlayRoom(struct level_progress *self)
{
    s32 mode;
    s32 result;

    CreateEntitySpawner();
    ClearRoomExit();

    gUpdateOnlyPartList = (struct part_list *)new PartList(0x20);
    gTouchableList = (struct part_list *)new PartList(0xc0);
    gCrateList = (struct pool_manager *)new CrateList(0xc0);
    gCollidableList = (struct part_list *)new PartList(0x80);
    gDecorationList = (struct part_list *)new PartList(0x40);
    gForegroundList = (struct part_list *)new PartList(0x40);
    gCamera = (struct camera *)operator new(0x18);

    {
        struct level_layers *layers = (struct level_layers *)LevelLayers::Get();

        gLevelLayers = layers;
    }

    gPlayer = new Player(0xffff, 0, 0, 0);
    SetEntityPos((struct actor *)gPlayer, self->checkpointX, self->checkpointY);
    gPlayer->f.bytes.flags |= 0x10;
    gPlayer->mirrorFlags.mirrorX = self->checkpointFlags;

    mode = self->cat->kind;
    switch (mode) {
    case ROOM_KIND_ON_FOOT:
        {
            ActionCtrl *ctrl = new ActionCtrl;

            ctrl->SetAnimSet(&gActionCtrlMotionSet);
            gPlayer->ctrlMode = mode;
            {
                struct anim_table *bank = (struct anim_table *)SPRITE_BANK_BASE;
                Player *pl = gPlayer;

                pl->anim = bank;
                pl->mover = ctrl;
                ctrl->Attach(pl);
            }
            break;
        }
    case ROOM_KIND_UNDERWATER:
        {
            gPlayerCtrl = new PlayerCtrl;
            ((PlayerCtrl *)gPlayerCtrl)->SetAnimSet(&gPlayerCtrlMotionSet);
            gPlayer->ctrlMode = mode;
            {
                struct anim_table *bank = (struct anim_table *)(SPRITE_BANK_BASE + 0xc);
                Player *pl = gPlayer;

                pl->anim = bank;
                {
                    u8 anim = 0x1f;

                    pl->tag = anim;
                }
                pl->ResetFrameTimer();
                pl->ResetFrameIndex();
                pl->SetAnimDone(0);
            }
            {
                Player *pl = gPlayer;
                PlayerCtrl *ctrl = (PlayerCtrl *)gPlayerCtrl;

                pl->mover = ctrl;
                ctrl->Attach(pl);
            }
            break;
        }
    case ROOM_KIND_HOVER:
        {
            InputCtrl *ctrl = new InputCtrl;

            ctrl->SetAnimSet(&gInputCtrlMotionSet);
            {
                Player *pl = gPlayer;
                u8 hover = 3;

                pl->ctrlMode = hover;
            }
            {
                struct anim_table *bank = (struct anim_table *)(SPRITE_BANK_BASE + 0x18);
                Player *pl = gPlayer;

                pl->anim = bank;
                pl->mover = ctrl;
                ctrl->Attach(pl);
            }
            break;
        }
    }

    result = RunRoom(self);

    delete (LevelLayers *)gLevelLayers;
    operator delete(gCamera);
    delete gPlayer;
    delete (PartList *)gForegroundList;
    delete (PartList *)gDecorationList;
    delete (PartList *)gCollidableList;
    delete (CrateList *)gCrateList;
    delete (PartList *)gTouchableList;
    delete (PartList *)gUpdateOnlyPartList;

    DestroyEntitySpawner();

    return result;
}
