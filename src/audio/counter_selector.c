#include "core.h"
#include "gba/dma_macros.h"
#include "memory.h"

/* A small on-screen widget: cycles a 0-5 value with input (drawn via two
 * flanking icons through gUnknown_030012DC/gUnknown_030012E0, see
 * icon_manager.h) and confirms/cancels with a PlaySfx cue. Sits at the
 * very start of the address range docs/audio.md calls the GAX2 engine,
 * but reads like game/HUD-side code that merely *uses* PlaySfx rather
 * than GAX2 engine internals - not confidently identified as any one
 * specific screen (a jukebox/sound-test track selector is the leading
 * guess, given the SFX ids and the 0-5 range, but not confirmed), so
 * kept as sub_XXXXXXXX per docs/naming.md rather than guessing a name. */
struct counter_widget {
    u32 field_0;
    u8 field_4;
    u8 pad_5[3];
    s32 field_8;
    u8 field_c;
    u8 field_d;
    u8 pad_e[2];
    void *field_10;
};

extern void *sub_8026EC0(u32 size);
extern void sub_8026EB4(void *ptr);
extern void sub_8026ED0(void *self);
extern void sub_80006A8(void);
extern void *gUnknown_03001304;
extern u16 gUnknown_030007E0;
extern void *gUnknown_030012BC;
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);
extern s32 sub_80007AC(void *arg0);
extern void sub_8037548(struct counter_widget *self);
extern void sub_80372BC(struct counter_widget *self);
extern void sub_8034688(void *arg0);
extern struct counter_widget *gUnknown_030008CC;
extern void LoadTaggedAsset(void *asset, void *dest);

void sub_8037224(struct counter_widget *self, u32 flags);

/* Loads a "tagged" asset (see LoadTaggedAsset, src/system/asset_util.c)
 * into a freshly allocated buffer, then queues a DMA3 transfer from that
 * buffer out to `dest` - `unused` (r0) is never read. */
void sub_8037110(void *unused, void *asset, void *dest)
{
    u32 val = *(u32 *)asset;
    struct dma_regs *dma;
    void *buf;
    u32 cnt;

    val >>= 8;
    buf = sub_8026EC0(val);
    LoadTaggedAsset(asset, buf);
    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
    dma->src = (u32)buf;
    dma->dst = (u32)dest;
    val >>= 1;
    dma->cnt = val | 0x80000000;
    cnt = dma->cnt;
    if (buf != NULL) {
        sub_8026EB4(buf);
    }
}

void nullsub_7(void)
{
}

void sub_8037154(void *self, u32 flags)
{
    if (flags & 1) {
        sub_8026ED0(self);
    }
}

/* UNUSED - no caller anywhere in the ROM (checked every asm/, expected/,
 * and src/ source file for a `bl sub_803716C` or a raw `0x0803716D`
 * reference). Unlinks `self` from a doubly-linked list
 * (`next->prev = prev; prev->next = next;`) after flipping a
 * state/vtable-looking pointer at +0x50 between two constants and
 * running two commit-style calls in between - a much larger, unrelated
 * object than the 0x14-byte `counter_widget` every neighboring function
 * in this file operates on, so it gets its own minimal, locally-scoped
 * struct instead of being folded into that one. */
struct linked_node {
    u8 unused_00[0x48];
    struct linked_node *prev;
    struct linked_node *next;
    void *field_50;
};

extern void sub_8028C48(void *arg0);
extern void *gUnknown_0300160C[2];
extern u8 gStaticData_087E55C4[];
extern u8 gStaticData_087E4DF4[];

void sub_803716C(struct linked_node *self, u32 flags)
{
    self->field_50 = gStaticData_087E55C4;
    sub_8028C48(gUnknown_0300160C[0]);
    sub_8028C48(gUnknown_0300160C[1]);
    self->field_50 = gStaticData_087E4DF4;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

/* Runs the widget: resets it, draws/flushes once, then polls input each
 * frame (dispatching newly-pressed keys to sub_8037224) until it signals
 * done via `field_4`, returning the final selected value in `field_8`. */
s32 sub_80371B4(void)
{
    gUnknown_030008CC->field_8 = 0;
    gUnknown_030008CC->field_0 = 0;
    gUnknown_030008CC->field_4 = 0;
    sub_80372BC(gUnknown_030008CC);
    sub_80006A8();
    sub_8037548(gUnknown_030008CC);

    while (gUnknown_030008CC->field_4 == 0) {
        u16 keys;
        u16 *addr;

        sub_80007AC(gUnknown_03001304);
        addr = &gUnknown_030007E0;
        keys = *(u16 *)((u8 *)addr + 2);
        sub_8037224(gUnknown_030008CC, keys);
        sub_80372BC(gUnknown_030008CC);
        sub_80006A8();
        sub_8037548(gUnknown_030008CC);
        sub_8034688(gUnknown_030008CC->field_10);
    }

    return gUnknown_030008CC->field_8;
}

/* Dispatches one frame's newly-pressed `flags` for the widget above:
 * bit 3 or bit 0 confirms/cancels (sets `field_4` to end the loop, sfx
 * 0x49); bit 6/bit 7 decrement/increment the 0-5 `field_8` value
 * (wrapping around, sfx 0x46). `field_0` is a free-running frame
 * counter, incremented every call regardless. */
void sub_8037224(struct counter_widget *self, u32 flags)
{
    if (flags & 8) {
        self->field_4 = 1;
        goto confirm;
    } else if (flags & 1) {
        self->field_4 = 1;
    confirm:
        PlaySfx(gUnknown_030012BC, 0x49, 0x100);
    } else if (flags & 0x40) {
        self->field_8--;
        if (self->field_8 < 0) {
            self->field_8 = 5;
        }
        PlaySfx(gUnknown_030012BC, 0x46, 0x100);
    } else if (flags & 0x80) {
        self->field_8++;
        if (self->field_8 > 5) {
            self->field_8 = 0;
        }
        PlaySfx(gUnknown_030012BC, 0x46, 0x100);
    }
    self->field_0 = (self->field_0 + 1) & 0xff;
}
