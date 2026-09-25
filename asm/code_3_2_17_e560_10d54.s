.include "asm/macros.inc"

.syntax unified
.arm
	thumb_func_start sub_8010E34
sub_8010E34: @ 0x08010E34
	push {r4, r5, lr}
	sub sp, #0x20
	adds r5, r0, #0
	adds r0, #0x4a
	ldrb r0, [r0]
	cmp r0, #0
	beq _08010E58
	adds r0, r5, #0
	adds r0, #0x4b
	ldrb r0, [r0]
	cmp r0, #0x16
	bhi _08010E58
	ldr r0, _08010EA8 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #3
	bne _08010EA0
_08010E58:
	ldrb r0, [r5, #0xc]
	lsls r1, r0, #0x18
	lsrs r0, r1, #0x1b
	movs r2, #1
	ands r0, r2
	cmp r0, #0
	bne _08010EA0
	lsrs r0, r1, #0x1a
	ands r0, r2
	cmp r0, #0
	beq _08010EA0
	mov r0, sp
	adds r1, r5, #0
	bl sub_8007B98
	ldr r0, _08010EA8 @ =gUnknown_030012D8
	ldr r1, [r0]
	add r4, sp, #0x10
	adds r0, r4, #0
	bl sub_8007B98
	adds r0, r4, #0
	mov r1, sp
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08010EA0
	movs r0, #8
	ldrb r1, [r5, #0xc]
	orrs r0, r1
	strb r0, [r5, #0xc]
	adds r0, r5, #0
	movs r1, #0
	bl sub_8010EAC
_08010EA0:
	add sp, #0x20
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08010EA8: .4byte gUnknown_030012D8

	thumb_func_start sub_8010EAC
sub_8010EAC: @ 0x08010EAC
	push {r4, r5, r6, lr}
	sub sp, #0xc
	adds r5, r0, #0
	lsls r4, r1, #0x18
	lsrs r4, r4, #0x18
	ldr r0, _08010EF4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #7
	bl PlaySfx
	movs r0, #0xa0
	strh r0, [r5, #0x3c]
	cmp r4, #0
	beq _08010F1C
	bl rand
	lsls r0, r0, #0x10
	lsrs r2, r0, #0x10
	movs r1, #1
	adds r0, r2, #0
	ands r0, r1
	adds r1, r5, #0
	adds r1, #0x49
	strb r0, [r1]
	cmp r0, #0
	beq _08010F02
	movs r0, #2
	ands r0, r2
	cmp r0, #0
	beq _08010EF8
	movs r0, #0x3f
	ands r0, r2
	adds r0, #5
	b _08010F08
	.align 2, 0
_08010EF4: .4byte gUnknown_030012BC
_08010EF8:
	movs r1, #0x3f
	ands r1, r2
	movs r0, #0xeb
	subs r0, r0, r1
	b _08010F08
_08010F02:
	movs r0, #0x7f
	ands r0, r2
	adds r0, #0x24
_08010F08:
	lsls r4, r0, #8
	movs r0, #0x1f
	ands r0, r2
	adds r0, #0x10
	lsls r6, r0, #8
	adds r1, r5, #0
	adds r1, #0x48
	movs r0, #2
	strb r0, [r1]
	b _08010F34
_08010F1C:
	movs r4, #0xb4
	lsls r4, r4, #8
	movs r6, #0xc0
	lsls r6, r6, #4
	adds r1, r5, #0
	adds r1, #0x48
	movs r0, #1
	strb r0, [r1]
	ldr r0, _08010F88 @ =gUnknown_03001318
	ldr r0, [r0]
	bl sub_80284A4
_08010F34:
	movs r0, #0x10
	ldrb r1, [r5, #0xc]
	orrs r0, r1
	strb r0, [r5, #0xc]
	movs r0, #1
	adds r1, r5, #0
	adds r1, #0x25
	strb r0, [r1]
	ldr r1, [r5]
	asrs r1, r1, #8
	ldr r2, [r5, #4]
	asrs r2, r2, #8
	add r0, sp, #8
	str r0, [sp]
	adds r0, r5, #0
	add r3, sp, #4
	bl sub_8007174
	ldr r0, [sp, #4]
	lsls r0, r0, #8
	str r0, [r5]
	subs r0, r0, r4
	movs r4, #0xa0
	lsls r4, r4, #5
	adds r1, r4, #0
	bl sub_80008F0
	rsbs r0, r0, #0
	str r0, [r5, #0x40]
	ldr r0, [sp, #8]
	lsls r0, r0, #8
	str r0, [r5, #4]
	subs r0, r0, r6
	adds r1, r4, #0
	bl sub_80008F0
	rsbs r0, r0, #0
	str r0, [r5, #0x44]
	add sp, #0xc
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08010F88: .4byte gUnknown_03001318

	thumb_func_start sub_8010F8C
sub_8010F8C: @ 0x08010F8C
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r0, #0x48
	ldrb r5, [r0]
	cmp r5, #1
	bne _08011014
	ldr r1, [r4]
	ldr r3, [r4, #0x40]
	adds r0, r1, r3
	str r0, [r4]
	ldr r2, [r4, #4]
	ldr r0, [r4, #0x44]
	adds r0, r2, r0
	str r0, [r4, #4]
	adds r1, r1, r3
	asrs r1, r1, #8
	cmp r1, #0xb4
	ble _08010FB2
	b _080110C0
_08010FB2:
	asrs r0, r0, #8
	cmp r0, #0xc
	ble _08010FBA
	b _080110C0
_08010FBA:
	ldr r0, _08011004 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xe
	bl PlaySfx
	ldr r0, _08011008 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023464
	movs r0, #1
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _0801100C @ =0x0000FFFF
	ldrh r2, [r4, #8]
	cmp r2, r0
	beq _080110C0
	ldrh r3, [r4, #8]
	ldr r0, _08011010 @ =gUnknown_030012B4
	ldr r1, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r2, r0, #2
	movs r6, #0x84
	lsls r6, r6, #1
	adds r1, r1, r6
	adds r1, r1, r2
	lsls r0, r0, #5
	subs r0, r3, r0
	lsls r5, r0
	ldr r0, [r1]
	orrs r0, r5
	str r0, [r1]
	b _080110C0
	.align 2, 0
_08011004: .4byte gUnknown_030012BC
_08011008: .4byte gUnknown_030012C0
_0801100C: .4byte 0x0000FFFF
_08011010: .4byte gUnknown_030012B4
_08011014:
	cmp r5, #2
	bne _08011094
	ldr r1, [r4]
	ldr r0, [r4, #0x40]
	adds r1, r1, r0
	str r1, [r4]
	ldr r1, [r4, #4]
	ldr r0, [r4, #0x44]
	adds r1, r1, r0
	str r1, [r4, #4]
	movs r1, #0
	adds r0, r4, #0
	adds r0, #0x49
	ldrb r0, [r0]
	cmp r0, #0
	bne _08011040
	ldrh r0, [r4, #0x3c]
	subs r0, #4
	strh r0, [r4, #0x3c]
	cmp r0, #0x3f
	bgt _08011052
	b _08011056
_08011040:
	ldrh r0, [r4, #0x3c]
	adds r0, #0xc
	strh r0, [r4, #0x3c]
	movs r0, #0xd8
	lsls r0, r0, #1
	ldrh r2, [r4, #0x3c]
	cmp r2, r0
	ble _08011052
	movs r1, #1
_08011052:
	cmp r1, #0
	beq _080110C0
_08011056:
	movs r0, #1
	ldrb r5, [r4, #0xc]
	orrs r0, r5
	strb r0, [r4, #0xc]
	ldr r0, _0801108C @ =0x0000FFFF
	ldrh r6, [r4, #8]
	cmp r6, r0
	beq _080110C0
	ldrh r3, [r4, #8]
	ldr r0, _08011090 @ =gUnknown_030012B4
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
	b _080110C0
	.align 2, 0
_0801108C: .4byte 0x0000FFFF
_08011090: .4byte gUnknown_030012B4
_08011094:
	adds r2, r4, #0
	adds r2, #0x4a
	ldrb r0, [r2]
	cmp r0, #0
	bne _080110AA
	adds r1, r4, #0
	adds r1, #0x49
	ldrb r0, [r1]
	adds r0, #1
	strb r0, [r1]
	b _080110C0
_080110AA:
	adds r1, r4, #0
	adds r1, #0x4b
	ldrb r0, [r1]
	adds r0, #1
	strb r0, [r1]
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #0x1f
	bls _080110C0
	movs r0, #0
	strb r0, [r2]
_080110C0:
	adds r0, r4, #0
	adds r0, #0x48
	ldrb r0, [r0]
	cmp r0, #0
	bne _08011106
	adds r0, r4, #0
	adds r0, #0x4a
	ldrb r0, [r0]
	cmp r0, #0
	bne _08011100
	ldr r1, _080110FC @ =gStaticData_0816A820
	adds r2, r4, #0
	adds r2, #0x49
	movs r0, #0x7f
	ldrb r2, [r2]
	ands r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	movs r6, #0
	ldrsh r2, [r0, r6]
	movs r1, #0xa0
	lsls r1, r1, #2
	adds r0, r2, #0
	bl sub_80008FC
	adds r2, r0, #0
	ldr r0, [r4, #0x50]
	adds r0, r0, r2
	str r0, [r4, #4]
	b _08011106
	.align 2, 0
_080110FC: .4byte gStaticData_0816A820
_08011100:
	adds r0, r4, #0
	bl sub_8011248
_08011106:
	adds r0, r4, #0
	bl sub_8008364
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8011114
sub_8011114: @ 0x08011114
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	adds r4, r1, #0
	adds r5, r2, #0
	lsls r6, r6, #0x10
	lsrs r6, r6, #0x10
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	movs r0, #0x54
	bl sub_8026EDC
	adds r7, r0, #0
	bl sub_80084A4
	ldr r0, _080111B0 @ =gStaticData_087E40DC
	str r0, [r7, #0x18]
	adds r0, r7, #0
	bl sub_8011308
	movs r0, #0
	mov r8, r0
	strh r6, [r7, #8]
	lsls r4, r4, #8
	str r4, [r7]
	lsls r5, r5, #8
	str r5, [r7, #4]
	ldr r0, [r7]
	ldr r1, [r7, #4]
	str r0, [r7, #0x4c]
	str r1, [r7, #0x50]
	ldr r0, _080111B4 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r7, #0
	bl sub_8008E94
	movs r3, #0
	ldr r0, [r7, #0x20]
	adds r2, r7, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r4, [r2]
	lsls r0, r4, #3
	subs r0, r0, r4
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _0801117E
	subs r3, r0, #1
_0801117E:
	str r3, [r7, #0x30]
	adds r0, r7, #0
	adds r0, #0x28
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r2, [r0]
	ands r1, r2
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	strb r1, [r0]
	adds r0, #0x21
	mov r4, r8
	strb r4, [r0]
	adds r0, #1
	strb r4, [r0]
	adds r0, #1
	strb r4, [r0]
	adds r0, r7, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080111B0: .4byte gStaticData_087E40DC
_080111B4: .4byte gUnknown_030012EC

	thumb_func_start sub_80111B8
sub_80111B8: @ 0x080111B8
	push {r4, r5, lr}
	sub sp, #0xc
	adds r5, r0, #0
	ldr r0, _08011238 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #7
	bl PlaySfx
	adds r0, r5, #0
	adds r0, #0x48
	movs r2, #1
	strb r2, [r0]
	adds r0, #2
	ldrb r0, [r0]
	lsls r1, r0, #8
	ldr r0, [r5]
	subs r0, r0, r1
	str r0, [r5]
	adds r0, r5, #0
	adds r0, #0x25
	strb r2, [r0]
	ldr r1, [r5]
	asrs r1, r1, #8
	ldr r2, [r5, #4]
	asrs r2, r2, #8
	add r0, sp, #8
	str r0, [sp]
	adds r0, r5, #0
	add r3, sp, #4
	bl sub_8007174
	ldr r0, [sp, #4]
	lsls r0, r0, #8
	str r0, [r5]
	ldr r1, _0801123C @ =0xFFFF4C00
	adds r0, r0, r1
	movs r4, #0xa0
	lsls r4, r4, #5
	adds r1, r4, #0
	bl sub_80008F0
	rsbs r0, r0, #0
	str r0, [r5, #0x40]
	ldr r0, [sp, #8]
	lsls r0, r0, #8
	str r0, [r5, #4]
	ldr r1, _08011240 @ =0xFFFFF400
	adds r0, r0, r1
	adds r1, r4, #0
	bl sub_80008F0
	rsbs r0, r0, #0
	str r0, [r5, #0x44]
	ldr r0, _08011244 @ =gUnknown_03001318
	ldr r0, [r0]
	bl sub_80284A4
	add sp, #0xc
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08011238: .4byte gUnknown_030012BC
_0801123C: .4byte 0xFFFF4C00
_08011240: .4byte 0xFFFFF400
_08011244: .4byte gUnknown_03001318

