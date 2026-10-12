#include "yeti.hpp"
#include "vehicle.hpp"

extern "C" {
#include "core.h"
#include "actor_anim.h"
}

/*
 * ROM 0x0817AA6C-0x0817C1C0: a palette and two boxes of the
 * UpdateYeti actor, then the data of actor categories 3-6 (the family
 * whose frames are the gJetpackSpriteSheet sheet): their two OBJ
 * palettes, the 47-record animation table gCategoryFamily1AnimTable
 * (include/actor_anim.h) and its keyframe (table_A) and frame (table_B)
 * arrays. Linked in ROM order between data/data.s sections by
 * ldscript.txt - see docs/data.md, "Category families".
 *
 * Record 0's table_B points at compressed frames of set C
 * (rle_sprites_15a050.cpp), the others' at frames of the framed sheet
 * graphics/vehicles/jetpack/sprite_sheet/. Both are written with the offsets the
 * build generates.
 */

#include "vehicles/jetpack/player.h"
#include "vehicles/jetpack/sprite_sheet_frames.h"

extern const u8 gJetpackPlayerRleFrames[];

extern const struct anim_frame_record gJetpackPlayerKeyframes[6];
extern const u8 *const gJetpackPlayerFrames[90];
extern const struct anim_frame_record gJetpackShotKeyframes[1];
extern const u32 gJetpackShotFrames[3];
extern const struct anim_frame_record gJetpackPlaneKeyframes[6];
extern const u32 gJetpackPlaneFrames[32];
extern const struct anim_frame_record gJetpackBomberKeyframes[2];
extern const u32 gJetpackBomberFrames[20];
extern const struct anim_frame_record gJetpackCannonballKeyframes[1];
extern const u32 gJetpackCannonballFrames[1];
extern const struct anim_frame_record gBossFireballKeyframes[2];
extern const u32 gBossFireballFrames[18];
extern const struct anim_frame_record gHovercraftCannonKeyframes[3];
extern const u32 gHovercraftCannonFrames[26];
extern const struct anim_frame_record gHovercraftLauncherKeyframes[4];
extern const u32 gHovercraftLauncherFrames[31];
extern const struct anim_frame_record gHovercraftSideGunKeyframes[2];
extern const u32 gHovercraftSideGunFrames[1];
extern const struct anim_frame_record gHovercraftCannonFlashKeyframes[1];
extern const u32 gHovercraftCannonFlashFrames[6];
extern const struct anim_frame_record gJetpackCrateKeyframes[2];
extern const u32 gJetpackQuestionCrateFrames[9];
extern const u32 gJetpackTimeCrate1Frames[9];
extern const u32 gJetpackTimeCrate2Frames[9];
extern const u32 gJetpackTimeCrate3Frames[9];
extern const struct anim_frame_record gJetpackParachuteNitroKeyframes[2];
extern const u32 gJetpackParachuteNitroFrames[18];
extern const struct anim_frame_record gJetpackRocketKeyframes[3];
extern const u32 gJetpackRocketFrames[26];
extern const struct anim_frame_record gJetpackStopwatchKeyframes[2];
extern const u32 gJetpackStopwatchFrames[6];
extern const struct anim_frame_record gJetpackBalloonKeyframes[2];
extern const u32 gJetpackRedBalloonFrames[6];
extern const u32 gJetpackRedCrossBalloonFrames[6];
extern const u32 gJetpackYellowBalloonFrames[6];
extern const struct anim_frame_record gJetpackRingLeftKeyframes[1];
extern const struct anim_frame_record gJetpackRingRightKeyframes[1];
extern const u32 gJetpackRingFrames[6];
extern const struct anim_frame_record gJetpackCollectedWumpaKeyframes[1];
extern const u32 gJetpackCollectedWumpaFrames[14];
extern const struct anim_frame_record gJetpackExplosionKeyframes[1];
extern const u32 gJetpackExplosionFrames[10];
extern const struct anim_frame_record gJetpackCheckpointTextKeyframes[1];
extern const u32 gJetpackCheckpointTextFrames[3];

/* Yeti::palette (include/yeti.hpp): the 16-colour gradient
 * Yeti::UpdatePalette (yeti_update.cpp) DMAs to OBJ
 * palette 15, or fades towards. */
const u16 Yeti::palette[16] = {
    0x03E0, 0x3547, 0x24E5, 0x3DA9, 0x49EC, 0x1083, 0x0421, 0x522E,
    0x5A70, 0x62B2, 0x66D3, 0x6F15, 0x7757, 0x7FB9, 0x7FFC, 0x0000,
};

/* Yeti::box: Yeti::IsTouching's (yeti_graphics.cpp) hit box. */
const struct anim_box Yeti::box = { -28, -24, -2, 56, 90, 4 };

/* Yeti::catchBox: Yeti::Update's (yeti_update.cpp) hit box. */
const struct anim_box Yeti::catchBox = { -80, -40, -14, 160, 110, 16 };

/* The 256-colour OBJ palette InitActorCategory loads for these
 * categories. The second 0x200 bytes are zero; nothing reads them. */
extern const u16 gAirshipCategoryPalette[0x200] = {
    0x03E0, 0x00BD, 0x004A, 0x035D, 0x0071, 0x069A, 0x0DBB, 0x3C43, 0x0823, 0x198E, 0x2210, 0x10E8,
    0x194A, 0x4337, 0x36B4, 0x0C64, 0x03E0, 0x1CC6, 0x107F, 0x0D04, 0x0F9F, 0x094C, 0x0864, 0x05D4,
    0x0ABE, 0x227F, 0x09BE, 0x1D5F, 0x086B, 0x14DF, 0x0C9B, 0x0873, 0x03E0, 0x737C, 0x5EDA, 0x463B,
    0x7FFF, 0x5231, 0x35BC, 0x251E, 0x3DAE, 0x1CF7, 0x2530, 0x2D29, 0x14B0, 0x108C, 0x1086, 0x0022,
    0x03E0, 0x7FBA, 0x07BE, 0x7757, 0x569C, 0x6AD2, 0x029C, 0x7FFE, 0x624D, 0x09BC, 0x4A0D, 0x053A,
    0x0515, 0x050F, 0x001C, 0x0034, 0x03E0, 0x22DE, 0x32D8, 0x0E1D, 0x1A54, 0x09D2, 0x00FC, 0x0112,
    0x014C, 0x0058, 0x00C9, 0x0011, 0x000B, 0x0007, 0x0003, 0x3F7D, 0x03E0, 0x000E, 0x0014, 0x0019,
    0x001E, 0x0021, 0x04E7, 0x09CD, 0x14BF, 0x0E73, 0x1F58, 0x27FD, 0x35BF, 0x49ED, 0x4A0D, 0x35ED,
    0x03E0, 0x37FF, 0x23FE, 0x23DA, 0x1F78, 0x035D, 0x1B15, 0x1AB1, 0x57FF, 0x05DD, 0x4E0E, 0x164D,
    0x49ED, 0x0158, 0x0110, 0x00B6, 0x03E0, 0x7FBA, 0x07BE, 0x7757, 0x569C, 0x6AD2, 0x029C, 0x7FFE,
    0x624D, 0x09BC, 0x4A0D, 0x053A, 0x0515, 0x050F, 0x001C, 0x0034, 0x7C1F, 0x7B9B, 0x6F38, 0x62B4,
    0x4E0E, 0x3D8B, 0x2D29, 0x0161, 0x0206, 0x02CA, 0x032F, 0x03FF, 0x3BFF, 0x0142, 0x0000, 0x7FFF,
    0x03E0, 0x1CC6, 0x107F, 0x0D04, 0x0F9F, 0x094C, 0x0864, 0x05D4, 0x0ABE, 0x227F, 0x09BE, 0x1D5F,
    0x086B, 0x14DF, 0x0C9B, 0x0873, 0x03E0, 0x1CC6, 0x107F, 0x0D04, 0x0F9F, 0x094C, 0x0864, 0x05D4,
    0x0ABE, 0x227F, 0x09BE, 0x1D5F, 0x086B, 0x14DF, 0x0C9B, 0x0873, 0x03E0, 0x3BFF, 0x271F, 0x1E9C,
    0x18C6, 0x0DD2, 0x575F, 0x3EDE, 0x4297, 0x3A35, 0x2DB0, 0x3569, 0x571C, 0x3E12, 0x41CD, 0x35AD,
    0x53E0, 0x7E60, 0x5C1F, 0x03FF, 0x03E9, 0x25B3, 0x471C, 0x7FFF, 0x163F, 0x057A, 0x0116, 0x00B0,
    0x000B, 0x0000, 0x1248, 0x045B, 0x03E0, 0x03FF, 0x03DF, 0x03BF, 0x039F, 0x035F, 0x031F, 0x02FF,
    0x02BF, 0x029F, 0x12BF, 0x129F, 0x025F, 0x021F, 0x01DF, 0x0000, 0x03E0, 0x12FE, 0x0D34, 0x0850,
    0x4E2F, 0x1CC5, 0x3189, 0x3F1D, 0x1800, 0x0001, 0x039D, 0x779C, 0x05FD, 0x3DB8, 0x337E, 0x62DD,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
};

/* The 256-colour OBJ palette InitActorCategory loads for these
 * categories. The second 0x200 bytes are zero; nothing reads them. */
extern const u16 gHovercraftCategoryPalette[0x200] = {
    0x03E0, 0x00BD, 0x004A, 0x035D, 0x0071, 0x069A, 0x0DBB, 0x3C43, 0x0823, 0x198E, 0x2210, 0x10E8,
    0x194A, 0x4337, 0x36B4, 0x0C64, 0x03E0, 0x1CC6, 0x107F, 0x0D04, 0x0F9F, 0x094C, 0x0864, 0x05D4,
    0x0ABE, 0x227F, 0x09BE, 0x1D5F, 0x086B, 0x14DF, 0x0C9B, 0x0873, 0x03E0, 0x737C, 0x5EDA, 0x463B,
    0x7FFF, 0x5231, 0x35BC, 0x251E, 0x3DAE, 0x1CF7, 0x2530, 0x2D29, 0x14B0, 0x108C, 0x1086, 0x0022,
    0x03E0, 0x7FBA, 0x07BE, 0x7757, 0x569C, 0x6AD2, 0x029C, 0x7FFE, 0x624D, 0x09BC, 0x4A0D, 0x053A,
    0x0515, 0x050F, 0x001C, 0x0034, 0x03E0, 0x22DE, 0x32D8, 0x0E1D, 0x1A54, 0x09D2, 0x00FC, 0x0112,
    0x014C, 0x0058, 0x00C9, 0x0011, 0x000B, 0x0007, 0x0003, 0x3F7D, 0x03E0, 0x000E, 0x0014, 0x0019,
    0x001E, 0x0021, 0x04E7, 0x09CD, 0x14BF, 0x0E73, 0x1F58, 0x27FD, 0x35BF, 0x49ED, 0x4A0D, 0x35ED,
    0x03E0, 0x37FF, 0x23FE, 0x23DA, 0x1F78, 0x035D, 0x1B15, 0x1AB1, 0x57FF, 0x05DD, 0x4E0E, 0x164D,
    0x49ED, 0x0158, 0x0110, 0x00B6, 0x03E0, 0x7FBA, 0x07BE, 0x7757, 0x569C, 0x6AD2, 0x029C, 0x7FFE,
    0x624D, 0x09BC, 0x4A0D, 0x053A, 0x0515, 0x050F, 0x001C, 0x0034, 0x7C1F, 0x7B9B, 0x6F38, 0x62B4,
    0x4E0E, 0x3D8B, 0x2D29, 0x0161, 0x0206, 0x02CA, 0x032F, 0x03FF, 0x3BFF, 0x0142, 0x0000, 0x7FFF,
    0x03E0, 0x1CC6, 0x107F, 0x0D04, 0x0F9F, 0x094C, 0x0864, 0x05D4, 0x0ABE, 0x227F, 0x09BE, 0x1D5F,
    0x086B, 0x14DF, 0x0C9B, 0x0873, 0x03E0, 0x66F5, 0x5250, 0x41EF, 0x25AF, 0x7FFF, 0x1CE9, 0x2D04,
    0x3988, 0x0C45, 0x35DE, 0x003C, 0x14B5, 0x0936, 0x15F5, 0x16FF, 0x03E0, 0x3BFF, 0x271F, 0x1E9C,
    0x18C6, 0x0DD2, 0x575F, 0x3EDE, 0x4297, 0x3A35, 0x2DB0, 0x3569, 0x571C, 0x3E12, 0x41CD, 0x35AD,
    0x53E0, 0x7E60, 0x5C1F, 0x03FF, 0x03E9, 0x25B3, 0x471C, 0x7FFF, 0x163F, 0x057A, 0x0116, 0x00B0,
    0x000B, 0x0000, 0x1248, 0x045B, 0x03E0, 0x03FF, 0x03DF, 0x03BF, 0x039F, 0x035F, 0x031F, 0x02FF,
    0x02BF, 0x029F, 0x12BF, 0x129F, 0x025F, 0x021F, 0x01DF, 0x0000, 0x03E0, 0x12FE, 0x0D34, 0x0850,
    0x4E2F, 0x1CC5, 0x3189, 0x3F1D, 0x1800, 0x0001, 0x039D, 0x779C, 0x05FD, 0x3DB8, 0x337E, 0x62DD,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
};

const struct anim_table_record gCategoryFamily1AnimTable[47] = {
    { 0,
      (struct anim_frame_record *)gJetpackPlayerKeyframes,
      (u32 *)gJetpackPlayerFrames,
      0,
      0x1C00,
      { -10, -20, -1, 20, 42, 3 },
      0,
      0 },
    { 1,
      (struct anim_frame_record *)gJetpackPlaneKeyframes,
      (u32 *)gJetpackPlaneFrames,
      2,
      0x2400,
      { -32, -16, -2, 64, 32, 4 },
      0,
      0 },
    { 2,
      (struct anim_frame_record *)gJetpackShotKeyframes,
      (u32 *)gJetpackShotFrames,
      1,
      0x2C00,
      { -12, -12, -2, 24, 24, 4 },
      0,
      0 },
    { 3,
      (struct anim_frame_record *)gJetpackCannonballKeyframes,
      (u32 *)gJetpackCannonballFrames,
      2,
      0x2A00,
      { -6, -6, -2, 12, 12, 4 },
      0,
      0 },
    { 4,
      (struct anim_frame_record *)gJetpackBomberKeyframes,
      (u32 *)gJetpackBomberFrames,
      2,
      0x2400,
      { -16, -16, -2, 32, 32, 4 },
      0,
      0 },
    { 5,
      (struct anim_frame_record *)gJetpackBomberKeyframes,
      (u32 *)gJetpackBomberFrames,
      2,
      0x2400,
      { -16, -16, -2, 32, 32, 4 },
      0,
      0 },
    { 6,
      (struct anim_frame_record *)gJetpackBomberKeyframes,
      (u32 *)gJetpackBomberFrames,
      2,
      0x2400,
      { -16, -16, -2, 32, 32, 4 },
      0,
      0 },
    { 7,
      (struct anim_frame_record *)gJetpackBomberKeyframes,
      (u32 *)gJetpackBomberFrames,
      2,
      0x2400,
      { -16, -16, -2, 32, 32, 4 },
      0,
      0 },
    { 8,
      (struct anim_frame_record *)gJetpackBomberKeyframes,
      (u32 *)gJetpackBomberFrames,
      2,
      0x2400,
      { -16, -16, -2, 32, 32, 4 },
      0,
      0 },
    { 9,
      (struct anim_frame_record *)gJetpackBomberKeyframes,
      (u32 *)gJetpackBomberFrames,
      2,
      0x2400,
      { -16, -16, -2, 32, 32, 4 },
      0,
      0 },
    { 10,
      (struct anim_frame_record *)gHovercraftCannonKeyframes,
      (u32 *)gHovercraftCannonFrames,
      10,
      0x2C00,
      { -8, -8, -2, 16, 16, 4 },
      0,
      0 },
    { 11,
      (struct anim_frame_record *)gHovercraftCannonKeyframes,
      (u32 *)gHovercraftCannonFrames,
      10,
      0x2C00,
      { -8, -8, -2, 16, 16, 4 },
      0,
      0 },
    { 12,
      (struct anim_frame_record *)gHovercraftLauncherKeyframes,
      (u32 *)gHovercraftLauncherFrames,
      10,
      0x2C00,
      { -8, -8, -2, 16, 16, 4 },
      0,
      0 },
    { 13,
      (struct anim_frame_record *)gHovercraftSideGunKeyframes,
      (u32 *)gHovercraftSideGunFrames,
      10,
      0x2C00,
      { -8, -8, -2, 16, 16, 4 },
      0,
      0 },
    { 14,
      (struct anim_frame_record *)gHovercraftCannonFlashKeyframes,
      (u32 *)gHovercraftCannonFlashFrames,
      4,
      0x2C00,
      { -8, -8, -2, 16, 16, 4 },
      0,
      0 },
    { 0,
      (struct anim_frame_record *)gHovercraftCannonKeyframes,
      (u32 *)gHovercraftCannonFrames,
      10,
      0x2C00,
      { -8, -8, -2, 16, 16, 4 },
      0,
      0 },
    { 16,
      (struct anim_frame_record *)gHovercraftCannonKeyframes,
      (u32 *)gHovercraftCannonFrames,
      10,
      0x2C00,
      { -8, -8, -2, 16, 16, 4 },
      0,
      0 },
    { 17,
      (struct anim_frame_record *)gHovercraftCannonKeyframes,
      (u32 *)gHovercraftCannonFrames,
      10,
      0x2C00,
      { -8, -8, -2, 16, 16, 4 },
      0,
      0 },
    { 18,
      (struct anim_frame_record *)gHovercraftCannonKeyframes,
      (u32 *)gHovercraftCannonFrames,
      10,
      0x2C00,
      { -8, -8, -2, 16, 16, 4 },
      0,
      0 },
    { 19,
      (struct anim_frame_record *)gJetpackCrateKeyframes,
      (u32 *)gJetpackQuestionCrateFrames,
      7,
      0x2400,
      { -11, -16, -2, 22, 22, 4 },
      0,
      0 },
    { 20,
      (struct anim_frame_record *)gJetpackCrateKeyframes,
      (u32 *)gJetpackQuestionCrateFrames,
      7,
      0x2400,
      { -11, -16, -2, 22, 22, 4 },
      0,
      0 },
    { 21,
      (struct anim_frame_record *)gJetpackCrateKeyframes,
      (u32 *)gJetpackQuestionCrateFrames,
      7,
      0x2400,
      { -11, -16, -2, 22, 22, 4 },
      0,
      0 },
    { 22,
      (struct anim_frame_record *)gJetpackCrateKeyframes,
      (u32 *)gJetpackQuestionCrateFrames,
      7,
      0x2400,
      { -11, -16, -2, 22, 22, 4 },
      0,
      0 },
    { 23,
      (struct anim_frame_record *)gJetpackCrateKeyframes,
      (u32 *)gJetpackQuestionCrateFrames,
      7,
      0x2400,
      { -11, -16, -2, 22, 22, 4 },
      0,
      0 },
    { 24,
      (struct anim_frame_record *)gJetpackCrateKeyframes,
      (u32 *)gJetpackTimeCrate1Frames,
      5,
      0x2400,
      { -11, -16, -2, 22, 22, 4 },
      0,
      0 },
    { 25,
      (struct anim_frame_record *)gJetpackCrateKeyframes,
      (u32 *)gJetpackTimeCrate2Frames,
      5,
      0x2400,
      { -11, -16, -2, 22, 22, 4 },
      0,
      0 },
    { 26,
      (struct anim_frame_record *)gJetpackCrateKeyframes,
      (u32 *)gJetpackTimeCrate3Frames,
      5,
      0x2400,
      { -11, -16, -2, 22, 22, 4 },
      0,
      0 },
    { 27,
      (struct anim_frame_record *)gJetpackParachuteNitroKeyframes,
      (u32 *)gJetpackParachuteNitroFrames,
      8,
      0x2400,
      { -21, 5, -2, 32, 31, 4 },
      0,
      0 },
    { 28,
      (struct anim_frame_record *)gJetpackRocketKeyframes,
      (u32 *)gJetpackRocketFrames,
      5,
      0x2400,
      { -4, -21, -2, 8, 42, 4 },
      0,
      0 },
    { 29,
      (struct anim_frame_record *)gJetpackStopwatchKeyframes,
      (u32 *)gJetpackStopwatchFrames,
      5,
      0x2400,
      { -17, 4, -2, 25, 24, 4 },
      0,
      0 },
    { 30,
      (struct anim_frame_record *)gJetpackRocketKeyframes,
      (u32 *)gJetpackRocketFrames,
      5,
      0x2400,
      { -17, 4, -2, 25, 24, 4 },
      0,
      0 },
    { 31,
      (struct anim_frame_record *)gJetpackRingLeftKeyframes,
      (u32 *)gJetpackRingFrames,
      11,
      0x1C00,
      { 3, -19, -2, 50, 38, 4 },
      -7168,
      0 },
    { 32,
      (struct anim_frame_record *)gHovercraftCannonKeyframes,
      (u32 *)gHovercraftCannonFrames,
      10,
      0x2C00,
      { -8, -8, -2, 16, 16, 4 },
      0,
      0 },
    { 33,
      (struct anim_frame_record *)gHovercraftCannonKeyframes,
      (u32 *)gHovercraftCannonFrames,
      10,
      0x2C00,
      { -8, -8, -2, 16, 16, 4 },
      0,
      0 },
    { 34,
      (struct anim_frame_record *)gHovercraftCannonKeyframes,
      (u32 *)gHovercraftCannonFrames,
      10,
      0x2C00,
      { -8, -8, -2, 16, 16, 4 },
      0,
      0 },
    { 35,
      (struct anim_frame_record *)gHovercraftCannonKeyframes,
      (u32 *)gHovercraftCannonFrames,
      10,
      0x2C00,
      { -8, -8, -2, 16, 16, 4 },
      0,
      0 },
    { 36,
      (struct anim_frame_record *)gHovercraftCannonKeyframes,
      (u32 *)gHovercraftCannonFrames,
      10,
      0x2C00,
      { -8, -8, -2, 16, 16, 4 },
      0,
      0 },
    { 37,
      (struct anim_frame_record *)gHovercraftCannonKeyframes,
      (u32 *)gHovercraftCannonFrames,
      10,
      0x2C00,
      { -8, -8, -2, 16, 16, 4 },
      0,
      0 },
    { 38,
      (struct anim_frame_record *)gBossFireballKeyframes,
      (u32 *)gBossFireballFrames,
      3,
      0x1C00,
      { -8, -8, -2, 16, 16, 4 },
      0,
      0 },
    { 39,
      (struct anim_frame_record *)gBossFireballKeyframes,
      (u32 *)gBossFireballFrames,
      3,
      0x1C00,
      { -8, -8, -2, 16, 16, 4 },
      0,
      0 },
    { 40,
      (struct anim_frame_record *)gJetpackBalloonKeyframes,
      (u32 *)gJetpackRedBalloonFrames,
      5,
      0x2400,
      { -17, -38, -2, 53, 16, 4 },
      0,
      0 },
    { 41,
      (struct anim_frame_record *)gJetpackBalloonKeyframes,
      (u32 *)gJetpackYellowBalloonFrames,
      6,
      0x2400,
      { -17, -38, -2, 53, 16, 4 },
      0,
      0 },
    { 42,
      (struct anim_frame_record *)gJetpackBalloonKeyframes,
      (u32 *)gJetpackRedCrossBalloonFrames,
      7,
      0x2400,
      { -17, -38, -2, 53, 16, 4 },
      0,
      0 },
    { 43,
      (struct anim_frame_record *)gJetpackRingRightKeyframes,
      (u32 *)gJetpackRingFrames,
      11,
      0x1C00,
      { 0, 0, 0, 0, 0, 0 },
      7168,
      0 },
    { 44,
      (struct anim_frame_record *)gJetpackCollectedWumpaKeyframes,
      (u32 *)gJetpackCollectedWumpaFrames,
      9,
      0x1C00,
      { 0, 0, 0, 0, 0, 0 },
      0,
      0 },
    { 45,
      (struct anim_frame_record *)gJetpackExplosionKeyframes,
      (u32 *)gJetpackExplosionFrames,
      4,
      0x2C00,
      { 0, 0, 0, 0, 0, 0 },
      0,
      0 },
    { 46,
      (struct anim_frame_record *)gJetpackCheckpointTextKeyframes,
      (u32 *)gJetpackCheckpointTextFrames,
      9,
      0x2C00,
      { 0, 0, 0, 0, 0, 0 },
      0,
      0 },
};

const struct anim_frame_record gJetpackPlayerKeyframes[6] = {
    { 64, 0, 18, 0, 0x0, { 0, 0 } },   { 85, 18, 18, 0, 0x0, { 0, 0 } },
    { 85, 36, 18, 0, 0x0, { 0, 0 } },  { 64, 54, 18, 17, 0x0, { 0, 0 } },
    { 42, 72, 18, 17, 0x0, { 0, 0 } }, { 42, 72, 8, 7, 0x0, { 0, 0 } },
};

const u8 *const gJetpackPlayerFrames[90] = {
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_018,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_019,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_020,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_021,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_022,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_023,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_024,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_025,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_026,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_027,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_028,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_029,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_030,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_031,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_032,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_033,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_034,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_035,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_036,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_037,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_038,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_039,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_040,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_041,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_042,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_043,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_044,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_045,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_046,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_047,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_048,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_049,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_050,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_051,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_052,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_053,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_054,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_055,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_056,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_057,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_058,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_059,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_060,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_061,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_062,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_063,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_064,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_065,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_066,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_067,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_068,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_069,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_070,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_071,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_000,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_001,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_002,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_003,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_004,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_005,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_006,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_007,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_008,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_009,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_010,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_011,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_012,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_013,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_014,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_015,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_016,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_017,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_072,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_073,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_074,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_075,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_076,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_077,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_078,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_079,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_079,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_079,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_079,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_079,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_079,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_079,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_079,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_079,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_079,
    gJetpackPlayerRleFrames + RLE_SPRITES_JETPACK_PLAYER_FRAME_079,
};

const struct anim_frame_record gJetpackShotKeyframes[1] = {
    { 64, 0, 3, 0, 0x0, { 0, 0 } },
};

const u32 gJetpackShotFrames[3] = {
    FRAMED_JETPACK_SHOT_00,
    FRAMED_JETPACK_SHOT_01,
    FRAMED_JETPACK_SHOT_02,
};

const struct anim_frame_record gJetpackPlaneKeyframes[6] = {
    { 64, 0, 3, 0, 0x0, { 0, 0 } },  { 64, 3, 6, 5, 0x0, { 0, 0 } },
    { 64, 9, 7, 0, 0x0, { 0, 0 } },  { 64, 16, 3, 0, 0x0, { 0, 0 } },
    { 64, 19, 6, 5, 0x0, { 0, 0 } }, { 64, 25, 7, 0, 0x0, { 0, 0 } },
};

const u32 gJetpackPlaneFrames[32] = {
    FRAMED_JETPACK_PLANE_00, FRAMED_JETPACK_PLANE_01, FRAMED_JETPACK_PLANE_02, FRAMED_JETPACK_PLANE_10,
    FRAMED_JETPACK_PLANE_11, FRAMED_JETPACK_PLANE_12, FRAMED_JETPACK_PLANE_13, FRAMED_JETPACK_PLANE_14,
    FRAMED_JETPACK_PLANE_15, FRAMED_JETPACK_PLANE_03, FRAMED_JETPACK_PLANE_04, FRAMED_JETPACK_PLANE_05,
    FRAMED_JETPACK_PLANE_06, FRAMED_JETPACK_PLANE_07, FRAMED_JETPACK_PLANE_08, FRAMED_JETPACK_PLANE_09,
    FRAMED_JETPACK_PLANE_16, FRAMED_JETPACK_PLANE_17, FRAMED_JETPACK_PLANE_18, FRAMED_JETPACK_PLANE_26,
    FRAMED_JETPACK_PLANE_27, FRAMED_JETPACK_PLANE_28, FRAMED_JETPACK_PLANE_29, FRAMED_JETPACK_PLANE_30,
    FRAMED_JETPACK_PLANE_31, FRAMED_JETPACK_PLANE_19, FRAMED_JETPACK_PLANE_20, FRAMED_JETPACK_PLANE_21,
    FRAMED_JETPACK_PLANE_22, FRAMED_JETPACK_PLANE_23, FRAMED_JETPACK_PLANE_24, FRAMED_JETPACK_PLANE_25,
};

const struct anim_frame_record gJetpackBomberKeyframes[2] = {
    { 64, 0, 10, 0, 0x0, { 0, 0 } },
    { 51, 10, 10, 9, 0x0, { 0, 0 } },
};

const u32 gJetpackBomberFrames[20] = {
    FRAMED_JETPACK_BOMBER_00,    FRAMED_JETPACK_BOMBER_01,    FRAMED_JETPACK_BOMBER_02,
    FRAMED_JETPACK_BOMBER_03,    FRAMED_JETPACK_BOMBER_04,    FRAMED_JETPACK_BOMBER_05,
    FRAMED_JETPACK_BOMBER_06,    FRAMED_JETPACK_BOMBER_07,    FRAMED_JETPACK_BOMBER_08,
    FRAMED_JETPACK_BOMBER_09,    FRAMED_JETPACK_EXPLOSION_00, FRAMED_JETPACK_EXPLOSION_01,
    FRAMED_JETPACK_EXPLOSION_02, FRAMED_JETPACK_EXPLOSION_03, FRAMED_JETPACK_EXPLOSION_04,
    FRAMED_JETPACK_EXPLOSION_05, FRAMED_JETPACK_EXPLOSION_06, FRAMED_JETPACK_EXPLOSION_07,
    FRAMED_JETPACK_EXPLOSION_08, FRAMED_JETPACK_EXPLOSION_09,
};

const struct anim_frame_record gJetpackCannonballKeyframes[1] = {
    { 64, 0, 1, 0, 0x0, { 0, 0 } },
};

const u32 gJetpackCannonballFrames[1] = {
    FRAMED_JETPACK_CANNONBALL_00,
};

const struct anim_frame_record gBossFireballKeyframes[2] = {
    { 42, 0, 8, 0, 0x0, { 0, 0 } },
    { 42, 8, 10, 9, 0x0, { 0, 0 } },
};

const u32 gBossFireballFrames[18] = {
    FRAMED_JETPACK_FIREBALL_00,       FRAMED_JETPACK_FIREBALL_01,
    FRAMED_JETPACK_FIREBALL_02,       FRAMED_JETPACK_FIREBALL_03,
    FRAMED_JETPACK_FIREBALL_04,       FRAMED_JETPACK_FIREBALL_05,
    FRAMED_JETPACK_FIREBALL_06,       FRAMED_JETPACK_FIREBALL_07,
    FRAMED_JETPACK_FIREBALL_BURST_00, FRAMED_JETPACK_FIREBALL_BURST_01,
    FRAMED_JETPACK_FIREBALL_BURST_02, FRAMED_JETPACK_FIREBALL_BURST_03,
    FRAMED_JETPACK_FIREBALL_BURST_04, FRAMED_JETPACK_FIREBALL_BURST_05,
    FRAMED_JETPACK_FIREBALL_BURST_06, FRAMED_JETPACK_FIREBALL_BURST_07,
    FRAMED_JETPACK_FIREBALL_BURST_08, FRAMED_JETPACK_FIREBALL_BURST_09,
};

const struct anim_frame_record gHovercraftCannonKeyframes[3] = {
    { 64, 0, 1, 0, 0x0, { 0, 0 } },
    { 64, 0, 11, 0, 0x0, { 0, 0 } },
    { 64, 11, 14, 11, 0x0, { 0, 0 } },
};

const u32 gHovercraftCannonFrames[26] = {
    FRAMED_JETPACK_HOVERCRAFT_CANNON_00, FRAMED_JETPACK_HOVERCRAFT_CANNON_00,
    FRAMED_JETPACK_HOVERCRAFT_CANNON_01, FRAMED_JETPACK_HOVERCRAFT_CANNON_02,
    FRAMED_JETPACK_HOVERCRAFT_CANNON_03, FRAMED_JETPACK_HOVERCRAFT_CANNON_04,
    FRAMED_JETPACK_HOVERCRAFT_CANNON_05, FRAMED_JETPACK_HOVERCRAFT_CANNON_06,
    FRAMED_JETPACK_HOVERCRAFT_CANNON_07, FRAMED_JETPACK_HOVERCRAFT_CANNON_08,
    FRAMED_JETPACK_HOVERCRAFT_CANNON_09, FRAMED_JETPACK_HOVERCRAFT_CANNON_10,
    FRAMED_JETPACK_HOVERCRAFT_CANNON_11, FRAMED_JETPACK_HOVERCRAFT_CANNON_12,
    FRAMED_JETPACK_HOVERCRAFT_CANNON_13, FRAMED_JETPACK_HOVERCRAFT_CANNON_14,
    FRAMED_JETPACK_HOVERCRAFT_CANNON_15, FRAMED_JETPACK_HOVERCRAFT_CANNON_16,
    FRAMED_JETPACK_HOVERCRAFT_CANNON_17, FRAMED_JETPACK_HOVERCRAFT_CANNON_18,
    FRAMED_JETPACK_HOVERCRAFT_CANNON_19, FRAMED_JETPACK_HOVERCRAFT_CANNON_20,
    FRAMED_JETPACK_HOVERCRAFT_CANNON_21, FRAMED_JETPACK_HOVERCRAFT_CANNON_22,
    FRAMED_JETPACK_HOVERCRAFT_CANNON_23, FRAMED_JETPACK_HOVERCRAFT_CANNON_24,
};

const struct anim_frame_record gHovercraftLauncherKeyframes[4] = {
    { 64, 0, 1, 0, 0x0, { 0, 0 } },
    { 64, 1, 8, 7, 0x0, { 0, 0 } },
    { 64, 9, 8, 7, 0x0, { 0, 0 } },
    { 64, 17, 14, 0, 0x0, { 0, 0 } },
};

const u32 gHovercraftLauncherFrames[31] = {
    FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_00, FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_00,
    FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_01, FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_02,
    FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_03, FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_04,
    FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_05, FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_06,
    FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_07, FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_07,
    FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_06, FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_05,
    FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_04, FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_03,
    FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_02, FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_01,
    FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_00, FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_08,
    FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_09, FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_10,
    FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_11, FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_12,
    FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_13, FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_14,
    FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_15, FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_16,
    FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_17, FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_18,
    FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_19, FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_20,
    FRAMED_JETPACK_HOVERCRAFT_LAUNCHER_21,
};

const struct anim_frame_record gHovercraftSideGunKeyframes[2] = {
    { 64, 0, 1, 0, 0x0, { 0, 0 } },
    { 64, 0, 1, 0, 0x1000, { 0, 0 } },
};

const u32 gHovercraftSideGunFrames[1] = {
    FRAMED_JETPACK_HOVERCRAFT_SIDE_GUN_00,
};

const struct anim_frame_record gHovercraftCannonFlashKeyframes[1] = {
    { 64, 0, 6, 0, 0x0, { 0, 0 } },
};

const u32 gHovercraftCannonFlashFrames[6] = {
    FRAMED_JETPACK_HOVERCRAFT_CANNON_FLASH_00, FRAMED_JETPACK_HOVERCRAFT_CANNON_FLASH_01,
    FRAMED_JETPACK_HOVERCRAFT_CANNON_FLASH_02, FRAMED_JETPACK_HOVERCRAFT_CANNON_FLASH_03,
    FRAMED_JETPACK_HOVERCRAFT_CANNON_FLASH_04, FRAMED_JETPACK_HOVERCRAFT_CANNON_FLASH_05,
};

const struct anim_frame_record gJetpackCrateKeyframes[2] = {
    { 42, 0, 1, 0, 0x0, { 0, 0 } },
    { 42, 1, 8, 7, 0x0, { 0, 0 } },
};

const u32 gJetpackQuestionCrateFrames[9] = {
    FRAMED_JETPACK_QUESTION_CRATE_00, FRAMED_JETPACK_CRATE_SHARDS_00, FRAMED_JETPACK_CRATE_SHARDS_01,
    FRAMED_JETPACK_CRATE_SHARDS_02,   FRAMED_JETPACK_CRATE_SHARDS_03, FRAMED_JETPACK_CRATE_SHARDS_04,
    FRAMED_JETPACK_CRATE_SHARDS_05,   FRAMED_JETPACK_CRATE_SHARDS_06, FRAMED_JETPACK_CRATE_SHARDS_07,
};

const u32 gJetpackTimeCrate1Frames[9] = {
    FRAMED_JETPACK_TIME_CRATE_1_00,      FRAMED_JETPACK_TIME_CRATE_SHARDS_00,
    FRAMED_JETPACK_TIME_CRATE_SHARDS_01, FRAMED_JETPACK_TIME_CRATE_SHARDS_02,
    FRAMED_JETPACK_TIME_CRATE_SHARDS_03, FRAMED_JETPACK_TIME_CRATE_SHARDS_04,
    FRAMED_JETPACK_TIME_CRATE_SHARDS_05, FRAMED_JETPACK_TIME_CRATE_SHARDS_06,
    FRAMED_JETPACK_TIME_CRATE_SHARDS_07,
};

const u32 gJetpackTimeCrate2Frames[9] = {
    FRAMED_JETPACK_TIME_CRATE_2_00,      FRAMED_JETPACK_TIME_CRATE_SHARDS_00,
    FRAMED_JETPACK_TIME_CRATE_SHARDS_01, FRAMED_JETPACK_TIME_CRATE_SHARDS_02,
    FRAMED_JETPACK_TIME_CRATE_SHARDS_03, FRAMED_JETPACK_TIME_CRATE_SHARDS_04,
    FRAMED_JETPACK_TIME_CRATE_SHARDS_05, FRAMED_JETPACK_TIME_CRATE_SHARDS_06,
    FRAMED_JETPACK_TIME_CRATE_SHARDS_07,
};

const u32 gJetpackTimeCrate3Frames[9] = {
    FRAMED_JETPACK_TIME_CRATE_3_00,      FRAMED_JETPACK_TIME_CRATE_SHARDS_00,
    FRAMED_JETPACK_TIME_CRATE_SHARDS_01, FRAMED_JETPACK_TIME_CRATE_SHARDS_02,
    FRAMED_JETPACK_TIME_CRATE_SHARDS_03, FRAMED_JETPACK_TIME_CRATE_SHARDS_04,
    FRAMED_JETPACK_TIME_CRATE_SHARDS_05, FRAMED_JETPACK_TIME_CRATE_SHARDS_06,
    FRAMED_JETPACK_TIME_CRATE_SHARDS_07,
};

const struct anim_frame_record gJetpackParachuteNitroKeyframes[2] = {
    { 42, 0, 10, 0, 0x0, { 0, 0 } },
    { 42, 10, 8, 7, 0x0, { 0, 0 } },
};

const u32 gJetpackParachuteNitroFrames[18] = {
    FRAMED_JETPACK_PARACHUTE_NITRO_00, FRAMED_JETPACK_PARACHUTE_NITRO_01,
    FRAMED_JETPACK_PARACHUTE_NITRO_02, FRAMED_JETPACK_PARACHUTE_NITRO_03,
    FRAMED_JETPACK_PARACHUTE_NITRO_04, FRAMED_JETPACK_PARACHUTE_NITRO_05,
    FRAMED_JETPACK_PARACHUTE_NITRO_04, FRAMED_JETPACK_PARACHUTE_NITRO_03,
    FRAMED_JETPACK_PARACHUTE_NITRO_02, FRAMED_JETPACK_PARACHUTE_NITRO_01,
    FRAMED_JETPACK_PARACHUTE_NITRO_06, FRAMED_JETPACK_PARACHUTE_NITRO_07,
    FRAMED_JETPACK_PARACHUTE_NITRO_08, FRAMED_JETPACK_PARACHUTE_NITRO_09,
    FRAMED_JETPACK_PARACHUTE_NITRO_10, FRAMED_JETPACK_PARACHUTE_NITRO_11,
    FRAMED_JETPACK_PARACHUTE_NITRO_12, FRAMED_JETPACK_PARACHUTE_NITRO_13,
};

const struct anim_frame_record gJetpackRocketKeyframes[3] = {
    { 42, 0, 5, 0, 0x0, { 0, 0 } },
    { 34, 5, 11, 9, 0x0, { 0, 0 } },
    { 42, 16, 10, 9, 0x0, { 0, 0 } },
};

const u32 gJetpackRocketFrames[26] = {
    FRAMED_JETPACK_ROCKET_00,       FRAMED_JETPACK_ROCKET_01,       FRAMED_JETPACK_ROCKET_02,
    FRAMED_JETPACK_ROCKET_03,       FRAMED_JETPACK_ROCKET_04,       FRAMED_JETPACK_ROCKET_BURST_00,
    FRAMED_JETPACK_ROCKET_BURST_01, FRAMED_JETPACK_ROCKET_BURST_02, FRAMED_JETPACK_ROCKET_BURST_03,
    FRAMED_JETPACK_ROCKET_BURST_04, FRAMED_JETPACK_ROCKET_BURST_05, FRAMED_JETPACK_ROCKET_BURST_06,
    FRAMED_JETPACK_ROCKET_BURST_07, FRAMED_JETPACK_ROCKET_BURST_08, FRAMED_JETPACK_ROCKET_BURST_09,
    FRAMED_JETPACK_ROCKET_BURST_10, FRAMED_JETPACK_EXPLOSION_00,    FRAMED_JETPACK_EXPLOSION_01,
    FRAMED_JETPACK_EXPLOSION_02,    FRAMED_JETPACK_EXPLOSION_03,    FRAMED_JETPACK_EXPLOSION_04,
    FRAMED_JETPACK_EXPLOSION_05,    FRAMED_JETPACK_EXPLOSION_06,    FRAMED_JETPACK_EXPLOSION_07,
    FRAMED_JETPACK_EXPLOSION_08,    FRAMED_JETPACK_EXPLOSION_09,
};

/* Record 29: the time-trial stopwatch, whole and then shattering. Kind 29
 * only spawns once the level's crystal is saved, and breaking it calls
 * StartTimeTrial (jetpack_crates.cpp). */
const struct anim_frame_record gJetpackStopwatchKeyframes[2] = {
    { 42, 0, 1, 0, 0x0, { 0, 0 } },
    { 42, 1, 5, 4, 0x0, { 0, 0 } },
};

const u32 gJetpackStopwatchFrames[6] = {
    FRAMED_JETPACK_STOPWATCH_00, FRAMED_JETPACK_STOPWATCH_01, FRAMED_JETPACK_STOPWATCH_02,
    FRAMED_JETPACK_STOPWATCH_03, FRAMED_JETPACK_STOPWATCH_04, FRAMED_JETPACK_STOPWATCH_05,
};

const struct anim_frame_record gJetpackBalloonKeyframes[2] = {
    { 42, 0, 1, 0, 0x0, { 0, 0 } },
    { 42, 1, 5, 4, 0x0, { 0, 0 } },
};

const u32 gJetpackRedBalloonFrames[6] = {
    FRAMED_JETPACK_BALLOON_RED_00, FRAMED_JETPACK_BALLOON_RED_01, FRAMED_JETPACK_BALLOON_RED_02,
    FRAMED_JETPACK_BALLOON_RED_03, FRAMED_JETPACK_BALLOON_RED_04, FRAMED_JETPACK_BALLOON_RED_05,
};

const u32 gJetpackRedCrossBalloonFrames[6] = {
    FRAMED_JETPACK_BALLOON_RED_CROSS_00, FRAMED_JETPACK_BALLOON_RED_CROSS_01,
    FRAMED_JETPACK_BALLOON_RED_CROSS_02, FRAMED_JETPACK_BALLOON_RED_CROSS_03,
    FRAMED_JETPACK_BALLOON_RED_CROSS_04, FRAMED_JETPACK_BALLOON_RED_CROSS_05,
};

const u32 gJetpackYellowBalloonFrames[6] = {
    FRAMED_JETPACK_BALLOON_YELLOW_00, FRAMED_JETPACK_BALLOON_YELLOW_01,
    FRAMED_JETPACK_BALLOON_YELLOW_02, FRAMED_JETPACK_BALLOON_YELLOW_03,
    FRAMED_JETPACK_BALLOON_YELLOW_04, FRAMED_JETPACK_BALLOON_YELLOW_05,
};

/* Records 31/43: the left half of a ring and its mirrored (hflip) right
 * half, which CreateJetpackActor spawns with it. */
const struct anim_frame_record gJetpackRingLeftKeyframes[1] = {
    { 42, 0, 6, 0, 0x0, { 0, 0 } },
};

const struct anim_frame_record gJetpackRingRightKeyframes[1] = {
    { 42, 0, 6, 0, 0x1000, { 0, 0 } },
};

const u32 gJetpackRingFrames[6] = {
    FRAMED_JETPACK_RING_00, FRAMED_JETPACK_RING_00, FRAMED_JETPACK_RING_00,
    FRAMED_JETPACK_RING_01, FRAMED_JETPACK_RING_02, FRAMED_JETPACK_RING_03,
};

const struct anim_frame_record gJetpackCollectedWumpaKeyframes[1] = {
    { 85, 0, 14, 0, 0x0, { 0, 0 } },
};

const u32 gJetpackCollectedWumpaFrames[14] = {
    FRAMED_JETPACK_COLLECTED_WUMPA_00, FRAMED_JETPACK_COLLECTED_WUMPA_01,
    FRAMED_JETPACK_COLLECTED_WUMPA_02, FRAMED_JETPACK_COLLECTED_WUMPA_03,
    FRAMED_JETPACK_COLLECTED_WUMPA_04, FRAMED_JETPACK_COLLECTED_WUMPA_05,
    FRAMED_JETPACK_COLLECTED_WUMPA_06, FRAMED_JETPACK_COLLECTED_WUMPA_07,
    FRAMED_JETPACK_COLLECTED_WUMPA_08, FRAMED_JETPACK_COLLECTED_WUMPA_09,
    FRAMED_JETPACK_COLLECTED_WUMPA_10, FRAMED_JETPACK_COLLECTED_WUMPA_11,
    FRAMED_JETPACK_COLLECTED_WUMPA_12, FRAMED_JETPACK_COLLECTED_WUMPA_13,
};

const struct anim_frame_record gJetpackExplosionKeyframes[1] = {
    { 51, 0, 10, 9, 0x0, { 0, 0 } },
};

const u32 gJetpackExplosionFrames[10] = {
    FRAMED_JETPACK_EXPLOSION_00, FRAMED_JETPACK_EXPLOSION_01, FRAMED_JETPACK_EXPLOSION_02,
    FRAMED_JETPACK_EXPLOSION_03, FRAMED_JETPACK_EXPLOSION_04, FRAMED_JETPACK_EXPLOSION_05,
    FRAMED_JETPACK_EXPLOSION_06, FRAMED_JETPACK_EXPLOSION_07, FRAMED_JETPACK_EXPLOSION_08,
    FRAMED_JETPACK_EXPLOSION_09,
};

const struct anim_frame_record gJetpackCheckpointTextKeyframes[1] = {
    { 8, 0, 3, 0, 0x0, { 0, 0 } },
};

const u32 gJetpackCheckpointTextFrames[3] = {
    FRAMED_JETPACK_CHECKPOINT_TEXT_00,
    FRAMED_JETPACK_CHECKPOINT_TEXT_00,
    FRAMED_JETPACK_CHECKPOINT_TEXT_00,
};
