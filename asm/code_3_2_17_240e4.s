.include "asm/macros.inc"

.syntax unified
.arm

@ sub_80240E4 is reconstructed (but not yet byte-matching) as C in
@ src/system/game_loop8.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-37-game-loop-234e8.md.
.if NON_MATCHING == 0
	thumb_func_start sub_80240E4
sub_80240E4: @ 0x080240E4
	push {r4, r5, r6, r7, lr}
	adds r2, r0, #0
	ldr r6, _08024154 @ =gUnknown_03001280
	movs r0, #0
	str r0, [r6]
	ldr r3, _08024158 @ =gUnknown_03001308
	ldr r1, [r3]
	adds r1, #0x2b
	strb r0, [r1]
	ldr r1, [r2, #0x18]
	ldrh r0, [r1, #0x10]
	cmp r0, #0
	beq _0802415C
	ldr r1, [r1, #8]
	cmp r1, #1
	bne _0802410A
	ldr r0, [r3]
	adds r0, #0x2b
	strb r1, [r0]
_0802410A:
	ldr r4, [r2, #0x18]
	ldrb r1, [r4, #0x10]
	lsls r0, r1, #6
	movs r2, #0x3f
	ldrb r3, [r6]
	ands r2, r3
	orrs r2, r0
	strb r2, [r6]
	movs r3, #0x1f
	ldrb r5, [r4, #0x12]
	ands r5, r3
	movs r1, #0x20
	rsbs r1, r1, #0
	adds r0, r1, #0
	ldrb r7, [r6, #2]
	ands r0, r7
	orrs r0, r5
	strb r0, [r6, #2]
	ldrb r4, [r4, #0x13]
	ands r3, r4
	ldrb r0, [r6, #3]
	ands r1, r0
	orrs r1, r3
	strb r1, [r6, #3]
	movs r0, #8
	orrs r2, r0
	strb r2, [r6]
	movs r0, #1
	ldrb r1, [r6, #1]
	orrs r0, r1
	movs r1, #2
	orrs r0, r1
	movs r1, #4
	orrs r0, r1
	movs r1, #0x10
	orrs r0, r1
	b _08024190
	.align 2, 0
_08024154: .4byte gUnknown_03001280
_08024158: .4byte gUnknown_03001308
_0802415C:
	movs r2, #0x3f
	ldrb r3, [r6]
	ands r2, r3
	movs r0, #0x20
	rsbs r0, r0, #0
	adds r1, r0, #0
	ldrb r7, [r6, #2]
	ands r1, r7
	movs r3, #0x10
	orrs r1, r3
	strb r1, [r6, #2]
	ldrb r1, [r6, #3]
	ands r0, r1
	orrs r0, r3
	strb r0, [r6, #3]
	movs r0, #8
	orrs r2, r0
	strb r2, [r6]
	movs r0, #1
	ldrb r7, [r6, #1]
	orrs r0, r7
	movs r1, #2
	orrs r0, r1
	movs r1, #4
	orrs r0, r1
	orrs r0, r3
_08024190:
	strb r0, [r6, #1]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0


.endif
