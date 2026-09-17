.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8028A78 is reconstructed (semantics fully understood, but not yet
@ byte-matching) as C in src/graphics/hud_icon_widget_8a78.c, guarded by
@ #if NON_MATCHING - see docs/matching/issue-46-hud-icon-widget.md for
@ the exact remaining gap (the `posX`/`posY` zero-init pair's address
@ computation vs. store order - this compiler computes the two field
@ addresses in ascending-offset order but emits their stores in a
@ different order, a shape plain C reordering/raw-offset casts couldn't
@ reproduce - same gap as InitHudIconWidgetA/InitHudIconWidgetB in
@ asm/code_3_2_20_85c4.s, which share this exact preamble).
.if NON_MATCHING == 0
	thumb_func_start sub_8028A78
sub_8028A78: @ 0x08028A78
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	movs r0, #0x98
	lsls r0, r0, #1
	adds r1, r4, r0
	ldr r0, _08028ABC @ =gStaticData_087E4DAC
	str r0, [r1]
	movs r1, #0x88
	lsls r1, r1, #1
	adds r2, r4, r1
	adds r1, #4
	adds r0, r4, r1
	movs r1, #0
	str r1, [r0]
	str r1, [r2]
	movs r2, #0x8c
	lsls r2, r2, #1
	adds r0, r4, r2
	str r1, [r0]
	adds r2, #0x14
	adds r0, r4, r2
	str r1, [r0]
	str r1, [sp]
	ldr r2, _08028AC0 @ =0x05000002
	mov r0, sp
	adds r1, r4, #0
	bl sub_803A94C
	adds r0, r4, #0
	add sp, #4
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08028ABC: .4byte gStaticData_087E4DAC
_08028AC0: .4byte 0x05000002
.endif
