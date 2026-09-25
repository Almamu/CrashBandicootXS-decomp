.include "asm/macros.inc"

.syntax unified
.arm

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

