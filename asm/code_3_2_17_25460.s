.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8025460 is reconstructed (but not yet byte-matching) as C in
@ src/system/game_loop4.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-40-terrain-tile-cache.md.
.if NON_MATCHING == 0
	thumb_func_start sub_8025460
sub_8025460: @ 0x08025460
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	adds r4, r1, #0
	adds r6, r2, #0
	adds r7, r3, #0
	cmp r4, #0
	blt _08025472
	cmp r6, #0
	bge _08025476
_08025472:
	movs r0, #0
	b _080254BA
_08025476:
	asrs r3, r4, #4
	asrs r1, r6, #3
	ldr r2, [r5]
	ldr r0, [r5, #0x18]
	muls r0, r1, r0
	adds r0, r0, r3
	ldr r1, [r2]
	lsls r0, r0, #1
	adds r0, r0, r1
	ldrh r1, [r0]
	adds r0, r5, #0
	bl sub_8024F24
	movs r1, #7
	ands r1, r6
	movs r3, #0xf
	ands r4, r3
	lsls r1, r1, #4
	adds r1, r1, r4
	lsls r1, r1, #1
	adds r1, r1, r0
	ldrh r1, [r1]
	lsls r1, r1, #0x10
	lsrs r4, r1, #0x10
	lsrs r2, r1, #0x1c
	ldr r0, [sp, #0x14]
	str r2, [r0]
	lsrs r1, r1, #0x18
	ands r1, r3
	cmp r1, #0
	beq _080254B6
	strb r1, [r7]
_080254B6:
	movs r0, #0xff
	ands r0, r4
_080254BA:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1


.endif
