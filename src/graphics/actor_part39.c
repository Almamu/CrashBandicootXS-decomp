#include "core.h"
#include "actor.h"

extern void sub_8007A84(void *self, void *part);
extern void *gUnknown_030012CC;

/* Calls `sub_8007A84` (already matched in `actor_part.c`) with the
 * global `gUnknown_030012CC` as `self` - same tail-call shape as
 * `sub_8008350` (`actor_part5.c`), but here as a leading step rather
 * than the whole body. If `part+0x38` (a field not otherwise
 * characterized yet in this ROM region) is nonzero, also clears
 * `part->flags` bit 3 - written as `& -9` (the established
 * negative-constant bit-clear idiom, see `matching.md`'s `& -5`/`& -2`/
 * `& -3` entries) to reproduce the ROM's runtime `movs`+`rsbs` instead
 * of a folded immediate AND. */
void sub_80119A8(struct actor *part)
{
    sub_8007A84(gUnknown_030012CC, part);
    if (*((u8 *)part + 0x38) != 0) {
        register s32 mask asm("r0") = -9;
        register u8 flags asm("r1") = part->flags;

        mask &= flags;
        part->flags = mask;
    }
}

/* Always-2 stub - same shape as `sub_8008480`'s always-true stub
 * (`actor_part6.c`). */
s32 sub_80119D4(void)
{
    return 2;
}

extern void sub_8008484(struct actor *self, u32 arg1);
extern u8 gStaticData_087E414C[];

/* Sets `self->table` then tail-calls `sub_8008484` (already matched in
 * `actor_part6.c`), which unconditionally overwrites `table` again
 * with `gStaticData_087E3BEC` - so this function's own store is
 * immediately clobbered by the callee. Kept faithfully anyway; the
 * compiler can't see through the opaque call to know the store is
 * dead. */
void sub_80119D8(struct actor *self, u32 arg1)
{
    self->table = gStaticData_087E414C;
    sub_8008484(self, arg1);
}

/* Sets flag bit 6, clears `self+0x48` (a field not yet characterized
 * in this ROM region - see the neighboring `sub_80119A8`'s `+0x38`). */
void sub_80119EC(struct actor *self)
{
    register u8 mask asm("r1") = 0x40;

    mask |= self->flags;
    *(volatile u8 *)&self->flags = mask;
    *((u8 *)self + 0x48) = 0;
}

extern struct actor *sub_80084A4(struct actor *self);

/* Re-initializes `self` via `sub_80084A4` (already matched in
 * `actor_part6.c`), then overwrites its table with
 * `gStaticData_087E414C` and runs `sub_80119EC` on it. */
struct actor *sub_80119FC(struct actor *self)
{
    sub_80084A4(self);
    self->table = gStaticData_087E414C;
    sub_80119EC(self);
    return self;
}

extern void *sub_803AD7C(void *arg0, void *arg1);
extern void *gUnknown_030012D8;

/* If `self+0x48` is zero and the player (`gUnknown_030012D8`)'s top
 * flag bit is set, fires a `self->table+0x68`-driven trampoline (the
 * same idiom documented in `actor_part12.c`/`actor_part13.c`) on
 * `self` itself. Always returns 0. */
s32 sub_8011A1C(struct actor *self)
{
    if (*((u8 *)self + 0x48) == 0) {
        struct actor *player = gUnknown_030012D8;

        if (player->flags >> 7) {
            u8 *rec = (u8 *)self->table + 0x68;
            s16 offset = *(s16 *)rec;

            sub_803AD7C((u8 *)self + offset, *(void **)(rec + 4));
        }
    }
    return 0;
}

/* Sets `self->x`/`self->y` (Q8 fixed-point) from raw pixel `x`/`y`,
 * and mirrors the result into a second `{x, y}` pair at `self+0x4c`/
 * `+0x50` (not otherwise characterized in this ROM region yet). */
void sub_8011A50(struct actor *self, s32 x, s32 y)
{
    s32 storedX, storedY;

    self->x = x << 8;
    self->y = y << 8;
    storedX = *(volatile s32 *)&self->x;
    storedY = *(volatile s32 *)&self->y;
    *(s32 *)((u8 *)self + 0x4c) = storedX;
    *(s32 *)((u8 *)self + 0x50) = storedY;
}

extern void sub_801191C(struct actor *self);

/* Sets `self+0x4a`/`+0x4b` (not otherwise characterized yet), and if
 * `value == 0xff` also calls `sub_801191C` (still raw asm just above
 * this ROM region, `0x0801191C`) on `self`. */
void sub_8011A64(struct actor *self, s32 value)
{
    u8 *p = (u8 *)self + 0x4a;
    u8 zero = 0;

    *p = value;
    p++;
    *p = zero;
    if (value == 0xff) {
        sub_801191C(self);
    }
}

/* Trivial one-byte setter - `self+0x49` (not otherwise characterized
 * yet). */
void sub_8011A84(struct actor *self, u8 value)
{
    *((u8 *)self + 0x49) = value;
}

extern void sub_8008364(struct actor *part);
extern void *gUnknown_030012B4;

/* Distance-gate: if the player (`gUnknown_030012D8`) is within 0x180
 * (384 px) of `self` on both axes, calls `sub_8008364` (already
 * matched in `actor_part5.c`) on `self`. Otherwise sets `self->flags`
 * bit 0 and, unless `self->field_08 == 0xFFFF`, marks its bit in the
 * same `gUnknown_030012B4+0x108` bitmap `actor_part2.c` already
 * writes - identical idiom, reused verbatim including the
 * register-pinned `>> 5` (see that file's note on why a plain C shift
 * doesn't reproduce the ROM's exact instruction here). */
void sub_8011A8C(struct actor *self)
{
    struct actor *player = gUnknown_030012D8;
    register s32 rawX asm("r0") = player->x;
    register s32 dxPart asm("r1");
    s32 dx;
    s32 dy;

    asm volatile("asr %0, %1, #8" : "=r" (dxPart) : "r" (rawX));
    dx = dxPart - (self->x >> 8);
    if (dx < 0) {
        dx = -dx;
    }
    if (dx > 0x180) {
        goto outOfRange;
    }
    {
        register s32 rawY asm("r0") = player->y;
        register s32 dyPart asm("r1");

        asm volatile("asr %0, %1, #8" : "=r" (dyPart) : "r" (rawY));
        dy = dyPart - (self->y >> 8);
    }
    if (dy < 0) {
        dy = -dy;
    }
    if (dy <= 0x180) {
        goto inRange;
    }

outOfRange:
    {
        register s32 mask asm("r0") = 1;

        mask |= self->flags;
        self->flags = mask;
    }
    {
        register s32 ffff asm("r0") = 0xFFFF;
        register u16 field08a asm("r4") = *(volatile u16 *)&self->field_08;

        if (field08a != ffff) {
            register u16 field08b asm("r3") = *(volatile u16 *)&self->field_08;
            void *base = gUnknown_030012B4;
            register s32 word asm("r0");
            s32 wordOffset;

            asm volatile("add %0, %1, #0\n\tasr %0, %0, #5" : "=r" (word) : "r" ((s32)field08b));
            wordOffset = word << 2;
            {
                u8 *bitmapAddr = (u8 *)base + 0x108;

                bitmapAddr = bitmapAddr + wordOffset;
                word = field08b - (word << 5);
                *(s32 *)bitmapAddr |= 1 << word;
            }
        }
    }
    return;

inRange:
    sub_8008364(self);
}

extern void *sub_8026EDC(s32 size);
extern void nullsub_16(void *self);
extern u8 gStaticData_087E41BC[];

/* Allocates a new `struct actor`-shaped object (`sub_8026EDC(0x40)`,
 * same size as `sub_8008434`'s constructor in `actor_part6.c`),
 * re-initializes it via `sub_80084A4`, overwrites its table with
 * `gStaticData_087E41BC`, and runs the empty `nullsub_16` on it before
 * setting `field_08`/`x`/`y` from the raw pixel arguments. */
struct actor *sub_8011B0C(u16 arg0, u16 arg1, u16 arg2)
{
    struct actor *self = sub_8026EDC(0x40);

    sub_80084A4(self);
    self->table = gStaticData_087E41BC;
    nullsub_16(self);
    self->field_08 = arg0;
    self->x = (s32)arg1 << 8;
    self->y = (s32)arg2 << 8;
    return self;
}

/* Empty stub. */
void nullsub_16(void *self)
{
}

/* Same `table`-set/tail-call-`sub_8008484` shape as `sub_80119D8`
 * above, with a different vtable. */
void sub_8011B5C(struct actor *self, u32 arg1)
{
    self->table = gStaticData_087E41BC;
    sub_8008484(self, arg1);
}

/* Same re-init/table-set/`nullsub_16` shape as `sub_8011B0C` above,
 * but re-initializing an existing `self` instead of allocating a new
 * one - the same relationship `sub_80084A4` itself has to
 * `sub_8008434` (see `actor_part6.c`'s note on that pair). */
struct actor *sub_8011B70(struct actor *self)
{
    sub_80084A4(self);
    self->table = gStaticData_087E41BC;
    nullsub_16(self);
    return self;
}

/* Zeroes/initializes a run of fields from `self+0x25` through `+0x34`
 * (not otherwise characterized yet), plus `self+8`/`+0x10`/`+0x14`/
 * `+0x18`/`+0x1c`, and sets `+0x2f`/`+0x30` to 1. This is the
 * constructor sibling `sub_801588C` (still raw, `0x0801588C`) calls
 * right after wiring up `gStaticData_087E4224` - that caller stores
 * its own vtable pointer at `+0xc`, not `+0x18` the way `struct actor`
 * does, so `self` here is a *different*, still-unnamed "child object"
 * struct - the same one several large state machines in this ROM
 * region (`sub_8011BD4` etc., left raw for now) read/write through
 * many more offsets not characterized here. Kept as a raw `void *`
 * rather than `struct actor *` to avoid implying it shares that
 * layout. */
void sub_8011B90(void *selfArg)
{
    register u8 *p asm("r3") = (u8 *)selfArg;
    register u8 *q asm("r1") = p + 0x29;
    register s32 zero asm("r0") = 0;

    *q = zero;
    *(s32 *)(p + 8) = zero;
    q += 3;
    *q = zero;
    q -= 5;
    *q = zero;
    q += 1;
    *q = zero;
    q += 7;
    *q = 1;
    q += 1;
    *q = 1;
    *(s32 *)(p + 0x14) = zero;
    *(s32 *)(p + 0x10) = zero;
    q -= 0xa;
    *q = zero;
    q += 4;
    *q = zero;
    q -= 5;
    *q = zero;
    q += 6;
    *q = zero;
    *(s32 *)(p + 0x18) = zero;
    *(s32 *)(p + 0x1c) = zero;
    q += 8;
    *q = zero;
    q += 1;
    *q = zero;
}
