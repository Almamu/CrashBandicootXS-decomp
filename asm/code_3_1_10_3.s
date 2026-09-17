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

