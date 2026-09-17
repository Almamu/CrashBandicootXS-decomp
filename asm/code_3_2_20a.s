.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_80372BC
sub_80372BC: @ 0x080372BC
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r7, r0, #0
	ldr r0, _08037300 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006A90
	ldr r0, _08037304 @ =gUnknown_030012FC
	ldr r0, [r0]
	bl sub_8006C28
	movs r0, #0x32
	mov r8, r0
	movs r5, #0
	ldr r6, _08037308 @ =gUnknown_030012DC
	movs r2, #0x98
	lsls r2, r2, #1
	mov sb, r2
_080372E4:
	ldr r0, [r7, #8]
	cmp r5, r0
	bne _0803730C
	ldr r4, [r6]
	adds r0, r7, #0
	bl sub_8037534
	adds r1, r0, #0
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	adds r0, r4, #0
	bl sub_8028A30
	b _08037314
	.align 2, 0
_08037300: .4byte gUnknown_03001300
_08037304: .4byte gUnknown_030012FC
_08037308: .4byte gUnknown_030012DC
_0803730C:
	ldr r0, [r6]
	movs r1, #0
	bl sub_8028A30
_08037314:
	ldr r0, [r6]
	mov r3, sb
	adds r1, r0, r3
	ldr r3, [r1]
	movs r2, #0x10
	ldrsh r1, [r3, r2]
	adds r0, r0, r1
	ldr r2, _08037380 @ =gStaticData_0817E714
	lsls r1, r5, #2
	adds r1, r1, r2
	ldr r4, [r1]
	ldr r2, [r3, #0x14]
	adds r1, r4, #0
	bl sub_803AD80
	movs r2, #0xf0
	subs r2, r2, r0
	asrs r2, r2, #1
	ldr r0, [r6]
	movs r3, #0x88
	lsls r3, r3, #1
	adds r1, r0, r3
	str r2, [r1]
	movs r2, #0x8a
	lsls r2, r2, #1
	adds r1, r0, r2
	mov r3, r8
	str r3, [r1]
	mov r2, sb
	adds r1, r0, r2
	ldr r2, [r1]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #0xa
	add r8, r0
	adds r5, #1
	cmp r5, #5
	ble _080372E4
	ldr r0, _08037384 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006A48
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08037380: .4byte gStaticData_0817E714
_08037384: .4byte gUnknown_03001300

	thumb_func_start sub_8037388
sub_8037388: @ 0x08037388
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r4, _080374AC @ =gUnknown_03001300
	ldr r0, [r4]
	bl sub_8006A90
	ldr r0, [r4]
	bl sub_8006A48
	bl sub_80006A8
	ldr r0, [r4]
	bl sub_8006AAC
	ldr r4, _080374B0 @ =gUnknown_030012B8
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
	ldr r6, _080374B4 @ =gStaticData_0817E74C
	adds r1, r0, #0
	adds r1, #0x2c
	ldr r5, _080374B8 @ =gStaticData_0817E72C
	ldr r4, _080374BC @ =gStaticData_0817E78C
	ldr r3, _080374C0 @ =gStaticData_0817E76C
_080373E2:
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
	ble _080373E2
	movs r0, #0
	mov r8, r0
	ldr r4, _080374C4 @ =gUnknown_030012DC
	ldr r0, [r4]
	movs r1, #0
	bl sub_8028A30
	ldr r6, _080374C8 @ =gUnknown_030012E0
	ldr r0, [r6]
	movs r1, #0
	bl sub_8028A30
	ldr r5, _080374CC @ =gUnknown_030012FC
	ldr r0, [r5]
	mov r1, r8
	str r1, [r0, #8]
	bl sub_8006C4C
	ldr r0, [r5]
	bl sub_8006C4C
	ldr r0, [r4]
	movs r2, #0x84
	lsls r2, r2, #1
	adds r1, r0, r2
	mov r3, r8
	str r3, [r1]
	adds r2, #0x28
	adds r1, r0, r2
	ldr r1, [r1]
	adds r1, #0x40
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r1, [r1, #4]
	bl sub_803AD7C
	ldr r0, [r5]
	ldr r1, [r4]
	movs r2, #0x96
	lsls r2, r2, #1
	adds r1, r1, r2
	ldr r1, [r1]
	lsls r1, r1, #5
	bl sub_8006C58
	ldr r0, [r4]
	movs r3, #0x96
	lsls r3, r3, #1
	adds r0, r0, r3
	ldr r2, [r0]
	ldr r0, [r6]
	subs r3, #0x24
	adds r1, r0, r3
	str r2, [r1]
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
	ldr r0, [r5]
	ldr r1, [r6]
	movs r2, #0x96
	lsls r2, r2, #1
	adds r1, r1, r2
	ldr r1, [r1]
	lsls r1, r1, #5
	bl sub_8006C58
	ldr r0, [r5]
	bl sub_8006C30
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080374AC: .4byte gUnknown_03001300
_080374B0: .4byte gUnknown_030012B8
_080374B4: .4byte gStaticData_0817E74C
_080374B8: .4byte gStaticData_0817E72C
_080374BC: .4byte gStaticData_0817E78C
_080374C0: .4byte gStaticData_0817E76C
_080374C4: .4byte gUnknown_030012DC
_080374C8: .4byte gUnknown_030012E0
_080374CC: .4byte gUnknown_030012FC

