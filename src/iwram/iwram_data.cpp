#include "level_state.hpp"

extern "C" {
#include "core.h"
#include "system.h"
#include "cutscene.h"
#include "hud.h"
#include "link.h"
#include "save.h"
#include "frontend.h"
#include "util.h"
#include "audio.h"
#include "menus.h"
#include "player.h"
#include "actor.h"
#include "bosses.h"
#include "vehicle.h"
#include "gfx.h"
#include "iwram.h"
#include "level.h"
#include "globals.h"
}

/*
 * IWRAM 0x030007CC-0x030009E8 (stored in ROM at 0x087E5DB0-0x087E5FCC):
 * the initialised IWRAM globals, the end of the IWRAM image crt0 copies
 * to 0x03000000 at boot. The image is IntrMain (asm/intr_main.s), the ARM
 * code (src/iwram/string_arm.cpp, src/iwram/sprite_arm.cpp) and this file's
 * .data, linked to run at 0x03000000 and stored after the vtables in ROM
 * (the `iwram` section in ldscript.txt, docs/decomp_dev.md). Everything
 * after 0x030009E8 is uninitialised (sym_iwram.txt).
 *
 * The globals are in address order, each defined with an initialiser
 * (zero ones too) so agbcp puts them all in .data, in this order. The
 * types are the plainest ones the users agree on; where users declare
 * different local structs, see the comment.
 */


/* src/system/memory.cpp's heaps, and their free space right after
 * mem_heap_init (checked by mem_heap_shutdown). */
struct mem_heap *mem_iwram_heap_pointer = 0;
struct mem_heap *mem_ewram_heap_pointer = 0;
s32 mem_initial_free_bytes = 0;

/* irq.cpp: the frame counter the VBlank handler increments, and the
 * frame-rate limiter's enable flag. */
u32 gVBlankCounter = 0;
u8 gFrameLimitEnabled = 0;
/* Set while the music player's per-frame update is installed
 * (audio.cpp). */
u8 gGaxIrqEnabled = 0;

/* Held keys and newly pressed keys (key_input.cpp's UpdateKeys). */
union key_state gKeys = { { 0, 0 } };

/* rand.cpp's seed. */
u32 gRandSeed = 1;

/* fade.cpp's brightness fade state. */
struct brightness_fade gBrightnessFade = { -1, -1, 0 };

s32 gBrightnessFadeStep = 0;
s32 gBrightnessFadeTimer = 0;
u32 gSfxVoiceToggle = 0;

/* Link cable (src/link/*.c, src/save/*.c): gLinkSession is the
 * session object the link IRQ handlers work on. gEepromNeedsInit is the
 * save code's (save_data.cpp): set until its first EEPROMConfigure. */
u8 gLinkSessionReset = 1;
LinkSession *gLinkSession = 0;
u8 gEepromNeedsInit = 1;
SaveMenu *gSaveMenu = 0;
/* The two link compatibility messages, stored after the CRC table
 * (src/data/link_crc_16af10.c): "crash 1 <-> crash 2", "crash 1 <-> crash 3". */
const u8 *gCrash2LinkTextPtr = (const u8 *)gCrash2LinkText;
const u8 *gCrash3LinkTextPtr = (const u8 *)gCrash3LinkText;

/* The Aku Aku mask's animation frame (player_event.c): re-rolled every
 * 8 frames while invincible (mask level 3), random-walked in 0-3 while
 * following the player (levels 1-2). */
s32 gAkuAkuInvincibleFrame = 0;
s32 gAkuAkuFollowFrame = 0;
LevelSelect *gLevelSelect = 0;
u8 gNewWorldOpened = 0;
LevelState *gLevelStateSingleton = 0;
u32 gRoomFrameCount = 0;
u8 gRoomExitRequested = 0;

/* The cutscene text of each language, indexed by gLanguage
 * (src/data/cutscenes_16d1c8.c, level_cutscene.cpp). */
const struct cutscene_page *const *gCutsceneTexts[6] = {
    gCutsceneTextEnglish, gCutsceneTextFrench,  gCutsceneTextGerman,
    gCutsceneTextSpanish, gCutsceneTextItalian, gCutsceneTextDutch,
};

LevelLayers *gLevelLayersSingleton = 0;

/* Per-language string tables (main_loop.cpp indexes them by
 * gLanguage), src/data/ui_text_172cd4.c. */
const u8 *const *gUiTextTables[6] = {
    gUiTextEnglish, gUiTextFrench, gUiTextGerman, gUiTextSpanish, gUiTextItalian, gUiTextDutch,
};

/* The language, 0-5 (English, French, German, Spanish, Italian, Dutch);
 * main_loop.cpp sets it at boot. */
s32 gLanguage = 3;
s32 gHudSlideOffset = 0;

/* Hooks into the ARM code (see sprite_arm.cpp). */
s32 (*gLookupSpriteFrameCacheFunc)(u8 *frame) = LookupSpriteFrameCache;
void (*gUnpackRleSpriteFrameFunc)(u16 *dst, struct rle_frame *frame) = UnpackRleSpriteFrame;
s32 gActorCheckpoint = 0;
void (*gDrawMirroredTilemapFunc)(u8 *pal, s32 lowBlock, s32 w, s32 h) = DrawMirroredTilemap;
void (*gHeapSortActorsByKeyFunc)(s32 n, ActorSelf **list) = HeapSortActorsByKey;
ActorSelf *gActorList = 0;
s32 gCollectedSpawnCount = 0;
/* The polar penguin's Z speed toward a path point, indexed by the
 * point's spawn kind minus 0x20 (GetActorSpawnKindIndex; AimPolarPenguin). */
s32 gPolarPenguinSpeeds[3] = { 0x40, 0x62, 0x95 };
void (*gUnpackNibbleTilesFunc)(u16 *src, s32 lowBlock) = UnpackNibbleTiles;
/* The jetpack plane's hop speed toward a path point, indexed by the
 * point's spawn kind minus 0x20 (GetActorSpawnKindIndex; AimJetpackPlane). */
s32 gJetpackPlaneHopSpeeds[6] = { 0x1555, 0x1155, 0xD55, 0x955, 0x555, 0x155 };

/* Palette RAM addresses (hovercraft.cpp, hovercraft_parts.cpp). */
u16 *gFlashBgPalette = (u16 *)(PLTT + 0x20);
u16 *gFlashObjPalette = (u16 *)(PLTT + 0x340);

/* title_screen_init.cpp: the four OBJ sprite packages of
 * src/data/level_gfx_17cff4.c. */
const void *gTitleObjPackages[4] = {
    &gTitleCrashObj,
    &gTitleArrow2Obj,
    &gTitleArrow1Obj,
    &gTitleBandicootObj,
};

LanguageSelect *gLanguageSelect = 0;

/* GAX2's fatal-error screen font (gax_fatal_error.c), Huffman-compressed
 * for the BIOS HuffUnComp: 8-bit symbols, 0x4A0 bytes (37 4bpp tiles)
 * once unpacked. */
u32 gGaxHaltFont[70] = {
    0x0004A028, 0x80008011, 0x01804010, 0x42804011, 0x804080FF, 0x00000002, 0x00000000, 0x00000000,
    0xC0000000, 0x0000F600, 0x34D0D835, 0x0D66C01B, 0x0CE9262A, 0x5E48CE04, 0x30421086, 0x7033A422,
    0xE7216601, 0x9D2119C4, 0x919C072E, 0x7E9D660E, 0x382CC421, 0x67124670, 0x2E721674, 0x923382CC,
    0x10E438E3, 0x80CE9246, 0x74923381, 0x9D248CC1, 0x0E600CE9, 0x2492CC49, 0x2059D249, 0x6749259C,
    0x0CE92108, 0x48CE0B3A, 0x4924924B, 0x382CC421, 0x67421660, 0x2CC42167, 0x42100674, 0x90AC4923,
    0x30124925, 0x98924901, 0x9C842108, 0x67010842, 0x49233824, 0x9D45CA27, 0x48108421, 0x08598092,
    0xF8A92492, 0x48125E54, 0x98924901, 0x9D249249, 0x23382CE9, 0x24B3A108, 0x033A4924, 0xA939902C,
    0xE924B3A7, 0x492033A4, 0x83389233, 0x82CC2108, 0x42101249, 0x249248CE, 0x09249249, 0x1F901249,
    0x52A54A8F, 0xC1248FC8, 0x7E924092, 0x491F9084, 0x059C721C, 0x842CE000,
};
