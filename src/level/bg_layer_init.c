#include "core.h"
#include "bg_scroll_layer.h"

extern void InitBgLayerBase(void *self, s32 bgIndex);
extern u8 gBgLayerVtable[];

/* Initializes a BG-scroll-layer object (see crate_hit.c's viewport/
 * parallax-scroll-layer family) for hardware BG `bgIndex`: caches
 * `gBgLayerVtable` as its method table, screen block
 * `bgIndex + 0x1c`'s address as `screen`, `&REG_BGnCNT` as `cntReg`,
 * `&REG_BGnHOFS` as `ofsReg`, and the BGnCNT shadow `cnt` (screen base
 * `(bgIndex + 0x1c) & 0x1f`, char base 2, priority 0).
 *
 * Was NAKED asm, not plain C - see
 * docs/matching/naked-sub_8025d74-matched.md for the derivation. The
 * `& -0x20`/`& -0xd` masks always fold to their positive
 * byte-immediate equivalent instead of the ROM's runtime `movs`+`rsbs`
 * negation, closed the same way as `SetDispcntMode`/`CollideCrateWithPlayer`:
 * materializing each fold as an opaque inline-asm block. A second gap
 * (the three pointer-sized constants this function loads all need to
 * land in one shared literal pool, in the ROM's own order, for the
 * function to stay byte-exact - letting even one of them fall back to
 * an ordinary C reference lets the compiler's own pool disagree on
 * count/order with the other two) is closed by materializing all
 * three loads too, against one explicit trailing pool this function
 * owns outright. */
void *InitBgLayer(void *self, s32 bgIndex)
{
    register struct bg_scroll_layer *s asm("r5") = self;
    register s32 idx asm("r4") = bgIndex;
    register s32 t asm("r1");
    register u8 *addr34 asm("r2");
    register u8 *addr35 asm("r3");

    InitBgLayerBase(self, bgIndex);

    { register void *gsPtr asm("r0");
      asm volatile("ldr %0, 90f" : "=r"(gsPtr));
      s->vtable = gsPtr; }

    t = idx + 0x1c;
    s->screen = (u16 *)((t << 0xb) + (0xc0 << 0x13)); /* BG_SCREEN_ADDR(t) */

    { register s32 shifted1 asm("r0") = idx << 1;
      register s32 bgnCntAddr asm("r3");
      asm volatile("ldr %0, 90f+4" : "=r"(bgnCntAddr));
      s->cntReg = (vu16 *)(shifted1 + bgnCntAddr); }

    idx = idx << 2;
    { register s32 bgnHofsAddr asm("r0");
      asm volatile("ldr %0, 90f+8" : "=r"(bgnHofsAddr));
      idx = idx + bgnHofsAddr; }
    s->ofsReg = (vu32 *)idx;

    s->cnt.raw = 0;

    addr34 = (u8 *)&s->cnt;
    asm volatile(
        "mov r0, #0x7f\n\t"
        "ldrb r3, [%0]\n\t"
        "and r0, r0, r3\n\t"
        "strb r0, [%0]\n\t"
        : : "r"(addr34) : "r0", "r3", "memory"
    );

    addr35 = (u8 *)&s->cnt + 1;
    t = t & 0x1f;
    asm volatile(
        "mov r0, #0x20\n\t"
        "neg r0, r0\n\t"
        "ldrb r4, [%0]\n\t"
        "and r0, r0, r4\n\t"
        "orr r0, r0, %1\n\t"
        "strb r0, [%0]\n\t"
        : : "r"(addr35), "r"(t) : "r0", "r4", "memory"
    );

    asm volatile(
        "mov r0, #0xd\n\t"
        "neg r0, r0\n\t"
        "ldrb r1, [%0]\n\t"
        "and r0, r0, r1\n\t"
        "mov r1, #8\n\t"
        "orr r0, r0, r1\n\t"
        "strb r0, [%0]\n\t"
        : : "r"(addr34) : "r0", "r1", "memory"
    );

    return (void *)s;
}
#define ASM_STR2(x) #x
#define ASM_STR(x) ASM_STR2(x)
asm(".align 2, 0\n90: .word gBgLayerVtable\n.word " ASM_STR(REG_ADDR_BG0CNT)
    "\n.word " ASM_STR(REG_ADDR_BG0HOFS));

extern void _call_via_r2(void *arg0, s32 arg1, void *fn);

/* Grows `self+0x3c` down to `lo` and `self+0x40` up to `hi` one step
 * at a time, firing `self->0x30`'s `+0x30`-offset/`+0x34`-fn trampoline
 * (via `_call_via_r2`, an interworking veneer picked automatically by
 * the compiler for indirect calls - see `SpawnEntity` in
 * entity_spawner.c) after every step - the streamed-tile-range grower
 * `ScrollBgLayer` drives for one axis; `GrowBgLayerColumns` is its twin for the
 * other axis's `+0x44`/`+0x48` fields. */
void GrowBgLayerRows(struct bg_scroll_layer *self, s32 lo, s32 hi)
{
    while (self->rowLo > lo) {
        struct bg_layer_vtable *layer;
        s32 off;
        void *addr;
        void *fn;
        s32 v;

        v = self->rowLo - 1;
        self->rowLo = v;
        layer = self->vtable;
        off = layer->drawRow.thisOffset;
        addr = (u8 *)self + off;
        fn = layer->drawRow.fn;
        _call_via_r2(addr, v, fn);
    }
    while (self->rowHi < hi) {
        struct bg_layer_vtable *layer;
        s32 off;
        void *addr;
        void *fn;
        s32 v;

        v = self->rowHi + 1;
        self->rowHi = v;
        layer = self->vtable;
        off = layer->drawRow.thisOffset;
        addr = (u8 *)self + off;
        fn = layer->drawRow.fn;
        _call_via_r2(addr, v, fn);
    }
}

/* Same shape as `GrowBgLayerRows` above, but grows `self+0x44`/`self+0x48`
 * via `self->0x30`'s `+0x38`-offset/`+0x3c`-fn trampoline instead. */
void GrowBgLayerColumns(struct bg_scroll_layer *self, s32 lo, s32 hi)
{
    while (self->colLo > lo) {
        struct bg_layer_vtable *layer;
        s32 off;
        void *addr;
        void *fn;
        s32 v;

        v = self->colLo - 1;
        self->colLo = v;
        layer = self->vtable;
        off = layer->drawCol.thisOffset;
        addr = (u8 *)self + off;
        fn = layer->drawCol.fn;
        _call_via_r2(addr, v, fn);
    }
    while (self->colHi < hi) {
        struct bg_layer_vtable *layer;
        s32 off;
        void *addr;
        void *fn;
        s32 v;

        v = self->colHi + 1;
        self->colHi = v;
        layer = self->vtable;
        off = layer->drawCol.thisOffset;
        addr = (u8 *)self + off;
        fn = layer->drawCol.fn;
        _call_via_r2(addr, v, fn);
    }
}

/* Clamp-to-at-least/at-most pair on `self+0x44`(max with `a`)/
 * `self+0x48`(min with `b`) - the bookkeeping half of the
 * `GrowBgLayerRows`/`GrowBgLayerColumns` streamed-range growers above (this
 * variant just widens the recorded extent, without firing any
 * trampoline). */
void ClipBgLayerColumns(struct bg_scroll_layer *self, s32 a, s32 b)
{
    if (self->colLo < a) {
        self->colLo = a;
    }
    if (self->colHi > b) {
        self->colHi = b;
    }
}

/* Same shape as `ClipBgLayerColumns` above, on `self+0x3c`/`self+0x40`. */
void ClipBgLayerRows(struct bg_scroll_layer *self, s32 a, s32 b)
{
    if (self->rowLo < a) {
        self->rowLo = a;
    }
    if (self->rowHi > b) {
        self->rowHi = b;
    }
}
