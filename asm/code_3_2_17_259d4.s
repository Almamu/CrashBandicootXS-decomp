.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_80259D4
sub_80259D4: @ 0x080259D4
	mov ip, r0
	adds r2, r1, #0
	adds r0, r2, #0
	cmp r2, #0
	bge _080259E0
	adds r0, #0x1f
_080259E0:
	asrs r0, r0, #5
	lsls r3, r0, #2
	movs r1, #0x82
	lsls r1, r1, #2
	add r1, ip
	adds r1, r1, r3
	lsls r0, r0, #5
	subs r0, r2, r0
	movs r2, #1
	lsls r2, r0
	ldr r0, [r1]
	orrs r0, r2
	str r0, [r1]
	movs r1, #0xc2
	lsls r1, r1, #2
	add r1, ip
	adds r1, r1, r3
	ldr r0, [r1]
	orrs r0, r2
	str r0, [r1]
	bx lr
	.align 2, 0

