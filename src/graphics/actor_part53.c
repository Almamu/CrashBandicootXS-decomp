#include "core.h"

/* Same palette-cycle cluster as actor_part41.c/actor_part43.c - see
 * docs/matching/issue-50-actor-2a69c.md. */

extern u8 gUnknown_03001464;
extern s32 gUnknown_03001470;
extern s32 gUnknown_03001474;
extern s32 gUnknown_03001478;
extern u8 gStaticData_08175760[];
extern s32 QueueVramDmaTransfer(void *arg0, void *arg1, u16 arg2, u16 arg3);

/* Per-frame palette-cycle DMA: while `gUnknown_03001464` is set, DMAs one
 * `0x1c0`-byte palette-animation "frame" (`gStaticData_08175760 +
 * cursor*0x1c0`) to BG palette RAM. Every `0x24` calls (the
 * `gUnknown_03001478` counter), advances the cursor (`gUnknown_03001470`)
 * toward `gUnknown_03001474`: increments while still below the bound,
 * decrements once past it, and holds steady exactly at the bound -
 * `sub_802AB34`/`sub_802ABC8` flip which end is "the bound" to make this
 * ping-pong.
 *
 * Two gaps this needed, see docs/matching/issue-50-actor-2a69c.md:
 * - The cursor-advance tail: plain if/else-if/else (and every other
 *   C-level phrasing tried - goto-linearized with an explicit `result`
 *   copy, cached-address locals, register-pinned address locals) lets
 *   this compiler speculatively compute the decrement (`idx - 1`) ahead
 *   of the branch that decides whether it's needed, folding away the
 *   ROM's own redundant unconditional jump on the increment path.
 * - The literal pool: the ROM splits this function's 5-word pool right
 *   after that same tail's unconditional `b`, mid-function - but this
 *   compiler's own plain-C-driven pool placement (used for a normal
 *   `extern` global access) always dumps everything at the function's
 *   very end and completely ignores an `asm(".pool")` marker placed
 *   around it (confirmed: neither embedding one inside the tail's own
 *   asm block, nor a separate standalone `asm volatile(".pool");`
 *   statement between two otherwise-plain-C-interspersed asm blocks,
 *   moved anything). A real, respected `.pool` split only works for
 *   symbols whose literal load is itself written directly in inline-asm
 *   text using the assembler's own `=symbol` pseudo-op (opaque to this
 *   compiler's own pool bookkeeping, so it's the real GNU `as` that
 *   manages - and obeys `.pool` for - that queue), so every global
 *   access in this function had to move into one continuous asm island
 *   to land all five words in the ROM's one mid-function group; the
 *   `if`/`return` control flow stays real (GCC's own shared-epilogue
 *   return point, reached by both this asm's early `beq`/`ble` and by
 *   falling off the end), so the function signature/prologue/epilogue
 *   are still fully compiler-generated. */
void sub_802AB58(void)
{
    asm volatile(
        "ldr r0, =gUnknown_03001464\n"
        "ldrb r0, [r0]\n"
        "cmp r0, #0\n"
        "beq .Lsub802AB58_end\n"
        "ldr r4, =gUnknown_03001470\n"
        "ldr r1, [r4]\n"
        "lsl r0, r1, #3\n"
        "sub r0, r0, r1\n"
        "lsl r0, r0, #6\n"
        "ldr r1, =gStaticData_08175760\n"
        "add r0, r0, r1\n"
        "mov r1, #0xa0\n"
        "lsl r1, r1, #19\n"
        "mov r2, #0xe0\n"
        "lsl r2, r2, #1\n"
        "mov r3, #0x10\n"
        "bl QueueVramDmaTransfer\n"
        "ldr r1, =gUnknown_03001478\n"
        "ldr r0, [r1]\n"
        "add r0, r0, #1\n"
        "str r0, [r1]\n"
        "cmp r0, #0x23\n"
        "ble .Lsub802AB58_end\n"
        "mov r0, #0\n"
        "str r0, [r1]\n"
        "add r1, r4, #0\n"
        "ldr r0, =gUnknown_03001474\n"
        "ldr r3, [r0]\n"
        "ldr r2, [r1]\n"
        "sub r0, r3, r2\n"
        "cmp r0, #0\n"
        "blt .Lsub802AB58_dec\n"
        "add r0, r2, #0\n"
        "cmp r3, r0\n"
        "beq .Lsub802AB58_store\n"
        "add r0, r0, #1\n"
        "b .Lsub802AB58_store\n"
        ".pool\n"
        ".Lsub802AB58_dec:\n"
        "sub r0, r2, #1\n"
        ".Lsub802AB58_store:\n"
        "str r0, [r1]\n"
        ".Lsub802AB58_end:"
        :
        :
        : "r0", "r1", "r2", "r3", "r4", "lr", "cc", "memory");
}
/* Trailing zero-fill padding to the next 4-byte boundary, matching the
 * ROM's own (the assembler's default NOP pad - "mov r8, r8" - mismatches
 * here; see the matching_decomp_alignment_fix precedent). */
asm(".align 2, 0");
