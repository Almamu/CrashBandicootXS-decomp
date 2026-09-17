.include "asm/macros.inc"

.syntax unified
.arm

@ sub_800E494/sub_800E4E4 are reconstructed (but not yet
@ byte-matching) as C in src/system/game_loop7.c, guarded by
@ #if NON_MATCHING - this raw version is only assembled for the
@ default (matching) build. See
@ docs/matching/issue-12-physics-collision.md.
.if NON_MATCHING == 0
	thumb_func_start sub_800E494
sub_800E494: @ 0x0800E494
	push {r4, lr}
	adds r4, r0, #0
	b _0800E4B0
_0800E49A:
	adds r0, r1, #0
	adds r0, #0x4d
	movs r2, #0x7f
	ldrb r0, [r0]
	ands r2, r0
	cmp r2, #0
	bne _0800E4AE
	adds r0, r1, #0
	adds r0, #0x58
	strb r2, [r0]
_0800E4AE:
	adds r0, r1, #0
_0800E4B0:
	bl sub_801070C
	adds r1, r0, #0
	cmp r1, #0
	bne _0800E49A
	adds r0, r4, #0
	b _0800E4D4
_0800E4BE:
	adds r0, r1, #0
	adds r0, #0x4d
	movs r2, #0x7f
	ldrb r0, [r0]
	ands r2, r0
	cmp r2, #0
	bne _0800E4D2
	adds r0, r1, #0
	adds r0, #0x58
	strb r2, [r0]
_0800E4D2:
	adds r0, r1, #0
_0800E4D4:
	bl sub_8010708
	adds r1, r0, #0
	cmp r1, #0
	bne _0800E4BE
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_800E4E4
sub_800E4E4: @ 0x0800E4E4
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	adds r4, r1, #0
	ldr r5, [r4, #0xc]
	bl sub_801070C
	adds r2, r0, #0
	cmp r2, #0
	beq _0800E524
	movs r7, #1
_0800E4F8:
	adds r1, r2, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800E518
	adds r0, r2, #0
	adds r0, #0x58
	strb r7, [r0]
	ldr r0, [r4, #4]
	subs r0, r0, r5
	str r0, [r4, #4]
	ldr r0, [r4, #0xc]
	adds r0, r0, r5
	str r0, [r4, #0xc]
_0800E518:
	adds r0, r2, #0
	bl sub_801070C
	adds r2, r0, #0
	cmp r2, #0
	bne _0800E4F8
_0800E524:
	adds r0, r6, #0
	bl sub_8010708
	adds r2, r0, #0
	cmp r2, #0
	beq _0800E558
	movs r6, #1
_0800E532:
	adds r1, r2, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800E54C
	adds r0, r2, #0
	adds r0, #0x58
	strb r6, [r0]
	ldr r0, [r4, #0xc]
	adds r0, r0, r5
	str r0, [r4, #0xc]
_0800E54C:
	adds r0, r2, #0
	bl sub_8010708
	adds r2, r0, #0
	cmp r2, #0
	bne _0800E532
_0800E558:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
.endif
