.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_801967C
sub_801967C: @ 0x0801967C
	push {r4, r5, r6, lr}
	lsls r1, r1, #0x18
	lsrs r3, r1, #0x18
	ldr r1, _080196A4 @ =gUnknown_030012EC
	ldr r0, [r1]
	ldr r4, [r0, #4]
	movs r2, #0
	cmp r2, r4
	bge _080196B0
	adds r6, r1, #0
	movs r5, #1
_08019692:
	ldr r0, [r6]
	ldr r1, [r0, #0xc]
	lsls r0, r2, #2
	adds r0, r0, r1
	ldr r0, [r0]
	cmp r3, #0
	beq _080196A8
	strb r5, [r0, #0xa]
	b _080196AA
	.align 2, 0
_080196A4: .4byte gUnknown_030012EC
_080196A8:
	strb r3, [r0, #0xa]
_080196AA:
	adds r2, #1
	cmp r2, r4
	blt _08019692
_080196B0:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80196B8
sub_80196B8: @ 0x080196B8
	push {r4, lr}
	str r2, [r0, #0x14]
	str r3, [r0, #0x18]
	ldr r4, [r1]
	subs r2, r2, r4
	str r2, [r0, #0x1c]
	ldr r1, [r1, #4]
	subs r3, r3, r1
	str r3, [r0, #0x20]
	ldr r1, [r0, #0x3c]
	ldr r1, [r1, #0x10]
	ldr r2, _080196E0 @ =gStaticData_0816C358
	adds r1, r1, r2
	ldrb r1, [r1]
	str r1, [r0, #0x28]
	str r1, [r0, #0x24]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080196E0: .4byte gStaticData_0816C358

	thumb_func_start sub_80196E4
sub_80196E4: @ 0x080196E4
	push {lr}
	ldr r2, _080196F4 @ =gStaticData_087E4704
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_080196F4: .4byte gStaticData_087E4704

	thumb_func_start sub_80196F8
sub_80196F8: @ 0x080196F8
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	bl sub_800B8C8
	ldr r0, _08019714 @ =gStaticData_087E4704
	str r0, [r4, #0xc]
	movs r0, #0
	str r0, [r4, #0x24]
	str r5, [r4, #0x3c]
	adds r0, r4, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08019714: .4byte gStaticData_087E4704

	thumb_func_start sub_8019718
sub_8019718: @ 0x08019718
	push {r4, lr}
	adds r1, r2, #0
	ldr r3, [r0, #0xc]
	movs r4, #0x20
	ldrsh r2, [r3, r4]
	adds r0, r0, r2
	ldr r2, [r3, #0x24]
	bl sub_803AD80
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8019730
sub_8019730: @ 0x08019730
	ldr r0, [r0, #8]
	cmp r0, #0
	bne _08019740
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xc]
	ands r0, r2
	strb r0, [r1, #0xc]
_08019740:
	bx lr
	.align 2, 0

	thumb_func_start sub_8019744
sub_8019744: @ 0x08019744
	push {lr}
	ldr r2, _08019754 @ =gStaticData_087E476C
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_08019754: .4byte gStaticData_087E476C

	thumb_func_start sub_8019758
sub_8019758: @ 0x08019758
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800B8C8
	ldr r0, _0801976C @ =gStaticData_087E476C
	str r0, [r4, #0xc]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801976C: .4byte gStaticData_087E476C

	thumb_func_start sub_8019770
sub_8019770: @ 0x08019770
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r2, #0
	cmp r5, #3
	bne _080197A8
	ldr r0, [r4, #0x20]
	ldr r0, [r0, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #9
	bl sub_803AD80
	ldr r0, _080197C0 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231C4
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _080197A8
	ldr r0, _080197C4 @ =0x0000FFFF
	movs r1, #0x8c
	movs r2, #0x98
	movs r3, #0
	bl sub_8021D80
_080197A8:
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	adds r1, r5, #0
	bl sub_803AD80
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080197C0: .4byte gUnknown_030012C0
_080197C4: .4byte 0x0000FFFF

	thumb_func_start sub_80197C8
sub_80197C8: @ 0x080197C8
	push {lr}
	ldr r2, _080197D8 @ =gStaticData_087E47D4
	str r2, [r0, #0xc]
	bl sub_8017A78
	pop {r0}
	bx r0
	.align 2, 0
_080197D8: .4byte gStaticData_087E47D4

	thumb_func_start sub_80197DC
sub_80197DC: @ 0x080197DC
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8017A8C
	ldr r0, _080197F0 @ =gStaticData_087E47D4
	str r0, [r4, #0xc]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_080197F0: .4byte gStaticData_087E47D4

	thumb_func_start sub_80197F4
sub_80197F4: @ 0x080197F4
	ldr r0, [r0, #0x10]
	bx lr

	thumb_func_start sub_80197F8
sub_80197F8: @ 0x080197F8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x24
	adds r5, r0, #0
	adds r4, r1, #0
	adds r0, r4, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _0801981E
	ldr r0, [r4]
	ldr r2, [r4, #4]
	ldr r1, [r5, #0x2c]
	movs r3, #0xc0
	lsls r3, r3, #3
	adds r0, r0, r3
	b _08019828
_0801981E:
	ldr r0, [r4]
	ldr r2, [r4, #4]
	ldr r1, [r5, #0x2c]
	ldr r6, _08019884 @ =0xFFFFFA00
	adds r0, r0, r6
_08019828:
	str r0, [r1]
	str r2, [r1, #4]
	add r0, sp, #4
	adds r1, r4, #0
	bl sub_8007CF8
	ldr r0, [r5, #8]
	cmp r0, #8
	bne _08019870
	ldr r0, _08019888 @ =gUnknown_030012D8
	ldr r1, [r0]
	ldrb r7, [r1, #0xa]
	cmp r7, #0x13
	bne _08019870
	add r6, sp, #0x14
	adds r0, r6, #0
	bl sub_8007C30
	ldr r0, [sp, #0x1c]
	cmp r0, #0
	beq _08019870
	adds r0, r6, #0
	add r1, sp, #4
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08019870
	ldr r0, [r5, #0x10]
	adds r0, #1
	str r0, [r5, #0x10]
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0xb
	bl sub_8019CE4
_08019870:
	ldr r3, [r5, #8]
	cmp r3, #0x11
	bls _08019878
	b _08019CD0
_08019878:
	lsls r0, r3, #2
	ldr r1, _0801988C @ =_08019890
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08019884: .4byte 0xFFFFFA00
_08019888: .4byte gUnknown_030012D8
_0801988C: .4byte _08019890
_08019890: @ jump table
	.4byte _080198D8 @ case 0
	.4byte _080198FA @ case 1
	.4byte _08019BA0 @ case 2
	.4byte _080199E4 @ case 3
	.4byte _08019AEC @ case 4
	.4byte _08019AFA @ case 5
	.4byte _08019AFA @ case 6
	.4byte _08019BB8 @ case 7
	.4byte _08019BE2 @ case 8
	.4byte _08019BF8 @ case 9
	.4byte _08019BF8 @ case 10
	.4byte _08019C1E @ case 11
	.4byte _08019C50 @ case 12
	.4byte _080199E4 @ case 13
	.4byte _080198FA @ case 14
	.4byte _08019AAE @ case 15
	.4byte _08019C94 @ case 16
	.4byte _08019CD0 @ case 17
_080198D8:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #1
	bl sub_8019CE4
	movs r0, #0
	str r0, [r5, #0x1c]
	str r0, [r5, #0x28]
	strb r0, [r4, #0xa]
	subs r0, #5
	ldrb r1, [r4, #0xc]
	ands r0, r1
	movs r1, #0x41
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r4, #0xc]
	b _08019CD0
_080198FA:
	adds r6, r4, #0
	adds r6, #0x28
	ldr r2, [r4]
	cmp r3, #1
	bne _08019980
	ldrb r7, [r6]
	lsls r0, r7, #0x1b
	cmp r0, #0
	bge _08019938
	ldr r1, _0801992C @ =gStaticData_0816C368
	ldr r0, [r5, #0x10]
	cmp r0, #0
	ble _08019916
	ldr r1, _08019930 @ =gStaticData_0816C378
_08019916:
	ldr r0, [r5, #0x1c]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	cmp r2, r0
	bgt _08019980
	ldr r0, _08019934 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0]
	subs r0, r0, r2
	b _08019956
	.align 2, 0
_0801992C: .4byte gStaticData_0816C368
_08019930: .4byte gStaticData_0816C378
_08019934: .4byte gUnknown_030012D8
_08019938:
	ldr r1, _08019970 @ =gStaticData_0816C390
	ldr r0, [r5, #0x10]
	cmp r0, #0
	ble _08019942
	ldr r1, _08019974 @ =gStaticData_0816C3A0
_08019942:
	ldr r0, [r5, #0x1c]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	cmp r2, r0
	blt _08019980
	ldr r0, _08019978 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0]
	subs r0, r2, r0
_08019956:
	ldr r1, _0801997C @ =0x00001FFF
	cmp r0, r1
	bgt _08019966
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #5
	bl sub_8019CE4
_08019966:
	ldr r0, [r5, #0x1c]
	adds r0, #1
	str r0, [r5, #0x1c]
	b _08019CD0
	.align 2, 0
_08019970: .4byte gStaticData_0816C390
_08019974: .4byte gStaticData_0816C3A0
_08019978: .4byte gUnknown_030012D8
_0801997C: .4byte 0x00001FFF
_08019980:
	ldrb r6, [r6]
	lsls r0, r6, #0x1b
	cmp r0, #0
	bge _08019994
	movs r1, #0
	movs r0, #0x80
	lsls r0, r0, #6
	cmp r2, r0
	bgt _080199AA
	b _080199A8
_08019994:
	ldr r0, _080199DC @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x10]
	lsls r0, r0, #8
	movs r1, #0
	ldr r6, _080199E0 @ =0xFFFFE000
	adds r0, r0, r6
	cmp r2, r0
	blt _080199AA
_080199A8:
	movs r1, #1
_080199AA:
	adds r0, r1, #0
	cmp r0, #0
	bne _080199B2
	b _08019CD0
_080199B2:
	cmp r3, #1
	beq _080199B8
	b _08019B6A
_080199B8:
	movs r1, #1
	ldr r0, [r5, #0x10]
	cmp r0, #0
	ble _080199C2
	movs r1, #2
_080199C2:
	ldr r0, [r5, #0x28]
	adds r0, #1
	str r0, [r5, #0x28]
	cmp r0, r1
	bge _080199CE
	b _08019B6A
_080199CE:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #6
	bl sub_8019CE4
	b _08019CD0
	.align 2, 0
_080199DC: .4byte gUnknown_03001308
_080199E0: .4byte 0xFFFFE000
_080199E4:
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	bne _080199F0
	b _08019CD0
_080199F0:
	mov r8, r3
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #1
	bl sub_8019CE4
	adds r0, r4, #0
	adds r0, #0x28
	ldrb r7, [r0]
	lsls r1, r7, #0x1b
	adds r6, r0, #0
	cmp r1, #0
	bge _08019A16
	ldr r0, [r4]
	asrs r0, r0, #8
	ldr r1, [r4, #4]
	asrs r1, r1, #8
	adds r0, #6
	b _08019A20
_08019A16:
	ldr r0, [r4]
	asrs r0, r0, #8
	ldr r1, [r4, #4]
	asrs r1, r1, #8
	subs r0, #6
_08019A20:
	lsls r0, r0, #8
	str r0, [r4]
	lsls r1, r1, #8
	str r1, [r4, #4]
	movs r3, #8
	ldr r0, [r4, #0x20]
	adds r2, r4, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r7, [r2]
	lsls r0, r7, #3
	adds r2, r7, #0
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08019A46
	subs r3, r0, #1
_08019A46:
	str r3, [r4, #0x30]
	ldrb r2, [r6]
	lsls r0, r2, #0x1b
	movs r1, #0
	cmp r0, #0
	blt _08019A54
	movs r1, #1
_08019A54:
	lsls r1, r1, #4
	movs r0, #0x11
	rsbs r0, r0, #0
	ands r0, r2
	orrs r0, r1
	strb r0, [r6]
	mov r0, r8
	cmp r0, #3
	bne _08019AA2
	ldr r0, [r5, #0x28]
	cmp r0, #0
	bne _08019A8A
	movs r0, #0x73
	str r0, [r5, #0x20]
	ldr r0, [r5, #0x10]
	cmp r0, #1
	ble _08019A7A
	movs r0, #0xf
	b _08019A7C
_08019A7A:
	movs r0, #1
_08019A7C:
	str r0, [r5, #0x24]
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #2
	bl sub_8019CE4
	b _08019A9C
_08019A8A:
	movs r0, #0x64
	str r0, [r5, #0x20]
	movs r0, #1
	str r0, [r5, #0x24]
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #2
	bl sub_8019CE4
_08019A9C:
	movs r0, #0
	str r0, [r5, #0x1c]
	b _08019CD0
_08019AA2:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0xe
	bl sub_8019CE4
	b _08019CD0
_08019AAE:
	adds r0, r4, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _08019AC8
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0x2d
	movs r3, #0
	bl sub_801A03C
	b _08019C06
_08019AC8:
	ldr r0, _08019AE8 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #0x10
	movs r2, #0xa0
	lsls r2, r2, #0xe
	adds r1, r1, r2
	lsrs r1, r1, #0x10
	adds r0, r5, #0
	movs r2, #0x2d
	movs r3, #1
	bl sub_801A03C
	b _08019C06
	.align 2, 0
_08019AE8: .4byte gUnknown_03001308
_08019AEC:
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	bne _08019AF8
	b _08019CD0
_08019AF8:
	b _08019C06
_08019AFA:
	ldr r0, [r4, #0x30]
	cmp r0, #0x14
	bne _08019B50
	ldr r0, [r4, #0x34]
	cmp r0, #0
	bne _08019B50
	adds r0, r4, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _08019B32
	ldr r2, [r4]
	asrs r2, r2, #8
	adds r2, #6
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	ldr r3, [r4, #4]
	asrs r3, r3, #8
	subs r3, #0x32
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	str r4, [sp]
	adds r0, r5, #0
	movs r1, #1
	bl sub_8019EBC
	b _08019B50
_08019B32:
	ldr r2, [r4]
	asrs r2, r2, #8
	subs r2, #6
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	ldr r3, [r4, #4]
	asrs r3, r3, #8
	subs r3, #0x32
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	str r4, [sp]
	adds r0, r5, #0
	movs r1, #1
	bl sub_8019EBC
_08019B50:
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	bne _08019B5C
	b _08019CD0
_08019B5C:
	ldr r0, [r5, #8]
	cmp r0, #6
	bne _08019B76
	movs r0, #0x40
	ldrb r3, [r4, #0xc]
	orrs r0, r3
	strb r0, [r4, #0xc]
_08019B6A:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #3
	bl sub_8019CE4
	b _08019CD0
_08019B76:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #1
	bl sub_8019CE4
	movs r3, #8
	ldr r0, [r4, #0x20]
	adds r2, r4, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r5, [r2]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08019B9C
	subs r3, r0, #1
_08019B9C:
	str r3, [r4, #0x30]
	b _08019CD0
_08019BA0:
	ldr r0, [r5, #0x20]
	subs r0, #1
	str r0, [r5, #0x20]
	cmp r0, #0
	beq _08019BAC
	b _08019CD0
_08019BAC:
	ldr r2, [r5, #0x24]
	adds r0, r5, #0
	adds r1, r4, #0
	bl sub_8019CE4
	b _08019CD0
_08019BB8:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0
	bl sub_801A7AC
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r6, #0
	ldrsh r0, [r1, r6]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r4, #0
	movs r2, #5
	bl sub_803AD84
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #8
	bl sub_8019CE4
	b _08019CD0
_08019BE2:
	ldr r0, [r5, #0x20]
	subs r0, #1
	str r0, [r5, #0x20]
	cmp r0, #0
	bne _08019CD0
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #9
	bl sub_8019CE4
	b _08019CD0
_08019BF8:
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08019CD0
	cmp r3, #9
	bne _08019C12
_08019C06:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #1
	bl sub_8019CE4
	b _08019CD0
_08019C12:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0xc
	bl sub_8019CE4
	b _08019CD0
_08019C1E:
	ldr r0, [r5, #0x20]
	cmp r0, #0
	beq _08019C2A
	subs r0, #1
	str r0, [r5, #0x20]
	b _08019CD0
_08019C2A:
	ldr r0, [r5, #0x10]
	cmp r0, #2
	ble _08019C3A
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0x10
	bl sub_8019CE4
_08019C3A:
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08019CD0
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0xa
	bl sub_8019CE4
	b _08019CD0
_08019C50:
	movs r2, #0x80
	lsls r2, r2, #7
	ldr r0, [r5, #0x10]
	cmp r0, #0
	ble _08019C5E
	movs r2, #0x80
	lsls r2, r2, #6
_08019C5E:
	adds r0, r4, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _08019C72
	ldr r0, [r4]
	cmp r0, r2
	bgt _08019CD0
	b _08019C84
_08019C72:
	ldr r1, [r4]
	ldr r0, _08019C90 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x10]
	lsls r0, r0, #8
	subs r0, r0, r2
	cmp r1, r0
	blt _08019CD0
_08019C84:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0xd
	bl sub_8019CE4
	b _08019CD0
	.align 2, 0
_08019C90: .4byte gUnknown_03001308
_08019C94:
	ldr r1, [r4, #4]
	ldr r0, _08019CDC @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x14]
	lsls r0, r0, #8
	movs r7, #0x80
	lsls r7, r7, #6
	adds r0, r0, r7
	cmp r1, r0
	blt _08019CD0
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0
	bl sub_801A7AC
	ldr r0, _08019CE0 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231BC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08019CC6
	bl sub_80241A4
_08019CC6:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0x11
	bl sub_8019CE4
_08019CD0:
	add sp, #0x24
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08019CDC: .4byte gUnknown_03001308
_08019CE0: .4byte gUnknown_030012C0

	thumb_func_start sub_8019CE4
sub_8019CE4: @ 0x08019CE4
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	adds r4, r2, #0
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	adds r1, r4, #0
	bl sub_803AD80
	subs r0, r4, #1
	cmp r0, #0xf
	bls _08019D04
	b _08019EB0
_08019D04:
	lsls r0, r0, #2
	ldr r1, _08019D10 @ =_08019D14
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08019D10: .4byte _08019D14
_08019D14: @ jump table
	.4byte _08019DBC @ case 0
	.4byte _08019E1C @ case 1
	.4byte _08019DE0 @ case 2
	.4byte _08019DF2 @ case 3
	.4byte _08019E08 @ case 4
	.4byte _08019E04 @ case 5
	.4byte _08019EB0 @ case 6
	.4byte _08019E28 @ case 7
	.4byte _08019E42 @ case 8
	.4byte _08019E42 @ case 9
	.4byte _08019E6C @ case 10
	.4byte _08019D84 @ case 11
	.4byte _08019DE0 @ case 12
	.4byte _08019DBC @ case 13
	.4byte _08019EB0 @ case 14
	.4byte _08019D54 @ case 15
_08019D54:
	ldr r0, _08019D7C @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231BC
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08019D6E
	ldr r0, _08019D80 @ =0x0000FFFF
	movs r1, #0xa0
	movs r2, #0xa9
	movs r3, #0
	bl sub_8021EF4
_08019D6E:
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #3
	bl sub_801A7AC
	b _08019EB0
	.align 2, 0
_08019D7C: .4byte gUnknown_030012C0
_08019D80: .4byte 0x0000FFFF
_08019D84:
	ldr r4, _08019DDC @ =gUnknown_03001308
	ldr r0, [r4]
	ldr r0, [r0, #0x10]
	ldrh r1, [r0, #0x10]
	adds r0, r5, #0
	movs r2, #0x28
	movs r3, #1
	bl sub_801A03C
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0x46
	movs r3, #0
	bl sub_801A03C
	ldr r0, [r4]
	ldr r0, [r0, #0x10]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #0x10
	movs r3, #0x8c
	lsls r3, r3, #0xf
	adds r1, r1, r3
	lsrs r1, r1, #0x10
	adds r0, r5, #0
	movs r2, #0x64
	movs r3, #1
	bl sub_801A03C
_08019DBC:
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #0
	bl sub_803AD84
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #1
	bl sub_801A7AC
	b _08019EB0
	.align 2, 0
_08019DDC: .4byte gUnknown_03001308
_08019DE0:
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #6
	b _08019E18
_08019DF2:
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #1
	b _08019E18
_08019E04:
	movs r0, #0
	str r0, [r5, #0x28]
_08019E08:
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #4
_08019E18:
	bl sub_803AD84
_08019E1C:
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #0
	bl sub_801A7AC
	b _08019EB0
_08019E28:
	ldr r0, [r5, #0x2c]
	ldr r0, [r0, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #2
	bl sub_803AD80
	movs r0, #0xd2
	str r0, [r5, #0x20]
	b _08019EB0
_08019E42:
	ldr r0, [r5, #0x2c]
	ldr r0, [r0, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #1
	bl sub_803AD80
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #2
	bl sub_803AD84
	b _08019EB0
_08019E6C:
	movs r0, #0x64
	str r0, [r5, #0x20]
	ldr r0, [r5, #0x10]
	cmp r0, #2
	ble _08019E7A
	movs r0, #1
	str r0, [r5, #0x20]
_08019E7A:
	ldr r0, _08019EB8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x15
	bl PlaySfx
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r3, [r6, #0xc]
	ands r0, r3
	strb r0, [r6, #0xc]
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #2
	bl sub_801A7AC
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #1
	bl sub_803AD84
_08019EB0:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08019EB8: .4byte gUnknown_030012BC

	thumb_func_start sub_8019EBC
sub_8019EBC: @ 0x08019EBC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r0
	adds r7, r1, #0
	adds r1, r2, #0
	adds r2, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	ldr r0, _08019F04 @ =0x0000FFFF
	movs r3, #0
	bl sub_8009ED0
	adds r6, r0, #0
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r1, [r6, #0xc]
	ands r0, r1
	strb r0, [r6, #0xc]
	ldr r0, _08019F08 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r2, #0xa2
	lsls r2, r2, #2
	adds r0, r0, r2
	str r0, [r6, #0x20]
	cmp r7, #0
	beq _08019F0C
	cmp r7, #1
	beq _08019F54
	movs r5, #0
	b _08019F92
	.align 2, 0
_08019F04: .4byte 0x0000FFFF
_08019F08: .4byte gUnknown_030012D0
_08019F0C:
	movs r5, #1
	adds r4, r6, #0
	adds r4, #0x28
	movs r1, #1
	movs r0, #4
	rsbs r0, r0, #0
	ldrb r3, [r4]
	ands r0, r3
	orrs r0, r1
	strb r0, [r4]
	movs r0, #3
	adds r1, r6, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r6, #0
	bl sub_80087C0
	adds r0, r6, #0
	bl sub_80087B4
	adds r0, r6, #0
	movs r1, #0
	bl sub_800872C
	strb r5, [r6, #0xa]
	movs r0, #0x28
	bl sub_8026EDC
	bl sub_801A794
	adds r5, r0, #0
	ldr r0, [sp, #0x18]
	str r0, [r5, #0x24]
	mov r1, r8
	str r6, [r1, #0x2c]
	b _08019F96
_08019F54:
	ldr r0, _0801A018 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x29
	bl PlaySfx
	movs r0, #7
	adds r1, r6, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r6, #0
	bl sub_80087C0
	adds r0, r6, #0
	bl sub_80087B4
	adds r0, r6, #0
	movs r1, #0
	bl sub_800872C
	movs r0, #4
	strb r0, [r6, #0xa]
	movs r0, #0x20
	bl sub_8026EDC
	bl sub_801A768
	adds r5, r0, #0
	ldr r2, [sp, #0x18]
	str r2, [r5, #0x1c]
_08019F92:
	adds r4, r6, #0
	adds r4, #0x28
_08019F96:
	adds r0, r6, #0
	bl sub_800815C
	adds r2, r6, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	str r5, [r6, #0x44]
	ldr r1, [r5, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x1c]
	adds r1, r6, #0
	bl sub_803AD80
	ldr r0, _0801A01C @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	ldr r3, [r0, #0xc]
	ldrh r1, [r1]
	adds r3, r1, r3
	ldrb r5, [r3]
	lsrs r0, r5, #1
	movs r2, #1
	eors r0, r2
	ands r0, r2
	ands r0, r2
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r5, [r4]
	ands r1, r5
	orrs r1, r0
	strb r1, [r4]
	ldrb r3, [r3]
	lsrs r0, r3, #2
	ands r0, r2
	ands r0, r2
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r4]
	movs r0, #0x10
	ldrb r1, [r6, #0xc]
	orrs r0, r1
	strb r0, [r6, #0xc]
	cmp r7, #0
	bne _0801A024
	ldr r0, _0801A020 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r6, #0
	bl sub_8008E94
	b _0801A02E
	.align 2, 0
_0801A018: .4byte gUnknown_030012BC
_0801A01C: .4byte gUnknown_030012B4
_0801A020: .4byte gUnknown_030012EC
_0801A024:
	ldr r0, _0801A038 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r6, #0
	bl sub_8008E94
_0801A02E:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801A038: .4byte gUnknown_030012F0

	thumb_func_start sub_801A03C
sub_801A03C: @ 0x0801A03C
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r6, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r6, r6, #0x18
	lsrs r6, r6, #0x18
	ldr r0, _0801A108 @ =0x0000FFFF
	movs r3, #0
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _0801A10C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x30
	str r0, [r4, #0x20]
	movs r0, #1
	adds r1, r4, #0
	adds r1, #0x2d
	movs r2, #1
	mov r8, r2
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	movs r0, #6
	strb r0, [r4, #0xa]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_801A724
	adds r5, r0, #0
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	str r5, [r4, #0x44]
	ldr r1, [r5, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	adds r1, r4, #0
	adds r1, #0x28
	mov r3, r8
	ands r6, r3
	lsls r6, r6, #4
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r2, [r1]
	ands r0, r2
	orrs r0, r6
	strb r0, [r1]
	movs r0, #0x10
	ldrb r3, [r4, #0xc]
	orrs r0, r3
	strb r0, [r4, #0xc]
	ldr r1, [r5, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r5, r5, r0
	ldr r2, [r1, #0x1c]
	adds r0, r5, #0
	adds r1, r4, #0
	bl sub_803AD80
	ldr r0, _0801A110 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801A108: .4byte 0x0000FFFF
_0801A10C: .4byte gUnknown_030012D0
_0801A110: .4byte gUnknown_030012F0

	thumb_func_start sub_801A114
sub_801A114: @ 0x0801A114
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x30
	adds r7, r0, #0
	mov sb, r1
	ldr r1, [r1, #0x18]
	movs r2, #0x28
	ldrsh r0, [r1, r2]
	add r0, sb
	ldr r1, [r1, #0x2c]
	bl sub_803AD7C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801A1A6
	ldr r3, _0801A1B8 @ =gUnknown_030012D8
	mov sl, r3
	ldr r0, [r3]
	movs r1, #0x82
	lsls r1, r1, #1
	adds r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0
	bne _0801A1A6
	mov r0, sp
	mov r1, sb
	bl sub_8007C30
	add r2, sp, #0x10
	mov r8, r2
	mov r3, sl
	ldr r1, [r3]
	mov r0, r8
	bl sub_8007CF8
	ldr r0, [sp, #0x18]
	cmp r0, #0
	bne _0801A17C
	mov r0, sl
	ldr r1, [r0]
	add r4, sp, #0x20
	adds r0, r4, #0
	bl sub_8007C30
	mov r0, r8
	ldm r4!, {r1, r2, r3}
	stm r0!, {r1, r2, r3}
	ldr r1, [r4]
	str r1, [r0]
_0801A17C:
	mov r0, r8
	mov r1, sp
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801A1A6
	mov r1, sl
	ldr r0, [r1]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	mov r3, sb
	ldrb r2, [r3, #0xa]
	ldr r4, [r1, #4]
	movs r1, #0
	movs r3, #0
	bl sub_803AD88
_0801A1A6:
	ldr r0, [r7, #8]
	cmp r0, #5
	bhi _0801A298
	lsls r0, r0, #2
	ldr r1, _0801A1BC @ =_0801A1C0
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801A1B8: .4byte gUnknown_030012D8
_0801A1BC: .4byte _0801A1C0
_0801A1C0: @ jump table
	.4byte _0801A1D8 @ case 0
	.4byte _0801A220 @ case 1
	.4byte _0801A23A @ case 2
	.4byte _0801A254 @ case 3
	.4byte _0801A254 @ case 4
	.4byte _0801A298 @ case 5
_0801A1D8:
	movs r5, #0x10
	movs r0, #0x80
	lsls r0, r0, #1
	orrs r5, r0
	movs r0, #0x80
	lsls r0, r0, #2
	orrs r5, r0
	movs r0, #0x80
	lsls r0, r0, #3
	orrs r5, r0
	movs r0, #0x80
	lsls r0, r0, #4
	orrs r5, r0
	movs r0, #0x80
	lsls r0, r0, #5
	orrs r0, r5
	movs r1, #0x80
	lsls r1, r1, #0xd
	orrs r0, r1
	movs r1, #0x80
	lsls r1, r1, #0x15
	orrs r0, r1
	ldr r1, _0801A21C @ =0x04000050
	str r0, [r1]
	ldr r1, [r7, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r7, r0
	ldr r2, [r1, #0x24]
	movs r1, #5
	bl sub_803AD80
	b _0801A298
	.align 2, 0
_0801A21C: .4byte 0x04000050
_0801A220:
	movs r0, #2
	str r0, [r7, #0x20]
	movs r0, #0
	str r0, [r7, #0x1c]
	ldr r1, [r7, #0xc]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r7, r0
	ldr r2, [r1, #0x24]
	movs r1, #3
	bl sub_803AD80
	b _0801A298
_0801A23A:
	movs r0, #2
	str r0, [r7, #0x20]
	movs r0, #0
	str r0, [r7, #0x1c]
	ldr r1, [r7, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r7, r0
	ldr r2, [r1, #0x24]
	movs r1, #4
	bl sub_803AD80
	b _0801A298
_0801A254:
	ldr r0, [r7, #0x1c]
	cmp r0, #0
	bne _0801A294
	movs r0, #0x14
	str r0, [r7, #0x1c]
	mov r3, sb
	ldrb r2, [r3, #0xd]
	lsrs r1, r2, #2
	movs r0, #1
	eors r1, r0
	ands r1, r0
	lsls r1, r1, #2
	movs r0, #5
	rsbs r0, r0, #0
	ands r0, r2
	orrs r0, r1
	strb r0, [r3, #0xd]
	ldr r0, [r7, #0x20]
	cmp r0, #0
	bne _0801A28C
	ldr r1, [r7, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r7, r0
	ldr r2, [r1, #0x24]
	movs r1, #5
	bl sub_803AD80
_0801A28C:
	ldr r0, [r7, #0x20]
	subs r0, #1
	str r0, [r7, #0x20]
	ldr r0, [r7, #0x1c]
_0801A294:
	subs r0, #1
	str r0, [r7, #0x1c]
_0801A298:
	add sp, #0x30
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_801A2A8
sub_801A2A8: @ 0x0801A2A8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x30
	adds r5, r0, #0
	adds r6, r1, #0
	mov r0, sp
	bl sub_8007C30
	ldr r0, [sp, #8]
	cmp r0, #0
	bne _0801A2C2
	b _0801A3CE
_0801A2C2:
	ldr r0, [r5, #8]
	cmp r0, #4
	beq _0801A338
	cmp r0, #6
	beq _0801A338
	ldr r2, [r5, #0x1c]
	ldrb r1, [r2, #0xc]
	lsrs r0, r1, #6
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _0801A338
	add r4, sp, #0x10
	adds r0, r4, #0
	adds r1, r2, #0
	bl sub_8007B98
	mov r0, sp
	adds r1, r4, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801A338
	ldr r0, [r5, #0x1c]
	ldr r0, [r0, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #7
	bl sub_803AD80
	ldr r1, [r5, #0xc]
	movs r4, #0x20
	ldrsh r0, [r1, r4]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #6
	bl sub_803AD80
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #8
	bl sub_803AD84
	ldr r0, _0801A3E0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x39
	bl PlaySfx
_0801A338:
	ldr r3, _0801A3E4 @ =gUnknown_030012D8
	mov r8, r3
	ldr r1, [r3]
	movs r4, #0x82
	lsls r4, r4, #1
	adds r0, r1, r4
	ldrb r0, [r0]
	cmp r0, #0
	bne _0801A3CE
	add r7, sp, #0x10
	adds r0, r7, #0
	bl sub_8007CF8
	ldr r0, [sp, #0x18]
	cmp r0, #0
	bne _0801A36E
	mov r0, r8
	ldr r1, [r0]
	add r4, sp, #0x20
	adds r0, r4, #0
	bl sub_8007C30
	adds r0, r7, #0
	ldm r4!, {r1, r2, r3}
	stm r0!, {r1, r2, r3}
	ldr r1, [r4]
	str r1, [r0]
_0801A36E:
	mov r0, sp
	adds r1, r7, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801A3CE
	mov r4, r8
	ldr r0, [r4]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldrb r2, [r6, #0xa]
	ldr r4, [r1, #4]
	movs r1, #0
	movs r3, #0
	bl sub_803AD88
	ldr r0, [r5, #8]
	cmp r0, #3
	bne _0801A3CE
	ldr r1, [r5, #0xc]
	movs r4, #0x20
	ldrsh r0, [r1, r4]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #6
	bl sub_803AD80
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #8
	bl sub_803AD84
	ldr r0, _0801A3E0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x39
	bl PlaySfx
_0801A3CE:
	ldr r0, [r5, #8]
	cmp r0, #6
	bls _0801A3D6
	b _0801A56E
_0801A3D6:
	lsls r0, r0, #2
	ldr r1, _0801A3E8 @ =_0801A3EC
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801A3E0: .4byte gUnknown_030012BC
_0801A3E4: .4byte gUnknown_030012D8
_0801A3E8: .4byte _0801A3EC
_0801A3EC: @ jump table
	.4byte _0801A408 @ case 0
	.4byte _0801A430 @ case 1
	.4byte _0801A532 @ case 2
	.4byte _0801A4D4 @ case 3
	.4byte _0801A4D4 @ case 4
	.4byte _0801A498 @ case 5
	.4byte _0801A528 @ case 6
_0801A408:
	ldr r1, [r5, #0xc]
	movs r3, #0x30
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r2, _0801A42C @ =gStaticData_0816C3E8
	ldr r3, [r1, #0x34]
	adds r1, r6, #0
	bl sub_803AD84
	ldr r1, [r5, #0xc]
	movs r4, #0x20
	ldrsh r0, [r1, r4]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #1
	bl sub_803AD80
	b _0801A56E
	.align 2, 0
_0801A42C: .4byte gStaticData_0816C3E8
_0801A430:
	ldr r1, [r6, #4]
	movs r0, #0x80
	lsls r0, r0, #4
	cmp r1, r0
	ble _0801A43C
	b _0801A56E
_0801A43C:
	movs r0, #0
	str r0, [r6, #0x64]
	str r0, [r6, #0x54]
	str r0, [r6, #0x58]
	str r0, [r6, #0x5c]
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #8
	bl sub_803AD84
	adds r0, r6, #0
	bl sub_800815C
	adds r2, r6, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldr r1, [r6]
	lsls r1, r1, #8
	lsrs r1, r1, #0x10
	ldr r2, [r6, #4]
	lsls r2, r2, #8
	lsrs r2, r2, #0x10
	adds r0, r5, #0
	bl sub_801A584
	ldr r1, [r5, #0xc]
	movs r4, #0x20
	ldrsh r0, [r1, r4]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #2
	bl sub_803AD80
	b _0801A56E
_0801A498:
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #9
	bl sub_803AD84
	ldr r1, [r5, #0xc]
	movs r3, #0x30
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r2, _0801A4D0 @ =gStaticData_0816C3F4
	ldr r3, [r1, #0x34]
	adds r1, r6, #0
	bl sub_803AD84
	ldr r1, [r5, #0xc]
	movs r4, #0x20
	ldrsh r0, [r1, r4]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #3
	bl sub_803AD80
	b _0801A56E
	.align 2, 0
_0801A4D0: .4byte gStaticData_0816C3F4
_0801A4D4:
	ldr r1, [r6, #4]
	ldr r0, _0801A51C @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x14]
	lsls r0, r0, #8
	ldr r2, _0801A520 @ =0xFFFFE000
	adds r0, r0, r2
	cmp r1, r0
	blt _0801A56E
	ldr r1, [r5, #0xc]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #6
	bl sub_803AD80
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r4, #0
	ldrsh r0, [r1, r4]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #8
	bl sub_803AD84
	ldr r0, _0801A524 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x39
	bl PlaySfx
	b _0801A56E
	.align 2, 0
_0801A51C: .4byte gUnknown_03001308
_0801A520: .4byte 0xFFFFE000
_0801A524: .4byte gUnknown_030012BC
_0801A528:
	movs r0, #0
	str r0, [r6, #0x64]
	str r0, [r6, #0x54]
	str r0, [r6, #0x58]
	str r0, [r6, #0x5c]
_0801A532:
	adds r0, r6, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801A56E
	movs r0, #1
	ldrb r1, [r6, #0xc]
	orrs r0, r1
	strb r0, [r6, #0xc]
	ldr r0, _0801A57C @ =0x0000FFFF
	ldrh r2, [r6, #8]
	cmp r2, r0
	beq _0801A56E
	ldrh r3, [r6, #8]
	ldr r0, _0801A580 @ =gUnknown_030012B4
	ldr r2, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r1, r0, #2
	movs r4, #0x84
	lsls r4, r4, #1
	adds r2, r2, r4
	adds r2, r2, r1
	lsls r0, r0, #5
	subs r0, r3, r0
	movs r1, #1
	lsls r1, r0
	ldr r0, [r2]
	orrs r0, r1
	str r0, [r2]
_0801A56E:
	add sp, #0x30
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801A57C: .4byte 0x0000FFFF
_0801A580: .4byte gUnknown_030012B4

	thumb_func_start sub_801A584
sub_801A584: @ 0x0801A584
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	ldr r0, _0801A63C @ =0x0000FFFF
	movs r3, #0
	bl sub_8009ED0
	adds r5, r0, #0
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r1, [r5, #0xc]
	ands r0, r1
	strb r0, [r5, #0xc]
	ldr r0, _0801A640 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r2, #0xa2
	lsls r2, r2, #2
	adds r0, r0, r2
	str r0, [r5, #0x20]
	movs r0, #8
	adds r1, r5, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r5, #0
	bl sub_80087C0
	adds r0, r5, #0
	bl sub_80087B4
	adds r0, r5, #0
	movs r1, #0
	bl sub_800872C
	movs r0, #1
	strb r0, [r5, #0xa]
	movs r0, #0x20
	bl sub_8026EDC
	adds r4, r0, #0
	bl sub_8017A8C
	ldr r1, _0801A644 @ =gStaticData_087E48A4
	str r1, [r4, #0xc]
	ldr r0, [r6, #0x1c]
	str r0, [r4, #0x1c]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #5
	bl sub_803AD80
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	str r4, [r5, #0x44]
	ldr r1, [r4, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r4, r4, r0
	ldr r2, [r1, #0x1c]
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #0x10
	ldrb r3, [r5, #0xc]
	orrs r0, r3
	strb r0, [r5, #0xc]
	ldr r0, _0801A648 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801A63C: .4byte 0x0000FFFF
_0801A640: .4byte gUnknown_030012D0
_0801A644: .4byte gStaticData_087E48A4
_0801A648: .4byte gUnknown_030012F0

	thumb_func_start sub_801A64C
sub_801A64C: @ 0x0801A64C
	push {r4, r5, lr}
	adds r3, r1, #0
	ldr r4, [r0, #8]
	cmp r4, #0
	beq _0801A65C
	cmp r4, #1
	beq _0801A69C
	b _0801A712
_0801A65C:
	adds r0, r3, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _0801A684
	ldr r1, _0801A680 @ =gStaticData_0816C3B8
	ldr r0, [r1, #0x24]
	rsbs r0, r0, #0
	ldr r2, [r1, #0x2c]
	rsbs r2, r2, #0
	ldr r1, [r1, #0x28]
	str r0, [r3, #0x60]
	str r0, [r3, #0x48]
	str r1, [r3, #0x4c]
	str r2, [r3, #0x50]
	b _0801A712
	.align 2, 0
_0801A680: .4byte gStaticData_0816C3B8
_0801A684:
	ldr r0, _0801A698 @ =gStaticData_0816C3B8
	ldr r1, [r0, #0x24]
	ldr r2, [r0, #0x28]
	ldr r0, [r0, #0x2c]
	str r1, [r3, #0x60]
	str r1, [r3, #0x48]
	str r2, [r3, #0x4c]
	str r0, [r3, #0x50]
	b _0801A712
	.align 2, 0
_0801A698: .4byte gStaticData_0816C3B8
_0801A69C:
	adds r0, r3, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _0801A6CC
	ldr r0, [r3]
	movs r1, #0xa0
	lsls r1, r1, #6
	adds r0, r0, r1
	cmp r0, #0
	bgt _0801A712
	movs r0, #1
	ldrb r2, [r3, #0xc]
	orrs r0, r2
	strb r0, [r3, #0xc]
	ldr r0, _0801A6C8 @ =0x0000FFFF
	ldrh r5, [r3, #8]
	cmp r5, r0
	beq _0801A712
	b _0801A6F2
	.align 2, 0
_0801A6C8: .4byte 0x0000FFFF
_0801A6CC:
	ldr r1, [r3]
	ldr r0, _0801A718 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x10]
	lsls r0, r0, #8
	movs r2, #0xa0
	lsls r2, r2, #6
	adds r0, r0, r2
	cmp r1, r0
	blt _0801A712
	movs r0, #1
	ldrb r5, [r3, #0xc]
	orrs r0, r5
	strb r0, [r3, #0xc]
	ldr r0, _0801A71C @ =0x0000FFFF
	ldrh r1, [r3, #8]
	cmp r1, r0
	beq _0801A712
_0801A6F2:
	ldrh r3, [r3, #8]
	ldr r0, _0801A720 @ =gUnknown_030012B4
	ldr r1, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r2, r0, #2
	movs r5, #0x84
	lsls r5, r5, #1
	adds r1, r1, r5
	adds r1, r1, r2
	lsls r0, r0, #5
	subs r0, r3, r0
	lsls r4, r0
	ldr r0, [r1]
	orrs r0, r4
	str r0, [r1]
_0801A712:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0801A718: .4byte gUnknown_03001308
_0801A71C: .4byte 0x0000FFFF
_0801A720: .4byte gUnknown_030012B4

	thumb_func_start sub_801A724
sub_801A724: @ 0x0801A724
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800CA74
	ldr r0, _0801A738 @ =gStaticData_087E483C
	str r0, [r4, #0xc]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801A738: .4byte gStaticData_087E483C

	thumb_func_start sub_801A73C
sub_801A73C: @ 0x0801A73C
	push {lr}
	ldr r2, _0801A74C @ =gStaticData_087E483C
	str r2, [r0, #0xc]
	bl sub_800CA60
	pop {r0}
	bx r0
	.align 2, 0
_0801A74C: .4byte gStaticData_087E483C

	thumb_func_start sub_801A750
sub_801A750: @ 0x0801A750
	push {lr}
	ldr r2, _0801A764 @ =gStaticData_087E48A4
	str r2, [r0, #0xc]
	movs r2, #0
	str r2, [r0, #0x1c]
	bl sub_8017A78
	pop {r0}
	bx r0
	.align 2, 0
_0801A764: .4byte gStaticData_087E48A4

	thumb_func_start sub_801A768
sub_801A768: @ 0x0801A768
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8017A8C
	ldr r0, _0801A77C @ =gStaticData_087E48A4
	str r0, [r4, #0xc]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801A77C: .4byte gStaticData_087E48A4

	thumb_func_start sub_801A780
sub_801A780: @ 0x0801A780
	push {lr}
	ldr r2, _0801A790 @ =gStaticData_087E490C
	str r2, [r0, #0xc]
	bl sub_8017A78
	pop {r0}
	bx r0
	.align 2, 0
_0801A790: .4byte gStaticData_087E490C
