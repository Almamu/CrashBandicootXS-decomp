.include "asm/macros.inc"

.syntax unified
.arm

@ sub_803ADB4 is reconstructed (but not yet byte-matching) as C in
@ src/util/math_div_util.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching.md, GitHub issue #70's entry.
.if NON_MATCHING == 0
	thumb_func_start sub_803ADB4
sub_803ADB4: @ 0x0803ADB4
	cmp r1, #0
	beq _0803AE3C
	push {r4}
	adds r4, r0, #0
	eors r4, r1
	mov ip, r4
	movs r3, #1
	movs r2, #0
	cmp r1, #0
	bpl _0803ADCA
	rsbs r1, r1, #0
_0803ADCA:
	cmp r0, #0
	bpl _0803ADD0
	rsbs r0, r0, #0
_0803ADD0:
	cmp r0, r1
	blo _0803AE2E
	movs r4, #1
	lsls r4, r4, #0x1c
_0803ADD8:
	cmp r1, r4
	bhs _0803ADE6
	cmp r1, r0
	bhs _0803ADE6
	lsls r1, r1, #4
	lsls r3, r3, #4
	b _0803ADD8
_0803ADE6:
	lsls r4, r4, #3
_0803ADE8:
	cmp r1, r4
	bhs _0803ADF6
	cmp r1, r0
	bhs _0803ADF6
	lsls r1, r1, #1
	lsls r3, r3, #1
	b _0803ADE8
_0803ADF6:
	cmp r0, r1
	blo _0803ADFE
	subs r0, r0, r1
	orrs r2, r3
_0803ADFE:
	lsrs r4, r1, #1
	cmp r0, r4
	blo _0803AE0A
	subs r0, r0, r4
	lsrs r4, r3, #1
	orrs r2, r4
_0803AE0A:
	lsrs r4, r1, #2
	cmp r0, r4
	blo _0803AE16
	subs r0, r0, r4
	lsrs r4, r3, #2
	orrs r2, r4
_0803AE16:
	lsrs r4, r1, #3
	cmp r0, r4
	blo _0803AE22
	subs r0, r0, r4
	lsrs r4, r3, #3
	orrs r2, r4
_0803AE22:
	cmp r0, #0
	beq _0803AE2E
	lsrs r3, r3, #4
	beq _0803AE2E
	lsrs r1, r1, #4
	b _0803ADF6
_0803AE2E:
	adds r0, r2, #0
	mov r4, ip
	cmp r4, #0
	bpl _0803AE38
	rsbs r0, r0, #0
_0803AE38:
	pop {r4}
	mov pc, lr
_0803AE3C:
	push {lr}
	bl nullsub_8
	movs r0, #0
	pop {pc}
	.align 2, 0
.endif
