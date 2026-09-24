.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_80188D0
sub_80188D0: @ 0x080188D0
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800B8C8
	ldr r0, _080188E4 @ =gStaticData_087E4494
	str r0, [r4, #0xc]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_080188E4: .4byte gStaticData_087E4494

	thumb_func_start sub_80188E8
sub_80188E8: @ 0x080188E8
	push {lr}
	ldr r2, _080188F8 @ =gStaticData_087E4494
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_080188F8: .4byte gStaticData_087E4494

	thumb_func_start sub_80188FC
sub_80188FC: @ 0x080188FC
	push {r4, lr}
	adds r0, r1, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801893A
	movs r0, #1
	ldrb r2, [r1, #0xc]
	orrs r0, r2
	strb r0, [r1, #0xc]
	ldr r0, _08018940 @ =0x0000FFFF
	ldrh r4, [r1, #8]
	cmp r4, r0
	beq _0801893A
	ldrh r3, [r1, #8]
	ldr r0, _08018944 @ =gUnknown_030012B4
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
_0801893A:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08018940: .4byte 0x0000FFFF
_08018944: .4byte gUnknown_030012B4

	thumb_func_start sub_8018948
sub_8018948: @ 0x08018948
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800B8C8
	ldr r0, _0801895C @ =gStaticData_087E44FC
	str r0, [r4, #0xc]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801895C: .4byte gStaticData_087E44FC

	thumb_func_start sub_8018960
sub_8018960: @ 0x08018960
	push {lr}
	ldr r2, _08018970 @ =gStaticData_087E44FC
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_08018970: .4byte gStaticData_087E44FC

	thumb_func_start nullsub_19
nullsub_19: @ 0x08018974
	bx lr
	.align 2, 0

	thumb_func_start sub_8018978
sub_8018978: @ 0x08018978
	adds r3, r0, #0
	mov ip, r1
	ldr r1, [r1]
	ldr r0, [r3, #0x30]
	cmp r0, r1
	bgt _08018998
	mov r0, ip
	adds r0, #0x28
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r2, [r0]
	ands r1, r2
	movs r2, #0x10
	orrs r1, r2
	strb r1, [r0]
	b _080189A6
_08018998:
	mov r1, ip
	adds r1, #0x28
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r2, [r1]
	ands r0, r2
	strb r0, [r1]
_080189A6:
	movs r0, #0x1a
	str r0, [r3, #0x38]
	str r0, [r3, #0x3c]
	mov r1, ip
	ldr r0, [r1, #4]
	ldr r1, [r3, #0x34]
	subs r0, r0, r1
	str r0, [r3, #0x40]
	mov r2, ip
	ldr r0, [r2]
	ldr r1, [r3, #0x30]
	subs r0, r0, r1
	str r0, [r3, #0x44]
	bx lr
	.align 2, 0

	thumb_func_start sub_80189C4
sub_80189C4: @ 0x080189C4
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, _080189E8 @ =gStaticData_087E4564
	str r0, [r4, #0xc]
	ldr r0, [r4, #0x48]
	cmp r0, #0
	beq _080189D8
	bl sub_8026EB4
_080189D8:
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_8017A78
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080189E8: .4byte gStaticData_087E4564

	thumb_func_start sub_80189EC
sub_80189EC: @ 0x080189EC
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8017A8C
	ldr r0, _08018A28 @ =gStaticData_087E4564
	str r0, [r4, #0xc]
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r4, #0x24]
	ldr r0, _08018A2C @ =0x00000202
	bl sub_8026EC0
	str r0, [r4, #0x48]
	movs r2, #0
	movs r3, #0x80
	lsls r3, r3, #1
	adds r1, r0, #0
_08018A0E:
	adds r0, r2, #0
	muls r0, r2, r0
	asrs r0, r0, #8
	strh r0, [r1]
	adds r1, #2
	adds r2, #1
	cmp r2, r3
	ble _08018A0E
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08018A28: .4byte gStaticData_087E4564
_08018A2C: .4byte 0x00000202

	thumb_func_start sub_8018A30
sub_8018A30: @ 0x08018A30
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	ldr r0, [r5, #8]
	cmp r0, #4
	bls _08018A3E
	b _08018BCC
_08018A3E:
	lsls r0, r0, #2
	ldr r1, _08018A48 @ =_08018A4C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08018A48: .4byte _08018A4C
_08018A4C: @ jump table
	.4byte _08018A60 @ case 0
	.4byte _08018A7C @ case 1
	.4byte _08018B6C @ case 2
	.4byte _08018B8E @ case 3
	.4byte _08018BCC @ case 4
_08018A60:
	adds r0, r5, #0
	adds r1, r6, #0
	bl sub_8018BDC
	adds r0, r5, #0
	adds r1, r6, #0
	bl sub_8018CB0
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r1, [r6, #0xc]
	ands r0, r1
	strb r0, [r6, #0xc]
	b _08018B82
_08018A7C:
	ldr r0, [r5, #0x20]
	ldr r1, [r0, #4]
	movs r0, #0xa0
	lsls r0, r0, #7
	cmp r1, r0
	bgt _08018A8E
	ldr r4, [r5, #0x1c]
	movs r0, #5
	b _08018A9A
_08018A8E:
	movs r0, #0xf0
	lsls r0, r0, #7
	cmp r1, r0
	bgt _08018AB6
	ldr r4, [r5, #0x1c]
	movs r0, #4
_08018A9A:
	adds r1, r4, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	b _08018AD4
_08018AB6:
	ldr r4, [r5, #0x1c]
	movs r0, #3
	adds r1, r4, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
_08018AD4:
	ldr r0, [r5, #0x20]
	ldr r1, [r0]
	ldr r0, [r6]
	subs r4, r1, r0
	mvns r2, r4
	adds r3, r6, #0
	adds r3, #0x28
	lsrs r2, r2, #0x1f
	lsls r2, r2, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	adds r0, r1, #0
	ldrb r7, [r3]
	ands r0, r7
	orrs r0, r2
	strb r0, [r3]
	ldr r0, [r5, #0x1c]
	adds r0, #0x28
	ldrb r3, [r0]
	ands r1, r3
	orrs r1, r2
	strb r1, [r0]
	ldr r0, _08018B68 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #8
	asrs r2, r4, #0x1f
	eors r4, r2
	subs r2, r4, r2
	lsls r0, r2, #1
	adds r0, r0, r2
	lsls r0, r0, #2
	bl sub_8037E54
	adds r4, r0, #0
	cmp r4, #5
	ble _08018B22
	movs r4, #5
_08018B22:
	movs r0, #5
	subs r4, r0, r4
	adds r3, r4, #0
	ldr r0, [r6, #0x20]
	adds r2, r6, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r7, [r2]
	lsls r0, r7, #3
	adds r2, r7, #0
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _08018B44
	subs r3, r0, #1
_08018B44:
	str r3, [r6, #0x30]
	ldr r5, [r5, #0x1c]
	adds r2, r4, #0
	ldr r0, [r5, #0x20]
	adds r3, r5, #0
	adds r3, #0x2d
	ldr r1, [r0]
	ldrb r4, [r3]
	lsls r0, r4, #3
	subs r0, r0, r4
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r2, r0
	blt _08018B64
	subs r2, r0, #1
_08018B64:
	str r2, [r5, #0x30]
	b _08018BCC
	.align 2, 0
_08018B68: .4byte gUnknown_03001308
_08018B6C:
	ldr r0, [r5, #0x10]
	adds r0, #1
	str r0, [r5, #0x10]
	cmp r0, #2
	ble _08018B82
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #3
	bl sub_8019770
	b _08018BCC
_08018B82:
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #1
	bl sub_8019770
	b _08018BCC
_08018B8E:
	ldr r1, [r5, #0x1c]
	ldr r0, [r1, #4]
	adds r0, #0x80
	str r0, [r1, #4]
	ldr r1, [r6, #4]
	adds r1, #0x80
	str r1, [r6, #4]
	ldr r0, _08018BD4 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x14]
	lsls r0, r0, #8
	movs r7, #0x80
	lsls r7, r7, #7
	adds r0, r0, r7
	cmp r1, r0
	blt _08018BCC
	ldr r0, _08018BD8 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231C4
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08018BC2
	bl sub_80241A4
_08018BC2:
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #4
	bl sub_8019770
_08018BCC:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08018BD4: .4byte gUnknown_03001308
_08018BD8: .4byte gUnknown_030012C0

	thumb_func_start sub_8018BDC
sub_8018BDC: @ 0x08018BDC
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	mov r8, r0
	adds r6, r1, #0
	ldr r0, _08018CA4 @ =0x0000FFFF
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _08018CA8 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x9f
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x20]
	movs r0, #3
	adds r1, r4, #0
	adds r1, #0x2d
	movs r5, #0
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	adds r0, #0x2c
	strb r5, [r0]
	movs r0, #0x10
	bl sub_8026EDC
	bl sub_8019758
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
	ldr r0, [r6]
	ldr r1, [r6, #4]
	str r0, [r4]
	str r1, [r4, #4]
	adds r6, #0x28
	adds r2, r4, #0
	adds r2, #0x28
	movs r1, #0x10
	ldrb r6, [r6]
	ands r1, r6
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r3, [r2]
	ands r0, r3
	orrs r0, r1
	strb r0, [r2]
	movs r0, #0x10
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _08018CAC @ =gUnknown_030012F4
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	mov r2, r8
	str r4, [r2, #0x1c]
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08018CA4: .4byte 0x0000FFFF
_08018CA8: .4byte gUnknown_030012D0
_08018CAC: .4byte gUnknown_030012F4

	thumb_func_start sub_8018CB0
sub_8018CB0: @ 0x08018CB0
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	mov r8, r0
	adds r6, r1, #0
	ldr r0, _08018D60 @ =0x0000FFFF
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _08018D64 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x9f
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x20]
	movs r0, #0xf
	adds r1, r4, #0
	adds r1, #0x2d
	movs r5, #0xf
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	ands r0, r5
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x40
	bl sub_8026EDC
	mov r1, r8
	bl sub_80196F8
	str r0, [r4, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	ldr r0, [r6]
	ldr r1, [r6, #4]
	movs r2, #0x80
	lsls r2, r2, #6
	adds r0, r0, r2
	ldr r3, _08018D68 @ =0xFFFFC000
	adds r1, r1, r3
	str r0, [r4]
	str r1, [r4, #4]
	movs r0, #0x10
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _08018D6C @ =gUnknown_030012F4
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	mov r2, r8
	str r4, [r2, #0x20]
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08018D60: .4byte 0x0000FFFF
_08018D64: .4byte gUnknown_030012D0
_08018D68: .4byte 0xFFFFC000
_08018D6C: .4byte gUnknown_030012F4

	thumb_func_start sub_8018D70
sub_8018D70: @ 0x08018D70
	push {r4, r5, lr}
	ldr r5, [sp, #0xc]
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _08018DA8 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	cmp r5, #1
	beq _08018DB6
	cmp r5, #1
	bgt _08018DAC
	cmp r5, #0
	beq _08018DB2
	b _08018DF0
	.align 2, 0
_08018DA8: .4byte gUnknown_030012D0
_08018DAC:
	cmp r5, #2
	beq _08018DD4
	b _08018DF0
_08018DB2:
	movs r0, #3
	b _08018DB8
_08018DB6:
	movs r0, #2
_08018DB8:
	adds r1, r4, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	b _08018DF0
_08018DD4:
	movs r0, #0
	adds r1, r4, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
_08018DF0:
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
	movs r0, #0x14
	bl sub_8026EDC
	adds r1, r5, #0
	bl sub_80195EC
	str r0, [r4, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #0
	strb r0, [r4, #0xa]
	subs r0, #5
	ldrb r1, [r4, #0xc]
	ands r0, r1
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _08018E48 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08018E48: .4byte gUnknown_030012F0

	thumb_func_start sub_8018E4C
sub_8018E4C: @ 0x08018E4C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	adds r6, r1, #0
	ldr r5, [r7, #0x24]
	cmp r5, #0
	beq _08018E82
	subs r5, #1
	str r5, [r7, #0x24]
	ldr r0, [r7, #0x1c]
	muls r0, r5, r0
	ldr r1, [r7, #0x28]
	mov r8, r1
	bl sub_803ADB4
	ldr r4, [r7, #0x14]
	subs r4, r4, r0
	ldr r0, [r7, #0x20]
	muls r0, r5, r0
	mov r1, r8
	bl sub_803ADB4
	ldr r1, [r7, #0x18]
	subs r1, r1, r0
	str r4, [r6]
	str r1, [r6, #4]
_08018E82:
	ldr r0, [r7, #8]
	cmp r0, #0xa
	bls _08018E8A
	b _0801908A
_08018E8A:
	lsls r0, r0, #2
	ldr r1, _08018E94 @ =_08018E98
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08018E94: .4byte _08018E98
_08018E98: @ jump table
	.4byte _08018EC4 @ case 0
	.4byte _08018EDA @ case 1
	.4byte _08018EDA @ case 2
	.4byte _08018EE4 @ case 3
	.4byte _08018F08 @ case 4
	.4byte _08018F4A @ case 5
	.4byte _08018EDA @ case 6
	.4byte _08018F32 @ case 7
	.4byte _08019070 @ case 8
	.4byte _08019080 @ case 9
	.4byte _0801908A @ case 10
_08018EC4:
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r2, [r6, #0xc]
	ands r0, r2
	strb r0, [r6, #0xc]
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #1
	bl sub_8019094
	b _0801908A
_08018EDA:
	ldr r0, [r7, #0x24]
	cmp r0, #0
	beq _08018EE2
	b _0801908A
_08018EE2:
	b _08018F3E
_08018EE4:
	movs r0, #4
	str r0, [r7, #0x2c]
	ldr r1, [r7, #0xc]
	adds r1, #0x50
	movs r4, #0
	ldrsh r0, [r1, r4]
	adds r0, r7, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #0x12
	bl sub_803AD84
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #7
	bl sub_8019094
	b _0801908A
_08018F08:
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #0
	bl sub_8019214
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #2
	bl sub_8019094
	ldr r1, [r7, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r7, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #0xf
	bl sub_803AD84
	b _0801908A
_08018F32:
	adds r0, r6, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	bne _08018F3E
	b _0801908A
_08018F3E:
	ldr r2, [r7, #0x2c]
	adds r0, r7, #0
	adds r1, r6, #0
	bl sub_8019094
	b _0801908A
_08018F4A:
	adds r0, r7, #0
	adds r0, #0x38
	ldrb r1, [r0]
	adds r5, r0, #0
	cmp r1, #0
	beq _08018F86
	ldr r0, [r7, #0x34]
	adds r0, #1
	str r0, [r7, #0x34]
	cmp r0, #9
	ble _08018F86
	movs r0, #0
	str r0, [r7, #0x34]
	ldr r3, [r6, #0x30]
	movs r0, #1
	eors r3, r0
	ldr r0, [r6, #0x20]
	adds r2, r6, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r4, [r2]
	lsls r0, r4, #3
	subs r0, r0, r4
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08018F84
	subs r3, r0, #1
_08018F84:
	str r3, [r6, #0x30]
_08018F86:
	ldr r4, [r7, #0x24]
	cmp r4, #0
	beq _08018F8E
	b _0801908A
_08018F8E:
	ldr r0, [r7, #0x30]
	subs r2, r0, #1
	str r2, [r7, #0x30]
	cmp r2, #0
	bne _08018FAE
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #1
	bl sub_8019094
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #1
	bl sub_8019214
	b _0801908A
_08018FAE:
	ldr r1, _08019058 @ =gStaticData_0816C35F
	ldr r0, [r7, #0x3c]
	ldr r0, [r0, #0x10]
	adds r0, r0, r1
	ldrb r0, [r0]
	cmp r2, r0
	bne _08018FEA
	ldr r0, _0801905C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x5c
	bl PlaySfx
	adds r0, r6, #0
	adds r0, #0x2c
	strb r4, [r0]
	ldr r1, [r7, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r7, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #0x10
	bl sub_803AD84
	movs r0, #1
	strb r0, [r5]
	str r4, [r7, #0x34]
_08018FEA:
	ldr r1, _08019060 @ =gStaticData_0816C362
	ldr r0, [r7, #0x3c]
	ldr r0, [r0, #0x10]
	adds r0, r0, r1
	ldr r1, [r7, #0x30]
	ldrb r0, [r0]
	cmp r1, r0
	bne _08019034
	adds r0, r6, #0
	adds r0, #0x2c
	strb r4, [r0]
	ldr r1, [r7, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r7, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #0x10
	bl sub_803AD84
	strb r4, [r5]
	movs r3, #1
	ldr r0, [r6, #0x20]
	adds r2, r6, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r4, [r2]
	lsls r0, r4, #3
	subs r0, r0, r4
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08019032
	subs r3, r0, #1
_08019032:
	str r3, [r6, #0x30]
_08019034:
	ldr r0, _08019064 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r2, [r0]
	ldr r3, [r0, #4]
	ldr r0, _08019068 @ =0xFFFFF600
	adds r3, r3, r0
	adds r0, r7, #0
	adds r1, r6, #0
	bl sub_80196B8
	ldr r0, [r7, #0x3c]
	ldr r0, [r0, #0x10]
	ldr r1, _0801906C @ =gStaticData_0816C35C
	adds r0, r0, r1
	ldrb r0, [r0]
	str r0, [r7, #0x28]
	str r0, [r7, #0x24]
	b _0801908A
	.align 2, 0
_08019058: .4byte gStaticData_0816C35F
_0801905C: .4byte gUnknown_030012BC
_08019060: .4byte gStaticData_0816C362
_08019064: .4byte gUnknown_030012D8
_08019068: .4byte 0xFFFFF600
_0801906C: .4byte gStaticData_0816C35C
_08019070:
	movs r0, #0xa
	str r0, [r7, #0x2c]
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #6
	bl sub_8019094
	b _0801908A
_08019080:
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #8
	bl sub_8019094
_0801908A:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8019094
sub_8019094: @ 0x08019094
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	adds r7, r1, #0
	adds r6, r2, #0
	cmp r6, #2
	beq _08019128
	cmp r6, #2
	bgt _080190AA
	cmp r6, #1
	beq _080190D4
	b _080191FE
_080190AA:
	cmp r6, #5
	bne _080190B0
	b _080191EA
_080190B0:
	cmp r6, #8
	beq _080190B6
	b _080191FE
_080190B6:
	ldr r0, _080190D0 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r2, [r0, #0x10]
	lsls r2, r2, #8
	lsrs r2, r2, #1
	ldr r3, [r0, #0x14]
	lsls r3, r3, #8
	movs r0, #0x80
	lsls r0, r0, #6
	adds r3, r3, r0
	b _080191E0
	.align 2, 0
_080190D0: .4byte gUnknown_03001308
_080190D4:
	adds r0, r5, #0
	movs r1, #0
	bl sub_801967C
	adds r0, r7, #0
	adds r0, #0x2c
	movs r4, #0
	strb r6, [r0]
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r7, #0
	movs r2, #0xf
	bl sub_803AD84
	strb r6, [r5, #0x10]
	strb r6, [r5, #0x11]
	strb r4, [r5, #0x12]
	ldr r0, _08019120 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r2, [r0, #0x10]
	lsls r2, r2, #8
	ldr r0, _08019124 @ =0xFFFFFC00
	adds r2, r2, r0
	movs r3, #0x98
	lsls r3, r3, #8
	adds r0, r5, #0
	adds r1, r7, #0
	bl sub_80196B8
	movs r0, #2
	str r0, [r5, #0x2c]
	b _080191FE
	.align 2, 0
_08019120: .4byte gUnknown_03001308
_08019124: .4byte 0xFFFFFC00
_08019128:
	movs r0, #3
	str r0, [r5, #0x2c]
	ldr r2, [r5, #0x14]
	ldrb r0, [r5, #0x10]
	cmp r0, #0
	beq _0801914C
	ldr r0, _08019148 @ =0xFFFFE800
	adds r1, r2, r0
	movs r0, #0x80
	lsls r0, r0, #3
	cmp r1, r0
	bgt _08019164
	movs r0, #0
	strb r0, [r5, #0x10]
	b _08019164
	.align 2, 0
_08019148: .4byte 0xFFFFE800
_0801914C:
	movs r0, #0xe0
	lsls r0, r0, #5
	adds r1, r2, r0
	ldr r0, _0801917C @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x10]
	lsls r0, r0, #8
	cmp r1, r0
	blt _08019164
	movs r0, #5
	str r0, [r5, #0x2c]
_08019164:
	ldr r0, [r5, #0x3c]
	ldr r0, [r0, #0x10]
	adds r1, r0, #0
	cmp r0, #1
	beq _08019190
	cmp r0, #1
	bgt _08019180
	cmp r0, #0
	beq _08019188
	ldrb r1, [r5, #0x10]
	b _080191B0
	.align 2, 0
_0801917C: .4byte gUnknown_03001308
_08019180:
	cmp r1, #2
	beq _0801919C
	ldrb r1, [r5, #0x10]
	b _080191B0
_08019188:
	ldrb r0, [r5, #0x10]
	strb r0, [r5, #0x11]
	adds r1, r0, #0
	b _080191B0
_08019190:
	movs r0, #1
	ldrb r1, [r5, #0x11]
	eors r0, r1
	strb r0, [r5, #0x11]
	ldrb r1, [r5, #0x10]
	b _080191B0
_0801919C:
	movs r3, #1
	ldrb r0, [r5, #0x11]
	eors r0, r3
	strb r0, [r5, #0x11]
	ldrb r1, [r5, #0x10]
	cmp r0, #0
	bne _080191B0
	ldrb r0, [r5, #0x12]
	eors r0, r3
	strb r0, [r5, #0x12]
_080191B0:
	lsls r0, r1, #0x18
	cmp r0, #0
	beq _080191C0
	ldr r0, _080191BC @ =0xFFFFE800
	adds r2, r2, r0
	b _080191C6
	.align 2, 0
_080191BC: .4byte 0xFFFFE800
_080191C0:
	movs r1, #0xc0
	lsls r1, r1, #5
	adds r2, r2, r1
_080191C6:
	ldrb r0, [r5, #0x11]
	cmp r0, #0
	beq _080191DC
	ldrb r0, [r5, #0x12]
	movs r3, #0x98
	lsls r3, r3, #8
	cmp r0, #0
	beq _080191E0
	movs r3, #0xf8
	lsls r3, r3, #6
	b _080191E0
_080191DC:
	movs r3, #0x82
	lsls r3, r3, #8
_080191E0:
	adds r0, r5, #0
	adds r1, r7, #0
	bl sub_80196B8
	b _080191FE
_080191EA:
	adds r0, r5, #0
	adds r0, #0x38
	movs r1, #0
	strb r1, [r0]
	adds r0, r5, #0
	movs r1, #1
	bl sub_801967C
	movs r0, #0x14
	str r0, [r5, #0x30]
_080191FE:
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	adds r1, r6, #0
	bl sub_803AD80
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8019214
sub_8019214: @ 0x08019214
	push {r4, r5, r6, r7, lr}
	adds r7, r0, #0
	adds r6, r1, #0
	adds r5, r2, #0
	ldr r0, _08019244 @ =0x0000FFFF
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _08019248 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x9f
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x20]
	cmp r5, #0
	beq _0801924C
	cmp r5, #1
	beq _0801926A
	b _08019286
	.align 2, 0
_08019244: .4byte 0x0000FFFF
_08019248: .4byte gUnknown_030012D0
_0801924C:
	movs r0, #0xe
	adds r1, r4, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	b _08019286
_0801926A:
	movs r0, #0x11
	adds r1, r4, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
_08019286:
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
	movs r0, #0x18
	bl sub_8026EDC
	ldr r1, [r7, #0x3c]
	bl sub_8019660
	adds r2, r0, #0
	movs r0, #0
	cmp r5, #1
	bne _080192B6
	movs r0, #1
_080192B6:
	strb r0, [r2, #0x10]
	str r2, [r4, #0x44]
	ldr r1, [r2, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	ldr r0, [r6]
	ldr r1, [r6, #4]
	str r0, [r4]
	str r1, [r4, #4]
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r1, [r4, #0xc]
	ands r0, r1
	movs r1, #1
	strb r1, [r4, #0xa]
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _08019304 @ =gUnknown_030012F4
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	cmp r5, #1
	bne _0801930C
	ldr r0, _08019308 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x31
	bl PlaySfx
	b _0801931A
	.align 2, 0
_08019304: .4byte gUnknown_030012F4
_08019308: .4byte gUnknown_030012BC
_0801930C:
	ldr r0, _08019320 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x32
	bl PlaySfx
_0801931A:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08019320: .4byte gUnknown_030012BC

	thumb_func_start sub_8019324
sub_8019324: @ 0x08019324
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x30
	mov sl, r0
	mov r8, r1
	ldrb r0, [r1, #0xa]
	cmp r0, #1
	bne _08019406
	ldr r4, _080193A4 @ =gUnknown_030012D8
	ldr r1, [r4]
	mov r0, sp
	bl sub_8007CF8
	ldr r0, [sp, #8]
	add r6, sp, #0x10
	cmp r0, #0
	bne _08019360
	ldr r1, [r4]
	adds r0, r6, #0
	bl sub_8007C30
	mov r1, sp
	adds r0, r6, #0
	ldm r0!, {r2, r3, r5}
	stm r1!, {r2, r3, r5}
	ldr r0, [r0]
	str r0, [r1]
_08019360:
	adds r0, r6, #0
	mov r1, r8
	bl sub_8007C30
	mov r0, sp
	adds r1, r6, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _080193A8
	ldr r2, [r4]
	movs r1, #0x82
	lsls r1, r1, #1
	adds r0, r2, r1
	ldrb r0, [r0]
	cmp r0, #0
	bne _0801939A
	ldr r1, [r2, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #9
	movs r3, #0
	bl sub_803AD88
_0801939A:
	movs r0, #0
	mov r4, r8
	strb r0, [r4, #0xa]
	b _08019406
	.align 2, 0
_080193A4: .4byte gUnknown_030012D8
_080193A8:
	mov r5, sl
	ldrb r0, [r5, #0x10]
	cmp r0, #0
	beq _08019406
	ldr r0, _08019458 @ =gUnknown_030012F0
	ldr r0, [r0]
	ldr r0, [r0, #4]
	mov sb, r0
	movs r5, #0
	cmp r5, sb
	bge _08019406
	add r7, sp, #0x20
_080193C0:
	ldr r0, _08019458 @ =gUnknown_030012F0
	ldr r0, [r0]
	ldr r1, [r0, #0xc]
	lsls r0, r5, #2
	adds r0, r0, r1
	ldr r4, [r0]
	adds r0, r7, #0
	adds r1, r4, #0
	bl sub_8007B98
	adds r0, r7, #0
	adds r1, r6, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08019400
	mov r1, sl
	ldr r0, [r1, #0x14]
	ldr r2, [r0, #0xc]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #2
	bl sub_803AD80
	movs r0, #1
	strb r0, [r4, #0xa]
	movs r2, #0
	mov r1, r8
	strb r2, [r1, #0xa]
_08019400:
	adds r5, #1
	cmp r5, sb
	blt _080193C0
_08019406:
	mov r0, r8
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08019446
	movs r0, #1
	mov r3, r8
	ldrb r3, [r3, #0xc]
	orrs r0, r3
	mov r4, r8
	strb r0, [r4, #0xc]
	ldr r0, _0801945C @ =0x0000FFFF
	ldrh r5, [r4, #8]
	cmp r5, r0
	beq _08019446
	ldrh r3, [r4, #8]
	ldr r0, _08019460 @ =gUnknown_030012B4
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
_08019446:
	add sp, #0x30
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08019458: .4byte gUnknown_030012F0
_0801945C: .4byte 0x0000FFFF
_08019460: .4byte gUnknown_030012B4

	thumb_func_start sub_8019464
sub_8019464: @ 0x08019464
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	adds r3, r1, #0
	ldr r0, [r5, #8]
	cmp r0, #0
	bne _08019492
	movs r4, #0x1a
	ldr r0, [r3, #0x20]
	adds r2, r3, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r6, [r2]
	lsls r0, r6, #3
	subs r0, r0, r6
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _0801948C
	subs r4, r0, #1
_0801948C:
	str r4, [r3, #0x30]
	movs r0, #1
	str r0, [r5, #8]
_08019492:
	movs r1, #0x1a
	ldrb r0, [r3, #0xa]
	cmp r0, #1
	bne _0801949C
	movs r1, #0xa
_0801949C:
	ldr r0, [r3, #0x30]
	cmp r0, r1
	bne _080194AC
	adds r1, r3, #0
	adds r1, #0x2c
	movs r0, #0
	strb r0, [r1]
	b _080194DA
_080194AC:
	movs r1, #1
	adds r0, r3, #0
	adds r0, #0x2c
	strb r1, [r0]
	adds r0, #0xc
	ldrb r0, [r0]
	cmp r0, #0
	beq _080194DA
	movs r4, #0
	ldr r0, [r3, #0x20]
	adds r2, r3, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r5, [r2]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _080194D8
	subs r4, r0, #1
_080194D8:
	str r4, [r3, #0x30]
_080194DA:
	pop {r4, r5, r6}
	pop {r0}
	bx r0

	thumb_func_start sub_80194E0
sub_80194E0: @ 0x080194E0
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	adds r4, r1, #0
	ldr r6, [r5, #8]
	cmp r6, #0
	beq _080194F2
	cmp r6, #1
	beq _0801958E
	b _080195C8
_080194F2:
	ldrb r0, [r4, #0xa]
	cmp r0, #1
	bne _080195C8
	ldr r0, _08019518 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x9f
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x20]
	ldr r0, [r5, #0x10]
	cmp r0, #1
	beq _08019538
	cmp r0, #1
	bgt _0801951C
	cmp r0, #0
	beq _08019522
	b _08019562
	.align 2, 0
_08019518: .4byte gUnknown_030012D0
_0801951C:
	cmp r0, #2
	beq _0801954E
	b _08019562
_08019522:
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r4, #0
	movs r2, #0xc
	bl sub_803AD84
	b _08019562
_08019538:
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r4, #0
	movs r2, #0xb
	bl sub_803AD84
	b _08019562
_0801954E:
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r4, #0
	movs r2, #0xd
	bl sub_803AD84
_08019562:
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
	ldr r1, [r5, #0xc]
	movs r4, #0x20
	ldrsh r0, [r1, r4]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #1
	bl sub_803AD80
	b _080195C8
_0801958E:
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _080195C8
	movs r0, #1
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _080195D0 @ =0x0000FFFF
	ldrh r2, [r4, #8]
	cmp r2, r0
	beq _080195C8
	ldrh r3, [r4, #8]
	ldr r0, _080195D4 @ =gUnknown_030012B4
	ldr r1, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r2, r0, #2
	movs r4, #0x84
	lsls r4, r4, #1
	adds r1, r1, r4
	adds r1, r1, r2
	lsls r0, r0, #5
	subs r0, r3, r0
	lsls r6, r0
	ldr r0, [r1]
	orrs r0, r6
	str r0, [r1]
_080195C8:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_080195D0: .4byte 0x0000FFFF
_080195D4: .4byte gUnknown_030012B4

	thumb_func_start sub_80195D8
sub_80195D8: @ 0x080195D8
	push {lr}
	ldr r2, _080195E8 @ =gStaticData_087E45CC
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_080195E8: .4byte gStaticData_087E45CC

	thumb_func_start sub_80195EC
sub_80195EC: @ 0x080195EC
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	bl sub_800B8C8
	ldr r0, _08019604 @ =gStaticData_087E45CC
	str r0, [r4, #0xc]
	str r5, [r4, #0x10]
	adds r0, r4, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08019604: .4byte gStaticData_087E45CC

	thumb_func_start sub_8019608
sub_8019608: @ 0x08019608
	push {lr}
	ldr r2, _08019618 @ =gStaticData_087E4634
	str r2, [r0, #0xc]
	bl sub_801B7C4
	pop {r0}
	bx r0
	.align 2, 0
_08019618: .4byte gStaticData_087E4634

	thumb_func_start sub_801961C
sub_801961C: @ 0x0801961C
	push {r4, lr}
	sub sp, #8
	adds r4, r0, #0
	mov r1, sp
	movs r0, #0
	strb r0, [r1]
	movs r0, #6
	str r0, [sp, #4]
	adds r0, r4, #0
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_801B7D8
	ldr r0, _08019648 @ =gStaticData_087E4634
	str r0, [r4, #0xc]
	adds r0, r4, #0
	add sp, #8
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08019648: .4byte gStaticData_087E4634

	thumb_func_start sub_801964C
sub_801964C: @ 0x0801964C
	push {lr}
	ldr r2, _0801965C @ =gStaticData_087E469C
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_0801965C: .4byte gStaticData_087E469C

	thumb_func_start sub_8019660
sub_8019660: @ 0x08019660
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	bl sub_800B8C8
	ldr r0, _08019678 @ =gStaticData_087E469C
	str r0, [r4, #0xc]
	str r5, [r4, #0x14]
	adds r0, r4, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08019678: .4byte gStaticData_087E469C

	thumb_func_start sub_801967C
sub_801967C: @ 0x0801967C
	push {r4, r5, r6, lr}
	lsls r1, r1, #0x18
	lsrs r3, r1, #0x18
	ldr r1, _080196A4 @ =gUnknown_030012EC
	ldr r0, [r1]
	ldr r4, [r0, #4]
	movs r2, #0
	cmp r2, r4
	bge _080196B0
	adds r6, r1, #0
	movs r5, #1
_08019692:
	ldr r0, [r6]
	ldr r1, [r0, #0xc]
	lsls r0, r2, #2
	adds r0, r0, r1
	ldr r0, [r0]
	cmp r3, #0
	beq _080196A8
	strb r5, [r0, #0xa]
	b _080196AA
	.align 2, 0
_080196A4: .4byte gUnknown_030012EC
_080196A8:
	strb r3, [r0, #0xa]
_080196AA:
	adds r2, #1
	cmp r2, r4
	blt _08019692
_080196B0:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80196B8
sub_80196B8: @ 0x080196B8
	push {r4, lr}
	str r2, [r0, #0x14]
	str r3, [r0, #0x18]
	ldr r4, [r1]
	subs r2, r2, r4
	str r2, [r0, #0x1c]
	ldr r1, [r1, #4]
	subs r3, r3, r1
	str r3, [r0, #0x20]
	ldr r1, [r0, #0x3c]
	ldr r1, [r1, #0x10]
	ldr r2, _080196E0 @ =gStaticData_0816C358
	adds r1, r1, r2
	ldrb r1, [r1]
	str r1, [r0, #0x28]
	str r1, [r0, #0x24]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080196E0: .4byte gStaticData_0816C358

	thumb_func_start sub_80196E4
sub_80196E4: @ 0x080196E4
	push {lr}
	ldr r2, _080196F4 @ =gStaticData_087E4704
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_080196F4: .4byte gStaticData_087E4704

	thumb_func_start sub_80196F8
sub_80196F8: @ 0x080196F8
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	bl sub_800B8C8
	ldr r0, _08019714 @ =gStaticData_087E4704
	str r0, [r4, #0xc]
	movs r0, #0
	str r0, [r4, #0x24]
	str r5, [r4, #0x3c]
	adds r0, r4, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08019714: .4byte gStaticData_087E4704

	thumb_func_start sub_8019718
sub_8019718: @ 0x08019718
	push {r4, lr}
	adds r1, r2, #0
	ldr r3, [r0, #0xc]
	movs r4, #0x20
	ldrsh r2, [r3, r4]
	adds r0, r0, r2
	ldr r2, [r3, #0x24]
	bl sub_803AD80
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8019730
sub_8019730: @ 0x08019730
	ldr r0, [r0, #8]
	cmp r0, #0
	bne _08019740
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xc]
	ands r0, r2
	strb r0, [r1, #0xc]
_08019740:
	bx lr
	.align 2, 0

	thumb_func_start sub_8019744
sub_8019744: @ 0x08019744
	push {lr}
	ldr r2, _08019754 @ =gStaticData_087E476C
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_08019754: .4byte gStaticData_087E476C

	thumb_func_start sub_8019758
sub_8019758: @ 0x08019758
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800B8C8
	ldr r0, _0801976C @ =gStaticData_087E476C
	str r0, [r4, #0xc]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801976C: .4byte gStaticData_087E476C

	thumb_func_start sub_8019770
sub_8019770: @ 0x08019770
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r2, #0
	cmp r5, #3
	bne _080197A8
	ldr r0, [r4, #0x20]
	ldr r0, [r0, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #9
	bl sub_803AD80
	ldr r0, _080197C0 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231C4
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _080197A8
	ldr r0, _080197C4 @ =0x0000FFFF
	movs r1, #0x8c
	movs r2, #0x98
	movs r3, #0
	bl sub_8021D80
_080197A8:
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	adds r1, r5, #0
	bl sub_803AD80
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080197C0: .4byte gUnknown_030012C0
_080197C4: .4byte 0x0000FFFF

	thumb_func_start sub_80197C8
sub_80197C8: @ 0x080197C8
	push {lr}
	ldr r2, _080197D8 @ =gStaticData_087E47D4
	str r2, [r0, #0xc]
	bl sub_8017A78
	pop {r0}
	bx r0
	.align 2, 0
_080197D8: .4byte gStaticData_087E47D4

	thumb_func_start sub_80197DC
sub_80197DC: @ 0x080197DC
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8017A8C
	ldr r0, _080197F0 @ =gStaticData_087E47D4
	str r0, [r4, #0xc]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_080197F0: .4byte gStaticData_087E47D4

	thumb_func_start sub_80197F4
sub_80197F4: @ 0x080197F4
	ldr r0, [r0, #0x10]
	bx lr

	thumb_func_start sub_80197F8
sub_80197F8: @ 0x080197F8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x24
	adds r5, r0, #0
	adds r4, r1, #0
	adds r0, r4, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _0801981E
	ldr r0, [r4]
	ldr r2, [r4, #4]
	ldr r1, [r5, #0x2c]
	movs r3, #0xc0
	lsls r3, r3, #3
	adds r0, r0, r3
	b _08019828
_0801981E:
	ldr r0, [r4]
	ldr r2, [r4, #4]
	ldr r1, [r5, #0x2c]
	ldr r6, _08019884 @ =0xFFFFFA00
	adds r0, r0, r6
_08019828:
	str r0, [r1]
	str r2, [r1, #4]
	add r0, sp, #4
	adds r1, r4, #0
	bl sub_8007CF8
	ldr r0, [r5, #8]
	cmp r0, #8
	bne _08019870
	ldr r0, _08019888 @ =gUnknown_030012D8
	ldr r1, [r0]
	ldrb r7, [r1, #0xa]
	cmp r7, #0x13
	bne _08019870
	add r6, sp, #0x14
	adds r0, r6, #0
	bl sub_8007C30
	ldr r0, [sp, #0x1c]
	cmp r0, #0
	beq _08019870
	adds r0, r6, #0
	add r1, sp, #4
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08019870
	ldr r0, [r5, #0x10]
	adds r0, #1
	str r0, [r5, #0x10]
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0xb
	bl sub_8019CE4
_08019870:
	ldr r3, [r5, #8]
	cmp r3, #0x11
	bls _08019878
	b _08019CD0
_08019878:
	lsls r0, r3, #2
	ldr r1, _0801988C @ =_08019890
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08019884: .4byte 0xFFFFFA00
_08019888: .4byte gUnknown_030012D8
_0801988C: .4byte _08019890
_08019890: @ jump table
	.4byte _080198D8 @ case 0
	.4byte _080198FA @ case 1
	.4byte _08019BA0 @ case 2
	.4byte _080199E4 @ case 3
	.4byte _08019AEC @ case 4
	.4byte _08019AFA @ case 5
	.4byte _08019AFA @ case 6
	.4byte _08019BB8 @ case 7
	.4byte _08019BE2 @ case 8
	.4byte _08019BF8 @ case 9
	.4byte _08019BF8 @ case 10
	.4byte _08019C1E @ case 11
	.4byte _08019C50 @ case 12
	.4byte _080199E4 @ case 13
	.4byte _080198FA @ case 14
	.4byte _08019AAE @ case 15
	.4byte _08019C94 @ case 16
	.4byte _08019CD0 @ case 17
_080198D8:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #1
	bl sub_8019CE4
	movs r0, #0
	str r0, [r5, #0x1c]
	str r0, [r5, #0x28]
	strb r0, [r4, #0xa]
	subs r0, #5
	ldrb r1, [r4, #0xc]
	ands r0, r1
	movs r1, #0x41
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r4, #0xc]
	b _08019CD0
_080198FA:
	adds r6, r4, #0
	adds r6, #0x28
	ldr r2, [r4]
	cmp r3, #1
	bne _08019980
	ldrb r7, [r6]
	lsls r0, r7, #0x1b
	cmp r0, #0
	bge _08019938
	ldr r1, _0801992C @ =gStaticData_0816C368
	ldr r0, [r5, #0x10]
	cmp r0, #0
	ble _08019916
	ldr r1, _08019930 @ =gStaticData_0816C378
_08019916:
	ldr r0, [r5, #0x1c]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	cmp r2, r0
	bgt _08019980
	ldr r0, _08019934 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0]
	subs r0, r0, r2
	b _08019956
	.align 2, 0
_0801992C: .4byte gStaticData_0816C368
_08019930: .4byte gStaticData_0816C378
_08019934: .4byte gUnknown_030012D8
_08019938:
	ldr r1, _08019970 @ =gStaticData_0816C390
	ldr r0, [r5, #0x10]
	cmp r0, #0
	ble _08019942
	ldr r1, _08019974 @ =gStaticData_0816C3A0
_08019942:
	ldr r0, [r5, #0x1c]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	cmp r2, r0
	blt _08019980
	ldr r0, _08019978 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0]
	subs r0, r2, r0
_08019956:
	ldr r1, _0801997C @ =0x00001FFF
	cmp r0, r1
	bgt _08019966
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #5
	bl sub_8019CE4
_08019966:
	ldr r0, [r5, #0x1c]
	adds r0, #1
	str r0, [r5, #0x1c]
	b _08019CD0
	.align 2, 0
_08019970: .4byte gStaticData_0816C390
_08019974: .4byte gStaticData_0816C3A0
_08019978: .4byte gUnknown_030012D8
_0801997C: .4byte 0x00001FFF
_08019980:
	ldrb r6, [r6]
	lsls r0, r6, #0x1b
	cmp r0, #0
	bge _08019994
	movs r1, #0
	movs r0, #0x80
	lsls r0, r0, #6
	cmp r2, r0
	bgt _080199AA
	b _080199A8
_08019994:
	ldr r0, _080199DC @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x10]
	lsls r0, r0, #8
	movs r1, #0
	ldr r6, _080199E0 @ =0xFFFFE000
	adds r0, r0, r6
	cmp r2, r0
	blt _080199AA
_080199A8:
	movs r1, #1
_080199AA:
	adds r0, r1, #0
	cmp r0, #0
	bne _080199B2
	b _08019CD0
_080199B2:
	cmp r3, #1
	beq _080199B8
	b _08019B6A
_080199B8:
	movs r1, #1
	ldr r0, [r5, #0x10]
	cmp r0, #0
	ble _080199C2
	movs r1, #2
_080199C2:
	ldr r0, [r5, #0x28]
	adds r0, #1
	str r0, [r5, #0x28]
	cmp r0, r1
	bge _080199CE
	b _08019B6A
_080199CE:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #6
	bl sub_8019CE4
	b _08019CD0
	.align 2, 0
_080199DC: .4byte gUnknown_03001308
_080199E0: .4byte 0xFFFFE000
_080199E4:
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	bne _080199F0
	b _08019CD0
_080199F0:
	mov r8, r3
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #1
	bl sub_8019CE4
	adds r0, r4, #0
	adds r0, #0x28
	ldrb r7, [r0]
	lsls r1, r7, #0x1b
	adds r6, r0, #0
	cmp r1, #0
	bge _08019A16
	ldr r0, [r4]
	asrs r0, r0, #8
	ldr r1, [r4, #4]
	asrs r1, r1, #8
	adds r0, #6
	b _08019A20
_08019A16:
	ldr r0, [r4]
	asrs r0, r0, #8
	ldr r1, [r4, #4]
	asrs r1, r1, #8
	subs r0, #6
_08019A20:
	lsls r0, r0, #8
	str r0, [r4]
	lsls r1, r1, #8
	str r1, [r4, #4]
	movs r3, #8
	ldr r0, [r4, #0x20]
	adds r2, r4, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r7, [r2]
	lsls r0, r7, #3
	adds r2, r7, #0
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08019A46
	subs r3, r0, #1
_08019A46:
	str r3, [r4, #0x30]
	ldrb r2, [r6]
	lsls r0, r2, #0x1b
	movs r1, #0
	cmp r0, #0
	blt _08019A54
	movs r1, #1
_08019A54:
	lsls r1, r1, #4
	movs r0, #0x11
	rsbs r0, r0, #0
	ands r0, r2
	orrs r0, r1
	strb r0, [r6]
	mov r0, r8
	cmp r0, #3
	bne _08019AA2
	ldr r0, [r5, #0x28]
	cmp r0, #0
	bne _08019A8A
	movs r0, #0x73
	str r0, [r5, #0x20]
	ldr r0, [r5, #0x10]
	cmp r0, #1
	ble _08019A7A
	movs r0, #0xf
	b _08019A7C
_08019A7A:
	movs r0, #1
_08019A7C:
	str r0, [r5, #0x24]
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #2
	bl sub_8019CE4
	b _08019A9C
_08019A8A:
	movs r0, #0x64
	str r0, [r5, #0x20]
	movs r0, #1
	str r0, [r5, #0x24]
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #2
	bl sub_8019CE4
_08019A9C:
	movs r0, #0
	str r0, [r5, #0x1c]
	b _08019CD0
_08019AA2:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0xe
	bl sub_8019CE4
	b _08019CD0
_08019AAE:
	adds r0, r4, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _08019AC8
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0x2d
	movs r3, #0
	bl sub_801A03C
	b _08019C06
_08019AC8:
	ldr r0, _08019AE8 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #0x10
	movs r2, #0xa0
	lsls r2, r2, #0xe
	adds r1, r1, r2
	lsrs r1, r1, #0x10
	adds r0, r5, #0
	movs r2, #0x2d
	movs r3, #1
	bl sub_801A03C
	b _08019C06
	.align 2, 0
_08019AE8: .4byte gUnknown_03001308
_08019AEC:
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	bne _08019AF8
	b _08019CD0
_08019AF8:
	b _08019C06
_08019AFA:
	ldr r0, [r4, #0x30]
	cmp r0, #0x14
	bne _08019B50
	ldr r0, [r4, #0x34]
	cmp r0, #0
	bne _08019B50
	adds r0, r4, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _08019B32
	ldr r2, [r4]
	asrs r2, r2, #8
	adds r2, #6
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	ldr r3, [r4, #4]
	asrs r3, r3, #8
	subs r3, #0x32
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	str r4, [sp]
	adds r0, r5, #0
	movs r1, #1
	bl sub_8019EBC
	b _08019B50
_08019B32:
	ldr r2, [r4]
	asrs r2, r2, #8
	subs r2, #6
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	ldr r3, [r4, #4]
	asrs r3, r3, #8
	subs r3, #0x32
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	str r4, [sp]
	adds r0, r5, #0
	movs r1, #1
	bl sub_8019EBC
_08019B50:
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	bne _08019B5C
	b _08019CD0
_08019B5C:
	ldr r0, [r5, #8]
	cmp r0, #6
	bne _08019B76
	movs r0, #0x40
	ldrb r3, [r4, #0xc]
	orrs r0, r3
	strb r0, [r4, #0xc]
_08019B6A:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #3
	bl sub_8019CE4
	b _08019CD0
_08019B76:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #1
	bl sub_8019CE4
	movs r3, #8
	ldr r0, [r4, #0x20]
	adds r2, r4, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r5, [r2]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08019B9C
	subs r3, r0, #1
_08019B9C:
	str r3, [r4, #0x30]
	b _08019CD0
_08019BA0:
	ldr r0, [r5, #0x20]
	subs r0, #1
	str r0, [r5, #0x20]
	cmp r0, #0
	beq _08019BAC
	b _08019CD0
_08019BAC:
	ldr r2, [r5, #0x24]
	adds r0, r5, #0
	adds r1, r4, #0
	bl sub_8019CE4
	b _08019CD0
_08019BB8:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0
	bl sub_801A7AC
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r6, #0
	ldrsh r0, [r1, r6]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r4, #0
	movs r2, #5
	bl sub_803AD84
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #8
	bl sub_8019CE4
	b _08019CD0
_08019BE2:
	ldr r0, [r5, #0x20]
	subs r0, #1
	str r0, [r5, #0x20]
	cmp r0, #0
	bne _08019CD0
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #9
	bl sub_8019CE4
	b _08019CD0
_08019BF8:
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08019CD0
	cmp r3, #9
	bne _08019C12
_08019C06:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #1
	bl sub_8019CE4
	b _08019CD0
_08019C12:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0xc
	bl sub_8019CE4
	b _08019CD0
_08019C1E:
	ldr r0, [r5, #0x20]
	cmp r0, #0
	beq _08019C2A
	subs r0, #1
	str r0, [r5, #0x20]
	b _08019CD0
_08019C2A:
	ldr r0, [r5, #0x10]
	cmp r0, #2
	ble _08019C3A
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0x10
	bl sub_8019CE4
_08019C3A:
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08019CD0
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0xa
	bl sub_8019CE4
	b _08019CD0
_08019C50:
	movs r2, #0x80
	lsls r2, r2, #7
	ldr r0, [r5, #0x10]
	cmp r0, #0
	ble _08019C5E
	movs r2, #0x80
	lsls r2, r2, #6
_08019C5E:
	adds r0, r4, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _08019C72
	ldr r0, [r4]
	cmp r0, r2
	bgt _08019CD0
	b _08019C84
_08019C72:
	ldr r1, [r4]
	ldr r0, _08019C90 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x10]
	lsls r0, r0, #8
	subs r0, r0, r2
	cmp r1, r0
	blt _08019CD0
_08019C84:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0xd
	bl sub_8019CE4
	b _08019CD0
	.align 2, 0
_08019C90: .4byte gUnknown_03001308
_08019C94:
	ldr r1, [r4, #4]
	ldr r0, _08019CDC @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x14]
	lsls r0, r0, #8
	movs r7, #0x80
	lsls r7, r7, #6
	adds r0, r0, r7
	cmp r1, r0
	blt _08019CD0
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0
	bl sub_801A7AC
	ldr r0, _08019CE0 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231BC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08019CC6
	bl sub_80241A4
_08019CC6:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0x11
	bl sub_8019CE4
_08019CD0:
	add sp, #0x24
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08019CDC: .4byte gUnknown_03001308
_08019CE0: .4byte gUnknown_030012C0

	thumb_func_start sub_8019CE4
sub_8019CE4: @ 0x08019CE4
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	adds r4, r2, #0
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	adds r1, r4, #0
	bl sub_803AD80
	subs r0, r4, #1
	cmp r0, #0xf
	bls _08019D04
	b _08019EB0
_08019D04:
	lsls r0, r0, #2
	ldr r1, _08019D10 @ =_08019D14
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08019D10: .4byte _08019D14
_08019D14: @ jump table
	.4byte _08019DBC @ case 0
	.4byte _08019E1C @ case 1
	.4byte _08019DE0 @ case 2
	.4byte _08019DF2 @ case 3
	.4byte _08019E08 @ case 4
	.4byte _08019E04 @ case 5
	.4byte _08019EB0 @ case 6
	.4byte _08019E28 @ case 7
	.4byte _08019E42 @ case 8
	.4byte _08019E42 @ case 9
	.4byte _08019E6C @ case 10
	.4byte _08019D84 @ case 11
	.4byte _08019DE0 @ case 12
	.4byte _08019DBC @ case 13
	.4byte _08019EB0 @ case 14
	.4byte _08019D54 @ case 15
_08019D54:
	ldr r0, _08019D7C @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231BC
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08019D6E
	ldr r0, _08019D80 @ =0x0000FFFF
	movs r1, #0xa0
	movs r2, #0xa9
	movs r3, #0
	bl sub_8021EF4
_08019D6E:
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #3
	bl sub_801A7AC
	b _08019EB0
	.align 2, 0
_08019D7C: .4byte gUnknown_030012C0
_08019D80: .4byte 0x0000FFFF
_08019D84:
	ldr r4, _08019DDC @ =gUnknown_03001308
	ldr r0, [r4]
	ldr r0, [r0, #0x10]
	ldrh r1, [r0, #0x10]
	adds r0, r5, #0
	movs r2, #0x28
	movs r3, #1
	bl sub_801A03C
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0x46
	movs r3, #0
	bl sub_801A03C
	ldr r0, [r4]
	ldr r0, [r0, #0x10]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #0x10
	movs r3, #0x8c
	lsls r3, r3, #0xf
	adds r1, r1, r3
	lsrs r1, r1, #0x10
	adds r0, r5, #0
	movs r2, #0x64
	movs r3, #1
	bl sub_801A03C
_08019DBC:
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #0
	bl sub_803AD84
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #1
	bl sub_801A7AC
	b _08019EB0
	.align 2, 0
_08019DDC: .4byte gUnknown_03001308
_08019DE0:
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #6
	b _08019E18
_08019DF2:
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #1
	b _08019E18
_08019E04:
	movs r0, #0
	str r0, [r5, #0x28]
_08019E08:
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #4
_08019E18:
	bl sub_803AD84
_08019E1C:
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #0
	bl sub_801A7AC
	b _08019EB0
_08019E28:
	ldr r0, [r5, #0x2c]
	ldr r0, [r0, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #2
	bl sub_803AD80
	movs r0, #0xd2
	str r0, [r5, #0x20]
	b _08019EB0
_08019E42:
	ldr r0, [r5, #0x2c]
	ldr r0, [r0, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #1
	bl sub_803AD80
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #2
	bl sub_803AD84
	b _08019EB0
_08019E6C:
	movs r0, #0x64
	str r0, [r5, #0x20]
	ldr r0, [r5, #0x10]
	cmp r0, #2
	ble _08019E7A
	movs r0, #1
	str r0, [r5, #0x20]
_08019E7A:
	ldr r0, _08019EB8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x15
	bl PlaySfx
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r3, [r6, #0xc]
	ands r0, r3
	strb r0, [r6, #0xc]
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #2
	bl sub_801A7AC
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #1
	bl sub_803AD84
_08019EB0:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08019EB8: .4byte gUnknown_030012BC

	thumb_func_start sub_8019EBC
sub_8019EBC: @ 0x08019EBC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r0
	adds r7, r1, #0
	adds r1, r2, #0
	adds r2, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	ldr r0, _08019F04 @ =0x0000FFFF
	movs r3, #0
	bl sub_8009ED0
	adds r6, r0, #0
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r1, [r6, #0xc]
	ands r0, r1
	strb r0, [r6, #0xc]
	ldr r0, _08019F08 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r2, #0xa2
	lsls r2, r2, #2
	adds r0, r0, r2
	str r0, [r6, #0x20]
	cmp r7, #0
	beq _08019F0C
	cmp r7, #1
	beq _08019F54
	movs r5, #0
	b _08019F92
	.align 2, 0
_08019F04: .4byte 0x0000FFFF
_08019F08: .4byte gUnknown_030012D0
_08019F0C:
	movs r5, #1
	adds r4, r6, #0
	adds r4, #0x28
	movs r1, #1
	movs r0, #4
	rsbs r0, r0, #0
	ldrb r3, [r4]
	ands r0, r3
	orrs r0, r1
	strb r0, [r4]
	movs r0, #3
	adds r1, r6, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r6, #0
	bl sub_80087C0
	adds r0, r6, #0
	bl sub_80087B4
	adds r0, r6, #0
	movs r1, #0
	bl sub_800872C
	strb r5, [r6, #0xa]
	movs r0, #0x28
	bl sub_8026EDC
	bl sub_801A794
	adds r5, r0, #0
	ldr r0, [sp, #0x18]
	str r0, [r5, #0x24]
	mov r1, r8
	str r6, [r1, #0x2c]
	b _08019F96
_08019F54:
	ldr r0, _0801A018 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x29
	bl PlaySfx
	movs r0, #7
	adds r1, r6, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r6, #0
	bl sub_80087C0
	adds r0, r6, #0
	bl sub_80087B4
	adds r0, r6, #0
	movs r1, #0
	bl sub_800872C
	movs r0, #4
	strb r0, [r6, #0xa]
	movs r0, #0x20
	bl sub_8026EDC
	bl sub_801A768
	adds r5, r0, #0
	ldr r2, [sp, #0x18]
	str r2, [r5, #0x1c]
_08019F92:
	adds r4, r6, #0
	adds r4, #0x28
_08019F96:
	adds r0, r6, #0
	bl sub_800815C
	adds r2, r6, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	str r5, [r6, #0x44]
	ldr r1, [r5, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x1c]
	adds r1, r6, #0
	bl sub_803AD80
	ldr r0, _0801A01C @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	ldr r3, [r0, #0xc]
	ldrh r1, [r1]
	adds r3, r1, r3
	ldrb r5, [r3]
	lsrs r0, r5, #1
	movs r2, #1
	eors r0, r2
	ands r0, r2
	ands r0, r2
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r5, [r4]
	ands r1, r5
	orrs r1, r0
	strb r1, [r4]
	ldrb r3, [r3]
	lsrs r0, r3, #2
	ands r0, r2
	ands r0, r2
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r4]
	movs r0, #0x10
	ldrb r1, [r6, #0xc]
	orrs r0, r1
	strb r0, [r6, #0xc]
	cmp r7, #0
	bne _0801A024
	ldr r0, _0801A020 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r6, #0
	bl sub_8008E94
	b _0801A02E
	.align 2, 0
_0801A018: .4byte gUnknown_030012BC
_0801A01C: .4byte gUnknown_030012B4
_0801A020: .4byte gUnknown_030012EC
_0801A024:
	ldr r0, _0801A038 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r6, #0
	bl sub_8008E94
_0801A02E:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801A038: .4byte gUnknown_030012F0

	thumb_func_start sub_801A03C
sub_801A03C: @ 0x0801A03C
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r6, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r6, r6, #0x18
	lsrs r6, r6, #0x18
	ldr r0, _0801A108 @ =0x0000FFFF
	movs r3, #0
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _0801A10C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x30
	str r0, [r4, #0x20]
	movs r0, #1
	adds r1, r4, #0
	adds r1, #0x2d
	movs r2, #1
	mov r8, r2
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	movs r0, #6
	strb r0, [r4, #0xa]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_801A724
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
	adds r0, r5, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	adds r1, r4, #0
	adds r1, #0x28
	mov r3, r8
	ands r6, r3
	lsls r6, r6, #4
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r2, [r1]
	ands r0, r2
	orrs r0, r6
	strb r0, [r1]
	movs r0, #0x10
	ldrb r3, [r4, #0xc]
	orrs r0, r3
	strb r0, [r4, #0xc]
	ldr r1, [r5, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r5, r5, r0
	ldr r2, [r1, #0x1c]
	adds r0, r5, #0
	adds r1, r4, #0
	bl sub_803AD80
	ldr r0, _0801A110 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801A108: .4byte 0x0000FFFF
_0801A10C: .4byte gUnknown_030012D0
_0801A110: .4byte gUnknown_030012F0

	thumb_func_start sub_801A114
sub_801A114: @ 0x0801A114
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x30
	adds r7, r0, #0
	mov sb, r1
	ldr r1, [r1, #0x18]
	movs r2, #0x28
	ldrsh r0, [r1, r2]
	add r0, sb
	ldr r1, [r1, #0x2c]
	bl sub_803AD7C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801A1A6
	ldr r3, _0801A1B8 @ =gUnknown_030012D8
	mov sl, r3
	ldr r0, [r3]
	movs r1, #0x82
	lsls r1, r1, #1
	adds r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0
	bne _0801A1A6
	mov r0, sp
	mov r1, sb
	bl sub_8007C30
	add r2, sp, #0x10
	mov r8, r2
	mov r3, sl
	ldr r1, [r3]
	mov r0, r8
	bl sub_8007CF8
	ldr r0, [sp, #0x18]
	cmp r0, #0
	bne _0801A17C
	mov r0, sl
	ldr r1, [r0]
	add r4, sp, #0x20
	adds r0, r4, #0
	bl sub_8007C30
	mov r0, r8
	ldm r4!, {r1, r2, r3}
	stm r0!, {r1, r2, r3}
	ldr r1, [r4]
	str r1, [r0]
_0801A17C:
	mov r0, r8
	mov r1, sp
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801A1A6
	mov r1, sl
	ldr r0, [r1]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	mov r3, sb
	ldrb r2, [r3, #0xa]
	ldr r4, [r1, #4]
	movs r1, #0
	movs r3, #0
	bl sub_803AD88
_0801A1A6:
	ldr r0, [r7, #8]
	cmp r0, #5
	bhi _0801A298
	lsls r0, r0, #2
	ldr r1, _0801A1BC @ =_0801A1C0
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801A1B8: .4byte gUnknown_030012D8
_0801A1BC: .4byte _0801A1C0
_0801A1C0: @ jump table
	.4byte _0801A1D8 @ case 0
	.4byte _0801A220 @ case 1
	.4byte _0801A23A @ case 2
	.4byte _0801A254 @ case 3
	.4byte _0801A254 @ case 4
	.4byte _0801A298 @ case 5
_0801A1D8:
	movs r5, #0x10
	movs r0, #0x80
	lsls r0, r0, #1
	orrs r5, r0
	movs r0, #0x80
	lsls r0, r0, #2
	orrs r5, r0
	movs r0, #0x80
	lsls r0, r0, #3
	orrs r5, r0
	movs r0, #0x80
	lsls r0, r0, #4
	orrs r5, r0
	movs r0, #0x80
	lsls r0, r0, #5
	orrs r0, r5
	movs r1, #0x80
	lsls r1, r1, #0xd
	orrs r0, r1
	movs r1, #0x80
	lsls r1, r1, #0x15
	orrs r0, r1
	ldr r1, _0801A21C @ =0x04000050
	str r0, [r1]
	ldr r1, [r7, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r7, r0
	ldr r2, [r1, #0x24]
	movs r1, #5
	bl sub_803AD80
	b _0801A298
	.align 2, 0
_0801A21C: .4byte 0x04000050
_0801A220:
	movs r0, #2
	str r0, [r7, #0x20]
	movs r0, #0
	str r0, [r7, #0x1c]
	ldr r1, [r7, #0xc]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r7, r0
	ldr r2, [r1, #0x24]
	movs r1, #3
	bl sub_803AD80
	b _0801A298
_0801A23A:
	movs r0, #2
	str r0, [r7, #0x20]
	movs r0, #0
	str r0, [r7, #0x1c]
	ldr r1, [r7, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r7, r0
	ldr r2, [r1, #0x24]
	movs r1, #4
	bl sub_803AD80
	b _0801A298
_0801A254:
	ldr r0, [r7, #0x1c]
	cmp r0, #0
	bne _0801A294
	movs r0, #0x14
	str r0, [r7, #0x1c]
	mov r3, sb
	ldrb r2, [r3, #0xd]
	lsrs r1, r2, #2
	movs r0, #1
	eors r1, r0
	ands r1, r0
	lsls r1, r1, #2
	movs r0, #5
	rsbs r0, r0, #0
	ands r0, r2
	orrs r0, r1
	strb r0, [r3, #0xd]
	ldr r0, [r7, #0x20]
	cmp r0, #0
	bne _0801A28C
	ldr r1, [r7, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r7, r0
	ldr r2, [r1, #0x24]
	movs r1, #5
	bl sub_803AD80
_0801A28C:
	ldr r0, [r7, #0x20]
	subs r0, #1
	str r0, [r7, #0x20]
	ldr r0, [r7, #0x1c]
_0801A294:
	subs r0, #1
	str r0, [r7, #0x1c]
_0801A298:
	add sp, #0x30
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_801A2A8
sub_801A2A8: @ 0x0801A2A8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x30
	adds r5, r0, #0
	adds r6, r1, #0
	mov r0, sp
	bl sub_8007C30
	ldr r0, [sp, #8]
	cmp r0, #0
	bne _0801A2C2
	b _0801A3CE
_0801A2C2:
	ldr r0, [r5, #8]
	cmp r0, #4
	beq _0801A338
	cmp r0, #6
	beq _0801A338
	ldr r2, [r5, #0x1c]
	ldrb r1, [r2, #0xc]
	lsrs r0, r1, #6
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _0801A338
	add r4, sp, #0x10
	adds r0, r4, #0
	adds r1, r2, #0
	bl sub_8007B98
	mov r0, sp
	adds r1, r4, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801A338
	ldr r0, [r5, #0x1c]
	ldr r0, [r0, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #7
	bl sub_803AD80
	ldr r1, [r5, #0xc]
	movs r4, #0x20
	ldrsh r0, [r1, r4]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #6
	bl sub_803AD80
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #8
	bl sub_803AD84
	ldr r0, _0801A3E0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x39
	bl PlaySfx
_0801A338:
	ldr r3, _0801A3E4 @ =gUnknown_030012D8
	mov r8, r3
	ldr r1, [r3]
	movs r4, #0x82
	lsls r4, r4, #1
	adds r0, r1, r4
	ldrb r0, [r0]
	cmp r0, #0
	bne _0801A3CE
	add r7, sp, #0x10
	adds r0, r7, #0
	bl sub_8007CF8
	ldr r0, [sp, #0x18]
	cmp r0, #0
	bne _0801A36E
	mov r0, r8
	ldr r1, [r0]
	add r4, sp, #0x20
	adds r0, r4, #0
	bl sub_8007C30
	adds r0, r7, #0
	ldm r4!, {r1, r2, r3}
	stm r0!, {r1, r2, r3}
	ldr r1, [r4]
	str r1, [r0]
_0801A36E:
	mov r0, sp
	adds r1, r7, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801A3CE
	mov r4, r8
	ldr r0, [r4]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldrb r2, [r6, #0xa]
	ldr r4, [r1, #4]
	movs r1, #0
	movs r3, #0
	bl sub_803AD88
	ldr r0, [r5, #8]
	cmp r0, #3
	bne _0801A3CE
	ldr r1, [r5, #0xc]
	movs r4, #0x20
	ldrsh r0, [r1, r4]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #6
	bl sub_803AD80
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #8
	bl sub_803AD84
	ldr r0, _0801A3E0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x39
	bl PlaySfx
_0801A3CE:
	ldr r0, [r5, #8]
	cmp r0, #6
	bls _0801A3D6
	b _0801A56E
_0801A3D6:
	lsls r0, r0, #2
	ldr r1, _0801A3E8 @ =_0801A3EC
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801A3E0: .4byte gUnknown_030012BC
_0801A3E4: .4byte gUnknown_030012D8
_0801A3E8: .4byte _0801A3EC
_0801A3EC: @ jump table
	.4byte _0801A408 @ case 0
	.4byte _0801A430 @ case 1
	.4byte _0801A532 @ case 2
	.4byte _0801A4D4 @ case 3
	.4byte _0801A4D4 @ case 4
	.4byte _0801A498 @ case 5
	.4byte _0801A528 @ case 6
_0801A408:
	ldr r1, [r5, #0xc]
	movs r3, #0x30
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r2, _0801A42C @ =gStaticData_0816C3E8
	ldr r3, [r1, #0x34]
	adds r1, r6, #0
	bl sub_803AD84
	ldr r1, [r5, #0xc]
	movs r4, #0x20
	ldrsh r0, [r1, r4]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #1
	bl sub_803AD80
	b _0801A56E
	.align 2, 0
_0801A42C: .4byte gStaticData_0816C3E8
_0801A430:
	ldr r1, [r6, #4]
	movs r0, #0x80
	lsls r0, r0, #4
	cmp r1, r0
	ble _0801A43C
	b _0801A56E
_0801A43C:
	movs r0, #0
	str r0, [r6, #0x64]
	str r0, [r6, #0x54]
	str r0, [r6, #0x58]
	str r0, [r6, #0x5c]
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #8
	bl sub_803AD84
	adds r0, r6, #0
	bl sub_800815C
	adds r2, r6, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldr r1, [r6]
	lsls r1, r1, #8
	lsrs r1, r1, #0x10
	ldr r2, [r6, #4]
	lsls r2, r2, #8
	lsrs r2, r2, #0x10
	adds r0, r5, #0
	bl sub_801A584
	ldr r1, [r5, #0xc]
	movs r4, #0x20
	ldrsh r0, [r1, r4]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #2
	bl sub_803AD80
	b _0801A56E
_0801A498:
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #9
	bl sub_803AD84
	ldr r1, [r5, #0xc]
	movs r3, #0x30
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r2, _0801A4D0 @ =gStaticData_0816C3F4
	ldr r3, [r1, #0x34]
	adds r1, r6, #0
	bl sub_803AD84
	ldr r1, [r5, #0xc]
	movs r4, #0x20
	ldrsh r0, [r1, r4]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #3
	bl sub_803AD80
	b _0801A56E
	.align 2, 0
_0801A4D0: .4byte gStaticData_0816C3F4
_0801A4D4:
	ldr r1, [r6, #4]
	ldr r0, _0801A51C @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x14]
	lsls r0, r0, #8
	ldr r2, _0801A520 @ =0xFFFFE000
	adds r0, r0, r2
	cmp r1, r0
	blt _0801A56E
	ldr r1, [r5, #0xc]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #6
	bl sub_803AD80
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r4, #0
	ldrsh r0, [r1, r4]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #8
	bl sub_803AD84
	ldr r0, _0801A524 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x39
	bl PlaySfx
	b _0801A56E
	.align 2, 0
_0801A51C: .4byte gUnknown_03001308
_0801A520: .4byte 0xFFFFE000
_0801A524: .4byte gUnknown_030012BC
_0801A528:
	movs r0, #0
	str r0, [r6, #0x64]
	str r0, [r6, #0x54]
	str r0, [r6, #0x58]
	str r0, [r6, #0x5c]
_0801A532:
	adds r0, r6, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801A56E
	movs r0, #1
	ldrb r1, [r6, #0xc]
	orrs r0, r1
	strb r0, [r6, #0xc]
	ldr r0, _0801A57C @ =0x0000FFFF
	ldrh r2, [r6, #8]
	cmp r2, r0
	beq _0801A56E
	ldrh r3, [r6, #8]
	ldr r0, _0801A580 @ =gUnknown_030012B4
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
_0801A56E:
	add sp, #0x30
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801A57C: .4byte 0x0000FFFF
_0801A580: .4byte gUnknown_030012B4

	thumb_func_start sub_801A584
sub_801A584: @ 0x0801A584
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	ldr r0, _0801A63C @ =0x0000FFFF
	movs r3, #0
	bl sub_8009ED0
	adds r5, r0, #0
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r1, [r5, #0xc]
	ands r0, r1
	strb r0, [r5, #0xc]
	ldr r0, _0801A640 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r2, #0xa2
	lsls r2, r2, #2
	adds r0, r0, r2
	str r0, [r5, #0x20]
	movs r0, #8
	adds r1, r5, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r5, #0
	bl sub_80087C0
	adds r0, r5, #0
	bl sub_80087B4
	adds r0, r5, #0
	movs r1, #0
	bl sub_800872C
	movs r0, #1
	strb r0, [r5, #0xa]
	movs r0, #0x20
	bl sub_8026EDC
	adds r4, r0, #0
	bl sub_8017A8C
	ldr r1, _0801A644 @ =gStaticData_087E48A4
	str r1, [r4, #0xc]
	ldr r0, [r6, #0x1c]
	str r0, [r4, #0x1c]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #5
	bl sub_803AD80
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	str r4, [r5, #0x44]
	ldr r1, [r4, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r4, r4, r0
	ldr r2, [r1, #0x1c]
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #0x10
	ldrb r3, [r5, #0xc]
	orrs r0, r3
	strb r0, [r5, #0xc]
	ldr r0, _0801A648 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801A63C: .4byte 0x0000FFFF
_0801A640: .4byte gUnknown_030012D0
_0801A644: .4byte gStaticData_087E48A4
_0801A648: .4byte gUnknown_030012F0

	thumb_func_start sub_801A64C
sub_801A64C: @ 0x0801A64C
	push {r4, r5, lr}
	adds r3, r1, #0
	ldr r4, [r0, #8]
	cmp r4, #0
	beq _0801A65C
	cmp r4, #1
	beq _0801A69C
	b _0801A712
_0801A65C:
	adds r0, r3, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _0801A684
	ldr r1, _0801A680 @ =gStaticData_0816C3B8
	ldr r0, [r1, #0x24]
	rsbs r0, r0, #0
	ldr r2, [r1, #0x2c]
	rsbs r2, r2, #0
	ldr r1, [r1, #0x28]
	str r0, [r3, #0x60]
	str r0, [r3, #0x48]
	str r1, [r3, #0x4c]
	str r2, [r3, #0x50]
	b _0801A712
	.align 2, 0
_0801A680: .4byte gStaticData_0816C3B8
_0801A684:
	ldr r0, _0801A698 @ =gStaticData_0816C3B8
	ldr r1, [r0, #0x24]
	ldr r2, [r0, #0x28]
	ldr r0, [r0, #0x2c]
	str r1, [r3, #0x60]
	str r1, [r3, #0x48]
	str r2, [r3, #0x4c]
	str r0, [r3, #0x50]
	b _0801A712
	.align 2, 0
_0801A698: .4byte gStaticData_0816C3B8
_0801A69C:
	adds r0, r3, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _0801A6CC
	ldr r0, [r3]
	movs r1, #0xa0
	lsls r1, r1, #6
	adds r0, r0, r1
	cmp r0, #0
	bgt _0801A712
	movs r0, #1
	ldrb r2, [r3, #0xc]
	orrs r0, r2
	strb r0, [r3, #0xc]
	ldr r0, _0801A6C8 @ =0x0000FFFF
	ldrh r5, [r3, #8]
	cmp r5, r0
	beq _0801A712
	b _0801A6F2
	.align 2, 0
_0801A6C8: .4byte 0x0000FFFF
_0801A6CC:
	ldr r1, [r3]
	ldr r0, _0801A718 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x10]
	lsls r0, r0, #8
	movs r2, #0xa0
	lsls r2, r2, #6
	adds r0, r0, r2
	cmp r1, r0
	blt _0801A712
	movs r0, #1
	ldrb r5, [r3, #0xc]
	orrs r0, r5
	strb r0, [r3, #0xc]
	ldr r0, _0801A71C @ =0x0000FFFF
	ldrh r1, [r3, #8]
	cmp r1, r0
	beq _0801A712
_0801A6F2:
	ldrh r3, [r3, #8]
	ldr r0, _0801A720 @ =gUnknown_030012B4
	ldr r1, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r2, r0, #2
	movs r5, #0x84
	lsls r5, r5, #1
	adds r1, r1, r5
	adds r1, r1, r2
	lsls r0, r0, #5
	subs r0, r3, r0
	lsls r4, r0
	ldr r0, [r1]
	orrs r0, r4
	str r0, [r1]
_0801A712:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0801A718: .4byte gUnknown_03001308
_0801A71C: .4byte 0x0000FFFF
_0801A720: .4byte gUnknown_030012B4

	thumb_func_start sub_801A724
sub_801A724: @ 0x0801A724
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800CA74
	ldr r0, _0801A738 @ =gStaticData_087E483C
	str r0, [r4, #0xc]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801A738: .4byte gStaticData_087E483C

	thumb_func_start sub_801A73C
sub_801A73C: @ 0x0801A73C
	push {lr}
	ldr r2, _0801A74C @ =gStaticData_087E483C
	str r2, [r0, #0xc]
	bl sub_800CA60
	pop {r0}
	bx r0
	.align 2, 0
_0801A74C: .4byte gStaticData_087E483C

	thumb_func_start sub_801A750
sub_801A750: @ 0x0801A750
	push {lr}
	ldr r2, _0801A764 @ =gStaticData_087E48A4
	str r2, [r0, #0xc]
	movs r2, #0
	str r2, [r0, #0x1c]
	bl sub_8017A78
	pop {r0}
	bx r0
	.align 2, 0
_0801A764: .4byte gStaticData_087E48A4

	thumb_func_start sub_801A768
sub_801A768: @ 0x0801A768
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8017A8C
	ldr r0, _0801A77C @ =gStaticData_087E48A4
	str r0, [r4, #0xc]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801A77C: .4byte gStaticData_087E48A4

	thumb_func_start sub_801A780
sub_801A780: @ 0x0801A780
	push {lr}
	ldr r2, _0801A790 @ =gStaticData_087E490C
	str r2, [r0, #0xc]
	bl sub_8017A78
	pop {r0}
	bx r0
	.align 2, 0
_0801A790: .4byte gStaticData_087E490C

	thumb_func_start sub_801A794
sub_801A794: @ 0x0801A794
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8017A8C
	ldr r0, _0801A7A8 @ =gStaticData_087E490C
	str r0, [r4, #0xc]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801A7A8: .4byte gStaticData_087E490C

	thumb_func_start sub_801A7AC
sub_801A7AC: @ 0x0801A7AC
	push {r4, r5, r6, r7, lr}
	adds r3, r1, #0
	adds r5, r2, #0
	ldr r2, _0801A7E8 @ =gStaticData_0816C418
	lsls r0, r5, #3
	adds r0, r0, r2
	ldr r1, [r0]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801A7EC @ =gStaticData_0816C3B8
	adds r4, r0, r1
	adds r0, r3, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	adds r6, r2, #0
	adds r7, r1, #0
	cmp r0, #0
	bge _0801A7F0
	ldr r0, [r4]
	rsbs r0, r0, #0
	ldr r1, [r4, #8]
	rsbs r1, r1, #0
	ldr r2, [r4, #4]
	str r0, [r3, #0x60]
	str r0, [r3, #0x48]
	str r2, [r3, #0x4c]
	str r1, [r3, #0x50]
	b _0801A7FE
	.align 2, 0
_0801A7E8: .4byte gStaticData_0816C418
_0801A7EC: .4byte gStaticData_0816C3B8
_0801A7F0:
	ldr r0, [r4]
	ldr r1, [r4, #4]
	ldr r2, [r4, #8]
	str r0, [r3, #0x60]
	str r0, [r3, #0x48]
	str r1, [r3, #0x4c]
	str r2, [r3, #0x50]
_0801A7FE:
	lsls r0, r5, #3
	adds r1, r6, #4
	adds r0, r0, r1
	ldr r1, [r0]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r7
	ldr r1, [r0]
	ldr r2, [r0, #4]
	ldr r0, [r0, #8]
	str r1, [r3, #0x64]
	str r1, [r3, #0x54]
	str r2, [r3, #0x58]
	str r0, [r3, #0x5c]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801A824
sub_801A824: @ 0x0801A824
	push {lr}
	ldr r2, _0801A834 @ =gStaticData_087E4974
	str r2, [r0, #0xc]
	bl sub_8017A78
	pop {r0}
	bx r0
	.align 2, 0
_0801A834: .4byte gStaticData_087E4974

	thumb_func_start sub_801A838
sub_801A838: @ 0x0801A838
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r6, r0, #0
	adds r4, r1, #0
	adds r5, r2, #0
	bl sub_8017A8C
	ldr r0, _0801A86C @ =gStaticData_087E4974
	str r0, [r6, #0xc]
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	movs r0, #0
	str r0, [sp]
	adds r0, r6, #0
	movs r1, #0
	adds r2, r4, #0
	adds r3, r5, #0
	bl sub_8019EBC
	adds r0, r6, #0
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0801A86C: .4byte gStaticData_087E4974

	thumb_func_start sub_801A870
sub_801A870: @ 0x0801A870
	str r1, [r0, #0x1c]
	bx lr

	thumb_func_start sub_801A874
sub_801A874: @ 0x0801A874
	str r1, [r0, #0x24]
	bx lr

	thumb_func_start sub_801A878
sub_801A878: @ 0x0801A878
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	mov sb, r0
	adds r5, r1, #0
	adds r6, r2, #0
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	mov sb, r0
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	lsls r6, r6, #0x10
	lsrs r6, r6, #0x10
	mov r1, r8
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	mov r8, r1
	movs r0, #0x80
	bl sub_8026EDC
	adds r4, r0, #0
	bl sub_8009F90
	ldr r0, _0801A8F4 @ =gStaticData_087E49DC
	str r0, [r4, #0x18]
	adds r0, r4, #0
	bl sub_801B2D8
	adds r7, r4, #0
	mov r2, sb
	strh r2, [r7, #8]
	lsls r5, r5, #8
	str r5, [r7]
	lsls r6, r6, #8
	str r6, [r7, #4]
	ldr r0, _0801A8F8 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r3, r8
	lsls r3, r3, #1
	mov r8, r3
	add r8, r0
	ldr r0, [r1, #0xc]
	mov r4, r8
	ldrh r5, [r4]
	adds r5, r5, r0
	mov r8, r5
	ldr r2, [r5, #4]
	ldr r0, [sp, #0x24]
	subs r0, #3
	cmp r0, #9
	bhi _0801A93A
	lsls r0, r0, #2
	ldr r1, _0801A8FC @ =_0801A900
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801A8F4: .4byte gStaticData_087E49DC
_0801A8F8: .4byte gUnknown_030012B4
_0801A8FC: .4byte _0801A900
_0801A900: @ jump table
	.4byte _0801A934 @ case 0
	.4byte _0801A928 @ case 1
	.4byte _0801A92C @ case 2
	.4byte _0801A938 @ case 3
	.4byte _0801A93A @ case 4
	.4byte _0801A930 @ case 5
	.4byte _0801A934 @ case 6
	.4byte _0801A934 @ case 7
	.4byte _0801A934 @ case 8
	.4byte _0801A934 @ case 9
_0801A928:
	movs r2, #2
	b _0801A93A
_0801A92C:
	movs r2, #3
	b _0801A93A
_0801A930:
	movs r2, #7
	b _0801A93A
_0801A934:
	movs r2, #4
	b _0801A93A
_0801A938:
	movs r2, #6
_0801A93A:
	cmp r2, #7
	bls _0801A940
	b _0801AA9C
_0801A940:
	lsls r0, r2, #2
	ldr r1, _0801A94C @ =_0801A950
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801A94C: .4byte _0801A950
_0801A950: @ jump table
	.4byte _0801A970 @ case 0
	.4byte _0801A976 @ case 1
	.4byte _0801A9D4 @ case 2
	.4byte _0801A9DA @ case 3
	.4byte _0801A9EE @ case 4
	.4byte _0801A9F4 @ case 5
	.4byte _0801AA1C @ case 6
	.4byte _0801AA6C @ case 7
_0801A970:
	movs r0, #0
	str r0, [r7, #0x78]
	b _0801AA9C
_0801A976:
	movs r6, #1
	str r6, [r7, #0x78]
	movs r0, #0x38
	bl sub_8026EDC
	mov r2, r8
	ldr r1, [r2, #8]
	ldr r2, [r2, #0xc]
	mov r3, r8
	movs r5, #0x10
	ldrsh r4, [r3, r5]
	rsbs r3, r4, #0
	orrs r3, r4
	lsrs r3, r3, #0x1f
	mov sb, r3
	mov r3, r8
	movs r4, #0x12
	ldrsh r5, [r3, r4]
	rsbs r4, r5, #0
	orrs r4, r5
	lsrs r4, r4, #0x1f
	mov r5, sp
	strb r4, [r5]
	str r6, [sp, #4]
	mov r3, sb
	bl sub_801B7D8
	adds r2, r0, #0
	str r2, [r7, #0x44]
	ldr r1, [r2, #0xc]
	movs r5, #0x18
	ldrsh r0, [r1, r5]
	adds r0, r2, r0
	ldr r2, [r1, #0x1c]
	adds r1, r7, #0
	bl sub_803AD80
	mov r1, r8
	movs r2, #0x14
	ldrsh r0, [r1, r2]
	cmp r0, #0
	beq _0801AA9C
	movs r0, #0x10
	ldrb r3, [r7, #0xc]
	orrs r0, r3
	strb r0, [r7, #0xc]
	b _0801AA9C
_0801A9D4:
	movs r0, #2
	str r0, [r7, #0x78]
	b _0801AA9C
_0801A9DA:
	movs r0, #3
	str r0, [r7, #0x78]
	movs r1, #1
	mov r4, r8
	ldrb r4, [r4]
	ands r1, r4
	adds r0, r7, #0
	bl sub_801B2A8
	b _0801AA9C
_0801A9EE:
	movs r0, #4
	str r0, [r7, #0x78]
	b _0801AA9C
_0801A9F4:
	movs r4, #5
	str r4, [r7, #0x78]
	movs r0, #0x38
	bl sub_8026EDC
	mov r2, sp
	movs r1, #0
	strb r1, [r2]
	str r4, [sp, #4]
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_801B7D8
	adds r2, r0, #0
	str r2, [r7, #0x44]
	ldr r1, [r2, #0xc]
	movs r5, #0x18
	ldrsh r0, [r1, r5]
	b _0801AA60
_0801AA1C:
	movs r4, #6
	str r4, [r7, #0x78]
	ldr r0, _0801AA48 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80233B4
	cmp r0, #1
	beq _0801AA4C
	movs r0, #0x38
	bl sub_8026EDC
	mov r2, sp
	movs r1, #0
	strb r1, [r2]
	str r4, [sp, #4]
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_801B7D8
	b _0801AA56
	.align 2, 0
_0801AA48: .4byte gUnknown_030012C0
_0801AA4C:
	movs r0, #0x38
	bl sub_8026EDC
	bl sub_801961C
_0801AA56:
	adds r2, r0, #0
	str r2, [r7, #0x44]
	ldr r1, [r2, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
_0801AA60:
	adds r0, r2, r0
	ldr r2, [r1, #0x1c]
	adds r1, r7, #0
	bl sub_803AD80
	b _0801AA9C
_0801AA6C:
	movs r4, #7
	str r4, [r7, #0x78]
	movs r0, #0x38
	bl sub_8026EDC
	mov r2, sp
	movs r1, #0
	strb r1, [r2]
	str r4, [sp, #4]
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_801B7D8
	adds r2, r0, #0
	str r2, [r7, #0x44]
	ldr r1, [r2, #0xc]
	movs r4, #0x18
	ldrsh r0, [r1, r4]
	adds r0, r2, r0
	ldr r2, [r1, #0x1c]
	adds r1, r7, #0
	bl sub_803AD80
_0801AA9C:
	ldr r0, _0801AB28 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r7, #0
	bl sub_8008E94
	ldr r0, _0801AB2C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r5, #0xea
	lsls r5, r5, #1
	adds r0, r0, r5
	str r0, [r7, #0x20]
	adds r4, r7, #0
	adds r4, #0x2d
	add r0, sp, #0x24
	ldrb r0, [r0]
	strb r0, [r4]
	adds r0, r7, #0
	bl sub_80087C0
	adds r0, r7, #0
	bl sub_80087B4
	adds r0, r7, #0
	movs r1, #0
	bl sub_800872C
	adds r2, r7, #0
	adds r2, #0x28
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r1, [r2]
	ands r0, r1
	movs r1, #0x21
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r2]
	ldr r0, [r7, #0x20]
	ldr r1, [r0]
	ldrb r2, [r4]
	lsls r0, r2, #3
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r1, r1, r0
	ldr r0, _0801AB30 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldrb r1, [r1, #0x14]
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	adds r2, r7, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	adds r0, r7, #0
	add sp, #8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0801AB28: .4byte gUnknown_030012EC
_0801AB2C: .4byte gUnknown_030012D0
_0801AB30: .4byte gUnknown_030012B8

	thumb_func_start sub_801AB34
sub_801AB34: @ 0x0801AB34
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x78]
	cmp r0, #6
	bne _0801AB44
	ldr r0, [r4, #0x30]
	cmp r0, #0x12
	bgt _0801AB88
_0801AB44:
	ldr r0, _0801AB90 @ =gUnknown_030012D8
	ldr r3, [r0]
	ldr r0, [r3, #0x44]
	ldr r5, [r0, #8]
	ldrb r1, [r3, #0xc]
	lsrs r0, r1, #7
	cmp r0, #0
	beq _0801AB7E
	ldr r2, [r4]
	ldr r0, [r3]
	subs r2, r2, r0
	cmp r2, #0
	bge _0801AB60
	rsbs r2, r2, #0
_0801AB60:
	ldr r1, _0801AB94 @ =0x00007FFF
	cmp r2, r1
	bgt _0801AB7E
	ldr r2, [r4, #4]
	ldr r0, [r3, #4]
	subs r2, r2, r0
	cmp r2, #0
	bge _0801AB72
	rsbs r2, r2, #0
_0801AB72:
	cmp r2, r1
	bgt _0801AB7E
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_801AB98
_0801AB7E:
	movs r0, #9
	rsbs r0, r0, #0
	ldrb r1, [r4, #0xc]
	ands r0, r1
	strb r0, [r4, #0xc]
_0801AB88:
	movs r0, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0801AB90: .4byte gUnknown_030012D8
_0801AB94: .4byte 0x00007FFF

	thumb_func_start sub_801AB98
sub_801AB98: @ 0x0801AB98
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x44
	mov sb, r0
	add r0, sp, #4
	mov r1, sb
	bl sub_8007B98
	ldr r0, _0801AC3C @ =gUnknown_030012D8
	ldr r1, [r0]
	ldr r0, [r1]
	asrs r5, r0, #8
	ldr r0, [r1, #4]
	asrs r0, r0, #8
	mov sl, r0
	add r4, sp, #0x14
	adds r0, r4, #0
	bl sub_8007B98
	ldr r1, _0801AC3C @ =gUnknown_030012D8
	ldr r0, [r1]
	ldr r2, [r0, #0x20]
	adds r0, #0x2d
	ldrb r3, [r0]
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	ldr r0, [r2]
	adds r0, r0, r1
	adds r7, r0, #4
	add r0, sp, #4
	adds r1, r4, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801ABEA
	b _0801B078
_0801ABEA:
	movs r0, #0
	mov r8, r0
	movs r1, #0
	str r1, [sp, #0x40]
	ldr r1, [sp, #0x18]
	ldr r0, [sp, #8]
	cmp r1, r0
	bge _0801ABFE
	movs r2, #1
	str r2, [sp, #0x40]
_0801ABFE:
	ldr r3, _0801AC3C @ =gUnknown_030012D8
	ldr r0, [r3]
	bl sub_8009EC4
	adds r4, r0, #0
	ldr r1, _0801AC3C @ =gUnknown_030012D8
	ldr r0, [r1]
	bl sub_8009EBC
	adds r6, r0, #0
	movs r2, #2
	str r2, [sp, #0x3c]
	cmp r5, r4
	ble _0801AC1E
	movs r3, #1
	str r3, [sp, #0x3c]
_0801AC1E:
	ldr r1, _0801AC3C @ =gUnknown_030012D8
	ldr r0, [r1]
	ldr r1, [r0]
	asrs r1, r1, #8
	mov r2, sb
	ldr r0, [r2]
	asrs r0, r0, #8
	cmp r1, r0
	bge _0801AC40
	movs r3, #1
	str r3, [sp, #0x34]
	ldr r0, [sp, #0x14]
	ldr r1, [sp, #0x1c]
	ldr r2, [sp, #4]
	b _0801AC4A
	.align 2, 0
_0801AC3C: .4byte gUnknown_030012D8
_0801AC40:
	movs r0, #2
	str r0, [sp, #0x34]
	ldr r0, [sp, #4]
	ldr r1, [sp, #0xc]
	ldr r2, [sp, #0x14]
_0801AC4A:
	adds r0, r0, r1
	subs r0, r0, r2
	adds r0, #1
	str r0, [sp, #0x2c]
	ldr r0, _0801AC70 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r1, [r0, #4]
	asrs r1, r1, #8
	mov r2, sb
	ldr r0, [r2, #4]
	asrs r0, r0, #8
	cmp r1, r0
	ble _0801AC74
	movs r3, #4
	str r3, [sp, #0x38]
	ldr r0, [sp, #8]
	ldr r1, [sp, #0x10]
	ldr r2, [sp, #0x18]
	b _0801AC7E
	.align 2, 0
_0801AC70: .4byte gUnknown_030012D8
_0801AC74:
	movs r0, #8
	str r0, [sp, #0x38]
	ldr r0, [sp, #0x18]
	ldr r1, [sp, #0x20]
	ldr r2, [sp, #8]
_0801AC7E:
	adds r0, r0, r1
	subs r0, r0, r2
	str r0, [sp, #0x30]
	mov r1, sb
	ldr r0, [r1, #0x78]
	cmp r0, #1
	beq _0801ACE0
	cmp r0, #5
	beq _0801ACE0
	cmp r0, #6
	beq _0801ACE0
	cmp r6, sl
	bne _0801ACC8
	cmp r4, r5
	bne _0801ACAC
	ldr r2, [sp, #0x34]
	mov r8, r2
	ldr r3, [sp, #0x30]
	cmp r3, #2
	bgt _0801ACE0
	movs r5, #8
	mov r8, r5
	b _0801ACE0
_0801ACAC:
	mov r0, sb
	bl sub_8009EBC
	mov r2, sb
	ldr r1, [r2, #4]
	asrs r1, r1, #8
	cmp r0, r1
	bne _0801ACC8
	ldr r3, [sp, #0x30]
	cmp r3, #2
	ble _0801ACC8
	ldr r5, [sp, #0x34]
	mov r8, r5
	b _0801ACE0
_0801ACC8:
	cmp r4, r5
	bne _0801ACE0
	mov r0, sb
	bl sub_8009EC4
	mov r2, sb
	ldr r1, [r2]
	asrs r1, r1, #8
	cmp r0, r1
	bne _0801ACE0
	ldr r3, [sp, #0x38]
	mov r8, r3
_0801ACE0:
	cmp r6, sl
	bgt _0801ADA8
	ldr r5, [sp, #0x40]
	cmp r5, #0
	beq _0801ADA8
	mov r0, r8
	cmp r0, #0
	beq _0801ACF2
	b _0801AE60
_0801ACF2:
	movs r1, #2
	ldrsh r0, [r7, r1]
	ldrb r2, [r7, #5]
	adds r0, r0, r2
	adds r6, r6, r0
	ldr r0, [sp, #8]
	ldr r1, [sp, #0x10]
	adds r0, r0, r1
	adds r3, r2, #0
	cmp r6, r0
	ble _0801AD0A
	b _0801AE5C
_0801AD0A:
	ldr r2, [sp, #0x3c]
	cmp r2, #1
	bne _0801AD2A
	ldr r1, [sp, #0x14]
	ldr r0, [sp, #0x1c]
	adds r0, r1, r0
	adds r2, r1, #0
	ldr r1, [sp, #4]
	cmp r0, r1
	blt _0801AD40
	ldr r5, [sp, #0x2c]
	cmp r5, #2
	ble _0801AD40
	movs r0, #8
	mov r8, r0
	b _0801AE60
_0801AD2A:
	ldr r1, [sp, #4]
	ldr r0, [sp, #0xc]
	adds r0, r1, r0
	ldr r2, [sp, #0x14]
	cmp r2, r0
	bgt _0801AD40
	ldr r5, [sp, #0x2c]
	cmp r5, #2
	ble _0801AD40
	movs r0, #8
	mov r8, r0
_0801AD40:
	mov r5, r8
	cmp r5, #0
	beq _0801AD48
	b _0801AE60
_0801AD48:
	movs r5, #2
	ldrsh r0, [r7, r5]
	adds r0, r0, r3
	add sl, r0
	ldr r0, [sp, #0x34]
	cmp r0, #1
	bne _0801AD6E
	movs r3, #0
	ldrsh r0, [r7, r3]
	ldrb r7, [r7, #4]
	adds r0, r7, r0
	adds r4, r4, r0
	ldr r0, [sp, #0x1c]
	adds r5, r2, r0
	str r1, [sp]
	adds r0, r4, #0
	adds r1, r6, #0
	adds r2, r5, #0
	b _0801AD80
_0801AD6E:
	movs r5, #0
	ldrsh r0, [r7, r5]
	adds r4, r4, r0
	adds r5, r2, #0
	ldr r0, [sp, #0xc]
	adds r0, r1, r0
	str r0, [sp]
	adds r0, r4, #0
	adds r1, r6, #0
_0801AD80:
	mov r3, sl
	bl sub_800FDC8
	adds r2, r0, #0
	cmp r2, #0
	bge _0801AD98
	ldr r0, [sp, #0x40]
	cmp r0, #0
	beq _0801AD98
	ldr r1, [sp, #0x30]
	cmp r1, #1
	ble _0801ADA2
_0801AD98:
	cmp r2, #0
	ble _0801AE56
	ldr r0, [sp, #8]
	cmp r2, r0
	bgt _0801AE56
_0801ADA2:
	movs r2, #8
	mov r8, r2
	b _0801AE60
_0801ADA8:
	mov r0, r8
	cmp r0, #0
	bne _0801AE60
	movs r1, #2
	ldrsh r0, [r7, r1]
	adds r6, r6, r0
	ldr r0, [sp, #8]
	cmp r6, r0
	blt _0801AE5C
	ldr r2, [sp, #0x3c]
	cmp r2, #1
	bne _0801ADD8
	ldr r1, [sp, #0x14]
	ldr r0, [sp, #0x1c]
	adds r0, r1, r0
	adds r2, r1, #0
	ldr r1, [sp, #4]
	cmp r0, r1
	blt _0801ADEE
	ldr r3, [sp, #0x2c]
	cmp r3, #3
	ble _0801ADEE
	movs r5, #4
	b _0801AE5E
_0801ADD8:
	ldr r1, [sp, #4]
	ldr r0, [sp, #0xc]
	adds r0, r1, r0
	ldr r2, [sp, #0x14]
	cmp r2, r0
	bgt _0801ADEE
	ldr r0, [sp, #0x2c]
	cmp r0, #3
	ble _0801ADEE
	movs r3, #4
	mov r8, r3
_0801ADEE:
	mov r5, r8
	cmp r5, #0
	bne _0801AE60
	movs r3, #2
	ldrsh r0, [r7, r3]
	add sl, r0
	ldr r5, [sp, #0x34]
	cmp r5, #1
	bne _0801AE18
	movs r3, #0
	ldrsh r0, [r7, r3]
	ldrb r7, [r7, #4]
	adds r0, r7, r0
	adds r4, r4, r0
	ldr r0, [sp, #0x1c]
	adds r5, r2, r0
	str r1, [sp]
	adds r0, r4, #0
	adds r1, r6, #0
	adds r2, r5, #0
	b _0801AE2A
_0801AE18:
	movs r5, #0
	ldrsh r0, [r7, r5]
	adds r4, r4, r0
	adds r5, r2, #0
	ldr r0, [sp, #0xc]
	adds r0, r1, r0
	str r0, [sp]
	adds r0, r4, #0
	adds r1, r6, #0
_0801AE2A:
	mov r3, sl
	bl sub_800FDC8
	adds r2, r0, #0
	cmp r2, #0
	bge _0801AE42
	ldr r0, [sp, #0x40]
	cmp r0, #0
	bne _0801AE42
	ldr r1, [sp, #0x2c]
	cmp r1, #3
	bgt _0801AE50
_0801AE42:
	cmp r2, #0
	ble _0801AE56
	ldr r0, [sp, #8]
	ldr r1, [sp, #0x10]
	adds r0, r0, r1
	cmp r2, r0
	blt _0801AE56
_0801AE50:
	movs r2, #4
	mov r8, r2
	b _0801AE60
_0801AE56:
	ldr r3, [sp, #0x34]
	mov r8, r3
	b _0801AE60
_0801AE5C:
	ldr r5, [sp, #0x34]
_0801AE5E:
	mov r8, r5
_0801AE60:
	ldr r2, _0801AE98 @ =gUnknown_030012D8
	ldr r1, [r2]
	ldr r0, [r1]
	str r0, [sp, #0x24]
	ldr r0, [r1, #4]
	add r1, sp, #0x24
	str r0, [r1, #4]
	mov sl, r2
	adds r7, r1, #0
	ldr r0, [sp, #0x2c]
	cmp r0, #0
	bge _0801AE7C
	movs r1, #0
	str r1, [sp, #0x2c]
_0801AE7C:
	ldr r2, [sp, #0x30]
	cmp r2, #0
	bge _0801AE86
	movs r3, #0
	str r3, [sp, #0x30]
_0801AE86:
	movs r5, #0
	mov r0, r8
	cmp r0, #8
	bhi _0801AF3C
	lsls r0, r0, #2
	ldr r1, _0801AE9C @ =_0801AEA0
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801AE98: .4byte gUnknown_030012D8
_0801AE9C: .4byte _0801AEA0
_0801AEA0: @ jump table
	.4byte _0801AF32 @ case 0
	.4byte _0801AF14 @ case 1
	.4byte _0801AF14 @ case 2
	.4byte _0801AF32 @ case 3
	.4byte _0801AEC4 @ case 4
	.4byte _0801AF32 @ case 5
	.4byte _0801AF32 @ case 6
	.4byte _0801AF32 @ case 7
	.4byte _0801AEFC @ case 8
_0801AEC4:
	ldr r0, _0801AEF8 @ =gUnknown_030012D8
	ldr r2, [r0]
	adds r1, r2, #0
	adds r1, #0x68
	movs r0, #8
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0801AF32
	ldr r1, [r2, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xc
	movs r3, #4
	bl sub_803AD88
	ldr r1, [sp, #0x30]
	lsls r0, r1, #8
	ldr r1, [r7, #4]
	adds r0, r0, r1
	str r0, [r7, #4]
	b _0801AF32
	.align 2, 0
_0801AEF8: .4byte gUnknown_030012D8
_0801AEFC:
	ldr r0, [sp, #0x30]
	subs r0, #1
	lsls r0, r0, #8
	ldr r1, [r7, #4]
	subs r1, r1, r0
	ldr r0, _0801AF10 @ =0xFFFFFF00
	ands r1, r0
	str r1, [r7, #4]
	b _0801AF32
	.align 2, 0
_0801AF10: .4byte 0xFFFFFF00
_0801AF14:
	ldr r5, [sp, #0x34]
	cmp r5, #2
	bne _0801AF24
	ldr r2, [sp, #0x2c]
	lsls r0, r2, #8
	ldr r1, [sp, #0x24]
	adds r0, r0, r1
	b _0801AF30
_0801AF24:
	cmp r5, #1
	bne _0801AF32
	ldr r3, [sp, #0x2c]
	lsls r1, r3, #8
	ldr r0, [sp, #0x24]
	subs r0, r0, r1
_0801AF30:
	str r0, [sp, #0x24]
_0801AF32:
	ldr r0, _0801AFD0 @ =gUnknown_030012D8
	mov sl, r0
	mov r1, r8
	cmp r1, #8
	beq _0801AF42
_0801AF3C:
	ldr r2, [sp, #0x30]
	cmp r2, #1
	bgt _0801AF7C
_0801AF42:
	mov r3, sl
	ldr r2, [r3]
	adds r1, r2, #0
	adds r1, #0x24
	movs r0, #4
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0801AF7C
	ldr r0, [sp, #0x40]
	cmp r0, #0
	beq _0801AF7C
	adds r0, r2, #0
	adds r0, #0xac
	mov r1, sb
	str r1, [r0]
	movs r1, #8
	subs r0, #0x44
	strb r1, [r0]
	ldr r2, [r3]
	ldr r1, [r2, #4]
	ldr r0, [sp, #0x30]
	subs r0, #1
	lsls r0, r0, #8
	subs r1, r1, r0
	str r1, [r7, #4]
	movs r5, #0
	ldr r0, [r2]
	str r0, [sp, #0x24]
_0801AF7C:
	mov r6, sl
	ldr r0, [r6]
	ldr r1, [sp, #0x24]
	ldr r2, [r7, #4]
	bl sub_8007398
	cmp r5, #0
	beq _0801AFAC
	ldr r0, [r6]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xc
	adds r3, r5, #0
	bl sub_803AD88
	ldr r1, [r6]
	ldr r0, [r1, #0x74]
	orrs r0, r5
	str r0, [r1, #0x74]
_0801AFAC:
	mov r5, r8
	cmp r5, #8
	beq _0801AFB4
	b _0801B1F8
_0801AFB4:
	mov r0, sb
	ldr r2, [r0, #0x78]
	cmp r2, #1
	beq _0801AFC4
	cmp r2, #5
	beq _0801AFC4
	cmp r2, #6
	bne _0801AFD4
_0801AFC4:
	mov r1, sb
	ldr r0, [r1, #0x44]
	adds r0, #0x32
	movs r1, #1
	b _0801B1F6
	.align 2, 0
_0801AFD0: .4byte gUnknown_030012D8
_0801AFD4:
	mov r3, sb
	ldr r0, [r3]
	asrs r0, r0, #8
	ldr r3, [r6]
	ldr r1, [r3]
	asrs r1, r1, #8
	subs r0, r0, r1
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	cmp r0, #7
	ble _0801AFEE
	b _0801B1F8
_0801AFEE:
	cmp r2, #3
	beq _0801B014
	cmp r2, #3
	bgt _0801AFFC
	cmp r2, #2
	beq _0801B002
	b _0801B1F8
_0801AFFC:
	cmp r2, #4
	beq _0801B048
	b _0801B1F8
_0801B002:
	ldr r1, [r3, #0x18]
	adds r1, #0x68
	movs r5, #0
	ldrsh r0, [r1, r5]
	adds r0, r3, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0x11
	b _0801B1D0
_0801B014:
	ldr r4, _0801B044 @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_80232A0
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801B024
	b _0801B1F8
_0801B024:
	ldr r0, [r4]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801B030
	b _0801B1F8
_0801B030:
	ldr r0, [r6]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xf
	b _0801B1D0
	.align 2, 0
_0801B044: .4byte gUnknown_030012C0
_0801B048:
	ldr r4, _0801B074 @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_8023278
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801B058
	b _0801B1F8
_0801B058:
	ldr r0, [r4]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801B064
	b _0801B1F8
_0801B064:
	mov r5, sl
	ldr r0, [r5]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	b _0801B1CA
	.align 2, 0
_0801B074: .4byte gUnknown_030012C0
_0801B078:
	ldr r0, [sp, #8]
	subs r0, #4
	str r0, [sp, #8]
	ldr r0, [sp, #0x10]
	adds r0, #4
	str r0, [sp, #0x10]
	mov r5, sb
	ldr r0, [r5, #0x78]
	cmp r0, #7
	bls _0801B08E
	b _0801B1F8
_0801B08E:
	lsls r0, r0, #2
	ldr r1, _0801B098 @ =_0801B09C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801B098: .4byte _0801B09C
_0801B09C: @ jump table
	.4byte _0801B0BC @ case 0
	.4byte _0801B1E0 @ case 1
	.4byte _0801B0E4 @ case 2
	.4byte _0801B124 @ case 3
	.4byte _0801B180 @ case 4
	.4byte _0801B1E0 @ case 5
	.4byte _0801B1E0 @ case 6
	.4byte _0801B0BC @ case 7
_0801B0BC:
	add r0, sp, #4
	adds r1, r4, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801B0CC
	b _0801B1F8
_0801B0CC:
	ldr r0, _0801B0E0 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r1, r0, #0
	adds r1, #0xac
	mov r2, sb
	str r2, [r1]
	movs r1, #8
	adds r0, #0x68
	b _0801B1F6
	.align 2, 0
_0801B0E0: .4byte gUnknown_030012D8
_0801B0E4:
	add r0, sp, #4
	adds r1, r4, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801B0F4
	b _0801B1F8
_0801B0F4:
	mov r3, sb
	ldr r1, [r3]
	asrs r1, r1, #8
	ldr r0, _0801B120 @ =gUnknown_030012D8
	ldr r2, [r0]
	ldr r0, [r2]
	asrs r0, r0, #8
	subs r1, r1, r0
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	cmp r1, #7
	bgt _0801B1F8
	ldr r1, [r2, #0x18]
	adds r1, #0x68
	movs r5, #0
	ldrsh r0, [r1, r5]
	adds r0, r2, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0x11
	b _0801B1D0
	.align 2, 0
_0801B120: .4byte gUnknown_030012D8
_0801B124:
	ldr r5, _0801B178 @ =gUnknown_030012C0
	ldr r0, [r5]
	bl sub_80232A0
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801B1F8
	ldr r0, [r5]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0801B1F8
	add r0, sp, #4
	adds r1, r4, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801B1F8
	mov r0, sb
	ldr r1, [r0]
	asrs r1, r1, #8
	ldr r0, _0801B17C @ =gUnknown_030012D8
	ldr r2, [r0]
	ldr r0, [r2]
	asrs r0, r0, #8
	subs r1, r1, r0
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	cmp r1, #7
	bgt _0801B1F8
	ldr r1, [r2, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xf
	b _0801B1D0
	.align 2, 0
_0801B178: .4byte gUnknown_030012C0
_0801B17C: .4byte gUnknown_030012D8
_0801B180:
	ldr r5, _0801B1D8 @ =gUnknown_030012C0
	ldr r0, [r5]
	bl sub_8023278
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801B1F8
	ldr r0, [r5]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0801B1F8
	add r0, sp, #4
	adds r1, r4, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801B1F8
	mov r5, sb
	ldr r1, [r5]
	asrs r1, r1, #8
	ldr r0, _0801B1DC @ =gUnknown_030012D8
	ldr r2, [r0]
	ldr r0, [r2]
	asrs r0, r0, #8
	subs r1, r1, r0
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	cmp r1, #7
	bgt _0801B1F8
	ldr r1, [r2, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
_0801B1CA:
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0x10
_0801B1D0:
	movs r3, #0
	bl sub_803AD88
	b _0801B1F8
	.align 2, 0
_0801B1D8: .4byte gUnknown_030012C0
_0801B1DC: .4byte gUnknown_030012D8
_0801B1E0:
	add r0, sp, #4
	adds r1, r4, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	lsrs r1, r0, #0x18
	cmp r1, #0
	bne _0801B1F8
	mov r5, sb
	ldr r0, [r5, #0x44]
	adds r0, #0x32
_0801B1F6:
	strb r1, [r0]
_0801B1F8:
	add sp, #0x44
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_801B208
sub_801B208: @ 0x0801B208
	push {r4, lr}
	adds r4, r0, #0
	ldr r1, [r4, #0x18]
	movs r2, #0x38
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #0x3c]
	bl sub_803AD7C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801B270
	adds r0, r4, #0
	bl sub_8008044
	ldr r1, [r4, #0x18]
	adds r1, #0x60
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	ldr r0, [r4, #0x78]
	cmp r0, #6
	bne _0801B254
	ldr r0, [r4, #0x30]
	cmp r0, #0x12
	ble _0801B254
	ldr r0, _0801B26C @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r1, r0, #0
	adds r1, #0xac
	ldr r0, [r1]
	cmp r0, r4
	bne _0801B254
	movs r0, #0
	str r0, [r1]
_0801B254:
	ldr r2, [r4, #0x44]
	cmp r2, #0
	beq _0801B296
	ldr r1, [r2, #0xc]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	adds r1, r4, #0
	bl sub_803AD80
	b _0801B296
	.align 2, 0
_0801B26C: .4byte gUnknown_030012D8
_0801B270:
	ldr r1, [r4, #0x18]
	adds r1, #0x60
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	ldr r2, [r4, #0x44]
	cmp r2, #0
	beq _0801B296
	ldr r1, [r2, #0xc]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	adds r1, r4, #0
	bl sub_803AD80
_0801B296:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_801B29C
sub_801B29C: @ 0x0801B29C
	ldrb r0, [r0, #0xd]
	lsrs r0, r0, #4
	movs r1, #1
	ands r0, r1
	bx lr
	.align 2, 0

	thumb_func_start sub_801B2A8
sub_801B2A8: @ 0x0801B2A8
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	movs r2, #1
	ands r1, r2
	lsls r1, r1, #4
	movs r2, #0x11
	rsbs r2, r2, #0
	ldrb r3, [r0, #0xd]
	ands r2, r3
	orrs r2, r1
	strb r2, [r0, #0xd]
	bx lr

	thumb_func_start sub_801B2C0
sub_801B2C0: @ 0x0801B2C0
	movs r0, #4
	bx lr

	thumb_func_start sub_801B2C4
sub_801B2C4: @ 0x0801B2C4
	push {lr}
	ldr r2, _0801B2D4 @ =gStaticData_087E49DC
	str r2, [r0, #0x18]
	bl sub_8009F1C
	pop {r0}
	bx r0
	.align 2, 0
_0801B2D4: .4byte gStaticData_087E49DC

	thumb_func_start sub_801B2D8
sub_801B2D8: @ 0x0801B2D8
	movs r1, #0x41
	rsbs r1, r1, #0
	ldrb r2, [r0, #0xc]
	ands r1, r2
	strb r1, [r0, #0xc]
	bx lr

	thumb_func_start sub_801B2E4
sub_801B2E4: @ 0x0801B2E4
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8009F90
	ldr r0, _0801B300 @ =gStaticData_087E49DC
	str r0, [r4, #0x18]
	adds r0, r4, #0
	bl sub_801B2D8
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801B300: .4byte gStaticData_087E49DC

	thumb_func_start sub_801B304
sub_801B304: @ 0x0801B304
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	adds r4, r1, #0
	ldr r0, [r6, #0x20]
	cmp r0, #0
	bne _0801B35E
	ldr r0, [r6, #0x28]
	cmp r0, #0
	ble _0801B35E
	ldr r0, [r4]
	asrs r0, r0, #8
	str r0, [r6, #0x20]
	ldr r0, [r6, #4]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801B348 @ =gStaticData_0816C460
	adds r3, r0, r1
	adds r0, r6, #0
	adds r0, #0x30
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801B34C
	ldr r0, [r3]
	ldr r1, [r3, #4]
	ldr r2, [r3, #8]
	str r0, [r4, #0x60]
	str r0, [r4, #0x48]
	str r1, [r4, #0x4c]
	str r2, [r4, #0x50]
	b _0801B35E
	.align 2, 0
_0801B348: .4byte gStaticData_0816C460
_0801B34C:
	ldr r0, [r3]
	rsbs r0, r0, #0
	ldr r1, [r3, #8]
	rsbs r1, r1, #0
	ldr r2, [r3, #4]
	str r0, [r4, #0x60]
	str r0, [r4, #0x48]
	str r2, [r4, #0x4c]
	str r1, [r4, #0x50]
_0801B35E:
	ldr r0, [r6, #0x24]
	cmp r0, #0
	bne _0801B3B2
	ldr r0, [r6, #0x2c]
	cmp r0, #0
	ble _0801B3B2
	ldr r0, [r4, #4]
	asrs r0, r0, #8
	str r0, [r6, #0x24]
	ldr r0, [r6, #4]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801B39C @ =gStaticData_0816C460
	adds r3, r0, r1
	adds r0, r6, #0
	adds r0, #0x31
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801B3A0
	ldr r0, [r3]
	ldr r1, [r3, #4]
	ldr r2, [r3, #8]
	str r0, [r4, #0x64]
	str r0, [r4, #0x54]
	str r1, [r4, #0x58]
	str r2, [r4, #0x5c]
	b _0801B3B2
	.align 2, 0
_0801B39C: .4byte gStaticData_0816C460
_0801B3A0:
	ldr r0, [r3]
	rsbs r0, r0, #0
	ldr r1, [r3, #8]
	rsbs r1, r1, #0
	ldr r2, [r3, #4]
	str r0, [r4, #0x64]
	str r0, [r4, #0x54]
	str r2, [r4, #0x58]
	str r1, [r4, #0x5c]
_0801B3B2:
	ldr r2, [r6, #0x18]
	movs r0, #1
	rsbs r0, r0, #0
	cmp r2, r0
	beq _0801B3CE
	ldr r0, [r4]
	asrs r0, r0, #8
	ldr r1, [r6, #0x20]
	subs r0, r0, r1
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	adds r0, r2, r0
	b _0801B3F0
_0801B3CE:
	ldr r2, [r4, #0x60]
	asrs r0, r2, #0x1f
	eors r2, r0
	subs r2, r2, r0
	ldr r3, _0801B410 @ =gStaticData_0816C460
	ldr r0, [r6, #4]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r3, #8
	adds r0, r0, r3
	ldr r0, [r0]
	cmp r2, r0
	blt _0801B3F2
	movs r0, #0
_0801B3F0:
	str r0, [r6, #0x18]
_0801B3F2:
	ldr r2, [r6, #0x1c]
	movs r0, #1
	rsbs r0, r0, #0
	cmp r2, r0
	beq _0801B414
	ldr r0, [r4, #4]
	asrs r0, r0, #8
	ldr r1, [r6, #0x24]
	subs r0, r0, r1
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	adds r0, r2, r0
	b _0801B436
	.align 2, 0
_0801B410: .4byte gStaticData_0816C460
_0801B414:
	ldr r2, [r4, #0x64]
	asrs r0, r2, #0x1f
	eors r2, r0
	subs r2, r2, r0
	ldr r3, _0801B474 @ =gStaticData_0816C460
	ldr r0, [r6, #4]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r3, #8
	adds r0, r0, r3
	ldr r0, [r0]
	cmp r2, r0
	blt _0801B438
	movs r0, #0
_0801B436:
	str r0, [r6, #0x1c]
_0801B438:
	ldr r0, [r6, #0x18]
	ldr r1, [r6, #0x28]
	cmp r0, r1
	ble _0801B48E
	cmp r1, #0
	beq _0801B48E
	adds r0, r6, #0
	adds r0, #0x30
	movs r2, #1
	ldrb r1, [r0]
	eors r2, r1
	strb r2, [r0]
	ldr r0, [r6, #4]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801B474 @ =gStaticData_0816C460
	adds r3, r0, r1
	cmp r2, #0
	beq _0801B478
	ldr r0, [r3]
	ldr r1, [r3, #4]
	ldr r2, [r3, #8]
	str r0, [r4, #0x48]
	str r1, [r4, #0x4c]
	str r2, [r4, #0x50]
	b _0801B488
	.align 2, 0
_0801B474: .4byte gStaticData_0816C460
_0801B478:
	ldr r0, [r3]
	rsbs r0, r0, #0
	ldr r1, [r3, #8]
	rsbs r1, r1, #0
	ldr r2, [r3, #4]
	str r0, [r4, #0x48]
	str r2, [r4, #0x4c]
	str r1, [r4, #0x50]
_0801B488:
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r6, #0x18]
_0801B48E:
	ldr r0, [r6, #0x1c]
	ldr r1, [r6, #0x2c]
	cmp r0, r1
	ble _0801B4E2
	cmp r1, #0
	beq _0801B4E2
	adds r0, r6, #0
	adds r0, #0x31
	movs r2, #1
	ldrb r5, [r0]
	eors r2, r5
	strb r2, [r0]
	ldr r0, [r6, #4]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801B4C8 @ =gStaticData_0816C460
	adds r3, r0, r1
	cmp r2, #0
	beq _0801B4CC
	ldr r0, [r3]
	ldr r1, [r3, #4]
	ldr r2, [r3, #8]
	str r0, [r4, #0x54]
	str r1, [r4, #0x58]
	str r2, [r4, #0x5c]
	b _0801B4DC
	.align 2, 0
_0801B4C8: .4byte gStaticData_0816C460
_0801B4CC:
	ldr r0, [r3]
	rsbs r0, r0, #0
	ldr r1, [r3, #8]
	rsbs r1, r1, #0
	ldr r2, [r3, #4]
	str r0, [r4, #0x54]
	str r2, [r4, #0x58]
	str r1, [r4, #0x5c]
_0801B4DC:
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r6, #0x1c]
_0801B4E2:
	ldr r7, [r6, #0x10]
	cmp r7, #5
	bne _0801B558
	ldr r1, [r6, #0x14]
	cmp r1, #0
	ble _0801B518
	ldr r0, _0801B514 @ =gUnknown_0300082C
	ldr r0, [r0]
	subs r0, r0, r1
	cmp r0, #0x3c
	bne _0801B518
	ldr r1, [r6, #0xc]
	adds r1, #0x60
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r3, [r1, #4]
	adds r1, r4, #0
	movs r2, #3
	bl sub_803AD84
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r6, #0x14]
	b _0801B604
	.align 2, 0
_0801B514: .4byte gUnknown_0300082C
_0801B518:
	cmp r7, #5
	bne _0801B558
	cmp r1, #0
	ble _0801B558
	ldr r0, _0801B544 @ =gUnknown_0300082C
	ldr r5, [r0]
	subs r0, r5, r1
	movs r1, #0x1e
	bl sub_803AF1C
	cmp r0, #4
	bhi _0801B558
	movs r0, #1
	ands r5, r0
	cmp r5, #0
	bne _0801B54C
	ldr r0, [r4, #4]
	ldr r5, _0801B548 @ =0xFFFFFD00
	adds r0, r0, r5
	str r0, [r4, #4]
	b _0801B604
	.align 2, 0
_0801B544: .4byte gUnknown_0300082C
_0801B548: .4byte 0xFFFFFD00
_0801B54C:
	ldr r0, [r4, #4]
	movs r2, #0xc0
	lsls r2, r2, #2
	adds r0, r0, r2
	str r0, [r4, #4]
	b _0801B604
_0801B558:
	cmp r7, #7
	bne _0801B56C
	ldr r0, [r4, #0x30]
	cmp r0, #1
	bgt _0801B56C
	adds r0, r6, #0
	adds r0, #0x32
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801B5E6
_0801B56C:
	cmp r7, #6
	bne _0801B580
	ldr r0, [r4, #0x30]
	cmp r0, #1
	bgt _0801B580
	ldr r0, _0801B5C4 @ =gUnknown_0300082C
	ldr r1, [r0]
	ldr r0, [r6, #0x34]
	cmp r1, r0
	blo _0801B5E6
_0801B580:
	cmp r7, #7
	bne _0801B5D0
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801B5D0
	movs r0, #1
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _0801B5C8 @ =0x0000FFFF
	ldrh r2, [r4, #8]
	cmp r2, r0
	beq _0801B604
	ldrh r3, [r4, #8]
	ldr r0, _0801B5CC @ =gUnknown_030012B4
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
	b _0801B604
	.align 2, 0
_0801B5C4: .4byte gUnknown_0300082C
_0801B5C8: .4byte 0x0000FFFF
_0801B5CC: .4byte gUnknown_030012B4
_0801B5D0:
	cmp r7, #6
	bne _0801B604
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801B604
	ldr r0, _0801B620 @ =gUnknown_0300082C
	ldr r0, [r0]
	adds r0, #0x78
	str r0, [r6, #0x34]
_0801B5E6:
	movs r3, #0
	ldr r0, [r4, #0x20]
	adds r2, r4, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r5, [r2]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _0801B602
	subs r3, r0, #1
_0801B602:
	str r3, [r4, #0x30]
_0801B604:
	adds r0, r6, #0
	adds r1, r4, #0
	bl sub_801B624
	ldr r0, [r4]
	asrs r0, r0, #8
	str r0, [r6, #0x20]
	ldr r0, [r4, #4]
	asrs r0, r0, #8
	str r0, [r6, #0x24]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801B620: .4byte gUnknown_0300082C

	thumb_func_start sub_801B624
sub_801B624: @ 0x0801B624
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	adds r7, r1, #0
	adds r0, #0x32
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801B6D8
	ldr r0, [r6, #0x10]
	cmp r0, #6
	beq _0801B6D8
	ldr r0, _0801B698 @ =gUnknown_030012D8
	mov r8, r0
	ldr r2, [r0]
	ldrb r1, [r2, #0xc]
	lsrs r0, r1, #7
	cmp r0, #0
	beq _0801B6D8
	adds r0, r2, #0
	adds r0, #0xac
	str r7, [r0]
	movs r1, #8
	subs r0, #0x44
	strb r1, [r0]
	mov r1, r8
	ldr r0, [r1]
	ldr r1, [r0]
	asrs r1, r1, #8
	ldr r5, [r7]
	asrs r5, r5, #8
	ldr r2, [r6, #0x20]
	subs r5, r5, r2
	ldr r2, [r0, #4]
	asrs r2, r2, #8
	ldr r3, [r7, #4]
	asrs r3, r3, #8
	ldr r4, [r6, #0x24]
	subs r3, r3, r4
	adds r1, r1, r5
	adds r2, r2, r3
	lsls r1, r1, #8
	str r1, [r0]
	lsls r2, r2, #8
	str r2, [r0, #4]
	bl sub_8009EA8
	mov r1, r8
	ldr r0, [r1]
	adds r0, #0x24
	ldrb r2, [r0]
	ldr r0, [r7, #0x60]
	cmp r0, #0
	ble _0801B69C
	movs r0, #1
	orrs r2, r0
	b _0801B6A8
	.align 2, 0
_0801B698: .4byte gUnknown_030012D8
_0801B69C:
	cmp r0, #0
	bge _0801B6A8
	movs r0, #2
	orrs r2, r0
	lsls r0, r2, #0x18
	lsrs r2, r0, #0x18
_0801B6A8:
	ldr r1, [r7, #0x64]
	cmp r1, #0
	ble _0801B6B2
	movs r0, #8
	b _0801B6B8
_0801B6B2:
	cmp r1, #0
	bge _0801B6BE
	movs r0, #4
_0801B6B8:
	orrs r2, r0
	lsls r0, r2, #0x18
	lsrs r2, r0, #0x18
_0801B6BE:
	ldr r0, _0801B6E4 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x24
	strb r2, [r0]
	ldr r0, [r6, #0x10]
	cmp r0, #5
	bne _0801B6D8
	ldr r0, [r6, #0x14]
	cmp r0, #0
	bne _0801B6D8
	ldr r0, _0801B6E8 @ =gUnknown_0300082C
	ldr r0, [r0]
	str r0, [r6, #0x14]
_0801B6D8:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801B6E4: .4byte gUnknown_030012D8
_0801B6E8: .4byte gUnknown_0300082C
	thumb_func_start sub_801B6EC
sub_801B6EC: @ 0x0801B6EC
	adds r3, r1, #0
	ldr r0, [r0, #4]
	ldr r0, [r0, #0]
	lsls r2, r2, #3
	adds r2, r2, r0
	ldr r1, [r2, #4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801B720 @ =gStaticData_0816C460
	adds r2, r0, r1
	adds r0, r3, #0
	adds r0, #0x28
	ldrb r0, [r0, #0]
	lsls r0, r0, #26
	cmp r0, #0
	bge _0801B724
	ldr r0, [r2, #0]
	negs r0, r0
	ldr r1, [r2, #8]
	negs r1, r1
	ldr r2, [r2, #4]
	str r0, [r3, #0x54]
	str r2, [r3, #0x58]
	str r1, [r3, #0x5c]
	b _0801B730
	.align 2, 0
_0801B720: .4byte gStaticData_0816C460
_0801B724:
	ldr r0, [r2, #0]
	ldr r1, [r2, #4]
	ldr r2, [r2, #8]
	str r0, [r3, #0x54]
	str r1, [r3, #0x58]
	str r2, [r3, #0x5c]
_0801B730:
	bx lr
	.align 2, 0

	thumb_func_start sub_801B734
sub_801B734: @ 0x0801B734
	adds r3, r1, #0
	ldr r0, [r0, #4]
	ldr r0, [r0, #0]
	lsls r2, r2, #3
	adds r2, r2, r0
	ldr r1, [r2, #0]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801B768 @ =gStaticData_0816C460
	adds r2, r0, r1
	adds r0, r3, #0
	adds r0, #0x28
	ldrb r0, [r0, #0]
	lsls r0, r0, #27
	cmp r0, #0
	bge _0801B76C
	ldr r0, [r2, #0]
	negs r0, r0
	ldr r1, [r2, #8]
	negs r1, r1
	ldr r2, [r2, #4]
	str r0, [r3, #0x48]
	str r2, [r3, #0x4c]
	str r1, [r3, #0x50]
	b _0801B778
	.align 2, 0
_0801B768: .4byte gStaticData_0816C460
_0801B76C:
	ldr r0, [r2, #0]
	ldr r1, [r2, #4]
	ldr r2, [r2, #8]
	str r0, [r3, #0x48]
	str r1, [r3, #0x4c]
	str r2, [r3, #0x50]
_0801B778:
	bx lr

	thumb_func_start sub_801B77C
sub_801B77C: @ 0x0801B77C
	push {lr}
	ldr r3, [r0, #4]
	ldr r3, [r3]
	lsls r2, r2, #3
	adds r2, r2, r3
	ldr r3, [r2, #4]
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r2, r2, #2
	ldr r3, _0801B79C @ =gStaticData_0816C460
	adds r2, r2, r3
	bl sub_800B6D0
	pop {r0}
	bx r0
	.align 2, 0
_0801B79C: .4byte gStaticData_0816C460

	thumb_func_start sub_801B7A0
sub_801B7A0: @ 0x0801B7A0
	push {lr}
	ldr r3, [r0, #4]
	ldr r3, [r3]
	lsls r2, r2, #3
	adds r2, r2, r3
	ldr r3, [r2]
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r2, r2, #2
	ldr r3, _0801B7C0 @ =gStaticData_0816C460
	adds r2, r2, r3
	bl sub_800B7B0
	pop {r0}
	bx r0
	.align 2, 0
_0801B7C0: .4byte gStaticData_0816C460

	thumb_func_start sub_801B7C4
sub_801B7C4: @ 0x0801B7C4
	push {lr}
	ldr r2, _0801B7D4 @ =gStaticData_087E4A54
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_0801B7D4: .4byte gStaticData_087E4A54

	thumb_func_start sub_801B7D8
sub_801B7D8: @ 0x0801B7D8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	add r0, sp, #0x18
	lsls r3, r3, #0x18
	lsrs r3, r3, #0x18
	mov r8, r3
	ldrb r7, [r0]
	adds r0, r4, #0
	bl sub_800B8C8
	ldr r0, _0801B848 @ =gStaticData_087E4A54
	str r0, [r4, #0xc]
	ldr r0, [sp, #0x1c]
	subs r0, #6
	cmp r0, #1
	bhi _0801B804
	movs r6, #0
	movs r5, #0
_0801B804:
	movs r1, #0
	str r1, [r4, #0x20]
	str r5, [r4, #0x18]
	str r1, [r4, #0x24]
	str r6, [r4, #0x1c]
	ldr r0, _0801B84C @ =gStaticData_0816C458
	str r0, [r4, #4]
	adds r0, r4, #0
	adds r0, #0x32
	strb r1, [r0]
	ldr r0, [sp, #0x1c]
	str r0, [r4, #0x10]
	str r1, [r4, #0x14]
	lsls r0, r5, #1
	str r0, [r4, #0x28]
	lsls r0, r6, #1
	str r0, [r4, #0x2c]
	adds r0, r4, #0
	adds r0, #0x30
	mov r1, r8
	strb r1, [r0]
	adds r0, #1
	strb r7, [r0]
	ldr r0, _0801B850 @ =gUnknown_0300082C
	ldr r0, [r0]
	adds r0, #0x78
	str r0, [r4, #0x34]
	adds r0, r4, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0801B848: .4byte gStaticData_087E4A54
_0801B84C: .4byte gStaticData_0816C458
_0801B850: .4byte gUnknown_0300082C

	thumb_func_start sub_801B854
sub_801B854: @ 0x0801B854
	adds r0, #0x32
	movs r1, #0
	strb r1, [r0]
	bx lr

	thumb_func_start sub_801B85C
sub_801B85C: @ 0x0801B85C
	adds r0, #0x32
	movs r1, #1
	strb r1, [r0]
	bx lr

	thumb_func_start sub_801B864
sub_801B864: @ 0x0801B864
	push {r4, lr}
	adds r3, r0, #0
	ldrb r4, [r3, #0xd]
	lsrs r2, r4, #2
	movs r1, #1
	adds r0, r2, #0
	ands r0, r1
	cmp r0, #0
	bne _0801B888
	movs r0, #1
	eors r2, r0
	ands r2, r0
	lsls r1, r2, #2
	movs r0, #5
	rsbs r0, r0, #0
	ands r0, r4
	orrs r0, r1
	strb r0, [r3, #0xd]
_0801B888:
	ldr r0, _0801B8B4 @ =gUnknown_030012D4
	ldr r0, [r0]
	str r3, [r0, #0x10]
	ldr r0, _0801B8B8 @ =gUnknown_030012D8
	ldr r1, [r0]
	ldr r0, [r1]
	ldr r2, [r1, #4]
	movs r1, #0xf0
	lsls r1, r1, #5
	adds r0, r0, r1
	str r0, [r3]
	str r2, [r3, #4]
	movs r0, #0x10
	ldrb r2, [r3, #0xc]
	orrs r0, r2
	strb r0, [r3, #0xc]
	str r1, [r3, #0x78]
	str r1, [r3, #0x7c]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0801B8B4: .4byte gUnknown_030012D4
_0801B8B8: .4byte gUnknown_030012D8

	thumb_func_start sub_801B8BC
sub_801B8BC: @ 0x0801B8BC
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8009FB0
	movs r1, #1
	adds r0, r4, #0
	adds r0, #0x24
	movs r2, #0
	strb r1, [r0]
	adds r0, #0x44
	strb r2, [r0]
	ldr r0, [r4, #0x7c]
	ldr r1, [r4, #0x78]
	cmp r0, r1
	bge _0801B8E6
	movs r2, #0x80
	lsls r2, r2, #2
	adds r0, r0, r2
	cmp r0, r1
	bgt _0801B8F2
	b _0801B8FC
_0801B8E6:
	cmp r0, r1
	ble _0801B8FE
	ldr r2, _0801B8F8 @ =0xFFFFFE00
	adds r0, r0, r2
	cmp r0, r1
	bge _0801B8FC
_0801B8F2:
	str r1, [r4, #0x7c]
	b _0801B8FE
	.align 2, 0
_0801B8F8: .4byte 0xFFFFFE00
_0801B8FC:
	str r0, [r4, #0x7c]
_0801B8FE:
	ldr r0, _0801B918 @ =gUnknown_030012D8
	ldr r2, [r0]
	ldr r0, [r2]
	ldr r3, [r2, #4]
	ldr r1, [r4, #0x7c]
	adds r0, r0, r1
	str r0, [r4]
	str r3, [r4, #4]
	ldr r0, [r2, #0x60]
	str r0, [r4, #0x60]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0801B918: .4byte gUnknown_030012D8

	thumb_func_start sub_801B91C
sub_801B91C: @ 0x0801B91C
	push {lr}
	ldr r2, _0801B934 @ =gStaticData_087E4ABC
	str r2, [r0, #0x18]
	ldr r2, _0801B938 @ =gUnknown_030012D4
	ldr r3, [r2]
	ldr r2, _0801B93C @ =gUnknown_030012D8
	ldr r2, [r2]
	str r2, [r3, #0x10]
	bl sub_8009F1C
	pop {r0}
	bx r0
	.align 2, 0
_0801B934: .4byte gStaticData_087E4ABC
_0801B938: .4byte gUnknown_030012D4
_0801B93C: .4byte gUnknown_030012D8

	thumb_func_start sub_801B940
sub_801B940: @ 0x0801B940
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8009F90
	ldr r0, _0801B95C @ =gStaticData_087E4ABC
	str r0, [r4, #0x18]
	adds r0, r4, #0
	bl sub_801B864
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801B95C: .4byte gStaticData_087E4ABC
	thumb_func_start sub_801B960
sub_801B960: @ 0x0801B960
	adds	r2, r0, #0
	movs	r0, #200	@ 0xc8
	lsls	r0, r0, #6
	cmp	r1, r0
	ble _0801B96E
	adds	r1, r0, #0
	b _0801B978
_0801B96E:
	ldr r0, _0801B97C
	cmp	r1, r0
	bgt _0801B978
	movs	r1, #160	@ 0xa0
	lsls	r1, r1, #4
_0801B978:
	str	r1, [r2, #120]	@ 0x78
	bx	lr
_0801B97C: .4byte 0x9ff

	thumb_func_start sub_801B980
sub_801B980: @ 0x0801B980
	ldr r0, [r0, #0x78]
	bx lr

	thumb_func_start sub_801B984
sub_801B984: @ 0x0801B984
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	mov r8, r0
	adds r4, r1, #0
	adds r5, r2, #0
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	mov r8, r0
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	movs r0, #0x78
	bl sub_8026EDC
	adds r6, r0, #0
	bl sub_8009F90
	ldr r0, _0801BA50 @ =gStaticData_087E4B34
	str r0, [r6, #0x18]
	adds r0, r6, #0
	bl sub_801BAC4
	movs r1, #0
	mov sb, r1
	mov r2, r8
	strh r2, [r6, #8]
	lsls r4, r4, #8
	str r4, [r6]
	lsls r5, r5, #8
	str r5, [r6, #4]
	ldr r0, _0801BA54 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r6, #0
	bl sub_8008E94
	ldr r0, _0801BA58 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r3, #0xa8
	lsls r3, r3, #1
	adds r0, r0, r3
	str r0, [r6, #0x20]
	adds r4, r6, #0
	adds r4, #0x2d
	mov r0, sb
	strb r0, [r4]
	adds r0, r6, #0
	bl sub_80087C0
	adds r0, r6, #0
	bl sub_80087B4
	adds r0, r6, #0
	movs r1, #0
	bl sub_800872C
	adds r2, r6, #0
	adds r2, #0x28
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r1, [r2]
	ands r0, r1
	movs r1, #0x21
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r2]
	ldr r0, [r6, #0x20]
	ldr r1, [r0]
	ldrb r2, [r4]
	lsls r0, r2, #3
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r1, r1, r0
	ldr r0, _0801BA5C @ =gUnknown_030012B8
	ldr r0, [r0]
	ldrb r1, [r1, #0x14]
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	adds r2, r6, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	adds r0, r6, #0
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0801BA50: .4byte gStaticData_087E4B34
_0801BA54: .4byte gUnknown_030012F0
_0801BA58: .4byte gUnknown_030012D0
_0801BA5C: .4byte gUnknown_030012B8

	thumb_func_start sub_801BA60
sub_801BA60: @ 0x0801BA60
	push {r4, lr}
	sub sp, #0x10
	adds r1, r0, #0
	ldr r4, _0801BAAC @ =gUnknown_030012D8
	ldr r0, [r4]
	ldrb r0, [r0, #0xc]
	lsrs r0, r0, #7
	cmp r0, #0
	beq _0801BAA4
	mov r0, sp
	bl sub_8007B98
	ldr r0, [sp, #8]
	cmp r0, #0
	beq _0801BAA4
	ldr r0, [r4]
	mov r1, sp
	bl sub_800B37C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801BAA4
	ldr r0, [r4]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0x19
	movs r3, #0
	bl sub_803AD88
_0801BAA4:
	add sp, #0x10
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0801BAAC: .4byte gUnknown_030012D8

	thumb_func_start sub_801BAB0
sub_801BAB0: @ 0x0801BAB0
	push {lr}
	ldr r2, _0801BAC0 @ =gStaticData_087E4B34
	str r2, [r0, #0x18]
	bl sub_8009F1C
	pop {r0}
	bx r0
	.align 2, 0
_0801BAC0: .4byte gStaticData_087E4B34

	thumb_func_start sub_801BAC4
sub_801BAC4: @ 0x0801BAC4
	movs r1, #0x41
	rsbs r1, r1, #0
	ldrb r2, [r0, #0xc]
	ands r1, r2
	strb r1, [r0, #0xc]
	bx lr

	thumb_func_start sub_801BAD0
sub_801BAD0: @ 0x0801BAD0
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8009F90
	ldr r0, _0801BAEC @ =gStaticData_087E4B34
	str r0, [r4, #0x18]
	adds r0, r4, #0
	bl sub_801BAC4
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801BAEC: .4byte gStaticData_087E4B34

	thumb_func_start sub_801BAF0
sub_801BAF0: @ 0x0801BAF0
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r6, r0, #0
	movs r0, #0xc0
	lsls r0, r0, #0x18
	mov sb, r0
	bl mem_free_bytes
	bl sub_80006A8
	movs r0, #0xa0
	lsls r0, r0, #0x13
	movs r1, #0
	strh r1, [r0]
	movs r0, #0x80
	lsls r0, r0, #0x13
	strh r1, [r0]
	ldr r7, _0801BC0C @ =gUnknown_030012B8
	ldr r0, [r7]
	bl sub_8006EA8
	ldr r0, [r7]
	movs r1, #0xf
	bl sub_8006D50
	ldr r1, [r7]
	ldr r0, _0801BC10 @ =gStaticData_0816C56C
	movs r2, #0x83
	lsls r2, r2, #2
	adds r1, r1, r2
	movs r2, #0x10
	bl sub_803A94C
	ldr r5, _0801BC14 @ =gUnknown_030012FC
	ldr r0, [r5]
	movs r3, #0
	mov r8, r3
	str r3, [r0, #8]
	bl sub_8006C4C
	ldr r0, [r5]
	bl sub_8006C4C
	ldr r4, _0801BC18 @ =gUnknown_030012DC
	ldr r0, [r4]
	movs r2, #0x84
	lsls r2, r2, #1
	adds r1, r0, r2
	mov r3, r8
	str r3, [r1]
	adds r2, #0x28
	adds r1, r0, r2
	ldr r1, [r1]
	adds r1, #0x40
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r1, [r1, #4]
	bl sub_803AD7C
	ldr r0, [r5]
	ldr r1, [r4]
	movs r2, #0x96
	lsls r2, r2, #1
	adds r1, r1, r2
	ldr r1, [r1]
	lsls r1, r1, #5
	bl sub_8006C58
	ldr r0, [r4]
	movs r3, #0x96
	lsls r3, r3, #1
	adds r0, r0, r3
	ldr r2, [r0]
	ldr r4, _0801BC1C @ =gUnknown_030012E0
	ldr r0, [r4]
	subs r3, #0x24
	adds r1, r0, r3
	str r2, [r1]
	movs r2, #0x98
	lsls r2, r2, #1
	adds r1, r0, r2
	ldr r1, [r1]
	adds r1, #0x40
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r1, [r1, #4]
	bl sub_803AD7C
	ldr r0, [r5]
	ldr r1, [r4]
	movs r2, #0x96
	lsls r2, r2, #1
	adds r1, r1, r2
	ldr r1, [r1]
	lsls r1, r1, #5
	bl sub_8006C58
	ldr r0, [r5]
	bl sub_8006C30
	ldr r0, _0801BC20 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x10
	bl sub_8001B54
	ldr r4, _0801BC24 @ =gUnknown_03000820
	movs r0, #0xac
	bl sub_8026EDC
	ldr r1, [r6]
	bl sub_801BC28
	str r0, [r4]
	bl sub_801C96C
	str r0, [r6]
	ldr r0, [r4]
	ldrb r5, [r0]
	cmp r0, #0
	beq _0801BBEE
	movs r1, #3
	bl sub_801C040
_0801BBEE:
	mov r3, r8
	str r3, [r4]
	ldr r0, [r7]
	bl sub_8006EA8
	mov r0, sb
	bl mem_free_bytes
	adds r0, r5, #0
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0801BC0C: .4byte gUnknown_030012B8
_0801BC10: .4byte gStaticData_0816C56C
_0801BC14: .4byte gUnknown_030012FC
_0801BC18: .4byte gUnknown_030012DC
_0801BC1C: .4byte gUnknown_030012E0
_0801BC20: .4byte gUnknown_030012BC
_0801BC24: .4byte gUnknown_03000820

	thumb_func_start sub_801BC28
sub_801BC28: @ 0x0801BC28
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x14
	adds r7, r0, #0
	adds r6, r1, #0
	adds r4, r7, #0
	adds r4, #0xa0
	movs r0, #0
	mov sb, r0
	str r0, [r4]
	movs r0, #0xc0
	ldrb r1, [r4]
	orrs r0, r1
	movs r1, #0x20
	orrs r0, r1
	movs r3, #1
	orrs r0, r3
	movs r2, #2
	mov r8, r2
	mov r1, r8
	orrs r0, r1
	movs r1, #4
	orrs r0, r1
	movs r1, #8
	orrs r0, r1
	movs r5, #0x10
	orrs r0, r5
	strb r0, [r4]
	adds r2, r7, #0
	adds r2, #0xa4
	movs r0, #0x20
	rsbs r0, r0, #0
	ldrb r1, [r2]
	ands r0, r1
	orrs r0, r5
	strb r0, [r2]
	ldr r1, _0801BCC8 @ =0x04000050
	ldr r0, [r4]
	str r0, [r1]
	adds r1, #4
	ldrb r2, [r2]
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	adds r2, r7, #0
	adds r2, #0xa8
	mov r0, sb
	strh r0, [r2]
	movs r0, #0x40
	ldrb r1, [r2]
	orrs r0, r1
	movs r1, #8
	rsbs r1, r1, #0
	ands r0, r1
	orrs r0, r3
	strb r0, [r2]
	adds r0, r7, #0
	adds r0, #0xa9
	ldrb r2, [r0]
	orrs r3, r2
	mov r1, r8
	orrs r3, r1
	orrs r3, r5
	strb r3, [r0]
	cmp r6, #0x13
	bgt _0801BCCC
	adds r0, r6, #0
	movs r1, #5
	bl sub_803ADB4
	str r0, [r7, #0xc]
	adds r0, r6, #0
	movs r1, #5
	bl sub_803AE4C
	b _0801BCD4
	.align 2, 0
_0801BCC8: .4byte 0x04000050
_0801BCCC:
	adds r0, r6, #0
	subs r0, #0x14
	str r0, [r7, #0xc]
	movs r0, #5
_0801BCD4:
	str r0, [r7, #8]
	movs r4, #0
	str r4, [r7, #0x14]
	ldr r0, _0801BFA4 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80236EC
	adds r1, r7, #0
	adds r1, #0x9c
	str r0, [r1]
	strb r4, [r7]
	movs r0, #0x28
	bl sub_8026EDC
	movs r1, #0
	movs r2, #0x1d
	bl sub_801D7F8
	str r0, [r7, #0x1c]
	movs r0, #3
	str r0, [sp]
	add r0, sp, #4
	movs r1, #2
	movs r2, #0x1e
	movs r3, #2
	bl sub_801E644
	ldr r1, _0801BFA8 @ =gStaticData_0816C484
	add r0, sp, #4
	bl LoadGraphicsPackage
	str r4, [r7, #0x7c]
	movs r0, #0x54
	bl sub_8026EDC
	bl sub_801E04C
	str r0, [r7, #0x3c]
	movs r0, #0x8c
	bl sub_8026EDC
	movs r1, #3
	movs r2, #0x1f
	bl sub_801D828
	str r0, [r7, #0x20]
	adds r6, r7, #0
	adds r6, #0x40
	adds r5, r7, #0
	adds r5, #0x24
	movs r4, #5
_0801BD3A:
	movs r0, #0x14
	bl sub_8026EDC
	bl sub_801DFEC
	stm r5!, {r0}
	subs r4, #1
	cmp r4, #0
	bge _0801BD3A
	adds r0, r7, #0
	bl sub_801D638
	adds r0, r7, #0
	bl sub_801D5CC
	adds r0, r7, #0
	bl sub_801D668
	movs r5, #0
	movs r2, #0x80
	mov r8, r2
	adds r4, r6, #0
_0801BD66:
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	str r0, [r4]
	movs r1, #1
	bl sub_80088D8
	cmp r5, #1
	ble _0801BD82
	ldr r0, [r4]
	mov r1, r8
	strh r1, [r0, #0x3c]
_0801BD82:
	adds r4, #4
	adds r5, #1
	cmp r5, #7
	ble _0801BD66
	ldr r2, _0801BFAC @ =gUnknown_030012D0
	mov sl, r2
	ldr r0, [r2]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x8d
	lsls r1, r1, #2
	adds r0, r0, r1
	ldr r4, [r7, #0x40]
	str r0, [r4, #0x20]
	ldr r1, _0801BFB0 @ =gStaticData_0816C548
	ldr r0, [r7, #0xc]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	adds r1, r4, #0
	adds r1, #0x2d
	movs r2, #0
	mov sb, r2
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r7, #0x40]
	ldr r2, _0801BFB4 @ =gStaticData_0816C498
	ldr r1, [r2]
	ldr r2, [r2, #4]
	bl sub_800737C
	mov r1, sl
	ldr r0, [r1]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r2, #0x8d
	lsls r2, r2, #2
	adds r0, r0, r2
	ldr r4, [r7, #0x44]
	str r0, [r4, #0x20]
	movs r0, #0xa
	adds r1, r4, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r7, #0x44]
	ldr r2, _0801BFB8 @ =gStaticData_0816C4A0
	ldr r1, [r2]
	ldr r2, [r2, #4]
	bl sub_800737C
	mov r1, sl
	ldr r0, [r1]
	ldr r0, [r0]
	ldr r1, [r0]
	movs r2, #0xde
	lsls r2, r2, #1
	adds r1, r1, r2
	ldr r0, [r7, #0x48]
	str r1, [r0, #0x20]
	ldr r2, _0801BFBC @ =gStaticData_0816C4A8
	ldr r1, [r2]
	ldr r2, [r2, #4]
	bl sub_800737C
	mov r1, sl
	ldr r0, [r1]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r2, #0xc0
	lsls r2, r2, #1
	adds r0, r0, r2
	ldr r4, [r7, #0x4c]
	str r0, [r4, #0x20]
	movs r0, #1
	mov r8, r0
	adds r0, r4, #0
	adds r0, #0x2d
	mov r1, r8
	strb r1, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r7, #0x4c]
	ldr r5, _0801BFC0 @ =gStaticData_0816C4B0
	ldr r1, [r5]
	ldr r2, [r5, #4]
	bl sub_800737C
	mov r2, sl
	ldr r0, [r2]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r0, r0, r1
	ldr r4, [r7, #0x50]
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r2, r8
	strb r2, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r7, #0x50]
	ldr r1, [r5]
	ldr r2, [r5, #4]
	bl sub_800737C
	mov r1, sl
	ldr r0, [r1]
	ldr r0, [r0]
	ldr r1, [r0]
	movs r2, #0xc6
	lsls r2, r2, #1
	adds r1, r1, r2
	ldr r0, [r7, #0x54]
	str r1, [r0, #0x20]
	ldr r4, _0801BFC4 @ =gStaticData_0816C4B8
	ldr r1, [r4]
	ldr r2, [r4, #4]
	bl sub_800737C
	mov r1, sl
	ldr r0, [r1]
	ldr r0, [r0]
	ldr r1, [r0]
	movs r2, #0xc6
	lsls r2, r2, #1
	adds r1, r1, r2
	ldr r0, [r7, #0x58]
	str r1, [r0, #0x20]
	ldr r1, [r4]
	ldr r2, [r4, #4]
	bl sub_800737C
	mov r1, sl
	ldr r0, [r1]
	ldr r0, [r0]
	ldr r1, [r0]
	movs r2, #0xc6
	lsls r2, r2, #1
	adds r1, r1, r2
	ldr r0, [r7, #0x5c]
	str r1, [r0, #0x20]
	ldr r2, _0801BFC8 @ =gStaticData_0816C4C0
	ldr r1, [r2]
	ldr r2, [r2, #4]
	bl sub_800737C
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	str r0, [r7, #0x60]
	movs r1, #1
	bl sub_80088D8
	mov r1, sl
	ldr r0, [r1]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r2, #0x9c
	lsls r2, r2, #2
	adds r0, r0, r2
	ldr r4, [r7, #0x60]
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r1, r8
	strb r1, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r7, #0x60]
	ldr r2, _0801BFCC @ =gStaticData_0816C4C8
	ldr r1, [r2]
	ldr r2, [r2, #4]
	bl sub_800737C
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	str r0, [r7, #0x64]
	movs r1, #1
	bl sub_80088D8
	mov r2, sl
	ldr r0, [r2]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x9c
	lsls r1, r1, #2
	adds r0, r0, r1
	ldr r4, [r7, #0x64]
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r2, sb
	strb r2, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r7, #0x64]
	ldr r2, _0801BFD0 @ =gStaticData_0816C4D0
	ldr r1, [r2]
	ldr r2, [r2, #4]
	bl sub_800737C
	ldr r0, _0801BFD4 @ =gUnknown_03000824
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801BFD8
	adds r0, r7, #0
	bl sub_801D434
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801BFD8
	ldr r0, [r7, #0x3c]
	bl sub_801E408
	b _0801BFEC
	.align 2, 0
_0801BFA4: .4byte gUnknown_030012C0
_0801BFA8: .4byte gStaticData_0816C484
_0801BFAC: .4byte gUnknown_030012D0
_0801BFB0: .4byte gStaticData_0816C548
_0801BFB4: .4byte gStaticData_0816C498
_0801BFB8: .4byte gStaticData_0816C4A0
_0801BFBC: .4byte gStaticData_0816C4A8
_0801BFC0: .4byte gStaticData_0816C4B0
_0801BFC4: .4byte gStaticData_0816C4B8
_0801BFC8: .4byte gStaticData_0816C4C0
_0801BFCC: .4byte gStaticData_0816C4C8
_0801BFD0: .4byte gStaticData_0816C4D0
_0801BFD4: .4byte gUnknown_03000824
_0801BFD8:
	ldr r0, [r7, #8]
	lsls r0, r0, #3
	ldr r2, [r7, #0x18]
	adds r2, r2, r0
	ldr r0, [r7, #0x3c]
	ldr r1, [r2]
	ldr r2, [r2, #4]
	subs r2, #0x18
	bl sub_801E480
_0801BFEC:
	ldr r1, _0801C02C @ =0x04000010
	movs r0, #0
	str r0, [r1]
	ldr r0, [r7, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801C030 @ =0x04000014
	str r0, [r1]
	add r0, sp, #4
	bl sub_801E640
	ldr r1, _0801C034 @ =0x04000008
	strh r0, [r1]
	ldr r0, [r7, #0x1c]
	bl sub_801E640
	ldr r1, _0801C038 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r7, #0x20]
	bl sub_801DE24
	ldr r1, _0801C03C @ =0x0400000C
	strh r0, [r1]
	adds r0, r7, #0
	add sp, #0x14
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0801C02C: .4byte 0x04000010
_0801C030: .4byte 0x04000014
_0801C034: .4byte 0x04000008
_0801C038: .4byte 0x0400000A
_0801C03C: .4byte 0x0400000C

	thumb_func_start sub_801C040
sub_801C040: @ 0x0801C040
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	mov r8, r1
	ldr r2, [r6, #0x64]
	cmp r2, #0
	beq _0801C062
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0801C062:
	ldr r2, [r6, #0x60]
	cmp r2, #0
	beq _0801C07A
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0801C07A:
	adds r7, r6, #0
	adds r7, #0x24
	adds r4, r6, #0
	adds r4, #0x40
	movs r5, #7
_0801C084:
	ldr r2, [r4]
	cmp r2, #0
	beq _0801C09C
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0801C09C:
	adds r4, #4
	subs r5, #1
	cmp r5, #0
	bge _0801C084
	ldr r0, [r6, #0x3c]
	cmp r0, #0
	beq _0801C0B0
	movs r1, #3
	bl sub_801E524
_0801C0B0:
	ldr r0, [r6, #0x20]
	cmp r0, #0
	beq _0801C0BC
	movs r1, #3
	bl sub_801DA38
_0801C0BC:
	adds r4, r7, #0
	movs r5, #5
_0801C0C0:
	ldr r2, [r4]
	cmp r2, #0
	beq _0801C0D6
	ldr r1, [r2, #0x10]
	movs r3, #0x28
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0x2c]
	movs r1, #3
	bl sub_803AD80
_0801C0D6:
	adds r4, #4
	subs r5, #1
	cmp r5, #0
	bge _0801C0C0
	ldr r0, [r6, #0x1c]
	cmp r0, #0
	beq _0801C0EA
	movs r1, #3
	bl sub_801D7E0
_0801C0EA:
	movs r0, #1
	mov r1, r8
	ands r0, r1
	cmp r0, #0
	beq _0801C0FA
	adds r0, r6, #0
	bl sub_8026ED0
_0801C0FA:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_801C104
sub_801C104: @ 0x0801C104
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	ldr r0, _0801C27C @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006A90
	ldr r0, _0801C280 @ =gUnknown_030012FC
	ldr r0, [r0]
	bl sub_8006C28
	ldr r0, [r7, #0x3c]
	bl sub_801E2BC
	ldr r0, [r7, #0x20]
	bl sub_801DD18
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801C19E
	ldr r1, [r7, #8]
	lsls r1, r1, #2
	adds r0, r7, #0
	adds r0, #0x24
	adds r0, r0, r1
	ldr r0, [r0]
	bl sub_801DE28
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801C19E
	ldr r5, _0801C284 @ =gUnknown_030012E0
	ldr r0, [r5]
	movs r4, #0x98
	lsls r4, r4, #1
	adds r1, r0, r4
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r7, #0x14]
	ldr r2, [r2, #0x14]
	bl sub_803AD80
	movs r3, #0xf0
	subs r3, r3, r0
	lsrs r3, r3, #1
	adds r0, r7, #0
	adds r0, #0x80
	ldr r2, [r0]
	rsbs r2, r2, #0
	ldr r0, [r5]
	adds r2, #2
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
	ldr r1, [r7, #0x14]
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r0, [r7, #8]
	cmp r0, #4
	bgt _0801C19E
	adds r0, r7, #0
	bl sub_801C364
_0801C19E:
	ldr r0, [r7, #0x1c]
	bl sub_801D7D4
	movs r6, #0
	ldr r0, [r7, #4]
	movs r1, #0xa9
	adds r1, r1, r7
	mov r8, r1
	cmp r6, r0
	bgt _0801C1E0
_0801C1B2:
	lsls r1, r6, #2
	adds r0, r7, #0
	adds r0, #0x24
	adds r0, r0, r1
	ldr r4, [r0]
	ldr r0, [r4, #0x10]
	adds r5, r0, #0
	adds r5, #8
	movs r2, #8
	ldrsh r0, [r0, r2]
	adds r4, r4, r0
	ldr r0, [r7, #0x1c]
	bl sub_801D77C
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	adds r6, #1
	ldr r0, [r7, #4]
	cmp r6, r0
	ble _0801C1B2
_0801C1E0:
	ldr r0, [r7, #0x1c]
	bl sub_801D780
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801C25E
	ldr r0, [r7, #0x20]
	bl sub_801DCF8
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801C258
	movs r0, #0x2f
	bl sub_8026F38
	adds r6, r0, #0
	ldr r5, _0801C288 @ =gUnknown_030012DC
	ldr r0, [r5]
	movs r4, #0x98
	lsls r4, r4, #1
	adds r1, r0, r4
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	adds r1, r6, #0
	bl sub_803AD80
	movs r1, #0xf0
	subs r1, r1, r0
	lsrs r1, r1, #1
	ldr r0, [r5]
	movs r2, #0x96
	mov ip, r2
	movs r3, #0x88
	lsls r3, r3, #1
	adds r2, r0, r3
	str r1, [r2]
	movs r2, #0x8a
	lsls r2, r2, #1
	adds r1, r0, r2
	mov r3, ip
	str r3, [r1]
	movs r1, #0xf
	bl sub_8028A30
	ldr r0, [r5]
	adds r4, r0, r4
	ldr r2, [r4]
	movs r5, #0x20
	ldrsh r1, [r2, r5]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	adds r1, r6, #0
	bl sub_803AD80
	adds r0, r7, #0
	bl sub_801C2B0
_0801C258:
	ldr r0, [r7, #0x20]
	bl sub_801DC28
_0801C25E:
	ldr r0, [r7, #0x20]
	bl sub_801DD28
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801C28C
	movs r0, #5
	rsbs r0, r0, #0
	mov r1, r8
	ldrb r1, [r1]
	ands r0, r1
	mov r2, r8
	strb r0, [r2]
	b _0801C298
	.align 2, 0
_0801C27C: .4byte gUnknown_03001300
_0801C280: .4byte gUnknown_030012FC
_0801C284: .4byte gUnknown_030012E0
_0801C288: .4byte gUnknown_030012DC
_0801C28C:
	movs r0, #4
	mov r3, r8
	ldrb r3, [r3]
	orrs r0, r3
	mov r5, r8
	strb r0, [r5]
_0801C298:
	ldr r0, _0801C2AC @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006A48
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801C2AC: .4byte gUnknown_03001300

	thumb_func_start sub_801C2B0
sub_801C2B0: @ 0x0801C2B0
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	ldr r0, [r6, #0x60]
	bl sub_800815C
	ldr r2, [r6, #0x60]
	adds r2, #0x29
	movs r5, #0xf
	ands r0, r5
	movs r4, #0x10
	rsbs r4, r4, #0
	adds r1, r4, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldr r0, [r6, #0x64]
	bl sub_800815C
	ldr r1, [r6, #0x64]
	adds r1, #0x29
	ands r0, r5
	ldrb r5, [r1]
	ands r4, r5
	orrs r4, r0
	strb r4, [r1]
	ldr r0, [r6, #0xc]
	cmp r0, #2
	bgt _0801C326
	adds r0, r6, #0
	bl sub_801D434
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801C2FC
	ldr r1, [r6, #0x60]
	movs r4, #0
	b _0801C300
_0801C2FC:
	ldr r1, [r6, #0x60]
	movs r4, #1
_0801C300:
	ldr r0, [r1, #0x20]
	adds r3, r1, #0
	adds r3, #0x2d
	ldr r2, [r0]
	ldrb r5, [r3]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r2, [r0, #0x16]
	adds r0, r1, #0
	cmp r4, r2
	blt _0801C31C
	subs r4, r2, #1
_0801C31C:
	str r4, [r0, #0x30]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
_0801C326:
	adds r0, r6, #0
	bl sub_801D428
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801C35C
	ldr r3, [r6, #0x64]
	movs r4, #0
	ldr r0, [r3, #0x20]
	adds r2, r3, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r5, [r2]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _0801C350
	subs r4, r0, #1
_0801C350:
	str r4, [r3, #0x30]
	adds r0, r3, #0
	movs r1, #0
	movs r2, #0
	bl sub_8008890
_0801C35C:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801C364
sub_801C364: @ 0x0801C364
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x40]
	adds r5, r4, #0
	adds r5, #0x80
	ldr r1, [r5]
	rsbs r1, r1, #0
	movs r2, #0
	bl sub_8008890
	ldr r0, [r4, #0x44]
	ldr r1, [r5]
	rsbs r1, r1, #0
	movs r2, #0
	bl sub_8008890
	ldr r0, [r4, #0x48]
	ldr r1, [r5]
	rsbs r1, r1, #0
	adds r2, r4, #0
	adds r2, #0x84
	ldr r2, [r2]
	bl sub_8008890
	ldr r0, [r4, #0x4c]
	ldr r1, [r5]
	rsbs r1, r1, #0
	adds r2, r4, #0
	adds r2, #0x88
	ldr r2, [r2]
	bl sub_8008890
	adds r0, r4, #0
	adds r0, #0x98
	ldr r0, [r0]
	cmp r0, #5
	beq _0801C3BE
	ldr r0, [r4, #0x50]
	ldr r1, [r5]
	rsbs r1, r1, #0
	adds r2, r4, #0
	adds r2, #0x8c
	ldr r2, [r2]
	bl sub_8008890
_0801C3BE:
	adds r1, r4, #0
	adds r1, #0x9c
	ldr r0, [r4, #0x10]
	lsls r0, r0, #2
	adds r0, #4
	ldr r1, [r1]
	adds r1, r1, r0
	movs r0, #1
	ldrb r2, [r1]
	ands r0, r2
	cmp r0, #0
	beq _0801C3E2
	ldr r1, [r1]
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x13
	adds r0, r4, #0
	bl sub_801C3E8
_0801C3E2:
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_801C3E8
sub_801C3E8: @ 0x0801C3E8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	adds r5, r1, #0
	ldr r0, [r7, #0x54]
	adds r4, r7, #0
	adds r4, #0x80
	ldr r1, [r4]
	rsbs r1, r1, #0
	adds r2, r7, #0
	adds r2, #0x90
	ldr r2, [r2]
	bl sub_8008890
	ldr r0, [r7, #0x58]
	ldr r1, [r4]
	rsbs r1, r1, #0
	adds r2, r7, #0
	adds r2, #0x94
	ldr r2, [r2]
	bl sub_8008890
	ldr r1, [r7, #0x10]
	lsls r0, r1, #3
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801C468 @ =gStaticData_0816C86C
	adds r0, r0, r1
	cmp r5, #0
	beq _0801C474
	ldr r0, [r0, #0x10]
	cmp r5, r0
	bhi _0801C474
	ldr r2, _0801C46C @ =gStaticData_0816C4C0
	ldr r0, _0801C470 @ =gUnknown_030012E0
	ldr r3, [r0]
	ldr r1, [r4]
	ldr r0, [r2]
	adds r1, r1, r0
	adds r1, #0xa
	ldr r2, [r2, #4]
	subs r2, #8
	movs r4, #0x88
	lsls r4, r4, #1
	adds r0, r3, r4
	str r1, [r0]
	movs r5, #0x8a
	lsls r5, r5, #1
	adds r0, r3, r5
	str r2, [r0]
	movs r1, #0x98
	lsls r1, r1, #1
	adds r0, r3, r1
	ldr r2, [r0]
	movs r4, #0x20
	ldrsh r0, [r2, r4]
	adds r0, r3, r0
	adds r1, r7, #0
	adds r1, #0x68
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	b _0801C508
	.align 2, 0
_0801C468: .4byte gStaticData_0816C86C
_0801C46C: .4byte gStaticData_0816C4C0
_0801C470: .4byte gUnknown_030012E0
_0801C474:
	ldr r0, [r7, #0x5c]
	movs r5, #0x80
	adds r5, r5, r7
	mov r8, r5
	ldr r1, [r5]
	movs r2, #0
	bl sub_8008890
	ldr r6, _0801C514 @ =gUnknown_030012E0
	ldr r0, [r6]
	ldr r1, [r7, #0x5c]
	adds r1, #0x29
	ldrb r1, [r1]
	lsls r1, r1, #0x1c
	lsrs r1, r1, #0x1c
	bl sub_8028A30
	ldr r5, _0801C518 @ =gStaticData_0816C4C0
	ldr r0, [r6]
	mov r1, r8
	ldr r2, [r1]
	ldr r1, [r5]
	adds r2, r2, r1
	adds r2, #0xa
	ldr r3, [r5, #4]
	subs r3, #8
	movs r4, #0x88
	lsls r4, r4, #1
	adds r1, r0, r4
	str r2, [r1]
	movs r2, #0x8a
	lsls r2, r2, #1
	adds r1, r0, r2
	str r3, [r1]
	adds r4, #0x20
	adds r1, r0, r4
	ldr r2, [r1]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	adds r1, r7, #0
	adds r1, #0x71
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r0, [r6]
	bl sub_8028A40
	ldr r0, [r6]
	mov r1, r8
	ldr r2, [r1]
	ldr r1, [r5]
	adds r2, r2, r1
	adds r2, #0xa
	ldr r3, [r5, #4]
	adds r3, #8
	movs r5, #0x88
	lsls r5, r5, #1
	adds r1, r0, r5
	str r2, [r1]
	movs r2, #0x8a
	lsls r2, r2, #1
	adds r1, r0, r2
	str r3, [r1]
	adds r4, r0, r4
	ldr r2, [r4]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	adds r1, r7, #0
	adds r1, #0x68
	ldr r2, [r2, #0x24]
	bl sub_803AD80
_0801C508:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801C514: .4byte gUnknown_030012E0
_0801C518: .4byte gStaticData_0816C4C0

	thumb_func_start sub_801C51C
sub_801C51C: @ 0x0801C51C
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	ldr r0, [r5, #0x1c]
	bl sub_801D7AC
	ldr r0, [r5, #0x3c]
	bl sub_801E190
	ldr r0, [r5, #0x1c]
	bl sub_801D780
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801C5F6
	ldr r0, [r5, #0x3c]
	bl sub_801E464
	lsls r0, r0, #0x18
	adds r6, r5, #0
	adds r6, #0x24
	cmp r0, #0
	beq _0801C5C2
	ldr r0, [r5, #8]
	lsls r0, r0, #2
	adds r0, r6, r0
	ldr r0, [r0]
	bl sub_801DE28
	lsls r0, r0, #0x18
	lsrs r7, r0, #0x18
	cmp r7, #0
	bne _0801C5C2
	ldr r0, [r5, #8]
	lsls r0, r0, #2
	adds r0, r6, r0
	ldr r4, [r0]
	adds r0, r4, #0
	movs r1, #1
	bl sub_801DEA0
	ldr r0, [r5, #0x20]
	bl sub_801DD18
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801C59A
	adds r0, r4, #0
	bl sub_801DE2C
	str r0, [r5, #0x10]
	lsls r4, r0, #3
	adds r4, r4, r0
	lsls r4, r4, #2
	ldr r0, _0801C5FC @ =gStaticData_0816C86C
	adds r4, r4, r0
	ldr r0, [r5, #0x20]
	ldr r1, [r4, #4]
	bl sub_801DD80
	ldr r0, [r4]
	bl sub_8026F38
	str r0, [r5, #0x14]
_0801C59A:
	adds r0, r5, #0
	adds r0, #0x80
	str r7, [r0]
	ldr r0, [r5, #8]
	cmp r0, #4
	bgt _0801C5BA
	adds r0, r5, #0
	bl sub_801C608
	ldr r0, _0801C600 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006EA8
	adds r0, r5, #0
	bl sub_801D730
_0801C5BA:
	ldr r0, _0801C604 @ =gUnknown_030012E0
	ldr r0, [r0]
	bl sub_8028A40
_0801C5C2:
	adds r7, r5, #0
	adds r7, #0x40
	movs r4, #5
_0801C5C8:
	ldm r6!, {r0}
	ldr r2, [r0, #0x10]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r2, #0x24]
	bl sub_803AD7C
	subs r4, #1
	cmp r4, #0
	bge _0801C5C8
	ldr r0, [r5, #0x20]
	bl sub_801DAD8
	adds r5, r7, #0
	adds r5, #8
	movs r4, #5
_0801C5EA:
	ldm r5!, {r0}
	bl sub_8008044
	subs r4, #1
	cmp r4, #0
	bge _0801C5EA
_0801C5F6:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801C5FC: .4byte gStaticData_0816C86C
_0801C600: .4byte gUnknown_030012B8
_0801C604: .4byte gUnknown_030012E0

	thumb_func_start sub_801C608
sub_801C608: @ 0x0801C608
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xc
	adds r7, r0, #0
	movs r0, #0x98
	adds r0, r0, r7
	mov r8, r0
	movs r0, #5
	mov r1, r8
	str r0, [r1]
	ldr r4, _0801C6F4 @ =gUnknown_030012C0
	ldr r0, [r4]
	ldr r1, [r7, #0x10]
	bl sub_802336C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801C638
	movs r0, #0
	mov r2, r8
	str r0, [r2]
_0801C638:
	ldr r0, [r4]
	ldr r1, [r7, #0x10]
	bl sub_8023360
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801C64C
	movs r0, #1
	mov r3, r8
	str r0, [r3]
_0801C64C:
	ldr r0, [r4]
	ldr r1, [r7, #0x10]
	bl sub_8023354
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801C660
	movs r0, #2
	mov r6, r8
	str r0, [r6]
_0801C660:
	ldr r0, [r4]
	ldr r1, [r7, #0x10]
	bl sub_8023348
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801C674
	movs r0, #3
	mov r1, r8
	str r0, [r1]
_0801C674:
	ldr r0, [r4]
	ldr r1, [r7, #0x10]
	bl sub_802333C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801C688
	movs r0, #4
	mov r2, r8
	str r0, [r2]
_0801C688:
	movs r3, #0x84
	adds r3, r3, r7
	mov ip, r3
	movs r0, #0
	str r0, [r3]
	movs r6, #0x88
	adds r6, r6, r7
	mov sl, r6
	str r0, [r6]
	adds r5, r7, #0
	adds r5, #0x8c
	str r0, [r5]
	adds r4, r7, #0
	adds r4, #0x90
	str r0, [r4]
	adds r3, r7, #0
	adds r3, #0x94
	str r0, [r3]
	adds r2, r7, #0
	adds r2, #0x9c
	ldr r0, [r7, #0x10]
	lsls r0, r0, #2
	adds r0, #4
	ldr r1, [r2]
	adds r1, r1, r0
	mov sb, r1
	movs r0, #1
	ldrb r1, [r1]
	ands r0, r1
	str r4, [sp, #4]
	str r3, [sp, #8]
	cmp r0, #0
	beq _0801C6D0
	movs r0, #0x1c
	mov r3, ip
	str r0, [r3]
_0801C6D0:
	movs r0, #2
	mov r1, sb
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _0801C6E0
	movs r0, #0x1c
	str r0, [r6]
_0801C6E0:
	mov r3, r8
	ldr r0, [r3]
	cmp r0, #5
	bhi _0801C74A
	lsls r0, r0, #2
	ldr r1, _0801C6F8 @ =_0801C6FC
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801C6F4: .4byte gUnknown_030012C0
_0801C6F8: .4byte _0801C6FC
_0801C6FC: @ jump table
	.4byte _0801C714 @ case 0
	.4byte _0801C71C @ case 1
	.4byte _0801C722 @ case 2
	.4byte _0801C728 @ case 3
	.4byte _0801C72E @ case 4
	.4byte _0801C73E @ case 5
_0801C714:
	movs r0, #4
	mov r1, sb
	ldrb r1, [r1]
	b _0801C734
_0801C71C:
	ldr r1, [r2]
	movs r0, #1
	b _0801C732
_0801C722:
	ldr r1, [r2]
	movs r0, #4
	b _0801C732
_0801C728:
	ldr r1, [r2]
	movs r0, #8
	b _0801C732
_0801C72E:
	ldr r1, [r2]
	movs r0, #2
_0801C732:
	ldrb r1, [r1, #2]
_0801C734:
	ands r0, r1
	cmp r0, #0
	beq _0801C73E
	movs r0, #0x1c
	str r0, [r5]
_0801C73E:
	adds r0, r7, #0
	adds r0, #0x98
	ldr r1, [r0]
	adds r3, r0, #0
	cmp r1, #5
	beq _0801C782
_0801C74A:
	ldr r1, _0801C86C @ =gStaticData_0816C558
	ldr r0, [r3]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r4, [r7, #0x50]
	ldr r0, [r0]
	adds r1, r4, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r1, [r6]
	ldr r0, [r5]
	cmp r1, r0
	bne _0801C782
	subs r0, r1, #6
	str r0, [r6]
	ldr r0, [r5]
	adds r0, #6
	str r0, [r5]
_0801C782:
	movs r2, #1
	mov sl, r2
	mov r0, sl
	mov r3, sb
	ldrb r3, [r3]
	ands r0, r3
	cmp r0, #0
	bne _0801C794
	b _0801C95A
_0801C794:
	ldr r1, [r7, #0x10]
	lsls r0, r1, #3
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801C870 @ =gStaticData_0816C86C
	adds r5, r0, r1
	str r5, [sp]
	ldr r0, [r5, #8]
	adds r6, r7, #0
	adds r6, #0x71
	adds r1, r6, #0
	bl FormatCentiseconds
	mov r1, sb
	ldr r0, [r1]
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x13
	adds r1, r7, #0
	adds r1, #0x68
	bl FormatCentiseconds
	ldr r4, [r7, #0x54]
	movs r2, #0
	mov r8, r2
	adds r0, r4, #0
	adds r0, #0x2d
	mov r3, r8
	strb r3, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r4, [r7, #0x58]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r1, r8
	strb r1, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r4, [r7, #0x5c]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r2, r8
	strb r2, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, _0801C874 @ =0x0000FFF8
	mov r3, sb
	ldrh r3, [r3]
	ands r0, r3
	cmp r0, #0
	bne _0801C82A
	b _0801C95A
_0801C82A:
	mov r1, sb
	ldr r0, [r1]
	lsls r1, r0, #0x10
	lsrs r0, r1, #0x13
	ldr r2, [r5, #0x10]
	cmp r0, r2
	bhi _0801C878
	movs r0, #0x1c
	ldr r2, [sp, #8]
	str r0, [r2]
	ldr r3, [sp, #4]
	str r0, [r3]
	ldr r4, [r7, #0x54]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r6, sl
	strb r6, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r4, [r7, #0x58]
	adds r0, r4, #0
	adds r0, #0x2d
	strb r6, [r0]
	b _0801C8D4
	.align 2, 0
_0801C86C: .4byte gStaticData_0816C558
_0801C870: .4byte gStaticData_0816C86C
_0801C874: .4byte 0x0000FFF8
_0801C878:
	lsrs r0, r1, #0x13
	ldr r3, [r5, #0xc]
	cmp r0, r3
	bhi _0801C8EA
	adds r0, r2, #0
	adds r1, r6, #0
	bl FormatCentiseconds
	movs r0, #0x1c
	ldr r1, [sp, #4]
	str r0, [r1]
	ldr r4, [r7, #0x54]
	movs r0, #2
	adds r1, r4, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r4, [r7, #0x58]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r2, sl
	strb r2, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r4, [r7, #0x5c]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r3, sl
	strb r3, [r0]
_0801C8D4:
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	b _0801C95A
_0801C8EA:
	lsrs r1, r1, #0x13
	ldr r2, [sp]
	ldr r0, [r2, #8]
	cmp r1, r0
	bhi _0801C95A
	adds r0, r3, #0
	adds r1, r6, #0
	bl FormatCentiseconds
	movs r0, #0x1c
	ldr r3, [sp, #4]
	str r0, [r3]
	ldr r4, [r7, #0x54]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r6, r8
	strb r6, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r4, [r7, #0x58]
	movs r5, #2
	adds r0, r4, #0
	adds r0, #0x2d
	strb r5, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r4, [r7, #0x5c]
	adds r0, r4, #0
	adds r0, #0x2d
	strb r5, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
_0801C95A:
	add sp, #0xc
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801C96C
sub_801C96C: @ 0x0801C96C
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	movs r0, #0
	strb r0, [r5]
	ldr r1, [r5, #8]
	lsls r1, r1, #2
	adds r0, r5, #0
	adds r0, #0x24
	adds r0, r0, r1
	ldr r0, [r0]
	bl sub_801DE2C
	str r0, [r5, #0x10]
	lsls r4, r0, #3
	adds r4, r4, r0
	lsls r4, r4, #2
	ldr r0, _0801C9A8 @ =gStaticData_0816C86C
	adds r4, r4, r0
	ldr r0, [r5, #0x20]
	ldr r1, [r4, #4]
	bl sub_801DD80
	ldr r0, [r4]
	bl sub_8026F38
	str r0, [r5, #0x14]
	b _0801CA48
	.align 2, 0
_0801C9A8: .4byte gStaticData_0816C86C
_0801C9AC:
	adds r4, r5, #0
	adds r4, #0xa4
	ldrb r2, [r4]
	movs r1, #0x1f
	movs r0, #0x1f
	ands r0, r2
	cmp r0, #0
	beq _0801C9D0
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1b
	subs r0, #1
	ands r0, r1
	movs r3, #0x20
	rsbs r3, r3, #0
	adds r1, r3, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r4]
_0801C9D0:
	adds r0, r5, #0
	bl sub_801C104
	bl sub_80006A8
	ldr r0, _0801CADC @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801CAE0 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r5, #0x20]
	bl sub_801DCBC
	ldr r0, [r5, #0x7c]
	adds r0, #1
	str r0, [r5, #0x7c]
	ldr r1, _0801CAE4 @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r5, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801CAE8 @ =0x04000014
	str r0, [r1]
	ldr r0, [r5, #0x1c]
	bl sub_801E640
	ldr r1, _0801CAEC @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r5, #0x20]
	bl sub_801DE24
	ldr r1, _0801CAF0 @ =0x0400000C
	strh r0, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	strh r6, [r0]
	adds r1, #0x44
	adds r0, r5, #0
	adds r0, #0xa0
	ldr r0, [r0]
	str r0, [r1]
	adds r1, #4
	ldrb r4, [r4]
	lsls r0, r4, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	adds r0, r5, #0
	adds r0, #0xa8
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, [r5, #0x20]
	bl sub_801DAD8
_0801CA48:
	ldr r0, [r5, #0x20]
	bl sub_801DD18
	lsls r0, r0, #0x18
	lsrs r6, r0, #0x18
	cmp r6, #0
	beq _0801C9AC
	ldr r0, _0801CAF4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x51
	bl PlaySfx
	adds r3, r5, #0
	adds r3, #0xa0
	movs r6, #0
	str r6, [r3]
	adds r2, r5, #0
	adds r2, #0xa1
	movs r0, #1
	ldrb r4, [r2]
	orrs r0, r4
	movs r1, #2
	orrs r0, r1
	movs r1, #4
	orrs r0, r1
	movs r1, #8
	orrs r0, r1
	movs r1, #0x20
	orrs r0, r1
	strb r0, [r2]
	adds r4, r5, #0
	adds r4, #0xa2
	movs r1, #0x20
	rsbs r1, r1, #0
	adds r0, r1, #0
	ldrb r2, [r4]
	ands r0, r2
	movs r2, #0x10
	orrs r0, r2
	strb r0, [r4]
	adds r0, r5, #0
	adds r0, #0xa3
	ldrb r4, [r0]
	ands r1, r4
	orrs r1, r2
	strb r1, [r0]
	ldr r0, _0801CAF8 @ =gUnknown_03000824
	ldrb r0, [r0]
	mov sb, r3
	cmp r0, #0
	beq _0801CAC6
	adds r0, r5, #0
	bl sub_801D434
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801CAC6
	str r6, [r5, #8]
	adds r0, r5, #0
	bl sub_801D548
_0801CAC6:
	ldr r1, _0801CAF8 @ =gUnknown_03000824
	movs r0, #0
	strb r0, [r1]
	movs r0, #0x24
	adds r0, r0, r5
	mov r8, r0
	adds r7, r5, #0
	adds r7, #0xa4
	adds r6, r5, #0
	adds r6, #0xa8
	b _0801CB14
	.align 2, 0
_0801CADC: .4byte gUnknown_030012B8
_0801CAE0: .4byte gUnknown_03001300
_0801CAE4: .4byte 0x04000010
_0801CAE8: .4byte 0x04000014
_0801CAEC: .4byte 0x0400000A
_0801CAF0: .4byte 0x0400000C
_0801CAF4: .4byte gUnknown_030012BC
_0801CAF8: .4byte gUnknown_03000824
_0801CAFC:
	ldr r1, _0801CB10 @ =gUnknown_030007E0
	movs r0, #8
	ldrh r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _0801CB14
	adds r0, r5, #0
	bl sub_801D300
	b _0801CC50
	.align 2, 0
_0801CB10: .4byte gUnknown_030007E0
_0801CB14:
	adds r0, r5, #0
	bl sub_801C104
	bl sub_80006A8
	ldr r0, _0801CBC0 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801CBC4 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r5, #0x20]
	bl sub_801DCBC
	ldr r0, [r5, #0x7c]
	adds r0, #1
	str r0, [r5, #0x7c]
	ldr r1, _0801CBC8 @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r5, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801CBCC @ =0x04000014
	str r0, [r1]
	ldr r0, [r5, #0x1c]
	bl sub_801E640
	ldr r1, _0801CBD0 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r5, #0x20]
	bl sub_801DE24
	ldr r1, _0801CBD4 @ =0x0400000C
	strh r0, [r1]
	movs r1, #0xa0
	lsls r1, r1, #0x13
	movs r0, #0
	strh r0, [r1]
	ldr r1, _0801CBD8 @ =0x04000050
	mov r2, sb
	ldr r0, [r2]
	str r0, [r1]
	adds r1, #4
	ldrb r3, [r7]
	lsls r0, r3, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	ldrh r0, [r6]
	strh r0, [r1]
	adds r0, r5, #0
	bl sub_801C51C
	ldr r0, [r5, #0x1c]
	bl sub_801D780
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801CB14
	ldr r0, [r5, #0x3c]
	bl sub_801E464
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801CB14
	ldr r0, _0801CBDC @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r0, _0801CBE0 @ =gUnknown_030007E0
	ldr r2, [r0]
	lsrs r1, r2, #0x10
	movs r0, #0x40
	ands r0, r1
	cmp r0, #0
	beq _0801CBE4
	adds r0, r5, #0
	bl sub_801D548
	b _0801CC1A
	.align 2, 0
_0801CBC0: .4byte gUnknown_030012B8
_0801CBC4: .4byte gUnknown_03001300
_0801CBC8: .4byte 0x04000010
_0801CBCC: .4byte 0x04000014
_0801CBD0: .4byte 0x0400000A
_0801CBD4: .4byte 0x0400000C
_0801CBD8: .4byte 0x04000050
_0801CBDC: .4byte gUnknown_03001304
_0801CBE0: .4byte gUnknown_030007E0
_0801CBE4:
	lsrs r1, r2, #0x10
	movs r0, #0x80
	ands r0, r1
	adds r1, r2, #0
	cmp r0, #0
	beq _0801CBF8
	adds r0, r5, #0
	bl sub_801D4C4
	b _0801CC1A
_0801CBF8:
	lsrs r1, r1, #0x10
	movs r0, #0x20
	ands r0, r1
	cmp r0, #0
	beq _0801CC0A
	adds r0, r5, #0
	bl sub_801CDE0
	b _0801CC1A
_0801CC0A:
	lsrs r1, r2, #0x10
	movs r0, #0x10
	ands r0, r1
	cmp r0, #0
	beq _0801CC1A
	adds r0, r5, #0
	bl sub_801CE60
_0801CC1A:
	ldr r1, _0801CCDC @ =gUnknown_030007E0
	movs r0, #1
	ldrh r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	bne _0801CC28
	b _0801CAFC
_0801CC28:
	ldr r0, [r5, #8]
	lsls r0, r0, #2
	add r0, r8
	ldr r0, [r0]
	bl sub_801DE28
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801CC3C
	b _0801CAFC
_0801CC3C:
	ldr r0, [r5, #0x20]
	bl sub_801DD18
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801CC4A
	b _0801CAFC
_0801CC4A:
	adds r0, r5, #0
	bl sub_801D110
_0801CC50:
	movs r4, #0
	strh r4, [r6]
	movs r0, #0x40
	ldrb r1, [r6]
	orrs r0, r1
	strb r0, [r6]
	bl sub_80006A8
	ldr r0, _0801CCE0 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801CCE4 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r5, #0x20]
	bl sub_801DCBC
	ldr r0, [r5, #0x7c]
	adds r0, #1
	str r0, [r5, #0x7c]
	ldr r1, _0801CCE8 @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r5, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801CCEC @ =0x04000014
	str r0, [r1]
	ldr r0, [r5, #0x1c]
	bl sub_801E640
	ldr r1, _0801CCF0 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r5, #0x20]
	bl sub_801DE24
	ldr r1, _0801CCF4 @ =0x0400000C
	strh r0, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	strh r4, [r0]
	adds r1, #0x44
	mov r2, sb
	ldr r0, [r2]
	str r0, [r1]
	adds r1, #4
	ldrb r7, [r7]
	lsls r0, r7, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	ldrh r0, [r6]
	strh r0, [r1]
	ldr r0, [r5, #8]
	lsls r0, r0, #2
	add r0, r8
	ldr r0, [r0]
	bl sub_801DE2C
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0801CCDC: .4byte gUnknown_030007E0
_0801CCE0: .4byte gUnknown_030012B8
_0801CCE4: .4byte gUnknown_03001300
_0801CCE8: .4byte 0x04000010
_0801CCEC: .4byte 0x04000014
_0801CCF0: .4byte 0x0400000A
_0801CCF4: .4byte 0x0400000C

	thumb_func_start sub_801CCF8
sub_801CCF8: @ 0x0801CCF8
	push {r4, lr}
	adds r4, r0, #0
	ldr r1, [r4, #8]
	lsls r1, r1, #2
	adds r0, #0x24
	adds r0, r0, r1
	ldr r0, [r0]
	movs r1, #0
	bl sub_801DEA0
	ldr r0, [r4, #0x20]
	bl sub_801DD5C
	ldr r0, [r4, #0x3c]
	bl sub_801E408
	b _0801CD9E
_0801CD1A:
	adds r0, r4, #0
	bl sub_801C104
	bl sub_80006A8
	ldr r0, _0801CDC4 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801CDC8 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r4, #0x20]
	bl sub_801DCBC
	ldr r0, [r4, #0x7c]
	adds r0, #1
	str r0, [r4, #0x7c]
	ldr r1, _0801CDCC @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801CDD0 @ =0x04000014
	str r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801E640
	ldr r1, _0801CDD4 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r4, #0x20]
	bl sub_801DE24
	ldr r1, _0801CDD8 @ =0x0400000C
	strh r0, [r1]
	movs r1, #0xa0
	lsls r1, r1, #0x13
	movs r0, #0
	strh r0, [r1]
	ldr r1, _0801CDDC @ =0x04000050
	adds r0, r4, #0
	adds r0, #0xa0
	ldr r0, [r0]
	str r0, [r1]
	adds r1, #4
	adds r0, r4, #0
	adds r0, #0xa4
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	adds r0, r4, #0
	adds r0, #0xa8
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, [r4, #0x3c]
	bl sub_801E190
	ldr r0, [r4, #0x20]
	bl sub_801DAD8
_0801CD9E:
	ldr r0, [r4, #0x20]
	bl sub_801DD38
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801CD1A
	ldr r0, [r4, #0x3c]
	bl sub_801E464
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801CD1A
	ldr r0, _0801CDC4 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006EA8
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0801CDC4: .4byte gUnknown_030012B8
_0801CDC8: .4byte gUnknown_03001300
_0801CDCC: .4byte 0x04000010
_0801CDD0: .4byte 0x04000014
_0801CDD4: .4byte 0x0400000A
_0801CDD8: .4byte 0x0400000C
_0801CDDC: .4byte 0x04000050

	thumb_func_start sub_801CDE0
sub_801CDE0: @ 0x0801CDE0
	push {r4, lr}
	adds r4, r0, #0
	ldr r1, [r4, #8]
	cmp r1, #0
	bne _0801CE00
	ldr r0, _0801CDFC @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x48
	bl PlaySfx
	b _0801CE50
	.align 2, 0
_0801CDFC: .4byte gUnknown_030012BC
_0801CE00:
	lsls r1, r1, #2
	adds r0, r4, #0
	adds r0, #0x24
	adds r0, r0, r1
	ldr r0, [r0]
	movs r1, #0
	bl sub_801DEA0
	ldr r0, [r4, #0x20]
	bl sub_801DD5C
	b _0801CE4A
_0801CE18:
	ldr r0, [r4, #8]
	subs r0, #1
	str r0, [r4, #8]
	lsls r0, r0, #3
	ldr r2, [r4, #0x18]
	adds r2, r2, r0
	ldr r0, [r4, #0x3c]
	ldr r1, [r2]
	ldr r2, [r2, #4]
	subs r2, #0x18
	bl sub_801E480
	adds r0, r4, #0
	bl sub_801D05C
	ldr r0, _0801CE58 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r0, _0801CE5C @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #0x20
	ands r0, r1
	cmp r0, #0
	beq _0801CE50
_0801CE4A:
	ldr r0, [r4, #8]
	cmp r0, #0
	bne _0801CE18
_0801CE50:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0801CE58: .4byte gUnknown_03001304
_0801CE5C: .4byte gUnknown_030007E0

	thumb_func_start sub_801CE60
sub_801CE60: @ 0x0801CE60
	push {r4, lr}
	adds r4, r0, #0
	ldr r1, [r4, #8]
	ldr r0, [r4, #4]
	cmp r1, r0
	bne _0801CE80
	ldr r0, _0801CE7C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x48
	bl PlaySfx
	b _0801CED0
	.align 2, 0
_0801CE7C: .4byte gUnknown_030012BC
_0801CE80:
	lsls r1, r1, #2
	adds r0, r4, #0
	adds r0, #0x24
	adds r0, r0, r1
	ldr r0, [r0]
	movs r1, #0
	bl sub_801DEA0
	ldr r0, [r4, #0x20]
	bl sub_801DD5C
	b _0801CEC8
_0801CE98:
	adds r0, r1, #1
	str r0, [r4, #8]
	lsls r0, r0, #3
	ldr r2, [r4, #0x18]
	adds r2, r2, r0
	ldr r0, [r4, #0x3c]
	ldr r1, [r2]
	ldr r2, [r2, #4]
	subs r2, #0x18
	bl sub_801E480
	adds r0, r4, #0
	bl sub_801D05C
	ldr r0, _0801CED8 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r0, _0801CEDC @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #0x10
	ands r0, r1
	cmp r0, #0
	beq _0801CED0
_0801CEC8:
	ldr r1, [r4, #8]
	ldr r0, [r4, #4]
	cmp r1, r0
	blt _0801CE98
_0801CED0:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0801CED8: .4byte gUnknown_03001304
_0801CEDC: .4byte gUnknown_030007E0

	thumb_func_start sub_801CEE0
sub_801CEE0: @ 0x0801CEE0
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r4, r0, #0
	b _0801D036
_0801CEEC:
	adds r0, r4, #0
	bl sub_801C104
	bl sub_80006A8
	ldr r0, _0801CFD4 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801CFD8 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r4, #0x20]
	bl sub_801DCBC
	ldr r0, [r4, #0x7c]
	adds r0, #1
	str r0, [r4, #0x7c]
	ldr r1, _0801CFDC @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801CFE0 @ =0x04000014
	str r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801E640
	ldr r1, _0801CFE4 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r4, #0x20]
	bl sub_801DE24
	ldr r1, _0801CFE8 @ =0x0400000C
	strh r0, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	strh r5, [r0]
	adds r1, #0x44
	adds r0, r4, #0
	adds r0, #0xa0
	ldr r0, [r0]
	str r0, [r1]
	adds r1, #4
	adds r0, r4, #0
	adds r0, #0xa4
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	adds r0, r4, #0
	adds r0, #0xa8
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801D7AC
	ldr r0, [r4, #0x3c]
	bl sub_801E190
	ldr r0, [r4, #0x1c]
	bl sub_801D77C
	movs r1, #0xff
	ands r1, r0
	cmp r1, #0xa0
	bne _0801D036
	movs r5, #0
	adds r7, r4, #0
	adds r7, #0x24
	movs r0, #0x9c
	adds r0, r0, r4
	mov r8, r0
	adds r6, r7, #0
_0801CF8A:
	ldm r6!, {r0}
	ldr r2, [r0, #0x10]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r4, #0xc]
	ldr r3, [r2, #0x14]
	adds r2, r5, #0
	bl sub_803AD84
	adds r5, #1
	cmp r5, #5
	ble _0801CF8A
	movs r3, #0
	movs r2, #0
	ldr r0, [r4, #0xc]
	lsls r1, r0, #2
	adds r1, r1, r0
	mov r5, r8
	ldr r0, [r5]
	lsls r1, r1, #2
	adds r1, r1, r0
_0801CFB6:
	ldrb r5, [r1, #4]
	lsls r0, r5, #0x1f
	lsrs r0, r0, #0x1f
	adds r3, r3, r0
	adds r1, #4
	adds r2, #1
	cmp r2, #4
	ble _0801CFB6
	cmp r3, #5
	bne _0801CFF0
	ldr r0, _0801CFEC @ =gStaticData_0816C508
	str r0, [r4, #0x18]
	str r3, [r4, #4]
	b _0801CFF8
	.align 2, 0
_0801CFD4: .4byte gUnknown_030012B8
_0801CFD8: .4byte gUnknown_03001300
_0801CFDC: .4byte 0x04000010
_0801CFE0: .4byte 0x04000014
_0801CFE4: .4byte 0x0400000A
_0801CFE8: .4byte 0x0400000C
_0801CFEC: .4byte gStaticData_0816C508
_0801CFF0:
	ldr r0, _0801D054 @ =gStaticData_0816C4D8
	str r0, [r4, #0x18]
	movs r0, #4
	str r0, [r4, #4]
_0801CFF8:
	movs r5, #0
	adds r6, r7, #0
_0801CFFC:
	ldm r6!, {r0}
	ldr r3, [r0, #0x10]
	movs r2, #0x18
	ldrsh r1, [r3, r2]
	adds r0, r0, r1
	lsls r2, r5, #3
	ldr r1, [r4, #0x18]
	adds r1, r1, r2
	ldr r2, [r3, #0x1c]
	bl sub_803AD80
	adds r5, #1
	cmp r5, #5
	ble _0801CFFC
	ldr r2, _0801D058 @ =gStaticData_0816C538
	adds r6, r7, #0
	movs r5, #5
_0801D01E:
	ldm r6!, {r0}
	ldr r1, [r4, #0xc]
	lsls r1, r1, #2
	adds r1, r1, r2
	ldr r1, [r1]
	str r2, [sp]
	bl sub_801DF0C
	subs r5, #1
	ldr r2, [sp]
	cmp r5, #0
	bge _0801D01E
_0801D036:
	ldr r0, [r4, #0x1c]
	bl sub_801D780
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	cmp r5, #0
	bne _0801D046
	b _0801CEEC
_0801D046:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801D054: .4byte gStaticData_0816C4D8
_0801D058: .4byte gStaticData_0816C538

	thumb_func_start sub_801D05C
sub_801D05C: @ 0x0801D05C
	push {r4, r5, lr}
	adds r4, r0, #0
	b _0801D0E4
_0801D062:
	adds r0, r4, #0
	bl sub_801C104
	bl sub_80006A8
	ldr r0, _0801D0F8 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801D0FC @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r4, #0x20]
	bl sub_801DCBC
	ldr r0, [r4, #0x7c]
	adds r0, #1
	str r0, [r4, #0x7c]
	ldr r1, _0801D100 @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801D104 @ =0x04000014
	str r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801E640
	ldr r1, _0801D108 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r4, #0x20]
	bl sub_801DE24
	ldr r1, _0801D10C @ =0x0400000C
	strh r0, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	strh r5, [r0]
	adds r1, #0x44
	adds r0, r4, #0
	adds r0, #0xa0
	ldr r0, [r0]
	str r0, [r1]
	adds r1, #4
	adds r0, r4, #0
	adds r0, #0xa4
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	adds r0, r4, #0
	adds r0, #0xa8
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, [r4, #0x3c]
	bl sub_801E190
	ldr r0, [r4, #0x20]
	bl sub_801DAD8
_0801D0E4:
	ldr r0, [r4, #0x3c]
	bl sub_801E464
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	cmp r5, #0
	beq _0801D062
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0801D0F8: .4byte gUnknown_030012B8
_0801D0FC: .4byte gUnknown_03001300
_0801D100: .4byte 0x04000010
_0801D104: .4byte 0x04000014
_0801D108: .4byte 0x0400000A
_0801D10C: .4byte 0x0400000C

	thumb_func_start sub_801D110
sub_801D110: @ 0x0801D110
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	ldr r0, _0801D150 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x52
	bl PlaySfx
	ldr r1, [r6, #8]
	lsls r1, r1, #2
	adds r0, r6, #0
	adds r0, #0x24
	adds r0, r0, r1
	ldr r0, [r0]
	movs r1, #0
	bl sub_801DEA0
	ldr r0, [r6, #0x3c]
	movs r1, #0x78
	movs r2, #0x35
	bl sub_801E480
	ldr r0, [r6, #0x3c]
	bl sub_801E3F4
	adds r0, r6, #0
	bl sub_801D05C
	b _0801D1D6
	.align 2, 0
_0801D150: .4byte gUnknown_030012BC
_0801D154:
	adds r0, r6, #0
	bl sub_801C104
	bl sub_80006A8
	ldr r0, _0801D224 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801D228 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r6, #0x20]
	bl sub_801DCBC
	ldr r0, [r6, #0x7c]
	adds r0, #1
	str r0, [r6, #0x7c]
	ldr r1, _0801D22C @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r6, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801D230 @ =0x04000014
	str r0, [r1]
	ldr r0, [r6, #0x1c]
	bl sub_801E640
	ldr r1, _0801D234 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r6, #0x20]
	bl sub_801DE24
	ldr r1, _0801D238 @ =0x0400000C
	strh r0, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	strh r4, [r0]
	adds r1, #0x44
	adds r0, r6, #0
	adds r0, #0xa0
	ldr r0, [r0]
	str r0, [r1]
	adds r1, #4
	adds r0, r6, #0
	adds r0, #0xa4
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	adds r0, r6, #0
	adds r0, #0xa8
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, [r6, #0x3c]
	bl sub_801E190
	ldr r0, [r6, #0x20]
	bl sub_801DAD8
_0801D1D6:
	ldr r0, [r6, #0x20]
	bl sub_801DD18
	lsls r0, r0, #0x18
	lsrs r4, r0, #0x18
	cmp r4, #0
	beq _0801D154
	adds r5, r6, #0
	adds r5, #0xa0
	movs r0, #0xc0
	ldrb r1, [r5]
	orrs r0, r1
	movs r1, #0x20
	orrs r0, r1
	movs r1, #1
	orrs r0, r1
	movs r1, #2
	orrs r0, r1
	movs r1, #4
	orrs r0, r1
	movs r1, #8
	orrs r0, r1
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r5]
	adds r4, r6, #0
	adds r4, #0xa4
	movs r0, #0x20
	rsbs r0, r0, #0
	ldrb r2, [r4]
	ands r0, r2
	strb r0, [r4]
	movs r7, #0
	ldr r0, [r6, #0x20]
	bl sub_801DD48
	mov r8, r5
	b _0801D2D0
	.align 2, 0
_0801D224: .4byte gUnknown_030012B8
_0801D228: .4byte gUnknown_03001300
_0801D22C: .4byte 0x04000010
_0801D230: .4byte 0x04000014
_0801D234: .4byte 0x0400000A
_0801D238: .4byte 0x0400000C
_0801D23C:
	adds r0, r6, #0
	bl sub_801C104
	bl sub_80006A8
	ldr r0, _0801D2E8 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801D2EC @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r6, #0x20]
	bl sub_801DCBC
	ldr r0, [r6, #0x7c]
	adds r0, #1
	str r0, [r6, #0x7c]
	ldr r1, _0801D2F0 @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r6, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801D2F4 @ =0x04000014
	str r0, [r1]
	ldr r0, [r6, #0x1c]
	bl sub_801E640
	ldr r1, _0801D2F8 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r6, #0x20]
	bl sub_801DE24
	ldr r1, _0801D2FC @ =0x0400000C
	strh r0, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	strh r5, [r0]
	adds r1, #0x44
	mov r2, r8
	ldr r0, [r2]
	str r0, [r1]
	adds r1, #4
	ldrb r2, [r4]
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	adds r0, r6, #0
	adds r0, #0xa8
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, [r6, #0x3c]
	bl sub_801E190
	ldr r0, [r6, #0x20]
	bl sub_801DAD8
	adds r7, #1
	lsrs r1, r7, #0x1f
	adds r1, r7, r1
	asrs r1, r1, #1
	movs r0, #0x1f
	ands r1, r0
	movs r0, #0x20
	rsbs r0, r0, #0
	ldrb r2, [r4]
	ands r0, r2
	orrs r0, r1
	strb r0, [r4]
_0801D2D0:
	ldr r0, [r6, #0x20]
	bl sub_801DD08
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	cmp r5, #0
	beq _0801D23C
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801D2E8: .4byte gUnknown_030012B8
_0801D2EC: .4byte gUnknown_03001300
_0801D2F0: .4byte 0x04000010
_0801D2F4: .4byte 0x04000014
_0801D2F8: .4byte 0x0400000A
_0801D2FC: .4byte 0x0400000C

	thumb_func_start sub_801D300
sub_801D300: @ 0x0801D300
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	ldr r0, _0801D350 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
	adds r2, r5, #0
	adds r2, #0xa0
	movs r0, #0xc0
	ldrb r1, [r2]
	orrs r0, r1
	movs r1, #0x20
	orrs r0, r1
	movs r1, #1
	orrs r0, r1
	movs r1, #2
	orrs r0, r1
	movs r1, #4
	orrs r0, r1
	movs r1, #8
	orrs r0, r1
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r2]
	adds r4, r5, #0
	adds r4, #0xa4
	movs r0, #0x20
	rsbs r0, r0, #0
	ldrb r2, [r4]
	ands r0, r2
	strb r0, [r4]
	movs r7, #0
	ldr r0, [r5, #0x20]
	bl sub_801DD5C
	adds r6, r4, #0
	b _0801D3EC
	.align 2, 0
_0801D350: .4byte gUnknown_030012BC
_0801D354:
	adds r0, r5, #0
	bl sub_801C104
	bl sub_80006A8
	ldr r0, _0801D404 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801D408 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r5, #0x20]
	bl sub_801DCBC
	ldr r0, [r5, #0x7c]
	adds r0, #1
	str r0, [r5, #0x7c]
	ldr r1, _0801D40C @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r5, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801D410 @ =0x04000014
	str r0, [r1]
	ldr r0, [r5, #0x1c]
	bl sub_801E640
	ldr r1, _0801D414 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r5, #0x20]
	bl sub_801DE24
	ldr r1, _0801D418 @ =0x0400000C
	strh r0, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	strh r4, [r0]
	adds r1, #0x44
	adds r0, r5, #0
	adds r0, #0xa0
	ldr r0, [r0]
	str r0, [r1]
	adds r1, #4
	ldrb r2, [r6]
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	adds r0, r5, #0
	adds r0, #0xa8
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, [r5, #0x3c]
	bl sub_801E190
	ldr r0, [r5, #0x20]
	bl sub_801DAD8
	adds r7, #1
	lsrs r1, r7, #0x1f
	adds r1, r7, r1
	asrs r1, r1, #1
	movs r0, #0x1f
	ands r1, r0
	movs r2, #0x20
	rsbs r2, r2, #0
	adds r0, r2, #0
	ldrb r2, [r6]
	ands r0, r2
	orrs r0, r1
	strb r0, [r6]
_0801D3EC:
	ldr r0, [r5, #0x20]
	bl sub_801DD28
	lsls r0, r0, #0x18
	lsrs r4, r0, #0x18
	cmp r4, #0
	beq _0801D354
	movs r0, #1
	strb r0, [r5]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801D404: .4byte gUnknown_030012B8
_0801D408: .4byte gUnknown_03001300
_0801D40C: .4byte 0x04000010
_0801D410: .4byte 0x04000014
_0801D414: .4byte 0x0400000A
_0801D418: .4byte 0x0400000C

	thumb_func_start sub_801D41C
sub_801D41C: @ 0x0801D41C
	ldr r1, _0801D424 @ =gUnknown_03000824
	movs r0, #1
	strb r0, [r1]
	bx lr
	.align 2, 0
_0801D424: .4byte gUnknown_03000824

	thumb_func_start sub_801D428
sub_801D428: @ 0x0801D428
	ldr r1, [r0, #0xc]
	rsbs r0, r1, #0
	orrs r0, r1
	lsrs r0, r0, #0x1f
	bx lr
	.align 2, 0

	thumb_func_start sub_801D434
sub_801D434: @ 0x0801D434
	movs r2, #0
	ldr r1, [r0, #0xc]
	cmp r1, #1
	beq _0801D456
	cmp r1, #1
	bgt _0801D446
	cmp r1, #0
	beq _0801D44C
	b _0801D46C
_0801D446:
	cmp r1, #2
	beq _0801D460
	b _0801D46C
_0801D44C:
	adds r0, #0x9c
	ldr r0, [r0]
	ldrb r0, [r0, #2]
	lsrs r2, r0, #5
	b _0801D468
_0801D456:
	adds r0, #0x9c
	ldr r0, [r0]
	ldrb r0, [r0, #2]
	lsrs r2, r0, #7
	b _0801D46C
_0801D460:
	adds r0, #0x9c
	ldr r0, [r0]
	ldrb r0, [r0, #2]
	lsrs r2, r0, #6
_0801D468:
	movs r0, #1
	ands r2, r0
_0801D46C:
	adds r0, r2, #0
	bx lr

	thumb_func_start sub_801D470
sub_801D470: @ 0x0801D470
	push {r4, r5, lr}
	adds r5, r0, #0
	ldr r1, _0801D4C0 @ =gStaticData_0816C548
	ldr r0, [r5, #0xc]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r4, [r5, #0x40]
	ldr r0, [r0]
	adds r1, r4, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r5, #8]
	ldr r1, [r5, #4]
	cmp r0, r1
	ble _0801D4A4
	str r1, [r5, #8]
_0801D4A4:
	ldr r0, [r5, #8]
	lsls r0, r0, #3
	ldr r2, [r5, #0x18]
	adds r2, r2, r0
	ldr r0, [r5, #0x3c]
	ldr r1, [r2]
	ldr r2, [r2, #4]
	subs r2, #0x18
	bl sub_801E480
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0801D4C0: .4byte gStaticData_0816C548

	thumb_func_start sub_801D4C4
sub_801D4C4: @ 0x0801D4C4
	push {r4, lr}
	adds r4, r0, #0
	bl sub_801D428
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801D530
	adds r0, r4, #0
	bl sub_801CCF8
	ldr r0, _0801D4E8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x56
	bl PlaySfx
	b _0801D512
	.align 2, 0
_0801D4E8: .4byte gUnknown_030012BC
_0801D4EC:
	ldr r0, [r4, #0xc]
	subs r0, #1
	str r0, [r4, #0xc]
	ldr r0, [r4, #0x1c]
	bl sub_801D790
	adds r0, r4, #0
	bl sub_801CEE0
	ldr r0, _0801D528 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r0, _0801D52C @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #0x80
	ands r0, r1
	cmp r0, #0
	beq _0801D51E
_0801D512:
	adds r0, r4, #0
	bl sub_801D428
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801D4EC
_0801D51E:
	adds r0, r4, #0
	bl sub_801D470
	b _0801D53E
	.align 2, 0
_0801D528: .4byte gUnknown_03001304
_0801D52C: .4byte gUnknown_030007E0
_0801D530:
	ldr r0, _0801D544 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x48
	bl PlaySfx
_0801D53E:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0801D544: .4byte gUnknown_030012BC

	thumb_func_start sub_801D548
sub_801D548: @ 0x0801D548
	push {r4, lr}
	adds r4, r0, #0
	bl sub_801D434
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801D5B4
	adds r0, r4, #0
	bl sub_801CCF8
	ldr r0, _0801D56C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x55
	bl PlaySfx
	b _0801D596
	.align 2, 0
_0801D56C: .4byte gUnknown_030012BC
_0801D570:
	ldr r0, [r4, #0xc]
	adds r0, #1
	str r0, [r4, #0xc]
	ldr r0, [r4, #0x1c]
	bl sub_801D79C
	adds r0, r4, #0
	bl sub_801CEE0
	ldr r0, _0801D5AC @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r0, _0801D5B0 @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #0x40
	ands r0, r1
	cmp r0, #0
	beq _0801D5A2
_0801D596:
	adds r0, r4, #0
	bl sub_801D434
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801D570
_0801D5A2:
	adds r0, r4, #0
	bl sub_801D470
	b _0801D5C2
	.align 2, 0
_0801D5AC: .4byte gUnknown_03001304
_0801D5B0: .4byte gUnknown_030007E0
_0801D5B4:
	ldr r0, _0801D5C8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x48
	bl PlaySfx
_0801D5C2:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0801D5C8: .4byte gUnknown_030012BC

	thumb_func_start sub_801D5CC
sub_801D5CC: @ 0x0801D5CC
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	movs r3, #0
	movs r2, #0
	ldr r1, [r5, #0xc]
	lsls r0, r1, #2
	adds r0, r0, r1
	adds r1, r5, #0
	adds r1, #0x9c
	ldr r1, [r1]
	lsls r0, r0, #2
	adds r1, r0, r1
_0801D5E4:
	ldrb r4, [r1, #4]
	lsls r0, r4, #0x1f
	lsrs r0, r0, #0x1f
	adds r3, r3, r0
	adds r1, #4
	adds r2, #1
	cmp r2, #4
	ble _0801D5E4
	cmp r3, #5
	bne _0801D604
	ldr r0, _0801D600 @ =gStaticData_0816C508
	str r0, [r5, #0x18]
	str r3, [r5, #4]
	b _0801D60C
	.align 2, 0
_0801D600: .4byte gStaticData_0816C508
_0801D604:
	ldr r0, _0801D634 @ =gStaticData_0816C4D8
	str r0, [r5, #0x18]
	movs r0, #4
	str r0, [r5, #4]
_0801D60C:
	movs r4, #0
	adds r6, r5, #0
	adds r6, #0x24
_0801D612:
	ldm r6!, {r0}
	ldr r3, [r0, #0x10]
	movs r2, #0x18
	ldrsh r1, [r3, r2]
	adds r0, r0, r1
	lsls r2, r4, #3
	ldr r1, [r5, #0x18]
	adds r1, r1, r2
	ldr r2, [r3, #0x1c]
	bl sub_803AD80
	adds r4, #1
	cmp r4, #5
	ble _0801D612
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801D634: .4byte gStaticData_0816C4D8

	thumb_func_start sub_801D638
sub_801D638: @ 0x0801D638
	push {r4, r5, lr}
	adds r5, r0, #0
	movs r4, #0
_0801D63E:
	lsls r1, r4, #2
	adds r0, r5, #0
	adds r0, #0x24
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r2, [r0, #0x10]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r5, #0xc]
	ldr r3, [r2, #0x14]
	adds r2, r4, #0
	bl sub_803AD84
	adds r4, #1
	cmp r4, #5
	ble _0801D63E
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801D668
sub_801D668: @ 0x0801D668
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	movs r4, #0
	ldr r6, _0801D694 @ =gStaticData_0816C538
_0801D670:
	lsls r1, r4, #2
	adds r0, r5, #0
	adds r0, #0x24
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r5, #0xc]
	lsls r1, r1, #2
	adds r1, r1, r6
	ldr r1, [r1]
	bl sub_801DF0C
	adds r4, #1
	cmp r4, #5
	ble _0801D670
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801D694: .4byte gStaticData_0816C538

	thumb_func_start sub_801D698
sub_801D698: @ 0x0801D698
	push {r4, lr}
	adds r4, r0, #0
	bl sub_80006A8
	ldr r0, _0801D714 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801D718 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r4, #0x20]
	bl sub_801DCBC
	ldr r0, [r4, #0x7c]
	adds r0, #1
	str r0, [r4, #0x7c]
	ldr r1, _0801D71C @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801D720 @ =0x04000014
	str r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801E640
	ldr r1, _0801D724 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r4, #0x20]
	bl sub_801DE24
	ldr r1, _0801D728 @ =0x0400000C
	strh r0, [r1]
	movs r1, #0xa0
	lsls r1, r1, #0x13
	movs r0, #0
	strh r0, [r1]
	ldr r1, _0801D72C @ =0x04000050
	adds r0, r4, #0
	adds r0, #0xa0
	ldr r0, [r0]
	str r0, [r1]
	adds r1, #4
	adds r0, r4, #0
	adds r0, #0xa4
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	adds r0, r4, #0
	adds r0, #0xa8
	ldrh r0, [r0]
	strh r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0801D714: .4byte gUnknown_030012B8
_0801D718: .4byte gUnknown_03001300
_0801D71C: .4byte 0x04000010
_0801D720: .4byte 0x04000014
_0801D724: .4byte 0x0400000A
_0801D728: .4byte 0x0400000C
_0801D72C: .4byte 0x04000050

	thumb_func_start sub_801D730
sub_801D730: @ 0x0801D730
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r0, _0801D778 @ =gUnknown_030012B8
	ldr r0, [r0]
	movs r1, #0xf
	bl sub_8006D50
	movs r5, #0
	movs r0, #0x10
	rsbs r0, r0, #0
	adds r7, r0, #0
_0801D746:
	lsls r0, r5, #2
	adds r4, r6, #0
	adds r4, #0x40
	adds r4, r4, r0
	ldr r0, [r4]
	bl sub_800815C
	ldr r2, [r4]
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	ldrb r1, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	adds r5, #1
	cmp r5, #7
	ble _0801D746
	adds r0, r6, #0
	bl sub_801D668
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801D778: .4byte gUnknown_030012B8

	thumb_func_start sub_801D77C
sub_801D77C: @ 0x0801D77C
	ldr r0, [r0, #0x10]
	bx lr

	thumb_func_start sub_801D780
sub_801D780: @ 0x0801D780
	movs r2, #0
	ldr r1, [r0, #0x10]
	ldr r0, [r0, #0x14]
	cmp r1, r0
	bne _0801D78C
	movs r2, #1
_0801D78C:
	adds r0, r2, #0
	bx lr

	thumb_func_start sub_801D790
sub_801D790: @ 0x0801D790
	ldr r1, [r0, #0x14]
	movs r2, #0x80
	lsls r2, r2, #1
	adds r1, r1, r2
	str r1, [r0, #0x14]
	bx lr

	thumb_func_start sub_801D79C
sub_801D79C: @ 0x0801D79C
	ldr r1, [r0, #0x14]
	ldr r2, _0801D7A8 @ =0xFFFFFF00
	adds r1, r1, r2
	str r1, [r0, #0x14]
	bx lr
	.align 2, 0
_0801D7A8: .4byte 0xFFFFFF00

	thumb_func_start sub_801D7AC
sub_801D7AC: @ 0x0801D7AC
	adds r1, r0, #0
	ldr r2, [r1, #0x10]
	ldr r0, [r1, #0x14]
	cmp r2, r0
	bge _0801D7BC
	adds r0, r2, #0
	adds r0, #8
	str r0, [r1, #0x10]
_0801D7BC:
	ldr r2, [r1, #0x10]
	ldr r0, [r1, #0x14]
	cmp r2, r0
	ble _0801D7CA
	adds r0, r2, #0
	subs r0, #8
	str r0, [r1, #0x10]
_0801D7CA:
	ldr r0, [r1, #0x10]
	strh r0, [r1, #0x26]
	bx lr

	thumb_func_start sub_801D7D0
sub_801D7D0: @ 0x0801D7D0
	ldr r0, [r0, #0x24]
	bx lr

	thumb_func_start sub_801D7D4
sub_801D7D4: @ 0x0801D7D4
	movs r1, #8
	strh r1, [r0, #0x24]
	ldrh r1, [r0, #0x10]
	adds r1, #0x30
	strh r1, [r0, #0x26]
	bx lr

	thumb_func_start sub_801D7E0
sub_801D7E0: @ 0x0801D7E0
	push {lr}
	adds r2, r0, #0
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _0801D7F2
	adds r0, r2, #0
	bl sub_8026ED0
_0801D7F2:
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801D7F8
sub_801D7F8: @ 0x0801D7F8
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	movs r0, #2
	str r0, [sp]
	adds r0, r4, #0
	movs r3, #0
	bl sub_801E644
	movs r0, #0xc0
	lsls r0, r0, #2
	str r0, [r4, #0x14]
	str r0, [r4, #0x10]
	ldr r1, _0801D824 @ =gStaticData_0816C58C
	adds r0, r4, #0
	bl LoadGraphicsPackage
	adds r0, r4, #0
	add sp, #4
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801D824: .4byte gStaticData_0816C58C

	thumb_func_start sub_801D828
sub_801D828: @ 0x0801D828
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xc
	adds r6, r0, #0
	movs r0, #0x78
	str r0, [r6, #0x20]
	movs r0, #0x35
	str r0, [r6, #0x24]
	movs r5, #0
	strh r5, [r6, #0x34]
	adds r4, r6, #0
	adds r4, #0x34
	subs r0, #0x39
	ldrb r3, [r4]
	ands r0, r3
	movs r3, #1
	orrs r0, r3
	strb r0, [r4]
	str r1, [r6, #0x18]
	movs r0, #3
	ands r1, r0
	lsls r1, r1, #2
	movs r0, #0xd
	rsbs r0, r0, #0
	ldrb r3, [r4]
	ands r0, r3
	orrs r0, r1
	strb r0, [r4]
	adds r1, r6, #0
	adds r1, #0x35
	movs r0, #0x3f
	ldrb r3, [r1]
	ands r0, r3
	strb r0, [r1]
	str r2, [r6, #0x1c]
	movs r0, #0x1f
	ands r2, r0
	movs r0, #0x20
	rsbs r0, r0, #0
	ldrb r3, [r1]
	ands r0, r3
	orrs r0, r2
	strb r0, [r1]
	movs r0, #0x80
	ldrb r1, [r4]
	orrs r0, r1
	strb r0, [r4]
	ldr r0, [r6, #0x1c]
	lsls r0, r0, #0xb
	movs r2, #0xc0
	lsls r2, r2, #0x13
	adds r3, r0, r2
	mov r0, sp
	strh r5, [r0]
	ldr r0, _0801DA20 @ =0x040000D4
	mov r4, sp
	str r4, [r0]
	str r3, [r0, #4]
	ldr r1, _0801DA24 @ =0x81000080
	str r1, [r0, #8]
	ldr r0, [r0, #8]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r0, #0
	ldr r1, [r6, #0x20]
	mov sl, r1
	movs r4, #0x40
	adds r4, r4, r6
	mov r8, r4
	movs r1, #0x42
	adds r1, r1, r6
	mov ip, r1
	movs r4, #0x48
	adds r4, r4, r6
	mov sb, r4
	adds r1, r6, #0
	adds r1, #0x64
	str r1, [sp, #4]
	adds r4, r6, #0
	adds r4, #0x88
	str r4, [sp, #8]
	ldr r7, _0801DA28 @ =0x00000202
_0801D8D2:
	adds r5, r3, #0
	adds r5, #0x10
	adds r4, r0, #1
	adds r0, r3, #0
	movs r1, #3
_0801D8DC:
	strh r2, [r0]
	adds r2, r2, r7
	adds r0, #2
	subs r1, #1
	cmp r1, #0
	bge _0801D8DC
	adds r3, r5, #0
	adds r0, r4, #0
	cmp r0, #7
	ble _0801D8D2
	movs r0, #0xb
	str r0, [r6, #0x10]
	movs r0, #2
	str r0, [r6, #0xc]
	movs r0, #8
	str r0, [r6, #0x14]
	movs r1, #0
	str r1, [r6, #0x30]
	str r1, [r6, #0x2c]
	str r1, [r6, #0x28]
	movs r0, #0x80
	lsls r0, r0, #6
	str r0, [r6, #0x38]
	str r0, [r6, #0x3c]
	mov r2, sl
	mov r0, r8
	strh r2, [r0]
	ldr r0, [r6, #0x24]
	mov r3, ip
	strh r0, [r3]
	mov r4, sb
	strh r1, [r4]
	adds r5, r6, #0
	adds r5, #0x5c
	ldr r4, [sp, #4]
	ldr r7, _0801DA2C @ =gStaticData_0816C5F0
	movs r0, #1
	mov sb, r0
	movs r1, #3
	mov r8, r1
_0801D92C:
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	str r0, [r4]
	ldr r1, _0801DA30 @ =gUnknown_030012D0
	ldr r1, [r1]
	ldr r1, [r1]
	ldr r1, [r1]
	movs r2, #0x96
	lsls r2, r2, #2
	adds r1, r1, r2
	str r1, [r0, #0x20]
	adds r0, #0x28
	movs r3, #4
	rsbs r3, r3, #0
	adds r1, r3, #0
	ldrb r2, [r0]
	ands r1, r2
	mov r3, sb
	orrs r1, r3
	strb r1, [r0]
	ldr r0, [r4]
	ldr r3, [r6, #0x20]
	ldr r1, [r7]
	adds r3, r3, r1
	ldr r2, [r6, #0x24]
	ldr r1, [r7, #4]
	adds r2, r2, r1
	lsls r3, r3, #8
	str r3, [r0]
	lsls r2, r2, #8
	str r2, [r0, #4]
	movs r1, #1
	bl sub_80088D8
	ldr r0, [r6, #0x64]
	bl sub_800815C
	ldr r2, [r4]
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r3, #0x10
	rsbs r3, r3, #0
	adds r1, r3, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	adds r0, r6, #0
	adds r1, r5, #0
	bl sub_801DDB4
	adds r5, #0xc
	adds r4, #0xc
	adds r7, #8
	movs r0, #1
	rsbs r0, r0, #0
	add r8, r0
	mov r1, r8
	cmp r1, #0
	bge _0801D92C
	ldr r0, _0801DA34 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r2, [r6, #0x64]
	ldr r1, [r2, #0x20]
	adds r2, #0x2d
	ldr r3, [r1]
	ldrb r4, [r2]
	lsls r1, r4, #3
	subs r1, r1, r4
	lsls r1, r1, #2
	adds r1, r1, r3
	ldrb r1, [r1, #0x14]
	bl sub_8006D84
	ldr r1, [r6, #0x70]
	adds r1, #0x28
	movs r3, #0x11
	rsbs r3, r3, #0
	adds r0, r3, #0
	ldrb r2, [r1]
	ands r0, r2
	movs r5, #0x10
	orrs r0, r5
	strb r0, [r1]
	ldr r2, [r6, #0x7c]
	adds r2, #0x28
	movs r1, #0x21
	rsbs r1, r1, #0
	adds r0, r1, #0
	ldrb r4, [r2]
	ands r0, r4
	movs r4, #0x20
	orrs r0, r4
	strb r0, [r2]
	ldr r2, [sp, #8]
	ldr r0, [r2]
	adds r0, #0x28
	ldrb r2, [r0]
	ands r3, r2
	orrs r3, r5
	strb r3, [r0]
	ldr r3, [sp, #8]
	ldr r0, [r3]
	adds r0, #0x28
	ldrb r2, [r0]
	ands r1, r2
	orrs r1, r4
	strb r1, [r0]
	adds r0, r6, #0
	add sp, #0xc
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0801DA20: .4byte 0x040000D4
_0801DA24: .4byte 0x81000080
_0801DA28: .4byte 0x00000202
_0801DA2C: .4byte gStaticData_0816C5F0
_0801DA30: .4byte gUnknown_030012D0
_0801DA34: .4byte gUnknown_030012B8

	thumb_func_start sub_801DA38
sub_801DA38: @ 0x0801DA38
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, _0801DAD4 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r2, [r4, #0x64]
	ldr r1, [r2, #0x20]
	adds r2, #0x2d
	ldr r3, [r1]
	ldrb r6, [r2]
	lsls r1, r6, #3
	subs r1, r1, r6
	lsls r1, r1, #2
	adds r1, r1, r3
	ldrb r1, [r1, #0x14]
	bl sub_8006D68
	adds r0, r4, #0
	adds r0, #0x88
	ldr r2, [r0]
	cmp r2, #0
	beq _0801DA76
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0801DA76:
	ldr r2, [r4, #0x7c]
	cmp r2, #0
	beq _0801DA8E
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r6, #0
	ldrsh r0, [r1, r6]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0801DA8E:
	ldr r2, [r4, #0x70]
	cmp r2, #0
	beq _0801DAA6
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0801DAA6:
	ldr r2, [r4, #0x64]
	cmp r2, #0
	beq _0801DABE
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r6, #0
	ldrsh r0, [r1, r6]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0801DABE:
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _0801DACC
	adds r0, r4, #0
	bl sub_8026ED0
_0801DACC:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801DAD4: .4byte gUnknown_030012B8

	thumb_func_start sub_801DAD8
sub_801DAD8: @ 0x0801DAD8
	push {r4, r5, lr}
	ldr r4, _0801DAF4 @ =0xFFFFFE00
	add sp, r4
	adds r5, r0, #0
	ldr r0, [r5, #0xc]
	cmp r0, #4
	bls _0801DAE8
	b _0801DBF2
_0801DAE8:
	lsls r0, r0, #2
	ldr r1, _0801DAF8 @ =_0801DAFC
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801DAF4: .4byte 0xFFFFFE00
_0801DAF8: .4byte _0801DAFC
_0801DAFC: @ jump table
	.4byte _0801DB10 @ case 0
	.4byte _0801DB26 @ case 1
	.4byte _0801DB6C @ case 2
	.4byte _0801DB3A @ case 3
	.4byte _0801DBD0 @ case 4
_0801DB10:
	ldr r0, [r5, #0x14]
	cmp r0, #0xff
	bgt _0801DB1C
	adds r0, #8
	str r0, [r5, #0x14]
	b _0801DBF2
_0801DB1C:
	movs r0, #0x80
	lsls r0, r0, #1
	str r0, [r5, #0x14]
	movs r0, #3
	b _0801DBF0
_0801DB26:
	ldr r0, [r5, #0x14]
	cmp r0, #8
	ble _0801DB32
	subs r0, #8
	str r0, [r5, #0x14]
	b _0801DBF2
_0801DB32:
	movs r0, #8
	str r0, [r5, #0x14]
	movs r0, #2
	b _0801DBF0
_0801DB3A:
	ldr r1, [r5, #0x30]
	adds r1, #1
	movs r2, #0xff
	ands r1, r2
	str r1, [r5, #0x30]
	ldr r3, _0801DB68 @ =gStaticData_0816A820
	adds r0, r1, #0
	ands r0, r2
	lsls r0, r0, #1
	adds r0, r0, r3
	movs r4, #0
	ldrsh r0, [r0, r4]
	asrs r0, r0, #6
	str r0, [r5, #0x28]
	lsls r1, r1, #1
	ands r1, r2
	lsls r1, r1, #1
	adds r1, r1, r3
	movs r2, #0
	ldrsh r0, [r1, r2]
	asrs r0, r0, #6
	str r0, [r5, #0x2c]
	b _0801DBF2
	.align 2, 0
_0801DB68: .4byte gStaticData_0816A820
_0801DB6C:
	ldr r0, [r5, #0x10]
	cmp r0, #0xb
	beq _0801DBF2
	ldr r0, _0801DBC0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x53
	bl PlaySfx
	ldr r4, _0801DBC4 @ =gStaticData_0816C5A0
	ldr r0, [r5, #0x10]
	lsls r0, r0, #3
	adds r0, r0, r4
	ldr r0, [r0]
	mov r1, sp
	bl LoadTaggedAsset
	ldr r1, _0801DBC8 @ =0x040000D4
	mov r3, sp
	str r3, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	str r0, [r1, #4]
	ldr r0, _0801DBCC @ =0x80000020
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r0, [r5, #0x10]
	lsls r0, r0, #3
	adds r4, #4
	adds r0, r0, r4
	ldr r0, [r0]
	ldr r1, [r5, #0x18]
	lsls r1, r1, #0xe
	movs r4, #0xc0
	lsls r4, r4, #0x13
	adds r1, r1, r4
	bl LoadTaggedAsset
	movs r0, #0
	b _0801DBF0
	.align 2, 0
_0801DBC0: .4byte gUnknown_030012BC
_0801DBC4: .4byte gStaticData_0816C5A0
_0801DBC8: .4byte 0x040000D4
_0801DBCC: .4byte 0x80000020
_0801DBD0:
	ldr r0, [r5, #0x14]
	cmp r0, #8
	ble _0801DBEA
	subs r0, #8
	str r0, [r5, #0x14]
	adds r1, r5, #0
	adds r1, #0x48
	ldrh r2, [r1]
	movs r3, #0x80
	lsls r3, r3, #1
	adds r0, r2, r3
	strh r0, [r1]
	b _0801DBF2
_0801DBEA:
	movs r0, #8
	str r0, [r5, #0x14]
	movs r0, #5
_0801DBF0:
	str r0, [r5, #0xc]
_0801DBF2:
	adds r1, r5, #0
	adds r1, #0x5c
	adds r0, r5, #0
	bl sub_801DE04
	adds r1, r5, #0
	adds r1, #0x68
	adds r0, r5, #0
	bl sub_801DE04
	adds r1, r5, #0
	adds r1, #0x74
	adds r0, r5, #0
	bl sub_801DE04
	adds r1, r5, #0
	adds r1, #0x80
	adds r0, r5, #0
	bl sub_801DE04
	movs r3, #0x80
	lsls r3, r3, #2
	add sp, r3
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801DC28
sub_801DC28: @ 0x0801DC28
	push {r4, lr}
	adds r4, r0, #0
	bl sub_801DD18
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801DC5E
	adds r1, r4, #0
	adds r1, #0x5c
	adds r0, r4, #0
	bl sub_801DD90
	adds r1, r4, #0
	adds r1, #0x68
	adds r0, r4, #0
	bl sub_801DD90
	adds r1, r4, #0
	adds r1, #0x74
	adds r0, r4, #0
	bl sub_801DD90
	adds r1, r4, #0
	adds r1, #0x80
	adds r0, r4, #0
	bl sub_801DD90
_0801DC5E:
	ldr r0, [r4, #0xc]
	cmp r0, #0
	blt _0801DCA8
	cmp r0, #3
	bgt _0801DC96
	ldrh r0, [r4, #0x28]
	ldrh r2, [r4, #0x20]
	adds r1, r0, r2
	adds r0, r4, #0
	adds r0, #0x40
	strh r1, [r0]
	ldrh r1, [r4, #0x2c]
	ldrh r2, [r4, #0x24]
	adds r0, r1, r2
	adds r1, r4, #0
	adds r1, #0x42
	strh r0, [r1]
	ldr r1, [r4, #0x14]
	movs r0, #0x80
	lsls r0, r0, #9
	bl sub_803ADB4
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r1, r4, #0
	adds r1, #0x44
	strh r0, [r1]
	b _0801DCA4
_0801DC96:
	cmp r0, #5
	bgt _0801DCA8
	ldr r0, [r4, #0x14]
	adds r1, r4, #0
	adds r1, #0x44
	strh r0, [r1]
	ldr r0, [r4, #0x14]
_0801DCA4:
	adds r1, #2
	strh r0, [r1]
_0801DCA8:
	adds r0, r4, #0
	adds r0, #0x38
	adds r1, r4, #0
	adds r1, #0x4c
	movs r2, #1
	bl sub_803A944
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_801DCBC
sub_801DCBC: @ 0x0801DCBC
	adds r2, r0, #0
	ldr r1, _0801DCF4 @ =0x04000020
	adds r0, #0x4c
	ldrh r0, [r0]
	strh r0, [r1]
	adds r1, #2
	adds r0, r2, #0
	adds r0, #0x4e
	ldrh r0, [r0]
	strh r0, [r1]
	adds r1, #2
	adds r0, r2, #0
	adds r0, #0x50
	ldrh r0, [r0]
	strh r0, [r1]
	adds r1, #2
	adds r0, r2, #0
	adds r0, #0x52
	ldrh r0, [r0]
	strh r0, [r1]
	adds r1, #2
	ldr r0, [r2, #0x54]
	str r0, [r1]
	adds r1, #4
	ldr r0, [r2, #0x58]
	str r0, [r1]
	bx lr
	.align 2, 0
_0801DCF4: .4byte 0x04000020

	thumb_func_start sub_801DCF8
sub_801DCF8: @ 0x0801DCF8
	movs r1, #0
	ldr r0, [r0, #0xc]
	cmp r0, #4
	bne _0801DD02
	movs r1, #1
_0801DD02:
	adds r0, r1, #0
	bx lr
	.align 2, 0

	thumb_func_start sub_801DD08
sub_801DD08: @ 0x0801DD08
	movs r1, #0
	ldr r0, [r0, #0xc]
	cmp r0, #5
	bne _0801DD12
	movs r1, #1
_0801DD12:
	adds r0, r1, #0
	bx lr
	.align 2, 0

	thumb_func_start sub_801DD18
sub_801DD18: @ 0x0801DD18
	movs r1, #0
	ldr r0, [r0, #0xc]
	cmp r0, #3
	bne _0801DD22
	movs r1, #1
_0801DD22:
	adds r0, r1, #0
	bx lr
	.align 2, 0

	thumb_func_start sub_801DD28
sub_801DD28: @ 0x0801DD28
	movs r1, #0
	ldr r0, [r0, #0xc]
	cmp r0, #2
	bne _0801DD32
	movs r1, #1
_0801DD32:
	adds r0, r1, #0
	bx lr
	.align 2, 0

	thumb_func_start sub_801DD38
sub_801DD38: @ 0x0801DD38
	movs r1, #0
	ldr r0, [r0, #0xc]
	cmp r0, #1
	bne _0801DD42
	movs r1, #1
_0801DD42:
	adds r0, r1, #0
	bx lr
	.align 2, 0

	thumb_func_start sub_801DD48
sub_801DD48: @ 0x0801DD48
	adds r2, r0, #0
	adds r2, #0x34
	movs r1, #4
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	strb r1, [r2]
	movs r1, #4
	str r1, [r0, #0xc]
	bx lr

	thumb_func_start sub_801DD5C
sub_801DD5C: @ 0x0801DD5C
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, _0801DD7C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x54
	bl PlaySfx
	movs r0, #1
	str r0, [r4, #0xc]
	movs r0, #0xb
	str r0, [r4, #0x10]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0801DD7C: .4byte gUnknown_030012BC

	thumb_func_start sub_801DD80
sub_801DD80: @ 0x0801DD80
	ldr r2, [r0, #0xc]
	cmp r2, #2
	bgt _0801DD8C
	cmp r2, #1
	blt _0801DD8C
	str r1, [r0, #0x10]
_0801DD8C:
	bx lr
	.align 2, 0

	thumb_func_start sub_801DD90
sub_801DD90: @ 0x0801DD90
	push {lr}
	adds r3, r0, #0
	adds r2, r1, #0
	ldr r0, [r2, #4]
	cmp r0, #0
	ble _0801DDB0
	ldr r0, [r2]
	movs r1, #4
	ands r0, r1
	cmp r0, #0
	beq _0801DDB0
	ldr r0, [r2, #8]
	ldr r1, [r3, #0x28]
	ldr r2, [r3, #0x2c]
	bl sub_8008890
_0801DDB0:
	pop {r0}
	bx r0

	thumb_func_start sub_801DDB4
sub_801DDB4: @ 0x0801DDB4
	push {r4, r5, r6, lr}
	adds r5, r1, #0
	movs r0, #8
	bl sub_8000E1C
	lsls r0, r0, #0x10
	ldr r4, [r5, #8]
	lsrs r3, r0, #0x10
	ldr r0, [r4, #0x20]
	adds r2, r4, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r6, [r2]
	lsls r0, r6, #3
	subs r0, r0, r6
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _0801DDDE
	subs r3, r0, #1
_0801DDDE:
	str r3, [r4, #0x30]
	movs r0, #0x3c
	bl sub_8000E1C
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	str r0, [r5]
	cmp r0, #0
	beq _0801DDFE
	lsrs r0, r0, #1
	adds r0, #1
	bl sub_8000E1C
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	str r0, [r5, #4]
_0801DDFE:
	pop {r4, r5, r6}
	pop {r0}
	bx r0

	thumb_func_start sub_801DE04
sub_801DE04: @ 0x0801DE04
	push {lr}
	adds r2, r0, #0
	ldr r0, [r1]
	cmp r0, #0
	beq _0801DE1A
	subs r0, #1
	str r0, [r1]
	ldr r0, [r1, #4]
	subs r0, #1
	str r0, [r1, #4]
	b _0801DE20
_0801DE1A:
	adds r0, r2, #0
	bl sub_801DDB4
_0801DE20:
	pop {r0}
	bx r0

	thumb_func_start sub_801DE24
sub_801DE24: @ 0x0801DE24
	ldrh r0, [r0, #0x34]
	bx lr

	thumb_func_start sub_801DE28
sub_801DE28: @ 0x0801DE28
	ldrb r0, [r0, #4]
	bx lr

	thumb_func_start sub_801DE2C
sub_801DE2C: @ 0x0801DE2C
	ldr r0, [r0]
	bx lr

	thumb_func_start sub_801DE30
sub_801DE30: @ 0x0801DE30
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	movs r0, #0xff
	ands r1, r0
	cmp r1, #0x9f
	bgt _0801DE40
	rsbs r5, r1, #0
	b _0801DE46
_0801DE40:
	movs r0, #0x80
	lsls r0, r0, #1
	subs r5, r0, r1
_0801DE46:
	ldrb r0, [r4, #4]
	cmp r0, #0
	beq _0801DE58
	ldr r0, [r4, #8]
	adds r2, r5, #2
	movs r1, #0
	bl sub_8008890
	b _0801DE62
_0801DE58:
	ldr r0, [r4, #8]
	movs r1, #0
	adds r2, r5, #0
	bl sub_8008890
_0801DE62:
	ldrb r0, [r4, #4]
	cmp r0, #0
	beq _0801DE6E
	ldr r1, [r4, #0xc]
	movs r4, #1
	b _0801DE72
_0801DE6E:
	ldr r1, [r4, #0xc]
	movs r4, #0
_0801DE72:
	ldr r0, [r1, #0x20]
	adds r3, r1, #0
	adds r3, #0x2d
	ldr r2, [r0]
	ldrb r6, [r3]
	lsls r0, r6, #3
	subs r0, r0, r6
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r2, [r0, #0x16]
	adds r0, r1, #0
	cmp r4, r2
	blt _0801DE8E
	subs r4, r2, #1
_0801DE8E:
	str r4, [r0, #0x30]
	movs r1, #0
	adds r2, r5, #0
	bl sub_8008890
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801DEA0
sub_801DEA0: @ 0x0801DEA0
	strb r1, [r0, #4]
	bx lr

	thumb_func_start sub_801DEA4
sub_801DEA4: @ 0x0801DEA4
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r3, r1, #0
	cmp r2, #4
	bgt _0801DED8
	lsls r0, r3, #2
	adds r0, r0, r3
	adds r0, r0, r2
	str r0, [r4]
	ldr r4, [r4, #8]
	adds r2, r0, #0
	ldr r0, [r4, #0x20]
	adds r3, r4, #0
	adds r3, #0x2d
	ldr r1, [r0]
	ldrb r5, [r3]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r2, r0
	blt _0801DED4
	subs r2, r0, #1
_0801DED4:
	str r2, [r4, #0x30]
	b _0801DF02
_0801DED8:
	adds r0, r3, #0
	adds r0, #0x14
	str r0, [r4]
	ldr r1, _0801DF08 @ =gStaticData_0816C624
	lsls r0, r3, #2
	adds r0, r0, r1
	ldr r4, [r4, #8]
	ldr r0, [r0]
	adds r2, r4, #0
	adds r2, #0x2d
	strb r0, [r2]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
_0801DF02:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0801DF08: .4byte gStaticData_0816C624

	thumb_func_start sub_801DF0C
sub_801DF0C: @ 0x0801DF0C
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	ldr r0, _0801DF6C @ =gStaticData_0816C610
	lsls r1, r1, #2
	adds r1, r1, r0
	ldr r4, [r5, #0xc]
	ldr r0, [r1]
	adds r1, r4, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r5, #0xc]
	bl sub_800815C
	ldr r2, [r5, #0xc]
	adds r2, #0x29
	movs r6, #0xf
	ands r0, r6
	movs r4, #0x10
	rsbs r4, r4, #0
	adds r1, r4, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldr r0, [r5, #8]
	bl sub_800815C
	ldr r1, [r5, #8]
	adds r1, #0x29
	ands r0, r6
	ldrb r2, [r1]
	ands r4, r2
	orrs r4, r0
	strb r4, [r1]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801DF6C: .4byte gStaticData_0816C610

	thumb_func_start sub_801DF70
sub_801DF70: @ 0x0801DF70
	push {r4, lr}
	ldr r4, [r0, #8]
	ldr r3, [r1]
	ldr r2, [r1, #4]
	subs r2, #3
	lsls r3, r3, #8
	str r3, [r4]
	lsls r2, r2, #8
	str r2, [r4, #4]
	ldr r0, [r0, #0xc]
	ldr r3, [r1]
	ldr r2, [r1, #4]
	adds r1, r3, #0
	bl sub_800737C
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start nullsub_20
nullsub_20: @ 0x0801DF94
	bx lr
	.align 2, 0

	thumb_func_start sub_801DF98
sub_801DF98: @ 0x0801DF98
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, _0801DFE8 @ =gStaticData_087E4BAC
	str r0, [r4, #0x10]
	ldr r2, [r4, #8]
	cmp r2, #0
	beq _0801DFBA
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0801DFBA:
	ldr r2, [r4, #0xc]
	cmp r2, #0
	beq _0801DFD2
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0801DFD2:
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _0801DFE0
	adds r0, r4, #0
	bl sub_8026ED0
_0801DFE0:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0801DFE8: .4byte gStaticData_087E4BAC

	thumb_func_start sub_801DFEC
sub_801DFEC: @ 0x0801DFEC
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r0, _0801E044 @ =gStaticData_087E4BAC
	str r0, [r4, #0x10]
	movs r0, #0
	strb r0, [r4, #4]
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	str r0, [r4, #0xc]
	ldr r5, _0801E048 @ =gUnknown_030012D0
	ldr r1, [r5]
	ldr r1, [r1]
	ldr r1, [r1]
	movs r2, #0x93
	lsls r2, r2, #2
	adds r1, r1, r2
	str r1, [r0, #0x20]
	movs r1, #1
	bl sub_80088D8
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	str r0, [r4, #8]
	ldr r1, [r5]
	ldr r1, [r1]
	ldr r1, [r1]
	movs r2, #0x99
	lsls r2, r2, #2
	adds r1, r1, r2
	str r1, [r0, #0x20]
	movs r1, #1
	bl sub_80088D8
	adds r0, r4, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0801E044: .4byte gStaticData_087E4BAC
_0801E048: .4byte gUnknown_030012D0

	thumb_func_start sub_801E04C
sub_801E04C: @ 0x0801E04C
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	adds r4, r0, #0
	str r4, [r6, #0x28]
	ldr r0, _0801E174 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x90
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x20]
	movs r5, #0
	adds r0, r4, #0
	adds r0, #0x2d
	strb r5, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r6, #0x28]
	bl sub_800815C
	ldr r2, [r6, #0x28]
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldr r0, _0801E178 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r2, [r6, #0x28]
	ldr r1, [r2, #0x20]
	adds r2, #0x2d
	ldr r3, [r1]
	ldrb r4, [r2]
	lsls r1, r4, #3
	subs r1, r1, r4
	lsls r1, r1, #2
	adds r1, r1, r3
	ldrb r1, [r1, #0x14]
	bl sub_8006D84
	adds r0, r6, #0
	movs r1, #0x78
	movs r2, #0x35
	bl sub_801E4F4
	ldrb r1, [r6, #4]
	subs r1, #0x20
	adds r0, r6, #0
	adds r0, #0x34
	strb r1, [r0]
	movs r7, #0x35
	movs r0, #4
	rsbs r0, r0, #0
	ldrb r1, [r7, r6]
	ands r0, r1
	movs r1, #1
	orrs r0, r1
	movs r2, #0xd
	rsbs r2, r2, #0
	ands r0, r2
	movs r4, #0x11
	rsbs r4, r4, #0
	ands r0, r4
	movs r3, #0x21
	rsbs r3, r3, #0
	ands r0, r3
	movs r1, #0x3f
	ands r0, r1
	strb r0, [r7, r6]
	ldr r1, [r6]
	subs r1, #0x20
	ldr r7, _0801E17C @ =0x000001FF
	adds r0, r7, #0
	ands r1, r0
	ldr r0, _0801E180 @ =0xFFFFFE00
	ldrh r7, [r6, #0x36]
	ands r0, r7
	orrs r0, r1
	strh r0, [r6, #0x36]
	movs r0, #0x37
	adds r0, r0, r6
	mov ip, r0
	movs r0, #0xf
	rsbs r0, r0, #0
	mov r1, ip
	ldrb r1, [r1]
	ands r0, r1
	ands r0, r4
	ands r0, r3
	movs r1, #0xc0
	orrs r0, r1
	mov r3, ip
	strb r0, [r3]
	ldr r0, _0801E184 @ =0xFFFFFC00
	ldrh r4, [r6, #0x38]
	ands r0, r4
	movs r7, #0xf0
	lsls r7, r7, #2
	adds r1, r7, #0
	orrs r0, r1
	strh r0, [r6, #0x38]
	adds r3, r6, #0
	adds r3, #0x39
	ldrb r0, [r3]
	ands r2, r0
	strb r2, [r3]
	ldr r0, [r6, #0x28]
	adds r0, #0x29
	ldrb r0, [r0]
	lsls r0, r0, #0x1c
	lsrs r0, r0, #0x18
	movs r1, #0xf
	ands r2, r1
	orrs r2, r0
	strb r2, [r3]
	ldr r0, _0801E188 @ =gStaticData_086377C0
	ldr r1, _0801E18C @ =0x06017800
	bl LoadTaggedAsset
	adds r0, r6, #0
	adds r0, #0x48
	strh r5, [r0]
	movs r0, #8
	str r0, [r6, #0x40]
	movs r0, #4
	str r0, [r6, #0x30]
	adds r0, r6, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0801E174: .4byte gUnknown_030012D0
_0801E178: .4byte gUnknown_030012B8
_0801E17C: .4byte 0x000001FF
_0801E180: .4byte 0xFFFFFE00
_0801E184: .4byte 0xFFFFFC00
_0801E188: .4byte gStaticData_086377C0
_0801E18C: .4byte 0x06017800

	thumb_func_start sub_801E190
sub_801E190: @ 0x0801E190
	push {r4, r5, lr}
	adds r4, r0, #0
	bl sub_801E43C
	ldr r0, [r4, #0x30]
	cmp r0, #5
	bls _0801E1A0
	b _0801E2B6
_0801E1A0:
	lsls r0, r0, #2
	ldr r1, _0801E1AC @ =_0801E1B0
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801E1AC: .4byte _0801E1B0
_0801E1B0: @ jump table
	.4byte _0801E1C8 @ case 0
	.4byte _0801E1F0 @ case 1
	.4byte _0801E210 @ case 2
	.4byte _0801E24C @ case 3
	.4byte _0801E288 @ case 4
	.4byte _0801E2A6 @ case 5
_0801E1C8:
	ldr r0, [r4, #0x28]
	bl sub_8008044
	ldr r0, [r4, #0x2c]
	cmp r0, #0
	beq _0801E1DA
	subs r0, #1
	str r0, [r4, #0x2c]
	b _0801E2B6
_0801E1DA:
	ldr r5, [r4, #0x28]
	ldr r0, [r5, #0x30]
	cmp r0, #0
	bne _0801E2B6
	movs r0, #1
	str r0, [r4, #0x30]
	ldr r0, _0801E1EC @ =gStaticData_0816C634
	ldr r0, [r0, #4]
	b _0801E22A
	.align 2, 0
_0801E1EC: .4byte gStaticData_0816C634
_0801E1F0:
	ldr r0, [r4, #0x28]
	bl sub_8008044
	ldr r5, [r4, #0x28]
	adds r0, r5, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801E2B6
	movs r0, #2
	str r0, [r4, #0x30]
	ldr r0, _0801E20C @ =gStaticData_0816C634
	ldr r0, [r0, #8]
	b _0801E22A
	.align 2, 0
_0801E20C: .4byte gStaticData_0816C634
_0801E210:
	ldr r0, [r4, #0x28]
	bl sub_8008044
	ldr r5, [r4, #0x28]
	adds r0, r5, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801E2B6
	movs r0, #3
	str r0, [r4, #0x30]
	ldr r0, _0801E248 @ =gStaticData_0816C634
	ldr r0, [r0, #0xc]
_0801E22A:
	adds r1, r5, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r5, #0
	bl sub_80087C0
	adds r0, r5, #0
	bl sub_80087B4
	adds r0, r5, #0
	movs r1, #0
	bl sub_800872C
	b _0801E2B6
	.align 2, 0
_0801E248: .4byte gStaticData_0816C634
_0801E24C:
	ldr r0, [r4, #0x28]
	bl sub_8008044
	ldr r5, [r4, #0x28]
	adds r0, r5, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801E2B6
	movs r0, #0
	str r0, [r4, #0x30]
	ldr r0, _0801E284 @ =gStaticData_0816C634
	ldr r0, [r0]
	adds r1, r5, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r5, #0
	bl sub_80087C0
	adds r0, r5, #0
	bl sub_80087B4
	adds r0, r5, #0
	movs r1, #0
	bl sub_800872C
	b _0801E29E
	.align 2, 0
_0801E284: .4byte gStaticData_0816C634
_0801E288:
	ldr r1, [r4, #0x40]
	cmp r1, #0xff
	bgt _0801E294
	ldr r0, [r4, #0x3c]
	adds r0, r1, r0
	b _0801E2B4
_0801E294:
	movs r0, #0x80
	lsls r0, r0, #1
	str r0, [r4, #0x40]
	movs r0, #0
	str r0, [r4, #0x30]
_0801E29E:
	adds r0, r4, #0
	bl sub_801E504
	b _0801E2B6
_0801E2A6:
	ldr r1, [r4, #0x40]
	cmp r1, #8
	ble _0801E2B2
	ldr r0, [r4, #0x3c]
	subs r0, r1, r0
	b _0801E2B4
_0801E2B2:
	movs r0, #8
_0801E2B4:
	str r0, [r4, #0x40]
_0801E2B6:
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_801E2BC
sub_801E2BC: @ 0x0801E2BC
	push {r4, r5, r6, r7, lr}
	adds r7, r0, #0
	ldr r0, [r7, #0x28]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	bl sub_800737C
	ldr r0, [r7, #0x30]
	cmp r0, #5
	bgt _0801E394
	cmp r0, #4
	blt _0801E394
	ldr r0, [r7, #0x40]
	cmp r0, #8
	ble _0801E39E
	ldr r1, [r7]
	subs r1, #0x20
	ldr r2, _0801E388 @ =0x000001FF
	adds r0, r2, #0
	ands r1, r0
	ldr r0, _0801E38C @ =0xFFFFFE00
	ldrh r3, [r7, #0x36]
	ands r0, r3
	orrs r0, r1
	strh r0, [r7, #0x36]
	ldrb r0, [r7, #4]
	subs r0, #0x20
	adds r6, r7, #0
	adds r6, #0x34
	strb r0, [r6]
	ldr r5, _0801E390 @ =gUnknown_03001300
	ldr r1, [r5]
	ldr r0, [r1, #8]
	adds r4, r0, #0
	adds r0, #1
	str r0, [r1, #8]
	movs r0, #0x37
	adds r0, r0, r7
	mov ip, r0
	movs r0, #7
	adds r1, r4, #0
	ands r1, r0
	lsls r1, r1, #1
	movs r0, #0xf
	rsbs r0, r0, #0
	mov r2, ip
	ldrb r2, [r2]
	ands r0, r2
	orrs r0, r1
	asrs r1, r4, #3
	movs r3, #1
	ands r1, r3
	lsls r1, r1, #4
	movs r2, #0x11
	rsbs r2, r2, #0
	ands r0, r2
	orrs r0, r1
	asrs r1, r4, #4
	ands r1, r3
	lsls r1, r1, #5
	subs r2, #0x10
	ands r0, r2
	orrs r0, r1
	mov r3, ip
	strb r0, [r3]
	adds r0, r7, #0
	bl sub_801E3A4
	adds r0, r7, #0
	adds r0, #0x4c
	ldrh r1, [r0]
	ldr r0, [r5]
	lsls r2, r4, #2
	lsls r4, r4, #5
	adds r4, r0, r4
	strh r1, [r4, #0x12]
	adds r1, r7, #0
	adds r1, #0x4e
	ldrh r3, [r1]
	adds r1, r2, #1
	lsls r1, r1, #3
	adds r1, r0, r1
	strh r3, [r1, #0x12]
	adds r1, r7, #0
	adds r1, #0x50
	ldrh r3, [r1]
	adds r1, r2, #2
	lsls r1, r1, #3
	adds r1, r0, r1
	strh r3, [r1, #0x12]
	adds r1, r7, #0
	adds r1, #0x52
	ldrh r1, [r1]
	adds r2, #3
	lsls r2, r2, #3
	adds r2, r0, r2
	strh r1, [r2, #0x12]
	adds r1, r6, #0
	bl sub_8006AC8
	b _0801E39E
	.align 2, 0
_0801E388: .4byte 0x000001FF
_0801E38C: .4byte 0xFFFFFE00
_0801E390: .4byte gUnknown_03001300
_0801E394:
	ldr r0, [r7, #0x28]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
_0801E39E:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_801E3A4
sub_801E3A4: @ 0x0801E3A4
	push {r4, lr}
	adds r4, r0, #0
	ldr r1, [r4, #0x40]
	movs r0, #0x80
	lsls r0, r0, #9
	bl sub_803ADB4
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r2, r4, #0
	adds r2, #0x44
	strh r0, [r2]
	adds r1, r4, #0
	adds r1, #0x46
	strh r0, [r1]
	adds r1, #6
	adds r0, r2, #0
	movs r2, #1
	movs r3, #2
	bl sub_803A954
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_801E3D4
sub_801E3D4: @ 0x0801E3D4
	movs r1, #0
	ldr r0, [r0, #0x30]
	cmp r0, #5
	bne _0801E3DE
	movs r1, #1
_0801E3DE:
	adds r0, r1, #0
	bx lr
	.align 2, 0

	thumb_func_start sub_801E3E4
sub_801E3E4: @ 0x0801E3E4
	movs r1, #0
	ldr r0, [r0, #0x30]
	cmp r0, #4
	bne _0801E3EE
	movs r1, #1
_0801E3EE:
	adds r0, r1, #0
	bx lr
	.align 2, 0

	thumb_func_start sub_801E3F4
sub_801E3F4: @ 0x0801E3F4
	push {lr}
	movs r1, #5
	str r1, [r0, #0x30]
	adds r1, #0xfb
	str r1, [r0, #0x40]
	bl sub_801E190
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801E408
sub_801E408: @ 0x0801E408
	push {r4, lr}
	sub sp, #8
	adds r4, r0, #0
	ldr r0, [r4, #0x28]
	ldr r0, [r0]
	asrs r0, r0, #8
	cmp r0, #0x78
	bgt _0801E41C
	movs r0, #0x14
	b _0801E41E
_0801E41C:
	movs r0, #0xdc
_0801E41E:
	str r0, [sp]
	movs r0, #0x88
	str r0, [sp, #4]
	ldr r1, [sp]
	adds r0, r4, #0
	movs r2, #0x88
	bl sub_801E480
	movs r0, #0
	str r0, [r4, #0x2c]
	add sp, #8
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801E43C
sub_801E43C: @ 0x0801E43C
	push {r4, r5, lr}
	adds r4, r0, #0
	movs r5, #1
_0801E442:
	ldr r1, [r4]
	ldr r0, [r4, #8]
	cmp r1, r0
	bne _0801E452
	ldr r1, [r4, #4]
	ldr r0, [r4, #0xc]
	cmp r1, r0
	beq _0801E458
_0801E452:
	adds r0, r4, #0
	bl StepBresenhamLine
_0801E458:
	subs r5, #1
	cmp r5, #0
	bge _0801E442
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_801E464
sub_801E464: @ 0x0801E464
	adds r2, r0, #0
	movs r3, #0
	ldr r1, [r2]
	ldr r0, [r2, #8]
	cmp r1, r0
	bne _0801E47A
	ldr r1, [r2, #4]
	ldr r0, [r2, #0xc]
	cmp r1, r0
	bne _0801E47A
	movs r3, #1
_0801E47A:
	adds r0, r3, #0
	bx lr
	.align 2, 0

	thumb_func_start sub_801E480
sub_801E480: @ 0x0801E480
	push {r4, lr}
	adds r4, r0, #0
	str r1, [r4, #8]
	str r2, [r4, #0xc]
	ldr r0, [r4]
	cmp r0, r1
	bne _0801E494
	ldr r0, [r4, #4]
	cmp r0, r2
	beq _0801E4C4
_0801E494:
	adds r0, r4, #0
	bl InitBresenhamLine
	adds r0, r4, #0
	adds r0, #0x24
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801E4AA
	ldr r0, [r4, #8]
	ldr r1, [r4]
	b _0801E4AE
_0801E4AA:
	ldr r0, [r4, #0xc]
	ldr r1, [r4, #4]
_0801E4AE:
	subs r0, r0, r1
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r1, r0, #1
	cmp r1, #0
	bge _0801E4BC
	rsbs r1, r1, #0
_0801E4BC:
	movs r0, #0xf8
	bl sub_803ADB4
	str r0, [r4, #0x3c]
_0801E4C4:
	ldr r0, [r4, #0x30]
	cmp r0, #0
	bne _0801E4DE
	movs r0, #0x96
	lsls r0, r0, #1
	bl sub_8000E1C
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r1, #0x96
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x2c]
_0801E4DE:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_801E4E4
sub_801E4E4: @ 0x0801E4E4
	push {lr}
	ldr r3, [r1]
	ldr r2, [r1, #4]
	adds r1, r3, #0
	bl sub_801E480
	pop {r0}
	bx r0

	thumb_func_start sub_801E4F4
sub_801E4F4: @ 0x0801E4F4
	push {lr}
	str r1, [r0]
	str r2, [r0, #4]
	ldr r0, [r0, #0x28]
	bl sub_800737C
	pop {r0}
	bx r0

	thumb_func_start sub_801E504
sub_801E504: @ 0x0801E504
	push {r4, lr}
	adds r4, r0, #0
	movs r0, #0x96
	lsls r0, r0, #1
	bl sub_8000E1C
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r1, #0x96
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x2c]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801E524
sub_801E524: @ 0x0801E524
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, _0801E574 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r2, [r4, #0x28]
	ldr r1, [r2, #0x20]
	adds r2, #0x2d
	ldr r3, [r1]
	ldrb r6, [r2]
	lsls r1, r6, #3
	subs r1, r1, r6
	lsls r1, r1, #2
	adds r1, r1, r3
	ldrb r1, [r1, #0x14]
	bl sub_8006D68
	ldr r2, [r4, #0x28]
	cmp r2, #0
	beq _0801E55E
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0801E55E:
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _0801E56C
	adds r0, r4, #0
	bl sub_8026ED0
_0801E56C:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801E574: .4byte gUnknown_030012B8
