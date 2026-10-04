#include "core.h"
#include "vtable.h"

/*
 * ROM 0x087E3BEC-0x087E55E4: the 93 virtual tables of the game's C++
 * object classes (docs/rom_map.md's "93 entity vtables"), in ROM order.
 * Constructors store one at the object's method-table pointer (e.g.
 * `obj->table = gStaticData_087E3BEC` in graphics.c, `self->vtable` in
 * gobj_1a794.h); the code reads the slots through `struct method` /
 * `struct actor_method`. Tables of the same class family share their
 * leading slots, the ROM's own inheritance. Linked in ROM order between
 * data/data.s sections by ldscript.txt - see docs/data.md.
 */

extern void DestroyFont();
extern void FontMeasureText();
extern void DrawActor();
extern void FontUploadTiles();
extern void nullsub_11();
extern void nullsub_13();
extern void nullsub_15();
extern void nullsub_20();
extern void nullsub_38();
extern void nullsub_44();
extern void nullsub_9();
extern void sub_8006FE4();
extern void sub_8007048();
extern void sub_80070D4();
extern void sub_80070E8();
extern void sub_800710C();
extern void sub_8007110();
extern void sub_8007114();
extern void sub_800722C();
extern void sub_80073BC();
extern void sub_8007DBC();
extern void sub_8007F78();
extern void sub_8007FD8();
extern void sub_8008304();
extern void sub_8008328();
extern void sub_800834C();
extern void sub_8008350();
extern void sub_8008364();
extern void sub_8008394();
extern void sub_8008408();
extern void sub_8008480();
extern void sub_8008484();
extern void sub_80088E8();
extern void sub_80088F0();
extern void sub_8009CA0();
extern void sub_8009DF4();
extern void sub_8009ECC();
extern void sub_8009F1C();
extern void sub_8009FB0();
extern void sub_8009FD4();
extern void sub_800A050();
extern void sub_800A0FC();
extern void sub_800A528();
extern void sub_800A5F4();
extern void sub_800A600();
extern void sub_800A650();
extern void sub_800A884();
extern void sub_800AB9C();
extern void sub_800AC2C();
extern void sub_800AFF4();
extern void sub_800B270();
extern void sub_800B360();
extern void sub_800B3AC();
extern void sub_800B698();
extern void sub_800B6A0();
extern void sub_800B6D0();
extern void sub_800B704();
extern void sub_800B734();
extern void sub_800B7B0();
extern void sub_800B838();
extern void sub_800B86C();
extern void sub_800B8A4();
extern void sub_800B8A8();
extern void sub_800B8DC();
extern void sub_800BD48();
extern void sub_800CA04();
extern void sub_800CA60();
extern void sub_800CACC();
extern void sub_800CB20();
extern void sub_800CB64();
extern void sub_800CBC0();
extern void sub_800CBF4();
extern void sub_800CCCC();
extern void sub_8010480();
extern void sub_80104E4();
extern void sub_8010674();
extern void sub_8010718();
extern void sub_801071C();
extern void sub_8010E34();
extern void sub_8010F8C();
extern void sub_80112C4();
extern void sub_80112F0();
extern void sub_80112F4();
extern void sub_8011330();
extern void sub_8011390();
extern void sub_8011548();
extern void sub_80119A8();
extern void sub_80119D4();
extern void sub_80119D8();
extern void sub_8011A1C();
extern void sub_8011A8C();
extern void sub_8011B5C();
extern void sub_8011BD4();
extern void sub_8012420();
extern void sub_8015350();
extern void sub_80155A8();
extern void sub_80157C4();
extern void sub_8015878();
extern void sub_8016128();
extern void sub_8016288();
extern void sub_8017218();
extern void sub_80174D8();
extern void sub_8017650();
extern void sub_80179D4();
extern void sub_80179E8();
extern void sub_80179EC();
extern void sub_8017A70();
extern void sub_8017A78();
extern void sub_8017AB0();
extern void sub_8017F5C();
extern void sub_8017F80();
extern void sub_8017FD4();
extern void sub_8018008();
extern void sub_80187FC();
extern void sub_8018858();
extern void sub_8018884();
extern void sub_80188E8();
extern void sub_80188FC();
extern void sub_8018960();
extern void sub_80189C4();
extern void sub_8018A30();
extern void sub_8018E4C();
extern void sub_8019324();
extern void sub_8019464();
extern void sub_80194E0();
extern void sub_80195D8();
extern void sub_8019608();
extern void sub_801964C();
extern void sub_80196E4();
extern void sub_8019730();
extern void sub_8019744();
extern void sub_80197C8();
extern void sub_80197F8();
extern void sub_801A114();
extern void sub_801A2A8();
extern void sub_801A64C();
extern void sub_801A73C();
extern void sub_801A750();
extern void sub_801A780();
extern void sub_801A824();
extern void sub_801AB34();
extern void sub_801B208();
extern void sub_801B2C0();
extern void sub_801B2C4();
extern void sub_801B304();
extern void sub_801B77C();
extern void sub_801B7A0();
extern void sub_801B7C4();
extern void sub_801B8BC();
extern void sub_801B91C();
extern void sub_801BA60();
extern void sub_801BAB0();
extern void sub_801DE30();
extern void sub_801DEA4();
extern void sub_801DF70();
extern void sub_801DF98();
extern void DestroyBgStreamer();
extern void DestroyBgLayerBase();
extern void sub_8024DCC();
extern void ScrollBgLayerBase();
extern void ResetBgLayerBase();
extern void ClipBgLayerColumns();
extern void ClipBgLayerRows();
extern void ScrollBgLayer();
extern void DrawBgLayerColumn();
extern void DrawBgLayerRow();
extern void ResetBgLayer();
extern void LoadBgLayerTiles();
extern void DestroyBgLayer();
extern void DrawPooledBgLayerColumn();
extern void sub_8026250();
extern void ClipPooledBgLayerColumns();
extern void ClipPooledBgLayerRows();
extern void DrawPooledBgLayerRow();
extern void ResetPooledBgLayer();
extern void LoadPooledBgLayerTiles();
extern void DestroyPooledBgLayer();
extern void sub_802710C();
extern void FontDrawGlyph();
extern void FontPutChar();
extern void FontDrawChars();
extern void FontDrawText();
extern void FontMeasureChars();
extern void UpdateActor();
extern void DestroyActor();
extern void sub_802B364();
extern void sub_802B5B4();
extern void sub_802C19C();
extern void sub_802C270();
extern void sub_802C2FC();
extern void sub_802C394();
extern void sub_802C464();
extern void sub_802C4C8();
extern void sub_802C540();
extern void sub_802C614();
extern void sub_802C6C0();
extern void sub_802C904();
extern void sub_802C99C();
extern void sub_802CA6C();
extern void sub_802CAD0();
extern void sub_802CC9C();
extern void sub_802CE10();
extern void sub_802CE5C();
extern void sub_802CF30();
extern void sub_802D0F4();
extern void sub_802D2DC();
extern void sub_802D59C();
extern void sub_802D600();
extern void sub_802D6A0();
extern void sub_802E84C();
extern void sub_802E9FC();
extern void sub_802EB78();
extern void sub_802F47C();
extern void sub_802F6DC();
extern void sub_802F97C();
extern void sub_802FA34();
extern void sub_802FA38();
extern void sub_802FD1C();
extern void sub_802FF00();
extern void sub_802FFB8();
extern void sub_80301EC();
extern void sub_8030290();
extern void sub_8030298();
extern void sub_8030330();
extern void sub_8030530();
extern void sub_8030574();
extern void sub_80306A4();
extern void sub_80317E0();
extern void sub_8031858();
extern void sub_8031A64();
extern void sub_8031A6C();
extern void sub_8031B0C();
extern void sub_8031C0C();
extern void sub_8031D04();
extern void sub_8031D7C();
extern void sub_8031E80();
extern void sub_8031FE8();
extern void sub_8032140();
extern void sub_8032170();
extern void sub_80321D0();
extern void sub_8032350();
extern void sub_8032358();
extern void sub_80323F4();
extern void sub_8032478();
extern void sub_8032480();
extern void sub_80325A4();
extern void sub_8032680();
extern void sub_8032688();
extern void sub_8032714();
extern void sub_8032718();
extern void sub_80327A4();
extern void sub_803283C();
extern void sub_803290C();
extern void sub_8032910();
extern void sub_8032950();
extern void sub_8032AF0();
extern void sub_8033AE0();
extern void sub_8033B44();
extern void sub_8033CF0();
extern void sub_8033E18();
extern void sub_8033E80();
extern void sub_8034050();
extern void sub_8034110();
extern void sub_8034188();
extern void sub_8034264();
extern void sub_8034270();
extern void sub_803436C();
extern void sub_8036EC4();
extern void sub_8036FBC();
extern void sub_803716C();
extern void DestroyLargeFont();
extern void DestroySmallFont();
extern void sub_803B0C4();
extern void sub_803B0F0();
extern void sub_803B128();
extern void sub_803B154();
extern void sub_803B180();
extern void sub_803B1AC();
extern void sub_803B1D8();
extern void sub_803B204();
extern void sub_803B230();
extern void sub_803B25C();
extern void sub_803B288();
extern void sub_803B2B4();
extern void sub_803B2E0();
extern void sub_803B30C();
extern void sub_803B338();
extern void sub_803B364();
extern void sub_803B390();
extern void sub_803B3BC();
extern void sub_803B3E8();
extern void sub_803B414();
extern void sub_803B440();
extern void sub_803B46C();
extern void sub_803B4EC();
extern void sub_803B54C();
extern void sub_803B550();
extern void sub_803B57C();
extern void sub_803B5AC();
extern void sub_803B5B0();
extern void sub_803B5DC();
extern void sub_803B5E4();
extern void sub_803B5E8();
extern void sub_803B614();
extern void sub_803B640();
extern void sub_803B66C();
extern void sub_803B698();
extern void sub_803B6C4();
extern void sub_803B6F0();
extern void sub_803B710();
extern void sub_803B730();
extern void sub_803B750();
extern void sub_803B77C();
extern void sub_803B7A8();
extern void sub_803B7D4();
extern void sub_803B800();
extern void sub_803B82C();
extern void sub_803B858();
extern void sub_803B884();

/* Used by actor_aabb_setup.c, actor_part124.c, actor_part39.c,
 * actor_part6.c (sub_8008484), actor_part7.c, graphics.c (nullsub_12,
 * sub_8007230, sub_80073BC). */
const struct vtable_slot gStaticData_087E3BEC[11] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8007048),
    VTABLE_SLOT(sub_80070E8),
    VTABLE_SLOT(sub_80070D4),
    VTABLE_SLOT(nullsub_11),
    VTABLE_SLOT(sub_8007110),
    VTABLE_SLOT(sub_800710C),
    VTABLE_SLOT(sub_8006FE4),
    VTABLE_SLOT(sub_8007114),
    VTABLE_SLOT(sub_800722C),
    VTABLE_SLOT(sub_80073BC),
};

/* Used by actor_part6.c (sub_8008408, sub_8008484), actor_part7.c. */
const struct vtable_slot gStaticData_087E3C44[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8007DBC),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(sub_8008364),
    VTABLE_SLOT(sub_8008350),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8008304),
    VTABLE_SLOT(sub_8008480),
    VTABLE_SLOT(sub_8008484),
    VTABLE_SLOT(sub_8008408),
    VTABLE_SLOT(sub_800834C),
};

/* Used by actor_part7.c (sub_80088F0). */
const struct vtable_slot gStaticData_087E3CAC[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8007DBC),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(sub_8008364),
    VTABLE_SLOT(sub_8008350),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8008304),
    VTABLE_SLOT(sub_8008480),
    VTABLE_SLOT(sub_80088F0),
    VTABLE_SLOT(sub_80088E8),
    VTABLE_SLOT(sub_800834C),
};

/* Used by actor_part14.c, actor_part8.c (sub_8009ECC, sub_8009F1C,
 * sub_8009F50). */
const struct vtable_slot gStaticData_087E3D14[15] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_800A050),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(sub_8009FB0),
    VTABLE_SLOT(sub_8008350),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8008304),
    VTABLE_SLOT(sub_8009ECC),
    VTABLE_SLOT(sub_8009F1C),
    VTABLE_SLOT(sub_8008408),
    VTABLE_SLOT(sub_8009DF4),
    VTABLE_SLOT(sub_8009FD4),
    VTABLE_SLOT(sub_8009CA0),
};

/* Used by actor_part14.c (sub_800A600, sub_800A650, sub_800A664). */
const struct vtable_slot gStaticData_087E3D8C[15] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_800A0FC),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(sub_800A528),
    VTABLE_SLOT(sub_800A5F4),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8008304),
    VTABLE_SLOT(sub_800A600),
    VTABLE_SLOT(sub_800A650),
    VTABLE_SLOT(sub_8008408),
    VTABLE_SLOT(sub_8009DF4),
    VTABLE_SLOT(sub_8009FD4),
    VTABLE_SLOT(sub_8009CA0),
};

/* Used by actor_part15.c, actor_part77.c. */
const struct vtable_slot gStaticData_087E3E04[15] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_800A884),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(sub_800B360),
    VTABLE_SLOT(sub_800AFF4),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8008304),
    VTABLE_SLOT(sub_800A600),
    VTABLE_SLOT(sub_800B3AC),
    VTABLE_SLOT(sub_8008408),
    VTABLE_SLOT(sub_800B270),
    VTABLE_SLOT(sub_800AC2C),
    VTABLE_SLOT(sub_800AB9C),
};

/* Used by actor_part124.c, actor_part17.c, actor_part27.c, actor_part57.c. */
const struct vtable_slot gStaticData_087E3E7C[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(nullsub_9),
    VTABLE_SLOT(nullsub_13),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_800B8A8),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part112.c, actor_part124.c. */
const struct vtable_slot gStaticData_087E3EE4[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_800B8DC),
    VTABLE_SLOT(sub_800BD48),
    VTABLE_SLOT(sub_800CA04),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_800CA60),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part124.c. */
const struct vtable_slot gStaticData_087E3F4C[11] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8007048),
    VTABLE_SLOT(sub_80070E8),
    VTABLE_SLOT(sub_800CACC),
    VTABLE_SLOT(nullsub_11),
    VTABLE_SLOT(sub_8007110),
    VTABLE_SLOT(sub_800710C),
    VTABLE_SLOT(sub_8006FE4),
    VTABLE_SLOT(sub_8007114),
    VTABLE_SLOT(sub_800722C),
    VTABLE_SLOT(sub_800CB20),
};

/* Used by actor_part112.c, actor_part117.c, actor_part124.c. */
const struct vtable_slot gStaticData_087E3FA4[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_800CB64),
    VTABLE_SLOT(nullsub_13),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_800CBC0),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part123.c. */
const struct vtable_slot gStaticData_087E400C[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_800CBF4),
    VTABLE_SLOT(nullsub_15),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_800CCCC),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by game_loop31.c (sub_801071C), game_loop36.c (sub_800FF0C). */
const struct vtable_slot gStaticData_087E4074[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8007DBC),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(sub_80104E4),
    VTABLE_SLOT(sub_8010480),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8010674),
    VTABLE_SLOT(sub_8010718),
    VTABLE_SLOT(sub_801071C),
    VTABLE_SLOT(sub_8008408),
    VTABLE_SLOT(sub_800834C),
};

/* Used by game_loop52.c, game_loop54.c (sub_8010F8C). */
const struct vtable_slot gStaticData_087E40DC[14] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8011330),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(sub_8010F8C),
    VTABLE_SLOT(sub_80112C4),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8008304),
    VTABLE_SLOT(sub_80112F0),
    VTABLE_SLOT(sub_80112F4),
    VTABLE_SLOT(sub_8008408),
    VTABLE_SLOT(sub_800834C),
    VTABLE_SLOT(sub_8010E34),
};

/* Used by actor_part39.c (sub_80119D8, sub_80119EC), game_loop53.c
 * (sub_8011548). */
const struct vtable_slot gStaticData_087E414C[14] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8011A1C),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(sub_8011548),
    VTABLE_SLOT(sub_80119A8),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8008304),
    VTABLE_SLOT(sub_80119D4),
    VTABLE_SLOT(sub_80119D8),
    VTABLE_SLOT(sub_8008408),
    VTABLE_SLOT(sub_800834C),
    VTABLE_SLOT(sub_8011390),
};

/* Used by actor_part39.c (sub_8011A8C, sub_8011B5C). */
const struct vtable_slot gStaticData_087E41BC[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8007DBC),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(sub_8011A8C),
    VTABLE_SLOT(sub_8008350),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8008304),
    VTABLE_SLOT(sub_8008480),
    VTABLE_SLOT(sub_8011B5C),
    VTABLE_SLOT(sub_8008408),
    VTABLE_SLOT(sub_800834C),
};

/* Used by actor_part39.c, actor_part57.c. */
const struct vtable_slot gStaticData_087E4224[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8012420),
    VTABLE_SLOT(sub_8011BD4),
    VTABLE_SLOT(sub_80155A8),
    VTABLE_SLOT(sub_8015350),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_8015878),
    VTABLE_SLOT(sub_80157C4),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_16048.c (sub_80174D8), player_ctrl.h. */
const struct vtable_slot gStaticData_087E428C[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8016288),
    VTABLE_SLOT(sub_8016128),
    VTABLE_SLOT(sub_8017218),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_80174D8),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_17524.c (sub_80179EC). */
const struct vtable_slot gStaticData_087E42F4[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8017650),
    VTABLE_SLOT(sub_80179D4),
    VTABLE_SLOT(sub_80179E8),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_80179EC),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part27.c. */
const struct vtable_slot gStaticData_087E435C[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(nullsub_9),
    VTABLE_SLOT(sub_8017A70),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_8017A78),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part27b.c. */
const struct vtable_slot gStaticData_087E43C4[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8017AB0),
    VTABLE_SLOT(sub_8017A70),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_8017FD4),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_8017F80),
    VTABLE_SLOT(sub_8017F5C),
};

/* Used by actor_part27c.c. */
const struct vtable_slot gStaticData_087E442C[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_80187FC),
    VTABLE_SLOT(nullsub_13),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_8018858),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_188d0.c (sub_80188D0, sub_80188E8). */
const struct vtable_slot gStaticData_087E4494[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8018884),
    VTABLE_SLOT(nullsub_13),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_80188E8),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_188d0.c (sub_8018948, sub_8018960). */
const struct vtable_slot gStaticData_087E44FC[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_80188FC),
    VTABLE_SLOT(nullsub_13),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_8018960),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_18008.c, actor_part_188d0.c (sub_80189C4,
 * sub_80189EC). */
const struct vtable_slot gStaticData_087E4564[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8018008),
    VTABLE_SLOT(sub_8017A70),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_80189C4),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_188d0.c (sub_80195D8, sub_80195EC). */
const struct vtable_slot gStaticData_087E45CC[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_80194E0),
    VTABLE_SLOT(nullsub_13),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_80195D8),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_188d0.c (sub_8019608, sub_801961C). */
const struct vtable_slot gStaticData_087E4634[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8019464),
    VTABLE_SLOT(nullsub_13),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_8019608),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_801B7A0),
    VTABLE_SLOT(sub_801B77C),
};

/* Used by actor_part_188d0.c (sub_801964C, sub_8019660). */
const struct vtable_slot gStaticData_087E469C[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8019324),
    VTABLE_SLOT(nullsub_13),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_801964C),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_1967c.c (sub_80196E4). */
const struct vtable_slot gStaticData_087E4704[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8018E4C),
    VTABLE_SLOT(nullsub_13),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_80196E4),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_1967c.c (sub_8019744). */
const struct vtable_slot gStaticData_087E476C[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8019730),
    VTABLE_SLOT(nullsub_13),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_8019744),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_1967c.c (sub_80197C8). */
const struct vtable_slot gStaticData_087E47D4[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8018A30),
    VTABLE_SLOT(sub_8017A70),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_80197C8),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_1967c.c (sub_801A64C, sub_801A73C). */
const struct vtable_slot gStaticData_087E483C[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_801A64C),
    VTABLE_SLOT(sub_800BD48),
    VTABLE_SLOT(sub_800CA04),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_801A73C),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_1967c.c (sub_801A584, sub_801A750). */
const struct vtable_slot gStaticData_087E48A4[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_801A2A8),
    VTABLE_SLOT(sub_8017A70),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_801A750),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_1967c.c (sub_801A780), actor_part_1a794.c,
 * gobj_1a794.h. */
const struct vtable_slot gStaticData_087E490C[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_801A114),
    VTABLE_SLOT(sub_8017A70),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_801A780),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_1967c.c, actor_part_1a794.c (sub_801A824),
 * gobj_1a794.h. */
const struct vtable_slot gStaticData_087E4974[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_80197F8),
    VTABLE_SLOT(sub_8017A70),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_801A824),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_800B838),
    VTABLE_SLOT(sub_800B704),
};

/* Used by actor_part_1b208.c (sub_801B2C4), gobj_1a794.h. */
const struct vtable_slot gStaticData_087E49DC[15] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_801AB34),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(sub_801B208),
    VTABLE_SLOT(sub_8008350),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8008304),
    VTABLE_SLOT(sub_801B2C0),
    VTABLE_SLOT(sub_801B2C4),
    VTABLE_SLOT(sub_8008408),
    VTABLE_SLOT(sub_8009DF4),
    VTABLE_SLOT(sub_8009FD4),
    VTABLE_SLOT(sub_8009CA0),
};

/* Used by actor_part_1b208.c (sub_801B7C4), gobj_1a794.h. */
const struct vtable_slot gStaticData_087E4A54[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_801B304),
    VTABLE_SLOT(nullsub_13),
    VTABLE_SLOT(sub_800B8A4),
    VTABLE_SLOT(sub_800B698),
    VTABLE_SLOT(sub_800B7B0),
    VTABLE_SLOT(sub_800B6D0),
    VTABLE_SLOT(sub_800B734),
    VTABLE_SLOT(sub_800B6A0),
    VTABLE_SLOT(sub_801B7C4),
    VTABLE_SLOT(sub_800B86C),
    VTABLE_SLOT(sub_801B7A0),
    VTABLE_SLOT(sub_801B77C),
};

/* Used by actor_part_1b85c.c (sub_801B91C). */
const struct vtable_slot gStaticData_087E4ABC[15] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_800A050),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(sub_801B8BC),
    VTABLE_SLOT(sub_8008350),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8008304),
    VTABLE_SLOT(sub_8009ECC),
    VTABLE_SLOT(sub_801B91C),
    VTABLE_SLOT(sub_8008408),
    VTABLE_SLOT(sub_8009DF4),
    VTABLE_SLOT(sub_8009FD4),
    VTABLE_SLOT(sub_8009CA0),
};

/* Used by actor_part_1b85c.c (sub_801B980, sub_801BAB0, sub_801BAC4). */
const struct vtable_slot gStaticData_087E4B34[15] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_800A050),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(sub_8009FB0),
    VTABLE_SLOT(sub_8008350),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8008304),
    VTABLE_SLOT(sub_8009ECC),
    VTABLE_SLOT(sub_801BAB0),
    VTABLE_SLOT(sub_8008408),
    VTABLE_SLOT(sub_8009DF4),
    VTABLE_SLOT(sub_8009FD4),
    VTABLE_SLOT(sub_801BA60),
};

/* Used by actor_part_1da38.c (sub_801DF98), actor_part_1dfec.c,
 * level_select_parts.h. */
const struct vtable_slot gStaticData_087E4BAC[6] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_801DE30),
    VTABLE_SLOT(sub_801DEA4),
    VTABLE_SLOT(sub_801DF70),
    VTABLE_SLOT(nullsub_20),
    VTABLE_SLOT(sub_801DF98),
};

/* Used by game_loop57.c. */
const struct vtable_slot gBgStreamerVtable[2] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyBgStreamer),
};

/* Used by game_loop57.c. */
const struct vtable_slot gBgLayerBaseVtable[5] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyBgLayerBase),
    VTABLE_SLOT(ResetBgLayerBase),
    VTABLE_SLOT(ScrollBgLayerBase),
    VTABLE_SLOT(sub_8024DCC),
};

/* Used by bg_scroll_layer_25fc8.c (DestroyBgLayer), game_loop15.c
 * (InitBgLayer), tile_slot_pool.c (DestroyPooledBgLayer), bg_scroll_layer.h. */
const struct vtable_slot gBgLayerVtable[10] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyBgLayer),
    VTABLE_SLOT(ResetBgLayer),
    VTABLE_SLOT(ScrollBgLayer),
    VTABLE_SLOT(sub_8024DCC),
    VTABLE_SLOT(LoadBgLayerTiles),
    VTABLE_SLOT(DrawBgLayerRow),
    VTABLE_SLOT(DrawBgLayerColumn),
    VTABLE_SLOT(ClipBgLayerColumns),
    VTABLE_SLOT(ClipBgLayerRows),
};

/* Used by bg_scroll_layer_25fc8.c, tile_slot_pool.c (DestroyPooledBgLayer),
 * bg_scroll_layer.h. */
const struct vtable_slot gPooledBgLayerVtable[10] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyPooledBgLayer),
    VTABLE_SLOT(ResetPooledBgLayer),
    VTABLE_SLOT(ScrollBgLayer),
    VTABLE_SLOT(sub_8026250),
    VTABLE_SLOT(LoadPooledBgLayerTiles),
    VTABLE_SLOT(DrawPooledBgLayerRow),
    VTABLE_SLOT(DrawPooledBgLayerColumn),
    VTABLE_SLOT(ClipPooledBgLayerColumns),
    VTABLE_SLOT(ClipPooledBgLayerRows),
};

/* Used by hud_icon_slot.c (sub_802710C). */
const struct vtable_slot gStaticData_087E4CB4[13] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_8007DBC),
    VTABLE_SLOT(sub_8008394),
    VTABLE_SLOT(sub_8008364),
    VTABLE_SLOT(sub_8008350),
    VTABLE_SLOT(sub_8007F78),
    VTABLE_SLOT(sub_8007FD8),
    VTABLE_SLOT(sub_8008328),
    VTABLE_SLOT(sub_8008304),
    VTABLE_SLOT(sub_8008480),
    VTABLE_SLOT(sub_802710C),
    VTABLE_SLOT(sub_80088E8),
    VTABLE_SLOT(sub_800834C),
};

/* Used by actor_aabb_setup.c, hud_icon_widget_85c4.c (FontDrawGlyph). */
const struct vtable_slot gLargeFontVtable[9] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyLargeFont),
    VTABLE_SLOT(FontMeasureText),
    VTABLE_SLOT(FontMeasureChars),
    VTABLE_SLOT(FontDrawText),
    VTABLE_SLOT(FontDrawChars),
    VTABLE_SLOT(FontDrawGlyph),
    VTABLE_SLOT(FontPutChar),
    VTABLE_SLOT(FontUploadTiles),
};

/* Used by actor_aabb_setup.c, hud_icon_widget_85c4.c (FontDrawGlyph). */
const struct vtable_slot gSmallFontVtable[9] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroySmallFont),
    VTABLE_SLOT(FontMeasureText),
    VTABLE_SLOT(FontMeasureChars),
    VTABLE_SLOT(FontDrawText),
    VTABLE_SLOT(FontDrawChars),
    VTABLE_SLOT(FontDrawGlyph),
    VTABLE_SLOT(FontPutChar),
    VTABLE_SLOT(FontUploadTiles),
};

/* Used by actor_aabb_setup.c, hud_icon_widget5.c (DestroyFont),
 * hud_icon_widget_85c4.c (FontDrawGlyph), hud_icon_widget_8a78.c. */
const struct vtable_slot gFontVtable[9] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyFont),
    VTABLE_SLOT(FontMeasureText),
    VTABLE_SLOT(FontMeasureChars),
    VTABLE_SLOT(FontDrawText),
    VTABLE_SLOT(FontDrawChars),
    VTABLE_SLOT(FontDrawGlyph),
    VTABLE_SLOT(FontPutChar),
    VTABLE_SLOT(FontUploadTiles),
};

/* The actor base class (struct actor_self): slot 1 its destructor
 * DestroyActor, slot 2 the per-frame UpdateActor, slot 3 DrawActor.
 * InitActorPart installs it, and every derived destructor puts it back
 * before unlinking the actor.
 *
 * Used by counter_selector.c (sub_803716C), actor_anim.c (sub_803B0C4,
 * sub_803B128, sub_803B154, sub_803B180, sub_803B1AC, sub_803B1D8,
 * sub_803B204, sub_803B230, sub_803B25C, sub_803B288, sub_803B2B4,
 * sub_803B2E0, sub_803B30C, sub_803B338, sub_803B364, sub_803B390,
 * sub_803B3BC, sub_803B3E8, sub_803B414, sub_803B440, sub_803B550,
 * sub_803B5B0, sub_803B5E8, sub_803B614, sub_803B640, sub_803B66C,
 * sub_803B698, sub_803B6C4, sub_803B750, sub_803B77C, sub_803B7A8,
 * sub_803B7D4, sub_803B800, sub_803B82C, sub_803B858, sub_803B884),
 * actor_part129.c, actor_part130.c (sub_803283C), actor_part19.c,
 * actor_part19c.c, actor_part44.c, actor_part50.c, actor_part52.c. */
const struct vtable_slot gActorVtable[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(DestroyActor),
    VTABLE_SLOT(UpdateActor),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part_2ac28.c. */
const struct vtable_slot gStaticData_087E4E14[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B0C4),
    VTABLE_SLOT(UpdateActor),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part_2ac28.c (sub_802B12C). */
const struct vtable_slot gStaticData_087E4E34[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B128),
    VTABLE_SLOT(sub_803B0F0),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part127.c, actor_part19.c, actor_part_2ac28.c
 * (ConstructAnimTableState). */
const struct vtable_slot gStaticData_087E4E54[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_802C19C),
    VTABLE_SLOT(sub_802B364),
    VTABLE_SLOT(sub_802B5B4),
};

/* Used by actor_part19.c, actor_part19c.c, actor_part19c2.c. */
const struct vtable_slot gStaticData_087E4E74[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_802C394),
    VTABLE_SLOT(sub_802C270),
    VTABLE_SLOT(sub_802C2FC),
};

/* Used by actor_part19.c, actor_part19g.c, actor_part_2ac28.c. */
const struct vtable_slot gStaticData_087E4E94[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B154),
    VTABLE_SLOT(sub_802C464),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part19i.c, actor_part_2ac28.c. */
const struct vtable_slot gStaticData_087E4EB4[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B180),
    VTABLE_SLOT(sub_802C99C),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part19i.c, actor_part_2ac28.c. */
const struct vtable_slot gStaticData_087E4ED4[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B1AC),
    VTABLE_SLOT(sub_802C540),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part19i.c, actor_part_2ac28.c. */
const struct vtable_slot gStaticData_087E4EF4[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B1D8),
    VTABLE_SLOT(sub_802C904),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part19i.c, actor_part_2ac28.c. */
const struct vtable_slot gStaticData_087E4F14[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B204),
    VTABLE_SLOT(sub_802C6C0),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part19i.c, actor_part_2ac28.c. */
const struct vtable_slot gStaticData_087E4F34[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B230),
    VTABLE_SLOT(sub_802C614),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part19i.c, actor_part_2ac28.c. */
const struct vtable_slot gStaticData_087E4F54[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B25C),
    VTABLE_SLOT(sub_802CA6C),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part19i.c, actor_part_2ac28.c. */
const struct vtable_slot gStaticData_087E4F74[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B288),
    VTABLE_SLOT(sub_802CAD0),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part19i.c. */
const struct vtable_slot gStaticData_087E4F94[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B2B4),
    VTABLE_SLOT(sub_802C4C8),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part126.c. */
const struct vtable_slot gStaticData_087E4FB4[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B2E0),
    VTABLE_SLOT(sub_802CC9C),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part126.c. */
const struct vtable_slot gStaticData_087E4FD4[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B30C),
    VTABLE_SLOT(sub_802CE10),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part126.c. */
const struct vtable_slot gStaticData_087E4FF4[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B338),
    VTABLE_SLOT(sub_802CE5C),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part126.c. */
const struct vtable_slot gStaticData_087E5014[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B364),
    VTABLE_SLOT(sub_802CF30),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part126.c. */
const struct vtable_slot gStaticData_087E5034[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B390),
    VTABLE_SLOT(sub_802D0F4),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part58.c. */
const struct vtable_slot gStaticData_087E5054[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B3BC),
    VTABLE_SLOT(sub_802D2DC),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part58.c. */
const struct vtable_slot gStaticData_087E5074[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B3E8),
    VTABLE_SLOT(sub_802D59C),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part58.c. */
const struct vtable_slot gStaticData_087E5094[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B414),
    VTABLE_SLOT(sub_802D600),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part58.c. */
const struct vtable_slot gStaticData_087E50B4[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B440),
    VTABLE_SLOT(sub_802D6A0),
    VTABLE_SLOT(DrawActor),
};

/* Used by actor_part128.c (sub_802E3CC). */
const struct vtable_slot gStaticData_087E50D4[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B550),
    VTABLE_SLOT(sub_803B4EC),
    VTABLE_SLOT(sub_803B46C),
    VTABLE_SLOT(nullsub_44),
    VTABLE_SLOT(sub_803B54C),
    VTABLE_SLOT(sub_803B5DC),
};

/* Used by actor_part128.c (sub_802E420). */
const struct vtable_slot gStaticData_087E510C[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B5B0),
    VTABLE_SLOT(sub_803B57C),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(nullsub_44),
    VTABLE_SLOT(sub_803B5AC),
    VTABLE_SLOT(sub_803B5DC),
};

/* Used by actor_part128.c (sub_802E710), actor_part44.c. */
const struct vtable_slot gStaticData_087E5144[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_802F6DC),
    VTABLE_SLOT(sub_802E84C),
    VTABLE_SLOT(sub_802E9FC),
    VTABLE_SLOT(sub_802EB78),
    VTABLE_SLOT(sub_803B5E4),
    VTABLE_SLOT(sub_802F47C),
};

/* Used by actor_part45c.c. */
const struct vtable_slot gStaticData_087E517C[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B5E8),
    VTABLE_SLOT(sub_802F97C),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(nullsub_44),
    VTABLE_SLOT(sub_802FA34),
    VTABLE_SLOT(sub_803B5DC),
};

/* Used by actor_part_2fbf0.c (sub_802FD8C). */
const struct vtable_slot gStaticData_087E51B4[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B614),
    VTABLE_SLOT(sub_802FA38),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(sub_802FD1C),
    VTABLE_SLOT(sub_802FF00),
    VTABLE_SLOT(sub_803B5DC),
};

/* Used by actor_part_2fbf0.c (sub_802FF08). */
const struct vtable_slot gStaticData_087E51EC[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B640),
    VTABLE_SLOT(sub_802FFB8),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(sub_80301EC),
    VTABLE_SLOT(sub_8030290),
    VTABLE_SLOT(sub_803B5DC),
};

/* Used by actor_part_2fbf0.c (sub_8030300). */
const struct vtable_slot gStaticData_087E5224[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B66C),
    VTABLE_SLOT(sub_8030298),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(nullsub_44),
    VTABLE_SLOT(sub_8030330),
    VTABLE_SLOT(sub_803B5DC),
};

/* Used by actor_part20d.c. */
const struct vtable_slot gStaticData_087E525C[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B698),
    VTABLE_SLOT(sub_8030574),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(sub_8030530),
    VTABLE_SLOT(sub_80306A4),
    VTABLE_SLOT(sub_803B5DC),
};

/* Used by actor_part125.c. */
const struct vtable_slot gStaticData_087E5294[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B6C4),
    VTABLE_SLOT(sub_80317E0),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(sub_8031858),
    VTABLE_SLOT(sub_8031A64),
    VTABLE_SLOT(sub_803B5DC),
};

/* Used by actor_part129.c. */
const struct vtable_slot gStaticData_087E52CC[8] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B6F0),
    VTABLE_SLOT(sub_8031D04),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(sub_8031FE8),
    VTABLE_SLOT(sub_8032350),
    VTABLE_SLOT(sub_803B5DC),
    VTABLE_SLOT(sub_8032140),
};

/* Used by actor_part129.c. */
const struct vtable_slot gStaticData_087E530C[8] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B710),
    VTABLE_SLOT(sub_8031D7C),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(sub_8031E80),
    VTABLE_SLOT(sub_8032350),
    VTABLE_SLOT(sub_803B5DC),
    VTABLE_SLOT(sub_8032140),
};

/* Used by actor_part129.c. */
const struct vtable_slot gStaticData_087E534C[8] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B730),
    VTABLE_SLOT(sub_8031B0C),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(sub_8031C0C),
    VTABLE_SLOT(sub_8032350),
    VTABLE_SLOT(sub_803B5DC),
    VTABLE_SLOT(sub_8032140),
};

/* Used by actor_part129.c. */
const struct vtable_slot gStaticData_087E538C[8] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_80321D0),
    VTABLE_SLOT(sub_8031A6C),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(sub_8032170),
    VTABLE_SLOT(sub_8032350),
    VTABLE_SLOT(sub_803B5DC),
    VTABLE_SLOT(sub_8032140),
};

/* Used by actor_part129.c (sub_8032440). */
const struct vtable_slot gStaticData_087E53CC[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B750),
    VTABLE_SLOT(sub_8032358),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(sub_80323F4),
    VTABLE_SLOT(sub_8032478),
    VTABLE_SLOT(sub_803B5DC),
};

/* Used by actor_part129.c. */
const struct vtable_slot gStaticData_087E5404[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B77C),
    VTABLE_SLOT(sub_8032480),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(sub_80325A4),
    VTABLE_SLOT(sub_8032680),
    VTABLE_SLOT(sub_803B5DC),
};

/* Used by actor_part130.c. */
const struct vtable_slot gStaticData_087E543C[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B7A8),
    VTABLE_SLOT(sub_8032688),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(nullsub_44),
    VTABLE_SLOT(sub_8032714),
    VTABLE_SLOT(sub_803B5DC),
};

/* Used by actor_part130.c (sub_803283C). */
const struct vtable_slot gStaticData_087E5474[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803283C),
    VTABLE_SLOT(sub_8032718),
    VTABLE_SLOT(sub_80327A4),
    VTABLE_SLOT(nullsub_44),
    VTABLE_SLOT(sub_803290C),
    VTABLE_SLOT(sub_803B5DC),
};

/* Used by actor_part130.c. */
const struct vtable_slot gStaticData_087E54AC[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B7D4),
    VTABLE_SLOT(sub_8032950),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(sub_8032910),
    VTABLE_SLOT(sub_8032AF0),
    VTABLE_SLOT(sub_803B5DC),
};

/* Used by actor_part32.c. */
const struct vtable_slot gStaticData_087E54E4[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B800),
    VTABLE_SLOT(sub_8033B44),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(sub_8033AE0),
    VTABLE_SLOT(sub_8033CF0),
    VTABLE_SLOT(sub_803B5DC),
};

/* Used by actor_part63.c. */
const struct vtable_slot gStaticData_087E551C[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B82C),
    VTABLE_SLOT(sub_8033E80),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(sub_8033E18),
    VTABLE_SLOT(sub_8034050),
    VTABLE_SLOT(sub_803B5DC),
};

/* Used by actor_part65.c, actor_part66.c (sub_8034058), actor_part67.c. */
const struct vtable_slot gStaticData_087E5554[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B858),
    VTABLE_SLOT(sub_8034188),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(sub_8034110),
    VTABLE_SLOT(sub_8034264),
    VTABLE_SLOT(sub_803B5DC),
};

/* Used by actor_part69.c. */
const struct vtable_slot gStaticData_087E558C[7] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803B884),
    VTABLE_SLOT(sub_8034270),
    VTABLE_SLOT(DrawActor),
    VTABLE_SLOT(nullsub_38),
    VTABLE_SLOT(sub_803436C),
    VTABLE_SLOT(sub_803B5DC),
};

/* Used by counter_selector.c (sub_803716C), graphics_loading_35780.c,
 * graphics_loading_35d1c.c, graphics_loading_3686c.c (sub_8036CF4). */
const struct vtable_slot gStaticData_087E55C4[4] = {
    VTABLE_SLOT(NULL),
    VTABLE_SLOT(sub_803716C),
    VTABLE_SLOT(sub_8036EC4),
    VTABLE_SLOT(sub_8036FBC),
};
