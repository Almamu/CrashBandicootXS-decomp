#include "sprite_obj.hpp"
#include "frontend.hpp"
#include "audio.hpp"
#include "key_input.hpp"

extern "C" {
#include "gba/io_reg.h"
#include "gba/dma_macros.h"
#include "graphics_package.h"
#include "text.h"
#include "util.h"
#include <libgcc.h>
#include "system.h"
#include "actor.h"
#include "gfx.h"
#include "globals.h"
#include "math_util.h"
}

/* Tail of GitHub issue #65's chunk (0x080361B0-0x08037110). The
 * company-logo screen's methods (CompanyLogos, #664 part 10b,
 * include/frontend.hpp): Run, the VV logo's graphics and pieces (the
 * first four, at the end of title_screen.cpp until #770), the VV logo
 * draw and the Universal logo BG (this file was split off
 * `title_screen.cpp` at `DrawVvLogoPieces`), then the logo actor's
 * (LogoActor: constructor, Update, Draw). The screen's constructor and
 * destructor and the actor's destructor follow in
 * company_logos_ctor.cpp.
 *
 * old_agbcp (OLD_AGBCC_OBJS), as its C was old_agbcc, with strength
 * reduction: `DrawVvLogoPieces`'s header loop is check_dbra_loop's
 * reversed counter after the hoisted `&oamA`, which only strength
 * reduction emits. See docs/matching/archive/sr65-naked-retry.md. */

/* Runs the company logos: the logo actor (Crash), the VV logo's pieces
 * over the starfield with a 60-frame fade-in, the Universal logo zooming
 * in on BG2 (A or START skips ahead) and fading out, then the VV logo
 * until its pieces have flown off, and frees everything.
 *
 * The zoom-in's decrement/grow/shrink is written as "step the counter,
 * then test it again" (which gives the ROM's block order), and the
 * affine X/Y values are computed before either register store. In the
 * fade-out both branches store the decremented value through a local
 * `n` (#662 round 3, for an r1 pin on `v`): with `v--` in the second
 * branch, `v` is live through it, local-alloc gives that block's
 * BLDY address r1 first, and global-alloc then puts `v` in r2 and
 * copies it to r1 for the alpha `v - 0x12`. A block-local `n` takes
 * r1's place there, so `v` gets r1 as in the ROM. */
void CompanyLogos::Run()
{
    LogoActor *part;
    Starfield *bg;
    s32 i;
    s32 scale;

    InitObjTileFreeList(OBJ_VRAM0);
    InitSpriteFrameOamQueue();
    InitSpriteFrameCache();
    part = new LogoActor(&gLogoActorAnim);
    {
        struct dma_regs *dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
        dma->src = (u32)gPolarCategoryPalette;
        dma->dst = OBJ_PLTT;
        dma->cnt = 0x80000100;
        dma->cnt;
    }
    LoadVvLogoGraphics();
    InitVvLogoPieces();
    bg = new Starfield;
    LoadUniversalLogoBg();
    for (i = 0; i <= 0x3b; i++) {
        if (i <= 0x10) {
            REG_BLDCNT = 0xff;
            REG_BLDY = 0x10 - i;
        } else {
            *(vu32 *)REG_ADDR_BLDCNT = 0;
        }
        WaitForVBlank();
        bg->Update();
    }
    gAudioContext->PlaySfx(SFX_UNIVERSAL_LOGO_IN, 0x100);
    scale = 0x2000;
    fade = -1;
    do {
        s32 v;
        s32 q;

        gInput->Update();
        if (gKeys.half.pressed & (A_BUTTON | START_BUTTON)) {
            if (fade > 0x40)
                fade = 0x40;
        }
        WaitForVBlank();
        CommitDispcnt();
        if (fade != -1) {
            if (fade == 0x40)
                gAudioContext->PlaySfx(SFX_UNIVERSAL_LOGO_OUT, 0x100);
            v = fade;
            if (v <= 0x40) {
                s32 a = v >> 2;
                REG_BLDCNT = 0x3f7f;
                REG_BLDALPHA = a | ((0x10 - a) << 8);
            }
            fade = v - 1;
        }
        if (fade == -1) {
            if (scale > 0xffff || (scale += 0x600) > 0xffff) {
                scale = 0x10000;
                if (fade == -1)
                    fade = 0xf4;
            }
        } else if (fade <= 0x40) {
            scale = Q8_MUL(scale, 0x118);
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
        bg->Update();
    } while (fade != 0);
    ((struct dispcnt_bits *)gDispcnt)->bg2 = 0;
    ((struct dispcnt_bits *)gDispcnt)->obj = 1;
    ((struct dispcnt_bits *)gDispcnt)->objMap1D = 1;
    CommitDispcnt();
    *(vu32 *)REG_ADDR_BLDCNT = 0;
    fade = -1;
    timer = -1;
    while (fade != 0) {
        s32 v;

        gInput->Update();
        if (gKeys.half.pressed & (A_BUTTON | START_BUTTON)) {
            if (timer > 0)
                timer = 1;
        }
        part->Update();
        part->Draw();
        gOamBuffer->Rewind();
        FlushSpriteFrameOamQueue();
        UpdateVvLogoPieces();
        DrawVvLogoPieces();
        bg->Update();
        WaitForVBlank();
        v = fade;
        if (v > 0x10) {
            s32 n = v - 1;
            s32 a;

            fade = n;
            a = v - 0x12;
            REG_BLDCNT = 0x3f7f;
            REG_BLDALPHA = (0x10 - a) | (a << 8);
            if (n == 0x11) {
                fade = -1;
                *(vu32 *)REG_ADDR_BLDCNT = 0;
            }
        } else if (v >= 0) {
            s32 n = v - 1;

            fade = n;
            REG_BLDY = 0x10 - n;
            REG_BLDCNT = 0xff;
        }
        gOamBuffer->Commit();
        FlushVramDmaQueue();
        AgeSpriteFrameCache();
    }
    delete part;
    delete bg;
    delete[] scratch;
    delete[] frames;
    FreeSpriteFrameCache();
    FreeSpriteFrameOamQueue();
    FreeObjTileFreeList();
    FreeCategorySpriteSheet();
}

/* Reserves the three OBJ VRAM tile blocks and loads the VV logo's three
 * palettes (gVvLogoEmblemObj's, gVvLogoLettersObj's, gVvLogoUrlObj's)
 * to OBJ palettes 15, 14 and 13, the letters' and the URL's tiles to
 * the first two blocks, and the emblem's frames to `frames`; allocates
 * `scratch`. The frame strip's size is a local, computed before the
 * allocation (the ROM's order). */
void CompanyLogos::LoadVvLogoGraphics()
{
    tilesA = (u32)AllocVramTileBlock(0x1200);
    tilesB = (u32)AllocVramTileBlock(0x400);
    tilesC = (u32)AllocVramTileBlock(0x1000);
    LoadAssetBuffered(gVvLogoEmblemObj.paletteAsset, (void *)(PLTT + 0x3E0));
    LoadAssetBuffered(gVvLogoLettersObj.paletteAsset, (void *)(PLTT + 0x3C0));
    LoadAssetBuffered(gVvLogoUrlObj.paletteAsset, (void *)(PLTT + 0x3A0));
    LoadAssetBuffered(gVvLogoLettersObj.tileAsset, (void *)tilesA);
    LoadAssetBuffered(gVvLogoUrlObj.tileAsset, (void *)tilesB);
    {
        u32 size = *(u32 *)gVvLogoEmblemObj.tileAsset >> 8;
        u8 *buf;

        frames = buf = new u8[size];
        LoadTaggedAsset(gVvLogoEmblemObj.tileAsset, buf);
    }
    scratch = new u8[0x1000];
}

/* Starts the 20 pieces' motions from gVvLogoPieceSeeds, sets every
 * sound-cue flag and rewinds the frame strip.
 *
 * Matches only because this object is built with -fno-strength-reduce
 * (see NO_STRENGTH_REDUCE_OBJS in the Makefile and
 * docs/matching/per-file-flags-investigation.md): with strength
 * reduction on, gcc's loop optimizer reverses the first loop into a
 * count-down (its counter is only used by the exit test) while the ROM
 * keeps `i` counting up. The pointer walks are the source's own - with
 * strength reduction off nothing would have produced them. The second
 * loop's `1` lives in a local assigned before its counter and pointer
 * (the ROM materializes it first). */
void CompanyLogos::InitVvLogoPieces()
{
    s32 i;

    for (i = 0; i <= 0x13; i++) {
        slots[i].active = 0;
        slots[i].countdown = gVvLogoPieceSeeds[i].hold + 1;
        slots[i].record = gVvLogoPieceSeeds[i].record;
    }
    {
        u8 one = 1;
        s32 j = 0x11;
        u8 *flags = &sfxPending[0x11];

        for (; j >= 0; j--)
            *flags-- = one;
    }
    frame = 0;
    loops = 0;
    frameTick = 0;
}

/* Moves the 20 pieces (as TitleScreen::UpdateLogoPieces, without the
 * header) while `timer` is -1, setting it to -2 when a piece's motion
 * has ended, and advances the emblem's frame strip (4 ticks a frame, 10
 * frames, twice). Then, 240 frames later (a sound), flies every piece
 * off upwards; once all are gone it starts the fade (`fade` 0x10). */
void CompanyLogos::UpdateVvLogoPieces()
{
    s32 i;

    if (timer == -1) {
        for (i = 0; i <= 0x13; i++) {
            if (slots[i].countdown != 0) {
                s32 countdown = slots[i].countdown - 1;

                slots[i].countdown = countdown;
                if (countdown == 0) {
                    const struct delta_record *record = slots[i].record++;

                    slots[i].active = 1;
                    countdown = record->hold;
                    slots[i].countdown = countdown;
                    if (countdown != 0) {
                        slots[i].posC = INT_TO_Q16(record->dPosC);
                        slots[i].deltaC = record->deltaC;
                        slots[i].velA = INT_TO_Q8(record->dVelA);
                        slots[i].deltaD = record->deltaD;
                        slots[i].velB = INT_TO_Q8(record->dVelB);
                        slots[i].deltaE = record->deltaE;
                        slots[i].posA.q = INT_TO_Q16(record->dPosA);
                        slots[i].deltaA = record->deltaA;
                        slots[i].posB.q = INT_TO_Q16(record->dPosB);
                        slots[i].deltaB = record->deltaB;
                    }
                } else {
                    slots[i].posC += slots[i].deltaC;
                    slots[i].velA += slots[i].deltaD;
                    slots[i].velB += slots[i].deltaE;
                    slots[i].posA.q += slots[i].deltaA;
                    slots[i].posB.q += slots[i].deltaB;
                }
            } else {
                timer = -2;
            }
        }
        if (loops <= 1) {
            if (++frameTick > 3) {
                frameTick = 0;
                if (++frame > 9) {
                    frame = 0;
                    ++loops;
                }
            }
        }
    }
    if (timer == -2)
        timer = 0xf0;
    if (timer > 0) {
        if (--timer != 0)
            return;
        gAudioContext->PlaySfx(SFX_UNKNOWN_50, 0x100);
    }
    if (timer == 0) {
        s32 allDone = 1;
        LogoPiece *slot;
        LogoPiece *end;

        slot = slots;
        end = &slots[19];
        do {
            if (slot->active != 0) {
                s32 y;

                allDone = 0;
                y = slot->posA.q - 0x80000;
                slot->posA.q = y;
                if (y < -0x7f0000)
                    slot->active = allDone;
            }
            slot++;
        } while ((s32)slot <= (s32)end);
        if (allDone) {
            fade = 0x10;
            timer = -3;
        }
    }
}

static inline void SetAffine(OamBuffer *buf, s32 m, u16 pa, u16 pb, u16 pc, u16 pd)
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
static inline void SetAffineZ(OamBuffer *buf, s32 m, u16 pa, u16 pd)
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
 * which is why this file was split off `title_screen.cpp`. */
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
        LogoPiece *hdr = &slots[19];

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
                gOamBuffer->Add(&oamA);
                oamA.tileNum += 8;
                oamA.x += 0x20;
            }
        }
    }
    {
        LogoPiece *slot = &slots[1];
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
                        gAudioContext->PlaySfx(SFX_UNKNOWN_4E, 0x100);
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
                gOamBuffer->Add(&oamB);
            }
            tile += 8;
            slot++;
        }
    }
    if (slots[0].active) {
        u8 *flag = sfxPending;
        LogoPiece *slot;
        u32 tile;
        u8 *buf;
        u8 *base;
        u8 *src;
        s32 row;
        s32 pa, pd;
        u8 affine;

        if (*flag) {
            gAudioContext->PlaySfx(SFX_UNKNOWN_4D, 0x100);
            *flag = 0;
        }
        slot = &slots[0];
        tile = (tilesC - (u32)OBJ_VRAM0) >> 5;
        /* `scratch` cleared with a 32-bit DMA3 fill from `zero32`, as
         * CLEAR_OAM does (include/frontend.h), and in the loop each row's
         * two halves copied with DmaCopy16. Written with one `dma` pointer
         * for the whole block (the loop's copies through it), the base
         * and `&frames` swapped r2 and r3, and the loop's insn count was
         * three short of the ROM's hoisting (#481); this needed an r2 pin
         * and three empty asm()s. DmaCopy16's own pointer, set in the
         * loop, gives both. */
        zero32 = 0;
        {
            struct dma_regs *dma = (struct dma_regs *)REG_ADDR_DMA3SAD;

            dma->src = (u32)&zero32;
            buf = scratch;
            dma->dst = (u32)buf;
            dma->cnt = 0x85000400;
            dma->cnt;
        }
        src = frames + frame * 0xa00;
        /* `base + row * 0x100 + 0x60` is a giv of the row counter, which
         * check_dbra_loop reverses, so loop.c must reduce it into its own
         * pointer (the ROM's r3). `buf + 0x800` stays unreduced: `buf`
         * steps by a 0x100 loaded inside the loop, so `buf` is no biv. */
        base = buf;
        for (row = 0; row < 8; row++) {
            DmaCopy16(3, src, base + row * 0x100 + 0x60, 0xa0);
            src += 0xa0;
            DmaCopy16(3, src, buf + 0x800, 0xa0);
            src += 0xa0;
            buf += 0x100;
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
        gOamBuffer->Add(&oamC);
        if (affine) {
            /* Read first, as the ROM loads posA before velA. */
            s32 x = slot->posA.h.i;

            oamC.x = Q16_TO_INT(slot->velA << 5) + x - 0x40;
        } else
            oamC.x = slot->posA.h.i;
        oamC.tileNum = tile + 0x40;
        gOamBuffer->Add(&oamC);
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
            gAudioContext->PlaySfx(SFX_UNKNOWN_4F, 0x100);
        }
        break;
    case 2:
        if (Q8_TO_INT(animTime) == 7) {
            animTimer = 0;
            state = 3;
            stateTime = 0;
            gAudioContext->PlaySfx(SFX_PLAYER_HURT, 0x100);
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
