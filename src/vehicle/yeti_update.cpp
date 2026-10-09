#include "vehicle.hpp"

extern "C" {
#include "core.h"
#include "math_util.h"
#include "actor_anim.h"
#include "system.h"
#include "actor.h"
#include "vehicle.h"
#include "globals.h"
}

/* Sits right after polar_aku_aku.cpp's `CreatePolarCheckpointCrate` and before
 * yeti_states.cpp's `YetiStateChase`/`YetiStateCharge` - the whole contiguous
 * range that used to be `asm/code_3_2_20_28568_c99c_d7b0.s`. All three
 * functions here operate on the `gYeti`-rooted "position-
 * tracking object with tier-threshold sound cues" documented in
 * yeti_states.cpp's header comment and docs/matching/archive/issue-54-actor-d3a8.md
 * (the "third RAM-struct family" from docs/rom_map.md). See that issue
 * doc's "Second pass" section for how the 12-byte AABB-record layout
 * used here and by `IsTouchingYeti` (yeti_graphics.cpp) was finally pinned
 * down.
 *
 * Built with old_agbcc: `UpdateYetiBg2` only matches under it, and
 * `UpdateYetiPalette` matches under both. */

extern void _call_via_r0(void *fn);
extern void _call_via_r2(void *arg0, s32 arg1, void *fn);

/* One of two confirmed slots (index 3, dispatched via
 * `gYetiStateFuncs[gYetiState]`) of the type-0
 * `category_vtable` (`gActorCategoryVtables[0]`, `include/actor_anim.h`)
 * - `UpdateGameFrame`'s own direct top-level callee for this object, per
 * docs/rom_map.md's "Two new type-0 vtable slots confirmed" section.
 *
 * Unless `gYetiState == 3`, first eases `gYetiX`
 * toward the player's cached X position (`gActorList->+0x1c`,
 * divisor 32 - the same rsb/lsr/add/asr round-toward-zero idiom as
 * `MovePolarAkuAku`/`SetEntitySize`). Then advances the object's own anim
 * frame (`+8` accumulator by the `+0x10` per-frame increment,
 * `GetAnimFrameBaseOffset` against the current part-table record's
 * `+4`/`+6` timing fields, latching the `+0x12` done flag and correcting
 * the accumulator on overrun), fires the current
 * `gYetiStateFuncs[gYetiState]` vtable slot via
 * `_call_via_r0`, and - only when the accumulator's `>>8` value actually
 * changed this frame - fires a `_call_via_r2` trampoline from the part
 * table's own `+2`-offset record (latching `gYetiBg2PageFlip`).
 *
 * Finally, while `gYetiState <= 1`, runs a 3-axis AABB overlap
 * test between two 12-byte `{s16 x, y, z, sizeX, sizeY, sizeZ}` records
 * (axes compared Z, then Y, then X - matching the ROM's own instruction
 * order, not storage order) built the same way both times: a
 * `gYetiCatchBox`-rooted static record with `gYetiX`/
 * `gYetiPosition` (both `>>8`) added into its `x`/`z` fields only (this
 * object tracks no Y), against the player's own `+0x38` 12-byte vector
 * with the player's `+0x1c`/`0x20`/`0x24` position (all `>>8`) added
 * into all three of `x`/`y`/`z`. The second record is then run through
 * `MemCopy32` - a real, byte-verified `memcpy(box, box, 0xc)`
 * self-copy (`MemCopy32`'s own definition, `src/system/bios_util.cpp`,
 * confirmed a plain `memcpy`-style `CpuSet` wrapper) - a genuine no-op
 * kept byte-faithful since a shared "copy src into a working buffer,
 * then test" helper is being called here with a buffer that already
 * *is* its own source, not a disassembly artifact. On overlap, arms
 * `gYetiState = 2`, resets the object's kind/anim state to the
 * part table's `+0x18` record, and refreshes the player via
 * `CatchPolarPlayer`/`SetCellAnimSpeed(0)`; skipped once `gPolarPlayerInactive` (an
 * already-consumed one-shot flag elsewhere in this ROM region) is set.
 *
 * The three boxes (static A at sp, the copy B at sp+0xc, the player
 * box built at sp+0x18) are members of one frame struct, so each of
 * their addresses is a fresh `add rX, sp, #off` rather than a pseudo
 * kept in a callee-saved register; `&f.b` goes through a pointer local
 * right before the `MemCopy32` call so it (alone) stays live across
 * it, and A's address is taken again after the call. The hit block
 * uses its own `g` local for the gauge object (the function-wide `obj`
 * would be allocated a callee-saved register). Built with old_agbcc
 * (docs/matching/archive/issue-51-54-naked-retry.md, later pass). */
static inline void BoxMove(struct anim_box *b, s32 x, s32 y, s32 z)
{
    b->x += x;
    b->y += y;
    b->z += z;
}

static inline u8 BoxOverlap(struct anim_box *b, struct anim_box *a)
{
    if (b->z < a->z + a->d && b->z + b->d > a->z && b->y < a->y + a->h && b->y + b->h > a->y &&
        b->x < a->x + a->w && b->x + b->w > a->x)
        goto hit;
    return 0;
hit:
    return 1;
}

void UpdateYeti(void)
{
    struct {
        struct anim_box a, b, t;
    } f;
    AnimPart *obj;
    s32 old, cur;

    if (gYetiState != 3)
        gYetiX += ((gActorList)->x - gYetiX) / 32;
    obj = gYeti;
    old = Q8_TO_INT(obj->animTime);
    obj->animTime += (s16)obj->animTimer;
    obj->animDone = 0;
    if (obj->GetAnimFrameBaseOffset() >= obj->anims[obj->animIndex].loopThreshold) {
        ANIM_REWIND(obj->animTime, obj->anims[obj->animIndex]);
        obj->animDone = 1;
    }
    gYetiStateFuncs[gYetiState]();
    obj = gYeti;
    cur = Q8_TO_INT(obj->animTime);
    if (old != cur) {
        gUnpackNibbleTilesFunc(
            (u16 *)((u8 *)obj->frameOffsets[obj->anims[obj->animIndex].frameIndex + cur] + 4),
            gYetiBg2Page);
        gYetiBg2PageFlip = 1;
    }
    SetActorBgLayerDepth(gYetiDistance);
    UpdateYetiPalette();
    f.a = gYetiCatchBox;
    BoxMove(&f.a, Q8_TO_INT(gYetiX), 0, Q8_TO_INT(gYetiPosition));
    if ((u32)gYetiState <= 1) {
        ActorSelf **playerAddr = &gActorList;
        ActorSelf *pl;
        struct anim_box *b;

        if (gPolarPlayerInactive != 0)
            return;
        pl = *playerAddr;
        f.t = pl->box;
        BoxMove(&f.t, Q8_TO_INT(pl->x), Q8_TO_INT(pl->y), Q8_TO_INT(pl->z));
        f.b = f.t;
        b = &f.b;
        MemCopy32(b, b, sizeof(*b));
        if (BoxOverlap(b, &f.a)) {
            AnimPart *g;

            gYetiState = 2;
            g = gYeti;
            g->RestartAnim(2);
            static_cast<PolarPlayer *>(gActorList)->Catch();
            SetCellAnimSpeed(0);
        }
    }
}

/* Palette-gradient cursor for the `gYeti` "gauge" object.
 * Below `0x5000`, DMAs a fixed 16-color gradient (`gYetiPalette`)
 * straight into BG palette RAM at `0x050001E0` (`REG_DMA3` at
 * `0x040000D4`). Above `0xBE00`, DMAs a single zeroed halfword instead
 * (blanking the gradient). In between, computes a `__divsi3`-scaled
 * factor from how far `gYetiDistance` sits into that `[0x5000,
 * 0xBE00]` range, then directly writes 16 colors: for each
 * `gYetiPalette` source halfword (a packed BGR555 color), its
 * 5-bit R and G channels are scaled by that factor (`>>8` after the
 * multiply) and repacked as `R | (G<<5) | (G<<10)` - the ROM really
 * reuses the scaled green for blue - into BG palette RAM at
 * `0x050001E0` onward, a manual brightness ramp rather than a second DMA.
 *
 * The two `0x1f` masks are separate locals: the ROM keeps one in `ip`
 * (set before the pointers) and one in `r7` (set after them). */
void UpdateYetiPalette(void)
{
    s32 v = gYetiDistance;

    if (v <= 0x4fff) {
        DmaCopy16(3, gYetiPalette, (void *)(PLTT + 0x1E0), 0x20);
    } else if (v > 0xbdff) {
        DmaFill16(3, 0, (void *)(PLTT + 0x1E0), 0x20);
    } else {
        s32 f = Q8_DIV(0xbe00 - v, 0x6e00);
        s32 mask = 0x1f;
        u16 *dst = (u16 *)(PLTT + 0x1E0);
        const u16 *src = gYetiPalette;
        s32 mask2 = 0x1f;
        s32 i;

        for (i = 15; i >= 0; i--) {
            u16 c = *src;
            s32 r = Q8_MUL(mask2 & c, f);
            s32 g = Q8_MUL((c >> 5) & mask, f);

            *dst = RGB16(r, g, g);
            dst++;
            src++;
        }
    }
}

/* Companion to `UpdateYetiPalette` above: the gauge's affine BG2 setup. When
 * `gYetiBg2PageFlip` is set, flips `REG_BG2CNT` (`0x0400000C`) between
 * two screen-base words according to `gYetiBg2Page`, clears `C1`
 * and toggles `C0`. Then derives a zoom factor from `gYetiDistance`
 * (`/0x5500`), writes `REG_BG2X` (`0x04000028`) from
 * `gYetiX` and `GetActorBgCenterX()`, `REG_BG2Y` (`0x0400002C`)
 * from `GetActorBgCenterY()`, and the `PA`/`PB`/`PC`/`PD` matrix at
 * `0x04000020` as `scale, 0, 0, scale`.
 *
 * Matches under old_agbcc. */
void UpdateYetiBg2(void)
{
    s32 scale, base, t;

    if (gYetiBg2PageFlip != 0) {
        if (gYetiBg2Page != 0)
            REG_BG2CNT = 0x1a09;
        else
            REG_BG2CNT = 0x1b09;
        gYetiBg2PageFlip = 0;
        gYetiBg2Page ^= 1;
    }
    scale = Q8_DIV(gYetiDistance, 0x5500);
    base = GetActorBgCenterX();
    t = Q8_DIV(gYetiX * 47, gYetiDistance) + base;
    *(vs32 *)REG_ADDR_BG2X = 0x4000 - Q8_MUL(t, scale);
    *(vs32 *)REG_ADDR_BG2Y = 0x4400 - Q8_MUL(GetActorBgCenterY(), scale);
    {
        vu16 *pa = (vu16 *)REG_ADDR_BG2PA;

        *pa++ = scale;
        *pa++ = 0;
        *pa++ = 0;
        *pa = scale;
    }
}
