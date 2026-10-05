#include "core.h"
#include "gba/io_reg.h"
#include "bitmap_font.h"
#include "actor_self.h"
#include "gba/dma_macros.h"
#include "graphics_package.h"

/* GitHub issue #65's chunk (0x080354E0-0x08037110): the remaining 19
 * functions after `InitTitleScreen`/`LoadTitleScreenBg`/
 * `LoadTitleScreenObjTiles` (src/graphics/level_graphics.c - see
 * docs/matching/issue-65-graphics-loading.md). Most of them operate on
 * the title-screen object `InitTitleScreen` builds (still a
 * raw `u32 *` here - stride-0x34 slot records at `self+0x10` holding
 * Q16.16 position / velocity fields fed by a per-slot "delta record"
 * pointer, a BG2 affine-scroll block at `self+0x214..0x21c` and a
 * rolling-hash "cheat code" detector at `self+0x210`), or on the
 * 20-slot variant `RunCompanyLogos`'s subsystem uses; `InitLogoActor`/
 * `UpdateLogoActor`/`DrawLogoActor` operate on a `struct actor_self` actor
 * part.
 *
 * This translation unit is old_agbcc code (see the Makefile's
 * OLD_AGBCC_OBJS) with the default -O2 strength reduction. It holds only
 * `UpdateTitleLogoPieces` and `DrawTitleLogoPieces`; the rest of the chunk (from
 * `TitleScreenCheatInput`) was split into graphics_loading_35d1c.c in the issues
 * #64/#65 second NAKED retry, because that half needs
 * -fno-strength-reduce for `InitVvLogoPieces` while `DrawTitleLogoPieces` needs
 * strength reduction on. See docs/matching/issue-64-65-naked-retry-2.md,
 * docs/matching/issue-65-0x08035780-graphics-loading.md and
 * docs/matching/per-file-flags-investigation.md. */

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
extern u8 gStaticData_08178F80[];
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

void UpdateTitleLogoPieces(u32 *self_arg)
{
    register u32 *self asm("ip") = self_arg;
    s32 i;

    for (i = 0; i <= 8; i++)
    {
        s32 stride = i * 0x34;
        u8 *countdownBase = (u8 *)self + 0x14;
        s32 *countdownPtr = (s32 *)(countdownBase + stride);

        if (*countdownPtr != 0)
        {
            s32 countdown = *countdownPtr - 1;
            *countdownPtr = countdown;
            if (countdown == 0)
            {
                struct delta_record **recordPtrAddr = RecordAt(self, stride);
                /* Pinned to r0 to match the ROM's own register split:
                 * the loaded pointer is kept in one temp (r0) purely to
                 * compute the advanced pointer stored back below, while
                 * `record` gets its own copy for every later dereference. */
                register struct delta_record *recordLoaded asm("r0") = *recordPtrAddr;
                struct delta_record *record = recordLoaded;

                *recordPtrAddr = (struct delta_record *)((u8 *)recordLoaded + 0x20);
                SlotBase(self, stride)[0x10] = 1;

                {
                    s32 hold = record->hold;
                    *countdownPtr = hold;
                    if (hold != 0)
                    {
                        {
                            s32 *dst = PosCAt(self, stride);
                            register u16 tmp asm("r4") = record->dPosC;
                            register s32 shifted asm("r1") = (s32)tmp << 16;
                            *dst = shifted;
                        }
                        *DeltaCAt(self, stride) = record->deltaC;
                        *VelAAt(self, stride) = record->dVelA << 8;
                        *DeltaDAt(self, stride) = record->deltaD;
                        *VelBAt(self, stride) = record->dVelB << 8;
                        *DeltaEAt(self, stride) = record->deltaE;
                        {
                            s32 *dst = PosAAt(self, stride);
                            register u16 tmp asm("r4") = record->dPosA;
                            register s32 shifted asm("r1") = (s32)tmp << 16;
                            *dst = shifted;
                        }
                        *DeltaAAt(self, stride) = record->deltaA;
                        {
                            s32 *dst = PosBAt(self, stride);
                            register u16 tmp asm("r4") = record->dPosB;
                            register s32 shifted asm("r1") = (s32)tmp << 16;
                            *dst = shifted;
                        }
                        *DeltaBAt(self, stride) = record->deltaB;
                    }
                }
            }
            else
            {
                {
                    u8 *dstBase = (u8 *)self + 0x20;
                    s32 *dst = (s32 *)(dstBase + stride);
                    u8 *srcBase = (u8 *)self + 0x34;
                    s32 *src = (s32 *)(srcBase + stride);
                    s32 val = *dst;
                    val += *src;
                    *dst = val;
                }
                {
                    u8 *dstBase = (u8 *)self + 0x24;
                    s32 *dst = (s32 *)(dstBase + stride);
                    u8 *srcBase = (u8 *)self + 0x38;
                    s32 *src = (s32 *)(srcBase + stride);
                    s32 val = *dst;
                    val += *src;
                    *dst = val;
                }
                {
                    u8 *dstBase = (u8 *)self + 0x28;
                    s32 *dst = (s32 *)(dstBase + stride);
                    u8 *srcBase = (u8 *)self + 0x3c;
                    s32 *src = (s32 *)(srcBase + stride);
                    s32 val = *dst;
                    val += *src;
                    *dst = val;
                }
                {
                    u8 *dstBase = (u8 *)self + 0x18;
                    s32 *dst = (s32 *)(dstBase + stride);
                    u8 *srcBase = (u8 *)self + 0x2c;
                    s32 *src = (s32 *)(srcBase + stride);
                    s32 val = *dst;
                    val += *src;
                    *dst = val;
                }
                {
                    u8 *dstBase = (u8 *)self + 0x1c;
                    s32 *dst = (s32 *)(dstBase + stride);
                    u8 *srcBase = (u8 *)self + 0x30;
                    s32 *src = (s32 *)(srcBase + stride);
                    s32 val = *dst;
                    val += *src;
                    *dst = val;
                }
            }
        }
    }
}

/* Builds an OAM affine-sprite entry (via `__divsi3`'s Q8 sine/cosine
 * lookup and `AddOamEntry`'s shadow-OAM insert) for the scratch
 * object's header record (`self+0x1b0`), then walks the same 9-slot
 * array `UpdateTitleLogoPieces` updates: for each active slot (`self+0x14+i*0x34`
 * / `self+0x40+i*0x34` fields, decrementing a per-slot countdown at
 * `gTitleLogoPieceSeeds`-seeded offsets `self+0xf2*2+i*4`, firing
 * `PlaySfx` ids 0x4a/0x3d at zero), and for slots 0-3, builds one more
 * OAM entry per active slot from the shared per-frame DMA-scratch
 * buffer, accumulating a shadow-OAM group index (`sb`) across up to 4
 * physical OAM writes per call. Tail computes two Q8 velocity-integrated
 * position outputs (`self+0x214`/`0x218`) from the header record's own
 * `self+0x210`/`+0x214`(BG scale) fields when the header's own hold
 * flag (`self+8`) is set. */
struct oam_attrs
{
    u32 y:8;            // 0x00
    u32 affineMode:2;   // 0x01
    u32 objMode:2;
    u32 mosaic:1;
    u32 bpp:1;
    u32 shape:2;
    u32 x:9;            // 0x02
    u32 matrixLo:3;
    u32 matrixBit3:1;
    u32 matrixBit4:1;
    u32 size:2;
    u16 tileNum:10;     // 0x04
    u16 priority:2;
    u16 palette:4;
    u16 affineParam;    // 0x06
};

struct oam_entry
{
    u16 attr0;
    u16 attr1;
    u16 attr2;
    s16 affineParam;
};

struct oam_buf
{
    s32 count;
    s32 base;
    s32 matrixCount;
    struct oam_entry entries[128];
};

struct logo_piece
{
    u8 active;          // 0x00
    u8 pad_01[3];
    s32 countdown;      // 0x04
    union {
        s32 q;          // 0x08 - Q16.16 x
        struct { u16 frac; s16 i; } h;
    } posA;
    union {
        s32 q;          // 0x0c - Q16.16 y
        struct { u16 frac; s16 i; } h;
    } posB;
    s32 posC;           // 0x10
    s32 velA;           // 0x14 - depth
    u8 pad_18[0x1c];
};

static inline void SetAffine(struct oam_buf *buf, s32 m, u16 pa, u16 pb, u16 pc, u16 pd)
{
    s32 idx = m * 4;

    buf->entries[idx].affineParam = pa;
    buf->entries[idx + 3].affineParam = pd;
    buf->entries[idx + 1].affineParam = pb;
    buf->entries[idx + 2].affineParam = pc;
}

#define SLOT_AT(self, i) (&((struct logo_piece *)((u8 *)(self) + 0x10))[i])

#define OAMBUF ((struct oam_buf *)gOamBuffer)

#define ClearOam(oam)                                           \
{                                                               \
    struct dma_regs *dma;                                       \
    zero = 0;                                                   \
    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;                  \
    dma->src = (u32)&zero;                                      \
    dma->dst = (u32)(oam);                                      \
    dma->cnt = 0x81000004;                                      \
    dma->cnt;                                                   \
}

/* Closed in the issues #64/#65 second NAKED retry (old_agbcc, strength
 * reduction on):
 * - The offset-table loop is written up-counting (`j = 0; j < 4`).
 *   Strength reduction reverses it (check_dbra_loop) and emits the
 *   `j = 3` start after the hoisted invariants, as in the ROM. This is
 *   why the function lives in an object without -fno-strength-reduce.
 * - ClearOam is a macro that stores `zero` before loading the DMA base.
 *   The second slot loop does its DMA through `dma2`, set before the
 *   loop, so loop.c hoists it into r9 and the table pointer is spilled.
 * - The counter addresses compute the scaled index before
 *   `self + 0x1e4`. `px + dx` locals stop fold from reassociating the
 *   -0x20. The random shake keeps `a - 5` as its own local after the
 *   call.
 * - The second loop has its own counter `k`, and `matrixLo = matrix`
 *   lets the bitfield store do the `& 7`. */
void DrawTitleLogoPieces(u32 *self)
{
    u16 zero;
    s32 matrix = 0;
    s32 i;
    s32 k;
    struct oam_attrs oamA;
    struct oam_attrs oamB;
    struct oam_attrs oamC;

    {
        struct logo_piece *slot = (struct logo_piece *)((u8 *)self + 0x1b0);

        if (slot->active)
        {
            u16 scale = 0x1000000 / slot->velA;
            SetAffine(OAMBUF, matrix, scale, 0, 0, scale);
            ClearOam(&oamA);
            oamA.affineMode = 3;
            oamA.matrixLo = 0;
            oamA.palette = 3;
            oamA.size = 2;
            oamA.shape = 1;
            oamA.y = (slot->posB.q >> 16) - 16;
            oamA.tileNum = 0x1c0;
            oamA.x = (slot->posA.q >> 16) - 0x20 - ((slot->velA << 5) >> 16);
            AddOamEntry(gOamBuffer, &oamA);
            oamA.tileNum += 8;
            oamA.x = (slot->posA.q >> 16) - 0x20;
            AddOamEntry(gOamBuffer, &oamA);
            oamA.tileNum += 8;
            {
                s32 px = slot->posA.q >> 16;
                s32 dx = ((slot->velA << 5) >> 16) - 0x20;

                oamA.x = px + dx;
            }
            AddOamEntry(gOamBuffer, &oamA);
            matrix = 2;
        }
    }
    for (i = 0; i <= 4; i++)
    {
        struct logo_piece *slot = SLOT_AT(self, 7) - i;

        if (slot->active)
        {
            s32 *cnt;
            s32 d;
            u16 scale;
            s32 off;

            {
                u32 idx = (7 - i) << 2;
                u32 base = (u32)self + 0x1e4;
                cnt = (s32 *)(base + idx);
            }
            if (*cnt != 0)
            {
                if (*cnt == -1)
                    *cnt = 10;
                if (--*cnt == 0)
                    PlaySfx(gAudioContext, 0x4a, 0x100);
            }
            d = 0x1000000 / slot->velA;
            scale = d;
            SetAffine(OAMBUF, matrix, scale, 0, 0, scale);
            ClearOam(&oamB);
            off = 0;
            if (d != 0x100)
            {
                off = -32;
                oamB.affineMode = 3;
            }
            else
            {
                oamB.affineMode = 1;
            }
            oamB.matrixLo = matrix;
            matrix++;
            oamB.palette = 0;
            oamB.tileNum = i << 6;
            oamB.size = 3;
            {
                s32 px = slot->posA.q >> 16;
                s32 dx = off - 32;

                oamB.x = px + dx;
            }
            oamB.y = (slot->posB.q >> 16) - 32 + off;
            AddOamEntry(gOamBuffer, &oamB);
        }
    }
    {
        s32 *tbl = (s32 *)gTitleArrowPieceOffsets;
        struct dma_regs *dma2;

        k = 0;
        dma2 = (struct dma_regs *)REG_ADDR_DMA3SAD;
        for (; k <= 1; k++)
        {
            struct logo_piece *slot = SLOT_AT(self, k);
            s32 j;

            if (slot->active)
            {
                s32 *cnt;
                {
                    u32 off = k << 2;
                    u32 base = (u32)self + 0x1e4;
                    cnt = (s32 *)(base + off);
                }

                if (*cnt != 0)
                {
                    if (*cnt == -1)
                    {
                        *cnt = 8;
                        PlaySfx(gAudioContext, 0x3d, 0x100);
                    }
                    else if (--*cnt == 0)
                    {
                        *(s32 *)((u8 *)self + 0x20c) = 30;
                    }
                }
            }
            zero = 0;
            dma2->src = (u32)&zero;
            dma2->dst = (u32)&oamC;
            dma2->cnt = 0x81000004;
            dma2->cnt;
            oamC.palette = k + 1;
            oamC.tileNum = (k << 6) + 0x140;
            oamC.size = 2;
            oamC.priority = 2;
            for (j = 0; j < 4; j++)
            {
                s32 x = (slot->posA.q >> 16) + *tbl++;
                s32 y = (slot->posB.q >> 16) + *tbl++;

                if (y <= 0x8b)
                {
                    oamC.x = x;
                    oamC.y = y;
                    if (slot->active)
                        AddOamEntry(gOamBuffer, &oamC);
                }
                oamC.tileNum += 0x10;
            }
        }
    }
    {
        struct logo_piece *rec = SLOT_AT(self, 2);

        if (rec->active)
        {
            s32 a, b;
            s32 *shake;
            s32 *q;

            ShowBg2();
            a = rec->posA.q;
            b = rec->posB.q;
            shake = (s32 *)((u8 *)self + 0x20c);
            if (*shake != 0)
            {
                --*shake;
                {
                    s32 r = RandRange(10);
                    s32 t = a - 5;
                    a = t + (u16)r;
                }
                {
                    s32 r = RandRange(10);
                    s32 t = b - 5;
                    b = t + (u16)r;
                }
            }
            q = (s32 *)((u8 *)self + 0x21c);
            *q = 0x1000000 / rec->velA;
            *(s32 *)((u8 *)self + 0x214) = *q * (-a >> 16) + 0x4000;
            *(s32 *)((u8 *)self + 0x218) = (-b >> 16) * *q + 0x4000;
        }
    }
}
