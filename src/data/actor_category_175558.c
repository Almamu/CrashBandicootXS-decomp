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
extern void sub_802B218();
extern void sub_802D7B0();
extern void sub_802DA68();
extern void sub_802DE70();
extern void sub_802DFC8();
extern void sub_802DFDC();
extern void sub_802E0CC();
extern void sub_802E710();
extern void sub_8030F88();
extern void sub_80311C4();
extern void sub_80312C4();
extern void sub_8031504();
extern void sub_80317C4();
extern void sub_80331BC();
extern void sub_8033470();
extern void sub_8033550();
extern void sub_8033604();
extern void sub_80337E4();

extern const u8 gStaticData_0803B8B0[];
extern const u8 gStaticData_080B2120[];
extern const u8 gStaticData_080C0C36[];
extern const u8 gStaticData_0814174C[];
extern const u8 gStaticData_08151AC2[];
extern const u8 gStaticData_08178F80[];
extern const u8 gStaticData_0817AA98[];

/* One descriptor per actor category 0-6 (type 0: categories 0-2,
 * type 1: 3-5, type 2: 6). data.s used to split this table at
 * 0x08175564 and 0x08175584; nothing referenced those labels. */
const struct category_descriptor gStaticData_08175558[7] = {
    /* 0 */ {
        0,
        (void *)gStaticData_0803B8B0,
        0x75b94,
        NULL,
        (const u16 *)gStaticData_08178F80,
        (struct sub_effect_record *)(gStaticData_0803B8B0 + 0x75b94),
        (struct anim_table_record *)(gStaticData_08178F80 + 0x74c),
        gStaticData_080B2120,
        4,
        3,
        2,
        1,
        0,
    },
    /* 1 */ {
        0,
        (void *)gStaticData_0803B8B0,
        0x75b94,
        NULL,
        (const u16 *)gStaticData_08178F80,
        (struct sub_effect_record *)(gStaticData_080C0C36 + 0x2),
        (struct anim_table_record *)(gStaticData_08178F80 + 0x74c),
        gStaticData_080B2120,
        4,
        3,
        2,
        3,
        2,
    },
    /* 2 */ {
        0,
        (void *)gStaticData_0803B8B0,
        0x75b94,
        NULL,
        (const u16 *)gStaticData_08178F80,
        (struct sub_effect_record *)(gStaticData_080C0C36 + 0xdba),
        (struct anim_table_record *)(gStaticData_08178F80 + 0x74c),
        gStaticData_080B2120,
        4,
        3,
        2,
        5,
        4,
    },
    /* 3 */ {
        1,
        (void *)(gStaticData_080C0C36 + 0x3e57a),
        0x3e784,
        (void *)(gStaticData_080C0C36 + 0x7ccfe),
        (const u16 *)(gStaticData_0817AA98 + 0xc),
        (struct sub_effect_record *)(gStaticData_080C0C36 + 0x80196),
        (struct anim_table_record *)(gStaticData_0817AA98 + 0x80c),
        gStaticData_0814174C,
        7,
        4,
        6,
        0,
        3,
    },
    /* 4 */ {
        1,
        (void *)(gStaticData_080C0C36 + 0x3e57a),
        0x3e784,
        (void *)(gStaticData_08151AC2 + 0x2),
        (const u16 *)(gStaticData_0817AA98 + 0xc),
        (struct sub_effect_record *)(gStaticData_08151AC2 + 0x259a),
        (struct anim_table_record *)(gStaticData_0817AA98 + 0x80c),
        gStaticData_0814174C,
        7,
        4,
        6,
        0,
        3,
    },
    /* 5 */ {
        1,
        (void *)(gStaticData_080C0C36 + 0x3e57a),
        0x3e784,
        (void *)(gStaticData_08151AC2 + 0x379e),
        (const u16 *)(gStaticData_0817AA98 + 0xc),
        (struct sub_effect_record *)(gStaticData_08151AC2 + 0x6e56),
        (struct anim_table_record *)(gStaticData_0817AA98 + 0x80c),
        gStaticData_0814174C,
        7,
        4,
        6,
        0,
        3,
    },
    /* 6 */ {
        2,
        (void *)(gStaticData_080C0C36 + 0x3e57a),
        0x3e784,
        (void *)(gStaticData_08151AC2 + 0x379e),
        (const u16 *)(gStaticData_0817AA98 + 0x40c),
        (struct sub_effect_record *)(gStaticData_08151AC2 + 0x856e),
        (struct anim_table_record *)(gStaticData_0817AA98 + 0x80c),
        gStaticData_0814174C,
        4,
        3,
        3,
        0,
        1,
    },
};

/* The three per-type category vtables, `gUnknown_03001418 =
 * &gStaticData_081756C4[type]` in SelectActorCategory
 * (actor_part102.c). Slots 7 and 8 are not code addresses. */
const struct category_vtable gStaticData_081756C4[3] = {
    /* 0 */ { {
        ConstructAnimTableState,
        sub_802B218,
        sub_802DFDC,
        sub_802D7B0,
        sub_802DA68,
        sub_802DFC8,
        sub_802DE70,
        (void (*)(void))0xffffffef,
        (void (*)(void))0xffffffd1,
        sub_802A018,
        sub_802A6D8,
        sub_802A6B0,
        sub_802A688,
    } },
    /* 1 */ { {
        sub_802E710,
        sub_802E0CC,
        sub_8030F88,
        sub_80311C4,
        sub_80312C4,
        sub_80317C4,
        sub_8031504,
        (void (*)(void))0xa9,
        (void (*)(void))0x1c,
        sub_802A110,
        sub_802A6C4,
        sub_802A69C,
        sub_802A674,
    } },
    /* 2 */ { {
        sub_802E710,
        sub_802E0CC,
        sub_80331BC,
        sub_8033470,
        sub_8033550,
        sub_80337E4,
        sub_8033604,
        (void (*)(void))0xa9,
        (void (*)(void))0x1c,
        sub_802A110,
        sub_802A6C4,
        sub_802A69C,
        sub_802A674,
    } },
};
