.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8007634
sub_8007634: @ 0x08007634
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x48
	str r1, [sp, #8]
	str r2, [sp, #0xc]
	movs r0, #0
	str r0, [sp, #0x10]
	adds r0, r1, #0
	bl sub_80083B8
	str r0, [sp, #0x14]
	ldr r0, _0800768C @ =gUnknown_030012FC
	ldr r0, [r0]
	bl sub_8006C44
	str r0, [sp, #0x18]
	ldr r1, [sp, #8]
	ldrh r1, [r1, #0x3c]
	str r1, [sp, #0x1c]
	cmp r1, #0x3f
	bgt _08007668
	movs r2, #0x40
	str r2, [sp, #0x1c]
_08007668:
	ldr r3, [sp, #0x1c]
	lsls r0, r3, #0x10
	asrs r0, r0, #0x10
	bl sub_800090C
	lsls r0, r0, #0x10
	lsrs r6, r0, #0x10
	adds r7, r6, #0
	movs r2, #0x80
	lsls r2, r2, #1
	ldr r4, [sp, #0x1c]
	cmp r4, r2
	bgt _08007694
	ldr r1, _08007690 @ =0xFFFFFCFF
	ldr r0, [sp]
	ands r0, r1
	orrs r0, r2
	b _0800769C
	.align 2, 0
_0800768C: .4byte gUnknown_030012FC
_08007690: .4byte 0xFFFFFCFF
_08007694:
	movs r1, #0xc0
	lsls r1, r1, #2
	ldr r0, [sp]
	orrs r0, r1
_0800769C:
	str r0, [sp]
	ldr r0, _080077F0 @ =gUnknown_03001300
	ldr r4, [r0]
	ldr r0, [r4, #8]
	adds r3, r0, #0
	adds r0, #1
	str r0, [r4, #8]
	movs r1, #7
	ands r1, r3
	lsls r1, r1, #0x19
	ldr r2, _080077F4 @ =0xF1FFFFFF
	ldr r0, [sp]
	ands r0, r2
	orrs r0, r1
	asrs r1, r3, #3
	movs r5, #1
	ands r1, r5
	lsls r1, r1, #0x1c
	ldr r2, _080077F8 @ =0xEFFFFFFF
	ands r0, r2
	orrs r0, r1
	asrs r1, r3, #4
	ands r1, r5
	lsls r1, r1, #0x1d
	ldr r2, _080077FC @ =0xDFFFFFFF
	ands r0, r2
	orrs r0, r1
	str r0, [sp]
	lsls r1, r3, #2
	lsls r3, r3, #5
	adds r3, r4, r3
	movs r2, #0
	strh r6, [r3, #0x12]
	adds r0, r1, #1
	lsls r0, r0, #3
	adds r0, r4, r0
	strh r2, [r0, #0x12]
	adds r0, r1, #2
	lsls r0, r0, #3
	adds r0, r4, r0
	strh r2, [r0, #0x12]
	adds r1, #3
	lsls r1, r1, #3
	adds r4, r4, r1
	strh r7, [r4, #0x12]
	ldr r7, [sp, #8]
	adds r7, #0x28
	mov r8, r7
	ldrb r3, [r7]
	lsls r0, r3, #0x1e
	movs r6, #3
	lsrs r0, r0, #0x14
	ldr r4, _08007800 @ =0xFFFFF3FF
	ldr r1, [sp]
	ands r1, r4
	orrs r1, r0
	lsls r0, r3, #0x1d
	lsrs r0, r0, #0x1f
	ands r0, r5
	lsls r0, r0, #0xc
	ldr r2, _08007804 @ =0xFFFFEFFF
	ands r1, r2
	orrs r1, r0
	lsls r3, r3, #0x1c
	lsrs r3, r3, #0x1f
	ands r3, r5
	lsls r3, r3, #0xd
	ldr r0, _08007808 @ =0xFFFFDFFF
	ands r1, r0
	orrs r1, r3
	str r1, [sp]
	ldr r0, [sp, #8]
	ldr r1, [r0, #0x18]
	adds r1, #0x58
	movs r2, #0
	ldrsh r0, [r1, r2]
	ldr r3, [sp, #8]
	adds r0, r3, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	ands r0, r6
	lsls r0, r0, #0xa
	ldr r1, [sp, #4]
	ands r1, r4
	orrs r1, r0
	ldr r0, [sp, #8]
	adds r0, #0x29
	ldrb r0, [r0]
	lsls r0, r0, #0x1c
	lsrs r0, r0, #0x10
	ldr r2, _0800780C @ =0xFFFF0FFF
	ands r1, r2
	orrs r1, r0
	str r1, [sp, #4]
	movs r4, #0
	str r4, [sp, #0x20]
	movs r7, #0
	str r7, [sp, #0x24]
	movs r0, #0
	str r0, [sp, #0x28]
	movs r1, #0
	str r1, [sp, #0x2c]
	movs r2, #0
	str r2, [sp, #0x30]
	movs r3, #0
	str r3, [sp, #0x34]
	str r4, [sp, #0x38]
	mov r7, r8
	str r7, [sp, #0x44]
	ldr r0, [sp, #0x14]
	ldrb r0, [r0, #0xb]
	cmp r4, r0
	bne _08007786
	b _080079F4
_08007786:
	ldr r1, [sp, #0x14]
	ldr r0, [r1, #4]
	ldr r2, [sp, #0x38]
	adds r0, r0, r2
	movs r3, #0xf
	ldrb r0, [r0]
	ands r0, r3
	str r0, [sp, #0x3c]
	ldr r0, _08007810 @ =gStaticData_0816B2E0
	ldr r4, [sp, #0x3c]
	adds r0, r4, r0
	ldrb r0, [r0]
	mov r8, r0
	ldr r0, _08007814 @ =gStaticData_0816B2EC
	adds r0, r4, r0
	ldrb r0, [r0]
	mov sb, r0
	ldr r1, [r1]
	lsls r0, r2, #2
	adds r0, r0, r1
	movs r7, #0
	ldrsh r2, [r0, r7]
	movs r1, #2
	ldrsh r3, [r0, r1]
	mov r4, r8
	asrs r1, r4, #3
	mov r7, sb
	asrs r0, r7, #3
	adds r4, r1, #0
	muls r4, r0, r4
	str r4, [sp, #0x40]
	mov r7, r8
	lsls r0, r7, #8
	ldr r1, [sp, #0x1c]
	muls r0, r1, r0
	asrs r0, r0, #0x10
	mov r8, r0
	mov r4, sb
	lsls r0, r4, #8
	muls r0, r1, r0
	asrs r0, r0, #0x10
	mov sb, r0
	ldr r7, [sp, #0x44]
	ldrb r7, [r7]
	lsls r0, r7, #0x1a
	cmp r0, #0
	bge _08007818
	ldr r1, [sp, #0xc]
	ldr r0, [r1, #4]
	mov r4, sb
	subs r0, r0, r4
	subs r6, r0, r3
	b _0800781E
	.align 2, 0
_080077F0: .4byte gUnknown_03001300
_080077F4: .4byte 0xF1FFFFFF
_080077F8: .4byte 0xEFFFFFFF
_080077FC: .4byte 0xDFFFFFFF
_08007800: .4byte 0xFFFFF3FF
_08007804: .4byte 0xFFFFEFFF
_08007808: .4byte 0xFFFFDFFF
_0800780C: .4byte 0xFFFF0FFF
_08007810: .4byte gStaticData_0816B2E0
_08007814: .4byte gStaticData_0816B2EC
_08007818:
	ldr r7, [sp, #0xc]
	ldr r0, [r7, #4]
	adds r6, r0, r3
_0800781E:
	mov r1, sb
	adds r0, r6, r1
	cmp r0, #0
	bgt _08007828
	b _080079C4
_08007828:
	cmp r6, #0x9f
	ble _0800782E
	b _080079C4
_0800782E:
	ldr r3, [sp, #0x44]
	ldrb r3, [r3]
	lsls r0, r3, #0x1b
	cmp r0, #0
	bge _08007844
	ldr r4, [sp, #0xc]
	ldr r0, [r4]
	mov r7, r8
	subs r0, r0, r7
	subs r5, r0, r2
	b _0800784A
_08007844:
	ldr r1, [sp, #0xc]
	ldr r0, [r1]
	adds r5, r0, r2
_0800784A:
	mov r2, r8
	adds r0, r5, r2
	cmp r0, #0
	bgt _08007854
	b _080079C4
_08007854:
	cmp r5, #0xef
	ble _0800785A
	b _080079C4
_0800785A:
	ldr r3, [sp, #8]
	ldr r3, [r3, #0x30]
	mov sl, r3
	movs r2, #0
	ldr r4, [sp, #8]
	ldr r0, [r4, #0x20]
	adds r4, #0x2d
	ldr r1, [r0]
	ldrb r7, [r4]
	lsls r0, r7, #3
	adds r3, r7, #0
	subs r0, r0, r3
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r2, r0
	blt _0800787E
	subs r2, r0, #1
_0800787E:
	ldr r7, [sp, #8]
	str r2, [r7, #0x30]
	ldr r0, [sp, #8]
	bl sub_80083B8
	adds r7, r0, #0
	mov r2, sl
	ldr r1, [sp, #8]
	ldr r0, [r1, #0x20]
	ldr r1, [r0]
	ldrb r3, [r4]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r2, r0
	blt _080078A4
	subs r2, r0, #1
_080078A4:
	ldr r4, [sp, #8]
	str r2, [r4, #0x30]
	ldr r0, [r7]
	movs r1, #0
	ldrsh r3, [r0, r1]
	movs r2, #2
	ldrsh r4, [r0, r2]
	ldr r0, [r7, #4]
	movs r1, #0xf
	ldrb r0, [r0]
	ands r1, r0
	ldr r7, _08007934 @ =gStaticData_0816B2E0
	mov ip, r7
	mov r0, ip
	adds r2, r1, r0
	ldr r7, _08007938 @ =gStaticData_0816B2EC
	mov sl, r7
	add r1, sl
	ldr r7, [sp, #0xc]
	ldr r0, [r7]
	adds r3, r3, r0
	ldr r0, [r7, #4]
	adds r4, r4, r0
	ldrb r2, [r2]
	lsrs r0, r2, #1
	adds r3, r3, r0
	ldrb r1, [r1]
	lsrs r0, r1, #1
	adds r4, r4, r0
	ldr r0, [sp, #0x38]
	cmp r0, #0
	bne _0800793C
	ldr r0, [sp, #0x3c]
	add r0, ip
	ldrb r0, [r0]
	lsrs r0, r0, #1
	adds r0, r5, r0
	str r0, [sp, #0x30]
	ldr r0, [sp, #0x3c]
	add r0, sl
	ldrb r0, [r0]
	lsrs r0, r0, #1
	adds r0, r6, r0
	ldr r1, [sp, #0x30]
	subs r1, r1, r3
	subs r0, r0, r4
	str r0, [sp, #0x34]
	lsls r0, r1, #8
	ldr r2, [sp, #0x1c]
	muls r0, r2, r0
	asrs r0, r0, #0x10
	str r0, [sp, #0x30]
	ldr r3, [sp, #0x34]
	lsls r0, r3, #8
	muls r0, r2, r0
	asrs r0, r0, #0x10
	ldr r4, [sp, #0x30]
	rsbs r4, r4, #0
	str r4, [sp, #0x30]
	rsbs r0, r0, #0
	str r0, [sp, #0x34]
	adds r5, r5, r4
	adds r6, r6, r0
	str r5, [sp, #0x20]
	str r6, [sp, #0x24]
	mov r7, r8
	asrs r7, r7, #1
	str r7, [sp, #0x28]
	mov r0, sb
	asrs r0, r0, #1
	str r0, [sp, #0x2c]
	b _08007976
	.align 2, 0
_08007934: .4byte gStaticData_0816B2E0
_08007938: .4byte gStaticData_0816B2EC
_0800793C:
	ldr r1, [sp, #0x30]
	adds r5, r5, r1
	ldr r2, [sp, #0x34]
	adds r6, r6, r2
	ldr r3, [sp, #0x20]
	subs r2, r5, r3
	ldr r4, [sp, #0x24]
	subs r1, r6, r4
	lsls r0, r2, #8
	ldr r7, [sp, #0x1c]
	muls r0, r7, r0
	asrs r2, r0, #0x10
	lsls r0, r1, #8
	muls r0, r7, r0
	asrs r1, r0, #0x10
	ldr r0, [sp, #0x28]
	adds r2, r2, r0
	ldr r3, [sp, #0x2c]
	adds r1, r1, r3
	mov r4, r8
	asrs r0, r4, #1
	subs r2, r2, r0
	mov r7, sb
	asrs r0, r7, #1
	subs r1, r1, r0
	ldr r0, [sp, #0x20]
	adds r5, r0, r2
	ldr r2, [sp, #0x24]
	adds r6, r2, r1
_08007976:
	lsls r1, r6, #0x18
	lsrs r1, r1, #0x18
	ldr r2, _08007A24 @ =0xFFFFFF00
	ldr r0, [sp]
	ands r0, r2
	orrs r0, r1
	ldr r3, [sp, #0x3c]
	asrs r2, r3, #2
	movs r4, #3
	ands r2, r4
	lsls r2, r2, #0xe
	ldr r1, _08007A28 @ =0xFFFF3FFF
	ands r0, r1
	orrs r0, r2
	ldr r1, _08007A2C @ =0x000001FF
	ands r5, r1
	lsls r2, r5, #0x10
	ldr r1, _08007A30 @ =0xFE00FFFF
	ands r0, r1
	orrs r0, r2
	ands r3, r4
	lsls r2, r3, #0x1e
	ldr r1, _08007A34 @ =0x3FFFFFFF
	ands r0, r1
	orrs r0, r2
	str r0, [sp]
	ldr r7, [sp, #0x18]
	lsls r1, r7, #0x16
	lsrs r1, r1, #0x16
	ldr r2, _08007A38 @ =0xFFFFFC00
	ldr r0, [sp, #4]
	ands r0, r2
	orrs r0, r1
	str r0, [sp, #4]
	ldr r0, _08007A3C @ =gUnknown_03001300
	ldr r0, [r0]
	mov r1, sp
	bl sub_8006AC8
_080079C4:
	ldr r1, [sp, #0x44]
	ldrb r1, [r1]
	lsls r0, r1, #0x1c
	cmp r0, #0
	bge _080079D4
	ldr r2, [sp, #0x40]
	lsls r2, r2, #1
	str r2, [sp, #0x40]
_080079D4:
	ldr r3, [sp, #0x18]
	ldr r4, [sp, #0x40]
	adds r3, r3, r4
	str r3, [sp, #0x18]
	lsls r0, r4, #5
	ldr r7, [sp, #0x10]
	adds r7, r7, r0
	str r7, [sp, #0x10]
	ldr r0, [sp, #0x38]
	adds r0, #1
	str r0, [sp, #0x38]
	ldr r1, [sp, #0x14]
	ldrb r1, [r1, #0xb]
	cmp r0, r1
	beq _080079F4
	b _08007786
_080079F4:
	ldr r0, _08007A40 @ =gUnknown_030012FC
	ldr r4, [r0]
	ldr r0, [sp, #8]
	bl sub_80083A8
	adds r1, r0, #0
	ldr r2, [sp, #0x14]
	ldr r0, [r2, #8]
	ldr r2, _08007A44 @ =0x00FFFFFF
	ands r0, r2
	adds r1, r1, r0
	adds r0, r4, #0
	ldr r2, [sp, #0x10]
	bl sub_8006C84
	add sp, #0x48
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08007A24: .4byte 0xFFFFFF00
_08007A28: .4byte 0xFFFF3FFF
_08007A2C: .4byte 0x000001FF
_08007A30: .4byte 0xFE00FFFF
_08007A34: .4byte 0x3FFFFFFF
_08007A38: .4byte 0xFFFFFC00
_08007A3C: .4byte gUnknown_03001300
_08007A40: .4byte gUnknown_030012FC
_08007A44: .4byte 0x00FFFFFF

