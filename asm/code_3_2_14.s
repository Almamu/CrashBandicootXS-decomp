.include "asm/macros.inc"

.syntax unified
.arm

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
