.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8008D80 is reconstructed (semantics fully understood, but not
@ yet byte-matching) as C in src/graphics/actor_part7.c, guarded by
@ #if NON_MATCHING - see docs/matching.md for the exact remaining gap.
.if NON_MATCHING == 0
	thumb_func_start sub_8008D80
sub_8008D80: @ 0x08008D80
	sub sp, #0xc
	push {r4, r5, lr}
	str r1, [sp, #0xc]
	str r2, [sp, #0x10]
	str r3, [sp, #0x14]
	ldr r4, [sp, #0x1c]
	ldr r5, [sp, #0x20]
	adds r0, r4, #0
	add r1, sp, #0xc
	bl sub_8009FF4
	cmp r0, #0
	beq _08008DB8
	ldr r1, [r4, #0x18]
	adds r1, #0x68
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldrb r2, [r5, #0xa]
	ldr r4, [r1, #4]
	movs r1, #1
	movs r3, #0
	bl sub_803AD88
	movs r0, #8
	ldrb r1, [r5, #0xc]
	orrs r0, r1
	strb r0, [r5, #0xc]
_08008DB8:
	pop {r4, r5}
	pop {r3}
	add sp, #0xc
	bx r3
.endif

	thumb_func_start sub_8008DC0
sub_8008DC0: @ 0x08008DC0
	push {r4, r5, lr}
	adds r5, r0, #0
	movs r4, #0
	b _08008DE0
_08008DC8:
	ldr r1, [r5, #0x10]
	lsls r0, r4, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r2, [r0, #0x18]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r2, #0x24]
	bl sub_803AD7C
	adds r4, #1
_08008DE0:
	ldr r0, [r5, #8]
	cmp r4, r0
	blt _08008DC8
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_8008DEC
sub_8008DEC: @ 0x08008DEC
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	movs r3, #0
	ldr r2, [r4]
	cmp r3, r2
	bge _08008E44
	ldr r0, [r4, #0xc]
	ldr r1, [r0]
	adds r6, r0, #0
	cmp r1, r5
	beq _08008E14
	adds r1, r6, #0
_08008E06:
	adds r1, #4
	adds r3, #1
	cmp r3, r2
	bge _08008E44
	ldr r0, [r1]
	cmp r0, r5
	bne _08008E06
_08008E14:
	ldr r0, [r4]
	cmp r3, r0
	bge _08008E44
	lsls r1, r3, #2
	adds r0, r1, #4
	adds r0, r6, r0
	adds r1, r6, r1
	ldr r2, [r4, #4]
	subs r2, r2, r3
	ldr r3, _08008E4C @ =0x001FFFFF
	ands r2, r3
	movs r3, #0x80
	lsls r3, r3, #0x13
	orrs r2, r3
	bl sub_803A94C
	ldr r0, [r4, #4]
	subs r0, #1
	str r0, [r4, #4]
	ldr r1, [r4, #0xc]
	lsls r0, r0, #2
	adds r0, r0, r1
	movs r1, #0
	str r1, [r0]
_08008E44:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08008E4C: .4byte 0x001FFFFF

	thumb_func_start sub_8008E50
sub_8008E50: @ 0x08008E50
	push {r4, lr}
	adds r4, r0, #0
	adds r3, r1, #0
	ldr r0, [r4]
	cmp r3, r0
	bge _08008E88
	lsls r2, r3, #2
	adds r0, r2, #4
	ldr r1, [r4, #0xc]
	adds r0, r1, r0
	adds r1, r1, r2
	ldr r2, [r4, #4]
	subs r2, r2, r3
	ldr r3, _08008E90 @ =0x001FFFFF
	ands r2, r3
	movs r3, #0x80
	lsls r3, r3, #0x13
	orrs r2, r3
	bl sub_803A94C
	ldr r0, [r4, #4]
	subs r0, #1
	str r0, [r4, #4]
	ldr r1, [r4, #0xc]
	lsls r0, r0, #2
	adds r0, r0, r1
	movs r1, #0
	str r1, [r0]
_08008E88:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08008E90: .4byte 0x001FFFFF

	thumb_func_start sub_8008E94
sub_8008E94: @ 0x08008E94
	push {r4, lr}
	adds r3, r0, #0
	adds r4, r1, #0
	ldr r2, [r3, #4]
	ldr r0, [r3]
	cmp r2, r0
	bge _08008EAE
	ldr r0, [r3, #0xc]
	lsls r1, r2, #2
	adds r1, r1, r0
	str r4, [r1]
	adds r0, r2, #1
	str r0, [r3, #4]
_08008EAE:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8008EB4
sub_8008EB4: @ 0x08008EB4
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, [r4, #0x10]
	cmp r0, #0
	beq _08008EC4
	bl sub_8026EB4
_08008EC4:
	ldr r0, [r4, #0xc]
	cmp r0, #0
	beq _08008ECE
	bl sub_8026EB4
_08008ECE:
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _08008EDC
	adds r0, r4, #0
	bl sub_8026ED0
_08008EDC:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8008EE4
sub_8008EE4: @ 0x08008EE4
	push {r4, r5, lr}
	adds r5, r0, #0
	adds r4, r1, #0
	movs r0, #0
	str r0, [r5, #4]
	str r0, [r5, #8]
	str r4, [r5]
	lsls r4, r4, #2
	adds r0, r4, #0
	bl sub_8026EC0
	str r0, [r5, #0xc]
	adds r0, r4, #0
	bl sub_8026EC0
	str r0, [r5, #0x10]
	ldr r0, [r5]
	cmp r0, #0
	ble _08008F16
	movs r2, #0
	ldr r1, [r5, #0xc]
_08008F0E:
	stm r1!, {r2}
	subs r0, #1
	cmp r0, #0
	bne _08008F0E
_08008F16:
	adds r0, r5, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8008F20
sub_8008F20: @ 0x08008F20
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	adds r5, r0, #0
	adds r0, r1, #0
	movs r1, #0
	str r1, [r5]
	str r0, [r5, #4]
	lsls r0, r0, #2
	bl sub_8026EC0
	str r0, [r5, #8]
	ldr r1, [r5, #4]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	bl sub_8026EC0
	str r0, [r5, #0xc]
	movs r0, #0x81
	lsls r0, r0, #4
	adds r4, r5, r0
	ldr r0, [r5, #4]
	lsls r0, r0, #3
	bl sub_8026EC0
	str r0, [r4]
	ldr r0, [r5, #4]
	cmp r0, #0
	ble _08008F6C
	movs r2, #0
	ldr r1, [r5, #8]
_08008F64:
	stm r1!, {r2}
	subs r0, #1
	cmp r0, #0
	bne _08008F64
_08008F6C:
	ldr r3, [r5, #4]
	movs r1, #0x81
	lsls r1, r1, #4
	adds r1, r1, r5
	mov sb, r1
	ldr r2, _08008FD8 @ =0x00000814
	adds r2, r2, r5
	mov sl, r2
	movs r0, #0
	movs r1, #0x82
	lsls r1, r1, #3
	adds r2, r5, r1
	adds r1, r5, #0
	adds r1, #0x10
	movs r4, #0xff
_08008F8A:
	stm r1!, {r0}
	stm r2!, {r0}
	subs r4, #1
	cmp r4, #0
	bge _08008F8A
	movs r4, #0
	cmp r4, r3
	bge _08008FF0
	mov ip, sb
	movs r7, #0
	movs r2, #8
	mov r8, r2
	movs r6, #0
_08008FA4:
	mov r0, ip
	ldr r1, [r0]
	lsls r2, r4, #3
	adds r1, r2, r1
	ldr r0, [r5, #0xc]
	adds r0, r0, r6
	str r0, [r1]
	ldr r0, [r5, #0xc]
	adds r0, r6, r0
	str r7, [r0]
	str r7, [r0, #4]
	str r7, [r0, #0xc]
	strb r7, [r0, #0x10]
	ldr r0, [r5, #0xc]
	adds r0, r6, r0
	mov r1, ip
	ldr r3, [r1]
	adds r1, r3, r2
	str r1, [r0, #8]
	ldr r0, [r5, #4]
	subs r0, #1
	cmp r4, r0
	bne _08008FDC
	str r7, [r1, #4]
	b _08008FE2
	.align 2, 0
_08008FD8: .4byte 0x00000814
_08008FDC:
	mov r2, r8
	adds r0, r3, r2
	str r0, [r1, #4]
_08008FE2:
	movs r0, #8
	add r8, r0
	adds r6, #0x14
	adds r4, #1
	ldr r0, [r5, #4]
	cmp r4, r0
	blt _08008FA4
_08008FF0:
	mov r1, sb
	ldr r0, [r1]
	mov r2, sl
	str r0, [r2]
	adds r0, r5, #0
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_8009008
sub_8009008: @ 0x08009008
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov r8, r0
	mov sl, r1
	movs r0, #2
	ldrsh r2, [r1, r0]
	lsls r1, r2, #2
	mov r0, r8
	adds r0, #0x10
	adds r0, r0, r1
	ldr r5, [r0]
	movs r6, #0
	movs r1, #0
	str r1, [sp]
	cmp r5, #0
	beq _08009096
	movs r4, #0x82
	lsls r4, r4, #3
	add r4, r8
	ldr r7, _08009064 @ =0x00000814
	add r7, r8
_0800903A:
	ldr r0, [r5]
	cmp r0, sl
	bne _0800908E
	movs r0, #0
	str r0, [r5]
	ldr r0, [sp]
	adds r0, #1
	str r0, [sp]
	lsls r1, r2, #2
	mov r0, r8
	adds r0, #0x10
	adds r3, r0, r1
	ldr r0, [r3]
	cmp r5, r0
	bne _08009070
	ldr r2, [r5, #4]
	cmp r2, #0
	beq _08009068
	str r2, [r3]
	b _08009082
	.align 2, 0
_08009064: .4byte 0x00000814
_08009068:
	str r2, [r3]
	adds r0, r4, r1
	str r2, [r0]
	b _08009082
_08009070:
	cmp r6, #0
	beq _08009082
	adds r1, r4, r1
	ldr r0, [r1]
	cmp r5, r0
	bne _0800907E
	str r6, [r1]
_0800907E:
	ldr r0, [r5, #4]
	str r0, [r6, #4]
_08009082:
	ldr r1, [r7]
	ldr r0, [r5, #8]
	str r1, [r0, #4]
	ldr r0, [r5, #8]
	str r0, [r7]
	b _08009096
_0800908E:
	adds r6, r5, #0
	ldr r5, [r6, #4]
	cmp r5, #0
	bne _0800903A
_08009096:
	mov r2, sl
	ldrh r1, [r2, #8]
	cmp r5, #0
	beq _080090B0
	ldr r0, _080090F8 @ =0x0000FFFF
	cmp r1, r0
	beq _080090B0
	ldrb r1, [r2, #0xc]
	lsrs r0, r1, #4
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _08009140
_080090B0:
	movs r1, #0xff
	movs r2, #0x10
	add r2, r8
	mov sb, r2
_080090B8:
	movs r6, #0
	lsls r0, r1, #2
	add r0, sb
	ldr r3, [r0]
	cmp r3, #0
	beq _0800913A
	movs r0, #0x82
	lsls r0, r0, #3
	add r0, r8
	mov ip, r0
	ldr r7, _080090FC @ =0x00000814
	add r7, r8
_080090D0:
	ldr r0, [r3]
	cmp r0, sl
	bne _08009132
	adds r5, r3, #0
	movs r0, #0
	str r0, [r3]
	ldr r2, [sp]
	adds r2, #1
	str r2, [sp]
	lsls r4, r1, #2
	mov r0, sb
	adds r2, r0, r4
	ldr r0, [r2]
	cmp r3, r0
	bne _0800910A
	ldr r1, [r3, #4]
	cmp r1, #0
	beq _08009100
	str r1, [r2]
	b _0800911E
	.align 2, 0
_080090F8: .4byte 0x0000FFFF
_080090FC: .4byte 0x00000814
_08009100:
	str r1, [r2]
	mov r2, ip
	adds r0, r2, r4
	str r1, [r0]
	b _0800911E
_0800910A:
	cmp r6, #0
	beq _0800911E
	mov r0, ip
	adds r1, r0, r4
	ldr r0, [r1]
	cmp r3, r0
	bne _0800911A
	str r6, [r1]
_0800911A:
	ldr r0, [r3, #4]
	str r0, [r6, #4]
_0800911E:
	ldr r1, [r7]
	ldr r0, [r5, #8]
	str r1, [r0, #4]
	ldr r0, [r5, #8]
	str r0, [r7]
	movs r1, #0x80
	lsls r1, r1, #1
	ldr r2, [sp]
	cmp r2, #1
	bgt _0800913A
_08009132:
	adds r6, r3, #0
	ldr r3, [r3, #4]
	cmp r3, #0
	bne _080090D0
_0800913A:
	subs r1, #1
	cmp r1, #0
	bge _080090B8
_08009140:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8009150
sub_8009150: @ 0x08009150
	push {r4, r5, r6, r7, lr}
	adds r7, r0, #0
	adds r3, r1, #0
	movs r2, #0xfe
_08009158:
	lsls r1, r2, #2
	adds r0, r7, #0
	adds r0, #0x10
	adds r0, r0, r1
	ldr r4, [r0]
	cmp r4, #0
	beq _080091C6
	ldr r0, _080091B4 @ =0x00000814
	adds r6, r7, r0
_0800916A:
	ldr r5, [r4]
	cmp r5, r3
	bne _080091C0
	ldrb r1, [r5, #0xc]
	lsrs r0, r1, #4
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _080091CC
	ldr r3, [r4, #0xc]
	cmp r3, #0
	bne _080091CC
	ldr r0, [r6]
	ldr r2, [r0]
	ldr r1, [r0, #4]
	str r1, [r6]
	str r3, [r0, #4]
	str r5, [r2]
	str r3, [r2, #4]
	str r4, [r2, #0xc]
	strb r3, [r2, #0x10]
	strb r3, [r2, #0x11]
	ldr r0, _080091B8 @ =0x0000040C
	adds r1, r7, r0
	ldr r0, [r1]
	cmp r0, #0
	bne _080091A2
	str r2, [r1]
_080091A2:
	ldr r1, _080091BC @ =0x0000080C
	adds r0, r7, r1
	ldr r1, [r0]
	cmp r1, #0
	beq _080091AE
	str r2, [r1, #4]
_080091AE:
	str r2, [r0]
	str r2, [r4, #0xc]
	b _080091CC
	.align 2, 0
_080091B4: .4byte 0x00000814
_080091B8: .4byte 0x0000040C
_080091BC: .4byte 0x0000080C
_080091C0:
	ldr r4, [r4, #4]
	cmp r4, #0
	bne _0800916A
_080091C6:
	subs r2, #1
	cmp r2, #0
	bge _08009158
_080091CC:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80091D4
sub_80091D4: @ 0x080091D4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x1c
	adds r7, r0, #0
	movs r0, #0xdc
	lsls r0, r0, #9
	movs r1, #0x8c
	lsls r1, r1, #9
	str r0, [sp, #8]
	str r1, [sp, #0xc]
	ldr r0, _0800928C @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r2, [r0, #0x10]
	ldr r1, [r2]
	lsls r1, r1, #8
	ldr r0, _08009290 @ =0xFFFF9C00
	adds r1, r1, r0
	ldr r0, [r2, #4]
	lsls r0, r0, #8
	ldr r3, _08009294 @ =0xFFFFC400
	adds r0, r0, r3
	str r1, [sp]
	str r0, [sp, #4]
	ldr r2, [r2]
	asrs r2, r2, #8
	str r2, [sp, #0x10]
	cmp r2, #0
	bge _08009216
	movs r0, #0
	str r0, [sp, #0x10]
_08009216:
	ldr r1, [sp, #0x10]
	adds r1, #2
	adds r2, r7, #0
	adds r2, #0x10
	str r2, [sp, #0x18]
	ldr r3, _08009298 @ =0x0000040C
	adds r3, r3, r7
	mov sl, r3
_08009226:
	lsls r0, r1, #2
	ldr r2, [sp, #0x18]
	adds r0, r2, r0
	ldr r0, [r0]
	mov r8, r0
	subs r1, #1
	str r1, [sp, #0x14]
	cmp r0, #0
	bne _0800923A
	b _0800936E
_0800923A:
	mov r3, r8
	ldr r2, [r3]
	ldrb r0, [r2, #0xc]
	lsrs r1, r0, #4
	movs r0, #1
	ands r1, r0
	adds r5, r2, #0
	cmp r1, #0
	beq _080092A4
	ldr r4, [r3, #0xc]
	cmp r4, #0
	bne _080092A4
	ldr r1, _0800929C @ =0x00000814
	adds r2, r7, r1
	ldr r1, [r2]
	ldr r3, [r1]
	ldr r0, [r1, #4]
	str r0, [r2]
	str r4, [r1, #4]
	str r5, [r3]
	str r4, [r3, #4]
	mov r2, r8
	str r2, [r3, #0xc]
	strb r4, [r3, #0x10]
	strb r4, [r3, #0x11]
	mov r1, sl
	ldr r0, [r1]
	cmp r0, #0
	bne _08009276
	str r3, [r1]
_08009276:
	ldr r2, _080092A0 @ =0x0000080C
	adds r1, r7, r2
	ldr r0, [r1]
	cmp r0, #0
	beq _08009282
	str r3, [r0, #4]
_08009282:
	str r3, [r1]
	mov r0, r8
	str r3, [r0, #0xc]
	b _08009362
	.align 2, 0
_0800928C: .4byte gUnknown_03001308
_08009290: .4byte 0xFFFF9C00
_08009294: .4byte 0xFFFFC400
_08009298: .4byte 0x0000040C
_0800929C: .4byte 0x00000814
_080092A0: .4byte 0x0000080C
_080092A4:
	movs r4, #1
	adds r0, r4, #0
	ldrb r1, [r5, #0xc]
	ands r0, r1
	cmp r0, #0
	beq _08009334
	mov sb, r5
	mov ip, r5
	movs r6, #0
	ldr r2, [r7, #4]
	adds r4, r2, #0
	cmp r6, r4
	bge _08009314
	ldr r0, [r7, #8]
	ldr r1, [r0]
	adds r3, r0, #0
	cmp r1, r5
	beq _080092D8
	adds r1, r3, #0
_080092CA:
	adds r1, #4
	adds r6, #1
	cmp r6, r2
	bge _08009314
	ldr r0, [r1]
	cmp r0, ip
	bne _080092CA
_080092D8:
	cmp r6, r4
	bge _08009314
	lsls r4, r6, #2
	adds r0, r4, r3
	ldr r1, [r0]
	adds r0, r7, #0
	bl sub_8009008
	adds r0, r4, #4
	ldr r1, [r7, #8]
	adds r0, r1, r0
	adds r1, r1, r4
	ldr r2, [r7]
	subs r2, r2, r6
	ldr r3, _08009330 @ =0x001FFFFF
	ands r2, r3
	movs r3, #0x80
	lsls r3, r3, #0x13
	orrs r2, r3
	bl sub_803A94C
	ldr r2, [r7]
	ldr r1, [r7, #8]
	lsls r0, r2, #2
	adds r0, r0, r1
	subs r0, #4
	movs r1, #0
	str r1, [r0]
	subs r2, #1
	str r2, [r7]
_08009314:
	mov r2, sb
	cmp r2, #0
	beq _08009362
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	add r0, sb
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
	b _08009362
	.align 2, 0
_08009330: .4byte 0x001FFFFF
_08009334:
	ldr r1, [r5, #0x18]
	adds r1, #0x40
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #4]
	mov r1, sp
	bl sub_803AD80
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08009362
	mov r3, r8
	ldr r0, [r3]
	ldr r2, [r0, #0x18]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r2, #0x1c]
	bl sub_803AD7C
	mov r0, r8
	strb r4, [r0, #0x10]
_08009362:
	mov r1, r8
	ldr r1, [r1, #4]
	mov r8, r1
	cmp r1, #0
	beq _0800936E
	b _0800923A
_0800936E:
	ldr r1, [sp, #0x14]
	ldr r2, [sp, #0x10]
	cmp r1, r2
	blt _08009378
	b _08009226
_08009378:
	mov r3, sl
	ldr r3, [r3]
	mov r8, r3
	cmp r3, #0
	beq _0800943A
_08009382:
	mov r0, r8
	ldr r2, [r0]
	movs r3, #1
	ldrb r1, [r2, #0xc]
	ands r3, r1
	cmp r3, #0
	beq _08009414
	mov sl, r2
	mov sb, r2
	movs r5, #0
	ldr r6, [r7, #4]
	adds r4, r6, #0
	cmp r5, r4
	bge _080093F4
	ldr r0, [r7, #8]
	ldr r1, [r0]
	adds r3, r0, #0
	cmp r1, r2
	beq _080093B8
	adds r1, r3, #0
_080093AA:
	adds r1, #4
	adds r5, #1
	cmp r5, r6
	bge _080093F4
	ldr r0, [r1]
	cmp r0, sb
	bne _080093AA
_080093B8:
	cmp r5, r4
	bge _080093F4
	lsls r4, r5, #2
	adds r0, r4, r3
	ldr r1, [r0]
	adds r0, r7, #0
	bl sub_8009008
	adds r0, r4, #4
	ldr r1, [r7, #8]
	adds r0, r1, r0
	adds r1, r1, r4
	ldr r2, [r7]
	subs r2, r2, r5
	ldr r3, _08009410 @ =0x001FFFFF
	ands r2, r3
	movs r3, #0x80
	lsls r3, r3, #0x13
	orrs r2, r3
	bl sub_803A94C
	ldr r2, [r7]
	ldr r1, [r7, #8]
	lsls r0, r2, #2
	adds r0, r0, r1
	subs r0, #4
	movs r1, #0
	str r1, [r0]
	subs r2, #1
	str r2, [r7]
_080093F4:
	mov r2, sl
	cmp r2, #0
	beq _08009430
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	add r0, sl
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
	b _08009430
	.align 2, 0
_08009410: .4byte 0x001FFFFF
_08009414:
	mov r0, r8
	ldr r1, [r0, #0xc]
	ldrb r0, [r1, #0x10]
	cmp r0, #0
	bne _0800942E
	ldr r1, [r2, #0x18]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r1, [r1, #0x1c]
	bl sub_803AD7C
	b _08009430
_0800942E:
	strb r3, [r1, #0x10]
_08009430:
	mov r0, r8
	ldr r0, [r0, #4]
	mov r8, r0
	cmp r0, #0
	bne _08009382
_0800943A:
	add sp, #0x1c
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_800944C
sub_800944C: @ 0x0800944C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x10
	adds r3, r0, #0
	ldr r0, _08009508 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r2, [r0, #0x10]
	ldr r1, [r2]
	lsls r1, r1, #8
	ldr r0, [r2, #4]
	lsls r0, r0, #8
	str r1, [sp]
	str r0, [sp, #4]
	movs r0, #0xf0
	lsls r0, r0, #8
	movs r1, #0xa0
	lsls r1, r1, #8
	str r0, [sp, #8]
	str r1, [sp, #0xc]
	ldr r5, [r2]
	asrs r5, r5, #8
	cmp r5, #0
	bge _0800947E
	movs r5, #0
_0800947E:
	adds r1, r5, #2
	adds r7, r3, #0
	adds r7, #0x10
	ldr r0, _0800950C @ =0x0000040C
	adds r0, r0, r3
	mov r8, r0
_0800948A:
	lsls r0, r1, #2
	adds r0, r7, r0
	ldr r4, [r0]
	subs r6, r1, #1
	cmp r4, #0
	beq _080094C8
_08009496:
	ldr r0, [r4]
	ldr r2, [r0, #0x18]
	movs r3, #0x30
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x34]
	mov r1, sp
	bl sub_803AD80
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _080094C2
	ldr r0, [r4]
	ldr r2, [r0, #0x18]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r2, #0x24]
	bl sub_803AD7C
	movs r0, #1
	strb r0, [r4, #0x11]
_080094C2:
	ldr r4, [r4, #4]
	cmp r4, #0
	bne _08009496
_080094C8:
	adds r1, r6, #0
	cmp r1, r5
	bge _0800948A
	mov r0, r8
	ldr r4, [r0]
	cmp r4, #0
	beq _0800951A
_080094D6:
	ldr r1, [r4, #0xc]
	ldrb r0, [r1, #0x11]
	cmp r0, #0
	bne _08009510
	ldr r0, [r4]
	ldr r2, [r0, #0x18]
	movs r3, #0x30
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x34]
	mov r1, sp
	bl sub_803AD80
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08009514
	ldr r0, [r4]
	ldr r2, [r0, #0x18]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r2, #0x24]
	bl sub_803AD7C
	b _08009514
	.align 2, 0
_08009508: .4byte gUnknown_03001308
_0800950C: .4byte 0x0000040C
_08009510:
	movs r0, #0
	strb r0, [r1, #0x11]
_08009514:
	ldr r4, [r4, #4]
	cmp r4, #0
	bne _080094D6
_0800951A:
	add sp, #0x10
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8009528
sub_8009528: @ 0x08009528
	sub sp, #0xc
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x30
	adds r7, r0, #0
	str r1, [sp, #0x50]
	str r2, [sp, #0x54]
	str r3, [sp, #0x58]
	ldr r0, [sp, #0x64]
	mov sb, r0
	ldr r0, _080095E4 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r2, [r0, #0x10]
	ldr r1, [r2]
	lsls r1, r1, #8
	ldr r0, [r2, #4]
	lsls r0, r0, #8
	str r1, [sp, #0xc]
	str r0, [sp, #0x10]
	movs r0, #0xf0
	lsls r0, r0, #8
	movs r1, #0xa0
	lsls r1, r1, #8
	str r0, [sp, #0x14]
	str r1, [sp, #0x18]
	ldr r6, [r2]
	asrs r6, r6, #8
	cmp r6, #0
	bge _0800956A
	movs r6, #0
_0800956A:
	adds r1, r6, #2
	movs r2, #0x10
	adds r2, r2, r7
	mov sl, r2
	ldr r0, _080095E8 @ =0x0000040C
	adds r0, r7, r0
	str r0, [sp, #0x2c]
_08009578:
	lsls r0, r1, #2
	add r0, sl
	ldr r5, [r0]
	subs r1, #1
	mov r8, r1
	cmp r5, #0
	beq _08009616
_08009586:
	ldr r4, [r5]
	ldr r1, [r4, #0x18]
	movs r2, #0x30
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x34]
	add r1, sp, #0xc
	bl sub_803AD80
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08009610
	ldrb r1, [r4, #0xc]
	lsrs r0, r1, #2
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _08009610
	ldr r1, [r4, #0x18]
	adds r1, #0x48
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	cmp r0, #4
	ble _08009610
	ldr r0, _080095EC @ =gUnknown_030012D8
	ldr r0, [r0]
	cmp sb, r0
	bne _080095F0
	add r0, sp, #0x1c
	add r1, sp, #0x50
	movs r2, #0x10
	bl sub_800014C
	str r4, [sp, #4]
	ldr r0, [sp, #0x28]
	str r0, [sp]
	ldr r1, [sp, #0x1c]
	ldr r2, [sp, #0x20]
	ldr r3, [sp, #0x24]
	adds r0, r7, #0
	bl sub_80096C0
	b _08009610
	.align 2, 0
_080095E4: .4byte gUnknown_03001308
_080095E8: .4byte 0x0000040C
_080095EC: .4byte gUnknown_030012D8
_080095F0:
	add r0, sp, #0x1c
	add r1, sp, #0x50
	movs r2, #0x10
	bl sub_800014C
	str r4, [sp, #4]
	mov r0, sb
	str r0, [sp, #8]
	ldr r0, [sp, #0x28]
	str r0, [sp]
	ldr r1, [sp, #0x1c]
	ldr r2, [sp, #0x20]
	ldr r3, [sp, #0x24]
	adds r0, r7, #0
	bl sub_80099F0
_08009610:
	ldr r5, [r5, #4]
	cmp r5, #0
	bne _08009586
_08009616:
	mov r1, r8
	cmp r1, r6
	bge _08009578
	ldr r1, [sp, #0x2c]
	ldr r5, [r1]
	cmp r5, #0
	beq _080096AE
_08009624:
	ldr r4, [r5]
	ldr r1, [r4, #0x18]
	movs r2, #0x30
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x34]
	add r1, sp, #0xc
	bl sub_803AD80
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _080096A8
	ldrb r1, [r4, #0xc]
	lsrs r0, r1, #2
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _080096A8
	ldr r1, [r4, #0x18]
	adds r1, #0x48
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	cmp r0, #4
	ble _080096A8
	ldr r0, _08009684 @ =gUnknown_030012D8
	ldr r0, [r0]
	cmp sb, r0
	bne _08009688
	add r0, sp, #0x1c
	add r1, sp, #0x50
	movs r2, #0x10
	bl sub_800014C
	str r4, [sp, #4]
	ldr r0, [sp, #0x28]
	str r0, [sp]
	ldr r1, [sp, #0x1c]
	ldr r2, [sp, #0x20]
	ldr r3, [sp, #0x24]
	adds r0, r7, #0
	bl sub_80096C0
	b _080096A8
	.align 2, 0
_08009684: .4byte gUnknown_030012D8
_08009688:
	add r0, sp, #0x1c
	add r1, sp, #0x50
	movs r2, #0x10
	bl sub_800014C
	str r4, [sp, #4]
	mov r0, sb
	str r0, [sp, #8]
	ldr r0, [sp, #0x28]
	str r0, [sp]
	ldr r1, [sp, #0x1c]
	ldr r2, [sp, #0x20]
	ldr r3, [sp, #0x24]
	adds r0, r7, #0
	bl sub_80099F0
_080096A8:
	ldr r5, [r5, #4]
	cmp r5, #0
	bne _08009624
_080096AE:
	add sp, #0x30
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r3}
	add sp, #0xc
	bx r3

	thumb_func_start sub_80096C0
sub_80096C0: @ 0x080096C0
	sub sp, #0xc
	push {r4, r5, r6, lr}
	sub sp, #0x20
	str r1, [sp, #0x30]
	str r2, [sp, #0x34]
	str r3, [sp, #0x38]
	ldr r5, [sp, #0x40]
	ldr r4, _080096FC @ =gUnknown_030012C0
	ldr r0, [r4]
	ldr r0, [r0, #0x78]
	cmp r0, #3
	bne _08009704
	adds r0, r5, #0
	add r1, sp, #0x30
	bl sub_8009FF4
	cmp r0, #0
	bne _080096E6
	b _0800985A
_080096E6:
	ldr r3, [r5, #0x18]
	adds r3, #0x68
	movs r1, #0
	ldrsh r0, [r3, r1]
	adds r0, r5, r0
	ldr r1, _08009700 @ =gUnknown_030012D8
	ldr r1, [r1]
	ldrb r2, [r1, #0xa]
	ldr r4, [r3, #4]
	b _08009810
	.align 2, 0
_080096FC: .4byte gUnknown_030012C0
_08009700: .4byte gUnknown_030012D8
_08009704:
	ldrb r2, [r5, #0xd]
	lsrs r0, r2, #3
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _0800978C
	ldr r6, _08009764 @ =gUnknown_030012D8
	ldr r1, [r6]
	mov r0, sp
	bl sub_8007B98
	add r4, sp, #0x10
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_8007CF8
	mov r0, sp
	adds r1, r4, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08009734
	b _0800985A
_08009734:
	ldr r3, [r5]
	ldr r2, [r6]
	ldr r0, [r2]
	cmp r3, r0
	bge _08009768
	ldr r0, [r4, #8]
	ldr r1, [sp, #8]
	adds r0, r0, r1
	lsls r0, r0, #7
	adds r0, r3, r0
	str r0, [r2]
	ldr r1, [r2, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xc
	movs r3, #2
	bl sub_803AD88
	b _0800985A
	.align 2, 0
_08009764: .4byte gUnknown_030012D8
_08009768:
	ldr r0, [r4, #8]
	ldr r1, [sp, #8]
	adds r0, r0, r1
	lsls r0, r0, #7
	subs r0, r3, r0
	str r0, [r2]
	ldr r1, [r2, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xc
	movs r3, #1
	bl sub_803AD88
	b _0800985A
_0800978C:
	adds r0, r5, #0
	add r1, sp, #0x30
	bl sub_8009FF4
	cmp r0, #1
	beq _080097A2
	cmp r0, #1
	ble _0800985A
	cmp r0, #2
	beq _0800981A
	b _0800985A
_080097A2:
	ldr r6, _080097FC @ =gUnknown_030012D8
	ldr r1, [r6]
	movs r0, #8
	ldrb r2, [r1, #0xc]
	orrs r0, r2
	strb r0, [r1, #0xc]
	ldr r0, [r6]
	ldrb r2, [r0, #0xa]
	cmp r2, #1
	bne _08009804
	ldr r0, [r0, #0x64]
	cmp r0, #0
	ble _0800985A
	ldr r1, [r5, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r4, [r1, #4]
	movs r1, #1
	movs r2, #1
	movs r3, #0
	bl sub_803AD88
	ldr r0, [r6]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xd
	movs r3, #0
	bl sub_803AD88
	ldr r0, _08009800 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x21
	bl PlaySfx
	b _0800985A
	.align 2, 0
_080097FC: .4byte gUnknown_030012D8
_08009800: .4byte gUnknown_030012BC
_08009804:
	ldr r1, [r5, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r4, [r1, #4]
_08009810:
	movs r1, #1
	movs r3, #0
	bl sub_803AD88
	b _0800985A
_0800981A:
	movs r0, #8
	ldrb r1, [r5, #0xc]
	orrs r0, r1
	strb r0, [r5, #0xc]
	ldr r0, [r4]
	ldr r0, [r0, #0x78]
	cmp r0, #0
	beq _08009840
	ldr r1, [r5, #0x18]
	adds r1, #0x68
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r4, [r1, #4]
	movs r1, #1
	movs r2, #1
	movs r3, #0
	bl sub_803AD88
_08009840:
	ldr r0, _08009864 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldrb r2, [r5, #0xa]
	ldr r4, [r1, #4]
	movs r1, #1
	movs r3, #0
	bl sub_803AD88
_0800985A:
	add sp, #0x20
	pop {r4, r5, r6}
	pop {r3}
	add sp, #0xc
	bx r3
	.align 2, 0
_08009864: .4byte gUnknown_030012D8

	thumb_func_start sub_8009868
sub_8009868: @ 0x08009868
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	adds r4, r0, #0
	ldr r0, _080098BC @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r6, [r0]
	asrs r6, r6, #8
	cmp r6, #0
	bge _08009884
	movs r6, #0
_08009884:
	adds r2, r6, #2
	ldr r0, _080098C0 @ =gUnknown_030012D8
	ldr r3, [r0]
	adds r0, r3, #0
	adds r0, #0x88
	ldrb r1, [r0]
	movs r0, #0x10
	adds r0, r0, r4
	mov r8, r0
	cmp r1, #3
	bne _080098C4
_0800989A:
	lsls r0, r2, #2
	add r0, r8
	ldr r4, [r0]
	subs r5, r2, #1
	cmp r4, #0
	beq _080098B2
_080098A6:
	ldr r0, [r4]
	bl sub_800D040
	ldr r4, [r4, #4]
	cmp r4, #0
	bne _080098A6
_080098B2:
	adds r2, r5, #0
	cmp r2, r6
	bge _0800989A
	b _08009906
	.align 2, 0
_080098BC: .4byte gUnknown_03001308
_080098C0: .4byte gUnknown_030012D8
_080098C4:
	ldr r0, [r3, #0x44]
	ldr r7, [r0, #8]
	ldr r0, [r3]
	mov sl, r0
	ldr r0, [r3, #4]
	mov sb, r0
	cmp r1, #1
	bne _080098DE
	movs r7, #0
	ldrb r3, [r3, #0xa]
	cmp r3, #0x13
	bne _080098DE
	movs r7, #0xd
_080098DE:
	adds r4, #0x10
	mov r8, r4
_080098E2:
	lsls r0, r2, #2
	add r0, r8
	ldr r4, [r0]
	subs r5, r2, #1
	cmp r4, #0
	beq _08009900
_080098EE:
	ldr r0, [r4]
	adds r1, r7, #0
	mov r2, sl
	mov r3, sb
	bl sub_80109A4
	ldr r4, [r4, #4]
	cmp r4, #0
	bne _080098EE
_08009900:
	adds r2, r5, #0
	cmp r2, r6
	bge _080098E2
_08009906:
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8009914
sub_8009914: @ 0x08009914
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	adds r4, r0, #0
	movs r6, #0
	b _0800994C
_08009924:
	ldr r0, [r4, #8]
	lsls r5, r6, #2
	adds r0, r5, r0
	ldr r2, [r0]
	cmp r2, #0
	beq _08009942
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_08009942:
	ldr r0, [r4, #8]
	adds r0, r5, r0
	movs r1, #0
	str r1, [r0]
	adds r6, #1
_0800994C:
	ldr r0, [r4]
	cmp r6, r0
	blt _08009924
	movs r0, #0
	str r0, [r4]
	ldr r3, [r4, #4]
	movs r0, #0x81
	lsls r0, r0, #4
	adds r0, r0, r4
	mov sb, r0
	ldr r1, _080099C0 @ =0x00000814
	adds r1, r1, r4
	mov sl, r1
	movs r0, #0
	movs r1, #0x82
	lsls r1, r1, #3
	adds r2, r4, r1
	adds r1, r4, #0
	adds r1, #0x10
	movs r5, #0xff
_08009974:
	stm r1!, {r0}
	stm r2!, {r0}
	subs r5, #1
	cmp r5, #0
	bge _08009974
	movs r5, #0
	cmp r5, r3
	bge _080099D8
	mov ip, sb
	movs r7, #0
	movs r2, #8
	mov r8, r2
	movs r6, #0
_0800998E:
	mov r3, ip
	ldr r1, [r3]
	lsls r2, r5, #3
	adds r1, r2, r1
	ldr r0, [r4, #0xc]
	adds r0, r0, r6
	str r0, [r1]
	ldr r0, [r4, #0xc]
	adds r0, r6, r0
	str r7, [r0]
	str r7, [r0, #4]
	str r7, [r0, #0xc]
	strb r7, [r0, #0x10]
	ldr r0, [r4, #0xc]
	adds r0, r6, r0
	ldr r3, [r3]
	adds r1, r3, r2
	str r1, [r0, #8]
	ldr r0, [r4, #4]
	subs r0, #1
	cmp r5, r0
	bne _080099C4
	str r7, [r1, #4]
	b _080099CA
	.align 2, 0
_080099C0: .4byte 0x00000814
_080099C4:
	mov r2, r8
	adds r0, r3, r2
	str r0, [r1, #4]
_080099CA:
	movs r3, #8
	add r8, r3
	adds r6, #0x14
	adds r5, #1
	ldr r0, [r4, #4]
	cmp r5, r0
	blt _0800998E
_080099D8:
	mov r1, sb
	ldr r0, [r1]
	mov r2, sl
	str r0, [r2]
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80099F0
sub_80099F0: @ 0x080099F0
	sub sp, #0xc
	push {r4, r5, lr}
	str r1, [sp, #0xc]
	str r2, [sp, #0x10]
	str r3, [sp, #0x14]
	ldr r4, [sp, #0x1c]
	ldr r5, [sp, #0x20]
	adds r0, r4, #0
	add r1, sp, #0xc
	bl sub_8009FF4
	cmp r0, #0
	beq _08009A28
	ldr r1, [r4, #0x18]
	adds r1, #0x68
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldrb r2, [r5, #0xa]
	ldr r4, [r1, #4]
	movs r1, #1
	movs r3, #0
	bl sub_803AD88
	movs r0, #8
	ldrb r1, [r5, #0xc]
	orrs r0, r1
	strb r0, [r5, #0xc]
_08009A28:
	pop {r4, r5}
	pop {r3}
	add sp, #0xc
	bx r3

	thumb_func_start sub_8009A30
sub_8009A30: @ 0x08009A30
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	adds r3, r1, #0
	movs r6, #0
	ldr r2, [r5, #4]
	cmp r6, r2
	bge _08009A96
	ldr r0, [r5, #8]
	ldr r1, [r0]
	adds r7, r0, #0
	cmp r1, r3
	beq _08009A58
	adds r1, r7, #0
_08009A4A:
	adds r1, #4
	adds r6, #1
	cmp r6, r2
	bge _08009A96
	ldr r0, [r1]
	cmp r0, r3
	bne _08009A4A
_08009A58:
	ldr r0, [r5, #4]
	cmp r6, r0
	bge _08009A96
	lsls r4, r6, #2
	adds r0, r4, r7
	ldr r1, [r0]
	adds r0, r5, #0
	bl sub_8009008
	adds r0, r4, #4
	ldr r1, [r5, #8]
	adds r0, r1, r0
	adds r1, r1, r4
	ldr r2, [r5]
	subs r2, r2, r6
	ldr r3, _08009A9C @ =0x001FFFFF
	ands r2, r3
	movs r3, #0x80
	lsls r3, r3, #0x13
	orrs r2, r3
	bl sub_803A94C
	ldr r2, [r5]
	ldr r1, [r5, #8]
	lsls r0, r2, #2
	adds r0, r0, r1
	subs r0, #4
	movs r1, #0
	str r1, [r0]
	subs r2, #1
	str r2, [r5]
_08009A96:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08009A9C: .4byte 0x001FFFFF

	thumb_func_start sub_8009AA0
sub_8009AA0: @ 0x08009AA0
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	ldr r0, [r5, #4]
	cmp r6, r0
	bge _08009AE6
	ldr r0, [r5, #8]
	lsls r4, r6, #2
	adds r0, r4, r0
	ldr r1, [r0]
	adds r0, r5, #0
	bl sub_8009008
	adds r0, r4, #4
	ldr r1, [r5, #8]
	adds r0, r1, r0
	adds r1, r1, r4
	ldr r2, [r5]
	subs r2, r2, r6
	ldr r3, _08009AEC @ =0x001FFFFF
	ands r2, r3
	movs r3, #0x80
	lsls r3, r3, #0x13
	orrs r2, r3
	bl sub_803A94C
	ldr r2, [r5]
	ldr r1, [r5, #8]
	lsls r0, r2, #2
	adds r0, r0, r1
	subs r0, #4
	movs r1, #0
	str r1, [r0]
	subs r2, #1
	str r2, [r5]
_08009AE6:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08009AEC: .4byte 0x001FFFFF

	thumb_func_start sub_8009AF0
sub_8009AF0: @ 0x08009AF0
	push {r4, r5, r6, r7, lr}
	adds r7, r0, #0
	ldr r0, _08009B38 @ =0x00000814
	adds r5, r7, r0
	ldr r4, [r5]
	ldr r6, [r4]
	ldr r0, [r4, #4]
	str r0, [r5]
	movs r0, #0
	str r0, [r4, #4]
	str r1, [r6]
	str r0, [r6, #4]
	str r3, [r6, #0xc]
	strb r0, [r6, #0x10]
	strb r0, [r6, #0x11]
	lsls r2, r2, #2
	adds r0, r7, #0
	adds r0, #0x10
	adds r1, r0, r2
	ldr r0, [r1]
	cmp r0, #0
	bne _08009B1E
	str r6, [r1]
_08009B1E:
	movs r1, #0x82
	lsls r1, r1, #3
	adds r0, r7, r1
	adds r0, r0, r2
	ldr r1, [r0]
	cmp r1, #0
	beq _08009B2E
	str r6, [r1, #4]
_08009B2E:
	str r6, [r0]
	adds r0, r6, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08009B38: .4byte 0x00000814

	thumb_func_start sub_8009B3C
sub_8009B3C: @ 0x08009B3C
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	adds r4, r1, #0
	movs r0, #2
	ldrsh r2, [r4, r0]
	adds r0, r6, #0
	movs r3, #0
	bl sub_8009AF0
	adds r5, r0, #0
	ldrb r1, [r4, #0xc]
	lsrs r0, r1, #4
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _08009B6A
	adds r0, r6, #0
	adds r1, r4, #0
	movs r2, #0xff
	adds r3, r5, #0
	bl sub_8009AF0
	str r0, [r5, #0xc]
_08009B6A:
	pop {r4, r5, r6}
	pop {r0}
	bx r0

	thumb_func_start sub_8009B70
sub_8009B70: @ 0x08009B70
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r1, [r4]
	ldr r0, [r4, #4]
	cmp r1, r0
	bge _08009B94
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_8009B3C
	ldr r0, [r4]
	ldr r2, [r4, #8]
	lsls r1, r0, #2
	adds r1, r1, r2
	str r5, [r1]
	adds r0, #1
	str r0, [r4]
_08009B94:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8009B9C
sub_8009B9C: @ 0x08009B9C
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	movs r1, #0x81
	lsls r1, r1, #4
	adds r0, r4, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08009BB2
	bl sub_8026EB4
_08009BB2:
	ldr r0, [r4, #0xc]
	cmp r0, #0
	beq _08009BBC
	bl sub_8026EB4
_08009BBC:
	ldr r0, [r4, #8]
	cmp r0, #0
	beq _08009BC6
	bl sub_8026EB4
_08009BC6:
	movs r0, #0
	str r0, [r4, #4]
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _08009BD8
	adds r0, r4, #0
	bl sub_8026ED0
_08009BD8:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8009BE0
sub_8009BE0: @ 0x08009BE0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	adds r5, r0, #0
	mov sb, r1
	ldrb r0, [r2, #4]
	mov sl, r0
	ldr r0, [r5, #4]
	str r0, [sp, #0xc]
	ldr r0, [r5]
	ldr r1, [r5, #4]
	str r0, [sp, #4]
	str r1, [sp, #8]
	add r0, sp, #4
	mov r1, sb
	bl sub_8008278
	ldr r0, [sp, #4]
	asrs r0, r0, #8
	str r0, [sp, #4]
	ldr r0, [sp, #8]
	asrs r0, r0, #8
	str r0, [sp, #8]
	adds r6, r5, #0
	adds r6, #0x69
	movs r0, #0
	strb r0, [r6]
	ldr r1, _08009C40 @ =gUnknown_03001308
	mov r8, r1
	ldr r0, [r1]
	add r4, sp, #0xc
	str r4, [sp]
	mov r1, sb
	add r2, sp, #4
	mov r3, sl
	bl sub_8026628
	lsls r0, r0, #0x18
	lsrs r1, r0, #0x18
	cmp r1, #0
	beq _08009C44
	ldr r0, [sp, #0xc]
	str r0, [r5, #4]
	movs r0, #1
	b _08009C8C
	.align 2, 0
_08009C40: .4byte gUnknown_03001308
_08009C44:
	mov r2, r8
	ldr r0, [r2]
	adds r0, #0x2a
	ldrb r7, [r0]
	strb r1, [r0]
	adds r5, r6, #0
	add r4, sp, #4
_08009C52:
	ldrb r0, [r5]
	adds r0, #1
	strb r0, [r5]
	ldr r0, [r4, #4]
	adds r0, #8
	str r0, [r4, #4]
	mov r1, r8
	ldr r0, [r1]
	add r2, sp, #0xc
	str r2, [sp]
	mov r1, sb
	add r2, sp, #4
	mov r3, sl
	bl sub_8026628
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08009C82
	ldrb r0, [r6]
	cmp r0, #2
	bls _08009C52
	mov r1, r8
	ldr r0, [r1]
	b _08009C86
_08009C82:
	ldr r0, _08009C9C @ =gUnknown_03001308
	ldr r0, [r0]
_08009C86:
	adds r0, #0x2a
	strb r7, [r0]
	movs r0, #0
_08009C8C:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08009C9C: .4byte gUnknown_03001308

	thumb_func_start sub_8009CA0
sub_8009CA0: @ 0x08009CA0
	push {r4, r5, lr}
	sub sp, #0x20
	adds r4, r0, #0
	ldrb r1, [r4, #0xc]
	lsrs r0, r1, #2
	movs r5, #1
	ands r0, r5
	cmp r0, #0
	beq _08009CDE
	ldr r0, _08009D1C @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r3, #0
	adds r0, #0x8c
	ldr r1, _08009D20 @ =gUnknown_0300082C
	ldr r2, [r0]
	ldr r0, [r1]
	cmp r2, r0
	bls _08009CC6
	movs r3, #1
_08009CC6:
	cmp r3, #0
	beq _08009CD4
	ldr r0, _08009D24 @ =gUnknown_030012C0
	ldr r0, [r0]
	ldr r0, [r0, #0x78]
	cmp r0, #3
	bne _08009CDE
_08009CD4:
	ldrb r1, [r4, #0xd]
	lsrs r0, r1, #3
	ands r0, r5
	cmp r0, #0
	beq _08009CF4
_08009CDE:
	ldrb r1, [r4, #0xd]
	lsrs r0, r1, #3
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _08009D4E
	ldr r0, _08009D24 @ =gUnknown_030012C0
	ldr r0, [r0]
	ldr r0, [r0, #0x78]
	cmp r0, #3
	bne _08009D4E
_08009CF4:
	mov r0, sp
	adds r1, r4, #0
	bl sub_8007C30
	ldr r0, [sp, #8]
	cmp r0, #0
	beq _08009D28
	ldr r0, _08009D1C @ =gUnknown_030012D8
	ldr r0, [r0]
	mov r1, sp
	bl sub_800B37C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08009D28
	adds r0, r4, #0
	bl sub_8009D5C
	b _08009D4E
	.align 2, 0
_08009D1C: .4byte gUnknown_030012D8
_08009D20: .4byte gUnknown_0300082C
_08009D24: .4byte gUnknown_030012C0
_08009D28:
	add r5, sp, #0x10
	adds r0, r5, #0
	adds r1, r4, #0
	bl sub_8007CF8
	ldr r0, [sp, #0x18]
	cmp r0, #0
	beq _08009D4E
	ldr r0, _08009D58 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_800B37C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08009D4E
	adds r0, r4, #0
	bl sub_8009D5C
_08009D4E:
	add sp, #0x20
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08009D58: .4byte gUnknown_030012D8

	thumb_func_start sub_8009D5C
sub_8009D5C: @ 0x08009D5C
	push {r4, r5, lr}
	adds r5, r0, #0
	movs r0, #8
	ldrb r1, [r5, #0xc]
	orrs r0, r1
	strb r0, [r5, #0xc]
	ldr r0, _08009D7C @ =gUnknown_030012C0
	ldr r0, [r0]
	ldr r0, [r0, #0x78]
	cmp r0, #2
	bgt _08009D80
	cmp r0, #1
	bge _08009DA0
	cmp r0, #0
	beq _08009D86
	b _08009DEE
	.align 2, 0
_08009D7C: .4byte gUnknown_030012C0
_08009D80:
	cmp r0, #3
	beq _08009DD8
	b _08009DEE
_08009D86:
	ldr r0, _08009D9C @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldrb r2, [r5, #0xa]
	ldr r4, [r1, #4]
	movs r1, #0
	b _08009DCA
	.align 2, 0
_08009D9C: .4byte gUnknown_030012D8
_08009DA0:
	ldr r0, _08009DD4 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldrb r2, [r5, #0xa]
	ldr r4, [r1, #4]
	movs r1, #0
	movs r3, #0
	bl sub_803AD88
	ldr r1, [r5, #0x18]
	adds r1, #0x68
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r4, [r1, #4]
	movs r1, #1
	movs r2, #1
_08009DCA:
	movs r3, #0
	bl sub_803AD88
	b _08009DEE
	.align 2, 0
_08009DD4: .4byte gUnknown_030012D8
_08009DD8:
	ldr r1, [r5, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r4, [r1, #4]
	movs r1, #1
	movs r2, #1
	movs r3, #0
	bl sub_803AD88
_08009DEE:
	pop {r4, r5}
	pop {r0}
	bx r0

