.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8034374
sub_8034374: @ 0x08034374
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #8
	adds r6, r0, #0
	movs r0, #0x80
	lsls r0, r0, #4
	bl sub_8026EC0
	str r0, [r6, #8]
	ldr r1, _08034454 @ =gUnknown_03001288
	movs r4, #0
	movs r0, #0x40
	strh r0, [r1]
	movs r0, #0
	bl sub_8001524
	ldr r0, _08034458 @ =0x04000010
	str r4, [r0]
	ldr r0, _0803445C @ =0xFFFF0000
	ands r5, r0
	movs r0, #3
	orrs r5, r0
	movs r0, #0xf8
	lsls r0, r0, #5
	orrs r5, r0
	ldr r0, _08034460 @ =0x04000008
	strh r5, [r0]
	movs r0, #0xc0
	lsls r0, r0, #0x13
	str r0, [r6]
	ldr r0, _08034464 @ =0x0600F800
	str r0, [r6, #4]
	bl sub_80015D0
	movs r0, #0xa0
	lsls r0, r0, #0x13
	strh r4, [r0]
	ldr r4, _08034468 @ =0x050001E0
	add r0, sp, #4
	mov r8, r0
	movs r2, #0
	movs r3, #2
_080343CA:
	lsrs r0, r2, #0x1f
	adds r0, r2, r0
	asrs r0, r0, #1
	lsls r1, r0, #5
	orrs r1, r0
	lsls r0, r0, #0xa
	orrs r1, r0
	strh r1, [r4]
	adds r4, #2
	adds r2, #0x1f
	subs r3, #1
	cmp r3, #0
	bge _080343CA
	movs r3, #0
	movs r0, #0
	ldr r1, [r6]
	mov ip, r1
	ldr r7, [r6, #4]
	ldr r1, _0803446C @ =0xFFFFF000
	adds r5, r1, #0
_080343F2:
	adds r4, r0, #1
	lsls r0, r0, #6
	adds r1, r0, r7
	movs r2, #0x1d
_080343FA:
	adds r0, r3, #0
	orrs r0, r5
	strh r0, [r1]
	adds r3, #1
	adds r1, #2
	subs r2, #1
	cmp r2, #0
	bge _080343FA
	adds r0, r4, #0
	cmp r0, #0x13
	ble _080343F2
	movs r5, #0
	mov r0, sp
	strh r5, [r0]
	ldr r4, _08034470 @ =0x040000D4
	str r0, [r4]
	mov r0, ip
	str r0, [r4, #4]
	ldr r0, _08034474 @ =0x81002580
	str r0, [r4, #8]
	ldr r0, [r4, #8]
	ldr r0, _08034478 @ =0x04000050
	strh r5, [r0]
	bl sub_8001614
	str r5, [r6, #0xc]
	movs r0, #0x96
	lsls r0, r0, #7
	bl sub_8026EC0
	str r0, [r6, #0x10]
	str r5, [sp, #4]
	mov r1, r8
	str r1, [r4]
	str r0, [r4, #4]
	ldr r0, _0803447C @ =0x850012C0
	str r0, [r4, #8]
	ldr r0, [r4, #8]
	adds r0, r6, #0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08034454: .4byte gUnknown_03001288
_08034458: .4byte 0x04000010
_0803445C: .4byte 0xFFFF0000
_08034460: .4byte 0x04000008
_08034464: .4byte 0x0600F800
_08034468: .4byte 0x050001E0
_0803446C: .4byte 0xFFFFF000
_08034470: .4byte 0x040000D4
_08034474: .4byte 0x81002580
_08034478: .4byte 0x04000050
_0803447C: .4byte 0x850012C0

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

