.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_802C99C
sub_802C99C: @ 0x0802C99C
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0xc]
	cmp r0, #0x12
	beq _0802CA1A
	adds r0, r4, #0
	bl sub_802A6EC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802CA1A
	ldr r0, _0802C9DC @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	ldr r5, _0802C9E0 @ =gUnknown_030012C0
	ldr r0, [r5]
	bl sub_8022FEC
	ldr r0, [r4, #0x30]
	ldrb r0, [r0]
	cmp r0, #6
	beq _0802C9F4
	cmp r0, #6
	bgt _0802C9E4
	cmp r0, #5
	beq _0802C9EA
	b _0802CA06
	.align 2, 0
_0802C9DC: .4byte gUnknown_030012BC
_0802C9E0: .4byte gUnknown_030012C0
_0802C9E4:
	cmp r0, #7
	beq _0802C9FE
	b _0802CA06
_0802C9EA:
	ldr r0, [r5]
	movs r1, #1
	bl sub_8022EA8
	b _0802CA06
_0802C9F4:
	ldr r0, [r5]
	movs r1, #2
	bl sub_8022EA8
	b _0802CA06
_0802C9FE:
	ldr r0, [r5]
	movs r1, #3
	bl sub_8022EA8
_0802CA06:
	movs r0, #0x12
	str r0, [r4, #0xc]
	ldr r0, [r4]
	adds r0, #0xd8
	ldrh r0, [r0]
	movs r1, #0
	movs r2, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
_0802CA1A:
	adds r0, r4, #0
	bl sub_802C4C8
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_802CA28
sub_802CA28: @ 0x0802CA28
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0xc]
	cmp r0, #0x12
	beq _0802CA5E
	ldr r0, _0802CA64 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	ldr r0, _0802CA68 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022FEC
	movs r2, #0
	str r2, [r4, #0x44]
	movs r0, #0x12
	str r0, [r4, #0xc]
	ldr r0, [r4]
	adds r0, #0xd8
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
_0802CA5E:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0802CA64: .4byte gUnknown_030012BC
_0802CA68: .4byte gUnknown_030012C0

	thumb_func_start sub_802CA6C
sub_802CA6C: @ 0x0802CA6C
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0xc]
	cmp r0, #0x12
	beq _0802CAB6
	adds r0, r4, #0
	bl sub_802A6EC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802CAB6
	ldr r0, _0802CAC4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	ldr r0, _0802CAC8 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022FEC
	ldr r0, _0802CACC @ =gUnknown_03000884
	ldr r0, [r0]
	movs r1, #4
	bl sub_802C078
	movs r0, #0x12
	str r0, [r4, #0xc]
	ldr r0, [r4]
	adds r0, #0xd8
	ldrh r0, [r0]
	movs r1, #0
	movs r2, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
_0802CAB6:
	adds r0, r4, #0
	bl sub_802C4C8
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0802CAC4: .4byte gUnknown_030012BC
_0802CAC8: .4byte gUnknown_030012C0
_0802CACC: .4byte gUnknown_03000884

	thumb_func_start sub_802CAD0
sub_802CAD0: @ 0x0802CAD0
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0xc]
	cmp r0, #0x12
	beq _0802CB1A
	adds r0, r4, #0
	bl sub_802A6EC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802CB1A
	ldr r0, _0802CB28 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	ldr r0, _0802CB2C @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022FEC
	ldr r0, _0802CB30 @ =gUnknown_03000884
	ldr r0, [r0]
	movs r1, #1
	bl sub_802C078
	movs r0, #0x12
	str r0, [r4, #0xc]
	ldr r0, [r4]
	adds r0, #0xd8
	ldrh r0, [r0]
	movs r1, #0
	movs r2, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
_0802CB1A:
	adds r0, r4, #0
	bl sub_802C4C8
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0802CB28: .4byte gUnknown_030012BC
_0802CB2C: .4byte gUnknown_030012C0
_0802CB30: .4byte gUnknown_03000884

	thumb_func_start sub_802CB34
sub_802CB34: @ 0x0802CB34
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r4, r2, #0
	adds r6, r3, #0
	ldr r0, [sp, #0x14]
	str r0, [sp]
	adds r0, r5, #0
	bl InitActorPart
	ldr r0, _0802CB98 @ =gStaticData_087E4F94
	str r0, [r5, #0x50]
	asrs r4, r4, #8
	adds r4, #0x3c
	adds r0, r4, #0
	movs r1, #0x14
	bl sub_803ADB4
	adds r2, r0, #0
	cmp r2, #0
	bge _0802CB60
	movs r2, #0
_0802CB60:
	cmp r2, #5
	ble _0802CB66
	movs r2, #5
_0802CB66:
	asrs r3, r6, #8
	cmp r3, #0x2b
	bgt _0802CB6E
	adds r2, #6
_0802CB6E:
	cmp r3, #6
	bgt _0802CB74
	adds r2, #6
_0802CB74:
	str r2, [r5, #0xc]
	ldr r1, [r5]
	lsls r0, r2, #1
	adds r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrh r0, [r0]
	movs r1, #0
	movs r2, #0
	strh r0, [r5, #0x10]
	strb r1, [r5, #0x12]
	str r2, [r5, #8]
	adds r0, r5, #0
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0802CB98: .4byte gStaticData_087E4F94

	thumb_func_start sub_802CB9C
sub_802CB9C: @ 0x0802CB9C
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, [sp, #0xc]
	str r0, [sp]
	adds r0, r4, #0
	bl sub_802CB34
	ldr r0, _0802CBBC @ =gStaticData_087E4EB4
	str r0, [r4, #0x50]
	adds r0, r4, #0
	add sp, #4
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0802CBBC: .4byte gStaticData_087E4EB4

	thumb_func_start sub_802CBC0
sub_802CBC0: @ 0x0802CBC0
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, [sp, #0xc]
	str r0, [sp]
	adds r0, r4, #0
	bl sub_802CB34
	ldr r0, _0802CBE0 @ =gStaticData_087E4ED4
	str r0, [r4, #0x50]
	adds r0, r4, #0
	add sp, #4
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0802CBE0: .4byte gStaticData_087E4ED4

	thumb_func_start sub_802CBE4
sub_802CBE4: @ 0x0802CBE4
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, [sp, #0xc]
	str r0, [sp]
	adds r0, r4, #0
	bl sub_802CB34
	ldr r0, _0802CC04 @ =gStaticData_087E4EF4
	str r0, [r4, #0x50]
	adds r0, r4, #0
	add sp, #4
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0802CC04: .4byte gStaticData_087E4EF4

	thumb_func_start sub_802CC08
sub_802CC08: @ 0x0802CC08
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, [sp, #0xc]
	str r0, [sp]
	adds r0, r4, #0
	bl sub_802CB34
	ldr r0, _0802CC28 @ =gStaticData_087E4F14
	str r0, [r4, #0x50]
	adds r0, r4, #0
	add sp, #4
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0802CC28: .4byte gStaticData_087E4F14

	thumb_func_start sub_802CC2C
sub_802CC2C: @ 0x0802CC2C
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, [sp, #0x10]
	ldr r5, [sp, #0x14]
	str r0, [sp]
	adds r0, r4, #0
	bl sub_802CB34
	ldr r0, _0802CC50 @ =gStaticData_087E4F34
	str r0, [r4, #0x50]
	str r5, [r4, #0x54]
	adds r0, r4, #0
	add sp, #4
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0802CC50: .4byte gStaticData_087E4F34

	thumb_func_start sub_802CC54
sub_802CC54: @ 0x0802CC54
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, [sp, #0xc]
	str r0, [sp]
	adds r0, r4, #0
	bl sub_802CB34
	ldr r0, _0802CC74 @ =gStaticData_087E4F54
	str r0, [r4, #0x50]
	adds r0, r4, #0
	add sp, #4
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0802CC74: .4byte gStaticData_087E4F54

	thumb_func_start sub_802CC78
sub_802CC78: @ 0x0802CC78
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, [sp, #0xc]
	str r0, [sp]
	adds r0, r4, #0
	bl sub_802CB34
	ldr r0, _0802CC98 @ =gStaticData_087E4F74
	str r0, [r4, #0x50]
	adds r0, r4, #0
	add sp, #4
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0802CC98: .4byte gStaticData_087E4F74

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

	thumb_func_start sub_802D3A8
sub_802D3A8: @ 0x0802D3A8
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	mov ip, r2
	adds r7, r3, #0
	ldr r0, [r5, #0x28]
	cmp r0, #0
	bne _0802D41C
	ldr r4, _0802D40C @ =gStaticData_0816A820
	ldr r1, [r5, #0x44]
	lsls r0, r1, #2
	movs r3, #0xff
	ands r0, r3
	lsls r0, r0, #1
	adds r0, r0, r4
	movs r2, #0
	ldrsh r0, [r0, r2]
	lsls r2, r0, #1
	adds r2, r2, r0
	lsls r2, r2, #3
	ldr r0, _0802D410 @ =0xFFFFF000
	adds r2, r2, r0
	adds r2, r6, r2
	lsls r1, r1, #1
	ands r1, r3
	lsls r1, r1, #1
	adds r1, r1, r4
	movs r0, #0
	ldrsh r1, [r1, r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #1
	ldr r1, _0802D414 @ =0xFFFFE200
	adds r0, r0, r1
	mov r1, ip
	adds r4, r1, r0
	ldr r0, _0802D418 @ =0xFFFFFE00
	adds r3, r7, r0
	ldr r1, [r5, #0x1c]
	subs r0, r2, r1
	cmp r0, #0
	bge _0802D3FE
	adds r0, #0xf
_0802D3FE:
	asrs r0, r0, #4
	adds r0, r1, r0
	str r0, [r5, #0x1c]
	ldr r1, [r5, #0x20]
	subs r0, r4, r1
	b _0802D46C
	.align 2, 0
_0802D40C: .4byte gStaticData_0816A820
_0802D410: .4byte 0xFFFFF000
_0802D414: .4byte 0xFFFFE200
_0802D418: .4byte 0xFFFFFE00
_0802D41C:
	cmp r0, #1
	bne _0802D450
	str r6, [r5, #0x1c]
	ldr r2, _0802D448 @ =gStaticData_0816A820
	ldr r1, [r5, #0x44]
	lsls r0, r1, #3
	adds r0, r0, r1
	movs r1, #0xff
	ands r0, r1
	lsls r0, r0, #1
	adds r0, r0, r2
	movs r1, #0
	ldrsh r0, [r0, r1]
	lsls r0, r0, #2
	ldr r2, _0802D44C @ =0xFFFFF600
	adds r0, r0, r2
	add r0, ip
	str r0, [r5, #0x20]
	movs r1, #0x80
	lsls r1, r1, #2
	adds r0, r7, r1
	b _0802D486
	.align 2, 0
_0802D448: .4byte gStaticData_0816A820
_0802D44C: .4byte 0xFFFFF600
_0802D450:
	movs r2, #0x80
	lsls r2, r2, #2
	adds r3, r7, r2
	ldr r1, [r5, #0x1c]
	subs r0, r6, r1
	cmp r0, #0
	bge _0802D460
	adds r0, #0xf
_0802D460:
	asrs r0, r0, #4
	adds r0, r1, r0
	str r0, [r5, #0x1c]
	ldr r1, [r5, #0x20]
	mov r2, ip
	subs r0, r2, r1
_0802D46C:
	cmp r0, #0
	bge _0802D472
	adds r0, #0xf
_0802D472:
	asrs r0, r0, #4
	adds r0, r1, r0
	str r0, [r5, #0x20]
	ldr r1, [r5, #0x24]
	subs r0, r3, r1
	cmp r0, #0
	bge _0802D482
	adds r0, #3
_0802D482:
	asrs r0, r0, #2
	adds r0, r1, r0
_0802D486:
	str r0, [r5, #0x24]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_802D490
sub_802D490: @ 0x0802D490
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, _0802D4AC @ =gUnknown_030012C0
	ldr r0, [r0]
	movs r1, #0
	bl sub_80231EC
	adds r0, r4, #0
	movs r1, #0
	bl sub_802D204
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0802D4AC: .4byte gUnknown_030012C0

	thumb_func_start sub_802D4B0
sub_802D4B0: @ 0x0802D4B0
	push {r4, r5, lr}
	adds r5, r0, #0
	ldr r0, _0802D4E4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0
	bl PlaySfx
	ldr r0, _0802D4E8 @ =gUnknown_030012C0
	ldr r0, [r0]
	ldr r4, [r0, #0x78]
	cmp r4, #0
	beq _0802D4D4
	subs r4, #1
	adds r1, r4, #0
	bl sub_80231EC
_0802D4D4:
	adds r0, r5, #0
	movs r1, #1
	bl sub_802D204
	adds r0, r4, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0802D4E4: .4byte gUnknown_030012BC
_0802D4E8: .4byte gUnknown_030012C0

	thumb_func_start sub_802D4EC
sub_802D4EC: @ 0x0802D4EC
	push {r4, r5, lr}
	adds r5, r0, #0
	ldr r0, _0802D520 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #1
	bl PlaySfx
	ldr r0, _0802D524 @ =gUnknown_030012C0
	ldr r0, [r0]
	ldr r4, [r0, #0x78]
	cmp r4, #3
	beq _0802D510
	adds r4, #1
	adds r1, r4, #0
	bl sub_80231EC
_0802D510:
	adds r0, r5, #0
	movs r1, #0
	bl sub_802D204
	adds r0, r4, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0802D520: .4byte gUnknown_030012BC
_0802D524: .4byte gUnknown_030012C0

	thumb_func_start sub_802D528
sub_802D528: @ 0x0802D528
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, [sp, #0x14]
	ldr r5, [sp, #0x18]
	ldr r6, _0802D568 @ =0xFFFFF000
	adds r2, r2, r6
	ldr r6, _0802D56C @ =0xFFFFE200
	adds r3, r3, r6
	ldr r6, _0802D570 @ =0xFFFFFE00
	adds r0, r0, r6
	str r0, [sp]
	adds r0, r4, #0
	bl InitActorPart
	ldr r0, _0802D574 @ =gStaticData_087E5054
	str r0, [r4, #0x50]
	ldr r0, _0802D578 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_80231EC
	adds r0, r4, #0
	movs r1, #0
	bl sub_802D204
	adds r0, r4, #0
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0802D568: .4byte 0xFFFFF000
_0802D56C: .4byte 0xFFFFE200
_0802D570: .4byte 0xFFFFFE00
_0802D574: .4byte gStaticData_087E5054
_0802D578: .4byte gUnknown_030012C0

	thumb_func_start sub_802D57C
sub_802D57C: @ 0x0802D57C
	push {lr}
	ldr r0, _0802D58C @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231EC
	pop {r0}
	bx r0
	.align 2, 0
_0802D58C: .4byte gUnknown_030012C0

	thumb_func_start sub_802D590
sub_802D590: @ 0x0802D590
	ldr r0, _0802D598 @ =gUnknown_030012C0
	ldr r0, [r0]
	ldr r0, [r0, #0x78]
	bx lr
	.align 2, 0
_0802D598: .4byte gUnknown_030012C0

	thumb_func_start sub_802D59C
sub_802D59C: @ 0x0802D59C
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x44]
	cmp r0, #5
	ble _0802D5AE
	adds r1, r4, #0
	adds r1, #0x2c
	movs r0, #1
	strb r0, [r1]
_0802D5AE:
	adds r0, r4, #0
	bl sub_802A6EC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802D5C2
	ldr r0, _0802D5D0 @ =gUnknown_03000884
	ldr r0, [r0]
	bl sub_802BFD4
_0802D5C2:
	adds r0, r4, #0
	bl sub_802A7B8
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0802D5D0: .4byte gUnknown_03000884

	thumb_func_start sub_802D5D4
sub_802D5D4: @ 0x0802D5D4
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, [sp, #0xc]
	str r0, [sp]
	adds r0, r4, #0
	bl InitActorPart
	ldr r0, _0802D5FC @ =gStaticData_087E5074
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
_0802D5FC: .4byte gStaticData_087E5074

	thumb_func_start sub_802D600
sub_802D600: @ 0x0802D600
	push {r4, r5, lr}
	adds r4, r0, #0
	bl sub_802A6EC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802D634
	ldr r0, _0802D640 @ =gUnknown_03000884
	ldr r0, [r0]
	ldr r1, [r4, #0x1c]
	bl sub_802C0BC
	adds r5, r4, #0
	adds r5, #0x54
	ldrb r0, [r5]
	cmp r0, #0
	bne _0802D634
	ldr r0, _0802D644 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x28
	bl PlaySfx
	movs r0, #1
	strb r0, [r5]
_0802D634:
	adds r0, r4, #0
	bl sub_802A7B8
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802D640: .4byte gUnknown_03000884
_0802D644: .4byte gUnknown_030012BC

	thumb_func_start sub_802D648
sub_802D648: @ 0x0802D648
	push {r4, r5, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r4, r2, #0
	ldr r0, [sp, #0x10]
	str r0, [sp]
	adds r0, r5, #0
	bl InitActorPart
	ldr r0, _0802D69C @ =gStaticData_087E5094
	str r0, [r5, #0x50]
	asrs r4, r4, #8
	movs r0, #0x14
	rsbs r0, r0, #0
	movs r2, #0
	cmp r4, r0
	blt _0802D672
	movs r2, #1
	cmp r4, #0x14
	ble _0802D672
	movs r2, #2
_0802D672:
	str r2, [r5, #0xc]
	ldr r1, [r5]
	lsls r0, r2, #1
	adds r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrh r0, [r0]
	movs r2, #0
	movs r1, #0
	strh r0, [r5, #0x10]
	strb r2, [r5, #0x12]
	str r1, [r5, #8]
	adds r0, r5, #0
	adds r0, #0x54
	strb r2, [r0]
	adds r0, r5, #0
	add sp, #4
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0802D69C: .4byte gStaticData_087E5094

	thumb_func_start sub_802D6A0
sub_802D6A0: @ 0x0802D6A0
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r5, [r4, #0xc]
	cmp r5, #0
	bne _0802D72A
	bl sub_802A6EC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802D6EE
	movs r0, #1
	str r0, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
	ldr r0, _0802D74C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x17
	bl PlaySfx
	ldr r0, _0802D750 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022FEC
	ldr r0, [r4, #0x24]
	bl sub_8029748
	ldr r0, [r4, #0x1c]
	ldr r1, [r4, #0x20]
	ldr r2, _0802D754 @ =0xFFFFF100
	adds r1, r1, r2
	ldr r2, [r4, #0x24]
	bl sub_802B12C
_0802D6EE:
	ldr r5, [r4, #0xc]
	cmp r5, #0
	bne _0802D72A
	adds r0, r4, #0
	bl sub_802DD9C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802D72A
	movs r0, #3
	str r0, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0x24]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
	movs r0, #1
	str r0, [r4, #0x18]
	ldr r0, _0802D74C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	ldr r0, _0802D750 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022FEC
_0802D72A:
	ldr r0, [r4, #0xc]
	cmp r0, #3
	bne _0802D758
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _0802D758
	cmp r4, #0
	beq _0802D75E
	ldr r1, [r4, #0x50]
	movs r2, #8
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
	b _0802D75E
	.align 2, 0
_0802D74C: .4byte gUnknown_030012BC
_0802D750: .4byte gUnknown_030012C0
_0802D754: .4byte 0xFFFFF100
_0802D758:
	adds r0, r4, #0
	bl sub_802A7B8
_0802D75E:
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_802D764
sub_802D764: @ 0x0802D764
	push {r4, r5, lr}
	sub sp, #4
	adds r5, r0, #0
	ldr r4, [sp, #0x10]
	str r4, [sp]
	bl InitActorPart
	ldr r0, _0802D7A8 @ =gStaticData_087E50B4
	str r0, [r5, #0x50]
	bl sub_802973C
	cmp r0, r4
	bne _0802D79E
	movs r0, #2
	str r0, [r5, #0xc]
	ldr r0, [r5]
	ldrh r0, [r0, #0x18]
	movs r1, #0
	movs r2, #0
	strh r0, [r5, #0x10]
	strb r1, [r5, #0x12]
	str r2, [r5, #8]
	ldr r0, _0802D7AC @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x17
	bl PlaySfx
_0802D79E:
	adds r0, r5, #0
	add sp, #4
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0802D7A8: .4byte gStaticData_087E50B4
_0802D7AC: .4byte gUnknown_030012BC

	thumb_func_start sub_802D7B0
sub_802D7B0: @ 0x0802D7B0
	push {r4, r5, r6, r7, lr}
	sub sp, #0x24
	ldr r0, _0802D934 @ =gUnknown_030014D0
	ldr r0, [r0]
	cmp r0, #3
	beq _0802D7D4
	ldr r2, _0802D938 @ =gUnknown_030014C4
	ldr r0, _0802D93C @ =gUnknown_03000884
	ldr r0, [r0]
	ldr r0, [r0, #0x1c]
	ldr r1, [r2]
	subs r0, r0, r1
	cmp r0, #0
	bge _0802D7CE
	adds r0, #0x1f
_0802D7CE:
	asrs r0, r0, #5
	adds r0, r1, r0
	str r0, [r2]
_0802D7D4:
	ldr r5, _0802D940 @ =gUnknown_030014BC
	ldr r4, [r5]
	ldr r0, [r4, #8]
	asrs r6, r0, #8
	movs r2, #0x10
	ldrsh r1, [r4, r2]
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
	blt _0802D814
	movs r3, #6
	ldrsh r0, [r1, r3]
	subs r0, r2, r0
	lsls r0, r0, #8
	ldr r1, [r4, #8]
	subs r1, r1, r0
	str r1, [r4, #8]
	movs r0, #1
	strb r0, [r4, #0x12]
_0802D814:
	ldr r1, _0802D944 @ =gStaticData_0817A840
	ldr r7, _0802D934 @ =gUnknown_030014D0
	ldr r0, [r7]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	bl sub_803AD78
	ldr r4, [r5]
	ldr r0, [r4, #8]
	asrs r5, r0, #8
	cmp r6, r5
	beq _0802D85C
	ldr r3, _0802D948 @ =gUnknown_03000898
	ldr r1, [r4, #0xc]
	ldr r2, [r4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	movs r1, #2
	ldrsh r0, [r0, r1]
	adds r0, r0, r5
	ldr r1, [r4, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	adds r0, #4
	ldr r1, _0802D94C @ =gUnknown_030014C0
	ldrb r1, [r1]
	ldr r2, [r3]
	bl sub_803AD80
	ldr r1, _0802D950 @ =gUnknown_030014C1
	movs r0, #1
	strb r0, [r1]
_0802D85C:
	ldr r0, _0802D954 @ =gUnknown_030014CC
	ldr r0, [r0]
	bl sub_8029E34
	bl sub_802D9A8
	mov r1, sp
	ldr r0, _0802D958 @ =gStaticData_0817AA98
	ldm r0!, {r2, r3, r4}
	stm r1!, {r2, r3, r4}
	ldr r0, _0802D938 @ =gUnknown_030014C4
	ldr r1, [r0]
	asrs r1, r1, #8
	ldr r0, _0802D95C @ =gUnknown_030014C8
	ldr r2, [r0]
	asrs r2, r2, #8
	mov r0, sp
	ldrh r5, [r0]
	adds r1, r5, r1
	strh r1, [r0]
	ldrh r1, [r0, #4]
	adds r2, r1, r2
	strh r2, [r0, #4]
	ldr r0, [r7]
	cmp r0, #1
	bls _0802D892
	b _0802D992
_0802D892:
	ldr r2, _0802D93C @ =gUnknown_03000884
	ldr r0, _0802D960 @ =gUnknown_030014A0
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802D992
	ldr r2, [r2]
	add r1, sp, #0x18
	adds r0, r2, #0
	adds r0, #0x38
	ldm r0!, {r3, r4, r5}
	stm r1!, {r3, r4, r5}
	ldr r0, [r2, #0x1c]
	asrs r0, r0, #8
	ldr r3, [r2, #0x20]
	asrs r3, r3, #8
	ldr r2, [r2, #0x24]
	asrs r2, r2, #8
	add r1, sp, #0x18
	ldrh r4, [r1]
	adds r0, r4, r0
	strh r0, [r1]
	ldrh r0, [r1, #2]
	adds r0, r0, r3
	strh r0, [r1, #2]
	ldrh r5, [r1, #4]
	adds r2, r5, r2
	strh r2, [r1, #4]
	add r0, sp, #0xc
	ldm r1!, {r2, r3, r4}
	stm r0!, {r2, r3, r4}
	add r4, sp, #0xc
	adds r0, r4, #0
	adds r1, r4, #0
	movs r2, #0xc
	bl sub_800014C
	mov r1, sp
	movs r5, #4
	ldrsh r2, [r4, r5]
	movs r0, #4
	ldrsh r3, [r1, r0]
	movs r5, #0xa
	ldrsh r0, [r1, r5]
	adds r0, r3, r0
	cmp r2, r0
	bge _0802D930
	movs r5, #0xa
	ldrsh r0, [r4, r5]
	adds r0, r2, r0
	cmp r0, r3
	ble _0802D930
	movs r0, #2
	ldrsh r2, [r4, r0]
	movs r5, #2
	ldrsh r3, [r1, r5]
	movs r5, #8
	ldrsh r0, [r1, r5]
	adds r0, r3, r0
	cmp r2, r0
	bge _0802D930
	movs r5, #8
	ldrsh r0, [r4, r5]
	adds r0, r2, r0
	cmp r0, r3
	ble _0802D930
	movs r0, #0
	ldrsh r2, [r4, r0]
	movs r5, #0
	ldrsh r3, [r1, r5]
	movs r5, #6
	ldrsh r0, [r1, r5]
	adds r0, r3, r0
	cmp r2, r0
	bge _0802D930
	movs r1, #6
	ldrsh r0, [r4, r1]
	adds r0, r2, r0
	cmp r0, r3
	bgt _0802D964
_0802D930:
	movs r0, #0
	b _0802D966
	.align 2, 0
_0802D934: .4byte gUnknown_030014D0
_0802D938: .4byte gUnknown_030014C4
_0802D93C: .4byte gUnknown_03000884
_0802D940: .4byte gUnknown_030014BC
_0802D944: .4byte gStaticData_0817A840
_0802D948: .4byte gUnknown_03000898
_0802D94C: .4byte gUnknown_030014C0
_0802D950: .4byte gUnknown_030014C1
_0802D954: .4byte gUnknown_030014CC
_0802D958: .4byte gStaticData_0817AA98
_0802D95C: .4byte gUnknown_030014C8
_0802D960: .4byte gUnknown_030014A0
_0802D964:
	movs r0, #1
_0802D966:
	cmp r0, #0
	beq _0802D992
	ldr r0, _0802D99C @ =gUnknown_030014D0
	movs r2, #2
	str r2, [r0]
	ldr r0, _0802D9A0 @ =gUnknown_030014BC
	ldr r1, [r0]
	str r2, [r1, #0xc]
	ldr r0, [r1]
	ldrh r0, [r0, #0x18]
	movs r2, #0
	movs r3, #0
	strh r0, [r1, #0x10]
	strb r2, [r1, #0x12]
	str r3, [r1, #8]
	ldr r0, _0802D9A4 @ =gUnknown_03000884
	ldr r0, [r0]
	bl sub_802C018
	movs r0, #0
	bl sub_8029BAC
_0802D992:
	add sp, #0x24
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802D99C: .4byte gUnknown_030014D0
_0802D9A0: .4byte gUnknown_030014BC
_0802D9A4: .4byte gUnknown_03000884

	thumb_func_start sub_802D9A8
sub_802D9A8: @ 0x0802D9A8
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	ldr r0, _0802D9C4 @ =gUnknown_030014CC
	ldr r1, [r0]
	ldr r0, _0802D9C8 @ =0x00004FFF
	cmp r1, r0
	bgt _0802D9DC
	ldr r1, _0802D9CC @ =0x040000D4
	ldr r0, _0802D9D0 @ =gStaticData_0817AA6C
	str r0, [r1]
	ldr r0, _0802D9D4 @ =0x050001E0
	str r0, [r1, #4]
	ldr r0, _0802D9D8 @ =0x80000010
	b _0802D9F4
	.align 2, 0
_0802D9C4: .4byte gUnknown_030014CC
_0802D9C8: .4byte 0x00004FFF
_0802D9CC: .4byte 0x040000D4
_0802D9D0: .4byte gStaticData_0817AA6C
_0802D9D4: .4byte 0x050001E0
_0802D9D8: .4byte 0x80000010
_0802D9DC:
	ldr r0, _0802D9FC @ =0x0000BDFF
	cmp r1, r0
	ble _0802DA0C
	mov r1, sp
	movs r0, #0
	strh r0, [r1]
	ldr r1, _0802DA00 @ =0x040000D4
	mov r0, sp
	str r0, [r1]
	ldr r0, _0802DA04 @ =0x050001E0
	str r0, [r1, #4]
	ldr r0, _0802DA08 @ =0x81000010
_0802D9F4:
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	b _0802DA56
	.align 2, 0
_0802D9FC: .4byte 0x0000BDFF
_0802DA00: .4byte 0x040000D4
_0802DA04: .4byte 0x050001E0
_0802DA08: .4byte 0x81000010
_0802DA0C:
	movs r0, #0xbe
	lsls r0, r0, #8
	subs r0, r0, r1
	lsls r0, r0, #8
	movs r1, #0xdc
	lsls r1, r1, #7
	bl sub_803ADB4
	adds r3, r0, #0
	movs r0, #0x1f
	mov ip, r0
	ldr r6, _0802DA60 @ =0x050001E0
	ldr r5, _0802DA64 @ =gStaticData_0817AA6C
	movs r7, #0x1f
	movs r4, #0xf
_0802DA2A:
	ldrh r1, [r5]
	adds r0, r7, #0
	ands r0, r1
	adds r2, r0, #0
	muls r2, r3, r2
	asrs r2, r2, #8
	lsrs r1, r1, #5
	mov r0, ip
	ands r1, r0
	adds r0, r1, #0
	muls r0, r3, r0
	asrs r0, r0, #8
	lsls r1, r0, #5
	orrs r2, r1
	lsls r0, r0, #0xa
	orrs r2, r0
	strh r2, [r6]
	adds r6, #2
	adds r5, #2
	subs r4, #1
	cmp r4, #0
	bge _0802DA2A
_0802DA56:
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802DA60: .4byte 0x050001E0
_0802DA64: .4byte gStaticData_0817AA6C

	thumb_func_start sub_802DA68
sub_802DA68: @ 0x0802DA68
	push {r4, r5, r6, lr}
	ldr r0, _0802DA84 @ =gUnknown_030014C1
	ldrb r1, [r0]
	adds r3, r0, #0
	cmp r1, #0
	beq _0802DAA8
	ldr r0, _0802DA88 @ =gUnknown_030014C0
	ldrb r1, [r0]
	adds r2, r0, #0
	cmp r1, #0
	beq _0802DA94
	ldr r1, _0802DA8C @ =0x0400000C
	ldr r4, _0802DA90 @ =0x00001A09
	b _0802DA98
	.align 2, 0
_0802DA84: .4byte gUnknown_030014C1
_0802DA88: .4byte gUnknown_030014C0
_0802DA8C: .4byte 0x0400000C
_0802DA90: .4byte 0x00001A09
_0802DA94:
	ldr r1, _0802DB10 @ =0x0400000C
	ldr r4, _0802DB14 @ =0x00001B09
_0802DA98:
	adds r0, r4, #0
	strh r0, [r1]
	movs r0, #0
	strb r0, [r3]
	movs r0, #1
	ldrb r1, [r2]
	eors r0, r1
	strb r0, [r2]
_0802DAA8:
	ldr r5, _0802DB18 @ =gUnknown_030014CC
	ldr r0, [r5]
	lsls r0, r0, #8
	movs r1, #0xaa
	lsls r1, r1, #7
	bl sub_803ADB4
	adds r4, r0, #0
	bl sub_8029EB4
	adds r6, r0, #0
	ldr r0, _0802DB1C @ =gUnknown_030014C4
	ldr r1, [r0]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #4
	subs r0, r0, r1
	lsls r0, r0, #8
	ldr r1, [r5]
	bl sub_803ADB4
	adds r0, r0, r6
	ldr r2, _0802DB20 @ =0x04000028
	adds r1, r0, #0
	muls r1, r4, r1
	asrs r1, r1, #8
	movs r0, #0x80
	lsls r0, r0, #7
	subs r0, r0, r1
	str r0, [r2]
	bl sub_8029E98
	ldr r2, _0802DB24 @ =0x0400002C
	adds r1, r0, #0
	muls r1, r4, r1
	asrs r1, r1, #8
	movs r0, #0x88
	lsls r0, r0, #7
	subs r0, r0, r1
	str r0, [r2]
	ldr r0, _0802DB28 @ =0x04000020
	strh r4, [r0]
	adds r0, #2
	movs r1, #0
	strh r1, [r0]
	adds r0, #2
	strh r1, [r0]
	adds r0, #2
	strh r4, [r0]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802DB10: .4byte 0x0400000C
_0802DB14: .4byte 0x00001B09
_0802DB18: .4byte gUnknown_030014CC
_0802DB1C: .4byte gUnknown_030014C4
_0802DB20: .4byte 0x04000028
_0802DB24: .4byte 0x0400002C
_0802DB28: .4byte 0x04000020

	thumb_func_start sub_802DB2C
sub_802DB2C: @ 0x0802DB2C
	push {r4, r5, lr}
	sub sp, #4
	bl sub_8029B98
	cmp r0, #0x24
	bne _0802DB54
	ldr r4, _0802DB4C @ =gUnknown_030014C8
	bl sub_8029B2C
	lsls r0, r0, #8
	ldr r1, _0802DB50 @ =gUnknown_030014CC
	ldr r1, [r1]
	subs r0, r0, r1
	str r0, [r4]
	b _0802DB6A
	.align 2, 0
_0802DB4C: .4byte gUnknown_030014C8
_0802DB50: .4byte gUnknown_030014CC
_0802DB54:
	ldr r4, _0802DBB0 @ =gUnknown_030014C8
	ldr r0, [r4]
	adds r0, #0x99
	str r0, [r4]
	ldr r5, _0802DBB4 @ =gUnknown_030014CC
	bl sub_8029B2C
	lsls r0, r0, #8
	ldr r1, [r4]
	subs r0, r0, r1
	str r0, [r5]
_0802DB6A:
	ldr r5, _0802DBB4 @ =gUnknown_030014CC
	ldr r0, [r5]
	movs r1, #0xa0
	lsls r1, r1, #8
	cmp r0, r1
	ble _0802DB86
	str r1, [r5]
	ldr r4, _0802DBB0 @ =gUnknown_030014C8
	bl sub_8029B2C
	lsls r0, r0, #8
	ldr r1, [r5]
	subs r0, r0, r1
	str r0, [r4]
_0802DB86:
	ldr r0, _0802DBB8 @ =gUnknown_030014BC
	ldr r0, [r0]
	ldr r0, [r0, #8]
	asrs r2, r0, #8
	ldr r1, [r5]
	ldr r0, _0802DBBC @ =0x00004FFF
	cmp r1, r0
	bgt _0802DC00
	cmp r2, #0xc
	bne _0802DBC4
	ldr r0, _0802DBC0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0xfa
	lsls r2, r2, #2
	movs r3, #0x80
	lsls r3, r3, #1
	mov r4, sp
	movs r1, #1
	strb r1, [r4]
	movs r1, #0x3f
	b _0802DBDC
	.align 2, 0
_0802DBB0: .4byte gUnknown_030014C8
_0802DBB4: .4byte gUnknown_030014CC
_0802DBB8: .4byte gUnknown_030014BC
_0802DBBC: .4byte 0x00004FFF
_0802DBC0: .4byte gUnknown_030012BC
_0802DBC4:
	cmp r2, #0x1c
	bne _0802DBF0
	ldr r0, _0802DBEC @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0xfa
	lsls r2, r2, #2
	movs r3, #0x80
	lsls r3, r3, #1
	mov r4, sp
	movs r1, #1
	strb r1, [r4]
	movs r1, #0x40
_0802DBDC:
	bl sub_80019F8
	movs r0, #0x80
	lsls r0, r0, #2
	bl sub_8029E28
	b _0802DC00
	.align 2, 0
_0802DBEC: .4byte gUnknown_030012BC
_0802DBF0:
	cmp r2, #0xd
	beq _0802DBF8
	cmp r2, #0x1d
	bne _0802DC00
_0802DBF8:
	movs r0, #0x80
	lsls r0, r0, #1
	bl sub_8029E28
_0802DC00:
	ldr r0, _0802DCA8 @ =gUnknown_030014BC
	ldr r0, [r0]
	ldrb r0, [r0, #0x12]
	cmp r0, #0
	beq _0802DC9E
	ldr r0, _0802DCAC @ =gUnknown_030014CC
	ldr r1, [r0]
	movs r0, #0xb4
	lsls r0, r0, #7
	cmp r1, r0
	bgt _0802DC6A
	bl sub_8029B98
	cmp r0, #0x24
	ble _0802DC40
	movs r0, #0x80
	lsls r0, r0, #1
	bl sub_8000E1C
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	ldr r3, _0802DCB0 @ =gStaticData_0817A7F8
	ldr r1, _0802DCB4 @ =gUnknown_030014D4
	ldr r2, [r1]
	lsls r1, r2, #1
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r3, #4
	adds r1, r1, r3
	ldr r1, [r1]
	cmp r0, r1
	blt _0802DC6A
_0802DC40:
	bl sub_8029B98
	cmp r0, #0x24
	bgt _0802DC9E
	movs r0, #0x80
	lsls r0, r0, #1
	bl sub_8000E1C
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	ldr r3, _0802DCB0 @ =gStaticData_0817A7F8
	ldr r1, _0802DCB4 @ =gUnknown_030014D4
	ldr r2, [r1]
	lsls r1, r2, #1
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r3, #8
	adds r1, r1, r3
	ldr r1, [r1]
	cmp r0, r1
	bge _0802DC9E
_0802DC6A:
	ldr r0, _0802DCB8 @ =gUnknown_030014D0
	movs r2, #1
	str r2, [r0]
	ldr r0, _0802DCA8 @ =gUnknown_030014BC
	ldr r1, [r0]
	str r2, [r1, #0xc]
	ldr r0, [r1]
	ldrh r0, [r0, #0xc]
	movs r2, #0
	movs r3, #0
	strh r0, [r1, #0x10]
	strb r2, [r1, #0x12]
	str r3, [r1, #8]
	ldr r0, _0802DCAC @ =gUnknown_030014CC
	ldr r1, [r0]
	movs r0, #0xf0
	lsls r0, r0, #7
	cmp r1, r0
	bgt _0802DC9E
	ldr r0, _0802DCBC @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x20
	bl PlaySfx
_0802DC9E:
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802DCA8: .4byte gUnknown_030014BC
_0802DCAC: .4byte gUnknown_030014CC
_0802DCB0: .4byte gStaticData_0817A7F8
_0802DCB4: .4byte gUnknown_030014D4
_0802DCB8: .4byte gUnknown_030014D0
_0802DCBC: .4byte gUnknown_030012BC

	thumb_func_start sub_802DCC0
sub_802DCC0: @ 0x0802DCC0
	push {r4, r5, lr}
	ldr r5, _0802DD24 @ =gUnknown_030014C8
	ldr r2, _0802DD28 @ =gStaticData_0817A7F8
	ldr r0, _0802DD2C @ =gUnknown_030014D4
	ldr r1, [r0]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r1, [r5]
	ldr r0, [r0]
	adds r1, r1, r0
	str r1, [r5]
	ldr r4, _0802DD30 @ =gUnknown_030014CC
	bl sub_8029B2C
	lsls r0, r0, #8
	ldr r1, [r5]
	subs r0, r0, r1
	str r0, [r4]
	movs r1, #0xa0
	lsls r1, r1, #8
	cmp r0, r1
	ble _0802DCFE
	str r1, [r4]
	bl sub_8029B2C
	lsls r0, r0, #8
	ldr r1, [r4]
	subs r0, r0, r1
	str r0, [r5]
_0802DCFE:
	ldr r0, _0802DD34 @ =gUnknown_030014BC
	ldr r0, [r0]
	ldr r0, [r0, #8]
	asrs r2, r0, #8
	ldr r1, [r4]
	ldr r0, _0802DD38 @ =0x00004FFF
	cmp r1, r0
	bgt _0802DD70
	cmp r2, #0xb
	bne _0802DD40
	ldr r0, _0802DD3C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x3f
	bl PlaySfx
	b _0802DD52
	.align 2, 0
_0802DD24: .4byte gUnknown_030014C8
_0802DD28: .4byte gStaticData_0817A7F8
_0802DD2C: .4byte gUnknown_030014D4
_0802DD30: .4byte gUnknown_030014CC
_0802DD34: .4byte gUnknown_030014BC
_0802DD38: .4byte 0x00004FFF
_0802DD3C: .4byte gUnknown_030012BC
_0802DD40:
	cmp r2, #0x1b
	bne _0802DD60
	ldr r0, _0802DD5C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x40
	bl PlaySfx
_0802DD52:
	movs r0, #0x80
	lsls r0, r0, #2
	bl sub_8029E28
	b _0802DD70
	.align 2, 0
_0802DD5C: .4byte gUnknown_030012BC
_0802DD60:
	cmp r2, #0xc
	beq _0802DD68
	cmp r2, #0x1c
	bne _0802DD70
_0802DD68:
	movs r0, #0x80
	lsls r0, r0, #1
	bl sub_8029E28
_0802DD70:
	ldr r0, _0802DD94 @ =gUnknown_030014BC
	ldr r3, [r0]
	ldrb r0, [r3, #0x12]
	cmp r0, #0
	beq _0802DD8E
	ldr r0, _0802DD98 @ =gUnknown_030014D0
	movs r1, #0
	str r1, [r0]
	str r1, [r3, #0xc]
	ldr r0, [r3]
	ldrh r0, [r0]
	movs r2, #0
	strh r0, [r3, #0x10]
	strb r2, [r3, #0x12]
	str r1, [r3, #8]
_0802DD8E:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802DD94: .4byte gUnknown_030014BC
_0802DD98: .4byte gUnknown_030014D0

	thumb_func_start sub_802DD9C
sub_802DD9C: @ 0x0802DD9C
	push {r4, r5, lr}
	sub sp, #0x24
	mov r2, sp
	ldr r1, _0802DE58 @ =gStaticData_0817AA8C
	ldm r1!, {r3, r4, r5}
	stm r2!, {r3, r4, r5}
	ldr r1, _0802DE5C @ =gUnknown_030014C4
	ldr r2, [r1]
	asrs r2, r2, #8
	ldr r1, _0802DE60 @ =gUnknown_030014C8
	ldr r3, [r1]
	asrs r3, r3, #8
	mov r1, sp
	ldrh r4, [r1]
	adds r2, r4, r2
	strh r2, [r1]
	ldrh r5, [r1, #4]
	adds r3, r5, r3
	strh r3, [r1, #4]
	add r2, sp, #0x18
	adds r1, r0, #0
	adds r1, #0x38
	ldm r1!, {r3, r4, r5}
	stm r2!, {r3, r4, r5}
	ldr r2, [r0, #0x1c]
	asrs r2, r2, #8
	ldr r4, [r0, #0x20]
	asrs r4, r4, #8
	ldr r3, [r0, #0x24]
	asrs r3, r3, #8
	add r1, sp, #0x18
	ldrh r0, [r1]
	adds r2, r0, r2
	strh r2, [r1]
	ldrh r0, [r1, #2]
	adds r0, r0, r4
	strh r0, [r1, #2]
	ldrh r2, [r1, #4]
	adds r3, r2, r3
	strh r3, [r1, #4]
	add r0, sp, #0xc
	ldm r1!, {r3, r4, r5}
	stm r0!, {r3, r4, r5}
	add r4, sp, #0xc
	adds r0, r4, #0
	adds r1, r4, #0
	movs r2, #0xc
	bl sub_800014C
	mov r1, sp
	movs r0, #4
	ldrsh r2, [r1, r0]
	movs r5, #4
	ldrsh r3, [r4, r5]
	movs r5, #0xa
	ldrsh r0, [r4, r5]
	adds r0, r3, r0
	cmp r2, r0
	bge _0802DE54
	movs r5, #0xa
	ldrsh r0, [r1, r5]
	adds r0, r2, r0
	cmp r0, r3
	ble _0802DE54
	movs r0, #2
	ldrsh r2, [r1, r0]
	movs r5, #2
	ldrsh r3, [r4, r5]
	movs r5, #8
	ldrsh r0, [r4, r5]
	adds r0, r3, r0
	cmp r2, r0
	bge _0802DE54
	movs r5, #8
	ldrsh r0, [r1, r5]
	adds r0, r2, r0
	cmp r0, r3
	ble _0802DE54
	movs r0, #0
	ldrsh r2, [r1, r0]
	movs r5, #0
	ldrsh r3, [r4, r5]
	movs r5, #6
	ldrsh r0, [r4, r5]
	adds r0, r3, r0
	cmp r2, r0
	bge _0802DE54
	movs r4, #6
	ldrsh r0, [r1, r4]
	adds r0, r2, r0
	cmp r0, r3
	bgt _0802DE64
_0802DE54:
	movs r0, #0
	b _0802DE66
	.align 2, 0
_0802DE58: .4byte gStaticData_0817AA8C
_0802DE5C: .4byte gUnknown_030014C4
_0802DE60: .4byte gUnknown_030014C8
_0802DE64:
	movs r0, #1
_0802DE66:
	add sp, #0x24
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_802DE70
sub_802DE70: @ 0x0802DE70
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x100
	movs r0, #0x80
	lsls r0, r0, #0x13
	ldrh r1, [r0]
	movs r3, #0x80
	lsls r3, r3, #3
	adds r2, r3, #0
	orrs r1, r2
	strh r1, [r0]
	mov ip, sp
	movs r6, #0
	movs r5, #0
	ldr r0, _0802DEBC @ =gUnknown_030014C0
	mov sb, r0
	ldr r1, _0802DEC0 @ =gUnknown_03000898
	mov sl, r1
	ldr r3, _0802DEC4 @ =gUnknown_030014BC
	mov r8, r3
	movs r7, #0xff
_0802DEA0:
	movs r4, #0
	lsls r0, r5, #4
	adds r2, r5, #1
	mov r1, ip
	adds r3, r0, r1
_0802DEAA:
	subs r0, r4, #3
	cmp r0, #9
	bhi _0802DEB8
	cmp r5, #2
	ble _0802DEB8
	cmp r5, #0xc
	ble _0802DEC8
_0802DEB8:
	strb r7, [r3]
	b _0802DED2
	.align 2, 0
_0802DEBC: .4byte gUnknown_030014C0
_0802DEC0: .4byte gUnknown_03000898
_0802DEC4: .4byte gUnknown_030014BC
_0802DEC8:
	adds r1, r6, #0
	adds r0, r1, #1
	lsls r0, r0, #0x18
	lsrs r6, r0, #0x18
	strb r1, [r3]
_0802DED2:
	adds r3, #1
	adds r4, #1
	cmp r4, #0xf
	ble _0802DEAA
	adds r5, r2, #0
	cmp r5, #0xf
	ble _0802DEA0
	ldr r1, _0802DF14 @ =0x040000D4
	mov r3, sp
	str r3, [r1]
	ldr r0, _0802DF18 @ =0x0600D000
	str r0, [r1, #4]
	ldr r0, _0802DF1C @ =0x80000080
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	mov ip, sp
	movs r6, #0x80
	movs r5, #0
	movs r7, #0xff
_0802DEF8:
	movs r4, #0
	lsls r0, r5, #4
	adds r2, r5, #1
	mov r1, ip
	adds r3, r0, r1
_0802DF02:
	subs r0, r4, #3
	cmp r0, #9
	bhi _0802DF10
	cmp r5, #2
	ble _0802DF10
	cmp r5, #0xc
	ble _0802DF20
_0802DF10:
	strb r7, [r3]
	b _0802DF2A
	.align 2, 0
_0802DF14: .4byte 0x040000D4
_0802DF18: .4byte 0x0600D000
_0802DF1C: .4byte 0x80000080
_0802DF20:
	adds r1, r6, #0
	adds r0, r1, #1
	lsls r0, r0, #0x18
	lsrs r6, r0, #0x18
	strb r1, [r3]
_0802DF2A:
	adds r3, #1
	adds r4, #1
	cmp r4, #0xf
	ble _0802DF02
	adds r5, r2, #0
	cmp r5, #0xf
	ble _0802DEF8
	ldr r1, _0802DFA8 @ =0x040000D4
	mov r3, sp
	str r3, [r1]
	ldr r0, _0802DFAC @ =0x0600D800
	str r0, [r1, #4]
	ldr r0, _0802DFB0 @ =0x80000080
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r1, _0802DFB4 @ =0x0600BFC0
	movs r3, #0
	adds r0, r1, #0
	adds r0, #0x3c
_0802DF50:
	str r3, [r0]
	subs r0, #4
	cmp r0, r1
	bge _0802DF50
	movs r5, #1
	mov r0, sb
	strb r5, [r0]
	mov r1, r8
	ldr r2, [r1]
	ldr r3, [r2, #8]
	asrs r3, r3, #8
	ldr r1, [r2, #0xc]
	ldr r4, [r2]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r4
	movs r1, #2
	ldrsh r0, [r0, r1]
	adds r0, r0, r3
	ldr r1, [r2, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	adds r0, #4
	mov r3, sl
	ldr r2, [r3]
	movs r1, #1
	bl sub_803AD80
	ldr r0, _0802DFB8 @ =gUnknown_030014C1
	strb r5, [r0]
	bl sub_802DA68
	bl sub_802D9A8
	add sp, #0x100
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802DFA8: .4byte 0x040000D4
_0802DFAC: .4byte 0x0600D800
_0802DFB0: .4byte 0x80000080
_0802DFB4: .4byte 0x0600BFC0
_0802DFB8: .4byte gUnknown_030014C1

	thumb_func_start sub_802DFBC
sub_802DFBC: @ 0x0802DFBC
	ldr r1, _0802DFC4 @ =gUnknown_030014D0
	movs r0, #3
	str r0, [r1]
	bx lr
	.align 2, 0
_0802DFC4: .4byte gUnknown_030014D0

	thumb_func_start sub_802DFC8
sub_802DFC8: @ 0x0802DFC8
	push {lr}
	ldr r0, _0802DFD8 @ =gUnknown_030014BC
	ldr r0, [r0]
	bl mem_free
	pop {r0}
	bx r0
	.align 2, 0
_0802DFD8: .4byte gUnknown_030014BC

	thumb_func_start sub_802DFDC
sub_802DFDC: @ 0x0802DFDC
	push {r4, r5, r6, lr}
	ldr r1, _0802E038 @ =gUnknown_030014D4
	str r0, [r1]
	ldr r5, _0802E03C @ =gUnknown_030014BC
	movs r0, #0x1c
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	ldr r0, _0802E040 @ =gStaticData_0817A850
	ldr r1, _0802E044 @ =gStaticData_0817A880
	movs r2, #0xf
	str r0, [r4]
	str r1, [r4, #4]
	str r2, [r4, #0x18]
	adds r0, r4, #0
	movs r1, #0
	bl sub_803B0A8
	str r4, [r5]
	ldr r0, _0802E048 @ =gUnknown_030014C4
	movs r6, #0
	str r6, [r0]
	ldr r4, _0802E04C @ =gUnknown_030014CC
	movs r0, #0xa0
	lsls r0, r0, #8
	str r0, [r4]
	ldr r5, _0802E050 @ =gUnknown_030014C8
	bl sub_8029B2C
	lsls r0, r0, #8
	ldr r1, [r4]
	subs r0, r0, r1
	str r0, [r5]
	adds r0, r1, #0
	bl sub_8029E34
	ldr r0, _0802E054 @ =gUnknown_030014D0
	str r6, [r0]
	bl sub_802DE70
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802E038: .4byte gUnknown_030014D4
_0802E03C: .4byte gUnknown_030014BC
_0802E040: .4byte gStaticData_0817A850
_0802E044: .4byte gStaticData_0817A880
_0802E048: .4byte gUnknown_030014C4
_0802E04C: .4byte gUnknown_030014CC
_0802E050: .4byte gUnknown_030014C8
_0802E054: .4byte gUnknown_030014D0

	thumb_func_start sub_802E058
sub_802E058: @ 0x0802E058
	push {r4, r5, r6, r7, lr}
	mov ip, r0
	lsls r1, r1, #0x18
	lsrs r5, r1, #0x18
	movs r4, #0
	movs r7, #0xff
_0802E064:
	movs r3, #0
	lsls r0, r4, #4
	adds r6, r4, #1
	mov r1, ip
	adds r2, r0, r1
_0802E06E:
	subs r0, r3, #3
	cmp r0, #9
	bhi _0802E07C
	cmp r4, #2
	ble _0802E07C
	cmp r4, #0xc
	ble _0802E080
_0802E07C:
	strb r7, [r2]
	b _0802E08A
_0802E080:
	adds r1, r5, #0
	adds r0, r1, #1
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	strb r1, [r2]
_0802E08A:
	adds r2, #1
	adds r3, #1
	cmp r3, #0xf
	ble _0802E06E
	adds r4, r6, #0
	cmp r4, #0xf
	ble _0802E064
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start nullsub_27
nullsub_27: @ 0x0802E0A0
	bx lr
	.align 2, 0

	thumb_func_start sub_802E0A4
sub_802E0A4: @ 0x0802E0A4
	ldr r0, _0802E0C8 @ =gUnknown_030014BC
	ldr r3, [r0]
	ldr r0, [r3, #0xc]
	cmp r0, #3
	beq _0802E0C6
	ldrb r0, [r3, #0x12]
	cmp r0, #0
	beq _0802E0C6
	movs r0, #3
	str r0, [r3, #0xc]
	ldr r0, [r3]
	ldrh r0, [r0, #0x24]
	movs r1, #0
	movs r2, #0
	strh r0, [r3, #0x10]
	strb r1, [r3, #0x12]
	str r2, [r3, #8]
_0802E0C6:
	bx lr
	.align 2, 0
_0802E0C8: .4byte gUnknown_030014BC

	thumb_func_start sub_802E0CC
sub_802E0CC: @ 0x0802E0CC
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r6, r2, #0
	lsls r1, r1, #0x18
	lsrs r2, r1, #0x18
	ldrb r4, [r5]
	ldr r1, _0802E0F0 @ =gUnknown_030012C0
	ldr r0, [r1]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802E0F4
	ldrb r4, [r5, #1]
	cmp r4, #0x17
	bne _0802E0FA
	movs r4, #0x14
	b _0802E0FA
	.align 2, 0
_0802E0F0: .4byte gUnknown_030012C0
_0802E0F4:
	cmp r2, #0
	beq _0802E0FA
	ldrb r4, [r5, #2]
_0802E0FA:
	cmp r4, #0x1d
	bne _0802E10A
	ldr r0, [r1]
	bl sub_8023418
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802E166
_0802E10A:
	cmp r4, #0
	beq _0802E166
	cmp r4, #0x3e
	beq _0802E166
	adds r0, r4, #0
	subs r0, #0x20
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #5
	bls _0802E166
	ldr r0, [r5, #4]
	lsls r2, r0, #8
	ldr r0, [r5, #8]
	lsls r3, r0, #8
	ldr r0, [r5, #0xc]
	lsls r0, r0, #8
	adds r6, r0, r6
	adds r1, r4, #0
	subs r1, #0x10
	lsls r0, r1, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #2
	bhi _0802E146
	adds r0, r1, #0
	adds r1, r2, #0
	adds r2, r3, #0
	adds r3, r6, #0
	bl sub_8031040
	b _0802E166
_0802E146:
	cmp r4, #0xa
	beq _0802E15A
	str r5, [sp]
	adds r0, r4, #0
	adds r1, r2, #0
	adds r2, r3, #0
	adds r3, r6, #0
	bl sub_802E170
	b _0802E168
_0802E15A:
	movs r0, #0
	adds r1, r2, #0
	adds r2, r3, #0
	adds r3, r6, #0
	bl sub_8033264
_0802E166:
	movs r0, #0
_0802E168:
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1

	thumb_func_start sub_802E170
sub_802E170: @ 0x0802E170
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	adds r5, r1, #0
	mov r8, r2
	adds r7, r3, #0
	ldr r4, [sp, #0x24]
	lsls r0, r0, #0x18
	lsrs r6, r0, #0x18
	ldr r0, _0802E1AC @ =gUnknown_030014D8
	ldr r1, [r0]
	lsls r0, r6, #2
	adds r0, r0, r6
	lsls r0, r0, #3
	adds r0, r0, r1
	ldr r1, [r0, #0x20]
	adds r5, r5, r1
	ldr r0, [r0, #0x24]
	add r8, r0
	subs r0, r6, #1
	cmp r0, #0x1e
	bls _0802E1A2
	b _0802E3BC
_0802E1A2:
	lsls r0, r0, #2
	ldr r1, _0802E1B0 @ =_0802E1B4
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0802E1AC: .4byte gUnknown_030014D8
_0802E1B0: .4byte _0802E1B4
_0802E1B4: @ jump table
	.4byte _0802E230 @ case 0
	.4byte _0802E3BC @ case 1
	.4byte _0802E3BC @ case 2
	.4byte _0802E258 @ case 3
	.4byte _0802E258 @ case 4
	.4byte _0802E258 @ case 5
	.4byte _0802E258 @ case 6
	.4byte _0802E258 @ case 7
	.4byte _0802E258 @ case 8
	.4byte _0802E3BC @ case 9
	.4byte _0802E3BC @ case 10
	.4byte _0802E3BC @ case 11
	.4byte _0802E3BC @ case 12
	.4byte _0802E3BC @ case 13
	.4byte _0802E3BC @ case 14
	.4byte _0802E3BC @ case 15
	.4byte _0802E3BC @ case 16
	.4byte _0802E3BC @ case 17
	.4byte _0802E280 @ case 18
	.4byte _0802E2CC @ case 19
	.4byte _0802E2CC @ case 20
	.4byte _0802E2CC @ case 21
	.4byte _0802E2A8 @ case 22
	.4byte _0802E2F4 @ case 23
	.4byte _0802E2F4 @ case 24
	.4byte _0802E2F4 @ case 25
	.4byte _0802E31C @ case 26
	.4byte _0802E344 @ case 27
	.4byte _0802E2F4 @ case 28
	.4byte _0802E3BC @ case 29
	.4byte _0802E36C @ case 30
_0802E230:
	movs r0, #0x80
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E254 @ =gUnknown_030014D8
	lsls r2, r6, #2
	adds r2, r2, r6
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	str r7, [sp]
	str r4, [sp, #4]
	adds r2, r5, #0
	mov r3, r8
	bl sub_802FD8C
	b _0802E3BE
	.align 2, 0
_0802E254: .4byte gUnknown_030014D8
_0802E258:
	movs r0, #0x64
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E27C @ =gUnknown_030014D8
	lsls r2, r6, #2
	adds r2, r2, r6
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	str r7, [sp]
	adds r2, r5, #0
	mov r3, r8
	bl sub_802FF08
	b _0802E3BE
	.align 2, 0
_0802E27C: .4byte gUnknown_030014D8
_0802E280:
	movs r0, #0x70
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E2A4 @ =gUnknown_030014D8
	lsls r2, r6, #2
	adds r2, r2, r6
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	str r7, [sp]
	adds r2, r5, #0
	mov r3, r8
	bl sub_8032054
	b _0802E3BE
	.align 2, 0
_0802E2A4: .4byte gUnknown_030014D8
_0802E2A8:
	adds r0, r4, #0
	bl sub_802AA80
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802E2CC
	movs r0, #0x74
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E2C8 @ =gUnknown_030014D8
	ldr r1, [r1]
	movs r2, #0xc8
	lsls r2, r2, #2
	b _0802E2E0
	.align 2, 0
_0802E2C8: .4byte gUnknown_030014D8
_0802E2CC:
	movs r0, #0x74
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E2F0 @ =gUnknown_030014D8
	lsls r2, r6, #2
	adds r2, r2, r6
	lsls r2, r2, #3
	ldr r1, [r1]
_0802E2E0:
	adds r1, r1, r2
	str r7, [sp]
	str r4, [sp, #4]
	adds r2, r5, #0
	mov r3, r8
	bl sub_80320C4
	b _0802E3BE
	.align 2, 0
_0802E2F0: .4byte gUnknown_030014D8
_0802E2F4:
	movs r0, #0x70
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E318 @ =gUnknown_030014D8
	lsls r2, r6, #2
	adds r2, r2, r6
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	str r7, [sp]
	adds r2, r5, #0
	mov r3, r8
	bl sub_8031F78
	b _0802E3BE
	.align 2, 0
_0802E318: .4byte gUnknown_030014D8
_0802E31C:
	movs r0, #0x60
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E340 @ =gUnknown_030014D8
	lsls r2, r6, #2
	adds r2, r2, r6
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	str r7, [sp]
	adds r2, r5, #0
	mov r3, r8
	bl sub_8032440
	b _0802E3BE
	.align 2, 0
_0802E340: .4byte gUnknown_030014D8
_0802E344:
	movs r0, #0x68
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E368 @ =gUnknown_030014D8
	lsls r2, r6, #2
	adds r2, r2, r6
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	str r7, [sp]
	adds r2, r5, #0
	mov r3, r8
	bl sub_80325EC
	b _0802E3BE
	.align 2, 0
_0802E368: .4byte gUnknown_030014D8
_0802E36C:
	movs r0, #0x5c
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r3, _0802E3B8 @ =gUnknown_030014D8
	mov sb, r3
	ldr r2, [r3]
	movs r3, #0xd7
	lsls r3, r3, #3
	adds r1, r2, r3
	lsls r4, r6, #2
	adds r4, r4, r6
	lsls r4, r4, #3
	adds r2, r4, r2
	ldr r2, [r2, #0x20]
	subs r2, r5, r2
	ldr r3, [r1, #0x20]
	adds r2, r2, r3
	str r7, [sp]
	mov r3, r8
	bl sub_80326E4
	movs r0, #0x5c
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	mov r2, sb
	ldr r1, [r2]
	adds r1, r1, r4
	str r7, [sp]
	adds r2, r5, #0
	mov r3, r8
	bl sub_80326E4
	b _0802E3BE
	.align 2, 0
_0802E3B8: .4byte gUnknown_030014D8
_0802E3BC:
	movs r0, #0
_0802E3BE:
	add sp, #8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_802E3CC
sub_802E3CC: @ 0x0802E3CC
	push {r4, r5, lr}
	sub sp, #4
	ldr r0, _0802E414 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x17
	bl PlaySfx
	movs r0, #0x58
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	ldr r0, _0802E418 @ =gUnknown_030014D8
	ldr r1, [r0]
	movs r0, #0xe6
	lsls r0, r0, #3
	adds r1, r1, r0
	movs r0, #0
	movs r5, #1
	str r0, [sp]
	adds r0, r4, #0
	movs r2, #0
	movs r3, #0
	bl InitActorPart
	str r5, [r4, #0x54]
	ldr r0, _0802E41C @ =gStaticData_087E50D4
	str r0, [r4, #0x50]
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802E414: .4byte gUnknown_030012BC
_0802E418: .4byte gUnknown_030014D8
_0802E41C: .4byte gStaticData_087E50D4

	thumb_func_start sub_802E420
sub_802E420: @ 0x0802E420
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	sub sp, #4
	mov r8, r0
	mov sb, r1
	adds r6, r2, #0
	ldr r0, _0802E478 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	movs r0, #0x58
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	ldr r0, _0802E47C @ =gUnknown_030014D8
	ldr r1, [r0]
	movs r0, #0xe1
	lsls r0, r0, #3
	adds r1, r1, r0
	movs r5, #1
	str r6, [sp]
	adds r0, r4, #0
	mov r2, r8
	mov r3, sb
	bl InitActorPart
	str r5, [r4, #0x54]
	ldr r0, _0802E480 @ =gStaticData_087E510C
	str r0, [r4, #0x50]
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802E478: .4byte gUnknown_030012BC
_0802E47C: .4byte gUnknown_030014D8
_0802E480: .4byte gStaticData_087E510C

	thumb_func_start sub_802E484
sub_802E484: @ 0x0802E484
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r6, r1, #0
	adds r4, r2, #0
	movs r0, #0x64
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E4B4 @ =gUnknown_030014D8
	ldr r1, [r1]
	movs r2, #0xdc
	lsls r2, r2, #3
	adds r1, r1, r2
	str r4, [sp]
	adds r2, r5, #0
	adds r3, r6, #0
	bl sub_8032890
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802E4B4: .4byte gUnknown_030014D8

	thumb_func_start sub_802E4B8
sub_802E4B8: @ 0x0802E4B8
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	sub sp, #8
	adds r4, r0, #0
	mov r8, r1
	mov sb, r2
	adds r5, r3, #0
	ldr r6, [sp, #0x20]
	lsls r4, r4, #0x18
	lsrs r4, r4, #0x18
	movs r0, #0x64
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E500 @ =gUnknown_030014D8
	lsls r2, r4, #2
	adds r2, r2, r4
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	str r5, [sp]
	str r6, [sp, #4]
	mov r2, r8
	mov r3, sb
	bl sub_8031920
	add sp, #8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0802E500: .4byte gUnknown_030014D8

	thumb_func_start sub_802E504
sub_802E504: @ 0x0802E504
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r6, r1, #0
	adds r4, r2, #0
	movs r0, #0x5c
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E534 @ =gUnknown_030014D8
	ldr r1, [r1]
	movs r2, #0x8c
	lsls r2, r2, #2
	adds r1, r1, r2
	str r4, [sp]
	adds r2, r5, #0
	adds r3, r6, #0
	bl sub_80342D4
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802E534: .4byte gUnknown_030014D8

	thumb_func_start sub_802E538
sub_802E538: @ 0x0802E538
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	sub sp, #8
	adds r6, r0, #0
	mov r8, r1
	adds r5, r2, #0
	lsls r4, r3, #0x18
	lsrs r4, r4, #0x18
	movs r0, #0x70
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E578 @ =gUnknown_030014D8
	ldr r1, [r1]
	movs r2, #0x82
	lsls r2, r2, #2
	adds r1, r1, r2
	str r5, [sp]
	add r2, sp, #4
	strb r4, [r2]
	adds r2, r6, #0
	mov r3, r8
	bl sub_8034058
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802E578: .4byte gUnknown_030014D8

	thumb_func_start sub_802E57C
sub_802E57C: @ 0x0802E57C
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r6, r1, #0
	adds r4, r2, #0
	movs r0, #0x70
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E5AC @ =gUnknown_030014D8
	ldr r1, [r1]
	movs r2, #0xf0
	lsls r2, r2, #1
	adds r1, r1, r2
	str r4, [sp]
	adds r2, r5, #0
	adds r3, r6, #0
	bl sub_8033EF4
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802E5AC: .4byte gUnknown_030014D8

	thumb_func_start sub_802E5B0
sub_802E5B0: @ 0x0802E5B0
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r6, r1, #0
	adds r4, r2, #0
	movs r0, #0x70
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E5E0 @ =gUnknown_030014D8
	ldr r1, [r1]
	movs r2, #0xdc
	lsls r2, r2, #1
	adds r1, r1, r2
	str r4, [sp]
	adds r2, r5, #0
	adds r3, r6, #0
	bl sub_8033BB8
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802E5E0: .4byte gUnknown_030014D8

	thumb_func_start sub_802E5E4
sub_802E5E4: @ 0x0802E5E4
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r6, r1, #0
	adds r4, r2, #0
	ldr r0, _0802E624 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x38
	bl PlaySfx
	movs r0, #0x6c
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E628 @ =gUnknown_030014D8
	ldr r1, [r1]
	movs r2, #0xc3
	lsls r2, r2, #3
	adds r1, r1, r2
	str r4, [sp]
	adds r2, r5, #0
	adds r3, r6, #0
	bl sub_80329D4
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802E624: .4byte gUnknown_030012BC
_0802E628: .4byte gUnknown_030014D8

	thumb_func_start sub_802E62C
sub_802E62C: @ 0x0802E62C
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r6, r1, #0
	adds r4, r2, #0
	ldr r0, _0802E66C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x38
	bl PlaySfx
	movs r0, #0x6c
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E670 @ =gUnknown_030014D8
	ldr r1, [r1]
	movs r2, #0xbe
	lsls r2, r2, #3
	adds r1, r1, r2
	str r4, [sp]
	adds r2, r5, #0
	adds r3, r6, #0
	bl sub_80305F8
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802E66C: .4byte gUnknown_030012BC
_0802E670: .4byte gUnknown_030014D8

	thumb_func_start sub_802E674
sub_802E674: @ 0x0802E674
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	sub sp, #0xc
	mov r8, r0
	mov sb, r1
	adds r4, r2, #0
	adds r5, r3, #0
	ldr r6, [sp, #0x24]
	ldr r0, _0802E6C4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x30
	bl PlaySfx
	movs r0, #0x60
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E6C8 @ =gUnknown_030014D8
	ldr r1, [r1]
	adds r1, #0x78
	str r4, [sp]
	str r5, [sp, #4]
	str r6, [sp, #8]
	mov r2, r8
	mov r3, sb
	bl sub_8030300
	add sp, #0xc
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802E6C4: .4byte gUnknown_030012BC
_0802E6C8: .4byte gUnknown_030014D8

	thumb_func_start sub_802E6CC
sub_802E6CC: @ 0x0802E6CC
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	sub sp, #0xc
	mov r8, r0
	mov sb, r1
	adds r4, r2, #0
	adds r5, r3, #0
	ldr r6, [sp, #0x24]
	movs r0, #0x60
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E70C @ =gUnknown_030014D8
	ldr r1, [r1]
	adds r1, #0x50
	str r4, [sp]
	str r5, [sp, #4]
	str r6, [sp, #8]
	mov r2, r8
	mov r3, sb
	bl sub_802FA04
	add sp, #0xc
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802E70C: .4byte gUnknown_030014D8

	thumb_func_start sub_802E710
sub_802E710: @ 0x0802E710
	push {r4, r5, r6, lr}
	adds r5, r1, #0
	ldr r4, _0802E738 @ =gUnknown_030014D8
	str r0, [r4]
	ldr r6, _0802E73C @ =gUnknown_03000884
	movs r0, #0x58
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, [r4]
	adds r2, r5, #0
	bl sub_802E740
	str r0, [r6]
	str r0, [r0, #0x48]
	str r0, [r0, #0x4c]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802E738: .4byte gUnknown_030014D8
_0802E73C: .4byte gUnknown_03000884

	thumb_func_start sub_802E740
sub_802E740: @ 0x0802E740
	push {r4, r5, lr}
	sub sp, #4
	adds r5, r0, #0
	movs r3, #0
	cmp r2, #0
	bne _0802E74E
	ldr r3, _0802E794 @ =0xFFFF6A00
_0802E74E:
	movs r4, #0x64
	str r2, [sp]
	adds r0, r5, #0
	movs r2, #0
	bl InitActorPart
	str r4, [r5, #0x54]
	ldr r0, _0802E798 @ =gStaticData_087E5144
	str r0, [r5, #0x50]
	adds r0, r5, #0
	bl sub_802F338
	ldr r0, _0802E79C @ =gUnknown_0300150C
	movs r2, #0
	str r2, [r0]
	ldr r0, [r5, #0x24]
	cmp r0, #0
	beq _0802E7A4
	ldr r0, _0802E7A0 @ =gUnknown_03001508
	str r2, [r0]
	movs r0, #7
	str r0, [r5, #0x28]
	str r2, [r5, #0x44]
	str r2, [r5, #0xc]
	ldr r0, [r5]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r5, #0x10]
	strb r1, [r5, #0x12]
	str r2, [r5, #8]
	movs r0, #0x28
	bl sub_8029BAC
	b _0802E7B2
	.align 2, 0
_0802E794: .4byte 0xFFFF6A00
_0802E798: .4byte gStaticData_087E5144
_0802E79C: .4byte gUnknown_0300150C
_0802E7A0: .4byte gUnknown_03001508
_0802E7A4:
	movs r0, #0x1e
	bl sub_8029BAC
	ldr r1, _0802E810 @ =gUnknown_03001508
	movs r0, #0xc0
	lsls r0, r0, #1
	str r0, [r1]
_0802E7B2:
	ldr r1, _0802E814 @ =gUnknown_03001507
	movs r0, #0
	strb r0, [r1]
	ldr r1, _0802E818 @ =gUnknown_03001506
	movs r0, #1
	strb r0, [r1]
	ldr r0, _0802E81C @ =gUnknown_03001500
	movs r4, #0
	str r4, [r0]
	ldr r0, _0802E820 @ =gUnknown_03001505
	strb r4, [r0]
	ldr r0, _0802E824 @ =gUnknown_03001504
	strb r4, [r0]
	ldr r0, _0802E828 @ =gUnknown_030014FC
	str r4, [r0]
	ldr r0, _0802E82C @ =gUnknown_030014F8
	str r4, [r0]
	ldr r0, _0802E830 @ =gUnknown_030014F4
	str r4, [r0]
	ldr r1, _0802E834 @ =gUnknown_030014EC
	movs r0, #0xbe
	rsbs r0, r0, #0
	str r0, [r1]
	ldr r0, _0802E838 @ =gUnknown_030014F0
	str r4, [r0]
	ldr r0, _0802E83C @ =gUnknown_030014E8
	strb r4, [r0]
	bl sub_8029794
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802E7F6
	movs r0, #0x78
	str r0, [r5, #0x54]
_0802E7F6:
	ldr r1, _0802E840 @ =gUnknown_030014E4
	ldr r0, [r5, #0x54]
	str r0, [r1]
	ldr r0, _0802E844 @ =gUnknown_030014E0
	str r4, [r0]
	ldr r0, _0802E848 @ =gUnknown_030014DC
	str r4, [r0]
	adds r0, r5, #0
	add sp, #4
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0802E810: .4byte gUnknown_03001508
_0802E814: .4byte gUnknown_03001507
_0802E818: .4byte gUnknown_03001506
_0802E81C: .4byte gUnknown_03001500
_0802E820: .4byte gUnknown_03001505
_0802E824: .4byte gUnknown_03001504
_0802E828: .4byte gUnknown_030014FC
_0802E82C: .4byte gUnknown_030014F8
_0802E830: .4byte gUnknown_030014F4
_0802E834: .4byte gUnknown_030014EC
_0802E838: .4byte gUnknown_030014F0
_0802E83C: .4byte gUnknown_030014E8
_0802E840: .4byte gUnknown_030014E4
_0802E844: .4byte gUnknown_030014E0
_0802E848: .4byte gUnknown_030014DC

	thumb_func_start sub_802E84C
sub_802E84C: @ 0x0802E84C
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r0, _0802E99C @ =gUnknown_030014E0
	ldr r2, [r0]
	cmp r2, #0
	beq _0802E88A
	ldr r3, _0802E9A0 @ =gUnknown_030014DC
	ldr r0, [r3]
	adds r1, r0, #0
	subs r0, #1
	str r0, [r3]
	cmp r1, #0
	bgt _0802E884
	movs r0, #0x16
	str r0, [r3]
	lsls r0, r2, #1
	adds r0, r0, r2
	lsls r2, r0, #4
	movs r0, #0x80
	lsls r0, r0, #1
	cmp r2, r0
	ble _0802E87A
	adds r2, r0, #0
_0802E87A:
	ldr r0, _0802E9A4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x37
	bl PlaySfx
_0802E884:
	ldr r1, _0802E99C @ =gUnknown_030014E0
	movs r0, #0
	str r0, [r1]
_0802E88A:
	ldr r1, _0802E9A8 @ =gUnknown_03001500
	ldr r0, [r1]
	cmp r0, #0
	beq _0802E896
	subs r0, #1
	str r0, [r1]
_0802E896:
	adds r0, r4, #0
	bl sub_802F4CC
	adds r0, r4, #0
	bl sub_802F3BC
	ldr r0, _0802E9AC @ =gUnknown_0300150C
	ldr r1, [r4, #0x1c]
	ldr r0, [r0]
	adds r2, r1, r0
	str r2, [r4, #0x1c]
	ldr r0, _0802E9B0 @ =gUnknown_03001508
	ldr r1, [r4, #0x20]
	ldr r0, [r0]
	adds r3, r1, r0
	str r3, [r4, #0x20]
	ldr r0, _0802E9B4 @ =gUnknown_03001506
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802E8EE
	adds r1, r2, #0
	ldr r0, _0802E9B8 @ =0xFFFF8000
	cmp r1, r0
	bge _0802E8C8
	adds r1, r0, #0
_0802E8C8:
	str r1, [r4, #0x1c]
	movs r0, #0x80
	lsls r0, r0, #8
	cmp r1, r0
	ble _0802E8D4
	adds r1, r0, #0
_0802E8D4:
	str r1, [r4, #0x1c]
	adds r1, r3, #0
	ldr r0, _0802E9BC @ =0xFFFFB500
	cmp r1, r0
	bge _0802E8E0
	adds r1, r0, #0
_0802E8E0:
	str r1, [r4, #0x20]
	movs r0, #0x96
	lsls r0, r0, #7
	cmp r1, r0
	ble _0802E8EC
	adds r1, r0, #0
_0802E8EC:
	str r1, [r4, #0x20]
_0802E8EE:
	ldr r0, _0802E9C0 @ =gUnknown_03001504
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802E908
	movs r0, #0xe0
	lsls r0, r0, #5
	str r0, [r4, #0x34]
	bl sub_8029B2C
	lsls r0, r0, #8
	ldr r1, [r4, #0x34]
	adds r0, r0, r1
	str r0, [r4, #0x24]
_0802E908:
	ldr r3, [r4, #0x34]
	asrs r3, r3, #1
	movs r0, #0xff
	lsls r0, r0, #7
	ands r3, r0
	ldr r1, [r4, #0x20]
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	ldr r0, [r4, #0x1c]
	asrs r2, r0, #0x1f
	eors r0, r2
	subs r0, r0, r2
	adds r1, r1, r0
	asrs r1, r1, #0xb
	movs r0, #0x7f
	ands r1, r0
	orrs r3, r1
	str r3, [r4, #0x14]
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
	blt _0802E96E
	movs r7, #6
	ldrsh r0, [r1, r7]
	subs r0, r2, r0
	lsls r0, r0, #8
	ldr r1, [r4, #8]
	subs r1, r1, r0
	str r1, [r4, #8]
	movs r0, #1
	strb r0, [r4, #0x12]
_0802E96E:
	ldr r0, [r4, #0x1c]
	ldr r1, [r4, #0x20]
	bl sub_8029D8C
	ldr r3, _0802E9C4 @ =gStaticData_0817C1C0
	ldr r0, [r4, #0x28]
	lsls r1, r0, #3
	adds r0, r1, r3
	movs r7, #2
	ldrsh r2, [r0, r7]
	cmp r2, #0
	ble _0802E9C8
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
	b _0802E9CE
	.align 2, 0
_0802E99C: .4byte gUnknown_030014E0
_0802E9A0: .4byte gUnknown_030014DC
_0802E9A4: .4byte gUnknown_030012BC
_0802E9A8: .4byte gUnknown_03001500
_0802E9AC: .4byte gUnknown_0300150C
_0802E9B0: .4byte gUnknown_03001508
_0802E9B4: .4byte gUnknown_03001506
_0802E9B8: .4byte 0xFFFF8000
_0802E9BC: .4byte 0xFFFFB500
_0802E9C0: .4byte gUnknown_03001504
_0802E9C4: .4byte gStaticData_0817C1C0
_0802E9C8:
	adds r0, r3, #4
	adds r0, r1, r0
	ldr r3, [r0]
_0802E9CE:
	ldr r1, _0802E9E8 @ =gStaticData_0817C1C0
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r1
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _0802E9EC
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _0802E9EE
	.align 2, 0
_0802E9E8: .4byte gStaticData_0817C1C0
_0802E9EC:
	adds r0, r1, #0
_0802E9EE:
	adds r0, r4, r0
	bl sub_803AD84
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_802E9FC
sub_802E9FC: @ 0x0802E9FC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	adds r7, r0, #0
	movs r0, #0
	str r0, [sp, #4]
	ldr r2, [r7, #8]
	asrs r2, r2, #8
	ldr r1, [r7, #0xc]
	ldr r3, [r7]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r3
	movs r1, #2
	ldrsh r0, [r0, r1]
	adds r0, r0, r2
	ldr r1, [r7, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	mov sl, r0
	ldrb r0, [r0]
	str r0, [sp, #8]
	lsls r0, r0, #2
	mov sb, r0
	mov r1, sl
	ldrb r1, [r1, #1]
	str r1, [sp, #0xc]
	lsls r1, r1, #2
	mov r8, r1
	ldr r0, [r7, #0x30]
	ldr r4, [r7, #0x34]
	ldr r1, [r0, #0x10]
	cmp r4, r1
	bne _0802EA66
	movs r0, #0x80
	lsls r0, r0, #1
	str r0, [sp]
	bl sub_8029E98
	ldr r1, [r7, #0x20]
	adds r1, r1, r0
	asrs r5, r1, #8
	bl sub_8029EB4
	ldr r1, [r7, #0x1c]
	adds r1, r1, r0
	asrs r6, r1, #8
	b _0802EAB6
_0802EA66:
	lsls r0, r4, #8
	bl sub_803ADB4
	str r0, [sp]
	movs r0, #0xe0
	lsls r0, r0, #0x11
	adds r1, r4, #0
	bl sub_803ADB4
	adds r4, r0, #0
	bl sub_8029E98
	ldr r1, [r7, #0x20]
	muls r1, r4, r1
	asrs r1, r1, #0xc
	adds r1, r1, r0
	asrs r5, r1, #8
	bl sub_8029EB4
	ldr r1, [r7, #0x1c]
	muls r1, r4, r1
	asrs r1, r1, #0xc
	adds r1, r1, r0
	asrs r6, r1, #8
	movs r1, #0x80
	lsls r1, r1, #1
	str r1, [sp, #4]
	ldr r0, [sp]
	cmp r0, #0xff
	bgt _0802EAB6
	movs r0, #0x80
	lsls r0, r0, #2
	orrs r1, r0
	str r1, [sp, #4]
	ldr r1, [sp, #8]
	lsls r1, r1, #3
	mov sb, r1
	ldr r0, [sp, #0xc]
	lsls r0, r0, #3
	mov r8, r0
_0802EAB6:
	mov r1, sb
	subs r6, r6, r1
	mov r0, r8
	subs r5, r5, r0
	cmp r5, #0x9f
	bgt _0802EB4E
	lsls r0, r0, #1
	adds r0, r5, r0
	cmp r0, #0
	blt _0802EB4E
	cmp r6, #0xef
	bgt _0802EB4E
	lsls r0, r1, #1
	adds r0, r6, r0
	cmp r0, #0
	blt _0802EB4E
	ldr r1, [r7, #0xc]
	ldr r2, [r7]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrh r0, [r0, #8]
	lsls r4, r0, #0x10
	mov r0, sl
	bl sub_8029108
	movs r1, #0xff
	ands r5, r1
	ldr r1, _0802EB60 @ =0x000001FF
	ands r6, r1
	lsls r1, r6, #0x10
	orrs r5, r1
	orrs r5, r4
	orrs r5, r0
	ldr r1, [sp, #4]
	orrs r1, r5
	str r1, [sp, #4]
	ldr r4, _0802EB64 @ =gUnknown_03001514
	ldr r0, [r4]
	cmp sl, r0
	beq _0802EB2A
	ldr r2, _0802EB68 @ =gUnknown_03001510
	ldr r0, [r2]
	movs r1, #1
	eors r0, r1
	str r0, [r2]
	ldr r2, _0802EB6C @ =gUnknown_03000874
	ldr r1, _0802EB70 @ =gUnknown_03001518
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r2, [r2]
	mov r1, sl
	bl sub_803AD80
	mov r0, sl
	str r0, [r4]
_0802EB2A:
	ldr r1, _0802EB70 @ =gUnknown_03001518
	ldr r0, _0802EB68 @ =gUnknown_03001510
	ldr r0, [r0]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, _0802EB74 @ =0xF9FF0000
	adds r0, r0, r1
	lsrs r0, r0, #5
	ldr r1, [r7, #0x18]
	lsls r1, r1, #0xc
	orrs r1, r0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	ldr r0, [sp, #4]
	ldr r2, [sp]
	bl sub_8028DD8
_0802EB4E:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802EB60: .4byte 0x000001FF
_0802EB64: .4byte gUnknown_03001514
_0802EB68: .4byte gUnknown_03001510
_0802EB6C: .4byte gUnknown_03000874
_0802EB70: .4byte gUnknown_03001518
_0802EB74: .4byte 0xF9FF0000

	thumb_func_start sub_802EB78
sub_802EB78: @ 0x0802EB78
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x28]
	subs r0, #2
	cmp r0, #1
	bhi _0802EB8A
	ldr r0, [r4, #0x44]
	cmp r0, #0x10
	ble _0802EC5A
_0802EB8A:
	ldr r0, [r4, #0x54]
	subs r0, r0, r1
	str r0, [r4, #0x54]
	ldr r2, _0802EC10 @ =gUnknown_030014F4
	movs r1, #0x12
	str r1, [r2]
	cmp r0, #0
	bgt _0802EC4C
	movs r5, #0
	str r5, [r4, #0x54]
	ldr r0, _0802EC14 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x3a
	bl PlaySfx
	movs r0, #4
	movs r1, #3
	str r0, [r4, #0x28]
	str r5, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0x24]
	movs r6, #0
	strh r0, [r4, #0x10]
	strb r6, [r4, #0x12]
	str r5, [r4, #8]
	ldr r0, _0802EC18 @ =gUnknown_030012C0
	ldr r1, [r0]
	adds r0, r1, #0
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802EBD6
	adds r0, r1, #0
	bl sub_8023234
_0802EBD6:
	ldr r0, _0802EC1C @ =gUnknown_03001507
	strb r6, [r0]
	ldr r0, _0802EC20 @ =gUnknown_030014E8
	movs r1, #1
	strb r1, [r0]
	ldr r0, _0802EC24 @ =gUnknown_03001506
	strb r1, [r0]
	movs r0, #0x1e
	bl sub_8029BAC
	ldr r0, _0802EC28 @ =gUnknown_03001508
	str r5, [r0]
	ldr r3, _0802EC2C @ =gUnknown_0300150C
	ldr r2, [r3]
	asrs r1, r2, #0x1f
	adds r0, r2, #0
	eors r0, r1
	subs r0, r0, r1
	movs r1, #0x90
	lsls r1, r1, #2
	cmp r0, r1
	ble _0802EC34
	cmp r2, #0
	blt _0802EC30
	movs r0, #0
	cmp r2, #0
	beq _0802EC32
	adds r0, r1, #0
	b _0802EC32
	.align 2, 0
_0802EC10: .4byte gUnknown_030014F4
_0802EC14: .4byte gUnknown_030012BC
_0802EC18: .4byte gUnknown_030012C0
_0802EC1C: .4byte gUnknown_03001507
_0802EC20: .4byte gUnknown_030014E8
_0802EC24: .4byte gUnknown_03001506
_0802EC28: .4byte gUnknown_03001508
_0802EC2C: .4byte gUnknown_0300150C
_0802EC30:
	ldr r0, _0802EC44 @ =0xFFFFFDC0
_0802EC32:
	str r0, [r3]
_0802EC34:
	ldr r0, _0802EC48 @ =gUnknown_0300150C
	ldr r1, [r0]
	lsrs r2, r1, #0x1f
	adds r1, r1, r2
	asrs r1, r1, #1
	str r1, [r0]
	b _0802EC5A
	.align 2, 0
_0802EC44: .4byte 0xFFFFFDC0
_0802EC48: .4byte gUnknown_0300150C
_0802EC4C:
	ldr r0, _0802EC60 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x42
	bl PlaySfx
_0802EC5A:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802EC60: .4byte gUnknown_030012BC

	thumb_func_start sub_802EC64
sub_802EC64: @ 0x0802EC64
	ldr r0, _0802EC84 @ =gUnknown_03001507
	ldrb r1, [r0]
	adds r2, r0, #0
	cmp r1, #0
	beq _0802EC90
	ldr r0, _0802EC88 @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #0x40
	ands r0, r1
	cmp r0, #0
	beq _0802EC90
	ldr r1, _0802EC8C @ =gUnknown_03001508
	ldr r0, [r1]
	subs r0, #0x40
	b _0802ECA8
	.align 2, 0
_0802EC84: .4byte gUnknown_03001507
_0802EC88: .4byte gUnknown_030007E0
_0802EC8C: .4byte gUnknown_03001508
_0802EC90:
	ldrb r0, [r2]
	cmp r0, #0
	beq _0802ECB8
	ldr r0, _0802ECB0 @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #0x80
	ands r0, r1
	cmp r0, #0
	beq _0802ECB8
	ldr r1, _0802ECB4 @ =gUnknown_03001508
	ldr r0, [r1]
	adds r0, #0x40
_0802ECA8:
	str r0, [r1]
	adds r3, r1, #0
	b _0802ECE4
	.align 2, 0
_0802ECB0: .4byte gUnknown_030007E0
_0802ECB4: .4byte gUnknown_03001508
_0802ECB8:
	ldr r1, _0802ECCC @ =gUnknown_03001508
	ldr r0, [r1]
	adds r3, r1, #0
	cmp r0, #0
	blt _0802ECD0
	cmp r0, #0
	beq _0802ECD2
	subs r0, #0x40
	b _0802ECD2
	.align 2, 0
_0802ECCC: .4byte gUnknown_03001508
_0802ECD0:
	adds r0, #0x40
_0802ECD2:
	str r0, [r1]
	ldr r0, [r3]
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	cmp r0, #0x40
	bgt _0802ECE4
	movs r0, #0
	str r0, [r3]
_0802ECE4:
	ldr r2, [r3]
	asrs r1, r2, #0x1f
	adds r0, r2, #0
	eors r0, r1
	subs r0, r0, r1
	movs r1, #0x90
	lsls r1, r1, #2
	cmp r0, r1
	ble _0802ED0A
	adds r0, r3, #0
	cmp r2, #0
	blt _0802ED06
	movs r3, #0
	cmp r2, #0
	beq _0802ED08
	adds r3, r1, #0
	b _0802ED08
_0802ED06:
	ldr r3, _0802ED0C @ =0xFFFFFDC0
_0802ED08:
	str r3, [r0]
_0802ED0A:
	bx lr
	.align 2, 0
_0802ED0C: .4byte 0xFFFFFDC0

	thumb_func_start sub_802ED10
sub_802ED10: @ 0x0802ED10
	ldr r0, _0802ED30 @ =gUnknown_03001507
	ldrb r1, [r0]
	adds r2, r0, #0
	cmp r1, #0
	beq _0802ED3C
	ldr r0, _0802ED34 @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #0x20
	ands r0, r1
	cmp r0, #0
	beq _0802ED3C
	ldr r1, _0802ED38 @ =gUnknown_0300150C
	ldr r0, [r1]
	subs r0, #0x40
	b _0802ED54
	.align 2, 0
_0802ED30: .4byte gUnknown_03001507
_0802ED34: .4byte gUnknown_030007E0
_0802ED38: .4byte gUnknown_0300150C
_0802ED3C:
	ldrb r0, [r2]
	cmp r0, #0
	beq _0802ED64
	ldr r0, _0802ED5C @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #0x10
	ands r0, r1
	cmp r0, #0
	beq _0802ED64
	ldr r1, _0802ED60 @ =gUnknown_0300150C
	ldr r0, [r1]
	adds r0, #0x40
_0802ED54:
	str r0, [r1]
	adds r3, r1, #0
	b _0802ED90
	.align 2, 0
_0802ED5C: .4byte gUnknown_030007E0
_0802ED60: .4byte gUnknown_0300150C
_0802ED64:
	ldr r1, _0802ED78 @ =gUnknown_0300150C
	ldr r0, [r1]
	adds r3, r1, #0
	cmp r0, #0
	blt _0802ED7C
	cmp r0, #0
	beq _0802ED7E
	subs r0, #0x40
	b _0802ED7E
	.align 2, 0
_0802ED78: .4byte gUnknown_0300150C
_0802ED7C:
	adds r0, #0x40
_0802ED7E:
	str r0, [r1]
	ldr r0, [r3]
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	cmp r0, #0x40
	bgt _0802ED90
	movs r0, #0
	str r0, [r3]
_0802ED90:
	ldr r2, [r3]
	asrs r1, r2, #0x1f
	adds r0, r2, #0
	eors r0, r1
	subs r0, r0, r1
	movs r1, #0x90
	lsls r1, r1, #2
	cmp r0, r1
	ble _0802EDB6
	adds r0, r3, #0
	cmp r2, #0
	blt _0802EDB2
	movs r3, #0
	cmp r2, #0
	beq _0802EDB4
	adds r3, r1, #0
	b _0802EDB4
_0802EDB2:
	ldr r3, _0802EDB8 @ =0xFFFFFDC0
_0802EDB4:
	str r3, [r0]
_0802EDB6:
	bx lr
	.align 2, 0
_0802EDB8: .4byte 0xFFFFFDC0

	thumb_func_start sub_802EDBC
sub_802EDBC: @ 0x0802EDBC
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r0, #0
	bl sub_802EC64
	adds r0, r4, #0
	bl sub_802ED10
	ldr r0, _0802EE18 @ =gUnknown_03001507
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802EEB8
	ldr r0, _0802EE1C @ =gUnknown_030007E0
	ldr r2, [r0]
	movs r0, #0x80
	lsls r0, r0, #2
	adds r1, r0, #0
	adds r0, r2, #0
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r5, r0, #0x10
	cmp r5, #0
	beq _0802EE28
	ldr r1, _0802EE20 @ =gUnknown_030014F4
	movs r0, #0x12
	str r0, [r1]
	ldr r0, _0802EE24 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xa
	bl PlaySfx
	movs r0, #2
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
	b _0802EEB8
	.align 2, 0
_0802EE18: .4byte gUnknown_03001507
_0802EE1C: .4byte gUnknown_030007E0
_0802EE20: .4byte gUnknown_030014F4
_0802EE24: .4byte gUnknown_030012BC
_0802EE28:
	movs r0, #0x80
	lsls r0, r0, #1
	adds r1, r0, #0
	adds r0, r2, #0
	ands r0, r1
	cmp r0, #0
	beq _0802EE6C
	ldr r1, _0802EE64 @ =gUnknown_030014F4
	movs r0, #0x12
	str r0, [r1]
	ldr r0, _0802EE68 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xa
	bl PlaySfx
	movs r0, #3
	movs r1, #2
	str r0, [r4, #0x28]
	str r5, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0x18]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
	b _0802EEB8
	.align 2, 0
_0802EE64: .4byte gUnknown_030014F4
_0802EE68: .4byte gUnknown_030012BC
_0802EE6C:
	ldr r1, _0802EEC0 @ =gUnknown_03001500
	ldr r0, [r1]
	cmp r0, #0
	bne _0802EEB8
	movs r3, #1
	ands r2, r3
	cmp r2, #0
	beq _0802EEB8
	movs r0, #0x12
	str r0, [r1]
	ldr r0, _0802EEC4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0xfa
	lsls r2, r2, #2
	mov r1, sp
	strb r3, [r1]
	movs r1, #0x24
	movs r3, #0xa0
	bl sub_80019F8
	ldr r0, [r4, #0x1c]
	movs r1, #0x90
	lsls r1, r1, #5
	adds r0, r0, r1
	ldr r1, [r4, #0x20]
	ldr r2, _0802EEC8 @ =0xFFFFE800
	adds r1, r1, r2
	ldr r2, [r4, #0x24]
	adds r2, #0xa
	ldr r4, _0802EECC @ =0x00000199
	adds r3, r0, #0
	muls r3, r4, r3
	asrs r3, r3, #0xc
	muls r4, r1, r4
	asrs r4, r4, #0xc
	str r4, [sp]
	bl sub_802E6CC
_0802EEB8:
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802EEC0: .4byte gUnknown_03001500
_0802EEC4: .4byte gUnknown_030012BC
_0802EEC8: .4byte 0xFFFFE800
_0802EECC: .4byte 0x00000199

	thumb_func_start sub_802EED0
sub_802EED0: @ 0x0802EED0
	push {r4, r5, lr}
	adds r4, r0, #0
	bl sub_802EC64
	ldr r0, [r4, #0x44]
	cmp r0, #5
	bgt _0802EF00
	ldr r1, _0802EEF4 @ =gUnknown_0300150C
	ldr r0, [r1]
	ldr r2, _0802EEF8 @ =0xFFFFFF00
	adds r0, r0, r2
	str r0, [r1]
	ldr r2, _0802EEFC @ =0xFFFFFB00
	cmp r0, r2
	bge _0802EF16
	str r2, [r1]
	b _0802EF16
	.align 2, 0
_0802EEF4: .4byte gUnknown_0300150C
_0802EEF8: .4byte 0xFFFFFF00
_0802EEFC: .4byte 0xFFFFFB00
_0802EF00:
	ldr r2, _0802EF68 @ =gUnknown_0300150C
	ldr r0, [r2]
	adds r0, #0x2d
	str r0, [r2]
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	cmp r0, #0x2d
	bgt _0802EF16
	movs r0, #0
	str r0, [r2]
_0802EF16:
	ldr r0, [r4, #0x44]
	cmp r0, #0x21
	ble _0802EFAE
	adds r0, r4, #0
	bl sub_802ED10
	ldr r0, _0802EF6C @ =gUnknown_030007E0
	ldr r2, [r0]
	movs r0, #0x80
	lsls r0, r0, #2
	adds r1, r0, #0
	adds r0, r2, #0
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r5, r0, #0x10
	cmp r5, #0
	beq _0802EF78
	ldr r1, _0802EF70 @ =gUnknown_030014F4
	movs r0, #0x12
	str r0, [r1]
	ldr r0, _0802EF74 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xa
	bl PlaySfx
	movs r0, #2
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
	b _0802EFAE
	.align 2, 0
_0802EF68: .4byte gUnknown_0300150C
_0802EF6C: .4byte gUnknown_030007E0
_0802EF70: .4byte gUnknown_030014F4
_0802EF74: .4byte gUnknown_030012BC
_0802EF78:
	movs r1, #0x80
	lsls r1, r1, #1
	adds r0, r1, #0
	ands r2, r0
	cmp r2, #0
	beq _0802EFAE
	ldr r1, _0802EFD0 @ =gUnknown_030014F4
	movs r0, #0x12
	str r0, [r1]
	ldr r0, _0802EFD4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xa
	bl PlaySfx
	movs r0, #3
	movs r1, #2
	str r0, [r4, #0x28]
	str r5, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0x18]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
_0802EFAE:
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _0802EFCA
	movs r0, #1
	movs r2, #0
	str r0, [r4, #0x28]
	str r2, [r4, #0x44]
	str r2, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
_0802EFCA:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802EFD0: .4byte gUnknown_030014F4
_0802EFD4: .4byte gUnknown_030012BC

	thumb_func_start sub_802EFD8
sub_802EFD8: @ 0x0802EFD8
	push {r4, r5, lr}
	adds r4, r0, #0
	bl sub_802EC64
	ldr r0, [r4, #0x44]
	cmp r0, #5
	bgt _0802F004
	ldr r1, _0802F000 @ =gUnknown_0300150C
	ldr r0, [r1]
	movs r2, #0x80
	lsls r2, r2, #1
	adds r0, r0, r2
	str r0, [r1]
	movs r2, #0xa0
	lsls r2, r2, #3
	cmp r0, r2
	ble _0802F01A
	str r2, [r1]
	b _0802F01A
	.align 2, 0
_0802F000: .4byte gUnknown_0300150C
_0802F004:
	ldr r2, _0802F06C @ =gUnknown_0300150C
	ldr r0, [r2]
	subs r0, #0x2d
	str r0, [r2]
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	cmp r0, #0x2d
	bgt _0802F01A
	movs r0, #0
	str r0, [r2]
_0802F01A:
	ldr r0, [r4, #0x44]
	cmp r0, #0x21
	ble _0802F0B2
	adds r0, r4, #0
	bl sub_802ED10
	ldr r0, _0802F070 @ =gUnknown_030007E0
	ldr r2, [r0]
	movs r0, #0x80
	lsls r0, r0, #2
	adds r1, r0, #0
	adds r0, r2, #0
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r5, r0, #0x10
	cmp r5, #0
	beq _0802F07C
	ldr r1, _0802F074 @ =gUnknown_030014F4
	movs r0, #0x12
	str r0, [r1]
	ldr r0, _0802F078 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xa
	bl PlaySfx
	movs r0, #2
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
	b _0802F0B2
	.align 2, 0
_0802F06C: .4byte gUnknown_0300150C
_0802F070: .4byte gUnknown_030007E0
_0802F074: .4byte gUnknown_030014F4
_0802F078: .4byte gUnknown_030012BC
_0802F07C:
	movs r1, #0x80
	lsls r1, r1, #1
	adds r0, r1, #0
	ands r2, r0
	cmp r2, #0
	beq _0802F0B2
	ldr r1, _0802F0D4 @ =gUnknown_030014F4
	movs r0, #0x12
	str r0, [r1]
	ldr r0, _0802F0D8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xa
	bl PlaySfx
	movs r0, #3
	movs r1, #2
	str r0, [r4, #0x28]
	str r5, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0x18]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
_0802F0B2:
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _0802F0CE
	movs r0, #1
	movs r2, #0
	str r0, [r4, #0x28]
	str r2, [r4, #0x44]
	str r2, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
_0802F0CE:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802F0D4: .4byte gUnknown_030014F4
_0802F0D8: .4byte gUnknown_030012BC

	thumb_func_start sub_802F0DC
sub_802F0DC: @ 0x0802F0DC
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r2, _0802F144 @ =gUnknown_03001506
	ldrb r5, [r2]
	cmp r5, #0
	bne _0802F13C
	ldr r0, _0802F148 @ =gUnknown_03001507
	strb r5, [r0]
	ldr r1, _0802F14C @ =gUnknown_03001504
	movs r0, #1
	strb r0, [r1]
	strb r0, [r2]
	movs r0, #0x3c
	bl sub_8029BAC
	ldr r0, _0802F150 @ =gUnknown_03001508
	str r5, [r0]
	ldr r0, _0802F154 @ =gUnknown_0300150C
	str r5, [r0]
	movs r0, #5
	movs r1, #4
	str r0, [r4, #0x28]
	str r5, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0x30]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
	ldr r0, _0802F158 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x3b
	bl PlaySfx
	ldr r0, _0802F15C @ =gUnknown_030012C0
	ldr r2, [r0]
	adds r0, r2, #0
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802F13C
	ldr r1, _0802F160 @ =0x00002710
	adds r0, r2, #0
	bl sub_8022EA8
_0802F13C:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802F144: .4byte gUnknown_03001506
_0802F148: .4byte gUnknown_03001507
_0802F14C: .4byte gUnknown_03001504
_0802F150: .4byte gUnknown_03001508
_0802F154: .4byte gUnknown_0300150C
_0802F158: .4byte gUnknown_030012BC
_0802F15C: .4byte gUnknown_030012C0
_0802F160: .4byte 0x00002710

	thumb_func_start sub_802F164
sub_802F164: @ 0x0802F164
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	adds r3, r1, #0
	adds r4, r2, #0
	ldr r0, [r5, #0x28]
	cmp r0, #1
	beq _0802F180
	cmp r0, #6
	beq _0802F180
	cmp r0, #2
	beq _0802F180
	cmp r0, #3
	beq _0802F180
	b _0802F322
_0802F180:
	ldr r0, [r5, #0xc]
	cmp r0, #5
	beq _0802F198
	movs r0, #5
	str r0, [r5, #0xc]
	ldr r0, [r5]
	ldrh r0, [r0, #0x3c]
	movs r1, #0
	movs r2, #0
	strh r0, [r5, #0x10]
	strb r1, [r5, #0x12]
	str r2, [r5, #8]
_0802F198:
	str r3, [r5, #0x1c]
	str r4, [r5, #0x20]
	ldr r0, [r5, #0x28]
	cmp r0, #6
	beq _0802F1A8
	movs r0, #0x50
	bl sub_8029BAC
_0802F1A8:
	movs r0, #6
	str r0, [r5, #0x28]
	ldr r2, _0802F1FC @ =gUnknown_03001508
	ldr r1, _0802F200 @ =gUnknown_0300150C
	movs r0, #0
	str r0, [r1]
	str r0, [r2]
	str r0, [r5, #0x44]
	ldr r0, _0802F204 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r0, #0x8c
	ldrb r6, [r0]
	cmp r6, #0
	beq _0802F1C6
	b _0802F322
_0802F1C6:
	bl sub_802A4D4
	ldr r4, _0802F208 @ =gUnknown_030014EC
	ldr r1, [r4]
	subs r0, r0, r1
	cmp r0, #0x14
	bgt _0802F1D6
	b _0802F322
_0802F1D6:
	bl sub_802A4D4
	ldr r1, [r4]
	subs r0, r0, r1
	cmp r0, #0xbe
	ble _0802F1E6
	ldr r0, _0802F20C @ =gUnknown_030014F0
	str r6, [r0]
_0802F1E6:
	ldr r0, _0802F20C @ =gUnknown_030014F0
	ldr r0, [r0]
	cmp r0, #4
	bls _0802F1F0
	b _0802F30A
_0802F1F0:
	lsls r0, r0, #2
	ldr r1, _0802F210 @ =_0802F214
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0802F1FC: .4byte gUnknown_03001508
_0802F200: .4byte gUnknown_0300150C
_0802F204: .4byte gUnknown_030012C0
_0802F208: .4byte gUnknown_030014EC
_0802F20C: .4byte gUnknown_030014F0
_0802F210: .4byte _0802F214
_0802F214: @ jump table
	.4byte _0802F228 @ case 0
	.4byte _0802F258 @ case 1
	.4byte _0802F288 @ case 2
	.4byte _0802F2B8 @ case 3
	.4byte _0802F2E8 @ case 4
_0802F228:
	ldr r0, _0802F24C @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802F30A
	ldr r2, _0802F250 @ =gUnknown_030014FC
	ldr r0, [r2]
	cmp r0, #0
	bne _0802F242
	ldr r1, _0802F254 @ =gUnknown_030014F8
	movs r0, #0xf
	str r0, [r1]
_0802F242:
	ldr r0, [r2]
	adds r0, #1
	str r0, [r2]
	b _0802F30A
	.align 2, 0
_0802F24C: .4byte gUnknown_030012C0
_0802F250: .4byte gUnknown_030014FC
_0802F254: .4byte gUnknown_030014F8
_0802F258:
	ldr r0, _0802F27C @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802F30A
	ldr r2, _0802F280 @ =gUnknown_030014FC
	ldr r0, [r2]
	cmp r0, #0
	bne _0802F272
	ldr r1, _0802F284 @ =gUnknown_030014F8
	movs r0, #0xf
	str r0, [r1]
_0802F272:
	ldr r0, [r2]
	adds r0, #5
	str r0, [r2]
	b _0802F30A
	.align 2, 0
_0802F27C: .4byte gUnknown_030012C0
_0802F280: .4byte gUnknown_030014FC
_0802F284: .4byte gUnknown_030014F8
_0802F288:
	ldr r0, _0802F2AC @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802F30A
	ldr r2, _0802F2B0 @ =gUnknown_030014FC
	ldr r0, [r2]
	cmp r0, #0
	bne _0802F2A2
	ldr r1, _0802F2B4 @ =gUnknown_030014F8
	movs r0, #0xf
	str r0, [r1]
_0802F2A2:
	ldr r0, [r2]
	adds r0, #0x14
	str r0, [r2]
	b _0802F30A
	.align 2, 0
_0802F2AC: .4byte gUnknown_030012C0
_0802F2B0: .4byte gUnknown_030014FC
_0802F2B4: .4byte gUnknown_030014F8
_0802F2B8:
	ldr r0, _0802F2E0 @ =gUnknown_03001506
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802F30A
	ldr r4, _0802F2E4 @ =gUnknown_030014E4
	ldr r1, [r4]
	movs r0, #0x14
	muls r0, r1, r0
	movs r1, #0x64
	bl sub_803ADB4
	ldr r1, [r5, #0x54]
	adds r1, r1, r0
	str r1, [r5, #0x54]
	ldr r4, [r4]
	cmp r1, r4
	ble _0802F30A
	str r4, [r5, #0x54]
	b _0802F30A
	.align 2, 0
_0802F2E0: .4byte gUnknown_03001506
_0802F2E4: .4byte gUnknown_030014E4
_0802F2E8:
	ldr r0, _0802F328 @ =gUnknown_030012C0
	ldr r1, [r0]
	adds r0, r1, #0
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802F30A
	adds r0, r1, #0
	bl sub_8023464
	ldr r0, _0802F32C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #7
	bl PlaySfx
_0802F30A:
	bl sub_802A4D4
	ldr r1, _0802F330 @ =gUnknown_030014EC
	str r0, [r1]
	ldr r1, _0802F334 @ =gUnknown_030014F0
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
	cmp r0, #5
	bne _0802F322
	movs r0, #0
	str r0, [r1]
_0802F322:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802F328: .4byte gUnknown_030012C0
_0802F32C: .4byte gUnknown_030012BC
_0802F330: .4byte gUnknown_030014EC
_0802F334: .4byte gUnknown_030014F0

	thumb_func_start sub_802F338
sub_802F338: @ 0x0802F338
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r2, [r4, #8]
	asrs r2, r2, #8
	ldr r1, [r4, #0xc]
	ldr r3, [r4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r3
	movs r1, #2
	ldrsh r0, [r0, r1]
	adds r0, r0, r2
	ldr r1, [r4, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldrb r3, [r0]
	ldrb r1, [r0, #1]
	adds r2, r3, #0
	muls r2, r1, r2
	adds r0, r2, #0
	lsls r0, r0, #5
	bl sub_8028CD4
	ldr r5, _0802F3B0 @ =gUnknown_03001518
	str r0, [r5]
	ldr r2, [r4, #8]
	asrs r2, r2, #8
	ldr r1, [r4, #0xc]
	ldr r3, [r4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r3
	movs r3, #2
	ldrsh r0, [r0, r3]
	adds r0, r0, r2
	ldr r1, [r4, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldrb r2, [r0]
	ldrb r3, [r0, #1]
	adds r1, r2, #0
	muls r1, r3, r1
	adds r0, r1, #0
	lsls r0, r0, #5
	bl sub_8028CD4
	str r0, [r5, #4]
	ldr r1, _0802F3B4 @ =gUnknown_03001510
	movs r0, #1
	str r0, [r1]
	ldr r1, _0802F3B8 @ =gUnknown_03001514
	movs r0, #0
	str r0, [r1]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802F3B0: .4byte gUnknown_03001518
_0802F3B4: .4byte gUnknown_03001510
_0802F3B8: .4byte gUnknown_03001514

	thumb_func_start sub_802F3BC
sub_802F3BC: @ 0x0802F3BC
	push {r4, r5, lr}
	adds r1, r0, #0
	ldr r4, _0802F3E4 @ =gUnknown_030014FC
	ldr r2, [r4]
	cmp r2, #0
	beq _0802F462
	ldr r0, _0802F3E8 @ =gUnknown_03001506
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802F3F0
	ldr r5, _0802F3EC @ =gUnknown_030012C0
_0802F3D2:
	ldr r0, [r5]
	bl sub_8023430
	ldr r0, [r4]
	subs r0, #1
	str r0, [r4]
	cmp r0, #0
	bne _0802F3D2
	b _0802F462
	.align 2, 0
_0802F3E4: .4byte gUnknown_030014FC
_0802F3E8: .4byte gUnknown_03001506
_0802F3EC: .4byte gUnknown_030012C0
_0802F3F0:
	ldr r3, _0802F400 @ =gUnknown_030014F8
	ldr r0, [r3]
	cmp r0, #0
	beq _0802F404
	subs r0, #1
	str r0, [r3]
	b _0802F462
	.align 2, 0
_0802F400: .4byte gUnknown_030014F8
_0802F404:
	movs r0, #0xf
	str r0, [r3]
	cmp r2, #9
	bgt _0802F41C
	ldr r0, [r1, #0x1c]
	ldr r1, [r1, #0x20]
	movs r2, #1
	bl sub_802E484
	ldr r0, [r4]
	subs r0, #1
	b _0802F452
_0802F41C:
	cmp r2, #0x13
	bgt _0802F430
	ldr r0, [r1, #0x1c]
	ldr r1, [r1, #0x20]
	movs r2, #2
	bl sub_802E484
	ldr r0, [r4]
	subs r0, #2
	b _0802F452
_0802F430:
	cmp r2, #0x27
	bgt _0802F444
	ldr r0, [r1, #0x1c]
	ldr r1, [r1, #0x20]
	movs r2, #4
	bl sub_802E484
	ldr r0, [r4]
	subs r0, #4
	b _0802F452
_0802F444:
	ldr r0, [r1, #0x1c]
	ldr r1, [r1, #0x20]
	movs r2, #8
	bl sub_802E484
	ldr r0, [r4]
	subs r0, #8
_0802F452:
	str r0, [r4]
	ldr r0, _0802F468 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #8
	bl PlaySfx
_0802F462:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802F468: .4byte gUnknown_030012BC

	thumb_func_start sub_802F46C
sub_802F46C: @ 0x0802F46C
	ldr r1, _0802F478 @ =gUnknown_030014E0
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
	bx lr
	.align 2, 0
_0802F478: .4byte gUnknown_030014E0

	thumb_func_start sub_802F47C
sub_802F47C: @ 0x0802F47C
	push {r4, lr}
	adds r1, r0, #0
	ldr r0, _0802F48C @ =gUnknown_030014E4
	ldr r0, [r0]
	cmp r0, #0x64
	bne _0802F490
	ldr r0, [r1, #0x54]
	b _0802F4A6
	.align 2, 0
_0802F48C: .4byte gUnknown_030014E4
_0802F490:
	ldr r4, [r1, #0x54]
	movs r0, #0x64
	muls r0, r4, r0
	movs r1, #0x78
	bl sub_803ADB4
	cmp r0, #0
	bne _0802F4A6
	cmp r4, #0
	ble _0802F4A6
	movs r0, #1
_0802F4A6:
	pop {r4}
	pop {r1}
	bx r1

	thumb_func_start sub_802F4AC
sub_802F4AC: @ 0x0802F4AC
	push {lr}
	ldr r0, [r0, #0x24]
	movs r1, #0xf0
	lsls r1, r1, #7
	adds r0, r0, r1
	bl sub_8029748
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_802F4C0
sub_802F4C0: @ 0x0802F4C0
	ldr r0, _0802F4C8 @ =gUnknown_030014E8
	ldrb r0, [r0]
	bx lr
	.align 2, 0
_0802F4C8: .4byte gUnknown_030014E8

	thumb_func_start sub_802F4CC
sub_802F4CC: @ 0x0802F4CC
	push {lr}
	ldr r1, _0802F500 @ =gUnknown_030014F4
	ldr r0, [r1]
	cmp r0, #0
	beq _0802F4FA
	subs r0, #1
	str r0, [r1]
	movs r1, #3
	bl sub_803ADB4
	adds r1, r0, #0
	cmp r1, #2
	ble _0802F4EA
	movs r0, #5
	subs r1, r0, r1
_0802F4EA:
	lsls r0, r1, #5
	ldr r1, _0802F504 @ =gStaticData_0817C200
	adds r0, r0, r1
	ldr r1, _0802F508 @ =0x05000200
	movs r2, #0x20
	movs r3, #0x10
	bl QueueVramDmaTransfer
_0802F4FA:
	pop {r0}
	bx r0
	.align 2, 0
_0802F500: .4byte gUnknown_030014F4
_0802F504: .4byte gStaticData_0817C200
_0802F508: .4byte 0x05000200

	thumb_func_start sub_802F50C
sub_802F50C: @ 0x0802F50C
	push {r4, r5, lr}
	adds r5, r0, #0
	ldr r0, _0802F538 @ =gUnknown_03001506
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802F532
	ldr r0, _0802F53C @ =gUnknown_030014E4
	ldr r4, [r0]
	adds r0, r1, #0
	muls r0, r4, r0
	movs r1, #0x64
	bl sub_803ADB4
	ldr r1, [r5, #0x54]
	adds r1, r1, r0
	str r1, [r5, #0x54]
	cmp r1, r4
	ble _0802F532
	str r4, [r5, #0x54]
_0802F532:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802F538: .4byte gUnknown_03001506
_0802F53C: .4byte gUnknown_030014E4

	thumb_func_start sub_802F540
sub_802F540: @ 0x0802F540
	adds r3, r1, #0
	ldr r0, _0802F564 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802F562
	ldr r2, _0802F568 @ =gUnknown_030014FC
	ldr r0, [r2]
	cmp r0, #0
	bne _0802F55C
	ldr r1, _0802F56C @ =gUnknown_030014F8
	movs r0, #0xf
	str r0, [r1]
_0802F55C:
	ldr r0, [r2]
	adds r0, r0, r3
	str r0, [r2]
_0802F562:
	bx lr
	.align 2, 0
_0802F564: .4byte gUnknown_030012C0
_0802F568: .4byte gUnknown_030014FC
_0802F56C: .4byte gUnknown_030014F8

	thumb_func_start sub_802F570
sub_802F570: @ 0x0802F570
	push {r4, r5, lr}
	adds r2, r0, #0
	ldrb r0, [r2, #0x12]
	cmp r0, #0
	beq _0802F59E
	movs r5, #1
	movs r1, #0
	str r5, [r2, #0x28]
	str r1, [r2, #0x44]
	str r1, [r2, #0xc]
	ldr r0, [r2]
	ldrh r0, [r0]
	movs r4, #0
	strh r0, [r2, #0x10]
	strb r4, [r2, #0x12]
	str r1, [r2, #8]
	movs r0, #0x28
	bl sub_8029BAC
	ldr r0, _0802F5A4 @ =gUnknown_03001507
	strb r5, [r0]
	ldr r0, _0802F5A8 @ =gUnknown_03001506
	strb r4, [r0]
_0802F59E:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802F5A4: .4byte gUnknown_03001507
_0802F5A8: .4byte gUnknown_03001506

	thumb_func_start sub_802F5AC
sub_802F5AC: @ 0x0802F5AC
	push {lr}
	adds r3, r0, #0
	ldr r0, [r3, #0xc]
	cmp r0, #5
	bne _0802F5CC
	ldrb r0, [r3, #0x12]
	cmp r0, #0
	beq _0802F5CC
	movs r2, #0
	str r2, [r3, #0xc]
	ldr r0, [r3]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r3, #0x10]
	strb r1, [r3, #0x12]
	str r2, [r3, #8]
_0802F5CC:
	ldr r0, [r3, #0x44]
	cmp r0, #0x32
	bne _0802F5E0
	movs r0, #1
	str r0, [r3, #0x28]
	movs r0, #0
	str r0, [r3, #0x44]
	movs r0, #0x28
	bl sub_8029BAC
_0802F5E0:
	pop {r0}
	bx r0

	thumb_func_start sub_802F5E4
sub_802F5E4: @ 0x0802F5E4
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r2, _0802F638 @ =gUnknown_03001508
	ldr r0, [r2]
	adds r0, #9
	str r0, [r2]
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	movs r1, #0xa0
	lsls r1, r1, #1
	cmp r0, r1
	ble _0802F600
	str r1, [r2]
_0802F600:
	ldr r5, _0802F63C @ =gUnknown_03001505
	ldrb r0, [r5]
	cmp r0, #0
	bne _0802F620
	ldr r1, [r4, #0x20]
	movs r0, #0xe1
	lsls r0, r0, #7
	cmp r1, r0
	ble _0802F620
	movs r0, #0
	movs r1, #2
	movs r2, #1
	bl sub_800132C
	movs r0, #1
	strb r0, [r5]
_0802F620:
	ldr r1, [r4, #0x20]
	movs r0, #0xe1
	lsls r0, r0, #8
	cmp r1, r0
	ble _0802F630
	movs r0, #3
	bl sub_802A668
_0802F630:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802F638: .4byte gUnknown_03001508
_0802F63C: .4byte gUnknown_03001505

	thumb_func_start sub_802F640
sub_802F640: @ 0x0802F640
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x24]
	movs r1, #0x80
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x24]
	bl sub_8029B2C
	lsls r0, r0, #8
	ldr r1, [r4, #0x24]
	subs r1, r1, r0
	str r1, [r4, #0x34]
	ldr r5, _0802F694 @ =gUnknown_03001505
	ldrb r0, [r5]
	cmp r0, #0
	bne _0802F67C
	movs r0, #0x82
	lsls r0, r0, #8
	cmp r1, r0
	ble _0802F67C
	movs r0, #0
	movs r1, #2
	movs r2, #1
	bl sub_800132C
	movs r1, #1
	strb r1, [r5]
	ldr r0, _0802F698 @ =gUnknown_030014E8
	strb r1, [r0]
_0802F67C:
	ldr r1, [r4, #0x34]
	movs r0, #0xa0
	lsls r0, r0, #8
	cmp r1, r0
	ble _0802F68C
	movs r0, #1
	bl sub_802A668
_0802F68C:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802F694: .4byte gUnknown_03001505
_0802F698: .4byte gUnknown_030014E8

	thumb_func_start sub_802F69C
sub_802F69C: @ 0x0802F69C
	push {r4, r5, lr}
	adds r2, r0, #0
	ldr r1, [r2, #0x20]
	movs r0, #0xf0
	lsls r0, r0, #5
	cmp r1, r0
	ble _0802F6CE
	movs r5, #1
	movs r1, #0
	str r5, [r2, #0x28]
	str r1, [r2, #0x44]
	str r1, [r2, #0xc]
	ldr r0, [r2]
	ldrh r0, [r0]
	movs r4, #0
	strh r0, [r2, #0x10]
	strb r4, [r2, #0x12]
	str r1, [r2, #8]
	movs r0, #0x28
	bl sub_8029BAC
	ldr r0, _0802F6D4 @ =gUnknown_03001507
	strb r5, [r0]
	ldr r0, _0802F6D8 @ =gUnknown_03001506
	strb r4, [r0]
_0802F6CE:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802F6D4: .4byte gUnknown_03001507
_0802F6D8: .4byte gUnknown_03001506

	thumb_func_start sub_802F6DC
sub_802F6DC: @ 0x0802F6DC
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	adds r7, r1, #0
	ldr r0, _0802F734 @ =gStaticData_087E5144
	str r0, [r5, #0x50]
	ldr r1, _0802F738 @ =gUnknown_030014FC
	ldr r0, [r1]
	cmp r0, #0
	beq _0802F702
	ldr r6, _0802F73C @ =gUnknown_030012C0
	adds r4, r1, #0
_0802F6F2:
	ldr r0, [r6]
	bl sub_8023430
	ldr r0, [r4]
	subs r0, #1
	str r0, [r4]
	cmp r0, #0
	bne _0802F6F2
_0802F702:
	ldr r4, _0802F740 @ =gUnknown_03001518
	ldr r0, [r4]
	bl sub_8028C48
	ldr r0, [r4, #4]
	bl sub_8028C48
	ldr r0, _0802F744 @ =gStaticData_087E4DF4
	str r0, [r5, #0x50]
	ldr r1, [r5, #0x4c]
	ldr r0, [r5, #0x48]
	str r0, [r1, #0x48]
	ldr r1, [r5, #0x48]
	ldr r0, [r5, #0x4c]
	str r0, [r1, #0x4c]
	movs r0, #1
	ands r0, r7
	cmp r0, #0
	beq _0802F72E
	adds r0, r5, #0
	bl mem_free
_0802F72E:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802F734: .4byte gStaticData_087E5144
_0802F738: .4byte gUnknown_030014FC
_0802F73C: .4byte gUnknown_030012C0
_0802F740: .4byte gUnknown_03001518
_0802F744: .4byte gStaticData_087E4DF4

	thumb_func_start sub_802F748
sub_802F748: @ 0x0802F748
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r1, _0802F774 @ =gStaticData_0817C1C0
	ldr r0, [r4, #0x28]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r7, #2
	ldrsh r2, [r0, r7]
	adds r7, r1, #0
	cmp r2, #0
	ble _0802F778
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
	b _0802F77E
	.align 2, 0
_0802F774: .4byte gStaticData_0817C1C0
_0802F778:
	adds r0, r7, #4
	adds r0, r3, r0
	ldr r3, [r0]
_0802F77E:
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _0802F794
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _0802F796
_0802F794:
	adds r0, r1, #0
_0802F796:
	adds r0, r4, r0
	bl sub_803AD84
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_802F7A4
sub_802F7A4: @ 0x0802F7A4
	ldr r0, _0802F7AC @ =gUnknown_03001506
	ldrb r0, [r0]
	bx lr
	.align 2, 0
_0802F7AC: .4byte gUnknown_03001506

	thumb_func_start sub_802F7B0
sub_802F7B0: @ 0x0802F7B0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x14
	ldr r2, _0802F850 @ =0x040000D4
	str r0, [r2]
	movs r1, #0xa0
	lsls r1, r1, #0x13
	str r1, [r2, #4]
	ldr r1, _0802F854 @ =0x80000100
	str r1, [r2, #8]
	ldr r1, [r2, #8]
	movs r2, #0x80
	lsls r2, r2, #2
	adds r1, r0, r2
	movs r5, #0
	ldrsh r3, [r1, r5]
	mov r8, r3
	adds r2, #2
	adds r1, r0, r2
	movs r5, #0
	ldrsh r3, [r1, r5]
	str r3, [sp]
	movs r1, #0x81
	lsls r1, r1, #2
	adds r0, r0, r1
	ldm r0!, {r2}
	str r2, [sp, #4]
	mov r1, r8
	muls r1, r3, r1
	adds r1, #1
	lsrs r2, r1, #0x1f
	adds r1, r1, r2
	asrs r1, r1, #1
	lsls r1, r1, #2
	adds r1, r0, r1
	str r1, [sp, #8]
	ldr r3, [sp, #4]
	lsls r1, r3, #5
	ldr r5, [sp, #8]
	adds r6, r1, r5
	adds r7, r0, #0
	ldr r5, _0802F858 @ =0x0600D000
	bl sub_8029AC4
	ldr r1, _0802F85C @ =0xFFFFFE00
	adds r1, r0, r1
	str r1, [sp, #0xc]
	movs r2, #0
	mov ip, r2
	movs r0, #0
	ldr r3, [sp]
	cmp r0, r3
	bge _0802F892
_0802F820:
	movs r4, #0
	movs r1, #0x40
	adds r1, r1, r5
	mov sb, r1
	adds r0, #1
	mov sl, r0
	cmp r4, r8
	bge _0802F888
	movs r2, #0xf8
	lsls r2, r2, #3
	adds r3, r5, r2
	adds r2, r5, #0
_0802F838:
	ldrh r5, [r7]
	ldr r0, [sp, #0xc]
	adds r5, r5, r0
	str r5, [sp, #0x10]
	adds r7, #2
	mov r1, ip
	cmp r1, #0
	beq _0802F860
	ldrb r5, [r6]
	lsrs r1, r5, #4
	adds r6, #1
	b _0802F866
	.align 2, 0
_0802F850: .4byte 0x040000D4
_0802F854: .4byte 0x80000100
_0802F858: .4byte 0x0600D000
_0802F85C: .4byte 0xFFFFFE00
_0802F860:
	movs r1, #0xf
	ldrb r0, [r6]
	ands r1, r0
_0802F866:
	movs r0, #1
	mov r5, ip
	eors r5, r0
	mov ip, r5
	lsls r1, r1, #0xc
	ldr r0, [sp, #0x10]
	orrs r1, r0
	cmp r4, #0x1f
	bgt _0802F87C
	strh r1, [r2]
	b _0802F87E
_0802F87C:
	strh r1, [r3]
_0802F87E:
	adds r3, #2
	adds r2, #2
	adds r4, #1
	cmp r4, r8
	blt _0802F838
_0802F888:
	mov r5, sb
	mov r0, sl
	ldr r1, [sp]
	cmp r0, r1
	blt _0802F820
_0802F892:
	movs r2, #0x80
	lsls r2, r2, #0x13
	ldrh r0, [r2]
	movs r3, #0x80
	lsls r3, r3, #2
	adds r1, r3, #0
	orrs r0, r1
	strh r0, [r2]
	ldr r1, _0802F8DC @ =0x0400000A
	ldr r5, _0802F8E0 @ =0x00005A07
	adds r0, r5, #0
	strh r0, [r1]
	bl sub_8029AC4
	lsls r0, r0, #5
	movs r1, #0xc0
	lsls r1, r1, #0x13
	adds r0, r0, r1
	ldr r2, _0802F8E4 @ =0x040000D4
	ldr r3, [sp, #8]
	str r3, [r2]
	str r0, [r2, #4]
	ldr r5, [sp, #4]
	lsls r0, r5, #4
	movs r1, #0x80
	lsls r1, r1, #0x18
	orrs r0, r1
	str r0, [r2, #8]
	ldr r0, [r2, #8]
	add sp, #0x14
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802F8DC: .4byte 0x0400000A
_0802F8E0: .4byte 0x00005A07
_0802F8E4: .4byte 0x040000D4

	thumb_func_start sub_802F8E8
sub_802F8E8: @ 0x0802F8E8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	adds r4, r0, #0
	adds r5, r1, #0
	mov r8, r2
	str r3, [sp]
	ldr r6, _0802F910 @ =0x0600D000
	bl sub_8029AC4
	ldr r1, _0802F914 @ =0xFFFFFE00
	adds r1, r1, r0
	mov sl, r1
	movs r7, #0
	movs r0, #0
	b _0802F966
	.align 2, 0
_0802F910: .4byte 0x0600D000
_0802F914: .4byte 0xFFFFFE00
_0802F918:
	movs r3, #0
	movs r1, #0x40
	adds r1, r1, r6
	mov ip, r1
	adds r0, #1
	mov sb, r0
	cmp r3, r8
	bge _0802F962
	adds r2, r6, #0
_0802F92A:
	ldrh r6, [r5]
	add r6, sl
	adds r5, #2
	cmp r7, #0
	beq _0802F93C
	ldrb r0, [r4]
	lsrs r1, r0, #4
	adds r4, #1
	b _0802F942
_0802F93C:
	movs r1, #0xf
	ldrb r0, [r4]
	ands r1, r0
_0802F942:
	movs r0, #1
	eors r7, r0
	lsls r1, r1, #0xc
	orrs r1, r6
	cmp r3, #0x1f
	bgt _0802F952
	strh r1, [r2]
	b _0802F95A
_0802F952:
	movs r6, #0xf8
	lsls r6, r6, #3
	adds r0, r2, r6
	strh r1, [r0]
_0802F95A:
	adds r2, #2
	adds r3, #1
	cmp r3, r8
	blt _0802F92A
_0802F962:
	mov r6, ip
	mov r0, sb
_0802F966:
	ldr r1, [sp]
	cmp r0, r1
	blt _0802F918
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_802F97C
sub_802F97C: @ 0x0802F97C
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x1c]
	ldr r1, [r4, #0x58]
	adds r0, r0, r1
	str r0, [r4, #0x1c]
	ldr r0, [r4, #0x20]
	subs r0, #0xc0
	ldr r1, [r4, #0x5c]
	adds r0, r0, r1
	str r0, [r4, #0x20]
	ldr r0, [r4, #0x24]
	movs r1, #0x80
	lsls r1, r1, #3
	adds r0, r0, r1
	str r0, [r4, #0x24]
	adds r0, r4, #0
	bl sub_802A3AC
	adds r2, r0, #0
	cmp r2, #0
	beq _0802F9BA
	ldr r1, [r2, #0x50]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0x24]
	movs r1, #2
	bl sub_803AD80
	b _0802F9E2
_0802F9BA:
	adds r0, r4, #0
	bl sub_8031378
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802F9D8
	movs r0, #2
	bl sub_803146C
	cmp r4, #0
	beq _0802F9FE
	ldr r1, [r4, #0x50]
	movs r3, #8
	ldrsh r0, [r1, r3]
	b _0802F9EC
_0802F9D8:
	ldr r1, [r4, #0x34]
	movs r0, #0x82
	lsls r0, r0, #8
	cmp r1, r0
	ble _0802F9F8
_0802F9E2:
	cmp r4, #0
	beq _0802F9FE
	ldr r1, [r4, #0x50]
	movs r2, #8
	ldrsh r0, [r1, r2]
_0802F9EC:
	adds r0, r4, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
	b _0802F9FE
_0802F9F8:
	adds r0, r4, #0
	bl sub_802A7B8
_0802F9FE:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_802FA04
sub_802FA04: @ 0x0802FA04
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
	ldr r0, _0802FA30 @ =gStaticData_087E517C
	str r0, [r4, #0x50]
	str r6, [r4, #0x58]
	str r7, [r4, #0x5c]
	adds r0, r4, #0
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0802FA30: .4byte gStaticData_087E517C

	thumb_func_start sub_802FA34
sub_802FA34: @ 0x0802FA34
	movs r0, #1
	bx lr

	thumb_func_start sub_802FA38
sub_802FA38: @ 0x0802FA38
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	adds r4, r0, #0
	ldr r1, [r4, #0x34]
	movs r0, #0xd8
	lsls r0, r0, #5
	cmp r1, r0
	ble _0802FA56
	adds r1, r4, #0
	adds r1, #0x2c
	movs r0, #1
	b _0802FA5C
_0802FA56:
	adds r1, r4, #0
	adds r1, #0x2c
	movs r0, #0
_0802FA5C:
	strb r0, [r1]
	ldr r1, [r4, #0x60]
	asrs r1, r1, #4
	ldr r0, [r4, #0x1c]
	adds r0, r0, r1
	str r0, [r4, #0x1c]
	ldr r1, [r4, #0x64]
	asrs r1, r1, #4
	ldr r0, [r4, #0x20]
	adds r0, r0, r1
	str r0, [r4, #0x20]
	ldr r1, [r4, #0x68]
	asrs r1, r1, #4
	ldr r0, [r4, #0x24]
	adds r0, r0, r1
	str r0, [r4, #0x24]
	ldr r1, _0802FAA4 @ =gStaticData_0817C260
	ldr r0, [r4, #0x28]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r7, #2
	ldrsh r2, [r0, r7]
	adds r7, r1, #0
	cmp r2, #0
	ble _0802FAA8
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
	b _0802FAAE
	.align 2, 0
_0802FAA4: .4byte gStaticData_0817C260
_0802FAA8:
	adds r0, r7, #4
	adds r0, r3, r0
	ldr r3, [r0]
_0802FAAE:
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _0802FAC4
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _0802FAC6
_0802FAC4:
	adds r0, r1, #0
_0802FAC6:
	adds r0, r4, r0
	bl sub_803AD84
	ldr r0, [r4, #0xc]
	cmp r0, #3
	bne _0802FB76
	ldr r0, [r4, #0x58]
	mov r8, r0
	cmp r0, #0
	bne _0802FB70
	ldr r0, _0802FB5C @ =gUnknown_03000884
	ldr r5, [r0]
	ldr r0, [r5, #0x24]
	adds r0, #0xa
	ldr r1, [r4, #0x24]
	mov sb, r1
	subs r0, r0, r1
	ldr r1, _0802FB60 @ =0xFFFFFE56
	bl sub_803ADB4
	adds r2, r0, #0
	cmp r2, #0
	ble _0802FB76
	ldr r1, [r4, #0x34]
	ldr r0, _0802FB64 @ =0x00008BFF
	cmp r1, r0
	bgt _0802FB76
	movs r0, #0x80
	lsls r0, r0, #5
	adds r1, r2, #0
	bl sub_803ADB4
	ldr r1, [r5, #0x1c]
	ldr r7, [r4, #0x1c]
	subs r1, r1, r7
	adds r3, r1, #0
	muls r3, r0, r3
	asrs r2, r3, #0xc
	mov ip, r2
	ldr r1, [r5, #0x20]
	ldr r6, [r4, #0x20]
	subs r1, r1, r6
	adds r2, r1, #0
	muls r2, r0, r2
	asrs r5, r2, #0xc
	asrs r3, r3, #0x1f
	mov r1, ip
	eors r1, r3
	subs r1, r1, r3
	asrs r2, r2, #0x1f
	adds r0, r5, #0
	eors r0, r2
	subs r0, r0, r2
	adds r1, r1, r0
	ldr r0, _0802FB68 @ =0x000005FF
	cmp r1, r0
	bgt _0802FB76
	mov r2, sb
	subs r2, #0xa
	str r5, [sp]
	adds r0, r7, #0
	adds r1, r6, #0
	mov r3, ip
	bl sub_802E674
	ldr r0, [r4, #0x5c]
	adds r0, #1
	str r0, [r4, #0x5c]
	cmp r0, #3
	bne _0802FB6C
	mov r3, r8
	str r3, [r4, #0x5c]
	movs r0, #0x3c
	b _0802FB74
	.align 2, 0
_0802FB5C: .4byte gUnknown_03000884
_0802FB60: .4byte 0xFFFFFE56
_0802FB64: .4byte 0x00008BFF
_0802FB68: .4byte 0x000005FF
_0802FB6C:
	movs r0, #0x14
	b _0802FB74
_0802FB70:
	mov r0, r8
	subs r0, #1
_0802FB74:
	str r0, [r4, #0x58]
_0802FB76:
	adds r0, r4, #0
	adds r0, #0x7c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802FBB0
	adds r0, r4, #0
	bl sub_802A6EC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802FBB0
	ldr r0, _0802FBD8 @ =gUnknown_03000884
	ldr r0, [r0]
	ldr r2, [r0, #0x50]
	movs r7, #0x20
	ldrsh r1, [r2, r7]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #6
	bl sub_803AD80
	ldr r1, [r4, #0x50]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #4
	bl sub_803AD80
_0802FBB0:
	ldr r0, [r4, #0x28]
	cmp r0, #3
	bne _0802FBDC
	ldr r1, [r4, #0x20]
	movs r0, #0xe1
	lsls r0, r0, #8
	cmp r1, r0
	ble _0802FBDC
	cmp r4, #0
	beq _0802FBE2
	ldr r1, [r4, #0x50]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
	b _0802FBE2
	.align 2, 0
_0802FBD8: .4byte gUnknown_03000884
_0802FBDC:
	adds r0, r4, #0
	bl sub_802A7B8
_0802FBE2:
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

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

