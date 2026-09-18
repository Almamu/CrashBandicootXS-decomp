#include "core.h"
#include "icon_manager.h"
#include "pause_options_screen.h"

extern s32 sub_8028A30(void *mgr, s32 arg1);
extern s32 sub_8026F38(s32 arg0);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern struct icon_manager *gUnknown_030012E0;
extern struct icon_manager *gUnknown_030012DC;

#if NON_MATCHING
/* The functions below (0x08003B40, 0x08003BDC, 0x08003C90, 0x08003D3C,
 * 0x08003F30, 0x08004914, 0x080041BC) are all reconstructed (semantics
 * understood) but NOT YET BYTE-MATCHING - parked here the same way
 * sub_8006600 (src/graphics/oam_count.c) is. Their raw bytes stay
 * wrapped in `.if NON_MATCHING == 0` in asm/code_3_1_10_3.s and the two
 * new fragments split off it (asm/code_3_1_10_4.s, asm/code_3_1_10_5.s)
 * - see docs/matching.md's write-up for this chunk (and
 * docs/matching/issue-6-0x08003f30-overlay-ui.md for sub_8003F30,
 * genuinely untouched until a later pass) for the full
 * register-allocation story. */

/* Same centered-label shape as sub_80049CC below, but always label
 * 0x23, drawn into gUnknown_030012DC (not E0) at fixed Y=0x87, and with
 * a highlight-dependent initial visibility call. NOT YET BYTE-MATCHING:
 * came within one register-letter choice of matching (a scratch
 * register for reloading `mgr` before the posX/posY writes - r0 here,
 * r3 in the ROM) after reordering the width->half computation ahead of
 * the reload; several explicit-register-pin attempts on just that one
 * temporary didn't close it, and it wasn't worth the same open-ended
 * chase documented for sub_80049CC below. */
void sub_8003C90(struct pause_options_screen *self, u8 highlight)
{
    register struct icon_manager **mgrAddr asm("r8");
    register struct icon_manager *mgr asm("r4");
    register s32 recOff asm("r5");
    struct icon_slot *slot0;
    s32 str;
    s32 width;

    if (highlight) {
        sub_8028A30(gUnknown_030012DC, ((self->flags >> 2) & 1) ? 1 : 2);
    } else {
        sub_8028A30(gUnknown_030012DC, 0);
    }

    mgrAddr = &gUnknown_030012DC;
    mgr = *mgrAddr;
    recOff = 0x98 << 1;
    slot0 = &(*(struct icon_record **)((u8 *)mgr + recOff))->slots[0];
    mgr = (struct icon_manager *)((u8 *)mgr + slot0->offset);
    str = sub_8026F38(0x23);
    width = sub_803AD80(mgr, (void *)str, slot0->ptr);

    {
        s32 half = (0xf0 - width) >> 1;
        mgr = *mgrAddr;
        mgr->posX = half;
        mgr->posY = 0x87;
    }
    slot0 = &(*(struct icon_record **)((u8 *)mgr + recOff))->slots[2];
    mgr = (struct icon_manager *)((u8 *)mgr + slot0->offset);
    str = sub_8026F38(0x23);
    sub_803AD80(mgr, (void *)str, slot0->ptr);
}

/* Draws a centered label (from the runtime string table via
 * sub_8026F38) into gUnknown_030012E0's icon pair - `self` is unused.
 * Matches sub_8006600's (src/graphics/oam_count.c) centered-icon shape
 * exactly, just for a single label rather than flanking a number.
 * NOT YET BYTE-MATCHING: gcc's natural register choice for the s16
 * shift-index reused in the offset read (record+0x10/+0x20) differs
 * from the ROM's fresh reload into r3 both times - every other
 * instruction (including the r4/r5/r6/r8/r9 register-variable pins
 * below) matches exactly. */
void sub_80049CC(struct pause_options_screen *self, s32 labelIndex)
{
    register s32 label asm("r9") = labelIndex;
    register struct icon_slot *slot0 asm("r8");
    register struct icon_manager **mgrAddr asm("r6");
    register struct icon_manager *mgr asm("r4");
    register s32 recOff asm("r5");
    struct icon_record *rec;
    s16 off;
    s32 str;
    s32 width;

    mgrAddr = &gUnknown_030012E0;
    sub_8028A30(*mgrAddr, 0);
    mgr = *mgrAddr;
    recOff = 0x98 << 1;
    rec = *(struct icon_record **)((u8 *)mgr + recOff);
    slot0 = &rec->slots[0];
    off = *(s16 *)((u8 *)rec + 0x10);
    mgr = (struct icon_manager *)((u8 *)mgr + off);
    str = sub_8026F38(label);
    width = sub_803AD80(mgr, (void *)str, slot0->ptr);

    mgr = *mgrAddr;
    mgr->posX = (0xf0 - width) >> 1;
    mgr->posY = 6;
    rec = *(struct icon_record **)((u8 *)mgr + recOff);
    slot0 = &rec->slots[2];
    off = *(s16 *)((u8 *)rec + 0x20);
    mgr = (struct icon_manager *)((u8 *)mgr + off);
    str = sub_8026F38(label);
    sub_803AD80(mgr, (void *)str, slot0->ptr);
}

/* NOT YET BYTE-MATCHING: same register-letter difficulty as
 * sub_80049CC above (this function additionally spills a constant
 * through `ip` in the ROM, which plain C has no way to request).
 * Draws `label1` (if non-zero) centered at Y=0x87, then `label2` (if
 * non-zero) centered at Y=0x91, both into gUnknown_030012DC. */
void sub_8003BDC(struct pause_options_screen *self, s32 label1, s32 label2)
{
    struct icon_manager **mgrAddr = &gUnknown_030012DC;
    struct icon_manager *mgr;
    struct icon_record *rec;
    s16 off;
    s32 width;

    sub_8028A30(*mgrAddr, 0);

    if (label1 != 0) {
        mgr = *mgrAddr;
        rec = *(struct icon_record **)((u8 *)mgr + (0x98 << 1));
        off = *(s16 *)((u8 *)rec + 0x10);
        width = sub_803AD80((u8 *)mgr + off, (void *)label1, *(void **)((u8 *)rec + 0x14));
        mgr = *mgrAddr;
        mgr->posX = (0xf0 - width) >> 1;
        mgr->posY = 0x87;
        rec = *(struct icon_record **)((u8 *)mgr + (0x98 << 1));
        off = *(s16 *)((u8 *)rec + 0x20);
        sub_803AD80((u8 *)mgr + off, (void *)label1, *(void **)((u8 *)rec + 0x24));
    }
    if (label2 != 0) {
        mgr = *mgrAddr;
        rec = *(struct icon_record **)((u8 *)mgr + (0x98 << 1));
        off = *(s16 *)((u8 *)rec + 0x10);
        width = sub_803AD80((u8 *)mgr + off, (void *)label2, *(void **)((u8 *)rec + 0x14));
        mgr = *mgrAddr;
        mgr->posX = (0xf0 - width) >> 1;
        mgr->posY = 0x91;
        rec = *(struct icon_record **)((u8 *)mgr + (0x98 << 1));
        off = *(s16 *)((u8 *)rec + 0x20);
        sub_803AD80((u8 *)mgr + off, (void *)label2, *(void **)((u8 *)rec + 0x24));
    }
}

extern u8 gStaticData_0816B138[];

/* NOT YET BYTE-MATCHING: same register-letter difficulty as
 * sub_80049CC above - the prologue/instruction shape is right but a
 * handful of scratch-register choices differ, and this function
 * additionally spills a constant through `ip` in the ROM. Draws
 * `value`'s label centered at Y=0x87, then draws a highlighted/plain
 * pair of fixed labels (0x29/0x2a, purpose unconfirmed) swapping
 * Y=0x87 vs Y=0x91 depending on `self->field_10` - each pair member's
 * slot gets a `gStaticData_0816B138` draw at its *previous* position
 * right before the real label, which reads as a clear/overwrite step
 * rather than a width probe (the return value is never used). */
void sub_8003D3C(struct pause_options_screen *self, s32 value)
{
    struct icon_manager **mgrAddr = &gUnknown_030012DC;
    struct icon_manager *mgr;
    struct icon_record *rec;
    s16 off;
    s32 str;
    s32 width;
    s32 highlight;

    sub_8028A30(*mgrAddr, 0);

    mgr = *mgrAddr;
    rec = *(struct icon_record **)((u8 *)mgr + (0x98 << 1));
    off = *(s16 *)((u8 *)rec + 0x10);
    str = sub_8026F38(value);
    width = sub_803AD80((u8 *)mgr + off, (void *)str, *(void **)((u8 *)rec + 0x14));
    mgr = *mgrAddr;
    mgr->posX = 0xa0 - width;
    mgr->posY = 0x87;
    rec = *(struct icon_record **)((u8 *)mgr + (0x98 << 1));
    off = *(s16 *)((u8 *)rec + 0x20);
    str = sub_8026F38(value);
    sub_803AD80((u8 *)mgr + off, (void *)str, *(void **)((u8 *)rec + 0x24));

    mgr = *mgrAddr;
    highlight = ((self->flags >> 2) & 1) ? 1 : 2;
    sub_8028A30(mgr, highlight);

    if (self->field_10 == 0) {
        mgr = *mgrAddr;
        rec = *(struct icon_record **)((u8 *)mgr + (0x98 << 1));
        off = *(s16 *)((u8 *)rec + 0x20);
        sub_803AD80((u8 *)mgr + off, gStaticData_0816B138, *(void **)((u8 *)rec + 0x24));
        mgr = *mgrAddr;
        mgr->posX = 0xb0;
        mgr->posY = 0x87;
        rec = *(struct icon_record **)((u8 *)mgr + (0x98 << 1));
        off = *(s16 *)((u8 *)rec + 0x20);
        str = sub_8026F38(0x29);
        sub_803AD80((u8 *)mgr + off, (void *)str, *(void **)((u8 *)rec + 0x24));
    } else {
        mgr = *mgrAddr;
        rec = *(struct icon_record **)((u8 *)mgr + (0x98 << 1));
        off = *(s16 *)((u8 *)rec + 0x20);
        sub_803AD80((u8 *)mgr + off, gStaticData_0816B138, *(void **)((u8 *)rec + 0x24));
        mgr = *mgrAddr;
        mgr->posX = 0xb0;
        mgr->posY = 0x91;
        rec = *(struct icon_record **)((u8 *)mgr + (0x98 << 1));
        off = *(s16 *)((u8 *)rec + 0x20);
        str = sub_8026F38(0x2a);
        sub_803AD80((u8 *)mgr + off, (void *)str, *(void **)((u8 *)rec + 0x24));
    }

    mgr = *mgrAddr;
    sub_8028A30(mgr, 0);
    if (self->field_10 == 0) {
        mgr = *mgrAddr;
        mgr->posX = 0xb0;
        mgr->posY = 0x91;
        rec = *(struct icon_record **)((u8 *)mgr + (0x98 << 1));
        off = *(s16 *)((u8 *)rec + 0x20);
        str = sub_8026F38(0x2a);
        sub_803AD80((u8 *)mgr + off, (void *)str, *(void **)((u8 *)rec + 0x24));
    } else {
        mgr = *mgrAddr;
        mgr->posX = 0xb0;
        mgr->posY = 0x87;
        rec = *(struct icon_record **)((u8 *)mgr + (0x98 << 1));
        off = *(s16 *)((u8 *)rec + 0x20);
        str = sub_8026F38(0x29);
        sub_803AD80((u8 *)mgr + off, (void *)str, *(void **)((u8 *)rec + 0x24));
    }
}

extern u8 sub_8002CE8(void *handle, s32 rowIndex);
extern void sub_8008890(void *arg0, s32 arg1, s32 arg2);
extern s32 itoa(s32 value, u8 *buffer, s32 base);

/* `rowObjA`/`rowObjB`/`rowObjC` entries (see pause_options_screen.h)
 * are small on-screen objects with just a Q8 `x`/`y` position at their
 * front - `sub_800450C` (this chunk's other remaining function,
 * currently still fully raw) allocates and positions them. */
struct row_obj {
    s32 x;
    s32 y;
};

/* NOT YET BYTE-MATCHING: same difficulty class as this file's other
 * parked functions (register-allocation nondeterminism around several
 * near-identical unrolled blocks) - genuinely left completely
 * untouched until this pass, real bytes still raw in
 * asm/code_3_1_10_4.s. Draws this settings row's three numeric stat
 * values - `statPtr->field_4`/`field_10`/`field_8` of the row's own
 * `struct settings_row_stats` (`statPtr` is `(&self->currentStats)
 * [rowIdx]`, i.e. `currentStats` and `rowStats[0..3]` read as one
 * contiguous 5-element array - `sub_8004860`/`sub_80048E0`,
 * `src/graphics/settings_menu2.c`, already establish `rowStats` as
 * this same array shape) - as plain decimal strings into
 * `self->rowObjA[rowIdx]`/`rowObjC[rowIdx]`/`rowObjB[rowIdx]`
 * respectively (each drawn via `gUnknown_030012DC`'s `record->slots[2]`
 * trampoline, and each preceded by the same highlight/dim
 * `sub_8028A30` call this chunk's other row-label functions already
 * establish - `sub_80041BC`'s own `flag` parameter selects which row
 * is "selected", matching that shared idiom). A fourth value
 * (`statPtr->field_0`) is formatted as `"NN%"` by `itoa`-ing then
 * manually scanning for the NUL terminator and overwriting it with a
 * literal `%` byte (re-terminating one byte later) - measured once via
 * `gUnknown_030012E0`'s `slots[0]` trampoline to get its pixel width,
 * then drawn a second time via that same manager's `slots[2]`
 * trampoline, right-aligned against `label1` using the measured
 * width (`posX = label1 - width + 0x1f`) - the standard
 * "measure, then right-align" idiom this ROM region uses throughout. */
void sub_8003F30(struct pause_options_screen *self, s32 label1, s32 label2, s32 rowIdx, u8 flag)
{
    struct settings_row_stats *statPtr =
        (struct settings_row_stats *)((u8 *)&self->currentStats + rowIdx * (s32)sizeof(struct settings_row_stats));
    struct row_obj *obj;
    struct icon_manager *mgr;
    struct icon_record *rec;
    u8 buf[12];
    s32 highlight = flag ? (((self->flags >> 2) & 1) ? 1 : 2) : 0;
    s32 width;
    s32 i;

    /* Row A: statPtr->field_4, object at (label1+0x2b, label2+5), text
     * drawn at (label1+0x38, label2). */
    obj = (struct row_obj *)self->rowObjA[rowIdx];
    obj->x = (label1 + 0x2b) << 8;
    obj->y = (label2 + 5) << 8;
    sub_8008890(obj, 0, 0);
    itoa(statPtr->field_4, buf, 10);
    sub_8028A30(gUnknown_030012DC, highlight);
    mgr = gUnknown_030012DC;
    mgr->posX = label1 + 0x38;
    mgr->posY = label2;
    rec = mgr->record;
    sub_803AD80((u8 *)mgr + rec->slots[2].offset, buf, rec->slots[2].ptr);

    /* Row C: statPtr->field_10, object at (label1+7, label2+0x1e), text
     * drawn at (label1+0x10, label2+0x17). */
    obj = (struct row_obj *)self->rowObjC[rowIdx];
    obj->x = (label1 + 7) << 8;
    obj->y = (label2 + 0x1e) << 8;
    sub_8008890(obj, 0, 0);
    itoa(statPtr->field_10, buf, 10);
    sub_8028A30(gUnknown_030012DC, highlight);
    mgr = gUnknown_030012DC;
    mgr->posX = label1 + 0x10;
    mgr->posY = label2 + 0x17;
    rec = mgr->record;
    sub_803AD80((u8 *)mgr + rec->slots[2].offset, buf, rec->slots[2].ptr);

    /* Row B: statPtr->field_8, object at (label1+0x2b, label2+0x1e),
     * text drawn at (label1+0x38, label2+0x17). */
    obj = (struct row_obj *)self->rowObjB[rowIdx];
    obj->x = (label1 + 0x2b) << 8;
    obj->y = (label2 + 0x1e) << 8;
    sub_8008890(obj, 0, 0);
    itoa(statPtr->field_8, buf, 10);
    sub_8028A30(gUnknown_030012DC, highlight);
    mgr = gUnknown_030012DC;
    mgr->posX = label1 + 0x38;
    mgr->posY = label2 + 0x17;
    rec = mgr->record;
    sub_803AD80((u8 *)mgr + rec->slots[2].offset, buf, rec->slots[2].ptr);

    /* Fourth value: statPtr->field_0 formatted as "NN%". */
    itoa(statPtr->field_0, buf, 10);
    for (i = 0; i <= 6; i++) {
        if (buf[i] == 0) {
            buf[i] = '%';
            buf[i + 1] = 0;
            break;
        }
    }

    /* Measure via gUnknown_030012E0's slots[0]. */
    mgr = gUnknown_030012E0;
    rec = mgr->record;
    width = sub_803AD80((u8 *)mgr + rec->slots[0].offset, buf, rec->slots[0].ptr);

    sub_8028A30(gUnknown_030012E0, highlight);

    /* Draw right-aligned against label1 via slots[2], at Y=label2-2. */
    mgr = gUnknown_030012E0;
    mgr->posX = label1 - width + 0x1f;
    mgr->posY = label2 - 2;
    rec = mgr->record;
    sub_803AD80((u8 *)mgr + rec->slots[2].offset, buf, rec->slots[2].ptr);
}

/* NOT YET BYTE-MATCHING: same register-allocation difficulty as
 * sub_80049CC, compounded by 4x unrolling in the ROM (this
 * reconstruction uses a small table + loop instead, which is
 * semantically faithful but can't reproduce the ROM's per-occurrence
 * register choices). Per docs/rom_map.md's "narrowed down which screen
 * overlay_ui is" section: one of 4 settings rows, `handle`/
 * `selectedIndex` from the 6-wrapper-caller family (sub_8004AA4 etc.,
 * src/graphics/settings_menu3.c). When `sub_8002CE8(handle, i)`
 * reports row `i` selected, draws a highlighted numeric glyph
 * (label 0x25) centered at the row's fixed position; otherwise draws
 * the row's normal label pair via sub_8003F30 (parked, NOT YET
 * BYTE-MATCHING, above in this file), flagged if `selectedIndex == i`. */
void sub_80041BC(struct pause_options_screen *self, void *handle, s32 selectedIndex)
{
    static const struct { s32 x, y, label1, label2, idx; } rows[4] = {
        { 0x43, 0x2d, 0x26, 0x21, 1 },
        { 0x43, 0x5f, 0x26, 0x53, 2 },
        { 0xa3, 0x2d, 0x86, 0x21, 3 },
        { 0xa3, 0x5f, 0x86, 0x53, 4 },
    };
    s32 i;

    for (i = 0; i < 4; i++) {
        if (sub_8002CE8(handle, i)) {
            s32 highlight = ((self->flags >> 2) & 1) ? 1 : 2;
            struct icon_manager *mgr;
            struct icon_record *rec;
            s16 off;
            s32 str;
            s32 width;
            s32 half;

            sub_8028A30(gUnknown_030012DC, highlight);

            mgr = gUnknown_030012DC;
            rec = *(struct icon_record **)((u8 *)mgr + (0x98 << 1));
            off = *(s16 *)((u8 *)rec + 0x10);
            str = sub_8026F38(0x25);
            width = sub_803AD80((u8 *)mgr + off, (void *)str, *(void **)((u8 *)rec + 0x14));
            half = width / 2;

            mgr = gUnknown_030012DC;
            mgr->posX = rows[i].x - half;
            mgr->posY = rows[i].y;
            rec = *(struct icon_record **)((u8 *)mgr + (0x98 << 1));
            off = *(s16 *)((u8 *)rec + 0x20);
            str = sub_8026F38(0x25);
            sub_803AD80((u8 *)mgr + off, (void *)str, *(void **)((u8 *)rec + 0x24));
        } else {
            u8 flag = (selectedIndex == i);
            sub_8003F30(self, rows[i].label1, rows[i].label2, rows[i].idx, flag);
        }
    }
}

/* NOT YET BYTE-MATCHING: same class of difficulty as sub_80049CC
 * above. `arg1`/`arg2` are plain coordinate values here (not pointers
 * - the ROM does raw integer arithmetic on them, `arg1+0x1d`/
 * `arg2+0xc`), used as the on-screen anchor for a centered numeric
 * glyph (label 0x25) into gUnknown_030012DC. */
void sub_8004914(struct pause_options_screen *self, s32 arg1, s32 arg2, u8 arg3)
{
    s32 x = arg1 + 0x1d;
    s32 y = arg2 + 0xc;
    struct icon_manager *mgr;
    struct icon_record *rec;
    s16 off;
    s32 str;
    s32 width;

    if (arg3) {
        s32 highlight = ((self->flags >> 2) & 1) ? 1 : 2;
        sub_8028A30(gUnknown_030012DC, highlight);
    } else {
        sub_8028A30(gUnknown_030012DC, 0);
    }

    mgr = gUnknown_030012DC;
    rec = *(struct icon_record **)((u8 *)mgr + (0x98 << 1));
    off = *(s16 *)((u8 *)rec + 0x10);
    str = sub_8026F38(0x25);
    width = sub_803AD80((u8 *)mgr + off, (void *)str, *(void **)((u8 *)rec + 0x14));

    mgr = gUnknown_030012DC;
    mgr->posX = x - width / 2;
    mgr->posY = y;
    rec = *(struct icon_record **)((u8 *)mgr + (0x98 << 1));
    off = *(s16 *)((u8 *)rec + 0x20);
    str = sub_8026F38(0x25);
    sub_803AD80((u8 *)mgr + off, (void *)str, *(void **)((u8 *)rec + 0x24));
}

extern void *sub_8026EDC(s32 size);
extern void sub_8002FCC(void *newObj, void *tmpl);
extern void sub_8002FD8(void *newObj);
extern void sub_80006A8(void);
extern void sub_80007AC(void *arg0);
extern void *gUnknown_03001304;
extern u32 gUnknown_030007E0;
extern u8 gUnknown_03000800;
extern void *gUnknown_03000804;
extern s32 sub_8001F50(void *arg0);
extern s32 sub_8002EFC(void *newObj);
extern s32 sub_8002FD4(void *newObj);
extern void sub_800014C(void *arg0, s32 arg1, s32 arg2);
extern void sub_8026ED0(void *newObj);

/* NOT YET BYTE-MATCHING. A "connecting..." SIO-handshake spinner
 * dialog: allocates a small icon object from self->field_8c's
 * template, then loops VBlank-waiting while polling input (cancel ->
 * state 3), the link-active flag gUnknown_03000800, and sub_8001F50
 * (the link-connection/handshake driver documented in
 * docs/rom_map.md's SIO/link-cable section) until the spinner object's
 * own state (sub_8002EFC) settles. Returns that state; when it settles
 * at 0, also feeds a result value through self->field_90 via
 * sub_800014C. */
s32 sub_8003B40(struct pause_options_screen *self)
{
    void *obj;
    s32 state;

    obj = sub_8026EDC(0x88 << 2);
    sub_8002FCC(obj, self->field_8c);
    sub_8002FD8(obj);

    for (;;) {
        u32 input;

        sub_80006A8();
        sub_80007AC(gUnknown_03001304);
        input = gUnknown_030007E0 & 2;
        if (input != 0) {
            state = 3;
            break;
        }
        if (gUnknown_03000800) {
            gUnknown_03000800 = 0;
            sub_8002FD8(obj);
        }
        sub_8001F50(gUnknown_03000804);
        state = sub_8002EFC(obj);
        if (state != 1) {
            break;
        }
    }

    if (state == 0) {
        s32 result = sub_8002FD4(obj);
        sub_800014C(self->field_90, result, 0x80 << 2);
    }
    sub_8026ED0(obj);
    return state;
}
#endif /* NON_MATCHING */
