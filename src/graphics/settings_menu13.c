#include "core.h"
#include "actor.h"

/* GitHub issue #8's last two functions - docs/rom_map.md's "`sub_80063D8`
 * builds a two-string dialog/message box" section already traced both to
 * high confidence. This file holds `sub_80063D8` itself (matched
 * byte-exact); its caller `sub_80062A8` (the higher-level constructor -
 * resets palette color 0 and DISPCNT, re-initializes the popup-text
 * system's `gUnknown_030012DC`/`030012E0` icon managers, calls this
 * function then `sub_8006518` to run the dialog's lifecycle) is parked
 * as `NON_MATCHING` in `src/graphics/settings_menu14.c` - see that
 * file's header comment. */

/* Same small per-widget object `src/graphics/oam_count.c`/
 * `src/graphics/settings_menu10.c` already name `struct
 * sub_8006700_actor` (redeclared locally per this project's minimal-
 * local-type convention) - allocated here via `sub_8026EDC(0x2c)`,
 * exactly the struct's own size, and returned by `sub_80063D8` to feed
 * straight into `sub_8006518`'s (the fade/confirm driver) and
 * `sub_8006770`'s (the on-hit teardown/sound helper) existing
 * signatures. */
struct sub_8006700_actor {
    u8 unused_00[0x10];
    s32 field_10;
    void *field_14;
    struct settings_icon_actor *field_18;
    u32 field_1c;
    u32 field_20;
    u8 field_24;
    u8 unused_25[3];
    u16 field_28;
};

/* Same `struct settings_icon_actor` shape `src/graphics/settings_menu6.c`
 * already documents (a `struct actor`-derived on-screen icon, allocated
 * the same way via `sub_8008904(sub_8026EDC(0x40))`) - redeclared
 * locally per this project's convention. */
struct settings_icon_actor {
    struct actor base;    /* 0x00-0x1b */
    u8 unused_1c[0x20 - 0x1c];
    void **field_20;        /* 0x20 - keyframe-table pointer */
    u8 unused_24[0x29 - 0x24];
    u8 field_29;               /* 0x29 - low nibble set from sub_800815C's result */
    u8 unused_2a[0x2d - 0x2a];
    u8 frameIndex;                /* 0x2d - current keyframe index */
};

extern void *sub_8026EDC(s32 size);
extern struct actor *sub_8008904(struct actor *part);
extern void sub_80087C0(struct actor *part);
extern void sub_80087B4(struct actor *part);
extern void sub_800872C(struct actor *part, u8 val);
extern s32 sub_800815C(struct actor *part);
extern void *sub_801E644(void *buf, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void LoadGraphicsPackage(void *buf, void *asset);
extern s32 sub_801E640(void *buf);
extern void sub_8001B54(void *self, s32 id);

extern void ***gUnknown_030012D0;
extern u8 gStaticData_0816C484[];
extern void *gUnknown_030012BC;

/* Builds the actual two-string dialog/message box object: a small
 * `struct sub_8006700_actor` (`self`, allocated by the caller) plus one
 * `struct settings_icon_actor`-shaped background icon it owns via
 * `field_18`. Sets `field_20`/`field_24` (the `sub_8006714`-shape
 * BLDCNT+BLDALPHA/BLDY blend-register pair, forced to a fixed "fully
 * blended" value here rather than read from a caller-supplied source)
 * and `field_28` (a fixed priority/flags pair), stashes the two label
 * pointers at `field_10`/`field_14`, loads `gStaticData_0816C484`'s
 * background package, and builds the background icon the same way
 * `settings_menu6.c`'s icon-constructor family does (allocate via
 * `sub_8008904(sub_8026EDC(0x40))`, point `field_20` at the shared
 * `gUnknown_030012D0` header table at a new `0xe4<<1` offset - see
 * docs/rom_map.md's "five confirmed header-relative offsets" note,
 * frame index from `type`, the standard `sub_80087C0`/`sub_80087B4`/
 * `sub_800872C` OAM trio, positioned at a fixed (0xf0<<7, 0xa0<<7)
 * point, `field_29`'s low nibble from `sub_800815C`). Finally sets
 * `REG_BG0CNT` from `sub_801E640(self)`, clears `REG_BG0HOFS`/
 * `REG_BG0VOFS` (one 32-bit write), and restores the last-played song
 * via `sub_8001B54(gUnknown_030012BC, 0xf)`.
 *
 * Matched byte-exact, but only after heavy register pinning (mirroring
 * `oam_count.c`'s `SUB_8006600_*` macros and `settings_menu6.c`'s
 * `UPDATE_ICON_FRAME_NIBBLE`) - this function's 4-argument, many-hi-reg
 * calling convention (`r8`/`r9`/`sl` all live across calls) and its
 * several byte-level bitfield read-modify-write sequences hit the same
 * "last mile" gcc-2.9 register-allocation nondeterminism this ROM
 * region documents at length. Three concrete gotchas worth recording
 * for the next function like this one:
 * - A plain `*ptr = v; ptr += 4;` pair (the `REG_BLDCNT`->`REG_BLDY`
 *   address step) gets fused by this compiler into a single
 *   post-increment `stmia rN!, {r0}` - valid but different bytes from
 *   the ROM's separate `str`/`adds`. Forcing the increment through an
 *   `asm volatile("add %0, %0, #4" : "+r"(ptr))` blocks the fusion.
 * - Reassigning an already-computed address-typed register variable to
 *   a nearby offset (`addr = self + 0x29` right after `addr` held
 *   `self + 0x28`) lets gcc reuse the old value with `+1` instead of
 *   recomputing fresh from `self` - the ROM always recomputes fresh
 *   here. A second, differently-named pointer variable (a fresh
 *   register) avoids the reuse.
 * - A plain `register T v asm("rN") = expr;` initializer is only a
 *   hint - gcc still felt free to materialize `expr` into a different
 *   register than `rN` for a handful of these single-use copies
 *   (`field_10`/`field_14`/`frameIndex`). Routing the copy through an
 *   `asm volatile("" : "=r"(v) : "0"(expr))` (an explicit "same
 *   register in and out" constraint) forces the actual `mov` into the
 *   requested register, matching the ROM's own reuse of whichever
 *   register happened to be free at that point in its own allocation
 *   (typically `r3`, left over from an unrelated adjacent OR-chain). */
struct sub_8006700_actor *sub_80063D8(struct sub_8006700_actor *selfArg, s32 label1Arg, s32 label2Arg, s32 typeArg)
{
    register struct sub_8006700_actor *self asm("r5") = selfArg;
    register s32 label1 asm("r8") = label1Arg;
    register s32 label2 asm("r9") = label2Arg;
    register s32 type asm("sl") = typeArg;
    register s32 one asm("r3");
    register s32 sixteen asm("r4");
    struct settings_icon_actor *icon;

    sub_801E644(self, 0, 0x1f, 0, 3);

    self->field_20 = 0;
    {
        register u8 *addr asm("r2");
        register s32 v asm("r0");
        register s32 d asm("r1");

        addr = (u8 *)self + 0x20;
        v = 0xc0;
        d = *addr;
        v |= d;
        d = 0x20;
        v |= d;
        one = 1;
        v |= one;
        d = 2;
        v |= d;
        d = 4;
        v |= d;
        d = 8;
        v |= d;
        sixteen = 0x10;
        v |= sixteen;
        *addr = v;

        addr += 4;
        v = 0x20;
        v = -v;
        d = *addr;
        v &= d;
        v |= sixteen;
        *addr = v;

        {
            register vu32 *bldp asm("r1");

            bldp = (vu32 *)REG_ADDR_BLDCNT;
            v = self->field_20;
            *bldp = v;
            asm volatile("add %0, %0, #4" : "+r"(bldp));
            addr = (u8 *)(u32)*addr;
            v = (u32)(((u32)addr) << 27) >> 27;
            *(vu16 *)bldp = v;
        }
    }

    self->field_28 = 0;
    {
        register u8 *addr asm("r2");
        register s32 v asm("r0");
        register s32 d asm("r1");

        addr = (u8 *)self + 0x28;
        v = 0x40;
        d = *addr;
        v |= d;
        d = 8;
        d = -d;
        v &= d;
        *addr = v;

        {
            register u8 *addr2 asm("r0");

            addr2 = (u8 *)self + 0x29;
            d = *addr2;
            one |= d;
            one |= sixteen;
            *addr2 = one;
        }
    }

    {
        register s32 t1 asm("r3");
        register void *t2 asm("r0");

        asm volatile("" : "=r"(t1) : "0"(label1));
        self->field_10 = t1;
        asm volatile("" : "=r"(t2) : "0"((void *)label2));
        self->field_14 = t2;
    }

    LoadGraphicsPackage(self, gStaticData_0816C484);
    self->field_1c = 0;

    icon = (struct settings_icon_actor *)sub_8008904((struct actor *)sub_8026EDC(0x40));
    self->field_18 = icon;
    icon->field_20 = (void **)((u8 *)(**gUnknown_030012D0) + (0xe4 << 1));
    {
        register u8 *addr asm("r0");
        register u8 t3 asm("r3");

        addr = (u8 *)icon + 0x2d;
        asm volatile("" : "=r"(t3) : "0"((u8)type));
        *addr = t3;
    }
    sub_80087C0(&icon->base);
    sub_80087B4(&icon->base);
    sub_800872C(&icon->base, 0);

    {
        register struct actor *iconAddr asm("r0");
        register s32 v asm("r1");

        iconAddr = &self->field_18->base;
        v = 0xf0 << 7;
        iconAddr->x = v;
        v = 0xa0 << 7;
        iconAddr->y = v;

        {
            register s32 ret asm("r0") = sub_800815C(iconAddr);
            register u8 *addr asm("r2") = &self->field_18->field_29;
            register s32 mask asm("r1");
            register u8 byte asm("r3");

            mask = 0xf;
            ret &= mask;
            asm volatile("mov %0, #0x10\n\tneg %0, %0" : "=r"(mask));
            byte = *addr;
            mask &= byte;
            mask |= ret;
            *addr = mask;
        }
    }

    REG_BG0CNT = sub_801E640(self);
    *(vu32 *)REG_ADDR_BG0HOFS = 0;
    sub_8001B54(gUnknown_030012BC, 0xf);

    return self;
}
