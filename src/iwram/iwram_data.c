#include "core.h"
#include "memory.h"
#include "cutscene.h"
#include "hud.h"
#include "link.h"

/*
 * IWRAM 0x030007CC-0x030009E8 (stored in ROM at 0x087E5DB0-0x087E5FCC):
 * the initialised IWRAM globals, the end of the IWRAM image crt0 copies
 * to 0x03000000 at boot. The image is IntrMain (asm/intr_main.s), the ARM
 * code (src/iwram/string_arm.c, src/iwram/sprite_arm.c) and this file's
 * .data, linked to run at 0x03000000 and stored after the vtables in ROM
 * (the `iwram` section in ldscript.txt, docs/decomp_dev.md). Everything
 * after 0x030009E8 is uninitialised (sym_iwram.txt).
 *
 * The globals are in address order, each defined with an initialiser
 * (zero ones too) so agbcc puts them all in .data, in this order. The
 * types are the plainest ones the users agree on; where users declare
 * different local structs, see the comment.
 */

extern const char gCrash2LinkText[];
extern const char gCrash3LinkText[];
extern const u8 *const gUiTextEnglish[70];
extern const u8 *const gUiTextFrench[70];
extern const u8 *const gUiTextGerman[70];
extern const u8 *const gUiTextSpanish[70];
extern const u8 *const gUiTextItalian[70];
extern const u8 *const gUiTextDutch[70];
extern const u8 gTitleBandicootObj[];
extern const u8 gTitleCrashObj[];
extern const u8 gTitleArrow1Obj[];
extern const u8 gTitleArrow2Obj[];

/* The ARM routines the Thumb code calls through the pointers below. */
extern s32 LookupSpriteFrameCache(u8 *frame);
extern void UnpackRleSpriteFrame(void *dst, u8 *frame);
extern void DrawMirroredTilemap(u8 *pal, s32 lowBlock, s32 w, s32 h);
extern void HeapSortActorsByKey(s32 n, void **list);
extern void UnpackNibbleTiles(void *src, s32 lowBlock);

/* src/system/memory.c's heaps, and their free space right after
 * mem_heap_init (checked by mem_heap_shutdown). */
struct mem_heap *mem_iwram_heap_pointer = NULL;
struct mem_heap *mem_ewram_heap_pointer = NULL;
int *mem_initial_free_bytes = NULL;

/* irq.c: the frame counter the VBlank handler increments, and the
 * frame-rate limiter's enable flag. */
u32 gVBlankCounter = 0;
u8 gFrameLimitEnabled = 0;
/* Set while the music player's per-frame update is installed
 * (audio.c). */
u8 gGaxIrqEnabled = 0;

/* Held keys and newly pressed keys (irq.c's UpdateKeys; the users
 * declare it as a pair of u16s or a struct of two). */
struct {
    u16 held;
    u16 pressed;
} gKeys = { 0, 0 };

/* rand.c's seed. */
u32 gRandSeed = 1;

/* fade.c's `struct unk_030007E8`: the brightness fade state. */
struct {
    s32 field_0;
    s32 field_4;
    u8 field_8;
} gBrightnessFade = { -1, -1, 0 };

s32 gBrightnessFadeStep = 0;
s32 gBrightnessFadeTimer = 0;
u32 gSfxVoiceToggle = 0;

/* Link cable (src/link/*.c, src/save/*.c): gLinkSession is the
 * session object the link IRQ handlers work on. gEepromNeedsInit is the
 * save code's (save_data.c): set until its first EEPROMConfigure. */
u8 gLinkSessionReset = 1;
void *gLinkSession = NULL;
u8 gEepromNeedsInit = 1;
void *gSaveMenu = NULL;
/* The two link compatibility messages, stored after the CRC table
 * (src/data/link_crc_16af10.c): "crash 1 <-> crash 2", "crash 1 <-> crash 3". */
const u8 *gCrash2LinkTextPtr = (const u8 *)gCrash2LinkText;
const u8 *gCrash3LinkTextPtr = (const u8 *)gCrash3LinkText;

/* The Aku Aku mask's animation frame (player_event.c): re-rolled every
 * 8 frames while invincible (mask level 3), random-walked in 0-3 while
 * following the player (levels 1-2). */
s32 gAkuAkuInvincibleFrame = 0;
s32 gAkuAkuFollowFrame = 0;
void *gLevelSelect = NULL; /* struct level_menu * */
u8 gNewWorldOpened = 0;
void *gLevelStateSingleton = NULL;
u32 gRoomFrameCount = 0;
u8 gRoomExitRequested = 0;

/* The cutscene text of each language, indexed by gLanguage
 * (src/data/cutscenes_16d1c8.c, level_cutscene.c). */
extern const struct cutscene_page *const gCutsceneTextEnglish[11];
extern const struct cutscene_page *const gCutsceneTextFrench[11];
extern const struct cutscene_page *const gCutsceneTextGerman[11];
extern const struct cutscene_page *const gCutsceneTextSpanish[11];
extern const struct cutscene_page *const gCutsceneTextItalian[11];
extern const struct cutscene_page *const gCutsceneTextDutch[11];
const struct cutscene_page *const *gCutsceneTexts[6] = {
    gCutsceneTextEnglish,
    gCutsceneTextFrench,
    gCutsceneTextGerman,
    gCutsceneTextSpanish,
    gCutsceneTextItalian,
    gCutsceneTextDutch,
};

void *gLevelLayersSingleton = NULL; /* struct level_layers * */

/* Per-language string tables (main_loop.c indexes them by
 * gLanguage), src/data/ui_text_172cd4.c. */
const u8 *const *gUiTextTables[6] = {
    gUiTextEnglish,
    gUiTextFrench,
    gUiTextGerman,
    gUiTextSpanish,
    gUiTextItalian,
    gUiTextDutch,
};

/* The language, 0-5 (English, French, German, Spanish, Italian, Dutch);
 * main_loop.c sets it at boot. */
s32 gLanguage = 3;
s32 gHudSlideOffset = 0;

/* Hooks into the ARM code (see sprite_arm.c). */
s32 (*gLookupSpriteFrameCacheFunc)(u8 *frame) = LookupSpriteFrameCache;
void (*gUnpackRleSpriteFrameFunc)(void *dst, u8 *frame) = UnpackRleSpriteFrame;
s32 gActorCheckpoint = 0;
void (*gDrawMirroredTilemapFunc)(u8 *pal, s32 lowBlock, s32 w, s32 h) = DrawMirroredTilemap;
void (*gHeapSortActorsByKeyFunc)(s32 n, void **list) = HeapSortActorsByKey;
void *gActorList = NULL;
s32 gCollectedSpawnCount = 0;
/* Speeds, indexed by sub_802A570 (polar_objects.c). */
s32 gUnknown_0300088C[3] = { 0x40, 0x62, 0x95 };
void (*gUnpackNibbleTilesFunc)(void *src, s32 lowBlock) = UnpackNibbleTiles;
/* Speeds, indexed by sub_802A570 (jetpack_plane.c). */
s32 gUnknown_0300089C[6] = { 0x1555, 0x1155, 0xD55, 0x955, 0x555, 0x155 };

/* Palette RAM addresses (hovercraft.c, hovercraft_parts.c). */
void *gFlashBgPalette = (void *)(PLTT + 0x20);
void *gFlashObjPalette = (void *)(PLTT + 0x340);

/* title_screen_init.c: the four OBJ sprite packages of
 * src/data/level_gfx_17cff4.c. */
const void *gTitleObjPackages[4] = {
    gTitleCrashObj,
    gTitleArrow2Obj,
    gTitleArrow1Obj,
    gTitleBandicootObj,
};

void *gLanguageSelect = NULL; /* struct language_select * */

/* GAX2's fatal-error screen font (gax_fatal_error.c), Huffman-compressed
 * for the BIOS HuffUnComp: 8-bit symbols, 0x4A0 bytes (37 4bpp tiles)
 * once unpacked. */
u32 gGaxHaltFont[70] = {
    0x0004A028, 0x80008011, 0x01804010, 0x42804011, 0x804080FF, 0x00000002,
    0x00000000, 0x00000000, 0xC0000000, 0x0000F600, 0x34D0D835, 0x0D66C01B,
    0x0CE9262A, 0x5E48CE04, 0x30421086, 0x7033A422, 0xE7216601, 0x9D2119C4,
    0x919C072E, 0x7E9D660E, 0x382CC421, 0x67124670, 0x2E721674, 0x923382CC,
    0x10E438E3, 0x80CE9246, 0x74923381, 0x9D248CC1, 0x0E600CE9, 0x2492CC49,
    0x2059D249, 0x6749259C, 0x0CE92108, 0x48CE0B3A, 0x4924924B, 0x382CC421,
    0x67421660, 0x2CC42167, 0x42100674, 0x90AC4923, 0x30124925, 0x98924901,
    0x9C842108, 0x67010842, 0x49233824, 0x9D45CA27, 0x48108421, 0x08598092,
    0xF8A92492, 0x48125E54, 0x98924901, 0x9D249249, 0x23382CE9, 0x24B3A108,
    0x033A4924, 0xA939902C, 0xE924B3A7, 0x492033A4, 0x83389233, 0x82CC2108,
    0x42101249, 0x249248CE, 0x09249249, 0x1F901249, 0x52A54A8F, 0xC1248FC8,
    0x7E924092, 0x491F9084, 0x059C721C, 0x842CE000,
};
