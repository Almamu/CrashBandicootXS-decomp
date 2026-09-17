.include "asm/macros.inc"

.syntax unified
.arm

@ sub_80156EC is reconstructed (but not yet byte-matching) as C in
@ src/graphics/actor_part28.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-18-0x08014f8c-actor.md, "Parked, not matched:
@ sub_80156EC".
.if NON_MATCHING == 0
	thumb_func_start sub_80156EC
sub_80156EC: @ 0x080156EC
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08015744
	ldr r0, _08015730 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231BC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08015734
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x19
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #7
	bl sub_803AD84
	b _08015744
	.align 2, 0
_08015730: .4byte gUnknown_030012C0
_08015734:
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x18
	bl sub_803AD80
_08015744:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

.endif
