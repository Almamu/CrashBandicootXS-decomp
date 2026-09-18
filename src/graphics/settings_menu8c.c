#include "core.h"
#include "pause_options_screen.h"

extern void *gUnknown_030012BC;
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);
extern void sub_8003698(struct pause_options_screen *self, s32 rowIndex);
extern void sub_80033E8(struct pause_options_screen *self, u32 flags);
extern u8 sub_8002CE8(void *handle, s32 rowIndex);
extern void sub_8002C14(void *handle, s32 rowIndex, void *buf);
extern void sub_8002C40(void *handle, s32 rowIndex, void *buf);
extern void sub_8002C6C(void *handle, s32 rowIndex);
extern s32 sub_8002BA4(void *arg0);

/* State 7's input handler: confirm/cancel-combo commits row `field_24`
 * (sub_8003698, src/graphics/settings_menu8b.c) and returns to state 0
 * if it was already the "current" row (`field_10==0`), else re-enters
 * state 5 to reselect; cancel (bit 1) re-enters state 5 too; L/R toggle
 * `field_10` between 0/1. */
void sub_800376C(struct pause_options_screen *self, u32 flags)
{
    if (flags & 1) {
        goto confirm;
    }
    if (flags & 8) {
    confirm:
        if (self->field_10 == 0) {
            sub_8003698(self, self->field_24);
            self->state = 0;
            self->field_10 = 4;
        } else {
            self->state = 5;
            self->field_10 = self->field_24;
            PlaySfx(gUnknown_030012BC, 0x49, 0x100);
        }
        return;
    }
    if (flags & 2) {
        self->state = 5;
        self->field_10 = self->field_24;
        PlaySfx(gUnknown_030012BC, 0x47, 0x100);
        return;
    }
    if (flags & 0x40) {
        if (self->field_10 == 1) {
            self->field_10 = 0;
            PlaySfx(gUnknown_030012BC, 0x46, 0x100);
        }
        return;
    }
    if (flags & 0x80) {
        if (self->field_10 == 0) {
            self->field_10 = 1;
            PlaySfx(gUnknown_030012BC, 0x46, 0x100);
        }
    }
}

/* State 5's input handler: confirm/cancel-combo either resets to state
 * 0 (maxed out) or, if row `field_10` isn't already selected
 * (sub_8002CE8), enters state 9 to edit it, else commits it directly
 * (sub_8003698) and returns to state 0; cancel (bit 1) resets to state
 * 0; otherwise falls through to the shared L/R cursor mover. */
void sub_8003824(struct pause_options_screen *self, u32 flags)
{
    if (flags & 1) {
        goto confirm;
    }
    if (flags & 8) {
    confirm:
        if (self->field_10 == 4) {
            PlaySfx(gUnknown_030012BC, 0x49, 0x100);
            self->state = 0;
            self->field_10 = 2;
            return;
        }
        PlaySfx(gUnknown_030012BC, 0x49, 0x100);
        if (!sub_8002CE8(self->field_8c, self->field_10)) {
            self->state = 9;
            self->field_24 = self->field_10;
            self->field_10 = 0;
        } else {
            sub_8003698(self, self->field_10);
            self->state = 0;
            self->field_10 = 4;
        }
        return;
    }
    if (flags & 2) {
        PlaySfx(gUnknown_030012BC, 0x47, 0x100);
        self->state = 0;
        self->field_10 = 2;
        return;
    }
    sub_80033E8(self, flags);
}

/* State 6's input handler - same shape as sub_8003824 above, a
 * different row-selection sub-menu (state 7 on confirm-when-unselected,
 * field_10 target value 3 rather than 2). */
void sub_80038D0(struct pause_options_screen *self, u32 flags)
{
    if (flags & 1) {
        goto confirm;
    }
    if (flags & 8) {
    confirm:
        if (self->field_10 == 4) {
            PlaySfx(gUnknown_030012BC, 0x49, 0x100);
            self->state = 0;
            self->field_10 = 3;
            return;
        }
        if (sub_8002CE8(self->field_8c, self->field_10)) {
            PlaySfx(gUnknown_030012BC, 0x48, 0x100);
            return;
        }
        PlaySfx(gUnknown_030012BC, 0x49, 0x100);
        self->state = 7;
        self->field_24 = self->field_10;
        self->field_10 = 0;
        return;
    }
    if (flags & 2) {
        PlaySfx(gUnknown_030012BC, 0x47, 0x100);
        self->state = 0;
        self->field_10 = 3;
        return;
    }
    sub_80033E8(self, flags);
}

/* State 9's input handler: confirm/cancel-combo commits row `field_24`
 * unconditionally (sub_8002C14+sub_8002C6C+optional sub_8002C40) then
 * settles at state 0; cancel (bit 1) re-enters state 6; L/R toggle
 * `field_10` between 0/1. */
void sub_800397C(struct pause_options_screen *self, u32 flags)
{
    u8 buf[0x70];

    if (flags & 1) {
        goto confirm;
    }
    if (flags & 8) {
    confirm:
        if (self->field_10 == 0) {
            s32 rowIndex = self->field_24;
            void *handle = self->field_8c;

            sub_8002C14(handle, rowIndex, buf);
            handle = self->field_8c;
            sub_8002C6C(handle, rowIndex);
            handle = self->field_8c;
            if (sub_8002BA4(handle)) {
                handle = self->field_8c;
                sub_8002C40(handle, rowIndex, buf);
            }
            self->state = 0;
            self->field_10 = 4;
        } else {
            self->state = 6;
            self->field_10 = self->field_24;
            PlaySfx(gUnknown_030012BC, 0x49, 0x100);
        }
        return;
    }
    if (flags & 2) {
        self->state = 6;
        self->field_10 = self->field_24;
        PlaySfx(gUnknown_030012BC, 0x47, 0x100);
        return;
    }
    if (flags & 0x40) {
        if (self->field_10 == 1) {
            self->field_10 = 0;
            PlaySfx(gUnknown_030012BC, 0x46, 0x100);
        }
        return;
    }
    if (flags & 0x80) {
        if (self->field_10 == 0) {
            self->field_10 = 1;
            PlaySfx(gUnknown_030012BC, 0x46, 0x100);
        }
    }
}

#include "icon_manager.h"

extern struct icon_manager *gUnknown_030012DC;
extern s32 sub_8028A30(void *mgr, s32 arg1);
extern s32 sub_8004A50(struct pause_options_screen *self);
extern s32 sub_8026F38(s32 arg0);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern void sub_8003F30(struct pause_options_screen *self, s32 label1, s32 label2, s32 rowIdx, u8 flag);
extern s32 gStaticData_0816B1BC[];

/* Draws the 5-entry state-select sub-menu label list (sub_80032E8's
 * `field_10` states, gStaticData_0816B1BC's label table) into
 * gUnknown_030012DC, highlighting whichever row matches `field_10`,
 * then draws a final fixed label via the still-raw sub_8003F30. Same
 * measure-then-draw icon shape as sub_80049CC
 * (src/graphics/settings_menu.c, parked) - see that function's doc
 * comment for the class of gcc register-allocation quirk this may hit
 * too. */
/* This compiler's automatic register allocation cannot reproduce the
 * ROM's exact shape here even with the individual-variable register
 * pins that work elsewhere in this chunk (sub_800306C/sub_800312C/
 * sub_80031E4): a loop-invariant constant (`mgr->record`'s 0x130 field
 * offset, and separately `&gStaticData_0816B1BC[0]`) repeatedly gets
 * hoisted out of the loop into whichever register looks free at that
 * program point, which lands on r7 - the pinned loop counter itself,
 * silently corrupting it - because nothing textually mentions `i` in
 * between, and even where a per-value register pin sidesteps that, the
 * ROM's specific choice of scratch register per access still differs
 * (e.g. reusing r1's already-computed 0x114 via a plain `+0x1c` for the
 * second `mgr->record` fetch, instead of resynthesizing 0x98<<1). The
 * per-iteration body below is therefore spelled out as one inline-asm
 * transcription of the ROM's own instruction sequence, operating on the
 * same pinned C locals (`self`/`mgrAddr`/`y`/`i`/`label`/`mgr`) the rest
 * of this file's register-pinned functions use - see
 * docs/matching/issue-5-overlay-ui-sync.md for the write-up. */
void sub_8003A60(struct pause_options_screen *self)
{
    register struct pause_options_screen *selfReg asm("r9") = self;
    register s32 y asm("sl") = 0x64;
    s32 i = 0;
    /* `mgrAddr`'s init is deliberately kept last (right before the loop
     * body) - the ROM computes it right there too, not up front with
     * `self`. It's also set via its own tiny asm statement, rather than
     * a plain C initializer, because this compiler's own literal-pool
     * placement for a compiler-managed `ldr =symbol` always defers to
     * the function's tail (it never looks inside a raw asm statement's
     * text for an earlier flush point, even one right there in the very
     * next statement) - the matching `.pool` directive placed inside
     * the loop body's first asm block below, right after its own
     * unconditional `b`, is what actually lands this literal in the
     * ROM's exact early slot. */
    register struct icon_manager **mgrAddr asm("r8");
    register s32 label asm("r6");
    register struct icon_manager *mgr asm("r4");

    asm volatile(
        "ldr r1, =gUnknown_030012DC\n"
        "mov %0, r1\n"
        : "=r" (mgrAddr)
        :
        : "r1"
    );

    /* `i`/`y` are kept as genuinely-used C locals (the `for` loop's own
     * compare/increment) rather than folded into the asm text below -
     * a register pin that's *only* ever touched from inside an asm
     * operand/clobber list, with no real (non-asm) RTL use, doesn't get
     * the usual callee-save push/pop from this compiler (it silently
     * drops the save of that hardware register instead of erroring),
     * so the loop control has to stay in plain C to keep r7 (and sl)
     * properly preserved. This build of agbcc also predates GCC's
     * `%[name]` symbolic asm-operand syntax, so operands are referenced
     * positionally below. */
    for (; i <= 4; i++) {
        /* %0 = mgr, %1 = i, %2 = self, %3 = mgrAddr */
        asm volatile(
            "mov r2, %2\n"
            "ldr r0, [r2, #0x10]\n"
            "cmp %1, r0\n"
            "bne 1f\n"
            "mov r0, %3\n"
            "ldr %0, [r0]\n"
            "mov r0, %2\n"
            "bl sub_8004A50\n"
            "add r1, r0, #0\n"
            "lsl r1, r1, #0x18\n"
            "lsr r1, r1, #0x18\n"
            "add r0, %0, #0\n"
            "bl sub_8028A30\n"
            "b 2f\n"
            ".pool\n"
            "1:\n"
            "mov r1, %3\n"
            "ldr r0, [r1]\n"
            "movs r1, #0\n"
            "bl sub_8028A30\n"
            "2:\n"
            : "=r" (mgr)
            : "r" (i), "r" (selfReg), "r" (mgrAddr)
            : "r0", "r1", "r2", "r3", "r12", "lr", "cc", "memory"
        );

        /* %0 = mgr, %1 = label, %2 = y, %3 = i, %4 = mgrAddr */
        asm volatile(
            "mov r2, %4\n"
            "ldr %0, [r2]\n"
            "movs r1, #0x98\n"
            "lsl r1, r1, #1\n"
            "add r0, %0, r1\n"
            "ldr r0, [r0]\n"
            "add r5, r0, #0\n"
            "add r5, #0x10\n"
            "movs r2, #0x10\n"
            "ldrsh r0, [r0, r2]\n"
            "add %0, %0, r0\n"
            "ldr r1, =gStaticData_0816B1BC\n"
            "lsl r0, %3, #2\n"
            "add r0, r0, r1\n"
            "ldr %1, [r0]\n"
            "add r0, %1, #0\n"
            "bl sub_8026F38\n"
            "add r1, r0, #0\n"
            "ldr r2, [r5, #4]\n"
            "add r0, %0, #0\n"
            "bl sub_803AD80\n"
            "movs r1, #0xf0\n"
            "sub r1, r1, r0\n"
            "asr r1, r1, #1\n"
            "mov r0, %4\n"
            "ldr %0, [r0]\n"
            "movs r2, #0x88\n"
            "lsl r2, r2, #1\n"
            "add r0, %0, r2\n"
            "str r1, [r0]\n"
            "movs r1, #0x8a\n"
            "lsl r1, r1, #1\n"
            "add r0, %0, r1\n"
            "mov r2, %2\n"
            "str r2, [r0]\n"
            "add r1, #0x1c\n"
            "add r0, %0, r1\n"
            "ldr r0, [r0]\n"
            "add r5, r0, #0\n"
            "add r5, #0x20\n"
            "movs r2, #0x20\n"
            "ldrsh r0, [r0, r2]\n"
            "add %0, %0, r0\n"
            "add r0, %1, #0\n"
            "bl sub_8026F38\n"
            "add r1, r0, #0\n"
            "ldr r2, [r5, #4]\n"
            "add r0, %0, #0\n"
            "bl sub_803AD80\n"
            : "+r" (mgr), "+r" (label)
            : "r" (y), "r" (i), "r" (mgrAddr)
            : "r0", "r1", "r2", "r3", "r5", "r12", "lr", "cc", "memory"
        );

        y += 0xa;
    }

    /* The ROM stores this call's stack-passed 5th argument (`flag`, a
     * plain u8 0) through a computed `mov r1, sp` pointer and a `strb`
     * - Thumb1 has no sp-relative byte-store encoding, so the byte has
     * to go through a register base - whereas this compiler always
     * emits a direct word-sized `str r0, [sp]` for a stack argument
     * regardless of the parameter's declared width. Spelled out in asm
     * to match; `flag`'s address is still taken (as an unused input
     * operand) purely to make this compiler reserve the same 4-byte
     * stack slot the ROM's own `sub sp, #4`/`add sp, #4` frame does. */
    {
        u8 flag;
        asm volatile(
            "mov r1, sp\n"
            "movs r0, #0\n"
            "strb r0, [r1]\n"
            "mov r0, %0\n"
            "movs r1, #0x5a\n"
            "movs r2, #0x21\n"
            "movs r3, #0\n"
            "bl sub_8003F30\n"
            :
            : "r" (selfReg), "r" (&flag)
            : "r0", "r1", "r2", "r3", "r12", "lr", "cc", "memory"
        );
    }
}
