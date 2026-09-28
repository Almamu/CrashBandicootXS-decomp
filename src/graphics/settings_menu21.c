#include "core.h"
#include "audio.h"
#include "actor.h"
#include "icon_manager.h"
#include "vram_pool.h"
#include "pause_screen_results.h"
#include "memory.h"

/* sub_80053F4 + sub_800556C: mutually address-adjacent (nothing real
 * sits between them), but bracketed by the already-matched
 * sub_8005304 (settings_menu17.c) before and sub_800570C
 * (settings_menu18.c) after - own object file for the same reason
 * settings_menu20.c documents. See
 * docs/matching/issue-7-0x08004d74-overlay-ui.md. */

extern void sub_8006A90(void *arg0);
extern void sub_8006C28(struct vram_upload_cursor *self);
extern struct oam_shadow_buffer *gUnknown_03001300;
extern void sub_8006A48(struct oam_shadow_buffer *arg0);
extern void sub_800556C(struct pause_screen_results *self);
extern void sub_80061E8(struct pause_screen_results *self);
extern void sub_800619C(struct pause_screen_results *self);
extern void sub_8006124(struct pause_screen_results *self);
extern void sub_800570C(struct pause_screen_results *self);
extern void sub_80057E0(struct pause_screen_results *self);
extern void sub_80058C0(struct pause_screen_results *self);
extern void sub_8008890(void *icon, s32 dx, s32 dy);
extern u32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern struct vram_upload_cursor *gUnknown_030012FC;
extern struct icon_manager *gUnknown_030012DC;
extern struct icon_manager *gUnknown_030012E0;
extern void *sub_8026F38(s32 id);
extern void sub_8028A40(struct icon_manager *self);

static inline void set_icon_mgr_pos(struct icon_manager *m, u32 x, u32 y)
{
    m->posX = x;
    m->posY = y;
}

/* slot 0: measure a label's width; slot 2: draw it. */
#define ICON_SLOT_CALL(mgrExpr, slot, label)                                         \
    ({                                                                               \
        struct icon_manager *_m = (mgrExpr);                                         \
        struct icon_record *_r = _m->record;                                         \
        sub_803AD80((u8 *)_m + _r->slots[slot].offset, (label), _r->slots[slot].ptr); \
    })

/* The composite pause/options screen's per-frame "draw the current
 * settings row" step: draws `self->field_70` (the current level's name
 * label) centered into `gUnknown_030012E0`'s slot pair, then - only
 * when `self->field_74` is set (levels 0-0x13, see sub_800599C) -
 * draws `field_74` followed immediately by `self->buf78` (" N") at a
 * fixed position, forming a "LEVEL N"-shaped composite label.
 * Unconditionally right-aligns `self->buf41` (the completion
 * percentage string) at a fixed row. Calls the per-row list renderer
 * (`sub_800556C`) and an unread sibling (`sub_80061E8`), then
 * dispatches on `self->field_24` (the same state sub_8005304 cycles -
 * cases 0-4 map to `sub_800619C`/`sub_800570C`/`sub_80057E0`/
 * `sub_80058C0`/`sub_8006124`, one per icon-row group), and finally
 * hides `self->field_c0` (the row-cursor icon) if its blink countdown
 * (`field_c4`) has reached 0.
 *
 * Still NAKED (transcribed from the ROM). The C draft under
 * NON_MATCHING is 20 halfwords off under either compiler, all in the
 * two computed-x `set_icon_mgr_pos` calls: the ROM puts x in r3 and y
 * in r2, the draft x in r1/r2 and y in r3. The `field_74` block
 * (constant position) and the jump table already match. */
#if NON_MATCHING
void sub_80053F4(struct pause_screen_results *self)
{
    void *label;
    u32 width;

    sub_8006A90(gUnknown_03001300);
    sub_8006C28(gUnknown_030012FC);
    width = ICON_SLOT_CALL(gUnknown_030012E0, 0, self->field_70);
    set_icon_mgr_pos(gUnknown_030012E0, (0xf0 - width) >> 1, 0xe);
    ICON_SLOT_CALL(gUnknown_030012E0, 2, self->field_70);
    label = self->field_74;
    if (label != NULL) {
        set_icon_mgr_pos(gUnknown_030012E0, 0x20, 0x26);
        ICON_SLOT_CALL(gUnknown_030012E0, 2, label);
        ICON_SLOT_CALL(gUnknown_030012E0, 2, self->buf78);
    }
    width = ICON_SLOT_CALL(gUnknown_030012E0, 0, self->buf41);
    set_icon_mgr_pos(gUnknown_030012E0, 0x8c - width, 0x88);
    ICON_SLOT_CALL(gUnknown_030012E0, 2, self->buf41);
    sub_800556C(self);
    sub_80061E8(self);
    switch (self->field_24) {
    case 0:
        sub_800619C(self);
        break;
    case 1:
        sub_800570C(self);
        break;
    case 2:
        sub_80057E0(self);
        break;
    case 3:
        sub_80058C0(self);
        break;
    case 4:
        sub_8006124(self);
        break;
    }
    if (self->field_c4 == 0)
        sub_8008890(self->field_c0, 0, 0);
    sub_8006A48(gUnknown_03001300);
}
#else
NAKED void sub_80053F4(struct pause_screen_results *self)
{
    asm(
    "push {r4, r5, r6, r7, lr}\n\t"
    "add r5, r0, #0\n\t"
    "ldr r0, 2f\n\t"
    "ldr r0, [r0]\n\t"
    "bl sub_8006A90\n\t"
    "ldr r0, 3f\n\t"
    "ldr r0, [r0]\n\t"
    "bl sub_8006C28\n\t"
    "ldr r6, 4f\n\t"
    "ldr r0, [r6]\n\t"
    "mov r7, #0x98\n\t"
    "lsl r7, r7, #1\n\t"
    "add r1, r0, r7\n\t"
    "ldr r2, [r1]\n\t"
    "mov r3, #0x10\n\t"
    "ldrsh r1, [r2, r3]\n\t"
    "add r0, r0, r1\n\t"
    "ldr r1, [r5, #0x70]\n\t"
    "ldr r2, [r2, #0x14]\n\t"
    "bl sub_803AD80\n\t"
    "mov r1, #0xf0\n\t"
    "sub r1, r1, r0\n\t"
    "lsr r3, r1, #1\n\t"
    "ldr r0, [r6]\n\t"
    "mov r2, #0xe\n\t"
    "mov r4, #0x88\n\t"
    "lsl r4, r4, #1\n\t"
    "add r1, r0, r4\n\t"
    "str r3, [r1]\n\t"
    "mov r3, #0x8a\n\t"
    "lsl r3, r3, #1\n\t"
    "add r1, r0, r3\n\t"
    "str r2, [r1]\n\t"
    "add r1, r0, r7\n\t"
    "ldr r2, [r1]\n\t"
    "mov r4, #0x20\n\t"
    "ldrsh r1, [r2, r4]\n\t"
    "add r0, r0, r1\n\t"
    "ldr r1, [r5, #0x70]\n\t"
    "ldr r2, [r2, #0x24]\n\t"
    "bl sub_803AD80\n\t"
    "ldr r4, [r5, #0x74]\n\t"
    "cmp r4, #0\n\t"
    "beq 1f\n\t"
    "ldr r3, [r6]\n\t"
    "mov r1, #0x20\n\t"
    "mov r0, #0x26\n\t"
    "mov ip, r0\n\t"
    "mov r2, #0x88\n\t"
    "lsl r2, r2, #1\n\t"
    "add r0, r3, r2\n\t"
    "str r1, [r0]\n\t"
    "add r1, #0xf4\n\t"
    "add r0, r3, r1\n\t"
    "mov r2, ip\n\t"
    "str r2, [r0]\n\t"
    "add r1, r7, #0\n\t"
    "add r0, r3, r1\n\t"
    "ldr r1, [r0]\n\t"
    "mov r2, #0x20\n\t"
    "ldrsh r0, [r1, r2]\n\t"
    "add r0, r3, r0\n\t"
    "ldr r2, [r1, #0x24]\n\t"
    "add r1, r4, #0\n\t"
    "bl sub_803AD80\n\t"
    "ldr r0, [r6]\n\t"
    "add r3, r7, #0\n\t"
    "add r1, r0, r3\n\t"
    "ldr r2, [r1]\n\t"
    "mov r4, #0x20\n\t"
    "ldrsh r1, [r2, r4]\n\t"
    "add r0, r0, r1\n\t"
    "add r1, r5, #0\n\t"
    "add r1, #0x78\n\t"
    "ldr r2, [r2, #0x24]\n\t"
    "bl sub_803AD80\n\t"
    "1:\n\t"
    "ldr r0, [r6]\n\t"
    "add r1, r0, r7\n\t"
    "ldr r2, [r1]\n\t"
    "mov r3, #0x10\n\t"
    "ldrsh r1, [r2, r3]\n\t"
    "add r0, r0, r1\n\t"
    "add r4, r5, #0\n\t"
    "add r4, #0x41\n\t"
    "ldr r2, [r2, #0x14]\n\t"
    "add r1, r4, #0\n\t"
    "bl sub_803AD80\n\t"
    "mov r1, #0x8c\n\t"
    "sub r3, r1, r0\n\t"
    "ldr r0, [r6]\n\t"
    "mov r2, #0x88\n\t"
    "mov r6, #0x88\n\t"
    "lsl r6, r6, #1\n\t"
    "add r1, r0, r6\n\t"
    "str r3, [r1]\n\t"
    "mov r3, #0x8a\n\t"
    "lsl r3, r3, #1\n\t"
    "add r1, r0, r3\n\t"
    "str r2, [r1]\n\t"
    "add r1, r0, r7\n\t"
    "ldr r2, [r1]\n\t"
    "mov r6, #0x20\n\t"
    "ldrsh r1, [r2, r6]\n\t"
    "add r0, r0, r1\n\t"
    "ldr r2, [r2, #0x24]\n\t"
    "add r1, r4, #0\n\t"
    "bl sub_803AD80\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_800556C\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_80061E8\n\t"
    "ldr r0, [r5, #0x24]\n\t"
    "cmp r0, #4\n\t"
    "bhi 12f\n\t"
    "lsl r0, r0, #2\n\t"
    "ldr r1, 5f\n\t"
    "add r0, r0, r1\n\t"
    "ldr r0, [r0]\n\t"
    "mov pc, r0\n\t"
    ".align 2, 0\n"
    "2: .4byte gUnknown_03001300\n"
    "3: .4byte gUnknown_030012FC\n"
    "4: .4byte gUnknown_030012E0\n"
    "5: .4byte 6f\n"
    "6:\n\t"
    ".4byte 7f\n"
    ".4byte 8f\n"
    ".4byte 9f\n"
    ".4byte 10f\n"
    ".4byte 11f\n"
    "7:\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_800619C\n\t"
    "b 12f\n\t"
    "8:\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_800570C\n\t"
    "b 12f\n\t"
    "9:\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_80057E0\n\t"
    "b 12f\n\t"
    "10:\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_80058C0\n\t"
    "b 12f\n\t"
    "11:\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_8006124\n\t"
    "12:\n\t"
    "add r0, r5, #0\n\t"
    "add r0, #0xc4\n\t"
    "ldr r0, [r0]\n\t"
    "cmp r0, #0\n\t"
    "bne 13f\n\t"
    "add r0, r5, #0\n\t"
    "add r0, #0xc0\n\t"
    "ldr r0, [r0]\n\t"
    "mov r1, #0\n\t"
    "mov r2, #0\n\t"
    "bl sub_8008890\n\t"
    "13:\n\t"
    "ldr r0, 14f\n\t"
    "ldr r0, [r0]\n\t"
    "bl sub_8006A48\n\t"
    "pop {r4, r5, r6, r7}\n\t"
    "pop {r0}\n\t"
    "bx r0\n\t"
    ".align 2, 0\n"
    "14: .4byte gUnknown_03001300\n"
    );
}
#endif

extern s32 sub_8028A30(struct icon_manager *self, s32 val);

/* One 8-byte record of `self->field_14`'s per-row array: a runtime
 * string-table label id, then a type tag (`sub_800556C` branches on
 * `==4`/`==5`/else; `sub_8005100`'s confirm check uses the same tag). */
struct pause_screen_row_record {
    s32 labelId;
    s32 typeTag;
};

/* The composite pause/options screen's per-row list renderer - draws
 * `self->field_1c` rows (from `self->field_14`'s record array),
 * highlighting whichever matches `self->field_18` (the selected
 * index), each centered horizontally and stacked vertically by
 * `self->field_20` pixels starting at y=0x4a. Three layout variants
 * per row, keyed by the record's type tag (docs/rom_map.md's
 * overlay_ui section, "sub_800556C branches on a per-row type tag"):
 * a plain centered label (any other tag), or - for tags 4/5 - the
 * label additionally offset left by half of a second string's width
 * (`self->buf57` for tag 4, `self->buf4f` for tag 5 - the " <NN%>"
 * scratch buffers sub_800599C/sub_8005EF4/FBC fill), with that second
 * string drawn immediately after at the same position (auto-advancing
 * - "label <NN%>" on one line).
 */
void sub_800556C(struct pause_screen_results *self)
{
    s32 y = 0x4a;
    s32 i;

    for (i = 0; i < self->field_1c; i++) {
        void *label;
        s32 x;

        if (i == self->field_18)
            sub_8028A30(gUnknown_030012DC, 0xf);
        else
            sub_8028A40(gUnknown_030012DC);
        label = sub_8026F38(((struct pause_screen_row_record *)self->field_14)[i].labelId);
        x = 0x32 - (ICON_SLOT_CALL(gUnknown_030012DC, 0, label) >> 1);
        switch (((struct pause_screen_row_record *)self->field_14)[i].typeTag) {
        case 4:
            x -= ICON_SLOT_CALL(gUnknown_030012DC, 0, self->buf57) >> 1;
            set_icon_mgr_pos(gUnknown_030012DC, x, y);
            ICON_SLOT_CALL(gUnknown_030012DC, 2, label);
            ICON_SLOT_CALL(gUnknown_030012DC, 2, self->buf57);
            break;
        case 5:
            x -= ICON_SLOT_CALL(gUnknown_030012DC, 0, self->buf4f) >> 1;
            set_icon_mgr_pos(gUnknown_030012DC, x, y);
            ICON_SLOT_CALL(gUnknown_030012DC, 2, label);
            ICON_SLOT_CALL(gUnknown_030012DC, 2, self->buf4f);
            break;
        default:
            set_icon_mgr_pos(gUnknown_030012DC, x, y);
            ICON_SLOT_CALL(gUnknown_030012DC, 2, label);
            break;
        }
        y += self->field_20;
    }
    sub_8028A40(gUnknown_030012DC);
}
