.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8026F54
sub_8026F54: @ 0x08026F54
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	ldrb r0, [r7]
	cmp r0, #0
	beq _0802700C
	movs r4, #0
	ldr r0, [r7, #0x40]
	mov r8, r0
	cmp r4, r8
	bge _0802700C
_08026F6C:
	ldr r0, _08026FD4 @ =gUnknown_0300082C
	lsls r5, r4, #2
	adds r1, r7, #0
	adds r1, #0x28
	adds r1, r1, r5
	ldr r0, [r0]
	ldr r1, [r1]
	bl sub_803AF1C
	adds r4, #1
	mov ip, r4
	cmp r0, #0
	bne _08027006
	adds r0, r7, #0
	adds r0, #0x10
	adds r0, r0, r5
	ldr r6, [r0]
	adds r0, r7, #0
	adds r0, #0x1c
	adds r0, r0, r5
	ldr r3, [r0]
	adds r0, r7, #0
	adds r0, #0x44
	ldrb r0, [r0]
	cmp r0, #0
	beq _08026FD8
	adds r0, r7, #0
	adds r0, #0x34
	adds r0, r0, r5
	ldr r1, [r0]
	lsls r0, r1, #1
	adds r0, r0, r3
	subs r0, #2
	ldrh r0, [r0]
	lsls r0, r0, #1
	adds r0, r0, r6
	ldrh r4, [r0]
	cmp r1, #0
	ble _08027006
	adds r2, r1, #0
_08026FBC:
	ldrh r1, [r3]
	lsls r0, r1, #1
	adds r0, r0, r6
	ldrh r1, [r0]
	strh r4, [r0]
	adds r4, r1, #0
	adds r3, #2
	subs r2, #1
	cmp r2, #0
	bne _08026FBC
	b _08027006
	.align 2, 0
_08026FD4: .4byte gUnknown_0300082C
_08026FD8:
	ldrh r1, [r3]
	lsls r0, r1, #1
	adds r0, r0, r6
	ldrh r4, [r0]
	adds r0, r7, #0
	adds r0, #0x34
	adds r0, r0, r5
	ldr r0, [r0]
	subs r2, r0, #1
	cmp r2, #0
	blt _08027006
	lsls r0, r2, #1
	adds r3, r0, r3
_08026FF2:
	ldrh r1, [r3]
	lsls r0, r1, #1
	adds r0, r0, r6
	ldrh r1, [r0]
	strh r4, [r0]
	adds r4, r1, #0
	subs r3, #2
	subs r2, #1
	cmp r2, #0
	bge _08026FF2
_08027006:
	mov r4, ip
	cmp r4, r8
	blt _08026F6C
_0802700C:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8027018
sub_8027018: @ 0x08027018
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	adds r4, r1, #0
	add r0, sp, #0x18
	ldrb r7, [r0]
	cmp r4, #0
	beq _08027032
	cmp r2, #0
	beq _08027032
	adds r6, r5, #0
	adds r6, #0x34
	cmp r6, #0
	bne _08027038
_08027032:
	movs r0, #0
	strb r0, [r5]
	b _08027082
_08027038:
	ldr r1, [r5, #0x40]
	lsls r1, r1, #2
	adds r0, r5, #0
	adds r0, #0x10
	adds r0, r0, r1
	str r4, [r0]
	adds r0, r5, #0
	adds r0, #0x1c
	adds r0, r0, r1
	str r2, [r0]
	adds r4, r5, #0
	adds r4, #0x28
	adds r4, r4, r1
	movs r0, #0x3c
	adds r1, r3, #0
	bl sub_803ADB4
	str r0, [r4]
	movs r2, #0
	movs r0, #1
	strb r0, [r5]
	ldr r1, [r5, #0x40]
	lsls r1, r1, #2
	adds r0, r5, #4
	adds r0, r0, r1
	str r2, [r0]
	ldr r0, [r5, #0x40]
	lsls r0, r0, #2
	adds r0, r6, r0
	ldr r1, [sp, #0x14]
	str r1, [r0]
	ldr r0, [r5, #0x40]
	adds r0, #1
	str r0, [r5, #0x40]
	adds r0, r5, #0
	adds r0, #0x44
	strb r7, [r0]
_08027082:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
