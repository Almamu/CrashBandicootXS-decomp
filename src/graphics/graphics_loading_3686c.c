#include "core.h"
#include "gba/io_reg.h"
#include "icon_manager.h"
#include "actor_self.h"
#include "gba/dma_macros.h"
#include "graphics_package.h"
#include "logo_screen.h"

/* Tail of GitHub issue #65's chunk (0x0803686C-0x08037110), split off
 * `graphics_loading_35d1c.c` at `DrawVvLogoPieces`. Like both earlier halves
 * this is old_agbcc code (OLD_AGBCC_OBJS), but it is built WITH strength
 * reduction (it is not on NO_STRENGTH_REDUCE_OBJS): `DrawVvLogoPieces`'s
 * header loop is check_dbra_loop's reversed counter after the hoisted
 * `&oamA`, which only strength reduction emits, while `InitVvLogoPieces`
 * (still in `graphics_loading_35d1c.c`) needs it off. The shared
 * declarations below are copied from the first file. See
 * docs/matching/sr65-naked-retry.md. */

extern struct oam_shadow_buffer *gOamBuffer;
extern struct AudioContext *gAudioContext;
extern u8 gDispcnt[2];
extern void *gUnknown_03001304;
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
extern u8 gStaticData_0817D768[];
extern u8 gStaticData_0817D77C[];
extern u8 gStaticData_0817D790[];
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
extern void FontSetPalette(struct icon_manager *self, u8 val);
extern s32 GetUiText(s32 arg0);
extern void UpdateStarfield(s32 arg0);
extern void *_call_via_r1(void *arg0, void *fn);
extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern void ShowBg2(void);
extern s32 RandRange(s32 arg0);
extern void *sub_8026EC0(u32 size);
extern void sub_8026EB4(void *ptr);
extern void *sub_8026EDC(s32 size);
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
extern void sub_8026ED0(void *self);
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


/* The other half of `UpdateVvLogoPieces`'s per-frame slot-array update: if
 * the header's own `self+0x3dc` byte is set, positions the header's own
 * OAM-attribute build (via `AddOamEntry`, looped 4x for a 4-frame
 * animation strip) from `self+0x224`'s int16 fields; then, for each of
 * 18 slots, builds and queues (`AddOamEntry`) an OAM entry from that
 * slot's own position fields whenever its `__divsi3`-derived on/off-
 * screen test passes, using the header's own play-index accumulator
 * (`sp+0x20`) to place it into consecutive shadow-OAM group slots. Tail
 * repeats the whole shape once more, unconditionally, for a 19th
 * "extra" slot pair fed from `self+0x424`/`self+0x42c`/`self+0x434`
 * (the same header fields `LoadVvLogoGraphics` populates), queuing a
 * `QueueVramDmaTransfer` for its tile data first. */
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


static inline void SetAffine(struct oam_buf *buf, s32 m, u16 pa, u16 pb, u16 pc, u16 pd)
{
    s32 idx = m * 4;

    buf->entries[idx].affineParam = pa;
    buf->entries[idx + 3].affineParam = pd;
    buf->entries[idx + 1].affineParam = pb;
    buf->entries[idx + 2].affineParam = pc;
}

/* The tail's matrix write. Writing the two zero terms as literals
 * (instead of passing 0 through parameters as the slot loop does) is
 * what places the ROM's `movs r3, #0` after the entry address. */
static inline void SetAffineZ(struct oam_buf *buf, s32 m, u16 pa, u16 pd)
{
    s32 idx = m * 4;

    buf->entries[idx].affineParam = pa;
    buf->entries[idx + 3].affineParam = pd;
    buf->entries[idx + 1].affineParam = 0;
    buf->entries[idx + 2].affineParam = 0;
}

#define SLOT_AT(self, i) (&((struct logo_piece *)(self))[i])
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

/* Matched in the #65 strength-reduction retry
 * (docs/matching/sr65-naked-retry.md). It needs strength reduction ON,
 * which is why this file was split off `graphics_loading_35d1c.c`. */
void DrawVvLogoPieces(struct logo_screen *self)
{
    vu16 zero;
    vu32 zero32;
    struct oam_attrs oamA;
    struct oam_attrs oamB;
    struct oam_attrs oamC;
    s32 matrix = 1;
    s32 i;

    {
        struct logo_piece *hdr = SLOT_AT(self, 19);

        if (hdr->active)
        {
            u32 tiles;
            s32 j;

            ClearOam(&oamA);
            tiles = self->tilesB;
            oamA.palette = 0xd;
            oamA.size = 2;
            oamA.shape = 1;
            oamA.y = hdr->posB.q >> 16;
            oamA.x = hdr->posA.h.i;
            oamA.tileNum = tiles >> 5;
            for (j = 0; j < 4; j++)
            {
                AddOamEntry(gOamBuffer, &oamA);
                oamA.tileNum += 8;
                oamA.x += 0x20;
            }
        }
    }
    {
        struct logo_piece *slot = SLOT_AT(self, 1);
        u32 tile = (self->tilesA - (u32)OBJ_VRAM0) >> 5;

        for (i = 0; i <= 0x11; i++)
        {
            if (slot->active)
            {
                s32 pa, pd, affine;

                ClearOam(&oamB);
                pa = 0x1000000 / slot->velA;
                pd = 0x1000000 / slot->velB;
                affine = (pa != 0x100 || pd != pa);
                if (affine)
                {
                    u8 *flags = self->sfxPending;
                    u8 *flag = flags + i;

                    if (*flag)
                    {
                        /* A u8 local for the 0 gives the ROM's r3
                         * reload for the store (and so the matrix
                         * reload in r0 below). */
                        u8 z = 0;

                        *flag = z;
                        PlaySfx(gAudioContext, 0x4e, 0x100);
                    }
                    SetAffine(OAMBUF, matrix, pa, 0, 0, pd);
                    oamB.affineMode = 1;
                    oamB.matrixLo = matrix;
                    matrix++;
                }
                oamB.palette = 0xe;
                oamB.size = 2;
                oamB.shape = 2;
                oamB.y = (slot->posB.q >> 16) - 0x10;
                oamB.x = slot->posA.h.i - 8;
                oamB.tileNum = tile;
                AddOamEntry(gOamBuffer, &oamB);
            }
            /* Extra-reference nudge (#468): one more use of `tile` raises
             * its allocation priority above `self`'s, so `tile` takes r8
             * and `self` sb, as in the ROM. Emits no code. */
            asm("" : : "r"(tile));
            tile += 8;
            slot++;
        }
    }
    if (SLOT_AT(self, 0)->active)
    {
        u8 *flag = self->sfxPending;
        struct logo_piece *slot;
        u32 tile;
        u8 *buf;
        u8 *base;
        u8 *src;
        s32 row;
        s32 pa, pd;
        u8 affine;
        /* Pinned so the local `self + 0x430` address below takes r3 and
         * `d` (the reduced `base + row * 0x100 + 0x60`) r3 in the loop. */
        register struct dma_regs *dma asm("r2");

        if (*flag)
        {
            PlaySfx(gAudioContext, 0x4d, 0x100);
            *flag = 0;
        }
        slot = SLOT_AT(self, 0);
        tile = (self->tilesC - (u32)OBJ_VRAM0) >> 5;
        zero32 = 0;
        dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
        dma->src = (u32)&zero32;
        buf = self->scratch;
        dma->dst = (u32)buf;
        dma->cnt = 0x85000400;
        dma->cnt;
        src = self->frames + self->frame * 0xa00;
        /* `base + row * 0x100 + 0x60` is a giv of the row counter, which
         * check_dbra_loop reverses, so loop.c must reduce it into its own
         * pointer (the ROM's r3). `buf + 0x800` stays unreduced: `buf`
         * steps by a 0x100 loaded inside the loop, so `buf` is no biv. */
        base = buf;
        for (row = 0; row < 8; row++)
        {
            dma->src = (u32)src;
            dma->dst = (u32)(base + row * 0x100 + 0x60);
            dma->cnt = 0x80000050;
            dma->cnt;
            src += 0xa0;
            dma->src = (u32)src;
            dma->dst = (u32)(buf + 0x800);
            dma->cnt = 0x80000050;
            dma->cnt;
            src += 0xa0;
            buf += 0x100;
            /* Instruction-count padding (#481): three empty asm()s raise
             * the loop's insn count so loop pass 1 hoists only the
             * 0x80000050 count (ip), and pass 2 hoists 0x800 (sl) and the
             * 0x100 after the reduced pointer's init, as in the ROM.
             * They emit no code. */
            asm("");
            asm("");
            asm("");
        }
        QueueVramDmaTransfer(self->scratch, (void *)self->tilesC, 0x1000, 0x10);
        ClearOam(&oamC);
        pa = 0x1000000 / slot->velA;
        pd = 0x1000000 / slot->velB;
        affine = (pa != 0x100 || pd != pa);
        /* Extra-reference nudge (#468): lifts `affine` above `matrix` in
         * global-alloc priority, which keeps sb (preferred by `matrix`)
         * out of its first-pass choice, so it lands in r7. No code. */
        asm("" : : "r"(affine));
        if (affine)
        {
            SetAffineZ(OAMBUF, matrix, pa, pd);
            oamC.affineMode = 3;
            oamC.matrixLo = matrix;
        }
        oamC.palette = 0xf;
        oamC.size = 3;
        oamC.shape = 0;
        if (affine)
        {
            oamC.y = (slot->posB.q >> 16) - 0x40;
            oamC.x = slot->posA.h.i - ((slot->velA << 5) >> 16) - 0x40;
        }
        else
        {
            oamC.y = (slot->posB.q >> 16) - 0x20;
            oamC.x = slot->posA.h.i - 0x40;
        }
        oamC.tileNum = tile;
        AddOamEntry(gOamBuffer, &oamC);
        if (affine)
        {
            /* Read first, as the ROM loads posA before velA. */
            s32 x = slot->posA.h.i;

            oamC.x = ((slot->velA << 5) >> 16) + x - 0x40;
        }
        else
            oamC.x = slot->posA.h.i;
        oamC.tileNum = tile + 0x40;
        AddOamEntry(gOamBuffer, &oamC);
    }
}

/* BG2's tilemap remap loader (the same "remap the tilemap's per-tile
 * palette-select nibble while copying it to VRAM" shape
 * `LoadTitleScreenBg`/`LoadTitleScreenObjTiles` already established), here
 * for `gUniversalLogoBg`'s own package: loads its palette (DMA'd to
 * `0x05000002`), tileset (`0x06008000`), and tilemap (into a freshly
 * `sub_8026EC0`-allocated scratch buffer), remaps every tile's palette
 * nibble into `0x0600F000`, then sets `REG_BG2CNT` (256-color, 8x8
 * screen, priority/base built from the same bit pattern
 * `LoadTitleScreenBg` uses) and marks the icon-manager/HUD blend flags
 * (`gDispcnt`) active. Takes no arguments - this package's
 * pointer lives entirely in the static table, not the scratch
 * object. */
/* Closed in the issue #64/#65 NAKED retry: each branch stores through
 * `dest++` itself (cross-jumping merges the two stores back into the
 * ROM's single shared `strh`), which doubles `dest`'s reference count
 * and gives it r4 ahead of `y`/`bg2cnt`. */
void LoadUniversalLogoBg(u32 *self)
{
    struct bg_package *pkg = (struct bg_package *)gUniversalLogoBg;
    u16 *palBuf;
    u16 *mapBuf;
    u16 *dest;
    s32 x;
    s32 y;
    union bgcnt bg2cnt;

    palBuf = sub_8026EC0(0x200);
    LoadTaggedAsset(pkg->paletteAsset, palBuf);
    {
        struct dma_regs *dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
        dma->src = (u32)(palBuf + 1);
        dma->dst = PLTT + 2;
        dma->cnt = 0x80000040;
        dma->cnt;
    }
    if (palBuf != NULL)
        sub_8026EB4(palBuf);
    LoadTaggedAsset(pkg->tileAsset, (void *)(VRAM + 0x8000));
    mapBuf = sub_8026EC0((s32)pkg->height * (s32)pkg->width * 2);
    LoadTaggedAsset(pkg->mapAsset, mapBuf);
    dest = (u16 *)(VRAM + 0xF000);
    for (y = 0; y <= 0x1f; y++)
    {
        for (x = 0; x <= 0x1f; x += 2)
        {
            if (y < (s32)pkg->height && x < (s32)pkg->width)
            {
                s32 i = (s32)pkg->width * y + x;
                *dest++ = (mapBuf[i] & 0xff) | ((mapBuf[i + 1] & 0xff) << 8);
            }
            else
            {
                *dest++ = 0;
            }
        }
    }
    bg2cnt.raw = 0;
    bg2cnt.bits.charBase = 2;
    bg2cnt.bits.screenBase = 0x1e;
    bg2cnt.bits.colorMode = 1;
    bg2cnt.bits.priority = 1;
    bg2cnt.bits.size = 1;
    REG_BG2CNT = bg2cnt.raw;
    ((struct dispcnt_bits *)gDispcnt)->bg2 = 1;
    ((struct dispcnt_bits *)gDispcnt)->mode = 1;
    if (mapBuf != NULL)
        sub_8026EB4(mapBuf);
}

/* Constructs an actor-part object via `InitActorPart(self, ?, 0, 0,
 * 0x100)` (the "a" parameter is passed straight through from this
 * function's own, unused-by-name second argument - the ROM leaves it
 * as whatever the caller's own `r1` held, here always
 * `gLogoActorAnim`'s address per `RunCompanyLogos`'s call site) then
 * sets its vtable pointer (`self+0x50`) to `gLogoActorVtable` and
 * allocates two VRAM tile blocks sized from the part's own current
 * animation frame's tile dimensions (`GetAnimFrameData`-shaped lookup,
 * inlined twice), stashing both into `gLogoActorTiles[0]`/`[1]` and
 * resetting the `gLogoActorTileBuffer`/`gLogoActorLastFrame`
 * frame-tile-cache bookkeeping pair `DrawLogoActor` reads back. Returns
 * `self`. */
static inline u8 *CurFrame(struct actor_self *self)
{
    s32 base = self->animTime >> 8;
    s32 idx = self->animIndex;
    struct anim_frame_record *table = self->anims;
    s32 val = table[idx].frameIndex;

    val += base;
    return (u8 *)self->frameOffsets[val];
}

struct actor_self *InitLogoActor(struct actor_self *self, void *a)
{
    u8 *frame;

    InitActorPart(self, (s32)a, 0, 0, 0x100);
    self->vtable = (struct actor_vtable *)gLogoActorVtable;
    frame = CurFrame(self);
    gLogoActorTiles[0] = AllocVramTileBlock(frame[1] * frame[0] * 32);
    frame = CurFrame(self);
    gLogoActorTiles[1] = AllocVramTileBlock(frame[1] * frame[0] * 32);
    gLogoActorTileBuffer = 1;
    gLogoActorLastFrame = 0;
    return self;
}

/* One state (of at least 5, `self+0x28`) in an actor-part's own
 * animation-state machine (see `struct anim_part_instance`,
 * src/graphics/actor_anim.c): state 0 waits for a `self+0x12` flag then
 * jumps to state 1 (resets the frame accumulator and reloads the
 * initial frame's duration from the part table's own header);
 * state 1 waits for frame id 0x12 then jumps to state 2 (loads a
 * different frame, plays SFX 0x4f); state 2 waits for frame id 7 then
 * jumps to state 3 (plays SFX 0x1b); state 3 decays a position field
 * (`self+0x24`/`self+0x20`) for 16 frames then jumps to state 4 (a
 * terminal/idle state, tested by `DrawLogoActor`). Every state's tail
 * advances the frame accumulator by the current frame's duration
 * (`self+0x10`) and rolls over to the next keyframe via
 * `GetAnimFrameBaseOffset` once it crosses the current keyframe's own
 * threshold (`+4`), wrapping the accumulator back by `(threshold -
 * loopBase) << 8` per `struct anim_frame_record`. */
void UpdateLogoActor(struct actor_self *self)
{
    s32 time = ++self->stateTime;

    switch ((u32)self->state)
    {
    case 0:
        if (self->animDone)
        {
            ACTOR_SET_STATE(self, 1, 1);
        }
        break;
    case 1:
        if ((self->animTime >> 8) == 0x12)
        {
            ACTOR_SET_STATE(self, 2, 7);
            PlaySfx(gAudioContext, 0x4f, 0x100);
        }
        break;
    case 2:
        if ((self->animTime >> 8) == 7)
        {
            self->animTimer = 0;
            self->state = 3;
            self->stateTime = 0;
            PlaySfx(gAudioContext, 0x1b, 0x100);
        }
        break;
    case 3:
        self->z -= 0xe;
        self->y -= 0x100;
        if (time > 0xf)
            self->state = 4;
        break;
    }
    self->animTime += *(s16 *)&self->animTimer;
    self->animDone = 0;
    if (GetAnimFrameBaseOffset(self) >= self->anims[self->animIndex].loopThreshold)
    {
        self->animTime -= (self->anims[self->animIndex].loopThreshold
                           - self->anims[self->animIndex].loopBase) << 8;
        self->animDone = 1;
    }
}

/* The actor-part's own OAM builder, a no-op once its state machine
 * (`UpdateLogoActor`) reaches state 4 (`self+0x28 == 4`). Resolves the
 * part's current keyframe's tile-graphics pointer (the same
 * `GetAnimFrameData`-shaped lookup `InitLogoActor` inlines), computes its
 * screen position via two `__divsi3` sine/cosine projections against
 * the part's own position/scale fields (`self+0x1c`/`self+0x20`,
 * `self+0x30`'s trampoline record), clips it against the screen bounds,
 * and - only if the resolved tile pointer differs from the last frame's
 * cached one (`gLogoActorLastFrame`) - re-uploads it to whichever of the
 * two VRAM tile blocks `InitLogoActor` allocated isn't currently displayed
 * (`gLogoActorTileBuffer` toggles which), before queuing the OAM entry
 * itself via `QueueSpriteFrameOam`. */
void DrawLogoActor(struct actor_self *self)
{
    s32 scale;
    s32 attr1;
    struct anim_frame_record *rec;
    u8 *frame;

    if (self->state == 4)
        return;
    {
        s32 base = self->animTime >> 8;
        s32 idx = self->animIndex;
        struct anim_frame_record *table = self->anims;
        s32 val;

        val = table[idx].frameIndex;
        rec = &table[idx];
        val += base;
        frame = (u8 *)self->frameOffsets[val];
    }
    /* Declared in a nested block so their stack slots land after
     * `rec`'s, as in the ROM. */
    {
        u32 w, h;
        s32 halfW, halfH;
        s32 sx, sy;
        s32 depth;
        s32 f;

        w = frame[0];
        halfW = w * 4;
        h = frame[1];
        halfH = h * 4;
        depth = self->z;
        scale = (depth << 8) / (*(struct cam_ref **)&self->unk_2C[4])->depth;
        f = 0x100000 / depth;
        sy = (((self->y * f) >> 12) + 0x5000) >> 8;
        sx = (((self->x * f) >> 12) + 0x7800) >> 8;
        attr1 = 0x100;
        if (scale <= 0xff)
        {
            attr1 |= 0x200;
            halfW = w * 8;
            halfH = h * 8;
        }
        sx -= halfW;
        sy -= halfH;
        if (sy <= 0x9f && sy + halfH * 2 >= 0 && sx <= 0xef && sx + halfW * 2 >= 0)
        {
            u32 attr = (s32)rec->attr << 16;

            attr1 |= (sy & 0xff) | ((sx & 0x1ff) << 16) | attr | GetSpriteShapeSizeBits(frame);
            if (frame != gLogoActorLastFrame)
            {
                gLogoActorTileBuffer ^= 1;
                gUnpackRleSpriteFrameFunc(gLogoActorTiles[gLogoActorTileBuffer], frame);
                gLogoActorLastFrame = frame;
            }
            {
                /* the ROM computes the tile number in r0 */
                register u32 tile asm("r0") = GET_TILE_NUM(gLogoActorTiles[gLogoActorTileBuffer]);

                QueueSpriteFrameOam(attr1, tile | (self->palette << 12), scale);
            }
        }
    }
}

