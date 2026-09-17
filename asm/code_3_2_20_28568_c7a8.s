.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_802C7A8
sub_802C7A8: @ 0x0802C7A8
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #0x24
	mov sb, r0
	ldr r0, _0802C8A4 @ =gUnknown_03000884
	ldr r0, [r0]
	ldr r4, [r0, #0x4c]
	add r7, sp, #0x18
	movs r0, #0
	mov r8, r0
_0802C7C0:
	ldr r0, [r4, #0x30]
	ldrb r0, [r0]
	cmp r0, #4
	beq _0802C7CA
	b _0802C8DE
_0802C7CA:
	cmp r4, sb
	bne _0802C7D0
	b _0802C8DE
_0802C7D0:
	add r1, sp, #0xc
	mov r0, sb
	adds r0, #0x38
	ldm r0!, {r2, r3, r5}
	stm r1!, {r2, r3, r5}
	mov r6, sb
	ldr r0, [r6, #0x1c]
	asrs r0, r0, #8
	ldr r2, [r6, #0x20]
	asrs r2, r2, #8
	ldr r1, [r6, #0x24]
	asrs r1, r1, #8
	add r5, sp, #0xc
	ldrh r3, [r5]
	adds r0, r3, r0
	strh r0, [r5]
	ldrh r0, [r5, #2]
	adds r0, r0, r2
	strh r0, [r5, #2]
	ldrh r6, [r5, #4]
	adds r1, r6, r1
	strh r1, [r5, #4]
	mov r1, sp
	adds r0, r5, #0
	ldm r0!, {r2, r3, r6}
	stm r1!, {r2, r3, r6}
	mov r0, sp
	mov r1, sp
	movs r2, #0xc
	bl sub_800014C
	add r1, sp, #0x18
	adds r0, r4, #0
	adds r0, #0x38
	ldm r0!, {r2, r3, r6}
	stm r1!, {r2, r3, r6}
	ldr r0, [r4, #0x1c]
	asrs r0, r0, #8
	ldr r2, [r4, #0x20]
	asrs r2, r2, #8
	ldr r1, [r4, #0x24]
	asrs r1, r1, #8
	ldrh r3, [r7]
	adds r0, r3, r0
	strh r0, [r7]
	ldrh r0, [r7, #2]
	adds r0, r0, r2
	strh r0, [r7, #2]
	ldrh r6, [r7, #4]
	adds r1, r6, r1
	strh r1, [r7, #4]
	adds r1, r5, #0
	adds r0, r7, #0
	ldm r0!, {r2, r3, r6}
	stm r1!, {r2, r3, r6}
	adds r0, r5, #0
	adds r1, r5, #0
	movs r2, #0xc
	bl sub_800014C
	mov r1, sp
	movs r0, #4
	ldrsh r2, [r1, r0]
	movs r6, #4
	ldrsh r3, [r5, r6]
	movs r6, #0xa
	ldrsh r0, [r5, r6]
	adds r0, r3, r0
	cmp r2, r0
	bge _0802C89E
	movs r6, #0xa
	ldrsh r0, [r1, r6]
	adds r0, r2, r0
	cmp r0, r3
	ble _0802C89E
	movs r0, #2
	ldrsh r2, [r1, r0]
	movs r6, #2
	ldrsh r3, [r5, r6]
	movs r6, #8
	ldrsh r0, [r5, r6]
	adds r0, r3, r0
	cmp r2, r0
	bge _0802C89E
	movs r6, #8
	ldrsh r0, [r1, r6]
	adds r0, r2, r0
	cmp r0, r3
	ble _0802C89E
	movs r0, #0
	ldrsh r2, [r1, r0]
	movs r6, #0
	ldrsh r3, [r5, r6]
	movs r0, #6
	ldrsh r5, [r5, r0]
	adds r0, r3, r5
	cmp r2, r0
	bge _0802C89E
	movs r5, #6
	ldrsh r0, [r1, r5]
	adds r0, r2, r0
	cmp r0, r3
	bgt _0802C8A8
_0802C89E:
	movs r0, #0
	b _0802C8AA
	.align 2, 0
_0802C8A4: .4byte gUnknown_03000884
_0802C8A8:
	movs r0, #1
_0802C8AA:
	cmp r0, #0
	beq _0802C8DE
	ldr r0, [r4, #0xc]
	cmp r0, #0x12
	beq _0802C8DE
	ldr r0, _0802C8F8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #4
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	ldr r0, _0802C8FC @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022FEC
	mov r6, r8
	str r6, [r4, #0x44]
	movs r0, #0x12
	str r0, [r4, #0xc]
	ldr r0, [r4]
	adds r0, #0xd8
	ldrh r0, [r0]
	strh r0, [r4, #0x10]
	strb r6, [r4, #0x12]
	str r6, [r4, #8]
_0802C8DE:
	ldr r4, [r4, #0x4c]
	ldr r0, _0802C900 @ =gUnknown_03000884
	ldr r0, [r0]
	cmp r4, r0
	beq _0802C8EA
	b _0802C7C0
_0802C8EA:
	add sp, #0x24
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802C8F8: .4byte gUnknown_030012BC
_0802C8FC: .4byte gUnknown_030012C0
_0802C900: .4byte gUnknown_03000884

