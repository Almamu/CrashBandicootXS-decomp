.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8014674
sub_8014674: @ 0x08014674
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r2, [r4, #0x10]
	adds r1, r2, #0
	adds r1, #0x68
	movs r0, #8
	ldrb r1, [r1]
	ands r0, r1
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	cmp r5, #0
	bne _08014690
	b _0801480A
_08014690:
	movs r0, #1
	ldrb r1, [r2, #0xd]
	orrs r0, r1
	strb r0, [r2, #0xd]
	adds r0, r4, #0
	adds r0, #0x34
	movs r5, #0
	strb r5, [r0]
	ldr r2, [r4, #0x10]
	adds r0, r2, #0
	adds r0, #0x2d
	ldrb r0, [r0]
	cmp r0, #0xd
	beq _080146B4
	cmp r0, #0x18
	bne _08014778
	cmp r0, #0xd
	bne _0801473E
_080146B4:
	adds r0, r4, #0
	adds r0, #0x29
	ldrb r0, [r0]
	cmp r0, #0
	beq _080146FC
	str r5, [r4, #0x18]
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r2, #0
	movs r2, #0x18
	bl sub_803AD84
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #4
	bl sub_803AD80
	movs r1, #0x1b
	adds r0, r4, #0
	adds r0, #0x31
	strb r5, [r0]
	adds r2, r4, #0
	adds r2, #0x2f
	movs r0, #1
	strb r0, [r2]
	adds r0, r4, #0
	adds r0, #0x27
	strb r1, [r0]
	b _08014726
_080146FC:
	str r5, [r4, #0x18]
	ldr r1, [r4, #0xc]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #3
	bl sub_803AD80
	adds r2, r4, #0
	adds r2, #0x27
	ldrb r0, [r2]
	cmp r0, #1
	beq _08014726
	movs r0, #1
	adds r1, r4, #0
	adds r1, #0x31
	strb r5, [r1]
	subs r1, #2
	strb r0, [r1]
	strb r0, [r2]
_08014726:
	movs r2, #0
	adds r0, r4, #0
	adds r0, #0x32
	strb r2, [r0]
	adds r1, r4, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x28
	strb r2, [r0]
	b _08014934
_0801473E:
	cmp r0, #0x18
	beq _08014744
	b _08014934
_08014744:
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #4
	bl sub_803AD80
	adds r0, r4, #0
	adds r0, #0x32
	strb r5, [r0]
	subs r0, #2
	movs r1, #1
	strb r1, [r0]
	subs r0, #8
	strb r5, [r0]
	adds r0, #1
	strb r1, [r0]
	movs r2, #0x1b
	adds r0, #8
	strb r5, [r0]
	subs r0, #2
	strb r1, [r0]
	subs r0, #8
	strb r2, [r0]
	b _08014934
_08014778:
	ldr r0, [r4, #8]
	cmp r0, #0xe
	bne _080147DC
	ldr r0, _0801479C @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #0x30
	ands r0, r1
	cmp r0, #0
	beq _080147A0
	movs r0, #1
	adds r1, r4, #0
	adds r1, #0x31
	strb r5, [r1]
	subs r1, #2
	strb r0, [r1]
	subs r1, #8
	strb r0, [r1]
	b _080147B4
	.align 2, 0
_0801479C: .4byte gUnknown_030007E0
_080147A0:
	adds r0, r4, #0
	adds r0, #0x31
	strb r5, [r0]
	adds r1, r4, #0
	adds r1, #0x2f
	movs r0, #1
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x27
	strb r5, [r0]
_080147B4:
	movs r2, #0
	adds r0, r4, #0
	adds r0, #0x32
	strb r2, [r0]
	adds r1, r4, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x28
	strb r2, [r0]
	ldr r1, [r4, #0xc]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xd
	bl sub_803AD80
	b _08014934
_080147DC:
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0
	bl sub_803AD80
	adds r0, r4, #0
	adds r0, #0x32
	strb r5, [r0]
	subs r0, #2
	movs r1, #1
	strb r1, [r0]
	subs r0, #8
	strb r5, [r0]
	adds r0, #9
	strb r5, [r0]
	subs r0, #2
	strb r1, [r0]
	subs r0, #8
	strb r5, [r0]
	b _08014934
_0801480A:
	ldr r7, _08014858 @ =gUnknown_030007E0
	ldr r0, [r7]
	str r0, [sp]
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r6, #1
	movs r3, #1
	ands r3, r1
	cmp r3, #0
	beq _0801485C
	ldr r1, [r4, #0xc]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #5
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x13
	bl sub_803AD84
	str r5, [r4, #0x18]
	movs r1, #7
	adds r0, r4, #0
	adds r0, #0x32
	strb r5, [r0]
	subs r0, #2
	strb r6, [r0]
	subs r0, #8
	strb r1, [r0]
	b _08014910
	.align 2, 0
_08014858: .4byte gUnknown_030007E0
_0801485C:
	movs r0, #2
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r5, r0, #0x10
	cmp r5, #0
	beq _080148C0
	movs r0, #1
	ldrb r1, [r2, #0xd]
	orrs r0, r1
	strb r0, [r2, #0xd]
	adds r0, r4, #0
	adds r0, #0x34
	strb r3, [r0]
	ldr r1, [r7]
	movs r0, #0x30
	ands r1, r0
	cmp r1, #0
	beq _08014890
	adds r0, r4, #0
	adds r0, #0x31
	strb r3, [r0]
	subs r0, #2
	strb r6, [r0]
	subs r0, #8
	strb r6, [r0]
	b _0801489E
_08014890:
	adds r0, r4, #0
	adds r0, #0x31
	strb r1, [r0]
	subs r0, #2
	strb r6, [r0]
	subs r0, #8
	strb r1, [r0]
_0801489E:
	ldr r0, [r4, #8]
	subs r0, #0xd
	cmp r0, #1
	bls _080148AE
	adds r0, r4, #0
	bl sub_8015398
	b _08014910
_080148AE:
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xd
	bl sub_803AD80
	b _08014910
_080148C0:
	mov r1, sp
	movs r0, #0x80
	lsls r0, r0, #1
	ldrh r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _08014910
	movs r0, #1
	ldrb r3, [r2, #0xd]
	orrs r0, r3
	strb r0, [r2, #0xd]
	adds r0, r4, #0
	adds r0, #0x34
	strb r5, [r0]
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x10
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r3, #0
	ldrsh r0, [r2, r3]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #3
	bl sub_803AD84
	str r5, [r4, #0x1c]
	adds r0, r4, #0
	adds r0, #0x31
	strb r5, [r0]
	subs r0, #2
	strb r6, [r0]
	subs r0, #8
	strb r5, [r0]
_08014910:
	ldr r0, _0801493C @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r1, r0, #0x18
	cmp r1, #0
	bne _08014934
	adds r0, r4, #0
	adds r0, #0x31
	strb r1, [r0]
	adds r2, r4, #0
	adds r2, #0x2f
	movs r0, #1
	strb r0, [r2]
	adds r0, r4, #0
	adds r0, #0x27
	strb r1, [r0]
_08014934:
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801493C: .4byte gUnknown_03001304

	thumb_func_start sub_8014940
sub_8014940: @ 0x08014940
	push {r4, lr}
	adds r4, r0, #0
	ldr r1, [r4, #0x10]
	adds r0, r1, #0
	adds r0, #0x2d
	ldrb r0, [r0]
	cmp r0, #0x2f
	bne _0801496A
	ldr r0, [r1, #0x30]
	cmp r0, #3
	bne _0801496A
	ldr r0, [r1, #0x34]
	cmp r0, #0
	bne _0801496A
	ldr r0, _080149B0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x2e
	bl PlaySfx
_0801496A:
	ldr r1, [r4, #0x10]
	adds r0, r1, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _080149A8
	movs r0, #1
	ldrb r2, [r1, #0xc]
	orrs r0, r2
	strb r0, [r1, #0xc]
	ldr r0, _080149B4 @ =0x0000FFFF
	ldrh r4, [r1, #8]
	cmp r4, r0
	beq _080149A8
	ldrh r3, [r1, #8]
	ldr r0, _080149B8 @ =gUnknown_030012B4
	ldr r2, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r1, r0, #2
	movs r4, #0x84
	lsls r4, r4, #1
	adds r2, r2, r4
	adds r2, r2, r1
	lsls r0, r0, #5
	subs r0, r3, r0
	movs r1, #1
	lsls r1, r0
	ldr r0, [r2]
	orrs r0, r1
	str r0, [r2]
_080149A8:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080149B0: .4byte gUnknown_030012BC
_080149B4: .4byte 0x0000FFFF
_080149B8: .4byte gUnknown_030012B4

	thumb_func_start sub_80149BC
sub_80149BC: @ 0x080149BC
	push {r4, r5, lr}
	sub sp, #4
	adds r5, r0, #0
	ldr r0, [r5, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08014A2A
	ldr r0, _08014A34 @ =gUnknown_030012D8
	ldr r1, [r0]
	movs r0, #0x80
	ldrb r2, [r1, #0xc]
	orrs r0, r2
	strb r0, [r1, #0xc]
	movs r4, #0
	str r4, [sp]
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0x12
	movs r3, #0
	bl sub_8015780
	adds r0, r5, #0
	adds r0, #0x31
	strb r4, [r0]
	subs r0, #2
	movs r1, #1
	strb r1, [r0]
	subs r0, #8
	strb r4, [r0]
	adds r0, #0xb
	strb r4, [r0]
	subs r0, #2
	strb r1, [r0]
	subs r0, #8
	strb r4, [r0]
	ldr r0, _08014A38 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r3, [r5, #0x10]
	adds r1, r3, #0
	adds r1, #0x29
	ldrb r1, [r1]
	lsls r1, r1, #0x1c
	lsrs r1, r1, #0x1c
	ldr r2, [r3, #0x20]
	adds r3, #0x2d
	ldr r4, [r2]
	ldrb r5, [r3]
	lsls r2, r5, #3
	subs r2, r2, r5
	lsls r2, r2, #2
	adds r2, r2, r4
	ldrb r2, [r2, #0x14]
	bl sub_8006D08
_08014A2A:
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08014A34: .4byte gUnknown_030012D8
_08014A38: .4byte gUnknown_030012B8

	thumb_func_start sub_8014A3C
sub_8014A3C: @ 0x08014A3C
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _08014ABC @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r1, r0, #0x18
	ldr r0, _08014AC0 @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp]
	cmp r1, #0
	beq _08014A9A
	cmp r1, #8
	bgt _08014A9A
	cmp r1, #3
	blt _08014A9A
	movs r1, #0x20
	adds r2, r4, #0
	adds r2, #0x31
	movs r0, #0
	strb r0, [r2]
	subs r2, #2
	movs r0, #1
	strb r0, [r2]
	adds r0, r4, #0
	adds r0, #0x27
	strb r1, [r0]
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x25
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x20
	bl sub_803AD84
_08014A9A:
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08014AC8
	ldr r0, _08014AC4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xd
	bl PlaySfx
	adds r0, r4, #0
	bl sub_8014B54
	b _08014AE4
	.align 2, 0
_08014ABC: .4byte gUnknown_03001304
_08014AC0: .4byte gUnknown_030007E0
_08014AC4: .4byte gUnknown_030012BC
_08014AC8:
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _08014ADE
	adds r0, r4, #0
	bl sub_80153FC
	adds r0, r4, #0
	bl sub_80122CC
	b _08014AE4
_08014ADE:
	adds r0, r4, #0
	bl sub_80122CC
_08014AE4:
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8014AEC
sub_8014AEC: @ 0x08014AEC
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _08014B1C @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp]
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r5, #1
	ands r5, r1
	cmp r5, #0
	beq _08014B24
	ldr r0, _08014B20 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xd
	bl PlaySfx
	adds r0, r4, #0
	bl sub_8014B54
	b _08014B4C
	.align 2, 0
_08014B1C: .4byte gUnknown_030007E0
_08014B20: .4byte gUnknown_030012BC
_08014B24:
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _08014B4C
	adds r0, r4, #0
	bl sub_80153FC
	adds r0, r4, #0
	bl sub_80122CC
	adds r0, r4, #0
	adds r0, #0x31
	strb r5, [r0]
	adds r1, r4, #0
	adds r1, #0x2f
	movs r0, #1
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x27
	strb r5, [r0]
_08014B4C:
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_8014B54
sub_8014B54: @ 0x08014B54
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x10]
	ldr r1, _08014BC8 @ =0x00000101
	adds r0, r0, r1
	movs r5, #0
	strb r5, [r0]
	ldr r1, [r4, #0x10]
	ldr r0, [r1, #4]
	movs r3, #0xc0
	lsls r3, r3, #3
	adds r0, r0, r3
	str r0, [r1, #4]
	ldr r1, [r4, #0xc]
	movs r6, #0x20
	ldrsh r0, [r1, r6]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x1a
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x1b
	bl sub_803AD84
	ldr r1, [r4, #0x10]
	ldr r0, [r1, #0x20]
	adds r3, r1, #0
	adds r3, #0x2d
	ldr r2, [r0]
	ldrb r6, [r3]
	lsls r0, r6, #3
	subs r0, r0, r6
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #0x16]
	subs r0, #1
	str r0, [r1, #0x30]
	movs r2, #4
	adds r0, r4, #0
	adds r0, #0x32
	strb r5, [r0]
	adds r1, r4, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	adds r4, #0x28
	strb r2, [r4]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08014BC8: .4byte 0x00000101

	thumb_func_start sub_8014BCC
sub_8014BCC: @ 0x08014BCC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _08014C14 @ =gUnknown_03001304
	ldr r2, [r0]
	ldr r0, _08014C18 @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp]
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r6, #1
	movs r5, #1
	ands r5, r1
	cmp r5, #0
	beq _08014C20
	ldr r0, _08014C1C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xd
	bl PlaySfx
	movs r0, #0
	adds r1, r4, #0
	adds r1, #0x31
	strb r0, [r1]
	subs r1, #2
	strb r6, [r1]
	subs r1, #8
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_8014B54
	b _08014D0A
	.align 2, 0
_08014C14: .4byte gUnknown_03001304
_08014C18: .4byte gUnknown_030007E0
_08014C1C: .4byte gUnknown_030012BC
_08014C20:
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _08014C36
	adds r0, r4, #0
	bl sub_80153FC
	adds r0, r4, #0
	bl sub_80122CC
	b _08014C68
_08014C36:
	adds r0, r2, #0
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	cmp r5, #0
	bne _08014C78
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x28
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r7, #0
	ldrsh r0, [r2, r7]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x22
	bl sub_803AD84
_08014C68:
	adds r0, r4, #0
	adds r0, #0x31
	strb r5, [r0]
	subs r0, #2
	strb r6, [r0]
	subs r0, #8
	strb r5, [r0]
	b _08014D0A
_08014C78:
	adds r0, r4, #0
	adds r0, #0x27
	ldrb r2, [r0]
	mov r8, r0
	cmp r2, #0
	bne _08014C9C
	cmp r5, #8
	bgt _08014C9C
	cmp r5, #3
	blt _08014C9C
	movs r0, #0x20
	adds r1, r4, #0
	adds r1, #0x31
	strb r2, [r1]
	subs r1, #2
	strb r6, [r1]
	mov r1, r8
	strb r0, [r1]
_08014C9C:
	ldr r0, [r4, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08014D04
	movs r6, #0
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x26
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r7, #0
	ldrsh r0, [r2, r7]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x21
	bl sub_803AD84
	str r6, [r4, #0x18]
	ldr r3, [r4, #0x10]
	movs r5, #5
	ldr r0, [r3, #0x20]
	adds r2, r3, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r7, [r2]
	lsls r0, r7, #3
	adds r2, r7, #0
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r5, r0
	blt _08014CEE
	subs r5, r0, #1
_08014CEE:
	str r5, [r3, #0x30]
	movs r1, #0x20
	adds r0, r4, #0
	adds r0, #0x31
	strb r6, [r0]
	adds r2, r4, #0
	adds r2, #0x2f
	movs r0, #1
	strb r0, [r2]
	mov r0, r8
	strb r1, [r0]
_08014D04:
	adds r0, r4, #0
	bl sub_80122CC
_08014D0A:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8014D18
sub_8014D18: @ 0x08014D18
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _08014D94 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	mov r8, r0
	ldr r0, _08014D98 @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp]
	ldr r5, [r4, #0x10]
	adds r0, r5, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08014D5E
	movs r0, #0x21
	adds r1, r5, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r5, #0
	bl sub_80087C0
	adds r0, r5, #0
	bl sub_80087B4
	adds r0, r5, #0
	movs r1, #0
	bl sub_800872C
_08014D5E:
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r7, #1
	movs r6, #1
	ands r6, r1
	cmp r6, #0
	beq _08014DA0
	ldr r0, _08014D9C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xd
	bl PlaySfx
	movs r0, #0
	adds r1, r4, #0
	adds r1, #0x31
	strb r0, [r1]
	subs r1, #2
	strb r7, [r1]
	subs r1, #8
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_8014B54
	b _08014ED4
	.align 2, 0
_08014D94: .4byte gUnknown_03001304
_08014D98: .4byte gUnknown_030007E0
_08014D9C: .4byte gUnknown_030012BC
_08014DA0:
	movs r0, #2
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r5, r0, #0x10
	cmp r5, #0
	beq _08014DC8
	adds r0, r4, #0
	bl sub_80153FC
	adds r0, r4, #0
	bl sub_80122CC
	adds r0, r4, #0
	adds r0, #0x31
	strb r6, [r0]
	subs r0, #2
	strb r7, [r0]
	subs r0, #8
	strb r6, [r0]
	b _08014ED4
_08014DC8:
	mov r0, r8
	cmp r0, #0
	bne _08014E40
	ldr r0, [r4, #0x18]
	adds r0, #1
	str r0, [r4, #0x18]
	cmp r0, #3
	ble _08014E52
	str r5, [r4, #0x18]
	ldr r0, [r4, #0x10]
	ldr r0, [r0, #0x30]
	cmp r0, #0
	beq _08014E0C
	cmp r0, #4
	bgt _08014E08
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x28
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x23
	b _08014E2C
_08014E08:
	cmp r0, #9
	ble _08014E30
_08014E0C:
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x28
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x22
_08014E2C:
	bl sub_803AD84
_08014E30:
	adds r0, r4, #0
	adds r0, #0x31
	strb r5, [r0]
	subs r0, #2
	strb r7, [r0]
	subs r0, #8
	strb r5, [r0]
	b _08014E52
_08014E40:
	str r5, [r4, #0x18]
	movs r0, #0x20
	adds r1, r4, #0
	adds r1, #0x31
	strb r5, [r1]
	subs r1, #2
	strb r7, [r1]
	subs r1, #8
	strb r0, [r1]
_08014E52:
	adds r0, r4, #0
	bl sub_80122CC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08014ED4
	ldr r0, [r4, #0x10]
	bl sub_80083B8
	adds r2, r0, #0
	ldr r0, [r2, #4]
	ldrb r0, [r0]
	lsrs r0, r0, #4
	cmp r0, #6
	bhi _08014EA4
	lsls r0, r0, #2
	ldr r1, _08014E7C @ =_08014E80
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08014E7C: .4byte _08014E80
_08014E80: @ jump table
	.4byte _08014E9C @ case 0
	.4byte _08014EA4 @ case 1
	.4byte _08014EA4 @ case 2
	.4byte _08014EA4 @ case 3
	.4byte _08014EA4 @ case 4
	.4byte _08014EA4 @ case 5
	.4byte _08014EA0 @ case 6
_08014E9C:
	adds r2, #0x24
	b _08014EA6
_08014EA0:
	adds r2, #0x14
	b _08014EA6
_08014EA4:
	ldr r2, _08014EC4 @ =gStaticData_0816B300
_08014EA6:
	ldr r1, [r4, #0x10]
	ldr r0, [r1]
	asrs r3, r0, #8
	ldr r4, [r1, #4]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _08014EC8
	movs r5, #0
	ldrsh r0, [r2, r5]
	adds r3, r3, r0
	b _08014ECE
	.align 2, 0
_08014EC4: .4byte gStaticData_0816B300
_08014EC8:
	movs r5, #0
	ldrsh r0, [r2, r5]
	subs r3, r3, r0
_08014ECE:
	lsls r0, r3, #8
	str r0, [r1]
	str r4, [r1, #4]
_08014ED4:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8014EE0
sub_8014EE0: @ 0x08014EE0
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _08014F20 @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp]
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r7, #1
	movs r5, #1
	ands r5, r1
	cmp r5, #0
	beq _08014F28
	ldr r0, _08014F24 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xd
	bl PlaySfx
	movs r0, #0
	adds r1, r4, #0
	adds r1, #0x31
	strb r0, [r1]
	subs r1, #2
	strb r7, [r1]
	subs r1, #8
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_8014B54
	b _08014F82
	.align 2, 0
_08014F20: .4byte gUnknown_030007E0
_08014F24: .4byte gUnknown_030012BC
_08014F28:
	movs r0, #2
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r6, r0, #0x10
	cmp r6, #0
	beq _08014F50
	adds r0, r4, #0
	bl sub_80153FC
	adds r0, r4, #0
	bl sub_80122CC
	adds r0, r4, #0
	adds r0, #0x31
	strb r5, [r0]
	subs r0, #2
	strb r7, [r0]
	subs r0, #8
	strb r5, [r0]
	b _08014F82
_08014F50:
	ldr r0, [r4, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08014F82
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x20
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x1f
	bl sub_803AD84
	str r6, [r4, #0x18]
	str r6, [r4, #0x1c]
_08014F82:
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8014F8C
sub_8014F8C: @ 0x08014F8C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r4, r0, #0
	ldr r1, [r4, #0x10]
	ldr r0, [r1]
	asrs r0, r0, #8
	ldr r1, [r1, #4]
	asrs r1, r1, #8
	movs r2, #0x40
	movs r3, #0x12
	bl sub_800F6B8
	movs r0, #0x40
	mov r8, r0
	ldr r1, [r4, #0x10]
	ldr r0, [r1]
	asrs r7, r0, #8
	ldr r0, [r1, #4]
	asrs r6, r0, #8
	movs r5, #0
	b _0801501E
_08014FB8:
	ldr r0, _08015034 @ =gUnknown_030012F0
	ldr r0, [r0]
	ldr r1, [r0, #0xc]
	lsls r0, r5, #2
	adds r0, r0, r1
	ldr r4, [r0]
	ldr r1, [r4, #0x18]
	adds r1, #0x48
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	cmp r0, #4
	ble _0801501C
	ldr r1, [r4]
	asrs r1, r1, #8
	subs r1, r1, r7
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	ldr r0, [r4, #4]
	asrs r0, r0, #8
	subs r0, r0, r6
	asrs r2, r0, #0x1f
	eors r0, r2
	subs r2, r0, r2
	adds r1, r1, r2
	cmp r1, r8
	bgt _0801501C
	ldrb r1, [r4, #0xc]
	lsrs r0, r1, #6
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _0801501C
	cmp r2, #0x11
	bgt _0801501C
	ldr r1, [r4, #0x18]
	adds r1, #0x68
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0x16
	movs r3, #0
	bl sub_803AD88
_0801501C:
	adds r5, #1
_0801501E:
	ldr r0, _08015034 @ =gUnknown_030012F0
	ldr r0, [r0]
	ldr r0, [r0, #4]
	cmp r5, r0
	blt _08014FB8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08015034: .4byte gUnknown_030012F0

	thumb_func_start sub_8015038
sub_8015038: @ 0x08015038
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r6, r0, #0
	mov ip, r1
	adds r3, r2, #0
	adds r0, #0x24
	ldrb r0, [r0]
	cmp r0, #0
	bne _080150E6
	movs r1, #0x17
	mov r8, r1
	adds r1, r6, #0
	adds r1, #0x21
	strb r0, [r1]
	adds r0, r6, #0
	adds r0, #0x22
	ldrb r2, [r0]
	adds r7, r1, #0
	adds r5, r0, #0
	cmp r2, #1
	bne _0801506C
	movs r0, #0x28
	mov r8, r0
	b _08015074
_0801506C:
	cmp r2, #2
	bne _08015076
	movs r1, #0x27
	mov r8, r1
_08015074:
	strb r2, [r7]
_08015076:
	movs r2, #0
	mov sb, r2
	movs r4, #0x14
	ldr r1, [r6, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x24]
	mov r1, ip
	bl sub_803AD80
	ldr r2, [r6, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r6, r0
	ldr r1, [r6, #0x10]
	ldr r3, [r2, #4]
	mov r2, r8
	bl sub_803AD84
	mov r2, sb
	str r2, [r6, #0x18]
	str r4, [r6, #0x1c]
	ldr r0, _080150DC @ =gUnknown_030012BC
	ldr r0, [r0]
	ldrb r1, [r7]
	adds r1, #0x57
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	ldrb r0, [r5]
	adds r0, #1
	strb r0, [r5]
	adds r1, r6, #0
	adds r1, #0x20
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	ldrb r1, [r1]
	cmp r0, r1
	blo _080151AE
	adds r0, r6, #0
	adds r0, #0x24
	movs r1, #1
	strb r1, [r0]
	ldrb r0, [r5]
	cmp r0, #1
	bls _080150E0
	strb r1, [r5]
	b _080151AE
	.align 2, 0
_080150DC: .4byte gUnknown_030012BC
_080150E0:
	mov r1, sb
	strb r1, [r5]
	b _080151AE
_080150E6:
	adds r0, r6, #0
	adds r0, #0x22
	adds r5, r0, #0
	ldrb r2, [r5]
	cmp r2, #0xf0
	bls _0801515C
	movs r0, #0x17
	mov r8, r0
	adds r1, r6, #0
	adds r1, #0x21
	movs r0, #0
	strb r0, [r1]
	ldrb r0, [r5]
	adds r7, r1, #0
	cmp r0, #1
	bne _0801510C
	movs r1, #0x28
	mov r8, r1
	b _08015114
_0801510C:
	cmp r0, #2
	bne _08015116
	movs r2, #0x27
	mov r8, r2
_08015114:
	strb r0, [r7]
_08015116:
	movs r4, #0
	movs r0, #0x14
	mov sb, r0
	ldr r1, [r6, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x24]
	mov r1, ip
	bl sub_803AD80
	ldr r2, [r6, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r6, r0
	ldr r1, [r6, #0x10]
	ldr r3, [r2, #4]
	mov r2, r8
	bl sub_803AD84
	str r4, [r6, #0x18]
	mov r2, sb
	str r2, [r6, #0x1c]
	ldr r0, _08015158 @ =gUnknown_030012BC
	ldr r0, [r0]
	ldrb r1, [r7]
	adds r1, #0x57
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	b _080151A8
	.align 2, 0
_08015158: .4byte gUnknown_030012BC
_0801515C:
	adds r0, r6, #0
	adds r0, #0x21
	movs r4, #0
	strb r4, [r0]
	subs r0, #1
	strb r4, [r0]
	movs r7, #0x18
	ldr r1, [r6, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x24]
	adds r1, r3, #0
	bl sub_803AD80
	ldr r2, [r6, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r6, r0
	ldr r1, [r6, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x10
	bl sub_803AD84
	str r4, [r6, #0x18]
	str r7, [r6, #0x1c]
	ldr r0, _080151C4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xa
	bl PlaySfx
	adds r1, r6, #0
	adds r1, #0x26
	movs r0, #0x63
	strb r0, [r1]
_080151A8:
	ldrb r0, [r5]
	subs r0, #1
	strb r0, [r5]
_080151AE:
	adds r1, r6, #0
	adds r1, #0x23
	movs r0, #0
	strb r0, [r1]
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080151C4: .4byte gUnknown_030012BC

	thumb_func_start sub_80151C8
sub_80151C8: @ 0x080151C8
	adds r3, r0, #0
	adds r1, r3, #0
	adds r1, #0x23
	ldrb r0, [r1]
	cmp r0, #0
	bne _08015234
	movs r0, #1
	strb r0, [r1]
	adds r0, r3, #0
	adds r0, #0x22
	ldrb r0, [r0]
	cmp r0, #4
	bhi _08015222
	lsls r0, r0, #2
	ldr r1, _080151EC @ =_080151F0
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080151EC: .4byte _080151F0
_080151F0: @ jump table
	.4byte _08015204 @ case 0
	.4byte _08015204 @ case 1
	.4byte _08015208 @ case 2
	.4byte _0801520C @ case 3
	.4byte _0801520C @ case 4
_08015204:
	movs r2, #0x18
	b _0801520E
_08015208:
	movs r2, #0x19
	b _0801520E
_0801520C:
	movs r2, #0x1a
_0801520E:
	adds r1, r3, #0
	adds r1, #0x32
	movs r0, #0
	strb r0, [r1]
	subs r1, #2
	movs r0, #1
	strb r0, [r1]
	adds r0, r3, #0
	adds r0, #0x28
	strb r2, [r0]
_08015222:
	ldr r1, [r3, #0x10]
	movs r0, #1
	ldrb r2, [r1, #0xd]
	orrs r0, r2
	strb r0, [r1, #0xd]
	adds r1, r3, #0
	adds r1, #0x34
	movs r0, #0
	strb r0, [r1]
_08015234:
	bx lr
	.align 2, 0

	thumb_func_start sub_8015238
sub_8015238: @ 0x08015238
	push {r4, r5, lr}
	sub sp, #4
	adds r5, r0, #0
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	adds r3, r5, #0
	adds r3, #0x26
	movs r0, #0xc
	strb r0, [r3]
	cmp r1, #4
	bgt _080152BC
	cmp r1, #3
	blt _080152BC
	movs r1, #0x80
	lsls r1, r1, #2
	adds r0, r1, #0
	ands r2, r0
	cmp r2, #0
	beq _080152B4
	ldr r0, _080152B0 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231C4
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _080152B4
	adds r0, r5, #0
	adds r0, #0x29
	movs r4, #1
	strb r4, [r0]
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #4
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x18
	bl sub_803AD84
	movs r1, #0x1b
	adds r2, r5, #0
	adds r2, #0x31
	movs r0, #0
	strb r0, [r2]
	adds r0, r5, #0
	adds r0, #0x2f
	strb r4, [r0]
	subs r0, #8
	strb r1, [r0]
	b _080152E8
	.align 2, 0
_080152B0: .4byte gUnknown_030012C0
_080152B4:
	adds r0, r5, #0
	bl sub_8015460
	b _080152E8
_080152BC:
	movs r4, #0
	str r4, [sp]
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0x12
	movs r3, #0
	bl sub_8015780
	adds r0, r5, #0
	adds r0, #0x31
	strb r4, [r0]
	subs r0, #2
	movs r1, #1
	strb r1, [r0]
	subs r0, #8
	strb r4, [r0]
	adds r0, #0xb
	strb r4, [r0]
	subs r0, #2
	strb r1, [r0]
	subs r0, #8
	strb r4, [r0]
_080152E8:
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_80152F0
sub_80152F0: @ 0x080152F0
	push {r4, lr}
	adds r2, r0, #0
	lsls r1, r1, #0x18
	lsrs r4, r1, #0x18
	movs r0, #0x27
	adds r0, r0, r2
	mov ip, r0
	ldrb r0, [r0]
	cmp r0, #0
	bne _08015328
	adds r0, r2, #0
	adds r0, #0x2b
	ldrb r3, [r0]
	cmp r3, #0
	bne _08015328
	cmp r4, #4
	bgt _08015328
	cmp r4, #3
	blt _08015328
	movs r1, #0x17
	adds r0, #6
	strb r3, [r0]
	adds r3, r2, #0
	adds r3, #0x2f
	movs r0, #1
	strb r0, [r3]
	mov r0, ip
	strb r1, [r0]
_08015328:
	cmp r4, #2
	bhi _08015342
	movs r1, #0
	adds r0, r2, #0
	adds r0, #0x31
	strb r1, [r0]
	adds r3, r2, #0
	adds r3, #0x2f
	movs r0, #1
	strb r0, [r3]
	adds r0, r2, #0
	adds r0, #0x27
	strb r1, [r0]
_08015342:
	adds r0, r2, #0
	bl sub_80122CC
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8015350
sub_8015350: @ 0x08015350
	push {r4, lr}
	adds r2, r0, #0
	adds r0, #0x33
	movs r4, #0
	strb r4, [r0]
	ldr r0, [r2, #8]
	adds r3, r2, #0
	adds r3, #0x2d
	strb r0, [r3]
	str r1, [r2, #8]
	adds r0, r2, #0
	adds r0, #0x2c
	strb r4, [r0]
	subs r0, #1
	strb r4, [r0]
	subs r1, #0xd
	cmp r1, #1
	bls _0801538E
	ldr r0, [r2, #0x10]
	adds r0, #0x90
	strb r4, [r0]
	ldr r1, _08015394 @ =gUnknown_030012D8
	ldr r0, [r1]
	adds r0, #0x92
	strb r4, [r0]
	ldr r0, [r1]
	adds r0, #0x94
	strb r4, [r0]
	ldr r0, [r1]
	adds r0, #0x94
	strb r4, [r0]
_0801538E:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08015394: .4byte gUnknown_030012D8

	thumb_func_start sub_8015398
sub_8015398: @ 0x08015398
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r0, #0x26
	ldrb r5, [r0]
	cmp r5, #0
	bne _080153F2
	ldr r0, _080153F8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xa
	bl PlaySfx
	str r5, [r4, #0x18]
	movs r0, #0x18
	str r0, [r4, #0x1c]
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x10
	bl sub_803AD84
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xd
	bl sub_803AD80
	adds r0, r4, #0
	adds r0, #0x21
	strb r5, [r0]
	subs r0, #1
	strb r5, [r0]
	adds r0, #2
	strb r5, [r0]
	adds r0, #1
	strb r5, [r0]
	adds r0, #1
	strb r5, [r0]
_080153F2:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080153F8: .4byte gUnknown_030012BC

	thumb_func_start sub_80153FC
sub_80153FC: @ 0x080153FC
	push {r4, r5, lr}
	adds r5, r0, #0
	adds r0, #0x26
	ldrb r4, [r0]
	cmp r4, #0
	bne _08015456
	ldr r0, _0801545C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xa
	bl PlaySfx
	str r4, [r5, #0x18]
	movs r0, #0x18
	str r0, [r5, #0x1c]
	adds r0, r5, #0
	adds r0, #0x21
	strb r4, [r0]
	subs r0, #1
	strb r4, [r0]
	adds r0, #2
	strb r4, [r0]
	adds r0, #1
	strb r4, [r0]
	adds r0, #1
	strb r4, [r0]
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x1e
	bl sub_803AD84
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x21
	bl sub_803AD80
_08015456:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0801545C: .4byte gUnknown_030012BC

	thumb_func_start sub_8015460
sub_8015460: @ 0x08015460
	push {r4, r5, lr}
	adds r5, r0, #0
	adds r0, #0x29
	ldrb r4, [r0]
	cmp r4, #0
	beq _080154BA
	movs r4, #0
	str r4, [r5, #0x18]
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x18
	bl sub_803AD84
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #4
	bl sub_803AD80
	movs r0, #0x1b
	adds r2, r5, #0
	adds r2, #0x31
	strb r4, [r2]
	adds r1, r5, #0
	adds r1, #0x2f
	movs r3, #1
	strb r3, [r1]
	subs r1, #8
	strb r0, [r1]
	ldr r0, [r5, #0x10]
	movs r1, #0x80
	lsls r1, r1, #1
	adds r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0
	beq _08015502
	strb r3, [r2]
	b _08015502
_080154BA:
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r3, #0
	ldrsh r0, [r2, r3]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0xd
	bl sub_803AD84
	str r4, [r5, #0x18]
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #3
	bl sub_803AD80
	movs r1, #1
	adds r2, r5, #0
	adds r2, #0x31
	strb r4, [r2]
	adds r0, r5, #0
	adds r0, #0x2f
	strb r1, [r0]
	subs r0, #8
	strb r1, [r0]
	ldr r0, [r5, #0x10]
	movs r3, #0x80
	lsls r3, r3, #1
	adds r0, r0, r3
	ldrb r0, [r0]
	cmp r0, #0
	beq _08015502
	strb r1, [r2]
_08015502:
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_8015508
sub_8015508: @ 0x08015508
	push {r4, r5, lr}
	adds r4, r0, #0
	movs r5, #0
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xb
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0xb
	bl sub_803AD84
	str r5, [r4, #0x18]
	movs r2, #0xb
	adds r0, r4, #0
	adds r0, #0x32
	strb r5, [r0]
	adds r1, r4, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x28
	strb r2, [r0]
	ldr r0, [r4, #0x10]
	adds r0, #0x68
	strb r5, [r0]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8015558
sub_8015558: @ 0x08015558
	push {r4, r5, lr}
	adds r4, r0, #0
	movs r5, #0
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xb
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0xb
	bl sub_803AD84
	str r5, [r4, #0x18]
	movs r2, #7
	adds r0, r4, #0
	adds r0, #0x32
	strb r5, [r0]
	adds r1, r4, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x28
	strb r2, [r0]
	ldr r0, [r4, #0x10]
	adds r0, #0x68
	strb r5, [r0]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80155A8
sub_80155A8: @ 0x080155A8
	str r1, [r0, #0x10]
	bx lr

	thumb_func_start sub_80155AC
sub_80155AC: @ 0x080155AC
	push {lr}
	bl sub_8014B54
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80155B8
sub_80155B8: @ 0x080155B8
	push {r4, r5, lr}
	adds r5, r0, #0
	ldr r0, [r5, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _080155F0
	movs r4, #0
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x20
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x1f
	bl sub_803AD84
	str r4, [r5, #0x18]
	str r4, [r5, #0x1c]
_080155F0:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80155F8
sub_80155F8: @ 0x080155F8
	push {r4, r5, lr}
	adds r5, r0, #0
	ldr r0, [r5, #0x18]
	adds r0, #1
	str r0, [r5, #0x18]
	ldr r1, [r5, #0x1c]
	cmp r0, r1
	bge _08015612
	ldr r0, [r5, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08015644
_08015612:
	adds r1, r5, #0
	adds r1, #0x26
	movs r4, #0
	movs r0, #0xc
	strb r0, [r1]
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x20
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x1f
	bl sub_803AD84
	str r4, [r5, #0x18]
	str r4, [r5, #0x1c]
_08015644:
	adds r0, r5, #0
	bl sub_80122CC
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_8015650
sub_8015650: @ 0x08015650
	push {r4, r5, lr}
	adds r5, r0, #0
	ldr r0, [r5, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08015688
	movs r4, #0
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x20
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x1f
	bl sub_803AD84
	str r4, [r5, #0x18]
	str r4, [r5, #0x1c]
_08015688:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8015690
sub_8015690: @ 0x08015690
	push {lr}
	ldr r0, [r0, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _080156AC
	ldr r0, _080156B0 @ =gUnknown_030012D8
	ldr r1, [r0]
	movs r0, #0x80
	ldrb r2, [r1, #0xc]
	orrs r0, r2
	strb r0, [r1, #0xc]
	bl sub_80241A4
_080156AC:
	pop {r0}
	bx r0
	.align 2, 0
_080156B0: .4byte gUnknown_030012D8

	thumb_func_start sub_80156B4
sub_80156B4: @ 0x080156B4
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _080156E6
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x11
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #4
	bl sub_803AD84
_080156E6:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_80156EC
sub_80156EC: @ 0x080156EC
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08015744
	ldr r0, _08015730 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231BC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08015734
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x19
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #7
	bl sub_803AD84
	b _08015744
	.align 2, 0
_08015730: .4byte gUnknown_030012C0
_08015734:
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x18
	bl sub_803AD80
_08015744:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start nullsub_17
nullsub_17: @ 0x0801574C
	bx lr
	.align 2, 0

	thumb_func_start sub_8015750
sub_8015750: @ 0x08015750
	push {r4, lr}
	adds r4, r0, #0
	adds r0, #0x29
	ldrb r0, [r0]
	cmp r0, #0
	bne _08015762
	adds r0, r4, #0
	bl sub_8015460
_08015762:
	adds r0, r4, #0
	bl sub_8012FBC
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start nullsub_18
nullsub_18: @ 0x08015770
	bx lr
	.align 2, 0

	thumb_func_start sub_8015774
sub_8015774: @ 0x08015774
	push {lr}
	bl sub_8012D24
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8015780
sub_8015780: @ 0x08015780
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	adds r4, r2, #0
	adds r6, r3, #0
	ldr r7, [sp, #0x14]
	ldr r2, [r5, #0xc]
	movs r3, #0x20
	ldrsh r0, [r2, r3]
	adds r0, r5, r0
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	adds r2, r4, #0
	bl sub_803AD84
	ldr r0, _080157C0 @ =0x7FFFFFFF
	cmp r6, r0
	beq _080157B4
	str r6, [r5, #0x18]
_080157B4:
	cmp r7, r0
	beq _080157BA
	str r7, [r5, #0x1c]
_080157BA:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080157C0: .4byte 0x7FFFFFFF

	thumb_func_start sub_80157C4
sub_80157C4: @ 0x080157C4
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	adds r7, r1, #0
	adds r5, r2, #0
	ldr r0, _080157EC @ =gUnknown_030012D8
	ldr r1, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	adds r0, r1, r2
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801582A
	cmp r5, #0x12
	beq _080157F6
	cmp r5, #0x12
	bgt _080157F0
	cmp r5, #0xd
	beq _08015800
	b _08015820
	.align 2, 0
_080157EC: .4byte gUnknown_030012D8
_080157F0:
	cmp r5, #0x18
	beq _08015800
	b _08015820
_080157F6:
	ldr r0, [r1, #0x60]
	cmp r0, #0
	beq _0801582A
	movs r5, #0x25
	b _08015802
_08015800:
	movs r5, #0x26
_08015802:
	ldr r4, _0801581C @ =gUnknown_030012BC
	ldr r0, [r4]
	movs r1, #0x36
	bl sub_80019A8
	ldr r0, [r4]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x36
	bl PlaySfx
	b _0801582A
	.align 2, 0
_0801581C: .4byte gUnknown_030012BC
_08015820:
	ldr r0, _0801583C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x36
	bl sub_80019A8
_0801582A:
	adds r0, r6, #0
	adds r1, r7, #0
	adds r2, r5, #0
	bl sub_800B86C
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0801583C: .4byte gUnknown_030012BC

	thumb_func_start sub_8015840
sub_8015840: @ 0x08015840
	push {r4, r5, lr}
	sub sp, #4
	adds r5, r0, #0
	movs r4, #0
	str r4, [sp]
	movs r1, #0
	movs r2, #0x12
	movs r3, #0
	bl sub_8015780
	adds r0, r5, #0
	adds r0, #0x31
	strb r4, [r0]
	subs r0, #2
	movs r1, #1
	strb r1, [r0]
	subs r0, #8
	strb r4, [r0]
	adds r0, #0xb
	strb r4, [r0]
	subs r0, #2
	strb r1, [r0]
	subs r0, #8
	strb r4, [r0]
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_8015878
sub_8015878: @ 0x08015878
	push {lr}
	ldr r2, _08015888 @ =gStaticData_087E4224
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_08015888: .4byte gStaticData_087E4224

	thumb_func_start sub_801588C
sub_801588C: @ 0x0801588C
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800B8C8
	ldr r0, _080158A8 @ =gStaticData_087E4224
	str r0, [r4, #0xc]
	adds r0, r4, #0
	bl sub_8011B90
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_080158A8: .4byte gStaticData_087E4224

	thumb_func_start sub_80158AC
sub_80158AC: @ 0x080158AC
	movs r1, #0
	str r1, [r0, #0x14]
	bx lr
	.align 2, 0

	thumb_func_start sub_80158B4
sub_80158B4: @ 0x080158B4
	adds r0, #0x32
	movs r1, #1
	strb r1, [r0]
	bx lr

	thumb_func_start sub_80158BC
sub_80158BC: @ 0x080158BC
	adds r0, #0x31
	movs r1, #1
	strb r1, [r0]
	bx lr

	thumb_func_start sub_80158C4
sub_80158C4: @ 0x080158C4
	adds r0, #0x30
	movs r1, #1
	strb r1, [r0]
	bx lr

	thumb_func_start sub_80158CC
sub_80158CC: @ 0x080158CC
	adds r0, #0x2f
	movs r1, #1
	strb r1, [r0]
	bx lr

	thumb_func_start sub_80158D4
sub_80158D4: @ 0x080158D4
	adds r0, #0x30
	movs r1, #0
	strb r1, [r0]
	bx lr

	thumb_func_start sub_80158DC
sub_80158DC: @ 0x080158DC
	adds r0, #0x2f
	movs r1, #0
	strb r1, [r0]
	bx lr

	thumb_func_start sub_80158E4
sub_80158E4: @ 0x080158E4
	adds r0, #0x30
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_80158EC
sub_80158EC: @ 0x080158EC
	adds r0, #0x2f
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_80158F4
sub_80158F4: @ 0x080158F4
	adds r3, r0, #0
	adds r0, #0x32
	movs r2, #1
	strb r2, [r0]
	subs r0, #2
	strb r2, [r0]
	subs r0, #8
	strb r1, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8015908
sub_8015908: @ 0x08015908
	adds r2, r0, #0
	adds r3, r2, #0
	adds r3, #0x31
	movs r0, #1
	strb r0, [r3]
	subs r3, #2
	strb r0, [r3]
	adds r0, r2, #0
	adds r0, #0x27
	strb r1, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8015920
sub_8015920: @ 0x08015920
	adds r3, r0, #0
	adds r2, r3, #0
	adds r2, #0x32
	movs r0, #0
	strb r0, [r2]
	subs r2, #2
	movs r0, #1
	strb r0, [r2]
	adds r0, r3, #0
	adds r0, #0x28
	strb r1, [r0]
	bx lr

	thumb_func_start sub_8015938
sub_8015938: @ 0x08015938
	adds r2, r0, #0
	adds r3, r2, #0
	adds r3, #0x31
	movs r0, #0
	strb r0, [r3]
	subs r3, #2
	movs r0, #1
	strb r0, [r3]
	adds r0, r2, #0
	adds r0, #0x27
	strb r1, [r0]
	bx lr

	thumb_func_start sub_8015950
sub_8015950: @ 0x08015950
	adds r0, #0x2d
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8015958
sub_8015958: @ 0x08015958
	adds r3, r0, #0
	adds r0, #0x26
	movs r1, #0
	strb r1, [r0]
	str r1, [r3, #8]
	subs r0, #2
	strb r1, [r0]
	adds r0, #1
	strb r1, [r0]
	adds r0, #7
	movs r2, #1
	strb r2, [r0]
	adds r0, #1
	strb r2, [r0]
	str r1, [r3, #0x14]
	str r1, [r3, #0x10]
	subs r0, #0xb
	strb r1, [r0]
	adds r0, #1
	strb r1, [r0]
	adds r0, #4
	strb r1, [r0]
	subs r0, #7
	strb r1, [r0]
	adds r2, r3, #0
	adds r2, #0x21
	movs r0, #6
	strb r0, [r2]
	str r1, [r3, #0x18]
	str r1, [r3, #0x1c]
	ldr r0, _080159A0 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x92
	strb r1, [r0]
	bx lr
	.align 2, 0
_080159A0: .4byte gUnknown_030012D8

	thumb_func_start sub_80159A4
sub_80159A4: @ 0x080159A4
	push {r4, r5, lr}
	sub sp, #4
	adds r5, r0, #0
	movs r4, #0
	str r4, [sp]
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_8017264
	adds r0, r5, #0
	adds r0, #0x27
	strb r4, [r0]
	subs r0, #7
	strb r4, [r0]
	adds r1, r5, #0
	adds r1, #0x21
	movs r0, #6
	strb r0, [r1]
	adds r0, r5, #0
	adds r0, #0x22
	strb r4, [r0]
	ldr r0, _080159F4 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x92
	strb r4, [r0]
	adds r0, r5, #0
	adds r0, #0x2c
	movs r1, #1
	strb r1, [r0]
	subs r0, #8
	strb r4, [r0]
	adds r0, #9
	strb r1, [r0]
	subs r0, #8
	strb r4, [r0]
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080159F4: .4byte gUnknown_030012D8

	thumb_func_start sub_80159F8
sub_80159F8: @ 0x080159F8
	push {r4, r5, lr}
	sub sp, #0x24
	adds r5, r0, #0
	ldr r0, _08015A38 @ =gUnknown_0300082C
	ldr r0, [r0]
	adds r0, #0x10
	str r0, [r5, #0x28]
	ldr r0, [r5, #8]
	cmp r0, #4
	bne _08015A52
	add r1, sp, #4
	ldr r0, _08015A3C @ =gStaticData_0816C090
	ldm r0!, {r2, r3, r4}
	stm r1!, {r2, r3, r4}
	ldm r0!, {r2, r3, r4}
	stm r1!, {r2, r3, r4}
	ldm r0!, {r2, r3}
	stm r1!, {r2, r3}
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	cmp r0, #6
	bne _08015A40
	ldr r1, [r5, #0x10]
	ldr r0, [r1, #0x30]
	lsls r0, r0, #2
	add r0, sp
	adds r0, #4
	ldr r0, [r0]
	str r0, [r1, #0x60]
	b _08015C64
	.align 2, 0
_08015A38: .4byte gUnknown_0300082C
_08015A3C: .4byte gStaticData_0816C090
_08015A40:
	ldr r1, [r5, #0x10]
	ldr r0, [r1, #0x30]
	lsls r0, r0, #2
	add r0, sp
	adds r0, #4
	ldr r0, [r0]
	rsbs r0, r0, #0
	str r0, [r1, #0x60]
	b _08015C64
_08015A52:
	ldr r4, [r5, #0x10]
	adds r1, r4, #0
	adds r1, #0x2d
	ldrb r0, [r1]
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r3, _08015A98 @ =0x7FFFFFFF
	str r3, [sp]
	adds r0, r5, #0
	movs r1, #2
	movs r2, #2
	bl sub_8017264
	adds r0, r5, #0
	adds r0, #0x21
	ldrb r1, [r0]
	adds r3, r0, #0
	cmp r1, #0xc
	bls _08015A8C
	b _08015C64
_08015A8C:
	lsls r0, r1, #2
	ldr r1, _08015A9C @ =_08015AA0
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08015A98: .4byte 0x7FFFFFFF
_08015A9C: .4byte _08015AA0
_08015AA0: @ jump table
	.4byte _08015C30 @ case 0
	.4byte _08015AD4 @ case 1
	.4byte _08015AF8 @ case 2
	.4byte _08015C10 @ case 3
	.4byte _08015B1C @ case 4
	.4byte _08015B44 @ case 5
	.4byte _08015BF2 @ case 6
	.4byte _08015B64 @ case 7
	.4byte _08015B88 @ case 8
	.4byte _08015C3C @ case 9
	.4byte _08015BA8 @ case 10
	.4byte _08015BD0 @ case 11
	.4byte _08015C5C @ case 12
_08015AD4:
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	movs r2, #0xb0
	cmp r0, #0
	bge _08015AE8
	movs r2, #0xb0
	rsbs r2, r2, #0
_08015AE8:
	str r2, [r1, #0x60]
	ldr r0, _08015AF4 @ =0xFFFFFD40
	str r0, [r1, #0x64]
	movs r0, #0
	strb r0, [r3]
	b _08015C64
	.align 2, 0
_08015AF4: .4byte 0xFFFFFD40
_08015AF8:
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	movs r2, #0xb0
	lsls r2, r2, #1
	cmp r0, #0
	bge _08015B0C
	ldr r2, _08015B14 @ =0xFFFFFEA0
_08015B0C:
	str r2, [r1, #0x60]
	ldr r0, _08015B18 @ =0xFFFFFD40
	b _08015B34
	.align 2, 0
_08015B14: .4byte 0xFFFFFEA0
_08015B18: .4byte 0xFFFFFD40
_08015B1C:
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	movs r2, #0xb0
	lsls r2, r2, #2
	cmp r0, #0
	bge _08015B30
	ldr r2, _08015B3C @ =0xFFFFFD40
_08015B30:
	str r2, [r1, #0x60]
	ldr r0, _08015B40 @ =0xFFFFFEA0
_08015B34:
	str r0, [r1, #0x64]
	movs r0, #3
	strb r0, [r3]
	b _08015C64
	.align 2, 0
_08015B3C: .4byte 0xFFFFFD40
_08015B40: .4byte 0xFFFFFEA0
_08015B44:
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	movs r2, #0xb0
	lsls r2, r2, #2
	cmp r0, #0
	bge _08015B58
	ldr r2, _08015B60 @ =0xFFFFFD40
_08015B58:
	str r2, [r1, #0x60]
	movs r0, #0xb0
	rsbs r0, r0, #0
	b _08015B7C
	.align 2, 0
_08015B60: .4byte 0xFFFFFD40
_08015B64:
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	movs r2, #0xb0
	lsls r2, r2, #2
	cmp r0, #0
	bge _08015B78
	ldr r2, _08015B84 @ =0xFFFFFD40
_08015B78:
	str r2, [r1, #0x60]
	movs r0, #0xb0
_08015B7C:
	str r0, [r1, #0x64]
	movs r0, #6
	strb r0, [r3]
	b _08015C64
	.align 2, 0
_08015B84: .4byte 0xFFFFFD40
_08015B88:
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	movs r2, #0xb0
	lsls r2, r2, #2
	cmp r0, #0
	bge _08015B9C
	ldr r2, _08015BA4 @ =0xFFFFFD40
_08015B9C:
	str r2, [r1, #0x60]
	movs r0, #0xb0
	lsls r0, r0, #1
	b _08015BC2
	.align 2, 0
_08015BA4: .4byte 0xFFFFFD40
_08015BA8:
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	movs r2, #0xb0
	lsls r2, r2, #1
	cmp r0, #0
	bge _08015BBC
	ldr r2, _08015BCC @ =0xFFFFFEA0
_08015BBC:
	str r2, [r1, #0x60]
	movs r0, #0xb0
	lsls r0, r0, #2
_08015BC2:
	str r0, [r1, #0x64]
	movs r0, #9
	strb r0, [r3]
	b _08015C64
	.align 2, 0
_08015BCC: .4byte 0xFFFFFEA0
_08015BD0:
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	movs r2, #0xb0
	cmp r0, #0
	bge _08015BE4
	movs r2, #0xb0
	rsbs r2, r2, #0
_08015BE4:
	str r2, [r1, #0x60]
	movs r0, #0xb0
	lsls r0, r0, #2
	str r0, [r1, #0x64]
	movs r0, #0xc
	strb r0, [r3]
	b _08015C64
_08015BF2:
	ldr r2, [r5, #0x10]
	adds r0, r2, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	movs r1, #0xb0
	lsls r1, r1, #2
	cmp r0, #0
	bge _08015C06
	ldr r1, _08015C0C @ =0xFFFFFD40
_08015C06:
	str r1, [r2, #0x60]
	b _08015C64
	.align 2, 0
_08015C0C: .4byte 0xFFFFFD40
_08015C10:
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	movs r2, #0x84
	lsls r2, r2, #2
	cmp r0, #0
	bge _08015C24
	ldr r2, _08015C2C @ =0xFFFFFDF0
_08015C24:
	str r2, [r1, #0x60]
	ldr r0, _08015C2C @ =0xFFFFFDF0
	b _08015C62
	.align 2, 0
_08015C2C: .4byte 0xFFFFFDF0
_08015C30:
	ldr r1, [r5, #0x10]
	ldr r0, _08015C38 @ =0xFFFFFD40
	b _08015C62
	.align 2, 0
_08015C38: .4byte 0xFFFFFD40
_08015C3C:
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	movs r2, #0x84
	lsls r2, r2, #2
	cmp r0, #0
	bge _08015C50
	ldr r2, _08015C58 @ =0xFFFFFDF0
_08015C50:
	str r2, [r1, #0x60]
	movs r0, #0x84
	lsls r0, r0, #2
	b _08015C62
	.align 2, 0
_08015C58: .4byte 0xFFFFFDF0
_08015C5C:
	ldr r1, [r5, #0x10]
	movs r0, #0xb0
	lsls r0, r0, #2
_08015C62:
	str r0, [r1, #0x64]
_08015C64:
	add sp, #0x24
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_8015C6C
sub_8015C6C: @ 0x08015C6C
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r4, r0, #0
	adds r0, #0x23
	ldrb r0, [r0]
	cmp r0, #0
	beq _08015C7C
	b _08015DF0
_08015C7C:
	movs r5, #0xf0
	lsls r5, r5, #2
	movs r6, #0
	ldr r0, _08015CD0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #9
	bl PlaySfx
	ldr r1, [r4, #0x10]
	adds r1, #0x68
	movs r0, #3
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _08015CB0
	ldr r0, _08015CD4 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #2
	bhi _08015CB0
	movs r6, #1
_08015CB0:
	ldr r0, [r4, #8]
	cmp r0, #4
	bne _08015CD8
	cmp r6, #0
	bne _08015D52
	adds r1, r5, #0
	adds r0, r4, #0
	adds r0, #0x22
	ldrb r0, [r0]
	cmp r0, #7
	bne _08015CC8
	rsbs r1, r5, #0
_08015CC8:
	ldr r0, [r4, #0x10]
	str r1, [r0, #0x60]
	b _08015DF0
	.align 2, 0
_08015CD0: .4byte gUnknown_030012BC
_08015CD4: .4byte gUnknown_03001304
_08015CD8:
	movs r0, #0x18
	str r0, [sp]
	adds r0, r4, #0
	movs r1, #3
	movs r2, #3
	movs r3, #0
	bl sub_8017264
	adds r0, r4, #0
	adds r0, #0x21
	ldrb r0, [r0]
	cmp r0, #0xc
	bls _08015CF4
	b _08015DF0
_08015CF4:
	lsls r0, r0, #2
	ldr r1, _08015D00 @ =_08015D04
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08015D00: .4byte _08015D04
_08015D04: @ jump table
	.4byte _08015DA2 @ case 0
	.4byte _08015DF0 @ case 1
	.4byte _08015DF0 @ case 2
	.4byte _08015D5A @ case 3
	.4byte _08015DF0 @ case 4
	.4byte _08015DF0 @ case 5
	.4byte _08015D38 @ case 6
	.4byte _08015DF0 @ case 7
	.4byte _08015DF0 @ case 8
	.4byte _08015DAA @ case 9
	.4byte _08015DF0 @ case 10
	.4byte _08015DF0 @ case 11
	.4byte _08015DEC @ case 12
_08015D38:
	cmp r6, #0
	bne _08015D52
	ldr r2, [r4, #0x10]
	adds r0, r2, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	adds r1, r5, #0
	cmp r0, #0
	bge _08015D4E
	rsbs r1, r5, #0
_08015D4E:
	str r1, [r2, #0x60]
	b _08015DF0
_08015D52:
	ldr r1, [r4, #0x10]
	movs r0, #0
	str r0, [r1, #0x60]
	b _08015DF0
_08015D5A:
	cmp r6, #0
	bne _08015D8C
	ldr r0, [r4, #0x10]
	adds r1, r0, #0
	adds r1, #0x28
	ldrb r1, [r1]
	lsls r1, r1, #0x1b
	adds r3, r0, #0
	cmp r1, #0
	bge _08015D80
	rsbs r0, r5, #0
	lsls r1, r0, #1
	adds r1, r1, r0
	adds r2, r0, #0
	cmp r1, #0
	bge _08015D7C
	adds r1, #3
_08015D7C:
	asrs r0, r1, #2
	b _08015D88
_08015D80:
	lsls r0, r5, #1
	adds r0, r0, r5
	asrs r0, r0, #2
	rsbs r2, r5, #0
_08015D88:
	str r0, [r3, #0x60]
	b _08015D96
_08015D8C:
	ldr r1, [r4, #0x10]
	movs r0, #0
	str r0, [r1, #0x60]
	adds r3, r1, #0
	rsbs r2, r5, #0
_08015D96:
	lsls r0, r2, #1
	adds r0, r0, r2
	cmp r0, #0
	bge _08015DE6
	adds r0, #3
	b _08015DE6
_08015DA2:
	rsbs r1, r5, #0
	ldr r0, [r4, #0x10]
	str r1, [r0, #0x64]
	b _08015DF0
_08015DAA:
	cmp r6, #0
	bne _08015DDA
	ldr r0, [r4, #0x10]
	adds r1, r0, #0
	adds r1, #0x28
	ldrb r1, [r1]
	lsls r1, r1, #0x1b
	adds r3, r0, #0
	cmp r1, #0
	bge _08015DD0
	rsbs r1, r5, #0
	lsls r0, r1, #1
	adds r0, r0, r1
	cmp r0, #0
	bge _08015DCA
	adds r0, #3
_08015DCA:
	asrs r1, r0, #2
	lsls r0, r5, #1
	b _08015DD6
_08015DD0:
	lsls r0, r5, #1
	adds r1, r0, r5
	asrs r1, r1, #2
_08015DD6:
	str r1, [r3, #0x60]
	b _08015DE4
_08015DDA:
	ldr r1, [r4, #0x10]
	movs r0, #0
	str r0, [r1, #0x60]
	adds r3, r1, #0
	lsls r0, r5, #1
_08015DE4:
	adds r0, r0, r5
_08015DE6:
	asrs r0, r0, #2
	str r0, [r3, #0x64]
	b _08015DF0
_08015DEC:
	ldr r0, [r4, #0x10]
	str r5, [r0, #0x64]
_08015DF0:
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0

	thumb_func_start sub_8015DF8
sub_8015DF8: @ 0x08015DF8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #8
	adds r6, r0, #0
	ldr r0, [r6, #8]
	cmp r0, #2
	bne _08015E14
	movs r0, #0x96
	lsls r0, r0, #1
	mov r8, r0
	movs r5, #0x14
	movs r4, #0x1e
	b _08015E2E
_08015E14:
	cmp r0, #3
	bne _08015E24
	movs r1, #0x96
	lsls r1, r1, #1
	mov r8, r1
	movs r5, #0x14
	movs r4, #0x20
	b _08015E2E
_08015E24:
	movs r2, #0x96
	lsls r2, r2, #1
	mov r8, r2
	movs r5, #0xf
	movs r4, #5
_08015E2E:
	ldr r0, _08015E98 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r7, r0, #0x18
	ldr r0, _08015E9C @ =gUnknown_0300082C
	ldr r0, [r0]
	movs r1, #0x7f
	ands r0, r1
	cmp r0, #0
	bne _08015E86
	movs r0, #2
	bl sub_8000E1C
	lsls r0, r0, #0x10
	cmp r0, #0
	bne _08015E86
	ldr r0, [r6, #0x10]
	ldr r3, [r0]
	asrs r3, r3, #8
	ldr r2, [r0, #4]
	asrs r2, r2, #8
	subs r2, #0x14
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r1, r0, #0x1b
	lsrs r1, r1, #0x1f
	ldr r0, _08015EA0 @ =gUnknown_030012E4
	ldr r0, [r0]
	str r2, [sp]
	str r1, [sp, #4]
	movs r1, #0x28
	movs r2, #4
	bl sub_8025BAC
	adds r1, r0, #0
	cmp r1, #0
	beq _08015E86
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xc]
	ands r0, r2
	strb r0, [r1, #0xc]
_08015E86:
	cmp r7, #8
	bls _08015E8C
	b _08015FD0
_08015E8C:
	lsls r0, r7, #2
	ldr r1, _08015EA4 @ =_08015EA8
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08015E98: .4byte gUnknown_03001304
_08015E9C: .4byte gUnknown_0300082C
_08015EA0: .4byte gUnknown_030012E4
_08015EA4: .4byte _08015EA8
_08015EA8: @ jump table
	.4byte _08015F6E @ case 0
	.4byte _08015FA4 @ case 1
	.4byte _08015FBC @ case 2
	.4byte _08015F84 @ case 3
	.4byte _08015F8E @ case 4
	.4byte _08015ECC @ case 5
	.4byte _08015EF2 @ case 6
	.4byte _08015F44 @ case 7
	.4byte _08015F1C @ case 8
_08015ECC:
	lsls r0, r5, #1
	adds r5, r0, r5
	adds r1, r5, #0
	asrs r6, r1, #2
	mov r0, r8
	rsbs r1, r0, #0
	lsls r0, r1, #1
	adds r4, r0, r1
	adds r2, r4, #0
	cmp r4, #0
	bge _08015EE4
	adds r2, r4, #3
_08015EE4:
	asrs r5, r2, #2
	movs r0, #0
	adds r1, r6, #0
	adds r2, r5, #0
	bl sub_80172D0
	b _08015F38
_08015EF2:
	lsls r0, r5, #1
	adds r4, r0, r5
	adds r1, r4, #0
	asrs r5, r1, #2
	mov r1, r8
	lsls r0, r1, #1
	add r0, r8
	asrs r2, r0, #2
	movs r0, #0
	adds r1, r5, #0
	bl sub_80172D0
	mov r2, r8
	rsbs r1, r2, #0
	lsls r0, r1, #1
	adds r0, r0, r1
	cmp r0, #0
	bge _08015F18
	adds r0, #3
_08015F18:
	asrs r2, r0, #2
	b _08015FB2
_08015F1C:
	lsls r0, r5, #1
	adds r5, r0, r5
	adds r1, r5, #0
	asrs r6, r1, #2
	mov r1, r8
	lsls r0, r1, #1
	adds r4, r0, r1
	adds r2, r4, #0
	asrs r5, r2, #2
	movs r0, #0
	adds r1, r6, #0
	adds r2, r5, #0
	bl sub_80172D0
_08015F38:
	movs r0, #0
	adds r1, r6, #0
	adds r2, r5, #0
	bl sub_8015FDC
	b _08015FD0
_08015F44:
	lsls r0, r5, #1
	adds r4, r0, r5
	adds r1, r4, #0
	asrs r5, r1, #2
	mov r2, r8
	rsbs r1, r2, #0
	lsls r0, r1, #1
	adds r0, r0, r1
	cmp r0, #0
	bge _08015F5A
	adds r0, #3
_08015F5A:
	asrs r2, r0, #2
	movs r0, #0
	adds r1, r5, #0
	bl sub_80172D0
	mov r1, r8
	lsls r0, r1, #1
	add r0, r8
	asrs r2, r0, #2
	b _08015FB2
_08015F6E:
	movs r0, #0
	adds r1, r4, #0
	movs r2, #0
	bl sub_8015FDC
	movs r0, #0
	adds r1, r4, #0
	movs r2, #0
	bl sub_80172D0
	b _08015FD0
_08015F84:
	mov r0, r8
	rsbs r2, r0, #0
	movs r0, #0
	adds r1, r5, #0
	b _08015F94
_08015F8E:
	movs r0, #0
	adds r1, r5, #0
	mov r2, r8
_08015F94:
	bl sub_80172D0
	movs r0, #0
	adds r1, r4, #0
	movs r2, #0
	bl sub_8015FDC
	b _08015FD0
_08015FA4:
	movs r0, #0
	adds r1, r4, #0
	movs r2, #0
	bl sub_80172D0
	mov r1, r8
	rsbs r2, r1, #0
_08015FB2:
	movs r0, #0
	adds r1, r5, #0
	bl sub_8015FDC
	b _08015FD0
_08015FBC:
	movs r0, #0
	adds r1, r4, #0
	movs r2, #0
	bl sub_80172D0
	movs r0, #0
	adds r1, r5, #0
	mov r2, r8
	bl sub_8015FDC
_08015FD0:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8015FDC
sub_8015FDC: @ 0x08015FDC
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	mov ip, r1
	adds r5, r2, #0
	ldr r0, _08016020 @ =gUnknown_030012D8
	ldr r3, [r0]
	ldr r4, [r3, #0x64]
	adds r1, r4, #0
	muls r1, r4, r1
	cmp r1, #0
	bge _08015FF6
	ldr r0, _08016024 @ =0x00003FFF
	adds r1, r1, r0
_08015FF6:
	asrs r1, r1, #0xe
	adds r1, #4
	lsls r0, r1, #1
	adds r0, r0, r1
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r7, r0, #1
	asrs r0, r4, #0x1f
	adds r1, r4, #0
	eors r1, r0
	subs r1, r1, r0
	asrs r2, r5, #0x1f
	adds r0, r5, #0
	eors r0, r2
	subs r0, r0, r2
	cmp r1, r0
	ble _08016028
	str r6, [r3, #0x54]
	str r7, [r3, #0x58]
	b _0801603E
	.align 2, 0
_08016020: .4byte gUnknown_030012D8
_08016024: .4byte 0x00003FFF
_08016028:
	adds r0, r4, #0
	muls r0, r5, r0
	cmp r0, #0
	bge _08016038
	mov r1, ip
	adds r0, r7, r1
	str r6, [r3, #0x54]
	b _0801603C
_08016038:
	str r6, [r3, #0x54]
	mov r0, ip
_0801603C:
	str r0, [r3, #0x58]
_0801603E:
	str r5, [r3, #0x5c]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	non_word_aligned_thumb_func_start sub_8016046
sub_8016046: @ 0x08016046
	movs r0, r0

	thumb_func_start sub_8016048
sub_8016048: @ 0x08016048
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _080160B4 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r2, r0, #0x18
	ldr r0, [r4, #8]
	cmp r0, #0
	blt _0801611A
	cmp r0, #3
	ble _0801606C
	cmp r0, #6
	bgt _0801611A
	cmp r0, #5
	blt _0801611A
_0801606C:
	ldr r1, [r4, #0x10]
	adds r1, #0x28
	movs r0, #0x21
	rsbs r0, r0, #0
	ldrb r3, [r1]
	ands r0, r3
	strb r0, [r1]
	ldr r0, [r4, #0x10]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _080160CC
	cmp r2, #4
	beq _08016092
	cmp r2, #6
	beq _08016092
	cmp r2, #8
	bne _080160CC
_08016092:
	ldr r0, [r4, #0x10]
	adds r0, #0x28
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r2, [r0]
	ands r1, r2
	strb r1, [r0]
	ldr r0, [r4, #8]
	cmp r0, #3
	beq _080160BC
	ldr r3, _080160B8 @ =0x7FFFFFFF
	str r3, [sp]
	adds r0, r4, #0
	movs r1, #4
	movs r2, #4
	b _080160F4
	.align 2, 0
_080160B4: .4byte gUnknown_03001304
_080160B8: .4byte 0x7FFFFFFF
_080160BC:
	ldr r3, _080160C8 @ =0x7FFFFFFF
	str r3, [sp]
	adds r0, r4, #0
	movs r1, #4
	movs r2, #5
	b _0801610E
	.align 2, 0
_080160C8: .4byte 0x7FFFFFFF
_080160CC:
	ldr r0, [r4, #0x10]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	blt _0801611A
	cmp r2, #3
	beq _080160E4
	cmp r2, #5
	beq _080160E4
	cmp r2, #7
	bne _0801611A
_080160E4:
	ldr r0, [r4, #8]
	cmp r0, #3
	beq _08016104
	ldr r3, _08016100 @ =0x7FFFFFFF
	str r3, [sp]
	adds r0, r4, #0
	movs r1, #4
	movs r2, #6
_080160F4:
	bl sub_8017264
	movs r0, #0
	str r0, [r4, #0x1c]
	b _08016112
	.align 2, 0
_08016100: .4byte 0x7FFFFFFF
_08016104:
	ldr r3, _08016124 @ =0x7FFFFFFF
	str r3, [sp]
	adds r0, r4, #0
	movs r1, #4
	movs r2, #7
_0801610E:
	bl sub_8017264
_08016112:
	adds r1, r4, #0
	adds r1, #0x26
	movs r0, #0
	strb r0, [r1]
_0801611A:
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08016124: .4byte 0x7FFFFFFF

	thumb_func_start sub_8016128
sub_8016128: @ 0x08016128
	push {r4, lr}
	adds r4, r0, #0
	subs r0, r2, #1
	cmp r0, #0xc
	bhi _080161E4
	lsls r0, r0, #2
	ldr r1, _0801613C @ =_08016140
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801613C: .4byte _08016140
_08016140: @ jump table
	.4byte _080161DC @ case 0
	.4byte _080161E4 @ case 1
	.4byte _080161C8 @ case 2
	.4byte _080161D2 @ case 3
	.4byte _080161BE @ case 4
	.4byte _080161DC @ case 5
	.4byte _080161E4 @ case 6
	.4byte _080161E4 @ case 7
	.4byte _080161E4 @ case 8
	.4byte _080161DC @ case 9
	.4byte _080161E4 @ case 10
	.4byte _08016174 @ case 11
	.4byte _080161E4 @ case 12
_08016174:
	movs r2, #3
	ands r2, r3
	cmp r2, #2
	bne _0801619A
	ldr r0, [r4, #0x10]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _080161B6
	movs r2, #0
	adds r1, r4, #0
	adds r1, #0x2c
	movs r0, #1
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x24
	strb r2, [r0]
	b _080161B6
_0801619A:
	cmp r2, #1
	bne _080161E4
	ldr r0, [r4, #0x10]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	lsrs r1, r0, #0x1f
	cmp r1, #0
	bne _080161B6
	adds r0, r4, #0
	adds r0, #0x2c
	strb r2, [r0]
	subs r0, #8
	strb r1, [r0]
_080161B6:
	ldr r1, [r4, #0x10]
	movs r0, #0
	str r0, [r1, #0x60]
	b _080161E4
_080161BE:
	adds r0, r4, #0
	movs r1, #0x2d
	bl sub_80161EC
	b _080161E4
_080161C8:
	adds r0, r4, #0
	movs r1, #0x2b
	bl sub_80161EC
	b _080161E4
_080161D2:
	adds r0, r4, #0
	movs r1, #0x2c
	bl sub_80161EC
	b _080161E4
_080161DC:
	adds r0, r4, #0
	movs r1, #0x2e
	bl sub_80161EC
_080161E4:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80161EC
sub_80161EC: @ 0x080161EC
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, _0801627C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x1b
	bl PlaySfx
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #7
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	adds r2, r5, #0
	bl sub_803AD84
	ldr r1, [r4, #0x10]
	movs r0, #0x7f
	ldrb r2, [r1, #0xc]
	ands r0, r2
	strb r0, [r1, #0xc]
	ldr r1, [r4, #0x10]
	movs r0, #0x41
	rsbs r0, r0, #0
	ldrb r5, [r1, #0xc]
	ands r0, r5
	strb r0, [r1, #0xc]
	ldr r0, [r4, #0x10]
	movs r1, #0x82
	lsls r1, r1, #1
	adds r0, r0, r1
	movs r1, #1
	strb r1, [r0]
	ldr r0, _08016280 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023234
	ldr r0, _08016284 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r3, [r4, #0x10]
	adds r1, r3, #0
	adds r1, #0x29
	ldrb r1, [r1]
	lsls r1, r1, #0x1c
	lsrs r1, r1, #0x1c
	ldr r2, [r3, #0x20]
	adds r3, #0x2d
	ldr r4, [r2]
	ldrb r5, [r3]
	lsls r2, r5, #3
	subs r2, r2, r5
	lsls r2, r2, #2
	adds r2, r2, r4
	ldrb r2, [r2, #0x14]
	bl sub_8006D08
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0801627C: .4byte gUnknown_030012BC
_08016280: .4byte gUnknown_030012C0
_08016284: .4byte gUnknown_030012B8

	thumb_func_start sub_8016288
sub_8016288: @ 0x08016288
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	adds r5, r0, #0
	ldr r0, [r5, #8]
	cmp r0, #7
	bne _080162E2
	ldr r0, _080162BC @ =gStaticData_0816C250
	adds r1, r0, #0
	adds r1, #0x38
	movs r6, #2
	ldrsh r2, [r1, r6]
	adds r7, r0, #0
	cmp r2, #0
	ble _080162C0
	movs r3, #4
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r1, [r0]
	lsls r0, r2, #3
	adds r0, r0, r1
	subs r0, #8
	ldr r3, [r0]
	ldr r4, [r0, #4]
	adds r6, r4, #0
	b _080162C2
	.align 2, 0
_080162BC: .4byte gStaticData_0816C250
_080162C0:
	ldr r6, [r7, #0x3c]
_080162C2:
	ldr r0, [r5, #8]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _080162D8
	lsls r0, r3, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _080162DA
_080162D8:
	adds r0, r1, #0
_080162DA:
	adds r0, r5, r0
	bl sub_803AD90
	b _08016A88
_080162E2:
	adds r1, r5, #0
	adds r1, #0x23
	ldrb r0, [r1]
	cmp r0, #0
	beq _080162F0
	subs r0, #1
	strb r0, [r1]
_080162F0:
	ldr r0, _0801632C @ =gUnknown_03001304
	ldr r0, [r0]
	ldr r1, _08016330 @ =gUnknown_030007E0
	ldr r4, [r1]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r1, r0, #0x18
	movs r0, #0xc0
	ands r0, r4
	cmp r0, #0
	beq _0801630A
	b _08016428
_0801630A:
	ldr r0, [r5, #8]
	cmp r0, #2
	bne _08016312
	b _0801642E
_08016312:
	adds r0, r5, #0
	adds r0, #0x20
	ldrb r1, [r0]
	adds r3, r0, #0
	cmp r1, #0
	beq _08016334
	adds r0, #1
	ldrb r0, [r0]
	cmp r0, #6
	beq _08016334
	subs r0, r1, #1
	b _08016336
	.align 2, 0
_0801632C: .4byte gUnknown_03001304
_08016330: .4byte gUnknown_030007E0
_08016334:
	movs r0, #3
_08016336:
	strb r0, [r3]
	ldrb r0, [r3]
	cmp r0, #0
	beq _08016340
	b _080169FC
_08016340:
	adds r2, r5, #0
	adds r2, #0x21
	ldrb r0, [r2]
	adds r1, r0, #0
	cmp r1, #6
	bne _0801634E
	b _080169FC
_0801634E:
	cmp r1, #5
	bhi _08016356
	adds r0, #1
	strb r0, [r2]
_08016356:
	ldrb r0, [r2]
	cmp r0, #6
	bls _08016360
	subs r0, #1
	strb r0, [r2]
_08016360:
	ldrb r0, [r2]
	cmp r0, #6
	beq _0801636A
	movs r0, #3
	strb r0, [r3]
_0801636A:
	ldr r6, [r5, #0x10]
	adds r3, r6, #0
	adds r3, #0x2d
	ldrb r0, [r3]
	cmp r0, #0x20
	beq _08016380
	cmp r0, #0x1d
	beq _08016380
	cmp r0, #0x1f
	beq _08016380
	b _080168AC
_08016380:
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	subs r0, #7
	cmp r0, #0x22
	bls _0801638E
	b _08016870
_0801638E:
	lsls r0, r0, #2
	ldr r1, _08016398 @ =_0801639C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08016398: .4byte _0801639C
_0801639C: @ jump table
	.4byte _080169AC @ case 0
	.4byte _08016870 @ case 1
	.4byte _08016870 @ case 2
	.4byte _08016870 @ case 3
	.4byte _08016870 @ case 4
	.4byte _08016870 @ case 5
	.4byte _08016870 @ case 6
	.4byte _080169AC @ case 7
	.4byte _080169AC @ case 8
	.4byte _080169AC @ case 9
	.4byte _080169AC @ case 10
	.4byte _080169AC @ case 11
	.4byte _08016870 @ case 12
	.4byte _08016870 @ case 13
	.4byte _08016870 @ case 14
	.4byte _08016870 @ case 15
	.4byte _080169AC @ case 16
	.4byte _08016870 @ case 17
	.4byte _08016870 @ case 18
	.4byte _08016870 @ case 19
	.4byte _08016870 @ case 20
	.4byte _08016870 @ case 21
	.4byte _08016870 @ case 22
	.4byte _08016870 @ case 23
	.4byte _08016870 @ case 24
	.4byte _08016870 @ case 25
	.4byte _080169AC @ case 26
	.4byte _080169AC @ case 27
	.4byte _080169AC @ case 28
	.4byte _080169AC @ case 29
	.4byte _080169AC @ case 30
	.4byte _08016870 @ case 31
	.4byte _08016870 @ case 32
	.4byte _08016870 @ case 33
	.4byte _080169AC @ case 34
_08016428:
	ldr r0, [r5, #8]
	cmp r0, #2
	bne _08016434
_0801642E:
	cmp r1, #0
	bne _08016434
	b _080169FC
_08016434:
	adds r1, r5, #0
	adds r1, #0x20
	ldrb r0, [r1]
	cmp r0, #0
	beq _0801644A
	subs r0, #1
	strb r0, [r1]
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801644A
	b _080169FC
_0801644A:
	movs r0, #3
	strb r0, [r1]
	movs r0, #0x40
	ands r0, r4
	cmp r0, #0
	bne _08016458
	b _080166C8
_08016458:
	movs r0, #0x30
	ands r4, r0
	cmp r4, #0
	bne _08016462
	b _080165F8
_08016462:
	adds r2, r5, #0
	adds r2, #0x21
	ldrb r0, [r2]
	adds r1, r0, #0
	cmp r1, #3
	bls _08016530
	subs r0, #1
	strb r0, [r2]
	ldr r6, [r5, #0x10]
	adds r3, r6, #0
	adds r3, #0x2d
	ldrb r0, [r3]
	cmp r0, #0x20
	beq _08016488
	cmp r0, #0x1d
	beq _08016488
	cmp r0, #0x1f
	beq _08016488
	b _080168AC
_08016488:
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	subs r0, #7
	cmp r0, #0x22
	bls _08016496
	b _08016870
_08016496:
	lsls r0, r0, #2
	ldr r1, _080164A0 @ =_080164A4
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080164A0: .4byte _080164A4
_080164A4: @ jump table
	.4byte _080169AC @ case 0
	.4byte _08016870 @ case 1
	.4byte _08016870 @ case 2
	.4byte _08016870 @ case 3
	.4byte _08016870 @ case 4
	.4byte _08016870 @ case 5
	.4byte _08016870 @ case 6
	.4byte _080169AC @ case 7
	.4byte _080169AC @ case 8
	.4byte _080169AC @ case 9
	.4byte _080169AC @ case 10
	.4byte _080169AC @ case 11
	.4byte _08016870 @ case 12
	.4byte _08016870 @ case 13
	.4byte _08016870 @ case 14
	.4byte _08016870 @ case 15
	.4byte _080169AC @ case 16
	.4byte _08016870 @ case 17
	.4byte _08016870 @ case 18
	.4byte _08016870 @ case 19
	.4byte _08016870 @ case 20
	.4byte _08016870 @ case 21
	.4byte _08016870 @ case 22
	.4byte _08016870 @ case 23
	.4byte _08016870 @ case 24
	.4byte _08016870 @ case 25
	.4byte _080169AC @ case 26
	.4byte _080169AC @ case 27
	.4byte _080169AC @ case 28
	.4byte _080169AC @ case 29
	.4byte _080169AC @ case 30
	.4byte _08016870 @ case 31
	.4byte _08016870 @ case 32
	.4byte _08016870 @ case 33
	.4byte _080169AC @ case 34
_08016530:
	cmp r1, #2
	bls _08016536
	b _080169FC
_08016536:
	adds r0, #1
	strb r0, [r2]
	ldr r6, [r5, #0x10]
	adds r3, r6, #0
	adds r3, #0x2d
	ldrb r0, [r3]
	cmp r0, #0x20
	beq _08016550
	cmp r0, #0x1d
	beq _08016550
	cmp r0, #0x1f
	beq _08016550
	b _080168AC
_08016550:
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	subs r0, #7
	cmp r0, #0x22
	bls _0801655E
	b _08016870
_0801655E:
	lsls r0, r0, #2
	ldr r1, _08016568 @ =_0801656C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08016568: .4byte _0801656C
_0801656C: @ jump table
	.4byte _080169AC @ case 0
	.4byte _08016870 @ case 1
	.4byte _08016870 @ case 2
	.4byte _08016870 @ case 3
	.4byte _08016870 @ case 4
	.4byte _08016870 @ case 5
	.4byte _08016870 @ case 6
	.4byte _080169AC @ case 7
	.4byte _080169AC @ case 8
	.4byte _080169AC @ case 9
	.4byte _080169AC @ case 10
	.4byte _080169AC @ case 11
	.4byte _08016870 @ case 12
	.4byte _08016870 @ case 13
	.4byte _08016870 @ case 14
	.4byte _08016870 @ case 15
	.4byte _080169AC @ case 16
	.4byte _08016870 @ case 17
	.4byte _08016870 @ case 18
	.4byte _08016870 @ case 19
	.4byte _08016870 @ case 20
	.4byte _08016870 @ case 21
	.4byte _08016870 @ case 22
	.4byte _08016870 @ case 23
	.4byte _08016870 @ case 24
	.4byte _08016870 @ case 25
	.4byte _080169AC @ case 26
	.4byte _080169AC @ case 27
	.4byte _080169AC @ case 28
	.4byte _080169AC @ case 29
	.4byte _080169AC @ case 30
	.4byte _08016870 @ case 31
	.4byte _08016870 @ case 32
	.4byte _08016870 @ case 33
	.4byte _080169AC @ case 34
_080165F8:
	adds r2, r5, #0
	adds r2, #0x21
	ldrb r0, [r2]
	cmp r0, #0
	bne _08016604
	b _080169FC
_08016604:
	subs r0, #1
	strb r0, [r2]
	ldr r6, [r5, #0x10]
	adds r3, r6, #0
	adds r3, #0x2d
	ldrb r0, [r3]
	cmp r0, #0x20
	beq _0801661E
	cmp r0, #0x1d
	beq _0801661E
	cmp r0, #0x1f
	beq _0801661E
	b _080168AC
_0801661E:
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	subs r0, #7
	cmp r0, #0x22
	bls _0801662C
	b _08016870
_0801662C:
	lsls r0, r0, #2
	ldr r1, _08016638 @ =_0801663C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08016638: .4byte _0801663C
_0801663C: @ jump table
	.4byte _080169AC @ case 0
	.4byte _08016870 @ case 1
	.4byte _08016870 @ case 2
	.4byte _08016870 @ case 3
	.4byte _08016870 @ case 4
	.4byte _08016870 @ case 5
	.4byte _08016870 @ case 6
	.4byte _080169AC @ case 7
	.4byte _080169AC @ case 8
	.4byte _080169AC @ case 9
	.4byte _080169AC @ case 10
	.4byte _080169AC @ case 11
	.4byte _08016870 @ case 12
	.4byte _08016870 @ case 13
	.4byte _08016870 @ case 14
	.4byte _08016870 @ case 15
	.4byte _080169AC @ case 16
	.4byte _08016870 @ case 17
	.4byte _08016870 @ case 18
	.4byte _08016870 @ case 19
	.4byte _08016870 @ case 20
	.4byte _08016870 @ case 21
	.4byte _08016870 @ case 22
	.4byte _08016870 @ case 23
	.4byte _08016870 @ case 24
	.4byte _08016870 @ case 25
	.4byte _080169AC @ case 26
	.4byte _080169AC @ case 27
	.4byte _080169AC @ case 28
	.4byte _080169AC @ case 29
	.4byte _080169AC @ case 30
	.4byte _08016870 @ case 31
	.4byte _08016870 @ case 32
	.4byte _08016870 @ case 33
	.4byte _080169AC @ case 34
_080166C8:
	movs r0, #0x80
	ands r0, r4
	cmp r0, #0
	bne _080166D2
	b _080169FC
_080166D2:
	movs r0, #0x30
	ands r4, r0
	cmp r4, #0
	bne _080166DC
	b _08016888
_080166DC:
	adds r2, r5, #0
	adds r2, #0x21
	ldrb r0, [r2]
	adds r1, r0, #0
	cmp r1, #8
	bhi _080167AC
	adds r0, #1
	strb r0, [r2]
	ldr r6, [r5, #0x10]
	adds r3, r6, #0
	adds r3, #0x2d
	ldrb r0, [r3]
	cmp r0, #0x20
	beq _08016702
	cmp r0, #0x1d
	beq _08016702
	cmp r0, #0x1f
	beq _08016702
	b _080168AC
_08016702:
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	subs r0, #7
	cmp r0, #0x22
	bls _08016710
	b _08016870
_08016710:
	lsls r0, r0, #2
	ldr r1, _0801671C @ =_08016720
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801671C: .4byte _08016720
_08016720: @ jump table
	.4byte _080169AC @ case 0
	.4byte _08016870 @ case 1
	.4byte _08016870 @ case 2
	.4byte _08016870 @ case 3
	.4byte _08016870 @ case 4
	.4byte _08016870 @ case 5
	.4byte _08016870 @ case 6
	.4byte _080169AC @ case 7
	.4byte _080169AC @ case 8
	.4byte _080169AC @ case 9
	.4byte _080169AC @ case 10
	.4byte _080169AC @ case 11
	.4byte _08016870 @ case 12
	.4byte _08016870 @ case 13
	.4byte _08016870 @ case 14
	.4byte _08016870 @ case 15
	.4byte _080169AC @ case 16
	.4byte _08016870 @ case 17
	.4byte _08016870 @ case 18
	.4byte _08016870 @ case 19
	.4byte _08016870 @ case 20
	.4byte _08016870 @ case 21
	.4byte _08016870 @ case 22
	.4byte _08016870 @ case 23
	.4byte _08016870 @ case 24
	.4byte _08016870 @ case 25
	.4byte _080169AC @ case 26
	.4byte _080169AC @ case 27
	.4byte _080169AC @ case 28
	.4byte _080169AC @ case 29
	.4byte _080169AC @ case 30
	.4byte _08016870 @ case 31
	.4byte _08016870 @ case 32
	.4byte _08016870 @ case 33
	.4byte _080169AC @ case 34
_080167AC:
	cmp r1, #9
	bhi _080167B2
	b _080169FC
_080167B2:
	subs r0, #1
	strb r0, [r2]
	ldr r6, [r5, #0x10]
	adds r3, r6, #0
	adds r3, #0x2d
	ldrb r0, [r3]
	cmp r0, #0x20
	beq _080167CA
	cmp r0, #0x1d
	beq _080167CA
	cmp r0, #0x1f
	bne _080168AC
_080167CA:
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	subs r0, #7
	cmp r0, #0x22
	bhi _08016870
	lsls r0, r0, #2
	ldr r1, _080167E0 @ =_080167E4
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080167E0: .4byte _080167E4
_080167E4: @ jump table
	.4byte _080169AC @ case 0
	.4byte _08016870 @ case 1
	.4byte _08016870 @ case 2
	.4byte _08016870 @ case 3
	.4byte _08016870 @ case 4
	.4byte _08016870 @ case 5
	.4byte _08016870 @ case 6
	.4byte _080169AC @ case 7
	.4byte _080169AC @ case 8
	.4byte _080169AC @ case 9
	.4byte _080169AC @ case 10
	.4byte _080169AC @ case 11
	.4byte _08016870 @ case 12
	.4byte _08016870 @ case 13
	.4byte _08016870 @ case 14
	.4byte _08016870 @ case 15
	.4byte _080169AC @ case 16
	.4byte _08016870 @ case 17
	.4byte _08016870 @ case 18
	.4byte _08016870 @ case 19
	.4byte _08016870 @ case 20
	.4byte _08016870 @ case 21
	.4byte _08016870 @ case 22
	.4byte _08016870 @ case 23
	.4byte _08016870 @ case 24
	.4byte _08016870 @ case 25
	.4byte _080169AC @ case 26
	.4byte _080169AC @ case 27
	.4byte _080169AC @ case 28
	.4byte _080169AC @ case 29
	.4byte _080169AC @ case 30
	.4byte _08016870 @ case 31
	.4byte _08016870 @ case 32
	.4byte _08016870 @ case 33
	.4byte _080169AC @ case 34
_08016870:
	movs r0, #0
	str r0, [sp]
	adds r0, r5, #0
	movs r1, #1
	movs r2, #1
	ldr r3, _08016884 @ =0x7FFFFFFF
	bl sub_8017264
	b _080169FC
	.align 2, 0
_08016884: .4byte 0x7FFFFFFF
_08016888:
	adds r2, r5, #0
	adds r2, #0x21
	ldrb r0, [r2]
	cmp r0, #0xb
	bls _08016894
	b _080169FC
_08016894:
	adds r0, #1
	strb r0, [r2]
	ldr r6, [r5, #0x10]
	adds r3, r6, #0
	adds r3, #0x2d
	ldrb r0, [r3]
	cmp r0, #0x20
	beq _08016904
	cmp r0, #0x1d
	beq _08016904
	cmp r0, #0x1f
	beq _08016904
_080168AC:
	ldr r4, [r6, #0x30]
	ldr r7, [r6, #0x34]
	ldr r1, _08016900 @ =gStaticData_0816C070
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r1, [r0]
	ldrb r2, [r2]
	lsls r0, r2, #2
	adds r0, r0, r1
	ldrb r0, [r0]
	strb r0, [r3]
	adds r0, r6, #0
	bl sub_80087C0
	adds r0, r6, #0
	bl sub_80087B4
	adds r0, r6, #0
	movs r1, #0
	bl sub_800872C
	ldr r3, [r5, #0x10]
	ldr r0, [r3, #0x20]
	adds r2, r3, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r6, [r2]
	lsls r0, r6, #3
	subs r0, r0, r6
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _080168F8
	subs r4, r0, #1
_080168F8:
	str r4, [r3, #0x30]
	str r7, [r3, #0x34]
	b _080169FC
	.align 2, 0
_08016900: .4byte gStaticData_0816C070
_08016904:
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	subs r0, #7
	cmp r0, #0x22
	bhi _080169EC
	lsls r0, r0, #2
	ldr r1, _0801691C @ =_08016920
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801691C: .4byte _08016920
_08016920: @ jump table
	.4byte _080169AC @ case 0
	.4byte _080169EC @ case 1
	.4byte _080169EC @ case 2
	.4byte _080169EC @ case 3
	.4byte _080169EC @ case 4
	.4byte _080169EC @ case 5
	.4byte _080169EC @ case 6
	.4byte _080169AC @ case 7
	.4byte _080169AC @ case 8
	.4byte _080169AC @ case 9
	.4byte _080169AC @ case 10
	.4byte _080169AC @ case 11
	.4byte _080169EC @ case 12
	.4byte _080169EC @ case 13
	.4byte _080169EC @ case 14
	.4byte _080169EC @ case 15
	.4byte _080169AC @ case 16
	.4byte _080169EC @ case 17
	.4byte _080169EC @ case 18
	.4byte _080169EC @ case 19
	.4byte _080169EC @ case 20
	.4byte _080169EC @ case 21
	.4byte _080169EC @ case 22
	.4byte _080169EC @ case 23
	.4byte _080169EC @ case 24
	.4byte _080169EC @ case 25
	.4byte _080169AC @ case 26
	.4byte _080169AC @ case 27
	.4byte _080169AC @ case 28
	.4byte _080169AC @ case 29
	.4byte _080169AC @ case 30
	.4byte _080169EC @ case 31
	.4byte _080169EC @ case 32
	.4byte _080169EC @ case 33
	.4byte _080169AC @ case 34
_080169AC:
	ldr r0, [r5, #0x10]
	ldr r4, [r0, #0x30]
	ldr r6, [r0, #0x34]
	movs r0, #0
	str r0, [sp]
	adds r0, r5, #0
	movs r1, #1
	movs r2, #1
	ldr r3, _080169E8 @ =0x7FFFFFFF
	bl sub_8017264
	ldr r3, [r5, #0x10]
	ldr r0, [r3, #0x20]
	adds r2, r3, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r7, [r2]
	lsls r0, r7, #3
	adds r2, r7, #0
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _080169E0
	subs r4, r0, #1
_080169E0:
	str r4, [r3, #0x30]
	str r6, [r3, #0x34]
	b _080169FC
	.align 2, 0
_080169E8: .4byte 0x7FFFFFFF
_080169EC:
	movs r0, #0
	str r0, [sp]
	adds r0, r5, #0
	movs r1, #1
	movs r2, #1
	ldr r3, _08016A2C @ =0x7FFFFFFF
	bl sub_8017264
_080169FC:
	ldr r1, _08016A30 @ =gStaticData_0816C250
	ldr r0, [r5, #8]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r4, #2
	ldrsh r2, [r0, r4]
	adds r7, r1, #0
	cmp r2, #0
	ble _08016A34
	movs r6, #4
	ldrsh r0, [r0, r6]
	adds r0, r5, r0
	ldr r1, [r0]
	lsls r0, r2, #3
	adds r0, r0, r1
	adds r3, r0, #0
	subs r3, #8
	ldr r0, [r3]
	ldr r1, [r3, #4]
	str r0, [sp, #4]
	str r1, [sp, #8]
	ldr r3, [sp, #8]
	b _08016A3A
	.align 2, 0
_08016A2C: .4byte 0x7FFFFFFF
_08016A30: .4byte gStaticData_0816C250
_08016A34:
	adds r0, r7, #4
	adds r0, r3, r0
	ldr r3, [r0]
_08016A3A:
	ldr r0, [r5, #8]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r4, #0
	ldrsh r1, [r0, r4]
	cmp r2, #0
	ble _08016A52
	ldr r6, [sp, #4]
	lsls r0, r6, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _08016A54
_08016A52:
	adds r0, r1, #0
_08016A54:
	adds r0, r5, r0
	bl sub_803AD84
	ldr r2, [r5, #0x10]
	ldr r0, [r2, #0x74]
	movs r1, #3
	ands r0, r1
	cmp r0, #0
	beq _08016A6A
	movs r0, #0
	str r0, [r2, #0x60]
_08016A6A:
	ldr r2, [r5, #0x10]
	ldr r0, [r2, #0x74]
	movs r1, #0xc
	ands r0, r1
	cmp r0, #0
	beq _08016A7A
	movs r0, #0
	str r0, [r2, #0x64]
_08016A7A:
	ldr r0, [r5, #0x10]
	movs r1, #0
	adds r0, #0x68
	strb r1, [r0]
	adds r0, r5, #0
	bl sub_8015DF8
_08016A88:
	ldr r0, [r5, #8]
	cmp r0, #3
	beq _08016A9C
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	cmp r0, #5
	beq _08016A9C
	cmp r0, #7
	bne _08016AA2
_08016A9C:
	ldr r1, [r5, #0x10]
	movs r0, #0x13
	b _08016AA6
_08016AA2:
	ldr r1, [r5, #0x10]
	movs r0, #1
_08016AA6:
	strb r0, [r1, #0xa]
	add sp, #0xc
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8016AB0
sub_8016AB0: @ 0x08016AB0
	push {r4, lr}
	adds r4, r0, #0
	adds r1, r4, #0
	adds r1, #0x2c
	ldrb r0, [r1]
	cmp r0, #1
	bne _08016AE2
	movs r0, #0
	strb r0, [r1]
	ldr r0, [r4, #4]
	subs r1, #8
	ldr r2, [r0]
	ldrb r1, [r1]
	lsls r0, r1, #3
	adds r0, r0, r2
	ldr r1, [r0]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _08016B18 @ =gStaticData_0816B61C
	adds r2, r0, r1
	ldr r1, [r4, #0x10]
	adds r0, r4, #0
	bl sub_800B7B0
_08016AE2:
	adds r1, r4, #0
	adds r1, #0x2d
	ldrb r0, [r1]
	cmp r0, #1
	bne _08016B12
	movs r0, #0
	strb r0, [r1]
	ldr r0, [r4, #4]
	adds r2, r4, #0
	adds r2, #0x25
	ldr r1, [r0]
	ldrb r2, [r2]
	lsls r0, r2, #3
	adds r0, r0, r1
	ldr r1, [r0, #4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _08016B18 @ =gStaticData_0816B61C
	adds r2, r0, r1
	ldr r1, [r4, #0x10]
	adds r0, r4, #0
	bl sub_800B6D0
_08016B12:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08016B18: .4byte gStaticData_0816B61C

	thumb_func_start sub_8016B1C
sub_8016B1C: @ 0x08016B1C
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _08016B54 @ =gUnknown_03001304
	ldr r0, [r0]
	ldr r1, _08016B58 @ =gUnknown_030007E0
	ldr r1, [r1]
	str r1, [sp]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	adds r3, r4, #0
	adds r3, #0x27
	ldrb r0, [r3]
	adds r0, #1
	strb r0, [r3]
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #0x1e
	bne _08016B5C
	movs r0, #1
	adds r1, r4, #0
	adds r1, #0x2d
	strb r0, [r1]
	subs r1, #8
	strb r0, [r1]
	b _08016B74
	.align 2, 0
_08016B54: .4byte gUnknown_03001304
_08016B58: .4byte gUnknown_030007E0
_08016B5C:
	cmp r0, #0x3b
	bls _08016B74
	movs r0, #2
	adds r2, r4, #0
	adds r2, #0x2d
	movs r1, #1
	strb r1, [r2]
	adds r1, r4, #0
	adds r1, #0x25
	strb r0, [r1]
	movs r0, #0
	strb r0, [r3]
_08016B74:
	ldr r2, [r4, #0x10]
	adds r0, r2, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08016B94
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r2, #0
	movs r2, #0x1f
	bl sub_803AD84
_08016B94:
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08016BA8
	adds r0, r4, #0
	bl sub_80159F8
	b _08016BF2
_08016BA8:
	movs r0, #0x81
	lsls r0, r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08016BBA
	adds r0, r4, #0
	bl sub_8015C6C
	b _08016BF2
_08016BBA:
	cmp r5, #0
	beq _08016BF2
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #6
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r3, #0
	ldrsh r0, [r2, r3]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x1d
	bl sub_803AD84
	movs r2, #0xc
	adds r1, r4, #0
	adds r1, #0x2c
	movs r0, #1
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x24
	strb r2, [r0]
_08016BF2:
	adds r1, r4, #0
	adds r1, #0x26
	movs r0, #0
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_8016048
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_8016C08
sub_8016C08: @ 0x08016C08
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _08016C34 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	ldr r0, _08016C38 @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp]
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08016C3C
	adds r0, r4, #0
	bl sub_80159F8
	b _08016C8A
	.align 2, 0
_08016C34: .4byte gUnknown_03001304
_08016C38: .4byte gUnknown_030007E0
_08016C3C:
	movs r0, #0x81
	lsls r0, r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08016C4C
	adds r0, r4, #0
	bl sub_8015C6C
_08016C4C:
	cmp r5, #0
	bne _08016C84
	adds r0, r4, #0
	adds r0, #0x21
	ldrb r0, [r0]
	cmp r0, #6
	bne _08016C84
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #5
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x20
	bl sub_803AD84
	adds r0, r4, #0
	adds r0, #0x22
	strb r5, [r0]
_08016C84:
	adds r0, r4, #0
	bl sub_8016048
_08016C8A:
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8016C94
sub_8016C94: @ 0x08016C94
	push {r4, r5, lr}
	sub sp, #8
	adds r4, r0, #0
	ldr r0, _08016CB8 @ =gUnknown_03001304
	ldr r2, [r0]
	ldr r0, _08016CBC @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp, #4]
	add r5, sp, #4
	ldrh r1, [r5, #2]
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _08016CC0
	adds r0, r4, #0
	bl sub_8015C6C
	b _08016D50
	.align 2, 0
_08016CB8: .4byte gUnknown_03001304
_08016CBC: .4byte gUnknown_030007E0
_08016CC0:
	ldr r0, [r4, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	bne _08016CD4
	ldr r0, _08016CF0 @ =gUnknown_0300082C
	ldr r1, [r0]
	ldr r0, [r4, #0x28]
	cmp r1, r0
	bls _08016D4A
_08016CD4:
	adds r0, r2, #0
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r2, r0, #0x18
	ldrh r0, [r5, #2]
	movs r1, #1
	ands r1, r0
	cmp r1, #0
	beq _08016CF4
	adds r0, r4, #0
	bl sub_80159F8
	b _08016D4A
	.align 2, 0
_08016CF0: .4byte gUnknown_0300082C
_08016CF4:
	cmp r2, #0
	beq _08016D0C
	str r1, [sp]
	adds r0, r4, #0
	movs r1, #1
	movs r2, #1
	ldr r3, _08016D08 @ =0x7FFFFFFF
	bl sub_8017264
	b _08016D4A
	.align 2, 0
_08016D08: .4byte 0x7FFFFFFF
_08016D0C:
	adds r0, r4, #0
	adds r0, #0x21
	ldrb r0, [r0]
	cmp r0, #6
	bne _08016D3C
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #5
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x20
	bl sub_803AD84
	b _08016D4A
_08016D3C:
	str r2, [sp]
	adds r0, r4, #0
	movs r1, #1
	movs r2, #1
	ldr r3, _08016D58 @ =0x7FFFFFFF
	bl sub_8017264
_08016D4A:
	adds r0, r4, #0
	bl sub_8016048
_08016D50:
	add sp, #8
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08016D58: .4byte 0x7FFFFFFF

	thumb_func_start sub_8016D5C
sub_8016D5C: @ 0x08016D5C
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _08016DB0 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r3, r0, #0x18
	ldr r0, [r4, #0x18]
	adds r0, #1
	str r0, [r4, #0x18]
	ldr r1, [r4, #0x1c]
	cmp r0, r1
	bge _08016D84
	ldr r0, [r4, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08016DCA
_08016D84:
	ldr r0, _08016DB4 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x92
	movs r2, #0
	strb r2, [r0]
	adds r1, r4, #0
	adds r1, #0x23
	movs r0, #0xc
	strb r0, [r1]
	cmp r3, #0
	bne _08016DBC
	str r2, [sp]
	adds r0, r4, #0
	movs r1, #1
	movs r2, #1
	ldr r3, _08016DB8 @ =0x7FFFFFFF
	bl sub_8017264
	movs r0, #1
	str r0, [r4, #0x1c]
	b _08016DCA
	.align 2, 0
_08016DB0: .4byte gUnknown_03001304
_08016DB4: .4byte gUnknown_030012D8
_08016DB8: .4byte 0x7FFFFFFF
_08016DBC:
	str r2, [sp]
	adds r0, r4, #0
	movs r1, #1
	movs r2, #1
	ldr r3, _08016DD8 @ =0x7FFFFFFF
	bl sub_8017264
_08016DCA:
	adds r0, r4, #0
	bl sub_8016048
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08016DD8: .4byte 0x7FFFFFFF

	thumb_func_start sub_8016DDC
sub_8016DDC: @ 0x08016DDC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #8
	adds r5, r0, #0
	ldr r0, _08016E34 @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp, #4]
	ldr r1, [r5, #0x1c]
	cmp r1, #0
	beq _08016E86
	ldr r0, [r5, #0x18]
	adds r0, #1
	str r0, [r5, #0x18]
	cmp r0, r1
	blt _08016E86
	adds r2, r5, #0
	adds r2, #0x23
	movs r1, #0
	movs r0, #0xc
	strb r0, [r2]
	str r1, [r5, #0x1c]
	adds r1, r5, #0
	adds r1, #0x22
	ldrb r0, [r1]
	cmp r0, #5
	beq _08016E3C
	cmp r0, #7
	bne _08016E86
	ldr r0, [r5, #0x10]
	ldr r6, [r0, #0x30]
	movs r0, #6
	strb r0, [r1]
	ldr r4, [r5, #0xc]
	adds r4, #0x50
	movs r1, #0
	ldrsh r0, [r4, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r2, _08016E38 @ =gStaticData_0816C070
	movs r3, #0x21
	ldr r2, [r2, #0x18]
	b _08016E56
	.align 2, 0
_08016E34: .4byte gUnknown_030007E0
_08016E38: .4byte gStaticData_0816C070
_08016E3C:
	ldr r0, [r5, #0x10]
	ldr r6, [r0, #0x30]
	movs r0, #4
	strb r0, [r1]
	ldr r4, [r5, #0xc]
	adds r4, #0x50
	movs r7, #0
	ldrsh r0, [r4, r7]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r2, _08016EC4 @ =gStaticData_0816C070
	movs r3, #0x21
	ldr r2, [r2, #0x10]
_08016E56:
	mov r8, r2
	ldrb r7, [r3, r5]
	lsls r2, r7, #2
	add r2, r8
	ldrb r2, [r2]
	ldr r3, [r4, #4]
	bl sub_803AD84
	ldr r4, [r5, #0x10]
	adds r3, r6, #0
	ldr r0, [r4, #0x20]
	adds r2, r4, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r6, [r2]
	lsls r0, r6, #3
	subs r0, r0, r6
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08016E84
	subs r3, r0, #1
_08016E84:
	str r3, [r4, #0x30]
_08016E86:
	add r0, sp, #4
	ldrh r1, [r0, #2]
	movs r4, #0x81
	lsls r4, r4, #1
	ands r4, r1
	cmp r4, #0
	beq _08016F20
	adds r0, r5, #0
	adds r0, #0x23
	ldrb r0, [r0]
	cmp r0, #0
	beq _08016EA0
	b _08017034
_08016EA0:
	ldr r0, [r5, #0x1c]
	cmp r0, #0
	beq _08016EA8
	b _08017034
_08016EA8:
	str r0, [r5, #0x18]
	movs r0, #0x18
	str r0, [r5, #0x1c]
	ldr r0, [r5, #0x10]
	ldr r6, [r0, #0x30]
	adds r0, r5, #0
	adds r0, #0x22
	adds r2, r0, #0
	ldrb r7, [r2]
	cmp r7, #4
	bne _08016EC8
	movs r0, #5
	b _08016ECA
	.align 2, 0
_08016EC4: .4byte gStaticData_0816C070
_08016EC8:
	movs r0, #7
_08016ECA:
	strb r0, [r2]
	ldr r4, [r5, #0xc]
	adds r4, #0x50
	movs r1, #0
	ldrsh r0, [r4, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, _08016F1C @ =gStaticData_0816C070
	ldrb r2, [r2]
	lsls r2, r2, #2
	adds r2, r2, r3
	movs r3, #0x21
	ldr r2, [r2]
	mov r8, r2
	ldrb r7, [r3, r5]
	lsls r2, r7, #2
	add r2, r8
	ldrb r2, [r2]
	ldr r3, [r4, #4]
	bl sub_803AD84
	adds r0, r5, #0
	bl sub_8015C6C
	ldr r4, [r5, #0x10]
	adds r3, r6, #0
	ldr r0, [r4, #0x20]
	adds r2, r4, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r5, [r2]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08016F18
	subs r3, r0, #1
_08016F18:
	str r3, [r4, #0x30]
	b _08017034
	.align 2, 0
_08016F1C: .4byte gStaticData_0816C070
_08016F20:
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08016F2E
	adds r0, r5, #0
	bl sub_80159F8
_08016F2E:
	ldr r2, [r5, #0x10]
	adds r0, r2, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08017034
	adds r6, r5, #0
	adds r6, #0x22
	ldrb r0, [r6]
	cmp r0, #5
	beq _08016F94
	cmp r0, #5
	bgt _08016F4E
	cmp r0, #4
	beq _08016F58
	b _0801702C
_08016F4E:
	cmp r0, #6
	beq _08016F6C
	cmp r0, #7
	beq _08016FE8
	b _0801702C
_08016F58:
	adds r2, #0x28
	ldrb r1, [r2]
	lsls r0, r1, #0x1b
	cmp r0, #0
	bge _08016F7E
	movs r0, #0x11
	rsbs r0, r0, #0
	ands r0, r1
	strb r0, [r2]
	b _08016F7E
_08016F6C:
	adds r0, r2, #0
	adds r0, #0x28
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r6, [r0]
	ands r1, r6
	movs r2, #0x10
	orrs r1, r2
	strb r1, [r0]
_08016F7E:
	str r4, [sp]
	adds r0, r5, #0
	movs r1, #1
	movs r2, #1
	ldr r3, _08016F90 @ =0x7FFFFFFF
	bl sub_8017264
	b _0801702C
	.align 2, 0
_08016F90: .4byte 0x7FFFFFFF
_08016F94:
	adds r2, #0x28
	ldrb r1, [r2]
	lsls r0, r1, #0x1b
	cmp r0, #0
	bge _08016FA6
	movs r0, #0x11
	rsbs r0, r0, #0
	ands r0, r1
	strb r0, [r2]
_08016FA6:
	movs r4, #3
	ldr r1, [r5, #0xc]
	movs r7, #0x20
	ldrsh r0, [r1, r7]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #3
	bl sub_803AD80
	strb r4, [r6]
	ldr r4, [r5, #0xc]
	adds r4, #0x50
	movs r1, #0
	ldrsh r0, [r4, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r2, _08016FE4 @ =gStaticData_0816C070
	movs r3, #0x21
	adds r3, r3, r5
	mov ip, r3
	ldr r3, [r2, #0xc]
	mov r6, ip
	ldrb r6, [r6]
	lsls r2, r6, #2
	adds r2, r2, r3
	ldrb r2, [r2]
	ldr r3, [r4, #4]
	bl sub_803AD84
	b _0801702C
	.align 2, 0
_08016FE4: .4byte gStaticData_0816C070
_08016FE8:
	adds r2, #0x28
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r7, [r2]
	ands r0, r7
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r2]
	movs r4, #3
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #3
	bl sub_803AD80
	strb r4, [r6]
	ldr r4, [r5, #0xc]
	adds r4, #0x50
	movs r3, #0
	ldrsh r0, [r4, r3]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r2, _08017040 @ =gStaticData_0816C070
	movs r6, #0x21
	ldr r3, [r2, #0xc]
	ldrb r7, [r6, r5]
	lsls r2, r7, #2
	adds r2, r2, r3
	ldrb r2, [r2]
	ldr r3, [r4, #4]
	bl sub_803AD84
_0801702C:
	adds r1, r5, #0
	adds r1, #0x2c
	movs r0, #1
	strb r0, [r1]
_08017034:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08017040: .4byte gStaticData_0816C070

	thumb_func_start sub_8017044
sub_8017044: @ 0x08017044
	push {r4, r5, lr}
	sub sp, #8
	adds r4, r0, #0
	ldr r0, _08017068 @ =gUnknown_03001304
	ldr r2, [r0]
	ldr r0, _0801706C @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp, #4]
	add r5, sp, #4
	ldrh r1, [r5, #2]
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _08017070
	adds r0, r4, #0
	bl sub_8015C6C
	b _080170E4
	.align 2, 0
_08017068: .4byte gUnknown_03001304
_0801706C: .4byte gUnknown_030007E0
_08017070:
	ldr r0, [r4, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _080170DE
	adds r0, r2, #0
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r2, r0, #0x18
	ldrh r0, [r5, #2]
	movs r1, #1
	ands r1, r0
	cmp r1, #0
	beq _08017096
	adds r0, r4, #0
	bl sub_80159F8
	b _080170DE
_08017096:
	cmp r2, #0
	beq _080170B0
	str r1, [sp]
	adds r0, r4, #0
	movs r1, #1
	movs r2, #1
	ldr r3, _080170AC @ =0x7FFFFFFF
	bl sub_8017264
	b _080170DE
	.align 2, 0
_080170AC: .4byte 0x7FFFFFFF
_080170B0:
	adds r0, r4, #0
	adds r0, #0x21
	ldrb r0, [r0]
	cmp r0, #6
	bne _080170DE
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #5
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x20
	bl sub_803AD84
_080170DE:
	adds r0, r4, #0
	bl sub_8016048
_080170E4:
	add sp, #8
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_80170EC
sub_80170EC: @ 0x080170EC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r5, r0, #0
	ldr r0, _08017110 @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp]
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08017114
	adds r0, r5, #0
	bl sub_80159F8
	b _08017174
	.align 2, 0
_08017110: .4byte gUnknown_030007E0
_08017114:
	movs r6, #0x81
	lsls r6, r6, #1
	ands r6, r1
	cmp r6, #0
	beq _08017126
	adds r0, r5, #0
	bl sub_8015C6C
	b _08017174
_08017126:
	ldr r0, [r5, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801716E
	adds r4, r5, #0
	adds r4, #0x22
	strb r6, [r4]
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0
	bl sub_803AD80
	strb r6, [r4]
	ldr r4, [r5, #0xc]
	adds r4, #0x50
	movs r3, #0
	ldrsh r0, [r4, r3]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r2, _08017180 @ =gStaticData_0816C070
	movs r7, #0x21
	ldr r2, [r2]
	mov r8, r2
	ldrb r3, [r7, r5]
	lsls r2, r3, #2
	add r2, r8
	ldrb r2, [r2]
	ldr r3, [r4, #4]
	bl sub_803AD84
	str r6, [r5, #0x18]
	str r6, [r5, #0x1c]
_0801716E:
	adds r0, r5, #0
	bl sub_8016048
_08017174:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08017180: .4byte gStaticData_0816C070

	thumb_func_start sub_8017184
sub_8017184: @ 0x08017184
	push {r4, r5, lr}
	adds r5, r0, #0
	movs r0, #0
	movs r1, #5
	movs r2, #0
	bl sub_8015FDC
	movs r4, #0
	ldr r0, _080171BC @ =gUnknown_030012D8
	ldr r2, [r0]
	ldr r1, [r2, #0x60]
	adds r0, r1, #0
	muls r0, r1, r0
	cmp r0, #0
	bge _080171A6
	ldr r3, _080171C0 @ =0x00003FFF
	adds r0, r0, r3
_080171A6:
	asrs r0, r0, #0xe
	adds r3, r0, #4
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r0, r1, r0
	cmp r0, #0
	ble _080171C4
	str r4, [r2, #0x48]
	str r3, [r2, #0x4c]
	b _080171CA
	.align 2, 0
_080171BC: .4byte gUnknown_030012D8
_080171C0: .4byte 0x00003FFF
_080171C4:
	str r4, [r2, #0x48]
	movs r0, #5
	str r0, [r2, #0x4c]
_080171CA:
	str r4, [r2, #0x50]
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801720A
	movs r0, #1
	ldrb r4, [r1, #0xc]
	orrs r0, r4
	strb r0, [r1, #0xc]
	ldr r0, _08017210 @ =0x0000FFFF
	ldrh r2, [r1, #8]
	cmp r2, r0
	beq _0801720A
	ldrh r3, [r1, #8]
	ldr r0, _08017214 @ =gUnknown_030012B4
	ldr r2, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r1, r0, #2
	movs r4, #0x84
	lsls r4, r4, #1
	adds r2, r2, r4
	adds r2, r2, r1
	lsls r0, r0, #5
	subs r0, r3, r0
	movs r1, #1
	lsls r1, r0
	ldr r0, [r2]
	orrs r0, r1
	str r0, [r2]
_0801720A:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08017210: .4byte 0x0000FFFF
_08017214: .4byte gUnknown_030012B4

	thumb_func_start sub_8017218
sub_8017218: @ 0x08017218
	str r1, [r0, #0x10]
	bx lr

	thumb_func_start sub_801721C
sub_801721C: @ 0x0801721C
	push {lr}
	ldr r3, [r0, #4]
	ldr r3, [r3]
	lsls r2, r2, #3
	adds r2, r2, r3
	ldr r3, [r2, #4]
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r2, r2, #2
	ldr r3, _0801723C @ =gStaticData_0816B61C
	adds r2, r2, r3
	bl sub_800B6D0
	pop {r0}
	bx r0
	.align 2, 0
_0801723C: .4byte gStaticData_0816B61C

	thumb_func_start sub_8017240
sub_8017240: @ 0x08017240
	push {lr}
	ldr r3, [r0, #4]
	ldr r3, [r3]
	lsls r2, r2, #3
	adds r2, r2, r3
	ldr r3, [r2]
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r2, r2, #2
	ldr r3, _08017260 @ =gStaticData_0816B61C
	adds r2, r2, r3
	bl sub_800B7B0
	pop {r0}
	bx r0
	.align 2, 0
_08017260: .4byte gStaticData_0816B61C

	thumb_func_start sub_8017264
sub_8017264: @ 0x08017264
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	adds r4, r2, #0
	adds r6, r3, #0
	ldr r2, [r5, #0xc]
	movs r3, #0x20
	ldrsh r0, [r2, r3]
	adds r0, r5, r0
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	adds r2, r5, #0
	adds r2, #0x22
	strb r4, [r2]
	ldr r4, [r5, #0xc]
	adds r4, #0x50
	movs r7, #0
	ldrsh r0, [r4, r7]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, _080172C8 @ =gStaticData_0816C070
	ldrb r2, [r2]
	lsls r2, r2, #2
	adds r2, r2, r3
	movs r3, #0x21
	ldr r2, [r2]
	mov r8, r2
	ldrb r7, [r3, r5]
	lsls r2, r7, #2
	add r2, r8
	ldrb r2, [r2]
	ldr r3, [r4, #4]
	bl sub_803AD84
	ldr r0, _080172CC @ =0x7FFFFFFF
	cmp r6, r0
	beq _080172B4
	str r6, [r5, #0x18]
_080172B4:
	ldr r1, [sp, #0x18]
	cmp r1, r0
	beq _080172BC
	str r1, [r5, #0x1c]
_080172BC:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080172C8: .4byte gStaticData_0816C070
_080172CC: .4byte 0x7FFFFFFF

	thumb_func_start sub_80172D0
sub_80172D0: @ 0x080172D0
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	mov ip, r1
	adds r5, r2, #0
	ldr r0, _08017308 @ =gUnknown_030012D8
	ldr r3, [r0]
	ldr r4, [r3, #0x60]
	adds r0, r4, #0
	muls r0, r4, r0
	cmp r0, #0
	bge _080172EA
	ldr r1, _0801730C @ =0x00003FFF
	adds r0, r0, r1
_080172EA:
	asrs r0, r0, #0xe
	adds r7, r0, #4
	asrs r0, r4, #0x1f
	adds r1, r4, #0
	eors r1, r0
	subs r1, r1, r0
	asrs r2, r5, #0x1f
	adds r0, r5, #0
	eors r0, r2
	subs r0, r0, r2
	cmp r1, r0
	ble _08017310
	str r6, [r3, #0x48]
	str r7, [r3, #0x4c]
	b _08017326
	.align 2, 0
_08017308: .4byte gUnknown_030012D8
_0801730C: .4byte 0x00003FFF
_08017310:
	adds r0, r4, #0
	muls r0, r5, r0
	cmp r0, #0
	bge _08017320
	mov r1, ip
	adds r0, r7, r1
	str r6, [r3, #0x48]
	b _08017324
_08017320:
	str r6, [r3, #0x48]
	mov r0, ip
_08017324:
	str r0, [r3, #0x4c]
_08017326:
	str r5, [r3, #0x50]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
	thumb_func_start sub_8017330
sub_8017330: @ 0x08017330
	adds	r1, r0, #0
	muls	r1, r0
	adds	r0, r1, #0
	cmp	r0, #0
	bge _0801733E
	ldr r1, _08017344
	adds	r0, r0, r1
_0801733E:
	asrs	r0, r0, #14
	adds	r0, #4
	bx	lr
_08017344: .4byte 0x3fff

	thumb_func_start sub_8017348
sub_8017348: @ 0x08017348
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r5, r0, #0
	ldr r6, [r5, #0x10]
	adds r3, r6, #0
	adds r3, #0x2d
	ldrb r0, [r3]
	cmp r0, #0x20
	beq _080173BC
	cmp r0, #0x1d
	beq _080173BC
	cmp r0, #0x1f
	beq _080173BC
	ldr r4, [r6, #0x30]
	ldr r7, [r6, #0x34]
	ldr r1, _080173B8 @ =gStaticData_0816C070
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	lsls r0, r0, #2
	adds r0, r0, r1
	adds r2, r5, #0
	adds r2, #0x21
	ldr r1, [r0]
	ldrb r2, [r2]
	lsls r0, r2, #2
	adds r0, r0, r1
	ldrb r0, [r0]
	strb r0, [r3]
	adds r0, r6, #0
	bl sub_80087C0
	adds r0, r6, #0
	bl sub_80087B4
	adds r0, r6, #0
	movs r1, #0
	bl sub_800872C
	ldr r3, [r5, #0x10]
	ldr r0, [r3, #0x20]
	adds r2, r3, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r5, [r2]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _080173B2
	subs r4, r0, #1
_080173B2:
	str r4, [r3, #0x30]
	str r7, [r3, #0x34]
	b _080174B0
	.align 2, 0
_080173B8: .4byte gStaticData_0816C070
_080173BC:
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	subs r0, #7
	cmp r0, #0x22
	bhi _080174A0
	lsls r0, r0, #2
	ldr r1, _080173D4 @ =_080173D8
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080173D4: .4byte _080173D8
_080173D8: @ jump table
	.4byte _08017464 @ case 0
	.4byte _080174A0 @ case 1
	.4byte _080174A0 @ case 2
	.4byte _080174A0 @ case 3
	.4byte _080174A0 @ case 4
	.4byte _080174A0 @ case 5
	.4byte _080174A0 @ case 6
	.4byte _08017464 @ case 7
	.4byte _08017464 @ case 8
	.4byte _08017464 @ case 9
	.4byte _08017464 @ case 10
	.4byte _08017464 @ case 11
	.4byte _080174A0 @ case 12
	.4byte _080174A0 @ case 13
	.4byte _080174A0 @ case 14
	.4byte _080174A0 @ case 15
	.4byte _08017464 @ case 16
	.4byte _080174A0 @ case 17
	.4byte _080174A0 @ case 18
	.4byte _080174A0 @ case 19
	.4byte _080174A0 @ case 20
	.4byte _080174A0 @ case 21
	.4byte _080174A0 @ case 22
	.4byte _080174A0 @ case 23
	.4byte _080174A0 @ case 24
	.4byte _080174A0 @ case 25
	.4byte _08017464 @ case 26
	.4byte _08017464 @ case 27
	.4byte _08017464 @ case 28
	.4byte _08017464 @ case 29
	.4byte _08017464 @ case 30
	.4byte _080174A0 @ case 31
	.4byte _080174A0 @ case 32
	.4byte _080174A0 @ case 33
	.4byte _08017464 @ case 34
_08017464:
	ldr r0, [r5, #0x10]
	ldr r4, [r0, #0x30]
	ldr r6, [r0, #0x34]
	movs r0, #0
	str r0, [sp]
	adds r0, r5, #0
	movs r1, #1
	movs r2, #1
	ldr r3, _0801749C @ =0x7FFFFFFF
	bl sub_8017264
	ldr r3, [r5, #0x10]
	ldr r0, [r3, #0x20]
	adds r2, r3, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r5, [r2]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _08017496
	subs r4, r0, #1
_08017496:
	str r4, [r3, #0x30]
	str r6, [r3, #0x34]
	b _080174B0
	.align 2, 0
_0801749C: .4byte 0x7FFFFFFF
_080174A0:
	movs r0, #0
	str r0, [sp]
	adds r0, r5, #0
	movs r1, #1
	movs r2, #1
	ldr r3, _080174B8 @ =0x7FFFFFFF
	bl sub_8017264
_080174B0:
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080174B8: .4byte 0x7FFFFFFF

	thumb_func_start sub_80174BC
sub_80174BC: @ 0x080174BC
	push {lr}
	sub sp, #4
	ldr r3, _080174D4 @ =0x7FFFFFFF
	movs r1, #0
	str r1, [sp]
	movs r1, #1
	movs r2, #1
	bl sub_8017264
	add sp, #4
	pop {r0}
	bx r0
	.align 2, 0
_080174D4: .4byte 0x7FFFFFFF

	thumb_func_start sub_80174D8
sub_80174D8: @ 0x080174D8
	push {lr}
	ldr r2, _080174E8 @ =gStaticData_087E428C
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_080174E8: .4byte gStaticData_087E428C

	thumb_func_start sub_80174EC
sub_80174EC: @ 0x080174EC
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800B8C8
	ldr r0, _08017508 @ =gStaticData_087E428C
	str r0, [r4, #0xc]
	adds r0, r4, #0
	bl sub_8015958
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08017508: .4byte gStaticData_087E428C

	thumb_func_start sub_801750C
sub_801750C: @ 0x0801750C
	movs r1, #0
	str r1, [r0, #0x14]
	bx lr
	.align 2, 0

	thumb_func_start sub_8017514
sub_8017514: @ 0x08017514
	adds r0, #0x2d
	movs r1, #1
	strb r1, [r0]
	bx lr

	thumb_func_start sub_801751C
sub_801751C: @ 0x0801751C
	adds r0, #0x2c
	movs r1, #1
	strb r1, [r0]
	bx lr

	thumb_func_start sub_8017524
sub_8017524: @ 0x08017524
	adds r0, #0x2d
	movs r1, #0
	strb r1, [r0]
	bx lr

	thumb_func_start sub_801752C
sub_801752C: @ 0x0801752C
	adds r0, #0x2c
	movs r1, #0
	strb r1, [r0]
	bx lr

	thumb_func_start sub_8017534
sub_8017534: @ 0x08017534
	adds r0, #0x2d
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_801753C
sub_801753C: @ 0x0801753C
	adds r0, #0x2c
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8017544
sub_8017544: @ 0x08017544
	adds r3, r0, #0
	adds r3, #0x2d
	movs r2, #1
	strb r2, [r3]
	adds r0, #0x25
	strb r1, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8017554
sub_8017554: @ 0x08017554
	adds r3, r0, #0
	adds r3, #0x2c
	movs r2, #1
	strb r2, [r3]
	adds r0, #0x24
	strb r1, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8017564
sub_8017564: @ 0x08017564
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, _080175F4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x1b
	bl PlaySfx
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #3
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	adds r2, r5, #0
	bl sub_803AD84
	ldr r1, [r4, #0x10]
	movs r0, #0x7f
	ldrb r2, [r1, #0xc]
	ands r0, r2
	strb r0, [r1, #0xc]
	ldr r1, [r4, #0x10]
	movs r0, #0x41
	rsbs r0, r0, #0
	ldrb r5, [r1, #0xc]
	ands r0, r5
	strb r0, [r1, #0xc]
	ldr r0, [r4, #0x10]
	movs r1, #0x82
	lsls r1, r1, #1
	adds r0, r0, r1
	movs r1, #1
	strb r1, [r0]
	ldr r0, _080175F8 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023234
	ldr r0, _080175FC @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r3, [r4, #0x10]
	adds r1, r3, #0
	adds r1, #0x29
	ldrb r1, [r1]
	lsls r1, r1, #0x1c
	lsrs r1, r1, #0x1c
	ldr r2, [r3, #0x20]
	adds r3, #0x2d
	ldr r4, [r2]
	ldrb r5, [r3]
	lsls r2, r5, #3
	subs r2, r2, r5
	lsls r2, r2, #2
	adds r2, r2, r4
	ldrb r2, [r2, #0x14]
	bl sub_8006D08
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080175F4: .4byte gUnknown_030012BC
_080175F8: .4byte gUnknown_030012C0
_080175FC: .4byte gUnknown_030012B8

	thumb_func_start sub_8017600
sub_8017600: @ 0x08017600
	push {r4, r5, lr}
	sub sp, #4
	adds r5, r0, #0
	movs r4, #0
	str r4, [sp]
	movs r1, #1
	movs r2, #0
	movs r3, #0
	bl sub_80178BC
	movs r0, #1
	strb r0, [r5, #0x17]
	strb r0, [r5, #0x14]
	strb r0, [r5, #0x18]
	strb r4, [r5, #0x15]
	strb r4, [r5, #0x16]
	ldr r0, [r5, #0x1c]
	cmp r0, #0
	bne _0801763C
	movs r0, #0x80
	bl sub_8026EDC
	bl sub_801B940
	adds r1, r0, #0
	str r1, [r5, #0x1c]
	ldr r0, _0801764C @ =gUnknown_030012F0
	ldr r0, [r0]
	bl sub_8008E94
_0801763C:
	ldr r0, [r5, #0x1c]
	bl sub_801B864
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0801764C: .4byte gUnknown_030012F0

	thumb_func_start sub_8017650
sub_8017650: @ 0x08017650
	push {r4, r5, r6, r7, lr}
	sub sp, #8
	adds r4, r0, #0
	ldr r0, [r4, #8]
	cmp r0, #3
	bne _0801765E
	b _080177A2
_0801765E:
	ldr r0, [r4, #0x10]
	ldr r1, [r0]
	ldr r0, _080176D0 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x10]
	lsls r0, r0, #8
	ldr r2, _080176D4 @ =0xFFFFF600
	adds r0, r0, r2
	cmp r1, r0
	ble _080176B0
	ldr r1, [r4, #0x1c]
	movs r0, #1
	ldrb r3, [r1, #0xc]
	orrs r0, r3
	strb r0, [r1, #0xc]
	ldr r0, _080176D8 @ =0x0000FFFF
	ldrh r5, [r1, #8]
	cmp r5, r0
	beq _080176A8
	ldrh r3, [r1, #8]
	ldr r0, _080176DC @ =gUnknown_030012B4
	ldr r2, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r1, r0, #2
	movs r6, #0x84
	lsls r6, r6, #1
	adds r2, r2, r6
	adds r2, r2, r1
	lsls r0, r0, #5
	subs r0, r3, r0
	movs r1, #1
	lsls r1, r0
	ldr r0, [r2]
	orrs r0, r1
	str r0, [r2]
_080176A8:
	movs r0, #0
	str r0, [r4, #0x1c]
	bl sub_80241A4
_080176B0:
	ldr r0, _080176E0 @ =gUnknown_030007E0
	ldr r2, [r0]
	movs r0, #0x40
	ands r0, r2
	cmp r0, #0
	beq _080176E4
	ldrb r0, [r4, #0x16]
	cmp r0, #1
	beq _080176E4
	movs r1, #3
	movs r0, #1
	strb r0, [r4, #0x18]
	strb r1, [r4, #0x15]
	strb r0, [r4, #0x16]
	b _08017710
	.align 2, 0
_080176D0: .4byte gUnknown_03001308
_080176D4: .4byte 0xFFFFF600
_080176D8: .4byte 0x0000FFFF
_080176DC: .4byte gUnknown_030012B4
_080176E0: .4byte gUnknown_030007E0
_080176E4:
	movs r0, #0x80
	ands r0, r2
	cmp r0, #0
	beq _08017700
	ldrb r1, [r4, #0x16]
	cmp r1, #2
	beq _08017700
	movs r0, #5
	movs r1, #1
	strb r1, [r4, #0x18]
	strb r0, [r4, #0x15]
	movs r0, #2
	strb r0, [r4, #0x16]
	b _08017710
_08017700:
	movs r1, #0xc0
	ands r1, r2
	cmp r1, #0
	bne _08017710
	movs r0, #1
	strb r0, [r4, #0x18]
	strb r1, [r4, #0x15]
	strb r1, [r4, #0x16]
_08017710:
	movs r0, #0x20
	ands r0, r2
	adds r3, r4, #0
	adds r3, #0x20
	cmp r0, #0
	beq _08017746
	ldrb r0, [r3]
	cmp r0, #0
	beq _08017746
	movs r1, #7
	movs r0, #1
	strb r0, [r4, #0x17]
	strb r1, [r4, #0x14]
	movs r1, #0xc8
	lsls r1, r1, #6
	ldr r0, [r4, #0x1c]
	str r1, [r0, #0x78]
	ldr r0, [r4, #0x24]
	adds r0, #1
	str r0, [r4, #0x24]
	cmp r0, #0x1e
	ble _08017784
	movs r0, #0
	strb r0, [r3]
	movs r0, #0xa
	str r0, [r4, #0x24]
	b _08017784
_08017746:
	movs r0, #0x10
	ands r0, r2
	cmp r0, #0
	beq _08017760
	movs r1, #8
	movs r0, #1
	strb r0, [r4, #0x17]
	strb r1, [r4, #0x14]
	movs r1, #0xa0
	lsls r1, r1, #4
	ldr r0, [r4, #0x1c]
	str r1, [r0, #0x78]
	b _08017784
_08017760:
	movs r0, #0x30
	ands r0, r2
	cmp r0, #0
	beq _08017776
	movs r0, #0x20
	ands r0, r2
	cmp r0, #0
	beq _08017784
	ldrb r0, [r3]
	cmp r0, #0
	bne _080177A2
_08017776:
	movs r0, #0xf0
	lsls r0, r0, #5
	ldr r1, [r4, #0x1c]
	str r0, [r1, #0x78]
	movs r0, #1
	strb r0, [r4, #0x17]
	strb r0, [r4, #0x14]
_08017784:
	ldrb r1, [r3]
	cmp r1, #0
	bne _080177A2
	ldr r0, [r4, #0x24]
	subs r0, #1
	str r0, [r4, #0x24]
	cmp r0, #0
	bge _080177A2
	str r1, [r4, #0x24]
	movs r0, #0x20
	ands r2, r0
	cmp r2, #0
	bne _080177A2
	movs r0, #1
	strb r0, [r3]
_080177A2:
	ldr r1, _080177D0 @ =gStaticData_0816C290
	ldr r0, [r4, #8]
	lsls r5, r0, #3
	adds r2, r5, r1
	movs r6, #2
	ldrsh r3, [r2, r6]
	adds r6, r0, #0
	adds r7, r1, #0
	cmp r3, #0
	ble _080177D4
	movs r1, #4
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r0]
	lsls r0, r3, #3
	adds r0, r0, r1
	subs r0, #8
	ldr r1, [r0]
	ldr r2, [r0, #4]
	str r1, [sp]
	str r2, [sp, #4]
	ldr r2, [sp, #4]
	b _080177DA
	.align 2, 0
_080177D0: .4byte gStaticData_0816C290
_080177D4:
	adds r0, r7, #4
	adds r0, r5, r0
	ldr r2, [r0]
_080177DA:
	lsls r0, r6, #3
	adds r0, r0, r7
	movs r5, #0
	ldrsh r1, [r0, r5]
	cmp r3, #0
	ble _080177F0
	ldr r6, [sp]
	lsls r0, r6, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _080177F2
_080177F0:
	adds r0, r1, #0
_080177F2:
	adds r0, r4, r0
	bl sub_803AD80
	adds r0, r4, #0
	bl sub_8017808
	add sp, #8
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8017808
sub_8017808: @ 0x08017808
	push {r4, r5, lr}
	adds r4, r0, #0
	ldrb r0, [r4, #0x17]
	cmp r0, #1
	bne _08017860
	ldr r0, [r4, #4]
	ldr r1, [r0]
	ldrb r2, [r4, #0x14]
	lsls r0, r2, #3
	adds r0, r0, r1
	ldr r1, [r0]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _08017844 @ =gStaticData_0816B8C0
	adds r5, r0, r1
	ldrb r0, [r4, #0x19]
	cmp r0, #0
	beq _08017848
	ldr r2, [r4, #0xc]
	movs r1, #0x38
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #0x3c]
	adds r2, r5, #0
	bl sub_803AD84
	b _0801785A
	.align 2, 0
_08017844: .4byte gStaticData_0816B8C0
_08017848:
	ldr r2, [r4, #0xc]
	movs r1, #0x28
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #0x2c]
	adds r2, r5, #0
	bl sub_803AD84
_0801785A:
	movs r0, #0
	strb r0, [r4, #0x17]
	strb r0, [r4, #0x19]
_08017860:
	ldrb r2, [r4, #0x18]
	cmp r2, #1
	bne _080178B4
	ldr r0, [r4, #4]
	ldr r1, [r0]
	ldrb r2, [r4, #0x15]
	lsls r0, r2, #3
	adds r0, r0, r1
	ldr r1, [r0, #4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _08017898 @ =gStaticData_0816B8C0
	adds r5, r0, r1
	ldrb r0, [r4, #0x1a]
	cmp r0, #0
	beq _0801789C
	ldr r2, [r4, #0xc]
	adds r2, #0x40
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	adds r2, r5, #0
	bl sub_803AD84
	b _080178AE
	.align 2, 0
_08017898: .4byte gStaticData_0816B8C0
_0801789C:
	ldr r2, [r4, #0xc]
	movs r1, #0x30
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #0x34]
	adds r2, r5, #0
	bl sub_803AD84
_080178AE:
	movs r0, #0
	strb r0, [r4, #0x18]
	strb r0, [r4, #0x1a]
_080178B4:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80178BC
sub_80178BC: @ 0x080178BC
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r2, #0
	ldr r2, [r4, #0xc]
	movs r3, #0x20
	ldrsh r0, [r2, r3]
	adds r0, r4, r0
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	adds r2, r5, #0
	bl sub_803AD84
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80178EC
sub_80178EC: @ 0x080178EC
	push {r4, lr}
	ldr r1, [r0, #0x10]
	adds r0, r1, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801792C
	movs r0, #1
	ldrb r2, [r1, #0xc]
	orrs r0, r2
	strb r0, [r1, #0xc]
	ldr r0, _08017934 @ =0x0000FFFF
	ldrh r4, [r1, #8]
	cmp r4, r0
	beq _0801792C
	ldrh r3, [r1, #8]
	ldr r0, _08017938 @ =gUnknown_030012B4
	ldr r2, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r1, r0, #2
	movs r4, #0x84
	lsls r4, r4, #1
	adds r2, r2, r4
	adds r2, r2, r1
	lsls r0, r0, #5
	subs r0, r3, r0
	movs r1, #1
	lsls r1, r0
	ldr r0, [r2]
	orrs r0, r1
	str r0, [r2]
_0801792C:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08017934: .4byte 0x0000FFFF
_08017938: .4byte gUnknown_030012B4

	thumb_func_start sub_801793C
sub_801793C: @ 0x0801793C
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, [r4, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08017964
	movs r0, #0
	str r0, [sp]
	adds r0, r4, #0
	movs r1, #1
	movs r2, #0
	movs r3, #0
	bl sub_80178BC
	movs r1, #2
	movs r0, #1
	strb r0, [r4, #0x17]
	strb r1, [r4, #0x14]
_08017964:
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_801796C
sub_801796C: @ 0x0801796C
	push {lr}
	sub sp, #4
	adds r1, r0, #0
	ldr r0, [r1, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801798C
	movs r0, #0
	str r0, [sp]
	adds r0, r1, #0
	movs r1, #2
	movs r2, #0
	movs r3, #0
	bl sub_80178BC
_0801798C:
	add sp, #4
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8017994
sub_8017994: @ 0x08017994
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r0, #0
	movs r5, #0
	str r5, [sp]
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_80178BC
	movs r0, #1
	strb r0, [r4, #0x17]
	strb r5, [r4, #0x14]
	strb r0, [r4, #0x18]
	strb r5, [r4, #0x15]
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80179BC
sub_80179BC: @ 0x080179BC
	movs r2, #1
	str r2, [r0, #8]
	movs r1, #0
	strb r1, [r0, #0x14]
	strb r1, [r0, #0x15]
	strb r2, [r0, #0x17]
	strb r2, [r0, #0x18]
	str r1, [r0, #0x10]
	str r1, [r0, #0x1c]
	adds r0, #0x20
	strb r1, [r0]
	bx lr

	thumb_func_start sub_80179D4
sub_80179D4: @ 0x080179D4
	push {lr}
	cmp r2, #1
	blt _080179E4
	cmp r2, #4
	bgt _080179E4
	movs r1, #1
	bl sub_8017564
_080179E4:
	pop {r0}
	bx r0

	thumb_func_start sub_80179E8
sub_80179E8: @ 0x080179E8
	str r1, [r0, #0x10]
	bx lr

	thumb_func_start sub_80179EC
sub_80179EC: @ 0x080179EC
	push {lr}
	ldr r2, _080179FC @ =gStaticData_087E42F4
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_080179FC: .4byte gStaticData_087E42F4

	thumb_func_start sub_8017A00
sub_8017A00: @ 0x08017A00
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800B8C8
	ldr r0, _08017A1C @ =gStaticData_087E42F4
	str r0, [r4, #0xc]
	adds r0, r4, #0
	bl sub_80179BC
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08017A1C: .4byte gStaticData_087E42F4

	thumb_func_start sub_8017A20
sub_8017A20: @ 0x08017A20
	movs r1, #1
	strb r1, [r0, #0x18]
	bx lr
	.align 2, 0

	thumb_func_start sub_8017A28
sub_8017A28: @ 0x08017A28
	movs r1, #1
	strb r1, [r0, #0x17]
	bx lr
	.align 2, 0

	thumb_func_start sub_8017A30
sub_8017A30: @ 0x08017A30
	movs r1, #0
	strb r1, [r0, #0x18]
	strb r1, [r0, #0x1a]
	bx lr

	thumb_func_start sub_8017A38
sub_8017A38: @ 0x08017A38
	movs r1, #0
	strb r1, [r0, #0x17]
	strb r1, [r0, #0x19]
	bx lr

	thumb_func_start sub_8017A40
sub_8017A40: @ 0x08017A40
	ldrb r0, [r0, #0x18]
	bx lr

	thumb_func_start sub_8017A44
sub_8017A44: @ 0x08017A44
	ldrb r0, [r0, #0x17]
	bx lr

	thumb_func_start sub_8017A48
sub_8017A48: @ 0x08017A48
	movs r2, #1
	strb r2, [r0, #0x1a]
	strb r2, [r0, #0x18]
	strb r1, [r0, #0x15]
	bx lr
	.align 2, 0

	thumb_func_start sub_8017A54
sub_8017A54: @ 0x08017A54
	movs r2, #1
	strb r2, [r0, #0x19]
	strb r2, [r0, #0x17]
	strb r1, [r0, #0x14]
	bx lr
	.align 2, 0

	thumb_func_start sub_8017A60
sub_8017A60: @ 0x08017A60
	movs r2, #1
	strb r2, [r0, #0x18]
	strb r1, [r0, #0x15]
	bx lr

	thumb_func_start sub_8017A68
sub_8017A68: @ 0x08017A68
	movs r2, #1
	strb r2, [r0, #0x17]
	strb r1, [r0, #0x14]
	bx lr

	thumb_func_start sub_8017A70
sub_8017A70: @ 0x08017A70
	str r2, [r0, #0x14]
	str r3, [r0, #0x18]
	bx lr
	.align 2, 0

	thumb_func_start sub_8017A78
sub_8017A78: @ 0x08017A78
	push {lr}
	ldr r2, _08017A88 @ =gStaticData_087E435C
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_08017A88: .4byte gStaticData_087E435C

	thumb_func_start sub_8017A8C
sub_8017A8C: @ 0x08017A8C
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800B8C8
	ldr r0, _08017AA8 @ =gStaticData_087E435C
	str r0, [r4, #0xc]
	movs r0, #0
	str r0, [r4, #0x10]
	str r0, [r4, #0x14]
	str r0, [r4, #0x18]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08017AA8: .4byte gStaticData_087E435C

	thumb_func_start sub_8017AAC
sub_8017AAC: @ 0x08017AAC
	ldr r0, [r0, #0x10]
	bx lr

	thumb_func_start sub_8017AB0
sub_8017AB0: @ 0x08017AB0
	push {r4, r5, r6, lr}
	sub sp, #0x1c
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r1, [r4, #0x1c]
	movs r0, #1
	rsbs r0, r0, #0
	cmp r1, r0
	bne _08017AD0
	adds r0, r4, #0
	adds r1, r5, #0
	movs r2, #1
	bl sub_8017F14
	movs r0, #0
	str r0, [r4, #0x1c]
_08017AD0:
	ldr r0, [r4, #8]
	cmp r0, #1
	beq _08017B10
	cmp r0, #1
	bgt _08017AE0
	cmp r0, #0
	beq _08017AE8
	b _08017EC4
_08017AE0:
	cmp r0, #2
	bne _08017AE6
	b _08017DC0
_08017AE6:
	b _08017EC4
_08017AE8:
	ldr r0, _08017B0C @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r1, #0x82
	lsls r1, r1, #1
	adds r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0
	beq _08017AFA
	b _08017EC4
_08017AFA:
	adds r0, r5, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _08017B08
	b _08017E42
_08017B08:
	b _08017E5C
	.align 2, 0
_08017B0C: .4byte gUnknown_030012D8
_08017B10:
	ldr r0, _08017B8C @ =gUnknown_030012D8
	ldr r1, [r0]
	movs r3, #0x82
	lsls r3, r3, #1
	adds r0, r1, r3
	ldrb r0, [r0]
	cmp r0, #0
	bne _08017B28
	ldr r0, [r1, #0x44]
	ldr r0, [r0, #8]
	cmp r0, #0x1e
	bne _08017B60
_08017B28:
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r5, #0
	movs r2, #2
	bl sub_803AD84
	ldr r1, [r4, #0xc]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0
	bl sub_803AD80
	ldr r1, [r4, #0xc]
	adds r1, #0x58
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r5, #0
	movs r2, #0
	bl sub_803AD84
_08017B60:
	ldr r1, [r5, #0x18]
	movs r3, #0x28
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r1, [r1, #0x2c]
	bl sub_803AD7C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08017B94
	adds r1, r4, #0
	adds r1, #0x20
	ldrb r0, [r1]
	cmp r0, #0
	bne _08017B94
	ldr r0, _08017B90 @ =gUnknown_0300082C
	ldr r0, [r0]
	str r0, [r4, #0x1c]
	movs r0, #1
	strb r0, [r1]
	b _08017BBC
	.align 2, 0
_08017B8C: .4byte gUnknown_030012D8
_08017B90: .4byte gUnknown_0300082C
_08017B94:
	ldr r1, [r5, #0x18]
	movs r2, #0x28
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r1, [r1, #0x2c]
	bl sub_803AD7C
	lsls r0, r0, #0x18
	lsrs r2, r0, #0x18
	cmp r2, #0
	bne _08017BBC
	adds r1, r4, #0
	adds r1, #0x20
	ldrb r0, [r1]
	cmp r0, #0
	beq _08017BBC
	ldr r0, _08017BF4 @ =gUnknown_0300082C
	ldr r0, [r0]
	str r0, [r4, #0x1c]
	strb r2, [r1]
_08017BBC:
	ldr r1, [r4, #0x1c]
	cmp r1, #0
	beq _08017C0E
	ldr r0, _08017BF4 @ =gUnknown_0300082C
	ldr r0, [r0]
	subs r0, r0, r1
	cmp r0, #0x3c
	bls _08017C0E
	movs r0, #0
	str r0, [r4, #0x1c]
	adds r0, r5, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	blt _08017C04
	adds r0, r4, #0
	adds r0, #0x20
	ldrb r0, [r0]
	cmp r0, #0
	beq _08017BF8
	adds r0, r4, #0
	adds r1, r5, #0
	movs r2, #1
	bl sub_8017F14
	b _08017C0E
	.align 2, 0
_08017BF4: .4byte gUnknown_0300082C
_08017BF8:
	adds r0, r4, #0
	adds r1, r5, #0
	movs r2, #2
	bl sub_8017F14
	b _08017C0E
_08017C04:
	adds r0, r4, #0
	adds r1, r5, #0
	movs r2, #3
	bl sub_8017F14
_08017C0E:
	ldr r0, _08017CD8 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r5]
	cmp r1, r0
	bge _08017C46
	adds r2, r5, #0
	adds r2, #0x28
	ldrb r1, [r2]
	lsls r0, r1, #0x1b
	cmp r0, #0
	blt _08017C46
	movs r0, #0x11
	rsbs r0, r0, #0
	ands r0, r1
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r2]
	ldr r1, [r4, #0xc]
	adds r1, #0x58
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r5, #0
	movs r2, #3
	bl sub_803AD84
_08017C46:
	ldr r0, _08017CD8 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r5]
	movs r2, #0xa0
	lsls r2, r2, #4
	adds r0, r0, r2
	cmp r1, r0
	ble _08017C80
	adds r2, r5, #0
	adds r2, #0x28
	ldrb r1, [r2]
	lsls r0, r1, #0x1b
	cmp r0, #0
	bge _08017C80
	movs r0, #0x11
	rsbs r0, r0, #0
	ands r0, r1
	strb r0, [r2]
	ldr r1, [r4, #0xc]
	adds r1, #0x58
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r5, #0
	movs r2, #1
	bl sub_803AD84
_08017C80:
	ldr r0, _08017CD8 @ =gUnknown_030012D8
	ldr r2, [r0]
	ldr r0, [r2]
	ldr r1, [r5]
	subs r0, r0, r1
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	ldr r1, _08017CDC @ =0x000027FF
	cmp r0, r1
	bgt _08017CE4
	ldr r0, [r2, #4]
	ldr r1, [r5, #4]
	subs r0, r0, r1
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	ldr r1, _08017CE0 @ =0x000031FF
	cmp r0, r1
	bgt _08017CE4
	adds r0, r4, #0
	adds r1, r5, #0
	movs r2, #0
	bl sub_8017F14
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #2
	bl sub_803AD80
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r5, #0
	movs r2, #1
	bl sub_803AD84
	b _08017EC4
	.align 2, 0
_08017CD8: .4byte gUnknown_030012D8
_08017CDC: .4byte 0x000027FF
_08017CE0: .4byte 0x000031FF
_08017CE4:
	add r0, sp, #0xc
	adds r1, r5, #0
	bl sub_8007B98
	ldr r0, _08017D90 @ =gUnknown_030012F0
	ldr r0, [r0]
	movs r1, #0
	str r1, [sp, #4]
	str r5, [sp, #8]
	ldr r1, [sp, #0x18]
	str r1, [sp]
	ldr r1, [sp, #0xc]
	ldr r2, [sp, #0x10]
	ldr r3, [sp, #0x14]
	bl sub_8008A40
	movs r6, #0
	ldr r0, _08017D94 @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r0, [r0]
	cmp r6, r0
	blt _08017D12
	b _08017EC4
_08017D12:
	ldr r0, _08017D94 @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r6, #2
	adds r0, r0, r1
	ldr r4, [r0]
	ldr r1, [r4, #0x18]
	adds r1, #0x48
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	cmp r0, #3
	bne _08017DAC
	adds r2, r4, #0
	ldr r0, [r4]
	asrs r0, r0, #8
	ldr r1, [r5]
	asrs r1, r1, #8
	subs r0, r0, r1
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	cmp r0, #0x27
	bgt _08017DAC
	ldr r0, [r4, #4]
	asrs r0, r0, #8
	ldr r1, [r5, #4]
	asrs r1, r1, #8
	subs r0, r0, r1
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	cmp r0, #0x3b
	bgt _08017DAC
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _08017DAC
	adds r0, r4, #0
	adds r0, #0x4e
	ldrb r1, [r0]
	cmp r1, #0xe
	beq _08017D84
	cmp r1, #0x13
	beq _08017D84
	cmp r1, #0x14
	beq _08017D84
	cmp r1, #0x15
	beq _08017D84
	cmp r1, #0xa
	bne _08017D98
_08017D84:
	adds r0, r2, #0
	movs r1, #0
	bl sub_800EEF0
	b _08017DAC
	.align 2, 0
_08017D90: .4byte gUnknown_030012F0
_08017D94: .4byte gUnknown_0300130C
_08017D98:
	adds r0, r4, #0
	bl sub_8010908
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08017DAC
	adds r0, r4, #0
	movs r1, #1
	bl sub_800E888
_08017DAC:
	adds r6, #1
	ldr r0, _08017DBC @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r0, [r0]
	cmp r6, r0
	blt _08017D12
	b _08017EC4
	.align 2, 0
_08017DBC: .4byte gUnknown_0300130C
_08017DC0:
	ldr r0, [r5, #0x30]
	cmp r0, #8
	bne _08017E1C
	ldr r0, [r5, #0x34]
	cmp r0, #0
	bne _08017E1C
	ldr r0, _08017E10 @ =gUnknown_030012D8
	ldr r2, [r0]
	ldr r0, [r2]
	ldr r1, [r5]
	subs r0, r0, r1
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	ldr r1, _08017E14 @ =0x000027FF
	cmp r0, r1
	ble _08017DE4
	b _08017AFA
_08017DE4:
	ldr r0, [r2, #4]
	ldr r1, [r5, #4]
	subs r0, r0, r1
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	ldr r1, _08017E18 @ =0x000031FF
	cmp r0, r1
	ble _08017DF8
	b _08017AFA
_08017DF8:
	ldr r1, [r2, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #0
	bl sub_803AD88
	b _08017EC4
	.align 2, 0
_08017E10: .4byte gUnknown_030012D8
_08017E14: .4byte 0x000027FF
_08017E18: .4byte 0x000031FF
_08017E1C:
	adds r0, r5, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08017EC4
	ldr r0, _08017E58 @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r3, #0x82
	lsls r3, r3, #1
	adds r0, r0, r3
	ldrb r0, [r0]
	cmp r0, #0
	bne _08017E8C
	adds r0, r5, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _08017E5C
_08017E42:
	ldr r1, [r4, #0xc]
	adds r1, #0x58
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r5, #0
	movs r2, #3
	bl sub_803AD84
	b _08017E66
	.align 2, 0
_08017E58: .4byte gUnknown_030012D8
_08017E5C:
	adds r0, r4, #0
	adds r1, r5, #0
	movs r2, #1
	bl sub_8017F14
_08017E66:
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r5, #0
	movs r2, #0
	bl sub_803AD84
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #1
	bl sub_803AD80
	b _08017EC4
_08017E8C:
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r5, #0
	movs r2, #2
	bl sub_803AD84
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0
	bl sub_803AD80
	ldr r1, [r4, #0xc]
	adds r1, #0x58
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r5, #0
	movs r2, #0
	bl sub_803AD84
_08017EC4:
	add sp, #0x1c
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	thumb_func_start sub_8017ECC
sub_8017ECC: @ 0x08017ECC
	adds r3, r1, #0
	ldr r0, [r0, #4]
	ldr r0, [r0, #0]
	lsls r2, r2, #3
	adds r2, r2, r0
	ldr r1, [r2, #4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _08017F00 @ =gStaticData_0816C2D8
	adds r2, r0, r1
	adds r0, r3, #0
	adds r0, #0x28
	ldrb r0, [r0, #0]
	lsls r0, r0, #26
	cmp r0, #0
	bge _08017F04
	ldr r0, [r2, #0]
	negs r0, r0
	ldr r1, [r2, #8]
	negs r1, r1
	ldr r2, [r2, #4]
	str r0, [r3, #0x54]
	str r2, [r3, #0x58]
	str r1, [r3, #0x5c]
	b _08017F10
	.align 2, 0
_08017F00: .4byte gStaticData_0816C2D8
_08017F04:
	ldr r0, [r2, #0]
	ldr r1, [r2, #4]
	ldr r2, [r2, #8]
	str r0, [r3, #0x54]
	str r1, [r3, #0x58]
	str r2, [r3, #0x5c]
_08017F10:
	bx lr

	thumb_func_start sub_8017F14
sub_8017F14: @ 0x08017F14
	adds r3, r1, #0
	ldr r0, [r0, #4]
	ldr r0, [r0]
	lsls r2, r2, #3
	adds r2, r2, r0
	ldr r1, [r2]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _08017F48 @ =gStaticData_0816C2D8
	adds r2, r0, r1
	adds r0, r3, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _08017F4C
	ldr r0, [r2]
	rsbs r0, r0, #0
	ldr r1, [r2, #8]
	rsbs r1, r1, #0
	ldr r2, [r2, #4]
	str r0, [r3, #0x48]
	str r2, [r3, #0x4c]
	str r1, [r3, #0x50]
	b _08017F58
	.align 2, 0
_08017F48: .4byte gStaticData_0816C2D8
_08017F4C:
	ldr r0, [r2]
	ldr r1, [r2, #4]
	ldr r2, [r2, #8]
	str r0, [r3, #0x48]
	str r1, [r3, #0x4c]
	str r2, [r3, #0x50]
_08017F58:
	bx lr
	.align 2, 0

	thumb_func_start sub_8017F5C
sub_8017F5C: @ 0x08017F5C
	push {lr}
	ldr r3, [r0, #4]
	ldr r3, [r3]
	lsls r2, r2, #3
	adds r2, r2, r3
	ldr r3, [r2, #4]
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r2, r2, #2
	ldr r3, _08017F7C @ =gStaticData_0816C2D8
	adds r2, r2, r3
	bl sub_800B6D0
	pop {r0}
	bx r0
	.align 2, 0
_08017F7C: .4byte gStaticData_0816C2D8

	thumb_func_start sub_8017F80
sub_8017F80: @ 0x08017F80
	push {lr}
	ldr r3, [r0, #4]
	ldr r3, [r3]
	lsls r2, r2, #3
	adds r2, r2, r3
	ldr r3, [r2]
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r2, r2, #2
	ldr r3, _08017FA0 @ =gStaticData_0816C2D8
	adds r2, r2, r3
	bl sub_800B7B0
	pop {r0}
	bx r0
	.align 2, 0
_08017FA0: .4byte gStaticData_0816C2D8

	thumb_func_start sub_8017FA4
sub_8017FA4: @ 0x08017FA4
	push {r4, lr}
	adds r4, r0, #0
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #1
	bl sub_803AD80
	adds r1, r4, #0
	adds r1, #0x20
	movs r0, #0
	strb r0, [r1]
	subs r0, #1
	str r0, [r4, #0x1c]
	ldr r0, _08017FD0 @ =gStaticData_0816C2D0
	str r0, [r4, #4]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08017FD0: .4byte gStaticData_0816C2D0

	thumb_func_start sub_8017FD4
sub_8017FD4: @ 0x08017FD4
	push {lr}
	ldr r2, _08017FE4 @ =gStaticData_087E43C4
	str r2, [r0, #0xc]
	bl sub_8017A78
	pop {r0}
	bx r0
	.align 2, 0
_08017FE4: .4byte gStaticData_087E43C4

	thumb_func_start sub_8017FE8
sub_8017FE8: @ 0x08017FE8
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8017A8C
	ldr r0, _08018004 @ =gStaticData_087E43C4
	str r0, [r4, #0xc]
	adds r0, r4, #0
	bl sub_8017FA4
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08018004: .4byte gStaticData_087E43C4

	thumb_func_start sub_8018008
sub_8018008: @ 0x08018008
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #0x20
	adds r7, r0, #0
	mov r8, r1
	ldr r2, [r7, #0x24]
	movs r5, #1
	rsbs r5, r5, #0
	cmp r2, r5
	beq _08018070
	ldr r0, _080180B8 @ =gUnknown_030012EC
	ldr r0, [r0]
	ldr r1, [r0, #0xc]
	lsls r0, r2, #2
	adds r0, r0, r1
	ldr r4, [r0]
	ldr r2, [r4, #0x44]
	cmp r2, #0
	beq _08018044
	ldr r1, [r2, #0xc]
	adds r1, #0x48
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_08018044:
	movs r0, #0x10
	bl sub_8026EDC
	bl sub_801886C
	str r0, [r4, #0x44]
	ldr r2, [r0, #0xc]
	movs r6, #0x18
	ldrsh r1, [r2, r6]
	adds r0, r0, r1
	ldr r2, [r2, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	str r5, [r7, #0x24]
	ldr r0, _080180BC @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x39
	bl PlaySfx
_08018070:
	ldr r0, [r7, #8]
	cmp r0, #8
	bne _080180C4
	ldr r5, _080180C0 @ =gUnknown_030012D8
	ldr r1, [r5]
	mov r0, sp
	bl sub_8007C30
	add r4, sp, #0x10
	adds r0, r4, #0
	mov r1, r8
	bl sub_8007CF8
	ldr r0, [sp, #8]
	cmp r0, #0
	beq _08018130
	ldr r0, [sp, #0x18]
	cmp r0, #0
	beq _08018130
	adds r0, r4, #0
	mov r1, sp
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08018130
	ldr r0, [r5]
	ldrb r0, [r0, #0xa]
	cmp r0, #0x13
	bne _08018130
	adds r0, r7, #0
	mov r1, r8
	movs r2, #9
	bl sub_8018400
	b _08018130
	.align 2, 0
_080180B8: .4byte gUnknown_030012EC
_080180BC: .4byte gUnknown_030012BC
_080180C0: .4byte gUnknown_030012D8
_080180C4:
	ldr r5, _08018144 @ =gUnknown_030012D8
	ldr r1, [r5]
	movs r2, #0x82
	lsls r2, r2, #1
	adds r0, r1, r2
	ldrb r0, [r0]
	cmp r0, #0
	bne _08018130
	mov r0, sp
	bl sub_8007CF8
	ldr r0, [sp, #8]
	add r4, sp, #0x10
	cmp r0, #0
	bne _080180F6
	ldr r1, [r5]
	adds r0, r4, #0
	bl sub_8007C30
	mov r1, sp
	adds r0, r4, #0
	ldm r0!, {r2, r3, r6}
	stm r1!, {r2, r3, r6}
	ldr r0, [r0]
	str r0, [r1]
_080180F6:
	adds r0, r4, #0
	mov r1, r8
	bl sub_8007C30
	ldr r0, [sp, #0x18]
	cmp r0, #0
	beq _08018130
	ldr r0, [sp, #8]
	cmp r0, #0
	beq _08018130
	adds r0, r4, #0
	mov r1, sp
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08018130
	ldr r0, [r5]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #0
	bl sub_803AD88
_08018130:
	ldr r6, [r7, #8]
	mov sb, r6
	cmp r6, #0xf
	bls _0801813A
	b _080183EE
_0801813A:
	lsls r0, r6, #2
	ldr r1, _08018148 @ =_0801814C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08018144: .4byte gUnknown_030012D8
_08018148: .4byte _0801814C
_0801814C: @ jump table
	.4byte _0801818C @ case 0
	.4byte _080181A6 @ case 1
	.4byte _080181A6 @ case 2
	.4byte _0801826C @ case 3
	.4byte _08018388 @ case 4
	.4byte _08018374 @ case 5
	.4byte _0801831C @ case 6
	.4byte _0801826C @ case 7
	.4byte _08018340 @ case 8
	.4byte _080183BC @ case 9
	.4byte _0801826C @ case 10
	.4byte _080181A6 @ case 11
	.4byte _0801839E @ case 12
	.4byte _080182EC @ case 13
	.4byte _0801826C @ case 14
	.4byte _080181A6 @ case 15
_0801818C:
	movs r0, #5
	rsbs r0, r0, #0
	mov r1, r8
	ldrb r1, [r1, #0xc]
	ands r0, r1
	mov r2, r8
	strb r0, [r2, #0xc]
	movs r0, #1
	movs r1, #0
	strb r0, [r2, #0xa]
	str r0, [r7, #0x28]
	str r1, [r7, #0x2c]
	b _080182C2
_080181A6:
	ldr r0, [r7, #0x38]
	subs r6, r0, #1
	str r6, [r7, #0x38]
	ldr r0, [r7, #0x44]
	muls r0, r6, r0
	ldr r5, [r7, #0x3c]
	adds r1, r5, #0
	bl sub_803ADB4
	adds r4, r0, #0
	ldr r0, [r7, #0x30]
	adds r4, r4, r0
	lsls r0, r6, #8
	adds r1, r5, #0
	bl sub_803ADB4
	movs r5, #0x80
	lsls r5, r5, #1
	subs r0, r5, r0
	ldr r1, [r7, #0x48]
	lsls r0, r0, #1
	adds r0, r0, r1
	movs r3, #0
	ldrsh r0, [r0, r3]
	subs r0, r5, r0
	ldr r1, [r7, #0x40]
	muls r0, r1, r0
	asrs r0, r0, #8
	ldr r1, [r7, #0x34]
	adds r0, r0, r1
	mov r1, r8
	str r4, [r1]
	str r0, [r1, #4]
	cmp r6, #0
	beq _080181EE
	b _080183EE
_080181EE:
	ldr r4, _08018220 @ =gUnknown_030012BC
	ldr r0, [r4]
	movs r1, #0x2a
	adds r2, r5, #0
	bl PlaySfx
	ldr r0, [r7, #8]
	cmp r0, #0xf
	bne _08018228
	ldr r0, _08018224 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231B4
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08018212
	bl sub_80241A4
_08018212:
	adds r0, r7, #0
	mov r1, r8
	movs r2, #0x10
	bl sub_8018400
	b _080183EE
	.align 2, 0
_08018220: .4byte gUnknown_030012BC
_08018224: .4byte gUnknown_030012C0
_08018228:
	cmp r0, #1
	bne _08018238
	adds r0, r7, #0
	mov r1, r8
	movs r2, #6
	bl sub_8018400
	b _080183EE
_08018238:
	cmp r0, #0xb
	bne _08018256
	str r6, [r7, #0x2c]
	ldr r1, [r7, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r7, r0
	ldr r3, [r1, #4]
	mov r1, r8
	movs r2, #3
	bl sub_803AD84
	movs r0, #0xc
	b _08018366
_08018256:
	ldr r0, [r4]
	movs r1, #0x3d
	adds r2, r5, #0
	bl PlaySfx
	adds r0, r7, #0
	mov r1, r8
	movs r2, #8
	bl sub_8018400
	b _080183EE
_0801826C:
	ldr r4, [r7, #0x38]
	subs r4, #1
	str r4, [r7, #0x38]
	ldr r0, [r7, #0x44]
	muls r0, r4, r0
	ldr r6, [r7, #0x3c]
	adds r1, r6, #0
	bl sub_803ADB4
	adds r5, r0, #0
	ldr r0, [r7, #0x30]
	adds r5, r5, r0
	lsls r0, r4, #8
	adds r1, r6, #0
	bl sub_803ADB4
	ldr r1, [r7, #0x48]
	lsls r0, r0, #1
	adds r0, r0, r1
	movs r3, #0
	ldrsh r1, [r0, r3]
	ldr r0, [r7, #0x40]
	muls r0, r1, r0
	asrs r0, r0, #8
	ldr r1, [r7, #0x34]
	adds r0, r0, r1
	mov r6, r8
	str r5, [r6]
	str r0, [r6, #4]
	cmp r4, #0
	beq _080182AC
	b _080183EE
_080182AC:
	mov r0, sb
	cmp r0, #0xe
	bne _080182BE
	adds r0, r7, #0
	mov r1, r8
	movs r2, #0xf
	bl sub_8018400
	b _080183EE
_080182BE:
	cmp r0, #3
	bne _080182CE
_080182C2:
	adds r0, r7, #0
	mov r1, r8
	movs r2, #1
	bl sub_8018400
	b _080183EE
_080182CE:
	mov r0, sb
	cmp r0, #7
	bne _080182E0
	adds r0, r7, #0
	mov r1, r8
	movs r2, #2
	bl sub_8018400
	b _080183EE
_080182E0:
	adds r0, r7, #0
	mov r1, r8
	movs r2, #0xd
	bl sub_8018400
	b _080183EE
_080182EC:
	ldr r0, [r7, #0x1c]
	cmp r0, #0
	bne _08018314
	ldr r0, [r7, #0x20]
	subs r2, r0, #1
	str r2, [r7, #0x20]
	cmp r2, #0
	bne _08018308
	adds r0, r7, #0
	mov r1, r8
	movs r2, #0xb
	bl sub_8018400
	b _08018314
_08018308:
	movs r0, #0x46
	str r0, [r7, #0x1c]
	adds r0, r7, #0
	mov r1, r8
	bl sub_80186F0
_08018314:
	ldr r0, [r7, #0x1c]
	subs r0, #1
	str r0, [r7, #0x1c]
	b _080183EE
_0801831C:
	mov r0, r8
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _080183EE
	ldr r0, [r7, #0x2c]
	adds r0, #1
	str r0, [r7, #0x2c]
	cmp r0, #3
	ble _080183AA
	movs r0, #0
	str r0, [r7, #0x2c]
	adds r0, r7, #0
	mov r1, r8
	movs r2, #7
	bl sub_8018400
	b _080183EE
_08018340:
	ldr r0, [r7, #0x20]
	cmp r0, #0
	beq _0801834C
	subs r0, #1
	str r0, [r7, #0x20]
	b _080183EE
_0801834C:
	subs r0, #1
	str r0, [r7, #0x20]
	ldr r1, [r7, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r7, r0
	ldr r3, [r1, #4]
	mov r1, r8
	movs r2, #0
	bl sub_803AD84
	movs r0, #3
_08018366:
	str r0, [r7, #0x1c]
	adds r0, r7, #0
	mov r1, r8
	movs r2, #5
	bl sub_8018400
	b _080183EE
_08018374:
	mov r0, r8
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08018388
	ldr r2, [r7, #0x1c]
	adds r0, r7, #0
	mov r1, r8
	bl sub_8018400
_08018388:
	ldr r0, [r7, #0x20]
	subs r0, #1
	str r0, [r7, #0x20]
	cmp r0, #0
	bne _080183EE
	ldr r2, [r7, #0x1c]
	adds r0, r7, #0
	mov r1, r8
	bl sub_8018400
	b _080183EE
_0801839E:
	ldr r0, _080183B8 @ =gStaticData_0816C308
	ldr r1, [r7, #0x10]
	subs r1, #1
	adds r1, r1, r0
	ldrb r0, [r1]
	str r0, [r7, #0x24]
_080183AA:
	adds r0, r7, #0
	mov r1, r8
	movs r2, #3
	bl sub_8018400
	b _080183EE
	.align 2, 0
_080183B8: .4byte gStaticData_0816C308
_080183BC:
	ldr r0, [r7, #0x10]
	cmp r0, #2
	ble _080183CC
	adds r0, r7, #0
	mov r1, r8
	movs r2, #0xe
	bl sub_8018400
_080183CC:
	mov r0, r8
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _080183EE
	adds r0, r7, #0
	mov r1, r8
	movs r2, #0xa
	bl sub_8018400
	ldr r0, _080183FC @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xd
	bl PlaySfx
_080183EE:
	add sp, #0x20
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080183FC: .4byte gUnknown_030012BC

	thumb_func_start sub_8018400
sub_8018400: @ 0x08018400
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r7, r2, #0
	subs r0, r7, #1
	cmp r0, #0xe
	bls _08018410
	b _08018642
_08018410:
	lsls r0, r0, #2
	ldr r1, _0801841C @ =_08018420
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801841C: .4byte _08018420
_08018420: @ jump table
	.4byte _08018470 @ case 0
	.4byte _08018470 @ case 1
	.4byte _080184F0 @ case 2
	.4byte _08018642 @ case 3
	.4byte _08018642 @ case 4
	.4byte _08018642 @ case 5
	.4byte _080184F0 @ case 6
	.4byte _08018568 @ case 7
	.4byte _08018582 @ case 8
	.4byte _08018550 @ case 9
	.4byte _08018464 @ case 10
	.4byte _08018642 @ case 11
	.4byte _0801845C @ case 12
	.4byte _080185DC @ case 13
	.4byte _0801861C @ case 14
_0801845C:
	movs r0, #0
	str r0, [r4, #0x1c]
	movs r0, #4
	str r0, [r4, #0x20]
_08018464:
	ldr r0, _0801848C @ =gStaticData_0816C308
	ldr r1, [r4, #0x10]
	subs r1, #1
	adds r1, r1, r0
	ldrb r0, [r1]
	str r0, [r4, #0x28]
_08018470:
	cmp r7, #1
	bne _08018490
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r5, #0
	movs r2, #2
	bl sub_803AD84
	b _080184A4
	.align 2, 0
_0801848C: .4byte gStaticData_0816C308
_08018490:
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r6, #0
	ldrsh r0, [r1, r6]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r5, #0
	movs r2, #1
	bl sub_803AD84
_080184A4:
	movs r3, #4
	ldr r0, [r5, #0x20]
	adds r2, r5, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r6, [r2]
	lsls r0, r6, #3
	subs r0, r0, r6
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _080184C0
	subs r3, r0, #1
_080184C0:
	str r3, [r5, #0x30]
	ldr r0, _080184E8 @ =gUnknown_030012EC
	ldr r1, [r0]
	ldr r0, [r4, #0x28]
	ldr r1, [r1, #0xc]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r2, [r0]
	ldr r1, [r2]
	str r1, [r4, #0x30]
	cmp r7, #0xb
	beq _080184DC
	cmp r7, #0xd
	bne _080184DE
_080184DC:
	str r1, [r5]
_080184DE:
	ldr r0, [r2, #4]
	ldr r1, _080184EC @ =0xFFFFDC00
	adds r0, r0, r1
	str r0, [r4, #0x34]
	b _08018558
	.align 2, 0
_080184E8: .4byte gUnknown_030012EC
_080184EC: .4byte 0xFFFFDC00
_080184F0:
	adds r0, r4, #0
	bl sub_801865C
	str r0, [r4, #0x28]
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r5, #0
	movs r2, #4
	bl sub_803AD84
	ldr r0, _08018538 @ =gUnknown_030012EC
	ldr r1, [r0]
	ldr r0, [r4, #0x28]
	ldr r1, [r1, #0xc]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r1, [r0]
	ldr r0, [r1]
	ldr r3, [r1, #4]
	ldr r6, _0801853C @ =0xFFFFDC00
	adds r2, r3, r6
	ldr r1, [r5]
	adds r0, r0, r1
	asrs r0, r0, #1
	str r0, [r4, #0x30]
	ldr r0, [r5, #4]
	cmp r2, r0
	blt _08018544
	ldr r1, _08018540 @ =0xFFFFA700
	adds r2, r0, r1
	b _08018548
	.align 2, 0
_08018538: .4byte gUnknown_030012EC
_0801853C: .4byte 0xFFFFDC00
_08018540: .4byte 0xFFFFA700
_08018544:
	ldr r6, _0801854C @ =0xFFFF8300
	adds r2, r3, r6
_08018548:
	str r2, [r4, #0x34]
	b _08018558
	.align 2, 0
_0801854C: .4byte 0xFFFF8300
_08018550:
	ldr r0, _08018564 @ =0xFFFFD000
	str r0, [r4, #0x34]
	ldr r0, [r5]
	str r0, [r4, #0x30]
_08018558:
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_8018978
	b _08018642
	.align 2, 0
_08018564: .4byte 0xFFFFD000
_08018568:
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r5, #0
	movs r2, #6
	bl sub_803AD84
	movs r0, #0xb4
	str r0, [r4, #0x20]
	b _08018642
_08018582:
	ldr r0, _080185D4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x15
	bl PlaySfx
	ldr r0, [r4, #0x10]
	adds r0, #1
	str r0, [r4, #0x10]
	cmp r0, #2
	ble _080185B4
	ldr r1, [r5]
	ldr r0, [r5, #4]
	ldr r6, _080185D8 @ =0xFFFF9C00
	adds r0, r0, r6
	str r0, [r4, #0x34]
	movs r0, #0xc8
	lsls r0, r0, #7
	adds r1, r1, r0
	str r1, [r4, #0x30]
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_8018978
_080185B4:
	adds r0, r4, #0
	adds r1, r5, #0
	bl nullsub_19
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r5, #0
	movs r2, #7
	bl sub_803AD84
	b _08018642
	.align 2, 0
_080185D4: .4byte gUnknown_030012BC
_080185D8: .4byte 0xFFFF9C00
_080185DC:
	ldr r0, _0801860C @ =gUnknown_030012EC
	ldr r0, [r0]
	ldr r0, [r0, #0xc]
	ldr r0, [r0, #8]
	ldr r5, [r0]
	ldr r0, [r0, #4]
	ldr r1, _08018610 @ =0xFFFFE800
	adds r6, r0, r1
	ldr r0, _08018614 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231B4
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08018642
	ldr r0, _08018618 @ =0x0000FFFF
	lsls r1, r5, #8
	lsrs r1, r1, #0x10
	lsls r2, r6, #8
	lsrs r2, r2, #0x10
	movs r3, #0
	bl sub_8021DFC
	b _08018642
	.align 2, 0
_0801860C: .4byte gUnknown_030012EC
_08018610: .4byte 0xFFFFE800
_08018614: .4byte gUnknown_030012C0
_08018618: .4byte 0x0000FFFF
_0801861C:
	ldr r1, [r5]
	str r1, [r4, #0x30]
	ldr r0, _08018658 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x14]
	lsls r0, r0, #8
	movs r2, #0x80
	lsls r2, r2, #7
	adds r0, r0, r2
	str r0, [r4, #0x34]
	movs r6, #0xc8
	lsls r6, r6, #7
	adds r1, r1, r6
	str r1, [r4, #0x30]
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_8018978
_08018642:
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	adds r1, r7, #0
	bl sub_803AD80
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08018658: .4byte gUnknown_03001308

	thumb_func_start sub_801865C
sub_801865C: @ 0x0801865C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	mov sb, r0
	movs r0, #0
	mov ip, r0
	ldr r5, _080186E0 @ =0x00FFFFFF
	movs r4, #0
	ldr r0, _080186E4 @ =gUnknown_030012EC
	ldr r1, [r0]
	ldr r2, [r1, #4]
	ldr r3, _080186E8 @ =gStaticData_0816C30B
	mov sl, r3
	cmp ip, r2
	bge _080186B6
	ldr r0, _080186EC @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r3, [r0]
	mov r8, r3
	ldr r7, [r0, #4]
	adds r6, r2, #0
	ldr r3, [r1, #0xc]
_0801868C:
	ldr r0, [r3]
	ldr r1, [r0]
	ldr r0, [r0, #4]
	mov r2, r8
	subs r1, r1, r2
	asrs r2, r1, #0x1f
	eors r1, r2
	subs r1, r1, r2
	subs r0, r0, r7
	asrs r2, r0, #0x1f
	eors r0, r2
	subs r0, r0, r2
	adds r1, r1, r0
	cmp r1, r5
	bge _080186AE
	adds r5, r1, #0
	mov ip, r4
_080186AE:
	adds r3, #4
	adds r4, #1
	cmp r4, r6
	blt _0801868C
_080186B6:
	mov r3, sb
	ldr r0, [r3, #0x28]
	lsls r1, r0, #2
	adds r1, r1, r0
	add r1, ip
	ldr r2, [r3, #0x10]
	lsls r0, r2, #1
	adds r0, r0, r2
	lsls r0, r0, #3
	adds r0, r0, r2
	adds r1, r1, r0
	add r1, sl
	ldrb r0, [r1]
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080186E0: .4byte 0x00FFFFFF
_080186E4: .4byte gUnknown_030012EC
_080186E8: .4byte gStaticData_0816C30B
_080186EC: .4byte gUnknown_030012D8

	thumb_func_start sub_80186F0
sub_80186F0: @ 0x080186F0
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	mov sb, r1
	adds r6, r2, #0
	ldr r0, _080187E4 @ =0x0000FFFF
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _080187E8 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xa5
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x20]
	movs r0, #5
	adds r1, r4, #0
	adds r1, #0x2d
	movs r2, #0
	mov r8, r2
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	movs r0, #0x10
	bl sub_8026EDC
	bl sub_80188D0
	adds r5, r0, #0
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	str r5, [r4, #0x44]
	ldr r1, [r5, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r5, r5, r0
	ldr r2, [r1, #0x1c]
	adds r0, r5, #0
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #0x10
	ldrb r3, [r4, #0xc]
	orrs r0, r3
	strb r0, [r4, #0xc]
	ldr r0, _080187EC @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	movs r0, #0x80
	mov r1, r8
	str r1, [r4, #0x64]
	str r1, [r4, #0x54]
	str r0, [r4, #0x58]
	str r0, [r4, #0x5c]
	mov r2, sb
	ldr r5, [r2]
	ldr r0, _080187F0 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0]
	subs r0, r0, r5
	subs r6, #1
	muls r0, r6, r0
	movs r1, #3
	bl sub_803ADB4
	adds r5, r5, r0
	ldr r0, _080187F4 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #4]
	lsls r0, r0, #8
	str r5, [r4]
	str r0, [r4, #4]
	movs r0, #1
	strb r0, [r4, #0xa]
	subs r0, #6
	ldrb r3, [r4, #0xc]
	ands r0, r3
	movs r1, #0x41
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _080187F8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x13
	bl PlaySfx
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_080187E4: .4byte 0x0000FFFF
_080187E8: .4byte gUnknown_030012D0
_080187EC: .4byte gUnknown_030012F0
_080187F0: .4byte gUnknown_030012D8
_080187F4: .4byte gUnknown_03001308
_080187F8: .4byte gUnknown_030012BC

	thumb_func_start sub_80187FC
sub_80187FC: @ 0x080187FC
	push {r4, lr}
	adds r3, r0, #0
	adds r2, r1, #0
	ldr r0, [r3, #8]
	cmp r0, #1
	beq _0801882A
	cmp r0, #1
	bgt _0801884C
	cmp r0, #0
	bne _0801884C
	movs r0, #1
	str r0, [r3, #8]
	ldr r1, [r3, #0xc]
	adds r1, #0x50
	movs r4, #0
	ldrsh r0, [r1, r4]
	adds r0, r3, r0
	ldr r3, [r1, #4]
	adds r1, r2, #0
	movs r2, #8
	bl sub_803AD84
	b _0801884C
_0801882A:
	ldr r1, [r2, #4]
	movs r0, #0x80
	lsls r0, r0, #3
	adds r1, r1, r0
	str r1, [r2, #4]
	ldr r0, _08018854 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x14]
	lsls r0, r0, #8
	movs r2, #0x80
	lsls r2, r2, #6
	adds r0, r0, r2
	cmp r1, r0
	blt _0801884C
	movs r0, #2
	str r0, [r3, #8]
_0801884C:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08018854: .4byte gUnknown_03001308

	thumb_func_start sub_8018858
sub_8018858: @ 0x08018858
	push {lr}
	ldr r2, _08018868 @ =gStaticData_087E442C
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_08018868: .4byte gStaticData_087E442C

	thumb_func_start sub_801886C
sub_801886C: @ 0x0801886C
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800B8C8
	ldr r0, _08018880 @ =gStaticData_087E442C
	str r0, [r4, #0xc]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08018880: .4byte gStaticData_087E442C

	thumb_func_start sub_8018884
sub_8018884: @ 0x08018884
	push {r4, lr}
	adds r0, r1, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _080188C2
	movs r0, #1
	ldrb r2, [r1, #0xc]
	orrs r0, r2
	strb r0, [r1, #0xc]
	ldr r0, _080188C8 @ =0x0000FFFF
	ldrh r4, [r1, #8]
	cmp r4, r0
	beq _080188C2
	ldrh r3, [r1, #8]
	ldr r0, _080188CC @ =gUnknown_030012B4
	ldr r2, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r1, r0, #2
	movs r4, #0x84
	lsls r4, r4, #1
	adds r2, r2, r4
	adds r2, r2, r1
	lsls r0, r0, #5
	subs r0, r3, r0
	movs r1, #1
	lsls r1, r0
	ldr r0, [r2]
	orrs r0, r1
	str r0, [r2]
_080188C2:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080188C8: .4byte 0x0000FFFF
_080188CC: .4byte gUnknown_030012B4

	thumb_func_start sub_80188D0
sub_80188D0: @ 0x080188D0
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800B8C8
	ldr r0, _080188E4 @ =gStaticData_087E4494
	str r0, [r4, #0xc]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_080188E4: .4byte gStaticData_087E4494

	thumb_func_start sub_80188E8
sub_80188E8: @ 0x080188E8
	push {lr}
	ldr r2, _080188F8 @ =gStaticData_087E4494
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_080188F8: .4byte gStaticData_087E4494

	thumb_func_start sub_80188FC
sub_80188FC: @ 0x080188FC
	push {r4, lr}
	adds r0, r1, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801893A
	movs r0, #1
	ldrb r2, [r1, #0xc]
	orrs r0, r2
	strb r0, [r1, #0xc]
	ldr r0, _08018940 @ =0x0000FFFF
	ldrh r4, [r1, #8]
	cmp r4, r0
	beq _0801893A
	ldrh r3, [r1, #8]
	ldr r0, _08018944 @ =gUnknown_030012B4
	ldr r2, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r1, r0, #2
	movs r4, #0x84
	lsls r4, r4, #1
	adds r2, r2, r4
	adds r2, r2, r1
	lsls r0, r0, #5
	subs r0, r3, r0
	movs r1, #1
	lsls r1, r0
	ldr r0, [r2]
	orrs r0, r1
	str r0, [r2]
_0801893A:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08018940: .4byte 0x0000FFFF
_08018944: .4byte gUnknown_030012B4

	thumb_func_start sub_8018948
sub_8018948: @ 0x08018948
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800B8C8
	ldr r0, _0801895C @ =gStaticData_087E44FC
	str r0, [r4, #0xc]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801895C: .4byte gStaticData_087E44FC

	thumb_func_start sub_8018960
sub_8018960: @ 0x08018960
	push {lr}
	ldr r2, _08018970 @ =gStaticData_087E44FC
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_08018970: .4byte gStaticData_087E44FC

	thumb_func_start nullsub_19
nullsub_19: @ 0x08018974
	bx lr
	.align 2, 0

	thumb_func_start sub_8018978
sub_8018978: @ 0x08018978
	adds r3, r0, #0
	mov ip, r1
	ldr r1, [r1]
	ldr r0, [r3, #0x30]
	cmp r0, r1
	bgt _08018998
	mov r0, ip
	adds r0, #0x28
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r2, [r0]
	ands r1, r2
	movs r2, #0x10
	orrs r1, r2
	strb r1, [r0]
	b _080189A6
_08018998:
	mov r1, ip
	adds r1, #0x28
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r2, [r1]
	ands r0, r2
	strb r0, [r1]
_080189A6:
	movs r0, #0x1a
	str r0, [r3, #0x38]
	str r0, [r3, #0x3c]
	mov r1, ip
	ldr r0, [r1, #4]
	ldr r1, [r3, #0x34]
	subs r0, r0, r1
	str r0, [r3, #0x40]
	mov r2, ip
	ldr r0, [r2]
	ldr r1, [r3, #0x30]
	subs r0, r0, r1
	str r0, [r3, #0x44]
	bx lr
	.align 2, 0

	thumb_func_start sub_80189C4
sub_80189C4: @ 0x080189C4
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, _080189E8 @ =gStaticData_087E4564
	str r0, [r4, #0xc]
	ldr r0, [r4, #0x48]
	cmp r0, #0
	beq _080189D8
	bl sub_8026EB4
_080189D8:
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_8017A78
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080189E8: .4byte gStaticData_087E4564

	thumb_func_start sub_80189EC
sub_80189EC: @ 0x080189EC
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8017A8C
	ldr r0, _08018A28 @ =gStaticData_087E4564
	str r0, [r4, #0xc]
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r4, #0x24]
	ldr r0, _08018A2C @ =0x00000202
	bl sub_8026EC0
	str r0, [r4, #0x48]
	movs r2, #0
	movs r3, #0x80
	lsls r3, r3, #1
	adds r1, r0, #0
_08018A0E:
	adds r0, r2, #0
	muls r0, r2, r0
	asrs r0, r0, #8
	strh r0, [r1]
	adds r1, #2
	adds r2, #1
	cmp r2, r3
	ble _08018A0E
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08018A28: .4byte gStaticData_087E4564
_08018A2C: .4byte 0x00000202

	thumb_func_start sub_8018A30
sub_8018A30: @ 0x08018A30
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	ldr r0, [r5, #8]
	cmp r0, #4
	bls _08018A3E
	b _08018BCC
_08018A3E:
	lsls r0, r0, #2
	ldr r1, _08018A48 @ =_08018A4C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08018A48: .4byte _08018A4C
_08018A4C: @ jump table
	.4byte _08018A60 @ case 0
	.4byte _08018A7C @ case 1
	.4byte _08018B6C @ case 2
	.4byte _08018B8E @ case 3
	.4byte _08018BCC @ case 4
_08018A60:
	adds r0, r5, #0
	adds r1, r6, #0
	bl sub_8018BDC
	adds r0, r5, #0
	adds r1, r6, #0
	bl sub_8018CB0
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r1, [r6, #0xc]
	ands r0, r1
	strb r0, [r6, #0xc]
	b _08018B82
_08018A7C:
	ldr r0, [r5, #0x20]
	ldr r1, [r0, #4]
	movs r0, #0xa0
	lsls r0, r0, #7
	cmp r1, r0
	bgt _08018A8E
	ldr r4, [r5, #0x1c]
	movs r0, #5
	b _08018A9A
_08018A8E:
	movs r0, #0xf0
	lsls r0, r0, #7
	cmp r1, r0
	bgt _08018AB6
	ldr r4, [r5, #0x1c]
	movs r0, #4
_08018A9A:
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
	b _08018AD4
_08018AB6:
	ldr r4, [r5, #0x1c]
	movs r0, #3
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
_08018AD4:
	ldr r0, [r5, #0x20]
	ldr r1, [r0]
	ldr r0, [r6]
	subs r4, r1, r0
	mvns r2, r4
	adds r3, r6, #0
	adds r3, #0x28
	lsrs r2, r2, #0x1f
	lsls r2, r2, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	adds r0, r1, #0
	ldrb r7, [r3]
	ands r0, r7
	orrs r0, r2
	strb r0, [r3]
	ldr r0, [r5, #0x1c]
	adds r0, #0x28
	ldrb r3, [r0]
	ands r1, r3
	orrs r1, r2
	strb r1, [r0]
	ldr r0, _08018B68 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #8
	asrs r2, r4, #0x1f
	eors r4, r2
	subs r2, r4, r2
	lsls r0, r2, #1
	adds r0, r0, r2
	lsls r0, r0, #2
	bl sub_8037E54
	adds r4, r0, #0
	cmp r4, #5
	ble _08018B22
	movs r4, #5
_08018B22:
	movs r0, #5
	subs r4, r0, r4
	adds r3, r4, #0
	ldr r0, [r6, #0x20]
	adds r2, r6, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r7, [r2]
	lsls r0, r7, #3
	adds r2, r7, #0
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _08018B44
	subs r3, r0, #1
_08018B44:
	str r3, [r6, #0x30]
	ldr r5, [r5, #0x1c]
	adds r2, r4, #0
	ldr r0, [r5, #0x20]
	adds r3, r5, #0
	adds r3, #0x2d
	ldr r1, [r0]
	ldrb r4, [r3]
	lsls r0, r4, #3
	subs r0, r0, r4
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r2, r0
	blt _08018B64
	subs r2, r0, #1
_08018B64:
	str r2, [r5, #0x30]
	b _08018BCC
	.align 2, 0
_08018B68: .4byte gUnknown_03001308
_08018B6C:
	ldr r0, [r5, #0x10]
	adds r0, #1
	str r0, [r5, #0x10]
	cmp r0, #2
	ble _08018B82
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #3
	bl sub_8019770
	b _08018BCC
_08018B82:
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #1
	bl sub_8019770
	b _08018BCC
_08018B8E:
	ldr r1, [r5, #0x1c]
	ldr r0, [r1, #4]
	adds r0, #0x80
	str r0, [r1, #4]
	ldr r1, [r6, #4]
	adds r1, #0x80
	str r1, [r6, #4]
	ldr r0, _08018BD4 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x14]
	lsls r0, r0, #8
	movs r7, #0x80
	lsls r7, r7, #7
	adds r0, r0, r7
	cmp r1, r0
	blt _08018BCC
	ldr r0, _08018BD8 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231C4
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08018BC2
	bl sub_80241A4
_08018BC2:
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #4
	bl sub_8019770
_08018BCC:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08018BD4: .4byte gUnknown_03001308
_08018BD8: .4byte gUnknown_030012C0

	thumb_func_start sub_8018BDC
sub_8018BDC: @ 0x08018BDC
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	mov r8, r0
	adds r6, r1, #0
	ldr r0, _08018CA4 @ =0x0000FFFF
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _08018CA8 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x9f
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x20]
	movs r0, #3
	adds r1, r4, #0
	adds r1, #0x2d
	movs r5, #0
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	adds r0, #0x2c
	strb r5, [r0]
	movs r0, #0x10
	bl sub_8026EDC
	bl sub_8019758
	adds r5, r0, #0
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	str r5, [r4, #0x44]
	ldr r1, [r5, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r5, r5, r0
	ldr r2, [r1, #0x1c]
	adds r0, r5, #0
	adds r1, r4, #0
	bl sub_803AD80
	ldr r0, [r6]
	ldr r1, [r6, #4]
	str r0, [r4]
	str r1, [r4, #4]
	adds r6, #0x28
	adds r2, r4, #0
	adds r2, #0x28
	movs r1, #0x10
	ldrb r6, [r6]
	ands r1, r6
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r3, [r2]
	ands r0, r3
	orrs r0, r1
	strb r0, [r2]
	movs r0, #0x10
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _08018CAC @ =gUnknown_030012F4
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	mov r2, r8
	str r4, [r2, #0x1c]
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08018CA4: .4byte 0x0000FFFF
_08018CA8: .4byte gUnknown_030012D0
_08018CAC: .4byte gUnknown_030012F4

	thumb_func_start sub_8018CB0
sub_8018CB0: @ 0x08018CB0
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	mov r8, r0
	adds r6, r1, #0
	ldr r0, _08018D60 @ =0x0000FFFF
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _08018D64 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x9f
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x20]
	movs r0, #0xf
	adds r1, r4, #0
	adds r1, #0x2d
	movs r5, #0xf
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	ands r0, r5
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x40
	bl sub_8026EDC
	mov r1, r8
	bl sub_80196F8
	str r0, [r4, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	ldr r0, [r6]
	ldr r1, [r6, #4]
	movs r2, #0x80
	lsls r2, r2, #6
	adds r0, r0, r2
	ldr r3, _08018D68 @ =0xFFFFC000
	adds r1, r1, r3
	str r0, [r4]
	str r1, [r4, #4]
	movs r0, #0x10
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _08018D6C @ =gUnknown_030012F4
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	mov r2, r8
	str r4, [r2, #0x20]
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08018D60: .4byte 0x0000FFFF
_08018D64: .4byte gUnknown_030012D0
_08018D68: .4byte 0xFFFFC000
_08018D6C: .4byte gUnknown_030012F4

	thumb_func_start sub_8018D70
sub_8018D70: @ 0x08018D70
	push {r4, r5, lr}
	ldr r5, [sp, #0xc]
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _08018DA8 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	cmp r5, #1
	beq _08018DB6
	cmp r5, #1
	bgt _08018DAC
	cmp r5, #0
	beq _08018DB2
	b _08018DF0
	.align 2, 0
_08018DA8: .4byte gUnknown_030012D0
_08018DAC:
	cmp r5, #2
	beq _08018DD4
	b _08018DF0
_08018DB2:
	movs r0, #3
	b _08018DB8
_08018DB6:
	movs r0, #2
_08018DB8:
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
	b _08018DF0
_08018DD4:
	movs r0, #0
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
_08018DF0:
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x14
	bl sub_8026EDC
	adds r1, r5, #0
	bl sub_80195EC
	str r0, [r4, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #0
	strb r0, [r4, #0xa]
	subs r0, #5
	ldrb r1, [r4, #0xc]
	ands r0, r1
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _08018E48 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08018E48: .4byte gUnknown_030012F0

	thumb_func_start sub_8018E4C
sub_8018E4C: @ 0x08018E4C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	adds r6, r1, #0
	ldr r5, [r7, #0x24]
	cmp r5, #0
	beq _08018E82
	subs r5, #1
	str r5, [r7, #0x24]
	ldr r0, [r7, #0x1c]
	muls r0, r5, r0
	ldr r1, [r7, #0x28]
	mov r8, r1
	bl sub_803ADB4
	ldr r4, [r7, #0x14]
	subs r4, r4, r0
	ldr r0, [r7, #0x20]
	muls r0, r5, r0
	mov r1, r8
	bl sub_803ADB4
	ldr r1, [r7, #0x18]
	subs r1, r1, r0
	str r4, [r6]
	str r1, [r6, #4]
_08018E82:
	ldr r0, [r7, #8]
	cmp r0, #0xa
	bls _08018E8A
	b _0801908A
_08018E8A:
	lsls r0, r0, #2
	ldr r1, _08018E94 @ =_08018E98
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08018E94: .4byte _08018E98
_08018E98: @ jump table
	.4byte _08018EC4 @ case 0
	.4byte _08018EDA @ case 1
	.4byte _08018EDA @ case 2
	.4byte _08018EE4 @ case 3
	.4byte _08018F08 @ case 4
	.4byte _08018F4A @ case 5
	.4byte _08018EDA @ case 6
	.4byte _08018F32 @ case 7
	.4byte _08019070 @ case 8
	.4byte _08019080 @ case 9
	.4byte _0801908A @ case 10
_08018EC4:
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r2, [r6, #0xc]
	ands r0, r2
	strb r0, [r6, #0xc]
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #1
	bl sub_8019094
	b _0801908A
_08018EDA:
	ldr r0, [r7, #0x24]
	cmp r0, #0
	beq _08018EE2
	b _0801908A
_08018EE2:
	b _08018F3E
_08018EE4:
	movs r0, #4
	str r0, [r7, #0x2c]
	ldr r1, [r7, #0xc]
	adds r1, #0x50
	movs r4, #0
	ldrsh r0, [r1, r4]
	adds r0, r7, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #0x12
	bl sub_803AD84
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #7
	bl sub_8019094
	b _0801908A
_08018F08:
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #0
	bl sub_8019214
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #2
	bl sub_8019094
	ldr r1, [r7, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r7, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #0xf
	bl sub_803AD84
	b _0801908A
_08018F32:
	adds r0, r6, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	bne _08018F3E
	b _0801908A
_08018F3E:
	ldr r2, [r7, #0x2c]
	adds r0, r7, #0
	adds r1, r6, #0
	bl sub_8019094
	b _0801908A
_08018F4A:
	adds r0, r7, #0
	adds r0, #0x38
	ldrb r1, [r0]
	adds r5, r0, #0
	cmp r1, #0
	beq _08018F86
	ldr r0, [r7, #0x34]
	adds r0, #1
	str r0, [r7, #0x34]
	cmp r0, #9
	ble _08018F86
	movs r0, #0
	str r0, [r7, #0x34]
	ldr r3, [r6, #0x30]
	movs r0, #1
	eors r3, r0
	ldr r0, [r6, #0x20]
	adds r2, r6, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r4, [r2]
	lsls r0, r4, #3
	subs r0, r0, r4
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08018F84
	subs r3, r0, #1
_08018F84:
	str r3, [r6, #0x30]
_08018F86:
	ldr r4, [r7, #0x24]
	cmp r4, #0
	beq _08018F8E
	b _0801908A
_08018F8E:
	ldr r0, [r7, #0x30]
	subs r2, r0, #1
	str r2, [r7, #0x30]
	cmp r2, #0
	bne _08018FAE
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #1
	bl sub_8019094
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #1
	bl sub_8019214
	b _0801908A
_08018FAE:
	ldr r1, _08019058 @ =gStaticData_0816C35F
	ldr r0, [r7, #0x3c]
	ldr r0, [r0, #0x10]
	adds r0, r0, r1
	ldrb r0, [r0]
	cmp r2, r0
	bne _08018FEA
	ldr r0, _0801905C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x5c
	bl PlaySfx
	adds r0, r6, #0
	adds r0, #0x2c
	strb r4, [r0]
	ldr r1, [r7, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r7, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #0x10
	bl sub_803AD84
	movs r0, #1
	strb r0, [r5]
	str r4, [r7, #0x34]
_08018FEA:
	ldr r1, _08019060 @ =gStaticData_0816C362
	ldr r0, [r7, #0x3c]
	ldr r0, [r0, #0x10]
	adds r0, r0, r1
	ldr r1, [r7, #0x30]
	ldrb r0, [r0]
	cmp r1, r0
	bne _08019034
	adds r0, r6, #0
	adds r0, #0x2c
	strb r4, [r0]
	ldr r1, [r7, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r7, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #0x10
	bl sub_803AD84
	strb r4, [r5]
	movs r3, #1
	ldr r0, [r6, #0x20]
	adds r2, r6, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r4, [r2]
	lsls r0, r4, #3
	subs r0, r0, r4
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08019032
	subs r3, r0, #1
_08019032:
	str r3, [r6, #0x30]
_08019034:
	ldr r0, _08019064 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r2, [r0]
	ldr r3, [r0, #4]
	ldr r0, _08019068 @ =0xFFFFF600
	adds r3, r3, r0
	adds r0, r7, #0
	adds r1, r6, #0
	bl sub_80196B8
	ldr r0, [r7, #0x3c]
	ldr r0, [r0, #0x10]
	ldr r1, _0801906C @ =gStaticData_0816C35C
	adds r0, r0, r1
	ldrb r0, [r0]
	str r0, [r7, #0x28]
	str r0, [r7, #0x24]
	b _0801908A
	.align 2, 0
_08019058: .4byte gStaticData_0816C35F
_0801905C: .4byte gUnknown_030012BC
_08019060: .4byte gStaticData_0816C362
_08019064: .4byte gUnknown_030012D8
_08019068: .4byte 0xFFFFF600
_0801906C: .4byte gStaticData_0816C35C
_08019070:
	movs r0, #0xa
	str r0, [r7, #0x2c]
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #6
	bl sub_8019094
	b _0801908A
_08019080:
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #8
	bl sub_8019094
_0801908A:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8019094
sub_8019094: @ 0x08019094
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	adds r7, r1, #0
	adds r6, r2, #0
	cmp r6, #2
	beq _08019128
	cmp r6, #2
	bgt _080190AA
	cmp r6, #1
	beq _080190D4
	b _080191FE
_080190AA:
	cmp r6, #5
	bne _080190B0
	b _080191EA
_080190B0:
	cmp r6, #8
	beq _080190B6
	b _080191FE
_080190B6:
	ldr r0, _080190D0 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r2, [r0, #0x10]
	lsls r2, r2, #8
	lsrs r2, r2, #1
	ldr r3, [r0, #0x14]
	lsls r3, r3, #8
	movs r0, #0x80
	lsls r0, r0, #6
	adds r3, r3, r0
	b _080191E0
	.align 2, 0
_080190D0: .4byte gUnknown_03001308
_080190D4:
	adds r0, r5, #0
	movs r1, #0
	bl sub_801967C
	adds r0, r7, #0
	adds r0, #0x2c
	movs r4, #0
	strb r6, [r0]
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r7, #0
	movs r2, #0xf
	bl sub_803AD84
	strb r6, [r5, #0x10]
	strb r6, [r5, #0x11]
	strb r4, [r5, #0x12]
	ldr r0, _08019120 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r2, [r0, #0x10]
	lsls r2, r2, #8
	ldr r0, _08019124 @ =0xFFFFFC00
	adds r2, r2, r0
	movs r3, #0x98
	lsls r3, r3, #8
	adds r0, r5, #0
	adds r1, r7, #0
	bl sub_80196B8
	movs r0, #2
	str r0, [r5, #0x2c]
	b _080191FE
	.align 2, 0
_08019120: .4byte gUnknown_03001308
_08019124: .4byte 0xFFFFFC00
_08019128:
	movs r0, #3
	str r0, [r5, #0x2c]
	ldr r2, [r5, #0x14]
	ldrb r0, [r5, #0x10]
	cmp r0, #0
	beq _0801914C
	ldr r0, _08019148 @ =0xFFFFE800
	adds r1, r2, r0
	movs r0, #0x80
	lsls r0, r0, #3
	cmp r1, r0
	bgt _08019164
	movs r0, #0
	strb r0, [r5, #0x10]
	b _08019164
	.align 2, 0
_08019148: .4byte 0xFFFFE800
_0801914C:
	movs r0, #0xe0
	lsls r0, r0, #5
	adds r1, r2, r0
	ldr r0, _0801917C @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x10]
	lsls r0, r0, #8
	cmp r1, r0
	blt _08019164
	movs r0, #5
	str r0, [r5, #0x2c]
_08019164:
	ldr r0, [r5, #0x3c]
	ldr r0, [r0, #0x10]
	adds r1, r0, #0
	cmp r0, #1
	beq _08019190
	cmp r0, #1
	bgt _08019180
	cmp r0, #0
	beq _08019188
	ldrb r1, [r5, #0x10]
	b _080191B0
	.align 2, 0
_0801917C: .4byte gUnknown_03001308
_08019180:
	cmp r1, #2
	beq _0801919C
	ldrb r1, [r5, #0x10]
	b _080191B0
_08019188:
	ldrb r0, [r5, #0x10]
	strb r0, [r5, #0x11]
	adds r1, r0, #0
	b _080191B0
_08019190:
	movs r0, #1
	ldrb r1, [r5, #0x11]
	eors r0, r1
	strb r0, [r5, #0x11]
	ldrb r1, [r5, #0x10]
	b _080191B0
_0801919C:
	movs r3, #1
	ldrb r0, [r5, #0x11]
	eors r0, r3
	strb r0, [r5, #0x11]
	ldrb r1, [r5, #0x10]
	cmp r0, #0
	bne _080191B0
	ldrb r0, [r5, #0x12]
	eors r0, r3
	strb r0, [r5, #0x12]
_080191B0:
	lsls r0, r1, #0x18
	cmp r0, #0
	beq _080191C0
	ldr r0, _080191BC @ =0xFFFFE800
	adds r2, r2, r0
	b _080191C6
	.align 2, 0
_080191BC: .4byte 0xFFFFE800
_080191C0:
	movs r1, #0xc0
	lsls r1, r1, #5
	adds r2, r2, r1
_080191C6:
	ldrb r0, [r5, #0x11]
	cmp r0, #0
	beq _080191DC
	ldrb r0, [r5, #0x12]
	movs r3, #0x98
	lsls r3, r3, #8
	cmp r0, #0
	beq _080191E0
	movs r3, #0xf8
	lsls r3, r3, #6
	b _080191E0
_080191DC:
	movs r3, #0x82
	lsls r3, r3, #8
_080191E0:
	adds r0, r5, #0
	adds r1, r7, #0
	bl sub_80196B8
	b _080191FE
_080191EA:
	adds r0, r5, #0
	adds r0, #0x38
	movs r1, #0
	strb r1, [r0]
	adds r0, r5, #0
	movs r1, #1
	bl sub_801967C
	movs r0, #0x14
	str r0, [r5, #0x30]
_080191FE:
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	adds r1, r6, #0
	bl sub_803AD80
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8019214
sub_8019214: @ 0x08019214
	push {r4, r5, r6, r7, lr}
	adds r7, r0, #0
	adds r6, r1, #0
	adds r5, r2, #0
	ldr r0, _08019244 @ =0x0000FFFF
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _08019248 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x9f
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x20]
	cmp r5, #0
	beq _0801924C
	cmp r5, #1
	beq _0801926A
	b _08019286
	.align 2, 0
_08019244: .4byte 0x0000FFFF
_08019248: .4byte gUnknown_030012D0
_0801924C:
	movs r0, #0xe
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
	b _08019286
_0801926A:
	movs r0, #0x11
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
_08019286:
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x18
	bl sub_8026EDC
	ldr r1, [r7, #0x3c]
	bl sub_8019660
	adds r2, r0, #0
	movs r0, #0
	cmp r5, #1
	bne _080192B6
	movs r0, #1
_080192B6:
	strb r0, [r2, #0x10]
	str r2, [r4, #0x44]
	ldr r1, [r2, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	ldr r0, [r6]
	ldr r1, [r6, #4]
	str r0, [r4]
	str r1, [r4, #4]
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r1, [r4, #0xc]
	ands r0, r1
	movs r1, #1
	strb r1, [r4, #0xa]
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _08019304 @ =gUnknown_030012F4
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	cmp r5, #1
	bne _0801930C
	ldr r0, _08019308 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x31
	bl PlaySfx
	b _0801931A
	.align 2, 0
_08019304: .4byte gUnknown_030012F4
_08019308: .4byte gUnknown_030012BC
_0801930C:
	ldr r0, _08019320 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x32
	bl PlaySfx
_0801931A:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08019320: .4byte gUnknown_030012BC

	thumb_func_start sub_8019324
sub_8019324: @ 0x08019324
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x30
	mov sl, r0
	mov r8, r1
	ldrb r0, [r1, #0xa]
	cmp r0, #1
	bne _08019406
	ldr r4, _080193A4 @ =gUnknown_030012D8
	ldr r1, [r4]
	mov r0, sp
	bl sub_8007CF8
	ldr r0, [sp, #8]
	add r6, sp, #0x10
	cmp r0, #0
	bne _08019360
	ldr r1, [r4]
	adds r0, r6, #0
	bl sub_8007C30
	mov r1, sp
	adds r0, r6, #0
	ldm r0!, {r2, r3, r5}
	stm r1!, {r2, r3, r5}
	ldr r0, [r0]
	str r0, [r1]
_08019360:
	adds r0, r6, #0
	mov r1, r8
	bl sub_8007C30
	mov r0, sp
	adds r1, r6, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _080193A8
	ldr r2, [r4]
	movs r1, #0x82
	lsls r1, r1, #1
	adds r0, r2, r1
	ldrb r0, [r0]
	cmp r0, #0
	bne _0801939A
	ldr r1, [r2, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #9
	movs r3, #0
	bl sub_803AD88
_0801939A:
	movs r0, #0
	mov r4, r8
	strb r0, [r4, #0xa]
	b _08019406
	.align 2, 0
_080193A4: .4byte gUnknown_030012D8
_080193A8:
	mov r5, sl
	ldrb r0, [r5, #0x10]
	cmp r0, #0
	beq _08019406
	ldr r0, _08019458 @ =gUnknown_030012F0
	ldr r0, [r0]
	ldr r0, [r0, #4]
	mov sb, r0
	movs r5, #0
	cmp r5, sb
	bge _08019406
	add r7, sp, #0x20
_080193C0:
	ldr r0, _08019458 @ =gUnknown_030012F0
	ldr r0, [r0]
	ldr r1, [r0, #0xc]
	lsls r0, r5, #2
	adds r0, r0, r1
	ldr r4, [r0]
	adds r0, r7, #0
	adds r1, r4, #0
	bl sub_8007B98
	adds r0, r7, #0
	adds r1, r6, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08019400
	mov r1, sl
	ldr r0, [r1, #0x14]
	ldr r2, [r0, #0xc]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #2
	bl sub_803AD80
	movs r0, #1
	strb r0, [r4, #0xa]
	movs r2, #0
	mov r1, r8
	strb r2, [r1, #0xa]
_08019400:
	adds r5, #1
	cmp r5, sb
	blt _080193C0
_08019406:
	mov r0, r8
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08019446
	movs r0, #1
	mov r3, r8
	ldrb r3, [r3, #0xc]
	orrs r0, r3
	mov r4, r8
	strb r0, [r4, #0xc]
	ldr r0, _0801945C @ =0x0000FFFF
	ldrh r5, [r4, #8]
	cmp r5, r0
	beq _08019446
	ldrh r3, [r4, #8]
	ldr r0, _08019460 @ =gUnknown_030012B4
	ldr r2, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r1, r0, #2
	movs r4, #0x84
	lsls r4, r4, #1
	adds r2, r2, r4
	adds r2, r2, r1
	lsls r0, r0, #5
	subs r0, r3, r0
	movs r1, #1
	lsls r1, r0
	ldr r0, [r2]
	orrs r0, r1
	str r0, [r2]
_08019446:
	add sp, #0x30
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08019458: .4byte gUnknown_030012F0
_0801945C: .4byte 0x0000FFFF
_08019460: .4byte gUnknown_030012B4

	thumb_func_start sub_8019464
sub_8019464: @ 0x08019464
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	adds r3, r1, #0
	ldr r0, [r5, #8]
	cmp r0, #0
	bne _08019492
	movs r4, #0x1a
	ldr r0, [r3, #0x20]
	adds r2, r3, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r6, [r2]
	lsls r0, r6, #3
	subs r0, r0, r6
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _0801948C
	subs r4, r0, #1
_0801948C:
	str r4, [r3, #0x30]
	movs r0, #1
	str r0, [r5, #8]
_08019492:
	movs r1, #0x1a
	ldrb r0, [r3, #0xa]
	cmp r0, #1
	bne _0801949C
	movs r1, #0xa
_0801949C:
	ldr r0, [r3, #0x30]
	cmp r0, r1
	bne _080194AC
	adds r1, r3, #0
	adds r1, #0x2c
	movs r0, #0
	strb r0, [r1]
	b _080194DA
_080194AC:
	movs r1, #1
	adds r0, r3, #0
	adds r0, #0x2c
	strb r1, [r0]
	adds r0, #0xc
	ldrb r0, [r0]
	cmp r0, #0
	beq _080194DA
	movs r4, #0
	ldr r0, [r3, #0x20]
	adds r2, r3, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r5, [r2]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _080194D8
	subs r4, r0, #1
_080194D8:
	str r4, [r3, #0x30]
_080194DA:
	pop {r4, r5, r6}
	pop {r0}
	bx r0

	thumb_func_start sub_80194E0
sub_80194E0: @ 0x080194E0
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	adds r4, r1, #0
	ldr r6, [r5, #8]
	cmp r6, #0
	beq _080194F2
	cmp r6, #1
	beq _0801958E
	b _080195C8
_080194F2:
	ldrb r0, [r4, #0xa]
	cmp r0, #1
	bne _080195C8
	ldr r0, _08019518 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x9f
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x20]
	ldr r0, [r5, #0x10]
	cmp r0, #1
	beq _08019538
	cmp r0, #1
	bgt _0801951C
	cmp r0, #0
	beq _08019522
	b _08019562
	.align 2, 0
_08019518: .4byte gUnknown_030012D0
_0801951C:
	cmp r0, #2
	beq _0801954E
	b _08019562
_08019522:
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r4, #0
	movs r2, #0xc
	bl sub_803AD84
	b _08019562
_08019538:
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r4, #0
	movs r2, #0xb
	bl sub_803AD84
	b _08019562
_0801954E:
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r4, #0
	movs r2, #0xd
	bl sub_803AD84
_08019562:
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldr r1, [r5, #0xc]
	movs r4, #0x20
	ldrsh r0, [r1, r4]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #1
	bl sub_803AD80
	b _080195C8
_0801958E:
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _080195C8
	movs r0, #1
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _080195D0 @ =0x0000FFFF
	ldrh r2, [r4, #8]
	cmp r2, r0
	beq _080195C8
	ldrh r3, [r4, #8]
	ldr r0, _080195D4 @ =gUnknown_030012B4
	ldr r1, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r2, r0, #2
	movs r4, #0x84
	lsls r4, r4, #1
	adds r1, r1, r4
	adds r1, r1, r2
	lsls r0, r0, #5
	subs r0, r3, r0
	lsls r6, r0
	ldr r0, [r1]
	orrs r0, r6
	str r0, [r1]
_080195C8:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_080195D0: .4byte 0x0000FFFF
_080195D4: .4byte gUnknown_030012B4

	thumb_func_start sub_80195D8
sub_80195D8: @ 0x080195D8
	push {lr}
	ldr r2, _080195E8 @ =gStaticData_087E45CC
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_080195E8: .4byte gStaticData_087E45CC

	thumb_func_start sub_80195EC
sub_80195EC: @ 0x080195EC
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	bl sub_800B8C8
	ldr r0, _08019604 @ =gStaticData_087E45CC
	str r0, [r4, #0xc]
	str r5, [r4, #0x10]
	adds r0, r4, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08019604: .4byte gStaticData_087E45CC

	thumb_func_start sub_8019608
sub_8019608: @ 0x08019608
	push {lr}
	ldr r2, _08019618 @ =gStaticData_087E4634
	str r2, [r0, #0xc]
	bl sub_801B7C4
	pop {r0}
	bx r0
	.align 2, 0
_08019618: .4byte gStaticData_087E4634

	thumb_func_start sub_801961C
sub_801961C: @ 0x0801961C
	push {r4, lr}
	sub sp, #8
	adds r4, r0, #0
	mov r1, sp
	movs r0, #0
	strb r0, [r1]
	movs r0, #6
	str r0, [sp, #4]
	adds r0, r4, #0
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_801B7D8
	ldr r0, _08019648 @ =gStaticData_087E4634
	str r0, [r4, #0xc]
	adds r0, r4, #0
	add sp, #8
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08019648: .4byte gStaticData_087E4634

	thumb_func_start sub_801964C
sub_801964C: @ 0x0801964C
	push {lr}
	ldr r2, _0801965C @ =gStaticData_087E469C
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_0801965C: .4byte gStaticData_087E469C

	thumb_func_start sub_8019660
sub_8019660: @ 0x08019660
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	bl sub_800B8C8
	ldr r0, _08019678 @ =gStaticData_087E469C
	str r0, [r4, #0xc]
	str r5, [r4, #0x14]
	adds r0, r4, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08019678: .4byte gStaticData_087E469C

	thumb_func_start sub_801967C
sub_801967C: @ 0x0801967C
	push {r4, r5, r6, lr}
	lsls r1, r1, #0x18
	lsrs r3, r1, #0x18
	ldr r1, _080196A4 @ =gUnknown_030012EC
	ldr r0, [r1]
	ldr r4, [r0, #4]
	movs r2, #0
	cmp r2, r4
	bge _080196B0
	adds r6, r1, #0
	movs r5, #1
_08019692:
	ldr r0, [r6]
	ldr r1, [r0, #0xc]
	lsls r0, r2, #2
	adds r0, r0, r1
	ldr r0, [r0]
	cmp r3, #0
	beq _080196A8
	strb r5, [r0, #0xa]
	b _080196AA
	.align 2, 0
_080196A4: .4byte gUnknown_030012EC
_080196A8:
	strb r3, [r0, #0xa]
_080196AA:
	adds r2, #1
	cmp r2, r4
	blt _08019692
_080196B0:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80196B8
sub_80196B8: @ 0x080196B8
	push {r4, lr}
	str r2, [r0, #0x14]
	str r3, [r0, #0x18]
	ldr r4, [r1]
	subs r2, r2, r4
	str r2, [r0, #0x1c]
	ldr r1, [r1, #4]
	subs r3, r3, r1
	str r3, [r0, #0x20]
	ldr r1, [r0, #0x3c]
	ldr r1, [r1, #0x10]
	ldr r2, _080196E0 @ =gStaticData_0816C358
	adds r1, r1, r2
	ldrb r1, [r1]
	str r1, [r0, #0x28]
	str r1, [r0, #0x24]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080196E0: .4byte gStaticData_0816C358

	thumb_func_start sub_80196E4
sub_80196E4: @ 0x080196E4
	push {lr}
	ldr r2, _080196F4 @ =gStaticData_087E4704
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_080196F4: .4byte gStaticData_087E4704

	thumb_func_start sub_80196F8
sub_80196F8: @ 0x080196F8
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	bl sub_800B8C8
	ldr r0, _08019714 @ =gStaticData_087E4704
	str r0, [r4, #0xc]
	movs r0, #0
	str r0, [r4, #0x24]
	str r5, [r4, #0x3c]
	adds r0, r4, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08019714: .4byte gStaticData_087E4704

	thumb_func_start sub_8019718
sub_8019718: @ 0x08019718
	push {r4, lr}
	adds r1, r2, #0
	ldr r3, [r0, #0xc]
	movs r4, #0x20
	ldrsh r2, [r3, r4]
	adds r0, r0, r2
	ldr r2, [r3, #0x24]
	bl sub_803AD80
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8019730
sub_8019730: @ 0x08019730
	ldr r0, [r0, #8]
	cmp r0, #0
	bne _08019740
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xc]
	ands r0, r2
	strb r0, [r1, #0xc]
_08019740:
	bx lr
	.align 2, 0

	thumb_func_start sub_8019744
sub_8019744: @ 0x08019744
	push {lr}
	ldr r2, _08019754 @ =gStaticData_087E476C
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_08019754: .4byte gStaticData_087E476C

	thumb_func_start sub_8019758
sub_8019758: @ 0x08019758
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800B8C8
	ldr r0, _0801976C @ =gStaticData_087E476C
	str r0, [r4, #0xc]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801976C: .4byte gStaticData_087E476C

	thumb_func_start sub_8019770
sub_8019770: @ 0x08019770
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r2, #0
	cmp r5, #3
	bne _080197A8
	ldr r0, [r4, #0x20]
	ldr r0, [r0, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #9
	bl sub_803AD80
	ldr r0, _080197C0 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231C4
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _080197A8
	ldr r0, _080197C4 @ =0x0000FFFF
	movs r1, #0x8c
	movs r2, #0x98
	movs r3, #0
	bl sub_8021D80
_080197A8:
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	adds r1, r5, #0
	bl sub_803AD80
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080197C0: .4byte gUnknown_030012C0
_080197C4: .4byte 0x0000FFFF

	thumb_func_start sub_80197C8
sub_80197C8: @ 0x080197C8
	push {lr}
	ldr r2, _080197D8 @ =gStaticData_087E47D4
	str r2, [r0, #0xc]
	bl sub_8017A78
	pop {r0}
	bx r0
	.align 2, 0
_080197D8: .4byte gStaticData_087E47D4

	thumb_func_start sub_80197DC
sub_80197DC: @ 0x080197DC
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8017A8C
	ldr r0, _080197F0 @ =gStaticData_087E47D4
	str r0, [r4, #0xc]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_080197F0: .4byte gStaticData_087E47D4

	thumb_func_start sub_80197F4
sub_80197F4: @ 0x080197F4
	ldr r0, [r0, #0x10]
	bx lr

	thumb_func_start sub_80197F8
sub_80197F8: @ 0x080197F8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x24
	adds r5, r0, #0
	adds r4, r1, #0
	adds r0, r4, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _0801981E
	ldr r0, [r4]
	ldr r2, [r4, #4]
	ldr r1, [r5, #0x2c]
	movs r3, #0xc0
	lsls r3, r3, #3
	adds r0, r0, r3
	b _08019828
_0801981E:
	ldr r0, [r4]
	ldr r2, [r4, #4]
	ldr r1, [r5, #0x2c]
	ldr r6, _08019884 @ =0xFFFFFA00
	adds r0, r0, r6
_08019828:
	str r0, [r1]
	str r2, [r1, #4]
	add r0, sp, #4
	adds r1, r4, #0
	bl sub_8007CF8
	ldr r0, [r5, #8]
	cmp r0, #8
	bne _08019870
	ldr r0, _08019888 @ =gUnknown_030012D8
	ldr r1, [r0]
	ldrb r7, [r1, #0xa]
	cmp r7, #0x13
	bne _08019870
	add r6, sp, #0x14
	adds r0, r6, #0
	bl sub_8007C30
	ldr r0, [sp, #0x1c]
	cmp r0, #0
	beq _08019870
	adds r0, r6, #0
	add r1, sp, #4
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08019870
	ldr r0, [r5, #0x10]
	adds r0, #1
	str r0, [r5, #0x10]
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0xb
	bl sub_8019CE4
_08019870:
	ldr r3, [r5, #8]
	cmp r3, #0x11
	bls _08019878
	b _08019CD0
_08019878:
	lsls r0, r3, #2
	ldr r1, _0801988C @ =_08019890
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08019884: .4byte 0xFFFFFA00
_08019888: .4byte gUnknown_030012D8
_0801988C: .4byte _08019890
_08019890: @ jump table
	.4byte _080198D8 @ case 0
	.4byte _080198FA @ case 1
	.4byte _08019BA0 @ case 2
	.4byte _080199E4 @ case 3
	.4byte _08019AEC @ case 4
	.4byte _08019AFA @ case 5
	.4byte _08019AFA @ case 6
	.4byte _08019BB8 @ case 7
	.4byte _08019BE2 @ case 8
	.4byte _08019BF8 @ case 9
	.4byte _08019BF8 @ case 10
	.4byte _08019C1E @ case 11
	.4byte _08019C50 @ case 12
	.4byte _080199E4 @ case 13
	.4byte _080198FA @ case 14
	.4byte _08019AAE @ case 15
	.4byte _08019C94 @ case 16
	.4byte _08019CD0 @ case 17
_080198D8:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #1
	bl sub_8019CE4
	movs r0, #0
	str r0, [r5, #0x1c]
	str r0, [r5, #0x28]
	strb r0, [r4, #0xa]
	subs r0, #5
	ldrb r1, [r4, #0xc]
	ands r0, r1
	movs r1, #0x41
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r4, #0xc]
	b _08019CD0
_080198FA:
	adds r6, r4, #0
	adds r6, #0x28
	ldr r2, [r4]
	cmp r3, #1
	bne _08019980
	ldrb r7, [r6]
	lsls r0, r7, #0x1b
	cmp r0, #0
	bge _08019938
	ldr r1, _0801992C @ =gStaticData_0816C368
	ldr r0, [r5, #0x10]
	cmp r0, #0
	ble _08019916
	ldr r1, _08019930 @ =gStaticData_0816C378
_08019916:
	ldr r0, [r5, #0x1c]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	cmp r2, r0
	bgt _08019980
	ldr r0, _08019934 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0]
	subs r0, r0, r2
	b _08019956
	.align 2, 0
_0801992C: .4byte gStaticData_0816C368
_08019930: .4byte gStaticData_0816C378
_08019934: .4byte gUnknown_030012D8
_08019938:
	ldr r1, _08019970 @ =gStaticData_0816C390
	ldr r0, [r5, #0x10]
	cmp r0, #0
	ble _08019942
	ldr r1, _08019974 @ =gStaticData_0816C3A0
_08019942:
	ldr r0, [r5, #0x1c]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	cmp r2, r0
	blt _08019980
	ldr r0, _08019978 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0]
	subs r0, r2, r0
_08019956:
	ldr r1, _0801997C @ =0x00001FFF
	cmp r0, r1
	bgt _08019966
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #5
	bl sub_8019CE4
_08019966:
	ldr r0, [r5, #0x1c]
	adds r0, #1
	str r0, [r5, #0x1c]
	b _08019CD0
	.align 2, 0
_08019970: .4byte gStaticData_0816C390
_08019974: .4byte gStaticData_0816C3A0
_08019978: .4byte gUnknown_030012D8
_0801997C: .4byte 0x00001FFF
_08019980:
	ldrb r6, [r6]
	lsls r0, r6, #0x1b
	cmp r0, #0
	bge _08019994
	movs r1, #0
	movs r0, #0x80
	lsls r0, r0, #6
	cmp r2, r0
	bgt _080199AA
	b _080199A8
_08019994:
	ldr r0, _080199DC @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x10]
	lsls r0, r0, #8
	movs r1, #0
	ldr r6, _080199E0 @ =0xFFFFE000
	adds r0, r0, r6
	cmp r2, r0
	blt _080199AA
_080199A8:
	movs r1, #1
_080199AA:
	adds r0, r1, #0
	cmp r0, #0
	bne _080199B2
	b _08019CD0
_080199B2:
	cmp r3, #1
	beq _080199B8
	b _08019B6A
_080199B8:
	movs r1, #1
	ldr r0, [r5, #0x10]
	cmp r0, #0
	ble _080199C2
	movs r1, #2
_080199C2:
	ldr r0, [r5, #0x28]
	adds r0, #1
	str r0, [r5, #0x28]
	cmp r0, r1
	bge _080199CE
	b _08019B6A
_080199CE:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #6
	bl sub_8019CE4
	b _08019CD0
	.align 2, 0
_080199DC: .4byte gUnknown_03001308
_080199E0: .4byte 0xFFFFE000
_080199E4:
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	bne _080199F0
	b _08019CD0
_080199F0:
	mov r8, r3
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #1
	bl sub_8019CE4
	adds r0, r4, #0
	adds r0, #0x28
	ldrb r7, [r0]
	lsls r1, r7, #0x1b
	adds r6, r0, #0
	cmp r1, #0
	bge _08019A16
	ldr r0, [r4]
	asrs r0, r0, #8
	ldr r1, [r4, #4]
	asrs r1, r1, #8
	adds r0, #6
	b _08019A20
_08019A16:
	ldr r0, [r4]
	asrs r0, r0, #8
	ldr r1, [r4, #4]
	asrs r1, r1, #8
	subs r0, #6
_08019A20:
	lsls r0, r0, #8
	str r0, [r4]
	lsls r1, r1, #8
	str r1, [r4, #4]
	movs r3, #8
	ldr r0, [r4, #0x20]
	adds r2, r4, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r7, [r2]
	lsls r0, r7, #3
	adds r2, r7, #0
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08019A46
	subs r3, r0, #1
_08019A46:
	str r3, [r4, #0x30]
	ldrb r2, [r6]
	lsls r0, r2, #0x1b
	movs r1, #0
	cmp r0, #0
	blt _08019A54
	movs r1, #1
_08019A54:
	lsls r1, r1, #4
	movs r0, #0x11
	rsbs r0, r0, #0
	ands r0, r2
	orrs r0, r1
	strb r0, [r6]
	mov r0, r8
	cmp r0, #3
	bne _08019AA2
	ldr r0, [r5, #0x28]
	cmp r0, #0
	bne _08019A8A
	movs r0, #0x73
	str r0, [r5, #0x20]
	ldr r0, [r5, #0x10]
	cmp r0, #1
	ble _08019A7A
	movs r0, #0xf
	b _08019A7C
_08019A7A:
	movs r0, #1
_08019A7C:
	str r0, [r5, #0x24]
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #2
	bl sub_8019CE4
	b _08019A9C
_08019A8A:
	movs r0, #0x64
	str r0, [r5, #0x20]
	movs r0, #1
	str r0, [r5, #0x24]
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #2
	bl sub_8019CE4
_08019A9C:
	movs r0, #0
	str r0, [r5, #0x1c]
	b _08019CD0
_08019AA2:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0xe
	bl sub_8019CE4
	b _08019CD0
_08019AAE:
	adds r0, r4, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _08019AC8
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0x2d
	movs r3, #0
	bl sub_801A03C
	b _08019C06
_08019AC8:
	ldr r0, _08019AE8 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #0x10
	movs r2, #0xa0
	lsls r2, r2, #0xe
	adds r1, r1, r2
	lsrs r1, r1, #0x10
	adds r0, r5, #0
	movs r2, #0x2d
	movs r3, #1
	bl sub_801A03C
	b _08019C06
	.align 2, 0
_08019AE8: .4byte gUnknown_03001308
_08019AEC:
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	bne _08019AF8
	b _08019CD0
_08019AF8:
	b _08019C06
_08019AFA:
	ldr r0, [r4, #0x30]
	cmp r0, #0x14
	bne _08019B50
	ldr r0, [r4, #0x34]
	cmp r0, #0
	bne _08019B50
	adds r0, r4, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _08019B32
	ldr r2, [r4]
	asrs r2, r2, #8
	adds r2, #6
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	ldr r3, [r4, #4]
	asrs r3, r3, #8
	subs r3, #0x32
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	str r4, [sp]
	adds r0, r5, #0
	movs r1, #1
	bl sub_8019EBC
	b _08019B50
_08019B32:
	ldr r2, [r4]
	asrs r2, r2, #8
	subs r2, #6
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	ldr r3, [r4, #4]
	asrs r3, r3, #8
	subs r3, #0x32
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	str r4, [sp]
	adds r0, r5, #0
	movs r1, #1
	bl sub_8019EBC
_08019B50:
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	bne _08019B5C
	b _08019CD0
_08019B5C:
	ldr r0, [r5, #8]
	cmp r0, #6
	bne _08019B76
	movs r0, #0x40
	ldrb r3, [r4, #0xc]
	orrs r0, r3
	strb r0, [r4, #0xc]
_08019B6A:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #3
	bl sub_8019CE4
	b _08019CD0
_08019B76:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #1
	bl sub_8019CE4
	movs r3, #8
	ldr r0, [r4, #0x20]
	adds r2, r4, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r5, [r2]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08019B9C
	subs r3, r0, #1
_08019B9C:
	str r3, [r4, #0x30]
	b _08019CD0
_08019BA0:
	ldr r0, [r5, #0x20]
	subs r0, #1
	str r0, [r5, #0x20]
	cmp r0, #0
	beq _08019BAC
	b _08019CD0
_08019BAC:
	ldr r2, [r5, #0x24]
	adds r0, r5, #0
	adds r1, r4, #0
	bl sub_8019CE4
	b _08019CD0
_08019BB8:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0
	bl sub_801A7AC
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r6, #0
	ldrsh r0, [r1, r6]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r4, #0
	movs r2, #5
	bl sub_803AD84
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #8
	bl sub_8019CE4
	b _08019CD0
_08019BE2:
	ldr r0, [r5, #0x20]
	subs r0, #1
	str r0, [r5, #0x20]
	cmp r0, #0
	bne _08019CD0
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #9
	bl sub_8019CE4
	b _08019CD0
_08019BF8:
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08019CD0
	cmp r3, #9
	bne _08019C12
_08019C06:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #1
	bl sub_8019CE4
	b _08019CD0
_08019C12:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0xc
	bl sub_8019CE4
	b _08019CD0
_08019C1E:
	ldr r0, [r5, #0x20]
	cmp r0, #0
	beq _08019C2A
	subs r0, #1
	str r0, [r5, #0x20]
	b _08019CD0
_08019C2A:
	ldr r0, [r5, #0x10]
	cmp r0, #2
	ble _08019C3A
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0x10
	bl sub_8019CE4
_08019C3A:
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08019CD0
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0xa
	bl sub_8019CE4
	b _08019CD0
_08019C50:
	movs r2, #0x80
	lsls r2, r2, #7
	ldr r0, [r5, #0x10]
	cmp r0, #0
	ble _08019C5E
	movs r2, #0x80
	lsls r2, r2, #6
_08019C5E:
	adds r0, r4, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _08019C72
	ldr r0, [r4]
	cmp r0, r2
	bgt _08019CD0
	b _08019C84
_08019C72:
	ldr r1, [r4]
	ldr r0, _08019C90 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x10]
	lsls r0, r0, #8
	subs r0, r0, r2
	cmp r1, r0
	blt _08019CD0
_08019C84:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0xd
	bl sub_8019CE4
	b _08019CD0
	.align 2, 0
_08019C90: .4byte gUnknown_03001308
_08019C94:
	ldr r1, [r4, #4]
	ldr r0, _08019CDC @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x14]
	lsls r0, r0, #8
	movs r7, #0x80
	lsls r7, r7, #6
	adds r0, r0, r7
	cmp r1, r0
	blt _08019CD0
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0
	bl sub_801A7AC
	ldr r0, _08019CE0 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231BC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08019CC6
	bl sub_80241A4
_08019CC6:
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0x11
	bl sub_8019CE4
_08019CD0:
	add sp, #0x24
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08019CDC: .4byte gUnknown_03001308
_08019CE0: .4byte gUnknown_030012C0

	thumb_func_start sub_8019CE4
sub_8019CE4: @ 0x08019CE4
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	adds r4, r2, #0
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	adds r1, r4, #0
	bl sub_803AD80
	subs r0, r4, #1
	cmp r0, #0xf
	bls _08019D04
	b _08019EB0
_08019D04:
	lsls r0, r0, #2
	ldr r1, _08019D10 @ =_08019D14
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08019D10: .4byte _08019D14
_08019D14: @ jump table
	.4byte _08019DBC @ case 0
	.4byte _08019E1C @ case 1
	.4byte _08019DE0 @ case 2
	.4byte _08019DF2 @ case 3
	.4byte _08019E08 @ case 4
	.4byte _08019E04 @ case 5
	.4byte _08019EB0 @ case 6
	.4byte _08019E28 @ case 7
	.4byte _08019E42 @ case 8
	.4byte _08019E42 @ case 9
	.4byte _08019E6C @ case 10
	.4byte _08019D84 @ case 11
	.4byte _08019DE0 @ case 12
	.4byte _08019DBC @ case 13
	.4byte _08019EB0 @ case 14
	.4byte _08019D54 @ case 15
_08019D54:
	ldr r0, _08019D7C @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231BC
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08019D6E
	ldr r0, _08019D80 @ =0x0000FFFF
	movs r1, #0xa0
	movs r2, #0xa9
	movs r3, #0
	bl sub_8021EF4
_08019D6E:
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #3
	bl sub_801A7AC
	b _08019EB0
	.align 2, 0
_08019D7C: .4byte gUnknown_030012C0
_08019D80: .4byte 0x0000FFFF
_08019D84:
	ldr r4, _08019DDC @ =gUnknown_03001308
	ldr r0, [r4]
	ldr r0, [r0, #0x10]
	ldrh r1, [r0, #0x10]
	adds r0, r5, #0
	movs r2, #0x28
	movs r3, #1
	bl sub_801A03C
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0x46
	movs r3, #0
	bl sub_801A03C
	ldr r0, [r4]
	ldr r0, [r0, #0x10]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #0x10
	movs r3, #0x8c
	lsls r3, r3, #0xf
	adds r1, r1, r3
	lsrs r1, r1, #0x10
	adds r0, r5, #0
	movs r2, #0x64
	movs r3, #1
	bl sub_801A03C
_08019DBC:
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #0
	bl sub_803AD84
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #1
	bl sub_801A7AC
	b _08019EB0
	.align 2, 0
_08019DDC: .4byte gUnknown_03001308
_08019DE0:
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #6
	b _08019E18
_08019DF2:
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #1
	b _08019E18
_08019E04:
	movs r0, #0
	str r0, [r5, #0x28]
_08019E08:
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #4
_08019E18:
	bl sub_803AD84
_08019E1C:
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #0
	bl sub_801A7AC
	b _08019EB0
_08019E28:
	ldr r0, [r5, #0x2c]
	ldr r0, [r0, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #2
	bl sub_803AD80
	movs r0, #0xd2
	str r0, [r5, #0x20]
	b _08019EB0
_08019E42:
	ldr r0, [r5, #0x2c]
	ldr r0, [r0, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #1
	bl sub_803AD80
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #2
	bl sub_803AD84
	b _08019EB0
_08019E6C:
	movs r0, #0x64
	str r0, [r5, #0x20]
	ldr r0, [r5, #0x10]
	cmp r0, #2
	ble _08019E7A
	movs r0, #1
	str r0, [r5, #0x20]
_08019E7A:
	ldr r0, _08019EB8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x15
	bl PlaySfx
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r3, [r6, #0xc]
	ands r0, r3
	strb r0, [r6, #0xc]
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #2
	bl sub_801A7AC
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #1
	bl sub_803AD84
_08019EB0:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08019EB8: .4byte gUnknown_030012BC

	thumb_func_start sub_8019EBC
sub_8019EBC: @ 0x08019EBC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r0
	adds r7, r1, #0
	adds r1, r2, #0
	adds r2, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	ldr r0, _08019F04 @ =0x0000FFFF
	movs r3, #0
	bl sub_8009ED0
	adds r6, r0, #0
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r1, [r6, #0xc]
	ands r0, r1
	strb r0, [r6, #0xc]
	ldr r0, _08019F08 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r2, #0xa2
	lsls r2, r2, #2
	adds r0, r0, r2
	str r0, [r6, #0x20]
	cmp r7, #0
	beq _08019F0C
	cmp r7, #1
	beq _08019F54
	movs r5, #0
	b _08019F92
	.align 2, 0
_08019F04: .4byte 0x0000FFFF
_08019F08: .4byte gUnknown_030012D0
_08019F0C:
	movs r5, #1
	adds r4, r6, #0
	adds r4, #0x28
	movs r1, #1
	movs r0, #4
	rsbs r0, r0, #0
	ldrb r3, [r4]
	ands r0, r3
	orrs r0, r1
	strb r0, [r4]
	movs r0, #3
	adds r1, r6, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r6, #0
	bl sub_80087C0
	adds r0, r6, #0
	bl sub_80087B4
	adds r0, r6, #0
	movs r1, #0
	bl sub_800872C
	strb r5, [r6, #0xa]
	movs r0, #0x28
	bl sub_8026EDC
	bl sub_801A794
	adds r5, r0, #0
	ldr r0, [sp, #0x18]
	str r0, [r5, #0x24]
	mov r1, r8
	str r6, [r1, #0x2c]
	b _08019F96
_08019F54:
	ldr r0, _0801A018 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x29
	bl PlaySfx
	movs r0, #7
	adds r1, r6, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r6, #0
	bl sub_80087C0
	adds r0, r6, #0
	bl sub_80087B4
	adds r0, r6, #0
	movs r1, #0
	bl sub_800872C
	movs r0, #4
	strb r0, [r6, #0xa]
	movs r0, #0x20
	bl sub_8026EDC
	bl sub_801A768
	adds r5, r0, #0
	ldr r2, [sp, #0x18]
	str r2, [r5, #0x1c]
_08019F92:
	adds r4, r6, #0
	adds r4, #0x28
_08019F96:
	adds r0, r6, #0
	bl sub_800815C
	adds r2, r6, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	str r5, [r6, #0x44]
	ldr r1, [r5, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x1c]
	adds r1, r6, #0
	bl sub_803AD80
	ldr r0, _0801A01C @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	ldr r3, [r0, #0xc]
	ldrh r1, [r1]
	adds r3, r1, r3
	ldrb r5, [r3]
	lsrs r0, r5, #1
	movs r2, #1
	eors r0, r2
	ands r0, r2
	ands r0, r2
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r5, [r4]
	ands r1, r5
	orrs r1, r0
	strb r1, [r4]
	ldrb r3, [r3]
	lsrs r0, r3, #2
	ands r0, r2
	ands r0, r2
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r4]
	movs r0, #0x10
	ldrb r1, [r6, #0xc]
	orrs r0, r1
	strb r0, [r6, #0xc]
	cmp r7, #0
	bne _0801A024
	ldr r0, _0801A020 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r6, #0
	bl sub_8008E94
	b _0801A02E
	.align 2, 0
_0801A018: .4byte gUnknown_030012BC
_0801A01C: .4byte gUnknown_030012B4
_0801A020: .4byte gUnknown_030012EC
_0801A024:
	ldr r0, _0801A038 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r6, #0
	bl sub_8008E94
_0801A02E:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801A038: .4byte gUnknown_030012F0

	thumb_func_start sub_801A03C
sub_801A03C: @ 0x0801A03C
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r6, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r6, r6, #0x18
	lsrs r6, r6, #0x18
	ldr r0, _0801A108 @ =0x0000FFFF
	movs r3, #0
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _0801A10C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x30
	str r0, [r4, #0x20]
	movs r0, #1
	adds r1, r4, #0
	adds r1, #0x2d
	movs r2, #1
	mov r8, r2
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	movs r0, #6
	strb r0, [r4, #0xa]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_801A724
	adds r5, r0, #0
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	str r5, [r4, #0x44]
	ldr r1, [r5, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	adds r1, r4, #0
	adds r1, #0x28
	mov r3, r8
	ands r6, r3
	lsls r6, r6, #4
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r2, [r1]
	ands r0, r2
	orrs r0, r6
	strb r0, [r1]
	movs r0, #0x10
	ldrb r3, [r4, #0xc]
	orrs r0, r3
	strb r0, [r4, #0xc]
	ldr r1, [r5, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r5, r5, r0
	ldr r2, [r1, #0x1c]
	adds r0, r5, #0
	adds r1, r4, #0
	bl sub_803AD80
	ldr r0, _0801A110 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801A108: .4byte 0x0000FFFF
_0801A10C: .4byte gUnknown_030012D0
_0801A110: .4byte gUnknown_030012F0

	thumb_func_start sub_801A114
sub_801A114: @ 0x0801A114
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x30
	adds r7, r0, #0
	mov sb, r1
	ldr r1, [r1, #0x18]
	movs r2, #0x28
	ldrsh r0, [r1, r2]
	add r0, sb
	ldr r1, [r1, #0x2c]
	bl sub_803AD7C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801A1A6
	ldr r3, _0801A1B8 @ =gUnknown_030012D8
	mov sl, r3
	ldr r0, [r3]
	movs r1, #0x82
	lsls r1, r1, #1
	adds r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0
	bne _0801A1A6
	mov r0, sp
	mov r1, sb
	bl sub_8007C30
	add r2, sp, #0x10
	mov r8, r2
	mov r3, sl
	ldr r1, [r3]
	mov r0, r8
	bl sub_8007CF8
	ldr r0, [sp, #0x18]
	cmp r0, #0
	bne _0801A17C
	mov r0, sl
	ldr r1, [r0]
	add r4, sp, #0x20
	adds r0, r4, #0
	bl sub_8007C30
	mov r0, r8
	ldm r4!, {r1, r2, r3}
	stm r0!, {r1, r2, r3}
	ldr r1, [r4]
	str r1, [r0]
_0801A17C:
	mov r0, r8
	mov r1, sp
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801A1A6
	mov r1, sl
	ldr r0, [r1]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	mov r3, sb
	ldrb r2, [r3, #0xa]
	ldr r4, [r1, #4]
	movs r1, #0
	movs r3, #0
	bl sub_803AD88
_0801A1A6:
	ldr r0, [r7, #8]
	cmp r0, #5
	bhi _0801A298
	lsls r0, r0, #2
	ldr r1, _0801A1BC @ =_0801A1C0
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801A1B8: .4byte gUnknown_030012D8
_0801A1BC: .4byte _0801A1C0
_0801A1C0: @ jump table
	.4byte _0801A1D8 @ case 0
	.4byte _0801A220 @ case 1
	.4byte _0801A23A @ case 2
	.4byte _0801A254 @ case 3
	.4byte _0801A254 @ case 4
	.4byte _0801A298 @ case 5
_0801A1D8:
	movs r5, #0x10
	movs r0, #0x80
	lsls r0, r0, #1
	orrs r5, r0
	movs r0, #0x80
	lsls r0, r0, #2
	orrs r5, r0
	movs r0, #0x80
	lsls r0, r0, #3
	orrs r5, r0
	movs r0, #0x80
	lsls r0, r0, #4
	orrs r5, r0
	movs r0, #0x80
	lsls r0, r0, #5
	orrs r0, r5
	movs r1, #0x80
	lsls r1, r1, #0xd
	orrs r0, r1
	movs r1, #0x80
	lsls r1, r1, #0x15
	orrs r0, r1
	ldr r1, _0801A21C @ =0x04000050
	str r0, [r1]
	ldr r1, [r7, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r7, r0
	ldr r2, [r1, #0x24]
	movs r1, #5
	bl sub_803AD80
	b _0801A298
	.align 2, 0
_0801A21C: .4byte 0x04000050
_0801A220:
	movs r0, #2
	str r0, [r7, #0x20]
	movs r0, #0
	str r0, [r7, #0x1c]
	ldr r1, [r7, #0xc]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r7, r0
	ldr r2, [r1, #0x24]
	movs r1, #3
	bl sub_803AD80
	b _0801A298
_0801A23A:
	movs r0, #2
	str r0, [r7, #0x20]
	movs r0, #0
	str r0, [r7, #0x1c]
	ldr r1, [r7, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r7, r0
	ldr r2, [r1, #0x24]
	movs r1, #4
	bl sub_803AD80
	b _0801A298
_0801A254:
	ldr r0, [r7, #0x1c]
	cmp r0, #0
	bne _0801A294
	movs r0, #0x14
	str r0, [r7, #0x1c]
	mov r3, sb
	ldrb r2, [r3, #0xd]
	lsrs r1, r2, #2
	movs r0, #1
	eors r1, r0
	ands r1, r0
	lsls r1, r1, #2
	movs r0, #5
	rsbs r0, r0, #0
	ands r0, r2
	orrs r0, r1
	strb r0, [r3, #0xd]
	ldr r0, [r7, #0x20]
	cmp r0, #0
	bne _0801A28C
	ldr r1, [r7, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r7, r0
	ldr r2, [r1, #0x24]
	movs r1, #5
	bl sub_803AD80
_0801A28C:
	ldr r0, [r7, #0x20]
	subs r0, #1
	str r0, [r7, #0x20]
	ldr r0, [r7, #0x1c]
_0801A294:
	subs r0, #1
	str r0, [r7, #0x1c]
_0801A298:
	add sp, #0x30
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_801A2A8
sub_801A2A8: @ 0x0801A2A8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x30
	adds r5, r0, #0
	adds r6, r1, #0
	mov r0, sp
	bl sub_8007C30
	ldr r0, [sp, #8]
	cmp r0, #0
	bne _0801A2C2
	b _0801A3CE
_0801A2C2:
	ldr r0, [r5, #8]
	cmp r0, #4
	beq _0801A338
	cmp r0, #6
	beq _0801A338
	ldr r2, [r5, #0x1c]
	ldrb r1, [r2, #0xc]
	lsrs r0, r1, #6
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _0801A338
	add r4, sp, #0x10
	adds r0, r4, #0
	adds r1, r2, #0
	bl sub_8007B98
	mov r0, sp
	adds r1, r4, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801A338
	ldr r0, [r5, #0x1c]
	ldr r0, [r0, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #7
	bl sub_803AD80
	ldr r1, [r5, #0xc]
	movs r4, #0x20
	ldrsh r0, [r1, r4]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #6
	bl sub_803AD80
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #8
	bl sub_803AD84
	ldr r0, _0801A3E0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x39
	bl PlaySfx
_0801A338:
	ldr r3, _0801A3E4 @ =gUnknown_030012D8
	mov r8, r3
	ldr r1, [r3]
	movs r4, #0x82
	lsls r4, r4, #1
	adds r0, r1, r4
	ldrb r0, [r0]
	cmp r0, #0
	bne _0801A3CE
	add r7, sp, #0x10
	adds r0, r7, #0
	bl sub_8007CF8
	ldr r0, [sp, #0x18]
	cmp r0, #0
	bne _0801A36E
	mov r0, r8
	ldr r1, [r0]
	add r4, sp, #0x20
	adds r0, r4, #0
	bl sub_8007C30
	adds r0, r7, #0
	ldm r4!, {r1, r2, r3}
	stm r0!, {r1, r2, r3}
	ldr r1, [r4]
	str r1, [r0]
_0801A36E:
	mov r0, sp
	adds r1, r7, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801A3CE
	mov r4, r8
	ldr r0, [r4]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldrb r2, [r6, #0xa]
	ldr r4, [r1, #4]
	movs r1, #0
	movs r3, #0
	bl sub_803AD88
	ldr r0, [r5, #8]
	cmp r0, #3
	bne _0801A3CE
	ldr r1, [r5, #0xc]
	movs r4, #0x20
	ldrsh r0, [r1, r4]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #6
	bl sub_803AD80
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #8
	bl sub_803AD84
	ldr r0, _0801A3E0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x39
	bl PlaySfx
_0801A3CE:
	ldr r0, [r5, #8]
	cmp r0, #6
	bls _0801A3D6
	b _0801A56E
_0801A3D6:
	lsls r0, r0, #2
	ldr r1, _0801A3E8 @ =_0801A3EC
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801A3E0: .4byte gUnknown_030012BC
_0801A3E4: .4byte gUnknown_030012D8
_0801A3E8: .4byte _0801A3EC
_0801A3EC: @ jump table
	.4byte _0801A408 @ case 0
	.4byte _0801A430 @ case 1
	.4byte _0801A532 @ case 2
	.4byte _0801A4D4 @ case 3
	.4byte _0801A4D4 @ case 4
	.4byte _0801A498 @ case 5
	.4byte _0801A528 @ case 6
_0801A408:
	ldr r1, [r5, #0xc]
	movs r3, #0x30
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r2, _0801A42C @ =gStaticData_0816C3E8
	ldr r3, [r1, #0x34]
	adds r1, r6, #0
	bl sub_803AD84
	ldr r1, [r5, #0xc]
	movs r4, #0x20
	ldrsh r0, [r1, r4]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #1
	bl sub_803AD80
	b _0801A56E
	.align 2, 0
_0801A42C: .4byte gStaticData_0816C3E8
_0801A430:
	ldr r1, [r6, #4]
	movs r0, #0x80
	lsls r0, r0, #4
	cmp r1, r0
	ble _0801A43C
	b _0801A56E
_0801A43C:
	movs r0, #0
	str r0, [r6, #0x64]
	str r0, [r6, #0x54]
	str r0, [r6, #0x58]
	str r0, [r6, #0x5c]
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #8
	bl sub_803AD84
	adds r0, r6, #0
	bl sub_800815C
	adds r2, r6, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldr r1, [r6]
	lsls r1, r1, #8
	lsrs r1, r1, #0x10
	ldr r2, [r6, #4]
	lsls r2, r2, #8
	lsrs r2, r2, #0x10
	adds r0, r5, #0
	bl sub_801A584
	ldr r1, [r5, #0xc]
	movs r4, #0x20
	ldrsh r0, [r1, r4]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #2
	bl sub_803AD80
	b _0801A56E
_0801A498:
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #9
	bl sub_803AD84
	ldr r1, [r5, #0xc]
	movs r3, #0x30
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r2, _0801A4D0 @ =gStaticData_0816C3F4
	ldr r3, [r1, #0x34]
	adds r1, r6, #0
	bl sub_803AD84
	ldr r1, [r5, #0xc]
	movs r4, #0x20
	ldrsh r0, [r1, r4]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #3
	bl sub_803AD80
	b _0801A56E
	.align 2, 0
_0801A4D0: .4byte gStaticData_0816C3F4
_0801A4D4:
	ldr r1, [r6, #4]
	ldr r0, _0801A51C @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x14]
	lsls r0, r0, #8
	ldr r2, _0801A520 @ =0xFFFFE000
	adds r0, r0, r2
	cmp r1, r0
	blt _0801A56E
	ldr r1, [r5, #0xc]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #6
	bl sub_803AD80
	ldr r1, [r5, #0xc]
	adds r1, #0x50
	movs r4, #0
	ldrsh r0, [r1, r4]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	movs r2, #8
	bl sub_803AD84
	ldr r0, _0801A524 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x39
	bl PlaySfx
	b _0801A56E
	.align 2, 0
_0801A51C: .4byte gUnknown_03001308
_0801A520: .4byte 0xFFFFE000
_0801A524: .4byte gUnknown_030012BC
_0801A528:
	movs r0, #0
	str r0, [r6, #0x64]
	str r0, [r6, #0x54]
	str r0, [r6, #0x58]
	str r0, [r6, #0x5c]
_0801A532:
	adds r0, r6, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801A56E
	movs r0, #1
	ldrb r1, [r6, #0xc]
	orrs r0, r1
	strb r0, [r6, #0xc]
	ldr r0, _0801A57C @ =0x0000FFFF
	ldrh r2, [r6, #8]
	cmp r2, r0
	beq _0801A56E
	ldrh r3, [r6, #8]
	ldr r0, _0801A580 @ =gUnknown_030012B4
	ldr r2, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r1, r0, #2
	movs r4, #0x84
	lsls r4, r4, #1
	adds r2, r2, r4
	adds r2, r2, r1
	lsls r0, r0, #5
	subs r0, r3, r0
	movs r1, #1
	lsls r1, r0
	ldr r0, [r2]
	orrs r0, r1
	str r0, [r2]
_0801A56E:
	add sp, #0x30
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801A57C: .4byte 0x0000FFFF
_0801A580: .4byte gUnknown_030012B4

	thumb_func_start sub_801A584
sub_801A584: @ 0x0801A584
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	ldr r0, _0801A63C @ =0x0000FFFF
	movs r3, #0
	bl sub_8009ED0
	adds r5, r0, #0
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r1, [r5, #0xc]
	ands r0, r1
	strb r0, [r5, #0xc]
	ldr r0, _0801A640 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r2, #0xa2
	lsls r2, r2, #2
	adds r0, r0, r2
	str r0, [r5, #0x20]
	movs r0, #8
	adds r1, r5, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r5, #0
	bl sub_80087C0
	adds r0, r5, #0
	bl sub_80087B4
	adds r0, r5, #0
	movs r1, #0
	bl sub_800872C
	movs r0, #1
	strb r0, [r5, #0xa]
	movs r0, #0x20
	bl sub_8026EDC
	adds r4, r0, #0
	bl sub_8017A8C
	ldr r1, _0801A644 @ =gStaticData_087E48A4
	str r1, [r4, #0xc]
	ldr r0, [r6, #0x1c]
	str r0, [r4, #0x1c]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #5
	bl sub_803AD80
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	str r4, [r5, #0x44]
	ldr r1, [r4, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r4, r4, r0
	ldr r2, [r1, #0x1c]
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #0x10
	ldrb r3, [r5, #0xc]
	orrs r0, r3
	strb r0, [r5, #0xc]
	ldr r0, _0801A648 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801A63C: .4byte 0x0000FFFF
_0801A640: .4byte gUnknown_030012D0
_0801A644: .4byte gStaticData_087E48A4
_0801A648: .4byte gUnknown_030012F0

	thumb_func_start sub_801A64C
sub_801A64C: @ 0x0801A64C
	push {r4, r5, lr}
	adds r3, r1, #0
	ldr r4, [r0, #8]
	cmp r4, #0
	beq _0801A65C
	cmp r4, #1
	beq _0801A69C
	b _0801A712
_0801A65C:
	adds r0, r3, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _0801A684
	ldr r1, _0801A680 @ =gStaticData_0816C3B8
	ldr r0, [r1, #0x24]
	rsbs r0, r0, #0
	ldr r2, [r1, #0x2c]
	rsbs r2, r2, #0
	ldr r1, [r1, #0x28]
	str r0, [r3, #0x60]
	str r0, [r3, #0x48]
	str r1, [r3, #0x4c]
	str r2, [r3, #0x50]
	b _0801A712
	.align 2, 0
_0801A680: .4byte gStaticData_0816C3B8
_0801A684:
	ldr r0, _0801A698 @ =gStaticData_0816C3B8
	ldr r1, [r0, #0x24]
	ldr r2, [r0, #0x28]
	ldr r0, [r0, #0x2c]
	str r1, [r3, #0x60]
	str r1, [r3, #0x48]
	str r2, [r3, #0x4c]
	str r0, [r3, #0x50]
	b _0801A712
	.align 2, 0
_0801A698: .4byte gStaticData_0816C3B8
_0801A69C:
	adds r0, r3, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _0801A6CC
	ldr r0, [r3]
	movs r1, #0xa0
	lsls r1, r1, #6
	adds r0, r0, r1
	cmp r0, #0
	bgt _0801A712
	movs r0, #1
	ldrb r2, [r3, #0xc]
	orrs r0, r2
	strb r0, [r3, #0xc]
	ldr r0, _0801A6C8 @ =0x0000FFFF
	ldrh r5, [r3, #8]
	cmp r5, r0
	beq _0801A712
	b _0801A6F2
	.align 2, 0
_0801A6C8: .4byte 0x0000FFFF
_0801A6CC:
	ldr r1, [r3]
	ldr r0, _0801A718 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x10]
	lsls r0, r0, #8
	movs r2, #0xa0
	lsls r2, r2, #6
	adds r0, r0, r2
	cmp r1, r0
	blt _0801A712
	movs r0, #1
	ldrb r5, [r3, #0xc]
	orrs r0, r5
	strb r0, [r3, #0xc]
	ldr r0, _0801A71C @ =0x0000FFFF
	ldrh r1, [r3, #8]
	cmp r1, r0
	beq _0801A712
_0801A6F2:
	ldrh r3, [r3, #8]
	ldr r0, _0801A720 @ =gUnknown_030012B4
	ldr r1, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r2, r0, #2
	movs r5, #0x84
	lsls r5, r5, #1
	adds r1, r1, r5
	adds r1, r1, r2
	lsls r0, r0, #5
	subs r0, r3, r0
	lsls r4, r0
	ldr r0, [r1]
	orrs r0, r4
	str r0, [r1]
_0801A712:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0801A718: .4byte gUnknown_03001308
_0801A71C: .4byte 0x0000FFFF
_0801A720: .4byte gUnknown_030012B4

	thumb_func_start sub_801A724
sub_801A724: @ 0x0801A724
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800CA74
	ldr r0, _0801A738 @ =gStaticData_087E483C
	str r0, [r4, #0xc]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801A738: .4byte gStaticData_087E483C

	thumb_func_start sub_801A73C
sub_801A73C: @ 0x0801A73C
	push {lr}
	ldr r2, _0801A74C @ =gStaticData_087E483C
	str r2, [r0, #0xc]
	bl sub_800CA60
	pop {r0}
	bx r0
	.align 2, 0
_0801A74C: .4byte gStaticData_087E483C

	thumb_func_start sub_801A750
sub_801A750: @ 0x0801A750
	push {lr}
	ldr r2, _0801A764 @ =gStaticData_087E48A4
	str r2, [r0, #0xc]
	movs r2, #0
	str r2, [r0, #0x1c]
	bl sub_8017A78
	pop {r0}
	bx r0
	.align 2, 0
_0801A764: .4byte gStaticData_087E48A4

	thumb_func_start sub_801A768
sub_801A768: @ 0x0801A768
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8017A8C
	ldr r0, _0801A77C @ =gStaticData_087E48A4
	str r0, [r4, #0xc]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801A77C: .4byte gStaticData_087E48A4

	thumb_func_start sub_801A780
sub_801A780: @ 0x0801A780
	push {lr}
	ldr r2, _0801A790 @ =gStaticData_087E490C
	str r2, [r0, #0xc]
	bl sub_8017A78
	pop {r0}
	bx r0
	.align 2, 0
_0801A790: .4byte gStaticData_087E490C

	thumb_func_start sub_801A794
sub_801A794: @ 0x0801A794
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8017A8C
	ldr r0, _0801A7A8 @ =gStaticData_087E490C
	str r0, [r4, #0xc]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801A7A8: .4byte gStaticData_087E490C

	thumb_func_start sub_801A7AC
sub_801A7AC: @ 0x0801A7AC
	push {r4, r5, r6, r7, lr}
	adds r3, r1, #0
	adds r5, r2, #0
	ldr r2, _0801A7E8 @ =gStaticData_0816C418
	lsls r0, r5, #3
	adds r0, r0, r2
	ldr r1, [r0]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801A7EC @ =gStaticData_0816C3B8
	adds r4, r0, r1
	adds r0, r3, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	adds r6, r2, #0
	adds r7, r1, #0
	cmp r0, #0
	bge _0801A7F0
	ldr r0, [r4]
	rsbs r0, r0, #0
	ldr r1, [r4, #8]
	rsbs r1, r1, #0
	ldr r2, [r4, #4]
	str r0, [r3, #0x60]
	str r0, [r3, #0x48]
	str r2, [r3, #0x4c]
	str r1, [r3, #0x50]
	b _0801A7FE
	.align 2, 0
_0801A7E8: .4byte gStaticData_0816C418
_0801A7EC: .4byte gStaticData_0816C3B8
_0801A7F0:
	ldr r0, [r4]
	ldr r1, [r4, #4]
	ldr r2, [r4, #8]
	str r0, [r3, #0x60]
	str r0, [r3, #0x48]
	str r1, [r3, #0x4c]
	str r2, [r3, #0x50]
_0801A7FE:
	lsls r0, r5, #3
	adds r1, r6, #4
	adds r0, r0, r1
	ldr r1, [r0]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r7
	ldr r1, [r0]
	ldr r2, [r0, #4]
	ldr r0, [r0, #8]
	str r1, [r3, #0x64]
	str r1, [r3, #0x54]
	str r2, [r3, #0x58]
	str r0, [r3, #0x5c]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801A824
sub_801A824: @ 0x0801A824
	push {lr}
	ldr r2, _0801A834 @ =gStaticData_087E4974
	str r2, [r0, #0xc]
	bl sub_8017A78
	pop {r0}
	bx r0
	.align 2, 0
_0801A834: .4byte gStaticData_087E4974

	thumb_func_start sub_801A838
sub_801A838: @ 0x0801A838
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r6, r0, #0
	adds r4, r1, #0
	adds r5, r2, #0
	bl sub_8017A8C
	ldr r0, _0801A86C @ =gStaticData_087E4974
	str r0, [r6, #0xc]
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	movs r0, #0
	str r0, [sp]
	adds r0, r6, #0
	movs r1, #0
	adds r2, r4, #0
	adds r3, r5, #0
	bl sub_8019EBC
	adds r0, r6, #0
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0801A86C: .4byte gStaticData_087E4974

	thumb_func_start sub_801A870
sub_801A870: @ 0x0801A870
	str r1, [r0, #0x1c]
	bx lr

	thumb_func_start sub_801A874
sub_801A874: @ 0x0801A874
	str r1, [r0, #0x24]
	bx lr

	thumb_func_start sub_801A878
sub_801A878: @ 0x0801A878
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	mov sb, r0
	adds r5, r1, #0
	adds r6, r2, #0
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	mov sb, r0
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	lsls r6, r6, #0x10
	lsrs r6, r6, #0x10
	mov r1, r8
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	mov r8, r1
	movs r0, #0x80
	bl sub_8026EDC
	adds r4, r0, #0
	bl sub_8009F90
	ldr r0, _0801A8F4 @ =gStaticData_087E49DC
	str r0, [r4, #0x18]
	adds r0, r4, #0
	bl sub_801B2D8
	adds r7, r4, #0
	mov r2, sb
	strh r2, [r7, #8]
	lsls r5, r5, #8
	str r5, [r7]
	lsls r6, r6, #8
	str r6, [r7, #4]
	ldr r0, _0801A8F8 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r3, r8
	lsls r3, r3, #1
	mov r8, r3
	add r8, r0
	ldr r0, [r1, #0xc]
	mov r4, r8
	ldrh r5, [r4]
	adds r5, r5, r0
	mov r8, r5
	ldr r2, [r5, #4]
	ldr r0, [sp, #0x24]
	subs r0, #3
	cmp r0, #9
	bhi _0801A93A
	lsls r0, r0, #2
	ldr r1, _0801A8FC @ =_0801A900
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801A8F4: .4byte gStaticData_087E49DC
_0801A8F8: .4byte gUnknown_030012B4
_0801A8FC: .4byte _0801A900
_0801A900: @ jump table
	.4byte _0801A934 @ case 0
	.4byte _0801A928 @ case 1
	.4byte _0801A92C @ case 2
	.4byte _0801A938 @ case 3
	.4byte _0801A93A @ case 4
	.4byte _0801A930 @ case 5
	.4byte _0801A934 @ case 6
	.4byte _0801A934 @ case 7
	.4byte _0801A934 @ case 8
	.4byte _0801A934 @ case 9
_0801A928:
	movs r2, #2
	b _0801A93A
_0801A92C:
	movs r2, #3
	b _0801A93A
_0801A930:
	movs r2, #7
	b _0801A93A
_0801A934:
	movs r2, #4
	b _0801A93A
_0801A938:
	movs r2, #6
_0801A93A:
	cmp r2, #7
	bls _0801A940
	b _0801AA9C
_0801A940:
	lsls r0, r2, #2
	ldr r1, _0801A94C @ =_0801A950
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801A94C: .4byte _0801A950
_0801A950: @ jump table
	.4byte _0801A970 @ case 0
	.4byte _0801A976 @ case 1
	.4byte _0801A9D4 @ case 2
	.4byte _0801A9DA @ case 3
	.4byte _0801A9EE @ case 4
	.4byte _0801A9F4 @ case 5
	.4byte _0801AA1C @ case 6
	.4byte _0801AA6C @ case 7
_0801A970:
	movs r0, #0
	str r0, [r7, #0x78]
	b _0801AA9C
_0801A976:
	movs r6, #1
	str r6, [r7, #0x78]
	movs r0, #0x38
	bl sub_8026EDC
	mov r2, r8
	ldr r1, [r2, #8]
	ldr r2, [r2, #0xc]
	mov r3, r8
	movs r5, #0x10
	ldrsh r4, [r3, r5]
	rsbs r3, r4, #0
	orrs r3, r4
	lsrs r3, r3, #0x1f
	mov sb, r3
	mov r3, r8
	movs r4, #0x12
	ldrsh r5, [r3, r4]
	rsbs r4, r5, #0
	orrs r4, r5
	lsrs r4, r4, #0x1f
	mov r5, sp
	strb r4, [r5]
	str r6, [sp, #4]
	mov r3, sb
	bl sub_801B7D8
	adds r2, r0, #0
	str r2, [r7, #0x44]
	ldr r1, [r2, #0xc]
	movs r5, #0x18
	ldrsh r0, [r1, r5]
	adds r0, r2, r0
	ldr r2, [r1, #0x1c]
	adds r1, r7, #0
	bl sub_803AD80
	mov r1, r8
	movs r2, #0x14
	ldrsh r0, [r1, r2]
	cmp r0, #0
	beq _0801AA9C
	movs r0, #0x10
	ldrb r3, [r7, #0xc]
	orrs r0, r3
	strb r0, [r7, #0xc]
	b _0801AA9C
_0801A9D4:
	movs r0, #2
	str r0, [r7, #0x78]
	b _0801AA9C
_0801A9DA:
	movs r0, #3
	str r0, [r7, #0x78]
	movs r1, #1
	mov r4, r8
	ldrb r4, [r4]
	ands r1, r4
	adds r0, r7, #0
	bl sub_801B2A8
	b _0801AA9C
_0801A9EE:
	movs r0, #4
	str r0, [r7, #0x78]
	b _0801AA9C
_0801A9F4:
	movs r4, #5
	str r4, [r7, #0x78]
	movs r0, #0x38
	bl sub_8026EDC
	mov r2, sp
	movs r1, #0
	strb r1, [r2]
	str r4, [sp, #4]
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_801B7D8
	adds r2, r0, #0
	str r2, [r7, #0x44]
	ldr r1, [r2, #0xc]
	movs r5, #0x18
	ldrsh r0, [r1, r5]
	b _0801AA60
_0801AA1C:
	movs r4, #6
	str r4, [r7, #0x78]
	ldr r0, _0801AA48 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80233B4
	cmp r0, #1
	beq _0801AA4C
	movs r0, #0x38
	bl sub_8026EDC
	mov r2, sp
	movs r1, #0
	strb r1, [r2]
	str r4, [sp, #4]
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_801B7D8
	b _0801AA56
	.align 2, 0
_0801AA48: .4byte gUnknown_030012C0
_0801AA4C:
	movs r0, #0x38
	bl sub_8026EDC
	bl sub_801961C
_0801AA56:
	adds r2, r0, #0
	str r2, [r7, #0x44]
	ldr r1, [r2, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
_0801AA60:
	adds r0, r2, r0
	ldr r2, [r1, #0x1c]
	adds r1, r7, #0
	bl sub_803AD80
	b _0801AA9C
_0801AA6C:
	movs r4, #7
	str r4, [r7, #0x78]
	movs r0, #0x38
	bl sub_8026EDC
	mov r2, sp
	movs r1, #0
	strb r1, [r2]
	str r4, [sp, #4]
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_801B7D8
	adds r2, r0, #0
	str r2, [r7, #0x44]
	ldr r1, [r2, #0xc]
	movs r4, #0x18
	ldrsh r0, [r1, r4]
	adds r0, r2, r0
	ldr r2, [r1, #0x1c]
	adds r1, r7, #0
	bl sub_803AD80
_0801AA9C:
	ldr r0, _0801AB28 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r7, #0
	bl sub_8008E94
	ldr r0, _0801AB2C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r5, #0xea
	lsls r5, r5, #1
	adds r0, r0, r5
	str r0, [r7, #0x20]
	adds r4, r7, #0
	adds r4, #0x2d
	add r0, sp, #0x24
	ldrb r0, [r0]
	strb r0, [r4]
	adds r0, r7, #0
	bl sub_80087C0
	adds r0, r7, #0
	bl sub_80087B4
	adds r0, r7, #0
	movs r1, #0
	bl sub_800872C
	adds r2, r7, #0
	adds r2, #0x28
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r1, [r2]
	ands r0, r1
	movs r1, #0x21
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r2]
	ldr r0, [r7, #0x20]
	ldr r1, [r0]
	ldrb r2, [r4]
	lsls r0, r2, #3
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r1, r1, r0
	ldr r0, _0801AB30 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldrb r1, [r1, #0x14]
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	adds r2, r7, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	adds r0, r7, #0
	add sp, #8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0801AB28: .4byte gUnknown_030012EC
_0801AB2C: .4byte gUnknown_030012D0
_0801AB30: .4byte gUnknown_030012B8

	thumb_func_start sub_801AB34
sub_801AB34: @ 0x0801AB34
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x78]
	cmp r0, #6
	bne _0801AB44
	ldr r0, [r4, #0x30]
	cmp r0, #0x12
	bgt _0801AB88
_0801AB44:
	ldr r0, _0801AB90 @ =gUnknown_030012D8
	ldr r3, [r0]
	ldr r0, [r3, #0x44]
	ldr r5, [r0, #8]
	ldrb r1, [r3, #0xc]
	lsrs r0, r1, #7
	cmp r0, #0
	beq _0801AB7E
	ldr r2, [r4]
	ldr r0, [r3]
	subs r2, r2, r0
	cmp r2, #0
	bge _0801AB60
	rsbs r2, r2, #0
_0801AB60:
	ldr r1, _0801AB94 @ =0x00007FFF
	cmp r2, r1
	bgt _0801AB7E
	ldr r2, [r4, #4]
	ldr r0, [r3, #4]
	subs r2, r2, r0
	cmp r2, #0
	bge _0801AB72
	rsbs r2, r2, #0
_0801AB72:
	cmp r2, r1
	bgt _0801AB7E
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_801AB98
_0801AB7E:
	movs r0, #9
	rsbs r0, r0, #0
	ldrb r1, [r4, #0xc]
	ands r0, r1
	strb r0, [r4, #0xc]
_0801AB88:
	movs r0, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0801AB90: .4byte gUnknown_030012D8
_0801AB94: .4byte 0x00007FFF

	thumb_func_start sub_801AB98
sub_801AB98: @ 0x0801AB98
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x44
	mov sb, r0
	add r0, sp, #4
	mov r1, sb
	bl sub_8007B98
	ldr r0, _0801AC3C @ =gUnknown_030012D8
	ldr r1, [r0]
	ldr r0, [r1]
	asrs r5, r0, #8
	ldr r0, [r1, #4]
	asrs r0, r0, #8
	mov sl, r0
	add r4, sp, #0x14
	adds r0, r4, #0
	bl sub_8007B98
	ldr r1, _0801AC3C @ =gUnknown_030012D8
	ldr r0, [r1]
	ldr r2, [r0, #0x20]
	adds r0, #0x2d
	ldrb r3, [r0]
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	ldr r0, [r2]
	adds r0, r0, r1
	adds r7, r0, #4
	add r0, sp, #4
	adds r1, r4, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801ABEA
	b _0801B078
_0801ABEA:
	movs r0, #0
	mov r8, r0
	movs r1, #0
	str r1, [sp, #0x40]
	ldr r1, [sp, #0x18]
	ldr r0, [sp, #8]
	cmp r1, r0
	bge _0801ABFE
	movs r2, #1
	str r2, [sp, #0x40]
_0801ABFE:
	ldr r3, _0801AC3C @ =gUnknown_030012D8
	ldr r0, [r3]
	bl sub_8009EC4
	adds r4, r0, #0
	ldr r1, _0801AC3C @ =gUnknown_030012D8
	ldr r0, [r1]
	bl sub_8009EBC
	adds r6, r0, #0
	movs r2, #2
	str r2, [sp, #0x3c]
	cmp r5, r4
	ble _0801AC1E
	movs r3, #1
	str r3, [sp, #0x3c]
_0801AC1E:
	ldr r1, _0801AC3C @ =gUnknown_030012D8
	ldr r0, [r1]
	ldr r1, [r0]
	asrs r1, r1, #8
	mov r2, sb
	ldr r0, [r2]
	asrs r0, r0, #8
	cmp r1, r0
	bge _0801AC40
	movs r3, #1
	str r3, [sp, #0x34]
	ldr r0, [sp, #0x14]
	ldr r1, [sp, #0x1c]
	ldr r2, [sp, #4]
	b _0801AC4A
	.align 2, 0
_0801AC3C: .4byte gUnknown_030012D8
_0801AC40:
	movs r0, #2
	str r0, [sp, #0x34]
	ldr r0, [sp, #4]
	ldr r1, [sp, #0xc]
	ldr r2, [sp, #0x14]
_0801AC4A:
	adds r0, r0, r1
	subs r0, r0, r2
	adds r0, #1
	str r0, [sp, #0x2c]
	ldr r0, _0801AC70 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r1, [r0, #4]
	asrs r1, r1, #8
	mov r2, sb
	ldr r0, [r2, #4]
	asrs r0, r0, #8
	cmp r1, r0
	ble _0801AC74
	movs r3, #4
	str r3, [sp, #0x38]
	ldr r0, [sp, #8]
	ldr r1, [sp, #0x10]
	ldr r2, [sp, #0x18]
	b _0801AC7E
	.align 2, 0
_0801AC70: .4byte gUnknown_030012D8
_0801AC74:
	movs r0, #8
	str r0, [sp, #0x38]
	ldr r0, [sp, #0x18]
	ldr r1, [sp, #0x20]
	ldr r2, [sp, #8]
_0801AC7E:
	adds r0, r0, r1
	subs r0, r0, r2
	str r0, [sp, #0x30]
	mov r1, sb
	ldr r0, [r1, #0x78]
	cmp r0, #1
	beq _0801ACE0
	cmp r0, #5
	beq _0801ACE0
	cmp r0, #6
	beq _0801ACE0
	cmp r6, sl
	bne _0801ACC8
	cmp r4, r5
	bne _0801ACAC
	ldr r2, [sp, #0x34]
	mov r8, r2
	ldr r3, [sp, #0x30]
	cmp r3, #2
	bgt _0801ACE0
	movs r5, #8
	mov r8, r5
	b _0801ACE0
_0801ACAC:
	mov r0, sb
	bl sub_8009EBC
	mov r2, sb
	ldr r1, [r2, #4]
	asrs r1, r1, #8
	cmp r0, r1
	bne _0801ACC8
	ldr r3, [sp, #0x30]
	cmp r3, #2
	ble _0801ACC8
	ldr r5, [sp, #0x34]
	mov r8, r5
	b _0801ACE0
_0801ACC8:
	cmp r4, r5
	bne _0801ACE0
	mov r0, sb
	bl sub_8009EC4
	mov r2, sb
	ldr r1, [r2]
	asrs r1, r1, #8
	cmp r0, r1
	bne _0801ACE0
	ldr r3, [sp, #0x38]
	mov r8, r3
_0801ACE0:
	cmp r6, sl
	bgt _0801ADA8
	ldr r5, [sp, #0x40]
	cmp r5, #0
	beq _0801ADA8
	mov r0, r8
	cmp r0, #0
	beq _0801ACF2
	b _0801AE60
_0801ACF2:
	movs r1, #2
	ldrsh r0, [r7, r1]
	ldrb r2, [r7, #5]
	adds r0, r0, r2
	adds r6, r6, r0
	ldr r0, [sp, #8]
	ldr r1, [sp, #0x10]
	adds r0, r0, r1
	adds r3, r2, #0
	cmp r6, r0
	ble _0801AD0A
	b _0801AE5C
_0801AD0A:
	ldr r2, [sp, #0x3c]
	cmp r2, #1
	bne _0801AD2A
	ldr r1, [sp, #0x14]
	ldr r0, [sp, #0x1c]
	adds r0, r1, r0
	adds r2, r1, #0
	ldr r1, [sp, #4]
	cmp r0, r1
	blt _0801AD40
	ldr r5, [sp, #0x2c]
	cmp r5, #2
	ble _0801AD40
	movs r0, #8
	mov r8, r0
	b _0801AE60
_0801AD2A:
	ldr r1, [sp, #4]
	ldr r0, [sp, #0xc]
	adds r0, r1, r0
	ldr r2, [sp, #0x14]
	cmp r2, r0
	bgt _0801AD40
	ldr r5, [sp, #0x2c]
	cmp r5, #2
	ble _0801AD40
	movs r0, #8
	mov r8, r0
_0801AD40:
	mov r5, r8
	cmp r5, #0
	beq _0801AD48
	b _0801AE60
_0801AD48:
	movs r5, #2
	ldrsh r0, [r7, r5]
	adds r0, r0, r3
	add sl, r0
	ldr r0, [sp, #0x34]
	cmp r0, #1
	bne _0801AD6E
	movs r3, #0
	ldrsh r0, [r7, r3]
	ldrb r7, [r7, #4]
	adds r0, r7, r0
	adds r4, r4, r0
	ldr r0, [sp, #0x1c]
	adds r5, r2, r0
	str r1, [sp]
	adds r0, r4, #0
	adds r1, r6, #0
	adds r2, r5, #0
	b _0801AD80
_0801AD6E:
	movs r5, #0
	ldrsh r0, [r7, r5]
	adds r4, r4, r0
	adds r5, r2, #0
	ldr r0, [sp, #0xc]
	adds r0, r1, r0
	str r0, [sp]
	adds r0, r4, #0
	adds r1, r6, #0
_0801AD80:
	mov r3, sl
	bl sub_800FDC8
	adds r2, r0, #0
	cmp r2, #0
	bge _0801AD98
	ldr r0, [sp, #0x40]
	cmp r0, #0
	beq _0801AD98
	ldr r1, [sp, #0x30]
	cmp r1, #1
	ble _0801ADA2
_0801AD98:
	cmp r2, #0
	ble _0801AE56
	ldr r0, [sp, #8]
	cmp r2, r0
	bgt _0801AE56
_0801ADA2:
	movs r2, #8
	mov r8, r2
	b _0801AE60
_0801ADA8:
	mov r0, r8
	cmp r0, #0
	bne _0801AE60
	movs r1, #2
	ldrsh r0, [r7, r1]
	adds r6, r6, r0
	ldr r0, [sp, #8]
	cmp r6, r0
	blt _0801AE5C
	ldr r2, [sp, #0x3c]
	cmp r2, #1
	bne _0801ADD8
	ldr r1, [sp, #0x14]
	ldr r0, [sp, #0x1c]
	adds r0, r1, r0
	adds r2, r1, #0
	ldr r1, [sp, #4]
	cmp r0, r1
	blt _0801ADEE
	ldr r3, [sp, #0x2c]
	cmp r3, #3
	ble _0801ADEE
	movs r5, #4
	b _0801AE5E
_0801ADD8:
	ldr r1, [sp, #4]
	ldr r0, [sp, #0xc]
	adds r0, r1, r0
	ldr r2, [sp, #0x14]
	cmp r2, r0
	bgt _0801ADEE
	ldr r0, [sp, #0x2c]
	cmp r0, #3
	ble _0801ADEE
	movs r3, #4
	mov r8, r3
_0801ADEE:
	mov r5, r8
	cmp r5, #0
	bne _0801AE60
	movs r3, #2
	ldrsh r0, [r7, r3]
	add sl, r0
	ldr r5, [sp, #0x34]
	cmp r5, #1
	bne _0801AE18
	movs r3, #0
	ldrsh r0, [r7, r3]
	ldrb r7, [r7, #4]
	adds r0, r7, r0
	adds r4, r4, r0
	ldr r0, [sp, #0x1c]
	adds r5, r2, r0
	str r1, [sp]
	adds r0, r4, #0
	adds r1, r6, #0
	adds r2, r5, #0
	b _0801AE2A
_0801AE18:
	movs r5, #0
	ldrsh r0, [r7, r5]
	adds r4, r4, r0
	adds r5, r2, #0
	ldr r0, [sp, #0xc]
	adds r0, r1, r0
	str r0, [sp]
	adds r0, r4, #0
	adds r1, r6, #0
_0801AE2A:
	mov r3, sl
	bl sub_800FDC8
	adds r2, r0, #0
	cmp r2, #0
	bge _0801AE42
	ldr r0, [sp, #0x40]
	cmp r0, #0
	bne _0801AE42
	ldr r1, [sp, #0x2c]
	cmp r1, #3
	bgt _0801AE50
_0801AE42:
	cmp r2, #0
	ble _0801AE56
	ldr r0, [sp, #8]
	ldr r1, [sp, #0x10]
	adds r0, r0, r1
	cmp r2, r0
	blt _0801AE56
_0801AE50:
	movs r2, #4
	mov r8, r2
	b _0801AE60
_0801AE56:
	ldr r3, [sp, #0x34]
	mov r8, r3
	b _0801AE60
_0801AE5C:
	ldr r5, [sp, #0x34]
_0801AE5E:
	mov r8, r5
_0801AE60:
	ldr r2, _0801AE98 @ =gUnknown_030012D8
	ldr r1, [r2]
	ldr r0, [r1]
	str r0, [sp, #0x24]
	ldr r0, [r1, #4]
	add r1, sp, #0x24
	str r0, [r1, #4]
	mov sl, r2
	adds r7, r1, #0
	ldr r0, [sp, #0x2c]
	cmp r0, #0
	bge _0801AE7C
	movs r1, #0
	str r1, [sp, #0x2c]
_0801AE7C:
	ldr r2, [sp, #0x30]
	cmp r2, #0
	bge _0801AE86
	movs r3, #0
	str r3, [sp, #0x30]
_0801AE86:
	movs r5, #0
	mov r0, r8
	cmp r0, #8
	bhi _0801AF3C
	lsls r0, r0, #2
	ldr r1, _0801AE9C @ =_0801AEA0
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801AE98: .4byte gUnknown_030012D8
_0801AE9C: .4byte _0801AEA0
_0801AEA0: @ jump table
	.4byte _0801AF32 @ case 0
	.4byte _0801AF14 @ case 1
	.4byte _0801AF14 @ case 2
	.4byte _0801AF32 @ case 3
	.4byte _0801AEC4 @ case 4
	.4byte _0801AF32 @ case 5
	.4byte _0801AF32 @ case 6
	.4byte _0801AF32 @ case 7
	.4byte _0801AEFC @ case 8
_0801AEC4:
	ldr r0, _0801AEF8 @ =gUnknown_030012D8
	ldr r2, [r0]
	adds r1, r2, #0
	adds r1, #0x68
	movs r0, #8
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0801AF32
	ldr r1, [r2, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xc
	movs r3, #4
	bl sub_803AD88
	ldr r1, [sp, #0x30]
	lsls r0, r1, #8
	ldr r1, [r7, #4]
	adds r0, r0, r1
	str r0, [r7, #4]
	b _0801AF32
	.align 2, 0
_0801AEF8: .4byte gUnknown_030012D8
_0801AEFC:
	ldr r0, [sp, #0x30]
	subs r0, #1
	lsls r0, r0, #8
	ldr r1, [r7, #4]
	subs r1, r1, r0
	ldr r0, _0801AF10 @ =0xFFFFFF00
	ands r1, r0
	str r1, [r7, #4]
	b _0801AF32
	.align 2, 0
_0801AF10: .4byte 0xFFFFFF00
_0801AF14:
	ldr r5, [sp, #0x34]
	cmp r5, #2
	bne _0801AF24
	ldr r2, [sp, #0x2c]
	lsls r0, r2, #8
	ldr r1, [sp, #0x24]
	adds r0, r0, r1
	b _0801AF30
_0801AF24:
	cmp r5, #1
	bne _0801AF32
	ldr r3, [sp, #0x2c]
	lsls r1, r3, #8
	ldr r0, [sp, #0x24]
	subs r0, r0, r1
_0801AF30:
	str r0, [sp, #0x24]
_0801AF32:
	ldr r0, _0801AFD0 @ =gUnknown_030012D8
	mov sl, r0
	mov r1, r8
	cmp r1, #8
	beq _0801AF42
_0801AF3C:
	ldr r2, [sp, #0x30]
	cmp r2, #1
	bgt _0801AF7C
_0801AF42:
	mov r3, sl
	ldr r2, [r3]
	adds r1, r2, #0
	adds r1, #0x24
	movs r0, #4
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0801AF7C
	ldr r0, [sp, #0x40]
	cmp r0, #0
	beq _0801AF7C
	adds r0, r2, #0
	adds r0, #0xac
	mov r1, sb
	str r1, [r0]
	movs r1, #8
	subs r0, #0x44
	strb r1, [r0]
	ldr r2, [r3]
	ldr r1, [r2, #4]
	ldr r0, [sp, #0x30]
	subs r0, #1
	lsls r0, r0, #8
	subs r1, r1, r0
	str r1, [r7, #4]
	movs r5, #0
	ldr r0, [r2]
	str r0, [sp, #0x24]
_0801AF7C:
	mov r6, sl
	ldr r0, [r6]
	ldr r1, [sp, #0x24]
	ldr r2, [r7, #4]
	bl sub_8007398
	cmp r5, #0
	beq _0801AFAC
	ldr r0, [r6]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xc
	adds r3, r5, #0
	bl sub_803AD88
	ldr r1, [r6]
	ldr r0, [r1, #0x74]
	orrs r0, r5
	str r0, [r1, #0x74]
_0801AFAC:
	mov r5, r8
	cmp r5, #8
	beq _0801AFB4
	b _0801B1F8
_0801AFB4:
	mov r0, sb
	ldr r2, [r0, #0x78]
	cmp r2, #1
	beq _0801AFC4
	cmp r2, #5
	beq _0801AFC4
	cmp r2, #6
	bne _0801AFD4
_0801AFC4:
	mov r1, sb
	ldr r0, [r1, #0x44]
	adds r0, #0x32
	movs r1, #1
	b _0801B1F6
	.align 2, 0
_0801AFD0: .4byte gUnknown_030012D8
_0801AFD4:
	mov r3, sb
	ldr r0, [r3]
	asrs r0, r0, #8
	ldr r3, [r6]
	ldr r1, [r3]
	asrs r1, r1, #8
	subs r0, r0, r1
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	cmp r0, #7
	ble _0801AFEE
	b _0801B1F8
_0801AFEE:
	cmp r2, #3
	beq _0801B014
	cmp r2, #3
	bgt _0801AFFC
	cmp r2, #2
	beq _0801B002
	b _0801B1F8
_0801AFFC:
	cmp r2, #4
	beq _0801B048
	b _0801B1F8
_0801B002:
	ldr r1, [r3, #0x18]
	adds r1, #0x68
	movs r5, #0
	ldrsh r0, [r1, r5]
	adds r0, r3, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0x11
	b _0801B1D0
_0801B014:
	ldr r4, _0801B044 @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_80232A0
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801B024
	b _0801B1F8
_0801B024:
	ldr r0, [r4]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801B030
	b _0801B1F8
_0801B030:
	ldr r0, [r6]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xf
	b _0801B1D0
	.align 2, 0
_0801B044: .4byte gUnknown_030012C0
_0801B048:
	ldr r4, _0801B074 @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_8023278
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801B058
	b _0801B1F8
_0801B058:
	ldr r0, [r4]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801B064
	b _0801B1F8
_0801B064:
	mov r5, sl
	ldr r0, [r5]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	b _0801B1CA
	.align 2, 0
_0801B074: .4byte gUnknown_030012C0
_0801B078:
	ldr r0, [sp, #8]
	subs r0, #4
	str r0, [sp, #8]
	ldr r0, [sp, #0x10]
	adds r0, #4
	str r0, [sp, #0x10]
	mov r5, sb
	ldr r0, [r5, #0x78]
	cmp r0, #7
	bls _0801B08E
	b _0801B1F8
_0801B08E:
	lsls r0, r0, #2
	ldr r1, _0801B098 @ =_0801B09C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801B098: .4byte _0801B09C
_0801B09C: @ jump table
	.4byte _0801B0BC @ case 0
	.4byte _0801B1E0 @ case 1
	.4byte _0801B0E4 @ case 2
	.4byte _0801B124 @ case 3
	.4byte _0801B180 @ case 4
	.4byte _0801B1E0 @ case 5
	.4byte _0801B1E0 @ case 6
	.4byte _0801B0BC @ case 7
_0801B0BC:
	add r0, sp, #4
	adds r1, r4, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801B0CC
	b _0801B1F8
_0801B0CC:
	ldr r0, _0801B0E0 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r1, r0, #0
	adds r1, #0xac
	mov r2, sb
	str r2, [r1]
	movs r1, #8
	adds r0, #0x68
	b _0801B1F6
	.align 2, 0
_0801B0E0: .4byte gUnknown_030012D8
_0801B0E4:
	add r0, sp, #4
	adds r1, r4, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801B0F4
	b _0801B1F8
_0801B0F4:
	mov r3, sb
	ldr r1, [r3]
	asrs r1, r1, #8
	ldr r0, _0801B120 @ =gUnknown_030012D8
	ldr r2, [r0]
	ldr r0, [r2]
	asrs r0, r0, #8
	subs r1, r1, r0
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	cmp r1, #7
	bgt _0801B1F8
	ldr r1, [r2, #0x18]
	adds r1, #0x68
	movs r5, #0
	ldrsh r0, [r1, r5]
	adds r0, r2, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0x11
	b _0801B1D0
	.align 2, 0
_0801B120: .4byte gUnknown_030012D8
_0801B124:
	ldr r5, _0801B178 @ =gUnknown_030012C0
	ldr r0, [r5]
	bl sub_80232A0
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801B1F8
	ldr r0, [r5]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0801B1F8
	add r0, sp, #4
	adds r1, r4, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801B1F8
	mov r0, sb
	ldr r1, [r0]
	asrs r1, r1, #8
	ldr r0, _0801B17C @ =gUnknown_030012D8
	ldr r2, [r0]
	ldr r0, [r2]
	asrs r0, r0, #8
	subs r1, r1, r0
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	cmp r1, #7
	bgt _0801B1F8
	ldr r1, [r2, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xf
	b _0801B1D0
	.align 2, 0
_0801B178: .4byte gUnknown_030012C0
_0801B17C: .4byte gUnknown_030012D8
_0801B180:
	ldr r5, _0801B1D8 @ =gUnknown_030012C0
	ldr r0, [r5]
	bl sub_8023278
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801B1F8
	ldr r0, [r5]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0801B1F8
	add r0, sp, #4
	adds r1, r4, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801B1F8
	mov r5, sb
	ldr r1, [r5]
	asrs r1, r1, #8
	ldr r0, _0801B1DC @ =gUnknown_030012D8
	ldr r2, [r0]
	ldr r0, [r2]
	asrs r0, r0, #8
	subs r1, r1, r0
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	cmp r1, #7
	bgt _0801B1F8
	ldr r1, [r2, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
_0801B1CA:
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0x10
_0801B1D0:
	movs r3, #0
	bl sub_803AD88
	b _0801B1F8
	.align 2, 0
_0801B1D8: .4byte gUnknown_030012C0
_0801B1DC: .4byte gUnknown_030012D8
_0801B1E0:
	add r0, sp, #4
	adds r1, r4, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	lsrs r1, r0, #0x18
	cmp r1, #0
	bne _0801B1F8
	mov r5, sb
	ldr r0, [r5, #0x44]
	adds r0, #0x32
_0801B1F6:
	strb r1, [r0]
_0801B1F8:
	add sp, #0x44
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_801B208
sub_801B208: @ 0x0801B208
	push {r4, lr}
	adds r4, r0, #0
	ldr r1, [r4, #0x18]
	movs r2, #0x38
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #0x3c]
	bl sub_803AD7C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801B270
	adds r0, r4, #0
	bl sub_8008044
	ldr r1, [r4, #0x18]
	adds r1, #0x60
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	ldr r0, [r4, #0x78]
	cmp r0, #6
	bne _0801B254
	ldr r0, [r4, #0x30]
	cmp r0, #0x12
	ble _0801B254
	ldr r0, _0801B26C @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r1, r0, #0
	adds r1, #0xac
	ldr r0, [r1]
	cmp r0, r4
	bne _0801B254
	movs r0, #0
	str r0, [r1]
_0801B254:
	ldr r2, [r4, #0x44]
	cmp r2, #0
	beq _0801B296
	ldr r1, [r2, #0xc]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	adds r1, r4, #0
	bl sub_803AD80
	b _0801B296
	.align 2, 0
_0801B26C: .4byte gUnknown_030012D8
_0801B270:
	ldr r1, [r4, #0x18]
	adds r1, #0x60
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	ldr r2, [r4, #0x44]
	cmp r2, #0
	beq _0801B296
	ldr r1, [r2, #0xc]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	adds r1, r4, #0
	bl sub_803AD80
_0801B296:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_801B29C
sub_801B29C: @ 0x0801B29C
	ldrb r0, [r0, #0xd]
	lsrs r0, r0, #4
	movs r1, #1
	ands r0, r1
	bx lr
	.align 2, 0

	thumb_func_start sub_801B2A8
sub_801B2A8: @ 0x0801B2A8
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	movs r2, #1
	ands r1, r2
	lsls r1, r1, #4
	movs r2, #0x11
	rsbs r2, r2, #0
	ldrb r3, [r0, #0xd]
	ands r2, r3
	orrs r2, r1
	strb r2, [r0, #0xd]
	bx lr

	thumb_func_start sub_801B2C0
sub_801B2C0: @ 0x0801B2C0
	movs r0, #4
	bx lr

	thumb_func_start sub_801B2C4
sub_801B2C4: @ 0x0801B2C4
	push {lr}
	ldr r2, _0801B2D4 @ =gStaticData_087E49DC
	str r2, [r0, #0x18]
	bl sub_8009F1C
	pop {r0}
	bx r0
	.align 2, 0
_0801B2D4: .4byte gStaticData_087E49DC

	thumb_func_start sub_801B2D8
sub_801B2D8: @ 0x0801B2D8
	movs r1, #0x41
	rsbs r1, r1, #0
	ldrb r2, [r0, #0xc]
	ands r1, r2
	strb r1, [r0, #0xc]
	bx lr

	thumb_func_start sub_801B2E4
sub_801B2E4: @ 0x0801B2E4
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8009F90
	ldr r0, _0801B300 @ =gStaticData_087E49DC
	str r0, [r4, #0x18]
	adds r0, r4, #0
	bl sub_801B2D8
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801B300: .4byte gStaticData_087E49DC

	thumb_func_start sub_801B304
sub_801B304: @ 0x0801B304
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	adds r4, r1, #0
	ldr r0, [r6, #0x20]
	cmp r0, #0
	bne _0801B35E
	ldr r0, [r6, #0x28]
	cmp r0, #0
	ble _0801B35E
	ldr r0, [r4]
	asrs r0, r0, #8
	str r0, [r6, #0x20]
	ldr r0, [r6, #4]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801B348 @ =gStaticData_0816C460
	adds r3, r0, r1
	adds r0, r6, #0
	adds r0, #0x30
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801B34C
	ldr r0, [r3]
	ldr r1, [r3, #4]
	ldr r2, [r3, #8]
	str r0, [r4, #0x60]
	str r0, [r4, #0x48]
	str r1, [r4, #0x4c]
	str r2, [r4, #0x50]
	b _0801B35E
	.align 2, 0
_0801B348: .4byte gStaticData_0816C460
_0801B34C:
	ldr r0, [r3]
	rsbs r0, r0, #0
	ldr r1, [r3, #8]
	rsbs r1, r1, #0
	ldr r2, [r3, #4]
	str r0, [r4, #0x60]
	str r0, [r4, #0x48]
	str r2, [r4, #0x4c]
	str r1, [r4, #0x50]
_0801B35E:
	ldr r0, [r6, #0x24]
	cmp r0, #0
	bne _0801B3B2
	ldr r0, [r6, #0x2c]
	cmp r0, #0
	ble _0801B3B2
	ldr r0, [r4, #4]
	asrs r0, r0, #8
	str r0, [r6, #0x24]
	ldr r0, [r6, #4]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801B39C @ =gStaticData_0816C460
	adds r3, r0, r1
	adds r0, r6, #0
	adds r0, #0x31
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801B3A0
	ldr r0, [r3]
	ldr r1, [r3, #4]
	ldr r2, [r3, #8]
	str r0, [r4, #0x64]
	str r0, [r4, #0x54]
	str r1, [r4, #0x58]
	str r2, [r4, #0x5c]
	b _0801B3B2
	.align 2, 0
_0801B39C: .4byte gStaticData_0816C460
_0801B3A0:
	ldr r0, [r3]
	rsbs r0, r0, #0
	ldr r1, [r3, #8]
	rsbs r1, r1, #0
	ldr r2, [r3, #4]
	str r0, [r4, #0x64]
	str r0, [r4, #0x54]
	str r2, [r4, #0x58]
	str r1, [r4, #0x5c]
_0801B3B2:
	ldr r2, [r6, #0x18]
	movs r0, #1
	rsbs r0, r0, #0
	cmp r2, r0
	beq _0801B3CE
	ldr r0, [r4]
	asrs r0, r0, #8
	ldr r1, [r6, #0x20]
	subs r0, r0, r1
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	adds r0, r2, r0
	b _0801B3F0
_0801B3CE:
	ldr r2, [r4, #0x60]
	asrs r0, r2, #0x1f
	eors r2, r0
	subs r2, r2, r0
	ldr r3, _0801B410 @ =gStaticData_0816C460
	ldr r0, [r6, #4]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r3, #8
	adds r0, r0, r3
	ldr r0, [r0]
	cmp r2, r0
	blt _0801B3F2
	movs r0, #0
_0801B3F0:
	str r0, [r6, #0x18]
_0801B3F2:
	ldr r2, [r6, #0x1c]
	movs r0, #1
	rsbs r0, r0, #0
	cmp r2, r0
	beq _0801B414
	ldr r0, [r4, #4]
	asrs r0, r0, #8
	ldr r1, [r6, #0x24]
	subs r0, r0, r1
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	adds r0, r2, r0
	b _0801B436
	.align 2, 0
_0801B410: .4byte gStaticData_0816C460
_0801B414:
	ldr r2, [r4, #0x64]
	asrs r0, r2, #0x1f
	eors r2, r0
	subs r2, r2, r0
	ldr r3, _0801B474 @ =gStaticData_0816C460
	ldr r0, [r6, #4]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r3, #8
	adds r0, r0, r3
	ldr r0, [r0]
	cmp r2, r0
	blt _0801B438
	movs r0, #0
_0801B436:
	str r0, [r6, #0x1c]
_0801B438:
	ldr r0, [r6, #0x18]
	ldr r1, [r6, #0x28]
	cmp r0, r1
	ble _0801B48E
	cmp r1, #0
	beq _0801B48E
	adds r0, r6, #0
	adds r0, #0x30
	movs r2, #1
	ldrb r1, [r0]
	eors r2, r1
	strb r2, [r0]
	ldr r0, [r6, #4]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801B474 @ =gStaticData_0816C460
	adds r3, r0, r1
	cmp r2, #0
	beq _0801B478
	ldr r0, [r3]
	ldr r1, [r3, #4]
	ldr r2, [r3, #8]
	str r0, [r4, #0x48]
	str r1, [r4, #0x4c]
	str r2, [r4, #0x50]
	b _0801B488
	.align 2, 0
_0801B474: .4byte gStaticData_0816C460
_0801B478:
	ldr r0, [r3]
	rsbs r0, r0, #0
	ldr r1, [r3, #8]
	rsbs r1, r1, #0
	ldr r2, [r3, #4]
	str r0, [r4, #0x48]
	str r2, [r4, #0x4c]
	str r1, [r4, #0x50]
_0801B488:
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r6, #0x18]
_0801B48E:
	ldr r0, [r6, #0x1c]
	ldr r1, [r6, #0x2c]
	cmp r0, r1
	ble _0801B4E2
	cmp r1, #0
	beq _0801B4E2
	adds r0, r6, #0
	adds r0, #0x31
	movs r2, #1
	ldrb r5, [r0]
	eors r2, r5
	strb r2, [r0]
	ldr r0, [r6, #4]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801B4C8 @ =gStaticData_0816C460
	adds r3, r0, r1
	cmp r2, #0
	beq _0801B4CC
	ldr r0, [r3]
	ldr r1, [r3, #4]
	ldr r2, [r3, #8]
	str r0, [r4, #0x54]
	str r1, [r4, #0x58]
	str r2, [r4, #0x5c]
	b _0801B4DC
	.align 2, 0
_0801B4C8: .4byte gStaticData_0816C460
_0801B4CC:
	ldr r0, [r3]
	rsbs r0, r0, #0
	ldr r1, [r3, #8]
	rsbs r1, r1, #0
	ldr r2, [r3, #4]
	str r0, [r4, #0x54]
	str r2, [r4, #0x58]
	str r1, [r4, #0x5c]
_0801B4DC:
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r6, #0x1c]
_0801B4E2:
	ldr r7, [r6, #0x10]
	cmp r7, #5
	bne _0801B558
	ldr r1, [r6, #0x14]
	cmp r1, #0
	ble _0801B518
	ldr r0, _0801B514 @ =gUnknown_0300082C
	ldr r0, [r0]
	subs r0, r0, r1
	cmp r0, #0x3c
	bne _0801B518
	ldr r1, [r6, #0xc]
	adds r1, #0x60
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r3, [r1, #4]
	adds r1, r4, #0
	movs r2, #3
	bl sub_803AD84
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r6, #0x14]
	b _0801B604
	.align 2, 0
_0801B514: .4byte gUnknown_0300082C
_0801B518:
	cmp r7, #5
	bne _0801B558
	cmp r1, #0
	ble _0801B558
	ldr r0, _0801B544 @ =gUnknown_0300082C
	ldr r5, [r0]
	subs r0, r5, r1
	movs r1, #0x1e
	bl sub_803AF1C
	cmp r0, #4
	bhi _0801B558
	movs r0, #1
	ands r5, r0
	cmp r5, #0
	bne _0801B54C
	ldr r0, [r4, #4]
	ldr r5, _0801B548 @ =0xFFFFFD00
	adds r0, r0, r5
	str r0, [r4, #4]
	b _0801B604
	.align 2, 0
_0801B544: .4byte gUnknown_0300082C
_0801B548: .4byte 0xFFFFFD00
_0801B54C:
	ldr r0, [r4, #4]
	movs r2, #0xc0
	lsls r2, r2, #2
	adds r0, r0, r2
	str r0, [r4, #4]
	b _0801B604
_0801B558:
	cmp r7, #7
	bne _0801B56C
	ldr r0, [r4, #0x30]
	cmp r0, #1
	bgt _0801B56C
	adds r0, r6, #0
	adds r0, #0x32
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801B5E6
_0801B56C:
	cmp r7, #6
	bne _0801B580
	ldr r0, [r4, #0x30]
	cmp r0, #1
	bgt _0801B580
	ldr r0, _0801B5C4 @ =gUnknown_0300082C
	ldr r1, [r0]
	ldr r0, [r6, #0x34]
	cmp r1, r0
	blo _0801B5E6
_0801B580:
	cmp r7, #7
	bne _0801B5D0
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801B5D0
	movs r0, #1
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _0801B5C8 @ =0x0000FFFF
	ldrh r2, [r4, #8]
	cmp r2, r0
	beq _0801B604
	ldrh r3, [r4, #8]
	ldr r0, _0801B5CC @ =gUnknown_030012B4
	ldr r2, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r1, r0, #2
	movs r5, #0x84
	lsls r5, r5, #1
	adds r2, r2, r5
	adds r2, r2, r1
	lsls r0, r0, #5
	subs r0, r3, r0
	movs r1, #1
	lsls r1, r0
	ldr r0, [r2]
	orrs r0, r1
	str r0, [r2]
	b _0801B604
	.align 2, 0
_0801B5C4: .4byte gUnknown_0300082C
_0801B5C8: .4byte 0x0000FFFF
_0801B5CC: .4byte gUnknown_030012B4
_0801B5D0:
	cmp r7, #6
	bne _0801B604
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801B604
	ldr r0, _0801B620 @ =gUnknown_0300082C
	ldr r0, [r0]
	adds r0, #0x78
	str r0, [r6, #0x34]
_0801B5E6:
	movs r3, #0
	ldr r0, [r4, #0x20]
	adds r2, r4, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r5, [r2]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _0801B602
	subs r3, r0, #1
_0801B602:
	str r3, [r4, #0x30]
_0801B604:
	adds r0, r6, #0
	adds r1, r4, #0
	bl sub_801B624
	ldr r0, [r4]
	asrs r0, r0, #8
	str r0, [r6, #0x20]
	ldr r0, [r4, #4]
	asrs r0, r0, #8
	str r0, [r6, #0x24]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801B620: .4byte gUnknown_0300082C

	thumb_func_start sub_801B624
sub_801B624: @ 0x0801B624
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	adds r7, r1, #0
	adds r0, #0x32
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801B6D8
	ldr r0, [r6, #0x10]
	cmp r0, #6
	beq _0801B6D8
	ldr r0, _0801B698 @ =gUnknown_030012D8
	mov r8, r0
	ldr r2, [r0]
	ldrb r1, [r2, #0xc]
	lsrs r0, r1, #7
	cmp r0, #0
	beq _0801B6D8
	adds r0, r2, #0
	adds r0, #0xac
	str r7, [r0]
	movs r1, #8
	subs r0, #0x44
	strb r1, [r0]
	mov r1, r8
	ldr r0, [r1]
	ldr r1, [r0]
	asrs r1, r1, #8
	ldr r5, [r7]
	asrs r5, r5, #8
	ldr r2, [r6, #0x20]
	subs r5, r5, r2
	ldr r2, [r0, #4]
	asrs r2, r2, #8
	ldr r3, [r7, #4]
	asrs r3, r3, #8
	ldr r4, [r6, #0x24]
	subs r3, r3, r4
	adds r1, r1, r5
	adds r2, r2, r3
	lsls r1, r1, #8
	str r1, [r0]
	lsls r2, r2, #8
	str r2, [r0, #4]
	bl sub_8009EA8
	mov r1, r8
	ldr r0, [r1]
	adds r0, #0x24
	ldrb r2, [r0]
	ldr r0, [r7, #0x60]
	cmp r0, #0
	ble _0801B69C
	movs r0, #1
	orrs r2, r0
	b _0801B6A8
	.align 2, 0
_0801B698: .4byte gUnknown_030012D8
_0801B69C:
	cmp r0, #0
	bge _0801B6A8
	movs r0, #2
	orrs r2, r0
	lsls r0, r2, #0x18
	lsrs r2, r0, #0x18
_0801B6A8:
	ldr r1, [r7, #0x64]
	cmp r1, #0
	ble _0801B6B2
	movs r0, #8
	b _0801B6B8
_0801B6B2:
	cmp r1, #0
	bge _0801B6BE
	movs r0, #4
_0801B6B8:
	orrs r2, r0
	lsls r0, r2, #0x18
	lsrs r2, r0, #0x18
_0801B6BE:
	ldr r0, _0801B6E4 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x24
	strb r2, [r0]
	ldr r0, [r6, #0x10]
	cmp r0, #5
	bne _0801B6D8
	ldr r0, [r6, #0x14]
	cmp r0, #0
	bne _0801B6D8
	ldr r0, _0801B6E8 @ =gUnknown_0300082C
	ldr r0, [r0]
	str r0, [r6, #0x14]
_0801B6D8:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801B6E4: .4byte gUnknown_030012D8
_0801B6E8: .4byte gUnknown_0300082C
	thumb_func_start sub_801B6EC
sub_801B6EC: @ 0x0801B6EC
	adds r3, r1, #0
	ldr r0, [r0, #4]
	ldr r0, [r0, #0]
	lsls r2, r2, #3
	adds r2, r2, r0
	ldr r1, [r2, #4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801B720 @ =gStaticData_0816C460
	adds r2, r0, r1
	adds r0, r3, #0
	adds r0, #0x28
	ldrb r0, [r0, #0]
	lsls r0, r0, #26
	cmp r0, #0
	bge _0801B724
	ldr r0, [r2, #0]
	negs r0, r0
	ldr r1, [r2, #8]
	negs r1, r1
	ldr r2, [r2, #4]
	str r0, [r3, #0x54]
	str r2, [r3, #0x58]
	str r1, [r3, #0x5c]
	b _0801B730
	.align 2, 0
_0801B720: .4byte gStaticData_0816C460
_0801B724:
	ldr r0, [r2, #0]
	ldr r1, [r2, #4]
	ldr r2, [r2, #8]
	str r0, [r3, #0x54]
	str r1, [r3, #0x58]
	str r2, [r3, #0x5c]
_0801B730:
	bx lr
	.align 2, 0

	thumb_func_start sub_801B734
sub_801B734: @ 0x0801B734
	adds r3, r1, #0
	ldr r0, [r0, #4]
	ldr r0, [r0, #0]
	lsls r2, r2, #3
	adds r2, r2, r0
	ldr r1, [r2, #0]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801B768 @ =gStaticData_0816C460
	adds r2, r0, r1
	adds r0, r3, #0
	adds r0, #0x28
	ldrb r0, [r0, #0]
	lsls r0, r0, #27
	cmp r0, #0
	bge _0801B76C
	ldr r0, [r2, #0]
	negs r0, r0
	ldr r1, [r2, #8]
	negs r1, r1
	ldr r2, [r2, #4]
	str r0, [r3, #0x48]
	str r2, [r3, #0x4c]
	str r1, [r3, #0x50]
	b _0801B778
	.align 2, 0
_0801B768: .4byte gStaticData_0816C460
_0801B76C:
	ldr r0, [r2, #0]
	ldr r1, [r2, #4]
	ldr r2, [r2, #8]
	str r0, [r3, #0x48]
	str r1, [r3, #0x4c]
	str r2, [r3, #0x50]
_0801B778:
	bx lr

	thumb_func_start sub_801B77C
sub_801B77C: @ 0x0801B77C
	push {lr}
	ldr r3, [r0, #4]
	ldr r3, [r3]
	lsls r2, r2, #3
	adds r2, r2, r3
	ldr r3, [r2, #4]
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r2, r2, #2
	ldr r3, _0801B79C @ =gStaticData_0816C460
	adds r2, r2, r3
	bl sub_800B6D0
	pop {r0}
	bx r0
	.align 2, 0
_0801B79C: .4byte gStaticData_0816C460

	thumb_func_start sub_801B7A0
sub_801B7A0: @ 0x0801B7A0
	push {lr}
	ldr r3, [r0, #4]
	ldr r3, [r3]
	lsls r2, r2, #3
	adds r2, r2, r3
	ldr r3, [r2]
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r2, r2, #2
	ldr r3, _0801B7C0 @ =gStaticData_0816C460
	adds r2, r2, r3
	bl sub_800B7B0
	pop {r0}
	bx r0
	.align 2, 0
_0801B7C0: .4byte gStaticData_0816C460

	thumb_func_start sub_801B7C4
sub_801B7C4: @ 0x0801B7C4
	push {lr}
	ldr r2, _0801B7D4 @ =gStaticData_087E4A54
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_0801B7D4: .4byte gStaticData_087E4A54

	thumb_func_start sub_801B7D8
sub_801B7D8: @ 0x0801B7D8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	add r0, sp, #0x18
	lsls r3, r3, #0x18
	lsrs r3, r3, #0x18
	mov r8, r3
	ldrb r7, [r0]
	adds r0, r4, #0
	bl sub_800B8C8
	ldr r0, _0801B848 @ =gStaticData_087E4A54
	str r0, [r4, #0xc]
	ldr r0, [sp, #0x1c]
	subs r0, #6
	cmp r0, #1
	bhi _0801B804
	movs r6, #0
	movs r5, #0
_0801B804:
	movs r1, #0
	str r1, [r4, #0x20]
	str r5, [r4, #0x18]
	str r1, [r4, #0x24]
	str r6, [r4, #0x1c]
	ldr r0, _0801B84C @ =gStaticData_0816C458
	str r0, [r4, #4]
	adds r0, r4, #0
	adds r0, #0x32
	strb r1, [r0]
	ldr r0, [sp, #0x1c]
	str r0, [r4, #0x10]
	str r1, [r4, #0x14]
	lsls r0, r5, #1
	str r0, [r4, #0x28]
	lsls r0, r6, #1
	str r0, [r4, #0x2c]
	adds r0, r4, #0
	adds r0, #0x30
	mov r1, r8
	strb r1, [r0]
	adds r0, #1
	strb r7, [r0]
	ldr r0, _0801B850 @ =gUnknown_0300082C
	ldr r0, [r0]
	adds r0, #0x78
	str r0, [r4, #0x34]
	adds r0, r4, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0801B848: .4byte gStaticData_087E4A54
_0801B84C: .4byte gStaticData_0816C458
_0801B850: .4byte gUnknown_0300082C

	thumb_func_start sub_801B854
sub_801B854: @ 0x0801B854
	adds r0, #0x32
	movs r1, #0
	strb r1, [r0]
	bx lr

	thumb_func_start sub_801B85C
sub_801B85C: @ 0x0801B85C
	adds r0, #0x32
	movs r1, #1
	strb r1, [r0]
	bx lr

	thumb_func_start sub_801B864
sub_801B864: @ 0x0801B864
	push {r4, lr}
	adds r3, r0, #0
	ldrb r4, [r3, #0xd]
	lsrs r2, r4, #2
	movs r1, #1
	adds r0, r2, #0
	ands r0, r1
	cmp r0, #0
	bne _0801B888
	movs r0, #1
	eors r2, r0
	ands r2, r0
	lsls r1, r2, #2
	movs r0, #5
	rsbs r0, r0, #0
	ands r0, r4
	orrs r0, r1
	strb r0, [r3, #0xd]
_0801B888:
	ldr r0, _0801B8B4 @ =gUnknown_030012D4
	ldr r0, [r0]
	str r3, [r0, #0x10]
	ldr r0, _0801B8B8 @ =gUnknown_030012D8
	ldr r1, [r0]
	ldr r0, [r1]
	ldr r2, [r1, #4]
	movs r1, #0xf0
	lsls r1, r1, #5
	adds r0, r0, r1
	str r0, [r3]
	str r2, [r3, #4]
	movs r0, #0x10
	ldrb r2, [r3, #0xc]
	orrs r0, r2
	strb r0, [r3, #0xc]
	str r1, [r3, #0x78]
	str r1, [r3, #0x7c]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0801B8B4: .4byte gUnknown_030012D4
_0801B8B8: .4byte gUnknown_030012D8

	thumb_func_start sub_801B8BC
sub_801B8BC: @ 0x0801B8BC
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8009FB0
	movs r1, #1
	adds r0, r4, #0
	adds r0, #0x24
	movs r2, #0
	strb r1, [r0]
	adds r0, #0x44
	strb r2, [r0]
	ldr r0, [r4, #0x7c]
	ldr r1, [r4, #0x78]
	cmp r0, r1
	bge _0801B8E6
	movs r2, #0x80
	lsls r2, r2, #2
	adds r0, r0, r2
	cmp r0, r1
	bgt _0801B8F2
	b _0801B8FC
_0801B8E6:
	cmp r0, r1
	ble _0801B8FE
	ldr r2, _0801B8F8 @ =0xFFFFFE00
	adds r0, r0, r2
	cmp r0, r1
	bge _0801B8FC
_0801B8F2:
	str r1, [r4, #0x7c]
	b _0801B8FE
	.align 2, 0
_0801B8F8: .4byte 0xFFFFFE00
_0801B8FC:
	str r0, [r4, #0x7c]
_0801B8FE:
	ldr r0, _0801B918 @ =gUnknown_030012D8
	ldr r2, [r0]
	ldr r0, [r2]
	ldr r3, [r2, #4]
	ldr r1, [r4, #0x7c]
	adds r0, r0, r1
	str r0, [r4]
	str r3, [r4, #4]
	ldr r0, [r2, #0x60]
	str r0, [r4, #0x60]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0801B918: .4byte gUnknown_030012D8

	thumb_func_start sub_801B91C
sub_801B91C: @ 0x0801B91C
	push {lr}
	ldr r2, _0801B934 @ =gStaticData_087E4ABC
	str r2, [r0, #0x18]
	ldr r2, _0801B938 @ =gUnknown_030012D4
	ldr r3, [r2]
	ldr r2, _0801B93C @ =gUnknown_030012D8
	ldr r2, [r2]
	str r2, [r3, #0x10]
	bl sub_8009F1C
	pop {r0}
	bx r0
	.align 2, 0
_0801B934: .4byte gStaticData_087E4ABC
_0801B938: .4byte gUnknown_030012D4
_0801B93C: .4byte gUnknown_030012D8

	thumb_func_start sub_801B940
sub_801B940: @ 0x0801B940
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8009F90
	ldr r0, _0801B95C @ =gStaticData_087E4ABC
	str r0, [r4, #0x18]
	adds r0, r4, #0
	bl sub_801B864
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801B95C: .4byte gStaticData_087E4ABC
	thumb_func_start sub_801B960
sub_801B960: @ 0x0801B960
	adds	r2, r0, #0
	movs	r0, #200	@ 0xc8
	lsls	r0, r0, #6
	cmp	r1, r0
	ble _0801B96E
	adds	r1, r0, #0
	b _0801B978
_0801B96E:
	ldr r0, _0801B97C
	cmp	r1, r0
	bgt _0801B978
	movs	r1, #160	@ 0xa0
	lsls	r1, r1, #4
_0801B978:
	str	r1, [r2, #120]	@ 0x78
	bx	lr
_0801B97C: .4byte 0x9ff

	thumb_func_start sub_801B980
sub_801B980: @ 0x0801B980
	ldr r0, [r0, #0x78]
	bx lr

	thumb_func_start sub_801B984
sub_801B984: @ 0x0801B984
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	mov r8, r0
	adds r4, r1, #0
	adds r5, r2, #0
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	mov r8, r0
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	movs r0, #0x78
	bl sub_8026EDC
	adds r6, r0, #0
	bl sub_8009F90
	ldr r0, _0801BA50 @ =gStaticData_087E4B34
	str r0, [r6, #0x18]
	adds r0, r6, #0
	bl sub_801BAC4
	movs r1, #0
	mov sb, r1
	mov r2, r8
	strh r2, [r6, #8]
	lsls r4, r4, #8
	str r4, [r6]
	lsls r5, r5, #8
	str r5, [r6, #4]
	ldr r0, _0801BA54 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r6, #0
	bl sub_8008E94
	ldr r0, _0801BA58 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r3, #0xa8
	lsls r3, r3, #1
	adds r0, r0, r3
	str r0, [r6, #0x20]
	adds r4, r6, #0
	adds r4, #0x2d
	mov r0, sb
	strb r0, [r4]
	adds r0, r6, #0
	bl sub_80087C0
	adds r0, r6, #0
	bl sub_80087B4
	adds r0, r6, #0
	movs r1, #0
	bl sub_800872C
	adds r2, r6, #0
	adds r2, #0x28
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r1, [r2]
	ands r0, r1
	movs r1, #0x21
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r2]
	ldr r0, [r6, #0x20]
	ldr r1, [r0]
	ldrb r2, [r4]
	lsls r0, r2, #3
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r1, r1, r0
	ldr r0, _0801BA5C @ =gUnknown_030012B8
	ldr r0, [r0]
	ldrb r1, [r1, #0x14]
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	adds r2, r6, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	adds r0, r6, #0
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0801BA50: .4byte gStaticData_087E4B34
_0801BA54: .4byte gUnknown_030012F0
_0801BA58: .4byte gUnknown_030012D0
_0801BA5C: .4byte gUnknown_030012B8

	thumb_func_start sub_801BA60
sub_801BA60: @ 0x0801BA60
	push {r4, lr}
	sub sp, #0x10
	adds r1, r0, #0
	ldr r4, _0801BAAC @ =gUnknown_030012D8
	ldr r0, [r4]
	ldrb r0, [r0, #0xc]
	lsrs r0, r0, #7
	cmp r0, #0
	beq _0801BAA4
	mov r0, sp
	bl sub_8007B98
	ldr r0, [sp, #8]
	cmp r0, #0
	beq _0801BAA4
	ldr r0, [r4]
	mov r1, sp
	bl sub_800B37C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801BAA4
	ldr r0, [r4]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0x19
	movs r3, #0
	bl sub_803AD88
_0801BAA4:
	add sp, #0x10
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0801BAAC: .4byte gUnknown_030012D8

	thumb_func_start sub_801BAB0
sub_801BAB0: @ 0x0801BAB0
	push {lr}
	ldr r2, _0801BAC0 @ =gStaticData_087E4B34
	str r2, [r0, #0x18]
	bl sub_8009F1C
	pop {r0}
	bx r0
	.align 2, 0
_0801BAC0: .4byte gStaticData_087E4B34

	thumb_func_start sub_801BAC4
sub_801BAC4: @ 0x0801BAC4
	movs r1, #0x41
	rsbs r1, r1, #0
	ldrb r2, [r0, #0xc]
	ands r1, r2
	strb r1, [r0, #0xc]
	bx lr

	thumb_func_start sub_801BAD0
sub_801BAD0: @ 0x0801BAD0
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8009F90
	ldr r0, _0801BAEC @ =gStaticData_087E4B34
	str r0, [r4, #0x18]
	adds r0, r4, #0
	bl sub_801BAC4
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801BAEC: .4byte gStaticData_087E4B34

	thumb_func_start sub_801BAF0
sub_801BAF0: @ 0x0801BAF0
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r6, r0, #0
	movs r0, #0xc0
	lsls r0, r0, #0x18
	mov sb, r0
	bl mem_free_bytes
	bl sub_80006A8
	movs r0, #0xa0
	lsls r0, r0, #0x13
	movs r1, #0
	strh r1, [r0]
	movs r0, #0x80
	lsls r0, r0, #0x13
	strh r1, [r0]
	ldr r7, _0801BC0C @ =gUnknown_030012B8
	ldr r0, [r7]
	bl sub_8006EA8
	ldr r0, [r7]
	movs r1, #0xf
	bl sub_8006D50
	ldr r1, [r7]
	ldr r0, _0801BC10 @ =gStaticData_0816C56C
	movs r2, #0x83
	lsls r2, r2, #2
	adds r1, r1, r2
	movs r2, #0x10
	bl sub_803A94C
	ldr r5, _0801BC14 @ =gUnknown_030012FC
	ldr r0, [r5]
	movs r3, #0
	mov r8, r3
	str r3, [r0, #8]
	bl sub_8006C4C
	ldr r0, [r5]
	bl sub_8006C4C
	ldr r4, _0801BC18 @ =gUnknown_030012DC
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
	ldr r4, _0801BC1C @ =gUnknown_030012E0
	ldr r0, [r4]
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
	ldr r1, [r4]
	movs r2, #0x96
	lsls r2, r2, #1
	adds r1, r1, r2
	ldr r1, [r1]
	lsls r1, r1, #5
	bl sub_8006C58
	ldr r0, [r5]
	bl sub_8006C30
	ldr r0, _0801BC20 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x10
	bl sub_8001B54
	ldr r4, _0801BC24 @ =gUnknown_03000820
	movs r0, #0xac
	bl sub_8026EDC
	ldr r1, [r6]
	bl sub_801BC28
	str r0, [r4]
	bl sub_801C96C
	str r0, [r6]
	ldr r0, [r4]
	ldrb r5, [r0]
	cmp r0, #0
	beq _0801BBEE
	movs r1, #3
	bl sub_801C040
_0801BBEE:
	mov r3, r8
	str r3, [r4]
	ldr r0, [r7]
	bl sub_8006EA8
	mov r0, sb
	bl mem_free_bytes
	adds r0, r5, #0
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0801BC0C: .4byte gUnknown_030012B8
_0801BC10: .4byte gStaticData_0816C56C
_0801BC14: .4byte gUnknown_030012FC
_0801BC18: .4byte gUnknown_030012DC
_0801BC1C: .4byte gUnknown_030012E0
_0801BC20: .4byte gUnknown_030012BC
_0801BC24: .4byte gUnknown_03000820

	thumb_func_start sub_801BC28
sub_801BC28: @ 0x0801BC28
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x14
	adds r7, r0, #0
	adds r6, r1, #0
	adds r4, r7, #0
	adds r4, #0xa0
	movs r0, #0
	mov sb, r0
	str r0, [r4]
	movs r0, #0xc0
	ldrb r1, [r4]
	orrs r0, r1
	movs r1, #0x20
	orrs r0, r1
	movs r3, #1
	orrs r0, r3
	movs r2, #2
	mov r8, r2
	mov r1, r8
	orrs r0, r1
	movs r1, #4
	orrs r0, r1
	movs r1, #8
	orrs r0, r1
	movs r5, #0x10
	orrs r0, r5
	strb r0, [r4]
	adds r2, r7, #0
	adds r2, #0xa4
	movs r0, #0x20
	rsbs r0, r0, #0
	ldrb r1, [r2]
	ands r0, r1
	orrs r0, r5
	strb r0, [r2]
	ldr r1, _0801BCC8 @ =0x04000050
	ldr r0, [r4]
	str r0, [r1]
	adds r1, #4
	ldrb r2, [r2]
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	adds r2, r7, #0
	adds r2, #0xa8
	mov r0, sb
	strh r0, [r2]
	movs r0, #0x40
	ldrb r1, [r2]
	orrs r0, r1
	movs r1, #8
	rsbs r1, r1, #0
	ands r0, r1
	orrs r0, r3
	strb r0, [r2]
	adds r0, r7, #0
	adds r0, #0xa9
	ldrb r2, [r0]
	orrs r3, r2
	mov r1, r8
	orrs r3, r1
	orrs r3, r5
	strb r3, [r0]
	cmp r6, #0x13
	bgt _0801BCCC
	adds r0, r6, #0
	movs r1, #5
	bl sub_803ADB4
	str r0, [r7, #0xc]
	adds r0, r6, #0
	movs r1, #5
	bl sub_803AE4C
	b _0801BCD4
	.align 2, 0
_0801BCC8: .4byte 0x04000050
_0801BCCC:
	adds r0, r6, #0
	subs r0, #0x14
	str r0, [r7, #0xc]
	movs r0, #5
_0801BCD4:
	str r0, [r7, #8]
	movs r4, #0
	str r4, [r7, #0x14]
	ldr r0, _0801BFA4 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80236EC
	adds r1, r7, #0
	adds r1, #0x9c
	str r0, [r1]
	strb r4, [r7]
	movs r0, #0x28
	bl sub_8026EDC
	movs r1, #0
	movs r2, #0x1d
	bl sub_801D7F8
	str r0, [r7, #0x1c]
	movs r0, #3
	str r0, [sp]
	add r0, sp, #4
	movs r1, #2
	movs r2, #0x1e
	movs r3, #2
	bl sub_801E644
	ldr r1, _0801BFA8 @ =gStaticData_0816C484
	add r0, sp, #4
	bl LoadGraphicsPackage
	str r4, [r7, #0x7c]
	movs r0, #0x54
	bl sub_8026EDC
	bl sub_801E04C
	str r0, [r7, #0x3c]
	movs r0, #0x8c
	bl sub_8026EDC
	movs r1, #3
	movs r2, #0x1f
	bl sub_801D828
	str r0, [r7, #0x20]
	adds r6, r7, #0
	adds r6, #0x40
	adds r5, r7, #0
	adds r5, #0x24
	movs r4, #5
_0801BD3A:
	movs r0, #0x14
	bl sub_8026EDC
	bl sub_801DFEC
	stm r5!, {r0}
	subs r4, #1
	cmp r4, #0
	bge _0801BD3A
	adds r0, r7, #0
	bl sub_801D638
	adds r0, r7, #0
	bl sub_801D5CC
	adds r0, r7, #0
	bl sub_801D668
	movs r5, #0
	movs r2, #0x80
	mov r8, r2
	adds r4, r6, #0
_0801BD66:
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	str r0, [r4]
	movs r1, #1
	bl sub_80088D8
	cmp r5, #1
	ble _0801BD82
	ldr r0, [r4]
	mov r1, r8
	strh r1, [r0, #0x3c]
_0801BD82:
	adds r4, #4
	adds r5, #1
	cmp r5, #7
	ble _0801BD66
	ldr r2, _0801BFAC @ =gUnknown_030012D0
	mov sl, r2
	ldr r0, [r2]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x8d
	lsls r1, r1, #2
	adds r0, r0, r1
	ldr r4, [r7, #0x40]
	str r0, [r4, #0x20]
	ldr r1, _0801BFB0 @ =gStaticData_0816C548
	ldr r0, [r7, #0xc]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	adds r1, r4, #0
	adds r1, #0x2d
	movs r2, #0
	mov sb, r2
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r7, #0x40]
	ldr r2, _0801BFB4 @ =gStaticData_0816C498
	ldr r1, [r2]
	ldr r2, [r2, #4]
	bl sub_800737C
	mov r1, sl
	ldr r0, [r1]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r2, #0x8d
	lsls r2, r2, #2
	adds r0, r0, r2
	ldr r4, [r7, #0x44]
	str r0, [r4, #0x20]
	movs r0, #0xa
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
	ldr r0, [r7, #0x44]
	ldr r2, _0801BFB8 @ =gStaticData_0816C4A0
	ldr r1, [r2]
	ldr r2, [r2, #4]
	bl sub_800737C
	mov r1, sl
	ldr r0, [r1]
	ldr r0, [r0]
	ldr r1, [r0]
	movs r2, #0xde
	lsls r2, r2, #1
	adds r1, r1, r2
	ldr r0, [r7, #0x48]
	str r1, [r0, #0x20]
	ldr r2, _0801BFBC @ =gStaticData_0816C4A8
	ldr r1, [r2]
	ldr r2, [r2, #4]
	bl sub_800737C
	mov r1, sl
	ldr r0, [r1]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r2, #0xc0
	lsls r2, r2, #1
	adds r0, r0, r2
	ldr r4, [r7, #0x4c]
	str r0, [r4, #0x20]
	movs r0, #1
	mov r8, r0
	adds r0, r4, #0
	adds r0, #0x2d
	mov r1, r8
	strb r1, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r7, #0x4c]
	ldr r5, _0801BFC0 @ =gStaticData_0816C4B0
	ldr r1, [r5]
	ldr r2, [r5, #4]
	bl sub_800737C
	mov r2, sl
	ldr r0, [r2]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r0, r0, r1
	ldr r4, [r7, #0x50]
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r2, r8
	strb r2, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r7, #0x50]
	ldr r1, [r5]
	ldr r2, [r5, #4]
	bl sub_800737C
	mov r1, sl
	ldr r0, [r1]
	ldr r0, [r0]
	ldr r1, [r0]
	movs r2, #0xc6
	lsls r2, r2, #1
	adds r1, r1, r2
	ldr r0, [r7, #0x54]
	str r1, [r0, #0x20]
	ldr r4, _0801BFC4 @ =gStaticData_0816C4B8
	ldr r1, [r4]
	ldr r2, [r4, #4]
	bl sub_800737C
	mov r1, sl
	ldr r0, [r1]
	ldr r0, [r0]
	ldr r1, [r0]
	movs r2, #0xc6
	lsls r2, r2, #1
	adds r1, r1, r2
	ldr r0, [r7, #0x58]
	str r1, [r0, #0x20]
	ldr r1, [r4]
	ldr r2, [r4, #4]
	bl sub_800737C
	mov r1, sl
	ldr r0, [r1]
	ldr r0, [r0]
	ldr r1, [r0]
	movs r2, #0xc6
	lsls r2, r2, #1
	adds r1, r1, r2
	ldr r0, [r7, #0x5c]
	str r1, [r0, #0x20]
	ldr r2, _0801BFC8 @ =gStaticData_0816C4C0
	ldr r1, [r2]
	ldr r2, [r2, #4]
	bl sub_800737C
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	str r0, [r7, #0x60]
	movs r1, #1
	bl sub_80088D8
	mov r1, sl
	ldr r0, [r1]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r2, #0x9c
	lsls r2, r2, #2
	adds r0, r0, r2
	ldr r4, [r7, #0x60]
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r1, r8
	strb r1, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r7, #0x60]
	ldr r2, _0801BFCC @ =gStaticData_0816C4C8
	ldr r1, [r2]
	ldr r2, [r2, #4]
	bl sub_800737C
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	str r0, [r7, #0x64]
	movs r1, #1
	bl sub_80088D8
	mov r2, sl
	ldr r0, [r2]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x9c
	lsls r1, r1, #2
	adds r0, r0, r1
	ldr r4, [r7, #0x64]
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r2, sb
	strb r2, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r7, #0x64]
	ldr r2, _0801BFD0 @ =gStaticData_0816C4D0
	ldr r1, [r2]
	ldr r2, [r2, #4]
	bl sub_800737C
	ldr r0, _0801BFD4 @ =gUnknown_03000824
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801BFD8
	adds r0, r7, #0
	bl sub_801D434
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801BFD8
	ldr r0, [r7, #0x3c]
	bl sub_801E408
	b _0801BFEC
	.align 2, 0
_0801BFA4: .4byte gUnknown_030012C0
_0801BFA8: .4byte gStaticData_0816C484
_0801BFAC: .4byte gUnknown_030012D0
_0801BFB0: .4byte gStaticData_0816C548
_0801BFB4: .4byte gStaticData_0816C498
_0801BFB8: .4byte gStaticData_0816C4A0
_0801BFBC: .4byte gStaticData_0816C4A8
_0801BFC0: .4byte gStaticData_0816C4B0
_0801BFC4: .4byte gStaticData_0816C4B8
_0801BFC8: .4byte gStaticData_0816C4C0
_0801BFCC: .4byte gStaticData_0816C4C8
_0801BFD0: .4byte gStaticData_0816C4D0
_0801BFD4: .4byte gUnknown_03000824
_0801BFD8:
	ldr r0, [r7, #8]
	lsls r0, r0, #3
	ldr r2, [r7, #0x18]
	adds r2, r2, r0
	ldr r0, [r7, #0x3c]
	ldr r1, [r2]
	ldr r2, [r2, #4]
	subs r2, #0x18
	bl sub_801E480
_0801BFEC:
	ldr r1, _0801C02C @ =0x04000010
	movs r0, #0
	str r0, [r1]
	ldr r0, [r7, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801C030 @ =0x04000014
	str r0, [r1]
	add r0, sp, #4
	bl sub_801E640
	ldr r1, _0801C034 @ =0x04000008
	strh r0, [r1]
	ldr r0, [r7, #0x1c]
	bl sub_801E640
	ldr r1, _0801C038 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r7, #0x20]
	bl sub_801DE24
	ldr r1, _0801C03C @ =0x0400000C
	strh r0, [r1]
	adds r0, r7, #0
	add sp, #0x14
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0801C02C: .4byte 0x04000010
_0801C030: .4byte 0x04000014
_0801C034: .4byte 0x04000008
_0801C038: .4byte 0x0400000A
_0801C03C: .4byte 0x0400000C

	thumb_func_start sub_801C040
sub_801C040: @ 0x0801C040
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	mov r8, r1
	ldr r2, [r6, #0x64]
	cmp r2, #0
	beq _0801C062
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0801C062:
	ldr r2, [r6, #0x60]
	cmp r2, #0
	beq _0801C07A
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0801C07A:
	adds r7, r6, #0
	adds r7, #0x24
	adds r4, r6, #0
	adds r4, #0x40
	movs r5, #7
_0801C084:
	ldr r2, [r4]
	cmp r2, #0
	beq _0801C09C
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0801C09C:
	adds r4, #4
	subs r5, #1
	cmp r5, #0
	bge _0801C084
	ldr r0, [r6, #0x3c]
	cmp r0, #0
	beq _0801C0B0
	movs r1, #3
	bl sub_801E524
_0801C0B0:
	ldr r0, [r6, #0x20]
	cmp r0, #0
	beq _0801C0BC
	movs r1, #3
	bl sub_801DA38
_0801C0BC:
	adds r4, r7, #0
	movs r5, #5
_0801C0C0:
	ldr r2, [r4]
	cmp r2, #0
	beq _0801C0D6
	ldr r1, [r2, #0x10]
	movs r3, #0x28
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0x2c]
	movs r1, #3
	bl sub_803AD80
_0801C0D6:
	adds r4, #4
	subs r5, #1
	cmp r5, #0
	bge _0801C0C0
	ldr r0, [r6, #0x1c]
	cmp r0, #0
	beq _0801C0EA
	movs r1, #3
	bl sub_801D7E0
_0801C0EA:
	movs r0, #1
	mov r1, r8
	ands r0, r1
	cmp r0, #0
	beq _0801C0FA
	adds r0, r6, #0
	bl sub_8026ED0
_0801C0FA:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_801C104
sub_801C104: @ 0x0801C104
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	ldr r0, _0801C27C @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006A90
	ldr r0, _0801C280 @ =gUnknown_030012FC
	ldr r0, [r0]
	bl sub_8006C28
	ldr r0, [r7, #0x3c]
	bl sub_801E2BC
	ldr r0, [r7, #0x20]
	bl sub_801DD18
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801C19E
	ldr r1, [r7, #8]
	lsls r1, r1, #2
	adds r0, r7, #0
	adds r0, #0x24
	adds r0, r0, r1
	ldr r0, [r0]
	bl sub_801DE28
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801C19E
	ldr r5, _0801C284 @ =gUnknown_030012E0
	ldr r0, [r5]
	movs r4, #0x98
	lsls r4, r4, #1
	adds r1, r0, r4
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r7, #0x14]
	ldr r2, [r2, #0x14]
	bl sub_803AD80
	movs r3, #0xf0
	subs r3, r3, r0
	lsrs r3, r3, #1
	adds r0, r7, #0
	adds r0, #0x80
	ldr r2, [r0]
	rsbs r2, r2, #0
	ldr r0, [r5]
	adds r2, #2
	movs r5, #0x88
	lsls r5, r5, #1
	adds r1, r0, r5
	str r3, [r1]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r1, r0, r3
	str r2, [r1]
	adds r4, r0, r4
	ldr r2, [r4]
	movs r5, #0x20
	ldrsh r1, [r2, r5]
	adds r0, r0, r1
	ldr r1, [r7, #0x14]
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r0, [r7, #8]
	cmp r0, #4
	bgt _0801C19E
	adds r0, r7, #0
	bl sub_801C364
_0801C19E:
	ldr r0, [r7, #0x1c]
	bl sub_801D7D4
	movs r6, #0
	ldr r0, [r7, #4]
	movs r1, #0xa9
	adds r1, r1, r7
	mov r8, r1
	cmp r6, r0
	bgt _0801C1E0
_0801C1B2:
	lsls r1, r6, #2
	adds r0, r7, #0
	adds r0, #0x24
	adds r0, r0, r1
	ldr r4, [r0]
	ldr r0, [r4, #0x10]
	adds r5, r0, #0
	adds r5, #8
	movs r2, #8
	ldrsh r0, [r0, r2]
	adds r4, r4, r0
	ldr r0, [r7, #0x1c]
	bl sub_801D77C
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	adds r6, #1
	ldr r0, [r7, #4]
	cmp r6, r0
	ble _0801C1B2
_0801C1E0:
	ldr r0, [r7, #0x1c]
	bl sub_801D780
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801C25E
	ldr r0, [r7, #0x20]
	bl sub_801DCF8
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801C258
	movs r0, #0x2f
	bl sub_8026F38
	adds r6, r0, #0
	ldr r5, _0801C288 @ =gUnknown_030012DC
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
	movs r1, #0xf0
	subs r1, r1, r0
	lsrs r1, r1, #1
	ldr r0, [r5]
	movs r2, #0x96
	mov ip, r2
	movs r3, #0x88
	lsls r3, r3, #1
	adds r2, r0, r3
	str r1, [r2]
	movs r2, #0x8a
	lsls r2, r2, #1
	adds r1, r0, r2
	mov r3, ip
	str r3, [r1]
	movs r1, #0xf
	bl sub_8028A30
	ldr r0, [r5]
	adds r4, r0, r4
	ldr r2, [r4]
	movs r5, #0x20
	ldrsh r1, [r2, r5]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	adds r1, r6, #0
	bl sub_803AD80
	adds r0, r7, #0
	bl sub_801C2B0
_0801C258:
	ldr r0, [r7, #0x20]
	bl sub_801DC28
_0801C25E:
	ldr r0, [r7, #0x20]
	bl sub_801DD28
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801C28C
	movs r0, #5
	rsbs r0, r0, #0
	mov r1, r8
	ldrb r1, [r1]
	ands r0, r1
	mov r2, r8
	strb r0, [r2]
	b _0801C298
	.align 2, 0
_0801C27C: .4byte gUnknown_03001300
_0801C280: .4byte gUnknown_030012FC
_0801C284: .4byte gUnknown_030012E0
_0801C288: .4byte gUnknown_030012DC
_0801C28C:
	movs r0, #4
	mov r3, r8
	ldrb r3, [r3]
	orrs r0, r3
	mov r5, r8
	strb r0, [r5]
_0801C298:
	ldr r0, _0801C2AC @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006A48
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801C2AC: .4byte gUnknown_03001300

	thumb_func_start sub_801C2B0
sub_801C2B0: @ 0x0801C2B0
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	ldr r0, [r6, #0x60]
	bl sub_800815C
	ldr r2, [r6, #0x60]
	adds r2, #0x29
	movs r5, #0xf
	ands r0, r5
	movs r4, #0x10
	rsbs r4, r4, #0
	adds r1, r4, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldr r0, [r6, #0x64]
	bl sub_800815C
	ldr r1, [r6, #0x64]
	adds r1, #0x29
	ands r0, r5
	ldrb r5, [r1]
	ands r4, r5
	orrs r4, r0
	strb r4, [r1]
	ldr r0, [r6, #0xc]
	cmp r0, #2
	bgt _0801C326
	adds r0, r6, #0
	bl sub_801D434
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801C2FC
	ldr r1, [r6, #0x60]
	movs r4, #0
	b _0801C300
_0801C2FC:
	ldr r1, [r6, #0x60]
	movs r4, #1
_0801C300:
	ldr r0, [r1, #0x20]
	adds r3, r1, #0
	adds r3, #0x2d
	ldr r2, [r0]
	ldrb r5, [r3]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r2, [r0, #0x16]
	adds r0, r1, #0
	cmp r4, r2
	blt _0801C31C
	subs r4, r2, #1
_0801C31C:
	str r4, [r0, #0x30]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
_0801C326:
	adds r0, r6, #0
	bl sub_801D428
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801C35C
	ldr r3, [r6, #0x64]
	movs r4, #0
	ldr r0, [r3, #0x20]
	adds r2, r3, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r5, [r2]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _0801C350
	subs r4, r0, #1
_0801C350:
	str r4, [r3, #0x30]
	adds r0, r3, #0
	movs r1, #0
	movs r2, #0
	bl sub_8008890
_0801C35C:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801C364
sub_801C364: @ 0x0801C364
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x40]
	adds r5, r4, #0
	adds r5, #0x80
	ldr r1, [r5]
	rsbs r1, r1, #0
	movs r2, #0
	bl sub_8008890
	ldr r0, [r4, #0x44]
	ldr r1, [r5]
	rsbs r1, r1, #0
	movs r2, #0
	bl sub_8008890
	ldr r0, [r4, #0x48]
	ldr r1, [r5]
	rsbs r1, r1, #0
	adds r2, r4, #0
	adds r2, #0x84
	ldr r2, [r2]
	bl sub_8008890
	ldr r0, [r4, #0x4c]
	ldr r1, [r5]
	rsbs r1, r1, #0
	adds r2, r4, #0
	adds r2, #0x88
	ldr r2, [r2]
	bl sub_8008890
	adds r0, r4, #0
	adds r0, #0x98
	ldr r0, [r0]
	cmp r0, #5
	beq _0801C3BE
	ldr r0, [r4, #0x50]
	ldr r1, [r5]
	rsbs r1, r1, #0
	adds r2, r4, #0
	adds r2, #0x8c
	ldr r2, [r2]
	bl sub_8008890
_0801C3BE:
	adds r1, r4, #0
	adds r1, #0x9c
	ldr r0, [r4, #0x10]
	lsls r0, r0, #2
	adds r0, #4
	ldr r1, [r1]
	adds r1, r1, r0
	movs r0, #1
	ldrb r2, [r1]
	ands r0, r2
	cmp r0, #0
	beq _0801C3E2
	ldr r1, [r1]
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x13
	adds r0, r4, #0
	bl sub_801C3E8
_0801C3E2:
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_801C3E8
sub_801C3E8: @ 0x0801C3E8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	adds r5, r1, #0
	ldr r0, [r7, #0x54]
	adds r4, r7, #0
	adds r4, #0x80
	ldr r1, [r4]
	rsbs r1, r1, #0
	adds r2, r7, #0
	adds r2, #0x90
	ldr r2, [r2]
	bl sub_8008890
	ldr r0, [r7, #0x58]
	ldr r1, [r4]
	rsbs r1, r1, #0
	adds r2, r7, #0
	adds r2, #0x94
	ldr r2, [r2]
	bl sub_8008890
	ldr r1, [r7, #0x10]
	lsls r0, r1, #3
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801C468 @ =gStaticData_0816C86C
	adds r0, r0, r1
	cmp r5, #0
	beq _0801C474
	ldr r0, [r0, #0x10]
	cmp r5, r0
	bhi _0801C474
	ldr r2, _0801C46C @ =gStaticData_0816C4C0
	ldr r0, _0801C470 @ =gUnknown_030012E0
	ldr r3, [r0]
	ldr r1, [r4]
	ldr r0, [r2]
	adds r1, r1, r0
	adds r1, #0xa
	ldr r2, [r2, #4]
	subs r2, #8
	movs r4, #0x88
	lsls r4, r4, #1
	adds r0, r3, r4
	str r1, [r0]
	movs r5, #0x8a
	lsls r5, r5, #1
	adds r0, r3, r5
	str r2, [r0]
	movs r1, #0x98
	lsls r1, r1, #1
	adds r0, r3, r1
	ldr r2, [r0]
	movs r4, #0x20
	ldrsh r0, [r2, r4]
	adds r0, r3, r0
	adds r1, r7, #0
	adds r1, #0x68
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	b _0801C508
	.align 2, 0
_0801C468: .4byte gStaticData_0816C86C
_0801C46C: .4byte gStaticData_0816C4C0
_0801C470: .4byte gUnknown_030012E0
_0801C474:
	ldr r0, [r7, #0x5c]
	movs r5, #0x80
	adds r5, r5, r7
	mov r8, r5
	ldr r1, [r5]
	movs r2, #0
	bl sub_8008890
	ldr r6, _0801C514 @ =gUnknown_030012E0
	ldr r0, [r6]
	ldr r1, [r7, #0x5c]
	adds r1, #0x29
	ldrb r1, [r1]
	lsls r1, r1, #0x1c
	lsrs r1, r1, #0x1c
	bl sub_8028A30
	ldr r5, _0801C518 @ =gStaticData_0816C4C0
	ldr r0, [r6]
	mov r1, r8
	ldr r2, [r1]
	ldr r1, [r5]
	adds r2, r2, r1
	adds r2, #0xa
	ldr r3, [r5, #4]
	subs r3, #8
	movs r4, #0x88
	lsls r4, r4, #1
	adds r1, r0, r4
	str r2, [r1]
	movs r2, #0x8a
	lsls r2, r2, #1
	adds r1, r0, r2
	str r3, [r1]
	adds r4, #0x20
	adds r1, r0, r4
	ldr r2, [r1]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	adds r1, r7, #0
	adds r1, #0x71
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r0, [r6]
	bl sub_8028A40
	ldr r0, [r6]
	mov r1, r8
	ldr r2, [r1]
	ldr r1, [r5]
	adds r2, r2, r1
	adds r2, #0xa
	ldr r3, [r5, #4]
	adds r3, #8
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
	adds r1, r7, #0
	adds r1, #0x68
	ldr r2, [r2, #0x24]
	bl sub_803AD80
_0801C508:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801C514: .4byte gUnknown_030012E0
_0801C518: .4byte gStaticData_0816C4C0

	thumb_func_start sub_801C51C
sub_801C51C: @ 0x0801C51C
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	ldr r0, [r5, #0x1c]
	bl sub_801D7AC
	ldr r0, [r5, #0x3c]
	bl sub_801E190
	ldr r0, [r5, #0x1c]
	bl sub_801D780
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801C5F6
	ldr r0, [r5, #0x3c]
	bl sub_801E464
	lsls r0, r0, #0x18
	adds r6, r5, #0
	adds r6, #0x24
	cmp r0, #0
	beq _0801C5C2
	ldr r0, [r5, #8]
	lsls r0, r0, #2
	adds r0, r6, r0
	ldr r0, [r0]
	bl sub_801DE28
	lsls r0, r0, #0x18
	lsrs r7, r0, #0x18
	cmp r7, #0
	bne _0801C5C2
	ldr r0, [r5, #8]
	lsls r0, r0, #2
	adds r0, r6, r0
	ldr r4, [r0]
	adds r0, r4, #0
	movs r1, #1
	bl sub_801DEA0
	ldr r0, [r5, #0x20]
	bl sub_801DD18
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801C59A
	adds r0, r4, #0
	bl sub_801DE2C
	str r0, [r5, #0x10]
	lsls r4, r0, #3
	adds r4, r4, r0
	lsls r4, r4, #2
	ldr r0, _0801C5FC @ =gStaticData_0816C86C
	adds r4, r4, r0
	ldr r0, [r5, #0x20]
	ldr r1, [r4, #4]
	bl sub_801DD80
	ldr r0, [r4]
	bl sub_8026F38
	str r0, [r5, #0x14]
_0801C59A:
	adds r0, r5, #0
	adds r0, #0x80
	str r7, [r0]
	ldr r0, [r5, #8]
	cmp r0, #4
	bgt _0801C5BA
	adds r0, r5, #0
	bl sub_801C608
	ldr r0, _0801C600 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006EA8
	adds r0, r5, #0
	bl sub_801D730
_0801C5BA:
	ldr r0, _0801C604 @ =gUnknown_030012E0
	ldr r0, [r0]
	bl sub_8028A40
_0801C5C2:
	adds r7, r5, #0
	adds r7, #0x40
	movs r4, #5
_0801C5C8:
	ldm r6!, {r0}
	ldr r2, [r0, #0x10]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r2, #0x24]
	bl sub_803AD7C
	subs r4, #1
	cmp r4, #0
	bge _0801C5C8
	ldr r0, [r5, #0x20]
	bl sub_801DAD8
	adds r5, r7, #0
	adds r5, #8
	movs r4, #5
_0801C5EA:
	ldm r5!, {r0}
	bl sub_8008044
	subs r4, #1
	cmp r4, #0
	bge _0801C5EA
_0801C5F6:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801C5FC: .4byte gStaticData_0816C86C
_0801C600: .4byte gUnknown_030012B8
_0801C604: .4byte gUnknown_030012E0

	thumb_func_start sub_801C608
sub_801C608: @ 0x0801C608
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xc
	adds r7, r0, #0
	movs r0, #0x98
	adds r0, r0, r7
	mov r8, r0
	movs r0, #5
	mov r1, r8
	str r0, [r1]
	ldr r4, _0801C6F4 @ =gUnknown_030012C0
	ldr r0, [r4]
	ldr r1, [r7, #0x10]
	bl sub_802336C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801C638
	movs r0, #0
	mov r2, r8
	str r0, [r2]
_0801C638:
	ldr r0, [r4]
	ldr r1, [r7, #0x10]
	bl sub_8023360
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801C64C
	movs r0, #1
	mov r3, r8
	str r0, [r3]
_0801C64C:
	ldr r0, [r4]
	ldr r1, [r7, #0x10]
	bl sub_8023354
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801C660
	movs r0, #2
	mov r6, r8
	str r0, [r6]
_0801C660:
	ldr r0, [r4]
	ldr r1, [r7, #0x10]
	bl sub_8023348
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801C674
	movs r0, #3
	mov r1, r8
	str r0, [r1]
_0801C674:
	ldr r0, [r4]
	ldr r1, [r7, #0x10]
	bl sub_802333C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801C688
	movs r0, #4
	mov r2, r8
	str r0, [r2]
_0801C688:
	movs r3, #0x84
	adds r3, r3, r7
	mov ip, r3
	movs r0, #0
	str r0, [r3]
	movs r6, #0x88
	adds r6, r6, r7
	mov sl, r6
	str r0, [r6]
	adds r5, r7, #0
	adds r5, #0x8c
	str r0, [r5]
	adds r4, r7, #0
	adds r4, #0x90
	str r0, [r4]
	adds r3, r7, #0
	adds r3, #0x94
	str r0, [r3]
	adds r2, r7, #0
	adds r2, #0x9c
	ldr r0, [r7, #0x10]
	lsls r0, r0, #2
	adds r0, #4
	ldr r1, [r2]
	adds r1, r1, r0
	mov sb, r1
	movs r0, #1
	ldrb r1, [r1]
	ands r0, r1
	str r4, [sp, #4]
	str r3, [sp, #8]
	cmp r0, #0
	beq _0801C6D0
	movs r0, #0x1c
	mov r3, ip
	str r0, [r3]
_0801C6D0:
	movs r0, #2
	mov r1, sb
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _0801C6E0
	movs r0, #0x1c
	str r0, [r6]
_0801C6E0:
	mov r3, r8
	ldr r0, [r3]
	cmp r0, #5
	bhi _0801C74A
	lsls r0, r0, #2
	ldr r1, _0801C6F8 @ =_0801C6FC
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801C6F4: .4byte gUnknown_030012C0
_0801C6F8: .4byte _0801C6FC
_0801C6FC: @ jump table
	.4byte _0801C714 @ case 0
	.4byte _0801C71C @ case 1
	.4byte _0801C722 @ case 2
	.4byte _0801C728 @ case 3
	.4byte _0801C72E @ case 4
	.4byte _0801C73E @ case 5
_0801C714:
	movs r0, #4
	mov r1, sb
	ldrb r1, [r1]
	b _0801C734
_0801C71C:
	ldr r1, [r2]
	movs r0, #1
	b _0801C732
_0801C722:
	ldr r1, [r2]
	movs r0, #4
	b _0801C732
_0801C728:
	ldr r1, [r2]
	movs r0, #8
	b _0801C732
_0801C72E:
	ldr r1, [r2]
	movs r0, #2
_0801C732:
	ldrb r1, [r1, #2]
_0801C734:
	ands r0, r1
	cmp r0, #0
	beq _0801C73E
	movs r0, #0x1c
	str r0, [r5]
_0801C73E:
	adds r0, r7, #0
	adds r0, #0x98
	ldr r1, [r0]
	adds r3, r0, #0
	cmp r1, #5
	beq _0801C782
_0801C74A:
	ldr r1, _0801C86C @ =gStaticData_0816C558
	ldr r0, [r3]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r4, [r7, #0x50]
	ldr r0, [r0]
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
	ldr r1, [r6]
	ldr r0, [r5]
	cmp r1, r0
	bne _0801C782
	subs r0, r1, #6
	str r0, [r6]
	ldr r0, [r5]
	adds r0, #6
	str r0, [r5]
_0801C782:
	movs r2, #1
	mov sl, r2
	mov r0, sl
	mov r3, sb
	ldrb r3, [r3]
	ands r0, r3
	cmp r0, #0
	bne _0801C794
	b _0801C95A
_0801C794:
	ldr r1, [r7, #0x10]
	lsls r0, r1, #3
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801C870 @ =gStaticData_0816C86C
	adds r5, r0, r1
	str r5, [sp]
	ldr r0, [r5, #8]
	adds r6, r7, #0
	adds r6, #0x71
	adds r1, r6, #0
	bl FormatCentiseconds
	mov r1, sb
	ldr r0, [r1]
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x13
	adds r1, r7, #0
	adds r1, #0x68
	bl FormatCentiseconds
	ldr r4, [r7, #0x54]
	movs r2, #0
	mov r8, r2
	adds r0, r4, #0
	adds r0, #0x2d
	mov r3, r8
	strb r3, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r4, [r7, #0x58]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r1, r8
	strb r1, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r4, [r7, #0x5c]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r2, r8
	strb r2, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, _0801C874 @ =0x0000FFF8
	mov r3, sb
	ldrh r3, [r3]
	ands r0, r3
	cmp r0, #0
	bne _0801C82A
	b _0801C95A
_0801C82A:
	mov r1, sb
	ldr r0, [r1]
	lsls r1, r0, #0x10
	lsrs r0, r1, #0x13
	ldr r2, [r5, #0x10]
	cmp r0, r2
	bhi _0801C878
	movs r0, #0x1c
	ldr r2, [sp, #8]
	str r0, [r2]
	ldr r3, [sp, #4]
	str r0, [r3]
	ldr r4, [r7, #0x54]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r6, sl
	strb r6, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r4, [r7, #0x58]
	adds r0, r4, #0
	adds r0, #0x2d
	strb r6, [r0]
	b _0801C8D4
	.align 2, 0
_0801C86C: .4byte gStaticData_0816C558
_0801C870: .4byte gStaticData_0816C86C
_0801C874: .4byte 0x0000FFF8
_0801C878:
	lsrs r0, r1, #0x13
	ldr r3, [r5, #0xc]
	cmp r0, r3
	bhi _0801C8EA
	adds r0, r2, #0
	adds r1, r6, #0
	bl FormatCentiseconds
	movs r0, #0x1c
	ldr r1, [sp, #4]
	str r0, [r1]
	ldr r4, [r7, #0x54]
	movs r0, #2
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
	ldr r4, [r7, #0x58]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r2, sl
	strb r2, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r4, [r7, #0x5c]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r3, sl
	strb r3, [r0]
_0801C8D4:
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	b _0801C95A
_0801C8EA:
	lsrs r1, r1, #0x13
	ldr r2, [sp]
	ldr r0, [r2, #8]
	cmp r1, r0
	bhi _0801C95A
	adds r0, r3, #0
	adds r1, r6, #0
	bl FormatCentiseconds
	movs r0, #0x1c
	ldr r3, [sp, #4]
	str r0, [r3]
	ldr r4, [r7, #0x54]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r6, r8
	strb r6, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r4, [r7, #0x58]
	movs r5, #2
	adds r0, r4, #0
	adds r0, #0x2d
	strb r5, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r4, [r7, #0x5c]
	adds r0, r4, #0
	adds r0, #0x2d
	strb r5, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
_0801C95A:
	add sp, #0xc
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801C96C
sub_801C96C: @ 0x0801C96C
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	movs r0, #0
	strb r0, [r5]
	ldr r1, [r5, #8]
	lsls r1, r1, #2
	adds r0, r5, #0
	adds r0, #0x24
	adds r0, r0, r1
	ldr r0, [r0]
	bl sub_801DE2C
	str r0, [r5, #0x10]
	lsls r4, r0, #3
	adds r4, r4, r0
	lsls r4, r4, #2
	ldr r0, _0801C9A8 @ =gStaticData_0816C86C
	adds r4, r4, r0
	ldr r0, [r5, #0x20]
	ldr r1, [r4, #4]
	bl sub_801DD80
	ldr r0, [r4]
	bl sub_8026F38
	str r0, [r5, #0x14]
	b _0801CA48
	.align 2, 0
_0801C9A8: .4byte gStaticData_0816C86C
_0801C9AC:
	adds r4, r5, #0
	adds r4, #0xa4
	ldrb r2, [r4]
	movs r1, #0x1f
	movs r0, #0x1f
	ands r0, r2
	cmp r0, #0
	beq _0801C9D0
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1b
	subs r0, #1
	ands r0, r1
	movs r3, #0x20
	rsbs r3, r3, #0
	adds r1, r3, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r4]
_0801C9D0:
	adds r0, r5, #0
	bl sub_801C104
	bl sub_80006A8
	ldr r0, _0801CADC @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801CAE0 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r5, #0x20]
	bl sub_801DCBC
	ldr r0, [r5, #0x7c]
	adds r0, #1
	str r0, [r5, #0x7c]
	ldr r1, _0801CAE4 @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r5, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801CAE8 @ =0x04000014
	str r0, [r1]
	ldr r0, [r5, #0x1c]
	bl sub_801E640
	ldr r1, _0801CAEC @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r5, #0x20]
	bl sub_801DE24
	ldr r1, _0801CAF0 @ =0x0400000C
	strh r0, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	strh r6, [r0]
	adds r1, #0x44
	adds r0, r5, #0
	adds r0, #0xa0
	ldr r0, [r0]
	str r0, [r1]
	adds r1, #4
	ldrb r4, [r4]
	lsls r0, r4, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	adds r0, r5, #0
	adds r0, #0xa8
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, [r5, #0x20]
	bl sub_801DAD8
_0801CA48:
	ldr r0, [r5, #0x20]
	bl sub_801DD18
	lsls r0, r0, #0x18
	lsrs r6, r0, #0x18
	cmp r6, #0
	beq _0801C9AC
	ldr r0, _0801CAF4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x51
	bl PlaySfx
	adds r3, r5, #0
	adds r3, #0xa0
	movs r6, #0
	str r6, [r3]
	adds r2, r5, #0
	adds r2, #0xa1
	movs r0, #1
	ldrb r4, [r2]
	orrs r0, r4
	movs r1, #2
	orrs r0, r1
	movs r1, #4
	orrs r0, r1
	movs r1, #8
	orrs r0, r1
	movs r1, #0x20
	orrs r0, r1
	strb r0, [r2]
	adds r4, r5, #0
	adds r4, #0xa2
	movs r1, #0x20
	rsbs r1, r1, #0
	adds r0, r1, #0
	ldrb r2, [r4]
	ands r0, r2
	movs r2, #0x10
	orrs r0, r2
	strb r0, [r4]
	adds r0, r5, #0
	adds r0, #0xa3
	ldrb r4, [r0]
	ands r1, r4
	orrs r1, r2
	strb r1, [r0]
	ldr r0, _0801CAF8 @ =gUnknown_03000824
	ldrb r0, [r0]
	mov sb, r3
	cmp r0, #0
	beq _0801CAC6
	adds r0, r5, #0
	bl sub_801D434
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801CAC6
	str r6, [r5, #8]
	adds r0, r5, #0
	bl sub_801D548
_0801CAC6:
	ldr r1, _0801CAF8 @ =gUnknown_03000824
	movs r0, #0
	strb r0, [r1]
	movs r0, #0x24
	adds r0, r0, r5
	mov r8, r0
	adds r7, r5, #0
	adds r7, #0xa4
	adds r6, r5, #0
	adds r6, #0xa8
	b _0801CB14
	.align 2, 0
_0801CADC: .4byte gUnknown_030012B8
_0801CAE0: .4byte gUnknown_03001300
_0801CAE4: .4byte 0x04000010
_0801CAE8: .4byte 0x04000014
_0801CAEC: .4byte 0x0400000A
_0801CAF0: .4byte 0x0400000C
_0801CAF4: .4byte gUnknown_030012BC
_0801CAF8: .4byte gUnknown_03000824
_0801CAFC:
	ldr r1, _0801CB10 @ =gUnknown_030007E0
	movs r0, #8
	ldrh r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _0801CB14
	adds r0, r5, #0
	bl sub_801D300
	b _0801CC50
	.align 2, 0
_0801CB10: .4byte gUnknown_030007E0
_0801CB14:
	adds r0, r5, #0
	bl sub_801C104
	bl sub_80006A8
	ldr r0, _0801CBC0 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801CBC4 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r5, #0x20]
	bl sub_801DCBC
	ldr r0, [r5, #0x7c]
	adds r0, #1
	str r0, [r5, #0x7c]
	ldr r1, _0801CBC8 @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r5, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801CBCC @ =0x04000014
	str r0, [r1]
	ldr r0, [r5, #0x1c]
	bl sub_801E640
	ldr r1, _0801CBD0 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r5, #0x20]
	bl sub_801DE24
	ldr r1, _0801CBD4 @ =0x0400000C
	strh r0, [r1]
	movs r1, #0xa0
	lsls r1, r1, #0x13
	movs r0, #0
	strh r0, [r1]
	ldr r1, _0801CBD8 @ =0x04000050
	mov r2, sb
	ldr r0, [r2]
	str r0, [r1]
	adds r1, #4
	ldrb r3, [r7]
	lsls r0, r3, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	ldrh r0, [r6]
	strh r0, [r1]
	adds r0, r5, #0
	bl sub_801C51C
	ldr r0, [r5, #0x1c]
	bl sub_801D780
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801CB14
	ldr r0, [r5, #0x3c]
	bl sub_801E464
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801CB14
	ldr r0, _0801CBDC @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r0, _0801CBE0 @ =gUnknown_030007E0
	ldr r2, [r0]
	lsrs r1, r2, #0x10
	movs r0, #0x40
	ands r0, r1
	cmp r0, #0
	beq _0801CBE4
	adds r0, r5, #0
	bl sub_801D548
	b _0801CC1A
	.align 2, 0
_0801CBC0: .4byte gUnknown_030012B8
_0801CBC4: .4byte gUnknown_03001300
_0801CBC8: .4byte 0x04000010
_0801CBCC: .4byte 0x04000014
_0801CBD0: .4byte 0x0400000A
_0801CBD4: .4byte 0x0400000C
_0801CBD8: .4byte 0x04000050
_0801CBDC: .4byte gUnknown_03001304
_0801CBE0: .4byte gUnknown_030007E0
_0801CBE4:
	lsrs r1, r2, #0x10
	movs r0, #0x80
	ands r0, r1
	adds r1, r2, #0
	cmp r0, #0
	beq _0801CBF8
	adds r0, r5, #0
	bl sub_801D4C4
	b _0801CC1A
_0801CBF8:
	lsrs r1, r1, #0x10
	movs r0, #0x20
	ands r0, r1
	cmp r0, #0
	beq _0801CC0A
	adds r0, r5, #0
	bl sub_801CDE0
	b _0801CC1A
_0801CC0A:
	lsrs r1, r2, #0x10
	movs r0, #0x10
	ands r0, r1
	cmp r0, #0
	beq _0801CC1A
	adds r0, r5, #0
	bl sub_801CE60
_0801CC1A:
	ldr r1, _0801CCDC @ =gUnknown_030007E0
	movs r0, #1
	ldrh r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	bne _0801CC28
	b _0801CAFC
_0801CC28:
	ldr r0, [r5, #8]
	lsls r0, r0, #2
	add r0, r8
	ldr r0, [r0]
	bl sub_801DE28
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801CC3C
	b _0801CAFC
_0801CC3C:
	ldr r0, [r5, #0x20]
	bl sub_801DD18
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801CC4A
	b _0801CAFC
_0801CC4A:
	adds r0, r5, #0
	bl sub_801D110
_0801CC50:
	movs r4, #0
	strh r4, [r6]
	movs r0, #0x40
	ldrb r1, [r6]
	orrs r0, r1
	strb r0, [r6]
	bl sub_80006A8
	ldr r0, _0801CCE0 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801CCE4 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r5, #0x20]
	bl sub_801DCBC
	ldr r0, [r5, #0x7c]
	adds r0, #1
	str r0, [r5, #0x7c]
	ldr r1, _0801CCE8 @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r5, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801CCEC @ =0x04000014
	str r0, [r1]
	ldr r0, [r5, #0x1c]
	bl sub_801E640
	ldr r1, _0801CCF0 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r5, #0x20]
	bl sub_801DE24
	ldr r1, _0801CCF4 @ =0x0400000C
	strh r0, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	strh r4, [r0]
	adds r1, #0x44
	mov r2, sb
	ldr r0, [r2]
	str r0, [r1]
	adds r1, #4
	ldrb r7, [r7]
	lsls r0, r7, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	ldrh r0, [r6]
	strh r0, [r1]
	ldr r0, [r5, #8]
	lsls r0, r0, #2
	add r0, r8
	ldr r0, [r0]
	bl sub_801DE2C
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0801CCDC: .4byte gUnknown_030007E0
_0801CCE0: .4byte gUnknown_030012B8
_0801CCE4: .4byte gUnknown_03001300
_0801CCE8: .4byte 0x04000010
_0801CCEC: .4byte 0x04000014
_0801CCF0: .4byte 0x0400000A
_0801CCF4: .4byte 0x0400000C

	thumb_func_start sub_801CCF8
sub_801CCF8: @ 0x0801CCF8
	push {r4, lr}
	adds r4, r0, #0
	ldr r1, [r4, #8]
	lsls r1, r1, #2
	adds r0, #0x24
	adds r0, r0, r1
	ldr r0, [r0]
	movs r1, #0
	bl sub_801DEA0
	ldr r0, [r4, #0x20]
	bl sub_801DD5C
	ldr r0, [r4, #0x3c]
	bl sub_801E408
	b _0801CD9E
_0801CD1A:
	adds r0, r4, #0
	bl sub_801C104
	bl sub_80006A8
	ldr r0, _0801CDC4 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801CDC8 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r4, #0x20]
	bl sub_801DCBC
	ldr r0, [r4, #0x7c]
	adds r0, #1
	str r0, [r4, #0x7c]
	ldr r1, _0801CDCC @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801CDD0 @ =0x04000014
	str r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801E640
	ldr r1, _0801CDD4 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r4, #0x20]
	bl sub_801DE24
	ldr r1, _0801CDD8 @ =0x0400000C
	strh r0, [r1]
	movs r1, #0xa0
	lsls r1, r1, #0x13
	movs r0, #0
	strh r0, [r1]
	ldr r1, _0801CDDC @ =0x04000050
	adds r0, r4, #0
	adds r0, #0xa0
	ldr r0, [r0]
	str r0, [r1]
	adds r1, #4
	adds r0, r4, #0
	adds r0, #0xa4
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	adds r0, r4, #0
	adds r0, #0xa8
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, [r4, #0x3c]
	bl sub_801E190
	ldr r0, [r4, #0x20]
	bl sub_801DAD8
_0801CD9E:
	ldr r0, [r4, #0x20]
	bl sub_801DD38
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801CD1A
	ldr r0, [r4, #0x3c]
	bl sub_801E464
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801CD1A
	ldr r0, _0801CDC4 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006EA8
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0801CDC4: .4byte gUnknown_030012B8
_0801CDC8: .4byte gUnknown_03001300
_0801CDCC: .4byte 0x04000010
_0801CDD0: .4byte 0x04000014
_0801CDD4: .4byte 0x0400000A
_0801CDD8: .4byte 0x0400000C
_0801CDDC: .4byte 0x04000050

	thumb_func_start sub_801CDE0
sub_801CDE0: @ 0x0801CDE0
	push {r4, lr}
	adds r4, r0, #0
	ldr r1, [r4, #8]
	cmp r1, #0
	bne _0801CE00
	ldr r0, _0801CDFC @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x48
	bl PlaySfx
	b _0801CE50
	.align 2, 0
_0801CDFC: .4byte gUnknown_030012BC
_0801CE00:
	lsls r1, r1, #2
	adds r0, r4, #0
	adds r0, #0x24
	adds r0, r0, r1
	ldr r0, [r0]
	movs r1, #0
	bl sub_801DEA0
	ldr r0, [r4, #0x20]
	bl sub_801DD5C
	b _0801CE4A
_0801CE18:
	ldr r0, [r4, #8]
	subs r0, #1
	str r0, [r4, #8]
	lsls r0, r0, #3
	ldr r2, [r4, #0x18]
	adds r2, r2, r0
	ldr r0, [r4, #0x3c]
	ldr r1, [r2]
	ldr r2, [r2, #4]
	subs r2, #0x18
	bl sub_801E480
	adds r0, r4, #0
	bl sub_801D05C
	ldr r0, _0801CE58 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r0, _0801CE5C @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #0x20
	ands r0, r1
	cmp r0, #0
	beq _0801CE50
_0801CE4A:
	ldr r0, [r4, #8]
	cmp r0, #0
	bne _0801CE18
_0801CE50:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0801CE58: .4byte gUnknown_03001304
_0801CE5C: .4byte gUnknown_030007E0

	thumb_func_start sub_801CE60
sub_801CE60: @ 0x0801CE60
	push {r4, lr}
	adds r4, r0, #0
	ldr r1, [r4, #8]
	ldr r0, [r4, #4]
	cmp r1, r0
	bne _0801CE80
	ldr r0, _0801CE7C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x48
	bl PlaySfx
	b _0801CED0
	.align 2, 0
_0801CE7C: .4byte gUnknown_030012BC
_0801CE80:
	lsls r1, r1, #2
	adds r0, r4, #0
	adds r0, #0x24
	adds r0, r0, r1
	ldr r0, [r0]
	movs r1, #0
	bl sub_801DEA0
	ldr r0, [r4, #0x20]
	bl sub_801DD5C
	b _0801CEC8
_0801CE98:
	adds r0, r1, #1
	str r0, [r4, #8]
	lsls r0, r0, #3
	ldr r2, [r4, #0x18]
	adds r2, r2, r0
	ldr r0, [r4, #0x3c]
	ldr r1, [r2]
	ldr r2, [r2, #4]
	subs r2, #0x18
	bl sub_801E480
	adds r0, r4, #0
	bl sub_801D05C
	ldr r0, _0801CED8 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r0, _0801CEDC @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #0x10
	ands r0, r1
	cmp r0, #0
	beq _0801CED0
_0801CEC8:
	ldr r1, [r4, #8]
	ldr r0, [r4, #4]
	cmp r1, r0
	blt _0801CE98
_0801CED0:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0801CED8: .4byte gUnknown_03001304
_0801CEDC: .4byte gUnknown_030007E0

	thumb_func_start sub_801CEE0
sub_801CEE0: @ 0x0801CEE0
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r4, r0, #0
	b _0801D036
_0801CEEC:
	adds r0, r4, #0
	bl sub_801C104
	bl sub_80006A8
	ldr r0, _0801CFD4 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801CFD8 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r4, #0x20]
	bl sub_801DCBC
	ldr r0, [r4, #0x7c]
	adds r0, #1
	str r0, [r4, #0x7c]
	ldr r1, _0801CFDC @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801CFE0 @ =0x04000014
	str r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801E640
	ldr r1, _0801CFE4 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r4, #0x20]
	bl sub_801DE24
	ldr r1, _0801CFE8 @ =0x0400000C
	strh r0, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	strh r5, [r0]
	adds r1, #0x44
	adds r0, r4, #0
	adds r0, #0xa0
	ldr r0, [r0]
	str r0, [r1]
	adds r1, #4
	adds r0, r4, #0
	adds r0, #0xa4
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	adds r0, r4, #0
	adds r0, #0xa8
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801D7AC
	ldr r0, [r4, #0x3c]
	bl sub_801E190
	ldr r0, [r4, #0x1c]
	bl sub_801D77C
	movs r1, #0xff
	ands r1, r0
	cmp r1, #0xa0
	bne _0801D036
	movs r5, #0
	adds r7, r4, #0
	adds r7, #0x24
	movs r0, #0x9c
	adds r0, r0, r4
	mov r8, r0
	adds r6, r7, #0
_0801CF8A:
	ldm r6!, {r0}
	ldr r2, [r0, #0x10]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r4, #0xc]
	ldr r3, [r2, #0x14]
	adds r2, r5, #0
	bl sub_803AD84
	adds r5, #1
	cmp r5, #5
	ble _0801CF8A
	movs r3, #0
	movs r2, #0
	ldr r0, [r4, #0xc]
	lsls r1, r0, #2
	adds r1, r1, r0
	mov r5, r8
	ldr r0, [r5]
	lsls r1, r1, #2
	adds r1, r1, r0
_0801CFB6:
	ldrb r5, [r1, #4]
	lsls r0, r5, #0x1f
	lsrs r0, r0, #0x1f
	adds r3, r3, r0
	adds r1, #4
	adds r2, #1
	cmp r2, #4
	ble _0801CFB6
	cmp r3, #5
	bne _0801CFF0
	ldr r0, _0801CFEC @ =gStaticData_0816C508
	str r0, [r4, #0x18]
	str r3, [r4, #4]
	b _0801CFF8
	.align 2, 0
_0801CFD4: .4byte gUnknown_030012B8
_0801CFD8: .4byte gUnknown_03001300
_0801CFDC: .4byte 0x04000010
_0801CFE0: .4byte 0x04000014
_0801CFE4: .4byte 0x0400000A
_0801CFE8: .4byte 0x0400000C
_0801CFEC: .4byte gStaticData_0816C508
_0801CFF0:
	ldr r0, _0801D054 @ =gStaticData_0816C4D8
	str r0, [r4, #0x18]
	movs r0, #4
	str r0, [r4, #4]
_0801CFF8:
	movs r5, #0
	adds r6, r7, #0
_0801CFFC:
	ldm r6!, {r0}
	ldr r3, [r0, #0x10]
	movs r2, #0x18
	ldrsh r1, [r3, r2]
	adds r0, r0, r1
	lsls r2, r5, #3
	ldr r1, [r4, #0x18]
	adds r1, r1, r2
	ldr r2, [r3, #0x1c]
	bl sub_803AD80
	adds r5, #1
	cmp r5, #5
	ble _0801CFFC
	ldr r2, _0801D058 @ =gStaticData_0816C538
	adds r6, r7, #0
	movs r5, #5
_0801D01E:
	ldm r6!, {r0}
	ldr r1, [r4, #0xc]
	lsls r1, r1, #2
	adds r1, r1, r2
	ldr r1, [r1]
	str r2, [sp]
	bl sub_801DF0C
	subs r5, #1
	ldr r2, [sp]
	cmp r5, #0
	bge _0801D01E
_0801D036:
	ldr r0, [r4, #0x1c]
	bl sub_801D780
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	cmp r5, #0
	bne _0801D046
	b _0801CEEC
_0801D046:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801D054: .4byte gStaticData_0816C4D8
_0801D058: .4byte gStaticData_0816C538

	thumb_func_start sub_801D05C
sub_801D05C: @ 0x0801D05C
	push {r4, r5, lr}
	adds r4, r0, #0
	b _0801D0E4
_0801D062:
	adds r0, r4, #0
	bl sub_801C104
	bl sub_80006A8
	ldr r0, _0801D0F8 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801D0FC @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r4, #0x20]
	bl sub_801DCBC
	ldr r0, [r4, #0x7c]
	adds r0, #1
	str r0, [r4, #0x7c]
	ldr r1, _0801D100 @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801D104 @ =0x04000014
	str r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801E640
	ldr r1, _0801D108 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r4, #0x20]
	bl sub_801DE24
	ldr r1, _0801D10C @ =0x0400000C
	strh r0, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	strh r5, [r0]
	adds r1, #0x44
	adds r0, r4, #0
	adds r0, #0xa0
	ldr r0, [r0]
	str r0, [r1]
	adds r1, #4
	adds r0, r4, #0
	adds r0, #0xa4
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	adds r0, r4, #0
	adds r0, #0xa8
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, [r4, #0x3c]
	bl sub_801E190
	ldr r0, [r4, #0x20]
	bl sub_801DAD8
_0801D0E4:
	ldr r0, [r4, #0x3c]
	bl sub_801E464
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	cmp r5, #0
	beq _0801D062
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0801D0F8: .4byte gUnknown_030012B8
_0801D0FC: .4byte gUnknown_03001300
_0801D100: .4byte 0x04000010
_0801D104: .4byte 0x04000014
_0801D108: .4byte 0x0400000A
_0801D10C: .4byte 0x0400000C

	thumb_func_start sub_801D110
sub_801D110: @ 0x0801D110
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	ldr r0, _0801D150 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x52
	bl PlaySfx
	ldr r1, [r6, #8]
	lsls r1, r1, #2
	adds r0, r6, #0
	adds r0, #0x24
	adds r0, r0, r1
	ldr r0, [r0]
	movs r1, #0
	bl sub_801DEA0
	ldr r0, [r6, #0x3c]
	movs r1, #0x78
	movs r2, #0x35
	bl sub_801E480
	ldr r0, [r6, #0x3c]
	bl sub_801E3F4
	adds r0, r6, #0
	bl sub_801D05C
	b _0801D1D6
	.align 2, 0
_0801D150: .4byte gUnknown_030012BC
_0801D154:
	adds r0, r6, #0
	bl sub_801C104
	bl sub_80006A8
	ldr r0, _0801D224 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801D228 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r6, #0x20]
	bl sub_801DCBC
	ldr r0, [r6, #0x7c]
	adds r0, #1
	str r0, [r6, #0x7c]
	ldr r1, _0801D22C @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r6, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801D230 @ =0x04000014
	str r0, [r1]
	ldr r0, [r6, #0x1c]
	bl sub_801E640
	ldr r1, _0801D234 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r6, #0x20]
	bl sub_801DE24
	ldr r1, _0801D238 @ =0x0400000C
	strh r0, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	strh r4, [r0]
	adds r1, #0x44
	adds r0, r6, #0
	adds r0, #0xa0
	ldr r0, [r0]
	str r0, [r1]
	adds r1, #4
	adds r0, r6, #0
	adds r0, #0xa4
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	adds r0, r6, #0
	adds r0, #0xa8
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, [r6, #0x3c]
	bl sub_801E190
	ldr r0, [r6, #0x20]
	bl sub_801DAD8
_0801D1D6:
	ldr r0, [r6, #0x20]
	bl sub_801DD18
	lsls r0, r0, #0x18
	lsrs r4, r0, #0x18
	cmp r4, #0
	beq _0801D154
	adds r5, r6, #0
	adds r5, #0xa0
	movs r0, #0xc0
	ldrb r1, [r5]
	orrs r0, r1
	movs r1, #0x20
	orrs r0, r1
	movs r1, #1
	orrs r0, r1
	movs r1, #2
	orrs r0, r1
	movs r1, #4
	orrs r0, r1
	movs r1, #8
	orrs r0, r1
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r5]
	adds r4, r6, #0
	adds r4, #0xa4
	movs r0, #0x20
	rsbs r0, r0, #0
	ldrb r2, [r4]
	ands r0, r2
	strb r0, [r4]
	movs r7, #0
	ldr r0, [r6, #0x20]
	bl sub_801DD48
	mov r8, r5
	b _0801D2D0
	.align 2, 0
_0801D224: .4byte gUnknown_030012B8
_0801D228: .4byte gUnknown_03001300
_0801D22C: .4byte 0x04000010
_0801D230: .4byte 0x04000014
_0801D234: .4byte 0x0400000A
_0801D238: .4byte 0x0400000C
_0801D23C:
	adds r0, r6, #0
	bl sub_801C104
	bl sub_80006A8
	ldr r0, _0801D2E8 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801D2EC @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r6, #0x20]
	bl sub_801DCBC
	ldr r0, [r6, #0x7c]
	adds r0, #1
	str r0, [r6, #0x7c]
	ldr r1, _0801D2F0 @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r6, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801D2F4 @ =0x04000014
	str r0, [r1]
	ldr r0, [r6, #0x1c]
	bl sub_801E640
	ldr r1, _0801D2F8 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r6, #0x20]
	bl sub_801DE24
	ldr r1, _0801D2FC @ =0x0400000C
	strh r0, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	strh r5, [r0]
	adds r1, #0x44
	mov r2, r8
	ldr r0, [r2]
	str r0, [r1]
	adds r1, #4
	ldrb r2, [r4]
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	adds r0, r6, #0
	adds r0, #0xa8
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, [r6, #0x3c]
	bl sub_801E190
	ldr r0, [r6, #0x20]
	bl sub_801DAD8
	adds r7, #1
	lsrs r1, r7, #0x1f
	adds r1, r7, r1
	asrs r1, r1, #1
	movs r0, #0x1f
	ands r1, r0
	movs r0, #0x20
	rsbs r0, r0, #0
	ldrb r2, [r4]
	ands r0, r2
	orrs r0, r1
	strb r0, [r4]
_0801D2D0:
	ldr r0, [r6, #0x20]
	bl sub_801DD08
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	cmp r5, #0
	beq _0801D23C
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801D2E8: .4byte gUnknown_030012B8
_0801D2EC: .4byte gUnknown_03001300
_0801D2F0: .4byte 0x04000010
_0801D2F4: .4byte 0x04000014
_0801D2F8: .4byte 0x0400000A
_0801D2FC: .4byte 0x0400000C

	thumb_func_start sub_801D300
sub_801D300: @ 0x0801D300
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	ldr r0, _0801D350 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
	adds r2, r5, #0
	adds r2, #0xa0
	movs r0, #0xc0
	ldrb r1, [r2]
	orrs r0, r1
	movs r1, #0x20
	orrs r0, r1
	movs r1, #1
	orrs r0, r1
	movs r1, #2
	orrs r0, r1
	movs r1, #4
	orrs r0, r1
	movs r1, #8
	orrs r0, r1
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r2]
	adds r4, r5, #0
	adds r4, #0xa4
	movs r0, #0x20
	rsbs r0, r0, #0
	ldrb r2, [r4]
	ands r0, r2
	strb r0, [r4]
	movs r7, #0
	ldr r0, [r5, #0x20]
	bl sub_801DD5C
	adds r6, r4, #0
	b _0801D3EC
	.align 2, 0
_0801D350: .4byte gUnknown_030012BC
_0801D354:
	adds r0, r5, #0
	bl sub_801C104
	bl sub_80006A8
	ldr r0, _0801D404 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801D408 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r5, #0x20]
	bl sub_801DCBC
	ldr r0, [r5, #0x7c]
	adds r0, #1
	str r0, [r5, #0x7c]
	ldr r1, _0801D40C @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r5, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801D410 @ =0x04000014
	str r0, [r1]
	ldr r0, [r5, #0x1c]
	bl sub_801E640
	ldr r1, _0801D414 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r5, #0x20]
	bl sub_801DE24
	ldr r1, _0801D418 @ =0x0400000C
	strh r0, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	strh r4, [r0]
	adds r1, #0x44
	adds r0, r5, #0
	adds r0, #0xa0
	ldr r0, [r0]
	str r0, [r1]
	adds r1, #4
	ldrb r2, [r6]
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	adds r0, r5, #0
	adds r0, #0xa8
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, [r5, #0x3c]
	bl sub_801E190
	ldr r0, [r5, #0x20]
	bl sub_801DAD8
	adds r7, #1
	lsrs r1, r7, #0x1f
	adds r1, r7, r1
	asrs r1, r1, #1
	movs r0, #0x1f
	ands r1, r0
	movs r2, #0x20
	rsbs r2, r2, #0
	adds r0, r2, #0
	ldrb r2, [r6]
	ands r0, r2
	orrs r0, r1
	strb r0, [r6]
_0801D3EC:
	ldr r0, [r5, #0x20]
	bl sub_801DD28
	lsls r0, r0, #0x18
	lsrs r4, r0, #0x18
	cmp r4, #0
	beq _0801D354
	movs r0, #1
	strb r0, [r5]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801D404: .4byte gUnknown_030012B8
_0801D408: .4byte gUnknown_03001300
_0801D40C: .4byte 0x04000010
_0801D410: .4byte 0x04000014
_0801D414: .4byte 0x0400000A
_0801D418: .4byte 0x0400000C

	thumb_func_start sub_801D41C
sub_801D41C: @ 0x0801D41C
	ldr r1, _0801D424 @ =gUnknown_03000824
	movs r0, #1
	strb r0, [r1]
	bx lr
	.align 2, 0
_0801D424: .4byte gUnknown_03000824

	thumb_func_start sub_801D428
sub_801D428: @ 0x0801D428
	ldr r1, [r0, #0xc]
	rsbs r0, r1, #0
	orrs r0, r1
	lsrs r0, r0, #0x1f
	bx lr
	.align 2, 0

	thumb_func_start sub_801D434
sub_801D434: @ 0x0801D434
	movs r2, #0
	ldr r1, [r0, #0xc]
	cmp r1, #1
	beq _0801D456
	cmp r1, #1
	bgt _0801D446
	cmp r1, #0
	beq _0801D44C
	b _0801D46C
_0801D446:
	cmp r1, #2
	beq _0801D460
	b _0801D46C
_0801D44C:
	adds r0, #0x9c
	ldr r0, [r0]
	ldrb r0, [r0, #2]
	lsrs r2, r0, #5
	b _0801D468
_0801D456:
	adds r0, #0x9c
	ldr r0, [r0]
	ldrb r0, [r0, #2]
	lsrs r2, r0, #7
	b _0801D46C
_0801D460:
	adds r0, #0x9c
	ldr r0, [r0]
	ldrb r0, [r0, #2]
	lsrs r2, r0, #6
_0801D468:
	movs r0, #1
	ands r2, r0
_0801D46C:
	adds r0, r2, #0
	bx lr

	thumb_func_start sub_801D470
sub_801D470: @ 0x0801D470
	push {r4, r5, lr}
	adds r5, r0, #0
	ldr r1, _0801D4C0 @ =gStaticData_0816C548
	ldr r0, [r5, #0xc]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r4, [r5, #0x40]
	ldr r0, [r0]
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
	ldr r0, [r5, #8]
	ldr r1, [r5, #4]
	cmp r0, r1
	ble _0801D4A4
	str r1, [r5, #8]
_0801D4A4:
	ldr r0, [r5, #8]
	lsls r0, r0, #3
	ldr r2, [r5, #0x18]
	adds r2, r2, r0
	ldr r0, [r5, #0x3c]
	ldr r1, [r2]
	ldr r2, [r2, #4]
	subs r2, #0x18
	bl sub_801E480
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0801D4C0: .4byte gStaticData_0816C548

	thumb_func_start sub_801D4C4
sub_801D4C4: @ 0x0801D4C4
	push {r4, lr}
	adds r4, r0, #0
	bl sub_801D428
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801D530
	adds r0, r4, #0
	bl sub_801CCF8
	ldr r0, _0801D4E8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x56
	bl PlaySfx
	b _0801D512
	.align 2, 0
_0801D4E8: .4byte gUnknown_030012BC
_0801D4EC:
	ldr r0, [r4, #0xc]
	subs r0, #1
	str r0, [r4, #0xc]
	ldr r0, [r4, #0x1c]
	bl sub_801D790
	adds r0, r4, #0
	bl sub_801CEE0
	ldr r0, _0801D528 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r0, _0801D52C @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #0x80
	ands r0, r1
	cmp r0, #0
	beq _0801D51E
_0801D512:
	adds r0, r4, #0
	bl sub_801D428
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801D4EC
_0801D51E:
	adds r0, r4, #0
	bl sub_801D470
	b _0801D53E
	.align 2, 0
_0801D528: .4byte gUnknown_03001304
_0801D52C: .4byte gUnknown_030007E0
_0801D530:
	ldr r0, _0801D544 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x48
	bl PlaySfx
_0801D53E:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0801D544: .4byte gUnknown_030012BC

	thumb_func_start sub_801D548
sub_801D548: @ 0x0801D548
	push {r4, lr}
	adds r4, r0, #0
	bl sub_801D434
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801D5B4
	adds r0, r4, #0
	bl sub_801CCF8
	ldr r0, _0801D56C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x55
	bl PlaySfx
	b _0801D596
	.align 2, 0
_0801D56C: .4byte gUnknown_030012BC
_0801D570:
	ldr r0, [r4, #0xc]
	adds r0, #1
	str r0, [r4, #0xc]
	ldr r0, [r4, #0x1c]
	bl sub_801D79C
	adds r0, r4, #0
	bl sub_801CEE0
	ldr r0, _0801D5AC @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r0, _0801D5B0 @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #0x40
	ands r0, r1
	cmp r0, #0
	beq _0801D5A2
_0801D596:
	adds r0, r4, #0
	bl sub_801D434
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801D570
_0801D5A2:
	adds r0, r4, #0
	bl sub_801D470
	b _0801D5C2
	.align 2, 0
_0801D5AC: .4byte gUnknown_03001304
_0801D5B0: .4byte gUnknown_030007E0
_0801D5B4:
	ldr r0, _0801D5C8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x48
	bl PlaySfx
_0801D5C2:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0801D5C8: .4byte gUnknown_030012BC

	thumb_func_start sub_801D5CC
sub_801D5CC: @ 0x0801D5CC
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	movs r3, #0
	movs r2, #0
	ldr r1, [r5, #0xc]
	lsls r0, r1, #2
	adds r0, r0, r1
	adds r1, r5, #0
	adds r1, #0x9c
	ldr r1, [r1]
	lsls r0, r0, #2
	adds r1, r0, r1
_0801D5E4:
	ldrb r4, [r1, #4]
	lsls r0, r4, #0x1f
	lsrs r0, r0, #0x1f
	adds r3, r3, r0
	adds r1, #4
	adds r2, #1
	cmp r2, #4
	ble _0801D5E4
	cmp r3, #5
	bne _0801D604
	ldr r0, _0801D600 @ =gStaticData_0816C508
	str r0, [r5, #0x18]
	str r3, [r5, #4]
	b _0801D60C
	.align 2, 0
_0801D600: .4byte gStaticData_0816C508
_0801D604:
	ldr r0, _0801D634 @ =gStaticData_0816C4D8
	str r0, [r5, #0x18]
	movs r0, #4
	str r0, [r5, #4]
_0801D60C:
	movs r4, #0
	adds r6, r5, #0
	adds r6, #0x24
_0801D612:
	ldm r6!, {r0}
	ldr r3, [r0, #0x10]
	movs r2, #0x18
	ldrsh r1, [r3, r2]
	adds r0, r0, r1
	lsls r2, r4, #3
	ldr r1, [r5, #0x18]
	adds r1, r1, r2
	ldr r2, [r3, #0x1c]
	bl sub_803AD80
	adds r4, #1
	cmp r4, #5
	ble _0801D612
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801D634: .4byte gStaticData_0816C4D8

	thumb_func_start sub_801D638
sub_801D638: @ 0x0801D638
	push {r4, r5, lr}
	adds r5, r0, #0
	movs r4, #0
_0801D63E:
	lsls r1, r4, #2
	adds r0, r5, #0
	adds r0, #0x24
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r2, [r0, #0x10]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r5, #0xc]
	ldr r3, [r2, #0x14]
	adds r2, r4, #0
	bl sub_803AD84
	adds r4, #1
	cmp r4, #5
	ble _0801D63E
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801D668
sub_801D668: @ 0x0801D668
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	movs r4, #0
	ldr r6, _0801D694 @ =gStaticData_0816C538
_0801D670:
	lsls r1, r4, #2
	adds r0, r5, #0
	adds r0, #0x24
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r5, #0xc]
	lsls r1, r1, #2
	adds r1, r1, r6
	ldr r1, [r1]
	bl sub_801DF0C
	adds r4, #1
	cmp r4, #5
	ble _0801D670
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801D694: .4byte gStaticData_0816C538

	thumb_func_start sub_801D698
sub_801D698: @ 0x0801D698
	push {r4, lr}
	adds r4, r0, #0
	bl sub_80006A8
	ldr r0, _0801D714 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801D718 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r4, #0x20]
	bl sub_801DCBC
	ldr r0, [r4, #0x7c]
	adds r0, #1
	str r0, [r4, #0x7c]
	ldr r1, _0801D71C @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801D720 @ =0x04000014
	str r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801E640
	ldr r1, _0801D724 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r4, #0x20]
	bl sub_801DE24
	ldr r1, _0801D728 @ =0x0400000C
	strh r0, [r1]
	movs r1, #0xa0
	lsls r1, r1, #0x13
	movs r0, #0
	strh r0, [r1]
	ldr r1, _0801D72C @ =0x04000050
	adds r0, r4, #0
	adds r0, #0xa0
	ldr r0, [r0]
	str r0, [r1]
	adds r1, #4
	adds r0, r4, #0
	adds r0, #0xa4
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	adds r0, r4, #0
	adds r0, #0xa8
	ldrh r0, [r0]
	strh r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0801D714: .4byte gUnknown_030012B8
_0801D718: .4byte gUnknown_03001300
_0801D71C: .4byte 0x04000010
_0801D720: .4byte 0x04000014
_0801D724: .4byte 0x0400000A
_0801D728: .4byte 0x0400000C
_0801D72C: .4byte 0x04000050

	thumb_func_start sub_801D730
sub_801D730: @ 0x0801D730
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r0, _0801D778 @ =gUnknown_030012B8
	ldr r0, [r0]
	movs r1, #0xf
	bl sub_8006D50
	movs r5, #0
	movs r0, #0x10
	rsbs r0, r0, #0
	adds r7, r0, #0
_0801D746:
	lsls r0, r5, #2
	adds r4, r6, #0
	adds r4, #0x40
	adds r4, r4, r0
	ldr r0, [r4]
	bl sub_800815C
	ldr r2, [r4]
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	ldrb r1, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	adds r5, #1
	cmp r5, #7
	ble _0801D746
	adds r0, r6, #0
	bl sub_801D668
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801D778: .4byte gUnknown_030012B8

	thumb_func_start sub_801D77C
sub_801D77C: @ 0x0801D77C
	ldr r0, [r0, #0x10]
	bx lr

	thumb_func_start sub_801D780
sub_801D780: @ 0x0801D780
	movs r2, #0
	ldr r1, [r0, #0x10]
	ldr r0, [r0, #0x14]
	cmp r1, r0
	bne _0801D78C
	movs r2, #1
_0801D78C:
	adds r0, r2, #0
	bx lr

	thumb_func_start sub_801D790
sub_801D790: @ 0x0801D790
	ldr r1, [r0, #0x14]
	movs r2, #0x80
	lsls r2, r2, #1
	adds r1, r1, r2
	str r1, [r0, #0x14]
	bx lr

	thumb_func_start sub_801D79C
sub_801D79C: @ 0x0801D79C
	ldr r1, [r0, #0x14]
	ldr r2, _0801D7A8 @ =0xFFFFFF00
	adds r1, r1, r2
	str r1, [r0, #0x14]
	bx lr
	.align 2, 0
_0801D7A8: .4byte 0xFFFFFF00

	thumb_func_start sub_801D7AC
sub_801D7AC: @ 0x0801D7AC
	adds r1, r0, #0
	ldr r2, [r1, #0x10]
	ldr r0, [r1, #0x14]
	cmp r2, r0
	bge _0801D7BC
	adds r0, r2, #0
	adds r0, #8
	str r0, [r1, #0x10]
_0801D7BC:
	ldr r2, [r1, #0x10]
	ldr r0, [r1, #0x14]
	cmp r2, r0
	ble _0801D7CA
	adds r0, r2, #0
	subs r0, #8
	str r0, [r1, #0x10]
_0801D7CA:
	ldr r0, [r1, #0x10]
	strh r0, [r1, #0x26]
	bx lr

	thumb_func_start sub_801D7D0
sub_801D7D0: @ 0x0801D7D0
	ldr r0, [r0, #0x24]
	bx lr

	thumb_func_start sub_801D7D4
sub_801D7D4: @ 0x0801D7D4
	movs r1, #8
	strh r1, [r0, #0x24]
	ldrh r1, [r0, #0x10]
	adds r1, #0x30
	strh r1, [r0, #0x26]
	bx lr

	thumb_func_start sub_801D7E0
sub_801D7E0: @ 0x0801D7E0
	push {lr}
	adds r2, r0, #0
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _0801D7F2
	adds r0, r2, #0
	bl sub_8026ED0
_0801D7F2:
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801D7F8
sub_801D7F8: @ 0x0801D7F8
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	movs r0, #2
	str r0, [sp]
	adds r0, r4, #0
	movs r3, #0
	bl sub_801E644
	movs r0, #0xc0
	lsls r0, r0, #2
	str r0, [r4, #0x14]
	str r0, [r4, #0x10]
	ldr r1, _0801D824 @ =gStaticData_0816C58C
	adds r0, r4, #0
	bl LoadGraphicsPackage
	adds r0, r4, #0
	add sp, #4
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801D824: .4byte gStaticData_0816C58C

	thumb_func_start sub_801D828
sub_801D828: @ 0x0801D828
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xc
	adds r6, r0, #0
	movs r0, #0x78
	str r0, [r6, #0x20]
	movs r0, #0x35
	str r0, [r6, #0x24]
	movs r5, #0
	strh r5, [r6, #0x34]
	adds r4, r6, #0
	adds r4, #0x34
	subs r0, #0x39
	ldrb r3, [r4]
	ands r0, r3
	movs r3, #1
	orrs r0, r3
	strb r0, [r4]
	str r1, [r6, #0x18]
	movs r0, #3
	ands r1, r0
	lsls r1, r1, #2
	movs r0, #0xd
	rsbs r0, r0, #0
	ldrb r3, [r4]
	ands r0, r3
	orrs r0, r1
	strb r0, [r4]
	adds r1, r6, #0
	adds r1, #0x35
	movs r0, #0x3f
	ldrb r3, [r1]
	ands r0, r3
	strb r0, [r1]
	str r2, [r6, #0x1c]
	movs r0, #0x1f
	ands r2, r0
	movs r0, #0x20
	rsbs r0, r0, #0
	ldrb r3, [r1]
	ands r0, r3
	orrs r0, r2
	strb r0, [r1]
	movs r0, #0x80
	ldrb r1, [r4]
	orrs r0, r1
	strb r0, [r4]
	ldr r0, [r6, #0x1c]
	lsls r0, r0, #0xb
	movs r2, #0xc0
	lsls r2, r2, #0x13
	adds r3, r0, r2
	mov r0, sp
	strh r5, [r0]
	ldr r0, _0801DA20 @ =0x040000D4
	mov r4, sp
	str r4, [r0]
	str r3, [r0, #4]
	ldr r1, _0801DA24 @ =0x81000080
	str r1, [r0, #8]
	ldr r0, [r0, #8]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r0, #0
	ldr r1, [r6, #0x20]
	mov sl, r1
	movs r4, #0x40
	adds r4, r4, r6
	mov r8, r4
	movs r1, #0x42
	adds r1, r1, r6
	mov ip, r1
	movs r4, #0x48
	adds r4, r4, r6
	mov sb, r4
	adds r1, r6, #0
	adds r1, #0x64
	str r1, [sp, #4]
	adds r4, r6, #0
	adds r4, #0x88
	str r4, [sp, #8]
	ldr r7, _0801DA28 @ =0x00000202
_0801D8D2:
	adds r5, r3, #0
	adds r5, #0x10
	adds r4, r0, #1
	adds r0, r3, #0
	movs r1, #3
_0801D8DC:
	strh r2, [r0]
	adds r2, r2, r7
	adds r0, #2
	subs r1, #1
	cmp r1, #0
	bge _0801D8DC
	adds r3, r5, #0
	adds r0, r4, #0
	cmp r0, #7
	ble _0801D8D2
	movs r0, #0xb
	str r0, [r6, #0x10]
	movs r0, #2
	str r0, [r6, #0xc]
	movs r0, #8
	str r0, [r6, #0x14]
	movs r1, #0
	str r1, [r6, #0x30]
	str r1, [r6, #0x2c]
	str r1, [r6, #0x28]
	movs r0, #0x80
	lsls r0, r0, #6
	str r0, [r6, #0x38]
	str r0, [r6, #0x3c]
	mov r2, sl
	mov r0, r8
	strh r2, [r0]
	ldr r0, [r6, #0x24]
	mov r3, ip
	strh r0, [r3]
	mov r4, sb
	strh r1, [r4]
	adds r5, r6, #0
	adds r5, #0x5c
	ldr r4, [sp, #4]
	ldr r7, _0801DA2C @ =gStaticData_0816C5F0
	movs r0, #1
	mov sb, r0
	movs r1, #3
	mov r8, r1
_0801D92C:
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	str r0, [r4]
	ldr r1, _0801DA30 @ =gUnknown_030012D0
	ldr r1, [r1]
	ldr r1, [r1]
	ldr r1, [r1]
	movs r2, #0x96
	lsls r2, r2, #2
	adds r1, r1, r2
	str r1, [r0, #0x20]
	adds r0, #0x28
	movs r3, #4
	rsbs r3, r3, #0
	adds r1, r3, #0
	ldrb r2, [r0]
	ands r1, r2
	mov r3, sb
	orrs r1, r3
	strb r1, [r0]
	ldr r0, [r4]
	ldr r3, [r6, #0x20]
	ldr r1, [r7]
	adds r3, r3, r1
	ldr r2, [r6, #0x24]
	ldr r1, [r7, #4]
	adds r2, r2, r1
	lsls r3, r3, #8
	str r3, [r0]
	lsls r2, r2, #8
	str r2, [r0, #4]
	movs r1, #1
	bl sub_80088D8
	ldr r0, [r6, #0x64]
	bl sub_800815C
	ldr r2, [r4]
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r3, #0x10
	rsbs r3, r3, #0
	adds r1, r3, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	adds r0, r6, #0
	adds r1, r5, #0
	bl sub_801DDB4
	adds r5, #0xc
	adds r4, #0xc
	adds r7, #8
	movs r0, #1
	rsbs r0, r0, #0
	add r8, r0
	mov r1, r8
	cmp r1, #0
	bge _0801D92C
	ldr r0, _0801DA34 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r2, [r6, #0x64]
	ldr r1, [r2, #0x20]
	adds r2, #0x2d
	ldr r3, [r1]
	ldrb r4, [r2]
	lsls r1, r4, #3
	subs r1, r1, r4
	lsls r1, r1, #2
	adds r1, r1, r3
	ldrb r1, [r1, #0x14]
	bl sub_8006D84
	ldr r1, [r6, #0x70]
	adds r1, #0x28
	movs r3, #0x11
	rsbs r3, r3, #0
	adds r0, r3, #0
	ldrb r2, [r1]
	ands r0, r2
	movs r5, #0x10
	orrs r0, r5
	strb r0, [r1]
	ldr r2, [r6, #0x7c]
	adds r2, #0x28
	movs r1, #0x21
	rsbs r1, r1, #0
	adds r0, r1, #0
	ldrb r4, [r2]
	ands r0, r4
	movs r4, #0x20
	orrs r0, r4
	strb r0, [r2]
	ldr r2, [sp, #8]
	ldr r0, [r2]
	adds r0, #0x28
	ldrb r2, [r0]
	ands r3, r2
	orrs r3, r5
	strb r3, [r0]
	ldr r3, [sp, #8]
	ldr r0, [r3]
	adds r0, #0x28
	ldrb r2, [r0]
	ands r1, r2
	orrs r1, r4
	strb r1, [r0]
	adds r0, r6, #0
	add sp, #0xc
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0801DA20: .4byte 0x040000D4
_0801DA24: .4byte 0x81000080
_0801DA28: .4byte 0x00000202
_0801DA2C: .4byte gStaticData_0816C5F0
_0801DA30: .4byte gUnknown_030012D0
_0801DA34: .4byte gUnknown_030012B8

	thumb_func_start sub_801DA38
sub_801DA38: @ 0x0801DA38
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, _0801DAD4 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r2, [r4, #0x64]
	ldr r1, [r2, #0x20]
	adds r2, #0x2d
	ldr r3, [r1]
	ldrb r6, [r2]
	lsls r1, r6, #3
	subs r1, r1, r6
	lsls r1, r1, #2
	adds r1, r1, r3
	ldrb r1, [r1, #0x14]
	bl sub_8006D68
	adds r0, r4, #0
	adds r0, #0x88
	ldr r2, [r0]
	cmp r2, #0
	beq _0801DA76
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0801DA76:
	ldr r2, [r4, #0x7c]
	cmp r2, #0
	beq _0801DA8E
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r6, #0
	ldrsh r0, [r1, r6]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0801DA8E:
	ldr r2, [r4, #0x70]
	cmp r2, #0
	beq _0801DAA6
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0801DAA6:
	ldr r2, [r4, #0x64]
	cmp r2, #0
	beq _0801DABE
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r6, #0
	ldrsh r0, [r1, r6]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0801DABE:
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _0801DACC
	adds r0, r4, #0
	bl sub_8026ED0
_0801DACC:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801DAD4: .4byte gUnknown_030012B8

	thumb_func_start sub_801DAD8
sub_801DAD8: @ 0x0801DAD8
	push {r4, r5, lr}
	ldr r4, _0801DAF4 @ =0xFFFFFE00
	add sp, r4
	adds r5, r0, #0
	ldr r0, [r5, #0xc]
	cmp r0, #4
	bls _0801DAE8
	b _0801DBF2
_0801DAE8:
	lsls r0, r0, #2
	ldr r1, _0801DAF8 @ =_0801DAFC
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801DAF4: .4byte 0xFFFFFE00
_0801DAF8: .4byte _0801DAFC
_0801DAFC: @ jump table
	.4byte _0801DB10 @ case 0
	.4byte _0801DB26 @ case 1
	.4byte _0801DB6C @ case 2
	.4byte _0801DB3A @ case 3
	.4byte _0801DBD0 @ case 4
_0801DB10:
	ldr r0, [r5, #0x14]
	cmp r0, #0xff
	bgt _0801DB1C
	adds r0, #8
	str r0, [r5, #0x14]
	b _0801DBF2
_0801DB1C:
	movs r0, #0x80
	lsls r0, r0, #1
	str r0, [r5, #0x14]
	movs r0, #3
	b _0801DBF0
_0801DB26:
	ldr r0, [r5, #0x14]
	cmp r0, #8
	ble _0801DB32
	subs r0, #8
	str r0, [r5, #0x14]
	b _0801DBF2
_0801DB32:
	movs r0, #8
	str r0, [r5, #0x14]
	movs r0, #2
	b _0801DBF0
_0801DB3A:
	ldr r1, [r5, #0x30]
	adds r1, #1
	movs r2, #0xff
	ands r1, r2
	str r1, [r5, #0x30]
	ldr r3, _0801DB68 @ =gStaticData_0816A820
	adds r0, r1, #0
	ands r0, r2
	lsls r0, r0, #1
	adds r0, r0, r3
	movs r4, #0
	ldrsh r0, [r0, r4]
	asrs r0, r0, #6
	str r0, [r5, #0x28]
	lsls r1, r1, #1
	ands r1, r2
	lsls r1, r1, #1
	adds r1, r1, r3
	movs r2, #0
	ldrsh r0, [r1, r2]
	asrs r0, r0, #6
	str r0, [r5, #0x2c]
	b _0801DBF2
	.align 2, 0
_0801DB68: .4byte gStaticData_0816A820
_0801DB6C:
	ldr r0, [r5, #0x10]
	cmp r0, #0xb
	beq _0801DBF2
	ldr r0, _0801DBC0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x53
	bl PlaySfx
	ldr r4, _0801DBC4 @ =gStaticData_0816C5A0
	ldr r0, [r5, #0x10]
	lsls r0, r0, #3
	adds r0, r0, r4
	ldr r0, [r0]
	mov r1, sp
	bl LoadTaggedAsset
	ldr r1, _0801DBC8 @ =0x040000D4
	mov r3, sp
	str r3, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	str r0, [r1, #4]
	ldr r0, _0801DBCC @ =0x80000020
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r0, [r5, #0x10]
	lsls r0, r0, #3
	adds r4, #4
	adds r0, r0, r4
	ldr r0, [r0]
	ldr r1, [r5, #0x18]
	lsls r1, r1, #0xe
	movs r4, #0xc0
	lsls r4, r4, #0x13
	adds r1, r1, r4
	bl LoadTaggedAsset
	movs r0, #0
	b _0801DBF0
	.align 2, 0
_0801DBC0: .4byte gUnknown_030012BC
_0801DBC4: .4byte gStaticData_0816C5A0
_0801DBC8: .4byte 0x040000D4
_0801DBCC: .4byte 0x80000020
_0801DBD0:
	ldr r0, [r5, #0x14]
	cmp r0, #8
	ble _0801DBEA
	subs r0, #8
	str r0, [r5, #0x14]
	adds r1, r5, #0
	adds r1, #0x48
	ldrh r2, [r1]
	movs r3, #0x80
	lsls r3, r3, #1
	adds r0, r2, r3
	strh r0, [r1]
	b _0801DBF2
_0801DBEA:
	movs r0, #8
	str r0, [r5, #0x14]
	movs r0, #5
_0801DBF0:
	str r0, [r5, #0xc]
_0801DBF2:
	adds r1, r5, #0
	adds r1, #0x5c
	adds r0, r5, #0
	bl sub_801DE04
	adds r1, r5, #0
	adds r1, #0x68
	adds r0, r5, #0
	bl sub_801DE04
	adds r1, r5, #0
	adds r1, #0x74
	adds r0, r5, #0
	bl sub_801DE04
	adds r1, r5, #0
	adds r1, #0x80
	adds r0, r5, #0
	bl sub_801DE04
	movs r3, #0x80
	lsls r3, r3, #2
	add sp, r3
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801DC28
sub_801DC28: @ 0x0801DC28
	push {r4, lr}
	adds r4, r0, #0
	bl sub_801DD18
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801DC5E
	adds r1, r4, #0
	adds r1, #0x5c
	adds r0, r4, #0
	bl sub_801DD90
	adds r1, r4, #0
	adds r1, #0x68
	adds r0, r4, #0
	bl sub_801DD90
	adds r1, r4, #0
	adds r1, #0x74
	adds r0, r4, #0
	bl sub_801DD90
	adds r1, r4, #0
	adds r1, #0x80
	adds r0, r4, #0
	bl sub_801DD90
_0801DC5E:
	ldr r0, [r4, #0xc]
	cmp r0, #0
	blt _0801DCA8
	cmp r0, #3
	bgt _0801DC96
	ldrh r0, [r4, #0x28]
	ldrh r2, [r4, #0x20]
	adds r1, r0, r2
	adds r0, r4, #0
	adds r0, #0x40
	strh r1, [r0]
	ldrh r1, [r4, #0x2c]
	ldrh r2, [r4, #0x24]
	adds r0, r1, r2
	adds r1, r4, #0
	adds r1, #0x42
	strh r0, [r1]
	ldr r1, [r4, #0x14]
	movs r0, #0x80
	lsls r0, r0, #9
	bl sub_803ADB4
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r1, r4, #0
	adds r1, #0x44
	strh r0, [r1]
	b _0801DCA4
_0801DC96:
	cmp r0, #5
	bgt _0801DCA8
	ldr r0, [r4, #0x14]
	adds r1, r4, #0
	adds r1, #0x44
	strh r0, [r1]
	ldr r0, [r4, #0x14]
_0801DCA4:
	adds r1, #2
	strh r0, [r1]
_0801DCA8:
	adds r0, r4, #0
	adds r0, #0x38
	adds r1, r4, #0
	adds r1, #0x4c
	movs r2, #1
	bl sub_803A944
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_801DCBC
sub_801DCBC: @ 0x0801DCBC
	adds r2, r0, #0
	ldr r1, _0801DCF4 @ =0x04000020
	adds r0, #0x4c
	ldrh r0, [r0]
	strh r0, [r1]
	adds r1, #2
	adds r0, r2, #0
	adds r0, #0x4e
	ldrh r0, [r0]
	strh r0, [r1]
	adds r1, #2
	adds r0, r2, #0
	adds r0, #0x50
	ldrh r0, [r0]
	strh r0, [r1]
	adds r1, #2
	adds r0, r2, #0
	adds r0, #0x52
	ldrh r0, [r0]
	strh r0, [r1]
	adds r1, #2
	ldr r0, [r2, #0x54]
	str r0, [r1]
	adds r1, #4
	ldr r0, [r2, #0x58]
	str r0, [r1]
	bx lr
	.align 2, 0
_0801DCF4: .4byte 0x04000020

	thumb_func_start sub_801DCF8
sub_801DCF8: @ 0x0801DCF8
	movs r1, #0
	ldr r0, [r0, #0xc]
	cmp r0, #4
	bne _0801DD02
	movs r1, #1
_0801DD02:
	adds r0, r1, #0
	bx lr
	.align 2, 0

	thumb_func_start sub_801DD08
sub_801DD08: @ 0x0801DD08
	movs r1, #0
	ldr r0, [r0, #0xc]
	cmp r0, #5
	bne _0801DD12
	movs r1, #1
_0801DD12:
	adds r0, r1, #0
	bx lr
	.align 2, 0

	thumb_func_start sub_801DD18
sub_801DD18: @ 0x0801DD18
	movs r1, #0
	ldr r0, [r0, #0xc]
	cmp r0, #3
	bne _0801DD22
	movs r1, #1
_0801DD22:
	adds r0, r1, #0
	bx lr
	.align 2, 0

	thumb_func_start sub_801DD28
sub_801DD28: @ 0x0801DD28
	movs r1, #0
	ldr r0, [r0, #0xc]
	cmp r0, #2
	bne _0801DD32
	movs r1, #1
_0801DD32:
	adds r0, r1, #0
	bx lr
	.align 2, 0

	thumb_func_start sub_801DD38
sub_801DD38: @ 0x0801DD38
	movs r1, #0
	ldr r0, [r0, #0xc]
	cmp r0, #1
	bne _0801DD42
	movs r1, #1
_0801DD42:
	adds r0, r1, #0
	bx lr
	.align 2, 0

	thumb_func_start sub_801DD48
sub_801DD48: @ 0x0801DD48
	adds r2, r0, #0
	adds r2, #0x34
	movs r1, #4
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	strb r1, [r2]
	movs r1, #4
	str r1, [r0, #0xc]
	bx lr

	thumb_func_start sub_801DD5C
sub_801DD5C: @ 0x0801DD5C
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, _0801DD7C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x54
	bl PlaySfx
	movs r0, #1
	str r0, [r4, #0xc]
	movs r0, #0xb
	str r0, [r4, #0x10]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0801DD7C: .4byte gUnknown_030012BC

	thumb_func_start sub_801DD80
sub_801DD80: @ 0x0801DD80
	ldr r2, [r0, #0xc]
	cmp r2, #2
	bgt _0801DD8C
	cmp r2, #1
	blt _0801DD8C
	str r1, [r0, #0x10]
_0801DD8C:
	bx lr
	.align 2, 0

	thumb_func_start sub_801DD90
sub_801DD90: @ 0x0801DD90
	push {lr}
	adds r3, r0, #0
	adds r2, r1, #0
	ldr r0, [r2, #4]
	cmp r0, #0
	ble _0801DDB0
	ldr r0, [r2]
	movs r1, #4
	ands r0, r1
	cmp r0, #0
	beq _0801DDB0
	ldr r0, [r2, #8]
	ldr r1, [r3, #0x28]
	ldr r2, [r3, #0x2c]
	bl sub_8008890
_0801DDB0:
	pop {r0}
	bx r0

	thumb_func_start sub_801DDB4
sub_801DDB4: @ 0x0801DDB4
	push {r4, r5, r6, lr}
	adds r5, r1, #0
	movs r0, #8
	bl sub_8000E1C
	lsls r0, r0, #0x10
	ldr r4, [r5, #8]
	lsrs r3, r0, #0x10
	ldr r0, [r4, #0x20]
	adds r2, r4, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r6, [r2]
	lsls r0, r6, #3
	subs r0, r0, r6
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _0801DDDE
	subs r3, r0, #1
_0801DDDE:
	str r3, [r4, #0x30]
	movs r0, #0x3c
	bl sub_8000E1C
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	str r0, [r5]
	cmp r0, #0
	beq _0801DDFE
	lsrs r0, r0, #1
	adds r0, #1
	bl sub_8000E1C
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	str r0, [r5, #4]
_0801DDFE:
	pop {r4, r5, r6}
	pop {r0}
	bx r0

	thumb_func_start sub_801DE04
sub_801DE04: @ 0x0801DE04
	push {lr}
	adds r2, r0, #0
	ldr r0, [r1]
	cmp r0, #0
	beq _0801DE1A
	subs r0, #1
	str r0, [r1]
	ldr r0, [r1, #4]
	subs r0, #1
	str r0, [r1, #4]
	b _0801DE20
_0801DE1A:
	adds r0, r2, #0
	bl sub_801DDB4
_0801DE20:
	pop {r0}
	bx r0

	thumb_func_start sub_801DE24
sub_801DE24: @ 0x0801DE24
	ldrh r0, [r0, #0x34]
	bx lr

	thumb_func_start sub_801DE28
sub_801DE28: @ 0x0801DE28
	ldrb r0, [r0, #4]
	bx lr

	thumb_func_start sub_801DE2C
sub_801DE2C: @ 0x0801DE2C
	ldr r0, [r0]
	bx lr

	thumb_func_start sub_801DE30
sub_801DE30: @ 0x0801DE30
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	movs r0, #0xff
	ands r1, r0
	cmp r1, #0x9f
	bgt _0801DE40
	rsbs r5, r1, #0
	b _0801DE46
_0801DE40:
	movs r0, #0x80
	lsls r0, r0, #1
	subs r5, r0, r1
_0801DE46:
	ldrb r0, [r4, #4]
	cmp r0, #0
	beq _0801DE58
	ldr r0, [r4, #8]
	adds r2, r5, #2
	movs r1, #0
	bl sub_8008890
	b _0801DE62
_0801DE58:
	ldr r0, [r4, #8]
	movs r1, #0
	adds r2, r5, #0
	bl sub_8008890
_0801DE62:
	ldrb r0, [r4, #4]
	cmp r0, #0
	beq _0801DE6E
	ldr r1, [r4, #0xc]
	movs r4, #1
	b _0801DE72
_0801DE6E:
	ldr r1, [r4, #0xc]
	movs r4, #0
_0801DE72:
	ldr r0, [r1, #0x20]
	adds r3, r1, #0
	adds r3, #0x2d
	ldr r2, [r0]
	ldrb r6, [r3]
	lsls r0, r6, #3
	subs r0, r0, r6
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r2, [r0, #0x16]
	adds r0, r1, #0
	cmp r4, r2
	blt _0801DE8E
	subs r4, r2, #1
_0801DE8E:
	str r4, [r0, #0x30]
	movs r1, #0
	adds r2, r5, #0
	bl sub_8008890
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801DEA0
sub_801DEA0: @ 0x0801DEA0
	strb r1, [r0, #4]
	bx lr

	thumb_func_start sub_801DEA4
sub_801DEA4: @ 0x0801DEA4
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r3, r1, #0
	cmp r2, #4
	bgt _0801DED8
	lsls r0, r3, #2
	adds r0, r0, r3
	adds r0, r0, r2
	str r0, [r4]
	ldr r4, [r4, #8]
	adds r2, r0, #0
	ldr r0, [r4, #0x20]
	adds r3, r4, #0
	adds r3, #0x2d
	ldr r1, [r0]
	ldrb r5, [r3]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r2, r0
	blt _0801DED4
	subs r2, r0, #1
_0801DED4:
	str r2, [r4, #0x30]
	b _0801DF02
_0801DED8:
	adds r0, r3, #0
	adds r0, #0x14
	str r0, [r4]
	ldr r1, _0801DF08 @ =gStaticData_0816C624
	lsls r0, r3, #2
	adds r0, r0, r1
	ldr r4, [r4, #8]
	ldr r0, [r0]
	adds r2, r4, #0
	adds r2, #0x2d
	strb r0, [r2]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
_0801DF02:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0801DF08: .4byte gStaticData_0816C624

	thumb_func_start sub_801DF0C
sub_801DF0C: @ 0x0801DF0C
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	ldr r0, _0801DF6C @ =gStaticData_0816C610
	lsls r1, r1, #2
	adds r1, r1, r0
	ldr r4, [r5, #0xc]
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
	ldr r0, [r5, #0xc]
	bl sub_800815C
	ldr r2, [r5, #0xc]
	adds r2, #0x29
	movs r6, #0xf
	ands r0, r6
	movs r4, #0x10
	rsbs r4, r4, #0
	adds r1, r4, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldr r0, [r5, #8]
	bl sub_800815C
	ldr r1, [r5, #8]
	adds r1, #0x29
	ands r0, r6
	ldrb r2, [r1]
	ands r4, r2
	orrs r4, r0
	strb r4, [r1]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801DF6C: .4byte gStaticData_0816C610

	thumb_func_start sub_801DF70
sub_801DF70: @ 0x0801DF70
	push {r4, lr}
	ldr r4, [r0, #8]
	ldr r3, [r1]
	ldr r2, [r1, #4]
	subs r2, #3
	lsls r3, r3, #8
	str r3, [r4]
	lsls r2, r2, #8
	str r2, [r4, #4]
	ldr r0, [r0, #0xc]
	ldr r3, [r1]
	ldr r2, [r1, #4]
	adds r1, r3, #0
	bl sub_800737C
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start nullsub_20
nullsub_20: @ 0x0801DF94
	bx lr
	.align 2, 0

	thumb_func_start sub_801DF98
sub_801DF98: @ 0x0801DF98
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, _0801DFE8 @ =gStaticData_087E4BAC
	str r0, [r4, #0x10]
	ldr r2, [r4, #8]
	cmp r2, #0
	beq _0801DFBA
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0801DFBA:
	ldr r2, [r4, #0xc]
	cmp r2, #0
	beq _0801DFD2
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0801DFD2:
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _0801DFE0
	adds r0, r4, #0
	bl sub_8026ED0
_0801DFE0:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0801DFE8: .4byte gStaticData_087E4BAC

	thumb_func_start sub_801DFEC
sub_801DFEC: @ 0x0801DFEC
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r0, _0801E044 @ =gStaticData_087E4BAC
	str r0, [r4, #0x10]
	movs r0, #0
	strb r0, [r4, #4]
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	str r0, [r4, #0xc]
	ldr r5, _0801E048 @ =gUnknown_030012D0
	ldr r1, [r5]
	ldr r1, [r1]
	ldr r1, [r1]
	movs r2, #0x93
	lsls r2, r2, #2
	adds r1, r1, r2
	str r1, [r0, #0x20]
	movs r1, #1
	bl sub_80088D8
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	str r0, [r4, #8]
	ldr r1, [r5]
	ldr r1, [r1]
	ldr r1, [r1]
	movs r2, #0x99
	lsls r2, r2, #2
	adds r1, r1, r2
	str r1, [r0, #0x20]
	movs r1, #1
	bl sub_80088D8
	adds r0, r4, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0801E044: .4byte gStaticData_087E4BAC
_0801E048: .4byte gUnknown_030012D0

	thumb_func_start sub_801E04C
sub_801E04C: @ 0x0801E04C
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	adds r4, r0, #0
	str r4, [r6, #0x28]
	ldr r0, _0801E174 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x90
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x20]
	movs r5, #0
	adds r0, r4, #0
	adds r0, #0x2d
	strb r5, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r6, #0x28]
	bl sub_800815C
	ldr r2, [r6, #0x28]
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldr r0, _0801E178 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r2, [r6, #0x28]
	ldr r1, [r2, #0x20]
	adds r2, #0x2d
	ldr r3, [r1]
	ldrb r4, [r2]
	lsls r1, r4, #3
	subs r1, r1, r4
	lsls r1, r1, #2
	adds r1, r1, r3
	ldrb r1, [r1, #0x14]
	bl sub_8006D84
	adds r0, r6, #0
	movs r1, #0x78
	movs r2, #0x35
	bl sub_801E4F4
	ldrb r1, [r6, #4]
	subs r1, #0x20
	adds r0, r6, #0
	adds r0, #0x34
	strb r1, [r0]
	movs r7, #0x35
	movs r0, #4
	rsbs r0, r0, #0
	ldrb r1, [r7, r6]
	ands r0, r1
	movs r1, #1
	orrs r0, r1
	movs r2, #0xd
	rsbs r2, r2, #0
	ands r0, r2
	movs r4, #0x11
	rsbs r4, r4, #0
	ands r0, r4
	movs r3, #0x21
	rsbs r3, r3, #0
	ands r0, r3
	movs r1, #0x3f
	ands r0, r1
	strb r0, [r7, r6]
	ldr r1, [r6]
	subs r1, #0x20
	ldr r7, _0801E17C @ =0x000001FF
	adds r0, r7, #0
	ands r1, r0
	ldr r0, _0801E180 @ =0xFFFFFE00
	ldrh r7, [r6, #0x36]
	ands r0, r7
	orrs r0, r1
	strh r0, [r6, #0x36]
	movs r0, #0x37
	adds r0, r0, r6
	mov ip, r0
	movs r0, #0xf
	rsbs r0, r0, #0
	mov r1, ip
	ldrb r1, [r1]
	ands r0, r1
	ands r0, r4
	ands r0, r3
	movs r1, #0xc0
	orrs r0, r1
	mov r3, ip
	strb r0, [r3]
	ldr r0, _0801E184 @ =0xFFFFFC00
	ldrh r4, [r6, #0x38]
	ands r0, r4
	movs r7, #0xf0
	lsls r7, r7, #2
	adds r1, r7, #0
	orrs r0, r1
	strh r0, [r6, #0x38]
	adds r3, r6, #0
	adds r3, #0x39
	ldrb r0, [r3]
	ands r2, r0
	strb r2, [r3]
	ldr r0, [r6, #0x28]
	adds r0, #0x29
	ldrb r0, [r0]
	lsls r0, r0, #0x1c
	lsrs r0, r0, #0x18
	movs r1, #0xf
	ands r2, r1
	orrs r2, r0
	strb r2, [r3]
	ldr r0, _0801E188 @ =gStaticData_086377C0
	ldr r1, _0801E18C @ =0x06017800
	bl LoadTaggedAsset
	adds r0, r6, #0
	adds r0, #0x48
	strh r5, [r0]
	movs r0, #8
	str r0, [r6, #0x40]
	movs r0, #4
	str r0, [r6, #0x30]
	adds r0, r6, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0801E174: .4byte gUnknown_030012D0
_0801E178: .4byte gUnknown_030012B8
_0801E17C: .4byte 0x000001FF
_0801E180: .4byte 0xFFFFFE00
_0801E184: .4byte 0xFFFFFC00
_0801E188: .4byte gStaticData_086377C0
_0801E18C: .4byte 0x06017800

	thumb_func_start sub_801E190
sub_801E190: @ 0x0801E190
	push {r4, r5, lr}
	adds r4, r0, #0
	bl sub_801E43C
	ldr r0, [r4, #0x30]
	cmp r0, #5
	bls _0801E1A0
	b _0801E2B6
_0801E1A0:
	lsls r0, r0, #2
	ldr r1, _0801E1AC @ =_0801E1B0
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801E1AC: .4byte _0801E1B0
_0801E1B0: @ jump table
	.4byte _0801E1C8 @ case 0
	.4byte _0801E1F0 @ case 1
	.4byte _0801E210 @ case 2
	.4byte _0801E24C @ case 3
	.4byte _0801E288 @ case 4
	.4byte _0801E2A6 @ case 5
_0801E1C8:
	ldr r0, [r4, #0x28]
	bl sub_8008044
	ldr r0, [r4, #0x2c]
	cmp r0, #0
	beq _0801E1DA
	subs r0, #1
	str r0, [r4, #0x2c]
	b _0801E2B6
_0801E1DA:
	ldr r5, [r4, #0x28]
	ldr r0, [r5, #0x30]
	cmp r0, #0
	bne _0801E2B6
	movs r0, #1
	str r0, [r4, #0x30]
	ldr r0, _0801E1EC @ =gStaticData_0816C634
	ldr r0, [r0, #4]
	b _0801E22A
	.align 2, 0
_0801E1EC: .4byte gStaticData_0816C634
_0801E1F0:
	ldr r0, [r4, #0x28]
	bl sub_8008044
	ldr r5, [r4, #0x28]
	adds r0, r5, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801E2B6
	movs r0, #2
	str r0, [r4, #0x30]
	ldr r0, _0801E20C @ =gStaticData_0816C634
	ldr r0, [r0, #8]
	b _0801E22A
	.align 2, 0
_0801E20C: .4byte gStaticData_0816C634
_0801E210:
	ldr r0, [r4, #0x28]
	bl sub_8008044
	ldr r5, [r4, #0x28]
	adds r0, r5, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801E2B6
	movs r0, #3
	str r0, [r4, #0x30]
	ldr r0, _0801E248 @ =gStaticData_0816C634
	ldr r0, [r0, #0xc]
_0801E22A:
	adds r1, r5, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r5, #0
	bl sub_80087C0
	adds r0, r5, #0
	bl sub_80087B4
	adds r0, r5, #0
	movs r1, #0
	bl sub_800872C
	b _0801E2B6
	.align 2, 0
_0801E248: .4byte gStaticData_0816C634
_0801E24C:
	ldr r0, [r4, #0x28]
	bl sub_8008044
	ldr r5, [r4, #0x28]
	adds r0, r5, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801E2B6
	movs r0, #0
	str r0, [r4, #0x30]
	ldr r0, _0801E284 @ =gStaticData_0816C634
	ldr r0, [r0]
	adds r1, r5, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r5, #0
	bl sub_80087C0
	adds r0, r5, #0
	bl sub_80087B4
	adds r0, r5, #0
	movs r1, #0
	bl sub_800872C
	b _0801E29E
	.align 2, 0
_0801E284: .4byte gStaticData_0816C634
_0801E288:
	ldr r1, [r4, #0x40]
	cmp r1, #0xff
	bgt _0801E294
	ldr r0, [r4, #0x3c]
	adds r0, r1, r0
	b _0801E2B4
_0801E294:
	movs r0, #0x80
	lsls r0, r0, #1
	str r0, [r4, #0x40]
	movs r0, #0
	str r0, [r4, #0x30]
_0801E29E:
	adds r0, r4, #0
	bl sub_801E504
	b _0801E2B6
_0801E2A6:
	ldr r1, [r4, #0x40]
	cmp r1, #8
	ble _0801E2B2
	ldr r0, [r4, #0x3c]
	subs r0, r1, r0
	b _0801E2B4
_0801E2B2:
	movs r0, #8
_0801E2B4:
	str r0, [r4, #0x40]
_0801E2B6:
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_801E2BC
sub_801E2BC: @ 0x0801E2BC
	push {r4, r5, r6, r7, lr}
	adds r7, r0, #0
	ldr r0, [r7, #0x28]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	bl sub_800737C
	ldr r0, [r7, #0x30]
	cmp r0, #5
	bgt _0801E394
	cmp r0, #4
	blt _0801E394
	ldr r0, [r7, #0x40]
	cmp r0, #8
	ble _0801E39E
	ldr r1, [r7]
	subs r1, #0x20
	ldr r2, _0801E388 @ =0x000001FF
	adds r0, r2, #0
	ands r1, r0
	ldr r0, _0801E38C @ =0xFFFFFE00
	ldrh r3, [r7, #0x36]
	ands r0, r3
	orrs r0, r1
	strh r0, [r7, #0x36]
	ldrb r0, [r7, #4]
	subs r0, #0x20
	adds r6, r7, #0
	adds r6, #0x34
	strb r0, [r6]
	ldr r5, _0801E390 @ =gUnknown_03001300
	ldr r1, [r5]
	ldr r0, [r1, #8]
	adds r4, r0, #0
	adds r0, #1
	str r0, [r1, #8]
	movs r0, #0x37
	adds r0, r0, r7
	mov ip, r0
	movs r0, #7
	adds r1, r4, #0
	ands r1, r0
	lsls r1, r1, #1
	movs r0, #0xf
	rsbs r0, r0, #0
	mov r2, ip
	ldrb r2, [r2]
	ands r0, r2
	orrs r0, r1
	asrs r1, r4, #3
	movs r3, #1
	ands r1, r3
	lsls r1, r1, #4
	movs r2, #0x11
	rsbs r2, r2, #0
	ands r0, r2
	orrs r0, r1
	asrs r1, r4, #4
	ands r1, r3
	lsls r1, r1, #5
	subs r2, #0x10
	ands r0, r2
	orrs r0, r1
	mov r3, ip
	strb r0, [r3]
	adds r0, r7, #0
	bl sub_801E3A4
	adds r0, r7, #0
	adds r0, #0x4c
	ldrh r1, [r0]
	ldr r0, [r5]
	lsls r2, r4, #2
	lsls r4, r4, #5
	adds r4, r0, r4
	strh r1, [r4, #0x12]
	adds r1, r7, #0
	adds r1, #0x4e
	ldrh r3, [r1]
	adds r1, r2, #1
	lsls r1, r1, #3
	adds r1, r0, r1
	strh r3, [r1, #0x12]
	adds r1, r7, #0
	adds r1, #0x50
	ldrh r3, [r1]
	adds r1, r2, #2
	lsls r1, r1, #3
	adds r1, r0, r1
	strh r3, [r1, #0x12]
	adds r1, r7, #0
	adds r1, #0x52
	ldrh r1, [r1]
	adds r2, #3
	lsls r2, r2, #3
	adds r2, r0, r2
	strh r1, [r2, #0x12]
	adds r1, r6, #0
	bl sub_8006AC8
	b _0801E39E
	.align 2, 0
_0801E388: .4byte 0x000001FF
_0801E38C: .4byte 0xFFFFFE00
_0801E390: .4byte gUnknown_03001300
_0801E394:
	ldr r0, [r7, #0x28]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
_0801E39E:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_801E3A4
sub_801E3A4: @ 0x0801E3A4
	push {r4, lr}
	adds r4, r0, #0
	ldr r1, [r4, #0x40]
	movs r0, #0x80
	lsls r0, r0, #9
	bl sub_803ADB4
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r2, r4, #0
	adds r2, #0x44
	strh r0, [r2]
	adds r1, r4, #0
	adds r1, #0x46
	strh r0, [r1]
	adds r1, #6
	adds r0, r2, #0
	movs r2, #1
	movs r3, #2
	bl sub_803A954
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_801E3D4
sub_801E3D4: @ 0x0801E3D4
	movs r1, #0
	ldr r0, [r0, #0x30]
	cmp r0, #5
	bne _0801E3DE
	movs r1, #1
_0801E3DE:
	adds r0, r1, #0
	bx lr
	.align 2, 0

	thumb_func_start sub_801E3E4
sub_801E3E4: @ 0x0801E3E4
	movs r1, #0
	ldr r0, [r0, #0x30]
	cmp r0, #4
	bne _0801E3EE
	movs r1, #1
_0801E3EE:
	adds r0, r1, #0
	bx lr
	.align 2, 0

	thumb_func_start sub_801E3F4
sub_801E3F4: @ 0x0801E3F4
	push {lr}
	movs r1, #5
	str r1, [r0, #0x30]
	adds r1, #0xfb
	str r1, [r0, #0x40]
	bl sub_801E190
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801E408
sub_801E408: @ 0x0801E408
	push {r4, lr}
	sub sp, #8
	adds r4, r0, #0
	ldr r0, [r4, #0x28]
	ldr r0, [r0]
	asrs r0, r0, #8
	cmp r0, #0x78
	bgt _0801E41C
	movs r0, #0x14
	b _0801E41E
_0801E41C:
	movs r0, #0xdc
_0801E41E:
	str r0, [sp]
	movs r0, #0x88
	str r0, [sp, #4]
	ldr r1, [sp]
	adds r0, r4, #0
	movs r2, #0x88
	bl sub_801E480
	movs r0, #0
	str r0, [r4, #0x2c]
	add sp, #8
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801E43C
sub_801E43C: @ 0x0801E43C
	push {r4, r5, lr}
	adds r4, r0, #0
	movs r5, #1
_0801E442:
	ldr r1, [r4]
	ldr r0, [r4, #8]
	cmp r1, r0
	bne _0801E452
	ldr r1, [r4, #4]
	ldr r0, [r4, #0xc]
	cmp r1, r0
	beq _0801E458
_0801E452:
	adds r0, r4, #0
	bl StepBresenhamLine
_0801E458:
	subs r5, #1
	cmp r5, #0
	bge _0801E442
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_801E464
sub_801E464: @ 0x0801E464
	adds r2, r0, #0
	movs r3, #0
	ldr r1, [r2]
	ldr r0, [r2, #8]
	cmp r1, r0
	bne _0801E47A
	ldr r1, [r2, #4]
	ldr r0, [r2, #0xc]
	cmp r1, r0
	bne _0801E47A
	movs r3, #1
_0801E47A:
	adds r0, r3, #0
	bx lr
	.align 2, 0

	thumb_func_start sub_801E480
sub_801E480: @ 0x0801E480
	push {r4, lr}
	adds r4, r0, #0
	str r1, [r4, #8]
	str r2, [r4, #0xc]
	ldr r0, [r4]
	cmp r0, r1
	bne _0801E494
	ldr r0, [r4, #4]
	cmp r0, r2
	beq _0801E4C4
_0801E494:
	adds r0, r4, #0
	bl InitBresenhamLine
	adds r0, r4, #0
	adds r0, #0x24
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801E4AA
	ldr r0, [r4, #8]
	ldr r1, [r4]
	b _0801E4AE
_0801E4AA:
	ldr r0, [r4, #0xc]
	ldr r1, [r4, #4]
_0801E4AE:
	subs r0, r0, r1
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r1, r0, #1
	cmp r1, #0
	bge _0801E4BC
	rsbs r1, r1, #0
_0801E4BC:
	movs r0, #0xf8
	bl sub_803ADB4
	str r0, [r4, #0x3c]
_0801E4C4:
	ldr r0, [r4, #0x30]
	cmp r0, #0
	bne _0801E4DE
	movs r0, #0x96
	lsls r0, r0, #1
	bl sub_8000E1C
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r1, #0x96
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x2c]
_0801E4DE:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_801E4E4
sub_801E4E4: @ 0x0801E4E4
	push {lr}
	ldr r3, [r1]
	ldr r2, [r1, #4]
	adds r1, r3, #0
	bl sub_801E480
	pop {r0}
	bx r0

	thumb_func_start sub_801E4F4
sub_801E4F4: @ 0x0801E4F4
	push {lr}
	str r1, [r0]
	str r2, [r0, #4]
	ldr r0, [r0, #0x28]
	bl sub_800737C
	pop {r0}
	bx r0

	thumb_func_start sub_801E504
sub_801E504: @ 0x0801E504
	push {r4, lr}
	adds r4, r0, #0
	movs r0, #0x96
	lsls r0, r0, #1
	bl sub_8000E1C
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r1, #0x96
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x2c]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801E524
sub_801E524: @ 0x0801E524
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, _0801E574 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r2, [r4, #0x28]
	ldr r1, [r2, #0x20]
	adds r2, #0x2d
	ldr r3, [r1]
	ldrb r6, [r2]
	lsls r1, r6, #3
	subs r1, r1, r6
	lsls r1, r1, #2
	adds r1, r1, r3
	ldrb r1, [r1, #0x14]
	bl sub_8006D68
	ldr r2, [r4, #0x28]
	cmp r2, #0
	beq _0801E55E
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0801E55E:
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _0801E56C
	adds r0, r4, #0
	bl sub_8026ED0
_0801E56C:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801E574: .4byte gUnknown_030012B8

	thumb_func_start LoadGraphicsPackage
LoadGraphicsPackage: @ 0x0801E578
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	adds r5, r0, #0
	adds r6, r1, #0
	ldr r0, [r6, #8]
	ldr r0, [r0]
	lsrs r0, r0, #8
	cmp r0, #0x20
	bhi _0801E59A
	movs r0, #0x7f
	ldrb r1, [r5, #0xc]
	ands r0, r1
	b _0801E5A0
_0801E59A:
	movs r0, #0x80
	ldrb r2, [r5, #0xc]
	orrs r0, r2
_0801E5A0:
	strb r0, [r5, #0xc]
	ldr r0, [r6, #8]
	ldr r1, [r5, #8]
	lsls r1, r1, #5
	movs r2, #0xa0
	lsls r2, r2, #0x13
	adds r1, r1, r2
	bl LoadTaggedAsset
	ldr r0, [r6, #0xc]
	ldr r1, [r5]
	lsls r1, r1, #0xe
	movs r4, #0xc0
	lsls r4, r4, #0x13
	adds r1, r1, r4
	bl LoadTaggedAsset
	ldr r0, [r6, #0x10]
	ldr r0, [r0]
	lsrs r0, r0, #9
	lsls r0, r0, #1
	bl sub_8026EC0
	mov r8, r0
	ldr r0, [r6, #0x10]
	mov r1, r8
	bl LoadTaggedAsset
	ldr r0, [r5, #8]
	lsls r0, r0, #0xc
	mov ip, r0
	mov r7, r8
	ldr r0, [r5, #4]
	lsls r0, r0, #0xb
	adds r0, r0, r4
	movs r2, #0
	ldr r3, [r6, #4]
	cmp r2, r3
	bge _0801E624
	ldr r4, [r6]
	mov sl, r3
	lsls r6, r4, #1
	mov sb, r6
_0801E5F6:
	adds r1, r0, #0
	adds r1, #0x40
	str r1, [sp]
	adds r5, r2, #1
	cmp r4, #0
	ble _0801E61A
	adds r2, r7, #0
	adds r1, r0, #0
	adds r3, r4, #0
_0801E608:
	mov r0, ip
	ldrh r6, [r2]
	orrs r0, r6
	strh r0, [r1]
	adds r2, #2
	adds r1, #2
	subs r3, #1
	cmp r3, #0
	bne _0801E608
_0801E61A:
	add r7, sb
	ldr r0, [sp]
	adds r2, r5, #0
	cmp r2, sl
	blt _0801E5F6
_0801E624:
	mov r0, r8
	cmp r0, #0
	beq _0801E62E
	bl sub_8026EB4
_0801E62E:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801E640
sub_801E640: @ 0x0801E640
	ldrh r0, [r0, #0xc]
	bx lr

	thumb_func_start sub_801E644
sub_801E644: @ 0x0801E644
	push {r4, r5, r6, r7, lr}
	ldr r5, [sp, #0x14]
	movs r4, #0
	strh r4, [r0, #0xc]
	movs r6, #3
	ands r5, r6
	subs r4, #4
	ldrb r7, [r0, #0xc]
	ands r4, r7
	orrs r4, r5
	str r1, [r0]
	ands r1, r6
	lsls r1, r1, #2
	movs r5, #0xd
	rsbs r5, r5, #0
	ands r4, r5
	orrs r4, r1
	strb r4, [r0, #0xc]
	movs r1, #0x3f
	ldrb r4, [r0, #0xd]
	ands r1, r4
	str r2, [r0, #4]
	movs r4, #0x1f
	ands r2, r4
	movs r4, #0x20
	rsbs r4, r4, #0
	ands r1, r4
	orrs r1, r2
	strb r1, [r0, #0xd]
	str r3, [r0, #8]
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_801E688
sub_801E688: @ 0x0801E688
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	adds r6, r0, #0
	adds r7, r1, #0
	mov r8, r2
	str r7, [r6, #8]
	str r2, [r6, #0xc]
	movs r0, #0x80
	lsls r0, r0, #5
	mov ip, r0
	movs r3, #0
	ldr r1, _0801E748 @ =gStaticData_0816C644
	mov sb, r1
	ldr r2, _0801E74C @ =gStaticData_0816C674
	mov sl, r2
	mov r5, sl
	mov r4, sb
_0801E6B0:
	ldr r2, [r4]
	lsls r0, r2, #1
	cmp r7, r0
	bgt _0801E6CC
	ldr r1, [r5]
	lsls r0, r1, #1
	cmp r8, r0
	bgt _0801E6CC
	adds r0, r2, #0
	muls r0, r1, r0
	cmp r0, ip
	bge _0801E6CC
	mov ip, r0
	str r3, [r6, #0x18]
_0801E6CC:
	adds r5, #4
	adds r4, #4
	adds r3, #1
	cmp r3, #0xb
	ble _0801E6B0
	ldr r4, [r6, #0x18]
	lsls r1, r4, #6
	movs r2, #0x3f
	adds r0, r2, #0
	ldrb r3, [r6, #0x13]
	ands r0, r3
	orrs r0, r1
	strb r0, [r6, #0x13]
	asrs r0, r4, #2
	lsls r0, r0, #6
	adds r5, r2, #0
	ldrb r1, [r6, #0x11]
	ands r5, r1
	orrs r5, r0
	strb r5, [r6, #0x11]
	mov r0, ip
	cmp r0, #0
	bge _0801E6FC
	adds r0, #0x1f
_0801E6FC:
	asrs r0, r0, #5
	movs r2, #0x80
	lsls r2, r2, #3
	adds r1, r2, #0
	subs r1, r1, r0
	ldr r3, _0801E750 @ =0x000003FF
	adds r0, r3, #0
	ands r1, r0
	ldr r0, _0801E754 @ =0xFFFFFC00
	ldrh r2, [r6, #0x14]
	ands r0, r2
	orrs r0, r1
	strh r0, [r6, #0x14]
	lsls r4, r4, #2
	mov r3, sb
	adds r0, r4, r3
	ldr r0, [r0]
	lsls r0, r0, #8
	adds r1, r7, #0
	bl sub_803ADB4
	adds r7, r0, #0
	str r7, [r6, #0x20]
	add r4, sl
	ldr r0, [r4]
	lsls r0, r0, #8
	mov r1, r8
	bl sub_803ADB4
	str r0, [r6, #0x24]
	cmp r7, #0xff
	ble _0801E740
	cmp r0, #0xff
	bgt _0801E758
_0801E740:
	movs r0, #3
	orrs r5, r0
	b _0801E776
	.align 2, 0
_0801E748: .4byte gStaticData_0816C644
_0801E74C: .4byte gStaticData_0816C674
_0801E750: .4byte 0x000003FF
_0801E754: .4byte 0xFFFFFC00
_0801E758:
	movs r1, #0x80
	lsls r1, r1, #1
	cmp r7, r1
	bgt _0801E764
	cmp r0, r1
	ble _0801E770
_0801E764:
	movs r0, #4
	rsbs r0, r0, #0
	ands r5, r0
	movs r0, #1
	orrs r5, r0
	b _0801E776
_0801E770:
	movs r0, #4
	rsbs r0, r0, #0
	ands r5, r0
_0801E776:
	strb r5, [r6, #0x11]
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801E788
sub_801E788: @ 0x0801E788
	push {r4, r5, r6, r7, lr}
	adds r7, r0, #0
	ldrb r1, [r7, #0x11]
	lsls r0, r1, #0x1e
	lsrs r0, r0, #0x1e
	cmp r0, #1
	beq _0801E7C0
	cmp r0, #1
	blo _0801E7A0
	cmp r0, #3
	beq _0801E810
	b _0801E84E
_0801E7A0:
	ldr r1, [r7]
	ldr r2, _0801E7B8 @ =0x000001FF
	adds r0, r2, #0
	ands r1, r0
	ldr r0, _0801E7BC @ =0xFFFFFE00
	ldrh r3, [r7, #0x12]
	ands r0, r3
	orrs r0, r1
	strh r0, [r7, #0x12]
	ldr r0, [r7, #4]
	strb r0, [r7, #0x10]
	b _0801E84E
	.align 2, 0
_0801E7B8: .4byte 0x000001FF
_0801E7BC: .4byte 0xFFFFFE00
_0801E7C0:
	ldr r0, _0801E800 @ =gStaticData_0816C644
	ldr r2, [r7, #0x18]
	lsls r2, r2, #2
	adds r0, r2, r0
	ldr r0, [r0]
	ldr r1, [r7, #8]
	subs r0, r0, r1
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	ldr r1, [r7]
	subs r1, r1, r0
	ldr r3, _0801E804 @ =0x000001FF
	adds r0, r3, #0
	ands r1, r0
	ldr r0, _0801E808 @ =0xFFFFFE00
	ldrh r3, [r7, #0x12]
	ands r0, r3
	orrs r0, r1
	strh r0, [r7, #0x12]
	ldr r0, _0801E80C @ =gStaticData_0816C674
	adds r2, r2, r0
	ldr r0, [r2]
	ldr r1, [r7, #0xc]
	subs r0, r0, r1
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	ldrb r1, [r7, #4]
	subs r0, r1, r0
	strb r0, [r7, #0x10]
	b _0801E84E
	.align 2, 0
_0801E800: .4byte gStaticData_0816C644
_0801E804: .4byte 0x000001FF
_0801E808: .4byte 0xFFFFFE00
_0801E80C: .4byte gStaticData_0816C674
_0801E810:
	ldr r0, [r7, #8]
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	ldr r1, [r7]
	adds r1, r1, r0
	ldr r0, _0801E86C @ =gStaticData_0816C644
	ldr r3, [r7, #0x18]
	lsls r3, r3, #2
	adds r0, r3, r0
	ldr r0, [r0]
	subs r1, r1, r0
	ldr r2, _0801E870 @ =0x000001FF
	adds r0, r2, #0
	ands r1, r0
	ldr r0, _0801E874 @ =0xFFFFFE00
	ldrh r2, [r7, #0x12]
	ands r0, r2
	orrs r0, r1
	strh r0, [r7, #0x12]
	ldr r2, [r7, #4]
	ldr r0, [r7, #0xc]
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	adds r2, r2, r0
	ldr r0, _0801E878 @ =gStaticData_0816C674
	adds r3, r3, r0
	ldrb r3, [r3]
	subs r2, r2, r3
	strb r2, [r7, #0x10]
_0801E84E:
	movs r0, #3
	ldrb r3, [r7, #0x11]
	ands r0, r3
	cmp r0, #0
	bne _0801E880
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r1, [r7, #0x13]
	ands r0, r1
	movs r1, #0x21
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r7, #0x13]
	ldr r6, _0801E87C @ =gUnknown_03001300
	b _0801E8E2
	.align 2, 0
_0801E86C: .4byte gStaticData_0816C644
_0801E870: .4byte 0x000001FF
_0801E874: .4byte 0xFFFFFE00
_0801E878: .4byte gStaticData_0816C674
_0801E87C: .4byte gUnknown_03001300
_0801E880:
	ldr r6, _0801E8F4 @ =gUnknown_03001300
	ldr r4, [r6]
	ldr r0, [r4, #8]
	adds r3, r0, #0
	adds r0, #1
	str r0, [r4, #8]
	movs r0, #7
	adds r1, r3, #0
	ands r1, r0
	lsls r1, r1, #1
	movs r0, #0xf
	rsbs r0, r0, #0
	ldrb r2, [r7, #0x13]
	ands r0, r2
	orrs r0, r1
	asrs r1, r3, #3
	movs r5, #1
	ands r1, r5
	lsls r1, r1, #4
	movs r2, #0x11
	rsbs r2, r2, #0
	ands r0, r2
	orrs r0, r1
	asrs r1, r3, #4
	ands r1, r5
	lsls r1, r1, #5
	subs r2, #0x10
	ands r0, r2
	orrs r0, r1
	strb r0, [r7, #0x13]
	ldrh r0, [r7, #0x20]
	lsls r1, r3, #2
	lsls r3, r3, #5
	adds r3, r4, r3
	movs r2, #0
	strh r0, [r3, #0x12]
	adds r0, r1, #1
	lsls r0, r0, #3
	adds r0, r4, r0
	strh r2, [r0, #0x12]
	adds r0, r1, #2
	lsls r0, r0, #3
	adds r0, r4, r0
	strh r2, [r0, #0x12]
	ldrh r0, [r7, #0x24]
	adds r1, #3
	lsls r1, r1, #3
	adds r4, r4, r1
	strh r0, [r4, #0x12]
_0801E8E2:
	ldr r0, [r6]
	adds r1, r7, #0
	adds r1, #0x10
	bl sub_8006AC8
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801E8F4: .4byte gUnknown_03001300

	thumb_func_start sub_801E8F8
sub_801E8F8: @ 0x0801E8F8
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r0, #0
	adds r3, r1, #0
	str r3, [r4, #0x1c]
	adds r2, r3, #0
	cmp r3, #0
	bge _0801E90A
	adds r2, #0xf
_0801E90A:
	asrs r2, r2, #4
	lsls r2, r2, #4
	movs r1, #0xf
	adds r0, r1, #0
	ldrb r5, [r4, #0x15]
	ands r0, r5
	orrs r0, r2
	strb r0, [r4, #0x15]
	ands r1, r3
	lsls r0, r1, #4
	lsls r2, r1, #8
	orrs r0, r2
	lsls r2, r1, #0xc
	orrs r0, r2
	orrs r1, r0
	ldr r2, _0801E944 @ =0x06017800
	mov r0, sp
	strh r1, [r0]
	ldr r0, _0801E948 @ =0x040000D4
	mov r1, sp
	str r1, [r0]
	str r2, [r0, #4]
	ldr r1, _0801E94C @ =0x81000400
	str r1, [r0, #8]
	ldr r0, [r0, #8]
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0801E944: .4byte 0x06017800
_0801E948: .4byte 0x040000D4
_0801E94C: .4byte 0x81000400

	thumb_func_start sub_801E950
sub_801E950: @ 0x0801E950
	movs r2, #3
	ands r1, r2
	lsls r1, r1, #2
	movs r2, #0xd
	rsbs r2, r2, #0
	ldrb r3, [r0, #0x15]
	ands r2, r3
	orrs r2, r1
	strb r2, [r0, #0x15]
	bx lr

	thumb_func_start sub_801E964
sub_801E964: @ 0x0801E964
	str r1, [r0]
	str r2, [r0, #4]
	bx lr
	.align 2, 0

	thumb_func_start sub_801E96C
sub_801E96C: @ 0x0801E96C
	movs r3, #0xd
	rsbs r3, r3, #0
	adds r1, r3, #0
	ldrb r2, [r0, #0x11]
	ands r1, r2
	movs r2, #0x11
	rsbs r2, r2, #0
	ands r1, r2
	subs r2, #0x10
	ands r1, r2
	movs r2, #0x3f
	ands r1, r2
	strb r1, [r0, #0x11]
	ldrb r1, [r0, #0x15]
	ands r3, r1
	strb r3, [r0, #0x15]
	bx lr
	.align 2, 0

	thumb_func_start sub_801E990
sub_801E990: @ 0x0801E990
	push {r4, r5, r6, r7, lr}
	lsls r1, r1, #0x10
	lsrs r6, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r7, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r4, r3, #0x10
	ldr r5, _0801EA4C @ =gUnknown_030012C0
	ldr r0, [r5]
	bl sub_80232F4
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801E9E2
	ldr r0, _0801EA50 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r2, [r0]
	ldr r0, [r2, #8]
	lsls r1, r4, #1
	adds r1, r1, r0
	ldr r0, [r2, #0xc]
	ldrh r1, [r1]
	adds r0, r1, r0
	ldrb r0, [r0]
	lsrs r2, r0, #1
	movs r0, #1
	ldr r3, _0801EA54 @ =gUnknown_030012D8
	ldr r1, [r3]
	adds r1, #0x28
	ands r2, r0
	lsls r2, r2, #4
	subs r0, #0x12
	ldrb r4, [r1]
	ands r0, r4
	orrs r0, r2
	strb r0, [r1]
	ldr r1, [r3]
	lsls r0, r6, #8
	str r0, [r1]
	lsls r0, r7, #8
	str r0, [r1, #4]
_0801E9E2:
	ldr r1, [r5]
	adds r0, r1, #0
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0801EA46
	adds r0, r1, #0
	bl sub_80232E0
	adds r4, r0, #0
	ldr r0, [r5]
	bl sub_8023130
	cmp r4, r0
	bge _0801EA1E
	ldr r0, [r5]
	bl sub_803AFEC
	cmp r0, #0
	bne _0801EA46
	ldr r0, [r5]
	bl sub_80232B8
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801EA46
	ldr r0, [r5]
	ldr r0, [r0, #0x78]
	cmp r0, #0
	bne _0801EA46
_0801EA1E:
	ldr r0, _0801EA54 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0x1a
	movs r3, #0
	bl sub_803AD88
	ldr r0, _0801EA58 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #1
	bl PlaySfx
_0801EA46:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801EA4C: .4byte gUnknown_030012C0
_0801EA50: .4byte gUnknown_030012B4
_0801EA54: .4byte gUnknown_030012D8
_0801EA58: .4byte gUnknown_030012BC

	thumb_func_start sub_801EA5C
sub_801EA5C: @ 0x0801EA5C
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r0
	lsls r1, r1, #0x10
	lsrs r5, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r7, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r4, r3, #0x10
	ldr r0, _0801EAF8 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023404
	movs r6, #1
	ldrb r0, [r0]
	ands r6, r0
	cmp r6, #0
	bne _0801EAEC
	movs r0, #0x1b
	mov sb, r0
	mov r1, r8
	lsls r0, r1, #0x10
	lsrs r0, r0, #0x10
	adds r1, r5, #0
	adds r2, r7, #0
	adds r3, r4, #0
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _0801EAFC @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r3, #0xde
	lsls r3, r3, #1
	adds r0, r0, r3
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	strb r6, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	mov r0, sb
	strb r0, [r4, #0xa]
	ldr r0, _0801EB00 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
_0801EAEC:
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801EAF8: .4byte gUnknown_030012C0
_0801EAFC: .4byte gUnknown_030012D0
_0801EB00: .4byte gUnknown_030012EC

	thumb_func_start sub_801EB04
sub_801EB04: @ 0x0801EB04
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	adds r7, r0, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	mov sl, r1
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	mov sb, r2
	lsls r3, r3, #0x10
	lsrs r4, r3, #0x10
	ldr r0, _0801EBE0 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023404
	movs r1, #2
	ldrb r0, [r0]
	ands r1, r0
	lsls r1, r1, #0x18
	lsrs r6, r1, #0x18
	cmp r6, #0
	bne _0801EBD0
	movs r5, #1
	movs r0, #0x1d
	mov r8, r0
	lsls r0, r7, #0x10
	lsrs r0, r0, #0x10
	mov r1, sl
	mov r2, sb
	adds r3, r4, #0
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _0801EBE4 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	movs r7, #1
	strb r5, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	mov r0, r8
	strb r0, [r4, #0xa]
	ldr r0, _0801EBE8 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	ldr r0, _0801EBEC @ =gUnknown_030012E4
	ldr r0, [r0]
	mov r1, sb
	str r1, [sp]
	str r6, [sp, #4]
	movs r1, #0x2b
	movs r2, #2
	mov r3, sl
	bl sub_8025BAC
	adds r2, r0, #0
	adds r2, #0x28
	movs r1, #4
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r7
	strb r1, [r2]
	movs r1, #5
	rsbs r1, r1, #0
	ldrb r2, [r0, #0xc]
	ands r1, r2
	strb r1, [r0, #0xc]
_0801EBD0:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801EBE0: .4byte gUnknown_030012C0
_0801EBE4: .4byte gUnknown_030012D0
_0801EBE8: .4byte gUnknown_030012EC
_0801EBEC: .4byte gUnknown_030012E4

	thumb_func_start sub_801EBF0
sub_801EBF0: @ 0x0801EBF0
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	lsls r1, r1, #0x10
	lsrs r6, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r7, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r4, r3, #0x10
	ldr r0, _0801EC90 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023404
	movs r1, #4
	ldrb r0, [r0]
	ands r1, r0
	cmp r1, #0
	bne _0801EC84
	movs r0, #1
	mov r8, r0
	movs r1, #0x1e
	mov sb, r1
	lsls r0, r5, #0x10
	lsrs r0, r0, #0x10
	adds r1, r6, #0
	adds r2, r7, #0
	adds r3, r4, #0
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _0801EC94 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r3, #0xc0
	lsls r3, r3, #1
	adds r0, r0, r3
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r1, r8
	strb r1, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	mov r0, sb
	strb r0, [r4, #0xa]
	ldr r0, _0801EC98 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
_0801EC84:
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801EC90: .4byte gUnknown_030012C0
_0801EC94: .4byte gUnknown_030012D0
_0801EC98: .4byte gUnknown_030012EC

	thumb_func_start sub_801EC9C
sub_801EC9C: @ 0x0801EC9C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov r8, r0
	lsls r1, r1, #0x10
	lsrs r7, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r6, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r5, r3, #0x10
	ldr r4, _0801ED40 @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_80233B4
	cmp r0, #1
	beq _0801ED4C
	ldr r1, [r4]
	movs r0, #1
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	bne _0801ED5C
	movs r0, #3
	mov sb, r0
	movs r1, #0x1f
	mov sl, r1
	mov r3, r8
	lsls r0, r3, #0x10
	lsrs r0, r0, #0x10
	adds r1, r7, #0
	adds r2, r6, #0
	adds r3, r5, #0
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _0801ED44 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r3, sb
	strb r3, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	mov r0, sl
	strb r0, [r4, #0xa]
	ldr r0, _0801ED48 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	b _0801ED5C
	.align 2, 0
_0801ED40: .4byte gUnknown_030012C0
_0801ED44: .4byte gUnknown_030012D0
_0801ED48: .4byte gUnknown_030012EC
_0801ED4C:
	movs r0, #0
	str r0, [sp]
	mov r0, r8
	adds r1, r7, #0
	adds r2, r6, #0
	adds r3, r5, #0
	bl sub_8018D70
_0801ED5C:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_801ED6C
sub_801ED6C: @ 0x0801ED6C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov r8, r0
	lsls r1, r1, #0x10
	lsrs r7, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r6, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r5, r3, #0x10
	ldr r4, _0801EE10 @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_80233B4
	cmp r0, #1
	beq _0801EE1C
	ldr r1, [r4]
	movs r0, #4
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	bne _0801EE2A
	movs r0, #2
	mov sb, r0
	movs r1, #0x20
	mov sl, r1
	mov r3, r8
	lsls r0, r3, #0x10
	lsrs r0, r0, #0x10
	adds r1, r7, #0
	adds r2, r6, #0
	adds r3, r5, #0
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _0801EE14 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r3, sb
	strb r3, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	mov r0, sl
	strb r0, [r4, #0xa]
	ldr r0, _0801EE18 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	b _0801EE2A
	.align 2, 0
_0801EE10: .4byte gUnknown_030012C0
_0801EE14: .4byte gUnknown_030012D0
_0801EE18: .4byte gUnknown_030012EC
_0801EE1C:
	str r0, [sp]
	mov r0, r8
	adds r1, r7, #0
	adds r2, r6, #0
	adds r3, r5, #0
	bl sub_8018D70
_0801EE2A:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801EE3C
sub_801EE3C: @ 0x0801EE3C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov sb, r0
	lsls r1, r1, #0x10
	lsrs r7, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r5, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	ldr r4, _0801EEE0 @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_80233B4
	cmp r0, #1
	beq _0801EEEC
	ldr r1, [r4]
	movs r0, #2
	ldrb r1, [r1, #2]
	ands r0, r1
	lsls r0, r0, #0x18
	lsrs r6, r0, #0x18
	cmp r6, #0
	bne _0801EEFC
	movs r0, #0x22
	mov sl, r0
	mov r1, sb
	lsls r0, r1, #0x10
	lsrs r0, r0, #0x10
	adds r1, r7, #0
	adds r2, r5, #0
	mov r3, r8
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _0801EEE4 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r3, #0xc0
	lsls r3, r3, #1
	adds r0, r0, r3
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	strb r6, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	mov r0, sl
	strb r0, [r4, #0xa]
	ldr r0, _0801EEE8 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	b _0801EEFC
	.align 2, 0
_0801EEE0: .4byte gUnknown_030012C0
_0801EEE4: .4byte gUnknown_030012D0
_0801EEE8: .4byte gUnknown_030012EC
_0801EEEC:
	movs r0, #2
	str r0, [sp]
	mov r0, sb
	adds r1, r7, #0
	adds r2, r5, #0
	mov r3, r8
	bl sub_8018D70
_0801EEFC:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_801EF0C
sub_801EF0C: @ 0x0801EF0C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _0801F040 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x9c
	str r0, [r4, #0x20]
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r7, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #0xd
	str r0, [r6, #0x6c]
	str r6, [r4, #0x44]
	ldr r1, [r6, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #1
	movs r7, #0
	mov sl, r7
	movs r5, #1
	strb r0, [r4, #0xa]
	movs r0, #0x7f
	ldrb r1, [r4, #0xc]
	ands r0, r1
	strb r0, [r4, #0xc]
	ldr r2, _0801F044 @ =gUnknown_030012B4
	mov sb, r2
	ldr r0, [r2]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r3, r8
	lsls r3, r3, #1
	mov r8, r3
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r7, [r2]
	lsrs r0, r7, #1
	eors r0, r5
	ands r0, r5
	adds r3, r4, #0
	adds r3, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r5
	ands r0, r5
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0801F048 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	ldr r1, _0801F04C @ =gStaticData_0816B98C
	adds r0, r6, #0
	adds r0, #0x84
	str r1, [r0]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r5, [r1, #0xc]
	mov r2, r8
	ldrh r2, [r2]
	adds r5, r2, r5
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
	adds r0, r6, #0
	movs r1, #2
	bl sub_800C6A8
	ldr r1, [r5, #4]
	adds r0, r6, #0
	bl sub_800C898
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801F040: .4byte gUnknown_030012D0
_0801F044: .4byte gUnknown_030012B4
_0801F048: .4byte gUnknown_030012F0
_0801F04C: .4byte gStaticData_0816B98C

	thumb_func_start sub_801F050
sub_801F050: @ 0x0801F050
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	adds r5, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r5, #0
	bl sub_8009ED0
	mov r8, r0
	ldr r0, _0801F15C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x84
	mov r1, r8
	str r0, [r1, #0x20]
	mov r0, r8
	bl sub_800815C
	mov r2, r8
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r4, r0, #0
	ldr r1, [r4, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x1c]
	mov r1, r8
	bl sub_803AD80
	movs r0, #0xb
	str r0, [r4, #0x6c]
	mov r3, r8
	str r4, [r3, #0x44]
	ldr r1, [r4, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x1c]
	mov r1, r8
	bl sub_803AD80
	movs r0, #1
	movs r3, #0
	mov sb, r3
	movs r6, #1
	mov r1, r8
	strb r0, [r1, #0xa]
	movs r0, #0x7f
	ldrb r2, [r1, #0xc]
	ands r0, r2
	strb r0, [r1, #0xc]
	ldr r0, _0801F160 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r5, r5, #1
	adds r5, r5, r0
	ldr r2, [r1, #0xc]
	ldrh r5, [r5]
	adds r2, r5, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r6
	ands r0, r6
	mov r3, r8
	adds r3, #0x28
	ands r0, r6
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r5, [r3]
	ands r1, r5
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r6
	ands r0, r6
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0801F164 @ =gUnknown_030012F0
	ldr r0, [r0]
	mov r1, r8
	bl sub_8008E94
	ldr r0, _0801F168 @ =gStaticData_0816B98C
	adds r5, r4, #0
	adds r5, #0x84
	str r0, [r5]
	adds r0, r4, #0
	movs r1, #0x11
	bl sub_800C6A8
	ldr r0, _0801F16C @ =gStaticData_0816BA2C
	str r0, [r5]
	movs r0, #0x64
	movs r1, #0x32
	mov r2, sb
	str r2, [r4, #0x20]
	str r0, [r4, #0x28]
	str r2, [r4, #0x24]
	str r1, [r4, #0x2c]
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801F15C: .4byte gUnknown_030012D0
_0801F160: .4byte gUnknown_030012B4
_0801F164: .4byte gUnknown_030012F0
_0801F168: .4byte gStaticData_0816B98C
_0801F16C: .4byte gStaticData_0816BA2C

	thumb_func_start sub_801F170
sub_801F170: @ 0x0801F170
	push {r4, r5, r6, lr}
	mov r6, sl
	mov r5, sb
	mov r4, r8
	push {r4, r5, r6}
	adds r5, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r5, #0
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _0801F2A8 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x78
	str r0, [r4, #0x20]
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #0xa
	str r0, [r6, #0x6c]
	str r6, [r4, #0x44]
	ldr r1, [r6, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #1
	mov sb, r0
	movs r1, #0
	mov sl, r1
	movs r2, #1
	mov r8, r2
	mov r3, sb
	strb r3, [r4, #0xa]
	movs r0, #0x7f
	ldrb r1, [r4, #0xc]
	ands r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _0801F2AC @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r5, r5, #1
	adds r5, r5, r0
	ldr r2, [r1, #0xc]
	ldrh r5, [r5]
	adds r2, r5, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	mov r5, r8
	eors r0, r5
	ands r0, r5
	adds r3, r4, #0
	adds r3, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r5, [r3]
	ands r1, r5
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	mov r2, r8
	ands r0, r2
	ands r0, r2
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0801F2B0 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	ldr r0, _0801F2B4 @ =gStaticData_0816B98C
	adds r5, r6, #0
	adds r5, #0x84
	str r0, [r5]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r3, sb
	strb r3, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	movs r0, #6
	strb r0, [r4, #0xa]
	adds r0, r6, #0
	movs r1, #3
	bl sub_800C6A8
	ldr r0, _0801F2B8 @ =gStaticData_0816BA0C
	str r0, [r5]
	movs r2, #0x14
	rsbs r2, r2, #0
	movs r0, #0x2d
	movs r1, #0x14
	mov r5, sl
	str r5, [r6, #0x20]
	str r0, [r6, #0x28]
	str r2, [r6, #0x24]
	str r1, [r6, #0x2c]
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801F2A8: .4byte gUnknown_030012D0
_0801F2AC: .4byte gUnknown_030012B4
_0801F2B0: .4byte gUnknown_030012F0
_0801F2B4: .4byte gStaticData_0816B98C
_0801F2B8: .4byte gStaticData_0816BA0C

	thumb_func_start sub_801F2BC
sub_801F2BC: @ 0x0801F2BC
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _0801F3CC @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0xa8
	str r0, [r4, #0x20]
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r7, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #0xe
	str r0, [r6, #0x6c]
	str r6, [r4, #0x44]
	ldr r1, [r6, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #1
	movs r5, #1
	strb r0, [r4, #0xa]
	movs r0, #0x7f
	ldrb r7, [r4, #0xc]
	ands r0, r7
	strb r0, [r4, #0xc]
	ldr r0, _0801F3D0 @ =gUnknown_030012B4
	mov sb, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r5
	ands r0, r5
	adds r3, r4, #0
	adds r3, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r5
	ands r0, r5
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0801F3D4 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	ldr r1, _0801F3D8 @ =gStaticData_0816B98C
	adds r0, r6, #0
	adds r0, #0x84
	str r1, [r0]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r4, [r1, #0xc]
	mov r2, r8
	ldrh r2, [r2]
	adds r4, r2, r4
	adds r0, r6, #0
	movs r1, #2
	bl sub_800C6A8
	ldr r1, [r4, #4]
	adds r0, r6, #0
	bl sub_800C898
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801F3CC: .4byte gUnknown_030012D0
_0801F3D0: .4byte gUnknown_030012B4
_0801F3D4: .4byte gUnknown_030012F0
_0801F3D8: .4byte gStaticData_0816B98C

	thumb_func_start sub_801F3DC
sub_801F3DC: @ 0x0801F3DC
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r6, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r6, r6, #0x10
	lsrs r6, r6, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r6, #0
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _0801F514 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x90
	str r0, [r4, #0x20]
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r7, r0, #0
	ldr r1, [r7, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r7, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #0xc
	str r0, [r7, #0x6c]
	str r7, [r4, #0x44]
	ldr r1, [r7, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r7, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #1
	movs r5, #1
	strb r0, [r4, #0xa]
	movs r0, #0x7f
	ldrb r1, [r4, #0xc]
	ands r0, r1
	strb r0, [r4, #0xc]
	ldr r2, _0801F518 @ =gUnknown_030012B4
	mov r8, r2
	ldr r0, [r2]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r6, r6, #1
	mov sb, r6
	add r0, sb
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r5
	ands r0, r5
	adds r3, r4, #0
	adds r3, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r6, [r3]
	ands r1, r6
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r5
	ands r0, r5
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0801F51C @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	ldr r0, _0801F520 @ =gStaticData_0816B98C
	adds r2, r7, #0
	adds r2, #0x84
	str r0, [r2]
	mov r1, r8
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r3, sb
	adds r6, r3, r0
	ldr r0, [r1, #0xc]
	ldrh r6, [r6]
	adds r0, r6, r0
	adds r3, r0, #0
	ldr r0, _0801F524 @ =gStaticData_0816B9EC
	str r0, [r2]
	ldr r0, [r3, #4]
	ldr r1, [r3, #8]
	ldr r2, [r3, #0xc]
	str r0, [r7, #0x30]
	str r1, [r7, #0x34]
	str r2, [r7, #0x38]
	ldr r0, [r3, #4]
	ldr r1, [r3, #8]
	adds r0, r0, r1
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r2, r0, #1
	adds r1, r2, #0
	cmp r2, #0
	bge _0801F4F4
	adds r1, r2, #3
_0801F4F4:
	asrs r1, r1, #2
	ldr r0, [r3, #0xc]
	adds r0, r0, r1
	str r2, [r7, #0x48]
	str r0, [r7, #0x4c]
	adds r0, r7, #0
	movs r1, #0x10
	bl sub_800C6A8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801F514: .4byte gUnknown_030012D0
_0801F518: .4byte gUnknown_030012B4
_0801F51C: .4byte gUnknown_030012F0
_0801F520: .4byte gStaticData_0816B98C
_0801F524: .4byte gStaticData_0816B9EC

	thumb_func_start sub_801F528
sub_801F528: @ 0x0801F528
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r5, r0, #0
	ldr r0, _0801F66C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0xb4
	str r0, [r5, #0x20]
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r7, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #0xf
	str r0, [r6, #0x6c]
	str r6, [r5, #0x44]
	ldr r1, [r6, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #1
	movs r4, #1
	strb r0, [r5, #0xa]
	movs r0, #0x7f
	ldrb r7, [r5, #0xc]
	ands r0, r7
	strb r0, [r5, #0xc]
	ldr r0, _0801F670 @ =gUnknown_030012B4
	mov sl, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r4
	ands r0, r4
	adds r3, r5, #0
	adds r3, #0x28
	ands r0, r4
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r4
	ands r0, r4
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0801F674 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	ldr r0, _0801F678 @ =gStaticData_0816B98C
	movs r1, #0x84
	adds r1, r1, r6
	mov sb, r1
	str r0, [r1]
	mov r2, sl
	ldr r0, [r2]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r4, [r1, #0xc]
	mov r3, r8
	ldrh r3, [r3]
	adds r4, r3, r4
	adds r0, r5, #0
	adds r0, #0x2d
	movs r7, #0
	strb r7, [r0]
	adds r0, r5, #0
	bl sub_80087C0
	adds r0, r5, #0
	bl sub_80087B4
	adds r0, r5, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, _0801F67C @ =gStaticData_0816B9AC
	mov r1, sb
	str r0, [r1]
	ldr r0, [r4, #0xc]
	ldr r1, [r4, #8]
	ldr r2, [r4, #0x10]
	str r0, [r6, #0x30]
	str r1, [r6, #0x34]
	str r2, [r6, #0x38]
	ldr r1, [r4, #4]
	adds r0, r6, #0
	bl sub_800C898
	adds r0, r6, #0
	movs r1, #0xd
	bl sub_800C6A8
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801F66C: .4byte gUnknown_030012D0
_0801F670: .4byte gUnknown_030012B4
_0801F674: .4byte gUnknown_030012F0
_0801F678: .4byte gStaticData_0816B98C
_0801F67C: .4byte gStaticData_0816B9AC

	thumb_func_start sub_801F680
sub_801F680: @ 0x0801F680
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r6, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r6, r6, #0x10
	lsrs r6, r6, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r6, #0
	bl sub_800A604
	adds r4, r0, #0
	ldr r0, _0801F7A4 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0xcc
	str r0, [r4, #0x20]
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	mov r8, r0
	ldr r1, [r0, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	add r0, r8
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #0x11
	mov r3, r8
	str r0, [r3, #0x6c]
	movs r0, #0
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
	mov r0, r8
	str r0, [r4, #0x44]
	ldr r1, [r0, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	add r0, r8
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #1
	movs r5, #1
	strb r0, [r4, #0xa]
	movs r0, #0x7f
	ldrb r3, [r4, #0xc]
	ands r0, r3
	strb r0, [r4, #0xc]
	ldr r0, _0801F7A8 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r6, r6, #1
	adds r6, r6, r0
	ldr r2, [r1, #0xc]
	ldrh r6, [r6]
	adds r2, r6, r2
	ldrb r6, [r2]
	lsrs r0, r6, #1
	eors r0, r5
	ands r0, r5
	adds r3, r4, #0
	adds r3, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r6, [r3]
	ands r1, r6
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r5
	ands r0, r5
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	movs r0, #0x10
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _0801F7AC @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	ldr r1, _0801F7B0 @ =gStaticData_0816B98C
	mov r0, r8
	adds r0, #0x84
	str r1, [r0]
	mov r0, r8
	movs r1, #5
	bl sub_800C6A8
	ldr r0, _0801F7B4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x27
	bl PlaySfx
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801F7A4: .4byte gUnknown_030012D0
_0801F7A8: .4byte gUnknown_030012B4
_0801F7AC: .4byte gUnknown_030012F0
_0801F7B0: .4byte gStaticData_0816B98C
_0801F7B4: .4byte gUnknown_030012BC

	thumb_func_start sub_801F7B8
sub_801F7B8: @ 0x0801F7B8
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r5, r0, #0
	ldr r0, _0801F8CC @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0xc0
	str r0, [r5, #0x20]
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r7, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #0x10
	str r0, [r6, #0x6c]
	str r6, [r5, #0x44]
	ldr r1, [r6, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #1
	movs r4, #1
	strb r0, [r5, #0xa]
	movs r0, #0x7f
	ldrb r7, [r5, #0xc]
	ands r0, r7
	strb r0, [r5, #0xc]
	ldr r0, _0801F8D0 @ =gUnknown_030012B4
	mov sb, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r4
	ands r0, r4
	adds r3, r5, #0
	adds r3, #0x28
	ands r0, r4
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r4
	ands r0, r4
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0801F8D4 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	ldr r1, _0801F8D8 @ =gStaticData_0816B98C
	adds r0, r6, #0
	adds r0, #0x84
	str r1, [r0]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r4, [r1, #0xc]
	mov r2, r8
	ldrh r2, [r2]
	adds r4, r2, r4
	movs r0, #6
	strb r0, [r5, #0xa]
	adds r0, r6, #0
	movs r1, #2
	bl sub_800C6A8
	ldr r1, [r4, #4]
	adds r0, r6, #0
	bl sub_800C898
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801F8CC: .4byte gUnknown_030012D0
_0801F8D0: .4byte gUnknown_030012B4
_0801F8D4: .4byte gUnknown_030012F0
_0801F8D8: .4byte gStaticData_0816B98C

	thumb_func_start sub_801F8DC
sub_801F8DC: @ 0x0801F8DC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r5, r0, #0
	ldr r0, _0801FA28 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x3c
	str r0, [r5, #0x20]
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r7, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r3, #5
	str r3, [r6, #0x6c]
	str r6, [r5, #0x44]
	ldr r1, [r6, #0xc]
	movs r7, #0x18
	ldrsh r0, [r1, r7]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #1
	movs r4, #1
	strb r0, [r5, #0xa]
	movs r0, #0x7f
	ldrb r1, [r5, #0xc]
	ands r0, r1
	strb r0, [r5, #0xc]
	ldr r2, _0801FA2C @ =gUnknown_030012B4
	mov sl, r2
	ldr r0, [r2]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r3, r8
	lsls r3, r3, #1
	mov r8, r3
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r7, [r2]
	lsrs r0, r7, #1
	eors r0, r4
	ands r0, r4
	adds r3, r5, #0
	adds r3, #0x28
	ands r0, r4
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r4
	ands r0, r4
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0801FA30 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	ldr r0, _0801FA34 @ =gStaticData_0816B98C
	movs r1, #0x84
	adds r1, r1, r6
	mov sb, r1
	str r0, [r1]
	mov r2, sl
	ldr r0, [r2]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r4, [r1, #0xc]
	mov r3, r8
	ldrh r3, [r3]
	adds r4, r3, r4
	movs r0, #2
	adds r1, r5, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r5, #0
	bl sub_80087C0
	adds r0, r5, #0
	bl sub_80087B4
	adds r0, r5, #0
	movs r1, #0
	bl sub_800872C
	movs r7, #5
	strb r7, [r5, #0xa]
	ldr r0, _0801FA38 @ =gStaticData_0816B9CC
	mov r1, sb
	str r0, [r1]
	ldr r0, [r4, #4]
	ldr r1, [r4, #8]
	ldr r2, [r4, #0xc]
	str r0, [r6, #0x30]
	str r1, [r6, #0x34]
	str r2, [r6, #0x38]
	ldr r0, [r4, #0x14]
	ldr r1, [r4, #0x18]
	ldr r2, [r4, #0x10]
	str r0, [r6, #0x3c]
	str r1, [r6, #0x40]
	str r2, [r6, #0x44]
	adds r0, r6, #0
	movs r1, #0xe
	bl sub_800C6A8
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801FA28: .4byte gUnknown_030012D0
_0801FA2C: .4byte gUnknown_030012B4
_0801FA30: .4byte gUnknown_030012F0
_0801FA34: .4byte gStaticData_0816B98C
_0801FA38: .4byte gStaticData_0816B9CC

	thumb_func_start sub_801FA3C
sub_801FA3C: @ 0x0801FA3C
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r5, r0, #0
	ldr r0, _0801FB60 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x30
	str r0, [r5, #0x20]
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r7, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #4
	str r0, [r6, #0x6c]
	str r6, [r5, #0x44]
	ldr r1, [r6, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #1
	movs r4, #1
	strb r0, [r5, #0xa]
	movs r0, #0x7f
	ldrb r7, [r5, #0xc]
	ands r0, r7
	strb r0, [r5, #0xc]
	ldr r0, _0801FB64 @ =gUnknown_030012B4
	mov sb, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r4
	ands r0, r4
	adds r3, r5, #0
	adds r3, #0x28
	ands r0, r4
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r4
	ands r0, r4
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0801FB68 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	ldr r0, _0801FB6C @ =gStaticData_0816B98C
	adds r2, r6, #0
	adds r2, #0x84
	str r0, [r2]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r4, [r1, #0xc]
	mov r3, r8
	ldrh r3, [r3]
	adds r4, r3, r4
	movs r0, #6
	strb r0, [r5, #0xa]
	ldr r0, _0801FB70 @ =gStaticData_0816BA4C
	str r0, [r2]
	adds r0, r6, #0
	movs r1, #0xf
	bl sub_800C6A8
	ldr r1, [r4, #4]
	adds r0, r6, #0
	bl sub_800C898
	ldr r1, [r4, #8]
	ldr r2, [r4, #0xc]
	ldr r3, [r4, #0x10]
	adds r0, r6, #0
	bl sub_800C87C
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801FB60: .4byte gUnknown_030012D0
_0801FB64: .4byte gUnknown_030012B4
_0801FB68: .4byte gUnknown_030012F0
_0801FB6C: .4byte gStaticData_0816B98C
_0801FB70: .4byte gStaticData_0816BA4C

	thumb_func_start sub_801FB74
sub_801FB74: @ 0x0801FB74
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r4, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r4, #0
	bl sub_8009ED0
	adds r6, r0, #0
	ldr r0, _0801FCA4 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x24
	str r0, [r6, #0x20]
	adds r0, r6, #0
	bl sub_800815C
	adds r2, r6, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r7, r0, #0
	ldr r1, [r7, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r7, r0
	ldr r2, [r1, #0x1c]
	adds r1, r6, #0
	bl sub_803AD80
	movs r0, #3
	str r0, [r7, #0x6c]
	str r7, [r6, #0x44]
	ldr r1, [r7, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r7, r0
	ldr r2, [r1, #0x1c]
	adds r1, r6, #0
	bl sub_803AD80
	movs r0, #1
	movs r1, #0
	mov sb, r1
	movs r5, #1
	strb r0, [r6, #0xa]
	movs r0, #0x7f
	ldrb r2, [r6, #0xc]
	ands r0, r2
	strb r0, [r6, #0xc]
	ldr r0, _0801FCA8 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r4, r4, #1
	adds r4, r4, r0
	ldr r2, [r1, #0xc]
	ldrh r4, [r4]
	adds r2, r4, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r5
	ands r0, r5
	adds r4, r6, #0
	adds r4, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	mov r8, r1
	ldrb r3, [r4]
	ands r1, r3
	orrs r1, r0
	strb r1, [r4]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r5
	ands r0, r5
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r4]
	ldr r0, _0801FCAC @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r6, #0
	bl sub_8008E94
	ldr r1, _0801FCB0 @ =gStaticData_0816B98C
	adds r0, r7, #0
	adds r0, #0x84
	str r1, [r0]
	adds r0, r6, #0
	adds r0, #0x2d
	mov r1, sb
	strb r1, [r0]
	adds r0, r6, #0
	bl sub_80087C0
	adds r0, r6, #0
	bl sub_80087B4
	adds r0, r6, #0
	movs r1, #0
	bl sub_800872C
	ldrb r2, [r4]
	lsls r0, r2, #0x1b
	movs r1, #0
	cmp r0, #0
	blt _0801FC7E
	movs r1, #1
_0801FC7E:
	adds r0, r5, #0
	ands r0, r1
	lsls r0, r0, #4
	mov r1, r8
	ands r1, r2
	orrs r1, r0
	strb r1, [r4]
	movs r0, #6
	strb r0, [r6, #0xa]
	adds r0, r7, #0
	movs r1, #1
	bl sub_800C6A8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801FCA4: .4byte gUnknown_030012D0
_0801FCA8: .4byte gUnknown_030012B4
_0801FCAC: .4byte gUnknown_030012F0
_0801FCB0: .4byte gStaticData_0816B98C

	thumb_func_start sub_801FCB4
sub_801FCB4: @ 0x0801FCB4
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r6, r0, #0
	ldr r0, _0801FDD8 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x60
	str r0, [r6, #0x20]
	adds r0, r6, #0
	bl sub_800815C
	adds r2, r6, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r7, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r4, r0, #0
	ldr r1, [r4, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x1c]
	adds r1, r6, #0
	bl sub_803AD80
	movs r0, #8
	str r0, [r4, #0x6c]
	str r4, [r6, #0x44]
	ldr r1, [r4, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r2, [r1, #0x1c]
	adds r1, r6, #0
	bl sub_803AD80
	movs r0, #1
	movs r5, #1
	strb r0, [r6, #0xa]
	movs r0, #0x7f
	ldrb r7, [r6, #0xc]
	ands r0, r7
	strb r0, [r6, #0xc]
	ldr r0, _0801FDDC @ =gUnknown_030012B4
	mov sb, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r5
	ands r0, r5
	adds r3, r6, #0
	adds r3, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r5
	ands r0, r5
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0801FDE0 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r6, #0
	bl sub_8008E94
	ldr r0, _0801FDE4 @ =gStaticData_0816B98C
	adds r2, r4, #0
	adds r2, #0x84
	str r0, [r2]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r1, [r1, #0xc]
	mov r3, r8
	ldrh r3, [r3]
	adds r1, r3, r1
	movs r0, #3
	strb r0, [r6, #0xa]
	ldr r0, _0801FDE8 @ =gStaticData_0816BA8C
	str r0, [r2]
	ldr r0, [r1, #8]
	ldr r2, [r1, #0xc]
	ldr r3, [r1, #0x10]
	str r0, [r4, #0x30]
	str r2, [r4, #0x34]
	str r3, [r4, #0x38]
	ldr r1, [r1, #4]
	adds r0, r4, #0
	bl sub_800C898
	adds r0, r4, #0
	movs r1, #0xd
	bl sub_800C6A8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801FDD8: .4byte gUnknown_030012D0
_0801FDDC: .4byte gUnknown_030012B4
_0801FDE0: .4byte gUnknown_030012F0
_0801FDE4: .4byte gStaticData_0816B98C
_0801FDE8: .4byte gStaticData_0816BA8C

	thumb_func_start sub_801FDEC
sub_801FDEC: @ 0x0801FDEC
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r4, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r4, #0
	bl sub_8009ED0
	adds r5, r0, #0
	ldr r0, _0801FEDC @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x54
	str r0, [r5, #0x20]
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	mov r8, r0
	ldr r1, [r0, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	add r0, r8
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #7
	mov r3, r8
	str r0, [r3, #0x6c]
	str r3, [r5, #0x44]
	ldr r1, [r3, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	add r0, r8
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #1
	movs r6, #1
	strb r0, [r5, #0xa]
	movs r0, #0x7f
	ldrb r3, [r5, #0xc]
	ands r0, r3
	strb r0, [r5, #0xc]
	ldr r0, _0801FEE0 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r4, r4, #1
	adds r4, r4, r0
	ldr r2, [r1, #0xc]
	ldrh r4, [r4]
	adds r2, r4, r2
	ldrb r4, [r2]
	lsrs r0, r4, #1
	eors r0, r6
	ands r0, r6
	adds r3, r5, #0
	adds r3, #0x28
	ands r0, r6
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r4, [r3]
	ands r1, r4
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r6
	ands r0, r6
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0801FEE4 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	mov r1, r8
	adds r1, #0x84
	ldr r0, _0801FEE8 @ =gStaticData_0816BA6C
	str r0, [r1]
	mov r0, r8
	movs r1, #7
	bl sub_800C6A8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801FEDC: .4byte gUnknown_030012D0
_0801FEE0: .4byte gUnknown_030012B4
_0801FEE4: .4byte gUnknown_030012F0
_0801FEE8: .4byte gStaticData_0816BA6C

	thumb_func_start sub_801FEEC
sub_801FEEC: @ 0x0801FEEC
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _08020000 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x6c
	str r0, [r4, #0x20]
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r7, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r5, r0, #0
	ldr r1, [r5, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #9
	str r0, [r5, #0x6c]
	str r5, [r4, #0x44]
	ldr r1, [r5, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #1
	movs r6, #1
	strb r0, [r4, #0xa]
	movs r0, #0x7f
	ldrb r7, [r4, #0xc]
	ands r0, r7
	strb r0, [r4, #0xc]
	ldr r0, _08020004 @ =gUnknown_030012B4
	mov sb, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r6
	ands r0, r6
	adds r3, r4, #0
	adds r3, #0x28
	ands r0, r6
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r6
	ands r0, r6
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _08020008 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	ldr r1, _0802000C @ =gStaticData_0816B98C
	adds r0, r5, #0
	adds r0, #0x84
	str r1, [r0]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r4, [r1, #0xc]
	mov r2, r8
	ldrh r2, [r2]
	adds r4, r2, r4
	adds r0, r5, #0
	movs r1, #6
	bl sub_800C6A8
	ldr r0, [r4, #8]
	ldr r1, [r4, #0xc]
	ldr r2, [r4, #4]
	str r0, [r5, #0x3c]
	str r1, [r5, #0x40]
	str r2, [r5, #0x44]
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08020000: .4byte gUnknown_030012D0
_08020004: .4byte gUnknown_030012B4
_08020008: .4byte gUnknown_030012F0
_0802000C: .4byte gStaticData_0816B98C

	thumb_func_start sub_8020010
sub_8020010: @ 0x08020010
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r4, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r4, #0
	bl sub_8009ED0
	adds r5, r0, #0
	ldr r0, _08020128 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x96
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r5, #0x20]
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #0x19
	str r0, [r6, #0x6c]
	str r6, [r5, #0x44]
	ldr r1, [r6, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #1
	movs r7, #1
	strb r0, [r5, #0xa]
	movs r0, #0x7f
	ldrb r1, [r5, #0xc]
	ands r0, r1
	strb r0, [r5, #0xc]
	ldr r0, _0802012C @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r4, r4, #1
	adds r4, r4, r0
	ldr r2, [r1, #0xc]
	ldrh r4, [r4]
	adds r2, r4, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r7
	ands r0, r7
	adds r4, r5, #0
	adds r4, #0x28
	ands r0, r7
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	mov r8, r1
	ldrb r3, [r4]
	ands r1, r3
	orrs r1, r0
	strb r1, [r4]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r7
	ands r0, r7
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r4]
	ldr r0, _08020130 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	ldr r1, _08020134 @ =gStaticData_0816B98C
	adds r0, r6, #0
	adds r0, #0x84
	str r1, [r0]
	ldrb r2, [r4]
	lsls r0, r2, #0x1b
	movs r1, #0
	cmp r0, #0
	blt _080200FC
	movs r1, #1
_080200FC:
	ands r1, r7
	lsls r1, r1, #4
	mov r0, r8
	ands r0, r2
	orrs r0, r1
	strb r0, [r4]
	movs r0, #2
	strb r0, [r5, #0xa]
	subs r0, #0x43
	ldrb r1, [r5, #0xc]
	ands r0, r1
	strb r0, [r5, #0xc]
	adds r0, r6, #0
	movs r1, #1
	bl sub_800C6A8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08020128: .4byte gUnknown_030012D0
_0802012C: .4byte gUnknown_030012B4
_08020130: .4byte gUnknown_030012F0
_08020134: .4byte gStaticData_0816B98C

	thumb_func_start sub_8020138
sub_8020138: @ 0x08020138
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r5, r0, #0
	ldr r0, _08020258 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r7, #0xa2
	lsls r7, r7, #1
	adds r0, r0, r7
	str r0, [r5, #0x20]
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r4, r0, #0
	ldr r1, [r4, #0xc]
	movs r7, #0x18
	ldrsh r0, [r1, r7]
	adds r0, r4, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #0x1b
	str r0, [r4, #0x6c]
	str r4, [r5, #0x44]
	ldr r1, [r4, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #1
	movs r6, #1
	strb r0, [r5, #0xa]
	movs r0, #0x7f
	ldrb r3, [r5, #0xc]
	ands r0, r3
	strb r0, [r5, #0xc]
	ldr r7, _0802025C @ =gUnknown_030012B4
	mov sb, r7
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r6
	ands r0, r6
	adds r3, r5, #0
	adds r3, #0x28
	ands r0, r6
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r6
	ands r0, r6
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _08020260 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	ldr r0, _08020264 @ =gStaticData_0816B98C
	adds r2, r4, #0
	adds r2, #0x84
	str r0, [r2]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r0, [r1, #0xc]
	mov r3, r8
	ldrh r3, [r3]
	adds r0, r3, r0
	ldr r1, _08020268 @ =gStaticData_0816BACC
	str r1, [r2]
	ldr r1, [r0, #8]
	ldr r2, [r0, #4]
	ldr r0, [r0, #0xc]
	str r1, [r4, #0x30]
	str r2, [r4, #0x34]
	str r0, [r4, #0x38]
	adds r0, r4, #0
	movs r1, #4
	bl sub_800C6A8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08020258: .4byte gUnknown_030012D0
_0802025C: .4byte gUnknown_030012B4
_08020260: .4byte gUnknown_030012F0
_08020264: .4byte gStaticData_0816B98C
_08020268: .4byte gStaticData_0816BACC

	thumb_func_start sub_802026C
sub_802026C: @ 0x0802026C
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r5, r0, #0
	ldr r0, _08020394 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r7, #0x90
	lsls r7, r7, #1
	adds r0, r0, r7
	str r0, [r5, #0x20]
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r4, r0, #0
	ldr r1, [r4, #0xc]
	movs r7, #0x18
	ldrsh r0, [r1, r7]
	adds r0, r4, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #0x18
	str r0, [r4, #0x6c]
	str r4, [r5, #0x44]
	ldr r1, [r4, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #1
	movs r6, #1
	strb r0, [r5, #0xa]
	movs r0, #0x7f
	ldrb r3, [r5, #0xc]
	ands r0, r3
	strb r0, [r5, #0xc]
	ldr r7, _08020398 @ =gUnknown_030012B4
	mov sb, r7
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r6
	ands r0, r6
	adds r3, r5, #0
	adds r3, #0x28
	ands r0, r6
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r6
	ands r0, r6
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0802039C @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	ldr r0, _080203A0 @ =gStaticData_0816B98C
	adds r2, r4, #0
	adds r2, #0x84
	str r0, [r2]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r1, [r1, #0xc]
	mov r3, r8
	ldrh r3, [r3]
	adds r1, r3, r1
	ldr r0, _080203A4 @ =gStaticData_0816BAEC
	str r0, [r2]
	ldr r0, [r1, #8]
	ldr r2, [r1, #0xc]
	ldr r3, [r1, #0x10]
	str r0, [r4, #0x30]
	str r2, [r4, #0x34]
	str r3, [r4, #0x38]
	ldr r1, [r1, #4]
	adds r0, r4, #0
	bl sub_800C898
	adds r0, r4, #0
	movs r1, #0xd
	bl sub_800C6A8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08020394: .4byte gUnknown_030012D0
_08020398: .4byte gUnknown_030012B4
_0802039C: .4byte gUnknown_030012F0
_080203A0: .4byte gStaticData_0816B98C
_080203A4: .4byte gStaticData_0816BAEC

	thumb_func_start sub_80203A8
sub_80203A8: @ 0x080203A8
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	ldr r7, _080204D4 @ =0xFFD80000
	adds r2, r2, r7
	lsrs r2, r2, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r5, r0, #0
	ldr r0, _080204D8 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xae
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r5, #0x20]
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r4, r0, #0
	ldr r1, [r4, #0xc]
	movs r7, #0x18
	ldrsh r0, [r1, r7]
	adds r0, r4, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #0x1d
	str r0, [r4, #0x6c]
	str r4, [r5, #0x44]
	ldr r1, [r4, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #1
	movs r6, #1
	strb r0, [r5, #0xa]
	movs r0, #0x7f
	ldrb r3, [r5, #0xc]
	ands r0, r3
	strb r0, [r5, #0xc]
	ldr r7, _080204DC @ =gUnknown_030012B4
	mov sb, r7
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r6
	ands r0, r6
	adds r3, r5, #0
	adds r3, #0x28
	ands r0, r6
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r6
	ands r0, r6
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _080204E0 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	ldr r0, _080204E4 @ =gStaticData_0816B98C
	adds r2, r4, #0
	adds r2, #0x84
	str r0, [r2]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r1, [r1, #0xc]
	mov r3, r8
	ldrh r3, [r3]
	adds r1, r3, r1
	ldr r0, _080204E8 @ =gStaticData_0816BB4C
	str r0, [r2]
	movs r0, #0x78
	movs r2, #0x5a
	ldr r1, [r1, #0xc]
	str r0, [r4, #0x30]
	str r2, [r4, #0x34]
	str r1, [r4, #0x38]
	adds r0, r4, #0
	movs r1, #0x28
	bl sub_800C898
	adds r0, r4, #0
	movs r1, #0x12
	bl sub_800C6A8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080204D4: .4byte 0xFFD80000
_080204D8: .4byte gUnknown_030012D0
_080204DC: .4byte gUnknown_030012B4
_080204E0: .4byte gUnknown_030012F0
_080204E4: .4byte gStaticData_0816B98C
_080204E8: .4byte gStaticData_0816BB4C

	thumb_func_start sub_80204EC
sub_80204EC: @ 0x080204EC
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _08020618 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r7, #0x9c
	lsls r7, r7, #1
	adds r0, r0, r7
	str r0, [r4, #0x20]
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r7, #0x18
	ldrsh r0, [r1, r7]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #0x1a
	str r0, [r6, #0x6c]
	str r6, [r4, #0x44]
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #1
	movs r5, #1
	strb r0, [r4, #0xa]
	movs r0, #0x7f
	ldrb r3, [r4, #0xc]
	ands r0, r3
	strb r0, [r4, #0xc]
	ldr r7, _0802061C @ =gUnknown_030012B4
	mov sb, r7
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r5
	ands r0, r5
	adds r3, r4, #0
	adds r3, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r5
	ands r0, r5
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _08020620 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	ldr r0, _08020624 @ =gStaticData_0816B98C
	adds r2, r6, #0
	adds r2, #0x84
	str r0, [r2]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r1, [r1, #0xc]
	mov r3, r8
	ldrh r3, [r3]
	adds r1, r3, r1
	movs r0, #0xa
	strb r0, [r4, #0xa]
	subs r0, #0x4b
	ldrb r7, [r4, #0xc]
	ands r0, r7
	strb r0, [r4, #0xc]
	ldr r0, _08020628 @ =gStaticData_0816BB2C
	str r0, [r2]
	ldr r0, [r1, #4]
	ldr r2, [r1, #8]
	ldr r1, [r1, #0xc]
	str r0, [r6, #0x30]
	str r2, [r6, #0x34]
	str r1, [r6, #0x38]
	adds r0, r6, #0
	movs r1, #4
	bl sub_800C6A8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08020618: .4byte gUnknown_030012D0
_0802061C: .4byte gUnknown_030012B4
_08020620: .4byte gUnknown_030012F0
_08020624: .4byte gStaticData_0816B98C
_08020628: .4byte gStaticData_0816BB2C

	thumb_func_start sub_802062C
sub_802062C: @ 0x0802062C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	adds r4, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r4, #0
	bl sub_8009ED0
	adds r7, r0, #0
	ldr r0, _08020774 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r7, #0x20]
	adds r0, r7, #0
	bl sub_800815C
	adds r2, r7, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r5, #0x18
	ldrsh r0, [r1, r5]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r7, #0
	bl sub_803AD80
	movs r0, #0x17
	str r0, [r6, #0x6c]
	str r6, [r7, #0x44]
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r7, #0
	bl sub_803AD80
	movs r3, #1
	mov r8, r3
	strb r3, [r7, #0xa]
	movs r0, #0x7f
	ldrb r5, [r7, #0xc]
	ands r0, r5
	strb r0, [r7, #0xc]
	ldr r0, _08020778 @ =gUnknown_030012B4
	mov sb, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r4, r4, #1
	adds r0, r4, r0
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r1, [r2]
	lsrs r0, r1, #1
	mov r3, r8
	eors r0, r3
	ands r0, r3
	adds r3, r7, #0
	adds r3, #0x28
	mov r5, r8
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	mov sl, r1
	ldrb r5, [r3]
	ands r1, r5
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	mov r2, r8
	ands r0, r2
	ands r0, r2
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0802077C @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r7, #0
	str r3, [sp]
	bl sub_8008E94
	ldr r0, _08020780 @ =gStaticData_0816B98C
	adds r5, r6, #0
	adds r5, #0x84
	str r0, [r5]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	adds r4, r4, r0
	ldr r0, [r1, #0xc]
	ldrh r4, [r4]
	adds r2, r4, r0
	ldr r3, [sp]
	ldrb r4, [r3]
	lsls r0, r4, #0x1b
	movs r1, #0
	cmp r0, #0
	blt _0802073A
	movs r1, #1
_0802073A:
	mov r0, r8
	ands r1, r0
	lsls r1, r1, #4
	mov r0, sl
	ands r0, r4
	orrs r0, r1
	strb r0, [r3]
	movs r1, #1
	strb r1, [r7, #0xa]
	ldr r0, _08020784 @ =gStaticData_0816BAAC
	str r0, [r5]
	ldr r0, [r2, #8]
	ldr r1, [r2, #4]
	ldr r2, [r2, #0xc]
	str r0, [r6, #0x30]
	str r1, [r6, #0x34]
	str r2, [r6, #0x38]
	adds r0, r6, #0
	movs r1, #4
	bl sub_800C6A8
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08020774: .4byte gUnknown_030012D0
_08020778: .4byte gUnknown_030012B4
_0802077C: .4byte gUnknown_030012F0
_08020780: .4byte gStaticData_0816B98C
_08020784: .4byte gStaticData_0816BAAC

	thumb_func_start sub_8020788
sub_8020788: @ 0x08020788
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r6, r0, #0
	ldr r0, _080208B4 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r7, #0x84
	lsls r7, r7, #1
	adds r0, r0, r7
	str r0, [r6, #0x20]
	adds r0, r6, #0
	bl sub_800815C
	adds r2, r6, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r5, r0, #0
	ldr r1, [r5, #0xc]
	movs r7, #0x18
	ldrsh r0, [r1, r7]
	adds r0, r5, r0
	ldr r2, [r1, #0x1c]
	adds r1, r6, #0
	bl sub_803AD80
	movs r0, #0x16
	str r0, [r5, #0x6c]
	str r5, [r6, #0x44]
	ldr r1, [r5, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x1c]
	adds r1, r6, #0
	bl sub_803AD80
	movs r0, #1
	movs r3, #0
	mov sl, r3
	movs r4, #1
	strb r0, [r6, #0xa]
	movs r0, #0x7f
	ldrb r7, [r6, #0xc]
	ands r0, r7
	strb r0, [r6, #0xc]
	ldr r0, _080208B8 @ =gUnknown_030012B4
	mov sb, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r4
	ands r0, r4
	adds r3, r6, #0
	adds r3, #0x28
	ands r0, r4
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r4
	ands r0, r4
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _080208BC @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r6, #0
	bl sub_8008E94
	ldr r1, _080208C0 @ =gStaticData_0816B98C
	adds r0, r5, #0
	adds r0, #0x84
	str r1, [r0]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r4, [r1, #0xc]
	mov r2, r8
	ldrh r2, [r2]
	adds r4, r2, r4
	adds r0, r5, #0
	movs r1, #9
	bl sub_800C6A8
	ldr r1, [r4, #4]
	ldr r2, [r4, #8]
	ldr r3, [r4, #0xc]
	adds r0, r5, #0
	bl sub_800C860
	movs r0, #0x80
	movs r1, #0x14
	str r0, [r5, #0x3c]
	mov r3, sl
	str r3, [r5, #0x40]
	str r1, [r5, #0x44]
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080208B4: .4byte gUnknown_030012D0
_080208B8: .4byte gUnknown_030012B4
_080208BC: .4byte gUnknown_030012F0
_080208C0: .4byte gStaticData_0816B98C

	thumb_func_start sub_80208C4
sub_80208C4: @ 0x080208C4
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _080209D8 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0xf0
	str r0, [r4, #0x20]
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r7, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #0x14
	str r0, [r6, #0x6c]
	str r6, [r4, #0x44]
	ldr r1, [r6, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #1
	movs r5, #1
	strb r0, [r4, #0xa]
	movs r0, #0x7f
	ldrb r7, [r4, #0xc]
	ands r0, r7
	strb r0, [r4, #0xc]
	ldr r0, _080209DC @ =gUnknown_030012B4
	mov sb, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r5
	ands r0, r5
	adds r3, r4, #0
	adds r3, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r5
	ands r0, r5
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _080209E0 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	ldr r0, _080209E4 @ =gStaticData_0816B98C
	adds r2, r6, #0
	adds r2, #0x84
	str r0, [r2]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r4, [r1, #0xc]
	mov r3, r8
	ldrh r3, [r3]
	adds r4, r3, r4
	ldr r0, _080209E8 @ =gStaticData_0816BB0C
	str r0, [r2]
	adds r0, r6, #0
	movs r1, #2
	bl sub_800C6A8
	ldr r1, [r4, #4]
	adds r0, r6, #0
	bl sub_800C898
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080209D8: .4byte gUnknown_030012D0
_080209DC: .4byte gUnknown_030012B4
_080209E0: .4byte gUnknown_030012F0
_080209E4: .4byte gStaticData_0816B98C
_080209E8: .4byte gStaticData_0816BB0C

	thumb_func_start sub_80209EC
sub_80209EC: @ 0x080209EC
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _08020AFC @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0xfc
	str r0, [r4, #0x20]
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r7, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #0x15
	str r0, [r6, #0x6c]
	str r6, [r4, #0x44]
	ldr r1, [r6, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #1
	movs r5, #1
	strb r0, [r4, #0xa]
	movs r0, #0x7f
	ldrb r7, [r4, #0xc]
	ands r0, r7
	strb r0, [r4, #0xc]
	ldr r0, _08020B00 @ =gUnknown_030012B4
	mov sb, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r5
	ands r0, r5
	adds r3, r4, #0
	adds r3, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r5
	ands r0, r5
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _08020B04 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	ldr r1, _08020B08 @ =gStaticData_0816B98C
	adds r0, r6, #0
	adds r0, #0x84
	str r1, [r0]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r4, [r1, #0xc]
	mov r2, r8
	ldrh r2, [r2]
	adds r4, r2, r4
	adds r0, r6, #0
	movs r1, #2
	bl sub_800C6A8
	ldr r1, [r4, #4]
	adds r0, r6, #0
	bl sub_800C898
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08020AFC: .4byte gUnknown_030012D0
_08020B00: .4byte gUnknown_030012B4
_08020B04: .4byte gUnknown_030012F0
_08020B08: .4byte gStaticData_0816B98C

	thumb_func_start sub_8020B0C
sub_8020B0C: @ 0x08020B0C
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r4, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r4, #0
	bl sub_8009ED0
	adds r5, r0, #0
	ldr r0, _08020C04 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0xe4
	str r0, [r5, #0x20]
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	mov r8, r0
	ldr r1, [r0, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	add r0, r8
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #0x13
	mov r3, r8
	str r0, [r3, #0x6c]
	str r3, [r5, #0x44]
	ldr r1, [r3, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	add r0, r8
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #1
	movs r6, #1
	strb r0, [r5, #0xa]
	movs r0, #0x7f
	ldrb r3, [r5, #0xc]
	ands r0, r3
	strb r0, [r5, #0xc]
	ldr r0, _08020C08 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r4, r4, #1
	adds r4, r4, r0
	ldr r2, [r1, #0xc]
	ldrh r4, [r4]
	adds r2, r4, r2
	ldrb r4, [r2]
	lsrs r0, r4, #1
	eors r0, r6
	ands r0, r6
	adds r3, r5, #0
	adds r3, #0x28
	ands r0, r6
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r4, [r3]
	ands r1, r4
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r6
	ands r0, r6
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _08020C0C @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	ldr r0, _08020C10 @ =gStaticData_0816B98C
	mov r1, r8
	adds r1, #0x84
	str r0, [r1]
	movs r0, #7
	strb r0, [r5, #0xa]
	ldr r0, _08020C14 @ =gStaticData_0816BB0C
	str r0, [r1]
	mov r0, r8
	movs r1, #8
	bl sub_800C6A8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08020C04: .4byte gUnknown_030012D0
_08020C08: .4byte gUnknown_030012B4
_08020C0C: .4byte gUnknown_030012F0
_08020C10: .4byte gStaticData_0816B98C
_08020C14: .4byte gStaticData_0816BB0C

	thumb_func_start sub_8020C18
sub_8020C18: @ 0x08020C18
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r5, r0, #0
	ldr r0, _08020D3C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x48
	str r0, [r5, #0x20]
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r7, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #6
	str r0, [r6, #0x6c]
	str r6, [r5, #0x44]
	ldr r1, [r6, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #1
	movs r4, #1
	strb r0, [r5, #0xa]
	movs r0, #0x7f
	ldrb r7, [r5, #0xc]
	ands r0, r7
	strb r0, [r5, #0xc]
	ldr r0, _08020D40 @ =gUnknown_030012B4
	mov sb, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r4
	ands r0, r4
	adds r3, r5, #0
	adds r3, #0x28
	ands r0, r4
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r4
	ands r0, r4
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _08020D44 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	ldr r1, _08020D48 @ =gStaticData_0816B98C
	adds r0, r6, #0
	adds r0, #0x84
	str r1, [r0]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r4, [r1, #0xc]
	mov r2, r8
	ldrh r2, [r2]
	adds r4, r2, r4
	movs r0, #4
	strb r0, [r5, #0xa]
	adds r0, r6, #0
	movs r1, #0xb
	bl sub_800C6A8
	ldr r1, [r4, #0x10]
	ldr r2, [r4, #0x14]
	ldr r3, [r4, #0x18]
	adds r0, r6, #0
	bl sub_800C860
	ldr r1, [r4, #4]
	ldr r2, [r4, #8]
	ldr r3, [r4, #0xc]
	adds r0, r6, #0
	bl sub_800C87C
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08020D3C: .4byte gUnknown_030012D0
_08020D40: .4byte gUnknown_030012B4
_08020D44: .4byte gUnknown_030012F0
_08020D48: .4byte gStaticData_0816B98C

	thumb_func_start sub_8020D4C
sub_8020D4C: @ 0x08020D4C
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _08020E70 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0xd8
	str r0, [r4, #0x20]
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r7, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #0x12
	str r0, [r6, #0x6c]
	str r6, [r4, #0x44]
	ldr r1, [r6, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #1
	movs r5, #1
	strb r0, [r4, #0xa]
	movs r0, #0x7f
	ldrb r7, [r4, #0xc]
	ands r0, r7
	strb r0, [r4, #0xc]
	ldr r0, _08020E74 @ =gUnknown_030012B4
	mov sb, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r5
	ands r0, r5
	adds r3, r4, #0
	adds r3, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r5
	ands r0, r5
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _08020E78 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	ldr r0, _08020E7C @ =gStaticData_0816B98C
	adds r2, r6, #0
	adds r2, #0x84
	str r0, [r2]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r1, [r1, #0xc]
	mov r3, r8
	ldrh r3, [r3]
	adds r1, r3, r1
	movs r0, #0xa
	strb r0, [r4, #0xa]
	subs r0, #0x4b
	ldrb r7, [r4, #0xc]
	ands r0, r7
	strb r0, [r4, #0xc]
	ldr r0, _08020E80 @ =gStaticData_0816BB2C
	str r0, [r2]
	ldr r0, [r1, #4]
	ldr r2, [r1, #8]
	ldr r1, [r1, #0xc]
	str r0, [r6, #0x30]
	str r2, [r6, #0x34]
	str r1, [r6, #0x38]
	adds r0, r6, #0
	movs r1, #4
	bl sub_800C6A8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08020E70: .4byte gUnknown_030012D0
_08020E74: .4byte gUnknown_030012B4
_08020E78: .4byte gUnknown_030012F0
_08020E7C: .4byte gStaticData_0816B98C
_08020E80: .4byte gStaticData_0816BB2C

@ sub_8020E84/sub_8020F7C/sub_802107C/sub_802117C (the "trigger effect
@ type N" twin family) are reconstructed (extremely close, but not yet
@ byte-matching) as C in src/graphics/trigger_effect.c, guarded by
@ #if NON_MATCHING - this raw version is used only for the real
@ byte-matching build. See docs/matching.md, "Parked, not matched:
@ sub_8020E84/sub_8020F7C/sub_802107C/sub_802117C".
.if NON_MATCHING == 0
	thumb_func_start sub_8020E84
sub_8020E84: @ 0x08020E84
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	adds r4, r0, #0
	lsls r1, r1, #0x10
	lsrs r6, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r7, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r5, r3, #0x10
	ldr r0, _08020ECC @ =gUnknown_030012C0
	mov sb, r0
	ldr r0, [r0]
	movs r1, #1
	ldrb r2, [r0, #2]
	ands r2, r1
	mov r8, r2
	cmp r2, #0
	beq _08020EF4
	bl sub_8023278
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08020EC4
	mov r3, sb
	ldr r0, [r3]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _08020ED0
_08020EC4:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	movs r1, #0xc
	b _08020ED6
	.align 2, 0
_08020ECC: .4byte gUnknown_030012C0
_08020ED0:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	movs r1, #0xb
_08020ED6:
	str r1, [sp]
	adds r1, r6, #0
	adds r2, r7, #0
	adds r3, r5, #0
	bl sub_801A878
	adds r1, r0, #0
	ldr r0, _08020EF0 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80234E8
	b _08020F66
	.align 2, 0
_08020EF0: .4byte gUnknown_030012C0
_08020EF4:
	movs r0, #7
	mov sb, r0
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	adds r1, r6, #0
	adds r2, r7, #0
	adds r3, r5, #0
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _08020F74 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r2, sb
	strb r2, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	mov r0, r8
	strb r0, [r4, #0xa]
	ldr r0, _08020F78 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r1, [r4, #0xc]
	ands r0, r1
	strb r0, [r4, #0xc]
_08020F66:
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08020F74: .4byte gUnknown_030012D0
_08020F78: .4byte gUnknown_030012EC

	thumb_func_start sub_8020F7C
sub_8020F7C: @ 0x08020F7C
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	adds r4, r0, #0
	lsls r1, r1, #0x10
	lsrs r6, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r7, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r5, r3, #0x10
	ldr r0, _08020FCC @ =gUnknown_030012C0
	mov r8, r0
	ldr r1, [r0]
	movs r0, #2
	ldrb r2, [r1, #2]
	ands r0, r2
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	mov sb, r0
	cmp r0, #0
	beq _08020FF4
	adds r0, r1, #0
	bl sub_8023278
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08020FC2
	mov r3, r8
	ldr r0, [r3]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _08020FD0
_08020FC2:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	movs r1, #0xc
	b _08020FD6
	.align 2, 0
_08020FCC: .4byte gUnknown_030012C0
_08020FD0:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	movs r1, #3
_08020FD6:
	str r1, [sp]
	adds r1, r6, #0
	adds r2, r7, #0
	adds r3, r5, #0
	bl sub_801A878
	adds r1, r0, #0
	ldr r0, _08020FF0 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80234E8
	b _08021066
	.align 2, 0
_08020FF0: .4byte gUnknown_030012C0
_08020FF4:
	movs r0, #5
	mov r8, r0
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	adds r1, r6, #0
	adds r2, r7, #0
	adds r3, r5, #0
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _08021074 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r2, r8
	strb r2, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	mov r0, sb
	strb r0, [r4, #0xa]
	ldr r0, _08021078 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r1, [r4, #0xc]
	ands r0, r1
	strb r0, [r4, #0xc]
_08021066:
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08021074: .4byte gUnknown_030012D0
_08021078: .4byte gUnknown_030012EC

	thumb_func_start sub_802107C
sub_802107C: @ 0x0802107C
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	adds r4, r0, #0
	lsls r1, r1, #0x10
	lsrs r6, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r7, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r5, r3, #0x10
	ldr r0, _080210CC @ =gUnknown_030012C0
	mov r8, r0
	ldr r1, [r0]
	movs r0, #4
	ldrb r2, [r1, #2]
	ands r0, r2
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	mov sb, r0
	cmp r0, #0
	beq _080210F4
	adds r0, r1, #0
	bl sub_8023278
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _080210C2
	mov r3, r8
	ldr r0, [r3]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _080210D0
_080210C2:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	movs r1, #0xc
	b _080210D6
	.align 2, 0
_080210CC: .4byte gUnknown_030012C0
_080210D0:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	movs r1, #0xa
_080210D6:
	str r1, [sp]
	adds r1, r6, #0
	adds r2, r7, #0
	adds r3, r5, #0
	bl sub_801A878
	adds r1, r0, #0
	ldr r0, _080210F0 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80234E8
	b _08021166
	.align 2, 0
_080210F0: .4byte gUnknown_030012C0
_080210F4:
	movs r0, #6
	mov r8, r0
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	adds r1, r6, #0
	adds r2, r7, #0
	adds r3, r5, #0
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _08021174 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r2, r8
	strb r2, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	mov r0, sb
	strb r0, [r4, #0xa]
	ldr r0, _08021178 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r1, [r4, #0xc]
	ands r0, r1
	strb r0, [r4, #0xc]
_08021166:
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08021174: .4byte gUnknown_030012D0
_08021178: .4byte gUnknown_030012EC

	thumb_func_start sub_802117C
sub_802117C: @ 0x0802117C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	adds r4, r0, #0
	lsls r1, r1, #0x10
	lsrs r5, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r6, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r7, r3, #0x10
	ldr r0, _080211D0 @ =gUnknown_030012C0
	mov r8, r0
	ldr r1, [r0]
	movs r2, #8
	mov sl, r2
	mov r0, sl
	ldrb r3, [r1, #2]
	ands r0, r3
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	mov sb, r0
	cmp r0, #0
	beq _080211F8
	adds r0, r1, #0
	bl sub_8023278
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _080211C8
	mov r1, r8
	ldr r0, [r1]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _080211D4
_080211C8:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	movs r1, #0xc
	b _080211DA
	.align 2, 0
_080211D0: .4byte gUnknown_030012C0
_080211D4:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	movs r1, #9
_080211DA:
	str r1, [sp]
	adds r1, r5, #0
	adds r2, r6, #0
	adds r3, r7, #0
	bl sub_801A878
	adds r1, r0, #0
	ldr r0, _080211F4 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80234E8
	b _08021266
	.align 2, 0
_080211F4: .4byte gUnknown_030012C0
_080211F8:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	adds r1, r5, #0
	adds r2, r6, #0
	adds r3, r7, #0
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _08021278 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r2, #0xc0
	lsls r2, r2, #1
	adds r0, r0, r2
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
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	mov r0, sb
	strb r0, [r4, #0xa]
	ldr r0, _0802127C @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r1, [r4, #0xc]
	ands r0, r1
	strb r0, [r4, #0xc]
_08021266:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08021278: .4byte gUnknown_030012D0
_0802127C: .4byte gUnknown_030012EC
.endif

	thumb_func_start sub_8021280
sub_8021280: @ 0x08021280
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #0xc
	adds r6, r0, #0
	lsls r1, r1, #0x10
	lsrs r7, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	mov r8, r2
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov sb, r3
	ldr r5, _08021300 @ =gUnknown_030012C0
	ldr r0, [r5]
	bl sub_8023290
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0802130C
	ldr r0, [r5]
	bl sub_80232B8
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0802130C
	ldr r0, [r5]
	bl sub_8023324
	cmp r0, #0
	bne _0802130C
	ldr r4, _08021304 @ =gStaticData_0816C86C
	ldr r0, [r5]
	bl sub_802332C
	lsls r1, r0, #3
	adds r1, r1, r0
	lsls r1, r1, #2
	adds r4, #4
	adds r1, r1, r4
	ldr r0, [r1]
	cmp r0, #0
	bne _0802130C
	lsls r0, r6, #0x10
	lsrs r0, r0, #0x10
	adds r1, r7, #0
	mov r2, r8
	mov r3, sb
	bl sub_80071E4
	adds r4, r0, #0
	movs r1, #0x64
	movs r2, #0x64
	bl sub_80070EC
	movs r0, #0x12
	strb r0, [r4, #0xa]
	ldr r0, _08021308 @ =gUnknown_030012E8
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	b _08021376
	.align 2, 0
_08021300: .4byte gUnknown_030012C0
_08021304: .4byte gStaticData_0816C86C
_08021308: .4byte gUnknown_030012E8
_0802130C:
	ldr r0, _08021348 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #0
	bne _08021350
	lsls r0, r6, #0x10
	lsrs r0, r0, #0x10
	movs r1, #4
	str r1, [sp]
	adds r1, r7, #0
	mov r2, r8
	mov r3, sb
	bl sub_801A878
	ldr r1, [r0]
	asrs r1, r1, #8
	subs r2, r1, #2
	ldr r0, [r0, #4]
	asrs r0, r0, #8
	adds r3, r0, #0
	subs r3, #0x1e
	str r2, [sp, #4]
	str r3, [sp, #8]
	ldr r0, _0802134C @ =gUnknown_030012C0
	ldr r0, [r0]
	add r1, sp, #4
	bl sub_8023500
	b _08021376
	.align 2, 0
_08021348: .4byte gUnknown_030012D8
_0802134C: .4byte gUnknown_030012C0
_08021350:
	lsls r0, r6, #0x10
	lsrs r0, r0, #0x10
	adds r1, r7, #0
	mov r2, r8
	mov r3, sb
	bl sub_80071E4
	adds r4, r0, #0
	movs r1, #0x28
	movs r2, #0x28
	bl sub_80070EC
	movs r0, #0x12
	strb r0, [r4, #0xa]
	ldr r0, _08021384 @ =gUnknown_030012E8
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
_08021376:
	add sp, #0xc
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08021384: .4byte gUnknown_030012E8

	thumb_func_start sub_8021388
sub_8021388: @ 0x08021388
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	mov r8, r1
	mov sb, r2
	adds r4, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	mov r8, r1
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	mov sb, r2
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r4, #0
	bl sub_8009ED0
	adds r6, r0, #0
	ldr r0, _08021470 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r3, #0xa2
	lsls r3, r3, #2
	adds r0, r0, r3
	str r0, [r6, #0x20]
	adds r0, r6, #0
	bl sub_800815C
	adds r2, r6, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x10
	ldrb r1, [r6, #0xc]
	orrs r0, r1
	strb r0, [r6, #0xc]
	movs r0, #1
	movs r5, #1
	strb r0, [r6, #0xa]
	ldr r0, _08021474 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r4, r4, #1
	adds r4, r4, r0
	ldr r2, [r1, #0xc]
	ldrh r4, [r4]
	adds r2, r4, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r5
	ands r0, r5
	adds r3, r6, #0
	adds r3, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r4, [r3]
	ands r1, r4
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r5
	ands r0, r5
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _08021478 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r6, #0
	bl sub_8008E94
	movs r0, #0x30
	bl sub_8026EDC
	mov r1, r8
	mov r2, sb
	bl sub_801A838
	adds r4, r0, #0
	str r4, [r6, #0x44]
	ldr r1, [r4, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x1c]
	adds r1, r6, #0
	bl sub_803AD80
	ldr r0, _0802147C @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8023318
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08021470: .4byte gUnknown_030012D0
_08021474: .4byte gUnknown_030012B4
_08021478: .4byte gUnknown_030012F0
_0802147C: .4byte gUnknown_030012C0

	thumb_func_start sub_8021480
sub_8021480: @ 0x08021480
	push {r4, r5, r6, r7, lr}
	adds r4, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r4, #0
	bl sub_8009ED0
	adds r5, r0, #0
	ldr r0, _0802154C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xa5
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r5, #0x20]
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x4c
	bl sub_8026EDC
	bl sub_80189EC
	adds r6, r0, #0
	str r6, [r5, #0x44]
	ldr r1, [r6, #0xc]
	movs r7, #0x18
	ldrsh r0, [r1, r7]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	ldr r0, _08021550 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r4, r4, #1
	adds r4, r4, r0
	ldr r3, [r1, #0xc]
	ldrh r4, [r4]
	adds r3, r4, r3
	ldrb r1, [r3]
	lsrs r0, r1, #1
	movs r2, #1
	eors r0, r2
	ands r0, r2
	adds r4, r5, #0
	adds r4, #0x28
	ands r0, r2
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r4]
	ands r1, r7
	orrs r1, r0
	strb r1, [r4]
	ldrb r3, [r3]
	lsrs r0, r3, #2
	ands r0, r2
	ands r0, r2
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r4]
	movs r0, #0x10
	ldrb r1, [r5, #0xc]
	orrs r0, r1
	strb r0, [r5, #0xc]
	ldr r0, _08021554 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	ldr r0, _08021558 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r1, r6, #0
	bl sub_8023318
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802154C: .4byte gUnknown_030012D0
_08021550: .4byte gUnknown_030012B4
_08021554: .4byte gUnknown_030012F0
_08021558: .4byte gUnknown_030012C0

	thumb_func_start sub_802155C
sub_802155C: @ 0x0802155C
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	adds r5, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r5, #0
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _08021658 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x9f
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x20]
	movs r0, #1
	adds r1, r4, #0
	adds r1, #0x2d
	movs r2, #0
	mov sb, r2
	movs r6, #1
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x24
	bl sub_8026EDC
	bl sub_80197DC
	mov r8, r0
	str r0, [r4, #0x44]
	ldr r1, [r0, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	add r0, r8
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	ldr r0, _0802165C @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r5, r5, #1
	adds r5, r5, r0
	ldr r2, [r1, #0xc]
	ldrh r5, [r5]
	adds r2, r5, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r6
	ands r0, r6
	adds r3, r4, #0
	adds r3, #0x28
	ands r0, r6
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r5, [r3]
	ands r1, r5
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r6
	ands r0, r6
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	movs r0, #0x10
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _08021660 @ =gUnknown_030012F4
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	adds r4, #0x2c
	mov r2, sb
	strb r2, [r4]
	ldr r0, _08021664 @ =gUnknown_030012C0
	ldr r0, [r0]
	mov r1, r8
	bl sub_8023318
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08021658: .4byte gUnknown_030012D0
_0802165C: .4byte gUnknown_030012B4
_08021660: .4byte gUnknown_030012F4
_08021664: .4byte gUnknown_030012C0

	thumb_func_start sub_8021668
sub_8021668: @ 0x08021668
	push {r4, r5, r6, lr}
	adds r4, r1, #0
	adds r5, r2, #0
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_8009ED0
	adds r6, r0, #0
	ldr r0, _0802173C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xb4
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r6, #0x20]
	lsls r4, r4, #8
	str r4, [r6]
	lsls r5, r5, #8
	str r5, [r6, #4]
	movs r0, #0
	adds r1, r6, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r6, #0
	bl sub_80087C0
	adds r0, r6, #0
	bl sub_80087B4
	adds r0, r6, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, _08021740 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r1, [r6, #0x20]
	ldr r1, [r1]
	ldrb r1, [r1, #0x14]
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	adds r2, r6, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	subs r2, #1
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r1, [r2]
	ands r0, r1
	movs r1, #0x21
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r2]
	movs r0, #0x24
	bl sub_8026EDC
	bl sub_8017FE8
	str r0, [r6, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x1c]
	adds r1, r6, #0
	bl sub_803AD80
	movs r0, #1
	strb r0, [r6, #0xa]
	movs r0, #0x7f
	ldrb r1, [r6, #0xc]
	ands r0, r1
	movs r1, #5
	rsbs r1, r1, #0
	ands r0, r1
	subs r1, #0x3c
	ands r0, r1
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r6, #0xc]
	ldr r0, _08021744 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r6, #0
	bl sub_8008E94
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802173C: .4byte gUnknown_030012D0
_08021740: .4byte gUnknown_030012B8
_08021744: .4byte gUnknown_030012F0

	thumb_func_start sub_8021748
sub_8021748: @ 0x08021748
	push {r4, r5, lr}
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _080217C8 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x87
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x20]
	movs r5, #0
	adds r0, r4, #0
	adds r0, #0x2d
	strb r5, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x7f
	ldrb r1, [r4, #0xc]
	ands r0, r1
	movs r1, #5
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r4, #0xc]
	strb r5, [r4, #0xa]
	ldr r0, _080217CC @ =gUnknown_030012F8
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080217C8: .4byte gUnknown_030012D0
_080217CC: .4byte gUnknown_030012F8

	thumb_func_start sub_80217D0
sub_80217D0: @ 0x080217D0
	push {r4, lr}
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _08021834 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x87
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x7f
	ldrb r1, [r4, #0xc]
	ands r0, r1
	movs r1, #5
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r4, #0xc]
	movs r0, #0
	strb r0, [r4, #0xa]
	ldr r0, _08021838 @ =gUnknown_030012F8
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08021834: .4byte gUnknown_030012D0
_08021838: .4byte gUnknown_030012F8

	thumb_func_start sub_802183C
sub_802183C: @ 0x0802183C
	push {r4, r5, lr}
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _080218BC @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x84
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x20]
	movs r5, #0
	adds r0, r4, #0
	adds r0, #0x2d
	strb r5, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x7f
	ldrb r1, [r4, #0xc]
	ands r0, r1
	movs r1, #5
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r4, #0xc]
	strb r5, [r4, #0xa]
	ldr r0, _080218C0 @ =gUnknown_030012F8
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080218BC: .4byte gUnknown_030012D0
_080218C0: .4byte gUnknown_030012F8

	thumb_func_start sub_80218C4
sub_80218C4: @ 0x080218C4
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #8
	str r4, [sp]
	bl sub_801A878
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_80218E8
sub_80218E8: @ 0x080218E8
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #6
	str r4, [sp]
	bl sub_801A878
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_802190C
sub_802190C: @ 0x0802190C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r5, r0, #0
	lsls r1, r1, #0x10
	lsrs r6, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r7, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	ldr r4, _08021944 @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_80232A0
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0802193C
	ldr r0, [r4]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _08021948
_0802193C:
	lsls r0, r5, #0x10
	lsrs r0, r0, #0x10
	movs r1, #7
	b _0802194E
	.align 2, 0
_08021944: .4byte gUnknown_030012C0
_08021948:
	lsls r0, r5, #0x10
	lsrs r0, r0, #0x10
	movs r1, #5
_0802194E:
	str r1, [sp]
	adds r1, r6, #0
	adds r2, r7, #0
	mov r3, r8
	bl sub_801A878
	adds r1, r0, #0
	ldr r0, _08021970 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80234F4
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08021970: .4byte gUnknown_030012C0

	thumb_func_start sub_8021974
sub_8021974: @ 0x08021974
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #2
	str r4, [sp]
	bl sub_801A878
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021998
sub_8021998: @ 0x08021998
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #1
	str r4, [sp]
	bl sub_801A878
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_80219BC
sub_80219BC: @ 0x080219BC
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0
	str r4, [sp]
	bl sub_801A878
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_80219E0
sub_80219E0: @ 0x080219E0
	push {lr}
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_801B984
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start nullsub_21
nullsub_21: @ 0x080219FC
	bx lr
	.align 2, 0

	thumb_func_start sub_8021A00
sub_8021A00: @ 0x08021A00
	push {r4, r5, lr}
	adds r4, r1, #0
	adds r5, r2, #0
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	movs r0, #0x28
	bl sub_8026EDC
	bl sub_800CB40
	adds r1, r0, #0
	movs r2, #0
	ldr r0, _08021A44 @ =sub_801F680
	str r0, [r1, #0x1c]
	movs r0, #0x78
	str r0, [r1, #0x20]
	str r2, [r1, #0x24]
	lsls r4, r4, #8
	str r4, [r1]
	lsls r5, r5, #8
	str r5, [r1, #4]
	movs r0, #0x10
	ldrb r2, [r1, #0xc]
	orrs r0, r2
	strb r0, [r1, #0xc]
	ldr r0, _08021A48 @ =gUnknown_030012E8
	ldr r0, [r0]
	bl sub_8008E94
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08021A44: .4byte sub_801F680
_08021A48: .4byte gUnknown_030012E8

	thumb_func_start sub_8021A4C
sub_8021A4C: @ 0x08021A4C
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0x12
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021A70
sub_8021A70: @ 0x08021A70
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0x11
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021A94
sub_8021A94: @ 0x08021A94
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0x10
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021AB8
sub_8021AB8: @ 0x08021AB8
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0xf
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021ADC
sub_8021ADC: @ 0x08021ADC
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0xe
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021B00
sub_8021B00: @ 0x08021B00
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0xd
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021B24
sub_8021B24: @ 0x08021B24
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0xc
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021B48
sub_8021B48: @ 0x08021B48
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0xb
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021B6C
sub_8021B6C: @ 0x08021B6C
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0xa
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021B90
sub_8021B90: @ 0x08021B90
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #9
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021BB4
sub_8021BB4: @ 0x08021BB4
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #8
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021BD8
sub_8021BD8: @ 0x08021BD8
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #7
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021BFC
sub_8021BFC: @ 0x08021BFC
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r4, r0, #0
	lsls r1, r1, #0x10
	lsrs r5, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r6, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r7, r3, #0x10
	ldr r0, _08021C30 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80232C8
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08021C34
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	movs r1, #7
	str r1, [sp]
	adds r1, r5, #0
	adds r2, r6, #0
	adds r3, r7, #0
	bl sub_800FF0C
	b _08021C46
	.align 2, 0
_08021C30: .4byte gUnknown_030012C0
_08021C34:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	movs r1, #6
	str r1, [sp]
	adds r1, r5, #0
	adds r2, r6, #0
	adds r3, r7, #0
	bl sub_800FF0C
_08021C46:
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8021C50
sub_8021C50: @ 0x08021C50
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #5
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021C74
sub_8021C74: @ 0x08021C74
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #4
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021C98
sub_8021C98: @ 0x08021C98
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #3
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021CBC
sub_8021CBC: @ 0x08021CBC
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #2
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021CE0
sub_8021CE0: @ 0x08021CE0
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #1
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021D04
sub_8021D04: @ 0x08021D04
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r3, #0
	str r3, [sp]
	adds r3, r4, #0
	bl sub_800FF0C
	adds r5, r0, #0
	ldr r0, _08021D7C @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r4, r4, #1
	adds r4, r4, r0
	ldr r0, [r1, #0xc]
	ldrh r4, [r4]
	adds r0, r4, r0
	adds r3, r0, #0
	movs r0, #2
	ldrb r1, [r3]
	ands r0, r1
	cmp r0, #0
	beq _08021D56
	adds r0, r5, #0
	adds r0, #0x28
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r2, [r0]
	ands r1, r2
	movs r2, #0x10
	orrs r1, r2
	strb r1, [r0]
_08021D56:
	movs r0, #4
	ldrb r3, [r3]
	ands r0, r3
	cmp r0, #0
	beq _08021D72
	adds r0, r5, #0
	adds r0, #0x28
	movs r1, #0x21
	rsbs r1, r1, #0
	ldrb r2, [r0]
	ands r1, r2
	movs r2, #0x20
	orrs r1, r2
	strb r1, [r0]
_08021D72:
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08021D7C: .4byte gUnknown_030012B4

	thumb_func_start sub_8021D80
sub_8021D80: @ 0x08021D80
	push {r4, r5, r6, lr}
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	movs r5, #1
	movs r6, #0x25
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _08021DF4 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xe4
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	strb r5, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	strb r6, [r4, #0xa]
	ldr r0, _08021DF8 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08021DF4: .4byte gUnknown_030012D0
_08021DF8: .4byte gUnknown_030012EC

	thumb_func_start sub_8021DFC
sub_8021DFC: @ 0x08021DFC
	push {r4, r5, r6, lr}
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	movs r5, #0
	movs r6, #0x24
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _08021E70 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xe4
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	strb r5, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	strb r6, [r4, #0xa]
	ldr r0, _08021E74 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08021E70: .4byte gUnknown_030012D0
_08021E74: .4byte gUnknown_030012EC

	thumb_func_start sub_8021E78
sub_8021E78: @ 0x08021E78
	push {r4, r5, r6, lr}
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	movs r5, #2
	movs r6, #0x23
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _08021EEC @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xe4
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	strb r5, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	strb r6, [r4, #0xa]
	ldr r0, _08021EF0 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08021EEC: .4byte gUnknown_030012D0
_08021EF0: .4byte gUnknown_030012EC

	thumb_func_start sub_8021EF4
sub_8021EF4: @ 0x08021EF4
	push {r4, r5, r6, lr}
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	movs r5, #3
	movs r6, #0x26
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _08021F68 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xe4
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	strb r5, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	strb r6, [r4, #0xa]
	ldr r0, _08021F6C @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08021F68: .4byte gUnknown_030012D0
_08021F6C: .4byte gUnknown_030012EC

	thumb_func_start sub_8021F70
sub_8021F70: @ 0x08021F70
	push {r4, r5, r6, r7, lr}
	adds r7, r0, #0
	lsls r1, r1, #0x10
	lsrs r6, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r5, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r4, r3, #0x10
	ldr r0, _08022000 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023418
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08021FFA
	lsls r0, r7, #0x10
	lsrs r0, r0, #0x10
	adds r1, r6, #0
	adds r2, r5, #0
	adds r3, r4, #0
	bl sub_8011B0C
	adds r4, r0, #0
	ldr r0, _08022004 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xd8
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	movs r0, #0
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
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x1c
	strb r0, [r4, #0xa]
	movs r0, #0x10
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _08022008 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
_08021FFA:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08022000: .4byte gUnknown_030012C0
_08022004: .4byte gUnknown_030012D0
_08022008: .4byte gUnknown_030012EC

	thumb_func_start sub_802200C
sub_802200C: @ 0x0802200C
	push {r4, r5, r6, r7, lr}
	adds r7, r0, #0
	lsls r1, r1, #0x10
	lsrs r4, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	ldr r0, _08022090 @ =gUnknown_030012C0
	ldr r1, [r0]
	movs r0, #8
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	bne _0802208A
	movs r5, #4
	movs r6, #0x21
	lsls r0, r7, #0x10
	lsrs r0, r0, #0x10
	adds r1, r4, #0
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _08022094 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	strb r5, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	strb r6, [r4, #0xa]
	ldr r0, _08022098 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
_0802208A:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08022090: .4byte gUnknown_030012C0
_08022094: .4byte gUnknown_030012D0
_08022098: .4byte gUnknown_030012EC

	thumb_func_start sub_802209C
sub_802209C: @ 0x0802209C
	push {r4, lr}
	sub sp, #8
	lsls r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r3, r1, #0x10
	lsrs r4, r2, #0x10
	str r3, [sp]
	str r4, [sp, #4]
	ldr r0, _080220C0 @ =gUnknown_030012C0
	ldr r0, [r0]
	mov r1, sp
	bl sub_8023500
	add sp, #8
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080220C0: .4byte gUnknown_030012C0

	thumb_func_start sub_80220C4
sub_80220C4: @ 0x080220C4
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r5, r0, #0
	adds r6, r1, #0
	mov r8, r2
	adds r0, r3, #0
	ldr r1, [sp, #0x14]
	ldr r2, [sp, #0x18]
	ldr r3, [sp, #0x1c]
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _08022150 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	lsls r1, r5, #1
	adds r1, r1, r5
	lsls r1, r1, #2
	ldr r0, [r0]
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	strb r6, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	mov r0, r8
	strb r0, [r4, #0xa]
	ldr r0, _08022154 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	adds r0, r4, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08022150: .4byte gUnknown_030012D0
_08022154: .4byte gUnknown_030012EC

	thumb_func_start sub_8022158
sub_8022158: @ 0x08022158
	push {r4, lr}
	adds r4, r0, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	ldr r0, _08022184 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802217C
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	bl sub_801173C
_0802217C:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08022184: .4byte gUnknown_030012C0

	thumb_func_start nullsub_22
nullsub_22: @ 0x08022188
	bx lr
	.align 2, 0

	thumb_func_start sub_802218C
sub_802218C: @ 0x0802218C
	push {lr}
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	bl sub_801E990
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80221A4
sub_80221A4: @ 0x080221A4
	lsls r1, r1, #0x10
	lsls r2, r2, #0x10
	ldr r0, _080221B8 @ =gUnknown_030012D8
	ldr r0, [r0]
	lsrs r1, r1, #8
	str r1, [r0]
	lsrs r2, r2, #8
	str r2, [r0, #4]
	bx lr
	.align 2, 0
_080221B8: .4byte gUnknown_030012D8

	thumb_func_start sub_80221BC
sub_80221BC: @ 0x080221BC
	push {lr}
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	bl sub_801E990
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80221D4
sub_80221D4: @ 0x080221D4
	lsls r1, r1, #0x10
	lsls r2, r2, #0x10
	ldr r0, _080221E8 @ =gUnknown_030012D8
	ldr r0, [r0]
	lsrs r1, r1, #8
	str r1, [r0]
	lsrs r2, r2, #8
	str r2, [r0, #4]
	bx lr
	.align 2, 0
_080221E8: .4byte gUnknown_030012D8

	thumb_func_start nullsub_23
nullsub_23: @ 0x080221EC
	bx lr
	.align 2, 0

	thumb_func_start sub_80221F0
sub_80221F0: @ 0x080221F0
	push {lr}
	ldr r0, _08022204 @ =gUnknown_030012E4
	ldr r0, [r0]
	cmp r0, #0
	beq _08022200
	movs r1, #3
	bl sub_8025D54
_08022200:
	pop {r0}
	bx r0
	.align 2, 0
_08022204: .4byte gUnknown_030012E4

	thumb_func_start sub_8022208
sub_8022208: @ 0x08022208
	push {r4, lr}
	ldr r4, _08022228 @ =gUnknown_030012E4
	movs r0, #8
	bl sub_8026EDC
	bl sub_8025D6C
	str r0, [r4]
	ldr r1, _0802222C @ =gStaticData_0816C6A4
	movs r2, #0x5c
	bl sub_8025D4C
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08022228: .4byte gUnknown_030012E4
_0802222C: .4byte gStaticData_0816C6A4

	thumb_func_start sub_8022230
sub_8022230: @ 0x08022230
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	ldr r4, _08022318 @ =gUnknown_030012BC
	ldr r0, _0802231C @ =0x00002094
	bl sub_80016DC
	bl sub_8001C2C
	str r0, [r4]
	bl sub_8001C80
	ldr r0, [r4]
	movs r1, #0xc0
	bl sub_8001B50
	ldr r0, [r4]
	movs r1, #0xc0
	bl sub_8001B30
	ldr r4, _08022320 @ =gUnknown_030012CC
	movs r0, #4
	bl sub_8026EDC
	bl nullsub_2
	str r0, [r4]
	ldr r4, _08022324 @ =gUnknown_030012D0
	movs r0, #4
	bl sub_8026EDC
	bl nullsub_1
	str r0, [r4]
	ldr r4, _08022328 @ =gStaticData_084A5600
	str r4, [r0]
	ldr r5, _0802232C @ =gUnknown_030012B8
	movs r0, #0x8c
	lsls r0, r0, #2
	bl sub_8026EDC
	bl sub_8006FB4
	str r0, [r5]
	ldrh r1, [r4, #0xe]
	ldr r2, [r4, #8]
	bl sub_8006EF0
	ldr r5, _08022330 @ =gUnknown_030012DC
	movs r4, #0x9a
	lsls r4, r4, #1
	adds r0, r4, #0
	bl sub_8026EDC
	bl InitHudIconWidgetA
	str r0, [r5]
	ldr r5, _08022334 @ =gUnknown_030012E0
	adds r0, r4, #0
	bl sub_8026EDC
	bl InitHudIconWidgetB
	str r0, [r5]
	bl AllocVramDmaQueue
	ldr r4, _08022338 @ =gUnknown_03001300
	ldr r0, _0802233C @ =0x0000040C
	bl sub_8026EDC
	bl sub_8006B0C
	str r0, [r4]
	ldr r4, _08022340 @ =gUnknown_030012FC
	movs r0, #0xc
	bl sub_8026EDC
	movs r1, #0
	bl sub_8006CE8
	str r0, [r4]
	ldr r4, _08022344 @ =gUnknown_03001304
	movs r0, #4
	bl sub_8026EDC
	bl sub_80007DC
	str r0, [r4]
	ldr r4, _08022348 @ =gUnknown_030012B4
	movs r0, #0x81
	lsls r0, r0, #3
	bl sub_8026EDC
	bl sub_8025A5C
	str r0, [r4]
	ldr r4, _0802234C @ =gUnknown_030012C8
	movs r0, #0x48
	bl sub_8026EDC
	bl sub_80270C0
	str r0, [r4]
	ldr r0, _08022350 @ =gUnknown_03001288
	movs r4, #0
	strh r4, [r0]
	bl sub_8001604
	bl sub_8001614
	adds r0, r6, #0
	adds r0, #0xc0
	str r4, [r0]
	adds r0, r6, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08022318: .4byte gUnknown_030012BC
_0802231C: .4byte 0x00002094
_08022320: .4byte gUnknown_030012CC
_08022324: .4byte gUnknown_030012D0
_08022328: .4byte gStaticData_084A5600
_0802232C: .4byte gUnknown_030012B8
_08022330: .4byte gUnknown_030012DC
_08022334: .4byte gUnknown_030012E0
_08022338: .4byte gUnknown_03001300
_0802233C: .4byte 0x0000040C
_08022340: .4byte gUnknown_030012FC
_08022344: .4byte gUnknown_03001304
_08022348: .4byte gUnknown_030012B4
_0802234C: .4byte gUnknown_030012C8
_08022350: .4byte gUnknown_03001288

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

