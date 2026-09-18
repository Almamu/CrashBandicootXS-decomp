.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8034314
sub_8034314: @ 0x08034314
	push {r4, lr}
	adds r4, r0, #0
	bl sub_80338E8
	ldr r1, _08034360 @ =0xFFFFFE00
	adds r0, r0, r1
	str r0, [r4, #0x24]
	bl sub_8033900
	movs r2, #0x80
	lsls r2, r2, #6
	adds r0, r0, r2
	str r0, [r4, #0x1c]
	bl sub_80338F4
	movs r1, #0xc0
	lsls r1, r1, #6
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r1, r4, #0
	adds r1, #0x58
	movs r0, #1
	strb r0, [r1]
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _08034364
	cmp r4, #0
	beq _0803435C
	ldr r1, [r4, #0x50]
	movs r2, #8
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_0803435C:
	movs r0, #0
	b _08034366
	.align 2, 0
_08034360: .4byte 0xFFFFFE00
_08034364:
	movs r0, #1
_08034366:
	pop {r4}
	pop {r1}
	bx r1
