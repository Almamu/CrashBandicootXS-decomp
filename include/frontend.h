#ifndef GUARD_FRONTEND_H
#define GUARD_FRONTEND_H

/* The front end (src/frontend/): the language select at boot, the
 * company logos, the title screen, the credits and the starfield the
 * language select and the credits draw behind their text.
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers_plan.md).
 *
 * Not here, by ownership (docs/headers_plan.md, "Who owns a symbol"):
 * the continue prompt functions at the start of credits.cpp
 * (DrawContinuePrompt..RunContinuePrompt) are in menus.h, with the rest
 * of the continue prompt. LoadTaggedAssetBuffered
 * (language_select.cpp, CompanyLogos::LoadAssetBuffered) is in system.h,
 * with LoadTaggedAsset. */

#include "core.h"
#include "actor_self.h"
#include "graphics_package.h"
#include "vtable.h"

/* One step of a logo piece's motion (the gTitleLogoPieceMotionN and
 * gVvLogoPieceMotionNN tables). When the piece's hold count runs out it
 * loads the next record: a new hold count (0 ends the sequence), three
 * Q16.16 positions, two Q24.8 velocities and the five per-frame deltas
 * added to them while the hold lasts. */
struct delta_record {
    s16 hold;   /* 0x00 - countdown reload value */
    u16 dPosA;  /* 0x02 - Q16.16 position (<<16) */
    u16 dPosB;  /* 0x04 - Q16.16 position (<<16) */
    u16 dPosC;  /* 0x06 - Q16.16 position (<<16) */
    s16 dVelA;  /* 0x08 - Q24.8 velocity (<<8) */
    s16 dVelB;  /* 0x0a - Q24.8 velocity (<<8) */
    s32 deltaA; /* 0x0c - raw delta for dPosA's live field */
    s32 deltaB; /* 0x10 - raw delta for dPosB's live field */
    s32 deltaC; /* 0x14 - raw delta for dPosC's live field */
    s32 deltaD; /* 0x18 - raw delta for dVelA's live field */
    s32 deltaE; /* 0x1c - raw delta for dVelB's live field */
};

/* One entry of the {motion, initial hold} seed tables
 * (gTitleLogoPieceSeeds/gVvLogoPieceSeeds) the logo pieces start from. */
struct slot_seed {
    const struct delta_record *record;
    s32 hold;
};

/* The language select, between OpenLanguageSelect and
 * CloseLanguageSelect (src/iwram/iwram_data.cpp): a LanguageSelect
 * (frontend.hpp). No C file uses it. */
#ifdef __cplusplus
extern class LanguageSelect *gLanguageSelect;
#endif

/* The six language names (src/data/digit_glyphs_17e714.c) and the four
 * palettes InitLanguageSelectGraphics copies (palettes_17e72c.c,
 * level_tilesets_17e78c.c). */
extern const u8 *const gLanguageNames[6];
extern const u16 gLanguageSelectPalette0[16];
extern const u16 gLanguageSelectPalette1[16];
extern const u16 gLanguageSelectPalette2[16];
extern const u16 gLanguageSelectPalette3[16];

/* The company logo actor (InitLogoActor): its animation record
 * (src/data/level_gfx_17cff4.c, an anim_table_record) and the two VRAM
 * tile blocks DrawLogoActor
 * double-buffers its frames in (IWRAM, sym_iwram.txt). */
extern const struct anim_table_record gLogoActorAnim; /* actor_anim.h */
extern void *gLogoActorTiles[2];
extern s32 gLogoActorTileBuffer;  /* the block on screen, 0 or 1 */
extern void *gLogoActorLastFrame; /* the frame last unpacked */

/* The animation family RunCompanyLogos loads the logo actor's palette
 * from, and gLogoActorAnim its frames (src/data/anim_family_178f80.c). */
extern const u16 gPolarCategoryPalette[0x200];

/* The logo pieces' seed tables (src/data/popup_glyphs_17cf40.c,
 * slot_seeds_17d6c0.c), NULL-terminated. */
extern const struct slot_seed gTitleLogoPieceSeeds[10];
extern const struct slot_seed gVvLogoPieceSeeds[21];

/* The {x, y} offsets of the eight OAM pieces DrawTitleLogoPieces draws
 * around each of its two slots (src/data/level_gfx_17cff4.c). */
extern const s32 gTitleArrowPieceOffsets[8][2];

/* The title screen's graphics (src/data/level_gfx_17cff4.c): the menu
 * palettes InitTitleScreen DMAs to OBJ palettes 13-15, the BG2 package
 * LoadTitleScreenBg loads and the four OBJ packages LoadTitleScreenObjTiles
 * loads (gTitleObjPackages, src/iwram/iwram_data.cpp). */
extern const u16 gTitleMenuPalette[16];
extern const u16 gTitleMenuSelectedPalette[16];
extern const u16 gTitleMenuBlinkPalette[16];
extern const struct bg_package gTitleScreenBg;
extern const void *gTitleObjPackages[4];
/* The four packages gTitleObjPackages points at (src/data/level_gfx_17cff4.c). */
extern const struct bg_package gTitleCrashObj;
extern const struct bg_package gTitleArrow2Obj;
extern const struct bg_package gTitleArrow1Obj;
extern const struct bg_package gTitleBandicootObj;

/* The company logos' graphics (src/data/slot_seeds_17d6c0.c,
 * countdown_17d7a4.c). */
extern const struct bg_package gVvLogoEmblemObj;
extern const struct bg_package gVvLogoLettersObj;
extern const struct bg_package gVvLogoUrlObj;
extern const struct bg_package gUniversalLogoBg;

/* The credits (src/data/credits_17c5d0.c, popup_glyphs_17cf40.c). */
extern const u8 gCreditsText[];
extern const u8 gCreditsEmptyText[4];
extern const struct bg_package gCreditsLogos[5];

/* src/frontend/credits.cpp (C++, frontend.hpp: the C names of Credits's
 * methods, for the C callers) */
extern void RunCredits(void);

/* src/frontend/language_select.cpp (C++, as above) */
extern s32 RunLanguageSelect(void);

/* src/frontend/language_select_setup.cpp (C++, as above) */
extern void CloseLanguageSelect(void);
extern void OpenLanguageSelect(void);

/* Clears one OAM entry (4 words) with a DMA3 fill from `zero`, a
 * variable the caller declares (company_logos.cpp, title_screen_init.cpp).
 * A macro, not a function: `zero` is stored before the DMA base is
 * loaded, as in the ROM. */
#define CLEAR_OAM(oam)                                          \
{                                                               \
    struct dma_regs *dma;                                       \
    zero = 0;                                                   \
    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;                  \
    dma->src = (u32)&zero;                                      \
    dma->dst = (u32)(oam);                                      \
    dma->cnt = 0x81000004;                                      \
    dma->cnt;                                                   \
}

#endif /* GUARD_FRONTEND_H */
