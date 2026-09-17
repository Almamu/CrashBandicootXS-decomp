.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8038240
sub_8038240: @ 0x08038240
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x28
	str r0, [sp]
	str r1, [sp, #4]
	str r2, [sp, #8]
	str r3, [sp, #0xc]
	ldr r0, [r0]
	mov sb, r0
	cmp r1, #0
	beq _08038268
	ldr r0, _08038298 @ =gUnknown_03001630
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	cmp r0, #0
	bne _08038268
	add sb, r2
_08038268:
	movs r6, #0
	ldr r1, [sp]
	ldr r1, [r1]
	str r1, [sp, #0x18]
	cmp r6, sb
	bhs _08038310
	movs r2, #0
	str r2, [sp, #0x20]
	lsls r0, r1, #2
	ldr r3, [sp, #4]
	subs r0, r3, r0
	str r0, [sp, #0x24]
	ldr r0, [sp]
	mov sl, r0
	str r1, [sp, #0x10]
_08038286:
	ldr r1, [sp, #0xc]
	ldr r5, [r1]
	str r5, [sp, #0x1c]
	ldr r2, [sp, #0x10]
	cmp r6, r2
	bge _0803829C
	mov r3, sl
	ldr r4, [r3, #4]
	b _080382A0
	.align 2, 0
_08038298: .4byte gUnknown_03001630
_0803829C:
	ldr r0, [sp, #0x24]
	ldr r4, [r0]
_080382A0:
	cmp r6, #2
	beq _080382FA
	ldr r0, [r4, #0xc]
	cmp r6, #0
	bne _080382AE
	ldr r1, [sp, #8]
	adds r0, r0, r1
_080382AE:
	lsls r0, r0, #2
	mov r8, r0
	adds r0, #0xc
	ldr r3, [r4, #0x14]
	adds r0, r0, r3
	mov ip, r0
	ldr r2, [sp, #0x48]
	ldr r7, [r2]
	cmp r7, ip
	bhs _080382C6
	movs r0, #0
	b _080384C6
_080382C6:
	ldr r0, _0803833C @ =gUnknown_03001630
	ldr r2, [r0]
	ldr r1, [r2, #0x10]
	lsls r1, r1, #2
	adds r0, r2, #0
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [sp, #0x20]
	adds r0, r1, r0
	str r5, [r0]
	str r4, [r5]
	ldr r0, [r2, #0x14]
	str r0, [r5, #4]
	adds r0, r3, #0
	adds r0, #0xc
	ldr r2, [sp, #0x1c]
	adds r0, r2, r0
	str r0, [r5, #8]
	add r0, r8
	ldr r3, [sp, #0xc]
	str r0, [r3]
	mov r1, ip
	subs r0, r7, r1
	ldr r2, [sp, #0x48]
	str r0, [r2]
_080382FA:
	ldr r3, [sp, #0x20]
	adds r3, #4
	str r3, [sp, #0x20]
	ldr r0, [sp, #0x24]
	adds r0, #4
	str r0, [sp, #0x24]
	movs r1, #4
	add sl, r1
	adds r6, #1
	cmp r6, sb
	blo _08038286
_08038310:
	movs r6, #0
	cmp r6, sb
	bhs _080383B6
_08038316:
	ldr r2, _0803833C @ =gUnknown_03001630
	ldr r0, [r2]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	lsls r1, r6, #2
	adds r0, r1, r0
	ldr r0, [r0]
	mov ip, r0
	ldr r3, [sp, #0x18]
	cmp r6, r3
	bge _08038340
	ldr r2, [sp]
	adds r0, r1, r2
	ldr r0, [r0, #4]
	b _0803834C
	.align 2, 0
_0803833C: .4byte gUnknown_03001630
_08038340:
	ldr r3, [sp, #0x18]
	subs r0, r6, r3
	lsls r0, r0, #2
	ldr r1, [sp, #4]
	adds r0, r0, r1
	ldr r0, [r0]
_0803834C:
	adds r2, r6, #1
	str r2, [sp, #0x14]
	cmp r6, #2
	beq _080383B0
	movs r2, #0
	ldr r3, [r0, #0xc]
	mov r8, r3
	cmp r2, r8
	bhs _080383B0
	ldr r7, [r0, #0x10]
	mov sl, r7
_08038362:
	lsls r0, r2, #2
	mov r3, sl
	adds r1, r0, r3
	ldr r1, [r1]
	adds r3, r0, #0
	adds r5, r2, #1
	cmp r1, #0
	beq _080383AA
	movs r2, #0
	cmp r2, sb
	bhs _080383AA
	ldr r6, _080383A0 @ =gUnknown_03001630
	adds r0, r3, r7
	ldr r4, [r0]
_0803837E:
	ldr r0, [r6]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r1, [r0]
	lsls r0, r2, #2
	adds r0, r0, r1
	ldr r1, [r0]
	ldr r0, [r1]
	cmp r0, r4
	bne _080383A4
	mov r2, ip
	ldr r0, [r2, #8]
	adds r0, r3, r0
	str r1, [r0]
	b _080383AA
	.align 2, 0
_080383A0: .4byte gUnknown_03001630
_080383A4:
	adds r2, #1
	cmp r2, sb
	blo _0803837E
_080383AA:
	adds r2, r5, #0
	cmp r2, r8
	blo _08038362
_080383B0:
	ldr r6, [sp, #0x14]
	cmp r6, sb
	blo _08038316
_080383B6:
	ldr r3, _080384D8 @ =gUnknown_03001630
	ldr r0, [r3]
	ldr r0, [r0, #0x10]
	cmp r0, #1
	bne _08038402
	ldr r0, [sp, #4]
	cmp r0, #0
	beq _08038456
	movs r6, #0
	ldr r1, [sp, #8]
	cmp r6, r1
	bge _08038402
	adds r5, r3, #0
	ldr r2, [sp, #0x18]
	lsls r4, r2, #2
_080383D4:
	ldr r2, [r5]
	ldr r1, [r2, #0x10]
	lsls r1, r1, #2
	adds r0, r2, #0
	adds r0, #8
	adds r0, r0, r1
	ldr r3, [r0]
	adds r3, r4, r3
	ldr r0, [r2, #8]
	ldr r1, [r0]
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	adds r0, r0, r6
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	str r0, [r3]
	adds r4, #4
	adds r6, #1
	ldr r3, [sp, #8]
	cmp r6, r3
	blt _080383D4
_08038402:
	ldr r0, [sp, #4]
	cmp r0, #0
	beq _08038456
	movs r6, #0
	ldr r1, [sp, #8]
	cmp r6, r1
	bge _08038456
	ldr r4, _080384D8 @ =gUnknown_03001630
_08038412:
	ldr r1, [r4]
	ldr r0, [r1, #0x10]
	lsls r0, r0, #2
	adds r1, #8
	adds r1, r1, r0
	ldr r2, [sp, #0x18]
	adds r3, r2, r6
	ldr r2, [r1]
	lsls r3, r3, #2
	adds r0, r3, r2
	ldr r0, [r0]
	ldr r1, [r0, #8]
	ldr r0, [r2, #4]
	str r0, [r1]
	ldr r0, [r4]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r2, [r0]
	ldr r1, [r2]
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	adds r0, r0, r6
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r0, r0, r1
	adds r3, r3, r2
	ldr r1, [r3]
	str r1, [r0]
	adds r6, #1
	ldr r3, [sp, #8]
	cmp r6, r3
	blt _08038412
_08038456:
	movs r6, #0
	ldr r0, [sp, #0x18]
	subs r0, #3
	cmp r6, r0
	bge _08038484
	ldr r2, _080384D8 @ =gUnknown_03001630
_08038462:
	ldr r0, [r2]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r1, [r0]
	lsls r0, r6, #2
	adds r0, r0, r1
	ldr r0, [r0, #0xc]
	adds r0, #0x53
	strb r6, [r0]
	adds r6, #1
	ldr r1, [sp]
	ldr r0, [r1]
	subs r0, #3
	cmp r6, r0
	blt _08038462
_08038484:
	ldr r2, _080384D8 @ =gUnknown_03001630
	ldr r0, [r2]
	ldr r0, [r0, #0x24]
	cmp r0, #0
	beq _080384C4
	movs r6, #0
	ldr r3, [sp]
	ldr r7, [r3, #4]
_08038494:
	ldr r1, _080384D8 @ =gUnknown_03001630
	ldr r0, [r1]
	ldr r5, [r0, #0x24]
	lsls r4, r6, #3
	adds r5, r4, r5
	ldr r1, [r7, #0x18]
	adds r1, r4, r1
	ldr r0, [r0, #0x14]
	ldrh r2, [r0, #2]
	ldr r0, [r1, #4]
	muls r0, r2, r0
	movs r1, #0xfa
	lsls r1, r1, #2
	bl sub_8037E54
	lsls r0, r0, #1
	str r0, [r5]
	ldr r0, [r7, #0x18]
	adds r4, r4, r0
	ldr r0, [r4, #8]
	str r0, [r5, #4]
	adds r6, #1
	cmp r6, #2
	ble _08038494
_080384C4:
	movs r0, #1
_080384C6:
	add sp, #0x28
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080384D8: .4byte gUnknown_03001630

	thumb_func_start sub_80384DC
sub_80384DC: @ 0x080384DC
	ldr r1, _08038524 @ =0x040000C6
	ldr r2, _08038528 @ =0x00008640
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
	subs r1, #2
	movs r0, #4
	str r0, [r1]
	subs r1, #0x40
	movs r0, #0
	strh r0, [r1]
	subs r1, #2
	ldr r2, _0803852C @ =0x00000B04
	adds r0, r2, #0
	strh r0, [r1]
	ldr r2, _08038530 @ =0x040000A0
	movs r1, #0
	movs r0, #7
_0803850E:
	strh r1, [r2]
	subs r0, #1
	cmp r0, #0
	bge _0803850E
	ldr r1, _08038534 @ =0x04000089
	movs r0, #0x42
	strb r0, [r1]
	adds r1, #0x37
	ldr r0, _08038530 @ =0x040000A0
	str r0, [r1]
	bx lr
	.align 2, 0
_08038524: .4byte 0x040000C6
_08038528: .4byte 0x00008640
_0803852C: .4byte 0x00000B04
_08038530: .4byte 0x040000A0
_08038534: .4byte 0x04000089

