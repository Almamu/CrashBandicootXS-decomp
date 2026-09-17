.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8001624 is reconstructed (semantics fully understood, but not
@ yet byte-matching) as C in src/graphics/fade_screen_mode.c, guarded
@ by #if NON_MATCHING - see docs/matching.md for the exact remaining
@ gap.
.if NON_MATCHING == 0
	thumb_func_start sub_8001624
sub_8001624: @ 0x08001624
	ldr r2, _08001638 @ =0x04000050
	ldr r1, _0800163C @ =gUnknown_03001280
	ldr r0, [r1]
	str r0, [r2]
	adds r2, #4
	ldrb r1, [r1, #4]
	lsls r0, r1, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r2]
	bx lr
	.align 2, 0
_08001638: .4byte 0x04000050
_0800163C: .4byte gUnknown_03001280
.endif

