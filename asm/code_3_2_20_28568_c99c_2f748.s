.include "asm/macros.inc"

.syntax unified
.arm

@ sub_802F748 is reconstructed (but not yet byte-matching) as C in
@ src/graphics/actor_part44b.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-56-0x0802f0dc-actor.md, "Parked, not matched:
@ sub_802F748".
.if NON_MATCHING == 0
	thumb_func_start sub_802F748
sub_802F748: @ 0x0802F748
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r1, _0802F774 @ =gStaticData_0817C1C0
	ldr r0, [r4, #0x28]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r7, #2
	ldrsh r2, [r0, r7]
	adds r7, r1, #0
	cmp r2, #0
	ble _0802F778
	movs r1, #4
	ldrsh r0, [r0, r1]
	adds r0, r4, r0
	ldr r1, [r0]
	lsls r0, r2, #3
	adds r0, r0, r1
	subs r0, #8
	ldr r5, [r0]
	ldr r6, [r0, #4]
	adds r3, r6, #0
	b _0802F77E
	.align 2, 0
_0802F774: .4byte gStaticData_0817C1C0
_0802F778:
	adds r0, r7, #4
	adds r0, r3, r0
	ldr r3, [r0]
_0802F77E:
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _0802F794
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _0802F796
_0802F794:
	adds r0, r1, #0
_0802F796:
	adds r0, r4, r0
	bl sub_803AD84
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

.endif
