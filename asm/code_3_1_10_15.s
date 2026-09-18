.include "asm/macros.inc"

.syntax unified
.arm

@ sub_80062A8 is reconstructed (but not yet byte-matching) as C in
@ src/graphics/settings_menu14.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-8-0x080060ac-overlay-ui.md.
.if NON_MATCHING == 0
	thumb_func_start sub_80062A8
sub_80062A8: @ 0x080062A8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	mov sb, r0
	mov sl, r1
	adds r7, r2, #0
	movs r0, #0xc0
	lsls r0, r0, #0x18
	bl mem_free_bytes
	bl sub_80006A8
	movs r0, #0xa0
	lsls r0, r0, #0x13
	movs r1, #0
	strh r1, [r0]
	movs r0, #0x80
	lsls r0, r0, #0x13
	strh r1, [r0]
	ldr r1, _080063C8 @ =gUnknown_030012B8
	ldr r0, [r1]
	bl sub_8006EA8
	ldr r5, _080063CC @ =gUnknown_030012DC
	ldr r0, [r5]
	bl sub_8028A40
	ldr r2, _080063D0 @ =gUnknown_030012E0
	mov r8, r2
	ldr r0, [r2]
	bl sub_8028A40
	ldr r6, _080063D4 @ =gUnknown_030012FC
	ldr r0, [r6]
	movs r4, #0
	str r4, [r0, #8]
	bl sub_8006C4C
	ldr r0, [r6]
	bl sub_8006C4C
	ldr r0, [r5]
	movs r3, #0x84
	lsls r3, r3, #1
	adds r1, r0, r3
	str r4, [r1]
	movs r2, #0x98
	lsls r2, r2, #1
	adds r1, r0, r2
	ldr r1, [r1]
	adds r1, #0x40
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r1, [r1, #4]
	bl sub_803AD7C
	ldr r0, [r6]
	ldr r1, [r5]
	movs r2, #0x96
	lsls r2, r2, #1
	adds r1, r1, r2
	ldr r1, [r1]
	lsls r1, r1, #5
	bl sub_8006C58
	ldr r0, [r5]
	movs r3, #0x96
	lsls r3, r3, #1
	adds r0, r0, r3
	ldr r2, [r0]
	mov r1, r8
	ldr r0, [r1]
	subs r3, #0x24
	adds r1, r0, r3
	str r2, [r1]
	movs r2, #0x98
	lsls r2, r2, #1
	adds r1, r0, r2
	ldr r1, [r1]
	adds r1, #0x40
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r1, [r1, #4]
	bl sub_803AD7C
	ldr r0, [r6]
	mov r2, r8
	ldr r1, [r2]
	movs r3, #0x96
	lsls r3, r3, #1
	adds r1, r1, r3
	ldr r1, [r1]
	lsls r1, r1, #5
	bl sub_8006C58
	ldr r0, [r6]
	bl sub_8006C30
	movs r0, #0x2c
	bl sub_8026EDC
	adds r5, r0, #0
	mov r0, sb
	bl sub_8026F38
	adds r4, r0, #0
	mov r0, sl
	bl sub_8026F38
	adds r2, r0, #0
	adds r0, r5, #0
	adds r1, r4, #0
	adds r3, r7, #0
	bl sub_80063D8
	adds r4, r0, #0
	bl sub_8006518
	cmp r4, #0
	beq _080063A8
	adds r0, r4, #0
	movs r1, #3
	bl sub_8006770
_080063A8:
	ldr r1, _080063C8 @ =gUnknown_030012B8
	ldr r0, [r1]
	bl sub_8006EA8
	movs r0, #0xc0
	lsls r0, r0, #0x18
	bl mem_free_bytes
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080063C8: .4byte gUnknown_030012B8
_080063CC: .4byte gUnknown_030012DC
_080063D0: .4byte gUnknown_030012E0
_080063D4: .4byte gUnknown_030012FC
.endif
