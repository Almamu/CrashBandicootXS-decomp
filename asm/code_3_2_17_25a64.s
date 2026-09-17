.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8025A64
sub_8025A64: @ 0x08025A64
	push {r4, r5, r6, r7, lr}
	adds r6, r3, #0
	add r0, sp, #0x18
	ldrb r7, [r0]
	movs r4, #0
	ldr r0, _08025B00 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r0, #0x8c
	ldrb r5, [r0]
	cmp r5, #0
	bne _08025AF6
	ldr r0, _08025B04 @ =0x0000FFFF
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	movs r3, #0
	bl sub_8011114
	adds r4, r0, #0
	movs r0, #0x10
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	adds r0, r4, #0
	adds r0, #0x49
	strb r6, [r0]
	adds r1, r4, #0
	adds r1, #0x4a
	ldr r0, [sp, #0x14]
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x4b
	strb r5, [r0]
	ldr r0, _08025B08 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r3, #0x8d
	lsls r3, r3, #2
	adds r0, r0, r3
	str r0, [r4, #0x20]
	movs r0, #0xa
	subs r1, #0x1d
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
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	cmp r7, #0
	beq _08025AF6
	adds r0, r4, #0
	bl sub_80111B8
_08025AF6:
	adds r0, r4, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08025B00: .4byte gUnknown_030012C0
_08025B04: .4byte 0x0000FFFF
_08025B08: .4byte gUnknown_030012D0

	thumb_func_start sub_8025B0C
sub_8025B0C: @ 0x08025B0C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x28
	mov r8, r3
	ldr r7, [sp, #0x44]
	ldr r6, [sp, #0x48]
	ldr r3, [r6]
	asrs r3, r3, #8
	ldr r5, [r6, #4]
	asrs r5, r5, #8
	adds r4, r6, #0
	adds r4, #0x28
	ldrb r4, [r4]
	lsls r4, r4, #0x1b
	lsrs r4, r4, #0x1f
	str r5, [sp]
	str r4, [sp, #4]
	bl sub_8025BAC
	adds r5, r0, #0
	add r0, sp, #8
	adds r1, r5, #0
	bl sub_8007B98
	ldr r4, [sp, #0x10]
	add r0, sp, #0x18
	adds r1, r6, #0
	bl sub_8007B98
	ldr r0, [sp, #0x20]
	lsrs r1, r4, #0x1f
	adds r4, r4, r1
	asrs r4, r4, #1
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	adds r4, r4, r0
	add r4, r8
	ldr r0, [r5]
	asrs r1, r0, #8
	adds r3, r5, #0
	adds r3, #0x28
	ldrb r2, [r3]
	lsls r0, r2, #0x1b
	adds r2, r1, r4
	cmp r0, #0
	bge _08025B6E
	subs r2, r1, r4
_08025B6E:
	ldr r0, [r5, #4]
	asrs r0, r0, #8
	ldr r1, [sp, #0x40]
	adds r0, r0, r1
	lsls r1, r2, #8
	str r1, [r5]
	lsls r0, r0, #8
	str r0, [r5, #4]
	ldrb r3, [r3]
	lsls r0, r3, #0x1b
	cmp r0, #0
	bge _08025B94
	rsbs r0, r7, #0
	movs r1, #0x40
	str r0, [r5, #0x60]
	str r0, [r5, #0x48]
	str r1, [r5, #0x4c]
	str r0, [r5, #0x50]
	b _08025B9E
_08025B94:
	movs r0, #0x40
	str r7, [r5, #0x60]
	str r7, [r5, #0x48]
	str r0, [r5, #0x4c]
	str r7, [r5, #0x50]
_08025B9E:
	adds r0, r5, #0
	add sp, #0x28
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_8025BAC
sub_8025BAC: @ 0x08025BAC
	push {r4, r5, r6, r7, lr}
	adds r5, r1, #0
	adds r7, r2, #0
	ldr r2, [sp, #0x14]
	ldr r6, [sp, #0x18]
	cmp r3, #0
	bge _08025BBC
	movs r3, #0
_08025BBC:
	ldr r0, _08025C94 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r1, [r0, #0x10]
	ldr r0, [r1, #0x10]
	lsls r4, r0, #8
	asrs r0, r4, #8
	cmp r3, r0
	blt _08025BD0
	lsrs r0, r4, #8
	subs r3, r0, #1
_08025BD0:
	cmp r2, #0
	bge _08025BD6
	movs r2, #0
_08025BD6:
	ldr r0, [r1, #0x14]
	lsls r4, r0, #8
	asrs r0, r4, #8
	cmp r2, r0
	blt _08025BE4
	lsrs r0, r4, #8
	subs r2, r0, #1
_08025BE4:
	ldr r0, _08025C98 @ =0x0000FFFF
	lsls r1, r3, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	movs r3, #0
	bl sub_8009ED0
	adds r4, r0, #0
	rsbs r1, r6, #0
	orrs r1, r6
	adds r2, r4, #0
	adds r2, #0x28
	lsrs r1, r1, #0x1f
	lsls r1, r1, #4
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r3, [r2]
	ands r0, r3
	orrs r0, r1
	strb r0, [r2]
	ldr r0, _08025C9C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	lsls r1, r5, #1
	adds r1, r1, r5
	lsls r1, r1, #2
	ldr r0, [r0]
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	strb r7, [r0]
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
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x10
	bl sub_8026EDC
	bl sub_800CCE0
	str r0, [r4, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r1, [r4, #0xc]
	ands r0, r1
	movs r1, #3
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _08025CA0 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	adds r0, r4, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08025C94: .4byte gUnknown_03001308
_08025C98: .4byte 0x0000FFFF
_08025C9C: .4byte gUnknown_030012D0
_08025CA0: .4byte gUnknown_030012F0

	thumb_func_start sub_8025CA4
sub_8025CA4: @ 0x08025CA4
	push {r4, r5, r6, r7, lr}
	adds r7, r3, #0
	ldr r5, [sp, #0x14]
	add r0, sp, #0x18
	ldrb r6, [r0]
	movs r4, #0
	ldr r0, _08025CD4 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _08025D1A
	cmp r6, #0
	bne _08025CC4
	cmp r5, #0xff
	bne _08025CDC
_08025CC4:
	ldr r3, _08025CD8 @ =0x0000FFFF
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	adds r0, r3, #0
	b _08025CE8
	.align 2, 0
_08025CD4: .4byte gUnknown_030012C0
_08025CD8: .4byte 0x0000FFFF
_08025CDC:
	ldr r0, _08025D24 @ =0x0000FFFF
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	movs r3, #0
_08025CE8:
	bl sub_801173C
	adds r4, r0, #0
	movs r0, #0x10
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	adds r1, r4, #0
	adds r1, #0x49
	movs r0, #0
	strb r7, [r1]
	adds r1, #1
	strb r5, [r1]
	adds r1, #1
	strb r0, [r1]
	cmp r5, #0xff
	bne _08025D10
	adds r0, r4, #0
	bl sub_801191C
_08025D10:
	cmp r6, #0
	beq _08025D1A
	adds r0, r4, #0
	bl sub_8011870
_08025D1A:
	adds r0, r4, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08025D24: .4byte 0x0000FFFF

