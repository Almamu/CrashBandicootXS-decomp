#include "core.h"
#include "actor.h"

extern s32 gUnknown_03001298;

#if NON_MATCHING
/* A velocity/position integrator: for each axis (X at `self+0x60`,
 * `self+0x50` max, `self+0x4c` accel; Y at `self+0x64`/`self+0x5c`/
 * `self+0x58`), steps the velocity toward its max by the accel amount,
 * clamping so it never overshoots. Builds a "direction" byte at
 * `self+0x24` from the sign of each clamped velocity (1=right,
 * 2=left, 8=down, 4=up, OR'd together, matching this ROM's earlier
 * `sub_8007B00`-style mirror-flag bit encoding). Caches the pre-move
 * position at `self+0x6c`/`self+0x70` (read back by `sub_8009EB0`/
 * `sub_8009EBC`/`sub_8009EC4`), then applies the clamped velocity to
 * `self+0`/`self+4`. Finally updates the global `gUnknown_03001298`
 * with the Y velocity (a redundant-looking early write of 0 happens
 * only on the path where the Y velocity is already 0, so it's a
 * genuine no-op preserved as found) and returns whether either axis
 * is still moving.
 *
 * NOT YET BYTE-MATCHING, but a true `push`/`pop`-free leaf function
 * now (matching the ROM's own register budget exactly) after modeling
 * this reconstruction on `sub_800B270`'s own working shape (see
 * `docs/matching/issue-9-0x08007634-actor.md` - same per-axis clamp
 * structure, same `self` pinned to `r2`, same `vs32`-forced reloads
 * for the ROM's own redundant `self->x`/`self->y` re-reads right
 * before applying velocity): every branch, comparison, field offset,
 * and register role now matches one-for-one through the position
 * update, including the dirFlags OR-combine's accumulator-register
 * pattern and its `goto`-based skip when an axis's velocity is
 * exactly zero. The remaining gap is the same one `sub_800B270` itself
 * is still parked on: the trailing `gUnknown_03001298` block's
 * address/value register roles - the ROM loads the global's address
 * into `r0` and its value into `r2`, while this compiler's natural
 * allocation keeps the opposite (address in `r2`, value in `r0` -
 * functionally identical, same instruction count, wrong register
 * letters). Register-pinning either side of that swap reproduces
 * `sub_800B270`'s own two failure modes exactly: pinning the loaded
 * *value* to a specific register (`r0` or `r2`) makes this compiler's
 * dead-store-elimination pass prove the whole conditional redundant
 * and delete it outright (the unconditional final store already
 * writes the same value either way, so the pass isn't wrong, just not
 * what the ROM's own build produced); pinning the *address* to `r0`
 * instead reintroduces a `push {r4, lr}`/`pop {r4}` pair elsewhere in
 * the function, this time to preserve `vy` across the reallocation.
 * Parked on this one shared address/value register-letter swap. */
s32 sub_8009DF4(void *arg0)
{
    register s32 *w asm("r2") = (s32 *)arg0;
    register u8 *flags asm("r1");
    s32 fx, fy;

    {
        register s32 v asm("r1") = w[0x60 / 4];
        register s32 target asm("r3") = w[0x50 / 4];

        if (v >= target) goto case1_ge;
        {
            s32 step = w[0x4c / 4];
            register s32 result asm("r0") = v + step;
            w[0x60 / 4] = result;
            if (result <= target) goto case1_done;
            goto case1_clamp;
        }
    case1_ge:
        if (v <= target) goto case1_done;
        {
            s32 step = w[0x4c / 4];
            register s32 result asm("r0") = v - step;
            w[0x60 / 4] = result;
            if (result >= target) goto case1_done;
        }
    case1_clamp:
        w[0x60 / 4] = target;
    case1_done:
        ;
    }

    {
        register s32 v asm("r1") = w[0x64 / 4];
        register s32 target asm("r3") = w[0x5c / 4];

        if (v >= target) goto case2_ge;
        {
            s32 step = w[0x58 / 4];
            register s32 result asm("r0") = v + step;
            w[0x64 / 4] = result;
            if (result <= target) goto case2_done;
            goto case2_clamp;
        }
    case2_ge:
        if (v <= target) goto case2_done;
        {
            s32 step = w[0x58 / 4];
            register s32 result asm("r0") = v - step;
            w[0x64 / 4] = result;
            if (result >= target) goto case2_done;
        }
    case2_clamp:
        w[0x64 / 4] = target;
    case2_done:
        ;
    }

    flags = (u8 *)w + 0x24;
    *flags = 0;

    fx = w[0x60 / 4];
    if (fx > 0) *flags = 1;
    else if (fx < 0) *flags = 2;

    fy = w[0x64 / 4];
    {
        register s32 mask asm("r0");
        if (fy > 0) {
            mask = 8;
        } else if (fy < 0) {
            mask = 4;
        } else {
            goto skipY;
        }
        mask = mask | *flags;
        *flags = mask;
    }
skipY:

    {
        s32 x0 = w[0];
        s32 y0 = w[1];
        w[0x6c / 4] = x0;
        w[0x70 / 4] = y0;
    }
    {
        s32 x = *(vs32 *)&w[0];
        register s32 vx asm("r3") = w[0x60 / 4];
        x = x + vx;
        w[0] = x;
        {
            s32 y = *(vs32 *)&w[1];
            s32 vy = w[0x64 / 4];
            y = y + vy;
            w[1] = y;

            {
                s32 *g = &gUnknown_03001298;

                if (*g != 0 && vy == 0) {
                    *g = vy;
                }
                *g = vy;
            }

            return (vx != 0 || vy != 0);
        }
    }
}
#endif /* NON_MATCHING */

/* `self+0x6c`/`self+0x70` (previous position, cached by `sub_8009DF4`
 * above) get/set accessors. */
void sub_8009EA8(void *self, s32 x, s32 y)
{
    *(s32 *)((u8 *)self + 0x6c) = x;
    *(s32 *)((u8 *)self + 0x70) = y;
}

void sub_8009EB0(void *dest, void *self)
{
    s32 y = *(s32 *)((u8 *)self + 0x70);
    s32 x = *(s32 *)((u8 *)self + 0x6c);
    *(s32 *)dest = x;
    *(s32 *)((u8 *)dest + 4) = y;
}

/* Q8-to-integer converters for the same previous-position fields. */
s32 sub_8009EBC(void *self)
{
    return *(s32 *)((u8 *)self + 0x70) >> 8;
}

s32 sub_8009EC4(void *self)
{
    return *(s32 *)((u8 *)self + 0x6c) >> 8;
}

/* Constant-5 stub. */
s32 sub_8009ECC(void)
{
    return 5;
}

extern void *sub_8026EDC(s32 size);
extern struct actor *sub_80084A4(struct actor *self);
extern u8 gStaticData_087E3D14[];
extern void sub_8009F50(void *part);

/* Same shape as `sub_8008434`/`sub_8009ED0`'s siblings elsewhere in
 * this ROM region: allocates a bigger (0x78-byte) part-object,
 * initializes it via `sub_80084A4`, overwrites its table with
 * `gStaticData_087E3D14`, clears its extra fields via `sub_8009F50`
 * (see below), then sets `field_08` and the Q8 `x`/`y` position from
 * the three `u16` arguments. */
struct actor *sub_8009ED0(u16 arg0, u16 arg1, u16 arg2)
{
    struct actor *part = sub_8026EDC(0x78);

    sub_80084A4(part);
    part->table = gStaticData_087E3D14;
    sub_8009F50(part);
    part->field_08 = arg0;
    part->x = (s32)arg1 << 8;
    part->y = (s32)arg2 << 8;
    return part;
}

extern s32 sub_803AD80(void *arg0, void *arg1, void *fn);
extern void sub_8008484(struct actor *self, u32 arg1);

/* Overwrites `self->table`, then (if `self+0x44`'s record is set)
 * fires a `record->table+0x48/0x4c`-driven trampoline with a constant
 * argument `3` via `sub_803AD80` (same convention as
 * `sub_800891C`/`sub_8006FE4`), and finally tail-calls `sub_8008484`
 * (already matched in `actor_part6.c`). The trampoline's `addr =
 * rec + offset` needed computing before the `fn` load (reusing
 * `rec`'s own dying register), matching the accumulator-register
 * pattern used throughout this ROM region - computing them in the
 * opposite order aliases `rec` and `fn` onto the same register and
 * silently corrupts the address. */
void sub_8009F1C(struct actor *self, u32 arg1)
{
    self->table = gStaticData_087E3D14;

    {
        register void *rec asm("r2") = *(void **)((u8 *)self + 0x44);

        if (rec != 0) {
            register u8 *tblAdj asm("r1") = *(u8 **)((u8 *)rec + 0xc) + 0x48;
            register s32 offset asm("r0") = *(s16 *)tblAdj;
            register void *addr asm("r0");
            register void *fn asm("r2");

            addr = (u8 *)rec + offset;
            fn = *(void **)(tblAdj + 4);
            sub_803AD80(addr, (void *)3, fn);
        }
    }

    sub_8008484(self, arg1);
}

/* Part-object field clearer/initializer, called from every
 * `sub_8009ED0`-family constructor above and below. Sets `flags` bit
 * 6, clears `part+0xd` bit 3 (same `-9` mask trick as `sub_8008680`),
 * zeroes the velocity/accel/max-velocity fields consumed by
 * `sub_8009DF4` (`+0x60`/`+0x64`/`+0x48`/`+0x4c`/`+0x50`/`+0x54`/
 * `+0x58`/`+0x5c`) plus `+0x24`/`+0x44`/`+0x40`, and sets `+0x68` to
 * 8 and clears `+0x69`. */
void sub_8009F50(void *self)
{
    {
        register s32 mask asm("r0") = 0x40;
        register s32 byte asm("r1") = *((u8 *)self + 0xc);
        register s32 result asm("r0");

        result = mask | byte;
        *((u8 *)self + 0xc) = result;
    }
    {
        register s32 mask asm("r0") = -9;
        register s32 byte asm("r1") = *((u8 *)self + 0xd);
        register s32 result asm("r0");

        result = mask & byte;
        *((u8 *)self + 0xd) = result;
    }

    *(s32 *)((u8 *)self + 0x60) = 0;
    *(s32 *)((u8 *)self + 0x64) = 0;
    *(s32 *)((u8 *)self + 0x48) = 0;
    *(s32 *)((u8 *)self + 0x4c) = 0;
    *(s32 *)((u8 *)self + 0x50) = 0;
    *(s32 *)((u8 *)self + 0x54) = 0;
    *(s32 *)((u8 *)self + 0x58) = 0;
    *(s32 *)((u8 *)self + 0x5c) = 0;
    *((u8 *)self + 0x68) = 8;
    *((u8 *)self + 0x24) = 0;
    *((u8 *)self + 0x69) = 0;
    *(s32 *)((u8 *)self + 0x44) = 0;
    *(s32 *)((u8 *)self + 0x40) = 0;
}

/* Same `sub_80084A4`/table-swap/`sub_8009F50` shape as `sub_8009ED0`
 * above, but re-initializes an existing `part` instead of allocating
 * a new one - the same relationship `sub_80084A4` itself has to
 * `sub_8008434`. */
struct actor *sub_8009F90(struct actor *part)
{
    sub_80084A4(part);
    part->table = gStaticData_087E3D14;
    sub_8009F50(part);
    return part;
}

extern void sub_8008364(struct actor *part);

/* Calls `sub_8008364` (already matched in `actor_part5.c`), then (if
 * `self+0x44`'s record is set) fires a `record->table+8/0xc`-driven
 * trampoline via `sub_803AD80` with `self` itself as the second
 * argument. Same `addr`-before-`fn` ordering fix as `sub_8009F1C`
 * above. */
void sub_8009FB0(struct actor *self)
{
    register void *rec asm("r2") = *(void **)((u8 *)self + 0x44);

    sub_8008364(self);
    rec = *(void **)((u8 *)self + 0x44);
    if (rec != 0) {
        register u8 *tbl asm("r1") = *(u8 **)((u8 *)rec + 0xc);
        register s32 offset asm("r0") = *(s16 *)(tbl + 8);
        register void *addr asm("r0");
        register void *fn asm("r2");
        register void *arg1 asm("r1");

        addr = (u8 *)rec + offset;
        fn = *(void **)(tbl + 0xc);
        arg1 = self;
        sub_803AD80(addr, arg1, fn);
    }
}
asm(".align 2, 0");
