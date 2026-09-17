.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_80060AC
sub_80060AC: @ 0x080060AC
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	adds r7, r1, #0
	adds r5, r0, #0
	movs r6, #0
_080060B6:
	mov r0, sp
	adds r4, r0, r6
	adds r0, r5, #0
	movs r1, #0xa
	bl sub_803AE4C
	adds r0, #0x30
	strb r0, [r4]
	adds r0, r5, #0
	movs r1, #0xa
	bl sub_803ADB4
	adds r5, r0, #0
	adds r6, #1
	cmp r5, #0
	bne _080060B6
	movs r2, #0
_080060D8:
	subs r6, #1
	adds r0, r7, r6
	mov r3, sp
	adds r1, r3, r2
	ldrb r1, [r1]
	strb r1, [r0]
	adds r2, #1
	cmp r6, #0
	bne _080060D8
	adds r0, r7, r2
	strb r6, [r0]
	adds r0, r2, #0
	add sp, #0xc
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_80060F8
sub_80060F8: @ 0x080060F8
	push {r4, lr}
	adds r4, r2, #0
	lsls r0, r1, #2
	adds r0, r0, r1
	movs r1, #0x20
	strb r1, [r4]
	movs r1, #0x3c
	strb r1, [r4, #1]
	adds r1, r4, #2
	bl sub_80060AC
	adds r0, r0, r4
	movs r1, #0x25
	strb r1, [r0, #2]
	movs r1, #0x3e
	strb r1, [r0, #3]
	movs r1, #0
	strb r1, [r0, #4]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8006124
sub_8006124: @ 0x08006124
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	adds r0, #0x6c
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800613E
	adds r0, r6, #0
	adds r0, #0xbc
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
_0800613E:
	ldr r5, _08006194 @ =gUnknown_030012DC
	ldr r0, [r5]
	movs r4, #0x98
	lsls r4, r4, #1
	adds r1, r0, r4
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	adds r6, #0x7c
	ldr r2, [r2, #0x14]
	adds r1, r6, #0
	bl sub_803AD80
	ldr r1, _08006198 @ =gStaticData_0816B27C
	lsrs r0, r0, #1
	ldr r2, [r1]
	subs r2, r2, r0
	ldr r0, [r5]
	subs r2, #2
	ldr r3, [r1, #4]
	subs r3, #0x23
	movs r5, #0x88
	lsls r5, r5, #1
	adds r1, r0, r5
	str r2, [r1]
	movs r2, #0x8a
	lsls r2, r2, #1
	adds r1, r0, r2
	str r3, [r1]
	adds r4, r0, r4
	ldr r2, [r4]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	adds r1, r6, #0
	bl sub_803AD80
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08006194: .4byte gUnknown_030012DC
_08006198: .4byte gStaticData_0816B27C

	thumb_func_start sub_800619C
sub_800619C: @ 0x0800619C
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r0, #0x88
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	ldr r2, _080061E0 @ =gStaticData_0816B1E4
	ldr r0, _080061E4 @ =gUnknown_030012DC
	ldr r3, [r0]
	ldr r1, [r2]
	subs r1, #0x2c
	ldr r2, [r2, #4]
	subs r2, #8
	movs r5, #0x88
	lsls r5, r5, #1
	adds r0, r3, r5
	str r1, [r0]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r3, r1
	str r2, [r0]
	adds r1, r4, #0
	adds r1, #0x2c
	adds r2, r4, #0
	adds r2, #0x46
	adds r0, r4, #0
	bl sub_8005E5C
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080061E0: .4byte gStaticData_0816B1E4
_080061E4: .4byte gUnknown_030012DC

	thumb_func_start sub_80061E8
sub_80061E8: @ 0x080061E8
	push {r4, r5, r6, lr}
	ldr r1, _08006248 @ =gStaticData_0816B1D0
	ldr r0, [r0, #0x24]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	bl sub_8026F38
	adds r6, r0, #0
	ldr r5, _0800624C @ =gUnknown_030012DC
	ldr r0, [r5]
	movs r4, #0x98
	lsls r4, r4, #1
	adds r1, r0, r4
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	adds r1, r6, #0
	bl sub_803AD80
	lsrs r0, r0, #1
	movs r2, #0xc2
	subs r2, r2, r0
	ldr r0, [r5]
	movs r3, #0x2c
	movs r5, #0x88
	lsls r5, r5, #1
	adds r1, r0, r5
	str r2, [r1]
	movs r2, #0x8a
	lsls r2, r2, #1
	adds r1, r0, r2
	str r3, [r1]
	adds r4, r0, r4
	ldr r2, [r4]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	adds r1, r6, #0
	bl sub_803AD80
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08006248: .4byte gStaticData_0816B1D0
_0800624C: .4byte gUnknown_030012DC

	thumb_func_start sub_8006250
sub_8006250: @ 0x08006250
	push {r4, lr}
	adds r4, r0, #0
	bl sub_80006A8
	ldr r0, _0800629C @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _080062A0 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	movs r1, #0xa0
	lsls r1, r1, #0x13
	movs r0, #0
	strh r0, [r1]
	ldr r1, _080062A4 @ =0x04000050
	adds r0, r4, #0
	adds r0, #0xc8
	ldr r0, [r0]
	str r0, [r1]
	adds r1, #4
	adds r0, r4, #0
	adds r0, #0xcc
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	adds r0, r4, #0
	adds r0, #0xd0
	ldrh r0, [r0]
	strh r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0800629C: .4byte gUnknown_030012B8
_080062A0: .4byte gUnknown_03001300
_080062A4: .4byte 0x04000050

	thumb_func_start sub_80062A8
sub_80062A8: @ 0x080062A8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	mov sb, r0
	mov sl, r1
	adds r7, r2, #0
	movs r0, #0xc0
	lsls r0, r0, #0x18
	bl mem_free_bytes
	bl sub_80006A8
	movs r0, #0xa0
	lsls r0, r0, #0x13
	movs r1, #0
	strh r1, [r0]
	movs r0, #0x80
	lsls r0, r0, #0x13
	strh r1, [r0]
	ldr r1, _080063C8 @ =gUnknown_030012B8
	ldr r0, [r1]
	bl sub_8006EA8
	ldr r5, _080063CC @ =gUnknown_030012DC
	ldr r0, [r5]
	bl sub_8028A40
	ldr r2, _080063D0 @ =gUnknown_030012E0
	mov r8, r2
	ldr r0, [r2]
	bl sub_8028A40
	ldr r6, _080063D4 @ =gUnknown_030012FC
	ldr r0, [r6]
	movs r4, #0
	str r4, [r0, #8]
	bl sub_8006C4C
	ldr r0, [r6]
	bl sub_8006C4C
	ldr r0, [r5]
	movs r3, #0x84
	lsls r3, r3, #1
	adds r1, r0, r3
	str r4, [r1]
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
	ldr r0, [r6]
	ldr r1, [r5]
	movs r2, #0x96
	lsls r2, r2, #1
	adds r1, r1, r2
	ldr r1, [r1]
	lsls r1, r1, #5
	bl sub_8006C58
	ldr r0, [r5]
	movs r3, #0x96
	lsls r3, r3, #1
	adds r0, r0, r3
	ldr r2, [r0]
	mov r1, r8
	ldr r0, [r1]
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
	ldr r0, [r6]
	mov r2, r8
	ldr r1, [r2]
	movs r3, #0x96
	lsls r3, r3, #1
	adds r1, r1, r3
	ldr r1, [r1]
	lsls r1, r1, #5
	bl sub_8006C58
	ldr r0, [r6]
	bl sub_8006C30
	movs r0, #0x2c
	bl sub_8026EDC
	adds r5, r0, #0
	mov r0, sb
	bl sub_8026F38
	adds r4, r0, #0
	mov r0, sl
	bl sub_8026F38
	adds r2, r0, #0
	adds r0, r5, #0
	adds r1, r4, #0
	adds r3, r7, #0
	bl sub_80063D8
	adds r4, r0, #0
	bl sub_8006518
	cmp r4, #0
	beq _080063A8
	adds r0, r4, #0
	movs r1, #3
	bl sub_8006770
_080063A8:
	ldr r1, _080063C8 @ =gUnknown_030012B8
	ldr r0, [r1]
	bl sub_8006EA8
	movs r0, #0xc0
	lsls r0, r0, #0x18
	bl mem_free_bytes
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080063C8: .4byte gUnknown_030012B8
_080063CC: .4byte gUnknown_030012DC
_080063D0: .4byte gUnknown_030012E0
_080063D4: .4byte gUnknown_030012FC

	thumb_func_start sub_80063D8
sub_80063D8: @ 0x080063D8
	push {r4, r5, r6, lr}
	mov r6, sl
	mov r5, sb
	mov r4, r8
	push {r4, r5, r6}
	sub sp, #4
	adds r5, r0, #0
	mov r8, r1
	mov sb, r2
	mov sl, r3
	movs r0, #3
	str r0, [sp]
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0x1f
	movs r3, #0
	bl sub_801E644
	movs r6, #0
	str r6, [r5, #0x20]
	adds r2, r5, #0
	adds r2, #0x20
	movs r0, #0xc0
	ldrb r1, [r2]
	orrs r0, r1
	movs r1, #0x20
	orrs r0, r1
	movs r3, #1
	orrs r0, r3
	movs r1, #2
	orrs r0, r1
	movs r1, #4
	orrs r0, r1
	movs r1, #8
	orrs r0, r1
	movs r4, #0x10
	orrs r0, r4
	strb r0, [r2]
	adds r2, #4
	movs r0, #0x20
	rsbs r0, r0, #0
	ldrb r1, [r2]
	ands r0, r1
	orrs r0, r4
	strb r0, [r2]
	ldr r1, _08006500 @ =0x04000050
	ldr r0, [r5, #0x20]
	str r0, [r1]
	adds r1, #4
	ldrb r2, [r2]
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	strh r6, [r5, #0x28]
	adds r2, r5, #0
	adds r2, #0x28
	movs r0, #0x40
	ldrb r1, [r2]
	orrs r0, r1
	movs r1, #8
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r2]
	adds r0, r5, #0
	adds r0, #0x29
	ldrb r1, [r0]
	orrs r3, r1
	orrs r3, r4
	strb r3, [r0]
	mov r3, r8
	str r3, [r5, #0x10]
	mov r0, sb
	str r0, [r5, #0x14]
	ldr r1, _08006504 @ =gStaticData_0816C484
	adds r0, r5, #0
	bl LoadGraphicsPackage
	str r6, [r5, #0x1c]
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	adds r4, r0, #0
	str r4, [r5, #0x18]
	ldr r0, _08006508 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xe4
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r3, sl
	strb r3, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r5, #0x18]
	movs r1, #0xf0
	lsls r1, r1, #7
	str r1, [r0]
	movs r1, #0xa0
	lsls r1, r1, #7
	str r1, [r0, #4]
	bl sub_800815C
	ldr r2, [r5, #0x18]
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	adds r0, r5, #0
	bl sub_801E640
	ldr r1, _0800650C @ =0x04000008
	strh r0, [r1]
	ldr r0, _08006510 @ =0x04000010
	str r6, [r0]
	ldr r0, _08006514 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0xf
	bl sub_8001B54
	adds r0, r5, #0
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08006500: .4byte 0x04000050
_08006504: .4byte gStaticData_0816C484
_08006508: .4byte gUnknown_030012D0
_0800650C: .4byte 0x04000008
_08006510: .4byte 0x04000010
_08006514: .4byte gUnknown_030012BC

	thumb_func_start sub_8006518
sub_8006518: @ 0x08006518
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #0x24
	movs r0, #0x1f
	ldrb r1, [r6]
	ands r0, r1
	cmp r0, #0
	beq _08006564
	movs r2, #0x20
	rsbs r2, r2, #0
	adds r7, r2, #0
_08006534:
	adds r4, r6, #0
	ldrb r2, [r6]
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1b
	subs r0, #1
	movs r1, #0x1f
	ands r0, r1
	ands r2, r7
	orrs r2, r0
	strb r2, [r6]
	adds r0, r5, #0
	bl sub_8006600
	adds r0, r5, #0
	bl sub_8006714
	adds r0, r5, #0
	bl sub_8006700
	movs r0, #0x1f
	ldrb r4, [r4]
	ands r0, r4
	cmp r0, #0
	bne _08006534
_08006564:
	adds r4, r5, #0
	adds r4, #0x24
	movs r0, #0x28
	adds r0, r0, r5
	mov r8, r0
_0800656E:
	adds r0, r5, #0
	bl sub_8006600
	adds r0, r5, #0
	bl sub_8006714
	adds r0, r5, #0
	bl sub_8006700
	ldr r0, _080065F8 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r1, _080065FC @ =gUnknown_030007E0
	movs r0, #8
	ldrh r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _0800656E
	adds r6, r4, #0
	movs r0, #0x1f
	ldrb r1, [r6]
	ands r0, r1
	cmp r0, #0x10
	beq _080065D6
	movs r2, #0x20
	rsbs r2, r2, #0
	adds r7, r2, #0
_080065A6:
	adds r4, r6, #0
	ldrb r2, [r6]
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1b
	adds r0, #1
	movs r1, #0x1f
	ands r0, r1
	ands r2, r7
	orrs r2, r0
	strb r2, [r6]
	adds r0, r5, #0
	bl sub_8006600
	adds r0, r5, #0
	bl sub_8006714
	adds r0, r5, #0
	bl sub_8006700
	movs r0, #0x1f
	ldrb r4, [r4]
	ands r0, r4
	cmp r0, #0x10
	bne _080065A6
_080065D6:
	movs r0, #0
	strh r0, [r5, #0x28]
	movs r0, #0x40
	mov r1, r8
	ldrb r1, [r1]
	orrs r0, r1
	mov r2, r8
	strb r0, [r2]
	adds r0, r5, #0
	bl sub_8006714
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080065F8: .4byte gUnknown_03001304
_080065FC: .4byte gUnknown_030007E0

@ sub_8006600 is reconstructed (but not yet byte-matching) as C in
@ src/oam_count.c, guarded by #if NON_MATCHING - this raw version is
@ only assembled for the default (matching) build. See docs/graphics.md,
@ "Parked, not matched: sub_8006600".
.if NON_MATCHING == 0
	thumb_func_start sub_8006600
sub_8006600: @ 0x08006600
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #0x10
	adds r4, r0, #0
	ldr r0, _080066F0 @ =gUnknown_03001300
	mov sb, r0
	ldr r0, [r0]
	bl sub_8006A90
	ldr r0, _080066F4 @ =gUnknown_030012FC
	ldr r0, [r0]
	bl sub_8006C28
	ldr r0, [r4, #0x18]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	ldr r1, _080066F8 @ =gUnknown_030012E0
	mov r8, r1
	ldr r0, [r1]
	movs r5, #0x98
	lsls r5, r5, #1
	adds r1, r0, r5
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r4, #0x10]
	ldr r2, [r2, #0x14]
	bl sub_803AD80
	movs r6, #0xf0
	subs r0, r6, r0
	lsrs r3, r0, #1
	mov r7, r8
	ldr r0, [r7]
	movs r2, #0x2d
	movs r7, #0x88
	lsls r7, r7, #1
	adds r1, r0, r7
	str r3, [r1]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r1, r0, r3
	str r2, [r1]
	adds r1, r0, r5
	ldr r2, [r1]
	movs r7, #0x20
	ldrsh r1, [r2, r7]
	adds r0, r0, r1
	ldr r1, [r4, #0x10]
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	mov r0, sp
	movs r1, #0x10
	movs r2, #0x6a
	bl sub_803AFE4
	mov r0, sp
	movs r1, #0xd0
	movs r2, #0x35
	bl sub_803AFDC
	ldr r0, [r4, #0x14]
	ldr r4, _080066FC @ =gUnknown_030012DC
	ldr r1, [r4]
	mov r2, sp
	movs r3, #0
	bl sub_8001214
	movs r0, #0x2e
	bl sub_8026F38
	mov r8, r0
	ldr r0, [r4]
	adds r1, r0, r5
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	mov r1, r8
	bl sub_803AD80
	subs r6, r6, r0
	lsrs r3, r6, #1
	ldr r0, [r4]
	movs r2, #0x90
	movs r4, #0x88
	lsls r4, r4, #1
	adds r1, r0, r4
	str r3, [r1]
	movs r7, #0x8a
	lsls r7, r7, #1
	adds r1, r0, r7
	str r2, [r1]
	adds r5, r0, r5
	ldr r2, [r5]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	mov r1, r8
	bl sub_803AD80
	mov r4, sb
	ldr r0, [r4]
	bl sub_8006A48
	add sp, #0x10
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080066F0: .4byte gUnknown_03001300
_080066F4: .4byte gUnknown_030012FC
_080066F8: .4byte gUnknown_030012E0
_080066FC: .4byte gUnknown_030012DC
.endif
