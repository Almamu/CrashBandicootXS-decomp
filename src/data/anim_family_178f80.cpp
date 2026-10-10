extern "C" {
#include "core.h"
#include "actor_self.h"
#include "actor_anim.h"
#include "frontend.h"
}

/*
 * ROM 0x08178F80-0x0817A6B8: the data of actor categories 0-2 (the family
 * whose frames are the gPolarSpriteSheet sheet): their OBJ palette,
 * the 41-record animation table gCategoryFamily0AnimTable
 * (include/actor_anim.h) and the keyframe (table_A) and frame (table_B)
 * arrays its records point at. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md, "Category families".
 *
 * Record 0's table_B points at compressed frames of set A
 * (rle_sprites_0c2758.c), the others' at frames of the framed sheet
 * graphics/unknown/00_0b2120/ (byte offsets into its decompressed
 * data). Both kinds are written with the offsets the build generates,
 * so edited frames move them along. The arrays are cut at the next
 * array a record points at; nothing but the records references them.
 */

#include "rle_sprites/0c2758_frames.h"
#include "unknown/00_0b2120_frames.h"

extern const u8 gPolarPlayerRleFrames[];

extern const struct anim_frame_record gPolarPlayerKeyframes[13];
extern const u8 *const gPolarPlayerFrames[156];
extern const struct anim_frame_record gRiderlessPolarKeyframes[2];
extern const u32 gRiderlessPolarFrames[10];
extern const struct anim_frame_record gPolarWumpaKeyframes[1];
extern const u32 gPolarWumpaFrames[14];
extern const struct anim_frame_record gPolarLauncherKeyframes[2];
extern const u32 gPolarLauncherFrames[20];
extern const struct anim_frame_record gPolarBoostPadKeyframes[3];
extern const u32 gPolarBoostPadFrames[8];
extern const struct anim_frame_record gPolarElectricFenceKeyframes[2];
extern const u32 gPolarElectricFenceFrames[22];
extern const struct anim_frame_record gPolarIcicleKeyframes[24];
extern const u32 gPolarIcicleFrames[12];
extern const u32 gPolarObstacleFrames[1];
extern const struct anim_frame_record gPolarObstacleKeyframes[2];
extern const u32 gPolarGoalFrames[1];
extern const struct anim_frame_record gPolarGoalKeyframes[2];
extern const u32 gPolarPenguinFrames[9];
extern const struct anim_frame_record gPolarPenguinKeyframes[1];
extern const struct anim_frame_record gPolarBasicCrateKeyframes[19];
extern const u32 gPolarBasicCrateFrames[15];
extern const struct anim_frame_record gPolarNitroCrateKeyframes[19];
extern const u32 gPolarNitroCrateFrames[26];
extern const struct anim_frame_record gPolarCrateKeyframes[19];
extern const u32 gPolarTimeCrate1Frames[24];
extern const u32 gPolarTimeCrate2Frames[24];
extern const u32 gPolarTimeCrate3Frames[24];
extern const u32 gPolarQuestionCrateFrames[24];
extern const u32 gPolarLifeCrateFrames[24];
extern const u32 gPolarAkuAkuCrateFrames[24];
extern const u32 gPolarCheckpointCrateFrames[20];
extern const struct anim_frame_record gPolarCheckpointCrateKeyframes[4];
extern const u32 gPolarAkuAkuFrames[11];
extern const struct anim_frame_record gPolarAkuAkuKeyframes[2];
extern const struct anim_frame_record gPolarCheckpointTextKeyframes[1];
extern const u32 gPolarCheckpointTextFrames[3];

/* The 256-colour OBJ palette InitActorCategory loads for these
 * categories. The second 0x200 bytes are zero; nothing reads them. */
const u16 gPolarCategoryPalette[0x200] = {
    0x03E0, 0x01DD, 0x00DE, 0x0050, 0x00B7, 0x3461, 0x0005, 0x31DB, 0x18C8, 0x31B1, 0x294C, 0x52D8,
    0x3E32, 0x5F3B, 0x4675, 0x6BBF, 0x03E0, 0x5B9C, 0x033C, 0x05F1, 0x1657, 0x3697, 0x1DB3, 0x057A,
    0x15DE, 0x00B0, 0x0116, 0x08F1, 0x1179, 0x006B, 0x0076, 0x0072, 0x03E0, 0x0120, 0x0182, 0x01E4,
    0x0246, 0x02CA, 0x032F, 0x0777, 0x07DD, 0x0A73, 0x09EE, 0x3BFF, 0x7FFF, 0x1EFB, 0x1A39, 0x0915,
    0x03E0, 0x5D87, 0x1248, 0x0CC5, 0x5F7C, 0x42FA, 0x3237, 0x057A, 0x15DE, 0x2192, 0x1179, 0x04F6,
    0x00B0, 0x14EF, 0x0048, 0x4A5E, 0x03E0, 0x198B, 0x0FDE, 0x0276, 0x031E, 0x02DA, 0x01D1, 0x00E9,
    0x0063, 0x00EE, 0x05BD, 0x0072, 0x007C, 0x0010, 0x0017, 0x000B, 0x03E0, 0x00AF, 0x00D0, 0x00D0,
    0x0111, 0x0132, 0x0133, 0x0076, 0x0134, 0x01D1, 0x0116, 0x0179, 0x00ED, 0x05BE, 0x033B, 0x03DF,
    0x701F, 0x0380, 0x1700, 0x3EC1, 0x35CE, 0x4A52, 0x29E0, 0x2129, 0x1920, 0x10C6, 0x10E0, 0x0883,
    0x0CA0, 0x0860, 0x0440, 0x0000, 0x03E0, 0x1CC6, 0x107F, 0x0D04, 0x0F9F, 0x094C, 0x0864, 0x05D4,
    0x0ABE, 0x227F, 0x09BE, 0x1D5F, 0x086B, 0x14DF, 0x0C9B, 0x0873, 0x7C1F, 0x218A, 0x10C6, 0x25D0,
    0x02FF, 0x1D8E, 0x150A, 0x152D, 0x08CA, 0x09FF, 0x0464, 0x21F7, 0x1993, 0x0D53, 0x00CF, 0x055A,
    0x7C1F, 0x0A3E, 0x0488, 0x152E, 0x055A, 0x5AD4, 0x21B1, 0x14E8, 0x7F71, 0x4900, 0x6DA0, 0x7E85,
    0x7FFD, 0x15FC, 0x02FF, 0x218C, 0x6C1F, 0x7F78, 0x7379, 0x7B35, 0x72D4, 0x5ED4, 0x72D0, 0x5E70,
    0x5271, 0x6A8C, 0x5E2D, 0x4E0E, 0x5628, 0x4DCB, 0x7FDC, 0x49A5, 0x03E0, 0x77B9, 0x6356, 0x52D3,
    0x001F, 0x001F, 0x001F, 0x001F, 0x3E4F, 0x2DEC, 0x198A, 0x2569, 0x1128, 0x08E6, 0x0483, 0x7FFF,
    0x03E0, 0x3504, 0x3D45, 0x4DA8, 0x4566, 0x1860, 0x20C2, 0x28E4, 0x3126, 0x3DA9, 0x3568, 0x2504,
    0x3187, 0x462E, 0x3A6C, 0x0C00, 0x03E0, 0x023D, 0x0131, 0x05B4, 0x04AB, 0x4E25, 0x1862, 0x24A3,
    0x2CE4, 0x2679, 0x3F1E, 0x3569, 0x49EE, 0x6B17, 0x5693, 0x77BD, 0x35AC, 0x2192, 0x14EF, 0x04F6,
    0x1179, 0x023F, 0x03FF, 0x03E9, 0x1248, 0x42FA, 0x5D87, 0x7E60, 0x4414, 0x401F, 0x001C, 0x0000,
    0x53E0, 0x7E60, 0x5C1F, 0x03FF, 0x03E9, 0x25B3, 0x471C, 0x7FFF, 0x163F, 0x057A, 0x0116, 0x00B0,
    0x000B, 0x0000, 0x1248, 0x045B,
};

const struct anim_frame_record gPolarPlayerKeyframes[13] = {
    { 128, 0, 20, 0, 0x0, { 0, 0 } },  { 128, 20, 39, 0, 0x0, { 0, 0 } },
    { 128, 59, 20, 0, 0x0, { 0, 0 } }, { 64, 79, 9, 8, 0x0, { 0, 0 } },
    { 128, 88, 9, 8, 0x0, { 0, 0 } },  { 64, 97, 16, 0, 0x0, { 0, 0 } },
    { 64, 113, 8, 7, 0x0, { 0, 0 } },  { 64, 121, 11, 0, 0x0, { 0, 0 } },
    { 128, 132, 1, 0, 0x0, { 0, 0 } }, { 128, 133, 23, 22, 0x0, { 0, 0 } },
    { 64, 117, 4, 3, 0x0, { 0, 0 } },  { 64, 60, 2, 0, 0x0, { 0, 0 } },
    { 64, 0, 1, 0, 0x0, { 0, 0 } },
};

const u8 *const gPolarPlayerFrames[156] = {
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_000,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_001,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_002,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_003,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_004,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_005,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_006,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_007,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_008,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_009,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_010,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_011,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_012,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_013,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_014,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_015,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_016,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_017,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_018,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_019,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_020,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_021,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_022,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_023,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_024,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_025,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_026,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_027,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_028,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_029,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_030,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_031,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_032,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_033,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_034,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_035,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_036,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_037,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_038,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_039,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_040,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_041,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_042,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_043,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_044,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_045,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_046,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_047,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_048,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_049,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_050,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_051,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_052,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_053,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_054,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_055,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_056,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_057,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_058,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_059,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_060,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_061,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_062,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_063,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_064,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_065,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_066,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_067,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_068,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_069,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_070,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_071,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_072,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_073,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_074,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_075,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_076,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_077,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_078,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_079,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_080,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_081,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_082,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_083,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_084,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_085,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_086,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_087,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_088,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_089,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_090,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_091,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_092,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_093,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_094,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_095,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_096,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_136,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_137,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_138,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_139,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_140,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_141,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_142,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_143,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_144,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_145,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_146,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_147,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_148,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_149,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_150,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_151,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_020,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_021,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_022,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_023,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_097,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_098,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_099,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_100,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_101,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_102,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_103,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_104,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_105,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_106,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_107,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_108,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_109,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_110,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_111,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_135,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_112,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_113,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_114,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_115,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_116,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_117,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_118,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_119,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_120,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_121,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_122,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_123,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_124,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_125,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_126,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_127,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_128,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_129,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_130,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_131,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_132,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_133,
    gPolarPlayerRleFrames + RLE_SPRITES_0C2758_FRAME_134,
};

const struct anim_frame_record gRiderlessPolarKeyframes[2] = {
    { 64, 0, 9, 8, 0x0, { 0, 0 } },
    { 64, 9, 1, 0, 0x0, { 0, 0 } },
};

const u32 gRiderlessPolarFrames[10] = {
    FRAMED_0B2120_SMALL_CREATURE_00, FRAMED_0B2120_SMALL_CREATURE_01,
    FRAMED_0B2120_SMALL_CREATURE_02, FRAMED_0B2120_SMALL_CREATURE_03,
    FRAMED_0B2120_SMALL_CREATURE_04, FRAMED_0B2120_SMALL_CREATURE_05,
    FRAMED_0B2120_SMALL_CREATURE_06, FRAMED_0B2120_SMALL_CREATURE_07,
    FRAMED_0B2120_SMALL_CREATURE_08, FRAMED_0B2120_SMALL_CREATURE_09,
};

const struct anim_table_record gCategoryFamily0AnimTable[41] = {
    { 0,
      (struct anim_frame_record *)gPolarPlayerKeyframes,
      (u32 *)gPolarPlayerFrames,
      0,
      0x2F00,
      { -10, -20, -1, 20, 44, 3 },
      0,
      0 },
    { 1,
      (struct anim_frame_record *)gPolarBasicCrateKeyframes,
      (u32 *)gPolarBasicCrateFrames,
      1,
      0x313C,
      { -10, -10, -1, 20, 20, 3 },
      0,
      0 },
    { 2,
      (struct anim_frame_record *)gRiderlessPolarKeyframes,
      (u32 *)gRiderlessPolarFrames,
      0,
      0x2F00,
      { 0, 0, 0, 0, 0, 0 },
      0,
      0 },
    { 3,
      (struct anim_frame_record *)gPolarCheckpointCrateKeyframes,
      (u32 *)gPolarCheckpointCrateFrames,
      5,
      0x313C,
      { -12, -20, -2, 24, 30, 5 },
      0,
      256 },
    { 4,
      (struct anim_frame_record *)gPolarNitroCrateKeyframes,
      (u32 *)gPolarNitroCrateFrames,
      2,
      0x313C,
      { -10, -10, -1, 20, 20, 3 },
      0,
      0 },
    { 5,
      (struct anim_frame_record *)gPolarCrateKeyframes,
      (u32 *)gPolarTimeCrate1Frames,
      4,
      0x313C,
      { -10, -10, -1, 20, 20, 3 },
      0,
      0 },
    { 6,
      (struct anim_frame_record *)gPolarCrateKeyframes,
      (u32 *)gPolarTimeCrate2Frames,
      4,
      0x313C,
      { -10, -10, -1, 20, 20, 3 },
      0,
      0 },
    { 7,
      (struct anim_frame_record *)gPolarCrateKeyframes,
      (u32 *)gPolarTimeCrate3Frames,
      4,
      0x313C,
      { -10, -10, -1, 20, 20, 3 },
      0,
      0 },
    { 8,
      (struct anim_frame_record *)gPolarCrateKeyframes,
      (u32 *)gPolarLifeCrateFrames,
      3,
      0x313C,
      { -10, -10, -1, 20, 20, 3 },
      0,
      0 },
    { 9,
      (struct anim_frame_record *)gPolarCrateKeyframes,
      (u32 *)gPolarAkuAkuCrateFrames,
      3,
      0x313C,
      { -10, -10, -1, 20, 20, 3 },
      0,
      0 },
    { 10,
      (struct anim_frame_record *)gPolarBasicCrateKeyframes,
      (u32 *)gPolarBasicCrateFrames,
      1,
      0x313C,
      { -10, -10, -1, 20, 20, 3 },
      0,
      0 },
    { 11,
      (struct anim_frame_record *)gPolarWumpaKeyframes,
      (u32 *)gPolarWumpaFrames,
      7,
      0x2849,
      { -12, -12, -1, 24, 24, 3 },
      0,
      0 },
    { 12,
      (struct anim_frame_record *)gPolarBoostPadKeyframes,
      (u32 *)gPolarBoostPadFrames,
      6,
      0x2849,
      { -1, 4, -2, 2, 10, 4 },
      0,
      0 },
    { 13,
      (struct anim_frame_record *)gPolarObstacleKeyframes,
      (u32 *)gPolarObstacleFrames,
      11,
      0x2DE1,
      { -30, -4, -1, 120, 20, 3 },
      -7680,
      -1024 },
    { 14,
      (struct anim_frame_record *)gPolarObstacleKeyframes,
      (u32 *)gPolarObstacleFrames,
      11,
      0x2DE1,
      { 0, 0, 0, 0, 0, 0 },
      7680,
      0 },
    { 0,
      (struct anim_frame_record *)gPolarBoostPadKeyframes,
      (u32 *)gPolarBoostPadFrames,
      6,
      0x2849,
      { -10, 8, -1, 20, 3, 3 },
      0,
      0 },
    { 16,
      (struct anim_frame_record *)gPolarIcicleKeyframes,
      (u32 *)gPolarIcicleFrames,
      10,
      0x2F00,
      { -8, -20, 0, 16, 36, 1 },
      0,
      0 },
    { 17,
      (struct anim_frame_record *)gPolarIcicleKeyframes,
      (u32 *)gPolarIcicleFrames,
      10,
      0x2F00,
      { -8, -20, 0, 16, 36, 1 },
      0,
      0 },
    { 18,
      (struct anim_frame_record *)gPolarIcicleKeyframes,
      (u32 *)gPolarIcicleFrames,
      10,
      0x2F00,
      { -4, -20, 0, 8, 36, 1 },
      0,
      0 },
    { 19,
      (struct anim_frame_record *)gPolarIcicleKeyframes,
      (u32 *)gPolarIcicleFrames,
      10,
      0x2F00,
      { -4, -20, 0, 8, 36, 1 },
      0,
      0 },
    { 20,
      (struct anim_frame_record *)gPolarIcicleKeyframes,
      (u32 *)gPolarIcicleFrames,
      10,
      0x2F00,
      { -3, -12, 0, 6, 24, 1 },
      0,
      -2048 },
    { 21,
      (struct anim_frame_record *)gPolarIcicleKeyframes,
      (u32 *)gPolarIcicleFrames,
      10,
      0x2F00,
      { -3, -12, 0, 6, 24, 1 },
      0,
      -2048 },
    { 22,
      (struct anim_frame_record *)gPolarLauncherKeyframes,
      (u32 *)gPolarLauncherFrames,
      8,
      0x313C,
      { -9, -1, -1, 18, 4, 2 },
      0,
      2048 },
    { 23,
      (struct anim_frame_record *)gPolarElectricFenceKeyframes,
      (u32 *)gPolarElectricFenceFrames,
      9,
      0x260C,
      { -24, -25, 0, 48, 40, 1 },
      0,
      -1024 },
    { 24,
      (struct anim_frame_record *)gPolarPenguinKeyframes,
      (u32 *)gPolarPenguinFrames,
      13,
      0x36D5,
      { -12, -2, -1, 24, 12, 2 },
      0,
      0 },
    { 25,
      (struct anim_frame_record *)gPolarGoalKeyframes,
      (u32 *)gPolarGoalFrames,
      12,
      0x2CC3,
      { -30, -120, -28, 120, 140, 3 },
      -7680,
      1536 },
    { 26,
      (struct anim_frame_record *)gPolarGoalKeyframes,
      (u32 *)gPolarGoalFrames,
      12,
      0x2CC3,
      { 0, 0, 0, 0, 0, 0 },
      7680,
      0 },
    { 27,
      (struct anim_frame_record *)gPolarAkuAkuKeyframes,
      (u32 *)gPolarAkuAkuFrames,
      14,
      0x2F00,
      { 0, 0, 0, 0, 0, 0 },
      0,
      0 },
    { 28,
      (struct anim_frame_record *)gPolarCrateKeyframes,
      (u32 *)gPolarQuestionCrateFrames,
      1,
      0x313C,
      { -10, -10, -1, 20, 20, 3 },
      0,
      0 },
    { 29,
      (struct anim_frame_record *)gPolarCrateKeyframes,
      (u32 *)gPolarQuestionCrateFrames,
      1,
      0x313C,
      { -10, -10, -1, 20, 20, 3 },
      0,
      0 },
    { 30,
      (struct anim_frame_record *)gPolarCrateKeyframes,
      (u32 *)gPolarQuestionCrateFrames,
      1,
      0x313C,
      { -10, -10, -1, 20, 20, 3 },
      0,
      0 },
    { 31,
      (struct anim_frame_record *)gPolarCrateKeyframes,
      (u32 *)gPolarQuestionCrateFrames,
      1,
      0x313C,
      { -10, -10, -1, 20, 20, 3 },
      0,
      0 },
    { 32 }, /* unused */
    { 33 }, /* unused */
    { 34 }, /* unused */
    { 35,
      (struct anim_frame_record *)gPolarCrateKeyframes,
      (u32 *)gPolarQuestionCrateFrames,
      1,
      0x313C,
      { -10, -10, -1, 20, 20, 3 },
      0,
      0 },
    { 36,
      (struct anim_frame_record *)gPolarBoostPadKeyframes,
      (u32 *)gPolarBoostPadFrames,
      6,
      0x2849,
      { 0, 0, 0, 0, 0, 0 },
      0,
      0 },
    { 37,
      (struct anim_frame_record *)gPolarBoostPadKeyframes,
      (u32 *)gPolarBoostPadFrames,
      6,
      0x2849,
      { 0, 0, 0, 0, 0, 0 },
      0,
      0 },
    { 38,
      (struct anim_frame_record *)gPolarBoostPadKeyframes,
      (u32 *)gPolarBoostPadFrames,
      6,
      0x2849,
      { 0, 0, 0, 0, 0, 0 },
      0,
      0 },
    { 39,
      (struct anim_frame_record *)gPolarBoostPadKeyframes,
      (u32 *)gPolarBoostPadFrames,
      6,
      0x2849,
      { 0, 0, 0, 0, 0, 0 },
      0,
      0 },
    { 40,
      (struct anim_frame_record *)gPolarCheckpointTextKeyframes,
      (u32 *)gPolarCheckpointTextFrames,
      7,
      0x313C,
      { 0, 0, 0, 0, 0, 0 },
      0,
      0 },
};

const struct anim_frame_record gPolarWumpaKeyframes[1] = {
    { 85, 0, 14, 0, 0x0, { 0, 0 } },
};

const u32 gPolarWumpaFrames[14] = {
    FRAMED_0B2120_WUMPA_FRUIT_00, FRAMED_0B2120_WUMPA_FRUIT_01, FRAMED_0B2120_WUMPA_FRUIT_02,
    FRAMED_0B2120_WUMPA_FRUIT_03, FRAMED_0B2120_WUMPA_FRUIT_04, FRAMED_0B2120_WUMPA_FRUIT_05,
    FRAMED_0B2120_WUMPA_FRUIT_06, FRAMED_0B2120_WUMPA_FRUIT_07, FRAMED_0B2120_WUMPA_FRUIT_08,
    FRAMED_0B2120_WUMPA_FRUIT_09, FRAMED_0B2120_WUMPA_FRUIT_10, FRAMED_0B2120_WUMPA_FRUIT_11,
    FRAMED_0B2120_WUMPA_FRUIT_12, FRAMED_0B2120_WUMPA_FRUIT_13,
};

const struct anim_frame_record gPolarLauncherKeyframes[2] = {
    { 64, 0, 10, 0, 0x0, { 0, 0 } },
    { 64, 10, 10, 0, 0x0, { 0, 0 } },
};

const u32 gPolarLauncherFrames[20] = {
    FRAMED_0B2120_EMERGING_CREATURE_00, FRAMED_0B2120_EMERGING_CREATURE_01,
    FRAMED_0B2120_EMERGING_CREATURE_02, FRAMED_0B2120_EMERGING_CREATURE_03,
    FRAMED_0B2120_EMERGING_CREATURE_04, FRAMED_0B2120_EMERGING_CREATURE_05,
    FRAMED_0B2120_EMERGING_CREATURE_06, FRAMED_0B2120_EMERGING_CREATURE_07,
    FRAMED_0B2120_EMERGING_CREATURE_08, FRAMED_0B2120_EMERGING_CREATURE_09,
    FRAMED_0B2120_EMERGING_CREATURE_10, FRAMED_0B2120_EMERGING_CREATURE_11,
    FRAMED_0B2120_EMERGING_CREATURE_12, FRAMED_0B2120_EMERGING_CREATURE_13,
    FRAMED_0B2120_EMERGING_CREATURE_14, FRAMED_0B2120_EMERGING_CREATURE_15,
    FRAMED_0B2120_EMERGING_CREATURE_16, FRAMED_0B2120_EMERGING_CREATURE_17,
    FRAMED_0B2120_EMERGING_CREATURE_18, FRAMED_0B2120_EMERGING_CREATURE_19,
};

const struct anim_frame_record gPolarBoostPadKeyframes[3] = {
    { 42, 4, 4, 0, 0x1000, { 0, 0 } },
    { 42, 0, 4, 0, 0x0, { 0, 0 } },
    { 42, 4, 4, 0, 0x0, { 0, 0 } },
};

const u32 gPolarBoostPadFrames[8] = {
    FRAMED_0B2120_RECORD_SLOT12_00, FRAMED_0B2120_RECORD_SLOT12_01, FRAMED_0B2120_RECORD_SLOT12_02,
    FRAMED_0B2120_RECORD_SLOT12_03, FRAMED_0B2120_RECORD_SLOT12_04, FRAMED_0B2120_RECORD_SLOT12_05,
    FRAMED_0B2120_RECORD_SLOT12_06, FRAMED_0B2120_RECORD_SLOT12_07,
};

const struct anim_frame_record gPolarElectricFenceKeyframes[2] = {
    { 64, 0, 8, 0, 0x0, { 0, 0 } },
    { 64, 8, 14, 13, 0x0, { 0, 0 } },
};

const u32 gPolarElectricFenceFrames[22] = {
    FRAMED_0B2120_GUARD_BARRIER_00,     FRAMED_0B2120_GUARD_BARRIER_01,
    FRAMED_0B2120_GUARD_BARRIER_02,     FRAMED_0B2120_GUARD_BARRIER_03,
    FRAMED_0B2120_GUARD_BARRIER_04,     FRAMED_0B2120_GUARD_BARRIER_05,
    FRAMED_0B2120_GUARD_BARRIER_06,     FRAMED_0B2120_GUARD_BARRIER_07,
    FRAMED_0B2120_EMERGING_CREATURE_40, FRAMED_0B2120_EMERGING_CREATURE_41,
    FRAMED_0B2120_EMERGING_CREATURE_42, FRAMED_0B2120_EMERGING_CREATURE_43,
    FRAMED_0B2120_EMERGING_CREATURE_44, FRAMED_0B2120_EMERGING_CREATURE_45,
    FRAMED_0B2120_EMERGING_CREATURE_46, FRAMED_0B2120_EMERGING_CREATURE_47,
    FRAMED_0B2120_EMERGING_CREATURE_48, FRAMED_0B2120_EMERGING_CREATURE_49,
    FRAMED_0B2120_EMERGING_CREATURE_50, FRAMED_0B2120_EMERGING_CREATURE_51,
    FRAMED_0B2120_EMERGING_CREATURE_52, FRAMED_0B2120_EMERGING_CREATURE_53,
};

const struct anim_frame_record gPolarIcicleKeyframes[24] = {
    { 0, 0, 1, 0, 0x0, { 0, 0 } },     { 0, 1, 1, 0, 0x0, { 0, 0 } },
    { 0, 2, 1, 0, 0x0, { 0, 0 } },     { 0, 3, 1, 0, 0x0, { 0, 0 } },
    { 0, 0, 1, 0, 0x1000, { 0, 0 } },  { 0, 1, 1, 0, 0x1000, { 0, 0 } },
    { 0, 2, 1, 0, 0x1000, { 0, 0 } },  { 0, 3, 1, 0, 0x1000, { 0, 0 } },
    { 0, 4, 1, 0, 0x0, { 0, 0 } },     { 0, 5, 1, 0, 0x0, { 0, 0 } },
    { 0, 6, 1, 0, 0x0, { 0, 0 } },     { 0, 7, 1, 0, 0x0, { 0, 0 } },
    { 0, 4, 1, 0, 0x1000, { 0, 0 } },  { 0, 5, 1, 0, 0x1000, { 0, 0 } },
    { 0, 6, 1, 0, 0x1000, { 0, 0 } },  { 0, 7, 1, 0, 0x1000, { 0, 0 } },
    { 0, 8, 1, 0, 0x0, { 0, 0 } },     { 0, 9, 1, 0, 0x0, { 0, 0 } },
    { 0, 10, 1, 0, 0x0, { 0, 0 } },    { 0, 11, 1, 0, 0x0, { 0, 0 } },
    { 0, 8, 1, 0, 0x1000, { 0, 0 } },  { 0, 9, 1, 0, 0x1000, { 0, 0 } },
    { 0, 10, 1, 0, 0x1000, { 0, 0 } }, { 0, 11, 1, 0, 0x1000, { 0, 0 } },
};

const u32 gPolarIcicleFrames[12] = {
    FRAMED_0B2120_RECORD_SLOT16_00, FRAMED_0B2120_RECORD_SLOT16_01, FRAMED_0B2120_RECORD_SLOT16_02,
    FRAMED_0B2120_RECORD_SLOT16_03, FRAMED_0B2120_RECORD_SLOT16_04, FRAMED_0B2120_RECORD_SLOT16_05,
    FRAMED_0B2120_RECORD_SLOT16_06, FRAMED_0B2120_RECORD_SLOT16_07, FRAMED_0B2120_RECORD_SLOT16_08,
    FRAMED_0B2120_RECORD_SLOT16_09, FRAMED_0B2120_RECORD_SLOT16_10, FRAMED_0B2120_RECORD_SLOT16_11,
};

/* Records 13/14, the two halves of the polar obstacle (CreatePolarObstacle):
 * a low 120-pixel-wide ridge across the track, only in category 2. */
const u32 gPolarObstacleFrames[1] = {
    FRAMED_0B2120_RECORD_SLOT13_00,
};

const struct anim_frame_record gPolarObstacleKeyframes[2] = {
    { 0, 0, 1, 0, 0x0, { 0, 0 } },
    { 0, 0, 1, 0, 0x1000, { 0, 0 } },
};

const u32 gPolarGoalFrames[1] = {
    FRAMED_0B2120_RECORD_SLOT25_00,
};

const struct anim_frame_record gPolarGoalKeyframes[2] = {
    { 0, 0, 1, 0, 0x0, { 0, 0 } },
    { 0, 0, 1, 0, 0x1000, { 0, 0 } },
};

const u32 gPolarPenguinFrames[9] = {
    FRAMED_0B2120_RECORD_SLOT24_00, FRAMED_0B2120_RECORD_SLOT24_01, FRAMED_0B2120_RECORD_SLOT24_02,
    FRAMED_0B2120_RECORD_SLOT24_03, FRAMED_0B2120_RECORD_SLOT24_04, FRAMED_0B2120_RECORD_SLOT24_05,
    FRAMED_0B2120_RECORD_SLOT24_06, FRAMED_0B2120_RECORD_SLOT24_07, FRAMED_0B2120_RECORD_SLOT24_08,
};

const struct anim_frame_record gPolarPenguinKeyframes[1] = {
    { 64, 0, 9, 0, 0x0, { 0, 0 } },
};

const struct anim_frame_record gPolarBasicCrateKeyframes[19] = {
    { 0, 0, 1, 0, 0x0, { 0, 0 } },    { 0, 1, 1, 0, 0x0, { 0, 0 } },
    { 0, 2, 1, 0, 0x0, { 0, 0 } },    { 0, 2, 1, 0, 0x1000, { 0, 0 } },
    { 0, 1, 1, 0, 0x1000, { 0, 0 } }, { 0, 0, 1, 0, 0x1000, { 0, 0 } },
    { 0, 3, 1, 0, 0x0, { 0, 0 } },    { 0, 4, 1, 0, 0x0, { 0, 0 } },
    { 0, 5, 1, 0, 0x0, { 0, 0 } },    { 0, 5, 1, 0, 0x1000, { 0, 0 } },
    { 0, 4, 1, 0, 0x1000, { 0, 0 } }, { 0, 3, 1, 0, 0x1000, { 0, 0 } },
    { 0, 6, 1, 0, 0x0, { 0, 0 } },    { 0, 7, 1, 0, 0x0, { 0, 0 } },
    { 0, 8, 1, 0, 0x0, { 0, 0 } },    { 0, 8, 1, 0, 0x1000, { 0, 0 } },
    { 0, 7, 1, 0, 0x1000, { 0, 0 } }, { 0, 6, 1, 0, 0x1000, { 0, 0 } },
    { 51, 9, 6, 5, 0x0, { 0, 0 } },
};

const u32 gPolarBasicCrateFrames[15] = {
    FRAMED_0B2120_TNT_CRATE_00,         FRAMED_0B2120_TNT_CRATE_01,
    FRAMED_0B2120_TNT_CRATE_02,         FRAMED_0B2120_TNT_CRATE_03,
    FRAMED_0B2120_TNT_CRATE_04,         FRAMED_0B2120_TNT_CRATE_05,
    FRAMED_0B2120_TNT_CRATE_06,         FRAMED_0B2120_TNT_CRATE_07,
    FRAMED_0B2120_TNT_CRATE_08,         FRAMED_0B2120_EMERGING_CREATURE_20,
    FRAMED_0B2120_EMERGING_CREATURE_21, FRAMED_0B2120_EMERGING_CREATURE_22,
    FRAMED_0B2120_EMERGING_CREATURE_23, FRAMED_0B2120_EMERGING_CREATURE_24,
    FRAMED_0B2120_EMERGING_CREATURE_25,
};

const struct anim_frame_record gPolarNitroCrateKeyframes[19] = {
    { 0, 0, 1, 0, 0x0, { 0, 0 } },   { 0, 1, 1, 0, 0x0, { 0, 0 } },  { 0, 2, 1, 0, 0x0, { 0, 0 } },
    { 0, 3, 1, 0, 0x0, { 0, 0 } },   { 0, 4, 1, 0, 0x0, { 0, 0 } },  { 0, 5, 1, 0, 0x0, { 0, 0 } },
    { 0, 6, 1, 0, 0x0, { 0, 0 } },   { 0, 7, 1, 0, 0x0, { 0, 0 } },  { 0, 8, 1, 0, 0x0, { 0, 0 } },
    { 0, 9, 1, 0, 0x0, { 0, 0 } },   { 0, 10, 1, 0, 0x0, { 0, 0 } }, { 0, 11, 1, 0, 0x0, { 0, 0 } },
    { 0, 12, 1, 0, 0x0, { 0, 0 } },  { 0, 13, 1, 0, 0x0, { 0, 0 } }, { 0, 14, 1, 0, 0x0, { 0, 0 } },
    { 0, 15, 1, 0, 0x0, { 0, 0 } },  { 0, 16, 1, 0, 0x0, { 0, 0 } }, { 0, 17, 1, 0, 0x0, { 0, 0 } },
    { 51, 18, 8, 7, 0x0, { 0, 0 } },
};

const u32 gPolarNitroCrateFrames[26] = {
    FRAMED_0B2120_NITRO_CRATE_00,       FRAMED_0B2120_NITRO_CRATE_01,
    FRAMED_0B2120_NITRO_CRATE_02,       FRAMED_0B2120_NITRO_CRATE_03,
    FRAMED_0B2120_NITRO_CRATE_04,       FRAMED_0B2120_NITRO_CRATE_05,
    FRAMED_0B2120_NITRO_CRATE_06,       FRAMED_0B2120_NITRO_CRATE_07,
    FRAMED_0B2120_NITRO_CRATE_08,       FRAMED_0B2120_NITRO_CRATE_09,
    FRAMED_0B2120_NITRO_CRATE_10,       FRAMED_0B2120_NITRO_CRATE_11,
    FRAMED_0B2120_NITRO_CRATE_12,       FRAMED_0B2120_NITRO_CRATE_13,
    FRAMED_0B2120_NITRO_CRATE_14,       FRAMED_0B2120_NITRO_CRATE_15,
    FRAMED_0B2120_NITRO_CRATE_16,       FRAMED_0B2120_NITRO_CRATE_17,
    FRAMED_0B2120_EMERGING_CREATURE_26, FRAMED_0B2120_EMERGING_CREATURE_27,
    FRAMED_0B2120_EMERGING_CREATURE_28, FRAMED_0B2120_EMERGING_CREATURE_29,
    FRAMED_0B2120_EMERGING_CREATURE_30, FRAMED_0B2120_EMERGING_CREATURE_31,
    FRAMED_0B2120_EMERGING_CREATURE_32, FRAMED_0B2120_EMERGING_CREATURE_33,
};

const struct anim_frame_record gPolarCrateKeyframes[19] = {
    { 0, 0, 1, 0, 0x0, { 0, 0 } },   { 0, 1, 1, 0, 0x0, { 0, 0 } },  { 0, 2, 1, 0, 0x0, { 0, 0 } },
    { 0, 3, 1, 0, 0x0, { 0, 0 } },   { 0, 4, 1, 0, 0x0, { 0, 0 } },  { 0, 5, 1, 0, 0x0, { 0, 0 } },
    { 0, 6, 1, 0, 0x0, { 0, 0 } },   { 0, 7, 1, 0, 0x0, { 0, 0 } },  { 0, 8, 1, 0, 0x0, { 0, 0 } },
    { 0, 9, 1, 0, 0x0, { 0, 0 } },   { 0, 10, 1, 0, 0x0, { 0, 0 } }, { 0, 11, 1, 0, 0x0, { 0, 0 } },
    { 0, 12, 1, 0, 0x0, { 0, 0 } },  { 0, 13, 1, 0, 0x0, { 0, 0 } }, { 0, 14, 1, 0, 0x0, { 0, 0 } },
    { 0, 15, 1, 0, 0x0, { 0, 0 } },  { 0, 16, 1, 0, 0x0, { 0, 0 } }, { 0, 17, 1, 0, 0x0, { 0, 0 } },
    { 51, 18, 6, 5, 0x0, { 0, 0 } },
};

const u32 gPolarTimeCrate1Frames[24] = {
    FRAMED_0B2120_CRATE_VARIANT_A_00,   FRAMED_0B2120_CRATE_VARIANT_A_01,
    FRAMED_0B2120_CRATE_VARIANT_A_02,   FRAMED_0B2120_CRATE_VARIANT_A_03,
    FRAMED_0B2120_CRATE_VARIANT_A_04,   FRAMED_0B2120_CRATE_VARIANT_A_05,
    FRAMED_0B2120_CRATE_VARIANT_A_06,   FRAMED_0B2120_CRATE_VARIANT_A_07,
    FRAMED_0B2120_CRATE_VARIANT_A_08,   FRAMED_0B2120_CRATE_VARIANT_A_09,
    FRAMED_0B2120_CRATE_VARIANT_A_10,   FRAMED_0B2120_CRATE_VARIANT_A_11,
    FRAMED_0B2120_CRATE_VARIANT_A_12,   FRAMED_0B2120_CRATE_VARIANT_A_13,
    FRAMED_0B2120_CRATE_VARIANT_A_14,   FRAMED_0B2120_CRATE_VARIANT_A_15,
    FRAMED_0B2120_CRATE_VARIANT_A_16,   FRAMED_0B2120_CRATE_VARIANT_A_17,
    FRAMED_0B2120_EMERGING_CREATURE_34, FRAMED_0B2120_EMERGING_CREATURE_35,
    FRAMED_0B2120_EMERGING_CREATURE_36, FRAMED_0B2120_EMERGING_CREATURE_37,
    FRAMED_0B2120_EMERGING_CREATURE_38, FRAMED_0B2120_EMERGING_CREATURE_39,
};

const u32 gPolarTimeCrate2Frames[24] = {
    FRAMED_0B2120_CRATE_VARIANT_B_00,   FRAMED_0B2120_CRATE_VARIANT_B_01,
    FRAMED_0B2120_CRATE_VARIANT_B_02,   FRAMED_0B2120_CRATE_VARIANT_B_03,
    FRAMED_0B2120_CRATE_VARIANT_B_04,   FRAMED_0B2120_CRATE_VARIANT_B_05,
    FRAMED_0B2120_CRATE_VARIANT_B_06,   FRAMED_0B2120_CRATE_VARIANT_B_07,
    FRAMED_0B2120_CRATE_VARIANT_B_08,   FRAMED_0B2120_CRATE_VARIANT_B_09,
    FRAMED_0B2120_CRATE_VARIANT_B_10,   FRAMED_0B2120_CRATE_VARIANT_B_11,
    FRAMED_0B2120_CRATE_VARIANT_B_12,   FRAMED_0B2120_CRATE_VARIANT_B_13,
    FRAMED_0B2120_CRATE_VARIANT_B_14,   FRAMED_0B2120_CRATE_VARIANT_B_15,
    FRAMED_0B2120_CRATE_VARIANT_B_16,   FRAMED_0B2120_CRATE_VARIANT_B_17,
    FRAMED_0B2120_EMERGING_CREATURE_34, FRAMED_0B2120_EMERGING_CREATURE_35,
    FRAMED_0B2120_EMERGING_CREATURE_36, FRAMED_0B2120_EMERGING_CREATURE_37,
    FRAMED_0B2120_EMERGING_CREATURE_38, FRAMED_0B2120_EMERGING_CREATURE_39,
};

const u32 gPolarTimeCrate3Frames[24] = {
    FRAMED_0B2120_CRATE_VARIANT_C_00,   FRAMED_0B2120_CRATE_VARIANT_C_01,
    FRAMED_0B2120_CRATE_VARIANT_C_02,   FRAMED_0B2120_CRATE_VARIANT_C_03,
    FRAMED_0B2120_CRATE_VARIANT_C_04,   FRAMED_0B2120_CRATE_VARIANT_C_05,
    FRAMED_0B2120_CRATE_VARIANT_C_06,   FRAMED_0B2120_CRATE_VARIANT_C_07,
    FRAMED_0B2120_CRATE_VARIANT_C_08,   FRAMED_0B2120_CRATE_VARIANT_C_09,
    FRAMED_0B2120_CRATE_VARIANT_C_10,   FRAMED_0B2120_CRATE_VARIANT_C_11,
    FRAMED_0B2120_CRATE_VARIANT_C_12,   FRAMED_0B2120_CRATE_VARIANT_C_13,
    FRAMED_0B2120_CRATE_VARIANT_C_14,   FRAMED_0B2120_CRATE_VARIANT_C_15,
    FRAMED_0B2120_CRATE_VARIANT_C_16,   FRAMED_0B2120_CRATE_VARIANT_C_17,
    FRAMED_0B2120_EMERGING_CREATURE_34, FRAMED_0B2120_EMERGING_CREATURE_35,
    FRAMED_0B2120_EMERGING_CREATURE_36, FRAMED_0B2120_EMERGING_CREATURE_37,
    FRAMED_0B2120_EMERGING_CREATURE_38, FRAMED_0B2120_EMERGING_CREATURE_39,
};

const u32 gPolarQuestionCrateFrames[24] = {
    FRAMED_0B2120_RECORD_SLOT28_00,     FRAMED_0B2120_RECORD_SLOT28_01,
    FRAMED_0B2120_RECORD_SLOT28_02,     FRAMED_0B2120_RECORD_SLOT28_03,
    FRAMED_0B2120_RECORD_SLOT28_04,     FRAMED_0B2120_RECORD_SLOT28_05,
    FRAMED_0B2120_RECORD_SLOT28_06,     FRAMED_0B2120_RECORD_SLOT28_07,
    FRAMED_0B2120_RECORD_SLOT28_08,     FRAMED_0B2120_RECORD_SLOT28_09,
    FRAMED_0B2120_RECORD_SLOT28_10,     FRAMED_0B2120_RECORD_SLOT28_11,
    FRAMED_0B2120_RECORD_SLOT28_12,     FRAMED_0B2120_RECORD_SLOT28_13,
    FRAMED_0B2120_RECORD_SLOT28_14,     FRAMED_0B2120_RECORD_SLOT28_15,
    FRAMED_0B2120_RECORD_SLOT28_16,     FRAMED_0B2120_RECORD_SLOT28_17,
    FRAMED_0B2120_EMERGING_CREATURE_20, FRAMED_0B2120_EMERGING_CREATURE_21,
    FRAMED_0B2120_EMERGING_CREATURE_22, FRAMED_0B2120_EMERGING_CREATURE_23,
    FRAMED_0B2120_EMERGING_CREATURE_24, FRAMED_0B2120_EMERGING_CREATURE_25,
};

const u32 gPolarLifeCrateFrames[24] = {
    FRAMED_0B2120_CRATE_VARIANT_D_00,   FRAMED_0B2120_CRATE_VARIANT_D_01,
    FRAMED_0B2120_CRATE_VARIANT_D_02,   FRAMED_0B2120_CRATE_VARIANT_D_03,
    FRAMED_0B2120_CRATE_VARIANT_D_04,   FRAMED_0B2120_CRATE_VARIANT_D_05,
    FRAMED_0B2120_CRATE_VARIANT_D_06,   FRAMED_0B2120_CRATE_VARIANT_D_07,
    FRAMED_0B2120_CRATE_VARIANT_D_08,   FRAMED_0B2120_CRATE_VARIANT_D_09,
    FRAMED_0B2120_CRATE_VARIANT_D_10,   FRAMED_0B2120_CRATE_VARIANT_D_11,
    FRAMED_0B2120_CRATE_VARIANT_D_12,   FRAMED_0B2120_CRATE_VARIANT_D_13,
    FRAMED_0B2120_CRATE_VARIANT_D_14,   FRAMED_0B2120_CRATE_VARIANT_D_15,
    FRAMED_0B2120_CRATE_VARIANT_D_16,   FRAMED_0B2120_CRATE_VARIANT_D_17,
    FRAMED_0B2120_EMERGING_CREATURE_20, FRAMED_0B2120_EMERGING_CREATURE_21,
    FRAMED_0B2120_EMERGING_CREATURE_22, FRAMED_0B2120_EMERGING_CREATURE_23,
    FRAMED_0B2120_EMERGING_CREATURE_24, FRAMED_0B2120_EMERGING_CREATURE_25,
};

const u32 gPolarAkuAkuCrateFrames[24] = {
    FRAMED_0B2120_CRATE_VARIANT_E_00,   FRAMED_0B2120_CRATE_VARIANT_E_01,
    FRAMED_0B2120_CRATE_VARIANT_E_02,   FRAMED_0B2120_CRATE_VARIANT_E_03,
    FRAMED_0B2120_CRATE_VARIANT_E_04,   FRAMED_0B2120_CRATE_VARIANT_E_05,
    FRAMED_0B2120_CRATE_VARIANT_E_06,   FRAMED_0B2120_CRATE_VARIANT_E_07,
    FRAMED_0B2120_CRATE_VARIANT_E_08,   FRAMED_0B2120_CRATE_VARIANT_E_09,
    FRAMED_0B2120_CRATE_VARIANT_E_10,   FRAMED_0B2120_CRATE_VARIANT_E_11,
    FRAMED_0B2120_CRATE_VARIANT_E_12,   FRAMED_0B2120_CRATE_VARIANT_E_13,
    FRAMED_0B2120_CRATE_VARIANT_E_14,   FRAMED_0B2120_CRATE_VARIANT_E_15,
    FRAMED_0B2120_CRATE_VARIANT_E_16,   FRAMED_0B2120_CRATE_VARIANT_E_17,
    FRAMED_0B2120_EMERGING_CREATURE_20, FRAMED_0B2120_EMERGING_CREATURE_21,
    FRAMED_0B2120_EMERGING_CREATURE_22, FRAMED_0B2120_EMERGING_CREATURE_23,
    FRAMED_0B2120_EMERGING_CREATURE_24, FRAMED_0B2120_EMERGING_CREATURE_25,
};

const u32 gPolarCheckpointCrateFrames[20] = {
    FRAMED_0B2120_BARREL_00,
    FRAMED_0B2120_BARREL_01,
    FRAMED_0B2120_BARREL_02,
    FRAMED_0B2120_BARREL_03,
    FRAMED_0B2120_BARREL_04,
    FRAMED_0B2120_BARREL_05,
    FRAMED_0B2120_BARREL_06,
    FRAMED_0B2120_BARREL_07,
    FRAMED_0B2120_BARREL_08,
    FRAMED_0B2120_BARREL_09,
    FRAMED_0B2120_BARREL_10,
    FRAMED_0B2120_BARREL_11,
    FRAMED_0B2120_BARREL_12,
    FRAMED_0B2120_BARREL_13,
    FRAMED_0B2120_EMERGING_CREATURE_20,
    FRAMED_0B2120_EMERGING_CREATURE_21,
    FRAMED_0B2120_EMERGING_CREATURE_22,
    FRAMED_0B2120_EMERGING_CREATURE_23,
    FRAMED_0B2120_EMERGING_CREATURE_24,
    FRAMED_0B2120_EMERGING_CREATURE_25,
};

const struct anim_frame_record gPolarCheckpointCrateKeyframes[4] = {
    { 128, 0, 1, 0, 0x0, { 0, 0 } },
    { 128, 1, 13, 12, 0x0, { 0, 0 } },
    { 128, 13, 1, 0, 0x0, { 0, 0 } },
    { 51, 14, 6, 5, 0x0, { 0, 0 } },
};

const u32 gPolarAkuAkuFrames[11] = {
    FRAMED_0B2120_RECORD_SLOT27_00,     FRAMED_0B2120_RECORD_SLOT27_01,
    FRAMED_0B2120_RECORD_SLOT27_01,     FRAMED_0B2120_RECORD_SLOT27_01,
    FRAMED_0B2120_RECORD_SLOT27_01,     FRAMED_0B2120_RECORD_SLOT27_01,
    FRAMED_0B2120_EMERGING_CREATURE_54, FRAMED_0B2120_EMERGING_CREATURE_55,
    FRAMED_0B2120_EMERGING_CREATURE_56, FRAMED_0B2120_EMERGING_CREATURE_57,
    FRAMED_0B2120_EMERGING_CREATURE_58,
};

const struct anim_frame_record gPolarAkuAkuKeyframes[2] = {
    { 34, 0, 6, 0, 0x0, { 0, 0 } },
    { 34, 6, 5, 4, 0x0, { 0, 0 } },
};

const struct anim_frame_record gPolarCheckpointTextKeyframes[1] = {
    { 8, 0, 3, 0, 0x0, { 0, 0 } },
};

const u32 gPolarCheckpointTextFrames[3] = {
    FRAMED_0B2120_CHECKPOINT_TEXT_00,
    FRAMED_0B2120_CHECKPOINT_TEXT_00,
    FRAMED_0B2120_CHECKPOINT_TEXT_00,
};
