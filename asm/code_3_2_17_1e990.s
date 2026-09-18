.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_801E990
sub_801E990: @ 0x0801E990
	push {r4, r5, r6, r7, lr}
	lsls r1, r1, #0x10
	lsrs r6, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r7, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r4, r3, #0x10
	ldr r5, _0801EA4C @ =gUnknown_030012C0
	ldr r0, [r5]
	bl sub_80232F4
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801E9E2
	ldr r0, _0801EA50 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r2, [r0]
	ldr r0, [r2, #8]
	lsls r1, r4, #1
	adds r1, r1, r0
	ldr r0, [r2, #0xc]
	ldrh r1, [r1]
	adds r0, r1, r0
	ldrb r0, [r0]
	lsrs r2, r0, #1
	movs r0, #1
	ldr r3, _0801EA54 @ =gUnknown_030012D8
	ldr r1, [r3]
	adds r1, #0x28
	ands r2, r0
	lsls r2, r2, #4
	subs r0, #0x12
	ldrb r4, [r1]
	ands r0, r4
	orrs r0, r2
	strb r0, [r1]
	ldr r1, [r3]
	lsls r0, r6, #8
	str r0, [r1]
	lsls r0, r7, #8
	str r0, [r1, #4]
_0801E9E2:
	ldr r1, [r5]
	adds r0, r1, #0
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0801EA46
	adds r0, r1, #0
	bl sub_80232E0
	adds r4, r0, #0
	ldr r0, [r5]
	bl sub_8023130
	cmp r4, r0
	bge _0801EA1E
	ldr r0, [r5]
	bl sub_803AFEC
	cmp r0, #0
	bne _0801EA46
	ldr r0, [r5]
	bl sub_80232B8
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801EA46
	ldr r0, [r5]
	ldr r0, [r0, #0x78]
	cmp r0, #0
	bne _0801EA46
_0801EA1E:
	ldr r0, _0801EA54 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0x1a
	movs r3, #0
	bl sub_803AD88
	ldr r0, _0801EA58 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #1
	bl PlaySfx
_0801EA46:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801EA4C: .4byte gUnknown_030012C0
_0801EA50: .4byte gUnknown_030012B4
_0801EA54: .4byte gUnknown_030012D8
_0801EA58: .4byte gUnknown_030012BC

	thumb_func_start sub_801EA5C
sub_801EA5C: @ 0x0801EA5C
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r0
	lsls r1, r1, #0x10
	lsrs r5, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r7, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r4, r3, #0x10
	ldr r0, _0801EAF8 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023404
	movs r6, #1
	ldrb r0, [r0]
	ands r6, r0
	cmp r6, #0
	bne _0801EAEC
	movs r0, #0x1b
	mov sb, r0
	mov r1, r8
	lsls r0, r1, #0x10
	lsrs r0, r0, #0x10
	adds r1, r5, #0
	adds r2, r7, #0
	adds r3, r4, #0
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _0801EAFC @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r3, #0xde
	lsls r3, r3, #1
	adds r0, r0, r3
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	strb r6, [r0]
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
	mov r0, sb
	strb r0, [r4, #0xa]
	ldr r0, _0801EB00 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
_0801EAEC:
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801EAF8: .4byte gUnknown_030012C0
_0801EAFC: .4byte gUnknown_030012D0
_0801EB00: .4byte gUnknown_030012EC

	thumb_func_start sub_801EB04
sub_801EB04: @ 0x0801EB04
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	adds r7, r0, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	mov sl, r1
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	mov sb, r2
	lsls r3, r3, #0x10
	lsrs r4, r3, #0x10
	ldr r0, _0801EBE0 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023404
	movs r1, #2
	ldrb r0, [r0]
	ands r1, r0
	lsls r1, r1, #0x18
	lsrs r6, r1, #0x18
	cmp r6, #0
	bne _0801EBD0
	movs r5, #1
	movs r0, #0x1d
	mov r8, r0
	lsls r0, r7, #0x10
	lsrs r0, r0, #0x10
	mov r1, sl
	mov r2, sb
	adds r3, r4, #0
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _0801EBE4 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	movs r7, #1
	strb r5, [r0]
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
	mov r0, r8
	strb r0, [r4, #0xa]
	ldr r0, _0801EBE8 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	ldr r0, _0801EBEC @ =gUnknown_030012E4
	ldr r0, [r0]
	mov r1, sb
	str r1, [sp]
	str r6, [sp, #4]
	movs r1, #0x2b
	movs r2, #2
	mov r3, sl
	bl sub_8025BAC
	adds r2, r0, #0
	adds r2, #0x28
	movs r1, #4
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r7
	strb r1, [r2]
	movs r1, #5
	rsbs r1, r1, #0
	ldrb r2, [r0, #0xc]
	ands r1, r2
	strb r1, [r0, #0xc]
_0801EBD0:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801EBE0: .4byte gUnknown_030012C0
_0801EBE4: .4byte gUnknown_030012D0
_0801EBE8: .4byte gUnknown_030012EC
_0801EBEC: .4byte gUnknown_030012E4

	thumb_func_start sub_801EBF0
sub_801EBF0: @ 0x0801EBF0
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	lsls r1, r1, #0x10
	lsrs r6, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r7, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r4, r3, #0x10
	ldr r0, _0801EC90 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023404
	movs r1, #4
	ldrb r0, [r0]
	ands r1, r0
	cmp r1, #0
	bne _0801EC84
	movs r0, #1
	mov r8, r0
	movs r1, #0x1e
	mov sb, r1
	lsls r0, r5, #0x10
	lsrs r0, r0, #0x10
	adds r1, r6, #0
	adds r2, r7, #0
	adds r3, r4, #0
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _0801EC94 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r3, #0xc0
	lsls r3, r3, #1
	adds r0, r0, r3
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r1, r8
	strb r1, [r0]
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
	mov r0, sb
	strb r0, [r4, #0xa]
	ldr r0, _0801EC98 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
_0801EC84:
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801EC90: .4byte gUnknown_030012C0
_0801EC94: .4byte gUnknown_030012D0
_0801EC98: .4byte gUnknown_030012EC

	thumb_func_start sub_801EC9C
sub_801EC9C: @ 0x0801EC9C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov r8, r0
	lsls r1, r1, #0x10
	lsrs r7, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r6, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r5, r3, #0x10
	ldr r4, _0801ED40 @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_80233B4
	cmp r0, #1
	beq _0801ED4C
	ldr r1, [r4]
	movs r0, #1
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	bne _0801ED5C
	movs r0, #3
	mov sb, r0
	movs r1, #0x1f
	mov sl, r1
	mov r3, r8
	lsls r0, r3, #0x10
	lsrs r0, r0, #0x10
	adds r1, r7, #0
	adds r2, r6, #0
	adds r3, r5, #0
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _0801ED44 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r3, sb
	strb r3, [r0]
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
	mov r0, sl
	strb r0, [r4, #0xa]
	ldr r0, _0801ED48 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	b _0801ED5C
	.align 2, 0
_0801ED40: .4byte gUnknown_030012C0
_0801ED44: .4byte gUnknown_030012D0
_0801ED48: .4byte gUnknown_030012EC
_0801ED4C:
	movs r0, #0
	str r0, [sp]
	mov r0, r8
	adds r1, r7, #0
	adds r2, r6, #0
	adds r3, r5, #0
	bl sub_8018D70
_0801ED5C:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_801ED6C
sub_801ED6C: @ 0x0801ED6C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov r8, r0
	lsls r1, r1, #0x10
	lsrs r7, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r6, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r5, r3, #0x10
	ldr r4, _0801EE10 @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_80233B4
	cmp r0, #1
	beq _0801EE1C
	ldr r1, [r4]
	movs r0, #4
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	bne _0801EE2A
	movs r0, #2
	mov sb, r0
	movs r1, #0x20
	mov sl, r1
	mov r3, r8
	lsls r0, r3, #0x10
	lsrs r0, r0, #0x10
	adds r1, r7, #0
	adds r2, r6, #0
	adds r3, r5, #0
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _0801EE14 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r3, sb
	strb r3, [r0]
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
	mov r0, sl
	strb r0, [r4, #0xa]
	ldr r0, _0801EE18 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	b _0801EE2A
	.align 2, 0
_0801EE10: .4byte gUnknown_030012C0
_0801EE14: .4byte gUnknown_030012D0
_0801EE18: .4byte gUnknown_030012EC
_0801EE1C:
	str r0, [sp]
	mov r0, r8
	adds r1, r7, #0
	adds r2, r6, #0
	adds r3, r5, #0
	bl sub_8018D70
_0801EE2A:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801EE3C
sub_801EE3C: @ 0x0801EE3C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov sb, r0
	lsls r1, r1, #0x10
	lsrs r7, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r5, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	ldr r4, _0801EEE0 @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_80233B4
	cmp r0, #1
	beq _0801EEEC
	ldr r1, [r4]
	movs r0, #2
	ldrb r1, [r1, #2]
	ands r0, r1
	lsls r0, r0, #0x18
	lsrs r6, r0, #0x18
	cmp r6, #0
	bne _0801EEFC
	movs r0, #0x22
	mov sl, r0
	mov r1, sb
	lsls r0, r1, #0x10
	lsrs r0, r0, #0x10
	adds r1, r7, #0
	adds r2, r5, #0
	mov r3, r8
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _0801EEE4 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r3, #0xc0
	lsls r3, r3, #1
	adds r0, r0, r3
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	strb r6, [r0]
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
	mov r0, sl
	strb r0, [r4, #0xa]
	ldr r0, _0801EEE8 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	b _0801EEFC
	.align 2, 0
_0801EEE0: .4byte gUnknown_030012C0
_0801EEE4: .4byte gUnknown_030012D0
_0801EEE8: .4byte gUnknown_030012EC
_0801EEEC:
	movs r0, #2
	str r0, [sp]
	mov r0, sb
	adds r1, r7, #0
	adds r2, r5, #0
	mov r3, r8
	bl sub_8018D70
_0801EEFC:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_801EF0C
sub_801EF0C: @ 0x0801EF0C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _0801F040 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x9c
	str r0, [r4, #0x20]
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r7, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #0xd
	str r0, [r6, #0x6c]
	str r6, [r4, #0x44]
	ldr r1, [r6, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #1
	movs r7, #0
	mov sl, r7
	movs r5, #1
	strb r0, [r4, #0xa]
	movs r0, #0x7f
	ldrb r1, [r4, #0xc]
	ands r0, r1
	strb r0, [r4, #0xc]
	ldr r2, _0801F044 @ =gUnknown_030012B4
	mov sb, r2
	ldr r0, [r2]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r3, r8
	lsls r3, r3, #1
	mov r8, r3
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r7, [r2]
	lsrs r0, r7, #1
	eors r0, r5
	ands r0, r5
	adds r3, r4, #0
	adds r3, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r5
	ands r0, r5
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0801F048 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	ldr r1, _0801F04C @ =gStaticData_0816B98C
	adds r0, r6, #0
	adds r0, #0x84
	str r1, [r0]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r5, [r1, #0xc]
	mov r2, r8
	ldrh r2, [r2]
	adds r5, r2, r5
	adds r0, r4, #0
	adds r0, #0x2d
	mov r3, sl
	strb r3, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r6, #0
	movs r1, #2
	bl sub_800C6A8
	ldr r1, [r5, #4]
	adds r0, r6, #0
	bl sub_800C898
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801F040: .4byte gUnknown_030012D0
_0801F044: .4byte gUnknown_030012B4
_0801F048: .4byte gUnknown_030012F0
_0801F04C: .4byte gStaticData_0816B98C

	thumb_func_start sub_801F050
sub_801F050: @ 0x0801F050
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	adds r5, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r5, #0
	bl sub_8009ED0
	mov r8, r0
	ldr r0, _0801F15C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x84
	mov r1, r8
	str r0, [r1, #0x20]
	mov r0, r8
	bl sub_800815C
	mov r2, r8
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r4, r0, #0
	ldr r1, [r4, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x1c]
	mov r1, r8
	bl sub_803AD80
	movs r0, #0xb
	str r0, [r4, #0x6c]
	mov r3, r8
	str r4, [r3, #0x44]
	ldr r1, [r4, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x1c]
	mov r1, r8
	bl sub_803AD80
	movs r0, #1
	movs r3, #0
	mov sb, r3
	movs r6, #1
	mov r1, r8
	strb r0, [r1, #0xa]
	movs r0, #0x7f
	ldrb r2, [r1, #0xc]
	ands r0, r2
	strb r0, [r1, #0xc]
	ldr r0, _0801F160 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r5, r5, #1
	adds r5, r5, r0
	ldr r2, [r1, #0xc]
	ldrh r5, [r5]
	adds r2, r5, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r6
	ands r0, r6
	mov r3, r8
	adds r3, #0x28
	ands r0, r6
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r5, [r3]
	ands r1, r5
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r6
	ands r0, r6
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0801F164 @ =gUnknown_030012F0
	ldr r0, [r0]
	mov r1, r8
	bl sub_8008E94
	ldr r0, _0801F168 @ =gStaticData_0816B98C
	adds r5, r4, #0
	adds r5, #0x84
	str r0, [r5]
	adds r0, r4, #0
	movs r1, #0x11
	bl sub_800C6A8
	ldr r0, _0801F16C @ =gStaticData_0816BA2C
	str r0, [r5]
	movs r0, #0x64
	movs r1, #0x32
	mov r2, sb
	str r2, [r4, #0x20]
	str r0, [r4, #0x28]
	str r2, [r4, #0x24]
	str r1, [r4, #0x2c]
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801F15C: .4byte gUnknown_030012D0
_0801F160: .4byte gUnknown_030012B4
_0801F164: .4byte gUnknown_030012F0
_0801F168: .4byte gStaticData_0816B98C
_0801F16C: .4byte gStaticData_0816BA2C

	thumb_func_start sub_801F170
sub_801F170: @ 0x0801F170
	push {r4, r5, r6, lr}
	mov r6, sl
	mov r5, sb
	mov r4, r8
	push {r4, r5, r6}
	adds r5, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r5, #0
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _0801F2A8 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x78
	str r0, [r4, #0x20]
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
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #0xa
	str r0, [r6, #0x6c]
	str r6, [r4, #0x44]
	ldr r1, [r6, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #1
	mov sb, r0
	movs r1, #0
	mov sl, r1
	movs r2, #1
	mov r8, r2
	mov r3, sb
	strb r3, [r4, #0xa]
	movs r0, #0x7f
	ldrb r1, [r4, #0xc]
	ands r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _0801F2AC @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r5, r5, #1
	adds r5, r5, r0
	ldr r2, [r1, #0xc]
	ldrh r5, [r5]
	adds r2, r5, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	mov r5, r8
	eors r0, r5
	ands r0, r5
	adds r3, r4, #0
	adds r3, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r5, [r3]
	ands r1, r5
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	mov r2, r8
	ands r0, r2
	ands r0, r2
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0801F2B0 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	ldr r0, _0801F2B4 @ =gStaticData_0816B98C
	adds r5, r6, #0
	adds r5, #0x84
	str r0, [r5]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r3, sb
	strb r3, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	movs r0, #6
	strb r0, [r4, #0xa]
	adds r0, r6, #0
	movs r1, #3
	bl sub_800C6A8
	ldr r0, _0801F2B8 @ =gStaticData_0816BA0C
	str r0, [r5]
	movs r2, #0x14
	rsbs r2, r2, #0
	movs r0, #0x2d
	movs r1, #0x14
	mov r5, sl
	str r5, [r6, #0x20]
	str r0, [r6, #0x28]
	str r2, [r6, #0x24]
	str r1, [r6, #0x2c]
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801F2A8: .4byte gUnknown_030012D0
_0801F2AC: .4byte gUnknown_030012B4
_0801F2B0: .4byte gUnknown_030012F0
_0801F2B4: .4byte gStaticData_0816B98C
_0801F2B8: .4byte gStaticData_0816BA0C

	thumb_func_start sub_801F2BC
sub_801F2BC: @ 0x0801F2BC
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _0801F3CC @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0xa8
	str r0, [r4, #0x20]
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r7, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #0xe
	str r0, [r6, #0x6c]
	str r6, [r4, #0x44]
	ldr r1, [r6, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #1
	movs r5, #1
	strb r0, [r4, #0xa]
	movs r0, #0x7f
	ldrb r7, [r4, #0xc]
	ands r0, r7
	strb r0, [r4, #0xc]
	ldr r0, _0801F3D0 @ =gUnknown_030012B4
	mov sb, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r5
	ands r0, r5
	adds r3, r4, #0
	adds r3, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r5
	ands r0, r5
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0801F3D4 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	ldr r1, _0801F3D8 @ =gStaticData_0816B98C
	adds r0, r6, #0
	adds r0, #0x84
	str r1, [r0]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r4, [r1, #0xc]
	mov r2, r8
	ldrh r2, [r2]
	adds r4, r2, r4
	adds r0, r6, #0
	movs r1, #2
	bl sub_800C6A8
	ldr r1, [r4, #4]
	adds r0, r6, #0
	bl sub_800C898
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801F3CC: .4byte gUnknown_030012D0
_0801F3D0: .4byte gUnknown_030012B4
_0801F3D4: .4byte gUnknown_030012F0
_0801F3D8: .4byte gStaticData_0816B98C

	thumb_func_start sub_801F3DC
sub_801F3DC: @ 0x0801F3DC
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r6, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r6, r6, #0x10
	lsrs r6, r6, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r6, #0
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _0801F514 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x90
	str r0, [r4, #0x20]
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
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r7, r0, #0
	ldr r1, [r7, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r7, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #0xc
	str r0, [r7, #0x6c]
	str r7, [r4, #0x44]
	ldr r1, [r7, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r7, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #1
	movs r5, #1
	strb r0, [r4, #0xa]
	movs r0, #0x7f
	ldrb r1, [r4, #0xc]
	ands r0, r1
	strb r0, [r4, #0xc]
	ldr r2, _0801F518 @ =gUnknown_030012B4
	mov r8, r2
	ldr r0, [r2]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r6, r6, #1
	mov sb, r6
	add r0, sb
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r5
	ands r0, r5
	adds r3, r4, #0
	adds r3, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r6, [r3]
	ands r1, r6
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r5
	ands r0, r5
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0801F51C @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	ldr r0, _0801F520 @ =gStaticData_0816B98C
	adds r2, r7, #0
	adds r2, #0x84
	str r0, [r2]
	mov r1, r8
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r3, sb
	adds r6, r3, r0
	ldr r0, [r1, #0xc]
	ldrh r6, [r6]
	adds r0, r6, r0
	adds r3, r0, #0
	ldr r0, _0801F524 @ =gStaticData_0816B9EC
	str r0, [r2]
	ldr r0, [r3, #4]
	ldr r1, [r3, #8]
	ldr r2, [r3, #0xc]
	str r0, [r7, #0x30]
	str r1, [r7, #0x34]
	str r2, [r7, #0x38]
	ldr r0, [r3, #4]
	ldr r1, [r3, #8]
	adds r0, r0, r1
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r2, r0, #1
	adds r1, r2, #0
	cmp r2, #0
	bge _0801F4F4
	adds r1, r2, #3
_0801F4F4:
	asrs r1, r1, #2
	ldr r0, [r3, #0xc]
	adds r0, r0, r1
	str r2, [r7, #0x48]
	str r0, [r7, #0x4c]
	adds r0, r7, #0
	movs r1, #0x10
	bl sub_800C6A8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801F514: .4byte gUnknown_030012D0
_0801F518: .4byte gUnknown_030012B4
_0801F51C: .4byte gUnknown_030012F0
_0801F520: .4byte gStaticData_0816B98C
_0801F524: .4byte gStaticData_0816B9EC

	thumb_func_start sub_801F528
sub_801F528: @ 0x0801F528
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r5, r0, #0
	ldr r0, _0801F66C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0xb4
	str r0, [r5, #0x20]
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r7, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #0xf
	str r0, [r6, #0x6c]
	str r6, [r5, #0x44]
	ldr r1, [r6, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #1
	movs r4, #1
	strb r0, [r5, #0xa]
	movs r0, #0x7f
	ldrb r7, [r5, #0xc]
	ands r0, r7
	strb r0, [r5, #0xc]
	ldr r0, _0801F670 @ =gUnknown_030012B4
	mov sl, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r4
	ands r0, r4
	adds r3, r5, #0
	adds r3, #0x28
	ands r0, r4
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r4
	ands r0, r4
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0801F674 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	ldr r0, _0801F678 @ =gStaticData_0816B98C
	movs r1, #0x84
	adds r1, r1, r6
	mov sb, r1
	str r0, [r1]
	mov r2, sl
	ldr r0, [r2]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r4, [r1, #0xc]
	mov r3, r8
	ldrh r3, [r3]
	adds r4, r3, r4
	adds r0, r5, #0
	adds r0, #0x2d
	movs r7, #0
	strb r7, [r0]
	adds r0, r5, #0
	bl sub_80087C0
	adds r0, r5, #0
	bl sub_80087B4
	adds r0, r5, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, _0801F67C @ =gStaticData_0816B9AC
	mov r1, sb
	str r0, [r1]
	ldr r0, [r4, #0xc]
	ldr r1, [r4, #8]
	ldr r2, [r4, #0x10]
	str r0, [r6, #0x30]
	str r1, [r6, #0x34]
	str r2, [r6, #0x38]
	ldr r1, [r4, #4]
	adds r0, r6, #0
	bl sub_800C898
	adds r0, r6, #0
	movs r1, #0xd
	bl sub_800C6A8
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801F66C: .4byte gUnknown_030012D0
_0801F670: .4byte gUnknown_030012B4
_0801F674: .4byte gUnknown_030012F0
_0801F678: .4byte gStaticData_0816B98C
_0801F67C: .4byte gStaticData_0816B9AC

	thumb_func_start sub_801F680
sub_801F680: @ 0x0801F680
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r6, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r6, r6, #0x10
	lsrs r6, r6, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r6, #0
	bl sub_800A604
	adds r4, r0, #0
	ldr r0, _0801F7A4 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0xcc
	str r0, [r4, #0x20]
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
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	mov r8, r0
	ldr r1, [r0, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	add r0, r8
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #0x11
	mov r3, r8
	str r0, [r3, #0x6c]
	movs r0, #0
	adds r1, r4, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	mov r0, r8
	str r0, [r4, #0x44]
	ldr r1, [r0, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	add r0, r8
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #1
	movs r5, #1
	strb r0, [r4, #0xa]
	movs r0, #0x7f
	ldrb r3, [r4, #0xc]
	ands r0, r3
	strb r0, [r4, #0xc]
	ldr r0, _0801F7A8 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r6, r6, #1
	adds r6, r6, r0
	ldr r2, [r1, #0xc]
	ldrh r6, [r6]
	adds r2, r6, r2
	ldrb r6, [r2]
	lsrs r0, r6, #1
	eors r0, r5
	ands r0, r5
	adds r3, r4, #0
	adds r3, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r6, [r3]
	ands r1, r6
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r5
	ands r0, r5
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	movs r0, #0x10
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _0801F7AC @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	ldr r1, _0801F7B0 @ =gStaticData_0816B98C
	mov r0, r8
	adds r0, #0x84
	str r1, [r0]
	mov r0, r8
	movs r1, #5
	bl sub_800C6A8
	ldr r0, _0801F7B4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x27
	bl PlaySfx
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801F7A4: .4byte gUnknown_030012D0
_0801F7A8: .4byte gUnknown_030012B4
_0801F7AC: .4byte gUnknown_030012F0
_0801F7B0: .4byte gStaticData_0816B98C
_0801F7B4: .4byte gUnknown_030012BC

	thumb_func_start sub_801F7B8
sub_801F7B8: @ 0x0801F7B8
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r5, r0, #0
	ldr r0, _0801F8CC @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0xc0
	str r0, [r5, #0x20]
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r7, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #0x10
	str r0, [r6, #0x6c]
	str r6, [r5, #0x44]
	ldr r1, [r6, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #1
	movs r4, #1
	strb r0, [r5, #0xa]
	movs r0, #0x7f
	ldrb r7, [r5, #0xc]
	ands r0, r7
	strb r0, [r5, #0xc]
	ldr r0, _0801F8D0 @ =gUnknown_030012B4
	mov sb, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r4
	ands r0, r4
	adds r3, r5, #0
	adds r3, #0x28
	ands r0, r4
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r4
	ands r0, r4
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0801F8D4 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	ldr r1, _0801F8D8 @ =gStaticData_0816B98C
	adds r0, r6, #0
	adds r0, #0x84
	str r1, [r0]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r4, [r1, #0xc]
	mov r2, r8
	ldrh r2, [r2]
	adds r4, r2, r4
	movs r0, #6
	strb r0, [r5, #0xa]
	adds r0, r6, #0
	movs r1, #2
	bl sub_800C6A8
	ldr r1, [r4, #4]
	adds r0, r6, #0
	bl sub_800C898
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801F8CC: .4byte gUnknown_030012D0
_0801F8D0: .4byte gUnknown_030012B4
_0801F8D4: .4byte gUnknown_030012F0
_0801F8D8: .4byte gStaticData_0816B98C

	thumb_func_start sub_801F8DC
sub_801F8DC: @ 0x0801F8DC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r5, r0, #0
	ldr r0, _0801FA28 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x3c
	str r0, [r5, #0x20]
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r7, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r3, #5
	str r3, [r6, #0x6c]
	str r6, [r5, #0x44]
	ldr r1, [r6, #0xc]
	movs r7, #0x18
	ldrsh r0, [r1, r7]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #1
	movs r4, #1
	strb r0, [r5, #0xa]
	movs r0, #0x7f
	ldrb r1, [r5, #0xc]
	ands r0, r1
	strb r0, [r5, #0xc]
	ldr r2, _0801FA2C @ =gUnknown_030012B4
	mov sl, r2
	ldr r0, [r2]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r3, r8
	lsls r3, r3, #1
	mov r8, r3
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r7, [r2]
	lsrs r0, r7, #1
	eors r0, r4
	ands r0, r4
	adds r3, r5, #0
	adds r3, #0x28
	ands r0, r4
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r4
	ands r0, r4
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0801FA30 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	ldr r0, _0801FA34 @ =gStaticData_0816B98C
	movs r1, #0x84
	adds r1, r1, r6
	mov sb, r1
	str r0, [r1]
	mov r2, sl
	ldr r0, [r2]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r4, [r1, #0xc]
	mov r3, r8
	ldrh r3, [r3]
	adds r4, r3, r4
	movs r0, #2
	adds r1, r5, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r5, #0
	bl sub_80087C0
	adds r0, r5, #0
	bl sub_80087B4
	adds r0, r5, #0
	movs r1, #0
	bl sub_800872C
	movs r7, #5
	strb r7, [r5, #0xa]
	ldr r0, _0801FA38 @ =gStaticData_0816B9CC
	mov r1, sb
	str r0, [r1]
	ldr r0, [r4, #4]
	ldr r1, [r4, #8]
	ldr r2, [r4, #0xc]
	str r0, [r6, #0x30]
	str r1, [r6, #0x34]
	str r2, [r6, #0x38]
	ldr r0, [r4, #0x14]
	ldr r1, [r4, #0x18]
	ldr r2, [r4, #0x10]
	str r0, [r6, #0x3c]
	str r1, [r6, #0x40]
	str r2, [r6, #0x44]
	adds r0, r6, #0
	movs r1, #0xe
	bl sub_800C6A8
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801FA28: .4byte gUnknown_030012D0
_0801FA2C: .4byte gUnknown_030012B4
_0801FA30: .4byte gUnknown_030012F0
_0801FA34: .4byte gStaticData_0816B98C
_0801FA38: .4byte gStaticData_0816B9CC

	thumb_func_start sub_801FA3C
sub_801FA3C: @ 0x0801FA3C
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r5, r0, #0
	ldr r0, _0801FB60 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x30
	str r0, [r5, #0x20]
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r7, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #4
	str r0, [r6, #0x6c]
	str r6, [r5, #0x44]
	ldr r1, [r6, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #1
	movs r4, #1
	strb r0, [r5, #0xa]
	movs r0, #0x7f
	ldrb r7, [r5, #0xc]
	ands r0, r7
	strb r0, [r5, #0xc]
	ldr r0, _0801FB64 @ =gUnknown_030012B4
	mov sb, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r4
	ands r0, r4
	adds r3, r5, #0
	adds r3, #0x28
	ands r0, r4
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r4
	ands r0, r4
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0801FB68 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	ldr r0, _0801FB6C @ =gStaticData_0816B98C
	adds r2, r6, #0
	adds r2, #0x84
	str r0, [r2]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r4, [r1, #0xc]
	mov r3, r8
	ldrh r3, [r3]
	adds r4, r3, r4
	movs r0, #6
	strb r0, [r5, #0xa]
	ldr r0, _0801FB70 @ =gStaticData_0816BA4C
	str r0, [r2]
	adds r0, r6, #0
	movs r1, #0xf
	bl sub_800C6A8
	ldr r1, [r4, #4]
	adds r0, r6, #0
	bl sub_800C898
	ldr r1, [r4, #8]
	ldr r2, [r4, #0xc]
	ldr r3, [r4, #0x10]
	adds r0, r6, #0
	bl sub_800C87C
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801FB60: .4byte gUnknown_030012D0
_0801FB64: .4byte gUnknown_030012B4
_0801FB68: .4byte gUnknown_030012F0
_0801FB6C: .4byte gStaticData_0816B98C
_0801FB70: .4byte gStaticData_0816BA4C

	thumb_func_start sub_801FB74
sub_801FB74: @ 0x0801FB74
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r4, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r4, #0
	bl sub_8009ED0
	adds r6, r0, #0
	ldr r0, _0801FCA4 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x24
	str r0, [r6, #0x20]
	adds r0, r6, #0
	bl sub_800815C
	adds r2, r6, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r7, r0, #0
	ldr r1, [r7, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r7, r0
	ldr r2, [r1, #0x1c]
	adds r1, r6, #0
	bl sub_803AD80
	movs r0, #3
	str r0, [r7, #0x6c]
	str r7, [r6, #0x44]
	ldr r1, [r7, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r7, r0
	ldr r2, [r1, #0x1c]
	adds r1, r6, #0
	bl sub_803AD80
	movs r0, #1
	movs r1, #0
	mov sb, r1
	movs r5, #1
	strb r0, [r6, #0xa]
	movs r0, #0x7f
	ldrb r2, [r6, #0xc]
	ands r0, r2
	strb r0, [r6, #0xc]
	ldr r0, _0801FCA8 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r4, r4, #1
	adds r4, r4, r0
	ldr r2, [r1, #0xc]
	ldrh r4, [r4]
	adds r2, r4, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r5
	ands r0, r5
	adds r4, r6, #0
	adds r4, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	mov r8, r1
	ldrb r3, [r4]
	ands r1, r3
	orrs r1, r0
	strb r1, [r4]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r5
	ands r0, r5
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r4]
	ldr r0, _0801FCAC @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r6, #0
	bl sub_8008E94
	ldr r1, _0801FCB0 @ =gStaticData_0816B98C
	adds r0, r7, #0
	adds r0, #0x84
	str r1, [r0]
	adds r0, r6, #0
	adds r0, #0x2d
	mov r1, sb
	strb r1, [r0]
	adds r0, r6, #0
	bl sub_80087C0
	adds r0, r6, #0
	bl sub_80087B4
	adds r0, r6, #0
	movs r1, #0
	bl sub_800872C
	ldrb r2, [r4]
	lsls r0, r2, #0x1b
	movs r1, #0
	cmp r0, #0
	blt _0801FC7E
	movs r1, #1
_0801FC7E:
	adds r0, r5, #0
	ands r0, r1
	lsls r0, r0, #4
	mov r1, r8
	ands r1, r2
	orrs r1, r0
	strb r1, [r4]
	movs r0, #6
	strb r0, [r6, #0xa]
	adds r0, r7, #0
	movs r1, #1
	bl sub_800C6A8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801FCA4: .4byte gUnknown_030012D0
_0801FCA8: .4byte gUnknown_030012B4
_0801FCAC: .4byte gUnknown_030012F0
_0801FCB0: .4byte gStaticData_0816B98C

	thumb_func_start sub_801FCB4
sub_801FCB4: @ 0x0801FCB4
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r6, r0, #0
	ldr r0, _0801FDD8 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x60
	str r0, [r6, #0x20]
	adds r0, r6, #0
	bl sub_800815C
	adds r2, r6, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r7, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r4, r0, #0
	ldr r1, [r4, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x1c]
	adds r1, r6, #0
	bl sub_803AD80
	movs r0, #8
	str r0, [r4, #0x6c]
	str r4, [r6, #0x44]
	ldr r1, [r4, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r2, [r1, #0x1c]
	adds r1, r6, #0
	bl sub_803AD80
	movs r0, #1
	movs r5, #1
	strb r0, [r6, #0xa]
	movs r0, #0x7f
	ldrb r7, [r6, #0xc]
	ands r0, r7
	strb r0, [r6, #0xc]
	ldr r0, _0801FDDC @ =gUnknown_030012B4
	mov sb, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r5
	ands r0, r5
	adds r3, r6, #0
	adds r3, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r5
	ands r0, r5
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0801FDE0 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r6, #0
	bl sub_8008E94
	ldr r0, _0801FDE4 @ =gStaticData_0816B98C
	adds r2, r4, #0
	adds r2, #0x84
	str r0, [r2]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r1, [r1, #0xc]
	mov r3, r8
	ldrh r3, [r3]
	adds r1, r3, r1
	movs r0, #3
	strb r0, [r6, #0xa]
	ldr r0, _0801FDE8 @ =gStaticData_0816BA8C
	str r0, [r2]
	ldr r0, [r1, #8]
	ldr r2, [r1, #0xc]
	ldr r3, [r1, #0x10]
	str r0, [r4, #0x30]
	str r2, [r4, #0x34]
	str r3, [r4, #0x38]
	ldr r1, [r1, #4]
	adds r0, r4, #0
	bl sub_800C898
	adds r0, r4, #0
	movs r1, #0xd
	bl sub_800C6A8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801FDD8: .4byte gUnknown_030012D0
_0801FDDC: .4byte gUnknown_030012B4
_0801FDE0: .4byte gUnknown_030012F0
_0801FDE4: .4byte gStaticData_0816B98C
_0801FDE8: .4byte gStaticData_0816BA8C

