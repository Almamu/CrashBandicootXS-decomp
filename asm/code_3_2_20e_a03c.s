.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_803A03C
sub_803A03C: @ 0x0803A03C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r2, r0, #0
	ldrb r0, [r2, #0x12]
	cmp r0, #0
	beq _0803A0F8
	ldrb r0, [r2, #0x14]
	subs r0, #1
	strb r0, [r2, #0x14]
	movs r1, #0xff
	mov r8, r1
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0803A0F8
	ldr r6, [r2, #0x48]
	ldr r0, [r2, #0x3c]
	mov ip, r0
	ldrb r1, [r2, #0x10]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r0, r0, #2
	add r0, ip
	ldrh r0, [r0, #0x24]
	strb r0, [r2, #0x14]
	ldrb r7, [r2, #0x13]
	movs r0, #0x13
	ldrsb r0, [r2, r0]
	cmp r0, #0
	ble _0803A0B6
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r0, r0, #2
	mov r5, ip
	adds r5, #0x20
	adds r0, r5, r0
	ldr r0, [r0]
	adds r4, r6, r0
	str r4, [r2, #0x48]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r3, r0, #2
	mov r0, ip
	adds r0, #0x1c
	adds r0, r0, r3
	ldr r1, [r0]
	adds r1, r4, r1
	mov r0, ip
	adds r0, #0x18
	adds r0, r0, r3
	ldr r0, [r0]
	cmp r1, r0
	ble _0803A0EA
	adds r0, r5, r3
	ldr r0, [r0]
	lsls r0, r0, #1
	subs r0, r4, r0
	str r0, [r2, #0x48]
	mov r0, r8
	orrs r0, r7
	b _0803A0E8
_0803A0B6:
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r0, r0, #2
	mov r4, ip
	adds r4, #0x20
	adds r0, r4, r0
	ldr r0, [r0]
	subs r3, r6, r0
	str r3, [r2, #0x48]
	ldrb r1, [r2, #0x10]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r1, r0, #2
	mov r0, ip
	adds r0, #0x14
	adds r0, r0, r1
	ldr r0, [r0]
	cmp r3, r0
	bge _0803A0EA
	adds r0, r4, r1
	ldr r0, [r0]
	lsls r0, r0, #1
	adds r0, r3, r0
	str r0, [r2, #0x48]
	movs r0, #1
_0803A0E8:
	strb r0, [r2, #0x13]
_0803A0EA:
	lsls r0, r6, #0xb
	ldr r1, [r2, #0x44]
	subs r1, r1, r0
	ldr r0, [r2, #0x48]
	lsls r0, r0, #0xb
	adds r1, r1, r0
	str r1, [r2, #0x44]
_0803A0F8:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

