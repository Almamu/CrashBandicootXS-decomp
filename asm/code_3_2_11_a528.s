.include "asm/macros.inc"

.syntax unified
.arm

@ sub_800A528/sub_800A590 are reconstructed (but not yet byte-matching) as
@ C in src/graphics/actor_part47.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-9-0x08007634-actor.md.
.if NON_MATCHING == 0
	thumb_func_start sub_800A528
sub_800A528: @ 0x0800A528
	push {r4, r5, lr}
	adds r4, r0, #0
	bl sub_8009FB0
	ldr r1, [r4, #0x18]
	movs r2, #0x10
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #0x14]
	bl sub_803AD7C
	adds r3, r0, #0
	ldr r1, [r4, #0x1c]
	cmp r1, r3
	beq _0800A586
	cmp r1, #0
	beq _0800A586
	adds r0, r4, #0
	adds r0, #0x68
	ldrb r0, [r0]
	cmp r0, #8
	bne _0800A56C
	movs r5, #2
	ldrsh r0, [r1, r5]
	ldrb r1, [r1, #5]
	adds r2, r1, r0
	movs r1, #2
	ldrsh r0, [r3, r1]
	ldrb r5, [r3, #5]
	adds r1, r5, r0
	cmp r2, r1
	beq _0800A586
	subs r1, r2, r1
	b _0800A57E
_0800A56C:
	cmp r0, #4
	bne _0800A586
	movs r2, #2
	ldrsh r0, [r1, r2]
	movs r5, #2
	ldrsh r1, [r3, r5]
	cmp r0, r1
	beq _0800A586
	subs r1, r0, r1
_0800A57E:
	lsls r1, r1, #8
	ldr r0, [r4, #4]
	adds r0, r0, r1
	str r0, [r4, #4]
_0800A586:
	str r3, [r4, #0x1c]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_800A590
sub_800A590: @ 0x0800A590
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r1, [r4, #0x18]
	movs r2, #0x10
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #0x14]
	bl sub_803AD7C
	adds r3, r0, #0
	ldr r1, [r4, #0x1c]
	cmp r1, r3
	beq _0800A5EA
	cmp r1, #0
	beq _0800A5EA
	adds r0, r4, #0
	adds r0, #0x68
	ldrb r0, [r0]
	cmp r0, #8
	bne _0800A5D0
	movs r5, #2
	ldrsh r0, [r1, r5]
	ldrb r1, [r1, #5]
	adds r2, r1, r0
	movs r1, #2
	ldrsh r0, [r3, r1]
	ldrb r5, [r3, #5]
	adds r1, r5, r0
	cmp r2, r1
	beq _0800A5EA
	subs r1, r2, r1
	b _0800A5E2
_0800A5D0:
	cmp r0, #4
	bne _0800A5EA
	movs r2, #2
	ldrsh r0, [r1, r2]
	movs r5, #2
	ldrsh r1, [r3, r5]
	cmp r0, r1
	beq _0800A5EA
	subs r1, r0, r1
_0800A5E2:
	lsls r1, r1, #8
	ldr r0, [r4, #4]
	adds r0, r0, r1
	str r0, [r4, #4]
_0800A5EA:
	str r3, [r4, #0x1c]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
.endif
