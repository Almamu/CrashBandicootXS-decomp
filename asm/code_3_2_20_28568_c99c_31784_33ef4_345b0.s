.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_80345B0
sub_80345B0: @ 0x080345B0
	push {r4, r5, r6, lr}
	adds r2, r0, #0
	cmp r1, #0x7f
	ble _080345BA
_080345B8:
	b _080345B8
_080345BA:
	lsls r0, r1, #4
	ldr r5, [r2, #8]
	adds r5, r5, r0
	movs r0, #0xf0
	lsls r0, r0, #7
	str r0, [r5]
	movs r0, #0xa0
	lsls r0, r0, #7
	str r0, [r5, #4]
	movs r6, #0x80
	lsls r6, r6, #1
	adds r0, r6, #0
	bl sub_8000E1C
	adds r4, r0, #0
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	movs r0, #0x80
	lsls r0, r0, #2
	bl sub_8000E1C
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r0, r0, r6
	ldr r6, _08034630 @ =gStaticData_0816A820
	adds r1, r4, #0
	adds r1, #0x40
	movs r2, #0xff
	ands r1, r2
	lsls r1, r1, #1
	adds r1, r1, r6
	movs r3, #0
	ldrsh r1, [r1, r3]
	adds r3, r1, #0
	muls r3, r0, r3
	asrs r3, r3, #8
	str r3, [r5, #8]
	ands r4, r2
	lsls r4, r4, #1
	adds r4, r4, r6
	movs r2, #0
	ldrsh r1, [r4, r2]
	adds r2, r1, #0
	muls r2, r0, r2
	asrs r2, r2, #8
	str r2, [r5, #0xc]
	lsls r1, r3, #2
	adds r1, r1, r3
	ldr r0, [r5]
	adds r0, r0, r1
	str r0, [r5]
	lsls r1, r2, #2
	adds r1, r1, r2
	ldr r0, [r5, #4]
	adds r0, r0, r1
	str r0, [r5, #4]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08034630: .4byte gStaticData_0816A820

	thumb_func_start sub_8034634
sub_8034634: @ 0x08034634
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	adds r5, r1, #0
	adds r4, r2, #0
	cmp r5, #0xef
	bhi _08034680
	cmp r4, #0
	blt _08034680
	cmp r4, #0x9f
	bgt _08034680
	asrs r1, r5, #3
	lsls r1, r1, #6
	asrs r2, r4, #3
	lsls r0, r2, #4
	subs r0, r0, r2
	lsls r0, r0, #7
	adds r1, r1, r0
	movs r0, #7
	ands r5, r0
	adds r1, r1, r5
	ands r4, r0
	lsls r0, r4, #3
	adds r1, r1, r0
	asrs r0, r1, #2
	lsls r0, r0, #1
	ldr r2, [r6, #0x10]
	adds r2, r2, r0
	movs r0, #3
	ands r1, r0
	lsls r1, r1, #2
	movs r0, #0xf
	lsls r0, r1
	ldrh r4, [r2]
	bics r4, r0
	adds r0, r4, #0
	lsls r3, r1
	orrs r0, r3
	strh r0, [r2]
_08034680:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

