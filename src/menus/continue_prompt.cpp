/* The continue prompt (ContinuePrompt, frontend.hpp): the constructor,
 * InitGraphics, Loop, then Draw, Blink, CommitFrame, the destructor and
 * Run (GitHub issue #64; those five started frontend/credits.cpp until
 * #767, and the constructor was continue_prompt_init.cpp until #771). All
 * old_agbcc. */

#include "sprite_obj.hpp"
#include "frontend.hpp"
#include "audio.hpp"

extern "C" {
#include "gba/io_reg.h"
#include "graphics_package.h"
#include "memory.h"
#include "globals.h"
#include "match.h"
#include "text.h"
#include "system.h"
}

/* The continue prompt's constructor (ContinuePrompt, frontend.hpp;
 * RunContinuePrompt allocates it): three BG buffers (BG0 the smoke, BG1
 * the Uka Uka background, BG2 the glow) and their graphics packages,
 * palette color 0 cleared, DISPCNT with BG0-BG2 on, the graphics
 * (InitGraphics, below), then BLDCNT/BLDALPHA blending BG2
 * over BG0 (EVA 8/16, EVB 16/16), every register applied, and the music
 * faded out. */
ContinuePrompt::ContinuePrompt()
{
    bg0Buf = new BgSetup(0, 0x1f, 0, 3);
    bg1Buf = new BgSetup(3, 0x1e, 0, 1);
    bg2Buf = new BgSetup(2, 0x1d, 1, 2);
    bg1Buf->Load(&gContinuePromptUkaUkaBg);
    bg0Buf->Load(&gContinuePromptSmokeBg);
    bg2Buf->Load(&gContinuePromptGlowBg);
    *(vu16 *)PLTT = 0;
    dispcnt.raw = 0;
    dispcnt.bits.objMap1D = 1;
    dispcnt.bits.mode = 0;
    dispcnt.bits.bg0 = 1;
    dispcnt.bits.bg1 = 1;
    dispcnt.bits.bg2 = 1;
    InitGraphics();
    blend.raw = 0;
    blend.bits.bg2First = 1;
    blend.bits.bg0Second = 1;
    blend.bits.eva = 8;
    blend.bits.evb = 16;
    blend.bits.effect = 1;
    REG_BG0CNT = bg0Buf->GetControl();
    *(vu32 *)REG_ADDR_BG0HOFS = 0;
    REG_BG1CNT = bg1Buf->GetControl();
    *(vu32 *)REG_ADDR_BG1HOFS = 0;
    REG_BG2CNT = bg2Buf->GetControl();
    *(vu32 *)REG_ADDR_BG2HOFS = 0;
    *(vu16 *)REG_ADDR_DISPCNT = dispcnt.raw;
    *(vu32 *)REG_ADDR_BLDCNT = blend.raw;
    blinkCounter = 0;
    selection = 0;
    gAudioContext->FadeOutMusic(0);
}

/* The continue prompt's graphics (ContinuePrompt, frontend.hpp; called by
 * the constructor, above): resets the OBJ VRAM cursor,
 * sets up gSmallFont (its tiles uploaded, no margin, its tiles reserved),
 * claims palette slots 0-3 and fills them from gContinuePromptPalette0-3,
 * turns the OBJs on and clears OAM.
 *
 * The palette loop is indexed through `i`: gcc strength-reduces every
 * access into its own pointer but keeps `i` as the up-counting trip
 * counter (r7) the ROM has. Written with explicit pointer increments, `i`
 * has nothing left to do but count, and gcc reverses it into a
 * down-counter. */
void ContinuePrompt::InitGraphics()
{
    PaletteCache *cache;
    s32 i;

    gObjVramCursor->baseTile = 0;
    gObjVramCursor->Reset();
    gObjVramCursor->Reset();

    icons = gSmallFont;
    icons->SetTileBase(0);
    icons->SetMargin(0);
    gObjVramCursor->Reserve(icons->tileCount << 5);
    gObjVramCursor->Mark();

    gPaletteCache->FreeUnlockedSlots();
    gPaletteCache->ClaimSlot(0);
    gPaletteCache->ClaimSlot(1);
    gPaletteCache->ClaimSlot(2);
    gPaletteCache->ClaimSlot(3);

    cache = gPaletteCache;
    {
        u16 *destA = (u16 *)cache->slots[0];
        u16 *destB = (u16 *)cache->slots[2];

        for (i = 0; i < 16; i++) {
            destA[i] = gContinuePromptPalette0[i];
            destA[i + 0x10] = gContinuePromptPalette1[i];
            destB[i] = gContinuePromptPalette2[i];
            destB[i + 0x10] = gContinuePromptPalette3[i];
        }
    }
    gPaletteCache->Upload();

    dispcnt.bits.obj = 1;

    gOamBuffer->Reset();
    gOamBuffer->HideUnused();
    WaitForVBlank();
    gOamBuffer->Commit();
}

/* The prompt's loop: up and down move the cursor between Yes (0) and No
 * (1), A or START confirms; every second frame BLDALPHA's EVA steps
 * down to 0 and back up to 16. Returns 1 for Yes.
 *
 * From the late-ROM retry passes (docs/matching/archive/late-rom-naked-retry.md):
 * `k` is a struct copy of the input word (the ROM's word load and `lsrs
 * #16` per test, then a fresh `ldrh` for the DPAD_DOWN test after the
 * calls), and the loop is `while (dir >= 0)` (with `while (1)` jump.c
 * moves the return computation to the `break`). Two empty asm statements,
 * which emit no code, are still needed in C++:
 *  - `MATCH_USE(audio)` at the top of the loop adds one reference to
 *    `audio`, so it outranks the pair counter for r7 (the counter lands in
 *    r8 without a pin);
 *  - `MATCH_KEEP(k)` between the A and START tests makes the second
 *    test's shift a fresh value, so CSE doesn't share it with the first,
 *    and the first test keeps the ROM's `movs r0, #1; ands r0, r1`
 *    register choice. The START test's `u16` result is the 0 the ROM
 *    then stores into `selection`.
 * #662 round 2: without the MATCH_KEEP the only difference is that shared
 * shift (inline u16 helpers, a union copy, swapped operands and `!= 0`
 * don't separate it), and `gAudioContext->` written directly gives the
 * counter r7 and the pointer r8.
 * #662 round 3, from the -da dumps: the two shifts are separate insns up
 * to reload (cse, gcse and combine keep both); it is reload's
 * reload_cse_regs that deletes the START test's `lsrs r1, r2, #16`,
 * because r1 still holds `word >> 16` from the A test. The keep's "+r"
 * operand is what tells it r2 may have changed; the ROM's compile had
 * something there that did the same, which no C statement between the
 * tests reproduces (agbcp instead of old_agbcp is further off). Without
 * the MATCH_USE, global-alloc ranks the pair counter (5 refs in the
 * loop) above `audio` (3) and gives it r7; declaration order, `u32 i`
 * and `++i >= 2` don't change the ranking. */
s32 ContinuePrompt::Loop()
{
    s32 dir = 1;
    s32 i = 0;
    s32 level = blend.bits.eva;
    struct held_pressed_pair *input = &gKeys.half;
    AudioContext **audio = &gAudioContext;

    while (dir >= 0) {
        MATCH_USE(audio); /* extra reference: audio outranks i for r7 */
        UpdateKeys(gInput);
        {
            struct held_pressed_pair k = *input;

            /* The MATCH_KEEP keeps the & 8 test's shift separate from the
             * & 1 test's. */
            // clang-format off
            if ((k.pressed & A_BUTTON) || ({ MATCH_KEEP(k); (u16)(k.pressed & START_BUTTON); })) {
                // clang-format on
                (*audio)->PlaySfx(SFX_MENU_SELECT, 0x100);
                break;
            }
            if ((k.pressed & DPAD_UP) && selection == 1) {
                (*audio)->PlaySfx(SFX_MENU_MOVE, 0x100);
                selection = 0;
            }
        }
        if ((input->pressed & DPAD_DOWN) && selection == 0) {
            (*audio)->PlaySfx(SFX_MENU_MOVE, 0x100);
            selection = 1;
        }
        Draw();
        CommitFrame();
        if (++i > 1) {
            i = 0;
            if (dir) {
                if (--level <= 0)
                    dir = 0;
            } else {
                if (++level > 15)
                    dir = 1;
            }
            blend.bits.eva = level;
            *(vu32 *)REG_ADDR_BLDCNT = blend.raw;
        }
    }
    return selection == 0;
}

/* Draws the Yes/No labels (UI texts 0x28-0x2a) with gSmallFont, the
 * selected one blinking (Blink) and marked with the cursor. */
void ContinuePrompt::Draw()
{
    s32 w;

    gOamBuffer->Reset();
    gObjVramCursor->Rewind();
    w = icons->MeasureText((u8 *)GetUiText(0x28));
    icons->SetPalette(0);
    icons->SetPos(0x88 - w, 0x87);
    icons->DrawText((u8 *)GetUiText(0x28));
    icons->SetPalette(Blink(0));
    if (selection == 0) {
        icons->SetPos(0x90, 0x87);
        icons->DrawText((u8 *)gContinuePromptCursorText);
    }
    icons->SetPos(0x98, 0x87);
    icons->DrawText((u8 *)GetUiText(0x29));
    icons->SetPalette(Blink(1));
    if (selection == 1) {
        icons->SetPos(0x90, 0x91);
        icons->DrawText((u8 *)gContinuePromptCursorText);
    }
    icons->SetPos(0x98, 0x91);
    icons->DrawText((u8 *)GetUiText(0x2a));
    gOamBuffer->HideUnused();
}

/* The palette of `option`'s label: 1 when it isn't selected, else 0 or 2
 * from the blink counter, which it advances. */
s32 ContinuePrompt::Blink(s32 option)
{
    if (option == selection) {
        return (blinkCounter++ >> 1) & 2;
    }
    return 1;
}

void ContinuePrompt::CommitFrame()
{
    WaitForVBlank();
    gOamBuffer->Commit();
    FlushVramDmaQueue();
    REG_DISPCNT = dispcnt.raw;
}

/* Frees the three BG buffers. */
ContinuePrompt::~ContinuePrompt()
{
    delete bg0Buf;
    delete bg1Buf;
    delete bg2Buf;
}

/* Runs the prompt and returns the choice (0 yes, 1 no). */
u8 ContinuePrompt::Run()
{
    ContinuePrompt *self;
    u8 result;

    mem_free_bytes(0xc0000000);
    self = new ContinuePrompt;
    result = self->Loop();
    delete self;
    mem_free_bytes(0xc0000000);
    return result;
}
