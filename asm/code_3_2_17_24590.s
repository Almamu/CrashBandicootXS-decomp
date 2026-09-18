.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8024590 is reconstructed (but not yet byte-matching) as C in
@ src/system/game_loop35.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-38-sound-channel-family.md.
.if NON_MATCHING == 0
	thumb_func_start sub_8024590
sub_8024590: @ 0x08024590
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	ldr r6, _080245E8 @ =gUnknown_030012BC
	ldr r0, [r6]
	ldr r2, [r5]
	lsls r4, r1, #2
	adds r2, r4, r2
	ldr r1, [r2]
	ldr r1, [r1, #0x14]
	bl sub_8001B54
	ldr r0, [r6]
	bl sub_8001AB8
	ldr r1, [r5]
	adds r1, r4, r1
	ldr r2, [r1]
	ldr r1, [r2, #0x14]
	cmp r0, r1
	bne _080245EC
	ldr r1, [r2, #0x18]
	cmp r1, #0x63
	beq _080245C8
	ldr r0, [r6]
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
_080245C8:
	ldr r0, [r5]
	adds r0, r4, r0
	ldr r0, [r0]
	ldr r0, [r0, #8]
	movs r2, #0x80
	rsbs r2, r2, #0
	adds r1, r2, #0
	orrs r0, r1
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	movs r1, #1
	movs r2, #0
	bl sub_800132C
	b _08024636
	.align 2, 0
_080245E8: .4byte gUnknown_030012BC
_080245EC:
	ldr r0, [r2, #8]
	movs r2, #0x80
	rsbs r2, r2, #0
	adds r1, r2, #0
	orrs r0, r1
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	movs r1, #1
	movs r2, #0
	bl sub_800132C
	ldr r0, [r5]
	adds r0, r4, r0
	ldr r0, [r0]
	ldr r0, [r0, #0x18]
	cmp r0, #0x63
	beq _08024636
	adds r7, r6, #0
	adds r6, r4, #0
_08024612:
	ldr r0, [r7]
	bl sub_8001AB8
	ldr r2, [r5]
	adds r1, r4, r2
	ldr r1, [r1]
	ldr r1, [r1, #0x14]
	cmp r0, r1
	bne _08024612
	ldr r0, _0802463C @ =gUnknown_030012BC
	ldr r0, [r0]
	adds r1, r6, r2
	ldr r1, [r1]
	ldr r1, [r1, #0x18]
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
_08024636:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802463C: .4byte gUnknown_030012BC
.endif
