.include "asm/macros.inc"

.syntax unified
.arm

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
