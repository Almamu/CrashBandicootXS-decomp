.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8028568
sub_8028568: @ 0x08028568
	str r1, [r0, #0x28]
	bx lr

	thumb_func_start sub_802856C
sub_802856C: @ 0x0802856C
	ldr r1, [r0, #0x28]
	adds r1, #1
	str r1, [r0, #0x28]
	bx lr

	thumb_func_start sub_8028574
sub_8028574: @ 0x08028574
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	ldr r1, [r5, #0x64]
	cmp r1, #0
	beq _080285AE
	subs r0, r1, #4
	ldr r0, [r0]
	lsls r0, r0, #6
	adds r4, r1, r0
	cmp r1, r4
	beq _080285A6
_0802858C:
	subs r4, #0x40
	ldr r1, [r4, #0x18]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #4]
	movs r1, #0
	bl sub_803AD80
	ldr r0, [r5, #0x64]
	cmp r0, r4
	bne _0802858C
_080285A6:
	ldr r0, [r5, #0x64]
	subs r0, #4
	bl sub_8026EB4
_080285AE:
	movs r0, #1
	ands r0, r6
	cmp r0, #0
	beq _080285BC
	adds r0, r5, #0
	bl sub_8026ED0
_080285BC:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

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

	thumb_func_start sub_8028808
sub_8028808: @ 0x08028808
	push {r4, lr}
	adds r3, r0, #0
	lsls r1, r1, #0x18
	lsrs r4, r1, #0x18
	cmp r4, #0xa
	beq _08028822
	cmp r4, #0x20
	bne _08028844
	movs r0, #0x88
	lsls r0, r0, #1
	adds r2, r3, r0
	adds r0, #0x10
	b _08028838
_08028822:
	movs r2, #0x88
	lsls r2, r2, #1
	adds r1, r3, r2
	adds r2, #8
	adds r0, r3, r2
	ldr r0, [r0]
	str r0, [r1]
	movs r0, #0x8a
	lsls r0, r0, #1
	adds r2, r3, r0
	adds r0, #8
_08028838:
	adds r1, r3, r0
	ldr r0, [r2]
	ldr r1, [r1]
	adds r0, r0, r1
	str r0, [r2]
	b _0802885A
_08028844:
	movs r1, #0x98
	lsls r1, r1, #1
	adds r0, r3, r1
	ldr r1, [r0]
	movs r2, #0x30
	ldrsh r0, [r1, r2]
	adds r0, r3, r0
	ldr r2, [r1, #0x34]
	adds r1, r4, #0
	bl sub_803AD80
_0802885A:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8028860
sub_8028860: @ 0x08028860
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	adds r5, r1, #0
	cmp r2, #0
	ble _0802888A
	adds r4, r2, #0
_0802886C:
	movs r1, #0x98
	lsls r1, r1, #1
	adds r0, r6, r1
	ldr r2, [r0]
	movs r1, #0x38
	ldrsh r0, [r2, r1]
	adds r0, r6, r0
	ldrb r1, [r5]
	ldr r2, [r2, #0x3c]
	bl sub_803AD80
	adds r5, #1
	subs r4, #1
	cmp r4, #0
	bne _0802886C
_0802888A:
	pop {r4, r5, r6}
	pop {r0}
	bx r0

	thumb_func_start sub_8028890
sub_8028890: @ 0x08028890
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldrb r3, [r5]
	cmp r3, #0
	beq _080288F8
	movs r0, #0x88
	lsls r0, r0, #1
	adds r6, r4, r0
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r7, r4, r1
_080288A8:
	cmp r3, #0xa
	beq _080288C0
	cmp r3, #0x20
	bne _080288DA
	movs r2, #0x90
	lsls r2, r2, #1
	adds r0, r4, r2
	ldr r1, [r6]
	ldr r0, [r0]
	adds r1, r1, r0
	str r1, [r6]
	b _080288F0
_080288C0:
	movs r1, #0x8c
	lsls r1, r1, #1
	adds r0, r4, r1
	ldr r0, [r0]
	str r0, [r6]
	movs r2, #0x8e
	lsls r2, r2, #1
	adds r1, r4, r2
	ldr r0, [r7]
	ldr r1, [r1]
	adds r0, r0, r1
	str r0, [r7]
	b _080288F0
_080288DA:
	movs r1, #0x98
	lsls r1, r1, #1
	adds r0, r4, r1
	ldr r1, [r0]
	movs r2, #0x30
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x34]
	adds r1, r3, #0
	bl sub_803AD80
_080288F0:
	adds r5, #1
	ldrb r3, [r5]
	cmp r3, #0
	bne _080288A8
_080288F8:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8028900
sub_8028900: @ 0x08028900
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r1
	adds r5, r2, #0
	movs r3, #0
	movs r4, #0
	cmp r3, r5
	bge _08028958
	movs r1, #0x90
	lsls r1, r1, #1
	adds r1, r1, r0
	mov ip, r1
	movs r7, #0x86
	lsls r7, r7, #1
	adds r7, r7, r0
	mov sb, r7
	adds r6, r0, #0
	adds r6, #8
_08028928:
	mov r1, r8
	adds r0, r1, r4
	ldrb r0, [r0]
	cmp r0, #0xa
	beq _08028952
	cmp r0, #0x20
	bne _0802893C
	mov r7, ip
	ldr r0, [r7]
	b _08028950
_0802893C:
	adds r1, r6, r0
	mov r0, sb
	ldr r2, [r0]
	ldrb r7, [r1]
	lsls r0, r7, #1
	adds r1, r7, #0
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
_08028950:
	adds r3, r3, r0
_08028952:
	adds r4, #1
	cmp r4, r5
	blt _08028928
_08028958:
	adds r0, r3, #0
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8028968
sub_8028968: @ 0x08028968
	push {r4, lr}
	movs r2, #0x8e
	lsls r2, r2, #1
	adds r0, r0, r2
	ldr r3, [r0]
	ldrb r2, [r1]
	cmp r2, #0
	beq _0802898A
	adds r4, r0, #0
_0802897A:
	cmp r2, #0xa
	bne _08028982
	ldr r0, [r4]
	adds r3, r3, r0
_08028982:
	adds r1, #1
	ldrb r2, [r1]
	cmp r2, #0
	bne _0802897A
_0802898A:
	adds r0, r3, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start MeasureText
MeasureText: @ 0x08028994
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov ip, r0
	adds r4, r1, #0
	movs r5, #0
	movs r3, #0
	ldrb r0, [r4]
	cmp r0, #0
	beq _080289EE
	movs r1, #0x90
	lsls r1, r1, #1
	add r1, ip
	mov r8, r1
	movs r6, #0x86
	lsls r6, r6, #1
	add r6, ip
_080289B6:
	cmp r0, #0xa
	beq _080289C4
	cmp r0, #0x20
	bne _080289CE
	mov r7, r8
	ldr r0, [r7]
	b _080289E4
_080289C4:
	cmp r3, r5
	bls _080289CA
	adds r5, r3, #0
_080289CA:
	movs r3, #0
	b _080289E6
_080289CE:
	mov r1, ip
	adds r1, #8
	adds r1, r1, r0
	ldr r2, [r6]
	ldrb r7, [r1]
	lsls r0, r7, #1
	adds r1, r7, #0
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
_080289E4:
	adds r3, r3, r0
_080289E6:
	adds r4, #1
	ldrb r0, [r4]
	cmp r0, #0
	bne _080289B6
_080289EE:
	adds r0, r3, #0
	cmp r0, r5
	bhs _080289F6
	adds r0, r5, #0
_080289F6:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start UploadHudTile
UploadHudTile: @ 0x08028A00
	push {lr}
	adds r3, r0, #0
	movs r1, #0x94
	lsls r1, r1, #1
	adds r0, r3, r1
	ldr r0, [r0]
	adds r1, #4
	adds r2, r3, r1
	ldr r1, [r0]
	lsrs r1, r1, #0xd
	str r1, [r2]
	movs r2, #0x84
	lsls r2, r2, #1
	adds r1, r3, r2
	ldr r1, [r1]
	lsls r1, r1, #5
	ldr r2, _08028A2C @ =0x06010000
	adds r1, r1, r2
	bl LoadTaggedAsset
	pop {r0}
	bx r0
	.align 2, 0
_08028A2C: .4byte 0x06010000

	thumb_func_start sub_8028A30
sub_8028A30: @ 0x08028A30
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x14
	movs r2, #0xf
	ldrb r3, [r0, #5]
	ands r2, r3
	orrs r2, r1
	strb r2, [r0, #5]
	bx lr

	thumb_func_start sub_8028A40
sub_8028A40: @ 0x08028A40
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, _08028A70 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r1, _08028A74 @ =gUnknown_030012D0
	ldr r1, [r1]
	ldr r1, [r1]
	ldr r1, [r1]
	movs r2, #0xd2
	lsls r2, r2, #1
	adds r1, r1, r2
	ldr r1, [r1]
	ldrb r1, [r1, #0x14]
	bl sub_8006DF8
	lsls r0, r0, #4
	movs r1, #0xf
	ldrb r2, [r4, #5]
	ands r1, r2
	orrs r1, r0
	strb r1, [r4, #5]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08028A70: .4byte gUnknown_030012B8
_08028A74: .4byte gUnknown_030012D0

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

	thumb_func_start sub_8028AC4
sub_8028AC4: @ 0x08028AC4
	push {lr}
	adds r2, r1, #0
	movs r1, #0x8e
	lsls r1, r1, #1
	adds r0, r0, r1
	ldr r1, [r0]
	adds r0, r2, #0
	bl sub_8037E54
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8028ADC
sub_8028ADC: @ 0x08028ADC
	movs r1, #0x96
	lsls r1, r1, #1
	adds r0, r0, r1
	ldr r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8028AE8
sub_8028AE8: @ 0x08028AE8
	push {r4, lr}
	adds r3, r0, #0
	movs r4, #0x88
	lsls r4, r4, #1
	adds r0, r3, r4
	str r1, [r0]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r3, r1
	str r2, [r0]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8028B04
sub_8028B04: @ 0x08028B04
	push {r4, lr}
	adds r3, r0, #0
	movs r0, #0x88
	lsls r0, r0, #1
	adds r2, r3, r0
	movs r4, #0x8c
	lsls r4, r4, #1
	adds r0, r3, r4
	ldr r0, [r0]
	str r0, [r2]
	movs r2, #0x8a
	lsls r2, r2, #1
	adds r0, r3, r2
	str r1, [r0]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8028B28
sub_8028B28: @ 0x08028B28
	movs r1, #0x8c
	lsls r1, r1, #1
	adds r0, r0, r1
	ldr r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8028B34
sub_8028B34: @ 0x08028B34
	movs r2, #0x8c
	lsls r2, r2, #1
	adds r0, r0, r2
	str r1, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8028B40
sub_8028B40: @ 0x08028B40
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r0, r1
	ldr r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8028B4C
sub_8028B4C: @ 0x08028B4C
	movs r1, #0x88
	lsls r1, r1, #1
	adds r0, r0, r1
	ldr r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8028B58
sub_8028B58: @ 0x08028B58
	push {lr}
	movs r3, #0x84
	lsls r3, r3, #1
	adds r2, r0, r3
	str r1, [r2]
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
	pop {r0}
	bx r0

	thumb_func_start InitHudTextWidget
InitHudTextWidget: @ 0x08028B7C
	push {lr}
	adds r3, r0, #0
	movs r0, #0x98
	lsls r0, r0, #1
	adds r2, r3, r0
	ldr r0, _08028B9C @ =gStaticData_087E4DAC
	str r0, [r2]
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08028B98
	adds r0, r3, #0
	bl sub_8026ED0
_08028B98:
	pop {r0}
	bx r0
	.align 2, 0
_08028B9C: .4byte gStaticData_087E4DAC

	thumb_func_start InitObjTileFreeList
InitObjTileFreeList: @ 0x08028BA0
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r7, r0, #0
	ldr r6, _08028C2C @ =gUnknown_03001320
	movs r0, #0x80
	lsls r0, r0, #4
	movs r4, #0x80
	lsls r4, r4, #0x17
	adds r1, r4, #0
	bl mem_alloc
	str r0, [r6]
	ldr r5, _08028C30 @ =gUnknown_03001340
	movs r0, #0x80
	lsls r0, r0, #3
	adds r1, r4, #0
	bl mem_alloc
	str r0, [r5]
	ldr r0, _08028C34 @ =0x06018000
	subs r4, r0, r7
	mov r1, sp
	movs r0, #0
	strh r0, [r1]
	ldr r2, _08028C38 @ =0x040000D4
	str r1, [r2]
	str r7, [r2, #4]
	lsrs r0, r4, #0x1f
	adds r0, r4, r0
	asrs r0, r0, #1
	movs r1, #0x81
	lsls r1, r1, #0x18
	orrs r0, r1
	str r0, [r2, #8]
	ldr r0, [r2, #8]
	ldr r1, _08028C3C @ =gUnknown_0300133C
	ldr r0, [r6]
	str r0, [r1]
	adds r3, r0, #0
	movs r1, #0x7d
_08028BF0:
	adds r0, r3, #0
	adds r0, #0x10
	str r0, [r3, #8]
	adds r3, r0, #0
	subs r1, #1
	cmp r1, #0
	bge _08028BF0
	movs r2, #0
	str r2, [r3, #8]
	adds r3, #0x10
	ldr r0, _08028C40 @ =gUnknown_03001328
	str r3, [r0, #8]
	str r3, [r0, #0xc]
	movs r1, #1
	strh r1, [r0, #6]
	strh r2, [r0, #4]
	ldr r1, _08028C34 @ =0x06018000
	str r1, [r0]
	strh r2, [r3, #6]
	str r0, [r3, #0xc]
	str r0, [r3, #8]
	strh r4, [r3, #4]
	str r7, [r3]
	ldr r0, _08028C44 @ =gUnknown_03001338
	str r3, [r0]
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08028C2C: .4byte gUnknown_03001320
_08028C30: .4byte gUnknown_03001340
_08028C34: .4byte 0x06018000
_08028C38: .4byte 0x040000D4
_08028C3C: .4byte gUnknown_0300133C
_08028C40: .4byte gUnknown_03001328
_08028C44: .4byte gUnknown_03001338

	thumb_func_start sub_8028C48
sub_8028C48: @ 0x08028C48
	push {r4, lr}
	cmp r0, #0
	beq _08028CBA
	ldr r2, _08028CC0 @ =0xF9FF0000
	adds r1, r0, r2
	lsrs r1, r1, #5
	ldr r2, _08028CC4 @ =gUnknown_03001320
	ldr r0, _08028CC8 @ =gUnknown_03001340
	ldr r0, [r0]
	adds r0, r0, r1
	ldrb r0, [r0]
	lsls r1, r0, #4
	ldr r0, [r2]
	adds r2, r0, r1
	movs r0, #0
	strh r0, [r2, #6]
	ldr r3, [r2, #0xc]
	ldrh r0, [r3, #6]
	cmp r0, #0
	bne _08028C92
	ldrh r4, [r3, #4]
	ldrh r1, [r2, #4]
	adds r0, r4, r1
	strh r0, [r3, #4]
	ldr r0, [r2, #8]
	str r0, [r3, #8]
	str r3, [r0, #0xc]
	ldr r1, _08028CCC @ =gUnknown_03001338
	ldr r0, [r1]
	cmp r2, r0
	bne _08028C88
	str r3, [r1]
_08028C88:
	ldr r1, _08028CD0 @ =gUnknown_0300133C
	ldr r0, [r1]
	str r0, [r2, #8]
	str r2, [r1]
	adds r2, r3, #0
_08028C92:
	ldr r3, [r2, #8]
	ldrh r0, [r3, #6]
	cmp r0, #0
	bne _08028CBA
	ldrh r4, [r2, #4]
	ldrh r1, [r3, #4]
	adds r0, r4, r1
	strh r0, [r2, #4]
	ldr r0, [r3, #8]
	str r0, [r2, #8]
	str r2, [r0, #0xc]
	ldr r1, _08028CCC @ =gUnknown_03001338
	ldr r0, [r1]
	cmp r3, r0
	bne _08028CB2
	str r2, [r1]
_08028CB2:
	ldr r1, _08028CD0 @ =gUnknown_0300133C
	ldr r0, [r1]
	str r0, [r3, #8]
	str r3, [r1]
_08028CBA:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08028CC0: .4byte 0xF9FF0000
_08028CC4: .4byte gUnknown_03001320
_08028CC8: .4byte gUnknown_03001340
_08028CCC: .4byte gUnknown_03001338
_08028CD0: .4byte gUnknown_0300133C

	thumb_func_start sub_8028CD4
sub_8028CD4: @ 0x08028CD4
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	ldr r0, _08028D14 @ =gUnknown_03001338
	ldr r1, [r0]
	ldr r2, [r1, #0xc]
	adds r3, r1, #0
	ldrh r1, [r3, #6]
	adds r6, r0, #0
	cmp r1, #0
	bne _08028CEE
	ldrh r0, [r3, #4]
	cmp r0, r4
	bge _08028D00
_08028CEE:
	cmp r3, r2
	beq _08028D10
	ldr r3, [r3, #8]
	ldrh r0, [r3, #6]
	cmp r0, #0
	bne _08028CEE
	ldrh r1, [r3, #4]
	cmp r1, r4
	blt _08028CEE
_08028D00:
	ldrh r0, [r3, #4]
	subs r5, r0, r4
	cmp r5, #0
	beq _08028D38
	ldr r1, _08028D18 @ =gUnknown_0300133C
	ldr r2, [r1]
	cmp r2, #0
	bne _08028D1C
_08028D10:
	movs r0, #0
	b _08028D5A
	.align 2, 0
_08028D14: .4byte gUnknown_03001338
_08028D18: .4byte gUnknown_0300133C
_08028D1C:
	ldr r0, [r2, #8]
	str r0, [r1]
	movs r1, #0
	strh r5, [r2, #4]
	ldr r0, [r3]
	adds r0, r0, r4
	str r0, [r2]
	strh r1, [r2, #6]
	str r3, [r2, #0xc]
	ldr r0, [r3, #8]
	str r0, [r2, #8]
	str r2, [r0, #0xc]
	str r2, [r3, #8]
	strh r4, [r3, #4]
_08028D38:
	movs r0, #2
	strh r0, [r3, #6]
	ldr r0, [r3, #8]
	str r0, [r6]
	ldr r0, [r3]
	ldr r1, _08028D60 @ =0xF9FF0000
	adds r0, r0, r1
	lsrs r0, r0, #5
	ldr r1, _08028D64 @ =gUnknown_03001340
	ldr r1, [r1]
	adds r1, r1, r0
	ldr r0, _08028D68 @ =gUnknown_03001320
	ldr r0, [r0]
	subs r0, r3, r0
	asrs r0, r0, #4
	strb r0, [r1]
	ldr r0, [r3]
_08028D5A:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08028D60: .4byte 0xF9FF0000
_08028D64: .4byte gUnknown_03001340
_08028D68: .4byte gUnknown_03001320
	thumb_func_start sub_8028D6C
sub_8028D6C: @ 0x08028D6C
	ldr r0, _08028D8C
	ldr	r0, [r0, #0]
	ldr r1, _08028D90
	cmp	r0, #0
	beq _08028D7C
_08028D76:
	ldr	r0, [r0, #8]
	cmp	r0, #0
	bne _08028D76
_08028D7C:
	ldr	r0, [r1, #8]
	cmp	r0, r1
	beq _08028D88
_08028D82:
	ldr	r0, [r0, #8]
	cmp	r0, r1
	bne _08028D82
_08028D88:
	bx	lr
	movs	r0, r0
_08028D8C: .4byte gUnknown_0300133C
_08028D90: .4byte gUnknown_03001328
	movs	r2, #0
	ldr r0, _08028DB4
	ldr	r1, [r0, #8]
	cmp	r1, r0
	beq _08028DB0
	adds	r3, r0, #0
_08028DA0:
	ldrh	r0, [r1, #6]
	cmp	r0, #0
	bne _08028DAA
	ldrh	r0, [r1, #4]
	adds	r2, r0, r2
_08028DAA:
	ldr	r1, [r1, #8]
	cmp	r1, r3
	bne _08028DA0
_08028DB0:
	adds	r0, r2, #0
	bx	lr
_08028DB4: .4byte gUnknown_03001328

	thumb_func_start sub_8028DB8
sub_8028DB8: @ 0x08028DB8
	push {lr}
	ldr r0, _08028DD0 @ =gUnknown_03001340
	ldr r0, [r0]
	bl mem_free
	ldr r0, _08028DD4 @ =gUnknown_03001320
	ldr r0, [r0]
	bl mem_free
	pop {r0}
	bx r0
	.align 2, 0
_08028DD0: .4byte gUnknown_03001340
_08028DD4: .4byte gUnknown_03001320

	thumb_func_start sub_8028DD8
sub_8028DD8: @ 0x08028DD8
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	lsls r1, r1, #0x10
	lsrs r7, r1, #0x10
	movs r0, #0x80
	lsls r0, r0, #1
	ands r0, r4
	cmp r0, #0
	beq _08028E54
	movs r0, #0x80
	lsls r0, r0, #0x15
	ands r0, r4
	cmp r0, #0
	beq _08028DF8
	rsbs r0, r2, #0
	b _08028DFA
_08028DF8:
	adds r0, r2, #0
_08028DFA:
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	ldr r1, _08028E14 @ =0xFFFF0000
	ands r3, r1
	orrs r3, r0
	movs r0, #0x80
	lsls r0, r0, #0x16
	ands r0, r4
	cmp r0, #0
	beq _08028E18
	rsbs r1, r2, #0
	b _08028E1A
	.align 2, 0
_08028E14: .4byte 0xFFFF0000
_08028E18:
	adds r1, r2, #0
_08028E1A:
	lsls r1, r1, #0x10
	ldr r0, _08028E70 @ =0x0000FFFF
	ands r3, r0
	orrs r3, r1
	ldr r0, _08028E74 @ =0xCFFFFFFF
	ands r4, r0
	ldr r0, _08028E78 @ =gUnknown_03001350
	ldr r2, [r0]
	adds r6, r0, #0
	ldr r5, _08028E7C @ =gUnknown_03001348
	cmp r2, #0
	beq _08028E40
	ldr r0, [r5]
	lsls r1, r2, #2
	adds r1, r1, r0
	subs r1, #4
	ldr r0, [r1]
	cmp r3, r0
	beq _08028E4C
_08028E40:
	ldr r0, [r5]
	lsls r1, r2, #2
	adds r1, r1, r0
	str r3, [r1]
	adds r0, r2, #1
	str r0, [r6]
_08028E4C:
	ldr r0, [r6]
	subs r0, #1
	lsls r0, r0, #0x19
	orrs r4, r0
_08028E54:
	ldr r3, _08028E80 @ =gUnknown_0300134C
	ldr r1, [r3]
	ldr r0, _08028E84 @ =gUnknown_03001344
	ldr r2, [r0]
	lsls r0, r1, #3
	adds r0, r0, r2
	str r4, [r0]
	strh r7, [r0, #4]
	adds r1, #1
	str r1, [r3]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08028E70: .4byte 0x0000FFFF
_08028E74: .4byte 0xCFFFFFFF
_08028E78: .4byte gUnknown_03001350
_08028E7C: .4byte gUnknown_03001348
_08028E80: .4byte gUnknown_0300134C
_08028E84: .4byte gUnknown_03001344

	thumb_func_start sub_8028E88
sub_8028E88: @ 0x08028E88
	push {lr}
	ldr r0, _08028EA0 @ =gUnknown_03001348
	ldr r0, [r0]
	bl mem_free
	ldr r0, _08028EA4 @ =gUnknown_03001344
	ldr r0, [r0]
	bl mem_free
	pop {r0}
	bx r0
	.align 2, 0
_08028EA0: .4byte gUnknown_03001348
_08028EA4: .4byte gUnknown_03001344

	thumb_func_start sub_8028EA8
sub_8028EA8: @ 0x08028EA8
	push {r4, r5, lr}
	ldr r4, _08028EDC @ =gUnknown_03001300
	ldr r0, [r4]
	ldr r1, _08028EE0 @ =gUnknown_03001344
	ldr r1, [r1]
	ldr r5, _08028EE4 @ =gUnknown_0300134C
	ldr r2, [r5]
	bl sub_8006A14
	ldr r0, [r4]
	bl sub_8006A48
	ldr r0, [r4]
	ldr r1, _08028EE8 @ =gUnknown_03001348
	ldr r1, [r1]
	ldr r4, _08028EEC @ =gUnknown_03001350
	ldr r2, [r4]
	bl sub_80069E8
	movs r0, #0
	str r0, [r5]
	str r0, [r4]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08028EDC: .4byte gUnknown_03001300
_08028EE0: .4byte gUnknown_03001344
_08028EE4: .4byte gUnknown_0300134C
_08028EE8: .4byte gUnknown_03001348
_08028EEC: .4byte gUnknown_03001350

	thumb_func_start sub_8028EF0
sub_8028EF0: @ 0x08028EF0
	push {r4, r5, lr}
	sub sp, #4
	ldr r4, _08028F40 @ =gUnknown_03001344
	movs r0, #0x80
	lsls r0, r0, #3
	movs r5, #0x80
	lsls r5, r5, #0x17
	adds r1, r5, #0
	bl mem_alloc
	str r0, [r4]
	ldr r4, _08028F44 @ =gUnknown_03001348
	movs r0, #0x80
	adds r1, r5, #0
	bl mem_alloc
	str r0, [r4]
	ldr r0, _08028F48 @ =gUnknown_0300134C
	movs r1, #0
	str r1, [r0]
	ldr r0, _08028F4C @ =gUnknown_03001350
	str r1, [r0]
	mov r1, sp
	movs r2, #0x80
	lsls r2, r2, #2
	adds r0, r2, #0
	strh r0, [r1]
	ldr r1, _08028F50 @ =0x040000D4
	mov r0, sp
	str r0, [r1]
	movs r0, #0xe0
	lsls r0, r0, #0x13
	str r0, [r1, #4]
	ldr r0, _08028F54 @ =0x81000200
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08028F40: .4byte gUnknown_03001344
_08028F44: .4byte gUnknown_03001348
_08028F48: .4byte gUnknown_0300134C
_08028F4C: .4byte gUnknown_03001350
_08028F50: .4byte 0x040000D4
_08028F54: .4byte 0x81000200

	thumb_func_start LoadSpriteFrameTiles
LoadSpriteFrameTiles: @ 0x08028F58
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r0, _08028F74 @ =gUnknown_03000870
	ldr r1, [r0]
	adds r0, r4, #0
	bl sub_803AD7C
	adds r1, r0, #0
	movs r0, #1
	rsbs r0, r0, #0
	cmp r1, r0
	beq _08028F78
	adds r0, r1, #0
	b _08028FE8
	.align 2, 0
_08028F74: .4byte gUnknown_03000870
_08028F78:
	ldr r2, _08028FA0 @ =gUnknown_03001374
	ldr r5, [r2]
	ldr r0, [r5]
	str r0, [r2]
	str r4, [r5, #8]
	ldr r1, _08028FA4 @ =gUnknown_03001354
	str r1, [r5, #4]
	ldr r0, [r1]
	str r0, [r5]
	ldr r0, [r1]
	str r5, [r0, #4]
	str r5, [r1]
	ldrb r1, [r4]
	ldrb r3, [r4, #1]
	adds r0, r1, #0
	muls r0, r3, r0
	lsls r6, r0, #5
	adds r7, r2, #0
	b _08028FC4
	.align 2, 0
_08028FA0: .4byte gUnknown_03001374
_08028FA4: .4byte gUnknown_03001354
_08028FA8:
	ldr r0, _08028FF0 @ =gUnknown_03001364
	ldr r4, [r0, #4]
	ldr r0, [r4, #0xc]
	bl sub_8028C48
	ldr r1, [r4, #4]
	ldr r0, [r4]
	str r0, [r1]
	ldr r1, [r4]
	ldr r0, [r4, #4]
	str r0, [r1, #4]
	ldr r0, [r7]
	str r0, [r4]
	str r4, [r7]
_08028FC4:
	adds r0, r6, #0
	bl sub_8028CD4
	adds r1, r0, #0
	str r1, [r5, #0xc]
	cmp r1, #0
	beq _08028FA8
	ldr r0, [r5, #8]
	adds r0, #4
	lsls r2, r6, #0x10
	lsrs r2, r2, #0x10
	movs r3, #0x10
	bl QueueVramDmaTransfer
	ldr r0, [r5, #0xc]
	ldr r1, _08028FF4 @ =0xF9FF0000
	adds r0, r0, r1
	lsrs r0, r0, #5
_08028FE8:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08028FF0: .4byte gUnknown_03001364
_08028FF4: .4byte 0xF9FF0000

	thumb_func_start SetupSpriteFrameOam
SetupSpriteFrameOam: @ 0x08028FF8
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	adds r7, r3, #0
	lsls r2, r2, #0x10
	lsrs r4, r2, #0x10
	ldrb r1, [r5]
	ldrb r0, [r5, #1]
	cmp r1, r0
	bne _0802902C
	movs r2, #0
	cmp r1, #8
	bne _08029018
	movs r2, #0xc0
	lsls r2, r2, #0x18
	b _0802905E
_08029018:
	cmp r1, #4
	bne _08029022
	movs r2, #0x80
	lsls r2, r2, #0x18
	b _0802905E
_08029022:
	cmp r1, #2
	bne _0802905E
	movs r2, #0x80
	lsls r2, r2, #0x17
	b _0802905E
_0802902C:
	movs r2, #0x80
	lsls r2, r2, #7
	cmp r1, r0
	bge _08029038
	movs r2, #0x80
	lsls r2, r2, #8
_08029038:
	subs r0, r1, r0
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	cmp r0, #4
	bne _0802904A
	movs r0, #0xc0
	lsls r0, r0, #0x18
	b _0802905C
_0802904A:
	cmp r0, #2
	bne _08029054
	movs r0, #0x80
	lsls r0, r0, #0x18
	b _0802905C
_08029054:
	cmp r0, #3
	bne _0802905E
	movs r0, #0x80
	lsls r0, r0, #0x17
_0802905C:
	orrs r2, r0
_0802905E:
	orrs r6, r2
	adds r0, r5, #0
	bl LoadSpriteFrameTiles
	orrs r4, r0
	lsls r0, r4, #0x10
	lsrs r4, r0, #0x10
	adds r0, r6, #0
	adds r1, r4, #0
	adds r2, r7, #0
	bl sub_8028DD8
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_802907C
sub_802907C: @ 0x0802907C
	push {lr}
	ldr r0, _0802908C @ =gUnknown_03001378
	ldr r0, [r0]
	bl mem_free
	pop {r0}
	bx r0
	.align 2, 0
_0802908C: .4byte gUnknown_03001378

	thumb_func_start sub_8029090
sub_8029090: @ 0x08029090
	ldr r3, _080290B4 @ =gUnknown_03001354
	ldr r0, [r3]
	cmp r0, r3
	beq _080290B0
	ldr r1, _080290B8 @ =gUnknown_03001364
	str r1, [r0, #4]
	ldr r2, [r3, #4]
	ldr r0, [r1]
	str r0, [r2]
	ldr r2, [r1]
	ldr r0, [r3, #4]
	str r0, [r2, #4]
	ldr r0, [r3]
	str r0, [r1]
	str r3, [r3, #4]
	str r3, [r3]
_080290B0:
	bx lr
	.align 2, 0
_080290B4: .4byte gUnknown_03001354
_080290B8: .4byte gUnknown_03001364

	thumb_func_start sub_80290BC
sub_80290BC: @ 0x080290BC
	push {r4, lr}
	ldr r4, _080290F8 @ =gUnknown_03001378
	movs r0, #0x80
	lsls r0, r0, #4
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	str r0, [r4]
	ldr r1, _080290FC @ =gUnknown_03001354
	str r1, [r1, #4]
	str r1, [r1]
	ldr r1, _08029100 @ =gUnknown_03001364
	str r1, [r1, #4]
	str r1, [r1]
	ldr r1, _08029104 @ =gUnknown_03001374
	str r0, [r1]
	movs r2, #0x7e
_080290E0:
	adds r1, r0, #0
	adds r1, #0x10
	str r1, [r0]
	adds r0, r1, #0
	subs r2, #1
	cmp r2, #0
	bge _080290E0
	movs r0, #0
	str r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080290F8: .4byte gUnknown_03001378
_080290FC: .4byte gUnknown_03001354
_08029100: .4byte gUnknown_03001364
_08029104: .4byte gUnknown_03001374

	thumb_func_start sub_8029108
sub_8029108: @ 0x08029108
	ldrb r1, [r0]
	ldrb r0, [r0, #1]
	cmp r1, r0
	bne _08029130
	movs r2, #0
	cmp r1, #8
	bne _0802911C
	movs r2, #0xc0
	lsls r2, r2, #0x18
	b _08029162
_0802911C:
	cmp r1, #4
	bne _08029126
	movs r2, #0x80
	lsls r2, r2, #0x18
	b _08029162
_08029126:
	cmp r1, #2
	bne _08029162
	movs r2, #0x80
	lsls r2, r2, #0x17
	b _08029162
_08029130:
	movs r2, #0x80
	lsls r2, r2, #7
	cmp r1, r0
	bge _0802913C
	movs r2, #0x80
	lsls r2, r2, #8
_0802913C:
	subs r0, r1, r0
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	cmp r0, #4
	bne _0802914E
	movs r0, #0xc0
	lsls r0, r0, #0x18
	b _08029160
_0802914E:
	cmp r0, #2
	bne _08029158
	movs r0, #0x80
	lsls r0, r0, #0x18
	b _08029160
_08029158:
	cmp r0, #3
	bne _08029162
	movs r0, #0x80
	lsls r0, r0, #0x17
_08029160:
	orrs r2, r0
_08029162:
	adds r0, r2, #0
	bx lr
	.align 2, 0

	thumb_func_start sub_8029168
sub_8029168: @ 0x08029168
	push {lr}
	ldr r0, _08029178 @ =gUnknown_0300137C
	ldr r0, [r0]
	bl mem_free
	pop {r0}
	bx r0
	.align 2, 0
_08029178: .4byte gUnknown_0300137C

	thumb_func_start DecompressCategorySpriteSheet
DecompressCategorySpriteSheet: @ 0x0802917C
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4]
	lsrs r0, r0, #8
	movs r1, #0x80
	lsls r1, r1, #0x17
	bl mem_alloc
	adds r1, r0, #0
	ldr r0, _080291A0 @ =gUnknown_0300137C
	str r1, [r0]
	adds r0, r4, #0
	bl LoadTaggedAsset
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080291A0: .4byte gUnknown_0300137C

	thumb_func_start SetupActorVramPool
SetupActorVramPool: @ 0x080291A4
	push {r4, r5, r6, lr}
	ldr r0, _08029200 @ =0x06011400
	bl InitObjTileFreeList
	bl sub_8028EF0
	bl sub_80290BC
	ldr r0, _08029204 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xd2
	lsls r1, r1, #1
	adds r6, r0, r1
	adds r1, #0x90
	adds r5, r0, r1
	ldr r0, _08029208 @ =gUnknown_030012B8
	ldr r4, [r0]
	adds r0, r4, #0
	bl sub_8006EA8
	ldr r2, _0802920C @ =gStaticData_08175558
	ldr r0, _08029210 @ =gUnknown_03001380
	ldr r1, [r0]
	movs r0, #0x34
	muls r0, r1, r0
	adds r0, r0, r2
	ldr r0, [r0]
	cmp r0, #0
	bne _08029214
	ldr r0, [r6]
	ldrb r2, [r0, #0x14]
	adds r0, r4, #0
	movs r1, #7
	bl sub_8006D40
	ldr r0, [r5]
	adds r0, #0x4c
	ldrb r2, [r0]
	adds r0, r4, #0
	movs r1, #0xf
	bl sub_8006D40
	b _0802925C
	.align 2, 0
_08029200: .4byte 0x06011400
_08029204: .4byte gUnknown_030012D0
_08029208: .4byte gUnknown_030012B8
_0802920C: .4byte gStaticData_08175558
_08029210: .4byte gUnknown_03001380
_08029214:
	ldr r0, [r6]
	ldrb r2, [r0, #0x14]
	adds r0, r4, #0
	movs r1, #9
	bl sub_8006D40
	ldr r0, [r5]
	adds r0, #0x4c
	ldrb r2, [r0]
	adds r0, r4, #0
	movs r1, #0xc
	bl sub_8006D40
	ldr r0, [r5]
	adds r0, #0xe0
	ldrb r2, [r0, #0x14]
	adds r0, r4, #0
	movs r1, #0xe
	bl sub_8006D40
	ldr r0, [r5]
	adds r0, #0x8c
	ldrb r2, [r0, #0x14]
	adds r0, r4, #0
	movs r1, #0xd
	bl sub_8006D40
	ldr r0, [r5]
	movs r1, #0xa8
	lsls r1, r1, #1
	adds r0, r0, r1
	ldrb r2, [r0, #0x14]
	adds r0, r4, #0
	movs r1, #8
	bl sub_8006D40
_0802925C:
	ldr r0, _08029280 @ =gUnknown_03001318
	ldr r0, [r0]
	ldr r3, _08029284 @ =gStaticData_08175558
	ldr r1, _08029288 @ =gUnknown_03001380
	ldr r2, [r1]
	movs r1, #0x34
	muls r1, r2, r1
	adds r1, r1, r3
	ldr r2, [r1]
	rsbs r1, r2, #0
	orrs r1, r2
	lsrs r1, r1, #0x1f
	bl sub_802732C
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08029280: .4byte gUnknown_03001318
_08029284: .4byte gStaticData_08175558
_08029288: .4byte gUnknown_03001380

	thumb_func_start InitActorCategory
InitActorCategory: @ 0x0802928C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	movs r1, #1
	str r1, [sp, #0xc]
	ldr r2, _080292FC @ =gUnknown_03001390
	movs r1, #0
	str r1, [r2]
	ldr r6, _08029300 @ =gUnknown_03001380
	str r0, [r6]
	ldr r0, _08029304 @ =gUnknown_03000878
	str r1, [r0]
	ldr r3, _08029308 @ =gUnknown_03001384
	str r1, [r3]
	ldr r7, _0802930C @ =gUnknown_03001388
	str r1, [r7]
	ldr r0, _08029310 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022CA0
	ldr r5, _08029314 @ =gStaticData_08175558
	ldr r0, [r6]
	movs r4, #0x34
	muls r0, r4, r0
	adds r1, r5, #0
	adds r1, #0x1c
	adds r0, r0, r1
	ldr r0, [r0]
	bl DecompressCategorySpriteSheet
	bl SetupActorVramPool
	bl sub_802AAFC
	movs r1, #0
	ldr r0, [r6]
	muls r0, r4, r0
	adds r0, r0, r5
	ldr r0, [r0]
	cmp r0, #0
	bne _080292E6
	movs r1, #1
_080292E6:
	adds r0, r1, #0
	bl sub_802ABFC
	ldr r0, [r6]
	muls r0, r4, r0
	adds r0, r0, r5
	ldr r0, [r0]
	bl sub_8029C30
	b _08029332
	.align 2, 0
_080292FC: .4byte gUnknown_03001390
_08029300: .4byte gUnknown_03001380
_08029304: .4byte gUnknown_03000878
_08029308: .4byte gUnknown_03001384
_0802930C: .4byte gUnknown_03001388
_08029310: .4byte gUnknown_030012C0
_08029314: .4byte gStaticData_08175558
_08029318:
	ldr r4, _08029364 @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_803AFEC
	cmp r0, #0
	bge _08029326
	b _08029654
_08029326:
	ldr r0, [r4]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _08029332
	b _08029654
_08029332:
	ldr r1, _08029368 @ =gUnknown_0300138C
	ldr r0, _0802936C @ =gUnknown_03001390
	ldr r0, [r0]
	str r0, [r1]
	ldr r0, _08029364 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023548
	ldr r0, _08029370 @ =gUnknown_03001380
	ldr r1, [r0]
	movs r0, #0x34
	adds r2, r1, #0
	muls r2, r0, r2
	ldr r0, _08029374 @ =gStaticData_08175558
	adds r0, #0x28
	adds r0, r2, r0
	ldr r5, _08029378 @ =gUnknown_03001388
	ldr r1, [r5]
	ldr r0, [r0]
	cmp r1, r0
	blt _08029394
	ldr r0, _08029374 @ =gStaticData_08175558
	adds r0, #0x30
	adds r0, r2, r0
	b _08029398
	.align 2, 0
_08029364: .4byte gUnknown_030012C0
_08029368: .4byte gUnknown_0300138C
_0802936C: .4byte gUnknown_03001390
_08029370: .4byte gUnknown_03001380
_08029374: .4byte gStaticData_08175558
_08029378: .4byte gUnknown_03001388
_0802937C:
	movs r0, #0
	str r0, [sp, #0xc]
	b _08029644
_08029382:
	movs r1, #2
	str r1, [sp, #0xc]
	b _08029644
_08029388:
	movs r3, #0
	str r3, [sp, #0xc]
	b _08029644
_0802938E:
	movs r5, #1
	str r5, [sp, #0xc]
	b _08029644
_08029394:
	ldr r1, _080294F4 @ =gStaticData_08175584
	adds r0, r2, r1
_08029398:
	ldr r0, [r0]
	mov sl, r0
	add r1, sp, #8
	movs r0, #0
	strh r0, [r1]
	ldr r6, _080294F8 @ =0x040000D4
	str r1, [r6]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	str r0, [r6, #4]
	ldr r0, _080294FC @ =0x81000200
	str r0, [r6, #8]
	ldr r0, [r6, #8]
	movs r0, #0x80
	movs r1, #2
	movs r2, #1
	bl sub_800132C
	ldr r4, _08029500 @ =gStaticData_08175558
	ldr r3, _08029504 @ =gUnknown_03001380
	mov sb, r3
	ldr r0, [r3]
	movs r5, #0x34
	mov r8, r5
	mov r3, r8
	muls r3, r0, r3
	adds r0, r3, r4
	ldr r0, [r0]
	adds r1, r4, #4
	adds r1, r3, r1
	ldr r1, [r1]
	adds r2, r4, #0
	adds r2, #8
	adds r3, r3, r2
	ldr r2, [r3]
	ldr r5, _08029508 @ =gUnknown_03000878
	ldr r3, [r5]
	bl sub_8029890
	mov r1, sb
	ldr r0, [r1]
	mov r3, r8
	muls r3, r0, r3
	adds r0, r3, #0
	ldr r5, _0802950C @ =gStaticData_08175564
	adds r0, r0, r5
	ldr r0, [r0]
	cmp r0, #0
	beq _080293FE
	bl sub_802F7B0
_080293FE:
	bl sub_802AB08
	mov r1, sb
	ldr r0, [r1]
	mov r2, r8
	muls r2, r0, r2
	adds r0, r2, r4
	ldr r7, [r0]
	adds r0, r4, #0
	adds r0, #0x14
	adds r0, r2, r0
	ldr r3, [r0]
	adds r0, r4, #0
	adds r0, #0x18
	adds r0, r2, r0
	ldr r5, [r0]
	movs r0, #0
	mov ip, r0
	adds r0, r4, #0
	adds r0, #0x24
	adds r2, r2, r0
	ldr r0, _08029510 @ =gUnknown_03001384
	ldr r1, [r0]
	ldr r0, [r2]
	cmp r1, r0
	blt _08029436
	movs r1, #1
	mov ip, r1
_08029436:
	mov r0, sl
	str r0, [sp]
	ldr r1, _08029508 @ =gUnknown_03000878
	ldr r0, [r1]
	str r0, [sp, #4]
	adds r0, r7, #0
	adds r1, r3, #0
	adds r2, r5, #0
	mov r3, ip
	bl SelectActorCategory
	mov r3, sb
	ldr r0, [r3]
	mov r5, r8
	muls r5, r0, r5
	adds r0, r5, #0
	adds r1, r4, #0
	adds r1, #0x10
	adds r0, r0, r1
	ldr r0, [r0]
	str r0, [r6]
	ldr r1, _08029514 @ =0x05000200
	str r1, [r6, #4]
	ldr r0, _08029518 @ =0x80000100
	str r0, [r6, #8]
	ldr r0, [r6, #8]
	ldr r7, _0802951C @ =gUnknown_030012C0
	ldr r3, _08029520 @ =gUnknown_03001300
	mov sb, r3
	mov sl, r4
	mov r8, r1
_08029474:
	ldr r0, _08029524 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	bl sub_8029B38
	bl sub_802A208
	adds r5, r0, #0
	ldr r1, [r7]
	adds r0, r1, #0
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _08029498
	adds r0, r1, #0
	bl sub_8022F2C
_08029498:
	ldr r0, _08029528 @ =gUnknown_030012FC
	ldr r0, [r0]
	bl sub_8006C4C
	mov r1, sb
	ldr r0, [r1]
	bl sub_8006A78
	ldr r4, _0802952C @ =gUnknown_03001318
	ldr r0, [r4]
	bl sub_8028400
	ldr r0, [r4]
	bl sub_80274EC
	bl sub_8028EA8
	bl sub_80006A8
	bl sub_8029E50
	mov r3, sb
	ldr r0, [r3]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	bl sub_8029ADC
	bl sub_802A650
	bl sub_8029090
	cmp r5, #0
	beq _08029574
	cmp r5, #1
	bne _080294E4
	b _0802937C
_080294E4:
	cmp r5, #2
	bne _08029530
	ldr r0, [r7]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _08029560
	b _08029550
	.align 2, 0
_080294F4: .4byte gStaticData_08175584
_080294F8: .4byte 0x040000D4
_080294FC: .4byte 0x81000200
_08029500: .4byte gStaticData_08175558
_08029504: .4byte gUnknown_03001380
_08029508: .4byte gUnknown_03000878
_0802950C: .4byte gStaticData_08175564
_08029510: .4byte gUnknown_03001384
_08029514: .4byte 0x05000200
_08029518: .4byte 0x80000100
_0802951C: .4byte gUnknown_030012C0
_08029520: .4byte gUnknown_03001300
_08029524: .4byte gUnknown_03001304
_08029528: .4byte gUnknown_030012FC
_0802952C: .4byte gUnknown_03001318
_08029530:
	cmp r5, #3
	beq _08029536
	b _08029644
_08029536:
	ldr r0, [r7]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _08029560
	ldr r0, _08029568 @ =gUnknown_03001380
	ldr r1, [r0]
	movs r0, #0x34
	muls r0, r1, r0
	add r0, sl
	ldr r0, [r0]
	cmp r0, #0
	beq _08029558
_08029550:
	ldr r5, _0802956C @ =gUnknown_03001388
	ldr r0, [r5]
	adds r0, #1
	str r0, [r5]
_08029558:
	ldr r1, _08029570 @ =gUnknown_03001384
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
_08029560:
	movs r3, #1
	str r3, [sp, #0xc]
	b _08029644
	.align 2, 0
_08029568: .4byte gUnknown_03001380
_0802956C: .4byte gUnknown_03001388
_08029570: .4byte gUnknown_03001384
_08029574:
	movs r4, #0
	bl sub_8001510
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08029598
	ldr r1, _08029630 @ =gUnknown_030007E0
	movs r0, #8
	ldrh r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _08029598
	bl sub_802A5AC
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	rsbs r0, r0, #0
	lsrs r4, r0, #0x1f
_08029598:
	cmp r4, #0
	beq _08029618
	movs r0, #0x80
	lsls r0, r0, #2
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	mov r5, r8
	str r5, [r6]
	str r4, [r6, #4]
	ldr r0, _08029634 @ =0x80000100
	str r0, [r6, #8]
	ldr r0, [r6, #8]
	bl sub_802907C
	bl sub_8028E88
	bl sub_8028DB8
	bl sub_8004D74
	adds r5, r0, #0
	bl SetupActorVramPool
	movs r0, #0x80
	movs r1, #1
	movs r2, #1
	bl sub_800132C
	str r4, [r6]
	mov r1, r8
	str r1, [r6, #4]
	ldr r3, _08029634 @ =0x80000100
	str r3, [r6, #8]
	ldr r0, [r6, #8]
	adds r0, r4, #0
	bl mem_free
	bl sub_802996C
	ldr r0, _08029638 @ =gUnknown_03001380
	ldr r1, [r0]
	movs r0, #0x34
	muls r0, r1, r0
	ldr r1, _0802963C @ =gStaticData_08175564
	adds r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08029602
	bl sub_802F7B0
_08029602:
	bl sub_802A5C4
	cmp r5, #2
	bne _0802960C
	b _08029382
_0802960C:
	cmp r5, #3
	bne _08029612
	b _08029388
_08029612:
	cmp r5, #1
	bne _08029618
	b _0802938E
_08029618:
	ldr r0, _08029630 @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #4
	ands r0, r1
	cmp r0, #0
	bne _08029626
	b _08029474
_08029626:
	ldr r0, _08029640 @ =gUnknown_03001318
	ldr r0, [r0]
	bl sub_8028504
	b _08029474
	.align 2, 0
_08029630: .4byte gUnknown_030007E0
_08029634: .4byte 0x80000100
_08029638: .4byte gUnknown_03001380
_0802963C: .4byte gStaticData_08175564
_08029640: .4byte gUnknown_03001318
_08029644:
	bl sub_802A5E4
	bl nullsub_5
	ldr r3, [sp, #0xc]
	cmp r3, #1
	bne _08029654
	b _08029318
_08029654:
	bl nullsub_6
	bl sub_802907C
	bl sub_8028E88
	bl sub_8028DB8
	bl sub_8029168
	movs r1, #0x80
	lsls r1, r1, #0x13
	movs r0, #0x41
	strh r0, [r1]
	movs r1, #0xa0
	lsls r1, r1, #0x13
	movs r0, #0
	strh r0, [r1]
	ldr r0, [sp, #0xc]
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_802968C
sub_802968C: @ 0x0802968C
	push {r4, r5, lr}
	movs r4, #0
	ldr r2, _080296EC @ =gStaticData_08175558
	movs r1, #0x34
	muls r1, r0, r1
	adds r0, r2, #0
	adds r0, #0x14
	adds r0, r1, r0
	ldr r3, [r0]
	adds r1, r1, r2
	ldr r0, [r1]
	cmp r0, #0
	bne _080296F0
	movs r5, #0
	ldr r2, [r3, #4]
	cmp r4, r2
	bge _08029718
	adds r0, r3, #0
_080296B0:
	ldrb r1, [r0, #8]
	cmp r1, #1
	beq _080296DE
	cmp r1, #3
	beq _080296DE
	cmp r1, #4
	beq _080296DE
	cmp r1, #8
	beq _080296DE
	cmp r1, #9
	beq _080296DE
	cmp r1, #0xa
	beq _080296DE
	cmp r1, #0x1c
	beq _080296DE
	cmp r1, #0x1d
	beq _080296DE
	cmp r1, #0x1e
	beq _080296DE
	cmp r1, #0x1f
	beq _080296DE
	cmp r1, #0x23
	bne _080296E0
_080296DE:
	adds r4, #1
_080296E0:
	adds r0, #0x14
	adds r5, #1
	cmp r5, r2
	blt _080296B0
	b _08029718
	.align 2, 0
_080296EC: .4byte gStaticData_08175558
_080296F0:
	ldr r2, [r3, #4]
	cmp r4, r2
	bge _08029718
_080296F6:
	ldrb r1, [r3, #8]
	adds r0, r1, #0
	subs r0, #0x13
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #4
	bls _0802970E
	adds r0, r1, #0
	cmp r0, #0x1b
	beq _0802970E
	cmp r0, #0x1e
	bne _08029710
_0802970E:
	adds r4, #1
_08029710:
	adds r3, #0x14
	subs r2, #1
	cmp r2, #0
	bne _080296F6
_08029718:
	adds r0, r4, #0
	pop {r4, r5}
	pop {r1}
	bx r1

	thumb_func_start sub_8029720
sub_8029720: @ 0x08029720
	ldr r1, _0802972C @ =gUnknown_0300138C
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
	bx lr
	.align 2, 0
_0802972C: .4byte gUnknown_0300138C

	thumb_func_start sub_8029730
sub_8029730: @ 0x08029730
	ldr r0, _08029738 @ =gUnknown_0300138C
	ldr r0, [r0]
	bx lr
	.align 2, 0
_08029738: .4byte gUnknown_0300138C

	thumb_func_start sub_802973C
sub_802973C: @ 0x0802973C
	ldr r0, _08029744 @ =gUnknown_03000878
	ldr r0, [r0]
	bx lr
	.align 2, 0
_08029744: .4byte gUnknown_03000878

	thumb_func_start sub_8029748
sub_8029748: @ 0x08029748
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r5, _0802977C @ =gUnknown_03000878
	bl sub_802A4E0
	subs r4, r4, r0
	str r4, [r5]
	ldr r0, _08029780 @ =gUnknown_03001384
	movs r1, #0
	str r1, [r0]
	ldr r0, _08029784 @ =gUnknown_03001388
	str r1, [r0]
	ldr r0, _08029788 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022CA0
	ldr r1, _0802978C @ =gUnknown_03001390
	ldr r0, _08029790 @ =gUnknown_0300138C
	ldr r0, [r0]
	str r0, [r1]
	bl sub_802AB34
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802977C: .4byte gUnknown_03000878
_08029780: .4byte gUnknown_03001384
_08029784: .4byte gUnknown_03001388
_08029788: .4byte gUnknown_030012C0
_0802978C: .4byte gUnknown_03001390
_08029790: .4byte gUnknown_0300138C

	thumb_func_start sub_8029794
sub_8029794: @ 0x08029794
	push {r4, lr}
	movs r4, #0
	ldr r3, _080297BC @ =gUnknown_03001384
	ldr r1, _080297C0 @ =gStaticData_08175558
	ldr r0, _080297C4 @ =gUnknown_03001380
	ldr r2, [r0]
	movs r0, #0x34
	muls r0, r2, r0
	adds r1, #0x20
	adds r0, r0, r1
	ldr r1, [r3]
	ldr r0, [r0]
	cmp r1, r0
	blt _080297B2
	movs r4, #1
_080297B2:
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_080297BC: .4byte gUnknown_03001384
_080297C0: .4byte gStaticData_08175558
_080297C4: .4byte gUnknown_03001380

	thumb_func_start sub_80297C8
sub_80297C8: @ 0x080297C8
	push {r4, r5, r6, lr}
	ldr r2, _08029814 @ =gUnknown_03001394
	ldr r0, _08029818 @ =gUnknown_030013B0
	ldr r0, [r0]
	asrs r0, r0, #8
	ldr r1, _0802981C @ =gUnknown_030013A4
	ldr r1, [r1]
	muls r0, r1, r0
	movs r1, #0x81
	lsls r1, r1, #2
	adds r0, r0, r1
	ldr r1, [r2]
	adds r6, r1, r0
	ldr r0, _08029820 @ =gUnknown_030013B8
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802983C
	ldr r1, _08029824 @ =gUnknown_030013B9
	ldrb r0, [r1]
	ldr r5, _08029828 @ =0x06002000
	cmp r0, #0
	beq _080297F8
	movs r5, #0xc0
	lsls r5, r5, #0x13
_080297F8:
	ldr r0, _0802982C @ =gUnknown_030013A0
	ldr r0, [r0]
	adds r0, r6, r0
	ldr r4, _08029830 @ =gUnknown_0300087C
	ldrb r1, [r1]
	ldr r2, _08029834 @ =gUnknown_03001398
	ldr r2, [r2]
	ldr r3, _08029838 @ =gUnknown_0300139C
	ldr r3, [r3]
	ldr r4, [r4]
	bl sub_803AD88
	b _08029858
	.align 2, 0
_08029814: .4byte gUnknown_03001394
_08029818: .4byte gUnknown_030013B0
_0802981C: .4byte gUnknown_030013A4
_08029820: .4byte gUnknown_030013B8
_08029824: .4byte gUnknown_030013B9
_08029828: .4byte 0x06002000
_0802982C: .4byte gUnknown_030013A0
_08029830: .4byte gUnknown_0300087C
_08029834: .4byte gUnknown_03001398
_08029838: .4byte gUnknown_0300139C
_0802983C:
	ldr r0, _08029848 @ =gUnknown_030013B9
	ldrb r0, [r0]
	cmp r0, #0
	beq _08029850
	ldr r5, _0802984C @ =0x06000020
	b _08029858
	.align 2, 0
_08029848: .4byte gUnknown_030013B9
_0802984C: .4byte 0x06000020
_08029850:
	ldr r0, _08029880 @ =gUnknown_030013A0
	ldr r0, [r0]
	ldr r1, _08029884 @ =0x06000020
	adds r5, r0, r1
_08029858:
	ldr r2, _08029888 @ =0x040000D4
	str r6, [r2]
	str r5, [r2, #4]
	ldr r0, _08029880 @ =gUnknown_030013A0
	ldr r0, [r0]
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	movs r1, #0x80
	lsls r1, r1, #0x18
	orrs r0, r1
	str r0, [r2, #8]
	ldr r0, [r2, #8]
	ldr r1, _0802988C @ =gUnknown_030013BA
	movs r0, #1
	strb r0, [r1]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08029880: .4byte gUnknown_030013A0
_08029884: .4byte 0x06000020
_08029888: .4byte 0x040000D4
_0802988C: .4byte gUnknown_030013BA

	thumb_func_start sub_8029890
sub_8029890: @ 0x08029890
	push {r4, r5, r6, r7, lr}
	adds r4, r1, #0
	adds r6, r2, #0
	adds r7, r3, #0
	ldr r1, _08029914 @ =gUnknown_030013B8
	movs r5, #0
	cmp r0, #0
	bne _080298A2
	movs r5, #1
_080298A2:
	strb r5, [r1]
	ldr r0, _08029918 @ =gUnknown_03001394
	str r4, [r0]
	ldr r1, _0802991C @ =gUnknown_03001398
	movs r2, #0x80
	lsls r2, r2, #2
	adds r0, r4, r2
	movs r3, #0
	ldrsh r2, [r0, r3]
	str r2, [r1]
	ldr r1, _08029920 @ =gUnknown_0300139C
	ldr r3, _08029924 @ =0x00000202
	adds r0, r4, r3
	movs r3, #0
	ldrsh r0, [r0, r3]
	str r0, [r1]
	ldr r1, _08029928 @ =gUnknown_030013A0
	muls r2, r0, r2
	lsls r0, r2, #5
	str r0, [r1]
	ldr r4, _0802992C @ =gUnknown_030013A4
	adds r3, r0, #0
	adds r1, r4, #0
	cmp r5, #0
	beq _080298E2
	adds r0, r2, #7
	cmp r0, #0
	bge _080298DC
	adds r0, #7
_080298DC:
	asrs r0, r0, #3
	lsls r0, r0, #2
	adds r3, r3, r0
_080298E2:
	str r3, [r4]
	ldr r4, _08029930 @ =gUnknown_030013AC
	ldr r2, _08029934 @ =0xFFFFFDFC
	adds r0, r6, r2
	ldr r1, [r1]
	bl sub_803ADB4
	lsls r0, r0, #8
	str r0, [r4]
	ldr r0, _08029938 @ =gUnknown_030013B0
	movs r4, #0
	str r4, [r0]
	bl sub_802996C
	ldr r0, _0802993C @ =gUnknown_030013B4
	str r4, [r0]
	ldr r0, _08029914 @ =gUnknown_030013B8
	ldrb r0, [r0]
	cmp r0, #0
	bne _08029948
	ldr r1, _08029940 @ =gUnknown_030013A8
	ldr r0, _08029944 @ =gUnknown_030013C8
	ldr r0, [r0]
	subs r0, r7, r0
	b _08029950
	.align 2, 0
_08029914: .4byte gUnknown_030013B8
_08029918: .4byte gUnknown_03001394
_0802991C: .4byte gUnknown_03001398
_08029920: .4byte gUnknown_0300139C
_08029924: .4byte 0x00000202
_08029928: .4byte gUnknown_030013A0
_0802992C: .4byte gUnknown_030013A4
_08029930: .4byte gUnknown_030013AC
_08029934: .4byte 0xFFFFFDFC
_08029938: .4byte gUnknown_030013B0
_0802993C: .4byte gUnknown_030013B4
_08029940: .4byte gUnknown_030013A8
_08029944: .4byte gUnknown_030013C8
_08029948:
	ldr r1, _08029960 @ =gUnknown_030013A8
	ldr r0, _08029964 @ =gUnknown_030013C8
	ldr r0, [r0]
	adds r0, r7, r0
_08029950:
	asrs r0, r0, #8
	str r0, [r1]
	ldr r1, _08029968 @ =gUnknown_030013BC
	movs r0, #0
	str r0, [r1]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08029960: .4byte gUnknown_030013A8
_08029964: .4byte gUnknown_030013C8
_08029968: .4byte gUnknown_030013BC

	thumb_func_start sub_802996C
sub_802996C: @ 0x0802996C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	movs r1, #0x80
	lsls r1, r1, #0x13
	ldr r2, _08029A0C @ =0x00001141
	adds r0, r2, #0
	strh r0, [r1]
	adds r1, #8
	ldr r2, _08029A10 @ =0x00005C02
	adds r0, r2, #0
	strh r0, [r1]
	adds r1, #0xcc
	ldr r0, _08029A14 @ =gUnknown_03001394
	ldr r0, [r0]
	str r0, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	str r0, [r1, #4]
	ldr r0, _08029A18 @ =0x80000100
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r0, _08029A1C @ =gUnknown_030013B8
	ldrb r0, [r0]
	cmp r0, #0
	beq _080299A8
	b _08029AA2
_080299A8:
	movs r1, #0xc0
	lsls r1, r1, #0x13
	ldr r0, _08029A20 @ =gUnknown_03001398
	mov sb, r0
	ldr r2, _08029A24 @ =gUnknown_0300139C
	mov sl, r2
	movs r2, #0
	adds r0, r1, #0
	adds r0, #0x1c
_080299BA:
	str r2, [r0]
	subs r0, #4
	cmp r0, r1
	bge _080299BA
	mov r1, sp
	movs r0, #0
	strh r0, [r1]
	ldr r1, _08029A28 @ =0x040000D4
	mov r0, sp
	str r0, [r1]
	ldr r0, _08029A2C @ =0x0600E000
	str r0, [r1, #4]
	ldr r0, _08029A30 @ =0x81001000
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	mov r1, sb
	ldr r7, [r1]
	mov r2, sl
	ldr r2, [r2]
	mov ip, r2
	ldr r4, _08029A34 @ =0x0600E400
	movs r3, #1
	movs r0, #0
	cmp r0, ip
	bge _08029A4E
	movs r1, #0xf8
	lsls r1, r1, #3
	mov r8, r1
_080299F2:
	movs r2, #0
	adds r5, r4, #0
	adds r5, #0x40
	adds r6, r0, #1
	cmp r2, r7
	bge _08029A46
	mov r0, r8
	adds r1, r4, r0
	adds r0, r4, #0
_08029A04:
	cmp r2, #0x1f
	bgt _08029A38
	strh r3, [r0]
	b _08029A3A
	.align 2, 0
_08029A0C: .4byte 0x00001141
_08029A10: .4byte 0x00005C02
_08029A14: .4byte gUnknown_03001394
_08029A18: .4byte 0x80000100
_08029A1C: .4byte gUnknown_030013B8
_08029A20: .4byte gUnknown_03001398
_08029A24: .4byte gUnknown_0300139C
_08029A28: .4byte 0x040000D4
_08029A2C: .4byte 0x0600E000
_08029A30: .4byte 0x81001000
_08029A34: .4byte 0x0600E400
_08029A38:
	strh r3, [r1]
_08029A3A:
	adds r3, #1
	adds r1, #2
	adds r0, #2
	adds r2, #1
	cmp r2, r7
	blt _08029A04
_08029A46:
	adds r4, r5, #0
	adds r0, r6, #0
	cmp r0, ip
	blt _080299F2
_08029A4E:
	mov r1, sb
	ldr r5, [r1]
	mov r2, sl
	ldr r2, [r2]
	mov ip, r2
	ldr r4, _08029A88 @ =0x0600F400
	mov r0, ip
	muls r0, r5, r0
	adds r0, #1
	movs r1, #0
	cmp r1, ip
	bge _08029AA2
	movs r2, #0xf8
	lsls r2, r2, #3
	mov r8, r2
_08029A6C:
	movs r3, #0
	adds r7, r4, #0
	adds r7, #0x40
	adds r6, r1, #1
	cmp r3, r5
	bge _08029A9A
	mov r1, r8
	adds r2, r4, r1
	adds r1, r4, #0
_08029A7E:
	cmp r3, #0x1f
	bgt _08029A8C
	strh r0, [r1]
	b _08029A8E
	.align 2, 0
_08029A88: .4byte 0x0600F400
_08029A8C:
	strh r0, [r2]
_08029A8E:
	adds r0, #1
	adds r2, #2
	adds r1, #2
	adds r3, #1
	cmp r3, r5
	blt _08029A7E
_08029A9A:
	adds r4, r7, #0
	adds r1, r6, #0
	cmp r1, ip
	blt _08029A6C
_08029AA2:
	movs r0, #1
	ldr r2, _08029ABC @ =gUnknown_030013B9
	strb r0, [r2]
	bl sub_80297C8
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08029ABC: .4byte gUnknown_030013B9

	thumb_func_start nullsub_5
nullsub_5: @ 0x08029AC0
	bx lr
	.align 2, 0

	thumb_func_start sub_8029AC4
sub_8029AC4: @ 0x08029AC4
	ldr r0, _08029AD4 @ =gUnknown_03001398
	ldr r1, _08029AD8 @ =gUnknown_0300139C
	ldr r2, [r0]
	ldr r0, [r1]
	muls r0, r2, r0
	lsls r0, r0, #1
	adds r0, #1
	bx lr
	.align 2, 0
_08029AD4: .4byte gUnknown_03001398
_08029AD8: .4byte gUnknown_0300139C

	thumb_func_start sub_8029ADC
sub_8029ADC: @ 0x08029ADC
	push {r4, lr}
	ldr r0, _08029AF8 @ =gUnknown_030013BA
	ldrb r1, [r0]
	adds r3, r0, #0
	cmp r1, #0
	beq _08029B1C
	ldr r0, _08029AFC @ =gUnknown_030013B9
	ldrb r1, [r0]
	adds r2, r0, #0
	cmp r1, #0
	beq _08029B08
	ldr r1, _08029B00 @ =0x04000008
	ldr r4, _08029B04 @ =0x00005C02
	b _08029B0C
	.align 2, 0
_08029AF8: .4byte gUnknown_030013BA
_08029AFC: .4byte gUnknown_030013B9
_08029B00: .4byte 0x04000008
_08029B04: .4byte 0x00005C02
_08029B08:
	ldr r1, _08029B24 @ =0x04000008
	ldr r4, _08029B28 @ =0x00005E02
_08029B0C:
	adds r0, r4, #0
	strh r0, [r1]
	movs r0, #0
	strb r0, [r3]
	movs r0, #1
	ldrb r1, [r2]
	eors r0, r1
	strb r0, [r2]
_08029B1C:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08029B24: .4byte 0x04000008
_08029B28: .4byte 0x00005E02

	thumb_func_start sub_8029B2C
sub_8029B2C: @ 0x08029B2C
	ldr r0, _08029B34 @ =gUnknown_030013A8
	ldr r0, [r0]
	bx lr
	.align 2, 0
_08029B34: .4byte gUnknown_030013A8

	thumb_func_start sub_8029B38
sub_8029B38: @ 0x08029B38
	push {r4, lr}
	ldr r4, _08029B78 @ =gUnknown_030013B0
	ldr r1, [r4]
	asrs r2, r1, #8
	ldr r0, _08029B7C @ =gUnknown_030013B4
	ldr r0, [r0]
	adds r3, r1, r0
	str r3, [r4]
	ldr r1, _08029B80 @ =gUnknown_030013BC
	asrs r0, r3, #8
	subs r2, r0, r2
	str r2, [r1]
	ldr r0, _08029B84 @ =gUnknown_030013AC
	ldr r0, [r0]
	cmp r3, r0
	blt _08029B5C
	subs r0, r3, r0
	str r0, [r4]
_08029B5C:
	cmp r2, #0
	beq _08029B70
	ldr r1, _08029B88 @ =gUnknown_030013A8
	ldr r0, [r1]
	adds r0, r0, r2
	str r0, [r1]
	bl sub_80297C8
	bl sub_802AB58
_08029B70:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08029B78: .4byte gUnknown_030013B0
_08029B7C: .4byte gUnknown_030013B4
_08029B80: .4byte gUnknown_030013BC
_08029B84: .4byte gUnknown_030013AC
_08029B88: .4byte gUnknown_030013A8

	thumb_func_start sub_8029B8C
sub_8029B8C: @ 0x08029B8C
	ldr r0, _08029B94 @ =gUnknown_030013BC
	ldr r0, [r0]
	bx lr
	.align 2, 0
_08029B94: .4byte gUnknown_030013BC

	thumb_func_start sub_8029B98
sub_8029B98: @ 0x08029B98
	ldr r0, _08029BA8 @ =gUnknown_030013B4
	ldr r1, [r0]
	lsls r0, r1, #4
	subs r0, r0, r1
	lsls r0, r0, #2
	asrs r0, r0, #8
	bx lr
	.align 2, 0
_08029BA8: .4byte gUnknown_030013B4

	thumb_func_start sub_8029BAC
sub_8029BAC: @ 0x08029BAC
	push {r4, lr}
	ldr r4, _08029BC0 @ =gUnknown_030013B4
	lsls r0, r0, #8
	movs r1, #0x3c
	bl sub_803ADB4
	str r0, [r4]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08029BC0: .4byte gUnknown_030013B4

	thumb_func_start sub_8029BC4
sub_8029BC4: @ 0x08029BC4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r1, #0
	mov ip, r2
	ldr r4, _08029BE0 @ =0x0600E400
	cmp r0, #0
	beq _08029BE8
	ldr r4, _08029BE4 @ =0x0600F400
	mov r0, ip
	muls r0, r7, r0
	adds r0, #1
	b _08029BEA
	.align 2, 0
_08029BE0: .4byte 0x0600E400
_08029BE4: .4byte 0x0600F400
_08029BE8:
	movs r0, #1
_08029BEA:
	movs r3, #0
	cmp r3, ip
	bge _08029C26
	movs r1, #0xf8
	lsls r1, r1, #3
	mov r8, r1
_08029BF6:
	movs r2, #0
	adds r6, r4, #0
	adds r6, #0x40
	adds r5, r3, #1
	cmp r2, r7
	bge _08029C1E
	mov r3, r8
	adds r1, r4, r3
	adds r3, r4, #0
_08029C08:
	cmp r2, #0x1f
	bgt _08029C10
	strh r0, [r3]
	b _08029C12
_08029C10:
	strh r0, [r1]
_08029C12:
	adds r0, #1
	adds r1, #2
	adds r3, #2
	adds r2, #1
	cmp r2, r7
	blt _08029C08
_08029C1E:
	adds r4, r6, #0
	adds r3, r5, #0
	cmp r3, ip
	blt _08029BF6
_08029C26:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8029C30
sub_8029C30: @ 0x08029C30
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	ldr r0, _08029C84 @ =gUnknown_030013DC
	str r5, [r0]
	cmp r5, #0
	bne _08029CAC
	ldr r1, _08029C88 @ =gUnknown_030013C0
	movs r0, #0x88
	lsls r0, r0, #5
	str r0, [r1]
	ldr r4, _08029C8C @ =gUnknown_030013C4
	movs r0, #0xa0
	lsls r0, r0, #8
	str r0, [r4]
	ldr r1, _08029C90 @ =gUnknown_030013C8
	movs r0, #0xbc
	lsls r0, r0, #6
	str r0, [r1]
	ldr r3, _08029C94 @ =gUnknown_030013F8
	movs r0, #0x98
	lsls r0, r0, #9
	str r0, [r3]
	ldr r2, _08029C98 @ =gUnknown_030013FC
	movs r0, #0xd0
	lsls r0, r0, #8
	str r0, [r2]
	ldr r1, _08029C9C @ =gUnknown_030013E0
	movs r0, #2
	str r0, [r1]
	ldr r1, _08029CA0 @ =gUnknown_030013E4
	movs r0, #0x64
	str r0, [r1]
	ldr r1, _08029CA4 @ =gUnknown_030013E8
	movs r0, #0x51
	str r0, [r1]
	ldr r0, _08029CA8 @ =gUnknown_030013F4
	str r5, [r0]
	adds r6, r4, #0
	adds r5, r2, #0
	adds r4, r0, #0
	b _08029CF2
	.align 2, 0
_08029C84: .4byte gUnknown_030013DC
_08029C88: .4byte gUnknown_030013C0
_08029C8C: .4byte gUnknown_030013C4
_08029C90: .4byte gUnknown_030013C8
_08029C94: .4byte gUnknown_030013F8
_08029C98: .4byte gUnknown_030013FC
_08029C9C: .4byte gUnknown_030013E0
_08029CA0: .4byte gUnknown_030013E4
_08029CA4: .4byte gUnknown_030013E8
_08029CA8: .4byte gUnknown_030013F4
_08029CAC:
	ldr r1, _08029D3C @ =gUnknown_030013C0
	movs r0, #0xd0
	lsls r0, r0, #5
	str r0, [r1]
	ldr r4, _08029D40 @ =gUnknown_030013C4
	movs r0, #0xaa
	lsls r0, r0, #8
	str r0, [r4]
	ldr r1, _08029D44 @ =gUnknown_030013C8
	movs r0, #0xe0
	lsls r0, r0, #5
	str r0, [r1]
	ldr r3, _08029D48 @ =gUnknown_030013F8
	movs r0, #0x98
	lsls r0, r0, #9
	str r0, [r3]
	ldr r2, _08029D4C @ =gUnknown_030013FC
	movs r0, #0xce
	lsls r0, r0, #8
	str r0, [r2]
	ldr r1, _08029D50 @ =gUnknown_030013E0
	movs r0, #3
	str r0, [r1]
	ldr r1, _08029D54 @ =gUnknown_030013E4
	adds r0, #0xfd
	str r0, [r1]
	ldr r1, _08029D58 @ =gUnknown_030013E8
	movs r0, #0x96
	str r0, [r1]
	ldr r1, _08029D5C @ =gUnknown_030013F4
	movs r0, #2
	str r0, [r1]
	adds r6, r4, #0
	adds r5, r2, #0
	adds r4, r1, #0
_08029CF2:
	ldr r0, _08029D60 @ =gUnknown_030013EC
	ldr r1, [r3]
	ldr r2, _08029D64 @ =0xFFFF1000
	adds r1, r1, r2
	str r1, [r0]
	ldr r2, _08029D68 @ =gUnknown_030013F0
	ldr r0, [r5]
	ldr r3, _08029D6C @ =0xFFFF6000
	adds r0, r0, r3
	str r0, [r2]
	ldr r2, _08029D70 @ =gUnknown_030013D0
	lsrs r0, r1, #0x1f
	adds r1, r1, r0
	asrs r0, r1, #1
	str r0, [r2]
	ldr r0, _08029D74 @ =gUnknown_030013CC
	movs r3, #0
	str r3, [r0]
	ldr r0, _08029D78 @ =0x04000010
	asrs r1, r1, #9
	strh r1, [r0]
	ldr r2, _08029D7C @ =0x04000012
	ldr r0, [r4]
	strh r0, [r2]
	ldr r0, _08029D80 @ =0x04000014
	strh r1, [r0]
	adds r0, #2
	strh r3, [r0]
	ldr r0, _08029D84 @ =gUnknown_030013D4
	str r3, [r0]
	ldr r1, _08029D88 @ =gUnknown_030013D8
	ldr r0, [r6]
	str r0, [r1]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08029D3C: .4byte gUnknown_030013C0
_08029D40: .4byte gUnknown_030013C4
_08029D44: .4byte gUnknown_030013C8
_08029D48: .4byte gUnknown_030013F8
_08029D4C: .4byte gUnknown_030013FC
_08029D50: .4byte gUnknown_030013E0
_08029D54: .4byte gUnknown_030013E4
_08029D58: .4byte gUnknown_030013E8
_08029D5C: .4byte gUnknown_030013F4
_08029D60: .4byte gUnknown_030013EC
_08029D64: .4byte 0xFFFF1000
_08029D68: .4byte gUnknown_030013F0
_08029D6C: .4byte 0xFFFF6000
_08029D70: .4byte gUnknown_030013D0
_08029D74: .4byte gUnknown_030013CC
_08029D78: .4byte 0x04000010
_08029D7C: .4byte 0x04000012
_08029D80: .4byte 0x04000014
_08029D84: .4byte gUnknown_030013D4
_08029D88: .4byte gUnknown_030013D8

	thumb_func_start sub_8029D8C
sub_8029D8C: @ 0x08029D8C
	push {r4, r5, r6, lr}
	adds r6, r1, #0
	ldr r1, _08029E08 @ =gUnknown_030013EC
	ldr r4, [r1]
	asrs r1, r4, #8
	muls r0, r1, r0
	ldr r1, _08029E0C @ =gUnknown_030013E4
	ldr r1, [r1]
	bl sub_803ADB4
	lsrs r1, r4, #0x1f
	adds r1, r4, r1
	asrs r1, r1, #1
	adds r0, r0, r1
	ldr r3, _08029E10 @ =gUnknown_030013D0
	ldr r2, [r3]
	subs r0, r0, r2
	ldr r1, _08029E14 @ =gUnknown_030013E0
	ldr r5, [r1]
	asrs r0, r5
	adds r2, r2, r0
	str r2, [r3]
	cmp r2, #0
	bge _08029DBE
	movs r2, #0
_08029DBE:
	adds r1, r4, #0
	cmp r1, r2
	ble _08029DC6
	adds r1, r2, #0
_08029DC6:
	str r1, [r3]
	ldr r0, _08029E18 @ =gUnknown_030013F0
	ldr r4, [r0]
	asrs r0, r4, #8
	muls r0, r6, r0
	ldr r1, _08029E1C @ =gUnknown_030013E8
	ldr r1, [r1]
	bl sub_803ADB4
	lsrs r1, r4, #0x1f
	adds r1, r4, r1
	asrs r1, r1, #1
	adds r0, r0, r1
	ldr r3, _08029E20 @ =gUnknown_030013CC
	ldr r2, [r3]
	subs r0, r0, r2
	asrs r0, r5
	ldr r1, _08029E24 @ =gUnknown_030013D4
	ldr r1, [r1]
	subs r0, r0, r1
	adds r2, r2, r0
	str r2, [r3]
	cmp r2, #0
	bge _08029DF8
	movs r2, #0
_08029DF8:
	adds r0, r4, #0
	cmp r0, r2
	ble _08029E00
	adds r0, r2, #0
_08029E00:
	str r0, [r3]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08029E08: .4byte gUnknown_030013EC
_08029E0C: .4byte gUnknown_030013E4
_08029E10: .4byte gUnknown_030013D0
_08029E14: .4byte gUnknown_030013E0
_08029E18: .4byte gUnknown_030013F0
_08029E1C: .4byte gUnknown_030013E8
_08029E20: .4byte gUnknown_030013CC
_08029E24: .4byte gUnknown_030013D4

	thumb_func_start sub_8029E28
sub_8029E28: @ 0x08029E28
	ldr r1, _08029E30 @ =gUnknown_030013D4
	str r0, [r1]
	bx lr
	.align 2, 0
_08029E30: .4byte gUnknown_030013D4

	thumb_func_start sub_8029E34
sub_8029E34: @ 0x08029E34
	ldr r1, _08029E3C @ =gUnknown_030013D8
	str r0, [r1]
	bx lr
	.align 2, 0
_08029E3C: .4byte gUnknown_030013D8

	thumb_func_start sub_8029E40
sub_8029E40: @ 0x08029E40
	ldr r0, _08029E48 @ =gUnknown_030013D8
	ldr r0, [r0]
	bx lr
	.align 2, 0
_08029E48: .4byte gUnknown_030013D8

	thumb_func_start nullsub_6
nullsub_6: @ 0x08029E4C
	bx lr
	.align 2, 0

	thumb_func_start sub_8029E50
sub_8029E50: @ 0x08029E50
	ldr r1, _08029E7C @ =0x04000010
	ldr r0, _08029E80 @ =gUnknown_030013D0
	ldr r2, [r0]
	asrs r2, r2, #8
	strh r2, [r1]
	ldr r3, _08029E84 @ =0x04000012
	ldr r0, _08029E88 @ =gUnknown_030013CC
	ldr r1, [r0]
	asrs r1, r1, #8
	ldr r0, _08029E8C @ =gUnknown_030013F4
	ldr r0, [r0]
	adds r0, r0, r1
	strh r0, [r3]
	ldr r0, _08029E90 @ =0x04000014
	strh r2, [r0]
	adds r0, #2
	strh r1, [r0]
	ldr r1, _08029E94 @ =gUnknown_030013D4
	movs r0, #0
	str r0, [r1]
	bx lr
	.align 2, 0
_08029E7C: .4byte 0x04000010
_08029E80: .4byte gUnknown_030013D0
_08029E84: .4byte 0x04000012
_08029E88: .4byte gUnknown_030013CC
_08029E8C: .4byte gUnknown_030013F4
_08029E90: .4byte 0x04000014
_08029E94: .4byte gUnknown_030013D4

	thumb_func_start sub_8029E98
sub_8029E98: @ 0x08029E98
	ldr r0, _08029EAC @ =gUnknown_030013FC
	ldr r0, [r0]
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	ldr r1, _08029EB0 @ =gUnknown_030013CC
	ldr r1, [r1]
	subs r0, r0, r1
	bx lr
	.align 2, 0
_08029EAC: .4byte gUnknown_030013FC
_08029EB0: .4byte gUnknown_030013CC

	thumb_func_start sub_8029EB4
sub_8029EB4: @ 0x08029EB4
	ldr r0, _08029EC8 @ =gUnknown_030013F8
	ldr r0, [r0]
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	ldr r1, _08029ECC @ =gUnknown_030013D0
	ldr r1, [r1]
	subs r0, r0, r1
	bx lr
	.align 2, 0
_08029EC8: .4byte gUnknown_030013F8
_08029ECC: .4byte gUnknown_030013D0

	thumb_func_start SelectActorCategory
SelectActorCategory: @ 0x08029ED0
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	ldr r6, [sp, #0x20]
	ldr r4, _08029FF4 @ =gUnknown_03001418
	mov sb, r4
	movs r4, #0x34
	muls r4, r0, r4
	ldr r0, _08029FF8 @ =gStaticData_081756C4
	adds r4, r4, r0
	mov r0, sb
	str r4, [r0]
	ldr r0, _08029FFC @ =gUnknown_03001414
	strb r3, [r0]
	ldr r5, _0802A000 @ =gUnknown_03001400
	str r1, [r5]
	ldr r7, _0802A004 @ =gUnknown_03001404
	movs r1, #0
	str r1, [r7]
	ldr r0, _0802A008 @ =gUnknown_0300141C
	strb r1, [r0]
	ldr r0, _0802A00C @ =gUnknown_03001420
	str r1, [r0]
	ldr r3, [r4]
	adds r0, r2, #0
	adds r1, r6, #0
	bl sub_803AD84
	bl sub_8029B2C
	mov r8, r0
	ldr r5, [r5]
	ldr r1, [r7]
	ldr r0, [r5, #4]
	cmp r1, r0
	bge _08029F58
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r2, r5, #0
	adds r2, #0x14
	adds r0, r2, r0
	mov r3, sb
	ldr r1, [r3]
	ldr r1, [r1, #0x20]
	add r1, r8
	ldr r0, [r0]
	cmp r0, r1
	bge _08029F58
	adds r3, r7, #0
	mov r4, sb
_08029F38:
	ldr r0, [r3]
	adds r1, r0, #1
	str r1, [r3]
	ldr r0, [r5, #4]
	cmp r1, r0
	bge _08029F58
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r2, r0
	ldr r1, [r4]
	ldr r1, [r1, #0x20]
	add r1, r8
	ldr r0, [r0]
	cmp r0, r1
	blt _08029F38
_08029F58:
	ldr r4, _0802A010 @ =gUnknown_03001408
	movs r1, #0x80
	lsls r1, r1, #0x18
	movs r0, #0xc8
	bl mem_alloc
	str r0, [r4]
	ldr r4, _08029FF4 @ =gUnknown_03001418
	ldr r0, [r4]
	ldr r1, [r0, #8]
	cmp r1, #0
	beq _08029F76
	ldr r0, [sp, #0x1c]
	bl sub_803AD7C
_08029F76:
	ldr r7, _0802A004 @ =gUnknown_03001404
	ldr r3, _0802A000 @ =gUnknown_03001400
	ldr r1, [r3]
	ldr r2, [r7]
	ldr r0, [r1, #4]
	cmp r2, r0
	bge _08029FE0
	lsls r0, r2, #2
	adds r0, r0, r2
	lsls r0, r0, #2
	adds r1, #0x14
	adds r1, r1, r0
	ldr r0, [r4]
	ldr r0, [r0, #0x1c]
	add r0, r8
	ldr r1, [r1]
	cmp r1, r0
	bgt _08029FE0
	adds r6, r4, #0
	adds r5, r3, #0
	adds r4, r7, #0
_08029FA0:
	ldr r2, [r6]
	ldr r0, [r4]
	lsls r1, r0, #2
	adds r1, r1, r0
	lsls r1, r1, #2
	adds r1, #8
	ldr r0, [r5]
	adds r0, r0, r1
	ldr r1, _08029FFC @ =gUnknown_03001414
	ldrb r1, [r1]
	ldr r3, [r2, #4]
	movs r2, #0
	bl sub_803AD84
	ldr r0, [r4]
	adds r2, r0, #1
	str r2, [r4]
	ldr r1, [r5]
	ldr r0, [r1, #4]
	cmp r2, r0
	bge _08029FE0
	lsls r0, r2, #2
	adds r0, r0, r2
	lsls r0, r0, #2
	adds r1, #0x14
	adds r1, r1, r0
	ldr r0, [r6]
	ldr r0, [r0, #0x1c]
	add r0, r8
	ldr r1, [r1]
	cmp r1, r0
	ble _08029FA0
_08029FE0:
	ldr r1, _0802A014 @ =gUnknown_03001424
	movs r0, #0
	str r0, [r1]
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08029FF4: .4byte gUnknown_03001418
_08029FF8: .4byte gStaticData_081756C4
_08029FFC: .4byte gUnknown_03001414
_0802A000: .4byte gUnknown_03001400
_0802A004: .4byte gUnknown_03001404
_0802A008: .4byte gUnknown_0300141C
_0802A00C: .4byte gUnknown_03001420
_0802A010: .4byte gUnknown_03001408
_0802A014: .4byte gUnknown_03001424

	thumb_func_start sub_802A018
sub_802A018: @ 0x0802A018
	push {r4, r5, r6, lr}
	sub sp, #0x24
	adds r5, r0, #0
	ldr r2, _0802A0FC @ =gUnknown_03000884
	ldr r0, _0802A100 @ =gUnknown_030014A0
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802A0F6
	ldr r2, [r2]
	add r1, sp, #0xc
	adds r0, r2, #0
	adds r0, #0x38
	ldm r0!, {r3, r4, r6}
	stm r1!, {r3, r4, r6}
	ldr r0, [r2, #0x1c]
	asrs r0, r0, #8
	ldr r3, [r2, #0x20]
	asrs r3, r3, #8
	ldr r1, [r2, #0x24]
	asrs r1, r1, #8
	add r4, sp, #0xc
	ldrh r2, [r4]
	adds r0, r2, r0
	strh r0, [r4]
	ldrh r0, [r4, #2]
	adds r0, r0, r3
	strh r0, [r4, #2]
	ldrh r3, [r4, #4]
	adds r1, r3, r1
	strh r1, [r4, #4]
	mov r1, sp
	adds r0, r4, #0
	ldm r0!, {r2, r3, r6}
	stm r1!, {r2, r3, r6}
	mov r0, sp
	mov r1, sp
	movs r2, #0xc
	bl sub_800014C
	add r1, sp, #0x18
	adds r0, r5, #0
	adds r0, #0x38
	ldm r0!, {r2, r3, r6}
	stm r1!, {r2, r3, r6}
	ldr r0, [r5, #0x1c]
	asrs r0, r0, #8
	ldr r3, [r5, #0x20]
	asrs r3, r3, #8
	ldr r2, [r5, #0x24]
	asrs r2, r2, #8
	add r1, sp, #0x18
	ldrh r5, [r1]
	adds r0, r5, r0
	strh r0, [r1]
	ldrh r0, [r1, #2]
	adds r0, r0, r3
	strh r0, [r1, #2]
	ldrh r6, [r1, #4]
	adds r2, r6, r2
	strh r2, [r1, #4]
	adds r0, r4, #0
	ldm r1!, {r2, r3, r5}
	stm r0!, {r2, r3, r5}
	adds r0, r4, #0
	adds r1, r4, #0
	movs r2, #0xc
	bl sub_800014C
	mov r1, sp
	movs r6, #4
	ldrsh r2, [r1, r6]
	movs r0, #4
	ldrsh r3, [r4, r0]
	movs r5, #0xa
	ldrsh r0, [r4, r5]
	adds r0, r3, r0
	cmp r2, r0
	bge _0802A0F6
	movs r6, #0xa
	ldrsh r0, [r1, r6]
	adds r0, r2, r0
	cmp r0, r3
	ble _0802A0F6
	movs r0, #2
	ldrsh r2, [r1, r0]
	movs r5, #2
	ldrsh r3, [r4, r5]
	movs r6, #8
	ldrsh r0, [r4, r6]
	adds r0, r3, r0
	cmp r2, r0
	bge _0802A0F6
	movs r5, #8
	ldrsh r0, [r1, r5]
	adds r0, r2, r0
	cmp r0, r3
	ble _0802A0F6
	movs r6, #0
	ldrsh r2, [r1, r6]
	movs r0, #0
	ldrsh r3, [r4, r0]
	movs r5, #6
	ldrsh r0, [r4, r5]
	adds r0, r3, r0
	cmp r2, r0
	bge _0802A0F6
	movs r6, #6
	ldrsh r0, [r1, r6]
	adds r0, r2, r0
	cmp r0, r3
	bgt _0802A104
_0802A0F6:
	movs r0, #0
	b _0802A106
	.align 2, 0
_0802A0FC: .4byte gUnknown_03000884
_0802A100: .4byte gUnknown_030014A0
_0802A104:
	movs r0, #1
_0802A106:
	add sp, #0x24
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_802A110
sub_802A110: @ 0x0802A110
	push {r4, r5, r6, lr}
	sub sp, #0x24
	adds r5, r0, #0
	ldr r2, _0802A1F4 @ =gUnknown_03000884
	ldr r0, _0802A1F8 @ =gUnknown_03001506
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802A1EE
	ldr r2, [r2]
	add r1, sp, #0xc
	adds r0, r2, #0
	adds r0, #0x38
	ldm r0!, {r3, r4, r6}
	stm r1!, {r3, r4, r6}
	ldr r0, [r2, #0x1c]
	asrs r0, r0, #8
	ldr r3, [r2, #0x20]
	asrs r3, r3, #8
	ldr r1, [r2, #0x24]
	asrs r1, r1, #8
	add r4, sp, #0xc
	ldrh r2, [r4]
	adds r0, r2, r0
	strh r0, [r4]
	ldrh r0, [r4, #2]
	adds r0, r0, r3
	strh r0, [r4, #2]
	ldrh r3, [r4, #4]
	adds r1, r3, r1
	strh r1, [r4, #4]
	mov r1, sp
	adds r0, r4, #0
	ldm r0!, {r2, r3, r6}
	stm r1!, {r2, r3, r6}
	mov r0, sp
	mov r1, sp
	movs r2, #0xc
	bl sub_800014C
	add r1, sp, #0x18
	adds r0, r5, #0
	adds r0, #0x38
	ldm r0!, {r2, r3, r6}
	stm r1!, {r2, r3, r6}
	ldr r0, [r5, #0x1c]
	asrs r0, r0, #8
	ldr r3, [r5, #0x20]
	asrs r3, r3, #8
	ldr r2, [r5, #0x24]
	asrs r2, r2, #8
	add r1, sp, #0x18
	ldrh r5, [r1]
	adds r0, r5, r0
	strh r0, [r1]
	ldrh r0, [r1, #2]
	adds r0, r0, r3
	strh r0, [r1, #2]
	ldrh r6, [r1, #4]
	adds r2, r6, r2
	strh r2, [r1, #4]
	adds r0, r4, #0
	ldm r1!, {r2, r3, r5}
	stm r0!, {r2, r3, r5}
	adds r0, r4, #0
	adds r1, r4, #0
	movs r2, #0xc
	bl sub_800014C
	mov r1, sp
	movs r6, #4
	ldrsh r2, [r1, r6]
	movs r0, #4
	ldrsh r3, [r4, r0]
	movs r5, #0xa
	ldrsh r0, [r4, r5]
	adds r0, r3, r0
	cmp r2, r0
	bge _0802A1EE
	movs r6, #0xa
	ldrsh r0, [r1, r6]
	adds r0, r2, r0
	cmp r0, r3
	ble _0802A1EE
	movs r0, #2
	ldrsh r2, [r1, r0]
	movs r5, #2
	ldrsh r3, [r4, r5]
	movs r6, #8
	ldrsh r0, [r4, r6]
	adds r0, r3, r0
	cmp r2, r0
	bge _0802A1EE
	movs r5, #8
	ldrsh r0, [r1, r5]
	adds r0, r2, r0
	cmp r0, r3
	ble _0802A1EE
	movs r6, #0
	ldrsh r2, [r1, r6]
	movs r0, #0
	ldrsh r3, [r4, r0]
	movs r5, #6
	ldrsh r0, [r4, r5]
	adds r0, r3, r0
	cmp r2, r0
	bge _0802A1EE
	movs r6, #6
	ldrsh r0, [r1, r6]
	adds r0, r2, r0
	cmp r0, r3
	bgt _0802A1FC
_0802A1EE:
	movs r0, #0
	b _0802A1FE
	.align 2, 0
_0802A1F4: .4byte gUnknown_03000884
_0802A1F8: .4byte gUnknown_03001506
_0802A1FC:
	movs r0, #1
_0802A1FE:
	add sp, #0x24
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_802A208
sub_802A208: @ 0x0802A208
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r7, _0802A254 @ =gUnknown_03001418
	ldr r0, [r7]
	ldr r0, [r0, #0xc]
	cmp r0, #0
	beq _0802A21C
	bl sub_803AD78
_0802A21C:
	ldr r1, _0802A258 @ =gUnknown_03001410
	movs r0, #0
	str r0, [r1]
	bl sub_8029B2C
	adds r6, r0, #0
	ldr r5, _0802A25C @ =gUnknown_03001420
	ldr r1, [r5]
	subs r1, r6, r1
	ldr r4, _0802A260 @ =gUnknown_03001400
	ldr r0, [r4]
	ldr r0, [r0]
	cmp r1, r0
	ble _0802A240
	ldr r0, [r7]
	ldr r0, [r0, #0x28]
	bl sub_803AD78
_0802A240:
	ldr r0, _0802A264 @ =gUnknown_0300141C
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802A268
	bl sub_8029B8C
	ldr r1, [r5]
	adds r1, r1, r0
	str r1, [r5]
	b _0802A2E4
	.align 2, 0
_0802A254: .4byte gUnknown_03001418
_0802A258: .4byte gUnknown_03001410
_0802A25C: .4byte gUnknown_03001420
_0802A260: .4byte gUnknown_03001400
_0802A264: .4byte gUnknown_0300141C
_0802A268:
	ldr r0, _0802A38C @ =gUnknown_03001404
	mov ip, r0
	ldr r3, [r4]
	ldr r2, [r0]
	ldr r0, [r3, #4]
	cmp r2, r0
	bge _0802A2E4
	lsls r1, r2, #2
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r0, r3, #0
	adds r0, #0x14
	adds r0, r0, r1
	ldr r1, [r0]
	ldr r0, [r5]
	adds r1, r1, r0
	ldr r0, [r7]
	ldr r0, [r0, #0x1c]
	adds r0, r6, r0
	cmp r1, r0
	bgt _0802A2E4
	mov r8, r7
	adds r7, r4, #0
	mov r4, ip
_0802A298:
	mov r2, r8
	ldr r3, [r2]
	ldr r0, [r4]
	lsls r1, r0, #2
	adds r1, r1, r0
	lsls r1, r1, #2
	adds r1, #8
	ldr r0, [r7]
	adds r0, r0, r1
	ldr r1, _0802A390 @ =gUnknown_03001414
	ldrb r1, [r1]
	ldr r2, [r5]
	lsls r2, r2, #8
	ldr r3, [r3, #4]
	bl sub_803AD84
	ldr r0, [r4]
	adds r2, r0, #1
	str r2, [r4]
	ldr r3, [r7]
	ldr r0, [r3, #4]
	cmp r2, r0
	bge _0802A2E4
	lsls r1, r2, #2
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r0, r3, #0
	adds r0, #0x14
	adds r0, r0, r1
	ldr r1, [r0]
	ldr r0, [r5]
	adds r1, r1, r0
	mov r3, r8
	ldr r0, [r3]
	ldr r0, [r0, #0x1c]
	adds r0, r6, r0
	cmp r1, r0
	ble _0802A298
_0802A2E4:
	ldr r0, _0802A394 @ =gUnknown_03000884
	ldr r3, [r0]
	adds r5, r0, #0
_0802A2EA:
	ldr r4, [r3, #0x4c]
	ldr r1, [r3, #0x50]
	movs r2, #0x10
	ldrsh r0, [r1, r2]
	adds r0, r3, r0
	ldr r1, [r1, #0x14]
	bl sub_803AD7C
	adds r3, r4, #0
	ldr r0, [r5]
	cmp r3, r0
	bne _0802A2EA
	ldr r0, _0802A398 @ =gUnknown_0300140C
	movs r1, #0
	str r1, [r0]
	ldr r1, _0802A394 @ =gUnknown_03000884
	ldr r3, [r1]
	adds r5, r0, #0
	ldr r0, _0802A39C @ =gUnknown_03001408
	mov r8, r0
	ldr r2, _0802A3A0 @ =gUnknown_03000880
	mov ip, r2
	adds r4, r5, #0
	mov r7, r8
	adds r6, r1, #0
_0802A31C:
	adds r0, r3, #0
	adds r0, #0x2c
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802A334
	ldr r0, [r4]
	ldr r2, [r7]
	lsls r1, r0, #2
	adds r1, r1, r2
	str r3, [r1]
	adds r0, #1
	str r0, [r4]
_0802A334:
	ldr r3, [r3, #0x4c]
	ldr r0, [r6]
	cmp r3, r0
	bne _0802A31C
	ldr r0, [r5]
	mov r3, r8
	ldr r1, [r3]
	mov r3, ip
	ldr r2, [r3]
	bl sub_803AD80
	movs r4, #0
	ldr r0, [r5]
	cmp r4, r0
	bge _0802A374
_0802A352:
	ldr r0, _0802A39C @ =gUnknown_03001408
	ldr r1, [r0]
	lsls r0, r4, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r2, [r0, #0x50]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r2, #0x1c]
	bl sub_803AD7C
	adds r4, #1
	ldr r0, _0802A398 @ =gUnknown_0300140C
	ldr r0, [r0]
	cmp r4, r0
	blt _0802A352
_0802A374:
	ldr r1, _0802A3A4 @ =gUnknown_03001424
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
	ldr r0, _0802A3A8 @ =gUnknown_03001410
	ldr r0, [r0]
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0802A38C: .4byte gUnknown_03001404
_0802A390: .4byte gUnknown_03001414
_0802A394: .4byte gUnknown_03000884
_0802A398: .4byte gUnknown_0300140C
_0802A39C: .4byte gUnknown_03001408
_0802A3A0: .4byte gUnknown_03000880
_0802A3A4: .4byte gUnknown_03001424
_0802A3A8: .4byte gUnknown_03001410

	thumb_func_start sub_802A3AC
sub_802A3AC: @ 0x0802A3AC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x24
	mov r8, r0
	ldr r0, _0802A4A8 @ =gUnknown_03000884
	ldr r0, [r0]
	ldr r5, [r0, #0x4c]
	add r7, sp, #0x18
_0802A3BE:
	cmp r5, r8
	beq _0802A4B6
	ldr r1, [r5, #0x50]
	movs r2, #0x28
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r1, [r1, #0x2c]
	bl sub_803AD7C
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0802A4B6
	add r1, sp, #0xc
	mov r0, r8
	adds r0, #0x38
	ldm r0!, {r3, r4, r6}
	stm r1!, {r3, r4, r6}
	mov r1, r8
	ldr r0, [r1, #0x1c]
	asrs r0, r0, #8
	ldr r2, [r1, #0x20]
	asrs r2, r2, #8
	ldr r1, [r1, #0x24]
	asrs r1, r1, #8
	add r4, sp, #0xc
	ldrh r3, [r4]
	adds r0, r3, r0
	strh r0, [r4]
	ldrh r0, [r4, #2]
	adds r0, r0, r2
	strh r0, [r4, #2]
	ldrh r6, [r4, #4]
	adds r1, r6, r1
	strh r1, [r4, #4]
	mov r1, sp
	adds r0, r4, #0
	ldm r0!, {r2, r3, r6}
	stm r1!, {r2, r3, r6}
	mov r0, sp
	mov r1, sp
	movs r2, #0xc
	bl sub_800014C
	add r1, sp, #0x18
	adds r0, r5, #0
	adds r0, #0x38
	ldm r0!, {r2, r3, r6}
	stm r1!, {r2, r3, r6}
	ldr r0, [r5, #0x1c]
	asrs r0, r0, #8
	ldr r2, [r5, #0x20]
	asrs r2, r2, #8
	ldr r1, [r5, #0x24]
	asrs r1, r1, #8
	ldrh r3, [r7]
	adds r0, r3, r0
	strh r0, [r7]
	ldrh r0, [r7, #2]
	adds r0, r0, r2
	strh r0, [r7, #2]
	ldrh r6, [r7, #4]
	adds r1, r6, r1
	strh r1, [r7, #4]
	adds r1, r4, #0
	adds r0, r7, #0
	ldm r0!, {r2, r3, r6}
	stm r1!, {r2, r3, r6}
	adds r0, r4, #0
	adds r1, r4, #0
	movs r2, #0xc
	bl sub_800014C
	mov r1, sp
	movs r0, #4
	ldrsh r2, [r1, r0]
	movs r6, #4
	ldrsh r3, [r4, r6]
	movs r6, #0xa
	ldrsh r0, [r4, r6]
	adds r0, r3, r0
	cmp r2, r0
	bge _0802A4A4
	movs r6, #0xa
	ldrsh r0, [r1, r6]
	adds r0, r2, r0
	cmp r0, r3
	ble _0802A4A4
	movs r0, #2
	ldrsh r2, [r1, r0]
	movs r6, #2
	ldrsh r3, [r4, r6]
	movs r6, #8
	ldrsh r0, [r4, r6]
	adds r0, r3, r0
	cmp r2, r0
	bge _0802A4A4
	movs r6, #8
	ldrsh r0, [r1, r6]
	adds r0, r2, r0
	cmp r0, r3
	ble _0802A4A4
	movs r0, #0
	ldrsh r2, [r1, r0]
	movs r6, #0
	ldrsh r3, [r4, r6]
	movs r0, #6
	ldrsh r4, [r4, r0]
	adds r0, r3, r4
	cmp r2, r0
	bge _0802A4A4
	movs r4, #6
	ldrsh r0, [r1, r4]
	adds r0, r2, r0
	cmp r0, r3
	bgt _0802A4AC
_0802A4A4:
	movs r0, #0
	b _0802A4AE
	.align 2, 0
_0802A4A8: .4byte gUnknown_03000884
_0802A4AC:
	movs r0, #1
_0802A4AE:
	cmp r0, #0
	beq _0802A4B6
	adds r0, r5, #0
	b _0802A4C4
_0802A4B6:
	ldr r5, [r5, #0x4c]
	ldr r0, _0802A4D0 @ =gUnknown_03000884
	ldr r0, [r0]
	cmp r5, r0
	beq _0802A4C2
	b _0802A3BE
_0802A4C2:
	movs r0, #0
_0802A4C4:
	add sp, #0x24
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0802A4D0: .4byte gUnknown_03000884

	thumb_func_start sub_802A4D4
sub_802A4D4: @ 0x0802A4D4
	ldr r0, _0802A4DC @ =gUnknown_03001424
	ldr r0, [r0]
	bx lr
	.align 2, 0
_0802A4DC: .4byte gUnknown_03001424

	thumb_func_start sub_802A4E0
sub_802A4E0: @ 0x0802A4E0
	ldr r0, _0802A4E8 @ =gUnknown_03001420
	ldr r0, [r0]
	lsls r0, r0, #8
	bx lr
	.align 2, 0
_0802A4E8: .4byte gUnknown_03001420

	thumb_func_start sub_802A4EC
sub_802A4EC: @ 0x0802A4EC
	ldr r1, _0802A4F4 @ =gUnknown_0300141C
	movs r0, #0
	strb r0, [r1]
	bx lr
	.align 2, 0
_0802A4F4: .4byte gUnknown_0300141C

	thumb_func_start sub_802A4F8
sub_802A4F8: @ 0x0802A4F8
	ldr r1, _0802A500 @ =gUnknown_0300141C
	movs r0, #1
	strb r0, [r1]
	bx lr
	.align 2, 0
_0802A500: .4byte gUnknown_0300141C

	thumb_func_start sub_802A504
sub_802A504: @ 0x0802A504
	ldr r1, _0802A518 @ =gUnknown_03001400
	ldr r2, [r1]
	lsls r1, r0, #2
	adds r1, r1, r0
	lsls r1, r1, #2
	adds r2, #0x18
	adds r2, r2, r1
	ldr r0, [r2]
	bx lr
	.align 2, 0
_0802A518: .4byte gUnknown_03001400

	thumb_func_start sub_802A51C
sub_802A51C: @ 0x0802A51C
	ldr r1, _0802A538 @ =gUnknown_03001400
	ldr r2, [r1]
	lsls r1, r0, #2
	adds r1, r1, r0
	lsls r1, r1, #2
	adds r2, #0x14
	adds r2, r2, r1
	ldr r1, _0802A53C @ =gUnknown_03001420
	ldr r0, [r2]
	ldr r1, [r1]
	adds r0, r0, r1
	lsls r0, r0, #8
	bx lr
	.align 2, 0
_0802A538: .4byte gUnknown_03001400
_0802A53C: .4byte gUnknown_03001420

	thumb_func_start sub_802A540
sub_802A540: @ 0x0802A540
	ldr r1, _0802A554 @ =gUnknown_03001400
	ldr r2, [r1]
	lsls r1, r0, #2
	adds r1, r1, r0
	lsls r1, r1, #2
	adds r2, #0x10
	adds r2, r2, r1
	ldr r0, [r2]
	lsls r0, r0, #8
	bx lr
	.align 2, 0
_0802A554: .4byte gUnknown_03001400

	thumb_func_start sub_802A558
sub_802A558: @ 0x0802A558
	ldr r1, _0802A56C @ =gUnknown_03001400
	ldr r2, [r1]
	lsls r1, r0, #2
	adds r1, r1, r0
	lsls r1, r1, #2
	adds r2, #0xc
	adds r2, r2, r1
	ldr r0, [r2]
	lsls r0, r0, #8
	bx lr
	.align 2, 0
_0802A56C: .4byte gUnknown_03001400

	thumb_func_start sub_802A570
sub_802A570: @ 0x0802A570
	ldr r1, _0802A590 @ =gUnknown_03001400
	ldr r2, [r1]
	lsls r1, r0, #2
	adds r1, r1, r0
	lsls r1, r1, #2
	adds r2, r2, r1
	ldrb r1, [r2, #8]
	ldr r0, _0802A594 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802A598
	ldrb r1, [r2, #9]
	b _0802A5A2
	.align 2, 0
_0802A590: .4byte gUnknown_03001400
_0802A594: .4byte gUnknown_030012C0
_0802A598:
	ldr r0, _0802A5A8 @ =gUnknown_03001414
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802A5A2
	ldrb r1, [r2, #0xa]
_0802A5A2:
	adds r0, r1, #0
	subs r0, #0x20
	bx lr
	.align 2, 0
_0802A5A8: .4byte gUnknown_03001414

	thumb_func_start sub_802A5AC
sub_802A5AC: @ 0x0802A5AC
	push {lr}
	ldr r0, _0802A5C0 @ =gUnknown_03001418
	ldr r0, [r0]
	ldr r0, [r0, #0x30]
	bl sub_803AD78
	movs r1, #1
	eors r0, r1
	pop {r1}
	bx r1
	.align 2, 0
_0802A5C0: .4byte gUnknown_03001418

	thumb_func_start sub_802A5C4
sub_802A5C4: @ 0x0802A5C4
	push {r4, lr}
	ldr r4, _0802A5E0 @ =gUnknown_03001418
	ldr r0, [r4]
	ldr r0, [r0, #0x18]
	bl sub_803AD78
	ldr r0, [r4]
	ldr r0, [r0, #0x2c]
	bl sub_803AD78
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0802A5E0: .4byte gUnknown_03001418

	thumb_func_start sub_802A5E4
sub_802A5E4: @ 0x0802A5E4
	push {r4, r5, lr}
	ldr r0, _0802A644 @ =gUnknown_03001418
	ldr r0, [r0]
	ldr r0, [r0, #0x14]
	cmp r0, #0
	beq _0802A5F4
	bl sub_803AD78
_0802A5F4:
	ldr r0, _0802A648 @ =gUnknown_03000884
	ldr r1, [r0]
	ldr r2, [r1, #0x4c]
	adds r5, r0, #0
	cmp r2, r1
	beq _0802A620
_0802A600:
	ldr r4, [r2, #0x4c]
	cmp r2, #0
	beq _0802A616
	ldr r1, [r2, #0x50]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_0802A616:
	adds r2, r4, #0
	ldr r0, _0802A648 @ =gUnknown_03000884
	ldr r0, [r0]
	cmp r2, r0
	bne _0802A600
_0802A620:
	ldr r2, [r5]
	cmp r2, #0
	beq _0802A636
	ldr r1, [r2, #0x50]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_0802A636:
	ldr r0, _0802A64C @ =gUnknown_03001408
	ldr r0, [r0]
	bl mem_free
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802A644: .4byte gUnknown_03001418
_0802A648: .4byte gUnknown_03000884
_0802A64C: .4byte gUnknown_03001408

	thumb_func_start sub_802A650
sub_802A650: @ 0x0802A650
	push {lr}
	ldr r0, _0802A664 @ =gUnknown_03001418
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	cmp r0, #0
	beq _0802A660
	bl sub_803AD78
_0802A660:
	pop {r0}
	bx r0
	.align 2, 0
_0802A664: .4byte gUnknown_03001418

	thumb_func_start sub_802A668
sub_802A668: @ 0x0802A668
	ldr r1, _0802A670 @ =gUnknown_03001410
	str r0, [r1]
	bx lr
	.align 2, 0
_0802A670: .4byte gUnknown_03001410

	thumb_func_start sub_802A674
sub_802A674: @ 0x0802A674
	push {lr}
	ldr r0, _0802A684 @ =gUnknown_03000884
	ldr r0, [r0]
	bl sub_802F4C0
	pop {r1}
	bx r1
	.align 2, 0
_0802A684: .4byte gUnknown_03000884

	thumb_func_start sub_802A688
sub_802A688: @ 0x0802A688
	push {lr}
	ldr r0, _0802A698 @ =gUnknown_03000884
	ldr r0, [r0]
	bl sub_802BD18
	pop {r1}
	bx r1
	.align 2, 0
_0802A698: .4byte gUnknown_03000884

	thumb_func_start sub_802A69C
sub_802A69C: @ 0x0802A69C
	push {lr}
	ldr r0, _0802A6AC @ =gUnknown_03000884
	ldr r0, [r0]
	bl sub_802F338
	pop {r0}
	bx r0
	.align 2, 0
_0802A6AC: .4byte gUnknown_03000884

	thumb_func_start sub_802A6B0
sub_802A6B0: @ 0x0802A6B0
	push {lr}
	ldr r0, _0802A6C0 @ =gUnknown_03000884
	ldr r0, [r0]
	bl sub_802B864
	pop {r0}
	bx r0
	.align 2, 0
_0802A6C0: .4byte gUnknown_03000884

	thumb_func_start sub_802A6C4
sub_802A6C4: @ 0x0802A6C4
	push {lr}
	ldr r0, _0802A6D4 @ =gUnknown_03000884
	ldr r0, [r0]
	bl sub_802F0DC
	pop {r0}
	bx r0
	.align 2, 0
_0802A6D4: .4byte gUnknown_03000884

	thumb_func_start sub_802A6D8
sub_802A6D8: @ 0x0802A6D8
	push {lr}
	ldr r0, _0802A6E8 @ =gUnknown_03000884
	ldr r0, [r0]
	bl sub_802BFD4
	pop {r0}
	bx r0
	.align 2, 0
_0802A6E8: .4byte gUnknown_03000884

	thumb_func_start sub_802A6EC
sub_802A6EC: @ 0x0802A6EC
	push {lr}
	ldr r1, _0802A6FC @ =gUnknown_03001418
	ldr r1, [r1]
	ldr r1, [r1, #0x24]
	bl sub_803AD7C
	pop {r1}
	bx r1
	.align 2, 0
_0802A6FC: .4byte gUnknown_03001418

	thumb_func_start InitActorPart
InitActorPart: @ 0x0802A700
	push {r4, r5, r6, r7, lr}
	adds r7, r0, #0
	adds r4, r1, #0
	adds r5, r2, #0
	adds r6, r3, #0
	ldrb r2, [r4, #0xc]
	ldr r0, [r4, #4]
	ldr r1, [r4, #8]
	str r0, [r7]
	str r1, [r7, #4]
	str r2, [r7, #0x18]
	adds r0, r7, #0
	movs r1, #0
	bl sub_803B0A8
	ldr r0, _0802A7A4 @ =gStaticData_087E4DF4
	str r0, [r7, #0x50]
	str r5, [r7, #0x1c]
	str r6, [r7, #0x20]
	ldr r0, [sp, #0x14]
	str r0, [r7, #0x24]
	str r4, [r7, #0x30]
	adds r0, r7, #0
	adds r0, #0x38
	adds r4, #0x14
	ldm r4!, {r1, r2, r3}
	stm r0!, {r1, r2, r3}
	movs r0, #0
	str r0, [r7, #0x28]
	str r0, [r7, #0x44]
	adds r1, r7, #0
	adds r1, #0x2c
	movs r0, #1
	strb r0, [r1]
	bl sub_8029B2C
	lsls r0, r0, #8
	ldr r2, [r7, #0x24]
	subs r2, r2, r0
	asrs r0, r2, #0x1f
	eors r2, r0
	subs r2, r2, r0
	str r2, [r7, #0x34]
	asrs r2, r2, #1
	movs r0, #0xff
	lsls r0, r0, #7
	ands r2, r0
	ldr r1, [r7, #0x20]
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	ldr r0, [r7, #0x1c]
	asrs r3, r0, #0x1f
	eors r0, r3
	subs r0, r0, r3
	adds r1, r1, r0
	asrs r1, r1, #0xb
	movs r0, #0x7f
	ands r1, r0
	orrs r2, r1
	str r2, [r7, #0x14]
	bl sub_8029E40
	ldr r1, [r7, #0x34]
	cmp r1, r0
	ble _0802A78E
	ldr r0, [r7, #0x14]
	movs r1, #0x80
	lsls r1, r1, #8
	orrs r0, r1
	str r0, [r7, #0x14]
_0802A78E:
	ldr r0, _0802A7A8 @ =gUnknown_03000884
	ldr r1, [r0]
	cmp r1, #0
	beq _0802A7AC
	str r1, [r7, #0x4c]
	ldr r0, [r1, #0x48]
	str r0, [r7, #0x48]
	str r7, [r1, #0x48]
	ldr r0, [r7, #0x48]
	str r7, [r0, #0x4c]
	b _0802A7B0
	.align 2, 0
_0802A7A4: .4byte gStaticData_087E4DF4
_0802A7A8: .4byte gUnknown_03000884
_0802A7AC:
	str r7, [r7, #0x4c]
	str r7, [r7, #0x48]
_0802A7B0:
	adds r0, r7, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_802A7B8
sub_802A7B8: @ 0x0802A7B8
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8029B2C
	lsls r0, r0, #8
	ldr r2, [r4, #0x24]
	subs r2, r2, r0
	asrs r0, r2, #0x1f
	eors r2, r0
	subs r2, r2, r0
	str r2, [r4, #0x34]
	asrs r2, r2, #1
	movs r0, #0xff
	lsls r0, r0, #7
	ands r2, r0
	ldr r1, [r4, #0x20]
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	ldr r0, [r4, #0x1c]
	asrs r3, r0, #0x1f
	eors r0, r3
	subs r0, r0, r3
	adds r1, r1, r0
	asrs r1, r1, #0xb
	movs r0, #0x7f
	ands r1, r0
	orrs r2, r1
	str r2, [r4, #0x14]
	bl sub_8029E40
	ldr r1, [r4, #0x34]
	cmp r1, r0
	ble _0802A806
	ldr r0, [r4, #0x14]
	movs r1, #0x80
	lsls r1, r1, #8
	orrs r0, r1
	str r0, [r4, #0x14]
_0802A806:
	ldr r0, _0802A838 @ =gUnknown_030013C4
	ldr r0, [r0]
	movs r1, #0x80
	lsls r1, r1, #2
	adds r0, r0, r1
	ldr r1, [r4, #0x34]
	cmp r1, r0
	bgt _0802A822
	ldr r0, _0802A83C @ =gUnknown_030013C0
	ldr r0, [r0]
	ldr r2, _0802A840 @ =0xFFFFFE00
	adds r0, r0, r2
	cmp r1, r0
	bge _0802A844
_0802A822:
	cmp r4, #0
	beq _0802A884
	ldr r1, [r4, #0x50]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
	b _0802A884
	.align 2, 0
_0802A838: .4byte gUnknown_030013C4
_0802A83C: .4byte gUnknown_030013C0
_0802A840: .4byte 0xFFFFFE00
_0802A844:
	ldr r0, [r4, #0x44]
	adds r0, #1
	str r0, [r4, #0x44]
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
	blt _0802A884
	movs r3, #6
	ldrsh r0, [r1, r3]
	subs r0, r2, r0
	lsls r0, r0, #8
	ldr r1, [r4, #8]
	subs r1, r1, r0
	str r1, [r4, #8]
	movs r0, #1
	strb r0, [r4, #0x12]
_0802A884:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start UpdateAnimatedActorPart
UpdateAnimatedActorPart: @ 0x0802A88C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r6, r0, #0
	ldr r4, [r6, #0x34]
	lsls r0, r4, #8
	ldr r1, [r6, #0x30]
	ldr r1, [r1, #0x10]
	bl sub_803ADB4
	mov r8, r0
	ldr r0, _0802A8F0 @ =gUnknown_030013C8
	ldr r0, [r0]
	lsls r0, r0, #0xc
	adds r1, r4, #0
	bl sub_803ADB4
	adds r5, r0, #0
	bl sub_8029E98
	ldr r1, [r6, #0x20]
	adds r4, r1, #0
	muls r4, r5, r4
	asrs r4, r4, #0xc
	adds r4, r4, r0
	asrs r4, r4, #8
	bl sub_8029EB4
	ldr r1, [r6, #0x1c]
	muls r1, r5, r1
	asrs r1, r1, #0xc
	adds r1, r1, r0
	asrs r5, r1, #8
	adds r0, r6, #0
	bl GetAnimFrameData
	adds r7, r0, #0
	movs r2, #0
	mov r1, r8
	cmp r1, #0xff
	bgt _0802A8E4
	movs r2, #0x80
	lsls r2, r2, #2
_0802A8E4:
	cmp r2, #0
	beq _0802A8F4
	ldrb r3, [r7]
	lsls r1, r3, #3
	b _0802A8F8
	.align 2, 0
_0802A8F0: .4byte gUnknown_030013C8
_0802A8F4:
	ldrb r3, [r7]
	lsls r1, r3, #2
_0802A8F8:
	cmp r2, #0
	beq _0802A902
	ldrb r0, [r0, #1]
	lsls r0, r0, #3
	b _0802A906
_0802A902:
	ldrb r0, [r0, #1]
	lsls r0, r0, #2
_0802A906:
	subs r5, r5, r1
	subs r4, r4, r0
	cmp r4, #0x9f
	bgt _0802A972
	lsls r0, r0, #1
	adds r0, r4, r0
	cmp r0, #0
	blt _0802A972
	cmp r5, #0xef
	bgt _0802A972
	lsls r0, r1, #1
	adds r0, r5, r0
	cmp r0, #0
	blt _0802A972
	movs r0, #0x80
	lsls r0, r0, #1
	cmp r8, r0
	beq _0802A92C
	orrs r2, r0
_0802A92C:
	adds r0, r6, #0
	str r2, [sp]
	bl sub_803B060
	movs r3, #0xff
	ands r3, r4
	ldr r1, _0802A960 @ =0x000001FF
	ands r5, r1
	lsls r1, r5, #0x10
	orrs r3, r1
	orrs r3, r0
	ldr r2, [sp]
	orrs r3, r2
	ldr r4, [r6, #0x18]
	lsls r2, r4, #0xc
	ldr r0, [r6, #0x14]
	movs r1, #0x80
	lsls r1, r1, #8
	ands r0, r1
	cmp r0, #0
	beq _0802A964
	movs r0, #0x80
	lsls r0, r0, #4
	orrs r2, r0
	lsls r0, r2, #0x10
	b _0802A966
	.align 2, 0
_0802A960: .4byte 0x000001FF
_0802A964:
	lsls r0, r4, #0x1c
_0802A966:
	lsrs r2, r0, #0x10
	adds r0, r7, #0
	adds r1, r3, #0
	mov r3, r8
	bl SetupSpriteFrameOam
_0802A972:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_802A980
sub_802A980: @ 0x0802A980
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8029B2C
	lsls r0, r0, #8
	ldr r2, [r4, #0x24]
	subs r2, r2, r0
	asrs r0, r2, #0x1f
	eors r2, r0
	subs r2, r2, r0
	str r2, [r4, #0x34]
	asrs r2, r2, #1
	movs r0, #0xff
	lsls r0, r0, #7
	ands r2, r0
	ldr r1, [r4, #0x20]
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	ldr r0, [r4, #0x1c]
	asrs r3, r0, #0x1f
	eors r0, r3
	subs r0, r0, r3
	adds r1, r1, r0
	asrs r1, r1, #0xb
	movs r0, #0x7f
	ands r1, r0
	orrs r2, r1
	str r2, [r4, #0x14]
	bl sub_8029E40
	ldr r1, [r4, #0x34]
	cmp r1, r0
	ble _0802A9CE
	ldr r0, [r4, #0x14]
	movs r1, #0x80
	lsls r1, r1, #8
	orrs r0, r1
	str r0, [r4, #0x14]
_0802A9CE:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_802A9D4
sub_802A9D4: @ 0x0802A9D4
	ldr r0, [r0, #0x30]
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_802A9DC
sub_802A9DC: @ 0x0802A9DC
	push {r4, lr}
	str r1, [r0, #0x28]
	movs r4, #0
	str r4, [r0, #0x44]
	str r2, [r0, #0xc]
	ldr r3, [r0]
	lsls r1, r2, #1
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r1, r1, r3
	ldrh r1, [r1]
	movs r2, #0
	strh r1, [r0, #0x10]
	strb r2, [r0, #0x12]
	str r4, [r0, #8]
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_802AA00
sub_802AA00: @ 0x0802AA00
	ldr r0, [r0, #0x24]
	bx lr

	thumb_func_start sub_802AA04
sub_802AA04: @ 0x0802AA04
	ldr r0, [r0, #0x20]
	bx lr

	thumb_func_start sub_802AA08
sub_802AA08: @ 0x0802AA08
	ldr r0, [r0, #0x1c]
	bx lr

	thumb_func_start sub_802AA0C
sub_802AA0C: @ 0x0802AA0C
	push {r4, r5, r6, lr}
	sub sp, #0xc
	mov r3, sp
	adds r2, r1, #0
	adds r2, #0x38
	ldm r2!, {r4, r5, r6}
	stm r3!, {r4, r5, r6}
	ldr r3, [r1, #0x1c]
	asrs r3, r3, #8
	ldr r5, [r1, #0x20]
	asrs r5, r5, #8
	ldr r4, [r1, #0x24]
	asrs r4, r4, #8
	mov r2, sp
	ldrh r1, [r2]
	adds r3, r1, r3
	strh r3, [r2]
	ldrh r1, [r2, #2]
	adds r1, r1, r5
	strh r1, [r2, #2]
	ldrh r3, [r2, #4]
	adds r4, r3, r4
	strh r4, [r2, #4]
	adds r2, r0, #0
	mov r1, sp
	ldm r1!, {r4, r5, r6}
	stm r2!, {r4, r5, r6}
	add sp, #0xc
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_802AA4C
sub_802AA4C: @ 0x0802AA4C
	adds r0, #0x2c
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_802AA54
sub_802AA54: @ 0x0802AA54
	push {lr}
	adds r3, r0, #0
	ldr r0, _0802AA7C @ =gStaticData_087E4DF4
	str r0, [r3, #0x50]
	ldr r2, [r3, #0x4c]
	ldr r0, [r3, #0x48]
	str r0, [r2, #0x48]
	ldr r2, [r3, #0x48]
	ldr r0, [r3, #0x4c]
	str r0, [r2, #0x4c]
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _0802AA76
	adds r0, r3, #0
	bl mem_free
_0802AA76:
	pop {r0}
	bx r0
	.align 2, 0
_0802AA7C: .4byte gStaticData_087E4DF4

	thumb_func_start sub_802AA80
sub_802AA80: @ 0x0802AA80
	push {r4, lr}
	adds r4, r0, #0
	movs r2, #0
	ldr r0, _0802AA9C @ =gUnknown_03000888
	ldr r0, [r0]
	cmp r2, r0
	bge _0802AAAC
	adds r3, r0, #0
	ldr r1, _0802AAA0 @ =gUnknown_03001428
_0802AA92:
	ldr r0, [r1]
	cmp r0, r4
	bne _0802AAA4
	movs r0, #1
	b _0802AAAE
	.align 2, 0
_0802AA9C: .4byte gUnknown_03000888
_0802AAA0: .4byte gUnknown_03001428
_0802AAA4:
	adds r1, #4
	adds r2, #1
	cmp r2, r3
	blt _0802AA92
_0802AAAC:
	movs r0, #0
_0802AAAE:
	pop {r4}
	pop {r1}
	bx r1

	thumb_func_start sub_802AAB4
sub_802AAB4: @ 0x0802AAB4
	push {r4, r5, r6, lr}
	adds r3, r0, #0
	ldr r0, _0802AAF4 @ =gUnknown_03000888
	ldr r2, [r0]
	adds r6, r0, #0
	cmp r2, #0xf
	beq _0802AAEC
	cmp r3, #0
	beq _0802AAEC
	movs r1, #0
	ldr r5, _0802AAF8 @ =gUnknown_03001428
	cmp r1, r2
	bge _0802AAE0
	adds r4, r2, #0
	adds r2, r5, #0
_0802AAD2:
	ldr r0, [r2]
	cmp r0, r3
	beq _0802AAEC
	adds r2, #4
	adds r1, #1
	cmp r1, r4
	blt _0802AAD2
_0802AAE0:
	ldr r1, [r6]
	lsls r0, r1, #2
	adds r0, r0, r5
	str r3, [r0]
	adds r1, #1
	str r1, [r6]
_0802AAEC:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802AAF4: .4byte gUnknown_03000888
_0802AAF8: .4byte gUnknown_03001428

	thumb_func_start sub_802AAFC
sub_802AAFC: @ 0x0802AAFC
	ldr r1, _0802AB04 @ =gUnknown_03000888
	movs r0, #0
	str r0, [r1]
	bx lr
	.align 2, 0
_0802AB04: .4byte gUnknown_03000888

	thumb_func_start sub_802AB08
sub_802AB08: @ 0x0802AB08
	ldr r1, _0802AB20 @ =gUnknown_03001470
	ldr r0, _0802AB24 @ =gUnknown_03001468
	ldr r0, [r0]
	str r0, [r1]
	ldr r1, _0802AB28 @ =gUnknown_03001474
	ldr r0, _0802AB2C @ =gUnknown_0300146C
	ldr r0, [r0]
	str r0, [r1]
	ldr r1, _0802AB30 @ =gUnknown_03001478
	movs r0, #0
	str r0, [r1]
	bx lr
	.align 2, 0
_0802AB20: .4byte gUnknown_03001470
_0802AB24: .4byte gUnknown_03001468
_0802AB28: .4byte gUnknown_03001474
_0802AB2C: .4byte gUnknown_0300146C
_0802AB30: .4byte gUnknown_03001478

	thumb_func_start sub_802AB34
sub_802AB34: @ 0x0802AB34
	ldr r1, _0802AB48 @ =gUnknown_03001468
	ldr r0, _0802AB4C @ =gUnknown_03001470
	ldr r0, [r0]
	str r0, [r1]
	ldr r1, _0802AB50 @ =gUnknown_0300146C
	ldr r0, _0802AB54 @ =gUnknown_03001474
	ldr r0, [r0]
	str r0, [r1]
	bx lr
	.align 2, 0
_0802AB48: .4byte gUnknown_03001468
_0802AB4C: .4byte gUnknown_03001470
_0802AB50: .4byte gUnknown_0300146C
_0802AB54: .4byte gUnknown_03001474

	thumb_func_start sub_802AB58
sub_802AB58: @ 0x0802AB58
	push {r4, lr}
	ldr r0, _0802ABA8 @ =gUnknown_03001464
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802ABC0
	ldr r4, _0802ABAC @ =gUnknown_03001470
	ldr r1, [r4]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r0, r0, #6
	ldr r1, _0802ABB0 @ =gStaticData_08175760
	adds r0, r0, r1
	movs r1, #0xa0
	lsls r1, r1, #0x13
	movs r2, #0xe0
	lsls r2, r2, #1
	movs r3, #0x10
	bl QueueVramDmaTransfer
	ldr r1, _0802ABB4 @ =gUnknown_03001478
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
	cmp r0, #0x23
	ble _0802ABC0
	movs r0, #0
	str r0, [r1]
	adds r1, r4, #0
	ldr r0, _0802ABB8 @ =gUnknown_03001474
	ldr r3, [r0]
	ldr r2, [r1]
	subs r0, r3, r2
	cmp r0, #0
	blt _0802ABBC
	adds r0, r2, #0
	cmp r3, r0
	beq _0802ABBE
	adds r0, #1
	b _0802ABBE
	.align 2, 0
_0802ABA8: .4byte gUnknown_03001464
_0802ABAC: .4byte gUnknown_03001470
_0802ABB0: .4byte gStaticData_08175760
_0802ABB4: .4byte gUnknown_03001478
_0802ABB8: .4byte gUnknown_03001474
_0802ABBC:
	subs r0, r2, #1
_0802ABBE:
	str r0, [r1]
_0802ABC0:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_802ABC8
sub_802ABC8: @ 0x0802ABC8
	ldr r2, _0802ABE8 @ =gUnknown_03001470
	ldr r1, _0802ABEC @ =gStaticData_08178F60
	lsls r0, r0, #2
	adds r1, r0, r1
	ldr r1, [r1]
	str r1, [r2]
	ldr r2, _0802ABF0 @ =gUnknown_03001474
	ldr r1, _0802ABF4 @ =gStaticData_08178F70
	adds r0, r0, r1
	ldr r0, [r0]
	str r0, [r2]
	ldr r1, _0802ABF8 @ =gUnknown_03001478
	movs r0, #0
	str r0, [r1]
	bx lr
	.align 2, 0
_0802ABE8: .4byte gUnknown_03001470
_0802ABEC: .4byte gStaticData_08178F60
_0802ABF0: .4byte gUnknown_03001474
_0802ABF4: .4byte gStaticData_08178F70
_0802ABF8: .4byte gUnknown_03001478

	thumb_func_start sub_802ABFC
sub_802ABFC: @ 0x0802ABFC
	push {lr}
	ldr r1, _0802AC18 @ =gUnknown_03001464
	strb r0, [r1]
	ldr r0, _0802AC1C @ =gUnknown_03001470
	movs r1, #0
	str r1, [r0]
	ldr r0, _0802AC20 @ =gUnknown_03001474
	str r1, [r0]
	ldr r0, _0802AC24 @ =gUnknown_03001478
	str r1, [r0]
	bl sub_802AB34
	pop {r0}
	bx r0
	.align 2, 0
_0802AC18: .4byte gUnknown_03001464
_0802AC1C: .4byte gUnknown_03001470
_0802AC20: .4byte gUnknown_03001474
_0802AC24: .4byte gUnknown_03001478

	thumb_func_start sub_802AC28
sub_802AC28: @ 0x0802AC28
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	adds r7, r1, #0
	mov r8, r2
	adds r6, r3, #0
	ldr r1, [sp, #0x24]
	mov sb, r1
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	ldr r0, _0802AC68 @ =gUnknown_0300147C
	ldr r1, [r0]
	lsls r0, r5, #2
	adds r0, r0, r5
	lsls r0, r0, #3
	adds r0, r0, r1
	ldr r1, [r0, #0x20]
	adds r7, r7, r1
	ldr r0, [r0, #0x24]
	add r8, r0
	subs r0, r5, #1
	cmp r0, #0x26
	bls _0802AC5C
	b _0802B11C
_0802AC5C:
	lsls r0, r0, #2
	ldr r1, _0802AC6C @ =_0802AC70
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0802AC68: .4byte gUnknown_0300147C
_0802AC6C: .4byte _0802AC70
_0802AC70: @ jump table
	.4byte _0802AD94 @ case 0
	.4byte _0802B0E0 @ case 1
	.4byte _0802AD3C @ case 2
	.4byte _0802ADC4 @ case 3
	.4byte _0802AD64 @ case 4
	.4byte _0802AD64 @ case 5
	.4byte _0802AD64 @ case 6
	.4byte _0802AF14 @ case 7
	.4byte _0802AD0C @ case 8
	.4byte _0802AF88 @ case 9
	.4byte _0802AFB8 @ case 10
	.4byte _0802AE1C @ case 11
	.4byte _0802B08C @ case 12
	.4byte _0802B11C @ case 13
	.4byte _0802B11C @ case 14
	.4byte _0802B010 @ case 15
	.4byte _0802B11C @ case 16
	.4byte _0802B010 @ case 17
	.4byte _0802B11C @ case 18
	.4byte _0802B010 @ case 19
	.4byte _0802B11C @ case 20
	.4byte _0802ADF4 @ case 21
	.4byte _0802AFE8 @ case 22
	.4byte _0802AE44 @ case 23
	.4byte _0802B038 @ case 24
	.4byte _0802B11C @ case 25
	.4byte _0802B11C @ case 26
	.4byte _0802AE70 @ case 27
	.4byte _0802AE70 @ case 28
	.4byte _0802AE70 @ case 29
	.4byte _0802AE70 @ case 30
	.4byte _0802B11C @ case 31
	.4byte _0802B11C @ case 32
	.4byte _0802B11C @ case 33
	.4byte _0802AEA0 @ case 34
	.4byte _0802B114 @ case 35
	.4byte _0802B114 @ case 36
	.4byte _0802B114 @ case 37
	.4byte _0802B114 @ case 38
_0802AD0C:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	lsls r1, r5, #2
	adds r1, r1, r5
	lsls r1, r1, #3
	ldr r0, _0802AD34 @ =gUnknown_0300147C
	ldr r0, [r0]
	adds r1, r1, r0
	str r6, [sp]
	adds r0, r4, #0
	adds r2, r7, #0
	mov r3, r8
	bl sub_802CB34
	ldr r0, _0802AD38 @ =gStaticData_087E4EF4
	b _0802B106
	.align 2, 0
_0802AD34: .4byte gUnknown_0300147C
_0802AD38: .4byte gStaticData_087E4EF4
_0802AD3C:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802AD60 @ =gUnknown_0300147C
	lsls r2, r5, #2
	adds r2, r2, r5
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	ldr r2, [r1, #0x20]
	str r6, [sp]
	mov r3, r8
	bl sub_802D764
	b _0802B11E
	.align 2, 0
_0802AD60: .4byte gUnknown_0300147C
_0802AD64:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	lsls r1, r5, #2
	adds r1, r1, r5
	lsls r1, r1, #3
	ldr r0, _0802AD8C @ =gUnknown_0300147C
	ldr r0, [r0]
	adds r1, r1, r0
	str r6, [sp]
	adds r0, r4, #0
	adds r2, r7, #0
	mov r3, r8
	bl sub_802CB34
	ldr r0, _0802AD90 @ =gStaticData_087E4EB4
	b _0802B106
	.align 2, 0
_0802AD8C: .4byte gUnknown_0300147C
_0802AD90: .4byte gStaticData_087E4EB4
_0802AD94:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	lsls r1, r5, #2
	adds r1, r1, r5
	lsls r1, r1, #3
	ldr r0, _0802ADBC @ =gUnknown_0300147C
	ldr r0, [r0]
	adds r1, r1, r0
	str r6, [sp]
	adds r0, r4, #0
	adds r2, r7, #0
	mov r3, r8
	bl sub_802CB34
	ldr r0, _0802ADC0 @ =gStaticData_087E4F74
	b _0802B106
	.align 2, 0
_0802ADBC: .4byte gUnknown_0300147C
_0802ADC0: .4byte gStaticData_087E4F74
_0802ADC4:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	lsls r1, r5, #2
	adds r1, r1, r5
	lsls r1, r1, #3
	ldr r0, _0802ADEC @ =gUnknown_0300147C
	ldr r0, [r0]
	adds r1, r1, r0
	str r6, [sp]
	adds r0, r4, #0
	adds r2, r7, #0
	mov r3, r8
	bl sub_802CB34
	ldr r0, _0802ADF0 @ =gStaticData_087E4F14
	b _0802B106
	.align 2, 0
_0802ADEC: .4byte gUnknown_0300147C
_0802ADF0: .4byte gStaticData_087E4F14
_0802ADF4:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802AE18 @ =gUnknown_0300147C
	lsls r2, r5, #2
	adds r2, r2, r5
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	str r6, [sp]
	adds r2, r7, #0
	mov r3, r8
	bl sub_802CF0C
	b _0802B11E
	.align 2, 0
_0802AE18: .4byte gUnknown_0300147C
_0802AE1C:
	movs r0, #0x58
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802AE40 @ =gUnknown_0300147C
	lsls r2, r5, #2
	adds r2, r2, r5
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	str r6, [sp]
	adds r2, r7, #0
	mov r3, r8
	bl sub_802D648
	b _0802B11E
	.align 2, 0
_0802AE40: .4byte gUnknown_0300147C
_0802AE44:
	movs r0, #0x68
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802AE6C @ =gUnknown_0300147C
	lsls r2, r5, #2
	adds r2, r2, r5
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	str r6, [sp]
	mov r2, sb
	str r2, [sp, #4]
	adds r2, r7, #0
	mov r3, r8
	bl sub_802D0C8
	b _0802B11E
	.align 2, 0
_0802AE6C: .4byte gUnknown_0300147C
_0802AE70:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	lsls r1, r5, #2
	adds r1, r1, r5
	lsls r1, r1, #3
	ldr r0, _0802AE98 @ =gUnknown_0300147C
	ldr r0, [r0]
	adds r1, r1, r0
	str r6, [sp]
	adds r0, r4, #0
	adds r2, r7, #0
	mov r3, r8
	bl sub_802CB34
	ldr r0, _0802AE9C @ =gStaticData_087E4ED4
	b _0802B106
	.align 2, 0
_0802AE98: .4byte gUnknown_0300147C
_0802AE9C: .4byte gStaticData_087E4ED4
_0802AEA0:
	mov r0, sb
	bl sub_802AA80
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802AEDC
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	ldr r0, _0802AED4 @ =gUnknown_0300147C
	ldr r1, [r0]
	movs r0, #0x8c
	lsls r0, r0, #3
	adds r1, r1, r0
	str r6, [sp]
	adds r0, r4, #0
	adds r2, r7, #0
	mov r3, r8
	bl sub_802CB34
	ldr r0, _0802AED8 @ =gStaticData_087E4ED4
	b _0802B106
	.align 2, 0
_0802AED4: .4byte gUnknown_0300147C
_0802AED8: .4byte gStaticData_087E4ED4
_0802AEDC:
	movs r0, #0x58
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	lsls r1, r5, #2
	adds r1, r1, r5
	lsls r1, r1, #3
	ldr r0, _0802AF0C @ =gUnknown_0300147C
	ldr r0, [r0]
	adds r1, r1, r0
	str r6, [sp]
	adds r0, r4, #0
	adds r2, r7, #0
	mov r3, r8
	bl sub_802CB34
	ldr r0, _0802AF10 @ =gStaticData_087E4F34
	str r0, [r4, #0x50]
	mov r1, sb
	str r1, [r4, #0x54]
	adds r0, r4, #0
	b _0802B11E
	.align 2, 0
_0802AF0C: .4byte gUnknown_0300147C
_0802AF10: .4byte gStaticData_087E4F34
_0802AF14:
	mov r0, sb
	bl sub_802AA80
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802AF50
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	ldr r0, _0802AF48 @ =gUnknown_0300147C
	ldr r1, [r0]
	movs r2, #0x8c
	lsls r2, r2, #3
	adds r1, r1, r2
	str r6, [sp]
	adds r0, r4, #0
	adds r2, r7, #0
	mov r3, r8
	bl sub_802CB34
	ldr r0, _0802AF4C @ =gStaticData_087E4ED4
	b _0802B106
	.align 2, 0
_0802AF48: .4byte gUnknown_0300147C
_0802AF4C: .4byte gStaticData_087E4ED4
_0802AF50:
	movs r0, #0x58
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	lsls r1, r5, #2
	adds r1, r1, r5
	lsls r1, r1, #3
	ldr r0, _0802AF80 @ =gUnknown_0300147C
	ldr r0, [r0]
	adds r1, r1, r0
	str r6, [sp]
	adds r0, r4, #0
	adds r2, r7, #0
	mov r3, r8
	bl sub_802CB34
	ldr r0, _0802AF84 @ =gStaticData_087E4F34
	str r0, [r4, #0x50]
	mov r0, sb
	str r0, [r4, #0x54]
	adds r0, r4, #0
	b _0802B11E
	.align 2, 0
_0802AF80: .4byte gUnknown_0300147C
_0802AF84: .4byte gStaticData_087E4F34
_0802AF88:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	lsls r1, r5, #2
	adds r1, r1, r5
	lsls r1, r1, #3
	ldr r0, _0802AFB0 @ =gUnknown_0300147C
	ldr r0, [r0]
	adds r1, r1, r0
	str r6, [sp]
	adds r0, r4, #0
	adds r2, r7, #0
	mov r3, r8
	bl sub_802CB34
	ldr r0, _0802AFB4 @ =gStaticData_087E4F54
	b _0802B106
	.align 2, 0
_0802AFB0: .4byte gUnknown_0300147C
_0802AFB4: .4byte gStaticData_087E4F54
_0802AFB8:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	lsls r1, r5, #2
	adds r1, r1, r5
	lsls r1, r1, #3
	ldr r0, _0802AFE0 @ =gUnknown_0300147C
	ldr r0, [r0]
	adds r1, r1, r0
	str r6, [sp]
	adds r0, r4, #0
	adds r2, r7, #0
	mov r3, r8
	bl InitActorPart
	ldr r0, _0802AFE4 @ =gStaticData_087E4E94
	b _0802B106
	.align 2, 0
_0802AFE0: .4byte gUnknown_0300147C
_0802AFE4: .4byte gStaticData_087E4E94
_0802AFE8:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802B00C @ =gUnknown_0300147C
	lsls r2, r5, #2
	adds r2, r2, r5
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	str r6, [sp]
	adds r2, r7, #0
	mov r3, r8
	bl sub_802CDE4
	b _0802B11E
	.align 2, 0
_0802B00C: .4byte gUnknown_0300147C
_0802B010:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802B034 @ =gUnknown_0300147C
	lsls r2, r5, #2
	adds r2, r2, r5
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	str r6, [sp]
	adds r2, r7, #0
	mov r3, r8
	bl sub_802D1B8
	b _0802B11E
	.align 2, 0
_0802B034: .4byte gUnknown_0300147C
_0802B038:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r4, _0802B088 @ =gUnknown_0300147C
	ldr r1, [r4]
	movs r2, #0x82
	lsls r2, r2, #3
	adds r1, r1, r2
	ldr r2, [r1, #0x20]
	str r6, [sp]
	mov r3, r8
	bl sub_802D5D4
	movs r1, #1
	str r1, [r0, #0xc]
	ldr r1, [r0]
	ldrh r1, [r1, #0xc]
	movs r2, #0
	movs r3, #0
	strh r1, [r0, #0x10]
	strb r2, [r0, #0x12]
	str r3, [r0, #8]
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	lsls r2, r5, #2
	adds r2, r2, r5
	lsls r2, r2, #3
	ldr r1, [r4]
	adds r1, r1, r2
	ldr r2, [r1, #0x20]
	str r6, [sp]
	mov r3, r8
	bl sub_802D5D4
	b _0802B11E
	.align 2, 0
_0802B088: .4byte gUnknown_0300147C
_0802B08C:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r4, _0802B0DC @ =gUnknown_0300147C
	ldr r1, [r4]
	movs r2, #0x8c
	lsls r2, r2, #2
	adds r1, r1, r2
	ldr r2, [r1, #0x20]
	str r6, [sp]
	mov r3, r8
	bl sub_802CE38
	movs r1, #1
	str r1, [r0, #0xc]
	ldr r1, [r0]
	ldrh r1, [r1, #0xc]
	movs r2, #0
	movs r3, #0
	strh r1, [r0, #0x10]
	strb r2, [r0, #0x12]
	str r3, [r0, #8]
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	lsls r2, r5, #2
	adds r2, r2, r5
	lsls r2, r2, #3
	ldr r1, [r4]
	adds r1, r1, r2
	ldr r2, [r1, #0x20]
	str r6, [sp]
	mov r3, r8
	bl sub_802CE38
	b _0802B11E
	.align 2, 0
_0802B0DC: .4byte gUnknown_0300147C
_0802B0E0:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	lsls r1, r5, #2
	adds r1, r1, r5
	lsls r1, r1, #3
	ldr r0, _0802B10C @ =gUnknown_0300147C
	ldr r0, [r0]
	adds r1, r1, r0
	str r6, [sp]
	adds r0, r4, #0
	adds r2, r7, #0
	mov r3, r8
	bl InitActorPart
	ldr r0, _0802B110 @ =gStaticData_087E4E14
_0802B106:
	str r0, [r4, #0x50]
	adds r0, r4, #0
	b _0802B11E
	.align 2, 0
_0802B10C: .4byte gUnknown_0300147C
_0802B110: .4byte gStaticData_087E4E14
_0802B114:
	adds r0, r5, #0
	subs r0, #0x24
	bl sub_802ABC8
_0802B11C:
	movs r0, #0
_0802B11E:
	add sp, #8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_802B12C
sub_802B12C: @ 0x0802B12C
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	sub sp, #4
	adds r6, r0, #0
	mov r8, r1
	adds r5, r2, #0
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	ldr r0, _0802B16C @ =gUnknown_0300147C
	ldr r1, [r0]
	movs r0, #0xc8
	lsls r0, r0, #3
	adds r1, r1, r0
	str r5, [sp]
	adds r0, r4, #0
	adds r2, r6, #0
	mov r3, r8
	bl InitActorPart
	ldr r0, _0802B170 @ =gStaticData_087E4E34
	str r0, [r4, #0x50]
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802B16C: .4byte gUnknown_0300147C
_0802B170: .4byte gStaticData_087E4E34

	thumb_func_start sub_802B174
sub_802B174: @ 0x0802B174
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r6, r1, #0
	adds r4, r2, #0
	movs r0, #0x60
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802B1A4 @ =gUnknown_0300147C
	ldr r1, [r1]
	movs r2, #0xdc
	lsls r2, r2, #1
	adds r1, r1, r2
	str r4, [sp]
	adds r2, r5, #0
	adds r3, r6, #0
	bl sub_802C3E8
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802B1A4: .4byte gUnknown_0300147C

	thumb_func_start sub_802B1A8
sub_802B1A8: @ 0x0802B1A8
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	sub sp, #8
	adds r6, r0, #0
	mov r8, r1
	adds r4, r2, #0
	adds r5, r3, #0
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802B1E4 @ =gUnknown_0300147C
	ldr r1, [r1]
	movs r2, #0x87
	lsls r2, r2, #3
	adds r1, r1, r2
	str r4, [sp]
	str r5, [sp, #4]
	adds r2, r6, #0
	mov r3, r8
	bl sub_802D528
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0802B1E4: .4byte gUnknown_0300147C

	thumb_func_start ConstructAnimTableState
ConstructAnimTableState: @ 0x0802B1E8
	push {r4, r5, r6, lr}
	adds r6, r1, #0
	ldr r4, _0802B210 @ =gUnknown_0300147C
	str r0, [r4]
	ldr r5, _0802B214 @ =gUnknown_03000884
	movs r0, #0
	str r0, [r5]
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, [r4]
	adds r2, r6, #0
	bl ConstructActorPart
	str r0, [r5]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802B210: .4byte gUnknown_0300147C
_0802B214: .4byte gUnknown_03000884

	thumb_func_start sub_802B218
sub_802B218: @ 0x0802B218
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r6, r2, #0
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	ldrb r4, [r5]
	ldr r0, _0802B258 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802B25C
	ldrb r4, [r5, #1]
	cmp r4, #0xb
	beq _0802B276
	cmp r4, #3
	beq _0802B254
	cmp r4, #8
	beq _0802B254
	cmp r4, #0x1c
	beq _0802B254
	cmp r4, #0x1d
	beq _0802B254
	cmp r4, #0x1e
	beq _0802B254
	cmp r4, #0x1f
	beq _0802B254
	cmp r4, #0x23
	bne _0802B262
_0802B254:
	movs r4, #1
	b _0802B266
	.align 2, 0
_0802B258: .4byte gUnknown_030012C0
_0802B25C:
	cmp r1, #0
	beq _0802B262
	ldrb r4, [r5, #2]
_0802B262:
	cmp r4, #0
	beq _0802B276
_0802B266:
	cmp r4, #0x20
	beq _0802B276
	cmp r4, #0x21
	beq _0802B276
	cmp r4, #0x22
	beq _0802B276
	cmp r4, #0x3e
	bne _0802B27A
_0802B276:
	movs r0, #0
	b _0802B290
_0802B27A:
	ldr r1, [r5, #4]
	lsls r1, r1, #8
	ldr r2, [r5, #8]
	lsls r2, r2, #8
	ldr r3, [r5, #0xc]
	lsls r3, r3, #8
	adds r3, r3, r6
	str r5, [sp]
	adds r0, r4, #0
	bl sub_802AC28
_0802B290:
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1

	thumb_func_start ConstructActorPart
ConstructActorPart: @ 0x0802B298
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r3, _0802B2D4 @ =0xFFFFB000
	cmp r2, #0
	beq _0802B2A8
	movs r3, #0xa0
	lsls r3, r3, #6
_0802B2A8:
	str r2, [sp]
	adds r0, r4, #0
	movs r2, #0
	bl InitActorPart
	ldr r0, _0802B2D8 @ =gStaticData_087E4E54
	str r0, [r4, #0x50]
	adds r0, r4, #0
	bl sub_802B864
	ldr r2, [r4, #0x24]
	cmp r2, #0
	beq _0802B2DC
	movs r0, #0xd
	movs r1, #0xc
	str r0, [r4, #0x28]
	movs r2, #0
	str r2, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	adds r0, #0x90
	b _0802B2E4
	.align 2, 0
_0802B2D4: .4byte 0xFFFFB000
_0802B2D8: .4byte gStaticData_087E4E54
_0802B2DC:
	movs r0, #8
	str r0, [r4, #0xc]
	ldr r0, [r4]
	adds r0, #0x60
_0802B2E4:
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	ldr r0, _0802B330 @ =gUnknown_030014A4
	movs r1, #0
	str r1, [r0]
	ldr r0, _0802B334 @ =gUnknown_03001490
	str r1, [r0]
	ldr r0, _0802B338 @ =gUnknown_03001494
	str r1, [r0]
	ldr r0, _0802B33C @ =gUnknown_030014A3
	strb r1, [r0]
	ldr r0, _0802B340 @ =gUnknown_0300149C
	str r1, [r0]
	ldr r0, _0802B344 @ =gUnknown_03001498
	str r1, [r0]
	ldr r0, _0802B348 @ =gUnknown_0300148C
	str r1, [r0]
	ldr r2, _0802B34C @ =gUnknown_030014A0
	movs r0, #1
	strb r0, [r2]
	ldr r0, _0802B350 @ =gUnknown_030014A2
	strb r1, [r0]
	ldr r0, _0802B354 @ =gUnknown_030014A1
	strb r1, [r0]
	ldr r0, _0802B358 @ =gUnknown_03001488
	str r1, [r0]
	ldr r0, _0802B35C @ =gUnknown_03001484
	str r1, [r0]
	ldr r0, _0802B360 @ =gUnknown_03001480
	strb r1, [r0]
	adds r0, r4, #0
	add sp, #4
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0802B330: .4byte gUnknown_030014A4
_0802B334: .4byte gUnknown_03001490
_0802B338: .4byte gUnknown_03001494
_0802B33C: .4byte gUnknown_030014A3
_0802B340: .4byte gUnknown_0300149C
_0802B344: .4byte gUnknown_03001498
_0802B348: .4byte gUnknown_0300148C
_0802B34C: .4byte gUnknown_030014A0
_0802B350: .4byte gUnknown_030014A2
_0802B354: .4byte gUnknown_030014A1
_0802B358: .4byte gUnknown_03001488
_0802B35C: .4byte gUnknown_03001484
_0802B360: .4byte gUnknown_03001480

	thumb_func_start sub_802B364
sub_802B364: @ 0x0802B364
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	bl sub_802BC68
	ldr r1, _0802B3CC @ =gUnknown_0300148C
	ldr r0, [r1]
	cmp r0, #0
	beq _0802B39E
	subs r5, r0, #1
	str r5, [r1]
	cmp r5, #0
	bne _0802B39E
	ldr r1, _0802B3D0 @ =gUnknown_030014A1
	movs r0, #1
	strb r0, [r1]
	movs r0, #0
	bl sub_8029BAC
	movs r0, #0xa
	str r0, [r4, #0x28]
	str r5, [r4, #0x44]
	str r0, [r4, #0xc]
	ldr r0, [r4]
	adds r0, #0x78
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
_0802B39E:
	ldr r2, _0802B3D4 @ =gUnknown_0300149C
	ldr r0, [r2]
	cmp r0, #0
	beq _0802B3E0
	subs r1, r0, #1
	str r1, [r2]
	cmp r1, #0
	beq _0802B3E0
	ldr r0, _0802B3D8 @ =gUnknown_030014A0
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802B3E0
	ldr r0, _0802B3DC @ =gUnknown_030012C0
	ldr r0, [r0]
	ldr r0, [r0, #0x78]
	cmp r0, #3
	beq _0802B3E0
	lsrs r0, r1, #2
	movs r1, #1
	ands r0, r1
	adds r1, r4, #0
	adds r1, #0x2c
	b _0802B3E6
	.align 2, 0
_0802B3CC: .4byte gUnknown_0300148C
_0802B3D0: .4byte gUnknown_030014A1
_0802B3D4: .4byte gUnknown_0300149C
_0802B3D8: .4byte gUnknown_030014A0
_0802B3DC: .4byte gUnknown_030012C0
_0802B3E0:
	adds r1, r4, #0
	adds r1, #0x2c
	movs r0, #1
_0802B3E6:
	strb r0, [r1]
	ldr r0, _0802B498 @ =gUnknown_030014A1
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802B402
	movs r0, #0xbc
	lsls r0, r0, #6
	str r0, [r4, #0x34]
	bl sub_8029B2C
	lsls r0, r0, #8
	ldr r1, [r4, #0x34]
	subs r0, r0, r1
	str r0, [r4, #0x24]
_0802B402:
	ldr r3, [r4, #0x34]
	asrs r3, r3, #1
	movs r0, #0xff
	lsls r0, r0, #7
	ands r3, r0
	ldr r1, [r4, #0x20]
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	ldr r0, [r4, #0x1c]
	asrs r2, r0, #0x1f
	eors r0, r2
	subs r0, r0, r2
	adds r1, r1, r0
	asrs r1, r1, #0xb
	movs r0, #0x7f
	ands r1, r0
	orrs r3, r1
	str r3, [r4, #0x14]
	ldr r0, [r4, #0x44]
	adds r0, #1
	str r0, [r4, #0x44]
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
	blt _0802B468
	movs r5, #6
	ldrsh r0, [r1, r5]
	subs r0, r2, r0
	lsls r0, r0, #8
	ldr r1, [r4, #8]
	subs r1, r1, r0
	str r1, [r4, #8]
	movs r0, #1
	strb r0, [r4, #0x12]
_0802B468:
	ldr r0, [r4, #0x1c]
	ldr r1, [r4, #0x20]
	bl sub_8029D8C
	ldr r3, _0802B49C @ =gStaticData_0817A6B8
	ldr r0, [r4, #0x28]
	lsls r1, r0, #3
	adds r0, r1, r3
	movs r5, #2
	ldrsh r2, [r0, r5]
	cmp r2, #0
	ble _0802B4A0
	movs r1, #4
	ldrsh r0, [r0, r1]
	adds r0, r4, r0
	ldr r1, [r0]
	lsls r0, r2, #3
	adds r0, r0, r1
	subs r0, #8
	ldr r6, [r0]
	ldr r7, [r0, #4]
	adds r3, r7, #0
	b _0802B4A6
	.align 2, 0
_0802B498: .4byte gUnknown_030014A1
_0802B49C: .4byte gStaticData_0817A6B8
_0802B4A0:
	adds r0, r3, #4
	adds r0, r1, r0
	ldr r3, [r0]
_0802B4A6:
	ldr r1, _0802B4C0 @ =gStaticData_0817A6B8
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r1
	movs r5, #0
	ldrsh r1, [r0, r5]
	cmp r2, #0
	ble _0802B4C4
	lsls r0, r6, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _0802B4C6
	.align 2, 0
_0802B4C0: .4byte gStaticData_0817A6B8
_0802B4C4:
	adds r0, r1, #0
_0802B4C6:
	adds r0, r4, r0
	bl sub_803AD84
	ldr r0, _0802B4F8 @ =gUnknown_030014A3
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802B56C
	ldr r0, _0802B4FC @ =gUnknown_030007E0
	ldr r2, [r0]
	movs r1, #0x20
	adds r0, r2, #0
	ands r0, r1
	cmp r0, #0
	beq _0802B524
	ldr r0, _0802B500 @ =gUnknown_03001498
	ldr r1, [r0]
	adds r2, r1, #0
	adds r1, #1
	str r1, [r0]
	cmp r2, #0xc
	ble _0802B508
	ldr r0, [r4, #0x1c]
	ldr r1, _0802B504 @ =0xFFFFFC80
	adds r0, r0, r1
	b _0802B50E
	.align 2, 0
_0802B4F8: .4byte gUnknown_030014A3
_0802B4FC: .4byte gUnknown_030007E0
_0802B500: .4byte gUnknown_03001498
_0802B504: .4byte 0xFFFFFC80
_0802B508:
	ldr r0, [r4, #0x1c]
	ldr r3, _0802B51C @ =0xFFFFFD33
	adds r0, r0, r3
_0802B50E:
	str r0, [r4, #0x1c]
	ldr r0, [r4, #0x1c]
	ldr r1, _0802B520 @ =0xFFFFCE00
	cmp r0, r1
	bge _0802B56C
	str r1, [r4, #0x1c]
	b _0802B56C
	.align 2, 0
_0802B51C: .4byte 0xFFFFFD33
_0802B520: .4byte 0xFFFFCE00
_0802B524:
	movs r0, #0x10
	ands r2, r0
	lsls r0, r2, #0x10
	lsrs r1, r0, #0x10
	cmp r1, #0
	beq _0802B568
	ldr r0, _0802B548 @ =gUnknown_03001498
	ldr r1, [r0]
	adds r2, r1, #0
	adds r1, #1
	str r1, [r0]
	cmp r2, #0xc
	ble _0802B54C
	ldr r0, [r4, #0x1c]
	movs r5, #0xe0
	lsls r5, r5, #2
	adds r0, r0, r5
	b _0802B552
	.align 2, 0
_0802B548: .4byte gUnknown_03001498
_0802B54C:
	ldr r0, [r4, #0x1c]
	ldr r1, _0802B564 @ =0x000002CD
	adds r0, r0, r1
_0802B552:
	str r0, [r4, #0x1c]
	ldr r0, [r4, #0x1c]
	movs r1, #0xc8
	lsls r1, r1, #6
	cmp r0, r1
	ble _0802B56C
	str r1, [r4, #0x1c]
	b _0802B56C
	.align 2, 0
_0802B564: .4byte 0x000002CD
_0802B568:
	ldr r0, _0802B580 @ =gUnknown_03001498
	str r1, [r0]
_0802B56C:
	ldr r5, _0802B584 @ =gUnknown_03001494
	ldr r0, [r5]
	cmp r0, #0
	beq _0802B588
	ldr r1, [r4, #0x1c]
	ldr r2, [r4, #0x20]
	ldr r3, [r4, #0x24]
	bl sub_802D3A8
	b _0802B5AA
	.align 2, 0
_0802B580: .4byte gUnknown_03001498
_0802B584: .4byte gUnknown_03001494
_0802B588:
	ldr r0, _0802B5B0 @ =gUnknown_030012C0
	ldr r0, [r0]
	ldr r3, [r0, #0x78]
	ldr r0, [r4, #0x1c]
	ldr r1, [r4, #0x20]
	ldr r2, [r4, #0x24]
	bl sub_802B1A8
	str r0, [r5]
	bl sub_8029794
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802B5AA
	ldr r0, [r5]
	bl sub_802D4EC
_0802B5AA:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802B5B0: .4byte gUnknown_030012C0

	thumb_func_start sub_802B5B4
sub_802B5B4: @ 0x0802B5B4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	adds r7, r0, #0
	movs r0, #0
	str r0, [sp, #4]
	ldr r2, [r7, #8]
	asrs r2, r2, #8
	ldr r1, [r7, #0xc]
	ldr r3, [r7]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r3
	movs r1, #2
	ldrsh r0, [r0, r1]
	adds r0, r0, r2
	ldr r1, [r7, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	mov sl, r0
	ldrb r0, [r0]
	str r0, [sp, #8]
	lsls r0, r0, #2
	mov sb, r0
	mov r1, sl
	ldrb r1, [r1, #1]
	str r1, [sp, #0xc]
	lsls r1, r1, #2
	mov r8, r1
	ldr r0, [r7, #0x30]
	ldr r4, [r7, #0x34]
	ldr r1, [r0, #0x10]
	cmp r4, r1
	bne _0802B61E
	movs r0, #0x80
	lsls r0, r0, #1
	str r0, [sp]
	bl sub_8029E98
	ldr r1, [r7, #0x20]
	adds r1, r1, r0
	asrs r5, r1, #8
	bl sub_8029EB4
	ldr r1, [r7, #0x1c]
	adds r1, r1, r0
	asrs r6, r1, #8
	b _0802B66E
_0802B61E:
	lsls r0, r4, #8
	bl sub_803ADB4
	str r0, [sp]
	movs r0, #0xbc
	lsls r0, r0, #0x12
	adds r1, r4, #0
	bl sub_803ADB4
	adds r4, r0, #0
	bl sub_8029E98
	ldr r1, [r7, #0x20]
	muls r1, r4, r1
	asrs r1, r1, #0xc
	adds r1, r1, r0
	asrs r5, r1, #8
	bl sub_8029EB4
	ldr r1, [r7, #0x1c]
	muls r1, r4, r1
	asrs r1, r1, #0xc
	adds r1, r1, r0
	asrs r6, r1, #8
	movs r1, #0x80
	lsls r1, r1, #1
	str r1, [sp, #4]
	ldr r0, [sp]
	cmp r0, #0xff
	bgt _0802B66E
	movs r0, #0x80
	lsls r0, r0, #2
	orrs r1, r0
	str r1, [sp, #4]
	ldr r1, [sp, #8]
	lsls r1, r1, #3
	mov sb, r1
	ldr r0, [sp, #0xc]
	lsls r0, r0, #3
	mov r8, r0
_0802B66E:
	mov r1, sb
	subs r6, r6, r1
	mov r0, r8
	subs r5, r5, r0
	cmp r5, #0x9f
	bgt _0802B706
	lsls r0, r0, #1
	adds r0, r5, r0
	cmp r0, #0
	blt _0802B706
	cmp r6, #0xef
	bgt _0802B706
	lsls r0, r1, #1
	adds r0, r6, r0
	cmp r0, #0
	blt _0802B706
	ldr r1, [r7, #0xc]
	ldr r2, [r7]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrh r0, [r0, #8]
	lsls r4, r0, #0x10
	mov r0, sl
	bl sub_8029108
	movs r1, #0xff
	ands r5, r1
	ldr r1, _0802B718 @ =0x000001FF
	ands r6, r1
	lsls r1, r6, #0x10
	orrs r5, r1
	orrs r5, r4
	orrs r5, r0
	ldr r1, [sp, #4]
	orrs r1, r5
	str r1, [sp, #4]
	ldr r4, _0802B71C @ =gUnknown_030014AC
	ldr r0, [r4]
	cmp sl, r0
	beq _0802B6E2
	ldr r2, _0802B720 @ =gUnknown_030014A8
	ldr r0, [r2]
	movs r1, #1
	eors r0, r1
	str r0, [r2]
	ldr r2, _0802B724 @ =gUnknown_03000874
	ldr r1, _0802B728 @ =gUnknown_030014B0
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r2, [r2]
	mov r1, sl
	bl sub_803AD80
	mov r0, sl
	str r0, [r4]
_0802B6E2:
	ldr r1, _0802B728 @ =gUnknown_030014B0
	ldr r0, _0802B720 @ =gUnknown_030014A8
	ldr r0, [r0]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, _0802B72C @ =0xF9FF0000
	adds r0, r0, r1
	lsrs r0, r0, #5
	ldr r1, [r7, #0x18]
	lsls r1, r1, #0xc
	orrs r1, r0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	ldr r0, [sp, #4]
	ldr r2, [sp]
	bl sub_8028DD8
_0802B706:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802B718: .4byte 0x000001FF
_0802B71C: .4byte gUnknown_030014AC
_0802B720: .4byte gUnknown_030014A8
_0802B724: .4byte gUnknown_03000874
_0802B728: .4byte gUnknown_030014B0
_0802B72C: .4byte 0xF9FF0000

	thumb_func_start sub_802B730
sub_802B730: @ 0x0802B730
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r1, _0802B740 @ =gUnknown_0300149C
	ldr r0, [r1]
	cmp r0, #0
	beq _0802B744
	movs r0, #1
	b _0802B7D8
	.align 2, 0
_0802B740: .4byte gUnknown_0300149C
_0802B744:
	ldr r2, _0802B7AC @ =gUnknown_03001494
	ldr r7, _0802B7B0 @ =gUnknown_030012C0
	ldr r0, [r7]
	ldr r5, [r0, #0x78]
	cmp r5, #0
	bne _0802B7CC
	ldr r0, _0802B7B4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x1b
	bl PlaySfx
	ldr r0, _0802B7B8 @ =gStaticData_0817A728
	ldr r1, _0802B7BC @ =0x05000200
	movs r2, #0x20
	movs r3, #0x10
	bl QueueVramDmaTransfer
	movs r0, #6
	movs r1, #5
	str r0, [r4, #0x28]
	str r5, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0x3c]
	movs r6, #0
	strh r0, [r4, #0x10]
	strb r6, [r4, #0x12]
	str r5, [r4, #8]
	ldr r0, _0802B7C0 @ =gUnknown_03001480
	movs r4, #1
	strb r4, [r0]
	ldr r1, [r7]
	adds r0, r1, #0
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802B798
	adds r0, r1, #0
	bl sub_8023234
_0802B798:
	ldr r0, _0802B7C4 @ =gUnknown_030014A3
	strb r6, [r0]
	ldr r0, _0802B7C8 @ =gUnknown_030014A0
	strb r4, [r0]
	movs r0, #0
	bl sub_8029BAC
	bl sub_802DFBC
	b _0802B7D6
	.align 2, 0
_0802B7AC: .4byte gUnknown_03001494
_0802B7B0: .4byte gUnknown_030012C0
_0802B7B4: .4byte gUnknown_030012BC
_0802B7B8: .4byte gStaticData_0817A728
_0802B7BC: .4byte 0x05000200
_0802B7C0: .4byte gUnknown_03001480
_0802B7C4: .4byte gUnknown_030014A3
_0802B7C8: .4byte gUnknown_030014A0
_0802B7CC:
	movs r0, #0x4b
	str r0, [r1]
	ldr r0, [r2]
	bl sub_802D4B0
_0802B7D6:
	movs r0, #0
_0802B7D8:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_802B7E0
sub_802B7E0: @ 0x0802B7E0
	push {r4, lr}
	adds r2, r0, #0
	ldr r1, _0802B7F0 @ =gUnknown_0300149C
	ldr r0, [r1]
	cmp r0, #0
	beq _0802B7F4
	movs r0, #1
	b _0802B85C
	.align 2, 0
_0802B7F0: .4byte gUnknown_0300149C
_0802B7F4:
	ldr r4, _0802B83C @ =gUnknown_03001494
	ldr r0, _0802B840 @ =gUnknown_030012C0
	ldr r0, [r0]
	ldr r3, [r0, #0x78]
	cmp r3, #0
	bne _0802B850
	movs r0, #0xc
	movs r1, #0xb
	str r0, [r2, #0x28]
	str r3, [r2, #0x44]
	str r1, [r2, #0xc]
	ldr r0, [r2]
	adds r0, #0x84
	ldrh r0, [r0]
	movs r4, #0
	strh r0, [r2, #0x10]
	strb r4, [r2, #0x12]
	str r3, [r2, #8]
	ldr r0, _0802B844 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x33
	bl PlaySfx
	ldr r0, _0802B848 @ =gUnknown_030014A3
	strb r4, [r0]
	ldr r1, _0802B84C @ =gUnknown_030014A0
	movs r0, #1
	strb r0, [r1]
	movs r0, #0
	bl sub_8029BAC
	bl sub_802DFBC
	b _0802B85A
	.align 2, 0
_0802B83C: .4byte gUnknown_03001494
_0802B840: .4byte gUnknown_030012C0
_0802B844: .4byte gUnknown_030012BC
_0802B848: .4byte gUnknown_030014A3
_0802B84C: .4byte gUnknown_030014A0
_0802B850:
	movs r0, #0x4b
	str r0, [r1]
	ldr r0, [r4]
	bl sub_802D4B0
_0802B85A:
	movs r0, #0
_0802B85C:
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_802B864
sub_802B864: @ 0x0802B864
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r2, [r4, #8]
	asrs r2, r2, #8
	ldr r1, [r4, #0xc]
	ldr r3, [r4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r3
	movs r1, #2
	ldrsh r0, [r0, r1]
	adds r0, r0, r2
	ldr r1, [r4, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldrb r3, [r0]
	ldrb r1, [r0, #1]
	adds r2, r3, #0
	muls r2, r1, r2
	adds r0, r2, #0
	lsls r0, r0, #5
	bl sub_8028CD4
	ldr r5, _0802B8DC @ =gUnknown_030014B0
	str r0, [r5]
	ldr r2, [r4, #8]
	asrs r2, r2, #8
	ldr r1, [r4, #0xc]
	ldr r3, [r4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r3
	movs r3, #2
	ldrsh r0, [r0, r3]
	adds r0, r0, r2
	ldr r1, [r4, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldrb r2, [r0]
	ldrb r3, [r0, #1]
	adds r1, r2, #0
	muls r1, r3, r1
	adds r0, r1, #0
	lsls r0, r0, #5
	bl sub_8028CD4
	str r0, [r5, #4]
	ldr r1, _0802B8E0 @ =gUnknown_030014A8
	movs r0, #1
	str r0, [r1]
	ldr r1, _0802B8E4 @ =gUnknown_030014AC
	movs r0, #0
	str r0, [r1]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802B8DC: .4byte gUnknown_030014B0
_0802B8E0: .4byte gUnknown_030014A8
_0802B8E4: .4byte gUnknown_030014AC

	thumb_func_start sub_802B8E8
sub_802B8E8: @ 0x0802B8E8
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r6, _0802B980 @ =gUnknown_03001490
	ldr r5, [r6]
	cmp r5, #0
	bne _0802B918
	ldr r1, [r4, #0x1c]
	movs r2, #0xa0
	lsls r2, r2, #6
	ldr r3, [r4, #0x24]
	str r5, [sp]
	movs r0, #2
	bl sub_802AC28
	str r0, [r6]
	movs r1, #1
	str r1, [r0, #0xc]
	ldr r1, [r0]
	ldrh r1, [r1, #0xc]
	movs r2, #0
	strh r1, [r0, #0x10]
	strb r2, [r0, #0x12]
	str r5, [r0, #8]
_0802B918:
	ldr r2, _0802B984 @ =gUnknown_030014A4
	ldr r1, [r4, #0x20]
	ldr r0, [r2]
	adds r1, r1, r0
	str r1, [r4, #0x20]
	adds r0, #0x2d
	str r0, [r2]
	movs r5, #0xa0
	lsls r5, r5, #6
	cmp r1, r5
	ble _0802B978
	ldr r0, _0802B988 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x35
	bl PlaySfx
	str r5, [r4, #0x20]
	movs r0, #9
	str r0, [r4, #0x28]
	movs r5, #0
	str r5, [r4, #0x44]
	str r0, [r4, #0xc]
	ldr r0, [r4]
	adds r0, #0x6c
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
	ldr r0, _0802B98C @ =gUnknown_030014A0
	strb r1, [r0]
	ldr r2, [r6]
	cmp r2, #0
	beq _0802B970
	ldr r1, [r2, #0x50]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_0802B970:
	str r5, [r6]
	movs r0, #0x19
	bl sub_8029BAC
_0802B978:
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802B980: .4byte gUnknown_03001490
_0802B984: .4byte gUnknown_030014A4
_0802B988: .4byte gUnknown_030012BC
_0802B98C: .4byte gUnknown_030014A0

	thumb_func_start sub_802B990
sub_802B990: @ 0x0802B990
	push {r4, r5, lr}
	adds r4, r0, #0
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _0802B9DC
	movs r0, #0x24
	bl sub_8029BAC
	ldr r5, [r4, #0xc]
	cmp r5, #0
	beq _0802B9B8
	movs r2, #0
	str r2, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	b _0802B9DC
_0802B9B8:
	movs r0, #3
	bl sub_8000E1C
	lsls r0, r0, #0x10
	cmp r0, #0
	bne _0802B9CE
	movs r0, #1
	str r0, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	b _0802B9D4
_0802B9CE:
	str r5, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0]
_0802B9D4:
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
_0802B9DC:
	ldr r0, _0802BA48 @ =gUnknown_030014A3
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802BA42
	ldr r5, _0802BA4C @ =gUnknown_030007E0
	movs r0, #1
	ldrh r1, [r5, #2]
	ands r0, r1
	cmp r0, #0
	beq _0802BA1C
	movs r0, #4
	movs r1, #3
	str r0, [r4, #0x28]
	movs r2, #0
	str r2, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0x24]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	ldr r0, _0802BA50 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xd
	bl PlaySfx
	ldr r1, _0802BA54 @ =gUnknown_030014A4
	ldr r0, _0802BA58 @ =0xFFFFF880
	str r0, [r1]
_0802BA1C:
	ldr r0, [r5]
	movs r1, #2
	ands r0, r1
	cmp r0, #0
	beq _0802BA42
	movs r0, #2
	str r0, [r4, #0x28]
	movs r2, #0
	str r2, [r4, #0x44]
	str r0, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0x18]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	movs r0, #0x38
	bl sub_8029BAC
_0802BA42:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802BA48: .4byte gUnknown_030014A3
_0802BA4C: .4byte gUnknown_030007E0
_0802BA50: .4byte gUnknown_030012BC
_0802BA54: .4byte gUnknown_030014A4
_0802BA58: .4byte 0xFFFFF880

	thumb_func_start sub_802BA5C
sub_802BA5C: @ 0x0802BA5C
	push {lr}
	adds r3, r0, #0
	ldr r2, _0802BAC4 @ =gUnknown_030014A4
	ldr r0, [r3, #0x20]
	ldr r1, [r2]
	adds r0, r0, r1
	str r0, [r3, #0x20]
	adds r1, #0x60
	str r1, [r2]
	movs r0, #0xf0
	lsls r0, r0, #3
	cmp r1, r0
	ble _0802BA78
	str r0, [r2]
_0802BA78:
	ldr r0, [r3, #0x44]
	cmp r0, #0xa
	bgt _0802BA94
	ldr r0, _0802BAC8 @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	bne _0802BA94
	ldr r0, [r2]
	ldr r1, _0802BACC @ =0xFFFFFC00
	cmp r0, r1
	bge _0802BA94
	str r1, [r2]
_0802BA94:
	ldr r0, [r3, #0x20]
	movs r1, #0xa0
	lsls r1, r1, #6
	cmp r0, r1
	ble _0802BABE
	str r1, [r3, #0x20]
	movs r0, #1
	movs r1, #4
	str r0, [r3, #0x28]
	movs r2, #0
	str r2, [r3, #0x44]
	str r1, [r3, #0xc]
	ldr r0, [r3]
	ldrh r0, [r0, #0x30]
	movs r1, #0
	strh r0, [r3, #0x10]
	strb r1, [r3, #0x12]
	str r2, [r3, #8]
	movs r0, #0x24
	bl sub_8029BAC
_0802BABE:
	pop {r0}
	bx r0
	.align 2, 0
_0802BAC4: .4byte gUnknown_030014A4
_0802BAC8: .4byte gUnknown_030007E0
_0802BACC: .4byte 0xFFFFFC00

	thumb_func_start sub_802BAD0
sub_802BAD0: @ 0x0802BAD0
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r5, _0802BB3C @ =gUnknown_030007E0
	ldr r0, [r5]
	movs r1, #2
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r2, r0, #0x10
	cmp r2, #0
	bne _0802BAFE
	movs r0, #1
	str r0, [r4, #0x28]
	str r2, [r4, #0x44]
	str r2, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	movs r0, #0x24
	bl sub_8029BAC
_0802BAFE:
	movs r0, #1
	ldrh r5, [r5, #2]
	ands r0, r5
	cmp r0, #0
	beq _0802BB34
	movs r0, #4
	movs r1, #3
	str r0, [r4, #0x28]
	movs r2, #0
	str r2, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0x24]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	ldr r0, _0802BB40 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xd
	bl PlaySfx
	ldr r1, _0802BB44 @ =gUnknown_030014A4
	ldr r0, _0802BB48 @ =0xFFFFF880
	str r0, [r1]
_0802BB34:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802BB3C: .4byte gUnknown_030007E0
_0802BB40: .4byte gUnknown_030012BC
_0802BB44: .4byte gUnknown_030014A4
_0802BB48: .4byte 0xFFFFF880

	thumb_func_start sub_802BB4C
sub_802BB4C: @ 0x0802BB4C
	push {r4, lr}
	adds r4, r0, #0
	ldr r1, [r4, #0x44]
	cmp r1, #0x2c
	ble _0802BBA8
	ldr r0, _0802BB98 @ =gStaticData_0817A728
	ldr r1, _0802BB9C @ =0x05000200
	movs r2, #0x20
	movs r3, #0x10
	bl QueueVramDmaTransfer
	ldr r1, _0802BBA0 @ =gUnknown_03001480
	movs r0, #1
	strb r0, [r1]
	movs r0, #6
	movs r1, #5
	str r0, [r4, #0x28]
	movs r2, #0
	str r2, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0x3c]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	ldr r0, _0802BBA4 @ =gUnknown_030012C0
	ldr r1, [r0]
	adds r0, r1, #0
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802BBD4
	adds r0, r1, #0
	bl sub_8023234
	b _0802BBD4
	.align 2, 0
_0802BB98: .4byte gStaticData_0817A728
_0802BB9C: .4byte 0x05000200
_0802BBA0: .4byte gUnknown_03001480
_0802BBA4: .4byte gUnknown_030012C0
_0802BBA8:
	movs r0, #4
	ands r1, r0
	cmp r1, #0
	beq _0802BBC8
	ldr r0, _0802BBC0 @ =gStaticData_0817A728
	ldr r1, _0802BBC4 @ =0x05000200
	movs r2, #0x20
	movs r3, #0x10
	bl QueueVramDmaTransfer
	b _0802BBD4
	.align 2, 0
_0802BBC0: .4byte gStaticData_0817A728
_0802BBC4: .4byte 0x05000200
_0802BBC8:
	ldr r0, _0802BBDC @ =gStaticData_0817A748
	ldr r1, _0802BBE0 @ =0x05000200
	movs r2, #0x20
	movs r3, #0x10
	bl QueueVramDmaTransfer
_0802BBD4:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0802BBDC: .4byte gStaticData_0817A748
_0802BBE0: .4byte 0x05000200

	thumb_func_start sub_802BBE4
sub_802BBE4: @ 0x0802BBE4
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _0802BC4A
	ldr r1, _0802BC54 @ =gUnknown_030014A4
	ldr r0, _0802BC58 @ =0xFFFFF980
	str r0, [r1]
	ldr r1, [r4, #0x20]
	movs r0, #0x80
	lsls r0, r0, #6
	cmp r1, r0
	ble _0802BC16
	ldr r1, [r4, #0x1c]
	movs r2, #0xa0
	lsls r2, r2, #6
	ldr r3, [r4, #0x24]
	movs r0, #0
	str r0, [sp]
	movs r0, #2
	bl sub_802AC28
	ldr r1, _0802BC5C @ =gUnknown_03001490
	str r0, [r1]
_0802BC16:
	movs r0, #8
	movs r1, #7
	str r0, [r4, #0x28]
	movs r2, #0
	str r2, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	adds r0, #0x54
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	ldr r1, _0802BC60 @ =gUnknown_03001480
	movs r0, #1
	strb r0, [r1]
	ldr r0, _0802BC64 @ =gUnknown_030012C0
	ldr r1, [r0]
	adds r0, r1, #0
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802BC4A
	adds r0, r1, #0
	bl sub_8023234
_0802BC4A:
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0802BC54: .4byte gUnknown_030014A4
_0802BC58: .4byte 0xFFFFF980
_0802BC5C: .4byte gUnknown_03001490
_0802BC60: .4byte gUnknown_03001480
_0802BC64: .4byte gUnknown_030012C0

	thumb_func_start sub_802BC68
sub_802BC68: @ 0x0802BC68
	push {r4, r5, lr}
	adds r1, r0, #0
	ldr r4, _0802BC90 @ =gUnknown_03001488
	ldr r2, [r4]
	cmp r2, #0
	beq _0802BD0E
	ldr r0, _0802BC94 @ =gUnknown_030014A0
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802BC9C
	ldr r5, _0802BC98 @ =gUnknown_030012C0
_0802BC7E:
	ldr r0, [r5]
	bl sub_8023430
	ldr r0, [r4]
	subs r0, #1
	str r0, [r4]
	cmp r0, #0
	bne _0802BC7E
	b _0802BD0E
	.align 2, 0
_0802BC90: .4byte gUnknown_03001488
_0802BC94: .4byte gUnknown_030014A0
_0802BC98: .4byte gUnknown_030012C0
_0802BC9C:
	ldr r3, _0802BCAC @ =gUnknown_03001484
	ldr r0, [r3]
	cmp r0, #0
	beq _0802BCB0
	subs r0, #1
	str r0, [r3]
	b _0802BD0E
	.align 2, 0
_0802BCAC: .4byte gUnknown_03001484
_0802BCB0:
	movs r0, #0xf
	str r0, [r3]
	cmp r2, #9
	bgt _0802BCC8
	ldr r0, [r1, #0x1c]
	ldr r1, [r1, #0x20]
	movs r2, #1
	bl sub_802B174
	ldr r0, [r4]
	subs r0, #1
	b _0802BCFE
_0802BCC8:
	cmp r2, #0x13
	bgt _0802BCDC
	ldr r0, [r1, #0x1c]
	ldr r1, [r1, #0x20]
	movs r2, #2
	bl sub_802B174
	ldr r0, [r4]
	subs r0, #2
	b _0802BCFE
_0802BCDC:
	cmp r2, #0x27
	bgt _0802BCF0
	ldr r0, [r1, #0x1c]
	ldr r1, [r1, #0x20]
	movs r2, #4
	bl sub_802B174
	ldr r0, [r4]
	subs r0, #4
	b _0802BCFE
_0802BCF0:
	ldr r0, [r1, #0x1c]
	ldr r1, [r1, #0x20]
	movs r2, #8
	bl sub_802B174
	ldr r0, [r4]
	subs r0, #8
_0802BCFE:
	str r0, [r4]
	ldr r0, _0802BD14 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #8
	bl PlaySfx
_0802BD0E:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802BD14: .4byte gUnknown_030012BC

	thumb_func_start sub_802BD18
sub_802BD18: @ 0x0802BD18
	ldr r0, _0802BD20 @ =gUnknown_03001480
	ldrb r0, [r0]
	bx lr
	.align 2, 0
_0802BD20: .4byte gUnknown_03001480

	thumb_func_start sub_802BD24
sub_802BD24: @ 0x0802BD24
	push {lr}
	adds r3, r0, #0
	ldr r0, [r3, #0x44]
	cmp r0, #0x13
	ble _0802BD56
	ldr r1, _0802BD5C @ =gUnknown_030014A3
	movs r0, #1
	strb r0, [r1]
	ldr r1, _0802BD60 @ =gUnknown_030014A0
	movs r0, #0
	strb r0, [r1]
	movs r0, #1
	movs r2, #0
	str r0, [r3, #0x28]
	str r2, [r3, #0x44]
	str r2, [r3, #0xc]
	ldr r0, [r3]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r3, #0x10]
	strb r1, [r3, #0x12]
	str r2, [r3, #8]
	movs r0, #0x24
	bl sub_8029BAC
_0802BD56:
	pop {r0}
	bx r0
	.align 2, 0
_0802BD5C: .4byte gUnknown_030014A3
_0802BD60: .4byte gUnknown_030014A0

	thumb_func_start sub_802BD64
sub_802BD64: @ 0x0802BD64
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r2, _0802BDBC @ =gUnknown_030014A4
	ldr r0, [r4, #0x20]
	ldr r1, [r2]
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r1, #0x2d
	str r1, [r2]
	ldr r0, [r4, #0x24]
	adds r0, #0x3c
	str r0, [r4, #0x24]
	bl sub_8029B2C
	lsls r0, r0, #8
	ldr r1, [r4, #0x24]
	subs r1, r0, r1
	str r1, [r4, #0x34]
	ldr r5, _0802BDC0 @ =gUnknown_030014A2
	ldrb r0, [r5]
	cmp r0, #0
	bne _0802BDA8
	ldr r0, _0802BDC4 @ =0x000016FF
	cmp r1, r0
	bgt _0802BDA8
	movs r0, #0
	movs r1, #2
	movs r2, #1
	bl sub_800132C
	ldr r1, _0802BDC8 @ =gUnknown_03001480
	movs r0, #1
	strb r0, [r1]
	strb r0, [r5]
_0802BDA8:
	ldr r1, [r4, #0x34]
	ldr r0, _0802BDCC @ =0x000003FF
	cmp r1, r0
	bgt _0802BDB6
	movs r0, #1
	bl sub_802A668
_0802BDB6:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802BDBC: .4byte gUnknown_030014A4
_0802BDC0: .4byte gUnknown_030014A2
_0802BDC4: .4byte 0x000016FF
_0802BDC8: .4byte gUnknown_03001480
_0802BDCC: .4byte 0x000003FF

	thumb_func_start sub_802BDD0
sub_802BDD0: @ 0x0802BDD0
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r2, _0802BE24 @ =gUnknown_030014A4
	ldr r0, [r4, #0x20]
	ldr r1, [r2]
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r1, #0x2d
	str r1, [r2]
	ldr r0, [r4, #0x24]
	adds r0, #0x3c
	str r0, [r4, #0x24]
	bl sub_8029B2C
	lsls r0, r0, #8
	ldr r1, [r4, #0x24]
	subs r1, r0, r1
	str r1, [r4, #0x34]
	ldr r5, _0802BE28 @ =gUnknown_030014A2
	ldrb r0, [r5]
	cmp r0, #0
	bne _0802BE10
	ldr r0, _0802BE2C @ =0x000016FF
	cmp r1, r0
	bgt _0802BE10
	movs r0, #0
	movs r1, #2
	movs r2, #1
	bl sub_800132C
	movs r0, #1
	strb r0, [r5]
_0802BE10:
	ldr r1, [r4, #0x34]
	ldr r0, _0802BE30 @ =0x000003FF
	cmp r1, r0
	bgt _0802BE1E
	movs r0, #2
	bl sub_802A668
_0802BE1E:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802BE24: .4byte gUnknown_030014A4
_0802BE28: .4byte gUnknown_030014A2
_0802BE2C: .4byte 0x000016FF
_0802BE30: .4byte 0x000003FF

	thumb_func_start sub_802BE34
sub_802BE34: @ 0x0802BE34
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x20]
	ldr r2, _0802BE70 @ =0xFFFFFF00
	adds r1, r0, r2
	str r1, [r4, #0x20]
	ldr r5, _0802BE74 @ =gUnknown_030014A2
	ldrb r0, [r5]
	cmp r0, #0
	bne _0802BE5C
	ldr r0, _0802BE78 @ =0xFFFFC000
	cmp r1, r0
	bge _0802BE5C
	movs r0, #0
	movs r1, #2
	movs r2, #1
	bl sub_800132C
	movs r0, #1
	strb r0, [r5]
_0802BE5C:
	ldr r1, [r4, #0x20]
	ldr r0, _0802BE7C @ =0xFFFF8E00
	cmp r1, r0
	bge _0802BE6A
	movs r0, #3
	bl sub_802A668
_0802BE6A:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802BE70: .4byte 0xFFFFFF00
_0802BE74: .4byte gUnknown_030014A2
_0802BE78: .4byte 0xFFFFC000
_0802BE7C: .4byte 0xFFFF8E00

	thumb_func_start sub_802BE80
sub_802BE80: @ 0x0802BE80
	push {lr}
	adds r2, r0, #0
	ldr r0, [r2, #0x44]
	cmp r0, #0x1e
	bne _0802BED2
	ldr r1, _0802BEBC @ =gUnknown_030014A3
	movs r0, #1
	strb r0, [r1]
	ldr r0, _0802BEC0 @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #2
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	cmp r3, #0
	bne _0802BEC4
	movs r0, #1
	str r0, [r2, #0x28]
	str r3, [r2, #0x44]
	str r3, [r2, #0xc]
	ldr r0, [r2]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r2, #0x10]
	strb r1, [r2, #0x12]
	str r3, [r2, #8]
	movs r0, #0x24
	bl sub_8029BAC
	b _0802BED2
	.align 2, 0
_0802BEBC: .4byte gUnknown_030014A3
_0802BEC0: .4byte gUnknown_030007E0
_0802BEC4:
	movs r0, #2
	str r0, [r2, #0x28]
	movs r0, #0
	str r0, [r2, #0x44]
	movs r0, #0x38
	bl sub_8029BAC
_0802BED2:
	pop {r0}
	bx r0
	.align 2, 0

