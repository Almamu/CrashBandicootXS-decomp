#include "core.h"
#include "gba/io_reg.h"
#include "gba/dma_macros.h"
#include "icon_manager.h"

/* 0x08022354-0x080225A0, formerly asm/code_3_2_17_22354.s: the two
 * functions between issue #33's chunk (graphics_loading_21d80.c, which
 * ends with the game-context constructor sub_8022230) and
 * UpdateGameFrame (game_loop55.c). See
 * docs/matching/gap-22354-game-context.md.
 *
 * - sub_8022354 (UNUSED): the destructor matching sub_8022230 - frees every
 *   subsystem singleton that constructor built and clears the context
 *   pointer gUnknown_03000828.
 * - PlayCutscene: plays cutscene `idx` (include/cutscene.h): blanks the
 *   palette, resets the BG2 affine transform, then runs a stack-allocated
 *   cutscene player (InitCutscenePlayer/RunCutscenePlayer) over the
 *   slides gCutscenes[idx] and the current language's pages until it
 *   finishes.
 *
 * Both match under either compiler; built with the current agbcc like
 * their neighbours. */

extern void *gUnknown_03000828;
extern u16 gUnknown_03001288;
extern void *gUnknown_03001300;
extern void *gUnknown_030012FC;
extern void *gUnknown_03001304;
extern void *gUnknown_030012BC;
extern struct icon_manager *gUnknown_030012E0;
extern struct icon_manager *gUnknown_030012DC;
extern void *gUnknown_030012CC;
extern void *gUnknown_030012D0;
extern void *gUnknown_030012B8;
extern void *gEntityFlags;
extern void *gUnknown_030012C8;
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
extern void sub_8006AF4(void *self, u32 flags);
extern void sub_8006CD0(void *self, u32 flags);
extern void sub_8026ED0(void *p);
/* Takes no argument (audio_context.c), but the ROM loads the audio
 * context into r0 before calling it anyway. */
extern void sub_8001C64(void *audio);
extern void sub_8001C04(void *audio, u32 flags);
extern void sub_8007A98(void *self, u32 flags);
extern void sub_8006FC8(void *self, u32 flags);
extern void sub_8006F94(void *self, u32 flags);
extern void sub_8025A44(void *self, s32 flags);
extern void sub_80270A8(void *self, s32 flags);

typedef void (*destroy_fn)(void *self, s32 flags);

/* Destroys an icon manager through its method table (a gcc 2.x virtual
 * `delete`). */
#define DESTROY_ICON_MANAGER(m)                                                \
    {                                                                          \
        struct icon_manager *_m = (m);                                         \
        struct icon_record *_r = _m->record;                                   \
        ((destroy_fn)_r->destroy.ptr)((u8 *)_m + _r->destroy.offset, 3);       \
    }

/* UNUSED - no caller anywhere in the ROM (checked every asm/ and src/
 * file for the symbol, every Thumb `bl` in baserom.gba for its address,
 * and every word in baserom.gba for 0x08022355). Matched anyway.
 *
 * The game context's destructor: tears down every subsystem singleton
 * sub_8022230 (graphics_loading_21d80.c) constructed, each with the
 * "delete" flags 3, clears the context pointer and - on bit 0 of
 * `flags`, gcc 2.x's deleting-destructor flag - frees `self`. The game
 * never leaves MainLoop, so it never runs. */
void sub_8022354(void *self, s32 flags)
{
    FreeVramDmaQueue();
    if (gUnknown_03001300 != NULL)
        sub_8006AF4(gUnknown_03001300, 3);
    if (gUnknown_030012FC != NULL)
        sub_8006CD0(gUnknown_030012FC, 3);
    if (gUnknown_03001304 != NULL)
        sub_8026ED0(gUnknown_03001304);
    sub_8001C64(gUnknown_030012BC);
    if (gUnknown_030012BC != NULL)
        sub_8001C04(gUnknown_030012BC, 3);
    if (gUnknown_030012E0 != NULL)
        DESTROY_ICON_MANAGER(gUnknown_030012E0);
    if (gUnknown_030012DC != NULL)
        DESTROY_ICON_MANAGER(gUnknown_030012DC);
    if (gUnknown_030012CC != NULL)
        sub_8007A98(gUnknown_030012CC, 3);
    if (gUnknown_030012D0 != NULL)
        sub_8006FC8(gUnknown_030012D0, 3);
    if (gUnknown_030012B8 != NULL)
        sub_8006F94(gUnknown_030012B8, 3);
    if (gEntityFlags != NULL)
        sub_8025A44(gEntityFlags, 3);
    if (gUnknown_030012C8 != NULL)
        sub_80270A8(gUnknown_030012C8, 3);
    gUnknown_03000828 = NULL;
    if (flags & 1)
        sub_8026ED0(self);
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
 * (game_loop57.c): the +0/+4 item list, the +0x10 per-item page table,
 * the font (an icon manager) at +0x14 and the text box at +0x18. */
struct text_pager
{
    void **items;                   // 0x00
    s32 count;                      // 0x04
    u8 unk_08[8];
    u32 *pages;                     // 0x10
    struct icon_manager *font;      // 0x14
    struct text_rect box;           // 0x18
};

extern void sub_8001524(s32 val);
extern void sub_80015B0(void);
extern void sub_80015E0(void);
extern void sub_8001614(void);
extern void sub_8006EA8(void *cache);
extern void sub_8006DC8(void *cache);
extern void sub_8028A40(struct icon_manager *self);
extern void InitCutscenePlayer(struct text_pager *self);
extern void sub_8024784(u32 value);
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
    dispcnt = &gUnknown_03001288;
    zero = 0;
    mode = 0x40;
    *dispcnt = mode;
    sub_8001524(4);
    sub_80015B0();
    sub_80015E0();
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
    sub_8006EA8(gUnknown_030012B8);
    {
        struct icon_manager *m = gUnknown_030012DC;
        u32 tileBase = 0x200;
        struct icon_slot *slot;

        m->field_108 = tileBase;
        slot = &m->record->slots[6];
        ((method_fn)slot->ptr)((u8 *)m + slot->offset);
    }
    sub_8028A40(gUnknown_030012DC);
    sub_8006DC8(gUnknown_030012B8);
    InitCutscenePlayer(&f.pager);
    f.pager.font = gUnknown_030012DC;
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
    sub_8024784(*(u32 *)dispcnt);
    f.pager.items = gCutscenes[idx].items;
    f.pager.count = gCutscenes[idx].count;
    f.pager.pages = (u32 *)gCutsceneTexts[gLanguage][idx];
    RunCutscenePlayer(&f.pager);
    *dispcnt = mode;
    sub_8001614();
    DestroyCutscenePlayer(&f.pager, 2);
}
