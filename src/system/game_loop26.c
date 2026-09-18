#include "core.h"

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see game_loop17.c's header comment and
 * docs/matching/issue-13-graphics-fc70.md). `sub_8010914`/
 * `sub_801095C`/`sub_80109A4` right before this function are left
 * untouched raw; `sub_8010A0C` right after it is outside this issue's
 * range and also stays raw. */

/* Extracts `self+0x48` bits 6-7 (a 2-bit sub-state field packed
 * alongside the low bits other functions in this subsystem test via
 * `& 7`). */
u32 sub_8010A00(void *selfArg)
{
    u8 *self = selfArg;
    return (*(u32 *)(self + 0x48) & 0xc0) >> 6;
}
/* Trailing byte-padding mismatch fix: the function body is 10 bytes
 * (not a multiple of 4), and the ROM pads the gap before the next
 * function with a zero halfword, not the assembler's default `nop`
 * (`mov r8, r8`) - see matching_decomp_alignment_fix memory. */
asm(".align 2, 0");
