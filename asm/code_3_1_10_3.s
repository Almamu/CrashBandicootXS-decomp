.include "asm/macros.inc"

.syntax unified
.arm
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
