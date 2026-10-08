#include "frontend.hpp"

extern "C" {
#include "match.h"
#include "gba/io_reg.h"
#include "bitmap_font.h"
#include "gba/dma_macros.h"
#include "graphics_package.h"
#include "text.h"
#include "util.h"
#include <libgcc.h>
#include "system.h"
#include "audio.h"
#include "actor.h"
#include "gfx.h"
#include "globals.h"
}

/* Tail of GitHub issue #65's chunk (0x0803686C-0x08037110), split off
 * `title_screen.c` at `DrawVvLogoPieces`. The company-logo screen's last
 * two methods (CompanyLogos, #664 part 10b, include/frontend.hpp; the
 * others are still C in title_screen.c) and the logo actor's (LogoActor:
 * constructor, Update, Draw; its destructor starts language_select.cpp).
 *
 * old_agbcp (OLD_AGBCC_OBJS), as its C was old_agbcc, but built WITH
 * strength reduction (it is not on NO_STRENGTH_REDUCE_OBJS):
 * `DrawVvLogoPieces`'s header loop is check_dbra_loop's reversed counter
 * after the hoisted `&oamA`, which only strength reduction emits, while
 * `InitVvLogoPieces` (still in `title_screen.c`) needs it off. See
 * docs/matching/archive/sr65-naked-retry.md. */

static inline void SetAffine(struct oam_shadow_buffer *buf, s32 m, u16 pa, u16 pb, u16 pc, u16 pd)
{
    s32 idx = m * 4;

    buf->table[idx].attr[3] = pa;
    buf->table[idx + 3].attr[3] = pd;
    buf->table[idx + 1].attr[3] = pb;
    buf->table[idx + 2].attr[3] = pc;
}

/* The tail's matrix write. Writing the two zero terms as literals
 * (instead of passing 0 through parameters as the slot loop does) is
 * what places the ROM's `movs r3, #0` after the entry address. */
static inline void SetAffineZ(struct oam_shadow_buffer *buf, s32 m, u16 pa, u16 pd)
{
    s32 idx = m * 4;

    buf->table[idx].attr[3] = pa;
    buf->table[idx + 3].attr[3] = pd;
    buf->table[idx + 1].attr[3] = 0;
    buf->table[idx + 2].attr[3] = 0;
}

/* Draws the Vicarious Visions logo: piece 19 (four 32x16 entries 32 px
 * apart, tilesB) when it is active; pieces 1-18 (tilesA, 8 tiles each),
 * each one affine while its scale isn't 1:1, playing its sound cue once
 * when it first is; and piece 0, the emblem, whose current frame of
 * `frames` is copied into `scratch`, queued to tilesC and drawn as two
 * 64x64 halves (pulled apart by its x scale while affine).
 *
 * Matched in the #65 strength-reduction retry
 * (docs/matching/archive/sr65-naked-retry.md). It needs strength reduction ON,
 * which is why this file was split off `title_screen.c`. */
void CompanyLogos::DrawVvLogoPieces()
{
    vu16 zero;
    vu32 zero32;
    struct oam_attrs oamA;
    struct oam_attrs oamB;
    struct oam_attrs oamC;
    s32 matrix = 1;
    s32 i;

    {
        struct logo_piece *hdr = &slots[19];

        if (hdr->active) {
            u32 tiles;
            s32 j;

            CLEAR_OAM(&oamA);
            tiles = tilesB;
            oamA.palette = 0xd;
            oamA.size = 2;
            oamA.shape = 1;
            oamA.y = Q16_TO_INT(hdr->posB.q);
            oamA.x = hdr->posA.h.i;
            oamA.tileNum = tiles >> 5;
            for (j = 0; j < 4; j++) {
                AddOamEntry(gOamBuffer, &oamA);
                oamA.tileNum += 8;
                oamA.x += 0x20;
            }
        }
    }
    {
        struct logo_piece *slot = &slots[1];
        u32 tile = (tilesA - (u32)OBJ_VRAM0) >> 5;

        for (i = 0; i <= 0x11; i++) {
            if (slot->active) {
                s32 pa, pd, affine;

                CLEAR_OAM(&oamB);
                pa = 0x1000000 / slot->velA;
                pd = 0x1000000 / slot->velB;
                affine = (pa != 0x100 || pd != pa);
                if (affine) {
                    u8 *flags = sfxPending;
                    u8 *flag = flags + i;

                    if (*flag) {
                        /* A u8 local for the 0 gives the ROM's r3
                         * reload for the store (and so the matrix
                         * reload in r0 below). */
                        u8 z = 0;

                        *flag = z;
                        PlaySfx(gAudioContext, SFX_UNKNOWN_4E, 0x100);
                    }
                    SetAffine(gOamBuffer, matrix, pa, 0, 0, pd);
                    oamB.affineMode = 1;
                    oamB.matrixLo = matrix;
                    matrix++;
                }
                oamB.palette = 0xe;
                oamB.size = 2;
                oamB.shape = 2;
                oamB.y = Q16_TO_INT(slot->posB.q) - 0x10;
                oamB.x = slot->posA.h.i - 8;
                oamB.tileNum = tile;
                AddOamEntry(gOamBuffer, &oamB);
            }
            tile += 8;
            slot++;
        }
    }
    if (slots[0].active) {
        u8 *flag = sfxPending;
        struct logo_piece *slot;
        u32 tile;
        u8 *buf;
        u8 *base;
        u8 *src;
        s32 row;
        s32 pa, pd;
        u8 affine;
        /* Pinned (register allocation; unpinned the DMA base and the
         * address of `frames` swap r2 and r3), so `&frames` takes r3 and
         * the reduced `base + row * 0x100 + 0x60` r3 in the loop. */
        MATCH_HOLD_REG(struct dma_regs *, dma, r2);

        if (*flag) {
            PlaySfx(gAudioContext, SFX_UNKNOWN_4D, 0x100);
            *flag = 0;
        }
        slot = &slots[0];
        tile = (tilesC - (u32)OBJ_VRAM0) >> 5;
        zero32 = 0;
        dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
        dma->src = (u32)&zero32;
        buf = scratch;
        dma->dst = (u32)buf;
        dma->cnt = 0x85000400;
        dma->cnt;
        src = frames + frame * 0xa00;
        /* `base + row * 0x100 + 0x60` is a giv of the row counter, which
         * check_dbra_loop reverses, so loop.c must reduce it into its own
         * pointer (the ROM's r3). `buf + 0x800` stays unreduced: `buf`
         * steps by a 0x100 loaded inside the loop, so `buf` is no biv. */
        base = buf;
        for (row = 0; row < 8; row++) {
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
            MATCH_BARRIER();
            MATCH_BARRIER();
            MATCH_BARRIER();
        }
        QueueVramDmaTransfer(scratch, (void *)tilesC, 0x1000, 0x10);
        CLEAR_OAM(&oamC);
        pa = 0x1000000 / slot->velA;
        pd = 0x1000000 / slot->velB;
        affine = (pa != 0x100 || pd != pa);
        if (affine) {
            SetAffineZ(gOamBuffer, matrix, pa, pd);
            oamC.affineMode = 3;
            oamC.matrixLo = matrix;
        }
        oamC.palette = 0xf;
        oamC.size = 3;
        oamC.shape = 0;
        if (affine) {
            oamC.y = Q16_TO_INT(slot->posB.q) - 0x40;
            oamC.x = slot->posA.h.i - Q16_TO_INT(slot->velA << 5) - 0x40;
        } else {
            oamC.y = Q16_TO_INT(slot->posB.q) - 0x20;
            oamC.x = slot->posA.h.i - 0x40;
        }
        oamC.tileNum = tile;
        AddOamEntry(gOamBuffer, &oamC);
        if (affine) {
            /* Read first, as the ROM loads posA before velA. */
            s32 x = slot->posA.h.i;

            oamC.x = Q16_TO_INT(slot->velA << 5) + x - 0x40;
        } else
            oamC.x = slot->posA.h.i;
        oamC.tileNum = tile + 0x40;
        AddOamEntry(gOamBuffer, &oamC);
    }
}

/* Loads the Universal logo onto BG2 (gUniversalLogoBg): its palette
 * (through a scratch buffer, DMA'd from colour 1), its tiles at VRAM
 * +0x8000, and its map, which BG2's 8-bit affine map at +0xF000 gets a
 * byte per tile of; then BG2CNT (256 colours, 256x256, priority 1) and
 * BG2 on in mode 1. The screen's own fields aren't used.
 *
 * Closed in the issue #64/#65 NAKED retry: each branch stores through
 * `dest++` itself (cross-jumping merges the two stores back into the
 * ROM's single shared `strh`), which doubles `dest`'s reference count
 * and gives it r4 ahead of `y`/`bg2cnt`. */
void CompanyLogos::LoadUniversalLogoBg()
{
    const struct bg_package *pkg = &gUniversalLogoBg;
    u16 *palBuf;
    u16 *mapBuf;
    u16 *dest;
    s32 x;
    s32 y;
    union bgcnt bg2cnt;

    palBuf = new u16[0x100];
    LoadTaggedAsset(pkg->paletteAsset, palBuf);
    {
        struct dma_regs *dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
        dma->src = (u32)(palBuf + 1);
        dma->dst = PLTT + 2;
        dma->cnt = 0x80000040;
        dma->cnt;
    }
    delete[] palBuf;
    LoadTaggedAsset(pkg->tileAsset, (void *)(VRAM + 0x8000));
    mapBuf = new u16[(s32)pkg->height * (s32)pkg->width];
    LoadTaggedAsset(pkg->mapAsset, mapBuf);
    dest = (u16 *)(VRAM + 0xF000);
    for (y = 0; y <= 0x1f; y++) {
        for (x = 0; x <= 0x1f; x += 2) {
            if (y < (s32)pkg->height && x < (s32)pkg->width) {
                s32 i = (s32)pkg->width * y + x;
                *dest++ = (mapBuf[i] & 0xff) | ((mapBuf[i + 1] & 0xff) << 8);
            } else {
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
    delete[] mapBuf;
}

/* The constructor: ActorSelf's (InitActorPart) at (0, 0, 0x100), then
 * two VRAM tile blocks sized from the current frame's tile dimensions
 * (CurFrame, inlined twice), the frame cache's buffer index and last
 * frame reset (DrawLogoActor). */
LogoActor::LogoActor(const struct anim_table_record *anim) : ActorSelf(anim, 0, 0, 0x100)
{
    u8 *frame;

    frame = CurFrame();
    gLogoActorTiles[0] = AllocVramTileBlock(frame[1] * frame[0] * 32);
    frame = CurFrame();
    gLogoActorTiles[1] = AllocVramTileBlock(frame[1] * frame[0] * 32);
    gLogoActorTileBuffer = 1;
    gLogoActorLastFrame = 0;
}

/* The actor's state machine: 0 waits for the first animation to play
 * through, then 1 (sequence 1) waits for frame 0x12, then 2 (sequence 7,
 * SFX_UNKNOWN_4F) waits for frame 7 and freezes the animation
 * (SFX_PLAYER_HURT); 3 moves the actor towards the camera and up for 16
 * frames, then 4, the end (Draw draws nothing). Every frame then steps
 * the animation and rewinds it at the keyframe's loop threshold. */
void LogoActor::Update()
{
    s32 time = ++stateTime;

    /* On the unsigned state: one bounds check for the jump table. */
    switch ((u32)state) {
    case 0:
        if (animDone) {
            SetState(1, 1);
        }
        break;
    case 1:
        if (Q8_TO_INT(animTime) == 0x12) {
            SetState(2, 7);
            PlaySfx(gAudioContext, SFX_UNKNOWN_4F, 0x100);
        }
        break;
    case 2:
        if (Q8_TO_INT(animTime) == 7) {
            animTimer = 0;
            state = 3;
            stateTime = 0;
            PlaySfx(gAudioContext, SFX_PLAYER_HURT, 0x100);
        }
        break;
    case 3:
        z -= 0xe;
        y -= 0x100;
        if (time > 0xf)
            state = 4;
        break;
    }
    animTime += (s16)animTimer;
    animDone = 0;
    if (GetAnimFrameBaseOffset() >= anims[animIndex].loopThreshold) {
        ANIM_REWIND(animTime, anims[animIndex]);
        animDone = 1;
    }
}

/* Draws the actor, unless it is in state 4: projects its position by
 * its depth (an affine sprite scaled by depth / the record's baseDepth,
 * double-size when nearer than that), clips it against the screen and, when the frame changed since
 * the last draw, unpacks the new one into the other of the two VRAM tile
 * blocks before queuing the OAM entry (QueueSpriteFrameOam). */
void LogoActor::Draw()
{
    s32 scale;
    s32 attr1;
    struct anim_frame_record *rec;
    u8 *frame;

    if (state == 4)
        return;
    {
        s32 base = Q8_TO_INT(animTime);
        s32 idx = animIndex;
        struct anim_frame_record *table = anims;
        s32 val;

        val = table[idx].frameIndex;
        rec = &table[idx];
        val += base;
        frame = (u8 *)frameOffsets[val];
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
        depth = z;
        scale = Q8_DIV(depth, record->baseDepth);
        f = 0x100000 / depth;
        sy = Q8_TO_INT(Q12_MUL(y, f) + 0x5000);
        sx = Q8_TO_INT(Q12_MUL(x, f) + 0x7800);
        attr1 = 0x100;
        if (scale <= 0xff) {
            attr1 |= 0x200;
            halfW = w * 8;
            halfH = h * 8;
        }
        sx -= halfW;
        sy -= halfH;
        if (sy <= 0x9f && sy + halfH * 2 >= 0 && sx <= 0xef && sx + halfW * 2 >= 0) {
            u32 attr = (s32)rec->attr << 16;

            attr1 |= (sy & 0xff) | ((sx & 0x1ff) << 16) | attr | GetSpriteShapeSizeBits(frame);
            if (frame != gLogoActorLastFrame) {
                gLogoActorTileBuffer ^= 1;
                gUnpackRleSpriteFrameFunc((u16 *)gLogoActorTiles[gLogoActorTileBuffer],
                                          (struct rle_frame *)frame);
                gLogoActorLastFrame = frame;
            }
            {
                /* The tile number first (r0), then the palette into r1:
                 * a local, ORed into the palette. */
                u32 tile = GET_TILE_NUM(gLogoActorTiles[gLogoActorTileBuffer]);

                QueueSpriteFrameOam(attr1, (palette << 12) | tile, scale);
            }
        }
    }
}
