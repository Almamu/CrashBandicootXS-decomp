.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8008D80 is reconstructed (semantics fully understood, but not
@ yet byte-matching) as C in src/graphics/actor_part7.c, guarded by
@ #if NON_MATCHING - see docs/matching.md for the exact remaining gap.
.if NON_MATCHING == 0
	thumb_func_start sub_8008D80
sub_8008D80: @ 0x08008D80
	sub sp, #0xc
	push {r4, r5, lr}
	str r1, [sp, #0xc]
	str r2, [sp, #0x10]
	str r3, [sp, #0x14]
	ldr r4, [sp, #0x1c]
	ldr r5, [sp, #0x20]
	adds r0, r4, #0
	add r1, sp, #0xc
	bl sub_8009FF4
	cmp r0, #0
	beq _08008DB8
	ldr r1, [r4, #0x18]
	adds r1, #0x68
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldrb r2, [r5, #0xa]
	ldr r4, [r1, #4]
	movs r1, #1
	movs r3, #0
	bl sub_803AD88
	movs r0, #8
	ldrb r1, [r5, #0xc]
	orrs r0, r1
	strb r0, [r5, #0xc]
_08008DB8:
	pop {r4, r5}
	pop {r3}
	add sp, #0xc
	bx r3
.endif

