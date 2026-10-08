#include "level_state.hpp"
#include "sprite_obj.hpp"
#include "part_list.hpp"
#include "spawners.hpp"
#include "font.hpp"

extern "C" {
#include "core.h"
#include "gba/io_reg.h"
#include "gba/dma_macros.h"
#include "bitmap_font.h"
#include "text.h"
#include "cutscene.h"
#include "system.h"
#include "audio.h"
#include "gfx.h"
#include "objects.h"
#include "level.h"
#include "globals.h"
}

/* 0x08022354-0x080225A0, formerly asm/code_3_2_17_22354.s: the two
 * functions between issue #33's chunk (spawn_pickups.cpp, which
 * ends with the game-context constructor InitLevelState) and
 * UpdateGameFrame (game_frame.cpp). See
 * docs/matching/archive/gap-22354-game-context.md.
 *
 * - DestroyLevelState (UNUSED): LevelState's destructor
 *   (include/level_state.hpp), the counterpart of its constructor
 *   (InitLevelState) - frees every subsystem singleton that constructor
 *   built and clears the context pointer gLevelStateSingleton.
 * - PlayCutscene: plays cutscene `idx` (include/cutscene.h): blanks the
 *   palette, resets the BG2 affine transform, then runs a stack-allocated
 *   cutscene player (InitCutscenePlayer/RunCutscenePlayer) over the
 *   slides gCutscenes[idx] and the current language's pages until it
 *   finishes.
 *
 * C++ since the #664 cleanup (the destructor's `delete`s and the font's
 * virtual calls were spelled out as calls and slot reads); built with the
 * current agbcp, as the C was with agbcc. */

/* UNUSED - no caller anywhere in the ROM (checked every asm/ and src/
 * file for the symbol, every Thumb `bl` in baserom.gba for its address,
 * and every word in baserom.gba for 0x08022355). Matched anyway.
 *
 * The game context's destructor: deletes every subsystem singleton its
 * constructor (InitLevelState, spawn_pickups.cpp) built (the fonts
 * through their virtual destructors) and clears the context pointer;
 * g++'s deleting destructor then frees `this` on bit 0 of its __in_chrg.
 * The game never leaves MainLoop, so it never runs. The globals keep
 * their C types, so each `delete` names its class. */
LevelState::~LevelState()
{
    FreeVramDmaQueue();
    delete (OamBuffer *)gOamBuffer;
    delete (ObjVramCursor *)gObjVramCursor;
    if (gInput != NULL) /* KeyInput has no destructor: `delete` alone tests nothing */
        delete (KeyInput *)gInput;
    DisableMusicVCountIrq(gAudioContext);
    if (gAudioContext != NULL)
        DestroyAudioContext(gAudioContext, 3);
    delete gLargeFont;
    delete gSmallFont;
    delete (SpriteRenderer *)gSpriteRenderer;
    delete (SpriteBankSet *)gSpriteBankSet;
    delete (PaletteCache *)gPaletteCache;
    delete (LevelEntityFlags *)gEntityFlags;
    delete (PaletteCycles *)gPaletteCycles;
    gLevelStateSingleton = 0;
}

struct text_vec {
    s32 x;
    s32 y;
};

struct text_rect {
    struct text_vec pos;
    struct text_vec size;
};

/* Through an inline's parameters both values of a pair are loaded before
 * the two stores, as in the ROM (the C needed a brace-initialized struct
 * returned by value; returned by value in C++, the pair goes through a
 * stack temporary, and stored field by field, each constant is loaded
 * right before its store). */
static inline void SetVec(struct text_vec *v, s32 x, s32 y)
{
    v->x = x;
    v->y = y;
}

void PlayCutscene(void *self, s32 idx)
{
    /* One aggregate so that every field access stays sp-relative: as
     * separate locals, the pager's field stores go through the register
     * holding &pager instead. */
    struct {
        struct text_rect box;
        u16 fill;
        struct cutscene_player pager;
    } f;
    s32 zero;
    u16 mode;
    u16 *dispcnt;

    SetVec(&f.box.pos, 7, 0x7E);
    SetVec(&f.box.size, 0xE4, 0x1E);
    dispcnt = (u16 *)gDispcnt;
    zero = 0;
    mode = 0x40;
    *dispcnt = mode;
    SetDispcntMode(4);
    ShowBg2();
    ShowObj();
    {
        u16 *src = &f.fill;

        *src = zero;
        DmaSet(3, src, PLTT,
               (DMA_ENABLE | DMA_START_NOW | DMA_16BIT | DMA_SRC_FIXED | DMA_DEST_INC) << 16 |
                   (BG_PLTT_SIZE / 2));
    }
    REG_BG2PA = 0x100;
    REG_BG2PB = zero;
    REG_BG2PC = zero;
    REG_BG2PD = 0x100;
    REG_BG2X = zero;
    REG_BG2Y = zero;
    FreeUnlockedPaletteSlots(gPaletteCache);
    gSmallFont->SetTileBase(0x200);
    gSmallFont->ResetPalette();
    UploadPaletteCache(gPaletteCache);
    InitCutscenePlayer(&f.pager);
    f.pager.font = gSmallFont;
    {
        /* f.pager.box = f.box, spelled out: the ROM stores the two x
         * words sp-relative and the two y words through one pointer
         * register - see docs/matching/archive/gap-22354-game-context.md. */
        s32 x0 = f.box.pos.x;
        s32 y0 = f.box.pos.y;
        s32 *d = &f.pager.box.x;
        s32 x1, y1;

        d[0] = x0;
        d[1] = y0;
        x1 = f.box.size.x;
        y1 = f.box.size.y;
        f.pager.box.w = x1;
        d[3] = y1;
    }
    SetSlideshowDispcnt(*(u32 *)dispcnt);
    f.pager.slides = gCutscenes[idx].slides;
    f.pager.count = gCutscenes[idx].count;
    f.pager.pages = gCutsceneTexts[gLanguage][idx];
    RunCutscenePlayer(&f.pager);
    *dispcnt = mode;
    CommitDispcnt();
    DestroyCutscenePlayer(&f.pager, 2);
}
