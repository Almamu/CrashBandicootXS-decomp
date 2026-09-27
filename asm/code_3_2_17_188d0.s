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
