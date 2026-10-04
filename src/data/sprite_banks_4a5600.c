#include "gba/types.h"
#include "sprite_bank.h"

/*
 * ROM 0x084a5600-0x084b0ae0: sprite banks 0-9 of the sprite-bank
 * animation system (gSpriteBankTable, include/sprite_bank.h). Linked in
 * ROM order between data/data.s sections by ldscript.txt - see
 * docs/data.md and docs/data_map.md ("gSpriteBankTable").
 *
 * Extracted once from baserom.gba by tools/sprite_banks.py; this file is
 * the source now. Per bank: the animations (a frame-index sequence each),
 * the frame pointer array, the frames (header plus the boxes/anchor of
 * their layout type), then every frame's piece positions and piece bytes.
 * A frame's tiles are an offset into the sprite tile pool
 * (src/data/sprite_tiles_2bf120.c), written relative to the pool range of
 * the bank that owns them (SPRITE_TILES_BANKnn). Editing a piece's shape,
 * adding a piece or moving the tiles means redrawing
 * graphics/sprites/bankNN_*.png to match.
 */

extern const u8 gSpriteBank00Tiles[];  /* sprite_tiles_2bf120.c, bank 0's tiles */
extern const u8 gFixedObjTiles[0xfa0];
extern const struct sprite_bank gSpriteBanks[56];
extern const struct sprite_anim gSpriteBank10Anims[2];
extern const struct sprite_frame *const gSpriteBank10Frames[20];
extern const struct sprite_anim gSpriteBank11Anims[2];
extern const struct sprite_frame *const gSpriteBank11Frames[25];
extern const struct sprite_anim gSpriteBank12Anims[7];
extern const struct sprite_frame *const gSpriteBank12Frames[57];
extern const struct sprite_anim gSpriteBank13Anims[2];
extern const struct sprite_frame *const gSpriteBank13Frames[19];
extern const struct sprite_anim gSpriteBank14Anims[2];
extern const struct sprite_frame *const gSpriteBank14Frames[23];
extern const struct sprite_anim gSpriteBank15Anims[5];
extern const struct sprite_frame *const gSpriteBank15Frames[58];
extern const struct sprite_anim gSpriteBank16Anims[2];
extern const struct sprite_frame *const gSpriteBank16Frames[24];
extern const struct sprite_anim gSpriteBank17Anims[1];
extern const struct sprite_frame *const gSpriteBank17Frames[15];
extern const struct sprite_anim gSpriteBank18Anims[2];
extern const struct sprite_frame *const gSpriteBank18Frames[14];
extern const struct sprite_anim gSpriteBank19Anims[2];
extern const struct sprite_frame *const gSpriteBank19Frames[35];
extern const struct sprite_anim gSpriteBank20Anims[2];
extern const struct sprite_frame *const gSpriteBank20Frames[32];
extern const struct sprite_anim gSpriteBank21Anims[2];
extern const struct sprite_frame *const gSpriteBank21Frames[15];
extern const struct sprite_anim gSpriteBank22Anims[2];
extern const struct sprite_frame *const gSpriteBank22Frames[31];
extern const struct sprite_anim gSpriteBank23Anims[5];
extern const struct sprite_frame *const gSpriteBank23Frames[59];
extern const struct sprite_anim gSpriteBank24Anims[6];
extern const struct sprite_frame *const gSpriteBank24Frames[74];
extern const struct sprite_anim gSpriteBank25Anims[1];
extern const struct sprite_frame *const gSpriteBank25Frames[16];
extern const struct sprite_anim gSpriteBank26Anims[2];
extern const struct sprite_frame *const gSpriteBank26Frames[14];
extern const struct sprite_anim gSpriteBank27Anims[2];
extern const struct sprite_frame *const gSpriteBank27Frames[30];
extern const struct sprite_anim gSpriteBank28Anims[1];
extern const struct sprite_frame *const gSpriteBank28Frames[3];
extern const struct sprite_anim gSpriteBank29Anims[4];
extern const struct sprite_frame *const gSpriteBank29Frames[42];
extern const struct sprite_anim gSpriteBank30Anims[3];
extern const struct sprite_frame *const gSpriteBank30Frames[42];
extern const struct sprite_anim gSpriteBank31Anims[36];
extern const struct sprite_frame *const gSpriteBank31Frames[112];
extern const struct sprite_anim gSpriteBank32Anims[9];
extern const struct sprite_frame *const gSpriteBank32Frames[17];
extern const struct sprite_anim gSpriteBank33Anims[3];
extern const struct sprite_frame *const gSpriteBank33Frames[16];
extern const struct sprite_anim gSpriteBank34Anims[5];
extern const struct sprite_frame *const gSpriteBank34Frames[22];
extern const struct sprite_anim gSpriteBank35Anims[2];
extern const struct sprite_frame *const gSpriteBank35Frames[28];
extern const struct sprite_anim gSpriteBank36Anims[1];
extern const struct sprite_frame *const gSpriteBank36Frames[10];
extern const struct sprite_anim gSpriteBank37Anims[1];
extern const struct sprite_frame *const gSpriteBank37Frames[8];
extern const struct sprite_anim gSpriteBank38Anims[4];
extern const struct sprite_frame *const gSpriteBank38Frames[60];
extern const struct sprite_anim gSpriteBank39Anims[13];
extern const struct sprite_frame *const gSpriteBank39Frames[24];
extern const struct sprite_anim gSpriteBank40Anims[5];
extern const struct sprite_frame *const gSpriteBank40Frames[30];
extern const struct sprite_anim gSpriteBank41Anims[3];
extern const struct sprite_frame *const gSpriteBank41Frames[24];
extern const struct sprite_anim gSpriteBank42Anims[1];
extern const struct sprite_frame *const gSpriteBank42Frames[11];
extern const struct sprite_anim gSpriteBank43Anims[7];
extern const struct sprite_frame *const gSpriteBank43Frames[21];
extern const struct sprite_anim gSpriteBank44Anims[1];
extern const struct sprite_frame *const gSpriteBank44Frames[13];
extern const struct sprite_anim gSpriteBank45Anims[1];
extern const struct sprite_frame *const gSpriteBank45Frames[13];
extern const struct sprite_anim gSpriteBank46Anims[1];
extern const struct sprite_frame *const gSpriteBank46Frames[2];
extern const struct sprite_anim gSpriteBank47Anims[13];
extern const struct sprite_frame *const gSpriteBank47Frames[43];
extern const struct sprite_anim gSpriteBank48Anims[4];
extern const struct sprite_frame *const gSpriteBank48Frames[38];
extern const struct sprite_anim gSpriteBank49Anims[5];
extern const struct sprite_frame *const gSpriteBank49Frames[2];
extern const struct sprite_anim gSpriteBank50Anims[1];
extern const struct sprite_frame *const gSpriteBank50Frames[8];
extern const struct sprite_anim gSpriteBank51Anims[5];
extern const struct sprite_frame *const gSpriteBank51Frames[24];
extern const struct sprite_anim gSpriteBank52Anims[2];
extern const struct sprite_frame *const gSpriteBank52Frames[4];
extern const struct sprite_anim gSpriteBank53Anims[19];
extern const struct sprite_frame *const gSpriteBank53Frames[67];
extern const struct sprite_anim gSpriteBank54Anims[10];
extern const struct sprite_frame *const gSpriteBank54Frames[121];
extern const struct sprite_anim gSpriteBank55Anims[9];
extern const struct sprite_frame *const gSpriteBank55Frames[130];
extern const struct sprite_anim gSpriteBank00Anims[48];
extern const struct sprite_frame *const gSpriteBank00Frames[465];
extern const struct sprite_anim gSpriteBank01Anims[47];
extern const struct sprite_frame *const gSpriteBank01Frames[274];
extern const struct sprite_anim gSpriteBank02Anims[2];
extern const struct sprite_frame *const gSpriteBank02Frames[17];
extern const struct sprite_anim gSpriteBank03Anims[1];
extern const struct sprite_frame *const gSpriteBank03Frames[25];
extern const struct sprite_anim gSpriteBank04Anims[2];
extern const struct sprite_frame *const gSpriteBank04Frames[32];
extern const struct sprite_anim gSpriteBank05Anims[4];
extern const struct sprite_frame *const gSpriteBank05Frames[25];
extern const struct sprite_anim gSpriteBank06Anims[1];
extern const struct sprite_frame *const gSpriteBank06Frames[7];
extern const struct sprite_anim gSpriteBank07Anims[3];
extern const struct sprite_frame *const gSpriteBank07Frames[38];
extern const struct sprite_anim gSpriteBank08Anims[3];
extern const struct sprite_frame *const gSpriteBank08Frames[36];
extern const struct sprite_anim gSpriteBank09Anims[1];
extern const struct sprite_frame *const gSpriteBank09Frames[14];

/* The root of the system: sub_8022230 (graphics_loading_21d80.c) points
 * *gUnknown_030012D0 here. GetSpriteTileBase returns tileBase;
 * sub_8022230 and RunPauseMenu (settings_menu15.c) build the tile-asset
 * cache from tilePool/tilePoolCount. */
const struct sprite_bank_table gSpriteBankTable = {
    .banks = gSpriteBanks,
    .tileBase = gSpriteBank00Tiles,
    .tilePool = gFixedObjTiles,
    .bankCount = ARRAY_COUNT(gSpriteBanks),
    .tilePoolCount = sizeof(gFixedObjTiles) / 32,
};

/* Bank N is `**gUnknown_030012D0 + 12 * N` in the code (a part's +0x20). */
const struct sprite_bank gSpriteBanks[56] = {
    [0] = { gSpriteBank00Anims, gSpriteBank00Frames, 0, ARRAY_COUNT(gSpriteBank00Anims) },
    [1] = { gSpriteBank01Anims, gSpriteBank01Frames, 0, ARRAY_COUNT(gSpriteBank01Anims) },
    [2] = { gSpriteBank02Anims, gSpriteBank02Frames, 0, ARRAY_COUNT(gSpriteBank02Anims) },
    [3] = { gSpriteBank03Anims, gSpriteBank03Frames, 0, ARRAY_COUNT(gSpriteBank03Anims) },
    [4] = { gSpriteBank04Anims, gSpriteBank04Frames, 0, ARRAY_COUNT(gSpriteBank04Anims) },
    [5] = { gSpriteBank05Anims, gSpriteBank05Frames, 0, ARRAY_COUNT(gSpriteBank05Anims) },
    [6] = { gSpriteBank06Anims, gSpriteBank06Frames, 0, ARRAY_COUNT(gSpriteBank06Anims) },
    [7] = { gSpriteBank07Anims, gSpriteBank07Frames, 0, ARRAY_COUNT(gSpriteBank07Anims) },
    [8] = { gSpriteBank08Anims, gSpriteBank08Frames, 0, ARRAY_COUNT(gSpriteBank08Anims) },
    [9] = { gSpriteBank09Anims, gSpriteBank09Frames, 0, ARRAY_COUNT(gSpriteBank09Anims) },
    [10] = { gSpriteBank10Anims, gSpriteBank10Frames, 0, ARRAY_COUNT(gSpriteBank10Anims) },
    [11] = { gSpriteBank11Anims, gSpriteBank11Frames, 0, ARRAY_COUNT(gSpriteBank11Anims) },
    [12] = { gSpriteBank12Anims, gSpriteBank12Frames, 0, ARRAY_COUNT(gSpriteBank12Anims) },
    [13] = { gSpriteBank13Anims, gSpriteBank13Frames, 0, ARRAY_COUNT(gSpriteBank13Anims) },
    [14] = { gSpriteBank14Anims, gSpriteBank14Frames, 0, ARRAY_COUNT(gSpriteBank14Anims) },
    [15] = { gSpriteBank15Anims, gSpriteBank15Frames, 0, ARRAY_COUNT(gSpriteBank15Anims) },
    [16] = { gSpriteBank16Anims, gSpriteBank16Frames, 0, ARRAY_COUNT(gSpriteBank16Anims) },
    [17] = { gSpriteBank17Anims, gSpriteBank17Frames, 0, ARRAY_COUNT(gSpriteBank17Anims) },
    [18] = { gSpriteBank18Anims, gSpriteBank18Frames, 0, ARRAY_COUNT(gSpriteBank18Anims) },
    [19] = { gSpriteBank19Anims, gSpriteBank19Frames, 0, ARRAY_COUNT(gSpriteBank19Anims) },
    [20] = { gSpriteBank20Anims, gSpriteBank20Frames, 0, ARRAY_COUNT(gSpriteBank20Anims) },
    [21] = { gSpriteBank21Anims, gSpriteBank21Frames, 0, ARRAY_COUNT(gSpriteBank21Anims) },
    [22] = { gSpriteBank22Anims, gSpriteBank22Frames, 0, ARRAY_COUNT(gSpriteBank22Anims) },
    [23] = { gSpriteBank23Anims, gSpriteBank23Frames, 0, ARRAY_COUNT(gSpriteBank23Anims) },
    [24] = { gSpriteBank24Anims, gSpriteBank24Frames, 0, ARRAY_COUNT(gSpriteBank24Anims) },
    [25] = { gSpriteBank25Anims, gSpriteBank25Frames, 0, ARRAY_COUNT(gSpriteBank25Anims) },
    [26] = { gSpriteBank26Anims, gSpriteBank26Frames, 0, ARRAY_COUNT(gSpriteBank26Anims) },
    [27] = { gSpriteBank27Anims, gSpriteBank27Frames, 0, ARRAY_COUNT(gSpriteBank27Anims) },
    [28] = { gSpriteBank28Anims, gSpriteBank28Frames, 0, ARRAY_COUNT(gSpriteBank28Anims) },
    [29] = { gSpriteBank29Anims, gSpriteBank29Frames, 0, ARRAY_COUNT(gSpriteBank29Anims) },
    [30] = { gSpriteBank30Anims, gSpriteBank30Frames, 0, ARRAY_COUNT(gSpriteBank30Anims) },
    [31] = { gSpriteBank31Anims, gSpriteBank31Frames, 0, ARRAY_COUNT(gSpriteBank31Anims) },
    [32] = { gSpriteBank32Anims, gSpriteBank32Frames, 0, ARRAY_COUNT(gSpriteBank32Anims) },
    [33] = { gSpriteBank33Anims, gSpriteBank33Frames, 0, ARRAY_COUNT(gSpriteBank33Anims) },
    [34] = { gSpriteBank34Anims, gSpriteBank34Frames, 0, ARRAY_COUNT(gSpriteBank34Anims) },
    [35] = { gSpriteBank35Anims, gSpriteBank35Frames, 0, ARRAY_COUNT(gSpriteBank35Anims) },
    [36] = { gSpriteBank36Anims, gSpriteBank36Frames, 0, ARRAY_COUNT(gSpriteBank36Anims) },
    [37] = { gSpriteBank37Anims, gSpriteBank37Frames, 0, ARRAY_COUNT(gSpriteBank37Anims) },
    [38] = { gSpriteBank38Anims, gSpriteBank38Frames, 0, ARRAY_COUNT(gSpriteBank38Anims) },
    [39] = { gSpriteBank39Anims, gSpriteBank39Frames, 0, ARRAY_COUNT(gSpriteBank39Anims) },
    [40] = { gSpriteBank40Anims, gSpriteBank40Frames, 0, ARRAY_COUNT(gSpriteBank40Anims) },
    [41] = { gSpriteBank41Anims, gSpriteBank41Frames, 0, ARRAY_COUNT(gSpriteBank41Anims) },
    [42] = { gSpriteBank42Anims, gSpriteBank42Frames, 0, ARRAY_COUNT(gSpriteBank42Anims) },
    [43] = { gSpriteBank43Anims, gSpriteBank43Frames, 0, ARRAY_COUNT(gSpriteBank43Anims) },
    [44] = { gSpriteBank44Anims, gSpriteBank44Frames, 0, ARRAY_COUNT(gSpriteBank44Anims) },
    [45] = { gSpriteBank45Anims, gSpriteBank45Frames, 0, ARRAY_COUNT(gSpriteBank45Anims) },
    [46] = { gSpriteBank46Anims, gSpriteBank46Frames, 0, ARRAY_COUNT(gSpriteBank46Anims) },
    [47] = { gSpriteBank47Anims, gSpriteBank47Frames, 0, ARRAY_COUNT(gSpriteBank47Anims) },
    [48] = { gSpriteBank48Anims, gSpriteBank48Frames, 0, ARRAY_COUNT(gSpriteBank48Anims) },
    [49] = { gSpriteBank49Anims, gSpriteBank49Frames, 0, ARRAY_COUNT(gSpriteBank49Anims) },
    [50] = { gSpriteBank50Anims, gSpriteBank50Frames, 0, ARRAY_COUNT(gSpriteBank50Anims) },
    [51] = { gSpriteBank51Anims, gSpriteBank51Frames, 0, ARRAY_COUNT(gSpriteBank51Anims) },
    [52] = { gSpriteBank52Anims, gSpriteBank52Frames, 0, ARRAY_COUNT(gSpriteBank52Anims) },
    [53] = { gSpriteBank53Anims, gSpriteBank53Frames, 0, ARRAY_COUNT(gSpriteBank53Anims) },
    [54] = { gSpriteBank54Anims, gSpriteBank54Frames, 0, ARRAY_COUNT(gSpriteBank54Anims) },
    [55] = { gSpriteBank55Anims, gSpriteBank55Frames, 0, ARRAY_COUNT(gSpriteBank55Anims) },
};

/* ---------------------------------------------------------------------- */
/* Bank 0: 48 animations, 465 frames, tiles in gSpriteBank00Tiles (SPRITE_TILES_BANK00). */

extern const u16 gSpriteBank00Anim00Seq[12];
extern const u16 gSpriteBank00Anim01Seq[3];
extern const u16 gSpriteBank00Anim02Seq[4];
extern const u16 gSpriteBank00Anim03Seq[4];
extern const u16 gSpriteBank00Anim04Seq[1];
extern const u16 gSpriteBank00Anim05Seq[24];
extern const u16 gSpriteBank00Anim06Seq[9];
extern const u16 gSpriteBank00Anim07Seq[7];
extern const u16 gSpriteBank00Anim08Seq[3];
extern const u16 gSpriteBank00Anim09Seq[3];
extern const u16 gSpriteBank00Anim10Seq[10];
extern const u16 gSpriteBank00Anim11Seq[12];
extern const u16 gSpriteBank00Anim12Seq[10];
extern const u16 gSpriteBank00Anim13Seq[20];
extern const u16 gSpriteBank00Anim14Seq[24];
extern const u16 gSpriteBank00Anim15Seq[5];
extern const u16 gSpriteBank00Anim16Seq[10];
extern const u16 gSpriteBank00Anim17Seq[29];
extern const u16 gSpriteBank00Anim18Seq[10];
extern const u16 gSpriteBank00Anim19Seq[3];
extern const u16 gSpriteBank00Anim20Seq[3];
extern const u16 gSpriteBank00Anim21Seq[3];
extern const u16 gSpriteBank00Anim22Seq[13];
extern const u16 gSpriteBank00Anim23Seq[10];
extern const u16 gSpriteBank00Anim24Seq[5];
extern const u16 gSpriteBank00Anim25Seq[4];
extern const u16 gSpriteBank00Anim26Seq[107];
extern const u16 gSpriteBank00Anim27Seq[3];
extern const u16 gSpriteBank00Anim28Seq[8];
extern const u16 gSpriteBank00Anim29Seq[2];
extern const u16 gSpriteBank00Anim30Seq[10];
extern const u16 gSpriteBank00Anim31Seq[15];
extern const u16 gSpriteBank00Anim32Seq[3];
extern const u16 gSpriteBank00Anim33Seq[14];
extern const u16 gSpriteBank00Anim34Seq[3];
extern const u16 gSpriteBank00Anim35Seq[3];
extern const u16 gSpriteBank00Anim36Seq[19];
extern const u16 gSpriteBank00Anim37Seq[8];
extern const u16 gSpriteBank00Anim38Seq[12];
extern const u16 gSpriteBank00Anim39Seq[10];
extern const u16 gSpriteBank00Anim40Seq[10];
extern const u16 gSpriteBank00Anim41Seq[19];
extern const u16 gSpriteBank00Anim42Seq[35];
extern const u16 gSpriteBank00Anim43Seq[27];
extern const u16 gSpriteBank00Anim44Seq[15];
extern const u16 gSpriteBank00Anim45Seq[16];
extern const u16 gSpriteBank00Anim46Seq[24];
extern const u16 gSpriteBank00Anim47Seq[10];
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame000;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame001;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame002;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame003;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame004;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame005;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame006;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame007;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame008;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame009;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame010;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame011;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame012;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame013;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame014;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame015;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame016;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame017;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame018;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame019;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame020;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame021;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame022;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame023;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame024;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame025;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame026;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame027;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame028;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame029;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame030;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame031;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame032;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame033;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame034;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame035;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame036;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame037;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame038;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame039;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame040;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame041;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame042;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame043;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame044;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame045;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame046;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame047;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame048;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame049;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame050;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame051;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame052;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame053;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame054;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame055;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame056;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame057;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame058;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame059;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame060;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame061;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame062;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame063;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame064;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame065;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame066;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame067;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame068;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame069;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame070;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame071;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame072;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame073;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame074;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame075;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame076;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame077;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame078;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame079;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame080;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame081;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame082;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame083;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame084;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame085;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame086;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame087;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame088;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame089;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame090;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame091;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame092;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame093;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame094;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame095;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame096;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame097;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame098;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame099;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame100;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame101;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame102;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame103;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame104;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame105;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame106;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame107;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame108;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame109;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame110;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame111;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame112;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame113;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame114;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame115;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame116;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame117;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame118;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame119;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame120;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame121;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame122;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame123;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame124;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame125;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame126;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame127;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame128;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame129;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame130;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame131;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame132;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame133;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame134;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame135;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame136;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame137;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame138;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame139;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame140;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame141;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame142;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame143;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame144;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame145;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame146;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame147;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame148;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame149;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame150;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame151;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame152;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame153;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame154;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame155;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame156;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame157;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame158;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame159;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame160;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame161;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame162;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame163;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame164;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame165;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame166;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame167;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame168;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame169;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame170;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame171;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame172;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame173;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame174;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame175;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame176;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame177;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame178;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame179;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame180;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame181;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame182;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame183;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame184;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame185;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame186;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame187;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame188;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame189;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame190;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame191;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame192;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame193;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame194;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame195;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame196;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame197;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame198;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame199;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame200;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame201;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame202;
extern const struct sprite_frame_3box gSpriteBank00Frame203;
extern const struct sprite_frame_3box gSpriteBank00Frame204;
extern const struct sprite_frame_3box gSpriteBank00Frame205;
extern const struct sprite_frame_3box gSpriteBank00Frame206;
extern const struct sprite_frame_3box gSpriteBank00Frame207;
extern const struct sprite_frame_3box gSpriteBank00Frame208;
extern const struct sprite_frame_3box gSpriteBank00Frame209;
extern const struct sprite_frame_3box gSpriteBank00Frame210;
extern const struct sprite_frame_3box gSpriteBank00Frame211;
extern const struct sprite_frame_3box gSpriteBank00Frame212;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame213;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame214;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame215;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame216;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame217;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame218;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame219;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame220;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame221;
extern const struct sprite_frame_1box gSpriteBank00Frame222;
extern const struct sprite_frame_1box gSpriteBank00Frame223;
extern const struct sprite_frame_1box gSpriteBank00Frame224;
extern const struct sprite_frame_1box gSpriteBank00Frame225;
extern const struct sprite_frame_1box gSpriteBank00Frame226;
extern const struct sprite_frame_1box gSpriteBank00Frame227;
extern const struct sprite_frame_1box gSpriteBank00Frame228;
extern const struct sprite_frame_1box gSpriteBank00Frame229;
extern const struct sprite_frame_1box gSpriteBank00Frame230;
extern const struct sprite_frame_1box gSpriteBank00Frame231;
extern const struct sprite_frame_1box gSpriteBank00Frame232;
extern const struct sprite_frame_1box gSpriteBank00Frame233;
extern const struct sprite_frame_1box gSpriteBank00Frame234;
extern const struct sprite_frame_1box gSpriteBank00Frame235;
extern const struct sprite_frame_1box gSpriteBank00Frame236;
extern const struct sprite_frame_1box gSpriteBank00Frame237;
extern const struct sprite_frame_1box gSpriteBank00Frame238;
extern const struct sprite_frame_1box gSpriteBank00Frame239;
extern const struct sprite_frame_1box gSpriteBank00Frame240;
extern const struct sprite_frame_1box gSpriteBank00Frame241;
extern const struct sprite_frame_1box gSpriteBank00Frame242;
extern const struct sprite_frame_1box gSpriteBank00Frame243;
extern const struct sprite_frame_1box gSpriteBank00Frame244;
extern const struct sprite_frame_1box gSpriteBank00Frame245;
extern const struct sprite_frame_1box gSpriteBank00Frame246;
extern const struct sprite_frame_1box gSpriteBank00Frame247;
extern const struct sprite_frame_1box gSpriteBank00Frame248;
extern const struct sprite_frame_1box gSpriteBank00Frame249;
extern const struct sprite_frame_1box gSpriteBank00Frame250;
extern const struct sprite_frame_1box gSpriteBank00Frame251;
extern const struct sprite_frame_1box gSpriteBank00Frame252;
extern const struct sprite_frame_1box gSpriteBank00Frame253;
extern const struct sprite_frame_1box gSpriteBank00Frame254;
extern const struct sprite_frame_1box gSpriteBank00Frame255;
extern const struct sprite_frame_1box gSpriteBank00Frame256;
extern const struct sprite_frame_1box gSpriteBank00Frame257;
extern const struct sprite_frame_1box gSpriteBank00Frame258;
extern const struct sprite_frame_1box gSpriteBank00Frame259;
extern const struct sprite_frame_1box gSpriteBank00Frame260;
extern const struct sprite_frame_1box gSpriteBank00Frame261;
extern const struct sprite_frame_1box gSpriteBank00Frame262;
extern const struct sprite_frame_1box gSpriteBank00Frame263;
extern const struct sprite_frame_1box gSpriteBank00Frame264;
extern const struct sprite_frame_1box gSpriteBank00Frame265;
extern const struct sprite_frame_1box gSpriteBank00Frame266;
extern const struct sprite_frame_1box gSpriteBank00Frame267;
extern const struct sprite_frame_1box gSpriteBank00Frame268;
extern const struct sprite_frame_1box gSpriteBank00Frame269;
extern const struct sprite_frame_1box gSpriteBank00Frame270;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame271;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame272;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame273;
extern const struct sprite_frame_1box gSpriteBank00Frame274;
extern const struct sprite_frame_1box gSpriteBank00Frame275;
extern const struct sprite_frame_1box gSpriteBank00Frame276;
extern const struct sprite_frame_1box gSpriteBank00Frame277;
extern const struct sprite_frame_1box gSpriteBank00Frame278;
extern const struct sprite_frame_1box gSpriteBank00Frame279;
extern const struct sprite_frame_1box gSpriteBank00Frame280;
extern const struct sprite_frame_1box gSpriteBank00Frame281;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame282;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame283;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame284;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame285;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame286;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame287;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame288;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame289;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame290;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame291;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame292;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame293;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame294;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame295;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame296;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame297;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame298;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame299;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame300;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame301;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame302;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame303;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame304;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame305;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame306;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame307;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame308;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame309;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame310;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame311;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame312;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame313;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame314;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame315;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame316;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame317;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame318;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame319;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame320;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame321;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame322;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame323;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame324;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame325;
extern const struct sprite_frame_1box_anchor gSpriteBank00Frame326;
extern const struct sprite_frame gSpriteBank00Frame327;
extern const struct sprite_frame gSpriteBank00Frame328;
extern const struct sprite_frame gSpriteBank00Frame329;
extern const struct sprite_frame gSpriteBank00Frame330;
extern const struct sprite_frame gSpriteBank00Frame331;
extern const struct sprite_frame gSpriteBank00Frame332;
extern const struct sprite_frame gSpriteBank00Frame333;
extern const struct sprite_frame gSpriteBank00Frame334;
extern const struct sprite_frame gSpriteBank00Frame335;
extern const struct sprite_frame gSpriteBank00Frame336;
extern const struct sprite_frame gSpriteBank00Frame337;
extern const struct sprite_frame gSpriteBank00Frame338;
extern const struct sprite_frame gSpriteBank00Frame339;
extern const struct sprite_frame gSpriteBank00Frame340;
extern const struct sprite_frame gSpriteBank00Frame341;
extern const struct sprite_frame gSpriteBank00Frame342;
extern const struct sprite_frame gSpriteBank00Frame343;
extern const struct sprite_frame gSpriteBank00Frame344;
extern const struct sprite_frame gSpriteBank00Frame345;
extern const struct sprite_frame_1box gSpriteBank00Frame346;
extern const struct sprite_frame_1box gSpriteBank00Frame347;
extern const struct sprite_frame_1box gSpriteBank00Frame348;
extern const struct sprite_frame_1box gSpriteBank00Frame349;
extern const struct sprite_frame_1box gSpriteBank00Frame350;
extern const struct sprite_frame_1box gSpriteBank00Frame351;
extern const struct sprite_frame_1box gSpriteBank00Frame352;
extern const struct sprite_frame_1box gSpriteBank00Frame353;
extern const struct sprite_frame_1box gSpriteBank00Frame354;
extern const struct sprite_frame_1box gSpriteBank00Frame355;
extern const struct sprite_frame_1box gSpriteBank00Frame356;
extern const struct sprite_frame_1box gSpriteBank00Frame357;
extern const struct sprite_frame_1box gSpriteBank00Frame358;
extern const struct sprite_frame_1box gSpriteBank00Frame359;
extern const struct sprite_frame_1box gSpriteBank00Frame360;
extern const struct sprite_frame_1box gSpriteBank00Frame361;
extern const struct sprite_frame_1box gSpriteBank00Frame362;
extern const struct sprite_frame_1box gSpriteBank00Frame363;
extern const struct sprite_frame_1box gSpriteBank00Frame364;
extern const struct sprite_frame_1box gSpriteBank00Frame365;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame366;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame367;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame368;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame369;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame370;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame371;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame372;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame373;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame374;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame375;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame376;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame377;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame378;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame379;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame380;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame381;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame382;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame383;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame384;
extern const struct sprite_frame_3box_anchor gSpriteBank00Frame385;
extern const struct sprite_frame gSpriteBank00Frame386;
extern const struct sprite_frame gSpriteBank00Frame387;
extern const struct sprite_frame gSpriteBank00Frame388;
extern const struct sprite_frame gSpriteBank00Frame389;
extern const struct sprite_frame gSpriteBank00Frame390;
extern const struct sprite_frame gSpriteBank00Frame391;
extern const struct sprite_frame gSpriteBank00Frame392;
extern const struct sprite_frame gSpriteBank00Frame393;
extern const struct sprite_frame gSpriteBank00Frame394;
extern const struct sprite_frame gSpriteBank00Frame395;
extern const struct sprite_frame gSpriteBank00Frame396;
extern const struct sprite_frame gSpriteBank00Frame397;
extern const struct sprite_frame gSpriteBank00Frame398;
extern const struct sprite_frame gSpriteBank00Frame399;
extern const struct sprite_frame gSpriteBank00Frame400;
extern const struct sprite_frame gSpriteBank00Frame401;
extern const struct sprite_frame gSpriteBank00Frame402;
extern const struct sprite_frame gSpriteBank00Frame403;
extern const struct sprite_frame_1box gSpriteBank00Frame404;
extern const struct sprite_frame_1box gSpriteBank00Frame405;
extern const struct sprite_frame_1box gSpriteBank00Frame406;
extern const struct sprite_frame_1box gSpriteBank00Frame407;
extern const struct sprite_frame_1box gSpriteBank00Frame408;
extern const struct sprite_frame_1box gSpriteBank00Frame409;
extern const struct sprite_frame_1box gSpriteBank00Frame410;
extern const struct sprite_frame_1box gSpriteBank00Frame411;
extern const struct sprite_frame_1box gSpriteBank00Frame412;
extern const struct sprite_frame_1box gSpriteBank00Frame413;
extern const struct sprite_frame_1box gSpriteBank00Frame414;
extern const struct sprite_frame_1box gSpriteBank00Frame415;
extern const struct sprite_frame_1box gSpriteBank00Frame416;
extern const struct sprite_frame_1box gSpriteBank00Frame417;
extern const struct sprite_frame_1box gSpriteBank00Frame418;
extern const struct sprite_frame_1box gSpriteBank00Frame419;
extern const struct sprite_frame_1box gSpriteBank00Frame420;
extern const struct sprite_frame_1box gSpriteBank00Frame421;
extern const struct sprite_frame_1box gSpriteBank00Frame422;
extern const struct sprite_frame_1box gSpriteBank00Frame423;
extern const struct sprite_frame_1box gSpriteBank00Frame424;
extern const struct sprite_frame_1box gSpriteBank00Frame425;
extern const struct sprite_frame_1box gSpriteBank00Frame426;
extern const struct sprite_frame_1box gSpriteBank00Frame427;
extern const struct sprite_frame_1box gSpriteBank00Frame428;
extern const struct sprite_frame_1box gSpriteBank00Frame429;
extern const struct sprite_frame_1box gSpriteBank00Frame430;
extern const struct sprite_frame_1box gSpriteBank00Frame431;
extern const struct sprite_frame_1box gSpriteBank00Frame432;
extern const struct sprite_frame_1box gSpriteBank00Frame433;
extern const struct sprite_frame_1box gSpriteBank00Frame434;
extern const struct sprite_frame_1box gSpriteBank00Frame435;
extern const struct sprite_frame_1box gSpriteBank00Frame436;
extern const struct sprite_frame_1box gSpriteBank00Frame437;
extern const struct sprite_frame_1box gSpriteBank00Frame438;
extern const struct sprite_frame_1box gSpriteBank00Frame439;
extern const struct sprite_frame gSpriteBank00Frame440;
extern const struct sprite_frame_1box gSpriteBank00Frame441;
extern const struct sprite_frame_1box gSpriteBank00Frame442;
extern const struct sprite_frame_1box gSpriteBank00Frame443;
extern const struct sprite_frame_1box gSpriteBank00Frame444;
extern const struct sprite_frame_1box gSpriteBank00Frame445;
extern const struct sprite_frame_1box gSpriteBank00Frame446;
extern const struct sprite_frame_1box gSpriteBank00Frame447;
extern const struct sprite_frame_1box gSpriteBank00Frame448;
extern const struct sprite_frame_1box gSpriteBank00Frame449;
extern const struct sprite_frame_1box gSpriteBank00Frame450;
extern const struct sprite_frame_1box gSpriteBank00Frame451;
extern const struct sprite_frame_1box gSpriteBank00Frame452;
extern const struct sprite_frame_1box gSpriteBank00Frame453;
extern const struct sprite_frame_1box gSpriteBank00Frame454;
extern const struct sprite_frame_1box gSpriteBank00Frame455;
extern const struct sprite_frame_1box gSpriteBank00Frame456;
extern const struct sprite_frame_1box gSpriteBank00Frame457;
extern const struct sprite_frame_1box gSpriteBank00Frame458;
extern const struct sprite_frame_1box gSpriteBank00Frame459;
extern const struct sprite_frame_1box gSpriteBank00Frame460;
extern const struct sprite_frame_1box gSpriteBank00Frame461;
extern const struct sprite_frame_1box gSpriteBank00Frame462;
extern const struct sprite_frame_1box gSpriteBank00Frame463;
extern const struct sprite_frame_1box gSpriteBank00Frame464;
extern const struct sprite_piece_pos gSpriteBank00Frame000Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame001Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame002Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame003Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame004Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame005Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame006Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame007Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame008Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame009Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame010Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame011Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame012Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame013Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame014Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame015Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame016Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame017Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame018Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame019Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame020Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame021Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame022Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame023Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame024Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame025Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame026Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame027Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame028Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame029Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame030Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame031Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame032Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame033Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame034Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame035Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame036Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame037Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame038Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame039Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame040Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame041Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame042Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame043Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame044Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame045Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame046Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame047Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame048Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame049Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame050Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame051Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame052Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame053Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame054Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame055Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame056Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame057Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame058Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame059Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame060Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame061Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame062Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame063Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame064Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame065Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame066Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame067Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame068Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame069Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame070Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame071Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame072Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame073Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame074Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame075Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame076Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame077Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame078Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame079Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame080Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame081Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame082Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame083Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame084Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame085Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame086Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame087Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame088Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame089Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame090Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame091Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame092Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame093Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame094Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame095Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame096Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame097Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame098Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame099Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame100Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame101Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame102Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame103Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame104Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame105Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame106Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame107Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame108Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame109Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame110Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame111Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame112Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame113Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame114Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame115Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame116Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame117Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame118Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame119Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame120Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame121Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame122Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame123Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame124Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame125Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame126Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame127Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame128Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame129Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame130Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame131Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame132Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame133Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame134Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame135Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame136Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame137Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame138Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame139Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame140Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame141Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame142Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame143Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame144Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame145Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame146Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame147Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame148Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame149Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame150Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame151Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame152Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame153Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame154Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame155Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame156Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame157Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame158Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame159Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame160Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame161Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame162Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame163Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame164Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame165Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame166Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame167Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame168Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame169Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame170Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame171Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame172Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame173Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame174Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame175Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame176Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame177Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame178Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame179Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame180Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame181Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame182Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame183Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame184Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame185Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame186Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame187Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame188Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame189Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame190Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame191Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame192Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame193Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame194Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame195Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame196Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame197Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame198Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame199Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame200Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame201Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame202Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame203Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame204Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame205Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame206Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame207Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame208Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame209Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame210Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame211Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame212Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame213Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame214Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame215Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame216Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame217Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame218Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame219Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame220Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame221Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame222Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame223Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame224Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame225Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame226Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame227Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame228Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame229Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame230Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame231Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame232Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame233Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame234Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame235Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame236Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame237Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame238Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame239Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame240Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame241Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame242Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame243Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame244Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame245Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame246Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame247Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame248Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame249Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame250Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame251Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame252Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame253Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame254Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame255Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame256Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame257Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame258Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame259Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame260Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame261Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame262Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame263Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame264Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame265Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame266Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame267Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame268Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame269Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame270Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame271Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame272Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame273Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame274Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame275Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame276Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame277Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame278Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame279Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame280Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame281Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame282Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame283Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame284Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame285Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame286Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame287Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame288Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame289Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame290Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame291Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame292Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame293Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame294Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame295Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame296Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame297Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame298Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame299Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame300Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame301Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame302Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame303Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame304Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame305Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame306Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame307Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame308Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame309Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame310Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame311Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame312Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame313Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame314Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame315Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame316Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame317Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame318Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame319Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame320Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame321Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame322Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame323Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame324Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame325Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame326Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame327Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame328Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame329Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame330Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame331Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame332Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame333Pos[9];
extern const struct sprite_piece_pos gSpriteBank00Frame334Pos[7];
extern const struct sprite_piece_pos gSpriteBank00Frame335Pos[9];
extern const struct sprite_piece_pos gSpriteBank00Frame336Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame337Pos[10];
extern const struct sprite_piece_pos gSpriteBank00Frame338Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame339Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame340Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame341Pos[6];
extern const struct sprite_piece_pos gSpriteBank00Frame342Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame343Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame344Pos[6];
extern const struct sprite_piece_pos gSpriteBank00Frame345Pos[8];
extern const struct sprite_piece_pos gSpriteBank00Frame346Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame347Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame348Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame349Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame350Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame351Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame352Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame353Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame354Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame355Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame356Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame357Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame358Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame359Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame360Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame361Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame362Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame363Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame364Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame365Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame366Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame367Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame368Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame369Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame370Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame371Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame372Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame373Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame374Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame375Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame376Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame377Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame378Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame379Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame380Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame381Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame382Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame383Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame384Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame385Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame386Pos[8];
extern const struct sprite_piece_pos gSpriteBank00Frame387Pos[6];
extern const struct sprite_piece_pos gSpriteBank00Frame388Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame389Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame390Pos[6];
extern const struct sprite_piece_pos gSpriteBank00Frame391Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame392Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame393Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame394Pos[10];
extern const struct sprite_piece_pos gSpriteBank00Frame395Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame396Pos[9];
extern const struct sprite_piece_pos gSpriteBank00Frame397Pos[7];
extern const struct sprite_piece_pos gSpriteBank00Frame398Pos[9];
extern const struct sprite_piece_pos gSpriteBank00Frame399Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame400Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame401Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame402Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame403Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame404Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame405Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame406Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame407Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame408Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame409Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame410Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame411Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame412Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame413Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame414Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame415Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame416Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame417Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame418Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame419Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame420Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame421Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame422Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame423Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame424Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame425Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame426Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame427Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame428Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame429Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame430Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame431Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame432Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame433Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame434Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame435Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame436Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame437Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame438Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame439Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame441Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame442Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame443Pos[5];
extern const struct sprite_piece_pos gSpriteBank00Frame444Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame445Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame446Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame447Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame448Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame449Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame450Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame451Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame452Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame453Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame454Pos[1];
extern const struct sprite_piece_pos gSpriteBank00Frame455Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame456Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame457Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame458Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame459Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame460Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame461Pos[4];
extern const struct sprite_piece_pos gSpriteBank00Frame462Pos[2];
extern const struct sprite_piece_pos gSpriteBank00Frame463Pos[3];
extern const struct sprite_piece_pos gSpriteBank00Frame464Pos[4];
extern const u8 gSpriteBank00Frame000Pieces[4];
extern const u8 gSpriteBank00Frame001Pieces[4];
extern const u8 gSpriteBank00Frame002Pieces[2];
extern const u8 gSpriteBank00Frame003Pieces[3];
extern const u8 gSpriteBank00Frame004Pieces[3];
extern const u8 gSpriteBank00Frame005Pieces[4];
extern const u8 gSpriteBank00Frame006Pieces[2];
extern const u8 gSpriteBank00Frame007Pieces[2];
extern const u8 gSpriteBank00Frame008Pieces[4];
extern const u8 gSpriteBank00Frame009Pieces[3];
extern const u8 gSpriteBank00Frame010Pieces[3];
extern const u8 gSpriteBank00Frame011Pieces[3];
extern const u8 gSpriteBank00Frame012Pieces[4];
extern const u8 gSpriteBank00Frame013Pieces[1];
extern const u8 gSpriteBank00Frame014Pieces[1];
extern const u8 gSpriteBank00Frame015Pieces[3];
extern const u8 gSpriteBank00Frame016Pieces[3];
extern const u8 gSpriteBank00Frame017Pieces[4];
extern const u8 gSpriteBank00Frame018Pieces[3];
extern const u8 gSpriteBank00Frame019Pieces[3];
extern const u8 gSpriteBank00Frame020Pieces[4];
extern const u8 gSpriteBank00Frame021Pieces[3];
extern const u8 gSpriteBank00Frame022Pieces[3];
extern const u8 gSpriteBank00Frame023Pieces[3];
extern const u8 gSpriteBank00Frame024Pieces[4];
extern const u8 gSpriteBank00Frame025Pieces[4];
extern const u8 gSpriteBank00Frame026Pieces[3];
extern const u8 gSpriteBank00Frame027Pieces[3];
extern const u8 gSpriteBank00Frame028Pieces[4];
extern const u8 gSpriteBank00Frame029Pieces[3];
extern const u8 gSpriteBank00Frame030Pieces[3];
extern const u8 gSpriteBank00Frame031Pieces[3];
extern const u8 gSpriteBank00Frame032Pieces[3];
extern const u8 gSpriteBank00Frame033Pieces[3];
extern const u8 gSpriteBank00Frame034Pieces[3];
extern const u8 gSpriteBank00Frame035Pieces[3];
extern const u8 gSpriteBank00Frame036Pieces[4];
extern const u8 gSpriteBank00Frame037Pieces[4];
extern const u8 gSpriteBank00Frame038Pieces[4];
extern const u8 gSpriteBank00Frame039Pieces[4];
extern const u8 gSpriteBank00Frame040Pieces[4];
extern const u8 gSpriteBank00Frame041Pieces[4];
extern const u8 gSpriteBank00Frame042Pieces[4];
extern const u8 gSpriteBank00Frame043Pieces[4];
extern const u8 gSpriteBank00Frame044Pieces[4];
extern const u8 gSpriteBank00Frame045Pieces[4];
extern const u8 gSpriteBank00Frame046Pieces[4];
extern const u8 gSpriteBank00Frame047Pieces[4];
extern const u8 gSpriteBank00Frame048Pieces[1];
extern const u8 gSpriteBank00Frame049Pieces[4];
extern const u8 gSpriteBank00Frame050Pieces[2];
extern const u8 gSpriteBank00Frame051Pieces[1];
extern const u8 gSpriteBank00Frame052Pieces[4];
extern const u8 gSpriteBank00Frame053Pieces[3];
extern const u8 gSpriteBank00Frame054Pieces[3];
extern const u8 gSpriteBank00Frame055Pieces[2];
extern const u8 gSpriteBank00Frame056Pieces[3];
extern const u8 gSpriteBank00Frame057Pieces[3];
extern const u8 gSpriteBank00Frame058Pieces[3];
extern const u8 gSpriteBank00Frame059Pieces[2];
extern const u8 gSpriteBank00Frame060Pieces[5];
extern const u8 gSpriteBank00Frame061Pieces[2];
extern const u8 gSpriteBank00Frame062Pieces[4];
extern const u8 gSpriteBank00Frame063Pieces[4];
extern const u8 gSpriteBank00Frame064Pieces[1];
extern const u8 gSpriteBank00Frame065Pieces[3];
extern const u8 gSpriteBank00Frame066Pieces[4];
extern const u8 gSpriteBank00Frame067Pieces[2];
extern const u8 gSpriteBank00Frame068Pieces[3];
extern const u8 gSpriteBank00Frame069Pieces[4];
extern const u8 gSpriteBank00Frame070Pieces[3];
extern const u8 gSpriteBank00Frame071Pieces[4];
extern const u8 gSpriteBank00Frame072Pieces[3];
extern const u8 gSpriteBank00Frame073Pieces[2];
extern const u8 gSpriteBank00Frame074Pieces[4];
extern const u8 gSpriteBank00Frame075Pieces[4];
extern const u8 gSpriteBank00Frame076Pieces[3];
extern const u8 gSpriteBank00Frame077Pieces[5];
extern const u8 gSpriteBank00Frame078Pieces[3];
extern const u8 gSpriteBank00Frame079Pieces[3];
extern const u8 gSpriteBank00Frame080Pieces[3];
extern const u8 gSpriteBank00Frame081Pieces[3];
extern const u8 gSpriteBank00Frame082Pieces[3];
extern const u8 gSpriteBank00Frame083Pieces[4];
extern const u8 gSpriteBank00Frame084Pieces[3];
extern const u8 gSpriteBank00Frame085Pieces[2];
extern const u8 gSpriteBank00Frame086Pieces[4];
extern const u8 gSpriteBank00Frame087Pieces[4];
extern const u8 gSpriteBank00Frame088Pieces[3];
extern const u8 gSpriteBank00Frame089Pieces[5];
extern const u8 gSpriteBank00Frame090Pieces[3];
extern const u8 gSpriteBank00Frame091Pieces[3];
extern const u8 gSpriteBank00Frame092Pieces[1];
extern const u8 gSpriteBank00Frame093Pieces[1];
extern const u8 gSpriteBank00Frame094Pieces[3];
extern const u8 gSpriteBank00Frame095Pieces[3];
extern const u8 gSpriteBank00Frame096Pieces[3];
extern const u8 gSpriteBank00Frame097Pieces[3];
extern const u8 gSpriteBank00Frame098Pieces[3];
extern const u8 gSpriteBank00Frame099Pieces[3];
extern const u8 gSpriteBank00Frame100Pieces[3];
extern const u8 gSpriteBank00Frame101Pieces[3];
extern const u8 gSpriteBank00Frame102Pieces[4];
extern const u8 gSpriteBank00Frame103Pieces[3];
extern const u8 gSpriteBank00Frame104Pieces[3];
extern const u8 gSpriteBank00Frame105Pieces[4];
extern const u8 gSpriteBank00Frame106Pieces[4];
extern const u8 gSpriteBank00Frame107Pieces[4];
extern const u8 gSpriteBank00Frame108Pieces[5];
extern const u8 gSpriteBank00Frame109Pieces[3];
extern const u8 gSpriteBank00Frame110Pieces[2];
extern const u8 gSpriteBank00Frame111Pieces[4];
extern const u8 gSpriteBank00Frame112Pieces[3];
extern const u8 gSpriteBank00Frame113Pieces[2];
extern const u8 gSpriteBank00Frame114Pieces[5];
extern const u8 gSpriteBank00Frame115Pieces[4];
extern const u8 gSpriteBank00Frame116Pieces[5];
extern const u8 gSpriteBank00Frame117Pieces[3];
extern const u8 gSpriteBank00Frame118Pieces[4];
extern const u8 gSpriteBank00Frame119Pieces[2];
extern const u8 gSpriteBank00Frame120Pieces[2];
extern const u8 gSpriteBank00Frame121Pieces[4];
extern const u8 gSpriteBank00Frame122Pieces[4];
extern const u8 gSpriteBank00Frame123Pieces[4];
extern const u8 gSpriteBank00Frame124Pieces[4];
extern const u8 gSpriteBank00Frame125Pieces[3];
extern const u8 gSpriteBank00Frame126Pieces[3];
extern const u8 gSpriteBank00Frame127Pieces[3];
extern const u8 gSpriteBank00Frame128Pieces[3];
extern const u8 gSpriteBank00Frame129Pieces[4];
extern const u8 gSpriteBank00Frame130Pieces[3];
extern const u8 gSpriteBank00Frame131Pieces[4];
extern const u8 gSpriteBank00Frame132Pieces[4];
extern const u8 gSpriteBank00Frame133Pieces[4];
extern const u8 gSpriteBank00Frame134Pieces[4];
extern const u8 gSpriteBank00Frame135Pieces[4];
extern const u8 gSpriteBank00Frame136Pieces[4];
extern const u8 gSpriteBank00Frame137Pieces[3];
extern const u8 gSpriteBank00Frame138Pieces[3];
extern const u8 gSpriteBank00Frame139Pieces[3];
extern const u8 gSpriteBank00Frame140Pieces[3];
extern const u8 gSpriteBank00Frame141Pieces[4];
extern const u8 gSpriteBank00Frame142Pieces[4];
extern const u8 gSpriteBank00Frame143Pieces[3];
extern const u8 gSpriteBank00Frame144Pieces[2];
extern const u8 gSpriteBank00Frame145Pieces[3];
extern const u8 gSpriteBank00Frame146Pieces[3];
extern const u8 gSpriteBank00Frame147Pieces[3];
extern const u8 gSpriteBank00Frame148Pieces[3];
extern const u8 gSpriteBank00Frame149Pieces[4];
extern const u8 gSpriteBank00Frame150Pieces[3];
extern const u8 gSpriteBank00Frame151Pieces[1];
extern const u8 gSpriteBank00Frame152Pieces[1];
extern const u8 gSpriteBank00Frame153Pieces[1];
extern const u8 gSpriteBank00Frame154Pieces[3];
extern const u8 gSpriteBank00Frame155Pieces[3];
extern const u8 gSpriteBank00Frame156Pieces[5];
extern const u8 gSpriteBank00Frame157Pieces[3];
extern const u8 gSpriteBank00Frame158Pieces[3];
extern const u8 gSpriteBank00Frame159Pieces[3];
extern const u8 gSpriteBank00Frame160Pieces[2];
extern const u8 gSpriteBank00Frame161Pieces[2];
extern const u8 gSpriteBank00Frame162Pieces[1];
extern const u8 gSpriteBank00Frame163Pieces[2];
extern const u8 gSpriteBank00Frame164Pieces[3];
extern const u8 gSpriteBank00Frame165Pieces[4];
extern const u8 gSpriteBank00Frame166Pieces[5];
extern const u8 gSpriteBank00Frame167Pieces[3];
extern const u8 gSpriteBank00Frame168Pieces[3];
extern const u8 gSpriteBank00Frame169Pieces[4];
extern const u8 gSpriteBank00Frame170Pieces[3];
extern const u8 gSpriteBank00Frame171Pieces[4];
extern const u8 gSpriteBank00Frame172Pieces[4];
extern const u8 gSpriteBank00Frame173Pieces[4];
extern const u8 gSpriteBank00Frame174Pieces[4];
extern const u8 gSpriteBank00Frame175Pieces[4];
extern const u8 gSpriteBank00Frame176Pieces[3];
extern const u8 gSpriteBank00Frame177Pieces[3];
extern const u8 gSpriteBank00Frame178Pieces[3];
extern const u8 gSpriteBank00Frame179Pieces[4];
extern const u8 gSpriteBank00Frame180Pieces[4];
extern const u8 gSpriteBank00Frame181Pieces[5];
extern const u8 gSpriteBank00Frame182Pieces[3];
extern const u8 gSpriteBank00Frame183Pieces[1];
extern const u8 gSpriteBank00Frame184Pieces[1];
extern const u8 gSpriteBank00Frame185Pieces[1];
extern const u8 gSpriteBank00Frame186Pieces[4];
extern const u8 gSpriteBank00Frame187Pieces[4];
extern const u8 gSpriteBank00Frame188Pieces[3];
extern const u8 gSpriteBank00Frame189Pieces[1];
extern const u8 gSpriteBank00Frame190Pieces[3];
extern const u8 gSpriteBank00Frame191Pieces[4];
extern const u8 gSpriteBank00Frame192Pieces[1];
extern const u8 gSpriteBank00Frame193Pieces[1];
extern const u8 gSpriteBank00Frame194Pieces[1];
extern const u8 gSpriteBank00Frame195Pieces[1];
extern const u8 gSpriteBank00Frame196Pieces[3];
extern const u8 gSpriteBank00Frame197Pieces[1];
extern const u8 gSpriteBank00Frame198Pieces[2];
extern const u8 gSpriteBank00Frame199Pieces[2];
extern const u8 gSpriteBank00Frame200Pieces[4];
extern const u8 gSpriteBank00Frame201Pieces[4];
extern const u8 gSpriteBank00Frame202Pieces[3];
extern const u8 gSpriteBank00Frame203Pieces[4];
extern const u8 gSpriteBank00Frame204Pieces[4];
extern const u8 gSpriteBank00Frame205Pieces[3];
extern const u8 gSpriteBank00Frame206Pieces[2];
extern const u8 gSpriteBank00Frame207Pieces[3];
extern const u8 gSpriteBank00Frame208Pieces[3];
extern const u8 gSpriteBank00Frame209Pieces[3];
extern const u8 gSpriteBank00Frame210Pieces[3];
extern const u8 gSpriteBank00Frame211Pieces[4];
extern const u8 gSpriteBank00Frame212Pieces[3];
extern const u8 gSpriteBank00Frame213Pieces[1];
extern const u8 gSpriteBank00Frame214Pieces[3];
extern const u8 gSpriteBank00Frame215Pieces[2];
extern const u8 gSpriteBank00Frame216Pieces[1];
extern const u8 gSpriteBank00Frame217Pieces[3];
extern const u8 gSpriteBank00Frame218Pieces[3];
extern const u8 gSpriteBank00Frame219Pieces[3];
extern const u8 gSpriteBank00Frame220Pieces[3];
extern const u8 gSpriteBank00Frame221Pieces[4];
extern const u8 gSpriteBank00Frame222Pieces[4];
extern const u8 gSpriteBank00Frame223Pieces[4];
extern const u8 gSpriteBank00Frame224Pieces[4];
extern const u8 gSpriteBank00Frame225Pieces[4];
extern const u8 gSpriteBank00Frame226Pieces[5];
extern const u8 gSpriteBank00Frame227Pieces[5];
extern const u8 gSpriteBank00Frame228Pieces[5];
extern const u8 gSpriteBank00Frame229Pieces[5];
extern const u8 gSpriteBank00Frame230Pieces[5];
extern const u8 gSpriteBank00Frame231Pieces[5];
extern const u8 gSpriteBank00Frame232Pieces[5];
extern const u8 gSpriteBank00Frame233Pieces[4];
extern const u8 gSpriteBank00Frame234Pieces[2];
extern const u8 gSpriteBank00Frame235Pieces[2];
extern const u8 gSpriteBank00Frame236Pieces[4];
extern const u8 gSpriteBank00Frame237Pieces[2];
extern const u8 gSpriteBank00Frame238Pieces[3];
extern const u8 gSpriteBank00Frame239Pieces[4];
extern const u8 gSpriteBank00Frame240Pieces[4];
extern const u8 gSpriteBank00Frame241Pieces[5];
extern const u8 gSpriteBank00Frame242Pieces[3];
extern const u8 gSpriteBank00Frame243Pieces[2];
extern const u8 gSpriteBank00Frame244Pieces[4];
extern const u8 gSpriteBank00Frame245Pieces[5];
extern const u8 gSpriteBank00Frame246Pieces[5];
extern const u8 gSpriteBank00Frame247Pieces[4];
extern const u8 gSpriteBank00Frame248Pieces[4];
extern const u8 gSpriteBank00Frame249Pieces[5];
extern const u8 gSpriteBank00Frame250Pieces[5];
extern const u8 gSpriteBank00Frame251Pieces[4];
extern const u8 gSpriteBank00Frame252Pieces[4];
extern const u8 gSpriteBank00Frame253Pieces[5];
extern const u8 gSpriteBank00Frame254Pieces[4];
extern const u8 gSpriteBank00Frame255Pieces[3];
extern const u8 gSpriteBank00Frame256Pieces[3];
extern const u8 gSpriteBank00Frame257Pieces[3];
extern const u8 gSpriteBank00Frame258Pieces[3];
extern const u8 gSpriteBank00Frame259Pieces[4];
extern const u8 gSpriteBank00Frame260Pieces[4];
extern const u8 gSpriteBank00Frame261Pieces[3];
extern const u8 gSpriteBank00Frame262Pieces[3];
extern const u8 gSpriteBank00Frame263Pieces[3];
extern const u8 gSpriteBank00Frame264Pieces[3];
extern const u8 gSpriteBank00Frame265Pieces[3];
extern const u8 gSpriteBank00Frame266Pieces[4];
extern const u8 gSpriteBank00Frame267Pieces[4];
extern const u8 gSpriteBank00Frame268Pieces[3];
extern const u8 gSpriteBank00Frame269Pieces[5];
extern const u8 gSpriteBank00Frame270Pieces[5];
extern const u8 gSpriteBank00Frame271Pieces[4];
extern const u8 gSpriteBank00Frame272Pieces[3];
extern const u8 gSpriteBank00Frame273Pieces[1];
extern const u8 gSpriteBank00Frame274Pieces[5];
extern const u8 gSpriteBank00Frame275Pieces[4];
extern const u8 gSpriteBank00Frame276Pieces[4];
extern const u8 gSpriteBank00Frame277Pieces[4];
extern const u8 gSpriteBank00Frame278Pieces[2];
extern const u8 gSpriteBank00Frame279Pieces[3];
extern const u8 gSpriteBank00Frame280Pieces[3];
extern const u8 gSpriteBank00Frame281Pieces[4];
extern const u8 gSpriteBank00Frame282Pieces[3];
extern const u8 gSpriteBank00Frame283Pieces[3];
extern const u8 gSpriteBank00Frame284Pieces[1];
extern const u8 gSpriteBank00Frame285Pieces[2];
extern const u8 gSpriteBank00Frame286Pieces[2];
extern const u8 gSpriteBank00Frame287Pieces[1];
extern const u8 gSpriteBank00Frame288Pieces[2];
extern const u8 gSpriteBank00Frame289Pieces[3];
extern const u8 gSpriteBank00Frame290Pieces[3];
extern const u8 gSpriteBank00Frame291Pieces[3];
extern const u8 gSpriteBank00Frame292Pieces[4];
extern const u8 gSpriteBank00Frame293Pieces[4];
extern const u8 gSpriteBank00Frame294Pieces[4];
extern const u8 gSpriteBank00Frame295Pieces[4];
extern const u8 gSpriteBank00Frame296Pieces[4];
extern const u8 gSpriteBank00Frame297Pieces[3];
extern const u8 gSpriteBank00Frame298Pieces[3];
extern const u8 gSpriteBank00Frame299Pieces[3];
extern const u8 gSpriteBank00Frame300Pieces[2];
extern const u8 gSpriteBank00Frame301Pieces[2];
extern const u8 gSpriteBank00Frame302Pieces[2];
extern const u8 gSpriteBank00Frame303Pieces[2];
extern const u8 gSpriteBank00Frame304Pieces[3];
extern const u8 gSpriteBank00Frame305Pieces[3];
extern const u8 gSpriteBank00Frame306Pieces[3];
extern const u8 gSpriteBank00Frame307Pieces[3];
extern const u8 gSpriteBank00Frame308Pieces[3];
extern const u8 gSpriteBank00Frame309Pieces[2];
extern const u8 gSpriteBank00Frame310Pieces[4];
extern const u8 gSpriteBank00Frame311Pieces[4];
extern const u8 gSpriteBank00Frame312Pieces[3];
extern const u8 gSpriteBank00Frame313Pieces[3];
extern const u8 gSpriteBank00Frame314Pieces[3];
extern const u8 gSpriteBank00Frame315Pieces[2];
extern const u8 gSpriteBank00Frame316Pieces[3];
extern const u8 gSpriteBank00Frame317Pieces[1];
extern const u8 gSpriteBank00Frame318Pieces[3];
extern const u8 gSpriteBank00Frame319Pieces[3];
extern const u8 gSpriteBank00Frame320Pieces[3];
extern const u8 gSpriteBank00Frame321Pieces[3];
extern const u8 gSpriteBank00Frame322Pieces[4];
extern const u8 gSpriteBank00Frame323Pieces[4];
extern const u8 gSpriteBank00Frame324Pieces[3];
extern const u8 gSpriteBank00Frame325Pieces[4];
extern const u8 gSpriteBank00Frame326Pieces[4];
extern const u8 gSpriteBank00Frame327Pieces[3];
extern const u8 gSpriteBank00Frame328Pieces[4];
extern const u8 gSpriteBank00Frame329Pieces[2];
extern const u8 gSpriteBank00Frame330Pieces[1];
extern const u8 gSpriteBank00Frame331Pieces[2];
extern const u8 gSpriteBank00Frame332Pieces[5];
extern const u8 gSpriteBank00Frame333Pieces[9];
extern const u8 gSpriteBank00Frame334Pieces[7];
extern const u8 gSpriteBank00Frame335Pieces[9];
extern const u8 gSpriteBank00Frame336Pieces[4];
extern const u8 gSpriteBank00Frame337Pieces[10];
extern const u8 gSpriteBank00Frame338Pieces[5];
extern const u8 gSpriteBank00Frame339Pieces[5];
extern const u8 gSpriteBank00Frame340Pieces[3];
extern const u8 gSpriteBank00Frame341Pieces[6];
extern const u8 gSpriteBank00Frame342Pieces[4];
extern const u8 gSpriteBank00Frame343Pieces[4];
extern const u8 gSpriteBank00Frame344Pieces[6];
extern const u8 gSpriteBank00Frame345Pieces[8];
extern const u8 gSpriteBank00Frame346Pieces[3];
extern const u8 gSpriteBank00Frame347Pieces[3];
extern const u8 gSpriteBank00Frame348Pieces[3];
extern const u8 gSpriteBank00Frame349Pieces[3];
extern const u8 gSpriteBank00Frame350Pieces[3];
extern const u8 gSpriteBank00Frame351Pieces[3];
extern const u8 gSpriteBank00Frame352Pieces[3];
extern const u8 gSpriteBank00Frame353Pieces[3];
extern const u8 gSpriteBank00Frame354Pieces[3];
extern const u8 gSpriteBank00Frame355Pieces[3];
extern const u8 gSpriteBank00Frame356Pieces[5];
extern const u8 gSpriteBank00Frame357Pieces[4];
extern const u8 gSpriteBank00Frame358Pieces[4];
extern const u8 gSpriteBank00Frame359Pieces[3];
extern const u8 gSpriteBank00Frame360Pieces[2];
extern const u8 gSpriteBank00Frame361Pieces[3];
extern const u8 gSpriteBank00Frame362Pieces[3];
extern const u8 gSpriteBank00Frame363Pieces[4];
extern const u8 gSpriteBank00Frame364Pieces[4];
extern const u8 gSpriteBank00Frame365Pieces[3];
extern const u8 gSpriteBank00Frame366Pieces[4];
extern const u8 gSpriteBank00Frame367Pieces[4];
extern const u8 gSpriteBank00Frame368Pieces[3];
extern const u8 gSpriteBank00Frame369Pieces[2];
extern const u8 gSpriteBank00Frame370Pieces[3];
extern const u8 gSpriteBank00Frame371Pieces[3];
extern const u8 gSpriteBank00Frame372Pieces[3];
extern const u8 gSpriteBank00Frame373Pieces[3];
extern const u8 gSpriteBank00Frame374Pieces[4];
extern const u8 gSpriteBank00Frame375Pieces[3];
extern const u8 gSpriteBank00Frame376Pieces[4];
extern const u8 gSpriteBank00Frame377Pieces[4];
extern const u8 gSpriteBank00Frame378Pieces[3];
extern const u8 gSpriteBank00Frame379Pieces[2];
extern const u8 gSpriteBank00Frame380Pieces[3];
extern const u8 gSpriteBank00Frame381Pieces[3];
extern const u8 gSpriteBank00Frame382Pieces[3];
extern const u8 gSpriteBank00Frame383Pieces[3];
extern const u8 gSpriteBank00Frame384Pieces[4];
extern const u8 gSpriteBank00Frame385Pieces[3];
extern const u8 gSpriteBank00Frame386Pieces[8];
extern const u8 gSpriteBank00Frame387Pieces[6];
extern const u8 gSpriteBank00Frame388Pieces[4];
extern const u8 gSpriteBank00Frame389Pieces[4];
extern const u8 gSpriteBank00Frame390Pieces[6];
extern const u8 gSpriteBank00Frame391Pieces[3];
extern const u8 gSpriteBank00Frame392Pieces[5];
extern const u8 gSpriteBank00Frame393Pieces[5];
extern const u8 gSpriteBank00Frame394Pieces[10];
extern const u8 gSpriteBank00Frame395Pieces[4];
extern const u8 gSpriteBank00Frame396Pieces[9];
extern const u8 gSpriteBank00Frame397Pieces[7];
extern const u8 gSpriteBank00Frame398Pieces[9];
extern const u8 gSpriteBank00Frame399Pieces[5];
extern const u8 gSpriteBank00Frame400Pieces[2];
extern const u8 gSpriteBank00Frame401Pieces[1];
extern const u8 gSpriteBank00Frame402Pieces[2];
extern const u8 gSpriteBank00Frame403Pieces[4];
extern const u8 gSpriteBank00Frame404Pieces[3];
extern const u8 gSpriteBank00Frame405Pieces[3];
extern const u8 gSpriteBank00Frame406Pieces[2];
extern const u8 gSpriteBank00Frame407Pieces[3];
extern const u8 gSpriteBank00Frame408Pieces[3];
extern const u8 gSpriteBank00Frame409Pieces[3];
extern const u8 gSpriteBank00Frame410Pieces[3];
extern const u8 gSpriteBank00Frame411Pieces[4];
extern const u8 gSpriteBank00Frame412Pieces[4];
extern const u8 gSpriteBank00Frame413Pieces[4];
extern const u8 gSpriteBank00Frame414Pieces[3];
extern const u8 gSpriteBank00Frame415Pieces[3];
extern const u8 gSpriteBank00Frame416Pieces[3];
extern const u8 gSpriteBank00Frame417Pieces[3];
extern const u8 gSpriteBank00Frame418Pieces[3];
extern const u8 gSpriteBank00Frame419Pieces[3];
extern const u8 gSpriteBank00Frame420Pieces[3];
extern const u8 gSpriteBank00Frame421Pieces[2];
extern const u8 gSpriteBank00Frame422Pieces[2];
extern const u8 gSpriteBank00Frame423Pieces[2];
extern const u8 gSpriteBank00Frame424Pieces[2];
extern const u8 gSpriteBank00Frame425Pieces[2];
extern const u8 gSpriteBank00Frame426Pieces[3];
extern const u8 gSpriteBank00Frame427Pieces[3];
extern const u8 gSpriteBank00Frame428Pieces[3];
extern const u8 gSpriteBank00Frame429Pieces[3];
extern const u8 gSpriteBank00Frame430Pieces[3];
extern const u8 gSpriteBank00Frame431Pieces[3];
extern const u8 gSpriteBank00Frame432Pieces[3];
extern const u8 gSpriteBank00Frame433Pieces[3];
extern const u8 gSpriteBank00Frame434Pieces[4];
extern const u8 gSpriteBank00Frame435Pieces[5];
extern const u8 gSpriteBank00Frame436Pieces[2];
extern const u8 gSpriteBank00Frame437Pieces[4];
extern const u8 gSpriteBank00Frame438Pieces[1];
extern const u8 gSpriteBank00Frame439Pieces[1];
extern const u8 gSpriteBank00Frame441Pieces[2];
extern const u8 gSpriteBank00Frame442Pieces[5];
extern const u8 gSpriteBank00Frame443Pieces[5];
extern const u8 gSpriteBank00Frame444Pieces[3];
extern const u8 gSpriteBank00Frame445Pieces[3];
extern const u8 gSpriteBank00Frame446Pieces[2];
extern const u8 gSpriteBank00Frame447Pieces[3];
extern const u8 gSpriteBank00Frame448Pieces[1];
extern const u8 gSpriteBank00Frame449Pieces[3];
extern const u8 gSpriteBank00Frame450Pieces[2];
extern const u8 gSpriteBank00Frame451Pieces[1];
extern const u8 gSpriteBank00Frame452Pieces[1];
extern const u8 gSpriteBank00Frame453Pieces[1];
extern const u8 gSpriteBank00Frame454Pieces[1];
extern const u8 gSpriteBank00Frame455Pieces[3];
extern const u8 gSpriteBank00Frame456Pieces[3];
extern const u8 gSpriteBank00Frame457Pieces[4];
extern const u8 gSpriteBank00Frame458Pieces[2];
extern const u8 gSpriteBank00Frame459Pieces[3];
extern const u8 gSpriteBank00Frame460Pieces[3];
extern const u8 gSpriteBank00Frame461Pieces[4];
extern const u8 gSpriteBank00Frame462Pieces[2];
extern const u8 gSpriteBank00Frame463Pieces[3];
extern const u8 gSpriteBank00Frame464Pieces[4];

const struct sprite_anim gSpriteBank00Anims[48] = {
    [0] = {
        .seq = gSpriteBank00Anim00Seq,
        .box = { { -7, -6, 14, 20 }, { -21, -8, 41, 25 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim00Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [1] = {
        .seq = gSpriteBank00Anim01Seq,
        .box = { { -7, -6, 14, 20 }, { -16, -14, 34, 29 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim01Seq),
        .flags = 0,
    },
    [2] = {
        .seq = gSpriteBank00Anim02Seq,
        .box = { { -7, -27, 14, 41 }, { -10, -25, 26, 38 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim02Seq),
        .flags = 0,
    },
    [3] = {
        .seq = gSpriteBank00Anim03Seq,
        .box = { { -7, -6, 14, 20 }, { -10, -25, 26, 38 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim03Seq),
        .flags = 0,
    },
    [4] = {
        .seq = gSpriteBank00Anim04Seq,
        .box = { { -7, -6, 14, 20 }, { -6, -14, 22, 26 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim04Seq),
        .flags = 0,
    },
    [5] = {
        .seq = gSpriteBank00Anim05Seq,
        .box = { { -7, -27, 14, 41 }, { -10, -30, 20, 43 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim05Seq),
        .flags = 0,
    },
    [6] = {
        .seq = gSpriteBank00Anim06Seq,
        .box = { { -7, -27, 14, 41 }, { -18, -32, 40, 50 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim06Seq),
        .flags = 0,
    },
    [7] = {
        .seq = gSpriteBank00Anim07Seq,
        .box = { { -7, -27, 14, 41 }, { -32, -23, 55, 26 } },
        .tileRecord = 0,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim07Seq),
        .flags = 0,
    },
    [8] = {
        .seq = gSpriteBank00Anim08Seq,
        .box = { { -7, -27, 14, 41 }, { -17, -24, 40, 38 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim08Seq),
        .flags = 0,
    },
    [9] = {
        .seq = gSpriteBank00Anim09Seq,
        .box = { { -7, -27, 14, 41 }, { -25, -23, 48, 32 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim09Seq),
        .flags = 0,
    },
    [10] = {
        .seq = gSpriteBank00Anim10Seq,
        .box = { { -7, -27, 14, 41 }, { -10, -29, 25, 49 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim10Seq),
        .flags = 0,
    },
    [11] = {
        .seq = gSpriteBank00Anim11Seq,
        .box = { { -7, -27, 14, 41 }, { -11, -29, 30, 51 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim11Seq),
        .flags = 0,
    },
    [12] = {
        .seq = gSpriteBank00Anim12Seq,
        .box = { { -7, -27, 14, 41 }, { -15, -34, 24, 51 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim12Seq),
        .flags = 0,
    },
    [13] = {
        .seq = gSpriteBank00Anim13Seq,
        .box = { { -7, -27, 14, 41 }, { -22, -36, 41, 48 } },
        .tileRecord = 0,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim13Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [14] = {
        .seq = gSpriteBank00Anim14Seq,
        .box = { { -7, -27, 14, 41 }, { -14, -28, 24, 41 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim14Seq),
        .flags = 0,
    },
    [15] = {
        .seq = gSpriteBank00Anim15Seq,
        .box = { { -7, -6, 14, 20 }, { -28, -27, 45, 42 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim15Seq),
        .flags = 0,
    },
    [16] = {
        .seq = gSpriteBank00Anim16Seq,
        .box = { { -7, -27, 14, 41 }, { -25, -24, 45, 38 } },
        .tileRecord = 0,
        .duration = 2,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim16Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [17] = {
        .seq = gSpriteBank00Anim17Seq,
        .box = { { -7, -27, 14, 41 }, { -33, -25, 63, 48 } },
        .tileRecord = 0,
        .duration = 2,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim17Seq),
        .flags = 0,
    },
    [18] = {
        .seq = gSpriteBank00Anim18Seq,
        .box = { { -7, -27, 14, 41 }, { -11, -25, 22, 38 } },
        .tileRecord = 1,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim18Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [19] = {
        .seq = gSpriteBank00Anim19Seq,
        .box = { { -7, -27, 14, 41 }, { -15, -35, 27, 54 } },
        .tileRecord = 0,
        .duration = 2,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim19Seq),
        .flags = 0,
    },
    [20] = {
        .seq = gSpriteBank00Anim20Seq,
        .box = { { -7, -6, 14, 20 }, { -16, -14, 34, 29 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim20Seq),
        .flags = 0,
    },
    [21] = {
        .seq = gSpriteBank00Anim21Seq,
        .box = { { -7, -27, 14, 41 }, { -15, -33, 27, 52 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim21Seq),
        .flags = 0,
    },
    [22] = {
        .seq = gSpriteBank00Anim22Seq,
        .box = { { -7, -27, 14, 41 }, { -17, -28, 31, 44 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim22Seq),
        .flags = 0,
    },
    [23] = {
        .seq = gSpriteBank00Anim23Seq,
        .box = { { -7, -27, 14, 41 }, { -25, -24, 45, 38 } },
        .tileRecord = 2,
        .duration = 2,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim23Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [24] = {
        .seq = gSpriteBank00Anim24Seq,
        .box = { { -7, -27, 14, 41 }, { -20, -26, 38, 39 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim24Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [25] = {
        .seq = gSpriteBank00Anim25Seq,
        .box = { { -7, -27, 14, 41 }, { -24, -33, 47, 47 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim25Seq),
        .flags = 0,
    },
    [26] = {
        .seq = gSpriteBank00Anim26Seq,
        .box = { { -7, -27, 14, 41 }, { -24, -25, 50, 39 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim26Seq),
        .flags = 0,
    },
    [27] = {
        .seq = gSpriteBank00Anim27Seq,
        .box = { { -7, -27, 14, 41 }, { -20, -34, 39, 53 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim27Seq),
        .flags = 0,
    },
    [28] = {
        .seq = gSpriteBank00Anim28Seq,
        .box = { { -25, -26, 36, 41 }, { -25, -28, 43, 44 } },
        .tileRecord = 3,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim28Seq),
        .flags = 0,
    },
    [29] = {
        .seq = gSpriteBank00Anim29Seq,
        .box = { { -7, -27, 14, 41 }, { -12, -28, 21, 43 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim29Seq),
        .flags = 0,
    },
    [30] = {
        .seq = gSpriteBank00Anim30Seq,
        .box = { { -7, -27, 14, 41 }, { -33, -28, 63, 34 } },
        .tileRecord = 0,
        .duration = 2,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim30Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [31] = {
        .seq = gSpriteBank00Anim31Seq,
        .box = { { -7, -27, 14, 41 }, { -17, -28, 28, 38 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim31Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [32] = {
        .seq = gSpriteBank00Anim32Seq,
        .box = { { -7, -27, 14, 41 }, { -12, -28, 25, 39 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim32Seq),
        .flags = 0,
    },
    [33] = {
        .seq = gSpriteBank00Anim33Seq,
        .box = { { -7, -27, 14, 41 }, { -27, -28, 42, 48 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim33Seq),
        .flags = 0,
    },
    [34] = {
        .seq = gSpriteBank00Anim34Seq,
        .box = { { -7, -27, 14, 41 }, { -25, -28, 35, 38 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim34Seq),
        .flags = 0,
    },
    [35] = {
        .seq = gSpriteBank00Anim35Seq,
        .box = { { -7, -27, 14, 41 }, { -13, -28, 24, 42 } },
        .tileRecord = 0,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim35Seq),
        .flags = 0,
    },
    [36] = {
        .seq = gSpriteBank00Anim36Seq,
        .box = { { -11, -26, 21, 39 }, { -42, -72, 85, 94 } },
        .tileRecord = 4,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim36Seq),
        .flags = 0,
    },
    [37] = {
        .seq = gSpriteBank00Anim37Seq,
        .box = { { -7, -27, 14, 41 }, { -16, -24, 28, 36 } },
        .tileRecord = 2,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim37Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [38] = {
        .seq = gSpriteBank00Anim38Seq,
        .box = { { -7, -27, 14, 41 }, { -16, -24, 32, 38 } },
        .tileRecord = 2,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim38Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [39] = {
        .seq = gSpriteBank00Anim39Seq,
        .box = { { -7, -27, 14, 41 }, { -25, -24, 45, 38 } },
        .tileRecord = 2,
        .duration = 2,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim39Seq),
        .flags = 0,
    },
    [40] = {
        .seq = gSpriteBank00Anim40Seq,
        .box = { { -7, -27, 14, 41 }, { -25, -24, 45, 38 } },
        .tileRecord = 2,
        .duration = 2,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim40Seq),
        .flags = 0,
    },
    [41] = {
        .seq = gSpriteBank00Anim41Seq,
        .box = { { -11, -26, 21, 39 }, { -42, -72, 85, 94 } },
        .tileRecord = 4,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim41Seq),
        .flags = 0,
    },
    [42] = {
        .seq = gSpriteBank00Anim42Seq,
        .box = { { -10, -10, 20, 20 }, { -13, -10, 24, 20 } },
        .tileRecord = 109,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim42Seq),
        .flags = 0,
    },
    [43] = {
        .seq = gSpriteBank00Anim43Seq,
        .box = { { -12, -18, 25, 37 }, { -12, -18, 44, 51 } },
        .tileRecord = 114,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim43Seq),
        .flags = 0,
    },
    [44] = {
        .seq = gSpriteBank00Anim44Seq,
        .box = { { -14, -23, 28, 46 }, { -14, -23, 28, 46 } },
        .tileRecord = 115,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim44Seq),
        .flags = 0,
    },
    [45] = {
        .seq = gSpriteBank00Anim45Seq,
        .box = { { -12, -18, 25, 36 }, { -22, -29, 44, 48 } },
        .tileRecord = 109,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim45Seq),
        .flags = 0,
    },
    [46] = {
        .seq = gSpriteBank00Anim46Seq,
        .box = { { -11, -19, 23, 39 }, { -15, -19, 30, 40 } },
        .tileRecord = 110,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim46Seq),
        .flags = 0,
    },
    [47] = {
        .seq = gSpriteBank00Anim47Seq,
        .box = { { -10, -21, 20, 43 }, { -23, -27, 48, 49 } },
        .tileRecord = 0,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank00Anim47Seq),
        .flags = 0,
    },
};

const u16 gSpriteBank00Anim00Seq[12] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
};
const u16 gSpriteBank00Anim01Seq[3] = {
    12, 13, 14,
};
const u16 gSpriteBank00Anim02Seq[4] = {
    15, 16, 17, 18,
};
const u16 gSpriteBank00Anim03Seq[4] = {
    19, 20, 21, 22,
};
const u16 gSpriteBank00Anim04Seq[1] = {
    23,
};
const u16 gSpriteBank00Anim05Seq[24] = {
    24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47,
};
const u16 gSpriteBank00Anim06Seq[9] = {
    48, 49, 50, 51, 52, 53, 54, 55, 56,
};
const u16 gSpriteBank00Anim07Seq[7] = {
    57, 58, 59, 60, 61, 62, 63,
};
const u16 gSpriteBank00Anim08Seq[3] = {
    64, 65, 66,
};
const u16 gSpriteBank00Anim09Seq[3] = {
    67, 68, 69,
};
const u16 gSpriteBank00Anim10Seq[10] = {
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
};
const u16 gSpriteBank00Anim11Seq[12] = {
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
};
const u16 gSpriteBank00Anim12Seq[10] = {
    92, 93, 94, 95, 96, 97, 98, 99, 100, 101,
};
const u16 gSpriteBank00Anim13Seq[20] = {
    102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117,
    118, 119, 120, 121,
};
const u16 gSpriteBank00Anim14Seq[24] = {
    122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 132, 131,
    130, 129, 128, 127, 126, 125, 124, 123,
};
const u16 gSpriteBank00Anim15Seq[5] = {
    136, 137, 138, 139, 140,
};
const u16 gSpriteBank00Anim16Seq[10] = {
    141, 142, 143, 144, 145, 146, 147, 148, 149, 150,
};
const u16 gSpriteBank00Anim17Seq[29] = {
    151, 152, 153, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 155, 156, 157,
    158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170,
};
const u16 gSpriteBank00Anim18Seq[10] = {
    171, 172, 173, 174, 175, 176, 177, 178, 179, 180,
};
const u16 gSpriteBank00Anim19Seq[3] = {
    181, 182, 183,
};
const u16 gSpriteBank00Anim20Seq[3] = {
    184, 185, 186,
};
const u16 gSpriteBank00Anim21Seq[3] = {
    187, 188, 189,
};
const u16 gSpriteBank00Anim22Seq[13] = {
    190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202,
};
const u16 gSpriteBank00Anim23Seq[10] = {
    203, 204, 205, 206, 207, 208, 209, 210, 211, 212,
};
const u16 gSpriteBank00Anim24Seq[5] = {
    213, 214, 215, 216, 217,
};
const u16 gSpriteBank00Anim25Seq[4] = {
    218, 219, 220, 221,
};
const u16 gSpriteBank00Anim26Seq[107] = {
    222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237,
    238, 239, 240, 241, 242, 243, 244, 245, 246, 246, 247, 247, 246, 246, 248, 248,
    249, 250, 245, 244, 243, 242, 241, 240, 251, 252, 253, 254, 255, 256, 257, 258,
    259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 256, 257, 258, 259, 260, 261,
    262, 263, 264, 265, 266, 267, 268, 256, 257, 258, 259, 260, 261, 262, 263, 264,
    265, 266, 267, 268, 255, 254, 253, 252, 251, 239, 238, 237, 236, 235, 234, 233,
    232, 269, 270, 229, 228, 227, 226, 225, 224, 223, 222,
};
const u16 gSpriteBank00Anim27Seq[3] = {
    271, 272, 273,
};
const u16 gSpriteBank00Anim28Seq[8] = {
    274, 275, 276, 277, 278, 279, 280, 281,
};
const u16 gSpriteBank00Anim29Seq[2] = {
    282, 283,
};
const u16 gSpriteBank00Anim30Seq[10] = {
    284, 285, 286, 287, 288, 284, 285, 286, 287, 288,
};
const u16 gSpriteBank00Anim31Seq[15] = {
    289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303,
};
const u16 gSpriteBank00Anim32Seq[3] = {
    304, 305, 306,
};
const u16 gSpriteBank00Anim33Seq[14] = {
    307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 320,
};
const u16 gSpriteBank00Anim34Seq[3] = {
    321, 322, 323,
};
const u16 gSpriteBank00Anim35Seq[3] = {
    324, 325, 326,
};
const u16 gSpriteBank00Anim36Seq[19] = {
    327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342,
    343, 344, 345,
};
const u16 gSpriteBank00Anim37Seq[8] = {
    346, 347, 348, 349, 350, 351, 352, 353,
};
const u16 gSpriteBank00Anim38Seq[12] = {
    354, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 365,
};
const u16 gSpriteBank00Anim39Seq[10] = {
    366, 367, 368, 369, 370, 371, 372, 373, 374, 375,
};
const u16 gSpriteBank00Anim40Seq[10] = {
    376, 377, 378, 379, 380, 381, 382, 383, 384, 385,
};
const u16 gSpriteBank00Anim41Seq[19] = {
    386, 387, 388, 389, 390, 391, 392, 393, 394, 395, 396, 397, 398, 399, 400, 401,
    402, 403, 327,
};
const u16 gSpriteBank00Anim42Seq[35] = {
    404, 405, 406, 407, 408, 409, 410, 411, 411, 411, 411, 411, 411, 412, 413, 411,
    411, 411, 411, 411, 411, 411, 411, 411, 411, 413, 411, 411, 413, 412, 411, 411,
    411, 411, 411,
};
const u16 gSpriteBank00Anim43Seq[27] = {
    414, 415, 416, 417, 418, 418, 418, 418, 418, 418, 419, 420, 421, 422, 423, 424,
    425, 426, 427, 428, 428, 428, 428, 428, 428, 428, 428,
};
const u16 gSpriteBank00Anim44Seq[15] = {
    429, 430, 431, 429, 430, 431, 429, 430, 431, 429, 430, 431, 429, 430, 431,
};
const u16 gSpriteBank00Anim45Seq[16] = {
    432, 433, 434, 434, 434, 434, 434, 434, 434, 434, 435, 436, 437, 438, 439, 440,
};
const u16 gSpriteBank00Anim46Seq[24] = {
    441, 441, 441, 441, 442, 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453,
    454, 454, 454, 454, 454, 454, 454, 454,
};
const u16 gSpriteBank00Anim47Seq[10] = {
    455, 456, 457, 458, 459, 460, 461, 462, 463, 464,
};

const struct sprite_frame *const gSpriteBank00Frames[465] = {
    &gSpriteBank00Frame000.frame,
    &gSpriteBank00Frame001.frame,
    &gSpriteBank00Frame002.frame,
    &gSpriteBank00Frame003.frame,
    &gSpriteBank00Frame004.frame,
    &gSpriteBank00Frame005.frame,
    &gSpriteBank00Frame006.frame,
    &gSpriteBank00Frame007.frame,
    &gSpriteBank00Frame008.frame,
    &gSpriteBank00Frame009.frame,
    &gSpriteBank00Frame010.frame,
    &gSpriteBank00Frame011.frame,
    &gSpriteBank00Frame012.frame,
    &gSpriteBank00Frame013.frame,
    &gSpriteBank00Frame014.frame,
    &gSpriteBank00Frame015.frame,
    &gSpriteBank00Frame016.frame,
    &gSpriteBank00Frame017.frame,
    &gSpriteBank00Frame018.frame,
    &gSpriteBank00Frame019.frame,
    &gSpriteBank00Frame020.frame,
    &gSpriteBank00Frame021.frame,
    &gSpriteBank00Frame022.frame,
    &gSpriteBank00Frame023.frame,
    &gSpriteBank00Frame024.frame,
    &gSpriteBank00Frame025.frame,
    &gSpriteBank00Frame026.frame,
    &gSpriteBank00Frame027.frame,
    &gSpriteBank00Frame028.frame,
    &gSpriteBank00Frame029.frame,
    &gSpriteBank00Frame030.frame,
    &gSpriteBank00Frame031.frame,
    &gSpriteBank00Frame032.frame,
    &gSpriteBank00Frame033.frame,
    &gSpriteBank00Frame034.frame,
    &gSpriteBank00Frame035.frame,
    &gSpriteBank00Frame036.frame,
    &gSpriteBank00Frame037.frame,
    &gSpriteBank00Frame038.frame,
    &gSpriteBank00Frame039.frame,
    &gSpriteBank00Frame040.frame,
    &gSpriteBank00Frame041.frame,
    &gSpriteBank00Frame042.frame,
    &gSpriteBank00Frame043.frame,
    &gSpriteBank00Frame044.frame,
    &gSpriteBank00Frame045.frame,
    &gSpriteBank00Frame046.frame,
    &gSpriteBank00Frame047.frame,
    &gSpriteBank00Frame048.frame,
    &gSpriteBank00Frame049.frame,
    &gSpriteBank00Frame050.frame,
    &gSpriteBank00Frame051.frame,
    &gSpriteBank00Frame052.frame,
    &gSpriteBank00Frame053.frame,
    &gSpriteBank00Frame054.frame,
    &gSpriteBank00Frame055.frame,
    &gSpriteBank00Frame056.frame,
    &gSpriteBank00Frame057.frame,
    &gSpriteBank00Frame058.frame,
    &gSpriteBank00Frame059.frame,
    &gSpriteBank00Frame060.frame,
    &gSpriteBank00Frame061.frame,
    &gSpriteBank00Frame062.frame,
    &gSpriteBank00Frame063.frame,
    &gSpriteBank00Frame064.frame,
    &gSpriteBank00Frame065.frame,
    &gSpriteBank00Frame066.frame,
    &gSpriteBank00Frame067.frame,
    &gSpriteBank00Frame068.frame,
    &gSpriteBank00Frame069.frame,
    &gSpriteBank00Frame070.frame,
    &gSpriteBank00Frame071.frame,
    &gSpriteBank00Frame072.frame,
    &gSpriteBank00Frame073.frame,
    &gSpriteBank00Frame074.frame,
    &gSpriteBank00Frame075.frame,
    &gSpriteBank00Frame076.frame,
    &gSpriteBank00Frame077.frame,
    &gSpriteBank00Frame078.frame,
    &gSpriteBank00Frame079.frame,
    &gSpriteBank00Frame080.frame,
    &gSpriteBank00Frame081.frame,
    &gSpriteBank00Frame082.frame,
    &gSpriteBank00Frame083.frame,
    &gSpriteBank00Frame084.frame,
    &gSpriteBank00Frame085.frame,
    &gSpriteBank00Frame086.frame,
    &gSpriteBank00Frame087.frame,
    &gSpriteBank00Frame088.frame,
    &gSpriteBank00Frame089.frame,
    &gSpriteBank00Frame090.frame,
    &gSpriteBank00Frame091.frame,
    &gSpriteBank00Frame092.frame,
    &gSpriteBank00Frame093.frame,
    &gSpriteBank00Frame094.frame,
    &gSpriteBank00Frame095.frame,
    &gSpriteBank00Frame096.frame,
    &gSpriteBank00Frame097.frame,
    &gSpriteBank00Frame098.frame,
    &gSpriteBank00Frame099.frame,
    &gSpriteBank00Frame100.frame,
    &gSpriteBank00Frame101.frame,
    &gSpriteBank00Frame102.frame,
    &gSpriteBank00Frame103.frame,
    &gSpriteBank00Frame104.frame,
    &gSpriteBank00Frame105.frame,
    &gSpriteBank00Frame106.frame,
    &gSpriteBank00Frame107.frame,
    &gSpriteBank00Frame108.frame,
    &gSpriteBank00Frame109.frame,
    &gSpriteBank00Frame110.frame,
    &gSpriteBank00Frame111.frame,
    &gSpriteBank00Frame112.frame,
    &gSpriteBank00Frame113.frame,
    &gSpriteBank00Frame114.frame,
    &gSpriteBank00Frame115.frame,
    &gSpriteBank00Frame116.frame,
    &gSpriteBank00Frame117.frame,
    &gSpriteBank00Frame118.frame,
    &gSpriteBank00Frame119.frame,
    &gSpriteBank00Frame120.frame,
    &gSpriteBank00Frame121.frame,
    &gSpriteBank00Frame122.frame,
    &gSpriteBank00Frame123.frame,
    &gSpriteBank00Frame124.frame,
    &gSpriteBank00Frame125.frame,
    &gSpriteBank00Frame126.frame,
    &gSpriteBank00Frame127.frame,
    &gSpriteBank00Frame128.frame,
    &gSpriteBank00Frame129.frame,
    &gSpriteBank00Frame130.frame,
    &gSpriteBank00Frame131.frame,
    &gSpriteBank00Frame132.frame,
    &gSpriteBank00Frame133.frame,
    &gSpriteBank00Frame134.frame,
    &gSpriteBank00Frame135.frame,
    &gSpriteBank00Frame136.frame,
    &gSpriteBank00Frame137.frame,
    &gSpriteBank00Frame138.frame,
    &gSpriteBank00Frame139.frame,
    &gSpriteBank00Frame140.frame,
    &gSpriteBank00Frame141.frame,
    &gSpriteBank00Frame142.frame,
    &gSpriteBank00Frame143.frame,
    &gSpriteBank00Frame144.frame,
    &gSpriteBank00Frame145.frame,
    &gSpriteBank00Frame146.frame,
    &gSpriteBank00Frame147.frame,
    &gSpriteBank00Frame148.frame,
    &gSpriteBank00Frame149.frame,
    &gSpriteBank00Frame150.frame,
    &gSpriteBank00Frame151.frame,
    &gSpriteBank00Frame152.frame,
    &gSpriteBank00Frame153.frame,
    &gSpriteBank00Frame154.frame,
    &gSpriteBank00Frame155.frame,
    &gSpriteBank00Frame156.frame,
    &gSpriteBank00Frame157.frame,
    &gSpriteBank00Frame158.frame,
    &gSpriteBank00Frame159.frame,
    &gSpriteBank00Frame160.frame,
    &gSpriteBank00Frame161.frame,
    &gSpriteBank00Frame162.frame,
    &gSpriteBank00Frame163.frame,
    &gSpriteBank00Frame164.frame,
    &gSpriteBank00Frame165.frame,
    &gSpriteBank00Frame166.frame,
    &gSpriteBank00Frame167.frame,
    &gSpriteBank00Frame168.frame,
    &gSpriteBank00Frame169.frame,
    &gSpriteBank00Frame170.frame,
    &gSpriteBank00Frame171.frame,
    &gSpriteBank00Frame172.frame,
    &gSpriteBank00Frame173.frame,
    &gSpriteBank00Frame174.frame,
    &gSpriteBank00Frame175.frame,
    &gSpriteBank00Frame176.frame,
    &gSpriteBank00Frame177.frame,
    &gSpriteBank00Frame178.frame,
    &gSpriteBank00Frame179.frame,
    &gSpriteBank00Frame180.frame,
    &gSpriteBank00Frame181.frame,
    &gSpriteBank00Frame182.frame,
    &gSpriteBank00Frame183.frame,
    &gSpriteBank00Frame184.frame,
    &gSpriteBank00Frame185.frame,
    &gSpriteBank00Frame186.frame,
    &gSpriteBank00Frame187.frame,
    &gSpriteBank00Frame188.frame,
    &gSpriteBank00Frame189.frame,
    &gSpriteBank00Frame190.frame,
    &gSpriteBank00Frame191.frame,
    &gSpriteBank00Frame192.frame,
    &gSpriteBank00Frame193.frame,
    &gSpriteBank00Frame194.frame,
    &gSpriteBank00Frame195.frame,
    &gSpriteBank00Frame196.frame,
    &gSpriteBank00Frame197.frame,
    &gSpriteBank00Frame198.frame,
    &gSpriteBank00Frame199.frame,
    &gSpriteBank00Frame200.frame,
    &gSpriteBank00Frame201.frame,
    &gSpriteBank00Frame202.frame,
    &gSpriteBank00Frame203.frame,
    &gSpriteBank00Frame204.frame,
    &gSpriteBank00Frame205.frame,
    &gSpriteBank00Frame206.frame,
    &gSpriteBank00Frame207.frame,
    &gSpriteBank00Frame208.frame,
    &gSpriteBank00Frame209.frame,
    &gSpriteBank00Frame210.frame,
    &gSpriteBank00Frame211.frame,
    &gSpriteBank00Frame212.frame,
    &gSpriteBank00Frame213.frame,
    &gSpriteBank00Frame214.frame,
    &gSpriteBank00Frame215.frame,
    &gSpriteBank00Frame216.frame,
    &gSpriteBank00Frame217.frame,
    &gSpriteBank00Frame218.frame,
    &gSpriteBank00Frame219.frame,
    &gSpriteBank00Frame220.frame,
    &gSpriteBank00Frame221.frame,
    &gSpriteBank00Frame222.frame,
    &gSpriteBank00Frame223.frame,
    &gSpriteBank00Frame224.frame,
    &gSpriteBank00Frame225.frame,
    &gSpriteBank00Frame226.frame,
    &gSpriteBank00Frame227.frame,
    &gSpriteBank00Frame228.frame,
    &gSpriteBank00Frame229.frame,
    &gSpriteBank00Frame230.frame,
    &gSpriteBank00Frame231.frame,
    &gSpriteBank00Frame232.frame,
    &gSpriteBank00Frame233.frame,
    &gSpriteBank00Frame234.frame,
    &gSpriteBank00Frame235.frame,
    &gSpriteBank00Frame236.frame,
    &gSpriteBank00Frame237.frame,
    &gSpriteBank00Frame238.frame,
    &gSpriteBank00Frame239.frame,
    &gSpriteBank00Frame240.frame,
    &gSpriteBank00Frame241.frame,
    &gSpriteBank00Frame242.frame,
    &gSpriteBank00Frame243.frame,
    &gSpriteBank00Frame244.frame,
    &gSpriteBank00Frame245.frame,
    &gSpriteBank00Frame246.frame,
    &gSpriteBank00Frame247.frame,
    &gSpriteBank00Frame248.frame,
    &gSpriteBank00Frame249.frame,
    &gSpriteBank00Frame250.frame,
    &gSpriteBank00Frame251.frame,
    &gSpriteBank00Frame252.frame,
    &gSpriteBank00Frame253.frame,
    &gSpriteBank00Frame254.frame,
    &gSpriteBank00Frame255.frame,
    &gSpriteBank00Frame256.frame,
    &gSpriteBank00Frame257.frame,
    &gSpriteBank00Frame258.frame,
    &gSpriteBank00Frame259.frame,
    &gSpriteBank00Frame260.frame,
    &gSpriteBank00Frame261.frame,
    &gSpriteBank00Frame262.frame,
    &gSpriteBank00Frame263.frame,
    &gSpriteBank00Frame264.frame,
    &gSpriteBank00Frame265.frame,
    &gSpriteBank00Frame266.frame,
    &gSpriteBank00Frame267.frame,
    &gSpriteBank00Frame268.frame,
    &gSpriteBank00Frame269.frame,
    &gSpriteBank00Frame270.frame,
    &gSpriteBank00Frame271.frame,
    &gSpriteBank00Frame272.frame,
    &gSpriteBank00Frame273.frame,
    &gSpriteBank00Frame274.frame,
    &gSpriteBank00Frame275.frame,
    &gSpriteBank00Frame276.frame,
    &gSpriteBank00Frame277.frame,
    &gSpriteBank00Frame278.frame,
    &gSpriteBank00Frame279.frame,
    &gSpriteBank00Frame280.frame,
    &gSpriteBank00Frame281.frame,
    &gSpriteBank00Frame282.frame,
    &gSpriteBank00Frame283.frame,
    &gSpriteBank00Frame284.frame,
    &gSpriteBank00Frame285.frame,
    &gSpriteBank00Frame286.frame,
    &gSpriteBank00Frame287.frame,
    &gSpriteBank00Frame288.frame,
    &gSpriteBank00Frame289.frame,
    &gSpriteBank00Frame290.frame,
    &gSpriteBank00Frame291.frame,
    &gSpriteBank00Frame292.frame,
    &gSpriteBank00Frame293.frame,
    &gSpriteBank00Frame294.frame,
    &gSpriteBank00Frame295.frame,
    &gSpriteBank00Frame296.frame,
    &gSpriteBank00Frame297.frame,
    &gSpriteBank00Frame298.frame,
    &gSpriteBank00Frame299.frame,
    &gSpriteBank00Frame300.frame,
    &gSpriteBank00Frame301.frame,
    &gSpriteBank00Frame302.frame,
    &gSpriteBank00Frame303.frame,
    &gSpriteBank00Frame304.frame,
    &gSpriteBank00Frame305.frame,
    &gSpriteBank00Frame306.frame,
    &gSpriteBank00Frame307.frame,
    &gSpriteBank00Frame308.frame,
    &gSpriteBank00Frame309.frame,
    &gSpriteBank00Frame310.frame,
    &gSpriteBank00Frame311.frame,
    &gSpriteBank00Frame312.frame,
    &gSpriteBank00Frame313.frame,
    &gSpriteBank00Frame314.frame,
    &gSpriteBank00Frame315.frame,
    &gSpriteBank00Frame316.frame,
    &gSpriteBank00Frame317.frame,
    &gSpriteBank00Frame318.frame,
    &gSpriteBank00Frame319.frame,
    &gSpriteBank00Frame320.frame,
    &gSpriteBank00Frame321.frame,
    &gSpriteBank00Frame322.frame,
    &gSpriteBank00Frame323.frame,
    &gSpriteBank00Frame324.frame,
    &gSpriteBank00Frame325.frame,
    &gSpriteBank00Frame326.frame,
    &gSpriteBank00Frame327,
    &gSpriteBank00Frame328,
    &gSpriteBank00Frame329,
    &gSpriteBank00Frame330,
    &gSpriteBank00Frame331,
    &gSpriteBank00Frame332,
    &gSpriteBank00Frame333,
    &gSpriteBank00Frame334,
    &gSpriteBank00Frame335,
    &gSpriteBank00Frame336,
    &gSpriteBank00Frame337,
    &gSpriteBank00Frame338,
    &gSpriteBank00Frame339,
    &gSpriteBank00Frame340,
    &gSpriteBank00Frame341,
    &gSpriteBank00Frame342,
    &gSpriteBank00Frame343,
    &gSpriteBank00Frame344,
    &gSpriteBank00Frame345,
    &gSpriteBank00Frame346.frame,
    &gSpriteBank00Frame347.frame,
    &gSpriteBank00Frame348.frame,
    &gSpriteBank00Frame349.frame,
    &gSpriteBank00Frame350.frame,
    &gSpriteBank00Frame351.frame,
    &gSpriteBank00Frame352.frame,
    &gSpriteBank00Frame353.frame,
    &gSpriteBank00Frame354.frame,
    &gSpriteBank00Frame355.frame,
    &gSpriteBank00Frame356.frame,
    &gSpriteBank00Frame357.frame,
    &gSpriteBank00Frame358.frame,
    &gSpriteBank00Frame359.frame,
    &gSpriteBank00Frame360.frame,
    &gSpriteBank00Frame361.frame,
    &gSpriteBank00Frame362.frame,
    &gSpriteBank00Frame363.frame,
    &gSpriteBank00Frame364.frame,
    &gSpriteBank00Frame365.frame,
    &gSpriteBank00Frame366.frame,
    &gSpriteBank00Frame367.frame,
    &gSpriteBank00Frame368.frame,
    &gSpriteBank00Frame369.frame,
    &gSpriteBank00Frame370.frame,
    &gSpriteBank00Frame371.frame,
    &gSpriteBank00Frame372.frame,
    &gSpriteBank00Frame373.frame,
    &gSpriteBank00Frame374.frame,
    &gSpriteBank00Frame375.frame,
    &gSpriteBank00Frame376.frame,
    &gSpriteBank00Frame377.frame,
    &gSpriteBank00Frame378.frame,
    &gSpriteBank00Frame379.frame,
    &gSpriteBank00Frame380.frame,
    &gSpriteBank00Frame381.frame,
    &gSpriteBank00Frame382.frame,
    &gSpriteBank00Frame383.frame,
    &gSpriteBank00Frame384.frame,
    &gSpriteBank00Frame385.frame,
    &gSpriteBank00Frame386,
    &gSpriteBank00Frame387,
    &gSpriteBank00Frame388,
    &gSpriteBank00Frame389,
    &gSpriteBank00Frame390,
    &gSpriteBank00Frame391,
    &gSpriteBank00Frame392,
    &gSpriteBank00Frame393,
    &gSpriteBank00Frame394,
    &gSpriteBank00Frame395,
    &gSpriteBank00Frame396,
    &gSpriteBank00Frame397,
    &gSpriteBank00Frame398,
    &gSpriteBank00Frame399,
    &gSpriteBank00Frame400,
    &gSpriteBank00Frame401,
    &gSpriteBank00Frame402,
    &gSpriteBank00Frame403,
    &gSpriteBank00Frame404.frame,
    &gSpriteBank00Frame405.frame,
    &gSpriteBank00Frame406.frame,
    &gSpriteBank00Frame407.frame,
    &gSpriteBank00Frame408.frame,
    &gSpriteBank00Frame409.frame,
    &gSpriteBank00Frame410.frame,
    &gSpriteBank00Frame411.frame,
    &gSpriteBank00Frame412.frame,
    &gSpriteBank00Frame413.frame,
    &gSpriteBank00Frame414.frame,
    &gSpriteBank00Frame415.frame,
    &gSpriteBank00Frame416.frame,
    &gSpriteBank00Frame417.frame,
    &gSpriteBank00Frame418.frame,
    &gSpriteBank00Frame419.frame,
    &gSpriteBank00Frame420.frame,
    &gSpriteBank00Frame421.frame,
    &gSpriteBank00Frame422.frame,
    &gSpriteBank00Frame423.frame,
    &gSpriteBank00Frame424.frame,
    &gSpriteBank00Frame425.frame,
    &gSpriteBank00Frame426.frame,
    &gSpriteBank00Frame427.frame,
    &gSpriteBank00Frame428.frame,
    &gSpriteBank00Frame429.frame,
    &gSpriteBank00Frame430.frame,
    &gSpriteBank00Frame431.frame,
    &gSpriteBank00Frame432.frame,
    &gSpriteBank00Frame433.frame,
    &gSpriteBank00Frame434.frame,
    &gSpriteBank00Frame435.frame,
    &gSpriteBank00Frame436.frame,
    &gSpriteBank00Frame437.frame,
    &gSpriteBank00Frame438.frame,
    &gSpriteBank00Frame439.frame,
    &gSpriteBank00Frame440,
    &gSpriteBank00Frame441.frame,
    &gSpriteBank00Frame442.frame,
    &gSpriteBank00Frame443.frame,
    &gSpriteBank00Frame444.frame,
    &gSpriteBank00Frame445.frame,
    &gSpriteBank00Frame446.frame,
    &gSpriteBank00Frame447.frame,
    &gSpriteBank00Frame448.frame,
    &gSpriteBank00Frame449.frame,
    &gSpriteBank00Frame450.frame,
    &gSpriteBank00Frame451.frame,
    &gSpriteBank00Frame452.frame,
    &gSpriteBank00Frame453.frame,
    &gSpriteBank00Frame454.frame,
    &gSpriteBank00Frame455.frame,
    &gSpriteBank00Frame456.frame,
    &gSpriteBank00Frame457.frame,
    &gSpriteBank00Frame458.frame,
    &gSpriteBank00Frame459.frame,
    &gSpriteBank00Frame460.frame,
    &gSpriteBank00Frame461.frame,
    &gSpriteBank00Frame462.frame,
    &gSpriteBank00Frame463.frame,
    &gSpriteBank00Frame464.frame,
};

const struct sprite_frame_1box_anchor gSpriteBank00Frame000 = {
    SPRITE_FRAME(gSpriteBank00Frame000, SPRITE_TILES_BANK00 + 0x00000),
    { { -12, -3, 28, 11 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame001 = {
    SPRITE_FRAME(gSpriteBank00Frame001, SPRITE_TILES_BANK00 + 0x001a0),
    { { -12, -3, 28, 11 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame002 = {
    SPRITE_FRAME(gSpriteBank00Frame002, SPRITE_TILES_BANK00 + 0x00320),
    { { -12, -3, 28, 11 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame003 = {
    SPRITE_FRAME(gSpriteBank00Frame003, SPRITE_TILES_BANK00 + 0x004a0),
    { { -12, -3, 28, 11 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame004 = {
    SPRITE_FRAME(gSpriteBank00Frame004, SPRITE_TILES_BANK00 + 0x00640),
    { { -12, -3, 28, 11 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame005 = {
    SPRITE_FRAME(gSpriteBank00Frame005, SPRITE_TILES_BANK00 + 0x007e0),
    { { -12, -3, 28, 11 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame006 = {
    SPRITE_FRAME(gSpriteBank00Frame006, SPRITE_TILES_BANK00 + 0x009c0),
    { { -12, -3, 28, 11 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame007 = {
    SPRITE_FRAME(gSpriteBank00Frame007, SPRITE_TILES_BANK00 + 0x00c40),
    { { -12, -3, 28, 11 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame008 = {
    SPRITE_FRAME(gSpriteBank00Frame008, SPRITE_TILES_BANK00 + 0x00ec0),
    { { -12, -3, 28, 11 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame009 = {
    SPRITE_FRAME(gSpriteBank00Frame009, SPRITE_TILES_BANK00 + 0x01040),
    { { -12, -3, 28, 11 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame010 = {
    SPRITE_FRAME(gSpriteBank00Frame010, SPRITE_TILES_BANK00 + 0x011a0),
    { { -12, -3, 28, 11 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame011 = {
    SPRITE_FRAME(gSpriteBank00Frame011, SPRITE_TILES_BANK00 + 0x01300),
    { { -12, -3, 28, 11 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame012 = {
    SPRITE_FRAME(gSpriteBank00Frame012, SPRITE_TILES_BANK00 + 0x014a0),
    { { -11, -2, 23, 12 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame013 = {
    SPRITE_FRAME(gSpriteBank00Frame013, SPRITE_TILES_BANK00 + 0x01680),
    { { -7, -5, 21, 15 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame014 = {
    SPRITE_FRAME(gSpriteBank00Frame014, SPRITE_TILES_BANK00 + 0x01880),
    { { -6, -7, 15, 19 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame015 = {
    SPRITE_FRAME(gSpriteBank00Frame015, SPRITE_TILES_BANK00 + 0x01a80),
    { { -6, -14, 22, 26 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame016 = {
    SPRITE_FRAME(gSpriteBank00Frame016, SPRITE_TILES_BANK00 + 0x01be0),
    { { -6, -17, 20, 29 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame017 = {
    SPRITE_FRAME(gSpriteBank00Frame017, SPRITE_TILES_BANK00 + 0x01d40),
    { { -7, -21, 19, 34 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame018 = {
    SPRITE_FRAME(gSpriteBank00Frame018, SPRITE_TILES_BANK00 + 0x01ec0),
    { { -10, -25, 20, 38 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame019 = {
    SPRITE_FRAME(gSpriteBank00Frame019, SPRITE_TILES_BANK00 + 0x01ec0),
    { { -5, -8, 11, 21 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame020 = {
    SPRITE_FRAME(gSpriteBank00Frame020, SPRITE_TILES_BANK00 + 0x02040),
    { { -5, -8, 11, 21 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame021 = {
    SPRITE_FRAME(gSpriteBank00Frame021, SPRITE_TILES_BANK00 + 0x01be0),
    { { -5, -8, 11, 21 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame022 = {
    SPRITE_FRAME(gSpriteBank00Frame022, SPRITE_TILES_BANK00 + 0x01a80),
    { { -5, -8, 11, 21 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame023 = {
    SPRITE_FRAME(gSpriteBank00Frame023, SPRITE_TILES_BANK00 + 0x01a80),
    { { -7, -7, 14, 21 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame024 = {
    SPRITE_FRAME(gSpriteBank00Frame024, SPRITE_TILES_BANK00 + 0x021c0),
    { { -10, -23, 20, 36 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame025 = {
    SPRITE_FRAME(gSpriteBank00Frame025, SPRITE_TILES_BANK00 + 0x02340),
    { { -10, -24, 20, 37 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame026 = {
    SPRITE_FRAME(gSpriteBank00Frame026, SPRITE_TILES_BANK00 + 0x024c0),
    { { -10, -25, 20, 38 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame027 = {
    SPRITE_FRAME(gSpriteBank00Frame027, SPRITE_TILES_BANK00 + 0x02640),
    { { -10, -27, 20, 40 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame028 = {
    SPRITE_FRAME(gSpriteBank00Frame028, SPRITE_TILES_BANK00 + 0x02800),
    { { -10, -28, 20, 41 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame029 = {
    SPRITE_FRAME(gSpriteBank00Frame029, SPRITE_TILES_BANK00 + 0x029e0),
    { { -10, -30, 20, 43 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame030 = {
    SPRITE_FRAME(gSpriteBank00Frame030, SPRITE_TILES_BANK00 + 0x02ba0),
    { { -10, -30, 20, 43 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame031 = {
    SPRITE_FRAME(gSpriteBank00Frame031, SPRITE_TILES_BANK00 + 0x02d60),
    { { -9, -30, 19, 43 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame032 = {
    SPRITE_FRAME(gSpriteBank00Frame032, SPRITE_TILES_BANK00 + 0x02f00),
    { { -9, -29, 19, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame033 = {
    SPRITE_FRAME(gSpriteBank00Frame033, SPRITE_TILES_BANK00 + 0x030a0),
    { { -9, -29, 19, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame034 = {
    SPRITE_FRAME(gSpriteBank00Frame034, SPRITE_TILES_BANK00 + 0x03240),
    { { -8, -27, 18, 40 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame035 = {
    SPRITE_FRAME(gSpriteBank00Frame035, SPRITE_TILES_BANK00 + 0x033e0),
    { { -8, -25, 18, 38 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame036 = {
    SPRITE_FRAME(gSpriteBank00Frame036, SPRITE_TILES_BANK00 + 0x03540),
    { { -8, -24, 17, 37 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame037 = {
    SPRITE_FRAME(gSpriteBank00Frame037, SPRITE_TILES_BANK00 + 0x036a0),
    { { -8, -22, 17, 35 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame038 = {
    SPRITE_FRAME(gSpriteBank00Frame038, SPRITE_TILES_BANK00 + 0x03800),
    { { -8, -22, 17, 35 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame039 = {
    SPRITE_FRAME(gSpriteBank00Frame039, SPRITE_TILES_BANK00 + 0x03960),
    { { -8, -21, 17, 34 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame040 = {
    SPRITE_FRAME(gSpriteBank00Frame040, SPRITE_TILES_BANK00 + 0x03ac0),
    { { -8, -21, 18, 34 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame041 = {
    SPRITE_FRAME(gSpriteBank00Frame041, SPRITE_TILES_BANK00 + 0x03c20),
    { { -8, -20, 18, 33 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame042 = {
    SPRITE_FRAME(gSpriteBank00Frame042, SPRITE_TILES_BANK00 + 0x03d80),
    { { -8, -21, 18, 34 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame043 = {
    SPRITE_FRAME(gSpriteBank00Frame043, SPRITE_TILES_BANK00 + 0x03ee0),
    { { -8, -21, 18, 34 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame044 = {
    SPRITE_FRAME(gSpriteBank00Frame044, SPRITE_TILES_BANK00 + 0x04040),
    { { -8, -22, 18, 35 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame045 = {
    SPRITE_FRAME(gSpriteBank00Frame045, SPRITE_TILES_BANK00 + 0x041a0),
    { { -9, -23, 19, 36 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame046 = {
    SPRITE_FRAME(gSpriteBank00Frame046, SPRITE_TILES_BANK00 + 0x04300),
    { { -10, -23, 20, 36 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame047 = {
    SPRITE_FRAME(gSpriteBank00Frame047, SPRITE_TILES_BANK00 + 0x04480),
    { { -10, -23, 20, 36 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame048 = {
    SPRITE_FRAME(gSpriteBank00Frame048, SPRITE_TILES_BANK00 + 0x04600),
    { { -5, -25, 8, 36 } },
    { -3, -32 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame049 = {
    SPRITE_FRAME(gSpriteBank00Frame049, SPRITE_TILES_BANK00 + 0x04a00),
    { { -4, -20, 8, 28 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame050 = {
    SPRITE_FRAME(gSpriteBank00Frame050, SPRITE_TILES_BANK00 + 0x04ba0),
    { { -4, -17, 11, 20 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame051 = {
    SPRITE_FRAME(gSpriteBank00Frame051, SPRITE_TILES_BANK00 + 0x04dc0),
    { { -5, -11, 15, 13 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame052 = {
    SPRITE_FRAME(gSpriteBank00Frame052, SPRITE_TILES_BANK00 + 0x04fc0),
    { { -7, -14, 14, 13 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame053 = {
    SPRITE_FRAME(gSpriteBank00Frame053, SPRITE_TILES_BANK00 + 0x050e0),
    { { -9, -15, 22, 14 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame054 = {
    SPRITE_FRAME(gSpriteBank00Frame054, SPRITE_TILES_BANK00 + 0x052a0),
    { { 0, 0, 0, 0 }, { -7, -12, 28, 18 }, { 0, 0, 0, 0 } },
    { -12, -24 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame055 = {
    SPRITE_FRAME(gSpriteBank00Frame055, SPRITE_TILES_BANK00 + 0x05520),
    { { -14, -25, 14, 15 }, { -7, -7, 19, 25 }, { 0, 0, 0, 0 } },
    { -7, -28 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame056 = {
    SPRITE_FRAME(gSpriteBank00Frame056, SPRITE_TILES_BANK00 + 0x057a0),
    { { -8, -24, 12, 15 }, { -6, -6, 12, 23 }, { 0, 0, 0, 0 } },
    { 6, -25 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame057 = {
    SPRITE_FRAME(gSpriteBank00Frame057, SPRITE_TILES_BANK00 + 0x05960),
    { { 0, 0, 0, 0 }, { -20, -14, 34, 11 }, { -15, -5, 30, 6 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame058 = {
    SPRITE_FRAME(gSpriteBank00Frame058, SPRITE_TILES_BANK00 + 0x05b60),
    { { 0, 0, 0, 0 }, { -15, -7, 21, 5 }, { -14, -5, 18, 8 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame059 = {
    SPRITE_FRAME(gSpriteBank00Frame059, SPRITE_TILES_BANK00 + 0x05cc0),
    { { 0, 0, 0, 0 }, { -20, -12, 29, 12 }, { -18, -4, 26, 7 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame060 = {
    SPRITE_FRAME(gSpriteBank00Frame060, SPRITE_TILES_BANK00 + 0x05ee0),
    { { 0, 0, 0, 0 }, { -20, -11, 31, 9 }, { -17, -4, 26, 5 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame061 = {
    SPRITE_FRAME(gSpriteBank00Frame061, SPRITE_TILES_BANK00 + 0x06140),
    { { 0, 0, 0, 0 }, { -11, -12, 21, 8 }, { -9, -6, 18, 6 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame062 = {
    SPRITE_FRAME(gSpriteBank00Frame062, SPRITE_TILES_BANK00 + 0x062c0),
    { { 0, 0, 0, 0 }, { -10, -10, 26, 8 }, { -10, -7, 25, 7 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame063 = {
    SPRITE_FRAME(gSpriteBank00Frame063, SPRITE_TILES_BANK00 + 0x062c0),
    { { 0, 0, 0, 0 }, { -19, -10, 42, 10 }, { -9, -4, 25, 6 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame064 = {
    SPRITE_FRAME(gSpriteBank00Frame064, SPRITE_TILES_BANK00 + 0x06440),
    { { -14, -16, 18, 21 }, { -13, -12, 17, 17 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame065 = {
    SPRITE_FRAME(gSpriteBank00Frame065, SPRITE_TILES_BANK00 + 0x06640),
    { { -14, -16, 29, 22 }, { -17, -8, 32, 8 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame066 = {
    SPRITE_FRAME(gSpriteBank00Frame066, SPRITE_TILES_BANK00 + 0x062c0),
    { { 0, 0, 0, 0 }, { -18, -10, 40, 10 }, { -10, -10, 26, 6 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame067 = {
    SPRITE_FRAME(gSpriteBank00Frame067, SPRITE_TILES_BANK00 + 0x06880),
    { { -12, -18, 26, 9 }, { -8, -9, 16, 17 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame068 = {
    SPRITE_FRAME(gSpriteBank00Frame068, SPRITE_TILES_BANK00 + 0x06ac0),
    { { -20, -18, 31, 18 }, { -17, -13, 30, 11 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame069 = {
    SPRITE_FRAME(gSpriteBank00Frame069, SPRITE_TILES_BANK00 + 0x06d60),
    { { 0, 0, 0, 0 }, { -19, -12, 43, 11 }, { -9, -9, 25, 5 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame070 = {
    SPRITE_FRAME(gSpriteBank00Frame070, SPRITE_TILES_BANK00 + 0x06ee0),
    { { -8, -16, 17, 15 }, { -9, -8, 19, 29 }, { 0, 0, 0, 0 } },
    { 9, -22 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame071 = {
    SPRITE_FRAME(gSpriteBank00Frame071, SPRITE_TILES_BANK00 + 0x07140),
    { { -7, -19, 14, 20 }, { -8, -10, 16, 32 }, { 0, 0, 0, 0 } },
    { 9, -24 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame072 = {
    SPRITE_FRAME(gSpriteBank00Frame072, SPRITE_TILES_BANK00 + 0x07300),
    { { -5, -19, 15, 14 }, { -4, -9, 13, 24 }, { 0, 0, 0, 0 } },
    { 9, -22 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame073 = {
    SPRITE_FRAME(gSpriteBank00Frame073, SPRITE_TILES_BANK00 + 0x07460),
    { { -4, -21, 15, 8 }, { -4, -10, 15, 16 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame074 = {
    SPRITE_FRAME(gSpriteBank00Frame074, SPRITE_TILES_BANK00 + 0x075a0),
    { { -4, -24, 14, 8 }, { -4, -14, 14, 18 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame075 = {
    SPRITE_FRAME(gSpriteBank00Frame075, SPRITE_TILES_BANK00 + 0x07720),
    { { -4, -26, 14, 11 }, { -4, -15, 14, 19 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame076 = {
    SPRITE_FRAME(gSpriteBank00Frame076, SPRITE_TILES_BANK00 + 0x078c0),
    { { -4, -24, 15, 10 }, { -4, -13, 15, 18 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame077 = {
    SPRITE_FRAME(gSpriteBank00Frame077, SPRITE_TILES_BANK00 + 0x07a20),
    { { -5, -21, 16, 16 }, { -5, -10, 16, 24 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame078 = {
    SPRITE_FRAME(gSpriteBank00Frame078, SPRITE_TILES_BANK00 + 0x07bc0),
    { { -6, -21, 14, 20 }, { -6, -12, 14, 32 }, { 0, 0, 0, 0 } },
    { 7, -23 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame079 = {
    SPRITE_FRAME(gSpriteBank00Frame079, SPRITE_TILES_BANK00 + 0x07d60),
    { { -8, -23, 14, 21 }, { -8, -14, 14, 33 }, { 0, 0, 0, 0 } },
    { 3, -26 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame080 = {
    SPRITE_FRAME(gSpriteBank00Frame080, SPRITE_TILES_BANK00 + 0x07f00),
    { { -6, -15, 22, 10 }, { -5, -1, 15, 14 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame081 = {
    SPRITE_FRAME(gSpriteBank00Frame081, SPRITE_TILES_BANK00 + 0x08060),
    { { -4, -19, 13, 23 }, { -5, 2, 10, 16 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame082 = {
    SPRITE_FRAME(gSpriteBank00Frame082, SPRITE_TILES_BANK00 + 0x082a0),
    { { -7, -20, 13, 27 }, { -9, 6, 12, 13 }, { 0, 0, 0, 0 } },
    { 9, -22 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame083 = {
    SPRITE_FRAME(gSpriteBank00Frame083, SPRITE_TILES_BANK00 + 0x07140),
    { { -7, -22, 14, 24 }, { -8, 2, 10, 17 }, { 0, 0, 0, 0 } },
    { 7, -24 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame084 = {
    SPRITE_FRAME(gSpriteBank00Frame084, SPRITE_TILES_BANK00 + 0x07300),
    { { -3, -21, 11, 16 }, { -5, -2, 12, 15 }, { 0, 0, 0, 0 } },
    { 10, -22 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame085 = {
    SPRITE_FRAME(gSpriteBank00Frame085, SPRITE_TILES_BANK00 + 0x07460),
    { { -4, -23, 15, 10 }, { -5, -9, 16, 14 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame086 = {
    SPRITE_FRAME(gSpriteBank00Frame086, SPRITE_TILES_BANK00 + 0x075a0),
    { { -4, -18, 15, 7 }, { -6, -7, 17, 11 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame087 = {
    SPRITE_FRAME(gSpriteBank00Frame087, SPRITE_TILES_BANK00 + 0x07720),
    { { -4, -17, 14, 13 }, { -5, -6, 16, 9 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame088 = {
    SPRITE_FRAME(gSpriteBank00Frame088, SPRITE_TILES_BANK00 + 0x078c0),
    { { -4, -20, 14, 11 }, { -5, -8, 17, 12 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame089 = {
    SPRITE_FRAME(gSpriteBank00Frame089, SPRITE_TILES_BANK00 + 0x07a20),
    { { -5, -22, 16, 16 }, { -5, -3, 14, 18 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame090 = {
    SPRITE_FRAME(gSpriteBank00Frame090, SPRITE_TILES_BANK00 + 0x08500),
    { { -7, -24, 16, 24 }, { -6, 2, 14, 18 }, { 0, 0, 0, 0 } },
    { 7, -23 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame091 = {
    SPRITE_FRAME(gSpriteBank00Frame091, SPRITE_TILES_BANK00 + 0x07d60),
    { { -9, -26, 16, 25 }, { -5, -2, 13, 21 }, { 0, 0, 0, 0 } },
    { 3, -26 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame092 = {
    SPRITE_FRAME(gSpriteBank00Frame092, SPRITE_TILES_BANK00 + 0x086a0),
    { { -16, -29, 13, 25 }, { -5, 6, 8, 11 }, { 0, 0, 0, 0 } },
    { -8, -33 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame093 = {
    SPRITE_FRAME(gSpriteBank00Frame093, SPRITE_TILES_BANK00 + 0x08aa0),
    { { -16, -28, 13, 22 }, { -5, 5, 6, 10 }, { 0, 0, 0, 0 } },
    { -7, -33 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame094 = {
    SPRITE_FRAME(gSpriteBank00Frame094, SPRITE_TILES_BANK00 + 0x08ea0),
    { { -15, -27, 14, 20 }, { -5, 2, 8, 12 }, { 0, 0, 0, 0 } },
    { -5, -32 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame095 = {
    SPRITE_FRAME(gSpriteBank00Frame095, SPRITE_TILES_BANK00 + 0x090a0),
    { { -14, -26, 15, 19 }, { -6, 6, 8, 9 }, { 0, 0, 0, 0 } },
    { -3, -30 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame096 = {
    SPRITE_FRAME(gSpriteBank00Frame096, SPRITE_TILES_BANK00 + 0x092a0),
    { { -13, -22, 14, 18 }, { -5, 6, 8, 10 }, { 0, 0, 0, 0 } },
    { -3, -26 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame097 = {
    SPRITE_FRAME(gSpriteBank00Frame097, SPRITE_TILES_BANK00 + 0x09460),
    { { -13, -19, 14, 17 }, { -5, 5, 9, 10 }, { 0, 0, 0, 0 } },
    { -3, -23 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame098 = {
    SPRITE_FRAME(gSpriteBank00Frame098, SPRITE_TILES_BANK00 + 0x09620),
    { { -13, -17, 14, 18 }, { -5, 5, 10, 11 }, { 0, 0, 0, 0 } },
    { -3, -22 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame099 = {
    SPRITE_FRAME(gSpriteBank00Frame099, SPRITE_TILES_BANK00 + 0x09820),
    { { -12, -18, 14, 21 }, { -4, 5, 10, 10 }, { 0, 0, 0, 0 } },
    { -2, -23 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame100 = {
    SPRITE_FRAME(gSpriteBank00Frame100, SPRITE_TILES_BANK00 + 0x09a20),
    { { -7, -16, 13, 19 }, { -4, 5, 9, 10 }, { 0, 0, 0, 0 } },
    { 4, -20 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame101 = {
    SPRITE_FRAME(gSpriteBank00Frame101, SPRITE_TILES_BANK00 + 0x09be0),
    { { -7, -20, 13, 20 }, { -3, 6, 9, 10 }, { 0, 0, 0, 0 } },
    { 4, -25 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame102 = {
    SPRITE_FRAME(gSpriteBank00Frame102, SPRITE_TILES_BANK00 + 0x09da0),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame103 = {
    SPRITE_FRAME(gSpriteBank00Frame103, SPRITE_TILES_BANK00 + 0x09f00),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame104 = {
    SPRITE_FRAME(gSpriteBank00Frame104, SPRITE_TILES_BANK00 + 0x0a140),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame105 = {
    SPRITE_FRAME(gSpriteBank00Frame105, SPRITE_TILES_BANK00 + 0x0a380),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame106 = {
    SPRITE_FRAME(gSpriteBank00Frame106, SPRITE_TILES_BANK00 + 0x0a620),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame107 = {
    SPRITE_FRAME(gSpriteBank00Frame107, SPRITE_TILES_BANK00 + 0x0a8e0),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame108 = {
    SPRITE_FRAME(gSpriteBank00Frame108, SPRITE_TILES_BANK00 + 0x0aba0),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame109 = {
    SPRITE_FRAME(gSpriteBank00Frame109, SPRITE_TILES_BANK00 + 0x0aea0),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame110 = {
    SPRITE_FRAME(gSpriteBank00Frame110, SPRITE_TILES_BANK00 + 0x0b100),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame111 = {
    SPRITE_FRAME(gSpriteBank00Frame111, SPRITE_TILES_BANK00 + 0x0b380),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame112 = {
    SPRITE_FRAME(gSpriteBank00Frame112, SPRITE_TILES_BANK00 + 0x0b560),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame113 = {
    SPRITE_FRAME(gSpriteBank00Frame113, SPRITE_TILES_BANK00 + 0x0b7a0),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame114 = {
    SPRITE_FRAME(gSpriteBank00Frame114, SPRITE_TILES_BANK00 + 0x0b9e0),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame115 = {
    SPRITE_FRAME(gSpriteBank00Frame115, SPRITE_TILES_BANK00 + 0x0bc80),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame116 = {
    SPRITE_FRAME(gSpriteBank00Frame116, SPRITE_TILES_BANK00 + 0x0bfc0),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame117 = {
    SPRITE_FRAME(gSpriteBank00Frame117, SPRITE_TILES_BANK00 + 0x0c2a0),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame118 = {
    SPRITE_FRAME(gSpriteBank00Frame118, SPRITE_TILES_BANK00 + 0x0c540),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame119 = {
    SPRITE_FRAME(gSpriteBank00Frame119, SPRITE_TILES_BANK00 + 0x0c7e0),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame120 = {
    SPRITE_FRAME(gSpriteBank00Frame120, SPRITE_TILES_BANK00 + 0x0ca20),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame121 = {
    SPRITE_FRAME(gSpriteBank00Frame121, SPRITE_TILES_BANK00 + 0x0cc60),
    { { -7, -28, 14, 42 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame122 = {
    SPRITE_FRAME(gSpriteBank00Frame122, SPRITE_TILES_BANK00 + 0x0ce20),
    { { -10, -22, 20, 35 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame123 = {
    SPRITE_FRAME(gSpriteBank00Frame123, SPRITE_TILES_BANK00 + 0x0cfa0),
    { { -9, -23, 19, 36 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame124 = {
    SPRITE_FRAME(gSpriteBank00Frame124, SPRITE_TILES_BANK00 + 0x0d120),
    { { -8, -24, 18, 37 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame125 = {
    SPRITE_FRAME(gSpriteBank00Frame125, SPRITE_TILES_BANK00 + 0x0d280),
    { { -9, -25, 19, 38 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame126 = {
    SPRITE_FRAME(gSpriteBank00Frame126, SPRITE_TILES_BANK00 + 0x0d400),
    { { -9, -25, 19, 38 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame127 = {
    SPRITE_FRAME(gSpriteBank00Frame127, SPRITE_TILES_BANK00 + 0x0d560),
    { { -10, -27, 18, 40 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame128 = {
    SPRITE_FRAME(gSpriteBank00Frame128, SPRITE_TILES_BANK00 + 0x0d700),
    { { -11, -27, 18, 40 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame129 = {
    SPRITE_FRAME(gSpriteBank00Frame129, SPRITE_TILES_BANK00 + 0x0d8a0),
    { { -12, -27, 18, 40 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame130 = {
    SPRITE_FRAME(gSpriteBank00Frame130, SPRITE_TILES_BANK00 + 0x0da60),
    { { -13, -25, 19, 38 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame131 = {
    SPRITE_FRAME(gSpriteBank00Frame131, SPRITE_TILES_BANK00 + 0x0dc20),
    { { -13, -27, 19, 40 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame132 = {
    SPRITE_FRAME(gSpriteBank00Frame132, SPRITE_TILES_BANK00 + 0x0de00),
    { { -13, -28, 19, 41 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame133 = {
    SPRITE_FRAME(gSpriteBank00Frame133, SPRITE_TILES_BANK00 + 0x0dfe0),
    { { -13, -26, 19, 39 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame134 = {
    SPRITE_FRAME(gSpriteBank00Frame134, SPRITE_TILES_BANK00 + 0x0e180),
    { { -14, -27, 20, 40 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame135 = {
    SPRITE_FRAME(gSpriteBank00Frame135, SPRITE_TILES_BANK00 + 0x0e360),
    { { -13, -26, 19, 39 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame136 = {
    SPRITE_FRAME(gSpriteBank00Frame136, SPRITE_TILES_BANK00 + 0x0e500),
    { { -18, -8, 17, 17 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame137 = {
    SPRITE_FRAME(gSpriteBank00Frame137, SPRITE_TILES_BANK00 + 0x0e720),
    { { -18, -8, 17, 17 }, { -4, 2, 20, 10 }, { 5, 3, 12, 9 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame138 = {
    SPRITE_FRAME(gSpriteBank00Frame138, SPRITE_TILES_BANK00 + 0x0e9e0),
    { { -18, -8, 17, 17 }, { -5, 1, 22, 11 }, { 6, 4, 12, 7 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame139 = {
    SPRITE_FRAME(gSpriteBank00Frame139, SPRITE_TILES_BANK00 + 0x0eca0),
    { { -18, -8, 17, 17 }, { -4, 2, 19, 11 }, { 7, 3, 9, 7 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame140 = {
    SPRITE_FRAME(gSpriteBank00Frame140, SPRITE_TILES_BANK00 + 0x0ef00),
    { { -18, -8, 17, 17 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame141 = {
    SPRITE_FRAME(gSpriteBank00Frame141, SPRITE_TILES_BANK00 + 0x0f060),
    { { 0, 0, 0, 0 }, { -18, -22, 32, 32 }, { -18, -8, 32, 16 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame142 = {
    SPRITE_FRAME(gSpriteBank00Frame142, SPRITE_TILES_BANK00 + 0x0f2e0),
    { { 0, 0, 0, 0 }, { -20, -20, 35, 29 }, { -18, -8, 32, 16 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame143 = {
    SPRITE_FRAME(gSpriteBank00Frame143, SPRITE_TILES_BANK00 + 0x0f560),
    { { 0, 0, 0, 0 }, { -20, -16, 32, 28 }, { -18, -8, 32, 16 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame144 = {
    SPRITE_FRAME(gSpriteBank00Frame144, SPRITE_TILES_BANK00 + 0x0f7c0),
    { { 0, 0, 0, 0 }, { -22, -14, 36, 27 }, { -18, -8, 32, 16 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame145 = {
    SPRITE_FRAME(gSpriteBank00Frame145, SPRITE_TILES_BANK00 + 0x0fa40),
    { { 0, 0, 0, 0 }, { -24, -14, 39, 26 }, { -18, -8, 32, 16 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame146 = {
    SPRITE_FRAME(gSpriteBank00Frame146, SPRITE_TILES_BANK00 + 0x0fce0),
    { { 0, 0, 0, 0 }, { -22, -14, 40, 27 }, { -18, -8, 32, 16 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame147 = {
    SPRITE_FRAME(gSpriteBank00Frame147, SPRITE_TILES_BANK00 + 0x0ff40),
    { { 0, 0, 0, 0 }, { -20, -15, 36, 27 }, { -18, -8, 32, 16 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame148 = {
    SPRITE_FRAME(gSpriteBank00Frame148, SPRITE_TILES_BANK00 + 0x101e0),
    { { 0, 0, 0, 0 }, { -20, -15, 40, 27 }, { -18, -8, 32, 16 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame149 = {
    SPRITE_FRAME(gSpriteBank00Frame149, SPRITE_TILES_BANK00 + 0x10440),
    { { 0, 0, 0, 0 }, { -21, -18, 35, 27 }, { -18, -8, 32, 16 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame150 = {
    SPRITE_FRAME(gSpriteBank00Frame150, SPRITE_TILES_BANK00 + 0x106c0),
    { { 0, 0, 0, 0 }, { -19, -21, 29, 32 }, { -18, -8, 32, 16 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame151 = {
    SPRITE_FRAME(gSpriteBank00Frame151, SPRITE_TILES_BANK00 + 0x10940),
    { { -33, -2, 63, 25 }, { -22, 3, 38, 9 }, { -19, 6, 39, 10 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame152 = {
    SPRITE_FRAME(gSpriteBank00Frame152, SPRITE_TILES_BANK00 + 0x10d40),
    { { -31, -2, 58, 25 }, { -22, 3, 39, 10 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame153 = {
    SPRITE_FRAME(gSpriteBank00Frame153, SPRITE_TILES_BANK00 + 0x11140),
    { { -29, -1, 52, 24 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame154 = {
    SPRITE_FRAME(gSpriteBank00Frame154, SPRITE_TILES_BANK00 + 0x11540),
    { { -26, -1, 46, 22 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame155 = {
    SPRITE_FRAME(gSpriteBank00Frame155, SPRITE_TILES_BANK00 + 0x11700),
    { { -26, -1, 43, 20 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame156 = {
    SPRITE_FRAME(gSpriteBank00Frame156, SPRITE_TILES_BANK00 + 0x118a0),
    { { -25, -5, 40, 23 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame157 = {
    SPRITE_FRAME(gSpriteBank00Frame157, SPRITE_TILES_BANK00 + 0x11ac0),
    { { -25, -7, 40, 25 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame158 = {
    SPRITE_FRAME(gSpriteBank00Frame158, SPRITE_TILES_BANK00 + 0x11d80),
    { { -25, -8, 38, 26 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame159 = {
    SPRITE_FRAME(gSpriteBank00Frame159, SPRITE_TILES_BANK00 + 0x11fe0),
    { { -24, -8, 36, 27 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame160 = {
    SPRITE_FRAME(gSpriteBank00Frame160, SPRITE_TILES_BANK00 + 0x12240),
    { { -23, -7, 37, 26 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame161 = {
    SPRITE_FRAME(gSpriteBank00Frame161, SPRITE_TILES_BANK00 + 0x12480),
    { { -21, -7, 37, 26 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame162 = {
    SPRITE_FRAME(gSpriteBank00Frame162, SPRITE_TILES_BANK00 + 0x126c0),
    { { -17, -13, 31, 26 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame163 = {
    SPRITE_FRAME(gSpriteBank00Frame163, SPRITE_TILES_BANK00 + 0x128c0),
    { { -15, -17, 27, 33 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame164 = {
    SPRITE_FRAME(gSpriteBank00Frame164, SPRITE_TILES_BANK00 + 0x12ae0),
    { { -14, -20, 24, 35 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame165 = {
    SPRITE_FRAME(gSpriteBank00Frame165, SPRITE_TILES_BANK00 + 0x12d20),
    { { -13, -21, 21, 35 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame166 = {
    SPRITE_FRAME(gSpriteBank00Frame166, SPRITE_TILES_BANK00 + 0x12ee0),
    { { -12, -23, 21, 37 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame167 = {
    SPRITE_FRAME(gSpriteBank00Frame167, SPRITE_TILES_BANK00 + 0x13080),
    { { -11, -24, 20, 37 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame168 = {
    SPRITE_FRAME(gSpriteBank00Frame168, SPRITE_TILES_BANK00 + 0x13200),
    { { -9, -25, 19, 38 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame169 = {
    SPRITE_FRAME(gSpriteBank00Frame169, SPRITE_TILES_BANK00 + 0x13380),
    { { -10, -25, 20, 38 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame170 = {
    SPRITE_FRAME(gSpriteBank00Frame170, SPRITE_TILES_BANK00 + 0x13500),
    { { -10, -25, 20, 38 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame171 = {
    SPRITE_FRAME(gSpriteBank00Frame171, SPRITE_TILES_BANK00 + 0x13680),
    { { -7, -27, 14, 41 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame172 = {
    SPRITE_FRAME(gSpriteBank00Frame172, SPRITE_TILES_BANK00 + 0x13800),
    { { -7, -27, 14, 41 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame173 = {
    SPRITE_FRAME(gSpriteBank00Frame173, SPRITE_TILES_BANK00 + 0x13980),
    { { -7, -27, 14, 41 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame174 = {
    SPRITE_FRAME(gSpriteBank00Frame174, SPRITE_TILES_BANK00 + 0x13b00),
    { { -7, -27, 14, 41 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame175 = {
    SPRITE_FRAME(gSpriteBank00Frame175, SPRITE_TILES_BANK00 + 0x13c80),
    { { -7, -27, 14, 41 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame176 = {
    SPRITE_FRAME(gSpriteBank00Frame176, SPRITE_TILES_BANK00 + 0x13e00),
    { { -7, -27, 14, 41 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame177 = {
    SPRITE_FRAME(gSpriteBank00Frame177, SPRITE_TILES_BANK00 + 0x13f80),
    { { -7, -27, 14, 41 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame178 = {
    SPRITE_FRAME(gSpriteBank00Frame178, SPRITE_TILES_BANK00 + 0x14140),
    { { -7, -27, 14, 41 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame179 = {
    SPRITE_FRAME(gSpriteBank00Frame179, SPRITE_TILES_BANK00 + 0x14300),
    { { -7, -27, 14, 41 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame180 = {
    SPRITE_FRAME(gSpriteBank00Frame180, SPRITE_TILES_BANK00 + 0x144c0),
    { { -7, -27, 14, 41 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame181 = {
    SPRITE_FRAME(gSpriteBank00Frame181, SPRITE_TILES_BANK00 + 0x14640),
    { { -9, -26, 14, 38 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame182 = {
    SPRITE_FRAME(gSpriteBank00Frame182, SPRITE_TILES_BANK00 + 0x14820),
    { { -14, -26, 21, 41 } },
    { 5, -29 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame183 = {
    SPRITE_FRAME(gSpriteBank00Frame183, SPRITE_TILES_BANK00 + 0x14aa0),
    { { -15, -30, 13, 23 }, { -7, -8, 7, 25 }, { 0, 0, 0, 0 } },
    { -10, -35 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame184 = {
    SPRITE_FRAME(gSpriteBank00Frame184, SPRITE_TILES_BANK00 + 0x01880),
    { { -7, -6, 15, 20 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame185 = {
    SPRITE_FRAME(gSpriteBank00Frame185, SPRITE_TILES_BANK00 + 0x14ea0),
    { { -7, -6, 15, 20 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame186 = {
    SPRITE_FRAME(gSpriteBank00Frame186, SPRITE_TILES_BANK00 + 0x014a0),
    { { -7, -6, 15, 20 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame187 = {
    SPRITE_FRAME(gSpriteBank00Frame187, SPRITE_TILES_BANK00 + 0x150a0),
    { { -8, -24, 11, 25 }, { -7, 2, 9, 12 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame188 = {
    SPRITE_FRAME(gSpriteBank00Frame188, SPRITE_TILES_BANK00 + 0x15220),
    { { -11, -24, 14, 21 }, { -7, -6, 9, 24 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame189 = {
    SPRITE_FRAME(gSpriteBank00Frame189, SPRITE_TILES_BANK00 + 0x15480),
    { { -18, -29, 17, 25 }, { -7, -7, 7, 25 }, { 0, 0, 0, 0 } },
    { -8, -32 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame190 = {
    SPRITE_FRAME(gSpriteBank00Frame190, SPRITE_TILES_BANK00 + 0x15880),
    { { -3, -28, 9, 44 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame191 = {
    SPRITE_FRAME(gSpriteBank00Frame191, SPRITE_TILES_BANK00 + 0x15a20),
    { { -4, -24, 8, 37 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame192 = {
    SPRITE_FRAME(gSpriteBank00Frame192, SPRITE_TILES_BANK00 + 0x15ba0),
    { { -9, -18, 14, 31 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame193 = {
    SPRITE_FRAME(gSpriteBank00Frame193, SPRITE_TILES_BANK00 + 0x15da0),
    { { -11, -15, 18, 28 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame194 = {
    SPRITE_FRAME(gSpriteBank00Frame194, SPRITE_TILES_BANK00 + 0x15fa0),
    { { -9, -14, 17, 27 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame195 = {
    SPRITE_FRAME(gSpriteBank00Frame195, SPRITE_TILES_BANK00 + 0x161a0),
    { { -11, -12, 19, 25 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame196 = {
    SPRITE_FRAME(gSpriteBank00Frame196, SPRITE_TILES_BANK00 + 0x163a0),
    { { -10, -9, 18, 22 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame197 = {
    SPRITE_FRAME(gSpriteBank00Frame197, SPRITE_TILES_BANK00 + 0x164e0),
    { { -3, -11, 11, 24 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame198 = {
    SPRITE_FRAME(gSpriteBank00Frame198, SPRITE_TILES_BANK00 + 0x166e0),
    { { 0, -14, 8, 27 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame199 = {
    SPRITE_FRAME(gSpriteBank00Frame199, SPRITE_TILES_BANK00 + 0x16800),
    { { -1, -18, 7, 31 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame200 = {
    SPRITE_FRAME(gSpriteBank00Frame200, SPRITE_TILES_BANK00 + 0x16940),
    { { -2, -22, 7, 35 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame201 = {
    SPRITE_FRAME(gSpriteBank00Frame201, SPRITE_TILES_BANK00 + 0x16ac0),
    { { -4, -24, 8, 37 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame202 = {
    SPRITE_FRAME(gSpriteBank00Frame202, SPRITE_TILES_BANK00 + 0x16c40),
    { { -4, -25, 8, 38 } },
    { -1, 13 },
};
const struct sprite_frame_3box gSpriteBank00Frame203 = {
    SPRITE_FRAME(gSpriteBank00Frame203, SPRITE_TILES_BANK00 + 0x0f060),
    { { -11, -22, 16, 9 }, { -15, -19, 27, 27 }, { -18, -8, 32, 15 } },
};
const struct sprite_frame_3box gSpriteBank00Frame204 = {
    SPRITE_FRAME(gSpriteBank00Frame204, SPRITE_TILES_BANK00 + 0x16dc0),
    { { -11, -22, 16, 9 }, { -16, -17, 28, 25 }, { -18, -8, 32, 15 } },
};
const struct sprite_frame_3box gSpriteBank00Frame205 = {
    SPRITE_FRAME(gSpriteBank00Frame205, SPRITE_TILES_BANK00 + 0x17040),
    { { -11, -17, 17, 9 }, { -21, -14, 33, 23 }, { -18, -8, 32, 15 } },
};
const struct sprite_frame_3box gSpriteBank00Frame206 = {
    SPRITE_FRAME(gSpriteBank00Frame206, SPRITE_TILES_BANK00 + 0x172a0),
    { { -12, -15, 17, 7 }, { -19, -12, 31, 22 }, { -18, -8, 32, 15 } },
};
const struct sprite_frame_3box gSpriteBank00Frame207 = {
    SPRITE_FRAME(gSpriteBank00Frame207, SPRITE_TILES_BANK00 + 0x17520),
    { { -11, -17, 15, 6 }, { -21, -12, 34, 22 }, { -18, -8, 32, 15 } },
};
const struct sprite_frame_3box gSpriteBank00Frame208 = {
    SPRITE_FRAME(gSpriteBank00Frame208, SPRITE_TILES_BANK00 + 0x177c0),
    { { -10, -15, 16, 5 }, { -19, -12, 35, 22 }, { -18, -8, 32, 15 } },
};
const struct sprite_frame_3box gSpriteBank00Frame209 = {
    SPRITE_FRAME(gSpriteBank00Frame209, SPRITE_TILES_BANK00 + 0x17a20),
    { { -13, -15, 20, 8 }, { -18, -12, 32, 22 }, { -18, -8, 32, 15 } },
};
const struct sprite_frame_3box gSpriteBank00Frame210 = {
    SPRITE_FRAME(gSpriteBank00Frame210, SPRITE_TILES_BANK00 + 0x17cc0),
    { { -12, -16, 20, 8 }, { -18, -13, 34, 22 }, { -18, -8, 32, 15 } },
};
const struct sprite_frame_3box gSpriteBank00Frame211 = {
    SPRITE_FRAME(gSpriteBank00Frame211, SPRITE_TILES_BANK00 + 0x17f20),
    { { -11, -22, 16, 9 }, { -19, -17, 29, 25 }, { -18, -8, 32, 15 } },
};
const struct sprite_frame_3box gSpriteBank00Frame212 = {
    SPRITE_FRAME(gSpriteBank00Frame212, SPRITE_TILES_BANK00 + 0x181a0),
    { { -11, -22, 16, 9 }, { -18, -20, 26, 28 }, { -18, -8, 32, 15 } },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame213 = {
    SPRITE_FRAME(gSpriteBank00Frame213, SPRITE_TILES_BANK00 + 0x18420),
    { { -7, -28, 14, 36 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame214 = {
    SPRITE_FRAME(gSpriteBank00Frame214, SPRITE_TILES_BANK00 + 0x185e0),
    { { -7, -28, 14, 36 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame215 = {
    SPRITE_FRAME(gSpriteBank00Frame215, SPRITE_TILES_BANK00 + 0x18840),
    { { -7, -28, 14, 36 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame216 = {
    SPRITE_FRAME(gSpriteBank00Frame216, SPRITE_TILES_BANK00 + 0x18a60),
    { { -7, -28, 14, 36 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame217 = {
    SPRITE_FRAME(gSpriteBank00Frame217, SPRITE_TILES_BANK00 + 0x18c60),
    { { -7, -27, 14, 38 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame218 = {
    SPRITE_FRAME(gSpriteBank00Frame218, SPRITE_TILES_BANK00 + 0x18ea0),
    { { -12, -29, 11, 23 }, { -8, -6, 8, 18 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame219 = {
    SPRITE_FRAME(gSpriteBank00Frame219, SPRITE_TILES_BANK00 + 0x190a0),
    { { -7, -24, 13, 16 } },
    { -1, 13 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame220 = {
    SPRITE_FRAME(gSpriteBank00Frame220, SPRITE_TILES_BANK00 + 0x19300),
    { { -8, -20, 16, 13 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame221 = {
    SPRITE_FRAME(gSpriteBank00Frame221, SPRITE_TILES_BANK00 + 0x19540),
    { { 0, 0, 0, 0 }, { -10, -19, 28, 15 }, { -11, -4, 27, 5 } },
    { -1, 13 },
};
const struct sprite_frame_1box gSpriteBank00Frame222 = {
    SPRITE_FRAME(gSpriteBank00Frame222, SPRITE_TILES_BANK00 + 0x196c0),
    { { -10, -23, 20, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame223 = {
    SPRITE_FRAME(gSpriteBank00Frame223, SPRITE_TILES_BANK00 + 0x19840),
    { { -11, -23, 21, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame224 = {
    SPRITE_FRAME(gSpriteBank00Frame224, SPRITE_TILES_BANK00 + 0x19a00),
    { { -11, -22, 20, 35 } },
};
const struct sprite_frame_1box gSpriteBank00Frame225 = {
    SPRITE_FRAME(gSpriteBank00Frame225, SPRITE_TILES_BANK00 + 0x19bc0),
    { { -12, -22, 20, 35 } },
};
const struct sprite_frame_1box gSpriteBank00Frame226 = {
    SPRITE_FRAME(gSpriteBank00Frame226, SPRITE_TILES_BANK00 + 0x19d80),
    { { -12, -22, 18, 35 } },
};
const struct sprite_frame_1box gSpriteBank00Frame227 = {
    SPRITE_FRAME(gSpriteBank00Frame227, SPRITE_TILES_BANK00 + 0x19f20),
    { { -12, -22, 18, 35 } },
};
const struct sprite_frame_1box gSpriteBank00Frame228 = {
    SPRITE_FRAME(gSpriteBank00Frame228, SPRITE_TILES_BANK00 + 0x1a0c0),
    { { -12, -22, 18, 35 } },
};
const struct sprite_frame_1box gSpriteBank00Frame229 = {
    SPRITE_FRAME(gSpriteBank00Frame229, SPRITE_TILES_BANK00 + 0x1a260),
    { { -12, -23, 18, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame230 = {
    SPRITE_FRAME(gSpriteBank00Frame230, SPRITE_TILES_BANK00 + 0x1a400),
    { { -12, -23, 18, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame231 = {
    SPRITE_FRAME(gSpriteBank00Frame231, SPRITE_TILES_BANK00 + 0x1a5a0),
    { { -12, -23, 18, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame232 = {
    SPRITE_FRAME(gSpriteBank00Frame232, SPRITE_TILES_BANK00 + 0x1a740),
    { { -13, -23, 19, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame233 = {
    SPRITE_FRAME(gSpriteBank00Frame233, SPRITE_TILES_BANK00 + 0x1a8e0),
    { { -16, -23, 22, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame234 = {
    SPRITE_FRAME(gSpriteBank00Frame234, SPRITE_TILES_BANK00 + 0x1aac0),
    { { -22, -24, 28, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame235 = {
    SPRITE_FRAME(gSpriteBank00Frame235, SPRITE_TILES_BANK00 + 0x1ad40),
    { { -24, -24, 31, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame236 = {
    SPRITE_FRAME(gSpriteBank00Frame236, SPRITE_TILES_BANK00 + 0x1afc0),
    { { -14, -25, 22, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame237 = {
    SPRITE_FRAME(gSpriteBank00Frame237, SPRITE_TILES_BANK00 + 0x1b1a0),
    { { -9, -25, 29, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame238 = {
    SPRITE_FRAME(gSpriteBank00Frame238, SPRITE_TILES_BANK00 + 0x1b3e0),
    { { -9, -24, 29, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame239 = {
    SPRITE_FRAME(gSpriteBank00Frame239, SPRITE_TILES_BANK00 + 0x1b620),
    { { -8, -24, 22, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame240 = {
    SPRITE_FRAME(gSpriteBank00Frame240, SPRITE_TILES_BANK00 + 0x1b7a0),
    { { -8, -24, 20, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame241 = {
    SPRITE_FRAME(gSpriteBank00Frame241, SPRITE_TILES_BANK00 + 0x1b920),
    { { -9, -24, 21, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame242 = {
    SPRITE_FRAME(gSpriteBank00Frame242, SPRITE_TILES_BANK00 + 0x1bac0),
    { { -9, -25, 21, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame243 = {
    SPRITE_FRAME(gSpriteBank00Frame243, SPRITE_TILES_BANK00 + 0x1bc40),
    { { -10, -25, 25, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame244 = {
    SPRITE_FRAME(gSpriteBank00Frame244, SPRITE_TILES_BANK00 + 0x1be80),
    { { -13, -24, 38, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame245 = {
    SPRITE_FRAME(gSpriteBank00Frame245, SPRITE_TILES_BANK00 + 0x1c100),
    { { -15, -24, 41, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame246 = {
    SPRITE_FRAME(gSpriteBank00Frame246, SPRITE_TILES_BANK00 + 0x1c3a0),
    { { -17, -24, 43, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame247 = {
    SPRITE_FRAME(gSpriteBank00Frame247, SPRITE_TILES_BANK00 + 0x1c640),
    { { -17, -25, 43, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame248 = {
    SPRITE_FRAME(gSpriteBank00Frame248, SPRITE_TILES_BANK00 + 0x1c8e0),
    { { -17, -25, 43, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame249 = {
    SPRITE_FRAME(gSpriteBank00Frame249, SPRITE_TILES_BANK00 + 0x1cb80),
    { { -17, -24, 43, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame250 = {
    SPRITE_FRAME(gSpriteBank00Frame250, SPRITE_TILES_BANK00 + 0x1ce20),
    { { -17, -24, 43, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame251 = {
    SPRITE_FRAME(gSpriteBank00Frame251, SPRITE_TILES_BANK00 + 0x1d0c0),
    { { -8, -24, 20, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame252 = {
    SPRITE_FRAME(gSpriteBank00Frame252, SPRITE_TILES_BANK00 + 0x1d240),
    { { -8, -24, 20, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame253 = {
    SPRITE_FRAME(gSpriteBank00Frame253, SPRITE_TILES_BANK00 + 0x1d3c0),
    { { -9, -24, 21, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame254 = {
    SPRITE_FRAME(gSpriteBank00Frame254, SPRITE_TILES_BANK00 + 0x1d560),
    { { -9, -24, 22, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame255 = {
    SPRITE_FRAME(gSpriteBank00Frame255, SPRITE_TILES_BANK00 + 0x1d6e0),
    { { -10, -24, 23, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame256 = {
    SPRITE_FRAME(gSpriteBank00Frame256, SPRITE_TILES_BANK00 + 0x1d920),
    { { -10, -24, 23, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame257 = {
    SPRITE_FRAME(gSpriteBank00Frame257, SPRITE_TILES_BANK00 + 0x1db60),
    { { -10, -24, 23, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame258 = {
    SPRITE_FRAME(gSpriteBank00Frame258, SPRITE_TILES_BANK00 + 0x1dda0),
    { { -10, -24, 23, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame259 = {
    SPRITE_FRAME(gSpriteBank00Frame259, SPRITE_TILES_BANK00 + 0x1dfe0),
    { { -9, -24, 22, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame260 = {
    SPRITE_FRAME(gSpriteBank00Frame260, SPRITE_TILES_BANK00 + 0x1e160),
    { { -8, -24, 21, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame261 = {
    SPRITE_FRAME(gSpriteBank00Frame261, SPRITE_TILES_BANK00 + 0x1e2e0),
    { { -8, -25, 21, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame262 = {
    SPRITE_FRAME(gSpriteBank00Frame262, SPRITE_TILES_BANK00 + 0x1e460),
    { { -8, -25, 21, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame263 = {
    SPRITE_FRAME(gSpriteBank00Frame263, SPRITE_TILES_BANK00 + 0x1e5e0),
    { { -8, -25, 21, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame264 = {
    SPRITE_FRAME(gSpriteBank00Frame264, SPRITE_TILES_BANK00 + 0x1e760),
    { { -8, -25, 21, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame265 = {
    SPRITE_FRAME(gSpriteBank00Frame265, SPRITE_TILES_BANK00 + 0x1e8e0),
    { { -9, -25, 22, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame266 = {
    SPRITE_FRAME(gSpriteBank00Frame266, SPRITE_TILES_BANK00 + 0x1ea60),
    { { -9, -24, 22, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame267 = {
    SPRITE_FRAME(gSpriteBank00Frame267, SPRITE_TILES_BANK00 + 0x1ebe0),
    { { -9, -24, 22, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame268 = {
    SPRITE_FRAME(gSpriteBank00Frame268, SPRITE_TILES_BANK00 + 0x1ed60),
    { { -10, -24, 23, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame269 = {
    SPRITE_FRAME(gSpriteBank00Frame269, SPRITE_TILES_BANK00 + 0x1efa0),
    { { -12, -23, 18, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame270 = {
    SPRITE_FRAME(gSpriteBank00Frame270, SPRITE_TILES_BANK00 + 0x1f140),
    { { -12, -23, 18, 36 } },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame271 = {
    SPRITE_FRAME(gSpriteBank00Frame271, SPRITE_TILES_BANK00 + 0x1f2e0),
    { { -9, -30, 13, 18 }, { -8, -8, 14, 11 }, { 0, 0, 0, 0 } },
    { -1, 13 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame272 = {
    SPRITE_FRAME(gSpriteBank00Frame272, SPRITE_TILES_BANK00 + 0x1f600),
    { { -13, -27, 14, 27 }, { -11, 1, 15, 13 }, { 0, 0, 0, 0 } },
    { 6, -26 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame273 = {
    SPRITE_FRAME(gSpriteBank00Frame273, SPRITE_TILES_BANK00 + 0x1f8a0),
    { { -14, -29, 12, 31 }, { -8, -3, 8, 17 }, { 0, 0, 0, 0 } },
    { -8, -33 },
};
const struct sprite_frame_1box gSpriteBank00Frame274 = {
    SPRITE_FRAME(gSpriteBank00Frame274, SPRITE_TILES_BANK00 + 0x1fca0),
    { { -25, -26, 36, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame275 = {
    SPRITE_FRAME(gSpriteBank00Frame275, SPRITE_TILES_BANK00 + 0x1ff60),
    { { -21, -25, 33, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame276 = {
    SPRITE_FRAME(gSpriteBank00Frame276, SPRITE_TILES_BANK00 + 0x201e0),
    { { -24, -26, 36, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame277 = {
    SPRITE_FRAME(gSpriteBank00Frame277, SPRITE_TILES_BANK00 + 0x20480),
    { { -23, -27, 34, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame278 = {
    SPRITE_FRAME(gSpriteBank00Frame278, SPRITE_TILES_BANK00 + 0x20720),
    { { -6, -28, 24, 41 } },
};
const struct sprite_frame_1box gSpriteBank00Frame279 = {
    SPRITE_FRAME(gSpriteBank00Frame279, SPRITE_TILES_BANK00 + 0x209a0),
    { { -6, -28, 23, 40 } },
};
const struct sprite_frame_1box gSpriteBank00Frame280 = {
    SPRITE_FRAME(gSpriteBank00Frame280, SPRITE_TILES_BANK00 + 0x20c00),
    { { -12, -27, 27, 40 } },
};
const struct sprite_frame_1box gSpriteBank00Frame281 = {
    SPRITE_FRAME(gSpriteBank00Frame281, SPRITE_TILES_BANK00 + 0x20e60),
    { { -24, -27, 35, 41 } },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame282 = {
    SPRITE_FRAME(gSpriteBank00Frame282, SPRITE_TILES_BANK00 + 0x21100),
    { { -8, -25, 11, 37 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame283 = {
    SPRITE_FRAME(gSpriteBank00Frame283, SPRITE_TILES_BANK00 + 0x212c0),
    { { -7, -25, 12, 32 } },
    { -2, -29 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame284 = {
    SPRITE_FRAME(gSpriteBank00Frame284, SPRITE_TILES_BANK00 + 0x21420),
    { { -15, -23, 22, 21 }, { -31, -19, 57, 24 }, { -21, -19, 30, 13 } },
    { -2, -29 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame285 = {
    SPRITE_FRAME(gSpriteBank00Frame285, SPRITE_TILES_BANK00 + 0x217e0),
    { { -15, -23, 26, 23 }, { -31, -19, 57, 24 }, { -21, -19, 34, 15 } },
    { -2, -29 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame286 = {
    SPRITE_FRAME(gSpriteBank00Frame286, SPRITE_TILES_BANK00 + 0x21c20),
    { { -15, -23, 29, 24 }, { -31, -19, 57, 24 }, { -21, -19, 37, 16 } },
    { -2, -29 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame287 = {
    SPRITE_FRAME(gSpriteBank00Frame287, SPRITE_TILES_BANK00 + 0x22060),
    { { -14, -23, 30, 21 }, { -31, -19, 57, 24 }, { -20, -19, 38, 13 } },
    { -2, -29 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame288 = {
    SPRITE_FRAME(gSpriteBank00Frame288, SPRITE_TILES_BANK00 + 0x22460),
    { { -15, -23, 26, 22 }, { -31, -19, 57, 24 }, { -21, -19, 34, 14 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame289 = {
    SPRITE_FRAME(gSpriteBank00Frame289, SPRITE_TILES_BANK00 + 0x22880),
    { { -8, -23, 11, 27 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame290 = {
    SPRITE_FRAME(gSpriteBank00Frame290, SPRITE_TILES_BANK00 + 0x22a00),
    { { -6, -23, 11, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame291 = {
    SPRITE_FRAME(gSpriteBank00Frame291, SPRITE_TILES_BANK00 + 0x22b60),
    { { -4, -23, 9, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame292 = {
    SPRITE_FRAME(gSpriteBank00Frame292, SPRITE_TILES_BANK00 + 0x22cc0),
    { { -4, -23, 10, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame293 = {
    SPRITE_FRAME(gSpriteBank00Frame293, SPRITE_TILES_BANK00 + 0x22e60),
    { { -4, -23, 10, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame294 = {
    SPRITE_FRAME(gSpriteBank00Frame294, SPRITE_TILES_BANK00 + 0x23000),
    { { -4, -23, 10, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame295 = {
    SPRITE_FRAME(gSpriteBank00Frame295, SPRITE_TILES_BANK00 + 0x231a0),
    { { -4, -23, 10, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame296 = {
    SPRITE_FRAME(gSpriteBank00Frame296, SPRITE_TILES_BANK00 + 0x23340),
    { { -5, -23, 10, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame297 = {
    SPRITE_FRAME(gSpriteBank00Frame297, SPRITE_TILES_BANK00 + 0x234c0),
    { { -5, -23, 10, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame298 = {
    SPRITE_FRAME(gSpriteBank00Frame298, SPRITE_TILES_BANK00 + 0x23620),
    { { -7, -23, 10, 27 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame299 = {
    SPRITE_FRAME(gSpriteBank00Frame299, SPRITE_TILES_BANK00 + 0x23780),
    { { -9, -23, 12, 27 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame300 = {
    SPRITE_FRAME(gSpriteBank00Frame300, SPRITE_TILES_BANK00 + 0x23900),
    { { -11, -23, 13, 27 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame301 = {
    SPRITE_FRAME(gSpriteBank00Frame301, SPRITE_TILES_BANK00 + 0x23b40),
    { { -12, -23, 14, 26 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame302 = {
    SPRITE_FRAME(gSpriteBank00Frame302, SPRITE_TILES_BANK00 + 0x23d80),
    { { -12, -23, 14, 27 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame303 = {
    SPRITE_FRAME(gSpriteBank00Frame303, SPRITE_TILES_BANK00 + 0x23fc0),
    { { -10, -23, 13, 27 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame304 = {
    SPRITE_FRAME(gSpriteBank00Frame304, SPRITE_TILES_BANK00 + 0x24200),
    { { -7, -23, 13, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame305 = {
    SPRITE_FRAME(gSpriteBank00Frame305, SPRITE_TILES_BANK00 + 0x24360),
    { { -8, -23, 15, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame306 = {
    SPRITE_FRAME(gSpriteBank00Frame306, SPRITE_TILES_BANK00 + 0x244c0),
    { { -9, -23, 19, 29 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame307 = {
    SPRITE_FRAME(gSpriteBank00Frame307, SPRITE_TILES_BANK00 + 0x24720),
    { { -19, -23, 21, 26 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame308 = {
    SPRITE_FRAME(gSpriteBank00Frame308, SPRITE_TILES_BANK00 + 0x24960),
    { { -20, -23, 21, 31 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame309 = {
    SPRITE_FRAME(gSpriteBank00Frame309, SPRITE_TILES_BANK00 + 0x24be0),
    { { -7, -23, 9, 33 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame310 = {
    SPRITE_FRAME(gSpriteBank00Frame310, SPRITE_TILES_BANK00 + 0x24e60),
    { { -1, -23, 5, 33 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame311 = {
    SPRITE_FRAME(gSpriteBank00Frame311, SPRITE_TILES_BANK00 + 0x24fe0),
    { { -2, -23, 6, 32 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame312 = {
    SPRITE_FRAME(gSpriteBank00Frame312, SPRITE_TILES_BANK00 + 0x251a0),
    { { -4, -23, 10, 29 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame313 = {
    SPRITE_FRAME(gSpriteBank00Frame313, SPRITE_TILES_BANK00 + 0x25400),
    { { -10, -23, 14, 25 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame314 = {
    SPRITE_FRAME(gSpriteBank00Frame314, SPRITE_TILES_BANK00 + 0x25660),
    { { -13, -23, 14, 24 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame315 = {
    SPRITE_FRAME(gSpriteBank00Frame315, SPRITE_TILES_BANK00 + 0x258c0),
    { { -8, -23, 9, 31 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame316 = {
    SPRITE_FRAME(gSpriteBank00Frame316, SPRITE_TILES_BANK00 + 0x25b40),
    { { -6, -23, 8, 36 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame317 = {
    SPRITE_FRAME(gSpriteBank00Frame317, SPRITE_TILES_BANK00 + 0x25d00),
    { { -3, -23, 5, 38 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame318 = {
    SPRITE_FRAME(gSpriteBank00Frame318, SPRITE_TILES_BANK00 + 0x26100),
    { { -7, -23, 10, 37 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame319 = {
    SPRITE_FRAME(gSpriteBank00Frame319, SPRITE_TILES_BANK00 + 0x26380),
    { { -10, -23, 18, 32 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame320 = {
    SPRITE_FRAME(gSpriteBank00Frame320, SPRITE_TILES_BANK00 + 0x26620),
    { { -13, -23, 20, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame321 = {
    SPRITE_FRAME(gSpriteBank00Frame321, SPRITE_TILES_BANK00 + 0x26880),
    { { -19, -23, 20, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame322 = {
    SPRITE_FRAME(gSpriteBank00Frame322, SPRITE_TILES_BANK00 + 0x26ac0),
    { { -7, -23, 10, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame323 = {
    SPRITE_FRAME(gSpriteBank00Frame323, SPRITE_TILES_BANK00 + 0x26c80),
    { { -4, -23, 8, 28 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame324 = {
    SPRITE_FRAME(gSpriteBank00Frame324, SPRITE_TILES_BANK00 + 0x26e20),
    { { -7, -23, 11, 32 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame325 = {
    SPRITE_FRAME(gSpriteBank00Frame325, SPRITE_TILES_BANK00 + 0x27080),
    { { -5, -23, 9, 31 } },
    { -2, -29 },
};
const struct sprite_frame_1box_anchor gSpriteBank00Frame326 = {
    SPRITE_FRAME(gSpriteBank00Frame326, SPRITE_TILES_BANK00 + 0x27260),
    { { -3, -23, 8, 29 } },
    { -2, -29 },
};
const struct sprite_frame gSpriteBank00Frame327 = SPRITE_FRAME(gSpriteBank00Frame327, SPRITE_TILES_BANK00 + 0x27400);
const struct sprite_frame gSpriteBank00Frame328 = SPRITE_FRAME(gSpriteBank00Frame328, SPRITE_TILES_BANK00 + 0x275c0);
const struct sprite_frame gSpriteBank00Frame329 = SPRITE_FRAME(gSpriteBank00Frame329, SPRITE_TILES_BANK00 + 0x277a0);
const struct sprite_frame gSpriteBank00Frame330 = SPRITE_FRAME(gSpriteBank00Frame330, SPRITE_TILES_BANK00 + 0x27fc0);
const struct sprite_frame gSpriteBank00Frame331 = SPRITE_FRAME(gSpriteBank00Frame331, SPRITE_TILES_BANK00 + 0x28780);
const struct sprite_frame gSpriteBank00Frame332 = SPRITE_FRAME(gSpriteBank00Frame332, SPRITE_TILES_BANK00 + 0x28fa0);
const struct sprite_frame gSpriteBank00Frame333 = SPRITE_FRAME(gSpriteBank00Frame333, SPRITE_TILES_BANK00 + 0x29d00);
const struct sprite_frame gSpriteBank00Frame334 = SPRITE_FRAME(gSpriteBank00Frame334, SPRITE_TILES_BANK00 + 0x2a880);
const struct sprite_frame gSpriteBank00Frame335 = SPRITE_FRAME(gSpriteBank00Frame335, SPRITE_TILES_BANK00 + 0x2b4a0);
const struct sprite_frame gSpriteBank00Frame336 = SPRITE_FRAME(gSpriteBank00Frame336, SPRITE_TILES_BANK00 + 0x2c0c0);
const struct sprite_frame gSpriteBank00Frame337 = SPRITE_FRAME(gSpriteBank00Frame337, SPRITE_TILES_BANK00 + 0x2c940);
const struct sprite_frame gSpriteBank00Frame338 = SPRITE_FRAME(gSpriteBank00Frame338, SPRITE_TILES_BANK00 + 0x2d4e0);
const struct sprite_frame gSpriteBank00Frame339 = SPRITE_FRAME(gSpriteBank00Frame339, SPRITE_TILES_BANK00 + 0x2e200);
const struct sprite_frame gSpriteBank00Frame340 = SPRITE_FRAME(gSpriteBank00Frame340, SPRITE_TILES_BANK00 + 0x2ed00);
const struct sprite_frame gSpriteBank00Frame341 = SPRITE_FRAME(gSpriteBank00Frame341, SPRITE_TILES_BANK00 + 0x2f700);
const struct sprite_frame gSpriteBank00Frame342 = SPRITE_FRAME(gSpriteBank00Frame342, SPRITE_TILES_BANK00 + 0x30100);
const struct sprite_frame gSpriteBank00Frame343 = SPRITE_FRAME(gSpriteBank00Frame343, SPRITE_TILES_BANK00 + 0x30d40);
const struct sprite_frame gSpriteBank00Frame344 = SPRITE_FRAME(gSpriteBank00Frame344, SPRITE_TILES_BANK00 + 0x31760);
const struct sprite_frame gSpriteBank00Frame345 = SPRITE_FRAME(gSpriteBank00Frame345, SPRITE_TILES_BANK00 + 0x324a0);
const struct sprite_frame_1box gSpriteBank00Frame346 = {
    SPRITE_FRAME(gSpriteBank00Frame346, SPRITE_TILES_BANK00 + 0x32f40),
    { { -9, -22, 12, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame347 = {
    SPRITE_FRAME(gSpriteBank00Frame347, SPRITE_TILES_BANK00 + 0x33180),
    { { -9, -22, 12, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame348 = {
    SPRITE_FRAME(gSpriteBank00Frame348, SPRITE_TILES_BANK00 + 0x333c0),
    { { -9, -22, 12, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame349 = {
    SPRITE_FRAME(gSpriteBank00Frame349, SPRITE_TILES_BANK00 + 0x33600),
    { { -9, -22, 12, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame350 = {
    SPRITE_FRAME(gSpriteBank00Frame350, SPRITE_TILES_BANK00 + 0x33840),
    { { -9, -22, 12, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame351 = {
    SPRITE_FRAME(gSpriteBank00Frame351, SPRITE_TILES_BANK00 + 0x33aa0),
    { { -9, -22, 12, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame352 = {
    SPRITE_FRAME(gSpriteBank00Frame352, SPRITE_TILES_BANK00 + 0x33d00),
    { { -9, -22, 12, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame353 = {
    SPRITE_FRAME(gSpriteBank00Frame353, SPRITE_TILES_BANK00 + 0x33f40),
    { { -9, -22, 12, 34 } },
};
const struct sprite_frame_1box gSpriteBank00Frame354 = {
    SPRITE_FRAME(gSpriteBank00Frame354, SPRITE_TILES_BANK00 + 0x34180),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame355 = {
    SPRITE_FRAME(gSpriteBank00Frame355, SPRITE_TILES_BANK00 + 0x343c0),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame356 = {
    SPRITE_FRAME(gSpriteBank00Frame356, SPRITE_TILES_BANK00 + 0x34600),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame357 = {
    SPRITE_FRAME(gSpriteBank00Frame357, SPRITE_TILES_BANK00 + 0x347a0),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame358 = {
    SPRITE_FRAME(gSpriteBank00Frame358, SPRITE_TILES_BANK00 + 0x34900),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame359 = {
    SPRITE_FRAME(gSpriteBank00Frame359, SPRITE_TILES_BANK00 + 0x34aa0),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame360 = {
    SPRITE_FRAME(gSpriteBank00Frame360, SPRITE_TILES_BANK00 + 0x34ce0),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame361 = {
    SPRITE_FRAME(gSpriteBank00Frame361, SPRITE_TILES_BANK00 + 0x34f00),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame362 = {
    SPRITE_FRAME(gSpriteBank00Frame362, SPRITE_TILES_BANK00 + 0x35140),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame363 = {
    SPRITE_FRAME(gSpriteBank00Frame363, SPRITE_TILES_BANK00 + 0x35300),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame364 = {
    SPRITE_FRAME(gSpriteBank00Frame364, SPRITE_TILES_BANK00 + 0x35460),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame365 = {
    SPRITE_FRAME(gSpriteBank00Frame365, SPRITE_TILES_BANK00 + 0x355c0),
    { { -8, -22, 15, 36 } },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame366 = {
    SPRITE_FRAME(gSpriteBank00Frame366, SPRITE_TILES_BANK00 + 0x0f060),
    { { 0, 0, 0, 0 }, { -15, -19, 27, 27 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame367 = {
    SPRITE_FRAME(gSpriteBank00Frame367, SPRITE_TILES_BANK00 + 0x16dc0),
    { { 0, 0, 0, 0 }, { -16, -17, 28, 25 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame368 = {
    SPRITE_FRAME(gSpriteBank00Frame368, SPRITE_TILES_BANK00 + 0x17040),
    { { 0, 0, 0, 0 }, { -21, -14, 33, 23 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame369 = {
    SPRITE_FRAME(gSpriteBank00Frame369, SPRITE_TILES_BANK00 + 0x172a0),
    { { 0, 0, 0, 0 }, { -19, -12, 31, 22 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame370 = {
    SPRITE_FRAME(gSpriteBank00Frame370, SPRITE_TILES_BANK00 + 0x17520),
    { { 0, 0, 0, 0 }, { -21, -12, 34, 22 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame371 = {
    SPRITE_FRAME(gSpriteBank00Frame371, SPRITE_TILES_BANK00 + 0x177c0),
    { { 0, 0, 0, 0 }, { -19, -12, 35, 22 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame372 = {
    SPRITE_FRAME(gSpriteBank00Frame372, SPRITE_TILES_BANK00 + 0x17a20),
    { { 0, 0, 0, 0 }, { -18, -12, 32, 22 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame373 = {
    SPRITE_FRAME(gSpriteBank00Frame373, SPRITE_TILES_BANK00 + 0x17cc0),
    { { 0, 0, 0, 0 }, { -18, -13, 34, 22 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame374 = {
    SPRITE_FRAME(gSpriteBank00Frame374, SPRITE_TILES_BANK00 + 0x17f20),
    { { 0, 0, 0, 0 }, { -19, -17, 29, 25 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame375 = {
    SPRITE_FRAME(gSpriteBank00Frame375, SPRITE_TILES_BANK00 + 0x181a0),
    { { 0, 0, 0, 0 }, { -18, -20, 26, 28 }, { -15, -8, 29, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame376 = {
    SPRITE_FRAME(gSpriteBank00Frame376, SPRITE_TILES_BANK00 + 0x0f060),
    { { 0, 0, 0, 0 }, { -15, -19, 27, 27 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame377 = {
    SPRITE_FRAME(gSpriteBank00Frame377, SPRITE_TILES_BANK00 + 0x16dc0),
    { { 0, 0, 0, 0 }, { -16, -17, 28, 25 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame378 = {
    SPRITE_FRAME(gSpriteBank00Frame378, SPRITE_TILES_BANK00 + 0x17040),
    { { 0, 0, 0, 0 }, { -21, -14, 33, 23 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame379 = {
    SPRITE_FRAME(gSpriteBank00Frame379, SPRITE_TILES_BANK00 + 0x172a0),
    { { 0, 0, 0, 0 }, { -19, -12, 31, 22 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame380 = {
    SPRITE_FRAME(gSpriteBank00Frame380, SPRITE_TILES_BANK00 + 0x17520),
    { { 0, 0, 0, 0 }, { -21, -12, 34, 22 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame381 = {
    SPRITE_FRAME(gSpriteBank00Frame381, SPRITE_TILES_BANK00 + 0x177c0),
    { { 0, 0, 0, 0 }, { -19, -12, 35, 22 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame382 = {
    SPRITE_FRAME(gSpriteBank00Frame382, SPRITE_TILES_BANK00 + 0x17a20),
    { { 0, 0, 0, 0 }, { -18, -12, 32, 22 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame383 = {
    SPRITE_FRAME(gSpriteBank00Frame383, SPRITE_TILES_BANK00 + 0x17cc0),
    { { 0, 0, 0, 0 }, { -18, -13, 34, 22 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame384 = {
    SPRITE_FRAME(gSpriteBank00Frame384, SPRITE_TILES_BANK00 + 0x17f20),
    { { 0, 0, 0, 0 }, { -19, -17, 29, 25 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank00Frame385 = {
    SPRITE_FRAME(gSpriteBank00Frame385, SPRITE_TILES_BANK00 + 0x181a0),
    { { 0, 0, 0, 0 }, { -18, -20, 26, 28 }, { -18, -8, 31, 16 } },
    { 0, 0 },
};
const struct sprite_frame gSpriteBank00Frame386 = SPRITE_FRAME(gSpriteBank00Frame386, SPRITE_TILES_BANK00 + 0x324a0);
const struct sprite_frame gSpriteBank00Frame387 = SPRITE_FRAME(gSpriteBank00Frame387, SPRITE_TILES_BANK00 + 0x31760);
const struct sprite_frame gSpriteBank00Frame388 = SPRITE_FRAME(gSpriteBank00Frame388, SPRITE_TILES_BANK00 + 0x30d40);
const struct sprite_frame gSpriteBank00Frame389 = SPRITE_FRAME(gSpriteBank00Frame389, SPRITE_TILES_BANK00 + 0x30100);
const struct sprite_frame gSpriteBank00Frame390 = SPRITE_FRAME(gSpriteBank00Frame390, SPRITE_TILES_BANK00 + 0x2f700);
const struct sprite_frame gSpriteBank00Frame391 = SPRITE_FRAME(gSpriteBank00Frame391, SPRITE_TILES_BANK00 + 0x2ed00);
const struct sprite_frame gSpriteBank00Frame392 = SPRITE_FRAME(gSpriteBank00Frame392, SPRITE_TILES_BANK00 + 0x2e200);
const struct sprite_frame gSpriteBank00Frame393 = SPRITE_FRAME(gSpriteBank00Frame393, SPRITE_TILES_BANK00 + 0x2d4e0);
const struct sprite_frame gSpriteBank00Frame394 = SPRITE_FRAME(gSpriteBank00Frame394, SPRITE_TILES_BANK00 + 0x2c940);
const struct sprite_frame gSpriteBank00Frame395 = SPRITE_FRAME(gSpriteBank00Frame395, SPRITE_TILES_BANK00 + 0x2c0c0);
const struct sprite_frame gSpriteBank00Frame396 = SPRITE_FRAME(gSpriteBank00Frame396, SPRITE_TILES_BANK00 + 0x2b4a0);
const struct sprite_frame gSpriteBank00Frame397 = SPRITE_FRAME(gSpriteBank00Frame397, SPRITE_TILES_BANK00 + 0x2a880);
const struct sprite_frame gSpriteBank00Frame398 = SPRITE_FRAME(gSpriteBank00Frame398, SPRITE_TILES_BANK00 + 0x29d00);
const struct sprite_frame gSpriteBank00Frame399 = SPRITE_FRAME(gSpriteBank00Frame399, SPRITE_TILES_BANK00 + 0x28fa0);
const struct sprite_frame gSpriteBank00Frame400 = SPRITE_FRAME(gSpriteBank00Frame400, SPRITE_TILES_BANK00 + 0x28780);
const struct sprite_frame gSpriteBank00Frame401 = SPRITE_FRAME(gSpriteBank00Frame401, SPRITE_TILES_BANK00 + 0x27fc0);
const struct sprite_frame gSpriteBank00Frame402 = SPRITE_FRAME(gSpriteBank00Frame402, SPRITE_TILES_BANK00 + 0x277a0);
const struct sprite_frame gSpriteBank00Frame403 = SPRITE_FRAME(gSpriteBank00Frame403, SPRITE_TILES_BANK00 + 0x275c0);
const struct sprite_frame_1box gSpriteBank00Frame404 = {
    SPRITE_FRAME(gSpriteBank00Frame404, SPRITE_TILES_BANK00 + 0x35800),
    { { -10, -10, 20, 20 } },
};
const struct sprite_frame_1box gSpriteBank00Frame405 = {
    SPRITE_FRAME(gSpriteBank00Frame405, SPRITE_TILES_BANK00 + 0x358c0),
    { { -10, -10, 21, 17 } },
};
const struct sprite_frame_1box gSpriteBank00Frame406 = {
    SPRITE_FRAME(gSpriteBank00Frame406, SPRITE_TILES_BANK00 + 0x35980),
    { { -10, -10, 20, 15 } },
};
const struct sprite_frame_1box gSpriteBank00Frame407 = {
    SPRITE_FRAME(gSpriteBank00Frame407, SPRITE_TILES_BANK00 + 0x35a20),
    { { -11, -10, 20, 16 } },
};
const struct sprite_frame_1box gSpriteBank00Frame408 = {
    SPRITE_FRAME(gSpriteBank00Frame408, SPRITE_TILES_BANK00 + 0x35ae0),
    { { -11, -9, 17, 18 } },
};
const struct sprite_frame_1box gSpriteBank00Frame409 = {
    SPRITE_FRAME(gSpriteBank00Frame409, SPRITE_TILES_BANK00 + 0x35ba0),
    { { -12, -9, 18, 19 } },
};
const struct sprite_frame_1box gSpriteBank00Frame410 = {
    SPRITE_FRAME(gSpriteBank00Frame410, SPRITE_TILES_BANK00 + 0x35c60),
    { { -13, -9, 20, 18 } },
};
const struct sprite_frame_1box gSpriteBank00Frame411 = {
    SPRITE_FRAME(gSpriteBank00Frame411, SPRITE_TILES_BANK00 + 0x35d40),
    { { -13, -9, 20, 18 } },
};
const struct sprite_frame_1box gSpriteBank00Frame412 = {
    SPRITE_FRAME(gSpriteBank00Frame412, SPRITE_TILES_BANK00 + 0x35e40),
    { { -13, -9, 20, 18 } },
};
const struct sprite_frame_1box gSpriteBank00Frame413 = {
    SPRITE_FRAME(gSpriteBank00Frame413, SPRITE_TILES_BANK00 + 0x35f40),
    { { -13, -9, 20, 18 } },
};
const struct sprite_frame_1box gSpriteBank00Frame414 = {
    SPRITE_FRAME(gSpriteBank00Frame414, SPRITE_TILES_BANK00 + 0x36040),
    { { -12, -18, 25, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame415 = {
    SPRITE_FRAME(gSpriteBank00Frame415, SPRITE_TILES_BANK00 + 0x36280),
    { { -12, -18, 25, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame416 = {
    SPRITE_FRAME(gSpriteBank00Frame416, SPRITE_TILES_BANK00 + 0x364c0),
    { { -12, -18, 25, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame417 = {
    SPRITE_FRAME(gSpriteBank00Frame417, SPRITE_TILES_BANK00 + 0x36700),
    { { -12, -18, 25, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame418 = {
    SPRITE_FRAME(gSpriteBank00Frame418, SPRITE_TILES_BANK00 + 0x36940),
    { { -12, -18, 25, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame419 = {
    SPRITE_FRAME(gSpriteBank00Frame419, SPRITE_TILES_BANK00 + 0x36b80),
    { { -12, -18, 25, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame420 = {
    SPRITE_FRAME(gSpriteBank00Frame420, SPRITE_TILES_BANK00 + 0x36dc0),
    { { -12, -18, 26, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame421 = {
    SPRITE_FRAME(gSpriteBank00Frame421, SPRITE_TILES_BANK00 + 0x37000),
    { { -11, -18, 26, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame422 = {
    SPRITE_FRAME(gSpriteBank00Frame422, SPRITE_TILES_BANK00 + 0x37220),
    { { -11, -18, 28, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame423 = {
    SPRITE_FRAME(gSpriteBank00Frame423, SPRITE_TILES_BANK00 + 0x37440),
    { { -9, -17, 29, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame424 = {
    SPRITE_FRAME(gSpriteBank00Frame424, SPRITE_TILES_BANK00 + 0x37660),
    { { -7, -15, 31, 33 } },
};
const struct sprite_frame_1box gSpriteBank00Frame425 = {
    SPRITE_FRAME(gSpriteBank00Frame425, SPRITE_TILES_BANK00 + 0x37880),
    { { -3, -7, 35, 27 } },
};
const struct sprite_frame_1box gSpriteBank00Frame426 = {
    SPRITE_FRAME(gSpriteBank00Frame426, SPRITE_TILES_BANK00 + 0x37ac0),
    { { -11, 3, 43, 30 } },
};
const struct sprite_frame_1box gSpriteBank00Frame427 = {
    SPRITE_FRAME(gSpriteBank00Frame427, SPRITE_TILES_BANK00 + 0x37d80),
    { { -10, 3, 41, 29 } },
};
const struct sprite_frame_1box gSpriteBank00Frame428 = {
    SPRITE_FRAME(gSpriteBank00Frame428, SPRITE_TILES_BANK00 + 0x38040),
    { { -9, 4, 38, 25 } },
};
const struct sprite_frame_1box gSpriteBank00Frame429 = {
    SPRITE_FRAME(gSpriteBank00Frame429, SPRITE_TILES_BANK00 + 0x382a0),
    { { -14, -23, 28, 46 } },
};
const struct sprite_frame_1box gSpriteBank00Frame430 = {
    SPRITE_FRAME(gSpriteBank00Frame430, SPRITE_TILES_BANK00 + 0x38540),
    { { -14, -23, 28, 46 } },
};
const struct sprite_frame_1box gSpriteBank00Frame431 = {
    SPRITE_FRAME(gSpriteBank00Frame431, SPRITE_TILES_BANK00 + 0x387e0),
    { { -14, -23, 28, 46 } },
};
const struct sprite_frame_1box gSpriteBank00Frame432 = {
    SPRITE_FRAME(gSpriteBank00Frame432, SPRITE_TILES_BANK00 + 0x38a80),
    { { -12, -18, 25, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame433 = {
    SPRITE_FRAME(gSpriteBank00Frame433, SPRITE_TILES_BANK00 + 0x38cc0),
    { { -16, -17, 30, 35 } },
};
const struct sprite_frame_1box gSpriteBank00Frame434 = {
    SPRITE_FRAME(gSpriteBank00Frame434, SPRITE_TILES_BANK00 + 0x38f00),
    { { -17, -17, 32, 35 } },
};
const struct sprite_frame_1box gSpriteBank00Frame435 = {
    SPRITE_FRAME(gSpriteBank00Frame435, SPRITE_TILES_BANK00 + 0x39160),
    { { -21, -25, 40, 44 } },
};
const struct sprite_frame_1box gSpriteBank00Frame436 = {
    SPRITE_FRAME(gSpriteBank00Frame436, SPRITE_TILES_BANK00 + 0x394a0),
    { { -22, -29, 44, 48 } },
};
const struct sprite_frame_1box gSpriteBank00Frame437 = {
    SPRITE_FRAME(gSpriteBank00Frame437, SPRITE_TILES_BANK00 + 0x399a0),
    { { -20, -25, 40, 44 } },
};
const struct sprite_frame_1box gSpriteBank00Frame438 = {
    SPRITE_FRAME(gSpriteBank00Frame438, SPRITE_TILES_BANK00 + 0x39cc0),
    { { -13, -10, 24, 27 } },
};
const struct sprite_frame_1box gSpriteBank00Frame439 = {
    SPRITE_FRAME(gSpriteBank00Frame439, SPRITE_TILES_BANK00 + 0x39ec0),
    { { -4, 6, 7, 9 } },
};
const struct sprite_frame gSpriteBank00Frame440 = { gSpriteBank00Frame441Pos, gSpriteBank00Frame441Pieces, SPRITE_FRAME_TILES(SPRITE_TILES_BANK00 + 0x39f00, 0) };
const struct sprite_frame_1box gSpriteBank00Frame441 = {
    SPRITE_FRAME(gSpriteBank00Frame441, SPRITE_TILES_BANK00 + 0x39f00),
    { { -11, -19, 23, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame442 = {
    SPRITE_FRAME(gSpriteBank00Frame442, SPRITE_TILES_BANK00 + 0x3a140),
    { { -10, -19, 22, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame443 = {
    SPRITE_FRAME(gSpriteBank00Frame443, SPRITE_TILES_BANK00 + 0x3a2e0),
    { { -10, -19, 22, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame444 = {
    SPRITE_FRAME(gSpriteBank00Frame444, SPRITE_TILES_BANK00 + 0x3a480),
    { { -11, -19, 23, 40 } },
};
const struct sprite_frame_1box gSpriteBank00Frame445 = {
    SPRITE_FRAME(gSpriteBank00Frame445, SPRITE_TILES_BANK00 + 0x3a720),
    { { -13, -19, 25, 40 } },
};
const struct sprite_frame_1box gSpriteBank00Frame446 = {
    SPRITE_FRAME(gSpriteBank00Frame446, SPRITE_TILES_BANK00 + 0x3a9c0),
    { { -14, -18, 25, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame447 = {
    SPRITE_FRAME(gSpriteBank00Frame447, SPRITE_TILES_BANK00 + 0x3ac40),
    { { -14, -15, 25, 36 } },
};
const struct sprite_frame_1box gSpriteBank00Frame448 = {
    SPRITE_FRAME(gSpriteBank00Frame448, SPRITE_TILES_BANK00 + 0x3aea0),
    { { -14, -9, 25, 30 } },
};
const struct sprite_frame_1box gSpriteBank00Frame449 = {
    SPRITE_FRAME(gSpriteBank00Frame449, SPRITE_TILES_BANK00 + 0x3b0a0),
    { { -15, -14, 28, 35 } },
};
const struct sprite_frame_1box gSpriteBank00Frame450 = {
    SPRITE_FRAME(gSpriteBank00Frame450, SPRITE_TILES_BANK00 + 0x3b300),
    { { -15, -11, 30, 32 } },
};
const struct sprite_frame_1box gSpriteBank00Frame451 = {
    SPRITE_FRAME(gSpriteBank00Frame451, SPRITE_TILES_BANK00 + 0x3b520),
    { { -15, -10, 30, 31 } },
};
const struct sprite_frame_1box gSpriteBank00Frame452 = {
    SPRITE_FRAME(gSpriteBank00Frame452, SPRITE_TILES_BANK00 + 0x3b720),
    { { -14, -7, 25, 28 } },
};
const struct sprite_frame_1box gSpriteBank00Frame453 = {
    SPRITE_FRAME(gSpriteBank00Frame453, SPRITE_TILES_BANK00 + 0x3b920),
    { { -14, -4, 25, 25 } },
};
const struct sprite_frame_1box gSpriteBank00Frame454 = {
    SPRITE_FRAME(gSpriteBank00Frame454, SPRITE_TILES_BANK00 + 0x3bb20),
    { { -14, 8, 25, 13 } },
};
const struct sprite_frame_1box gSpriteBank00Frame455 = {
    SPRITE_FRAME(gSpriteBank00Frame455, SPRITE_TILES_BANK00 + 0x02ba0),
    { { -10, -21, 20, 43 } },
};
const struct sprite_frame_1box gSpriteBank00Frame456 = {
    SPRITE_FRAME(gSpriteBank00Frame456, SPRITE_TILES_BANK00 + 0x03240),
    { { -8, -18, 18, 40 } },
};
const struct sprite_frame_1box gSpriteBank00Frame457 = {
    SPRITE_FRAME(gSpriteBank00Frame457, SPRITE_TILES_BANK00 + 0x03540),
    { { -8, -15, 17, 37 } },
};
const struct sprite_frame_1box gSpriteBank00Frame458 = {
    SPRITE_FRAME(gSpriteBank00Frame458, SPRITE_TILES_BANK00 + 0x3bc20),
    { { -18, -17, 28, 39 } },
};
const struct sprite_frame_1box gSpriteBank00Frame459 = {
    SPRITE_FRAME(gSpriteBank00Frame459, SPRITE_TILES_BANK00 + 0x3be60),
    { { -19, -23, 34, 45 } },
};
const struct sprite_frame_1box gSpriteBank00Frame460 = {
    SPRITE_FRAME(gSpriteBank00Frame460, SPRITE_TILES_BANK00 + 0x3c1e0),
    { { -23, -27, 38, 49 } },
};
const struct sprite_frame_1box gSpriteBank00Frame461 = {
    SPRITE_FRAME(gSpriteBank00Frame461, SPRITE_TILES_BANK00 + 0x3c680),
    { { -15, -16, 39, 38 } },
};
const struct sprite_frame_1box gSpriteBank00Frame462 = {
    SPRITE_FRAME(gSpriteBank00Frame462, SPRITE_TILES_BANK00 + 0x3c9a0),
    { { -15, -4, 37, 26 } },
};
const struct sprite_frame_1box gSpriteBank00Frame463 = {
    SPRITE_FRAME(gSpriteBank00Frame463, SPRITE_TILES_BANK00 + 0x3cbe0),
    { { -15, -7, 40, 29 } },
};
const struct sprite_frame_1box gSpriteBank00Frame464 = {
    SPRITE_FRAME(gSpriteBank00Frame464, SPRITE_TILES_BANK00 + 0x3ce20),
    { { -15, 4, 39, 18 } },
};

const struct sprite_piece_pos gSpriteBank00Frame000Pos[4] = { { -17, -8 }, { 15, -2 }, { -16, 8 }, { 0, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame001Pos[4] = { { -19, -7 }, { 13, -4 }, { -12, 9 }, { -4, 9 } };
const struct sprite_piece_pos gSpriteBank00Frame002Pos[2] = { { -13, -7 }, { -12, 9 } };
const struct sprite_piece_pos gSpriteBank00Frame003Pos[3] = { { -15, -6 }, { 17, 0 }, { -13, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame004Pos[3] = { { -16, -6 }, { 16, -4 }, { -14, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame005Pos[4] = { { -17, -6 }, { 15, -4 }, { -15, 10 }, { 17, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame006Pos[2] = { { -18, -6 }, { 14, -7 } };
const struct sprite_piece_pos gSpriteBank00Frame007Pos[2] = { { -21, -6 }, { 11, -7 } };
const struct sprite_piece_pos gSpriteBank00Frame008Pos[4] = { { -16, -7 }, { 16, -4 }, { -2, 9 }, { 14, 9 } };
const struct sprite_piece_pos gSpriteBank00Frame009Pos[3] = { { -14, -7 }, { -3, 9 }, { 13, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame010Pos[3] = { { -14, -7 }, { -8, 9 }, { 12, 9 } };
const struct sprite_piece_pos gSpriteBank00Frame011Pos[3] = { { -16, -8 }, { 16, 0 }, { -14, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame012Pos[4] = { { -16, -8 }, { 16, -1 }, { -14, 8 }, { 18, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame013Pos[1] = { { -9, -12 } };
const struct sprite_piece_pos gSpriteBank00Frame014Pos[1] = { { -6, -14 } };
const struct sprite_piece_pos gSpriteBank00Frame015Pos[3] = { { -6, -14 }, { 10, -14 }, { 10, 2 } };
const struct sprite_piece_pos gSpriteBank00Frame016Pos[3] = { { -6, -17 }, { 10, -15 }, { 10, 1 } };
const struct sprite_piece_pos gSpriteBank00Frame017Pos[4] = { { -7, -21 }, { 9, -20 }, { -6, 11 }, { 2, 11 } };
const struct sprite_piece_pos gSpriteBank00Frame018Pos[3] = { { -10, -25 }, { 6, -23 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame019Pos[3] = { { -10, -25 }, { 6, -23 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame020Pos[4] = { { -7, -21 }, { 9, -20 }, { -6, 11 }, { 2, 11 } };
const struct sprite_piece_pos gSpriteBank00Frame021Pos[3] = { { -6, -17 }, { 10, -15 }, { 10, 1 } };
const struct sprite_piece_pos gSpriteBank00Frame022Pos[3] = { { -6, -14 }, { 10, -14 }, { 10, 2 } };
const struct sprite_piece_pos gSpriteBank00Frame023Pos[3] = { { -6, -14 }, { 10, -14 }, { 10, 2 } };
const struct sprite_piece_pos gSpriteBank00Frame024Pos[4] = { { -10, -23 }, { 6, -21 }, { -6, 9 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame025Pos[4] = { { -10, -24 }, { 6, -21 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame026Pos[3] = { { -10, -25 }, { 6, -22 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame027Pos[3] = { { -10, -27 }, { 6, -23 }, { -6, 5 } };
const struct sprite_piece_pos gSpriteBank00Frame028Pos[4] = { { -10, -28 }, { 6, -26 }, { 6, -5 }, { -6, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame029Pos[3] = { { -10, -30 }, { 6, -18 }, { -6, 2 } };
const struct sprite_piece_pos gSpriteBank00Frame030Pos[3] = { { -10, -30 }, { 6, -19 }, { -6, 2 } };
const struct sprite_piece_pos gSpriteBank00Frame031Pos[3] = { { -9, -30 }, { 7, -19 }, { -6, 2 } };
const struct sprite_piece_pos gSpriteBank00Frame032Pos[3] = { { -9, -29 }, { 7, -19 }, { -6, 3 } };
const struct sprite_piece_pos gSpriteBank00Frame033Pos[3] = { { -9, -29 }, { 7, -18 }, { -6, 3 } };
const struct sprite_piece_pos gSpriteBank00Frame034Pos[3] = { { -8, -27 }, { 8, -16 }, { -6, 5 } };
const struct sprite_piece_pos gSpriteBank00Frame035Pos[3] = { { -8, -25 }, { 8, -14 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame036Pos[4] = { { -8, -24 }, { 8, -13 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame037Pos[4] = { { -8, -22 }, { 8, -11 }, { -6, 10 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame038Pos[4] = { { -8, -22 }, { 8, -11 }, { -6, 10 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame039Pos[4] = { { -8, -21 }, { 8, -10 }, { -6, 11 }, { 2, 11 } };
const struct sprite_piece_pos gSpriteBank00Frame040Pos[4] = { { -8, -21 }, { 8, -10 }, { -6, 11 }, { 2, 11 } };
const struct sprite_piece_pos gSpriteBank00Frame041Pos[4] = { { -8, -20 }, { 8, -10 }, { -5, 12 }, { 3, 12 } };
const struct sprite_piece_pos gSpriteBank00Frame042Pos[4] = { { -8, -21 }, { 8, -10 }, { -6, 11 }, { 2, 11 } };
const struct sprite_piece_pos gSpriteBank00Frame043Pos[4] = { { -8, -21 }, { 8, -11 }, { -6, 11 }, { 2, 11 } };
const struct sprite_piece_pos gSpriteBank00Frame044Pos[4] = { { -8, -22 }, { 8, -11 }, { -6, 10 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame045Pos[4] = { { -9, -23 }, { 7, -12 }, { -6, 9 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame046Pos[4] = { { -10, -23 }, { 6, -21 }, { -6, 9 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame047Pos[4] = { { -10, -23 }, { 6, -21 }, { -6, 9 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame048Pos[1] = { { -13, -32 } };
const struct sprite_piece_pos gSpriteBank00Frame049Pos[4] = { { -7, -29 }, { 9, -20 }, { -7, 3 }, { 1, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame050Pos[2] = { { -14, -25 }, { -7, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame051Pos[1] = { { -14, -22 } };
const struct sprite_piece_pos gSpriteBank00Frame052Pos[4] = { { -11, -20 }, { 4, -19 }, { -12, -4 }, { 4, -4 } };
const struct sprite_piece_pos gSpriteBank00Frame053Pos[3] = { { -17, -20 }, { 15, -14 }, { -12, -4 } };
const struct sprite_piece_pos gSpriteBank00Frame054Pos[3] = { { -18, -24 }, { 14, -4 }, { 22, -3 } };
const struct sprite_piece_pos gSpriteBank00Frame055Pos[2] = { { -17, -28 }, { -1, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame056Pos[3] = { { -9, -28 }, { 7, -24 }, { -4, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame057Pos[3] = { { -22, -22 }, { 8, -17 }, { -24, -6 } };
const struct sprite_piece_pos gSpriteBank00Frame058Pos[3] = { { -18, -22 }, { -2, -22 }, { -2, -6 } };
const struct sprite_piece_pos gSpriteBank00Frame059Pos[2] = { { -24, -23 }, { 8, -5 } };
const struct sprite_piece_pos gSpriteBank00Frame060Pos[5] = { { -32, -22 }, { 0, -23 }, { 16, -23 }, { -16, -7 }, { 16, -7 } };
const struct sprite_piece_pos gSpriteBank00Frame061Pos[2] = { { -12, -23 }, { -13, -7 } };
const struct sprite_piece_pos gSpriteBank00Frame062Pos[4] = { { -14, -23 }, { 18, -13 }, { -11, -7 }, { 5, -7 } };
const struct sprite_piece_pos gSpriteBank00Frame063Pos[4] = { { -14, -23 }, { 18, -13 }, { -11, -7 }, { 5, -7 } };
const struct sprite_piece_pos gSpriteBank00Frame064Pos[1] = { { -15, -20 } };
const struct sprite_piece_pos gSpriteBank00Frame065Pos[3] = { { -17, -24 }, { 15, -6 }, { 1, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame066Pos[4] = { { -14, -23 }, { 18, -13 }, { -11, -7 }, { 5, -7 } };
const struct sprite_piece_pos gSpriteBank00Frame067Pos[2] = { { -19, -21 }, { 13, -17 } };
const struct sprite_piece_pos gSpriteBank00Frame068Pos[3] = { { -25, -22 }, { 7, -20 }, { 15, -10 } };
const struct sprite_piece_pos gSpriteBank00Frame069Pos[4] = { { -14, -23 }, { 18, -13 }, { -11, -7 }, { 5, -7 } };
const struct sprite_piece_pos gSpriteBank00Frame070Pos[3] = { { -5, -27 }, { -8, 5 }, { 0, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame071Pos[4] = { { -5, -28 }, { 10, -29 }, { -6, 3 }, { 2, 3 } };
const struct sprite_piece_pos gSpriteBank00Frame072Pos[3] = { { -5, -25 }, { 11, -24 }, { -4, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame073Pos[2] = { { -6, -24 }, { 10, -11 } };
const struct sprite_piece_pos gSpriteBank00Frame074Pos[4] = { { -6, -28 }, { 10, -19 }, { 10, -2 }, { -1, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame075Pos[4] = { { -6, -29 }, { 10, -18 }, { 10, -1 }, { -1, 3 } };
const struct sprite_piece_pos gSpriteBank00Frame076Pos[3] = { { -6, -27 }, { 10, -11 }, { -1, 5 } };
const struct sprite_piece_pos gSpriteBank00Frame077Pos[5] = { { -7, -24 }, { 9, -15 }, { 9, 1 }, { -3, 8 }, { 5, 12 } };
const struct sprite_piece_pos gSpriteBank00Frame078Pos[3] = { { -8, -26 }, { 8, -23 }, { -4, 6 } };
const struct sprite_piece_pos gSpriteBank00Frame079Pos[3] = { { -10, -28 }, { 6, -25 }, { -5, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame080Pos[3] = { { -6, -14 }, { 10, -14 }, { 10, 2 } };
const struct sprite_piece_pos gSpriteBank00Frame081Pos[3] = { { -6, -20 }, { -7, 12 }, { 1, 12 } };
const struct sprite_piece_pos gSpriteBank00Frame082Pos[3] = { { -8, -22 }, { -11, 10 }, { -3, 13 } };
const struct sprite_piece_pos gSpriteBank00Frame083Pos[4] = { { -9, -23 }, { 6, -24 }, { -10, 8 }, { -2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame084Pos[3] = { { -6, -23 }, { 10, -22 }, { -5, 9 } };
const struct sprite_piece_pos gSpriteBank00Frame085Pos[2] = { { -6, -24 }, { 10, -11 } };
const struct sprite_piece_pos gSpriteBank00Frame086Pos[4] = { { -6, -28 }, { 10, -19 }, { 10, -2 }, { -1, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame087Pos[4] = { { -6, -29 }, { 10, -18 }, { 10, -1 }, { -1, 3 } };
const struct sprite_piece_pos gSpriteBank00Frame088Pos[3] = { { -6, -27 }, { 10, -11 }, { -1, 5 } };
const struct sprite_piece_pos gSpriteBank00Frame089Pos[5] = { { -7, -24 }, { 9, -15 }, { 9, 1 }, { -3, 8 }, { 5, 12 } };
const struct sprite_piece_pos gSpriteBank00Frame090Pos[3] = { { -7, -26 }, { 9, -23 }, { -3, 6 } };
const struct sprite_piece_pos gSpriteBank00Frame091Pos[3] = { { -8, -28 }, { 8, -25 }, { -3, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame092Pos[1] = { { -15, -34 } };
const struct sprite_piece_pos gSpriteBank00Frame093Pos[1] = { { -15, -33 } };
const struct sprite_piece_pos gSpriteBank00Frame094Pos[3] = { { -14, -32 }, { 2, -32 }, { -7, 0 } };
const struct sprite_piece_pos gSpriteBank00Frame095Pos[3] = { { -13, -31 }, { 3, -30 }, { -8, 1 } };
const struct sprite_piece_pos gSpriteBank00Frame096Pos[3] = { { -12, -29 }, { 4, -28 }, { -7, 3 } };
const struct sprite_piece_pos gSpriteBank00Frame097Pos[3] = { { -13, -29 }, { 3, -28 }, { -6, 3 } };
const struct sprite_piece_pos gSpriteBank00Frame098Pos[3] = { { -14, -28 }, { 2, -28 }, { -6, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame099Pos[3] = { { -14, -30 }, { 2, -29 }, { -5, 2 } };
const struct sprite_piece_pos gSpriteBank00Frame100Pos[3] = { { -10, -29 }, { 6, -28 }, { -5, 3 } };
const struct sprite_piece_pos gSpriteBank00Frame101Pos[3] = { { -10, -29 }, { 6, -28 }, { -5, 3 } };
const struct sprite_piece_pos gSpriteBank00Frame102Pos[4] = { { -9, -23 }, { 7, -12 }, { -7, 9 }, { 1, 9 } };
const struct sprite_piece_pos gSpriteBank00Frame103Pos[3] = { { -15, -25 }, { -9, 7 }, { -1, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame104Pos[3] = { { -20, -29 }, { -11, 3 }, { -3, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame105Pos[4] = { { -21, -33 }, { 11, -25 }, { 11, -9 }, { -14, -1 } };
const struct sprite_piece_pos gSpriteBank00Frame106Pos[4] = { { -21, -35 }, { 11, -29 }, { 19, -17 }, { -20, -3 } };
const struct sprite_piece_pos gSpriteBank00Frame107Pos[4] = { { -19, -34 }, { 11, -27 }, { 19, -14 }, { -21, -2 } };
const struct sprite_piece_pos gSpriteBank00Frame108Pos[5] = { { -19, -32 }, { 13, -20 }, { 13, -4 }, { -7, 0 }, { 13, 0 } };
const struct sprite_piece_pos gSpriteBank00Frame109Pos[3] = { { -17, -29 }, { -5, 3 }, { 11, 3 } };
const struct sprite_piece_pos gSpriteBank00Frame110Pos[2] = { { -14, -27 }, { -14, 5 } };
const struct sprite_piece_pos gSpriteBank00Frame111Pos[4] = { { -12, -24 }, { 4, -22 }, { -11, 7 }, { 5, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame112Pos[3] = { { -14, -24 }, { -7, 8 }, { 1, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame113Pos[2] = { { -20, -26 }, { -8, 6 } };
const struct sprite_piece_pos gSpriteBank00Frame114Pos[5] = { { -22, -30 }, { 10, -21 }, { 10, 0 }, { -10, 2 }, { 6, 2 } };
const struct sprite_piece_pos gSpriteBank00Frame115Pos[4] = { { -21, -34 }, { 11, -26 }, { 11, -9 }, { -13, -2 } };
const struct sprite_piece_pos gSpriteBank00Frame116Pos[5] = { { -21, -36 }, { 11, -29 }, { 19, -14 }, { -20, -4 }, { -12, -4 } };
const struct sprite_piece_pos gSpriteBank00Frame117Pos[3] = { { -20, -34 }, { 12, -28 }, { -20, -2 } };
const struct sprite_piece_pos gSpriteBank00Frame118Pos[4] = { { -18, -32 }, { 14, -20 }, { 14, -4 }, { 3, 0 } };
const struct sprite_piece_pos gSpriteBank00Frame119Pos[2] = { { -16, -28 }, { 1, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame120Pos[2] = { { -15, -26 }, { -2, 6 } };
const struct sprite_piece_pos gSpriteBank00Frame121Pos[4] = { { -13, -24 }, { 3, -22 }, { -2, 8 }, { 6, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame122Pos[4] = { { -10, -22 }, { 6, -21 }, { -6, 10 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame123Pos[4] = { { -9, -23 }, { 7, -21 }, { -6, 9 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame124Pos[4] = { { -8, -24 }, { 8, -13 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame125Pos[3] = { { -9, -25 }, { 7, -14 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame126Pos[3] = { { -9, -25 }, { 7, -16 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame127Pos[3] = { { -10, -27 }, { 6, -21 }, { -6, 5 } };
const struct sprite_piece_pos gSpriteBank00Frame128Pos[3] = { { -11, -27 }, { 5, -17 }, { -6, 5 } };
const struct sprite_piece_pos gSpriteBank00Frame129Pos[4] = { { -12, -27 }, { 4, -18 }, { 4, 4 }, { -6, 5 } };
const struct sprite_piece_pos gSpriteBank00Frame130Pos[3] = { { -13, -25 }, { 3, -18 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame131Pos[4] = { { -13, -27 }, { 3, -18 }, { 3, 3 }, { -6, 5 } };
const struct sprite_piece_pos gSpriteBank00Frame132Pos[4] = { { -13, -28 }, { 3, -18 }, { 3, 3 }, { -6, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame133Pos[4] = { { -13, -26 }, { 3, -18 }, { 3, 3 }, { -6, 6 } };
const struct sprite_piece_pos gSpriteBank00Frame134Pos[4] = { { -14, -27 }, { 2, -18 }, { 2, 3 }, { -6, 5 } };
const struct sprite_piece_pos gSpriteBank00Frame135Pos[4] = { { -13, -26 }, { 3, -18 }, { 3, 3 }, { -6, 6 } };
const struct sprite_piece_pos gSpriteBank00Frame136Pos[4] = { { -13, -27 }, { 3, -26 }, { -13, 5 }, { 3, 5 } };
const struct sprite_piece_pos gSpriteBank00Frame137Pos[3] = { { -21, -20 }, { 11, 3 }, { -20, 12 } };
const struct sprite_piece_pos gSpriteBank00Frame138Pos[3] = { { -28, -13 }, { 4, -9 }, { 12, 1 } };
const struct sprite_piece_pos gSpriteBank00Frame139Pos[3] = { { -21, -17 }, { 11, 3 }, { -19, 15 } };
const struct sprite_piece_pos gSpriteBank00Frame140Pos[3] = { { -6, -14 }, { 10, -14 }, { 10, 2 } };
const struct sprite_piece_pos gSpriteBank00Frame141Pos[4] = { { -19, -23 }, { 13, -9 }, { -7, 9 }, { 1, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame142Pos[4] = { { -20, -21 }, { 12, -10 }, { -7, 11 }, { 1, 12 } };
const struct sprite_piece_pos gSpriteBank00Frame143Pos[3] = { { -25, -18 }, { 7, -7 }, { 15, -3 } };
const struct sprite_piece_pos gSpriteBank00Frame144Pos[2] = { { -23, -16 }, { 9, -8 } };
const struct sprite_piece_pos gSpriteBank00Frame145Pos[3] = { { -25, -16 }, { 7, -9 }, { 15, -4 } };
const struct sprite_piece_pos gSpriteBank00Frame146Pos[3] = { { -23, -16 }, { 9, -9 }, { 17, -5 } };
const struct sprite_piece_pos gSpriteBank00Frame147Pos[3] = { { -22, -16 }, { 10, -7 }, { 18, 3 } };
const struct sprite_piece_pos gSpriteBank00Frame148Pos[3] = { { -22, -17 }, { 10, -7 }, { 18, -5 } };
const struct sprite_piece_pos gSpriteBank00Frame149Pos[4] = { { -23, -21 }, { 9, -10 }, { -10, 11 }, { -2, 11 } };
const struct sprite_piece_pos gSpriteBank00Frame150Pos[3] = { { -22, -24 }, { 10, -10 }, { -10, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame151Pos[1] = { { -33, -2 } };
const struct sprite_piece_pos gSpriteBank00Frame152Pos[1] = { { -31, -2 } };
const struct sprite_piece_pos gSpriteBank00Frame153Pos[1] = { { -29, -1 } };
const struct sprite_piece_pos gSpriteBank00Frame154Pos[3] = { { -26, -1 }, { 6, 2 }, { 8, 15 } };
const struct sprite_piece_pos gSpriteBank00Frame155Pos[3] = { { -26, -1 }, { 6, 0 }, { 9, 15 } };
const struct sprite_piece_pos gSpriteBank00Frame156Pos[5] = { { -24, -1 }, { 7, -5 }, { 15, 0 }, { -25, 11 }, { 7, 12 } };
const struct sprite_piece_pos gSpriteBank00Frame157Pos[3] = { { -25, -4 }, { 7, -7 }, { 15, -4 } };
const struct sprite_piece_pos gSpriteBank00Frame158Pos[3] = { { -25, -7 }, { 7, -8 }, { 7, 14 } };
const struct sprite_piece_pos gSpriteBank00Frame159Pos[3] = { { -24, -8 }, { 8, -8 }, { 8, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame160Pos[2] = { { -23, -7 }, { 9, -7 } };
const struct sprite_piece_pos gSpriteBank00Frame161Pos[2] = { { -21, -7 }, { 11, -6 } };
const struct sprite_piece_pos gSpriteBank00Frame162Pos[1] = { { -17, -13 } };
const struct sprite_piece_pos gSpriteBank00Frame163Pos[2] = { { -15, -17 }, { -8, 15 } };
const struct sprite_piece_pos gSpriteBank00Frame164Pos[3] = { { -14, -20 }, { -4, 12 }, { 4, 12 } };
const struct sprite_piece_pos gSpriteBank00Frame165Pos[4] = { { -13, -21 }, { 3, -21 }, { -3, 11 }, { 5, 11 } };
const struct sprite_piece_pos gSpriteBank00Frame166Pos[5] = { { -12, -22 }, { 4, -22 }, { 4, -5 }, { -3, 9 }, { 5, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame167Pos[3] = { { -11, -24 }, { 5, -22 }, { -7, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame168Pos[3] = { { -9, -25 }, { 7, -22 }, { -8, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame169Pos[4] = { { -10, -25 }, { 6, -23 }, { -6, 7 }, { 2, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame170Pos[3] = { { -10, -25 }, { 6, -23 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame171Pos[4] = { { -10, -23 }, { 6, -21 }, { -6, 9 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame172Pos[4] = { { -10, -22 }, { 6, -21 }, { -6, 10 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame173Pos[4] = { { -10, -23 }, { 6, -21 }, { -6, 9 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame174Pos[4] = { { -10, -24 }, { 6, -22 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame175Pos[4] = { { -10, -24 }, { 6, -23 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame176Pos[3] = { { -10, -25 }, { 6, -23 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame177Pos[3] = { { -11, -25 }, { 5, -23 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame178Pos[3] = { { -11, -25 }, { 5, -22 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame179Pos[4] = { { -11, -24 }, { 5, -22 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame180Pos[4] = { { -10, -23 }, { 6, -21 }, { -6, 9 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame181Pos[5] = { { -11, -28 }, { 5, -18 }, { 5, -2 }, { -6, 4 }, { 2, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame182Pos[3] = { { -15, -29 }, { -6, 3 }, { 2, 3 } };
const struct sprite_piece_pos gSpriteBank00Frame183Pos[1] = { { -14, -35 } };
const struct sprite_piece_pos gSpriteBank00Frame184Pos[1] = { { -6, -14 } };
const struct sprite_piece_pos gSpriteBank00Frame185Pos[1] = { { -9, -12 } };
const struct sprite_piece_pos gSpriteBank00Frame186Pos[4] = { { -16, -8 }, { 16, -1 }, { -14, 8 }, { 18, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame187Pos[4] = { { -9, -25 }, { 7, -15 }, { -6, 7 }, { 2, 12 } };
const struct sprite_piece_pos gSpriteBank00Frame188Pos[3] = { { -13, -27 }, { -5, 5 }, { 3, 9 } };
const struct sprite_piece_pos gSpriteBank00Frame189Pos[1] = { { -15, -33 } };
const struct sprite_piece_pos gSpriteBank00Frame190Pos[3] = { { -9, -28 }, { 7, -19 }, { -6, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame191Pos[4] = { { -10, -24 }, { 6, -21 }, { -5, 8 }, { 3, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame192Pos[1] = { { -15, -18 } };
const struct sprite_piece_pos gSpriteBank00Frame193Pos[1] = { { -17, -15 } };
const struct sprite_piece_pos gSpriteBank00Frame194Pos[1] = { { -15, -14 } };
const struct sprite_piece_pos gSpriteBank00Frame195Pos[1] = { { -17, -12 } };
const struct sprite_piece_pos gSpriteBank00Frame196Pos[3] = { { -16, -9 }, { -5, 7 }, { 3, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame197Pos[1] = { { -9, -11 } };
const struct sprite_piece_pos gSpriteBank00Frame198Pos[2] = { { -6, -14 }, { 10, -14 } };
const struct sprite_piece_pos gSpriteBank00Frame199Pos[2] = { { -7, -18 }, { 9, -17 } };
const struct sprite_piece_pos gSpriteBank00Frame200Pos[4] = { { -8, -22 }, { 8, -21 }, { -6, 10 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame201Pos[4] = { { -10, -24 }, { 6, -23 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame202Pos[3] = { { -10, -25 }, { 6, -23 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame203Pos[4] = { { -19, -23 }, { 13, -9 }, { -7, 9 }, { 1, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame204Pos[4] = { { -20, -21 }, { 12, -10 }, { -7, 11 }, { 1, 12 } };
const struct sprite_piece_pos gSpriteBank00Frame205Pos[3] = { { -25, -18 }, { 7, -7 }, { 15, -3 } };
const struct sprite_piece_pos gSpriteBank00Frame206Pos[2] = { { -23, -16 }, { 9, -8 } };
const struct sprite_piece_pos gSpriteBank00Frame207Pos[3] = { { -25, -16 }, { 7, -9 }, { 15, -4 } };
const struct sprite_piece_pos gSpriteBank00Frame208Pos[3] = { { -23, -16 }, { 9, -9 }, { 17, -5 } };
const struct sprite_piece_pos gSpriteBank00Frame209Pos[3] = { { -22, -16 }, { 10, -7 }, { 18, 3 } };
const struct sprite_piece_pos gSpriteBank00Frame210Pos[3] = { { -22, -17 }, { 10, -7 }, { 18, -5 } };
const struct sprite_piece_pos gSpriteBank00Frame211Pos[4] = { { -23, -21 }, { 9, -10 }, { -10, 11 }, { -2, 11 } };
const struct sprite_piece_pos gSpriteBank00Frame212Pos[3] = { { -22, -24 }, { 10, -10 }, { -10, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame213Pos[1] = { { -11, -17 } };
const struct sprite_piece_pos gSpriteBank00Frame214Pos[3] = { { -20, -26 }, { 12, -22 }, { 12, -6 } };
const struct sprite_piece_pos gSpriteBank00Frame215Pos[2] = { { -18, -21 }, { 14, -18 } };
const struct sprite_piece_pos gSpriteBank00Frame216Pos[1] = { { -12, -22 } };
const struct sprite_piece_pos gSpriteBank00Frame217Pos[3] = { { -18, -24 }, { 14, -20 }, { 14, -1 } };
const struct sprite_piece_pos gSpriteBank00Frame218Pos[3] = { { -17, -33 }, { -1, -32 }, { -12, -1 } };
const struct sprite_piece_pos gSpriteBank00Frame219Pos[3] = { { -20, -28 }, { 11, -10 }, { -21, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame220Pos[3] = { { -24, -26 }, { 8, -19 }, { 16, -19 } };
const struct sprite_piece_pos gSpriteBank00Frame221Pos[4] = { { -14, -23 }, { 18, -12 }, { -12, -7 }, { 4, -7 } };
const struct sprite_piece_pos gSpriteBank00Frame222Pos[4] = { { -10, -23 }, { 6, -21 }, { -6, 9 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame223Pos[4] = { { -11, -23 }, { 5, -21 }, { -6, 9 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame224Pos[4] = { { -11, -22 }, { 5, -20 }, { -6, 10 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame225Pos[4] = { { -12, -22 }, { 4, -20 }, { -6, 10 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame226Pos[5] = { { -12, -22 }, { 4, -10 }, { 4, 6 }, { -6, 10 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame227Pos[5] = { { -12, -22 }, { 4, -10 }, { 4, 6 }, { -6, 10 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame228Pos[5] = { { -12, -22 }, { 4, -10 }, { 4, 6 }, { -6, 10 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame229Pos[5] = { { -12, -23 }, { 4, -10 }, { 4, 6 }, { -6, 9 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame230Pos[5] = { { -12, -23 }, { 4, -11 }, { 4, 5 }, { -6, 9 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame231Pos[5] = { { -12, -23 }, { 4, -11 }, { 4, 5 }, { -6, 9 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame232Pos[5] = { { -13, -23 }, { 3, -12 }, { 3, 4 }, { -6, 9 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame233Pos[4] = { { -16, -22 }, { 0, -23 }, { -14, 9 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame234Pos[2] = { { -22, -24 }, { -22, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame235Pos[2] = { { -24, -24 }, { -23, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame236Pos[4] = { { -14, -25 }, { 2, -22 }, { -12, 7 }, { 4, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame237Pos[2] = { { -9, -25 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame238Pos[3] = { { -9, -24 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame239Pos[4] = { { -8, -24 }, { 8, -15 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame240Pos[4] = { { -8, -24 }, { 8, -14 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame241Pos[5] = { { -9, -24 }, { 7, -15 }, { 7, 1 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame242Pos[3] = { { -9, -25 }, { 7, -14 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame243Pos[2] = { { -10, -25 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame244Pos[4] = { { -13, -24 }, { 19, -14 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame245Pos[5] = { { -15, -24 }, { 17, -22 }, { 25, -21 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame246Pos[5] = { { -17, -24 }, { 15, -23 }, { 23, -22 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame247Pos[4] = { { -17, -25 }, { 15, -23 }, { 23, -22 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame248Pos[4] = { { -17, -25 }, { 15, -23 }, { 23, -22 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame249Pos[5] = { { -17, -24 }, { 15, -23 }, { 23, -22 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame250Pos[5] = { { -17, -24 }, { 15, -23 }, { 23, -22 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame251Pos[4] = { { -8, -24 }, { 8, -15 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame252Pos[4] = { { -8, -24 }, { 8, -15 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame253Pos[5] = { { -9, -24 }, { 7, -16 }, { 7, 0 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame254Pos[4] = { { -9, -24 }, { 7, -16 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame255Pos[3] = { { -10, -24 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame256Pos[3] = { { -10, -24 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame257Pos[3] = { { -10, -24 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame258Pos[3] = { { -10, -24 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame259Pos[4] = { { -9, -24 }, { 7, -16 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame260Pos[4] = { { -8, -24 }, { 8, -15 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame261Pos[3] = { { -8, -25 }, { 8, -15 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame262Pos[3] = { { -8, -25 }, { 8, -14 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame263Pos[3] = { { -8, -25 }, { 8, -14 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame264Pos[3] = { { -8, -25 }, { 8, -14 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame265Pos[3] = { { -9, -25 }, { 7, -15 }, { -6, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame266Pos[4] = { { -9, -24 }, { 7, -16 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame267Pos[4] = { { -9, -24 }, { 7, -16 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame268Pos[3] = { { -10, -24 }, { -6, 8 }, { 2, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame269Pos[5] = { { -12, -23 }, { 4, -11 }, { 4, 5 }, { -6, 9 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame270Pos[5] = { { -12, -23 }, { 4, -11 }, { 4, 5 }, { -6, 9 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame271Pos[4] = { { -16, -33 }, { 12, -19 }, { -20, -1 }, { 12, -1 } };
const struct sprite_piece_pos gSpriteBank00Frame272Pos[3] = { { -14, -29 }, { -9, 3 }, { 7, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame273Pos[1] = { { -14, -34 } };
const struct sprite_piece_pos gSpriteBank00Frame274Pos[5] = { { -25, -26 }, { 7, -21 }, { 7, -1 }, { -3, 6 }, { 5, 6 } };
const struct sprite_piece_pos gSpriteBank00Frame275Pos[4] = { { -21, -25 }, { 11, -14 }, { -2, 7 }, { 6, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame276Pos[4] = { { -24, -26 }, { 8, -21 }, { -2, 6 }, { 6, 6 } };
const struct sprite_piece_pos gSpriteBank00Frame277Pos[4] = { { -23, -27 }, { 9, -26 }, { -2, 5 }, { 6, 5 } };
const struct sprite_piece_pos gSpriteBank00Frame278Pos[2] = { { -6, -28 }, { -3, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame279Pos[3] = { { -6, -28 }, { -2, 4 }, { 6, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame280Pos[3] = { { -12, -27 }, { -3, 5 }, { 5, 5 } };
const struct sprite_piece_pos gSpriteBank00Frame281Pos[4] = { { -24, -27 }, { 8, -22 }, { -3, 5 }, { 5, 5 } };
const struct sprite_piece_pos gSpriteBank00Frame282Pos[3] = { { -12, -28 }, { 4, -28 }, { -9, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame283Pos[3] = { { -11, -28 }, { 5, -18 }, { -10, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame284Pos[1] = { { -33, -28 } };
const struct sprite_piece_pos gSpriteBank00Frame285Pos[2] = { { -33, -28 }, { -7, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame286Pos[2] = { { -33, -28 }, { -11, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame287Pos[1] = { { -32, -28 } };
const struct sprite_piece_pos gSpriteBank00Frame288Pos[2] = { { -33, -28 }, { -8, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame289Pos[3] = { { -12, -28 }, { 3, -25 }, { -13, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame290Pos[3] = { { -10, -28 }, { 5, -18 }, { -11, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame291Pos[3] = { { -9, -28 }, { 7, -18 }, { -8, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame292Pos[4] = { { -9, -28 }, { 7, -18 }, { 7, -2 }, { -6, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame293Pos[4] = { { -9, -28 }, { 7, -19 }, { 7, -3 }, { -4, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame294Pos[4] = { { -9, -28 }, { 7, -19 }, { 7, -3 }, { -4, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame295Pos[4] = { { -9, -28 }, { 7, -18 }, { 7, 1 }, { -5, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame296Pos[4] = { { -10, -28 }, { 6, -18 }, { 6, 3 }, { -7, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame297Pos[3] = { { -10, -28 }, { 6, -18 }, { -9, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame298Pos[3] = { { -12, -28 }, { 4, -18 }, { -12, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame299Pos[3] = { { -14, -28 }, { 2, -25 }, { -14, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame300Pos[2] = { { -15, -28 }, { -16, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame301Pos[2] = { { -16, -28 }, { -17, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame302Pos[2] = { { -16, -28 }, { -17, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame303Pos[2] = { { -14, -28 }, { -15, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame304Pos[3] = { { -10, -28 }, { 6, -17 }, { -10, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame305Pos[3] = { { -11, -28 }, { 5, -18 }, { -11, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame306Pos[3] = { { -12, -28 }, { -11, 4 }, { 5, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame307Pos[3] = { { -26, -28 }, { 6, -28 }, { -12, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame308Pos[3] = { { -27, -28 }, { 5, -28 }, { -11, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame309Pos[2] = { { -14, -28 }, { -14, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame310Pos[4] = { { -8, -28 }, { 8, -15 }, { -7, 4 }, { 1, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame311Pos[4] = { { -9, -28 }, { 7, -17 }, { 7, 1 }, { -7, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame312Pos[3] = { { -11, -28 }, { -11, 4 }, { 5, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame313Pos[3] = { { -16, -28 }, { -17, 4 }, { -1, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame314Pos[3] = { { -19, -28 }, { -20, 4 }, { -4, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame315Pos[2] = { { -14, -28 }, { -15, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame316Pos[3] = { { -8, -28 }, { 3, -23 }, { -13, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame317Pos[1] = { { -10, -28 } };
const struct sprite_piece_pos gSpriteBank00Frame318Pos[3] = { { -14, -28 }, { -8, 4 }, { 0, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame319Pos[3] = { { -17, -28 }, { 15, -19 }, { -11, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame320Pos[3] = { { -20, -28 }, { 12, -28 }, { -14, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame321Pos[3] = { { -25, -28 }, { 7, -19 }, { -11, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame322Pos[4] = { { -13, -28 }, { 3, -28 }, { -8, 4 }, { 0, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame323Pos[4] = { { -10, -28 }, { 6, -19 }, { 6, -3 }, { -6, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame324Pos[3] = { { -13, -28 }, { -7, 4 }, { 1, 6 } };
const struct sprite_piece_pos gSpriteBank00Frame325Pos[4] = { { -11, -28 }, { 5, -18 }, { 5, -2 }, { -6, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame326Pos[4] = { { -9, -28 }, { 7, -18 }, { 7, -2 }, { -5, 4 } };
const struct sprite_piece_pos gSpriteBank00Frame327Pos[3] = { { -11, -25 }, { 5, -26 }, { -7, 6 } };
const struct sprite_piece_pos gSpriteBank00Frame328Pos[4] = { { -10, -24 }, { 6, -25 }, { -9, 7 }, { 7, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame329Pos[2] = { { -42, -48 }, { 34, -4 } };
const struct sprite_piece_pos gSpriteBank00Frame330Pos[1] = { { -32, -39 } };
const struct sprite_piece_pos gSpriteBank00Frame331Pos[2] = { { -31, -46 }, { 33, -18 } };
const struct sprite_piece_pos gSpriteBank00Frame332Pos[5] = { { -42, -52 }, { 22, -50 }, { -27, 12 }, { 5, 12 }, { 21, 12 } };
const struct sprite_piece_pos gSpriteBank00Frame333Pos[9] = { { -38, -71 }, { 26, -43 }, { 42, -27 }, { 26, -11 }, { -30, -7 }, { 2, -7 }, { 18, -7 }, { -9, 9 }, { 7, 9 } };
const struct sprite_piece_pos gSpriteBank00Frame334Pos[7] = { { -37, -72 }, { 27, -39 }, { -34, -8 }, { -2, -8 }, { 30, 1 }, { -31, 8 }, { 1, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame335Pos[9] = { { -38, -70 }, { 26, -43 }, { 26, -8 }, { -31, -6 }, { 1, -6 }, { 33, 1 }, { -25, 10 }, { 7, 10 }, { 23, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame336Pos[4] = { { -34, -59 }, { 30, -3 }, { -9, 5 }, { 7, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame337Pos[10] = { { -34, -72 }, { 26, -28 }, { 40, -15 }, { 24, -11 }, { -40, -8 }, { -8, -8 }, { 30, -8 }, { 40, -1 }, { -9, 8 }, { 7, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame338Pos[5] = { { -32, -72 }, { 27, -27 }, { 27, -10 }, { -37, -8 }, { 27, -8 } };
const struct sprite_piece_pos gSpriteBank00Frame339Pos[5] = { { -22, -72 }, { -31, -8 }, { 1, -8 }, { -29, 8 }, { 3, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame340Pos[3] = { { -21, -66 }, { -31, -2 }, { 1, -2 } };
const struct sprite_piece_pos gSpriteBank00Frame341Pos[6] = { { -40, -58 }, { 24, -34 }, { 40, -18 }, { 24, -2 }, { -10, 6 }, { 22, 6 } };
const struct sprite_piece_pos gSpriteBank00Frame342Pos[4] = { { -32, -72 }, { 30, -17 }, { -34, -8 }, { 30, 2 } };
const struct sprite_piece_pos gSpriteBank00Frame343Pos[4] = { { -29, -72 }, { -29, -8 }, { 3, -8 }, { 35, -4 } };
const struct sprite_piece_pos gSpriteBank00Frame344Pos[6] = { { -37, -72 }, { 27, -19 }, { -34, -8 }, { 30, -4 }, { 38, -4 }, { 38, 12 } };
const struct sprite_piece_pos gSpriteBank00Frame345Pos[8] = { { -36, -65 }, { 28, -36 }, { 28, -18 }, { -20, -1 }, { 11, -1 }, { 27, -1 }, { -21, 15 }, { 13, 15 } };
const struct sprite_piece_pos gSpriteBank00Frame346Pos[3] = { { -16, -22 }, { 4, 10 }, { 12, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame347Pos[3] = { { -16, -23 }, { 3, 9 }, { 11, 9 } };
const struct sprite_piece_pos gSpriteBank00Frame348Pos[3] = { { -14, -23 }, { 3, 9 }, { 11, 9 } };
const struct sprite_piece_pos gSpriteBank00Frame349Pos[3] = { { -13, -23 }, { 4, 9 }, { 12, 9 } };
const struct sprite_piece_pos gSpriteBank00Frame350Pos[3] = { { -13, -24 }, { -7, 8 }, { 9, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame351Pos[3] = { { -14, -24 }, { -7, 8 }, { 9, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame352Pos[3] = { { -15, -23 }, { 3, 9 }, { 11, 9 } };
const struct sprite_piece_pos gSpriteBank00Frame353Pos[3] = { { -16, -22 }, { 4, 10 }, { 12, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame354Pos[3] = { { -13, -22 }, { -5, 10 }, { 3, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame355Pos[3] = { { -13, -23 }, { -6, 9 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame356Pos[5] = { { -13, -23 }, { 3, -23 }, { 3, -7 }, { -7, 9 }, { 1, 9 } };
const struct sprite_piece_pos gSpriteBank00Frame357Pos[4] = { { -10, -23 }, { 6, -13 }, { -10, 9 }, { -2, 9 } };
const struct sprite_piece_pos gSpriteBank00Frame358Pos[4] = { { -9, -22 }, { 5, -23 }, { 5, -7 }, { -11, 9 } };
const struct sprite_piece_pos gSpriteBank00Frame359Pos[3] = { { -13, -23 }, { -11, 9 }, { -3, 9 } };
const struct sprite_piece_pos gSpriteBank00Frame360Pos[2] = { { -13, -23 }, { -12, 9 } };
const struct sprite_piece_pos gSpriteBank00Frame361Pos[3] = { { -14, -24 }, { -16, 8 }, { 3, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame362Pos[3] = { { -11, -24 }, { 4, -22 }, { -11, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame363Pos[4] = { { -8, -23 }, { 7, -12 }, { -9, 9 }, { -1, 9 } };
const struct sprite_piece_pos gSpriteBank00Frame364Pos[4] = { { -10, -22 }, { 6, -11 }, { -6, 10 }, { 2, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame365Pos[3] = { { -13, -22 }, { -5, 10 }, { 3, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame366Pos[4] = { { -19, -23 }, { 13, -9 }, { -7, 9 }, { 1, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame367Pos[4] = { { -20, -21 }, { 12, -10 }, { -7, 11 }, { 1, 12 } };
const struct sprite_piece_pos gSpriteBank00Frame368Pos[3] = { { -25, -18 }, { 7, -7 }, { 15, -3 } };
const struct sprite_piece_pos gSpriteBank00Frame369Pos[2] = { { -23, -16 }, { 9, -8 } };
const struct sprite_piece_pos gSpriteBank00Frame370Pos[3] = { { -25, -16 }, { 7, -9 }, { 15, -4 } };
const struct sprite_piece_pos gSpriteBank00Frame371Pos[3] = { { -23, -16 }, { 9, -9 }, { 17, -5 } };
const struct sprite_piece_pos gSpriteBank00Frame372Pos[3] = { { -22, -16 }, { 10, -7 }, { 18, 3 } };
const struct sprite_piece_pos gSpriteBank00Frame373Pos[3] = { { -22, -17 }, { 10, -7 }, { 18, -5 } };
const struct sprite_piece_pos gSpriteBank00Frame374Pos[4] = { { -23, -21 }, { 9, -10 }, { -10, 11 }, { -2, 11 } };
const struct sprite_piece_pos gSpriteBank00Frame375Pos[3] = { { -22, -24 }, { 10, -10 }, { -10, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame376Pos[4] = { { -19, -23 }, { 13, -9 }, { -7, 9 }, { 1, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame377Pos[4] = { { -20, -21 }, { 12, -10 }, { -7, 11 }, { 1, 12 } };
const struct sprite_piece_pos gSpriteBank00Frame378Pos[3] = { { -25, -18 }, { 7, -7 }, { 15, -3 } };
const struct sprite_piece_pos gSpriteBank00Frame379Pos[2] = { { -23, -16 }, { 9, -8 } };
const struct sprite_piece_pos gSpriteBank00Frame380Pos[3] = { { -25, -16 }, { 7, -9 }, { 15, -4 } };
const struct sprite_piece_pos gSpriteBank00Frame381Pos[3] = { { -23, -16 }, { 9, -9 }, { 17, -5 } };
const struct sprite_piece_pos gSpriteBank00Frame382Pos[3] = { { -22, -16 }, { 10, -7 }, { 18, 3 } };
const struct sprite_piece_pos gSpriteBank00Frame383Pos[3] = { { -22, -17 }, { 10, -7 }, { 18, -5 } };
const struct sprite_piece_pos gSpriteBank00Frame384Pos[4] = { { -23, -21 }, { 9, -10 }, { -10, 11 }, { -2, 11 } };
const struct sprite_piece_pos gSpriteBank00Frame385Pos[3] = { { -22, -24 }, { 10, -10 }, { -10, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame386Pos[8] = { { -36, -65 }, { 28, -36 }, { 28, -18 }, { -20, -1 }, { 11, -1 }, { 27, -1 }, { -21, 15 }, { 13, 15 } };
const struct sprite_piece_pos gSpriteBank00Frame387Pos[6] = { { -37, -72 }, { 27, -19 }, { -34, -8 }, { 30, -4 }, { 38, -4 }, { 38, 12 } };
const struct sprite_piece_pos gSpriteBank00Frame388Pos[4] = { { -29, -72 }, { -29, -8 }, { 3, -8 }, { 35, -4 } };
const struct sprite_piece_pos gSpriteBank00Frame389Pos[4] = { { -32, -72 }, { 30, -17 }, { -34, -8 }, { 30, 2 } };
const struct sprite_piece_pos gSpriteBank00Frame390Pos[6] = { { -40, -58 }, { 24, -34 }, { 40, -18 }, { 24, -2 }, { -10, 6 }, { 22, 6 } };
const struct sprite_piece_pos gSpriteBank00Frame391Pos[3] = { { -21, -66 }, { -31, -2 }, { 1, -2 } };
const struct sprite_piece_pos gSpriteBank00Frame392Pos[5] = { { -22, -72 }, { -31, -8 }, { 1, -8 }, { -29, 8 }, { 3, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame393Pos[5] = { { -32, -72 }, { 27, -27 }, { 27, -10 }, { -37, -8 }, { 27, -8 } };
const struct sprite_piece_pos gSpriteBank00Frame394Pos[10] = { { -34, -72 }, { 26, -28 }, { 40, -15 }, { 24, -11 }, { -40, -8 }, { -8, -8 }, { 30, -8 }, { 40, -1 }, { -9, 8 }, { 7, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame395Pos[4] = { { -34, -59 }, { 30, -3 }, { -9, 5 }, { 7, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame396Pos[9] = { { -38, -70 }, { 26, -43 }, { 26, -8 }, { -31, -6 }, { 1, -6 }, { 33, 1 }, { -25, 10 }, { 7, 10 }, { 23, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame397Pos[7] = { { -37, -72 }, { 27, -39 }, { -34, -8 }, { -2, -8 }, { 30, 1 }, { -31, 8 }, { 1, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame398Pos[9] = { { -38, -71 }, { 26, -43 }, { 42, -27 }, { 26, -11 }, { -30, -7 }, { 2, -7 }, { 18, -7 }, { -9, 9 }, { 7, 9 } };
const struct sprite_piece_pos gSpriteBank00Frame399Pos[5] = { { -42, -52 }, { 22, -50 }, { -27, 12 }, { 5, 12 }, { 21, 12 } };
const struct sprite_piece_pos gSpriteBank00Frame400Pos[2] = { { -31, -46 }, { 33, -18 } };
const struct sprite_piece_pos gSpriteBank00Frame401Pos[1] = { { -32, -39 } };
const struct sprite_piece_pos gSpriteBank00Frame402Pos[2] = { { -42, -48 }, { 34, -4 } };
const struct sprite_piece_pos gSpriteBank00Frame403Pos[4] = { { -10, -24 }, { 6, -25 }, { -9, 7 }, { 7, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame404Pos[3] = { { -10, -10 }, { 6, -5 }, { -8, 6 } };
const struct sprite_piece_pos gSpriteBank00Frame405Pos[3] = { { -10, -10 }, { 6, -4 }, { -9, 6 } };
const struct sprite_piece_pos gSpriteBank00Frame406Pos[2] = { { -10, -10 }, { 6, -2 } };
const struct sprite_piece_pos gSpriteBank00Frame407Pos[3] = { { -10, -10 }, { 5, -2 }, { -11, 6 } };
const struct sprite_piece_pos gSpriteBank00Frame408Pos[3] = { { -10, -9 }, { 5, -2 }, { -11, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame409Pos[3] = { { -12, -9 }, { 4, -5 }, { -12, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame410Pos[3] = { { -13, -9 }, { 3, -5 }, { -13, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame411Pos[4] = { { -13, -9 }, { 3, -4 }, { -13, 7 }, { 5, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame412Pos[4] = { { -13, -9 }, { 3, -4 }, { -13, 7 }, { 5, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame413Pos[4] = { { -13, -9 }, { 3, -4 }, { -13, 7 }, { 5, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame414Pos[3] = { { -12, -18 }, { -5, 14 }, { 3, 17 } };
const struct sprite_piece_pos gSpriteBank00Frame415Pos[3] = { { -12, -18 }, { -5, 14 }, { 3, 17 } };
const struct sprite_piece_pos gSpriteBank00Frame416Pos[3] = { { -12, -18 }, { -5, 14 }, { 3, 17 } };
const struct sprite_piece_pos gSpriteBank00Frame417Pos[3] = { { -12, -18 }, { -5, 14 }, { 3, 17 } };
const struct sprite_piece_pos gSpriteBank00Frame418Pos[3] = { { -12, -18 }, { -5, 14 }, { 3, 17 } };
const struct sprite_piece_pos gSpriteBank00Frame419Pos[3] = { { -12, -18 }, { -5, 14 }, { 3, 17 } };
const struct sprite_piece_pos gSpriteBank00Frame420Pos[3] = { { -12, -18 }, { -5, 14 }, { 3, 17 } };
const struct sprite_piece_pos gSpriteBank00Frame421Pos[2] = { { -11, -18 }, { -4, 14 } };
const struct sprite_piece_pos gSpriteBank00Frame422Pos[2] = { { -11, -18 }, { -4, 14 } };
const struct sprite_piece_pos gSpriteBank00Frame423Pos[2] = { { -9, -17 }, { -4, 15 } };
const struct sprite_piece_pos gSpriteBank00Frame424Pos[2] = { { -7, -15 }, { -1, 17 } };
const struct sprite_piece_pos gSpriteBank00Frame425Pos[2] = { { -3, -7 }, { 29, -2 } };
const struct sprite_piece_pos gSpriteBank00Frame426Pos[3] = { { -11, 3 }, { 21, 12 }, { 29, 15 } };
const struct sprite_piece_pos gSpriteBank00Frame427Pos[3] = { { -10, 3 }, { 22, 12 }, { 30, 16 } };
const struct sprite_piece_pos gSpriteBank00Frame428Pos[3] = { { -9, 4 }, { 23, 13 }, { 24, 29 } };
const struct sprite_piece_pos gSpriteBank00Frame429Pos[3] = { { -14, -23 }, { -8, 9 }, { 8, 9 } };
const struct sprite_piece_pos gSpriteBank00Frame430Pos[3] = { { -14, -23 }, { -8, 9 }, { 8, 9 } };
const struct sprite_piece_pos gSpriteBank00Frame431Pos[3] = { { -14, -23 }, { -8, 9 }, { 8, 9 } };
const struct sprite_piece_pos gSpriteBank00Frame432Pos[3] = { { -12, -18 }, { -3, 14 }, { 5, 15 } };
const struct sprite_piece_pos gSpriteBank00Frame433Pos[3] = { { -16, -17 }, { -3, 15 }, { 5, 15 } };
const struct sprite_piece_pos gSpriteBank00Frame434Pos[4] = { { -17, -17 }, { 15, 5 }, { -3, 15 }, { 5, 15 } };
const struct sprite_piece_pos gSpriteBank00Frame435Pos[5] = { { -21, -25 }, { 11, -18 }, { 19, 4 }, { -4, 7 }, { 16, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame436Pos[2] = { { -22, -29 }, { 10, -20 } };
const struct sprite_piece_pos gSpriteBank00Frame437Pos[4] = { { -20, -25 }, { 12, -16 }, { 20, 4 }, { -3, 7 } };
const struct sprite_piece_pos gSpriteBank00Frame438Pos[1] = { { -13, -10 } };
const struct sprite_piece_pos gSpriteBank00Frame439Pos[1] = { { -4, 6 } };
const struct sprite_piece_pos gSpriteBank00Frame441Pos[2] = { { -11, -19 }, { -4, 13 } };
const struct sprite_piece_pos gSpriteBank00Frame442Pos[5] = { { -10, -19 }, { 6, -10 }, { 6, 12 }, { -10, 13 }, { 6, 13 } };
const struct sprite_piece_pos gSpriteBank00Frame443Pos[5] = { { -8, -19 }, { 6, -10 }, { 6, 12 }, { -10, 13 }, { 6, 13 } };
const struct sprite_piece_pos gSpriteBank00Frame444Pos[3] = { { -7, -19 }, { -11, 13 }, { 5, 13 } };
const struct sprite_piece_pos gSpriteBank00Frame445Pos[3] = { { -8, -19 }, { -13, 13 }, { 3, 13 } };
const struct sprite_piece_pos gSpriteBank00Frame446Pos[2] = { { -7, -18 }, { -14, 14 } };
const struct sprite_piece_pos gSpriteBank00Frame447Pos[3] = { { -11, -15 }, { -14, 17 }, { 2, 17 } };
const struct sprite_piece_pos gSpriteBank00Frame448Pos[1] = { { -14, -9 } };
const struct sprite_piece_pos gSpriteBank00Frame449Pos[3] = { { -15, -14 }, { -14, 18 }, { 2, 18 } };
const struct sprite_piece_pos gSpriteBank00Frame450Pos[2] = { { -15, -11 }, { -9, 21 } };
const struct sprite_piece_pos gSpriteBank00Frame451Pos[1] = { { -15, -10 } };
const struct sprite_piece_pos gSpriteBank00Frame452Pos[1] = { { -14, -7 } };
const struct sprite_piece_pos gSpriteBank00Frame453Pos[1] = { { -14, -4 } };
const struct sprite_piece_pos gSpriteBank00Frame454Pos[1] = { { -14, 8 } };
const struct sprite_piece_pos gSpriteBank00Frame455Pos[3] = { { -10, -21 }, { 6, -10 }, { -6, 11 } };
const struct sprite_piece_pos gSpriteBank00Frame456Pos[3] = { { -8, -18 }, { 8, -7 }, { -6, 14 } };
const struct sprite_piece_pos gSpriteBank00Frame457Pos[4] = { { -8, -15 }, { 8, -4 }, { -6, 17 }, { 2, 17 } };
const struct sprite_piece_pos gSpriteBank00Frame458Pos[2] = { { -18, -17 }, { -6, 15 } };
const struct sprite_piece_pos gSpriteBank00Frame459Pos[3] = { { -19, -23 }, { 13, -23 }, { -19, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame460Pos[3] = { { -23, -27 }, { 9, -22 }, { 9, 10 } };
const struct sprite_piece_pos gSpriteBank00Frame461Pos[4] = { { -5, -16 }, { 17, -12 }, { -15, 16 }, { 17, 16 } };
const struct sprite_piece_pos gSpriteBank00Frame462Pos[2] = { { -15, -4 }, { 17, 12 } };
const struct sprite_piece_pos gSpriteBank00Frame463Pos[3] = { { -15, -7 }, { 17, 15 }, { 25, 15 } };
const struct sprite_piece_pos gSpriteBank00Frame464Pos[4] = { { -13, 4 }, { 17, 13 }, { -15, 20 }, { 18, 20 } };

const u8 gSpriteBank00Frame000Pieces[4] = { SPRITE_PIECE(6, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame001Pieces[4] = { SPRITE_PIECE(6, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame002Pieces[2] = { SPRITE_PIECE(6, 6), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank00Frame003Pieces[3] = { SPRITE_PIECE(6, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank00Frame004Pieces[3] = { SPRITE_PIECE(6, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank00Frame005Pieces[4] = { SPRITE_PIECE(6, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame006Pieces[2] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank00Frame007Pieces[2] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank00Frame008Pieces[4] = { SPRITE_PIECE(6, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame009Pieces[3] = { SPRITE_PIECE(6, 6), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame010Pieces[3] = { SPRITE_PIECE(6, 6), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame011Pieces[3] = { SPRITE_PIECE(6, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank00Frame012Pieces[4] = { SPRITE_PIECE(6, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame013Pieces[1] = { SPRITE_PIECE(6, 2) };
const u8 gSpriteBank00Frame014Pieces[1] = { SPRITE_PIECE(6, 2) };
const u8 gSpriteBank00Frame015Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame016Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame017Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame018Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame019Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame020Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame021Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame022Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame023Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame024Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame025Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame026Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame027Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame028Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame029Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame030Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame031Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame032Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame033Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame034Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame035Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame036Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame037Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame038Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame039Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame040Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame041Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame042Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame043Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame044Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame045Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame046Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame047Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame048Pieces[1] = { SPRITE_PIECE(6, 11) };
const u8 gSpriteBank00Frame049Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank00Frame050Pieces[2] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame051Pieces[1] = { SPRITE_PIECE(6, 2) };
const u8 gSpriteBank00Frame052Pieces[4] = { SPRITE_PIECE(6, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame053Pieces[3] = { SPRITE_PIECE(6, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank00Frame054Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank00Frame055Pieces[2] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame056Pieces[3] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame057Pieces[3] = { SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank00Frame058Pieces[3] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame059Pieces[2] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame060Pieces[5] = { SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame061Pieces[2] = { SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank00Frame062Pieces[4] = { SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame063Pieces[4] = { SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame064Pieces[1] = { SPRITE_PIECE(0, 2) };
const u8 gSpriteBank00Frame065Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame066Pieces[4] = { SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame067Pieces[2] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank00Frame068Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame069Pieces[4] = { SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame070Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame071Pieces[4] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank00Frame072Pieces[3] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame073Pieces[2] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank00Frame074Pieces[4] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame075Pieces[4] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame076Pieces[3] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame077Pieces[5] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame078Pieces[3] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame079Pieces[3] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame080Pieces[3] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame081Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame082Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame083Pieces[4] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank00Frame084Pieces[3] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame085Pieces[2] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank00Frame086Pieces[4] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame087Pieces[4] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame088Pieces[3] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame089Pieces[5] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame090Pieces[3] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame091Pieces[3] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame092Pieces[1] = { SPRITE_PIECE(0, 11) };
const u8 gSpriteBank00Frame093Pieces[1] = { SPRITE_PIECE(0, 11) };
const u8 gSpriteBank00Frame094Pieces[3] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame095Pieces[3] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame096Pieces[3] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame097Pieces[3] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame098Pieces[3] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame099Pieces[3] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame100Pieces[3] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame101Pieces[3] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame102Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame103Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame104Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame105Pieces[4] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank00Frame106Pieces[4] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame107Pieces[4] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame108Pieces[5] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame109Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame110Pieces[2] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank00Frame111Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame112Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame113Pieces[2] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame114Pieces[5] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame115Pieces[4] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank00Frame116Pieces[5] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame117Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame118Pieces[4] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame119Pieces[2] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame120Pieces[2] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame121Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame122Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame123Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame124Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame125Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame126Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame127Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame128Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame129Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame130Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame131Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame132Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame133Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame134Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame135Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame136Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame137Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank00Frame138Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank00Frame139Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame140Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame141Pieces[4] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame142Pieces[4] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame143Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame144Pieces[2] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank00Frame145Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame146Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame147Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame148Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame149Pieces[4] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame150Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame151Pieces[1] = { SPRITE_PIECE(0, 7) };
const u8 gSpriteBank00Frame152Pieces[1] = { SPRITE_PIECE(0, 7) };
const u8 gSpriteBank00Frame153Pieces[1] = { SPRITE_PIECE(6, 7) };
const u8 gSpriteBank00Frame154Pieces[3] = { SPRITE_PIECE(6, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame155Pieces[3] = { SPRITE_PIECE(6, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame156Pieces[5] = { SPRITE_PIECE(6, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame157Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank00Frame158Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame159Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame160Pieces[2] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank00Frame161Pieces[2] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank00Frame162Pieces[1] = { SPRITE_PIECE(6, 2) };
const u8 gSpriteBank00Frame163Pieces[2] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame164Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame165Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame166Pieces[5] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame167Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame168Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame169Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame170Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame171Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame172Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame173Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame174Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame175Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame176Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame177Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame178Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame179Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame180Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame181Pieces[5] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank00Frame182Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank00Frame183Pieces[1] = { SPRITE_PIECE(0, 11) };
const u8 gSpriteBank00Frame184Pieces[1] = { SPRITE_PIECE(6, 2) };
const u8 gSpriteBank00Frame185Pieces[1] = { SPRITE_PIECE(6, 2) };
const u8 gSpriteBank00Frame186Pieces[4] = { SPRITE_PIECE(6, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame187Pieces[4] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame188Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame189Pieces[1] = { SPRITE_PIECE(0, 11) };
const u8 gSpriteBank00Frame190Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame191Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame192Pieces[1] = { SPRITE_PIECE(6, 2) };
const u8 gSpriteBank00Frame193Pieces[1] = { SPRITE_PIECE(6, 2) };
const u8 gSpriteBank00Frame194Pieces[1] = { SPRITE_PIECE(6, 2) };
const u8 gSpriteBank00Frame195Pieces[1] = { SPRITE_PIECE(6, 2) };
const u8 gSpriteBank00Frame196Pieces[3] = { SPRITE_PIECE(6, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame197Pieces[1] = { SPRITE_PIECE(6, 2) };
const u8 gSpriteBank00Frame198Pieces[2] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame199Pieces[2] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank00Frame200Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame201Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame202Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame203Pieces[4] = { SPRITE_PIECE(4, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame204Pieces[4] = { SPRITE_PIECE(4, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame205Pieces[3] = { SPRITE_PIECE(4, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame206Pieces[2] = { SPRITE_PIECE(4, 2), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank00Frame207Pieces[3] = { SPRITE_PIECE(4, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame208Pieces[3] = { SPRITE_PIECE(4, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame209Pieces[3] = { SPRITE_PIECE(4, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame210Pieces[3] = { SPRITE_PIECE(4, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame211Pieces[4] = { SPRITE_PIECE(4, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame212Pieces[3] = { SPRITE_PIECE(4, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame213Pieces[1] = { SPRITE_PIECE(6, 2) };
const u8 gSpriteBank00Frame214Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame215Pieces[2] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame216Pieces[1] = { SPRITE_PIECE(6, 2) };
const u8 gSpriteBank00Frame217Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame218Pieces[3] = { SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame219Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame220Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame221Pieces[4] = { SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame222Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame223Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame224Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame225Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame226Pieces[5] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame227Pieces[5] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame228Pieces[5] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame229Pieces[5] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame230Pieces[5] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame231Pieces[5] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame232Pieces[5] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame233Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame234Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank00Frame235Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank00Frame236Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame237Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame238Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame239Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame240Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame241Pieces[5] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame242Pieces[3] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame243Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame244Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame245Pieces[5] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame246Pieces[5] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame247Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame248Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame249Pieces[5] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame250Pieces[5] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame251Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame252Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame253Pieces[5] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame254Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame255Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame256Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame257Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame258Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame259Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame260Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame261Pieces[3] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame262Pieces[3] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame263Pieces[3] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame264Pieces[3] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame265Pieces[3] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame266Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame267Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame268Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame269Pieces[5] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame270Pieces[5] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame271Pieces[4] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame272Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame273Pieces[1] = { SPRITE_PIECE(0, 11) };
const u8 gSpriteBank00Frame274Pieces[5] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame275Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame276Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame277Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame278Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame279Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame280Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame281Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame282Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame283Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame284Pieces[1] = { SPRITE_PIECE(0, 7) };
const u8 gSpriteBank00Frame285Pieces[2] = { SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame286Pieces[2] = { SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame287Pieces[1] = { SPRITE_PIECE(0, 7) };
const u8 gSpriteBank00Frame288Pieces[2] = { SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame289Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame290Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame291Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame292Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame293Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame294Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame295Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame296Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame297Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame298Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame299Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame300Pieces[2] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame301Pieces[2] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame302Pieces[2] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame303Pieces[2] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame304Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame305Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame306Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame307Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame308Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank00Frame309Pieces[2] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame310Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame311Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame312Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame313Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame314Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame315Pieces[2] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame316Pieces[3] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame317Pieces[1] = { SPRITE_PIECE(6, 11) };
const u8 gSpriteBank00Frame318Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank00Frame319Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame320Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame321Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame322Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame323Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame324Pieces[3] = { SPRITE_PIECE(6, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame325Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame326Pieces[4] = { SPRITE_PIECE(6, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame327Pieces[3] = { SPRITE_PIECE(1, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame328Pieces[4] = { SPRITE_PIECE(1, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame329Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame330Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank00Frame331Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame332Pieces[5] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame333Pieces[9] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame334Pieces[7] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank00Frame335Pieces[9] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame336Pieces[4] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame337Pieces[10] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame338Pieces[5] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame339Pieces[5] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank00Frame340Pieces[3] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank00Frame341Pieces[6] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame342Pieces[4] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame343Pieces[4] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame344Pieces[6] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame345Pieces[8] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame346Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame347Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame348Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame349Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame350Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame351Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame352Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame353Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame354Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame355Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame356Pieces[5] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame357Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame358Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame359Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame360Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame361Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame362Pieces[3] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame363Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame364Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame365Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame366Pieces[4] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame367Pieces[4] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame368Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame369Pieces[2] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank00Frame370Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame371Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame372Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame373Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame374Pieces[4] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame375Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame376Pieces[4] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame377Pieces[4] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame378Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame379Pieces[2] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank00Frame380Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame381Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame382Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame383Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame384Pieces[4] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame385Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame386Pieces[8] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame387Pieces[6] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame388Pieces[4] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame389Pieces[4] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame390Pieces[6] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame391Pieces[3] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank00Frame392Pieces[5] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank00Frame393Pieces[5] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame394Pieces[10] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame395Pieces[4] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame396Pieces[9] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame397Pieces[7] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank00Frame398Pieces[9] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame399Pieces[5] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame400Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame401Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank00Frame402Pieces[2] = { SPRITE_PIECE(1, 3), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame403Pieces[4] = { SPRITE_PIECE(1, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame404Pieces[3] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame405Pieces[3] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame406Pieces[2] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame407Pieces[3] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame408Pieces[3] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame409Pieces[3] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame410Pieces[3] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame411Pieces[4] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame412Pieces[4] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame413Pieces[4] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame414Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame415Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame416Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame417Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame418Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame419Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame420Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame421Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame422Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame423Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame424Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame425Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank00Frame426Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank00Frame427Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank00Frame428Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame429Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame430Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame431Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame432Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame433Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame434Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame435Pieces[5] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame436Pieces[2] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 10) };
const u8 gSpriteBank00Frame437Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame438Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank00Frame439Pieces[1] = { SPRITE_PIECE(2, 8) };
const u8 gSpriteBank00Frame441Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame442Pieces[5] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame443Pieces[5] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame444Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame445Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame446Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank00Frame447Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame448Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank00Frame449Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame450Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame451Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank00Frame452Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank00Frame453Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank00Frame454Pieces[1] = { SPRITE_PIECE(2, 6) };
const u8 gSpriteBank00Frame455Pieces[3] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame456Pieces[3] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank00Frame457Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame458Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank00Frame459Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank00Frame460Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame461Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame462Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank00Frame463Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank00Frame464Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };

/* ---------------------------------------------------------------------- */
/* Bank 1: 47 animations, 274 frames, tiles in gSpriteBank01Tiles (SPRITE_TILES_BANK01). */

extern const u16 gSpriteBank01Anim00Seq[2];
extern const u16 gSpriteBank01Anim01Seq[4];
extern const u16 gSpriteBank01Anim02Seq[3];
extern const u16 gSpriteBank01Anim03Seq[4];
extern const u16 gSpriteBank01Anim04Seq[2];
extern const u16 gSpriteBank01Anim05Seq[4];
extern const u16 gSpriteBank01Anim06Seq[2];
extern const u16 gSpriteBank01Anim07Seq[8];
extern const u16 gSpriteBank01Anim08Seq[4];
extern const u16 gSpriteBank01Anim09Seq[3];
extern const u16 gSpriteBank01Anim10Seq[4];
extern const u16 gSpriteBank01Anim11Seq[2];
extern const u16 gSpriteBank01Anim12Seq[4];
extern const u16 gSpriteBank01Anim13Seq[2];
extern const u16 gSpriteBank01Anim14Seq[8];
extern const u16 gSpriteBank01Anim15Seq[8];
extern const u16 gSpriteBank01Anim16Seq[8];
extern const u16 gSpriteBank01Anim17Seq[8];
extern const u16 gSpriteBank01Anim18Seq[8];
extern const u16 gSpriteBank01Anim19Seq[4];
extern const u16 gSpriteBank01Anim20Seq[4];
extern const u16 gSpriteBank01Anim21Seq[2];
extern const u16 gSpriteBank01Anim22Seq[2];
extern const u16 gSpriteBank01Anim23Seq[8];
extern const u16 gSpriteBank01Anim24Seq[4];
extern const u16 gSpriteBank01Anim25Seq[6];
extern const u16 gSpriteBank01Anim26Seq[6];
extern const u16 gSpriteBank01Anim27Seq[6];
extern const u16 gSpriteBank01Anim28Seq[6];
extern const u16 gSpriteBank01Anim29Seq[3];
extern const u16 gSpriteBank01Anim30Seq[5];
extern const u16 gSpriteBank01Anim31Seq[15];
extern const u16 gSpriteBank01Anim32Seq[8];
extern const u16 gSpriteBank01Anim33Seq[8];
extern const u16 gSpriteBank01Anim34Seq[8];
extern const u16 gSpriteBank01Anim35Seq[8];
extern const u16 gSpriteBank01Anim36Seq[8];
extern const u16 gSpriteBank01Anim37Seq[8];
extern const u16 gSpriteBank01Anim38Seq[4];
extern const u16 gSpriteBank01Anim39Seq[4];
extern const u16 gSpriteBank01Anim40Seq[2];
extern const u16 gSpriteBank01Anim41Seq[8];
extern const u16 gSpriteBank01Anim42Seq[1];
extern const u16 gSpriteBank01Anim43Seq[15];
extern const u16 gSpriteBank01Anim44Seq[15];
extern const u16 gSpriteBank01Anim45Seq[9];
extern const u16 gSpriteBank01Anim46Seq[21];
extern const struct sprite_frame_1box gSpriteBank01Frame000;
extern const struct sprite_frame_1box gSpriteBank01Frame001;
extern const struct sprite_frame_1box gSpriteBank01Frame002;
extern const struct sprite_frame_1box gSpriteBank01Frame003;
extern const struct sprite_frame_1box gSpriteBank01Frame004;
extern const struct sprite_frame_1box gSpriteBank01Frame005;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame006;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame007;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame008;
extern const struct sprite_frame_1box gSpriteBank01Frame009;
extern const struct sprite_frame_1box gSpriteBank01Frame010;
extern const struct sprite_frame_1box gSpriteBank01Frame011;
extern const struct sprite_frame_1box gSpriteBank01Frame012;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame013;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame014;
extern const struct sprite_frame_1box gSpriteBank01Frame015;
extern const struct sprite_frame_1box gSpriteBank01Frame016;
extern const struct sprite_frame_1box gSpriteBank01Frame017;
extern const struct sprite_frame_1box gSpriteBank01Frame018;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame019;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame020;
extern const struct sprite_frame_1box gSpriteBank01Frame021;
extern const struct sprite_frame_1box gSpriteBank01Frame022;
extern const struct sprite_frame_1box gSpriteBank01Frame023;
extern const struct sprite_frame_1box gSpriteBank01Frame024;
extern const struct sprite_frame_1box gSpriteBank01Frame025;
extern const struct sprite_frame_1box gSpriteBank01Frame026;
extern const struct sprite_frame_1box gSpriteBank01Frame027;
extern const struct sprite_frame_1box gSpriteBank01Frame028;
extern const struct sprite_frame_1box gSpriteBank01Frame029;
extern const struct sprite_frame_1box gSpriteBank01Frame030;
extern const struct sprite_frame_1box gSpriteBank01Frame031;
extern const struct sprite_frame_1box gSpriteBank01Frame032;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame033;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame034;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame035;
extern const struct sprite_frame_1box gSpriteBank01Frame036;
extern const struct sprite_frame_1box gSpriteBank01Frame037;
extern const struct sprite_frame_1box gSpriteBank01Frame038;
extern const struct sprite_frame_1box gSpriteBank01Frame039;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame040;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame041;
extern const struct sprite_frame_1box gSpriteBank01Frame042;
extern const struct sprite_frame_1box gSpriteBank01Frame043;
extern const struct sprite_frame_1box gSpriteBank01Frame044;
extern const struct sprite_frame_1box gSpriteBank01Frame045;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame046;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame047;
extern const struct sprite_frame_1box gSpriteBank01Frame048;
extern const struct sprite_frame_1box gSpriteBank01Frame049;
extern const struct sprite_frame_1box gSpriteBank01Frame050;
extern const struct sprite_frame_1box gSpriteBank01Frame051;
extern const struct sprite_frame_1box gSpriteBank01Frame052;
extern const struct sprite_frame_1box gSpriteBank01Frame053;
extern const struct sprite_frame_1box gSpriteBank01Frame054;
extern const struct sprite_frame_1box gSpriteBank01Frame055;
extern const struct sprite_frame_1box gSpriteBank01Frame056;
extern const struct sprite_frame_1box gSpriteBank01Frame057;
extern const struct sprite_frame_1box gSpriteBank01Frame058;
extern const struct sprite_frame_1box gSpriteBank01Frame059;
extern const struct sprite_frame_1box gSpriteBank01Frame060;
extern const struct sprite_frame_1box gSpriteBank01Frame061;
extern const struct sprite_frame_1box gSpriteBank01Frame062;
extern const struct sprite_frame_1box gSpriteBank01Frame063;
extern const struct sprite_frame_1box gSpriteBank01Frame064;
extern const struct sprite_frame_1box gSpriteBank01Frame065;
extern const struct sprite_frame_1box gSpriteBank01Frame066;
extern const struct sprite_frame_1box gSpriteBank01Frame067;
extern const struct sprite_frame_1box gSpriteBank01Frame068;
extern const struct sprite_frame_1box gSpriteBank01Frame069;
extern const struct sprite_frame_1box gSpriteBank01Frame070;
extern const struct sprite_frame_1box gSpriteBank01Frame071;
extern const struct sprite_frame_1box gSpriteBank01Frame072;
extern const struct sprite_frame_1box gSpriteBank01Frame073;
extern const struct sprite_frame_1box gSpriteBank01Frame074;
extern const struct sprite_frame_1box gSpriteBank01Frame075;
extern const struct sprite_frame_1box gSpriteBank01Frame076;
extern const struct sprite_frame_1box gSpriteBank01Frame077;
extern const struct sprite_frame_1box gSpriteBank01Frame078;
extern const struct sprite_frame_1box gSpriteBank01Frame079;
extern const struct sprite_frame_1box gSpriteBank01Frame080;
extern const struct sprite_frame_1box gSpriteBank01Frame081;
extern const struct sprite_frame_1box gSpriteBank01Frame082;
extern const struct sprite_frame_1box gSpriteBank01Frame083;
extern const struct sprite_frame_1box gSpriteBank01Frame084;
extern const struct sprite_frame_1box gSpriteBank01Frame085;
extern const struct sprite_frame_1box gSpriteBank01Frame086;
extern const struct sprite_frame_1box gSpriteBank01Frame087;
extern const struct sprite_frame_1box gSpriteBank01Frame088;
extern const struct sprite_frame_1box gSpriteBank01Frame089;
extern const struct sprite_frame_1box gSpriteBank01Frame090;
extern const struct sprite_frame_1box gSpriteBank01Frame091;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame092;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame093;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame094;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame095;
extern const struct sprite_frame_1box gSpriteBank01Frame096;
extern const struct sprite_frame_1box gSpriteBank01Frame097;
extern const struct sprite_frame_1box gSpriteBank01Frame098;
extern const struct sprite_frame_1box gSpriteBank01Frame099;
extern const struct sprite_frame_1box gSpriteBank01Frame100;
extern const struct sprite_frame_1box gSpriteBank01Frame101;
extern const struct sprite_frame_1box gSpriteBank01Frame102;
extern const struct sprite_frame_1box gSpriteBank01Frame103;
extern const struct sprite_frame_1box gSpriteBank01Frame104;
extern const struct sprite_frame_1box gSpriteBank01Frame105;
extern const struct sprite_frame_1box gSpriteBank01Frame106;
extern const struct sprite_frame_1box gSpriteBank01Frame107;
extern const struct sprite_frame_1box gSpriteBank01Frame108;
extern const struct sprite_frame_1box gSpriteBank01Frame109;
extern const struct sprite_frame_1box gSpriteBank01Frame110;
extern const struct sprite_frame_1box gSpriteBank01Frame111;
extern const struct sprite_frame_1box gSpriteBank01Frame112;
extern const struct sprite_frame_1box gSpriteBank01Frame113;
extern const struct sprite_frame_1box gSpriteBank01Frame114;
extern const struct sprite_frame_1box gSpriteBank01Frame115;
extern const struct sprite_frame_1box gSpriteBank01Frame116;
extern const struct sprite_frame_1box gSpriteBank01Frame117;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame118;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame119;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame120;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame121;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame122;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame123;
extern const struct sprite_frame_1box gSpriteBank01Frame124;
extern const struct sprite_frame_1box gSpriteBank01Frame125;
extern const struct sprite_frame_1box gSpriteBank01Frame126;
extern const struct sprite_frame_1box gSpriteBank01Frame127;
extern const struct sprite_frame_1box gSpriteBank01Frame128;
extern const struct sprite_frame_1box gSpriteBank01Frame129;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame130;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame131;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame132;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame133;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame134;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame135;
extern const struct sprite_frame_1box gSpriteBank01Frame136;
extern const struct sprite_frame_1box gSpriteBank01Frame137;
extern const struct sprite_frame_1box gSpriteBank01Frame138;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame139;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame140;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame141;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame142;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame143;
extern const struct sprite_frame_1box gSpriteBank01Frame144;
extern const struct sprite_frame_1box gSpriteBank01Frame145;
extern const struct sprite_frame_1box gSpriteBank01Frame146;
extern const struct sprite_frame_1box gSpriteBank01Frame147;
extern const struct sprite_frame_1box gSpriteBank01Frame148;
extern const struct sprite_frame_1box gSpriteBank01Frame149;
extern const struct sprite_frame_1box gSpriteBank01Frame150;
extern const struct sprite_frame_1box gSpriteBank01Frame151;
extern const struct sprite_frame_1box gSpriteBank01Frame152;
extern const struct sprite_frame_1box gSpriteBank01Frame153;
extern const struct sprite_frame_1box gSpriteBank01Frame154;
extern const struct sprite_frame_1box gSpriteBank01Frame155;
extern const struct sprite_frame_1box gSpriteBank01Frame156;
extern const struct sprite_frame_1box gSpriteBank01Frame157;
extern const struct sprite_frame_1box gSpriteBank01Frame158;
extern const struct sprite_frame_1box gSpriteBank01Frame159;
extern const struct sprite_frame_1box gSpriteBank01Frame160;
extern const struct sprite_frame_1box gSpriteBank01Frame161;
extern const struct sprite_frame_1box gSpriteBank01Frame162;
extern const struct sprite_frame_1box gSpriteBank01Frame163;
extern const struct sprite_frame_1box gSpriteBank01Frame164;
extern const struct sprite_frame_1box gSpriteBank01Frame165;
extern const struct sprite_frame_1box gSpriteBank01Frame166;
extern const struct sprite_frame_1box gSpriteBank01Frame167;
extern const struct sprite_frame_1box gSpriteBank01Frame168;
extern const struct sprite_frame_1box gSpriteBank01Frame169;
extern const struct sprite_frame_1box gSpriteBank01Frame170;
extern const struct sprite_frame_1box gSpriteBank01Frame171;
extern const struct sprite_frame_1box gSpriteBank01Frame172;
extern const struct sprite_frame_1box gSpriteBank01Frame173;
extern const struct sprite_frame_1box gSpriteBank01Frame174;
extern const struct sprite_frame_1box gSpriteBank01Frame175;
extern const struct sprite_frame_1box gSpriteBank01Frame176;
extern const struct sprite_frame_1box gSpriteBank01Frame177;
extern const struct sprite_frame_1box gSpriteBank01Frame178;
extern const struct sprite_frame_1box gSpriteBank01Frame179;
extern const struct sprite_frame_1box gSpriteBank01Frame180;
extern const struct sprite_frame_1box gSpriteBank01Frame181;
extern const struct sprite_frame_1box gSpriteBank01Frame182;
extern const struct sprite_frame_1box gSpriteBank01Frame183;
extern const struct sprite_frame_1box gSpriteBank01Frame184;
extern const struct sprite_frame_1box gSpriteBank01Frame185;
extern const struct sprite_frame_1box gSpriteBank01Frame186;
extern const struct sprite_frame_1box gSpriteBank01Frame187;
extern const struct sprite_frame_1box gSpriteBank01Frame188;
extern const struct sprite_frame_1box gSpriteBank01Frame189;
extern const struct sprite_frame_1box gSpriteBank01Frame190;
extern const struct sprite_frame_1box gSpriteBank01Frame191;
extern const struct sprite_frame_1box gSpriteBank01Frame192;
extern const struct sprite_frame_1box gSpriteBank01Frame193;
extern const struct sprite_frame_1box gSpriteBank01Frame194;
extern const struct sprite_frame_1box gSpriteBank01Frame195;
extern const struct sprite_frame_1box gSpriteBank01Frame196;
extern const struct sprite_frame_1box gSpriteBank01Frame197;
extern const struct sprite_frame_1box gSpriteBank01Frame198;
extern const struct sprite_frame_1box gSpriteBank01Frame199;
extern const struct sprite_frame_1box gSpriteBank01Frame200;
extern const struct sprite_frame_1box gSpriteBank01Frame201;
extern const struct sprite_frame_1box gSpriteBank01Frame202;
extern const struct sprite_frame_1box gSpriteBank01Frame203;
extern const struct sprite_frame_1box gSpriteBank01Frame204;
extern const struct sprite_frame_1box gSpriteBank01Frame205;
extern const struct sprite_frame_1box gSpriteBank01Frame206;
extern const struct sprite_frame_1box gSpriteBank01Frame207;
extern const struct sprite_frame_1box gSpriteBank01Frame208;
extern const struct sprite_frame_1box gSpriteBank01Frame209;
extern const struct sprite_frame_1box gSpriteBank01Frame210;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame211;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame212;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame213;
extern const struct sprite_frame_3box_anchor gSpriteBank01Frame214;
extern const struct sprite_frame_1box gSpriteBank01Frame215;
extern const struct sprite_frame_1box gSpriteBank01Frame216;
extern const struct sprite_frame_1box gSpriteBank01Frame217;
extern const struct sprite_frame_1box gSpriteBank01Frame218;
extern const struct sprite_frame_1box gSpriteBank01Frame219;
extern const struct sprite_frame_1box gSpriteBank01Frame220;
extern const struct sprite_frame_1box gSpriteBank01Frame221;
extern const struct sprite_frame_1box gSpriteBank01Frame222;
extern const struct sprite_frame_1box gSpriteBank01Frame223;
extern const struct sprite_frame_1box gSpriteBank01Frame224;
extern const struct sprite_frame_1box gSpriteBank01Frame225;
extern const struct sprite_frame gSpriteBank01Frame226;
extern const struct sprite_frame gSpriteBank01Frame227;
extern const struct sprite_frame gSpriteBank01Frame228;
extern const struct sprite_frame gSpriteBank01Frame229;
extern const struct sprite_frame gSpriteBank01Frame230;
extern const struct sprite_frame gSpriteBank01Frame231;
extern const struct sprite_frame gSpriteBank01Frame232;
extern const struct sprite_frame gSpriteBank01Frame233;
extern const struct sprite_frame gSpriteBank01Frame234;
extern const struct sprite_frame gSpriteBank01Frame235;
extern const struct sprite_frame gSpriteBank01Frame236;
extern const struct sprite_frame gSpriteBank01Frame237;
extern const struct sprite_frame gSpriteBank01Frame238;
extern const struct sprite_frame gSpriteBank01Frame239;
extern const struct sprite_frame gSpriteBank01Frame240;
extern const struct sprite_frame gSpriteBank01Frame241;
extern const struct sprite_frame gSpriteBank01Frame242;
extern const struct sprite_frame gSpriteBank01Frame243;
extern const struct sprite_frame gSpriteBank01Frame244;
extern const struct sprite_frame gSpriteBank01Frame245;
extern const struct sprite_frame gSpriteBank01Frame246;
extern const struct sprite_frame gSpriteBank01Frame247;
extern const struct sprite_frame gSpriteBank01Frame248;
extern const struct sprite_frame gSpriteBank01Frame249;
extern const struct sprite_frame gSpriteBank01Frame250;
extern const struct sprite_frame gSpriteBank01Frame251;
extern const struct sprite_frame gSpriteBank01Frame252;
extern const struct sprite_frame_1box gSpriteBank01Frame253;
extern const struct sprite_frame_1box gSpriteBank01Frame254;
extern const struct sprite_frame_1box gSpriteBank01Frame255;
extern const struct sprite_frame_1box gSpriteBank01Frame256;
extern const struct sprite_frame_1box gSpriteBank01Frame257;
extern const struct sprite_frame_1box gSpriteBank01Frame258;
extern const struct sprite_frame_1box gSpriteBank01Frame259;
extern const struct sprite_frame_1box gSpriteBank01Frame260;
extern const struct sprite_frame_1box gSpriteBank01Frame261;
extern const struct sprite_frame_1box gSpriteBank01Frame262;
extern const struct sprite_frame_1box gSpriteBank01Frame263;
extern const struct sprite_frame_1box gSpriteBank01Frame264;
extern const struct sprite_frame_1box gSpriteBank01Frame265;
extern const struct sprite_frame_1box gSpriteBank01Frame266;
extern const struct sprite_frame_1box gSpriteBank01Frame267;
extern const struct sprite_frame_1box gSpriteBank01Frame268;
extern const struct sprite_frame_1box gSpriteBank01Frame269;
extern const struct sprite_frame_1box gSpriteBank01Frame270;
extern const struct sprite_frame_1box gSpriteBank01Frame271;
extern const struct sprite_frame_1box gSpriteBank01Frame272;
extern const struct sprite_frame_1box gSpriteBank01Frame273;
extern const struct sprite_piece_pos gSpriteBank01Frame000Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame001Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame002Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame003Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame004Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame005Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame006Pos[5];
extern const struct sprite_piece_pos gSpriteBank01Frame007Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame008Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame009Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame010Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame011Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame012Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame013Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame014Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame015Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame016Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame017Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame018Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame019Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame020Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame021Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame022Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame023Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame024Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame025Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame026Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame027Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame028Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame029Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame030Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame031Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame032Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame033Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame034Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame035Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame036Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame037Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame038Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame039Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame040Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame041Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame042Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame043Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame044Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame045Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame046Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame047Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame048Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame049Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame050Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame051Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame052Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame053Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame054Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame055Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame056Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame057Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame058Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame059Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame060Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame061Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame062Pos[5];
extern const struct sprite_piece_pos gSpriteBank01Frame063Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame064Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame065Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame066Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame067Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame068Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame069Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame070Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame071Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame072Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame073Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame074Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame075Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame076Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame077Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame078Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame079Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame080Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame081Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame082Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame083Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame084Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame085Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame086Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame087Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame088Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame089Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame090Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame091Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame092Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame093Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame094Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame095Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame096Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame097Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame098Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame099Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame100Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame101Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame102Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame103Pos[5];
extern const struct sprite_piece_pos gSpriteBank01Frame104Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame105Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame106Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame107Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame108Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame109Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame110Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame111Pos[5];
extern const struct sprite_piece_pos gSpriteBank01Frame112Pos[5];
extern const struct sprite_piece_pos gSpriteBank01Frame113Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame114Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame115Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame116Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame117Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame118Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame119Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame120Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame121Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame122Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame123Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame124Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame125Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame126Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame127Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame128Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame129Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame130Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame131Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame132Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame133Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame134Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame135Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame136Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame137Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame138Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame139Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame140Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame141Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame142Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame143Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame144Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame145Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame146Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame147Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame148Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame149Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame150Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame151Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame152Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame153Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame154Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame155Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame156Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame157Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame158Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame159Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame160Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame161Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame162Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame163Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame164Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame165Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame166Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame167Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame168Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame169Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame170Pos[5];
extern const struct sprite_piece_pos gSpriteBank01Frame171Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame172Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame173Pos[5];
extern const struct sprite_piece_pos gSpriteBank01Frame174Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame175Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame176Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame177Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame178Pos[5];
extern const struct sprite_piece_pos gSpriteBank01Frame179Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame180Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame181Pos[5];
extern const struct sprite_piece_pos gSpriteBank01Frame182Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame183Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame184Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame185Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame186Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame187Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame188Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame189Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame190Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame191Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame192Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame193Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame194Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame195Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame196Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame197Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame198Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame199Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame200Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame201Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame202Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame203Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame204Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame205Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame206Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame207Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame208Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame209Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame210Pos[5];
extern const struct sprite_piece_pos gSpriteBank01Frame211Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame212Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame213Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame214Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame215Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame216Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame217Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame218Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame219Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame220Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame221Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame222Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame223Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame224Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame225Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame226Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame227Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame228Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame229Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame230Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame231Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame232Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame233Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame234Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame235Pos[5];
extern const struct sprite_piece_pos gSpriteBank01Frame236Pos[5];
extern const struct sprite_piece_pos gSpriteBank01Frame237Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame238Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame239Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame240Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame241Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame242Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame243Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame244Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame245Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame246Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame247Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame248Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame249Pos[1];
extern const struct sprite_piece_pos gSpriteBank01Frame250Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame251Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame252Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame253Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame254Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame255Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame256Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame257Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame258Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame259Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame260Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame261Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame262Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame263Pos[2];
extern const struct sprite_piece_pos gSpriteBank01Frame264Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame265Pos[5];
extern const struct sprite_piece_pos gSpriteBank01Frame266Pos[5];
extern const struct sprite_piece_pos gSpriteBank01Frame267Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame268Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame269Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame270Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame271Pos[3];
extern const struct sprite_piece_pos gSpriteBank01Frame272Pos[4];
extern const struct sprite_piece_pos gSpriteBank01Frame273Pos[4];
extern const u8 gSpriteBank01Frame000Pieces[3];
extern const u8 gSpriteBank01Frame001Pieces[4];
extern const u8 gSpriteBank01Frame002Pieces[3];
extern const u8 gSpriteBank01Frame003Pieces[3];
extern const u8 gSpriteBank01Frame004Pieces[3];
extern const u8 gSpriteBank01Frame005Pieces[3];
extern const u8 gSpriteBank01Frame006Pieces[5];
extern const u8 gSpriteBank01Frame007Pieces[4];
extern const u8 gSpriteBank01Frame008Pieces[4];
extern const u8 gSpriteBank01Frame009Pieces[3];
extern const u8 gSpriteBank01Frame010Pieces[3];
extern const u8 gSpriteBank01Frame011Pieces[3];
extern const u8 gSpriteBank01Frame012Pieces[2];
extern const u8 gSpriteBank01Frame013Pieces[1];
extern const u8 gSpriteBank01Frame014Pieces[3];
extern const u8 gSpriteBank01Frame015Pieces[3];
extern const u8 gSpriteBank01Frame016Pieces[2];
extern const u8 gSpriteBank01Frame017Pieces[3];
extern const u8 gSpriteBank01Frame018Pieces[2];
extern const u8 gSpriteBank01Frame019Pieces[3];
extern const u8 gSpriteBank01Frame020Pieces[3];
extern const u8 gSpriteBank01Frame021Pieces[4];
extern const u8 gSpriteBank01Frame022Pieces[4];
extern const u8 gSpriteBank01Frame023Pieces[4];
extern const u8 gSpriteBank01Frame024Pieces[3];
extern const u8 gSpriteBank01Frame025Pieces[3];
extern const u8 gSpriteBank01Frame026Pieces[3];
extern const u8 gSpriteBank01Frame027Pieces[3];
extern const u8 gSpriteBank01Frame028Pieces[2];
extern const u8 gSpriteBank01Frame029Pieces[4];
extern const u8 gSpriteBank01Frame030Pieces[4];
extern const u8 gSpriteBank01Frame031Pieces[4];
extern const u8 gSpriteBank01Frame032Pieces[3];
extern const u8 gSpriteBank01Frame033Pieces[4];
extern const u8 gSpriteBank01Frame034Pieces[3];
extern const u8 gSpriteBank01Frame035Pieces[4];
extern const u8 gSpriteBank01Frame036Pieces[3];
extern const u8 gSpriteBank01Frame037Pieces[2];
extern const u8 gSpriteBank01Frame038Pieces[2];
extern const u8 gSpriteBank01Frame039Pieces[2];
extern const u8 gSpriteBank01Frame040Pieces[1];
extern const u8 gSpriteBank01Frame041Pieces[4];
extern const u8 gSpriteBank01Frame042Pieces[2];
extern const u8 gSpriteBank01Frame043Pieces[3];
extern const u8 gSpriteBank01Frame044Pieces[3];
extern const u8 gSpriteBank01Frame045Pieces[2];
extern const u8 gSpriteBank01Frame046Pieces[4];
extern const u8 gSpriteBank01Frame047Pieces[4];
extern const u8 gSpriteBank01Frame048Pieces[2];
extern const u8 gSpriteBank01Frame049Pieces[3];
extern const u8 gSpriteBank01Frame050Pieces[2];
extern const u8 gSpriteBank01Frame051Pieces[3];
extern const u8 gSpriteBank01Frame052Pieces[2];
extern const u8 gSpriteBank01Frame053Pieces[3];
extern const u8 gSpriteBank01Frame054Pieces[4];
extern const u8 gSpriteBank01Frame055Pieces[3];
extern const u8 gSpriteBank01Frame056Pieces[2];
extern const u8 gSpriteBank01Frame057Pieces[4];
extern const u8 gSpriteBank01Frame058Pieces[3];
extern const u8 gSpriteBank01Frame059Pieces[3];
extern const u8 gSpriteBank01Frame060Pieces[2];
extern const u8 gSpriteBank01Frame061Pieces[3];
extern const u8 gSpriteBank01Frame062Pieces[5];
extern const u8 gSpriteBank01Frame063Pieces[4];
extern const u8 gSpriteBank01Frame064Pieces[3];
extern const u8 gSpriteBank01Frame065Pieces[4];
extern const u8 gSpriteBank01Frame066Pieces[4];
extern const u8 gSpriteBank01Frame067Pieces[2];
extern const u8 gSpriteBank01Frame068Pieces[3];
extern const u8 gSpriteBank01Frame069Pieces[4];
extern const u8 gSpriteBank01Frame070Pieces[2];
extern const u8 gSpriteBank01Frame071Pieces[3];
extern const u8 gSpriteBank01Frame072Pieces[3];
extern const u8 gSpriteBank01Frame073Pieces[4];
extern const u8 gSpriteBank01Frame074Pieces[2];
extern const u8 gSpriteBank01Frame075Pieces[2];
extern const u8 gSpriteBank01Frame076Pieces[4];
extern const u8 gSpriteBank01Frame077Pieces[3];
extern const u8 gSpriteBank01Frame078Pieces[3];
extern const u8 gSpriteBank01Frame079Pieces[2];
extern const u8 gSpriteBank01Frame080Pieces[3];
extern const u8 gSpriteBank01Frame081Pieces[3];
extern const u8 gSpriteBank01Frame082Pieces[4];
extern const u8 gSpriteBank01Frame083Pieces[4];
extern const u8 gSpriteBank01Frame084Pieces[4];
extern const u8 gSpriteBank01Frame085Pieces[4];
extern const u8 gSpriteBank01Frame086Pieces[4];
extern const u8 gSpriteBank01Frame087Pieces[4];
extern const u8 gSpriteBank01Frame088Pieces[2];
extern const u8 gSpriteBank01Frame089Pieces[3];
extern const u8 gSpriteBank01Frame090Pieces[2];
extern const u8 gSpriteBank01Frame091Pieces[3];
extern const u8 gSpriteBank01Frame092Pieces[3];
extern const u8 gSpriteBank01Frame093Pieces[3];
extern const u8 gSpriteBank01Frame094Pieces[3];
extern const u8 gSpriteBank01Frame095Pieces[3];
extern const u8 gSpriteBank01Frame096Pieces[4];
extern const u8 gSpriteBank01Frame097Pieces[3];
extern const u8 gSpriteBank01Frame098Pieces[2];
extern const u8 gSpriteBank01Frame099Pieces[3];
extern const u8 gSpriteBank01Frame100Pieces[3];
extern const u8 gSpriteBank01Frame101Pieces[3];
extern const u8 gSpriteBank01Frame102Pieces[4];
extern const u8 gSpriteBank01Frame103Pieces[5];
extern const u8 gSpriteBank01Frame104Pieces[4];
extern const u8 gSpriteBank01Frame105Pieces[4];
extern const u8 gSpriteBank01Frame106Pieces[4];
extern const u8 gSpriteBank01Frame107Pieces[3];
extern const u8 gSpriteBank01Frame108Pieces[3];
extern const u8 gSpriteBank01Frame109Pieces[3];
extern const u8 gSpriteBank01Frame110Pieces[4];
extern const u8 gSpriteBank01Frame111Pieces[5];
extern const u8 gSpriteBank01Frame112Pieces[5];
extern const u8 gSpriteBank01Frame113Pieces[3];
extern const u8 gSpriteBank01Frame114Pieces[2];
extern const u8 gSpriteBank01Frame115Pieces[1];
extern const u8 gSpriteBank01Frame116Pieces[1];
extern const u8 gSpriteBank01Frame117Pieces[2];
extern const u8 gSpriteBank01Frame118Pieces[3];
extern const u8 gSpriteBank01Frame119Pieces[2];
extern const u8 gSpriteBank01Frame120Pieces[1];
extern const u8 gSpriteBank01Frame121Pieces[2];
extern const u8 gSpriteBank01Frame122Pieces[2];
extern const u8 gSpriteBank01Frame123Pieces[4];
extern const u8 gSpriteBank01Frame124Pieces[4];
extern const u8 gSpriteBank01Frame125Pieces[3];
extern const u8 gSpriteBank01Frame126Pieces[2];
extern const u8 gSpriteBank01Frame127Pieces[1];
extern const u8 gSpriteBank01Frame128Pieces[1];
extern const u8 gSpriteBank01Frame129Pieces[2];
extern const u8 gSpriteBank01Frame130Pieces[4];
extern const u8 gSpriteBank01Frame131Pieces[3];
extern const u8 gSpriteBank01Frame132Pieces[3];
extern const u8 gSpriteBank01Frame133Pieces[3];
extern const u8 gSpriteBank01Frame134Pieces[3];
extern const u8 gSpriteBank01Frame135Pieces[3];
extern const u8 gSpriteBank01Frame136Pieces[1];
extern const u8 gSpriteBank01Frame137Pieces[3];
extern const u8 gSpriteBank01Frame138Pieces[4];
extern const u8 gSpriteBank01Frame139Pieces[3];
extern const u8 gSpriteBank01Frame140Pieces[4];
extern const u8 gSpriteBank01Frame141Pieces[1];
extern const u8 gSpriteBank01Frame142Pieces[4];
extern const u8 gSpriteBank01Frame143Pieces[3];
extern const u8 gSpriteBank01Frame144Pieces[1];
extern const u8 gSpriteBank01Frame145Pieces[3];
extern const u8 gSpriteBank01Frame146Pieces[1];
extern const u8 gSpriteBank01Frame147Pieces[1];
extern const u8 gSpriteBank01Frame148Pieces[1];
extern const u8 gSpriteBank01Frame149Pieces[1];
extern const u8 gSpriteBank01Frame150Pieces[4];
extern const u8 gSpriteBank01Frame151Pieces[4];
extern const u8 gSpriteBank01Frame152Pieces[1];
extern const u8 gSpriteBank01Frame153Pieces[1];
extern const u8 gSpriteBank01Frame154Pieces[1];
extern const u8 gSpriteBank01Frame155Pieces[1];
extern const u8 gSpriteBank01Frame156Pieces[4];
extern const u8 gSpriteBank01Frame157Pieces[4];
extern const u8 gSpriteBank01Frame158Pieces[4];
extern const u8 gSpriteBank01Frame159Pieces[3];
extern const u8 gSpriteBank01Frame160Pieces[3];
extern const u8 gSpriteBank01Frame161Pieces[3];
extern const u8 gSpriteBank01Frame162Pieces[3];
extern const u8 gSpriteBank01Frame163Pieces[3];
extern const u8 gSpriteBank01Frame164Pieces[3];
extern const u8 gSpriteBank01Frame165Pieces[3];
extern const u8 gSpriteBank01Frame166Pieces[3];
extern const u8 gSpriteBank01Frame167Pieces[2];
extern const u8 gSpriteBank01Frame168Pieces[3];
extern const u8 gSpriteBank01Frame169Pieces[3];
extern const u8 gSpriteBank01Frame170Pieces[5];
extern const u8 gSpriteBank01Frame171Pieces[2];
extern const u8 gSpriteBank01Frame172Pieces[2];
extern const u8 gSpriteBank01Frame173Pieces[5];
extern const u8 gSpriteBank01Frame174Pieces[4];
extern const u8 gSpriteBank01Frame175Pieces[3];
extern const u8 gSpriteBank01Frame176Pieces[3];
extern const u8 gSpriteBank01Frame177Pieces[4];
extern const u8 gSpriteBank01Frame178Pieces[5];
extern const u8 gSpriteBank01Frame179Pieces[4];
extern const u8 gSpriteBank01Frame180Pieces[3];
extern const u8 gSpriteBank01Frame181Pieces[5];
extern const u8 gSpriteBank01Frame182Pieces[3];
extern const u8 gSpriteBank01Frame183Pieces[3];
extern const u8 gSpriteBank01Frame184Pieces[4];
extern const u8 gSpriteBank01Frame185Pieces[3];
extern const u8 gSpriteBank01Frame186Pieces[2];
extern const u8 gSpriteBank01Frame187Pieces[3];
extern const u8 gSpriteBank01Frame188Pieces[4];
extern const u8 gSpriteBank01Frame189Pieces[3];
extern const u8 gSpriteBank01Frame190Pieces[3];
extern const u8 gSpriteBank01Frame191Pieces[3];
extern const u8 gSpriteBank01Frame192Pieces[4];
extern const u8 gSpriteBank01Frame193Pieces[4];
extern const u8 gSpriteBank01Frame194Pieces[2];
extern const u8 gSpriteBank01Frame195Pieces[3];
extern const u8 gSpriteBank01Frame196Pieces[3];
extern const u8 gSpriteBank01Frame197Pieces[2];
extern const u8 gSpriteBank01Frame198Pieces[3];
extern const u8 gSpriteBank01Frame199Pieces[2];
extern const u8 gSpriteBank01Frame200Pieces[3];
extern const u8 gSpriteBank01Frame201Pieces[2];
extern const u8 gSpriteBank01Frame202Pieces[3];
extern const u8 gSpriteBank01Frame203Pieces[2];
extern const u8 gSpriteBank01Frame204Pieces[3];
extern const u8 gSpriteBank01Frame205Pieces[4];
extern const u8 gSpriteBank01Frame206Pieces[3];
extern const u8 gSpriteBank01Frame207Pieces[2];
extern const u8 gSpriteBank01Frame208Pieces[3];
extern const u8 gSpriteBank01Frame209Pieces[3];
extern const u8 gSpriteBank01Frame210Pieces[5];
extern const u8 gSpriteBank01Frame211Pieces[3];
extern const u8 gSpriteBank01Frame212Pieces[3];
extern const u8 gSpriteBank01Frame213Pieces[2];
extern const u8 gSpriteBank01Frame214Pieces[2];
extern const u8 gSpriteBank01Frame215Pieces[2];
extern const u8 gSpriteBank01Frame216Pieces[4];
extern const u8 gSpriteBank01Frame217Pieces[3];
extern const u8 gSpriteBank01Frame218Pieces[3];
extern const u8 gSpriteBank01Frame219Pieces[3];
extern const u8 gSpriteBank01Frame220Pieces[3];
extern const u8 gSpriteBank01Frame221Pieces[3];
extern const u8 gSpriteBank01Frame222Pieces[3];
extern const u8 gSpriteBank01Frame223Pieces[3];
extern const u8 gSpriteBank01Frame224Pieces[2];
extern const u8 gSpriteBank01Frame225Pieces[1];
extern const u8 gSpriteBank01Frame226Pieces[2];
extern const u8 gSpriteBank01Frame227Pieces[2];
extern const u8 gSpriteBank01Frame228Pieces[2];
extern const u8 gSpriteBank01Frame229Pieces[3];
extern const u8 gSpriteBank01Frame230Pieces[3];
extern const u8 gSpriteBank01Frame231Pieces[3];
extern const u8 gSpriteBank01Frame232Pieces[2];
extern const u8 gSpriteBank01Frame233Pieces[2];
extern const u8 gSpriteBank01Frame234Pieces[4];
extern const u8 gSpriteBank01Frame235Pieces[5];
extern const u8 gSpriteBank01Frame236Pieces[5];
extern const u8 gSpriteBank01Frame237Pieces[1];
extern const u8 gSpriteBank01Frame238Pieces[1];
extern const u8 gSpriteBank01Frame239Pieces[1];
extern const u8 gSpriteBank01Frame240Pieces[4];
extern const u8 gSpriteBank01Frame241Pieces[4];
extern const u8 gSpriteBank01Frame242Pieces[4];
extern const u8 gSpriteBank01Frame243Pieces[4];
extern const u8 gSpriteBank01Frame244Pieces[1];
extern const u8 gSpriteBank01Frame245Pieces[3];
extern const u8 gSpriteBank01Frame246Pieces[1];
extern const u8 gSpriteBank01Frame247Pieces[4];
extern const u8 gSpriteBank01Frame248Pieces[1];
extern const u8 gSpriteBank01Frame249Pieces[1];
extern const u8 gSpriteBank01Frame250Pieces[3];
extern const u8 gSpriteBank01Frame251Pieces[4];
extern const u8 gSpriteBank01Frame252Pieces[3];
extern const u8 gSpriteBank01Frame253Pieces[4];
extern const u8 gSpriteBank01Frame254Pieces[4];
extern const u8 gSpriteBank01Frame255Pieces[4];
extern const u8 gSpriteBank01Frame256Pieces[4];
extern const u8 gSpriteBank01Frame257Pieces[4];
extern const u8 gSpriteBank01Frame258Pieces[4];
extern const u8 gSpriteBank01Frame259Pieces[4];
extern const u8 gSpriteBank01Frame260Pieces[4];
extern const u8 gSpriteBank01Frame261Pieces[2];
extern const u8 gSpriteBank01Frame262Pieces[2];
extern const u8 gSpriteBank01Frame263Pieces[2];
extern const u8 gSpriteBank01Frame264Pieces[4];
extern const u8 gSpriteBank01Frame265Pieces[5];
extern const u8 gSpriteBank01Frame266Pieces[5];
extern const u8 gSpriteBank01Frame267Pieces[3];
extern const u8 gSpriteBank01Frame268Pieces[3];
extern const u8 gSpriteBank01Frame269Pieces[3];
extern const u8 gSpriteBank01Frame270Pieces[3];
extern const u8 gSpriteBank01Frame271Pieces[3];
extern const u8 gSpriteBank01Frame272Pieces[4];
extern const u8 gSpriteBank01Frame273Pieces[4];

const struct sprite_anim gSpriteBank01Anims[47] = {
    [0] = {
        .seq = gSpriteBank01Anim00Seq,
        .box = { { -5, -7, 18, 20 }, { -12, -17, 34, 43 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim00Seq),
        .flags = 0,
    },
    [1] = {
        .seq = gSpriteBank01Anim01Seq,
        .box = { { -5, -7, 18, 20 }, { -20, -21, 37, 45 } },
        .tileRecord = 5,
        .duration = 2,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim01Seq),
        .flags = 0,
    },
    [2] = {
        .seq = gSpriteBank01Anim02Seq,
        .box = { { -5, -7, 18, 20 }, { -18, -17, 38, 42 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim02Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [3] = {
        .seq = gSpriteBank01Anim03Seq,
        .box = { { -5, -7, 18, 20 }, { -20, -21, 47, 42 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim03Seq),
        .flags = 0,
    },
    [4] = {
        .seq = gSpriteBank01Anim04Seq,
        .box = { { -5, -7, 18, 20 }, { -13, -12, 32, 34 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim04Seq),
        .flags = 0,
    },
    [5] = {
        .seq = gSpriteBank01Anim05Seq,
        .box = { { -5, -7, 18, 20 }, { -25, -21, 47, 45 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim05Seq),
        .flags = 0,
    },
    [6] = {
        .seq = gSpriteBank01Anim06Seq,
        .box = { { -5, -7, 18, 20 }, { -15, -19, 37, 43 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim06Seq),
        .flags = 0,
    },
    [7] = {
        .seq = gSpriteBank01Anim07Seq,
        .box = { { -5, -7, 18, 20 }, { -22, -12, 45, 38 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim07Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [8] = {
        .seq = gSpriteBank01Anim08Seq,
        .box = { { -5, -7, 18, 20 }, { -22, -11, 45, 37 } },
        .tileRecord = 5,
        .duration = 2,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim08Seq),
        .flags = 0,
    },
    [9] = {
        .seq = gSpriteBank01Anim09Seq,
        .box = { { -5, -7, 18, 20 }, { -18, -14, 42, 38 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim09Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [10] = {
        .seq = gSpriteBank01Anim10Seq,
        .box = { { -5, -7, 18, 20 }, { -12, -11, 32, 41 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim10Seq),
        .flags = 0,
    },
    [11] = {
        .seq = gSpriteBank01Anim11Seq,
        .box = { { -5, -7, 18, 20 }, { -13, -15, 32, 34 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim11Seq),
        .flags = 0,
    },
    [12] = {
        .seq = gSpriteBank01Anim12Seq,
        .box = { { -5, -7, 18, 20 }, { -12, -14, 36, 43 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim12Seq),
        .flags = 0,
    },
    [13] = {
        .seq = gSpriteBank01Anim13Seq,
        .box = { { -5, -7, 18, 20 }, { -15, -17, 37, 43 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim13Seq),
        .flags = 0,
    },
    [14] = {
        .seq = gSpriteBank01Anim14Seq,
        .box = { { -5, -7, 18, 20 }, { -19, -19, 37, 42 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim14Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [15] = {
        .seq = gSpriteBank01Anim15Seq,
        .box = { { -5, -7, 18, 20 }, { -20, -16, 41, 36 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim15Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [16] = {
        .seq = gSpriteBank01Anim16Seq,
        .box = { { -5, -7, 18, 20 }, { -20, -19, 39, 40 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim16Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [17] = {
        .seq = gSpriteBank01Anim17Seq,
        .box = { { -5, -7, 18, 20 }, { -20, -22, 35, 47 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim17Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [18] = {
        .seq = gSpriteBank01Anim18Seq,
        .box = { { -5, -7, 18, 20 }, { -18, -21, 35, 47 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim18Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [19] = {
        .seq = gSpriteBank01Anim19Seq,
        .box = { { -5, -7, 18, 20 }, { -19, -19, 37, 42 } },
        .tileRecord = 5,
        .duration = 2,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim19Seq),
        .flags = 0,
    },
    [20] = {
        .seq = gSpriteBank01Anim20Seq,
        .box = { { -5, -7, 18, 20 }, { -10, -19, 32, 41 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim20Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [21] = {
        .seq = gSpriteBank01Anim21Seq,
        .box = { { -5, -7, 18, 20 }, { -12, -19, 34, 43 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim21Seq),
        .flags = 0,
    },
    [22] = {
        .seq = gSpriteBank01Anim22Seq,
        .box = { { -5, -7, 18, 20 }, { -11, -19, 24, 40 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim22Seq),
        .flags = 0,
    },
    [23] = {
        .seq = gSpriteBank01Anim23Seq,
        .box = { { -5, -7, 18, 20 }, { -22, -16, 47, 34 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim23Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [24] = {
        .seq = gSpriteBank01Anim24Seq,
        .box = { { -5, -7, 18, 20 }, { -22, -15, 47, 30 } },
        .tileRecord = 5,
        .duration = 2,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim24Seq),
        .flags = 0,
    },
    [25] = {
        .seq = gSpriteBank01Anim25Seq,
        .box = { { -5, -7, 18, 20 }, { -25, -19, 53, 33 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim25Seq),
        .flags = 0,
    },
    [26] = {
        .seq = gSpriteBank01Anim26Seq,
        .box = { { -5, -7, 18, 20 }, { -20, -15, 46, 35 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim26Seq),
        .flags = 0,
    },
    [27] = {
        .seq = gSpriteBank01Anim27Seq,
        .box = { { -5, -7, 18, 20 }, { -25, -18, 49, 35 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim27Seq),
        .flags = 0,
    },
    [28] = {
        .seq = gSpriteBank01Anim28Seq,
        .box = { { -5, -7, 18, 20 }, { -24, -13, 55, 36 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim28Seq),
        .flags = 0,
    },
    [29] = {
        .seq = gSpriteBank01Anim29Seq,
        .box = { { -5, -7, 18, 20 }, { -23, -11, 47, 32 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim29Seq),
        .flags = 0,
    },
    [30] = {
        .seq = gSpriteBank01Anim30Seq,
        .box = { { -5, -7, 18, 20 }, { -24, -13, 48, 33 } },
        .tileRecord = 5,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim30Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [31] = {
        .seq = gSpriteBank01Anim31Seq,
        .box = { { -5, -7, 18, 20 }, { -24, -11, 49, 35 } },
        .tileRecord = 7,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim31Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [32] = {
        .seq = gSpriteBank01Anim32Seq,
        .box = { { -5, -7, 18, 20 }, { -23, -16, 47, 37 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim32Seq),
        .flags = 0,
    },
    [33] = {
        .seq = gSpriteBank01Anim33Seq,
        .box = { { -5, -7, 18, 20 }, { -13, -16, 37, 42 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim33Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [34] = {
        .seq = gSpriteBank01Anim34Seq,
        .box = { { -5, -7, 18, 20 }, { -22, -11, 47, 35 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim34Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [35] = {
        .seq = gSpriteBank01Anim35Seq,
        .box = { { -5, -7, 18, 20 }, { -23, -9, 47, 35 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim35Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [36] = {
        .seq = gSpriteBank01Anim36Seq,
        .box = { { -5, -7, 18, 20 }, { -20, -13, 40, 39 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim36Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [37] = {
        .seq = gSpriteBank01Anim37Seq,
        .box = { { -5, -7, 18, 20 }, { -17, -15, 36, 41 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim37Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [38] = {
        .seq = gSpriteBank01Anim38Seq,
        .box = { { -5, -7, 18, 20 }, { -13, -16, 37, 42 } },
        .tileRecord = 5,
        .duration = 2,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim38Seq),
        .flags = 0,
    },
    [39] = {
        .seq = gSpriteBank01Anim39Seq,
        .box = { { -5, -7, 18, 20 }, { -10, -15, 32, 41 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim39Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [40] = {
        .seq = gSpriteBank01Anim40Seq,
        .box = { { -5, -7, 18, 20 }, { -11, -14, 24, 40 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim40Seq),
        .flags = 0,
    },
    [41] = {
        .seq = gSpriteBank01Anim41Seq,
        .box = { { -5, -7, 18, 20 }, { -20, -21, 38, 45 } },
        .tileRecord = 5,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim41Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [42] = {
        .seq = gSpriteBank01Anim42Seq,
        .box = { { -6, -2, 13, 4 }, { -6, -2, 13, 4 } },
        .tileRecord = 7,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim42Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [43] = {
        .seq = gSpriteBank01Anim43Seq,
        .box = { { -16, -28, 32, 57 }, { -16, -28, 32, 57 } },
        .tileRecord = 116,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim43Seq),
        .flags = 0,
    },
    [44] = {
        .seq = gSpriteBank01Anim44Seq,
        .box = { { -15, -23, 31, 46 }, { -69, -51, 85, 74 } },
        .tileRecord = 5,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim44Seq),
        .flags = 0,
    },
    [45] = {
        .seq = gSpriteBank01Anim45Seq,
        .box = { { -25, -15, 51, 31 }, { -25, -30, 57, 56 } },
        .tileRecord = 5,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim45Seq),
        .flags = 0,
    },
    [46] = {
        .seq = gSpriteBank01Anim46Seq,
        .box = { { -22, -17, 45, 34 }, { -22, -17, 47, 41 } },
        .tileRecord = 121,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank01Anim46Seq),
        .flags = 0,
    },
};

const u16 gSpriteBank01Anim00Seq[2] = {
    0, 1,
};
const u16 gSpriteBank01Anim01Seq[4] = {
    2, 3, 4, 5,
};
const u16 gSpriteBank01Anim02Seq[3] = {
    6, 7, 8,
};
const u16 gSpriteBank01Anim03Seq[4] = {
    9, 10, 11, 12,
};
const u16 gSpriteBank01Anim04Seq[2] = {
    13, 14,
};
const u16 gSpriteBank01Anim05Seq[4] = {
    15, 16, 17, 18,
};
const u16 gSpriteBank01Anim06Seq[2] = {
    19, 20,
};
const u16 gSpriteBank01Anim07Seq[8] = {
    21, 22, 23, 24, 25, 26, 27, 28,
};
const u16 gSpriteBank01Anim08Seq[4] = {
    29, 30, 31, 32,
};
const u16 gSpriteBank01Anim09Seq[3] = {
    33, 34, 35,
};
const u16 gSpriteBank01Anim10Seq[4] = {
    36, 37, 38, 39,
};
const u16 gSpriteBank01Anim11Seq[2] = {
    40, 41,
};
const u16 gSpriteBank01Anim12Seq[4] = {
    42, 43, 44, 45,
};
const u16 gSpriteBank01Anim13Seq[2] = {
    46, 47,
};
const u16 gSpriteBank01Anim14Seq[8] = {
    48, 49, 50, 51, 52, 53, 54, 55,
};
const u16 gSpriteBank01Anim15Seq[8] = {
    56, 57, 58, 59, 60, 61, 62, 63,
};
const u16 gSpriteBank01Anim16Seq[8] = {
    64, 65, 66, 67, 68, 69, 70, 71,
};
const u16 gSpriteBank01Anim17Seq[8] = {
    72, 73, 74, 75, 76, 77, 78, 79,
};
const u16 gSpriteBank01Anim18Seq[8] = {
    80, 81, 82, 83, 84, 85, 86, 87,
};
const u16 gSpriteBank01Anim19Seq[4] = {
    88, 89, 90, 91,
};
const u16 gSpriteBank01Anim20Seq[4] = {
    92, 93, 94, 95,
};
const u16 gSpriteBank01Anim21Seq[2] = {
    96, 97,
};
const u16 gSpriteBank01Anim22Seq[2] = {
    98, 99,
};
const u16 gSpriteBank01Anim23Seq[8] = {
    100, 101, 102, 103, 104, 105, 106, 107,
};
const u16 gSpriteBank01Anim24Seq[4] = {
    108, 109, 110, 111,
};
const u16 gSpriteBank01Anim25Seq[6] = {
    112, 113, 114, 115, 116, 117,
};
const u16 gSpriteBank01Anim26Seq[6] = {
    118, 119, 120, 121, 122, 123,
};
const u16 gSpriteBank01Anim27Seq[6] = {
    124, 125, 126, 127, 128, 129,
};
const u16 gSpriteBank01Anim28Seq[6] = {
    130, 131, 132, 133, 134, 135,
};
const u16 gSpriteBank01Anim29Seq[3] = {
    136, 137, 138,
};
const u16 gSpriteBank01Anim30Seq[5] = {
    139, 140, 141, 142, 143,
};
const u16 gSpriteBank01Anim31Seq[15] = {
    144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158,
};
const u16 gSpriteBank01Anim32Seq[8] = {
    159, 160, 161, 162, 163, 164, 165, 166,
};
const u16 gSpriteBank01Anim33Seq[8] = {
    167, 168, 169, 170, 171, 172, 173, 174,
};
const u16 gSpriteBank01Anim34Seq[8] = {
    175, 176, 177, 178, 179, 180, 181, 182,
};
const u16 gSpriteBank01Anim35Seq[8] = {
    183, 184, 185, 186, 187, 188, 189, 190,
};
const u16 gSpriteBank01Anim36Seq[8] = {
    191, 192, 193, 194, 195, 196, 197, 198,
};
const u16 gSpriteBank01Anim37Seq[8] = {
    199, 200, 201, 202, 203, 204, 205, 206,
};
const u16 gSpriteBank01Anim38Seq[4] = {
    207, 208, 209, 210,
};
const u16 gSpriteBank01Anim39Seq[4] = {
    211, 212, 213, 214,
};
const u16 gSpriteBank01Anim40Seq[2] = {
    215, 216,
};
const u16 gSpriteBank01Anim41Seq[8] = {
    217, 218, 219, 220, 221, 222, 223, 224,
};
const u16 gSpriteBank01Anim42Seq[1] = {
    225,
};
const u16 gSpriteBank01Anim43Seq[15] = {
    226, 227, 228, 226, 227, 228, 226, 227, 228, 226, 227, 228, 226, 227, 228,
};
const u16 gSpriteBank01Anim44Seq[15] = {
    229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243,
};
const u16 gSpriteBank01Anim45Seq[9] = {
    244, 245, 246, 247, 248, 249, 250, 251, 252,
};
const u16 gSpriteBank01Anim46Seq[21] = {
    253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268,
    269, 270, 271, 272, 273,
};

const struct sprite_frame *const gSpriteBank01Frames[274] = {
    &gSpriteBank01Frame000.frame,
    &gSpriteBank01Frame001.frame,
    &gSpriteBank01Frame002.frame,
    &gSpriteBank01Frame003.frame,
    &gSpriteBank01Frame004.frame,
    &gSpriteBank01Frame005.frame,
    &gSpriteBank01Frame006.frame,
    &gSpriteBank01Frame007.frame,
    &gSpriteBank01Frame008.frame,
    &gSpriteBank01Frame009.frame,
    &gSpriteBank01Frame010.frame,
    &gSpriteBank01Frame011.frame,
    &gSpriteBank01Frame012.frame,
    &gSpriteBank01Frame013.frame,
    &gSpriteBank01Frame014.frame,
    &gSpriteBank01Frame015.frame,
    &gSpriteBank01Frame016.frame,
    &gSpriteBank01Frame017.frame,
    &gSpriteBank01Frame018.frame,
    &gSpriteBank01Frame019.frame,
    &gSpriteBank01Frame020.frame,
    &gSpriteBank01Frame021.frame,
    &gSpriteBank01Frame022.frame,
    &gSpriteBank01Frame023.frame,
    &gSpriteBank01Frame024.frame,
    &gSpriteBank01Frame025.frame,
    &gSpriteBank01Frame026.frame,
    &gSpriteBank01Frame027.frame,
    &gSpriteBank01Frame028.frame,
    &gSpriteBank01Frame029.frame,
    &gSpriteBank01Frame030.frame,
    &gSpriteBank01Frame031.frame,
    &gSpriteBank01Frame032.frame,
    &gSpriteBank01Frame033.frame,
    &gSpriteBank01Frame034.frame,
    &gSpriteBank01Frame035.frame,
    &gSpriteBank01Frame036.frame,
    &gSpriteBank01Frame037.frame,
    &gSpriteBank01Frame038.frame,
    &gSpriteBank01Frame039.frame,
    &gSpriteBank01Frame040.frame,
    &gSpriteBank01Frame041.frame,
    &gSpriteBank01Frame042.frame,
    &gSpriteBank01Frame043.frame,
    &gSpriteBank01Frame044.frame,
    &gSpriteBank01Frame045.frame,
    &gSpriteBank01Frame046.frame,
    &gSpriteBank01Frame047.frame,
    &gSpriteBank01Frame048.frame,
    &gSpriteBank01Frame049.frame,
    &gSpriteBank01Frame050.frame,
    &gSpriteBank01Frame051.frame,
    &gSpriteBank01Frame052.frame,
    &gSpriteBank01Frame053.frame,
    &gSpriteBank01Frame054.frame,
    &gSpriteBank01Frame055.frame,
    &gSpriteBank01Frame056.frame,
    &gSpriteBank01Frame057.frame,
    &gSpriteBank01Frame058.frame,
    &gSpriteBank01Frame059.frame,
    &gSpriteBank01Frame060.frame,
    &gSpriteBank01Frame061.frame,
    &gSpriteBank01Frame062.frame,
    &gSpriteBank01Frame063.frame,
    &gSpriteBank01Frame064.frame,
    &gSpriteBank01Frame065.frame,
    &gSpriteBank01Frame066.frame,
    &gSpriteBank01Frame067.frame,
    &gSpriteBank01Frame068.frame,
    &gSpriteBank01Frame069.frame,
    &gSpriteBank01Frame070.frame,
    &gSpriteBank01Frame071.frame,
    &gSpriteBank01Frame072.frame,
    &gSpriteBank01Frame073.frame,
    &gSpriteBank01Frame074.frame,
    &gSpriteBank01Frame075.frame,
    &gSpriteBank01Frame076.frame,
    &gSpriteBank01Frame077.frame,
    &gSpriteBank01Frame078.frame,
    &gSpriteBank01Frame079.frame,
    &gSpriteBank01Frame080.frame,
    &gSpriteBank01Frame081.frame,
    &gSpriteBank01Frame082.frame,
    &gSpriteBank01Frame083.frame,
    &gSpriteBank01Frame084.frame,
    &gSpriteBank01Frame085.frame,
    &gSpriteBank01Frame086.frame,
    &gSpriteBank01Frame087.frame,
    &gSpriteBank01Frame088.frame,
    &gSpriteBank01Frame089.frame,
    &gSpriteBank01Frame090.frame,
    &gSpriteBank01Frame091.frame,
    &gSpriteBank01Frame092.frame,
    &gSpriteBank01Frame093.frame,
    &gSpriteBank01Frame094.frame,
    &gSpriteBank01Frame095.frame,
    &gSpriteBank01Frame096.frame,
    &gSpriteBank01Frame097.frame,
    &gSpriteBank01Frame098.frame,
    &gSpriteBank01Frame099.frame,
    &gSpriteBank01Frame100.frame,
    &gSpriteBank01Frame101.frame,
    &gSpriteBank01Frame102.frame,
    &gSpriteBank01Frame103.frame,
    &gSpriteBank01Frame104.frame,
    &gSpriteBank01Frame105.frame,
    &gSpriteBank01Frame106.frame,
    &gSpriteBank01Frame107.frame,
    &gSpriteBank01Frame108.frame,
    &gSpriteBank01Frame109.frame,
    &gSpriteBank01Frame110.frame,
    &gSpriteBank01Frame111.frame,
    &gSpriteBank01Frame112.frame,
    &gSpriteBank01Frame113.frame,
    &gSpriteBank01Frame114.frame,
    &gSpriteBank01Frame115.frame,
    &gSpriteBank01Frame116.frame,
    &gSpriteBank01Frame117.frame,
    &gSpriteBank01Frame118.frame,
    &gSpriteBank01Frame119.frame,
    &gSpriteBank01Frame120.frame,
    &gSpriteBank01Frame121.frame,
    &gSpriteBank01Frame122.frame,
    &gSpriteBank01Frame123.frame,
    &gSpriteBank01Frame124.frame,
    &gSpriteBank01Frame125.frame,
    &gSpriteBank01Frame126.frame,
    &gSpriteBank01Frame127.frame,
    &gSpriteBank01Frame128.frame,
    &gSpriteBank01Frame129.frame,
    &gSpriteBank01Frame130.frame,
    &gSpriteBank01Frame131.frame,
    &gSpriteBank01Frame132.frame,
    &gSpriteBank01Frame133.frame,
    &gSpriteBank01Frame134.frame,
    &gSpriteBank01Frame135.frame,
    &gSpriteBank01Frame136.frame,
    &gSpriteBank01Frame137.frame,
    &gSpriteBank01Frame138.frame,
    &gSpriteBank01Frame139.frame,
    &gSpriteBank01Frame140.frame,
    &gSpriteBank01Frame141.frame,
    &gSpriteBank01Frame142.frame,
    &gSpriteBank01Frame143.frame,
    &gSpriteBank01Frame144.frame,
    &gSpriteBank01Frame145.frame,
    &gSpriteBank01Frame146.frame,
    &gSpriteBank01Frame147.frame,
    &gSpriteBank01Frame148.frame,
    &gSpriteBank01Frame149.frame,
    &gSpriteBank01Frame150.frame,
    &gSpriteBank01Frame151.frame,
    &gSpriteBank01Frame152.frame,
    &gSpriteBank01Frame153.frame,
    &gSpriteBank01Frame154.frame,
    &gSpriteBank01Frame155.frame,
    &gSpriteBank01Frame156.frame,
    &gSpriteBank01Frame157.frame,
    &gSpriteBank01Frame158.frame,
    &gSpriteBank01Frame159.frame,
    &gSpriteBank01Frame160.frame,
    &gSpriteBank01Frame161.frame,
    &gSpriteBank01Frame162.frame,
    &gSpriteBank01Frame163.frame,
    &gSpriteBank01Frame164.frame,
    &gSpriteBank01Frame165.frame,
    &gSpriteBank01Frame166.frame,
    &gSpriteBank01Frame167.frame,
    &gSpriteBank01Frame168.frame,
    &gSpriteBank01Frame169.frame,
    &gSpriteBank01Frame170.frame,
    &gSpriteBank01Frame171.frame,
    &gSpriteBank01Frame172.frame,
    &gSpriteBank01Frame173.frame,
    &gSpriteBank01Frame174.frame,
    &gSpriteBank01Frame175.frame,
    &gSpriteBank01Frame176.frame,
    &gSpriteBank01Frame177.frame,
    &gSpriteBank01Frame178.frame,
    &gSpriteBank01Frame179.frame,
    &gSpriteBank01Frame180.frame,
    &gSpriteBank01Frame181.frame,
    &gSpriteBank01Frame182.frame,
    &gSpriteBank01Frame183.frame,
    &gSpriteBank01Frame184.frame,
    &gSpriteBank01Frame185.frame,
    &gSpriteBank01Frame186.frame,
    &gSpriteBank01Frame187.frame,
    &gSpriteBank01Frame188.frame,
    &gSpriteBank01Frame189.frame,
    &gSpriteBank01Frame190.frame,
    &gSpriteBank01Frame191.frame,
    &gSpriteBank01Frame192.frame,
    &gSpriteBank01Frame193.frame,
    &gSpriteBank01Frame194.frame,
    &gSpriteBank01Frame195.frame,
    &gSpriteBank01Frame196.frame,
    &gSpriteBank01Frame197.frame,
    &gSpriteBank01Frame198.frame,
    &gSpriteBank01Frame199.frame,
    &gSpriteBank01Frame200.frame,
    &gSpriteBank01Frame201.frame,
    &gSpriteBank01Frame202.frame,
    &gSpriteBank01Frame203.frame,
    &gSpriteBank01Frame204.frame,
    &gSpriteBank01Frame205.frame,
    &gSpriteBank01Frame206.frame,
    &gSpriteBank01Frame207.frame,
    &gSpriteBank01Frame208.frame,
    &gSpriteBank01Frame209.frame,
    &gSpriteBank01Frame210.frame,
    &gSpriteBank01Frame211.frame,
    &gSpriteBank01Frame212.frame,
    &gSpriteBank01Frame213.frame,
    &gSpriteBank01Frame214.frame,
    &gSpriteBank01Frame215.frame,
    &gSpriteBank01Frame216.frame,
    &gSpriteBank01Frame217.frame,
    &gSpriteBank01Frame218.frame,
    &gSpriteBank01Frame219.frame,
    &gSpriteBank01Frame220.frame,
    &gSpriteBank01Frame221.frame,
    &gSpriteBank01Frame222.frame,
    &gSpriteBank01Frame223.frame,
    &gSpriteBank01Frame224.frame,
    &gSpriteBank01Frame225.frame,
    &gSpriteBank01Frame226,
    &gSpriteBank01Frame227,
    &gSpriteBank01Frame228,
    &gSpriteBank01Frame229,
    &gSpriteBank01Frame230,
    &gSpriteBank01Frame231,
    &gSpriteBank01Frame232,
    &gSpriteBank01Frame233,
    &gSpriteBank01Frame234,
    &gSpriteBank01Frame235,
    &gSpriteBank01Frame236,
    &gSpriteBank01Frame237,
    &gSpriteBank01Frame238,
    &gSpriteBank01Frame239,
    &gSpriteBank01Frame240,
    &gSpriteBank01Frame241,
    &gSpriteBank01Frame242,
    &gSpriteBank01Frame243,
    &gSpriteBank01Frame244,
    &gSpriteBank01Frame245,
    &gSpriteBank01Frame246,
    &gSpriteBank01Frame247,
    &gSpriteBank01Frame248,
    &gSpriteBank01Frame249,
    &gSpriteBank01Frame250,
    &gSpriteBank01Frame251,
    &gSpriteBank01Frame252,
    &gSpriteBank01Frame253.frame,
    &gSpriteBank01Frame254.frame,
    &gSpriteBank01Frame255.frame,
    &gSpriteBank01Frame256.frame,
    &gSpriteBank01Frame257.frame,
    &gSpriteBank01Frame258.frame,
    &gSpriteBank01Frame259.frame,
    &gSpriteBank01Frame260.frame,
    &gSpriteBank01Frame261.frame,
    &gSpriteBank01Frame262.frame,
    &gSpriteBank01Frame263.frame,
    &gSpriteBank01Frame264.frame,
    &gSpriteBank01Frame265.frame,
    &gSpriteBank01Frame266.frame,
    &gSpriteBank01Frame267.frame,
    &gSpriteBank01Frame268.frame,
    &gSpriteBank01Frame269.frame,
    &gSpriteBank01Frame270.frame,
    &gSpriteBank01Frame271.frame,
    &gSpriteBank01Frame272.frame,
    &gSpriteBank01Frame273.frame,
};

const struct sprite_frame_1box gSpriteBank01Frame000 = {
    SPRITE_FRAME(gSpriteBank01Frame000, SPRITE_TILES_BANK01 + 0x00000),
    { { -4, -9, 13, 24 } },
};
const struct sprite_frame_1box gSpriteBank01Frame001 = {
    SPRITE_FRAME(gSpriteBank01Frame001, SPRITE_TILES_BANK01 + 0x002a0),
    { { -2, -12, 10, 27 } },
};
const struct sprite_frame_1box gSpriteBank01Frame002 = {
    SPRITE_FRAME(gSpriteBank01Frame002, SPRITE_TILES_BANK01 + 0x00480),
    { { -4, -2, 16, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame003 = {
    SPRITE_FRAME(gSpriteBank01Frame003, SPRITE_TILES_BANK01 + 0x00720),
    { { -4, -2, 16, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame004 = {
    SPRITE_FRAME(gSpriteBank01Frame004, SPRITE_TILES_BANK01 + 0x009c0),
    { { -4, -2, 16, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame005 = {
    SPRITE_FRAME(gSpriteBank01Frame005, SPRITE_TILES_BANK01 + 0x00c60),
    { { -4, -2, 16, 20 } },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame006 = {
    SPRITE_FRAME(gSpriteBank01Frame006, SPRITE_TILES_BANK01 + 0x00ec0),
    { { 0, 0, 0, 0 }, { -13, -6, 31, 30 }, { -16, -12, 33, 35 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame007 = {
    SPRITE_FRAME(gSpriteBank01Frame007, SPRITE_TILES_BANK01 + 0x01180),
    { { 0, 0, 0, 0 }, { -13, -6, 31, 30 }, { -15, -12, 33, 34 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame008 = {
    SPRITE_FRAME(gSpriteBank01Frame008, SPRITE_TILES_BANK01 + 0x01420),
    { { 0, 0, 0, 0 }, { -13, -6, 31, 30 }, { -16, -15, 31, 37 } },
    { 0, 0 },
};
const struct sprite_frame_1box gSpriteBank01Frame009 = {
    SPRITE_FRAME(gSpriteBank01Frame009, SPRITE_TILES_BANK01 + 0x01780),
    { { -6, -4, 11, 21 } },
};
const struct sprite_frame_1box gSpriteBank01Frame010 = {
    SPRITE_FRAME(gSpriteBank01Frame010, SPRITE_TILES_BANK01 + 0x019e0),
    { { -10, -2, 17, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame011 = {
    SPRITE_FRAME(gSpriteBank01Frame011, SPRITE_TILES_BANK01 + 0x01c20),
    { { -1, -3, 14, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame012 = {
    SPRITE_FRAME(gSpriteBank01Frame012, SPRITE_TILES_BANK01 + 0x01ea0),
    { { -1, -3, 14, 20 } },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame013 = {
    SPRITE_FRAME(gSpriteBank01Frame013, SPRITE_TILES_BANK01 + 0x02120),
    { { 0, 0, 0, 0 }, { -10, -9, 26, 25 }, { -12, -12, 31, 31 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame014 = {
    SPRITE_FRAME(gSpriteBank01Frame014, SPRITE_TILES_BANK01 + 0x02320),
    { { 0, 0, 0, 0 }, { -8, -10, 26, 27 }, { -11, -10, 29, 28 } },
    { 0, 0 },
};
const struct sprite_frame_1box gSpriteBank01Frame015 = {
    SPRITE_FRAME(gSpriteBank01Frame015, SPRITE_TILES_BANK01 + 0x02560),
    { { -3, 0, 17, 18 } },
};
const struct sprite_frame_1box gSpriteBank01Frame016 = {
    SPRITE_FRAME(gSpriteBank01Frame016, SPRITE_TILES_BANK01 + 0x02820),
    { { 0, -4, 14, 22 } },
};
const struct sprite_frame_1box gSpriteBank01Frame017 = {
    SPRITE_FRAME(gSpriteBank01Frame017, SPRITE_TILES_BANK01 + 0x02aa0),
    { { -1, -1, 13, 21 } },
};
const struct sprite_frame_1box gSpriteBank01Frame018 = {
    SPRITE_FRAME(gSpriteBank01Frame018, SPRITE_TILES_BANK01 + 0x02d40),
    { { -4, -3, 14, 22 } },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame019 = {
    SPRITE_FRAME(gSpriteBank01Frame019, SPRITE_TILES_BANK01 + 0x02fc0),
    { { 0, 0, 0, 0 }, { -10, -5, 31, 25 }, { -10, -15, 30, 36 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame020 = {
    SPRITE_FRAME(gSpriteBank01Frame020, SPRITE_TILES_BANK01 + 0x032e0),
    { { 0, 0, 0, 0 }, { -15, -6, 27, 25 }, { -12, -10, 31, 32 } },
    { 0, 0 },
};
const struct sprite_frame_1box gSpriteBank01Frame021 = {
    SPRITE_FRAME(gSpriteBank01Frame021, SPRITE_TILES_BANK01 + 0x035e0),
    { { -2, -3, 18, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame022 = {
    SPRITE_FRAME(gSpriteBank01Frame022, SPRITE_TILES_BANK01 + 0x03880),
    { { -2, -3, 18, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame023 = {
    SPRITE_FRAME(gSpriteBank01Frame023, SPRITE_TILES_BANK01 + 0x03b20),
    { { -2, -3, 18, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame024 = {
    SPRITE_FRAME(gSpriteBank01Frame024, SPRITE_TILES_BANK01 + 0x03da0),
    { { -2, -3, 18, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame025 = {
    SPRITE_FRAME(gSpriteBank01Frame025, SPRITE_TILES_BANK01 + 0x03fe0),
    { { -2, -3, 18, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame026 = {
    SPRITE_FRAME(gSpriteBank01Frame026, SPRITE_TILES_BANK01 + 0x04240),
    { { -2, -3, 18, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame027 = {
    SPRITE_FRAME(gSpriteBank01Frame027, SPRITE_TILES_BANK01 + 0x044a0),
    { { -2, -3, 18, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame028 = {
    SPRITE_FRAME(gSpriteBank01Frame028, SPRITE_TILES_BANK01 + 0x04700),
    { { -2, -3, 18, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame029 = {
    SPRITE_FRAME(gSpriteBank01Frame029, SPRITE_TILES_BANK01 + 0x035e0),
    { { -1, -6, 18, 13 } },
};
const struct sprite_frame_1box gSpriteBank01Frame030 = {
    SPRITE_FRAME(gSpriteBank01Frame030, SPRITE_TILES_BANK01 + 0x03880),
    { { -1, -6, 18, 13 } },
};
const struct sprite_frame_1box gSpriteBank01Frame031 = {
    SPRITE_FRAME(gSpriteBank01Frame031, SPRITE_TILES_BANK01 + 0x03b20),
    { { -1, -6, 18, 13 } },
};
const struct sprite_frame_1box gSpriteBank01Frame032 = {
    SPRITE_FRAME(gSpriteBank01Frame032, SPRITE_TILES_BANK01 + 0x03da0),
    { { -1, -6, 18, 13 } },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame033 = {
    SPRITE_FRAME(gSpriteBank01Frame033, SPRITE_TILES_BANK01 + 0x04940),
    { { 0, 0, 0, 0 }, { -10, -12, 31, 33 }, { -11, -9, 31, 30 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame034 = {
    SPRITE_FRAME(gSpriteBank01Frame034, SPRITE_TILES_BANK01 + 0x04c00),
    { { 0, 0, 0, 0 }, { -10, -12, 31, 33 }, { -11, -9, 31, 30 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame035 = {
    SPRITE_FRAME(gSpriteBank01Frame035, SPRITE_TILES_BANK01 + 0x04ec0),
    { { 0, 0, 0, 0 }, { -10, -12, 31, 33 }, { -11, -9, 31, 30 } },
    { 0, 0 },
};
const struct sprite_frame_1box gSpriteBank01Frame036 = {
    SPRITE_FRAME(gSpriteBank01Frame036, SPRITE_TILES_BANK01 + 0x051a0),
    { { -3, -7, 13, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame037 = {
    SPRITE_FRAME(gSpriteBank01Frame037, SPRITE_TILES_BANK01 + 0x053e0),
    { { -3, -7, 13, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame038 = {
    SPRITE_FRAME(gSpriteBank01Frame038, SPRITE_TILES_BANK01 + 0x05600),
    { { -3, -7, 13, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame039 = {
    SPRITE_FRAME(gSpriteBank01Frame039, SPRITE_TILES_BANK01 + 0x05820),
    { { -3, -7, 13, 19 } },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame040 = {
    SPRITE_FRAME(gSpriteBank01Frame040, SPRITE_TILES_BANK01 + 0x05aa0),
    { { 0, 0, 0, 0 }, { -8, -9, 24, 28 }, { -11, -10, 28, 30 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame041 = {
    SPRITE_FRAME(gSpriteBank01Frame041, SPRITE_TILES_BANK01 + 0x05ca0),
    { { 0, 0, 0, 0 }, { -8, -9, 24, 28 }, { -11, -12, 29, 31 } },
    { 0, 0 },
};
const struct sprite_frame_1box gSpriteBank01Frame042 = {
    SPRITE_FRAME(gSpriteBank01Frame042, SPRITE_TILES_BANK01 + 0x05f00),
    { { 2, -7, 10, 16 } },
};
const struct sprite_frame_1box gSpriteBank01Frame043 = {
    SPRITE_FRAME(gSpriteBank01Frame043, SPRITE_TILES_BANK01 + 0x06120),
    { { 2, -10, 9, 24 } },
};
const struct sprite_frame_1box gSpriteBank01Frame044 = {
    SPRITE_FRAME(gSpriteBank01Frame044, SPRITE_TILES_BANK01 + 0x06320),
    { { -5, -7, 11, 17 } },
};
const struct sprite_frame_1box gSpriteBank01Frame045 = {
    SPRITE_FRAME(gSpriteBank01Frame045, SPRITE_TILES_BANK01 + 0x065c0),
    { { -5, -5, 12, 18 } },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame046 = {
    SPRITE_FRAME(gSpriteBank01Frame046, SPRITE_TILES_BANK01 + 0x067e0),
    { { 0, 0, 0, 0 }, { -10, -16, 31, 30 }, { -12, -12, 31, 34 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame047 = {
    SPRITE_FRAME(gSpriteBank01Frame047, SPRITE_TILES_BANK01 + 0x06b40),
    { { 0, 0, 0, 0 }, { -12, -12, 28, 28 }, { -13, -15, 34, 38 } },
    { 0, 0 },
};
const struct sprite_frame_1box gSpriteBank01Frame048 = {
    SPRITE_FRAME(gSpriteBank01Frame048, SPRITE_TILES_BANK01 + 0x06e40),
    { { -4, -6, 9, 24 } },
};
const struct sprite_frame_1box gSpriteBank01Frame049 = {
    SPRITE_FRAME(gSpriteBank01Frame049, SPRITE_TILES_BANK01 + 0x07080),
    { { -4, -6, 9, 24 } },
};
const struct sprite_frame_1box gSpriteBank01Frame050 = {
    SPRITE_FRAME(gSpriteBank01Frame050, SPRITE_TILES_BANK01 + 0x072e0),
    { { -4, -6, 9, 24 } },
};
const struct sprite_frame_1box gSpriteBank01Frame051 = {
    SPRITE_FRAME(gSpriteBank01Frame051, SPRITE_TILES_BANK01 + 0x07560),
    { { -4, -6, 9, 24 } },
};
const struct sprite_frame_1box gSpriteBank01Frame052 = {
    SPRITE_FRAME(gSpriteBank01Frame052, SPRITE_TILES_BANK01 + 0x076e0),
    { { -4, -6, 9, 24 } },
};
const struct sprite_frame_1box gSpriteBank01Frame053 = {
    SPRITE_FRAME(gSpriteBank01Frame053, SPRITE_TILES_BANK01 + 0x07920),
    { { -4, -6, 9, 24 } },
};
const struct sprite_frame_1box gSpriteBank01Frame054 = {
    SPRITE_FRAME(gSpriteBank01Frame054, SPRITE_TILES_BANK01 + 0x07bc0),
    { { -4, -6, 9, 24 } },
};
const struct sprite_frame_1box gSpriteBank01Frame055 = {
    SPRITE_FRAME(gSpriteBank01Frame055, SPRITE_TILES_BANK01 + 0x07da0),
    { { -4, -6, 9, 24 } },
};
const struct sprite_frame_1box gSpriteBank01Frame056 = {
    SPRITE_FRAME(gSpriteBank01Frame056, SPRITE_TILES_BANK01 + 0x07f60),
    { { -7, 1, 23, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame057 = {
    SPRITE_FRAME(gSpriteBank01Frame057, SPRITE_TILES_BANK01 + 0x081a0),
    { { -7, 1, 23, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame058 = {
    SPRITE_FRAME(gSpriteBank01Frame058, SPRITE_TILES_BANK01 + 0x084c0),
    { { -7, 1, 23, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame059 = {
    SPRITE_FRAME(gSpriteBank01Frame059, SPRITE_TILES_BANK01 + 0x08780),
    { { -7, 1, 23, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame060 = {
    SPRITE_FRAME(gSpriteBank01Frame060, SPRITE_TILES_BANK01 + 0x08900),
    { { -7, 1, 23, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame061 = {
    SPRITE_FRAME(gSpriteBank01Frame061, SPRITE_TILES_BANK01 + 0x08b40),
    { { -7, 1, 23, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame062 = {
    SPRITE_FRAME(gSpriteBank01Frame062, SPRITE_TILES_BANK01 + 0x08de0),
    { { -7, 1, 23, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame063 = {
    SPRITE_FRAME(gSpriteBank01Frame063, SPRITE_TILES_BANK01 + 0x08fa0),
    { { -7, 1, 23, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame064 = {
    SPRITE_FRAME(gSpriteBank01Frame064, SPRITE_TILES_BANK01 + 0x09160),
    { { -5, 6, 20, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame065 = {
    SPRITE_FRAME(gSpriteBank01Frame065, SPRITE_TILES_BANK01 + 0x093e0),
    { { -5, 6, 20, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame066 = {
    SPRITE_FRAME(gSpriteBank01Frame066, SPRITE_TILES_BANK01 + 0x096c0),
    { { -5, 6, 20, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame067 = {
    SPRITE_FRAME(gSpriteBank01Frame067, SPRITE_TILES_BANK01 + 0x09980),
    { { -5, 6, 20, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame068 = {
    SPRITE_FRAME(gSpriteBank01Frame068, SPRITE_TILES_BANK01 + 0x09bc0),
    { { -5, 6, 20, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame069 = {
    SPRITE_FRAME(gSpriteBank01Frame069, SPRITE_TILES_BANK01 + 0x09e20),
    { { -5, 6, 20, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame070 = {
    SPRITE_FRAME(gSpriteBank01Frame070, SPRITE_TILES_BANK01 + 0x0a0a0),
    { { -5, 6, 20, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame071 = {
    SPRITE_FRAME(gSpriteBank01Frame071, SPRITE_TILES_BANK01 + 0x0a320),
    { { -5, 6, 20, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame072 = {
    SPRITE_FRAME(gSpriteBank01Frame072, SPRITE_TILES_BANK01 + 0x0a580),
    { { 0, 2, 11, 17 } },
};
const struct sprite_frame_1box gSpriteBank01Frame073 = {
    SPRITE_FRAME(gSpriteBank01Frame073, SPRITE_TILES_BANK01 + 0x0a820),
    { { 0, 2, 11, 17 } },
};
const struct sprite_frame_1box gSpriteBank01Frame074 = {
    SPRITE_FRAME(gSpriteBank01Frame074, SPRITE_TILES_BANK01 + 0x0aae0),
    { { 0, 2, 11, 17 } },
};
const struct sprite_frame_1box gSpriteBank01Frame075 = {
    SPRITE_FRAME(gSpriteBank01Frame075, SPRITE_TILES_BANK01 + 0x0ad60),
    { { 0, 2, 11, 17 } },
};
const struct sprite_frame_1box gSpriteBank01Frame076 = {
    SPRITE_FRAME(gSpriteBank01Frame076, SPRITE_TILES_BANK01 + 0x0afa0),
    { { 0, 2, 11, 17 } },
};
const struct sprite_frame_1box gSpriteBank01Frame077 = {
    SPRITE_FRAME(gSpriteBank01Frame077, SPRITE_TILES_BANK01 + 0x0b240),
    { { 0, 2, 11, 17 } },
};
const struct sprite_frame_1box gSpriteBank01Frame078 = {
    SPRITE_FRAME(gSpriteBank01Frame078, SPRITE_TILES_BANK01 + 0x0b500),
    { { 0, 2, 11, 17 } },
};
const struct sprite_frame_1box gSpriteBank01Frame079 = {
    SPRITE_FRAME(gSpriteBank01Frame079, SPRITE_TILES_BANK01 + 0x0b7a0),
    { { 0, 2, 11, 17 } },
};
const struct sprite_frame_1box gSpriteBank01Frame080 = {
    SPRITE_FRAME(gSpriteBank01Frame080, SPRITE_TILES_BANK01 + 0x0ba20),
    { { 0, 0, 10, 18 } },
};
const struct sprite_frame_1box gSpriteBank01Frame081 = {
    SPRITE_FRAME(gSpriteBank01Frame081, SPRITE_TILES_BANK01 + 0x0bcc0),
    { { 0, 0, 10, 18 } },
};
const struct sprite_frame_1box gSpriteBank01Frame082 = {
    SPRITE_FRAME(gSpriteBank01Frame082, SPRITE_TILES_BANK01 + 0x0bf60),
    { { 0, 0, 10, 18 } },
};
const struct sprite_frame_1box gSpriteBank01Frame083 = {
    SPRITE_FRAME(gSpriteBank01Frame083, SPRITE_TILES_BANK01 + 0x0c180),
    { { 0, 0, 10, 18 } },
};
const struct sprite_frame_1box gSpriteBank01Frame084 = {
    SPRITE_FRAME(gSpriteBank01Frame084, SPRITE_TILES_BANK01 + 0x0c3a0),
    { { 0, 0, 10, 18 } },
};
const struct sprite_frame_1box gSpriteBank01Frame085 = {
    SPRITE_FRAME(gSpriteBank01Frame085, SPRITE_TILES_BANK01 + 0x0c620),
    { { 0, 0, 10, 18 } },
};
const struct sprite_frame_1box gSpriteBank01Frame086 = {
    SPRITE_FRAME(gSpriteBank01Frame086, SPRITE_TILES_BANK01 + 0x0c8a0),
    { { 0, 0, 10, 18 } },
};
const struct sprite_frame_1box gSpriteBank01Frame087 = {
    SPRITE_FRAME(gSpriteBank01Frame087, SPRITE_TILES_BANK01 + 0x0cac0),
    { { 0, 0, 10, 18 } },
};
const struct sprite_frame_1box gSpriteBank01Frame088 = {
    SPRITE_FRAME(gSpriteBank01Frame088, SPRITE_TILES_BANK01 + 0x06e40),
    { { -5, -7, 12, 23 } },
};
const struct sprite_frame_1box gSpriteBank01Frame089 = {
    SPRITE_FRAME(gSpriteBank01Frame089, SPRITE_TILES_BANK01 + 0x07080),
    { { -5, -7, 12, 23 } },
};
const struct sprite_frame_1box gSpriteBank01Frame090 = {
    SPRITE_FRAME(gSpriteBank01Frame090, SPRITE_TILES_BANK01 + 0x072e0),
    { { -5, -7, 12, 23 } },
};
const struct sprite_frame_1box gSpriteBank01Frame091 = {
    SPRITE_FRAME(gSpriteBank01Frame091, SPRITE_TILES_BANK01 + 0x07560),
    { { -5, -7, 12, 23 } },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame092 = {
    SPRITE_FRAME(gSpriteBank01Frame092, SPRITE_TILES_BANK01 + 0x0cca0),
    { { 0, 0, 0, 0 }, { -10, -9, 32, 30 }, { -9, -19, 30, 41 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame093 = {
    SPRITE_FRAME(gSpriteBank01Frame093, SPRITE_TILES_BANK01 + 0x0cf40),
    { { 0, 0, 0, 0 }, { -10, -9, 32, 30 }, { -10, -19, 31, 41 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame094 = {
    SPRITE_FRAME(gSpriteBank01Frame094, SPRITE_TILES_BANK01 + 0x0d200),
    { { 0, 0, 0, 0 }, { -10, -9, 32, 30 }, { -9, -19, 27, 41 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame095 = {
    SPRITE_FRAME(gSpriteBank01Frame095, SPRITE_TILES_BANK01 + 0x0d4a0),
    { { 0, 0, 0, 0 }, { -10, -9, 32, 30 }, { -8, -19, 30, 40 } },
    { 0, 0 },
};
const struct sprite_frame_1box gSpriteBank01Frame096 = {
    SPRITE_FRAME(gSpriteBank01Frame096, SPRITE_TILES_BANK01 + 0x0d760),
    { { -4, -4, 14, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame097 = {
    SPRITE_FRAME(gSpriteBank01Frame097, SPRITE_TILES_BANK01 + 0x0d9e0),
    { { -4, -4, 14, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame098 = {
    SPRITE_FRAME(gSpriteBank01Frame098, SPRITE_TILES_BANK01 + 0x0dba0),
    { { -4, -8, 12, 24 } },
};
const struct sprite_frame_1box gSpriteBank01Frame099 = {
    SPRITE_FRAME(gSpriteBank01Frame099, SPRITE_TILES_BANK01 + 0x0de20),
    { { -4, -8, 12, 24 } },
};
const struct sprite_frame_1box gSpriteBank01Frame100 = {
    SPRITE_FRAME(gSpriteBank01Frame100, SPRITE_TILES_BANK01 + 0x0dfe0),
    { { -4, -1, 22, 9 } },
};
const struct sprite_frame_1box gSpriteBank01Frame101 = {
    SPRITE_FRAME(gSpriteBank01Frame101, SPRITE_TILES_BANK01 + 0x0e280),
    { { -4, -1, 22, 9 } },
};
const struct sprite_frame_1box gSpriteBank01Frame102 = {
    SPRITE_FRAME(gSpriteBank01Frame102, SPRITE_TILES_BANK01 + 0x0e520),
    { { -4, -1, 22, 9 } },
};
const struct sprite_frame_1box gSpriteBank01Frame103 = {
    SPRITE_FRAME(gSpriteBank01Frame103, SPRITE_TILES_BANK01 + 0x0e740),
    { { -4, -1, 22, 9 } },
};
const struct sprite_frame_1box gSpriteBank01Frame104 = {
    SPRITE_FRAME(gSpriteBank01Frame104, SPRITE_TILES_BANK01 + 0x0e980),
    { { -4, -1, 22, 9 } },
};
const struct sprite_frame_1box gSpriteBank01Frame105 = {
    SPRITE_FRAME(gSpriteBank01Frame105, SPRITE_TILES_BANK01 + 0x0ec00),
    { { -4, -1, 22, 9 } },
};
const struct sprite_frame_1box gSpriteBank01Frame106 = {
    SPRITE_FRAME(gSpriteBank01Frame106, SPRITE_TILES_BANK01 + 0x0eec0),
    { { -4, -1, 22, 9 } },
};
const struct sprite_frame_1box gSpriteBank01Frame107 = {
    SPRITE_FRAME(gSpriteBank01Frame107, SPRITE_TILES_BANK01 + 0x0f100),
    { { -4, -1, 22, 9 } },
};
const struct sprite_frame_1box gSpriteBank01Frame108 = {
    SPRITE_FRAME(gSpriteBank01Frame108, SPRITE_TILES_BANK01 + 0x0dfe0),
    { { -5, -1, 23, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame109 = {
    SPRITE_FRAME(gSpriteBank01Frame109, SPRITE_TILES_BANK01 + 0x0e280),
    { { -5, -1, 23, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame110 = {
    SPRITE_FRAME(gSpriteBank01Frame110, SPRITE_TILES_BANK01 + 0x0e520),
    { { -5, -1, 23, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame111 = {
    SPRITE_FRAME(gSpriteBank01Frame111, SPRITE_TILES_BANK01 + 0x0e740),
    { { -5, -1, 23, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame112 = {
    SPRITE_FRAME(gSpriteBank01Frame112, SPRITE_TILES_BANK01 + 0x0f300),
    { { -17, -2, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame113 = {
    SPRITE_FRAME(gSpriteBank01Frame113, SPRITE_TILES_BANK01 + 0x0f500),
    { { -17, -2, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame114 = {
    SPRITE_FRAME(gSpriteBank01Frame114, SPRITE_TILES_BANK01 + 0x0f760),
    { { -7, -5, 20, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame115 = {
    SPRITE_FRAME(gSpriteBank01Frame115, SPRITE_TILES_BANK01 + 0x0f9a0),
    { { -4, -5, 18, 9 } },
};
const struct sprite_frame_1box gSpriteBank01Frame116 = {
    SPRITE_FRAME(gSpriteBank01Frame116, SPRITE_TILES_BANK01 + 0x0fba0),
    { { -4, -7, 21, 9 } },
};
const struct sprite_frame_1box gSpriteBank01Frame117 = {
    SPRITE_FRAME(gSpriteBank01Frame117, SPRITE_TILES_BANK01 + 0x0fda0),
    { { -3, -3, 23, 10 } },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame118 = {
    SPRITE_FRAME(gSpriteBank01Frame118, SPRITE_TILES_BANK01 + 0x10020),
    { { 0, 0, 0, 0 }, { -18, -10, 35, 25 }, { -17, -10, 38, 27 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame119 = {
    SPRITE_FRAME(gSpriteBank01Frame119, SPRITE_TILES_BANK01 + 0x10280),
    { { 0, 0, 0, 0 }, { -16, -11, 32, 29 }, { -17, -12, 34, 26 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame120 = {
    SPRITE_FRAME(gSpriteBank01Frame120, SPRITE_TILES_BANK01 + 0x104a0),
    { { 0, 0, 0, 0 }, { -15, -15, 33, 31 }, { -13, -13, 29, 28 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame121 = {
    SPRITE_FRAME(gSpriteBank01Frame121, SPRITE_TILES_BANK01 + 0x106a0),
    { { 0, 0, 0, 0 }, { -13, -10, 32, 28 }, { -14, -8, 33, 25 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame122 = {
    SPRITE_FRAME(gSpriteBank01Frame122, SPRITE_TILES_BANK01 + 0x108c0),
    { { 0, 0, 0, 0 }, { -12, -12, 31, 29 }, { -13, -12, 33, 31 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame123 = {
    SPRITE_FRAME(gSpriteBank01Frame123, SPRITE_TILES_BANK01 + 0x10ae0),
    { { 0, 0, 0, 0 }, { -11, -13, 32, 31 }, { -17, -12, 40, 31 } },
    { 0, 0 },
};
const struct sprite_frame_1box gSpriteBank01Frame124 = {
    SPRITE_FRAME(gSpriteBank01Frame124, SPRITE_TILES_BANK01 + 0x10dc0),
    { { -25, -10, 49, 17 } },
};
const struct sprite_frame_1box gSpriteBank01Frame125 = {
    SPRITE_FRAME(gSpriteBank01Frame125, SPRITE_TILES_BANK01 + 0x10fe0),
    { { -20, -17, 40, 27 } },
};
const struct sprite_frame_1box gSpriteBank01Frame126 = {
    SPRITE_FRAME(gSpriteBank01Frame126, SPRITE_TILES_BANK01 + 0x11280),
    { { -16, -18, 32, 31 } },
};
const struct sprite_frame_1box gSpriteBank01Frame127 = {
    SPRITE_FRAME(gSpriteBank01Frame127, SPRITE_TILES_BANK01 + 0x114a0),
    { { -14, -14, 30, 30 } },
};
const struct sprite_frame_1box gSpriteBank01Frame128 = {
    SPRITE_FRAME(gSpriteBank01Frame128, SPRITE_TILES_BANK01 + 0x116a0),
    { { -14, -11, 28, 27 } },
};
const struct sprite_frame_1box gSpriteBank01Frame129 = {
    SPRITE_FRAME(gSpriteBank01Frame129, SPRITE_TILES_BANK01 + 0x118a0),
    { { -19, -11, 35, 28 } },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame130 = {
    SPRITE_FRAME(gSpriteBank01Frame130, SPRITE_TILES_BANK01 + 0x11ae0),
    { { 0, 0, 0, 0 }, { -7, -12, 29, 28 }, { -22, -10, 46, 26 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame131 = {
    SPRITE_FRAME(gSpriteBank01Frame131, SPRITE_TILES_BANK01 + 0x11dc0),
    { { 0, 0, 0, 0 }, { -13, -10, 31, 24 }, { -22, -10, 46, 26 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame132 = {
    SPRITE_FRAME(gSpriteBank01Frame132, SPRITE_TILES_BANK01 + 0x12020),
    { { 0, 0, 0, 0 }, { -10, -9, 28, 26 }, { -12, -10, 33, 28 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame133 = {
    SPRITE_FRAME(gSpriteBank01Frame133, SPRITE_TILES_BANK01 + 0x12260),
    { { 0, 0, 0, 0 }, { -11, -9, 29, 25 }, { -13, -9, 38, 24 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame134 = {
    SPRITE_FRAME(gSpriteBank01Frame134, SPRITE_TILES_BANK01 + 0x124c0),
    { { 0, 0, 0, 0 }, { -14, -11, 31, 25 }, { -16, -10, 38, 26 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame135 = {
    SPRITE_FRAME(gSpriteBank01Frame135, SPRITE_TILES_BANK01 + 0x12720),
    { { 0, 0, 0, 0 }, { -14, -9, 33, 28 }, { -17, -9, 44, 27 } },
    { 0, 0 },
};
const struct sprite_frame_1box gSpriteBank01Frame136 = {
    SPRITE_FRAME(gSpriteBank01Frame136, SPRITE_TILES_BANK01 + 0x129c0),
    { { -3, 0, 19, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame137 = {
    SPRITE_FRAME(gSpriteBank01Frame137, SPRITE_TILES_BANK01 + 0x12dc0),
    { { -3, 0, 19, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame138 = {
    SPRITE_FRAME(gSpriteBank01Frame138, SPRITE_TILES_BANK01 + 0x13080),
    { { -3, 0, 19, 6 } },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame139 = {
    SPRITE_FRAME(gSpriteBank01Frame139, SPRITE_TILES_BANK01 + 0x132a0),
    { { 0, 0, 0, 0 }, { -9, -9, 33, 28 }, { -19, -9, 41, 27 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame140 = {
    SPRITE_FRAME(gSpriteBank01Frame140, SPRITE_TILES_BANK01 + 0x13560),
    { { 0, 0, 0, 0 }, { -8, -11, 29, 28 }, { -19, -11, 41, 26 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame141 = {
    SPRITE_FRAME(gSpriteBank01Frame141, SPRITE_TILES_BANK01 + 0x13840),
    { { 0, 0, 0, 0 }, { -10, -8, 32, 26 }, { -22, -8, 43, 25 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame142 = {
    SPRITE_FRAME(gSpriteBank01Frame142, SPRITE_TILES_BANK01 + 0x13c40),
    { { 0, 0, 0, 0 }, { -7, -11, 29, 30 }, { -18, -10, 40, 28 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame143 = {
    SPRITE_FRAME(gSpriteBank01Frame143, SPRITE_TILES_BANK01 + 0x132a0),
    { { 0, 0, 0, 0 }, { -9, -10, 29, 29 }, { -19, -9, 41, 27 } },
    { 0, 0 },
};
const struct sprite_frame_1box gSpriteBank01Frame144 = {
    SPRITE_FRAME(gSpriteBank01Frame144, SPRITE_TILES_BANK01 + 0x129c0),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame145 = {
    SPRITE_FRAME(gSpriteBank01Frame145, SPRITE_TILES_BANK01 + 0x13f20),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame146 = {
    SPRITE_FRAME(gSpriteBank01Frame146, SPRITE_TILES_BANK01 + 0x141e0),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame147 = {
    SPRITE_FRAME(gSpriteBank01Frame147, SPRITE_TILES_BANK01 + 0x145e0),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame148 = {
    SPRITE_FRAME(gSpriteBank01Frame148, SPRITE_TILES_BANK01 + 0x149e0),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame149 = {
    SPRITE_FRAME(gSpriteBank01Frame149, SPRITE_TILES_BANK01 + 0x14de0),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame150 = {
    SPRITE_FRAME(gSpriteBank01Frame150, SPRITE_TILES_BANK01 + 0x151e0),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame151 = {
    SPRITE_FRAME(gSpriteBank01Frame151, SPRITE_TILES_BANK01 + 0x154c0),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame152 = {
    SPRITE_FRAME(gSpriteBank01Frame152, SPRITE_TILES_BANK01 + 0x157a0),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame153 = {
    SPRITE_FRAME(gSpriteBank01Frame153, SPRITE_TILES_BANK01 + 0x15ba0),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame154 = {
    SPRITE_FRAME(gSpriteBank01Frame154, SPRITE_TILES_BANK01 + 0x15fa0),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame155 = {
    SPRITE_FRAME(gSpriteBank01Frame155, SPRITE_TILES_BANK01 + 0x163a0),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame156 = {
    SPRITE_FRAME(gSpriteBank01Frame156, SPRITE_TILES_BANK01 + 0x167a0),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame157 = {
    SPRITE_FRAME(gSpriteBank01Frame157, SPRITE_TILES_BANK01 + 0x16a80),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame158 = {
    SPRITE_FRAME(gSpriteBank01Frame158, SPRITE_TILES_BANK01 + 0x16d60),
    { { -6, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame159 = {
    SPRITE_FRAME(gSpriteBank01Frame159, SPRITE_TILES_BANK01 + 0x0dfe0),
    { { -6, 0, 24, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame160 = {
    SPRITE_FRAME(gSpriteBank01Frame160, SPRITE_TILES_BANK01 + 0x17040),
    { { -6, 0, 24, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame161 = {
    SPRITE_FRAME(gSpriteBank01Frame161, SPRITE_TILES_BANK01 + 0x172a0),
    { { -6, 0, 24, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame162 = {
    SPRITE_FRAME(gSpriteBank01Frame162, SPRITE_TILES_BANK01 + 0x17540),
    { { -6, 0, 24, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame163 = {
    SPRITE_FRAME(gSpriteBank01Frame163, SPRITE_TILES_BANK01 + 0x17800),
    { { -6, 0, 24, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame164 = {
    SPRITE_FRAME(gSpriteBank01Frame164, SPRITE_TILES_BANK01 + 0x17ac0),
    { { -6, 0, 24, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame165 = {
    SPRITE_FRAME(gSpriteBank01Frame165, SPRITE_TILES_BANK01 + 0x17d80),
    { { -6, 0, 24, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame166 = {
    SPRITE_FRAME(gSpriteBank01Frame166, SPRITE_TILES_BANK01 + 0x18040),
    { { -6, 0, 24, 6 } },
};
const struct sprite_frame_1box gSpriteBank01Frame167 = {
    SPRITE_FRAME(gSpriteBank01Frame167, SPRITE_TILES_BANK01 + 0x18300),
    { { 0, -10, 9, 26 } },
};
const struct sprite_frame_1box gSpriteBank01Frame168 = {
    SPRITE_FRAME(gSpriteBank01Frame168, SPRITE_TILES_BANK01 + 0x18580),
    { { 0, -10, 9, 26 } },
};
const struct sprite_frame_1box gSpriteBank01Frame169 = {
    SPRITE_FRAME(gSpriteBank01Frame169, SPRITE_TILES_BANK01 + 0x18820),
    { { 0, -10, 9, 26 } },
};
const struct sprite_frame_1box gSpriteBank01Frame170 = {
    SPRITE_FRAME(gSpriteBank01Frame170, SPRITE_TILES_BANK01 + 0x18ac0),
    { { 0, -10, 9, 26 } },
};
const struct sprite_frame_1box gSpriteBank01Frame171 = {
    SPRITE_FRAME(gSpriteBank01Frame171, SPRITE_TILES_BANK01 + 0x18c40),
    { { 0, -10, 9, 26 } },
};
const struct sprite_frame_1box gSpriteBank01Frame172 = {
    SPRITE_FRAME(gSpriteBank01Frame172, SPRITE_TILES_BANK01 + 0x18ec0),
    { { 0, -10, 9, 26 } },
};
const struct sprite_frame_1box gSpriteBank01Frame173 = {
    SPRITE_FRAME(gSpriteBank01Frame173, SPRITE_TILES_BANK01 + 0x191c0),
    { { 0, -10, 9, 26 } },
};
const struct sprite_frame_1box gSpriteBank01Frame174 = {
    SPRITE_FRAME(gSpriteBank01Frame174, SPRITE_TILES_BANK01 + 0x19380),
    { { 0, -10, 9, 26 } },
};
const struct sprite_frame_1box gSpriteBank01Frame175 = {
    SPRITE_FRAME(gSpriteBank01Frame175, SPRITE_TILES_BANK01 + 0x19560),
    { { -1, -3, 24, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame176 = {
    SPRITE_FRAME(gSpriteBank01Frame176, SPRITE_TILES_BANK01 + 0x19820),
    { { -1, -3, 24, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame177 = {
    SPRITE_FRAME(gSpriteBank01Frame177, SPRITE_TILES_BANK01 + 0x19ae0),
    { { -1, -3, 24, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame178 = {
    SPRITE_FRAME(gSpriteBank01Frame178, SPRITE_TILES_BANK01 + 0x19d00),
    { { -1, -3, 24, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame179 = {
    SPRITE_FRAME(gSpriteBank01Frame179, SPRITE_TILES_BANK01 + 0x19f20),
    { { -1, -3, 24, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame180 = {
    SPRITE_FRAME(gSpriteBank01Frame180, SPRITE_TILES_BANK01 + 0x1a1a0),
    { { -1, -3, 24, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame181 = {
    SPRITE_FRAME(gSpriteBank01Frame181, SPRITE_TILES_BANK01 + 0x1a440),
    { { -1, -3, 24, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame182 = {
    SPRITE_FRAME(gSpriteBank01Frame182, SPRITE_TILES_BANK01 + 0x1a660),
    { { -1, -3, 24, 10 } },
};
const struct sprite_frame_1box gSpriteBank01Frame183 = {
    SPRITE_FRAME(gSpriteBank01Frame183, SPRITE_TILES_BANK01 + 0x1a860),
    { { -2, -3, 21, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame184 = {
    SPRITE_FRAME(gSpriteBank01Frame184, SPRITE_TILES_BANK01 + 0x1ab20),
    { { -2, -3, 21, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame185 = {
    SPRITE_FRAME(gSpriteBank01Frame185, SPRITE_TILES_BANK01 + 0x1ae00),
    { { -2, -3, 21, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame186 = {
    SPRITE_FRAME(gSpriteBank01Frame186, SPRITE_TILES_BANK01 + 0x1b080),
    { { -2, -3, 21, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame187 = {
    SPRITE_FRAME(gSpriteBank01Frame187, SPRITE_TILES_BANK01 + 0x1b2c0),
    { { -2, -3, 21, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame188 = {
    SPRITE_FRAME(gSpriteBank01Frame188, SPRITE_TILES_BANK01 + 0x1b560),
    { { -2, -3, 21, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame189 = {
    SPRITE_FRAME(gSpriteBank01Frame189, SPRITE_TILES_BANK01 + 0x1b7e0),
    { { -2, -3, 21, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame190 = {
    SPRITE_FRAME(gSpriteBank01Frame190, SPRITE_TILES_BANK01 + 0x1ba80),
    { { -2, -3, 21, 8 } },
};
const struct sprite_frame_1box gSpriteBank01Frame191 = {
    SPRITE_FRAME(gSpriteBank01Frame191, SPRITE_TILES_BANK01 + 0x1bce0),
    { { 2, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame192 = {
    SPRITE_FRAME(gSpriteBank01Frame192, SPRITE_TILES_BANK01 + 0x1bf40),
    { { 2, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame193 = {
    SPRITE_FRAME(gSpriteBank01Frame193, SPRITE_TILES_BANK01 + 0x1c1c0),
    { { 2, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame194 = {
    SPRITE_FRAME(gSpriteBank01Frame194, SPRITE_TILES_BANK01 + 0x1c460),
    { { 2, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame195 = {
    SPRITE_FRAME(gSpriteBank01Frame195, SPRITE_TILES_BANK01 + 0x1c6a0),
    { { 2, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame196 = {
    SPRITE_FRAME(gSpriteBank01Frame196, SPRITE_TILES_BANK01 + 0x1c8e0),
    { { 2, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame197 = {
    SPRITE_FRAME(gSpriteBank01Frame197, SPRITE_TILES_BANK01 + 0x1cb20),
    { { 2, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame198 = {
    SPRITE_FRAME(gSpriteBank01Frame198, SPRITE_TILES_BANK01 + 0x1cd40),
    { { 2, -9, 9, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame199 = {
    SPRITE_FRAME(gSpriteBank01Frame199, SPRITE_TILES_BANK01 + 0x1cfa0),
    { { 0, -10, 10, 22 } },
};
const struct sprite_frame_1box gSpriteBank01Frame200 = {
    SPRITE_FRAME(gSpriteBank01Frame200, SPRITE_TILES_BANK01 + 0x1d220),
    { { 0, -10, 10, 22 } },
};
const struct sprite_frame_1box gSpriteBank01Frame201 = {
    SPRITE_FRAME(gSpriteBank01Frame201, SPRITE_TILES_BANK01 + 0x1d480),
    { { 0, -10, 10, 22 } },
};
const struct sprite_frame_1box gSpriteBank01Frame202 = {
    SPRITE_FRAME(gSpriteBank01Frame202, SPRITE_TILES_BANK01 + 0x1d780),
    { { 0, -10, 10, 22 } },
};
const struct sprite_frame_1box gSpriteBank01Frame203 = {
    SPRITE_FRAME(gSpriteBank01Frame203, SPRITE_TILES_BANK01 + 0x1d900),
    { { 0, -10, 10, 22 } },
};
const struct sprite_frame_1box gSpriteBank01Frame204 = {
    SPRITE_FRAME(gSpriteBank01Frame204, SPRITE_TILES_BANK01 + 0x1db20),
    { { 0, -10, 10, 22 } },
};
const struct sprite_frame_1box gSpriteBank01Frame205 = {
    SPRITE_FRAME(gSpriteBank01Frame205, SPRITE_TILES_BANK01 + 0x1dd80),
    { { 0, -10, 10, 22 } },
};
const struct sprite_frame_1box gSpriteBank01Frame206 = {
    SPRITE_FRAME(gSpriteBank01Frame206, SPRITE_TILES_BANK01 + 0x1df60),
    { { 0, -10, 10, 22 } },
};
const struct sprite_frame_1box gSpriteBank01Frame207 = {
    SPRITE_FRAME(gSpriteBank01Frame207, SPRITE_TILES_BANK01 + 0x18300),
    { { -1, -4, 11, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame208 = {
    SPRITE_FRAME(gSpriteBank01Frame208, SPRITE_TILES_BANK01 + 0x18580),
    { { -1, -4, 11, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame209 = {
    SPRITE_FRAME(gSpriteBank01Frame209, SPRITE_TILES_BANK01 + 0x18820),
    { { -1, -4, 11, 19 } },
};
const struct sprite_frame_1box gSpriteBank01Frame210 = {
    SPRITE_FRAME(gSpriteBank01Frame210, SPRITE_TILES_BANK01 + 0x18ac0),
    { { -1, -4, 11, 19 } },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame211 = {
    SPRITE_FRAME(gSpriteBank01Frame211, SPRITE_TILES_BANK01 + 0x1e1a0),
    { { 0, 0, 0, 0 }, { -7, -14, 28, 34 }, { -7, -13, 26, 37 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame212 = {
    SPRITE_FRAME(gSpriteBank01Frame212, SPRITE_TILES_BANK01 + 0x1e440),
    { { 0, 0, 0, 0 }, { -8, -14, 28, 37 }, { -8, -13, 27, 37 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame213 = {
    SPRITE_FRAME(gSpriteBank01Frame213, SPRITE_TILES_BANK01 + 0x1e6e0),
    { { 0, 0, 0, 0 }, { -7, -14, 24, 34 }, { -7, -13, 23, 37 } },
    { 0, 0 },
};
const struct sprite_frame_3box_anchor gSpriteBank01Frame214 = {
    SPRITE_FRAME(gSpriteBank01Frame214, SPRITE_TILES_BANK01 + 0x1e960),
    { { 0, 0, 0, 0 }, { -7, -13, 29, 32 }, { -6, -12, 26, 36 } },
    { 0, 0 },
};
const struct sprite_frame_1box gSpriteBank01Frame215 = {
    SPRITE_FRAME(gSpriteBank01Frame215, SPRITE_TILES_BANK01 + 0x1ebe0),
    { { -2, -10, 10, 25 } },
};
const struct sprite_frame_1box gSpriteBank01Frame216 = {
    SPRITE_FRAME(gSpriteBank01Frame216, SPRITE_TILES_BANK01 + 0x1eee0),
    { { -2, -10, 10, 25 } },
};
const struct sprite_frame_1box gSpriteBank01Frame217 = {
    SPRITE_FRAME(gSpriteBank01Frame217, SPRITE_TILES_BANK01 + 0x00480),
    { { 1, -1, 9, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame218 = {
    SPRITE_FRAME(gSpriteBank01Frame218, SPRITE_TILES_BANK01 + 0x00720),
    { { 1, -1, 9, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame219 = {
    SPRITE_FRAME(gSpriteBank01Frame219, SPRITE_TILES_BANK01 + 0x009c0),
    { { 1, -1, 9, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame220 = {
    SPRITE_FRAME(gSpriteBank01Frame220, SPRITE_TILES_BANK01 + 0x00c60),
    { { 1, -1, 9, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame221 = {
    SPRITE_FRAME(gSpriteBank01Frame221, SPRITE_TILES_BANK01 + 0x1f0c0),
    { { 1, -1, 9, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame222 = {
    SPRITE_FRAME(gSpriteBank01Frame222, SPRITE_TILES_BANK01 + 0x1f340),
    { { 1, -1, 9, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame223 = {
    SPRITE_FRAME(gSpriteBank01Frame223, SPRITE_TILES_BANK01 + 0x1f5c0),
    { { 1, -1, 9, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame224 = {
    SPRITE_FRAME(gSpriteBank01Frame224, SPRITE_TILES_BANK01 + 0x1f840),
    { { 1, -1, 9, 20 } },
};
const struct sprite_frame_1box gSpriteBank01Frame225 = {
    SPRITE_FRAME(gSpriteBank01Frame225, SPRITE_TILES_BANK01 + 0x1fa80),
    { { -6, -2, 13, 4 } },
};
const struct sprite_frame gSpriteBank01Frame226 = SPRITE_FRAME(gSpriteBank01Frame226, SPRITE_TILES_BANK01 + 0x1fac0);
const struct sprite_frame gSpriteBank01Frame227 = SPRITE_FRAME(gSpriteBank01Frame227, SPRITE_TILES_BANK01 + 0x1fee0);
const struct sprite_frame gSpriteBank01Frame228 = SPRITE_FRAME(gSpriteBank01Frame228, SPRITE_TILES_BANK01 + 0x20300);
const struct sprite_frame gSpriteBank01Frame229 = SPRITE_FRAME(gSpriteBank01Frame229, SPRITE_TILES_BANK01 + 0x20720);
const struct sprite_frame gSpriteBank01Frame230 = SPRITE_FRAME(gSpriteBank01Frame230, SPRITE_TILES_BANK01 + 0x20980);
const struct sprite_frame gSpriteBank01Frame231 = SPRITE_FRAME(gSpriteBank01Frame231, SPRITE_TILES_BANK01 + 0x20c20);
const struct sprite_frame gSpriteBank01Frame232 = SPRITE_FRAME(gSpriteBank01Frame232, SPRITE_TILES_BANK01 + 0x20e80);
const struct sprite_frame gSpriteBank01Frame233 = SPRITE_FRAME(gSpriteBank01Frame233, SPRITE_TILES_BANK01 + 0x212a0);
const struct sprite_frame gSpriteBank01Frame234 = SPRITE_FRAME(gSpriteBank01Frame234, SPRITE_TILES_BANK01 + 0x216c0);
const struct sprite_frame gSpriteBank01Frame235 = SPRITE_FRAME(gSpriteBank01Frame235, SPRITE_TILES_BANK01 + 0x21a00);
const struct sprite_frame gSpriteBank01Frame236 = SPRITE_FRAME(gSpriteBank01Frame236, SPRITE_TILES_BANK01 + 0x21c80);
const struct sprite_frame gSpriteBank01Frame237 = SPRITE_FRAME(gSpriteBank01Frame237, SPRITE_TILES_BANK01 + 0x221c0);
const struct sprite_frame gSpriteBank01Frame238 = SPRITE_FRAME(gSpriteBank01Frame238, SPRITE_TILES_BANK01 + 0x229c0);
const struct sprite_frame gSpriteBank01Frame239 = SPRITE_FRAME(gSpriteBank01Frame239, SPRITE_TILES_BANK01 + 0x231c0);
const struct sprite_frame gSpriteBank01Frame240 = SPRITE_FRAME(gSpriteBank01Frame240, SPRITE_TILES_BANK01 + 0x239c0);
const struct sprite_frame gSpriteBank01Frame241 = SPRITE_FRAME(gSpriteBank01Frame241, SPRITE_TILES_BANK01 + 0x23d00);
const struct sprite_frame gSpriteBank01Frame242 = SPRITE_FRAME(gSpriteBank01Frame242, SPRITE_TILES_BANK01 + 0x24040);
const struct sprite_frame gSpriteBank01Frame243 = SPRITE_FRAME(gSpriteBank01Frame243, SPRITE_TILES_BANK01 + 0x24380);
const struct sprite_frame gSpriteBank01Frame244 = SPRITE_FRAME(gSpriteBank01Frame244, SPRITE_TILES_BANK01 + 0x246c0);
const struct sprite_frame gSpriteBank01Frame245 = SPRITE_FRAME(gSpriteBank01Frame245, SPRITE_TILES_BANK01 + 0x24a80);
const struct sprite_frame gSpriteBank01Frame246 = SPRITE_FRAME(gSpriteBank01Frame246, SPRITE_TILES_BANK01 + 0x24d40);
const struct sprite_frame gSpriteBank01Frame247 = SPRITE_FRAME(gSpriteBank01Frame247, SPRITE_TILES_BANK01 + 0x25140);
const struct sprite_frame gSpriteBank01Frame248 = SPRITE_FRAME(gSpriteBank01Frame248, SPRITE_TILES_BANK01 + 0x256e0);
const struct sprite_frame gSpriteBank01Frame249 = SPRITE_FRAME(gSpriteBank01Frame249, SPRITE_TILES_BANK01 + 0x25ee0);
const struct sprite_frame gSpriteBank01Frame250 = SPRITE_FRAME(gSpriteBank01Frame250, SPRITE_TILES_BANK01 + 0x266e0);
const struct sprite_frame gSpriteBank01Frame251 = SPRITE_FRAME(gSpriteBank01Frame251, SPRITE_TILES_BANK01 + 0x26c00);
const struct sprite_frame gSpriteBank01Frame252 = SPRITE_FRAME(gSpriteBank01Frame252, SPRITE_TILES_BANK01 + 0x27140);
const struct sprite_frame_1box gSpriteBank01Frame253 = {
    SPRITE_FRAME(gSpriteBank01Frame253, SPRITE_TILES_BANK01 + 0x276c0),
    { { -22, -17, 45, 34 } },
};
const struct sprite_frame_1box gSpriteBank01Frame254 = {
    SPRITE_FRAME(gSpriteBank01Frame254, SPRITE_TILES_BANK01 + 0x279a0),
    { { -20, -16, 42, 33 } },
};
const struct sprite_frame_1box gSpriteBank01Frame255 = {
    SPRITE_FRAME(gSpriteBank01Frame255, SPRITE_TILES_BANK01 + 0x27c80),
    { { -21, -17, 43, 32 } },
};
const struct sprite_frame_1box gSpriteBank01Frame256 = {
    SPRITE_FRAME(gSpriteBank01Frame256, SPRITE_TILES_BANK01 + 0x27f00),
    { { -21, -17, 43, 32 } },
};
const struct sprite_frame_1box gSpriteBank01Frame257 = {
    SPRITE_FRAME(gSpriteBank01Frame257, SPRITE_TILES_BANK01 + 0x28220),
    { { -21, -10, 43, 33 } },
};
const struct sprite_frame_1box gSpriteBank01Frame258 = {
    SPRITE_FRAME(gSpriteBank01Frame258, SPRITE_TILES_BANK01 + 0x284a0),
    { { -22, -10, 45, 34 } },
};
const struct sprite_frame_1box gSpriteBank01Frame259 = {
    SPRITE_FRAME(gSpriteBank01Frame259, SPRITE_TILES_BANK01 + 0x28720),
    { { -22, -10, 45, 32 } },
};
const struct sprite_frame_1box gSpriteBank01Frame260 = {
    SPRITE_FRAME(gSpriteBank01Frame260, SPRITE_TILES_BANK01 + 0x289e0),
    { { -22, -10, 46, 32 } },
};
const struct sprite_frame_1box gSpriteBank01Frame261 = {
    SPRITE_FRAME(gSpriteBank01Frame261, SPRITE_TILES_BANK01 + 0x28cc0),
    { { -22, -12, 47, 32 } },
};
const struct sprite_frame_1box gSpriteBank01Frame262 = {
    SPRITE_FRAME(gSpriteBank01Frame262, SPRITE_TILES_BANK01 + 0x290e0),
    { { -22, -14, 47, 34 } },
};
const struct sprite_frame_1box gSpriteBank01Frame263 = {
    SPRITE_FRAME(gSpriteBank01Frame263, SPRITE_TILES_BANK01 + 0x29500),
    { { -22, -16, 47, 35 } },
};
const struct sprite_frame_1box gSpriteBank01Frame264 = {
    SPRITE_FRAME(gSpriteBank01Frame264, SPRITE_TILES_BANK01 + 0x29980),
    { { -21, -15, 46, 34 } },
};
const struct sprite_frame_1box gSpriteBank01Frame265 = {
    SPRITE_FRAME(gSpriteBank01Frame265, SPRITE_TILES_BANK01 + 0x29cc0),
    { { -21, -15, 46, 33 } },
};
const struct sprite_frame_1box gSpriteBank01Frame266 = {
    SPRITE_FRAME(gSpriteBank01Frame266, SPRITE_TILES_BANK01 + 0x2a020),
    { { -20, -15, 44, 33 } },
};
const struct sprite_frame_1box gSpriteBank01Frame267 = {
    SPRITE_FRAME(gSpriteBank01Frame267, SPRITE_TILES_BANK01 + 0x2a360),
    { { -19, -9, 43, 27 } },
};
const struct sprite_frame_1box gSpriteBank01Frame268 = {
    SPRITE_FRAME(gSpriteBank01Frame268, SPRITE_TILES_BANK01 + 0x2a600),
    { { -19, -9, 43, 28 } },
};
const struct sprite_frame_1box gSpriteBank01Frame269 = {
    SPRITE_FRAME(gSpriteBank01Frame269, SPRITE_TILES_BANK01 + 0x2a860),
    { { -18, -9, 42, 29 } },
};
const struct sprite_frame_1box gSpriteBank01Frame270 = {
    SPRITE_FRAME(gSpriteBank01Frame270, SPRITE_TILES_BANK01 + 0x2ab00),
    { { -18, -10, 42, 29 } },
};
const struct sprite_frame_1box gSpriteBank01Frame271 = {
    SPRITE_FRAME(gSpriteBank01Frame271, SPRITE_TILES_BANK01 + 0x2ada0),
    { { -18, -10, 42, 29 } },
};
const struct sprite_frame_1box gSpriteBank01Frame272 = {
    SPRITE_FRAME(gSpriteBank01Frame272, SPRITE_TILES_BANK01 + 0x2b000),
    { { -18, -13, 41, 32 } },
};
const struct sprite_frame_1box gSpriteBank01Frame273 = {
    SPRITE_FRAME(gSpriteBank01Frame273, SPRITE_TILES_BANK01 + 0x2b2c0),
    { { -21, -15, 44, 33 } },
};

const struct sprite_piece_pos gSpriteBank01Frame000Pos[3] = { { -6, -13 }, { -12, 19 }, { 20, 19 } };
const struct sprite_piece_pos gSpriteBank01Frame001Pos[4] = { { -6, -17 }, { 10, -6 }, { 10, 10 }, { -4, 15 } };
const struct sprite_piece_pos gSpriteBank01Frame002Pos[3] = { { -20, -21 }, { 12, 3 }, { 0, 11 } };
const struct sprite_piece_pos gSpriteBank01Frame003Pos[3] = { { -20, -21 }, { 12, 4 }, { 1, 11 } };
const struct sprite_piece_pos gSpriteBank01Frame004Pos[3] = { { -17, -18 }, { 15, 8 }, { 2, 14 } };
const struct sprite_piece_pos gSpriteBank01Frame005Pos[3] = { { -19, -11 }, { 13, 6 }, { 7, 21 } };
const struct sprite_piece_pos gSpriteBank01Frame006Pos[5] = { { -18, -14 }, { 14, -2 }, { 14, 14 }, { -2, 18 }, { 14, 18 } };
const struct sprite_piece_pos gSpriteBank01Frame007Pos[4] = { { -17, -14 }, { 15, 4 }, { -4, 18 }, { 12, 18 } };
const struct sprite_piece_pos gSpriteBank01Frame008Pos[4] = { { -18, -17 }, { 14, -3 }, { 14, 13 }, { -14, 15 } };
const struct sprite_piece_pos gSpriteBank01Frame009Pos[3] = { { -10, -18 }, { 22, -4 }, { -9, 14 } };
const struct sprite_piece_pos gSpriteBank01Frame010Pos[3] = { { -20, -15 }, { -4, 17 }, { 4, 18 } };
const struct sprite_piece_pos gSpriteBank01Frame011Pos[3] = { { -20, -18 }, { 12, 3 }, { 0, 14 } };
const struct sprite_piece_pos gSpriteBank01Frame012Pos[2] = { { -16, -21 }, { 0, 11 } };
const struct sprite_piece_pos gSpriteBank01Frame013Pos[1] = { { -12, -12 } };
const struct sprite_piece_pos gSpriteBank01Frame014Pos[3] = { { -13, -11 }, { 19, 7 }, { 3, 21 } };
const struct sprite_piece_pos gSpriteBank01Frame015Pos[3] = { { -25, -21 }, { 7, -5 }, { 0, 11 } };
const struct sprite_piece_pos gSpriteBank01Frame016Pos[2] = { { -12, -21 }, { 2, 11 } };
const struct sprite_piece_pos gSpriteBank01Frame017Pos[3] = { { -4, -21 }, { -5, 11 }, { 11, 15 } };
const struct sprite_piece_pos gSpriteBank01Frame018Pos[2] = { { -6, -17 }, { -8, 15 } };
const struct sprite_piece_pos gSpriteBank01Frame019Pos[3] = { { -15, -19 }, { 17, 3 }, { -6, 13 } };
const struct sprite_piece_pos gSpriteBank01Frame020Pos[3] = { { -15, -16 }, { 17, -17 }, { -14, 15 } };
const struct sprite_piece_pos gSpriteBank01Frame021Pos[4] = { { -22, -8 }, { 10, -9 }, { 18, -5 }, { 0, 23 } };
const struct sprite_piece_pos gSpriteBank01Frame022Pos[4] = { { -22, -9 }, { 10, -10 }, { 18, -8 }, { 2, 22 } };
const struct sprite_piece_pos gSpriteBank01Frame023Pos[4] = { { -19, -10 }, { 13, -11 }, { 21, -7 }, { -3, 21 } };
const struct sprite_piece_pos gSpriteBank01Frame024Pos[3] = { { -12, -10 }, { 20, -3 }, { -9, 22 } };
const struct sprite_piece_pos gSpriteBank01Frame025Pos[3] = { { -17, -9 }, { 15, -7 }, { -1, 23 } };
const struct sprite_piece_pos gSpriteBank01Frame026Pos[3] = { { -17, -11 }, { 15, -9 }, { -4, 21 } };
const struct sprite_piece_pos gSpriteBank01Frame027Pos[3] = { { -14, -12 }, { 18, -10 }, { -9, 20 } };
const struct sprite_piece_pos gSpriteBank01Frame028Pos[2] = { { -18, -10 }, { 14, -7 } };
const struct sprite_piece_pos gSpriteBank01Frame029Pos[4] = { { -22, -8 }, { 10, -9 }, { 18, -5 }, { 0, 23 } };
const struct sprite_piece_pos gSpriteBank01Frame030Pos[4] = { { -22, -9 }, { 10, -10 }, { 18, -8 }, { 2, 22 } };
const struct sprite_piece_pos gSpriteBank01Frame031Pos[4] = { { -19, -10 }, { 13, -11 }, { 21, -7 }, { -3, 21 } };
const struct sprite_piece_pos gSpriteBank01Frame032Pos[3] = { { -12, -10 }, { 20, -3 }, { -9, 22 } };
const struct sprite_piece_pos gSpriteBank01Frame033Pos[4] = { { -18, -10 }, { 14, -11 }, { 22, -7 }, { -7, 21 } };
const struct sprite_piece_pos gSpriteBank01Frame034Pos[3] = { { -15, -13 }, { 17, -10 }, { -12, 19 } };
const struct sprite_piece_pos gSpriteBank01Frame035Pos[4] = { { -12, -14 }, { 17, -10 }, { 18, 10 }, { -15, 18 } };
const struct sprite_piece_pos gSpriteBank01Frame036Pos[3] = { { -12, -10 }, { 20, 17 }, { -7, 22 } };
const struct sprite_piece_pos gSpriteBank01Frame037Pos[2] = { { -7, -8 }, { -9, 24 } };
const struct sprite_piece_pos gSpriteBank01Frame038Pos[2] = { { -9, -9 }, { -8, 23 } };
const struct sprite_piece_pos gSpriteBank01Frame039Pos[2] = { { -9, -11 }, { -10, 21 } };
const struct sprite_piece_pos gSpriteBank01Frame040Pos[1] = { { -12, -12 } };
const struct sprite_piece_pos gSpriteBank01Frame041Pos[4] = { { -12, -15 }, { 19, 0 }, { -1, 17 }, { 7, 17 } };
const struct sprite_piece_pos gSpriteBank01Frame042Pos[2] = { { -12, -10 }, { 8, 22 } };
const struct sprite_piece_pos gSpriteBank01Frame043Pos[3] = { { -5, -14 }, { 11, -13 }, { -5, 18 } };
const struct sprite_piece_pos gSpriteBank01Frame044Pos[3] = { { -12, -14 }, { 6, 18 }, { 22, 20 } };
const struct sprite_piece_pos gSpriteBank01Frame045Pos[2] = { { -12, -10 }, { 11, 22 } };
const struct sprite_piece_pos gSpriteBank01Frame046Pos[4] = { { -11, -17 }, { 17, -12 }, { 17, 4 }, { -15, 15 } };
const struct sprite_piece_pos gSpriteBank01Frame047Pos[4] = { { -15, -17 }, { 17, -17 }, { 0, 15 }, { 16, 15 } };
const struct sprite_piece_pos gSpriteBank01Frame048Pos[2] = { { -16, -19 }, { -8, 13 } };
const struct sprite_piece_pos gSpriteBank01Frame049Pos[3] = { { -19, -18 }, { 13, -16 }, { -9, 14 } };
const struct sprite_piece_pos gSpriteBank01Frame050Pos[2] = { { -13, -19 }, { -7, 13 } };
const struct sprite_piece_pos gSpriteBank01Frame051Pos[3] = { { -8, -19 }, { 8, -3 }, { -7, 13 } };
const struct sprite_piece_pos gSpriteBank01Frame052Pos[2] = { { -13, -19 }, { -9, 13 } };
const struct sprite_piece_pos gSpriteBank01Frame053Pos[3] = { { -13, -19 }, { -9, 13 }, { 7, 15 } };
const struct sprite_piece_pos gSpriteBank01Frame054Pos[4] = { { -8, -19 }, { 8, -2 }, { -8, 13 }, { 8, 13 } };
const struct sprite_piece_pos gSpriteBank01Frame055Pos[3] = { { -10, -19 }, { 6, -19 }, { -6, 13 } };
const struct sprite_piece_pos gSpriteBank01Frame056Pos[2] = { { -20, -13 }, { 12, 1 } };
const struct sprite_piece_pos gSpriteBank01Frame057Pos[4] = { { -19, -16 }, { 12, -1 }, { -20, 16 }, { 12, 16 } };
const struct sprite_piece_pos gSpriteBank01Frame058Pos[3] = { { -20, -10 }, { 12, -1 }, { 20, 5 } };
const struct sprite_piece_pos gSpriteBank01Frame059Pos[3] = { { -20, -4 }, { 12, 1 }, { 6, 12 } };
const struct sprite_piece_pos gSpriteBank01Frame060Pos[2] = { { -20, -9 }, { 12, 2 } };
const struct sprite_piece_pos gSpriteBank01Frame061Pos[3] = { { -20, -10 }, { 12, 0 }, { 20, 9 } };
const struct sprite_piece_pos gSpriteBank01Frame062Pos[5] = { { -20, -4 }, { 12, -2 }, { 20, 4 }, { -1, 12 }, { 15, 12 } };
const struct sprite_piece_pos gSpriteBank01Frame063Pos[4] = { { -20, -8 }, { 12, 0 }, { -20, 8 }, { 12, 8 } };
const struct sprite_piece_pos gSpriteBank01Frame064Pos[3] = { { -20, -17 }, { 12, 2 }, { 2, 15 } };
const struct sprite_piece_pos gSpriteBank01Frame065Pos[4] = { { -20, -19 }, { 12, 1 }, { -20, 13 }, { 12, 13 } };
const struct sprite_piece_pos gSpriteBank01Frame066Pos[4] = { { -20, -14 }, { 12, 1 }, { 7, 18 }, { 15, 18 } };
const struct sprite_piece_pos gSpriteBank01Frame067Pos[2] = { { -19, -7 }, { 13, 4 } };
const struct sprite_piece_pos gSpriteBank01Frame068Pos[3] = { { -20, -13 }, { 12, 3 }, { 3, 19 } };
const struct sprite_piece_pos gSpriteBank01Frame069Pos[4] = { { -20, -14 }, { 12, 1 }, { 12, 17 }, { 5, 18 } };
const struct sprite_piece_pos gSpriteBank01Frame070Pos[2] = { { -20, -9 }, { 12, 1 } };
const struct sprite_piece_pos gSpriteBank01Frame071Pos[3] = { { -20, -13 }, { 12, 1 }, { 12, 17 } };
const struct sprite_piece_pos gSpriteBank01Frame072Pos[3] = { { -16, -22 }, { -2, 10 }, { 14, 10 } };
const struct sprite_piece_pos gSpriteBank01Frame073Pos[4] = { { -18, -21 }, { 14, 8 }, { -1, 11 }, { 15, 11 } };
const struct sprite_piece_pos gSpriteBank01Frame074Pos[2] = { { -13, -20 }, { 0, 12 } };
const struct sprite_piece_pos gSpriteBank01Frame075Pos[2] = { { -15, -15 }, { -2, 17 } };
const struct sprite_piece_pos gSpriteBank01Frame076Pos[4] = { { -20, -17 }, { 12, 6 }, { -4, 15 }, { 12, 15 } };
const struct sprite_piece_pos gSpriteBank01Frame077Pos[3] = { { -20, -16 }, { 12, 6 }, { -2, 16 } };
const struct sprite_piece_pos gSpriteBank01Frame078Pos[3] = { { -15, -15 }, { -1, 17 }, { 15, 17 } };
const struct sprite_piece_pos gSpriteBank01Frame079Pos[2] = { { -11, -18 }, { -2, 14 } };
const struct sprite_piece_pos gSpriteBank01Frame080Pos[3] = { { -11, -21 }, { -4, 11 }, { 12, 11 } };
const struct sprite_piece_pos gSpriteBank01Frame081Pos[3] = { { -14, -19 }, { -5, 13 }, { 11, 13 } };
const struct sprite_piece_pos gSpriteBank01Frame082Pos[4] = { { -8, -20 }, { 8, -18 }, { -4, 12 }, { 12, 12 } };
const struct sprite_piece_pos gSpriteBank01Frame083Pos[4] = { { -9, -18 }, { 7, -11 }, { -4, 14 }, { 12, 14 } };
const struct sprite_piece_pos gSpriteBank01Frame084Pos[4] = { { -18, -14 }, { 14, -16 }, { -7, 16 }, { 9, 16 } };
const struct sprite_piece_pos gSpriteBank01Frame085Pos[4] = { { -17, -14 }, { 15, -15 }, { -6, 17 }, { 10, 17 } };
const struct sprite_piece_pos gSpriteBank01Frame086Pos[4] = { { -9, -16 }, { 7, -16 }, { -5, 16 }, { 11, 16 } };
const struct sprite_piece_pos gSpriteBank01Frame087Pos[4] = { { -8, -21 }, { 8, -3 }, { -4, 11 }, { 12, 11 } };
const struct sprite_piece_pos gSpriteBank01Frame088Pos[2] = { { -16, -19 }, { -8, 13 } };
const struct sprite_piece_pos gSpriteBank01Frame089Pos[3] = { { -19, -18 }, { 13, -16 }, { -9, 14 } };
const struct sprite_piece_pos gSpriteBank01Frame090Pos[2] = { { -13, -19 }, { -7, 13 } };
const struct sprite_piece_pos gSpriteBank01Frame091Pos[3] = { { -8, -19 }, { 8, -3 }, { -7, 13 } };
const struct sprite_piece_pos gSpriteBank01Frame092Pos[3] = { { -9, -19 }, { -2, 13 }, { 14, 20 } };
const struct sprite_piece_pos gSpriteBank01Frame093Pos[3] = { { -10, -19 }, { -4, 13 }, { 12, 13 } };
const struct sprite_piece_pos gSpriteBank01Frame094Pos[3] = { { -9, -19 }, { -3, 13 }, { 13, 14 } };
const struct sprite_piece_pos gSpriteBank01Frame095Pos[3] = { { -8, -19 }, { -4, 13 }, { 12, 13 } };
const struct sprite_piece_pos gSpriteBank01Frame096Pos[4] = { { -12, -19 }, { 20, -13 }, { -5, 13 }, { 11, 13 } };
const struct sprite_piece_pos gSpriteBank01Frame097Pos[3] = { { -6, -19 }, { 10, -3 }, { -4, 13 } };
const struct sprite_piece_pos gSpriteBank01Frame098Pos[2] = { { -11, -19 }, { -4, 13 } };
const struct sprite_piece_pos gSpriteBank01Frame099Pos[3] = { { -6, -19 }, { 10, -19 }, { -3, 13 } };
const struct sprite_piece_pos gSpriteBank01Frame100Pos[3] = { { -20, -13 }, { 12, -6 }, { 20, 5 } };
const struct sprite_piece_pos gSpriteBank01Frame101Pos[3] = { { -17, -15 }, { 15, -5 }, { 23, 5 } };
const struct sprite_piece_pos gSpriteBank01Frame102Pos[4] = { { -21, -8 }, { 11, -6 }, { -14, 8 }, { 18, 8 } };
const struct sprite_piece_pos gSpriteBank01Frame103Pos[5] = { { -22, -7 }, { 10, -5 }, { -20, 9 }, { 12, 9 }, { 20, 9 } };
const struct sprite_piece_pos gSpriteBank01Frame104Pos[4] = { { -15, -15 }, { 16, -2 }, { 16, 14 }, { -16, 17 } };
const struct sprite_piece_pos gSpriteBank01Frame105Pos[4] = { { -15, -16 }, { 16, -4 }, { 24, 4 }, { -16, 16 } };
const struct sprite_piece_pos gSpriteBank01Frame106Pos[4] = { { -17, -9 }, { 13, -5 }, { -19, 7 }, { 13, 7 } };
const struct sprite_piece_pos gSpriteBank01Frame107Pos[3] = { { -22, -8 }, { 10, -6 }, { -8, 8 } };
const struct sprite_piece_pos gSpriteBank01Frame108Pos[3] = { { -20, -13 }, { 12, -6 }, { 20, 5 } };
const struct sprite_piece_pos gSpriteBank01Frame109Pos[3] = { { -17, -15 }, { 15, -5 }, { 23, 5 } };
const struct sprite_piece_pos gSpriteBank01Frame110Pos[4] = { { -21, -8 }, { 11, -6 }, { -14, 8 }, { 18, 8 } };
const struct sprite_piece_pos gSpriteBank01Frame111Pos[5] = { { -22, -7 }, { 10, -5 }, { -20, 9 }, { 12, 9 }, { 20, 9 } };
const struct sprite_piece_pos gSpriteBank01Frame112Pos[5] = { { -25, -9 }, { 7, -5 }, { 23, -5 }, { -17, 7 }, { -1, 7 } };
const struct sprite_piece_pos gSpriteBank01Frame113Pos[3] = { { -17, -13 }, { 15, -8 }, { 23, -6 } };
const struct sprite_piece_pos gSpriteBank01Frame114Pos[2] = { { -13, -15 }, { 19, -10 } };
const struct sprite_piece_pos gSpriteBank01Frame115Pos[1] = { { -11, -14 } };
const struct sprite_piece_pos gSpriteBank01Frame116Pos[1] = { { -8, -19 } };
const struct sprite_piece_pos gSpriteBank01Frame117Pos[2] = { { -18, -14 }, { 14, -8 } };
const struct sprite_piece_pos gSpriteBank01Frame118Pos[3] = { { -20, -10 }, { 12, -7 }, { 20, -3 } };
const struct sprite_piece_pos gSpriteBank01Frame119Pos[2] = { { -17, -12 }, { 15, 3 } };
const struct sprite_piece_pos gSpriteBank01Frame120Pos[1] = { { -14, -15 } };
const struct sprite_piece_pos gSpriteBank01Frame121Pos[2] = { { -15, -11 }, { 17, 6 } };
const struct sprite_piece_pos gSpriteBank01Frame122Pos[2] = { { -14, -11 }, { 18, 6 } };
const struct sprite_piece_pos gSpriteBank01Frame123Pos[4] = { { -20, -13 }, { 12, -6 }, { 20, 2 }, { 0, 19 } };
const struct sprite_piece_pos gSpriteBank01Frame124Pos[4] = { { -25, -8 }, { 7, -9 }, { 23, -10 }, { -8, 6 } };
const struct sprite_piece_pos gSpriteBank01Frame125Pos[3] = { { -20, -12 }, { 12, -17 }, { 20, -8 } };
const struct sprite_piece_pos gSpriteBank01Frame126Pos[2] = { { -16, -18 }, { 16, -1 } };
const struct sprite_piece_pos gSpriteBank01Frame127Pos[1] = { { -14, -14 } };
const struct sprite_piece_pos gSpriteBank01Frame128Pos[1] = { { -14, -11 } };
const struct sprite_piece_pos gSpriteBank01Frame129Pos[2] = { { -19, -11 }, { 13, -9 } };
const struct sprite_piece_pos gSpriteBank01Frame130Pos[4] = { { -24, -13 }, { 8, -9 }, { 16, -9 }, { 16, 7 } };
const struct sprite_piece_pos gSpriteBank01Frame131Pos[3] = { { -17, -12 }, { 15, -6 }, { -10, 20 } };
const struct sprite_piece_pos gSpriteBank01Frame132Pos[3] = { { -12, -10 }, { 20, 5 }, { -3, 22 } };
const struct sprite_piece_pos gSpriteBank01Frame133Pos[3] = { { -14, -11 }, { 18, 5 }, { 26, 5 } };
const struct sprite_piece_pos gSpriteBank01Frame134Pos[3] = { { -15, -10 }, { 17, 3 }, { 25, 6 } };
const struct sprite_piece_pos gSpriteBank01Frame135Pos[3] = { { -16, -10 }, { 16, -3 }, { 24, 3 } };
const struct sprite_piece_pos gSpriteBank01Frame136Pos[1] = { { -23, -11 } };
const struct sprite_piece_pos gSpriteBank01Frame137Pos[3] = { { -23, -8 }, { 9, -7 }, { 17, -2 } };
const struct sprite_piece_pos gSpriteBank01Frame138Pos[4] = { { -22, -9 }, { 10, -6 }, { -16, 7 }, { 16, 7 } };
const struct sprite_piece_pos gSpriteBank01Frame139Pos[3] = { { -21, -11 }, { 11, -8 }, { 19, -1 } };
const struct sprite_piece_pos gSpriteBank01Frame140Pos[4] = { { -21, -13 }, { 11, -11 }, { 19, -3 }, { 21, 13 } };
const struct sprite_piece_pos gSpriteBank01Frame141Pos[1] = { { -24, -10 } };
const struct sprite_piece_pos gSpriteBank01Frame142Pos[4] = { { -20, -12 }, { 12, -9 }, { 20, 0 }, { 2, 20 } };
const struct sprite_piece_pos gSpriteBank01Frame143Pos[3] = { { -21, -11 }, { 11, -8 }, { 19, -1 } };
const struct sprite_piece_pos gSpriteBank01Frame144Pos[1] = { { -23, -11 } };
const struct sprite_piece_pos gSpriteBank01Frame145Pos[3] = { { -22, -9 }, { 10, -7 }, { 18, -2 } };
const struct sprite_piece_pos gSpriteBank01Frame146Pos[1] = { { -23, -8 } };
const struct sprite_piece_pos gSpriteBank01Frame147Pos[1] = { { -23, -7 } };
const struct sprite_piece_pos gSpriteBank01Frame148Pos[1] = { { -23, -7 } };
const struct sprite_piece_pos gSpriteBank01Frame149Pos[1] = { { -23, -6 } };
const struct sprite_piece_pos gSpriteBank01Frame150Pos[4] = { { -23, -6 }, { 9, -5 }, { 17, -3 }, { 17, 13 } };
const struct sprite_piece_pos gSpriteBank01Frame151Pos[4] = { { -23, -5 }, { 9, -5 }, { 17, -2 }, { 17, 14 } };
const struct sprite_piece_pos gSpriteBank01Frame152Pos[1] = { { -24, -5 } };
const struct sprite_piece_pos gSpriteBank01Frame153Pos[1] = { { -24, -6 } };
const struct sprite_piece_pos gSpriteBank01Frame154Pos[1] = { { -24, -6 } };
const struct sprite_piece_pos gSpriteBank01Frame155Pos[1] = { { -24, -7 } };
const struct sprite_piece_pos gSpriteBank01Frame156Pos[4] = { { -23, -9 }, { 9, -6 }, { 17, -1 }, { 19, 15 } };
const struct sprite_piece_pos gSpriteBank01Frame157Pos[4] = { { -22, -11 }, { 10, -6 }, { 18, -1 }, { 3, 21 } };
const struct sprite_piece_pos gSpriteBank01Frame158Pos[4] = { { -22, -11 }, { 10, -7 }, { 18, -2 }, { 4, 21 } };
const struct sprite_piece_pos gSpriteBank01Frame159Pos[3] = { { -20, -13 }, { 12, -6 }, { 20, 5 } };
const struct sprite_piece_pos gSpriteBank01Frame160Pos[3] = { { -17, -16 }, { 15, -4 }, { 16, 12 } };
const struct sprite_piece_pos gSpriteBank01Frame161Pos[3] = { { -19, -15 }, { 13, -6 }, { 21, 4 } };
const struct sprite_piece_pos gSpriteBank01Frame162Pos[3] = { { -21, -13 }, { 11, -7 }, { 19, -2 } };
const struct sprite_piece_pos gSpriteBank01Frame163Pos[3] = { { -22, -10 }, { 10, -7 }, { 18, -2 } };
const struct sprite_piece_pos gSpriteBank01Frame164Pos[3] = { { -23, -8 }, { 9, -7 }, { 17, -2 } };
const struct sprite_piece_pos gSpriteBank01Frame165Pos[3] = { { -23, -8 }, { 9, -7 }, { 17, -2 } };
const struct sprite_piece_pos gSpriteBank01Frame166Pos[3] = { { -22, -10 }, { 10, -7 }, { 18, -2 } };
const struct sprite_piece_pos gSpriteBank01Frame167Pos[2] = { { -6, -12 }, { -9, 20 } };
const struct sprite_piece_pos gSpriteBank01Frame168Pos[3] = { { -6, -14 }, { -13, 18 }, { 19, 20 } };
const struct sprite_piece_pos gSpriteBank01Frame169Pos[3] = { { -5, -16 }, { -1, 16 }, { 15, 23 } };
const struct sprite_piece_pos gSpriteBank01Frame170Pos[5] = { { -5, -13 }, { 11, -6 }, { 11, 15 }, { 2, 19 }, { 10, 19 } };
const struct sprite_piece_pos gSpriteBank01Frame171Pos[2] = { { -5, -12 }, { -5, 20 } };
const struct sprite_piece_pos gSpriteBank01Frame172Pos[2] = { { -5, -15 }, { -6, 17 } };
const struct sprite_piece_pos gSpriteBank01Frame173Pos[5] = { { -5, -16 }, { 11, -9 }, { 11, 7 }, { 2, 16 }, { 10, 16 } };
const struct sprite_piece_pos gSpriteBank01Frame174Pos[4] = { { -6, -13 }, { 10, -11 }, { -4, 19 }, { 12, 19 } };
const struct sprite_piece_pos gSpriteBank01Frame175Pos[3] = { { -22, -7 }, { 10, -7 }, { 18, 1 } };
const struct sprite_piece_pos gSpriteBank01Frame176Pos[3] = { { -20, -10 }, { 12, -7 }, { 20, 1 } };
const struct sprite_piece_pos gSpriteBank01Frame177Pos[4] = { { -21, -7 }, { 11, -7 }, { -11, 9 }, { 22, 9 } };
const struct sprite_piece_pos gSpriteBank01Frame178Pos[5] = { { -17, -6 }, { 13, -6 }, { 21, 0 }, { -19, 10 }, { 14, 10 } };
const struct sprite_piece_pos gSpriteBank01Frame179Pos[4] = { { -17, -10 }, { 15, -5 }, { 15, 11 }, { -12, 22 } };
const struct sprite_piece_pos gSpriteBank01Frame180Pos[3] = { { -16, -11 }, { 16, -5 }, { -12, 21 } };
const struct sprite_piece_pos gSpriteBank01Frame181Pos[5] = { { -17, -7 }, { 15, -6 }, { 23, -1 }, { -17, 9 }, { 15, 9 } };
const struct sprite_piece_pos gSpriteBank01Frame182Pos[3] = { { -22, -7 }, { 10, -7 }, { -15, 9 } };
const struct sprite_piece_pos gSpriteBank01Frame183Pos[3] = { { -23, -7 }, { 9, -8 }, { 17, -3 } };
const struct sprite_piece_pos gSpriteBank01Frame184Pos[4] = { { -22, -8 }, { 10, -9 }, { 18, -4 }, { -2, 23 } };
const struct sprite_piece_pos gSpriteBank01Frame185Pos[3] = { { -21, -8 }, { 11, -9 }, { 19, -5 } };
const struct sprite_piece_pos gSpriteBank01Frame186Pos[2] = { { -15, -8 }, { 17, -4 } };
const struct sprite_piece_pos gSpriteBank01Frame187Pos[3] = { { -18, -7 }, { 14, -6 }, { -6, 25 } };
const struct sprite_piece_pos gSpriteBank01Frame188Pos[4] = { { -17, -8 }, { 15, -7 }, { 23, 4 }, { -7, 24 } };
const struct sprite_piece_pos gSpriteBank01Frame189Pos[3] = { { -16, -9 }, { 16, -9 }, { 24, 1 } };
const struct sprite_piece_pos gSpriteBank01Frame190Pos[3] = { { -19, -8 }, { 13, -7 }, { 21, 3 } };
const struct sprite_piece_pos gSpriteBank01Frame191Pos[3] = { { -18, -10 }, { 14, -9 }, { 5, 22 } };
const struct sprite_piece_pos gSpriteBank01Frame192Pos[4] = { { -20, -12 }, { 12, -11 }, { 5, 20 }, { 13, 24 } };
const struct sprite_piece_pos gSpriteBank01Frame193Pos[4] = { { -13, -13 }, { 17, -11 }, { -15, 19 }, { 1, 19 } };
const struct sprite_piece_pos gSpriteBank01Frame194Pos[2] = { { -8, -11 }, { -8, 21 } };
const struct sprite_piece_pos gSpriteBank01Frame195Pos[3] = { { -14, -10 }, { 18, -3 }, { 3, 22 } };
const struct sprite_piece_pos gSpriteBank01Frame196Pos[3] = { { -15, -12 }, { 17, -5 }, { 0, 20 } };
const struct sprite_piece_pos gSpriteBank01Frame197Pos[2] = { { -9, -13 }, { -6, 19 } };
const struct sprite_piece_pos gSpriteBank01Frame198Pos[3] = { { -13, -11 }, { -14, 21 }, { 2, 21 } };
const struct sprite_piece_pos gSpriteBank01Frame199Pos[2] = { { -13, -11 }, { -14, 21 } };
const struct sprite_piece_pos gSpriteBank01Frame200Pos[3] = { { -17, -13 }, { 15, -6 }, { 8, 19 } };
const struct sprite_piece_pos gSpriteBank01Frame201Pos[2] = { { -5, -15 }, { -11, 17 } };
const struct sprite_piece_pos gSpriteBank01Frame202Pos[3] = { { -5, -12 }, { 11, -12 }, { -3, 20 } };
const struct sprite_piece_pos gSpriteBank01Frame203Pos[2] = { { -10, -11 }, { 6, 21 } };
const struct sprite_piece_pos gSpriteBank01Frame204Pos[3] = { { -11, -14 }, { 3, 18 }, { 11, 25 } };
const struct sprite_piece_pos gSpriteBank01Frame205Pos[4] = { { -5, -15 }, { 11, -15 }, { 11, 7 }, { -5, 17 } };
const struct sprite_piece_pos gSpriteBank01Frame206Pos[3] = { { -8, -12 }, { -9, 20 }, { 7, 20 } };
const struct sprite_piece_pos gSpriteBank01Frame207Pos[2] = { { -6, -12 }, { -9, 20 } };
const struct sprite_piece_pos gSpriteBank01Frame208Pos[3] = { { -6, -14 }, { -13, 18 }, { 19, 20 } };
const struct sprite_piece_pos gSpriteBank01Frame209Pos[3] = { { -5, -16 }, { -1, 16 }, { 15, 23 } };
const struct sprite_piece_pos gSpriteBank01Frame210Pos[5] = { { -5, -13 }, { 11, -6 }, { 11, 15 }, { 2, 19 }, { 10, 19 } };
const struct sprite_piece_pos gSpriteBank01Frame211Pos[3] = { { -9, -15 }, { -4, 17 }, { 12, 21 } };
const struct sprite_piece_pos gSpriteBank01Frame212Pos[3] = { { -10, -15 }, { -2, 17 }, { 14, 18 } };
const struct sprite_piece_pos gSpriteBank01Frame213Pos[2] = { { -9, -15 }, { 0, 17 } };
const struct sprite_piece_pos gSpriteBank01Frame214Pos[2] = { { -8, -14 }, { 0, 18 } };
const struct sprite_piece_pos gSpriteBank01Frame215Pos[2] = { { -9, -14 }, { -11, 18 } };
const struct sprite_piece_pos gSpriteBank01Frame216Pos[4] = { { -6, -13 }, { 10, -6 }, { -4, 19 }, { 12, 19 } };
const struct sprite_piece_pos gSpriteBank01Frame217Pos[3] = { { -20, -21 }, { 12, 3 }, { 0, 11 } };
const struct sprite_piece_pos gSpriteBank01Frame218Pos[3] = { { -20, -21 }, { 12, 4 }, { 1, 11 } };
const struct sprite_piece_pos gSpriteBank01Frame219Pos[3] = { { -17, -18 }, { 15, 8 }, { 2, 14 } };
const struct sprite_piece_pos gSpriteBank01Frame220Pos[3] = { { -19, -11 }, { 13, 6 }, { 7, 21 } };
const struct sprite_piece_pos gSpriteBank01Frame221Pos[3] = { { -20, -16 }, { 12, 5 }, { -1, 16 } };
const struct sprite_piece_pos gSpriteBank01Frame222Pos[3] = { { -20, -16 }, { 12, 4 }, { 1, 16 } };
const struct sprite_piece_pos gSpriteBank01Frame223Pos[3] = { { -19, -13 }, { 13, 5 }, { 2, 19 } };
const struct sprite_piece_pos gSpriteBank01Frame224Pos[2] = { { -15, -17 }, { 1, 15 } };
const struct sprite_piece_pos gSpriteBank01Frame225Pos[1] = { { -6, -2 } };
const struct sprite_piece_pos gSpriteBank01Frame226Pos[2] = { { -16, -28 }, { 16, 3 } };
const struct sprite_piece_pos gSpriteBank01Frame227Pos[2] = { { -16, -28 }, { 16, 3 } };
const struct sprite_piece_pos gSpriteBank01Frame228Pos[2] = { { -16, -28 }, { 16, 3 } };
const struct sprite_piece_pos gSpriteBank01Frame229Pos[3] = { { -15, -23 }, { -3, 13 }, { 5, 16 } };
const struct sprite_piece_pos gSpriteBank01Frame230Pos[3] = { { -20, -30 }, { 12, -7 }, { -7, 2 } };
const struct sprite_piece_pos gSpriteBank01Frame231Pos[3] = { { -27, -35 }, { -14, -3 }, { 3, -3 } };
const struct sprite_piece_pos gSpriteBank01Frame232Pos[2] = { { -33, -39 }, { -1, -12 } };
const struct sprite_piece_pos gSpriteBank01Frame233Pos[2] = { { -38, -44 }, { -6, -16 } };
const struct sprite_piece_pos gSpriteBank01Frame234Pos[4] = { { -43, -49 }, { -9, -21 }, { -38, -17 }, { -6, -17 } };
const struct sprite_piece_pos gSpriteBank01Frame235Pos[5] = { { -47, -50 }, { -9, -25 }, { -7, -25 }, { -42, -18 }, { -6, -18 } };
const struct sprite_piece_pos gSpriteBank01Frame236Pos[5] = { { -50, -50 }, { -5, -50 }, { -69, -18 }, { -7, -18 }, { -5, -18 } };
const struct sprite_piece_pos gSpriteBank01Frame237Pos[1] = { { -65, -51 } };
const struct sprite_piece_pos gSpriteBank01Frame238Pos[1] = { { -57, -51 } };
const struct sprite_piece_pos gSpriteBank01Frame239Pos[1] = { { -57, -51 } };
const struct sprite_piece_pos gSpriteBank01Frame240Pos[4] = { { -54, -50 }, { -58, -18 }, { -16, -18 }, { -10, -18 } };
const struct sprite_piece_pos gSpriteBank01Frame241Pos[4] = { { -54, -50 }, { -59, -18 }, { -16, -17 }, { -11, -17 } };
const struct sprite_piece_pos gSpriteBank01Frame242Pos[4] = { { -55, -49 }, { -60, -17 }, { -16, -15 }, { -12, -17 } };
const struct sprite_piece_pos gSpriteBank01Frame243Pos[4] = { { -55, -47 }, { -60, -15 }, { -16, -14 }, { -12, -15 } };
const struct sprite_piece_pos gSpriteBank01Frame244Pos[1] = { { -25, -15 } };
const struct sprite_piece_pos gSpriteBank01Frame245Pos[3] = { { -23, -15 }, { 9, -8 }, { 17, -7 } };
const struct sprite_piece_pos gSpriteBank01Frame246Pos[1] = { { -24, -13 } };
const struct sprite_piece_pos gSpriteBank01Frame247Pos[4] = { { -21, -25 }, { -25, 7 }, { 7, 7 }, { 23, 7 } };
const struct sprite_piece_pos gSpriteBank01Frame248Pos[1] = { { -25, -30 } };
const struct sprite_piece_pos gSpriteBank01Frame249Pos[1] = { { -23, -26 } };
const struct sprite_piece_pos gSpriteBank01Frame250Pos[3] = { { -22, -22 }, { -19, 10 }, { 13, 10 } };
const struct sprite_piece_pos gSpriteBank01Frame251Pos[4] = { { -21, -23 }, { -20, 9 }, { 12, 9 }, { 20, 9 } };
const struct sprite_piece_pos gSpriteBank01Frame252Pos[3] = { { -20, -24 }, { -22, 8 }, { 10, 8 } };
const struct sprite_piece_pos gSpriteBank01Frame253Pos[4] = { { -22, -17 }, { 10, -9 }, { 18, -6 }, { -18, 15 } };
const struct sprite_piece_pos gSpriteBank01Frame254Pos[4] = { { -20, -16 }, { 12, -10 }, { 20, -6 }, { -19, 16 } };
const struct sprite_piece_pos gSpriteBank01Frame255Pos[4] = { { -21, -17 }, { 11, -11 }, { 19, -7 }, { 6, 15 } };
const struct sprite_piece_pos gSpriteBank01Frame256Pos[4] = { { -21, -17 }, { 11, -16 }, { 19, -7 }, { -20, 15 } };
const struct sprite_piece_pos gSpriteBank01Frame257Pos[4] = { { -21, -10 }, { 11, -10 }, { 19, -6 }, { -10, 22 } };
const struct sprite_piece_pos gSpriteBank01Frame258Pos[4] = { { -22, -10 }, { 10, -10 }, { 18, -6 }, { -5, 22 } };
const struct sprite_piece_pos gSpriteBank01Frame259Pos[4] = { { -22, -9 }, { 10, -10 }, { 18, -6 }, { 0, 22 } };
const struct sprite_piece_pos gSpriteBank01Frame260Pos[4] = { { -22, -9 }, { 10, -10 }, { 18, -7 }, { -1, 22 } };
const struct sprite_piece_pos gSpriteBank01Frame261Pos[2] = { { -22, -12 }, { -12, 20 } };
const struct sprite_piece_pos gSpriteBank01Frame262Pos[2] = { { -22, -14 }, { -12, 18 } };
const struct sprite_piece_pos gSpriteBank01Frame263Pos[2] = { { -21, -16 }, { -13, 16 } };
const struct sprite_piece_pos gSpriteBank01Frame264Pos[4] = { { -21, -15 }, { 11, -9 }, { 19, -5 }, { -14, 17 } };
const struct sprite_piece_pos gSpriteBank01Frame265Pos[5] = { { -18, -15 }, { 11, -9 }, { 19, -5 }, { -21, 17 }, { 11, 17 } };
const struct sprite_piece_pos gSpriteBank01Frame266Pos[5] = { { -20, -15 }, { 12, -10 }, { 20, 2 }, { -20, 17 }, { 12, 17 } };
const struct sprite_piece_pos gSpriteBank01Frame267Pos[3] = { { -19, -9 }, { 13, -9 }, { 21, 1 } };
const struct sprite_piece_pos gSpriteBank01Frame268Pos[3] = { { -18, -9 }, { 14, -7 }, { 22, 2 } };
const struct sprite_piece_pos gSpriteBank01Frame269Pos[3] = { { -18, -9 }, { 14, -8 }, { 22, 1 } };
const struct sprite_piece_pos gSpriteBank01Frame270Pos[3] = { { -18, -10 }, { 14, -8 }, { 22, 0 } };
const struct sprite_piece_pos gSpriteBank01Frame271Pos[3] = { { -18, -10 }, { 14, -9 }, { 22, -3 } };
const struct sprite_piece_pos gSpriteBank01Frame272Pos[4] = { { -18, -11 }, { 14, -13 }, { 22, -4 }, { -18, 19 } };
const struct sprite_piece_pos gSpriteBank01Frame273Pos[4] = { { -21, -14 }, { 11, -15 }, { 19, -5 }, { -18, 17 } };

const u8 gSpriteBank01Frame000Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame001Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame002Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame003Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame004Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame005Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame006Pieces[5] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame007Pieces[4] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame008Pieces[4] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank01Frame009Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank01Frame010Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame011Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank01Frame012Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame013Pieces[1] = { SPRITE_PIECE(0, 2) };
const u8 gSpriteBank01Frame014Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame015Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame016Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame017Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame018Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame019Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank01Frame020Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank01Frame021Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame022Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame023Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame024Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame025Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame026Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame027Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame028Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame029Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame030Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame031Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame032Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame033Pieces[4] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame034Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank01Frame035Pieces[4] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank01Frame036Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame037Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame038Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame039Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame040Pieces[1] = { SPRITE_PIECE(0, 2) };
const u8 gSpriteBank01Frame041Pieces[4] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame042Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame043Pieces[3] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame044Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame045Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame046Pieces[4] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank01Frame047Pieces[4] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame048Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank01Frame049Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank01Frame050Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame051Pieces[3] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank01Frame052Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank01Frame053Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame054Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame055Pieces[3] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank01Frame056Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame057Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame058Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame059Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank01Frame060Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame061Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame062Pieces[5] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame063Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame064Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank01Frame065Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame066Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame067Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame068Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame069Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame070Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank01Frame071Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame072Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame073Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame074Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame075Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank01Frame076Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame077Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame078Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame079Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame080Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame081Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame082Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame083Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame084Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame085Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame086Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame087Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame088Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank01Frame089Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank01Frame090Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame091Pieces[3] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank01Frame092Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame093Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame094Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame095Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame096Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame097Pieces[3] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame098Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame099Pieces[3] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank01Frame100Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame101Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame102Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame103Pieces[5] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame104Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame105Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame106Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank01Frame107Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank01Frame108Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame109Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame110Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame111Pieces[5] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame112Pieces[5] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame113Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame114Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame115Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank01Frame116Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank01Frame117Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank01Frame118Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame119Pieces[2] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame120Pieces[1] = { SPRITE_PIECE(0, 2) };
const u8 gSpriteBank01Frame121Pieces[2] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame122Pieces[2] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame123Pieces[4] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame124Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank01Frame125Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame126Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame127Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank01Frame128Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank01Frame129Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame130Pieces[4] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame131Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame132Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame133Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame134Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame135Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame136Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank01Frame137Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame138Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame139Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame140Pieces[4] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame141Pieces[1] = { SPRITE_PIECE(0, 7) };
const u8 gSpriteBank01Frame142Pieces[4] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame143Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame144Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank01Frame145Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame146Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank01Frame147Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank01Frame148Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank01Frame149Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank01Frame150Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame151Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame152Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank01Frame153Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank01Frame154Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank01Frame155Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank01Frame156Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame157Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame158Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame159Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame160Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame161Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame162Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame163Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame164Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame165Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame166Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame167Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank01Frame168Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame169Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame170Pieces[5] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame171Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank01Frame172Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank01Frame173Pieces[5] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame174Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame175Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame176Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame177Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame178Pieces[5] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame179Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame180Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame181Pieces[5] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame182Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank01Frame183Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame184Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame185Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame186Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame187Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame188Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame189Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame190Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame191Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame192Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame193Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame194Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank01Frame195Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame196Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame197Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame198Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame199Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank01Frame200Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank01Frame201Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank01Frame202Pieces[3] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank01Frame203Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame204Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame205Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame206Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame207Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank01Frame208Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame209Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame210Pieces[5] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame211Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame212Pieces[3] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame213Pieces[2] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame214Pieces[2] = { SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame215Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank01Frame216Pieces[4] = { SPRITE_PIECE(2, 10), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame217Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame218Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame219Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame220Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame221Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank01Frame222Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank01Frame223Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank01Frame224Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank01Frame225Pieces[1] = { SPRITE_PIECE(2, 4) };
const u8 gSpriteBank01Frame226Pieces[2] = { SPRITE_PIECE(1, 11), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame227Pieces[2] = { SPRITE_PIECE(1, 11), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame228Pieces[2] = { SPRITE_PIECE(1, 11), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame229Pieces[3] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame230Pieces[3] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame231Pieces[3] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame232Pieces[2] = { SPRITE_PIECE(1, 11), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame233Pieces[2] = { SPRITE_PIECE(1, 11), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame234Pieces[4] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame235Pieces[5] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame236Pieces[5] = { SPRITE_PIECE(1, 7), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame237Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank01Frame238Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank01Frame239Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank01Frame240Pieces[4] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame241Pieces[4] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame242Pieces[4] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame243Pieces[4] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame244Pieces[1] = { SPRITE_PIECE(1, 7) };
const u8 gSpriteBank01Frame245Pieces[3] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank01Frame246Pieces[1] = { SPRITE_PIECE(1, 7) };
const u8 gSpriteBank01Frame247Pieces[4] = { SPRITE_PIECE(1, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame248Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank01Frame249Pieces[1] = { SPRITE_PIECE(1, 3) };
const u8 gSpriteBank01Frame250Pieces[3] = { SPRITE_PIECE(1, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame251Pieces[4] = { SPRITE_PIECE(1, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame252Pieces[3] = { SPRITE_PIECE(1, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank01Frame253Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank01Frame254Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank01Frame255Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame256Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank01Frame257Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame258Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame259Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame260Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame261Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame262Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame263Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank01Frame264Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank01Frame265Pieces[5] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame266Pieces[5] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame267Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame268Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame269Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame270Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame271Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame272Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank01Frame273Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };

/* ---------------------------------------------------------------------- */
/* Bank 2: 2 animations, 17 frames, tiles in gSpriteBank02Tiles (SPRITE_TILES_BANK02). */

extern const u16 gSpriteBank02Anim00Seq[6];
extern const u16 gSpriteBank02Anim01Seq[11];
extern const struct sprite_frame_1box gSpriteBank02Frame000;
extern const struct sprite_frame_1box gSpriteBank02Frame001;
extern const struct sprite_frame_1box gSpriteBank02Frame002;
extern const struct sprite_frame_1box gSpriteBank02Frame003;
extern const struct sprite_frame_1box gSpriteBank02Frame004;
extern const struct sprite_frame_1box gSpriteBank02Frame005;
extern const struct sprite_frame_1box gSpriteBank02Frame006;
extern const struct sprite_frame_1box gSpriteBank02Frame007;
extern const struct sprite_frame_1box gSpriteBank02Frame008;
extern const struct sprite_frame_1box gSpriteBank02Frame009;
extern const struct sprite_frame_1box gSpriteBank02Frame010;
extern const struct sprite_frame_1box gSpriteBank02Frame011;
extern const struct sprite_frame_1box gSpriteBank02Frame012;
extern const struct sprite_frame_1box gSpriteBank02Frame013;
extern const struct sprite_frame_1box gSpriteBank02Frame014;
extern const struct sprite_frame_1box gSpriteBank02Frame015;
extern const struct sprite_frame_1box gSpriteBank02Frame016;
extern const struct sprite_piece_pos gSpriteBank02Frame000Pos[6];
extern const struct sprite_piece_pos gSpriteBank02Frame001Pos[6];
extern const struct sprite_piece_pos gSpriteBank02Frame002Pos[6];
extern const struct sprite_piece_pos gSpriteBank02Frame003Pos[6];
extern const struct sprite_piece_pos gSpriteBank02Frame004Pos[6];
extern const struct sprite_piece_pos gSpriteBank02Frame005Pos[6];
extern const struct sprite_piece_pos gSpriteBank02Frame006Pos[4];
extern const struct sprite_piece_pos gSpriteBank02Frame007Pos[4];
extern const struct sprite_piece_pos gSpriteBank02Frame008Pos[2];
extern const struct sprite_piece_pos gSpriteBank02Frame009Pos[3];
extern const struct sprite_piece_pos gSpriteBank02Frame010Pos[1];
extern const struct sprite_piece_pos gSpriteBank02Frame011Pos[2];
extern const struct sprite_piece_pos gSpriteBank02Frame012Pos[5];
extern const struct sprite_piece_pos gSpriteBank02Frame013Pos[6];
extern const struct sprite_piece_pos gSpriteBank02Frame014Pos[6];
extern const struct sprite_piece_pos gSpriteBank02Frame015Pos[5];
extern const struct sprite_piece_pos gSpriteBank02Frame016Pos[5];
extern const u8 gSpriteBank02Frame000Pieces[6];
extern const u8 gSpriteBank02Frame001Pieces[6];
extern const u8 gSpriteBank02Frame002Pieces[6];
extern const u8 gSpriteBank02Frame003Pieces[6];
extern const u8 gSpriteBank02Frame004Pieces[6];
extern const u8 gSpriteBank02Frame005Pieces[6];
extern const u8 gSpriteBank02Frame006Pieces[4];
extern const u8 gSpriteBank02Frame007Pieces[4];
extern const u8 gSpriteBank02Frame008Pieces[2];
extern const u8 gSpriteBank02Frame009Pieces[3];
extern const u8 gSpriteBank02Frame010Pieces[1];
extern const u8 gSpriteBank02Frame011Pieces[2];
extern const u8 gSpriteBank02Frame012Pieces[5];
extern const u8 gSpriteBank02Frame013Pieces[6];
extern const u8 gSpriteBank02Frame014Pieces[6];
extern const u8 gSpriteBank02Frame015Pieces[5];
extern const u8 gSpriteBank02Frame016Pieces[5];

const struct sprite_anim gSpriteBank02Anims[2] = {
    [0] = {
        .seq = gSpriteBank02Anim00Seq,
        .box = { { -22, -20, 45, 40 }, { -23, -21, 47, 42 } },
        .tileRecord = 8,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank02Anim00Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [1] = {
        .seq = gSpriteBank02Anim01Seq,
        .box = { { -22, -13, 44, 26 }, { -51, -60, 106, 107 } },
        .tileRecord = 120,
        .duration = 2,
        .frameCount = ARRAY_COUNT(gSpriteBank02Anim01Seq),
        .flags = 0,
    },
};

const u16 gSpriteBank02Anim00Seq[6] = {
    0, 1, 2, 3, 4, 5,
};
const u16 gSpriteBank02Anim01Seq[11] = {
    6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
};

const struct sprite_frame *const gSpriteBank02Frames[17] = {
    &gSpriteBank02Frame000.frame,
    &gSpriteBank02Frame001.frame,
    &gSpriteBank02Frame002.frame,
    &gSpriteBank02Frame003.frame,
    &gSpriteBank02Frame004.frame,
    &gSpriteBank02Frame005.frame,
    &gSpriteBank02Frame006.frame,
    &gSpriteBank02Frame007.frame,
    &gSpriteBank02Frame008.frame,
    &gSpriteBank02Frame009.frame,
    &gSpriteBank02Frame010.frame,
    &gSpriteBank02Frame011.frame,
    &gSpriteBank02Frame012.frame,
    &gSpriteBank02Frame013.frame,
    &gSpriteBank02Frame014.frame,
    &gSpriteBank02Frame015.frame,
    &gSpriteBank02Frame016.frame,
};

const struct sprite_frame_1box gSpriteBank02Frame000 = {
    SPRITE_FRAME(gSpriteBank02Frame000, SPRITE_TILES_BANK02 + 0x00000),
    { { -4, -15, 17, 30 } },
};
const struct sprite_frame_1box gSpriteBank02Frame001 = {
    SPRITE_FRAME(gSpriteBank02Frame001, SPRITE_TILES_BANK02 + 0x00400),
    { { -4, -15, 17, 30 } },
};
const struct sprite_frame_1box gSpriteBank02Frame002 = {
    SPRITE_FRAME(gSpriteBank02Frame002, SPRITE_TILES_BANK02 + 0x00800),
    { { -4, -15, 17, 30 } },
};
const struct sprite_frame_1box gSpriteBank02Frame003 = {
    SPRITE_FRAME(gSpriteBank02Frame003, SPRITE_TILES_BANK02 + 0x00b80),
    { { -4, -15, 17, 30 } },
};
const struct sprite_frame_1box gSpriteBank02Frame004 = {
    SPRITE_FRAME(gSpriteBank02Frame004, SPRITE_TILES_BANK02 + 0x00f80),
    { { -4, -15, 17, 30 } },
};
const struct sprite_frame_1box gSpriteBank02Frame005 = {
    SPRITE_FRAME(gSpriteBank02Frame005, SPRITE_TILES_BANK02 + 0x01380),
    { { -4, -15, 17, 30 } },
};
const struct sprite_frame_1box gSpriteBank02Frame006 = {
    SPRITE_FRAME(gSpriteBank02Frame006, SPRITE_TILES_BANK02 + 0x01780),
    { { -22, -13, 44, 26 } },
};
const struct sprite_frame_1box gSpriteBank02Frame007 = {
    SPRITE_FRAME(gSpriteBank02Frame007, SPRITE_TILES_BANK02 + 0x01a60),
    { { -22, -15, 44, 28 } },
};
const struct sprite_frame_1box gSpriteBank02Frame008 = {
    SPRITE_FRAME(gSpriteBank02Frame008, SPRITE_TILES_BANK02 + 0x01d40),
    { { -23, -19, 47, 32 } },
};
const struct sprite_frame_1box gSpriteBank02Frame009 = {
    SPRITE_FRAME(gSpriteBank02Frame009, SPRITE_TILES_BANK02 + 0x02180),
    { { -25, -24, 53, 41 } },
};
const struct sprite_frame_1box gSpriteBank02Frame010 = {
    SPRITE_FRAME(gSpriteBank02Frame010, SPRITE_TILES_BANK02 + 0x026c0),
    { { -29, -29, 60, 51 } },
};
const struct sprite_frame_1box gSpriteBank02Frame011 = {
    SPRITE_FRAME(gSpriteBank02Frame011, SPRITE_TILES_BANK02 + 0x02e80),
    { { -33, -33, 68, 58 } },
};
const struct sprite_frame_1box gSpriteBank02Frame012 = {
    SPRITE_FRAME(gSpriteBank02Frame012, SPRITE_TILES_BANK02 + 0x03700),
    { { -37, -37, 75, 66 } },
};
const struct sprite_frame_1box gSpriteBank02Frame013 = {
    SPRITE_FRAME(gSpriteBank02Frame013, SPRITE_TILES_BANK02 + 0x040c0),
    { { -39, -42, 80, 75 } },
};
const struct sprite_frame_1box gSpriteBank02Frame014 = {
    SPRITE_FRAME(gSpriteBank02Frame014, SPRITE_TILES_BANK02 + 0x04be0),
    { { -43, -46, 88, 83 } },
};
const struct sprite_frame_1box gSpriteBank02Frame015 = {
    SPRITE_FRAME(gSpriteBank02Frame015, SPRITE_TILES_BANK02 + 0x05aa0),
    { { -47, -51, 95, 91 } },
};
const struct sprite_frame_1box gSpriteBank02Frame016 = {
    SPRITE_FRAME(gSpriteBank02Frame016, SPRITE_TILES_BANK02 + 0x06b40),
    { { -51, -60, 106, 107 } },
};

const struct sprite_piece_pos gSpriteBank02Frame000Pos[6] = { { -22, -20 }, { 10, -19 }, { 18, -3 }, { -18, 12 }, { 14, 12 }, { 22, 12 } };
const struct sprite_piece_pos gSpriteBank02Frame001Pos[6] = { { -22, -18 }, { 10, -19 }, { 18, -1 }, { -18, 13 }, { 14, 13 }, { 22, 13 } };
const struct sprite_piece_pos gSpriteBank02Frame002Pos[6] = { { -22, -19 }, { 10, -19 }, { 18, -1 }, { -18, 13 }, { 14, 13 }, { 22, 13 } };
const struct sprite_piece_pos gSpriteBank02Frame003Pos[6] = { { -22, -20 }, { 10, -21 }, { 18, -3 }, { -18, 11 }, { 14, 11 }, { 22, 11 } };
const struct sprite_piece_pos gSpriteBank02Frame004Pos[6] = { { -23, -21 }, { 9, -21 }, { 17, -4 }, { -18, 11 }, { 14, 11 }, { 22, 11 } };
const struct sprite_piece_pos gSpriteBank02Frame005Pos[6] = { { -22, -21 }, { 10, -20 }, { 18, -3 }, { -18, 11 }, { 14, 11 }, { 22, 11 } };
const struct sprite_piece_pos gSpriteBank02Frame006Pos[4] = { { -22, -11 }, { 10, -13 }, { 18, -9 }, { 18, 7 } };
const struct sprite_piece_pos gSpriteBank02Frame007Pos[4] = { { -22, -15 }, { 10, -14 }, { 18, -10 }, { 18, 6 } };
const struct sprite_piece_pos gSpriteBank02Frame008Pos[2] = { { -23, -19 }, { -9, 13 } };
const struct sprite_piece_pos gSpriteBank02Frame009Pos[3] = { { -25, -24 }, { -21, 8 }, { 12, 8 } };
const struct sprite_piece_pos gSpriteBank02Frame010Pos[1] = { { -29, -29 } };
const struct sprite_piece_pos gSpriteBank02Frame011Pos[2] = { { -33, -33 }, { 31, -21 } };
const struct sprite_piece_pos gSpriteBank02Frame012Pos[5] = { { -37, -37 }, { 27, -22 }, { 27, 10 }, { -16, 27 }, { 0, 27 } };
const struct sprite_piece_pos gSpriteBank02Frame013Pos[6] = { { -39, -42 }, { 25, -26 }, { 25, 10 }, { 41, 10 }, { -29, 22 }, { 8, 22 } };
const struct sprite_piece_pos gSpriteBank02Frame014Pos[6] = { { -43, -46 }, { 21, -46 }, { -32, 18 }, { 10, 20 }, { 32, 21 }, { -22, 34 } };
const struct sprite_piece_pos gSpriteBank02Frame015Pos[5] = { { -47, -50 }, { 21, -51 }, { -35, 15 }, { 30, 25 }, { 47, 13 } };
const struct sprite_piece_pos gSpriteBank02Frame016Pos[5] = { { -51, -56 }, { 14, -60 }, { 46, -33 }, { -42, 22 }, { 38, 36 } };

const u8 gSpriteBank02Frame000Pieces[6] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank02Frame001Pieces[6] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank02Frame002Pieces[6] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank02Frame003Pieces[6] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank02Frame004Pieces[6] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank02Frame005Pieces[6] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank02Frame006Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank02Frame007Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank02Frame008Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank02Frame009Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank02Frame010Pieces[1] = { SPRITE_PIECE(2, 3) };
const u8 gSpriteBank02Frame011Pieces[2] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 9) };
const u8 gSpriteBank02Frame012Pieces[5] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank02Frame013Pieces[6] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank02Frame014Pieces[6] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank02Frame015Pieces[5] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 11), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank02Frame016Pieces[5] = { SPRITE_PIECE(2, 3), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 7), SPRITE_PIECE(0, 0) };

/* ---------------------------------------------------------------------- */
/* Bank 3: 1 animation, 25 frames, tiles in gSpriteBank03Tiles (SPRITE_TILES_BANK03). */

extern const u16 gSpriteBank03Anim00Seq[25];
extern const struct sprite_frame_1box gSpriteBank03Frame000;
extern const struct sprite_frame_1box gSpriteBank03Frame001;
extern const struct sprite_frame_1box gSpriteBank03Frame002;
extern const struct sprite_frame_1box gSpriteBank03Frame003;
extern const struct sprite_frame_1box gSpriteBank03Frame004;
extern const struct sprite_frame_1box gSpriteBank03Frame005;
extern const struct sprite_frame_2box gSpriteBank03Frame006;
extern const struct sprite_frame_1box gSpriteBank03Frame007;
extern const struct sprite_frame_1box gSpriteBank03Frame008;
extern const struct sprite_frame_1box gSpriteBank03Frame009;
extern const struct sprite_frame_1box gSpriteBank03Frame010;
extern const struct sprite_frame_1box gSpriteBank03Frame011;
extern const struct sprite_frame_1box gSpriteBank03Frame012;
extern const struct sprite_frame_1box gSpriteBank03Frame013;
extern const struct sprite_frame_1box gSpriteBank03Frame014;
extern const struct sprite_frame_1box gSpriteBank03Frame015;
extern const struct sprite_frame_1box gSpriteBank03Frame016;
extern const struct sprite_frame_1box gSpriteBank03Frame017;
extern const struct sprite_frame_1box gSpriteBank03Frame018;
extern const struct sprite_frame_1box gSpriteBank03Frame019;
extern const struct sprite_frame_1box gSpriteBank03Frame020;
extern const struct sprite_frame_1box gSpriteBank03Frame021;
extern const struct sprite_frame_1box gSpriteBank03Frame022;
extern const struct sprite_frame_1box gSpriteBank03Frame023;
extern const struct sprite_frame_1box gSpriteBank03Frame024;
extern const struct sprite_piece_pos gSpriteBank03Frame000Pos[4];
extern const struct sprite_piece_pos gSpriteBank03Frame001Pos[5];
extern const struct sprite_piece_pos gSpriteBank03Frame002Pos[3];
extern const struct sprite_piece_pos gSpriteBank03Frame003Pos[1];
extern const struct sprite_piece_pos gSpriteBank03Frame004Pos[2];
extern const struct sprite_piece_pos gSpriteBank03Frame005Pos[2];
extern const struct sprite_piece_pos gSpriteBank03Frame006Pos[6];
extern const struct sprite_piece_pos gSpriteBank03Frame007Pos[4];
extern const struct sprite_piece_pos gSpriteBank03Frame008Pos[4];
extern const struct sprite_piece_pos gSpriteBank03Frame009Pos[3];
extern const struct sprite_piece_pos gSpriteBank03Frame010Pos[3];
extern const struct sprite_piece_pos gSpriteBank03Frame011Pos[3];
extern const struct sprite_piece_pos gSpriteBank03Frame012Pos[3];
extern const struct sprite_piece_pos gSpriteBank03Frame013Pos[2];
extern const struct sprite_piece_pos gSpriteBank03Frame014Pos[2];
extern const struct sprite_piece_pos gSpriteBank03Frame015Pos[3];
extern const struct sprite_piece_pos gSpriteBank03Frame016Pos[4];
extern const struct sprite_piece_pos gSpriteBank03Frame017Pos[2];
extern const struct sprite_piece_pos gSpriteBank03Frame018Pos[1];
extern const struct sprite_piece_pos gSpriteBank03Frame019Pos[1];
extern const struct sprite_piece_pos gSpriteBank03Frame020Pos[4];
extern const struct sprite_piece_pos gSpriteBank03Frame021Pos[4];
extern const struct sprite_piece_pos gSpriteBank03Frame022Pos[4];
extern const struct sprite_piece_pos gSpriteBank03Frame023Pos[4];
extern const struct sprite_piece_pos gSpriteBank03Frame024Pos[4];
extern const u8 gSpriteBank03Frame000Pieces[4];
extern const u8 gSpriteBank03Frame001Pieces[5];
extern const u8 gSpriteBank03Frame002Pieces[3];
extern const u8 gSpriteBank03Frame003Pieces[1];
extern const u8 gSpriteBank03Frame004Pieces[2];
extern const u8 gSpriteBank03Frame005Pieces[2];
extern const u8 gSpriteBank03Frame006Pieces[6];
extern const u8 gSpriteBank03Frame007Pieces[4];
extern const u8 gSpriteBank03Frame008Pieces[4];
extern const u8 gSpriteBank03Frame009Pieces[3];
extern const u8 gSpriteBank03Frame010Pieces[3];
extern const u8 gSpriteBank03Frame011Pieces[3];
extern const u8 gSpriteBank03Frame012Pieces[3];
extern const u8 gSpriteBank03Frame013Pieces[2];
extern const u8 gSpriteBank03Frame014Pieces[2];
extern const u8 gSpriteBank03Frame015Pieces[3];
extern const u8 gSpriteBank03Frame016Pieces[4];
extern const u8 gSpriteBank03Frame017Pieces[2];
extern const u8 gSpriteBank03Frame018Pieces[1];
extern const u8 gSpriteBank03Frame019Pieces[1];
extern const u8 gSpriteBank03Frame020Pieces[4];
extern const u8 gSpriteBank03Frame021Pieces[4];
extern const u8 gSpriteBank03Frame022Pieces[4];
extern const u8 gSpriteBank03Frame023Pieces[4];
extern const u8 gSpriteBank03Frame024Pieces[4];

const struct sprite_anim gSpriteBank03Anims[1] = {
    [0] = {
        .seq = gSpriteBank03Anim00Seq,
        .box = { { -19, -11, 39, 22 }, { -74, -36, 94, 70 } },
        .tileRecord = 9,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank03Anim00Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank03Anim00Seq[25] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24,
};

const struct sprite_frame *const gSpriteBank03Frames[25] = {
    &gSpriteBank03Frame000.frame,
    &gSpriteBank03Frame001.frame,
    &gSpriteBank03Frame002.frame,
    &gSpriteBank03Frame003.frame,
    &gSpriteBank03Frame004.frame,
    &gSpriteBank03Frame005.frame,
    &gSpriteBank03Frame006.frame,
    &gSpriteBank03Frame007.frame,
    &gSpriteBank03Frame008.frame,
    &gSpriteBank03Frame009.frame,
    &gSpriteBank03Frame010.frame,
    &gSpriteBank03Frame011.frame,
    &gSpriteBank03Frame012.frame,
    &gSpriteBank03Frame013.frame,
    &gSpriteBank03Frame014.frame,
    &gSpriteBank03Frame015.frame,
    &gSpriteBank03Frame016.frame,
    &gSpriteBank03Frame017.frame,
    &gSpriteBank03Frame018.frame,
    &gSpriteBank03Frame019.frame,
    &gSpriteBank03Frame020.frame,
    &gSpriteBank03Frame021.frame,
    &gSpriteBank03Frame022.frame,
    &gSpriteBank03Frame023.frame,
    &gSpriteBank03Frame024.frame,
};

const struct sprite_frame_1box gSpriteBank03Frame000 = {
    SPRITE_FRAME(gSpriteBank03Frame000, SPRITE_TILES_BANK03 + 0x00000),
    { { -19, -11, 39, 22 } },
};
const struct sprite_frame_1box gSpriteBank03Frame001 = {
    SPRITE_FRAME(gSpriteBank03Frame001, SPRITE_TILES_BANK03 + 0x001e0),
    { { -21, -10, 40, 22 } },
};
const struct sprite_frame_1box gSpriteBank03Frame002 = {
    SPRITE_FRAME(gSpriteBank03Frame002, SPRITE_TILES_BANK03 + 0x003e0),
    { { -25, -9, 42, 28 } },
};
const struct sprite_frame_1box gSpriteBank03Frame003 = {
    SPRITE_FRAME(gSpriteBank03Frame003, SPRITE_TILES_BANK03 + 0x006a0),
    { { -41, -12, 58, 26 } },
};
const struct sprite_frame_1box gSpriteBank03Frame004 = {
    SPRITE_FRAME(gSpriteBank03Frame004, SPRITE_TILES_BANK03 + 0x00a60),
    { { -44, -25, 61, 32 } },
};
const struct sprite_frame_1box gSpriteBank03Frame005 = {
    SPRITE_FRAME(gSpriteBank03Frame005, SPRITE_TILES_BANK03 + 0x00e80),
    { { -34, -36, 51, 43 } },
};
const struct sprite_frame_2box gSpriteBank03Frame006 = {
    SPRITE_FRAME(gSpriteBank03Frame006, SPRITE_TILES_BANK03 + 0x01380),
    { { -73, -13, 90, 22 }, { -71, -9, 32, 15 } },
};
const struct sprite_frame_1box gSpriteBank03Frame007 = {
    SPRITE_FRAME(gSpriteBank03Frame007, SPRITE_TILES_BANK03 + 0x01800),
    { { -73, -8, 90, 42 } },
};
const struct sprite_frame_1box gSpriteBank03Frame008 = {
    SPRITE_FRAME(gSpriteBank03Frame008, SPRITE_TILES_BANK03 + 0x01f00),
    { { -74, -9, 91, 37 } },
};
const struct sprite_frame_1box gSpriteBank03Frame009 = {
    SPRITE_FRAME(gSpriteBank03Frame009, SPRITE_TILES_BANK03 + 0x025c0),
    { { -67, -9, 84, 25 } },
};
const struct sprite_frame_1box gSpriteBank03Frame010 = {
    SPRITE_FRAME(gSpriteBank03Frame010, SPRITE_TILES_BANK03 + 0x02b00),
    { { -63, -9, 80, 26 } },
};
const struct sprite_frame_1box gSpriteBank03Frame011 = {
    SPRITE_FRAME(gSpriteBank03Frame011, SPRITE_TILES_BANK03 + 0x03020),
    { { -64, -9, 81, 28 } },
};
const struct sprite_frame_1box gSpriteBank03Frame012 = {
    SPRITE_FRAME(gSpriteBank03Frame012, SPRITE_TILES_BANK03 + 0x03560),
    { { -65, -9, 82, 30 } },
};
const struct sprite_frame_1box gSpriteBank03Frame013 = {
    SPRITE_FRAME(gSpriteBank03Frame013, SPRITE_TILES_BANK03 + 0x03aa0),
    { { -71, -9, 88, 27 } },
};
const struct sprite_frame_1box gSpriteBank03Frame014 = {
    SPRITE_FRAME(gSpriteBank03Frame014, SPRITE_TILES_BANK03 + 0x04060),
    { { -72, -9, 89, 29 } },
};
const struct sprite_frame_1box gSpriteBank03Frame015 = {
    SPRITE_FRAME(gSpriteBank03Frame015, SPRITE_TILES_BANK03 + 0x04620),
    { { -68, -9, 85, 29 } },
};
const struct sprite_frame_1box gSpriteBank03Frame016 = {
    SPRITE_FRAME(gSpriteBank03Frame016, SPRITE_TILES_BANK03 + 0x04b60),
    { { -58, -9, 75, 32 } },
};
const struct sprite_frame_1box gSpriteBank03Frame017 = {
    SPRITE_FRAME(gSpriteBank03Frame017, SPRITE_TILES_BANK03 + 0x05000),
    { { -52, -9, 69, 24 } },
};
const struct sprite_frame_1box gSpriteBank03Frame018 = {
    SPRITE_FRAME(gSpriteBank03Frame018, SPRITE_TILES_BANK03 + 0x05440),
    { { -44, -11, 61, 27 } },
};
const struct sprite_frame_1box gSpriteBank03Frame019 = {
    SPRITE_FRAME(gSpriteBank03Frame019, SPRITE_TILES_BANK03 + 0x05800),
    { { -37, -9, 54, 24 } },
};
const struct sprite_frame_1box gSpriteBank03Frame020 = {
    SPRITE_FRAME(gSpriteBank03Frame020, SPRITE_TILES_BANK03 + 0x05c00),
    { { -28, -9, 45, 28 } },
};
const struct sprite_frame_1box gSpriteBank03Frame021 = {
    SPRITE_FRAME(gSpriteBank03Frame021, SPRITE_TILES_BANK03 + 0x05ee0),
    { { -26, -10, 43, 25 } },
};
const struct sprite_frame_1box gSpriteBank03Frame022 = {
    SPRITE_FRAME(gSpriteBank03Frame022, SPRITE_TILES_BANK03 + 0x061c0),
    { { -21, -12, 38, 21 } },
};
const struct sprite_frame_1box gSpriteBank03Frame023 = {
    SPRITE_FRAME(gSpriteBank03Frame023, SPRITE_TILES_BANK03 + 0x063a0),
    { { -19, -12, 39, 23 } },
};
const struct sprite_frame_1box gSpriteBank03Frame024 = {
    SPRITE_FRAME(gSpriteBank03Frame024, SPRITE_TILES_BANK03 + 0x06580),
    { { -19, -11, 39, 22 } },
};

const struct sprite_piece_pos gSpriteBank03Frame000Pos[4] = { { -15, -11 }, { 13, -9 }, { -19, 5 }, { 13, 5 } };
const struct sprite_piece_pos gSpriteBank03Frame001Pos[5] = { { -17, -10 }, { 11, -9 }, { -21, 6 }, { 11, 6 }, { 19, 8 } };
const struct sprite_piece_pos gSpriteBank03Frame002Pos[3] = { { -25, -7 }, { 7, -9 }, { 15, -9 } };
const struct sprite_piece_pos gSpriteBank03Frame003Pos[1] = { { -41, -12 } };
const struct sprite_piece_pos gSpriteBank03Frame004Pos[2] = { { -44, -25 }, { 6, 7 } };
const struct sprite_piece_pos gSpriteBank03Frame005Pos[2] = { { -34, -36 }, { -10, -4 } };
const struct sprite_piece_pos gSpriteBank03Frame006Pos[6] = { { -73, -10 }, { -41, -13 }, { -9, -9 }, { -72, 3 }, { -40, 3 }, { -8, 3 } };
const struct sprite_piece_pos gSpriteBank03Frame007Pos[4] = { { -73, -2 }, { -9, -8 }, { -73, 24 }, { -41, 24 } };
const struct sprite_piece_pos gSpriteBank03Frame008Pos[4] = { { -74, -3 }, { -10, -9 }, { -72, 23 }, { -37, 23 } };
const struct sprite_piece_pos gSpriteBank03Frame009Pos[3] = { { -67, -6 }, { -3, -9 }, { 13, -9 } };
const struct sprite_piece_pos gSpriteBank03Frame010Pos[3] = { { -63, -7 }, { 1, -9 }, { 17, -3 } };
const struct sprite_piece_pos gSpriteBank03Frame011Pos[3] = { { -64, -8 }, { 0, -9 }, { 16, -9 } };
const struct sprite_piece_pos gSpriteBank03Frame012Pos[3] = { { -65, -8 }, { -1, -9 }, { 15, -9 } };
const struct sprite_piece_pos gSpriteBank03Frame013Pos[2] = { { -71, -8 }, { -7, -9 } };
const struct sprite_piece_pos gSpriteBank03Frame014Pos[2] = { { -72, -8 }, { -8, -9 } };
const struct sprite_piece_pos gSpriteBank03Frame015Pos[3] = { { -68, -5 }, { -4, -8 }, { 12, -9 } };
const struct sprite_piece_pos gSpriteBank03Frame016Pos[4] = { { -58, -8 }, { 6, -9 }, { 14, -9 }, { -54, 23 } };
const struct sprite_piece_pos gSpriteBank03Frame017Pos[2] = { { -52, -8 }, { 12, -9 } };
const struct sprite_piece_pos gSpriteBank03Frame018Pos[1] = { { -44, -11 } };
const struct sprite_piece_pos gSpriteBank03Frame019Pos[1] = { { -37, -9 } };
const struct sprite_piece_pos gSpriteBank03Frame020Pos[4] = { { -28, -7 }, { 4, -8 }, { 12, -9 }, { 12, 7 } };
const struct sprite_piece_pos gSpriteBank03Frame021Pos[4] = { { -26, -10 }, { 6, -10 }, { 14, -9 }, { 14, 10 } };
const struct sprite_piece_pos gSpriteBank03Frame022Pos[4] = { { -21, -11 }, { 11, -12 }, { -16, 4 }, { 16, 4 } };
const struct sprite_piece_pos gSpriteBank03Frame023Pos[4] = { { -19, -12 }, { 13, -11 }, { -19, 4 }, { 13, 4 } };
const struct sprite_piece_pos gSpriteBank03Frame024Pos[4] = { { -15, -11 }, { 13, -9 }, { -19, 5 }, { 13, 5 } };

const u8 gSpriteBank03Frame000Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank03Frame001Pieces[5] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank03Frame002Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank03Frame003Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank03Frame004Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank03Frame005Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank03Frame006Pieces[6] = { SPRITE_PIECE(3, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank03Frame007Pieces[4] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank03Frame008Pieces[4] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 2), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank03Frame009Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank03Frame010Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank03Frame011Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank03Frame012Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank03Frame013Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 2) };
const u8 gSpriteBank03Frame014Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 2) };
const u8 gSpriteBank03Frame015Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 10), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank03Frame016Pieces[4] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank03Frame017Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank03Frame018Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank03Frame019Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank03Frame020Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank03Frame021Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank03Frame022Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank03Frame023Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank03Frame024Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };

/* ---------------------------------------------------------------------- */
/* Bank 4: 2 animations, 32 frames, tiles in gSpriteBank04Tiles (SPRITE_TILES_BANK04). */

extern const u16 gSpriteBank04Anim00Seq[16];
extern const u16 gSpriteBank04Anim01Seq[16];
extern const struct sprite_frame_1box gSpriteBank04Frame000;
extern const struct sprite_frame_1box gSpriteBank04Frame001;
extern const struct sprite_frame_1box gSpriteBank04Frame002;
extern const struct sprite_frame_1box gSpriteBank04Frame003;
extern const struct sprite_frame_1box gSpriteBank04Frame004;
extern const struct sprite_frame_1box gSpriteBank04Frame005;
extern const struct sprite_frame_1box gSpriteBank04Frame006;
extern const struct sprite_frame_1box gSpriteBank04Frame007;
extern const struct sprite_frame_1box gSpriteBank04Frame008;
extern const struct sprite_frame_1box gSpriteBank04Frame009;
extern const struct sprite_frame_1box gSpriteBank04Frame010;
extern const struct sprite_frame_1box gSpriteBank04Frame011;
extern const struct sprite_frame_1box gSpriteBank04Frame012;
extern const struct sprite_frame_1box gSpriteBank04Frame013;
extern const struct sprite_frame_1box gSpriteBank04Frame014;
extern const struct sprite_frame_1box gSpriteBank04Frame015;
extern const struct sprite_frame_1box gSpriteBank04Frame016;
extern const struct sprite_frame_1box gSpriteBank04Frame017;
extern const struct sprite_frame_1box gSpriteBank04Frame018;
extern const struct sprite_frame_1box gSpriteBank04Frame019;
extern const struct sprite_frame_1box gSpriteBank04Frame020;
extern const struct sprite_frame_1box gSpriteBank04Frame021;
extern const struct sprite_frame_1box gSpriteBank04Frame022;
extern const struct sprite_frame_1box gSpriteBank04Frame023;
extern const struct sprite_frame_1box gSpriteBank04Frame024;
extern const struct sprite_frame_1box gSpriteBank04Frame025;
extern const struct sprite_frame_1box gSpriteBank04Frame026;
extern const struct sprite_frame_1box gSpriteBank04Frame027;
extern const struct sprite_frame_1box gSpriteBank04Frame028;
extern const struct sprite_frame_1box gSpriteBank04Frame029;
extern const struct sprite_frame_1box gSpriteBank04Frame030;
extern const struct sprite_frame_1box gSpriteBank04Frame031;
extern const struct sprite_piece_pos gSpriteBank04Frame000Pos[3];
extern const struct sprite_piece_pos gSpriteBank04Frame001Pos[3];
extern const struct sprite_piece_pos gSpriteBank04Frame002Pos[2];
extern const struct sprite_piece_pos gSpriteBank04Frame003Pos[3];
extern const struct sprite_piece_pos gSpriteBank04Frame004Pos[3];
extern const struct sprite_piece_pos gSpriteBank04Frame005Pos[2];
extern const struct sprite_piece_pos gSpriteBank04Frame006Pos[3];
extern const struct sprite_piece_pos gSpriteBank04Frame007Pos[2];
extern const struct sprite_piece_pos gSpriteBank04Frame008Pos[4];
extern const struct sprite_piece_pos gSpriteBank04Frame009Pos[4];
extern const struct sprite_piece_pos gSpriteBank04Frame010Pos[3];
extern const struct sprite_piece_pos gSpriteBank04Frame011Pos[4];
extern const struct sprite_piece_pos gSpriteBank04Frame012Pos[5];
extern const struct sprite_piece_pos gSpriteBank04Frame013Pos[2];
extern const struct sprite_piece_pos gSpriteBank04Frame014Pos[2];
extern const struct sprite_piece_pos gSpriteBank04Frame015Pos[3];
extern const struct sprite_piece_pos gSpriteBank04Frame016Pos[3];
extern const struct sprite_piece_pos gSpriteBank04Frame017Pos[2];
extern const struct sprite_piece_pos gSpriteBank04Frame018Pos[2];
extern const struct sprite_piece_pos gSpriteBank04Frame019Pos[2];
extern const struct sprite_piece_pos gSpriteBank04Frame020Pos[2];
extern const struct sprite_piece_pos gSpriteBank04Frame021Pos[4];
extern const struct sprite_piece_pos gSpriteBank04Frame022Pos[4];
extern const struct sprite_piece_pos gSpriteBank04Frame023Pos[4];
extern const struct sprite_piece_pos gSpriteBank04Frame024Pos[4];
extern const struct sprite_piece_pos gSpriteBank04Frame025Pos[2];
extern const struct sprite_piece_pos gSpriteBank04Frame026Pos[3];
extern const struct sprite_piece_pos gSpriteBank04Frame027Pos[3];
extern const struct sprite_piece_pos gSpriteBank04Frame028Pos[3];
extern const struct sprite_piece_pos gSpriteBank04Frame029Pos[3];
extern const struct sprite_piece_pos gSpriteBank04Frame030Pos[3];
extern const struct sprite_piece_pos gSpriteBank04Frame031Pos[3];
extern const u8 gSpriteBank04Frame000Pieces[3];
extern const u8 gSpriteBank04Frame001Pieces[3];
extern const u8 gSpriteBank04Frame002Pieces[2];
extern const u8 gSpriteBank04Frame003Pieces[3];
extern const u8 gSpriteBank04Frame004Pieces[3];
extern const u8 gSpriteBank04Frame005Pieces[2];
extern const u8 gSpriteBank04Frame006Pieces[3];
extern const u8 gSpriteBank04Frame007Pieces[2];
extern const u8 gSpriteBank04Frame008Pieces[4];
extern const u8 gSpriteBank04Frame009Pieces[4];
extern const u8 gSpriteBank04Frame010Pieces[3];
extern const u8 gSpriteBank04Frame011Pieces[4];
extern const u8 gSpriteBank04Frame012Pieces[5];
extern const u8 gSpriteBank04Frame013Pieces[2];
extern const u8 gSpriteBank04Frame014Pieces[2];
extern const u8 gSpriteBank04Frame015Pieces[3];
extern const u8 gSpriteBank04Frame016Pieces[3];
extern const u8 gSpriteBank04Frame017Pieces[2];
extern const u8 gSpriteBank04Frame018Pieces[2];
extern const u8 gSpriteBank04Frame019Pieces[2];
extern const u8 gSpriteBank04Frame020Pieces[2];
extern const u8 gSpriteBank04Frame021Pieces[4];
extern const u8 gSpriteBank04Frame022Pieces[4];
extern const u8 gSpriteBank04Frame023Pieces[4];
extern const u8 gSpriteBank04Frame024Pieces[4];
extern const u8 gSpriteBank04Frame025Pieces[2];
extern const u8 gSpriteBank04Frame026Pieces[3];
extern const u8 gSpriteBank04Frame027Pieces[3];
extern const u8 gSpriteBank04Frame028Pieces[3];
extern const u8 gSpriteBank04Frame029Pieces[3];
extern const u8 gSpriteBank04Frame030Pieces[3];
extern const u8 gSpriteBank04Frame031Pieces[3];

const struct sprite_anim gSpriteBank04Anims[2] = {
    [0] = {
        .seq = gSpriteBank04Anim00Seq,
        .box = { { -27, -17, 55, 35 }, { -29, -17, 60, 47 } },
        .tileRecord = 10,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank04Anim00Seq),
        .flags = 0,
    },
    [1] = {
        .seq = gSpriteBank04Anim01Seq,
        .box = { { -27, -17, 55, 35 }, { -29, -18, 57, 36 } },
        .tileRecord = 10,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank04Anim01Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank04Anim00Seq[16] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
};
const u16 gSpriteBank04Anim01Seq[16] = {
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
};

const struct sprite_frame *const gSpriteBank04Frames[32] = {
    &gSpriteBank04Frame000.frame,
    &gSpriteBank04Frame001.frame,
    &gSpriteBank04Frame002.frame,
    &gSpriteBank04Frame003.frame,
    &gSpriteBank04Frame004.frame,
    &gSpriteBank04Frame005.frame,
    &gSpriteBank04Frame006.frame,
    &gSpriteBank04Frame007.frame,
    &gSpriteBank04Frame008.frame,
    &gSpriteBank04Frame009.frame,
    &gSpriteBank04Frame010.frame,
    &gSpriteBank04Frame011.frame,
    &gSpriteBank04Frame012.frame,
    &gSpriteBank04Frame013.frame,
    &gSpriteBank04Frame014.frame,
    &gSpriteBank04Frame015.frame,
    &gSpriteBank04Frame016.frame,
    &gSpriteBank04Frame017.frame,
    &gSpriteBank04Frame018.frame,
    &gSpriteBank04Frame019.frame,
    &gSpriteBank04Frame020.frame,
    &gSpriteBank04Frame021.frame,
    &gSpriteBank04Frame022.frame,
    &gSpriteBank04Frame023.frame,
    &gSpriteBank04Frame024.frame,
    &gSpriteBank04Frame025.frame,
    &gSpriteBank04Frame026.frame,
    &gSpriteBank04Frame027.frame,
    &gSpriteBank04Frame028.frame,
    &gSpriteBank04Frame029.frame,
    &gSpriteBank04Frame030.frame,
    &gSpriteBank04Frame031.frame,
};

const struct sprite_frame_1box gSpriteBank04Frame000 = {
    SPRITE_FRAME(gSpriteBank04Frame000, SPRITE_TILES_BANK04 + 0x00000),
    { { -19, -5, 40, 18 } },
};
const struct sprite_frame_1box gSpriteBank04Frame001 = {
    SPRITE_FRAME(gSpriteBank04Frame001, SPRITE_TILES_BANK04 + 0x00440),
    { { -19, -5, 41, 19 } },
};
const struct sprite_frame_1box gSpriteBank04Frame002 = {
    SPRITE_FRAME(gSpriteBank04Frame002, SPRITE_TILES_BANK04 + 0x00880),
    { { -20, -6, 40, 18 } },
};
const struct sprite_frame_1box gSpriteBank04Frame003 = {
    SPRITE_FRAME(gSpriteBank04Frame003, SPRITE_TILES_BANK04 + 0x00d00),
    { { -20, -6, 40, 19 } },
};
const struct sprite_frame_1box gSpriteBank04Frame004 = {
    SPRITE_FRAME(gSpriteBank04Frame004, SPRITE_TILES_BANK04 + 0x011a0),
    { { -18, -4, 36, 17 } },
};
const struct sprite_frame_1box gSpriteBank04Frame005 = {
    SPRITE_FRAME(gSpriteBank04Frame005, SPRITE_TILES_BANK04 + 0x01600),
    { { -17, -5, 34, 22 } },
};
const struct sprite_frame_1box gSpriteBank04Frame006 = {
    SPRITE_FRAME(gSpriteBank04Frame006, SPRITE_TILES_BANK04 + 0x01a40),
    { { -15, -4, 31, 18 } },
};
const struct sprite_frame_1box gSpriteBank04Frame007 = {
    SPRITE_FRAME(gSpriteBank04Frame007, SPRITE_TILES_BANK04 + 0x01ea0),
    { { -12, -5, 29, 22 } },
};
const struct sprite_frame_1box gSpriteBank04Frame008 = {
    SPRITE_FRAME(gSpriteBank04Frame008, SPRITE_TILES_BANK04 + 0x02320),
    { { -5, -5, 18, 23 } },
};
const struct sprite_frame_1box gSpriteBank04Frame009 = {
    SPRITE_FRAME(gSpriteBank04Frame009, SPRITE_TILES_BANK04 + 0x025e0),
    { { -9, -8, 24, 27 } },
};
const struct sprite_frame_1box gSpriteBank04Frame010 = {
    SPRITE_FRAME(gSpriteBank04Frame010, SPRITE_TILES_BANK04 + 0x028c0),
    { { -12, -7, 28, 24 } },
};
const struct sprite_frame_1box gSpriteBank04Frame011 = {
    SPRITE_FRAME(gSpriteBank04Frame011, SPRITE_TILES_BANK04 + 0x02be0),
    { { -14, -6, 30, 22 } },
};
const struct sprite_frame_1box gSpriteBank04Frame012 = {
    SPRITE_FRAME(gSpriteBank04Frame012, SPRITE_TILES_BANK04 + 0x02fa0),
    { { -15, -6, 32, 21 } },
};
const struct sprite_frame_1box gSpriteBank04Frame013 = {
    SPRITE_FRAME(gSpriteBank04Frame013, SPRITE_TILES_BANK04 + 0x03300),
    { { -15, -5, 36, 18 } },
};
const struct sprite_frame_1box gSpriteBank04Frame014 = {
    SPRITE_FRAME(gSpriteBank04Frame014, SPRITE_TILES_BANK04 + 0x03780),
    { { -17, -7, 34, 19 } },
};
const struct sprite_frame_1box gSpriteBank04Frame015 = {
    SPRITE_FRAME(gSpriteBank04Frame015, SPRITE_TILES_BANK04 + 0x03c00),
    { { -18, -8, 36, 21 } },
};
const struct sprite_frame_1box gSpriteBank04Frame016 = {
    SPRITE_FRAME(gSpriteBank04Frame016, SPRITE_TILES_BANK04 + 0x04040),
    { { -18, -5, 40, 17 } },
};
const struct sprite_frame_1box gSpriteBank04Frame017 = {
    SPRITE_FRAME(gSpriteBank04Frame017, SPRITE_TILES_BANK04 + 0x04480),
    { { -21, -5, 43, 15 } },
};
const struct sprite_frame_1box gSpriteBank04Frame018 = {
    SPRITE_FRAME(gSpriteBank04Frame018, SPRITE_TILES_BANK04 + 0x04900),
    { { -16, -3, 38, 15 } },
};
const struct sprite_frame_1box gSpriteBank04Frame019 = {
    SPRITE_FRAME(gSpriteBank04Frame019, SPRITE_TILES_BANK04 + 0x04d80),
    { { -18, -5, 37, 16 } },
};
const struct sprite_frame_1box gSpriteBank04Frame020 = {
    SPRITE_FRAME(gSpriteBank04Frame020, SPRITE_TILES_BANK04 + 0x05200),
    { { -16, -6, 37, 16 } },
};
const struct sprite_frame_1box gSpriteBank04Frame021 = {
    SPRITE_FRAME(gSpriteBank04Frame021, SPRITE_TILES_BANK04 + 0x05620),
    { { -15, -5, 35, 15 } },
};
const struct sprite_frame_1box gSpriteBank04Frame022 = {
    SPRITE_FRAME(gSpriteBank04Frame022, SPRITE_TILES_BANK04 + 0x05900),
    { { -14, -7, 36, 19 } },
};
const struct sprite_frame_1box gSpriteBank04Frame023 = {
    SPRITE_FRAME(gSpriteBank04Frame023, SPRITE_TILES_BANK04 + 0x05bc0),
    { { -16, -6, 39, 19 } },
};
const struct sprite_frame_1box gSpriteBank04Frame024 = {
    SPRITE_FRAME(gSpriteBank04Frame024, SPRITE_TILES_BANK04 + 0x05e80),
    { { -16, -6, 38, 18 } },
};
const struct sprite_frame_1box gSpriteBank04Frame025 = {
    SPRITE_FRAME(gSpriteBank04Frame025, SPRITE_TILES_BANK04 + 0x06160),
    { { -18, -6, 42, 20 } },
};
const struct sprite_frame_1box gSpriteBank04Frame026 = {
    SPRITE_FRAME(gSpriteBank04Frame026, SPRITE_TILES_BANK04 + 0x06580),
    { { -20, -7, 43, 19 } },
};
const struct sprite_frame_1box gSpriteBank04Frame027 = {
    SPRITE_FRAME(gSpriteBank04Frame027, SPRITE_TILES_BANK04 + 0x069e0),
    { { -21, -6, 45, 19 } },
};
const struct sprite_frame_1box gSpriteBank04Frame028 = {
    SPRITE_FRAME(gSpriteBank04Frame028, SPRITE_TILES_BANK04 + 0x06e20),
    { { -20, -7, 42, 18 } },
};
const struct sprite_frame_1box gSpriteBank04Frame029 = {
    SPRITE_FRAME(gSpriteBank04Frame029, SPRITE_TILES_BANK04 + 0x07260),
    { { -20, -7, 43, 19 } },
};
const struct sprite_frame_1box gSpriteBank04Frame030 = {
    SPRITE_FRAME(gSpriteBank04Frame030, SPRITE_TILES_BANK04 + 0x076a0),
    { { -19, -7, 41, 19 } },
};
const struct sprite_frame_1box gSpriteBank04Frame031 = {
    SPRITE_FRAME(gSpriteBank04Frame031, SPRITE_TILES_BANK04 + 0x07ae0),
    { { -20, -6, 42, 18 } },
};

const struct sprite_piece_pos gSpriteBank04Frame000Pos[3] = { { -28, -16 }, { -21, 16 }, { -5, 16 } };
const struct sprite_piece_pos gSpriteBank04Frame001Pos[3] = { { -27, -17 }, { -21, 15 }, { -3, 15 } };
const struct sprite_piece_pos gSpriteBank04Frame002Pos[2] = { { -28, -17 }, { -22, 15 } };
const struct sprite_piece_pos gSpriteBank04Frame003Pos[3] = { { -29, -17 }, { -23, 15 }, { 9, 15 } };
const struct sprite_piece_pos gSpriteBank04Frame004Pos[3] = { { -28, -17 }, { -22, 15 }, { 13, 15 } };
const struct sprite_piece_pos gSpriteBank04Frame005Pos[2] = { { -28, -17 }, { -20, 15 } };
const struct sprite_piece_pos gSpriteBank04Frame006Pos[3] = { { -27, -16 }, { -15, 16 }, { -7, 16 } };
const struct sprite_piece_pos gSpriteBank04Frame007Pos[2] = { { -23, -16 }, { -9, 16 } };
const struct sprite_piece_pos gSpriteBank04Frame008Pos[4] = { { -16, -15 }, { 16, 3 }, { 24, 8 }, { -7, 17 } };
const struct sprite_piece_pos gSpriteBank04Frame009Pos[4] = { { -10, -16 }, { 22, 9 }, { -6, 16 }, { 10, 16 } };
const struct sprite_piece_pos gSpriteBank04Frame010Pos[3] = { { -14, -16 }, { 18, 8 }, { -6, 16 } };
const struct sprite_piece_pos gSpriteBank04Frame011Pos[4] = { { -18, -16 }, { 14, -17 }, { -12, 15 }, { 20, 15 } };
const struct sprite_piece_pos gSpriteBank04Frame012Pos[5] = { { -22, -15 }, { 10, -17 }, { 18, -3 }, { 18, 13 }, { -6, 15 } };
const struct sprite_piece_pos gSpriteBank04Frame013Pos[2] = { { -24, -17 }, { -5, 15 } };
const struct sprite_piece_pos gSpriteBank04Frame014Pos[2] = { { -26, -17 }, { -1, 15 } };
const struct sprite_piece_pos gSpriteBank04Frame015Pos[3] = { { -26, -17 }, { 3, 15 }, { 22, 15 } };
const struct sprite_piece_pos gSpriteBank04Frame016Pos[3] = { { -27, -17 }, { -22, 15 }, { -5, 15 } };
const struct sprite_piece_pos gSpriteBank04Frame017Pos[2] = { { -29, -17 }, { -23, 15 } };
const struct sprite_piece_pos gSpriteBank04Frame018Pos[2] = { { -29, -17 }, { -23, 15 } };
const struct sprite_piece_pos gSpriteBank04Frame019Pos[2] = { { -27, -18 }, { -22, 14 } };
const struct sprite_piece_pos gSpriteBank04Frame020Pos[2] = { { -24, -18 }, { -3, 14 } };
const struct sprite_piece_pos gSpriteBank04Frame021Pos[4] = { { -20, -18 }, { 12, -9 }, { 20, -1 }, { -3, 14 } };
const struct sprite_piece_pos gSpriteBank04Frame022Pos[4] = { { -17, -18 }, { 15, -7 }, { 23, 3 }, { -4, 14 } };
const struct sprite_piece_pos gSpriteBank04Frame023Pos[4] = { { -17, -18 }, { 15, -7 }, { 23, 3 }, { -4, 14 } };
const struct sprite_piece_pos gSpriteBank04Frame024Pos[4] = { { -18, -18 }, { 14, -7 }, { 22, 1 }, { -5, 14 } };
const struct sprite_piece_pos gSpriteBank04Frame025Pos[2] = { { -22, -17 }, { -5, 15 } };
const struct sprite_piece_pos gSpriteBank04Frame026Pos[3] = { { -24, -17 }, { -20, 15 }, { -4, 15 } };
const struct sprite_piece_pos gSpriteBank04Frame027Pos[3] = { { -26, -17 }, { -22, 15 }, { -6, 15 } };
const struct sprite_piece_pos gSpriteBank04Frame028Pos[3] = { { -28, -17 }, { -23, 15 }, { -6, 15 } };
const struct sprite_piece_pos gSpriteBank04Frame029Pos[3] = { { -27, -17 }, { -23, 15 }, { -6, 15 } };
const struct sprite_piece_pos gSpriteBank04Frame030Pos[3] = { { -27, -17 }, { -22, 15 }, { -5, 15 } };
const struct sprite_piece_pos gSpriteBank04Frame031Pos[3] = { { -27, -17 }, { -22, 15 }, { -5, 15 } };

const u8 gSpriteBank04Frame000Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank04Frame001Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank04Frame002Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank04Frame003Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank04Frame004Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank04Frame005Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank04Frame006Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank04Frame007Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank04Frame008Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank04Frame009Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank04Frame010Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank04Frame011Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 6), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank04Frame012Pieces[5] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank04Frame013Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank04Frame014Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank04Frame015Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank04Frame016Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank04Frame017Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank04Frame018Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank04Frame019Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank04Frame020Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank04Frame021Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank04Frame022Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank04Frame023Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank04Frame024Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank04Frame025Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank04Frame026Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank04Frame027Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank04Frame028Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank04Frame029Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank04Frame030Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank04Frame031Pieces[3] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };

/* ---------------------------------------------------------------------- */
/* Bank 5: 4 animations, 25 frames, tiles in gSpriteBank05Tiles (SPRITE_TILES_BANK05). */

extern const u16 gSpriteBank05Anim00Seq[7];
extern const u16 gSpriteBank05Anim01Seq[9];
extern const u16 gSpriteBank05Anim02Seq[5];
extern const u16 gSpriteBank05Anim03Seq[7];
extern const struct sprite_frame_1box gSpriteBank05Frame000;
extern const struct sprite_frame_1box gSpriteBank05Frame001;
extern const struct sprite_frame_1box gSpriteBank05Frame002;
extern const struct sprite_frame_1box gSpriteBank05Frame003;
extern const struct sprite_frame_1box gSpriteBank05Frame004;
extern const struct sprite_frame_1box gSpriteBank05Frame005;
extern const struct sprite_frame_1box gSpriteBank05Frame006;
extern const struct sprite_frame_1box gSpriteBank05Frame007;
extern const struct sprite_frame_1box gSpriteBank05Frame008;
extern const struct sprite_frame_1box gSpriteBank05Frame009;
extern const struct sprite_frame_1box gSpriteBank05Frame010;
extern const struct sprite_frame_1box gSpriteBank05Frame011;
extern const struct sprite_frame_1box gSpriteBank05Frame012;
extern const struct sprite_frame_1box gSpriteBank05Frame013;
extern const struct sprite_frame_1box gSpriteBank05Frame014;
extern const struct sprite_frame_1box gSpriteBank05Frame015;
extern const struct sprite_frame_1box gSpriteBank05Frame016;
extern const struct sprite_frame_1box gSpriteBank05Frame017;
extern const struct sprite_frame_1box gSpriteBank05Frame018;
extern const struct sprite_frame_1box gSpriteBank05Frame019;
extern const struct sprite_frame_1box gSpriteBank05Frame020;
extern const struct sprite_frame_1box gSpriteBank05Frame021;
extern const struct sprite_frame_1box gSpriteBank05Frame022;
extern const struct sprite_frame_1box gSpriteBank05Frame023;
extern const struct sprite_frame_1box gSpriteBank05Frame024;
extern const struct sprite_piece_pos gSpriteBank05Frame000Pos[1];
extern const struct sprite_piece_pos gSpriteBank05Frame001Pos[1];
extern const struct sprite_piece_pos gSpriteBank05Frame002Pos[1];
extern const struct sprite_piece_pos gSpriteBank05Frame003Pos[1];
extern const struct sprite_piece_pos gSpriteBank05Frame004Pos[4];
extern const struct sprite_piece_pos gSpriteBank05Frame005Pos[4];
extern const struct sprite_piece_pos gSpriteBank05Frame006Pos[3];
extern const struct sprite_piece_pos gSpriteBank05Frame007Pos[3];
extern const struct sprite_piece_pos gSpriteBank05Frame008Pos[3];
extern const struct sprite_piece_pos gSpriteBank05Frame009Pos[4];
extern const struct sprite_piece_pos gSpriteBank05Frame010Pos[4];
extern const struct sprite_piece_pos gSpriteBank05Frame011Pos[3];
extern const struct sprite_piece_pos gSpriteBank05Frame012Pos[3];
extern const struct sprite_piece_pos gSpriteBank05Frame013Pos[3];
extern const struct sprite_piece_pos gSpriteBank05Frame014Pos[3];
extern const struct sprite_piece_pos gSpriteBank05Frame015Pos[3];
extern const struct sprite_piece_pos gSpriteBank05Frame016Pos[1];
extern const struct sprite_piece_pos gSpriteBank05Frame017Pos[1];
extern const struct sprite_piece_pos gSpriteBank05Frame018Pos[1];
extern const struct sprite_piece_pos gSpriteBank05Frame019Pos[1];
extern const struct sprite_piece_pos gSpriteBank05Frame020Pos[1];
extern const struct sprite_piece_pos gSpriteBank05Frame021Pos[3];
extern const struct sprite_piece_pos gSpriteBank05Frame022Pos[4];
extern const struct sprite_piece_pos gSpriteBank05Frame023Pos[4];
extern const struct sprite_piece_pos gSpriteBank05Frame024Pos[1];
extern const u8 gSpriteBank05Frame000Pieces[1];
extern const u8 gSpriteBank05Frame001Pieces[1];
extern const u8 gSpriteBank05Frame002Pieces[1];
extern const u8 gSpriteBank05Frame003Pieces[1];
extern const u8 gSpriteBank05Frame004Pieces[4];
extern const u8 gSpriteBank05Frame005Pieces[4];
extern const u8 gSpriteBank05Frame006Pieces[3];
extern const u8 gSpriteBank05Frame007Pieces[3];
extern const u8 gSpriteBank05Frame008Pieces[3];
extern const u8 gSpriteBank05Frame009Pieces[4];
extern const u8 gSpriteBank05Frame010Pieces[4];
extern const u8 gSpriteBank05Frame011Pieces[3];
extern const u8 gSpriteBank05Frame012Pieces[3];
extern const u8 gSpriteBank05Frame013Pieces[3];
extern const u8 gSpriteBank05Frame014Pieces[3];
extern const u8 gSpriteBank05Frame015Pieces[3];
extern const u8 gSpriteBank05Frame016Pieces[1];
extern const u8 gSpriteBank05Frame017Pieces[1];
extern const u8 gSpriteBank05Frame018Pieces[1];
extern const u8 gSpriteBank05Frame019Pieces[1];
extern const u8 gSpriteBank05Frame020Pieces[1];
extern const u8 gSpriteBank05Frame021Pieces[3];
extern const u8 gSpriteBank05Frame022Pieces[4];
extern const u8 gSpriteBank05Frame023Pieces[4];
extern const u8 gSpriteBank05Frame024Pieces[1];

const struct sprite_anim gSpriteBank05Anims[4] = {
    [0] = {
        .seq = gSpriteBank05Anim00Seq,
        .box = { { -12, -7, 25, 14 }, { -22, -24, 40, 44 } },
        .tileRecord = 11,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank05Anim00Seq),
        .flags = 0,
    },
    [1] = {
        .seq = gSpriteBank05Anim01Seq,
        .box = { { -12, -7, 25, 14 }, { -21, -21, 40, 40 } },
        .tileRecord = 11,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank05Anim01Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [2] = {
        .seq = gSpriteBank05Anim02Seq,
        .box = { { -12, -7, 25, 14 }, { -15, -7, 28, 14 } },
        .tileRecord = 11,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank05Anim02Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [3] = {
        .seq = gSpriteBank05Anim03Seq,
        .box = { { -12, -7, 25, 14 }, { -22, -24, 40, 44 } },
        .tileRecord = 11,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank05Anim03Seq),
        .flags = 0,
    },
};

const u16 gSpriteBank05Anim00Seq[7] = {
    0, 1, 2, 3, 4, 5, 6,
};
const u16 gSpriteBank05Anim01Seq[9] = {
    7, 8, 9, 10, 11, 12, 13, 14, 15,
};
const u16 gSpriteBank05Anim02Seq[5] = {
    16, 17, 18, 19, 20,
};
const u16 gSpriteBank05Anim03Seq[7] = {
    21, 22, 23, 24, 2, 1, 0,
};

const struct sprite_frame *const gSpriteBank05Frames[25] = {
    &gSpriteBank05Frame000.frame,
    &gSpriteBank05Frame001.frame,
    &gSpriteBank05Frame002.frame,
    &gSpriteBank05Frame003.frame,
    &gSpriteBank05Frame004.frame,
    &gSpriteBank05Frame005.frame,
    &gSpriteBank05Frame006.frame,
    &gSpriteBank05Frame007.frame,
    &gSpriteBank05Frame008.frame,
    &gSpriteBank05Frame009.frame,
    &gSpriteBank05Frame010.frame,
    &gSpriteBank05Frame011.frame,
    &gSpriteBank05Frame012.frame,
    &gSpriteBank05Frame013.frame,
    &gSpriteBank05Frame014.frame,
    &gSpriteBank05Frame015.frame,
    &gSpriteBank05Frame016.frame,
    &gSpriteBank05Frame017.frame,
    &gSpriteBank05Frame018.frame,
    &gSpriteBank05Frame019.frame,
    &gSpriteBank05Frame020.frame,
    &gSpriteBank05Frame021.frame,
    &gSpriteBank05Frame022.frame,
    &gSpriteBank05Frame023.frame,
    &gSpriteBank05Frame024.frame,
};

const struct sprite_frame_1box gSpriteBank05Frame000 = {
    SPRITE_FRAME(gSpriteBank05Frame000, SPRITE_TILES_BANK05 + 0x00000),
    { { -12, -6, 25, 11 } },
};
const struct sprite_frame_1box gSpriteBank05Frame001 = {
    SPRITE_FRAME(gSpriteBank05Frame001, SPRITE_TILES_BANK05 + 0x00100),
    { { -13, -5, 27, 10 } },
};
const struct sprite_frame_1box gSpriteBank05Frame002 = {
    SPRITE_FRAME(gSpriteBank05Frame002, SPRITE_TILES_BANK05 + 0x00200),
    { { -14, -5, 28, 15 } },
};
const struct sprite_frame_1box gSpriteBank05Frame003 = {
    SPRITE_FRAME(gSpriteBank05Frame003, SPRITE_TILES_BANK05 + 0x00300),
    { { -14, -7, 25, 16 } },
};
const struct sprite_frame_1box gSpriteBank05Frame004 = {
    SPRITE_FRAME(gSpriteBank05Frame004, SPRITE_TILES_BANK05 + 0x00500),
    { { -14, -9, 26, 23 } },
};
const struct sprite_frame_1box gSpriteBank05Frame005 = {
    SPRITE_FRAME(gSpriteBank05Frame005, SPRITE_TILES_BANK05 + 0x007e0),
    { { -16, -15, 29, 25 } },
};
const struct sprite_frame_1box gSpriteBank05Frame006 = {
    SPRITE_FRAME(gSpriteBank05Frame006, SPRITE_TILES_BANK05 + 0x00b80),
    { { -14, -13, 28, 25 } },
};
const struct sprite_frame_1box gSpriteBank05Frame007 = {
    SPRITE_FRAME(gSpriteBank05Frame007, SPRITE_TILES_BANK05 + 0x00f00),
    { { -13, -17, 26, 31 } },
};
const struct sprite_frame_1box gSpriteBank05Frame008 = {
    SPRITE_FRAME(gSpriteBank05Frame008, SPRITE_TILES_BANK05 + 0x01280),
    { { -12, -15, 25, 24 } },
};
const struct sprite_frame_1box gSpriteBank05Frame009 = {
    SPRITE_FRAME(gSpriteBank05Frame009, SPRITE_TILES_BANK05 + 0x01600),
    { { -11, -14, 24, 26 } },
};
const struct sprite_frame_1box gSpriteBank05Frame010 = {
    SPRITE_FRAME(gSpriteBank05Frame010, SPRITE_TILES_BANK05 + 0x019a0),
    { { -11, -14, 24, 27 } },
};
const struct sprite_frame_1box gSpriteBank05Frame011 = {
    SPRITE_FRAME(gSpriteBank05Frame011, SPRITE_TILES_BANK05 + 0x01d40),
    { { -11, -14, 23, 27 } },
};
const struct sprite_frame_1box gSpriteBank05Frame012 = {
    SPRITE_FRAME(gSpriteBank05Frame012, SPRITE_TILES_BANK05 + 0x020c0),
    { { -12, -13, 24, 24 } },
};
const struct sprite_frame_1box gSpriteBank05Frame013 = {
    SPRITE_FRAME(gSpriteBank05Frame013, SPRITE_TILES_BANK05 + 0x02440),
    { { -14, -13, 26, 25 } },
};
const struct sprite_frame_1box gSpriteBank05Frame014 = {
    SPRITE_FRAME(gSpriteBank05Frame014, SPRITE_TILES_BANK05 + 0x027c0),
    { { -13, -13, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank05Frame015 = {
    SPRITE_FRAME(gSpriteBank05Frame015, SPRITE_TILES_BANK05 + 0x02b40),
    { { -13, -12, 25, 23 } },
};
const struct sprite_frame_1box gSpriteBank05Frame016 = {
    SPRITE_FRAME(gSpriteBank05Frame016, SPRITE_TILES_BANK05 + 0x02ec0),
    { { -12, -7, 25, 14 } },
};
const struct sprite_frame_1box gSpriteBank05Frame017 = {
    SPRITE_FRAME(gSpriteBank05Frame017, SPRITE_TILES_BANK05 + 0x02fc0),
    { { -14, -6, 27, 13 } },
};
const struct sprite_frame_1box gSpriteBank05Frame018 = {
    SPRITE_FRAME(gSpriteBank05Frame018, SPRITE_TILES_BANK05 + 0x030c0),
    { { -15, -6, 28, 13 } },
};
const struct sprite_frame_1box gSpriteBank05Frame019 = {
    SPRITE_FRAME(gSpriteBank05Frame019, SPRITE_TILES_BANK05 + 0x031c0),
    { { -14, -7, 27, 13 } },
};
const struct sprite_frame_1box gSpriteBank05Frame020 = {
    SPRITE_FRAME(gSpriteBank05Frame020, SPRITE_TILES_BANK05 + 0x032c0),
    { { -12, -7, 25, 13 } },
};
const struct sprite_frame_1box gSpriteBank05Frame021 = {
    SPRITE_FRAME(gSpriteBank05Frame021, SPRITE_TILES_BANK05 + 0x00b80),
    { { -13, -15, 25, 26 } },
};
const struct sprite_frame_1box gSpriteBank05Frame022 = {
    SPRITE_FRAME(gSpriteBank05Frame022, SPRITE_TILES_BANK05 + 0x007e0),
    { { -14, -15, 26, 26 } },
};
const struct sprite_frame_1box gSpriteBank05Frame023 = {
    SPRITE_FRAME(gSpriteBank05Frame023, SPRITE_TILES_BANK05 + 0x00500),
    { { -13, -13, 24, 27 } },
};
const struct sprite_frame_1box gSpriteBank05Frame024 = {
    SPRITE_FRAME(gSpriteBank05Frame024, SPRITE_TILES_BANK05 + 0x00300),
    { { -13, -7, 23, 17 } },
};

const struct sprite_piece_pos gSpriteBank05Frame000Pos[1] = { { -12, -6 } };
const struct sprite_piece_pos gSpriteBank05Frame001Pos[1] = { { -13, -5 } };
const struct sprite_piece_pos gSpriteBank05Frame002Pos[1] = { { -14, -5 } };
const struct sprite_piece_pos gSpriteBank05Frame003Pos[1] = { { -17, -11 } };
const struct sprite_piece_pos gSpriteBank05Frame004Pos[4] = { { -20, -19 }, { 12, -11 }, { 12, 5 }, { -12, 13 } };
const struct sprite_piece_pos gSpriteBank05Frame005Pos[4] = { { -22, -24 }, { 10, -19 }, { 18, -10 }, { -16, 8 } };
const struct sprite_piece_pos gSpriteBank05Frame006Pos[3] = { { -21, -22 }, { 11, -18 }, { -14, 10 } };
const struct sprite_piece_pos gSpriteBank05Frame007Pos[3] = { { -20, -21 }, { 12, -17 }, { -12, 11 } };
const struct sprite_piece_pos gSpriteBank05Frame008Pos[3] = { { -20, -21 }, { 12, -15 }, { -11, 11 } };
const struct sprite_piece_pos gSpriteBank05Frame009Pos[4] = { { -21, -21 }, { 11, -16 }, { 19, -7 }, { -11, 11 } };
const struct sprite_piece_pos gSpriteBank05Frame010Pos[4] = { { -21, -21 }, { 11, -16 }, { 19, -7 }, { -11, 11 } };
const struct sprite_piece_pos gSpriteBank05Frame011Pos[3] = { { -20, -21 }, { 12, -15 }, { -11, 11 } };
const struct sprite_piece_pos gSpriteBank05Frame012Pos[3] = { { -20, -21 }, { 12, -17 }, { -12, 11 } };
const struct sprite_piece_pos gSpriteBank05Frame013Pos[3] = { { -20, -21 }, { 12, -17 }, { -13, 11 } };
const struct sprite_piece_pos gSpriteBank05Frame014Pos[3] = { { -19, -21 }, { 13, -15 }, { -13, 11 } };
const struct sprite_piece_pos gSpriteBank05Frame015Pos[3] = { { -20, -21 }, { 12, -17 }, { -13, 11 } };
const struct sprite_piece_pos gSpriteBank05Frame016Pos[1] = { { -12, -7 } };
const struct sprite_piece_pos gSpriteBank05Frame017Pos[1] = { { -14, -6 } };
const struct sprite_piece_pos gSpriteBank05Frame018Pos[1] = { { -15, -6 } };
const struct sprite_piece_pos gSpriteBank05Frame019Pos[1] = { { -14, -7 } };
const struct sprite_piece_pos gSpriteBank05Frame020Pos[1] = { { -12, -7 } };
const struct sprite_piece_pos gSpriteBank05Frame021Pos[3] = { { -21, -22 }, { 11, -18 }, { -14, 10 } };
const struct sprite_piece_pos gSpriteBank05Frame022Pos[4] = { { -22, -24 }, { 10, -19 }, { 18, -10 }, { -16, 8 } };
const struct sprite_piece_pos gSpriteBank05Frame023Pos[4] = { { -20, -19 }, { 12, -11 }, { 12, 5 }, { -12, 13 } };
const struct sprite_piece_pos gSpriteBank05Frame024Pos[1] = { { -17, -11 } };

const u8 gSpriteBank05Frame000Pieces[1] = { SPRITE_PIECE(2, 6) };
const u8 gSpriteBank05Frame001Pieces[1] = { SPRITE_PIECE(2, 6) };
const u8 gSpriteBank05Frame002Pieces[1] = { SPRITE_PIECE(2, 6) };
const u8 gSpriteBank05Frame003Pieces[1] = { SPRITE_PIECE(5, 2) };
const u8 gSpriteBank05Frame004Pieces[4] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank05Frame005Pieces[4] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank05Frame006Pieces[3] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank05Frame007Pieces[3] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank05Frame008Pieces[3] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank05Frame009Pieces[4] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank05Frame010Pieces[4] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank05Frame011Pieces[3] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank05Frame012Pieces[3] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank05Frame013Pieces[3] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank05Frame014Pieces[3] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank05Frame015Pieces[3] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank05Frame016Pieces[1] = { SPRITE_PIECE(2, 6) };
const u8 gSpriteBank05Frame017Pieces[1] = { SPRITE_PIECE(2, 6) };
const u8 gSpriteBank05Frame018Pieces[1] = { SPRITE_PIECE(2, 6) };
const u8 gSpriteBank05Frame019Pieces[1] = { SPRITE_PIECE(2, 6) };
const u8 gSpriteBank05Frame020Pieces[1] = { SPRITE_PIECE(2, 6) };
const u8 gSpriteBank05Frame021Pieces[3] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank05Frame022Pieces[4] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank05Frame023Pieces[4] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank05Frame024Pieces[1] = { SPRITE_PIECE(5, 2) };

/* ---------------------------------------------------------------------- */
/* Bank 6: 1 animation, 7 frames, tiles in gSpriteBank06Tiles (SPRITE_TILES_BANK06). */

extern const u16 gSpriteBank06Anim00Seq[7];
extern const struct sprite_frame_1box gSpriteBank06Frame000;
extern const struct sprite_frame_1box gSpriteBank06Frame001;
extern const struct sprite_frame_1box gSpriteBank06Frame002;
extern const struct sprite_frame_1box gSpriteBank06Frame003;
extern const struct sprite_frame_1box gSpriteBank06Frame004;
extern const struct sprite_frame_1box gSpriteBank06Frame005;
extern const struct sprite_frame_1box gSpriteBank06Frame006;
extern const struct sprite_piece_pos gSpriteBank06Frame000Pos[4];
extern const struct sprite_piece_pos gSpriteBank06Frame001Pos[4];
extern const struct sprite_piece_pos gSpriteBank06Frame002Pos[4];
extern const struct sprite_piece_pos gSpriteBank06Frame003Pos[5];
extern const struct sprite_piece_pos gSpriteBank06Frame004Pos[5];
extern const struct sprite_piece_pos gSpriteBank06Frame005Pos[5];
extern const struct sprite_piece_pos gSpriteBank06Frame006Pos[4];
extern const u8 gSpriteBank06Frame000Pieces[4];
extern const u8 gSpriteBank06Frame001Pieces[4];
extern const u8 gSpriteBank06Frame002Pieces[4];
extern const u8 gSpriteBank06Frame003Pieces[5];
extern const u8 gSpriteBank06Frame004Pieces[5];
extern const u8 gSpriteBank06Frame005Pieces[5];
extern const u8 gSpriteBank06Frame006Pieces[4];

const struct sprite_anim gSpriteBank06Anims[1] = {
    [0] = {
        .seq = gSpriteBank06Anim00Seq,
        .box = { { -19, -18, 39, 36 }, { -19, -18, 39, 38 } },
        .tileRecord = 12,
        .duration = 5,
        .frameCount = ARRAY_COUNT(gSpriteBank06Anim00Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank06Anim00Seq[7] = {
    0, 1, 2, 3, 4, 5, 6,
};

const struct sprite_frame *const gSpriteBank06Frames[7] = {
    &gSpriteBank06Frame000.frame,
    &gSpriteBank06Frame001.frame,
    &gSpriteBank06Frame002.frame,
    &gSpriteBank06Frame003.frame,
    &gSpriteBank06Frame004.frame,
    &gSpriteBank06Frame005.frame,
    &gSpriteBank06Frame006.frame,
};

const struct sprite_frame_1box gSpriteBank06Frame000 = {
    SPRITE_FRAME(gSpriteBank06Frame000, SPRITE_TILES_BANK06 + 0x00000),
    { { -11, -11, 23, 24 } },
};
const struct sprite_frame_1box gSpriteBank06Frame001 = {
    SPRITE_FRAME(gSpriteBank06Frame001, SPRITE_TILES_BANK06 + 0x002e0),
    { { -13, -10, 25, 21 } },
};
const struct sprite_frame_1box gSpriteBank06Frame002 = {
    SPRITE_FRAME(gSpriteBank06Frame002, SPRITE_TILES_BANK06 + 0x005c0),
    { { -12, -11, 24, 25 } },
};
const struct sprite_frame_1box gSpriteBank06Frame003 = {
    SPRITE_FRAME(gSpriteBank06Frame003, SPRITE_TILES_BANK06 + 0x008a0),
    { { -12, -10, 24, 24 } },
};
const struct sprite_frame_1box gSpriteBank06Frame004 = {
    SPRITE_FRAME(gSpriteBank06Frame004, SPRITE_TILES_BANK06 + 0x00b60),
    { { -12, -11, 25, 24 } },
};
const struct sprite_frame_1box gSpriteBank06Frame005 = {
    SPRITE_FRAME(gSpriteBank06Frame005, SPRITE_TILES_BANK06 + 0x00e20),
    { { -12, -10, 24, 22 } },
};
const struct sprite_frame_1box gSpriteBank06Frame006 = {
    SPRITE_FRAME(gSpriteBank06Frame006, SPRITE_TILES_BANK06 + 0x010e0),
    { { -13, -12, 25, 25 } },
};

const struct sprite_piece_pos gSpriteBank06Frame000Pos[4] = { { -19, -18 }, { 13, -7 }, { -10, 14 }, { 6, 14 } };
const struct sprite_piece_pos gSpriteBank06Frame001Pos[4] = { { -19, -18 }, { 13, -7 }, { -10, 14 }, { 6, 14 } };
const struct sprite_piece_pos gSpriteBank06Frame002Pos[4] = { { -19, -17 }, { 13, -6 }, { -10, 15 }, { 6, 15 } };
const struct sprite_piece_pos gSpriteBank06Frame003Pos[5] = { { -18, -17 }, { 14, -6 }, { 14, 10 }, { -10, 15 }, { 6, 15 } };
const struct sprite_piece_pos gSpriteBank06Frame004Pos[5] = { { -18, -17 }, { 14, -6 }, { 14, 10 }, { -10, 15 }, { 6, 15 } };
const struct sprite_piece_pos gSpriteBank06Frame005Pos[5] = { { -18, -18 }, { 14, -6 }, { 14, 10 }, { -10, 14 }, { 6, 14 } };
const struct sprite_piece_pos gSpriteBank06Frame006Pos[4] = { { -19, -18 }, { 13, -7 }, { -10, 14 }, { 6, 14 } };

const u8 gSpriteBank06Frame000Pieces[4] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank06Frame001Pieces[4] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank06Frame002Pieces[4] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank06Frame003Pieces[5] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank06Frame004Pieces[5] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank06Frame005Pieces[5] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank06Frame006Pieces[4] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 9), SPRITE_PIECE(0, 4), SPRITE_PIECE(0, 0) };

/* ---------------------------------------------------------------------- */
/* Bank 7: 3 animations, 38 frames, tiles in gSpriteBank07Tiles (SPRITE_TILES_BANK07). */

extern const u16 gSpriteBank07Anim00Seq[9];
extern const u16 gSpriteBank07Anim01Seq[9];
extern const u16 gSpriteBank07Anim02Seq[20];
extern const struct sprite_frame gSpriteBank07Frame000;
extern const struct sprite_frame gSpriteBank07Frame001;
extern const struct sprite_frame gSpriteBank07Frame002;
extern const struct sprite_frame gSpriteBank07Frame003;
extern const struct sprite_frame gSpriteBank07Frame004;
extern const struct sprite_frame gSpriteBank07Frame005;
extern const struct sprite_frame gSpriteBank07Frame006;
extern const struct sprite_frame gSpriteBank07Frame007;
extern const struct sprite_frame gSpriteBank07Frame008;
extern const struct sprite_frame_1box gSpriteBank07Frame009;
extern const struct sprite_frame_1box gSpriteBank07Frame010;
extern const struct sprite_frame_1box gSpriteBank07Frame011;
extern const struct sprite_frame_1box gSpriteBank07Frame012;
extern const struct sprite_frame_1box gSpriteBank07Frame013;
extern const struct sprite_frame_1box gSpriteBank07Frame014;
extern const struct sprite_frame_1box gSpriteBank07Frame015;
extern const struct sprite_frame_1box gSpriteBank07Frame016;
extern const struct sprite_frame_1box gSpriteBank07Frame017;
extern const struct sprite_frame_1box gSpriteBank07Frame018;
extern const struct sprite_frame_1box gSpriteBank07Frame019;
extern const struct sprite_frame_1box gSpriteBank07Frame020;
extern const struct sprite_frame_1box gSpriteBank07Frame021;
extern const struct sprite_frame_1box gSpriteBank07Frame022;
extern const struct sprite_frame_1box gSpriteBank07Frame023;
extern const struct sprite_frame_1box gSpriteBank07Frame024;
extern const struct sprite_frame_1box gSpriteBank07Frame025;
extern const struct sprite_frame_1box gSpriteBank07Frame026;
extern const struct sprite_frame_1box gSpriteBank07Frame027;
extern const struct sprite_frame_1box gSpriteBank07Frame028;
extern const struct sprite_frame_1box gSpriteBank07Frame029;
extern const struct sprite_frame_1box gSpriteBank07Frame030;
extern const struct sprite_frame_1box gSpriteBank07Frame031;
extern const struct sprite_frame_1box gSpriteBank07Frame032;
extern const struct sprite_frame gSpriteBank07Frame033;
extern const struct sprite_frame gSpriteBank07Frame034;
extern const struct sprite_frame gSpriteBank07Frame035;
extern const struct sprite_frame gSpriteBank07Frame036;
extern const struct sprite_frame gSpriteBank07Frame037;
extern const struct sprite_piece_pos gSpriteBank07Frame000Pos[4];
extern const struct sprite_piece_pos gSpriteBank07Frame001Pos[1];
extern const struct sprite_piece_pos gSpriteBank07Frame002Pos[1];
extern const struct sprite_piece_pos gSpriteBank07Frame003Pos[1];
extern const struct sprite_piece_pos gSpriteBank07Frame004Pos[4];
extern const struct sprite_piece_pos gSpriteBank07Frame005Pos[2];
extern const struct sprite_piece_pos gSpriteBank07Frame006Pos[1];
extern const struct sprite_piece_pos gSpriteBank07Frame007Pos[2];
extern const struct sprite_piece_pos gSpriteBank07Frame008Pos[4];
extern const struct sprite_piece_pos gSpriteBank07Frame009Pos[4];
extern const struct sprite_piece_pos gSpriteBank07Frame010Pos[3];
extern const struct sprite_piece_pos gSpriteBank07Frame011Pos[1];
extern const struct sprite_piece_pos gSpriteBank07Frame012Pos[1];
extern const struct sprite_piece_pos gSpriteBank07Frame013Pos[2];
extern const struct sprite_piece_pos gSpriteBank07Frame014Pos[2];
extern const struct sprite_piece_pos gSpriteBank07Frame015Pos[1];
extern const struct sprite_piece_pos gSpriteBank07Frame016Pos[4];
extern const struct sprite_piece_pos gSpriteBank07Frame017Pos[5];
extern const struct sprite_piece_pos gSpriteBank07Frame018Pos[4];
extern const struct sprite_piece_pos gSpriteBank07Frame019Pos[3];
extern const struct sprite_piece_pos gSpriteBank07Frame020Pos[4];
extern const struct sprite_piece_pos gSpriteBank07Frame021Pos[3];
extern const struct sprite_piece_pos gSpriteBank07Frame022Pos[2];
extern const struct sprite_piece_pos gSpriteBank07Frame023Pos[1];
extern const struct sprite_piece_pos gSpriteBank07Frame024Pos[1];
extern const struct sprite_piece_pos gSpriteBank07Frame025Pos[1];
extern const struct sprite_piece_pos gSpriteBank07Frame026Pos[1];
extern const struct sprite_piece_pos gSpriteBank07Frame027Pos[3];
extern const struct sprite_piece_pos gSpriteBank07Frame028Pos[3];
extern const struct sprite_piece_pos gSpriteBank07Frame029Pos[3];
extern const struct sprite_piece_pos gSpriteBank07Frame030Pos[3];
extern const struct sprite_piece_pos gSpriteBank07Frame031Pos[4];
extern const struct sprite_piece_pos gSpriteBank07Frame032Pos[4];
extern const struct sprite_piece_pos gSpriteBank07Frame033Pos[3];
extern const struct sprite_piece_pos gSpriteBank07Frame034Pos[3];
extern const struct sprite_piece_pos gSpriteBank07Frame035Pos[4];
extern const struct sprite_piece_pos gSpriteBank07Frame036Pos[3];
extern const struct sprite_piece_pos gSpriteBank07Frame037Pos[3];
extern const u8 gSpriteBank07Frame000Pieces[4];
extern const u8 gSpriteBank07Frame001Pieces[1];
extern const u8 gSpriteBank07Frame002Pieces[1];
extern const u8 gSpriteBank07Frame003Pieces[1];
extern const u8 gSpriteBank07Frame004Pieces[4];
extern const u8 gSpriteBank07Frame005Pieces[2];
extern const u8 gSpriteBank07Frame006Pieces[1];
extern const u8 gSpriteBank07Frame007Pieces[2];
extern const u8 gSpriteBank07Frame008Pieces[4];
extern const u8 gSpriteBank07Frame009Pieces[4];
extern const u8 gSpriteBank07Frame010Pieces[3];
extern const u8 gSpriteBank07Frame011Pieces[1];
extern const u8 gSpriteBank07Frame012Pieces[1];
extern const u8 gSpriteBank07Frame013Pieces[2];
extern const u8 gSpriteBank07Frame014Pieces[2];
extern const u8 gSpriteBank07Frame015Pieces[1];
extern const u8 gSpriteBank07Frame016Pieces[4];
extern const u8 gSpriteBank07Frame017Pieces[5];
extern const u8 gSpriteBank07Frame018Pieces[4];
extern const u8 gSpriteBank07Frame019Pieces[3];
extern const u8 gSpriteBank07Frame020Pieces[4];
extern const u8 gSpriteBank07Frame021Pieces[3];
extern const u8 gSpriteBank07Frame022Pieces[2];
extern const u8 gSpriteBank07Frame023Pieces[1];
extern const u8 gSpriteBank07Frame024Pieces[1];
extern const u8 gSpriteBank07Frame025Pieces[1];
extern const u8 gSpriteBank07Frame026Pieces[1];
extern const u8 gSpriteBank07Frame027Pieces[3];
extern const u8 gSpriteBank07Frame028Pieces[3];
extern const u8 gSpriteBank07Frame029Pieces[3];
extern const u8 gSpriteBank07Frame030Pieces[3];
extern const u8 gSpriteBank07Frame031Pieces[4];
extern const u8 gSpriteBank07Frame032Pieces[4];
extern const u8 gSpriteBank07Frame033Pieces[3];
extern const u8 gSpriteBank07Frame034Pieces[3];
extern const u8 gSpriteBank07Frame035Pieces[4];
extern const u8 gSpriteBank07Frame036Pieces[3];
extern const u8 gSpriteBank07Frame037Pieces[3];

const struct sprite_anim gSpriteBank07Anims[3] = {
    [0] = {
        .seq = gSpriteBank07Anim00Seq,
        .box = { { -20, -19, 40, 38 }, { -26, -18, 56, 36 } },
        .tileRecord = 13,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank07Anim00Seq),
        .flags = 0,
    },
    [1] = {
        .seq = gSpriteBank07Anim01Seq,
        .box = { { -20, -19, 40, 38 }, { -24, -25, 49, 51 } },
        .tileRecord = 13,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank07Anim01Seq),
        .flags = 0,
    },
    [2] = {
        .seq = gSpriteBank07Anim02Seq,
        .box = { { -20, -19, 40, 38 }, { -24, -26, 48, 53 } },
        .tileRecord = 13,
        .duration = 1,
        .frameCount = ARRAY_COUNT(gSpriteBank07Anim02Seq),
        .flags = 0,
    },
};

const u16 gSpriteBank07Anim00Seq[9] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8,
};
const u16 gSpriteBank07Anim01Seq[9] = {
    9, 10, 11, 12, 13, 14, 15, 16, 17,
};
const u16 gSpriteBank07Anim02Seq[20] = {
    18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
    34, 35, 36, 37,
};

const struct sprite_frame *const gSpriteBank07Frames[38] = {
    &gSpriteBank07Frame000,
    &gSpriteBank07Frame001,
    &gSpriteBank07Frame002,
    &gSpriteBank07Frame003,
    &gSpriteBank07Frame004,
    &gSpriteBank07Frame005,
    &gSpriteBank07Frame006,
    &gSpriteBank07Frame007,
    &gSpriteBank07Frame008,
    &gSpriteBank07Frame009.frame,
    &gSpriteBank07Frame010.frame,
    &gSpriteBank07Frame011.frame,
    &gSpriteBank07Frame012.frame,
    &gSpriteBank07Frame013.frame,
    &gSpriteBank07Frame014.frame,
    &gSpriteBank07Frame015.frame,
    &gSpriteBank07Frame016.frame,
    &gSpriteBank07Frame017.frame,
    &gSpriteBank07Frame018.frame,
    &gSpriteBank07Frame019.frame,
    &gSpriteBank07Frame020.frame,
    &gSpriteBank07Frame021.frame,
    &gSpriteBank07Frame022.frame,
    &gSpriteBank07Frame023.frame,
    &gSpriteBank07Frame024.frame,
    &gSpriteBank07Frame025.frame,
    &gSpriteBank07Frame026.frame,
    &gSpriteBank07Frame027.frame,
    &gSpriteBank07Frame028.frame,
    &gSpriteBank07Frame029.frame,
    &gSpriteBank07Frame030.frame,
    &gSpriteBank07Frame031.frame,
    &gSpriteBank07Frame032.frame,
    &gSpriteBank07Frame033,
    &gSpriteBank07Frame034,
    &gSpriteBank07Frame035,
    &gSpriteBank07Frame036,
    &gSpriteBank07Frame037,
};

const struct sprite_frame gSpriteBank07Frame000 = SPRITE_FRAME(gSpriteBank07Frame000, SPRITE_TILES_BANK07 + 0x00000);
const struct sprite_frame gSpriteBank07Frame001 = SPRITE_FRAME(gSpriteBank07Frame001, SPRITE_TILES_BANK07 + 0x00280);
const struct sprite_frame gSpriteBank07Frame002 = SPRITE_FRAME(gSpriteBank07Frame002, SPRITE_TILES_BANK07 + 0x00680);
const struct sprite_frame gSpriteBank07Frame003 = SPRITE_FRAME(gSpriteBank07Frame003, SPRITE_TILES_BANK07 + 0x00a80);
const struct sprite_frame gSpriteBank07Frame004 = SPRITE_FRAME(gSpriteBank07Frame004, SPRITE_TILES_BANK07 + 0x00e80);
const struct sprite_frame gSpriteBank07Frame005 = SPRITE_FRAME(gSpriteBank07Frame005, SPRITE_TILES_BANK07 + 0x01060);
const struct sprite_frame gSpriteBank07Frame006 = SPRITE_FRAME(gSpriteBank07Frame006, SPRITE_TILES_BANK07 + 0x01260);
const struct sprite_frame gSpriteBank07Frame007 = SPRITE_FRAME(gSpriteBank07Frame007, SPRITE_TILES_BANK07 + 0x015e0);
const struct sprite_frame gSpriteBank07Frame008 = SPRITE_FRAME(gSpriteBank07Frame008, SPRITE_TILES_BANK07 + 0x01a00);
const struct sprite_frame_1box gSpriteBank07Frame009 = {
    SPRITE_FRAME(gSpriteBank07Frame009, SPRITE_TILES_BANK07 + 0x01cc0),
    { { 2, -14, 12, 14 } },
};
const struct sprite_frame_1box gSpriteBank07Frame010 = {
    SPRITE_FRAME(gSpriteBank07Frame010, SPRITE_TILES_BANK07 + 0x01f60),
    { { -2, -14, 11, 12 } },
};
const struct sprite_frame_1box gSpriteBank07Frame011 = {
    SPRITE_FRAME(gSpriteBank07Frame011, SPRITE_TILES_BANK07 + 0x02200),
    { { -4, -19, 7, 14 } },
};
const struct sprite_frame_1box gSpriteBank07Frame012 = {
    SPRITE_FRAME(gSpriteBank07Frame012, SPRITE_TILES_BANK07 + 0x02600),
    { { -9, -18, 9, 14 } },
};
const struct sprite_frame_1box gSpriteBank07Frame013 = {
    SPRITE_FRAME(gSpriteBank07Frame013, SPRITE_TILES_BANK07 + 0x02a00),
    { { -13, -14, 9, 12 } },
};
const struct sprite_frame_1box gSpriteBank07Frame014 = {
    SPRITE_FRAME(gSpriteBank07Frame014, SPRITE_TILES_BANK07 + 0x02c80),
    { { -15, -10, 11, 12 } },
};
const struct sprite_frame_1box gSpriteBank07Frame015 = {
    SPRITE_FRAME(gSpriteBank07Frame015, SPRITE_TILES_BANK07 + 0x030a0),
    { { -15, -10, 11, 10 } },
};
const struct sprite_frame_1box gSpriteBank07Frame016 = {
    SPRITE_FRAME(gSpriteBank07Frame016, SPRITE_TILES_BANK07 + 0x034a0),
    { { -15, -10, 14, 10 } },
};
const struct sprite_frame_1box gSpriteBank07Frame017 = {
    SPRITE_FRAME(gSpriteBank07Frame017, SPRITE_TILES_BANK07 + 0x03720),
    { { -15, -14, 11, 11 } },
};
const struct sprite_frame_1box gSpriteBank07Frame018 = {
    SPRITE_FRAME(gSpriteBank07Frame018, SPRITE_TILES_BANK07 + 0x039c0),
    { { -16, -16, 12, 13 } },
};
const struct sprite_frame_1box gSpriteBank07Frame019 = {
    SPRITE_FRAME(gSpriteBank07Frame019, SPRITE_TILES_BANK07 + 0x03c60),
    { { -14, -13, 11, 10 } },
};
const struct sprite_frame_1box gSpriteBank07Frame020 = {
    SPRITE_FRAME(gSpriteBank07Frame020, SPRITE_TILES_BANK07 + 0x03ec0),
    { { -12, -12, 12, 12 } },
};
const struct sprite_frame_1box gSpriteBank07Frame021 = {
    SPRITE_FRAME(gSpriteBank07Frame021, SPRITE_TILES_BANK07 + 0x04140),
    { { -10, -7, 11, 9 } },
};
const struct sprite_frame_1box gSpriteBank07Frame022 = {
    SPRITE_FRAME(gSpriteBank07Frame022, SPRITE_TILES_BANK07 + 0x043a0),
    { { -8, -7, 10, 11 } },
};
const struct sprite_frame_1box gSpriteBank07Frame023 = {
    SPRITE_FRAME(gSpriteBank07Frame023, SPRITE_TILES_BANK07 + 0x045c0),
    { { -8, -4, 9, 10 } },
};
const struct sprite_frame_1box gSpriteBank07Frame024 = {
    SPRITE_FRAME(gSpriteBank07Frame024, SPRITE_TILES_BANK07 + 0x047c0),
    { { -7, -3, 11, 11 } },
};
const struct sprite_frame_1box gSpriteBank07Frame025 = {
    SPRITE_FRAME(gSpriteBank07Frame025, SPRITE_TILES_BANK07 + 0x049c0),
    { { -4, 1, 9, 9 } },
};
const struct sprite_frame_1box gSpriteBank07Frame026 = {
    SPRITE_FRAME(gSpriteBank07Frame026, SPRITE_TILES_BANK07 + 0x04bc0),
    { { -5, -3, 10, 11 } },
};
const struct sprite_frame_1box gSpriteBank07Frame027 = {
    SPRITE_FRAME(gSpriteBank07Frame027, SPRITE_TILES_BANK07 + 0x04dc0),
    { { -9, -11, 11, 13 } },
};
const struct sprite_frame_1box gSpriteBank07Frame028 = {
    SPRITE_FRAME(gSpriteBank07Frame028, SPRITE_TILES_BANK07 + 0x05060),
    { { -16, -17, 11, 13 } },
};
const struct sprite_frame_1box gSpriteBank07Frame029 = {
    SPRITE_FRAME(gSpriteBank07Frame029, SPRITE_TILES_BANK07 + 0x05500),
    { { -18, -19, 11, 12 } },
};
const struct sprite_frame_1box gSpriteBank07Frame030 = {
    SPRITE_FRAME(gSpriteBank07Frame030, SPRITE_TILES_BANK07 + 0x059a0),
    { { -19, -22, 9, 14 } },
};
const struct sprite_frame_1box gSpriteBank07Frame031 = {
    SPRITE_FRAME(gSpriteBank07Frame031, SPRITE_TILES_BANK07 + 0x05e40),
    { { -20, -21, 12, 13 } },
};
const struct sprite_frame_1box gSpriteBank07Frame032 = {
    SPRITE_FRAME(gSpriteBank07Frame032, SPRITE_TILES_BANK07 + 0x06100),
    { { -21, -20, 12, 11 } },
};
const struct sprite_frame gSpriteBank07Frame033 = SPRITE_FRAME(gSpriteBank07Frame033, SPRITE_TILES_BANK07 + 0x063c0);
const struct sprite_frame gSpriteBank07Frame034 = SPRITE_FRAME(gSpriteBank07Frame034, SPRITE_TILES_BANK07 + 0x06660);
const struct sprite_frame gSpriteBank07Frame035 = SPRITE_FRAME(gSpriteBank07Frame035, SPRITE_TILES_BANK07 + 0x06900);
const struct sprite_frame gSpriteBank07Frame036 = SPRITE_FRAME(gSpriteBank07Frame036, SPRITE_TILES_BANK07 + 0x06b80);
const struct sprite_frame gSpriteBank07Frame037 = SPRITE_FRAME(gSpriteBank07Frame037, SPRITE_TILES_BANK07 + 0x06e20);

const struct sprite_piece_pos gSpriteBank07Frame000Pos[4] = { { -21, -17 }, { 11, 2 }, { 19, 8 }, { 13, 15 } };
const struct sprite_piece_pos gSpriteBank07Frame001Pos[1] = { { -22, -14 } };
const struct sprite_piece_pos gSpriteBank07Frame002Pos[1] = { { -23, -13 } };
const struct sprite_piece_pos gSpriteBank07Frame003Pos[1] = { { -25, -9 } };
const struct sprite_piece_pos gSpriteBank07Frame004Pos[4] = { { -26, -4 }, { 6, -3 }, { 22, 1 }, { 21, 12 } };
const struct sprite_piece_pos gSpriteBank07Frame005Pos[2] = { { -25, -3 }, { 7, -6 } };
const struct sprite_piece_pos gSpriteBank07Frame006Pos[1] = { { -26, -15 } };
const struct sprite_piece_pos gSpriteBank07Frame007Pos[2] = { { -23, -18 }, { -24, 14 } };
const struct sprite_piece_pos gSpriteBank07Frame008Pos[4] = { { -20, -11 }, { 10, -15 }, { 18, -17 }, { -22, 15 } };
const struct sprite_piece_pos gSpriteBank07Frame009Pos[4] = { { -22, -12 }, { 8, -16 }, { 16, -17 }, { -24, 15 } };
const struct sprite_piece_pos gSpriteBank07Frame010Pos[3] = { { -15, -19 }, { -21, 13 }, { -5, 13 } };
const struct sprite_piece_pos gSpriteBank07Frame011Pos[1] = { { -12, -25 } };
const struct sprite_piece_pos gSpriteBank07Frame012Pos[1] = { { -11, -24 } };
const struct sprite_piece_pos gSpriteBank07Frame013Pos[2] = { { -17, -20 }, { 3, 12 } };
const struct sprite_piece_pos gSpriteBank07Frame014Pos[2] = { { -22, -14 }, { 19, 18 } };
const struct sprite_piece_pos gSpriteBank07Frame015Pos[1] = { { -24, -13 } };
const struct sprite_piece_pos gSpriteBank07Frame016Pos[4] = { { -23, -15 }, { 9, 3 }, { 17, 7 }, { 14, 17 } };
const struct sprite_piece_pos gSpriteBank07Frame017Pos[5] = { { -21, -18 }, { 11, 3 }, { 21, 11 }, { 10, 14 }, { 18, 14 } };
const struct sprite_piece_pos gSpriteBank07Frame018Pos[4] = { { -20, -19 }, { 12, 4 }, { 20, 11 }, { 9, 13 } };
const struct sprite_piece_pos gSpriteBank07Frame019Pos[3] = { { -19, -19 }, { 13, 5 }, { 9, 13 } };
const struct sprite_piece_pos gSpriteBank07Frame020Pos[4] = { { -18, -16 }, { 14, 6 }, { 10, 16 }, { 18, 16 } };
const struct sprite_piece_pos gSpriteBank07Frame021Pos[3] = { { -16, -12 }, { 16, 7 }, { 12, 20 } };
const struct sprite_piece_pos gSpriteBank07Frame022Pos[2] = { { -13, -8 }, { 19, 14 } };
const struct sprite_piece_pos gSpriteBank07Frame023Pos[1] = { { -11, -5 } };
const struct sprite_piece_pos gSpriteBank07Frame024Pos[1] = { { -9, -3 } };
const struct sprite_piece_pos gSpriteBank07Frame025Pos[1] = { { -7, -2 } };
const struct sprite_piece_pos gSpriteBank07Frame026Pos[1] = { { -8, -5 } };
const struct sprite_piece_pos gSpriteBank07Frame027Pos[3] = { { -13, -14 }, { 19, 14 }, { 12, 18 } };
const struct sprite_piece_pos gSpriteBank07Frame028Pos[3] = { { -19, -22 }, { 13, 9 }, { 14, 25 } };
const struct sprite_piece_pos gSpriteBank07Frame029Pos[3] = { { -23, -26 }, { 9, 5 }, { 12, 21 } };
const struct sprite_piece_pos gSpriteBank07Frame030Pos[3] = { { -24, -26 }, { 8, 4 }, { 10, 20 } };
const struct sprite_piece_pos gSpriteBank07Frame031Pos[4] = { { -24, -25 }, { 8, 4 }, { 3, 7 }, { 19, 14 } };
const struct sprite_piece_pos gSpriteBank07Frame032Pos[4] = { { -23, -24 }, { 9, 5 }, { 4, 8 }, { 20, 14 } };
const struct sprite_piece_pos gSpriteBank07Frame033Pos[3] = { { -22, -22 }, { 10, 5 }, { 6, 10 } };
const struct sprite_piece_pos gSpriteBank07Frame034Pos[3] = { { -22, -21 }, { 10, 5 }, { 7, 11 } };
const struct sprite_piece_pos gSpriteBank07Frame035Pos[4] = { { -21, -21 }, { 11, 5 }, { 7, 11 }, { 15, 11 } };
const struct sprite_piece_pos gSpriteBank07Frame036Pos[3] = { { -21, -20 }, { 11, 5 }, { 8, 12 } };
const struct sprite_piece_pos gSpriteBank07Frame037Pos[3] = { { -20, -20 }, { 12, 5 }, { 7, 12 } };

const u8 gSpriteBank07Frame000Pieces[4] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank07Frame001Pieces[1] = { SPRITE_PIECE(1, 7) };
const u8 gSpriteBank07Frame002Pieces[1] = { SPRITE_PIECE(1, 7) };
const u8 gSpriteBank07Frame003Pieces[1] = { SPRITE_PIECE(1, 7) };
const u8 gSpriteBank07Frame004Pieces[4] = { SPRITE_PIECE(1, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank07Frame005Pieces[2] = { SPRITE_PIECE(1, 6), SPRITE_PIECE(0, 6) };
const u8 gSpriteBank07Frame006Pieces[1] = { SPRITE_PIECE(1, 7) };
const u8 gSpriteBank07Frame007Pieces[2] = { SPRITE_PIECE(1, 7), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank07Frame008Pieces[4] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank07Frame009Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank07Frame010Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank07Frame011Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank07Frame012Pieces[1] = { SPRITE_PIECE(2, 11) };
const u8 gSpriteBank07Frame013Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank07Frame014Pieces[2] = { SPRITE_PIECE(2, 7), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank07Frame015Pieces[1] = { SPRITE_PIECE(2, 7) };
const u8 gSpriteBank07Frame016Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank07Frame017Pieces[5] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank07Frame018Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank07Frame019Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank07Frame020Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank07Frame021Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank07Frame022Pieces[2] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank07Frame023Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank07Frame024Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank07Frame025Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank07Frame026Pieces[1] = { SPRITE_PIECE(2, 2) };
const u8 gSpriteBank07Frame027Pieces[3] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank07Frame028Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank07Frame029Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank07Frame030Pieces[3] = { SPRITE_PIECE(2, 11), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank07Frame031Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank07Frame032Pieces[4] = { SPRITE_PIECE(2, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank07Frame033Pieces[3] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank07Frame034Pieces[3] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank07Frame035Pieces[4] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank07Frame036Pieces[3] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank07Frame037Pieces[3] = { SPRITE_PIECE(1, 2), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 1) };

/* ---------------------------------------------------------------------- */
/* Bank 8: 3 animations, 36 frames, tiles in gSpriteBank08Tiles (SPRITE_TILES_BANK08). */

extern const u16 gSpriteBank08Anim00Seq[12];
extern const u16 gSpriteBank08Anim01Seq[12];
extern const u16 gSpriteBank08Anim02Seq[12];
extern const struct sprite_frame_1box gSpriteBank08Frame000;
extern const struct sprite_frame_1box gSpriteBank08Frame001;
extern const struct sprite_frame_1box gSpriteBank08Frame002;
extern const struct sprite_frame_1box gSpriteBank08Frame003;
extern const struct sprite_frame_1box gSpriteBank08Frame004;
extern const struct sprite_frame_1box gSpriteBank08Frame005;
extern const struct sprite_frame_1box gSpriteBank08Frame006;
extern const struct sprite_frame_1box gSpriteBank08Frame007;
extern const struct sprite_frame_1box gSpriteBank08Frame008;
extern const struct sprite_frame_1box gSpriteBank08Frame009;
extern const struct sprite_frame_1box gSpriteBank08Frame010;
extern const struct sprite_frame_1box gSpriteBank08Frame011;
extern const struct sprite_frame_1box gSpriteBank08Frame012;
extern const struct sprite_frame_1box gSpriteBank08Frame013;
extern const struct sprite_frame_1box gSpriteBank08Frame014;
extern const struct sprite_frame_1box gSpriteBank08Frame015;
extern const struct sprite_frame_1box gSpriteBank08Frame016;
extern const struct sprite_frame_1box gSpriteBank08Frame017;
extern const struct sprite_frame_1box gSpriteBank08Frame018;
extern const struct sprite_frame_1box gSpriteBank08Frame019;
extern const struct sprite_frame_1box gSpriteBank08Frame020;
extern const struct sprite_frame_1box gSpriteBank08Frame021;
extern const struct sprite_frame_1box gSpriteBank08Frame022;
extern const struct sprite_frame_1box gSpriteBank08Frame023;
extern const struct sprite_frame_1box gSpriteBank08Frame024;
extern const struct sprite_frame_1box gSpriteBank08Frame025;
extern const struct sprite_frame_1box gSpriteBank08Frame026;
extern const struct sprite_frame_1box gSpriteBank08Frame027;
extern const struct sprite_frame_1box gSpriteBank08Frame028;
extern const struct sprite_frame_1box gSpriteBank08Frame029;
extern const struct sprite_frame_1box gSpriteBank08Frame030;
extern const struct sprite_frame_1box gSpriteBank08Frame031;
extern const struct sprite_frame_1box gSpriteBank08Frame032;
extern const struct sprite_frame_1box gSpriteBank08Frame033;
extern const struct sprite_frame_1box gSpriteBank08Frame034;
extern const struct sprite_frame_1box gSpriteBank08Frame035;
extern const struct sprite_piece_pos gSpriteBank08Frame000Pos[3];
extern const struct sprite_piece_pos gSpriteBank08Frame001Pos[2];
extern const struct sprite_piece_pos gSpriteBank08Frame002Pos[2];
extern const struct sprite_piece_pos gSpriteBank08Frame003Pos[3];
extern const struct sprite_piece_pos gSpriteBank08Frame004Pos[3];
extern const struct sprite_piece_pos gSpriteBank08Frame005Pos[3];
extern const struct sprite_piece_pos gSpriteBank08Frame006Pos[4];
extern const struct sprite_piece_pos gSpriteBank08Frame007Pos[4];
extern const struct sprite_piece_pos gSpriteBank08Frame008Pos[3];
extern const struct sprite_piece_pos gSpriteBank08Frame009Pos[3];
extern const struct sprite_piece_pos gSpriteBank08Frame010Pos[2];
extern const struct sprite_piece_pos gSpriteBank08Frame011Pos[2];
extern const struct sprite_piece_pos gSpriteBank08Frame012Pos[3];
extern const struct sprite_piece_pos gSpriteBank08Frame013Pos[4];
extern const struct sprite_piece_pos gSpriteBank08Frame014Pos[3];
extern const struct sprite_piece_pos gSpriteBank08Frame015Pos[6];
extern const struct sprite_piece_pos gSpriteBank08Frame016Pos[5];
extern const struct sprite_piece_pos gSpriteBank08Frame017Pos[2];
extern const struct sprite_piece_pos gSpriteBank08Frame018Pos[2];
extern const struct sprite_piece_pos gSpriteBank08Frame019Pos[3];
extern const struct sprite_piece_pos gSpriteBank08Frame020Pos[4];
extern const struct sprite_piece_pos gSpriteBank08Frame021Pos[1];
extern const struct sprite_piece_pos gSpriteBank08Frame022Pos[2];
extern const struct sprite_piece_pos gSpriteBank08Frame023Pos[4];
extern const struct sprite_piece_pos gSpriteBank08Frame024Pos[3];
extern const struct sprite_piece_pos gSpriteBank08Frame025Pos[2];
extern const struct sprite_piece_pos gSpriteBank08Frame026Pos[3];
extern const struct sprite_piece_pos gSpriteBank08Frame027Pos[3];
extern const struct sprite_piece_pos gSpriteBank08Frame028Pos[2];
extern const struct sprite_piece_pos gSpriteBank08Frame029Pos[2];
extern const struct sprite_piece_pos gSpriteBank08Frame030Pos[2];
extern const struct sprite_piece_pos gSpriteBank08Frame031Pos[4];
extern const struct sprite_piece_pos gSpriteBank08Frame032Pos[4];
extern const struct sprite_piece_pos gSpriteBank08Frame033Pos[2];
extern const struct sprite_piece_pos gSpriteBank08Frame034Pos[3];
extern const struct sprite_piece_pos gSpriteBank08Frame035Pos[4];
extern const u8 gSpriteBank08Frame000Pieces[3];
extern const u8 gSpriteBank08Frame001Pieces[2];
extern const u8 gSpriteBank08Frame002Pieces[2];
extern const u8 gSpriteBank08Frame003Pieces[3];
extern const u8 gSpriteBank08Frame004Pieces[3];
extern const u8 gSpriteBank08Frame005Pieces[3];
extern const u8 gSpriteBank08Frame006Pieces[4];
extern const u8 gSpriteBank08Frame007Pieces[4];
extern const u8 gSpriteBank08Frame008Pieces[3];
extern const u8 gSpriteBank08Frame009Pieces[3];
extern const u8 gSpriteBank08Frame010Pieces[2];
extern const u8 gSpriteBank08Frame011Pieces[2];
extern const u8 gSpriteBank08Frame012Pieces[3];
extern const u8 gSpriteBank08Frame013Pieces[4];
extern const u8 gSpriteBank08Frame014Pieces[3];
extern const u8 gSpriteBank08Frame015Pieces[6];
extern const u8 gSpriteBank08Frame016Pieces[5];
extern const u8 gSpriteBank08Frame017Pieces[2];
extern const u8 gSpriteBank08Frame018Pieces[2];
extern const u8 gSpriteBank08Frame019Pieces[3];
extern const u8 gSpriteBank08Frame020Pieces[4];
extern const u8 gSpriteBank08Frame021Pieces[1];
extern const u8 gSpriteBank08Frame022Pieces[2];
extern const u8 gSpriteBank08Frame023Pieces[4];
extern const u8 gSpriteBank08Frame024Pieces[3];
extern const u8 gSpriteBank08Frame025Pieces[2];
extern const u8 gSpriteBank08Frame026Pieces[3];
extern const u8 gSpriteBank08Frame027Pieces[3];
extern const u8 gSpriteBank08Frame028Pieces[2];
extern const u8 gSpriteBank08Frame029Pieces[2];
extern const u8 gSpriteBank08Frame030Pieces[2];
extern const u8 gSpriteBank08Frame031Pieces[4];
extern const u8 gSpriteBank08Frame032Pieces[4];
extern const u8 gSpriteBank08Frame033Pieces[2];
extern const u8 gSpriteBank08Frame034Pieces[3];
extern const u8 gSpriteBank08Frame035Pieces[4];

const struct sprite_anim gSpriteBank08Anims[3] = {
    [0] = {
        .seq = gSpriteBank08Anim00Seq,
        .box = { { -20, -7, 40, 15 }, { -26, -12, 46, 20 } },
        .tileRecord = 122,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank08Anim00Seq),
        .flags = 0,
    },
    [1] = {
        .seq = gSpriteBank08Anim01Seq,
        .box = { { -20, -7, 40, 15 }, { -28, -17, 49, 31 } },
        .tileRecord = 122,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank08Anim01Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
    [2] = {
        .seq = gSpriteBank08Anim02Seq,
        .box = { { -20, -7, 40, 15 }, { -28, -10, 49, 19 } },
        .tileRecord = 122,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank08Anim02Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank08Anim00Seq[12] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
};
const u16 gSpriteBank08Anim01Seq[12] = {
    12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
};
const u16 gSpriteBank08Anim02Seq[12] = {
    24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
};

const struct sprite_frame *const gSpriteBank08Frames[36] = {
    &gSpriteBank08Frame000.frame,
    &gSpriteBank08Frame001.frame,
    &gSpriteBank08Frame002.frame,
    &gSpriteBank08Frame003.frame,
    &gSpriteBank08Frame004.frame,
    &gSpriteBank08Frame005.frame,
    &gSpriteBank08Frame006.frame,
    &gSpriteBank08Frame007.frame,
    &gSpriteBank08Frame008.frame,
    &gSpriteBank08Frame009.frame,
    &gSpriteBank08Frame010.frame,
    &gSpriteBank08Frame011.frame,
    &gSpriteBank08Frame012.frame,
    &gSpriteBank08Frame013.frame,
    &gSpriteBank08Frame014.frame,
    &gSpriteBank08Frame015.frame,
    &gSpriteBank08Frame016.frame,
    &gSpriteBank08Frame017.frame,
    &gSpriteBank08Frame018.frame,
    &gSpriteBank08Frame019.frame,
    &gSpriteBank08Frame020.frame,
    &gSpriteBank08Frame021.frame,
    &gSpriteBank08Frame022.frame,
    &gSpriteBank08Frame023.frame,
    &gSpriteBank08Frame024.frame,
    &gSpriteBank08Frame025.frame,
    &gSpriteBank08Frame026.frame,
    &gSpriteBank08Frame027.frame,
    &gSpriteBank08Frame028.frame,
    &gSpriteBank08Frame029.frame,
    &gSpriteBank08Frame030.frame,
    &gSpriteBank08Frame031.frame,
    &gSpriteBank08Frame032.frame,
    &gSpriteBank08Frame033.frame,
    &gSpriteBank08Frame034.frame,
    &gSpriteBank08Frame035.frame,
};

const struct sprite_frame_1box gSpriteBank08Frame000 = {
    SPRITE_FRAME(gSpriteBank08Frame000, SPRITE_TILES_BANK08 + 0x00000),
    { { -13, -1, 27, 8 } },
};
const struct sprite_frame_1box gSpriteBank08Frame001 = {
    SPRITE_FRAME(gSpriteBank08Frame001, SPRITE_TILES_BANK08 + 0x00160),
    { { -14, -2, 28, 8 } },
};
const struct sprite_frame_1box gSpriteBank08Frame002 = {
    SPRITE_FRAME(gSpriteBank08Frame002, SPRITE_TILES_BANK08 + 0x002e0),
    { { -20, -4, 36, 7 } },
};
const struct sprite_frame_1box gSpriteBank08Frame003 = {
    SPRITE_FRAME(gSpriteBank08Frame003, SPRITE_TILES_BANK08 + 0x00460),
    { { -19, -4, 33, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame004 = {
    SPRITE_FRAME(gSpriteBank08Frame004, SPRITE_TILES_BANK08 + 0x005c0),
    { { -10, -8, 19, 9 } },
};
const struct sprite_frame_1box gSpriteBank08Frame005 = {
    SPRITE_FRAME(gSpriteBank08Frame005, SPRITE_TILES_BANK08 + 0x00700),
    { { -6, -8, 13, 12 } },
};
const struct sprite_frame_1box gSpriteBank08Frame006 = {
    SPRITE_FRAME(gSpriteBank08Frame006, SPRITE_TILES_BANK08 + 0x007e0),
    { { -2, -11, 5, 16 } },
};
const struct sprite_frame_1box gSpriteBank08Frame007 = {
    SPRITE_FRAME(gSpriteBank08Frame007, SPRITE_TILES_BANK08 + 0x008a0),
    { { -7, -7, 12, 11 } },
};
const struct sprite_frame_1box gSpriteBank08Frame008 = {
    SPRITE_FRAME(gSpriteBank08Frame008, SPRITE_TILES_BANK08 + 0x009a0),
    { { -13, -4, 25, 5 } },
};
const struct sprite_frame_1box gSpriteBank08Frame009 = {
    SPRITE_FRAME(gSpriteBank08Frame009, SPRITE_TILES_BANK08 + 0x00ae0),
    { { -18, -5, 33, 6 } },
};
const struct sprite_frame_1box gSpriteBank08Frame010 = {
    SPRITE_FRAME(gSpriteBank08Frame010, SPRITE_TILES_BANK08 + 0x00c20),
    { { -19, -5, 32, 6 } },
};
const struct sprite_frame_1box gSpriteBank08Frame011 = {
    SPRITE_FRAME(gSpriteBank08Frame011, SPRITE_TILES_BANK08 + 0x00d60),
    { { -22, -4, 32, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame012 = {
    SPRITE_FRAME(gSpriteBank08Frame012, SPRITE_TILES_BANK08 + 0x00ea0),
    { { -15, -3, 30, 4 } },
};
const struct sprite_frame_1box gSpriteBank08Frame013 = {
    SPRITE_FRAME(gSpriteBank08Frame013, SPRITE_TILES_BANK08 + 0x01100),
    { { -15, -3, 30, 4 } },
};
const struct sprite_frame_1box gSpriteBank08Frame014 = {
    SPRITE_FRAME(gSpriteBank08Frame014, SPRITE_TILES_BANK08 + 0x01320),
    { { -15, -3, 30, 4 } },
};
const struct sprite_frame_1box gSpriteBank08Frame015 = {
    SPRITE_FRAME(gSpriteBank08Frame015, SPRITE_TILES_BANK08 + 0x014c0),
    { { -15, -3, 30, 4 } },
};
const struct sprite_frame_1box gSpriteBank08Frame016 = {
    SPRITE_FRAME(gSpriteBank08Frame016, SPRITE_TILES_BANK08 + 0x01720),
    { { -15, -3, 30, 4 } },
};
const struct sprite_frame_1box gSpriteBank08Frame017 = {
    SPRITE_FRAME(gSpriteBank08Frame017, SPRITE_TILES_BANK08 + 0x01960),
    { { -15, -3, 30, 4 } },
};
const struct sprite_frame_1box gSpriteBank08Frame018 = {
    SPRITE_FRAME(gSpriteBank08Frame018, SPRITE_TILES_BANK08 + 0x01ae0),
    { { -15, -3, 30, 4 } },
};
const struct sprite_frame_1box gSpriteBank08Frame019 = {
    SPRITE_FRAME(gSpriteBank08Frame019, SPRITE_TILES_BANK08 + 0x01d20),
    { { -15, -3, 30, 4 } },
};
const struct sprite_frame_1box gSpriteBank08Frame020 = {
    SPRITE_FRAME(gSpriteBank08Frame020, SPRITE_TILES_BANK08 + 0x01ec0),
    { { -15, -3, 30, 4 } },
};
const struct sprite_frame_1box gSpriteBank08Frame021 = {
    SPRITE_FRAME(gSpriteBank08Frame021, SPRITE_TILES_BANK08 + 0x02020),
    { { -15, -3, 30, 4 } },
};
const struct sprite_frame_1box gSpriteBank08Frame022 = {
    SPRITE_FRAME(gSpriteBank08Frame022, SPRITE_TILES_BANK08 + 0x02220),
    { { -15, -3, 30, 4 } },
};
const struct sprite_frame_1box gSpriteBank08Frame023 = {
    SPRITE_FRAME(gSpriteBank08Frame023, SPRITE_TILES_BANK08 + 0x02440),
    { { -15, -3, 30, 4 } },
};
const struct sprite_frame_1box gSpriteBank08Frame024 = {
    SPRITE_FRAME(gSpriteBank08Frame024, SPRITE_TILES_BANK08 + 0x025a0),
    { { -13, -5, 28, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame025 = {
    SPRITE_FRAME(gSpriteBank08Frame025, SPRITE_TILES_BANK08 + 0x02700),
    { { -13, -5, 28, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame026 = {
    SPRITE_FRAME(gSpriteBank08Frame026, SPRITE_TILES_BANK08 + 0x02880),
    { { -13, -5, 28, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame027 = {
    SPRITE_FRAME(gSpriteBank08Frame027, SPRITE_TILES_BANK08 + 0x02a20),
    { { -13, -5, 28, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame028 = {
    SPRITE_FRAME(gSpriteBank08Frame028, SPRITE_TILES_BANK08 + 0x02bc0),
    { { -13, -5, 28, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame029 = {
    SPRITE_FRAME(gSpriteBank08Frame029, SPRITE_TILES_BANK08 + 0x02d40),
    { { -13, -5, 28, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame030 = {
    SPRITE_FRAME(gSpriteBank08Frame030, SPRITE_TILES_BANK08 + 0x02ec0),
    { { -13, -5, 28, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame031 = {
    SPRITE_FRAME(gSpriteBank08Frame031, SPRITE_TILES_BANK08 + 0x03000),
    { { -13, -5, 28, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame032 = {
    SPRITE_FRAME(gSpriteBank08Frame032, SPRITE_TILES_BANK08 + 0x03160),
    { { -13, -5, 28, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame033 = {
    SPRITE_FRAME(gSpriteBank08Frame033, SPRITE_TILES_BANK08 + 0x032c0),
    { { -13, -5, 28, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame034 = {
    SPRITE_FRAME(gSpriteBank08Frame034, SPRITE_TILES_BANK08 + 0x03400),
    { { -13, -5, 28, 3 } },
};
const struct sprite_frame_1box gSpriteBank08Frame035 = {
    SPRITE_FRAME(gSpriteBank08Frame035, SPRITE_TILES_BANK08 + 0x03560),
    { { -13, -5, 28, 3 } },
};

const struct sprite_piece_pos gSpriteBank08Frame000Pos[3] = { { -20, -7 }, { 12, -1 }, { 20, 4 } };
const struct sprite_piece_pos gSpriteBank08Frame001Pos[2] = { { -24, -6 }, { 8, -2 } };
const struct sprite_piece_pos gSpriteBank08Frame002Pos[2] = { { -25, -6 }, { 7, -3 } };
const struct sprite_piece_pos gSpriteBank08Frame003Pos[3] = { { -24, -8 }, { 8, -4 }, { 16, 2 } };
const struct sprite_piece_pos gSpriteBank08Frame004Pos[3] = { { -19, -10 }, { 13, 0 }, { 10, 6 } };
const struct sprite_piece_pos gSpriteBank08Frame005Pos[3] = { { -12, -11 }, { 4, -7 }, { 2, 5 } };
const struct sprite_piece_pos gSpriteBank08Frame006Pos[4] = { { -4, -12 }, { 3, -11 }, { -5, 4 }, { 3, 4 } };
const struct sprite_piece_pos gSpriteBank08Frame007Pos[4] = { { -11, -6 }, { 4, -11 }, { -12, 5 }, { -4, 5 } };
const struct sprite_piece_pos gSpriteBank08Frame008Pos[3] = { { -18, -9 }, { 14, -10 }, { -17, 6 } };
const struct sprite_piece_pos gSpriteBank08Frame009Pos[3] = { { -22, -7 }, { 10, -8 }, { 18, -7 } };
const struct sprite_piece_pos gSpriteBank08Frame010Pos[2] = { { -24, -7 }, { 8, -7 } };
const struct sprite_piece_pos gSpriteBank08Frame011Pos[2] = { { -26, -8 }, { 6, -8 } };
const struct sprite_piece_pos gSpriteBank08Frame012Pos[3] = { { -20, -11 }, { 12, -1 }, { 20, 4 } };
const struct sprite_piece_pos gSpriteBank08Frame013Pos[4] = { { -24, -9 }, { 8, -7 }, { -22, 7 }, { 11, 7 } };
const struct sprite_piece_pos gSpriteBank08Frame014Pos[3] = { { -27, -4 }, { 5, -3 }, { 21, 2 } };
const struct sprite_piece_pos gSpriteBank08Frame015Pos[6] = { { -28, -11 }, { 4, -10 }, { 20, 1 }, { -24, 5 }, { 8, 5 }, { 16, 5 } };
const struct sprite_piece_pos gSpriteBank08Frame016Pos[5] = { { -26, -14 }, { 6, -10 }, { -21, 2 }, { 11, 2 }, { 19, 2 } };
const struct sprite_piece_pos gSpriteBank08Frame017Pos[2] = { { -22, -8 }, { 10, -5 } };
const struct sprite_piece_pos gSpriteBank08Frame018Pos[2] = { { -19, -17 }, { 13, -9 } };
const struct sprite_piece_pos gSpriteBank08Frame019Pos[3] = { { -16, -15 }, { 16, -1 }, { -6, 1 } };
const struct sprite_piece_pos gSpriteBank08Frame020Pos[4] = { { -13, -10 }, { 19, 4 }, { 10, 6 }, { 18, 6 } };
const struct sprite_piece_pos gSpriteBank08Frame021Pos[1] = { { -11, -14 } };
const struct sprite_piece_pos gSpriteBank08Frame022Pos[2] = { { -14, -12 }, { 18, 4 } };
const struct sprite_piece_pos gSpriteBank08Frame023Pos[4] = { { -17, -8 }, { 15, 1 }, { 9, 8 }, { 15, 8 } };
const struct sprite_piece_pos gSpriteBank08Frame024Pos[3] = { { -20, -7 }, { 12, -1 }, { 20, 4 } };
const struct sprite_piece_pos gSpriteBank08Frame025Pos[2] = { { -24, -5 }, { 8, -2 } };
const struct sprite_piece_pos gSpriteBank08Frame026Pos[3] = { { -27, -4 }, { 5, -3 }, { 21, 2 } };
const struct sprite_piece_pos gSpriteBank08Frame027Pos[3] = { { -28, -5 }, { 4, -5 }, { 20, 1 } };
const struct sprite_piece_pos gSpriteBank08Frame028Pos[2] = { { -26, -7 }, { 6, -6 } };
const struct sprite_piece_pos gSpriteBank08Frame029Pos[2] = { { -22, -8 }, { 10, -5 } };
const struct sprite_piece_pos gSpriteBank08Frame030Pos[2] = { { -19, -9 }, { 13, -3 } };
const struct sprite_piece_pos gSpriteBank08Frame031Pos[4] = { { -16, -10 }, { 16, -1 }, { 12, 6 }, { 16, 6 } };
const struct sprite_piece_pos gSpriteBank08Frame032Pos[4] = { { -13, -10 }, { 19, 4 }, { 10, 6 }, { 18, 6 } };
const struct sprite_piece_pos gSpriteBank08Frame033Pos[2] = { { -11, -10 }, { 6, 6 } };
const struct sprite_piece_pos gSpriteBank08Frame034Pos[3] = { { -13, -9 }, { 19, 5 }, { 5, 7 } };
const struct sprite_piece_pos gSpriteBank08Frame035Pos[4] = { { -17, -8 }, { 15, 1 }, { 9, 8 }, { 15, 8 } };

const u8 gSpriteBank08Frame000Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank08Frame001Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank08Frame002Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank08Frame003Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank08Frame004Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank08Frame005Pieces[3] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank08Frame006Pieces[4] = { SPRITE_PIECE(2, 8), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank08Frame007Pieces[4] = { SPRITE_PIECE(2, 1), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank08Frame008Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank08Frame009Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank08Frame010Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank08Frame011Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank08Frame012Pieces[3] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank08Frame013Pieces[4] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank08Frame014Pieces[3] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank08Frame015Pieces[6] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank08Frame016Pieces[5] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 5), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank08Frame017Pieces[2] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank08Frame018Pieces[2] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank08Frame019Pieces[3] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 5) };
const u8 gSpriteBank08Frame020Pieces[4] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank08Frame021Pieces[1] = { SPRITE_PIECE(5, 2) };
const u8 gSpriteBank08Frame022Pieces[2] = { SPRITE_PIECE(5, 2), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank08Frame023Pieces[4] = { SPRITE_PIECE(5, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank08Frame024Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank08Frame025Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank08Frame026Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank08Frame027Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank08Frame028Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank08Frame029Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 1) };
const u8 gSpriteBank08Frame030Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank08Frame031Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank08Frame032Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank08Frame033Pieces[2] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank08Frame034Pieces[3] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 4) };
const u8 gSpriteBank08Frame035Pieces[4] = { SPRITE_PIECE(2, 6), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };

/* ---------------------------------------------------------------------- */
/* Bank 9: 1 animation, 14 frames, tiles in gSpriteBank09Tiles (SPRITE_TILES_BANK09). */

extern const u16 gSpriteBank09Anim00Seq[14];
extern const struct sprite_frame_2box gSpriteBank09Frame000;
extern const struct sprite_frame_2box gSpriteBank09Frame001;
extern const struct sprite_frame_2box gSpriteBank09Frame002;
extern const struct sprite_frame_2box gSpriteBank09Frame003;
extern const struct sprite_frame_2box gSpriteBank09Frame004;
extern const struct sprite_frame_2box gSpriteBank09Frame005;
extern const struct sprite_frame_2box gSpriteBank09Frame006;
extern const struct sprite_frame_2box gSpriteBank09Frame007;
extern const struct sprite_frame_2box gSpriteBank09Frame008;
extern const struct sprite_frame_2box gSpriteBank09Frame009;
extern const struct sprite_frame_2box gSpriteBank09Frame010;
extern const struct sprite_frame_2box gSpriteBank09Frame011;
extern const struct sprite_frame_2box gSpriteBank09Frame012;
extern const struct sprite_frame_2box gSpriteBank09Frame013;
extern const struct sprite_piece_pos gSpriteBank09Frame000Pos[4];
extern const struct sprite_piece_pos gSpriteBank09Frame001Pos[4];
extern const struct sprite_piece_pos gSpriteBank09Frame002Pos[3];
extern const struct sprite_piece_pos gSpriteBank09Frame003Pos[3];
extern const struct sprite_piece_pos gSpriteBank09Frame004Pos[3];
extern const struct sprite_piece_pos gSpriteBank09Frame005Pos[3];
extern const struct sprite_piece_pos gSpriteBank09Frame006Pos[3];
extern const struct sprite_piece_pos gSpriteBank09Frame007Pos[4];
extern const struct sprite_piece_pos gSpriteBank09Frame008Pos[4];
extern const struct sprite_piece_pos gSpriteBank09Frame009Pos[3];
extern const struct sprite_piece_pos gSpriteBank09Frame010Pos[3];
extern const struct sprite_piece_pos gSpriteBank09Frame011Pos[4];
extern const struct sprite_piece_pos gSpriteBank09Frame012Pos[4];
extern const struct sprite_piece_pos gSpriteBank09Frame013Pos[3];
extern const u8 gSpriteBank09Frame000Pieces[4];
extern const u8 gSpriteBank09Frame001Pieces[4];
extern const u8 gSpriteBank09Frame002Pieces[3];
extern const u8 gSpriteBank09Frame003Pieces[3];
extern const u8 gSpriteBank09Frame004Pieces[3];
extern const u8 gSpriteBank09Frame005Pieces[3];
extern const u8 gSpriteBank09Frame006Pieces[3];
extern const u8 gSpriteBank09Frame007Pieces[4];
extern const u8 gSpriteBank09Frame008Pieces[4];
extern const u8 gSpriteBank09Frame009Pieces[3];
extern const u8 gSpriteBank09Frame010Pieces[3];
extern const u8 gSpriteBank09Frame011Pieces[4];
extern const u8 gSpriteBank09Frame012Pieces[4];
extern const u8 gSpriteBank09Frame013Pieces[3];

const struct sprite_anim gSpriteBank09Anims[1] = {
    [0] = {
        .seq = gSpriteBank09Anim00Seq,
        .box = { { -11, -17, 22, 35 }, { -11, -23, 22, 44 } },
        .tileRecord = 16,
        .duration = 3,
        .frameCount = ARRAY_COUNT(gSpriteBank09Anim00Seq),
        .flags = SPRITE_ANIM_LOOP,
    },
};

const u16 gSpriteBank09Anim00Seq[14] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
};

const struct sprite_frame *const gSpriteBank09Frames[14] = {
    &gSpriteBank09Frame000.frame,
    &gSpriteBank09Frame001.frame,
    &gSpriteBank09Frame002.frame,
    &gSpriteBank09Frame003.frame,
    &gSpriteBank09Frame004.frame,
    &gSpriteBank09Frame005.frame,
    &gSpriteBank09Frame006.frame,
    &gSpriteBank09Frame007.frame,
    &gSpriteBank09Frame008.frame,
    &gSpriteBank09Frame009.frame,
    &gSpriteBank09Frame010.frame,
    &gSpriteBank09Frame011.frame,
    &gSpriteBank09Frame012.frame,
    &gSpriteBank09Frame013.frame,
};

const struct sprite_frame_2box gSpriteBank09Frame000 = {
    SPRITE_FRAME(gSpriteBank09Frame000, SPRITE_TILES_BANK09 + 0x00000),
    { { -8, -15, 16, 17 }, { -7, -16, 15, 15 } },
};
const struct sprite_frame_2box gSpriteBank09Frame001 = {
    SPRITE_FRAME(gSpriteBank09Frame001, SPRITE_TILES_BANK09 + 0x00180),
    { { -8, -15, 16, 17 }, { -8, -15, 16, 11 } },
};
const struct sprite_frame_2box gSpriteBank09Frame002 = {
    SPRITE_FRAME(gSpriteBank09Frame002, SPRITE_TILES_BANK09 + 0x00300),
    { { -8, -15, 16, 17 }, { -7, -17, 14, 12 } },
};
const struct sprite_frame_2box gSpriteBank09Frame003 = {
    SPRITE_FRAME(gSpriteBank09Frame003, SPRITE_TILES_BANK09 + 0x00460),
    { { -8, -15, 16, 17 }, { -7, -19, 13, 16 } },
};
const struct sprite_frame_2box gSpriteBank09Frame004 = {
    SPRITE_FRAME(gSpriteBank09Frame004, SPRITE_TILES_BANK09 + 0x005e0),
    { { -8, -15, 16, 17 }, { -7, -18, 15, 13 } },
};
const struct sprite_frame_2box gSpriteBank09Frame005 = {
    SPRITE_FRAME(gSpriteBank09Frame005, SPRITE_TILES_BANK09 + 0x00760),
    { { -8, -15, 16, 17 }, { -7, -20, 14, 15 } },
};
const struct sprite_frame_2box gSpriteBank09Frame006 = {
    SPRITE_FRAME(gSpriteBank09Frame006, SPRITE_TILES_BANK09 + 0x008c0),
    { { -8, -15, 16, 17 }, { -6, -20, 13, 13 } },
};
const struct sprite_frame_2box gSpriteBank09Frame007 = {
    SPRITE_FRAME(gSpriteBank09Frame007, SPRITE_TILES_BANK09 + 0x00a40),
    { { -8, -15, 16, 17 }, { -7, -20, 15, 16 } },
};
const struct sprite_frame_2box gSpriteBank09Frame008 = {
    SPRITE_FRAME(gSpriteBank09Frame008, SPRITE_TILES_BANK09 + 0x00be0),
    { { -8, -15, 16, 17 }, { -7, -19, 15, 15 } },
};
const struct sprite_frame_2box gSpriteBank09Frame009 = {
    SPRITE_FRAME(gSpriteBank09Frame009, SPRITE_TILES_BANK09 + 0x00d80),
    { { -8, -15, 16, 17 }, { -6, -20, 12, 17 } },
};
const struct sprite_frame_2box gSpriteBank09Frame010 = {
    SPRITE_FRAME(gSpriteBank09Frame010, SPRITE_TILES_BANK09 + 0x00ee0),
    { { -8, -15, 16, 17 }, { -7, -19, 12, 19 } },
};
const struct sprite_frame_2box gSpriteBank09Frame011 = {
    SPRITE_FRAME(gSpriteBank09Frame011, SPRITE_TILES_BANK09 + 0x01040),
    { { -8, -15, 16, 17 }, { -7, -16, 14, 15 } },
};
const struct sprite_frame_2box gSpriteBank09Frame012 = {
    SPRITE_FRAME(gSpriteBank09Frame012, SPRITE_TILES_BANK09 + 0x011c0),
    { { -8, -15, 16, 17 }, { -8, -15, 14, 15 } },
};
const struct sprite_frame_2box gSpriteBank09Frame013 = {
    SPRITE_FRAME(gSpriteBank09Frame013, SPRITE_TILES_BANK09 + 0x01340),
    { { -8, -15, 16, 17 }, { -6, -15, 12, 14 } },
};

const struct sprite_piece_pos gSpriteBank09Frame000Pos[4] = { { -11, -17 }, { 5, -15 }, { 5, 1 }, { -3, 15 } };
const struct sprite_piece_pos gSpriteBank09Frame001Pos[4] = { { -11, -17 }, { 5, -15 }, { 5, 1 }, { -4, 15 } };
const struct sprite_piece_pos gSpriteBank09Frame002Pos[3] = { { -10, -19 }, { 6, -14 }, { -4, 13 } };
const struct sprite_piece_pos gSpriteBank09Frame003Pos[3] = { { -10, -21 }, { 6, -15 }, { -3, 11 } };
const struct sprite_piece_pos gSpriteBank09Frame004Pos[3] = { { -9, -22 }, { 7, -15 }, { -3, 10 } };
const struct sprite_piece_pos gSpriteBank09Frame005Pos[3] = { { -9, -23 }, { 7, -15 }, { -2, 9 } };
const struct sprite_piece_pos gSpriteBank09Frame006Pos[3] = { { -9, -23 }, { 7, -16 }, { -1, 9 } };
const struct sprite_piece_pos gSpriteBank09Frame007Pos[4] = { { -10, -23 }, { 6, -17 }, { 6, -1 }, { -1, 9 } };
const struct sprite_piece_pos gSpriteBank09Frame008Pos[4] = { { -10, -22 }, { 6, -17 }, { 6, -1 }, { 0, 10 } };
const struct sprite_piece_pos gSpriteBank09Frame009Pos[3] = { { -10, -21 }, { 6, -16 }, { 1, 11 } };
const struct sprite_piece_pos gSpriteBank09Frame010Pos[3] = { { -10, -20 }, { 6, -15 }, { 1, 12 } };
const struct sprite_piece_pos gSpriteBank09Frame011Pos[4] = { { -11, -19 }, { 5, -17 }, { 5, -1 }, { 0, 13 } };
const struct sprite_piece_pos gSpriteBank09Frame012Pos[4] = { { -11, -18 }, { 5, -16 }, { 5, 0 }, { -1, 14 } };
const struct sprite_piece_pos gSpriteBank09Frame013Pos[3] = { { -11, -18 }, { 5, -15 }, { -2, 14 } };

const u8 gSpriteBank09Frame000Pieces[4] = { SPRITE_PIECE(3, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank09Frame001Pieces[4] = { SPRITE_PIECE(3, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank09Frame002Pieces[3] = { SPRITE_PIECE(3, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank09Frame003Pieces[3] = { SPRITE_PIECE(3, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank09Frame004Pieces[3] = { SPRITE_PIECE(3, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank09Frame005Pieces[3] = { SPRITE_PIECE(3, 10), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank09Frame006Pieces[3] = { SPRITE_PIECE(3, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank09Frame007Pieces[4] = { SPRITE_PIECE(3, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank09Frame008Pieces[4] = { SPRITE_PIECE(3, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 8) };
const u8 gSpriteBank09Frame009Pieces[3] = { SPRITE_PIECE(3, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank09Frame010Pieces[3] = { SPRITE_PIECE(3, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank09Frame011Pieces[4] = { SPRITE_PIECE(3, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank09Frame012Pieces[4] = { SPRITE_PIECE(3, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0), SPRITE_PIECE(0, 0) };
const u8 gSpriteBank09Frame013Pieces[3] = { SPRITE_PIECE(3, 10), SPRITE_PIECE(0, 8), SPRITE_PIECE(0, 0) };
