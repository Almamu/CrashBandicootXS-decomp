.include "asm/macros.inc"

.syntax unified
.arm

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
