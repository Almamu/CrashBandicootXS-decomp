#include "core.h"
#include "gba/io_reg.h"
#include "icon_manager.h"
#include "actor_self.h"
#include "gba/dma_macros.h"
#include "graphics_package.h"

/* Middle part of GitHub issue #65's chunk (0x08035D1C-0x0803686C), split
 * off `graphics_loading_35780.c` at `sub_8035D1C` in the issues #64/#65
 * second NAKED retry. Both halves are old_agbcc code (see the Makefile's
 * OLD_AGBCC_OBJS), but only this one is built with -fno-strength-reduce
 * (NO_STRENGTH_REDUCE_OBJS): `sub_8036600` keeps its up-counting loop
 * only without strength reduction, while `sub_80358A8` (in the first
 * half) only matches with it (its inner loop is written up-counting and
 * strength reduction reverses it). The shared declarations below are
 * copied from the first half. Everything from `sub_803686C` on lives in
 * `graphics_loading_3686c.c`, which needs strength reduction on. See
 * docs/matching/issue-64-65-naked-retry-2.md and
 * docs/matching/sr65-naked-retry.md. */

extern struct oam_shadow_buffer *gUnknown_03001300;
extern struct AudioContext *gUnknown_030012BC;
extern u8 gUnknown_03001288[2];
extern void *gUnknown_03001304;
extern void *gUnknown_0300160C[2];
extern s32 gUnknown_03001604;
extern void *gUnknown_03001608;
extern void (*gUnknown_03000874)(void *dst, u8 *frame);
extern struct held_pressed_pair {
    u16 held;
    u16 pressed;
} gUnknown_030007E0;

extern u8 gStaticData_0817CFA4[];
extern u8 gStaticData_0817CFF4[];
extern u8 gStaticData_0817D698[];
extern u8 gStaticData_08178F80[];
extern u8 gStaticData_0817D768[];
extern u8 gStaticData_0817D77C[];
extern u8 gStaticData_0817D790[];
extern u8 gStaticData_0817D6C0[];
extern u8 gStaticData_0817D7A4[];
extern u8 gStaticData_087E55C4[];

extern void sub_8006A90(struct oam_shadow_buffer *arg0);
extern void sub_8006A48(struct oam_shadow_buffer *arg0);
extern void sub_80006A8(void);
extern void sub_8006AAC(struct oam_shadow_buffer *arg0);
extern void sub_8006AC8(struct oam_shadow_buffer *self, void *record);
extern void sub_8006A78(struct oam_shadow_buffer *arg0);
extern s32 sub_803ADB4(s32 arg0, s32 arg1);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void sub_8001614(void);
extern void sub_8001B54(struct AudioContext *self, u32 id);
extern void sub_8028A30(struct icon_manager *self, u8 val);
extern s32 sub_8026F38(s32 arg0);
extern void sub_8034688(s32 arg0);
extern void *sub_803AD7C(void *arg0, void *fn);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern void sub_80015B0(void);
extern s32 sub_8000E1C(s32 arg0);
extern void *sub_8026EC0(u32 size);
extern void sub_8026EB4(void *ptr);
extern void *sub_8026EDC(s32 size);
extern void *sub_8034374(void *arg0);
extern void LoadTaggedAsset(void *asset, void *dest);
extern void sub_8037110(void *self, void *asset, void *dest);
extern void *AllocVramTileBlock(u32 size);
extern void *mem_alloc(u32 size, u32 flags);
extern void InitObjTileFreeList(void *arg0);
extern void FreeObjTileFreeList(void);
extern void InitSpriteFrameOamQueue(void);
extern void FreeSpriteFrameOamQueue(void);
extern void FlushSpriteFrameOamQueue(void);
extern void InitSpriteFrameCache(void);
extern void FreeSpriteFrameCache(void);
extern void AgeSpriteFrameCache(void);
extern void FreeCategorySpriteSheet(void);
extern void FlushVramDmaQueue(void);
extern s32 QueueVramDmaTransfer(void *src, void *dest, u16 size, u16 unit);
extern void sub_80007AC(void *arg0);
extern void sub_8026ED0(void *self);
extern s32 sub_803AE4C(void *self, s32 arg1);
extern void sub_80346FC(void *self, s32 arg1);
extern void InitActorPart(void *self, s32 a, s32 b, s32 c, s32 d);
extern s32 GetAnimFrameBaseOffset(void *self);
extern s32 GetSpriteShapeSizeBits(void *self);
extern void QueueSpriteFrameOam(u32 attr01, u16 attr2, s32 scale);

/* libgcc helpers under this ROM's names: `sub_803ADB4` is `__divsi3`,
 * `sub_803AD80` is `_call_via_r2` (the Thumb indirect-call thunk). */
asm(".set __divsi3, sub_803ADB4");
asm(".set _call_via_r2, sub_803AD80");

/* The camera-ish object an actor part reads through `self+0x30`
 * (same shape as actor_part128.c's). */
struct cam_ref {
    u8 unk_00[0x10];
    s32 depth;      // 0x10 - the depth at which sprites draw unscaled
};

/* `gUnknown_03001288`, the REG_DISPCNT shadow `sub_8001614` commits,
 * viewed as its bitfields (field stores give the ROM's byte-wide
 * and/or sequences). */
struct dispcnt_bits
{
    u16 mode:3;
    u16 cgbMode:1;
    u16 frame:1;
    u16 hblankOam:1;
    u16 objMap1D:1;
    u16 forcedBlank:1;
    u16 bg0:1;
    u16 bg1:1;
    u16 bg2:1;
    u16 bg3:1;
    u16 obj:1;
    u16 win0:1;
    u16 win1:1;
    u16 objWin:1;
};

/* REG_BGnCNT as bitfields (same layout as `struct bg_setup`'s ctrl in
 * include/graphics_package.h). */
union bgcnt
{
    u16 raw;
    struct {
        u16 priority:2;
        u16 charBase:2;
        u16 unk_4:2;
        u16 mosaic:1;
        u16 colorMode:1;
        u16 screenBase:5;
        u16 wrap:1;
        u16 size:2;
    } bits;
};

/* One entry of the 8-byte {delta-record pointer, initial hold} seed
 * tables (`gStaticData_0817CFA4`/`gStaticData_0817D6C0`) the slot arrays
 * are initialized from. */
struct slot_seed
{
    struct delta_record *record;
    s32 hold;
};

/* `sub_803AE4C` is libgcc's `__modsi3`. */
asm(".set __modsi3, sub_803AE4C");

void sub_8035780(u32 *self);
void sub_80358A8(u32 *self);
void sub_8036068(u32 *self);
void sub_8035F9C(u32 *self);

/* Per-frame updater for the scratch object's 9-slot (`i` = 0..8,
 * stride 0x34, base `self+4`) record array: while a slot's countdown
 * word at `self+0x14+i*0x34` is nonzero, decrements it; on reaching 0,
 * walks that slot's "delta record" pointer at `self+0x40+i*0x34`
 * forward by 0x20 bytes and reloads position (`+2`/`+4`/`+6`, Q16.16
 * via `<<16`), velocity (`+8`/`+0xa`, Q24.8 via `<<8`), and 4
 * accompanying raw words (`+0xc`/`+0x10`/`+0x14`/`+0x18`/`+0x1c`) from
 * the new record's own head (`+0`, a signed 16-bit "hold" count) into
 * the slot's live fields; on a nonzero countdown, instead accumulates
 * each live position/velocity pair (`+0x18..+0x38` in
 * value/delta pairs) by its own delta once.
 *
 * This was originally NAKED (see docs/matching/issue-65-0x08035780-
 * graphics-loading.md) because the ROM keeps recomputing `self +
 * CONST + i*0x34` fresh for every single field access instead of
 * hoisting a shared `self+i*0x34` slot-base register the way any
 * plain-C phrasing (raw pointer casts included) naturally does; a bare
 * `asm volatile("" ::: "memory")` barrier didn't stop the fold either,
 * since it invalidates memory contents, not an already-computed pure-
 * address register. What closed it (see also `tile_slot_pool.c`'s
 * `PushFreeSlot`/`GetTileSlot`/`SetTileSlot` for the same idea): give
 * every field its own tiny `static inline` accessor, each its own
 * distinct call site, so this compiler's inliner treats every access
 * as a fresh expansion instead of one shared subexpression it can
 * hoist. Two more subtleties were needed on top of that for a fully
 * byte-exact match: (1) `self+CONST` has to be materialized as its own
 * named local *before* adding the `i*0x34` stride (an expression like
 * `self + CONST + stride` gets silently reassociated by this compiler
 * into `stride + CONST` first, `+ self` last - the opposite of what
 * the ROM does); and (2) a handful of individual loads (the "delta
 * record" pointer bump, and the three `<<16` position fields) needed
 * explicit register pins to land in the exact temp registers the
 * ROM's own build chose instead of whatever this compiler naturally
 * picks. */
struct delta_record
{
    s16 hold;      // 0x0 - countdown reload value
    u16 dPosA;     // 0x2 - Q16.16 position (<<16)
    u16 dPosB;     // 0x4 - Q16.16 position (<<16)
    u16 dPosC;     // 0x6 - Q16.16 position (<<16)
    s16 dVelA;     // 0x8 - Q24.8 velocity (<<8)
    s16 dVelB;     // 0xa - Q24.8 velocity (<<8)
    s32 deltaA;    // 0xc  - raw delta for dPosA's live field
    s32 deltaB;    // 0x10 - raw delta for dPosB's live field
    s32 deltaC;    // 0x14 - raw delta for dPosC's live field
    s32 deltaD;    // 0x18 - raw delta for dVelA's live field
    s32 deltaE;    // 0x1c - raw delta for dVelB's live field
};

static inline struct delta_record **RecordAt(u32 *self, s32 stride)
{
    u8 *base = (u8 *)self + 0x40;
    return (struct delta_record **)(base + stride);
}
static inline u8 *SlotBase(u32 *self, s32 stride)
{
    u8 *base = (u8 *)self;
    return base + stride;
}
static inline s32 *PosCAt(u32 *self, s32 stride)
{
    u8 *base = (u8 *)self + 0x20;
    return (s32 *)(base + stride);
}
static inline s32 *VelAAt(u32 *self, s32 stride)
{
    u8 *base = (u8 *)self + 0x24;
    return (s32 *)(base + stride);
}
static inline s32 *DeltaCAt(u32 *self, s32 stride)
{
    u8 *base = (u8 *)self + 0x34;
    return (s32 *)(base + stride);
}
static inline s32 *DeltaDAt(u32 *self, s32 stride)
{
    u8 *base = (u8 *)self + 0x38;
    return (s32 *)(base + stride);
}
static inline s32 *VelBAt(u32 *self, s32 stride)
{
    u8 *base = (u8 *)self + 0x28;
    return (s32 *)(base + stride);
}
static inline s32 *DeltaEAt(u32 *self, s32 stride)
{
    u8 *base = (u8 *)self + 0x3c;
    return (s32 *)(base + stride);
}
static inline s32 *PosAAt(u32 *self, s32 stride)
{
    u8 *base = (u8 *)self + 0x18;
    return (s32 *)(base + stride);
}
static inline s32 *DeltaAAt(u32 *self, s32 stride)
{
    u8 *base = (u8 *)self + 0x2c;
    return (s32 *)(base + stride);
}
static inline s32 *PosBAt(u32 *self, s32 stride)
{
    u8 *base = (u8 *)self + 0x1c;
    return (s32 *)(base + stride);
}
static inline s32 *DeltaBAt(u32 *self, s32 stride)
{
    u8 *base = (u8 *)self + 0x30;
    return (s32 *)(base + stride);
}

/* A 7-branch "cheat code" style detector: gated on
 * `gUnknown_030007E0.held`'s bit 0x100 (R shoulder) - if not held,
 * resets the rolling-hash slot at `self+0x210` to 0 and returns
 * `pressed` unmodified (so the caller can still act on ordinary button
 * presses). If held, folds one of 7 fixed "signature" constants
 * (selected by a bit of `pressed`: Left/Right/Up/Down/B/A/Start) into
 * `self+0x210` via the same rotate-then-multiply-by-521 hash
 * `sub_80360C0` implements standalone, then always checks whether that
 * slot now equals the fixed target `0x3034AF3B` - if so, plays song
 * `0xc` and resets the slot, consuming the input (returns 0) either
 * way once the gate was held. */
/* Closed in the issue #64/#65 NAKED retry: copying the whole
 * held/pressed pair into a local struct first is what makes the ROM
 * build the 0x100 mask in r4 and copy it to r1. */
static inline void HashInput(u32 *self, u32 val)
{
    u32 *slot = (u32 *)((u8 *)self + 0x210);
    register u32 v asm("r0") = *slot ^ val;
    register u32 hi asm("r1");
    register u32 lo asm("r0");
    register u32 rotated asm("r1");
    u32 out;

    hi = v << 1;
    lo = v >> 31;
    asm("" : "+r"(hi));
    rotated = hi | lo;
    out = (rotated << 6) + rotated;

    out = (out << 3) + rotated;
    *slot = out;
}

u32 sub_8035D1C(u32 *self, u32 pressed)
{
    struct held_pressed_pair input = gUnknown_030007E0;

    if (!(input.held & 0x100))
    {
        *(u32 *)((u8 *)self + 0x210) = 0;
        return pressed;
    }
    if (pressed & 0x20)
        HashInput(self, 0x12345678);
    else if (pressed & 0x10)
        HashInput(self, 0x31415926);
    else if (pressed & 0x40)
        HashInput(self, 0xC0DEBA1D);
    else if (pressed & 0x80)
        HashInput(self, 0xDEADBEEF);
    else if (pressed & 2)
        HashInput(self, 0xB1E4B1E4);
    else if (pressed & 1)
        HashInput(self, 0x71839406);
    else if (pressed & 8)
        HashInput(self, 0x828A048B);
    if (*(u32 *)((u8 *)self + 0x210) == 0x3034AF3B)
    {
        sub_8001B54(gUnknown_030012BC, 0xc);
        *(u32 *)((u8 *)self + 0x210) = 0;
    }
    return 0;
}

/* Drives what looks like a level-intro/tally sequence on the scratch
 * object: seeds all 9 slots' `self+0x40+i*0x34` delta-record pointers
 * and `self+0x14+i*0x34` countdowns from `gStaticData_0817CFA4`
 * (mirroring `sub_80360DC`'s init shape), then loops `sub_8035780`
 * (slot decay) + `sub_8036068` (per-frame OAM/icon flush) +
 * `sub_8034688` + `sub_80006A8` + `sub_8035F9C` (BG2 affine flush)
 * until `self+0x14` (the header's own hold record) drains to 0. Then
 * runs a second phase gated by `sub_8035D1C`'s cheat-detector return
 * value (bits 9/0x40/0x80 firing `PlaySfx` 0x49/0x46/0x46 and
 * nudging `self[0]`), followed by a fixed-length (17-frame) BG2
 * fade-out tail. Returns `self[0]`, a small state/counter field several
 * of this cluster's other functions also read. */
/* Closed in the issues #64/#65 second NAKED retry. Three loop shapes:
 * - The seed loop is a `goto` loop (nothing hoisted), as in
 *   `sub_80360DC`. Two extra `i` references (empty asms) give `i` the
 *   first free low register (r3) ahead of `slot`/`stride`. Both
 *   address sums compute the scaled index first (`off`), and the
 *   `-1` store adds it second (`base + off`).
 * - The menu loop is a real `for (;;)`, so `&gUnknown_030012BC` is
 *   hoisted into r6. Leaving it with `goto fadeLoop` instead of `break`
 *   keeps jump.c from rotating it around the `pressed & 9` exit.
 * - The fade loop is a `goto` loop again (its register addresses are
 *   reloaded each pass) with its own counter, so the seed loop's `i`
 *   does not cross calls.
 * `pressed` is loaded into its own variable first (the ROM's
 * `ldrh r5` / `add r1, r5, #0`). The `0x210` zero is a local, so it is
 * materialized before the `1`. */
s32 sub_8035E14(u32 *self)
{
    s32 i;
    struct slot_seed *seedBase;
    struct slot_seed *seed;
    u8 *slot;
    s32 stride;
    s32 zero;
    u32 pressed;
    s32 fade;

    i = 0;
    seedBase = (struct slot_seed *)gStaticData_0817CFA4;
    seed = seedBase;
    slot = (u8 *)self;
    stride = 0;
seedLoop:
    zero = 0;
    slot[0x10] = zero;
    {
        u8 *countdownBase = (u8 *)self + 0x14;
        s32 *dst = (s32 *)(countdownBase + stride);
        s32 off = i << 3;
        u32 holdBase = (u32)seedBase + 4;

        *dst = *(s32 *)(off + holdBase) + 1;
    }
    *RecordAt(self, stride) = seed->record;
    {
        s32 off = i << 2;
        u32 base = (u32)self + 0x1e4;

        *(s32 *)(base + off) = -1;
    }
    seed++;
    slot += 0x34;
    stride += 0x34;
    i++;
    asm("" : : "r"(i));
    asm("" : : "r"(i));
    if (i <= 8)
        goto seedLoop;
    *(s32 *)((u8 *)self + 0x20c) = zero;
    ((u8 *)self)[8] = zero;
    while (self[5] != 0)
    {
        sub_8035780(self);
        sub_8036068(self);
        sub_8034688(self[0x82]);
        sub_80006A8();
        *(vu32 *)REG_ADDR_BLDCNT = 0;
        sub_8035F9C(self);
    }
    {
        u32 z = 0;
        ((u8 *)self)[8] = 1;
        *(u32 *)((u8 *)self + 0x210) = z;
    }
    for (;;)
    {
        sub_8036068(self);
        sub_8034688(self[0x82]);
        sub_80007AC(gUnknown_03001304);
        pressed = gUnknown_030007E0.pressed;
        pressed = sub_8035D1C(self, pressed);
        if (pressed & 9)
        {
            PlaySfx(gUnknown_030012BC, 0x49, 0x100);
            fade = 0;
            goto fadeLoop;
        }
        if (pressed & 0x40)
        {
            PlaySfx(gUnknown_030012BC, 0x46, 0x100);
            if (self[0] != 0)
                self[0]--;
            else
                self[0] = 2;
        }
        if (pressed & 0x80)
        {
            PlaySfx(gUnknown_030012BC, 0x46, 0x100);
            self[0]++;
            self[0] = (s32)self[0] % 3;
        }
        sub_80006A8();
        sub_8035F9C(self);
    }
fadeLoop:
    sub_8036068(self);
    sub_8034688(self[0x82]);
    sub_80006A8();
    REG_BLDY = fade;
    REG_BLDCNT = 0xff;
    sub_8035F9C(self);
    fade++;
    if (fade <= 0x10)
        goto fadeLoop;
    return self[0];
}

static inline void SetIconPos(struct icon_manager *m, u32 x, u32 y)
{
    m->posX = x;
    m->posY = y;
}

/* Flushes the scratch object's BG2 affine-scroll fields
 * (`self+0x214`/`self+0x218` position, `self+0x21c` scale) to
 * `REG_BG2X`/`REG_BG2Y`/`REG_BG2PA`/`REG_BG2PD` (identity-shaped:
 * `PB`/`PC` cleared, `PA` == `PD`), then flushes the pending shadow-OAM
 * buffer (`sub_8001614` + `sub_8006AAC`). The ROM derives
 * `REG_BG2PA`'s address from `REG_BG2Y`'s (`-0xc`); that falls out of
 * writing the PA store as a chained assignment (`REG_BG2PA = scale =
 * ...`), which makes the address get computed before the load. */
void sub_8035F9C(u32 *self)
{
    s32 scale;

    REG_BG2X = self[0x85];
    REG_BG2Y = self[0x86];
    REG_BG2PA = scale = self[0x87];
    REG_BG2PB = 0;
    REG_BG2PC = 0;
    REG_BG2PD = scale;
    sub_8001614();
    sub_8006AAC(gUnknown_03001300);
}

/* Icon-manager position helper: for `variant == 0`, bumps a play
 * counter at `self+4` and alternates `sub_8028A30`'s icon-slot id
 * between 0xe/0xf every other call; for `variant != 0` (only ever
 * called with 1/2 by `sub_8036068`), always uses id 0xd. Either way,
 * feeds the resulting icon-manager slot's own position fields (looked
 * up at `self+0xc` + a fixed table offset) through `sub_803AD80` twice
 * (once for the icon at its own position, once for a second icon
 * `0xf0` px to its right), positioning them from the icon-manager's own
 * anchor record. The two `sub_803AD80` calls are `_call_via_r2`
 * virtual calls through the icon manager's `record->slots[0]`/`[2]`
 * entries (same shape as `actor_part_1b85c.c`). */

void sub_8035FEC(u32 *self, s32 text, s32 variant)
{
    struct icon_manager *im;
    struct icon_slot *slot;
    s32 x;

    if (variant == self[0])
    {
        s32 count = self[1] + 1;
        self[1] = count;
        sub_8028A30((struct icon_manager *)self[3], ((count >> 2) & 1) + 0xe);
    }
    else
    {
        sub_8028A30((struct icon_manager *)self[3], 0xd);
    }
    slot = &((struct icon_manager *)self[3])->record->slots[0];
    x = (0xf0 - sub_803AD80((u8 *)self[3] + slot->offset, (void *)text, slot->ptr)) >> 1;
    im = (struct icon_manager *)self[3];
    SetIconPos(im, x, variant * 10 + 0x80);
    slot = &im->record->slots[2];
    sub_803AD80((u8 *)im + slot->offset, (void *)text, slot->ptr);
}

/* Per-frame flush helper: resets the shadow-OAM buffer
 * (`sub_8006A90`), runs `sub_80358A8` (the header/slot OAM builder),
 * and - only while the scratch object's `self+8` hold flag is set -
 * positions three icon-manager slots (ids 0x1a/0x1b/0x3b, one call
 * each via `sub_8035FEC`) before releasing the shadow-OAM buffer
 * (`sub_8006A48`). */
void sub_8036068(u32 *self)
{
    sub_8006A90(gUnknown_03001300);
    sub_80358A8(self);
    if (((u8 *)self)[8] != 0)
    {
        sub_8035FEC(self, sub_8026F38(0x1a), 0);
        sub_8035FEC(self, sub_8026F38(0x1b), 1);
        sub_8035FEC(self, sub_8026F38(0x3b), 2);
    }
    sub_8006A48(gUnknown_03001300);
}

/* Standalone one-shot rolling-hash update: XORs `val` into the same
 * `self+0x210` "cheat code" slot `sub_8035D1C` inlines, rotates the
 * result left by 1 bit, then multiplies by 521 (`(x<<6)+x`, `<<3`,
 * `+x` - `x*65*8+x`) and stores it back. Matched as real C: the
 * rotate has to be split into an explicit shift-left/shift-right pair
 * with both intermediates register-pinned (`hi`/`lo` to `r3`/`r2`,
 * matching the ROM's own choice) rather than written as the natural
 * `(v << 1) | (v >> 31)` idiom, which this compiler's optimizer folds
 * into a single Thumb `ROR` instruction the ROM's own compiled output
 * never emits (it always expands the rotate into the explicit
 * shift-shift-or sequence, at least in this ROM's own build). */
void sub_80360C0(u32 *selfArg, u32 val)
{
    u8 *self = (u8 *)selfArg;
    u32 *slot = (u32 *)(self + 0x210);
    register u32 v asm("r2") = *slot ^ val;
    register u32 hi asm("r3");
    register u32 lo asm("r2");
    register u32 rotated asm("r3");
    u32 out;

    hi = v << 1;
    lo = v >> 31;
    asm("" : "+r"(hi));
    rotated = hi | lo;
    out = (rotated << 6) + rotated;
    out = (out << 3) + rotated;
    *slot = out;
}

/* `sub_8035780`'s init-time twin: seeds all 9 slots' countdown words
 * (`self+0x14+i*0x34`, from `gStaticData_0817CFA4`'s own `+4+i*8`
 * table, `+1`) and delta-record pointers (`self+0x40+i*0x34`, from the
 * same table's `+i*8` head) in one pass, and unconditionally stores
 * -1 into each slot's `self+0xf2*2+i*4` play-counter word (matching
 * `sub_8035780`'s own countdown-reset shape, just via `stm` instead of
 * a plain store - the same operation, a different ROM-side register
 * allocation). Clears the header hold flag (`self+0x20c`). */
/* Closed in the issues #64/#65 second NAKED retry. The loop is a
 * hand-written `goto` loop (no loop notes), so nothing is hoisted, as in
 * the ROM. The rest is global-alloc priority:
 * - `stride` starts from an empty asm (`"=r"(stride) : "0"(0)`). A plain
 *   `stride = 0` makes local-alloc double its live length, which drops
 *   it below `slot` (r5/r4 swapped).
 * - One extra `self` reference in the loop and `seedBase`/`zero`
 *   references after it (empty asms, no code) lift those three to the
 *   ROM's r3/r8/ip.
 * - `off = i << 3` computed before `holdBase` (as an integer) gives the
 *   ROM's `lsl` first, `add r0, r0, r1` order. */
void sub_80360DC(u32 *self)
{
    s32 i;
    struct slot_seed *seedBase;
    s32 *counter;
    struct slot_seed *seed;
    u8 *slot;
    s32 stride;
    s32 zero;

    i = 0;
    seedBase = (struct slot_seed *)gStaticData_0817CFA4;
    counter = (s32 *)((u8 *)self + 0x1e4);
    seed = seedBase;
    slot = (u8 *)self;
    asm("" : "=r"(stride) : "0"(0));
loop:
    zero = 0;
    slot[0x10] = zero;
    {
        u8 *countdownBase = (u8 *)self + 0x14;
        s32 *dst = (s32 *)(countdownBase + stride);
        s32 off = i << 3;
        u32 holdBase = (u32)seedBase + 4;

        *dst = *(s32 *)(off + holdBase) + 1;
    }
    *RecordAt(self, stride) = seed->record;
    *counter++ = -1;
    seed++;
    slot += 0x34;
    stride += 0x34;
    i++;
    asm("" : : "r"(self));
    if (i <= 8)
        goto loop;
    asm("" : : "r"(seedBase));
    asm("" : : "r"(zero));
    *(s32 *)((u8 *)self + 0x20c) = zero;
}

/* Teardown/reset helper: if the object at `self+0x208` exists, destroys
 * it (`sub_80346FC(obj, 3)`); clears the DISPCNT shadow
 * (`gUnknown_03001288`) and commits it (`sub_8001614`), zeroes all 256
 * BG palette entries (`0x05000000`), sets REG_BLDCNT/REG_BLDY to a full
 * fade (0xff/0x10), and - only if `flag`'s bit 0 is set - frees the
 * scratch object via `sub_8026ED0`. The palette clear needs its zero
 * in a local assigned before the pointer (the ROM materializes it
 * first). */
void sub_8036154(u32 *self, u32 flag)
{
    s32 i;
    u16 *pal;
    s32 zero;

    if (self[0x82] != 0)
        sub_80346FC((void *)self[0x82], 3);
    *(u16 *)gUnknown_03001288 = 0;
    sub_8001614();
    zero = 0;
    pal = (u16 *)0x05000000;
    for (i = 0xff; i >= 0; i--)
        *pal++ = zero;
    REG_BLDCNT = 0xff;
    REG_BLDY = 0x10;
    if (flag & 1)
        sub_8026ED0(self);
}

/* `sub_803AD7C` is the Thumb `_call_via_r1` thunk. */
asm(".set _call_via_r1, sub_803AD7C");

/* The part's method table as `sub_80361B0` uses it (gcc 2.x C++
 * {this-adjust, fn} records). */
struct part_vtable
{
    u8 unk_00[8];
    struct actor_method m08;    // 0x08 - destroy (arg 3)
    struct actor_method m10;    // 0x10
    struct actor_method m18;    // 0x18
};

void sub_8036CF4(u32 *self);
struct actor_self *sub_8036E20(struct actor_self *self, void *a);
void sub_8036528(u32 *self);
void sub_8036600(u32 *self);
void sub_8036668(u32 *self);
void sub_803686C(u32 *self);

/* `operator new`: an inline wrapper puts the size constant after the
 * heap flag, as the ROM loads them. */
static inline void *New(u32 size)
{
    return mem_alloc(size, 0x80000000);
}

/* The level-object subsystem's own init/run/teardown driver: sets up
 * the OBJ-tile free list, sprite-frame OAM queue and sprite-frame
 * cache, constructs the actor part (`sub_8036E20` on a 0x54-byte
 * `mem_alloc` block), DMAs the OBJ palette, loads BG2's tilesets and
 * the slot array (`sub_8036528`/`sub_8036600`), builds the particle
 * background (`sub_8034374`) and BG2's tilemap (`sub_8036CF4`). Then
 * a 60-frame fade-in, a zoom-in phase (BG2 affine scale driven by the
 * `self+0x444` counter, A/Start skips ahead), and the main phase
 * (the part's two per-frame methods, the slot-array update and OAM
 * build, a blend fade-out once `self+0x444` is set) until the counter
 * reaches 0; finally tears everything down again. Real C under
 * old_agbcc: the zoom-in's decrement/grow/shrink is written as "step
 * the counter, then test it again" (which gives the ROM's block order),
 * the affine X/Y values are computed before either register store, and
 * the fade-out value is pinned to r1 (the ROM's choice; without the pin
 * it lands in r2 and costs a copy). */
void sub_80361B0(u32 *self)
{
    struct actor_self *part;
    void *bgObj;
    s32 i;
    s32 scale;

    InitObjTileFreeList((void *)0x06010000);
    InitSpriteFrameOamQueue();
    InitSpriteFrameCache();
    part = sub_8036E20(New(0x54), gStaticData_0817D698);
    {
        struct dma_regs *dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
        dma->src = (u32)gStaticData_08178F80;
        dma->dst = 0x05000200;
        dma->cnt = 0x80000100;
        dma->cnt;
    }
    sub_8036528(self);
    sub_8036600(self);
    bgObj = sub_8034374(sub_8026EDC(0x14));
    sub_8036CF4(self);
    for (i = 0; i <= 0x3b; i++)
    {
        if (i <= 0x10)
        {
            REG_BLDCNT = 0xff;
            REG_BLDY = 0x10 - i;
        }
        else
        {
            *(vu32 *)REG_ADDR_BLDCNT = 0;
        }
        sub_80006A8();
        sub_8034688((s32)bgObj);
    }
    PlaySfx(gUnknown_030012BC, 0x4b, 0x100);
    scale = 0x2000;
    *(s32 *)((u8 *)self + 0x444) = -1;
    do
    {
        s32 *fade;
        s32 v;
        s32 q;

        sub_80007AC(gUnknown_03001304);
        if (gUnknown_030007E0.pressed & 9)
        {
            if (*(s32 *)((u8 *)self + 0x444) > 0x40)
                *(s32 *)((u8 *)self + 0x444) = 0x40;
        }
        sub_80006A8();
        sub_8001614();
        fade = (s32 *)((u8 *)self + 0x444);
        if (*fade != -1)
        {
            if (*fade == 0x40)
                PlaySfx(gUnknown_030012BC, 0x4c, 0x100);
            v = *fade;
            if (v <= 0x40)
            {
                s32 a = v >> 2;
                REG_BLDCNT = 0x3f7f;
                REG_BLDALPHA = a | ((0x10 - a) << 8);
            }
            *fade = v - 1;
        }
        if (*fade == -1)
        {
            if (scale > 0xffff || (scale += 0x600) > 0xffff)
            {
                scale = 0x10000;
                if (*(s32 *)((u8 *)self + 0x444) == -1)
                    *(s32 *)((u8 *)self + 0x444) = 0xf4;
            }
        }
        else if (*fade <= 0x40)
        {
            scale = (scale * 0x118) >> 8;
        }
        q = 0x1000000 / scale;
        {
            s32 x = -(q * 120) + 0x7800;
            s32 y = -(q * 80) + 0x5000;
            REG_BG2X = x;
            REG_BG2Y = y;
        }
        REG_BG2PA = q;
        REG_BG2PD = q;
        REG_BG2PB = 0;
        REG_BG2PC = 0;
        sub_8034688((s32)bgObj);
    } while (*(s32 *)((u8 *)self + 0x444) != 0);
    ((struct dispcnt_bits *)gUnknown_03001288)->bg2 = 0;
    ((struct dispcnt_bits *)gUnknown_03001288)->obj = 1;
    ((struct dispcnt_bits *)gUnknown_03001288)->objMap1D = 1;
    sub_8001614();
    *(vu32 *)REG_ADDR_BLDCNT = 0;
    *(s32 *)((u8 *)self + 0x444) = -1;
    *(s32 *)((u8 *)self + 0x448) = -1;
    while (*(s32 *)((u8 *)self + 0x444) != 0)
    {
        s32 *fade;
        register s32 v asm("r1");

        sub_80007AC(gUnknown_03001304);
        if (gUnknown_030007E0.pressed & 9)
        {
            if (*(s32 *)((u8 *)self + 0x448) > 0)
                *(s32 *)((u8 *)self + 0x448) = 1;
        }
        {
            struct part_vtable *vt = (struct part_vtable *)part->vtable;
            ((void (*)(void *))vt->m10.fn)((u8 *)part + vt->m10.thisOffset);
        }
        {
            struct part_vtable *vt = (struct part_vtable *)part->vtable;
            ((void (*)(void *))vt->m18.fn)((u8 *)part + vt->m18.thisOffset);
        }
        sub_8006A78(gUnknown_03001300);
        FlushSpriteFrameOamQueue();
        sub_8036668(self);
        sub_803686C(self);
        sub_8034688((s32)bgObj);
        sub_80006A8();
        fade = (s32 *)((u8 *)self + 0x444);
        v = *fade;
        if (v > 0x10)
        {
            s32 n = v - 1;
            s32 a;

            *fade = n;
            a = v - 0x12;
            REG_BLDCNT = 0x3f7f;
            REG_BLDALPHA = (0x10 - a) | (a << 8);
            if (n == 0x11)
            {
                *fade = -1;
                *(vu32 *)REG_ADDR_BLDCNT = 0;
            }
        }
        else if (v >= 0)
        {
            v--;
            *fade = v;
            REG_BLDY = 0x10 - v;
            REG_BLDCNT = 0xff;
        }
        sub_8006AAC(gUnknown_03001300);
        FlushVramDmaQueue();
        AgeSpriteFrameCache();
    }
    if (part != NULL)
    {
        struct part_vtable *vt = (struct part_vtable *)part->vtable;
        ((void (*)(void *, s32))vt->m08.fn)((u8 *)part + vt->m08.thisOffset, 3);
    }
    if (bgObj != NULL)
        sub_80346FC(bgObj, 3);
    if (*(void **)((u8 *)self + 0x434) != NULL)
        sub_8026EB4(*(void **)((u8 *)self + 0x434));
    if (*(void **)((u8 *)self + 0x430) != NULL)
        sub_8026EB4(*(void **)((u8 *)self + 0x430));
    FreeSpriteFrameCache();
    FreeSpriteFrameOamQueue();
    FreeObjTileFreeList();
    FreeCategorySpriteSheet();
}

/* BG2's own tileset/palette loader for this subsystem: allocates 3
 * VRAM tile blocks (`self+0x424`/`0x428`/`0x42c`), loads 3 palette
 * banks (`gStaticData_0817D768`/`_77c`/`_790`'s packages) to
 * `0x050003E0`/`_C0`/`_A0` and two packages' tile data (`sub_8037110`)
 * into the first two blocks, then allocates a buffer sized from the
 * first package's tile-asset header (`self+0x430`) and unpacks into it,
 * plus a 0x1000-byte scratch buffer (`self+0x434`). The last two
 * stores go through a destination pointer taken before the allocation
 * call, as the ROM computes the address first. */
#define PKG_A ((struct bg_package *)gStaticData_0817D768)
#define PKG_B ((struct bg_package *)gStaticData_0817D77C)
#define PKG_C ((struct bg_package *)gStaticData_0817D790)

void sub_8036528(u32 *self)
{
    self[0x109] = (u32)AllocVramTileBlock(0x1200);
    self[0x10a] = (u32)AllocVramTileBlock(0x400);
    self[0x10b] = (u32)AllocVramTileBlock(0x1000);
    sub_8037110(self, PKG_A->paletteAsset, (void *)0x050003E0);
    sub_8037110(self, PKG_B->paletteAsset, (void *)0x050003C0);
    sub_8037110(self, PKG_C->paletteAsset, (void *)0x050003A0);
    sub_8037110(self, PKG_B->tileAsset, (void *)self[0x109]);
    sub_8037110(self, PKG_C->tileAsset, (void *)self[0x10a]);
    {
        u32 size = *(u32 *)PKG_A->tileAsset >> 8;
        u32 *dst = &self[0x10c];
        void *buf;

        *dst = (u32)(buf = sub_8026EC0(size));
        LoadTaggedAsset(PKG_A->tileAsset, buf);
    }
    {
        u32 *dst = &self[0x10d];
        *dst = (u32)sub_8026EC0(0x1000);
    }
}

/* Slot-array field initializer: for all 20 (`0x13`+1) slots, clears
 * the active-flag byte (`self+i*0x34`), sets the play-counter word
 * (`self+4+i*0x34`) from `gStaticData_0817D6C0`'s own `+4+i*8` field
 * `+1`, and the delta-record pointer (`self+0x2c+i*0x34`) from that
 * same table's `+i*8` head. Also clears 3 header fields
 * (`self+0x438`/`self+0x43c`/`self+0x440`). */
/* Matches only because this object is built with -fno-strength-reduce
 * (see NO_STRENGTH_REDUCE_OBJS in the Makefile and
 * docs/matching/per-file-flags-investigation.md): with strength
 * reduction on, gcc's loop optimizer reverses the first loop into a
 * count-down (its counter is only used by the exit test) while the ROM
 * keeps `i` counting up. The pointer walks are the source's own - with
 * strength reduction off nothing would have produced them. The second
 * loop's `1` lives in a local assigned before its counter and pointer
 * (the ROM materializes it first). */
void sub_8036600(u32 *self)
{
    s32 i;
    u8 zero;
    struct slot_seed *seed;
    u8 *active;
    s32 *hold;
    s32 *countdown;

    i = 0;
    zero = 0;
    seed = (struct slot_seed *)gStaticData_0817D6C0;
    active = (u8 *)self;
    hold = &seed->hold;
    countdown = (s32 *)((u8 *)self + 4);
    for (; i <= 0x13; i++)
    {
        *active = zero;
        *countdown = *hold + 1;
        *(struct delta_record **)((u8 *)countdown + 0x2c) = seed->record;
        seed++;
        active += 0x34;
        countdown = (s32 *)((u8 *)countdown + 0x34);
        hold += 2;
    }
    {
        u8 one = 1;
        s32 j = 0x11;
        u8 *flags = (u8 *)self + 0x421;

        for (; j >= 0; j--)
            *flags-- = one;
    }
    *(s32 *)((u8 *)self + 0x438) = 0;
    *(s32 *)((u8 *)self + 0x43c) = 0;
    *(s32 *)((u8 *)self + 0x440) = 0;
}

/* `sub_8036528`'s per-frame slot-array updater, only while the header
 * hold-word (`self+0x448`) equals -1: for each of 20 slots, mirrors
 * `sub_8035780`'s own countdown/reload/accumulate shape (offsets `+0`
 * through `+0x38` this time, no `+0x14` base shift), then, once the
 * whole pass completes, drains a 2-stage decay counter
 * (`self+0x440`/`self+0x438`) that eventually bumps
 * `self+0x43c` and, separately, ages every active slot's own
 * `self+i*0x34+8` sub-timer, resetting `self+0x444`/`self+0x448` if any
 * slot's timer crosses its ceiling. */
/* Closed in the issue #64/#65 NAKED retry: the drain loop's end pointer
 * is its own local set before the do-while (so it is computed ahead of
 * the two constants loop.c hoists), which also settles the tail's
 * 0x444/0x448 registers. */
#define SLOT20_ACCESSOR(name, type, off)                   \
    static inline type *name(u32 *self, s32 stride)        \
    {                                                      \
        u8 *base = (u8 *)self + (off);                     \
        return (type *)(base + stride);                    \
    }

SLOT20_ACCESSOR(Rec20At, struct delta_record *, 0x30)
SLOT20_ACCESSOR(PosC20At, s32, 0x10)
SLOT20_ACCESSOR(DeltaC20At, s32, 0x24)
SLOT20_ACCESSOR(VelA20At, s32, 0x14)
SLOT20_ACCESSOR(DeltaD20At, s32, 0x28)
SLOT20_ACCESSOR(VelB20At, s32, 0x18)
SLOT20_ACCESSOR(DeltaE20At, s32, 0x2c)
SLOT20_ACCESSOR(PosA20At, s32, 0x08)
SLOT20_ACCESSOR(DeltaA20At, s32, 0x1c)
SLOT20_ACCESSOR(PosB20At, s32, 0x0c)
SLOT20_ACCESSOR(DeltaB20At, s32, 0x20)

void sub_8036668(u32 *self)
{
    s32 i;
    s32 *timer;

    if (*(s32 *)((u8 *)self + 0x448) == -1)
    {
        for (i = 0; i <= 0x13; i++)
        {
            s32 stride = i * 0x34;
            u8 *countdownBase = (u8 *)self + 4;
            s32 *countdownPtr = (s32 *)(countdownBase + stride);

            if (*countdownPtr != 0)
            {
                s32 countdown = *countdownPtr - 1;
                *countdownPtr = countdown;
                if (countdown == 0)
                {
                    struct delta_record **recordPtrAddr = Rec20At(self, stride);
                    register struct delta_record *recordLoaded asm("r0") = *recordPtrAddr;
                    struct delta_record *record = recordLoaded;

                    *recordPtrAddr = (struct delta_record *)((u8 *)recordLoaded + 0x20);
                    SlotBase(self, stride)[0] = 1;
                    {
                        s32 hold = record->hold;
                        *countdownPtr = hold;
                        if (hold != 0)
                        {
                            *PosC20At(self, stride) = record->dPosC << 16;
                            *DeltaC20At(self, stride) = record->deltaC;
                            *VelA20At(self, stride) = record->dVelA << 8;
                            *DeltaD20At(self, stride) = record->deltaD;
                            *VelB20At(self, stride) = record->dVelB << 8;
                            *DeltaE20At(self, stride) = record->deltaE;
                            *PosA20At(self, stride) = record->dPosA << 16;
                            *DeltaA20At(self, stride) = record->deltaA;
                            *PosB20At(self, stride) = record->dPosB << 16;
                            *DeltaB20At(self, stride) = record->deltaB;
                        }
                    }
                }
                else
                {
                    *PosC20At(self, stride) += *DeltaC20At(self, stride);
                    *VelA20At(self, stride) += *DeltaD20At(self, stride);
                    *VelB20At(self, stride) += *DeltaE20At(self, stride);
                    *PosA20At(self, stride) += *DeltaA20At(self, stride);
                    *PosB20At(self, stride) += *DeltaB20At(self, stride);
                }
            }
            else
            {
                *(s32 *)((u8 *)self + 0x448) = -2;
            }
        }
        {
            s32 *stage = (s32 *)((u8 *)self + 0x43c);
            if (*stage <= 1)
            {
                s32 *sub = (s32 *)((u8 *)self + 0x440);
                if (++*sub > 3)
                {
                    *sub = 0;
                    sub = (s32 *)((u8 *)self + 0x438);
                    if (++*sub > 9)
                    {
                        *sub = 0;
                        ++*stage;
                    }
                }
            }
        }
    }
    timer = (s32 *)((u8 *)self + 0x448);
    if (*timer == -2)
        *timer = 0xf0;
    if (*timer > 0)
    {
        if (--*timer != 0)
            return;
        PlaySfx(gUnknown_030012BC, 0x50, 0x100);
    }
    if (*timer == 0)
    {
        s32 allDone = 1;
        u8 *slot;
        u8 *end;

        slot = (u8 *)self;
        end = (u8 *)self + 0x3dc;
        do
        {
            if (*slot != 0)
            {
                s32 y;

                allDone = 0;
                y = *(s32 *)(slot + 8) - 0x80000;
                *(s32 *)(slot + 8) = y;
                if (y < -0x7f0000)
                    *slot = allDone;
            }
            slot += 0x34;
        } while ((s32)slot <= (s32)end);
        if (allDone)
        {
            *(s32 *)((u8 *)self + 0x444) = 0x10;
            *(s32 *)((u8 *)self + 0x448) = -3;
        }
    }
}
