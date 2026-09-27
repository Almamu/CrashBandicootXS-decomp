.include "asm/macros.inc"

.syntax unified
.arm

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
