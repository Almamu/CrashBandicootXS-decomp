.include "asm/macros.inc"

.syntax unified
.arm

@ sub_800B6A0 is reconstructed (semantics fully understood, but not
@ yet byte-matching) as C in src/graphics/actor_part15.c, guarded by
@ #if NON_MATCHING - see docs/matching.md for the exact remaining gap.
@ This compiler unconditionally spills the `vec` pointer to a
@ callee-saved register (r4, via push{r4,lr}/pop{r4}) whenever it is
@ referenced from both branches of an if/else, even though nothing in
@ either branch clobbers it - the real ROM is a true leaf function
@ using only r0-r3 with no stack frame at all. Three independent
@ workarounds (pinning `self` alone to r3; additionally pinning and
@ reassigning `vec` to r2; restructuring the if/else into an
@ equivalent goto-based flow) all produced the identical 8-byte-larger
@ leaf-with-frame version, so this is accepted as an unavoidable
@ compiler limitation for this pattern.
.if NON_MATCHING == 0
	thumb_func_start sub_800B6A0
sub_800B6A0: @ 0x0800B6A0
	adds r3, r1, #0
	adds r0, r3, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1a
	cmp r0, #0
	bge _0800B6C0
	ldr r0, [r2]
	rsbs r0, r0, #0
	ldr r1, [r2, #8]
	rsbs r1, r1, #0
	ldr r2, [r2, #4]
	str r0, [r3, #0x54]
	str r2, [r3, #0x58]
	str r1, [r3, #0x5c]
	b _0800B6CC
_0800B6C0:
	ldr r0, [r2]
	ldr r1, [r2, #4]
	ldr r2, [r2, #8]
	str r0, [r3, #0x54]
	str r1, [r3, #0x58]
	str r2, [r3, #0x5c]
_0800B6CC:
	bx lr
	.align 2, 0
.endif

@ sub_800B6D0 is reconstructed (semantics fully understood, but not
@ yet byte-matching) as C in src/graphics/actor_part15.c, guarded by
@ #if NON_MATCHING - see docs/matching.md for the exact remaining gap.
@ Same unavoidable spill-to-callee-saved-register limitation as
@ sub_800B6A0 above.
.if NON_MATCHING == 0
	thumb_func_start sub_800B6D0
sub_800B6D0: @ 0x0800B6D0
	adds r3, r1, #0
	adds r0, r3, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1a
	cmp r0, #0
	bge _0800B6F2
	ldr r0, [r2]
	rsbs r0, r0, #0
	ldr r1, [r2, #8]
	rsbs r1, r1, #0
	ldr r2, [r2, #4]
	str r0, [r3, #0x64]
	str r0, [r3, #0x54]
	str r2, [r3, #0x58]
	str r1, [r3, #0x5c]
	b _0800B700
_0800B6F2:
	ldr r0, [r2]
	ldr r1, [r2, #4]
	ldr r2, [r2, #8]
	str r0, [r3, #0x64]
	str r0, [r3, #0x54]
	str r1, [r3, #0x58]
	str r2, [r3, #0x5c]
_0800B700:
	bx lr
	.align 2, 0
.endif
