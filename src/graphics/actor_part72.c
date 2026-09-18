#include "core.h"

/* Same "self" object family as actor_part61.c - see that file's header
 * comment and docs/matching/issue-63-0x08033ef4-actor.md. These two
 * functions drive a small 128-slot particle-like effect array at
 * `self+8` (16-byte stride: `{s32 x; s32 y; s32 dx; s32 dy;}`) and a
 * 4-bit-per-cell tilemap at `self+0x10`. */

#if NON_MATCHING
/* NOT YET BYTE-MATCHING - see docs/matching/issue-63-0x08033ef4-actor.md,
 * "Parked: sub_80345B0" for the full account; compiled only under
 * `make NON_MATCHING=1`, the checked-in assembly
 * (asm/code_3_2_20_28568_c99c_31784_33ef4_345b0.s) is used otherwise.
 * Seeds particle slot `idx` at a fixed starting position, then rolls two
 * random values (`sub_8000E1C`) to pick a direction out of the 256-entry
 * `gStaticData_0816A820` sin-ish table and a speed, and applies the
 * resulting `dx`/`dy` to the slot's position. Semantics fully understood
 * and every field/call confirmed correct (including the `idx > 0x7f`
 * infinite-loop trap the ROM itself has); parked on this compiler
 * choosing a different register allocation for the 16-bit table lookups
 * and the final multiply/shift than the ROM's own build, despite several
 * register-pin attempts (including the `ldrsh r1,[r1,r3]` self-
 * overwriting-base idiom and reusing the table-base register across both
 * lookups). */
extern s32 sub_8000E1C(s32 max);
extern s16 gStaticData_0816A820[];

struct particle_slot {
    s32 x;
    s32 y;
    s32 dx;
    s32 dy;
};

void sub_80345B0(void *mgrArg, s32 idx)
{
    u8 *mgr = mgrArg;
    struct particle_slot *slot;
    s32 rng1;
    s32 speed;

    if (idx > 0x7f) {
        for (;;) {
        }
    }

    slot = (struct particle_slot *)(*(u8 **)(mgr + 8) + (idx << 4));
    slot->x = 0x7800;
    slot->y = 0x5000;

    rng1 = (u16)sub_8000E1C(0x100);
    speed = (u16)sub_8000E1C(0x200) + 0x100;

    slot->dx = (speed * gStaticData_0816A820[(rng1 + 0x40) & 0xff]) >> 8;
    slot->dy = (speed * gStaticData_0816A820[rng1 & 0xff]) >> 8;

    slot->x += slot->dx * 5;
    slot->y += slot->dy * 5;
}

/* NOT YET BYTE-MATCHING - see docs/matching/issue-63-0x08033ef4-actor.md,
 * "Parked: sub_8034634" for the full account; compiled only under
 * `make NON_MATCHING=1`, the checked-in assembly
 * (asm/code_3_2_20_28568_c99c_31784_33ef4_345b0.s) is used otherwise.
 * Writes a 4-bit nibble `val` into the `self+0x10` tilemap at pixel
 * `(x, y)`, once bounds-checked against the `0xf0x0xa0` screen
 * (silently doing nothing out of range). Semantics fully understood and
 * every field/shift confirmed correct. This compiler originally spilled
 * all four parameters into callee-saved registers at entry even though
 * `val` is never touched until the function's tail and no call happens
 * in between (a leaf function, so the ROM's own build simply leaves it
 * in `r3` the whole time) - pinning `val` to `register s32 val
 * asm("r3")` (assigned from a plain, unpinned `valArg` parameter; a
 * pinned parameter itself doesn't parse on this compiler) fixed that
 * specific gap, dropping the `push`/`pop` back down to the ROM's
 * `{r4, r5, r6}`. The residual gap is narrower now: this compiler
 * computes the `(x>>3)<<6`/`blockY`-derived halves of `addr` into `r2`/
 * `r1` respectively, opposite the ROM's `r1`/`r2` - every register-pin
 * variant tried on `addr`/`blockY` individually either left the swap in
 * place or corrupted a neighboring instruction's encoding (e.g. turned
 * an `adds` into an `orrs`), so the swap was left as the residual
 * NON_MATCHING gap rather than risk a wrong-but-plausible-looking
 * match. */
void sub_8034634(void *mgrArg, u32 x, s32 y, s32 valArg)
{
    u8 *mgr = mgrArg;
    register s32 val asm("r3") = valArg;

    if (x <= 0xef && y >= 0 && y <= 0x9f) {
        s32 blockY = y >> 3;
        s32 addr = ((x >> 3) << 6) + (((blockY << 4) - blockY) << 7);
        u16 *tileMapEntry;
        s32 shift;
        u16 mask;
        u16 cell;

        addr += (x & 7);
        addr += (y & 7) << 3;
        tileMapEntry = (u16 *)(*(u8 **)(mgr + 0x10) + ((addr >> 2) << 1));
        shift = (addr & 3) << 2;
        mask = 0xf << shift;
        cell = *tileMapEntry;
        cell &= ~mask;
        cell |= val << shift;
        *tileMapEntry = cell;
    }
}
#endif /* NON_MATCHING */

asm(".align 2, 0");
