.include "asm/macros.inc"

.syntax unified
.arm
	thumb_func_start sub_8001C80
sub_8001C80: @ 0x08001C80
	push {lr}
	ldr r1, _08001C9C @ =sub_8001CA4
	movs r0, #2
	bl sub_80005A0
	ldr r1, _08001CA0 @ =0x04000004
	movs r0, #0x35
	strb r0, [r1, #1]
	movs r0, #0x20
	ldrb r2, [r1]
	orrs r0, r2
	strb r0, [r1]
	pop {r0}
	bx r0
	.align 2, 0
_08001C9C: .4byte sub_8001CA4
_08001CA0: .4byte 0x04000004

	thumb_func_start sub_8001CA4
sub_8001CA4: @ 0x08001CA4
	push {lr}
	ldr r0, _08001CB4 @ =gUnknown_030012BC
	ldr r0, [r0]
	bl sub_80016EC
	pop {r0}
	bx r0
	.align 2, 0
_08001CB4: .4byte gUnknown_030012BC

	thumb_func_start sub_8001CB8
sub_8001CB8: @ 0x08001CB8
	push {r4, r5, r6, r7, lr}
	adds r3, r0, #0
	movs r1, #0xec
	adds r0, r3, #7
_08001CC0:
	strb r1, [r0]
	subs r0, #1
	cmp r0, r3
	bge _08001CC0
	movs r0, #0xf
	ldrb r1, [r3]
	ands r0, r1
	strb r0, [r3]
	movs r0, #0
	strb r0, [r3, #1]
	ldr r1, _08001D28 @ =0x00001234
	adds r2, r3, #1
	movs r4, #4
	ldr r7, _08001D2C @ =gStaticData_0816AF10
	mov ip, r7
	movs r6, #0xff
	movs r5, #1
	rsbs r5, r5, #0
_08001CE4:
	lsrs r0, r1, #8
	ldrb r7, [r2]
	eors r0, r7
	ands r0, r6
	lsls r0, r0, #1
	add r0, ip
	lsls r1, r1, #8
	ldrh r0, [r0]
	eors r1, r0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	adds r2, #1
	subs r4, #1
	cmp r4, r5
	bne _08001CE4
	strb r1, [r3, #6]
	lsrs r0, r1, #8
	strb r0, [r3, #7]
	ldrb r2, [r3]
	lsrs r1, r2, #4
	lsls r0, r0, #8
	ldrb r4, [r3, #6]
	orrs r0, r4
	adds r1, r1, r0
	movs r0, #0xf
	ands r1, r0
	subs r0, #0x1f
	ands r0, r2
	orrs r0, r1
	strb r0, [r3]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08001D28: .4byte 0x00001234
_08001D2C: .4byte gStaticData_0816AF10

	thumb_func_start sub_8001D30
sub_8001D30: @ 0x08001D30
	push {r4, r5, lr}
	ldr r4, _08001D94 @ =0x04000208
	movs r5, #0
	strh r5, [r4]
	ldrh r1, [r4]
	strh r5, [r4]
	ldr r3, _08001D98 @ =0x04000200
	ldrh r2, [r3]
	ldr r0, _08001D9C @ =0x0000FF7F
	ands r0, r2
	strh r0, [r3]
	strh r1, [r4]
	ldrh r1, [r4]
	strh r5, [r4]
	ldrh r2, [r3]
	ldr r0, _08001DA0 @ =0x0000FFBF
	ands r0, r2
	strh r0, [r3]
	strh r1, [r4]
	movs r0, #7
	bl sub_8000544
	movs r0, #6
	bl sub_8000544
	movs r0, #1
	strh r0, [r4]
	ldr r0, _08001DA4 @ =0x04000134
	strh r5, [r0]
	ldr r1, _08001DA8 @ =0x04000128
	movs r2, #0xc0
	lsls r2, r2, #6
	adds r0, r2, #0
	strh r0, [r1]
	subs r1, #0x1c
	ldr r0, _08001DAC @ =0x0000BBBC
	str r0, [r1]
	ldr r2, _08001DB0 @ =0x04000202
	ldrh r0, [r2]
	movs r1, #0x80
	orrs r0, r1
	strh r0, [r2]
	ldrh r0, [r2]
	movs r1, #0x40
	orrs r0, r1
	strh r0, [r2]
	movs r0, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08001D94: .4byte 0x04000208
_08001D98: .4byte 0x04000200
_08001D9C: .4byte 0x0000FF7F
_08001DA0: .4byte 0x0000FFBF
_08001DA4: .4byte 0x04000134
_08001DA8: .4byte 0x04000128
_08001DAC: .4byte 0x0000BBBC
_08001DB0: .4byte 0x04000202

	thumb_func_start sub_8001DB4
sub_8001DB4: @ 0x08001DB4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	adds r5, r0, #0
	movs r2, #0
	strb r2, [r5, #6]
	strb r2, [r5, #8]
	strb r2, [r5, #7]
	ldr r1, _08001F40 @ =gUnknown_03000800
	movs r0, #1
	strb r0, [r1]
	strb r2, [r5, #4]
	movs r1, #1
	rsbs r1, r1, #0
	str r1, [r5, #0x1c]
	movs r3, #0xff
	lsls r3, r3, #2
	adds r0, r5, r3
	str r1, [r0]
	adds r0, r5, #0
	adds r0, #0xc4
	str r2, [r0]
	adds r0, #4
	str r2, [r0]
	adds r1, r5, #0
	adds r1, #0xcc
	movs r0, #0x7f
	str r0, [r1]
	adds r4, r5, #0
	adds r4, #0x30
	adds r0, r4, #0
	bl sub_8001CB8
	adds r2, r5, #0
	adds r2, #0x28
	movs r6, #0xff
	movs r3, #3
_08001E04:
	ldrb r7, [r2, #9]
	lsls r1, r7, #8
	ldrb r0, [r2, #8]
	orrs r1, r0
	adds r0, r1, #0
	ands r0, r6
	strb r0, [r2]
	lsrs r1, r1, #8
	strb r1, [r2, #1]
	adds r2, #2
	subs r3, #1
	cmp r3, #0
	bge _08001E04
	movs r0, #0
	str r0, [r5, #0xc]
	str r0, [r5, #0x24]
	movs r6, #0
	adds r1, r5, #0
	adds r1, #0xfc
	str r1, [sp, #8]
	adds r2, r5, #0
	adds r2, #0x20
	str r2, [sp, #0xc]
	movs r3, #0xc8
	mov sl, r3
	movs r7, #0x80
	lsls r7, r7, #1
	adds r7, r5, r7
	str r7, [sp]
	str r4, [sp, #4]
_08001E40:
	mov r0, sl
	muls r0, r6, r0
	adds r0, r0, r5
	movs r1, #0x84
	lsls r1, r1, #1
	adds r0, r0, r1
	adds r1, r0, #0
	adds r1, #0x84
	movs r2, #0
	str r2, [r1]
	adds r1, #4
	str r2, [r1]
	adds r0, #0x8c
	movs r1, #0x7f
	str r1, [r0]
	mov r0, sl
	muls r0, r6, r0
	ldr r3, [sp]
	adds r0, r3, r0
	str r2, [r0]
	movs r4, #0
	adds r7, r6, #1
	mov r8, r7
	movs r0, #0xc8
	adds r1, r6, #0
	muls r1, r0, r1
	adds r0, r5, #0
	adds r0, #0xd0
	adds r2, r1, r0
	ldr r3, [sp, #4]
_08001E7C:
	ldrb r0, [r3, #1]
	lsls r1, r0, #8
	ldrb r7, [r3]
	orrs r1, r7
	adds r0, r1, #0
	movs r7, #0xff
	ands r0, r7
	strb r0, [r2]
	lsrs r1, r1, #8
	strb r1, [r2, #1]
	adds r2, #2
	adds r3, #2
	adds r4, #1
	cmp r4, #3
	ble _08001E7C
	mov r2, sl
	muls r2, r6, r2
	adds r0, r5, r2
	mov ip, r0
	mov r4, ip
	adds r4, #0xd1
	ldrb r3, [r4]
	lsls r1, r3, #0x1c
	lsrs r1, r1, #0x1c
	subs r1, #1
	movs r0, #0xf
	ands r1, r0
	movs r7, #0x10
	rsbs r7, r7, #0
	mov sb, r7
	mov r0, sb
	ands r0, r3
	orrs r0, r1
	strb r0, [r4]
	movs r1, #0x82
	lsls r1, r1, #1
	adds r0, r5, r1
	adds r0, r0, r2
	movs r3, #0
	str r3, [r0]
	ldr r7, [sp, #8]
	adds r2, r7, r2
	str r3, [r2]
	mov r0, ip
	adds r0, #0xd6
	ldr r1, _08001F44 @ =0x00001234
	strh r1, [r0]
	adds r0, #2
	adds r2, r1, #0
	strh r2, [r0]
	mov r6, r8
	cmp r6, #3
	ble _08001E40
	movs r3, #0xfc
	lsls r3, r3, #2
	adds r0, r5, r3
	movs r1, #0
	str r1, [r0]
	movs r7, #0xfd
	lsls r7, r7, #2
	adds r0, r5, r7
	str r1, [r0]
	movs r2, #0xfe
	lsls r2, r2, #2
	adds r0, r5, r2
	str r1, [r0]
	str r1, [r5, #0x38]
	str r1, [r5, #0x3c]
	movs r0, #0xf
	ldrh r3, [r5, #0x20]
	ands r0, r3
	ldr r7, _08001F48 @ =0x0000F0B0
	adds r1, r7, #0
	orrs r0, r1
	strh r0, [r5, #0x20]
	mov r0, sb
	ldr r1, [sp, #0xc]
	ldrb r1, [r1]
	ands r0, r1
	ldr r2, [sp, #0xc]
	strb r0, [r2]
	movs r3, #0x80
	lsls r3, r3, #3
	adds r1, r5, r3
	ldrh r0, [r5, #0x20]
	strh r0, [r1]
	ldrh r1, [r1]
	ldr r0, _08001F4C @ =0x0400012A
	strh r1, [r0]
	movs r0, #0
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08001F40: .4byte gUnknown_03000800
_08001F44: .4byte 0x00001234
_08001F48: .4byte 0x0000F0B0
_08001F4C: .4byte 0x0400012A

	thumb_func_start sub_8001F50
sub_8001F50: @ 0x08001F50
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	ldrb r0, [r5, #5]
	cmp r0, #0
	beq _08001FB6
	ldrb r1, [r5, #6]
	cmp r1, #0
	bne _08001F82
	ldr r0, _08001FBC @ =0x04000134
	strh r1, [r0]
	ldr r2, _08001FC0 @ =0x04000128
	movs r1, #0x80
	lsls r1, r1, #6
	adds r0, r1, #0
	strh r0, [r2]
	ldrh r0, [r2]
	ldr r3, _08001FC4 @ =0x00004003
	adds r1, r3, #0
	orrs r0, r1
	strh r0, [r2]
	movs r0, #1
	strb r0, [r5, #6]
_08001F82:
	ldrb r7, [r5, #8]
	cmp r7, #0
	bne _0800204A
	ldr r4, _08001FC0 @ =0x04000128
	ldrh r0, [r4]
	lsrs r0, r0, #3
	movs r1, #1
	movs r2, #1
	mov sb, r2
	ands r0, r2
	cmp r0, #0
	bne _08001FC8
	adds r0, r5, #0
	bl sub_8001D30
	ldr r0, _08001FBC @ =0x04000134
	strh r7, [r0]
	movs r3, #0x80
	lsls r3, r3, #6
	adds r0, r3, #0
	strh r0, [r4]
	ldrh r0, [r4]
	ldr r2, _08001FC4 @ =0x00004003
	adds r1, r2, #0
	orrs r0, r1
	strh r0, [r4]
_08001FB6:
	movs r0, #0
	b _08002106
	.align 2, 0
_08001FBC: .4byte 0x04000134
_08001FC0: .4byte 0x04000128
_08001FC4: .4byte 0x00004003
_08001FC8:
	mov r3, sb
	strb r3, [r5, #8]
	ldrh r4, [r4]
	lsrs r4, r4, #2
	eors r4, r1
	ands r4, r1
	ldr r0, _08002080 @ =0x04000208
	mov r8, r0
	strh r7, [r0]
	ldrh r1, [r0]
	strh r7, [r0]
	ldr r6, _08002084 @ =0x04000200
	ldrh r2, [r6]
	ldr r0, _08002088 @ =0x0000FF7F
	ands r0, r2
	strh r0, [r6]
	mov r2, r8
	strh r1, [r2]
	ldrh r1, [r2]
	strh r7, [r2]
	ldrh r2, [r6]
	ldr r0, _0800208C @ =0x0000FFBF
	ands r0, r2
	strh r0, [r6]
	mov r3, r8
	strh r1, [r3]
	movs r0, #6
	bl sub_8000544
	ldr r1, _08002090 @ =sub_8002830
	movs r0, #7
	bl sub_80005A0
	ldrh r0, [r6]
	movs r1, #0x80
	orrs r0, r1
	strh r0, [r6]
	cmp r4, #0
	beq _0800202C
	ldr r1, _08002094 @ =sub_8002848
	movs r0, #6
	bl sub_80005A0
	ldrh r0, [r6]
	movs r1, #0x40
	orrs r0, r1
	strh r0, [r6]
	ldr r1, _08002098 @ =0x0400010C
	ldr r0, _0800209C @ =0x00C0BBBC
	str r0, [r1]
_0800202C:
	mov r1, sb
	mov r0, r8
	strh r1, [r0]
	movs r2, #0xff
	lsls r2, r2, #2
	adds r1, r5, r2
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r1]
	str r7, [r5, #0x14]
	ldr r3, _080020A0 @ =0x00000404
	adds r0, r5, r3
	str r7, [r0]
	movs r0, #0
	strb r0, [r5, #0x18]
_0800204A:
	ldr r0, _080020A0 @ =0x00000404
	adds r1, r5, r0
	ldr r0, [r1]
	cmp r0, #0xf
	ble _08002060
	movs r0, #0xf
	rsbs r0, r0, #0
	str r0, [r5, #0xc]
	movs r0, #0xe1
	lsls r0, r0, #3
	str r0, [r5, #0x14]
_08002060:
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
	ldrb r0, [r5, #7]
	cmp r0, #0
	bne _080020D2
	movs r1, #0xff
	lsls r1, r1, #2
	adds r0, r5, r1
	ldr r0, [r0]
	cmp r0, #0
	bge _080020A4
	ldr r0, [r5, #0xc]
	subs r0, #1
	b _080020A8
	.align 2, 0
_08002080: .4byte 0x04000208
_08002084: .4byte 0x04000200
_08002088: .4byte 0x0000FF7F
_0800208C: .4byte 0x0000FFBF
_08002090: .4byte sub_8002830
_08002094: .4byte sub_8002848
_08002098: .4byte 0x0400010C
_0800209C: .4byte 0x00C0BBBC
_080020A0: .4byte 0x00000404
_080020A4:
	ldr r0, [r5, #0xc]
	adds r0, #1
_080020A8:
	str r0, [r5, #0xc]
	ldr r1, [r5, #0xc]
	cmp r1, #0xe
	ble _080020BC
	movs r1, #0
	movs r0, #1
	strb r0, [r5, #7]
	str r1, [r5, #0x14]
	str r1, [r5, #0x10]
	b _080020D2
_080020BC:
	movs r0, #0xf
	rsbs r0, r0, #0
	cmp r1, r0
	ble _080020C6
	b _08001FB6
_080020C6:
	adds r0, r5, #0
	bl sub_8001D30
	adds r0, r5, #0
	bl sub_8001DB4
_080020D2:
	ldr r0, [r5, #0x10]
	ldr r1, [r5, #0x14]
	cmp r0, r1
	bge _080020DC
	adds r0, r1, #0
_080020DC:
	str r0, [r5, #0x10]
	ldrb r0, [r5, #0x18]
	adds r1, #1
	cmp r0, #0
	beq _080020E8
	movs r1, #0
_080020E8:
	str r1, [r5, #0x14]
	movs r0, #0
	strb r0, [r5, #0x18]
	cmp r1, #0x1d
	ble _080020FE
	adds r0, r5, #0
	bl sub_8001D30
	adds r0, r5, #0
	bl sub_8001DB4
_080020FE:
	ldr r0, [r5, #0xc]
	adds r0, #1
	str r0, [r5, #0xc]
	movs r0, #1
_08002106:
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8002114
sub_8002114: @ 0x08002114
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x2c
	mov sl, r0
	str r1, [sp, #0x10]
	ldr r0, _08002144 @ =0x00000404
	add r0, sl
	movs r1, #0
	str r1, [r0]
	mov r1, sl
	ldrb r0, [r1, #4]
	cmp r0, #0
	beq _0800214C
	movs r0, #0x80
	lsls r0, r0, #3
	add r0, sl
	ldrh r1, [r0]
	ldr r0, _08002148 @ =0x0400012A
	strh r1, [r0]
	b _080026CE
	.align 2, 0
_08002144: .4byte 0x00000404
_08002148: .4byte 0x0400012A
_0800214C:
	movs r2, #1
	mov r3, sl
	strb r2, [r3, #4]
	ldr r0, _08002250 @ =0x04000128
	ldrh r0, [r0]
	lsls r3, r0, #0x10
	movs r4, #0
	str r4, [sp, #0x18]
	mov r1, sl
	adds r1, #0x31
	ldrb r5, [r1]
	lsls r0, r5, #0x1c
	lsrs r0, r0, #0x1c
	adds r0, #1
	str r0, [sp, #0x1c]
	movs r0, #0xf
	ldr r7, [sp, #0x1c]
	ands r7, r0
	str r7, [sp, #0x1c]
	lsrs r0, r3, #0x16
	ands r0, r2
	str r1, [sp, #0x28]
	cmp r0, #0
	beq _0800217E
	b _080026BC
_0800217E:
	mov r1, sl
	ldrb r0, [r1, #7]
	cmp r0, #0
	bne _0800225C
	lsrs r0, r3, #0x13
	ands r0, r2
	cmp r0, #0
	bne _08002190
	b _080026BC
_08002190:
	movs r5, #0
	movs r2, #0x20
	add r2, sl
	mov r8, r2
	ldr r3, _08002254 @ =0x00000F0B
	mov sb, r3
	ldr r6, _08002258 @ =0x0000FFFF
	ldr r1, [sp, #0x10]
	mov r2, sp
	adds r3, r1, #0
	movs r7, #3
	str r7, [sp, #0x14]
_080021A8:
	ldr r0, [r1]
	str r0, [r2]
	ldrh r7, [r2]
	lsrs r0, r7, #4
	cmp r0, sb
	bne _080021B6
	adds r4, #1
_080021B6:
	ldrh r0, [r3]
	cmp r0, r6
	bne _080021BE
	adds r5, #1
_080021BE:
	adds r1, #2
	adds r2, #4
	adds r3, #2
	ldr r0, [sp, #0x14]
	subs r0, #1
	str r0, [sp, #0x14]
	cmp r0, #0
	bge _080021A8
	movs r2, #1
	adds r3, r5, r4
	cmp r4, #0
	ble _080021F2
	mov r1, sp
	str r4, [sp, #0x14]
_080021DA:
	ldrb r5, [r1]
	lsls r0, r5, #0x1c
	lsrs r0, r0, #0x1c
	cmp r0, r4
	beq _080021E6
	movs r2, #0
_080021E6:
	adds r1, #4
	ldr r7, [sp, #0x14]
	subs r7, #1
	str r7, [sp, #0x14]
	cmp r7, #0
	bne _080021DA
_080021F2:
	cmp r3, #4
	bne _0800222E
	cmp r2, #0
	beq _0800222E
	cmp r4, #1
	ble _0800222E
	mov r0, sl
	str r4, [r0, #0x1c]
	ldr r0, _08002250 @ =0x04000128
	ldrh r1, [r0]
	movs r0, #0x30
	ands r0, r1
	lsrs r0, r0, #4
	movs r2, #0xff
	lsls r2, r2, #2
	add r2, sl
	str r0, [r2]
	movs r3, #0xfe
	lsls r3, r3, #2
	add r3, sl
	movs r1, #1
	rsbs r1, r1, #0
	lsls r1, r4
	mvns r1, r1
	str r1, [r3]
	ldr r2, [r2]
	movs r0, #1
	lsls r0, r2
	bics r1, r0
	str r1, [r3]
_0800222E:
	movs r0, #0xf
	ands r4, r0
	movs r0, #0x10
	rsbs r0, r0, #0
	mov r1, r8
	ldrb r1, [r1]
	ands r0, r1
	orrs r0, r4
	mov r2, r8
	strb r0, [r2]
	movs r1, #0x80
	lsls r1, r1, #3
	add r1, sl
	mov r3, sl
	ldrh r0, [r3, #0x20]
	strh r0, [r1]
	b _080026BC
	.align 2, 0
_08002250: .4byte 0x04000128
_08002254: .4byte 0x00000F0B
_08002258: .4byte 0x0000FFFF
_0800225C:
	movs r4, #0
	str r4, [sp, #0x14]
	mov r5, sl
	ldr r0, [r5, #0x1c]
	ldr r7, [sp, #0x18]
	cmp r7, r0
	blt _0800226C
	b _080024C2
_0800226C:
	movs r0, #0xfd
	lsls r0, r0, #2
	add r0, sl
	str r0, [sp, #0x20]
_08002274:
	movs r0, #0xff
	lsls r0, r0, #2
	add r0, sl
	ldr r0, [r0]
	ldr r1, [sp, #0x14]
	adds r1, #1
	str r1, [sp, #0x24]
	ldr r2, [sp, #0x14]
	cmp r2, r0
	bne _0800228A
	b _080024B2
_0800228A:
	movs r0, #0xc8
	muls r0, r2, r0
	adds r0, #0xd0
	add r0, sl
	mov ip, r0
	ldr r0, [r0, #0x2c]
	lsls r0, r0, #1
	mov r1, ip
	adds r1, #0xa
	adds r1, r1, r0
	lsls r0, r2, #1
	ldr r3, [sp, #0x10]
	adds r0, r0, r3
	ldrh r0, [r0]
	strh r0, [r1]
	mov r4, ip
	ldr r0, [r4, #0x2c]
	adds r0, #1
	movs r6, #0xf
	ands r0, r6
	str r0, [r4, #0x2c]
	cmp r0, #3
	bgt _080022BA
	b _080024B2
_080022BA:
	lsls r0, r0, #1
	adds r0, #2
	add r0, ip
	mov sb, r0
	movs r7, #0
	ldrb r5, [r0, #1]
	lsls r3, r5, #0x1c
	ldrb r2, [r0]
	lsls r4, r2, #0x18
	lsrs r1, r3, #0x1c
	lsrs r0, r4, #0x1c
	cmp r1, r0
	beq _080022DC
	subs r0, #1
	ands r0, r6
	cmp r1, r0
	bne _080022FE
_080022DC:
	lsrs r0, r5, #4
	cmp r0, #4
	bhi _080022FE
	lsls r2, r2, #0x1c
	lsrs r1, r4, #0x1c
	mov r5, sb
	ldrb r5, [r5, #7]
	lsls r0, r5, #8
	mov r3, sb
	ldrb r3, [r3, #6]
	orrs r0, r3
	adds r1, r1, r0
	ands r1, r6
	lsrs r2, r2, #0x1c
	cmp r2, r1
	bne _080022FE
	movs r7, #1
_080022FE:
	ldr r4, [sp, #0x14]
	adds r4, #1
	str r4, [sp, #0x24]
	cmp r7, #0
	bne _0800230A
	b _080024B2
_0800230A:
	mov r5, sb
	ldrb r3, [r5, #1]
	movs r0, #0xf
	mov r7, ip
	ldrb r2, [r7, #1]
	adds r1, r0, #0
	ands r1, r3
	ands r0, r2
	mov r8, r2
	cmp r1, r0
	bne _08002364
	ldrb r0, [r5, #7]
	lsls r3, r0, #8
	ldrb r1, [r5, #6]
	orrs r3, r1
	ldrh r0, [r7, #8]
	mov r2, sb
	adds r2, #1
	movs r4, #4
	ldr r5, _08002360 @ =gStaticData_0816AF10
	mov r8, r5
	movs r6, #0xff
	movs r5, #1
	rsbs r5, r5, #0
_0800233A:
	lsrs r1, r0, #8
	ldrb r7, [r2]
	eors r1, r7
	ands r1, r6
	lsls r1, r1, #1
	add r1, r8
	lsls r0, r0, #8
	ldrh r1, [r1]
	eors r0, r1
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r2, #1
	subs r4, #1
	cmp r4, r5
	bne _0800233A
	cmp r3, r0
	bne _0800235E
	b _08002466
_0800235E:
	b _080024B2
	.align 2, 0
_08002360: .4byte gStaticData_0816AF10
_08002364:
	lsls r0, r3, #0x1c
	lsrs r0, r0, #0x1c
	mov r2, ip
	ldr r1, [r2, #0x30]
	cmp r0, r1
	beq _08002372
	b _080024B2
_08002372:
	mov r3, sb
	ldrb r3, [r3, #7]
	lsls r4, r3, #8
	mov r5, sb
	ldrb r5, [r5, #6]
	orrs r4, r5
	ldrh r0, [r2, #6]
	mov r3, sb
	adds r3, #1
	movs r5, #4
	ldr r2, _08002400 @ =gStaticData_0816AF10
	movs r6, #1
	rsbs r6, r6, #0
_0800238C:
	lsrs r1, r0, #8
	ldrb r7, [r3]
	eors r1, r7
	movs r7, #0xff
	ands r1, r7
	lsls r1, r1, #1
	adds r1, r1, r2
	lsls r0, r0, #8
	ldrh r1, [r1]
	eors r0, r1
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, #1
	subs r5, #1
	cmp r5, r6
	bne _0800238C
	cmp r4, r0
	beq _080023B2
	b _080024B2
_080023B2:
	mov r0, r8
	lsrs r7, r0, #4
	mov r4, ip
	adds r4, #2
	mov r0, ip
	adds r0, #0xc4
	movs r1, #0x80
	subs r1, r1, r7
	ldr r0, [r0]
	cmp r0, r1
	bge _08002404
	subs r3, r7, #1
	movs r1, #1
	rsbs r1, r1, #0
	cmp r3, r1
	beq _08002442
	mov r2, ip
	adds r2, #0xc4
	mov r5, ip
	adds r5, #0xbc
	mov r6, ip
	adds r6, #0x3c
_080023DE:
	ldr r0, [r2]
	adds r0, #1
	str r0, [r2]
	ldr r0, [r5]
	adds r0, #1
	str r0, [r5]
	ldr r0, [r2]
	adds r0, r6, r0
	ldrb r1, [r4]
	strb r1, [r0]
	adds r4, #1
	subs r3, #1
	movs r0, #1
	rsbs r0, r0, #0
	cmp r3, r0
	bne _080023DE
	b _08002442
	.align 2, 0
_08002400: .4byte gStaticData_0816AF10
_08002404:
	subs r3, r7, #1
	movs r1, #1
	rsbs r1, r1, #0
	cmp r3, r1
	beq _08002442
	mov r2, ip
	adds r2, #0xc4
	mov r5, ip
	adds r5, #0xbc
	movs r0, #0x3c
	add r0, ip
	mov r8, r0
_0800241C:
	ldrb r6, [r4]
	adds r4, #1
	ldr r0, [r2]
	movs r1, #0
	cmp r0, #0x7f
	beq _0800242A
	adds r1, r0, #1
_0800242A:
	str r1, [r2]
	ldr r0, [r5]
	adds r0, #1
	str r0, [r5]
	ldr r0, [r2]
	add r0, r8
	strb r6, [r0]
	subs r3, #1
	movs r1, #1
	rsbs r1, r1, #0
	cmp r3, r1
	bne _0800241C
_08002442:
	mov r2, ip
	ldr r0, [r2, #0x34]
	adds r0, r0, r7
	str r0, [r2, #0x34]
	ldrh r0, [r2, #6]
	strh r0, [r2, #8]
	ldr r0, [r2, #0x30]
	adds r0, #1
	movs r1, #0xf
	ands r0, r1
	str r0, [r2, #0x30]
	movs r1, #1
	ldr r3, [sp, #0x14]
	lsls r1, r3
	ldr r4, [sp, #0x20]
	ldr r0, [r4]
	orrs r0, r1
	str r0, [r4]
_08002466:
	movs r5, #0xff
	mov r3, ip
	mov r2, sb
	movs r4, #3
_0800246E:
	ldrb r7, [r2, #1]
	lsls r1, r7, #8
	ldrb r0, [r2]
	orrs r1, r0
	adds r0, r1, #0
	ands r0, r5
	strb r0, [r3]
	lsrs r1, r1, #8
	strb r1, [r3, #1]
	adds r3, #2
	adds r2, #2
	subs r4, #1
	cmp r4, #0
	bge _0800246E
	mov r1, ip
	ldrb r1, [r1]
	lsrs r0, r1, #4
	ldr r2, [sp, #0x1c]
	cmp r0, r2
	bne _080024A8
	movs r2, #0xfc
	lsls r2, r2, #2
	add r2, sl
	movs r1, #1
	ldr r3, [sp, #0x14]
	lsls r1, r3
	ldr r0, [r2]
	orrs r0, r1
	str r0, [r2]
_080024A8:
	movs r0, #0
	mov r4, ip
	str r0, [r4, #0x2c]
	movs r5, #1
	str r5, [sp, #0x18]
_080024B2:
	ldr r7, [sp, #0x24]
	str r7, [sp, #0x14]
	mov r1, sl
	ldr r0, [r1, #0x1c]
	adds r2, r7, #0
	cmp r2, r0
	bge _080024C2
	b _08002274
_080024C2:
	ldr r3, [sp, #0x18]
	cmp r3, #0
	bne _080024CA
	b _08002668
_080024CA:
	movs r4, #0
	str r4, [sp, #0x18]
	movs r2, #0xfc
	lsls r2, r2, #2
	add r2, sl
	movs r0, #0xfe
	lsls r0, r0, #2
	add r0, sl
	ldr r1, [r2]
	ldr r0, [r0]
	cmp r1, r0
	beq _080024E4
	b _08002606
_080024E4:
	str r4, [r2]
	mov r5, sl
	adds r5, #0x30
	mov r7, sl
	adds r7, #0x40
	movs r0, #0xc4
	add r0, sl
	mov r8, r0
	movs r1, #0x32
	add r1, sl
	mov sb, r1
	movs r2, #0xc8
	add r2, sl
	mov ip, r2
	mov r3, sl
	adds r3, #0x28
	adds r2, r5, #0
	movs r6, #0xff
	movs r4, #3
_0800250A:
	ldrb r0, [r2, #1]
	lsls r1, r0, #8
	ldrb r0, [r2]
	orrs r1, r0
	adds r0, r1, #0
	ands r0, r6
	strb r0, [r3]
	lsrs r1, r1, #8
	strb r1, [r3, #1]
	adds r3, #2
	adds r2, #2
	subs r4, #1
	cmp r4, #0
	bge _0800250A
	adds r4, r7, #0
	mov r1, r8
	ldr r6, [r1]
	cmp r6, #4
	ble _08002532
	movs r6, #4
_08002532:
	lsls r1, r6, #4
	movs r0, #0xf
	ldrb r2, [r5, #1]
	ands r0, r2
	orrs r0, r1
	strb r0, [r5, #1]
	mov r3, sb
	movs r0, #0x80
	subs r0, r0, r6
	mov r5, ip
	ldr r1, [r5]
	cmp r1, r0
	bge _0800257E
	subs r2, r6, #1
	movs r0, #1
	rsbs r0, r0, #0
	cmp r2, r0
	beq _080025B4
	adds r1, r4, #0
	adds r1, #0x88
	adds r5, r4, #4
	adds r4, #0x84
	adds r7, r0, #0
_08002560:
	ldr r0, [r1]
	adds r0, r5, r0
	ldrb r0, [r0]
	strb r0, [r3]
	adds r3, #1
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
	ldr r0, [r4]
	subs r0, #1
	str r0, [r4]
	subs r2, #1
	cmp r2, r7
	bne _08002560
	b _080025B4
_0800257E:
	subs r2, r6, #1
	movs r0, #1
	rsbs r0, r0, #0
	cmp r2, r0
	beq _080025B4
	adds r5, r7, #0
	adds r5, #0x88
	adds r4, r7, #0
	adds r4, #0x84
	adds r7, #4
	mov r8, r0
_08002594:
	ldr r1, [r5]
	movs r0, #0
	cmp r1, #0x7f
	beq _0800259E
	adds r0, r1, #1
_0800259E:
	str r0, [r5]
	ldr r0, [r4]
	subs r0, #1
	str r0, [r4]
	adds r0, r7, r1
	ldrb r0, [r0]
	strb r0, [r3]
	adds r3, #1
	subs r2, #1
	cmp r2, r8
	bne _08002594
_080025B4:
	mov r7, sl
	ldr r0, [r7, #0x24]
	adds r0, r0, r6
	str r0, [r7, #0x24]
	movs r0, #0x10
	rsbs r0, r0, #0
	ldr r1, [sp, #0x28]
	ldrb r1, [r1]
	ands r0, r1
	ldr r2, [sp, #0x1c]
	orrs r0, r2
	ldr r3, [sp, #0x28]
	strb r0, [r3]
	ldrh r1, [r7, #0x36]
	ldr r2, [sp, #0x28]
	movs r3, #4
	ldr r6, _08002684 @ =gStaticData_0816AF10
	movs r5, #0xff
	movs r4, #1
	rsbs r4, r4, #0
_080025DC:
	lsrs r0, r1, #8
	ldrb r7, [r2]
	eors r0, r7
	ands r0, r5
	lsls r0, r0, #1
	adds r0, r0, r6
	lsls r1, r1, #8
	ldrh r0, [r0]
	eors r1, r0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	adds r2, #1
	subs r3, #1
	cmp r3, r4
	bne _080025DC
	movs r0, #0
	mov r2, sl
	strh r1, [r2, #0x36]
	str r0, [r2, #0x3c]
	movs r3, #1
	str r3, [sp, #0x18]
_08002606:
	movs r2, #0xfd
	lsls r2, r2, #2
	add r2, sl
	movs r0, #0xfe
	lsls r0, r0, #2
	add r0, sl
	ldr r1, [r2]
	ldr r0, [r0]
	cmp r1, r0
	bne _0800263A
	movs r0, #0
	str r0, [r2]
	mov r3, sl
	adds r3, #0x30
	ldrb r2, [r3]
	lsrs r1, r2, #4
	adds r1, #1
	movs r0, #0xf
	ands r1, r0
	lsls r1, r1, #4
	movs r0, #0xf
	ands r0, r2
	orrs r0, r1
	strb r0, [r3]
	movs r4, #1
	str r4, [sp, #0x18]
_0800263A:
	ldr r5, [sp, #0x18]
	cmp r5, #0
	beq _08002668
	movs r0, #0
	mov r7, sl
	str r0, [r7, #0x38]
	movs r0, #1
	strb r0, [r7, #0x18]
	mov r2, sl
	adds r2, #0x30
	ldrb r3, [r2]
	lsrs r1, r3, #4
	ldrb r4, [r2, #7]
	lsls r0, r4, #8
	ldrb r5, [r2, #6]
	orrs r0, r5
	adds r1, r1, r0
	movs r0, #0xf
	ands r1, r0
	subs r0, #0x1f
	ands r0, r3
	orrs r0, r1
	strb r0, [r2]
_08002668:
	mov r7, sl
	ldr r0, [r7, #0x3c]
	movs r1, #3
	ands r0, r1
	cmp r0, #3
	bne _08002688
	ldr r0, [r7, #0x38]
	lsls r0, r0, #1
	add r0, sl
	adds r1, r0, #0
	adds r1, #0x28
	adds r0, #0x29
	b _08002696
	.align 2, 0
_08002684: .4byte gStaticData_0816AF10
_08002688:
	mov r1, sl
	ldr r0, [r1, #0x38]
	lsls r0, r0, #1
	add r0, sl
	adds r1, r0, #0
	adds r1, #0x30
	adds r0, #0x31
_08002696:
	ldrb r0, [r0]
	lsls r0, r0, #8
	ldrb r1, [r1]
	orrs r0, r1
	movs r1, #0x80
	lsls r1, r1, #3
	add r1, sl
	strh r0, [r1]
	mov r2, sl
	ldr r0, [r2, #0x38]
	adds r0, #1
	str r0, [r2, #0x38]
	cmp r0, #4
	bne _080026BC
	movs r0, #0
	str r0, [r2, #0x38]
	ldr r0, [r2, #0x3c]
	adds r0, #1
	str r0, [r2, #0x3c]
_080026BC:
	movs r0, #0x80
	lsls r0, r0, #3
	add r0, sl
	ldrh r1, [r0]
	ldr r0, _080026E0 @ =0x0400012A
	strh r1, [r0]
	movs r0, #0
	mov r3, sl
	strb r0, [r3, #4]
_080026CE:
	add sp, #0x2c
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080026E0: .4byte 0x0400012A

	thumb_func_start sub_80026E4
sub_80026E4: @ 0x080026E4
	push {r4, r5, r6, lr}
	lsls r4, r1, #0x18
	lsrs r4, r4, #0x18
	ldr r6, _0800274C @ =0x04000208
	movs r3, #0
	strh r3, [r6]
	ldrh r1, [r6]
	strh r3, [r6]
	ldr r5, _08002750 @ =0x04000200
	ldrh r2, [r5]
	ldr r0, _08002754 @ =0x0000FF7F
	ands r0, r2
	strh r0, [r5]
	strh r1, [r6]
	ldrh r1, [r6]
	strh r3, [r6]
	ldrh r2, [r5]
	ldr r0, _08002758 @ =0x0000FFBF
	ands r0, r2
	strh r0, [r5]
	strh r1, [r6]
	movs r0, #6
	bl sub_8000544
	ldr r1, _0800275C @ =sub_8002830
	movs r0, #7
	bl sub_80005A0
	ldrh r0, [r5]
	movs r1, #0x80
	orrs r0, r1
	strh r0, [r5]
	cmp r4, #0
	beq _0800273E
	ldr r1, _08002760 @ =sub_8002848
	movs r0, #6
	bl sub_80005A0
	ldrh r0, [r5]
	movs r1, #0x40
	orrs r0, r1
	strh r0, [r5]
	ldr r1, _08002764 @ =0x0400010C
	ldr r0, _08002768 @ =0x00C0BBBC
	str r0, [r1]
_0800273E:
	movs r0, #1
	strh r0, [r6]
	movs r0, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0800274C: .4byte 0x04000208
_08002750: .4byte 0x04000200
_08002754: .4byte 0x0000FF7F
_08002758: .4byte 0x0000FFBF
_0800275C: .4byte sub_8002830
_08002760: .4byte sub_8002848
_08002764: .4byte 0x0400010C
_08002768: .4byte 0x00C0BBBC

	thumb_func_start sub_800276C
sub_800276C: @ 0x0800276C
	ldr r1, _0800278C @ =0x04000134
	movs r0, #0
	strh r0, [r1]
	ldr r2, _08002790 @ =0x04000128
	movs r1, #0x80
	lsls r1, r1, #6
	adds r0, r1, #0
	strh r0, [r2]
	ldrh r0, [r2]
	ldr r3, _08002794 @ =0x00004003
	adds r1, r3, #0
	orrs r0, r1
	strh r0, [r2]
	movs r0, #0
	bx lr
	.align 2, 0
_0800278C: .4byte 0x04000134
_08002790: .4byte 0x04000128
_08002794: .4byte 0x00004003

	thumb_func_start sub_8002798
sub_8002798: @ 0x08002798
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8001D30
	adds r0, r4, #0
	bl sub_8001DB4
	movs r0, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_80027B0
sub_80027B0: @ 0x080027B0
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	bl sub_8002798
	adds r1, r4, #0
	adds r1, #0xd0
	cmp r1, #0
	beq _080027D2
	movs r2, #0xfc
	lsls r2, r2, #2
	adds r0, r4, r2
	cmp r1, r0
	beq _080027D2
_080027CC:
	subs r0, #0xc8
	cmp r1, r0
	bne _080027CC
_080027D2:
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _080027E0
	adds r0, r4, #0
	bl sub_80016D0
_080027E0:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80027E8
sub_80027E8: @ 0x080027E8
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r0, #0xc4
	movs r1, #0
	str r1, [r0]
	adds r0, #4
	str r1, [r0]
	adds r1, r4, #0
	adds r1, #0xcc
	movs r0, #0x7f
	str r0, [r1]
	movs r1, #3
	movs r2, #0
	movs r5, #0x7f
	movs r3, #1
	rsbs r3, r3, #0
	movs r6, #0xc6
	lsls r6, r6, #1
	adds r0, r4, r6
_0800280E:
	str r2, [r0]
	str r2, [r0, #4]
	str r5, [r0, #8]
	adds r0, #0xc8
	subs r1, #1
	cmp r1, r3
	bne _0800280E
	adds r0, r4, #0
	bl sub_8002798
	movs r0, #0
	strb r0, [r4, #5]
	adds r0, r4, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8002830
sub_8002830: @ 0x08002830
	push {lr}
	ldr r0, _08002840 @ =gUnknown_03000804
	ldr r0, [r0]
	ldr r1, _08002844 @ =0x04000120
	bl sub_8002114
	pop {r0}
	bx r0
	.align 2, 0
_08002840: .4byte gUnknown_03000804
_08002844: .4byte 0x04000120

	thumb_func_start sub_8002848
sub_8002848: @ 0x08002848
	ldr r3, _08002860 @ =0x0400010E
	movs r0, #0
	strh r0, [r3]
	ldr r2, _08002864 @ =0x04000128
	ldrh r0, [r2]
	movs r1, #0x80
	orrs r0, r1
	strh r0, [r2]
	movs r0, #0xc0
	strh r0, [r3]
	bx lr
	.align 2, 0
_08002860: .4byte 0x0400010E
_08002864: .4byte 0x04000128

	thumb_func_start sub_8002868
sub_8002868: @ 0x08002868
	push {r4, r5, r6, r7, lr}
	ldr r4, _080028A0 @ =0xFFFFFE00
	add sp, r4
	adds r6, r0, #0
	adds r7, r1, #0
	ldr r4, _080028A4 @ =gUnknown_03000808
	ldrb r0, [r4]
	cmp r0, #0
	beq _0800288A
	movs r0, #4
	bl sub_803A968
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	cmp r0, #0
	bne _0800291C
	strb r0, [r4]
_0800288A:
	ldr r1, _080028A8 @ =0x04000208
	movs r0, #0
	strh r0, [r1]
	ldr r1, _080028AC @ =gUnknown_030009FC
	movs r0, #2
	bl sub_803A9D0
	mov r5, sp
	movs r4, #0
	b _080028C4
	.align 2, 0
_080028A0: .4byte 0xFFFFFE00
_080028A4: .4byte gUnknown_03000808
_080028A8: .4byte 0x04000208
_080028AC: .4byte gUnknown_030009FC
_080028B0:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	adds r1, r5, #0
	bl sub_803AB54
	lsls r0, r0, #0x10
	cmp r0, #0
	bne _08002904
	adds r5, #8
	adds r4, #1
_080028C4:
	ldr r0, _080028F4 @ =gUnknown_03001634
	ldr r0, [r0]
	ldrh r0, [r0, #4]
	cmp r4, r0
	blt _080028B0
	ldr r3, _080028F8 @ =0x04000208
	ldrh r2, [r3]
	movs r0, #0
	strh r0, [r3]
	ldr r4, _080028FC @ =0x04000200
	ldrh r1, [r4]
	ldr r0, _08002900 @ =0x0000FFDF
	ands r0, r1
	strh r0, [r4]
	strh r2, [r3]
	movs r0, #1
	strh r0, [r3]
	adds r0, r6, #0
	mov r1, sp
	adds r2, r7, #0
	bl sub_800014C
	movs r0, #0
	b _08002920
	.align 2, 0
_080028F4: .4byte gUnknown_03001634
_080028F8: .4byte 0x04000208
_080028FC: .4byte 0x04000200
_08002900: .4byte 0x0000FFDF
_08002904:
	ldr r3, _0800292C @ =0x04000208
	ldrh r2, [r3]
	movs r0, #0
	strh r0, [r3]
	ldr r4, _08002930 @ =0x04000200
	ldrh r1, [r4]
	ldr r0, _08002934 @ =0x0000FFDF
	ands r0, r1
	strh r0, [r4]
	strh r2, [r3]
	movs r0, #1
	strh r0, [r3]
_0800291C:
	movs r0, #1
	rsbs r0, r0, #0
_08002920:
	movs r3, #0x80
	lsls r3, r3, #2
	add sp, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0800292C: .4byte 0x04000208
_08002930: .4byte 0x04000200
_08002934: .4byte 0x0000FFDF

	thumb_func_start sub_8002938
sub_8002938: @ 0x08002938
	push {r4, r5, r6, lr}
	ldr r4, _08002978 @ =0xFFFFFE00
	add sp, r4
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r6, _0800297C @ =gUnknown_03000808
	ldrb r0, [r6]
	cmp r0, #0
	beq _0800295A
	movs r0, #4
	bl sub_803A968
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	cmp r0, #0
	bne _080029EC
	strb r0, [r6]
_0800295A:
	mov r0, sp
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_800014C
	ldr r1, _08002980 @ =0x04000208
	movs r0, #0
	strh r0, [r1]
	ldr r1, _08002984 @ =gUnknown_030009FC
	movs r0, #2
	bl sub_803A9D0
	mov r5, sp
	movs r4, #0
	b _0800299C
	.align 2, 0
_08002978: .4byte 0xFFFFFE00
_0800297C: .4byte gUnknown_03000808
_08002980: .4byte 0x04000208
_08002984: .4byte gUnknown_030009FC
_08002988:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	adds r1, r5, #0
	bl sub_803AD38
	lsls r0, r0, #0x10
	cmp r0, #0
	bne _080029D4
	adds r5, #8
	adds r4, #1
_0800299C:
	ldr r0, _080029C4 @ =gUnknown_03001634
	ldr r0, [r0]
	ldrh r0, [r0, #4]
	cmp r4, r0
	blt _08002988
	ldr r3, _080029C8 @ =0x04000208
	ldrh r2, [r3]
	movs r0, #0
	strh r0, [r3]
	ldr r4, _080029CC @ =0x04000200
	ldrh r1, [r4]
	ldr r0, _080029D0 @ =0x0000FFDF
	ands r0, r1
	strh r0, [r4]
	strh r2, [r3]
	movs r0, #1
	strh r0, [r3]
	movs r0, #0
	b _080029F0
	.align 2, 0
_080029C4: .4byte gUnknown_03001634
_080029C8: .4byte 0x04000208
_080029CC: .4byte 0x04000200
_080029D0: .4byte 0x0000FFDF
_080029D4:
	ldr r3, _080029FC @ =0x04000208
	ldrh r2, [r3]
	movs r0, #0
	strh r0, [r3]
	ldr r4, _08002A00 @ =0x04000200
	ldrh r1, [r4]
	ldr r0, _08002A04 @ =0x0000FFDF
	ands r0, r1
	strh r0, [r4]
	strh r2, [r3]
	movs r0, #1
	strh r0, [r3]
_080029EC:
	movs r0, #1
	rsbs r0, r0, #0
_080029F0:
	movs r3, #0x80
	lsls r3, r3, #2
	add sp, r3
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_080029FC: .4byte 0x04000208
_08002A00: .4byte 0x04000200
_08002A04: .4byte 0x0000FFDF

	thumb_func_start sub_8002A08
sub_8002A08: @ 0x08002A08
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	ldr r4, _08002A60 @ =gUnknown_030012BC
	ldr r1, [r4]
	movs r2, #0
	ldr r0, [r1, #4]
	cmp r0, #1
	bne _08002A1E
	movs r2, #1
_08002A1E:
	adds r7, r2, #0
	adds r0, r1, #0
	bl sub_8001AB8
	mov r8, r0
	cmp r7, #0
	beq _08002A32
	ldr r0, [r4]
	bl sub_8001BD4
_08002A32:
	movs r5, #0
_08002A34:
	adds r0, r6, #0
	movs r1, #0x80
	lsls r1, r1, #2
	bl sub_8002868
	adds r4, r0, #0
	adds r5, #1
	cmp r5, #2
	bgt _08002A4A
	cmp r4, #0
	bne _08002A34
_08002A4A:
	cmp r7, #0
	beq _08002A58
	ldr r0, _08002A60 @ =gUnknown_030012BC
	ldr r0, [r0]
	mov r1, r8
	bl sub_8001B54
_08002A58:
	cmp r4, #0
	beq _08002A64
	movs r0, #4
	b _08002A9A
	.align 2, 0
_08002A60: .4byte gUnknown_030012BC
_08002A64:
	movs r1, #0xfc
	lsls r1, r1, #1
	adds r0, r6, r1
	ldrb r0, [r0]
	cmp r0, #0x43
	beq _08002A74
	movs r0, #2
	b _08002A9A
_08002A74:
	ldr r1, _08002A84 @ =0x000001F9
	adds r0, r6, r1
	ldrb r0, [r0]
	cmp r0, #0x12
	beq _08002A88
	movs r0, #1
	b _08002A9A
	.align 2, 0
_08002A84: .4byte 0x000001F9
_08002A88:
	adds r0, r6, #0
	bl sub_8002B44
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08002A98
	movs r0, #0
	b _08002A9A
_08002A98:
	movs r0, #3
_08002A9A:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_8002AA4
sub_8002AA4: @ 0x08002AA4
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	adds r5, r0, #0
	adds r3, r5, #0
	movs r2, #0
	movs r1, #0x7e
_08002AB6:
	ldm r3!, {r0}
	adds r2, r2, r0
	subs r1, #1
	cmp r1, #0
	bge _08002AB6
	movs r1, #0
	movs r3, #0xfe
	lsls r3, r3, #1
	adds r0, r5, r3
	ldr r0, [r0]
	cmp r2, r0
	bne _08002AD0
	movs r1, #1
_08002AD0:
	cmp r1, #0
	bne _08002B26
	mov r0, sp
	strh r1, [r0]
	ldr r0, _08002B34 @ =0x040000D4
	mov r1, sp
	str r1, [r0]
	str r5, [r0, #4]
	ldr r1, _08002B38 @ =0x81000100
	str r1, [r0, #8]
	ldr r0, [r0, #8]
	movs r4, #0
	movs r2, #0xfc
	lsls r2, r2, #1
	adds r6, r5, r2
	ldr r3, _08002B3C @ =0x000001F9
	adds r3, r3, r5
	mov sb, r3
	movs r0, #0xfd
	lsls r0, r0, #1
	adds r7, r5, r0
	ldr r1, _08002B40 @ =0x000001FB
	adds r1, r1, r5
	mov r8, r1
_08002B00:
	adds r0, r5, #0
	adds r1, r4, #0
	bl sub_8002C6C
	adds r4, #1
	cmp r4, #3
	ble _08002B00
	movs r0, #0
	movs r1, #0x43
	strb r1, [r6]
	movs r1, #0x12
	mov r2, sb
	strb r1, [r2]
	strb r0, [r7]
	mov r3, r8
	strb r0, [r3]
	adds r0, r5, #0
	bl sub_8002B70
_08002B26:
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08002B34: .4byte 0x040000D4
_08002B38: .4byte 0x81000100
_08002B3C: .4byte 0x000001F9
_08002B40: .4byte 0x000001FB

	thumb_func_start sub_8002B44
sub_8002B44: @ 0x08002B44
	push {r4, lr}
	adds r4, r0, #0
	adds r3, r4, #0
	movs r2, #0
	movs r1, #0x7e
_08002B4E:
	ldm r3!, {r0}
	adds r2, r2, r0
	subs r1, #1
	cmp r1, #0
	bge _08002B4E
	movs r1, #0
	movs r3, #0xfe
	lsls r3, r3, #1
	adds r0, r4, r3
	ldr r0, [r0]
	cmp r2, r0
	bne _08002B68
	movs r1, #1
_08002B68:
	adds r0, r1, #0
	pop {r4}
	pop {r1}
	bx r1

	thumb_func_start sub_8002B70
sub_8002B70: @ 0x08002B70
	push {r4, lr}
	adds r4, r0, #0
	adds r3, r4, #0
	movs r2, #0
	movs r1, #0x7e
_08002B7A:
	ldm r3!, {r0}
	adds r2, r2, r0
	subs r1, #1
	cmp r1, #0
	bge _08002B7A
	movs r1, #0xfe
	lsls r1, r1, #1
	adds r0, r4, r1
	str r2, [r0]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8002B94
sub_8002B94: @ 0x08002B94
	ldr r1, _08002BA0 @ =0x000001F9
	adds r0, r0, r1
	ldrb r0, [r0]
	lsrs r0, r0, #4
	bx lr
	.align 2, 0
_08002BA0: .4byte 0x000001F9

	thumb_func_start sub_8002BA4
sub_8002BA4: @ 0x08002BA4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	ldr r4, _08002C04 @ =gUnknown_030012BC
	ldr r1, [r4]
	movs r2, #0
	ldr r0, [r1, #4]
	cmp r0, #1
	bne _08002BBA
	movs r2, #1
_08002BBA:
	adds r7, r2, #0
	adds r0, r1, #0
	bl sub_8001AB8
	mov r8, r0
	adds r0, r6, #0
	bl sub_8002B70
	cmp r7, #0
	beq _08002BD4
	ldr r0, [r4]
	bl sub_8001BD4
_08002BD4:
	movs r5, #0
_08002BD6:
	adds r0, r6, #0
	movs r1, #0x80
	lsls r1, r1, #2
	bl sub_8002938
	adds r4, r0, #0
	adds r5, #1
	cmp r5, #4
	bgt _08002BEC
	cmp r4, #0
	bne _08002BD6
_08002BEC:
	cmp r7, #0
	beq _08002BFA
	ldr r0, _08002C04 @ =gUnknown_030012BC
	ldr r0, [r0]
	mov r1, r8
	bl sub_8001B54
_08002BFA:
	cmp r4, #0
	bne _08002C08
	movs r0, #0
	b _08002C0A
	.align 2, 0
_08002C04: .4byte gUnknown_030012BC
_08002C08:
	movs r0, #4
_08002C0A:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_8002C14
sub_8002C14: @ 0x08002C14
	push {r4, lr}
	adds r4, r0, #0
	adds r3, r1, #0
	movs r1, #0xfa
	lsls r1, r1, #1
	adds r0, r4, r1
	adds r0, r0, r3
	ldrb r0, [r0]
	cmp r0, #0
	bne _08002C38
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #4
	adds r1, r1, r4
	adds r0, r2, #0
	movs r2, #0x70
	bl sub_800014C
_08002C38:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8002C40
sub_8002C40: @ 0x08002C40
	push {r4, lr}
	adds r4, r0, #0
	movs r3, #0xfa
	lsls r3, r3, #1
	adds r0, r4, r3
	adds r0, r0, r1
	movs r3, #0
	strb r3, [r0]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r0, r0, #4
	adds r0, r0, r4
	adds r1, r2, #0
	movs r2, #0x70
	bl sub_800014C
	adds r0, r4, #0
	bl sub_8002B70
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8002C6C
sub_8002C6C: @ 0x08002C6C
	push {lr}
	movs r3, #0xfa
	lsls r3, r3, #1
	adds r2, r0, r3
	adds r2, r2, r1
	movs r1, #1
	strb r1, [r2]
	bl sub_8002B70
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8002C84
sub_8002C84: @ 0x08002C84
	push {r4, r5, lr}
	sub sp, #4
	adds r5, r0, #0
	mov r1, sp
	movs r0, #0
	strh r0, [r1]
	ldr r0, _08002CDC @ =0x040000D4
	str r1, [r0]
	str r5, [r0, #4]
	ldr r1, _08002CE0 @ =0x81000100
	str r1, [r0, #8]
	ldr r0, [r0, #8]
	movs r4, #0
_08002C9E:
	adds r0, r5, #0
	adds r1, r4, #0
	bl sub_8002C6C
	adds r4, #1
	cmp r4, #3
	ble _08002C9E
	movs r0, #0xfc
	lsls r0, r0, #1
	adds r1, r5, r0
	movs r2, #0
	movs r0, #0x43
	strb r0, [r1]
	ldr r0, _08002CE4 @ =0x000001F9
	adds r1, r5, r0
	movs r0, #0x12
	strb r0, [r1]
	movs r1, #0xfd
	lsls r1, r1, #1
	adds r0, r5, r1
	strb r2, [r0]
	adds r1, #1
	adds r0, r5, r1
	strb r2, [r0]
	adds r0, r5, #0
	bl sub_8002B70
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08002CDC: .4byte 0x040000D4
_08002CE0: .4byte 0x81000100
_08002CE4: .4byte 0x000001F9

	thumb_func_start sub_8002CE8
sub_8002CE8: @ 0x08002CE8
	movs r2, #0xfa
	lsls r2, r2, #1
	adds r0, r0, r2
	adds r0, r0, r1
	ldrb r0, [r0]
	bx lr

	thumb_func_start sub_8002CF4
sub_8002CF4: @ 0x08002CF4
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	movs r2, #0xfd
	lsls r2, r2, #1
	adds r0, r0, r2
	ldrb r0, [r0]
	ands r1, r0
	adds r0, r1, #0
	cmp r1, #0
	beq _08002D0A
	movs r0, #1
_08002D0A:
	bx lr

	thumb_func_start sub_8002D0C
sub_8002D0C: @ 0x08002D0C
	push {lr}
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	movs r3, #0xfd
	lsls r3, r3, #1
	adds r2, r0, r3
	ldrb r3, [r2]
	bics r3, r1
	adds r1, r3, #0
	strb r1, [r2]
	bl sub_8002B70
	pop {r0}
	bx r0

	thumb_func_start sub_8002D28
sub_8002D28: @ 0x08002D28
	push {lr}
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	movs r3, #0xfd
	lsls r3, r3, #1
	adds r2, r0, r3
	ldrb r3, [r2]
	orrs r1, r3
	strb r1, [r2]
	bl sub_8002B70
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8002D44
sub_8002D44: @ 0x08002D44
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r0
	ldr r1, [r0]
	cmp r1, #0
	beq _08002DFA
	ldr r0, _08002DB0 @ =gUnknown_03000804
	ldr r0, [r0]
	mov ip, r0
	adds r0, #0xc4
	ldr r0, [r0]
	cmp r0, #0
	bne _08002E10
	adds r7, r1, #0
	cmp r7, #0x60
	ble _08002D6A
	movs r7, #0x60
_08002D6A:
	mov r0, r8
	ldr r4, [r0, #0xc]
	mov r3, ip
	adds r3, #0xcc
	movs r0, #0x80
	subs r0, r0, r7
	ldr r1, [r3]
	cmp r1, r0
	bge _08002DB4
	subs r2, r7, #1
	movs r0, #1
	rsbs r0, r0, #0
	cmp r2, r0
	beq _08002DEC
	mov r5, ip
	adds r5, #0xc4
	mov r6, ip
	adds r6, #0x44
	mov ip, r0
_08002D90:
	ldr r0, [r3]
	adds r0, #1
	str r0, [r3]
	ldr r0, [r5]
	adds r0, #1
	str r0, [r5]
	ldr r0, [r3]
	adds r0, r6, r0
	ldrb r1, [r4]
	strb r1, [r0]
	adds r4, #1
	subs r2, #1
	cmp r2, ip
	bne _08002D90
	b _08002DEC
	.align 2, 0
_08002DB0: .4byte gUnknown_03000804
_08002DB4:
	subs r2, r7, #1
	movs r0, #1
	rsbs r0, r0, #0
	cmp r2, r0
	beq _08002DEC
	adds r1, r3, #0
	mov r6, ip
	adds r6, #0xc4
	movs r3, #0x44
	add ip, r3
	mov sb, r0
_08002DCA:
	ldrb r5, [r4]
	adds r4, #1
	ldr r0, [r1]
	movs r3, #0
	cmp r0, #0x7f
	beq _08002DD8
	adds r3, r0, #1
_08002DD8:
	str r3, [r1]
	ldr r0, [r6]
	adds r0, #1
	str r0, [r6]
	ldr r0, [r1]
	add r0, ip
	strb r5, [r0]
	subs r2, #1
	cmp r2, sb
	bne _08002DCA
_08002DEC:
	mov r1, r8
	ldr r0, [r1, #0xc]
	adds r0, r0, r7
	str r0, [r1, #0xc]
	ldr r0, [r1]
	subs r0, r0, r7
	b _08002E0E
_08002DFA:
	ldr r0, _08002E1C @ =gUnknown_03000804
	ldr r0, [r0]
	adds r0, #0xc4
	ldr r0, [r0]
	cmp r0, #0
	bne _08002E10
	movs r1, #0x85
	lsls r1, r1, #2
	add r1, r8
	movs r0, #1
_08002E0E:
	str r0, [r1]
_08002E10:
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08002E1C: .4byte gUnknown_03000804

	thumb_func_start sub_8002E20
sub_8002E20: @ 0x08002E20
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov ip, r0
	ldr r0, _08002E90 @ =gUnknown_03000804
	ldr r3, [r0]
	movs r2, #0xc8
	adds r0, r1, #0
	muls r0, r2, r0
	adds r0, r0, r3
	movs r4, #0xc6
	lsls r4, r4, #1
	adds r0, r0, r4
	ldr r6, [r0]
	cmp r6, #0
	beq _08002EDC
	movs r0, #0x84
	lsls r0, r0, #2
	add r0, ip
	muls r2, r1, r2
	adds r2, r2, r3
	movs r1, #0x84
	lsls r1, r1, #1
	adds r2, r2, r1
	ldr r4, [r0]
	adds r5, r2, #0
	adds r5, #0x88
	movs r0, #0x80
	subs r0, r0, r6
	ldr r1, [r5]
	cmp r1, r0
	bge _08002E94
	subs r3, r6, #1
	movs r0, #1
	rsbs r0, r0, #0
	cmp r3, r0
	beq _08002EC6
	adds r1, r5, #0
	adds r5, r2, #4
	adds r2, #0x84
	adds r7, r0, #0
_08002E72:
	ldr r0, [r1]
	adds r0, r5, r0
	ldrb r0, [r0]
	strb r0, [r4]
	adds r4, #1
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
	ldr r0, [r2]
	subs r0, #1
	str r0, [r2]
	subs r3, #1
	cmp r3, r7
	bne _08002E72
	b _08002EC6
	.align 2, 0
_08002E90: .4byte gUnknown_03000804
_08002E94:
	subs r3, r6, #1
	movs r0, #1
	rsbs r0, r0, #0
	cmp r3, r0
	beq _08002EC6
	adds r1, r2, #0
	adds r1, #0x84
	adds r7, r2, #4
	mov r8, r0
_08002EA6:
	ldr r2, [r5]
	movs r0, #0
	cmp r2, #0x7f
	beq _08002EB0
	adds r0, r2, #1
_08002EB0:
	str r0, [r5]
	ldr r0, [r1]
	subs r0, #1
	str r0, [r1]
	adds r0, r7, r2
	ldrb r0, [r0]
	strb r0, [r4]
	adds r4, #1
	subs r3, #1
	cmp r3, r8
	bne _08002EA6
_08002EC6:
	movs r0, #0x84
	lsls r0, r0, #2
	add r0, ip
	ldr r1, [r0]
	adds r1, r1, r6
	str r1, [r0]
	mov r4, ip
	ldr r0, [r4, #4]
	adds r0, r0, r6
	str r0, [r4, #4]
	b _08002EF2
_08002EDC:
	mov r0, ip
	ldr r1, [r0, #4]
	movs r0, #0x80
	lsls r0, r0, #2
	cmp r1, r0
	bne _08002EF2
	movs r1, #0x86
	lsls r1, r1, #2
	add r1, ip
	movs r0, #1
	str r0, [r1]
_08002EF2:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8002EFC
sub_8002EFC: @ 0x08002EFC
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	ldr r0, _08002F54 @ =gUnknown_03000804
	ldr r1, [r0]
	ldrb r0, [r1, #7]
	cmp r0, #0
	bne _08002F58
	movs r1, #0x86
	lsls r1, r1, #2
	adds r0, r4, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08002F20
	subs r1, #4
	adds r0, r4, r1
	ldr r0, [r0]
	cmp r0, #0
	bne _08002FC2
_08002F20:
	movs r0, #0x80
	lsls r0, r0, #2
	str r0, [r4]
	movs r2, #0
	str r2, [r4, #4]
	ldr r0, [r4, #8]
	str r0, [r4, #0xc]
	movs r0, #0x84
	lsls r0, r0, #2
	adds r1, r4, r0
	adds r0, r4, #0
	adds r0, #0x10
	str r0, [r1]
	movs r1, #0x85
	lsls r1, r1, #2
	adds r0, r4, r1
	str r2, [r0]
	adds r1, #4
	adds r0, r4, r1
	str r2, [r0]
	adds r1, #4
	adds r0, r4, r1
	str r2, [r0]
	movs r0, #1
	b _08002FC4
	.align 2, 0
_08002F54: .4byte gUnknown_03000804
_08002F58:
	movs r0, #0xff
	lsls r0, r0, #2
	adds r1, r1, r0
	ldr r0, [r1]
	cmp r0, #0
	bne _08002F68
	movs r1, #1
	b _08002F74
_08002F68:
	ldr r0, [r1]
	cmp r0, #1
	beq _08002F72
	movs r0, #2
	b _08002FC4
_08002F72:
	movs r1, #0
_08002F74:
	movs r0, #0x86
	lsls r0, r0, #2
	adds r6, r4, r0
	ldr r0, [r6]
	cmp r0, #0
	bne _08002F86
	adds r0, r4, #0
	bl sub_8002E20
_08002F86:
	movs r1, #0x85
	lsls r1, r1, #2
	adds r5, r4, r1
	ldr r0, [r5]
	cmp r0, #0
	bne _08002F98
	adds r0, r4, #0
	bl sub_8002D44
_08002F98:
	movs r3, #0
	ldr r0, [r6]
	cmp r0, #0
	beq _08002FBA
	ldr r0, [r5]
	cmp r0, #0
	beq _08002FBA
	movs r1, #0x87
	lsls r1, r1, #2
	adds r0, r4, r1
	ldr r1, [r0]
	adds r2, r1, #0
	adds r1, #1
	str r1, [r0]
	cmp r2, #0x1e
	ble _08002FBA
	movs r3, #1
_08002FBA:
	cmp r3, #0
	bne _08002FC2
	movs r0, #1
	b _08002FC4
_08002FC2:
	movs r0, #0
_08002FC4:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8002FCC
sub_8002FCC: @ 0x08002FCC
	str r1, [r0, #8]
	str r1, [r0, #0xc]
	bx lr
	.align 2, 0

	thumb_func_start sub_8002FD4
sub_8002FD4: @ 0x08002FD4
	adds r0, #0x10
	bx lr

	thumb_func_start sub_8002FD8
sub_8002FD8: @ 0x08002FD8
	adds r3, r0, #0
	movs r0, #0x80
	lsls r0, r0, #2
	str r0, [r3]
	movs r2, #0
	str r2, [r3, #4]
	ldr r0, [r3, #8]
	str r0, [r3, #0xc]
	movs r0, #0x84
	lsls r0, r0, #2
	adds r1, r3, r0
	adds r0, r3, #0
	adds r0, #0x10
	str r0, [r1]
	movs r1, #0x85
	lsls r1, r1, #2
	adds r0, r3, r1
	str r2, [r0]
	adds r1, #4
	adds r0, r3, r1
	str r2, [r0]
	adds r1, #4
	adds r0, r3, r1
	str r2, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_800300C
sub_800300C: @ 0x0800300C
	push {r4, lr}
	ldr r4, _08003024 @ =gUnknown_0300080C
	ldr r2, [r4]
	str r0, [r2, #0xc]
	str r1, [r2, #0x10]
	movs r1, #0
	str r1, [r2, #4]
	strb r1, [r2, #8]
	ldr r0, [r4]
	adds r0, #0x20
	strb r1, [r0]
	b _0800303A
	.align 2, 0
_08003024: .4byte gUnknown_0300080C
_08003028:
	ldr r0, _08003060 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r0, _08003064 @ =gUnknown_030007E0
	ldrh r1, [r0, #2]
	ldr r0, [r4]
	bl sub_80031E4
_0800303A:
	ldr r0, [r4]
	bl sub_8004BD0
	bl sub_80006A8
	ldr r0, [r4]
	bl sub_8004CE8
	ldr r0, [r4]
	ldrb r0, [r0, #8]
	cmp r0, #0
	beq _08003028
	ldr r0, _08003068 @ =gUnknown_0300080C
	ldr r0, [r0]
	adds r0, #0x20
	ldrb r0, [r0]
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08003060: .4byte gUnknown_03001304
_08003064: .4byte gUnknown_030007E0
_08003068: .4byte gUnknown_0300080C

	thumb_func_start sub_800306C
sub_800306C: @ 0x0800306C
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	adds r5, r0, #0
	movs r0, #0x8c
	adds r0, r0, r5
	mov sb, r0
	movs r6, #0x80
	lsls r6, r6, #2
	adds r0, r6, #0
	bl sub_8026EDC
	adds r4, r0, #0
	bl sub_8002C84
	mov r0, sb
	str r4, [r0]
	movs r0, #0x90
	adds r0, r0, r5
	mov r8, r0
	adds r0, r6, #0
	bl sub_8026EDC
	adds r4, r0, #0
	bl sub_8002C84
	mov r0, r8
	str r4, [r0]
	ldr r0, _0800311C @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006EA8
	adds r0, r5, #0
	bl sub_800450C
	adds r0, r5, #0
	bl sub_80047F8
	ldr r0, _08003120 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x10
	bl sub_8001B54
	adds r0, r5, #0
	bl sub_80048BC
	adds r4, r5, #0
	adds r4, #0x28
	ldr r0, _08003124 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80236EC
	adds r2, r0, #0
	adds r0, r5, #0
	adds r1, r4, #0
	bl sub_80048E0
	mov r0, sb
	ldr r1, [r0]
	adds r0, r5, #0
	bl sub_8004860
	ldr r4, _08003128 @ =gUnknown_03000804
	movs r0, #0x81
	lsls r0, r0, #3
	bl sub_80016DC
	bl sub_80027E8
	str r0, [r4]
	movs r0, #0x80
	movs r1, #1
	movs r2, #0
	bl sub_800132C
	adds r1, r5, #0
	adds r1, #0x20
	movs r0, #0
	strb r0, [r1]
	adds r0, r5, #0
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0800311C: .4byte gUnknown_030012B8
_08003120: .4byte gUnknown_030012BC
_08003124: .4byte gUnknown_030012C0
_08003128: .4byte gUnknown_03000804

	thumb_func_start sub_800312C
sub_800312C: @ 0x0800312C
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r7, r0, #0
	mov sb, r1
	ldr r0, _080031E0 @ =gUnknown_03000804
	ldr r0, [r0]
	cmp r0, #0
	beq _08003146
	movs r1, #3
	bl sub_80027B0
_08003146:
	adds r0, r7, #0
	adds r0, #0x90
	ldr r0, [r0]
	bl sub_8026ED0
	adds r0, r7, #0
	adds r0, #0x8c
	ldr r0, [r0]
	bl sub_8026ED0
	adds r6, r7, #0
	adds r6, #0xd0
	adds r5, r7, #0
	adds r5, #0xbc
	adds r4, r7, #0
	adds r4, #0xa8
	movs r0, #4
	mov r8, r0
_0800316A:
	ldr r2, [r4]
	cmp r2, #0
	beq _08003182
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_08003182:
	ldr r2, [r5]
	cmp r2, #0
	beq _0800319A
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0800319A:
	ldr r2, [r6]
	cmp r2, #0
	beq _080031B2
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_080031B2:
	adds r6, #4
	adds r5, #4
	adds r4, #4
	movs r0, #1
	rsbs r0, r0, #0
	add r8, r0
	mov r1, r8
	cmp r1, #0
	bge _0800316A
	movs r0, #1
	mov r3, sb
	ands r0, r3
	cmp r0, #0
	beq _080031D4
	adds r0, r7, #0
	bl sub_8026ED0
_080031D4:
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080031E0: .4byte gUnknown_03000804

	thumb_func_start sub_80031E4
sub_80031E4: @ 0x080031E4
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	movs r7, #0
_080031EC:
	lsls r4, r7, #2
	adds r0, r5, #0
	adds r0, #0xa8
	adds r0, r0, r4
	ldr r0, [r0]
	ldr r2, [r0, #0x18]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r2, #0x1c]
	bl sub_803AD7C
	adds r0, r5, #0
	adds r0, #0xbc
	adds r0, r0, r4
	ldr r0, [r0]
	ldr r2, [r0, #0x18]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r2, #0x1c]
	bl sub_803AD7C
	adds r0, r5, #0
	adds r0, #0xd0
	adds r0, r0, r4
	ldr r0, [r0]
	ldr r2, [r0, #0x18]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r2, #0x1c]
	bl sub_803AD7C
	adds r7, #1
	cmp r7, #4
	ble _080031EC
	ldr r0, [r5, #0xc]
	cmp r0, #0xa
	bhi _080032D0
	lsls r0, r0, #2
	ldr r1, _08003248 @ =_0800324C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08003248: .4byte _0800324C
_0800324C: @ jump table
	.4byte _08003278 @ case 0
	.4byte _08003282 @ case 1
	.4byte _08003288 @ case 2
	.4byte _08003298 @ case 3
	.4byte _080032A0 @ case 4
	.4byte _080032AA @ case 5
	.4byte _080032B4 @ case 6
	.4byte _080032C8 @ case 7
	.4byte _080032D0 @ case 8
	.4byte _080032BE @ case 9
	.4byte _080032D0 @ case 10
_08003278:
	adds r0, r5, #0
	adds r1, r6, #0
	bl sub_80032E8
	b _080032D0
_08003282:
	adds r0, r5, #0
	adds r0, #0x8c
	b _0800328C
_08003288:
	adds r0, r5, #0
	adds r0, #0x90
_0800328C:
	ldr r2, [r0]
	adds r0, r5, #0
	adds r1, r6, #0
	bl sub_80034BC
	b _080032D0
_08003298:
	adds r0, r5, #0
	bl sub_80035C0
	b _080032D0
_080032A0:
	adds r0, r5, #0
	adds r1, r6, #0
	bl sub_8004CB4
	b _080032D0
_080032AA:
	adds r0, r5, #0
	adds r1, r6, #0
	bl sub_8003824
	b _080032D0
_080032B4:
	adds r0, r5, #0
	adds r1, r6, #0
	bl sub_80038D0
	b _080032D0
_080032BE:
	adds r0, r5, #0
	adds r1, r6, #0
	bl sub_800376C
	b _080032D0
_080032C8:
	adds r0, r5, #0
	adds r1, r6, #0
	bl sub_800397C
_080032D0:
	ldr r0, [r5, #4]
	adds r0, #1
	movs r1, #0xff
	ands r0, r1
	str r0, [r5, #4]
	ldr r0, [r5]
	adds r0, #1
	str r0, [r5]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80032E8
sub_80032E8: @ 0x080032E8
	push {r4, r5, lr}
	adds r4, r0, #0
	movs r0, #0xa
	ands r0, r1
	cmp r0, #0
	beq _08003308
	ldr r0, _08003304 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x47
	bl PlaySfx
	b _0800338A
	.align 2, 0
_08003304: .4byte gUnknown_030012BC
_08003308:
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08003390
	ldr r0, _08003330 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
	ldr r0, [r4, #0x10]
	cmp r0, #4
	bhi _080033DE
	lsls r0, r0, #2
	ldr r1, _08003334 @ =_08003338
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08003330: .4byte gUnknown_030012BC
_08003334: .4byte _08003338
_08003338: @ jump table
	.4byte _0800334C @ case 0
	.4byte _08003350 @ case 1
	.4byte _08003370 @ case 2
	.4byte _08003374 @ case 3
	.4byte _0800338A @ case 4
_0800334C:
	movs r0, #1
	b _08003376
_08003350:
	movs r0, #3
	str r0, [r4, #0xc]
	movs r0, #0
	str r0, [r4, #0x10]
	movs r0, #0x2b
	bl sub_8026F38
	str r0, [r4, #0x14]
	movs r0, #0x2d
	bl sub_8026F38
	str r0, [r4, #0x18]
	adds r0, r4, #0
	bl sub_8004A80
	b _080033DE
_08003370:
	movs r0, #5
	b _08003376
_08003374:
	movs r0, #6
_08003376:
	str r0, [r4, #0xc]
	movs r0, #0
	str r0, [r4, #0x10]
	adds r0, r4, #0
	adds r0, #0x8c
	ldr r1, [r0]
	adds r0, r4, #0
	bl sub_8004860
	b _080033DE
_0800338A:
	movs r0, #1
	strb r0, [r4, #8]
	b _080033DE
_08003390:
	movs r5, #0x40
	ands r5, r1
	cmp r5, #0
	beq _080033BC
	ldr r0, _080033B8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x46
	bl PlaySfx
	ldr r0, [r4, #0x10]
	subs r0, #1
	str r0, [r4, #0x10]
	cmp r0, #0
	bge _080033DE
	movs r0, #4
	str r0, [r4, #0x10]
	b _080033DE
	.align 2, 0
_080033B8: .4byte gUnknown_030012BC
_080033BC:
	movs r0, #0x80
	ands r0, r1
	cmp r0, #0
	beq _080033DE
	ldr r0, _080033E4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x46
	bl PlaySfx
	ldr r0, [r4, #0x10]
	adds r0, #1
	str r0, [r4, #0x10]
	cmp r0, #4
	ble _080033DE
	str r5, [r4, #0x10]
_080033DE:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080033E4: .4byte gUnknown_030012BC

	thumb_func_start sub_80033E8
sub_80033E8: @ 0x080033E8
	push {r4, lr}
	adds r4, r0, #0
	movs r0, #0x40
	ands r0, r1
	cmp r0, #0
	beq _0800343E
	ldr r0, _08003414 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x46
	bl PlaySfx
	ldr r0, [r4, #0x10]
	cmp r0, #4
	bhi _080034B2
	lsls r0, r0, #2
	ldr r1, _08003418 @ =_0800341C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08003414: .4byte gUnknown_030012BC
_08003418: .4byte _0800341C
_0800341C: @ jump table
	.4byte _08003430 @ case 0
	.4byte _08003434 @ case 1
	.4byte _08003430 @ case 2
	.4byte _08003434 @ case 3
	.4byte _0800343A @ case 4
_08003430:
	movs r0, #4
	b _080034B0
_08003434:
	ldr r0, [r4, #0x10]
	subs r0, #1
	b _080034B0
_0800343A:
	movs r0, #1
	b _080034B0
_0800343E:
	movs r0, #0x80
	ands r0, r1
	cmp r0, #0
	beq _0800348E
	ldr r0, _08003464 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x46
	bl PlaySfx
	ldr r0, [r4, #0x10]
	cmp r0, #4
	bhi _080034B2
	lsls r0, r0, #2
	ldr r1, _08003468 @ =_0800346C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08003464: .4byte gUnknown_030012BC
_08003468: .4byte _0800346C
_0800346C: @ jump table
	.4byte _08003480 @ case 0
	.4byte _08003486 @ case 1
	.4byte _08003480 @ case 2
	.4byte _08003486 @ case 3
	.4byte _0800348A @ case 4
_08003480:
	ldr r0, [r4, #0x10]
	adds r0, #1
	b _080034B0
_08003486:
	movs r0, #4
	b _080034B0
_0800348A:
	movs r0, #0
	b _080034B0
_0800348E:
	movs r0, #0x30
	ands r0, r1
	cmp r0, #0
	beq _080034B2
	ldr r0, [r4, #0x10]
	cmp r0, #4
	beq _080034B2
	ldr r0, _080034B8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x46
	bl PlaySfx
	ldr r0, [r4, #0x10]
	movs r1, #2
	eors r0, r1
_080034B0:
	str r0, [r4, #0x10]
_080034B2:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080034B8: .4byte gUnknown_030012BC

	thumb_func_start sub_80034BC
sub_80034BC: @ 0x080034BC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x70
	adds r6, r0, #0
	adds r7, r2, #0
	movs r0, #1
	mov r8, r0
	adds r0, r1, #0
	mov r2, r8
	ands r0, r2
	cmp r0, #0
	bne _080034DE
	movs r4, #8
	ands r4, r1
	cmp r4, #0
	beq _0800358C
_080034DE:
	ldr r1, [r6, #0x10]
	cmp r1, #4
	bne _08003500
	ldr r0, _080034FC @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
	movs r0, #0
	str r0, [r6, #0xc]
	str r0, [r6, #0x10]
	b _080035B2
	.align 2, 0
_080034FC: .4byte gUnknown_030012BC
_08003500:
	adds r0, r7, #0
	bl sub_8002CE8
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08003520
	ldr r0, _0800351C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x48
	bl PlaySfx
	b _080035B2
	.align 2, 0
_0800351C: .4byte gUnknown_030012BC
_08003520:
	ldr r5, _08003584 @ =gUnknown_030012BC
	ldr r0, [r5]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
	ldr r1, [r6, #0x10]
	adds r0, r7, #0
	mov r2, sp
	bl sub_8002C14
	ldr r4, _08003588 @ =gUnknown_030012C0
	ldr r0, [r4]
	mov r1, sp
	bl sub_80236AC
	ldr r0, [r4]
	add r1, sp, #0x68
	ldrb r1, [r1]
	bl sub_8023334
	ldr r0, [r5]
	mov r1, sp
	adds r1, #0x6a
	ldrh r1, [r1]
	bl sub_8001B50
	ldr r0, [r5]
	add r1, sp, #0x6c
	ldrh r1, [r1]
	bl sub_8001B30
	adds r5, r6, #0
	adds r5, #0x28
	ldr r0, [r4]
	bl sub_80236EC
	adds r2, r0, #0
	adds r0, r6, #0
	adds r1, r5, #0
	bl sub_80048E0
	adds r0, r6, #0
	adds r0, #0x20
	mov r1, r8
	strb r1, [r0]
	strb r1, [r6, #8]
	b _080035B2
	.align 2, 0
_08003584: .4byte gUnknown_030012BC
_08003588: .4byte gUnknown_030012C0
_0800358C:
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _080035AC
	ldr r0, _080035A8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x47
	bl PlaySfx
	str r4, [r6, #0xc]
	str r4, [r6, #0x10]
	b _080035B2
	.align 2, 0
_080035A8: .4byte gUnknown_030012BC
_080035AC:
	adds r0, r6, #0
	bl sub_80033E8
_080035B2:
	add sp, #0x70
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80035C0
sub_80035C0: @ 0x080035C0
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	bl sub_8003B40
	adds r4, r0, #0
	adds r0, r5, #0
	bl sub_8004A64
	cmp r4, #3
	bne _080035F0
	movs r0, #0
	str r0, [r5, #0xc]
	movs r0, #1
	str r0, [r5, #0x10]
	ldr r0, _080035EC @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x47
	bl PlaySfx
	b _0800368C
	.align 2, 0
_080035EC: .4byte gUnknown_030012BC
_080035F0:
	cmp r4, #2
	beq _08003604
	adds r7, r5, #0
	adds r7, #0x90
	ldr r0, [r7]
	bl sub_8002B44
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08003610
_08003604:
	movs r0, #4
	str r0, [r5, #0xc]
	movs r0, #0x2c
	bl sub_8026F38
	b _08003682
_08003610:
	adds r6, r5, #0
	adds r6, #0x8c
	ldr r0, [r6]
	bl sub_8002B94
	adds r4, r0, #0
	ldr r0, [r7]
	bl sub_8002B94
	cmp r4, r0
	bne _08003638
	movs r0, #2
	str r0, [r5, #0xc]
	movs r0, #0
	str r0, [r5, #0x10]
	ldr r1, [r7]
	adds r0, r5, #0
	bl sub_8004860
	b _0800368C
_08003638:
	ldr r0, [r7]
	bl sub_8002B94
	cmp r0, #2
	beq _08003650
	cmp r0, #3
	beq _0800366C
	movs r0, #0
	str r0, [r5, #0xc]
	movs r0, #1
	str r0, [r5, #0x10]
	b _0800368C
_08003650:
	ldr r0, [r6]
	movs r1, #2
	bl sub_8002D28
	ldr r0, [r6]
	bl sub_8002BA4
	movs r0, #4
	str r0, [r5, #0xc]
	ldr r0, _08003668 @ =gUnknown_03000810
	b _08003680
	.align 2, 0
_08003668: .4byte gUnknown_03000810
_0800366C:
	ldr r0, [r6]
	movs r1, #4
	bl sub_8002D28
	ldr r0, [r6]
	bl sub_8002BA4
	movs r0, #4
	str r0, [r5, #0xc]
	ldr r0, _08003694 @ =gUnknown_03000814
_08003680:
	ldr r0, [r0]
_08003682:
	str r0, [r5, #0x14]
	movs r0, #0x2e
	bl sub_8026F38
	str r0, [r5, #0x18]
_0800368C:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08003694: .4byte gUnknown_03000814

	thumb_func_start sub_8003698
sub_8003698: @ 0x08003698
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #0xe0
	adds r7, r0, #0
	adds r6, r1, #0
	adds r4, r7, #0
	adds r4, #0x8c
	ldr r0, [r4]
	bl sub_8002CE8
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _080036C6
	ldr r0, [r4]
	adds r1, r6, #0
	mov r2, sp
	bl sub_8002C14
	movs r0, #0
	mov sb, r0
	b _080036CA
_080036C6:
	movs r1, #1
	mov sb, r1
_080036CA:
	ldr r0, _0800372C @ =gUnknown_030012C0
	mov r8, r0
	ldr r0, [r0]
	bl sub_80236EC
	adds r1, r0, #0
	add r5, sp, #0x70
	adds r0, r5, #0
	movs r2, #0x68
	bl sub_800014C
	mov r1, r8
	ldr r0, [r1]
	bl sub_802332C
	add r1, sp, #0xd8
	strb r0, [r1]
	ldr r4, _08003730 @ =gUnknown_030012BC
	ldr r0, [r4]
	bl sub_8001ABC
	mov r1, sp
	adds r1, #0xda
	strh r0, [r1]
	ldr r0, [r4]
	bl sub_8001AC0
	add r1, sp, #0xdc
	strh r0, [r1]
	adds r4, r7, #0
	adds r4, #0x8c
	ldr r0, [r4]
	adds r1, r6, #0
	adds r2, r5, #0
	bl sub_8002C40
	ldr r0, [r4]
	bl sub_8002BA4
	cmp r0, #0
	beq _08003740
	mov r0, sb
	cmp r0, #0
	beq _08003734
	ldr r0, [r4]
	adds r1, r6, #0
	bl sub_8002C6C
	b _0800375C
	.align 2, 0
_0800372C: .4byte gUnknown_030012C0
_08003730: .4byte gUnknown_030012BC
_08003734:
	ldr r0, [r4]
	adds r1, r6, #0
	mov r2, sp
	bl sub_8002C40
	b _0800375C
_08003740:
	lsls r4, r6, #2
	adds r4, r4, r6
	lsls r4, r4, #2
	adds r4, #0x3c
	adds r4, r7, r4
	mov r1, r8
	ldr r0, [r1]
	bl sub_80236EC
	adds r2, r0, #0
	adds r0, r7, #0
	adds r1, r4, #0
	bl sub_80048E0
_0800375C:
	add sp, #0xe0
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_800376C
sub_800376C: @ 0x0800376C
	push {r4, r5, lr}
	adds r4, r0, #0
	movs r3, #1
	adds r0, r1, #0
	ands r0, r3
	cmp r0, #0
	bne _08003782
	movs r0, #8
	ands r0, r1
	cmp r0, #0
	beq _080037B4
_08003782:
	ldr r5, [r4, #0x10]
	cmp r5, #0
	bne _08003798
	ldr r1, [r4, #0x24]
	adds r0, r4, #0
	bl sub_8003698
	str r5, [r4, #0xc]
	movs r0, #4
	str r0, [r4, #0x10]
	b _0800381A
_08003798:
	movs r0, #5
	str r0, [r4, #0xc]
	ldr r0, [r4, #0x24]
	str r0, [r4, #0x10]
	ldr r0, _080037B0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
	b _0800381A
	.align 2, 0
_080037B0: .4byte gUnknown_030012BC
_080037B4:
	movs r2, #2
	ands r2, r1
	cmp r2, #0
	beq _080037D8
	movs r0, #5
	str r0, [r4, #0xc]
	ldr r0, [r4, #0x24]
	str r0, [r4, #0x10]
	ldr r0, _080037D4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x47
	bl PlaySfx
	b _0800381A
	.align 2, 0
_080037D4: .4byte gUnknown_030012BC
_080037D8:
	movs r0, #0x40
	ands r0, r1
	cmp r0, #0
	beq _080037FC
	ldr r0, [r4, #0x10]
	cmp r0, #1
	bne _0800381A
	str r2, [r4, #0x10]
	ldr r0, _080037F8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x46
	bl PlaySfx
	b _0800381A
	.align 2, 0
_080037F8: .4byte gUnknown_030012BC
_080037FC:
	movs r0, #0x80
	ands r0, r1
	cmp r0, #0
	beq _0800381A
	ldr r0, [r4, #0x10]
	cmp r0, #0
	bne _0800381A
	str r3, [r4, #0x10]
	ldr r0, _08003820 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x46
	bl PlaySfx
_0800381A:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08003820: .4byte gUnknown_030012BC

	thumb_func_start sub_8003824
sub_8003824: @ 0x08003824
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	bne _08003838
	movs r5, #8
	ands r5, r1
	cmp r5, #0
	beq _080038A2
_08003838:
	ldr r0, [r4, #0x10]
	cmp r0, #4
	bne _0800385C
	ldr r0, _08003858 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
	movs r0, #0
	str r0, [r4, #0xc]
	movs r0, #2
	str r0, [r4, #0x10]
	b _080038CA
	.align 2, 0
_08003858: .4byte gUnknown_030012BC
_0800385C:
	ldr r0, _0800388C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
	adds r0, r4, #0
	adds r0, #0x8c
	ldr r0, [r0]
	ldr r1, [r4, #0x10]
	bl sub_8002CE8
	lsls r0, r0, #0x18
	lsrs r1, r0, #0x18
	cmp r1, #0
	bne _08003890
	movs r0, #9
	str r0, [r4, #0xc]
	ldr r0, [r4, #0x10]
	str r0, [r4, #0x24]
	str r1, [r4, #0x10]
	b _080038CA
	.align 2, 0
_0800388C: .4byte gUnknown_030012BC
_08003890:
	ldr r1, [r4, #0x10]
	adds r0, r4, #0
	bl sub_8003698
	movs r0, #0
	str r0, [r4, #0xc]
	movs r0, #4
	str r0, [r4, #0x10]
	b _080038CA
_080038A2:
	movs r6, #2
	adds r0, r1, #0
	ands r0, r6
	cmp r0, #0
	beq _080038C4
	ldr r0, _080038C0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x47
	bl PlaySfx
	str r5, [r4, #0xc]
	str r6, [r4, #0x10]
	b _080038CA
	.align 2, 0
_080038C0: .4byte gUnknown_030012BC
_080038C4:
	adds r0, r4, #0
	bl sub_80033E8
_080038CA:
	pop {r4, r5, r6}
	pop {r0}
	bx r0

	thumb_func_start sub_80038D0
sub_80038D0: @ 0x080038D0
	push {r4, r5, lr}
	adds r4, r0, #0
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	bne _080038E4
	movs r5, #8
	ands r5, r1
	cmp r5, #0
	beq _0800394C
_080038E4:
	ldr r1, [r4, #0x10]
	cmp r1, #4
	bne _08003904
	ldr r0, _08003900 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
	movs r0, #0
	str r0, [r4, #0xc]
	b _08003964
	.align 2, 0
_08003900: .4byte gUnknown_030012BC
_08003904:
	adds r0, r4, #0
	adds r0, #0x8c
	ldr r0, [r0]
	bl sub_8002CE8
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	cmp r5, #0
	beq _0800392C
	ldr r0, _08003928 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x48
	bl PlaySfx
	b _08003976
	.align 2, 0
_08003928: .4byte gUnknown_030012BC
_0800392C:
	ldr r0, _08003948 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
	movs r0, #7
	str r0, [r4, #0xc]
	ldr r0, [r4, #0x10]
	str r0, [r4, #0x24]
	str r5, [r4, #0x10]
	b _08003976
	.align 2, 0
_08003948: .4byte gUnknown_030012BC
_0800394C:
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _08003970
	ldr r0, _0800396C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x47
	bl PlaySfx
	str r5, [r4, #0xc]
_08003964:
	movs r0, #3
	str r0, [r4, #0x10]
	b _08003976
	.align 2, 0
_0800396C: .4byte gUnknown_030012BC
_08003970:
	adds r0, r4, #0
	bl sub_80033E8
_08003976:
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_800397C
sub_800397C: @ 0x0800397C
	push {r4, r5, r6, r7, lr}
	sub sp, #0x70
	adds r4, r0, #0
	movs r3, #1
	adds r0, r1, #0
	ands r0, r3
	cmp r0, #0
	bne _08003994
	movs r0, #8
	ands r0, r1
	cmp r0, #0
	beq _080039EC
_08003994:
	ldr r7, [r4, #0x10]
	cmp r7, #0
	bne _080039CE
	ldr r6, [r4, #0x24]
	adds r5, r4, #0
	adds r5, #0x8c
	ldr r0, [r5]
	adds r1, r6, #0
	mov r2, sp
	bl sub_8002C14
	ldr r0, [r5]
	adds r1, r6, #0
	bl sub_8002C6C
	ldr r0, [r5]
	bl sub_8002BA4
	cmp r0, #0
	beq _080039C6
	ldr r0, [r5]
	adds r1, r6, #0
	mov r2, sp
	bl sub_8002C40
_080039C6:
	str r7, [r4, #0xc]
	movs r0, #4
	str r0, [r4, #0x10]
	b _08003A52
_080039CE:
	movs r0, #6
	str r0, [r4, #0xc]
	ldr r0, [r4, #0x24]
	str r0, [r4, #0x10]
	ldr r0, _080039E8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
	b _08003A52
	.align 2, 0
_080039E8: .4byte gUnknown_030012BC
_080039EC:
	movs r2, #2
	ands r2, r1
	cmp r2, #0
	beq _08003A10
	movs r0, #6
	str r0, [r4, #0xc]
	ldr r0, [r4, #0x24]
	str r0, [r4, #0x10]
	ldr r0, _08003A0C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x47
	bl PlaySfx
	b _08003A52
	.align 2, 0
_08003A0C: .4byte gUnknown_030012BC
_08003A10:
	movs r0, #0x40
	ands r0, r1
	cmp r0, #0
	beq _08003A34
	ldr r0, [r4, #0x10]
	cmp r0, #1
	bne _08003A52
	str r2, [r4, #0x10]
	ldr r0, _08003A30 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x46
	bl PlaySfx
	b _08003A52
	.align 2, 0
_08003A30: .4byte gUnknown_030012BC
_08003A34:
	movs r0, #0x80
	ands r0, r1
	cmp r0, #0
	beq _08003A52
	ldr r0, [r4, #0x10]
	cmp r0, #0
	bne _08003A52
	str r3, [r4, #0x10]
	ldr r0, _08003A5C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x46
	bl PlaySfx
_08003A52:
	add sp, #0x70
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08003A5C: .4byte gUnknown_030012BC

	thumb_func_start sub_8003A60
sub_8003A60: @ 0x08003A60
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov sb, r0
	movs r0, #0x64
	mov sl, r0
	movs r7, #0
	ldr r1, _08003A98 @ =gUnknown_030012DC
	mov r8, r1
_08003A78:
	mov r2, sb
	ldr r0, [r2, #0x10]
	cmp r7, r0
	bne _08003A9C
	mov r0, r8
	ldr r4, [r0]
	mov r0, sb
	bl sub_8004A50
	adds r1, r0, #0
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	adds r0, r4, #0
	bl sub_8028A30
	b _08003AA6
	.align 2, 0
_08003A98: .4byte gUnknown_030012DC
_08003A9C:
	mov r1, r8
	ldr r0, [r1]
	movs r1, #0
	bl sub_8028A30
_08003AA6:
	mov r2, r8
	ldr r4, [r2]
	movs r1, #0x98
	lsls r1, r1, #1
	adds r0, r4, r1
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x10
	movs r2, #0x10
	ldrsh r0, [r0, r2]
	adds r4, r4, r0
	ldr r1, _08003B3C @ =gStaticData_0816B1BC
	lsls r0, r7, #2
	adds r0, r0, r1
	ldr r6, [r0]
	adds r0, r6, #0
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	movs r1, #0xf0
	subs r1, r1, r0
	asrs r1, r1, #1
	mov r0, r8
	ldr r4, [r0]
	movs r2, #0x88
	lsls r2, r2, #1
	adds r0, r4, r2
	str r1, [r0]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r4, r1
	mov r2, sl
	str r2, [r0]
	adds r1, #0x1c
	adds r0, r4, r1
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r2, #0x20
	ldrsh r0, [r0, r2]
	adds r4, r4, r0
	adds r0, r6, #0
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	movs r0, #0xa
	add sl, r0
	adds r7, #1
	cmp r7, #4
	ble _08003A78
	mov r1, sp
	movs r0, #0
	strb r0, [r1]
	mov r0, sb
	movs r1, #0x5a
	movs r2, #0x21
	movs r3, #0
	bl sub_8003F30
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08003B3C: .4byte gStaticData_0816B1BC


@ sub_8003B40 is reconstructed (semantics understood) but NOT YET
@ byte-matching - parked as C in src/graphics/settings_menu.c, guarded
@ by #if NON_MATCHING. See docs/matching.md for this chunk's write-up.
.if NON_MATCHING == 0
	thumb_func_start sub_8003B40
sub_8003B40: @ 0x08003B40
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	movs r0, #0x88
	lsls r0, r0, #2
	bl sub_8026EDC
	adds r5, r0, #0
	adds r0, r6, #0
	adds r0, #0x8c
	ldr r1, [r0]
	adds r0, r5, #0
	bl sub_8002FCC
	adds r0, r5, #0
	bl sub_8002FD8
_08003B60:
	bl sub_80006A8
	ldr r0, _08003B80 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r0, _08003B84 @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #2
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r1, r0, #0x10
	cmp r1, #0
	beq _08003B88
	movs r4, #3
	b _08003BA8
	.align 2, 0
_08003B80: .4byte gUnknown_03001304
_08003B84: .4byte gUnknown_030007E0
_08003B88:
	ldr r2, _08003BD4 @ =gUnknown_03000800
	ldrb r0, [r2]
	cmp r0, #0
	beq _08003B98
	strb r1, [r2]
	adds r0, r5, #0
	bl sub_8002FD8
_08003B98:
	ldr r0, _08003BD8 @ =gUnknown_03000804
	ldr r0, [r0]
	bl sub_8001F50
	adds r0, r5, #0
	bl sub_8002EFC
	adds r4, r0, #0
_08003BA8:
	cmp r4, #1
	beq _08003B60
	cmp r4, #0
	bne _08003BC6
	adds r0, r5, #0
	bl sub_8002FD4
	adds r1, r0, #0
	adds r0, r6, #0
	adds r0, #0x90
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #2
	bl sub_800014C
_08003BC6:
	adds r0, r5, #0
	bl sub_8026ED0
	adds r0, r4, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08003BD4: .4byte gUnknown_03000800
_08003BD8: .4byte gUnknown_03000804
.endif

@ sub_8003BDC is reconstructed (semantics understood) but NOT YET
@ byte-matching - parked as C in src/graphics/settings_menu.c, guarded
@ by #if NON_MATCHING. See docs/matching.md for this chunk's write-up.
.if NON_MATCHING == 0
	thumb_func_start sub_8003BDC
sub_8003BDC: @ 0x08003BDC
	push {r4, r5, r6, r7, lr}
	adds r5, r1, #0
	adds r6, r2, #0
	ldr r7, _08003C8C @ =gUnknown_030012DC
	ldr r0, [r7]
	movs r1, #0
	bl sub_8028A30
	cmp r5, #0
	beq _08003C3A
	ldr r0, [r7]
	movs r4, #0x98
	lsls r4, r4, #1
	adds r1, r0, r4
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	adds r1, r5, #0
	bl sub_803AD80
	adds r1, r0, #0
	movs r0, #0xf0
	subs r0, r0, r1
	asrs r3, r0, #1
	ldr r0, [r7]
	movs r1, #0x87
	mov ip, r1
	movs r2, #0x88
	lsls r2, r2, #1
	adds r1, r0, r2
	str r3, [r1]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r1, r0, r3
	mov r2, ip
	str r2, [r1]
	adds r4, r0, r4
	ldr r2, [r4]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	adds r1, r5, #0
	bl sub_803AD80
_08003C3A:
	cmp r6, #0
	beq _08003C84
	ldr r0, [r7]
	movs r4, #0x98
	lsls r4, r4, #1
	adds r1, r0, r4
	ldr r2, [r1]
	movs r5, #0x10
	ldrsh r1, [r2, r5]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	adds r1, r6, #0
	bl sub_803AD80
	adds r1, r0, #0
	movs r0, #0xf0
	subs r0, r0, r1
	asrs r3, r0, #1
	ldr r0, [r7]
	movs r2, #0x91
	movs r5, #0x88
	lsls r5, r5, #1
	adds r1, r0, r5
	str r3, [r1]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r1, r0, r3
	str r2, [r1]
	adds r4, r0, r4
	ldr r2, [r4]
	movs r5, #0x20
	ldrsh r1, [r2, r5]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	adds r1, r6, #0
	bl sub_803AD80
_08003C84:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08003C8C: .4byte gUnknown_030012DC
.endif

@ sub_8003C90 is reconstructed (semantics understood) but NOT YET
@ byte-matching - parked as C in src/graphics/settings_menu.c, guarded
@ by #if NON_MATCHING. See docs/matching.md for this chunk's write-up.
.if NON_MATCHING == 0
	thumb_func_start sub_8003C90
sub_8003C90: @ 0x08003C90
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r2, r0, #0
	lsls r1, r1, #0x18
	cmp r1, #0
	beq _08003CC0
	ldr r0, _08003CBC @ =gUnknown_030012DC
	ldr r3, [r0]
	ldr r0, [r2, #4]
	asrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	movs r1, #2
	cmp r0, #0
	beq _08003CB2
	movs r1, #1
_08003CB2:
	adds r0, r3, #0
	bl sub_8028A30
	b _08003CCA
	.align 2, 0
_08003CBC: .4byte gUnknown_030012DC
_08003CC0:
	ldr r0, _08003D38 @ =gUnknown_030012DC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8028A30
_08003CCA:
	ldr r0, _08003D38 @ =gUnknown_030012DC
	mov r8, r0
	ldr r4, [r0]
	movs r5, #0x98
	lsls r5, r5, #1
	adds r0, r4, r5
	ldr r0, [r0]
	adds r6, r0, #0
	adds r6, #0x10
	movs r1, #0x10
	ldrsh r0, [r0, r1]
	adds r4, r4, r0
	movs r0, #0x23
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r6, #4]
	adds r0, r4, #0
	bl sub_803AD80
	movs r1, #0xf0
	subs r1, r1, r0
	asrs r1, r1, #1
	mov r3, r8
	ldr r4, [r3]
	movs r2, #0x87
	movs r3, #0x88
	lsls r3, r3, #1
	adds r0, r4, r3
	str r1, [r0]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r4, r1
	str r2, [r0]
	adds r5, r4, r5
	ldr r0, [r5]
	adds r5, r0, #0
	adds r5, #0x20
	movs r3, #0x20
	ldrsh r0, [r0, r3]
	adds r4, r4, r0
	movs r0, #0x23
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08003D38: .4byte gUnknown_030012DC

.endif
