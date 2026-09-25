.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_800CEAC
sub_800CEAC: @ 0x0800CEAC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x10
	adds r6, r1, #0
	mov r8, r2
	adds r7, r3, #0
	ldr r0, _0800CEF0 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x90
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800CEF4
	movs r0, #0
	ldrsh r1, [r6, r0]
	movs r0, #2
	ldrsh r2, [r6, r0]
	ldrb r5, [r6, #5]
	adds r1, r1, r7
	subs r1, #2
	ldr r0, [sp, #0x28]
	adds r2, r2, r0
	ldrb r4, [r6, #4]
	adds r4, #4
	mov r0, sp
	bl sub_803AFE4
	mov r0, sp
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_803AFDC
	b _0800CF16
	.align 2, 0
_0800CEF0: .4byte gUnknown_030012D8
_0800CEF4:
	movs r0, #0
	ldrsh r1, [r6, r0]
	movs r0, #2
	ldrsh r2, [r6, r0]
	ldrb r4, [r6, #4]
	ldrb r5, [r6, #5]
	adds r1, r1, r7
	ldr r0, [sp, #0x28]
	adds r2, r2, r0
	mov r0, sp
	bl sub_803AFE4
	mov r0, sp
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_803AFDC
_0800CF16:
	ldr r3, _0800CF5C @ =gUnknown_030012D8
	ldr r0, [r3]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _0800CF30
	lsls r0, r7, #1
	ldr r1, [sp]
	ldr r2, [sp, #8]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp]
_0800CF30:
	ldr r0, [r3]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1a
	cmp r0, #0
	bge _0800CF4A
	ldr r1, [sp, #0x28]
	lsls r0, r1, #1
	ldr r1, [sp, #4]
	ldr r2, [sp, #0xc]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #4]
_0800CF4A:
	mov r0, r8
	mov r1, sp
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0800CF60
	movs r0, #0
	b _0800CF62
	.align 2, 0
_0800CF5C: .4byte gUnknown_030012D8
_0800CF60:
	movs r0, #1
_0800CF62:
	add sp, #0x10
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_800CF70
sub_800CF70: @ 0x0800CF70
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	mov sl, r1
	adds r5, r2, #0
	adds r7, r0, #0
	bl sub_801070C
	adds r4, r0, #0
	adds r0, r7, #0
	bl sub_8010708
	adds r6, r0, #0
	cmp r4, #0
	bne _0800CF98
	cmp r6, #0
	beq _0800D02E
_0800CF98:
	movs r0, #1
	strb r0, [r5]
	cmp r6, #0
	beq _0800D02E
	adds r1, r6, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #1
	beq _0800D02E
	ldr r1, [r6, #0x20]
	adds r2, r6, #0
	adds r2, #0x2d
	ldrb r3, [r2]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	ldr r1, [r1]
	adds r1, r1, r0
	adds r3, r1, #4
	ldr r0, [r6]
	asrs r0, r0, #8
	mov r8, r0
	ldr r0, [r6, #4]
	asrs r0, r0, #8
	mov sb, r0
	movs r0, #4
	ldrsh r1, [r1, r0]
	movs r0, #2
	ldrsh r2, [r3, r0]
	ldrb r4, [r3, #4]
	ldrb r5, [r3, #5]
	add r1, r8
	add r2, sb
	mov r0, sp
	bl sub_803AFE4
	mov r0, sp
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_803AFDC
	adds r3, r7, #0
	adds r3, #0x28
	ldrb r1, [r3]
	lsls r0, r1, #0x1b
	cmp r0, #0
	bge _0800D008
	mov r1, r8
	lsls r0, r1, #1
	ldr r1, [sp]
	ldr r2, [sp, #8]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp]
_0800D008:
	ldrb r3, [r3]
	lsls r0, r3, #0x1a
	cmp r0, #0
	bge _0800D01E
	mov r3, sb
	lsls r0, r3, #1
	ldr r1, [sp, #4]
	ldr r2, [sp, #0xc]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #4]
_0800D01E:
	mov r0, sp
	mov r1, sl
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0800D02E
	adds r7, r6, #0
_0800D02E:
	adds r0, r7, #0
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

