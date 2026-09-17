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

