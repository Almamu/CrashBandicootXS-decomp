.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_803472C
sub_803472C: @ 0x0803472C
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	sub sp, #4
	adds r5, r0, #0
	movs r0, #0x10
	bl sub_8026EDC
	movs r1, #3
	str r1, [sp]
	movs r1, #0
	movs r2, #0x1f
	movs r3, #0
	bl sub_801E644
	str r0, [r5, #4]
	movs r0, #0x10
	bl sub_8026EDC
	movs r1, #1
	str r1, [sp]
	movs r1, #3
	movs r2, #0x1e
	movs r3, #0
	bl sub_801E644
	str r0, [r5]
	movs r0, #0x10
	bl sub_8026EDC
	movs r1, #2
	str r1, [sp]
	movs r2, #0x1d
	movs r3, #1
	bl sub_801E644
	str r0, [r5, #8]
	ldr r0, [r5]
	ldr r1, _08034854 @ =gStaticData_0817C5BC
	bl LoadGraphicsPackage
	ldr r0, [r5, #4]
	ldr r1, _08034858 @ =gStaticData_0817C594
	bl LoadGraphicsPackage
	ldr r0, [r5, #8]
	ldr r1, _0803485C @ =gStaticData_0817C5A8
	bl LoadGraphicsPackage
	movs r1, #0xa0
	lsls r1, r1, #0x13
	movs r0, #0
	strh r0, [r1]
	movs r0, #0
	mov r8, r0
	mov r1, r8
	strh r1, [r5, #0xc]
	movs r2, #0x40
	mov sb, r2
	mov r0, sb
	ldrb r1, [r5, #0xc]
	orrs r0, r1
	movs r1, #8
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r5, #0xc]
	movs r6, #1
	ldrb r0, [r5, #0xd]
	orrs r0, r6
	movs r1, #2
	orrs r0, r1
	movs r4, #4
	orrs r0, r4
	strb r0, [r5, #0xd]
	adds r0, r5, #0
	bl sub_803487C
	mov r2, r8
	str r2, [r5, #0x10]
	ldrb r0, [r5, #0x10]
	orrs r4, r0
	ldrb r1, [r5, #0x11]
	orrs r6, r1
	strb r6, [r5, #0x11]
	movs r1, #0x20
	rsbs r1, r1, #0
	adds r0, r1, #0
	ldrb r2, [r5, #0x12]
	ands r0, r2
	movs r2, #8
	orrs r0, r2
	strb r0, [r5, #0x12]
	ldrb r0, [r5, #0x13]
	ands r1, r0
	movs r0, #0x10
	orrs r1, r0
	strb r1, [r5, #0x13]
	movs r0, #0x3f
	ands r4, r0
	mov r1, sb
	orrs r4, r1
	strb r4, [r5, #0x10]
	ldr r0, [r5, #4]
	bl sub_801E640
	ldr r1, _08034860 @ =0x04000008
	strh r0, [r1]
	ldr r0, _08034864 @ =0x04000010
	mov r2, r8
	str r2, [r0]
	ldr r0, [r5]
	bl sub_801E640
	ldr r1, _08034868 @ =0x0400000A
	strh r0, [r1]
	ldr r0, _0803486C @ =0x04000014
	mov r1, r8
	str r1, [r0]
	ldr r0, [r5, #8]
	bl sub_801E640
	ldr r1, _08034870 @ =0x0400000C
	strh r0, [r1]
	ldr r0, _08034874 @ =0x04000018
	mov r2, r8
	str r2, [r0]
	subs r1, #0xc
	ldrh r0, [r5, #0xc]
	strh r0, [r1]
	adds r1, #0x50
	ldr r0, [r5, #0x10]
	str r0, [r1]
	str r2, [r5, #0x1c]
	str r2, [r5, #0x20]
	ldr r0, _08034878 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8001AC4
	adds r0, r5, #0
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08034854: .4byte gStaticData_0817C5BC
_08034858: .4byte gStaticData_0817C594
_0803485C: .4byte gStaticData_0817C5A8
_08034860: .4byte 0x04000008
_08034864: .4byte 0x04000010
_08034868: .4byte 0x0400000A
_0803486C: .4byte 0x04000014
_08034870: .4byte 0x0400000C
_08034874: .4byte 0x04000018
_08034878: .4byte gUnknown_030012BC

	thumb_func_start sub_803487C
sub_803487C: @ 0x0803487C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r0
	ldr r5, _08034974 @ =gUnknown_030012FC
	ldr r0, [r5]
	movs r4, #0
	str r4, [r0, #8]
	bl sub_8006C4C
	ldr r0, [r5]
	bl sub_8006C4C
	ldr r0, _08034978 @ =gUnknown_030012DC
	ldr r0, [r0]
	mov r1, r8
	str r0, [r1, #0x18]
	movs r2, #0x84
	lsls r2, r2, #1
	adds r1, r0, r2
	str r4, [r1]
	movs r3, #0x98
	lsls r3, r3, #1
	adds r1, r0, r3
	ldr r1, [r1]
	adds r1, #0x40
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r1, [r1, #4]
	bl sub_803AD7C
	mov r0, r8
	ldr r1, [r0, #0x18]
	movs r2, #0x8c
	lsls r2, r2, #1
	adds r0, r1, r2
	str r4, [r0]
	ldr r0, [r5]
	movs r3, #0x96
	lsls r3, r3, #1
	adds r1, r1, r3
	ldr r1, [r1]
	lsls r1, r1, #5
	bl sub_8006C58
	ldr r0, [r5]
	bl sub_8006C30
	ldr r4, _0803497C @ =gUnknown_030012B8
	ldr r0, [r4]
	bl sub_8006EA8
	ldr r0, [r4]
	movs r1, #0
	bl sub_8006D50
	ldr r0, [r4]
	movs r1, #1
	bl sub_8006D50
	ldr r0, [r4]
	movs r1, #2
	bl sub_8006D50
	ldr r0, [r4]
	movs r1, #3
	bl sub_8006D50
	ldr r0, [r4]
	movs r7, #0
	adds r2, r0, #0
	adds r2, #0x6c
	ldr r6, _08034980 @ =gStaticData_0817C532
	adds r1, r0, #0
	adds r1, #0x2c
	ldr r5, _08034984 @ =gStaticData_0817C512
	ldr r4, _08034988 @ =gStaticData_0817C572
	ldr r3, _0803498C @ =gStaticData_0817C552
_0803491A:
	ldrh r0, [r5]
	strh r0, [r1]
	ldrh r0, [r6]
	strh r0, [r1, #0x20]
	ldrh r0, [r3]
	strh r0, [r2]
	ldrh r0, [r4]
	strh r0, [r2, #0x20]
	adds r2, #2
	adds r6, #2
	adds r1, #2
	adds r5, #2
	adds r4, #2
	adds r3, #2
	adds r7, #1
	cmp r7, #0xf
	ble _0803491A
	ldr r0, _0803497C @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	movs r0, #0x10
	mov r1, r8
	ldrb r1, [r1, #0xd]
	orrs r0, r1
	mov r2, r8
	strb r0, [r2, #0xd]
	ldr r4, _08034990 @ =gUnknown_03001300
	ldr r0, [r4]
	bl sub_8006A90
	ldr r0, [r4]
	bl sub_8006A48
	bl sub_80006A8
	ldr r0, [r4]
	bl sub_8006AAC
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08034974: .4byte gUnknown_030012FC
_08034978: .4byte gUnknown_030012DC
_0803497C: .4byte gUnknown_030012B8
_08034980: .4byte gStaticData_0817C532
_08034984: .4byte gStaticData_0817C512
_08034988: .4byte gStaticData_0817C572
_0803498C: .4byte gStaticData_0817C552
_08034990: .4byte gUnknown_03001300

	thumb_func_start sub_8034994
sub_8034994: @ 0x08034994
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	adds r4, r0, #0
	movs r0, #1
	mov sb, r0
	movs r1, #0
	mov r8, r1
	ldrb r2, [r4, #0x12]
	lsls r0, r2, #0x1b
	lsrs r6, r0, #0x1b
	ldr r0, _080349E8 @ =gUnknown_030007E0
	mov sl, r0
	ldr r7, _080349EC @ =gUnknown_030012BC
_080349B4:
	ldr r0, _080349F0 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	mov r1, sl
	ldr r2, [r1]
	lsrs r1, r2, #0x10
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	bne _080349D8
	lsrs r1, r2, #0x10
	movs r0, #8
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r5, r0, #0x10
	cmp r5, #0
	beq _080349F4
_080349D8:
	ldr r0, [r7]
	movs r1, #0x49
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	b _08034A88
	.align 2, 0
_080349E8: .4byte gUnknown_030007E0
_080349EC: .4byte gUnknown_030012BC
_080349F0: .4byte gUnknown_03001304
_080349F4:
	lsrs r1, r2, #0x10
	movs r0, #0x40
	ands r0, r1
	cmp r0, #0
	beq _08034A12
	ldr r0, [r4, #0x20]
	cmp r0, #1
	bne _08034A12
	ldr r0, [r7]
	movs r1, #0x46
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	str r5, [r4, #0x20]
_08034A12:
	movs r0, #0x80
	mov r2, sl
	ldrh r2, [r2, #2]
	ands r0, r2
	cmp r0, #0
	beq _08034A34
	ldr r0, [r4, #0x20]
	cmp r0, #0
	bne _08034A34
	ldr r0, [r7]
	movs r1, #0x46
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	movs r0, #1
	str r0, [r4, #0x20]
_08034A34:
	adds r0, r4, #0
	bl sub_8034AA4
	adds r0, r4, #0
	bl sub_8034C5C
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #1
	ble _080349B4
	movs r2, #0
	mov r8, r2
	mov r0, sb
	cmp r0, #0
	beq _08034A5E
	subs r6, #1
	cmp r6, #0
	bgt _08034A68
	mov sb, r2
	b _08034A68
_08034A5E:
	adds r6, #1
	cmp r6, #0xf
	ble _08034A68
	movs r1, #1
	mov sb, r1
_08034A68:
	movs r0, #0x1f
	adds r1, r6, #0
	ands r1, r0
	movs r2, #0x20
	rsbs r2, r2, #0
	adds r0, r2, #0
	ldrb r2, [r4, #0x12]
	ands r0, r2
	orrs r0, r1
	strb r0, [r4, #0x12]
	ldr r1, _08034A84 @ =0x04000050
	ldr r0, [r4, #0x10]
	str r0, [r1]
	b _080349B4
	.align 2, 0
_08034A84: .4byte 0x04000050
_08034A88:
	movs r1, #0
	ldr r0, [r4, #0x20]
	cmp r0, #0
	bne _08034A92
	movs r1, #1
_08034A92:
	adds r0, r1, #0
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

