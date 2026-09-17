.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8033E80 is reconstructed (but not yet byte-matching) as C in
@ src/graphics/actor_part37.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-62-0x08033804-actor.md, "Parked, not matched:
@ sub_8033E80".
.if NON_MATCHING == 0
	thumb_func_start sub_8033E80
sub_8033E80: @ 0x08033E80
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r1, _08033EAC @ =gStaticData_0817C4F8
	ldr r0, [r4, #0x28]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r7, #2
	ldrsh r2, [r0, r7]
	adds r7, r1, #0
	cmp r2, #0
	ble _08033EB0
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
	b _08033EB6
	.align 2, 0
_08033EAC: .4byte gStaticData_0817C4F8
_08033EB0:
	adds r0, r7, #4
	adds r0, r3, r0
	ldr r3, [r0]
_08033EB6:
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _08033ECC
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _08033ECE
_08033ECC:
	adds r0, r1, #0
_08033ECE:
	adds r0, r4, r0
	bl sub_803AD84
	ldr r0, [r4, #0x28]
	movs r1, #1
	cmp r0, #2
	bne _08033EE4
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _08033EE4
	movs r1, #0
_08033EE4:
	cmp r1, #0
	beq _08033EEE
	adds r0, r4, #0
	bl sub_802A7B8
_08033EEE:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

.endif
