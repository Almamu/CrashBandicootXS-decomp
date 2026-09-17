.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_802FBF0
sub_802FBF0: @ 0x0802FBF0
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	adds r7, r1, #0
	cmp r7, #0
	bge _0802FC2E
	ldr r1, [r6, #0x68]
	ldr r0, _0802FC1C @ =0x00000955
	cmp r1, r0
	bgt _0802FC20
	movs r0, #1
	movs r1, #3
	str r0, [r6, #0x28]
	movs r2, #0
	str r2, [r6, #0x44]
	str r1, [r6, #0xc]
	ldr r0, [r6]
	ldrh r0, [r0, #0x24]
	movs r1, #0
	strh r0, [r6, #0x10]
	strb r1, [r6, #0x12]
	str r2, [r6, #8]
	b _0802FCAE
	.align 2, 0
_0802FC1C: .4byte 0x00000955
_0802FC20:
	movs r0, #0
	str r0, [r6, #0x6c]
	str r0, [r6, #0x70]
	movs r0, #0x80
	lsls r0, r0, #0x17
	str r0, [r6, #0x74]
	b _0802FCAE
_0802FC2E:
	adds r0, r7, #0
	bl sub_802A570
	ldr r1, _0802FCE4 @ =gUnknown_0300089C
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	str r0, [r6, #0x68]
	adds r0, r7, #0
	bl sub_802A51C
	ldr r1, [r6, #0x24]
	subs r0, r0, r1
	lsls r0, r0, #8
	ldr r1, [r6, #0x68]
	bl sub_803ADB4
	asrs r0, r0, #4
	str r0, [r6, #0x74]
	cmp r0, #0
	bne _0802FC5C
	movs r0, #1
	str r0, [r6, #0x74]
_0802FC5C:
	ldr r1, [r6, #0x74]
	movs r0, #0x80
	lsls r0, r0, #8
	bl sub_803ADB4
	adds r4, r0, #0
	adds r0, r7, #0
	bl sub_802A558
	ldr r1, [r6, #0x1c]
	subs r0, r0, r1
	ldr r2, [r6, #0x60]
	ldr r1, [r6, #0x74]
	muls r1, r2, r1
	asrs r1, r1, #4
	subs r0, r0, r1
	muls r0, r4, r0
	asrs r0, r0, #0xd
	lsls r5, r4, #1
	muls r0, r5, r0
	asrs r0, r0, #0xd
	str r0, [r6, #0x6c]
	adds r0, r7, #0
	bl sub_802A540
	ldr r1, [r6, #0x20]
	subs r0, r0, r1
	ldr r2, [r6, #0x64]
	ldr r1, [r6, #0x74]
	muls r1, r2, r1
	asrs r1, r1, #4
	subs r0, r0, r1
	muls r0, r4, r0
	asrs r0, r0, #0xd
	muls r0, r5, r0
	asrs r0, r0, #0xd
	str r0, [r6, #0x70]
	adds r0, r7, #0
	bl sub_802A504
	str r0, [r6, #0x78]
_0802FCAE:
	ldr r1, [r6, #0x68]
	ldr r0, _0802FCE8 @ =0x00000955
	cmp r1, r0
	bgt _0802FCEC
	movs r0, #3
	str r0, [r6, #0xc]
	ldr r0, [r6]
	ldrh r0, [r0, #0x24]
	movs r1, #0
	strh r0, [r6, #0x10]
	strb r1, [r6, #0x12]
	adds r0, r6, #0
	bl GetAnimFrameBaseOffset
	ldr r2, [r6, #0xc]
	ldr r3, [r6]
	lsls r1, r2, #1
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r1, r1, r3
	movs r2, #4
	ldrsh r1, [r1, r2]
	cmp r0, r1
	blt _0802FD16
	movs r0, #0
	str r0, [r6, #8]
	b _0802FD16
	.align 2, 0
_0802FCE4: .4byte gUnknown_0300089C
_0802FCE8: .4byte 0x00000955
_0802FCEC:
	movs r4, #0
	str r4, [r6, #0xc]
	ldr r0, [r6]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r6, #0x10]
	strb r1, [r6, #0x12]
	adds r0, r6, #0
	bl GetAnimFrameBaseOffset
	ldr r2, [r6, #0xc]
	ldr r3, [r6]
	lsls r1, r2, #1
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r1, r1, r3
	movs r2, #4
	ldrsh r1, [r1, r2]
	cmp r0, r1
	blt _0802FD16
	str r4, [r6, #8]
_0802FD16:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_802FD1C
sub_802FD1C: @ 0x0802FD1C
	push {r4, lr}
	adds r2, r0, #0
	ldr r0, [r2, #0x54]
	subs r0, r0, r1
	str r0, [r2, #0x54]
	cmp r0, #0
	bgt _0802FD80
	adds r1, r2, #0
	adds r1, #0x7c
	movs r4, #0
	movs r0, #1
	strb r0, [r1]
	ldr r0, [r2, #0x60]
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	str r0, [r2, #0x60]
	ldr r1, [r2, #0x64]
	cmp r1, #0
	bge _0802FD4C
	lsrs r0, r1, #0x1f
	adds r0, r1, r0
	asrs r0, r0, #1
	str r0, [r2, #0x64]
_0802FD4C:
	ldr r0, [r2, #0xc]
	movs r3, #4
	cmp r0, #0
	bne _0802FD56
	movs r3, #1
_0802FD56:
	movs r0, #2
	str r0, [r2, #0x28]
	str r4, [r2, #0x44]
	str r3, [r2, #0xc]
	ldr r1, [r2]
	lsls r0, r3, #1
	adds r0, r0, r3
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r2, #0x10]
	strb r1, [r2, #0x12]
	str r4, [r2, #8]
	ldr r0, _0802FD88 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x25
	bl PlaySfx
_0802FD80:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0802FD88: .4byte gUnknown_030012BC

	thumb_func_start sub_802FD8C
sub_802FD8C: @ 0x0802FD8C
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r5, r0, #0
	ldr r0, [sp, #0x18]
	ldr r7, [sp, #0x1c]
	movs r4, #4
	str r0, [sp]
	adds r0, r5, #0
	bl InitActorPart
	str r4, [r5, #0x54]
	ldr r0, _0802FDF0 @ =gStaticData_087E51B4
	str r0, [r5, #0x50]
	movs r0, #0x3c
	str r0, [r5, #0x58]
	movs r0, #0
	str r0, [r5, #0x5c]
	adds r1, r5, #0
	adds r1, #0x7c
	strb r0, [r1]
	str r0, [r5, #0x64]
	str r0, [r5, #0x60]
	ldr r6, _0802FDF4 @ =0x00000955
	str r6, [r5, #0x68]
	ldr r0, [r7, #0x10]
	cmp r0, #0
	blt _0802FDDE
	ldr r4, _0802FDF8 @ =gUnknown_0300089C
	bl sub_802A570
	lsls r0, r0, #2
	adds r0, r0, r4
	ldr r0, [r0]
	cmp r0, r6
	ble _0802FDDE
	ldr r0, [r5, #0x24]
	ldr r1, _0802FDFC @ =0xFFFF7200
	adds r0, r0, r1
	str r0, [r5, #0x24]
	ldr r0, _0802FE00 @ =0x00000D55
	str r0, [r5, #0x68]
_0802FDDE:
	ldr r1, [r7, #0x10]
	adds r0, r5, #0
	bl sub_802FBF0
	adds r0, r5, #0
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0802FDF0: .4byte gStaticData_087E51B4
_0802FDF4: .4byte 0x00000955
_0802FDF8: .4byte gUnknown_0300089C
_0802FDFC: .4byte 0xFFFF7200
_0802FE00: .4byte 0x00000D55

	thumb_func_start sub_802FE04
sub_802FE04: @ 0x0802FE04
	adds r2, r0, #0
	ldr r0, [r2, #0x64]
	ldr r1, [r2, #0x70]
	adds r0, r0, r1
	str r0, [r2, #0x64]
	movs r1, #0xa0
	lsls r1, r1, #5
	cmp r0, r1
	ble _0802FE18
	str r1, [r2, #0x64]
_0802FE18:
	bx lr
	.align 2, 0

	thumb_func_start sub_802FE1C
sub_802FE1C: @ 0x0802FE1C
	push {r4, lr}
	adds r3, r0, #0
	ldrb r0, [r3, #0x12]
	cmp r0, #0
	beq _0802FE52
	movs r0, #0xa0
	str r0, [r3, #0x70]
	ldr r0, [r3, #0xc]
	movs r4, #5
	cmp r0, #1
	bne _0802FE34
	movs r4, #2
_0802FE34:
	movs r0, #3
	str r0, [r3, #0x28]
	movs r2, #0
	str r2, [r3, #0x44]
	str r4, [r3, #0xc]
	ldr r1, [r3]
	lsls r0, r4, #1
	adds r0, r0, r4
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r3, #0x10]
	strb r1, [r3, #0x12]
	str r2, [r3, #8]
_0802FE52:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_802FE58
sub_802FE58: @ 0x0802FE58
	ldr r1, _0802FE74 @ =gUnknown_03000884
	ldr r3, [r1]
	ldr r1, [r3, #0x1c]
	ldr r2, [r0, #0x1c]
	subs r1, r1, r2
	asrs r1, r1, #4
	str r1, [r0, #0x60]
	ldr r1, [r3, #0x20]
	ldr r2, [r0, #0x20]
	subs r1, r1, r2
	asrs r1, r1, #4
	str r1, [r0, #0x64]
	bx lr
	.align 2, 0
_0802FE74: .4byte gUnknown_03000884

	thumb_func_start sub_802FE78
sub_802FE78: @ 0x0802FE78
	push {lr}
	adds r2, r0, #0
	ldr r0, [r2, #0x60]
	ldr r1, [r2, #0x6c]
	adds r0, r0, r1
	str r0, [r2, #0x60]
	ldr r0, [r2, #0x64]
	ldr r1, [r2, #0x70]
	adds r0, r0, r1
	str r0, [r2, #0x64]
	ldr r0, [r2, #0x74]
	subs r0, #1
	str r0, [r2, #0x74]
	cmp r0, #0
	bgt _0802FE9E
	ldr r1, [r2, #0x78]
	adds r0, r2, #0
	bl sub_802FBF0
_0802FE9E:
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_802FEA4
sub_802FEA4: @ 0x0802FEA4
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r1, _0802FED0 @ =gStaticData_0817C260
	ldr r0, [r4, #0x28]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r7, #2
	ldrsh r2, [r0, r7]
	adds r7, r1, #0
	cmp r2, #0
	ble _0802FED4
	movs r1, #4
	ldrsh r0, [r0, r1]
	adds r0, r4, r0
	ldr r1, [r0]
	lsls r0, r2, #3
	adds r0, r0, r1
	subs r0, #8
	ldr r5, [r0]
	ldr r6, [r0, #4]
	adds r3, r6, #0
	b _0802FEDA
	.align 2, 0
_0802FED0: .4byte gStaticData_0817C260
_0802FED4:
	adds r0, r7, #4
	adds r0, r3, r0
	ldr r3, [r0]
_0802FEDA:
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _0802FEF0
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _0802FEF2
_0802FEF0:
	adds r0, r1, #0
_0802FEF2:
	adds r0, r4, r0
	bl sub_803AD84
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_802FF00
sub_802FF00: @ 0x0802FF00
	adds r0, #0x7c
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_802FF08
sub_802FF08: @ 0x0802FF08
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r7, r0, #0
	mov r8, r1
	adds r5, r2, #0
	adds r6, r3, #0
	ldr r0, [sp, #0x1c]
	movs r4, #2
	str r0, [sp]
	adds r0, r7, #0
	bl InitActorPart
	str r4, [r7, #0x54]
	ldr r0, _0802FF4C @ =gStaticData_087E51EC
	str r0, [r7, #0x50]
	str r5, [r7, #0x58]
	str r6, [r7, #0x5c]
	adds r1, r7, #0
	adds r1, #0x60
	movs r0, #0
	strb r0, [r1]
	mov r1, r8
	ldrb r0, [r1]
	subs r0, #4
	cmp r0, #5
	bhi _0802FFA8
	lsls r0, r0, #2
	ldr r1, _0802FF50 @ =_0802FF54
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0802FF4C: .4byte gStaticData_087E51EC
_0802FF50: .4byte _0802FF54
_0802FF54: @ jump table
	.4byte _0802FF6C @ case 0
	.4byte _0802FF82 @ case 1
	.4byte _0802FF86 @ case 2
	.4byte _0802FF8A @ case 3
	.4byte _0802FF8E @ case 4
	.4byte _0802FF92 @ case 5
_0802FF6C:
	movs r1, #0
	str r1, [r7, #0x28]
	str r1, [r7, #0x44]
	str r1, [r7, #0xc]
	ldr r0, [r7]
	ldrh r0, [r0]
	movs r2, #0
	strh r0, [r7, #0x10]
	strb r2, [r7, #0x12]
	str r1, [r7, #8]
	b _0802FFA8
_0802FF82:
	movs r0, #1
	b _0802FF94
_0802FF86:
	movs r0, #2
	b _0802FF94
_0802FF8A:
	movs r0, #3
	b _0802FF94
_0802FF8E:
	movs r0, #4
	b _0802FF94
_0802FF92:
	movs r0, #5
_0802FF94:
	movs r2, #0
	str r0, [r7, #0x28]
	str r2, [r7, #0x44]
	str r2, [r7, #0xc]
	ldr r0, [r7]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r7, #0x10]
	strb r1, [r7, #0x12]
	str r2, [r7, #8]
_0802FFA8:
	adds r0, r7, #0
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_802FFB8
sub_802FFB8: @ 0x0802FFB8
	push {r4, r5, r6, r7, lr}
	sub sp, #8
	adds r4, r0, #0
	ldr r0, [r4, #0x28]
	cmp r0, #6
	beq _0803001A
	ldr r5, _08030048 @ =gUnknown_03000884
	ldr r0, [r5]
	bl sub_802F46C
	ldr r0, [r4, #0x28]
	cmp r0, #6
	beq _0803001A
	adds r0, r4, #0
	bl sub_802A6EC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0803001A
	ldr r0, [r5]
	ldr r2, [r0, #0x50]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #0xa
	bl sub_803AD80
	movs r0, #4
	str r0, [r4, #0x18]
	ldr r0, _0803004C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	movs r0, #6
	movs r1, #1
	str r0, [r4, #0x28]
	movs r2, #0
	str r2, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
_0803001A:
	ldr r1, _08030050 @ =gStaticData_0817C280
	ldr r0, [r4, #0x28]
	lsls r5, r0, #3
	adds r2, r5, r1
	movs r6, #2
	ldrsh r3, [r2, r6]
	adds r6, r0, #0
	adds r7, r1, #0
	cmp r3, #0
	ble _08030054
	movs r1, #4
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r0]
	lsls r0, r3, #3
	adds r0, r0, r1
	subs r0, #8
	ldr r1, [r0]
	ldr r2, [r0, #4]
	str r1, [sp]
	str r2, [sp, #4]
	ldr r2, [sp, #4]
	b _0803005A
	.align 2, 0
_08030048: .4byte gUnknown_03000884
_0803004C: .4byte gUnknown_030012BC
_08030050: .4byte gStaticData_0817C280
_08030054:
	adds r0, r7, #4
	adds r0, r5, r0
	ldr r2, [r0]
_0803005A:
	lsls r0, r6, #3
	adds r0, r0, r7
	movs r5, #0
	ldrsh r1, [r0, r5]
	cmp r3, #0
	ble _08030070
	ldr r6, [sp]
	lsls r0, r6, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _08030072
_08030070:
	adds r0, r1, #0
_08030072:
	adds r0, r4, r0
	bl sub_803AD80
	ldr r0, [r4, #0x28]
	cmp r0, #6
	bne _0803009A
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _0803009A
	cmp r4, #0
	beq _080300A6
	ldr r1, [r4, #0x50]
	movs r2, #8
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
	b _080300A6
_0803009A:
	ldr r0, [r4, #0x24]
	adds r0, #0x60
	str r0, [r4, #0x24]
	adds r0, r4, #0
	bl sub_802A7B8
_080300A6:
	add sp, #8
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80300B0
sub_80300B0: @ 0x080300B0
	ldr r1, _080300D0 @ =gUnknown_03000884
	ldr r3, [r1]
	ldr r1, [r3, #0x1c]
	ldr r2, [r0, #0x1c]
	subs r1, r1, r2
	asrs r1, r1, #5
	adds r2, r2, r1
	str r2, [r0, #0x1c]
	ldr r1, [r3, #0x20]
	ldr r2, [r0, #0x20]
	subs r1, r1, r2
	asrs r1, r1, #5
	adds r2, r2, r1
	str r2, [r0, #0x20]
	bx lr
	.align 2, 0
_080300D0: .4byte gUnknown_03000884

	thumb_func_start nullsub_28
nullsub_28: @ 0x080300D4
	bx lr
	.align 2, 0

	thumb_func_start sub_80300D8
sub_80300D8: @ 0x080300D8
	ldr r1, [r0, #0x24]
	subs r1, #0x88
	str r1, [r0, #0x24]
	bx lr

	thumb_func_start sub_80300E0
sub_80300E0: @ 0x080300E0
	push {r4, lr}
	adds r4, r0, #0
	ldr r1, [r4, #0x34]
	ldr r0, _08030128 @ =0x000035FF
	cmp r1, r0
	ble _08030130
	ldr r3, _0803012C @ =gStaticData_0816A820
	ldr r2, [r4, #0x44]
	lsls r2, r2, #4
	asrs r2, r2, #4
	movs r1, #0xff
	ands r2, r1
	adds r0, r2, #0
	adds r0, #0x40
	ands r0, r1
	lsls r0, r0, #1
	adds r0, r0, r3
	movs r1, #0
	ldrsh r0, [r0, r1]
	lsls r1, r0, #4
	subs r1, r1, r0
	lsls r1, r1, #2
	ldr r0, [r4, #0x58]
	adds r0, r0, r1
	str r0, [r4, #0x1c]
	lsls r2, r2, #1
	adds r2, r2, r3
	movs r1, #0
	ldrsh r0, [r2, r1]
	lsls r1, r0, #4
	subs r1, r1, r0
	lsls r1, r1, #2
	ldr r0, [r4, #0x5c]
	adds r0, r0, r1
	str r0, [r4, #0x20]
	b _08030136
	.align 2, 0
_08030128: .4byte 0x000035FF
_0803012C: .4byte gStaticData_0816A820
_08030130:
	adds r0, r4, #0
	bl sub_80300B0
_08030136:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_803013C
sub_803013C: @ 0x0803013C
	push {lr}
	adds r3, r0, #0
	ldr r1, [r3, #0x34]
	ldr r0, _08030174 @ =0x000035FF
	cmp r1, r0
	ble _0803017C
	ldr r2, _08030178 @ =gStaticData_0816A820
	ldr r1, [r3, #0x44]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #1
	asrs r0, r0, #4
	movs r1, #0xff
	ands r0, r1
	adds r0, #0x40
	ands r0, r1
	lsls r0, r0, #1
	adds r0, r0, r2
	movs r1, #0
	ldrsh r0, [r0, r1]
	lsls r1, r0, #2
	adds r1, r1, r0
	lsls r1, r1, #4
	ldr r0, [r3, #0x58]
	adds r0, r0, r1
	str r0, [r3, #0x1c]
	b _08030182
	.align 2, 0
_08030174: .4byte 0x000035FF
_08030178: .4byte gStaticData_0816A820
_0803017C:
	adds r0, r3, #0
	bl sub_80300B0
_08030182:
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8030188
sub_8030188: @ 0x08030188
	push {lr}
	adds r3, r0, #0
	ldr r1, [r3, #0x34]
	ldr r0, _080301B8 @ =0x000035FF
	cmp r1, r0
	ble _080301C0
	ldr r2, _080301BC @ =gStaticData_0816A820
	ldr r0, [r3, #0x44]
	lsls r0, r0, #4
	asrs r0, r0, #4
	movs r1, #0xff
	ands r0, r1
	lsls r0, r0, #1
	adds r0, r0, r2
	movs r1, #0
	ldrsh r0, [r0, r1]
	lsls r1, r0, #4
	subs r1, r1, r0
	lsls r1, r1, #2
	ldr r0, [r3, #0x5c]
	adds r0, r0, r1
	str r0, [r3, #0x20]
	b _080301C6
	.align 2, 0
_080301B8: .4byte 0x000035FF
_080301BC: .4byte gStaticData_0816A820
_080301C0:
	adds r0, r3, #0
	bl sub_80300B0
_080301C6:
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80301CC
sub_80301CC: @ 0x080301CC
	push {lr}
	adds r2, r0, #0
	ldr r1, [r2, #0x34]
	ldr r0, _080301E4 @ =0x000035FF
	cmp r1, r0
	bgt _080301DE
	adds r0, r2, #0
	bl sub_80300B0
_080301DE:
	pop {r0}
	bx r0
	.align 2, 0
_080301E4: .4byte 0x000035FF

	thumb_func_start nullsub_29
nullsub_29: @ 0x080301E8
	bx lr
	.align 2, 0

	thumb_func_start sub_80301EC
sub_80301EC: @ 0x080301EC
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x28]
	cmp r0, #6
	beq _0803022A
	ldr r0, [r4, #0x54]
	subs r0, r0, r1
	str r0, [r4, #0x54]
	cmp r0, #0
	bgt _0803022A
	movs r0, #4
	str r0, [r4, #0x18]
	ldr r0, _08030230 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	movs r0, #6
	movs r1, #1
	str r0, [r4, #0x28]
	movs r2, #0
	str r2, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
_0803022A:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08030230: .4byte gUnknown_030012BC

	thumb_func_start sub_8030234
sub_8030234: @ 0x08030234
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r1, _08030260 @ =gStaticData_0817C280
	ldr r0, [r4, #0x28]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r7, #2
	ldrsh r2, [r0, r7]
	adds r7, r1, #0
	cmp r2, #0
	ble _08030264
	movs r1, #4
	ldrsh r0, [r0, r1]
	adds r0, r4, r0
	ldr r1, [r0]
	lsls r0, r2, #3
	adds r0, r0, r1
	subs r0, #8
	ldr r5, [r0]
	ldr r6, [r0, #4]
	adds r3, r6, #0
	b _0803026A
	.align 2, 0
_08030260: .4byte gStaticData_0817C280
_08030264:
	adds r0, r7, #4
	adds r0, r3, r0
	ldr r3, [r0]
_0803026A:
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _08030280
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _08030282
_08030280:
	adds r0, r1, #0
_08030282:
	adds r0, r4, r0
	bl sub_803AD84
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8030290
sub_8030290: @ 0x08030290
	adds r0, #0x60
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8030298
sub_8030298: @ 0x08030298
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x1c]
	ldr r1, [r4, #0x58]
	adds r0, r0, r1
	str r0, [r4, #0x1c]
	ldr r0, [r4, #0x20]
	ldr r1, [r4, #0x5c]
	adds r0, r0, r1
	str r0, [r4, #0x20]
	ldr r0, [r4, #0x24]
	ldr r1, _080302EC @ =0xFFFFFF00
	adds r0, r0, r1
	str r0, [r4, #0x24]
	adds r0, r4, #0
	bl sub_802A6EC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _080302F4
	ldr r0, _080302F0 @ =gUnknown_03000884
	ldr r0, [r0]
	ldr r2, [r0, #0x50]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #2
	bl sub_803AD80
	cmp r4, #0
	beq _080302FA
	ldr r1, [r4, #0x50]
	movs r2, #8
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
	b _080302FA
	.align 2, 0
_080302EC: .4byte 0xFFFFFF00
_080302F0: .4byte gUnknown_03000884
_080302F4:
	adds r0, r4, #0
	bl sub_802A7B8
_080302FA:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8030300
sub_8030300: @ 0x08030300
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, [sp, #0x18]
	ldr r6, [sp, #0x1c]
	ldr r7, [sp, #0x20]
	movs r5, #1
	str r0, [sp]
	adds r0, r4, #0
	bl InitActorPart
	str r5, [r4, #0x54]
	ldr r0, _0803032C @ =gStaticData_087E5224
	str r0, [r4, #0x50]
	str r6, [r4, #0x58]
	str r7, [r4, #0x5c]
	adds r0, r4, #0
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0803032C: .4byte gStaticData_087E5224

	thumb_func_start sub_8030330
sub_8030330: @ 0x08030330
	movs r0, #1
	bx lr

	thumb_func_start sub_8030334
sub_8030334: @ 0x08030334
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r6, r0, #0
	ldr r0, [r6, #0x24]
	ldr r1, [r6, #0x60]
	adds r0, r0, r1
	str r0, [r6, #0x24]
	subs r1, #5
	str r1, [r6, #0x60]
	cmp r1, #0x13
	bgt _08030352
	movs r0, #0x14
	str r0, [r6, #0x60]
_08030352:
	ldr r0, [r6, #0x64]
	movs r1, #0x80
	lsls r1, r1, #1
	mov sb, r1
	add r0, sb
	str r0, [r6, #0x64]
	movs r1, #0xa8
	lsls r1, r1, #6
	cmp r0, r1
	ble _08030368
	str r1, [r6, #0x64]
_08030368:
	ldr r2, _08030438 @ =gUnknown_03000884
	mov r8, r2
	ldr r2, [r2]
	ldr r0, [r2, #0x1c]
	ldr r3, [r6, #0x58]
	ldr r4, _0803043C @ =0xFFFFFA00
	adds r1, r3, r4
	subs r0, r0, r1
	asrs r0, r0, #5
	adds r3, r3, r0
	str r3, [r6, #0x58]
	ldr r0, [r2, #0x20]
	ldr r4, [r6, #0x5c]
	movs r2, #0x80
	lsls r2, r2, #4
	adds r1, r4, r2
	subs r0, r0, r1
	asrs r0, r0, #5
	adds r4, r4, r0
	str r4, [r6, #0x5c]
	ldr r5, _08030440 @ =gStaticData_0816A820
	ldr r7, [r6, #0x44]
	lsls r1, r7, #5
	asrs r1, r1, #4
	movs r2, #0xff
	ands r1, r2
	adds r0, r1, #0
	adds r0, #0x40
	ands r0, r2
	lsls r0, r0, #1
	adds r0, r0, r5
	movs r2, #0
	ldrsh r0, [r0, r2]
	ldr r2, [r6, #0x64]
	muls r0, r2, r0
	asrs r0, r0, #8
	adds r3, r3, r0
	str r3, [r6, #0x1c]
	lsls r1, r1, #1
	adds r1, r1, r5
	movs r3, #0
	ldrsh r0, [r1, r3]
	muls r0, r2, r0
	asrs r0, r0, #8
	adds r4, r4, r0
	str r4, [r6, #0x20]
	ldr r1, [r6, #0x34]
	ldr r0, _08030444 @ =0x00002BFF
	cmp r1, r0
	bgt _080303E4
	movs r0, #1
	movs r2, #0
	str r0, [r6, #0x28]
	str r2, [r6, #0x44]
	str r2, [r6, #0xc]
	ldr r0, [r6]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r6, #0x10]
	strb r1, [r6, #0x12]
	str r2, [r6, #8]
	str r7, [r6, #0x44]
_080303E4:
	adds r0, r6, #0
	bl sub_802A6EC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0803042C
	mov r4, r8
	ldr r0, [r4]
	ldr r2, [r0, #0x50]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #6
	bl sub_803AD80
	movs r0, #4
	str r0, [r6, #0x18]
	ldr r0, _08030448 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #4
	mov r2, sb
	bl PlaySfx
	movs r0, #2
	movs r1, #1
	str r0, [r6, #0x28]
	movs r2, #0
	str r2, [r6, #0x44]
	str r1, [r6, #0xc]
	ldr r0, [r6]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r6, #0x10]
	strb r1, [r6, #0x12]
	str r2, [r6, #8]
_0803042C:
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08030438: .4byte gUnknown_03000884
_0803043C: .4byte 0xFFFFFA00
_08030440: .4byte gStaticData_0816A820
_08030444: .4byte 0x00002BFF
_08030448: .4byte gUnknown_030012BC

	thumb_func_start sub_803044C
sub_803044C: @ 0x0803044C
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r0, [r6, #0x24]
	ldr r1, [r6, #0x60]
	adds r0, r0, r1
	str r0, [r6, #0x24]
	subs r1, #5
	str r1, [r6, #0x60]
	cmp r1, #0x13
	bgt _08030464
	movs r0, #0x14
	str r0, [r6, #0x60]
_08030464:
	ldr r0, [r6, #0x64]
	ldr r1, _0803051C @ =0xFFFFFF00
	adds r0, r0, r1
	str r0, [r6, #0x64]
	cmp r0, #0
	bge _08030474
	movs r0, #0
	str r0, [r6, #0x64]
_08030474:
	ldr r7, _08030520 @ =gUnknown_03000884
	ldr r2, [r7]
	ldr r0, [r2, #0x1c]
	ldr r3, [r6, #0x58]
	ldr r4, _08030524 @ =0xFFFFFA00
	adds r1, r3, r4
	subs r0, r0, r1
	asrs r0, r0, #4
	adds r3, r3, r0
	str r3, [r6, #0x58]
	ldr r0, [r2, #0x20]
	ldr r4, [r6, #0x5c]
	movs r2, #0x80
	lsls r2, r2, #4
	adds r1, r4, r2
	subs r0, r0, r1
	asrs r0, r0, #4
	adds r4, r4, r0
	str r4, [r6, #0x5c]
	ldr r5, _08030528 @ =gStaticData_0816A820
	ldr r1, [r6, #0x44]
	lsls r1, r1, #5
	asrs r1, r1, #4
	movs r2, #0xff
	ands r1, r2
	adds r0, r1, #0
	adds r0, #0x40
	ands r0, r2
	lsls r0, r0, #1
	adds r0, r0, r5
	movs r2, #0
	ldrsh r0, [r0, r2]
	ldr r2, [r6, #0x64]
	muls r0, r2, r0
	asrs r0, r0, #8
	adds r3, r3, r0
	str r3, [r6, #0x1c]
	lsls r1, r1, #1
	adds r1, r1, r5
	movs r3, #0
	ldrsh r0, [r1, r3]
	muls r0, r2, r0
	asrs r0, r0, #8
	adds r4, r4, r0
	str r4, [r6, #0x20]
	adds r0, r6, #0
	bl sub_802A6EC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08030516
	ldr r0, [r7]
	ldr r2, [r0, #0x50]
	movs r4, #0x20
	ldrsh r1, [r2, r4]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #6
	bl sub_803AD80
	movs r0, #4
	str r0, [r6, #0x18]
	ldr r0, _0803052C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	movs r0, #2
	movs r1, #1
	str r0, [r6, #0x28]
	movs r2, #0
	str r2, [r6, #0x44]
	str r1, [r6, #0xc]
	ldr r0, [r6]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r6, #0x10]
	strb r1, [r6, #0x12]
	str r2, [r6, #8]
_08030516:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0803051C: .4byte 0xFFFFFF00
_08030520: .4byte gUnknown_03000884
_08030524: .4byte 0xFFFFFA00
_08030528: .4byte gStaticData_0816A820
_0803052C: .4byte gUnknown_030012BC

