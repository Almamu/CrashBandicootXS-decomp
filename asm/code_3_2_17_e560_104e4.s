.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_80104E4
sub_80104E4: @ 0x080104E4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r4, r0, #0
	adds r1, r4, #0
	adds r1, #0x4f
	ldrb r0, [r1]
	cmp r0, #0
	beq _0801055C
	subs r0, #1
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x4e
	ldrb r1, [r0]
	adds r6, r0, #0
	cmp r1, #0x12
	ble _0801051C
	cmp r1, #0x15
	bgt _0801051C
	adds r0, r4, #0
	bl sub_800F8E0
	ldr r1, _08010518 @ =gUnknown_030012B0
	movs r0, #1
	strb r0, [r1]
	b _0801055C
	.align 2, 0
_08010518: .4byte gUnknown_030012B0
_0801051C:
	ldrb r6, [r6]
	cmp r6, #0xf
	bne _08010538
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _08010538
	adds r0, r4, #0
	bl sub_800F990
	b _0801055C
_08010538:
	adds r0, r4, #0
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #0xc
	bne _08010552
	adds r0, r4, #0
	adds r0, #0x4f
	ldrb r1, [r0]
	cmp r1, #0
	bne _0801055C
	adds r0, #1
	strb r1, [r0]
	b _0801055C
_08010552:
	cmp r0, #3
	bne _0801055C
	adds r0, r4, #0
	bl sub_800F4F4
_0801055C:
	adds r0, r4, #0
	adds r0, #0x4e
	adds r6, r0, #0
	ldrb r0, [r6]
	cmp r0, #0xc
	bne _08010572
	ldr r0, [r4, #0x48]
	cmp r0, #0
	ble _08010572
	subs r0, #1
	str r0, [r4, #0x48]
_08010572:
	adds r0, r4, #0
	bl sub_800FC70
	adds r2, r4, #0
	adds r2, #0x4d
	ldrb r1, [r2]
	movs r0, #0x80
	ands r0, r1
	cmp r0, #0
	beq _08010646
	movs r1, #0x38
	adds r1, r1, r4
	mov r8, r1
	ldrb r0, [r1]
	cmp r0, #0
	beq _08010654
	movs r3, #0
	mov ip, r3
	ldr r0, [r4, #0x20]
	adds r5, r4, #0
	adds r5, #0x2d
	ldr r1, [r0]
	ldrb r7, [r5]
	lsls r0, r7, #3
	subs r0, r0, r7
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _080105B2
	subs r0, #1
	mov ip, r0
_080105B2:
	mov r0, ip
	str r0, [r4, #0x30]
	mov r1, r8
	strb r3, [r1]
	movs r0, #0x7f
	ldrb r7, [r2]
	ands r0, r7
	strb r0, [r2]
	ldr r0, _08010620 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x80
	strb r3, [r0]
	ldrb r0, [r6]
	cmp r0, #6
	bne _08010628
	movs r0, #7
	strb r0, [r6]
	movs r0, #0x20
	strb r0, [r5]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r4, #0x20]
	ldr r1, [r0]
	ldrb r2, [r5]
	lsls r0, r2, #3
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r1, r1, r0
	ldr r0, _08010624 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldrb r1, [r1, #0x14]
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	b _08010654
	.align 2, 0
_08010620: .4byte gUnknown_030012D8
_08010624: .4byte gUnknown_030012B8
_08010628:
	cmp r0, #3
	bne _08010654
	movs r0, #0x20
	strb r0, [r5]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	b _08010654
_08010646:
	movs r0, #0x7f
	ands r0, r1
	cmp r0, #1
	bne _08010654
	adds r0, r4, #0
	bl sub_800F798
_08010654:
	adds r0, r4, #0
	bl sub_8008044
	ldr r1, [r4, #0x18]
	adds r1, #0x60
	movs r7, #0
	ldrsh r0, [r1, r7]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
