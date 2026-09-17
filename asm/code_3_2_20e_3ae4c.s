.include "asm/macros.inc"

.syntax unified
.arm

@ sub_803AE4C/sub_803AF1C are reconstructed (but not yet byte-matching)
@ as C in src/util/math_div_util.c, guarded by #if NON_MATCHING - this
@ raw version is only assembled for the default (matching) build. See
@ docs/matching.md, GitHub issue #70's entry.
.if NON_MATCHING == 0
	thumb_func_start sub_803AE4C
sub_803AE4C: @ 0x0803AE4C
	movs r3, #1
	cmp r1, #0
	beq _0803AF10
	bpl _0803AE56
	rsbs r1, r1, #0
_0803AE56:
	push {r4}
	push {r0}
	cmp r0, #0
	bpl _0803AE60
	rsbs r0, r0, #0
_0803AE60:
	cmp r0, r1
	blo _0803AF04
	movs r4, #1
	lsls r4, r4, #0x1c
_0803AE68:
	cmp r1, r4
	bhs _0803AE76
	cmp r1, r0
	bhs _0803AE76
	lsls r1, r1, #4
	lsls r3, r3, #4
	b _0803AE68
_0803AE76:
	lsls r4, r4, #3
_0803AE78:
	cmp r1, r4
	bhs _0803AE86
	cmp r1, r0
	bhs _0803AE86
	lsls r1, r1, #1
	lsls r3, r3, #1
	b _0803AE78
_0803AE86:
	movs r2, #0
	cmp r0, r1
	blo _0803AE8E
	subs r0, r0, r1
_0803AE8E:
	lsrs r4, r1, #1
	cmp r0, r4
	blo _0803AEA0
	subs r0, r0, r4
	mov ip, r3
	movs r4, #1
	rors r3, r4
	orrs r2, r3
	mov r3, ip
_0803AEA0:
	lsrs r4, r1, #2
	cmp r0, r4
	blo _0803AEB2
	subs r0, r0, r4
	mov ip, r3
	movs r4, #2
	rors r3, r4
	orrs r2, r3
	mov r3, ip
_0803AEB2:
	lsrs r4, r1, #3
	cmp r0, r4
	blo _0803AEC4
	subs r0, r0, r4
	mov ip, r3
	movs r4, #3
	rors r3, r4
	orrs r2, r3
	mov r3, ip
_0803AEC4:
	mov ip, r3
	cmp r0, #0
	beq _0803AED2
	lsrs r3, r3, #4
	beq _0803AED2
	lsrs r1, r1, #4
	b _0803AE86
_0803AED2:
	movs r4, #0xe
	lsls r4, r4, #0x1c
	ands r2, r4
	beq _0803AF04
	mov r3, ip
	movs r4, #3
	rors r3, r4
	tst r2, r3
	beq _0803AEE8
	lsrs r4, r1, #3
	adds r0, r0, r4
_0803AEE8:
	mov r3, ip
	movs r4, #2
	rors r3, r4
	tst r2, r3
	beq _0803AEF6
	lsrs r4, r1, #2
	adds r0, r0, r4
_0803AEF6:
	mov r3, ip
	movs r4, #1
	rors r3, r4
	tst r2, r3
	beq _0803AF04
	lsrs r4, r1, #1
	adds r0, r0, r4
_0803AF04:
	pop {r4}
	cmp r4, #0
	bpl _0803AF0C
	rsbs r0, r0, #0
_0803AF0C:
	pop {r4}
	mov pc, lr
_0803AF10:
	push {lr}
	bl nullsub_8
	movs r0, #0
	pop {pc}
	.align 2, 0

	thumb_func_start sub_803AF1C
sub_803AF1C: @ 0x0803AF1C
	cmp r1, #0
	beq _0803AFD2
	movs r3, #1
	cmp r0, r1
	bhs _0803AF28
	mov pc, lr
_0803AF28:
	push {r4}
	movs r4, #1
	lsls r4, r4, #0x1c
_0803AF2E:
	cmp r1, r4
	bhs _0803AF3C
	cmp r1, r0
	bhs _0803AF3C
	lsls r1, r1, #4
	lsls r3, r3, #4
	b _0803AF2E
_0803AF3C:
	lsls r4, r4, #3
_0803AF3E:
	cmp r1, r4
	bhs _0803AF4C
	cmp r1, r0
	bhs _0803AF4C
	lsls r1, r1, #1
	lsls r3, r3, #1
	b _0803AF3E
_0803AF4C:
	movs r2, #0
	cmp r0, r1
	blo _0803AF54
	subs r0, r0, r1
_0803AF54:
	lsrs r4, r1, #1
	cmp r0, r4
	blo _0803AF66
	subs r0, r0, r4
	mov ip, r3
	movs r4, #1
	rors r3, r4
	orrs r2, r3
	mov r3, ip
_0803AF66:
	lsrs r4, r1, #2
	cmp r0, r4
	blo _0803AF78
	subs r0, r0, r4
	mov ip, r3
	movs r4, #2
	rors r3, r4
	orrs r2, r3
	mov r3, ip
_0803AF78:
	lsrs r4, r1, #3
	cmp r0, r4
	blo _0803AF8A
	subs r0, r0, r4
	mov ip, r3
	movs r4, #3
	rors r3, r4
	orrs r2, r3
	mov r3, ip
_0803AF8A:
	mov ip, r3
	cmp r0, #0
	beq _0803AF98
	lsrs r3, r3, #4
	beq _0803AF98
	lsrs r1, r1, #4
	b _0803AF4C
_0803AF98:
	movs r4, #0xe
	lsls r4, r4, #0x1c
	ands r2, r4
	bne _0803AFA4
	pop {r4}
	mov pc, lr
_0803AFA4:
	mov r3, ip
	movs r4, #3
	rors r3, r4
	tst r2, r3
	beq _0803AFB2
	lsrs r4, r1, #3
	adds r0, r0, r4
_0803AFB2:
	mov r3, ip
	movs r4, #2
	rors r3, r4
	tst r2, r3
	beq _0803AFC0
	lsrs r4, r1, #2
	adds r0, r0, r4
_0803AFC0:
	mov r3, ip
	movs r4, #1
	rors r3, r4
	tst r2, r3
	beq _0803AFCE
	lsrs r4, r1, #1
	adds r0, r0, r4
_0803AFCE:
	pop {r4}
	mov pc, lr
_0803AFD2:
	push {lr}
	bl nullsub_8
	movs r0, #0
	pop {pc}
.endif
