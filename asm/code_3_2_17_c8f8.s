.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_800C8F8
sub_800C8F8: @ 0x0800C8F8
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r5, _0800C934 @ =gStaticData_0816A820
	ldr r0, _0800C938 @ =gUnknown_0300082C
	ldr r0, [r0]
	lsls r0, r0, #8
	ldr r1, [r4, #0x3c]
	bl sub_8037E54
	ldr r1, [r4, #0x40]
	ldr r2, _0800C93C @ =0xFFFFFF00
	adds r1, r1, r2
	subs r0, r0, r1
	movs r1, #0xff
	ands r0, r1
	lsls r0, r0, #1
	adds r0, r0, r5
	movs r2, #0
	ldrsh r1, [r0, r2]
	ldr r0, [r4, #0x44]
	adds r2, r1, #0
	muls r2, r0, r2
	ldr r1, [r4, #0x70]
	ldr r0, [r4, #0x60]
	adds r0, r0, r2
	str r0, [r1]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0800C934: .4byte gStaticData_0816A820
_0800C938: .4byte gUnknown_0300082C
_0800C93C: .4byte 0xFFFFFF00

	thumb_func_start sub_800C940
sub_800C940: @ 0x0800C940
	push {r4, r5, r6, lr}
	ldr r3, [r0, #0x70]
	ldr r4, _0800C970 @ =gStaticData_0816A820
	ldr r1, _0800C974 @ =gUnknown_0300082C
	ldr r1, [r1]
	lsrs r1, r1, #1
	ldr r2, [r0, #0x40]
	ldr r6, _0800C978 @ =0xFFFFFF00
	adds r2, r2, r6
	subs r1, r1, r2
	movs r2, #0xff
	ands r1, r2
	lsls r1, r1, #1
	adds r1, r1, r4
	movs r4, #0
	ldrsh r2, [r1, r4]
	ldr r1, [r0, #0x44]
	muls r1, r2, r1
	ldr r0, [r0, #0x64]
	adds r0, r0, r1
	str r0, [r3, #4]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0800C970: .4byte gStaticData_0816A820
_0800C974: .4byte gUnknown_0300082C
_0800C978: .4byte 0xFFFFFF00

	thumb_func_start sub_800C97C
sub_800C97C: @ 0x0800C97C
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r4, r0, #0
	ldr r5, [r4, #0x70]
	ldr r6, _0800C9BC @ =gStaticData_0816A820
	ldr r0, _0800C9C0 @ =gUnknown_0300082C
	ldr r0, [r0]
	lsls r0, r0, #8
	ldr r1, [r4, #0x3c]
	bl sub_8037E54
	ldr r1, [r4, #0x40]
	ldr r2, _0800C9C4 @ =0xFFFFFF00
	adds r1, r1, r2
	subs r0, r0, r1
	movs r1, #0xff
	ands r0, r1
	lsls r0, r0, #1
	adds r0, r0, r6
	movs r2, #0
	ldrsh r1, [r0, r2]
	ldr r0, [r4, #0x44]
	muls r1, r0, r1
	ldr r0, [r4, #0x64]
	adds r0, r0, r1
	str r0, [r5, #4]
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0800C9BC: .4byte gStaticData_0816A820
_0800C9C0: .4byte gUnknown_0300082C
_0800C9C4: .4byte 0xFFFFFF00

	thumb_func_start sub_800C9C8
sub_800C9C8: @ 0x0800C9C8
	push {r4, r5, r6, lr}
	sub sp, #0xc
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	ldr r1, [sp, #0x1c]
	ldr r2, [sp, #0x20]
	ldr r0, _0800CA00 @ =gUnknown_030012E4
	ldr r0, [r0]
	str r3, [sp]
	str r1, [sp, #4]
	str r2, [sp, #8]
	adds r1, r4, #0
	adds r2, r5, #0
	adds r3, r6, #0
	bl sub_8025B0C
	movs r1, #4
	ldrb r2, [r0, #0xc]
	orrs r1, r2
	movs r2, #0x41
	rsbs r2, r2, #0
	ands r1, r2
	strb r1, [r0, #0xc]
	add sp, #0xc
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0800CA00: .4byte gUnknown_030012E4

	thumb_func_start sub_800CA04
sub_800CA04: @ 0x0800CA04
	str r1, [r0, #0x70]
	bx lr

	thumb_func_start sub_800CA08
sub_800CA08: @ 0x0800CA08
	ldr r2, _0800CA44 @ =gUnknown_030012D8
	ldr r3, [r2]
	ldr r2, [r3]
	asrs r2, r2, #8
	subs r0, r0, r2
	asrs r2, r0, #0x1f
	eors r0, r2
	subs r2, r0, r2
	ldr r0, [r3, #4]
	asrs r0, r0, #8
	subs r1, r1, r0
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	cmp r1, r2
	bge _0800CA2A
	adds r1, r2, #0
_0800CA2A:
	cmp r1, #0x20
	bge _0800CA30
	movs r1, #0x20
_0800CA30:
	cmp r1, #0xa0
	ble _0800CA36
	movs r1, #0xa0
_0800CA36:
	subs r1, #0x20
	lsls r1, r1, #1
	movs r0, #0x80
	lsls r0, r0, #1
	subs r0, r0, r1
	bx lr
	.align 2, 0
_0800CA44: .4byte gUnknown_030012D8

	thumb_func_start sub_800CA48
sub_800CA48: @ 0x0800CA48
	movs r2, #0
	str r2, [r0, #0x70]
	adds r1, r0, #0
	adds r1, #0x84
	str r2, [r1]
	ldr r1, _0800CA5C @ =gStaticData_0816BB6C
	str r1, [r0, #4]
	adds r0, #0x88
	str r2, [r0]
	bx lr
	.align 2, 0
_0800CA5C: .4byte gStaticData_0816BB6C

	thumb_func_start sub_800CA60
sub_800CA60: @ 0x0800CA60
	push {lr}
	ldr r2, _0800CA70 @ =gStaticData_087E3EE4
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_0800CA70: .4byte gStaticData_087E3EE4

	thumb_func_start sub_800CA74
sub_800CA74: @ 0x0800CA74
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800B8C8
	ldr r0, _0800CA90 @ =gStaticData_087E3EE4
	str r0, [r4, #0xc]
	adds r0, r4, #0
	bl sub_800CA48
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0800CA90: .4byte gStaticData_087E3EE4

	thumb_func_start sub_800CA94
sub_800CA94: @ 0x0800CA94
	str r1, [r0, #0x3c]
	str r2, [r0, #0x40]
	str r3, [r0, #0x44]
	bx lr

	thumb_func_start sub_800CA9C
sub_800CA9C: @ 0x0800CA9C
	str r1, [r0, #0x48]
	str r2, [r0, #0x4c]
	bx lr
	.align 2, 0

	thumb_func_start sub_800CAA4
sub_800CAA4: @ 0x0800CAA4
	str r1, [r0, #0x30]
	str r2, [r0, #0x34]
	str r3, [r0, #0x38]
	bx lr

	thumb_func_start sub_800CAAC
sub_800CAAC: @ 0x0800CAAC
	push {r4, lr}
	ldr r4, [sp, #8]
	str r1, [r0, #0x20]
	str r3, [r0, #0x28]
	str r2, [r0, #0x24]
	str r4, [r0, #0x2c]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_800CAC0
sub_800CAC0: @ 0x0800CAC0
	adds r0, #0x84
	str r1, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_800CAC8
sub_800CAC8: @ 0x0800CAC8
	str r1, [r0, #0x6c]
	bx lr

	thumb_func_start sub_800CACC
sub_800CACC: @ 0x0800CACC
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r0, [r4]
	asrs r5, r0, #8
	ldr r0, _0800CB14 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0]
	asrs r0, r0, #8
	subs r0, r5, r0
	subs r0, #0xa1
	cmp r0, #0xee
	bhi _0800CB0C
	ldr r0, _0800CB18 @ =gUnknown_0300082C
	ldr r0, [r0]
	ldr r1, [r4, #0x20]
	adds r0, r0, r1
	ldr r2, [r4, #0x24]
	subs r0, r0, r2
	bl sub_803AE4C
	cmp r0, #0
	bne _0800CB0C
	ldr r0, _0800CB1C @ =0x0000FFFF
	lsls r1, r5, #0x10
	lsrs r1, r1, #0x10
	ldr r2, [r4, #4]
	lsls r2, r2, #8
	lsrs r2, r2, #0x10
	ldr r4, [r4, #0x1c]
	movs r3, #0
	bl sub_803AD88
_0800CB0C:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0800CB14: .4byte gUnknown_030012D8
_0800CB18: .4byte gUnknown_0300082C
_0800CB1C: .4byte 0x0000FFFF

	thumb_func_start sub_800CB20
sub_800CB20: @ 0x0800CB20
	push {lr}
	adds r2, r0, #0
	ldr r0, _0800CB3C @ =gStaticData_087E3BEC
	str r0, [r2, #0x18]
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _0800CB36
	adds r0, r2, #0
	bl sub_8026ED0
_0800CB36:
	pop {r0}
	bx r0
	.align 2, 0
_0800CB3C: .4byte gStaticData_087E3BEC

	thumb_func_start sub_800CB40
sub_800CB40: @ 0x0800CB40
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800725C
	ldr r0, _0800CB54 @ =gStaticData_087E3F4C
	str r0, [r4, #0x18]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0800CB54: .4byte gStaticData_087E3F4C

	thumb_func_start sub_800CB58
sub_800CB58: @ 0x0800CB58
	str r1, [r0, #0x20]
	str r2, [r0, #0x24]
	bx lr
	.align 2, 0

	thumb_func_start sub_800CB60
sub_800CB60: @ 0x0800CB60
	str r1, [r0, #0x1c]
	bx lr

	thumb_func_start sub_800CB64
sub_800CB64: @ 0x0800CB64
	push {r4, lr}
	adds r4, r1, #0
	ldr r1, [r4, #0x18]
	movs r2, #0x28
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #0x2c]
	bl sub_803AD7C
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0800CBAE
	movs r0, #1
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _0800CBB4 @ =0x0000FFFF
	ldrh r2, [r4, #8]
	cmp r2, r0
	beq _0800CBAE
	ldrh r3, [r4, #8]
	ldr r0, _0800CBB8 @ =gUnknown_030012B4
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
_0800CBAE:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0800CBB4: .4byte 0x0000FFFF
_0800CBB8: .4byte gUnknown_030012B4

	thumb_func_start nullsub_14
nullsub_14: @ 0x0800CBBC
	bx lr
	.align 2, 0

	thumb_func_start sub_800CBC0
sub_800CBC0: @ 0x0800CBC0
	push {lr}
	ldr r2, _0800CBD0 @ =gStaticData_087E3FA4
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_0800CBD0: .4byte gStaticData_087E3FA4

	thumb_func_start sub_800CBD4
sub_800CBD4: @ 0x0800CBD4
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800B8C8
	ldr r0, _0800CBF0 @ =gStaticData_087E3FA4
	str r0, [r4, #0xc]
	adds r0, r4, #0
	bl nullsub_14
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0800CBF0: .4byte gStaticData_087E3FA4

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
