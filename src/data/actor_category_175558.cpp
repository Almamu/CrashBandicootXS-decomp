#include "yeti.hpp"
#include "airship.hpp"
#include "hovercraft.hpp"

extern "C" {
#include "core.h"
#include "actor_anim.h"
#include "actor.h"
#include "bosses.h"
#include "vehicle.h"
}

/*
 * ROM 0x08175558-0x08175760: the actor category descriptors and the
 * per-type category vtables (include/actor_anim.h, docs/graphics.md "An
 * actor \"vtable\" system"). Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern const u8 gCategoryFamily0CellAnim[];
extern const u8 gCategory0SpawnTable[];
extern const u8 gPolarSpriteSheet[];
extern const u8 gCategory1SpawnTable[];
extern const u8 gCategory2SpawnTable[];
extern const u8 gCategoryFamily1CellAnim[];
extern const u8 gCategory3BgPicture[];
extern const u8 gCategory3SpawnTable[];
extern const u8 gJetpackSpriteSheet[];
extern const u8 gCategory4BgPicture[];
extern const u8 gCategory4SpawnTable[];
extern const u8 gCategory5BgPicture[];
extern const u8 gCategory5SpawnTable[];
extern const u8 gCategory6SpawnTable[];
extern const u16 gPolarCategoryPalette[];
extern const u16 gAirshipCategoryPalette[];
extern const u16 gHovercraftCategoryPalette[];

/* One descriptor per actor category 0-6 (type 0: categories 0-2,
 * type 1: 3-5, type 2: 6). data.s used to split this table at
 * 0x08175564 and 0x08175584; nothing referenced those labels. */
const struct category_descriptor gActorCategories[CATEGORY_COUNT] = {
    /* CATEGORY_FROSTBITE_CAVERN */ {
        CATEGORY_TYPE_POLAR,
        (void *)gCategoryFamily0CellAnim,
        0x75b94,
        NULL,
        gPolarCategoryPalette,
        (struct sub_effect_record *)gCategory0SpawnTable,
        (struct anim_table_record *)gCategoryFamily0AnimTable,
        gPolarSpriteSheet,
        4,
        3,
        2,
        1,
        0,
    },
    /* CATEGORY_SNOW_CRASH */ {
        CATEGORY_TYPE_POLAR,
        (void *)gCategoryFamily0CellAnim,
        0x75b94,
        NULL,
        gPolarCategoryPalette,
        (struct sub_effect_record *)gCategory1SpawnTable,
        (struct anim_table_record *)gCategoryFamily0AnimTable,
        gPolarSpriteSheet,
        4,
        3,
        2,
        3,
        2,
    },
    /* CATEGORY_SNOW_JOB */ {
        CATEGORY_TYPE_POLAR,
        (void *)gCategoryFamily0CellAnim,
        0x75b94,
        NULL,
        gPolarCategoryPalette,
        (struct sub_effect_record *)gCategory2SpawnTable,
        (struct anim_table_record *)gCategoryFamily0AnimTable,
        gPolarSpriteSheet,
        4,
        3,
        2,
        5,
        4,
    },
    /* CATEGORY_ROCKET_RACKET */ {
        CATEGORY_TYPE_JETPACK,
        (void *)gCategoryFamily1CellAnim,
        0x3e784,
        (void *)gCategory3BgPicture,
        gAirshipCategoryPalette,
        (struct sub_effect_record *)gCategory3SpawnTable,
        (struct anim_table_record *)gCategoryFamily1AnimTable,
        gJetpackSpriteSheet,
        7,
        4,
        6,
        0,
        3,
    },
    /* CATEGORY_BLIMP_BONANZA */ {
        CATEGORY_TYPE_JETPACK,
        (void *)gCategoryFamily1CellAnim,
        0x3e784,
        (void *)gCategory4BgPicture,
        gAirshipCategoryPalette,
        (struct sub_effect_record *)gCategory4SpawnTable,
        (struct anim_table_record *)gCategoryFamily1AnimTable,
        gJetpackSpriteSheet,
        7,
        4,
        6,
        0,
        3,
    },
    /* CATEGORY_NO_FLY_ZONE */ {
        CATEGORY_TYPE_JETPACK,
        (void *)gCategoryFamily1CellAnim,
        0x3e784,
        (void *)gCategory5BgPicture,
        gAirshipCategoryPalette,
        (struct sub_effect_record *)gCategory5SpawnTable,
        (struct anim_table_record *)gCategoryFamily1AnimTable,
        gJetpackSpriteSheet,
        7,
        4,
        6,
        0,
        3,
    },
    /* CATEGORY_N_GIN */ {
        CATEGORY_TYPE_HOVERCRAFT,
        (void *)gCategoryFamily1CellAnim,
        0x3e784,
        (void *)gCategory5BgPicture,
        gHovercraftCategoryPalette,
        (struct sub_effect_record *)gCategory6SpawnTable,
        (struct anim_table_record *)gCategoryFamily1AnimTable,
        gJetpackSpriteSheet,
        4,
        3,
        3,
        0,
        1,
    },
};

/* The three per-type category vtables, `gActorCategoryVtable =
 * &gActorCategoryVtables[type]` in SelectActorCategory
 * (actor_category_select.cpp): include/actor_anim.h names the slots;
 * the two numbers are the spawn and skip distances. */
const struct category_vtable gActorCategoryVtables[CATEGORY_TYPE_COUNT] = {
    /* CATEGORY_TYPE_POLAR */ {
        ConstructAnimTableState,
        SpawnActor,
        Yeti::Create,
        Yeti::Update,
        Yeti::UpdateBg2,
        Yeti::Destroy,
        Yeti::LoadGraphics,
        -0x11,
        -0x2f,
        PolarIsTouchingPlayer,
        PolarReachCourseEnd,
        PolarReloadPlayerTiles,
        PolarIsPauseLocked,
    },
    /* CATEGORY_TYPE_JETPACK */ {
        CreateJetpackPlayer,
        SpawnJetpackActor,
        Airship::Create,
        Airship::Update,
        Airship::UpdateBg2,
        Airship::Destroy,
        Airship::LoadGraphics,
        0xa9,
        0x1c,
        JetpackIsTouchingPlayer,
        JetpackReachCourseEnd,
        JetpackReloadPlayerTiles,
        JetpackIsPauseLocked,
    },
    /* CATEGORY_TYPE_HOVERCRAFT */ {
        CreateJetpackPlayer,
        SpawnJetpackActor,
        Hovercraft::Create,
        Hovercraft::Update,
        Hovercraft::UpdateBg2,
        Hovercraft::Destroy,
        Hovercraft::LoadGraphics,
        0xa9,
        0x1c,
        JetpackIsTouchingPlayer,
        JetpackReachCourseEnd,
        JetpackReloadPlayerTiles,
        JetpackIsPauseLocked,
    },
};
