.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_800A0FC
sub_800A0FC: @ 0x0800A0FC
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r6, r4, #0
	adds r6, #0x68
	ldrb r5, [r6]
	ldrb r1, [r4, #0xc]
	lsrs r0, r1, #7
	cmp r0, #0
	beq _0800A16C
	adds r0, r4, #0
	bl sub_800A178
	orrs r5, r0
	strb r5, [r6]
	adds r0, r4, #0
	bl sub_800A050
	movs r0, #8
	ldrb r2, [r6]
	ands r0, r2
	cmp r0, #0
	beq _0800A16C
	movs r0, #0x21
	rsbs r0, r0, #0
	ldrb r1, [r4, #0xc]
	ands r0, r1
	strb r0, [r4, #0xc]
	ldrb r2, [r4, #0xd]
	lsrs r0, r2, #1
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	bne _0800A16C
	ldr r1, [r4, #0x18]
	movs r2, #0x10
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #0x14]
	bl sub_803AD7C
	adds r2, r0, #0
	adds r0, r4, #0
	movs r1, #8
	bl sub_8009BE0
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0800A16C
	movs r0, #0x20
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	movs r0, #7
	ldrb r2, [r6]
	ands r0, r2
	strb r0, [r6]
_0800A16C:
	adds r0, r4, #0
	adds r0, #0x68
	ldrb r0, [r0]
	pop {r4, r5, r6}
	pop {r1}
	bx r1

	thumb_func_start sub_800A178
sub_800A178: @ 0x0800A178
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x1c
	adds r6, r0, #0
	movs r0, #0
	mov sb, r0
	add r0, sp, #4
	mov r1, sb
	strb r1, [r0]
	movs r2, #0
	mov sl, r2
	ldr r1, [r6, #0x18]
	movs r2, #0x38
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r1, [r1, #0x3c]
	bl sub_803AD7C
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0800A1AA
	b _0800A408
_0800A1AA:
	ldrb r1, [r6, #0xc]
	lsrs r0, r1, #7
	cmp r0, #0
	bne _0800A1B4
	b _0800A408
_0800A1B4:
	adds r1, r6, #0
	adds r1, #0x24
	movs r0, #0xc
	ldrb r2, [r1]
	ands r0, r2
	str r1, [sp, #0x18]
	cmp r0, #0
	bne _0800A1DA
	movs r0, #1
	ldrb r1, [r6, #0xd]
	ands r0, r1
	rsbs r1, r0, #0
	orrs r1, r0
	asrs r1, r1, #0x1f
	mov sb, r1
	movs r0, #8
	mov r2, sb
	ands r2, r0
	mov sb, r2
_0800A1DA:
	movs r0, #0
	str r0, [r6, #0x74]
	ldr r1, [r6, #0x18]
	movs r2, #0x10
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r1, [r1, #0x14]
	bl sub_803AD7C
	mov r8, r0
	movs r0, #1
	ldrb r1, [r6, #0xd]
	ands r0, r1
	cmp r0, #0
	beq _0800A208
	adds r0, r6, #0
	mov r1, r8
	add r2, sp, #4
	bl sub_800A420
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	mov sl, r0
_0800A208:
	mov r2, sl
	cmp r2, #0
	beq _0800A218
	mov r0, sb
	cmp r0, #0
	bne _0800A218
	movs r1, #8
	mov sb, r1
_0800A218:
	add r0, sp, #4
	ldrb r0, [r0]
	cmp r0, #1
	bne _0800A2DC
	ldr r0, [r6, #4]
	str r0, [sp, #0x10]
	ldr r0, [r6]
	ldr r1, [r6, #4]
	str r0, [sp, #8]
	str r1, [sp, #0xc]
	add r4, sp, #8
	adds r0, r4, #0
	movs r1, #8
	mov r2, r8
	bl sub_8008200
	ldr r0, [sp, #8]
	asrs r0, r0, #8
	str r0, [sp, #8]
	ldr r0, [r4, #4]
	asrs r0, r0, #8
	str r0, [r4, #4]
	adds r0, r6, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _0800A25C
	mov r2, r8
	ldrb r2, [r2, #4]
	lsrs r1, r2, #1
	ldr r0, [sp, #8]
	subs r0, r0, r1
	b _0800A266
_0800A25C:
	mov r0, r8
	ldrb r0, [r0, #4]
	lsrs r1, r0, #1
	ldr r0, [sp, #8]
	adds r0, r0, r1
_0800A266:
	str r0, [sp, #8]
	ldr r0, _0800A2B0 @ =gUnknown_03001308
	ldr r0, [r0]
	add r2, sp, #0x10
	adds r1, r4, #0
	bl sub_8026C3C
	lsls r0, r0, #0x18
	mov r2, sp
	adds r2, #5
	movs r1, #0
	strb r1, [r2]
	cmp r0, #0
	beq _0800A2C4
	ldr r0, [sp, #0x10]
	ldr r4, _0800A2B4 @ =0xFFFFFF00
	ands r0, r4
	str r0, [r6, #4]
	adds r0, r6, #0
	mov r1, r8
	bl sub_800A420
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	mov sl, r0
	movs r0, #3
	ldr r1, [sp, #0x18]
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _0800A342
	cmp r0, #2
	bne _0800A2B8
	ldr r0, [r6]
	adds r0, r0, r4
	str r0, [r6]
	b _0800A2DC
	.align 2, 0
_0800A2B0: .4byte gUnknown_03001308
_0800A2B4: .4byte 0xFFFFFF00
_0800A2B8:
	ldr r0, [r6]
	movs r2, #0x80
	lsls r2, r2, #1
	adds r0, r0, r2
	str r0, [r6]
	b _0800A2DC
_0800A2C4:
	ldr r0, [r6, #4]
	movs r1, #0x80
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r6, #4]
	adds r0, r6, #0
	mov r1, r8
	bl sub_800A420
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	mov sl, r0
_0800A2DC:
	movs r7, #3
	ldr r2, [sp, #0x18]
	ldrb r2, [r2]
	ands r7, r2
	cmp r7, #0
	beq _0800A342
	mov r0, sl
	cmp r0, #0
	bne _0800A342
	ldr r0, [r6]
	ldr r1, [r6, #4]
	str r0, [sp, #8]
	str r1, [sp, #0xc]
	mov r1, r8
	ldrb r5, [r1, #5]
	subs r5, #0x10
	ldr r0, [r6]
	str r0, [sp, #0x14]
	add r4, sp, #8
	adds r0, r4, #0
	adds r1, r7, #0
	mov r2, r8
	bl sub_8008278
	ldr r0, [sp, #8]
	asrs r0, r0, #8
	str r0, [sp, #8]
	ldr r0, [r4, #4]
	asrs r0, r0, #8
	adds r0, #8
	str r0, [r4, #4]
	ldr r0, _0800A41C @ =gUnknown_03001308
	ldr r0, [r0]
	add r1, sp, #0x14
	str r1, [sp]
	adds r1, r7, #0
	adds r2, r4, #0
	adds r3, r5, #0
	bl sub_8026628
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0800A342
	ldr r0, [r6, #0x74]
	orrs r0, r7
	str r0, [r6, #0x74]
	mov r2, sb
	orrs r2, r7
	mov sb, r2
	ldr r0, [sp, #0x14]
	str r0, [r6]
_0800A342:
	movs r7, #0xc
	ldr r0, [sp, #0x18]
	ldrb r0, [r0]
	ands r7, r0
	cmp r7, #0
	beq _0800A3A6
	mov r1, sl
	cmp r1, #0
	bne _0800A3A6
	ldr r0, [r6]
	ldr r1, [r6, #4]
	str r0, [sp, #8]
	str r1, [sp, #0xc]
	mov r2, r8
	ldrb r5, [r2, #4]
	ldr r0, [r6]
	str r0, [sp, #0x14]
	ldr r0, [r6, #4]
	str r0, [sp, #0x10]
	add r4, sp, #8
	adds r0, r4, #0
	adds r1, r7, #0
	bl sub_8008278
	ldr r0, [sp, #8]
	asrs r0, r0, #8
	str r0, [sp, #8]
	ldr r0, [r4, #4]
	asrs r0, r0, #8
	str r0, [r4, #4]
	ldr r0, _0800A41C @ =gUnknown_03001308
	ldr r0, [r0]
	add r1, sp, #0x10
	str r1, [sp]
	adds r1, r7, #0
	adds r2, r4, #0
	adds r3, r5, #0
	bl sub_8026628
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0800A3A6
	mov r0, sb
	orrs r0, r7
	mov sb, r0
	ldr r0, [r6, #0x74]
	orrs r0, r7
	str r0, [r6, #0x74]
	ldr r0, [sp, #0x10]
	str r0, [r6, #4]
_0800A3A6:
	movs r7, #3
	ldr r1, [sp, #0x18]
	ldrb r1, [r1]
	ands r7, r1
	cmp r7, #0
	beq _0800A408
	mov r2, sl
	cmp r2, #0
	bne _0800A408
	ldr r0, [r6]
	ldr r1, [r6, #4]
	str r0, [sp, #8]
	str r1, [sp, #0xc]
	mov r0, r8
	ldrb r5, [r0, #5]
	ldr r0, [r6]
	str r0, [sp, #0x14]
	add r4, sp, #8
	adds r0, r4, #0
	adds r1, r7, #0
	mov r2, r8
	bl sub_8008278
	ldr r0, [sp, #8]
	asrs r0, r0, #8
	str r0, [sp, #8]
	ldr r0, [r4, #4]
	asrs r0, r0, #8
	str r0, [r4, #4]
	ldr r0, _0800A41C @ =gUnknown_03001308
	ldr r0, [r0]
	add r1, sp, #0x14
	str r1, [sp]
	adds r1, r7, #0
	adds r2, r4, #0
	adds r3, r5, #0
	bl sub_8026628
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0800A408
	ldr r0, [r6, #0x74]
	orrs r0, r7
	str r0, [r6, #0x74]
	mov r1, sb
	orrs r1, r7
	mov sb, r1
	ldr r0, [sp, #0x14]
	str r0, [r6]
_0800A408:
	mov r0, sb
	add sp, #0x1c
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0800A41C: .4byte gUnknown_03001308

	thumb_func_start sub_800A420
sub_800A420: @ 0x0800A420
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #0xc
	adds r4, r0, #0
	adds r5, r1, #0
	mov sb, r2
	ldr r0, [r4, #4]
	str r0, [sp, #8]
	ldr r0, [r4]
	ldr r1, [r4, #4]
	str r0, [sp]
	str r1, [sp, #4]
	mov r0, sp
	movs r1, #8
	adds r2, r5, #0
	bl sub_8008200
	ldr r0, [sp]
	asrs r0, r0, #8
	str r0, [sp]
	ldr r0, [sp, #4]
	asrs r2, r0, #8
	str r2, [sp, #4]
	ldrb r1, [r4, #0xd]
	lsrs r0, r1, #1
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	bne _0800A462
	subs r0, r2, #1
	str r0, [sp, #4]
_0800A462:
	adds r0, r4, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _0800A478
	ldrb r5, [r5, #4]
	lsrs r1, r5, #1
	ldr r0, [sp]
	subs r0, r0, r1
	b _0800A480
_0800A478:
	ldrb r5, [r5, #4]
	lsrs r1, r5, #1
	ldr r0, [sp]
	adds r0, r0, r1
_0800A480:
	str r0, [sp]
	ldr r2, _0800A4BC @ =gUnknown_03001308
	mov r8, r2
	ldr r0, [r2]
	add r6, sp, #8
	mov r1, sp
	adds r2, r6, #0
	bl sub_8026BF8
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	cmp r5, #0
	beq _0800A4C4
	ldr r3, [sp, #8]
	ldr r0, _0800A4C0 @ =0xFFFFFF00
	ands r3, r0
	str r3, [r4, #4]
	ldrb r2, [r4, #0xd]
	lsrs r0, r2, #1
	movs r1, #1
	ands r0, r1
	adds r1, r2, #0
	cmp r0, #0
	bne _0800A4B6
	ldr r2, _0800A4C0 @ =0xFFFFFF00
	adds r0, r3, r2
	str r0, [r4, #4]
_0800A4B6:
	movs r0, #2
	orrs r0, r1
	b _0800A516
	.align 2, 0
_0800A4BC: .4byte gUnknown_03001308
_0800A4C0: .4byte 0xFFFFFF00
_0800A4C4:
	ldrb r1, [r4, #0xd]
	lsrs r0, r1, #1
	movs r7, #1
	ands r0, r7
	cmp r0, #0
	bne _0800A50A
	ldr r0, [sp, #4]
	adds r0, #1
	str r0, [sp, #4]
	mov r1, r8
	ldr r0, [r1]
	mov r1, sp
	adds r2, r6, #0
	bl sub_8026BF8
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	cmp r5, #0
	beq _0800A500
	ldr r0, [sp, #8]
	ldr r1, _0800A4FC @ =0xFFFFFF00
	ands r0, r1
	str r0, [r4, #4]
	movs r0, #2
	ldrb r2, [r4, #0xd]
	orrs r0, r2
	b _0800A516
	.align 2, 0
_0800A4FC: .4byte 0xFFFFFF00
_0800A500:
	ldrb r1, [r4, #0xd]
	lsrs r0, r1, #1
	ands r0, r7
	cmp r0, #0
	beq _0800A510
_0800A50A:
	movs r0, #1
	mov r2, sb
	strb r0, [r2]
_0800A510:
	movs r0, #3
	rsbs r0, r0, #0
	ands r0, r1
_0800A516:
	strb r0, [r4, #0xd]
	adds r0, r5, #0
	add sp, #0xc
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_800A528
sub_800A528: @ 0x0800A528
	push {r4, r5, lr}
	adds r4, r0, #0
	bl sub_8009FB0
	ldr r1, [r4, #0x18]
	movs r2, #0x10
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #0x14]
	bl sub_803AD7C
	adds r3, r0, #0
	ldr r1, [r4, #0x1c]
	cmp r1, r3
	beq _0800A586
	cmp r1, #0
	beq _0800A586
	adds r0, r4, #0
	adds r0, #0x68
	ldrb r0, [r0]
	cmp r0, #8
	bne _0800A56C
	movs r5, #2
	ldrsh r0, [r1, r5]
	ldrb r1, [r1, #5]
	adds r2, r1, r0
	movs r1, #2
	ldrsh r0, [r3, r1]
	ldrb r5, [r3, #5]
	adds r1, r5, r0
	cmp r2, r1
	beq _0800A586
	subs r1, r2, r1
	b _0800A57E
_0800A56C:
	cmp r0, #4
	bne _0800A586
	movs r2, #2
	ldrsh r0, [r1, r2]
	movs r5, #2
	ldrsh r1, [r3, r5]
	cmp r0, r1
	beq _0800A586
	subs r1, r0, r1
_0800A57E:
	lsls r1, r1, #8
	ldr r0, [r4, #4]
	adds r0, r0, r1
	str r0, [r4, #4]
_0800A586:
	str r3, [r4, #0x1c]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_800A590
sub_800A590: @ 0x0800A590
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r1, [r4, #0x18]
	movs r2, #0x10
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #0x14]
	bl sub_803AD7C
	adds r3, r0, #0
	ldr r1, [r4, #0x1c]
	cmp r1, r3
	beq _0800A5EA
	cmp r1, #0
	beq _0800A5EA
	adds r0, r4, #0
	adds r0, #0x68
	ldrb r0, [r0]
	cmp r0, #8
	bne _0800A5D0
	movs r5, #2
	ldrsh r0, [r1, r5]
	ldrb r1, [r1, #5]
	adds r2, r1, r0
	movs r1, #2
	ldrsh r0, [r3, r1]
	ldrb r5, [r3, #5]
	adds r1, r5, r0
	cmp r2, r1
	beq _0800A5EA
	subs r1, r2, r1
	b _0800A5E2
_0800A5D0:
	cmp r0, #4
	bne _0800A5EA
	movs r2, #2
	ldrsh r0, [r1, r2]
	movs r5, #2
	ldrsh r1, [r3, r5]
	cmp r0, r1
	beq _0800A5EA
	subs r1, r0, r1
_0800A5E2:
	lsls r1, r1, #8
	ldr r0, [r4, #4]
	adds r0, r0, r1
	str r0, [r4, #4]
_0800A5EA:
	str r3, [r4, #0x1c]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

