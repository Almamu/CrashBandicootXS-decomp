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
 * established yet. See the (now removed) NON_MATCHING C draft in git
 * history for the full commented C reconstruction - every instruction
 * matched the ROM except a single register-register `add` in the
 * shared kind-8/12 block (the ROM's `adds r1, r2, r1` versus this
 * compiler's always-canonicalized `adds r1, r1, r2` - see
 * docs/matching.md's "Parked, not matched: sub_8008188" for the five
 * techniques tried and why each failed). Written as NAKED asm here
 * instead, same technique as `sub_8006600`/`sub_80073DC`/`sub_8007B00`/
 * `sub_8008044` above and this project's other hard-compiler-limitation
 * cases (see src/system/link_cable.c/src/audio/gax_swi.c). */
NAKED void sub_8008188(void *dest, s32 kind, void *rec)
{
    asm(
        "add r3, r0, #0\n\t"
        "sub r0, r1, #1\n\t"
        "cmp r0, #0xb\n\t"
        "bhi 8f\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r1, 1f\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "mov pc, r0\n\t"
        ".align 2, 0\n"
    "1: .4byte 2f\n"
    "2:\n\t"
        ".4byte 4f\n\t"
        ".4byte 3f\n\t"
        ".4byte 8f\n\t"
        ".4byte 5f\n\t"
        ".4byte 8f\n\t"
        ".4byte 8f\n\t"
        ".4byte 8f\n\t"
        ".4byte 6f\n\t"
        ".4byte 8f\n\t"
        ".4byte 8f\n\t"
        ".4byte 8f\n\t"
        ".4byte 6f\n\t"
    "3:\n\t"
        "ldrb r2, [r2, #4]\n\t"
        "lsl r1, r2, #7\n\t"
        "ldr r0, [r3]\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r3]\n\t"
        "b 8f\n\t"
    "4:\n\t"
        "ldrb r2, [r2, #4]\n\t"
        "lsl r1, r2, #7\n\t"
        "ldr r0, [r3]\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [r3]\n\t"
        "b 8f\n\t"
    "5:\n\t"
        "mov r0, #2\n\t"
        "ldrsh r1, [r2, r0]\n\t"
        "b 7f\n\t"
    "6:\n\t"
        "mov r0, #2\n\t"
        "ldrsh r1, [r2, r0]\n\t"
        "ldrb r2, [r2, #5]\n\t"
        "add r1, r2, r1\n\t"
    "7:\n\t"
        "lsl r1, r1, #8\n\t"
        "ldr r0, [r3, #4]\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [r3, #4]\n\t"
    "8:\n\t"
        "bx lr\n\t"
    );
}

/* Same shape as `sub_8008188` above (mirror-image add/subtract
 * directions: kind 1/2 do the opposite sign on `dest->field_0`, and
 * kinds 4/8/12 add to `dest->field_4` instead of subtracting). Same
 * single resistant `add`-operand-order gap in the shared kind-8/12
 * block as `sub_8008188` above - see its doc comment and
 * "Parked, not matched: `sub_8008188`" in docs/matching.md for the
 * full account of what was tried. Written as NAKED asm here too. */
NAKED void sub_8008200(void *dest, s32 kind, void *rec)
{
    asm(
        "add r3, r0, #0\n\t"
        "sub r0, r1, #1\n\t"
        "cmp r0, #0xb\n\t"
        "bhi 8f\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r1, 1f\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "mov pc, r0\n\t"
        ".align 2, 0\n"
    "1: .4byte 2f\n"
    "2:\n\t"
        ".4byte 4f\n\t"
        ".4byte 3f\n\t"
        ".4byte 8f\n\t"
        ".4byte 5f\n\t"
        ".4byte 8f\n\t"
        ".4byte 8f\n\t"
        ".4byte 8f\n\t"
        ".4byte 6f\n\t"
        ".4byte 8f\n\t"
        ".4byte 8f\n\t"
        ".4byte 8f\n\t"
        ".4byte 6f\n\t"
    "3:\n\t"
        "ldrb r2, [r2, #4]\n\t"
        "lsl r1, r2, #7\n\t"
        "ldr r0, [r3]\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [r3]\n\t"
        "b 8f\n\t"
    "4:\n\t"
        "ldrb r2, [r2, #4]\n\t"
        "lsl r1, r2, #7\n\t"
        "ldr r0, [r3]\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r3]\n\t"
        "b 8f\n\t"
    "5:\n\t"
        "mov r0, #2\n\t"
        "ldrsh r1, [r2, r0]\n\t"
        "b 7f\n\t"
    "6:\n\t"
        "mov r0, #2\n\t"
        "ldrsh r1, [r2, r0]\n\t"
        "ldrb r2, [r2, #5]\n\t"
        "add r1, r2, r1\n\t"
    "7:\n\t"
        "lsl r1, r1, #8\n\t"
        "ldr r0, [r3, #4]\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r3, #4]\n\t"
    "8:\n\t"
        "bx lr\n\t"
    );
}

/* A third variant of `sub_8008188`'s shape: kind 1/2 update
 * `dest->field_0` (sub/add) AND unconditionally also add `rec+2`'s
 * short (Q8) to `dest->field_4`; kinds 4/8/12 add to `dest->field_4`
 * (same as `sub_8008188`'s kinds) AND additionally always subtract
 * `rec+4`'s byte (Q8, `<<7`) from `dest->field_0` afterward. Same
 * single resistant `add`-operand-order gap as `sub_8008188`/
 * `sub_8008200` in the shared kind-8/12 block - see `sub_8008188`'s
 * doc comment for the full account of what was tried. Written as NAKED
 * asm here too. */
NAKED void sub_8008278(void *dest, s32 kind, void *rec)
{
    asm(
        "add r3, r0, #0\n\t"
        "sub r0, r1, #1\n\t"
        "cmp r0, #0xb\n\t"
        "bhi 9f\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r1, 1f\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "mov pc, r0\n\t"
        ".align 2, 0\n"
    "1: .4byte 2f\n"
    "2:\n\t"
        ".4byte 4f\n\t"
        ".4byte 3f\n\t"
        ".4byte 9f\n\t"
        ".4byte 6f\n\t"
        ".4byte 9f\n\t"
        ".4byte 9f\n\t"
        ".4byte 9f\n\t"
        ".4byte 7f\n\t"
        ".4byte 9f\n\t"
        ".4byte 9f\n\t"
        ".4byte 9f\n\t"
        ".4byte 7f\n\t"
    "3:\n\t"
        "ldrb r0, [r2, #4]\n\t"
        "lsl r1, r0, #7\n\t"
        "ldr r0, [r3]\n\t"
        "sub r0, r0, r1\n\t"
        "b 5f\n\t"
    "4:\n\t"
        "ldrb r0, [r2, #4]\n\t"
        "lsl r1, r0, #7\n\t"
        "ldr r0, [r3]\n\t"
        "add r0, r0, r1\n\t"
    "5:\n\t"
        "str r0, [r3]\n\t"
        "mov r0, #2\n\t"
        "ldrsh r1, [r2, r0]\n\t"
        "lsl r1, r1, #8\n\t"
        "ldr r0, [r3, #4]\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r3, #4]\n\t"
        "b 9f\n\t"
    "6:\n\t"
        "mov r0, #2\n\t"
        "ldrsh r1, [r2, r0]\n\t"
        "b 8f\n\t"
    "7:\n\t"
        "mov r0, #2\n\t"
        "ldrsh r1, [r2, r0]\n\t"
        "ldrb r0, [r2, #5]\n\t"
        "add r1, r0, r1\n\t"
    "8:\n\t"
        "lsl r1, r1, #8\n\t"
        "ldr r0, [r3, #4]\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r3, #4]\n\t"
        "ldrb r2, [r2, #4]\n\t"
        "lsl r1, r2, #7\n\t"
        "ldr r0, [r3]\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [r3]\n\t"
    "9:\n\t"
        "bx lr\n\t"
    );
}
asm(".align 2, 0");
