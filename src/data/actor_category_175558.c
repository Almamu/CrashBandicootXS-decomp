#include "core.h"
#include "actor_anim.h"

/*
 * ROM 0x08175558-0x08175760: the actor category descriptors and the
 * per-type category vtables (include/actor_anim.h, docs/graphics.md "An
 * actor \"vtable\" system"). Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern void ConstructAnimTableState();
extern void PolarIsTouchingPlayer();
extern void JetpackIsTouchingPlayer();
extern void sub_802A674();
extern void sub_802A688();
extern void JetpackReloadPlayerTiles();
extern void PolarReloadPlayerTiles();
extern void sub_802A6C4();
extern void sub_802A6D8();
extern void SpawnActor();
extern void UpdateYeti();
extern void UpdateYetiBg2();
extern void LoadYetiGraphics();
extern void DestroyYeti();
extern void CreateYeti();
extern void SpawnJetpackActor();
extern void CreateJetpackPlayer();
extern void CreateAirship();
extern void UpdateAirship();
extern void UpdateAirshipBg2();
extern void LoadAirshipGraphics();
extern void DestroyAirship();
extern void CreateHovercraft();
extern void UpdateHovercraft();
extern void UpdateHovercraftBg2();
extern void LoadHovercraftGraphics();
extern void DestroyHovercraft();

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
const struct category_descriptor gActorCategories[7] = {
    /* 0 */ {
        0,
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
    /* 1 */ {
        0,
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
    /* 2 */ {
        0,
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
    /* 3 */ {
        1,
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
    /* 4 */ {
        1,
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
    /* 5 */ {
        1,
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
    /* 6 */ {
        2,
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
 * (actor_part102.c). Slots 7 and 8 are not code addresses. */
const struct category_vtable gActorCategoryVtables[3] = {
    /* 0 */ { {
        ConstructAnimTableState,
        SpawnActor,
        CreateYeti,
        UpdateYeti,
        UpdateYetiBg2,
        DestroyYeti,
        LoadYetiGraphics,
        (void (*)(void))0xffffffef,
        (void (*)(void))0xffffffd1,
        PolarIsTouchingPlayer,
        sub_802A6D8,
        PolarReloadPlayerTiles,
        sub_802A688,
    } },
    /* 1 */ { {
        CreateJetpackPlayer,
        SpawnJetpackActor,
        CreateAirship,
        UpdateAirship,
        UpdateAirshipBg2,
        DestroyAirship,
        LoadAirshipGraphics,
        (void (*)(void))0xa9,
        (void (*)(void))0x1c,
        JetpackIsTouchingPlayer,
        sub_802A6C4,
        JetpackReloadPlayerTiles,
        sub_802A674,
    } },
    /* 2 */ { {
        CreateJetpackPlayer,
        SpawnJetpackActor,
        CreateHovercraft,
        UpdateHovercraft,
        UpdateHovercraftBg2,
        DestroyHovercraft,
        LoadHovercraftGraphics,
        (void (*)(void))0xa9,
        (void (*)(void))0x1c,
        JetpackIsTouchingPlayer,
        sub_802A6C4,
        JetpackReloadPlayerTiles,
        sub_802A674,
    } },
};
