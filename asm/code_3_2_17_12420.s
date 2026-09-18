.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8012420
sub_8012420: @ 0x08012420
	push {r4, r5, r6, r7, lr}
	sub sp, #8
	adds r5, r0, #0
	ldr r0, _08012564 @ =gUnknown_030007E0
	ldr r1, [r0]
	adds r2, r5, #0
	adds r2, #0x34
	ldrb r0, [r2]
	cmp r0, #0
	beq _08012444
	movs r0, #0x80
	lsls r0, r0, #1
	ands r1, r0
	lsls r0, r1, #0x10
	lsrs r0, r0, #0x10
	cmp r0, #0
	bne _08012444
	strb r0, [r2]
_08012444:
	adds r4, r5, #0
	adds r4, #0x2e
	ldr r0, [r5, #0x10]
	movs r1, #0x80
	lsls r1, r1, #1
	adds r0, r0, r1
	ldrb r2, [r4]
	ldrb r0, [r0]
	cmp r2, r0
	beq _0801245E
	adds r0, r5, #0
	bl sub_8012238
_0801245E:
	ldr r0, [r5, #0x10]
	movs r3, #0x80
	lsls r3, r3, #1
	adds r0, r0, r3
	ldrb r0, [r0]
	movs r3, #0
	strb r0, [r4]
	ldr r2, [r5, #0x10]
	ldr r1, [r2, #4]
	ldr r4, _08012568 @ =gUnknown_03001308
	ldr r0, [r4]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x14]
	lsls r0, r0, #8
	ldr r6, _0801256C @ =0xFFFFEC00
	adds r0, r0, r6
	cmp r1, r0
	ble _080124DA
	movs r0, #0x7f
	ldrb r1, [r2, #0xc]
	ands r0, r1
	strb r0, [r2, #0xc]
	ldr r1, [r5, #0x10]
	movs r2, #0x80
	lsls r2, r2, #1
	adds r0, r1, r2
	ldrb r0, [r0]
	cmp r0, #0
	bne _0801249A
	str r3, [r1, #0x60]
_0801249A:
	str r3, [r1, #0x48]
	str r3, [r1, #0x4c]
	str r3, [r1, #0x50]
	ldr r2, [r5, #0x10]
	ldr r1, [r2, #4]
	ldr r0, [r4]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x14]
	lsls r0, r0, #8
	movs r4, #0xa0
	lsls r4, r4, #5
	adds r0, r0, r4
	cmp r1, r0
	ble _080124DA
	adds r0, r2, #0
	adds r0, #0x8c
	str r3, [r0]
	ldr r0, _08012570 @ =gUnknown_030012C0
	ldr r0, [r0]
	movs r1, #0
	bl sub_80231EC
	ldr r1, [r5, #0xc]
	movs r6, #0x10
	ldrsh r0, [r1, r6]
	adds r0, r5, r0
	ldr r4, [r1, #0x14]
	movs r1, #0
	movs r2, #1
	movs r3, #0
	bl sub_803AD88
_080124DA:
	adds r1, r5, #0
	adds r1, #0x26
	ldrb r0, [r1]
	cmp r0, #0
	beq _080124E8
	subs r0, #1
	strb r0, [r1]
_080124E8:
	adds r2, r5, #0
	adds r2, #0x2b
	ldrb r1, [r2]
	cmp r1, #0
	beq _08012536
	ldr r0, [r5, #0x10]
	adds r0, #0x94
	ldrb r0, [r0]
	cmp r0, #1
	bgt _08012536
	subs r0, r1, #1
	strb r0, [r2]
	lsls r0, r0, #0x18
	lsrs r4, r0, #0x18
	cmp r4, #0
	bne _08012536
	movs r0, #0x27
	adds r0, r0, r5
	mov ip, r0
	ldrb r0, [r0]
	cmp r0, #0
	bne _0801252E
	adds r0, r5, #0
	adds r0, #0x2c
	ldrb r2, [r0]
	adds r1, r5, #0
	adds r1, #0x31
	strb r4, [r1]
	adds r3, r5, #0
	adds r3, #0x2f
	movs r1, #1
	strb r1, [r3]
	mov r1, ip
	strb r2, [r1]
	strb r4, [r0]
_0801252E:
	ldr r0, _08012574 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x90
	strb r4, [r0]
_08012536:
	ldr r1, _08012578 @ =gStaticData_0816BF20
	ldr r0, [r5, #8]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r4, #2
	ldrsh r2, [r0, r4]
	adds r4, r1, #0
	cmp r2, #0
	ble _0801257C
	movs r6, #4
	ldrsh r0, [r0, r6]
	adds r0, r5, r0
	ldr r1, [r0]
	lsls r0, r2, #3
	adds r0, r0, r1
	adds r3, r0, #0
	subs r3, #8
	ldr r0, [r3]
	ldr r1, [r3, #4]
	str r0, [sp]
	str r1, [sp, #4]
	ldr r3, [sp, #4]
	b _08012582
	.align 2, 0
_08012564: .4byte gUnknown_030007E0
_08012568: .4byte gUnknown_03001308
_0801256C: .4byte 0xFFFFEC00
_08012570: .4byte gUnknown_030012C0
_08012574: .4byte gUnknown_030012D8
_08012578: .4byte gStaticData_0816BF20
_0801257C:
	adds r0, r4, #4
	adds r0, r3, r0
	ldr r3, [r0]
_08012582:
	ldr r0, [r5, #8]
	lsls r0, r0, #3
	adds r0, r0, r4
	movs r4, #0
	ldrsh r1, [r0, r4]
	cmp r2, #0
	ble _0801259A
	ldr r6, [sp]
	lsls r0, r6, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _0801259C
_0801259A:
	adds r0, r1, #0
_0801259C:
	adds r0, r5, r0
	bl sub_803AD84
	ldr r1, [r5, #0x10]
	adds r1, #0x68
	movs r0, #8
	ldrb r2, [r1]
	ands r0, r2
	movs r3, #0
	strb r0, [r1]
	ldr r0, [r5, #0x10]
	adds r0, #0x68
	ldrb r0, [r0]
	cmp r0, #8
	bne _080125DA
	adds r2, r5, #0
	adds r2, #0x28
	ldrb r0, [r2]
	subs r0, #4
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #1
	bhi _080125DA
	adds r0, r5, #0
	adds r0, #0x32
	strb r3, [r0]
	adds r1, r5, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	strb r3, [r2]
_080125DA:
	adds r0, r5, #0
	bl sub_8012AF4
	ldr r2, [r5, #0x10]
	ldrb r3, [r2, #0xc]
	lsrs r1, r3, #7
	cmp r1, #0
	bne _080125FE
	movs r4, #0x80
	lsls r4, r4, #1
	adds r0, r2, r4
	ldrb r0, [r0]
	cmp r0, #0
	bne _080125F8
	str r1, [r2, #0x60]
_080125F8:
	str r1, [r2, #0x48]
	str r1, [r2, #0x4c]
	str r1, [r2, #0x50]
_080125FE:
	ldr r0, [r5, #8]
	subs r0, #0xc
	cmp r0, #0x15
	bhi _08012684
	lsls r0, r0, #2
	ldr r1, _08012610 @ =_08012614
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08012610: .4byte _08012614
_08012614: @ jump table
	.4byte _08012672 @ case 0
	.4byte _0801266C @ case 1
	.4byte _0801266C @ case 2
	.4byte _0801266C @ case 3
	.4byte _08012684 @ case 4
	.4byte _08012684 @ case 5
	.4byte _08012684 @ case 6
	.4byte _08012684 @ case 7
	.4byte _08012684 @ case 8
	.4byte _08012684 @ case 9
	.4byte _08012684 @ case 10
	.4byte _08012684 @ case 11
	.4byte _08012678 @ case 12
	.4byte _0801267E @ case 13
	.4byte _08012684 @ case 14
	.4byte _08012684 @ case 15
	.4byte _08012684 @ case 16
	.4byte _08012684 @ case 17
	.4byte _08012684 @ case 18
	.4byte _08012684 @ case 19
	.4byte _08012684 @ case 20
	.4byte _0801266C @ case 21
_0801266C:
	ldr r1, [r5, #0x10]
	movs r0, #0x13
	b _08012688
_08012672:
	ldr r1, [r5, #0x10]
	movs r0, #0x14
	b _08012688
_08012678:
	ldr r1, [r5, #0x10]
	movs r0, #0x15
	b _08012688
_0801267E:
	ldr r1, [r5, #0x10]
	movs r0, #0x16
	b _08012688
_08012684:
	ldr r1, [r5, #0x10]
	movs r0, #1
_08012688:
	strb r0, [r1, #0xa]
	add sp, #8
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8012694
sub_8012694: @ 0x08012694
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _0801274C @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp]
	ldr r0, [r4, #8]
	cmp r0, #0xe
	bne _080126A8
	b _08012830
_080126A8:
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r6, #1
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	bne _080126B8
	b _08012830
_080126B8:
	ldr r5, [r4, #0x18]
	cmp r5, #0
	beq _080126C0
	b _08012830
_080126C0:
	ldr r0, _08012750 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231CC
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _080126D0
	b _08012830
_080126D0:
	ldr r0, [r4, #0x10]
	adds r1, r0, #0
	adds r1, #0x2d
	adds r2, r0, #0
	ldrb r1, [r1]
	cmp r1, #6
	bne _08012758
	ldr r0, [r2, #0x30]
	cmp r0, #0
	blt _08012758
	ldr r0, [r4, #0x18]
	adds r0, #1
	str r0, [r4, #0x18]
	ldr r0, _08012754 @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r1, #0x80
	lsls r1, r1, #1
	adds r0, r0, r1
	strb r5, [r0]
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x12
	bl sub_803AD84
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #9
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #6
	bl sub_803AD84
	movs r1, #0xd
	adds r0, r4, #0
	adds r0, #0x31
	strb r5, [r0]
	subs r0, #2
	strb r6, [r0]
	subs r0, #8
	strb r1, [r0]
	adds r0, #0xb
	strb r5, [r0]
	subs r0, #2
	strb r6, [r0]
	subs r0, #8
	strb r1, [r0]
	b _0801281A
	.align 2, 0
_0801274C: .4byte gUnknown_030007E0
_08012750: .4byte gUnknown_030012C0
_08012754: .4byte gUnknown_030012D8
_08012758:
	adds r0, r2, #0
	adds r0, #0x2d
	ldrb r0, [r0]
	cmp r0, #0xb
	bne _080127C4
	ldr r0, [r2, #0x30]
	cmp r0, #0
	blt _080127C4
	ldr r0, [r4, #0x18]
	adds r0, #1
	str r0, [r4, #0x18]
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xb
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0xa
	bl sub_803AD84
	movs r2, #0xe
	adds r1, r4, #0
	adds r1, #0x31
	movs r0, #0
	strb r0, [r1]
	adds r3, r4, #0
	adds r3, #0x2f
	movs r1, #1
	strb r1, [r3]
	subs r3, #8
	strb r2, [r3]
	adds r3, #0xb
	strb r0, [r3]
	adds r0, r4, #0
	adds r0, #0x30
	strb r1, [r0]
	subs r0, #8
	strb r2, [r0]
	ldr r0, _080127C0 @ =gUnknown_030012BC
	ldr r0, [r0]
	adds r2, #0xf2
	b _08012822
	.align 2, 0
_080127C0: .4byte gUnknown_030012BC
_080127C4:
	adds r0, r2, #0
	adds r0, #0x2d
	ldrb r5, [r0]
	cmp r5, #0xc
	bne _08012830
	ldr r0, [r4, #0x18]
	adds r0, #1
	str r0, [r4, #0x18]
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xb
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0xa
	bl sub_803AD84
	adds r1, r4, #0
	adds r1, #0x31
	movs r0, #0
	strb r0, [r1]
	adds r2, r4, #0
	adds r2, #0x2f
	movs r1, #1
	strb r1, [r2]
	subs r2, #8
	strb r5, [r2]
	adds r2, #0xb
	strb r0, [r2]
	adds r0, r4, #0
	adds r0, #0x30
	strb r1, [r0]
	subs r0, #8
	strb r5, [r0]
_0801281A:
	ldr r0, _0801282C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
_08012822:
	movs r1, #0xc
	bl PlaySfx
	movs r0, #1
	b _08012832
	.align 2, 0
_0801282C: .4byte gUnknown_030012BC
_08012830:
	movs r0, #0
_08012832:
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_801283C
sub_801283C: @ 0x0801283C
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	movs r6, #0
	ldr r0, [r4, #0x10]
	ldr r1, [r0, #0x64]
	ldr r0, _0801287C @ =0x0000027F
	cmp r1, r0
	bgt _0801285C
	movs r6, #1
	adds r0, r4, #0
	bl sub_8012694
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801285C
	b _08012A72
_0801285C:
	ldr r0, _08012880 @ =gUnknown_030007E0
	ldr r5, [r0]
	ldr r0, [r4, #8]
	cmp r0, #7
	bne _08012888
	ldr r1, [r4, #0x10]
	ldr r0, [r1, #0x64]
	rsbs r2, r0, #0
	ldr r0, _08012884 @ =0x000001BF
	cmp r2, r0
	bgt _080128FA
	subs r0, #0x40
	cmp r2, r0
	bgt _080128FA
	b _080128B6
	.align 2, 0
_0801287C: .4byte 0x0000027F
_08012880: .4byte gUnknown_030007E0
_08012884: .4byte 0x000001BF
_08012888:
	cmp r0, #9
	bne _080128A4
	ldr r1, [r4, #0x10]
	ldr r0, [r1, #0x64]
	rsbs r2, r0, #0
	ldr r0, _080128A0 @ =0x000001BF
	cmp r2, r0
	bgt _080128FA
	cmp r2, #0x7f
	bgt _080128FA
	b _080128B6
	.align 2, 0
_080128A0: .4byte 0x000001BF
_080128A4:
	cmp r0, #0xb
	bne _080128D0
	ldr r1, [r4, #0x10]
	ldr r0, [r1, #0x64]
	rsbs r0, r0, #0
	cmp r0, #0xff
	bgt _080128FA
	cmp r0, #0x1f
	bgt _080128FA
_080128B6:
	movs r0, #1
	ldrb r2, [r1, #0xd]
	orrs r0, r2
	strb r0, [r1, #0xd]
	ldr r1, [r4, #0xc]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x1a
	bl sub_803AD80
	b _080128FA
_080128D0:
	cmp r0, #0xe
	bne _08012900
	adds r0, r4, #0
	adds r0, #0x22
	ldrb r0, [r0]
	cmp r0, #0
	beq _080128FA
	ldr r1, [r4, #0x10]
	ldr r2, [r1, #0x64]
	rsbs r0, r2, #0
	cmp r0, #0x7f
	bgt _080128F0
	movs r0, #1
	ldrb r3, [r1, #0xd]
	orrs r0, r3
	strb r0, [r1, #0xd]
_080128F0:
	cmp r2, #0
	ble _080128FA
	adds r0, r4, #0
	bl sub_80151C8
_080128FA:
	ldr r0, [r4, #8]
	cmp r0, #0xe
	beq _080129B8
_08012900:
	cmp r0, #0xb
	beq _080129B8
	cmp r6, #0
	beq _080129B8
	movs r0, #0x80
	lsls r0, r0, #1
	ands r5, r0
	cmp r5, #0
	beq _080129B8
	adds r0, r4, #0
	adds r0, #0x34
	ldrb r5, [r0]
	cmp r5, #0
	bne _080129B8
	ldr r1, [r4, #0x10]
	movs r0, #1
	ldrb r2, [r1, #0xd]
	orrs r0, r2
	strb r0, [r1, #0xd]
	adds r0, r4, #0
	adds r0, #0x2d
	ldrb r1, [r0]
	cmp r1, #9
	beq _08012936
	ldr r0, [r4, #8]
	cmp r0, #9
	bne _08012972
_08012936:
	ldr r1, [r4, #0xc]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xa
	bl sub_803AD80
	adds r0, r4, #0
	adds r0, #0x31
	strb r5, [r0]
	subs r0, #2
	movs r1, #1
	strb r1, [r0]
	subs r0, #8
	strb r5, [r0]
	movs r2, #0x16
	adds r0, #0xb
	strb r5, [r0]
	subs r0, #2
	strb r1, [r0]
	subs r0, #8
	strb r2, [r0]
	adds r0, r4, #0
	bl sub_80138E8
	adds r0, r4, #0
	adds r0, #0x2a
	strb r5, [r0]
	b _08012A72
_08012972:
	cmp r1, #7
	bne _080129B8
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #8
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r3, #0
	ldrsh r0, [r2, r3]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x19
	bl sub_803AD84
	adds r0, r4, #0
	adds r0, #0x31
	strb r5, [r0]
	subs r0, #2
	movs r1, #1
	strb r1, [r0]
	subs r0, #8
	strb r5, [r0]
	movs r2, #0x15
	adds r0, #0xb
	strb r5, [r0]
	subs r0, #2
	strb r1, [r0]
	subs r0, #8
	strb r2, [r0]
_080129B8:
	ldr r0, [r4, #8]
	cmp r0, #7
	beq _080129CE
	cmp r0, #9
	beq _080129CE
	cmp r0, #0xb
	beq _080129CE
	cmp r0, #0xe
	beq _080129CE
	cmp r0, #0x1a
	bne _08012A72
_080129CE:
	ldr r0, _080129F8 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #2
	bhi _080129FC
	movs r1, #0
	adds r0, r4, #0
	adds r0, #0x31
	strb r1, [r0]
	adds r2, r4, #0
	adds r2, #0x2f
	movs r0, #1
	strb r0, [r2]
	adds r0, r4, #0
	adds r0, #0x27
	strb r1, [r0]
	b _08012A5A
	.align 2, 0
_080129F8: .4byte gUnknown_03001304
_080129FC:
	adds r3, r4, #0
	adds r3, #0x27
	ldrb r1, [r3]
	adds r0, r1, #0
	subs r0, #0x1b
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #1
	bhi _08012A1C
	movs r1, #0x1c
	adds r2, r4, #0
	adds r2, #0x31
	movs r0, #1
	strb r0, [r2]
	subs r2, #2
	b _08012A56
_08012A1C:
	ldr r0, [r4, #8]
	cmp r0, #0xe
	bne _08012A26
	movs r0, #7
	b _08012A36
_08012A26:
	ldr r2, [r4, #0x18]
	cmp r2, #0
	beq _08012A48
	lsls r0, r1, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #0xd
	beq _08012A5A
	movs r0, #0xd
_08012A36:
	adds r2, r4, #0
	adds r2, #0x31
	movs r1, #0
	strb r1, [r2]
	subs r2, #2
	movs r1, #1
	strb r1, [r2]
	strb r0, [r3]
	b _08012A5A
_08012A48:
	movs r1, #7
	adds r0, r4, #0
	adds r0, #0x31
	strb r2, [r0]
	adds r2, r4, #0
	adds r2, #0x2f
	movs r0, #1
_08012A56:
	strb r0, [r2]
	strb r1, [r3]
_08012A5A:
	ldr r0, _08012A78 @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r1, #0x80
	lsls r1, r1, #1
	adds r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0
	beq _08012A72
	adds r1, r4, #0
	adds r1, #0x31
	movs r0, #1
	strb r0, [r1]
_08012A72:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08012A78: .4byte gUnknown_030012D8
