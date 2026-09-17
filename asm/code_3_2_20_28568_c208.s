.include "asm/macros.inc"

.syntax unified
.arm

@ sub_802C208 is reconstructed (but not yet byte-matching) as C in
@ src/graphics/actor_part19e.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching.md, "Parked, not matched: sub_802C208".
.if NON_MATCHING == 0
	thumb_func_start sub_802C208
sub_802C208: @ 0x0802C208
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r1, _0802C234 @ =gStaticData_0817A6B8
	ldr r0, [r4, #0x28]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r7, #2
	ldrsh r2, [r0, r7]
	adds r7, r1, #0
	cmp r2, #0
	ble _0802C238
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
	b _0802C23E
	.align 2, 0
_0802C234: .4byte gStaticData_0817A6B8
_0802C238:
	adds r0, r7, #4
	adds r0, r3, r0
	ldr r3, [r0]
_0802C23E:
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _0802C254
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _0802C256
_0802C254:
	adds r0, r1, #0
_0802C256:
	adds r0, r4, r0
	bl sub_803AD84
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

.endif
