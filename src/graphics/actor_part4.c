#include "core.h"
#include "actor.h"
#include "vram_pool.h"

struct aabb {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_c;
};

extern void sub_803AFE4(void *buf, s32 arg1, s32 arg2);
extern void sub_803AFDC(void *buf, s32 arg1, s32 arg2);
extern u8 sub_8001688(void *buf1, void *buf2);

/* Builds `part`'s AABB (same keyframe-table shape/record layout as
 * sub_8007B98, inlined directly here rather than calling it - this
 * function needs the box on the stack for the final overlap test, not
 * written out through a `dest` pointer) and mirrors it per
 * `part+0x28` bits 4/5, then tests it for overlap against `region` via
 * `sub_8001688` - the same collision-test function `sub_8007DBC`
 * uses. */
s32 sub_80080C0(struct actor *part, void *region)
{
    struct aabb buf_;
    void **tablePtr;
    register void *rec asm("r0");
    register u8 idx asm("r3");
    void *rec4;
    s32 offset;
    s32 offX, offY;
    s32 w, h;
    s32 x, y;
    s32 xpos, ypos;
    u8 flags;
    s32 mirrorX, mirrorY;

    flags = *((u8 *)part + 0x28);
    mirrorX = ((u32)flags << 27) >> 31;
    mirrorY = ((u32)flags << 26) >> 31;

    xpos = *(s32 *)part >> 8;
    ypos = *(s32 *)((u8 *)part + 4) >> 8;

    tablePtr = *(void ***)((u8 *)part + 0x20);
    part = (struct actor *)((u8 *)part + 0x2d);
    idx = *(u8 *)part;
    offset = idx * 0x1c;
    rec = *tablePtr;
    rec = (u8 *)rec + offset;
    rec4 = (u8 *)rec + 4;

    offX = *(s16 *)((u8 *)rec + 4);
    offY = *(s16 *)((u8 *)rec4 + 2);
    w = *((u8 *)rec4 + 4);
    h = *((u8 *)rec4 + 5);

    x = offX + xpos;
    y = offY + ypos;
    sub_803AFE4(&buf_, x, y);
    sub_803AFDC(&buf_, w, h);

    if (mirrorX) {
        buf_.field_0 = xpos * 2 - (buf_.field_0 + buf_.field_8);
    }
    if (mirrorY) {
        buf_.field_4 = ypos * 2 - (buf_.field_4 + buf_.field_c);
    }

    return (u8)sub_8001688(&buf_, region);
}

extern u8 sub_8006DF8(struct tile_asset_cache *self, s32 recordId);
extern struct tile_asset_cache *gUnknown_030012B8;

/* Reads `part`'s current keyframe record's `+0x14` byte as a
 * `sub_8006DF8` record id, looked up against the global tile-asset
 * cache `gUnknown_030012B8`. */
s32 sub_800815C(struct actor *part)
{
    struct tile_asset_cache *cache = gUnknown_030012B8;
    void **tablePtr;
    void *table;
    register u8 idx asm("r4");
    register s32 rec asm("r1");

    tablePtr = *(void ***)((u8 *)part + 0x20);
    part = (struct actor *)((u8 *)part + 0x2d);
    table = *tablePtr;
    idx = *(u8 *)part;
    rec = idx * 0x1c;
    rec = rec + (s32)table;
    rec = *((u8 *)rec + 0x14);

    return (u8)sub_8006DF8(cache, rec);
}

/* Adjusts `dest`'s `{s32 field_0, field_4}` (a position, working
 * theory) per `kind` (`kind-1` is the real switch selector, 0-11;
 * anything else - including the four explicit no-op cases 2/4/5/6/8/9/
 * 10 - does nothing): kind 1/2 add/subtract `rec+4`'s byte (Q8,
 * shifted by 7 not 8 - half-Q8?) from `dest->field_0`; kind 4
 * subtracts `rec+2`'s signed 16-bit value (shifted by 8, full Q8) from
 * `dest->field_4`; kinds 8/12 do the same but add `rec+5`'s byte to
 * the `rec+2` value first. `dest`/`rec` kept raw - neither type is
 * established yet.
 *
 * Matched in a later session than the original NAKED transcription
 * (see docs/matching.md's "Parked, not matched: sub_8008188" for the
 * original account and the five techniques that failed against it).
 * The holdout was always the shared kind-8/12 block's register-register
 * `add` (ROM's `adds r1, r2, r1` versus this compiler's always-
 * canonicalized `adds r1, r1, r2`), and an inline-asm anchor for just
 * that one instruction had previously broken the kind-8/12 case-body
 * merging (the compiler stopped recognizing the two switch cases'
 * bodies as identical once one of them contained an opaque asm
 * statement, so it quit merging them into one physical block). The fix
 * that finally worked: route both `case 8` and `case 12` to one shared
 * `goto` target (`kind8_12`) that holds the *entire* load+load+add
 * sequence as a single atomic `asm volatile` block, rather than two
 * switch-case bodies the optimizer has to independently notice are
 * identical - there is exactly one occurrence of this code at the C
 * source level, so there is nothing left for a cross-jump/tail-merging
 * pass to decide about. Every other switch case is likewise routed
 * through a bare `goto` to its own labeled block outside the switch
 * (rather than holding real code inside the switch itself) - this
 * turned out to matter for exact block *ordering* too (see
 * `sub_8008278` below, where a `case` body with real code physically
 * displaced a fallthrough block gcc would otherwise have placed
 * directly after its neighbor). */
void sub_8008188(void *dest, s32 kind, void *rec)
{
    s32 idx = kind - 1;

    switch (idx) {
    case 1:
        {
            register s32 byteVal asm("r2") = *((u8 *)rec + 4);
            register s32 shifted asm("r1");

            shifted = byteVal << 7;
            *(s32 *)dest += shifted;
        }
        goto end;
    case 0:
        {
            register s32 byteVal asm("r2") = *((u8 *)rec + 4);
            register s32 shifted asm("r1");

            shifted = byteVal << 7;
            *(s32 *)dest -= shifted;
        }
        goto end;
    case 2:
        goto end;
    case 3:
        {
            s32 v = *(s16 *)((u8 *)rec + 2);
            v <<= 8;
            *(s32 *)((u8 *)dest + 4) -= v;
        }
        goto end;
    case 4:
        goto end;
    case 5:
        goto end;
    case 6:
        goto end;
    case 7:
        goto kind8_12;
    case 8:
        goto end;
    case 9:
        goto end;
    case 10:
        goto end;
    case 11:
        goto kind8_12;
    default:
        goto end;
    }

kind8_12:
    {
        register void *recReg asm("r2") = rec;
        register s32 result asm("r1");

        asm volatile(
            "mov r0, #2\n\t"
            "ldrsh r1, [r2, r0]\n\t"
            "ldrb r2, [r2, #5]\n\t"
            "add r1, r2, r1\n\t"
            : "=r"(result), "+r"(recReg)
            :
            : "r0"
        );

        result <<= 8;
        *(s32 *)((u8 *)dest + 4) -= result;
    }
end:
    return;
}

/* Same shape as `sub_8008188` above (mirror-image add/subtract
 * directions: kind 1/2 do the opposite sign on `dest->field_0`, and
 * kinds 4/8/12 add to `dest->field_4` instead of subtracting).
 *
 * Matched in a later session, same `goto`-unified-block technique as
 * `sub_8008188` above - see its doc comment for the full account of
 * why the shared kind-8/12 block needs a single atomic `asm volatile`
 * reached via `goto` from both `case 8` and `case 12`, rather than an
 * asm anchor on just the `add` inside two ordinary switch-case
 * bodies. */
void sub_8008200(void *dest, s32 kind, void *rec)
{
    s32 idx = kind - 1;

    switch (idx) {
    case 1:
        {
            register s32 byteVal asm("r2") = *((u8 *)rec + 4);
            register s32 shifted asm("r1");

            shifted = byteVal << 7;
            *(s32 *)dest -= shifted;
        }
        goto end;
    case 0:
        {
            register s32 byteVal asm("r2") = *((u8 *)rec + 4);
            register s32 shifted asm("r1");

            shifted = byteVal << 7;
            *(s32 *)dest += shifted;
        }
        goto end;
    case 2:
        goto end;
    case 3:
        {
            s32 v = *(s16 *)((u8 *)rec + 2);
            v <<= 8;
            *(s32 *)((u8 *)dest + 4) += v;
        }
        goto end;
    case 4:
        goto end;
    case 5:
        goto end;
    case 6:
        goto end;
    case 7:
        goto kind8_12;
    case 8:
        goto end;
    case 9:
        goto end;
    case 10:
        goto end;
    case 11:
        goto kind8_12;
    default:
        goto end;
    }

kind8_12:
    {
        register void *recReg asm("r2") = rec;
        register s32 result asm("r1");

        asm volatile(
            "mov r0, #2\n\t"
            "ldrsh r1, [r2, r0]\n\t"
            "ldrb r2, [r2, #5]\n\t"
            "add r1, r2, r1\n\t"
            : "=r"(result), "+r"(recReg)
            :
            : "r0"
        );

        result <<= 8;
        *(s32 *)((u8 *)dest + 4) += result;
    }
end:
    return;
}

/* A third variant of `sub_8008188`'s shape: kind 1/2 update
 * `dest->field_0` (sub/add) AND unconditionally also add `rec+2`'s
 * short (Q8) to `dest->field_4`; kinds 4/8/12 add to `dest->field_4`
 * (same as `sub_8008188`'s kinds) AND additionally always subtract
 * `rec+4`'s byte (Q8, `<<7`) from `dest->field_0` afterward.
 *
 * Matched in a later session, same `goto`-unified atomic-asm-block
 * technique as `sub_8008188` above for the shared kind-8/12 gap.
 * `rec` itself also needs its own explicit `register ... asm("r2")`
 * pin (initialized from the incoming parameter right at function
 * entry) - without it, gcc decided `rec` needed to survive in a
 * callee-saved register (`r4`, behind a `push {r4, lr}`/`pop {r4}`
 * the ROM doesn't have) once its live range was forced to span every
 * `goto`-connected block by this restructuring; pinning it to `r2` up
 * front (its own natural incoming register, and the register the ROM
 * itself keeps it in end to end) removed the need for that spill
 * entirely. Also needed every switch case - even the two with real
 * code (`case 0`/`case 1`) - to hold nothing but a bare `goto` to a
 * block declared *outside* the switch, rather than the code directly:
 * an earlier draft that gave `case 0`'s body directly (so it could
 * fall through to the shared field_4 update without an explicit
 * branch, matching the ROM) still had `case 3`'s real code physically
 * appear between the `case 0`/`case 1` blocks and that shared
 * fallthrough target in the compiler's block layout (gcc places a
 * switch's in-place case bodies in switch-declaration order, ahead of
 * any post-switch fallthrough code, regardless of where a `goto`
 * target label textually sits) - forcing a spurious extra branch
 * where the ROM has a genuine fallthrough. Moving every case's real
 * code out to its own `goto` target, laid out in the exact order the
 * ROM's own blocks appear, fixed the layout without changing anything
 * about the kind-8/12 fix itself. */
void sub_8008278(void *dest, s32 kind, void *rec_)
{
    register void *rec asm("r2") = rec_;
    register s32 field0 asm("r0");
    s32 idx = kind - 1;
    s32 v;

    switch (idx) {
    case 1:
        goto subCase;
    case 0:
        goto addCase;
    case 2:
        goto end;
    case 3:
        goto kind4Case;
    case 4:
        goto end;
    case 5:
        goto end;
    case 6:
        goto end;
    case 7:
        goto kind8_12;
    case 8:
        goto end;
    case 9:
        goto end;
    case 10:
        goto end;
    case 11:
        goto kind8_12;
    default:
        goto end;
    }

subCase:
    {
        register s32 byteVal asm("r0") = *((u8 *)rec + 4);
        register s32 shifted asm("r1");

        shifted = byteVal << 7;
        field0 = *(s32 *)dest - shifted;
    }
    goto field4tail;

addCase:
    {
        register s32 byteVal asm("r0") = *((u8 *)rec + 4);
        register s32 shifted asm("r1");

        shifted = byteVal << 7;
        field0 = *(s32 *)dest + shifted;
    }

field4tail:
    *(s32 *)dest = field0;
    {
        s32 v2 = *(s16 *)((u8 *)rec + 2);
        v2 <<= 8;
        *(s32 *)((u8 *)dest + 4) += v2;
    }
    goto end;

kind4Case:
    v = *(s16 *)((u8 *)rec + 2);
    goto bigtail;

kind8_12:
    {
        register s32 result asm("r1");

        asm volatile(
            "mov r0, #2\n\t"
            "ldrsh r1, [r2, r0]\n\t"
            "ldrb r0, [r2, #5]\n\t"
            "add r1, r0, r1\n\t"
            : "=r"(result)
            : "r"(rec)
            : "r0"
        );
        v = result;
    }

bigtail:
    v <<= 8;
    *(s32 *)((u8 *)dest + 4) += v;
    {
        register s32 byteVal asm("r2") = *((u8 *)rec + 4);
        register s32 shifted asm("r1");
        register s32 field0b asm("r0");

        shifted = byteVal << 7;
        field0b = *(s32 *)dest;
        field0b -= shifted;
        *(s32 *)dest = field0b;
    }
end:
    return;
}
asm(".align 2, 0");
