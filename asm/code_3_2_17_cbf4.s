.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_800CBF4
sub_800CBF4: @ 0x0800CBF4
	push {r4, r5, lr}
	adds r4, r1, #0
	ldr r1, [r4, #0x18]
	movs r2, #0x28
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #0x2c]
	bl sub_803AD7C
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0800CC3E
	movs r0, #1
	ldrb r5, [r4, #0xc]
	orrs r0, r5
	strb r0, [r4, #0xc]
	ldr r0, _0800CCBC @ =0x0000FFFF
	ldrh r1, [r4, #8]
	cmp r1, r0
	beq _0800CC3E
	ldrh r3, [r4, #8]
	ldr r0, _0800CCC0 @ =gUnknown_030012B4
	ldr r2, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r1, r0, #2
	movs r5, #0x84
	lsls r5, r5, #1
	adds r2, r2, r5
	adds r2, r2, r1
	lsls r0, r0, #5
	subs r0, r3, r0
	movs r1, #1
	lsls r1, r0
	ldr r0, [r2]
	orrs r0, r1
	str r0, [r2]
_0800CC3E:
	ldrb r1, [r4, #0xc]
	lsrs r0, r1, #3
	movs r2, #1
	ands r0, r2
	cmp r0, #0
	beq _0800CC7A
	adds r0, r1, #0
	orrs r0, r2
	strb r0, [r4, #0xc]
	ldr r0, _0800CCBC @ =0x0000FFFF
	ldrh r1, [r4, #8]
	cmp r1, r0
	beq _0800CC7A
	ldrh r3, [r4, #8]
	ldr r0, _0800CCC0 @ =gUnknown_030012B4
	ldr r2, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r1, r0, #2
	movs r5, #0x84
	lsls r5, r5, #1
	adds r2, r2, r5
	adds r2, r2, r1
	lsls r0, r0, #5
	subs r0, r3, r0
	movs r1, #1
	lsls r1, r0
	ldr r0, [r2]
	orrs r0, r1
	str r0, [r2]
_0800CC7A:
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800CCB6
	movs r0, #1
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _0800CCBC @ =0x0000FFFF
	ldrh r2, [r4, #8]
	cmp r2, r0
	beq _0800CCB6
	ldrh r3, [r4, #8]
	ldr r0, _0800CCC0 @ =gUnknown_030012B4
	ldr r2, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r1, r0, #2
	movs r4, #0x84
	lsls r4, r4, #1
	adds r2, r2, r4
	adds r2, r2, r1
	lsls r0, r0, #5
	subs r0, r3, r0
	movs r1, #1
	lsls r1, r0
	ldr r0, [r2]
	orrs r0, r1
	str r0, [r2]
_0800CCB6:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0800CCBC: .4byte 0x0000FFFF
_0800CCC0: .4byte gUnknown_030012B4

	thumb_func_start nullsub_15
nullsub_15: @ 0x0800CCC4
	bx lr
	.align 2, 0

	thumb_func_start nullsub_3
nullsub_3: @ 0x0800CCC8
	bx lr
	.align 2, 0

	thumb_func_start sub_800CCCC
sub_800CCCC: @ 0x0800CCCC
	push {lr}
	ldr r2, _0800CCDC @ =gStaticData_087E400C
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_0800CCDC: .4byte gStaticData_087E400C

	thumb_func_start sub_800CCE0
sub_800CCE0: @ 0x0800CCE0
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800B8C8
	ldr r0, _0800CCFC @ =gStaticData_087E400C
	str r0, [r4, #0xc]
	adds r0, r4, #0
	bl nullsub_3
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0800CCFC: .4byte gStaticData_087E400C
