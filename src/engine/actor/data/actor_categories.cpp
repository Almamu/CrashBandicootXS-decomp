#include "game/vehicles/polar/yeti.hpp"
#include "game/vehicles/jetpack/airship.hpp"
#include "game/bosses/hovercraft.hpp"
#include "game/vehicles/vehicle.hpp"
#include "engine/actor/actor_category.hpp"
#include "engine/actor/actor_palette_cycle.hpp"

extern "C" {
#include "core.h"
#include "engine/actor/actor_anim.h"
#include "engine/actor/actor.h"
#include "game/bosses/bosses.h"
}

using Engine::Actor::ActorCategory;
using Engine::Actor::ActorPaletteCycle;
using Game::Bosses::Hovercraft;
using Game::Vehicles::Polar::PolarPlayer;
using Game::Vehicles::Polar::Yeti;
using Game::Vehicles::Jetpack::JetpackPlayer;
using Game::Vehicles::Jetpack::Airship;

/*
 * ROM 0x08175558-0x08175760: the actor category descriptors and the
 * per-type category vtables (include/engine/actor/actor_anim.h, docs/graphics.md "An
 * actor \"vtable\" system"). Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern const u8 gCategoryFamily0CellAnim[];
extern const u8 gPolarSpriteSheet[];
extern const u8 gCategoryFamily1CellAnim[];
extern const u8 gCategory3BgPicture[];
extern const u8 gJetpackSpriteSheet[];
extern const u8 gCategory4BgPicture[];
extern const u8 gCategory5BgPicture[];
extern const u16 gPolarCategoryPalette[];
extern const u16 gAirshipCategoryPalette[];
extern const u16 gHovercraftCategoryPalette[];

/* One descriptor per actor category (type 0: categories 0-2, type 1: 3-5,
 * type 2: 6 in the ROM), generated from levels.json's "categories"
 * (`levels.py categories`, docs/levels.md "Actor categories"), with the
 * spawn lists that aren't the ROM's before them. data.s used to split this
 * table at 0x08175564 and 0x08175584; nothing referenced those labels. */
#include "actor_categories.inc"

/* The three per-type category vtables, `ActorCategory::vtable =
 * &ActorCategory::vtables[type]` in ActorCategory::Select
 * (actor_category_select.cpp): include/engine/actor/actor_anim.h names the slots;
 * the two numbers are the spawn and skip distances. */
const struct category_vtable ActorCategory::vtables[CATEGORY_TYPE_COUNT] = {
    /* CATEGORY_TYPE_POLAR */ {
        PolarPlayer::Create,
        PolarPlayer::SpawnActor,
        Yeti::Create,
        Yeti::Update,
        Yeti::UpdateBg2,
        Yeti::Destroy,
        Yeti::LoadGraphics,
        -0x11,
        -0x2f,
        PolarPlayer::IsTouchingPlayer,
        PolarPlayer::ReachCourseEnd,
        PolarPlayer::ReloadPlayerTiles,
        PolarPlayer::IsPlayerPauseLocked,
    },
    /* CATEGORY_TYPE_JETPACK */
    {
        JetpackPlayer::Create,
        JetpackPlayer::SpawnActor,
        Airship::Create,
        Airship::Update,
        Airship::UpdateBg2,
        Airship::Destroy,
        Airship::LoadGraphics,
        0xa9,
        0x1c,
        JetpackPlayer::IsTouchingPlayer,
        JetpackPlayer::ReachCourseEnd,
        JetpackPlayer::ReloadPlayerTiles,
        JetpackPlayer::IsPlayerPauseLocked,
    },
    /* CATEGORY_TYPE_HOVERCRAFT */
    {
        JetpackPlayer::Create,
        JetpackPlayer::SpawnActor,
        Hovercraft::Create,
        Hovercraft::Update,
        Hovercraft::UpdateBg2,
        Hovercraft::Destroy,
        Hovercraft::LoadGraphics,
        0xa9,
        0x1c,
        JetpackPlayer::IsTouchingPlayer,
        JetpackPlayer::ReachCourseEnd,
        JetpackPlayer::ReloadPlayerTiles,
        JetpackPlayer::IsPlayerPauseLocked,
    },
};
