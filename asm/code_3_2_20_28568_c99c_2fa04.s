.include "asm/macros.inc"

.syntax unified
.arm

@ sub_802FA04 is reconstructed (but not yet byte-matching) as C in
@ src/graphics/actor_part45c.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-56-0x0802f0dc-actor.md, "Parked, not matched:
@ sub_802FA04".
.if NON_MATCHING == 0
	thumb_func_start sub_802FA04
sub_802FA04: @ 0x0802FA04
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, [sp, #0x18]
	ldr r6, [sp, #0x1c]
	ldr r7, [sp, #0x20]
	movs r5, #1
	str r0, [sp]
	adds r0, r4, #0
	bl InitActorPart
	str r5, [r4, #0x54]
	ldr r0, _0802FA30 @ =gStaticData_087E517C
	str r0, [r4, #0x50]
	str r6, [r4, #0x58]
	str r7, [r4, #0x5c]
	adds r0, r4, #0
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0802FA30: .4byte gStaticData_087E517C

.endif
