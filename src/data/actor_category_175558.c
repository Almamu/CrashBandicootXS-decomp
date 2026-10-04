#include "core.h"
#include "actor_anim.h"

/*
 * ROM 0x08175558-0x08175760: the actor category descriptors and the
 * per-type category vtables (include/actor_anim.h, docs/graphics.md "An
 * actor \"vtable\" system"). Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern void ConstructAnimTableState();
extern void sub_802A018();
extern void sub_802A110();
extern void sub_802A674();
extern void sub_802A688();
extern void sub_802A69C();
extern void sub_802A6B0();
extern void sub_802A6C4();
extern void sub_802A6D8();
extern void SpawnActor();
extern void UpdateYeti();
extern void UpdateYetiBg2();
extern void LoadYetiGraphics();
extern void DestroyYeti();
extern void CreateYeti();
extern void SpawnJetpackActor();
extern void sub_802E710();
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
extern const u8 gStaticData_080B2120[];
extern const u8 gCategory1SpawnTable[];
extern const u8 gCategory2SpawnTable[];
extern const u8 gCategoryFamily1CellAnim[];
extern const u8 gCategory3BgPicture[];
extern const u8 gCategory3SpawnTable[];
extern const u8 gStaticData_0814174C[];
extern const u8 gCategory4BgPicture[];
extern const u8 gCategory4SpawnTable[];
extern const u8 gCategory5BgPicture[];
extern const u8 gCategory5SpawnTable[];
extern const u8 gCategory6SpawnTable[];
extern const u16 gStaticData_08178F80[];
extern const u16 gStaticData_0817AAA4[];
extern const u16 gStaticData_0817AEA4[];

/* One descriptor per actor category 0-6 (type 0: categories 0-2,
 * type 1: 3-5, type 2: 6). data.s used to split this table at
 * 0x08175564 and 0x08175584; nothing referenced those labels. */
const struct category_descriptor gActorCategories[7] = {
    /* 0 */ {
        0,
        (void *)gCategoryFamily0CellAnim,
        0x75b94,
        NULL,
        gStaticData_08178F80,
        (struct sub_effect_record *)gCategory0SpawnTable,
        (struct anim_table_record *)gCategoryFamily0AnimTable,
        gStaticData_080B2120,
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
        gStaticData_08178F80,
        (struct sub_effect_record *)gCategory1SpawnTable,
        (struct anim_table_record *)gCategoryFamily0AnimTable,
        gStaticData_080B2120,
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
        gStaticData_08178F80,
        (struct sub_effect_record *)gCategory2SpawnTable,
        (struct anim_table_record *)gCategoryFamily0AnimTable,
        gStaticData_080B2120,
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
        gStaticData_0817AAA4,
        (struct sub_effect_record *)gCategory3SpawnTable,
        (struct anim_table_record *)gCategoryFamily1AnimTable,
        gStaticData_0814174C,
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
        gStaticData_0817AAA4,
        (struct sub_effect_record *)gCategory4SpawnTable,
        (struct anim_table_record *)gCategoryFamily1AnimTable,
        gStaticData_0814174C,
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
        gStaticData_0817AAA4,
        (struct sub_effect_record *)gCategory5SpawnTable,
        (struct anim_table_record *)gCategoryFamily1AnimTable,
        gStaticData_0814174C,
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
        gStaticData_0817AEA4,
        (struct sub_effect_record *)gCategory6SpawnTable,
        (struct anim_table_record *)gCategoryFamily1AnimTable,
        gStaticData_0814174C,
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
        sub_802A018,
        sub_802A6D8,
        sub_802A6B0,
        sub_802A688,
    } },
    /* 1 */ { {
        sub_802E710,
        SpawnJetpackActor,
        CreateAirship,
        UpdateAirship,
        UpdateAirshipBg2,
        DestroyAirship,
        LoadAirshipGraphics,
        (void (*)(void))0xa9,
        (void (*)(void))0x1c,
        sub_802A110,
        sub_802A6C4,
        sub_802A69C,
        sub_802A674,
    } },
    /* 2 */ { {
        sub_802E710,
        SpawnJetpackActor,
        CreateHovercraft,
        UpdateHovercraft,
        UpdateHovercraftBg2,
        DestroyHovercraft,
        LoadHovercraftGraphics,
        (void (*)(void))0xa9,
        (void (*)(void))0x1c,
        sub_802A110,
        sub_802A6C4,
        sub_802A69C,
        sub_802A674,
    } },
};
