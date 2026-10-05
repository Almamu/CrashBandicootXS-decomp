#include "core.h"
#include "gba/io_reg.h"
#include "bitmap_font.h"
#include "actor_self.h"
#include "gba/dma_macros.h"
#include "graphics_package.h"
#include "logo_screen.h"

/* Middle part of GitHub issue #65's chunk (0x08035D1C-0x0803686C), split
 * off `title_screen_init.c` at `TitleScreenCheatInput` in the issues #64/#65
 * second NAKED retry. Both halves are old_agbcc code (see the Makefile's
 * OLD_AGBCC_OBJS), but only this one is built with -fno-strength-reduce
 * (NO_STRENGTH_REDUCE_OBJS): `InitVvLogoPieces` keeps its up-counting loop
 * only without strength reduction, while `DrawTitleLogoPieces` (in the first
 * half) only matches with it (its inner loop is written up-counting and
 * strength reduction reverses it). The shared declarations below are
 * copied from the first half. Everything from `DrawVvLogoPieces` on lives in
 * `company_logos.c`, which needs strength reduction on. See
 * docs/matching/issue-64-65-naked-retry-2.md and
 * docs/matching/sr65-naked-retry.md. */

extern struct oam_shadow_buffer *gOamBuffer;
extern struct AudioContext *gAudioContext;
extern u8 gDispcnt[2];
extern void *gInput;
extern void *gLogoActorTiles[2];
extern s32 gLogoActorTileBuffer;
extern void *gLogoActorLastFrame;
extern void (*gUnpackRleSpriteFrameFunc)(void *dst, u8 *frame);
extern struct held_pressed_pair {
    u16 held;
    u16 pressed;
} gKeys;

extern u8 gTitleLogoPieceSeeds[];
extern u8 gTitleArrowPieceOffsets[];
extern u8 gLogoActorAnim[];
extern u8 gPolarCategoryPalette[];
extern u8 gVvLogoEmblemObj[];
extern u8 gVvLogoLettersObj[];
extern u8 gVvLogoUrlObj[];
extern u8 gVvLogoPieceSeeds[];
extern u8 gUniversalLogoBg[];
extern u8 gLogoActorVtable[];

extern void ResetOamBuffer(struct oam_shadow_buffer *arg0);
extern void HideUnusedOamEntries(struct oam_shadow_buffer *arg0);
extern void WaitForVBlank(void);
extern void CommitOamBuffer(struct oam_shadow_buffer *arg0);
extern void AddOamEntry(struct oam_shadow_buffer *self, void *record);
extern void RewindOamBuffer(struct oam_shadow_buffer *arg0);
extern s32 __divsi3(s32 arg0, s32 arg1);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void CommitDispcnt(void);
extern void PlaySong(struct AudioContext *self, u32 id);
extern void FontSetPalette(struct bitmap_font *self, u8 val);
extern s32 GetUiText(s32 arg0);
extern void UpdateStarfield(s32 arg0);
extern void *_call_via_r1(void *arg0, void *fn);
extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern void ShowBg2(void);
extern s32 RandRange(s32 arg0);
extern void *OperatorNewArray(u32 size);
extern void OperatorDeleteArray(void *ptr);
extern void *OperatorNew(s32 size);
extern void *InitStarfield(void *arg0);
extern void LoadTaggedAsset(void *asset, void *dest);
extern void LoadTaggedAssetBuffered(void *self, void *asset, void *dest);
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
extern void UpdateKeys(void *arg0);
extern void OperatorDelete(void *self);
extern s32 __modsi3(void *self, s32 arg1);
extern void DestroyStarfield(void *self, s32 arg1);
extern void InitActorPart(void *self, s32 a, s32 b, s32 c, s32 d);
extern s32 GetAnimFrameBaseOffset(void *self);
extern s32 GetSpriteShapeSizeBits(void *self);
extern void QueueSpriteFrameOam(u32 attr01, u16 attr2, s32 scale);

/* The camera-ish object an actor part reads through `self+0x30`
 * (same shape as actor_part128.c's). */
struct cam_ref {
    u8 unk_00[0x10];
    s32 depth;      // 0x10 - the depth at which sprites draw unscaled
};

/* `gDispcnt`, the REG_DISPCNT shadow `CommitDispcnt` commits,
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
 * tables (`gTitleLogoPieceSeeds`/`gVvLogoPieceSeeds`) the slot arrays
 * are initialized from. */
struct slot_seed
{
    struct delta_record *record;
    s32 hold;
};

void UpdateTitleLogoPieces(u32 *self);
void DrawTitleLogoPieces(u32 *self);
void DrawTitleScreen(u32 *self);
void CommitTitleScreenFrame(u32 *self);

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
 * `gKeys.held`'s bit 0x100 (R shoulder) - if not held,
 * resets the rolling-hash slot at `self+0x210` to 0 and returns
 * `pressed` unmodified (so the caller can still act on ordinary button
 * presses). If held, folds one of 7 fixed "signature" constants
 * (selected by a bit of `pressed`: Left/Right/Up/Down/B/A/Start) into
 * `self+0x210` via the same rotate-then-multiply-by-521 hash
 * `HashTitleCheatInput` implements standalone, then always checks whether that
 * slot now equals the fixed target `0x3034AF3B` - if so, plays song
 * `0xc` and resets the slot, consuming the input (returns 0) either
 * way once the gate was held. */
/* Closed in the issue #64/#65 NAKED retry: copying the whole
 * held/pressed pair into a local struct first is what makes the ROM
 * build the 0x100 mask in r4 and copy it to r1. */
/* The title-screen object's cheat-code hash word (see
 * title_screen_init.c for the rest of that raw `u32 *` object). */
struct level_scratch {
    u8 unk_000[0x210];
    u32 cheatHash;      // 0x210
};

static inline void HashInput(u32 *self, u32 val)
{
    u32 *slot = &((struct level_scratch *)self)->cheatHash;
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

u32 TitleScreenCheatInput(u32 *self, u32 pressed)
{
    struct held_pressed_pair input = gKeys;

    if (!(input.held & 0x100))
    {
        ((struct level_scratch *)self)->cheatHash = 0;
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
    if (((struct level_scratch *)self)->cheatHash == 0x3034AF3B)
    {
        PlaySong(gAudioContext, 0xc);
        ((struct level_scratch *)self)->cheatHash = 0;
    }
    return 0;
}

/* Drives what looks like a level-intro/tally sequence on the scratch
 * object: seeds all 9 slots' `self+0x40+i*0x34` delta-record pointers
 * and `self+0x14+i*0x34` countdowns from `gTitleLogoPieceSeeds`
 * (mirroring `ResetTitleLogoPieces`'s init shape), then loops `UpdateTitleLogoPieces`
 * (slot decay) + `DrawTitleScreen` (per-frame OAM/icon flush) +
 * `UpdateStarfield` + `WaitForVBlank` + `CommitTitleScreenFrame` (BG2 affine flush)
 * until `self+0x14` (the header's own hold record) drains to 0. Then
 * runs a second phase gated by `TitleScreenCheatInput`'s cheat-detector return
 * value (bits 9/0x40/0x80 firing `PlaySfx` 0x49/0x46/0x46 and
 * nudging `self[0]`), followed by a fixed-length (17-frame) BG2
 * fade-out tail. Returns `self[0]`, a small state/counter field several
 * of this cluster's other functions also read. */
/* Closed in the issues #64/#65 second NAKED retry. Three loop shapes:
 * - The seed loop is a `goto` loop (nothing hoisted), as in
 *   `ResetTitleLogoPieces`. Two extra `i` references (empty asms) give `i` the
 *   first free low register (r3) ahead of `slot`/`stride`. Both
 *   address sums compute the scaled index first (`off`), and the
 *   `-1` store adds it second (`base + off`).
 * - The menu loop is a real `for (;;)`, so `&gAudioContext` is
 *   hoisted into r6. Leaving it with `goto fadeLoop` instead of `break`
 *   keeps jump.c from rotating it around the `pressed & 9` exit.
 * - The fade loop is a `goto` loop again (its register addresses are
 *   reloaded each pass) with its own counter, so the seed loop's `i`
 *   does not cross calls.
 * `pressed` is loaded into its own variable first (the ROM's
 * `ldrh r5` / `add r1, r5, #0`). The `0x210` zero is a local, so it is
 * materialized before the `1`. */
s32 RunTitleScreen(u32 *self)
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
    seedBase = (struct slot_seed *)gTitleLogoPieceSeeds;
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
        UpdateTitleLogoPieces(self);
        DrawTitleScreen(self);
        UpdateStarfield(self[0x82]);
        WaitForVBlank();
        *(vu32 *)REG_ADDR_BLDCNT = 0;
        CommitTitleScreenFrame(self);
    }
    {
        u32 z = 0;
        ((u8 *)self)[8] = 1;
        *(u32 *)((u8 *)self + 0x210) = z;
    }
    for (;;)
    {
        DrawTitleScreen(self);
        UpdateStarfield(self[0x82]);
        UpdateKeys(gInput);
        pressed = gKeys.pressed;
        pressed = TitleScreenCheatInput(self, pressed);
        if (pressed & 9)
        {
            PlaySfx(gAudioContext, 0x49, 0x100);
            fade = 0;
            goto fadeLoop;
        }
        if (pressed & 0x40)
        {
            PlaySfx(gAudioContext, 0x46, 0x100);
            if (self[0] != 0)
                self[0]--;
            else
                self[0] = 2;
        }
        if (pressed & 0x80)
        {
            PlaySfx(gAudioContext, 0x46, 0x100);
            self[0]++;
            self[0] = (s32)self[0] % 3;
        }
        WaitForVBlank();
        CommitTitleScreenFrame(self);
    }
fadeLoop:
    DrawTitleScreen(self);
    UpdateStarfield(self[0x82]);
    WaitForVBlank();
    REG_BLDY = fade;
    REG_BLDCNT = 0xff;
    CommitTitleScreenFrame(self);
    fade++;
    if (fade <= 0x10)
        goto fadeLoop;
    return self[0];
}

static inline void SetIconPos(struct bitmap_font *m, u32 x, u32 y)
{
    m->posX = x;
    m->posY = y;
}

/* Flushes the scratch object's BG2 affine-scroll fields
 * (`self+0x214`/`self+0x218` position, `self+0x21c` scale) to
 * `REG_BG2X`/`REG_BG2Y`/`REG_BG2PA`/`REG_BG2PD` (identity-shaped:
 * `PB`/`PC` cleared, `PA` == `PD`), then flushes the pending shadow-OAM
 * buffer (`CommitDispcnt` + `CommitOamBuffer`). The ROM derives
 * `REG_BG2PA`'s address from `REG_BG2Y`'s (`-0xc`); that falls out of
 * writing the PA store as a chained assignment (`REG_BG2PA = scale =
 * ...`), which makes the address get computed before the load. */
void CommitTitleScreenFrame(u32 *self)
{
    s32 scale;

    REG_BG2X = self[0x85];
    REG_BG2Y = self[0x86];
    REG_BG2PA = scale = self[0x87];
    REG_BG2PB = 0;
    REG_BG2PC = 0;
    REG_BG2PD = scale;
    CommitDispcnt();
    CommitOamBuffer(gOamBuffer);
}

/* Icon-manager position helper: for `variant == 0`, bumps a play
 * counter at `self+4` and alternates `FontSetPalette`'s icon-slot id
 * between 0xe/0xf every other call; for `variant != 0` (only ever
 * called with 1/2 by `DrawTitleScreen`), always uses id 0xd. Either way,
 * feeds the resulting icon-manager slot's own position fields (looked
 * up at `self+0xc` + a fixed table offset) through `_call_via_r2` twice
 * (once for the icon at its own position, once for a second icon
 * `0xf0` px to its right), positioning them from the icon-manager's own
 * anchor record. The two `_call_via_r2` calls are
 * virtual calls through the icon manager's `record->slots[0]`/`[2]`
 * entries (same shape as `level_select.c`). */

void DrawTitleMenuItem(u32 *self, s32 text, s32 variant)
{
    struct bitmap_font *im;
    struct icon_slot *slot;
    s32 x;

    if (variant == self[0])
    {
        s32 count = self[1] + 1;
        self[1] = count;
        FontSetPalette((struct bitmap_font *)self[3], ((count >> 2) & 1) + 0xe);
    }
    else
    {
        FontSetPalette((struct bitmap_font *)self[3], 0xd);
    }
    slot = &((struct bitmap_font *)self[3])->record->slots[0];
    x = (0xf0 - _call_via_r2((u8 *)self[3] + slot->offset, (void *)text, slot->ptr)) >> 1;
    im = (struct bitmap_font *)self[3];
    SetIconPos(im, x, variant * 10 + 0x80);
    slot = &im->record->slots[2];
    _call_via_r2((u8 *)im + slot->offset, (void *)text, slot->ptr);
}

/* Per-frame flush helper: resets the shadow-OAM buffer
 * (`ResetOamBuffer`), runs `DrawTitleLogoPieces` (the header/slot OAM builder),
 * and - only while the scratch object's `self+8` hold flag is set -
 * positions three icon-manager slots (ids 0x1a/0x1b/0x3b, one call
 * each via `DrawTitleMenuItem`) before releasing the shadow-OAM buffer
 * (`HideUnusedOamEntries`). */
void DrawTitleScreen(u32 *self)
{
    ResetOamBuffer(gOamBuffer);
    DrawTitleLogoPieces(self);
    if (((u8 *)self)[8] != 0)
    {
        DrawTitleMenuItem(self, GetUiText(0x1a), 0);
        DrawTitleMenuItem(self, GetUiText(0x1b), 1);
        DrawTitleMenuItem(self, GetUiText(0x3b), 2);
    }
    HideUnusedOamEntries(gOamBuffer);
}

/* Standalone one-shot rolling-hash update: XORs `val` into the same
 * `self+0x210` "cheat code" slot `TitleScreenCheatInput` inlines, rotates the
 * result left by 1 bit, then multiplies by 521 (`(x<<6)+x`, `<<3`,
 * `+x` - `x*65*8+x`) and stores it back. Matched as real C: the
 * rotate has to be split into an explicit shift-left/shift-right pair
 * with both intermediates register-pinned (`hi`/`lo` to `r3`/`r2`,
 * matching the ROM's own choice) rather than written as the natural
 * `(v << 1) | (v >> 31)` idiom, which this compiler's optimizer folds
 * into a single Thumb `ROR` instruction the ROM's own compiled output
 * never emits (it always expands the rotate into the explicit
 * shift-shift-or sequence, at least in this ROM's own build). */
/* UNUSED - no caller anywhere in the ROM (no Thumb `bl` to it and no
 * pointer to it in baserom.gba, nor any reference in asm/ or src/). */
void HashTitleCheatInput(u32 *selfArg, u32 val)
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

/* `UpdateTitleLogoPieces`'s init-time twin: seeds all 9 slots' countdown words
 * (`self+0x14+i*0x34`, from `gTitleLogoPieceSeeds`'s own `+4+i*8`
 * table, `+1`) and delta-record pointers (`self+0x40+i*0x34`, from the
 * same table's `+i*8` head) in one pass, and unconditionally stores
 * -1 into each slot's `self+0xf2*2+i*4` play-counter word (matching
 * `UpdateTitleLogoPieces`'s own countdown-reset shape, just via `stm` instead of
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
/* UNUSED - no caller anywhere in the ROM (no Thumb `bl` to it and no
 * pointer to it in baserom.gba, nor any reference in asm/ or src/). */
void ResetTitleLogoPieces(u32 *self)
{
    s32 i;
    struct slot_seed *seedBase;
    s32 *counter;
    struct slot_seed *seed;
    u8 *slot;
    s32 stride;
    s32 zero;

    i = 0;
    seedBase = (struct slot_seed *)gTitleLogoPieceSeeds;
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
 * it (`DestroyStarfield(obj, 3)`); clears the DISPCNT shadow
 * (`gDispcnt`) and commits it (`CommitDispcnt`), zeroes all 256
 * BG palette entries (`0x05000000`), sets REG_BLDCNT/REG_BLDY to a full
 * fade (0xff/0x10), and - only if `flag`'s bit 0 is set - frees the
 * scratch object via `OperatorDelete`. The palette clear needs its zero
 * in a local assigned before the pointer (the ROM materializes it
 * first). */
void DestroyTitleScreen(u32 *self, u32 flag)
{
    s32 i;
    u16 *pal;
    s32 zero;

    if (self[0x82] != 0)
        DestroyStarfield((void *)self[0x82], 3);
    *(u16 *)gDispcnt = 0;
    CommitDispcnt();
    zero = 0;
    pal = (u16 *)PLTT;
    for (i = 0xff; i >= 0; i--)
        *pal++ = zero;
    REG_BLDCNT = 0xff;
    REG_BLDY = 0x10;
    if (flag & 1)
        OperatorDelete(self);
}

/* The part's method table as `RunCompanyLogos` uses it (gcc 2.x C++
 * {this-adjust, fn} records). */
struct part_vtable
{
    u8 unk_00[8];
    struct actor_method m08;    // 0x08 - destroy (arg 3)
    struct actor_method m10;    // 0x10
    struct actor_method m18;    // 0x18
};

void LoadUniversalLogoBg(u32 *self);

/* This subsystem's `self` (a raw `u32 *` throughout, as the matched
 * code indexes it) is a `struct logo_screen`. */
#define SLOT_SYSTEM(self) ((struct logo_screen *)(self))
struct actor_self *InitLogoActor(struct actor_self *self, void *a);
void LoadVvLogoGraphics(u32 *self);
void InitVvLogoPieces(u32 *self);
void UpdateVvLogoPieces(u32 *self);
void DrawVvLogoPieces(struct logo_screen *self);

/* `operator new`: an inline wrapper puts the size constant after the
 * heap flag, as the ROM loads them. */
static inline void *New(u32 size)
{
    return mem_alloc(size, 0x80000000);
}

/* The level-object subsystem's own init/run/teardown driver: sets up
 * the OBJ-tile free list, sprite-frame OAM queue and sprite-frame
 * cache, constructs the actor part (`InitLogoActor` on a 0x54-byte
 * `mem_alloc` block), DMAs the OBJ palette, loads BG2's tilesets and
 * the slot array (`LoadVvLogoGraphics`/`InitVvLogoPieces`), builds the particle
 * background (`InitStarfield`) and BG2's tilemap (`LoadUniversalLogoBg`). Then
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
void RunCompanyLogos(u32 *self)
{
    struct actor_self *part;
    void *bgObj;
    s32 i;
    s32 scale;

    InitObjTileFreeList(OBJ_VRAM0);
    InitSpriteFrameOamQueue();
    InitSpriteFrameCache();
    part = InitLogoActor(New(0x54), gLogoActorAnim);
    {
        struct dma_regs *dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
        dma->src = (u32)gPolarCategoryPalette;
        dma->dst = OBJ_PLTT;
        dma->cnt = 0x80000100;
        dma->cnt;
    }
    LoadVvLogoGraphics(self);
    InitVvLogoPieces(self);
    bgObj = InitStarfield(OperatorNew(0x14));
    LoadUniversalLogoBg(self);
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
        WaitForVBlank();
        UpdateStarfield((s32)bgObj);
    }
    PlaySfx(gAudioContext, 0x4b, 0x100);
    scale = 0x2000;
    SLOT_SYSTEM(self)->fade = -1;
    do
    {
        s32 *fade;
        s32 v;
        s32 q;

        UpdateKeys(gInput);
        if (gKeys.pressed & 9)
        {
            if (SLOT_SYSTEM(self)->fade > 0x40)
                SLOT_SYSTEM(self)->fade = 0x40;
        }
        WaitForVBlank();
        CommitDispcnt();
        fade = &SLOT_SYSTEM(self)->fade;
        if (*fade != -1)
        {
            if (*fade == 0x40)
                PlaySfx(gAudioContext, 0x4c, 0x100);
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
                if (SLOT_SYSTEM(self)->fade == -1)
                    SLOT_SYSTEM(self)->fade = 0xf4;
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
        UpdateStarfield((s32)bgObj);
    } while (SLOT_SYSTEM(self)->fade != 0);
    ((struct dispcnt_bits *)gDispcnt)->bg2 = 0;
    ((struct dispcnt_bits *)gDispcnt)->obj = 1;
    ((struct dispcnt_bits *)gDispcnt)->objMap1D = 1;
    CommitDispcnt();
    *(vu32 *)REG_ADDR_BLDCNT = 0;
    SLOT_SYSTEM(self)->fade = -1;
    SLOT_SYSTEM(self)->timer = -1;
    while (SLOT_SYSTEM(self)->fade != 0)
    {
        s32 *fade;
        register s32 v asm("r1");

        UpdateKeys(gInput);
        if (gKeys.pressed & 9)
        {
            if (SLOT_SYSTEM(self)->timer > 0)
                SLOT_SYSTEM(self)->timer = 1;
        }
        {
            struct part_vtable *vt = (struct part_vtable *)part->vtable;
            ((void (*)(void *))vt->m10.fn)((u8 *)part + vt->m10.thisOffset);
        }
        {
            struct part_vtable *vt = (struct part_vtable *)part->vtable;
            ((void (*)(void *))vt->m18.fn)((u8 *)part + vt->m18.thisOffset);
        }
        RewindOamBuffer(gOamBuffer);
        FlushSpriteFrameOamQueue();
        UpdateVvLogoPieces(self);
        DrawVvLogoPieces(SLOT_SYSTEM(self));
        UpdateStarfield((s32)bgObj);
        WaitForVBlank();
        fade = &SLOT_SYSTEM(self)->fade;
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
        CommitOamBuffer(gOamBuffer);
        FlushVramDmaQueue();
        AgeSpriteFrameCache();
    }
    if (part != NULL)
    {
        struct part_vtable *vt = (struct part_vtable *)part->vtable;
        ((void (*)(void *, s32))vt->m08.fn)((u8 *)part + vt->m08.thisOffset, 3);
    }
    if (bgObj != NULL)
        DestroyStarfield(bgObj, 3);
    if (SLOT_SYSTEM(self)->scratch != NULL)
        OperatorDeleteArray(SLOT_SYSTEM(self)->scratch);
    if (SLOT_SYSTEM(self)->frames != NULL)
        OperatorDeleteArray(SLOT_SYSTEM(self)->frames);
    FreeSpriteFrameCache();
    FreeSpriteFrameOamQueue();
    FreeObjTileFreeList();
    FreeCategorySpriteSheet();
}

/* BG2's own tileset/palette loader for this subsystem: allocates 3
 * VRAM tile blocks (`self+0x424`/`0x428`/`0x42c`), loads 3 palette
 * banks (`gVvLogoEmblemObj`/`gVvLogoLettersObj`/`gVvLogoUrlObj`'s packages) to
 * `0x050003E0`/`_C0`/`_A0` and two packages' tile data (`LoadTaggedAssetBuffered`)
 * into the first two blocks, then allocates a buffer sized from the
 * first package's tile-asset header (`self+0x430`) and unpacks into it,
 * plus a 0x1000-byte scratch buffer (`self+0x434`). The last two
 * stores go through a destination pointer taken before the allocation
 * call, as the ROM computes the address first. */
#define PKG_A ((struct bg_package *)gVvLogoEmblemObj)
#define PKG_B ((struct bg_package *)gVvLogoLettersObj)
#define PKG_C ((struct bg_package *)gVvLogoUrlObj)

void LoadVvLogoGraphics(u32 *self)
{
    SLOT_SYSTEM(self)->tilesA = (u32)AllocVramTileBlock(0x1200);
    SLOT_SYSTEM(self)->tilesB = (u32)AllocVramTileBlock(0x400);
    SLOT_SYSTEM(self)->tilesC = (u32)AllocVramTileBlock(0x1000);
    LoadTaggedAssetBuffered(self, PKG_A->paletteAsset, (void *)(PLTT + 0x3E0));
    LoadTaggedAssetBuffered(self, PKG_B->paletteAsset, (void *)(PLTT + 0x3C0));
    LoadTaggedAssetBuffered(self, PKG_C->paletteAsset, (void *)(PLTT + 0x3A0));
    LoadTaggedAssetBuffered(self, PKG_B->tileAsset, (void *)SLOT_SYSTEM(self)->tilesA);
    LoadTaggedAssetBuffered(self, PKG_C->tileAsset, (void *)SLOT_SYSTEM(self)->tilesB);
    {
        u32 size = *(u32 *)PKG_A->tileAsset >> 8;
        u8 **dst = &SLOT_SYSTEM(self)->frames;
        void *buf;

        *dst = buf = OperatorNewArray(size);
        LoadTaggedAsset(PKG_A->tileAsset, buf);
    }
    {
        u8 **dst = &SLOT_SYSTEM(self)->scratch;
        *dst = OperatorNewArray(0x1000);
    }
}

/* Slot-array field initializer: for all 20 (`0x13`+1) slots, clears
 * the active-flag byte (`self+i*0x34`), sets the play-counter word
 * (`self+4+i*0x34`) from `gVvLogoPieceSeeds`'s own `+4+i*8` field
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
void InitVvLogoPieces(u32 *self)
{
    s32 i;
    u8 zero;
    struct slot_seed *seed;
    u8 *active;
    s32 *hold;
    s32 *countdown;

    i = 0;
    zero = 0;
    seed = (struct slot_seed *)gVvLogoPieceSeeds;
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
        u8 *flags = &SLOT_SYSTEM(self)->sfxPending[0x11];

        for (; j >= 0; j--)
            *flags-- = one;
    }
    SLOT_SYSTEM(self)->frame = 0;
    SLOT_SYSTEM(self)->loops = 0;
    SLOT_SYSTEM(self)->frameTick = 0;
}

/* `LoadVvLogoGraphics`'s per-frame slot-array updater, only while the header
 * hold-word (`self+0x448`) equals -1: for each of 20 slots, mirrors
 * `UpdateTitleLogoPieces`'s own countdown/reload/accumulate shape (offsets `+0`
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

void UpdateVvLogoPieces(u32 *self)
{
    s32 i;
    s32 *timer;

    if (SLOT_SYSTEM(self)->timer == -1)
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
                SLOT_SYSTEM(self)->timer = -2;
            }
        }
        {
            s32 *stage = &SLOT_SYSTEM(self)->loops;
            if (*stage <= 1)
            {
                s32 *sub = &SLOT_SYSTEM(self)->frameTick;
                if (++*sub > 3)
                {
                    *sub = 0;
                    sub = &SLOT_SYSTEM(self)->frame;
                    if (++*sub > 9)
                    {
                        *sub = 0;
                        ++*stage;
                    }
                }
            }
        }
    }
    timer = &SLOT_SYSTEM(self)->timer;
    if (*timer == -2)
        *timer = 0xf0;
    if (*timer > 0)
    {
        if (--*timer != 0)
            return;
        PlaySfx(gAudioContext, 0x50, 0x100);
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
            SLOT_SYSTEM(self)->fade = 0x10;
            SLOT_SYSTEM(self)->timer = -3;
        }
    }
}
