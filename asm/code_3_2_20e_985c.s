.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_803985C
sub_803985C: @ 0x0803985C
	push {r4, lr}
	mov ip, r0
	adds r4, r2, #0
	cmp r4, #0
	beq _080398D2
	ldr r1, [r3, #0x10]
	lsls r0, r4, #2
	adds r0, r0, r1
	ldr r0, [r0]
	mov r1, ip
	str r0, [r1, #0x3c]
	movs r1, #0
	movs r2, #0
	mov r3, ip
	strh r2, [r3, #0x38]
	mov r0, ip
	adds r0, #0x22
	strb r1, [r0]
	strh r2, [r3, #0x3a]
	ldr r0, [r3, #0x3c]
	ldrb r0, [r0, #8]
	adds r3, #0x23
	strb r0, [r3]
	mov r0, ip
	strh r2, [r0, #0x36]
	strb r1, [r0, #0x1f]
	adds r0, #0x20
	strb r1, [r0]
	movs r0, #0xff
	mov r1, ip
	strb r0, [r1, #0x15]
	ldr r1, [r1, #0x3c]
	adds r0, r1, #0
	adds r0, #0x84
	ldrb r0, [r0]
	mov r3, ip
	strb r0, [r3, #0x1e]
	strh r2, [r3, #0x32]
	strh r2, [r3, #0x30]
	ldrb r0, [r1]
	cmp r0, #0
	beq _080398B2
	str r2, [r3, #0x3c]
_080398B2:
	mov r1, ip
	ldr r0, [r1, #0x3c]
	cmp r0, #0
	beq _080398D2
	ldr r0, _080398D8 @ =gUnknown_03001630
	ldr r0, [r0]
	ldr r0, [r0, #4]
	ldr r1, [r0, #0x34]
	cmp r1, #0
	beq _080398D2
	mov r0, ip
	adds r0, #0x53
	ldrb r0, [r0]
	lsls r0, r0, #2
	adds r0, r0, r1
	strb r4, [r0]
_080398D2:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080398D8: .4byte gUnknown_03001630

	thumb_func_start sub_80398DC
sub_80398DC: @ 0x080398DC
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r3, r0, #0
	ldr r1, [r3, #0x3c]
	adds r0, r1, #0
	adds r0, #0x88
	ldrh r4, [r3, #0x36]
	lsls r2, r4, #3
	ldr r0, [r0]
	adds r0, r0, r2
	mov ip, r0
	adds r1, #0x85
	ldrb r1, [r1]
	cmp r4, r1
	blo _08039904
	movs r0, #0
	strb r0, [r3, #0x1e]
	b _08039A98
_08039904:
	adds r0, r4, #1
	strh r0, [r3, #0x36]
	mov r1, ip
	ldrb r0, [r1]
	cmp r0, #0
	beq _080399E2
	subs r0, #2
	lsls r0, r0, #5
	strh r0, [r3, #0x2a]
	ldrb r0, [r1, #1]
	adds r1, r3, #0
	adds r1, #0x21
	strb r0, [r1]
	mov r2, ip
	ldrb r0, [r2, #2]
	cmp r0, #0
	beq _080399E2
	subs r0, #1
	strb r0, [r3, #0x10]
	movs r0, #0
	str r0, [r3, #0x44]
	movs r0, #1
	mov r8, r0
	mov r1, r8
	strb r1, [r3, #0x11]
	movs r0, #0xff
	strb r0, [r3, #0x17]
	movs r0, #0
	strb r0, [r3, #0x12]
	ldr r2, [r3, #0x3c]
	ldrb r1, [r3, #0x10]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r5, r0, #2
	adds r6, r2, r5
	ldrb r0, [r6, #0xc]
	adds r4, r2, #0
	cmp r0, #0
	beq _080399CE
	adds r0, r4, #0
	adds r0, #0x14
	adds r0, r0, r5
	movs r2, #0x18
	adds r2, r2, r4
	mov sb, r2
	adds r1, r2, r5
	ldr r2, [r0]
	ldr r0, [r1]
	cmp r2, r0
	bge _080399CE
	adds r7, r4, #0
	adds r7, #0x1c
	adds r0, r7, r5
	ldr r0, [r0]
	cmp r0, #0
	ble _080399CE
	ldrh r0, [r6, #0x24]
	cmp r0, #0
	beq _080399CE
	adds r0, r4, #0
	adds r0, #0x20
	adds r0, r0, r5
	ldr r0, [r0]
	cmp r0, #0
	ble _080399CE
	mov r0, r8
	strb r0, [r3, #0x12]
	ldrb r0, [r3, #0x10]
	lsls r1, r0, #3
	subs r1, r1, r0
	lsls r1, r1, #2
	adds r0, r4, #0
	adds r0, #0x10
	adds r0, r0, r1
	ldr r2, [r0]
	str r2, [r3, #0x48]
	lsls r0, r2, #0xb
	str r0, [r3, #0x44]
	ldrb r1, [r3, #0x10]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r0, r4, r0
	ldrh r0, [r0, #0x24]
	strb r0, [r3, #0x14]
	mov r1, r8
	strb r1, [r3, #0x13]
	ldrb r1, [r3, #0x10]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r1, r7, r0
	ldr r1, [r1]
	adds r2, r2, r1
	add r0, sb
	ldr r0, [r0]
	cmp r2, r0
	ble _080399E2
	movs r0, #0xff
	strb r0, [r3, #0x13]
	b _080399E2
_080399CE:
	ldrb r0, [r3, #0x10]
	lsls r1, r0, #3
	subs r1, r1, r0
	lsls r1, r1, #2
	adds r0, r4, #0
	adds r0, #0x10
	adds r0, r0, r1
	ldr r0, [r0]
	lsls r0, r0, #0xb
	str r0, [r3, #0x44]
_080399E2:
	movs r0, #0
	strh r0, [r3, #0x1c]
	strh r0, [r3, #0x2c]
	movs r4, #0
_080399EA:
	lsls r0, r4, #1
	add r0, ip
	ldrh r0, [r0, #4]
	lsrs r1, r0, #8
	movs r2, #0xff
	ands r2, r0
	subs r0, r1, #1
	cmp r0, #0xe
	bhi _08039A92
	lsls r0, r0, #2
	ldr r1, _08039A08 @ =_08039A0C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08039A08: .4byte _08039A0C
_08039A0C: @ jump table
	.4byte _08039A48 @ case 0
	.4byte _08039A4C @ case 1
	.4byte _08039A92 @ case 2
	.4byte _08039A92 @ case 3
	.4byte _08039A52 @ case 4
	.4byte _08039A6A @ case 5
	.4byte _08039A92 @ case 6
	.4byte _08039A92 @ case 7
	.4byte _08039A92 @ case 8
	.4byte _08039A82 @ case 9
	.4byte _08039A86 @ case 10
	.4byte _08039A8C @ case 11
	.4byte _08039A92 @ case 12
	.4byte _08039A92 @ case 13
	.4byte _08039A90 @ case 14
_08039A48:
	strh r2, [r3, #0x2c]
	b _08039A92
_08039A4C:
	rsbs r0, r2, #0
	strh r0, [r3, #0x2c]
	b _08039A92
_08039A52:
	adds r1, r3, #0
	adds r1, #0x20
	ldrb r0, [r1]
	cmp r0, #0
	beq _08039A66
	subs r0, #1
	strb r0, [r1]
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08039A92
_08039A66:
	strh r2, [r3, #0x36]
	b _08039A92
_08039A6A:
	adds r0, r3, #0
	adds r0, #0x20
	ldrb r1, [r0]
	cmp r1, #0
	bne _08039A92
	cmp r2, #0
	beq _08039A7C
	adds r1, r2, #1
	b _08039A7E
_08039A7C:
	movs r1, #0
_08039A7E:
	strb r1, [r0]
	b _08039A92
_08039A82:
	strh r2, [r3, #0x1c]
	b _08039A92
_08039A86:
	rsbs r0, r2, #0
	strh r0, [r3, #0x1c]
	b _08039A92
_08039A8C:
	strb r2, [r3, #0x17]
	b _08039A92
_08039A90:
	strb r2, [r3, #0x1e]
_08039A92:
	adds r4, #1
	cmp r4, #1
	bls _080399EA
_08039A98:
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8039AA4
sub_8039AA4: @ 0x08039AA4
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x3c]
	cmp r0, #0
	beq _08039AC8
	ldr r1, [r0, #0x7c]
	adds r2, r4, #0
	adds r2, #0x38
	adds r0, r4, #0
	bl sub_8039F30
	strb r0, [r4, #0x16]
	adds r0, r4, #0
	bl sub_8039FFC
	adds r0, r4, #0
	bl sub_803A03C
_08039AC8:
	movs r1, #0x1a
	ldrsh r0, [r4, r1]
	ldrb r2, [r4, #0x15]
	adds r0, r0, r2
	cmp r0, #0xff
	ble _08039AD6
	movs r0, #0xff
_08039AD6:
	cmp r0, #0
	bge _08039ADC
	movs r0, #0
_08039ADC:
	movs r6, #0
	strb r0, [r4, #0x15]
	movs r3, #0x1c
	ldrsh r0, [r4, r3]
	ldrb r5, [r4, #0x17]
	adds r0, r0, r5
	cmp r0, #0xff
	ble _08039AEE
	movs r0, #0xff
_08039AEE:
	cmp r0, #0
	bge _08039AF4
	movs r0, #0
_08039AF4:
	strb r0, [r4, #0x17]
	ldrh r0, [r4, #0x28]
	ldrh r1, [r4, #0x26]
	adds r5, r0, r1
	strh r5, [r4, #0x26]
	ldrh r0, [r4, #0x2c]
	ldrh r2, [r4, #0x2a]
	adds r0, r0, r2
	strh r0, [r4, #0x2a]
	ldrh r1, [r4, #0x32]
	movs r3, #0x32
	ldrsh r0, [r4, r3]
	cmp r0, #0
	beq _08039B3C
	movs r0, #0x30
	ldrsh r2, [r4, r0]
	movs r3, #0x26
	ldrsh r0, [r4, r3]
	subs r2, r2, r0
	movs r3, #0x80
	lsls r3, r3, #0x18
	ands r2, r3
	adds r0, r5, r1
	strh r0, [r4, #0x26]
	movs r5, #0x30
	ldrsh r0, [r4, r5]
	movs r5, #0x26
	ldrsh r1, [r4, r5]
	subs r0, r0, r1
	ands r0, r3
	cmp r2, r0
	beq _08039B3C
	strh r6, [r4, #0x32]
	ldrh r0, [r4, #0x30]
	strh r0, [r4, #0x26]
	strh r6, [r4, #0x30]
_08039B3C:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8039B44
sub_8039B44: @ 0x08039B44
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x5c
	adds r6, r0, #0
	adds r4, r1, #0
	str r2, [sp, #0x50]
	ldr r0, [sp, #0x80]
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	ldr r3, [r6, #0x3c]
	cmp r3, #0
	beq _08039B8C
	movs r0, #0x2a
	ldrsh r1, [r6, r0]
	ldr r0, _08039B90 @ =0xFFFF8AD0
	cmp r1, r0
	beq _08039B8C
	ldrb r0, [r6, #0x10]
	cmp r0, #3
	bhi _08039B8C
	ldrb r2, [r6, #0x10]
	adds r0, r3, #1
	adds r0, r0, r2
	ldrb r0, [r0]
	lsls r0, r0, #3
	ldr r1, [sp, #0x7c]
	ldr r1, [r1, #0x14]
	adds r1, r1, r0
	str r1, [sp, #0x54]
	ldr r0, [r1]
	mov sb, r2
	cmp r0, #0
	bne _08039B94
_08039B8C:
	movs r0, #0
	b _08039F1E
	.align 2, 0
_08039B90: .4byte 0xFFFF8AD0
_08039B94:
	movs r1, #0x2a
	ldrsh r0, [r6, r1]
	movs r2, #0x2e
	ldrsh r1, [r6, r2]
	adds r2, r0, r1
	adds r0, r6, #0
	adds r0, #0x21
	ldrb r0, [r0]
	cmp r0, #0
	bne _08039BC8
	movs r3, #0x26
	ldrsh r0, [r6, r3]
	adds r2, r2, r0
	cmp r5, #0
	bne _08039BC8
	ldr r1, [r6]
	movs r3, #0x14
	ldrsh r0, [r4, r3]
	ldr r1, [r1, #0x18]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #2]
	lsls r0, r0, #0x18
	asrs r0, r0, #0x18
	lsls r0, r0, #5
	adds r2, r2, r0
_08039BC8:
	ldr r0, [r6, #0x3c]
	mov r3, sb
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	adds r1, r0, r1
	movs r3, #0x26
	ldrsh r1, [r1, r3]
	ldr r3, _08039CB8 @ =gStaticData_085A62DC
	adds r1, r2, r1
	ldr r2, _08039CBC @ =0x00000EF3
	mov r8, r0
	cmp r1, r2
	bls _08039BE6
	adds r1, r2, #0
_08039BE6:
	lsls r0, r1, #2
	adds r0, r0, r3
	ldr r1, [r0]
	ldrb r0, [r6, #0x16]
	movs r7, #0x80
	lsls r7, r7, #1
	cmp r0, #0xff
	beq _08039BF8
	adds r7, r0, #0
_08039BF8:
	ldrb r0, [r6, #0x17]
	cmp r0, #0xff
	beq _08039C02
	muls r0, r7, r0
	lsrs r7, r0, #8
_08039C02:
	ldrb r0, [r6, #0x15]
	cmp r0, #0xff
	beq _08039C0C
	muls r0, r7, r0
	lsrs r7, r0, #8
_08039C0C:
	ldrb r0, [r6, #0x18]
	cmp r0, #0xff
	beq _08039C16
	muls r0, r7, r0
	lsrs r7, r0, #8
_08039C16:
	ldrb r0, [r4, #0x1f]
	cmp r0, #0xff
	beq _08039C20
	muls r0, r7, r0
	lsrs r7, r0, #8
_08039C20:
	cmp r5, #0
	bne _08039C2E
	ldr r0, [r4]
	ldr r0, [r0, #0x18]
	ldrh r0, [r0, #8]
	muls r0, r7, r0
	lsrs r7, r0, #8
_08039C2E:
	adds r4, r1, #0
	asrs r5, r1, #0x1f
	ldr r0, _08039CC0 @ =gUnknown_03001618
	ldr r2, [r0]
	ldr r3, [r0, #4]
	adds r1, r5, #0
	adds r0, r4, #0
	bl sub_8037ECC
	adds r4, r1, #0
	mov sl, r4
	ldr r4, [sp, #0x54]
	ldr r3, [r4, #4]
	movs r0, #0
	str r0, [sp, #0x58]
	mov r1, sb
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r2, r0, #2
	mov r4, r8
	adds r0, r4, r2
	ldrb r0, [r0, #0xc]
	cmp r0, #0
	bne _08039C76
	mov r1, r8
	adds r1, #0x14
	adds r1, r1, r2
	mov r0, r8
	adds r0, #0x18
	adds r0, r0, r2
	ldr r1, [r1]
	ldr r0, [r0]
	cmp r1, r0
	bge _08039C76
	movs r0, #1
	str r0, [sp, #0x58]
_08039C76:
	ldr r0, [r6, #4]
	ldrh r2, [r0, #4]
	ldr r1, [sp, #0x54]
	ldr r0, [r1]
	ldr r1, [r6, #0x44]
	mov r4, sp
	str r0, [sp, #0x28]
	ldr r0, [sp, #0x50]
	str r0, [sp, #0x2c]
	str r1, [sp, #0x30]
	lsls r0, r3, #0xb
	str r0, [sp, #0x34]
	str r2, [sp, #0x38]
	movs r0, #0
	str r0, [sp, #0x3c]
	str r7, [sp, #0x40]
	mov r1, sl
	str r1, [sp, #0x44]
	str r0, [sp, #0x48]
	ldrb r0, [r6, #0x12]
	cmp r0, #0
	beq _08039CC4
	ldr r1, [r6, #0x3c]
	ldrb r2, [r6, #0x10]
	lsls r0, r2, #3
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r1, #0x1c
	adds r1, r1, r0
	ldr r0, [r1]
	lsls r0, r0, #0xb
	b _08039CC6
	.align 2, 0
_08039CB8: .4byte gStaticData_085A62DC
_08039CBC: .4byte 0x00000EF3
_08039CC0: .4byte gUnknown_03001618
_08039CC4:
	movs r0, #0
_08039CC6:
	str r0, [sp, #0x4c]
	add r1, sp, #0x28
	adds r0, r4, #0
	movs r2, #0x28
	bl sub_800014C
	ldr r1, [r6, #4]
	ldr r0, [sp, #0x14]
	ldrh r1, [r1, #4]
	cmp r0, r1
	blo _08039CDE
	b _08039F18
_08039CDE:
	ldr r0, _08039D08 @ =gStaticData_0803A874
	ldr r7, _08039D0C @ =gStaticData_0803A818
	subs r0, r0, r7
	adds r5, r0, #2
_08039CE6:
	movs r0, #0x11
	ldrsb r0, [r6, r0]
	cmp r0, #0
	ble _08039DC4
	ldr r2, [sp, #0x58]
	cmp r2, #0
	beq _08039D10
	ldr r2, [r6, #0x3c]
	ldrb r1, [r6, #0x10]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r2, #0x18
	adds r2, r2, r0
	ldr r0, [r2]
	b _08039D30
	.align 2, 0
_08039D08: .4byte gStaticData_0803A874
_08039D0C: .4byte gStaticData_0803A818
_08039D10:
	ldrb r0, [r6, #0x12]
	cmp r0, #0
	beq _08039D2C
	ldr r2, [r6, #0x3c]
	ldrb r1, [r6, #0x10]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r2, #0x1c
	adds r2, r2, r0
	ldr r0, [r6, #0x48]
	ldr r1, [r2]
	adds r0, r0, r1
	b _08039D30
_08039D2C:
	ldr r3, [sp, #0x54]
	ldr r0, [r3, #4]
_08039D30:
	lsls r0, r0, #0xb
	str r0, [sp, #0xc]
	ldrb r0, [r6, #0xd]
	cmp r0, #0
	beq _08039D78
	movs r0, #0
	str r0, [sp, #0x20]
	ldr r3, _08039D68 @ =gUnknown_03001630
	ldr r1, [r3]
	lsrs r0, r5, #0x1f
	adds r0, r5, r0
	asrs r0, r0, #1
	ldr r2, [r1, #0x44]
	lsls r0, r0, #1
	adds r0, r0, r2
	ldr r4, _08039D6C @ =0x0000E082
	adds r1, r4, #0
	strh r1, [r0]
	ldr r0, _08039D70 @ =gStaticData_0803A884
	subs r0, r0, r7
	adds r0, #2
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	lsls r0, r0, #1
	adds r0, r0, r2
	ldr r2, _08039D74 @ =0x0000BAFF
	b _08039E3A
	.align 2, 0
_08039D68: .4byte gUnknown_03001630
_08039D6C: .4byte 0x0000E082
_08039D70: .4byte gStaticData_0803A884
_08039D74: .4byte 0x0000BAFF
_08039D78:
	adds r0, r6, #0
	adds r0, #0x52
	ldrb r0, [r0]
	str r0, [sp, #0x20]
	ldr r3, _08039DB0 @ =gUnknown_03001630
	ldr r2, [r3]
	ldr r0, _08039DB4 @ =gStaticData_0803A8B4
	subs r0, r0, r7
	adds r0, #2
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	ldr r2, [r2, #0x44]
	lsls r0, r0, #1
	adds r0, r0, r2
	ldr r4, _08039DB8 @ =0x0000E082
	adds r1, r4, #0
	strh r1, [r0]
	ldr r0, _08039DBC @ =gStaticData_0803A8C4
	subs r0, r0, r7
	adds r0, #2
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	lsls r0, r0, #1
	adds r0, r0, r2
	ldr r2, _08039DC0 @ =0x0000BAFF
	b _08039E3A
	.align 2, 0
_08039DB0: .4byte gUnknown_03001630
_08039DB4: .4byte gStaticData_0803A8B4
_08039DB8: .4byte 0x0000E082
_08039DBC: .4byte gStaticData_0803A8C4
_08039DC0: .4byte 0x0000BAFF
_08039DC4:
	ldr r2, [r6, #0x3c]
	ldrb r1, [r6, #0x10]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r2, #0x14
	adds r2, r2, r0
	ldr r3, [r2]
	lsls r0, r3, #0xb
	str r0, [sp, #0xc]
	ldrb r0, [r6, #0xd]
	cmp r0, #0
	beq _08039E08
	movs r0, #0
	str r0, [sp, #0x20]
	ldr r3, _08039DFC @ =gUnknown_03001630
	ldr r1, [r3]
	lsrs r0, r5, #0x1f
	adds r0, r5, r0
	asrs r0, r0, #1
	ldr r2, [r1, #0x44]
	lsls r0, r0, #1
	adds r0, r0, r2
	ldr r4, _08039E00 @ =0x0000E042
	adds r1, r4, #0
	strh r1, [r0]
	ldr r0, _08039E04 @ =gStaticData_0803A884
	b _08039E2A
	.align 2, 0
_08039DFC: .4byte gUnknown_03001630
_08039E00: .4byte 0x0000E042
_08039E04: .4byte gStaticData_0803A884
_08039E08:
	movs r0, #1
	str r0, [sp, #0x20]
	ldr r3, _08039E88 @ =gUnknown_03001630
	ldr r2, [r3]
	ldr r0, _08039E8C @ =gStaticData_0803A8B4
	subs r0, r0, r7
	adds r0, #2
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	ldr r2, [r2, #0x44]
	lsls r0, r0, #1
	adds r0, r0, r2
	ldr r4, _08039E90 @ =0x0000E042
	adds r1, r4, #0
	strh r1, [r0]
	ldr r0, _08039E94 @ =gStaticData_0803A8C4
_08039E2A:
	subs r0, r0, r7
	adds r0, #2
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	lsls r0, r0, #1
	adds r0, r0, r2
	ldr r2, _08039E98 @ =0x0000CAFF
_08039E3A:
	adds r1, r2, #0
	strh r1, [r0]
	mov r4, sp
	ldr r3, [r3]
	ldr r3, [r3, #0x44]
	adds r1, r3, #0
	adds r0, r4, #0
	mov r2, pc
	adds r2, #5
	mov lr, r2
	bx r1
	thumb_func_start sub_8039E50
sub_8039E50: @ 0x08039E50
	nop			@ (mov r8, r8)
	ldr	r0, [r6, #4]
	ldrh	r2, [r0, #4]
	ldr	r0, [sp, #20]
	cmp	r0, r2
	beq _08039F18
	ldr	r3, [sp, #88]	@ 0x58
	cmp	r3, #0
	beq _08039EC0
	ldr	r3, [r6, #60]	@ 0x3c
	ldrb	r1, [r6, #16]
	lsls	r0, r1, #3
	subs	r0, r0, r1
	lsls	r2, r0, #2
	adds	r0, r3, r2
	ldrb	r0, [r0, #13]
	cmp	r0, #0
	beq _08039EAC
	movs	r0, #17
	ldrsb	r0, [r6, r0]
	ldrb	r2, [r6, #17]
	cmp	r0, #0
	ble _08039E9C
	mov	r4, sl
	lsls	r1, r4, #1
	ldr	r0, [sp, #8]
	subs	r0, r0, r1
	b _08039EA4
_08039E88: .4byte gUnknown_03001630
_08039E8C: .4byte gStaticData_0803A8B4
_08039E90: .4byte 0x0000E042
_08039E94: .4byte gStaticData_0803A8C4
_08039E98: .4byte 0x0000CAFF
_08039E9C:
	mov	r0, sl
	lsls	r1, r0, #1
	ldr	r0, [sp, #8]
	adds	r0, r0, r1
_08039EA4:
	str	r0, [sp, #8]
	mvns	r0, r2
	strb	r0, [r6, #17]
	b _08039F0C
_08039EAC:
	adds	r1, r3, #0
	adds	r1, #24
	adds	r1, r1, r2
	adds	r0, r3, #0
	adds	r0, #20
	adds	r0, r0, r2
	ldr	r1, [r1, #0]
	ldr	r0, [r0, #0]
	subs	r1, r1, r0
	b _08039ED6
_08039EC0:
	ldrb	r4, [r6, #18]
	cmp	r4, #0
	beq _08039EE0
	ldr	r2, [r6, #60]	@ 0x3c
	ldrb	r1, [r6, #16]
	lsls	r0, r1, #3
	subs	r0, r0, r1
	lsls	r0, r0, #2
	adds	r2, #28
	adds	r2, r2, r0
	ldr	r1, [r2, #0]
_08039ED6:
	lsls	r1, r1, #11
	ldr	r0, [sp, #8]
	subs	r0, r0, r1
	str	r0, [sp, #8]
	b _08039F0C
_08039EE0:
	ldrb	r0, [r6, #13]
	cmp	r0, #0
	beq _08039EFA
	ldr	r0, [sp, #20]
	lsls	r0, r0, #1
	ldr	r1, [sp, #80]	@ 0x50
	adds	r0, r1, r0
	ldr	r1, [sp, #20]
	subs	r1, r2, r1
	adds	r1, #1
	lsls	r1, r1, #1
	bl sub_8037F3C
_08039EFA:
	ldr r0, _08039F08
	strh	r0, [r6, #42]	@ 0x2a
	strh	r4, [r6, #44]	@ 0x2c
	movs	r0, #128	@ 0x80
	lsls	r0, r0, #24
	str	r0, [r6, #76]	@ 0x4c
	b _08039F18
_08039F08: .4byte 0x8ad0
_08039F0C:
	ldr	r1, [r6, #4]
	ldr	r0, [sp, #20]
	ldrh	r1, [r1, #4]
	cmp	r0, r1
	bcs _08039F18
	b _08039CE6
_08039F18:
	ldr r0, [sp, #8]
	str r0, [r6, #0x44]
	movs r0, #1
_08039F1E:
	add sp, #0x5c
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8039F30
sub_8039F30: @ 0x08039F30
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	adds r3, r1, #0
	adds r6, r2, #0
	ldrh r0, [r6]
	adds r1, r0, #1
	strh r1, [r6]
	lsls r0, r0, #0x10
	lsrs r4, r0, #0x10
	ldrb r0, [r3, #1]
	cmp r0, #0xff
	beq _08039F60
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	cmp r0, #0
	bne _08039F60
	ldrb r0, [r3, #1]
	lsls r0, r0, #3
	adds r0, r3, r0
	ldrh r0, [r0, #4]
	cmp r4, r0
	bne _08039F60
	strh r4, [r6]
_08039F60:
	ldrb r0, [r3]
	subs r2, r0, #1
	lsls r0, r2, #3
	adds r0, r3, r0
	ldrh r1, [r0, #4]
	cmp r4, r1
	blo _08039F8C
	ldrb r1, [r0, #8]
	cmp r1, #0
	bne _08039F8A
	ldrb r0, [r3, #3]
	cmp r0, #0xff
	beq _08039F7E
	cmp r0, r2
	bge _08039F8A
_08039F7E:
	ldr r0, _08039FF0 @ =0x00008AD0
	strh r0, [r5, #0x2a]
	strh r1, [r5, #0x2c]
	movs r0, #0x80
	lsls r0, r0, #0x18
	str r0, [r5, #0x4c]
_08039F8A:
	strh r4, [r6]
_08039F8C:
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	cmp r0, #0
	bne _08039FB6
	ldrb r0, [r3, #2]
	cmp r0, #0xff
	beq _08039FB6
	ldrb r0, [r3, #3]
	cmp r0, #0xff
	beq _08039FB6
	lsls r0, r0, #3
	adds r0, r3, r0
	ldrh r0, [r0, #4]
	cmp r4, r0
	bne _08039FB6
	ldrb r0, [r3, #2]
	lsls r0, r0, #3
	adds r0, r3, r0
	ldrh r0, [r0, #4]
	strh r0, [r6]
_08039FB6:
	movs r1, #0
	ldrh r0, [r3, #4]
	cmp r0, r4
	bhs _08039FCA
	adds r2, r3, #0
_08039FC0:
	adds r2, #8
	adds r1, #1
	ldrh r0, [r2, #4]
	cmp r0, r4
	blo _08039FC0
_08039FCA:
	lsls r0, r1, #3
	adds r0, r3, r0
	ldrh r2, [r0, #4]
	cmp r4, r2
	beq _08039FF4
	movs r5, #6
	ldrsh r2, [r0, r5]
	subs r1, #1
	lsls r1, r1, #3
	adds r1, r3, r1
	ldrh r0, [r1, #4]
	subs r0, r4, r0
	muls r0, r2, r0
	asrs r0, r0, #8
	ldrb r1, [r1, #8]
	adds r0, r0, r1
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	b _08039FF6
	.align 2, 0
_08039FF0: .4byte 0x00008AD0
_08039FF4:
	ldrb r0, [r0, #8]
_08039FF6:
	pop {r4, r5, r6}
	pop {r1}
	bx r1

