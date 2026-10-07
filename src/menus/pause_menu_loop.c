#include "core.h"
#include "match.h"
#include "audio.h"
#include "actor.h"
#include "bitmap_font.h"
#include "vram_pool.h"
#include "pause_menu.h"
#include "system.h"
#include "menus.h"
#include "globals.h"

/* PauseMenuLoop alone: ROM-address-adjacent to pause_menu.c's
 * DestroyPauseMenu on one side and the already-matched AnimatePauseMenu
 * (pause_menu_draw.c) on the other, so it needs its own object file
 * to keep both neighbors' link-order positions intact (docs/workflow.md
 * step 4's "one .c file per contiguous ROM region" rule) - see
 * docs/matching/archive/issue-7-0x08004d74-overlay-ui.md. */

/* The composite pause/options screen's blocking cursor/confirm/cancel
 * driver (docs/rom_map.md's overlay_ui section) - runs until the user
 * confirms or cancels, redrawing every frame via DrawPauseMenu/
 * CommitPauseMenuFrame/AnimatePauseMenu (the same per-row draw/apply-registers/
 * icon-cycle trio every settings row already uses).
 *
 * `bldy`'s low 5 bits are a blend/fade level (see InitPauseMenu and
 * CommitPauseMenuFrame): first ramps it down to 0 one frame at a time (the
 * screen's fade-in), then the main input loop - L/R adjust the
 * currently-selected row's slider (PauseMenuCursorUp/PauseMenuCursorDown, playing a
 * confirm-ish SFX and arming a short flash via flashTimer), the D-pad
 * bumps the selected row's value up/down with an initial-press vs
 * held-repeat distinction (PauseMenuVolumeDown/PauseMenuVolumeUp), and A confirms the
 * selected row unless its type tag is 4 or 5 (the editable-percentage
 * rows just play SFX 0x48 and keep looping); B cancels (result 0).
 * Either way, ramps the fade level back up to 0x10, resets the DISPCNT
 * shadow `dispcnt` to just bit 6 and applies it once more, and returns
 * the confirmed row's type tag (or 0).
 *
 * Matches under old_agbcc only (this object is on the Makefile's
 * OLD_AGBCC_OBJS; agbcc is 4 bytes longer). `pressed` pinned to r1 keeps
 * the word load plus `lsr #16` (unpinned, combine folds it into
 * `ldrh [keys+2]`); `key` pinned to r3 and the pressed test spelled with
 * the literal give the ROM's two `mov #K`. The input loop is a plain
 * `for (;;)` with the B test at the bottom: old_agbcc's rotation puts
 * that test at the loop top and enters at the body, as in the ROM. The
 * fade pointer (self+0xcc) that GCSE carries past the input loop into
 * the fade-in loop is inserted at the end of the block after the
 * fade-out loop, so `disp` is only taken after the fade-in loop; taken
 * before the input loop it lands ahead of that insertion
 * (docs/matching/archive/early-rom-naked-retry-2.md). */

/* gKeys as the {held, newly pressed} key-state pair. */
struct pause_keys {
    u16 held;
    u16 pressed;
};
#define KEYS (*(struct pause_keys *)&gKeys)

/* bldy: REG_BLDY fade level in the low 5 bits. */
struct pause_fade {
    u8 level:5;
    u8 rest:3;
} __attribute__((packed));
#define FADE(self) ((struct pause_fade *)&(self)->bldy)

/* dispcnt: REG_DISPCNT shadow; bit 6 is set on exit. */
struct pause_dispcnt {
    u16 lo:6;
    u16 bit6:1;
    u16 hi:9;
};

static inline void draw_frame(struct pause_menu *self)
{
    DrawPauseMenu(self);
    CommitPauseMenuFrame(self);
    AnimatePauseMenu(self);
}

s32 PauseMenuLoop(struct pause_menu *self)
{
    s32 result;
    u16 *disp;

    while (FADE(self)->level != 0) {
        FADE(self)->level--;
        draw_frame(self);
    }

    for (;;) {
        u32 in;
        MATCH_HOLD_REG(u32, key, r3);
        MATCH_HOLD_REG(u32, pressed, r1);

        draw_frame(self);
        UpdateKeys(gInput);
        if (KEYS.pressed & DPAD_UP) {
            PauseMenuCursorUp(self);
            self->flashTimer = 0x1e;
            PlaySfx(gAudioContext, SFX_MENU_MOVE, 0x100);
        }
        if (KEYS.pressed & DPAD_DOWN) {
            PauseMenuCursorDown(self);
            self->flashTimer = 0x1e;
            PlaySfx(gAudioContext, SFX_MENU_MOVE, 0x100);
        }
        in = gKeys.all;
        pressed = in >> 16;
        key = DPAD_LEFT;
        if (pressed & DPAD_LEFT) {
            PauseMenuVolumeDown(self);
            self->flashTimer = 0x1e;
        } else if (in & key) {
            if (self->flashTimer == 0) {
                PauseMenuVolumeDown(self);
                self->flashTimer = 5;
            } else {
                self->flashTimer--;
            }
        }
        in = gKeys.all;
        pressed = in >> 16;
        key = DPAD_RIGHT;
        if (pressed & DPAD_RIGHT) {
            PauseMenuVolumeUp(self);
            self->flashTimer = 0x1e;
        } else if (in & key) {
            if (self->flashTimer == 0) {
                PauseMenuVolumeUp(self);
                self->flashTimer = 5;
            } else {
                self->flashTimer--;
            }
        }
        if (KEYS.pressed & A_BUTTON) {
            result = self->rows[self->cursor].type;
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
    while (FADE(self)->level != 0x10) {
        FADE(self)->level++;
        draw_frame(self);
    }
    disp = &self->dispcnt;
    *disp = 0;
    ((struct pause_dispcnt *)disp)->bit6 = 1;
    CommitPauseMenuFrame(self);
    return result;
}
