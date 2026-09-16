.include "asm/macros.inc"

.syntax unified
.arm

@ sub_800891C is reconstructed (semantics well understood, but not yet
@ byte-matching) as C in src/graphics/actor_part7.c, guarded by
@ #if NON_MATCHING - see docs/matching.md for the exact remaining gap.
.if NON_MATCHING == 0
	thumb_func_start sub_800891C
sub_800891C: @ 0x0800891C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x20
	adds r5, r0, #0
	movs r0, #0xdc
	lsls r0, r0, #9
	movs r1, #0x8c
	lsls r1, r1, #9
	str r0, [sp, #8]
	str r1, [sp, #0xc]
	ldr r0, _080089D0 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r2, [r0, #0x10]
	ldr r1, [r2]
	lsls r1, r1, #8
	movs r3, #0
	ldr r0, _080089D4 @ =0xFFFF9C00
	adds r1, r1, r0
	ldr r0, [r2, #4]
	lsls r0, r0, #8
	ldr r4, _080089D8 @ =0xFFFFC400
	adds r0, r0, r4
	str r1, [sp]
	str r0, [sp, #4]
	ldr r1, [r2]
	lsls r1, r1, #8
	ldr r0, [r2, #4]
	lsls r0, r0, #8
	str r1, [sp, #0x10]
	str r0, [sp, #0x14]
	add r0, sp, #0x10
	movs r1, #0xf0
	lsls r1, r1, #8
	movs r2, #0xa0
	lsls r2, r2, #8
	str r1, [r0, #8]
	str r2, [r0, #0xc]
	str r3, [r5, #8]
	movs r7, #0
	ldr r2, [r5, #4]
	mov r8, r0
	cmp r7, r2
	bge _08008A32
_08008974:
	ldr r3, [r5, #0xc]
	lsls r1, r7, #2
	adds r6, r1, r3
	mov ip, r6
	ldr r4, [r6]
	movs r0, #1
	ldrb r6, [r4, #0xc]
	ands r0, r6
	cmp r0, #0
	beq _080089E0
	ldr r0, [r5]
	cmp r7, r0
	bge _080089B4
	adds r0, r1, #4
	adds r0, r3, r0
	subs r2, r2, r7
	ldr r1, _080089DC @ =0x001FFFFF
	ands r2, r1
	movs r1, #0x80
	lsls r1, r1, #0x13
	orrs r2, r1
	mov r1, ip
	bl sub_803A94C
	ldr r0, [r5, #4]
	subs r0, #1
	str r0, [r5, #4]
	ldr r1, [r5, #0xc]
	lsls r0, r0, #2
	adds r0, r0, r1
	movs r1, #0
	str r1, [r0]
_080089B4:
	cmp r4, #0
	beq _080089CA
	ldr r1, [r4, #0x18]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_080089CA:
	subs r7, #1
	b _08008A2A
	.align 2, 0
_080089D0: .4byte gUnknown_03001308
_080089D4: .4byte 0xFFFF9C00
_080089D8: .4byte 0xFFFFC400
_080089DC: .4byte 0x001FFFFF
_080089E0:
	ldr r1, [r4, #0x18]
	adds r1, #0x40
	movs r6, #0
	ldrsh r0, [r1, r6]
	adds r0, r4, r0
	ldr r2, [r1, #4]
	mov r1, sp
	bl sub_803AD80
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08008A2A
	ldr r1, [r4, #0x18]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #0x1c]
	bl sub_803AD7C
	ldr r1, [r4, #0x18]
	movs r6, #0x30
	ldrsh r0, [r1, r6]
	adds r0, r4, r0
	ldr r2, [r1, #0x34]
	mov r1, r8
	bl sub_803AD80
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08008A2A
	ldr r0, [r5, #8]
	ldr r2, [r5, #0x10]
	lsls r1, r0, #2
	adds r1, r1, r2
	str r4, [r1]
	adds r0, #1
	str r0, [r5, #8]
_08008A2A:
	adds r7, #1
	ldr r2, [r5, #4]
	cmp r7, r2
	blt _08008974
_08008A32:
	add sp, #0x20
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
.endif

@ sub_8008A40 is reconstructed (semantics fully understood, but not
@ yet byte-matching) as C in src/graphics/actor_part7.c, guarded by
@ #if NON_MATCHING - see docs/matching.md for the exact remaining gap.
.if NON_MATCHING == 0
	thumb_func_start sub_8008A40
sub_8008A40: @ 0x08008A40
	sub sp, #0xc
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	adds r5, r0, #0
	str r1, [sp, #0x30]
	str r2, [sp, #0x34]
	str r3, [sp, #0x38]
	ldr r7, [sp, #0x44]
	movs r6, #0
	b _08008AC8
_08008A54:
	ldr r1, [r5, #0x10]
	lsls r0, r6, #2
	adds r0, r0, r1
	ldr r4, [r0]
	ldr r1, [r4, #0x18]
	adds r1, #0x48
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	cmp r0, #4
	ble _08008AC6
	ldrb r1, [r4, #0xc]
	lsrs r0, r1, #2
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _08008AC6
	ldr r0, _08008AA4 @ =gUnknown_030012D8
	ldr r0, [r0]
	cmp r7, r0
	bne _08008AA8
	add r0, sp, #0xc
	add r1, sp, #0x30
	movs r2, #0x10
	bl sub_800014C
	str r4, [sp, #4]
	ldr r0, [sp, #0x18]
	str r0, [sp]
	ldr r1, [sp, #0xc]
	ldr r2, [sp, #0x10]
	ldr r3, [sp, #0x14]
	adds r0, r5, #0
	bl sub_8008AD8
	b _08008AC6
	.align 2, 0
_08008AA4: .4byte gUnknown_030012D8
_08008AA8:
	add r0, sp, #0xc
	add r1, sp, #0x30
	movs r2, #0x10
	bl sub_800014C
	str r4, [sp, #4]
	str r7, [sp, #8]
	ldr r0, [sp, #0x18]
	str r0, [sp]
	ldr r1, [sp, #0xc]
	ldr r2, [sp, #0x10]
	ldr r3, [sp, #0x14]
	adds r0, r5, #0
	bl sub_8008D80
_08008AC6:
	adds r6, #1
_08008AC8:
	ldr r0, [r5, #8]
	cmp r6, r0
	blt _08008A54
	add sp, #0x1c
	pop {r4, r5, r6, r7}
	pop {r3}
	add sp, #0xc
	bx r3
.endif

@ sub_8008AD8 is reconstructed (semantics fully understood, but not
@ yet byte-matching) as C in src/graphics/actor_part7.c, guarded by
@ #if NON_MATCHING - see docs/matching.md for the exact remaining gap.
.if NON_MATCHING == 0
	thumb_func_start sub_8008AD8
sub_8008AD8: @ 0x08008AD8
	sub sp, #0xc
	push {r4, r5, r6, lr}
	sub sp, #0x20
	str r1, [sp, #0x30]
	str r2, [sp, #0x34]
	str r3, [sp, #0x38]
	ldr r5, [sp, #0x40]
	ldr r4, _08008B14 @ =gUnknown_030012C0
	ldr r0, [r4]
	ldr r0, [r0, #0x78]
	cmp r0, #3
	bne _08008B1C
	adds r0, r5, #0
	add r1, sp, #0x30
	bl sub_8009FF4
	cmp r0, #0
	bne _08008AFE
	b _08008C72
_08008AFE:
	ldr r3, [r5, #0x18]
	adds r3, #0x68
	movs r1, #0
	ldrsh r0, [r3, r1]
	adds r0, r5, r0
	ldr r1, _08008B18 @ =gUnknown_030012D8
	ldr r1, [r1]
	ldrb r2, [r1, #0xa]
	ldr r4, [r3, #4]
	b _08008C28
	.align 2, 0
_08008B14: .4byte gUnknown_030012C0
_08008B18: .4byte gUnknown_030012D8
_08008B1C:
	ldrb r2, [r5, #0xd]
	lsrs r0, r2, #3
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _08008BA4
	ldr r6, _08008B7C @ =gUnknown_030012D8
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
	bne _08008B4C
	b _08008C72
_08008B4C:
	ldr r3, [r5]
	ldr r2, [r6]
	ldr r0, [r2]
	cmp r3, r0
	bge _08008B80
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
	b _08008C72
	.align 2, 0
_08008B7C: .4byte gUnknown_030012D8
_08008B80:
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
	b _08008C72
_08008BA4:
	adds r0, r5, #0
	add r1, sp, #0x30
	bl sub_8009FF4
	cmp r0, #1
	beq _08008BBA
	cmp r0, #1
	ble _08008C72
	cmp r0, #2
	beq _08008C32
	b _08008C72
_08008BBA:
	ldr r6, _08008C14 @ =gUnknown_030012D8
	ldr r1, [r6]
	movs r0, #8
	ldrb r2, [r1, #0xc]
	orrs r0, r2
	strb r0, [r1, #0xc]
	ldr r0, [r6]
	ldrb r2, [r0, #0xa]
	cmp r2, #1
	bne _08008C1C
	ldr r0, [r0, #0x64]
	cmp r0, #0
	ble _08008C72
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
	ldr r0, _08008C18 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x21
	bl PlaySfx
	b _08008C72
	.align 2, 0
_08008C14: .4byte gUnknown_030012D8
_08008C18: .4byte gUnknown_030012BC
_08008C1C:
	ldr r1, [r5, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r4, [r1, #4]
_08008C28:
	movs r1, #1
	movs r3, #0
	bl sub_803AD88
	b _08008C72
_08008C32:
	movs r0, #8
	ldrb r1, [r5, #0xc]
	orrs r0, r1
	strb r0, [r5, #0xc]
	ldr r0, [r4]
	ldr r0, [r0, #0x78]
	cmp r0, #0
	beq _08008C58
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
_08008C58:
	ldr r0, _08008C7C @ =gUnknown_030012D8
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
_08008C72:
	add sp, #0x20
	pop {r4, r5, r6}
	pop {r3}
	add sp, #0xc
	bx r3
	.align 2, 0
_08008C7C: .4byte gUnknown_030012D8
.endif

