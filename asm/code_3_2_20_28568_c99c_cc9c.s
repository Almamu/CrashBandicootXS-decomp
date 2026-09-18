.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_802CC9C
sub_802CC9C: @ 0x0802CC9C
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r1, [r4, #0x34]
	ldr r0, _0802CD38 @ =0x000015FF
	cmp r1, r0
	ble _0802CCB0
	adds r1, r4, #0
	adds r1, #0x2c
	movs r0, #1
	strb r0, [r1]
_0802CCB0:
	ldr r7, [r4, #0xc]
	cmp r7, #0
	beq _0802CCB8
	b _0802CDBC
_0802CCB8:
	ldr r0, [r4, #0x30]
	adds r1, r4, #0
	adds r1, #0x38
	adds r0, #0x14
	ldm r0!, {r2, r3, r5}
	stm r1!, {r2, r3, r5}
	adds r0, r4, #0
	bl sub_802DD9C
	lsls r0, r0, #0x18
	adds r6, r4, #0
	adds r6, #0x38
	cmp r0, #0
	beq _0802CCF2
	ldr r0, _0802CD3C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	movs r0, #1
	str r0, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r7, [r4, #8]
_0802CCF2:
	adds r0, r6, #0
	ldr r1, _0802CD40 @ =gStaticData_0817A78C
	ldm r1!, {r2, r3, r5}
	stm r0!, {r2, r3, r5}
	adds r0, r4, #0
	bl sub_802A6EC
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	cmp r5, #0
	beq _0802CD48
	ldr r0, _0802CD44 @ =gUnknown_03000884
	ldr r0, [r0]
	bl sub_802B7E0
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802CDD8
	ldr r0, _0802CD3C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	movs r0, #1
	str r0, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r7, [r4, #8]
	b _0802CDD8
	.align 2, 0
_0802CD38: .4byte 0x000015FF
_0802CD3C: .4byte gUnknown_030012BC
_0802CD40: .4byte gStaticData_0817A78C
_0802CD44: .4byte gUnknown_03000884
_0802CD48:
	adds r0, r6, #0
	ldr r1, _0802CDB0 @ =gStaticData_0817A774
	ldm r1!, {r2, r3, r7}
	stm r0!, {r2, r3, r7}
	adds r0, r4, #0
	bl sub_802A6EC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802CD7A
	ldr r0, _0802CDB4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	movs r0, #1
	str r0, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
_0802CD7A:
	adds r0, r6, #0
	ldr r1, _0802CDB8 @ =gStaticData_0817A780
	ldm r1!, {r2, r6, r7}
	stm r0!, {r2, r6, r7}
	adds r0, r4, #0
	bl sub_802A6EC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802CDD8
	ldr r0, _0802CDB4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	movs r0, #1
	str r0, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
	b _0802CDD8
	.align 2, 0
_0802CDB0: .4byte gStaticData_0817A774
_0802CDB4: .4byte gUnknown_030012BC
_0802CDB8: .4byte gStaticData_0817A780
_0802CDBC:
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _0802CDD8
	cmp r4, #0
	beq _0802CDDE
	ldr r1, [r4, #0x50]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
	b _0802CDDE
_0802CDD8:
	adds r0, r4, #0
	bl sub_802A7B8
_0802CDDE:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_802CDE4
sub_802CDE4: @ 0x0802CDE4
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, [sp, #0xc]
	str r0, [sp]
	adds r0, r4, #0
	bl InitActorPart
	ldr r0, _0802CE0C @ =gStaticData_087E4FB4
	str r0, [r4, #0x50]
	adds r1, r4, #0
	adds r1, #0x2c
	movs r0, #0
	strb r0, [r1]
	adds r0, r4, #0
	add sp, #4
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0802CE0C: .4byte gStaticData_087E4FB4

	thumb_func_start sub_802CE10
sub_802CE10: @ 0x0802CE10
	push {r4, lr}
	adds r4, r0, #0
	bl sub_802A6EC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802CE26
	ldr r0, _0802CE34 @ =gUnknown_03000884
	ldr r0, [r0]
	bl sub_802B730
_0802CE26:
	adds r0, r4, #0
	bl sub_802A7B8
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0802CE34: .4byte gUnknown_03000884

	thumb_func_start sub_802CE38
sub_802CE38: @ 0x0802CE38
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, [sp, #0xc]
	str r0, [sp]
	adds r0, r4, #0
	bl InitActorPart
	ldr r0, _0802CE58 @ =gStaticData_087E4FD4
	str r0, [r4, #0x50]
	adds r0, r4, #0
	add sp, #4
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0802CE58: .4byte gStaticData_087E4FD4

	thumb_func_start sub_802CE5C
sub_802CE5C: @ 0x0802CE5C
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	ldr r5, [r4, #0x28]
	cmp r5, #0
	beq _0802CE6C
	cmp r5, #1
	beq _0802CEE4
	b _0802CF00
_0802CE6C:
	adds r0, r4, #0
	bl sub_802A6EC
	lsls r0, r0, #0x18
	lsrs r6, r0, #0x18
	cmp r6, #0
	beq _0802CEB0
	ldr r0, _0802CEA8 @ =gUnknown_03000884
	ldr r0, [r0]
	bl sub_802C14C
	ldr r0, _0802CEAC @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	movs r0, #1
	str r0, [r4, #0x28]
	str r5, [r4, #0x44]
	str r0, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
	b _0802CF00
	.align 2, 0
_0802CEA8: .4byte gUnknown_03000884
_0802CEAC: .4byte gUnknown_030012BC
_0802CEB0:
	adds r0, r4, #0
	bl sub_802DD9C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802CF00
	ldr r0, _0802CEE0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	movs r0, #1
	str r0, [r4, #0x28]
	str r6, [r4, #0x44]
	str r0, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r6, [r4, #8]
	b _0802CF00
	.align 2, 0
_0802CEE0: .4byte gUnknown_030012BC
_0802CEE4:
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _0802CF00
	cmp r4, #0
	beq _0802CF06
	ldr r1, [r4, #0x50]
	movs r2, #8
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
	b _0802CF06
_0802CF00:
	adds r0, r4, #0
	bl sub_802A7B8
_0802CF06:
	pop {r4, r5, r6}
	pop {r0}
	bx r0

	thumb_func_start sub_802CF0C
sub_802CF0C: @ 0x0802CF0C
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, [sp, #0xc]
	str r0, [sp]
	adds r0, r4, #0
	bl InitActorPart
	ldr r0, _0802CF2C @ =gStaticData_087E4FF4
	str r0, [r4, #0x50]
	adds r0, r4, #0
	add sp, #4
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0802CF2C: .4byte gStaticData_087E4FF4

	thumb_func_start sub_802CF30
sub_802CF30: @ 0x0802CF30
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x1c]
	ldr r1, [r4, #0x54]
	adds r0, r0, r1
	str r0, [r4, #0x1c]
	ldr r0, [r4, #0x20]
	ldr r1, [r4, #0x58]
	adds r0, r0, r1
	str r0, [r4, #0x20]
	ldr r0, [r4, #0x24]
	ldr r1, [r4, #0x5c]
	adds r0, r0, r1
	str r0, [r4, #0x24]
	ldr r6, [r4, #0x28]
	cmp r6, #0
	bne _0802D02E
	ldr r0, [r4, #0x60]
	subs r0, #1
	str r0, [r4, #0x60]
	cmp r0, #0
	bgt _0802CF64
	ldr r1, [r4, #0x64]
	adds r0, r4, #0
	bl sub_802D044
_0802CF64:
	adds r0, r4, #0
	bl sub_802A6EC
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	cmp r5, #0
	beq _0802CFD8
	ldr r0, _0802CFCC @ =gUnknown_03000884
	ldr r0, [r0]
	bl sub_802B730
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802D02E
	ldr r0, [r4, #0x1c]
	ldr r1, _0802CFD0 @ =0xFFFFFA00
	cmp r0, #0
	ble _0802CF8C
	movs r1, #0xc0
	lsls r1, r1, #3
_0802CF8C:
	str r1, [r4, #0x54]
	movs r0, #0xc0
	lsls r0, r0, #2
	bl sub_8000E1C
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	rsbs r0, r0, #0
	str r0, [r4, #0x58]
	ldr r0, [r4, #0x5c]
	movs r1, #0x80
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x5c]
	ldr r0, _0802CFD4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #5
	bl PlaySfx
	movs r0, #1
	str r0, [r4, #0x28]
	str r6, [r4, #0x44]
	str r6, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r6, [r4, #8]
	b _0802D02E
	.align 2, 0
_0802CFCC: .4byte gUnknown_03000884
_0802CFD0: .4byte 0xFFFFFA00
_0802CFD4: .4byte gUnknown_030012BC
_0802CFD8:
	adds r0, r4, #0
	bl sub_802DD9C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802D02E
	ldr r0, [r4, #0x1c]
	ldr r1, _0802D03C @ =0xFFFFFA00
	cmp r0, #0
	ble _0802CFF0
	movs r1, #0xc0
	lsls r1, r1, #3
_0802CFF0:
	str r1, [r4, #0x54]
	movs r0, #0xc0
	lsls r0, r0, #2
	bl sub_8000E1C
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	rsbs r0, r0, #0
	str r0, [r4, #0x58]
	ldr r0, [r4, #0x5c]
	movs r1, #0x80
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x5c]
	ldr r0, _0802D040 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #5
	bl PlaySfx
	movs r0, #1
	str r0, [r4, #0x28]
	str r5, [r4, #0x44]
	str r5, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
_0802D02E:
	adds r0, r4, #0
	bl sub_802A7B8
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802D03C: .4byte 0xFFFFFA00
_0802D040: .4byte gUnknown_030012BC

	thumb_func_start sub_802D044
sub_802D044: @ 0x0802D044
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	cmp r6, #0
	bge _0802D060
	movs r0, #0
	str r0, [r5, #0x58]
	str r0, [r5, #0x54]
	movs r0, #0x62
	str r0, [r5, #0x5c]
	movs r0, #0x80
	lsls r0, r0, #0x17
	str r0, [r5, #0x60]
	b _0802D0BE
_0802D060:
	adds r0, r6, #0
	bl sub_802A570
	ldr r1, _0802D0C4 @ =gUnknown_0300088C
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	str r0, [r5, #0x5c]
	adds r0, r6, #0
	bl sub_802A51C
	ldr r1, [r5, #0x24]
	subs r0, r0, r1
	ldr r1, [r5, #0x5c]
	bl sub_803ADB4
	str r0, [r5, #0x60]
	cmp r0, #0
	bne _0802D08A
	movs r0, #1
	str r0, [r5, #0x60]
_0802D08A:
	ldr r1, [r5, #0x60]
	movs r0, #0x80
	lsls r0, r0, #5
	bl sub_803ADB4
	adds r4, r0, #0
	adds r0, r6, #0
	bl sub_802A558
	ldr r1, [r5, #0x1c]
	subs r0, r0, r1
	muls r0, r4, r0
	asrs r0, r0, #0xc
	str r0, [r5, #0x54]
	adds r0, r6, #0
	bl sub_802A540
	ldr r1, [r5, #0x20]
	subs r0, r0, r1
	muls r0, r4, r0
	asrs r0, r0, #0xc
	str r0, [r5, #0x58]
	adds r0, r6, #0
	bl sub_802A504
	str r0, [r5, #0x64]
_0802D0BE:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802D0C4: .4byte gUnknown_0300088C

	thumb_func_start sub_802D0C8
sub_802D0C8: @ 0x0802D0C8
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, [sp, #0x10]
	ldr r5, [sp, #0x14]
	str r0, [sp]
	adds r0, r4, #0
	bl InitActorPart
	ldr r0, _0802D0F0 @ =gStaticData_087E5014
	str r0, [r4, #0x50]
	ldr r1, [r5, #0x10]
	adds r0, r4, #0
	bl sub_802D044
	adds r0, r4, #0
	add sp, #4
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0802D0F0: .4byte gStaticData_087E5014

	thumb_func_start sub_802D0F4
sub_802D0F4: @ 0x0802D0F4
	push {r4, r5, lr}
	adds r4, r0, #0
	bl sub_802A6EC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802D10A
	ldr r0, _0802D160 @ =gUnknown_03000884
	ldr r0, [r0]
	bl sub_802B730
_0802D10A:
	movs r0, #0xc8
	lsls r0, r0, #7
	ldr r1, [r4, #0x34]
	cmp r1, r0
	ble _0802D11A
	ldr r0, [r4, #0x28]
	cmp r0, #2
	beq _0802D128
_0802D11A:
	movs r0, #0xb4
	lsls r0, r0, #7
	cmp r1, r0
	ble _0802D164
	ldr r0, [r4, #0x28]
	cmp r0, #1
	bne _0802D164
_0802D128:
	ldr r1, [r4, #0xc]
	adds r1, #1
	str r1, [r4, #0xc]
	ldr r2, [r4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	adds r0, r4, #0
	bl GetAnimFrameBaseOffset
	ldr r2, [r4, #0xc]
	ldr r3, [r4]
	lsls r1, r2, #1
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r1, r1, r3
	movs r2, #4
	ldrsh r1, [r1, r2]
	cmp r0, r1
	blt _0802D1A6
	movs r0, #0
	str r0, [r4, #8]
	b _0802D1A6
	.align 2, 0
_0802D160: .4byte gUnknown_03000884
_0802D164:
	movs r0, #0xa0
	lsls r0, r0, #7
	cmp r1, r0
	ble _0802D1AC
	ldr r5, [r4, #0x28]
	cmp r5, #0
	bne _0802D1AC
	ldr r1, [r4, #0xc]
	adds r1, #1
	str r1, [r4, #0xc]
	ldr r2, [r4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	adds r0, r4, #0
	bl GetAnimFrameBaseOffset
	ldr r2, [r4, #0xc]
	ldr r3, [r4]
	lsls r1, r2, #1
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r1, r1, r3
	movs r2, #4
	ldrsh r1, [r1, r2]
	cmp r0, r1
	blt _0802D1A6
	str r5, [r4, #8]
_0802D1A6:
	ldr r0, [r4, #0x28]
	adds r0, #1
	str r0, [r4, #0x28]
_0802D1AC:
	adds r0, r4, #0
	bl sub_802A7B8
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_802D1B8
sub_802D1B8: @ 0x0802D1B8
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r6, r0, #0
	adds r4, r1, #0
	adds r5, r2, #0
	ldr r0, [sp, #0x14]
	str r0, [sp]
	adds r0, r6, #0
	bl InitActorPart
	ldr r0, _0802D200 @ =gStaticData_087E5034
	str r0, [r6, #0x50]
	ldrb r1, [r4]
	cmp r5, #0
	ble _0802D1D8
	adds r1, #1
_0802D1D8:
	lsls r1, r1, #2
	subs r1, #0x40
	str r1, [r6, #0xc]
	ldr r2, [r6]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrh r0, [r0]
	movs r1, #0
	movs r2, #0
	strh r0, [r6, #0x10]
	strb r1, [r6, #0x12]
	str r2, [r6, #8]
	adds r0, r6, #0
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0802D200: .4byte gStaticData_087E5034

	thumb_func_start sub_802D204
sub_802D204: @ 0x0802D204
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	lsls r1, r1, #0x18
	lsrs r7, r1, #0x18
	ldr r0, _0802D224 @ =gUnknown_030012C0
	ldr r0, [r0]
	ldr r5, [r0, #0x78]
	cmp r5, #0
	bne _0802D228
	cmp r7, #0
	bne _0802D228
	adds r0, r4, #0
	adds r0, #0x2c
	strb r5, [r0]
	b _0802D26C
	.align 2, 0
_0802D224: .4byte gUnknown_030012C0
_0802D228:
	subs r0, r5, #1
	lsls r0, r0, #5
	ldr r1, _0802D280 @ =gStaticData_0817A798
	adds r0, r0, r1
	ldr r1, _0802D284 @ =0x050003C0
	movs r2, #0x20
	movs r3, #0x10
	bl QueueVramDmaTransfer
	adds r1, r4, #0
	adds r1, #0x2c
	movs r6, #0
	movs r0, #1
	strb r0, [r1]
	str r6, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	adds r0, r4, #0
	bl GetAnimFrameBaseOffset
	ldr r2, [r4, #0xc]
	ldr r3, [r4]
	lsls r1, r2, #1
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r1, r1, r3
	movs r2, #4
	ldrsh r1, [r1, r2]
	cmp r0, r1
	blt _0802D26C
	str r6, [r4, #8]
_0802D26C:
	cmp r5, #3
	bne _0802D28C
	ldr r1, _0802D288 @ =gUnknown_030014B8
	movs r0, #0xfa
	lsls r0, r0, #1
	str r0, [r1]
	movs r0, #1
	movs r2, #0
	str r0, [r4, #0x28]
	b _0802D2C2
	.align 2, 0
_0802D280: .4byte gStaticData_0817A798
_0802D284: .4byte 0x050003C0
_0802D288: .4byte gUnknown_030014B8
_0802D28C:
	cmp r5, #0
	bne _0802D2B4
	cmp r7, #0
	beq _0802D2B4
	ldr r0, _0802D2B0 @ =gUnknown_030014B8
	str r5, [r0]
	movs r0, #2
	movs r1, #1
	str r0, [r4, #0x28]
	str r5, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
	b _0802D2D2
	.align 2, 0
_0802D2B0: .4byte gUnknown_030014B8
_0802D2B4:
	ldr r0, _0802D2D8 @ =gUnknown_030014B8
	movs r2, #0
	str r2, [r0]
	ldr r0, [r4, #0x28]
	cmp r0, #0
	beq _0802D2D2
	str r2, [r4, #0x28]
_0802D2C2:
	str r2, [r4, #0x44]
	str r2, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
_0802D2D2:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802D2D8: .4byte gUnknown_030014B8

	thumb_func_start sub_802D2DC
sub_802D2DC: @ 0x0802D2DC
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, _0802D300 @ =gUnknown_030014B8
	ldr r1, [r0]
	cmp r1, #0
	beq _0802D336
	movs r0, #4
	ands r1, r0
	cmp r1, #0
	beq _0802D30C
	ldr r0, _0802D304 @ =gStaticData_0817A7D8
	ldr r1, _0802D308 @ =0x050003C0
	movs r2, #0x20
	movs r3, #0x10
	bl QueueVramDmaTransfer
	b _0802D318
	.align 2, 0
_0802D300: .4byte gUnknown_030014B8
_0802D304: .4byte gStaticData_0817A7D8
_0802D308: .4byte 0x050003C0
_0802D30C:
	ldr r0, _0802D398 @ =gStaticData_0817A7B8
	ldr r1, _0802D39C @ =0x050003C0
	movs r2, #0x20
	movs r3, #0x10
	bl QueueVramDmaTransfer
_0802D318:
	ldr r1, _0802D3A0 @ =gUnknown_030014B8
	ldr r0, [r1]
	subs r0, #1
	str r0, [r1]
	cmp r0, #0
	bne _0802D336
	ldr r0, _0802D3A4 @ =gUnknown_030012C0
	ldr r0, [r0]
	movs r1, #2
	bl sub_80231EC
	adds r0, r4, #0
	movs r1, #0
	bl sub_802D204
_0802D336:
	ldr r0, [r4, #0x28]
	cmp r0, #2
	bne _0802D34A
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _0802D34A
	adds r0, r4, #0
	movs r1, #0
	bl sub_802D204
_0802D34A:
	adds r0, r4, #0
	bl sub_802A980
	ldr r0, [r4, #0x44]
	adds r0, #1
	str r0, [r4, #0x44]
	movs r0, #0x10
	ldrsh r1, [r4, r0]
	ldr r0, [r4, #8]
	adds r0, r0, r1
	str r0, [r4, #8]
	movs r0, #0
	strb r0, [r4, #0x12]
	adds r0, r4, #0
	bl GetAnimFrameBaseOffset
	ldr r2, [r4, #0xc]
	ldr r3, [r4]
	lsls r1, r2, #1
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r1, r1, r3
	movs r3, #4
	ldrsh r2, [r1, r3]
	cmp r0, r2
	blt _0802D390
	movs r3, #6
	ldrsh r0, [r1, r3]
	subs r0, r2, r0
	lsls r0, r0, #8
	ldr r1, [r4, #8]
	subs r1, r1, r0
	str r1, [r4, #8]
	movs r0, #1
	strb r0, [r4, #0x12]
_0802D390:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0802D398: .4byte gStaticData_0817A7B8
_0802D39C: .4byte 0x050003C0
_0802D3A0: .4byte gUnknown_030014B8
_0802D3A4: .4byte gUnknown_030012C0

