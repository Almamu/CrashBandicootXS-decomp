.include "asm/macros.inc"

.syntax unified
.arm

.if NON_MATCHING == 0
	thumb_func_start sub_8005EF4
sub_8005EF4: @ 0x08005EF4
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	ldr r0, [r6, #0x18]
	ldr r1, [r6, #0x14]
	lsls r0, r0, #3
	adds r0, r0, r1
	ldr r0, [r0, #4]
	cmp r0, #4
	beq _08005F0C
	cmp r0, #5
	beq _08005F5C
	b _08005FB0
_08005F0C:
	ldr r1, [r6, #0x60]
	cmp r1, #0
	beq _08005FB0
	subs r1, #1
	str r1, [r6, #0x60]
	adds r4, r6, #0
	adds r4, #0x57
	lsls r0, r1, #2
	adds r0, r0, r1
	movs r1, #0x20
	strb r1, [r4]
	movs r1, #0x3c
	strb r1, [r4, #1]
	adds r1, r6, #0
	adds r1, #0x59
	bl sub_80060AC
	adds r0, r0, r4
	movs r1, #0x25
	strb r1, [r0, #2]
	movs r1, #0x3e
	strb r1, [r0, #3]
	movs r1, #0
	strb r1, [r0, #4]
	ldr r0, _08005F58 @ =gUnknown_030012BC
	ldr r4, [r0]
	ldr r0, [r6, #0x60]
	lsls r0, r0, #8
	adds r0, #1
	movs r1, #0x14
	bl sub_803ADB4
	adds r1, r0, #0
	adds r0, r4, #0
	bl sub_8001B30
	b _08005FB0
	.align 2, 0
_08005F58: .4byte gUnknown_030012BC
_08005F5C:
	ldr r1, [r6, #0x64]
	cmp r1, #0
	beq _08005FB0
	subs r1, #1
	str r1, [r6, #0x64]
	adds r4, r6, #0
	adds r4, #0x4f
	lsls r0, r1, #2
	adds r0, r0, r1
	movs r1, #0x20
	strb r1, [r4]
	movs r1, #0x3c
	strb r1, [r4, #1]
	adds r1, r6, #0
	adds r1, #0x51
	bl sub_80060AC
	adds r0, r0, r4
	movs r1, #0x25
	strb r1, [r0, #2]
	movs r1, #0x3e
	strb r1, [r0, #3]
	movs r1, #0
	strb r1, [r0, #4]
	ldr r4, _08005FB8 @ =gUnknown_030012BC
	ldr r5, [r4]
	ldr r0, [r6, #0x64]
	lsls r0, r0, #8
	adds r0, #1
	movs r1, #0x14
	bl sub_803ADB4
	adds r1, r0, #0
	adds r0, r5, #0
	bl sub_8001B50
	ldr r0, [r4]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xe
	bl PlaySfx
_08005FB0:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08005FB8: .4byte gUnknown_030012BC
.endif

.if NON_MATCHING == 0
	thumb_func_start sub_8005FBC
sub_8005FBC: @ 0x08005FBC
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	ldr r0, [r6, #0x18]
	ldr r1, [r6, #0x14]
	lsls r0, r0, #3
	adds r0, r0, r1
	ldr r0, [r0, #4]
	cmp r0, #4
	beq _08005FD4
	cmp r0, #5
	beq _08006024
	b _08006078
_08005FD4:
	ldr r1, [r6, #0x60]
	cmp r1, #0x13
	bgt _08006078
	adds r1, #1
	str r1, [r6, #0x60]
	adds r4, r6, #0
	adds r4, #0x57
	lsls r0, r1, #2
	adds r0, r0, r1
	movs r1, #0x20
	strb r1, [r4]
	movs r1, #0x3c
	strb r1, [r4, #1]
	adds r1, r6, #0
	adds r1, #0x59
	bl sub_80060AC
	adds r0, r0, r4
	movs r1, #0x25
	strb r1, [r0, #2]
	movs r1, #0x3e
	strb r1, [r0, #3]
	movs r1, #0
	strb r1, [r0, #4]
	ldr r0, _08006020 @ =gUnknown_030012BC
	ldr r4, [r0]
	ldr r0, [r6, #0x60]
	lsls r0, r0, #8
	adds r0, #1
	movs r1, #0x14
	bl sub_803ADB4
	adds r1, r0, #0
	adds r0, r4, #0
	bl sub_8001B30
	b _08006078
	.align 2, 0
_08006020: .4byte gUnknown_030012BC
_08006024:
	ldr r1, [r6, #0x64]
	cmp r1, #0x13
	bgt _08006078
	adds r1, #1
	str r1, [r6, #0x64]
	adds r4, r6, #0
	adds r4, #0x4f
	lsls r0, r1, #2
	adds r0, r0, r1
	movs r1, #0x20
	strb r1, [r4]
	movs r1, #0x3c
	strb r1, [r4, #1]
	adds r1, r6, #0
	adds r1, #0x51
	bl sub_80060AC
	adds r0, r0, r4
	movs r1, #0x25
	strb r1, [r0, #2]
	movs r1, #0x3e
	strb r1, [r0, #3]
	movs r1, #0
	strb r1, [r0, #4]
	ldr r4, _08006080 @ =gUnknown_030012BC
	ldr r5, [r4]
	ldr r0, [r6, #0x64]
	lsls r0, r0, #8
	adds r0, #1
	movs r1, #0x14
	bl sub_803ADB4
	adds r1, r0, #0
	adds r0, r5, #0
	bl sub_8001B50
	ldr r0, [r4]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xe
	bl PlaySfx
_08006078:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08006080: .4byte gUnknown_030012BC
.endif
