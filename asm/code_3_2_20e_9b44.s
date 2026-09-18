.include "asm/macros.inc"

.syntax unified
.arm

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
