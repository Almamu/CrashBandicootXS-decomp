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

#if NON_MATCHING
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
 * NOT YET BYTE-MATCHING: every instruction matches the ROM exactly -
 * including the exact case-body layout order in the jump table (cases
 * 1/2 are declared kind-1-then-kind-0 in the switch to get the ROM's
 * own block ordering, the same non-obvious-declaration-order pattern
 * documented for sub_8007DBC's spawn switch) and the two duplicate
 * case labels (8 and 12) correctly sharing one code block - except a
 * single register-register `add` in that shared kind-8/12 block:
 * the ROM's `adds r1, r2, r1` (byte-value register as the first
 * source operand, short-value register second) versus this
 * reconstruction's `adds r1, r1, r2` (the more usual "accumulate
 * in-place" operand order). Every technique that worked for similar
 * cases elsewhere in this project failed here: swapping the C
 * addition's operand order, giving each operand its own pinned
 * register, using a separate unpinned destination variable, an
 * inline-asm anchor for just the add (which also broke the case-8/12
 * block merging, trading one mismatch for a worse one), and reversing
 * which operand loads first (gcc reschedules the loads back to the
 * same order regardless, and still emits the self-referencing add
 * form). This compiler appears to always canonicalize a
 * register-register add so the destination's own prior value becomes
 * the first source operand, with no C-level way found to override it.
 * Parked rather than keep chasing this one instruction - same call as
 * the other parked functions above. */
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
        break;
    case 0:
        {
            register s32 byteVal asm("r2") = *((u8 *)rec + 4);
            register s32 shifted asm("r1");

            shifted = byteVal << 7;
            *(s32 *)dest -= shifted;
        }
        break;
    case 2:
        break;
    case 3:
        {
            s32 v = *(s16 *)((u8 *)rec + 2);
            v <<= 8;
            *(s32 *)((u8 *)dest + 4) -= v;
        }
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        {
            register s32 v asm("r1") = *(s16 *)((u8 *)rec + 2);
            register s32 byteVal asm("r2") = *((u8 *)rec + 5);
            register s32 result asm("r1");

            result = byteVal + v;
            result <<= 8;
            *(s32 *)((u8 *)dest + 4) -= result;
        }
        break;
    case 8:
        break;
    case 9:
        break;
    case 10:
        break;
    case 11:
        {
            register s32 v asm("r1") = *(s16 *)((u8 *)rec + 2);
            register s32 byteVal asm("r2") = *((u8 *)rec + 5);
            register s32 result asm("r1");

            result = byteVal + v;
            result <<= 8;
            *(s32 *)((u8 *)dest + 4) -= result;
        }
        break;
    default:
        break;
    }
}
#endif /* NON_MATCHING */

#if NON_MATCHING
/* Same shape as `sub_8008188` above (mirror-image add/subtract
 * directions: kind 1/2 do the opposite sign on `dest->field_0`, and
 * kinds 4/8/12 add to `dest->field_4` instead of subtracting). Same
 * single resistant `add`-operand-order gap in the shared kind-8/12
 * block - see `sub_8008188`'s doc comment and
 * "Parked, not matched: `sub_8008188`" in docs/matching.md for the
 * full account of what was tried. */
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
        break;
    case 0:
        {
            register s32 byteVal asm("r2") = *((u8 *)rec + 4);
            register s32 shifted asm("r1");

            shifted = byteVal << 7;
            *(s32 *)dest += shifted;
        }
        break;
    case 2:
        break;
    case 3:
        {
            s32 v = *(s16 *)((u8 *)rec + 2);
            v <<= 8;
            *(s32 *)((u8 *)dest + 4) += v;
        }
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        {
            register s32 v asm("r1") = *(s16 *)((u8 *)rec + 2);
            register s32 byteVal asm("r2") = *((u8 *)rec + 5);
            register s32 result asm("r1");

            result = byteVal + v;
            result <<= 8;
            *(s32 *)((u8 *)dest + 4) += result;
        }
        break;
    case 8:
        break;
    case 9:
        break;
    case 10:
        break;
    case 11:
        {
            register s32 v asm("r1") = *(s16 *)((u8 *)rec + 2);
            register s32 byteVal asm("r2") = *((u8 *)rec + 5);
            register s32 result asm("r1");

            result = byteVal + v;
            result <<= 8;
            *(s32 *)((u8 *)dest + 4) += result;
        }
        break;
    default:
        break;
    }
}
#endif /* NON_MATCHING */
asm(".align 2, 0");
