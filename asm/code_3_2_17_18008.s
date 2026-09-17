.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8018008
sub_8018008: @ 0x08018008
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #0x20
	adds r7, r0, #0
	mov r8, r1
	ldr r2, [r7, #0x24]
	movs r5, #1
	rsbs r5, r5, #0
	cmp r2, r5
	beq _08018070
	ldr r0, _080180B8 @ =gUnknown_030012EC
	ldr r0, [r0]
	ldr r1, [r0, #0xc]
	lsls r0, r2, #2
	adds r0, r0, r1
	ldr r4, [r0]
	ldr r2, [r4, #0x44]
	cmp r2, #0
	beq _08018044
	ldr r1, [r2, #0xc]
	adds r1, #0x48
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_08018044:
	movs r0, #0x10
	bl sub_8026EDC
	bl sub_801886C
	str r0, [r4, #0x44]
	ldr r2, [r0, #0xc]
	movs r6, #0x18
	ldrsh r1, [r2, r6]
	adds r0, r0, r1
	ldr r2, [r2, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	str r5, [r7, #0x24]
	ldr r0, _080180BC @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x39
	bl PlaySfx
_08018070:
	ldr r0, [r7, #8]
	cmp r0, #8
	bne _080180C4
	ldr r5, _080180C0 @ =gUnknown_030012D8
	ldr r1, [r5]
	mov r0, sp
	bl sub_8007C30
	add r4, sp, #0x10
	adds r0, r4, #0
	mov r1, r8
	bl sub_8007CF8
	ldr r0, [sp, #8]
	cmp r0, #0
	beq _08018130
	ldr r0, [sp, #0x18]
	cmp r0, #0
	beq _08018130
	adds r0, r4, #0
	mov r1, sp
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08018130
	ldr r0, [r5]
	ldrb r0, [r0, #0xa]
	cmp r0, #0x13
	bne _08018130
	adds r0, r7, #0
	mov r1, r8
	movs r2, #9
	bl sub_8018400
	b _08018130
	.align 2, 0
_080180B8: .4byte gUnknown_030012EC
_080180BC: .4byte gUnknown_030012BC
_080180C0: .4byte gUnknown_030012D8
_080180C4:
	ldr r5, _08018144 @ =gUnknown_030012D8
	ldr r1, [r5]
	movs r2, #0x82
	lsls r2, r2, #1
	adds r0, r1, r2
	ldrb r0, [r0]
	cmp r0, #0
	bne _08018130
	mov r0, sp
	bl sub_8007CF8
	ldr r0, [sp, #8]
	add r4, sp, #0x10
	cmp r0, #0
	bne _080180F6
	ldr r1, [r5]
	adds r0, r4, #0
	bl sub_8007C30
	mov r1, sp
	adds r0, r4, #0
	ldm r0!, {r2, r3, r6}
	stm r1!, {r2, r3, r6}
	ldr r0, [r0]
	str r0, [r1]
_080180F6:
	adds r0, r4, #0
	mov r1, r8
	bl sub_8007C30
	ldr r0, [sp, #0x18]
	cmp r0, #0
	beq _08018130
	ldr r0, [sp, #8]
	cmp r0, #0
	beq _08018130
	adds r0, r4, #0
	mov r1, sp
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08018130
	ldr r0, [r5]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #0
	bl sub_803AD88
_08018130:
	ldr r6, [r7, #8]
	mov sb, r6
	cmp r6, #0xf
	bls _0801813A
	b _080183EE
_0801813A:
	lsls r0, r6, #2
	ldr r1, _08018148 @ =_0801814C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08018144: .4byte gUnknown_030012D8
_08018148: .4byte _0801814C
_0801814C: @ jump table
	.4byte _0801818C @ case 0
	.4byte _080181A6 @ case 1
	.4byte _080181A6 @ case 2
	.4byte _0801826C @ case 3
	.4byte _08018388 @ case 4
	.4byte _08018374 @ case 5
	.4byte _0801831C @ case 6
	.4byte _0801826C @ case 7
	.4byte _08018340 @ case 8
	.4byte _080183BC @ case 9
	.4byte _0801826C @ case 10
	.4byte _080181A6 @ case 11
	.4byte _0801839E @ case 12
	.4byte _080182EC @ case 13
	.4byte _0801826C @ case 14
	.4byte _080181A6 @ case 15
_0801818C:
	movs r0, #5
	rsbs r0, r0, #0
	mov r1, r8
	ldrb r1, [r1, #0xc]
	ands r0, r1
	mov r2, r8
	strb r0, [r2, #0xc]
	movs r0, #1
	movs r1, #0
	strb r0, [r2, #0xa]
	str r0, [r7, #0x28]
	str r1, [r7, #0x2c]
	b _080182C2
_080181A6:
	ldr r0, [r7, #0x38]
	subs r6, r0, #1
	str r6, [r7, #0x38]
	ldr r0, [r7, #0x44]
	muls r0, r6, r0
	ldr r5, [r7, #0x3c]
	adds r1, r5, #0
	bl sub_803ADB4
	adds r4, r0, #0
	ldr r0, [r7, #0x30]
	adds r4, r4, r0
	lsls r0, r6, #8
	adds r1, r5, #0
	bl sub_803ADB4
	movs r5, #0x80
	lsls r5, r5, #1
	subs r0, r5, r0
	ldr r1, [r7, #0x48]
	lsls r0, r0, #1
	adds r0, r0, r1
	movs r3, #0
	ldrsh r0, [r0, r3]
	subs r0, r5, r0
	ldr r1, [r7, #0x40]
	muls r0, r1, r0
	asrs r0, r0, #8
	ldr r1, [r7, #0x34]
	adds r0, r0, r1
	mov r1, r8
	str r4, [r1]
	str r0, [r1, #4]
	cmp r6, #0
	beq _080181EE
	b _080183EE
_080181EE:
	ldr r4, _08018220 @ =gUnknown_030012BC
	ldr r0, [r4]
	movs r1, #0x2a
	adds r2, r5, #0
	bl PlaySfx
	ldr r0, [r7, #8]
	cmp r0, #0xf
	bne _08018228
	ldr r0, _08018224 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231B4
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08018212
	bl sub_80241A4
_08018212:
	adds r0, r7, #0
	mov r1, r8
	movs r2, #0x10
	bl sub_8018400
	b _080183EE
	.align 2, 0
_08018220: .4byte gUnknown_030012BC
_08018224: .4byte gUnknown_030012C0
_08018228:
	cmp r0, #1
	bne _08018238
	adds r0, r7, #0
	mov r1, r8
	movs r2, #6
	bl sub_8018400
	b _080183EE
_08018238:
	cmp r0, #0xb
	bne _08018256
	str r6, [r7, #0x2c]
	ldr r1, [r7, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r7, r0
	ldr r3, [r1, #4]
	mov r1, r8
	movs r2, #3
	bl sub_803AD84
	movs r0, #0xc
	b _08018366
_08018256:
	ldr r0, [r4]
	movs r1, #0x3d
	adds r2, r5, #0
	bl PlaySfx
	adds r0, r7, #0
	mov r1, r8
	movs r2, #8
	bl sub_8018400
	b _080183EE
_0801826C:
	ldr r4, [r7, #0x38]
	subs r4, #1
	str r4, [r7, #0x38]
	ldr r0, [r7, #0x44]
	muls r0, r4, r0
	ldr r6, [r7, #0x3c]
	adds r1, r6, #0
	bl sub_803ADB4
	adds r5, r0, #0
	ldr r0, [r7, #0x30]
	adds r5, r5, r0
	lsls r0, r4, #8
	adds r1, r6, #0
	bl sub_803ADB4
	ldr r1, [r7, #0x48]
	lsls r0, r0, #1
	adds r0, r0, r1
	movs r3, #0
	ldrsh r1, [r0, r3]
	ldr r0, [r7, #0x40]
	muls r0, r1, r0
	asrs r0, r0, #8
	ldr r1, [r7, #0x34]
	adds r0, r0, r1
	mov r6, r8
	str r5, [r6]
	str r0, [r6, #4]
	cmp r4, #0
	beq _080182AC
	b _080183EE
_080182AC:
	mov r0, sb
	cmp r0, #0xe
	bne _080182BE
	adds r0, r7, #0
	mov r1, r8
	movs r2, #0xf
	bl sub_8018400
	b _080183EE
_080182BE:
	cmp r0, #3
	bne _080182CE
_080182C2:
	adds r0, r7, #0
	mov r1, r8
	movs r2, #1
	bl sub_8018400
	b _080183EE
_080182CE:
	mov r0, sb
	cmp r0, #7
	bne _080182E0
	adds r0, r7, #0
	mov r1, r8
	movs r2, #2
	bl sub_8018400
	b _080183EE
_080182E0:
	adds r0, r7, #0
	mov r1, r8
	movs r2, #0xd
	bl sub_8018400
	b _080183EE
_080182EC:
	ldr r0, [r7, #0x1c]
	cmp r0, #0
	bne _08018314
	ldr r0, [r7, #0x20]
	subs r2, r0, #1
	str r2, [r7, #0x20]
	cmp r2, #0
	bne _08018308
	adds r0, r7, #0
	mov r1, r8
	movs r2, #0xb
	bl sub_8018400
	b _08018314
_08018308:
	movs r0, #0x46
	str r0, [r7, #0x1c]
	adds r0, r7, #0
	mov r1, r8
	bl sub_80186F0
_08018314:
	ldr r0, [r7, #0x1c]
	subs r0, #1
	str r0, [r7, #0x1c]
	b _080183EE
_0801831C:
	mov r0, r8
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _080183EE
	ldr r0, [r7, #0x2c]
	adds r0, #1
	str r0, [r7, #0x2c]
	cmp r0, #3
	ble _080183AA
	movs r0, #0
	str r0, [r7, #0x2c]
	adds r0, r7, #0
	mov r1, r8
	movs r2, #7
	bl sub_8018400
	b _080183EE
_08018340:
	ldr r0, [r7, #0x20]
	cmp r0, #0
	beq _0801834C
	subs r0, #1
	str r0, [r7, #0x20]
	b _080183EE
_0801834C:
	subs r0, #1
	str r0, [r7, #0x20]
	ldr r1, [r7, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r7, r0
	ldr r3, [r1, #4]
	mov r1, r8
	movs r2, #0
	bl sub_803AD84
	movs r0, #3
_08018366:
	str r0, [r7, #0x1c]
	adds r0, r7, #0
	mov r1, r8
	movs r2, #5
	bl sub_8018400
	b _080183EE
_08018374:
	mov r0, r8
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08018388
	ldr r2, [r7, #0x1c]
	adds r0, r7, #0
	mov r1, r8
	bl sub_8018400
_08018388:
	ldr r0, [r7, #0x20]
	subs r0, #1
	str r0, [r7, #0x20]
	cmp r0, #0
	bne _080183EE
	ldr r2, [r7, #0x1c]
	adds r0, r7, #0
	mov r1, r8
	bl sub_8018400
	b _080183EE
_0801839E:
	ldr r0, _080183B8 @ =gStaticData_0816C308
	ldr r1, [r7, #0x10]
	subs r1, #1
	adds r1, r1, r0
	ldrb r0, [r1]
	str r0, [r7, #0x24]
_080183AA:
	adds r0, r7, #0
	mov r1, r8
	movs r2, #3
	bl sub_8018400
	b _080183EE
	.align 2, 0
_080183B8: .4byte gStaticData_0816C308
_080183BC:
	ldr r0, [r7, #0x10]
	cmp r0, #2
	ble _080183CC
	adds r0, r7, #0
	mov r1, r8
	movs r2, #0xe
	bl sub_8018400
_080183CC:
	mov r0, r8
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _080183EE
	adds r0, r7, #0
	mov r1, r8
	movs r2, #0xa
	bl sub_8018400
	ldr r0, _080183FC @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xd
	bl PlaySfx
_080183EE:
	add sp, #0x20
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080183FC: .4byte gUnknown_030012BC

	thumb_func_start sub_8018400
sub_8018400: @ 0x08018400
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r7, r2, #0
	subs r0, r7, #1
	cmp r0, #0xe
	bls _08018410
	b _08018642
_08018410:
	lsls r0, r0, #2
	ldr r1, _0801841C @ =_08018420
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801841C: .4byte _08018420
_08018420: @ jump table
	.4byte _08018470 @ case 0
	.4byte _08018470 @ case 1
	.4byte _080184F0 @ case 2
	.4byte _08018642 @ case 3
	.4byte _08018642 @ case 4
	.4byte _08018642 @ case 5
	.4byte _080184F0 @ case 6
	.4byte _08018568 @ case 7
	.4byte _08018582 @ case 8
	.4byte _08018550 @ case 9
	.4byte _08018464 @ case 10
	.4byte _08018642 @ case 11
	.4byte _0801845C @ case 12
	.4byte _080185DC @ case 13
	.4byte _0801861C @ case 14
_0801845C:
	movs r0, #0
	str r0, [r4, #0x1c]
	movs r0, #4
	str r0, [r4, #0x20]
_08018464:
	ldr r0, _0801848C @ =gStaticData_0816C308
	ldr r1, [r4, #0x10]
	subs r1, #1
	adds r1, r1, r0
	ldrb r0, [r1]
	str r0, [r4, #0x28]
_08018470:
	cmp r7, #1
	bne _08018490
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r5, #0
	movs r2, #2
	bl sub_803AD84
	b _080184A4
	.align 2, 0
_0801848C: .4byte gStaticData_0816C308
_08018490:
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r6, #0
	ldrsh r0, [r1, r6]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r5, #0
	movs r2, #1
	bl sub_803AD84
_080184A4:
	movs r3, #4
	ldr r0, [r5, #0x20]
	adds r2, r5, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r6, [r2]
	lsls r0, r6, #3
	subs r0, r0, r6
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _080184C0
	subs r3, r0, #1
_080184C0:
	str r3, [r5, #0x30]
	ldr r0, _080184E8 @ =gUnknown_030012EC
	ldr r1, [r0]
	ldr r0, [r4, #0x28]
	ldr r1, [r1, #0xc]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r2, [r0]
	ldr r1, [r2]
	str r1, [r4, #0x30]
	cmp r7, #0xb
	beq _080184DC
	cmp r7, #0xd
	bne _080184DE
_080184DC:
	str r1, [r5]
_080184DE:
	ldr r0, [r2, #4]
	ldr r1, _080184EC @ =0xFFFFDC00
	adds r0, r0, r1
	str r0, [r4, #0x34]
	b _08018558
	.align 2, 0
_080184E8: .4byte gUnknown_030012EC
_080184EC: .4byte 0xFFFFDC00
_080184F0:
	adds r0, r4, #0
	bl sub_801865C
	str r0, [r4, #0x28]
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r5, #0
	movs r2, #4
	bl sub_803AD84
	ldr r0, _08018538 @ =gUnknown_030012EC
	ldr r1, [r0]
	ldr r0, [r4, #0x28]
	ldr r1, [r1, #0xc]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r1, [r0]
	ldr r0, [r1]
	ldr r3, [r1, #4]
	ldr r6, _0801853C @ =0xFFFFDC00
	adds r2, r3, r6
	ldr r1, [r5]
	adds r0, r0, r1
	asrs r0, r0, #1
	str r0, [r4, #0x30]
	ldr r0, [r5, #4]
	cmp r2, r0
	blt _08018544
	ldr r1, _08018540 @ =0xFFFFA700
	adds r2, r0, r1
	b _08018548
	.align 2, 0
_08018538: .4byte gUnknown_030012EC
_0801853C: .4byte 0xFFFFDC00
_08018540: .4byte 0xFFFFA700
_08018544:
	ldr r6, _0801854C @ =0xFFFF8300
	adds r2, r3, r6
_08018548:
	str r2, [r4, #0x34]
	b _08018558
	.align 2, 0
_0801854C: .4byte 0xFFFF8300
_08018550:
	ldr r0, _08018564 @ =0xFFFFD000
	str r0, [r4, #0x34]
	ldr r0, [r5]
	str r0, [r4, #0x30]
_08018558:
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_8018978
	b _08018642
	.align 2, 0
_08018564: .4byte 0xFFFFD000
_08018568:
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r5, #0
	movs r2, #6
	bl sub_803AD84
	movs r0, #0xb4
	str r0, [r4, #0x20]
	b _08018642
_08018582:
	ldr r0, _080185D4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x15
	bl PlaySfx
	ldr r0, [r4, #0x10]
	adds r0, #1
	str r0, [r4, #0x10]
	cmp r0, #2
	ble _080185B4
	ldr r1, [r5]
	ldr r0, [r5, #4]
	ldr r6, _080185D8 @ =0xFFFF9C00
	adds r0, r0, r6
	str r0, [r4, #0x34]
	movs r0, #0xc8
	lsls r0, r0, #7
	adds r1, r1, r0
	str r1, [r4, #0x30]
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_8018978
_080185B4:
	adds r0, r4, #0
	adds r1, r5, #0
	bl nullsub_19
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r5, #0
	movs r2, #7
	bl sub_803AD84
	b _08018642
	.align 2, 0
_080185D4: .4byte gUnknown_030012BC
_080185D8: .4byte 0xFFFF9C00
_080185DC:
	ldr r0, _0801860C @ =gUnknown_030012EC
	ldr r0, [r0]
	ldr r0, [r0, #0xc]
	ldr r0, [r0, #8]
	ldr r5, [r0]
	ldr r0, [r0, #4]
	ldr r1, _08018610 @ =0xFFFFE800
	adds r6, r0, r1
	ldr r0, _08018614 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231B4
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08018642
	ldr r0, _08018618 @ =0x0000FFFF
	lsls r1, r5, #8
	lsrs r1, r1, #0x10
	lsls r2, r6, #8
	lsrs r2, r2, #0x10
	movs r3, #0
	bl sub_8021DFC
	b _08018642
	.align 2, 0
_0801860C: .4byte gUnknown_030012EC
_08018610: .4byte 0xFFFFE800
_08018614: .4byte gUnknown_030012C0
_08018618: .4byte 0x0000FFFF
_0801861C:
	ldr r1, [r5]
	str r1, [r4, #0x30]
	ldr r0, _08018658 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x14]
	lsls r0, r0, #8
	movs r2, #0x80
	lsls r2, r2, #7
	adds r0, r0, r2
	str r0, [r4, #0x34]
	movs r6, #0xc8
	lsls r6, r6, #7
	adds r1, r1, r6
	str r1, [r4, #0x30]
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_8018978
_08018642:
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	adds r1, r7, #0
	bl sub_803AD80
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08018658: .4byte gUnknown_03001308

	thumb_func_start sub_801865C
sub_801865C: @ 0x0801865C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	mov sb, r0
	movs r0, #0
	mov ip, r0
	ldr r5, _080186E0 @ =0x00FFFFFF
	movs r4, #0
	ldr r0, _080186E4 @ =gUnknown_030012EC
	ldr r1, [r0]
	ldr r2, [r1, #4]
	ldr r3, _080186E8 @ =gStaticData_0816C30B
	mov sl, r3
	cmp ip, r2
	bge _080186B6
	ldr r0, _080186EC @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r3, [r0]
	mov r8, r3
	ldr r7, [r0, #4]
	adds r6, r2, #0
	ldr r3, [r1, #0xc]
_0801868C:
	ldr r0, [r3]
	ldr r1, [r0]
	ldr r0, [r0, #4]
	mov r2, r8
	subs r1, r1, r2
	asrs r2, r1, #0x1f
	eors r1, r2
	subs r1, r1, r2
	subs r0, r0, r7
	asrs r2, r0, #0x1f
	eors r0, r2
	subs r0, r0, r2
	adds r1, r1, r0
	cmp r1, r5
	bge _080186AE
	adds r5, r1, #0
	mov ip, r4
_080186AE:
	adds r3, #4
	adds r4, #1
	cmp r4, r6
	blt _0801868C
_080186B6:
	mov r3, sb
	ldr r0, [r3, #0x28]
	lsls r1, r0, #2
	adds r1, r1, r0
	add r1, ip
	ldr r2, [r3, #0x10]
	lsls r0, r2, #1
	adds r0, r0, r2
	lsls r0, r0, #3
	adds r0, r0, r2
	adds r1, r1, r0
	add r1, sl
	ldrb r0, [r1]
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080186E0: .4byte 0x00FFFFFF
_080186E4: .4byte gUnknown_030012EC
_080186E8: .4byte gStaticData_0816C30B
_080186EC: .4byte gUnknown_030012D8

	thumb_func_start sub_80186F0
sub_80186F0: @ 0x080186F0
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	mov sb, r1
	adds r6, r2, #0
	ldr r0, _080187E4 @ =0x0000FFFF
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _080187E8 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xa5
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x20]
	movs r0, #5
	adds r1, r4, #0
	adds r1, #0x2d
	movs r2, #0
	mov r8, r2
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	movs r0, #0x10
	bl sub_8026EDC
	bl sub_80188D0
	adds r5, r0, #0
	adds r0, r4, #0
	bl sub_800815C
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
	str r5, [r4, #0x44]
	ldr r1, [r5, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r5, r5, r0
	ldr r2, [r1, #0x1c]
	adds r0, r5, #0
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #0x10
	ldrb r3, [r4, #0xc]
	orrs r0, r3
	strb r0, [r4, #0xc]
	ldr r0, _080187EC @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	movs r0, #0x80
	mov r1, r8
	str r1, [r4, #0x64]
	str r1, [r4, #0x54]
	str r0, [r4, #0x58]
	str r0, [r4, #0x5c]
	mov r2, sb
	ldr r5, [r2]
	ldr r0, _080187F0 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0]
	subs r0, r0, r5
	subs r6, #1
	muls r0, r6, r0
	movs r1, #3
	bl sub_803ADB4
	adds r5, r5, r0
	ldr r0, _080187F4 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #4]
	lsls r0, r0, #8
	str r5, [r4]
	str r0, [r4, #4]
	movs r0, #1
	strb r0, [r4, #0xa]
	subs r0, #6
	ldrb r3, [r4, #0xc]
	ands r0, r3
	movs r1, #0x41
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _080187F8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x13
	bl PlaySfx
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_080187E4: .4byte 0x0000FFFF
_080187E8: .4byte gUnknown_030012D0
_080187EC: .4byte gUnknown_030012F0
_080187F0: .4byte gUnknown_030012D8
_080187F4: .4byte gUnknown_03001308
_080187F8: .4byte gUnknown_030012BC
