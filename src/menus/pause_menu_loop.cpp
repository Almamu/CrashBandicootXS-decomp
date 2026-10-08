#include "menus.hpp"

extern "C" {
#include "core.h"
#include "match.h"
#include "audio.h"
#include "system.h"
#include "globals.h"
}

/* PauseMenu::Loop (menus.hpp; C++ since the #664 cleanup), between
 * pause_menu.cpp's destructor and Animate (pause_menu_draw.cpp) in the
 * ROM; see docs/matching/archive/issue-7-0x08004d74-overlay-ui.md.
 *
 * The pause menu's input loop, redrawing every frame (Draw, CommitFrame,
 * Animate): fades in (BLDY's level down to 0), then up/down move the
 * cursor and left/right turn the selected row's volume down/up (once on
 * the press, then every 5 frames while held; `flashTimer`), A confirms a
 * row (the volume rows only play a cue) and START resumes (0). Then it
 * fades out (the level back up to 0x10), sets REG_DISPCNT's shadow to
 * just bit 6, commits it and returns the confirmed row's type (or 0).
 *
 * Matches under old_agbcp only (Makefile OLD_AGBCC_OBJS, old_agbcc as C;
 * agbcc is 4 bytes longer). `pressed` pinned to r1 keeps the word load
 * plus `lsr #16` (unpinned, combine folds it into `ldrh [keys+2]`);
 * `key` pinned to r3 and the pressed test spelled with the literal give
 * the ROM's two `mov #K`. The input loop is a plain `for (;;)` with the
 * START test at the bottom: the old compiler's rotation puts that test at
 * the loop top and enters at the body, as in the ROM. The fade pointer
 * (this+0xcc) that GCSE carries past the input loop into the fade-in
 * loop is inserted at the end of the block after the fade-out loop, so
 * `disp` is only taken after the fade-in loop; taken before the input
 * loop it lands ahead of that insertion
 * (docs/matching/archive/early-rom-naked-retry-2.md). */

/* gKeys as the {held, newly pressed} key-state pair. */
struct pause_keys {
    u16 held;
    u16 pressed;
};
#define KEYS (*(struct pause_keys *)&gKeys)

s32 PauseMenu::Loop()
{
    s32 result;
    union MenuDispcnt *disp;

    while (bldy.evy != 0) {
        bldy.evy--;
        Draw();
        CommitFrame();
        Animate();
    }

    for (;;) {
        u32 in;
        MATCH_HOLD_REG(u32, key, r3);
        MATCH_HOLD_REG(u32, pressed, r1);

        Draw();
        CommitFrame();
        Animate();
        UpdateKeys(gInput);
        if (KEYS.pressed & DPAD_UP) {
            CursorUp();
            flashTimer = 0x1e;
            PlaySfx(gAudioContext, SFX_MENU_MOVE, 0x100);
        }
        if (KEYS.pressed & DPAD_DOWN) {
            CursorDown();
            flashTimer = 0x1e;
            PlaySfx(gAudioContext, SFX_MENU_MOVE, 0x100);
        }
        in = gKeys.all;
        pressed = in >> 16;
        key = DPAD_LEFT;
        if (pressed & DPAD_LEFT) {
            VolumeDown();
            flashTimer = 0x1e;
        } else if (in & key) {
            if (flashTimer == 0) {
                VolumeDown();
                flashTimer = 5;
            } else {
                flashTimer--;
            }
        }
        in = gKeys.all;
        pressed = in >> 16;
        key = DPAD_RIGHT;
        if (pressed & DPAD_RIGHT) {
            VolumeUp();
            flashTimer = 0x1e;
        } else if (in & key) {
            if (flashTimer == 0) {
                VolumeUp();
                flashTimer = 5;
            } else {
                flashTimer--;
            }
        }
        if (KEYS.pressed & A_BUTTON) {
            result = rows[cursor].type;
            if ((u32)(result - 4) <= 1) {
                PlaySfx(gAudioContext, SFX_MENU_ERROR, 0x100);
            } else {
                PlaySfx(gAudioContext, SFX_MENU_SELECT, 0x100);
                break;
            }
        }
        if (KEYS.pressed & START_BUTTON) {
            PlaySfx(gAudioContext, SFX_MENU_SELECT, 0x100);
            result = 0;
            break;
        }
    }
    while (bldy.evy != 0x10) {
        bldy.evy++;
        Draw();
        CommitFrame();
        Animate();
    }
    disp = &dispcnt;
    disp->raw = 0;
    disp->bits.objMap1D = 1;
    CommitFrame();
    return result;
}
