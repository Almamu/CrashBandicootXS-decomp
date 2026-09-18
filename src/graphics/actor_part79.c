#include "core.h"

/* Part of GitHub issue #16's remainder (0x08011BD4-0x08012D24) - the
 * "child object" family docs/rom_map.md's "Undifferentiated core"
 * investigation and docs/matching/issue-16-actor-11b0c.md both already
 * identified but left raw pending a dedicated pass (`self+0xc`/`+0x10`
 * hold pointers to further sub-records, distinct from `struct actor`).
 * `self+0xc` is a per-category table of `{s16 offset; void *fn}` pairs
 * (at least `+0x20`/`+0x24` and `+0x50`/`+0x54` entries known so far)
 * fed through the `sub_803AD80`/`sub_803AD84` trampolines together
 * with `self+offset` and `self+0x10` (a "part" sub-object) - the same
 * convention already named in actor_part18.c's doc comments, which
 * this file's functions are siblings of (not ROM-adjacent to them,
 * hence a separate file per the one-file-per-contiguous-region rule).
 * The `+0x27`-`+0x32` bytes are the same shared state/flag/table-index
 * trio pair actor_part18.c documents; none of the three objects' full
 * shapes are pinned down yet, so every access here stays a raw offset
 * rather than a guessed struct, same as that file. */

struct AudioContext;
struct tile_asset_cache;

extern void PlaySfx(struct AudioContext *arg0, s32 sfxId, s32 arg2);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern s32 sub_803AD84(void *arg0, void *arg1, void *arg2, void *arg3);
extern void sub_8012AF4(void *self);
extern void sub_8023234(void *arg0);
extern void sub_8006D08(struct tile_asset_cache *self, s32 slot, s32 recordId);
extern void sub_80087C0(void *part);
extern void sub_80087B4(void *part);
extern void sub_800872C(void *part, u8 val);
extern void sub_80019A8(struct AudioContext *self, u32 id);
extern void sub_8015780(void *self, s32 a, s32 b, s32 c, s32 d);
extern u8 sub_8000760(void *dummy);

extern struct AudioContext *gUnknown_030012BC;
extern void *gUnknown_030012C0;
extern struct tile_asset_cache *gUnknown_030012B8;
extern void *gUnknown_030012D8;
extern void *gUnknown_03001304;

/* Plays a sound, fires the `+0x50`/`+0x54` trampoline pair with `arg1`
 * as its "part" argument, then the `+0x20`/`+0x24` pair with id `0x1d`,
 * resets both halves of the state/flag/table-index trio (`+0x31`/
 * `+0x2f`/`+0x27` and `+0x32`/`+0x30`/`+0x28`) via a single walked
 * pointer, runs `sub_8012AF4`, clears/sets a few more `part` bytes
 * (`+0x100`/`+0x102`/`+0x103`/`+0x104`, and clears bits `0x20`/`0x10`
 * of `part+0xc`), then calls `sub_8023234` and looks up a byte from the
 * per-tag 28-byte-record table (`part+0x20 -> *ptr + tag*0x1C`, the
 * same dereference chain docs/rom_map.md's "eight more core reads"
 * documented from three other call sites) to feed `sub_8006D08`. */
void sub_8012160(void *selfArg, void *arg1)
{
    u8 *self = selfArg;

    {
        register void *a0 asm("r0") = gUnknown_030012BC;
        register s32 a2 asm("r2") = 0x100;
        register s32 a1 asm("r1") = 0x1b;
        PlaySfx(a0, a1, a2);
    }

    {
        u8 *off = *(u8 **)(self + 0xc) + 0x50;
        sub_803AD84(self + *(s16 *)off, *(void **)(self + 0x10), arg1, *(void **)(off + 4));
    }
    {
        u8 *mgr = *(u8 **)(self + 0xc);
        sub_803AD80(self + *(s16 *)(mgr + 0x20), (void *)0x1d, *(void **)(mgr + 0x24));
    }

    {
        register s32 zero asm("r4") = 0;
        register s32 one asm("r5");

        {
            register u8 *w asm("r0") = self + 0x31;
            *w = zero; w -= 2;
            one = 1;
            *w = one;  w -= 8;
            *w = zero; w += 0xb;
            *w = zero; w -= 2;
            *w = one;  w -= 8;
            *w = zero;
        }

        sub_8012AF4(self);

        (*(u8 **)(self + 0x10))[0x100] = zero;
        (*(u8 **)(self + 0x10))[0x102] = zero;
        (*(u8 **)(self + 0x10))[0x103] = zero;

        {
            register u8 *p asm("r1") = *(u8 **)(self + 0x10);
            register s32 mask asm("r0") = 0x7f;
            mask &= p[0xc];
            p[0xc] = mask;
        }
        {
            register u8 *p asm("r1") = *(u8 **)(self + 0x10);
            register s32 mask asm("r0") = 0x41;
            mask = -mask;
            mask &= p[0xc];
            p[0xc] = mask;
        }

        {
            /* Plain `addr[0x104] = one;` lets the compiler constant-fold
             * the 0x104 offset into whichever scratch register it
             * likes (r2), always the opposite of the ROM's r1 - an
             * inline-asm anchor (matching_decomp_register_pinning) is
             * the only way found to pin the folded constant's own
             * register. */
            register u8 *addr asm("r0") = *(u8 **)(self + 0x10);
            register s32 v asm("r1");
            asm volatile("mov %0, #0x82\n\tlsl %0, %0, #1" : "=r"(v));
            addr += v;
            *addr = one;
        }
    }

    sub_8023234(gUnknown_030012C0);

    {
        register struct tile_asset_cache *cache asm("r0") = gUnknown_030012B8;
        register u8 *p asm("r3") = *(u8 **)(self + 0x10);
        register u32 nibble asm("r1") = (u32)(p[0x29] << 28) >> 28;
        register u8 **xptr asm("r2") = *(u8 ***)(p + 0x20);

        p = p + 0x2d;
        {
            register u8 *base asm("r4") = *xptr;
            register u8 tag asm("r5") = *p;
            register s32 record asm("r2") = tag * 0x1c;
            record += (s32)base;
            sub_8006D08(cache, nibble, ((u8 *)record)[0x14]);
        }
    }
}

/* If the player (`gUnknown_030012D8`)'s `+0x100` "active" flag is set:
 * on type `0x12` (only if `+0x60` is nonzero) or type `0xd`/`0x18`,
 * re-tags the player as `0x25` (type `0x12`) or `0x26` (the other two,
 * re-reading the global fresh first) and fires the standard
 * `sub_80087C0`/`sub_80087B4`/`sub_800872C(..., 0)` teardown trio.
 * Otherwise (flag clear), on player type `0x25`/`0x26`, plays a sound
 * and resets the state/flag/table-index trio via `sub_8015780`. */
void sub_8012238(void *selfArg)
{
    u8 *self = selfArg;
    u8 *player = gUnknown_030012D8;
    register s32 flag asm("r5") = player[0x100];

    if (flag == 0)
        goto flag_zero;

    {
        register u8 *typeAddr asm("r3") = player + 0x2d;
        s32 type = *typeAddr;
        register s32 type2 asm("r2") = type;

        if (type == 0x12)
            goto case_12;
        if (type > 0x12)
            goto check_18;
        if (type == 0xd)
            goto case_set_26;
        goto end;
    check_18:
        if (type2 == 0x18)
            goto case_set_26;
        goto end;

    case_12:
        if (*(s32 *)(player + 0x60) == 0)
            goto end;
        *typeAddr = 0x25;
        goto common;
    }

case_set_26: {
    player = gUnknown_030012D8;
    {
        register s32 v asm("r0") = 0x26;
        player[0x2d] = v;
    }
}

common:
    sub_80087C0(player);
    sub_80087B4(player);
    sub_800872C(player, 0);
    goto end;

flag_zero:
    {
        s32 type2 = (player + 0x2d)[0];
        if (type2 == 0x25)
            goto do_call;
        if (type2 != 0x26)
            goto end;
    do_call:
        sub_80019A8(gUnknown_030012BC, 0x36);
        sub_8015780(self, 0, 0x12, 0, flag);
    }
end:
    return;
}

/* Reads D-pad input (unused directly, but forces the same call/reload
 * shape as the ROM) and dispatches on `self+8`'s type via a 39-entry
 * jump table (values 0-0x26; anything above returns 0 directly). 16 of
 * the 39 values (0, 3-5, 7, 9, 11, 13-15, 20, 26, 32-33, 37-38) run a
 * shared body: clear `part+0x28` bit `0x20`, then on bit `0x10` set,
 * either re-clear it (D-pad remap `4`/`6`/`8`) or set it while also
 * setting bit `0x10` back (D-pad remap `3`/`5`/`7`), in both cases also
 * setting the state/flag pair `self+0x2f`=1/`self+0x29`=0 and
 * returning 1; every other value/path returns 0. */
s32 sub_80122CC(void *selfArg)
{
    u8 *self = selfArg;
    register s32 dpad asm("r3") = sub_8000760(gUnknown_03001304);
    register s32 result asm("r2") = 0;
    s32 type = *(s32 *)(self + 8);

    if ((u32)type > 0x26)
        goto end;

    switch (type) {
    case 0: case 3: case 4: case 5: case 7: case 9: case 11: case 13:
    case 14: case 15: case 20: case 26: case 32: case 33: case 37: case 38:
        goto do_it;
    default:
        goto end;
    }

do_it:
    {
        register u8 *p asm("r1") = *(u8 **)(self + 0x10) + 0x28;
        register s32 mask asm("r0") = -0x21;
        register s32 val asm("r5") = *p;
        mask &= val;
        *p = mask;
    }

    if ((*(u8 **)(self + 0x10))[0x28] << 27 >= 0)
        goto check_2nd;
    if (dpad == 4 || dpad == 6 || dpad == 8)
        goto branch1;
    goto check_2nd;

branch1:
    {
        register u8 *p asm("r1") = *(u8 **)(self + 0x10);
        register s32 zero asm("r2") = 0;
        p += 0x28;
        {
            register s32 mask asm("r0") = -0x11;
            mask &= *p;
            *p = mask;
        }
        {
            register u8 *addr1 asm("r1") = self + 0x2f;
            register s32 one asm("r0") = 1;
            *addr1 = one;
        }
        {
            register u8 *addr2 asm("r0") = self + 0x29;
            *addr2 = zero;
        }
    }
    goto ret1;

check_2nd:
    {
        register u8 *part asm("r0") = *(u8 **)(self + 0x10);
        register u8 *addr asm("r1") = part + 0x28;
        if (*addr << 27 < 0)
            goto end;
        if (dpad == 3 || dpad == 5 || dpad == 7)
            goto branch2;
        goto end;

    branch2:
        {
            register s32 one asm("r3") = 1;
            register u8 *p asm("r2") = part + 0x28;
            register s32 mask asm("r0") = -0x11;
            register s32 val asm("r5") = *p;
            mask &= val;
            mask |= 0x10;
            *p = mask;
            {
                register u8 *addr asm("r0") = self + 0x2f;
                register s32 zero asm("r1") = 0;
                *addr = one;
                addr -= 6;
                *addr = zero;
            }
        }
        goto ret1;
    }

ret1:
    result = 1;
end:
    return result;
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");
