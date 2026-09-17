.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8002D0C is reconstructed (semantics fully understood) but NOT YET
@ byte-matching - parked as C in src/graphics/settings_menu8.c, guarded
@ by #if NON_MATCHING. See docs/matching/issue-5-overlay-ui-sync.md.
.if NON_MATCHING == 0
	thumb_func_start sub_8002D0C
sub_8002D0C: @ 0x08002D0C
	push {lr}
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	movs r3, #0xfd
	lsls r3, r3, #1
	adds r2, r0, r3
	ldrb r3, [r2]
	bics r3, r1
	adds r1, r3, #0
	strb r1, [r2]
	bl sub_8002B70
	pop {r0}
	bx r0
.endif
