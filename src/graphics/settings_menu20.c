#include "core.h"
#include "audio.h"
#include "actor.h"
#include "icon_manager.h"
#include "vram_pool.h"
#include "pause_screen_results.h"
#include "memory.h"

/* PauseMenuLoop alone: ROM-address-adjacent to settings_menu15.c's
 * DestroyPauseMenu on one side and the already-matched AnimatePauseMenu
 * (settings_menu17.c) on the other, so it needs its own object file
 * to keep both neighbors' link-order positions intact (docs/workflow.md
 * step 4's "one .c file per contiguous ROM region" rule) - see
 * docs/matching/issue-7-0x08004d74-overlay-ui.md. */

extern void UpdateKeys(void *arg0);
extern void *gUnknown_03001304;
extern u32 gKeys;
extern void PauseMenuCursorDown(struct pause_screen_results *self);
extern s32 PauseMenuCursorUp(struct pause_screen_results *self);
extern void PauseMenuVolumeDown(struct pause_screen_results *self);
extern void PauseMenuVolumeUp(struct pause_screen_results *self);
extern void CommitPauseMenuFrame(struct pause_screen_results *self);
extern void AnimatePauseMenu(struct pause_screen_results *self);
extern void PlaySfx(struct AudioContext *self, u32 id, u32 volumeParam);

/* The composite pause/options screen's blocking cursor/confirm/cancel
 * driver (docs/rom_map.md's overlay_ui section) - runs until the user
 * confirms or cancels, redrawing every frame via DrawPauseMenu/
 * CommitPauseMenuFrame/AnimatePauseMenu (the same per-row draw/apply-registers/
 * icon-cycle trio every settings row already uses).
 *
 * `field_cc`'s low 5 bits are a blend/fade level (see InitPauseMenu and
 * CommitPauseMenuFrame): first ramps it down to 0 one frame at a time (the
 * screen's fade-in), then the main input loop - L/R adjust the
 * currently-selected row's slider (PauseMenuCursorUp/PauseMenuCursorDown, playing a
 * confirm-ish SFX and arming a short flash via field_68), the D-pad
 * bumps the selected row's value up/down with an initial-press vs
 * held-repeat distinction (PauseMenuVolumeDown/PauseMenuVolumeUp), and A confirms the
 * selected row unless its type tag is 4 or 5 (the editable-percentage
 * rows just play SFX 0x48 and keep looping); B cancels (result 0).
 * Either way, ramps the fade level back up to 0x10, resets the DISPCNT
 * shadow `field_d0` to just bit 6 and applies it once more, and returns
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
 * (docs/matching/early-rom-naked-retry-2.md). */
extern struct AudioContext *gAudioContext;
extern void DrawPauseMenu(struct pause_screen_results *self);

/* gKeys as the {held, newly pressed} key-state pair. */
struct pause_keys {
    u16 held;
    u16 pressed;
};
#define KEYS (*(struct pause_keys *)&gKeys)

/* field_cc: REG_BLDY fade level in the low 5 bits. */
struct pause_fade {
    u8 level:5;
    u8 rest:3;
} __attribute__((packed));
#define FADE(self) ((struct pause_fade *)&(self)->field_cc)

/* field_d0: REG_DISPCNT shadow; bit 6 is set on exit. */
struct pause_dispcnt {
    u16 lo:6;
    u16 bit6:1;
    u16 hi:9;
};

struct pause_row {
    void *label;
    s32 type;
};

static inline void draw_frame(struct pause_screen_results *self)
{
    DrawPauseMenu(self);
    CommitPauseMenuFrame(self);
    AnimatePauseMenu(self);
}

s32 PauseMenuLoop(struct pause_screen_results *self)
{
    s32 result;
    u16 *disp;

    while (FADE(self)->level != 0) {
        FADE(self)->level--;
        draw_frame(self);
    }

    for (;;) {
        u32 in;
        register u32 key asm("r3");
        register u32 pressed asm("r1");

        draw_frame(self);
        UpdateKeys(gUnknown_03001304);
        if (KEYS.pressed & 0x40) {
            PauseMenuCursorUp(self);
            self->field_68 = 0x1e;
            PlaySfx(gAudioContext, 0x46, 0x100);
        }
        if (KEYS.pressed & 0x80) {
            PauseMenuCursorDown(self);
            self->field_68 = 0x1e;
            PlaySfx(gAudioContext, 0x46, 0x100);
        }
        in = gKeys;
        pressed = in >> 16;
        key = 0x20;
        if (pressed & 0x20) {
            PauseMenuVolumeDown(self);
            self->field_68 = 0x1e;
        } else if (in & key) {
            if (self->field_68 == 0) {
                PauseMenuVolumeDown(self);
                self->field_68 = 5;
            } else {
                self->field_68--;
            }
        }
        in = gKeys;
        pressed = in >> 16;
        key = 0x10;
        if (pressed & 0x10) {
            PauseMenuVolumeUp(self);
            self->field_68 = 0x1e;
        } else if (in & key) {
            if (self->field_68 == 0) {
                PauseMenuVolumeUp(self);
                self->field_68 = 5;
            } else {
                self->field_68--;
            }
        }
        if (KEYS.pressed & 1) {
            result = ((struct pause_row *)self->field_14)[self->field_18].type;
            if ((u32)(result - 4) <= 1) {
                PlaySfx(gAudioContext, 0x48, 0x100);
            } else {
                PlaySfx(gAudioContext, 0x49, 0x100);
                break;
            }
        }
        if (KEYS.pressed & 8) {
            PlaySfx(gAudioContext, 0x49, 0x100);
            result = 0;
            break;
        }
    }
    while (FADE(self)->level != 0x10) {
        FADE(self)->level++;
        draw_frame(self);
    }
    disp = &self->field_d0;
    *disp = 0;
    ((struct pause_dispcnt *)disp)->bit6 = 1;
    CommitPauseMenuFrame(self);
    return result;
}
