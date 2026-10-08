@ libgcc's lib1funcs.asm routines: hand-written Thumb, never compiled C.
@
@ As in gcc's own libgcc build, this one file is assembled once per
@ routine with `--defsym L_<name>=1` (Makefile LIB1FUNCS), one object per
@ routine: _udivsi3.o, _divsi3.o, _dvmd_tls.o, _modsi3.o, _umodsi3.o and
@ _call_via_rX.o. ldscript.txt places each where the ROM has it.
@
@ The bodies are byte-verified transcriptions of the ROM (they used to be
@ NAKED functions in lib/libgcc/lib1funcs.s, libgcc2.c and
@ lib/libgcc/lib1funcs.s). tools/report_units.py marks their ranges
@ HANDWRITTEN, so they count neither as matched nor as unmatched code.

	.text
	.code 16

.ifdef L_udivsi3

@ __udivsi3: unsigned 32-bit division, quotient only (what `/` on unsigned
@ operands compiles to). Shift-and-subtract long division: normalizes the
@ divisor and the quotient bit up 4 bits at a time, then 1 bit at a time,
@ then tests 4 candidate quotient bits per pass. A zero divisor calls
@ __div0 and returns 0. Linked between _udivdi3.o and _muldi3.o (inside
@ the GAX2 library's range).

	.align 2, 0
	.global __udivsi3
	.type __udivsi3, %function
	.thumb_func
__udivsi3:
	cmp r1, #0
	beq 10f
	mov r3, #1
	mov r2, #0
	push {r4}
	cmp r0, r1
	blo 9f
	mov r4, #1
	lsl r4, r4, #0x1c
1:
	cmp r1, r4
	bhs 2f
	cmp r1, r0
	bhs 2f
	lsl r1, r1, #4
	lsl r3, r3, #4
	b 1b
2:
	lsl r4, r4, #3
3:
	cmp r1, r4
	bhs 4f
	cmp r1, r0
	bhs 4f
	lsl r1, r1, #1
	lsl r3, r3, #1
	b 3b
4:
	cmp r0, r1
	blo 5f
	sub r0, r0, r1
	orr r2, r3
5:
	lsr r4, r1, #1
	cmp r0, r4
	blo 6f
	sub r0, r0, r4
	lsr r4, r3, #1
	orr r2, r4
6:
	lsr r4, r1, #2
	cmp r0, r4
	blo 7f
	sub r0, r0, r4
	lsr r4, r3, #2
	orr r2, r4
7:
	lsr r4, r1, #3
	cmp r0, r4
	blo 8f
	sub r0, r0, r4
	lsr r4, r3, #3
	orr r2, r4
8:
	cmp r0, #0
	beq 9f
	lsr r3, r3, #4
	beq 9f
	lsr r1, r1, #4
	b 4b
9:
	add r0, r2, #0
	pop {r4}
	mov pc, lr
10:
	push {lr}
	bl __div0
	mov r0, #0
	pop {pc}
	.size __udivsi3, .-__udivsi3

	.align 2, 0 @ pad with zeros, not the assembler's nops
.endif @ L_udivsi3

.ifdef L_divsi3

@ __divsi3: signed 32-bit division, truncating toward zero (what `/`
@ compiles to, also called directly, e.g. fixed_math.cpp's FixedDiv
@ wrappers). __udivsi3's loop on the magnitudes, with the result sign
@ (dividend ^ divisor) kept in ip. Note the per-path register saves:
@ `push {r4}` ... `mov pc, lr` on the normal path, a separate
@ `push {lr}; bl __div0; ...; pop {pc}` only on a zero divisor - the
@ shape that shows this is hand-written, not compiler output (agbcc
@ can't shrink-wrap). Linked after the AgbEeprom library with the rest
@ of the game's lib1funcs routines.

	.align 2, 0
	.global __divsi3
	.type __divsi3, %function
	.thumb_func
__divsi3:
	cmp r1, #0
	beq 13f
	push {r4}
	add r4, r0, #0
	eor r4, r1
	mov ip, r4
	mov r3, #1
	mov r2, #0
	cmp r1, #0
	bpl 1f
	neg r1, r1
1:
	cmp r0, #0
	bpl 2f
	neg r0, r0
2:
	cmp r0, r1
	blo 11f
	mov r4, #1
	lsl r4, r4, #0x1c
3:
	cmp r1, r4
	bhs 4f
	cmp r1, r0
	bhs 4f
	lsl r1, r1, #4
	lsl r3, r3, #4
	b 3b
4:
	lsl r4, r4, #3
5:
	cmp r1, r4
	bhs 6f
	cmp r1, r0
	bhs 6f
	lsl r1, r1, #1
	lsl r3, r3, #1
	b 5b
6:
	cmp r0, r1
	blo 7f
	sub r0, r0, r1
	orr r2, r3
7:
	lsr r4, r1, #1
	cmp r0, r4
	blo 8f
	sub r0, r0, r4
	lsr r4, r3, #1
	orr r2, r4
8:
	lsr r4, r1, #2
	cmp r0, r4
	blo 9f
	sub r0, r0, r4
	lsr r4, r3, #2
	orr r2, r4
9:
	lsr r4, r1, #3
	cmp r0, r4
	blo 10f
	sub r0, r0, r4
	lsr r4, r3, #3
	orr r2, r4
10:
	cmp r0, #0
	beq 11f
	lsr r3, r3, #4
	beq 11f
	lsr r1, r1, #4
	b 6b
11:
	add r0, r2, #0
	mov r4, ip
	cmp r4, #0
	bpl 12f
	neg r0, r0
12:
	pop {r4}
	mov pc, lr
13:
	push {lr}
	bl __div0
	mov r0, #0
	pop {pc}
	.size __divsi3, .-__divsi3

	.align 2, 0 @ pad with zeros, not the assembler's nops
.endif @ L_divsi3

.ifdef L_dvmd_tls

@ __div0: the divide-by-zero handler the division/modulo routines call.
@ A no-op (`mov pc, lr`).

	.align 2, 0
	.global __div0
	.type __div0, %function
	.thumb_func
__div0:
	mov pc, lr
	.size __div0, .-__div0

	.align 2, 0 @ pad with zeros, not the assembler's nops
.endif @ L_dvmd_tls

.ifdef L_modsi3

@ __modsi3: signed modulo (C's `%`, the result takes the dividend's
@ sign). The same normalize/subtract loop as __divsi3, but it tracks a
@ rotated (`ror`) copy of the bit weight as a correction mask, used
@ afterwards to add back the fractional divisor amounts that were
@ subtracted from the running remainder.

	.align 2, 0
	.global __modsi3
	.type __modsi3, %function
	.thumb_func
__modsi3:
	mov r3, #1
	cmp r1, #0
	beq 16f
	bpl 1f
	neg r1, r1
1:
	push {r4}
	push {r0}
	cmp r0, #0
	bpl 2f
	neg r0, r0
2:
	cmp r0, r1
	blo 14f
	mov r4, #1
	lsl r4, r4, #0x1c
3:
	cmp r1, r4
	bhs 4f
	cmp r1, r0
	bhs 4f
	lsl r1, r1, #4
	lsl r3, r3, #4
	b 3b
4:
	lsl r4, r4, #3
5:
	cmp r1, r4
	bhs 6f
	cmp r1, r0
	bhs 6f
	lsl r1, r1, #1
	lsl r3, r3, #1
	b 5b
6:
	mov r2, #0
	cmp r0, r1
	blo 7f
	sub r0, r0, r1
7:
	lsr r4, r1, #1
	cmp r0, r4
	blo 8f
	sub r0, r0, r4
	mov ip, r3
	mov r4, #1
	ror r3, r4
	orr r2, r3
	mov r3, ip
8:
	lsr r4, r1, #2
	cmp r0, r4
	blo 9f
	sub r0, r0, r4
	mov ip, r3
	mov r4, #2
	ror r3, r4
	orr r2, r3
	mov r3, ip
9:
	lsr r4, r1, #3
	cmp r0, r4
	blo 10f
	sub r0, r0, r4
	mov ip, r3
	mov r4, #3
	ror r3, r4
	orr r2, r3
	mov r3, ip
10:
	mov ip, r3
	cmp r0, #0
	beq 11f
	lsr r3, r3, #4
	beq 11f
	lsr r1, r1, #4
	b 6b
11:
	mov r4, #0xe
	lsl r4, r4, #0x1c
	and r2, r4
	beq 14f
	mov r3, ip
	mov r4, #3
	ror r3, r4
	tst r2, r3
	beq 12f
	lsr r4, r1, #3
	add r0, r0, r4
12:
	mov r3, ip
	mov r4, #2
	ror r3, r4
	tst r2, r3
	beq 13f
	lsr r4, r1, #2
	add r0, r0, r4
13:
	mov r3, ip
	mov r4, #1
	ror r3, r4
	tst r2, r3
	beq 14f
	lsr r4, r1, #1
	add r0, r0, r4
14:
	pop {r4}
	cmp r4, #0
	bpl 15f
	neg r0, r0
15:
	pop {r4}
	mov pc, lr
16:
	push {lr}
	bl __div0
	mov r0, #0
	pop {pc}
	.size __modsi3, .-__modsi3

	.align 2, 0 @ pad with zeros, not the assembler's nops
.endif @ L_modsi3

.ifdef L_umodsi3

@ __umodsi3: unsigned modulo. __modsi3 without the sign handling, plus a
@ `dividend < divisor` early return that pushes nothing.

	.align 2, 0
	.global __umodsi3
	.type __umodsi3, %function
	.thumb_func
__umodsi3:
	cmp r1, #0
	beq 15f
	mov r3, #1
	cmp r0, r1
	bhs 1f
	mov pc, lr
1:
	push {r4}
	mov r4, #1
	lsl r4, r4, #0x1c
2:
	cmp r1, r4
	bhs 3f
	cmp r1, r0
	bhs 3f
	lsl r1, r1, #4
	lsl r3, r3, #4
	b 2b
3:
	lsl r4, r4, #3
4:
	cmp r1, r4
	bhs 5f
	cmp r1, r0
	bhs 5f
	lsl r1, r1, #1
	lsl r3, r3, #1
	b 4b
5:
	mov r2, #0
	cmp r0, r1
	blo 6f
	sub r0, r0, r1
6:
	lsr r4, r1, #1
	cmp r0, r4
	blo 7f
	sub r0, r0, r4
	mov ip, r3
	mov r4, #1
	ror r3, r4
	orr r2, r3
	mov r3, ip
7:
	lsr r4, r1, #2
	cmp r0, r4
	blo 8f
	sub r0, r0, r4
	mov ip, r3
	mov r4, #2
	ror r3, r4
	orr r2, r3
	mov r3, ip
8:
	lsr r4, r1, #3
	cmp r0, r4
	blo 9f
	sub r0, r0, r4
	mov ip, r3
	mov r4, #3
	ror r3, r4
	orr r2, r3
	mov r3, ip
9:
	mov ip, r3
	cmp r0, #0
	beq 10f
	lsr r3, r3, #4
	beq 10f
	lsr r1, r1, #4
	b 5b
10:
	mov r4, #0xe
	lsl r4, r4, #0x1c
	and r2, r4
	bne 11f
	pop {r4}
	mov pc, lr
11:
	mov r3, ip
	mov r4, #3
	ror r3, r4
	tst r2, r3
	beq 12f
	lsr r4, r1, #3
	add r0, r0, r4
12:
	mov r3, ip
	mov r4, #2
	ror r3, r4
	tst r2, r3
	beq 13f
	lsr r4, r1, #2
	add r0, r0, r4
13:
	mov r3, ip
	mov r4, #1
	ror r3, r4
	tst r2, r3
	beq 14f
	lsr r4, r1, #1
	add r0, r0, r4
14:
	pop {r4}
	mov pc, lr
15:
	push {lr}
	bl __div0
	mov r0, #0
	pop {pc}
	.size __umodsi3, .-__umodsi3

	.align 2, 0 @ pad with zeros, not the assembler's nops
.endif @ L_umodsi3

.ifdef L_call_via_rX

@ _call_via_rN: the `bx rN` trampolines Thumb code uses for indirect
@ calls (`bl _call_via_rN`; ARMv4T Thumb has no `blx reg`): the target
@ is already in rN. Only _call_via_r0-r7 and _call_via_lr carry labels
@ here; the r8-sp entries follow _call_via_r7 inside its size, as the
@ original disassembly had them. _call_via_lr has no caller.

	.align 2, 0
	.global _call_via_r0
	.type _call_via_r0, %function
	.thumb_func
_call_via_r0:
	bx r0
	nop
	.size _call_via_r0, .-_call_via_r0
	.align 2, 0
	.global _call_via_r1
	.type _call_via_r1, %function
	.thumb_func
_call_via_r1:
	bx r1
	nop
	.size _call_via_r1, .-_call_via_r1
	.align 2, 0
	.global _call_via_r2
	.type _call_via_r2, %function
	.thumb_func
_call_via_r2:
	bx r2
	nop
	.size _call_via_r2, .-_call_via_r2
	.align 2, 0
	.global _call_via_r3
	.type _call_via_r3, %function
	.thumb_func
_call_via_r3:
	bx r3
	nop
	.size _call_via_r3, .-_call_via_r3
	.align 2, 0
	.global _call_via_r4
	.type _call_via_r4, %function
	.thumb_func
_call_via_r4:
	bx r4
	nop
	.size _call_via_r4, .-_call_via_r4
	.align 2, 0
	.global _call_via_r5
	.type _call_via_r5, %function
	.thumb_func
_call_via_r5:
	bx r5
	nop
	.size _call_via_r5, .-_call_via_r5
	.align 2, 0
	.global _call_via_r6
	.type _call_via_r6, %function
	.thumb_func
_call_via_r6:
	bx r6
	nop
	.size _call_via_r6, .-_call_via_r6
	.align 2, 0
	.global _call_via_r7
	.type _call_via_r7, %function
	.thumb_func
_call_via_r7:
	bx r7
	nop
	bx r8
	nop
	bx r9
	nop
	bx sl
	nop
	bx fp
	nop
	bx ip
	nop
	bx sp
	nop
	.size _call_via_r7, .-_call_via_r7
	.align 2, 0
	.global _call_via_lr
	.type _call_via_lr, %function
	.thumb_func
_call_via_lr:
	bx lr
	nop
	.size _call_via_lr, .-_call_via_lr

	.align 2, 0 @ pad with zeros, not the assembler's nops
.endif @ L_call_via_rX
