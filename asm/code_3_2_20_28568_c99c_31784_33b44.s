.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8033B44 is reconstructed (but not yet byte-matching) as C in
@ src/graphics/actor_part31.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-62-0x08033804-actor.md, "Parked, not matched:
@ sub_8033B44".
.if NON_MATCHING == 0
	thumb_func_start sub_8033B44
sub_8033B44: @ 0x08033B44
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r1, _08033B70 @ =gStaticData_0817C4E0
	ldr r0, [r4, #0x28]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r7, #2
	ldrsh r2, [r0, r7]
	adds r7, r1, #0
	cmp r2, #0
	ble _08033B74
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
	b _08033B7A
	.align 2, 0
_08033B70: .4byte gStaticData_0817C4E0
_08033B74:
	adds r0, r7, #4
	adds r0, r3, r0
	ldr r3, [r0]
_08033B7A:
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _08033B90
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _08033B92
_08033B90:
	adds r0, r1, #0
_08033B92:
	adds r0, r4, r0
	bl sub_803AD84
	ldr r0, [r4, #0x28]
	movs r1, #1
	cmp r0, #2
	bne _08033BA8
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _08033BA8
	movs r1, #0
_08033BA8:
	cmp r1, #0
	beq _08033BB2
	adds r0, r4, #0
	bl sub_802A7B8
_08033BB2:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

.endif
