.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8024810
sub_8024810: @ 0x08024810
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8024804
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1

	thumb_func_start sub_8024820
sub_8024820: @ 0x08024820
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	adds r4, r0, #0
	ldr r1, [r4, #0x18]
	ldr r2, [r4, #0x14]
	movs r3, #0x8c
	lsls r3, r3, #1
	adds r0, r2, r3
	str r1, [r0]
	ldr r0, [r4, #0x24]
	adds r3, #4
	adds r1, r2, r3
	ldr r1, [r1]
	bl sub_8037E54
	str r0, [sp, #4]
	movs r0, #0
	mov r8, r0
	b _08024924
_0802484E:
	movs r7, #1
	adds r0, r4, #0
	mov r1, r8
	bl sub_8024708
	ldr r1, _080248A4 @ =gUnknown_03001300
	ldr r0, [r1]
	bl sub_8006A90
	ldr r2, _080248A4 @ =gUnknown_03001300
	ldr r0, [r2]
	bl sub_8006A48
	bl sub_80006A8
	ldr r3, _080248A4 @ =gUnknown_03001300
	ldr r0, [r3]
	bl sub_8006AAC
	adds r0, r4, #0
	mov r1, r8
	bl sub_8024590
	ldr r0, [r4, #0x10]
	mov r2, r8
	lsls r1, r2, #3
	adds r0, r1, r0
	ldr r0, [r0, #4]
	mov sb, r1
	cmp r0, #0
	bne _080248A8
	ldr r1, [r4]
	lsls r0, r2, #2
	adds r0, r0, r1
	ldr r1, [r0]
	ldr r0, [r1, #4]
	ldrb r1, [r1, #0x10]
	movs r2, #9
	bl sub_80010E0
	lsls r0, r0, #0x18
	lsrs r7, r0, #0x18
	b _0802490C
	.align 2, 0
_080248A4: .4byte gUnknown_03001300
_080248A8:
	movs r2, #0
	cmp r2, r0
	bge _0802490C
_080248AE:
	ldr r0, [r4, #0x10]
	add r0, sb
	ldr r1, [r0]
	lsls r0, r2, #2
	adds r0, r0, r1
	ldr r5, [r0]
	movs r6, #0
	ldrb r0, [r5]
	adds r2, #1
	mov sl, r2
	b _080248F4
_080248C4:
	adds r0, r5, r6
	ldr r1, [r4, #0x14]
	movs r2, #1
	str r2, [sp]
	adds r2, r4, #0
	adds r2, #0x18
	ldr r3, [sp, #4]
	bl sub_8000EE4
	adds r6, r6, r0
	ldr r1, [r4]
	mov r3, r8
	lsls r0, r3, #2
	adds r0, r0, r1
	ldr r1, [r0]
	ldr r0, [r1, #4]
	ldrb r1, [r1, #0x10]
	movs r2, #9
	bl sub_80010E0
	lsls r0, r0, #0x18
	lsrs r7, r0, #0x18
	adds r0, r5, r6
	ldrb r0, [r0]
_080248F4:
	cmp r0, #0
	beq _080248FC
	cmp r7, #1
	beq _080248C4
_080248FC:
	mov r2, sl
	ldr r0, [r4, #0x10]
	add r0, sb
	ldr r0, [r0, #4]
	cmp r2, r0
	bge _0802490C
	cmp r7, #1
	beq _080248AE
_0802490C:
	adds r0, r4, #0
	mov r1, r8
	bl sub_8024790
	adds r0, r4, #0
	mov r1, r8
	adds r2, r7, #0
	bl sub_80246D8
	mov r8, r0
	movs r0, #1
	add r8, r0
_08024924:
	ldr r0, [r4, #4]
	cmp r8, r0
	blt _0802484E
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_802493C
sub_802493C: @ 0x0802493C
	push {lr}
	bl sub_80247EC
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8024948
sub_8024948: @ 0x08024948
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8024810
	movs r0, #0
	str r0, [r4, #0x10]
	str r0, [r4, #0x14]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8024960
sub_8024960: @ 0x08024960
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r7, r2, #0
	ldr r4, [r0, #4]
	lsls r1, r1, #1
	adds r1, r1, r4
	ldrh r1, [r1]
	lsls r0, r1, #2
	adds r4, r4, r0
	movs r0, #0x7f
	mov r8, r0
	movs r6, #0
_0802497C:
	ldrh r1, [r4]
	ldrb r3, [r4]
	adds r4, #2
	movs r0, #0x80
	lsls r0, r0, #8
	ands r0, r1
	cmp r0, #0
	beq _080249B6
	ldrh r2, [r4]
	adds r4, #2
	mov r0, r8
	subs r0, r0, r3
	mov r8, r0
	movs r5, #0xf
_08024998:
	asrs r0, r6, #4
	adds r1, r6, #0
	ands r1, r5
	lsls r0, r0, #6
	adds r0, r0, r1
	lsls r0, r0, #1
	adds r0, r0, r7
	strh r2, [r0]
	adds r6, #1
	subs r0, r3, #1
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	cmp r3, #0
	bne _08024998
	b _08024A8C
_080249B6:
	movs r0, #0x80
	lsls r0, r0, #7
	ands r1, r0
	cmp r1, #0
	beq _08024A64
	mov r2, r8
	subs r2, r2, r3
	mov r8, r2
	ldrh r5, [r4]
	adds r4, #2
	asrs r0, r6, #4
	movs r1, #0xf
	ands r1, r6
	lsls r0, r0, #6
	adds r0, r0, r1
	lsls r0, r0, #1
	adds r0, r0, r7
	strh r5, [r0]
	subs r0, r3, #1
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	adds r6, #1
	movs r0, #0xf
	mov ip, r0
_080249E6:
	ldrh r2, [r4]
	adds r4, #2
	lsls r1, r2, #0x18
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	asrs r1, r1, #0x18
	adds r0, r0, r1
	lsls r0, r0, #0x10
	lsrs r5, r0, #0x10
	asrs r0, r6, #4
	mov sb, r0
	adds r1, r6, #0
	mov r0, ip
	ands r1, r0
	mov r0, sb
	lsls r0, r0, #6
	mov sb, r0
	add r1, sb
	lsls r0, r1, #1
	adds r0, r0, r7
	strh r5, [r0]
	adds r6, #1
	lsls r2, r2, #0x10
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	asrs r2, r2, #0x18
	adds r0, r0, r2
	lsls r0, r0, #0x10
	lsrs r5, r0, #0x10
	asrs r0, r6, #4
	adds r1, r6, #0
	mov r2, ip
	ands r1, r2
	lsls r0, r0, #6
	adds r0, r0, r1
	lsls r0, r0, #1
	adds r0, r0, r7
	strh r5, [r0]
	adds r6, #1
	subs r0, r3, #2
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	cmp r3, #1
	bhi _080249E6
	cmp r3, #0
	beq _08024A8C
	ldrh r0, [r4]
	adds r4, #2
	lsls r0, r0, #0x18
	lsls r2, r5, #0x10
	asrs r2, r2, #0x10
	asrs r0, r0, #0x18
	adds r2, r2, r0
	asrs r0, r6, #4
	movs r1, #0xf
	ands r1, r6
	lsls r0, r0, #6
	adds r0, r0, r1
	lsls r0, r0, #1
	adds r0, r0, r7
	strh r2, [r0]
	adds r6, #1
	b _08024A8C
_08024A64:
	mov r0, r8
	subs r0, r0, r3
	mov r8, r0
	movs r2, #0xf
_08024A6C:
	asrs r0, r6, #4
	lsls r1, r0, #6
	adds r0, r6, #0
	ands r0, r2
	adds r0, r1, r0
	lsls r0, r0, #1
	adds r0, r0, r7
	ldrh r1, [r4]
	strh r1, [r0]
	adds r4, #2
	adds r6, #1
	subs r0, r3, #1
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	cmp r3, #0
	bne _08024A6C
_08024A8C:
	mov r2, r8
	cmp r2, #0
	blt _08024A94
	b _0802497C
_08024A94:
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8024AA0
sub_8024AA0: @ 0x08024AA0
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r0, [r1]
	asrs r6, r0, #7
	ldr r0, [r1, #4]
	asrs r7, r0, #6
	ldr r2, [r4, #0xc]
	ldr r5, [r4, #0x10]
	cmp r6, r2
	ble _08024AC8
	adds r1, r2, #4
	adds r0, r4, #0
	bl sub_8024C08
	ldrb r0, [r4, #0x14]
	adds r0, #1
	movs r1, #3
	ands r0, r1
	strb r0, [r4, #0x14]
	b _08024ADE
_08024AC8:
	cmp r6, r2
	bge _08024ADE
	ldrb r0, [r4, #0x14]
	subs r0, #1
	movs r1, #3
	ands r0, r1
	strb r0, [r4, #0x14]
	subs r1, r2, #1
	adds r0, r4, #0
	bl sub_8024C08
_08024ADE:
	str r6, [r4, #0xc]
	cmp r7, r5
	ble _08024AF8
	adds r1, r5, #4
	adds r0, r4, #0
	bl sub_8024BAC
	ldrb r0, [r4, #0x15]
	adds r0, #1
	movs r1, #3
	ands r0, r1
	strb r0, [r4, #0x15]
	b _08024B0E
_08024AF8:
	cmp r7, r5
	bge _08024B0E
	ldrb r0, [r4, #0x15]
	subs r0, #1
	movs r1, #3
	ands r0, r1
	strb r0, [r4, #0x15]
	subs r1, r5, #1
	adds r0, r4, #0
	bl sub_8024BAC
_08024B0E:
	str r7, [r4, #0x10]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8024B18
sub_8024B18: @ 0x08024B18
	push {r4, r5, lr}
	ldr r4, [r0, #0xc]
	lsls r4, r4, #4
	subs r1, r1, r4
	ldr r4, [r0, #0x10]
	lsls r4, r4, #3
	subs r2, r2, r4
	ldrb r5, [r0, #0x14]
	lsls r4, r5, #4
	adds r1, r1, r4
	ldrb r5, [r0, #0x15]
	lsls r4, r5, #3
	adds r2, r2, r4
	movs r4, #0x3f
	ands r1, r4
	movs r4, #0x1f
	ands r2, r4
	ldr r0, [r0, #8]
	lsls r1, r1, #1
	adds r0, r0, r1
	str r2, [r3]
	pop {r4, r5}
	pop {r1}
	bx r1

	thumb_func_start sub_8024B48
sub_8024B48: @ 0x08024B48
	push {r4, r5, lr}
	ldr r4, [r0, #0xc]
	lsls r4, r4, #4
	subs r1, r1, r4
	ldr r4, [r0, #0x10]
	lsls r4, r4, #3
	subs r2, r2, r4
	ldrb r5, [r0, #0x14]
	lsls r4, r5, #4
	adds r1, r1, r4
	ldrb r5, [r0, #0x15]
	lsls r4, r5, #3
	adds r2, r2, r4
	movs r4, #0x3f
	ands r1, r4
	movs r4, #0x1f
	ands r2, r4
	ldr r0, [r0, #8]
	lsls r2, r2, #7
	adds r0, r0, r2
	str r1, [r3]
	pop {r4, r5}
	pop {r1}
	bx r1

	thumb_func_start sub_8024B78
sub_8024B78: @ 0x08024B78
	push {r4, lr}
	ldr r3, [r0, #0xc]
	lsls r3, r3, #4
	subs r1, r1, r3
	ldr r3, [r0, #0x10]
	lsls r3, r3, #3
	subs r2, r2, r3
	ldrb r4, [r0, #0x14]
	lsls r3, r4, #4
	adds r1, r1, r3
	ldrb r4, [r0, #0x15]
	lsls r3, r4, #3
	adds r2, r2, r3
	movs r3, #0x3f
	ands r1, r3
	movs r3, #0x1f
	ands r2, r3
	ldr r0, [r0, #8]
	lsls r2, r2, #6
	adds r2, r2, r1
	lsls r2, r2, #1
	adds r0, r0, r2
	ldrh r0, [r0]
	pop {r4}
	pop {r1}
	bx r1

	thumb_func_start sub_8024BAC
sub_8024BAC: @ 0x08024BAC
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r0, [r4]
	ldrh r2, [r0, #0x18]
	cmp r1, r2
	bge _08024C00
	ldrh r0, [r0, #0x16]
	adds r5, r0, #0
	muls r5, r1, r5
	ldr r0, [r4, #0xc]
	adds r5, r5, r0
	movs r6, #0
	movs r7, #3
_08024BC6:
	ldr r0, [r4, #0xc]
	adds r0, r6, r0
	ldr r1, [r4]
	ldrh r3, [r1, #0x16]
	cmp r0, r3
	bge _08024BFA
	ldr r2, [r4, #8]
	ldrb r0, [r4, #0x15]
	adds r0, #4
	ands r0, r7
	lsls r0, r0, #0xa
	adds r2, r2, r0
	ldrb r3, [r4, #0x14]
	adds r0, r3, r6
	adds r0, #4
	ands r0, r7
	lsls r0, r0, #5
	adds r2, r2, r0
	ldr r1, [r1]
	lsls r0, r5, #1
	adds r0, r0, r1
	ldrh r1, [r0]
	adds r5, #1
	adds r0, r4, #0
	bl sub_8024960
_08024BFA:
	adds r6, #1
	cmp r6, #3
	ble _08024BC6
_08024C00:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8024C08
sub_8024C08: @ 0x08024C08
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r0, [r4]
	ldrh r2, [r0, #0x16]
	cmp r1, r2
	bge _08024C5C
	ldr r0, [r4, #0x10]
	adds r5, r0, #0
	muls r5, r2, r5
	adds r5, r5, r1
	movs r6, #0
	movs r7, #3
_08024C20:
	ldr r0, [r4, #0x10]
	adds r0, r6, r0
	ldr r3, [r4]
	ldrh r1, [r3, #0x18]
	cmp r0, r1
	bge _08024C56
	ldr r2, [r4, #8]
	ldrb r1, [r4, #0x15]
	adds r0, r1, r6
	adds r0, #4
	ands r0, r7
	lsls r0, r0, #0xa
	adds r2, r2, r0
	ldrb r0, [r4, #0x14]
	adds r0, #4
	ands r0, r7
	lsls r0, r0, #5
	adds r2, r2, r0
	ldr r1, [r3]
	lsls r0, r5, #1
	adds r0, r0, r1
	ldrh r1, [r0]
	ldrh r3, [r3, #0x16]
	adds r5, r3, r5
	adds r0, r4, #0
	bl sub_8024960
_08024C56:
	adds r6, #1
	cmp r6, #3
	ble _08024C20
_08024C5C:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8024C64
sub_8024C64: @ 0x08024C64
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	adds r4, r0, #0
	movs r0, #0x40
	mov sl, r0
	movs r2, #0x20
	mov sb, r2
	movs r0, #0
	strb r0, [r4, #0x14]
	strb r0, [r4, #0x15]
	ldr r0, [r1]
	asrs r0, r0, #7
	str r0, [r4, #0xc]
	ldr r0, [r1, #4]
	asrs r0, r0, #6
	str r0, [r4, #0x10]
	movs r6, #0
_08024C8C:
	ldr r0, [r4, #0x10]
	adds r0, r6, r0
	ldr r1, [r4]
	adds r3, r6, #1
	mov r8, r3
	ldrh r1, [r1, #0x18]
	cmp r0, r1
	bge _08024CDA
	movs r5, #0
	lsls r0, r6, #3
	mov r7, sl
	muls r7, r0, r7
_08024CA4:
	ldr r0, [r4, #0xc]
	adds r3, r5, r0
	ldr r1, [r4]
	ldrh r2, [r1, #0x16]
	cmp r3, r2
	bge _08024CD4
	ldr r0, [r4, #0x10]
	adds r0, r0, r6
	muls r0, r2, r0
	adds r0, r0, r3
	ldr r1, [r1]
	lsls r0, r0, #1
	adds r0, r0, r1
	ldrh r1, [r0]
	ldr r2, [r4, #8]
	mov r3, sb
	asrs r0, r3, #1
	muls r0, r5, r0
	adds r0, r7, r0
	lsls r0, r0, #1
	adds r2, r2, r0
	adds r0, r4, #0
	bl sub_8024960
_08024CD4:
	adds r5, #1
	cmp r5, #3
	ble _08024CA4
_08024CDA:
	mov r6, r8
	cmp r6, #3
	ble _08024C8C
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8024CF0
sub_8024CF0: @ 0x08024CF0
	str r1, [r0]
	ldrh r2, [r1, #0x1a]
	ldrh r3, [r1, #0x1c]
	str r2, [r0, #0x18]
	str r3, [r0, #0x1c]
	ldr r2, _08024D08 @ =gUnknown_03001308
	ldr r2, [r2]
	ldr r2, [r2, #0x24]
	ldr r1, [r1, #4]
	adds r2, r2, r1
	str r2, [r0, #4]
	bx lr
	.align 2, 0
_08024D08: .4byte gUnknown_03001308

	thumb_func_start sub_8024D0C
sub_8024D0C: @ 0x08024D0C
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, _08024D34 @ =gStaticData_087E4BDC
	str r0, [r4, #0x20]
	ldr r0, [r4, #8]
	cmp r0, #0
	beq _08024D20
	bl sub_8026EB4
_08024D20:
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _08024D2E
	adds r0, r4, #0
	bl sub_8026ED0
_08024D2E:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08024D34: .4byte gStaticData_087E4BDC

	thumb_func_start sub_8024D38
sub_8024D38: @ 0x08024D38
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, _08024D54 @ =gStaticData_087E4BDC
	str r0, [r4, #0x20]
	movs r0, #0x80
	lsls r0, r0, #5
	bl sub_8026EC0
	str r0, [r4, #8]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08024D54: .4byte gStaticData_087E4BDC

	thumb_func_start sub_8024D58
sub_8024D58: @ 0x08024D58
	ldr r0, [r0, #0x1c]
	bx lr

	thumb_func_start sub_8024D5C
sub_8024D5C: @ 0x08024D5C
	ldr r0, [r0, #0x18]
	bx lr

	thumb_func_start sub_8024D60
sub_8024D60: @ 0x08024D60
	ldr r2, [r1, #4]
	ldr r1, [r1]
	str r1, [r0, #0x18]
	str r2, [r0, #0x1c]
	bx lr
	.align 2, 0

	thumb_func_start sub_8024D6C
sub_8024D6C: @ 0x08024D6C
	str r1, [r0, #0x18]
	str r2, [r0, #0x1c]
	bx lr
	.align 2, 0

	thumb_func_start sub_8024D74
sub_8024D74: @ 0x08024D74
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, _08024DA8 @ =gStaticData_087E4BEC
	str r0, [r4, #0x30]
	ldr r2, [r4, #0x2c]
	cmp r2, #0
	beq _08024D94
	ldr r1, [r2, #0x20]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_08024D94:
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _08024DA2
	adds r0, r4, #0
	bl sub_8026ED0
_08024DA2:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08024DA8: .4byte gStaticData_087E4BEC

	thumb_func_start sub_8024DAC
sub_8024DAC: @ 0x08024DAC
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, _08024DC8 @ =gStaticData_087E4BEC
	str r0, [r4, #0x30]
	movs r0, #0x24
	bl sub_8026EDC
	bl sub_8024D38
	str r0, [r4, #0x2c]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08024DC8: .4byte gStaticData_087E4BEC

	thumb_func_start sub_8024DCC
sub_8024DCC: @ 0x08024DCC
	adds r0, r1, #0
	movs r1, #0x10
	rsbs r1, r1, #0
	cmp r0, r1
	bge _08024DD8
	adds r0, r1, #0
_08024DD8:
	cmp r0, #0x10
	ble _08024DDE
	movs r0, #0x10
_08024DDE:
	bx lr

	thumb_func_start sub_8024DE0
sub_8024DE0: @ 0x08024DE0
	ldr r2, [r0, #8]
	ldr r3, [r1]
	cmp r2, r3
	ble _08024DEA
	adds r2, r3, #0
_08024DEA:
	str r2, [r1]
	ldr r0, [r0, #0xc]
	ldr r2, [r1, #4]
	cmp r0, r2
	ble _08024DF6
	adds r0, r2, #0
_08024DF6:
	str r0, [r1, #4]
	bx lr
	.align 2, 0

	thumb_func_start sub_8024DFC
sub_8024DFC: @ 0x08024DFC
	adds r3, r0, #0
	adds r2, r1, #0
	ldr r1, [r2]
	ldr r0, [r3, #0x20]
	muls r0, r1, r0
	cmp r0, #0
	bge _08024E0C
	adds r0, #0xff
_08024E0C:
	asrs r0, r0, #8
	str r0, [r2]
	ldr r1, [r2, #4]
	ldr r0, [r3, #0x24]
	muls r0, r1, r0
	cmp r0, #0
	bge _08024E1C
	adds r0, #0xff
_08024E1C:
	asrs r0, r0, #8
	str r0, [r2, #4]
	bx lr
	.align 2, 0

	thumb_func_start sub_8024E24
sub_8024E24: @ 0x08024E24
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r3, [r4, #0x30]
	movs r1, #0x20
	ldrsh r0, [r3, r1]
	adds r0, r4, r0
	ldr r1, [r5]
	ldr r2, [r4]
	subs r1, r1, r2
	ldr r2, [r3, #0x24]
	bl sub_803AD80
	adds r6, r0, #0
	ldr r3, [r4, #0x30]
	movs r1, #0x20
	ldrsh r0, [r3, r1]
	adds r0, r4, r0
	ldr r1, [r5, #4]
	ldr r2, [r4, #4]
	subs r1, r1, r2
	ldr r2, [r3, #0x24]
	bl sub_803AD80
	ldr r1, [r4]
	adds r1, r1, r6
	str r1, [r4]
	ldr r1, [r4, #4]
	adds r1, r1, r0
	str r1, [r4, #4]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

