@ itoa_arm: the IWRAM image's ARM itoa (runs at 0x03000198, stored in
@ ROM at 0x087E577C), linked right after src/iwram/string_arm.o's four
@ string helpers. UNUSED: no caller anywhere in the ROM (see
@ string_arm.cpp's file comment).
@
@ Writes `value` in `base` to `buf` (upper-case hex digits, a leading '-'
@ for negative values), NUL-terminates it and returns its length. In C++:
@
@     s32 itoa_arm(s32 value, u8 *buf, s32 base)
@     {
@         s32 len = 0, neg = 0, i, j;
@
@         if (value < 0) {
@             neg = 1;
@             value = -value;
@         }
@         if (base != 16) {
@             do {
@                 // BIOS Div (swi 6): r0 = quotient, r1 = remainder
@                 DivResult r = Div(value, base);
@                 value = r.quotient;
@                 buf[len++] = r.remainder + '0';
@             } while (value != 0);
@         } else {
@             do {
@                 s32 digit = value & 15;
@                 value >>= 4;
@                 buf[len++] = digit >= 10 ? digit + 'A' - 10 : digit + '0';
@             } while (value != 0);
@         }
@         if (neg)
@             buf[len++] = '-';
@         buf[len] = 0;
@         i = 0;                          // reverse the digits in place
@         j = len - 1;
@         do {
@             u8 t = buf[i];
@             buf[i] = buf[j];
@             buf[j] = t;
@         } while (++i < --j);
@         return len;
@     }
@
@ Assembly because the ROM's code isn't gcc output (most likely ARM's
@ own compiler, armcc), so no C++ for agbcc_arm reproduces it. It
@ matched as C++ only with seven register pins, a MATCH_KEEP, a
@ MATCH_CONST and three compiler options nothing else needs (#662
@ rounds 6-11; docs/matching/iwram-image.md, "itoa_arm: assembly"). The
@ evidence, four things no gcc we have (agbcc_arm, FSF 2.95.3-3.4.6)
@ produces from any C:
@ - `cmp r1, #10` with `addge`/`addlt`: every gcc folds `digit >= 10` to
@   `digit > 9` (fold-const) and compares with 9.
@ - No conditional returns, and lr is pushed only when it is used
@   (`push {r4, r5, r6}` ... `bx lr`); agbcc_arm pushes lr with any
@   register and turns jumps to the return into `bxeq lr`.
@   (strncpy_arm, in string_arm.cpp, has no conditional return either.)
@ - The shared zero: r4 is the sign flag, then the '-' character, then
@   0 for both the terminator and the swap's left index. On the
@   non-negative path the compiler knows r4 is already 0 and sets it only
@   on the '-' path (`movne r4, #0`).
@ - The SWI returns the quotient in r0 and the remainder in r1, the
@   shape of an armcc `__value_in_regs __swi(0x60000)` declaration.

.include "asm/macros.inc"

.syntax unified

	.text

	arm_func_start itoa_arm
itoa_arm: @ 0x03000198
	push {r4, r5, r6}
	mov r5, #0			@ len = 0
	cmp r0, #0
	movge r4, #0			@ neg = 0
	movlt r4, #1			@ neg = 1
	rsblt r0, r0, #0		@ value = -value
	mov r6, r1			@ buf
	mov r12, r2			@ base
	cmp r2, #16
	beq .Lhex_loop
.Ldiv_loop:
	mov r1, r12
	svc #0x60000			@ Div: r0 = r0 / r1, r1 = r0 % r1 (r3 clobbered)
	add r1, r1, #'0'
	strb r1, [r6, r5]		@ buf[len] = remainder + '0'
	add r5, r5, #1
	cmp r0, #0
	bne .Ldiv_loop
	b .Lsign
.Lhex_loop:
	and r1, r0, #15			@ digit = value & 15
	asr r0, r0, #4			@ value >>= 4
	cmp r1, #10
	addge r1, r1, #'A' - 10
	addlt r1, r1, #'0'
	strb r1, [r6, r5]		@ buf[len] = digit
	add r5, r5, #1
	cmp r0, #0
	bne .Lhex_loop
.Lsign:
	cmp r4, #0
	movne r4, #'-'
	strbne r4, [r6, r5]		@ buf[len++] = '-'
	addne r5, r5, #1
	movne r4, #0			@ r4 = 0 on both paths: i = 0
	strb r4, [r6, r5]		@ buf[len] = 0
	sub r1, r5, #1			@ j = len - 1
.Lswap_loop:
	ldrb r3, [r6, r4]
	ldrb r0, [r6, r1]
	strb r3, [r6, r1]
	strb r0, [r6, r4]		@ swap buf[i], buf[j]
	add r4, r4, #1
	sub r1, r1, #1
	cmp r4, r1
	blt .Lswap_loop
	mov r0, r5			@ return len
	pop {r4, r5, r6}
	bx lr
	arm_func_end itoa_arm
