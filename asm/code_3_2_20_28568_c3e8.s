.include "asm/macros.inc"

.syntax unified
.arm

@ sub_802C3E8 is reconstructed (but not yet byte-matching) as C in
@ src/graphics/actor_part19c2.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching.md, "Parked, not matched: sub_802C3E8".
.if NON_MATCHING == 0
	thumb_func_start sub_802C3E8
sub_802C3E8: @ 0x0802C3E8
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r6, r0, #0
	ldr r4, [sp, #0x18]
	movs r0, #1
	str r0, [sp]
	adds r0, r6, #0
	bl InitActorPart
	ldr r0, _0802C458 @ =gStaticData_087E4E74
	str r0, [r6, #0x50]
	str r4, [r6, #0x5c]
	bl sub_8029E98
	ldr r1, [r6, #0x20]
	adds r1, r1, r0
	str r1, [r6, #0x20]
	bl sub_8029EB4
	ldr r1, [r6, #0x1c]
	adds r3, r1, r0
	str r3, [r6, #0x1c]
	ldr r0, _0802C45C @ =0xFFFFF000
	adds r1, r3, r0
	asrs r2, r1, #0x1f
	eors r1, r2
	subs r1, r1, r2
	ldr r7, [r6, #0x20]
	adds r0, r7, r0
	asrs r2, r0, #0x1f
	eors r0, r2
	subs r0, r0, r2
	adds r1, r1, r0
	cmp r1, #0
	bge _0802C432
	ldr r0, _0802C460 @ =0x000007FF
	adds r1, r1, r0
_0802C432:
	asrs r5, r1, #0xb
	movs r4, #0x80
	lsls r4, r4, #5
	subs r0, r4, r3
	adds r1, r5, #0
	bl sub_803ADB4
	str r0, [r6, #0x54]
	subs r4, r4, r7
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_803ADB4
	str r0, [r6, #0x58]
	adds r0, r6, #0
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0802C458: .4byte gStaticData_087E4E74
_0802C45C: .4byte 0xFFFFF000
_0802C460: .4byte 0x000007FF

.endif
