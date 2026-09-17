.include "asm/macros.inc"

.syntax unified
.arm
	thumb_func_start sub_80306AC
sub_80306AC: @ 0x080306AC
	push {r4, r5, lr}
	ldr r2, _08030710 @ =gUnknown_03001548
	ldr r1, _08030714 @ =gUnknown_03001560
	ldr r0, [r2]
	ldr r1, [r1]
	adds r0, r0, r1
	str r0, [r2]
	ldr r0, _08030718 @ =gUnknown_03001554
	ldr r1, [r0]
	ldr r0, _0803071C @ =0x000081FF
	cmp r1, r0
	bgt _08030708
	ldr r1, _08030720 @ =gUnknown_03001558
	ldr r0, _08030724 @ =gUnknown_0300155C
	movs r5, #0
	str r5, [r0]
	str r5, [r1]
	movs r1, #2
	ldr r0, _08030728 @ =gUnknown_03001538
	str r1, [r0]
	ldr r0, _0803072C @ =gUnknown_0300153C
	str r5, [r0]
	ldr r0, _08030730 @ =gUnknown_03001534
	ldr r4, [r0]
	str r5, [r4, #0xc]
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
	movs r2, #4
	ldrsh r1, [r1, r2]
	cmp r0, r1
	blt _08030704
	str r5, [r4, #8]
_08030704:
	bl sub_802A4F8
_08030708:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08030710: .4byte gUnknown_03001548
_08030714: .4byte gUnknown_03001560
_08030718: .4byte gUnknown_03001554
_0803071C: .4byte 0x000081FF
_08030720: .4byte gUnknown_03001558
_08030724: .4byte gUnknown_0300155C
_08030728: .4byte gUnknown_03001538
_0803072C: .4byte gUnknown_0300153C
_08030730: .4byte gUnknown_03001534

	thumb_func_start sub_8030734
sub_8030734: @ 0x08030734
	push {r4, r5, lr}
	ldr r1, _0803074C @ =gUnknown_03001548
	ldr r4, _08030750 @ =gUnknown_03001560
	ldr r0, [r1]
	ldr r3, [r4]
	adds r0, r0, r3
	str r0, [r1]
	adds r2, r1, #0
	cmp r3, #0x98
	bgt _08030754
	adds r0, r3, #1
	b _08030756
	.align 2, 0
_0803074C: .4byte gUnknown_03001548
_08030750: .4byte gUnknown_03001560
_08030754:
	subs r0, r3, #1
_08030756:
	str r0, [r4]
	ldr r5, _08030790 @ =gUnknown_03001570
	ldr r4, [r5]
	cmp r4, #0
	bne _080307B0
	ldr r0, _08030794 @ =gUnknown_03001540
	ldr r0, [r0]
	ldr r1, _08030798 @ =0xFFFFF325
	adds r0, r0, r1
	ldr r1, _0803079C @ =gUnknown_03001544
	ldr r1, [r1]
	ldr r3, _080307A0 @ =0x0000516D
	adds r1, r1, r3
	ldr r2, [r2]
	subs r2, #0xa
	bl sub_802E62C
	ldr r3, _080307A4 @ =gUnknown_03001574
	ldr r1, [r3]
	adds r1, #1
	str r1, [r3]
	ldr r0, _080307A8 @ =gUnknown_03001568
	ldr r2, [r0]
	ldr r0, [r2, #8]
	cmp r1, r0
	bne _080307AC
	str r4, [r3]
	ldr r0, [r2, #0xc]
	b _080307B2
	.align 2, 0
_08030790: .4byte gUnknown_03001570
_08030794: .4byte gUnknown_03001540
_08030798: .4byte 0xFFFFF325
_0803079C: .4byte gUnknown_03001544
_080307A0: .4byte 0x0000516D
_080307A4: .4byte gUnknown_03001574
_080307A8: .4byte gUnknown_03001568
_080307AC:
	ldr r0, [r2, #4]
	b _080307B2
_080307B0:
	subs r0, r4, #1
_080307B2:
	str r0, [r5]
	bl sub_8030E08
	ldr r0, _08030814 @ =gUnknown_03001554
	ldr r1, [r0]
	ldr r0, _08030818 @ =0x000031FF
	cmp r1, r0
	bgt _08030808
	ldr r1, _0803081C @ =gUnknown_03001570
	ldr r0, _08030820 @ =gUnknown_03001568
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	str r0, [r1]
	ldr r0, _08030824 @ =gUnknown_03001574
	movs r5, #0
	str r5, [r0]
	movs r1, #3
	ldr r0, _08030828 @ =gUnknown_03001538
	str r1, [r0]
	ldr r0, _0803082C @ =gUnknown_0300153C
	str r5, [r0]
	ldr r0, _08030830 @ =gUnknown_03001534
	ldr r4, [r0]
	str r5, [r4, #0xc]
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
	movs r2, #4
	ldrsh r1, [r1, r2]
	cmp r0, r1
	blt _08030808
	str r5, [r4, #8]
_08030808:
	bl sub_803171C
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08030814: .4byte gUnknown_03001554
_08030818: .4byte 0x000031FF
_0803081C: .4byte gUnknown_03001570
_08030820: .4byte gUnknown_03001568
_08030824: .4byte gUnknown_03001574
_08030828: .4byte gUnknown_03001538
_0803082C: .4byte gUnknown_0300153C
_08030830: .4byte gUnknown_03001534

	thumb_func_start sub_8030834
sub_8030834: @ 0x08030834
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	ldr r1, _080308E8 @ =gUnknown_03001548
	ldr r2, _080308EC @ =gUnknown_03001560
	ldr r0, [r1]
	ldr r3, [r2]
	adds r6, r0, r3
	str r6, [r1]
	cmp r3, #0xb2
	bgt _08030852
	adds r0, r3, #1
	str r0, [r2]
_08030852:
	ldr r0, _080308F0 @ =gUnknown_03001570
	mov sb, r0
	ldr r1, [r0]
	mov r8, r1
	cmp r1, #0
	bne _08030928
	ldr r0, _080308F4 @ =gUnknown_03000884
	ldr r4, [r0]
	ldr r0, [r4, #0x24]
	adds r0, #0xa
	subs r0, r0, r6
	ldr r1, _080308F8 @ =0xFFFFFE56
	bl sub_803ADB4
	adds r1, r0, #0
	cmp r1, #0
	ble _08030930
	movs r0, #0x80
	lsls r0, r0, #5
	bl sub_803ADB4
	ldr r1, [r4, #0x1c]
	ldr r2, _080308FC @ =0x00000CDB
	adds r1, r1, r2
	ldr r2, _08030900 @ =gUnknown_03001540
	ldr r7, [r2]
	subs r1, r1, r7
	adds r3, r1, #0
	muls r3, r0, r3
	asrs r1, r3, #0xc
	mov ip, r1
	ldr r1, [r4, #0x20]
	ldr r2, _08030904 @ =0xFFFFAE93
	adds r1, r1, r2
	ldr r2, _08030908 @ =gUnknown_03001544
	ldr r5, [r2]
	subs r1, r1, r5
	adds r2, r1, #0
	muls r2, r0, r2
	asrs r4, r2, #0xc
	asrs r3, r3, #0x1f
	mov r1, ip
	eors r1, r3
	subs r1, r1, r3
	asrs r2, r2, #0x1f
	adds r0, r4, #0
	eors r0, r2
	subs r0, r0, r2
	adds r1, r1, r0
	ldr r0, _0803090C @ =0x000007FF
	cmp r1, r0
	bgt _08030930
	ldr r1, _08030910 @ =0xFFFFF325
	adds r0, r7, r1
	ldr r2, _08030914 @ =0x0000516D
	adds r1, r5, r2
	adds r2, r6, #0
	subs r2, #0xa
	str r4, [sp]
	mov r3, ip
	bl sub_802E674
	ldr r3, _08030918 @ =gUnknown_03001574
	ldr r1, [r3]
	adds r1, #1
	str r1, [r3]
	ldr r0, _0803091C @ =gUnknown_03001568
	ldr r2, [r0]
	ldr r0, [r2, #0x14]
	cmp r1, r0
	bne _08030920
	mov r0, r8
	str r0, [r3]
	ldr r0, [r2, #0x18]
	b _0803092C
	.align 2, 0
_080308E8: .4byte gUnknown_03001548
_080308EC: .4byte gUnknown_03001560
_080308F0: .4byte gUnknown_03001570
_080308F4: .4byte gUnknown_03000884
_080308F8: .4byte 0xFFFFFE56
_080308FC: .4byte 0x00000CDB
_08030900: .4byte gUnknown_03001540
_08030904: .4byte 0xFFFFAE93
_08030908: .4byte gUnknown_03001544
_0803090C: .4byte 0x000007FF
_08030910: .4byte 0xFFFFF325
_08030914: .4byte 0x0000516D
_08030918: .4byte gUnknown_03001574
_0803091C: .4byte gUnknown_03001568
_08030920:
	ldr r0, [r2, #0x10]
	mov r2, sb
	str r0, [r2]
	b _08030930
_08030928:
	mov r0, r8
	subs r0, #1
_0803092C:
	mov r1, sb
	str r0, [r1]
_08030930:
	bl sub_8030E08
	ldr r0, _08030998 @ =gUnknown_03001554
	ldr r1, [r0]
	movs r0, #0x86
	lsls r0, r0, #7
	cmp r1, r0
	ble _08030986
	ldr r1, _0803099C @ =gUnknown_03001570
	ldr r0, _080309A0 @ =gUnknown_03001568
	ldr r0, [r0]
	ldr r0, [r0, #4]
	str r0, [r1]
	ldr r0, _080309A4 @ =gUnknown_03001574
	movs r5, #0
	str r5, [r0]
	movs r1, #2
	ldr r0, _080309A8 @ =gUnknown_03001538
	str r1, [r0]
	ldr r0, _080309AC @ =gUnknown_0300153C
	str r5, [r0]
	ldr r0, _080309B0 @ =gUnknown_03001534
	ldr r4, [r0]
	str r5, [r4, #0xc]
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
	movs r2, #4
	ldrsh r1, [r1, r2]
	cmp r0, r1
	blt _08030986
	str r5, [r4, #8]
_08030986:
	bl sub_803171C
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08030998: .4byte gUnknown_03001554
_0803099C: .4byte gUnknown_03001570
_080309A0: .4byte gUnknown_03001568
_080309A4: .4byte gUnknown_03001574
_080309A8: .4byte gUnknown_03001538
_080309AC: .4byte gUnknown_0300153C
_080309B0: .4byte gUnknown_03001534

	thumb_func_start sub_80309B4
sub_80309B4: @ 0x080309B4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	ldr r1, _08030A3C @ =gUnknown_03001540
	ldr r0, _08030A40 @ =gUnknown_03001558
	ldr r3, [r1]
	ldr r0, [r0]
	adds r3, r3, r0
	str r3, [r1]
	ldr r1, _08030A44 @ =gUnknown_03001544
	ldr r0, _08030A48 @ =gUnknown_0300155C
	ldr r2, [r1]
	ldr r0, [r0]
	adds r2, r2, r0
	str r2, [r1]
	ldr r0, _08030A4C @ =gUnknown_03001548
	mov sl, r0
	ldr r1, _08030A50 @ =gUnknown_03001560
	mov r8, r1
	ldr r0, [r0]
	ldr r1, [r1]
	adds r0, r0, r1
	mov r4, sl
	str r0, [r4]
	ldr r0, _08030A54 @ =gUnknown_03001578
	movs r6, #0
	str r6, [r0]
	ldr r1, _08030A58 @ =0x05000020
	ldr r5, _08030A5C @ =gStaticData_0817C3D8
	movs r4, #0
	ldrsh r0, [r5, r4]
	lsls r0, r0, #8
	adds r7, r3, r0
	movs r3, #2
	ldrsh r0, [r5, r3]
	lsls r0, r0, #8
	adds r2, r2, r0
	mov sb, r2
	ldr r4, _08030A60 @ =gUnknown_0300153C
	ldr r0, [r4]
	cmp r0, #0xa
	bne _08030A68
	strh r6, [r1, #0x1e]
	movs r4, #6
	ldrsh r0, [r5, r4]
	lsls r0, r0, #8
	bl sub_8000E1C
	adds r4, r0, #0
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	adds r4, r7, r4
	movs r1, #8
	ldrsh r0, [r5, r1]
	lsls r0, r0, #8
	bl sub_8000E1C
	adds r1, r0, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	add r1, sb
	mov r3, sl
	ldr r2, [r3]
	ldr r0, _08030A64 @ =0xFFFFFF00
	adds r2, r2, r0
	b _08030BE0
	.align 2, 0
_08030A3C: .4byte gUnknown_03001540
_08030A40: .4byte gUnknown_03001558
_08030A44: .4byte gUnknown_03001544
_08030A48: .4byte gUnknown_0300155C
_08030A4C: .4byte gUnknown_03001548
_08030A50: .4byte gUnknown_03001560
_08030A54: .4byte gUnknown_03001578
_08030A58: .4byte 0x05000020
_08030A5C: .4byte gStaticData_0817C3D8
_08030A60: .4byte gUnknown_0300153C
_08030A64: .4byte 0xFFFFFF00
_08030A68:
	cmp r0, #0x32
	bne _08030AA4
	strh r6, [r1, #2]
	movs r1, #6
	ldrsh r6, [r5, r1]
	lsls r6, r6, #8
	adds r0, r6, #0
	bl sub_8000E1C
	adds r4, r0, #0
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	adds r4, r7, r4
	movs r2, #8
	ldrsh r5, [r5, r2]
	lsls r5, r5, #8
	adds r0, r5, #0
	bl sub_8000E1C
	adds r1, r0, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	add r1, sb
	mov r3, sl
	ldr r2, [r3]
	ldr r0, _08030AA0 @ =0xFFFFFF00
	mov r8, r0
	b _08030BB6
	.align 2, 0
_08030AA0: .4byte 0xFFFFFF00
_08030AA4:
	cmp r0, #0x50
	bne _08030B30
	strh r6, [r1, #8]
	movs r4, #6
	ldrsh r6, [r5, r4]
	lsls r6, r6, #8
	adds r0, r6, #0
	bl sub_8000E1C
	adds r4, r0, #0
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	adds r4, r7, r4
	movs r0, #8
	ldrsh r5, [r5, r0]
	lsls r5, r5, #8
	adds r0, r5, #0
	bl sub_8000E1C
	adds r1, r0, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	add r1, sb
	mov r3, sl
	ldr r2, [r3]
	ldr r0, _08030B2C @ =0xFFFFFF00
	mov r8, r0
	add r2, r8
	adds r0, r4, #0
	bl sub_802E420
	adds r0, r6, #0
	bl sub_8000E1C
	adds r4, r0, #0
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	adds r4, r7, r4
	adds r0, r5, #0
	bl sub_8000E1C
	adds r1, r0, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	add r1, sb
	mov r3, sl
	ldr r2, [r3]
	add r2, r8
	adds r0, r4, #0
	bl sub_802E420
	adds r0, r6, #0
	bl sub_8000E1C
	adds r4, r0, #0
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	adds r4, r7, r4
	adds r0, r5, #0
	bl sub_8000E1C
	adds r1, r0, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	add r1, sb
	mov r0, sl
	ldr r2, [r0]
	b _08030BDE
	.align 2, 0
_08030B2C: .4byte 0xFFFFFF00
_08030B30:
	cmp r0, #0x6e
	bne _08030BEC
	strh r6, [r1, #0x10]
	movs r1, #6
	ldrsh r6, [r5, r1]
	lsls r6, r6, #8
	adds r0, r6, #0
	bl sub_8000E1C
	adds r4, r0, #0
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	adds r4, r7, r4
	movs r2, #8
	ldrsh r5, [r5, r2]
	lsls r5, r5, #8
	adds r0, r5, #0
	bl sub_8000E1C
	adds r1, r0, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	add r1, sb
	mov r3, sl
	ldr r2, [r3]
	ldr r0, _08030BE8 @ =0xFFFFFF00
	mov r8, r0
	add r2, r8
	adds r0, r4, #0
	bl sub_802E420
	adds r0, r6, #0
	bl sub_8000E1C
	adds r4, r0, #0
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	adds r4, r7, r4
	adds r0, r5, #0
	bl sub_8000E1C
	adds r1, r0, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	add r1, sb
	mov r3, sl
	ldr r2, [r3]
	add r2, r8
	adds r0, r4, #0
	bl sub_802E420
	adds r0, r6, #0
	bl sub_8000E1C
	adds r4, r0, #0
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	adds r4, r7, r4
	adds r0, r5, #0
	bl sub_8000E1C
	adds r1, r0, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	add r1, sb
	mov r0, sl
	ldr r2, [r0]
_08030BB6:
	add r2, r8
	adds r0, r4, #0
	bl sub_802E420
	adds r0, r6, #0
	bl sub_8000E1C
	adds r4, r0, #0
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	adds r4, r7, r4
	adds r0, r5, #0
	bl sub_8000E1C
	adds r1, r0, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	add r1, sb
	mov r3, sl
	ldr r2, [r3]
_08030BDE:
	add r2, r8
_08030BE0:
	adds r0, r4, #0
	bl sub_802E420
	b _08030C6C
	.align 2, 0
_08030BE8: .4byte 0xFFFFFF00
_08030BEC:
	cmp r0, #0xaa
	bne _08030C6C
	bl sub_802A4EC
	movs r1, #5
	movs r2, #1
	ldr r0, _08030C7C @ =gUnknown_03001538
	str r1, [r0]
	str r6, [r4]
	ldr r0, _08030C80 @ =gUnknown_03001534
	ldr r4, [r0]
	str r2, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
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
	movs r2, #4
	ldrsh r1, [r1, r2]
	cmp r0, r1
	blt _08030C2A
	str r6, [r4, #8]
_08030C2A:
	ldr r0, _08030C84 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x42
	bl PlaySfx
	movs r0, #0x9d
	mov r3, r8
	str r0, [r3]
	ldr r0, _08030C88 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _08030C6C
	ldr r4, _08030C8C @ =gUnknown_0300157C
	ldr r0, [r4]
	cmp r0, #1
	bgt _08030C6C
	ldr r1, _08030C90 @ =gUnknown_03000884
	ldr r0, _08030C94 @ =gUnknown_03001506
	ldrb r0, [r0]
	cmp r0, #0
	bne _08030C6C
	ldr r0, [r1]
	bl sub_802F4AC
	bl sub_802E3CC
	ldr r0, [r4]
	adds r0, #1
	str r0, [r4]
_08030C6C:
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08030C7C: .4byte gUnknown_03001538
_08030C80: .4byte gUnknown_03001534
_08030C84: .4byte gUnknown_030012BC
_08030C88: .4byte gUnknown_030012C0
_08030C8C: .4byte gUnknown_0300157C
_08030C90: .4byte gUnknown_03000884
_08030C94: .4byte gUnknown_03001506

