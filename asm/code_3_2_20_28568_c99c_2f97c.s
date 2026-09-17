.include "asm/macros.inc"

.syntax unified
.arm

@ sub_802F97C is reconstructed (but not yet byte-matching) as C in
@ src/graphics/actor_part45b.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-56-0x0802f0dc-actor.md, "Parked, not matched:
@ sub_802F97C".
.if NON_MATCHING == 0
	thumb_func_start sub_802F97C
sub_802F97C: @ 0x0802F97C
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x1c]
	ldr r1, [r4, #0x58]
	adds r0, r0, r1
	str r0, [r4, #0x1c]
	ldr r0, [r4, #0x20]
	subs r0, #0xc0
	ldr r1, [r4, #0x5c]
	adds r0, r0, r1
	str r0, [r4, #0x20]
	ldr r0, [r4, #0x24]
	movs r1, #0x80
	lsls r1, r1, #3
	adds r0, r0, r1
	str r0, [r4, #0x24]
	adds r0, r4, #0
	bl sub_802A3AC
	adds r2, r0, #0
	cmp r2, #0
	beq _0802F9BA
	ldr r1, [r2, #0x50]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0x24]
	movs r1, #2
	bl sub_803AD80
	b _0802F9E2
_0802F9BA:
	adds r0, r4, #0
	bl sub_8031378
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802F9D8
	movs r0, #2
	bl sub_803146C
	cmp r4, #0
	beq _0802F9FE
	ldr r1, [r4, #0x50]
	movs r3, #8
	ldrsh r0, [r1, r3]
	b _0802F9EC
_0802F9D8:
	ldr r1, [r4, #0x34]
	movs r0, #0x82
	lsls r0, r0, #8
	cmp r1, r0
	ble _0802F9F8
_0802F9E2:
	cmp r4, #0
	beq _0802F9FE
	ldr r1, [r4, #0x50]
	movs r2, #8
	ldrsh r0, [r1, r2]
_0802F9EC:
	adds r0, r4, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
	b _0802F9FE
_0802F9F8:
	adds r0, r4, #0
	bl sub_802A7B8
_0802F9FE:
	pop {r4}
	pop {r0}
	bx r0

.endif
