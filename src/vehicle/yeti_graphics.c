#include "core.h"
#include "actor_self.h"
#include "actor_anim.h"
#include "system.h"
#include "vehicle.h"

/* Sits right after yeti_states.c's `YetiStateCharge` and before
 * yeti.c's `StopYeti` - the whole contiguous range that used
 * to be `asm/code_3_2_20_28568_c99c_dd9c.s`. Both functions continue the
 * `gYeti`-rooted "gauge" object documented in yeti_states.c/
 * yeti_update.c's header comments. */

/* `UpdateYeti`'s (yeti_update.c) shared AABB-overlap-test tail,
 * factored out as its own function taking `self` explicitly instead of
 * always reading the player global - used by `UpdatePolarCheckpointCrate`
 * (polar_aku_aku.c, already matched, called as `IsTouchingYeti(self)`)
 * among others. Same 12-byte `{s16 x, y, z, sizeX, sizeY, sizeZ}` record
 * shape and same self-copy-through-`MemCopy32` idiom as `UpdateYeti`
 * - see that function's doc comment for the full record-layout writeup.
 * Box A: `gYetiBox` (a record adjacent to `UpdateYeti`'s
 * own `gYetiCatchBox` - literal-pool-verified 0xC bytes apart)
 * with `gYetiX`/`gYetiPosition` (both `>>8`) added into its `x`/
 * `z` fields only. Box B: `self+0x38`'s own 12-byte vector, with
 * `self`'s own `+0x1c`/`0x20`/`0x24` position (all `>>8`) added into
 * all three of `x`/`y`/`z` - this is the "self+0x38's own vector"
 * referenced from docs/matching/archive/issue-54-actor-d3a8.md's original
 * parked writeup.
 *
 * Same frame-struct shape as `UpdateYeti`: A, B and the self box are
 * members of one stack struct so their addresses are rematerialized
 * from sp instead of being kept in callee-saved registers, and only
 * `&f.b` goes through a pointer local across the `MemCopy32` call.
 * Needs old_agbcc (docs/matching/archive/issue-51-54-naked-retry.md). */
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

u8 IsTouchingYeti(struct actor_self *self)
{
    struct {
        struct anim_box a, b, t;
    } f;
    struct anim_box *b;

    f.a = gYetiBox;
    BoxMove(&f.a, gYetiX >> 8, 0, gYetiPosition >> 8);
    f.t = *(struct anim_box *)self->box;
    BoxMove(&f.t, self->x >> 8, self->y >> 8, self->z >> 8);
    f.b = f.t;
    b = &f.b;
    MemCopy32(b, b, sizeof(*b));
    return BoxOverlap(&f.a, b);
}

/* The `gYeti` object's own initial VRAM-pattern/DMA setup
 * (called once from `CreateYeti`'s constructor, yeti.c): sets
 * `REG_DISPCNT`'s OBJ-window-enable bit (`DISPCNT_OBJWIN_ON`, bit 15),
 * then runs the same 16x16 BG2 map-fill loop twice into a
 * 0x100-byte stack buffer (`BuildYetiBg2Map`'s own loop body, parameterized
 * there by seed/destination but fixed here to a `0`/`0x80` seed pair) -
 * DMA3-transferring the first fill to VRAM tile `0x0600D000` and the
 * second to `0x0600D800` (`REG_DMA3SAD`/`DAD`/`CNT` at `0x040000D4`,
 * 0x80 words, 32-bit transfers). Then clears a third tile
 * (`0x0600BFC0`-`0x0600BFFC`) word-by-word, arms the object
 * (`gYetiBg2Page = 1`), passes the object's current frame data
 * (past its 4-byte header) to the `gUnpackNibbleTilesFunc` hook, latches
 * `gYetiBg2PageFlip`, and
 * finally calls `UpdateYetiBg2`/`UpdateYetiPalette` (yeti_update.c) to prime
 * the gauge's sound/palette state immediately.
 *
 * The frame pointer goes through the usual `CurFrame()` inline with the
 * global passed straight in: a `struct actor_self *obj` local puts
 * `&gYeti` last in the r8/sb/sl assignment, where the ROM
 * gives it r8 (docs/matching/archive/issue-51-54-naked-retry.md). */
static inline void FillDotPattern(u8 *dst, u8 seed)
{
    s32 y, x;

    for (y = 0; y < 16; y++) {
        for (x = 0; x < 16; x++) {
            if ((u32)(x - 3) > 9 || y <= 2 || y > 12)
                dst[y * 16 + x] = 0xff;
            else
                dst[y * 16 + x] = seed++;
        }
    }
}

static inline u8 *CurFrame(struct actor_self *self)
{
    s32 t = self->animTime >> 8;

    return (u8 *)self->frameOffsets[self->anims[self->animIndex].frameIndex + t];
}

void LoadYetiGraphics(void)
{
    u8 buf[0x100];

    REG_DISPCNT |= DISPCNT_BG2_ON;
    FillDotPattern(buf, 0);
    DmaCopy16(3, buf, (void *)(VRAM + 0xD000), 0x100);
    FillDotPattern(buf, 0x80);
    DmaCopy16(3, buf, (void *)(VRAM + 0xD800), 0x100);
    {
        s32 base = VRAM + 0xBFC0;
        u32 zero = 0;
        s32 p;

        for (p = base + 0x3c; p >= base; p -= 4)
            *(u32 *)p = zero;
    }
    gYetiBg2Page = 1;
    gUnpackNibbleTilesFunc((u16 *)(CurFrame(gYeti) + 4), 1);
    gYetiBg2PageFlip = 1;
    UpdateYetiBg2();
    UpdateYetiPalette();
}
