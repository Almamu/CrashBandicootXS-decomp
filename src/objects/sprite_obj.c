#include "core.h"
#include "actor.h"
#include "vram_pool.h"
#include "gfx_part.h"
#include "sprite_bank.h"
#include "aabb.h"
#include "util.h"
#include "gfx.h"
#include "objects.h"
#include "memory.h"
#include "level.h"
#include "globals.h"

/* Builds `part`'s AABB (same keyframe-table shape/record layout as
 * GetSpriteHitbox, inlined directly here rather than calling it - this
 * function needs the box on the stack for the final overlap test, not
 * written out through a `dest` pointer) and mirrors it per
 * `part+0x28` bits 4/5, then tests it for overlap against `region` via
 * `AabbOverlaps` - the same collision-test function `CheckSpritePickup`
 * uses. */
s32 SpriteHitboxOverlaps(struct actor *part, void *region)
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
    SetAabbPos(&buf_, x, y);
    SetAabbSize(&buf_, w, h);

    if (mirrorX) {
        buf_.x = xpos * 2 - (buf_.x + buf_.w);
    }
    if (mirrorY) {
        buf_.y = ypos * 2 - (buf_.y + buf_.h);
    }

    return (u8)AabbOverlaps(&buf_, region);
}

/* Reads `part`'s current keyframe record's `+0x14` byte as a
 * `GetPaletteSlot` record id, looked up against the global tile-asset
 * cache `gPaletteCache`. */
s32 GetSpriteAnimPaletteSlot(struct actor *part)
{
    struct palette_cache *cache = gPaletteCache;
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

    return (u8)GetPaletteSlot(cache, rec);
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

/* `part+0x25 == 1` is the same fast override seen in
 * IsSpriteObjOnScreen/SpriteObjOverlapsRect; otherwise defers to `IsEntityInsideRect` (already
 * matched in graphics.c), forwarding `box` straight through
 * unmodified. */
s32 IsSpriteObjInsideRect(struct actor *part, void *box)
{
    s32 result = 0;
    register u8 *addr asm("r2") = (u8 *)part + 0x25;
    register u8 byteVal asm("r2");

    byteVal = *addr;
    if (byteVal == 1) {
        result = 1;
    } else if ((u8)IsEntityInsideRect(part, box)) {
        result = 1;
    }
    return result;
}

/* Same `part+0x25` fast-override shape as `IsSpriteObjInsideRect` above,
 * deferring to `IsEntityNearCamera` (already matched in `graphics.c`)
 * instead - a single-argument sibling, so the address scratch
 * naturally lands in `r1` instead of `r2` (no second call argument to
 * keep out of the way). */
s32 IsSpriteObjNearCamera(struct actor *part)
{
    s32 result = 0;
    register u8 *addr asm("r1") = (u8 *)part + 0x25;
    register u8 byteVal asm("r1");

    byteVal = *addr;
    if (byteVal == 1) {
        result = 1;
    } else if (IsEntityNearCamera(part)) {
        result = 1;
    }
    return result;
}

/* Always-true stub. */
s32 ApplySpriteObjVelocity(void)
{
    return 1;
}

/* Tail-calls `DrawSprite` (already matched in `sprite.c`) with
 * the global `gSpriteRenderer` as `self`. */
void DrawSpriteObj(void *part)
{
    DrawSprite(gSpriteRenderer, part);
}

extern void *_call_via_r1(void *arg0, void *arg1);

/* Advances `part`'s animation timer (`AdvanceSpriteAnim`), then resolves two
 * `table+N`/`table+N+4` offset/pointer slot pairs (the same convention
 * documented for `IsEntityNearCamera`/`IsSpriteObjOnScreen`) into `_call_via_r1` calls
 * - table+0x60/+0x64 first, then table+8/+0xc. */
void UpdateSpriteObj(struct actor *part)
{
    AdvanceSpriteAnim((struct box_part *)part);

    {
        void *table = part->table;
        void *slot = (u8 *)table + 0x60;
        s32 offset = *(s16 *)slot;
        void *addr = (u8 *)part + offset;
        void *ptr = *(void **)((u8 *)slot + 4);

        _call_via_r1(addr, ptr);
    }
    {
        void *table = part->table;
        s32 offset = *(s16 *)((u8 *)table + 8);
        void *addr = (u8 *)part + offset;
        void *ptr = *(void **)((u8 *)table + 0xc);

        _call_via_r1(addr, ptr);
    }
}

/* Returns a pointer to `part`'s current keyframe record's `+4` field -
 * the same keyframe-table lookup used throughout this ROM region. */
void *GetSpriteObjHitbox(struct actor *part)
{
    register void **tablePtr asm("r2") = *(void ***)((u8 *)part + 0x20);
    register u8 idx asm("r3") = *((u8 *)part + 0x2d);
    s32 offset = idx * 0x1c;
    void *table = *tablePtr;
    void *rec = (u8 *)table + offset;
    return (u8 *)rec + 4;
}

/* Ignores its `part` argument entirely (the ROM never reads r0 before
 * overwriting it) - already declared with this signature at its
 * `DrawSpritePieces` call site in graphics.c. Returns
 * `(*(void **)gSpriteBankSet)+4`'s value. */
s32 GetSpriteTileBase(void *part)
{
    void *p2 = (void *)gSpriteBankSet->table;
    return *(s32 *)((u8 *)p2 + 4);
}

/* Looks up `part`'s current keyframe record (same convention as
 * elsewhere in this ROM region). If `part+0x38` ("done", set by
 * `AdvanceSpriteAnim`) is set and the record's `+0x17` flags byte bit 1 is
 * clear (not looping), clamps `part`'s frame index (`+0x30`) to the
 * last frame (`record+0x16 - 1`) and resets the sub-counter
 * (`+0x34`) to the record's duration (`record+0x15`). Either way,
 * then resolves a final pointer: the record's own `+0` field is
 * itself a pointer (`recPtr`) to a per-frame `u16` array, indexed by
 * the (possibly just-clamped) frame index; that `u16` in turn indexes
 * a pointer array at `table+4`, and the result is that array's
 * pointer at the looked-up index.
 *
 * Matched in a later session than the original NAKED transcription -
 * see docs/matching.md's "Parked, not matched: GetSpriteFrame" for the
 * original account. Unlike `sub_8008188`/`sub_8008200`/`sub_8008278`'s
 * shared-switch-case gap, this function's resistant
 * `adds r0, r1, r0`-vs-`adds r0, r0, r1` add sits in genuinely
 * straight-line code (no switch, no case merging to protect), so a
 * plain inline-asm anchor on just that one instruction - the same
 * technique that had backfired inside those functions' shared case
 * blocks - works here with no caveats. Needed a trailing
 * `asm(".align 2, 0")` since it's the last function in this file (the
 * ROM has 2 bytes of zero padding here before `GetSpriteObjPriority`
 * below, and a plain compiled function's own natural
 * alignment produces a `0x46c0` nop-fill instead - the standard
 * `matching_decomp_alignment_fix` gotcha). */
void *GetSpriteFrame(struct gfx_part *part)
{
    register void *rec asm("r1") = part->bank;
    register u8 *idxAddr asm("r2") = &part->tag;
    register u8 idx asm("r4") = *idxAddr;
    s32 offset = idx * sizeof(struct sprite_anim);

    rec = (void *)((struct sprite_bank *)rec)->anims;
    rec = (u8 *)rec + offset; /* &bank->anims[part->tag] */

    if (part->animDone != 0) {
        register s32 mask asm("r0") = SPRITE_ANIM_LOOP;
        register s32 flags asm("r2") = ((struct sprite_anim *)rec)->flags;
        register s32 test asm("r0");

        test = mask & flags;
        if (!test) {
            part->frame = ((struct sprite_anim *)rec)->frameCount - 1;
            part->stepTimer = ((struct sprite_anim *)rec)->duration;
        }
    }

    {
        struct sprite_bank *bank = (struct sprite_bank *)part->bank;
        s32 frameIdx = part->frame;
        register void *recPtr asm("r1") = (void *)((struct sprite_anim *)rec)->seq;
        register s32 byteOffset asm("r0") = frameIdx * 2;
        register u16 *arr asm("r0");
        register void **ptrArray asm("r1");
        u16 idx2;

        /* The ROM's `adds r0, r0, r1` (byteOffset-then-recPtr operand
         * order) versus this compiler's always-canonicalized
         * `adds r0, r1, r0` - see the doc comment above. */
        asm volatile(
            "add r0, r0, r1\n\t"
            : "=r"(arr)
            : "0"(byteOffset), "r"(recPtr)
        );

        ptrArray = (void **)bank->frames;
        idx2 = *arr;
        return ptrArray[idx2];
    }
}
asm(".align 2, 0");

/* If `gLevelLayers+0x2b` is nonzero, returns
 * `(gLevelLayers's sub-object)+0x34`'s low 2 bits minus 1;
 * otherwise returns those same low 2 bits unmodified. Same
 * `gLevelLayers` sub-object convention used throughout this ROM
 * region (see `IsSpriteObjOnScreen`/`IsEntityNearCamera`). */
s32 GetSpriteObjPriority(void)
{
    if (gLevelLayers->unk_2B == 0) {
        void *subObj = gLevelLayers->layer0;
        u8 byte2 = *((u8 *)subObj + 0x34);
        u32 result = ((u32)byte2 << 30) >> 30;
        return result;
    } else {
        void *subObj = gLevelLayers->layer0;
        u8 byte2 = *((u8 *)subObj + 0x34);
        u32 result = ((u32)byte2 << 30) >> 30;
        return result - 1;
    }
}

/* Allocates a new `struct actor`-shaped object (`OperatorNew`),
 * initializes it via `InitEntity` (already matched in graphics.c -
 * wires up `gEntityVtable` and clears flags), then overwrites
 * its table with `gSpriteObjVtable` instead and clears its
 * part-object fields via `ResetSpriteObj` (already matched in
 * sprite.c). `arg0` becomes `field_08`, `arg1`/`arg2` become the
 * Q8 `x`/`y` position. */
void *CreateSpriteObj(u16 arg0, u16 arg1, u16 arg2, u16 unused)
{
    struct actor *part = OperatorNew(0x40);

    InitEntity(part);
    part->table = (void *)gSpriteObjVtable;
    ResetSpriteObj(part);
    part->field_08 = arg0;
    part->x = (s32)arg1 << 8;
    part->y = (s32)arg2 << 8;
    return part;
}

/* Always-true stub. */
s32 GetSpriteObjClassId(void)
{
    return 1;
}


/* Same `gEntityVtable`/conditional-`OperatorDelete` shape as
 * `DestroyEntity` (already matched in `graphics.c`). */
void DestroySpriteObj(struct actor *self, u32 arg1)
{
    self->table = (void *)gEntityVtable;
    if (arg1 & 1) {
        OperatorDelete(self);
    }
}

/* Same `InitEntity`/table-swap/`ResetSpriteObj` shape as `CreateSpriteObj`
 * above, but re-initializes an existing `self` instead of allocating
 * a new one. */
struct actor *InitSpriteObj(struct actor *self)
{
    InitEntity(self);
    self->table = (void *)gSpriteObjVtable;
    ResetSpriteObj(self);
    return self;
}


/* Looks up `part`'s keyframe record via `GetSpriteFrame` (already parked
 * as `NON_MATCHING` in `sprite_obj.c`), then picks a pointer off it
 * per the record's `+4` byte's upper nibble: 0 -> `info+0x24`, 6 ->
 * `info+0x14`, anything else (1-5, or above 6) -> the fixed fallback
 * table `gEmptySpritePoint`. Needed the case labels scattered
 * out of numeric order (rather than grouped into the obvious
 * contiguous "0 / 1-5 / 6" ranges) to get gcc to emit a real jump
 * table instead of a compare chain - this compiler only builds a
 * jump table when the case-to-block mapping can't be expressed as a
 * few simple range checks, so a source-level shape that *looks*
 * needlessly scattered is what is needed to match the ROM's own
 * jump table here. */
void *GetSpriteFrameAnchor(void *part)
{
    void *info = GetSpriteFrame(part);
    u8 type = *(u8 *)(*(void **)((u8 *)info + 4)) >> 4;
    void *result;

    switch (type) {
    case 0:
        result = (u8 *)info + 0x24;
        break;
    case 3:
    case 4:
        result = (void *)&gEmptySpritePoint;
        break;
    case 1:
    case 2:
        result = (void *)&gEmptySpritePoint;
        break;
    case 5:
        result = (void *)&gEmptySpritePoint;
        break;
    case 6:
        result = (u8 *)info + 0x14;
        break;
    default:
        result = (void *)&gEmptySpritePoint;
        break;
    }
    return result;
}

/* Same `GetSpriteFrame`-derived-record-nibble-switch shape as
 * `GetSpriteFrameAnchor` above, with a different result mapping: 0 and 4
 * select `info+0x1c`, anything else falls back to
 * `gEmptySpriteBox`. Unlike `GetSpriteFrameAnchor`, no case-scattering
 * trick was needed here - 0 and 4 are already non-adjacent, which is
 * enough on its own to make gcc emit a jump table instead of a
 * compare chain. */
void *GetSpriteFrameThirdBox(void *part)
{
    void *info = GetSpriteFrame(part);
    u8 type = *(u8 *)(*(void **)((u8 *)info + 4)) >> 4;
    void *result;

    switch (type) {
    case 0:
        result = (u8 *)info + 0x1c;
        break;
    case 1:
    case 2:
    case 3:
        result = (void *)&gEmptySpriteBox;
        break;
    case 4:
        result = (u8 *)info + 0x1c;
        break;
    case 5:
    case 6:
        result = (void *)&gEmptySpriteBox;
        break;
    default:
        result = (void *)&gEmptySpriteBox;
        break;
    }
    return result;
}

/* Same `GetSpriteFrame`-derived-record-nibble `switch` shape again, with
 * the exact same case-to-block mapping as `GetSpriteAttackBox` (already
 * matched in `sprite.c`) - 0/3/4 select `info+0x14`, 5 selects
 * `info+0xc`, and 1/2/6/anything-above-6 fall back to
 * `gEmptySpriteBox`. That mapping is non-contiguous on its own,
 * so plain ascending case order was enough for a jump table here too,
 * no scattering needed. */
void *GetSpriteFrameAttackBox(void *part)
{
    void *info = GetSpriteFrame(part);
    u8 type = *(u8 *)(*(void **)((u8 *)info + 4)) >> 4;
    void *result;

    switch (type) {
    case 0:
    case 3:
    case 4:
        result = (u8 *)info + 0x14;
        break;
    case 1:
    case 2:
    case 6:
        result = (void *)&gEmptySpriteBox;
        break;
    case 5:
        result = (u8 *)info + 0xc;
        break;
    default:
        result = (void *)&gEmptySpriteBox;
        break;
    }
    return result;
}

/* Same `GetSpriteFrame`-derived-record-nibble `switch` shape once more -
 * 0/2/3/4/6 select `info+0xc`, 1/5/anything-above-6 fall back to
 * `gEmptySpriteBox`. */
void *GetSpriteFrameBodyBox(void *part)
{
    void *info = GetSpriteFrame(part);
    u8 type = *(u8 *)(*(void **)((u8 *)info + 4)) >> 4;
    void *result;

    switch (type) {
    case 0:
        result = (u8 *)info + 0xc;
        break;
    case 1:
        result = (void *)&gEmptySpriteBox;
        break;
    case 2:
    case 3:
    case 4:
        result = (u8 *)info + 0xc;
        break;
    case 5:
        result = (void *)&gEmptySpriteBox;
        break;
    case 6:
        result = (u8 *)info + 0xc;
        break;
    default:
        result = (void *)&gEmptySpriteBox;
        break;
    }
    return result;
}

/* Same keyframe-record lookup used throughout this ROM region (see
 * `GetSpriteObjHitbox`) - `part`'s `+0x20` table pointer dereferenced twice,
 * indexed by the `+0x2d` frame index, times the record size (0x1c). */
void *GetSpriteAnimRecord(struct actor *part)
{
    register void **tablePtr asm("r2") = *(void ***)((u8 *)part + 0x20);
    register u8 idx asm("r3") = *((u8 *)part + 0x2d);
    s32 offset = idx * 0x1c;
    void *table = *tablePtr;
    return (u8 *)table + offset;
}

/* Clamps `frame` to `part`'s current keyframe record's duration
 * (`+0x16`) minus one if it's out of range, then stores the result
 * into `part+0x30` (the frame index also read/written by
 * `GetSpriteFrame`). Needed explicit register pins on the whole
 * tablePtr/idxAddr/table/idx chain to get the ROM's `r5` (rather than
 * a tighter, naturally-reused register) - `idx` genuinely outlives
 * `table`'s own register here. The final `rec = table + offset` add
 * also hit the resistant "which operand goes first" canonicalization
 * documented at length for `sub_8008188`/`sub_8008200`/
 * `sub_8008278`/`GetSpriteFrame` - but unlike those (which were inside a
 * `switch` and had to be parked to avoid breaking case-block merging),
 * this function has no such constraint, so a one-instruction inline
 * `asm` anchor for just this add gets a fully byte-exact match. */
void SetSpriteFrameIndex(struct actor *part, s32 frame)
{
    register void **tablePtr asm("r0") = *(void ***)((u8 *)part + 0x20);
    register u8 *idxAddr asm("r2") = (u8 *)part + 0x2d;
    register void *table asm("r1") = *tablePtr;
    register u8 idx asm("r5") = *idxAddr;
    register s32 offset asm("r0") = idx * 0x1c;
    void *rec;

    asm("add %0, %0, %1" : "+r" (offset) : "r" (table));
    rec = (void *)offset;

    {
        u8 duration = *((u8 *)rec + 0x16);

        if (frame >= duration) {
            frame = duration - 1;
        }
        *(s32 *)((u8 *)part + 0x30) = frame;
    }
}

/* `part+0x25` accessor pair - plain byte get/set, no other logic. */
u8 GetSpriteScreenSpace(void *part)
{
    return *((u8 *)part + 0x25);
}

void SetSpriteScreenSpace(void *part, u8 val)
{
    *((u8 *)part + 0x25) = val;
}

/* `part+0xd` bit-2 getter. */
s32 IsSpriteHidden(void *part)
{
    return (*((u8 *)part + 0xd) >> 2) & 1;
}

/* Toggles `part+0xd` bit 2. Needed the bit-flip (`(byte>>2)^1)&1`)
 * done via genuinely separate `eor`+`and` instructions instead of the
 * single `bic` this compiler normally folds that pattern into -
 * forced via a two-instruction inline `asm` block, whose "one" input
 * also needed marking `+r` (read-write) even though its value never
 * changes, purely to stop the compiler from constant-propagating its
 * value 1 past the asm block and computing the later mask (`-5`) as
 * `1 - 6` off of it instead of the ROM's fresh `movs r1, #5; negs r1,
 * r1`. Also needed the shifted-bit computed before (not after) the
 * mask, matching the ROM's own instruction order. */
void ToggleSpriteHidden(void *part)
{
    register u32 byte asm("r3") = *((u8 *)part + 0xd);
    register u32 shifted asm("r2") = byte >> 2;
    register u32 one asm("r1") = 1;
    register u32 bit asm("r2");
    register u32 shiftedBit asm("r2");
    register s32 mask asm("r1");
    register s32 result asm("r1");

    asm("eor %0, %0, %2\n\tand %0, %0, %2" : "=r" (bit), "+r" (one) : "1" (one), "0" (shifted));
    shiftedBit = bit << 2;

    mask = -5;
    result = mask & byte;
    result |= shiftedBit;
    *((u8 *)part + 0xd) = result;
}

/* `part+0xd` bit-3 getter - same shape as `IsSpriteHidden` above, one
 * bit over. */
s32 IsPartSolid(void *part)
{
    return (*((u8 *)part + 0xd) >> 3) & 1;
}

/* Clears `part+0xd` bit 3. Needed the mask register-pinned to a
 * literal `-9` (computed via `movs r1, #9; negs r1, r1`, same
 * `-(N+1) == ~N` trick as `ToggleSpriteHidden`'s `-5` mask above) instead of
 * `~8`, which this compiler folds directly into a single `mov #0xf7`
 * immediate load. */
void ClearPartSolid(void *part)
{
    register s32 mask asm("r1") = -9;
    register s32 byte asm("r2") = *((u8 *)part + 0xd);
    register s32 result asm("r1");

    result = mask & byte;
    *((u8 *)part + 0xd) = result;
}

/* Sets `part+0xd` bit 3. Needed the mask register-pinned and computed
 * before the byte load (matching the ROM's own instruction order) -
 * the natural allocation loads the byte first. Same accumulator-
 * register pattern used for every AND/OR accessor below. */
void SetPartSolid(void *part)
{
    register s32 mask asm("r1") = 8;
    register s32 byte asm("r2") = *((u8 *)part + 0xd);
    register s32 result asm("r1");

    result = mask | byte;
    *((u8 *)part + 0xd) = result;
}

/* `part->flags` bit-6 getter. */
s32 IsSpriteObjVulnerable(struct actor *part)
{
    return (part->flags >> 6) & 1;
}

/* Clears `part->flags` bit 6. */
void ClearSpriteObjVulnerable(struct actor *part)
{
    register s32 mask asm("r1") = -0x41;
    register s32 byte asm("r2") = part->flags;
    register s32 result asm("r1");

    result = mask & byte;
    part->flags = result;
}

/* Sets `part->flags` bit 6. */
void SetSpriteObjVulnerable(struct actor *part)
{
    register s32 mask asm("r1") = 0x40;
    register s32 byte asm("r2") = part->flags;
    register s32 result asm("r1");

    result = mask | byte;
    part->flags = result;
}

/* Resets `part`'s frame index (`+0x2d`) to 0. */
void ResetSpriteAnimIndex(void *part)
{
    *((u8 *)part + 0x2d) = 0;
}

/* `part->flags` bit-7 getter - no mask needed since the shift already
 * leaves only that bit in position 0 of an 8-bit value. */
s32 IsSpriteObjCollisionEnabled(struct actor *part)
{
    return part->flags >> 7;
}

/* Clears `part->flags` bit 7. */
void DisableSpriteObjCollision(struct actor *part)
{
    register s32 mask asm("r1") = 0x7f;
    register s32 byte asm("r2") = part->flags;
    register s32 result asm("r1");

    result = mask & byte;
    part->flags = result;
}

/* Sets `part->flags` bit 7. */
void EnableSpriteObjCollision(struct actor *part)
{
    register s32 mask asm("r1") = 0x80;
    register s32 byte asm("r2") = part->flags;
    register s32 result asm("r1");

    result = mask | byte;
    part->flags = result;
}

/* `part+0x2c` byte get/set pair. */
u8 GetSpriteAnimating(void *part)
{
    return *((u8 *)part + 0x2c);
}

void SetSpriteAnimating(void *part, u8 val)
{
    *((u8 *)part + 0x2c) = val;
}

/* Sets `part+0x28` bit 4 to `value & 1`. Needed the low-bit extraction
 * done via a two-instruction inline `asm` AND (rather than this
 * compiler's own `& 1`, which produces the same result but as three
 * instructions once the u8 parameter's mandatory entry truncation is
 * folded in) - and, as with `ToggleSpriteHidden`, the "1" input needed
 * marking `+r` to stop the mask constant `-0x11` from being computed
 * relative to that leftover register value instead of freshly. */
void SetSpriteFlipX(void *part, u8 value)
{
    register s32 truncVal asm("r1") = value;
    register u8 *addr asm("r0") = (u8 *)part + 0x28;
    register s32 one asm("r2") = 1;
    register s32 shiftedBit asm("r1");
    register s32 mask asm("r2");
    register s32 byte asm("r3");
    register s32 result asm("r2");

    asm("and %0, %0, %1" : "+r" (truncVal), "+r" (one));
    shiftedBit = truncVal << 4;
    mask = -0x11;
    byte = *addr;
    result = mask & byte;
    result |= shiftedBit;
    *addr = result;
}

/* Same shape as `SetSpriteFlipX` immediately above, sets `part+0x28` bit
 * 5 instead. */
void SetSpriteFlipY(void *part, u8 value)
{
    register s32 truncVal asm("r1") = value;
    register u8 *addr asm("r0") = (u8 *)part + 0x28;
    register s32 one asm("r2") = 1;
    register s32 shiftedBit asm("r1");
    register s32 mask asm("r2");
    register s32 byte asm("r3");
    register s32 result asm("r2");

    asm("and %0, %0, %1" : "+r" (truncVal), "+r" (one));
    shiftedBit = truncVal << 5;
    mask = -0x21;
    byte = *addr;
    result = mask & byte;
    result |= shiftedBit;
    *addr = result;
}

/* `part+0x38` ("done" flag, also read/written by `GetSpriteFrame`)
 * setter. */
void SetSpriteAnimDone(void *part, u8 val)
{
    *((u8 *)part + 0x38) = val;
}

/* Same keyframe-record lookup used throughout this ROM region (see
 * `GetSpriteObjHitbox`/`GetSpriteAnimRecord`), returning the record's `+0x14` byte
 * instead of the record pointer itself. The final `rec = table +
 * offset` add hit the same resistant operand-order gap as
 * `SetSpriteFrameIndex` - fixed the same way, with a one-instruction inline
 * `asm` anchor. */
u8 GetSpriteAnimPaletteId(struct actor *part)
{
    register void **tablePtr asm("r1") = *(void ***)((u8 *)part + 0x20);
    register u8 *idxAddr asm("r0") = (u8 *)part + 0x2d;
    register void *table asm("r2") = *tablePtr;
    register u8 idx asm("r3") = *idxAddr;
    register s32 offset asm("r1") = idx * 0x1c;
    void *rec;

    asm("add %0, %0, %1" : "+r" (offset) : "r" (table));
    rec = (void *)offset;
    return *((u8 *)rec + 0x14);
}

/* `part+0x29` low-nibble getter. */
s32 GetSpritePalette(void *part)
{
    u32 byte = *((u8 *)part + 0x29);
    return (byte << 0x1c) >> 0x1c;
}

/* Sets `part+0x29`'s low nibble to `value & 0xf`. Needed the
 * parameter typed `s32` rather than `u8` - the `& 0xf` mask on a `u8`-
 * typed parameter compiles to a much longer defensive shift-based
 * sequence in this compiler (confirmed in isolation), which the ROM
 * doesn't have. The mask constant also needed the same `+r`-on-the-
 * other-operand fix as `ToggleSpriteHidden`/`SetSpriteFlipX` to stop it being
 * computed relative to the leftover "0xf" register value. */
void SetSpritePalette(void *part, s32 value)
{
    register u8 *addr asm("r0") = (u8 *)part + 0x29;
    register s32 value_ asm("r1") = value;
    register s32 fifteen asm("r2") = 0xf;
    register s32 lowNibble asm("r1");
    register s32 mask asm("r2");
    register s32 byte asm("r3");
    register s32 result asm("r2");

    asm("and %0, %0, %1" : "+r" (value_), "+r" (fifteen));
    lowNibble = value_;

    mask = -0x10;
    byte = *addr;
    result = mask & byte;
    result |= lowNibble;
    *addr = result;
}

/* `part+0x20` table-pointer get/set pair. */
void SetSpriteAnimTable(void *part, void *val)
{
    *(void **)((u8 *)part + 0x20) = val;
}

void *GetSpriteAnimTable(void *part)
{
    return *(void **)((u8 *)part + 0x20);
}

/* Same keyframe-record lookup as `GetSpriteAnimPaletteId` above, testing the
 * record's `+0x17` flags bit 1 and returning it as a plain 0/1 value.
 * Matched after the NAKED transcription this function briefly used
 * (see git history and docs/matching.md's "Parked, not matched:
 * IsSpriteAnimLooping" entry for that account): every instruction here
 * matches the ROM up through the `ands r0, r1` on its own, but the
 * ROM's two trailing byte-truncation instructions (`lsls r0, r0,
 * #0x18; lsrs r0, r0, #0x18`, narrowing the AND result to the `u8`
 * return type) got optimized away by this compiler every time it
 * could prove the AND result (mask is the visible constant 2) already
 * fits in a byte. Closed with an empty `asm volatile("" : "+r"(test))`
 * barrier right after the `and`, making `test`'s value opaque to the
 * optimizer so it can no longer prove the automatic `s32`-to-`u8`
 * return-value truncation is redundant - the barrier itself emits no
 * instructions, it just forces the *existing* implicit truncation
 * back in. An explicit asm block emitting the shift pair directly was
 * tried first and also produced byte-exact output up through those
 * two instructions, but always duplicated them (the compiler still
 * inserted its own separate return-value truncation afterward,
 * regardless of whether the asm's output was typed `s32` or `u8`) -
 * the empty-barrier form avoids that by leaving the actual truncation
 * to the compiler's own return-conversion codegen. */
u8 IsSpriteAnimLooping(struct actor *part)
{
    register void **tablePtr asm("r1") = *(void ***)((u8 *)part + 0x20);
    register u8 *idxAddr asm("r0") = (u8 *)part + 0x2d;
    register void *table asm("r2") = *tablePtr;
    register u8 idx asm("r3") = *idxAddr;
    register s32 offset asm("r1") = idx * 0x1c;
    void *rec;
    register s32 mask asm("r0");
    register s32 flags asm("r1");
    register s32 test asm("r0");

    asm("add %0, %0, %1" : "+r" (offset) : "r" (table));
    rec = (void *)offset;

    mask = 2;
    flags = *((u8 *)rec + 0x17);
    test = mask & flags;
    asm volatile("" : "+r" (test));
    return test;
}
asm(".align 2, 0");
