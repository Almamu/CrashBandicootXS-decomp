.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8034AA4
sub_8034AA4: @ 0x08034AA4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	adds r6, r0, #0
	ldr r0, _08034C34 @ =gUnknown_03001300
	mov sl, r0
	ldr r0, [r0]
	bl sub_8006A90
	ldr r0, _08034C38 @ =gUnknown_030012FC
	ldr r0, [r0]
	bl sub_8006C28
	ldr r4, [r6, #0x18]
	movs r1, #0x98
	lsls r1, r1, #1
	mov r8, r1
	adds r0, r4, r1
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x10
	movs r2, #0x10
	ldrsh r0, [r0, r2]
	adds r4, r4, r0
	movs r0, #0x28
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	adds r4, r0, #0
	ldr r0, [r6, #0x18]
	movs r1, #0
	bl sub_8028A30
	movs r1, #0x88
	subs r1, r1, r4
	ldr r4, [r6, #0x18]
	movs r7, #0x87
	movs r3, #0x88
	lsls r3, r3, #1
	adds r0, r4, r3
	str r1, [r0]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r4, r1
	str r7, [r0]
	mov r2, r8
	adds r0, r4, r2
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r3, #0x20
	ldrsh r0, [r0, r3]
	adds r4, r4, r0
	movs r0, #0x28
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	ldr r4, [r6, #0x18]
	adds r0, r6, #0
	movs r1, #0
	bl sub_8034C40
	adds r1, r0, #0
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	adds r0, r4, #0
	bl sub_8028A30
	ldr r0, [r6, #0x20]
	cmp r0, #0
	bne _08034B6E
	ldr r3, [r6, #0x18]
	movs r1, #0x90
	movs r4, #0x88
	lsls r4, r4, #1
	adds r0, r3, r4
	str r1, [r0]
	adds r1, #0x84
	adds r0, r3, r1
	str r7, [r0]
	movs r2, #0x98
	lsls r2, r2, #1
	adds r0, r3, r2
	ldr r2, [r0]
	movs r4, #0x20
	ldrsh r0, [r2, r4]
	adds r0, r3, r0
	ldr r1, _08034C3C @ =gStaticData_0817C510
	ldr r2, [r2, #0x24]
	bl sub_803AD80
_08034B6E:
	ldr r4, [r6, #0x18]
	movs r0, #0x98
	mov sb, r0
	movs r1, #0x88
	lsls r1, r1, #1
	adds r0, r4, r1
	mov r2, sb
	str r2, [r0]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r0, r4, r3
	str r7, [r0]
	mov r1, r8
	adds r0, r4, r1
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r2, #0x20
	ldrsh r0, [r0, r2]
	adds r4, r4, r0
	movs r0, #0x29
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	ldr r4, [r6, #0x18]
	adds r0, r6, #0
	movs r1, #1
	bl sub_8034C40
	adds r1, r0, #0
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	adds r0, r4, #0
	bl sub_8028A30
	ldr r0, [r6, #0x20]
	cmp r0, #1
	bne _08034BEA
	ldr r0, [r6, #0x18]
	movs r2, #0x90
	movs r3, #0x91
	movs r4, #0x88
	lsls r4, r4, #1
	adds r1, r0, r4
	str r2, [r1]
	adds r2, #0x84
	adds r1, r0, r2
	str r3, [r1]
	mov r3, r8
	adds r1, r0, r3
	ldr r2, [r1]
	movs r4, #0x20
	ldrsh r1, [r2, r4]
	adds r0, r0, r1
	ldr r1, _08034C3C @ =gStaticData_0817C510
	ldr r2, [r2, #0x24]
	bl sub_803AD80
_08034BEA:
	ldr r4, [r6, #0x18]
	movs r1, #0x91
	movs r2, #0x88
	lsls r2, r2, #1
	adds r0, r4, r2
	mov r3, sb
	str r3, [r0]
	adds r2, #4
	adds r0, r4, r2
	str r1, [r0]
	mov r3, r8
	adds r0, r4, r3
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r1, #0x20
	ldrsh r0, [r0, r1]
	adds r4, r4, r0
	movs r0, #0x2a
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	mov r2, sl
	ldr r0, [r2]
	bl sub_8006A48
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08034C34: .4byte gUnknown_03001300
_08034C38: .4byte gUnknown_030012FC
_08034C3C: .4byte gStaticData_0817C510

	thumb_func_start sub_8034C40
sub_8034C40: @ 0x08034C40
	adds r3, r0, #0
	ldr r0, [r3, #0x20]
	cmp r1, r0
	beq _08034C4C
	movs r0, #1
	b _08034C58
_08034C4C:
	ldr r1, [r3, #0x1c]
	asrs r0, r1, #1
	movs r2, #2
	ands r0, r2
	adds r1, #1
	str r1, [r3, #0x1c]
_08034C58:
	bx lr
	.align 2, 0

	thumb_func_start sub_8034C5C
sub_8034C5C: @ 0x08034C5C
	push {r4, lr}
	adds r4, r0, #0
	bl sub_80006A8
	ldr r0, _08034C80 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	movs r1, #0x80
	lsls r1, r1, #0x13
	ldrh r0, [r4, #0xc]
	strh r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08034C80: .4byte gUnknown_03001300

	thumb_func_start sub_8034C84
sub_8034C84: @ 0x08034C84
	push {r4, r5, lr}
	adds r5, r0, #0
	adds r4, r1, #0
	ldr r0, [r5, #4]
	bl sub_8026ED0
	ldr r0, [r5]
	bl sub_8026ED0
	ldr r0, [r5, #8]
	bl sub_8026ED0
	movs r0, #1
	ands r0, r4
	cmp r0, #0
	beq _08034CAA
	adds r0, r5, #0
	bl sub_8026ED0
_08034CAA:
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_8034CB0
sub_8034CB0: @ 0x08034CB0
	push {r4, r5, r6, lr}
	movs r6, #0xc0
	lsls r6, r6, #0x18
	adds r0, r6, #0
	bl mem_free_bytes
	movs r0, #0x24
	bl sub_8026EDC
	bl sub_803472C
	adds r4, r0, #0
	bl sub_8034994
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	cmp r4, #0
	beq _08034CDC
	adds r0, r4, #0
	movs r1, #3
	bl sub_8034C84
_08034CDC:
	adds r0, r6, #0
	bl mem_free_bytes
	adds r0, r5, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8034CEC
sub_8034CEC: @ 0x08034CEC
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	adds r5, r0, #0
	movs r0, #0x14
	bl sub_8026EDC
	bl sub_8034374
	str r0, [r5, #0xc]
	ldr r4, _08034E0C @ =gUnknown_03001300
	ldr r0, [r4]
	bl sub_8006A90
	ldr r0, [r4]
	bl sub_8006A48
	bl sub_80006A8
	ldr r0, [r4]
	bl sub_8006AAC
	ldr r4, _08034E10 @ =gUnknown_030012B8
	ldr r0, [r4]
	bl sub_8006EA8
	ldr r6, _08034E14 @ =gUnknown_030012DC
	ldr r0, [r6]
	bl sub_8028A40
	ldr r0, _08034E18 @ =gUnknown_030012E0
	mov sb, r0
	ldr r0, [r0]
	movs r1, #0
	bl sub_8028A30
	adds r0, r5, #0
	bl sub_80352AC
	ldr r0, [r4]
	bl sub_8006DC8
	ldr r4, _08034E1C @ =gUnknown_030012FC
	ldr r0, [r4]
	movs r1, #0
	mov r8, r1
	str r1, [r0, #8]
	bl sub_8006C4C
	ldr r0, [r4]
	bl sub_8006C4C
	ldr r0, [r6]
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
	ldr r0, [r4]
	ldr r1, [r6]
	movs r2, #0x96
	lsls r2, r2, #1
	adds r1, r1, r2
	ldr r1, [r1]
	lsls r1, r1, #5
	bl sub_8006C58
	ldr r0, [r6]
	movs r3, #0x96
	lsls r3, r3, #1
	adds r0, r0, r3
	ldr r2, [r0]
	mov r1, sb
	ldr r0, [r1]
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
	ldr r0, [r4]
	mov r2, sb
	ldr r1, [r2]
	movs r3, #0x96
	lsls r3, r3, #1
	adds r1, r1, r3
	ldr r1, [r1]
	lsls r1, r1, #5
	bl sub_8006C58
	ldr r0, [r4]
	bl sub_8006C30
	mov r0, r8
	str r0, [r5]
	str r0, [r5, #0x10]
	ldr r0, _08034E20 @ =gStaticData_0817C5D0
	str r0, [r5, #4]
	str r0, [r5, #8]
	mov r1, r8
	str r1, [r5, #0x14]
	ldr r1, _08034E24 @ =gUnknown_03001288
	movs r0, #0x10
	ldrb r2, [r1, #1]
	orrs r0, r2
	strb r0, [r1, #1]
	bl sub_8001614
	adds r0, r5, #0
	adds r0, #0x94
	mov r3, r8
	str r3, [r0]
	ldr r0, _08034E28 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x11
	bl sub_8001B54
	adds r0, r5, #0
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08034E0C: .4byte gUnknown_03001300
_08034E10: .4byte gUnknown_030012B8
_08034E14: .4byte gUnknown_030012DC
_08034E18: .4byte gUnknown_030012E0
_08034E1C: .4byte gUnknown_030012FC
_08034E20: .4byte gStaticData_0817C5D0
_08034E24: .4byte gUnknown_03001288
_08034E28: .4byte gUnknown_030012BC

	thumb_func_start sub_8034E2C
sub_8034E2C: @ 0x08034E2C
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	adds r4, r5, #0
	adds r4, #0x94
	b _08034E60
_08034E36:
	ldr r0, [r4]
	adds r0, #1
	movs r1, #1
	ands r0, r1
	str r0, [r4]
	cmp r0, #0
	beq _08034E4A
	adds r0, r5, #0
	bl sub_80350A4
_08034E4A:
	adds r0, r5, #0
	bl sub_8034EF0
	bl sub_80006A8
	adds r0, r5, #0
	bl sub_803544C
	ldr r0, [r5, #0xc]
	bl sub_8034688
_08034E60:
	ldr r0, _08034EDC @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r1, _08034EE0 @ =gUnknown_030007E0
	movs r0, #9
	ldrh r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _08034E36
	ldr r0, _08034EE4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8001AC4
	movs r4, #0
	adds r6, r5, #0
	adds r6, #0x94
	ldr r7, _08034EE8 @ =0x04000050
_08034E86:
	ldr r0, [r6]
	adds r0, #1
	movs r1, #1
	ands r0, r1
	str r0, [r6]
	cmp r0, #0
	beq _08034E9A
	adds r0, r5, #0
	bl sub_80350A4
_08034E9A:
	adds r0, r5, #0
	bl sub_8034EF0
	bl sub_80006A8
	ldr r0, _08034EEC @ =0x04000054
	strh r4, [r0]
	movs r0, #0xff
	strh r0, [r7]
	adds r0, r5, #0
	bl sub_803544C
	ldr r0, [r5, #0xc]
	bl sub_8034688
	adds r4, #1
	cmp r4, #0x10
	ble _08034E86
	ldr r0, [r5]
	cmp r0, #0
	beq _08034ED0
_08034EC4:
	ldr r4, [r0]
	bl sub_8026ED0
	adds r0, r4, #0
	cmp r0, #0
	bne _08034EC4
_08034ED0:
	movs r0, #0
	str r0, [r5]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08034EDC: .4byte gUnknown_03001304
_08034EE0: .4byte gUnknown_030007E0
_08034EE4: .4byte gUnknown_030012BC
_08034EE8: .4byte 0x04000050
_08034EEC: .4byte 0x04000054

	thumb_func_start sub_8034EF0
sub_8034EF0: @ 0x08034EF0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x24
	str r0, [sp, #0xc]
	ldr r0, _08034F34 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006A90
	ldr r0, _08034F38 @ =gUnknown_030012FC
	ldr r0, [r0]
	bl sub_8006C28
	ldr r0, [sp, #0xc]
	ldr r0, [r0]
	mov sb, r0
	cmp r0, #0
	bne _08034F1A
	b _0803506E
_08034F1A:
	mov r1, sp
	adds r1, #4
	str r1, [sp, #0x10]
_08034F20:
	mov r2, sb
	ldr r0, [r2, #0x10]
	cmp r0, #1
	beq _08034F4C
	cmp r0, #1
	bgt _08034F3C
	cmp r0, #0
	beq _08034F42
	b _08035062
	.align 2, 0
_08034F34: .4byte gUnknown_03001300
_08034F38: .4byte gUnknown_030012FC
_08034F3C:
	cmp r0, #2
	beq _08034F84
	b _08035062
_08034F42:
	ldr r0, _08034F48 @ =gUnknown_030012DC
	b _08034F4E
	.align 2, 0
_08034F48: .4byte gUnknown_030012DC
_08034F4C:
	ldr r0, _08034F80 @ =gUnknown_030012E0
_08034F4E:
	ldr r3, [r0]
	mov r4, sb
	ldr r1, [r4, #4]
	ldr r2, [r4, #8]
	movs r4, #0x88
	lsls r4, r4, #1
	adds r0, r3, r4
	str r1, [r0]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r3, r1
	str r2, [r0]
	movs r2, #0x98
	lsls r2, r2, #1
	adds r0, r3, r2
	ldr r2, [r0]
	movs r4, #0x30
	ldrsh r0, [r2, r4]
	adds r0, r3, r0
	mov r3, sb
	ldrb r1, [r3, #0x14]
	ldr r2, [r2, #0x34]
	bl sub_803AD80
	b _08035062
	.align 2, 0
_08034F80: .4byte gUnknown_030012E0
_08034F84:
	mov r4, sb
	ldrb r4, [r4, #0x14]
	lsls r0, r4, #1
	mov r1, sb
	ldrb r1, [r1, #0x14]
	adds r0, r0, r1
	lsls r0, r0, #3
	adds r0, #0x1c
	ldr r2, [sp, #0xc]
	adds r6, r2, r0
	ldr r4, _08035088 @ =gUnknown_030012FC
	ldr r0, [r4]
	bl sub_8006C44
	mov sl, r0
	ldr r0, [r4]
	ldr r1, [r6, #0x14]
	ldr r3, [r6, #4]
	ldr r2, [r6]
	muls r2, r3, r2
	lsls r2, r2, #9
	bl sub_8006C84
	movs r0, #0
	str r0, [sp]
	mov r0, sp
	ldr r1, [sp, #0x10]
	ldr r2, _0803508C @ =0x05000002
	bl sub_803A94C
	ldr r3, [sp, #0x10]
	ldrb r1, [r3, #3]
	movs r0, #0x3f
	ands r0, r1
	movs r1, #0x80
	orrs r0, r1
	strb r0, [r3, #3]
	ldrb r4, [r6, #0x10]
	lsls r1, r4, #4
	movs r0, #0xf
	ldrb r2, [r3, #5]
	ands r0, r2
	orrs r0, r1
	strb r0, [r3, #5]
	mov r3, sb
	ldr r3, [r3, #8]
	mov r8, r3
	movs r1, #0
	ldr r0, [r6, #4]
	mov r4, sp
	adds r4, #4
	str r4, [sp, #0x20]
	cmp r1, r0
	bge _08035062
_08034FF0:
	mov r2, r8
	ldr r0, [sp, #0x20]
	strb r2, [r0]
	mov r3, sb
	ldr r5, [r3, #4]
	movs r7, #0
	ldr r0, [r6]
	mov r4, r8
	adds r4, #0x20
	str r4, [sp, #0x18]
	adds r1, #1
	str r1, [sp, #0x14]
	cmp r7, r0
	bge _08035056
	ldr r4, [sp, #0x20]
_0803500E:
	mov r0, r8
	adds r0, #0x1f
	cmp r0, #0xbe
	bhi _08035048
	ldr r1, _08035090 @ =0x000001FF
	adds r0, r1, #0
	adds r2, r5, #0
	ands r2, r0
	ldrh r0, [r4, #2]
	ldr r3, _08035094 @ =0xFFFFFE00
	adds r1, r3, #0
	ands r0, r1
	orrs r0, r2
	strh r0, [r4, #2]
	ldr r1, _08035098 @ =0x000003FF
	adds r0, r1, #0
	mov r1, sl
	ands r1, r0
	ldr r2, _0803509C @ =0xFFFFFC00
	adds r0, r2, #0
	ldrh r3, [r4, #4]
	ands r0, r3
	orrs r0, r1
	strh r0, [r4, #4]
	ldr r0, _080350A0 @ =gUnknown_03001300
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8006AC8
_08035048:
	movs r0, #0x10
	add sl, r0
	adds r5, #0x20
	adds r7, #1
	ldr r0, [r6]
	cmp r7, r0
	blt _0803500E
_08035056:
	ldr r1, [sp, #0x18]
	mov r8, r1
	ldr r1, [sp, #0x14]
	ldr r0, [r6, #4]
	cmp r1, r0
	blt _08034FF0
_08035062:
	mov r2, sb
	ldr r2, [r2]
	mov sb, r2
	cmp r2, #0
	beq _0803506E
	b _08034F20
_0803506E:
	ldr r0, _080350A0 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006A48
	add sp, #0x24
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08035088: .4byte gUnknown_030012FC
_0803508C: .4byte 0x05000002
_08035090: .4byte 0x000001FF
_08035094: .4byte 0xFFFFFE00
_08035098: .4byte 0x000003FF
_0803509C: .4byte 0xFFFFFC00
_080350A0: .4byte gUnknown_03001300

	thumb_func_start sub_80350A4
sub_80350A4: @ 0x080350A4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	adds r5, r0, #0
	adds r4, r5, #0
	ldr r0, [r5]
	cmp r0, #0
	beq _080350DE
_080350BA:
	ldr r2, [r4]
	ldr r0, [r2, #8]
	subs r0, #1
	str r0, [r2, #8]
	ldr r1, [r2, #0xc]
	adds r0, r0, r1
	cmp r0, #0
	bgt _080350D6
	ldr r0, [r2]
	str r0, [r4]
	adds r0, r2, #0
	bl sub_8026ED0
	b _080350D8
_080350D6:
	adds r4, r2, #0
_080350D8:
	ldr r0, [r4]
	cmp r0, #0
	bne _080350BA
_080350DE:
	ldr r0, [r5, #0x14]
	cmp r0, #0
	beq _080350EA
	subs r0, #1
	str r0, [r5, #0x14]
	b _08035298
_080350EA:
	mov r8, r5
	ldr r0, [r5]
	ldr r1, _08035154 @ =gUnknown_030012DC
	ldr r4, _08035158 @ =gStaticData_0817CF3C
	cmp r0, #0
	beq _08035102
_080350F6:
	mov r0, r8
	ldr r0, [r0]
	mov r8, r0
	ldr r0, [r0]
	cmp r0, #0
	bne _080350F6
_08035102:
	mov r7, r8
	ldr r0, [r1]
	adds r1, r4, #0
	bl sub_8028968
	str r0, [sp]
	ldr r0, _0803515C @ =gUnknown_030012E0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8028968
	str r0, [sp, #4]
	ldr r1, [sp]
	mov sb, r1
	movs r2, #0
	mov sl, r2
	ldr r0, [r5, #8]
	ldrb r0, [r0]
	cmp r0, #0
	bne _0803512E
	ldr r0, [r5, #4]
	str r0, [r5, #8]
_0803512E:
	ldr r4, [r5, #8]
	ldrb r0, [r4]
	cmp r0, #0xa
	bne _08035138
	b _08035254
_08035138:
	cmp r0, #0
	bne _0803513E
	b _0803524C
_0803513E:
	adds r3, r5, #0
	adds r3, #0x24
	str r3, [sp, #8]
_08035144:
	movs r2, #0
	movs r6, #0
	ldrb r0, [r4]
	cmp r0, #2
	bne _08035160
	str r6, [r5, #0x10]
	b _08035230
	.align 2, 0
_08035154: .4byte gUnknown_030012DC
_08035158: .4byte gStaticData_0817CF3C
_0803515C: .4byte gUnknown_030012E0
_08035160:
	cmp r0, #3
	bne _0803516A
	movs r0, #1
	str r0, [r5, #0x10]
	b _08035230
_0803516A:
	cmp r0, #1
	bne _080351B4
	adds r0, r4, #1
	str r0, [r5, #8]
	movs r0, #0x18
	bl sub_8026EDC
	str r0, [r7]
	movs r1, #2
	str r1, [r0, #0x10]
	str r6, [r0, #8]
	mov r1, sl
	str r1, [r0, #4]
	ldr r1, [r5, #8]
	ldrb r1, [r1]
	strb r1, [r0, #0x14]
	lsls r1, r1, #1
	ldrb r2, [r0, #0x14]
	adds r1, r1, r2
	lsls r1, r1, #3
	ldr r3, [sp, #8]
	adds r1, r3, r1
	ldr r1, [r1]
	str r1, [r0, #0xc]
	str r6, [r0]
	adds r7, r0, #0
	ldrb r0, [r7, #0x14]
	lsls r1, r0, #1
	adds r1, r1, r0
	lsls r1, r1, #3
	adds r0, r3, r1
	ldr r6, [r0]
	adds r0, r5, #0
	adds r0, #0x28
	adds r0, r0, r1
	ldr r2, [r0]
	b _08035230
_080351B4:
	ldr r0, [r5, #0x10]
	cmp r0, #0
	bne _080351E0
	ldr r0, _080351DC @ =gUnknown_030012DC
	ldr r0, [r0]
	movs r2, #0x98
	lsls r2, r2, #1
	adds r1, r0, r2
	ldr r2, [r1]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r3, [r2, #0x1c]
	adds r1, r4, #0
	movs r2, #1
	bl sub_803AD84
	adds r2, r0, #0
	ldr r6, [sp]
	b _08035200
	.align 2, 0
_080351DC: .4byte gUnknown_030012DC
_080351E0:
	ldr r0, _080352A8 @ =gUnknown_030012E0
	ldr r0, [r0]
	movs r2, #0x98
	lsls r2, r2, #1
	adds r1, r0, r2
	ldr r2, [r1]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r3, [r2, #0x1c]
	adds r1, r4, #0
	movs r2, #1
	bl sub_803AD84
	adds r2, r0, #0
	ldr r6, [sp, #4]
_08035200:
	ldr r0, [r5, #8]
	ldrb r0, [r0]
	cmp r0, #0x20
	beq _08035230
	movs r0, #0x18
	str r2, [sp, #0xc]
	bl sub_8026EDC
	str r0, [r7]
	ldr r1, [r5, #0x10]
	str r1, [r0, #0x10]
	movs r1, #0
	str r1, [r0, #8]
	mov r3, sl
	str r3, [r0, #4]
	ldr r1, [r5, #8]
	ldrb r1, [r1]
	strb r1, [r0, #0x14]
	ldr r0, [r7]
	str r6, [r0, #0xc]
	adds r7, r0, #0
	movs r0, #0
	str r0, [r7]
	ldr r2, [sp, #0xc]
_08035230:
	add sl, r2
	cmp sb, r6
	bge _08035238
	mov sb, r6
_08035238:
	ldr r0, [r5, #8]
	adds r1, r0, #1
	str r1, [r5, #8]
	ldrb r0, [r0, #1]
	cmp r0, #0xa
	beq _08035254
	adds r4, r1, #0
	cmp r0, #0
	beq _0803524C
	b _08035144
_0803524C:
	ldr r0, [r5, #8]
	ldrb r0, [r0]
	cmp r0, #0xa
	bne _0803525A
_08035254:
	ldr r0, [r5, #8]
	adds r0, #1
	str r0, [r5, #8]
_0803525A:
	mov r1, r8
	ldr r3, [r1]
	mov r6, sb
	adds r6, #6
	cmp r3, #0
	beq _08035296
	movs r0, #0xf0
	mov r2, sl
	subs r0, r0, r2
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r4, r0, #1
_08035272:
	ldr r0, [r3, #8]
	adds r2, r0, #0
	adds r2, #0xa0
	ldr r1, [r3, #0xc]
	adds r0, r0, r1
	mov r1, sb
	subs r0, r1, r0
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	adds r2, r2, r0
	str r2, [r3, #8]
	ldr r0, [r3, #4]
	adds r0, r0, r4
	str r0, [r3, #4]
	ldr r3, [r3]
	cmp r3, #0
	bne _08035272
_08035296:
	str r6, [r5, #0x14]
_08035298:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080352A8: .4byte gUnknown_030012E0

	thumb_func_start sub_80352AC
sub_80352AC: @ 0x080352AC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x24
	str r0, [sp, #4]
	ldr r0, _0803543C @ =gUnknown_030012B8
	ldr r0, [r0]
	adds r0, #0x2c
	str r0, [sp, #8]
	movs r0, #1
	str r0, [sp, #0xc]
	movs r5, #0
_080352C8:
	lsls r0, r5, #2
	adds r0, r0, r5
	lsls r0, r0, #2
	ldr r1, _08035440 @ =gStaticData_0817CF40
	adds r7, r0, r1
	lsls r0, r5, #1
	adds r0, r0, r5
	lsls r0, r0, #3
	adds r0, #0x1c
	ldr r1, [sp, #4]
	adds r1, r1, r0
	mov r8, r1
	ldr r1, [r7]
	ldr r2, [r7, #4]
	lsls r0, r2, #3
	mov r3, r8
	str r0, [r3, #8]
	lsls r0, r1, #3
	str r0, [r3, #0xc]
	adds r0, r1, #3
	cmp r0, #0
	bge _080352F6
	adds r0, r1, #6
_080352F6:
	asrs r0, r0, #2
	mov r1, r8
	str r0, [r1]
	adds r0, r2, #3
	cmp r0, #0
	bge _08035304
	adds r0, r2, #6
_08035304:
	asrs r0, r0, #2
	mov r2, r8
	str r0, [r2, #4]
	ldr r0, [r7, #0xc]
	ldr r0, [r0]
	lsrs r0, r0, #8
	bl sub_8026EC0
	str r0, [sp, #0x10]
	ldr r0, [r7, #0xc]
	ldr r1, [sp, #0x10]
	bl LoadTaggedAsset
	mov r3, r8
	ldr r1, [r3]
	ldr r0, [r3, #4]
	adds r4, r1, #0
	muls r4, r0, r4
	lsls r4, r4, #9
	adds r0, r4, #0
	bl sub_8026EC0
	mov r1, r8
	str r0, [r1, #0x14]
	movs r1, #0
	str r1, [sp]
	ldr r2, _08035444 @ =0x040000D4
	mov r3, sp
	str r3, [r2]
	str r0, [r2, #4]
	cmp r4, #0
	bge _08035346
	adds r4, #3
_08035346:
	asrs r0, r4, #2
	movs r1, #0x85
	lsls r1, r1, #0x18
	orrs r0, r1
	str r0, [r2, #8]
	ldr r0, [r2, #8]
	movs r6, #0
	ldr r0, [r7, #4]
	ldr r1, [sp, #0xc]
	adds r1, #1
	str r1, [sp, #0x18]
	adds r5, #1
	str r5, [sp, #0x1c]
	cmp r6, r0
	bge _080353CC
_08035364:
	movs r4, #0
	ldr r3, [r7]
	adds r2, r6, #1
	str r2, [sp, #0x20]
	cmp r4, r3
	bge _080353C4
	asrs r0, r6, #2
	str r0, [sp, #0x14]
	movs r1, #3
	mov sl, r1
	adds r0, r6, #0
	ands r0, r1
	lsls r0, r0, #2
	mov sb, r0
	ldr r5, _08035444 @ =0x040000D4
	mov r2, r8
	ldr r2, [r2, #0x14]
	mov ip, r2
_08035388:
	mov r1, r8
	ldr r0, [r1]
	ldr r2, [sp, #0x14]
	adds r1, r2, #0
	muls r1, r0, r1
	asrs r0, r4, #2
	adds r1, r1, r0
	adds r2, r4, #0
	mov r0, sl
	ands r2, r0
	add r2, sb
	adds r0, r6, #0
	muls r0, r3, r0
	adds r0, r0, r4
	lsls r0, r0, #5
	ldr r3, [sp, #0x10]
	adds r0, r3, r0
	str r0, [r5]
	lsls r1, r1, #4
	adds r1, r1, r2
	lsls r1, r1, #5
	add r1, ip
	str r1, [r5, #4]
	ldr r0, _08035448 @ =0x84000008
	str r0, [r5, #8]
	ldr r0, [r5, #8]
	adds r4, #1
	ldr r3, [r7]
	cmp r4, r3
	blt _08035388
_080353C4:
	ldr r6, [sp, #0x20]
	ldr r0, [r7, #4]
	cmp r6, r0
	blt _08035364
_080353CC:
	ldr r0, [sp, #0x10]
	cmp r0, #0
	beq _080353D6
	bl sub_8026EB4
_080353D6:
	ldr r0, [r7, #8]
	ldr r0, [r0]
	lsrs r0, r0, #9
	lsls r0, r0, #1
	bl sub_8026EC0
	adds r4, r0, #0
	ldr r0, [r7, #8]
	adds r1, r4, #0
	bl LoadTaggedAsset
	adds r2, r4, #0
	ldr r1, [sp, #0xc]
	lsls r0, r1, #5
	ldr r3, [sp, #8]
	adds r1, r0, r3
	movs r3, #0xf
_080353F8:
	ldrh r0, [r2]
	strh r0, [r1]
	adds r2, #2
	adds r1, #2
	subs r3, #1
	cmp r3, #0
	bge _080353F8
	cmp r4, #0
	beq _08035410
	adds r0, r4, #0
	bl sub_8026EB4
_08035410:
	ldr r0, _0803543C @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r1, [sp, #0xc]
	bl sub_8006D50
	ldr r0, [sp, #0xc]
	mov r1, r8
	str r0, [r1, #0x10]
	ldr r2, [sp, #0x18]
	str r2, [sp, #0xc]
	ldr r5, [sp, #0x1c]
	cmp r5, #4
	bgt _0803542C
	b _080352C8
_0803542C:
	add sp, #0x24
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0803543C: .4byte gUnknown_030012B8
_08035440: .4byte gStaticData_0817CF40
_08035444: .4byte 0x040000D4
_08035448: .4byte 0x84000008

	thumb_func_start sub_803544C
sub_803544C: @ 0x0803544C
	push {lr}
	bl sub_8001614
	ldr r1, _08035470 @ =0x04000010
	movs r0, #0
	str r0, [r1]
	ldr r0, _08035474 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _08035478 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	pop {r0}
	bx r0
	.align 2, 0
_08035470: .4byte 0x04000010
_08035474: .4byte gUnknown_030012B8
_08035478: .4byte gUnknown_03001300

	thumb_func_start sub_803547C
sub_803547C: @ 0x0803547C
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	adds r7, r1, #0
	ldr r0, [r6, #0xc]
	cmp r0, #0
	beq _0803548E
	movs r1, #3
	bl sub_80346FC
_0803548E:
	adds r4, r6, #0
	adds r4, #0x30
	movs r5, #4
_08035494:
	ldr r0, [r4]
	cmp r0, #0
	beq _0803549E
	bl sub_8026EB4
_0803549E:
	adds r4, #0x18
	subs r5, #1
	cmp r5, #0
	bge _08035494
	movs r0, #1
	ands r0, r7
	cmp r0, #0
	beq _080354B4
	adds r0, r6, #0
	bl sub_8026ED0
_080354B4:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80354BC
sub_80354BC: @ 0x080354BC
	push {r4, lr}
	movs r0, #0x98
	bl sub_8026EDC
	bl sub_8034CEC
	adds r4, r0, #0
	bl sub_8034E2C
	cmp r4, #0
	beq _080354DA
	adds r0, r4, #0
	movs r1, #3
	bl sub_803547C
_080354DA:
	pop {r4}
	pop {r0}
	bx r0
