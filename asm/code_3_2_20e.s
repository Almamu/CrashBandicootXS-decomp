.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8038538
sub_8038538: @ 0x08038538
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xc
	adds r7, r0, #0
	movs r0, #0
	mov r8, r0
	ldr r1, [r7]
	str r1, [sp, #4]
	ldr r2, [r7, #4]
	str r2, [sp, #8]
	ldr r0, _08038820 @ =0x0000018B
	cmp r2, r0
	bhi _0803855A
	b _080389F0
_0803855A:
	ldr r3, _08038824 @ =gUnknown_03001630
	mov sl, r3
	str r1, [r3]
	movs r4, #0xc6
	lsls r4, r4, #1
	adds r0, r1, r4
	str r0, [sp, #4]
	ldr r1, _08038828 @ =0xFFFFFE74
	adds r0, r2, r1
	str r0, [sp, #8]
	ldr r0, [r7, #0x30]
	cmp r0, #0
	bne _08038578
	ldr r0, _0803882C @ =gStaticData_085A4C5C
	str r0, [r7, #0x30]
_08038578:
	ldr r0, [r7, #0x2c]
	cmp r0, #0
	bne _08038582
	mov r2, r8
	strh r2, [r7, #0xe]
_08038582:
	ldrh r0, [r7, #8]
	ldr r1, _08038830 @ =0x0000FFFF
	cmp r0, r1
	bne _08038594
	ldr r0, [r7, #0x30]
	ldr r0, [r0, #8]
	ldr r0, [r0, #0x18]
	ldrh r0, [r0, #0x18]
	strh r0, [r7, #8]
_08038594:
	ldrh r0, [r7, #0xe]
	cmp r0, r1
	bne _080385A4
	ldr r0, [r7, #0x30]
	ldr r0, [r0, #8]
	ldr r0, [r0, #0x18]
	ldrb r0, [r0, #0x1a]
	strh r0, [r7, #0xe]
_080385A4:
	ldrh r0, [r7, #0x10]
	cmp r0, r1
	bne _080385AE
	movs r0, #0xff
	strh r0, [r7, #0x10]
_080385AE:
	mov r3, sl
	ldr r0, [r3]
	ldr r1, _08038834 @ =0x47415832
	str r1, [r0]
	str r7, [r0, #4]
	mov r4, r8
	str r4, [r0, #0x30]
	str r4, [r0, #0x10]
	str r4, [r0, #0x24]
	adds r0, #0x41
	strb r4, [r0]
	ldr r0, [r3]
	adds r0, #0x43
	movs r5, #1
	strb r5, [r0]
	ldr r0, [r7, #0x30]
	ldr r4, [r0]
	ldr r0, [r7, #0x2c]
	cmp r0, #0
	beq _080385DA
	ldrh r0, [r7, #0xe]
	adds r4, r4, r0
_080385DA:
	mov r0, sl
	ldr r3, [r0]
	ldr r0, [r3, #0x10]
	lsls r0, r0, #2
	adds r1, r3, #0
	adds r1, #8
	adds r1, r1, r0
	ldr r2, [sp, #4]
	str r2, [r1]
	lsls r1, r4, #2
	ldr r0, [sp, #8]
	subs r0, r0, r1
	adds r2, r2, r1
	mov sb, r2
	mov r1, sb
	adds r1, #8
	str r1, [sp, #4]
	subs r0, #8
	str r0, [sp, #8]
	mov r1, sb
	str r1, [r3, #0x14]
	ldrh r0, [r7, #8]
	bl sub_8037FA0
	adds r4, r0, #0
	movs r0, #8
	mov r2, sb
	strb r0, [r2]
	strb r5, [r2, #1]
	ldr r5, _08038838 @ =gStaticData_085A6150
	lsls r4, r4, #3
	adds r0, r4, r5
	ldr r0, [r0]
	movs r6, #0
	strh r0, [r2, #2]
	ldrh r1, [r2, #2]
	lsls r0, r1, #5
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r1
	lsls r0, r0, #3
	ldr r1, _0803883C @ =0x0000E94F
	bl sub_803ADB4
	mov r3, sb
	strh r0, [r3, #4]
	mov r0, sl
	ldr r1, [r0]
	adds r5, #4
	adds r4, r4, r5
	ldr r0, [r4]
	str r0, [r1, #0x34]
	adds r1, #0x40
	strb r6, [r1]
	ldr r0, _08038840 @ =gStaticData_085A614C
	ldr r1, [r0]
	ldrb r0, [r1, #2]
	cmp r0, #0x58
	bne _0803865C
	ldrb r0, [r1, #1]
	cmp r0, #0x41
	bne _0803865C
	ldrb r0, [r1]
	cmp r0, #0x47
	beq _08038668
_0803865C:
	ldr r2, _08038824 @ =gUnknown_03001630
	ldr r1, [r2]
	ldr r0, [r1, #0x34]
	lsls r0, r0, #1
	str r0, [r1, #0x34]
	mov sl, r2
_08038668:
	mov r2, sl
	ldr r1, [r2]
	ldr r3, [r1, #0x14]
	ldrh r0, [r3, #4]
	adds r0, #4
	lsls r0, r0, #1
	ldr r4, [sp, #8]
	cmp r4, r0
	bhs _0803867C
	b _080389F0
_0803867C:
	ldr r2, [sp, #4]
	str r2, [r1, #0x1c]
	ldrh r0, [r3, #4]
	adds r0, #4
	lsls r0, r0, #1
	adds r2, r2, r0
	str r2, [sp, #4]
	mov r3, sl
	ldr r0, [r3]
	ldr r0, [r0, #0x14]
	ldrh r0, [r0, #4]
	adds r0, #4
	lsls r0, r0, #1
	subs r0, r4, r0
	adds r1, r2, #4
	movs r5, #4
	rsbs r5, r5, #0
	ands r1, r5
	subs r1, r1, r2
	adds r2, r2, r1
	str r2, [sp, #4]
	subs r3, r0, r1
	str r3, [sp, #8]
	mov r4, sl
	ldr r1, [r4]
	movs r0, #0
	str r0, [r1, #0x2c]
	ldr r4, [r1, #0x14]
	ldrh r0, [r4, #4]
	lsls r0, r0, #1
	cmp r3, r0
	bhs _080386BE
	b _08038A02
_080386BE:
	str r2, [r1, #0x18]
	ldrh r0, [r4, #4]
	lsls r0, r0, #1
	adds r0, r2, r0
	str r0, [sp, #4]
	mov r1, sl
	ldr r0, [r1]
	ldr r0, [r0, #0x14]
	ldrh r0, [r0, #4]
	lsls r0, r0, #1
	subs r0, r3, r0
	str r0, [sp, #8]
	ldr r1, [r1]
	ldr r0, [r1, #0x18]
	ldr r1, [r1, #0x14]
	ldrh r1, [r1, #4]
	lsls r1, r1, #1
	bl sub_8037F3C
	ldr r1, [sp, #4]
	adds r0, r1, #4
	ands r0, r5
	subs r1, r0, r1
	str r0, [sp, #4]
	ldr r0, [sp, #8]
	subs r0, r0, r1
	str r0, [sp, #8]
	movs r2, #0
	ldr r0, [r7, #0x30]
	ldr r1, [r0, #4]
	adds r4, r0, #0
	ldr r1, [r1, #0x18]
_080386FE:
	ldr r0, [r1, #4]
	cmp r0, r8
	bls _08038706
	mov r8, r0
_08038706:
	adds r1, #8
	adds r2, #1
	cmp r2, #2
	bls _080386FE
	ldrh r1, [r7, #0xc]
	movs r0, #0x10
	ands r0, r1
	cmp r0, #0
	bne _0803874E
	ldr r0, [r4, #0xc]
	cmp r0, #0
	beq _0803874E
	adds r4, r0, #0
	movs r1, #0
	ldr r0, [r4]
	cmp r1, r0
	bge _0803874E
	adds r5, r0, #0
_0803872A:
	lsls r0, r1, #2
	adds r0, r0, r4
	ldr r0, [r0, #4]
	movs r2, #0
	adds r3, r1, #1
	ldr r0, [r0, #4]
	ldr r1, [r0, #0x18]
_08038738:
	ldr r0, [r1, #4]
	cmp r0, r8
	bls _08038740
	mov r8, r0
_08038740:
	adds r1, #8
	adds r2, #1
	cmp r2, #2
	bls _08038738
	adds r1, r3, #0
	cmp r1, r5
	blt _0803872A
_0803874E:
	mov r2, r8
	cmp r2, #0
	beq _080387BC
	ldr r0, [sp, #4]
	adds r1, r0, #4
	movs r3, #4
	rsbs r3, r3, #0
	mov sl, r3
	ands r1, r3
	subs r1, r1, r0
	adds r3, r0, r1
	str r3, [sp, #4]
	ldr r0, [sp, #8]
	subs r2, r0, r1
	str r2, [sp, #8]
	cmp r2, #0x17
	bhi _08038772
	b _080389F0
_08038772:
	ldr r0, _08038824 @ =gUnknown_03001630
	ldr r5, [r0]
	str r3, [r5, #0x24]
	adds r6, r3, #0
	adds r6, #0x18
	str r6, [sp, #4]
	adds r4, r2, #0
	subs r4, #0x18
	str r4, [sp, #8]
	mov r1, sb
	ldrh r0, [r1, #2]
	mov r2, r8
	muls r2, r0, r2
	adds r0, r2, #0
	movs r1, #0xfa
	lsls r1, r1, #2
	bl sub_8037E54
	lsls r3, r0, #1
	cmp r4, r3
	bhs _0803879E
	b _080389F0
_0803879E:
	str r6, [r5, #0x20]
	str r3, [r5, #0x28]
	adds r0, r6, r3
	subs r2, r4, r3
	adds r1, r0, #4
	mov r4, sl
	ands r1, r4
	subs r0, r1, r0
	str r1, [sp, #4]
	subs r2, r2, r0
	str r2, [sp, #8]
	adds r0, r6, #0
	adds r1, r3, #0
	bl sub_8037F3C
_080387BC:
	movs r2, #0
	ldr r0, _08038824 @ =gUnknown_03001630
	mov sl, r0
	ldr r4, [r7, #0x30]
	ldr r6, _08038844 @ =gStaticData_0803A73C
	ldr r1, _08038848 @ =gStaticData_0803A818
	mov r8, r1
	mov r3, sl
	ldr r5, _0803884C @ =gStaticData_0803A630
_080387CE:
	ldr r0, [r3]
	lsls r1, r2, #2
	adds r0, #0x48
	adds r0, r0, r1
	ldm r5!, {r1}
	str r1, [r0]
	adds r2, #1
	cmp r2, #0x14
	ble _080387CE
	movs r2, #0
	ldr r3, _08038824 @ =gUnknown_03001630
	adds r5, r6, #0
_080387E6:
	ldr r0, [r3]
	lsls r1, r2, #2
	adds r0, #0x9c
	adds r0, r0, r1
	ldm r5!, {r1}
	str r1, [r0]
	adds r2, #1
	cmp r2, #0x37
	ble _080387E6
	mov r6, r8
	ldr r0, [r4, #8]
	ldr r0, [r0, #0x18]
	ldrb r0, [r0, #0x1b]
	cmp r0, #0
	bne _08038812
	ldrh r1, [r7, #0xc]
	movs r0, #0x20
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r1, r0, #0x10
	cmp r1, #0
	beq _08038850
_08038812:
	mov r2, sl
	ldr r0, [r2]
	adds r0, #0x42
	movs r1, #1
	strb r1, [r0]
	movs r4, #0x4c
	b _0803885A
	.align 2, 0
_08038820: .4byte 0x0000018B
_08038824: .4byte gUnknown_03001630
_08038828: .4byte 0xFFFFFE74
_0803882C: .4byte gStaticData_085A4C5C
_08038830: .4byte 0x0000FFFF
_08038834: .4byte 0x47415832
_08038838: .4byte gStaticData_085A6150
_0803883C: .4byte 0x0000E94F
_08038840: .4byte gStaticData_085A614C
_08038844: .4byte gStaticData_0803A73C
_08038848: .4byte gStaticData_0803A818
_0803884C: .4byte gStaticData_0803A630
_08038850:
	mov r3, sl
	ldr r0, [r3]
	adds r0, #0x42
	strb r1, [r0]
	movs r4, #0x37
_0803885A:
	lsls r2, r4, #2
	ldr r3, [sp, #8]
	cmp r3, r2
	bhs _08038864
	b _080389F0
_08038864:
	mov r1, sl
	ldr r0, [r1]
	ldr r1, [sp, #4]
	str r1, [r0, #0x44]
	adds r1, r1, r2
	str r1, [sp, #4]
	subs r0, r3, r2
	str r0, [sp, #8]
	movs r2, #0
	cmp r2, r4
	bge _08038890
	mov r5, sl
	adds r3, r6, #0
_0803887E:
	ldr r0, [r5]
	ldr r1, [r0, #0x44]
	lsls r0, r2, #2
	adds r0, r0, r1
	ldm r3!, {r1}
	str r1, [r0]
	adds r2, #1
	cmp r2, r4
	blt _0803887E
_08038890:
	ldrh r1, [r7, #0xc]
	movs r0, #4
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r1, r0, #0x10
	cmp r1, #0
	beq _080388E8
	ldr r3, [sp, #8]
	cmp r3, #0xef
	bhi _080388A6
	b _080389F0
_080388A6:
	mov r2, sl
	ldr r1, [r2]
	movs r2, #0xbe
	lsls r2, r2, #1
	adds r1, r1, r2
	ldr r0, [sp, #4]
	str r0, [r1]
	adds r0, #0xf0
	str r0, [sp, #4]
	adds r0, r3, #0
	subs r0, #0xf0
	str r0, [sp, #8]
	movs r3, #0
	ldr r4, [r7, #0x30]
	ldr r0, [r7, #0x2c]
	mov sb, r0
	add r6, sp, #8
	mov r8, sl
	ldr r5, _080388E4 @ =gStaticData_0803A67C
_080388CC:
	mov r1, r8
	ldr r0, [r1]
	adds r0, r0, r2
	ldr r1, [r0]
	lsls r0, r3, #2
	adds r0, r0, r1
	ldm r5!, {r1}
	str r1, [r0]
	adds r3, #1
	cmp r3, #0x3b
	ble _080388CC
	b _080388FC
	.align 2, 0
_080388E4: .4byte gStaticData_0803A67C
_080388E8:
	mov r2, sl
	ldr r0, [r2]
	movs r3, #0xbe
	lsls r3, r3, #1
	adds r0, r0, r3
	str r1, [r0]
	ldr r4, [r7, #0x30]
	ldr r0, [r7, #0x2c]
	mov sb, r0
	add r6, sp, #8
_080388FC:
	mov r5, sl
	ldr r0, [r5]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r0, r0, r1
	movs r2, #0
	mov r8, r2
	str r2, [r0]
	ldrh r2, [r7, #0xe]
	str r6, [sp]
	adds r0, r4, #0
	mov r1, sb
	add r3, sp, #4
	bl sub_8038240
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _080389F0
	ldr r2, [sp, #4]
	adds r0, r2, #4
	movs r1, #4
	rsbs r1, r1, #0
	ands r0, r1
	subs r2, r0, r2
	str r0, [sp, #4]
	ldr r0, [sp, #8]
	subs r0, r0, r2
	str r0, [sp, #8]
	ldr r2, [r5]
	ldr r0, [r2, #0x10]
	lsls r0, r0, #2
	adds r3, r2, #0
	adds r3, #8
	adds r0, r3, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldrh r0, [r7, #0xe]
	str r0, [r1, #0x14]
	ldr r0, [r2, #0x10]
	lsls r0, r0, #2
	adds r0, r3, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r2, #0x1c]
	str r0, [r1, #0x10]
	ldr r0, [r2, #0x10]
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r1, [r1]
	bl sub_803AD7C
	ldr r2, [r5]
	movs r3, #0xc2
	lsls r3, r3, #1
	adds r1, r2, r3
	ldr r0, [sp, #4]
	str r0, [r1]
	movs r4, #0xc4
	lsls r4, r4, #1
	adds r1, r2, r4
	ldr r0, [sp, #8]
	str r0, [r1]
	bl sub_80384DC
	ldr r0, [r5]
	movs r3, #1
	str r3, [r0, #0x30]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0, #4]
	ldrh r0, [r7, #0xc]
	lsrs r0, r0, #3
	ands r0, r3
	adds r1, #0x20
	strb r0, [r1]
	ldr r0, [r5]
	ldr r0, [r0, #4]
	adds r0, #0x39
	mov r1, r8
	strb r1, [r0]
	ldr r0, [r5]
	ldr r0, [r0, #4]
	adds r0, #0x3a
	strb r1, [r0]
	ldrh r1, [r7, #0xc]
	movs r0, #2
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r1, r0, #0x10
	cmp r1, #0
	beq _080389E4
	ldr r2, [r5]
	ldr r1, [r2, #0x10]
	lsls r1, r1, #2
	adds r0, r2, #0
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0, #0x18]
	ldr r1, [r0, #4]
	cmp r1, #0
	beq _080389E0
	adds r0, r2, #0
	adds r0, #0x40
	strb r3, [r0]
	b _080389EC
_080389E0:
	adds r0, r2, #0
	b _080389E8
_080389E4:
	mov r2, sl
	ldr r0, [r2]
_080389E8:
	adds r0, #0x40
	strb r1, [r0]
_080389EC:
	movs r0, #1
	b _08038A04
_080389F0:
	adds r0, r7, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08038A02
	ldr r0, _08038A14 @ =gStaticData_085A61D0
	ldr r1, _08038A18 @ =gStaticData_085A61DC
	bl sub_80392E0
_08038A02:
	movs r0, #0
_08038A04:
	add sp, #0xc
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08038A14: .4byte gStaticData_085A61D0
_08038A18: .4byte gStaticData_085A61DC

	thumb_func_start sub_8038A1C
sub_8038A1C: @ 0x08038A1C
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #0xc
	mov sb, r0
	ldr r6, _08038A60 @ =gUnknown_03001630
	ldr r1, [r6]
	movs r2, #0xc2
	lsls r2, r2, #1
	adds r0, r1, r2
	ldr r4, [r0]
	str r4, [sp, #4]
	adds r2, #4
	adds r0, r1, r2
	ldr r3, [r0]
	str r3, [sp, #8]
	ldr r2, [r1, #4]
	ldrh r1, [r2, #0xc]
	movs r0, #0x10
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r7, r0, #0x10
	cmp r7, #0
	beq _08038A6C
	adds r0, r2, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08038B4A
	ldr r0, _08038A64 @ =gStaticData_085A61EC
	ldr r1, _08038A68 @ =gStaticData_085A61F8
	b _08038B46
	.align 2, 0
_08038A60: .4byte gUnknown_03001630
_08038A64: .4byte gStaticData_085A61EC
_08038A68: .4byte gStaticData_085A61F8
_08038A6C:
	adds r0, r4, #0
	adds r1, r3, #0
	bl sub_8037F3C
	ldr r3, [r6]
	str r7, [r3, #0x30]
	movs r0, #1
	mov r8, r0
	str r0, [r3, #0x10]
	mov r1, sb
	ldr r2, [r1]
	ldr r4, [r3, #4]
	ldr r5, [r4, #0x2c]
	cmp r5, #0
	beq _08038A8E
	ldrh r0, [r4, #0xe]
	adds r2, r2, r0
_08038A8E:
	ldr r0, [sp, #4]
	str r0, [r3, #0xc]
	lsls r2, r2, #2
	ldr r1, [sp, #8]
	cmp r1, r2
	bhs _08038AA2
	mov r2, r8
	str r2, [r3, #0x30]
	str r7, [r3, #0x10]
	b _08038B34
_08038AA2:
	adds r0, r0, r2
	str r0, [sp, #4]
	subs r0, r1, r2
	str r0, [sp, #8]
	ldrh r2, [r4, #0xe]
	add r0, sp, #8
	str r0, [sp]
	mov r0, sb
	adds r1, r5, #0
	add r3, sp, #4
	bl sub_8038240
	lsls r0, r0, #0x18
	lsrs r1, r0, #0x18
	cmp r1, #0
	beq _08038B2C
	ldr r2, [r6]
	ldr r0, [r2, #0x10]
	lsls r0, r0, #2
	adds r3, r2, #0
	adds r3, #8
	adds r0, r3, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r2, #4]
	ldrh r0, [r0, #0xe]
	str r0, [r1, #0x14]
	ldr r0, [r2, #0x10]
	lsls r0, r0, #2
	adds r0, r3, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r2, #0x1c]
	str r0, [r1, #0x10]
	ldr r0, [r2, #0x10]
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r1, [r1]
	bl sub_803AD7C
	ldr r0, [r6]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r0, [r0, #4]
	adds r0, #0x20
	mov r1, r8
	strb r1, [r0]
	ldr r0, [r6]
	ldr r0, [r0, #4]
	adds r0, #0x39
	strb r7, [r0]
	ldr r0, [r6]
	ldr r0, [r0, #4]
	adds r0, #0x3a
	strb r7, [r0]
	ldr r0, [r6]
	adds r0, #0x41
	strb r1, [r0]
	ldr r0, [r6]
	mov r2, r8
	str r2, [r0, #0x30]
	movs r0, #1
	b _08038B4C
_08038B2C:
	ldr r0, [r6]
	mov r2, r8
	str r2, [r0, #0x30]
	str r1, [r0, #0x10]
_08038B34:
	ldr r0, _08038B5C @ =gUnknown_03001630
	ldr r0, [r0]
	ldr r0, [r0, #4]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08038B4A
	ldr r0, _08038B60 @ =gStaticData_085A61D0
	ldr r1, _08038B64 @ =gStaticData_085A61DC
_08038B46:
	bl sub_80392E0
_08038B4A:
	movs r0, #0
_08038B4C:
	add sp, #0xc
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08038B5C: .4byte gUnknown_03001630
_08038B60: .4byte gStaticData_085A61D0
_08038B64: .4byte gStaticData_085A61DC

	thumb_func_start sub_8038B68
sub_8038B68: @ 0x08038B68
	push {r4, lr}
	ldr r4, _08038C00 @ =gUnknown_03001630
	ldr r3, [r4]
	ldr r1, [r3]
	ldr r0, _08038C04 @ =0x47415832
	cmp r1, r0
	bne _08038BF8
	ldr r0, [r3, #0x30]
	cmp r0, #0
	beq _08038BF8
	ldr r0, [r3, #0x30]
	cmp r0, #1
	bne _08038BA4
	ldr r1, _08038C08 @ =0x04000084
	movs r0, #0x80
	strh r0, [r1]
	movs r0, #2
	str r0, [r3, #0x30]
	adds r1, #0x7e
	movs r0, #0
	strh r0, [r1]
	ldr r2, _08038C0C @ =0x04000100
	ldr r1, [r3, #0x34]
	movs r0, #0x80
	lsls r0, r0, #9
	subs r0, r0, r1
	movs r1, #0xc0
	lsls r1, r1, #0x10
	orrs r0, r1
	str r0, [r2]
_08038BA4:
	ldr r1, [r4]
	ldr r0, [r1, #4]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08038BC2
	adds r0, r1, #0
	adds r0, #0x43
	ldrb r0, [r0]
	cmp r0, #0
	bne _08038BC2
	ldr r0, _08038C10 @ =gStaticData_085A6214
	ldr r1, _08038C14 @ =gStaticData_085A621C
	bl sub_80392E0
_08038BC2:
	ldr r4, _08038C00 @ =gUnknown_03001630
	ldr r3, [r4]
	ldr r0, [r3, #0x2c]
	cmp r0, #1
	bne _08038BF0
	ldr r2, _08038C18 @ =0x040000C6
	ldr r1, _08038C1C @ =0x00008640
	adds r0, r1, #0
	strh r0, [r2]
	# this expands to adds r3, r3, #0, but for some reason the compiler changes it
	# from 1b 1c to 00 33, which is not correct
	.byte 0x1b
	.byte 0x1c
	mov r8, r8
	mov r8, r8
	mov r8, r8
	movs r1, #0xc8
	lsls r1, r1, #3
	adds r0, r1, #0
	strh r0, [r2]
	ldr r1, _08038C20 @ =0x040000BC
	ldr r0, [r3, #0x18]
	str r0, [r1]
	ldr r1, _08038C24 @ =0x0000B660
	adds r0, r1, #0
	strh r0, [r2]
_08038BF0:
	ldr r0, [r4]
	adds r0, #0x43
	movs r1, #0
	strb r1, [r0]
_08038BF8:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08038C00: .4byte gUnknown_03001630
_08038C04: .4byte 0x47415832
_08038C08: .4byte 0x04000084
_08038C0C: .4byte 0x04000100
_08038C10: .4byte gStaticData_085A6214
_08038C14: .4byte gStaticData_085A621C
_08038C18: .4byte 0x040000C6
_08038C1C: .4byte 0x00008640
_08038C20: .4byte 0x040000BC
_08038C24: .4byte 0x0000B660

	thumb_func_start sub_8038C28
sub_8038C28: @ 0x08038C28
	ldr r0, _08038C44 @ =gUnknown_03001630
	ldr r1, [r0]
	ldr r0, [r1, #0x30]
	cmp r0, #0
	beq _08038C40
	movs r0, #0
	str r0, [r1, #0x30]
	ldr r2, _08038C48 @ =0x04000082
	ldrh r1, [r2]
	ldr r0, _08038C4C @ =0x0000FCFF
	ands r0, r1
	strh r0, [r2]
_08038C40:
	bx lr
	.align 2, 0
_08038C44: .4byte gUnknown_03001630
_08038C48: .4byte 0x04000082
_08038C4C: .4byte 0x0000FCFF

	thumb_func_start sub_8038C50
sub_8038C50: @ 0x08038C50
	ldr r0, _08038C7C @ =gUnknown_03001630
	ldr r1, [r0]
	ldr r0, [r1, #0x30]
	cmp r0, #0
	bne _08038C7A
	movs r0, #1
	str r0, [r1, #0x30]
	ldr r2, _08038C80 @ =0x040000A0
	movs r1, #0
	movs r0, #7
_08038C64:
	strh r1, [r2]
	subs r0, #1
	cmp r0, #0
	bge _08038C64
	ldr r0, _08038C84 @ =0x04000082
	ldrh r1, [r0]
	movs r3, #0xc0
	lsls r3, r3, #2
	adds r2, r3, #0
	orrs r1, r2
	strh r1, [r0]
_08038C7A:
	bx lr
	.align 2, 0
_08038C7C: .4byte gUnknown_03001630
_08038C80: .4byte 0x040000A0
_08038C84: .4byte 0x04000082

	thumb_func_start sub_8038C88
sub_8038C88: @ 0x08038C88
	push {r4, r5, r6, lr}
	ldr r6, _08038DBC @ =gUnknown_03001630
	ldr r1, [r6]
	ldr r0, [r1, #0x30]
	cmp r0, #0
	bne _08038C96
	b _08038DB4
_08038C96:
	ldr r0, [r1, #4]
	ldr r0, [r0, #0x34]
	cmp r0, #0
	beq _08038CA4
	movs r1, #0x40
	bl sub_8037F3C
_08038CA4:
	ldr r0, [r6]
	ldr r1, [r0, #4]
	ldrh r0, [r1, #0x10]
	cmp r0, #0xff
	bls _08038CB2
	movs r0, #0xff
	strh r0, [r1, #0x10]
_08038CB2:
	ldr r2, [r6]
	ldr r1, [r2, #0x10]
	lsls r1, r1, #2
	adds r0, r2, #0
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0, #4]
	ldr r0, [r2, #4]
	ldrh r0, [r0, #0x10]
	strb r0, [r1, #0x1f]
	ldr r0, [r6]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r2, r0, r1
	ldr r1, [r0, #4]
	ldrh r1, [r1, #0xa]
	str r1, [r2]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r0, [r0, #4]
	movs r4, #1
	strb r4, [r0, #0x1a]
	ldr r3, [r6]
	ldr r1, [r3, #0x10]
	lsls r1, r1, #2
	adds r0, r3, #0
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r1, [r0, #4]
	ldrh r2, [r1, #4]
	ldr r1, [r3, #0x2c]
	muls r2, r1, r2
	ldr r1, [r3, #0x18]
	adds r1, r1, r2
	bl sub_803A5A8
	ldr r1, [r6]
	ldr r0, [r1, #0x2c]
	eors r0, r4
	str r0, [r1, #0x2c]
	ldr r2, [r1, #4]
	ldr r0, [r1, #0x10]
	lsls r0, r0, #2
	adds r1, #8
	adds r1, r1, r0
	ldr r0, [r1]
	ldr r0, [r0, #4]
	adds r0, #0x21
	ldrb r0, [r0]
	adds r2, #0x39
	strb r0, [r2]
	ldr r1, [r6]
	ldr r3, [r1, #0x10]
	cmp r3, #1
	bne _08038DAA
	ldr r2, [r1, #4]
	adds r0, r2, #0
	adds r0, #0x39
	ldrb r0, [r0]
	cmp r0, #0
	beq _08038DAA
	movs r0, #0
	str r0, [r1, #0x10]
	adds r0, r2, #0
	adds r0, #0x3a
	strb r3, [r0]
	ldr r0, [r6]
	ldr r1, [r0, #4]
	ldr r0, [r1, #0x2c]
	cmp r0, #0
	beq _08038DAA
	movs r5, #0
	ldrh r1, [r1, #0xe]
	cmp r5, r1
	bge _08038DAA
_08038D54:
	ldr r1, [r6]
	ldr r0, [r1, #0x10]
	lsls r0, r0, #2
	adds r2, r1, #0
	adds r2, #8
	adds r2, r2, r0
	ldr r0, [r1, #4]
	ldr r0, [r0, #0x30]
	ldr r0, [r0]
	adds r0, r0, r5
	ldr r2, [r2]
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	ldr r1, [r0, #8]
	ldr r0, [r2, #4]
	str r0, [r1]
	ldr r3, [r6]
	ldr r1, [r3, #0x10]
	lsls r1, r1, #2
	adds r0, r3, #0
	adds r0, #8
	adds r0, r0, r1
	ldr r4, [r0]
	ldr r2, [r4]
	ldr r0, [r2]
	ldr r1, [r0, #0xc]
	adds r1, r1, r5
	ldr r0, [r2, #8]
	lsls r1, r1, #2
	adds r1, r1, r0
	ldr r2, [r3, #4]
	ldr r0, [r2, #0x30]
	ldr r0, [r0]
	adds r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r4
	ldr r0, [r0]
	str r0, [r1]
	adds r5, #1
	ldrh r2, [r2, #0xe]
	cmp r5, r2
	blt _08038D54
_08038DAA:
	ldr r0, _08038DBC @ =gUnknown_03001630
	ldr r0, [r0]
	adds r0, #0x43
	movs r1, #1
	strb r1, [r0]
_08038DB4:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08038DBC: .4byte gUnknown_03001630

	thumb_func_start sub_8038DC0
sub_8038DC0: @ 0x08038DC0
	push {r4, r5, r6, r7, lr}
	adds r7, r0, #0
	ldr r4, _08038E6C @ =0x7FFFFFFF
	movs r3, #0
	ldr r2, _08038E70 @ =gUnknown_03001630
	ldr r0, [r2]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #0x14]
	adds r6, r2, #0
	cmp r3, r0
	bhs _08038E00
	adds r2, r0, #0
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r1, r0, r1
_08038DEC:
	ldr r0, [r1]
	ldr r0, [r0, #0x4c]
	cmp r0, r4
	bgt _08038DF8
	adds r5, r3, #0
	adds r4, r0, #0
_08038DF8:
	adds r1, #4
	adds r3, #1
	cmp r3, r2
	blo _08038DEC
_08038E00:
	ldr r0, [r6]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	adds r0, r0, r5
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	adds r0, #0x24
	movs r2, #0
	movs r1, #8
	strb r1, [r0]
	ldr r0, [r6]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	adds r0, r0, r5
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	adds r0, #0x25
	strb r7, [r0]
	ldr r0, [r6]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	adds r0, r0, r5
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	str r2, [r0, #0x4c]
	adds r0, r5, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08038E6C: .4byte 0x7FFFFFFF
_08038E70: .4byte gUnknown_03001630

	thumb_func_start sub_8038E74
sub_8038E74: @ 0x08038E74
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r0
	adds r6, r2, #0
	adds r7, r3, #0
	movs r5, #1
	rsbs r5, r5, #0
	adds r4, r6, #0
	adds r3, r5, #0
	cmp r1, r5
	bne _08038ECC
	movs r3, #0
	ldr r2, _08038EC8 @ =gUnknown_03001630
	ldr r0, [r2]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #0x14]
	mov ip, r2
	cmp r3, r0
	bhs _08038F02
	adds r2, r0, #0
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r1, r0, r1
_08038EB2:
	ldr r0, [r1]
	ldr r0, [r0, #0x4c]
	cmp r0, r4
	bgt _08038EBE
	adds r5, r3, #0
	adds r4, r0, #0
_08038EBE:
	adds r1, #4
	adds r3, #1
	cmp r3, r2
	blo _08038EB2
	b _08038F02
	.align 2, 0
_08038EC8: .4byte gUnknown_03001630
_08038ECC:
	adds r5, r1, #0
	ldr r2, _08038F34 @ =gUnknown_03001630
	ldr r0, [r2]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #0x14]
	mov ip, r2
	cmp r5, r0
	blo _08038EE8
	adds r5, r3, #0
_08038EE8:
	cmp r5, r3
	beq _08038F02
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	adds r0, r0, r5
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r0, [r0, #0x4c]
	cmp r6, r0
	bge _08038F02
	adds r5, r3, #0
_08038F02:
	ldr r0, _08038F38 @ =0x0FFFFFFF
	cmp r5, r0
	beq _08038F88
	mov r1, ip
	ldr r0, [r1]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	adds r0, r0, r5
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r4, [r0]
	movs r0, #1
	rsbs r0, r0, #0
	cmp r7, r0
	beq _08038F3C
	asrs r0, r7, #5
	adds r1, r0, #2
	b _08038F3E
	.align 2, 0
_08038F34: .4byte gUnknown_03001630
_08038F38: .4byte 0x0FFFFFFF
_08038F3C:
	movs r1, #8
_08038F3E:
	adds r0, r4, #0
	adds r0, #0x24
	strb r1, [r0]
	mov r1, ip
	ldr r0, [r1]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	adds r0, r0, r5
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	adds r0, #0x25
	mov r1, r8
	strb r1, [r0]
	mov r1, ip
	ldr r0, [r1]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	adds r0, r0, r5
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	str r6, [r0, #0x4c]
_08038F88:
	adds r0, r5, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_start sub_8038F94
sub_8038F94: @ 0x08038F94
	adds	r3, r0, #0
	adds	r2, r1, #0
	ldr r0, _08038FC8
	cmp	r2, r0
	bhi _08038FC4
	ldr r0, _08038FCC
	ldr	r0, [r0, #0]
	ldr	r1, [r0, #16]
	lsls	r1, r1, #2
	adds	r0, #8
	adds	r0, r0, r1
	ldr	r0, [r0, #0]
	ldr	r1, [r0, #0]
	ldr	r0, [r1, #0]
	ldr	r0, [r0, #12]
	adds	r0, r0, r3
	ldr	r1, [r1, #8]
	lsls	r0, r0, #2
	adds	r0, r0, r1
	ldr	r1, [r0, #0]
	ldr	r0, [r1, #60]	@ 0x3c
	cmp	r0, #0
	beq _08038FC4
	strh	r2, [r1, #38]	@ 0x26
_08038FC4:
	bx	lr
	movs	r0, r0
_08038FC8: .4byte 0xeec
_08038FCC: .4byte gUnknown_03001630

	thumb_func_start sub_8038FD0
sub_8038FD0: @ 0x08038FD0
	adds r2, r0, #0
	movs r0, #1
	rsbs r0, r0, #0
	cmp r2, r0
	bne _08039030
	movs r2, #0
	ldr r3, _0803902C @ =gUnknown_03001630
	ldr r0, [r3]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0, #0x14]
	cmp r2, r0
	bhs _0803905E
_08038FF2:
	ldr r0, [r3]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	adds r0, r0, r2
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	adds r0, #0x24
	movs r1, #1
	strb r1, [r0]
	adds r2, #1
	ldr r0, [r3]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0, #0x14]
	cmp r2, r0
	blo _08038FF2
	b _0803905E
	.align 2, 0
_0803902C: .4byte gUnknown_03001630
_08039030:
	ldr r0, _08039060 @ =gUnknown_03001630
	ldr r0, [r0]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #0x14]
	cmp r2, r0
	bhs _0803905E
	cmp r2, #0
	blt _0803905E
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	adds r0, r0, r2
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	adds r0, #0x24
	movs r1, #1
	strb r1, [r0]
_0803905E:
	bx lr
	.align 2, 0
_08039060: .4byte gUnknown_03001630

	thumb_func_start sub_8039064
sub_8039064: @ 0x08039064
	push {r4, lr}
	adds r2, r0, #0
	adds r4, r1, #0
	cmp r4, #0xff
	bls _08039070
	movs r4, #0xff
_08039070:
	movs r0, #1
	rsbs r0, r0, #0
	cmp r2, r0
	bne _080390C4
	movs r2, #0
	ldr r3, _080390C0 @ =gUnknown_03001630
	ldr r0, [r3]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0, #0xc]
	cmp r2, r0
	bhs _080390EC
_08039092:
	ldr r0, [r3]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r1, [r0]
	lsls r0, r2, #2
	adds r0, r0, r1
	ldr r0, [r0, #0xc]
	strb r4, [r0, #0x18]
	adds r2, #1
	ldr r0, [r3]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0, #0xc]
	cmp r2, r0
	blo _08039092
	b _080390EC
	.align 2, 0
_080390C0: .4byte gUnknown_03001630
_080390C4:
	movs r0, #2
	rsbs r0, r0, #0
	cmp r2, r0
	ble _080390EC
	ldr r0, _080390F4 @ =gUnknown_03001630
	ldr r0, [r0]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r1, [r0]
	ldr r0, [r1]
	ldr r0, [r0]
	ldr r0, [r0, #0xc]
	cmp r2, r0
	bhs _080390EC
	lsls r0, r2, #2
	adds r0, r0, r1
	ldr r0, [r0, #0xc]
	strb r4, [r0, #0x18]
_080390EC:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080390F4: .4byte gUnknown_03001630

	thumb_func_start sub_80390F8
sub_80390F8: @ 0x080390F8
	push {r4, lr}
	adds r2, r0, #0
	adds r4, r1, #0
	cmp r4, #0xff
	bls _08039104
	movs r4, #0xff
_08039104:
	movs r0, #1
	rsbs r0, r0, #0
	cmp r2, r0
	bne _08039160
	movs r2, #0
	ldr r3, _0803915C @ =gUnknown_03001630
	ldr r0, [r3]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0, #0x14]
	cmp r2, r0
	bhs _0803918E
_08039124:
	ldr r0, [r3]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	adds r0, r0, r2
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	strb r4, [r0, #0x18]
	adds r2, #1
	ldr r0, [r3]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0, #0x14]
	cmp r2, r0
	blo _08039124
	b _0803918E
	.align 2, 0
_0803915C: .4byte gUnknown_03001630
_08039160:
	movs r0, #2
	rsbs r0, r0, #0
	cmp r2, r0
	ble _0803918E
	ldr r0, _08039194 @ =gUnknown_03001630
	ldr r0, [r0]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #0x14]
	cmp r2, r0
	bge _0803918E
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	adds r0, r0, r2
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	strb r4, [r0, #0x18]
_0803918E:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08039194: .4byte gUnknown_03001630

	thumb_func_start sub_8039198
sub_8039198: @ 0x08039198
	ldr r2, _080391D4 @ =gUnknown_03001630
	ldr r0, [r2]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r0, [r0, #4]
	movs r1, #0
	strb r1, [r0, #0x1a]
	ldr r0, [r2]
	str r1, [r0, #0x30]
	ldr r0, _080391D8 @ =0x04000084
	strh r1, [r0]
	ldr r2, _080391DC @ =0x040000C6
	ldr r3, _080391E0 @ =0x00008640
	adds r0, r3, #0
	strh r0, [r2]
	# this expands to adds r3, r3, #0, but for some reason the compiler changes it
	# from 1b 1c to 00 33, which is not correct
	.byte 0x1b
	.byte 0x1c
	mov r8, r8
	mov r8, r8
	mov r8, r8
	movs r3, #0xc8
	lsls r3, r3, #3
	adds r0, r3, #0
	strh r0, [r2]
	ldr r0, _080391E4 @ =0x04000100
	str r1, [r0]
	bx lr
	.align 2, 0
_080391D4: .4byte gUnknown_03001630
_080391D8: .4byte 0x04000084
_080391DC: .4byte 0x040000C6
_080391E0: .4byte 0x00008640
_080391E4: .4byte 0x04000100

	thumb_func_start sub_80391E8
sub_80391E8: @ 0x080391E8
	lsls r1, r0, #1
	adds r1, r1, r0
	lsls r1, r1, #2
	ldr r0, _0803920C @ =0x040000C6
	adds r1, r1, r0
	ldr r2, _08039210 @ =0x00008640
	adds r0, r2, #0
	strh r0, [r1]
	# this expands to adds r3, r3, #0, but for some reason the compiler changes it
	# from 1b 1c to 00 33, which is not correct
	.byte 0x1b
	.byte 0x1c
	mov r8, r8
	mov r8, r8
	mov r8, r8
	movs r2, #0xc8
	lsls r2, r2, #3
	adds r0, r2, #0
	strh r0, [r1]
	bx lr
	.align 2, 0
_0803920C: .4byte 0x040000C6
_08039210: .4byte 0x00008640

	thumb_func_start sub_8039214
sub_8039214: @ 0x08039214
	push {r4, r5, r6, r7, lr}
	adds r5, r2, #0
	movs r3, #0xc0
	lsls r3, r3, #0x13
	adds r2, r0, r3
	adds r0, r0, r2
	lsls r1, r1, #6
	adds r3, r0, r1
	ldrb r2, [r5]
	cmp r2, #0
	beq _080392BC
	movs r0, #0x40
	rsbs r0, r0, #0
	mov ip, r0
_08039230:
	movs r4, #0
	movs r0, #0x3f
	ands r0, r3
	lsrs r1, r0, #1
	ldrb r6, [r5]
	adds r7, r5, #1
	cmp r1, #0x1f
	bgt _08039268
	adds r0, r2, #0
	b _08039250
_08039244:
	adds r1, #1
	adds r4, #1
	cmp r1, #0x1f
	bgt _08039268
	adds r0, r5, r4
	ldrb r0, [r0]
_08039250:
	cmp r0, #0
	beq _08039268
	cmp r0, #0x20
	beq _08039268
	cmp r0, #0xa
	beq _08039268
	cmp r1, #0x1d
	ble _08039244
	mov r0, ip
	ands r0, r3
	adds r3, r0, #0
	adds r3, #0x40
_08039268:
	adds r1, r6, #0
	adds r5, r7, #0
	cmp r1, #0x5f
	bne _08039272
	movs r1, #0x5d
_08039272:
	cmp r1, #0x3a
	bne _08039278
	movs r1, #0x5c
_08039278:
	cmp r1, #0x2e
	bne _0803927E
	movs r1, #0x5b
_0803927E:
	cmp r1, #0xa
	bne _0803928A
	mov r0, ip
	ands r0, r3
	adds r3, r0, #0
	adds r3, #0x3f
_0803928A:
	cmp r1, #0x20
	bne _08039292
	movs r1, #0
	b _080392B2
_08039292:
	cmp r1, #0x40
	bhi _0803929C
	adds r0, r1, #0
	subs r0, #0x2f
	b _080392AE
_0803929C:
	cmp r1, #0x60
	bls _080392A6
	adds r0, r1, #0
	subs r0, #0x56
	b _080392AE
_080392A6:
	cmp r1, #0x40
	bls _080392B2
	adds r0, r1, #0
	subs r0, #0x36
_080392AE:
	lsls r0, r0, #0x18
	lsrs r1, r0, #0x18
_080392B2:
	strh r1, [r3]
	adds r3, #2
	ldrb r2, [r5]
	cmp r2, #0
	bne _08039230
_080392BC:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80392C4
sub_80392C4: @ 0x080392C4
	mov r3, r8
	push {r3}
	sub sp, #8
	str r0, [sp]
	str r1, [sp, #4]
	adds r7, r0, #0
	mov r8, r1
	svc #0x13
	adds r0, r7, #0
	mov r1, r8
	add sp, #8
	pop {r3}
	mov r8, r3
	bx lr

	thumb_func_start sub_80392E0
sub_80392E0: @ 0x080392E0
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	ldr r1, _0803939C @ =0x04000208
	movs r0, #0
	strh r0, [r1]
	movs r0, #0
	bl sub_80391E8
	movs r0, #1
	bl sub_80391E8
	movs r0, #2
	bl sub_80391E8
	movs r0, #3
	bl sub_80391E8
	movs r0, #0xc0
	lsls r0, r0, #0x13
	movs r1, #0x80
	lsls r1, r1, #9
	bl sub_8037F3C
	ldr r0, _080393A0 @ =gUnknown_030008D0
	ldr r4, _080393A4 @ =0x06004000
	adds r1, r4, #0
	bl sub_80392C4
	movs r1, #0
	movs r0, #0xf
_0803931E:
	strh r1, [r4]
	adds r4, #2
	subs r0, #1
	cmp r0, #0
	bge _0803931E
	ldr r1, _080393A8 @ =0x060044B8
	movs r0, #0x80
	lsls r0, r0, #5
	str r0, [r1]
	ldr r0, _080393AC @ =0x060044C8
	movs r1, #0x80
	lsls r1, r1, #9
	str r1, [r0]
	adds r0, #0xc
	str r1, [r0]
	ldr r1, _080393B0 @ =0x060044FC
	ldr r0, _080393B4 @ =0x01111110
	str r0, [r1]
	ldr r0, _080393B8 @ =gStaticData_085A62C8
	ldr r2, [r0]
	movs r0, #0
	movs r1, #0
	bl sub_8039214
	ldr r2, _080393BC @ =gStaticData_085A62CC
	movs r0, #0
	movs r1, #5
	bl sub_8039214
	movs r0, #0xf
	movs r1, #5
	adds r2, r5, #0
	bl sub_8039214
	movs r0, #0
	movs r1, #7
	adds r2, r6, #0
	bl sub_8039214
	ldr r0, _080393C0 @ =0x05000002
	movs r2, #0
	strh r2, [r0]
	movs r1, #0xa0
	lsls r1, r1, #0x13
	ldr r3, _080393C4 @ =0x00007FFF
	adds r0, r3, #0
	strh r0, [r1]
	ldr r1, _080393C8 @ =0x04000008
	movs r0, #4
	strh r0, [r1]
	subs r1, #8
	movs r3, #0x80
	lsls r3, r3, #1
	adds r0, r3, #0
	strh r0, [r1]
	ldr r0, _080393CC @ =0x04000050
	strh r2, [r0]
	adds r0, #2
	strh r2, [r0]
	adds r0, #2
	strh r2, [r0]
_08039398:
	b _08039398
	.align 2, 0
_0803939C: .4byte 0x04000208
_080393A0: .4byte gUnknown_030008D0
_080393A4: .4byte 0x06004000
_080393A8: .4byte 0x060044B8
_080393AC: .4byte 0x060044C8
_080393B0: .4byte 0x060044FC
_080393B4: .4byte 0x01111110
_080393B8: .4byte gStaticData_085A62C8
_080393BC: .4byte gStaticData_085A62CC
_080393C0: .4byte 0x05000002
_080393C4: .4byte 0x00007FFF
_080393C8: .4byte 0x04000008
_080393CC: .4byte 0x04000050

	thumb_func_start sub_80393D0
sub_80393D0: @ 0x080393D0
	ldr r1, _080393F4 @ =0x0000FFFF
	strh r1, [r0, #0x14]
	movs r2, #0
	movs r3, #0
	ldr r1, _080393F8 @ =0x00004E20
	strh r1, [r0, #0x16]
	strb r2, [r0, #0x1c]
	movs r1, #6
	strh r1, [r0, #0x18]
	movs r1, #0xff
	strb r1, [r0, #0x1f]
	strb r2, [r0, #0x1d]
	strb r2, [r0, #0x1e]
	adds r1, r0, #0
	adds r1, #0x22
	strb r2, [r1]
	strh r3, [r0, #0x24]
	bx lr
	.align 2, 0
_080393F4: .4byte 0x0000FFFF
_080393F8: .4byte 0x00004E20

	thumb_func_start sub_80393FC
sub_80393FC: @ 0x080393FC
	push {lr}
	movs r2, #0
	str r2, [r0, #0x10]
	str r2, [r0, #0xc]
	strb r2, [r0, #0x1a]
	strb r2, [r0, #0x1b]
	adds r1, r0, #0
	adds r1, #0x20
	strb r2, [r1]
	adds r1, #1
	strb r2, [r1]
	bl sub_80393D0
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_803941C
sub_803941C: @ 0x0803941C
	push {r4, lr}
	adds r4, r0, #0
	bl sub_80393D0
	movs r1, #0
	movs r0, #2
	strb r0, [r4, #0x1b]
	str r1, [r4, #0x10]
	movs r0, #1
	strb r0, [r4, #0x1a]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start nullsub_39
nullsub_39: @ 0x08039438
	bx lr
	.align 2, 0

	thumb_func_start sub_803943C
sub_803943C: @ 0x0803943C
	push {r4, r5, r6, lr}
	adds r3, r0, #0
	adds r6, r2, #0
	ldr r0, [r3, #0xc]
	cmp r0, r6
	beq _08039510
	ldrb r0, [r3, #0x1a]
	cmp r0, #0
	beq _08039510
	ldrb r0, [r3, #0x18]
	ldrh r4, [r3, #0x18]
	ldrb r1, [r3, #0x1c]
	cmp r0, #0
	beq _080394E4
	adds r2, r1, #0
	cmp r2, #0
	bne _080394E4
	adds r1, r3, #0
	adds r1, #0x22
	ldrb r0, [r1]
	cmp r0, #0
	beq _08039472
	strb r2, [r1]
	ldr r0, [r3]
	ldr r0, [r0, #0x18]
	ldrh r0, [r0, #2]
	b _08039476
_08039472:
	ldrh r0, [r3, #0x16]
	adds r0, #1
_08039476:
	strh r0, [r3, #0x16]
	ldrh r2, [r3, #0x18]
	lsrs r1, r2, #8
	cmp r1, #0
	beq _0803948A
	movs r0, #0xff
	ands r0, r2
	lsls r0, r0, #8
	orrs r1, r0
	strh r1, [r3, #0x18]
_0803948A:
	ldrb r0, [r3, #0x18]
	subs r0, #1
	movs r4, #0
	strb r0, [r3, #0x1c]
	movs r0, #0x16
	ldrsh r1, [r3, r0]
	ldr r2, [r3]
	ldr r0, [r2, #0x18]
	ldrh r0, [r0, #2]
	cmp r1, r0
	blt _080394DA
	strh r4, [r3, #0x16]
	strh r4, [r3, #0x24]
	movs r5, #1
	strb r5, [r3, #0x1e]
	ldrh r0, [r3, #0x14]
	adds r0, #1
	strh r0, [r3, #0x14]
	movs r0, #0x14
	ldrsh r1, [r3, r0]
	ldr r0, [r2, #0x18]
	ldrh r0, [r0, #4]
	cmp r1, r0
	blt _080394DC
	adds r0, r3, #0
	adds r0, #0x20
	ldrb r0, [r0]
	cmp r0, #0
	beq _080394CA
	movs r0, #0
	strb r0, [r3, #0x1a]
	strh r4, [r3, #0x18]
_080394CA:
	adds r0, r3, #0
	adds r0, #0x21
	strb r5, [r0]
	ldr r0, [r3]
	ldr r0, [r0, #0x18]
	ldrh r0, [r0, #6]
	strh r0, [r3, #0x14]
	b _080394DC
_080394DA:
	strb r4, [r3, #0x1e]
_080394DC:
	movs r0, #1
	strb r0, [r3, #0x1d]
	ldrh r4, [r3, #0x18]
	b _080394EC
_080394E4:
	subs r0, r1, #1
	movs r1, #0
	strb r0, [r3, #0x1c]
	strb r1, [r3, #0x1d]
_080394EC:
	movs r0, #0xff
	ands r0, r4
	cmp r0, #0
	bne _080394FC
	adds r1, r3, #0
	adds r1, #0x21
	movs r0, #1
	strb r0, [r1]
_080394FC:
	str r6, [r3, #0xc]
	ldr r0, [r3, #0x10]
	cmp r0, #0
	bne _08039506
	str r6, [r3, #0x10]
_08039506:
	ldrb r0, [r3, #0x1b]
	cmp r0, #0
	beq _08039510
	subs r0, #1
	strb r0, [r3, #0x1b]
_08039510:
	movs r0, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1

	thumb_func_start sub_8039518
sub_8039518: @ 0x08039518
	push {r4, r5, lr}
	adds r5, r0, #0
	movs r2, #0
	str r2, [r5, #0x44]
	strb r2, [r5, #0x10]
	str r2, [r5, #0x3c]
	movs r1, #0
	ldr r0, _08039570 @ =0x00008AD0
	strh r0, [r5, #0x2a]
	movs r3, #1
	strb r3, [r5, #0x11]
	movs r0, #0xff
	strb r0, [r5, #0x15]
	movs r0, #1
	rsbs r0, r0, #0
	strb r0, [r5, #0x18]
	strb r1, [r5, #0xc]
	strb r1, [r5, #0x12]
	strb r1, [r5, #0xd]
	strb r1, [r5, #0xf]
	strb r1, [r5, #0xe]
	adds r0, r5, #0
	adds r0, #0x24
	strb r1, [r0]
	adds r0, #1
	strb r1, [r0]
	strh r2, [r5, #0x34]
	strh r2, [r5, #0x32]
	strh r2, [r5, #0x30]
	adds r0, #0x2d
	strb r3, [r0]
	ldr r4, _08039574 @ =gUnknown_03001618
	ldr r0, [r5, #4]
	ldrh r2, [r0, #2]
	movs r3, #0
	ldr r0, _08039578 @ =0x00000000
	ldr r1, _0803957C @ =0x00000001
	bl sub_8037A7C
	str r0, [r4]
	str r1, [r4, #4]
	movs r4, #0
	b _08039592
	.align 2, 0
_08039570: .4byte 0x00008AD0
_08039574: .4byte gUnknown_03001618
_08039578: .4byte 0x00000000
_0803957C: .4byte 0x00000001
_08039580:
	ldr r1, [r5, #8]
	lsls r0, r4, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r1, [r1]
	bl sub_803AD7C
	adds r4, #1
_08039592:
	ldr r0, [r5]
	ldr r0, [r0, #0xc]
	cmp r4, r0
	blo _08039580
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start nullsub_40
nullsub_40: @ 0x080395A0
	bx lr
	.align 2, 0

	thumb_func_start sub_80395A4
sub_80395A4: @ 0x080395A4
	push {r4, r5, r6, r7, lr}
	sub sp, #8
	adds r4, r0, #0
	adds r6, r1, #0
	adds r7, r2, #0
	ldr r0, [r4, #8]
	ldr r5, [r0]
	ldr r0, [r5]
	ldr r3, [r0, #8]
	adds r0, r5, #0
	bl sub_803AD84
	ldrb r0, [r5, #0x1b]
	cmp r0, #0
	beq _080395C6
	movs r0, #0
	str r0, [r4, #0x3c]
_080395C6:
	ldrb r0, [r5, #0x1a]
	cmp r0, #0
	beq _08039600
	ldrh r1, [r4, #0x34]
	movs r2, #0x34
	ldrsh r0, [r4, r2]
	cmp r0, #0
	beq _080395EA
	subs r0, r1, #1
	strh r0, [r4, #0x34]
	lsls r0, r0, #0x10
	cmp r0, #0
	bne _080395EA
	adds r0, r4, #0
	adds r1, r5, #0
	movs r2, #1
	bl sub_8039658
_080395EA:
	ldrh r0, [r5, #0x18]
	cmp r0, #0
	beq _08039600
	ldrb r0, [r5, #0x1d]
	cmp r0, #0
	beq _08039600
	adds r0, r4, #0
	adds r1, r5, #0
	movs r2, #0
	bl sub_8039658
_08039600:
	ldr r0, [r4, #0x3c]
	ldrb r1, [r4, #0x1f]
	cmp r0, #0
	beq _08039620
	cmp r1, #0
	bne _08039620
	ldrb r0, [r4, #0x1e]
	cmp r0, #0
	beq _08039624
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_80398DC
	ldrb r0, [r4, #0x1e]
	subs r0, #1
	b _08039622
_08039620:
	subs r0, r1, #1
_08039622:
	strb r0, [r4, #0x1f]
_08039624:
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_8039AA4
	ldrb r1, [r4, #0xc]
	cmp r1, #0
	bne _0803964C
	ldr r0, [r5]
	ldr r0, [r0, #0x18]
	str r0, [sp]
	str r1, [sp, #4]
	adds r0, r4, #0
	adds r1, r5, #0
	adds r2, r6, #0
	adds r3, r7, #0
	bl sub_8039B44
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	b _0803964E
_0803964C:
	movs r0, #0
_0803964E:
	add sp, #8
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8039658
sub_8039658: @ 0x08039658
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r4, r0, #0
	adds r7, r1, #0
	lsls r2, r2, #0x18
	movs r0, #0
	strh r0, [r4, #0x1a]
	strh r0, [r4, #0x28]
	strh r0, [r4, #0x34]
	cmp r2, #0
	bne _0803972C
	ldrb r0, [r7, #0x1e]
	cmp r0, #0
	beq _0803969C
	ldr r0, [r7]
	ldr r2, [r0, #0x18]
	ldr r1, [r4]
	movs r3, #0x14
	ldrsh r0, [r7, r3]
	ldr r1, [r1, #0x18]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrh r1, [r0]
	ldr r0, [r2, #0xc]
	adds r0, r0, r1
	str r0, [r4, #0x40]
	movs r1, #0
	strb r1, [r4, #0xf]
	ldrb r1, [r0]
	strb r1, [r4, #0xe]
	adds r0, #1
	str r0, [r4, #0x40]
_0803969C:
	ldrb r0, [r4, #0xe]
	cmp r0, #0
	beq _080396A4
	b _0803980A
_080396A4:
	ldrb r0, [r4, #0xf]
	cmp r0, #0
	beq _080396B0
	subs r0, #1
	strb r0, [r4, #0xf]
	b _0803980A
_080396B0:
	ldr r2, [r4, #0x40]
	ldrb r3, [r2]
	adds r0, r3, #0
	cmp r0, #0xff
	bne _080396C6
	ldrb r0, [r2, #1]
	subs r0, #1
	strb r0, [r4, #0xf]
	adds r0, r2, #2
	str r0, [r4, #0x40]
	b _0803980A
_080396C6:
	movs r0, #0x80
	ands r0, r3
	cmp r0, #0
	beq _080396FC
	movs r1, #0x7f
	ands r1, r3
	cmp r1, #0
	bne _080396DC
	adds r0, r2, #1
	str r0, [r4, #0x40]
	b _0803980A
_080396DC:
	cmp r1, #0x79
	bhi _080396EC
	mov r8, r1
	ldrb r6, [r2, #1]
	movs r5, #0
	mov sb, r5
	adds r0, r2, #2
	b _0803970A
_080396EC:
	movs r0, #0
	mov r8, r0
	movs r6, #0
	ldrb r1, [r2, #1]
	mov sb, r1
	ldrb r5, [r2, #2]
	adds r0, r2, #3
	b _0803970A
_080396FC:
	ldrb r3, [r2]
	mov r8, r3
	ldrb r6, [r2, #1]
	ldrb r0, [r2, #2]
	mov sb, r0
	ldrb r5, [r2, #3]
	adds r0, r2, #4
_0803970A:
	str r0, [r4, #0x40]
	mov r1, sb
	cmp r1, #0xe
	bne _0803973E
	lsrs r0, r5, #4
	cmp r0, #0xd
	bne _0803973E
	movs r0, #0xf
	ands r5, r0
	strh r5, [r4, #0x34]
	adds r0, r4, #0
	adds r0, #0x50
	mov r2, r8
	strb r2, [r0]
	adds r0, #1
	strb r6, [r0]
	b _0803980A
_0803972C:
	adds r0, r4, #0
	adds r0, #0x50
	ldrb r0, [r0]
	mov r8, r0
	adds r0, r4, #0
	adds r0, #0x51
	ldrb r6, [r0]
	movs r5, #0
	mov sb, r5
_0803973E:
	mov r3, sb
	cmp r3, #3
	beq _0803974C
	adds r0, r4, #0
	mov r1, r8
	bl sub_8039818
_0803974C:
	ldr r0, [r7]
	ldr r3, [r0, #0x18]
	adds r0, r4, #0
	adds r1, r7, #0
	adds r2, r6, #0
	bl sub_803985C
	mov r0, sb
	subs r0, #1
	cmp r0, #0xe
	bhi _0803980A
	lsls r0, r0, #2
	ldr r1, _0803976C @ =_08039770
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0803976C: .4byte _08039770
_08039770: @ jump table
	.4byte _080397AC @ case 0
	.4byte _080397B0 @ case 1
	.4byte _080397B6 @ case 2
	.4byte _0803980A @ case 3
	.4byte _0803980A @ case 4
	.4byte _0803980A @ case 5
	.4byte _080397D6 @ case 6
	.4byte _0803980A @ case 7
	.4byte _0803980A @ case 8
	.4byte _080397EA @ case 9
	.4byte _080397EE @ case 10
	.4byte _080397F4 @ case 11
	.4byte _080397F8 @ case 12
	.4byte _0803980A @ case 13
	.4byte _08039804 @ case 14
_080397AC:
	strh r5, [r4, #0x28]
	b _0803980A
_080397B0:
	rsbs r0, r5, #0
	strh r0, [r4, #0x28]
	b _0803980A
_080397B6:
	cmp r5, #0
	beq _0803980A
	mov r0, r8
	subs r0, #2
	lsls r0, r0, #5
	strh r0, [r4, #0x30]
	movs r1, #0x30
	ldrsh r0, [r4, r1]
	movs r2, #0x26
	ldrsh r1, [r4, r2]
	subs r0, r0, r1
	adds r1, r5, #0
	bl sub_803ADB4
	strh r0, [r4, #0x32]
	b _0803980A
_080397D6:
	lsrs r0, r5, #4
	lsls r1, r5, #8
	movs r3, #0xf0
	lsls r3, r3, #4
	adds r2, r3, #0
	ands r1, r2
	orrs r0, r1
	strh r0, [r7, #0x18]
	subs r0, #1
	b _08039808
_080397EA:
	strh r5, [r4, #0x1a]
	b _0803980A
_080397EE:
	rsbs r0, r5, #0
	strh r0, [r4, #0x1a]
	b _0803980A
_080397F4:
	strb r5, [r4, #0x15]
	b _0803980A
_080397F8:
	adds r1, r7, #0
	adds r1, #0x22
	movs r0, #1
	strb r0, [r1]
	strh r5, [r7, #0x24]
	b _0803980A
_08039804:
	strh r5, [r7, #0x18]
	subs r0, r5, #1
_08039808:
	strb r0, [r7, #0x1c]
_0803980A:
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8039818
sub_8039818: @ 0x08039818
	adds r2, r0, #0
	adds r3, r1, #0
	cmp r3, #1
	bne _08039844
	ldr r0, [r2, #0x3c]
	cmp r0, #0
	beq _0803983C
	ldr r0, [r0, #0x7c]
	ldrb r0, [r0, #1]
	cmp r0, #0xff
	bne _0803983C
	movs r0, #0
	ldr r1, _08039858 @ =0x00008AD0
	strh r1, [r2, #0x2a]
	strh r0, [r2, #0x2c]
	movs r0, #0x80
	lsls r0, r0, #0x18
	str r0, [r2, #0x4c]
_0803983C:
	adds r1, r2, #0
	adds r1, #0x22
	movs r0, #1
	strb r0, [r1]
_08039844:
	cmp r3, #1
	bls _08039856
	subs r0, r3, #2
	lsls r0, r0, #5
	movs r1, #0
	strh r0, [r2, #0x26]
	adds r0, r2, #0
	adds r0, #0x22
	strb r1, [r0]
_08039856:
	bx lr
	.align 2, 0
_08039858: .4byte 0x00008AD0

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

	thumb_func_start sub_8039FFC
sub_8039FFC: @ 0x08039FFC
	adds r2, r0, #0
	ldr r3, [r2, #0x3c]
	ldrb r0, [r3, #9]
	cmp r0, #0
	beq _0803A034
	adds r1, r2, #0
	adds r1, #0x23
	ldrb r0, [r1]
	cmp r0, #0
	bne _0803A01E
	ldrb r0, [r3, #0xa]
	ldrh r1, [r2, #0x3a]
	adds r1, r1, r0
	movs r0, #0x3f
	ands r1, r0
	strh r1, [r2, #0x3a]
	b _0803A022
_0803A01E:
	subs r0, #1
	strb r0, [r1]
_0803A022:
	ldr r1, _0803A038 @ =gStaticData_085A9EAC
	ldrh r0, [r2, #0x3a]
	adds r0, r0, r1
	movs r1, #0
	ldrsb r1, [r0, r1]
	ldr r0, [r2, #0x3c]
	ldrb r0, [r0, #9]
	muls r0, r1, r0
	asrs r0, r0, #8
_0803A034:
	strh r0, [r2, #0x2e]
	bx lr
	.align 2, 0
_0803A038: .4byte gStaticData_085A9EAC

	thumb_func_start sub_803A03C
sub_803A03C: @ 0x0803A03C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r2, r0, #0
	ldrb r0, [r2, #0x12]
	cmp r0, #0
	beq _0803A0F8
	ldrb r0, [r2, #0x14]
	subs r0, #1
	strb r0, [r2, #0x14]
	movs r1, #0xff
	mov r8, r1
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0803A0F8
	ldr r6, [r2, #0x48]
	ldr r0, [r2, #0x3c]
	mov ip, r0
	ldrb r1, [r2, #0x10]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r0, r0, #2
	add r0, ip
	ldrh r0, [r0, #0x24]
	strb r0, [r2, #0x14]
	ldrb r7, [r2, #0x13]
	movs r0, #0x13
	ldrsb r0, [r2, r0]
	cmp r0, #0
	ble _0803A0B6
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r0, r0, #2
	mov r5, ip
	adds r5, #0x20
	adds r0, r5, r0
	ldr r0, [r0]
	adds r4, r6, r0
	str r4, [r2, #0x48]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r3, r0, #2
	mov r0, ip
	adds r0, #0x1c
	adds r0, r0, r3
	ldr r1, [r0]
	adds r1, r4, r1
	mov r0, ip
	adds r0, #0x18
	adds r0, r0, r3
	ldr r0, [r0]
	cmp r1, r0
	ble _0803A0EA
	adds r0, r5, r3
	ldr r0, [r0]
	lsls r0, r0, #1
	subs r0, r4, r0
	str r0, [r2, #0x48]
	mov r0, r8
	orrs r0, r7
	b _0803A0E8
_0803A0B6:
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r0, r0, #2
	mov r4, ip
	adds r4, #0x20
	adds r0, r4, r0
	ldr r0, [r0]
	subs r3, r6, r0
	str r3, [r2, #0x48]
	ldrb r1, [r2, #0x10]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r1, r0, #2
	mov r0, ip
	adds r0, #0x14
	adds r0, r0, r1
	ldr r0, [r0]
	cmp r3, r0
	bge _0803A0EA
	adds r0, r4, r1
	ldr r0, [r0]
	lsls r0, r0, #1
	adds r0, r3, r0
	str r0, [r2, #0x48]
	movs r0, #1
_0803A0E8:
	strb r0, [r2, #0x13]
_0803A0EA:
	lsls r0, r6, #0xb
	ldr r1, [r2, #0x44]
	subs r1, r1, r0
	ldr r0, [r2, #0x48]
	lsls r0, r0, #0xb
	adds r1, r1, r0
	str r1, [r2, #0x44]
_0803A0F8:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_803A104
sub_803A104: @ 0x0803A104
	adds r2, r0, #0
	movs r0, #0
	str r0, [r2, #0x44]
	strb r0, [r2, #0x10]
	str r0, [r2, #0x3c]
	movs r1, #0
	ldr r0, _0803A150 @ =0x00008AD0
	strh r0, [r2, #0x2a]
	movs r0, #1
	strb r0, [r2, #0x11]
	movs r0, #0xff
	strb r0, [r2, #0x15]
	movs r0, #1
	rsbs r0, r0, #0
	strb r0, [r2, #0x18]
	strb r1, [r2, #0xc]
	strb r1, [r2, #0x12]
	strb r1, [r2, #0xd]
	adds r0, r2, #0
	adds r0, #0x24
	strb r1, [r0]
	adds r0, #1
	strb r1, [r0]
	movs r0, #0x80
	lsls r0, r0, #0x18
	str r0, [r2, #0x4c]
	ldr r0, _0803A154 @ =gUnknown_03001630
	ldr r0, [r0]
	adds r0, #0x42
	ldrb r0, [r0]
	movs r1, #1
	cmp r0, #0
	beq _0803A148
	movs r1, #2
_0803A148:
	adds r0, r2, #0
	adds r0, #0x52
	strb r1, [r0]
	bx lr
	.align 2, 0
_0803A150: .4byte 0x00008AD0
_0803A154: .4byte gUnknown_03001630

	thumb_func_start sub_803A158
sub_803A158: @ 0x0803A158
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	adds r4, r0, #0
	mov sb, r1
	mov sl, r2
	ldr r0, [r4, #8]
	ldr r6, [r0]
	ldrb r0, [r6, #0x1b]
	cmp r0, #0
	beq _0803A178
	movs r0, #0
	str r0, [r4, #0x3c]
_0803A178:
	ldrb r0, [r6, #0x1a]
	cmp r0, #0
	beq _0803A1C6
	movs r0, #0x24
	adds r0, r0, r4
	mov r8, r0
	ldrb r0, [r0]
	cmp r0, #1
	beq _0803A198
	cmp r0, #0
	beq _0803A1C6
	adds r0, r4, #0
	adds r0, #0x25
	ldrb r0, [r0]
	cmp r0, #0
	beq _0803A1C6
_0803A198:
	ldrb r5, [r6, #0x1b]
	cmp r5, #0
	bne _0803A1C6
	mov r0, r8
	ldrb r1, [r0]
	adds r0, r4, #0
	bl sub_8039818
	adds r7, r4, #0
	adds r7, #0x25
	ldrb r2, [r7]
	ldr r0, [r4]
	ldr r3, [r0, #0x18]
	adds r0, r4, #0
	adds r1, r6, #0
	bl sub_803985C
	movs r0, #0
	strh r5, [r4, #0x1a]
	strh r5, [r4, #0x28]
	strb r0, [r7]
	mov r1, r8
	strb r0, [r1]
_0803A1C6:
	ldr r0, [r4, #0x3c]
	ldrb r1, [r4, #0x1f]
	cmp r0, #0
	beq _0803A1E6
	cmp r1, #0
	bne _0803A1E6
	ldrb r0, [r4, #0x1e]
	cmp r0, #0
	beq _0803A1EA
	adds r0, r4, #0
	adds r1, r6, #0
	bl sub_80398DC
	ldrb r0, [r4, #0x1e]
	subs r0, #1
	b _0803A1E8
_0803A1E6:
	subs r0, r1, #1
_0803A1E8:
	strb r0, [r4, #0x1f]
_0803A1EA:
	adds r0, r4, #0
	adds r1, r6, #0
	bl sub_8039AA4
	ldrb r0, [r4, #0xc]
	cmp r0, #0
	bne _0803A214
	ldr r0, [r4]
	ldr r0, [r0, #0x18]
	str r0, [sp]
	movs r0, #1
	str r0, [sp, #4]
	adds r0, r4, #0
	adds r1, r6, #0
	mov r2, sb
	mov r3, sl
	bl sub_8039B44
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	b _0803A216
_0803A214:
	movs r0, #0
_0803A216:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start nullsub_41
nullsub_41: @ 0x0803A228
	bx lr
	.align 2, 0

	thumb_func_start sub_803A22C
sub_803A22C: @ 0x0803A22C
	push {r4, r5, lr}
	adds r4, r0, #0
	movs r0, #1
	str r0, [r4, #0xc]
	movs r5, #0
	b _0803A24A
_0803A238:
	ldr r1, [r4, #8]
	lsls r0, r5, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r1, [r1]
	bl sub_803AD7C
	adds r5, #1
_0803A24A:
	ldr r0, _0803A260 @ =gUnknown_03001630
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	cmp r0, #0
	bne _0803A264
	ldr r0, [r4]
	ldr r0, [r0, #0xc]
	ldr r1, [r4, #0x14]
	adds r0, r0, r1
	b _0803A268
	.align 2, 0
_0803A260: .4byte gUnknown_03001630
_0803A264:
	ldr r0, [r4]
	ldr r0, [r0, #0xc]
_0803A268:
	cmp r5, r0
	blo _0803A238
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start nullsub_42
nullsub_42: @ 0x0803A274
	bx lr
	.align 2, 0

	thumb_func_start sub_803A278
sub_803A278: @ 0x0803A278
	push {r4, lr}
	sub sp, #0x2c
	ldr r0, [r0, #4]
	ldrh r2, [r0, #4]
	ldrb r0, [r0, #1]
	muls r0, r2, r0
	lsls r0, r0, #1
	ldr r4, _0803A2C4 @ =gUnknown_03001630
	ldr r2, [r4]
	ldr r3, [r2, #0x20]
	str r0, [sp, #0x14]
	str r1, [sp, #0x18]
	str r3, [sp, #0x1c]
	ldr r0, [r2, #0x28]
	str r0, [sp, #0x20]
	ldr r0, [r2, #0x24]
	str r0, [sp, #0x24]
	add r1, sp, #0x14
	mov r0, sp
	movs r2, #0x14
	bl sub_800014C
	mov r0, sp
	str r0, [sp, #0x28]
	ldr r3, [r4]
	adds r3, #0x9c
	adds r1, r3, #0
	ldr r0, [sp, #0x28]
	mov r2, pc
	adds r2, #5
	mov lr, r2
	bx r1
_0803A2B8:
	nop
	add sp, #0x2c
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0803A2C4: .4byte gUnknown_03001630

	thumb_func_start sub_803A2C8
sub_803A2C8: @ 0x0803A2C8
	push {r4, r5, r6, lr}
	sub sp, #0x3c
	ldr r0, [r0, #4]
	ldrh r4, [r0, #4]
	ldrb r0, [r0, #1]
	muls r0, r4, r0
	lsls r0, r0, #1
	ldr r6, _0803A320 @ =gUnknown_03001630
	ldr r4, [r6]
	ldr r5, [r4, #0x20]
	str r1, [sp, #0x1c]
	str r0, [sp, #0x20]
	movs r0, #0x55
	subs r0, r0, r2
	str r0, [sp, #0x24]
	str r3, [sp, #0x28]
	str r5, [sp, #0x2c]
	ldr r0, [r4, #0x28]
	str r0, [sp, #0x30]
	ldr r0, [r4, #0x24]
	str r0, [sp, #0x34]
	add r1, sp, #0x1c
	mov r0, sp
	movs r2, #0x1c
	bl sub_800014C
	mov r0, sp
	str r0, [sp, #0x38]
	ldr r3, [r6]
	movs r0, #0xbe
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r3, [r3]
	adds r1, r3, #0
	ldr r0, [sp, #0x38]
	mov r2, pc
	adds r2, #5
	mov lr, r2
	bx r1
	nop
	thumb_func_start sub_803A318
sub_803A318: @ 0x0803A318
	add	sp, #60	@ 0x3c
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
_0803A320: .4byte gUnknown_03001630

	thumb_func_start sub_803A324
sub_803A324: @ 0x0803A324
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	mov r8, r1
	mov sb, r2
	movs r6, #0
	ldr r0, _0803A494 @ =gUnknown_03001630
	ldr r0, [r0]
	adds r0, #0x41
	ldrb r0, [r0]
	cmp r0, #0
	bne _0803A382
	movs r4, #0
	ldr r0, [r5]
	ldr r0, [r0, #0x18]
	ldr r0, [r0]
	cmp r6, r0
	bhs _0803A382
_0803A34C:
	ldr r0, [r5, #8]
	lsls r1, r4, #2
	adds r0, r1, r0
	ldr r0, [r0]
	movs r2, #0
	cmp r6, #0
	bne _0803A35C
	movs r2, #1
_0803A35C:
	strb r2, [r0, #0xd]
	ldr r0, [r5, #8]
	adds r0, r1, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r3, [r1, #8]
	mov r1, r8
	mov r2, sb
	bl sub_803AD84
	orrs r6, r0
	lsls r0, r6, #0x18
	lsrs r6, r0, #0x18
	adds r4, #1
	ldr r0, [r5]
	ldr r0, [r0, #0x18]
	ldr r0, [r0]
	cmp r4, r0
	blo _0803A34C
_0803A382:
	ldr r0, _0803A494 @ =gUnknown_03001630
	ldr r1, [r0]
	adds r0, r1, #0
	adds r0, #0x40
	ldrb r0, [r0]
	cmp r0, #0
	beq _0803A410
	ldr r3, [r5]
	ldr r7, [r5, #0x14]
	cmp r6, #0
	bne _0803A3D0
	ldr r1, [r1, #4]
	ldr r0, [r1, #0x30]
	ldr r0, [r0, #8]
	ldr r0, [r0, #0x18]
	ldrb r0, [r0, #0x1b]
	cmp r0, #0
	bne _0803A3B0
	ldrh r1, [r1, #0xc]
	movs r0, #0x20
	ands r0, r1
	cmp r0, #0
	beq _0803A3D0
_0803A3B0:
	ldr r0, [r5, #4]
	ldrh r1, [r0, #4]
	ldrb r0, [r0, #1]
	muls r0, r1, r0
	lsls r0, r0, #1
	mov r2, r8
	movs r6, #1
	ldr r3, [r5]
	ldr r7, [r5, #0x14]
	cmp r0, #0
	ble _0803A3D0
	movs r1, #0
_0803A3C8:
	stm r2!, {r1}
	subs r0, #4
	cmp r0, #0
	bgt _0803A3C8
_0803A3D0:
	ldr r4, [r3, #0xc]
	adds r0, r4, r7
	cmp r4, r0
	bhs _0803A410
_0803A3D8:
	ldr r0, [r5, #8]
	lsls r1, r4, #2
	adds r0, r1, r0
	ldr r0, [r0]
	movs r2, #0
	cmp r6, #0
	bne _0803A3E8
	movs r2, #1
_0803A3E8:
	strb r2, [r0, #0xd]
	ldr r0, [r5, #8]
	adds r0, r1, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r3, [r1, #8]
	mov r1, r8
	mov r2, sb
	bl sub_803AD84
	orrs r6, r0
	lsls r0, r6, #0x18
	lsrs r6, r0, #0x18
	adds r4, #1
	ldr r0, [r5]
	ldr r0, [r0, #0xc]
	ldr r1, [r5, #0x14]
	adds r0, r0, r1
	cmp r4, r0
	blo _0803A3D8
_0803A410:
	ldr r0, [r5]
	ldr r1, [r0, #0x18]
	ldr r1, [r1, #4]
	adds r3, r0, #0
	cmp r1, #0
	beq _0803A480
	ldr r4, _0803A494 @ =gUnknown_03001630
	cmp r6, #0
	bne _0803A43E
	ldr r0, [r5, #4]
	ldrh r1, [r0, #4]
	ldrb r0, [r0, #1]
	muls r0, r1, r0
	lsls r0, r0, #1
	mov r1, r8
	movs r6, #1
	cmp r0, #0
	ble _0803A43E
	movs r2, #0
_0803A436:
	stm r1!, {r2}
	subs r0, #4
	cmp r0, #0
	bgt _0803A436
_0803A43E:
	movs r7, #1
	ldr r0, [r4]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r1, [r0]
	ldr r0, [r1, #4]
	ldrb r0, [r0, #0x1f]
	cmp r0, #0
	beq _0803A474
	movs r4, #0
	ldr r0, [r3, #0xc]
	cmp r4, r0
	bhs _0803A474
	adds r2, r0, #0
_0803A45E:
	ldr r0, [r1, #0xc]
	ldrb r0, [r0, #0x18]
	cmp r0, #0
	beq _0803A468
	movs r7, #0
_0803A468:
	adds r1, #4
	adds r4, #1
	cmp r4, r2
	bhs _0803A474
	cmp r7, #0
	bne _0803A45E
_0803A474:
	cmp r7, #0
	bne _0803A480
	adds r0, r5, #0
	mov r1, r8
	bl sub_803A278
_0803A480:
	ldr r0, _0803A494 @ =gUnknown_03001630
	ldr r0, [r0]
	adds r0, #0x41
	ldrb r0, [r0]
	cmp r0, #0
	bne _0803A4CC
	ldr r0, [r5]
	ldr r1, [r0, #0x18]
	ldr r4, [r1]
	b _0803A4C6
	.align 2, 0
_0803A494: .4byte gUnknown_03001630
_0803A498:
	ldr r0, [r5, #8]
	lsls r1, r4, #2
	adds r0, r1, r0
	ldr r0, [r0]
	movs r2, #0
	cmp r6, #0
	bne _0803A4A8
	movs r2, #1
_0803A4A8:
	strb r2, [r0, #0xd]
	ldr r0, [r5, #8]
	adds r0, r1, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r3, [r1, #8]
	mov r1, r8
	mov r2, sb
	bl sub_803AD84
	orrs r6, r0
	lsls r0, r6, #0x18
	lsrs r6, r0, #0x18
	adds r4, #1
	ldr r0, [r5]
_0803A4C6:
	ldr r0, [r0, #0xc]
	cmp r4, r0
	blo _0803A498
_0803A4CC:
	ldr r0, _0803A5A4 @ =gUnknown_03001630
	ldr r1, [r0]
	movs r2, #0xc0
	lsls r2, r2, #1
	adds r0, r1, r2
	ldr r2, [r0]
	cmp r2, #0x55
	bls _0803A4DE
	movs r2, #0x55
_0803A4DE:
	str r2, [r0]
	movs r3, #0xbe
	lsls r3, r3, #1
	adds r0, r1, r3
	ldr r0, [r0]
	cmp r0, #0
	beq _0803A4FC
	cmp r2, #0
	beq _0803A4FC
	ldr r0, [r5]
	ldr r3, [r0, #0xc]
	adds r0, r5, #0
	mov r1, r8
	bl sub_803A2C8
_0803A4FC:
	ldr r0, _0803A5A4 @ =gUnknown_03001630
	ldr r1, [r0]
	adds r0, r1, #0
	adds r0, #0x40
	ldrb r0, [r0]
	cmp r0, #0
	bne _0803A58A
	ldr r3, [r5]
	ldr r7, [r5, #0x14]
	cmp r6, #0
	bne _0803A54A
	ldr r1, [r1, #4]
	ldr r0, [r1, #0x30]
	ldr r0, [r0, #8]
	ldr r0, [r0, #0x18]
	ldrb r0, [r0, #0x1b]
	cmp r0, #0
	bne _0803A52A
	ldrh r1, [r1, #0xc]
	movs r0, #0x20
	ands r0, r1
	cmp r0, #0
	beq _0803A54A
_0803A52A:
	ldr r0, [r5, #4]
	ldrh r1, [r0, #4]
	ldrb r0, [r0, #1]
	muls r0, r1, r0
	lsls r0, r0, #1
	mov r2, r8
	movs r6, #1
	ldr r3, [r5]
	ldr r7, [r5, #0x14]
	cmp r0, #0
	ble _0803A54A
	movs r1, #0
_0803A542:
	stm r2!, {r1}
	subs r0, #4
	cmp r0, #0
	bgt _0803A542
_0803A54A:
	ldr r4, [r3, #0xc]
	adds r0, r4, r7
	cmp r4, r0
	bhs _0803A58A
_0803A552:
	ldr r0, [r5, #8]
	lsls r1, r4, #2
	adds r0, r1, r0
	ldr r0, [r0]
	movs r2, #0
	cmp r6, #0
	bne _0803A562
	movs r2, #1
_0803A562:
	strb r2, [r0, #0xd]
	ldr r0, [r5, #8]
	adds r0, r1, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r3, [r1, #8]
	mov r1, r8
	mov r2, sb
	bl sub_803AD84
	orrs r6, r0
	lsls r0, r6, #0x18
	lsrs r6, r0, #0x18
	adds r4, #1
	ldr r0, [r5]
	ldr r0, [r0, #0xc]
	ldr r1, [r5, #0x14]
	adds r0, r0, r1
	cmp r4, r0
	blo _0803A552
_0803A58A:
	ldr r0, _0803A5A4 @ =gUnknown_03001630
	ldr r0, [r0]
	adds r0, #0x41
	movs r1, #0
	strb r1, [r0]
	adds r0, r6, #0
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0803A5A4: .4byte gUnknown_03001630

	thumb_func_start sub_803A5A8
sub_803A5A8: @ 0x0803A5A8
	push {r4, r5, r6, lr}
	sub sp, #0x14
	adds r4, r0, #0
	adds r6, r1, #0
	ldr r0, [r4]
	ldr r1, [r0, #0xc]
	ldr r0, [r4, #0x14]
	adds r0, r1, r0
	cmp r0, #1
	beq _0803A5C2
	adds r0, #8
	lsls r3, r0, #6
	b _0803A5C6
_0803A5C2:
	movs r3, #0x80
	lsls r3, r3, #3
_0803A5C6:
	ldr r1, [r4, #0x10]
	ldr r0, [r4, #4]
	ldrh r2, [r0, #4]
	ldrb r0, [r0, #1]
	adds r5, r2, #0
	muls r5, r0, r5
	str r1, [sp]
	str r6, [sp, #4]
	str r5, [sp, #8]
	str r3, [sp, #0xc]
	ldr r3, [r4]
	ldr r0, [r4, #0xc]
	adds r2, r0, #0
	adds r0, #1
	str r0, [r4, #0xc]
	ldr r3, [r3, #8]
	adds r0, r4, #0
	bl sub_803AD84
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0803A610
	mov r0, sp
	str r0, [sp, #0x10]
	ldr r0, _0803A60C @ =gUnknown_03001630
	ldr r3, [r0]
	adds r3, #0x48
	adds r1, r3, #0
	ldr r0, [sp, #0x10]
	mov r2, pc
	adds r2, #5
	mov lr, r2
	bx r1
	thumb_func_start sub_803A608
sub_803A608: @ 0x0803A608
	nop			@ (mov r8, r8)
	b _0803A61E
_0803A60C: .4byte gUnknown_03001630
_0803A610:
	cmp r5, #0
	ble _0803A61E
	movs r0, #0
_0803A616:
	stm r6!, {r0}
	subs r5, #4
	cmp r5, #0
	bgt _0803A616
_0803A61E:
	add sp, #0x14
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0803A628:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
gStaticData_0803A630:
	.byte 0x60, 0x00, 0x2D, 0xE9, 0x0C, 0x50, 0x90, 0xE5, 0x08, 0x20, 0x90, 0xE5, 0x04, 0x10, 0x90, 0xE5
	.byte 0x00, 0x00, 0x90, 0xE5, 0x01, 0x20, 0x82, 0xE0, 0xF2, 0x60, 0xD0, 0xE0, 0x95, 0x06, 0x06, 0xE0
	.byte 0x46, 0x65, 0xA0, 0xE1, 0x80, 0x00, 0x76, 0xE3, 0x7F, 0x60, 0xE0, 0xB3, 0x7F, 0x00, 0x56, 0xE3
	.byte 0x7F, 0x60, 0xA0, 0xC3, 0x01, 0x60, 0xC1, 0xE4, 0x02, 0x00, 0x51, 0xE1, 0xF5, 0xFF, 0xFF, 0xBA
	.byte 0x60, 0x00, 0xBD, 0xE8, 0x0E, 0x00, 0xA0, 0xE1, 0x10, 0xFF, 0x2F, 0xE1
gStaticData_0803A67C:
	.byte 0xF0, 0x0F, 0x2D, 0xE9, 0x0C, 0x10, 0x90, 0xE5, 0x08, 0x70, 0x90, 0xE5, 0x04, 0x20, 0x90, 0xE5
	.byte 0x00, 0x00, 0x90, 0xE5, 0x98, 0x40, 0x8F, 0xE2, 0x04, 0x50, 0x94, 0xE5, 0x00, 0x40, 0x94, 0xE5
	.byte 0xCD, 0x8F, 0xA0, 0xE3, 0x98, 0x07, 0x07, 0xE0, 0x47, 0x74, 0xA0, 0xE1, 0x7F, 0x90, 0xA0, 0xE3
	.byte 0x09, 0x94, 0xA0, 0xE1, 0x91, 0x09, 0x09, 0xE0, 0xF0, 0x80, 0xD0, 0xE1, 0x08, 0x84, 0xA0, 0xE1
	.byte 0x05, 0x60, 0x48, 0xE0, 0x04, 0x60, 0x46, 0xE0, 0x09, 0x00, 0x76, 0xE1, 0x09, 0x60, 0xE0, 0xB1
	.byte 0x09, 0x00, 0x56, 0xE1, 0x09, 0x60, 0xA0, 0xC1, 0x96, 0x07, 0x0A, 0xE0, 0x4A, 0x54, 0x85, 0xE0
	.byte 0x09, 0x00, 0x75, 0xE1, 0x09, 0x50, 0xE0, 0xB1, 0x09, 0x00, 0x55, 0xE1, 0x09, 0x50, 0xA0, 0xC1
	.byte 0x95, 0x07, 0x0A, 0xE0, 0x4A, 0x44, 0x84, 0xE0, 0x09, 0x00, 0x74, 0xE1, 0x09, 0x40, 0xE0, 0xB1
	.byte 0x09, 0x00, 0x54, 0xE1, 0x09, 0x40, 0xA0, 0xC1, 0x44, 0xA4, 0xA0, 0xE1, 0xB0, 0xA0, 0xC0, 0xE1
	.byte 0x02, 0x00, 0x80, 0xE2, 0x02, 0x20, 0x52, 0xE2, 0x00, 0x00, 0x52, 0xE3, 0xE5, 0xFF, 0xFF, 0xCA
	.byte 0x0C, 0x00, 0x8F, 0xE2, 0x00, 0x40, 0x80, 0xE5, 0x04, 0x50, 0x80, 0xE5, 0xF0, 0x0F, 0xBD, 0xE8
	.byte 0x1E, 0xFF, 0x2F, 0xE1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x49, 0x4C, 0x54
gStaticData_0803A73C:
	.byte 0xF0, 0x01, 0x2D, 0xE9
	.byte 0xC8, 0x50, 0x9F, 0xE5, 0x00, 0x40, 0x90, 0xE5, 0x0C, 0x30, 0x90, 0xE5, 0x10, 0x20, 0x90, 0xE5
	.byte 0x08, 0x10, 0x90, 0xE5, 0x04, 0x00, 0x90, 0xE5, 0x00, 0x40, 0x84, 0xE0, 0x03, 0x00, 0x55, 0xE1
	.byte 0x00, 0x50, 0xA0, 0x03, 0x03, 0x70, 0x85, 0xE0, 0x00, 0x80, 0x92, 0xE5, 0x08, 0x70, 0x47, 0xE0
	.byte 0x03, 0x00, 0x57, 0xE1, 0x03, 0x70, 0x47, 0xA0, 0xF7, 0x70, 0x91, 0xE1, 0x04, 0x80, 0x92, 0xE5
	.byte 0x98, 0x07, 0x17, 0xE0, 0x47, 0x62, 0xA0, 0xE1, 0x08, 0x80, 0x92, 0xE5, 0x00, 0x00, 0x58, 0xE3
	.byte 0x11, 0x00, 0x00, 0x0A, 0x03, 0x70, 0x85, 0xE0, 0x08, 0x70, 0x47, 0xE0, 0x03, 0x00, 0x57, 0xE1
	.byte 0x03, 0x70, 0x47, 0xA0, 0xF7, 0x70, 0x91, 0xE1, 0x0C, 0x80, 0x92, 0xE5, 0x98, 0x07, 0x17, 0xE0
	.byte 0x47, 0x62, 0x86, 0xE0, 0x10, 0x80, 0x92, 0xE5, 0x00, 0x00, 0x58, 0xE3, 0x06, 0x00, 0x00, 0x0A
	.byte 0x03, 0x70, 0x85, 0xE0, 0x08, 0x70, 0x47, 0xE0, 0x03, 0x00, 0x57, 0xE1, 0x03, 0x70, 0x47, 0xA0
	.byte 0xF7, 0x70, 0x91, 0xE1, 0x14, 0x80, 0x92, 0xE5, 0x47, 0x62, 0x86, 0xE0, 0xF0, 0x80, 0xD0, 0xE1
	.byte 0x06, 0x80, 0x88, 0xE0, 0xB0, 0x80, 0xC0, 0xE1, 0xB5, 0x80, 0x81, 0xE1, 0x02, 0x00, 0x80, 0xE2
	.byte 0x02, 0x50, 0x85, 0xE2, 0x04, 0x00, 0x50, 0xE1, 0xD7, 0xFF, 0xFF, 0xBA, 0x0C, 0x00, 0x8F, 0xE2
	.byte 0x00, 0x50, 0x80, 0xE5, 0xF0, 0x01, 0xBD, 0xE8, 0x1E, 0xFF, 0x2F, 0xE1, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x42, 0x41, 0x52, 0x54
gStaticData_0803A818:
	.byte 0xF1, 0x0F, 0x2D, 0xE9, 0x18, 0x70, 0x90, 0xE5
	.byte 0x1C, 0x80, 0x90, 0xE5, 0x04, 0x10, 0x90, 0xE5, 0x08, 0x20, 0x90, 0xE5, 0x0C, 0x30, 0x90, 0xE5
	.byte 0x10, 0x40, 0x90, 0xE5, 0x84, 0x40, 0x81, 0xE0, 0x14, 0x50, 0x90, 0xE5, 0x85, 0x10, 0x81, 0xE0
	.byte 0x20, 0xA0, 0x90, 0xE5, 0x24, 0xB0, 0x90, 0xE5, 0x00, 0x00, 0x90, 0xE5, 0x0A, 0xA1, 0x9F, 0xE7
	.byte 0x0A, 0xF0, 0x8F, 0xE0, 0x20, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xC4, 0x00, 0x00, 0x00
	.byte 0xC2, 0x65, 0xA0, 0xE1, 0xD6, 0x60, 0x90, 0xE1, 0x97, 0x06, 0x06, 0xE0, 0x46, 0x64, 0xA0, 0xE1
	.byte 0xB2, 0x60, 0xC1, 0xE0
gStaticData_0803A874:
	.byte 0x08, 0x20, 0x82, 0xE0, 0x04, 0x00, 0x51, 0xE1, 0x14, 0x00, 0x00, 0xAA, 0x03, 0x00, 0x52, 0xE1
gStaticData_0803A884:
	.byte 0xF5, 0xFF, 0xFF, 0xBA, 0x00, 0x00, 0x5B, 0xE3, 0x10, 0x00, 0x00, 0x0A, 0x0B, 0x20, 0x42, 0xE0
	.byte 0xF1, 0xFF, 0xFF, 0xEA, 0xC2, 0x65, 0xA0, 0xE1, 0xD6, 0x60, 0x90, 0xE1, 0x97, 0x06, 0x06, 0xE0
	.byte 0x46, 0x64, 0xA0, 0xE1, 0xF0, 0x90, 0xD1, 0xE1, 0x09, 0x60, 0x86, 0xE0, 0xB2, 0x60, 0xC1, 0xE0
gStaticData_0803A8B4:
	.byte 0x08, 0x20, 0x82, 0xE0, 0x04, 0x00, 0x51, 0xE1, 0x04, 0x00, 0x00, 0xAA, 0x03, 0x00, 0x52, 0xE1
gStaticData_0803A8C4:
	.byte 0xF3, 0xFF, 0xFF, 0xBA, 0x00, 0x00, 0x5B, 0xE3, 0x0B, 0x20, 0x42, 0x10, 0xF0, 0xFF, 0xFF, 0x1A
	.byte 0xF1, 0x0F, 0xBD, 0xE8, 0x04, 0x30, 0x90, 0xE5, 0x03, 0x30, 0x41, 0xE0, 0xA3, 0x30, 0xA0, 0xE1
	.byte 0x08, 0x20, 0x80, 0xE5, 0x14, 0x30, 0x80, 0xE5, 0x0E, 0x00, 0xA0, 0xE1
	.byte 0x10, 0xFF, 0x2F, 0xE1, 0xC2, 0x65, 0xA0, 0xE1, 0xD6, 0x60, 0x90, 0xE1, 0x97, 0x06, 0x06, 0xE0
	.byte 0x46, 0x64, 0xA0, 0xE1, 0x06, 0x68, 0xA0, 0xE1, 0x26, 0x68, 0x86, 0xE1, 0x00, 0x90, 0x91, 0xE5
	.byte 0x09, 0x90, 0x86, 0xE0, 0x04, 0x90, 0x81, 0xE4, 0x88, 0x20, 0x82, 0xE0, 0x03, 0x00, 0x52, 0xE1
	.byte 0x04, 0x00, 0x51, 0xB1, 0xF2, 0xFF, 0xFF, 0xBA, 0x03, 0x00, 0x52, 0xE1, 0x01, 0x00, 0x00, 0xCA
	.byte 0x04, 0x00, 0x51, 0xE1, 0xE6, 0xFF, 0xFF, 0xDA, 0x02, 0x10, 0x41, 0xE2, 0x08, 0x20, 0x42, 0xE0
	.byte 0xE3, 0xFF, 0xFF, 0xEA

	thumb_func_start sub_803A944
sub_803A944: @ 0x0803A944
	svc #0xe
	bx lr

	thumb_func_start sub_803A948
sub_803A948: @ 0x0803A948
	svc #0xc
	bx lr

	thumb_func_start sub_803A94C
sub_803A94C: @ 0x0803A94C
	svc #0xb
	bx lr

	thumb_func_start LZ77UnCompWrapper
LZ77UnCompWrapper: @ 0x0803A950
	svc #0x12
	bx lr

	thumb_func_start sub_803A954
sub_803A954: @ 0x0803A954
	svc #0xf
	bx lr

	thumb_func_start RLUnCompWrapper
RLUnCompWrapper: @ 0x0803A958
	svc #0x15
	bx lr

	thumb_func_start sub_803A95C
sub_803A95C: @ 0x0803A95C
	svc #8
	bx lr

	thumb_func_start sub_0803A960
sub_0803A960: @ 0x0803A960
	movs r2, #0
	svc #5
	bx lr
	.align 2, 0

	thumb_func_start sub_803A968
sub_803A968: @ 0x0803A968
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r2, #0
	cmp r0, #4
	bne _0803A984
	ldr r1, _0803A97C @ =gUnknown_03001634
	ldr r0, _0803A980 @ =gStaticData_085A9EF8
	str r0, [r1]
	b _0803A9A0
	.align 2, 0
_0803A97C: .4byte gUnknown_03001634
_0803A980: .4byte gStaticData_085A9EF8
_0803A984:
	cmp r0, #0x40
	bne _0803A998
	ldr r1, _0803A990 @ =gUnknown_03001634
	ldr r0, _0803A994 @ =gStaticData_085A9F04
	str r0, [r1]
	b _0803A9A0
	.align 2, 0
_0803A990: .4byte gUnknown_03001634
_0803A994: .4byte gStaticData_085A9F04
_0803A998:
	ldr r1, _0803A9A4 @ =gUnknown_03001634
	ldr r0, _0803A9A8 @ =gStaticData_085A9EF8
	str r0, [r1]
	movs r2, #1
_0803A9A0:
	adds r0, r2, #0
	bx lr
	.align 2, 0
_0803A9A4: .4byte gUnknown_03001634
_0803A9A8: .4byte gStaticData_085A9EF8
_0803A9AC:
	.byte 0x06
gStaticData_0803A9AD:
	.byte 0x49, 0x08, 0x88
	.byte 0x00, 0x28, 0x08, 0xD0, 0x08, 0x88, 0x01, 0x38, 0x08, 0x80, 0x00, 0x04, 0x00, 0x28, 0x02, 0xD1
	.byte 0x02, 0x49, 0x01, 0x20, 0x08, 0x70, 0x70, 0x47, 0x22, 0x16, 0x00, 0x03, 0x24, 0x16, 0x00, 0x03

	thumb_func_start sub_803A9D0
sub_803A9D0: @ 0x0803A9D0
	adds r2, r1, #0
	lsls r0, r0, #0x18
	lsrs r1, r0, #0x18
	cmp r1, #3
	bhi _0803AA04
	ldr r0, _0803A9F4 @ =gUnknown_03001620
	strb r1, [r0]
	ldr r1, _0803A9F8 @ =gUnknown_03001628
	ldrb r0, [r0]
	lsls r0, r0, #2
	ldr r3, _0803A9FC @ =0x04000100
	adds r0, r0, r3
	str r0, [r1]
	ldr r0, _0803AA00 @ =gStaticData_0803A9AD
	str r0, [r2]
	movs r0, #0
	b _0803AA06
	.align 2, 0
_0803A9F4: .4byte gUnknown_03001620
_0803A9F8: .4byte gUnknown_03001628
_0803A9FC: .4byte 0x04000100
_0803AA00: .4byte gStaticData_0803A9AD
_0803AA04:
	movs r0, #1
_0803AA06:
	bx lr

	thumb_func_start sub_803AA08
sub_803AA08: @ 0x0803AA08
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	ldr r2, _0803AA74 @ =gUnknown_0300162C
	ldr r1, _0803AA78 @ =0x04000208
	mov sb, r1
	ldrh r1, [r1]
	strh r1, [r2]
	movs r6, #0
	mov r2, sb
	strh r6, [r2]
	ldr r3, _0803AA7C @ =gUnknown_03001628
	mov r8, r3
	ldr r5, [r3]
	strh r6, [r5, #2]
	ldr r3, _0803AA80 @ =0x04000202
	ldr r4, _0803AA84 @ =gUnknown_03001620
	ldrb r1, [r4]
	movs r2, #8
	adds r7, r2, #0
	lsls r7, r1
	adds r1, r7, #0
	strh r1, [r3]
	subs r3, #2
	ldrb r1, [r4]
	lsls r2, r1
	ldrh r1, [r3]
	orrs r1, r2
	strh r1, [r3]
	ldr r1, _0803AA88 @ =gUnknown_03001624
	strb r6, [r1]
	ldr r2, _0803AA8C @ =gUnknown_03001622
	ldrh r1, [r0]
	strh r1, [r2]
	adds r0, #2
	ldrh r1, [r0]
	strh r1, [r5]
	adds r1, r5, #2
	mov r2, r8
	str r1, [r2]
	ldrh r0, [r0, #2]
	strh r0, [r5, #2]
	str r5, [r2]
	movs r0, #1
	mov r3, sb
	strh r0, [r3]
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0803AA74: .4byte gUnknown_0300162C
_0803AA78: .4byte 0x04000208
_0803AA7C: .4byte gUnknown_03001628
_0803AA80: .4byte 0x04000202
_0803AA84: .4byte gUnknown_03001620
_0803AA88: .4byte gUnknown_03001624
_0803AA8C: .4byte gUnknown_03001622

	thumb_func_start sub_803AA90
sub_803AA90: @ 0x0803AA90
	ldr r3, _0803AAC0 @ =0x04000208
	movs r1, #0
	strh r1, [r3]
	ldr r2, _0803AAC4 @ =gUnknown_03001628
	ldr r0, [r2]
	strh r1, [r0]
	adds r0, #2
	str r0, [r2]
	strh r1, [r0]
	subs r0, #2
	str r0, [r2]
	ldr r2, _0803AAC8 @ =0x04000200
	ldr r0, _0803AACC @ =gUnknown_03001620
	ldrb r0, [r0]
	movs r1, #8
	lsls r1, r0
	ldrh r0, [r2]
	bics r0, r1
	strh r0, [r2]
	ldr r0, _0803AAD0 @ =gUnknown_0300162C
	ldrh r0, [r0]
	strh r0, [r3]
	bx lr
	.align 2, 0
_0803AAC0: .4byte 0x04000208
_0803AAC4: .4byte gUnknown_03001628
_0803AAC8: .4byte 0x04000200
_0803AACC: .4byte gUnknown_03001620
_0803AAD0: .4byte gUnknown_0300162C

	thumb_func_start sub_803AAD4
sub_803AAD4: @ 0x0803AAD4
	push {r4, r5, r6, lr}
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	ldr r4, _0803AB34 @ =0x04000208
	ldrh r3, [r4]
	adds r6, r3, #0
	movs r3, #0
	strh r3, [r4]
	ldr r5, _0803AB38 @ =0x04000204
	ldrh r4, [r5]
	ldr r3, _0803AB3C @ =0x0000F8FF
	ands r4, r3
	ldr r3, _0803AB40 @ =gUnknown_03001634
	ldr r3, [r3]
	ldrh r3, [r3, #6]
	orrs r4, r3
	strh r4, [r5]
	ldr r3, _0803AB44 @ =0x040000D4
	str r0, [r3]
	ldr r0, _0803AB48 @ =0x040000D8
	str r1, [r0]
	ldr r1, _0803AB4C @ =0x040000DC
	movs r0, #0x80
	lsls r0, r0, #0x18
	orrs r2, r0
	str r2, [r1]
	adds r1, #2
	movs r2, #0x80
	lsls r2, r2, #8
	adds r0, r2, #0
	ldrh r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _0803AB28
	ldr r2, _0803AB50 @ =0x040000DE
	movs r0, #0x80
	lsls r0, r0, #8
	adds r1, r0, #0
_0803AB20:
	ldrh r0, [r2]
	ands r0, r1
	cmp r0, #0
	bne _0803AB20
_0803AB28:
	ldr r0, _0803AB34 @ =0x04000208
	strh r6, [r0]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0803AB34: .4byte 0x04000208
_0803AB38: .4byte 0x04000204
_0803AB3C: .4byte 0x0000F8FF
_0803AB40: .4byte gUnknown_03001634
_0803AB44: .4byte 0x040000D4
_0803AB48: .4byte 0x040000D8
_0803AB4C: .4byte 0x040000DC
_0803AB50: .4byte 0x040000DE

	thumb_func_start sub_803AB54
sub_803AB54: @ 0x0803AB54
	push {r4, r5, r6, lr}
	sub sp, #0x88
	adds r5, r1, #0
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	ldr r0, _0803AB6C @ =gUnknown_03001634
	ldr r0, [r0]
	ldrh r0, [r0, #4]
	cmp r3, r0
	blo _0803AB74
	ldr r0, _0803AB70 @ =0x000080FF
	b _0803ABF6
	.align 2, 0
_0803AB6C: .4byte gUnknown_03001634
_0803AB70: .4byte 0x000080FF
_0803AB74:
	ldr r0, _0803AC00 @ =gUnknown_03001634
	adds r6, r0, #0
	ldr r0, [r0]
	ldrb r1, [r0, #8]
	lsls r0, r1, #1
	mov r4, sp
	adds r2, r0, r4
	adds r2, #2
	movs r4, #0
	cmp r4, r1
	bhs _0803AB9E
_0803AB8A:
	strh r3, [r2]
	subs r2, #2
	lsrs r3, r3, #1
	adds r0, r4, #1
	lsls r0, r0, #0x18
	lsrs r4, r0, #0x18
	ldr r0, [r6]
	ldrb r0, [r0, #8]
	cmp r4, r0
	blo _0803AB8A
_0803AB9E:
	movs r0, #1
	strh r0, [r2]
	subs r2, #2
	strh r0, [r2]
	movs r4, #0xd0
	lsls r4, r4, #0x14
	ldr r0, _0803AC00 @ =gUnknown_03001634
	ldr r0, [r0]
	ldrb r2, [r0, #8]
	adds r2, #3
	mov r0, sp
	adds r1, r4, #0
	bl sub_803AAD4
	adds r0, r4, #0
	mov r1, sp
	movs r2, #0x44
	bl sub_803AAD4
	add r2, sp, #8
	adds r5, #6
	movs r4, #0
	movs r6, #1
_0803ABCC:
	movs r1, #0
	movs r3, #0
_0803ABD0:
	lsls r1, r1, #0x11
	ldrh r0, [r2]
	ands r0, r6
	lsrs r1, r1, #0x10
	orrs r1, r0
	adds r2, #2
	adds r0, r3, #1
	lsls r0, r0, #0x18
	lsrs r3, r0, #0x18
	cmp r3, #0xf
	bls _0803ABD0
	strh r1, [r5]
	subs r5, #2
	adds r0, r4, #1
	lsls r0, r0, #0x18
	lsrs r4, r0, #0x18
	cmp r4, #3
	bls _0803ABCC
	movs r0, #0
_0803ABF6:
	add sp, #0x88
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0803AC00: .4byte gUnknown_03001634

	thumb_func_start sub_803AC04
sub_803AC04: @ 0x0803AC04
	push {r4, r5, lr}
	sub sp, #0xa4
	adds r5, r1, #0
	lsls r0, r0, #0x10
	lsrs r4, r0, #0x10
	ldr r0, _0803AC1C @ =gUnknown_03001634
	ldr r0, [r0]
	ldrh r0, [r0, #4]
	cmp r4, r0
	blo _0803AC24
	ldr r0, _0803AC20 @ =0x000080FF
	b _0803ACC8
	.align 2, 0
_0803AC1C: .4byte gUnknown_03001634
_0803AC20: .4byte 0x000080FF
_0803AC24:
	ldr r0, _0803AC64 @ =gUnknown_03001634
	ldr r0, [r0]
	ldrb r0, [r0, #8]
	lsls r0, r0, #1
	mov r1, sp
	adds r3, r0, r1
	adds r3, #0x84
	movs r0, #0
	strh r0, [r3]
	subs r3, #2
	movs r1, #0
_0803AC3A:
	ldrh r2, [r5]
	adds r5, #2
	movs r0, #0
_0803AC40:
	strh r2, [r3]
	subs r3, #2
	lsrs r2, r2, #1
	adds r0, #1
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #0xf
	bls _0803AC40
	adds r0, r1, #1
	lsls r0, r0, #0x18
	lsrs r1, r0, #0x18
	cmp r1, #3
	bls _0803AC3A
	movs r1, #0
	ldr r0, _0803AC64 @ =gUnknown_03001634
	adds r2, r0, #0
	ldr r0, [r0]
	b _0803AC76
	.align 2, 0
_0803AC64: .4byte gUnknown_03001634
_0803AC68:
	strh r4, [r3]
	subs r3, #2
	lsrs r4, r4, #1
	adds r0, r1, #1
	lsls r0, r0, #0x18
	lsrs r1, r0, #0x18
	ldr r0, [r2]
_0803AC76:
	ldrb r0, [r0, #8]
	cmp r1, r0
	blo _0803AC68
	movs r0, #0
	strh r0, [r3]
	subs r3, #2
	movs r0, #1
	strh r0, [r3]
	movs r1, #0xd0
	lsls r1, r1, #0x14
	ldr r0, _0803ACD0 @ =gUnknown_03001634
	ldr r0, [r0]
	ldrb r2, [r0, #8]
	adds r2, #0x43
	mov r0, sp
	bl sub_803AAD4
	ldr r0, _0803ACD4 @ =gStaticData_085A9F10
	bl sub_803AA08
	movs r4, #0
	movs r1, #0xd0
	lsls r1, r1, #0x14
	movs r3, #1
	ldr r2, _0803ACD8 @ =gUnknown_03001624
_0803ACA8:
	ldrh r0, [r1]
	ands r0, r3
	cmp r0, #0
	bne _0803ACC2
	ldrb r0, [r2]
	cmp r0, #0
	beq _0803ACA8
	ldrh r0, [r1]
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	bne _0803ACC2
	ldr r4, _0803ACDC @ =0x0000C001
_0803ACC2:
	bl sub_803AA90
	adds r0, r4, #0
_0803ACC8:
	add sp, #0xa4
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0803ACD0: .4byte gUnknown_03001634
_0803ACD4: .4byte gStaticData_085A9F10
_0803ACD8: .4byte gUnknown_03001624
_0803ACDC: .4byte 0x0000C001

	thumb_func_start sub_803ACE0
sub_803ACE0: @ 0x0803ACE0
	push {r4, r5, lr}
	sub sp, #8
	adds r4, r1, #0
	lsls r0, r0, #0x10
	lsrs r1, r0, #0x10
	movs r5, #0
	ldr r0, _0803ACFC @ =gUnknown_03001634
	ldr r0, [r0]
	ldrh r0, [r0, #4]
	cmp r1, r0
	blo _0803AD04
	ldr r0, _0803AD00 @ =0x000080FF
	b _0803AD2E
	.align 2, 0
_0803ACFC: .4byte gUnknown_03001634
_0803AD00: .4byte 0x000080FF
_0803AD04:
	adds r0, r1, #0
	mov r1, sp
	bl sub_803AB54
	mov r2, sp
	movs r3, #0
	b _0803AD1C
_0803AD12:
	adds r0, r3, #1
	lsls r0, r0, #0x18
	lsrs r3, r0, #0x18
	cmp r3, #3
	bhi _0803AD2C
_0803AD1C:
	ldrh r1, [r4]
	ldrh r0, [r2]
	adds r2, #2
	adds r4, #2
	cmp r1, r0
	beq _0803AD12
	movs r5, #0x80
	lsls r5, r5, #8
_0803AD2C:
	adds r0, r5, #0
_0803AD2E:
	add sp, #8
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_803AD38
sub_803AD38: @ 0x0803AD38
	push {r4, r5, r6, lr}
	adds r5, r1, #0
	lsls r0, r0, #0x10
	lsrs r4, r0, #0x10
	movs r6, #0
	b _0803AD4A
_0803AD44:
	adds r0, r6, #1
	lsls r0, r0, #0x18
	lsrs r6, r0, #0x18
_0803AD4A:
	cmp r6, #2
	bhi _0803AD6E
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_803AC04
	lsls r0, r0, #0x10
	lsrs r2, r0, #0x10
	cmp r2, #0
	bne _0803AD44
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_803ACE0
	lsls r0, r0, #0x10
	lsrs r2, r0, #0x10
	cmp r2, #0
	bne _0803AD44
_0803AD6E:
	adds r0, r2, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_803AD78
sub_803AD78: @ 0x0803AD78
	bx r0
	nop

	thumb_func_start sub_803AD7C
sub_803AD7C: @ 0x0803AD7C
	bx r1
	nop

	thumb_func_start sub_803AD80
sub_803AD80: @ 0x0803AD80
	bx r2
	nop

	thumb_func_start sub_803AD84
sub_803AD84: @ 0x0803AD84
	bx r3
	nop

	thumb_func_start sub_803AD88
sub_803AD88: @ 0x0803AD88
	bx r4
	nop

	thumb_func_start sub_803AD8C
sub_803AD8C: @ 0x0803AD8C
	bx r5
	nop

	thumb_func_start sub_803AD90
sub_803AD90: @ 0x0803AD90
	bx r6
	nop
	thumb_func_start sub_803AD94
sub_803AD94: @ 0x0803AD94
	bx	r7
	nop			@ (mov r8, r8)
	bx	r8
	nop			@ (mov r8, r8)
	bx	r9
	nop			@ (mov r8, r8)
	bx	sl
	nop			@ (mov r8, r8)
	bx	fp
	nop			@ (mov r8, r8)
	bx	ip
	nop			@ (mov r8, r8)
	bx	sp
	nop			@ (mov r8, r8)

	thumb_func_start nullsub_43
nullsub_43: @ 0x0803ADB0
	bx lr
	nop

	thumb_func_start sub_803ADB4
sub_803ADB4: @ 0x0803ADB4
	cmp r1, #0
	beq _0803AE3C
	push {r4}
	adds r4, r0, #0
	eors r4, r1
	mov ip, r4
	movs r3, #1
	movs r2, #0
	cmp r1, #0
	bpl _0803ADCA
	rsbs r1, r1, #0
_0803ADCA:
	cmp r0, #0
	bpl _0803ADD0
	rsbs r0, r0, #0
_0803ADD0:
	cmp r0, r1
	blo _0803AE2E
	movs r4, #1
	lsls r4, r4, #0x1c
_0803ADD8:
	cmp r1, r4
	bhs _0803ADE6
	cmp r1, r0
	bhs _0803ADE6
	lsls r1, r1, #4
	lsls r3, r3, #4
	b _0803ADD8
_0803ADE6:
	lsls r4, r4, #3
_0803ADE8:
	cmp r1, r4
	bhs _0803ADF6
	cmp r1, r0
	bhs _0803ADF6
	lsls r1, r1, #1
	lsls r3, r3, #1
	b _0803ADE8
_0803ADF6:
	cmp r0, r1
	blo _0803ADFE
	subs r0, r0, r1
	orrs r2, r3
_0803ADFE:
	lsrs r4, r1, #1
	cmp r0, r4
	blo _0803AE0A
	subs r0, r0, r4
	lsrs r4, r3, #1
	orrs r2, r4
_0803AE0A:
	lsrs r4, r1, #2
	cmp r0, r4
	blo _0803AE16
	subs r0, r0, r4
	lsrs r4, r3, #2
	orrs r2, r4
_0803AE16:
	lsrs r4, r1, #3
	cmp r0, r4
	blo _0803AE22
	subs r0, r0, r4
	lsrs r4, r3, #3
	orrs r2, r4
_0803AE22:
	cmp r0, #0
	beq _0803AE2E
	lsrs r3, r3, #4
	beq _0803AE2E
	lsrs r1, r1, #4
	b _0803ADF6
_0803AE2E:
	adds r0, r2, #0
	mov r4, ip
	cmp r4, #0
	bpl _0803AE38
	rsbs r0, r0, #0
_0803AE38:
	pop {r4}
	mov pc, lr
_0803AE3C:
	push {lr}
	bl nullsub_8
	movs r0, #0
	pop {pc}
	.align 2, 0

	thumb_func_start nullsub_8
nullsub_8: @ 0x0803AE48
	mov pc, lr
	.align 2, 0

	thumb_func_start sub_803AE4C
sub_803AE4C: @ 0x0803AE4C
	movs r3, #1
	cmp r1, #0
	beq _0803AF10
	bpl _0803AE56
	rsbs r1, r1, #0
_0803AE56:
	push {r4}
	push {r0}
	cmp r0, #0
	bpl _0803AE60
	rsbs r0, r0, #0
_0803AE60:
	cmp r0, r1
	blo _0803AF04
	movs r4, #1
	lsls r4, r4, #0x1c
_0803AE68:
	cmp r1, r4
	bhs _0803AE76
	cmp r1, r0
	bhs _0803AE76
	lsls r1, r1, #4
	lsls r3, r3, #4
	b _0803AE68
_0803AE76:
	lsls r4, r4, #3
_0803AE78:
	cmp r1, r4
	bhs _0803AE86
	cmp r1, r0
	bhs _0803AE86
	lsls r1, r1, #1
	lsls r3, r3, #1
	b _0803AE78
_0803AE86:
	movs r2, #0
	cmp r0, r1
	blo _0803AE8E
	subs r0, r0, r1
_0803AE8E:
	lsrs r4, r1, #1
	cmp r0, r4
	blo _0803AEA0
	subs r0, r0, r4
	mov ip, r3
	movs r4, #1
	rors r3, r4
	orrs r2, r3
	mov r3, ip
_0803AEA0:
	lsrs r4, r1, #2
	cmp r0, r4
	blo _0803AEB2
	subs r0, r0, r4
	mov ip, r3
	movs r4, #2
	rors r3, r4
	orrs r2, r3
	mov r3, ip
_0803AEB2:
	lsrs r4, r1, #3
	cmp r0, r4
	blo _0803AEC4
	subs r0, r0, r4
	mov ip, r3
	movs r4, #3
	rors r3, r4
	orrs r2, r3
	mov r3, ip
_0803AEC4:
	mov ip, r3
	cmp r0, #0
	beq _0803AED2
	lsrs r3, r3, #4
	beq _0803AED2
	lsrs r1, r1, #4
	b _0803AE86
_0803AED2:
	movs r4, #0xe
	lsls r4, r4, #0x1c
	ands r2, r4
	beq _0803AF04
	mov r3, ip
	movs r4, #3
	rors r3, r4
	tst r2, r3
	beq _0803AEE8
	lsrs r4, r1, #3
	adds r0, r0, r4
_0803AEE8:
	mov r3, ip
	movs r4, #2
	rors r3, r4
	tst r2, r3
	beq _0803AEF6
	lsrs r4, r1, #2
	adds r0, r0, r4
_0803AEF6:
	mov r3, ip
	movs r4, #1
	rors r3, r4
	tst r2, r3
	beq _0803AF04
	lsrs r4, r1, #1
	adds r0, r0, r4
_0803AF04:
	pop {r4}
	cmp r4, #0
	bpl _0803AF0C
	rsbs r0, r0, #0
_0803AF0C:
	pop {r4}
	mov pc, lr
_0803AF10:
	push {lr}
	bl nullsub_8
	movs r0, #0
	pop {pc}
	.align 2, 0

	thumb_func_start sub_803AF1C
sub_803AF1C: @ 0x0803AF1C
	cmp r1, #0
	beq _0803AFD2
	movs r3, #1
	cmp r0, r1
	bhs _0803AF28
	mov pc, lr
_0803AF28:
	push {r4}
	movs r4, #1
	lsls r4, r4, #0x1c
_0803AF2E:
	cmp r1, r4
	bhs _0803AF3C
	cmp r1, r0
	bhs _0803AF3C
	lsls r1, r1, #4
	lsls r3, r3, #4
	b _0803AF2E
_0803AF3C:
	lsls r4, r4, #3
_0803AF3E:
	cmp r1, r4
	bhs _0803AF4C
	cmp r1, r0
	bhs _0803AF4C
	lsls r1, r1, #1
	lsls r3, r3, #1
	b _0803AF3E
_0803AF4C:
	movs r2, #0
	cmp r0, r1
	blo _0803AF54
	subs r0, r0, r1
_0803AF54:
	lsrs r4, r1, #1
	cmp r0, r4
	blo _0803AF66
	subs r0, r0, r4
	mov ip, r3
	movs r4, #1
	rors r3, r4
	orrs r2, r3
	mov r3, ip
_0803AF66:
	lsrs r4, r1, #2
	cmp r0, r4
	blo _0803AF78
	subs r0, r0, r4
	mov ip, r3
	movs r4, #2
	rors r3, r4
	orrs r2, r3
	mov r3, ip
_0803AF78:
	lsrs r4, r1, #3
	cmp r0, r4
	blo _0803AF8A
	subs r0, r0, r4
	mov ip, r3
	movs r4, #3
	rors r3, r4
	orrs r2, r3
	mov r3, ip
_0803AF8A:
	mov ip, r3
	cmp r0, #0
	beq _0803AF98
	lsrs r3, r3, #4
	beq _0803AF98
	lsrs r1, r1, #4
	b _0803AF4C
_0803AF98:
	movs r4, #0xe
	lsls r4, r4, #0x1c
	ands r2, r4
	bne _0803AFA4
	pop {r4}
	mov pc, lr
_0803AFA4:
	mov r3, ip
	movs r4, #3
	rors r3, r4
	tst r2, r3
	beq _0803AFB2
	lsrs r4, r1, #3
	adds r0, r0, r4
_0803AFB2:
	mov r3, ip
	movs r4, #2
	rors r3, r4
	tst r2, r3
	beq _0803AFC0
	lsrs r4, r1, #2
	adds r0, r0, r4
_0803AFC0:
	mov r3, ip
	movs r4, #1
	rors r3, r4
	tst r2, r3
	beq _0803AFCE
	lsrs r4, r1, #1
	adds r0, r0, r4
_0803AFCE:
	pop {r4}
	mov pc, lr
_0803AFD2:
	push {lr}
	bl nullsub_8
	movs r0, #0
	pop {pc}

	thumb_func_start sub_803AFDC
sub_803AFDC: @ 0x0803AFDC
	str r1, [r0, #8]
	str r2, [r0, #0xc]
	bx lr
	.align 2, 0

	thumb_func_start sub_803AFE4
sub_803AFE4: @ 0x0803AFE4
	str r1, [r0]
	str r2, [r0, #4]
	bx lr
	.align 2, 0

	thumb_func_start sub_803AFEC
sub_803AFEC: @ 0x0803AFEC
	ldr r0, [r0, #0x74]
	bx lr
	thumb_func_start sub_803AFF0
sub_803AFF0: @ 0x0803AFF0
	push	{lr}
	adds	r3, r0, #0
	movs	r0, #152	@ 0x98
	lsls	r0, r0, #1
	adds	r2, r3, r0
	ldr r0, _0803B01C
	str	r0, [r2, #0]
	movs	r0, #152	@ 0x98
	lsls	r0, r0, #1
	adds	r2, r3, r0
	ldr r0, _0803B020
	str	r0, [r2, #0]
	movs	r0, #1
	ands	r0, r1
	cmp	r0, #0
	beq _0803B016
	adds	r0, r3, #0
	bl sub_8026ED0
_0803B016:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B01C: .4byte gStaticData_087E4D1C
_0803B020: .4byte gStaticData_087E4DAC

	thumb_func_start sub_803B024
sub_803B024: @ 0x0803B024
	push	{lr}
	adds	r3, r0, #0
	movs	r0, #152	@ 0x98
	lsls	r0, r0, #1
	adds	r2, r3, r0
	ldr r0, _0803B050
	str	r0, [r2, #0]
	movs	r0, #152	@ 0x98
	lsls	r0, r0, #1
	adds	r2, r3, r0
	ldr r0, _0803B054
	str	r0, [r2, #0]
	movs	r0, #1
	ands	r0, r1
	cmp	r0, #0
	beq _0803B04A
	adds	r0, r3, #0
	bl sub_8026ED0
_0803B04A:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B050: .4byte gStaticData_087E4D64
_0803B054: .4byte gStaticData_087E4DAC

