.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_803A158
sub_803A158: @ 0x0803A158
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	adds r4, r0, #0
	mov sb, r1
	mov sl, r2
	ldr r0, [r4, #8]
	ldr r6, [r0]
	ldrb r0, [r6, #0x1b]
	cmp r0, #0
	beq _0803A178
	movs r0, #0
	str r0, [r4, #0x3c]
_0803A178:
	ldrb r0, [r6, #0x1a]
	cmp r0, #0
	beq _0803A1C6
	movs r0, #0x24
	adds r0, r0, r4
	mov r8, r0
	ldrb r0, [r0]
	cmp r0, #1
	beq _0803A198
	cmp r0, #0
	beq _0803A1C6
	adds r0, r4, #0
	adds r0, #0x25
	ldrb r0, [r0]
	cmp r0, #0
	beq _0803A1C6
_0803A198:
	ldrb r5, [r6, #0x1b]
	cmp r5, #0
	bne _0803A1C6
	mov r0, r8
	ldrb r1, [r0]
	adds r0, r4, #0
	bl sub_8039818
	adds r7, r4, #0
	adds r7, #0x25
	ldrb r2, [r7]
	ldr r0, [r4]
	ldr r3, [r0, #0x18]
	adds r0, r4, #0
	adds r1, r6, #0
	bl sub_803985C
	movs r0, #0
	strh r5, [r4, #0x1a]
	strh r5, [r4, #0x28]
	strb r0, [r7]
	mov r1, r8
	strb r0, [r1]
_0803A1C6:
	ldr r0, [r4, #0x3c]
	ldrb r1, [r4, #0x1f]
	cmp r0, #0
	beq _0803A1E6
	cmp r1, #0
	bne _0803A1E6
	ldrb r0, [r4, #0x1e]
	cmp r0, #0
	beq _0803A1EA
	adds r0, r4, #0
	adds r1, r6, #0
	bl sub_80398DC
	ldrb r0, [r4, #0x1e]
	subs r0, #1
	b _0803A1E8
_0803A1E6:
	subs r0, r1, #1
_0803A1E8:
	strb r0, [r4, #0x1f]
_0803A1EA:
	adds r0, r4, #0
	adds r1, r6, #0
	bl sub_8039AA4
	ldrb r0, [r4, #0xc]
	cmp r0, #0
	bne _0803A214
	ldr r0, [r4]
	ldr r0, [r0, #0x18]
	str r0, [sp]
	movs r0, #1
	str r0, [sp, #4]
	adds r0, r4, #0
	adds r1, r6, #0
	mov r2, sb
	mov r3, sl
	bl sub_8039B44
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	b _0803A216
_0803A214:
	movs r0, #0
_0803A216:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

