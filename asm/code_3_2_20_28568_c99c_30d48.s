.include "asm/macros.inc"

.syntax unified
.arm
	thumb_func_start sub_8030D48
sub_8030D48: @ 0x08030D48
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	adds r5, r0, #0
	ldr r0, _08030DF8 @ =gUnknown_03001520
	ldr r0, [r0]
	adds r0, #0x18
	lsls r2, r0, #0xb
	ldr r7, _08030DFC @ =gUnknown_03001528
	ldr r0, [r7]
	movs r6, #0x20
	subs r1, r6, r0
	cmp r1, #0
	bge _08030D6A
	adds r1, #3
_08030D6A:
	asrs r1, r1, #2
	lsls r1, r1, #1
	movs r0, #0xc0
	lsls r0, r0, #0x13
	adds r1, r1, r0
	adds r1, r2, r1
	ldr r3, _08030E00 @ =gUnknown_0300152C
	ldr r4, [r3]
	subs r0, r6, r4
	lsrs r2, r0, #0x1f
	adds r0, r0, r2
	asrs r0, r0, #1
	lsls r0, r0, #5
	adds r0, #2
	adds r6, r1, r0
	movs r2, #0
	mov sl, r3
	cmp r2, r4
	bge _08030DEA
	mov r8, r7
	ldr r1, _08030E04 @ =gUnknown_03001530
	mov sb, r1
_08030D96:
	movs r4, #0
	mov r0, r8
	ldr r1, [r0]
	lsrs r0, r1, #0x1f
	adds r1, r1, r0
	asrs r1, r1, #1
	movs r0, #0x20
	adds r0, r0, r6
	mov ip, r0
	adds r7, r2, #1
	cmp r4, r1
	bge _08030DDE
	mov r3, sb
	adds r2, r6, #0
_08030DB2:
	ldrb r0, [r3]
	ldrh r6, [r5]
	adds r1, r6, r0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	adds r5, #2
	ldrh r6, [r5]
	adds r0, r6, r0
	lsls r0, r0, #0x10
	adds r5, #2
	lsrs r0, r0, #8
	orrs r1, r0
	strh r1, [r2]
	adds r2, #2
	adds r4, #1
	mov r1, r8
	ldr r0, [r1]
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	cmp r4, r0
	blt _08030DB2
_08030DDE:
	mov r6, ip
	adds r2, r7, #0
	mov r1, sl
	ldr r0, [r1]
	cmp r2, r0
	blt _08030D96
_08030DEA:
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08030DF8: .4byte gUnknown_03001520
_08030DFC: .4byte gUnknown_03001528
_08030E00: .4byte gUnknown_0300152C
_08030E04: .4byte gUnknown_03001530

	thumb_func_start sub_8030E08
sub_8030E08: @ 0x08030E08
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	ldr r1, _08030EA0 @ =gUnknown_03001540
	ldr r0, _08030EA4 @ =gUnknown_03001558
	mov sb, r0
	ldr r0, [r1]
	mov r2, sb
	ldr r2, [r2]
	mov ip, r2
	add r0, ip
	str r0, [r1]
	ldr r2, _08030EA8 @ =gUnknown_03001544
	ldr r3, _08030EAC @ =gUnknown_0300155C
	mov r8, r3
	ldr r0, [r2]
	ldr r1, [r3]
	adds r0, r0, r1
	str r0, [r2]
	ldr r0, _08030EB0 @ =gUnknown_03000884
	ldr r5, [r0]
	ldr r3, [r5, #0x1c]
	ldr r0, _08030EB4 @ =gUnknown_0300154C
	mov sl, r0
	ldr r0, [r0]
	ldr r1, _08030EB8 @ =0xFFFFEE00
	adds r0, r0, r1
	subs r3, r3, r0
	ldr r4, _08030EBC @ =gStaticData_0817C3D8
	movs r0, #0
	ldrsh r2, [r4, r0]
	movs r1, #6
	ldrsh r0, [r4, r1]
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	adds r2, r2, r0
	subs r7, r3, r2
	ldr r3, [r5, #0x20]
	ldr r5, _08030EC0 @ =gUnknown_03001550
	ldr r0, [r5]
	movs r2, #0xc0
	lsls r2, r2, #5
	adds r0, r0, r2
	subs r3, r3, r0
	movs r0, #2
	ldrsh r2, [r4, r0]
	movs r1, #8
	ldrsh r0, [r4, r1]
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	adds r2, r2, r0
	subs r3, r3, r2
	asrs r1, r7, #0x1f
	adds r0, r7, #0
	eors r0, r1
	subs r0, r0, r1
	ldr r1, _08030EC4 @ =0x00002CFF
	mov r6, sb
	mov r4, r8
	mov r2, sl
	cmp r0, r1
	bgt _08030ECE
	asrs r0, r7, #0xa
	cmp r0, #0
	blt _08030EC8
	mov r1, ip
	str r1, [r6]
	cmp r0, #0
	beq _08030ECE
	mov r0, ip
	subs r0, #3
	b _08030ECC
	.align 2, 0
_08030EA0: .4byte gUnknown_03001540
_08030EA4: .4byte gUnknown_03001558
_08030EA8: .4byte gUnknown_03001544
_08030EAC: .4byte gUnknown_0300155C
_08030EB0: .4byte gUnknown_03000884
_08030EB4: .4byte gUnknown_0300154C
_08030EB8: .4byte 0xFFFFEE00
_08030EBC: .4byte gStaticData_0817C3D8
_08030EC0: .4byte gUnknown_03001550
_08030EC4: .4byte 0x00002CFF
_08030EC8:
	mov r0, ip
	adds r0, #3
_08030ECC:
	str r0, [r6]
_08030ECE:
	asrs r0, r3, #0x1f
	adds r1, r3, #0
	eors r1, r0
	subs r1, r1, r0
	ldr r0, _08030EEC @ =0x00002CFF
	cmp r1, r0
	bgt _08030EF6
	asrs r1, r3, #0xa
	cmp r1, #0
	blt _08030EF0
	ldr r0, [r4]
	cmp r1, #0
	beq _08030EF6
	subs r0, #2
	b _08030EF4
	.align 2, 0
_08030EEC: .4byte 0x00002CFF
_08030EF0:
	ldr r0, [r4]
	adds r0, #2
_08030EF4:
	str r0, [r4]
_08030EF6:
	ldr r1, [r2]
	movs r0, #0xa0
	lsls r0, r0, #5
	cmp r1, r0
	bgt _08030F06
	ldr r0, [r6]
	adds r0, #6
	str r0, [r6]
_08030F06:
	ldr r1, [r2]
	ldr r0, _08030F74 @ =0x00004FFF
	cmp r1, r0
	ble _08030F14
	ldr r0, [r6]
	subs r0, #6
	str r0, [r6]
_08030F14:
	ldr r1, [r5]
	ldr r0, _08030F78 @ =0xFFFFD300
	cmp r1, r0
	bgt _08030F22
	ldr r0, [r4]
	adds r0, #3
	str r0, [r4]
_08030F22:
	ldr r1, [r5]
	ldr r0, _08030F7C @ =0x000013FF
	cmp r1, r0
	ble _08030F30
	ldr r0, [r4]
	subs r0, #3
	str r0, [r4]
_08030F30:
	adds r2, r6, #0
	ldr r0, [r2]
	movs r1, #0xc0
	lsls r1, r1, #1
	cmp r0, r1
	ble _08030F3E
	adds r0, r1, #0
_08030F3E:
	str r0, [r2]
	ldr r1, _08030F80 @ =0xFFFFFE80
	cmp r0, r1
	bge _08030F48
	adds r0, r1, #0
_08030F48:
	str r0, [r6]
	adds r2, r4, #0
	ldr r0, [r2]
	movs r1, #0x80
	lsls r1, r1, #1
	cmp r0, r1
	ble _08030F58
	adds r0, r1, #0
_08030F58:
	str r0, [r2]
	ldr r1, _08030F84 @ =0xFFFFFF00
	cmp r0, r1
	bge _08030F62
	adds r0, r1, #0
_08030F62:
	str r0, [r4]
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08030F74: .4byte 0x00004FFF
_08030F78: .4byte 0xFFFFD300
_08030F7C: .4byte 0x000013FF
_08030F80: .4byte 0xFFFFFE80
_08030F84: .4byte 0xFFFFFF00

	thumb_func_start sub_8030F88
sub_8030F88: @ 0x08030F88
	push {r4, r5, r6, lr}
	ldr r1, _08031014 @ =gUnknown_03001564
	str r0, [r1]
	bl sub_802973C
	adds r1, r0, #0
	cmp r1, #0
	bne _08030F9C
	ldr r0, _08031018 @ =gUnknown_0300157C
	str r1, [r0]
_08030F9C:
	ldr r1, _0803101C @ =gUnknown_03001528
	ldr r2, _08031020 @ =gStaticData_08167CD4
	movs r3, #0
	ldrsh r0, [r2, r3]
	str r0, [r1]
	ldr r1, _08031024 @ =gUnknown_0300152C
	movs r3, #2
	ldrsh r0, [r2, r3]
	str r0, [r1]
	ldr r4, _08031028 @ =gUnknown_03001534
	movs r0, #0x1c
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r5, r0, #0
	ldr r0, _0803102C @ =gStaticData_0817C3E4
	ldr r1, _08031030 @ =gUnknown_03001580
	movs r2, #1
	str r0, [r5]
	str r1, [r5, #4]
	str r2, [r5, #0x18]
	adds r0, r5, #0
	movs r1, #0
	bl sub_803B0A8
	str r5, [r4]
	movs r4, #0
	ldr r0, _08031034 @ =gUnknown_03001538
	str r4, [r0]
	ldr r0, _08031038 @ =gUnknown_0300153C
	str r4, [r0]
	str r4, [r5, #0xc]
	ldr r0, [r5]
	ldrh r0, [r0]
	movs r6, #0
	strh r0, [r5, #0x10]
	strb r6, [r5, #0x12]
	adds r0, r5, #0
	bl GetAnimFrameBaseOffset
	ldr r2, [r5, #0xc]
	ldr r3, [r5]
	lsls r1, r2, #1
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r1, r1, r3
	movs r2, #4
	ldrsh r1, [r1, r2]
	cmp r0, r1
	blt _08031004
	str r4, [r5, #8]
_08031004:
	bl sub_8031504
	ldr r0, _0803103C @ =gUnknown_03001524
	strb r6, [r0]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08031014: .4byte gUnknown_03001564
_08031018: .4byte gUnknown_0300157C
_0803101C: .4byte gUnknown_03001528
_08031020: .4byte gStaticData_08167CD4
_08031024: .4byte gUnknown_0300152C
_08031028: .4byte gUnknown_03001534
_0803102C: .4byte gStaticData_0817C3E4
_08031030: .4byte gUnknown_03001580
_08031034: .4byte gUnknown_03001538
_08031038: .4byte gUnknown_0300153C
_0803103C: .4byte gUnknown_03001524

	thumb_func_start sub_8031040
sub_8031040: @ 0x08031040
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	adds r6, r0, #0
	adds r5, r1, #0
	mov sb, r2
	mov sl, r3
	ldr r1, _08031170 @ =gUnknown_03001560
	movs r0, #0x66
	str r0, [r1]
	movs r7, #0
	ldr r0, _08031174 @ =gUnknown_03001538
	movs r1, #1
	str r1, [r0]
	ldr r0, _08031178 @ =gUnknown_0300153C
	str r7, [r0]
	ldr r2, _0803117C @ =gUnknown_03001534
	ldr r4, [r2]
	str r7, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	adds r0, r4, #0
	bl GetAnimFrameBaseOffset
	ldr r2, [r4, #0xc]
	ldr r3, [r4]
	lsls r1, r2, #1
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r1, r1, r3
	movs r3, #4
	ldrsh r1, [r1, r3]
	cmp r0, r1
	blt _08031090
	str r7, [r4, #8]
_08031090:
	ldr r0, _08031180 @ =gUnknown_03001540
	mov r8, r0
	lsls r0, r5, #2
	adds r0, r0, r5
	mov r1, r8
	str r0, [r1]
	mov r2, sb
	lsls r0, r2, #1
	ldr r3, _08031184 @ =gUnknown_03001544
	str r0, [r3]
	ldr r5, _08031188 @ =gUnknown_03001548
	movs r0, #0xa0
	lsls r0, r0, #8
	add r0, sl
	str r0, [r5]
	ldr r3, _0803118C @ =gUnknown_03001568
	ldr r0, _08031190 @ =gUnknown_03001564
	ldr r0, [r0]
	lsls r1, r0, #3
	subs r1, r1, r0
	lsls r1, r1, #2
	lsls r0, r6, #3
	subs r0, r0, r6
	lsls r0, r0, #2
	ldr r2, _08031194 @ =gStaticData_0817C2D0
	adds r0, r0, r2
	adds r1, r1, r0
	str r1, [r3]
	ldr r2, _08031198 @ =gUnknown_03001570
	ldr r0, [r1, #0xc]
	str r0, [r2]
	ldr r2, _0803119C @ =gUnknown_0300156C
	ldr r0, [r1]
	str r0, [r2]
	ldr r0, _080311A0 @ =gUnknown_03001574
	str r7, [r0]
	ldr r0, _080311A4 @ =gUnknown_03001524
	movs r1, #1
	strb r1, [r0]
	ldr r0, _080311A8 @ =gUnknown_03001520
	str r7, [r0]
	ldr r4, _080311AC @ =gUnknown_03001554
	bl sub_8029B2C
	lsls r0, r0, #8
	ldr r1, [r5]
	subs r1, r1, r0
	str r1, [r4]
	movs r0, #0xe0
	lsls r0, r0, #0x11
	bl sub_803ADB4
	ldr r2, _080311B0 @ =gUnknown_0300154C
	mov r3, r8
	ldr r1, [r3]
	muls r1, r0, r1
	asrs r1, r1, #0xc
	str r1, [r2]
	ldr r2, _080311B4 @ =gUnknown_03001550
	ldr r3, _08031184 @ =gUnknown_03001544
	ldr r1, [r3]
	muls r0, r1, r0
	asrs r0, r0, #0xc
	str r0, [r2]
	ldr r0, [r4]
	bl sub_8029E34
	ldr r0, _0803117C @ =gUnknown_03001534
	ldr r2, [r0]
	ldr r3, [r2, #8]
	asrs r3, r3, #8
	ldr r1, [r2, #0xc]
	ldr r4, [r2]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r4
	movs r1, #2
	ldrsh r0, [r0, r1]
	adds r0, r0, r3
	ldr r1, [r2, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	bl sub_8030D48
	movs r2, #0x80
	lsls r2, r2, #0x13
	ldrh r0, [r2]
	movs r3, #0x80
	lsls r3, r3, #3
	adds r1, r3, #0
	orrs r0, r1
	strh r0, [r2]
	bl sub_80312C4
	ldr r0, _080311B8 @ =gUnknown_03001578
	str r7, [r0]
	ldr r0, _080311BC @ =gStaticData_0817C378
	ldr r1, _080311C0 @ =0x05000020
	movs r2, #0x20
	movs r3, #0x10
	bl QueueVramDmaTransfer
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08031170: .4byte gUnknown_03001560
_08031174: .4byte gUnknown_03001538
_08031178: .4byte gUnknown_0300153C
_0803117C: .4byte gUnknown_03001534
_08031180: .4byte gUnknown_03001540
_08031184: .4byte gUnknown_03001544
_08031188: .4byte gUnknown_03001548
_0803118C: .4byte gUnknown_03001568
_08031190: .4byte gUnknown_03001564
_08031194: .4byte gStaticData_0817C2D0
_08031198: .4byte gUnknown_03001570
_0803119C: .4byte gUnknown_0300156C
_080311A0: .4byte gUnknown_03001574
_080311A4: .4byte gUnknown_03001524
_080311A8: .4byte gUnknown_03001520
_080311AC: .4byte gUnknown_03001554
_080311B0: .4byte gUnknown_0300154C
_080311B4: .4byte gUnknown_03001550
_080311B8: .4byte gUnknown_03001578
_080311BC: .4byte gStaticData_0817C378
_080311C0: .4byte 0x05000020

	thumb_func_start sub_80311C4
sub_80311C4: @ 0x080311C4
	push {r4, r5, r6, lr}
	ldr r5, _08031298 @ =gUnknown_03001534
	ldr r0, [r5]
	ldr r0, [r0, #8]
	asrs r6, r0, #8
	ldr r1, _0803129C @ =gStaticData_0817C3FC
	ldr r4, _080312A0 @ =gUnknown_03001538
	ldr r0, [r4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	bl sub_803AD78
	bl sub_8031744
	ldr r1, _080312A4 @ =gUnknown_0300153C
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
	ldr r0, [r4]
	cmp r0, #0
	beq _08031290
	ldr r4, [r5]
	movs r0, #0x10
	ldrsh r1, [r4, r0]
	ldr r0, [r4, #8]
	adds r0, r0, r1
	str r0, [r4, #8]
	movs r0, #0
	strb r0, [r4, #0x12]
	adds r0, r4, #0
	bl GetAnimFrameBaseOffset
	ldr r2, [r4, #0xc]
	ldr r3, [r4]
	lsls r1, r2, #1
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r1, r1, r3
	movs r3, #4
	ldrsh r2, [r1, r3]
	cmp r0, r2
	blt _0803122C
	movs r3, #6
	ldrsh r0, [r1, r3]
	subs r0, r2, r0
	lsls r0, r0, #8
	ldr r1, [r4, #8]
	subs r1, r1, r0
	str r1, [r4, #8]
	movs r0, #1
	strb r0, [r4, #0x12]
_0803122C:
	ldr r4, _080312A8 @ =gUnknown_03001554
	bl sub_8029B2C
	ldr r1, _080312AC @ =gUnknown_03001548
	lsls r0, r0, #8
	ldr r1, [r1]
	subs r1, r1, r0
	str r1, [r4]
	movs r0, #0xe0
	lsls r0, r0, #0x11
	bl sub_803ADB4
	ldr r2, _080312B0 @ =gUnknown_0300154C
	ldr r1, _080312B4 @ =gUnknown_03001540
	ldr r1, [r1]
	muls r1, r0, r1
	asrs r1, r1, #0xc
	str r1, [r2]
	ldr r2, _080312B8 @ =gUnknown_03001550
	ldr r1, _080312BC @ =gUnknown_03001544
	ldr r1, [r1]
	muls r0, r1, r0
	asrs r0, r0, #0xc
	str r0, [r2]
	ldr r0, [r4]
	bl sub_8029E34
	ldr r3, [r5]
	ldr r0, [r3, #8]
	asrs r4, r0, #8
	cmp r6, r4
	beq _08031290
	ldr r1, [r3, #0xc]
	ldr r2, [r3]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	movs r1, #2
	ldrsh r0, [r0, r1]
	adds r0, r0, r4
	ldr r1, [r3, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	bl sub_8030D48
	ldr r1, _080312C0 @ =gUnknown_03001524
	movs r0, #1
	strb r0, [r1]
_08031290:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08031298: .4byte gUnknown_03001534
_0803129C: .4byte gStaticData_0817C3FC
_080312A0: .4byte gUnknown_03001538
_080312A4: .4byte gUnknown_0300153C
_080312A8: .4byte gUnknown_03001554
_080312AC: .4byte gUnknown_03001548
_080312B0: .4byte gUnknown_0300154C
_080312B4: .4byte gUnknown_03001540
_080312B8: .4byte gUnknown_03001550
_080312BC: .4byte gUnknown_03001544
_080312C0: .4byte gUnknown_03001524

