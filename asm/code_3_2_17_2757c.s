.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_802757C
sub_802757C: @ 0x0802757C
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r1, _08027628 @ =gUnknown_0300086C
	movs r0, #0
	str r0, [r1]
	ldr r5, _0802762C @ =gStaticData_08174C6C
	ldr r2, [r6, #0x64]
	movs r0, #0xb0
	lsls r0, r0, #3
	adds r3, r2, r0
	adds r0, r5, #0
	adds r0, #0xb0
	ldr r0, [r0]
	adds r1, r5, #0
	adds r1, #0xb4
	ldr r1, [r1]
	lsls r0, r0, #8
	str r0, [r3]
	lsls r1, r1, #8
	str r1, [r3, #4]
	movs r4, #0
	ldr r0, [r3, #0x20]
	ldr r1, _08027630 @ =0x000005AD
	adds r2, r2, r1
	ldr r1, [r0]
	ldrb r7, [r2]
	lsls r0, r7, #3
	adds r2, r7, #0
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _080275C2
	subs r4, r0, #1
_080275C2:
	str r4, [r3, #0x30]
	adds r0, r3, #0
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, _08027634 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023378
	adds r3, r0, #0
	cmp r3, #0
	ble _08027620
	ldr r2, [r6, #0x64]
	movs r0, #0xb8
	lsls r0, r0, #3
	adds r4, r2, r0
	adds r0, r5, #0
	adds r0, #0xb8
	ldr r0, [r0]
	adds r1, r5, #0
	adds r1, #0xbc
	ldr r1, [r1]
	lsls r0, r0, #8
	str r0, [r4]
	lsls r1, r1, #8
	str r1, [r4, #4]
	subs r3, #1
	ldr r0, [r4, #0x20]
	ldr r1, _08027638 @ =0x000005ED
	adds r2, r2, r1
	ldr r1, [r0]
	ldrb r5, [r2]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08027614
	subs r3, r0, #1
_08027614:
	str r3, [r4, #0x30]
	adds r0, r4, #0
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
_08027620:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08027628: .4byte gUnknown_0300086C
_0802762C: .4byte gStaticData_08174C6C
_08027630: .4byte 0x000005AD
_08027634: .4byte gUnknown_030012C0
_08027638: .4byte 0x000005ED

	thumb_func_start sub_802763C
sub_802763C: @ 0x0802763C
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	ldr r1, _08027818 @ =gUnknown_0300086C
	movs r0, #0
	str r0, [r1]
	ldr r4, _0802781C @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_8023270
	ldr r1, [r5, #0x2c]
	cmp r1, r0
	beq _080276B6
	ldr r0, [r4]
	bl sub_8023270
	str r0, [r5, #0x2c]
	movs r1, #0xa
	bl sub_8037E54
	ldr r4, [r5, #0x64]
	movs r1, #0xe0
	lsls r1, r1, #2
	adds r6, r4, r1
	adds r3, r0, #0
	ldr r0, [r6, #0x20]
	ldr r2, _08027820 @ =0x000003AD
	adds r1, r4, r2
	ldr r2, [r0]
	ldrb r7, [r1]
	lsls r0, r7, #3
	adds r1, r7, #0
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08027688
	subs r3, r0, #1
_08027688:
	str r3, [r6, #0x30]
	ldr r0, [r5, #0x2c]
	movs r1, #0xa
	bl sub_803AF1C
	movs r1, #0xf0
	lsls r1, r1, #2
	adds r6, r4, r1
	adds r3, r0, #0
	ldr r0, [r6, #0x20]
	ldr r2, _08027824 @ =0x000003ED
	adds r1, r4, r2
	ldr r2, [r0]
	ldrb r4, [r1]
	lsls r0, r4, #3
	subs r0, r0, r4
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r1, [r0, #0x16]
	cmp r3, r1
	blt _080276B4
	subs r3, r1, #1
_080276B4:
	str r3, [r6, #0x30]
_080276B6:
	ldr r4, _0802781C @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_8023268
	ldr r1, [r5, #0x30]
	cmp r1, r0
	beq _08027726
	ldr r0, [r4]
	bl sub_8023268
	str r0, [r5, #0x30]
	movs r1, #0xa
	bl sub_8037E54
	ldr r4, [r5, #0x64]
	movs r7, #0x88
	lsls r7, r7, #3
	adds r6, r4, r7
	adds r3, r0, #0
	ldr r0, [r6, #0x20]
	ldr r2, _08027828 @ =0x0000046D
	adds r1, r4, r2
	ldr r2, [r0]
	ldrb r7, [r1]
	lsls r0, r7, #3
	adds r1, r7, #0
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _080276F8
	subs r3, r0, #1
_080276F8:
	str r3, [r6, #0x30]
	ldr r0, [r5, #0x30]
	movs r1, #0xa
	bl sub_803AF1C
	movs r1, #0x90
	lsls r1, r1, #3
	adds r6, r4, r1
	adds r3, r0, #0
	ldr r0, [r6, #0x20]
	ldr r2, _0802782C @ =0x000004AD
	adds r1, r4, r2
	ldr r2, [r0]
	ldrb r4, [r1]
	lsls r0, r4, #3
	subs r0, r0, r4
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r1, [r0, #0x16]
	cmp r3, r1
	blt _08027724
	subs r3, r1, #1
_08027724:
	str r3, [r6, #0x30]
_08027726:
	ldr r4, _0802781C @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_8023260
	ldr r1, [r5, #0x34]
	cmp r1, r0
	beq _08027788
	ldr r0, [r4]
	bl sub_8023260
	str r0, [r5, #0x34]
	ldr r4, [r5, #0x64]
	movs r7, #0xa0
	lsls r7, r7, #3
	adds r6, r4, r7
	adds r3, r0, #0
	ldr r0, [r6, #0x20]
	ldr r2, _08027830 @ =0x0000052D
	adds r1, r4, r2
	ldr r2, [r0]
	ldrb r7, [r1]
	lsls r0, r7, #3
	adds r1, r7, #0
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08027762
	subs r3, r0, #1
_08027762:
	str r3, [r6, #0x30]
	movs r0, #0xa8
	lsls r0, r0, #3
	adds r6, r4, r0
	movs r3, #0
	ldr r0, [r6, #0x20]
	ldr r2, _08027834 @ =0x0000056D
	adds r1, r4, r2
	ldr r2, [r0]
	ldrb r4, [r1]
	lsls r0, r4, #3
	subs r0, r0, r4
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r1, [r0, #0x16]
	cmp r3, r1
	blt _08027786
	subs r3, r1, #1
_08027786:
	str r3, [r6, #0x30]
_08027788:
	ldr r0, [r5, #0x64]
	movs r7, #0xe0
	lsls r7, r7, #2
	adds r0, r0, r7
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r5, #0x64]
	movs r1, #0xf0
	lsls r1, r1, #2
	adds r0, r0, r1
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r5, #0x64]
	movs r2, #0x88
	lsls r2, r2, #3
	adds r0, r0, r2
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r5, #0x64]
	movs r4, #0x90
	lsls r4, r4, #3
	adds r0, r0, r4
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r5, #0x64]
	movs r7, #0xa0
	lsls r7, r7, #3
	adds r0, r0, r7
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r5, #0x64]
	adds r4, #0xc0
	adds r0, r0, r4
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r5, #0x64]
	movs r1, #0x80
	lsls r1, r1, #3
	adds r0, r0, r1
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r5, #0x64]
	movs r2, #0x98
	lsls r2, r2, #3
	adds r0, r0, r2
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r5, #0x64]
	adds r0, r0, r4
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08027818: .4byte gUnknown_0300086C
_0802781C: .4byte gUnknown_030012C0
_08027820: .4byte 0x000003AD
_08027824: .4byte 0x000003ED
_08027828: .4byte 0x0000046D
_0802782C: .4byte 0x000004AD
_08027830: .4byte 0x0000052D
_08027834: .4byte 0x0000056D
