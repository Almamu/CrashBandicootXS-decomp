#ifndef GUARD_SYSTEM_H
#define GUARD_SYSTEM_H

/* The system subsystem (src/engine/sys/): boot, the main loop, VBlank wait
 * and frame limit, key polling, tagged-asset loading and the UI text
 * lookup. The interrupt table is in irq.h and the heap in memory.h; this
 * header includes both.
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers.md, "Codegen exceptions"). */

#include "core.h"
#include "engine/sys/irq.h"
#include "engine/sys/memory.h"
#include "constants/levels.h"

/* src/engine/sys/iwram_data.cpp */
extern u32 gVBlankCounter;
/* The language picked at boot (0-5), set by MainLoop. */
extern s32 gLanguage;
/* One UI string table per language (src/data/ui_text_172cd4.cpp). */
extern const u8 *const *gUiTextTables[6];
/* The tables gUiTextTables points at (src/data/ui_text_172cd4.cpp). */
extern const u8 *const gUiTextEnglish[UI_TEXT_COUNT];
extern const u8 *const gUiTextFrench[UI_TEXT_COUNT];
extern const u8 *const gUiTextGerman[UI_TEXT_COUNT];
extern const u8 *const gUiTextSpanish[UI_TEXT_COUNT];
extern const u8 *const gUiTextItalian[UI_TEXT_COUNT];
extern const u8 *const gUiTextDutch[UI_TEXT_COUNT];

/* Held d-pad bits (right/left/down/up as bits 3-0) -> direction 0-8
 * (src/data/boss_pictures_167ad4.cpp). */
extern const u8 gDpadDirectionTable[16];

/* src/engine/gfx/asset.cpp */
/* A tagged asset's type: bits 4-7 of its header word, the GBA BIOS's
 * compression type (#843, docs/flags/asset.md). */
#define ASSET_TYPE_RAW 0     // uncompressed: LoadTaggedAsset DMAs it
#define ASSET_TYPE_LZ77 1    // LZ77UnCompVram
#define ASSET_TYPE_HUFFMAN 2 // the BIOS's Huffman; LoadTaggedAsset skips it
#define ASSET_TYPE_RL 3      // run-length: RLUnCompVram
extern void LoadTaggedAsset(const void *asset, void *dest);
extern void LoadBackgroundTileAndPalette(const void *asset);

/* src/engine/sys/bios_util.cpp */
extern s32 DivMod(s32 number, s32 denom, s32 *remainderOut);
extern void *MemCopy32(void *dst, const void *src, u32 byteCount);

/* src/engine/sys/input.cpp */
extern s32 WaitForKeyPress(s32 count, u8 checkButtons, s32 mask);

/* src/engine/sys/key_input.cpp: KeyInput's constructor (ClearKeys),
 * UpdateKeys and GetDpadDirection are its methods (key_input.hpp). */

/* src/engine/sys/main.cpp */
extern s32 AgbMain(void);

/* src/system/main_loop.cpp. GetUiText returns UI string `index` in the
 * current language (docs/casts/text.md). */
extern s32 MainLoop(void);
extern const u8 *GetUiText(s32 index);

#endif // GUARD_SYSTEM_H
