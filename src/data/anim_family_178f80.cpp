#include "frontend.hpp"
#include "graphics.hpp"

extern "C" {
#include "core.h"
#include "engine/actor/actor_anim.h"
}

/*
 * ROM 0x08178F80-0x0817A6B8: the data of actor categories 0-2 (the family
 * whose frames are the gPolarSpriteSheet sheet): their OBJ palette,
 * the 41-record animation table gCategoryFamily0AnimTable
 * (include/engine/actor/actor_anim.h) and the keyframe (table_A) and frame (table_B)
 * arrays its records point at. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md, "Category families".
 *
 * Record 0's table_B points at compressed frames of set A
 * (rle_sprites_0c2758.cpp), the others' at frames of the framed sheet
 * graphics/vehicles/polar/sprite_sheet/ (byte offsets into its decompressed
 * data). Both kinds are written with the offsets the build generates,
 * so edited frames move them along. The arrays are cut at the next
 * array a record points at; nothing but the records references them.
 */

#include "vehicles/polar/player.h"
#include "vehicles/polar/sprite_sheet_frames.h"

using Engine::Gfx::PolarPlayerFrameSet;

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

/* The 256-colour OBJ palette ActorCategory::Init loads for these
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
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_000,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_001,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_002,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_003,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_004,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_005,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_006,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_007,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_008,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_009,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_010,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_011,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_012,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_013,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_014,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_015,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_016,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_017,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_018,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_019,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_020,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_021,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_022,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_023,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_024,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_025,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_026,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_027,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_028,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_029,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_030,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_031,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_032,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_033,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_034,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_035,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_036,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_037,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_038,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_039,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_040,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_041,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_042,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_043,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_044,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_045,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_046,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_047,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_048,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_049,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_050,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_051,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_052,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_053,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_054,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_055,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_056,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_057,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_058,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_059,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_060,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_061,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_062,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_063,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_064,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_065,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_066,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_067,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_068,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_069,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_070,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_071,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_072,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_073,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_074,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_075,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_076,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_077,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_078,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_079,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_080,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_081,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_082,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_083,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_084,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_085,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_086,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_087,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_088,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_089,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_090,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_091,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_092,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_093,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_094,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_095,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_096,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_136,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_137,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_138,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_139,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_140,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_141,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_142,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_143,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_144,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_145,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_146,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_147,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_148,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_149,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_150,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_151,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_020,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_021,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_022,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_023,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_097,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_098,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_099,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_100,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_101,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_102,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_103,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_104,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_105,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_106,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_107,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_108,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_109,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_110,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_111,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_135,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_112,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_113,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_114,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_115,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_116,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_117,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_118,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_119,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_120,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_121,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_122,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_123,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_124,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_125,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_126,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_127,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_128,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_129,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_130,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_131,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_132,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_133,
    PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_134,
};

const struct anim_frame_record gRiderlessPolarKeyframes[2] = {
    { 64, 0, 9, 8, 0x0, { 0, 0 } },
    { 64, 9, 1, 0, 0x0, { 0, 0 } },
};

const u32 gRiderlessPolarFrames[10] = {
    FRAMED_POLAR_RIDERLESS_POLAR_00, FRAMED_POLAR_RIDERLESS_POLAR_01,
    FRAMED_POLAR_RIDERLESS_POLAR_02, FRAMED_POLAR_RIDERLESS_POLAR_03,
    FRAMED_POLAR_RIDERLESS_POLAR_04, FRAMED_POLAR_RIDERLESS_POLAR_05,
    FRAMED_POLAR_RIDERLESS_POLAR_06, FRAMED_POLAR_RIDERLESS_POLAR_07,
    FRAMED_POLAR_RIDERLESS_POLAR_08, FRAMED_POLAR_RIDERLESS_POLAR_09,
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
    FRAMED_POLAR_WUMPA_00, FRAMED_POLAR_WUMPA_01, FRAMED_POLAR_WUMPA_02, FRAMED_POLAR_WUMPA_03,
    FRAMED_POLAR_WUMPA_04, FRAMED_POLAR_WUMPA_05, FRAMED_POLAR_WUMPA_06, FRAMED_POLAR_WUMPA_07,
    FRAMED_POLAR_WUMPA_08, FRAMED_POLAR_WUMPA_09, FRAMED_POLAR_WUMPA_10, FRAMED_POLAR_WUMPA_11,
    FRAMED_POLAR_WUMPA_12, FRAMED_POLAR_WUMPA_13,
};

const struct anim_frame_record gPolarLauncherKeyframes[2] = {
    { 64, 0, 10, 0, 0x0, { 0, 0 } },
    { 64, 10, 10, 0, 0x0, { 0, 0 } },
};

const u32 gPolarLauncherFrames[20] = {
    FRAMED_POLAR_LAUNCHER_00, FRAMED_POLAR_LAUNCHER_01, FRAMED_POLAR_LAUNCHER_02,
    FRAMED_POLAR_LAUNCHER_03, FRAMED_POLAR_LAUNCHER_04, FRAMED_POLAR_LAUNCHER_05,
    FRAMED_POLAR_LAUNCHER_06, FRAMED_POLAR_LAUNCHER_07, FRAMED_POLAR_LAUNCHER_08,
    FRAMED_POLAR_LAUNCHER_09, FRAMED_POLAR_LAUNCHER_10, FRAMED_POLAR_LAUNCHER_11,
    FRAMED_POLAR_LAUNCHER_12, FRAMED_POLAR_LAUNCHER_13, FRAMED_POLAR_LAUNCHER_14,
    FRAMED_POLAR_LAUNCHER_15, FRAMED_POLAR_LAUNCHER_16, FRAMED_POLAR_LAUNCHER_17,
    FRAMED_POLAR_LAUNCHER_18, FRAMED_POLAR_LAUNCHER_19,
};

const struct anim_frame_record gPolarBoostPadKeyframes[3] = {
    { 42, 4, 4, 0, 0x1000, { 0, 0 } },
    { 42, 0, 4, 0, 0x0, { 0, 0 } },
    { 42, 4, 4, 0, 0x0, { 0, 0 } },
};

const u32 gPolarBoostPadFrames[8] = {
    FRAMED_POLAR_BOOST_PAD_00, FRAMED_POLAR_BOOST_PAD_01, FRAMED_POLAR_BOOST_PAD_02,
    FRAMED_POLAR_BOOST_PAD_03, FRAMED_POLAR_BOOST_PAD_04, FRAMED_POLAR_BOOST_PAD_05,
    FRAMED_POLAR_BOOST_PAD_06, FRAMED_POLAR_BOOST_PAD_07,
};

const struct anim_frame_record gPolarElectricFenceKeyframes[2] = {
    { 64, 0, 8, 0, 0x0, { 0, 0 } },
    { 64, 8, 14, 13, 0x0, { 0, 0 } },
};

const u32 gPolarElectricFenceFrames[22] = {
    FRAMED_POLAR_ELECTRIC_FENCE_00,       FRAMED_POLAR_ELECTRIC_FENCE_01,
    FRAMED_POLAR_ELECTRIC_FENCE_02,       FRAMED_POLAR_ELECTRIC_FENCE_03,
    FRAMED_POLAR_ELECTRIC_FENCE_04,       FRAMED_POLAR_ELECTRIC_FENCE_05,
    FRAMED_POLAR_ELECTRIC_FENCE_06,       FRAMED_POLAR_ELECTRIC_FENCE_07,
    FRAMED_POLAR_ELECTRIC_FENCE_WRECK_00, FRAMED_POLAR_ELECTRIC_FENCE_WRECK_01,
    FRAMED_POLAR_ELECTRIC_FENCE_WRECK_02, FRAMED_POLAR_ELECTRIC_FENCE_WRECK_03,
    FRAMED_POLAR_ELECTRIC_FENCE_WRECK_04, FRAMED_POLAR_ELECTRIC_FENCE_WRECK_05,
    FRAMED_POLAR_ELECTRIC_FENCE_WRECK_06, FRAMED_POLAR_ELECTRIC_FENCE_WRECK_07,
    FRAMED_POLAR_ELECTRIC_FENCE_WRECK_08, FRAMED_POLAR_ELECTRIC_FENCE_WRECK_09,
    FRAMED_POLAR_ELECTRIC_FENCE_WRECK_10, FRAMED_POLAR_ELECTRIC_FENCE_WRECK_11,
    FRAMED_POLAR_ELECTRIC_FENCE_WRECK_12, FRAMED_POLAR_ELECTRIC_FENCE_WRECK_13,
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
    FRAMED_POLAR_ICICLE_00, FRAMED_POLAR_ICICLE_01, FRAMED_POLAR_ICICLE_02,
    FRAMED_POLAR_ICICLE_03, FRAMED_POLAR_ICICLE_04, FRAMED_POLAR_ICICLE_05,
    FRAMED_POLAR_ICICLE_06, FRAMED_POLAR_ICICLE_07, FRAMED_POLAR_ICICLE_08,
    FRAMED_POLAR_ICICLE_09, FRAMED_POLAR_ICICLE_10, FRAMED_POLAR_ICICLE_11,
};

/* Records 13/14, the two halves of the polar obstacle (CreatePolarObstacle):
 * a low 120-pixel-wide ridge across the track, only in category 2. */
const u32 gPolarObstacleFrames[1] = {
    FRAMED_POLAR_OBSTACLE_00,
};

const struct anim_frame_record gPolarObstacleKeyframes[2] = {
    { 0, 0, 1, 0, 0x0, { 0, 0 } },
    { 0, 0, 1, 0, 0x1000, { 0, 0 } },
};

const u32 gPolarGoalFrames[1] = {
    FRAMED_POLAR_GOAL_00,
};

const struct anim_frame_record gPolarGoalKeyframes[2] = {
    { 0, 0, 1, 0, 0x0, { 0, 0 } },
    { 0, 0, 1, 0, 0x1000, { 0, 0 } },
};

const u32 gPolarPenguinFrames[9] = {
    FRAMED_POLAR_PENGUIN_00, FRAMED_POLAR_PENGUIN_01, FRAMED_POLAR_PENGUIN_02,
    FRAMED_POLAR_PENGUIN_03, FRAMED_POLAR_PENGUIN_04, FRAMED_POLAR_PENGUIN_05,
    FRAMED_POLAR_PENGUIN_06, FRAMED_POLAR_PENGUIN_07, FRAMED_POLAR_PENGUIN_08,
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
    FRAMED_POLAR_BASIC_CRATE_00,  FRAMED_POLAR_BASIC_CRATE_01,  FRAMED_POLAR_BASIC_CRATE_02,
    FRAMED_POLAR_BASIC_CRATE_03,  FRAMED_POLAR_BASIC_CRATE_04,  FRAMED_POLAR_BASIC_CRATE_05,
    FRAMED_POLAR_BASIC_CRATE_06,  FRAMED_POLAR_BASIC_CRATE_07,  FRAMED_POLAR_BASIC_CRATE_08,
    FRAMED_POLAR_CRATE_SHARDS_00, FRAMED_POLAR_CRATE_SHARDS_01, FRAMED_POLAR_CRATE_SHARDS_02,
    FRAMED_POLAR_CRATE_SHARDS_03, FRAMED_POLAR_CRATE_SHARDS_04, FRAMED_POLAR_CRATE_SHARDS_05,
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
    FRAMED_POLAR_NITRO_CRATE_00,       FRAMED_POLAR_NITRO_CRATE_01,
    FRAMED_POLAR_NITRO_CRATE_02,       FRAMED_POLAR_NITRO_CRATE_03,
    FRAMED_POLAR_NITRO_CRATE_04,       FRAMED_POLAR_NITRO_CRATE_05,
    FRAMED_POLAR_NITRO_CRATE_06,       FRAMED_POLAR_NITRO_CRATE_07,
    FRAMED_POLAR_NITRO_CRATE_08,       FRAMED_POLAR_NITRO_CRATE_09,
    FRAMED_POLAR_NITRO_CRATE_10,       FRAMED_POLAR_NITRO_CRATE_11,
    FRAMED_POLAR_NITRO_CRATE_12,       FRAMED_POLAR_NITRO_CRATE_13,
    FRAMED_POLAR_NITRO_CRATE_14,       FRAMED_POLAR_NITRO_CRATE_15,
    FRAMED_POLAR_NITRO_CRATE_16,       FRAMED_POLAR_NITRO_CRATE_17,
    FRAMED_POLAR_NITRO_CRATE_BURST_00, FRAMED_POLAR_NITRO_CRATE_BURST_01,
    FRAMED_POLAR_NITRO_CRATE_BURST_02, FRAMED_POLAR_NITRO_CRATE_BURST_03,
    FRAMED_POLAR_NITRO_CRATE_BURST_04, FRAMED_POLAR_NITRO_CRATE_BURST_05,
    FRAMED_POLAR_NITRO_CRATE_BURST_06, FRAMED_POLAR_NITRO_CRATE_BURST_07,
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
    FRAMED_POLAR_TIME_CRATE_1_00,      FRAMED_POLAR_TIME_CRATE_1_01,
    FRAMED_POLAR_TIME_CRATE_1_02,      FRAMED_POLAR_TIME_CRATE_1_03,
    FRAMED_POLAR_TIME_CRATE_1_04,      FRAMED_POLAR_TIME_CRATE_1_05,
    FRAMED_POLAR_TIME_CRATE_1_06,      FRAMED_POLAR_TIME_CRATE_1_07,
    FRAMED_POLAR_TIME_CRATE_1_08,      FRAMED_POLAR_TIME_CRATE_1_09,
    FRAMED_POLAR_TIME_CRATE_1_10,      FRAMED_POLAR_TIME_CRATE_1_11,
    FRAMED_POLAR_TIME_CRATE_1_12,      FRAMED_POLAR_TIME_CRATE_1_13,
    FRAMED_POLAR_TIME_CRATE_1_14,      FRAMED_POLAR_TIME_CRATE_1_15,
    FRAMED_POLAR_TIME_CRATE_1_16,      FRAMED_POLAR_TIME_CRATE_1_17,
    FRAMED_POLAR_TIME_CRATE_SHARDS_00, FRAMED_POLAR_TIME_CRATE_SHARDS_01,
    FRAMED_POLAR_TIME_CRATE_SHARDS_02, FRAMED_POLAR_TIME_CRATE_SHARDS_03,
    FRAMED_POLAR_TIME_CRATE_SHARDS_04, FRAMED_POLAR_TIME_CRATE_SHARDS_05,
};

const u32 gPolarTimeCrate2Frames[24] = {
    FRAMED_POLAR_TIME_CRATE_2_00,      FRAMED_POLAR_TIME_CRATE_2_01,
    FRAMED_POLAR_TIME_CRATE_2_02,      FRAMED_POLAR_TIME_CRATE_2_03,
    FRAMED_POLAR_TIME_CRATE_2_04,      FRAMED_POLAR_TIME_CRATE_2_05,
    FRAMED_POLAR_TIME_CRATE_2_06,      FRAMED_POLAR_TIME_CRATE_2_07,
    FRAMED_POLAR_TIME_CRATE_2_08,      FRAMED_POLAR_TIME_CRATE_2_09,
    FRAMED_POLAR_TIME_CRATE_2_10,      FRAMED_POLAR_TIME_CRATE_2_11,
    FRAMED_POLAR_TIME_CRATE_2_12,      FRAMED_POLAR_TIME_CRATE_2_13,
    FRAMED_POLAR_TIME_CRATE_2_14,      FRAMED_POLAR_TIME_CRATE_2_15,
    FRAMED_POLAR_TIME_CRATE_2_16,      FRAMED_POLAR_TIME_CRATE_2_17,
    FRAMED_POLAR_TIME_CRATE_SHARDS_00, FRAMED_POLAR_TIME_CRATE_SHARDS_01,
    FRAMED_POLAR_TIME_CRATE_SHARDS_02, FRAMED_POLAR_TIME_CRATE_SHARDS_03,
    FRAMED_POLAR_TIME_CRATE_SHARDS_04, FRAMED_POLAR_TIME_CRATE_SHARDS_05,
};

const u32 gPolarTimeCrate3Frames[24] = {
    FRAMED_POLAR_TIME_CRATE_3_00,      FRAMED_POLAR_TIME_CRATE_3_01,
    FRAMED_POLAR_TIME_CRATE_3_02,      FRAMED_POLAR_TIME_CRATE_3_03,
    FRAMED_POLAR_TIME_CRATE_3_04,      FRAMED_POLAR_TIME_CRATE_3_05,
    FRAMED_POLAR_TIME_CRATE_3_06,      FRAMED_POLAR_TIME_CRATE_3_07,
    FRAMED_POLAR_TIME_CRATE_3_08,      FRAMED_POLAR_TIME_CRATE_3_09,
    FRAMED_POLAR_TIME_CRATE_3_10,      FRAMED_POLAR_TIME_CRATE_3_11,
    FRAMED_POLAR_TIME_CRATE_3_12,      FRAMED_POLAR_TIME_CRATE_3_13,
    FRAMED_POLAR_TIME_CRATE_3_14,      FRAMED_POLAR_TIME_CRATE_3_15,
    FRAMED_POLAR_TIME_CRATE_3_16,      FRAMED_POLAR_TIME_CRATE_3_17,
    FRAMED_POLAR_TIME_CRATE_SHARDS_00, FRAMED_POLAR_TIME_CRATE_SHARDS_01,
    FRAMED_POLAR_TIME_CRATE_SHARDS_02, FRAMED_POLAR_TIME_CRATE_SHARDS_03,
    FRAMED_POLAR_TIME_CRATE_SHARDS_04, FRAMED_POLAR_TIME_CRATE_SHARDS_05,
};

const u32 gPolarQuestionCrateFrames[24] = {
    FRAMED_POLAR_QUESTION_CRATE_00, FRAMED_POLAR_QUESTION_CRATE_01,
    FRAMED_POLAR_QUESTION_CRATE_02, FRAMED_POLAR_QUESTION_CRATE_03,
    FRAMED_POLAR_QUESTION_CRATE_04, FRAMED_POLAR_QUESTION_CRATE_05,
    FRAMED_POLAR_QUESTION_CRATE_06, FRAMED_POLAR_QUESTION_CRATE_07,
    FRAMED_POLAR_QUESTION_CRATE_08, FRAMED_POLAR_QUESTION_CRATE_09,
    FRAMED_POLAR_QUESTION_CRATE_10, FRAMED_POLAR_QUESTION_CRATE_11,
    FRAMED_POLAR_QUESTION_CRATE_12, FRAMED_POLAR_QUESTION_CRATE_13,
    FRAMED_POLAR_QUESTION_CRATE_14, FRAMED_POLAR_QUESTION_CRATE_15,
    FRAMED_POLAR_QUESTION_CRATE_16, FRAMED_POLAR_QUESTION_CRATE_17,
    FRAMED_POLAR_CRATE_SHARDS_00,   FRAMED_POLAR_CRATE_SHARDS_01,
    FRAMED_POLAR_CRATE_SHARDS_02,   FRAMED_POLAR_CRATE_SHARDS_03,
    FRAMED_POLAR_CRATE_SHARDS_04,   FRAMED_POLAR_CRATE_SHARDS_05,
};

const u32 gPolarLifeCrateFrames[24] = {
    FRAMED_POLAR_LIFE_CRATE_00,   FRAMED_POLAR_LIFE_CRATE_01,   FRAMED_POLAR_LIFE_CRATE_02,
    FRAMED_POLAR_LIFE_CRATE_03,   FRAMED_POLAR_LIFE_CRATE_04,   FRAMED_POLAR_LIFE_CRATE_05,
    FRAMED_POLAR_LIFE_CRATE_06,   FRAMED_POLAR_LIFE_CRATE_07,   FRAMED_POLAR_LIFE_CRATE_08,
    FRAMED_POLAR_LIFE_CRATE_09,   FRAMED_POLAR_LIFE_CRATE_10,   FRAMED_POLAR_LIFE_CRATE_11,
    FRAMED_POLAR_LIFE_CRATE_12,   FRAMED_POLAR_LIFE_CRATE_13,   FRAMED_POLAR_LIFE_CRATE_14,
    FRAMED_POLAR_LIFE_CRATE_15,   FRAMED_POLAR_LIFE_CRATE_16,   FRAMED_POLAR_LIFE_CRATE_17,
    FRAMED_POLAR_CRATE_SHARDS_00, FRAMED_POLAR_CRATE_SHARDS_01, FRAMED_POLAR_CRATE_SHARDS_02,
    FRAMED_POLAR_CRATE_SHARDS_03, FRAMED_POLAR_CRATE_SHARDS_04, FRAMED_POLAR_CRATE_SHARDS_05,
};

const u32 gPolarAkuAkuCrateFrames[24] = {
    FRAMED_POLAR_AKU_AKU_CRATE_00, FRAMED_POLAR_AKU_AKU_CRATE_01, FRAMED_POLAR_AKU_AKU_CRATE_02,
    FRAMED_POLAR_AKU_AKU_CRATE_03, FRAMED_POLAR_AKU_AKU_CRATE_04, FRAMED_POLAR_AKU_AKU_CRATE_05,
    FRAMED_POLAR_AKU_AKU_CRATE_06, FRAMED_POLAR_AKU_AKU_CRATE_07, FRAMED_POLAR_AKU_AKU_CRATE_08,
    FRAMED_POLAR_AKU_AKU_CRATE_09, FRAMED_POLAR_AKU_AKU_CRATE_10, FRAMED_POLAR_AKU_AKU_CRATE_11,
    FRAMED_POLAR_AKU_AKU_CRATE_12, FRAMED_POLAR_AKU_AKU_CRATE_13, FRAMED_POLAR_AKU_AKU_CRATE_14,
    FRAMED_POLAR_AKU_AKU_CRATE_15, FRAMED_POLAR_AKU_AKU_CRATE_16, FRAMED_POLAR_AKU_AKU_CRATE_17,
    FRAMED_POLAR_CRATE_SHARDS_00,  FRAMED_POLAR_CRATE_SHARDS_01,  FRAMED_POLAR_CRATE_SHARDS_02,
    FRAMED_POLAR_CRATE_SHARDS_03,  FRAMED_POLAR_CRATE_SHARDS_04,  FRAMED_POLAR_CRATE_SHARDS_05,
};

const u32 gPolarCheckpointCrateFrames[20] = {
    FRAMED_POLAR_CHECKPOINT_CRATE_00, FRAMED_POLAR_CHECKPOINT_CRATE_01,
    FRAMED_POLAR_CHECKPOINT_CRATE_02, FRAMED_POLAR_CHECKPOINT_CRATE_03,
    FRAMED_POLAR_CHECKPOINT_CRATE_04, FRAMED_POLAR_CHECKPOINT_CRATE_05,
    FRAMED_POLAR_CHECKPOINT_CRATE_06, FRAMED_POLAR_CHECKPOINT_CRATE_07,
    FRAMED_POLAR_CHECKPOINT_CRATE_08, FRAMED_POLAR_CHECKPOINT_CRATE_09,
    FRAMED_POLAR_CHECKPOINT_CRATE_10, FRAMED_POLAR_CHECKPOINT_CRATE_11,
    FRAMED_POLAR_CHECKPOINT_CRATE_12, FRAMED_POLAR_CHECKPOINT_CRATE_13,
    FRAMED_POLAR_CRATE_SHARDS_00,     FRAMED_POLAR_CRATE_SHARDS_01,
    FRAMED_POLAR_CRATE_SHARDS_02,     FRAMED_POLAR_CRATE_SHARDS_03,
    FRAMED_POLAR_CRATE_SHARDS_04,     FRAMED_POLAR_CRATE_SHARDS_05,
};

const struct anim_frame_record gPolarCheckpointCrateKeyframes[4] = {
    { 128, 0, 1, 0, 0x0, { 0, 0 } },
    { 128, 1, 13, 12, 0x0, { 0, 0 } },
    { 128, 13, 1, 0, 0x0, { 0, 0 } },
    { 51, 14, 6, 5, 0x0, { 0, 0 } },
};

const u32 gPolarAkuAkuFrames[11] = {
    FRAMED_POLAR_AKU_AKU_00,          FRAMED_POLAR_AKU_AKU_01,
    FRAMED_POLAR_AKU_AKU_01,          FRAMED_POLAR_AKU_AKU_01,
    FRAMED_POLAR_AKU_AKU_01,          FRAMED_POLAR_AKU_AKU_01,
    FRAMED_POLAR_AKU_AKU_FEATHERS_00, FRAMED_POLAR_AKU_AKU_FEATHERS_01,
    FRAMED_POLAR_AKU_AKU_FEATHERS_02, FRAMED_POLAR_AKU_AKU_FEATHERS_03,
    FRAMED_POLAR_AKU_AKU_FEATHERS_04,
};

const struct anim_frame_record gPolarAkuAkuKeyframes[2] = {
    { 34, 0, 6, 0, 0x0, { 0, 0 } },
    { 34, 6, 5, 4, 0x0, { 0, 0 } },
};

const struct anim_frame_record gPolarCheckpointTextKeyframes[1] = {
    { 8, 0, 3, 0, 0x0, { 0, 0 } },
};

const u32 gPolarCheckpointTextFrames[3] = {
    FRAMED_POLAR_CHECKPOINT_TEXT_00,
    FRAMED_POLAR_CHECKPOINT_TEXT_00,
    FRAMED_POLAR_CHECKPOINT_TEXT_00,
};
