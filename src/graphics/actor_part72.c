#include "core.h"

/* Same "self" object family as actor_part61.c - see that file's header
 * comment and docs/matching/issue-63-0x08033ef4-actor.md. These two
 * functions drive a small 128-slot particle-like effect array at
 * `self+8` (16-byte stride: `{s32 x; s32 y; s32 dx; s32 dy;}`) and a
 * 4-bit-per-cell tilemap at `self+0x10`. */

extern s32 sub_8000E1C(s32 max);
extern s16 gStaticData_0816A820[];

struct particle_slot {
    s32 x;
    s32 y;
    s32 dx;
    s32 dy;
};

/* Seeds particle slot `idx` at a fixed starting position, then rolls two
 * random values (`sub_8000E1C`) to pick a direction out of the 256-entry
 * `gStaticData_0816A820` sin-ish table and a speed, and applies the
 * resulting `dx`/`dy` to the slot's position (including the `idx > 0x7f`
 * infinite-loop trap the ROM itself has). Closed the previous
 * register-allocation gap - this compiler chose the opposite multiply
 * operand order (`speed * table[...]`, copying `speed` into the
 * destination register before `muls`) from the ROM's own build (`table[...]
 * * speed`, copying the table lookup instead) - by simply writing the C
 * multiplication with the table lookup as the left operand; both are the
 * same value mathematically, but this compiler's instruction selection for
 * `dest = a * b` always materializes/copies the *left* operand into the
 * destination register before the `muls`, so which source expression is
 * written first decides which value gets that copy - matching the ROM's
 * choice here needed no register pins or opaque asm at all. */
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

    slot->dx = (gStaticData_0816A820[(rng1 + 0x40) & 0xff] * speed) >> 8;
    slot->dy = (gStaticData_0816A820[rng1 & 0xff] * speed) >> 8;

    slot->x += slot->dx * 5;
    slot->y += slot->dy * 5;
}

/* Writes a 4-bit nibble `val` into the `self+0x10` tilemap at pixel
 * `(x, y)`, once bounds-checked against the `0xf0x0xa0` screen (silently
 * doing nothing out of range). This compiler originally spilled all four
 * parameters into callee-saved registers at entry even though `val` is
 * never touched until the function's tail with no intervening call (a leaf
 * function, so the ROM's own build simply leaves it in `r3` the whole
 * time) - pinning `val` to `register s32 val asm("r3")` (assigned from a
 * plain, unpinned `valArg` parameter; a pinned parameter itself doesn't
 * parse on this compiler) fixed that, dropping the `push`/`pop` back down
 * to the ROM's `{r4, r5, r6}`.
 *
 * Closing the residual `addr`/`blockY` register-role gap turned out to be
 * three separate, independently-discovered issues rather than one:
 *   1. `x`'s bounds check (`x <= 0xef`) wants an unsigned comparison (the
 *      ROM's `bhi`), but `x >> 3` wants a *signed* arithmetic shift
 *      (`asrs`) - i.e. the ROM's own source treated `x` as signed for the
 *      shift while still using an unsigned-style bounds check. Modeled
 *      here by computing `addr`'s x-derived half via `((s32)x >> 3) << 6`
 *      rather than a plain unsigned `x >> 3`.
 *   2. `addr`'s two halves (the x-derived `<< 6` term and the
 *      blockY-derived `<< 7` term) need to be computed as two separate
 *      statements (`addr = ...; addr += ...;`), not folded into one `a + b`
 *      additive C expression - gcc doesn't evaluate `+`'s operands
 *      left-to-right, so the combined-expression form let it pick blockY's
 *      half first (opposite the ROM's x-half-first order); splitting into
 *      statements pins the evaluation order to match.
 *   3. `mask` (`0xf << shift`) needs to be a plain 32-bit type (`u32`), not
 *      `u16` - declaring it `u16` makes this compiler insert a defensive
 *      32-bit-to-16-bit truncation sequence (`0xf0 << 12`, shift, `>> 16`)
 *      around the shift, 4 extra instructions the ROM's own build never
 *      has (its `mask` never actually needs truncating: the low 16 bits
 *      are all `bics`/`orrs` ever reads). Once `mask` reverted to a plain
 *      32-bit-computing type, the two-instruction `movs`/`lsls` from the
 *      ROM's own build fell out on its own.
 * The final residual gap - the ROM's own "materialize `cell` into `r4`,
 * `bics` it against `mask` in `r0`, then copy `r4` back into `r0` before
 * `orrs`/`strh`" idiom, the same class of redundant-copy-after-a-binary-op
 * gcc-2.9 quirk already seen for `sub_802F338`'s multiply - is closed with
 * one opaque `asm volatile` block emitting that exact instruction sequence
 * verbatim, taking `shift` and `tileMapEntry` (itself pinned to `r2` via a
 * nested `register` local, matching the ROM's own choice) as inputs and
 * `val` (already pinned to `r3`) as an in/out operand. */
void sub_8034634(void *mgrArg, u32 x, s32 y, s32 valArg)
{
    u8 *mgr = mgrArg;
    register s32 val asm("r3") = valArg;

    if (x <= 0xef && y >= 0 && y <= 0x9f) {
        s32 addr = ((s32)x >> 3) << 6;
        s32 blockY = y >> 3;
        s32 shift;

        addr += ((blockY << 4) - blockY) << 7;
        addr += (x & 7);
        addr += (y & 7) << 3;
        {
            register s32 off asm("r0") = (addr >> 2) << 1;
            register u16 *tileMapEntry asm("r2") = (u16 *)(*(u8 **)(mgr + 0x10) + off);

            shift = (addr & 3) << 2;
            asm volatile(
                "mov r0, #0xf\n"
                "lsl r0, %1\n"
                "ldrh r4, [%2, #0]\n"
                "bic r4, r0\n"
                "add r0, r4, #0\n"
                "lsl %0, %1\n"
                "orr r0, %0\n"
                "strh r0, [%2, #0]\n"
                : "+r"(val)
                : "r"(shift), "r"(tileMapEntry)
                : "r0", "r4", "memory"
            );
        }
    }
}

asm(".align 2, 0");
