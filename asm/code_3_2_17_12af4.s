.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8012AF4
sub_8012AF4: @ 0x08012AF4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0xc
	adds r4, r0, #0
	ldr r0, _08012B30 @ =gUnknown_030012D8
	ldr r2, [r0]
	movs r3, #0x81
	lsls r3, r3, #1
	adds r1, r2, r3
	ldrb r1, [r1]
	adds r3, r0, #0
	cmp r1, #0
	bne _08012B1A
	ldr r5, _08012B34 @ =0x00000103
	adds r0, r2, r5
	ldrb r0, [r0]
	cmp r0, #0
	beq _08012B5A
_08012B1A:
	ldr r0, [r4, #0x10]
	adds r0, #0x68
	ldrb r0, [r0]
	cmp r0, #8
	bne _08012B5A
	cmp r1, #0
	beq _08012B3C
	ldr r0, [r2]
	ldr r6, _08012B38 @ =0xFFFFFF00
	adds r0, r0, r6
	b _08012B4E
	.align 2, 0
_08012B30: .4byte gUnknown_030012D8
_08012B34: .4byte 0x00000103
_08012B38: .4byte 0xFFFFFF00
_08012B3C:
	ldr r1, _08012C88 @ =0x00000103
	adds r0, r2, r1
	ldrb r0, [r0]
	cmp r0, #0
	beq _08012B50
	ldr r0, [r2]
	movs r5, #0x80
	lsls r5, r5, #1
	adds r0, r0, r5
_08012B4E:
	str r0, [r2]
_08012B50:
	ldr r0, [r3]
	ldr r1, [r0]
	ldr r2, [r0, #4]
	bl sub_8009EA8
_08012B5A:
	ldr r0, [r4, #8]
	cmp r0, #0
	bne _08012BA0
	ldr r5, _08012C8C @ =gUnknown_030012D8
	ldr r1, [r5]
	ldr r0, [r1, #0x60]
	cmp r0, #0
	bne _08012B9A
	ldr r0, [r1, #0x20]
	ldrh r0, [r0, #0xa]
	cmp r0, #0x12
	beq _08012B9A
	adds r0, r4, #0
	adds r0, #0x33
	ldrb r0, [r0]
	cmp r0, #0
	bne _08012B9A
	ldr r0, _08012C90 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x36
	bl sub_80019A8
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r6, #0
	ldrsh r0, [r2, r6]
	adds r0, r4, r0
	ldr r1, [r5]
	ldr r3, [r2, #4]
	movs r2, #0x12
	bl sub_803AD84
_08012B9A:
	ldr r0, [r4, #8]
	cmp r0, #0
	beq _08012BA8
_08012BA0:
	adds r7, r4, #0
	adds r7, #0x2f
	cmp r0, #0x11
	bne _08012BD2
_08012BA8:
	ldr r0, _08012C8C @ =gUnknown_030012D8
	ldr r1, [r0]
	ldr r0, [r1, #0x60]
	adds r7, r4, #0
	adds r7, #0x2f
	cmp r0, #0
	beq _08012BD2
	movs r2, #0x80
	lsls r2, r2, #1
	adds r0, r1, r2
	ldrb r1, [r0]
	cmp r1, #0
	bne _08012BD2
	adds r0, r4, #0
	adds r0, #0x31
	strb r1, [r0]
	movs r0, #1
	strb r0, [r7]
	adds r0, r4, #0
	adds r0, #0x27
	strb r1, [r0]
_08012BD2:
	ldrb r3, [r7]
	mov ip, r3
	cmp r3, #1
	bne _08012CAE
	ldr r0, [r4, #4]
	adds r3, r4, #0
	adds r3, #0x27
	ldr r1, [r0]
	ldrb r5, [r3]
	lsls r0, r5, #3
	adds r0, r0, r1
	ldr r1, [r0]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r2, _08012C94 @ =gStaticData_0816B304
	mov r1, sp
	adds r0, r0, r2
	ldm r0!, {r2, r5, r6}
	stm r1!, {r2, r5, r6}
	ldr r0, _08012C8C @ =gUnknown_030012D8
	ldr r1, [r0]
	movs r6, #0x80
	lsls r6, r6, #1
	adds r0, r1, r6
	ldrb r0, [r0]
	adds r5, r4, #0
	adds r5, #0x31
	mov r8, r3
	cmp r0, #0
	beq _08012C3A
	ldr r0, [r4, #0x10]
	adds r0, #0x68
	ldrb r0, [r0]
	cmp r0, #8
	bne _08012C3A
	ldr r0, [r1, #0x60]
	cmp r0, #0
	beq _08012C3A
	mov r0, ip
	strb r0, [r5]
	ldr r0, [sp, #8]
	movs r1, #0xc0
	lsls r1, r1, #1
	bl sub_80008FC
	str r0, [sp, #8]
	ldr r0, [sp, #4]
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	str r0, [sp, #4]
_08012C3A:
	mov r1, r8
	ldrb r1, [r1]
	cmp r1, #0x1e
	bne _08012C6E
	movs r0, #0
	strb r0, [r5]
	ldr r0, _08012C8C @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	adds r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #0
	beq _08012C6E
	ldr r0, [sp, #4]
	movs r1, #0x80
	lsls r1, r1, #2
	bl sub_80008FC
	str r0, [sp, #4]
	ldr r0, [sp]
	movs r1, #0xc0
	lsls r1, r1, #1
	bl sub_80008FC
	str r0, [sp]
_08012C6E:
	ldrb r0, [r5]
	cmp r0, #0
	beq _08012C98
	ldr r2, [r4, #0xc]
	movs r3, #0x38
	ldrsh r0, [r2, r3]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #0x3c]
	mov r2, sp
	bl sub_803AD84
	b _08012CAA
	.align 2, 0
_08012C88: .4byte 0x00000103
_08012C8C: .4byte gUnknown_030012D8
_08012C90: .4byte gUnknown_030012BC
_08012C94: .4byte gStaticData_0816B304
_08012C98:
	ldr r2, [r4, #0xc]
	movs r5, #0x28
	ldrsh r0, [r2, r5]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #0x2c]
	mov r2, sp
	bl sub_803AD84
_08012CAA:
	movs r0, #0
	strb r0, [r7]
_08012CAE:
	adds r0, r4, #0
	adds r0, #0x30
	adds r5, r0, #0
	ldrb r6, [r5]
	cmp r6, #1
	bne _08012D16
	ldr r0, [r4, #4]
	adds r1, r4, #0
	adds r1, #0x28
	ldr r2, [r0]
	ldrb r1, [r1]
	lsls r0, r1, #3
	adds r0, r0, r2
	ldr r1, [r0, #4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r2, _08012CFC @ =gStaticData_0816B304
	mov r1, sp
	adds r0, r0, r2
	ldm r0!, {r2, r3, r6}
	stm r1!, {r2, r3, r6}
	adds r0, r4, #0
	adds r0, #0x32
	ldrb r0, [r0]
	cmp r0, #0
	beq _08012D00
	ldr r2, [r4, #0xc]
	adds r2, #0x40
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	mov r2, sp
	bl sub_803AD84
	b _08012D12
	.align 2, 0
_08012CFC: .4byte gStaticData_0816B304
_08012D00:
	ldr r2, [r4, #0xc]
	movs r3, #0x30
	ldrsh r0, [r2, r3]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #0x34]
	mov r2, sp
	bl sub_803AD84
_08012D12:
	movs r0, #0
	strb r0, [r5]
_08012D16:
	add sp, #0xc
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8012D24
sub_8012D24: @ 0x08012D24
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _08012DB4 @ =gUnknown_03001304
	ldr r0, [r0]
	ldr r1, _08012DB8 @ =gUnknown_030007E0
	ldr r1, [r1]
	str r1, [sp]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r6, r0, #0x18
	adds r1, r4, #0
	adds r1, #0x25
	ldrb r0, [r1]
	cmp r0, #0
	beq _08012D54
	subs r0, #1
	strb r0, [r1]
	cmp r6, #0
	bne _08012D54
	strb r6, [r1]
_08012D54:
	ldr r2, [r4, #0x10]
	adds r0, r2, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08012D7C
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r2, #0
	movs r2, #0x12
	bl sub_803AD84
	adds r1, r4, #0
	adds r1, #0x33
	movs r0, #0
	strb r0, [r1]
_08012D7C:
	ldr r1, [r4, #0x1c]
	adds r3, r1, #1
	str r3, [r4, #0x1c]
	ldr r2, [r4, #0x10]
	adds r0, r2, #0
	adds r0, #0x2d
	ldrb r0, [r0]
	cmp r0, #0x12
	bne _08012E0E
	ldr r5, [r2, #0x30]
	cmp r5, #0
	bne _08012E0E
	movs r0, #0xe1
	lsls r0, r0, #3
	cmp r3, r0
	ble _08012DBC
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r2, #0
	movs r2, #0x1a
	bl sub_803AD84
	str r5, [r4, #0x1c]
	b _08012E06
	.align 2, 0
_08012DB4: .4byte gUnknown_03001304
_08012DB8: .4byte gUnknown_030007E0
_08012DBC:
	ldr r3, _08012DDC @ =0xFFFFFB64
	adds r0, r1, r3
	cmp r0, #0x26
	bhi _08012DE4
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r2, #0
	movs r2, #5
	bl sub_803AD84
	ldr r0, _08012DE0 @ =0x000004C4
	b _08012E04
	.align 2, 0
_08012DDC: .4byte 0xFFFFFB64
_08012DE0: .4byte 0x000004C4
_08012DE4:
	ldr r3, _08012E78 @ =0xFFFFFE20
	adds r0, r1, r3
	cmp r0, #0x26
	bhi _08012E0E
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r2, #0
	movs r2, #0xe
	bl sub_803AD84
	movs r0, #0x82
	lsls r0, r0, #2
_08012E04:
	str r0, [r4, #0x1c]
_08012E06:
	adds r1, r4, #0
	adds r1, #0x33
	movs r0, #1
	strb r0, [r1]
_08012E0E:
	adds r0, r4, #0
	bl sub_8012A7C
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	cmp r5, #0
	beq _08012E1E
	b _08012FB0
_08012E1E:
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #1
	mov r8, r0
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08012E80
	ldr r0, _08012E7C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xd
	bl PlaySfx
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #5
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r3, #0
	ldrsh r0, [r2, r3]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x13
	bl sub_803AD84
	str r5, [r4, #0x18]
	movs r1, #7
	adds r0, r4, #0
	adds r0, #0x32
	strb r5, [r0]
	subs r0, #2
	mov r2, r8
	strb r2, [r0]
	subs r0, #8
	strb r1, [r0]
	b _08012ECC
	.align 2, 0
_08012E78: .4byte 0xFFFFFE20
_08012E7C: .4byte gUnknown_030012BC
_08012E80:
	movs r0, #2
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r5, r0, #0x10
	cmp r5, #0
	beq _08012E94
	adds r0, r4, #0
	bl sub_8015398
	b _08012ECC
_08012E94:
	mov r0, sp
	ldrh r1, [r0]
	movs r0, #0x80
	lsls r0, r0, #1
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	cmp r0, #0
	beq _08012ED4
	ldr r1, [r4, #0xc]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x10
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #3
	bl sub_803AD84
	str r5, [r4, #0x1c]
_08012ECC:
	adds r0, r4, #0
	bl sub_80122CC
	b _08012FB0
_08012ED4:
	adds r7, r4, #0
	adds r7, #0x29
	strb r0, [r7]
	cmp r6, #0
	bne _08012F0E
	ldr r1, [r4, #0x10]
	movs r2, #0x80
	lsls r2, r2, #1
	adds r0, r1, r2
	ldrb r0, [r0]
	cmp r0, #0
	beq _08012FAA
	adds r2, r4, #0
	adds r2, #0x27
	ldrb r3, [r2]
	cmp r3, #0x1f
	beq _08012FAA
	ldr r0, [r1, #0x60]
	cmp r0, #0
	beq _08012FAA
	movs r0, #0x1f
	adds r1, r4, #0
	adds r1, #0x31
	mov r3, r8
	strb r3, [r1]
	subs r1, #2
	strb r3, [r1]
	strb r0, [r2]
	b _08012FAA
_08012F0E:
	adds r0, r4, #0
	adds r0, #0x25
	ldrb r5, [r0]
	cmp r5, #0
	bne _08012FAA
	cmp r6, #2
	beq _08012F84
	cmp r6, #2
	blt _08012FAA
	cmp r6, #8
	bgt _08012FAA
	movs r0, #0x80
	lsls r0, r0, #2
	ands r0, r1
	cmp r0, #0
	beq _08012F7C
	ldr r0, _08012F78 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231C4
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08012F7C
	mov r0, r8
	strb r0, [r7]
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #4
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r3, #0
	ldrsh r0, [r2, r3]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x18
	bl sub_803AD84
	movs r0, #0x1b
	adds r1, r4, #0
	adds r1, #0x31
	strb r5, [r1]
	subs r1, #2
	mov r2, r8
	strb r2, [r1]
	subs r1, #8
	strb r0, [r1]
	b _08012FAA
	.align 2, 0
_08012F78: .4byte gUnknown_030012C0
_08012F7C:
	adds r0, r4, #0
	bl sub_8015460
	b _08012FAA
_08012F84:
	ldr r1, [r4, #0xc]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x10
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #3
	bl sub_803AD84
	str r5, [r4, #0x1c]
_08012FAA:
	adds r0, r4, #0
	bl sub_80122CC
_08012FB0:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8012FBC
sub_8012FBC: @ 0x08012FBC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x10
	adds r6, r0, #0
	ldr r0, _08013034 @ =gUnknown_03001304
	mov r8, r0
	ldr r0, _08013038 @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp, #0xc]
	adds r0, r6, #0
	bl sub_8012A7C
	lsls r0, r0, #0x18
	lsrs r4, r0, #0x18
	cmp r4, #0
	beq _08012FE0
	b _0801321A
_08012FE0:
	add r0, sp, #0xc
	ldrh r1, [r0, #2]
	movs r7, #1
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08013040
	ldr r0, _0801303C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xd
	bl PlaySfx
	ldr r1, [r6, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x24]
	movs r1, #5
	bl sub_803AD80
	ldr r2, [r6, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r6, r0
	ldr r1, [r6, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x13
	bl sub_803AD84
	str r4, [r6, #0x18]
	movs r1, #7
	adds r0, r6, #0
	adds r0, #0x32
	strb r4, [r0]
	subs r0, #2
	strb r7, [r0]
	subs r0, #8
	strb r1, [r0]
	b _0801321A
	.align 2, 0
_08013034: .4byte gUnknown_03001304
_08013038: .4byte gUnknown_030007E0
_0801303C: .4byte gUnknown_030012BC
_08013040:
	movs r0, #2
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r5, r0, #0x10
	cmp r5, #0
	beq _08013054
	adds r0, r6, #0
	bl sub_8015398
	b _0801321A
_08013054:
	movs r2, #0x80
	lsls r2, r2, #1
	adds r0, r2, #0
	ands r0, r1
	cmp r0, #0
	beq _080130E4
	ldr r0, _08013100 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x1a
	bl PlaySfx
	movs r4, #0x10
	ldr r1, [r6, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xc
	bl sub_803AD80
	ldr r2, [r6, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r6, r0
	ldr r1, [r6, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0xf
	bl sub_803AD84
	str r5, [r6, #0x18]
	str r4, [r6, #0x1c]
	movs r0, #0x1e
	adds r1, r6, #0
	adds r1, #0x31
	strb r5, [r1]
	subs r1, #2
	movs r4, #1
	strb r7, [r1]
	subs r1, #8
	strb r0, [r1]
	ldr r2, _08013104 @ =gUnknown_030012D8
	ldr r0, [r2]
	adds r0, #0x94
	strb r5, [r0]
	ldr r0, [r2]
	adds r0, #0x94
	strb r5, [r0]
	ldr r0, _08013108 @ =gUnknown_030012E4
	ldr r0, [r0]
	movs r1, #0xa
	str r1, [sp]
	str r5, [sp, #4]
	ldr r1, [r2]
	str r1, [sp, #8]
	movs r1, #0x29
	movs r2, #1
	movs r3, #0
	bl sub_8025B0C
	movs r1, #5
	rsbs r1, r1, #0
	ldrb r2, [r0, #0xc]
	ands r1, r2
	strb r1, [r0, #0xc]
	adds r0, #0x28
	movs r1, #4
	rsbs r1, r1, #0
	ldrb r2, [r0]
	ands r1, r2
	orrs r1, r4
	strb r1, [r0]
_080130E4:
	mov r1, r8
	ldr r0, [r1]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r4, r0, #0x18
	cmp r4, #2
	beq _0801314E
	cmp r4, #2
	bgt _0801310C
	cmp r4, #0
	beq _08013116
	b _0801318C
	.align 2, 0
_08013100: .4byte gUnknown_030012BC
_08013104: .4byte gUnknown_030012D8
_08013108: .4byte gUnknown_030012E4
_0801310C:
	cmp r4, #8
	bgt _0801318C
	cmp r4, #7
	blt _0801318C
	b _0801314E
_08013116:
	str r4, [sp]
	adds r0, r6, #0
	movs r1, #0
	movs r2, #0x12
	movs r3, #0
	bl sub_8015780
	adds r3, r6, #0
	adds r3, #0x31
	strb r4, [r3]
	adds r2, r6, #0
	adds r2, #0x2f
	strb r7, [r2]
	adds r1, r6, #0
	adds r1, #0x27
	strb r4, [r1]
	adds r0, r6, #0
	adds r0, #0x32
	strb r4, [r0]
	subs r0, #2
	strb r7, [r0]
	subs r0, #8
	strb r4, [r0]
	movs r0, #0x1d
	strb r4, [r3]
	strb r7, [r2]
	strb r0, [r1]
	b _0801318C
_0801314E:
	movs r4, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x10
	bl sub_803AD80
	ldr r2, [r6, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r6, r0
	ldr r1, [r6, #0x10]
	ldr r3, [r2, #4]
	movs r2, #3
	bl sub_803AD84
	str r4, [r6, #0x1c]
	movs r1, #0x1d
	adds r0, r6, #0
	adds r0, #0x31
	strb r4, [r0]
	adds r2, r6, #0
	adds r2, #0x2f
	movs r0, #1
	strb r0, [r2]
	adds r0, r6, #0
	adds r0, #0x27
	strb r1, [r0]
_0801318C:
	add r1, sp, #0xc
	movs r0, #0x80
	lsls r0, r0, #2
	ldrh r1, [r1]
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r1, r0, #0x10
	cmp r1, #0
	beq _080131F8
	ldr r0, [r6, #8]
	cmp r0, #3
	bne _08013214
	ldr r0, _080131F4 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231C4
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08013214
	adds r0, r6, #0
	adds r0, #0x29
	movs r5, #0
	movs r4, #1
	strb r4, [r0]
	ldr r1, [r6, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x24]
	movs r1, #4
	bl sub_803AD80
	ldr r2, [r6, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r6, r0
	ldr r1, [r6, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x18
	bl sub_803AD84
	movs r0, #0x1b
	adds r1, r6, #0
	adds r1, #0x31
	strb r5, [r1]
	subs r1, #2
	strb r4, [r1]
	subs r1, #8
	strb r0, [r1]
	b _08013214
	.align 2, 0
_080131F4: .4byte gUnknown_030012C0
_080131F8:
	ldr r0, [r6, #8]
	cmp r0, #4
	bne _0801320C
	adds r0, r6, #0
	adds r0, #0x29
	strb r1, [r0]
	adds r0, r6, #0
	bl sub_8015460
	b _08013214
_0801320C:
	ldr r0, [r6, #0x18]
	cmp r0, #0
	beq _08013214
	str r1, [r6, #0x18]
_08013214:
	adds r0, r6, #0
	bl sub_80122CC
_0801321A:
	add sp, #0x10
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8013228
sub_8013228: @ 0x08013228
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	ldr r1, [r5, #0x10]
	movs r0, #2
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xd]
	ands r0, r2
	strb r0, [r1, #0xd]
	ldr r1, [r5, #0x10]
	movs r0, #3
	rsbs r0, r0, #0
	ldrb r6, [r1, #0xd]
	ands r0, r6
	strb r0, [r1, #0xd]
	ldr r1, [r5, #0x10]
	adds r1, #0x68
	movs r0, #4
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _080132A8
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x1a
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r6, #0
	ldrsh r0, [r2, r6]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x15
	bl sub_803AD84
	ldr r3, [r5, #0x10]
	movs r4, #2
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
	blt _08013296
	subs r4, r0, #1
_08013296:
	str r4, [r3, #0x30]
	adds r0, r3, #0
	bl sub_800B334
	ldr r0, [r5, #0x10]
	movs r1, #0
	adds r0, #0x68
	strb r1, [r0]
	b _080134AA
_080132A8:
	ldr r1, _08013320 @ =gUnknown_030007E0
	ldr r0, [r1]
	str r0, [sp]
	adds r0, r5, #0
	adds r0, #0x26
	ldrb r6, [r0]
	adds r2, r1, #0
	cmp r6, #0
	bne _0801332C
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _0801332C
	ldr r0, _08013324 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xa
	bl PlaySfx
	movs r4, #0x18
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xe
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x10
	bl sub_803AD84
	str r6, [r5, #0x18]
	str r4, [r5, #0x1c]
	adds r0, r5, #0
	adds r0, #0x21
	strb r6, [r0]
	subs r0, #1
	strb r6, [r0]
	adds r0, #2
	strb r6, [r0]
	adds r0, #1
	strb r6, [r0]
	adds r0, #1
	strb r6, [r0]
	ldr r0, _08013328 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x92
	strb r6, [r0]
	b _080134AA
	.align 2, 0
_08013320: .4byte gUnknown_030007E0
_08013324: .4byte gUnknown_030012BC
_08013328: .4byte gUnknown_030012D8
_0801332C:
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08013402
	ldr r4, [r2]
	movs r0, #1
	ands r0, r4
	cmp r0, #0
	beq _080133A8
	movs r0, #0x30
	ands r0, r4
	cmp r0, #0
	beq _080133A8
	adds r0, r1, #0
	adds r0, #0x2d
	ldrb r0, [r0]
	cmp r0, #6
	bne _08013366
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #9
	bl sub_803AD80
	b _0801338A
_08013366:
	ldr r1, [r5, #0xc]
	movs r6, #0x20
	ldrsh r0, [r1, r6]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #9
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #6
	bl sub_803AD84
_0801338A:
	adds r3, r5, #0
	adds r3, #0x28
	ldrb r2, [r3]
	cmp r2, #7
	bne _08013402
	movs r2, #0xa
	adds r1, r5, #0
	adds r1, #0x32
	movs r0, #0
	strb r0, [r1]
	subs r1, #2
	movs r0, #1
	strb r0, [r1]
	strb r2, [r3]
	b _08013402
_080133A8:
	ldr r1, [r5, #0xc]
	movs r6, #0x20
	ldrsh r0, [r1, r6]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #7
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0xc
	bl sub_803AD84
	adds r3, r5, #0
	adds r3, #0x28
	ldrb r2, [r3]
	cmp r2, #7
	bne _08013402
	movs r6, #1
	movs r2, #1
	ands r2, r4
	cmp r2, #0
	beq _080133F4
	movs r0, #9
	adds r2, r5, #0
	adds r2, #0x32
	movs r1, #0
	strb r1, [r2]
	adds r1, r5, #0
	adds r1, #0x30
	strb r6, [r1]
	strb r0, [r3]
	b _08013402
_080133F4:
	movs r1, #8
	adds r0, r5, #0
	adds r0, #0x32
	strb r2, [r0]
	subs r0, #2
	strb r6, [r0]
	strb r1, [r3]
_08013402:
	ldr r0, _08013438 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #2
	bhi _08013440
	ldr r0, _0801343C @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r6, #0x80
	lsls r6, r6, #1
	adds r0, r0, r6
	ldrb r1, [r0]
	cmp r1, #0
	bne _080134A4
	adds r0, r5, #0
	adds r0, #0x31
	strb r1, [r0]
	adds r2, r5, #0
	adds r2, #0x2f
	movs r0, #1
	strb r0, [r2]
	adds r0, r5, #0
	adds r0, #0x27
	strb r1, [r0]
	b _080134A4
	.align 2, 0
_08013438: .4byte gUnknown_03001304
_0801343C: .4byte gUnknown_030012D8
_08013440:
	adds r3, r5, #0
	adds r3, #0x27
	ldrb r1, [r3]
	adds r0, r1, #0
	subs r0, #0x1b
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #1
	bhi _08013460
	movs r1, #0x1c
	adds r2, r5, #0
	adds r2, #0x31
	movs r0, #1
	strb r0, [r2]
	subs r2, #2
	b _080134A0
_08013460:
	ldr r0, [r5, #0x18]
	cmp r0, #0
	beq _08013482
	lsls r0, r1, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #0xd
	beq _080134A4
	movs r0, #0xd
	adds r2, r5, #0
	adds r2, #0x31
	movs r1, #0
	strb r1, [r2]
	subs r2, #2
	movs r1, #1
	strb r1, [r2]
	strb r0, [r3]
	b _080134A4
_08013482:
	ldr r0, _080134B4 @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r1, #0x80
	lsls r1, r1, #1
	adds r0, r0, r1
	ldrb r2, [r0]
	cmp r2, #0
	bne _080134A4
	movs r1, #7
	adds r0, r5, #0
	adds r0, #0x31
	strb r2, [r0]
	adds r2, r5, #0
	adds r2, #0x2f
	movs r0, #1
_080134A0:
	strb r0, [r2]
	strb r1, [r3]
_080134A4:
	adds r0, r5, #0
	bl sub_80122CC
_080134AA:
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_080134B4: .4byte gUnknown_030012D8

	thumb_func_start sub_80134B8
sub_80134B8: @ 0x080134B8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xc
	adds r5, r0, #0
	ldr r0, _08013580 @ =gUnknown_03001304
	ldr r0, [r0]
	ldr r1, _08013584 @ =gUnknown_030007E0
	ldr r1, [r1]
	str r1, [sp, #8]
	ldr r1, [r5, #0x10]
	adds r1, #0x68
	ldrb r7, [r1]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	mov r8, r0
	ldr r2, [r5, #8]
	cmp r2, #0xe
	beq _0801355A
	adds r0, r5, #0
	adds r0, #0x26
	ldrb r6, [r0]
	cmp r6, #0
	bne _0801355A
	add r0, sp, #8
	ldrh r1, [r0, #2]
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _0801355A
	adds r0, r2, #0
	subs r0, #0x18
	cmp r0, #1
	bls _0801355A
	ldr r0, _08013588 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xa
	bl PlaySfx
	movs r4, #0x18
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xe
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x10
	bl sub_803AD84
	str r6, [r5, #0x18]
	str r4, [r5, #0x1c]
	adds r0, r5, #0
	adds r0, #0x21
	strb r6, [r0]
	subs r0, #1
	strb r6, [r0]
	adds r0, #2
	strb r6, [r0]
	adds r0, #1
	strb r6, [r0]
	adds r0, #1
	strb r6, [r0]
	ldr r0, _0801358C @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x92
	strb r6, [r0]
_0801355A:
	cmp r7, #0
	bne _080135BE
	adds r0, r5, #0
	bl sub_80122CC
	adds r1, r5, #0
	adds r1, #0x25
	ldrb r0, [r1]
	cmp r0, #0
	beq _08013590
	subs r0, #1
	strb r0, [r1]
	mov r2, r8
	cmp r2, #0
	beq _0801357A
	b _080138D6
_0801357A:
	strb r7, [r1]
	b _080138D6
	.align 2, 0
_08013580: .4byte gUnknown_03001304
_08013584: .4byte gUnknown_030007E0
_08013588: .4byte gUnknown_030012BC
_0801358C: .4byte gUnknown_030012D8
_08013590:
	adds r0, r5, #0
	bl sub_801283C
	ldr r0, [r5, #8]
	cmp r0, #0x1a
	beq _0801359E
	b _080138D6
_0801359E:
	adds r3, r5, #0
	adds r3, #0x28
	ldrb r0, [r3]
	cmp r0, #0
	beq _080135AA
	b _080138D6
_080135AA:
	movs r2, #4
	adds r0, r5, #0
	adds r0, #0x32
	strb r7, [r0]
	adds r1, r5, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	strb r2, [r3]
	b _080138D6
_080135BE:
	movs r0, #4
	ands r0, r7
	lsls r0, r0, #0x18
	lsrs r6, r0, #0x18
	cmp r6, #0
	beq _08013648
	ldr r0, [r5, #8]
	cmp r0, #0x1a
	beq _08013632
	ldr r1, [r5, #0x10]
	movs r0, #1
	ldrb r6, [r1, #0xd]
	orrs r0, r6
	strb r0, [r1, #0xd]
	adds r1, r5, #0
	adds r1, #0x34
	movs r0, #0
	strb r0, [r1]
	ldr r0, [r5, #8]
	cmp r0, #0xe
	beq _0801362C
	ldr r1, [r5, #0xc]
	movs r7, #0x20
	ldrsh r0, [r1, r7]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x1a
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x15
	bl sub_803AD84
	ldr r3, [r5, #0x10]
	movs r4, #2
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
	blt _0801362A
	subs r4, r0, #1
_0801362A:
	str r4, [r3, #0x30]
_0801362C:
	ldr r0, [r5, #0x10]
	bl sub_800B334
_08013632:
	adds r0, r5, #0
	bl sub_80122CC
	adds r0, r5, #0
	bl sub_801283C
	ldr r0, [r5, #0x10]
	movs r1, #0
	adds r0, #0x68
	strb r1, [r0]
	b _080138D6
_08013648:
	subs r0, r7, #1
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #1
	bhi _08013666
	adds r0, r5, #0
	bl sub_80122CC
	adds r0, r5, #0
	bl sub_801283C
	ldr r0, [r5, #0x10]
	adds r0, #0x68
	strb r6, [r0]
	b _080138D6
_08013666:
	movs r0, #8
	ands r7, r0
	cmp r7, #0
	bne _08013670
	b _080138C0
_08013670:
	ldr r1, [r5, #0x10]
	ldr r0, [r1, #0x64]
	cmp r0, #0
	bge _0801367A
	b _080138C0
_0801367A:
	ldrb r0, [r1, #0xd]
	movs r7, #1
	orrs r0, r7
	strb r0, [r1, #0xd]
	adds r0, r5, #0
	adds r0, #0x34
	strb r6, [r0]
	ldr r1, [r5, #8]
	adds r0, r1, #0
	subs r0, #0x18
	cmp r0, #1
	bls _08013694
	b _080137C8
_08013694:
	ldr r1, _080137BC @ =gUnknown_030012D8
	ldr r0, [r1]
	ldr r3, [r0]
	asrs r3, r3, #8
	adds r3, #0x14
	ldr r1, [r0, #4]
	asrs r1, r1, #8
	adds r1, #0xc
	ldr r2, _080137C0 @ =gUnknown_030012E4
	ldr r0, [r2]
	str r1, [sp]
	str r7, [sp, #4]
	movs r1, #0x29
	movs r2, #1
	bl sub_8025BAC
	adds r3, r0, #0
	adds r2, r3, #0
	adds r2, #0x28
	movs r0, #4
	rsbs r0, r0, #0
	mov sl, r0
	ldrb r1, [r2]
	ands r0, r1
	orrs r0, r7
	strb r0, [r2]
	movs r0, #5
	rsbs r0, r0, #0
	mov sb, r0
	ldrb r1, [r3, #0xc]
	ands r0, r1
	strb r0, [r3, #0xc]
	subs r7, #0x12
	mov r8, r7
	mov r0, r8
	ldrb r1, [r2]
	ands r0, r1
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r2]
	movs r4, #3
	ldr r0, [r3, #0x20]
	adds r2, #5
	ldr r1, [r0]
	ldrb r7, [r2]
	lsls r0, r7, #3
	adds r2, r7, #0
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _08013700
	subs r4, r0, #1
_08013700:
	str r4, [r3, #0x30]
	ldr r1, _080137BC @ =gUnknown_030012D8
	ldr r0, [r1]
	ldr r3, [r0]
	asrs r3, r3, #8
	subs r3, #0x14
	ldr r1, [r0, #4]
	asrs r1, r1, #8
	adds r1, #0xc
	ldr r2, _080137C0 @ =gUnknown_030012E4
	ldr r0, [r2]
	str r1, [sp]
	str r6, [sp, #4]
	movs r1, #0x29
	movs r2, #1
	bl sub_8025BAC
	adds r3, r0, #0
	adds r1, r3, #0
	adds r1, #0x28
	mov r0, sl
	ldrb r7, [r1]
	ands r0, r7
	movs r2, #1
	orrs r0, r2
	strb r0, [r1]
	mov r0, sb
	ldrb r7, [r3, #0xc]
	ands r0, r7
	strb r0, [r3, #0xc]
	mov r0, r8
	ldrb r2, [r1]
	ands r0, r2
	strb r0, [r1]
	movs r4, #3
	ldr r0, [r3, #0x20]
	adds r2, r3, #0
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
	blt _08013762
	subs r4, r0, #1
_08013762:
	str r4, [r3, #0x30]
	ldr r0, [r5, #8]
	cmp r0, #0x19
	bne _08013770
	adds r0, r5, #0
	bl sub_8014F8C
_08013770:
	ldr r0, [r5, #8]
	cmp r0, #0x1d
	bne _08013778
	b _080138D6
_08013778:
	ldr r0, _080137C4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x19
	bl PlaySfx
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x16
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r7, #0
	ldrsh r0, [r2, r7]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x11
	bl sub_803AD84
	adds r0, r5, #0
	adds r0, #0x32
	strb r6, [r0]
	subs r0, #2
	movs r1, #1
	strb r1, [r0]
	subs r0, #8
	strb r6, [r0]
	b _080138D6
	.align 2, 0
_080137BC: .4byte gUnknown_030012D8
_080137C0: .4byte gUnknown_030012E4
_080137C4: .4byte gUnknown_030012BC
_080137C8:
	cmp r1, #0xe
	bne _08013840
	add r0, sp, #8
	movs r2, #0x30
	ldrh r0, [r0]
	ands r2, r0
	cmp r2, #0
	beq _080137EA
	movs r0, #1
	adds r1, r5, #0
	adds r1, #0x31
	strb r6, [r1]
	subs r1, #2
	strb r0, [r1]
	subs r1, #8
	strb r0, [r1]
	b _080137FE
_080137EA:
	adds r0, r5, #0
	adds r0, #0x31
	strb r2, [r0]
	adds r1, r5, #0
	adds r1, #0x2f
	movs r0, #1
	strb r0, [r1]
	adds r0, r5, #0
	adds r0, #0x27
	strb r2, [r0]
_080137FE:
	movs r2, #0
	adds r0, r5, #0
	adds r0, #0x32
	strb r2, [r0]
	adds r1, r5, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	adds r0, r5, #0
	adds r0, #0x28
	strb r2, [r0]
	subs r0, #6
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801382E
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xf
	bl sub_803AD80
	b _080138D6
_0801382E:
	ldr r1, [r5, #0xc]
	movs r6, #0x20
	ldrsh r0, [r1, r6]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xd
	bl sub_803AD80
	b _080138D6
_08013840:
	ldr r0, _08013878 @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r7, #0x80
	lsls r7, r7, #1
	adds r0, r0, r7
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801387C
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x17
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r6, #0
	ldrsh r0, [r2, r6]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x16
	bl sub_803AD84
	b _080138A0
	.align 2, 0
_08013878: .4byte gUnknown_030012D8
_0801387C:
	ldr r1, [r5, #0xc]
	movs r7, #0x20
	ldrsh r0, [r1, r7]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x17
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x16
	bl sub_803AD84
_080138A0:
	movs r1, #0
	adds r0, r5, #0
	adds r0, #0x31
	strb r1, [r0]
	subs r0, #2
	movs r2, #1
	strb r2, [r0]
	subs r0, #8
	strb r1, [r0]
	adds r0, #0xb
	strb r1, [r0]
	subs r0, #2
	strb r2, [r0]
	subs r0, #8
	strb r1, [r0]
	b _080138D6
_080138C0:
	movs r0, #0
	adds r1, r5, #0
	adds r1, #0x31
	strb r0, [r1]
	adds r2, r5, #0
	adds r2, #0x2f
	movs r1, #1
	strb r1, [r2]
	adds r1, r5, #0
	adds r1, #0x27
	strb r0, [r1]
_080138D6:
	add sp, #0xc
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80138E8
sub_80138E8: @ 0x080138E8
	push {r4, lr}
	adds r4, r0, #0
	ldr r2, [r4, #0x10]
	adds r0, r2, #0
	adds r0, #0x2d
	ldrb r0, [r0]
	cmp r0, #6
	bne _08013938
	ldr r0, [r2, #0x30]
	cmp r0, #3
	bne _08013914
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r2, #0
	movs r2, #9
	bl sub_803AD84
	b _0801398C
_08013914:
	cmp r0, #3
	bgt _08013922
	adds r0, r2, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801398C
_08013922:
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r2, #0
	movs r2, #8
	bl sub_803AD84
	b _0801398C
_08013938:
	adds r0, r2, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801398C
	ldr r0, _08013978 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231BC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801397C
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x19
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r3, #0
	ldrsh r0, [r2, r3]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #7
	bl sub_803AD84
	b _0801398C
	.align 2, 0
_08013978: .4byte gUnknown_030012C0
_0801397C:
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x18
	bl sub_803AD80
_0801398C:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8013994
sub_8013994: @ 0x08013994
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	ldr r0, _080139C4 @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp]
	ldr r2, [r5, #0x10]
	adds r0, r2, #0
	adds r0, #0x68
	ldrb r1, [r0]
	cmp r1, #0
	bne _080139C8
	movs r2, #5
	adds r0, r5, #0
	adds r0, #0x32
	strb r1, [r0]
	adds r1, r5, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	adds r0, r5, #0
	adds r0, #0x28
	strb r2, [r0]
	b _08013A4A
	.align 2, 0
_080139C4: .4byte gUnknown_030007E0
_080139C8:
	mov r0, sp
	movs r6, #1
	movs r4, #1
	ldrh r0, [r0]
	ands r4, r0
	cmp r4, #0
	beq _08013A18
	adds r0, r2, #0
	movs r1, #0xb
	bl sub_800AAEC
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #1
	bne _08013A4A
	ldr r0, _08013A14 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xc
	bl PlaySfx
	ldr r1, [r5, #0x10]
	movs r0, #2
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xd]
	ands r0, r2
	strb r0, [r1, #0xd]
	ldr r1, [r5, #0x10]
	movs r0, #3
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xd]
	ands r0, r2
	strb r0, [r1, #0xd]
	adds r0, r5, #0
	bl sub_8015508
	b _08013C58
	.align 2, 0
_08013A14: .4byte gUnknown_030012BC
_08013A18:
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _08013A4A
	adds r0, r2, #0
	movs r1, #0x10
	bl sub_800AAEC
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #1
	bne _08013A4A
	adds r0, r5, #0
	bl sub_8015398
	adds r0, r5, #0
	adds r0, #0x31
	strb r4, [r0]
	subs r0, #2
	strb r6, [r0]
	subs r0, #8
	strb r6, [r0]
	b _08013C58
_08013A4A:
	ldr r0, [r5, #0x18]
	adds r0, #1
	str r0, [r5, #0x18]
	ldr r1, [r5, #0x1c]
	cmp r0, r1
	bge _08013A7C
	ldr r3, [r5, #0x10]
	movs r0, #0
	str r0, [r3, #0x34]
	movs r4, #3
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
	blt _08013A78
	subs r4, r0, #1
_08013A78:
	str r4, [r3, #0x30]
	b _08013C58
_08013A7C:
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	bne _08013A8A
	b _08013C58
_08013A8A:
	adds r0, r1, #0
	adds r0, #0x68
	ldrb r4, [r0]
	cmp r4, #0
	bne _08013AD0
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x1a
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x1b
	bl sub_803AD84
	movs r2, #4
	adds r0, r5, #0
	adds r0, #0x32
	strb r4, [r0]
	adds r1, r5, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	adds r0, r5, #0
	adds r0, #0x28
	strb r2, [r0]
	b _08013C58
_08013AD0:
	mov r1, sp
	movs r0, #0x80
	lsls r0, r0, #1
	ldrh r1, [r1]
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r6, r0, #0x10
	cmp r6, #0
	beq _08013B28
	movs r4, #0
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x14
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0
	bl sub_803AD84
	str r4, [r5, #0x1c]
	movs r1, #3
	adds r0, r5, #0
	adds r0, #0x31
	strb r4, [r0]
	adds r2, r5, #0
	adds r2, #0x2f
	movs r0, #1
	strb r0, [r2]
	adds r0, r5, #0
	adds r0, #0x27
	strb r1, [r0]
	adds r0, r5, #0
	bl sub_801434C
	b _08013C58
_08013B28:
	ldr r0, _08013BA8 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r4, r0, #0x18
	cmp r4, #0
	beq _08013BDA
	ldr r0, [r5, #0x10]
	movs r1, #2
	bl sub_800AAEC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08013BDA
	cmp r4, #4
	bgt _08013BB8
	cmp r4, #3
	blt _08013BB8
	mov r1, sp
	movs r0, #0x80
	lsls r0, r0, #2
	ldrh r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _08013BB0
	ldr r0, _08013BAC @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231C4
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08013BB0
	adds r0, r5, #0
	adds r0, #0x29
	movs r4, #1
	strb r4, [r0]
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #4
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x18
	bl sub_803AD84
	movs r1, #0x1b
	adds r0, r5, #0
	adds r0, #0x31
	strb r6, [r0]
	subs r0, #2
	strb r4, [r0]
	subs r0, #8
	b _08013C56
	.align 2, 0
_08013BA8: .4byte gUnknown_03001304
_08013BAC: .4byte gUnknown_030012C0
_08013BB0:
	adds r0, r5, #0
	bl sub_8015460
	b _08013C58
_08013BB8:
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x12
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #2
	b _08013C3E
_08013BDA:
	ldr r0, [r5, #0x10]
	movs r1, #2
	bl sub_800AAEC
	lsls r0, r0, #0x18
	lsrs r4, r0, #0x18
	cmp r4, #1
	bne _08013C1E
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x12
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #2
	bl sub_803AD84
	movs r1, #0
	adds r0, r5, #0
	adds r0, #0x31
	strb r1, [r0]
	subs r0, #2
	strb r4, [r0]
	subs r0, #8
	b _08013C56
_08013C1E:
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x11
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #4
_08013C3E:
	bl sub_803AD84
	movs r1, #0
	adds r0, r5, #0
	adds r0, #0x31
	strb r1, [r0]
	adds r2, r5, #0
	adds r2, #0x2f
	movs r0, #1
	strb r0, [r2]
	adds r0, r5, #0
	adds r0, #0x27
_08013C56:
	strb r1, [r0]
_08013C58:
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0

	thumb_func_start sub_8013C60
sub_8013C60: @ 0x08013C60
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _08013D04 @ =gUnknown_03001304
	ldr r0, [r0]
	ldr r1, _08013D08 @ =gUnknown_030007E0
	ldr r1, [r1]
	str r1, [sp]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	ldr r0, [r4, #0x10]
	adds r0, #0x68
	ldrb r1, [r0]
	cmp r1, #0
	bne _08013C98
	movs r2, #5
	adds r0, r4, #0
	adds r0, #0x32
	strb r1, [r0]
	adds r1, r4, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x28
	strb r2, [r0]
_08013C98:
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08013D0C
	ldr r2, [r4, #0x10]
	adds r1, r2, #0
	adds r1, #0x68
	movs r0, #8
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _08013D0C
	movs r0, #2
	rsbs r0, r0, #0
	ldrb r1, [r2, #0xd]
	ands r0, r1
	strb r0, [r2, #0xd]
	ldr r1, [r4, #0x10]
	movs r0, #3
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xd]
	ands r0, r2
	strb r0, [r1, #0xd]
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xe
	bl sub_803AD80
	movs r3, #7
	adds r0, r4, #0
	adds r0, #0x32
	movs r2, #0
	strb r2, [r0]
	adds r1, r4, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x28
	strb r3, [r0]
	subs r0, #6
	strb r2, [r0]
	adds r0, #1
	strb r2, [r0]
	ldr r0, [r4, #0x10]
	adds r0, #0x68
	strb r2, [r0]
	b _08013D8A
	.align 2, 0
_08013D04: .4byte gUnknown_03001304
_08013D08: .4byte gUnknown_030007E0
_08013D0C:
	ldr r0, _08013D7C @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231B4
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08013D46
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _08013D46
	adds r0, r4, #0
	adds r0, #0x26
	ldrb r0, [r0]
	cmp r0, #0
	bne _08013D46
	adds r1, r4, #0
	adds r1, #0x20
	ldrb r0, [r1]
	adds r0, #1
	strb r0, [r1]
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #3
	bls _08013D46
	movs r0, #3
	strb r0, [r1]
_08013D46:
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_80152F0
	ldr r0, [r4, #0x18]
	adds r0, #1
	str r0, [r4, #0x18]
	ldr r1, [r4, #0x1c]
	cmp r0, r1
	bge _08013D64
	ldr r0, [r4, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08013D8A
_08013D64:
	adds r0, r4, #0
	adds r0, #0x20
	ldrb r0, [r0]
	cmp r0, #0
	beq _08013D80
	adds r0, r4, #0
	movs r1, #0xf
	movs r2, #0xd
	bl sub_8015038
	b _08013D8A
	.align 2, 0
_08013D7C: .4byte gUnknown_030012C0
_08013D80:
	ldr r2, [sp]
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_8015238
_08013D8A:
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8013D94
sub_8013D94: @ 0x08013D94
	push {r4, r5, lr}
	sub sp, #4
	adds r5, r0, #0
	ldr r0, _08013DF4 @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp]
	ldr r2, [r5, #0x10]
	adds r1, r2, #0
	adds r1, #0x68
	movs r0, #8
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _08013DF8
	ldr r0, [r2, #0x64]
	cmp r0, #0
	ble _08013DF8
	movs r0, #1
	ldrb r1, [r2, #0xd]
	orrs r0, r1
	strb r0, [r2, #0xd]
	adds r0, r5, #0
	adds r0, #0x34
	movs r4, #0
	strb r4, [r0]
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xd
	bl sub_803AD80
	adds r0, r5, #0
	adds r0, #0x32
	strb r4, [r0]
	adds r1, r5, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	adds r0, r5, #0
	adds r0, #0x28
	strb r4, [r0]
	adds r0, r5, #0
	bl sub_8013C60
	b _08013EA2
	.align 2, 0
_08013DF4: .4byte gUnknown_030007E0
_08013DF8:
	ldr r0, _08013E68 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231B4
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08013E32
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _08013E32
	adds r0, r5, #0
	adds r0, #0x26
	ldrb r0, [r0]
	cmp r0, #0
	bne _08013E32
	adds r1, r5, #0
	adds r1, #0x20
	ldrb r0, [r1]
	adds r0, #1
	strb r0, [r1]
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #3
	bls _08013E32
	movs r0, #3
	strb r0, [r1]
_08013E32:
	ldr r0, [r5, #0x18]
	adds r0, #1
	str r0, [r5, #0x18]
	ldr r1, [r5, #0x1c]
	ldr r2, [r5, #0x10]
	cmp r0, r1
	bge _08013E4A
	adds r0, r2, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08013E9C
_08013E4A:
	movs r0, #1
	ldrb r1, [r2, #0xd]
	orrs r0, r1
	strb r0, [r2, #0xd]
	adds r0, r5, #0
	adds r0, #0x20
	ldrb r4, [r0]
	cmp r4, #0
	beq _08013E6C
	adds r0, r5, #0
	movs r1, #0xe
	movs r2, #0xe
	bl sub_8015038
	b _08013E9C
	.align 2, 0
_08013E68: .4byte gUnknown_030012C0
_08013E6C:
	adds r1, r5, #0
	adds r1, #0x26
	movs r0, #0xc
	strb r0, [r1]
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x1a
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x15
	bl sub_803AD84
	str r4, [r5, #0x18]
	str r4, [r5, #0x1c]
_08013E9C:
	adds r0, r5, #0
	bl sub_80134B8
_08013EA2:
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8013EAC
sub_8013EAC: @ 0x08013EAC
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _08013EE0 @ =gUnknown_03001304
	ldr r0, [r0]
	ldr r1, _08013EE4 @ =gUnknown_030007E0
	ldr r1, [r1]
	str r1, [sp]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	ldr r0, [r4, #0x10]
	adds r0, #0x68
	ldrb r0, [r0]
	cmp r0, #0
	bne _08013EFE
	adds r0, r4, #0
	adds r0, #0x22
	ldrb r1, [r0]
	cmp r1, #0
	beq _08013EE8
	adds r0, r4, #0
	bl sub_80151C8
	b _08013EFE
	.align 2, 0
_08013EE0: .4byte gUnknown_03001304
_08013EE4: .4byte gUnknown_030007E0
_08013EE8:
	movs r2, #5
	adds r0, r4, #0
	adds r0, #0x32
	strb r1, [r0]
	adds r1, r4, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x28
	strb r2, [r0]
_08013EFE:
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08013F64
	ldr r2, [r4, #0x10]
	adds r1, r2, #0
	adds r1, #0x68
	movs r0, #8
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _08013F64
	movs r0, #2
	rsbs r0, r0, #0
	ldrb r1, [r2, #0xd]
	ands r0, r1
	strb r0, [r2, #0xd]
	ldr r1, [r4, #0x10]
	movs r0, #3
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xd]
	ands r0, r2
	strb r0, [r1, #0xd]
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xe
	bl sub_803AD80
	movs r3, #7
	adds r0, r4, #0
	adds r0, #0x32
	movs r2, #0
	strb r2, [r0]
	adds r1, r4, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x28
	strb r3, [r0]
	subs r0, #5
	strb r2, [r0]
	ldr r0, [r4, #0x10]
	adds r0, #0x68
	strb r2, [r0]
	b _08013FC6
_08013F64:
	ldr r0, _08013FD0 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231B4
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08013F9E
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _08013F9E
	adds r0, r4, #0
	adds r0, #0x26
	ldrb r0, [r0]
	cmp r0, #0
	bne _08013F9E
	adds r1, r4, #0
	adds r1, #0x20
	ldrb r0, [r1]
	adds r0, #1
	strb r0, [r1]
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #3
	bls _08013F9E
	movs r0, #3
	strb r0, [r1]
_08013F9E:
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_80152F0
	ldr r0, [r4, #0x18]
	adds r0, #1
	str r0, [r4, #0x18]
	ldr r1, [r4, #0x1c]
	cmp r0, r1
	bge _08013FBC
	ldr r0, [r4, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08013FC6
_08013FBC:
	adds r0, r4, #0
	movs r1, #0xf
	movs r2, #0xd
	bl sub_8015038
_08013FC6:
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08013FD0: .4byte gUnknown_030012C0

	thumb_func_start sub_8013FD4
sub_8013FD4: @ 0x08013FD4
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _0801401C @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp]
	mov r0, sp
	ldrh r0, [r0, #2]
	movs r2, #1
	ands r2, r0
	cmp r2, #0
	beq _08014024
	ldr r0, _08014020 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xc
	bl PlaySfx
	ldr r1, [r4, #0x10]
	movs r0, #2
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xd]
	ands r0, r2
	strb r0, [r1, #0xd]
	ldr r1, [r4, #0x10]
	movs r0, #3
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xd]
	ands r0, r2
	strb r0, [r1, #0xd]
	adds r0, r4, #0
	bl sub_8015508
	b _0801407C
	.align 2, 0
_0801401C: .4byte gUnknown_030007E0
_08014020: .4byte gUnknown_030012BC
_08014024:
	ldr r0, [r4, #0x10]
	adds r0, #0x68
	ldrb r0, [r0]
	cmp r0, #8
	bne _0801404E
	adds r3, r4, #0
	adds r3, #0x28
	ldrb r0, [r3]
	subs r0, #4
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #1
	bhi _0801404E
	adds r0, r4, #0
	adds r0, #0x32
	strb r2, [r0]
	adds r1, r4, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	strb r2, [r3]
_0801404E:
	ldr r0, [r4, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801407C
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x11
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #4
	bl sub_803AD84
_0801407C:
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8014084
sub_8014084: @ 0x08014084
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _080140E8 @ =gUnknown_03001304
	ldr r0, [r0]
	ldr r1, _080140EC @ =gUnknown_030007E0
	ldr r1, [r1]
	str r1, [sp]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _080140F4
	ldr r0, [r4, #0x10]
	movs r1, #0xb
	bl sub_800AAEC
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #1
	bne _080140F4
	ldr r0, _080140F0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xc
	bl PlaySfx
	ldr r1, [r4, #0x10]
	movs r0, #2
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xd]
	ands r0, r2
	strb r0, [r1, #0xd]
	ldr r1, [r4, #0x10]
	movs r0, #3
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xd]
	ands r0, r2
	strb r0, [r1, #0xd]
	adds r0, r4, #0
	bl sub_8015508
	b _08014264
	.align 2, 0
_080140E8: .4byte gUnknown_03001304
_080140EC: .4byte gUnknown_030007E0
_080140F0: .4byte gUnknown_030012BC
_080140F4:
	adds r0, r4, #0
	bl sub_8012A7C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08014102
	b _08014264
_08014102:
	movs r3, #0
	ldr r0, [r4, #0x10]
	adds r1, r0, #0
	adds r1, #0x28
	ldrb r1, [r1]
	lsls r1, r1, #0x1b
	adds r2, r0, #0
	cmp r1, #0
	bge _0801413A
	cmp r5, #4
	beq _08014120
	cmp r5, #6
	beq _08014120
	cmp r5, #8
	bne _0801413A
_08014120:
	adds r0, r2, #0
	adds r0, #0x28
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r2, [r0]
	ands r1, r2
	strb r1, [r0]
	adds r1, r4, #0
	adds r1, #0x2f
	movs r0, #1
	strb r0, [r1]
	movs r3, #1
	b _0801416A
_0801413A:
	adds r0, r2, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	blt _0801416A
	cmp r5, #3
	beq _08014152
	cmp r5, #5
	beq _08014152
	cmp r5, #7
	bne _0801416A
_08014152:
	movs r3, #1
	adds r2, #0x28
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r1, [r2]
	ands r0, r1
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r2]
	adds r0, r4, #0
	adds r0, #0x2f
	strb r3, [r0]
_0801416A:
	movs r6, #0
	cmp r3, #0
	bne _080141C8
	ldr r0, _0801421C @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #3
	blt _080141C8
	cmp r0, #4
	ble _0801418C
	cmp r0, #8
	bgt _080141C8
	cmp r0, #7
	blt _080141C8
_0801418C:
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x13
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x14
	bl sub_803AD84
	movs r1, #3
	adds r0, r4, #0
	adds r0, #0x31
	strb r6, [r0]
	adds r2, r4, #0
	adds r2, #0x2f
	movs r0, #1
	strb r0, [r2]
	adds r0, r4, #0
	adds r0, #0x27
	strb r1, [r0]
	movs r6, #1
_080141C8:
	mov r0, sp
	movs r5, #0xc0
	lsls r5, r5, #1
	ldrh r0, [r0]
	ands r5, r0
	cmp r5, #0
	bne _08014264
	ldr r0, [r4, #0x10]
	movs r1, #2
	bl sub_800AAEC
	lsls r0, r0, #0x18
	lsrs r7, r0, #0x18
	cmp r7, #1
	bne _08014220
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x12
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #2
	bl sub_803AD84
	adds r0, r4, #0
	adds r0, #0x31
	strb r5, [r0]
	subs r0, #2
	strb r7, [r0]
	subs r0, #8
	strb r5, [r0]
	b _08014264
	.align 2, 0
_0801421C: .4byte gUnknown_03001304
_08014220:
	cmp r6, #0
	bne _08014264
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x11
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #4
	bl sub_803AD84
	adds r0, r4, #0
	adds r0, #0x31
	strb r6, [r0]
	subs r0, #2
	movs r1, #1
	strb r1, [r0]
	subs r0, #8
	strb r6, [r0]
	adds r0, #0xb
	strb r6, [r0]
	subs r0, #2
	strb r1, [r0]
	subs r0, #8
	strb r6, [r0]
_08014264:
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

