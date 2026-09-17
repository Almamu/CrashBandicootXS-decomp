.include "asm/macros.inc"

.syntax unified
.arm

@ sub_802AA0C is reconstructed (but not yet byte-matching) as C in
@ src/graphics/actor_part39.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-50-actor-2a69c.md.
.if NON_MATCHING == 0
	thumb_func_start sub_802AA0C
sub_802AA0C: @ 0x0802AA0C
	push {r4, r5, r6, lr}
	sub sp, #0xc
	mov r3, sp
	adds r2, r1, #0
	adds r2, #0x38
	ldm r2!, {r4, r5, r6}
	stm r3!, {r4, r5, r6}
	ldr r3, [r1, #0x1c]
	asrs r3, r3, #8
	ldr r5, [r1, #0x20]
	asrs r5, r5, #8
	ldr r4, [r1, #0x24]
	asrs r4, r4, #8
	mov r2, sp
	ldrh r1, [r2]
	adds r3, r1, r3
	strh r3, [r2]
	ldrh r1, [r2, #2]
	adds r1, r1, r5
	strh r1, [r2, #2]
	ldrh r3, [r2, #4]
	adds r4, r3, r4
	strh r4, [r2, #4]
	adds r2, r0, #0
	mov r1, sp
	ldm r1!, {r4, r5, r6}
	stm r2!, {r4, r5, r6}
	add sp, #0xc
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
.endif
