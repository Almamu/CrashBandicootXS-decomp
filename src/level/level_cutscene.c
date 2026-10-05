#include "core.h"
#include "gba/io_reg.h"
#include "gba/dma_macros.h"
#include "bitmap_font.h"

/* 0x08022354-0x080225A0, formerly asm/code_3_2_17_22354.s: the two
 * functions between issue #33's chunk (graphics_loading_21d80.c, which
 * ends with the game-context constructor InitLevelState) and
 * UpdateGameFrame (game_loop55.c). See
 * docs/matching/gap-22354-game-context.md.
 *
 * - DestroyLevelState (UNUSED): the destructor matching InitLevelState - frees every
 *   subsystem singleton that constructor built and clears the context
 *   pointer gLevelStateSingleton.
 * - PlayCutscene: plays cutscene `idx` (include/cutscene.h): blanks the
 *   palette, resets the BG2 affine transform, then runs a stack-allocated
 *   cutscene player (InitCutscenePlayer/RunCutscenePlayer) over the
 *   slides gCutscenes[idx] and the current language's pages until it
 *   finishes.
 *
 * Both match under either compiler; built with the current agbcc like
 * their neighbours. */

extern void *gLevelStateSingleton;
extern u16 gDispcnt;
extern void *gOamBuffer;
extern void *gObjVramCursor;
extern void *gInput;
extern void *gAudioContext;
extern struct bitmap_font *gLargeFont;
extern struct bitmap_font *gSmallFont;
extern void *gSpriteRenderer;
extern void *gSpriteBankSet;
extern void *gPaletteCache;
extern void *gEntityFlags;
extern void *gPaletteCycles;
extern u32 *gCutsceneTexts[];
extern s32 gLanguage;

/* gCutscenes: {list, count} headers, one per text list
 * (docs/rom_map.md, "gCutscenes is a header array of
 * variable-length lists"). */
struct text_list
{
    void **items;
    s32 count;
};
extern struct text_list gCutscenes[];

extern void FreeVramDmaQueue(void);
extern void DestroyOamBuffer(void *self, u32 flags);
extern void DestroyObjVramCursor(void *self, u32 flags);
extern void OperatorDelete(void *p);
/* Takes no argument (audio.c), but the ROM loads the audio
 * context into r0 before calling it anyway. */
extern void DisableMusicVCountIrq(void *audio);
extern void DestroyAudioContext(void *audio, u32 flags);
extern void DestroySpriteRenderer(void *self, u32 flags);
extern void DestroySpriteBankSet(void *self, u32 flags);
extern void DestroyPaletteCache(void *self, u32 flags);
extern void DestroyEntityFlags(void *self, s32 flags);
extern void DestroyPaletteCycles(void *self, s32 flags);

typedef void (*destroy_fn)(void *self, s32 flags);

/* Destroys an icon manager through its method table (a gcc 2.x virtual
 * `delete`). */
#define DESTROY_FONT(m)                                                \
    {                                                                          \
        struct bitmap_font *_m = (m);                                         \
        struct icon_record *_r = _m->record;                                   \
        ((destroy_fn)_r->destroy.ptr)((u8 *)_m + _r->destroy.offset, 3);       \
    }

/* UNUSED - no caller anywhere in the ROM (checked every asm/ and src/
 * file for the symbol, every Thumb `bl` in baserom.gba for its address,
 * and every word in baserom.gba for 0x08022355). Matched anyway.
 *
 * The game context's destructor: tears down every subsystem singleton
 * InitLevelState (graphics_loading_21d80.c) constructed, each with the
 * "delete" flags 3, clears the context pointer and - on bit 0 of
 * `flags`, gcc 2.x's deleting-destructor flag - frees `self`. The game
 * never leaves MainLoop, so it never runs. */
void DestroyLevelState(void *self, s32 flags)
{
    FreeVramDmaQueue();
    if (gOamBuffer != NULL)
        DestroyOamBuffer(gOamBuffer, 3);
    if (gObjVramCursor != NULL)
        DestroyObjVramCursor(gObjVramCursor, 3);
    if (gInput != NULL)
        OperatorDelete(gInput);
    DisableMusicVCountIrq(gAudioContext);
    if (gAudioContext != NULL)
        DestroyAudioContext(gAudioContext, 3);
    if (gLargeFont != NULL)
        DESTROY_FONT(gLargeFont);
    if (gSmallFont != NULL)
        DESTROY_FONT(gSmallFont);
    if (gSpriteRenderer != NULL)
        DestroySpriteRenderer(gSpriteRenderer, 3);
    if (gSpriteBankSet != NULL)
        DestroySpriteBankSet(gSpriteBankSet, 3);
    if (gPaletteCache != NULL)
        DestroyPaletteCache(gPaletteCache, 3);
    if (gEntityFlags != NULL)
        DestroyEntityFlags(gEntityFlags, 3);
    if (gPaletteCycles != NULL)
        DestroyPaletteCycles(gPaletteCycles, 3);
    gLevelStateSingleton = NULL;
    if (flags & 1)
        OperatorDelete(self);
}

struct text_vec
{
    s32 x;
    s32 y;
};

struct text_rect
{
    struct text_vec pos;
    struct text_vec size;
};

/* The text pager InitCutscenePlayer constructs and RunCutscenePlayer runs
 * (cutscene_player.c): the +0/+4 item list, the +0x10 per-item page table,
 * the font (an icon manager) at +0x14 and the text box at +0x18. */
struct text_pager
{
    void **items;                   // 0x00
    s32 count;                      // 0x04
    u8 unk_08[8];
    u32 *pages;                     // 0x10
    struct bitmap_font *font;      // 0x14
    struct text_rect box;           // 0x18
};

extern void SetDispcntMode(s32 val);
extern void ShowBg2(void);
extern void ShowObj(void);
extern void CommitDispcnt(void);
extern void FreeUnlockedPaletteSlots(void *cache);
extern void UploadPaletteCache(void *cache);
extern void FontResetPalette(struct bitmap_font *self);
extern void InitCutscenePlayer(struct text_pager *self);
extern void SetSlideshowDispcnt(u32 value);
extern void RunCutscenePlayer(struct text_pager *self);
extern void DestroyCutscenePlayer(struct text_pager *self, s32 flags);

typedef void (*method_fn)(void *self);

/* Built with a brace initializer from its parameters so the pair is
 * materialized in a register pair and stored in one go, as the ROM does
 * (field-by-field assignment of a local, or a constant initializer,
 * compile differently). */
static inline struct text_vec MakeVec(s32 x, s32 y)
{
    struct text_vec v = {x, y};

    return v;
}

void PlayCutscene(void *self, s32 idx)
{
    /* One aggregate so that every field access stays sp-relative: as
     * separate locals, the pager's field stores go through the register
     * holding &pager instead. */
    struct {
        struct text_rect box;
        u16 fill;
        struct text_pager pager;
    } f;
    s32 zero;
    u16 mode;
    u16 *dispcnt;

    f.box.pos = MakeVec(7, 0x7E);
    f.box.size = MakeVec(0xE4, 0x1E);
    dispcnt = &gDispcnt;
    zero = 0;
    mode = 0x40;
    *dispcnt = mode;
    SetDispcntMode(4);
    ShowBg2();
    ShowObj();
    {
        u16 *src = &f.fill;

        *src = zero;
        DmaSet(3, src, PLTT, (DMA_ENABLE | DMA_START_NOW | DMA_16BIT | DMA_SRC_FIXED | DMA_DEST_INC) << 16 | (BG_PLTT_SIZE / 2));
    }
    REG_BG2PA = 0x100;
    REG_BG2PB = zero;
    REG_BG2PC = zero;
    REG_BG2PD = 0x100;
    REG_BG2X = zero;
    REG_BG2Y = zero;
    FreeUnlockedPaletteSlots(gPaletteCache);
    {
        struct bitmap_font *m = gSmallFont;
        u32 tileBase = 0x200;
        struct icon_slot *slot;

        m->tileBase = tileBase;
        slot = &m->record->slots[6];
        ((method_fn)slot->ptr)((u8 *)m + slot->offset);
    }
    FontResetPalette(gSmallFont);
    UploadPaletteCache(gPaletteCache);
    InitCutscenePlayer(&f.pager);
    f.pager.font = gSmallFont;
    {
        /* f.pager.box = f.box, spelled out: the ROM stores the two x
         * words sp-relative and the two y words through one pointer
         * register - see docs/matching/gap-22354-game-context.md. */
        s32 x0 = f.box.pos.x;
        s32 y0 = f.box.pos.y;
        s32 *d = &f.pager.box.pos.x;
        s32 x1, y1;

        d[0] = x0;
        d[1] = y0;
        x1 = f.box.size.x;
        y1 = f.box.size.y;
        f.pager.box.size.x = x1;
        d[3] = y1;
    }
    SetSlideshowDispcnt(*(u32 *)dispcnt);
    f.pager.items = gCutscenes[idx].items;
    f.pager.count = gCutscenes[idx].count;
    f.pager.pages = (u32 *)gCutsceneTexts[gLanguage][idx];
    RunCutscenePlayer(&f.pager);
    *dispcnt = mode;
    CommitDispcnt();
    DestroyCutscenePlayer(&f.pager, 2);
}
