#include "core.h"

/* Three of the fixed per-type function-pointer constants docs/audio.md
 * records for the GAX2_SoundHandler "Info" type (init_fn/unknown_fn/
 * play_fn = 0x080393FD/0x08039439/0x0803943D) live in this cluster:
 * sub_80393FC (init_fn) and nullsub_39 (unknown_fn, a no-op stub) are
 * matched here; play_fn (sub_803943C) is still raw. The object these
 * operate on isn't confidently modeled yet (same situation as
 * sub_80381FC's constructor in sound_object_init.c) - kept as raw
 * offsets rather than a guessed struct. */

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
