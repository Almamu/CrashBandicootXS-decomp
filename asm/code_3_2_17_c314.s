.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_800C314
sub_800C314: @ 0x0800C314
	push {r4, lr}
	adds r4, r0, #0
	ldr r2, [r4, #0x70]
	adds r0, r2, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800C406
	ldr r0, [r4, #0x68]
	cmp r0, #1
	beq _0800C388
	cmp r0, #1
	bgt _0800C334
	cmp r0, #0
	beq _0800C33A
	b _0800C406
_0800C334:
	cmp r0, #6
	beq _0800C3BA
	b _0800C406
_0800C33A:
	adds r0, r4, #0
	adds r0, #0x80
	ldr r0, [r0]
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _0800C36E
	adds r3, r2, #0
	adds r3, #0x28
	ldrb r2, [r3]
	lsls r0, r2, #0x1b
	movs r1, #0
	cmp r0, #0
	blt _0800C358
	movs r1, #1
_0800C358:
	lsls r1, r1, #4
	movs r0, #0x11
	rsbs r0, r0, #0
	ands r0, r2
	orrs r0, r1
	strb r0, [r3]
	adds r0, r4, #0
	movs r1, #1
	bl sub_800C8CC
	b _0800C376
_0800C36E:
	adds r0, r4, #0
	movs r1, #6
	bl sub_800C8CC
_0800C376:
	adds r0, r4, #0
	movs r1, #0
	bl sub_800C8BC
	adds r0, r4, #0
	movs r1, #0
	bl sub_800C8AC
	b _0800C406
_0800C388:
	adds r3, r4, #0
	adds r3, #0x80
	ldr r2, [r3]
	adds r1, r2, #1
	adds r0, r1, #0
	cmp r1, #0
	bge _0800C398
	adds r0, r2, #4
_0800C398:
	asrs r0, r0, #2
	lsls r0, r0, #2
	subs r0, r1, r0
	str r0, [r3]
	adds r0, r4, #0
	movs r1, #2
	bl sub_800C8BC
	adds r0, r4, #0
	movs r1, #2
	bl sub_800C8AC
	adds r0, r4, #0
	movs r1, #0
	bl sub_800C8CC
	b _0800C406
_0800C3BA:
	adds r3, r2, #0
	adds r3, #0x28
	ldrb r2, [r3]
	lsls r0, r2, #0x1a
	movs r1, #0
	cmp r0, #0
	blt _0800C3CA
	movs r1, #1
_0800C3CA:
	lsls r1, r1, #5
	movs r0, #0x21
	rsbs r0, r0, #0
	ands r0, r2
	orrs r0, r1
	strb r0, [r3]
	adds r3, r4, #0
	adds r3, #0x80
	ldr r2, [r3]
	adds r1, r2, #1
	adds r0, r1, #0
	cmp r1, #0
	bge _0800C3E6
	adds r0, r2, #4
_0800C3E6:
	asrs r0, r0, #2
	lsls r0, r0, #2
	subs r0, r1, r0
	str r0, [r3]
	adds r0, r4, #0
	movs r1, #2
	bl sub_800C8BC
	adds r0, r4, #0
	movs r1, #2
	bl sub_800C8AC
	adds r0, r4, #0
	movs r1, #0
	bl sub_800C8CC
_0800C406:
	pop {r4}
	pop {r0}
	bx r0
