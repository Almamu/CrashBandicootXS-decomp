.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8034480
sub_8034480: @ 0x08034480
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	adds r6, r0, #0
	ldr r1, _0803459C @ =0x040000D4
	ldr r2, [r6, #0x10]
	str r2, [r1]
	ldr r0, [r6]
	str r0, [r1, #4]
	ldr r0, _080345A0 @ =0x840012C0
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	movs r0, #0
	str r0, [sp]
	mov r0, sp
	str r0, [r1]
	str r2, [r1, #4]
	ldr r0, _080345A4 @ =0x850012C0
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r5, [r6, #8]
	movs r7, #0
	ldr r0, [r6, #0xc]
	cmp r7, r0
	bge _0803458C
	movs r0, #1
	mov sb, r0
	movs r0, #7
	mov r8, r0
_080344BE:
	ldr r1, [r5]
	asrs r4, r1, #8
	ldr r2, [r5, #4]
	asrs r3, r2, #8
	cmp r4, #0xef
	bhi _0803450A
	cmp r3, #0
	blt _0803450A
	cmp r3, #0x9f
	bgt _0803450A
	asrs r1, r1, #0xb
	lsls r1, r1, #6
	asrs r2, r2, #0xb
	lsls r0, r2, #4
	subs r0, r0, r2
	lsls r0, r0, #7
	adds r1, r1, r0
	mov r0, r8
	ands r4, r0
	adds r1, r1, r4
	ands r3, r0
	lsls r0, r3, #3
	adds r1, r1, r0
	asrs r0, r1, #2
	lsls r0, r0, #1
	ldr r3, [r6, #0x10]
	adds r3, r3, r0
	movs r0, #3
	ands r1, r0
	lsls r1, r1, #2
	movs r0, #0xf
	lsls r0, r1
	ldrh r2, [r3]
	bics r2, r0
	mov r0, sb
	lsls r0, r1
	orrs r2, r0
	strh r2, [r3]
_0803450A:
	ldr r2, [r5]
	ldr r0, [r5, #8]
	adds r2, r2, r0
	str r2, [r5]
	ldr r1, [r5, #4]
	ldr r0, [r5, #0xc]
	adds r1, r1, r0
	str r1, [r5, #4]
	ldr r0, _080345A8 @ =0x0000EFFF
	cmp r2, r0
	bhi _0803452A
	cmp r1, #0
	blt _0803452A
	ldr r0, _080345AC @ =0x00009FFF
	cmp r1, r0
	ble _08034532
_0803452A:
	adds r0, r6, #0
	adds r1, r7, #0
	bl sub_80345B0
_08034532:
	ldr r1, [r5]
	asrs r4, r1, #8
	ldr r2, [r5, #4]
	asrs r3, r2, #8
	movs r0, #2
	mov ip, r0
	cmp r4, #0xef
	bhi _08034582
	cmp r3, #0
	blt _08034582
	cmp r3, #0x9f
	bgt _08034582
	asrs r1, r1, #0xb
	lsls r1, r1, #6
	asrs r2, r2, #0xb
	lsls r0, r2, #4
	subs r0, r0, r2
	lsls r0, r0, #7
	adds r1, r1, r0
	mov r0, r8
	ands r4, r0
	adds r1, r1, r4
	ands r3, r0
	lsls r0, r3, #3
	adds r1, r1, r0
	asrs r0, r1, #2
	lsls r0, r0, #1
	ldr r3, [r6, #0x10]
	adds r3, r3, r0
	movs r0, #3
	ands r1, r0
	lsls r1, r1, #2
	movs r0, #0xf
	lsls r0, r1
	ldrh r2, [r3]
	bics r2, r0
	mov r0, ip
	lsls r0, r1
	orrs r2, r0
	strh r2, [r3]
_08034582:
	adds r5, #0x10
	adds r7, #1
	ldr r0, [r6, #0xc]
	cmp r7, r0
	blt _080344BE
_0803458C:
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0803459C: .4byte 0x040000D4
_080345A0: .4byte 0x840012C0
_080345A4: .4byte 0x850012C0
_080345A8: .4byte 0x0000EFFF
_080345AC: .4byte 0x00009FFF

