.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8022354
sub_8022354: @ 0x08022354
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	adds r5, r1, #0
	bl FreeVramDmaQueue
	ldr r0, _08022438 @ =gUnknown_03001300
	ldr r0, [r0]
	cmp r0, #0
	beq _0802236C
	movs r1, #3
	bl sub_8006AF4
_0802236C:
	ldr r0, _0802243C @ =gUnknown_030012FC
	ldr r0, [r0]
	cmp r0, #0
	beq _0802237A
	movs r1, #3
	bl sub_8006CD0
_0802237A:
	ldr r0, _08022440 @ =gUnknown_03001304
	ldr r0, [r0]
	cmp r0, #0
	beq _08022386
	bl sub_8026ED0
_08022386:
	ldr r4, _08022444 @ =gUnknown_030012BC
	ldr r0, [r4]
	bl sub_8001C64
	ldr r0, [r4]
	cmp r0, #0
	beq _0802239A
	movs r1, #3
	bl sub_8001C04
_0802239A:
	ldr r0, _08022448 @ =gUnknown_030012E0
	ldr r2, [r0]
	cmp r2, #0
	beq _080223B8
	movs r1, #0x98
	lsls r1, r1, #1
	adds r0, r2, r1
	ldr r1, [r0]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_080223B8:
	ldr r0, _0802244C @ =gUnknown_030012DC
	ldr r2, [r0]
	cmp r2, #0
	beq _080223D6
	movs r1, #0x98
	lsls r1, r1, #1
	adds r0, r2, r1
	ldr r1, [r0]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_080223D6:
	ldr r0, _08022450 @ =gUnknown_030012CC
	ldr r0, [r0]
	cmp r0, #0
	beq _080223E4
	movs r1, #3
	bl sub_8007A98
_080223E4:
	ldr r0, _08022454 @ =gUnknown_030012D0
	ldr r0, [r0]
	cmp r0, #0
	beq _080223F2
	movs r1, #3
	bl sub_8006FC8
_080223F2:
	ldr r0, _08022458 @ =gUnknown_030012B8
	ldr r0, [r0]
	cmp r0, #0
	beq _08022400
	movs r1, #3
	bl sub_8006F94
_08022400:
	ldr r0, _0802245C @ =gUnknown_030012B4
	ldr r0, [r0]
	cmp r0, #0
	beq _0802240E
	movs r1, #3
	bl sub_8025A44
_0802240E:
	ldr r0, _08022460 @ =gUnknown_030012C8
	ldr r0, [r0]
	cmp r0, #0
	beq _0802241C
	movs r1, #3
	bl sub_80270A8
_0802241C:
	ldr r1, _08022464 @ =gUnknown_03000828
	movs r0, #0
	str r0, [r1]
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _08022430
	adds r0, r6, #0
	bl sub_8026ED0
_08022430:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08022438: .4byte gUnknown_03001300
_0802243C: .4byte gUnknown_030012FC
_08022440: .4byte gUnknown_03001304
_08022444: .4byte gUnknown_030012BC
_08022448: .4byte gUnknown_030012E0
_0802244C: .4byte gUnknown_030012DC
_08022450: .4byte gUnknown_030012CC
_08022454: .4byte gUnknown_030012D0
_08022458: .4byte gUnknown_030012B8
_0802245C: .4byte gUnknown_030012B4
_08022460: .4byte gUnknown_030012C8
_08022464: .4byte gUnknown_03000828

	thumb_func_start sub_8022468
sub_8022468: @ 0x08022468
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	sub sp, #0x3c
	adds r6, r1, #0
	movs r0, #7
	movs r1, #0x7e
	str r0, [sp]
	str r1, [sp, #4]
	movs r0, #0xe4
	movs r1, #0x1e
	str r0, [sp, #8]
	str r1, [sp, #0xc]
	ldr r0, _0802257C @ =gUnknown_03001288
	mov r8, r0
	movs r4, #0
	movs r1, #0x40
	mov sb, r1
	mov r2, sb
	strh r2, [r0]
	movs r0, #4
	bl sub_8001524
	bl sub_80015B0
	bl sub_80015E0
	add r0, sp, #0x10
	strh r4, [r0]
	ldr r1, _08022580 @ =0x040000D4
	str r0, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	str r0, [r1, #4]
	ldr r0, _08022584 @ =0x81000100
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r0, _08022588 @ =0x04000020
	movs r3, #0x80
	lsls r3, r3, #1
	adds r1, r3, #0
	strh r1, [r0]
	adds r0, #2
	strh r4, [r0]
	adds r0, #2
	strh r4, [r0]
	adds r0, #2
	strh r1, [r0]
	adds r0, #2
	str r4, [r0]
	adds r0, #4
	str r4, [r0]
	ldr r5, _0802258C @ =gUnknown_030012B8
	ldr r0, [r5]
	bl sub_8006EA8
	ldr r4, _08022590 @ =gUnknown_030012DC
	ldr r0, [r4]
	movs r2, #0x80
	lsls r2, r2, #2
	movs r3, #0x84
	lsls r3, r3, #1
	adds r1, r0, r3
	str r2, [r1]
	subs r2, #0xd0
	adds r1, r0, r2
	ldr r1, [r1]
	adds r1, #0x40
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r1, [r1, #4]
	bl sub_803AD7C
	ldr r0, [r4]
	bl sub_8028A40
	ldr r0, [r5]
	bl sub_8006DC8
	add r5, sp, #0x14
	adds r0, r5, #0
	bl sub_8024948
	ldr r0, [r4]
	str r0, [sp, #0x28]
	ldr r0, [sp]
	ldr r1, [sp, #4]
	add r2, sp, #0x2c
	str r0, [sp, #0x2c]
	str r1, [r2, #4]
	ldr r0, [sp, #8]
	ldr r1, [sp, #0xc]
	str r0, [sp, #0x34]
	str r1, [r2, #0xc]
	mov r1, r8
	ldr r0, [r1]
	bl sub_8024784
	ldr r1, _08022594 @ =gStaticData_0816D1F4
	lsls r2, r6, #3
	adds r0, r2, r1
	ldr r0, [r0]
	str r0, [sp, #0x14]
	adds r1, #4
	adds r2, r2, r1
	ldr r0, [r2]
	str r0, [sp, #0x18]
	ldr r1, _08022598 @ =gUnknown_03000834
	ldr r0, _0802259C @ =gUnknown_03000868
	ldr r0, [r0]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	lsls r6, r6, #2
	adds r6, r6, r0
	ldr r0, [r6]
	str r0, [sp, #0x24]
	adds r0, r5, #0
	bl sub_8024820
	mov r3, sb
	mov r2, r8
	strh r3, [r2]
	bl sub_8001614
	adds r0, r5, #0
	movs r1, #2
	bl sub_802493C
	add sp, #0x3c
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802257C: .4byte gUnknown_03001288
_08022580: .4byte 0x040000D4
_08022584: .4byte 0x81000100
_08022588: .4byte 0x04000020
_0802258C: .4byte gUnknown_030012B8
_08022590: .4byte gUnknown_030012DC
_08022594: .4byte gStaticData_0816D1F4
_08022598: .4byte gUnknown_03000834
_0802259C: .4byte gUnknown_03000868

