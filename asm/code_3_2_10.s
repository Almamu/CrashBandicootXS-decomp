.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8009FD4
sub_8009FD4: @ 0x08009FD4
	push {r4, r5, r6, lr}
	adds r5, r1, #0
	ldr r4, [r0, #0x44]
	cmp r4, #0
	beq _08009FEE
	ldr r1, [r4, #0xc]
	movs r6, #0x10
	ldrsh r0, [r1, r6]
	adds r0, r4, r0
	ldr r4, [r1, #0x14]
	adds r1, r5, #0
	bl sub_803AD88
_08009FEE:
	pop {r4, r5, r6}
	pop {r0}
	bx r0

