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

