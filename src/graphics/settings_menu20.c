#include "core.h"
#include "audio.h"
#include "actor.h"
#include "icon_manager.h"
#include "vram_pool.h"
#include "pause_screen_results.h"
#include "memory.h"

/* sub_8005100 alone: ROM-address-adjacent to settings_menu15.c's
 * sub_8005004 on one side and the already-matched sub_8005304
 * (settings_menu17.c) on the other, so it needs its own object file
 * to keep both neighbors' link-order positions intact (docs/workflow.md
 * step 4's "one .c file per contiguous ROM region" rule) - see
 * docs/matching/issue-7-0x08004d74-overlay-ui.md. */

extern void sub_80007AC(void *arg0);
extern void *gUnknown_03001304;
extern u32 gUnknown_030007E0;
extern void sub_8006084(struct pause_screen_results *self);
extern s32 sub_800609C(struct pause_screen_results *self);
extern void sub_8005EF4(struct pause_screen_results *self);
extern void sub_8005FBC(struct pause_screen_results *self);
extern void sub_8006250(struct pause_screen_results *self);
extern void sub_8005304(struct pause_screen_results *self);
extern void PlaySfx(struct AudioContext *self, u32 id, u32 volumeParam);

/* The composite pause/options screen's blocking cursor/confirm/cancel
 * driver (docs/rom_map.md's overlay_ui section) - runs until the user
 * confirms or cancels, redrawing every frame via sub_80053F4/
 * sub_8006250/sub_8005304 (the same per-row draw/apply-registers/
 * icon-cycle trio every settings row already uses).
 *
 * `field_cc`'s low 5 bits are a blend/fade level (see sub_8004EC0 and
 * sub_8006250): first ramps it down to 0 one frame at a time (the
 * screen's fade-in), then the main input loop - L/R adjust the
 * currently-selected row's slider (sub_800609C/sub_8006084, playing a
 * confirm-ish SFX and arming a short flash via field_68), the D-pad
 * bumps the selected row's value up/down with an initial-press vs
 * held-repeat distinction (sub_8005EF4/FBC), and A confirms the
 * selected row unless its type tag is 4 or 5 (the editable-percentage
 * rows just play SFX 0x48 and keep looping); B cancels (result 0).
 * Either way, ramps the fade level back up to 0x10, resets the DISPCNT
 * shadow `field_d0` to just bit 6 and applies it once more, and returns
 * the confirmed row's type tag (or 0).
 *
 * Still NAKED (transcribed from the ROM). The C draft under
 * NON_MATCHING matches everywhere except the L/R key-repeat tests
 * under old_agbcc (14 halfwords): the ROM re-materializes the key mask
 * and reads the pressed half with `lsr #16`, while the draft copies the
 * mask register and gets an `ldrh [keys+2]`. The input loop is a goto
 * loop (no loop-invariant hoisting, as in the ROM); the two fade loops
 * are real loops. */
#if NON_MATCHING
extern struct AudioContext *gUnknown_030012BC;
extern void sub_80053F4(struct pause_screen_results *self);

/* gUnknown_030007E0 as the {held, newly pressed} key-state pair. */
struct pause_keys {
    u16 held;
    u16 pressed;
};
#define KEYS (*(struct pause_keys *)&gUnknown_030007E0)

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
    sub_80053F4(self);
    sub_8006250(self);
    sub_8005304(self);
}

s32 sub_8005100(struct pause_screen_results *self)
{
    s32 result;
    u16 *disp;

    while (FADE(self)->level != 0) {
        FADE(self)->level--;
        draw_frame(self);
    }

    disp = &self->field_d0;
    goto body;

top:
    if (KEYS.pressed & 8) {
        PlaySfx(gUnknown_030012BC, 0x49, 0x100);
        result = 0;
        goto fade_in;
    }
body:
    {
        u32 in;
        u32 key;
        u32 pressed;

        draw_frame(self);
        sub_80007AC(gUnknown_03001304);
        if (KEYS.pressed & 0x40) {
            sub_800609C(self);
            self->field_68 = 0x1e;
            PlaySfx(gUnknown_030012BC, 0x46, 0x100);
        }
        if (KEYS.pressed & 0x80) {
            sub_8006084(self);
            self->field_68 = 0x1e;
            PlaySfx(gUnknown_030012BC, 0x46, 0x100);
        }
        in = gUnknown_030007E0;
        pressed = in >> 16;
        key = 0x20;
        if (pressed & key) {
            sub_8005EF4(self);
            self->field_68 = 0x1e;
        } else if (in & key) {
            if (self->field_68 == 0) {
                sub_8005EF4(self);
                self->field_68 = 5;
            } else {
                self->field_68--;
            }
        }
        in = gUnknown_030007E0;
        pressed = in >> 16;
        key = 0x10;
        if (pressed & key) {
            sub_8005FBC(self);
            self->field_68 = 0x1e;
        } else if (in & key) {
            if (self->field_68 == 0) {
                sub_8005FBC(self);
                self->field_68 = 5;
            } else {
                self->field_68--;
            }
        }
    }
    if (!(KEYS.pressed & 1))
        goto top;
    result = ((struct pause_row *)self->field_14)[self->field_18].type;
    if ((u32)(result - 4) <= 1) {
        PlaySfx(gUnknown_030012BC, 0x48, 0x100);
        goto top;
    }
    PlaySfx(gUnknown_030012BC, 0x49, 0x100);

fade_in:
    while (FADE(self)->level != 0x10) {
        FADE(self)->level++;
        draw_frame(self);
    }
    *disp = 0;
    ((struct pause_dispcnt *)disp)->bit6 = 1;
    sub_8006250(self);
    return result;
}
#else
NAKED s32 sub_8005100(struct pause_screen_results *self)
{
    asm(
    "push {r4, r5, r6, r7, lr}\n\t"
    "mov r7, sb\n\t"
    "mov r6, r8\n\t"
    "push {r6, r7}\n\t"
    "add r5, r0, #0\n\t"
    "add r6, r5, #0\n\t"
    "add r6, #0xcc\n\t"
    "mov r0, #0x1f\n\t"
    "ldrb r1, [r6]\n\t"
    "and r0, r1\n\t"
    "cmp r0, #0\n\t"
    "beq 2f\n\t"
    "mov r2, #0x20\n\t"
    "neg r2, r2\n\t"
    "add r7, r2, #0\n\t"
    "1:\n\t"
    "add r4, r6, #0\n\t"
    "ldrb r2, [r6]\n\t"
    "lsl r0, r2, #0x1b\n\t"
    "lsr r0, r0, #0x1b\n\t"
    "sub r0, #1\n\t"
    "mov r1, #0x1f\n\t"
    "and r0, r1\n\t"
    "and r2, r7\n\t"
    "orr r2, r0\n\t"
    "strb r2, [r6]\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_80053F4\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_8006250\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_8005304\n\t"
    "mov r0, #0x1f\n\t"
    "ldrb r4, [r4]\n\t"
    "and r0, r4\n\t"
    "cmp r0, #0\n\t"
    "bne 1b\n\t"
    "2:\n\t"
    "add r4, r5, #0\n\t"
    "add r4, #0xcc\n\t"
    "mov r0, #0xd0\n\t"
    "add r0, r0, r5\n\t"
    "mov sb, r0\n\t"
    "b 6f\n\t"
    "3:\n\t"
    "ldr r1, 4f\n\t"
    "mov r0, #8\n\t"
    "ldrh r1, [r1, #2]\n\t"
    "and r0, r1\n\t"
    "cmp r0, #0\n\t"
    "beq 6f\n\t"
    "ldr r0, 5f\n\t"
    "ldr r0, [r0]\n\t"
    "mov r2, #0x80\n\t"
    "lsl r2, r2, #1\n\t"
    "mov r1, #0x49\n\t"
    "bl PlaySfx\n\t"
    "mov r7, #0\n\t"
    "b 25f\n\t"
    ".align 2, 0\n"
    "4: .4byte gUnknown_030007E0\n"
    "5: .4byte gUnknown_030012BC\n"
    "6:\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_80053F4\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_8006250\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_8005304\n\t"
    "ldr r0, 9f\n\t"
    "ldr r0, [r0]\n\t"
    "bl sub_80007AC\n\t"
    "ldr r6, 10f\n\t"
    "mov r0, #0x40\n\t"
    "ldrh r1, [r6, #2]\n\t"
    "and r0, r1\n\t"
    "cmp r0, #0\n\t"
    "beq 7f\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_800609C\n\t"
    "mov r0, #0x1e\n\t"
    "str r0, [r5, #0x68]\n\t"
    "ldr r0, 11f\n\t"
    "ldr r0, [r0]\n\t"
    "mov r2, #0x80\n\t"
    "lsl r2, r2, #1\n\t"
    "mov r1, #0x46\n\t"
    "bl PlaySfx\n\t"
    "7:\n\t"
    "mov r0, #0x80\n\t"
    "ldrh r2, [r6, #2]\n\t"
    "and r0, r2\n\t"
    "cmp r0, #0\n\t"
    "beq 8f\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_8006084\n\t"
    "mov r0, #0x1e\n\t"
    "str r0, [r5, #0x68]\n\t"
    "ldr r0, 11f\n\t"
    "ldr r0, [r0]\n\t"
    "mov r2, #0x80\n\t"
    "lsl r2, r2, #1\n\t"
    "mov r1, #0x46\n\t"
    "bl PlaySfx\n\t"
    "8:\n\t"
    "ldr r2, [r6]\n\t"
    "lsr r1, r2, #0x10\n\t"
    "mov r3, #0x20\n\t"
    "mov r0, #0x20\n\t"
    "and r0, r1\n\t"
    "cmp r0, #0\n\t"
    "beq 12f\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_8005EF4\n\t"
    "mov r0, #0x1e\n\t"
    "b 14f\n\t"
    ".align 2, 0\n"
    "9: .4byte gUnknown_03001304\n"
    "10: .4byte gUnknown_030007E0\n"
    "11: .4byte gUnknown_030012BC\n"
    "12:\n\t"
    "and r2, r3\n\t"
    "cmp r2, #0\n\t"
    "beq 15f\n\t"
    "ldr r0, [r5, #0x68]\n\t"
    "cmp r0, #0\n\t"
    "bne 13f\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_8005EF4\n\t"
    "mov r0, #5\n\t"
    "b 14f\n\t"
    "13:\n\t"
    "sub r0, #1\n\t"
    "14:\n\t"
    "str r0, [r5, #0x68]\n\t"
    "15:\n\t"
    "ldr r0, 16f\n\t"
    "ldr r2, [r0]\n\t"
    "lsr r1, r2, #0x10\n\t"
    "mov r3, #0x10\n\t"
    "mov r0, #0x10\n\t"
    "and r0, r1\n\t"
    "cmp r0, #0\n\t"
    "beq 17f\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_8005FBC\n\t"
    "mov r0, #0x1e\n\t"
    "b 19f\n\t"
    ".align 2, 0\n"
    "16: .4byte gUnknown_030007E0\n"
    "17:\n\t"
    "and r2, r3\n\t"
    "cmp r2, #0\n\t"
    "beq 20f\n\t"
    "ldr r0, [r5, #0x68]\n\t"
    "cmp r0, #0\n\t"
    "bne 18f\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_8005FBC\n\t"
    "mov r0, #5\n\t"
    "b 19f\n\t"
    "18:\n\t"
    "sub r0, #1\n\t"
    "19:\n\t"
    "str r0, [r5, #0x68]\n\t"
    "20:\n\t"
    "ldr r1, 22f\n\t"
    "mov r0, #1\n\t"
    "ldrh r1, [r1, #2]\n\t"
    "and r0, r1\n\t"
    "cmp r0, #0\n\t"
    "bne 21f\n\t"
    "b 3b\n\t"
    "21:\n\t"
    "ldr r0, [r5, #0x18]\n\t"
    "ldr r1, [r5, #0x14]\n\t"
    "lsl r0, r0, #3\n\t"
    "add r0, r0, r1\n\t"
    "ldr r7, [r0, #4]\n\t"
    "sub r0, r7, #4\n\t"
    "cmp r0, #1\n\t"
    "bhi 24f\n\t"
    "ldr r0, 23f\n\t"
    "ldr r0, [r0]\n\t"
    "mov r2, #0x80\n\t"
    "lsl r2, r2, #1\n\t"
    "mov r1, #0x48\n\t"
    "bl PlaySfx\n\t"
    "b 3b\n\t"
    ".align 2, 0\n"
    "22: .4byte gUnknown_030007E0\n"
    "23: .4byte gUnknown_030012BC\n"
    "24:\n\t"
    "ldr r0, 28f\n\t"
    "ldr r0, [r0]\n\t"
    "mov r2, #0x80\n\t"
    "lsl r2, r2, #1\n\t"
    "mov r1, #0x49\n\t"
    "bl PlaySfx\n\t"
    "25:\n\t"
    "add r6, r4, #0\n\t"
    "mov r0, #0x1f\n\t"
    "ldrb r1, [r6]\n\t"
    "and r0, r1\n\t"
    "cmp r0, #0x10\n\t"
    "beq 27f\n\t"
    "mov r2, #0x20\n\t"
    "neg r2, r2\n\t"
    "mov r8, r2\n\t"
    "26:\n\t"
    "add r4, r6, #0\n\t"
    "ldrb r2, [r6]\n\t"
    "lsl r0, r2, #0x1b\n\t"
    "lsr r0, r0, #0x1b\n\t"
    "add r0, #1\n\t"
    "mov r1, #0x1f\n\t"
    "and r0, r1\n\t"
    "mov r1, r8\n\t"
    "and r2, r1\n\t"
    "orr r2, r0\n\t"
    "strb r2, [r6]\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_80053F4\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_8006250\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_8005304\n\t"
    "mov r0, #0x1f\n\t"
    "ldrb r4, [r4]\n\t"
    "and r0, r4\n\t"
    "cmp r0, #0x10\n\t"
    "bne 26b\n\t"
    "27:\n\t"
    "mov r0, #0\n\t"
    "mov r2, sb\n\t"
    "strh r0, [r2]\n\t"
    "mov r0, #0x40\n\t"
    "ldrb r1, [r2]\n\t"
    "orr r0, r1\n\t"
    "strb r0, [r2]\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_8006250\n\t"
    "add r0, r7, #0\n\t"
    "pop {r3, r4}\n\t"
    "mov r8, r3\n\t"
    "mov sb, r4\n\t"
    "pop {r4, r5, r6, r7}\n\t"
    "pop {r1}\n\t"
    "bx r1\n\t"
    ".align 2, 0\n"
    "28: .4byte gUnknown_030012BC\n"
    );
}
#endif

