.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8004D74
sub_8004D74: @ 0x08004D74
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #0xc0
	lsls r0, r0, #0x18
	mov sl, r0
	bl mem_free_bytes
	ldr r0, _08004EA4 @ =gUnknown_030012BC
	ldr r0, [r0]
	bl sub_80019E8
	bl sub_80006A8
	movs r0, #0xa0
	lsls r0, r0, #0x13
	movs r1, #0
	strh r1, [r0]
	movs r0, #0x80
	lsls r0, r0, #0x13
	strh r1, [r0]
	ldr r6, _08004EA8 @ =gUnknown_030012B8
	ldr r1, [r6]
	mov sb, r1
	movs r0, #0x8c
	lsls r0, r0, #2
	bl sub_8026EDC
	bl sub_8006FB4
	str r0, [r6]
	ldr r2, _08004EAC @ =gStaticData_084A5600
	ldrh r1, [r2, #0xe]
	ldr r2, [r2, #8]
	bl sub_8006EF0
	ldr r0, [r6]
	movs r1, #0xf
	bl sub_8006D50
	ldr r1, [r6]
	ldr r0, _08004EB0 @ =gStaticData_0816B2C0
	movs r2, #0x83
	lsls r2, r2, #2
	adds r1, r1, r2
	movs r2, #0x10
	bl sub_803A94C
	ldr r4, _08004EB4 @ =gUnknown_030012DC
	ldr r0, [r4]
	bl sub_8028A40
	ldr r5, _08004EB8 @ =gUnknown_030012E0
	ldr r0, [r5]
	bl sub_8028A40
	ldr r0, [r4]
	movs r3, #0
	mov r8, r3
	movs r2, #0x84
	lsls r2, r2, #1
	adds r1, r0, r2
	str r3, [r1]
	movs r3, #0x98
	lsls r3, r3, #1
	adds r1, r0, r3
	ldr r1, [r1]
	adds r1, #0x40
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r1, [r1, #4]
	bl sub_803AD7C
	ldr r0, [r4]
	movs r1, #0x96
	lsls r1, r1, #1
	adds r0, r0, r1
	ldr r2, [r0]
	ldr r0, [r5]
	movs r3, #0x84
	lsls r3, r3, #1
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
	movs r1, #0x96
	lsls r1, r1, #1
	adds r0, r0, r1
	ldr r1, [r0]
	ldr r0, [r5]
	movs r2, #0x96
	lsls r2, r2, #1
	adds r0, r0, r2
	ldr r2, [r0]
	ldr r7, _08004EBC @ =gUnknown_030012FC
	ldr r0, [r7]
	adds r1, r1, r2
	str r1, [r0, #8]
	bl sub_8006C4C
	movs r0, #0xd4
	bl sub_8026EDC
	bl sub_8004EC0
	adds r4, r0, #0
	bl sub_8005100
	adds r5, r0, #0
	cmp r4, #0
	beq _08004E74
	adds r0, r4, #0
	movs r1, #3
	bl sub_8005004
_08004E74:
	ldr r0, [r7]
	mov r3, r8
	str r3, [r0, #8]
	bl sub_8006C4C
	ldr r0, [r6]
	cmp r0, #0
	beq _08004E8A
	movs r1, #3
	bl sub_8006F94
_08004E8A:
	mov r0, sb
	str r0, [r6]
	mov r0, sl
	bl mem_free_bytes
	adds r0, r5, #0
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08004EA4: .4byte gUnknown_030012BC
_08004EA8: .4byte gUnknown_030012B8
_08004EAC: .4byte gStaticData_084A5600
_08004EB0: .4byte gStaticData_0816B2C0
_08004EB4: .4byte gUnknown_030012DC
_08004EB8: .4byte gUnknown_030012E0
_08004EBC: .4byte gUnknown_030012FC

	thumb_func_start sub_8004EC0
sub_8004EC0: @ 0x08004EC0
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r7, r0, #0
	movs r0, #3
	str r0, [sp]
	adds r0, r7, #0
	movs r1, #0
	movs r2, #0x1f
	movs r3, #0
	bl sub_801E644
	adds r4, r7, #0
	adds r4, #0xc8
	movs r6, #0
	str r6, [r4]
	movs r0, #0xc0
	ldrb r1, [r4]
	orrs r0, r1
	movs r1, #0x20
	orrs r0, r1
	movs r3, #1
	orrs r0, r3
	movs r1, #2
	orrs r0, r1
	movs r1, #4
	orrs r0, r1
	movs r1, #8
	orrs r0, r1
	movs r5, #0x10
	orrs r0, r5
	strb r0, [r4]
	adds r2, r7, #0
	adds r2, #0xcc
	movs r0, #0x20
	rsbs r0, r0, #0
	ldrb r1, [r2]
	ands r0, r1
	orrs r0, r5
	strb r0, [r2]
	ldr r1, _08004FC0 @ =0x04000050
	ldr r0, [r4]
	str r0, [r1]
	adds r1, #4
	ldrb r2, [r2]
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	adds r2, r7, #0
	adds r2, #0xd0
	strh r6, [r2]
	movs r0, #0x40
	ldrb r1, [r2]
	orrs r0, r1
	movs r1, #8
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r2]
	adds r0, r7, #0
	adds r0, #0xd1
	ldrb r2, [r0]
	orrs r3, r2
	orrs r3, r5
	strb r3, [r0]
	ldr r1, _08004FC4 @ =gStaticData_0816B284
	adds r0, r7, #0
	bl LoadGraphicsPackage
	ldr r5, _08004FC8 @ =gUnknown_030012C0
	ldr r0, [r5]
	bl sub_80236EC
	str r0, [r7, #0x10]
	adds r0, r7, #0
	bl sub_800599C
	subs r4, #8
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	str r0, [r4]
	ldr r1, _08004FCC @ =gUnknown_030012D0
	ldr r1, [r1]
	ldr r1, [r1]
	ldr r1, [r1]
	movs r3, #0x8a
	lsls r3, r3, #2
	adds r1, r1, r3
	str r1, [r0, #0x20]
	bl sub_800815C
	ldr r2, [r4]
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldr r1, [r4]
	movs r0, #0xee
	lsls r0, r0, #7
	str r0, [r1]
	movs r0, #0xbc
	lsls r0, r0, #7
	str r0, [r1, #4]
	movs r0, #0x78
	bl sub_8000E1C
	adds r1, r7, #0
	adds r1, #0xc4
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r0, #0x78
	str r0, [r1]
	ldr r0, _08004FD0 @ =gStaticData_0816B298
	str r0, [r7, #0x14]
	str r6, [r7, #0x18]
	ldr r0, [r5]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _08004FD4
	movs r0, #5
	b _08004FD6
	.align 2, 0
_08004FC0: .4byte 0x04000050
_08004FC4: .4byte gStaticData_0816B284
_08004FC8: .4byte gUnknown_030012C0
_08004FCC: .4byte gUnknown_030012D0
_08004FD0: .4byte gStaticData_0816B298
_08004FD4:
	movs r0, #4
_08004FD6:
	str r0, [r7, #0x1c]
	movs r0, #0x10
	str r0, [r7, #0x20]
	movs r4, #0
	str r4, [r7, #0x24]
	movs r0, #0xb4
	str r0, [r7, #0x28]
	adds r0, r7, #0
	bl sub_801E640
	ldr r1, _08004FFC @ =0x04000008
	strh r0, [r1]
	ldr r0, _08005000 @ =0x04000010
	str r4, [r0]
	adds r0, r7, #0
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08004FFC: .4byte 0x04000008
_08005000: .4byte 0x04000010

	thumb_func_start sub_8005004
sub_8005004: @ 0x08005004
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	adds r6, r0, #0
	mov sl, r1
	adds r0, #0xc0
	ldr r2, [r0]
	cmp r2, #0
	beq _0800502C
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0800502C:
	adds r0, r6, #0
	adds r0, #0xbc
	ldr r2, [r0]
	cmp r2, #0
	beq _08005048
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_08005048:
	adds r7, r6, #0
	adds r7, #0x9c
	movs r0, #0x8c
	adds r0, r0, r6
	mov r8, r0
	movs r1, #0x88
	adds r1, r1, r6
	mov sb, r1
	adds r4, r6, #0
	adds r4, #0xb0
	movs r5, #2
_0800505E:
	ldr r2, [r4]
	cmp r2, #0
	beq _08005076
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_08005076:
	adds r4, #4
	subs r5, #1
	cmp r5, #0
	bge _0800505E
	adds r4, r7, #0
	movs r5, #4
_08005082:
	ldr r2, [r4]
	cmp r2, #0
	beq _0800509A
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0800509A:
	adds r4, #4
	subs r5, #1
	cmp r5, #0
	bge _08005082
	mov r4, r8
	movs r5, #3
_080050A6:
	ldr r2, [r4]
	cmp r2, #0
	beq _080050BE
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_080050BE:
	adds r4, #4
	subs r5, #1
	cmp r5, #0
	bge _080050A6
	mov r0, sb
	ldr r2, [r0]
	cmp r2, #0
	beq _080050E0
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_080050E0:
	movs r0, #1
	mov r1, sl
	ands r0, r1
	cmp r0, #0
	beq _080050F0
	adds r0, r6, #0
	bl sub_8026ED0
_080050F0:
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8005100
sub_8005100: @ 0x08005100
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #0xcc
	movs r0, #0x1f
	ldrb r1, [r6]
	ands r0, r1
	cmp r0, #0
	beq _0800514E
	movs r2, #0x20
	rsbs r2, r2, #0
	adds r7, r2, #0
_0800511E:
	adds r4, r6, #0
	ldrb r2, [r6]
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1b
	subs r0, #1
	movs r1, #0x1f
	ands r0, r1
	ands r2, r7
	orrs r2, r0
	strb r2, [r6]
	adds r0, r5, #0
	bl sub_80053F4
	adds r0, r5, #0
	bl sub_8006250
	adds r0, r5, #0
	bl sub_8005304
	movs r0, #0x1f
	ldrb r4, [r4]
	ands r0, r4
	cmp r0, #0
	bne _0800511E
_0800514E:
	adds r4, r5, #0
	adds r4, #0xcc
	movs r0, #0xd0
	adds r0, r0, r5
	mov sb, r0
	b _08005180
_0800515A:
	ldr r1, _08005178 @ =gUnknown_030007E0
	movs r0, #8
	ldrh r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _08005180
	ldr r0, _0800517C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
	movs r7, #0
	b _0800529A
	.align 2, 0
_08005178: .4byte gUnknown_030007E0
_0800517C: .4byte gUnknown_030012BC
_08005180:
	adds r0, r5, #0
	bl sub_80053F4
	adds r0, r5, #0
	bl sub_8006250
	adds r0, r5, #0
	bl sub_8005304
	ldr r0, _080051F8 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r6, _080051FC @ =gUnknown_030007E0
	movs r0, #0x40
	ldrh r1, [r6, #2]
	ands r0, r1
	cmp r0, #0
	beq _080051BE
	adds r0, r5, #0
	bl sub_800609C
	movs r0, #0x1e
	str r0, [r5, #0x68]
	ldr r0, _08005200 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x46
	bl PlaySfx
_080051BE:
	movs r0, #0x80
	ldrh r2, [r6, #2]
	ands r0, r2
	cmp r0, #0
	beq _080051E0
	adds r0, r5, #0
	bl sub_8006084
	movs r0, #0x1e
	str r0, [r5, #0x68]
	ldr r0, _08005200 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x46
	bl PlaySfx
_080051E0:
	ldr r2, [r6]
	lsrs r1, r2, #0x10
	movs r3, #0x20
	movs r0, #0x20
	ands r0, r1
	cmp r0, #0
	beq _08005204
	adds r0, r5, #0
	bl sub_8005EF4
	movs r0, #0x1e
	b _0800521C
	.align 2, 0
_080051F8: .4byte gUnknown_03001304
_080051FC: .4byte gUnknown_030007E0
_08005200: .4byte gUnknown_030012BC
_08005204:
	ands r2, r3
	cmp r2, #0
	beq _0800521E
	ldr r0, [r5, #0x68]
	cmp r0, #0
	bne _0800521A
	adds r0, r5, #0
	bl sub_8005EF4
	movs r0, #5
	b _0800521C
_0800521A:
	subs r0, #1
_0800521C:
	str r0, [r5, #0x68]
_0800521E:
	ldr r0, _08005238 @ =gUnknown_030007E0
	ldr r2, [r0]
	lsrs r1, r2, #0x10
	movs r3, #0x10
	movs r0, #0x10
	ands r0, r1
	cmp r0, #0
	beq _0800523C
	adds r0, r5, #0
	bl sub_8005FBC
	movs r0, #0x1e
	b _08005254
	.align 2, 0
_08005238: .4byte gUnknown_030007E0
_0800523C:
	ands r2, r3
	cmp r2, #0
	beq _08005256
	ldr r0, [r5, #0x68]
	cmp r0, #0
	bne _08005252
	adds r0, r5, #0
	bl sub_8005FBC
	movs r0, #5
	b _08005254
_08005252:
	subs r0, #1
_08005254:
	str r0, [r5, #0x68]
_08005256:
	ldr r1, _08005284 @ =gUnknown_030007E0
	movs r0, #1
	ldrh r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	bne _08005264
	b _0800515A
_08005264:
	ldr r0, [r5, #0x18]
	ldr r1, [r5, #0x14]
	lsls r0, r0, #3
	adds r0, r0, r1
	ldr r7, [r0, #4]
	subs r0, r7, #4
	cmp r0, #1
	bhi _0800528C
	ldr r0, _08005288 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x48
	bl PlaySfx
	b _0800515A
	.align 2, 0
_08005284: .4byte gUnknown_030007E0
_08005288: .4byte gUnknown_030012BC
_0800528C:
	ldr r0, _08005300 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
_0800529A:
	adds r6, r4, #0
	movs r0, #0x1f
	ldrb r1, [r6]
	ands r0, r1
	cmp r0, #0x10
	beq _080052DE
	movs r2, #0x20
	rsbs r2, r2, #0
	mov r8, r2
_080052AC:
	adds r4, r6, #0
	ldrb r2, [r6]
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1b
	adds r0, #1
	movs r1, #0x1f
	ands r0, r1
	mov r1, r8
	ands r2, r1
	orrs r2, r0
	strb r2, [r6]
	adds r0, r5, #0
	bl sub_80053F4
	adds r0, r5, #0
	bl sub_8006250
	adds r0, r5, #0
	bl sub_8005304
	movs r0, #0x1f
	ldrb r4, [r4]
	ands r0, r4
	cmp r0, #0x10
	bne _080052AC
_080052DE:
	movs r0, #0
	mov r2, sb
	strh r0, [r2]
	movs r0, #0x40
	ldrb r1, [r2]
	orrs r0, r1
	strb r0, [r2]
	adds r0, r5, #0
	bl sub_8006250
	adds r0, r7, #0
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08005300: .4byte gUnknown_030012BC

	thumb_func_start sub_8005304
sub_8005304: @ 0x08005304
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	ldr r0, [r5, #0x24]
	cmp r0, #4
	bhi _08005382
	lsls r0, r0, #2
	ldr r1, _08005318 @ =_0800531C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08005318: .4byte _0800531C
_0800531C: @ jump table
	.4byte _08005330 @ case 0
	.4byte _0800533C @ case 1
	.4byte _08005350 @ case 2
	.4byte _08005364 @ case 3
	.4byte _08005378 @ case 4
_08005330:
	adds r0, r5, #0
	adds r0, #0x88
	ldr r0, [r0]
	bl sub_8008044
	b _08005382
_0800533C:
	adds r6, r5, #0
	adds r6, #0x8c
	movs r4, #3
_08005342:
	ldm r6!, {r0}
	bl sub_8008044
	subs r4, #1
	cmp r4, #0
	bge _08005342
	b _08005382
_08005350:
	adds r6, r5, #0
	adds r6, #0x9c
	movs r4, #4
_08005356:
	ldm r6!, {r0}
	bl sub_8008044
	subs r4, #1
	cmp r4, #0
	bge _08005356
	b _08005382
_08005364:
	adds r6, r5, #0
	adds r6, #0xb0
	movs r4, #2
_0800536A:
	ldm r6!, {r0}
	bl sub_8008044
	subs r4, #1
	cmp r4, #0
	bge _0800536A
	b _08005382
_08005378:
	adds r0, r5, #0
	adds r0, #0xbc
	ldr r0, [r0]
	bl sub_8008044
_08005382:
	ldr r0, [r5, #0x28]
	subs r0, #1
	str r0, [r5, #0x28]
	cmp r0, #0
	bne _0800539E
	ldr r0, [r5, #0x24]
	adds r0, #1
	str r0, [r5, #0x24]
	movs r1, #5
	bl sub_803AE4C
	str r0, [r5, #0x24]
	movs r0, #0xb4
	str r0, [r5, #0x28]
_0800539E:
	adds r6, r5, #0
	adds r6, #0xc4
	ldr r1, [r6]
	cmp r1, #0
	bne _080053E8
	adds r0, r5, #0
	adds r0, #0xc0
	ldr r4, [r0]
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _080053E0
	adds r0, r4, #0
	adds r0, #0x2d
	strb r1, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	movs r0, #0x78
	bl sub_8000E1C
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r0, #0x78
	b _080053EA
_080053E0:
	adds r0, r4, #0
	bl sub_8008044
	b _080053EC
_080053E8:
	subs r0, r1, #1
_080053EA:
	str r0, [r6]
_080053EC:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80053F4
sub_80053F4: @ 0x080053F4
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	ldr r0, _080054F8 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006A90
	ldr r0, _080054FC @ =gUnknown_030012FC
	ldr r0, [r0]
	bl sub_8006C28
	ldr r6, _08005500 @ =gUnknown_030012E0
	ldr r0, [r6]
	movs r7, #0x98
	lsls r7, r7, #1
	adds r1, r0, r7
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r5, #0x70]
	ldr r2, [r2, #0x14]
	bl sub_803AD80
	movs r1, #0xf0
	subs r1, r1, r0
	lsrs r3, r1, #1
	ldr r0, [r6]
	movs r2, #0xe
	movs r4, #0x88
	lsls r4, r4, #1
	adds r1, r0, r4
	str r3, [r1]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r1, r0, r3
	str r2, [r1]
	adds r1, r0, r7
	ldr r2, [r1]
	movs r4, #0x20
	ldrsh r1, [r2, r4]
	adds r0, r0, r1
	ldr r1, [r5, #0x70]
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r4, [r5, #0x74]
	cmp r4, #0
	beq _08005498
	ldr r3, [r6]
	movs r1, #0x20
	movs r0, #0x26
	mov ip, r0
	movs r2, #0x88
	lsls r2, r2, #1
	adds r0, r3, r2
	str r1, [r0]
	adds r1, #0xf4
	adds r0, r3, r1
	mov r2, ip
	str r2, [r0]
	adds r1, r7, #0
	adds r0, r3, r1
	ldr r1, [r0]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r3, r0
	ldr r2, [r1, #0x24]
	adds r1, r4, #0
	bl sub_803AD80
	ldr r0, [r6]
	adds r3, r7, #0
	adds r1, r0, r3
	ldr r2, [r1]
	movs r4, #0x20
	ldrsh r1, [r2, r4]
	adds r0, r0, r1
	adds r1, r5, #0
	adds r1, #0x78
	ldr r2, [r2, #0x24]
	bl sub_803AD80
_08005498:
	ldr r0, [r6]
	adds r1, r0, r7
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	adds r4, r5, #0
	adds r4, #0x41
	ldr r2, [r2, #0x14]
	adds r1, r4, #0
	bl sub_803AD80
	movs r1, #0x8c
	subs r3, r1, r0
	ldr r0, [r6]
	movs r2, #0x88
	movs r6, #0x88
	lsls r6, r6, #1
	adds r1, r0, r6
	str r3, [r1]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r1, r0, r3
	str r2, [r1]
	adds r1, r0, r7
	ldr r2, [r1]
	movs r6, #0x20
	ldrsh r1, [r2, r6]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	adds r1, r4, #0
	bl sub_803AD80
	adds r0, r5, #0
	bl sub_800556C
	adds r0, r5, #0
	bl sub_80061E8
	ldr r0, [r5, #0x24]
	cmp r0, #4
	bhi _08005542
	lsls r0, r0, #2
	ldr r1, _08005504 @ =_08005508
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080054F8: .4byte gUnknown_03001300
_080054FC: .4byte gUnknown_030012FC
_08005500: .4byte gUnknown_030012E0
_08005504: .4byte _08005508
_08005508: @ jump table
	.4byte _0800551C @ case 0
	.4byte _08005524 @ case 1
	.4byte _0800552C @ case 2
	.4byte _08005534 @ case 3
	.4byte _0800553C @ case 4
_0800551C:
	adds r0, r5, #0
	bl sub_800619C
	b _08005542
_08005524:
	adds r0, r5, #0
	bl sub_800570C
	b _08005542
_0800552C:
	adds r0, r5, #0
	bl sub_80057E0
	b _08005542
_08005534:
	adds r0, r5, #0
	bl sub_80058C0
	b _08005542
_0800553C:
	adds r0, r5, #0
	bl sub_8006124
_08005542:
	adds r0, r5, #0
	adds r0, #0xc4
	ldr r0, [r0]
	cmp r0, #0
	bne _0800555A
	adds r0, r5, #0
	adds r0, #0xc0
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
_0800555A:
	ldr r0, _08005568 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006A48
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08005568: .4byte gUnknown_03001300

	thumb_func_start sub_800556C
sub_800556C: @ 0x0800556C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xc
	mov sb, r0
	movs r0, #0x4a
	str r0, [sp]
	movs r1, #0
	mov sl, r1
	mov r2, sb
	ldr r0, [r2, #0x1c]
	cmp sl, r0
	blt _0800558C
	b _080056F0
_0800558C:
	adds r2, #0x57
	str r2, [sp, #4]
	mov r3, sb
	adds r3, #0x4f
	str r3, [sp, #8]
_08005596:
	mov r1, sb
	ldr r0, [r1, #0x18]
	cmp sl, r0
	bne _080055B0
	ldr r0, _080055AC @ =gUnknown_030012DC
	ldr r0, [r0]
	movs r1, #0xf
	bl sub_8028A30
	b _080055B8
	.align 2, 0
_080055AC: .4byte gUnknown_030012DC
_080055B0:
	ldr r0, _08005624 @ =gUnknown_030012DC
	ldr r0, [r0]
	bl sub_8028A40
_080055B8:
	mov r2, sb
	ldr r0, [r2, #0x14]
	mov r3, sl
	lsls r4, r3, #3
	adds r0, r4, r0
	ldr r0, [r0]
	bl sub_8026F38
	adds r7, r0, #0
	ldr r6, _08005624 @ =gUnknown_030012DC
	ldr r0, [r6]
	movs r1, #0x98
	lsls r1, r1, #1
	mov r8, r1
	adds r1, r0, r1
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	adds r1, r7, #0
	bl sub_803AD80
	lsrs r0, r0, #1
	movs r1, #0x32
	subs r5, r1, r0
	mov r1, sb
	ldr r0, [r1, #0x14]
	adds r4, r4, r0
	ldr r0, [r4, #4]
	cmp r0, #4
	beq _08005628
	cmp r0, #5
	beq _08005682
	ldr r0, [r6]
	movs r2, #0x88
	lsls r2, r2, #1
	adds r1, r0, r2
	str r5, [r1]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r1, r0, r3
	ldr r2, [sp]
	str r2, [r1]
	mov r3, r8
	adds r1, r0, r3
	ldr r2, [r1]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	adds r1, r7, #0
	b _080056D6
	.align 2, 0
_08005624: .4byte gUnknown_030012DC
_08005628:
	ldr r0, [r6]
	movs r2, #0x98
	lsls r2, r2, #1
	adds r1, r0, r2
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	ldr r1, [sp, #4]
	bl sub_803AD80
	lsrs r0, r0, #1
	subs r5, r5, r0
	ldr r2, [r6]
	movs r1, #0x88
	lsls r1, r1, #1
	adds r0, r2, r1
	str r5, [r0]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r0, r2, r3
	ldr r1, [sp]
	str r1, [r0]
	adds r3, #0x1c
	adds r0, r2, r3
	ldr r1, [r0]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0x24]
	adds r1, r7, #0
	bl sub_803AD80
	ldr r0, [r6]
	movs r2, #0x98
	lsls r2, r2, #1
	adds r1, r0, r2
	ldr r2, [r1]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	ldr r1, [sp, #4]
	b _080056D6
_08005682:
	ldr r0, [r6]
	mov r2, r8
	adds r1, r0, r2
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	ldr r1, [sp, #8]
	bl sub_803AD80
	lsrs r0, r0, #1
	subs r5, r5, r0
	ldr r0, [r6]
	movs r2, #0x88
	lsls r2, r2, #1
	adds r1, r0, r2
	str r5, [r1]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r1, r0, r3
	ldr r2, [sp]
	str r2, [r1]
	mov r3, r8
	adds r1, r0, r3
	ldr r2, [r1]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	adds r1, r7, #0
	bl sub_803AD80
	ldr r0, [r6]
	mov r2, r8
	adds r1, r0, r2
	ldr r2, [r1]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	ldr r1, [sp, #8]
_080056D6:
	bl sub_803AD80
	mov r1, sb
	ldr r0, [r1, #0x20]
	ldr r2, [sp]
	adds r2, r2, r0
	str r2, [sp]
	movs r3, #1
	add sl, r3
	ldr r0, [r1, #0x1c]
	cmp sl, r0
	bge _080056F0
	b _08005596
_080056F0:
	ldr r0, _08005708 @ =gUnknown_030012DC
	ldr r0, [r0]
	bl sub_8028A40
	add sp, #0xc
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08005708: .4byte gUnknown_030012DC

	thumb_func_start sub_800570C
sub_800570C: @ 0x0800570C
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	movs r2, #1
	ldr r1, [r4, #0x10]
	movs r0, #0x20
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _0800572E
	adds r0, r4, #0
	adds r0, #0x8c
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	movs r2, #0
_0800572E:
	ldr r1, [r4, #0x10]
	movs r0, #0x80
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _0800574A
	adds r0, r4, #0
	adds r0, #0x90
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	movs r2, #0
_0800574A:
	ldr r1, [r4, #0x10]
	movs r0, #0x40
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _08005766
	adds r0, r4, #0
	adds r0, #0x94
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	movs r2, #0
_08005766:
	ldr r1, [r4, #0x10]
	movs r0, #0x10
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _08005782
	adds r0, r4, #0
	adds r0, #0x98
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	movs r2, #0
_08005782:
	cmp r2, #0
	beq _080057D4
	movs r0, #0x3a
	bl sub_8026F38
	adds r6, r0, #0
	ldr r5, _080057DC @ =gUnknown_030012DC
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
	lsrs r0, r0, #1
	movs r2, #0xc2
	subs r2, r2, r0
	ldr r0, [r5]
	movs r3, #0x64
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
	ldr r2, [r2, #0x24]
	adds r1, r6, #0
	bl sub_803AD80
_080057D4:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_080057DC: .4byte gUnknown_030012DC

	thumb_func_start sub_80057E0
sub_80057E0: @ 0x080057E0
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	ldr r1, [r5, #0x10]
	movs r0, #1
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _080057FE
	adds r0, r5, #0
	adds r0, #0xa0
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
_080057FE:
	ldr r1, [r5, #0x10]
	movs r0, #4
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _08005818
	adds r0, r5, #0
	adds r0, #0xa4
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
_08005818:
	ldr r1, [r5, #0x10]
	movs r0, #8
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _08005832
	adds r0, r5, #0
	adds r0, #0xa8
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
_08005832:
	ldr r1, [r5, #0x10]
	movs r0, #2
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _0800584C
	adds r0, r5, #0
	adds r0, #0xac
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
_0800584C:
	adds r0, r5, #0
	adds r0, #0x9c
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	ldr r0, _080058B8 @ =gStaticData_0816B21C
	ldr r4, _080058BC @ =gUnknown_030012DC
	ldr r3, [r4]
	ldr r1, [r0]
	subs r1, #0x14
	ldr r2, [r0, #4]
	subs r2, #4
	movs r6, #0x88
	lsls r6, r6, #1
	adds r0, r3, r6
	str r1, [r0]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r3, r1
	str r2, [r0]
	adds r6, #0x20
	adds r0, r3, r6
	ldr r2, [r0]
	movs r1, #0x20
	ldrsh r0, [r2, r1]
	adds r0, r3, r0
	adds r1, r5, #0
	adds r1, #0x2f
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r3, [r4]
	movs r1, #0xb4
	movs r2, #0x80
	movs r4, #0x88
	lsls r4, r4, #1
	adds r0, r3, r4
	str r1, [r0]
	subs r6, #0x1c
	adds r0, r3, r6
	str r2, [r0]
	adds r1, r5, #0
	adds r1, #0x32
	adds r2, r5, #0
	adds r2, #0x49
	adds r0, r5, #0
	bl sub_8005E5C
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_080058B8: .4byte gStaticData_0816B21C
_080058BC: .4byte gUnknown_030012DC

	thumb_func_start sub_80058C0
sub_80058C0: @ 0x080058C0
	push {r4, r5, r6, r7, lr}
	adds r7, r0, #0
	adds r5, r7, #0
	adds r5, #0xb0
	movs r4, #2
_080058CA:
	ldm r5!, {r0}
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	subs r4, #1
	cmp r4, #0
	bge _080058CA
	ldr r4, _08005994 @ =gStaticData_0816B258
	ldr r6, _08005998 @ =gUnknown_030012DC
	ldr r0, [r6]
	ldr r2, [r4, #0x10]
	subs r2, #4
	ldr r3, [r4, #0x14]
	adds r3, #0xe
	movs r5, #0x88
	lsls r5, r5, #1
	adds r1, r0, r5
	str r2, [r1]
	movs r2, #0x8a
	lsls r2, r2, #1
	adds r1, r0, r2
	str r3, [r1]
	adds r5, #0x20
	adds r1, r0, r5
	ldr r2, [r1]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	adds r1, r7, #0
	adds r1, #0x38
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r0, [r6]
	ldr r2, [r4, #8]
	subs r2, #4
	ldr r3, [r4, #0xc]
	adds r3, #0xe
	mov ip, r3
	movs r3, #0x88
	lsls r3, r3, #1
	adds r1, r0, r3
	str r2, [r1]
	movs r2, #0x8a
	lsls r2, r2, #1
	adds r1, r0, r2
	mov r3, ip
	str r3, [r1]
	adds r1, r0, r5
	ldr r2, [r1]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	adds r1, r7, #0
	adds r1, #0x3b
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r0, [r6]
	ldr r2, [r4]
	subs r2, #4
	ldr r3, [r4, #4]
	adds r3, #0xe
	movs r4, #0x88
	lsls r4, r4, #1
	adds r1, r0, r4
	str r2, [r1]
	movs r2, #0x8a
	lsls r2, r2, #1
	adds r1, r0, r2
	str r3, [r1]
	adds r5, r0, r5
	ldr r2, [r5]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	adds r1, r7, #0
	adds r1, #0x3e
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r3, [r6]
	movs r1, #0xb4
	movs r2, #0x80
	adds r0, r3, r4
	str r1, [r0]
	adds r4, #4
	adds r0, r3, r4
	str r2, [r0]
	adds r1, r7, #0
	adds r1, #0x35
	adds r2, r7, #0
	adds r2, #0x4c
	adds r0, r7, #0
	bl sub_8005E5C
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08005994: .4byte gStaticData_0816B258
_08005998: .4byte gUnknown_030012DC

	thumb_func_start sub_800599C
sub_800599C: @ 0x0800599C
	push {r4, r5, lr}
	adds r5, r0, #0
	ldr r0, _080059DC @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_802332C
	adds r4, r0, #0
	lsls r0, r4, #3
	adds r0, r0, r4
	lsls r0, r0, #2
	ldr r1, _080059E0 @ =gStaticData_0816C86C
	adds r0, r0, r1
	ldr r0, [r0]
	bl sub_8026F38
	str r0, [r5, #0x70]
	cmp r4, #0x13
	bgt _080059E4
	movs r0, #0
	bl sub_8026F38
	str r0, [r5, #0x74]
	adds r1, r5, #0
	adds r1, #0x78
	movs r0, #0x20
	strb r0, [r1]
	adds r0, r4, #1
	adds r1, #1
	bl sub_80060AC
	b _080059E8
	.align 2, 0
_080059DC: .4byte gUnknown_030012C0
_080059E0: .4byte gStaticData_0816C86C
_080059E4:
	movs r0, #0
	str r0, [r5, #0x74]
_080059E8:
	ldr r0, [r5, #0x10]
	bl sub_800697C
	adds r4, r5, #0
	adds r4, #0x41
	adds r1, r4, #0
	bl sub_80060AC
	adds r2, r4, r0
	movs r3, #0
	movs r1, #0x25
	strb r1, [r2]
	adds r0, #1
	adds r4, r4, r0
	strb r3, [r4]
	ldr r4, _08005A74 @ =gUnknown_030012BC
	ldr r0, [r4]
	bl sub_8001AC0
	adds r0, #0xc
	lsls r1, r0, #2
	adds r1, r1, r0
	lsls r0, r1, #2
	cmp r0, #0
	bge _08005A1C
	adds r0, #0xff
_08005A1C:
	asrs r0, r0, #8
	str r0, [r5, #0x60]
	ldr r0, [r4]
	bl sub_8001ABC
	adds r0, #0xc
	lsls r1, r0, #2
	adds r1, r1, r0
	lsls r0, r1, #2
	cmp r0, #0
	bge _08005A34
	adds r0, #0xff
_08005A34:
	asrs r0, r0, #8
	str r0, [r5, #0x64]
	ldr r1, [r5, #0x60]
	adds r2, r5, #0
	adds r2, #0x57
	adds r0, r5, #0
	bl sub_80060F8
	ldr r1, [r5, #0x64]
	adds r2, r5, #0
	adds r2, #0x4f
	adds r0, r5, #0
	bl sub_80060F8
	adds r0, r5, #0
	bl sub_8005A78
	adds r0, r5, #0
	bl sub_8005AE8
	adds r0, r5, #0
	bl sub_8005B80
	adds r0, r5, #0
	bl sub_8005C58
	adds r0, r5, #0
	bl sub_8005D44
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08005A74: .4byte gUnknown_030012BC
