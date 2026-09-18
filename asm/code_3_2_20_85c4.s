.include "asm/macros.inc"

.syntax unified
.arm

@ sub_80285C4/InitHudIconWidgetA/InitHudIconWidgetB are reconstructed
@ (semantics fully understood, but not yet byte-matching) as C in
@ src/graphics/hud_icon_widget_85c4.c, guarded by #if NON_MATCHING - see
@ docs/matching/issue-46-hud-icon-widget.md for the exact remaining gap
@ in each (excess-register-pressure/gcc-2.9 register-allocation issues,
@ including a categorical r7-pinning toolchain bug documented in
@ docs/matching.md's "Why not just pin r7"). sub_8028808, formerly here
@ too, is matched and lives at the end of the same .c file now.
.if NON_MATCHING == 0
	thumb_func_start sub_80285C4
sub_80285C4: @ 0x080285C4
	push {r4, r5, r6, r7, lr}
	adds r3, r0, #0
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	adds r0, #8
	adds r0, r0, r1
	ldrb r2, [r0]
	movs r0, #0x88
	lsls r0, r0, #1
	adds r6, r3, r0
	ldr r1, [r6]
	ldr r4, _08028658 @ =0x000001FF
	adds r0, r4, #0
	ands r1, r0
	ldr r0, _0802865C @ =0xFFFFFE00
	ldrh r7, [r3, #2]
	ands r0, r7
	orrs r0, r1
	strh r0, [r3, #2]
	movs r0, #0x8a
	lsls r0, r0, #1
	adds r1, r3, r0
	subs r4, #0xf3
	adds r5, r3, r4
	ldr r0, [r5]
	lsls r4, r2, #1
	adds r4, r4, r2
	lsls r4, r4, #2
	adds r0, r4, r0
	ldrb r0, [r0, #8]
	ldrb r1, [r1]
	adds r0, r0, r1
	strb r0, [r3]
	ldr r0, [r5]
	adds r0, r4, r0
	ldr r1, [r0, #4]
	lsls r1, r1, #6
	movs r0, #0x3f
	ldrb r7, [r3, #1]
	ands r0, r7
	orrs r0, r1
	strb r0, [r3, #1]
	movs r1, #0x84
	lsls r1, r1, #1
	adds r0, r3, r1
	ldr r1, [r0]
	movs r7, #0x92
	lsls r7, r7, #1
	adds r0, r3, r7
	ldr r0, [r0]
	muls r0, r2, r0
	adds r1, r1, r0
	ldr r2, _08028660 @ =0x000003FF
	adds r0, r2, #0
	ands r1, r0
	ldr r0, _08028664 @ =0xFFFFFC00
	ldrh r7, [r3, #4]
	ands r0, r7
	orrs r0, r1
	strh r0, [r3, #4]
	ldr r0, _08028668 @ =gUnknown_03001300
	ldr r0, [r0]
	adds r1, r3, #0
	bl sub_8006AC8
	ldr r0, [r5]
	adds r4, r4, r0
	ldr r0, [r6]
	ldr r1, [r4]
	adds r0, r0, r1
	str r0, [r6]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08028658: .4byte 0x000001FF
_0802865C: .4byte 0xFFFFFE00
_08028660: .4byte 0x000003FF
_08028664: .4byte 0xFFFFFC00
_08028668: .4byte gUnknown_03001300

	thumb_func_start InitHudIconWidgetA
InitHudIconWidgetA: @ 0x0802866C
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r4, r0, #0
	movs r0, #0x98
	lsls r0, r0, #1
	adds r1, r4, r0
	ldr r0, _0802871C @ =gStaticData_087E4DAC
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
	mov r0, sp
	adds r1, r4, #0
	ldr r2, _08028720 @ =0x05000002
	bl sub_803A94C
	movs r0, #0x98
	lsls r0, r0, #1
	adds r1, r4, r0
	ldr r0, _08028724 @ =gStaticData_087E4D64
	str r0, [r1]
	movs r2, #0x8e
	lsls r2, r2, #1
	adds r1, r4, r2
	movs r0, #9
	str r0, [r1]
	movs r0, #0x90
	lsls r0, r0, #1
	adds r1, r4, r0
	movs r0, #4
	str r0, [r1]
	adds r2, #0xc
	adds r1, r4, r2
	ldr r0, _08028728 @ =gStaticData_085A4E70
	str r0, [r1]
	movs r0, #0x92
	lsls r0, r0, #1
	adds r1, r4, r0
	movs r0, #2
	str r0, [r1]
	subs r2, #0x1c
	adds r1, r4, r2
	ldr r0, _0802872C @ =gStaticData_08174DD4
	str r0, [r1]
	movs r2, #0
	adds r5, r4, #0
	adds r5, #8
	ldr r6, _08028730 @ =gStaticData_08174D84
	ldrb r7, [r6]
	mov ip, r2
_080286EA:
	adds r0, r5, r2
	mov r1, ip
	strb r1, [r0]
	movs r1, #0
	adds r3, r2, #1
	cmp r7, r2
	beq _08028708
_080286F8:
	adds r1, #1
	cmp r1, #0x4f
	bhi _0802870A
	adds r0, r1, r6
	ldrb r0, [r0]
	cmp r0, r2
	bne _080286F8
	adds r0, r5, r2
_08028708:
	strb r1, [r0]
_0802870A:
	adds r2, r3, #0
	cmp r2, #0xff
	bls _080286EA
	adds r0, r4, #0
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0802871C: .4byte gStaticData_087E4DAC
_08028720: .4byte 0x05000002
_08028724: .4byte gStaticData_087E4D64
_08028728: .4byte gStaticData_085A4E70
_0802872C: .4byte gStaticData_08174DD4
_08028730: .4byte gStaticData_08174D84

	thumb_func_start InitHudIconWidgetB
InitHudIconWidgetB: @ 0x08028734
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r4, r0, #0
	movs r0, #0x98
	lsls r0, r0, #1
	adds r1, r4, r0
	ldr r0, _080287F0 @ =gStaticData_087E4DAC
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
	mov r0, sp
	adds r1, r4, #0
	ldr r2, _080287F4 @ =0x05000002
	bl sub_803A94C
	movs r0, #0x98
	lsls r0, r0, #1
	adds r1, r4, r0
	ldr r0, _080287F8 @ =gStaticData_087E4D1C
	str r0, [r1]
	movs r2, #0x8e
	lsls r2, r2, #1
	adds r1, r4, r2
	movs r0, #0x10
	str r0, [r1]
	movs r0, #0x90
	lsls r0, r0, #1
	adds r1, r4, r0
	movs r0, #6
	str r0, [r1]
	adds r2, #8
	adds r1, r4, r2
	movs r0, #4
	str r0, [r1]
	movs r0, #0x86
	lsls r0, r0, #1
	adds r1, r4, r0
	ldr r0, _080287FC @ =gStaticData_081751D4
	str r0, [r1]
	adds r2, #4
	adds r1, r4, r2
	ldr r0, _08028800 @ =gStaticData_085A551C
	str r0, [r1]
	movs r0, #0x3f
	ldrb r1, [r4, #3]
	ands r0, r1
	movs r1, #0x40
	orrs r0, r1
	strb r0, [r4, #3]
	movs r2, #0
	adds r5, r4, #0
	adds r5, #8
	ldr r6, _08028804 @ =gStaticData_08175188
	ldrb r7, [r6]
	mov ip, r2
_080287BE:
	adds r0, r5, r2
	mov r1, ip
	strb r1, [r0]
	movs r1, #0
	adds r3, r2, #1
	cmp r7, r2
	beq _080287DC
_080287CC:
	adds r1, #1
	cmp r1, #0x4b
	bhi _080287DE
	adds r0, r1, r6
	ldrb r0, [r0]
	cmp r0, r2
	bne _080287CC
	adds r0, r5, r2
_080287DC:
	strb r1, [r0]
_080287DE:
	adds r2, r3, #0
	cmp r2, #0xff
	bls _080287BE
	adds r0, r4, #0
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080287F0: .4byte gStaticData_087E4DAC
_080287F4: .4byte 0x05000002
_080287F8: .4byte gStaticData_087E4D1C
_080287FC: .4byte gStaticData_081751D4
_08028800: .4byte gStaticData_085A551C
_08028804: .4byte gStaticData_08175188

.endif
