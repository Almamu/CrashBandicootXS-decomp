#include "core.h"

/* All three fixed per-type function-pointer constants docs/audio.md
 * records for the GAX2_SoundHandler "Info" type (init_fn/unknown_fn/
 * play_fn = 0x080393FD/0x08039439/0x0803943D) live in this cluster:
 * sub_80393FC (init_fn), nullsub_39 (unknown_fn, a no-op stub), and
 * sub_803943C (play_fn) are all matched here. The object these operate
 * on isn't confidently modeled yet (same situation as sub_80381FC's
 * constructor in sound_object_init.c) - kept as raw offsets rather than
 * a guessed struct. */

/* Resets the shared per-instance state every SoundHandler "Info"-type
 * object carries, regardless of which higher-level init function called
 * it (sub_80393FC below, and sub_803941C, both reset different subsets
 * of fields first and then fall through to this shared core). */
void sub_80393D0(void *self)
{
    u8 *p = self;
    u16 val;
    u8 zeroByte;
    register u16 zeroHalf asm("r3");

    /* 0xFFFF/0x4E20 need to go through a named temp before the store -
     * assigning the literal straight to the dereferenced address loads
     * it into a scratch register and copies that into the real
     * destination register first, one instruction more than the ROM's
     * direct `ldr r0,=...; strh r0,[...]` (same gotcha documented on
     * sub_80381FC in sound_object_init.c). The two zero-fill temps
     * (`zeroByte`/`zeroHalf`) both get set right after the first store,
     * matching the ROM's `movs r2,#0; movs r3,#0` pair, rather than
     * being zeroed right before each individual use. */
    val = 0xFFFF;
    *(u16 *)(p + 0x14) = val;
    zeroByte = 0;
    zeroHalf = 0;
    val = 0x4E20;
    *(u16 *)(p + 0x16) = val;
    *(u8  *)(p + 0x1c) = zeroByte;
    *(u16 *)(p + 0x18) = 6;
    *(u8  *)(p + 0x1f) = 0xff;
    *(u8  *)(p + 0x1d) = zeroByte;
    *(u8  *)(p + 0x1e) = zeroByte;
    *(u8  *)(p + 0x22) = zeroByte;
    *(u16 *)(p + 0x24) = zeroHalf;
}

extern void sub_80393D0(void *self);

/* GAX2_SoundHandler "Info" type's init_fn (ROM 0x080393FD, see
 * docs/audio.md). */
void sub_80393FC(void *self)
{
    u8 *p = self;

    *(u32 *)(p + 0x10) = 0;
    *(u32 *)(p + 0xc) = 0;
    *(u8 *)(p + 0x1a) = 0;
    *(u8 *)(p + 0x1b) = 0;
    *(u8 *)(p + 0x20) = 0;
    *(u8 *)(p + 0x21) = 0;
    sub_80393D0(self);
}

/* Same reset, but with a different set of "armed" flags left set
 * afterwards - not yet confirmed which caller uses this variant over
 * sub_80393FC above. */
void sub_803941C(void *self)
{
    u8 *p = self;
    u32 zero;

    sub_80393D0(self);
    zero = 0;
    *(u8 *)(p + 0x1b) = 2;
    *(u32 *)(p + 0x10) = zero;
    *(u8 *)(p + 0x1a) = 1;
}

/* GAX2_SoundHandler "Info" type's unknown_fn (ROM 0x08039439, see
 * docs/audio.md) - a no-op stub. */
void nullsub_39(void)
{
}
asm(".align 2, 0");

/* GAX2_SoundHandler "Info" type's play_fn (ROM 0x0803943D, see
 * docs/audio.md's per-type function-pointer table). A per-tick repeat/
 * retrigger scheduler for the "Info" handler: bails out immediately
 * (still returning 0) unless this is a new channel (self->0xc != the
 * chanArg argument) and the handler is armed (self->0x1a != 0). When
 * armed and self->0x18(byte)!=0 && self->0x1c(byte)==0, it advances a
 * shared 8.8-style rate/counter pair (self->0x16, self->0x18) - either
 * reloading self->0x16 from self->0->0x18's table (a still-unmodeled
 * pointer chain, same situation as the other "Info"/"Channel" functions
 * in this file) when a one-shot retrigger flag (self->0x22) is set, or
 * simply incrementing it - then folds self->0x18's high byte back into
 * its low byte if present, decrements self->0x1c from self->0x18's low
 * byte, and compares self->0x16 against two more thresholds from the
 * same table (self->0->0x18 +2/+4/+6) to arm/disarm "loop point hit"
 * flags at self->0x1e/self->0x21 and reset self->0x14. The other branch
 * (self->0x18==0 or self->0x1c!=0) just decrements self->0x1c and clears
 * self->0x1d. Finally records the new chanArg into self->0xc (and
 * self->0x10, if that's still unset) and decrements self->0x1b if
 * armed. The chained object at self->0 isn't understood well enough yet
 * to give it a named struct (same as sub_8038F94/sub_80393D0's
 * situation) - kept as raw offsets.
 *
 * Several compiler-quirk fixups were needed to match:
 * - The `self->0x1c==0` check's "known zero" value needs materializing
 *   into its own register *only after* confirming `self->0x18(byte)!=0`
 *   (nested `if`, not a single `&&`) - the ROM only computes this copy
 *   inside the outer branch, then reuses it both for the second
 *   comparison and (via `goto`) as the literal 0 later stored into
 *   self->0x22.
 * - The two `self->0x16`/`self->0x14` reads compared against the
 *   self->0->0x18 table are 16-bit *signed* loads at a non-immediate
 *   offset (0x16/0x14) - Thumb's `ldrsh` has no immediate-offset form,
 *   only register-offset, and gcc's default codegen for this pattern
 *   picks the wrong register pairing (and a redundant sign-extend
 *   masking pass) compared to the ROM's `movs r0,#0x16; ldrsh
 *   r1,[r3,r0]` shape - transcribed as a tiny raw-asm block per read to
 *   force the ROM's exact register choice.
 * - The self->0x1c/self->0x1d "not retriggering" fallback path needs its
 *   decrement computed into a *fresh* register (pinned to r0) rather
 *   than modified in place on the register self->0x1c's byte was loaded
 *   into - otherwise gcc reuses that register directly instead of the
 *   ROM's separate `subs r0,r1,#1` / `movs r1,#0` pair. */
u32 sub_803943C(void *self, u32 arg1, u32 chanArg)
{
    u8 *p = self;
    register u32 c asm("r6") = chanArg;
    u8 b18lo;
    register u16 h18 asm("r4");
    register u8 b1c asm("r1");

    if (*(u32 *)(p + 0xc) == c) {
        return 0;
    }
    if (*(u8 *)(p + 0x1a) == 0) {
        return 0;
    }

    b18lo = *(u8 *)(p + 0x18);
    h18 = *(u16 *)(p + 0x18);
    b1c = *(u8 *)(p + 0x1c);

    if (b18lo != 0) {
    u32 zero = b1c;
    if (zero == 0) {
        u8 *flag = p + 0x22;

        if (*flag != 0) {
            *flag = zero;
            *(u16 *)(p + 0x16) = *(u16 *)((u8 *)(*(void **)((u8 *)(*(void **)p) + 0x18)) + 2);
        } else {
            *(u16 *)(p + 0x16) = *(u16 *)(p + 0x16) + 1;
        }

        {
            u16 v18 = *(u16 *)(p + 0x18);
            u16 lo = v18 >> 8;
            if (lo != 0) {
                register u16 mask asm("r0") = 0xff;
                register u32 hi asm("r0");
                hi = mask & v18;
                hi <<= 8;
                lo |= hi;
                *(u16 *)(p + 0x18) = lo;
            }
        }

        {
            u8 dec = *(u8 *)(p + 0x18) - 1;
            h18 = 0;
            *(u8 *)(p + 0x1c) = dec;

            {
                register s32 cnt asm("r1");
                void *base0;
                u16 thresh;

                asm("movs r0, #0x16\n\tldrsh r1, [%1, r0]" : "=r"(cnt) : "r"(p) : "r0");
                base0 = *(void **)p;
                thresh = *(u16 *)((u8 *)(*(void **)((u8 *)base0 + 0x18)) + 2);

                if (!(cnt < thresh)) {
                    u8 one;
                    register s32 cnt2 asm("r1");
                    u16 thresh2;

                    *(u16 *)(p + 0x16) = h18;
                    *(u16 *)(p + 0x24) = h18;
                    one = 1;
                    *(u8 *)(p + 0x1e) = one;
                    *(u16 *)(p + 0x14) = *(u16 *)(p + 0x14) + 1;

                    asm("movs r0, #0x14\n\tldrsh r1, [%1, r0]" : "=r"(cnt2) : "r"(p) : "r0");
                    thresh2 = *(u16 *)((u8 *)(*(void **)((u8 *)base0 + 0x18)) + 4);

                    if (!(cnt2 < thresh2)) {
                        if (*(u8 *)(p + 0x20) != 0) {
                            *(u8 *)(p + 0x1a) = 0;
                            *(u16 *)(p + 0x18) = h18;
                        }
                        *(u8 *)(p + 0x21) = one;
                        *(u16 *)(p + 0x14) = *(u16 *)((u8 *)(*(void **)((u8 *)(*(void **)p) + 0x18)) + 6);
                    }
                } else {
                    *(u8 *)(p + 0x1e) = h18;
                }
            }
        }
        *(u8 *)(p + 0x1d) = 1;
        h18 = *(u16 *)(p + 0x18);
        goto after_retrigger;
    }
    }
    {
        register u8 dec asm("r0") = b1c - 1;
        u8 zeroB = 0;
        *(u8 *)(p + 0x1c) = dec;
        *(u8 *)(p + 0x1d) = zeroB;
    }
after_retrigger:

    if ((h18 & 0xff) == 0) {
        *(u8 *)(p + 0x21) = 1;
    }

    *(u32 *)(p + 0xc) = c;
    if (*(u32 *)(p + 0x10) == 0) {
        *(u32 *)(p + 0x10) = c;
    }
    if (*(u8 *)(p + 0x1b) != 0) {
        *(u8 *)(p + 0x1b) = *(u8 *)(p + 0x1b) - 1;
    }

    return 0;
}
