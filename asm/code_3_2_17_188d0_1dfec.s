.include "asm/macros.inc"

.syntax unified
.arm

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
