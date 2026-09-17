#include "core.h"

/* GitHub issue #12: 0x0800D040-0x0800FC70, the physics/collision
 * subsystem (see game_loop6.c's header comment and
 * docs/matching/issue-12-physics-collision.md). `sub_0800D18C` and
 * `sub_800E08C` between game_loop6.c's `sub_800D040` and this file's
 * `sub_800E494` are left untouched raw - see
 * asm/code_3_2_17_d18c.s and the write-up doc. */

extern void *sub_801070C(void *obj);
extern void *sub_8010708(void *obj);

#if NON_MATCHING
/* Walks `obj`'s doubly-linked neighbor list both ways (`sub_801070C`
 * = next, `sub_8010708` = prev - the same "get next"-style pair
 * `docs/rom_map.md` ties to the physics/collision subsystem's
 * `sub_0800D18C`), clearing each visited neighbor's `+0x58` byte
 * whenever its own `+0x4d & 0x7f` state byte is 0.
 *
 * PARKED, NOT BYTE-MATCHING: every operation, operand and branch
 * matches the ROM one-for-one and gcc even finds the same "reuse the
 * AND result register as the literal 0 being stored" trick the ROM
 * uses (`strb r2,[r0]` off the just-computed `ands` result rather
 * than materializing a fresh `movs r2,#0`). The one remaining gap is
 * a single recurring instruction-order swap: the ROM materializes the
 * `0x7f` mask immediate *before* the `ldrb` byte load
 * (`movs r2,#0x7f; ldrb r0,[r0]; ands r2,r0`), while this compiler
 * always schedules the load first regardless of operand order in the
 * source (`a & b` vs `b & a` - both tried, same result). Parked rather
 * than keep chasing a scheduler-internal tie-break - see
 * docs/matching/issue-12-physics-collision.md. */
void sub_800E494(void *obj)
{
    void *cur;

    for (cur = sub_801070C(obj); cur != NULL; cur = sub_801070C(cur)) {
        if ((0x7f & *((u8 *)cur + 0x4d)) == 0) {
            *((u8 *)cur + 0x58) = 0;
        }
    }
    for (cur = sub_8010708(obj); cur != NULL; cur = sub_8010708(cur)) {
        if ((0x7f & *((u8 *)cur + 0x4d)) == 0) {
            *((u8 *)cur + 0x58) = 0;
        }
    }
}
#endif /* NON_MATCHING */
asm(".align 2, 0");

#if NON_MATCHING
/* Same bidirectional-neighbor walk as `sub_800E494` above, but instead
 * of clearing `+0x58` it sets it to 1 and, for the forward
 * (`sub_801070C`) direction only, also debits `ctx+4` and credits
 * `ctx+0xc` by `ctx+0xc`'s *original* value (`step`, cached once
 * before the loop - the ROM keeps it in `r5` throughout, since
 * `ctx+0xc` itself is mutated inside the loop and can't be re-read);
 * the reverse (`sub_8010708`) direction only credits `ctx+0xc`. Reads
 * as redistributing some accumulated "budget" field between a
 * just-touched neighbor and the rest of the chain.
 *
 * PARKED, NOT BYTE-MATCHING: same single resistant gap as
 * `sub_800E494` above (the `0x7f`-mask-before-`ldrb` instruction-order
 * swap, 4 occurrences here); everything else - including gcc
 * correctly hoisting the literal 1 into a register (`r7`/`r6` in the
 * ROM) kept live across each loop, reproduced here via the `one`
 * local - matches. See docs/matching/issue-12-physics-collision.md. */
void sub_800E4E4(void *obj, void *ctx)
{
    s32 step = *(s32 *)((u8 *)ctx + 0xc);
    void *cur;
    u8 one = 1;

    for (cur = sub_801070C(obj); cur != NULL; cur = sub_801070C(cur)) {
        if ((0x7f & *((u8 *)cur + 0x4d)) == 0) {
            *((u8 *)cur + 0x58) = one;
            *(s32 *)((u8 *)ctx + 4) -= step;
            *(s32 *)((u8 *)ctx + 0xc) += step;
        }
    }
    one = 1;
    for (cur = sub_8010708(obj); cur != NULL; cur = sub_8010708(cur)) {
        if ((0x7f & *((u8 *)cur + 0x4d)) == 0) {
            *((u8 *)cur + 0x58) = one;
            *(s32 *)((u8 *)ctx + 0xc) += step;
        }
    }
}
#endif /* NON_MATCHING */
asm(".align 2, 0");
