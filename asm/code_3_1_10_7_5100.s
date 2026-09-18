.include "asm/macros.inc"

.syntax unified
.arm

.if NON_MATCHING == 0
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
.endif
