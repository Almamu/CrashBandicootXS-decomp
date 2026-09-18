.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8023A1C: left untouched (not yet confidently understood
@ branch-by-branch - a ~650-instruction jump-table-driven level-lifecycle
@ state machine with several still-raw callees and gStaticData_0816C8xx
@ tables). Its caller sub_802375C is now matched, see
@ src/system/game_loop39.c and docs/matching/issue-37-game-loop-2375c.md.
	thumb_func_start sub_8023A1C
sub_8023A1C: @ 0x08023A1C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x18
	adds r7, r0, #0
	movs r0, #1
	mov sl, r0
	ldr r4, _08023AC4 @ =gUnknown_030012D8
	ldr r0, [r4]
	bl sub_800A810
	ldr r0, _08023AC8 @ =gUnknown_030012D4
	ldr r1, [r0]
	ldr r0, [r4]
	str r0, [r1, #0x10]
	mov r2, sl
	str r2, [r1, #0x14]
	ldr r0, _08023ACC @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r1, [r7, #0x18]
	bl sub_80266BC
	ldr r5, _08023AD0 @ =gStaticData_0816C86C
	ldr r1, [r7]
	lsls r0, r1, #3
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r5
	ldrb r0, [r0, #0x1c]
	cmp r0, #0
	bne _08023A66
	ldr r0, _08023AD4 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023484
_08023A66:
	ldr r4, _08023AD4 @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_80232C8
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08023A78
	bl sub_800F1B8
_08023A78:
	ldr r0, [r4]
	ldr r2, [r7]
	lsls r1, r2, #3
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r2, r5, #0
	adds r2, #0x14
	adds r1, r1, r2
	ldr r1, [r1]
	bl sub_8023118
	ldr r0, [r4]
	ldr r2, [r7]
	lsls r1, r2, #3
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r2, r5, #0
	adds r2, #0x18
	adds r1, r1, r2
	ldr r1, [r1]
	bl sub_8023110
	ldr r1, [r7]
	lsls r0, r1, #3
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r1, r5, #4
	adds r0, r0, r1
	ldr r0, [r0]
	subs r0, #1
	cmp r0, #5
	bls _08023ABA
	b _08023BC4
_08023ABA:
	lsls r0, r0, #2
	ldr r1, _08023AD8 @ =_08023ADC
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08023AC4: .4byte gUnknown_030012D8
_08023AC8: .4byte gUnknown_030012D4
_08023ACC: .4byte gUnknown_03001308
_08023AD0: .4byte gStaticData_0816C86C
_08023AD4: .4byte gUnknown_030012C0
_08023AD8: .4byte _08023ADC
_08023ADC: @ jump table
	.4byte _08023B20 @ case 0
	.4byte _08023AF4 @ case 1
	.4byte _08023B60 @ case 2
	.4byte _08023BC4 @ case 3
	.4byte _08023B8C @ case 4
	.4byte _08023B20 @ case 5
_08023AF4:
	ldr r4, _08023B18 @ =gUnknown_030012C8
	ldr r0, [r4]
	bl sub_8027088
	ldr r0, [r4]
	movs r1, #0xa0
	lsls r1, r1, #0x13
	ldr r2, _08023B1C @ =gStaticData_0816C814
	movs r3, #5
	str r3, [sp]
	add r4, sp, #4
	movs r3, #1
	strb r3, [r4]
	movs r3, #6
	bl sub_8027018
	b _08023BCC
	.align 2, 0
_08023B18: .4byte gUnknown_030012C8
_08023B1C: .4byte gStaticData_0816C814
_08023B20:
	ldr r4, _08023B54 @ =gUnknown_030012C8
	ldr r0, [r4]
	bl sub_8027088
	ldr r0, [r4]
	movs r3, #0xa0
	lsls r3, r3, #0x13
	mov sb, r3
	ldr r2, _08023B58 @ =gStaticData_0816C81E
	movs r5, #9
	mov r8, r5
	str r5, [sp]
	add r6, sp, #4
	movs r5, #0
	strb r5, [r6]
	mov r1, sb
	movs r3, #0x10
	bl sub_8027018
	ldr r0, [r4]
	ldr r2, _08023B5C @ =gStaticData_0816C830
	mov r1, r8
	str r1, [sp]
	strb r5, [r6]
	mov r1, sb
	b _08023BA6
	.align 2, 0
_08023B54: .4byte gUnknown_030012C8
_08023B58: .4byte gStaticData_0816C81E
_08023B5C: .4byte gStaticData_0816C830
_08023B60:
	ldr r4, _08023B84 @ =gUnknown_030012C8
	ldr r0, [r4]
	bl sub_8027088
	ldr r0, [r4]
	movs r1, #0xa0
	lsls r1, r1, #0x13
	ldr r2, _08023B88 @ =gStaticData_0816C842
	movs r3, #0x10
	str r3, [sp]
	add r4, sp, #4
	movs r3, #0
	strb r3, [r4]
	movs r3, #0xa
	bl sub_8027018
	b _08023BCC
	.align 2, 0
_08023B84: .4byte gUnknown_030012C8
_08023B88: .4byte gStaticData_0816C842
_08023B8C:
	ldr r4, _08023BB0 @ =gUnknown_030012C8
	ldr r0, [r4]
	bl sub_8027088
	ldr r0, [r4]
	movs r1, #0xa0
	lsls r1, r1, #0x13
	ldr r2, _08023BB4 @ =gStaticData_0816C862
	movs r3, #5
	str r3, [sp]
	add r4, sp, #4
	movs r3, #0
	strb r3, [r4]
_08023BA6:
	movs r3, #0x14
	bl sub_8027018
	b _08023BCC
	.align 2, 0
_08023BB0: .4byte gUnknown_030012C8
_08023BB4: .4byte gStaticData_0816C862
_08023BB8:
	movs r2, #1
	mov sl, r2
	b _08023E72
_08023BBE:
	movs r3, #2
	mov sl, r3
	b _08023E72
_08023BC4:
	ldr r0, _08023D4C @ =gUnknown_030012C8
	ldr r1, [r0]
	movs r0, #0
	strb r0, [r1]
_08023BCC:
	adds r0, r7, #0
	bl sub_80240E4
	bl sub_802423C
	ldr r0, [r7, #0x18]
	ldr r0, [r0, #8]
	cmp r0, #1
	bne _08023C06
	ldr r0, _08023D50 @ =gUnknown_030012D8
	ldr r4, [r0]
	movs r0, #0x1f
	adds r1, r4, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, _08023D54 @ =gUnknown_030012D4
	ldr r1, [r0]
	movs r0, #2
	str r0, [r1, #0x14]
_08023C06:
	ldr r4, _08023D50 @ =gUnknown_030012D8
	ldr r0, [r4]
	bl sub_800815C
	ldr r2, [r4]
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r5, [r2]
	ands r1, r5
	orrs r1, r0
	strb r1, [r2]
	ldr r0, _08023D58 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r3, [r4]
	adds r1, r3, #0
	adds r1, #0x29
	ldrb r1, [r1]
	lsls r1, r1, #0x1c
	lsrs r1, r1, #0x1c
	ldr r2, [r3, #0x20]
	adds r3, #0x2d
	ldr r4, [r2]
	ldrb r5, [r3]
	lsls r2, r5, #3
	subs r2, r2, r5
	lsls r2, r2, #2
	adds r2, r2, r4
	ldrb r2, [r2, #0x14]
	bl sub_8006D08
	ldr r0, _08023D54 @ =gUnknown_030012D4
	ldr r0, [r0]
	bl sub_8026DFC
	ldr r0, _08023D5C @ =gUnknown_03001308
	ldr r0, [r0]
	bl sub_8026984
	ldr r0, [r7, #0x18]
	ldr r0, [r0, #8]
	cmp r0, #0
	bne _08023D0C
	ldr r4, _08023D60 @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_80232B8
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08023C7A
	adds r0, r7, #0
	bl sub_8024404
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08023C92
_08023C7A:
	ldr r0, [r4]
	bl sub_8023290
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08023D0C
	adds r0, r7, #0
	bl sub_80243E0
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08023D0C
_08023C92:
	ldr r5, _08023D50 @ =gUnknown_030012D8
	ldr r1, [r5]
	movs r0, #0x7f
	ldrb r2, [r1, #0xc]
	ands r0, r2
	strb r0, [r1, #0xc]
	ldr r4, [r5]
	movs r0, #0x29
	adds r1, r4, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, _08023D64 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x2c
	bl PlaySfx
	ldr r0, [r5]
	ldr r0, [r0, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #0x29
	bl sub_803AD80
	ldr r0, _08023D58 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r3, [r5]
	adds r1, r3, #0
	adds r1, #0x29
	ldrb r1, [r1]
	lsls r1, r1, #0x1c
	lsrs r1, r1, #0x1c
	ldr r2, [r3, #0x20]
	adds r3, #0x2d
	ldr r4, [r2]
	ldrb r5, [r3]
	lsls r2, r5, #3
	subs r2, r2, r5
	lsls r2, r2, #2
	adds r2, r2, r4
	ldrb r2, [r2, #0x14]
	bl sub_8006D08
	ldr r0, _08023D68 @ =gUnknown_03001318
	ldr r0, [r0]
	bl sub_8028504
_08023D0C:
	ldr r0, _08023D6C @ =gUnknown_030012F4
	ldr r0, [r0]
	bl sub_8008C80
	ldr r0, _08023D70 @ =gUnknown_030012EC
	ldr r0, [r0]
	bl sub_8008C80
	ldr r0, _08023D74 @ =gUnknown_030012F0
	ldr r0, [r0]
	bl sub_8008C80
	ldr r0, _08023D78 @ =gUnknown_030012F8
	ldr r0, [r0]
	bl sub_8008C80
	adds r0, r7, #0
	bl sub_802400C
	movs r0, #0
	bl sub_8001524
	bl sub_8001604
	bl sub_80015E0
	bl sub_8001614
	bl sub_8001624
	b _08023E5A
	.align 2, 0
_08023D4C: .4byte gUnknown_030012C8
_08023D50: .4byte gUnknown_030012D8
_08023D54: .4byte gUnknown_030012D4
_08023D58: .4byte gUnknown_030012B8
_08023D5C: .4byte gUnknown_03001308
_08023D60: .4byte gUnknown_030012C0
_08023D64: .4byte gUnknown_030012BC
_08023D68: .4byte gUnknown_03001318
_08023D6C: .4byte gUnknown_030012F4
_08023D70: .4byte gUnknown_030012EC
_08023D74: .4byte gUnknown_030012F0
_08023D78: .4byte gUnknown_030012F8
_08023D7C:
	bl sub_802423C
	adds r0, r7, #0
	bl sub_802400C
	ldr r5, _08023ED4 @ =gUnknown_03001304
	ldr r0, [r5]
	bl sub_80007AC
	ldr r0, [r4]
	movs r1, #0x82
	lsls r1, r1, #1
	adds r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0
	bne _08023DCA
	ldr r1, _08023ED8 @ =gUnknown_030007E0
	movs r0, #8
	ldrh r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _08023DCA
	bl sub_8004D74
	adds r4, r0, #0
	cmp r4, #0
	bne _08023DBE
	adds r0, r7, #0
	bl sub_80241BC
	ldr r0, [r5]
	bl sub_80007AC
_08023DBE:
	cmp r4, #1
	bne _08023DC4
	b _08023BB8
_08023DC4:
	cmp r4, #2
	bne _08023DCA
	b _08023BBE
_08023DCA:
	ldr r0, _08023ED8 @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #4
	ands r0, r1
	cmp r0, #0
	beq _08023DDE
	ldr r0, _08023EDC @ =gUnknown_03001318
	ldr r0, [r0]
	bl sub_8028504
_08023DDE:
	ldr r0, _08023EE0 @ =gUnknown_030012F4
	ldr r0, [r0]
	bl sub_800891C
	ldr r0, _08023EE4 @ =gUnknown_030012E8
	ldr r0, [r0]
	bl sub_800891C
	ldr r4, _08023EE8 @ =gUnknown_030012D8
	ldr r0, [r4]
	ldr r2, [r0, #0x18]
	movs r3, #0x38
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r2, #0x3c]
	bl sub_803AD7C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08023E16
	ldr r0, [r4]
	ldr r2, [r0, #0x18]
	movs r5, #0x18
	ldrsh r1, [r2, r5]
	adds r0, r0, r1
	ldr r1, [r2, #0x1c]
	bl sub_803AD7C
_08023E16:
	ldr r0, _08023EEC @ =gUnknown_0300130C
	ldr r0, [r0]
	bl sub_80091D4
	ldr r0, _08023EF0 @ =gUnknown_030012EC
	ldr r0, [r0]
	bl sub_800891C
	ldr r0, _08023EF4 @ =gUnknown_030012F0
	ldr r0, [r0]
	bl sub_800891C
	ldr r0, _08023EF8 @ =gUnknown_030012F8
	ldr r0, [r0]
	bl sub_800891C
	ldr r0, _08023EDC @ =gUnknown_03001318
	ldr r0, [r0]
	bl sub_8028400
	ldr r0, _08023EFC @ =gUnknown_030012C0
	ldr r1, [r0]
	adds r0, r1, #0
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _08023E52
	adds r0, r1, #0
	bl sub_8022F2C
_08023E52:
	ldr r1, _08023F00 @ =gUnknown_0300082C
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
_08023E5A:
	bl sub_80241B0
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08023E72
	ldr r4, _08023EE8 @ =gUnknown_030012D8
	ldr r1, [r4]
	movs r0, #1
	ldrb r1, [r1, #0xc]
	ands r0, r1
	cmp r0, #0
	beq _08023D7C
_08023E72:
	bl sub_80014A4
	bl sub_80241B0
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08023E82
	b _08023F92
_08023E82:
	movs r0, #0
	mov sl, r0
	adds r0, r7, #0
	bl sub_8024404
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08023F08
	ldr r6, _08023EFC @ =gUnknown_030012C0
	ldr r0, [r6]
	bl sub_80232B8
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08023F08
	ldr r0, [r6]
	bl sub_8023104
	ldr r0, [r0]
	ldr r1, _08023F04 @ =0xFFFFE200
	adds r4, r0, r1
	ldr r0, _08023EE8 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0, #4]
	movs r2, #0x90
	lsls r2, r2, #5
	adds r5, r0, r2
	str r4, [sp, #8]
	str r5, [sp, #0xc]
	ldr r4, [r6]
	adds r0, r4, #0
	bl sub_8023104
	bl sub_801B29C
	adds r1, r0, #0
	adds r0, r4, #0
	add r2, sp, #8
	bl sub_802356C
	b _08023F92
	.align 2, 0
_08023ED4: .4byte gUnknown_03001304
_08023ED8: .4byte gUnknown_030007E0
_08023EDC: .4byte gUnknown_03001318
_08023EE0: .4byte gUnknown_030012F4
_08023EE4: .4byte gUnknown_030012E8
_08023EE8: .4byte gUnknown_030012D8
_08023EEC: .4byte gUnknown_0300130C
_08023EF0: .4byte gUnknown_030012EC
_08023EF4: .4byte gUnknown_030012F0
_08023EF8: .4byte gUnknown_030012F8
_08023EFC: .4byte gUnknown_030012C0
_08023F00: .4byte gUnknown_0300082C
_08023F04: .4byte 0xFFFFE200
_08023F08:
	adds r0, r7, #0
	bl sub_80243E0
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08023F44
	ldr r4, _08023F3C @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_8023290
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08023F44
	ldr r2, _08023F40 @ =gUnknown_030012D8
	ldr r2, [r2]
	ldr r0, [r2]
	ldr r1, [r2, #4]
	str r0, [sp, #0x10]
	str r1, [sp, #0x14]
	ldr r0, [r4]
	add r2, sp, #0x10
	movs r1, #0
	bl sub_802356C
	b _08023F92
	.align 2, 0
_08023F3C: .4byte gUnknown_030012C0
_08023F40: .4byte gUnknown_030012D8
_08023F44:
	movs r7, #0
	movs r5, #0
	ldr r1, _08023FF0 @ =gUnknown_0300130C
	ldr r0, [r1]
	ldr r0, [r0]
	cmp r7, r0
	bge _08023F88
	adds r6, r1, #0
_08023F54:
	ldr r0, [r6]
	ldr r1, [r0, #8]
	lsls r0, r5, #2
	adds r0, r0, r1
	ldr r4, [r0]
	ldr r1, [r4, #0x18]
	adds r1, #0x48
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	cmp r0, #3
	bne _08023F7E
	adds r0, r4, #0
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #0xa
	bne _08023F7E
	adds r7, #1
_08023F7E:
	adds r5, #1
	ldr r0, [r6]
	ldr r0, [r0]
	cmp r5, r0
	blt _08023F54
_08023F88:
	ldr r0, _08023FF4 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r1, r7, #0
	bl sub_8023140
_08023F92:
	ldr r0, _08023FF8 @ =gUnknown_030012E8
	ldr r0, [r0]
	bl sub_8008CEC
	ldr r0, _08023FF0 @ =gUnknown_0300130C
	ldr r0, [r0]
	bl sub_8009914
	ldr r0, _08023FFC @ =gUnknown_030012EC
	ldr r0, [r0]
	bl sub_8008CEC
	ldr r0, _08024000 @ =gUnknown_030012F0
	ldr r0, [r0]
	bl sub_8008CEC
	ldr r0, _08024004 @ =gUnknown_030012F8
	ldr r0, [r0]
	bl sub_8008CEC
	ldr r0, _08024008 @ =gUnknown_030012F4
	ldr r0, [r0]
	bl sub_8008CEC
	bl sub_8001578
	bl sub_8001564
	bl sub_8001550
	bl sub_800153C
	bl sub_800158C
	bl sub_80006A8
	bl sub_8001614
	mov r0, sl
	add sp, #0x18
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08023FF0: .4byte gUnknown_0300130C
_08023FF4: .4byte gUnknown_030012C0
_08023FF8: .4byte gUnknown_030012E8
_08023FFC: .4byte gUnknown_030012EC
_08024000: .4byte gUnknown_030012F0
_08024004: .4byte gUnknown_030012F8
_08024008: .4byte gUnknown_030012F4

