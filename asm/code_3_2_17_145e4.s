.include "asm/macros.inc"

.syntax unified
.arm

@ sub_80145E4 is reconstructed (but not yet byte-matching) as C in
@ src/graphics/actor_part18.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching.md, "Parked, not matched: sub_80145E4".
.if NON_MATCHING == 0
	thumb_func_start sub_80145E4
sub_80145E4: @ 0x080145E4
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r4, r0, #0
	movs r6, #0
	str r6, [r4, #0x18]
	ldr r0, _08014628 @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #0x80
	lsls r1, r1, #1
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r5, r0, #0x10
	cmp r5, #0
	beq _0801462C
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x10
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #3
	bl sub_803AD84
	str r6, [r4, #0x1c]
	b _0801466A
	.align 2, 0
_08014628: .4byte gUnknown_030007E0
_0801462C:
	ldr r0, [r4, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08014664
	str r5, [sp]
	adds r0, r4, #0
	movs r1, #0
	movs r2, #0x12
	movs r3, #0
	bl sub_8015780
	adds r0, r4, #0
	adds r0, #0x31
	strb r5, [r0]
	adds r1, r4, #0
	adds r1, #0x2f
	movs r0, #1
	strb r0, [r1]
	subs r1, #8
	strb r5, [r1]
	adds r1, #0xb
	strb r5, [r1]
	subs r1, #2
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x28
	strb r5, [r0]
_08014664:
	adds r0, r4, #0
	bl sub_8012D24
_0801466A:
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

.endif
