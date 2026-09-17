.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8038FD0
sub_8038FD0: @ 0x08038FD0
	adds r2, r0, #0
	movs r0, #1
	rsbs r0, r0, #0
	cmp r2, r0
	bne _08039030
	movs r2, #0
	ldr r3, _0803902C @ =gUnknown_03001630
	ldr r0, [r3]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0, #0x14]
	cmp r2, r0
	bhs _0803905E
_08038FF2:
	ldr r0, [r3]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	adds r0, r0, r2
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	adds r0, #0x24
	movs r1, #1
	strb r1, [r0]
	adds r2, #1
	ldr r0, [r3]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0, #0x14]
	cmp r2, r0
	blo _08038FF2
	b _0803905E
	.align 2, 0
_0803902C: .4byte gUnknown_03001630
_08039030:
	ldr r0, _08039060 @ =gUnknown_03001630
	ldr r0, [r0]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #0x14]
	cmp r2, r0
	bhs _0803905E
	cmp r2, #0
	blt _0803905E
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	adds r0, r0, r2
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	adds r0, #0x24
	movs r1, #1
	strb r1, [r0]
_0803905E:
	bx lr
	.align 2, 0
_08039060: .4byte gUnknown_03001630

	thumb_func_start sub_8039064
sub_8039064: @ 0x08039064
	push {r4, lr}
	adds r2, r0, #0
	adds r4, r1, #0
	cmp r4, #0xff
	bls _08039070
	movs r4, #0xff
_08039070:
	movs r0, #1
	rsbs r0, r0, #0
	cmp r2, r0
	bne _080390C4
	movs r2, #0
	ldr r3, _080390C0 @ =gUnknown_03001630
	ldr r0, [r3]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0, #0xc]
	cmp r2, r0
	bhs _080390EC
_08039092:
	ldr r0, [r3]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r1, [r0]
	lsls r0, r2, #2
	adds r0, r0, r1
	ldr r0, [r0, #0xc]
	strb r4, [r0, #0x18]
	adds r2, #1
	ldr r0, [r3]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0, #0xc]
	cmp r2, r0
	blo _08039092
	b _080390EC
	.align 2, 0
_080390C0: .4byte gUnknown_03001630
_080390C4:
	movs r0, #2
	rsbs r0, r0, #0
	cmp r2, r0
	ble _080390EC
	ldr r0, _080390F4 @ =gUnknown_03001630
	ldr r0, [r0]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r1, [r0]
	ldr r0, [r1]
	ldr r0, [r0]
	ldr r0, [r0, #0xc]
	cmp r2, r0
	bhs _080390EC
	lsls r0, r2, #2
	adds r0, r0, r1
	ldr r0, [r0, #0xc]
	strb r4, [r0, #0x18]
_080390EC:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080390F4: .4byte gUnknown_03001630

	thumb_func_start sub_80390F8
sub_80390F8: @ 0x080390F8
	push {r4, lr}
	adds r2, r0, #0
	adds r4, r1, #0
	cmp r4, #0xff
	bls _08039104
	movs r4, #0xff
_08039104:
	movs r0, #1
	rsbs r0, r0, #0
	cmp r2, r0
	bne _08039160
	movs r2, #0
	ldr r3, _0803915C @ =gUnknown_03001630
	ldr r0, [r3]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0, #0x14]
	cmp r2, r0
	bhs _0803918E
_08039124:
	ldr r0, [r3]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	adds r0, r0, r2
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	strb r4, [r0, #0x18]
	adds r2, #1
	ldr r0, [r3]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0, #0x14]
	cmp r2, r0
	blo _08039124
	b _0803918E
	.align 2, 0
_0803915C: .4byte gUnknown_03001630
_08039160:
	movs r0, #2
	rsbs r0, r0, #0
	cmp r2, r0
	ble _0803918E
	ldr r0, _08039194 @ =gUnknown_03001630
	ldr r0, [r0]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #0x14]
	cmp r2, r0
	bge _0803918E
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	adds r0, r0, r2
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	strb r4, [r0, #0x18]
_0803918E:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08039194: .4byte gUnknown_03001630

	thumb_func_start sub_8039198
sub_8039198: @ 0x08039198
	ldr r2, _080391D4 @ =gUnknown_03001630
	ldr r0, [r2]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r0, [r0, #4]
	movs r1, #0
	strb r1, [r0, #0x1a]
	ldr r0, [r2]
	str r1, [r0, #0x30]
	ldr r0, _080391D8 @ =0x04000084
	strh r1, [r0]
	ldr r2, _080391DC @ =0x040000C6
	ldr r3, _080391E0 @ =0x00008640
	adds r0, r3, #0
	strh r0, [r2]
	# this expands to adds r3, r3, #0, but for some reason the compiler changes it
	# from 1b 1c to 00 33, which is not correct
	.byte 0x1b
	.byte 0x1c
	mov r8, r8
	mov r8, r8
	mov r8, r8
	movs r3, #0xc8
	lsls r3, r3, #3
	adds r0, r3, #0
	strh r0, [r2]
	ldr r0, _080391E4 @ =0x04000100
	str r1, [r0]
	bx lr
	.align 2, 0
_080391D4: .4byte gUnknown_03001630
_080391D8: .4byte 0x04000084
_080391DC: .4byte 0x040000C6
_080391E0: .4byte 0x00008640
_080391E4: .4byte 0x04000100

	thumb_func_start sub_80391E8
sub_80391E8: @ 0x080391E8
	lsls r1, r0, #1
	adds r1, r1, r0
	lsls r1, r1, #2
	ldr r0, _0803920C @ =0x040000C6
	adds r1, r1, r0
	ldr r2, _08039210 @ =0x00008640
	adds r0, r2, #0
	strh r0, [r1]
	# this expands to adds r3, r3, #0, but for some reason the compiler changes it
	# from 1b 1c to 00 33, which is not correct
	.byte 0x1b
	.byte 0x1c
	mov r8, r8
	mov r8, r8
	mov r8, r8
	movs r2, #0xc8
	lsls r2, r2, #3
	adds r0, r2, #0
	strh r0, [r1]
	bx lr
	.align 2, 0
_0803920C: .4byte 0x040000C6
_08039210: .4byte 0x00008640

	thumb_func_start sub_8039214
sub_8039214: @ 0x08039214
	push {r4, r5, r6, r7, lr}
	adds r5, r2, #0
	movs r3, #0xc0
	lsls r3, r3, #0x13
	adds r2, r0, r3
	adds r0, r0, r2
	lsls r1, r1, #6
	adds r3, r0, r1
	ldrb r2, [r5]
	cmp r2, #0
	beq _080392BC
	movs r0, #0x40
	rsbs r0, r0, #0
	mov ip, r0
_08039230:
	movs r4, #0
	movs r0, #0x3f
	ands r0, r3
	lsrs r1, r0, #1
	ldrb r6, [r5]
	adds r7, r5, #1
	cmp r1, #0x1f
	bgt _08039268
	adds r0, r2, #0
	b _08039250
_08039244:
	adds r1, #1
	adds r4, #1
	cmp r1, #0x1f
	bgt _08039268
	adds r0, r5, r4
	ldrb r0, [r0]
_08039250:
	cmp r0, #0
	beq _08039268
	cmp r0, #0x20
	beq _08039268
	cmp r0, #0xa
	beq _08039268
	cmp r1, #0x1d
	ble _08039244
	mov r0, ip
	ands r0, r3
	adds r3, r0, #0
	adds r3, #0x40
_08039268:
	adds r1, r6, #0
	adds r5, r7, #0
	cmp r1, #0x5f
	bne _08039272
	movs r1, #0x5d
_08039272:
	cmp r1, #0x3a
	bne _08039278
	movs r1, #0x5c
_08039278:
	cmp r1, #0x2e
	bne _0803927E
	movs r1, #0x5b
_0803927E:
	cmp r1, #0xa
	bne _0803928A
	mov r0, ip
	ands r0, r3
	adds r3, r0, #0
	adds r3, #0x3f
_0803928A:
	cmp r1, #0x20
	bne _08039292
	movs r1, #0
	b _080392B2
_08039292:
	cmp r1, #0x40
	bhi _0803929C
	adds r0, r1, #0
	subs r0, #0x2f
	b _080392AE
_0803929C:
	cmp r1, #0x60
	bls _080392A6
	adds r0, r1, #0
	subs r0, #0x56
	b _080392AE
_080392A6:
	cmp r1, #0x40
	bls _080392B2
	adds r0, r1, #0
	subs r0, #0x36
_080392AE:
	lsls r0, r0, #0x18
	lsrs r1, r0, #0x18
_080392B2:
	strh r1, [r3]
	adds r3, #2
	ldrb r2, [r5]
	cmp r2, #0
	bne _08039230
_080392BC:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

