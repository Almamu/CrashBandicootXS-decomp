#include "core.h"

/* GitHub issue #9/#10: 0x0800AAEC, the input-action-check function the
 * 42-slot `gStaticData_0816BF20` action-dispatch table's own entries
 * (`sub_8013994` etc.) call for their action codes `0xB`/`0x10` (see
 * docs/rom_map.md). Iterates the `gUnknown_0300130C` object list (the
 * same count-prefixed `{count, unused_4, items}` layout already
 * established in `src/system/game_loop24.c`'s `sub_8010804`), testing
 * each entry's own `+0x18`-table `+0x48` trampoline via `sub_803AD7C`
 * (matched elsewhere) and, on a hit (state `3`), calling `sub_800CD00`
 * (this function's own companion, see `actor_part109.c`) with that
 * entry and `x`.
 *
 * Before the loop, gates the whole call on a `sub_8026628` proximity/
 * position probe against the player (`gUnknown_03001308`): builds an
 * integer `{x, y}` position from `self`'s own Q8 position plus the
 * target action `x`'s own `self+0x20`-table[`x`] record's `+6` (s16)
 * vertical offset (added in Q8 space before truncating, matching the
 * ROM's exact rounding), passes `self+0x28` bit 4 (the same mirror-
 * flag bit `actor_part16.c`/`actor_part17.c` read) as a `1`/`2`
 * selector, the record's own `+9` byte as a third scalar, and a
 * pointer to `self`'s original (untruncated) Q8 `y` for the callee to
 * restore/report through.
 *
 * **Build toggle**: default builds (`NON_MATCHING=0`) use the `#else`
 * branch's NAKED transcription of the ROM's own confirmed-correct
 * instructions, byte-exact (verified against `asm/code_3_2_16.s`,
 * which this object replaces - that fragment is now retired). The
 * `#if NON_MATCHING` branch is a real C reconstruction that gets every
 * instruction byte-exact except the `gUnknown_0300130C` list-walk's
 * loop-condition-check/loop-entry pair (5 instructions: which of
 * r0/r1 holds `&gUnknown_0300130C` versus the freshly dereferenced
 * list pointer/`->count`). The ROM re-loads `&gUnknown_0300130C` from
 * the literal pool fresh every iteration (conservative reload across
 * the `sub_803AD7C`/`sub_800CD00` calls, which alias-escape the
 * global) and happens to land it in r0, with the loop body's own
 * first instruction (`ldr r0, [r0]`) turning that same register from
 * "address" into "value" in place; no C-level phrasing tried (a plain
 * `for`, the `sub_8010804`-style cached-`&var`-inside-an-`if`-guard
 * idiom, an explicit `goto`-based loop with both the address and the
 * dereferenced value pinned to fixed registers) reproduced that exact
 * per-iteration re-materialize-into-r0-then-alias-in-place shape - the
 * closest attempts either matched register roles but dropped the
 * per-iteration reload (gcc correctly recognizes the address itself
 * is loop-invariant and hoists it, unlike the ROM's own compiler
 * here), or matched the reload but swapped which of r0/r1 won.
 * Every other block - the `self+0x20`/`self+0x2d`-shaped table
 * index (needing the same "materialize the `+4` in two separate
 * instructions" opaque-asm anchor `sub_8007B00`/`sub_800D040` already
 * needed for their own record-pointer builds), the `self+0x28` bit 4
 * mirror-flag default+override idiom, the Q8-to-int conversion's
 * exact load/store interleaving, and the `sub_8026628`/`sub_803AD7C`/
 * `sub_800CD00` call marshalling and `(u8)` result-truncation checks -
 * matches byte-for-byte. */

struct actor_list {
    s32 count;
    s32 unused_4;
    void **items;
};

extern struct actor_list *gUnknown_0300130C;
extern void *gUnknown_03001308;
extern s32 sub_803AD7C(void *addr, void *fn);
extern s32 sub_800CD00(void *entry, s32 x);
extern s32 sub_8026628(void *player, s32 arg1, void *posInt, s32 arg3, void *outY);

#if NON_MATCHING
u8 sub_800AAEC(void *selfArg, s32 x)
{
    u8 *self = selfArg;
    void **tablePtr = *(void ***)(self + 0x20);
    s32 off = x * 28;
    u8 *rec = (u8 *)(*tablePtr) + off;
    register u8 *rec4 asm("r4");
    register u32 recByte9 asm("r3");
    volatile struct {
        s32 x;
        s32 y;
        s32 origY;
    } probe;
    register s32 flagArg asm("r2");
    s32 testVal;
    s32 i;

    asm volatile("add %1, %1, #4\n\tadd %0, %1, #0" : "=r"(rec4), "+r"(rec));
    recByte9 = rec4[5];

    {
        s32 selfX = *(s32 *)self;
        register s32 selfY asm("r2") = *(s32 *)(self + 4);
        probe.x = selfX;
        probe.y = selfY;
    }
    probe.origY = *(volatile s32 *)(self + 4);

    testVal = (s32)(self[0x28] << 27);
    flagArg = 1;
    if (testVal < 0) {
        flagArg = 2;
    }

    {
        s32 offYShifted = *(s16 *)(rec4 + 2) << 8;
        s32 ySum = offYShifted + probe.y;

        probe.x = probe.x >> 8;
        probe.y = ySum >> 8;
    }

    if ((u8)sub_8026628(gUnknown_03001308, flagArg, (s32 *)&probe.x, recByte9, (s32 *)&probe.origY) != 0) {
        return 0;
    }

    for (i = 0; i < gUnknown_0300130C->count; i++) {
        u8 *entry = gUnknown_0300130C->items[i];
        u8 *tbl = *(u8 **)(entry + 0x18) + 0x48;
        s16 offset = *(s16 *)tbl;
        void *addr = entry + offset;
        void *fn = *(void **)(tbl + 4);

        if (sub_803AD7C(addr, fn) == 3) {
            if ((u8)sub_800CD00(entry, x) == 1) {
                return 0;
            }
        }
    }
    return 1;
}
#else
NAKED u8 sub_800AAEC(void *selfArg, s32 x)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "sub sp, #0x10\n\t"
        "add r6, r1, #0\n\t"
        "ldr r1, [r0, #0x20]\n\t"
        "lsl r2, r6, #3\n\t"
        "sub r2, r2, r6\n\t"
        "lsl r2, r2, #2\n\t"
        "ldr r1, [r1]\n\t"
        "add r1, r1, r2\n\t"
        "add r1, #4\n\t"
        "add r4, r1, #0\n\t"
        "ldrb r3, [r4, #5]\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r2, [r0, #4]\n\t"
        "str r1, [sp, #4]\n\t"
        "str r2, [sp, #8]\n\t"
        "ldr r1, [r0, #4]\n\t"
        "str r1, [sp, #0xc]\n\t"
        "add r0, #0x28\n\t"
        "ldrb r0, [r0]\n\t"
        "lsl r0, r0, #0x1b\n\t"
        "mov r2, #1\n\t"
        "cmp r0, #0\n\t"
        "bge 1f\n\t"
        "mov r2, #2\n\t"
    "1:\n\t"
        "mov r0, #2\n\t"
        "ldrsh r1, [r4, r0]\n\t"
        "lsl r1, r1, #8\n\t"
        "ldr r0, [sp, #8]\n\t"
        "add r1, r1, r0\n\t"
        "ldr r0, [sp, #4]\n\t"
        "asr r0, r0, #8\n\t"
        "str r0, [sp, #4]\n\t"
        "asr r1, r1, #8\n\t"
        "str r1, [sp, #8]\n\t"
        "ldr r0, 3f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, sp, #0xc\n\t"
        "str r1, [sp]\n\t"
        "add r1, r2, #0\n\t"
        "add r2, sp, #4\n\t"
        "bl sub_8026628\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq 4f\n\t"
    "2:\n\t"
        "mov r0, #0\n\t"
        "b 8f\n\t"
        ".align 2, 0\n"
    "3: .4byte gUnknown_03001308\n"
    "4:\n\t"
        "mov r5, #0\n\t"
        "b 7f\n\t"
    "5:\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0, #8]\n\t"
        "lsl r0, r5, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r4, [r0]\n\t"
        "ldr r1, [r4, #0x18]\n\t"
        "add r1, #0x48\n\t"
        "mov r2, #0\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r1, #4]\n\t"
        "bl sub_803AD7C\n\t"
        "cmp r0, #3\n\t"
        "bne 6f\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r6, #0\n\t"
        "bl sub_800CD00\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "cmp r0, #1\n\t"
        "beq 2b\n\t"
    "6:\n\t"
        "add r5, #1\n\t"
    "7:\n\t"
        "ldr r0, 9f\n\t"
        "ldr r1, [r0]\n\t"
        "ldr r1, [r1]\n\t"
        "cmp r5, r1\n\t"
        "blt 5b\n\t"
        "mov r0, #1\n\t"
    "8:\n\t"
        "add sp, #0x10\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    "9: .4byte gUnknown_0300130C\n"
    );
}
#endif /* NON_MATCHING */
