.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8006518 is reconstructed (but not yet byte-matching) as C in
@ src/graphics/settings_menu10.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-8-0x080060ac-overlay-ui.md.
.if NON_MATCHING == 0
	thumb_func_start sub_8006518
sub_8006518: @ 0x08006518
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #0x24
	movs r0, #0x1f
	ldrb r1, [r6]
	ands r0, r1
	cmp r0, #0
	beq _08006564
	movs r2, #0x20
	rsbs r2, r2, #0
	adds r7, r2, #0
_08006534:
	adds r4, r6, #0
	ldrb r2, [r6]
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1b
	subs r0, #1
	movs r1, #0x1f
	ands r0, r1
	ands r2, r7
	orrs r2, r0
	strb r2, [r6]
	adds r0, r5, #0
	bl sub_8006600
	adds r0, r5, #0
	bl sub_8006714
	adds r0, r5, #0
	bl sub_8006700
	movs r0, #0x1f
	ldrb r4, [r4]
	ands r0, r4
	cmp r0, #0
	bne _08006534
_08006564:
	adds r4, r5, #0
	adds r4, #0x24
	movs r0, #0x28
	adds r0, r0, r5
	mov r8, r0
_0800656E:
	adds r0, r5, #0
	bl sub_8006600
	adds r0, r5, #0
	bl sub_8006714
	adds r0, r5, #0
	bl sub_8006700
	ldr r0, _080065F8 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r1, _080065FC @ =gUnknown_030007E0
	movs r0, #8
	ldrh r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _0800656E
	adds r6, r4, #0
	movs r0, #0x1f
	ldrb r1, [r6]
	ands r0, r1
	cmp r0, #0x10
	beq _080065D6
	movs r2, #0x20
	rsbs r2, r2, #0
	adds r7, r2, #0
_080065A6:
	adds r4, r6, #0
	ldrb r2, [r6]
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1b
	adds r0, #1
	movs r1, #0x1f
	ands r0, r1
	ands r2, r7
	orrs r2, r0
	strb r2, [r6]
	adds r0, r5, #0
	bl sub_8006600
	adds r0, r5, #0
	bl sub_8006714
	adds r0, r5, #0
	bl sub_8006700
	movs r0, #0x1f
	ldrb r4, [r4]
	ands r0, r4
	cmp r0, #0x10
	bne _080065A6
_080065D6:
	movs r0, #0
	strh r0, [r5, #0x28]
	movs r0, #0x40
	mov r1, r8
	ldrb r1, [r1]
	orrs r0, r1
	mov r2, r8
	strb r0, [r2]
	adds r0, r5, #0
	bl sub_8006714
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080065F8: .4byte gUnknown_03001304
_080065FC: .4byte gUnknown_030007E0
.endif
