#include "core.h"
#include "actor.h"

/* GitHub issue #9/#10 (0x0800B8DC-0x0800D040 cluster, see
 * docs/matching/issue-9-10-0x0800b8dc-graphics.md): the small gap the
 * three parallel closing sessions all missed - `asm/code_3_2_17_ca04.s`
 * (ROM 0x0800CA04-0x0800CBD4, 464 bytes, 19 functions/stubs), sitting
 * directly between two already-matched neighbors from the same
 * session: `sub_800C9C8` (`actor_part116.c`) just before it, and
 * `sub_800CBD4` (`actor_part117.c`) - which calls this file's own
 * `nullsub_14` - immediately after. Despite the address range's small
 * size the whole file turned out to be nothing but tiny single-purpose
 * accessors on this cluster's already-well-characterized `self`/`owner`
 * object shape (field table in the Phase 1 section of the doc above),
 * plus two slightly larger helpers (`sub_800CA08`'s camera-distance/
 * volume calculator, `sub_800CACC`'s conditional `sub_803AD88`
 * trigger) and one instance of the "flag active + bitmap-set" idiom
 * (`sub_800CB64`) already matched as real C once before, in
 * `actor_part27c.c`'s `sub_8018884`.
 *
 * All 19 matched as **real C**, no NAKED fallback needed anywhere in
 * this file - smaller and more resistant-shape-free than most of this
 * cluster's other files, despite several needing the project's usual
 * gcc 2.9 register-pinning toolbox (see individual comments below).
 *
 * New field offsets this file resolves/confirms on the shared
 * `self`/`owner` object shape (offsets not already in the Phase 1
 * doc's own field table):
 *
 * - `self+0x70`: the `owner` pointer itself - `sub_800CA04` is its
 *   setter (every other function in this cluster only ever *reads*
 *   `self+0x70`; this is the first confirmed writer).
 * - `self+0x4`: the "manager" pointer Phase 2's doc already
 *   identified (`sub_800B704`/`sub_800B838`'s own 8-byte-record
 *   array base) - `sub_800CA48` resets it to the fixed global
 *   `gStaticData_0816BB6C`.
 * - `self+0x84`: the per-instance mode-indexed pointer table Phase 2
 *   already identified (`sub_800C8CC`/`sub_800C6A8`'s own trigger
 *   table) - `sub_800CAC0` is its setter, `sub_800CA48` clears it.
 * - `self+0x88`: the floating-popup child pointer the Phase 1 doc's
 *   field table already names - `sub_800CA48` clears it (part of the
 *   same reset this function performs on `self+0x70`/`self+0x84`).
 * - `self+0x3c`/`0x40`/`0x44`: the sine-oscillator parameters
 *   `sub_800C8F8`/`sub_800C940`/`sub_800C97C` (`actor_part116.c`)
 *   already consume (`self->0x3c` divisor, `self->0x40` phase offset,
 *   `self->0x44` amplitude) - `sub_800CA94` is their setter.
 * - `self+0x48`/`0x4c`: the fields `sub_800BFA8`'s (`actor_part121.c`)
 *   own `sub_803AE4C` "close enough" gate reads - `sub_800CA9C` is
 *   their setter.
 * - `self+0x30`/`0x34`/`0x38`: the "blocking condition" pair plus
 *   "enabled" byte the Phase 1 doc's field table already names -
 *   `sub_800CAA4` is their setter.
 * - `self+0x20`/`0x24`/`0x28`/`0x2c`: the per-instance AABB trigger
 *   box `sub_800C5D4` (`actor_part116.c`) already builds from -
 *   `sub_800CAAC` is its full 4-corner setter, `sub_800CB58` a
 *   2-field (position-only) partial setter.
 * - `self+0x6c`: the "second, larger-range state/anim-id byte" the
 *   Phase 1 doc's field table already names - `sub_800CAC8` is its
 *   setter.
 * - `self+0x1c`: the Y-axis homing bound `sub_800C87C`/`sub_800C898`
 *   (`actor_part122.c`) already write - `sub_800CB60` is its setter,
 *   and `sub_800CACC` reads it (into a value it never uses - see that
 *   function's own comment).
 * - `self+0x18`: reused here as the struct-actor-shaped "table"
 *   pointer role (`sub_800CB20`/`sub_800CB40`, and `sub_800CB64`'s own
 *   `other` argument) - the same nominal offset the Phase 4 doc's
 *   `sub_800C87C` uses for a Y-axis homing bound instead, on what must
 *   be a differently-shaped object at that call site (this cluster's
 *   `self`/`owner`/`other` roles are not one single reconciled struct,
 *   per the Phase 1 doc's own explicit caution - not resolved further
 *   here).
 *
 * Confirmed byte-identical to `baserom.gba` at `0x0800CA04`-`0x0800CBD4`
 * (464 bytes, all 19 functions) via the isolated cpp/agbcc/as +
 * objcopy/cmp pipeline (the only per-function differences from a direct
 * ROM slice were `bl`/literal-pool relocation sites, in every case),
 * plus a full clean `rm -rf build && make NON_MATCHING=1 report` (no
 * warnings) and `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
 * crashbandicootxs.map && make compare` (`crashbandicootxs.gba: La
 * suma coincide`). This fully consumes `asm/code_3_2_17_ca04.s`,
 * retired from `ldscript.txt`. */

/* `self+0x70` ("owner") setter - the first confirmed writer of this
 * field anywhere in the cluster (every other function only reads it). */
void sub_800CA04(void *selfArg, void *owner)
{
    u8 *self = selfArg;
    *(void **)(self + 0x70) = owner;
}

extern struct actor *gUnknown_030012D8;

/* The exact "distance-scaled ambient sound volume" calculation
 * `sub_800B8DC` state 18 (`actor_part112.c`) already documents inline
 * - `max(|x - cameraX|, |y - cameraY|)` against `gUnknown_030012D8`
 * (the player/camera object), clamped to `[0x20, 0xa0]`, converted to
 * `0x100 - (clamped - 0x20) * 2`. Whether this is the literal function
 * that inline block was compiled from, or an independently-written
 * sibling with identical logic, isn't resolved here - either way it's
 * the same primitive.
 *
 * Needed [[matching_decomp_register_pinning]]: pinning `x`/`y` to
 * `r0`/`r1` and every intermediate to the ROM's own `r2`/`r3` choices
 * was enough to get gcc 2.9 to emit the ROM's own branch-free
 * sign-mask abs idiom (`mask = v >> 31; v = (v ^ mask) - mask;`)
 * without spilling to `r4` - unpinned, the compiler kept `x`'s
 * distance live across the `y` computation in a spilled `r4`, forcing
 * an unwanted `push {r4, lr}`/`pop {r4}` pair the ROM's own leaf
 * function (no `bl` calls at all) never has. The final `d < 0x20`
 * clamp also needed rephrasing as `d = (d >= 0x20) ? d : 0x20` (rather
 * than the more natural `if (d < 0x20) d = 0x20;`) to make gcc emit
 * the ROM's own `cmp r1, #0x20; bge` pair instead of canonicalizing
 * the negated branch condition into `cmp r1, #0x1f; bgt`. */
s32 sub_800CA08(s32 x, s32 y)
{
    register s32 dx asm("r0") = x;
    register s32 dy asm("r1") = y;
    register struct actor *obj asm("r3") = gUnknown_030012D8;
    register s32 mask asm("r2");
    register s32 d asm("r1");

    mask = obj->x >> 8;
    dx = dx - mask;
    mask = dx >> 31;
    dx = dx ^ mask;
    mask = dx - mask;

    dy = dy - (obj->y >> 8);
    {
        register s32 mask2 asm("r0");
        mask2 = dy >> 31;
        dy = dy ^ mask2;
        dy = dy - mask2;
    }

    d = (dy >= mask) ? dy : mask;
    d = (d >= 0x20) ? d : 0x20;
    if (d > 0xa0)
        d = 0xa0;
    return 0x100 - (d - 0x20) * 2;
}

extern u8 gStaticData_0816BB6C[];

/* Resets `self+0x70` ("owner"), `self+0x84` (the per-instance
 * mode-indexed pointer table) and `self+0x88` (the floating-popup
 * child pointer) to null, and re-points `self+4` (the "manager"
 * pointer, per the Phase 2 doc) at the fixed `gStaticData_0816BB6C`
 * table - an initializer/reset for this object's own extension
 * fields, called by `sub_800CA74` below as part of its own
 * construction sequence. */
void sub_800CA48(void *selfArg)
{
    u8 *self = selfArg;
    *(void **)(self + 0x70) = NULL;
    *(void **)(self + 0x84) = NULL;
    *(void **)(self + 4) = gStaticData_0816BB6C;
    *(void **)(self + 0x88) = NULL;
}

extern u8 gStaticData_087E3EE4[];
extern void sub_800B8A8(void *self, s32 flags);
extern void sub_800B8C8(void *self);

/* Sets `self+0xc`'s table pointer to `gStaticData_087E3EE4` - the
 * same 93-vtable-family record `sub_800B8DC`/`sub_800BD48`
 * (`actor_part112.c`) themselves live in, per the Phase 1 doc's own
 * "Bounds and vtable status" section - then tail-calls `sub_800B8A8`.
 * Same "dead store, immediately overwritten by the callee" shape
 * already flagged as a likely oddity for this exact function in the
 * Phase 1 doc's own state-9 note: `sub_800B8A8` (`actor_part17.c`)
 * unconditionally resets `self+0xc` right back to
 * `gStaticData_087E3E7C` on every call, so this function's own store
 * never survives past the call - the same harmless double-set pattern
 * already established for `sub_8018858`/`sub_8017A78`/`sub_8017FD4`/
 * `sub_800CCCC`. */
void sub_800CA60(void *selfArg, s32 flags)
{
    u8 *self = selfArg;
    *(void **)(self + 0xc) = gStaticData_087E3EE4;
    sub_800B8A8(self, flags);
}

/* Resets via `sub_800B8C8`, re-points `self+0xc` at the same
 * `gStaticData_087E3EE4` table `sub_800CA60` above uses, then calls
 * `sub_800CA48` (clearing this object's own extension fields) and
 * returns `self` - the same "reset, re-point, hook, return self"
 * constructor shape already matched for `sub_801886C`/`sub_8018858`/
 * `sub_800CBD4`/`sub_800CCE0`, with `sub_800CA48` playing the
 * `nullsub_N`-hook role those other constructors give a no-op. */
void *sub_800CA74(void *selfArg)
{
    u8 *self = selfArg;
    sub_800B8C8(self);
    *(void **)(self + 0xc) = gStaticData_087E3EE4;
    sub_800CA48(self);
    return self;
}

/* `self+0x3c`/`0x40`/`0x44` setter - the sine-oscillator parameters
 * (divisor, phase offset, amplitude) `sub_800C8F8`/`sub_800C940`/
 * `sub_800C97C` (`actor_part116.c`) already consume. */
void sub_800CA94(void *selfArg, s32 a, s32 b, s32 c)
{
    u8 *self = selfArg;
    *(s32 *)(self + 0x3c) = a;
    *(s32 *)(self + 0x40) = b;
    *(s32 *)(self + 0x44) = c;
}
asm(".align 2, 0");

/* `self+0x48`/`0x4c` setter - the fields `sub_800BFA8`'s
 * (`actor_part121.c`) own `sub_803AE4C` "close enough" gate reads. */
void sub_800CA9C(void *selfArg, s32 a, s32 b)
{
    u8 *self = selfArg;
    *(s32 *)(self + 0x48) = a;
    *(s32 *)(self + 0x4c) = b;
}

/* `self+0x30`/`0x34`/`0x38` setter - the "blocking condition" pair
 * plus "enabled" byte the Phase 1 doc's field table already names. */
void sub_800CAA4(void *selfArg, s32 a, s32 b, s32 c)
{
    u8 *self = selfArg;
    *(s32 *)(self + 0x30) = a;
    *(s32 *)(self + 0x34) = b;
    *(s32 *)(self + 0x38) = c;
}

/* `self+0x20`/`0x24`/`0x28`/`0x2c` full 4-corner setter - the
 * per-instance AABB trigger box `sub_800C5D4` (`actor_part116.c`)
 * already builds from (`self`'s own position offset/size, distinct
 * from `owner`'s own smaller flags-byte field layout at the same
 * nominal offsets, per that function's own doc comment). The fourth
 * argument arrives on the stack (only 3 fit in `r1`-`r3`); needed the
 * trailing `[[matching_decomp_alignment_fix]]` idiom since its own
 * 18-byte body isn't 4-byte-aligned. */
void sub_800CAAC(void *selfArg, s32 a, s32 b, s32 c, s32 d)
{
    u8 *self = selfArg;
    *(s32 *)(self + 0x20) = a;
    *(s32 *)(self + 0x28) = c;
    *(s32 *)(self + 0x24) = b;
    *(s32 *)(self + 0x2c) = d;
}
asm(".align 2, 0");

/* `self+0x84` setter - the per-instance mode-indexed pointer table
 * `sub_800C8CC`/`sub_800C6A8` (`actor_part113.c`/`actor_part122.c`)
 * both trigger through. */
void sub_800CAC0(void *selfArg, s32 a)
{
    u8 *self = selfArg;
    *(s32 *)(self + 0x84) = a;
}
asm(".align 2, 0");

/* `self+0x6c` setter - the "second, larger-range state/anim-id byte"
 * the Phase 1 doc's field table already names. */
void sub_800CAC8(void *selfArg, s32 a)
{
    u8 *self = selfArg;
    *(s32 *)(self + 0x6c) = a;
}

extern u32 gUnknown_0300082C;
extern s32 sub_803AE4C(s32 a, s32 b);
extern void sub_803AD88(void *arg0, s32 arg1, s32 arg2, s32 arg3);

/* If `self`'s own X position (`self+0`, Q8.8) is within `[0xa1, 0x18f]`
 * tiles of `gUnknown_030012D8`'s (the player/camera object) own X
 * position, runs the same `sub_803AE4C(gUnknown_0300082C + a - b, a)`
 * "close enough" gate `sub_800BFA8` (`actor_part121.c`) already uses
 * (here against `self+0x20`/`self+0x24`, the AABB corner fields
 * `sub_800CAAC` above sets), and on a pass fires
 * `sub_803AD88((void*)0xffff, (u16)selfX, (u16)(self->4 >> 8), 0)` -
 * the same "directional-target table trigger" primitive
 * `sub_800B8DC` state 11 and `sub_800BD48` states 19-20
 * (`actor_part112.c`) already call directly. `self+0x1c` (the Y-axis
 * homing bound `sub_800CB60` below sets) is read here too but its
 * value is never used for anything - a genuine dead read the ROM's own
 * compiled output still performs (confirmed by the ROM's own `ldr r4,
 * [r4, #0x1c]` sitting right before the call with no further use of
 * `r4` after it).
 *
 * Needed [[matching_decomp_register_pinning]] in one spot: the dead
 * `self+0x1c` read had to be pinned to `r4` explicitly (the register
 * `self` itself was already using, and free again by this point) -
 * unpinned, gcc picked a spare `r3` for it instead, a harmless but
 * byte-different register choice from the ROM's own. */
void sub_800CACC(void *selfArg)
{
    u8 *self = selfArg;
    s32 selfX = *(s32 *)self >> 8;
    s32 cameraX = gUnknown_030012D8->x >> 8;

    if ((u32)(selfX - cameraX - 0xa1) <= 0xee) {
        s32 base = (s32)gUnknown_0300082C;
        s32 field20 = *(s32 *)(self + 0x20);
        s32 divCheck = sub_803AE4C(base + field20 - *(s32 *)(self + 0x24), field20);

        if (divCheck == 0) {
            void *arg0 = (void *)0xFFFF;
            u32 arg1 = ((u32)selfX << 16) >> 16;
            u32 arg2 = ((u32)*(s32 *)(self + 4) << 8) >> 16;
            register s32 dead asm("r4");

            dead = *(volatile s32 *)(self + 0x1c);
            (void)dead;
            sub_803AD88(arg0, arg1, arg2, 0);
        }
    }
}

extern u8 gStaticData_087E3BEC[];
extern void sub_8026ED0(void *self);

/* Sets `self+0x18`'s table pointer (the struct-actor-shaped "table"
 * field role, per this file's own banner comment) to
 * `gStaticData_087E3BEC` - the same table `graphics.c`'s own
 * constructors already use - then, only if bit 0 of `flags` is set,
 * fires `sub_8026ED0(self)`. */
void sub_800CB20(void *selfArg, s32 flags)
{
    u8 *self = selfArg;
    *(void **)(self + 0x18) = gStaticData_087E3BEC;
    if (flags & 1) {
        sub_8026ED0(self);
    }
}

extern struct actor *sub_800725C(struct actor *self);
extern u8 gStaticData_087E3F4C[];

/* Calls `sub_800725C(self)` (already matched, `graphics.c`) - its
 * return value discarded - then sets `self+0x18`'s table pointer to
 * `gStaticData_087E3F4C` and returns `self`. */
void *sub_800CB40(void *selfArg)
{
    u8 *self = selfArg;
    sub_800725C((struct actor *)self);
    *(void **)(self + 0x18) = gStaticData_087E3F4C;
    return self;
}

/* `self+0x20`/`0x24` partial (position-only) setter - the same AABB
 * trigger-box fields `sub_800CAAC` above sets all four corners of. */
void sub_800CB58(void *selfArg, s32 a, s32 b)
{
    u8 *self = selfArg;
    *(s32 *)(self + 0x20) = a;
    *(s32 *)(self + 0x24) = b;
}
asm(".align 2, 0");

/* `self+0x1c` setter - the Y-axis homing bound `sub_800C87C`/
 * `sub_800C898` (`actor_part122.c`) already write, and the field
 * `sub_800CACC` above reads (but never uses) via its own dead
 * `self+0x1c` load. */
void sub_800CB60(void *selfArg, s32 a)
{
    u8 *self = selfArg;
    *(s32 *)(self + 0x1c) = a;
}

extern void *gUnknown_030012B4;
extern void *sub_803AD7C(void *addr, void *fn);

/* `self` (the first argument) is never read - only `other` matters.
 * Reads `other+0x18`'s own struct-actor-shaped table pointer, fires a
 * `sub_803AD7C` hit-probe against its `+0x28`/`+0x2c` `{s16 offset,
 * void *fn}` pair (the exact same convention `src/system/game_loop8.c`'s
 * `sub_802400C` and `actor_part123.c`'s `sub_800CBF4` both already
 * read from their own `table+0x28`/`+0x2c`), and - only when that
 * probe reports *no* hit - runs the "flag active + bitmap-set" idiom
 * on `other` (`other+0xc` |= bit 0; unless `other+8`'s id sentinel-
 * checks as `0xffff`, also sets bit `other+8 & 0x1f` of word
 * `other+8 >> 5` in the `gUnknown_030012B4+0x108` bitmap) - the exact
 * idiom `actor_part27c.c`'s `sub_8018884` already matches as real C.
 *
 * Needed the same `[[matching_decomp_register_pinning]]` treatment
 * `sub_8018884` itself documents needing: `other` pinned to `r4`
 * (matching the ROM's own register choice, freed up again by the time
 * the bitmap-set idiom's own `0x108`-offset computation reuses it),
 * plus the same chain of `register ... asm("rN")` pins and the
 * `volatile` reload of `other+8` that function's own doc comment
 * already explains is needed to stop this compiler CSE-ing away the
 * ROM's own seemingly-redundant second `ldrh` and folding the
 * shift-setup pair into a single instruction. */
void sub_800CB64(void *selfArg, void *otherArg)
{
    register u8 *other asm("r4") = otherArg;
    u8 *table = *(u8 **)(other + 0x18);
    s16 offset = *(s16 *)(table + 0x28);
    void *addr = other + offset;
    void *fn = *(void **)(table + 0x2c);

    (void)selfArg;

    if ((u8)(s32)sub_803AD7C(addr, fn) == 0) {
        register s32 one asm("r0") = 1;
        register u8 flags asm("r1") = other[0xc];

        one |= flags;
        other[0xc] = one;

        {
            register s32 sentinel asm("r0") = 0xFFFF;
            register u16 val asm("r2") = *(u16 *)(other + 8);

            if (val != sentinel) {
                register u16 val2 asm("r3") = *(u16 volatile *)(other + 8);
                register u8 *base asm("r2") = gUnknown_030012B4;
                register s32 idx asm("r0");
                s32 idxOffset;
                s32 *bitmap;
                register s32 bit asm("r0");

                asm("add %0, %1, #0\n\tasr %0, %0, #5" : "=r" (idx) : "r" (val2));
                idxOffset = idx * 4;
                bitmap = (s32 *)(base + 0x108);
                bitmap = (s32 *)((u8 *)bitmap + idxOffset);
                bit = val2 - (idx << 5);
                *bitmap |= 1 << bit;
            }
        }
    }
}

/* Genuine empty stub (`bx lr`) - `sub_800CBD4`'s (`actor_part117.c`)
 * own tail-call hook, per that function's own doc comment. */
void nullsub_14(void *self)
{
}
asm(".align 2, 0");

extern u8 gStaticData_087E3FA4[];

/* Same "double-set" shape as `sub_800CA60` above: sets `self+0xc`'s
 * table pointer to `gStaticData_087E3FA4` - the same fixed anchor
 * table `sub_800CBD4` (`actor_part117.c`) itself re-points `self+0xc`
 * at - then tail-calls `sub_800B8A8`, which promptly resets `self+0xc`
 * right back to `gStaticData_087E3E7C` regardless (same harmless dead
 * store as `sub_800CA60`). */
void sub_800CBC0(void *selfArg, s32 flags)
{
    u8 *self = selfArg;
    *(void **)(self + 0xc) = gStaticData_087E3FA4;
    sub_800B8A8(self, flags);
}
