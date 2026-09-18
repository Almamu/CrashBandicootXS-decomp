.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8027138
sub_8027138: @ 0x08027138
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	ldr r0, _080271C4 @ =0x000008C4
	bl sub_8026EC0
	movs r1, #0x23
	stm r0!, {r1}
	adds r7, r0, #0
	adds r4, r7, #0
	movs r5, #0x22
	movs r0, #1
	rsbs r0, r0, #0
	mov r8, r0
_08027156:
	adds r0, r4, #0
	bl sub_8027120
	adds r4, #0x40
	subs r5, #1
	cmp r5, r8
	bne _08027156
	str r7, [r6, #0x64]
	movs r0, #0
	str r0, [r6]
	str r0, [r6, #0x10]
	str r0, [r6, #8]
	str r0, [r6, #4]
	str r0, [r6, #0x14]
	str r0, [r6, #0xc]
	movs r7, #0
	movs r5, #0
_08027178:
	ldr r0, [r6, #0x64]
	adds r0, r0, r5
	movs r1, #0
	bl sub_80088D8
	ldr r0, _080271C8 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r1, [r0]
	movs r2, #0x8d
	lsls r2, r2, #2
	adds r1, r1, r2
	ldr r0, [r6, #0x64]
	adds r4, r5, r0
	str r1, [r4, #0x20]
	cmp r7, #0x16
	bne _080271D4
	ldr r0, _080271CC @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80233B4
	ldr r1, [r6, #0x64]
	adds r4, r1, r5
	adds r0, #6
	ldr r3, _080271D0 @ =0x000005AD
	adds r1, r1, r3
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	b _080271F6
	.align 2, 0
_080271C4: .4byte 0x000008C4
_080271C8: .4byte gUnknown_030012D0
_080271CC: .4byte gUnknown_030012C0
_080271D0: .4byte 0x000005AD
_080271D4:
	ldr r0, _08027308 @ =gStaticData_08174BE0
	lsls r1, r7, #2
	adds r1, r1, r0
	ldr r0, [r1]
	adds r1, r4, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
_080271F6:
	ldr r0, [r6, #0x64]
	adds r0, r5, r0
	lsls r2, r7, #3
	ldr r1, _0802730C @ =gStaticData_08174C6C
	adds r2, r2, r1
	ldr r1, [r2]
	ldr r2, [r2, #4]
	bl sub_800737C
	adds r5, #0x40
	adds r7, #1
	cmp r7, #0x22
	ble _08027178
	ldr r0, _08027310 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r7, #0xd2
	lsls r7, r7, #1
	adds r0, r0, r7
	movs r5, #0xd0
	lsls r5, r5, #2
	ldr r1, [r6, #0x64]
	adds r4, r1, r5
	str r0, [r4, #0x20]
	ldr r0, _08027308 @ =gStaticData_08174BE0
	ldr r0, [r0, #0x34]
	ldr r2, _08027314 @ =0x0000036D
	adds r1, r1, r2
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r1, [r6, #0x64]
	adds r5, r1, r5
	ldr r0, [r5, #0x20]
	ldr r2, [r0]
	ldr r3, _08027314 @ =0x0000036D
	adds r1, r1, r3
	ldrb r7, [r1]
	lsls r0, r7, #3
	adds r1, r7, #0
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r2, r2, r0
	ldr r0, _08027318 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldrb r1, [r2, #0x14]
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	ldr r2, [r6, #0x64]
	ldr r1, _0802731C @ =0x00000369
	adds r2, r2, r1
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldr r3, [r6, #0x64]
	movs r7, #0x80
	lsls r7, r7, #3
	adds r5, r3, r7
	movs r4, #0xa
	ldr r0, [r5, #0x20]
	ldr r2, _08027320 @ =0x0000042D
	adds r1, r3, r2
	ldr r2, [r0]
	ldrb r7, [r1]
	lsls r0, r7, #3
	adds r1, r7, #0
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _080272A8
	subs r4, r0, #1
_080272A8:
	str r4, [r5, #0x30]
	movs r0, #0x98
	lsls r0, r0, #3
	adds r5, r3, r0
	movs r4, #0xb
	ldr r0, [r5, #0x20]
	ldr r2, _08027324 @ =0x000004ED
	adds r1, r3, r2
	ldr r2, [r0]
	ldrb r7, [r1]
	lsls r0, r7, #3
	adds r1, r7, #0
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _080272CE
	subs r4, r0, #1
_080272CE:
	str r4, [r5, #0x30]
	movs r0, #0xa8
	lsls r0, r0, #3
	adds r5, r3, r0
	movs r4, #0
	ldr r0, [r5, #0x20]
	ldr r2, _08027328 @ =0x0000056D
	adds r1, r3, r2
	ldr r2, [r0]
	ldrb r3, [r1]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r1, [r0, #0x16]
	cmp r4, r1
	blt _080272F2
	subs r4, r1, #1
_080272F2:
	str r4, [r5, #0x30]
	adds r0, r6, #0
	movs r1, #0
	bl sub_802732C
	adds r0, r6, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08027308: .4byte gStaticData_08174BE0
_0802730C: .4byte gStaticData_08174C6C
_08027310: .4byte gUnknown_030012D0
_08027314: .4byte 0x0000036D
_08027318: .4byte gUnknown_030012B8
_0802731C: .4byte 0x00000369
_08027320: .4byte 0x0000042D
_08027324: .4byte 0x000004ED
_08027328: .4byte 0x0000056D

	thumb_func_start sub_802732C
sub_802732C: @ 0x0802732C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	adds r6, r0, #0
	strb r1, [r6, #0x18]
	ldr r0, _080273E0 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xd2
	lsls r1, r1, #1
	adds r0, r0, r1
	movs r5, #0xd0
	lsls r5, r5, #2
	ldr r1, [r6, #0x64]
	adds r4, r1, r5
	str r0, [r4, #0x20]
	ldr r0, _080273E4 @ =gStaticData_08174BE0
	ldr r0, [r0, #0x34]
	ldr r2, _080273E8 @ =0x0000036D
	adds r1, r1, r2
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r6, #0x64]
	adds r0, r0, r5
	bl sub_800815C
	mov sl, r0
	ldr r2, [r6, #0x64]
	ldr r3, _080273EC @ =0x00000369
	adds r2, r2, r3
	movs r0, #0xf
	mov r1, sl
	ands r1, r0
	movs r3, #0x10
	rsbs r3, r3, #0
	adds r0, r3, #0
	ldrb r4, [r2]
	ands r0, r4
	orrs r0, r1
	strb r0, [r2]
	movs r7, #0
	mov sb, r3
	movs r0, #0
	mov r8, r0
_0802739E:
	lsls r5, r7, #6
	cmp r7, #0x16
	bne _080273CE
	ldr r0, _080273F0 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80233B4
	ldr r1, [r6, #0x64]
	mov r2, r8
	adds r4, r1, r2
	adds r0, #6
	ldr r3, _080273F4 @ =0x000005AD
	adds r1, r1, r3
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
_080273CE:
	movs r4, #0
	cmp r7, #0x16
	blt _0802741C
	cmp r7, #0x17
	ble _080273F8
	cmp r7, #0x1d
	beq _08027410
	b _0802741C
	.align 2, 0
_080273E0: .4byte gUnknown_030012D0
_080273E4: .4byte gStaticData_08174BE0
_080273E8: .4byte 0x0000036D
_080273EC: .4byte 0x00000369
_080273F0: .4byte gUnknown_030012C0
_080273F4: .4byte 0x000005AD
_080273F8:
	ldr r0, _0802740C @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80233B4
	movs r1, #1
	rsbs r1, r1, #0
	cmp r0, r1
	beq _08027426
	b _0802741C
	.align 2, 0
_0802740C: .4byte gUnknown_030012C0
_08027410:
	ldrb r0, [r6, #0x18]
	cmp r0, #0
	beq _08027426
	ldr r0, [r6, #0x64]
	adds r0, r0, r5
	b _08027420
_0802741C:
	ldr r0, [r6, #0x64]
	add r0, r8
_08027420:
	bl sub_800815C
	adds r4, r0, #0
_08027426:
	ldr r0, _08027468 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80233B4
	movs r1, #1
	rsbs r1, r1, #0
	cmp r0, r1
	bne _0802749A
	ldrb r0, [r6, #0x18]
	cmp r0, #0
	beq _0802749A
	cmp r7, #0x15
	bgt _08027470
	cmp r7, #0xe
	blt _08027470
	ldr r1, _0802746C @ =gStaticData_08174C6C
	lsls r0, r7, #3
	adds r0, r0, r1
	ldr r1, [r6, #0x64]
	add r1, r8
	ldr r0, [r0]
	lsls r0, r0, #8
	str r0, [r1]
	movs r0, #0xa0
	lsls r0, r0, #5
	str r0, [r1, #4]
	adds r1, #0x29
	movs r0, #0xf
	ands r4, r0
	mov r0, sb
	ldrb r2, [r1]
	ands r0, r2
	b _080274AA
	.align 2, 0
_08027468: .4byte gUnknown_030012C0
_0802746C: .4byte gStaticData_08174C6C
_08027470:
	cmp r4, sl
	bne _08027488
	ldr r0, [r6, #0x64]
	add r0, r8
	adds r0, #0x29
	movs r2, #0xa
	mov r1, sb
	ldrb r3, [r0]
	ands r1, r3
	orrs r1, r2
	strb r1, [r0]
	b _080274AE
_08027488:
	ldr r1, [r6, #0x64]
	add r1, r8
	adds r1, #0x29
	movs r0, #0xf
	ands r4, r0
	mov r0, sb
	ldrb r2, [r1]
	ands r0, r2
	b _080274AA
_0802749A:
	ldr r1, [r6, #0x64]
	add r1, r8
	adds r1, #0x29
	movs r0, #0xf
	ands r4, r0
	mov r0, sb
	ldrb r3, [r1]
	ands r0, r3
_080274AA:
	orrs r0, r4
	strb r0, [r1]
_080274AE:
	movs r4, #0x40
	add r8, r4
	adds r7, #1
	cmp r7, #0x22
	bgt _080274BA
	b _0802739E
_080274BA:
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [sp]
	ldr r1, _080274E4 @ =0x040000D4
	mov r0, sp
	str r0, [r1]
	adds r0, r6, #0
	adds r0, #0x40
	str r0, [r1, #4]
	ldr r0, _080274E8 @ =0x85000009
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080274E4: .4byte 0x040000D4
_080274E8: .4byte 0x85000009
