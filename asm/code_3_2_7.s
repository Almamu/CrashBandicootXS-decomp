.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8008770 is reconstructed (extremely close, but not yet
@ byte-matching) as C in src/graphics/actor_part6.c, guarded by
@ #if NON_MATCHING - see docs/matching.md for the exact remaining gap.
.if NON_MATCHING == 0
	thumb_func_start sub_8008770
sub_8008770: @ 0x08008770
	ldr r1, [r0, #0x20]
	adds r0, #0x2d
	ldr r2, [r1]
	ldrb r3, [r0]
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	adds r1, r1, r2
	movs r0, #2
	ldrb r1, [r1, #0x17]
	ands r0, r1
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	bx lr
.endif

