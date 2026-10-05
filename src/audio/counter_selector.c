#include "core.h"
#include "gba/dma_macros.h"
#include "memory.h"

/* The language menu shown at boot (OpenLanguageSelect/RunLanguageSelect/
 * CloseLanguageSelect, called from MainLoop): up/down cycles `language`
 * through the six entries of gLanguageNames ("english", "français",
 * "deutsch", "español", "italiano", "nederlands", drawn with gSmallFont
 * over a starfield), A or START confirms, and MainLoop stores the
 * result in gLanguage, which picks the gUiText<Lang>/cutscene tables.
 * Sits at the very start of the address range docs/audio.md calls the
 * GAX2 engine, but is game-side code that merely uses PlaySfx.
 * (Formerly `struct counter_widget`.) */
struct language_select {
    u32 frame;
    u8 done;
    u8 pad_5[3];
    s32 language;
    u8 field_c;
    u8 field_d;
    u8 pad_e[2];
    void *starfield;
};

extern void *OperatorNewArray(u32 size);
extern void OperatorDeleteArray(void *ptr);
extern void OperatorDelete(void *self);
extern void WaitForVBlank(void);
extern void *gInput;
extern u16 gKeys;
extern void *gAudioContext;
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);
extern s32 UpdateKeys(void *arg0);
extern void CommitLanguageSelectFrame(struct language_select *self);
extern void DrawLanguageSelect(struct language_select *self);
extern void UpdateStarfield(void *arg0);
extern struct language_select *gLanguageSelect;
extern void LoadTaggedAsset(void *asset, void *dest);

void LanguageSelectInput(struct language_select *self, u32 flags);

/* Loads a "tagged" asset (see LoadTaggedAsset, src/system/asset_util.c)
 * into a freshly allocated buffer, then queues a DMA3 transfer from that
 * buffer out to `dest` - `unused` (r0) is never read. */
void LoadTaggedAssetBuffered(void *unused, void *asset, void *dest)
{
    u32 val = *(u32 *)asset;
    struct dma_regs *dma;
    void *buf;
    u32 cnt;

    val >>= 8;
    buf = OperatorNewArray(val);
    LoadTaggedAsset(asset, buf);
    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
    dma->src = (u32)buf;
    dma->dst = (u32)dest;
    val >>= 1;
    dma->cnt = val | 0x80000000;
    cnt = dma->cnt;
    if (buf != NULL) {
        OperatorDeleteArray(buf);
    }
}

void nullsub_7(void)
{
}

void DestroyCompanyLogos(void *self, u32 flags)
{
    if (flags & 1) {
        OperatorDelete(self);
    }
}

/* The company-logo actor's destructor (RunCompanyLogos, graphics_loading_
 * 35d1c.c): slot 1 of gLogoActorVtable, so no `bl` reaches it - it is
 * called through the vtable with the deleting flags 3. It frees the two
 * VRAM tile blocks InitLogoActor allocated, drops back to the base
 * gActorVtable, unlinks the actor from the actor ring and frees it on
 * flags bit 0 - the same shape as DestroyActor. Its object is a much
 * larger one than the 0x14-byte `language_select` every neighboring
 * function in this file operates on, so it gets its own minimal,
 * locally-scoped struct. */
struct linked_node {
    u8 unused_00[0x48];
    struct linked_node *prev;
    struct linked_node *next;
    void *field_50;
};

extern void FreeVramTileBlock(void *arg0);
extern void *gLogoActorTiles[2];
extern u8 gLogoActorVtable[];
extern u8 gActorVtable[];

void DestroyLogoActor(struct linked_node *self, u32 flags)
{
    self->field_50 = gLogoActorVtable;
    FreeVramTileBlock(gLogoActorTiles[0]);
    FreeVramTileBlock(gLogoActorTiles[1]);
    self->field_50 = gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

/* Runs the widget: resets it, draws/flushes once, then polls input each
 * frame (dispatching newly-pressed keys to LanguageSelectInput) until it signals
 * done via `field_4`, returning the final selected value in `field_8`. */
s32 RunLanguageSelect(void)
{
    gLanguageSelect->language = 0;
    gLanguageSelect->frame = 0;
    gLanguageSelect->done = 0;
    DrawLanguageSelect(gLanguageSelect);
    WaitForVBlank();
    CommitLanguageSelectFrame(gLanguageSelect);

    while (gLanguageSelect->done == 0) {
        u16 keys;
        u16 *addr;

        UpdateKeys(gInput);
        addr = &gKeys;
        keys = *(u16 *)((u8 *)addr + 2);
        LanguageSelectInput(gLanguageSelect, keys);
        DrawLanguageSelect(gLanguageSelect);
        WaitForVBlank();
        CommitLanguageSelectFrame(gLanguageSelect);
        UpdateStarfield(gLanguageSelect->starfield);
    }

    return gLanguageSelect->language;
}

/* Dispatches one frame's newly-pressed `flags` for the widget above:
 * bit 3 or bit 0 confirms/cancels (sets `field_4` to end the loop, sfx
 * 0x49); bit 6/bit 7 decrement/increment the 0-5 `field_8` value
 * (wrapping around, sfx 0x46). `field_0` is a free-running frame
 * counter, incremented every call regardless. */
void LanguageSelectInput(struct language_select *self, u32 flags)
{
    if (flags & 8) {
        self->done = 1;
        goto confirm;
    } else if (flags & 1) {
        self->done = 1;
    confirm:
        PlaySfx(gAudioContext, 0x49, 0x100);
    } else if (flags & 0x40) {
        self->language--;
        if (self->language < 0) {
            self->language = 5;
        }
        PlaySfx(gAudioContext, 0x46, 0x100);
    } else if (flags & 0x80) {
        self->language++;
        if (self->language > 5) {
            self->language = 0;
        }
        PlaySfx(gAudioContext, 0x46, 0x100);
    }
    self->frame = (self->frame + 1) & 0xff;
}
