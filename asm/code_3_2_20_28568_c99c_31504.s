.include "asm/macros.inc"

.syntax unified
.arm
	thumb_func_start sub_8031504
sub_8031504: @ 0x08031504
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	ldr r1, _080315A4 @ =0x0600BFC0
	movs r2, #0
	adds r0, r1, #0
	adds r0, #0x3c
_08031510:
	str r2, [r0]
	subs r0, #4
	cmp r0, r1
	bge _08031510
	mov r1, sp
	ldr r2, _080315A8 @ =0x0000FFFF
	adds r0, r2, #0
	strh r0, [r1]
	ldr r5, _080315AC @ =0x040000D4
	str r1, [r5]
	ldr r0, _080315B0 @ =0x0600C000
	str r0, [r5, #4]
	ldr r0, _080315B4 @ =0x81000800
	str r0, [r5, #8]
	ldr r0, [r5, #8]
	bl sub_8031604
	ldr r7, _080315B8 @ =gUnknown_03001538
	ldr r0, [r7]
	cmp r0, #0
	beq _080315F6
	ldr r1, _080315BC @ =gUnknown_03001524
	movs r0, #1
	strb r0, [r1]
	ldr r0, _080315C0 @ =gUnknown_03001520
	movs r6, #0
	str r6, [r0]
	ldr r0, _080315C4 @ =gUnknown_03001534
	ldr r2, [r0]
	ldr r3, [r2, #8]
	asrs r3, r3, #8
	ldr r1, [r2, #0xc]
	ldr r4, [r2]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r4
	movs r1, #2
	ldrsh r0, [r0, r1]
	adds r0, r0, r3
	ldr r1, [r2, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	bl sub_8030D48
	movs r2, #0x80
	lsls r2, r2, #0x13
	ldrh r0, [r2]
	movs r3, #0x80
	lsls r3, r3, #3
	adds r1, r3, #0
	orrs r0, r1
	strh r0, [r2]
	bl sub_80312C4
	ldr r1, _080315C8 @ =0x05000020
	ldr r0, _080315CC @ =gStaticData_08167AD4
	str r0, [r5]
	str r1, [r5, #4]
	ldr r0, _080315D0 @ =0x80000010
	str r0, [r5, #8]
	ldr r0, [r5, #8]
	ldr r0, [r7]
	cmp r0, #5
	bne _080315D4
	strh r6, [r1, #0x10]
	ldrh r0, [r1, #0x10]
	strh r0, [r1, #8]
	ldrh r0, [r1, #8]
	strh r0, [r1, #2]
	ldrh r0, [r1, #2]
	strh r0, [r1, #0x1e]
	b _080315F6
	.align 2, 0
_080315A4: .4byte 0x0600BFC0
_080315A8: .4byte 0x0000FFFF
_080315AC: .4byte 0x040000D4
_080315B0: .4byte 0x0600C000
_080315B4: .4byte 0x81000800
_080315B8: .4byte gUnknown_03001538
_080315BC: .4byte gUnknown_03001524
_080315C0: .4byte gUnknown_03001520
_080315C4: .4byte gUnknown_03001534
_080315C8: .4byte 0x05000020
_080315CC: .4byte gStaticData_08167AD4
_080315D0: .4byte 0x80000010
_080315D4:
	cmp r0, #4
	bne _080315F6
	ldr r2, _08031600 @ =gUnknown_0300153C
	ldr r0, [r2]
	cmp r0, #9
	bls _080315E2
	strh r6, [r1, #0x1e]
_080315E2:
	ldr r0, [r2]
	cmp r0, #0x31
	bls _080315EA
	strh r6, [r1, #2]
_080315EA:
	cmp r0, #0x4f
	bls _080315F0
	strh r6, [r1, #8]
_080315F0:
	cmp r0, #0x6d
	bls _080315F6
	strh r6, [r1, #0x10]
_080315F6:
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08031600: .4byte gUnknown_0300153C

	thumb_func_start sub_8031604
sub_8031604: @ 0x08031604
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x14
	movs r5, #0
	movs r3, #0x81
	lsls r3, r3, #2
	ldr r0, _08031704 @ =gUnknown_03001528
	ldr r1, _08031708 @ =gUnknown_0300152C
	ldr r2, [r0]
	ldr r0, [r1]
	muls r0, r2, r0
	adds r0, #1
	lsrs r0, r0, #1
	lsls r0, r0, #2
	str r0, [sp, #0x10]
	ldr r7, _0803170C @ =gUnknown_03001530
	ldr r4, _08031710 @ =gStaticData_08167AD4
	mov r1, sp
	ldr r6, _08031714 @ =gUnknown_03001580
	movs r2, #3
_08031632:
	adds r0, r3, r4
	ldr r0, [r0]
	str r0, [r1]
	adds r5, r5, r0
	adds r3, #4
	adds r0, r3, r4
	stm r6!, {r0}
	ldr r0, [sp, #0x10]
	adds r3, r3, r0
	ldm r1!, {r0}
	lsls r0, r0, #5
	adds r3, r3, r0
	subs r2, #1
	cmp r2, #0
	bge _08031632
	movs r0, #0xff
	subs r0, r0, r5
	str r0, [r7]
	lsls r0, r0, #6
	ldr r1, _08031718 @ =0x06008000
	adds r3, r0, r1
	movs r2, #0
_0803165E:
	lsls r1, r2, #2
	ldr r4, _08031714 @ =gUnknown_03001580
	adds r0, r1, r4
	ldr r0, [r0]
	add r1, sp
	mov sb, r3
	ldr r3, [sp, #0x10]
	adds r5, r0, r3
	ldr r1, [r1]
	mov r8, r1
	movs r4, #0
	mov ip, r4
	lsls r0, r1, #4
	adds r2, #1
	mov sl, r2
	cmp ip, r0
	bge _080316EC
	movs r6, #0xf
	movs r7, #0x10
_08031684:
	ldrb r0, [r5]
	adds r4, r6, #0
	ands r4, r0
	movs r1, #0
	cmp r4, #0
	beq _08031694
	adds r1, r7, #0
	orrs r1, r4
_08031694:
	adds r4, r1, #0
	lsrs r0, r0, #4
	ands r0, r6
	adds r5, #1
	movs r1, #0
	cmp r0, #0
	beq _080316A6
	adds r1, r7, #0
	orrs r1, r0
_080316A6:
	adds r0, r1, #0
	ldrb r3, [r5]
	adds r1, r6, #0
	ands r1, r3
	movs r2, #0
	cmp r1, #0
	beq _080316B8
	adds r2, r7, #0
	orrs r2, r1
_080316B8:
	adds r1, r2, #0
	lsrs r3, r3, #4
	ands r3, r6
	adds r5, #1
	movs r2, #0
	cmp r3, #0
	beq _080316CA
	adds r2, r7, #0
	orrs r2, r3
_080316CA:
	lsls r0, r0, #8
	orrs r0, r4
	lsls r1, r1, #0x10
	orrs r1, r0
	lsls r0, r2, #0x18
	orrs r0, r1
	mov r1, sb
	adds r1, #4
	mov sb, r1
	subs r1, #4
	stm r1!, {r0}
	movs r3, #1
	add ip, r3
	mov r4, r8
	lsls r0, r4, #4
	cmp ip, r0
	blt _08031684
_080316EC:
	mov r3, sb
	mov r2, sl
	cmp r2, #3
	ble _0803165E
	add sp, #0x14
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08031704: .4byte gUnknown_03001528
_08031708: .4byte gUnknown_0300152C
_0803170C: .4byte gUnknown_03001530
_08031710: .4byte gStaticData_08167AD4
_08031714: .4byte gUnknown_03001580
_08031718: .4byte 0x06008000

