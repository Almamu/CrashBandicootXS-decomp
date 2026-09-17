.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8001524 is reconstructed (semantics fully understood, but not
@ yet byte-matching) as C in src/graphics/fade_screen_mode.c, guarded
@ by #if NON_MATCHING - see docs/matching.md for the exact remaining
@ gap.
.if NON_MATCHING == 0
	thumb_func_start sub_8001524
sub_8001524: @ 0x08001524
	ldr r2, _08001538 @ =gUnknown_03001288
	movs r1, #7
	ands r0, r1
	movs r1, #8
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	bx lr
	.align 2, 0
_08001538: .4byte gUnknown_03001288
.endif
