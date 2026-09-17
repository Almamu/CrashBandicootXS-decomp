.include "asm/macros.inc"

.syntax unified
.arm

@ sub_802F338 is reconstructed (but not yet byte-matching) as C in
@ src/graphics/actor_part43b.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-56-0x0802f0dc-actor.md, "Parked, not matched:
@ sub_802F338".
.if NON_MATCHING == 0
	thumb_func_start sub_802F338
sub_802F338: @ 0x0802F338
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r2, [r4, #8]
	asrs r2, r2, #8
	ldr r1, [r4, #0xc]
	ldr r3, [r4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r3
	movs r1, #2
	ldrsh r0, [r0, r1]
	adds r0, r0, r2
	ldr r1, [r4, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldrb r3, [r0]
	ldrb r1, [r0, #1]
	adds r2, r3, #0
	muls r2, r1, r2
	adds r0, r2, #0
	lsls r0, r0, #5
	bl sub_8028CD4
	ldr r5, _0802F3B0 @ =gUnknown_03001518
	str r0, [r5]
	ldr r2, [r4, #8]
	asrs r2, r2, #8
	ldr r1, [r4, #0xc]
	ldr r3, [r4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r3
	movs r3, #2
	ldrsh r0, [r0, r3]
	adds r0, r0, r2
	ldr r1, [r4, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldrb r2, [r0]
	ldrb r3, [r0, #1]
	adds r1, r2, #0
	muls r1, r3, r1
	adds r0, r1, #0
	lsls r0, r0, #5
	bl sub_8028CD4
	str r0, [r5, #4]
	ldr r1, _0802F3B4 @ =gUnknown_03001510
	movs r0, #1
	str r0, [r1]
	ldr r1, _0802F3B8 @ =gUnknown_03001514
	movs r0, #0
	str r0, [r1]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802F3B0: .4byte gUnknown_03001518
_0802F3B4: .4byte gUnknown_03001510
_0802F3B8: .4byte gUnknown_03001514

.endif
