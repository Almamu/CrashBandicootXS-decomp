#ifndef GUARD_SYSTEM_H
#define GUARD_SYSTEM_H

/* The system subsystem (src/system/): boot, the main loop, VBlank wait
 * and frame limit, key polling, tagged-asset loading and the UI text
 * lookup. The interrupt table is in irq.h and the heap in memory.h; this
 * header includes both.
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers_plan.md, "Codegen exceptions"). */

#include "core.h"
#include "irq.h"
#include "memory.h"

/* src/iwram/iwram_data.cpp */
extern u32 gVBlankCounter;
extern u8 gFrameLimitEnabled;
/* The language picked at boot (0-5), set by MainLoop. */
extern s32 gLanguage;
/* One UI string table per language (src/data/ui_text_172cd4.c). */
extern const u8 *const *gUiTextTables[6];
/* The tables gUiTextTables points at (src/data/ui_text_172cd4.c). */
extern const u8 *const gUiTextEnglish[70];
extern const u8 *const gUiTextFrench[70];
extern const u8 *const gUiTextGerman[70];
extern const u8 *const gUiTextSpanish[70];
extern const u8 *const gUiTextItalian[70];
extern const u8 *const gUiTextDutch[70];

/* sym_iwram.txt: WaitForVBlank's frame limit (SetFrameLimit). */
extern u32 gFrameLimitTarget;
extern u32 gFrameLimitInterval;

/* Held d-pad bits (right/left/down/up as bits 3-0) -> direction 0-8
 * (src/data/boss_pictures_167ad4.c). */
extern const u8 gDpadDirectionTable[16];

/* src/system/asset.cpp */
extern void LoadTaggedAsset(const void *asset, void *dest);
extern void LoadBackgroundTileAndPalette(const void *asset);

/* src/system/bios_util.cpp */
extern s32 DivMod(s32 number, s32 denom, s32 *remainderOut);
extern void *MemCopy32(void *dst, const void *src, u32 byteCount);

/* src/system/input.cpp */
extern s32 WaitForKeyPress(s32 count, u8 checkButtons, s32 mask);

/* src/system/irq.cpp. UpdateKeys and GetDpadDirection take the input
 * object (every caller passes gInput) but read gKeys directly; its
 * constructor, ClearKeys, is KeyInput's (spawners.hpp). */
extern void WaitForVBlank(void);
extern void DisableFrameLimit(void);
extern void SetFrameLimit(u32 interval);
extern u8 GetDpadDirection(void *input);
extern s32 UpdateKeys(void *input);

/* src/system/main.cpp */
extern s32 AgbMain(void);

/* src/system/main_loop.cpp. GetUiText returns the string's address as an
 * s32 (its callers store it in s32 fields). */
extern s32 MainLoop(void);
extern s32 GetUiText(s32 index);

#endif // GUARD_SYSTEM_H
