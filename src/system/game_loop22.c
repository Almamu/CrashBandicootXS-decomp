#include "core.h"

/* GitHub issue #13: 0x0800FC70-0x08010A0C - continues the physics/
 * collision subsystem `game_loop6.c`/`game_loop7.c` started (see
 * docs/matching/issue-12-physics-collision.md and
 * docs/matching/issue-13-graphics-fc70.md), still in the same
 * `asm/code_3_2_17_e560.s` region issue #12 left untouched past its
 * own scope. Recategorized `graphics` -> `game_loop` for the same
 * reason issue #12 recategorized the previous span: this whole
 * neighborhood (~0x0800D000-0x08010D54) is `docs/rom_map.md`'s
 * confirmed shared physics/collision subsystem, not per-entity
 * behavior. `sub_800FC70`/`sub_800FDC8` immediately before this
 * function are left untouched raw - see the write-up doc. */

extern void *gUnknown_030012D8;

/* Resets `self`'s collision-response bookkeeping: sets flags `+0xc`
 * bits 2/6, clears the low 7 bits of `+0x4d` (state byte) while also
 * clearing the global `gUnknown_030012D8+0x80` "hit" latch, then
 * zeroes the timer/list-link block `+0x44`-`+0x51`/`+0x58` and the two
 * neighbor-list pointers `+0x5c`/`+0x60`, and sets the `+0x54`
 * countdown to -1 (disabled). Matches the "get next"/"get prev" field
 * pair (`+0x5c`/`+0x60`) `sub_8010708`/`sub_801070C` in game_loop18.c
 * read/write. */
void sub_800FEB0(void *selfArg)
{
    register u8 *self asm("r2") = selfArg;
    u8 v = 4;
    register u8 *addr asm("r3");
    u8 zero;

    v |= self[0xc];
    v |= 0x40;
    self[0xc] = v;

    /* Inline-asm-anchored: the ROM computes the 0x7f/0x80 mask
     * immediate *before* the `ldrb` byte load in both of these
     * AND-and-store sequences (`movs r0,#mask; ldrb r4,[r3];
     * ands r0,r4; strb r0,[r3]`), with the loaded byte specifically
     * in r4 and the mask/result in r0 - every plain-C phrasing tried
     * (compound assignment either direction, a named "mask"/"loaded"
     * pair with and without register pins) instead had this compiler
     * either load the byte first or land the AND result in the wrong
     * register. Anchoring the exact instruction sequence here was
     * more reliable than continuing to chase the scheduler. */
    addr = self + 0x4d;
    {
        register u8 result asm("r0");
        asm volatile(
            "mov r0, #0x7f\n"
            "ldrb r4, [%1]\n"
            "and r0, r0, r4\n"
            : "=r"(result) : "l"(addr) : "r4"
        );
        zero = 0;
        *addr = result;
    }
    *((u8 *)gUnknown_030012D8 + 0x80) = zero;

    asm volatile(
        "mov r0, #0x80\n"
        "ldrb r4, [%0]\n"
        "and r0, r0, r4\n"
        "strb r0, [%0]\n"
        :: "l"(addr) : "r0", "r4", "memory"
    );

    *(u32 *)(self + 0x44) = zero;
    self[0x4c] = zero;
    *(u32 *)(self + 0x48) = zero;
    self[0x4f] = zero;
    self[0x50] = zero;
    self[0x51] = zero;
    self[0x58] = zero;
    *(s32 *)(self + 0x54) = -1;
    *(u32 *)(self + 0x5c) = zero;
    *(u32 *)(self + 0x60) = zero;
}
