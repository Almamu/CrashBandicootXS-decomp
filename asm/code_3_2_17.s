.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_800B8DC
sub_800B8DC: @ 0x0800B8DC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #8
	adds r5, r0, #0
	ldr r0, [r5, #0x74]
	subs r0, #1
	cmp r0, #0x11
	bls _0800B8F0
	b _0800BD3A
_0800B8F0:
	lsls r0, r0, #2
	ldr r1, _0800B8FC @ =_0800B900
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800B8FC: .4byte _0800B900
_0800B900: @ jump table
	.4byte _0800BD3A @ case 0
	.4byte _0800BC48 @ case 1
	.4byte _0800BC40 @ case 2
	.4byte _0800BC56 @ case 3
	.4byte _0800BA10 @ case 4
	.4byte _0800BC6C @ case 5
	.4byte _0800BC74 @ case 6
	.4byte _0800BC7C @ case 7
	.4byte _0800BC84 @ case 8
	.4byte _0800BD26 @ case 9
	.4byte _0800BCD8 @ case 10
	.4byte _0800BD3A @ case 11
	.4byte _0800BC50 @ case 12
	.4byte _0800BC5E @ case 13
	.4byte _0800BD20 @ case 14
	.4byte _0800BD2E @ case 15
	.4byte _0800B948 @ case 16
	.4byte _0800BAB2 @ case 17
_0800B948:
	ldr r2, [r5, #0x70]
	ldr r1, [r2, #4]
	ldr r0, [r5, #0x64]
	cmp r1, r0
	blt _0800B9D2
	ldr r0, [r2, #0x60]
	cmp r0, #0
	bne _0800B968
	ldr r0, [r2, #0x64]
	cmp r0, #0
	beq _0800B968
	adds r0, r5, #0
	movs r1, #0
	bl sub_800C8AC
	b _0800B9D2
_0800B968:
	ldr r1, [r5, #0x70]
	ldr r0, [r1, #0x60]
	cmp r0, #0
	bge _0800B9CC
	adds r0, r1, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800B982
	adds r0, r5, #0
	movs r1, #0
	bl sub_800C8CC
_0800B982:
	ldr r0, [r5, #0x70]
	ldr r2, [r0, #0x18]
	movs r3, #0x28
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r2, #0x2c]
	bl sub_803AD7C
	lsls r0, r0, #0x18
	lsrs r4, r0, #0x18
	cmp r4, #0
	bne _0800B9D2
	ldr r2, [r5, #0x70]
	ldr r1, [r5, #0x60]
	ldr r0, [r5, #0x64]
	ldr r6, _0800B9C8 @ =0xFFFF9C00
	adds r0, r0, r6
	str r1, [r2]
	str r0, [r2, #4]
	adds r0, r5, #0
	movs r1, #0
	bl sub_800C8BC
	ldr r1, [r5, #0x70]
	movs r0, #0x80
	str r0, [r1, #0x64]
	str r0, [r1, #0x54]
	str r4, [r1, #0x58]
	str r0, [r1, #0x5c]
	movs r0, #0x10
	ldrb r2, [r1, #0xc]
	orrs r0, r2
	strb r0, [r1, #0xc]
	b _0800B9D2
	.align 2, 0
_0800B9C8: .4byte 0xFFFF9C00
_0800B9CC:
	adds r0, r5, #0
	bl sub_800C5D4
_0800B9D2:
	ldr r4, [r5, #0x70]
	ldr r0, [r4, #0x30]
	cmp r0, #0
	beq _0800B9DC
	b _0800BD3A
_0800B9DC:
	ldr r0, [r4, #0x34]
	cmp r0, #0
	beq _0800B9E4
	b _0800BD3A
_0800B9E4:
	ldr r1, [r4, #0x18]
	movs r3, #0x28
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r1, [r1, #0x2c]
	bl sub_803AD7C
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0800B9FA
	b _0800BD3A
_0800B9FA:
	ldr r0, _0800BA0C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x13
	bl PlaySfx
	b _0800BD3A
	.align 2, 0
_0800BA0C: .4byte gUnknown_030012BC
_0800BA10:
	ldr r2, [r5, #0x70]
	ldr r1, [r2, #4]
	ldr r3, _0800BA7C @ =gUnknown_03001308
	ldr r0, [r3]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x14]
	lsls r0, r0, #8
	ldr r4, _0800BA80 @ =0xFFFFE200
	adds r0, r0, r4
	cmp r1, r0
	ble _0800BA8C
	movs r0, #0x7f
	ldrb r6, [r2, #0xc]
	ands r0, r6
	strb r0, [r2, #0xc]
	ldr r4, [r5, #0x70]
	ldr r1, [r4, #4]
	ldr r0, [r3]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x14]
	lsls r0, r0, #8
	movs r2, #0xf0
	lsls r2, r2, #5
	adds r0, r0, r2
	cmp r1, r0
	bgt _0800BA46
	b _0800BD3A
_0800BA46:
	movs r0, #1
	ldrb r3, [r4, #0xc]
	orrs r0, r3
	strb r0, [r4, #0xc]
	ldr r0, _0800BA84 @ =0x0000FFFF
	ldrh r6, [r4, #8]
	cmp r6, r0
	bne _0800BA58
	b _0800BD3A
_0800BA58:
	ldrh r4, [r4, #8]
	ldr r0, _0800BA88 @ =gUnknown_030012B4
	ldr r2, [r0]
	adds r0, r4, #0
	asrs r0, r0, #5
	lsls r1, r0, #2
	movs r3, #0x84
	lsls r3, r3, #1
	adds r2, r2, r3
	adds r2, r2, r1
	lsls r0, r0, #5
	subs r0, r4, r0
	movs r1, #1
	lsls r1, r0
	ldr r0, [r2]
	orrs r0, r1
	str r0, [r2]
	b _0800BD3A
	.align 2, 0
_0800BA7C: .4byte gUnknown_03001308
_0800BA80: .4byte 0xFFFFE200
_0800BA84: .4byte 0x0000FFFF
_0800BA88: .4byte gUnknown_030012B4
_0800BA8C:
	adds r0, r2, #0
	adds r0, #0x68
	ldrb r0, [r0]
	cmp r0, #8
	bne _0800BAA2
	movs r0, #0
	str r0, [r2, #0x64]
	str r0, [r2, #0x54]
	str r0, [r2, #0x58]
	str r0, [r2, #0x5c]
	b _0800BD3A
_0800BAA2:
	movs r0, #0x80
	lsls r0, r0, #3
	movs r1, #0
	str r0, [r2, #0x64]
	str r0, [r2, #0x54]
	str r1, [r2, #0x58]
	str r0, [r2, #0x5c]
	b _0800BD3A
_0800BAB2:
	ldr r0, [r5, #0x70]
	ldr r1, [r0]
	asrs r1, r1, #8
	ldr r2, [r0, #4]
	asrs r2, r2, #8
	ldr r0, _0800BB44 @ =gUnknown_030012D8
	ldr r3, [r0]
	ldr r0, [r3]
	asrs r0, r0, #8
	subs r1, r1, r0
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	ldr r0, [r3, #4]
	asrs r0, r0, #8
	subs r2, r2, r0
	asrs r0, r2, #0x1f
	eors r2, r0
	subs r2, r2, r0
	cmp r2, r1
	bge _0800BADE
	adds r2, r1, #0
_0800BADE:
	cmp r2, #0x20
	bge _0800BAE4
	movs r2, #0x20
_0800BAE4:
	cmp r2, #0xa0
	ble _0800BAEA
	movs r2, #0xa0
_0800BAEA:
	adds r3, r2, #0
	subs r3, #0x20
	lsls r3, r3, #1
	movs r4, #0x80
	lsls r4, r4, #1
	mov r8, r4
	subs r3, r4, r3
	ldr r7, _0800BB48 @ =gUnknown_030012BC
	ldr r0, [r7]
	mov r2, sp
	movs r1, #0
	strb r1, [r2]
	movs r1, #0x2b
	movs r2, #8
	bl sub_80019F8
	ldr r1, [r5, #0x70]
	adds r0, r1, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800BB4C
	ldr r6, [r5, #0x68]
	cmp r6, #3
	bne _0800BB4C
	movs r0, #0
	str r0, [sp]
	str r1, [sp, #4]
	movs r0, #0x1d
	movs r1, #0
	movs r2, #0
	movs r3, #0x2b
	bl sub_800C9C8
	adds r4, r5, #0
	adds r4, #0x88
	str r0, [r4]
	strb r6, [r0, #0xa]
	ldr r0, [r7]
	movs r1, #0x12
	mov r2, r8
	bl PlaySfx
	b _0800BB98
	.align 2, 0
_0800BB44: .4byte gUnknown_030012D8
_0800BB48: .4byte gUnknown_030012BC
_0800BB4C:
	ldr r0, [r5, #0x70]
	adds r0, #0x38
	ldrb r0, [r0]
	adds r4, r5, #0
	adds r4, #0x88
	cmp r0, #0
	beq _0800BB98
	ldr r0, [r5, #0x68]
	cmp r0, #5
	bne _0800BB98
	ldr r1, [r4]
	movs r0, #1
	ldrb r6, [r1, #0xc]
	orrs r0, r6
	strb r0, [r1, #0xc]
	ldr r0, _0800BBDC @ =0x0000FFFF
	ldrh r2, [r1, #8]
	cmp r2, r0
	beq _0800BB94
	ldrh r3, [r1, #8]
	ldr r0, _0800BBE0 @ =gUnknown_030012B4
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
_0800BB94:
	movs r0, #0
	str r0, [r4]
_0800BB98:
	adds r0, r5, #0
	bl sub_800C074
	adds r0, r5, #0
	bl sub_800C40C
	ldr r0, [r5, #0x68]
	cmp r0, #1
	bne _0800BBE4
	ldr r0, [r5, #0x70]
	adds r3, r0, #0
	adds r3, #0x28
	ldrb r2, [r3]
	lsls r0, r2, #0x1b
	movs r1, #0
	cmp r0, #0
	blt _0800BBBC
	movs r1, #1
_0800BBBC:
	lsls r1, r1, #4
	movs r0, #0x11
	rsbs r0, r0, #0
	ands r0, r2
	orrs r0, r1
	strb r0, [r3]
	adds r0, r5, #0
	movs r1, #0
	bl sub_800C8CC
	adds r0, r5, #0
	movs r1, #1
	bl sub_800C8BC
	b _0800BC30
	.align 2, 0
_0800BBDC: .4byte 0x0000FFFF
_0800BBE0: .4byte gUnknown_030012B4
_0800BBE4:
	cmp r0, #6
	bne _0800BC30
	ldr r0, [r5, #0x70]
	adds r3, r0, #0
	adds r3, #0x28
	ldrb r2, [r3]
	lsls r0, r2, #0x1b
	movs r1, #0
	cmp r0, #0
	blt _0800BBFA
	movs r1, #1
_0800BBFA:
	lsls r1, r1, #4
	movs r0, #0x11
	rsbs r0, r0, #0
	ands r0, r2
	orrs r0, r1
	strb r0, [r3]
	adds r0, r5, #0
	movs r1, #4
	bl sub_800C8CC
	ldr r1, [r5, #0x70]
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
	adds r0, r5, #0
	movs r1, #1
	bl sub_800C8BC
_0800BC30:
	ldr r4, [r4]
	cmp r4, #0
	bne _0800BC38
	b _0800BD3A
_0800BC38:
	ldr r0, [r5, #0x70]
	ldr r0, [r0]
	str r0, [r4]
	b _0800BD3A
_0800BC40:
	adds r0, r5, #0
	bl sub_800C5D4
	b _0800BD3A
_0800BC48:
	adds r0, r5, #0
	bl sub_800C074
	b _0800BD3A
_0800BC50:
	adds r0, r5, #0
	bl sub_800C074
_0800BC56:
	adds r0, r5, #0
	bl sub_800C40C
	b _0800BD3A
_0800BC5E:
	adds r0, r5, #0
	bl sub_800C40C
	adds r0, r5, #0
	bl sub_800C97C
	b _0800BD3A
_0800BC6C:
	adds r0, r5, #0
	bl sub_800C940
	b _0800BD3A
_0800BC74:
	adds r0, r5, #0
	bl sub_800C314
	b _0800BD3A
_0800BC7C:
	adds r0, r5, #0
	bl sub_800C244
	b _0800BD3A
_0800BC84:
	ldr r2, _0800BCC8 @ =gUnknown_030012A4
	ldr r0, [r2]
	cmp r0, #0
	bne _0800BC98
	ldr r0, [r5, #0x70]
	ldr r1, [r0]
	ldr r0, _0800BCCC @ =gUnknown_030012A0
	str r1, [r0]
	movs r0, #1
	str r0, [r2]
_0800BC98:
	ldr r2, _0800BCD0 @ =gUnknown_030012AC
	ldr r0, [r2]
	cmp r0, #0
	bne _0800BCAC
	ldr r0, [r5, #0x70]
	ldr r1, [r0, #4]
	ldr r0, _0800BCD4 @ =gUnknown_030012A8
	str r1, [r0]
	movs r0, #1
	str r0, [r2]
_0800BCAC:
	adds r0, r5, #0
	bl sub_800C18C
	adds r0, r5, #0
	bl sub_800C8F8
	ldr r2, [r5, #0x70]
	ldr r1, [r2]
	ldr r0, _0800BCCC @ =gUnknown_030012A0
	str r1, [r0]
	ldr r1, [r2, #4]
	ldr r0, _0800BCD4 @ =gUnknown_030012A8
	str r1, [r0]
	b _0800BD3A
	.align 2, 0
_0800BCC8: .4byte gUnknown_030012A4
_0800BCCC: .4byte gUnknown_030012A0
_0800BCD0: .4byte gUnknown_030012AC
_0800BCD4: .4byte gUnknown_030012A8
_0800BCD8:
	adds r0, r5, #0
	bl sub_800C18C
	adds r0, r5, #0
	bl sub_800C1E8
	ldr r0, [r5, #0x70]
	ldrb r0, [r0, #0xc]
	lsrs r0, r0, #3
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _0800BD3A
	ldr r0, [r5, #0x6c]
	cmp r0, #6
	bne _0800BD3A
	ldr r1, [r5, #0xc]
	movs r2, #0x10
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r4, [r1, #0x14]
	movs r1, #0
	movs r2, #1
	movs r3, #0
	bl sub_803AD88
	ldr r0, _0800BD1C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	b _0800BD3A
	.align 2, 0
_0800BD1C: .4byte gUnknown_030012BC
_0800BD20:
	adds r0, r5, #0
	bl sub_800C074
_0800BD26:
	adds r0, r5, #0
	bl sub_800C1E8
	b _0800BD3A
_0800BD2E:
	adds r0, r5, #0
	bl sub_800C40C
	adds r0, r5, #0
	bl sub_800BFA8
_0800BD3A:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_800BD48
sub_800BD48: @ 0x0800BD48
	push {r4, r5, r6, lr}
	sub sp, #8
	adds r5, r0, #0
	adds r6, r2, #0
	ldr r0, _0800BDB8 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x88
	ldrb r4, [r0]
	cmp r4, #1
	bne _0800BDCC
	ldr r1, [r5, #0x70]
	movs r0, #1
	ldrb r2, [r1, #0xc]
	orrs r0, r2
	strb r0, [r1, #0xc]
	ldr r0, _0800BDBC @ =0x0000FFFF
	ldrh r3, [r1, #8]
	cmp r3, r0
	beq _0800BD8E
	ldrh r3, [r1, #8]
	ldr r0, _0800BDC0 @ =gUnknown_030012B4
	ldr r1, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r2, r0, #2
	movs r6, #0x84
	lsls r6, r6, #1
	adds r1, r1, r6
	adds r1, r1, r2
	lsls r0, r0, #5
	subs r0, r3, r0
	lsls r4, r0
	ldr r0, [r1]
	orrs r0, r4
	str r0, [r1]
_0800BD8E:
	ldr r0, [r5, #0x70]
	ldr r3, [r0]
	asrs r3, r3, #8
	ldr r1, [r0, #4]
	asrs r1, r1, #8
	ldr r0, _0800BDC4 @ =gUnknown_030012E4
	ldr r0, [r0]
	str r1, [sp]
	movs r1, #0
	str r1, [sp, #4]
	movs r1, #0x28
	movs r2, #2
	bl sub_8025BAC
	ldr r0, _0800BDC8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x5a
	movs r2, #0x80
	bl PlaySfx
	b _0800BF94
	.align 2, 0
_0800BDB8: .4byte gUnknown_030012D8
_0800BDBC: .4byte 0x0000FFFF
_0800BDC0: .4byte gUnknown_030012B4
_0800BDC4: .4byte gUnknown_030012E4
_0800BDC8: .4byte gUnknown_030012BC
_0800BDCC:
	adds r0, r5, #0
	adds r0, #0x88
	ldr r1, [r0]
	cmp r1, #0
	beq _0800BE08
	movs r0, #1
	ldrb r2, [r1, #0xc]
	orrs r0, r2
	strb r0, [r1, #0xc]
	ldr r0, _0800BE1C @ =0x0000FFFF
	ldrh r3, [r1, #8]
	cmp r3, r0
	beq _0800BE08
	ldrh r3, [r1, #8]
	ldr r0, _0800BE20 @ =gUnknown_030012B4
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
_0800BE08:
	subs r0, r6, #1
	cmp r0, #0x15
	bls _0800BE10
	b _0800BF94
_0800BE10:
	lsls r0, r0, #2
	ldr r1, _0800BE24 @ =_0800BE28
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800BE1C: .4byte 0x0000FFFF
_0800BE20: .4byte gUnknown_030012B4
_0800BE24: .4byte _0800BE28
_0800BE28: @ jump table
	.4byte _0800BF2C @ case 0
	.4byte _0800BF94 @ case 1
	.4byte _0800BF94 @ case 2
	.4byte _0800BF94 @ case 3
	.4byte _0800BF94 @ case 4
	.4byte _0800BF94 @ case 5
	.4byte _0800BF94 @ case 6
	.4byte _0800BF94 @ case 7
	.4byte _0800BF94 @ case 8
	.4byte _0800BF94 @ case 9
	.4byte _0800BF94 @ case 10
	.4byte _0800BF94 @ case 11
	.4byte _0800BF94 @ case 12
	.4byte _0800BF94 @ case 13
	.4byte _0800BF94 @ case 14
	.4byte _0800BF94 @ case 15
	.4byte _0800BF94 @ case 16
	.4byte _0800BF94 @ case 17
	.4byte _0800BE80 @ case 18
	.4byte _0800BE80 @ case 19
	.4byte _0800BF2C @ case 20
	.4byte _0800BF2C @ case 21
_0800BE80:
	movs r0, #0x10
	bl sub_8026EDC
	bl sub_800CBD4
	ldr r1, [r5, #0x70]
	str r0, [r1, #0x44]
	ldr r3, [r0, #0xc]
	movs r6, #0x18
	ldrsh r2, [r3, r6]
	adds r0, r0, r2
	ldr r2, [r3, #0x1c]
	bl sub_803AD80
	ldr r1, [r5, #0x70]
	movs r0, #0x7f
	ldrb r2, [r1, #0xc]
	ands r0, r2
	strb r0, [r1, #0xc]
	ldr r3, [r5, #0x70]
	ldr r1, [r3]
	ldr r0, _0800BEC0 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0]
	cmp r1, r0
	ble _0800BEC4
	movs r0, #0x80
	lsls r0, r0, #5
	movs r1, #0
	movs r2, #0xc0
	lsls r2, r2, #5
	b _0800BECA
	.align 2, 0
_0800BEC0: .4byte gUnknown_030012D8
_0800BEC4:
	ldr r0, _0800BF1C @ =0xFFFFF000
	movs r1, #0
	ldr r2, _0800BF20 @ =0xFFFFE800
_0800BECA:
	str r0, [r3, #0x60]
	str r0, [r3, #0x48]
	str r1, [r3, #0x4c]
	str r2, [r3, #0x50]
	movs r0, #3
	bl sub_8000E1C
	lsls r0, r0, #0x10
	lsrs r0, r0, #7
	ldr r3, _0800BF24 @ =0xFFFFFE00
	adds r0, r0, r3
	ldr r1, [r5, #0x70]
	movs r2, #0
	str r0, [r1, #0x64]
	str r0, [r1, #0x54]
	str r2, [r1, #0x58]
	str r0, [r1, #0x5c]
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r4, [r1, #0xc]
	ands r0, r4
	strb r0, [r1, #0xc]
	ldr r0, _0800BF28 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #5
	movs r2, #0x80
	bl PlaySfx
	cmp r5, #0
	beq _0800BF94
	ldr r1, [r5, #0xc]
	adds r1, #0x48
	movs r6, #0
	ldrsh r0, [r1, r6]
	adds r0, r5, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
	b _0800BF94
	.align 2, 0
_0800BF1C: .4byte 0xFFFFF000
_0800BF20: .4byte 0xFFFFE800
_0800BF24: .4byte 0xFFFFFE00
_0800BF28: .4byte gUnknown_030012BC
_0800BF2C:
	ldr r0, [r5, #0x70]
	ldr r3, [r0]
	asrs r3, r3, #8
	ldr r1, [r0, #4]
	asrs r1, r1, #8
	ldr r0, _0800BF9C @ =gUnknown_030012E4
	ldr r0, [r0]
	str r1, [sp]
	movs r1, #0
	str r1, [sp, #4]
	movs r1, #0x29
	movs r2, #2
	bl sub_8025BAC
	movs r1, #5
	rsbs r1, r1, #0
	ldrb r2, [r0, #0xc]
	ands r1, r2
	strb r1, [r0, #0xc]
	adds r0, #0x28
	movs r2, #1
	movs r1, #4
	rsbs r1, r1, #0
	ldrb r3, [r0]
	ands r1, r3
	orrs r1, r2
	strb r1, [r0]
	ldr r1, [r5, #0x70]
	ldrb r4, [r1, #0xc]
	orrs r2, r4
	strb r2, [r1, #0xc]
	ldr r0, _0800BFA0 @ =0x0000FFFF
	ldrh r6, [r1, #8]
	cmp r6, r0
	beq _0800BF94
	ldrh r3, [r1, #8]
	ldr r0, _0800BFA4 @ =gUnknown_030012B4
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
_0800BF94:
	add sp, #8
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0800BF9C: .4byte gUnknown_030012E4
_0800BFA0: .4byte 0x0000FFFF
_0800BFA4: .4byte gUnknown_030012B4

	thumb_func_start sub_800BFA8
sub_800BFA8: @ 0x0800BFA8
	push {r4, lr}
	sub sp, #8
	adds r4, r0, #0
	ldr r0, _0800BFD0 @ =gUnknown_0300082C
	ldr r0, [r0]
	ldr r1, [r4, #0x48]
	adds r0, r0, r1
	ldr r2, [r4, #0x4c]
	subs r0, r0, r2
	bl sub_803AE4C
	cmp r0, #0
	bne _0800BFE8
	ldr r0, [r4, #0x68]
	cmp r0, #0
	beq _0800BFD4
	cmp r0, #4
	beq _0800BFDE
	b _0800C06C
	.align 2, 0
_0800BFD0: .4byte gUnknown_0300082C
_0800BFD4:
	adds r0, r4, #0
	movs r1, #2
	bl sub_800C8CC
	b _0800C06C
_0800BFDE:
	adds r0, r4, #0
	movs r1, #7
	bl sub_800C8CC
	b _0800C06C
_0800BFE8:
	ldr r1, [r4, #0x70]
	adds r0, r1, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800C014
	ldr r0, [r4, #0x68]
	cmp r0, #2
	beq _0800C000
	cmp r0, #7
	beq _0800C00A
	b _0800C06C
_0800C000:
	adds r0, r4, #0
	movs r1, #0
	bl sub_800C8CC
	b _0800C06C
_0800C00A:
	adds r0, r4, #0
	movs r1, #4
	bl sub_800C8CC
	b _0800C06C
_0800C014:
	movs r2, #0
	ldr r0, [r4, #0x68]
	cmp r0, #2
	beq _0800C022
	cmp r0, #7
	beq _0800C042
	b _0800C064
_0800C022:
	ldr r0, [r1, #0x30]
	cmp r0, #0xa
	bne _0800C064
	ldr r0, [r1, #0x34]
	cmp r0, #0
	bne _0800C064
	movs r3, #0xa
	rsbs r3, r3, #0
	movs r0, #0x80
	lsls r0, r0, #3
	str r0, [sp]
	str r1, [sp, #4]
	movs r0, #0xc
	movs r1, #6
	movs r2, #0
	b _0800C05E
_0800C042:
	ldr r0, [r1, #0x30]
	cmp r0, #8
	bne _0800C064
	ldr r0, [r1, #0x34]
	cmp r0, #0
	bne _0800C064
	movs r0, #0x80
	lsls r0, r0, #3
	str r0, [sp]
	str r1, [sp, #4]
	movs r0, #0xc
	movs r1, #6
	movs r2, #0
	movs r3, #8
_0800C05E:
	bl sub_800C9C8
	adds r2, r0, #0
_0800C064:
	cmp r2, #0
	beq _0800C06C
	movs r0, #8
	strb r0, [r2, #0xa]
_0800C06C:
	add sp, #8
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_800C074
sub_800C074: @ 0x0800C074
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x68]
	cmp r0, #1
	beq _0800C0BA
	cmp r0, #1
	bgt _0800C088
	cmp r0, #0
	beq _0800C092
	b _0800C186
_0800C088:
	cmp r0, #4
	beq _0800C11A
	cmp r0, #6
	beq _0800C14E
	b _0800C186
_0800C092:
	ldr r2, [r4, #0x70]
	adds r0, r2, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r3, r0, #0x1b
	cmp r3, #0
	bge _0800C0A8
	ldr r1, [r2]
	ldr r0, [r4, #0x10]
	cmp r1, r0
	blt _0800C0B4
_0800C0A8:
	cmp r3, #0
	blt _0800C186
	ldr r1, [r2]
	ldr r0, [r4, #0x14]
	cmp r1, r0
	ble _0800C186
_0800C0B4:
	adds r0, r4, #0
	movs r1, #1
	b _0800C140
_0800C0BA:
	ldr r1, [r4, #0x70]
	adds r0, r1, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800C186
	adds r3, r1, #0
	adds r3, #0x28
	ldrb r2, [r3]
	lsls r0, r2, #0x1b
	movs r1, #0
	cmp r0, #0
	blt _0800C0D6
	movs r1, #1
_0800C0D6:
	lsls r0, r1, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	adds r0, r4, #0
	movs r1, #0
	bl sub_800C8CC
	adds r0, r4, #0
	movs r1, #1
	bl sub_800C8BC
	ldr r0, [r4, #0x6c]
	cmp r0, #0xf
	bne _0800C186
	ldr r3, [r4, #0x70]
	movs r4, #8
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
	blt _0800C116
	subs r4, r0, #1
_0800C116:
	str r4, [r3, #0x30]
	b _0800C186
_0800C11A:
	ldr r2, [r4, #0x70]
	adds r0, r2, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r3, r0, #0x1b
	cmp r3, #0
	bge _0800C130
	ldr r1, [r2]
	ldr r0, [r4, #0x10]
	cmp r1, r0
	blt _0800C13C
_0800C130:
	cmp r3, #0
	blt _0800C186
	ldr r1, [r2]
	ldr r0, [r4, #0x14]
	cmp r1, r0
	ble _0800C186
_0800C13C:
	adds r0, r4, #0
	movs r1, #6
_0800C140:
	bl sub_800C8CC
	adds r0, r4, #0
	movs r1, #0
	bl sub_800C8BC
	b _0800C186
_0800C14E:
	ldr r1, [r4, #0x70]
	adds r0, r1, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800C186
	adds r3, r1, #0
	adds r3, #0x28
	ldrb r2, [r3]
	lsls r0, r2, #0x1b
	movs r1, #0
	cmp r0, #0
	blt _0800C16A
	movs r1, #1
_0800C16A:
	lsls r1, r1, #4
	movs r0, #0x11
	rsbs r0, r0, #0
	ands r0, r2
	orrs r0, r1
	strb r0, [r3]
	adds r0, r4, #0
	movs r1, #4
	bl sub_800C8CC
	adds r0, r4, #0
	movs r1, #1
	bl sub_800C8BC
_0800C186:
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_800C18C
sub_800C18C: @ 0x0800C18C
	push {r4, lr}
	adds r3, r0, #0
	ldr r2, [r3, #0x70]
	ldr r4, [r2]
	ldr r0, _0800C1AC @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0]
	subs r1, r4, r0
	cmp r1, #0x14
	ble _0800C1BE
	ldr r0, [r3, #0x10]
	cmp r4, r0
	bge _0800C1B0
	movs r1, #0
	movs r0, #0x10
	b _0800C1DA
	.align 2, 0
_0800C1AC: .4byte gUnknown_030012D8
_0800C1B0:
	ldr r0, [r3, #0x58]
	rsbs r0, r0, #0
	ldr r1, [r3, #0x5c]
	str r0, [r2, #0x48]
	str r1, [r2, #0x4c]
	str r0, [r2, #0x50]
	b _0800C1E0
_0800C1BE:
	movs r0, #0x14
	rsbs r0, r0, #0
	cmp r1, r0
	bge _0800C1D6
	ldr r0, [r3, #0x14]
	cmp r4, r0
	ble _0800C1D2
	movs r1, #0
	movs r0, #0x10
	b _0800C1DA
_0800C1D2:
	ldr r1, [r3, #0x58]
	b _0800C1D8
_0800C1D6:
	movs r1, #0
_0800C1D8:
	ldr r0, [r3, #0x5c]
_0800C1DA:
	str r1, [r2, #0x48]
	str r0, [r2, #0x4c]
	str r1, [r2, #0x50]
_0800C1E0:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_800C1E8
sub_800C1E8: @ 0x0800C1E8
	push {r4, lr}
	adds r3, r0, #0
	ldr r2, [r3, #0x70]
	ldr r4, [r2, #4]
	ldr r0, _0800C208 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0, #4]
	subs r1, r4, r0
	cmp r1, #0x14
	ble _0800C21A
	ldr r0, [r3, #0x1c]
	cmp r4, r0
	bge _0800C20C
	movs r1, #0
	movs r0, #0x10
	b _0800C236
	.align 2, 0
_0800C208: .4byte gUnknown_030012D8
_0800C20C:
	ldr r0, [r3, #0x58]
	rsbs r0, r0, #0
	ldr r1, [r3, #0x5c]
	str r0, [r2, #0x54]
	str r1, [r2, #0x58]
	str r0, [r2, #0x5c]
	b _0800C23C
_0800C21A:
	movs r0, #0x14
	rsbs r0, r0, #0
	cmp r1, r0
	bge _0800C232
	ldr r0, [r3, #0x18]
	cmp r4, r0
	ble _0800C22E
	movs r1, #0
	movs r0, #0x10
	b _0800C236
_0800C22E:
	ldr r1, [r3, #0x58]
	b _0800C234
_0800C232:
	movs r1, #0
_0800C234:
	ldr r0, [r3, #0x5c]
_0800C236:
	str r1, [r2, #0x54]
	str r0, [r2, #0x58]
	str r1, [r2, #0x5c]
_0800C23C:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_800C244
sub_800C244: @ 0x0800C244
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x70]
	ldr r1, [r0, #4]
	ldr r0, [r4, #0x64]
	cmp r1, r0
	blt _0800C30A
	adds r0, r4, #0
	movs r1, #0
	bl sub_800C8BC
	adds r0, r4, #0
	movs r1, #0
	bl sub_800C8AC
	ldr r2, [r4, #0x70]
	ldr r1, [r4, #0x64]
	str r1, [r2, #4]
	adds r0, r2, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800C2AE
	ldr r0, [r4, #0x68]
	cmp r0, #0
	beq _0800C27E
	cmp r0, #1
	beq _0800C288
	b _0800C30A
_0800C27E:
	adds r0, r4, #0
	movs r1, #1
	bl sub_800C8CC
	b _0800C30A
_0800C288:
	adds r3, r2, #0
	adds r3, #0x28
	ldrb r2, [r3]
	lsls r0, r2, #0x1b
	movs r1, #0
	cmp r0, #0
	blt _0800C298
	movs r1, #1
_0800C298:
	lsls r1, r1, #4
	movs r0, #0x11
	rsbs r0, r0, #0
	ands r0, r2
	orrs r0, r1
	strb r0, [r3]
	adds r0, r4, #0
	movs r1, #0
	bl sub_800C8CC
	b _0800C30A
_0800C2AE:
	ldr r0, [r2, #0x30]
	cmp r0, #8
	bne _0800C30A
	ldr r0, [r2, #0x34]
	cmp r0, #0
	bne _0800C30A
	ldr r0, [r4, #0x68]
	cmp r0, #0
	beq _0800C2C6
	cmp r0, #1
	beq _0800C2EC
	b _0800C30A
_0800C2C6:
	adds r0, r4, #0
	movs r1, #3
	bl sub_800C8BC
	adds r0, r4, #0
	movs r1, #3
	bl sub_800C8AC
	ldr r0, _0800C2E8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x14
	bl PlaySfx
	b _0800C30A
	.align 2, 0
_0800C2E8: .4byte gUnknown_030012BC
_0800C2EC:
	adds r0, r4, #0
	movs r1, #0
	bl sub_800C8BC
	adds r0, r4, #0
	movs r1, #3
	bl sub_800C8AC
	ldr r0, _0800C310 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x14
	bl PlaySfx
_0800C30A:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0800C310: .4byte gUnknown_030012BC

	thumb_func_start sub_800C314
sub_800C314: @ 0x0800C314
	push {r4, lr}
	adds r4, r0, #0
	ldr r2, [r4, #0x70]
	adds r0, r2, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800C406
	ldr r0, [r4, #0x68]
	cmp r0, #1
	beq _0800C388
	cmp r0, #1
	bgt _0800C334
	cmp r0, #0
	beq _0800C33A
	b _0800C406
_0800C334:
	cmp r0, #6
	beq _0800C3BA
	b _0800C406
_0800C33A:
	adds r0, r4, #0
	adds r0, #0x80
	ldr r0, [r0]
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _0800C36E
	adds r3, r2, #0
	adds r3, #0x28
	ldrb r2, [r3]
	lsls r0, r2, #0x1b
	movs r1, #0
	cmp r0, #0
	blt _0800C358
	movs r1, #1
_0800C358:
	lsls r1, r1, #4
	movs r0, #0x11
	rsbs r0, r0, #0
	ands r0, r2
	orrs r0, r1
	strb r0, [r3]
	adds r0, r4, #0
	movs r1, #1
	bl sub_800C8CC
	b _0800C376
_0800C36E:
	adds r0, r4, #0
	movs r1, #6
	bl sub_800C8CC
_0800C376:
	adds r0, r4, #0
	movs r1, #0
	bl sub_800C8BC
	adds r0, r4, #0
	movs r1, #0
	bl sub_800C8AC
	b _0800C406
_0800C388:
	adds r3, r4, #0
	adds r3, #0x80
	ldr r2, [r3]
	adds r1, r2, #1
	adds r0, r1, #0
	cmp r1, #0
	bge _0800C398
	adds r0, r2, #4
_0800C398:
	asrs r0, r0, #2
	lsls r0, r0, #2
	subs r0, r1, r0
	str r0, [r3]
	adds r0, r4, #0
	movs r1, #2
	bl sub_800C8BC
	adds r0, r4, #0
	movs r1, #2
	bl sub_800C8AC
	adds r0, r4, #0
	movs r1, #0
	bl sub_800C8CC
	b _0800C406
_0800C3BA:
	adds r3, r2, #0
	adds r3, #0x28
	ldrb r2, [r3]
	lsls r0, r2, #0x1a
	movs r1, #0
	cmp r0, #0
	blt _0800C3CA
	movs r1, #1
_0800C3CA:
	lsls r1, r1, #5
	movs r0, #0x21
	rsbs r0, r0, #0
	ands r0, r2
	orrs r0, r1
	strb r0, [r3]
	adds r3, r4, #0
	adds r3, #0x80
	ldr r2, [r3]
	adds r1, r2, #1
	adds r0, r1, #0
	cmp r1, #0
	bge _0800C3E6
	adds r0, r2, #4
_0800C3E6:
	asrs r0, r0, #2
	lsls r0, r0, #2
	subs r0, r1, r0
	str r0, [r3]
	adds r0, r4, #0
	movs r1, #2
	bl sub_800C8BC
	adds r0, r4, #0
	movs r1, #2
	bl sub_800C8AC
	adds r0, r4, #0
	movs r1, #0
	bl sub_800C8CC
_0800C406:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_800C40C
sub_800C40C: @ 0x0800C40C
	push {r4, lr}
	sub sp, #0xc
	adds r4, r0, #0
	ldr r0, [r4, #0x68]
	cmp r0, #3
	beq _0800C4E2
	cmp r0, #3
	bgt _0800C422
	cmp r0, #0
	beq _0800C42E
	b _0800C5C6
_0800C422:
	cmp r0, #4
	beq _0800C49C
	cmp r0, #5
	bne _0800C42C
	b _0800C584
_0800C42C:
	b _0800C5C6
_0800C42E:
	ldr r1, [r4, #0x34]
	cmp r1, #0
	bgt _0800C436
	b _0800C5C6
_0800C436:
	ldr r0, _0800C468 @ =gUnknown_0300082C
	ldr r3, [r4, #0x30]
	adds r1, r3, r1
	lsls r2, r1, #1
	ldr r0, [r0]
	adds r0, r0, r2
	ldr r2, [r4, #0x38]
	subs r0, r0, r2
	subs r0, r0, r3
	bl sub_803AE4C
	cmp r0, #0
	beq _0800C452
	b _0800C5C6
_0800C452:
	adds r0, r4, #0
	adds r0, #0x84
	ldr r0, [r0]
	ldr r0, [r0, #0xc]
	cmp r0, #8
	beq _0800C46C
	adds r0, r4, #0
	movs r1, #3
	bl sub_800C8CC
	b _0800C474
	.align 2, 0
_0800C468: .4byte gUnknown_0300082C
_0800C46C:
	adds r0, r4, #0
	movs r1, #4
	bl sub_800C8CC
_0800C474:
	ldr r0, [r4, #0x6c]
	cmp r0, #0xf
	bne _0800C484
	adds r0, r4, #0
	movs r1, #0
	bl sub_800C8BC
	b _0800C5C6
_0800C484:
	cmp r0, #0x12
	beq _0800C48E
	cmp r0, #0x1a
	beq _0800C48E
	b _0800C5C6
_0800C48E:
	ldr r1, [r4, #0x70]
	movs r0, #9
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xd]
	ands r0, r2
	strb r0, [r1, #0xd]
	b _0800C5C6
_0800C49C:
	ldr r3, [r4, #0x30]
	cmp r3, #0
	bgt _0800C4A4
	b _0800C5C6
_0800C4A4:
	ldr r0, _0800C4D4 @ =gUnknown_0300082C
	ldr r0, [r0]
	adds r0, r0, r3
	ldr r1, [r4, #0x34]
	adds r0, r0, r1
	ldr r2, [r4, #0x38]
	subs r0, r0, r2
	adds r1, r3, r1
	bl sub_803AE4C
	cmp r0, #0
	beq _0800C4BE
	b _0800C5C6
_0800C4BE:
	adds r0, r4, #0
	adds r0, #0x84
	ldr r0, [r0]
	ldr r0, [r0, #0x14]
	cmp r0, #8
	beq _0800C4D8
	adds r0, r4, #0
	movs r1, #5
	bl sub_800C8CC
	b _0800C5C6
	.align 2, 0
_0800C4D4: .4byte gUnknown_0300082C
_0800C4D8:
	adds r0, r4, #0
	movs r1, #0
	bl sub_800C8CC
	b _0800C5C6
_0800C4E2:
	ldr r0, [r4, #0x70]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800C52E
	adds r0, r4, #0
	movs r1, #4
	bl sub_800C8CC
	ldr r0, [r4, #0x6c]
	cmp r0, #0x12
	beq _0800C4FE
	cmp r0, #0x1a
	bne _0800C51C
_0800C4FE:
	ldr r1, [r4, #0x70]
	movs r0, #8
	ldrb r2, [r1, #0xd]
	orrs r0, r2
	strb r0, [r1, #0xd]
	ldr r0, _0800C518 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x26
	bl PlaySfx
	b _0800C52E
	.align 2, 0
_0800C518: .4byte gUnknown_030012BC
_0800C51C:
	cmp r0, #0xf
	bne _0800C52E
	ldr r0, _0800C57C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #9
	bl PlaySfx
_0800C52E:
	ldr r0, [r4, #0x6c]
	cmp r0, #0x17
	bne _0800C5C6
	ldr r1, [r4, #0x70]
	ldr r0, [r1, #0x30]
	cmp r0, #9
	bne _0800C5C6
	ldr r2, [r1, #0x34]
	cmp r2, #0
	bne _0800C5C6
	movs r4, #2
	ldr r0, _0800C580 @ =gUnknown_030012E4
	ldr r0, [r0]
	str r4, [sp]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r1, #0x17
	movs r2, #4
	movs r3, #0x2d
	rsbs r3, r3, #0
	bl sub_8025B0C
	movs r1, #4
	ldrb r2, [r0, #0xc]
	orrs r1, r2
	movs r2, #0x41
	rsbs r2, r2, #0
	ands r1, r2
	strb r1, [r0, #0xc]
	strb r4, [r0, #0xa]
	ldr r0, _0800C57C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x1e
	bl PlaySfx
	b _0800C5C6
	.align 2, 0
_0800C57C: .4byte gUnknown_030012BC
_0800C580: .4byte gUnknown_030012E4
_0800C584:
	ldr r0, [r4, #0x70]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800C5A4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800C8CC
	ldr r0, [r4, #0x6c]
	cmp r0, #0xf
	bne _0800C5C6
	adds r0, r4, #0
	movs r1, #1
	bl sub_800C8BC
_0800C5A4:
	ldr r0, [r4, #0x6c]
	cmp r0, #0xf
	bne _0800C5C6
	ldr r1, [r4, #0x70]
	ldr r0, [r1, #0x30]
	cmp r0, #8
	bne _0800C5C6
	ldr r0, [r1, #0x34]
	cmp r0, #0
	bne _0800C5C6
	ldr r0, _0800C5D0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x23
	bl PlaySfx
_0800C5C6:
	add sp, #0xc
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0800C5D0: .4byte gUnknown_030012BC

	thumb_func_start sub_800C5D4
sub_800C5D4: @ 0x0800C5D4
	push {r4, r5, r6, r7, lr}
	sub sp, #0x10
	adds r6, r0, #0
	ldr r0, [r6, #0x6c]
	cmp r0, #0xb
	bne _0800C5F4
	ldr r1, [r6, #0x70]
	ldr r0, [r1, #4]
	ldr r2, [r6, #0x64]
	cmp r0, r2
	bge _0800C5F4
	str r2, [r1, #4]
	adds r0, r6, #0
	movs r1, #0
	bl sub_800C8AC
_0800C5F4:
	ldr r7, [r6, #0x68]
	cmp r7, #0
	beq _0800C600
	cmp r7, #2
	beq _0800C68C
	b _0800C69E
_0800C600:
	ldr r0, [r6, #0x70]
	ldr r1, [r0]
	asrs r1, r1, #8
	ldr r2, [r0, #4]
	asrs r2, r2, #8
	ldr r5, [r6, #0x28]
	ldr r3, [r6, #0x20]
	subs r5, r5, r3
	ldr r4, [r6, #0x2c]
	ldr r0, [r6, #0x24]
	subs r4, r4, r0
	adds r1, r1, r3
	adds r2, r2, r0
	mov r0, sp
	bl sub_803AFE4
	mov r0, sp
	adds r1, r5, #0
	adds r2, r4, #0
	bl sub_803AFDC
	ldr r1, [r6, #0x70]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _0800C648
	ldr r0, [r1]
	asrs r0, r0, #8
	lsls r0, r0, #1
	ldr r1, [sp]
	ldr r2, [sp, #8]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp]
_0800C648:
	ldr r0, _0800C684 @ =gUnknown_030012D8
	ldr r0, [r0]
	mov r1, sp
	bl sub_800B37C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0800C69E
	adds r0, r6, #0
	movs r1, #2
	bl sub_800C8CC
	ldr r0, [r6, #0x6c]
	cmp r0, #0xb
	bne _0800C69E
	ldr r0, [r6, #0x70]
	movs r1, #0xc0
	lsls r1, r1, #2
	movs r2, #0x20
	str r1, [r0, #0x64]
	str r1, [r0, #0x54]
	str r2, [r0, #0x58]
	str r7, [r0, #0x5c]
	ldr r1, _0800C688 @ =0xFFFFFE00
	str r7, [r0, #0x60]
	str r7, [r0, #0x48]
	str r2, [r0, #0x4c]
	str r1, [r0, #0x50]
	b _0800C69E
	.align 2, 0
_0800C684: .4byte gUnknown_030012D8
_0800C688: .4byte 0xFFFFFE00
_0800C68C:
	ldr r0, [r6, #0x70]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800C69E
	adds r0, r6, #0
	movs r1, #0
	bl sub_800C8CC
_0800C69E:
	add sp, #0x10
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_800C6A8
sub_800C6A8: @ 0x0800C6A8
	push {r4, r5, lr}
	adds r5, r0, #0
	str r1, [r5, #0x74]
	subs r0, r1, #1
	cmp r0, #0x11
	bls _0800C6B6
	b _0800C84E
_0800C6B6:
	lsls r0, r0, #2
	ldr r1, _0800C6C0 @ =_0800C6C4
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800C6C0: .4byte _0800C6C4
_0800C6C4: @ jump table
	.4byte _0800C7D4 @ case 0
	.4byte _0800C742 @ case 1
	.4byte _0800C7D4 @ case 2
	.4byte _0800C76C @ case 3
	.4byte _0800C70C @ case 4
	.4byte _0800C734 @ case 5
	.4byte _0800C81C @ case 6
	.4byte _0800C7E2 @ case 7
	.4byte _0800C734 @ case 8
	.4byte _0800C734 @ case 9
	.4byte _0800C734 @ case 10
	.4byte _0800C84E @ case 11
	.4byte _0800C75E @ case 12
	.4byte _0800C76C @ case 13
	.4byte _0800C742 @ case 14
	.4byte _0800C76C @ case 15
	.4byte _0800C7D4 @ case 16
	.4byte _0800C75E @ case 17
_0800C70C:
	ldr r1, [r5, #0x70]
	ldr r0, _0800C730 @ =0xFFFFFE80
	movs r2, #0
	str r0, [r1, #0x60]
	str r0, [r1, #0x48]
	str r2, [r1, #0x4c]
	str r0, [r1, #0x50]
	movs r0, #0x80
	lsls r0, r0, #3
	str r0, [r1, #0x64]
	str r0, [r1, #0x54]
	str r2, [r1, #0x58]
	str r0, [r1, #0x5c]
	movs r0, #0x80
	ldrb r2, [r1, #0xc]
	orrs r0, r2
	strb r0, [r1, #0xc]
	b _0800C84E
	.align 2, 0
_0800C730: .4byte 0xFFFFFE80
_0800C734:
	movs r0, #0
	str r0, [r5, #0x68]
	ldr r3, [r5, #0xc]
	adds r3, #0x50
	movs r4, #0
	ldrsh r0, [r3, r4]
	b _0800C808
_0800C742:
	movs r0, #1
	str r0, [r5, #0x78]
	ldr r1, [r5, #0x70]
	adds r0, r5, #0
	movs r2, #1
	bl sub_800B838
	movs r0, #0
	str r0, [r5, #0x68]
	ldr r3, [r5, #0xc]
	adds r3, #0x50
	movs r1, #0
	ldrsh r0, [r3, r1]
	b _0800C808
_0800C75E:
	movs r0, #1
	str r0, [r5, #0x78]
	ldr r1, [r5, #0x70]
	adds r0, r5, #0
	movs r2, #1
	bl sub_800B838
_0800C76C:
	ldr r1, [r5, #0x38]
	ldr r0, [r5, #0x30]
	cmp r1, r0
	blt _0800C794
	movs r0, #4
	str r0, [r5, #0x68]
	ldr r3, [r5, #0xc]
	adds r3, #0x50
	movs r2, #0
	ldrsh r0, [r3, r2]
	adds r0, r5, r0
	ldr r1, [r5, #0x70]
	adds r2, r5, #0
	adds r2, #0x84
	ldr r2, [r2]
	ldr r2, [r2, #0x10]
	ldr r3, [r3, #4]
	bl sub_803AD84
	b _0800C7B8
_0800C794:
	movs r0, #0
	str r0, [r5, #0x68]
	ldr r3, [r5, #0xc]
	adds r3, #0x50
	movs r1, #0
	ldrsh r0, [r3, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x70]
	adds r2, r5, #0
	adds r2, #0x84
	ldr r2, [r2]
	ldr r2, [r2]
	ldr r3, [r3, #4]
	bl sub_803AD84
	ldr r0, [r5, #0x6c]
	cmp r0, #0x1b
	bne _0800C84E
_0800C7B8:
	ldr r1, [r5, #0x70]
	ldr r0, [r1, #0x20]
	adds r3, r1, #0
	adds r3, #0x2d
	ldr r2, [r0]
	ldrb r4, [r3]
	lsls r0, r4, #3
	subs r0, r0, r4
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #0x16]
	subs r0, #1
	str r0, [r1, #0x30]
	b _0800C84E
_0800C7D4:
	movs r0, #0
	str r0, [r5, #0x68]
	ldr r3, [r5, #0xc]
	adds r3, #0x50
	movs r1, #0
	ldrsh r0, [r3, r1]
	b _0800C808
_0800C7E2:
	movs r4, #3
	str r4, [r5, #0x78]
	ldr r1, [r5, #0x70]
	adds r0, r5, #0
	movs r2, #3
	bl sub_800B838
	str r4, [r5, #0x7c]
	ldr r1, [r5, #0x70]
	adds r0, r5, #0
	movs r2, #3
	bl sub_800B704
	movs r0, #0
	str r0, [r5, #0x68]
	ldr r3, [r5, #0xc]
	adds r3, #0x50
	movs r2, #0
	ldrsh r0, [r3, r2]
_0800C808:
	adds r0, r5, r0
	ldr r1, [r5, #0x70]
	adds r2, r5, #0
	adds r2, #0x84
	ldr r2, [r2]
	ldr r2, [r2]
	ldr r3, [r3, #4]
	bl sub_803AD84
	b _0800C84E
_0800C81C:
	movs r0, #2
	str r0, [r5, #0x78]
	ldr r1, [r5, #0x70]
	adds r0, r5, #0
	movs r2, #2
	bl sub_800B838
	movs r4, #0
	str r4, [r5, #0x68]
	ldr r3, [r5, #0xc]
	adds r3, #0x50
	movs r1, #0
	ldrsh r0, [r3, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x70]
	adds r2, r5, #0
	adds r2, #0x84
	ldr r2, [r2]
	ldr r2, [r2]
	ldr r3, [r3, #4]
	bl sub_803AD84
	adds r0, r5, #0
	adds r0, #0x80
	str r4, [r0]
_0800C84E:
	ldr r0, [r5, #0x70]
	ldr r1, [r0]
	str r1, [r5, #0x60]
	ldr r0, [r0, #4]
	str r0, [r5, #0x64]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_800C860
sub_800C860: @ 0x0800C860
	push {r4, r5, lr}
	ldr r5, [r0, #0x70]
	ldr r4, [r5]
	lsls r1, r1, #8
	subs r4, r4, r1
	str r4, [r0, #0x10]
	ldr r4, [r5]
	adds r4, r4, r1
	str r4, [r0, #0x14]
	str r3, [r0, #0x5c]
	str r2, [r0, #0x58]
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_800C87C
sub_800C87C: @ 0x0800C87C
	push {r4, r5, lr}
	ldr r5, [r0, #0x70]
	ldr r4, [r5, #4]
	lsls r1, r1, #8
	subs r4, r4, r1
	str r4, [r0, #0x1c]
	ldr r4, [r5, #4]
	adds r4, r4, r1
	str r4, [r0, #0x18]
	str r3, [r0, #0x5c]
	str r2, [r0, #0x58]
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_800C898
sub_800C898: @ 0x0800C898
	ldr r3, [r0, #0x70]
	ldr r2, [r3]
	lsls r1, r1, #8
	subs r2, r2, r1
	str r2, [r0, #0x10]
	ldr r2, [r3]
	adds r2, r2, r1
	str r2, [r0, #0x14]
	bx lr
	.align 2, 0

	thumb_func_start sub_800C8AC
sub_800C8AC: @ 0x0800C8AC
	push {lr}
	adds r2, r1, #0
	str r2, [r0, #0x7c]
	ldr r1, [r0, #0x70]
	bl sub_800B704
	pop {r0}
	bx r0

	thumb_func_start sub_800C8BC
sub_800C8BC: @ 0x0800C8BC
	push {lr}
	adds r2, r1, #0
	str r2, [r0, #0x78]
	ldr r1, [r0, #0x70]
	bl sub_800B838
	pop {r0}
	bx r0

	thumb_func_start sub_800C8CC
sub_800C8CC: @ 0x0800C8CC
	push {r4, r5, lr}
	str r1, [r0, #0x68]
	ldr r3, [r0, #0xc]
	adds r3, #0x50
	movs r2, #0
	ldrsh r4, [r3, r2]
	adds r4, r0, r4
	ldr r5, [r0, #0x70]
	adds r0, #0x84
	ldr r0, [r0]
	lsls r1, r1, #2
	adds r1, r1, r0
	ldr r2, [r1]
	ldr r3, [r3, #4]
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_803AD84
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_800C8F8
sub_800C8F8: @ 0x0800C8F8
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r5, _0800C934 @ =gStaticData_0816A820
	ldr r0, _0800C938 @ =gUnknown_0300082C
	ldr r0, [r0]
	lsls r0, r0, #8
	ldr r1, [r4, #0x3c]
	bl sub_8037E54
	ldr r1, [r4, #0x40]
	ldr r2, _0800C93C @ =0xFFFFFF00
	adds r1, r1, r2
	subs r0, r0, r1
	movs r1, #0xff
	ands r0, r1
	lsls r0, r0, #1
	adds r0, r0, r5
	movs r2, #0
	ldrsh r1, [r0, r2]
	ldr r0, [r4, #0x44]
	adds r2, r1, #0
	muls r2, r0, r2
	ldr r1, [r4, #0x70]
	ldr r0, [r4, #0x60]
	adds r0, r0, r2
	str r0, [r1]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0800C934: .4byte gStaticData_0816A820
_0800C938: .4byte gUnknown_0300082C
_0800C93C: .4byte 0xFFFFFF00

	thumb_func_start sub_800C940
sub_800C940: @ 0x0800C940
	push {r4, r5, r6, lr}
	ldr r3, [r0, #0x70]
	ldr r4, _0800C970 @ =gStaticData_0816A820
	ldr r1, _0800C974 @ =gUnknown_0300082C
	ldr r1, [r1]
	lsrs r1, r1, #1
	ldr r2, [r0, #0x40]
	ldr r6, _0800C978 @ =0xFFFFFF00
	adds r2, r2, r6
	subs r1, r1, r2
	movs r2, #0xff
	ands r1, r2
	lsls r1, r1, #1
	adds r1, r1, r4
	movs r4, #0
	ldrsh r2, [r1, r4]
	ldr r1, [r0, #0x44]
	muls r1, r2, r1
	ldr r0, [r0, #0x64]
	adds r0, r0, r1
	str r0, [r3, #4]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0800C970: .4byte gStaticData_0816A820
_0800C974: .4byte gUnknown_0300082C
_0800C978: .4byte 0xFFFFFF00

	thumb_func_start sub_800C97C
sub_800C97C: @ 0x0800C97C
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r4, r0, #0
	ldr r5, [r4, #0x70]
	ldr r6, _0800C9BC @ =gStaticData_0816A820
	ldr r0, _0800C9C0 @ =gUnknown_0300082C
	ldr r0, [r0]
	lsls r0, r0, #8
	ldr r1, [r4, #0x3c]
	bl sub_8037E54
	ldr r1, [r4, #0x40]
	ldr r2, _0800C9C4 @ =0xFFFFFF00
	adds r1, r1, r2
	subs r0, r0, r1
	movs r1, #0xff
	ands r0, r1
	lsls r0, r0, #1
	adds r0, r0, r6
	movs r2, #0
	ldrsh r1, [r0, r2]
	ldr r0, [r4, #0x44]
	muls r1, r0, r1
	ldr r0, [r4, #0x64]
	adds r0, r0, r1
	str r0, [r5, #4]
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0800C9BC: .4byte gStaticData_0816A820
_0800C9C0: .4byte gUnknown_0300082C
_0800C9C4: .4byte 0xFFFFFF00

	thumb_func_start sub_800C9C8
sub_800C9C8: @ 0x0800C9C8
	push {r4, r5, r6, lr}
	sub sp, #0xc
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	ldr r1, [sp, #0x1c]
	ldr r2, [sp, #0x20]
	ldr r0, _0800CA00 @ =gUnknown_030012E4
	ldr r0, [r0]
	str r3, [sp]
	str r1, [sp, #4]
	str r2, [sp, #8]
	adds r1, r4, #0
	adds r2, r5, #0
	adds r3, r6, #0
	bl sub_8025B0C
	movs r1, #4
	ldrb r2, [r0, #0xc]
	orrs r1, r2
	movs r2, #0x41
	rsbs r2, r2, #0
	ands r1, r2
	strb r1, [r0, #0xc]
	add sp, #0xc
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0800CA00: .4byte gUnknown_030012E4

	thumb_func_start sub_800CA04
sub_800CA04: @ 0x0800CA04
	str r1, [r0, #0x70]
	bx lr

	thumb_func_start sub_800CA08
sub_800CA08: @ 0x0800CA08
	ldr r2, _0800CA44 @ =gUnknown_030012D8
	ldr r3, [r2]
	ldr r2, [r3]
	asrs r2, r2, #8
	subs r0, r0, r2
	asrs r2, r0, #0x1f
	eors r0, r2
	subs r2, r0, r2
	ldr r0, [r3, #4]
	asrs r0, r0, #8
	subs r1, r1, r0
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	cmp r1, r2
	bge _0800CA2A
	adds r1, r2, #0
_0800CA2A:
	cmp r1, #0x20
	bge _0800CA30
	movs r1, #0x20
_0800CA30:
	cmp r1, #0xa0
	ble _0800CA36
	movs r1, #0xa0
_0800CA36:
	subs r1, #0x20
	lsls r1, r1, #1
	movs r0, #0x80
	lsls r0, r0, #1
	subs r0, r0, r1
	bx lr
	.align 2, 0
_0800CA44: .4byte gUnknown_030012D8

	thumb_func_start sub_800CA48
sub_800CA48: @ 0x0800CA48
	movs r2, #0
	str r2, [r0, #0x70]
	adds r1, r0, #0
	adds r1, #0x84
	str r2, [r1]
	ldr r1, _0800CA5C @ =gStaticData_0816BB6C
	str r1, [r0, #4]
	adds r0, #0x88
	str r2, [r0]
	bx lr
	.align 2, 0
_0800CA5C: .4byte gStaticData_0816BB6C

	thumb_func_start sub_800CA60
sub_800CA60: @ 0x0800CA60
	push {lr}
	ldr r2, _0800CA70 @ =gStaticData_087E3EE4
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_0800CA70: .4byte gStaticData_087E3EE4

	thumb_func_start sub_800CA74
sub_800CA74: @ 0x0800CA74
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800B8C8
	ldr r0, _0800CA90 @ =gStaticData_087E3EE4
	str r0, [r4, #0xc]
	adds r0, r4, #0
	bl sub_800CA48
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0800CA90: .4byte gStaticData_087E3EE4

	thumb_func_start sub_800CA94
sub_800CA94: @ 0x0800CA94
	str r1, [r0, #0x3c]
	str r2, [r0, #0x40]
	str r3, [r0, #0x44]
	bx lr

	thumb_func_start sub_800CA9C
sub_800CA9C: @ 0x0800CA9C
	str r1, [r0, #0x48]
	str r2, [r0, #0x4c]
	bx lr
	.align 2, 0

	thumb_func_start sub_800CAA4
sub_800CAA4: @ 0x0800CAA4
	str r1, [r0, #0x30]
	str r2, [r0, #0x34]
	str r3, [r0, #0x38]
	bx lr

	thumb_func_start sub_800CAAC
sub_800CAAC: @ 0x0800CAAC
	push {r4, lr}
	ldr r4, [sp, #8]
	str r1, [r0, #0x20]
	str r3, [r0, #0x28]
	str r2, [r0, #0x24]
	str r4, [r0, #0x2c]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_800CAC0
sub_800CAC0: @ 0x0800CAC0
	adds r0, #0x84
	str r1, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_800CAC8
sub_800CAC8: @ 0x0800CAC8
	str r1, [r0, #0x6c]
	bx lr

	thumb_func_start sub_800CACC
sub_800CACC: @ 0x0800CACC
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r0, [r4]
	asrs r5, r0, #8
	ldr r0, _0800CB14 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0]
	asrs r0, r0, #8
	subs r0, r5, r0
	subs r0, #0xa1
	cmp r0, #0xee
	bhi _0800CB0C
	ldr r0, _0800CB18 @ =gUnknown_0300082C
	ldr r0, [r0]
	ldr r1, [r4, #0x20]
	adds r0, r0, r1
	ldr r2, [r4, #0x24]
	subs r0, r0, r2
	bl sub_803AE4C
	cmp r0, #0
	bne _0800CB0C
	ldr r0, _0800CB1C @ =0x0000FFFF
	lsls r1, r5, #0x10
	lsrs r1, r1, #0x10
	ldr r2, [r4, #4]
	lsls r2, r2, #8
	lsrs r2, r2, #0x10
	ldr r4, [r4, #0x1c]
	movs r3, #0
	bl sub_803AD88
_0800CB0C:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0800CB14: .4byte gUnknown_030012D8
_0800CB18: .4byte gUnknown_0300082C
_0800CB1C: .4byte 0x0000FFFF

	thumb_func_start sub_800CB20
sub_800CB20: @ 0x0800CB20
	push {lr}
	adds r2, r0, #0
	ldr r0, _0800CB3C @ =gStaticData_087E3BEC
	str r0, [r2, #0x18]
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _0800CB36
	adds r0, r2, #0
	bl sub_8026ED0
_0800CB36:
	pop {r0}
	bx r0
	.align 2, 0
_0800CB3C: .4byte gStaticData_087E3BEC

	thumb_func_start sub_800CB40
sub_800CB40: @ 0x0800CB40
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800725C
	ldr r0, _0800CB54 @ =gStaticData_087E3F4C
	str r0, [r4, #0x18]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0800CB54: .4byte gStaticData_087E3F4C

	thumb_func_start sub_800CB58
sub_800CB58: @ 0x0800CB58
	str r1, [r0, #0x20]
	str r2, [r0, #0x24]
	bx lr
	.align 2, 0

	thumb_func_start sub_800CB60
sub_800CB60: @ 0x0800CB60
	str r1, [r0, #0x1c]
	bx lr

	thumb_func_start sub_800CB64
sub_800CB64: @ 0x0800CB64
	push {r4, lr}
	adds r4, r1, #0
	ldr r1, [r4, #0x18]
	movs r2, #0x28
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #0x2c]
	bl sub_803AD7C
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0800CBAE
	movs r0, #1
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _0800CBB4 @ =0x0000FFFF
	ldrh r2, [r4, #8]
	cmp r2, r0
	beq _0800CBAE
	ldrh r3, [r4, #8]
	ldr r0, _0800CBB8 @ =gUnknown_030012B4
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
_0800CBAE:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0800CBB4: .4byte 0x0000FFFF
_0800CBB8: .4byte gUnknown_030012B4

	thumb_func_start nullsub_14
nullsub_14: @ 0x0800CBBC
	bx lr
	.align 2, 0

	thumb_func_start sub_800CBC0
sub_800CBC0: @ 0x0800CBC0
	push {lr}
	ldr r2, _0800CBD0 @ =gStaticData_087E3FA4
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_0800CBD0: .4byte gStaticData_087E3FA4

	thumb_func_start sub_800CBD4
sub_800CBD4: @ 0x0800CBD4
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800B8C8
	ldr r0, _0800CBF0 @ =gStaticData_087E3FA4
	str r0, [r4, #0xc]
	adds r0, r4, #0
	bl nullsub_14
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0800CBF0: .4byte gStaticData_087E3FA4

	thumb_func_start sub_800CBF4
sub_800CBF4: @ 0x0800CBF4
	push {r4, r5, lr}
	adds r4, r1, #0
	ldr r1, [r4, #0x18]
	movs r2, #0x28
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #0x2c]
	bl sub_803AD7C
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0800CC3E
	movs r0, #1
	ldrb r5, [r4, #0xc]
	orrs r0, r5
	strb r0, [r4, #0xc]
	ldr r0, _0800CCBC @ =0x0000FFFF
	ldrh r1, [r4, #8]
	cmp r1, r0
	beq _0800CC3E
	ldrh r3, [r4, #8]
	ldr r0, _0800CCC0 @ =gUnknown_030012B4
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
_0800CC3E:
	ldrb r1, [r4, #0xc]
	lsrs r0, r1, #3
	movs r2, #1
	ands r0, r2
	cmp r0, #0
	beq _0800CC7A
	adds r0, r1, #0
	orrs r0, r2
	strb r0, [r4, #0xc]
	ldr r0, _0800CCBC @ =0x0000FFFF
	ldrh r1, [r4, #8]
	cmp r1, r0
	beq _0800CC7A
	ldrh r3, [r4, #8]
	ldr r0, _0800CCC0 @ =gUnknown_030012B4
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
_0800CC7A:
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800CCB6
	movs r0, #1
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _0800CCBC @ =0x0000FFFF
	ldrh r2, [r4, #8]
	cmp r2, r0
	beq _0800CCB6
	ldrh r3, [r4, #8]
	ldr r0, _0800CCC0 @ =gUnknown_030012B4
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
_0800CCB6:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0800CCBC: .4byte 0x0000FFFF
_0800CCC0: .4byte gUnknown_030012B4

	thumb_func_start nullsub_15
nullsub_15: @ 0x0800CCC4
	bx lr
	.align 2, 0

	thumb_func_start nullsub_3
nullsub_3: @ 0x0800CCC8
	bx lr
	.align 2, 0

	thumb_func_start sub_800CCCC
sub_800CCCC: @ 0x0800CCCC
	push {lr}
	ldr r2, _0800CCDC @ =gStaticData_087E400C
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_0800CCDC: .4byte gStaticData_087E400C

	thumb_func_start sub_800CCE0
sub_800CCE0: @ 0x0800CCE0
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800B8C8
	ldr r0, _0800CCFC @ =gStaticData_087E400C
	str r0, [r4, #0xc]
	adds r0, r4, #0
	bl nullsub_3
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0800CCFC: .4byte gStaticData_087E400C

	thumb_func_start sub_800CD00
sub_800CD00: @ 0x0800CD00
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x20
	adds r6, r0, #0
	mov sl, r1
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #5
	bne _0800CD1A
	b _0800CE98
_0800CD1A:
	cmp r0, #0xa
	bne _0800CD20
	b _0800CE98
_0800CD20:
	ldr r1, [r6, #0x20]
	adds r2, r6, #0
	adds r2, #0x2d
	ldrb r3, [r2]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	ldr r1, [r1]
	adds r1, r1, r0
	adds r3, r1, #4
	ldr r0, [r6]
	asrs r7, r0, #8
	ldr r0, [r6, #4]
	asrs r0, r0, #8
	mov r8, r0
	movs r0, #4
	ldrsh r1, [r1, r0]
	movs r0, #2
	ldrsh r2, [r3, r0]
	ldrb r4, [r3, #4]
	ldrb r5, [r3, #5]
	adds r1, r1, r7
	add r2, r8
	mov r0, sp
	bl sub_803AFE4
	mov r0, sp
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_803AFDC
	adds r3, r6, #0
	adds r3, #0x28
	ldrb r1, [r3]
	lsls r0, r1, #0x1b
	cmp r0, #0
	bge _0800CD76
	lsls r0, r7, #1
	ldr r1, [sp]
	ldr r2, [sp, #8]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp]
_0800CD76:
	ldrb r3, [r3]
	lsls r0, r3, #0x1a
	cmp r0, #0
	bge _0800CD8C
	mov r2, r8
	lsls r0, r2, #1
	ldr r1, [sp, #4]
	ldr r2, [sp, #0xc]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #4]
_0800CD8C:
	ldr r3, _0800CE94 @ =gUnknown_030012D8
	mov sb, r3
	ldr r0, [r3]
	ldr r1, [r0]
	asrs r7, r1, #8
	ldr r1, [r0, #4]
	asrs r1, r1, #8
	mov r8, r1
	ldr r2, [r0, #0x20]
	adds r0, #0x2d
	ldrb r3, [r0]
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	ldr r0, [r2]
	adds r1, r0, r1
	adds r0, r1, #4
	movs r2, #4
	ldrsh r1, [r1, r2]
	movs r3, #2
	ldrsh r2, [r0, r3]
	ldrb r4, [r0, #4]
	ldrb r5, [r0, #5]
	adds r1, r1, r7
	add r2, r8
	add r0, sp, #0x10
	bl sub_803AFE4
	add r0, sp, #0x10
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_803AFDC
	mov r1, sb
	ldr r0, [r1]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _0800CDE8
	lsls r0, r7, #1
	ldr r1, [sp, #0x10]
	ldr r2, [sp, #0x18]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #0x10]
_0800CDE8:
	mov r2, sb
	ldr r0, [r2]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1a
	cmp r0, #0
	bge _0800CE04
	mov r3, r8
	lsls r0, r3, #1
	ldr r1, [sp, #0x14]
	ldr r2, [sp, #0x1c]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #0x14]
_0800CE04:
	add r6, sp, #0x10
	mov r0, sp
	adds r1, r6, #0
	bl sub_8001640
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0800CE98
	mov r1, sb
	ldr r0, [r1]
	ldr r0, [r0, #0x20]
	mov r2, sl
	lsls r1, r2, #3
	subs r1, r1, r2
	lsls r1, r1, #2
	ldr r0, [r0]
	adds r0, r0, r1
	adds r3, r0, #4
	movs r2, #4
	ldrsh r1, [r0, r2]
	movs r0, #2
	ldrsh r2, [r3, r0]
	ldrb r4, [r3, #4]
	ldrb r5, [r3, #5]
	adds r1, r1, r7
	add r2, r8
	adds r0, r6, #0
	bl sub_803AFE4
	adds r0, r6, #0
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_803AFDC
	mov r1, sb
	ldr r0, [r1]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _0800CE62
	lsls r0, r7, #1
	ldr r1, [sp, #0x10]
	ldr r2, [sp, #0x18]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #0x10]
_0800CE62:
	mov r2, sb
	ldr r0, [r2]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1a
	cmp r0, #0
	bge _0800CE7E
	mov r3, r8
	lsls r0, r3, #1
	ldr r1, [sp, #0x14]
	ldr r2, [sp, #0x1c]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #0x14]
_0800CE7E:
	mov r0, sp
	adds r1, r6, #0
	bl sub_8001640
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #1
	bne _0800CE98
	movs r0, #1
	b _0800CE9A
	.align 2, 0
_0800CE94: .4byte gUnknown_030012D8
_0800CE98:
	movs r0, #0
_0800CE9A:
	add sp, #0x20
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_800CEAC
sub_800CEAC: @ 0x0800CEAC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x10
	adds r6, r1, #0
	mov r8, r2
	adds r7, r3, #0
	ldr r0, _0800CEF0 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x90
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800CEF4
	movs r0, #0
	ldrsh r1, [r6, r0]
	movs r0, #2
	ldrsh r2, [r6, r0]
	ldrb r5, [r6, #5]
	adds r1, r1, r7
	subs r1, #2
	ldr r0, [sp, #0x28]
	adds r2, r2, r0
	ldrb r4, [r6, #4]
	adds r4, #4
	mov r0, sp
	bl sub_803AFE4
	mov r0, sp
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_803AFDC
	b _0800CF16
	.align 2, 0
_0800CEF0: .4byte gUnknown_030012D8
_0800CEF4:
	movs r0, #0
	ldrsh r1, [r6, r0]
	movs r0, #2
	ldrsh r2, [r6, r0]
	ldrb r4, [r6, #4]
	ldrb r5, [r6, #5]
	adds r1, r1, r7
	ldr r0, [sp, #0x28]
	adds r2, r2, r0
	mov r0, sp
	bl sub_803AFE4
	mov r0, sp
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_803AFDC
_0800CF16:
	ldr r3, _0800CF5C @ =gUnknown_030012D8
	ldr r0, [r3]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _0800CF30
	lsls r0, r7, #1
	ldr r1, [sp]
	ldr r2, [sp, #8]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp]
_0800CF30:
	ldr r0, [r3]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1a
	cmp r0, #0
	bge _0800CF4A
	ldr r1, [sp, #0x28]
	lsls r0, r1, #1
	ldr r1, [sp, #4]
	ldr r2, [sp, #0xc]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #4]
_0800CF4A:
	mov r0, r8
	mov r1, sp
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0800CF60
	movs r0, #0
	b _0800CF62
	.align 2, 0
_0800CF5C: .4byte gUnknown_030012D8
_0800CF60:
	movs r0, #1
_0800CF62:
	add sp, #0x10
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_800CF70
sub_800CF70: @ 0x0800CF70
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	mov sl, r1
	adds r5, r2, #0
	adds r7, r0, #0
	bl sub_801070C
	adds r4, r0, #0
	adds r0, r7, #0
	bl sub_8010708
	adds r6, r0, #0
	cmp r4, #0
	bne _0800CF98
	cmp r6, #0
	beq _0800D02E
_0800CF98:
	movs r0, #1
	strb r0, [r5]
	cmp r6, #0
	beq _0800D02E
	adds r1, r6, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #1
	beq _0800D02E
	ldr r1, [r6, #0x20]
	adds r2, r6, #0
	adds r2, #0x2d
	ldrb r3, [r2]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	ldr r1, [r1]
	adds r1, r1, r0
	adds r3, r1, #4
	ldr r0, [r6]
	asrs r0, r0, #8
	mov r8, r0
	ldr r0, [r6, #4]
	asrs r0, r0, #8
	mov sb, r0
	movs r0, #4
	ldrsh r1, [r1, r0]
	movs r0, #2
	ldrsh r2, [r3, r0]
	ldrb r4, [r3, #4]
	ldrb r5, [r3, #5]
	add r1, r8
	add r2, sb
	mov r0, sp
	bl sub_803AFE4
	mov r0, sp
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_803AFDC
	adds r3, r7, #0
	adds r3, #0x28
	ldrb r1, [r3]
	lsls r0, r1, #0x1b
	cmp r0, #0
	bge _0800D008
	mov r1, r8
	lsls r0, r1, #1
	ldr r1, [sp]
	ldr r2, [sp, #8]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp]
_0800D008:
	ldrb r3, [r3]
	lsls r0, r3, #0x1a
	cmp r0, #0
	bge _0800D01E
	mov r3, sb
	lsls r0, r3, #1
	ldr r1, [sp, #4]
	ldr r2, [sp, #0xc]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #4]
_0800D01E:
	mov r0, sp
	mov r1, sl
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0800D02E
	adds r7, r6, #0
_0800D02E:
	adds r0, r7, #0
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_800D040
sub_800D040: @ 0x0800D040
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #0x20
	adds r6, r0, #0
	adds r1, r6, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #1
	bne _0800D05C
	b _0800D17C
_0800D05C:
	ldr r1, [r6, #0x20]
	adds r2, r6, #0
	adds r2, #0x2d
	ldrb r3, [r2]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	ldr r1, [r1]
	adds r1, r1, r0
	adds r3, r1, #4
	ldr r0, [r6]
	asrs r7, r0, #8
	ldr r0, [r6, #4]
	asrs r0, r0, #8
	mov r8, r0
	movs r0, #4
	ldrsh r1, [r1, r0]
	movs r0, #2
	ldrsh r2, [r3, r0]
	ldrb r4, [r3, #4]
	ldrb r5, [r3, #5]
	adds r1, r1, r7
	add r2, r8
	mov r0, sp
	bl sub_803AFE4
	mov r0, sp
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_803AFDC
	adds r3, r6, #0
	adds r3, #0x28
	ldrb r1, [r3]
	lsls r0, r1, #0x1b
	cmp r0, #0
	bge _0800D0B2
	lsls r0, r7, #1
	ldr r1, [sp]
	ldr r2, [sp, #8]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp]
_0800D0B2:
	ldrb r3, [r3]
	lsls r0, r3, #0x1a
	cmp r0, #0
	bge _0800D0C8
	mov r2, r8
	lsls r0, r2, #1
	ldr r1, [sp, #4]
	ldr r2, [sp, #0xc]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #4]
_0800D0C8:
	ldr r3, _0800D168 @ =gUnknown_030012D8
	mov sb, r3
	ldr r0, [r3]
	ldr r1, [r0]
	asrs r7, r1, #8
	ldr r1, [r0, #4]
	asrs r1, r1, #8
	mov r8, r1
	ldr r2, [r0, #0x20]
	adds r0, #0x2d
	ldrb r3, [r0]
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	ldr r0, [r2]
	adds r0, r0, r1
	adds r3, r0, #4
	movs r2, #4
	ldrsh r1, [r0, r2]
	movs r0, #2
	ldrsh r2, [r3, r0]
	ldrb r4, [r3, #4]
	ldrb r5, [r3, #5]
	adds r1, r1, r7
	add r2, r8
	add r0, sp, #0x10
	bl sub_803AFE4
	add r0, sp, #0x10
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_803AFDC
	mov r1, sb
	ldr r0, [r1]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _0800D124
	lsls r0, r7, #1
	ldr r1, [sp, #0x10]
	ldr r2, [sp, #0x18]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #0x10]
_0800D124:
	mov r2, sb
	ldr r0, [r2]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1a
	cmp r0, #0
	bge _0800D140
	mov r3, r8
	lsls r0, r3, #1
	ldr r1, [sp, #0x14]
	ldr r2, [sp, #0x1c]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #0x14]
_0800D140:
	add r1, sp, #0x10
	mov r0, sp
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0800D17C
	ldr r0, _0800D16C @ =gStaticData_0816BBC4
	adds r1, r6, #0
	adds r1, #0x4e
	ldrb r1, [r1]
	adds r0, r1, r0
	ldrb r0, [r0]
	cmp r0, #1
	bne _0800D170
	adds r0, r6, #0
	movs r1, #1
	bl sub_800EEF0
	b _0800D17C
	.align 2, 0
_0800D168: .4byte gUnknown_030012D8
_0800D16C: .4byte gStaticData_0816BBC4
_0800D170:
	adds r0, r6, #0
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_800E7A8
_0800D17C:
	add sp, #0x20
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_0800D18C
sub_0800D18C: @ 0x0800D18C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xa4
	mov sl, r0
	adds r6, r1, #0
	ldr r1, [r0, #0x20]
	mov r2, sl
	adds r2, #0x2d
	ldrb r3, [r2]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	ldr r1, [r1]
	adds r1, r1, r0
	adds r3, r1, #4
	mov r4, sl
	ldr r0, [r4]
	asrs r0, r0, #8
	str r0, [sp, #0x70]
	ldr r0, [r4, #4]
	asrs r0, r0, #8
	str r0, [sp, #0x74]
	movs r5, #4
	ldrsh r1, [r1, r5]
	movs r0, #2
	ldrsh r2, [r3, r0]
	ldrb r4, [r3, #4]
	ldrb r5, [r3, #5]
	ldr r3, [sp, #0x70]
	adds r1, r1, r3
	ldr r0, [sp, #0x74]
	adds r2, r2, r0
	add r0, sp, #0x1c
	bl sub_803AFE4
	add r0, sp, #0x1c
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_803AFDC
	mov r3, sl
	adds r3, #0x28
	ldrb r1, [r3]
	lsls r0, r1, #0x1b
	cmp r0, #0
	bge _0800D1FC
	ldr r2, [sp, #0x70]
	lsls r0, r2, #1
	ldr r1, [sp, #0x1c]
	ldr r2, [sp, #0x24]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #0x1c]
_0800D1FC:
	ldrb r3, [r3]
	lsls r0, r3, #0x1a
	cmp r0, #0
	bge _0800D212
	ldr r3, [sp, #0x74]
	lsls r0, r3, #1
	ldr r1, [sp, #0x20]
	ldr r2, [sp, #0x28]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #0x20]
_0800D212:
	ldr r1, _0800D234 @ =gUnknown_030012D8
	ldr r3, [r1]
	ldr r0, [r3]
	asrs r0, r0, #8
	str r0, [sp, #0x70]
	ldr r0, [r3, #4]
	asrs r0, r0, #8
	str r0, [sp, #0x74]
	ldr r0, _0800D238 @ =gUnknown_030012C0
	ldr r0, [r0]
	ldr r0, [r0, #0x78]
	cmp r0, #3
	bne _0800D23C
	movs r4, #6
	str r4, [sp, #0x78]
	b _0800D264
	.align 2, 0
_0800D234: .4byte gUnknown_030012D8
_0800D238: .4byte gUnknown_030012C0
_0800D23C:
	ldr r1, _0800D2A0 @ =gStaticData_0816BBF0
	lsls r0, r6, #2
	adds r0, r0, r1
	ldr r0, [r0]
	str r0, [sp, #0x78]
	mov r0, sl
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #0xd
	bne _0800D264
	ldr r5, [sp, #0x78]
	cmp r5, #5
	bne _0800D264
	adds r0, r3, #0
	adds r0, #0x24
	ldrb r0, [r0]
	cmp r0, #4
	bne _0800D264
	movs r6, #2
	str r6, [sp, #0x78]
_0800D264:
	mov r1, sl
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r2, [r1]
	ands r0, r2
	str r1, [sp, #0x94]
	cmp r0, #1
	bne _0800D276
	b _0800D6AC
_0800D276:
	mov r3, sl
	ldr r0, [r3, #0x44]
	cmp r0, #0
	beq _0800D280
	b _0800D6AC
_0800D280:
	ldr r4, _0800D2A4 @ =gUnknown_030012D8
	ldr r0, [r4]
	bl sub_80083B8
	adds r2, r0, #0
	ldr r0, [r2, #4]
	ldrb r0, [r0]
	lsrs r0, r0, #4
	cmp r0, #6
	bhi _0800D2CE
	lsls r0, r0, #2
	ldr r1, _0800D2A8 @ =_0800D2AC
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800D2A0: .4byte gStaticData_0816BBF0
_0800D2A4: .4byte gUnknown_030012D8
_0800D2A8: .4byte _0800D2AC
_0800D2AC: @ jump table
	.4byte _0800D2C8 @ case 0
	.4byte _0800D2CE @ case 1
	.4byte _0800D2CE @ case 2
	.4byte _0800D2CE @ case 3
	.4byte _0800D2C8 @ case 4
	.4byte _0800D2CE @ case 5
	.4byte _0800D2CE @ case 6
_0800D2C8:
	adds r3, r2, #0
	adds r3, #0x1c
	b _0800D2D0
_0800D2CE:
	ldr r3, _0800D36C @ =gStaticData_0816B2F8
_0800D2D0:
	movs r6, #0
	ldrb r0, [r3, #4]
	cmp r0, #0
	bne _0800D2E0
	ldrb r0, [r3, #5]
	cmp r0, #0
	bne _0800D2E0
	movs r6, #1
_0800D2E0:
	cmp r6, #0
	beq _0800D2E6
	b _0800D6AC
_0800D2E6:
	movs r5, #0
	ldrsh r1, [r3, r5]
	movs r0, #2
	ldrsh r2, [r3, r0]
	ldrb r4, [r3, #4]
	ldrb r5, [r3, #5]
	ldr r3, [sp, #0x70]
	adds r1, r1, r3
	ldr r0, [sp, #0x74]
	adds r2, r2, r0
	add r0, sp, #0x3c
	bl sub_803AFE4
	add r0, sp, #0x3c
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_803AFDC
	ldr r3, _0800D370 @ =gUnknown_030012D8
	ldr r0, [r3]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _0800D326
	ldr r1, [sp, #0x70]
	lsls r0, r1, #1
	ldr r1, [sp, #0x3c]
	ldr r2, [sp, #0x44]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #0x3c]
_0800D326:
	ldr r0, [r3]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1a
	cmp r0, #0
	bge _0800D340
	ldr r2, [sp, #0x74]
	lsls r0, r2, #1
	ldr r1, [sp, #0x40]
	ldr r2, [sp, #0x48]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #0x40]
_0800D340:
	add r4, sp, #0x3c
	add r0, sp, #0x1c
	adds r1, r4, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0800D352
	b _0800D6AC
_0800D352:
	add r0, sp, #0x4c
	strb r6, [r0]
	ldr r3, [sp, #0x78]
	cmp r3, #4
	bgt _0800D374
	mov r0, sl
	adds r1, r4, #0
	add r2, sp, #0x4c
	bl sub_800CF70
	mov r8, r0
	b _0800D376
	.align 2, 0
_0800D36C: .4byte gStaticData_0816B2F8
_0800D370: .4byte gUnknown_030012D8
_0800D374:
	mov r8, sl
_0800D376:
	ldr r2, _0800D3E4 @ =gStaticData_0816BC98
	mov r1, r8
	adds r1, #0x4e
	ldr r4, [sp, #0x78]
	lsls r3, r4, #2
	ldrb r5, [r1]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r3, r0
	adds r0, r0, r2
	ldr r6, [r0]
	ldr r0, _0800D3E8 @ =gUnknown_030012D8
	ldr r2, [r0]
	adds r4, r2, #0
	adds r4, #0x92
	mov sb, r1
	str r3, [sp, #0x98]
	ldrb r4, [r4]
	cmp r4, #4
	ble _0800D3A2
	movs r6, #0
_0800D3A2:
	add r1, sp, #0x4c
	ldrb r0, [r1]
	cmp r0, #0
	beq _0800D43A
	adds r0, r2, #0
	adds r0, #0x94
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800D43A
	movs r3, #0
	cmp r3, r0
	bge _0800D43A
_0800D3BA:
	ldr r4, _0800D3E8 @ =gUnknown_030012D8
	ldr r2, [r4]
	adds r0, r2, #0
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #0
	bne _0800D3EC
	cmp r3, #4
	ble _0800D3D6
	adds r0, r2, #0
	adds r0, #0x94
	ldrb r0, [r0]
	cmp r3, r0
	bge _0800D3EC
_0800D3D6:
	lsls r1, r3, #2
	adds r0, r2, #0
	adds r0, #0x98
	adds r0, r0, r1
	ldr r5, [r0]
	b _0800D3EE
	.align 2, 0
_0800D3E4: .4byte gStaticData_0816BC98
_0800D3E8: .4byte gUnknown_030012D8
_0800D3EC:
	movs r5, #0
_0800D3EE:
	adds r7, r3, #1
	cmp r5, #0
	beq _0800D42C
	adds r0, r5, #0
	bl sub_8010708
	adds r4, r0, #0
	cmp r4, #0
	beq _0800D416
	b _0800D40A
_0800D402:
	adds r0, r4, #0
	bl sub_8010708
	adds r4, r0, #0
_0800D40A:
	adds r0, r4, #0
	bl sub_8010708
	cmp r0, #0
	bne _0800D402
	b _0800D422
_0800D416:
	adds r4, r5, #0
	b _0800D422
_0800D41A:
	adds r0, r4, #0
	bl sub_801070C
	adds r4, r0, #0
_0800D422:
	cmp r4, #0
	beq _0800D42C
	cmp sl, r4
	bne _0800D41A
	movs r6, #0
_0800D42C:
	adds r3, r7, #0
	ldr r1, _0800D44C @ =gUnknown_030012D8
	ldr r0, [r1]
	adds r0, #0x94
	ldrb r0, [r0]
	cmp r3, r0
	blt _0800D3BA
_0800D43A:
	movs r7, #0
	cmp r6, #5
	bls _0800D442
	b _0800D6AC
_0800D442:
	lsls r0, r6, #2
	ldr r1, _0800D450 @ =_0800D454
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800D44C: .4byte gUnknown_030012D8
_0800D450: .4byte _0800D454
_0800D454: @ jump table
	.4byte _0800D46C @ case 0
	.4byte _0800D46C @ case 1
	.4byte _0800D48A @ case 2
	.4byte _0800D4A8 @ case 3
	.4byte _0800D684 @ case 4
	.4byte _0800D6A2 @ case 5
_0800D46C:
	mov r5, sb
	ldrb r1, [r5]
	cmp r1, #6
	bne _0800D47C
	mov r0, r8
	bl sub_800F2BC
	b _0800D6AC
_0800D47C:
	cmp r1, #3
	beq _0800D482
	b _0800D6AC
_0800D482:
	mov r0, r8
	bl sub_800F368
	b _0800D6AC
_0800D48A:
	mov r1, r8
	adds r1, #0x4d
	movs r0, #0x80
	ldrb r6, [r1]
	orrs r0, r6
	strb r0, [r1]
	ldr r1, _0800D4A4 @ =gUnknown_030012D8
	ldr r0, [r1]
	movs r1, #1
	adds r0, #0x80
	strb r1, [r0]
	b _0800D6AC
	.align 2, 0
_0800D4A4: .4byte gUnknown_030012D8
_0800D4A8:
	mov r0, r8
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_800E7A8
	ldr r2, _0800D534 @ =gUnknown_030012D8
	mov sb, r2
	ldr r1, [r2]
	adds r0, r1, #0
	adds r0, #0x94
	ldrb r0, [r0]
	cmp r0, #0
	bne _0800D56E
	ldr r2, [r1, #0x20]
	adds r1, #0x2d
	ldrb r3, [r1]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	ldr r1, [r2]
	adds r0, r1, r0
	ldr r4, [sp, #0x78]
	cmp r4, #3
	beq _0800D560
	adds r1, r0, #4
	ldr r5, [sp, #0x74]
	str r5, [sp]
	mov r0, sl
	add r2, sp, #0x1c
	ldr r3, [sp, #0x70]
	bl sub_800CEAC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0800D560
	mov r0, r8
	bl sub_801070C
	adds r4, r0, #0
	cmp r4, #0
	beq _0800D56E
	adds r3, r4, #0
	adds r3, #0x4d
	ldrb r5, [r3]
	movs r0, #0x7f
	ands r0, r5
	cmp r0, #1
	beq _0800D56E
	ldr r1, _0800D538 @ =gStaticData_0816BC98
	adds r2, r4, #0
	adds r2, #0x4e
	ldrb r6, [r2]
	lsls r0, r6, #3
	subs r0, r0, r6
	lsls r0, r0, #2
	ldr r2, [sp, #0x98]
	adds r0, r2, r0
	adds r0, r0, r1
	ldr r0, [r0]
	cmp r0, #3
	bne _0800D53C
	adds r0, r4, #0
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_800E7A8
	b _0800D56E
	.align 2, 0
_0800D534: .4byte gUnknown_030012D8
_0800D538: .4byte gStaticData_0816BC98
_0800D53C:
	cmp r0, #2
	bne _0800D552
	movs r0, #0x80
	orrs r0, r5
	strb r0, [r3]
	mov r3, sb
	ldr r0, [r3]
	movs r1, #1
	adds r0, #0x80
	strb r1, [r0]
	b _0800D56E
_0800D552:
	cmp r0, #4
	bne _0800D56E
	adds r0, r4, #0
	movs r1, #1
	bl sub_800EEF0
	b _0800D56E
_0800D560:
	ldr r0, _0800D5BC @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x24
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800D56E
	adds r7, #2
_0800D56E:
	add r4, sp, #0x4c
	ldrb r0, [r4]
	cmp r0, #0
	bne _0800D57A
	bl _0800E074
_0800D57A:
	ldr r5, _0800D5BC @ =gUnknown_030012D8
	ldr r0, [r5]
	ldr r1, [r0]
	asrs r1, r1, #8
	mov r6, sl
	ldr r0, [r6]
	asrs r0, r0, #8
	cmp r1, r0
	bge _0800D5C0
	ldr r0, [sp, #0x78]
	cmp r0, #3
	bne _0800D59C
	mov r0, r8
	bl sub_801070C
	cmp r0, #0
	beq _0800D5F2
_0800D59C:
	ldr r0, [r5]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xc
	movs r3, #1
	bl sub_803AD88
	ldr r2, [r5]
	movs r1, #1
	b _0800D5EC
	.align 2, 0
_0800D5BC: .4byte gUnknown_030012D8
_0800D5C0:
	ldr r4, [sp, #0x78]
	cmp r4, #3
	bne _0800D5D0
	mov r0, r8
	bl sub_801070C
	cmp r0, #0
	beq _0800D5F2
_0800D5D0:
	ldr r0, [r5]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r6, #0
	ldrsh r2, [r1, r6]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xc
	movs r3, #2
	bl sub_803AD88
	ldr r2, [r5]
	movs r1, #2
_0800D5EC:
	ldr r0, [r2, #0x74]
	orrs r0, r1
	str r0, [r2, #0x74]
_0800D5F2:
	ldr r0, _0800D680 @ =gUnknown_030012D8
	ldr r3, [r0]
	adds r1, r3, #0
	adds r1, #0x88
	ldrb r1, [r1]
	cmp r1, #0
	bne _0800D614
	adds r0, r3, #0
	adds r0, #0x94
	ldrb r1, [r0]
	cmp r1, #4
	bhi _0800D614
	lsls r1, r1, #2
	adds r0, #4
	adds r0, r0, r1
	mov r2, r8
	str r2, [r0]
_0800D614:
	ldr r3, _0800D680 @ =gUnknown_030012D8
	ldr r1, [r3]
	adds r0, r1, #0
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #0
	bne _0800D62A
	adds r1, #0x94
	ldrb r0, [r1]
	adds r0, #1
	strb r0, [r1]
_0800D62A:
	ldr r4, _0800D680 @ =gUnknown_030012D8
	ldr r0, [r4]
	adds r1, r0, #0
	adds r1, #0x92
	ldrb r0, [r1]
	cmp r0, #0
	bne _0800D63C
	movs r0, #1
	strb r0, [r1]
_0800D63C:
	cmp r7, #0
	bne _0800D644
	bl _0800E074
_0800D644:
	ldr r2, _0800D680 @ =gUnknown_030012D8
_0800D646:
	ldr r3, [r2]
	adds r0, r3, #0
	adds r0, #0x88
	ldrb r4, [r0]
	cmp r4, #0
	bne _0800D662
	adds r0, #0xc
	ldrb r5, [r0]
	cmp r5, #4
	bhi _0800D662
	lsls r1, r5, #2
	adds r0, #4
	adds r0, r0, r1
	str r4, [r0]
_0800D662:
	ldr r1, [r2]
	adds r0, r1, #0
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #0
	bne _0800D676
	adds r1, #0x94
	ldrb r0, [r1]
	adds r0, #1
	strb r0, [r1]
_0800D676:
	subs r7, #1
	cmp r7, #0
	bne _0800D646
	bl _0800E074
	.align 2, 0
_0800D680: .4byte gUnknown_030012D8
_0800D684:
	mov r1, r8
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _0800D696
	bl _0800E074
_0800D696:
	mov r0, r8
	movs r1, #1
	bl sub_800EEF0
	bl _0800E074
_0800D6A2:
	mov r0, r8
	bl sub_800E6B0
	bl _0800E074
_0800D6AC:
	ldr r6, [sp, #0x78]
	cmp r6, #5
	bne _0800D6C8
	ldr r0, _0800D6C4 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x24
	ldrb r0, [r0]
	cmp r0, #4
	bne _0800D6D2
	movs r0, #2
	str r0, [sp, #0x78]
	b _0800D6D2
	.align 2, 0
_0800D6C4: .4byte gUnknown_030012D8
_0800D6C8:
	ldr r1, [sp, #0x78]
	cmp r1, #3
	bne _0800D6D2
	movs r2, #1
	str r2, [sp, #0x78]
_0800D6D2:
	mov r1, sl
	adds r1, #0x58
	ldrb r0, [r1]
	cmp r0, #0
	beq _0800D6EC
	movs r0, #0
	strb r0, [r1]
	mov r3, sl
	ldr r0, [r3, #0x44]
	cmp r0, #0
	bne _0800D6EC
	bl _0800E074
_0800D6EC:
	add r2, sp, #0x2c
	adds r1, r2, #0
	add r0, sp, #0x1c
	ldm r0!, {r4, r5, r6}
	stm r1!, {r4, r5, r6}
	ldr r0, [r0]
	str r0, [r1]
	ldr r0, [sp, #0x78]
	cmp r0, #6
	beq _0800D708
	mov r0, sl
	add r1, sp, #0x1c
	bl sub_800E4E4
_0800D708:
	ldr r6, _0800D7F8 @ =gUnknown_030012D8
	ldr r0, [r6]
	ldr r2, [r0, #0x20]
	adds r0, #0x2d
	ldrb r3, [r0]
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	ldr r0, [r2]
	adds r0, r0, r1
	adds r4, r0, #4
	mov r8, r4
	movs r5, #4
	ldrsh r1, [r0, r5]
	movs r0, #2
	ldrsh r2, [r4, r0]
	ldrb r4, [r4, #4]
	mov r3, r8
	ldrb r5, [r3, #5]
	ldr r0, [sp, #0x70]
	adds r1, r1, r0
	ldr r3, [sp, #0x74]
	adds r2, r2, r3
	add r0, sp, #0x3c
	bl sub_803AFE4
	add r0, sp, #0x3c
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_803AFDC
	ldr r0, [r6]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _0800D760
	ldr r4, [sp, #0x70]
	lsls r0, r4, #1
	ldr r1, [sp, #0x3c]
	ldr r2, [sp, #0x44]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #0x3c]
_0800D760:
	ldr r0, [r6]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1a
	cmp r0, #0
	bge _0800D77A
	ldr r5, [sp, #0x74]
	lsls r0, r5, #1
	ldr r1, [sp, #0x40]
	ldr r2, [sp, #0x48]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #0x40]
_0800D77A:
	add r4, sp, #0x3c
	add r0, sp, #0x1c
	adds r1, r4, #0
	bl sub_8001640
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0800D78E
	bl _0800E074
_0800D78E:
	movs r0, #0
	str r0, [sp, #0x7c]
	movs r1, #0
	str r1, [sp, #0x88]
	ldr r1, [sp, #0x40]
	ldr r0, [sp, #0x20]
	cmp r1, r0
	bge _0800D7A2
	movs r2, #1
	str r2, [sp, #0x88]
_0800D7A2:
	mov r3, sl
	ldr r0, [r3, #0x44]
	cmp r0, #0
	bne _0800D7AC
	b _0800DA84
_0800D7AC:
	add r0, sp, #0x2c
	add r1, sp, #0x3c
	bl sub_8001640
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0800D7BE
	bl _0800E074
_0800D7BE:
	ldr r4, _0800D7FC @ =gStaticData_0816BBDA
	mov r0, sl
	adds r0, #0x4e
	ldrb r5, [r0]
	adds r1, r5, r4
	ldrb r1, [r1]
	adds r7, r0, #0
	cmp r1, #0
	bne _0800D7D2
	b _0800DA34
_0800D7D2:
	ldr r0, [r6]
	bl sub_8009EC4
	ldr r1, [sp, #0x70]
	ldr r0, [r6]
	ldr r1, [r0]
	asrs r1, r1, #8
	mov r2, sl
	ldr r0, [r2]
	asrs r0, r0, #8
	cmp r1, r0
	bge _0800D800
	movs r3, #1
	str r3, [sp, #0x80]
	ldr r0, [sp, #0x3c]
	ldr r1, [sp, #0x44]
	ldr r2, [sp, #0x2c]
	b _0800D80A
	.align 2, 0
_0800D7F8: .4byte gUnknown_030012D8
_0800D7FC: .4byte gStaticData_0816BBDA
_0800D800:
	movs r4, #2
	str r4, [sp, #0x80]
	ldr r0, [sp, #0x2c]
	ldr r1, [sp, #0x34]
	ldr r2, [sp, #0x3c]
_0800D80A:
	adds r0, r0, r1
	subs r0, r0, r2
	adds r5, r0, #1
	ldr r2, _0800D830 @ =gUnknown_030012D8
	ldr r0, [r2]
	ldr r1, [r0, #4]
	asrs r1, r1, #8
	mov r6, sl
	ldr r0, [r6, #4]
	asrs r0, r0, #8
	cmp r1, r0
	ble _0800D834
	movs r0, #4
	str r0, [sp, #0x84]
	ldr r0, [sp, #0x30]
	ldr r1, [sp, #0x38]
	ldr r2, [sp, #0x40]
	b _0800D83E
	.align 2, 0
_0800D830: .4byte gUnknown_030012D8
_0800D834:
	movs r1, #8
	str r1, [sp, #0x84]
	ldr r0, [sp, #0x40]
	ldr r1, [sp, #0x48]
	ldr r2, [sp, #0x30]
_0800D83E:
	adds r0, r0, r1
	subs r0, r0, r2
	mov sb, r0
	cmp r5, #5
	ble _0800D8E8
	ldr r2, [sp, #0x88]
	cmp r2, #0
	bne _0800D8E8
	ldr r0, _0800D8B0 @ =gUnknown_030012C0
	ldr r1, [r0]
	ldr r1, [r1, #0x78]
	adds r4, r0, #0
	cmp r1, #0
	bne _0800D880
	ldr r3, _0800D8B4 @ =gUnknown_030012D8
	ldr r2, [r3]
	ldrb r5, [r2, #0xc]
	lsrs r0, r5, #6
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _0800D880
	movs r3, #0
	adds r0, r2, #0
	adds r0, #0x8c
	ldr r1, _0800D8B8 @ =gUnknown_0300082C
	ldr r2, [r0]
	ldr r0, [r1]
	cmp r2, r0
	bls _0800D87C
	movs r3, #1
_0800D87C:
	cmp r3, #0
	beq _0800D886
_0800D880:
	ldrb r7, [r7]
	cmp r7, #0xd
	beq _0800D8BC
_0800D886:
	ldr r6, _0800D8B4 @ =gUnknown_030012D8
	ldr r1, [r6]
	movs r0, #0x40
	ldrb r2, [r1, #0xc]
	orrs r0, r2
	strb r0, [r1, #0xc]
	ldr r0, [r4]
	movs r1, #0
	bl sub_80231EC
	ldr r0, [r6]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xa
	b _0800D8DC
	.align 2, 0
_0800D8B0: .4byte gUnknown_030012C0
_0800D8B4: .4byte gUnknown_030012D8
_0800D8B8: .4byte gUnknown_0300082C
_0800D8BC:
	mov r0, sl
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_800E7A8
	ldr r0, _0800D8E4 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r4, #0
	ldrsh r2, [r1, r4]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #1
_0800D8DC:
	movs r3, #0
	bl sub_803AD88
	b _0800E074
	.align 2, 0
_0800D8E4: .4byte gUnknown_030012D8
_0800D8E8:
	cmp r5, #6
	ble _0800D940
	mov r6, sb
	cmp r6, #1
	ble _0800D93C
	ldr r0, [sp, #0x88]
	cmp r0, #0
	beq _0800D93C
	ldr r2, _0800D938 @ =gUnknown_030012D8
	ldr r1, [r2]
	ldr r0, [r1]
	str r0, [sp, #0x50]
	ldr r1, [r1, #4]
	add r2, sp, #0x50
	mov r0, sb
	subs r0, #1
	lsls r0, r0, #8
	subs r1, r1, r0
	str r1, [r2, #4]
	ldr r3, _0800D938 @ =gUnknown_030012D8
	ldr r0, [r3]
	movs r1, #0
	str r1, [r0, #0x64]
	ldr r1, [sp, #0x50]
	ldr r2, [r2, #4]
	bl sub_8007398
	ldr r4, _0800D938 @ =gUnknown_030012D8
	ldr r0, [r4]
	movs r5, #0x84
	lsls r5, r5, #1
	adds r0, r0, r5
	movs r1, #1
	strb r1, [r0, #4]
	ldr r1, [r4]
	ldr r0, [r1, #0x74]
	ldr r6, [sp, #0x84]
	orrs r0, r6
	str r0, [r1, #0x74]
	b _0800E074
	.align 2, 0
_0800D938: .4byte gUnknown_030012D8
_0800D93C:
	cmp r5, #6
	bgt _0800D9B4
_0800D940:
	mov r0, sb
	cmp r0, #2
	ble _0800D9B4
	ldr r2, _0800D964 @ =gUnknown_030012D8
	ldr r1, [r2]
	ldr r0, [r1]
	str r0, [sp, #0x58]
	ldr r1, [r1, #4]
	add r0, sp, #0x58
	str r1, [r0, #4]
	adds r2, r0, #0
	ldr r3, [sp, #0x80]
	cmp r3, #2
	bne _0800D968
	lsls r0, r5, #8
	ldr r1, [sp, #0x58]
	adds r0, r0, r1
	b _0800D974
	.align 2, 0
_0800D964: .4byte gUnknown_030012D8
_0800D968:
	ldr r4, [sp, #0x80]
	cmp r4, #1
	bne _0800D976
	lsls r1, r5, #8
	ldr r0, [sp, #0x58]
	subs r0, r0, r1
_0800D974:
	str r0, [sp, #0x58]
_0800D976:
	ldr r5, _0800D9B0 @ =gUnknown_030012D8
	ldr r0, [r5]
	ldr r1, [sp, #0x58]
	ldr r2, [r2, #4]
	bl sub_8007398
	ldr r0, [r5]
	movs r6, #0x84
	lsls r6, r6, #1
	adds r0, r0, r6
	movs r1, #1
	strb r1, [r0, #4]
	ldr r0, [r5]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xc
	ldr r3, [sp, #0x80]
	bl sub_803AD88
	ldr r1, [r5]
	ldr r0, [r1, #0x74]
	ldr r4, [sp, #0x80]
	b _0800DA28
	.align 2, 0
_0800D9B0: .4byte gUnknown_030012D8
_0800D9B4:
	ldr r5, _0800D9E0 @ =gUnknown_030012D8
	ldr r1, [r5]
	adds r0, r1, #0
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #1
	beq _0800D9C4
	b _0800E074
_0800D9C4:
	ldr r0, [r1]
	str r0, [sp, #0x60]
	ldr r1, [r1, #4]
	add r0, sp, #0x60
	str r1, [r0, #4]
	adds r2, r0, #0
	ldr r6, [sp, #0x84]
	cmp r6, #4
	bne _0800D9E4
	mov r3, sb
	lsls r0, r3, #8
	adds r0, r0, r1
	b _0800D9F0
	.align 2, 0
_0800D9E0: .4byte gUnknown_030012D8
_0800D9E4:
	ldr r4, [sp, #0x80]
	cmp r4, #8
	bne _0800D9F2
	mov r5, sb
	lsls r0, r5, #8
	subs r0, r1, r0
_0800D9F0:
	str r0, [r2, #4]
_0800D9F2:
	ldr r6, _0800DA30 @ =gUnknown_030012D8
	ldr r0, [r6]
	ldr r1, [sp, #0x60]
	ldr r2, [r2, #4]
	bl sub_8007398
	ldr r0, [r6]
	movs r1, #0x84
	lsls r1, r1, #1
	adds r0, r0, r1
	movs r1, #1
	strb r1, [r0, #4]
	ldr r0, [r6]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xc
	ldr r3, [sp, #0x84]
	bl sub_803AD88
	ldr r1, [r6]
	ldr r0, [r1, #0x74]
	ldr r4, [sp, #0x84]
_0800DA28:
	orrs r0, r4
	str r0, [r1, #0x74]
	b _0800E074
	.align 2, 0
_0800DA30: .4byte gUnknown_030012D8
_0800DA34:
	mov r0, sl
	b _0800DA58
_0800DA38:
	adds r0, r2, #0
	adds r0, #0x4e
	ldrb r0, [r0]
	adds r0, r0, r4
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800DA56
	adds r1, r2, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800DA56
	b _0800E074
_0800DA56:
	adds r0, r2, #0
_0800DA58:
	bl sub_8010708
	adds r2, r0, #0
	cmp r2, #0
	bne _0800DA38
	ldr r1, _0800DA80 @ =gStaticData_0816BC98
	ldrb r5, [r7]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r1, #0x14
	adds r0, r0, r1
	ldr r6, [r0]
	movs r5, #0
	mov sb, r5
	movs r0, #0
	str r0, [sp, #0x80]
	movs r1, #0
	str r1, [sp, #0x84]
	b _0800DD20
	.align 2, 0
_0800DA80: .4byte gStaticData_0816BC98
_0800DA84:
	ldr r0, [r6]
	bl sub_8009EC4
	adds r4, r0, #0
	ldr r0, [r6]
	bl sub_8009EBC
	adds r7, r0, #0
	movs r2, #2
	mov ip, r2
	ldr r3, [sp, #0x70]
	cmp r3, r4
	ble _0800DAA2
	movs r5, #1
	mov ip, r5
_0800DAA2:
	ldr r0, [r6]
	ldr r1, [r0]
	asrs r1, r1, #8
	mov r6, sl
	ldr r0, [r6]
	asrs r0, r0, #8
	cmp r1, r0
	bge _0800DAC8
	movs r0, #1
	str r0, [sp, #0x80]
	ldr r0, [sp, #0x3c]
	ldr r1, [sp, #0x44]
	ldr r2, [sp, #0x1c]
	adds r1, r0, r1
	subs r1, r1, r2
	adds r5, r1, #1
	adds r6, r2, #0
	adds r3, r0, #0
	b _0800DADC
_0800DAC8:
	movs r1, #2
	str r1, [sp, #0x80]
	ldr r0, [sp, #0x1c]
	ldr r1, [sp, #0x24]
	ldr r2, [sp, #0x3c]
	adds r1, r0, r1
	subs r1, r1, r2
	adds r5, r1, #1
	adds r6, r0, #0
	adds r3, r2, #0
_0800DADC:
	ldr r0, _0800DB04 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r1, [r0, #4]
	asrs r1, r1, #8
	mov r2, sl
	ldr r0, [r2, #4]
	asrs r0, r0, #8
	cmp r1, r0
	ble _0800DB08
	movs r0, #4
	str r0, [sp, #0x84]
	ldr r0, [sp, #0x20]
	ldr r1, [sp, #0x28]
	ldr r2, [sp, #0x40]
	adds r1, r0, r1
	subs r1, r1, r2
	mov sb, r1
	adds r2, r0, #0
	b _0800DB18
	.align 2, 0
_0800DB04: .4byte gUnknown_030012D8
_0800DB08:
	movs r1, #8
	str r1, [sp, #0x84]
	ldr r0, [sp, #0x40]
	ldr r1, [sp, #0x48]
	ldr r2, [sp, #0x20]
	adds r0, r0, r1
	subs r0, r0, r2
	mov sb, r0
_0800DB18:
	ldr r0, [sp, #0x74]
	cmp r7, r0
	bne _0800DB6C
	ldr r1, [sp, #0x70]
	cmp r4, r1
	bne _0800DB3E
	mov r2, sb
	cmp r2, #2
	ble _0800DB2C
	b _0800DD1A
_0800DB2C:
	movs r3, #4
	str r3, [sp, #0x7c]
	ldr r4, [sp, #0x88]
	cmp r4, #0
	bne _0800DB38
	b _0800DD1E
_0800DB38:
	movs r6, #8
	str r6, [sp, #0x7c]
	b _0800DD1E
_0800DB3E:
	mov r1, sb
	cmp r1, #2
	ble _0800DB46
	b _0800DD14
_0800DB46:
	cmp r5, #3
	bgt _0800DB5A
	ldr r0, _0800DB68 @ =gUnknown_030012D8
	ldr r0, [r0]
	bl sub_800B324
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0800DB5A
	b _0800DD14
_0800DB5A:
	movs r2, #4
	str r2, [sp, #0x7c]
	ldr r3, [sp, #0x88]
	cmp r3, #0
	bne _0800DB66
	b _0800DD1E
_0800DB66:
	b _0800DC54
	.align 2, 0
_0800DB68: .4byte gUnknown_030012D8
_0800DB6C:
	ldr r0, [sp, #0x70]
	cmp r4, r0
	bne _0800DB92
	ldr r1, [sp, #0x84]
	str r1, [sp, #0x7c]
	mov r2, sb
	cmp r2, #2
	bgt _0800DB7E
	b _0800DD1E
_0800DB7E:
	ldr r3, [sp, #0x80]
	str r3, [sp, #0x7c]
	cmp r2, #7
	ble _0800DB88
	b _0800DD1E
_0800DB88:
	cmp r5, #3
	bgt _0800DB8E
	b _0800DD1E
_0800DB8E:
	str r1, [sp, #0x7c]
	b _0800DD1E
_0800DB92:
	ldr r0, [sp, #0x74]
	cmp r7, r0
	bgt _0800DC5A
	ldr r1, [sp, #0x88]
	cmp r1, #0
	beq _0800DC5A
	mov r0, r8
	movs r1, #2
	ldrsh r0, [r0, r1]
	str r0, [sp, #0x9c]
	mov r0, r8
	ldrb r1, [r0, #5]
	ldr r0, [sp, #0x9c]
	adds r0, r0, r1
	str r0, [sp, #0x9c]
	adds r7, r7, r0
	ldr r0, [sp, #0x28]
	adds r0, r2, r0
	str r1, [sp, #0xa0]
	cmp r7, r0
	ble _0800DBBE
	b _0800DD1A
_0800DBBE:
	movs r1, #0
	str r1, [sp, #0x7c]
	mov r2, ip
	cmp r2, #1
	bne _0800DBDA
	ldr r0, [sp, #0x44]
	adds r0, r3, r0
	cmp r0, r6
	blt _0800DBEA
	cmp r5, #4
	ble _0800DBEA
	movs r3, #8
	str r3, [sp, #0x7c]
	b _0800DD1E
_0800DBDA:
	ldr r0, [sp, #0x24]
	adds r0, r6, r0
	cmp r3, r0
	bgt _0800DBEA
	cmp r5, #4
	ble _0800DBEA
	movs r0, #8
	str r0, [sp, #0x7c]
_0800DBEA:
	ldr r1, [sp, #0x7c]
	cmp r1, #0
	beq _0800DBF2
	b _0800DD1E
_0800DBF2:
	mov r2, r8
	movs r1, #2
	ldrsh r0, [r2, r1]
	ldr r2, [sp, #0xa0]
	adds r0, r0, r2
	ldr r1, [sp, #0x74]
	adds r1, r1, r0
	str r1, [sp, #0x74]
	ldr r2, [sp, #0x80]
	cmp r2, #1
	bne _0800DC1E
	mov r1, r8
	movs r2, #0
	ldrsh r0, [r1, r2]
	ldrb r1, [r1, #4]
	adds r0, r1, r0
	adds r4, r4, r0
	ldr r0, [sp, #0x44]
	adds r3, r3, r0
	str r3, [sp, #0x70]
	str r6, [sp]
	b _0800DC2E
_0800DC1E:
	mov r2, r8
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r4, r4, r0
	str r3, [sp, #0x70]
	ldr r0, [sp, #0x24]
	adds r0, r6, r0
	str r0, [sp]
_0800DC2E:
	adds r0, r4, #0
	adds r1, r7, #0
	adds r2, r3, #0
	ldr r3, [sp, #0x74]
	bl sub_800FDC8
	adds r2, r0, #0
	cmp r2, #0
	bge _0800DC4A
	ldr r3, [sp, #0x88]
	cmp r3, #0
	beq _0800DC4A
	cmp r5, #4
	bgt _0800DC54
_0800DC4A:
	cmp r2, #0
	ble _0800DD14
	ldr r0, [sp, #0x20]
	cmp r2, r0
	bgt _0800DD14
_0800DC54:
	movs r4, #8
	str r4, [sp, #0x7c]
	b _0800DD1E
_0800DC5A:
	mov r1, r8
	movs r0, #2
	ldrsh r1, [r1, r0]
	adds r7, r7, r1
	cmp r7, r2
	blt _0800DD1A
	movs r1, #0
	str r1, [sp, #0x7c]
	mov r2, ip
	cmp r2, #1
	bne _0800DC82
	ldr r0, [sp, #0x44]
	adds r0, r3, r0
	cmp r0, r6
	blt _0800DC92
	cmp r5, #5
	ble _0800DC92
	movs r3, #4
	str r3, [sp, #0x7c]
	b _0800DD1E
_0800DC82:
	ldr r0, [sp, #0x24]
	adds r0, r6, r0
	cmp r3, r0
	bgt _0800DC92
	cmp r5, #5
	ble _0800DC92
	movs r0, #4
	str r0, [sp, #0x7c]
_0800DC92:
	ldr r1, [sp, #0x7c]
	cmp r1, #0
	bne _0800DD1E
	mov r2, r8
	movs r1, #2
	ldrsh r0, [r2, r1]
	ldr r2, [sp, #0x74]
	adds r2, r2, r0
	str r2, [sp, #0x74]
	ldr r0, [sp, #0x80]
	cmp r0, #1
	bne _0800DCC0
	mov r1, r8
	movs r2, #0
	ldrsh r0, [r1, r2]
	ldrb r1, [r1, #4]
	adds r0, r1, r0
	adds r4, r4, r0
	ldr r0, [sp, #0x44]
	adds r3, r3, r0
	str r3, [sp, #0x70]
	str r6, [sp]
	b _0800DCD0
_0800DCC0:
	mov r2, r8
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r4, r4, r0
	str r3, [sp, #0x70]
	ldr r0, [sp, #0x24]
	adds r0, r6, r0
	str r0, [sp]
_0800DCD0:
	adds r0, r4, #0
	adds r1, r7, #0
	adds r2, r3, #0
	ldr r3, [sp, #0x74]
	bl sub_800FDC8
	adds r2, r0, #0
	ldr r0, _0800DD10 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #1
	bne _0800DCEC
	adds r2, #2
_0800DCEC:
	cmp r2, #0
	bge _0800DCFA
	ldr r3, [sp, #0x88]
	cmp r3, #0
	bne _0800DCFA
	cmp r5, #5
	bgt _0800DD08
_0800DCFA:
	cmp r2, #0
	ble _0800DD14
	ldr r0, [sp, #0x20]
	ldr r1, [sp, #0x28]
	adds r0, r0, r1
	cmp r2, r0
	blt _0800DD14
_0800DD08:
	movs r4, #4
	str r4, [sp, #0x7c]
	b _0800DD1E
	.align 2, 0
_0800DD10: .4byte gUnknown_030012D8
_0800DD14:
	ldr r6, [sp, #0x80]
	str r6, [sp, #0x7c]
	b _0800DD1E
_0800DD1A:
	ldr r0, [sp, #0x80]
	str r0, [sp, #0x7c]
_0800DD1E:
	movs r6, #0
_0800DD20:
	ldr r0, _0800DD64 @ =gUnknown_030012D8
	ldr r1, [r0]
	ldr r0, [r1]
	str r0, [sp, #0x68]
	ldr r1, [r1, #4]
	add r0, sp, #0x68
	str r1, [r0, #4]
	adds r7, r0, #0
	cmp r5, #0
	bge _0800DD36
	movs r5, #0
_0800DD36:
	mov r1, sb
	cmp r1, #0
	bge _0800DD40
	movs r2, #0
	mov sb, r2
_0800DD40:
	movs r3, #0
	str r3, [sp, #0x8c]
	ldr r0, _0800DD68 @ =gStaticData_0816BF00
	ldr r4, [sp, #0x78]
	adds r0, r4, r0
	ldrb r0, [r0]
	str r0, [sp, #0x90]
	mov r8, sl
	ldr r0, [sp, #0x7c]
	cmp r0, #8
	bls _0800DD58
	b _0800E00C
_0800DD58:
	lsls r0, r0, #2
	ldr r1, _0800DD6C @ =_0800DD70
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800DD64: .4byte gUnknown_030012D8
_0800DD68: .4byte gStaticData_0816BF00
_0800DD6C: .4byte _0800DD70
_0800DD70: @ jump table
	.4byte _0800E00C @ case 0
	.4byte _0800DEAC @ case 1
	.4byte _0800DEAC @ case 2
	.4byte _0800E00C @ case 3
	.4byte _0800DD94 @ case 4
	.4byte _0800E00C @ case 5
	.4byte _0800E00C @ case 6
	.4byte _0800E00C @ case 7
	.4byte _0800DE14 @ case 8
_0800DD94:
	mov r0, sl
	bl sub_801095C
	mov r8, r0
	ldr r2, _0800DE0C @ =gStaticData_0816BC98
	adds r0, #0x4e
	ldrb r1, [r0]
	lsls r0, r1, #3
	subs r0, r0, r1
	ldr r3, [sp, #0x78]
	adds r0, r0, r3
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r6, [r0]
	cmp r1, #4
	bne _0800DDBA
	cmp r3, #2
	bne _0800DDBA
	movs r6, #3
_0800DDBA:
	ldr r4, [sp, #0x78]
	cmp r4, #3
	ble _0800DDD0
	cmp r4, #6
	beq _0800DDD0
	cmp r4, #4
	beq _0800DDCA
	b _0800E00C
_0800DDCA:
	cmp r6, #2
	ble _0800DDD0
	b _0800E00C
_0800DDD0:
	ldr r5, _0800DE10 @ =gUnknown_030012D8
	ldr r0, [r5]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xc
	movs r3, #4
	bl sub_803AD88
	ldr r1, [r5]
	movs r2, #4
	ldr r0, [r1, #0x74]
	orrs r0, r2
	str r0, [r1, #0x74]
	adds r1, #0x68
	ldrb r1, [r1]
	cmp r1, #8
	bne _0800DDFE
	b _0800E00C
_0800DDFE:
	mov r4, sb
	lsls r0, r4, #8
	ldr r1, [r7, #4]
	adds r0, r0, r1
	str r0, [r7, #4]
	b _0800E00C
	.align 2, 0
_0800DE0C: .4byte gStaticData_0816BC98
_0800DE10: .4byte gUnknown_030012D8
_0800DE14:
	mov r0, sl
	bl sub_8010914
	mov r8, r0
	adds r0, #0x4e
	ldrb r2, [r0]
	ldr r1, _0800DE80 @ =gStaticData_0816BC98
	lsls r0, r2, #3
	subs r0, r0, r2
	ldr r5, [sp, #0x78]
	adds r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r6, [r0]
	cmp r5, #4
	bne _0800DE52
	cmp r2, #0xa
	beq _0800DE52
	ldr r0, _0800DE84 @ =gUnknown_030012D8
	ldr r1, [r0]
	adds r0, r1, #0
	adds r0, #0x94
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800DE52
	movs r0, #0
	str r0, [r1, #0x64]
	str r0, [r1, #0x54]
	str r0, [r1, #0x58]
	str r0, [r1, #0x5c]
	movs r6, #1
_0800DE52:
	subs r0, r6, #1
	cmp r0, #1
	bhi _0800DE8C
	mov r0, sb
	subs r0, #1
	lsls r0, r0, #8
	ldr r2, [r7, #4]
	subs r2, r2, r0
	ldr r0, _0800DE88 @ =0xFFFFFF00
	ands r2, r0
	str r2, [r7, #4]
	ldr r4, _0800DE84 @ =gUnknown_030012D8
	ldr r0, [r4]
	ldr r1, [sp, #0x68]
	bl sub_8007398
	ldr r0, [r4]
	movs r1, #0x84
	lsls r1, r1, #1
	adds r0, r0, r1
	movs r1, #1
	strb r1, [r0, #4]
	b _0800DE9E
	.align 2, 0
_0800DE80: .4byte gStaticData_0816BC98
_0800DE84: .4byte gUnknown_030012D8
_0800DE88: .4byte 0xFFFFFF00
_0800DE8C:
	cmp r6, #0
	beq _0800DE94
	cmp r6, #2
	bne _0800DE9E
_0800DE94:
	mov r2, sb
	lsls r1, r2, #8
	ldr r0, [r7, #4]
	subs r0, r0, r1
	str r0, [r7, #4]
_0800DE9E:
	ldr r0, [r7, #4]
	ldr r1, _0800DEA8 @ =0xFFFFFF00
	ands r0, r1
	str r0, [r7, #4]
	b _0800E00C
	.align 2, 0
_0800DEA8: .4byte 0xFFFFFF00
_0800DEAC:
	ldr r3, [sp, #0x80]
	str r3, [sp, #0x8c]
	add r0, sp, #0x2c
	add r1, sp, #0x3c
	bl sub_8001640
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0800DECC
	movs r6, #0
	mov r0, sl
	bl sub_800E494
	movs r4, #0
	str r4, [sp, #0x8c]
	b _0800DF9A
_0800DECC:
	movs r0, #0x7f
	ldr r1, [sp, #0x94]
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800DEF4
	ldr r2, [sp, #0x8c]
	cmp r2, #2
	bne _0800DEE6
	lsls r1, r5, #8
	ldr r0, [sp, #0x68]
	adds r0, r0, r1
	b _0800DEF2
_0800DEE6:
	ldr r3, [sp, #0x8c]
	cmp r3, #1
	bne _0800DEF4
	lsls r1, r5, #8
	ldr r0, [sp, #0x68]
	subs r0, r0, r1
_0800DEF2:
	str r0, [sp, #0x68]
_0800DEF4:
	ldr r4, [sp, #0x78]
	cmp r4, #2
	ble _0800DF34
	ldr r1, _0800DF2C @ =gStaticData_0816BC98
	mov r2, sl
	adds r2, #0x4e
	ldrb r5, [r2]
	lsls r0, r5, #3
	subs r0, r0, r5
	adds r0, r0, r4
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r6, [r0]
	cmp r4, #4
	bne _0800DF18
	cmp r6, #2
	bne _0800DF18
	movs r6, #0
_0800DF18:
	ldr r0, [sp, #0x78]
	cmp r0, #5
	bne _0800DF9A
	cmp r6, #3
	bne _0800DF9A
	ldr r0, _0800DF30 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0]
	str r0, [sp, #0x68]
	b _0800DF9A
	.align 2, 0
_0800DF2C: .4byte gStaticData_0816BC98
_0800DF30: .4byte gUnknown_030012D8
_0800DF34:
	mov r1, sb
	cmp r1, #4
	bgt _0800DF68
	cmp r5, #3
	ble _0800DF68
	ldr r2, [sp, #0x88]
	cmp r2, #0
	beq _0800DF68
	ldr r1, _0800DF64 @ =gStaticData_0816BC98
	mov r2, sl
	adds r2, #0x4e
	ldrb r3, [r2]
	lsls r0, r3, #3
	subs r0, r0, r3
	ldr r4, [sp, #0x78]
	adds r0, r0, r4
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r6, [r0]
	cmp r6, #1
	ble _0800DF9A
	movs r6, #0
	b _0800DF9A
	.align 2, 0
_0800DF64: .4byte gStaticData_0816BC98
_0800DF68:
	ldr r3, _0800E084 @ =gStaticData_0816BC98
	ldr r5, [sp, #0x78]
	lsls r2, r5, #2
	mov r1, sl
	adds r1, #0x4e
	ldrb r4, [r1]
	lsls r0, r4, #3
	subs r0, r0, r4
	lsls r0, r0, #2
	adds r0, r2, r0
	adds r0, r0, r3
	ldr r0, [r0]
	cmp r0, #4
	bne _0800DF9A
	ldr r0, _0800E088 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0]
	str r0, [sp, #0x68]
	ldrb r5, [r1]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r2, r0
	adds r0, r0, r3
	ldr r6, [r0]
_0800DF9A:
	cmp r6, #1
	beq _0800E00C
	ldr r0, [sp, #0x8c]
	cmp r0, #0
	beq _0800E00C
	movs r5, #1
	mov r0, sl
	bl sub_801070C
	adds r4, r0, #0
	mov r0, sl
	bl sub_8010708
	adds r2, r0, #0
	ldr r0, _0800E088 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0, #0x64]
	asrs r1, r0, #8
	ldr r3, [sp, #0x84]
	cmp r3, #8
	bne _0800DFD6
	cmp r4, #0
	bne _0800DFD6
	mov r0, sb
	subs r0, #1
	cmp r1, r0
	bge _0800E00C
	mov r4, sb
	cmp r4, #2
	ble _0800E00C
_0800DFD6:
	ldr r0, [sp, #0x84]
	cmp r0, #4
	bne _0800DFF0
	cmp r2, #0
	bne _0800DFF0
	mov r0, sb
	subs r0, #1
	cmp r1, r0
	bge _0800DFEE
	mov r1, sb
	cmp r1, #2
	bgt _0800DFF0
_0800DFEE:
	movs r5, #0
_0800DFF0:
	cmp r5, #0
	beq _0800E00C
	ldr r4, _0800E088 @ =gUnknown_030012D8
	ldr r0, [r4]
	ldr r1, [sp, #0x68]
	ldr r2, [r7, #4]
	bl sub_8007398
	ldr r0, [r4]
	movs r2, #0x84
	lsls r2, r2, #1
	adds r0, r0, r2
	movs r1, #1
	strb r1, [r0, #4]
_0800E00C:
	ldr r0, _0800E088 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #1
	bne _0800E03C
	mov r0, sl
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #0xe
	bne _0800E03C
	cmp r6, #1
	bgt _0800E03C
	add r0, sp, #0x2c
	add r1, sp, #0x3c
	bl sub_8001640
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #1
	bne _0800E03C
	mov r0, r8
	bl sub_800E620
_0800E03C:
	ldr r0, _0800E088 @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r3, #0x84
	lsls r3, r3, #1
	adds r0, r0, r3
	ldr r4, [sp, #0x7c]
	str r4, [sp]
	mov r5, sb
	str r5, [sp, #4]
	ldr r1, [sp, #0x68]
	ldr r2, [sp, #0x6c]
	str r1, [sp, #8]
	str r2, [sp, #0xc]
	ldr r1, [sp, #0x8c]
	str r1, [sp, #0x10]
	add r1, sp, #0x14
	add r2, sp, #0x90
	ldrb r2, [r2]
	strb r2, [r1]
	add r1, sp, #0x18
	add r3, sp, #0x88
	ldrb r3, [r3]
	strb r3, [r1]
	mov r1, r8
	ldr r2, [sp, #0x78]
	adds r3, r6, #0
	bl sub_8010D54
_0800E074:
	add sp, #0xa4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800E084: .4byte gStaticData_0816BC98
_0800E088: .4byte gUnknown_030012D8

	thumb_func_start sub_800E08C
sub_800E08C: @ 0x0800E08C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xc
	adds r6, r0, #0
	mov r8, r1
	adds r7, r2, #0
	mov sb, r3
	add r0, sp, #0x3c
	add r1, sp, #0x40
	add r2, sp, #0x44
	ldrb r0, [r0]
	str r0, [sp]
	ldrb r1, [r1]
	str r1, [sp, #4]
	ldrb r2, [r2]
	str r2, [sp, #8]
	adds r1, r6, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _0800E0C2
	b _0800E43C
_0800E0C2:
	ldr r0, _0800E13C @ =gUnknown_030012D8
	ldr r2, [r0]
	adds r1, r2, #0
	adds r1, #0x88
	mov sl, r0
	ldrb r1, [r1]
	cmp r1, #1
	bne _0800E0DE
	cmp r7, #2
	ble _0800E0DE
	ldr r0, [r2]
	str r0, [sp, #0x30]
	ldr r0, [r2, #4]
	str r0, [sp, #0x34]
_0800E0DE:
	subs r0, r7, #2
	cmp r0, #1
	bls _0800E0E8
	cmp r7, #5
	bne _0800E1D2
_0800E0E8:
	adds r0, r6, #0
	adds r0, #0x4e
	ldrb r1, [r0]
	mov r0, sl
	ldr r2, [r0]
	adds r0, r2, #0
	adds r0, #0x24
	movs r5, #4
	ldrb r0, [r0]
	ands r5, r0
	cmp r5, #0
	bne _0800E1D2
	cmp r1, #0xd
	beq _0800E1B8
	mov r3, r8
	cmp r3, #2
	bne _0800E170
	cmp r1, #4
	beq _0800E112
	cmp r1, #8
	bne _0800E144
_0800E112:
	ldr r0, _0800E140 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #2
	bl PlaySfx
	mov r5, sl
	ldr r0, [r5]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xe
	movs r3, #8
	bl sub_803AD88
	b _0800E15A
	.align 2, 0
_0800E13C: .4byte gUnknown_030012D8
_0800E140: .4byte gUnknown_030012BC
_0800E144:
	ldr r1, [r2, #0x18]
	adds r1, #0x68
	movs r5, #0
	ldrsh r0, [r1, r5]
	adds r0, r2, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xd
	movs r3, #8
	bl sub_803AD88
_0800E15A:
	ldr r0, _0800E16C @ =gUnknown_030012D8
	ldr r1, [r0]
	movs r0, #0
	str r0, [r1, #0x64]
	str r0, [r1, #0x54]
	str r0, [r1, #0x58]
	str r0, [r1, #0x5c]
	b _0800E1D2
	.align 2, 0
_0800E16C: .4byte gUnknown_030012D8
_0800E170:
	mov r0, r8
	subs r0, #5
	cmp r0, #1
	bhi _0800E1D2
	cmp r1, #8
	bne _0800E1D2
	ldr r0, _0800E1B4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #2
	bl PlaySfx
	mov r1, sl
	ldr r0, [r1]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xe
	movs r3, #8
	bl sub_803AD88
	mov r1, sl
	ldr r0, [r1]
	str r5, [r0, #0x64]
	str r5, [r0, #0x54]
	str r5, [r0, #0x58]
	str r5, [r0, #0x5c]
	b _0800E1D2
	.align 2, 0
_0800E1B4: .4byte gUnknown_030012BC
_0800E1B8:
	cmp r7, #2
	bne _0800E1D2
	adds r1, r6, #0
	adds r1, #0x4d
	movs r0, #0x80
	ldrb r3, [r1]
	orrs r0, r3
	strb r0, [r1]
	movs r1, #1
	adds r0, r2, #0
	adds r0, #0x80
	strb r1, [r0]
	movs r7, #1
_0800E1D2:
	cmp r7, #3
	bne _0800E202
	adds r2, r6, #0
	adds r2, #0x4e
	ldrb r5, [r2]
	cmp r5, #0xf
	bne _0800E202
	ldr r0, [r6, #0x48]
	movs r1, #7
	ands r0, r1
	cmp r0, #3
	bne _0800E202
	movs r0, #0xe
	movs r1, #0
	strb r0, [r2]
	str r1, [r6, #0x48]
	ldr r1, _0800E270 @ =gStaticData_0816BC98
	ldrb r3, [r2]
	lsls r0, r3, #3
	subs r0, r0, r3
	add r0, r8
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r7, [r0]
_0800E202:
	ldr r2, [sp, #8]
	cmp r7, #1
	bne _0800E260
	mov r5, r8
	cmp r5, #4
	bne _0800E260
	ldr r3, _0800E274 @ =gUnknown_030012D8
	ldr r1, [r3]
	adds r4, r1, #0
	adds r4, #0x92
	ldrb r0, [r4]
	cmp r0, #1
	bne _0800E260
	adds r1, #0x24
	movs r0, #0xc
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800E260
	ldr r1, _0800E270 @ =gStaticData_0816BC98
	adds r2, r6, #0
	adds r2, #0x4e
	ldrb r5, [r2]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, #0x10
	adds r0, r0, r1
	ldr r7, [r0]
	movs r0, #2
	strb r0, [r4]
	ldr r0, [r3]
	adds r0, #0x92
	ldrb r1, [r0]
	adds r1, #1
	strb r1, [r0]
	ldr r0, [r3]
	adds r0, #0x92
	ldrb r1, [r0]
	adds r1, #1
	strb r1, [r0]
	ldr r0, [r3]
	adds r0, #0x92
	ldrb r1, [r0]
	adds r1, #1
	strb r1, [r0]
	movs r2, #1
_0800E260:
	cmp r7, #5
	bls _0800E266
	b _0800E43C
_0800E266:
	lsls r0, r7, #2
	ldr r1, _0800E278 @ =_0800E27C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800E270: .4byte gStaticData_0816BC98
_0800E274: .4byte gUnknown_030012D8
_0800E278: .4byte _0800E27C
_0800E27C: @ jump table
	.4byte _0800E294 @ case 0
	.4byte _0800E294 @ case 1
	.4byte _0800E342 @ case 2
	.4byte _0800E37C @ case 3
	.4byte _0800E41C @ case 4
	.4byte _0800E434 @ case 5
_0800E294:
	ldr r0, _0800E330 @ =gUnknown_030012D8
	ldr r1, [r0]
	adds r1, #0x24
	movs r2, #4
	ldrb r1, [r1]
	ands r2, r1
	mov sl, r0
	cmp r2, #0
	bne _0800E2FE
	ldr r0, [sp, #4]
	cmp r0, #0
	beq _0800E2FE
	mov r1, sb
	cmp r1, #8
	bne _0800E2B8
	ldr r2, [sp, #0x2c]
	cmp r2, #1
	ble _0800E2D4
_0800E2B8:
	ldr r3, [sp, #0x2c]
	cmp r3, #1
	bgt _0800E2C2
	cmp r7, #1
	beq _0800E2D4
_0800E2C2:
	ldr r5, [sp, #0x2c]
	cmp r5, #7
	bgt _0800E2FE
	cmp r7, #1
	beq _0800E2CE
	b _0800E43C
_0800E2CE:
	mov r0, sb
	cmp r0, #8
	bne _0800E2FE
_0800E2D4:
	mov r1, sl
	ldr r0, [r1]
	adds r1, r0, #0
	adds r1, #0xac
	str r6, [r1]
	movs r1, #8
	adds r0, #0x68
	strb r1, [r0]
	mov r2, sl
	ldr r0, [r2]
	ldr r1, [r0, #4]
	ldr r0, [sp, #0x2c]
	subs r0, #1
	lsls r0, r0, #8
	subs r1, r1, r0
	str r1, [sp, #0x34]
	movs r3, #0
	str r3, [sp, #0x38]
	ldr r0, [r2]
	ldr r0, [r0]
	str r0, [sp, #0x30]
_0800E2FE:
	cmp r7, #1
	beq _0800E304
	b _0800E43C
_0800E304:
	mov r0, sb
	subs r0, #1
	cmp r0, #1
	bhi _0800E31E
	mov r5, r8
	cmp r5, #1
	bgt _0800E31E
	movs r0, #0
	str r0, [sp, #0x38]
	ldr r0, _0800E330 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0]
	str r0, [sp, #0x30]
_0800E31E:
	adds r0, r6, #0
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #6
	bne _0800E334
	adds r0, r6, #0
	bl sub_800F2BC
	b _0800E43C
	.align 2, 0
_0800E330: .4byte gUnknown_030012D8
_0800E334:
	cmp r0, #3
	beq _0800E33A
	b _0800E43C
_0800E33A:
	adds r0, r6, #0
	bl sub_800F368
	b _0800E43C
_0800E342:
	adds r0, r6, #0
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #0xe
	bne _0800E354
	adds r0, r6, #0
	bl sub_800E620
	b _0800E43C
_0800E354:
	cmp r0, #0xc
	bne _0800E360
	adds r0, r6, #0
	bl sub_800E560
	b _0800E43C
_0800E360:
	adds r1, r6, #0
	adds r1, #0x4d
	movs r0, #0x80
	ldrb r2, [r1]
	orrs r0, r2
	strb r0, [r1]
	ldr r0, _0800E378 @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r1, #1
	adds r0, #0x80
	strb r1, [r0]
	b _0800E43C
	.align 2, 0
_0800E378: .4byte gUnknown_030012D8
_0800E37C:
	mov r0, r8
	subs r0, #5
	cmp r0, #1
	bhi _0800E392
	adds r0, r6, #0
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_800E7A8
	b _0800E47E
_0800E392:
	ldr r0, [r6, #0x44]
	cmp r0, #0
	beq _0800E3A6
	adds r0, r6, #0
	movs r1, #0
	movs r2, #0
	movs r3, #4
	bl sub_800E7A8
	b _0800E47E
_0800E3A6:
	mov r3, r8
	cmp r3, #2
	bne _0800E3BC
	adds r0, r6, #0
	movs r1, #0
	mov r5, sp
	ldrb r2, [r5]
	mov r3, sb
	bl sub_800E7A8
	b _0800E47E
_0800E3BC:
	ldr r4, _0800E418 @ =gUnknown_030012D8
	ldr r0, [r4]
	adds r0, #0x94
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800E3CC
	cmp r2, #0
	beq _0800E47E
_0800E3CC:
	mov r0, sb
	cmp r0, #8
	beq _0800E3D6
	cmp r0, #4
	bne _0800E47E
_0800E3D6:
	adds r0, r6, #0
	movs r1, #0
	movs r2, #0
	mov r3, sb
	bl sub_800E7A8
	ldr r2, [r4]
	adds r0, r2, #0
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #0
	bne _0800E400
	adds r0, r2, #0
	adds r0, #0x94
	ldrb r1, [r0]
	cmp r1, #4
	bhi _0800E400
	lsls r1, r1, #2
	adds r0, #4
	adds r0, r0, r1
	str r6, [r0]
_0800E400:
	ldr r0, _0800E418 @ =gUnknown_030012D8
	ldr r1, [r0]
	adds r0, r1, #0
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #0
	bne _0800E47E
	adds r1, #0x94
	ldrb r0, [r1]
	adds r0, #1
	strb r0, [r1]
	b _0800E47E
	.align 2, 0
_0800E418: .4byte gUnknown_030012D8
_0800E41C:
	adds r1, r6, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800E47E
	adds r0, r6, #0
	movs r1, #1
	bl sub_800EEF0
	b _0800E47E
_0800E434:
	adds r0, r6, #0
	bl sub_800E6B0
	b _0800E47E
_0800E43C:
	ldr r5, _0800E490 @ =gUnknown_030012D8
	ldr r3, [r5]
	movs r2, #0x84
	lsls r2, r2, #1
	adds r0, r3, r2
	ldrb r0, [r0, #4]
	cmp r0, #0
	bne _0800E456
	ldr r1, [sp, #0x30]
	ldr r2, [sp, #0x34]
	adds r0, r3, #0
	bl sub_8007398
_0800E456:
	ldr r3, [sp, #0x38]
	cmp r3, #0
	beq _0800E47E
	ldr r0, [r5]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xc
	ldr r3, [sp, #0x38]
	bl sub_803AD88
	ldr r1, [r5]
	ldr r0, [r1, #0x74]
	ldr r5, [sp, #0x38]
	orrs r0, r5
	str r0, [r1, #0x74]
_0800E47E:
	add sp, #0xc
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800E490: .4byte gUnknown_030012D8

	thumb_func_start sub_800E494
sub_800E494: @ 0x0800E494
	push {r4, lr}
	adds r4, r0, #0
	b _0800E4B0
_0800E49A:
	adds r0, r1, #0
	adds r0, #0x4d
	movs r2, #0x7f
	ldrb r0, [r0]
	ands r2, r0
	cmp r2, #0
	bne _0800E4AE
	adds r0, r1, #0
	adds r0, #0x58
	strb r2, [r0]
_0800E4AE:
	adds r0, r1, #0
_0800E4B0:
	bl sub_801070C
	adds r1, r0, #0
	cmp r1, #0
	bne _0800E49A
	adds r0, r4, #0
	b _0800E4D4
_0800E4BE:
	adds r0, r1, #0
	adds r0, #0x4d
	movs r2, #0x7f
	ldrb r0, [r0]
	ands r2, r0
	cmp r2, #0
	bne _0800E4D2
	adds r0, r1, #0
	adds r0, #0x58
	strb r2, [r0]
_0800E4D2:
	adds r0, r1, #0
_0800E4D4:
	bl sub_8010708
	adds r1, r0, #0
	cmp r1, #0
	bne _0800E4BE
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_800E4E4
sub_800E4E4: @ 0x0800E4E4
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	adds r4, r1, #0
	ldr r5, [r4, #0xc]
	bl sub_801070C
	adds r2, r0, #0
	cmp r2, #0
	beq _0800E524
	movs r7, #1
_0800E4F8:
	adds r1, r2, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800E518
	adds r0, r2, #0
	adds r0, #0x58
	strb r7, [r0]
	ldr r0, [r4, #4]
	subs r0, r0, r5
	str r0, [r4, #4]
	ldr r0, [r4, #0xc]
	adds r0, r0, r5
	str r0, [r4, #0xc]
_0800E518:
	adds r0, r2, #0
	bl sub_801070C
	adds r2, r0, #0
	cmp r2, #0
	bne _0800E4F8
_0800E524:
	adds r0, r6, #0
	bl sub_8010708
	adds r2, r0, #0
	cmp r2, #0
	beq _0800E558
	movs r6, #1
_0800E532:
	adds r1, r2, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800E54C
	adds r0, r2, #0
	adds r0, #0x58
	strb r6, [r0]
	ldr r0, [r4, #0xc]
	adds r0, r0, r5
	str r0, [r4, #0xc]
_0800E54C:
	adds r0, r2, #0
	bl sub_8010708
	adds r2, r0, #0
	cmp r2, #0
	bne _0800E532
_0800E558:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_800E560
sub_800E560: @ 0x0800E560
	push {r4, r5, r6, r7, lr}
	sub sp, #8
	adds r7, r0, #0
	ldr r1, [r7, #0x48]
	movs r0, #0x2a
	rsbs r0, r0, #0
	cmp r1, r0
	bne _0800E57E
	movs r0, #0xb4
	lsls r0, r0, #1
	str r0, [r7, #0x48]
	adds r1, r7, #0
	adds r1, #0x51
	movs r0, #0
	strb r0, [r1]
_0800E57E:
	ldr r0, [r7, #0x48]
	cmp r0, #0
	ble _0800E60C
	adds r3, r7, #0
	adds r3, #0x50
	ldrb r0, [r3]
	cmp r0, #0
	bne _0800E618
	adds r1, r7, #0
	adds r1, #0x51
	ldrb r0, [r1]
	adds r0, #1
	strb r0, [r1]
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #4
	bls _0800E5AE
	adds r0, r7, #0
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_800E7A8
	b _0800E5CE
_0800E5AE:
	adds r1, r7, #0
	adds r1, #0x4d
	movs r0, #0x80
	ldrb r2, [r1]
	orrs r0, r2
	strb r0, [r1]
	ldr r0, _0800E604 @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r1, #1
	adds r0, #0x80
	strb r1, [r0]
	adds r2, r7, #0
	adds r2, #0x4f
	movs r0, #6
	strb r0, [r2]
	strb r1, [r3]
_0800E5CE:
	ldr r1, [r7]
	asrs r1, r1, #8
	ldr r2, [r7, #4]
	asrs r2, r2, #8
	subs r2, #6
	ldr r6, _0800E608 @ =gUnknown_030012E4
	ldr r0, [r6]
	movs r3, #0xe
	str r3, [sp]
	add r5, sp, #4
	movs r4, #1
	strb r4, [r5]
	movs r3, #0
	bl sub_8025CA4
	ldr r1, [r7]
	asrs r1, r1, #8
	adds r1, #3
	ldr r2, [r7, #4]
	asrs r2, r2, #8
	ldr r0, [r6]
	movs r3, #0
	str r3, [sp]
	strb r4, [r5]
	bl sub_8025CA4
	b _0800E618
	.align 2, 0
_0800E604: .4byte gUnknown_030012D8
_0800E608: .4byte gUnknown_030012E4
_0800E60C:
	adds r0, r7, #0
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_800E7A8
_0800E618:
	add sp, #8
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_800E620
sub_800E620: @ 0x0800E620
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r1, r4, #0
	adds r1, #0x4e
	movs r0, #0x15
	strb r0, [r1]
	movs r0, #0x14
	adds r5, r4, #0
	adds r5, #0x2d
	strb r0, [r5]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	movs r0, #0x10
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _0800E6A4 @ =gUnknown_0300130C
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8009150
	ldr r0, [r4, #0x20]
	ldr r1, [r0]
	ldrb r2, [r5]
	lsls r0, r2, #3
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r1, r1, r0
	ldr r0, _0800E6A8 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldrb r1, [r1, #0x14]
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
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
	ldr r0, _0800E6AC @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x11
	bl PlaySfx
	adds r4, #0x4f
	movs r0, #0x3c
	strb r0, [r4]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0800E6A4: .4byte gUnknown_0300130C
_0800E6A8: .4byte gUnknown_030012B8
_0800E6AC: .4byte gUnknown_030012BC

	thumb_func_start sub_800E6B0
sub_800E6B0: @ 0x0800E6B0
	push {r4, r5, lr}
	sub sp, #8
	adds r4, r0, #0
	ldr r3, [r4]
	asrs r3, r3, #8
	subs r3, #0xa
	ldr r1, [r4, #4]
	asrs r1, r1, #8
	ldr r0, _0800E788 @ =gUnknown_030012E4
	ldr r0, [r0]
	str r1, [sp]
	movs r5, #0
	str r5, [sp, #4]
	movs r1, #0x2a
	movs r2, #0
	bl sub_8025BAC
	movs r1, #5
	rsbs r1, r1, #0
	ldrb r2, [r0, #0xc]
	ands r1, r2
	strb r1, [r0, #0xc]
	adds r2, r0, #0
	adds r2, #0x28
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	strb r1, [r2]
	ldr r1, _0800E78C @ =0xFFFFFE80
	movs r2, #8
	movs r3, #0x10
	rsbs r3, r3, #0
	str r1, [r0, #0x64]
	str r1, [r0, #0x54]
	str r2, [r0, #0x58]
	str r3, [r0, #0x5c]
	movs r0, #0x1b
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
	ldr r0, _0800E790 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x17
	bl PlaySfx
	ldrh r1, [r4, #8]
	ldr r0, _0800E794 @ =0x0000FFFF
	cmp r1, r0
	beq _0800E734
	ldr r0, _0800E798 @ =gUnknown_030012B4
	ldr r0, [r0]
	bl sub_80259D4
_0800E734:
	ldr r0, _0800E79C @ =gStaticData_0816BB98
	adds r1, r4, #0
	adds r1, #0x4e
	ldrb r1, [r1]
	adds r0, r1, r0
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800E74C
	ldr r0, _0800E7A0 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022FEC
_0800E74C:
	ldr r0, _0800E7A0 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r2, r4, #0
	adds r2, #0x50
	ldrb r3, [r2]
	rsbs r1, r3, #0
	orrs r1, r3
	lsrs r1, r1, #0x1f
	bl sub_8022CA0
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r2, [r1]
	ands r0, r2
	strb r0, [r1]
	ldr r0, _0800E7A4 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x80
	strb r5, [r0]
	movs r2, #1
	movs r0, #0x80
	ldrb r3, [r1]
	ands r0, r3
	orrs r0, r2
	strb r0, [r1]
	add sp, #8
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0800E788: .4byte gUnknown_030012E4
_0800E78C: .4byte 0xFFFFFE80
_0800E790: .4byte gUnknown_030012BC
_0800E794: .4byte 0x0000FFFF
_0800E798: .4byte gUnknown_030012B4
_0800E79C: .4byte gStaticData_0816BB98
_0800E7A0: .4byte gUnknown_030012C0
_0800E7A4: .4byte gUnknown_030012D8

	thumb_func_start sub_800E7A8
sub_800E7A8: @ 0x0800E7A8
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	lsls r1, r1, #0x18
	lsrs r6, r1, #0x18
	lsls r2, r2, #0x18
	cmp r2, #0
	beq _0800E7D2
	ldr r2, _0800E810 @ =gUnknown_030012D8
	ldr r0, [r2]
	adds r1, r0, #0
	adds r1, #0x91
	ldrb r0, [r1]
	cmp r0, #0
	bne _0800E880
	movs r0, #1
	strb r0, [r1]
	ldr r0, [r2]
	adds r0, #0x91
	ldrb r1, [r0]
	adds r1, #1
	strb r1, [r0]
_0800E7D2:
	cmp r3, #4
	bne _0800E814
	adds r0, r5, #0
	bl sub_8010708
	adds r4, r0, #0
	cmp r4, #0
	beq _0800E832
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #1
	beq _0800E832
_0800E7F0:
	adds r0, r4, #0
	bl sub_8010708
	adds r2, r0, #0
	cmp r2, #0
	beq _0800E836
	adds r1, r2, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #1
	beq _0800E836
	adds r4, r2, #0
	b _0800E7F0
	.align 2, 0
_0800E810: .4byte gUnknown_030012D8
_0800E814:
	cmp r3, #8
	bne _0800E878
	adds r0, r5, #0
	bl sub_801070C
	adds r4, r0, #0
	cmp r4, #0
	beq _0800E832
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #1
	bne _0800E83A
_0800E832:
	adds r2, r5, #0
	b _0800E858
_0800E836:
	adds r2, r4, #0
	b _0800E858
_0800E83A:
	adds r0, r4, #0
	bl sub_801070C
	adds r2, r0, #0
	cmp r2, #0
	beq _0800E836
	adds r1, r2, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #1
	beq _0800E836
	adds r4, r2, #0
	b _0800E83A
_0800E858:
	ldr r0, _0800E874 @ =gStaticData_0816BBDA
	adds r1, r2, #0
	adds r1, #0x4e
	ldrb r1, [r1]
	adds r0, r1, r0
	ldrb r0, [r0]
	cmp r0, #0
	bne _0800E880
	adds r0, r2, #0
	adds r1, r6, #0
	bl sub_800E888
	b _0800E880
	.align 2, 0
_0800E874: .4byte gStaticData_0816BBDA
_0800E878:
	adds r0, r5, #0
	adds r1, r6, #0
	bl sub_800E888
_0800E880:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_800E888
sub_800E888: @ 0x0800E888
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	adds r4, r0, #0
	lsls r1, r1, #0x18
	lsrs r6, r1, #0x18
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #1
	bne _0800E8A8
	b _0800EAE6
_0800E8A8:
	movs r7, #0
	adds r0, r4, #0
	bl sub_801070C
	cmp r0, #0
	beq _0800E8BA
	cmp r6, #0
	bne _0800E8BA
	movs r7, #1
_0800E8BA:
	movs r0, #0x10
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _0800E9AC @ =gUnknown_0300130C
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8009150
	adds r2, r4, #0
	adds r2, #0x4d
	movs r0, #0x7f
	ldrb r3, [r2]
	ands r0, r3
	movs r1, #0
	strb r0, [r2]
	ldr r0, _0800E9B0 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x80
	strb r1, [r0]
	movs r5, #1
	mov r8, r5
	movs r0, #0x80
	ldrb r1, [r2]
	ands r0, r1
	orrs r0, r5
	strb r0, [r2]
	movs r0, #0x1d
	adds r5, r4, #0
	adds r5, #0x2d
	strb r0, [r5]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r4, #0x20]
	ldr r1, [r0]
	ldrb r2, [r5]
	lsls r0, r2, #3
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r1, r1, r0
	ldr r0, _0800E9B4 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldrb r1, [r1, #0x14]
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
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
	movs r2, #3
	ldr r0, [r4, #0x20]
	ldr r1, [r0]
	ldrb r3, [r5]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r2, r0
	blt _0800E954
	subs r2, r0, #1
_0800E954:
	str r2, [r4, #0x30]
	ldr r0, _0800E9B8 @ =gStaticData_0816BB98
	movs r5, #0x4e
	adds r5, r5, r4
	mov sb, r5
	ldrb r1, [r5]
	adds r0, r1, r0
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800E970
	ldr r0, _0800E9BC @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022FEC
_0800E970:
	ldrh r3, [r4, #8]
	ldr r0, _0800E9C0 @ =gUnknown_030012B4
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
	mov r1, r8
	lsls r1, r0
	ldr r0, [r2]
	orrs r0, r1
	str r0, [r2]
	adds r0, r4, #0
	bl sub_800EDBC
	mov r1, sb
	ldrb r0, [r1]
	cmp r0, #0x16
	bls _0800E9A2
	b _0800EAE6
_0800E9A2:
	lsls r0, r0, #2
	ldr r1, _0800E9C4 @ =_0800E9C8
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800E9AC: .4byte gUnknown_0300130C
_0800E9B0: .4byte gUnknown_030012D8
_0800E9B4: .4byte gUnknown_030012B8
_0800E9B8: .4byte gStaticData_0816BB98
_0800E9BC: .4byte gUnknown_030012C0
_0800E9C0: .4byte gUnknown_030012B4
_0800E9C4: .4byte _0800E9C8
_0800E9C8: @ jump table
	.4byte _0800EAB8 @ case 0
	.4byte _0800EAE6 @ case 1
	.4byte _0800EA24 @ case 2
	.4byte _0800EA3E @ case 3
	.4byte _0800EA5C @ case 4
	.4byte _0800EAE6 @ case 5
	.4byte _0800EA46 @ case 6
	.4byte _0800EAE6 @ case 7
	.4byte _0800EAE6 @ case 8
	.4byte _0800EA30 @ case 9
	.4byte _0800EA70 @ case 10
	.4byte _0800EA4E @ case 11
	.4byte _0800EA5C @ case 12
	.4byte _0800EA5C @ case 13
	.4byte _0800EA70 @ case 14
	.4byte _0800EA7A @ case 15
	.4byte _0800EA88 @ case 16
	.4byte _0800EA98 @ case 17
	.4byte _0800EAA8 @ case 18
	.4byte _0800EA70 @ case 19
	.4byte _0800EA70 @ case 20
	.4byte _0800EA70 @ case 21
	.4byte _0800EAE6 @ case 22
_0800EA24:
	cmp r6, #0
	bne _0800EAE6
	adds r0, r4, #0
	bl sub_801085C
	b _0800EAE6
_0800EA30:
	cmp r6, #0
	bne _0800EAE6
	adds r0, r4, #0
	adds r1, r7, #0
	bl sub_801089C
	b _0800EAE6
_0800EA3E:
	adds r0, r4, #0
	bl sub_800F368
	b _0800EAE6
_0800EA46:
	adds r0, r4, #0
	bl sub_800F2BC
	b _0800EAE6
_0800EA4E:
	cmp r6, #0
	bne _0800EAE6
	adds r0, r4, #0
	adds r1, r7, #0
	bl sub_800EAFC
	b _0800EAE6
_0800EA5C:
	ldr r0, _0800EA6C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	b _0800EAE6
	.align 2, 0
_0800EA6C: .4byte gUnknown_030012BC
_0800EA70:
	adds r0, r4, #0
	movs r1, #0
	bl sub_800EEF0
	b _0800EAE6
_0800EA7A:
	cmp r6, #0
	bne _0800EAE6
	adds r0, r4, #0
	adds r1, r7, #0
	bl sub_800ED08
	b _0800EAE6
_0800EA88:
	ldr r0, _0800EA94 @ =gUnknown_030012C0
	ldr r0, [r0]
	movs r1, #1
	bl sub_8022EA8
	b _0800EAE6
	.align 2, 0
_0800EA94: .4byte gUnknown_030012C0
_0800EA98:
	ldr r0, _0800EAA4 @ =gUnknown_030012C0
	ldr r0, [r0]
	movs r1, #2
	bl sub_8022EA8
	b _0800EAE6
	.align 2, 0
_0800EAA4: .4byte gUnknown_030012C0
_0800EAA8:
	ldr r0, _0800EAB4 @ =gUnknown_030012C0
	ldr r0, [r0]
	movs r1, #3
	bl sub_8022EA8
	b _0800EAE6
	.align 2, 0
_0800EAB4: .4byte gUnknown_030012C0
_0800EAB8:
	ldr r1, [r4]
	asrs r1, r1, #8
	ldr r2, [r4, #4]
	asrs r2, r2, #8
	adds r2, #3
	ldr r0, _0800EAF4 @ =gUnknown_030012E4
	ldr r0, [r0]
	movs r3, #3
	str r3, [sp]
	add r3, sp, #4
	strb r7, [r3]
	movs r3, #0
	bl sub_8025CA4
	cmp r6, #0
	bne _0800EAE6
	ldr r0, _0800EAF8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
_0800EAE6:
	add sp, #8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800EAF4: .4byte gUnknown_030012E4
_0800EAF8: .4byte gUnknown_030012BC

	thumb_func_start sub_800EAFC
sub_800EAFC: @ 0x0800EAFC
	push {r4, r5, r6, lr}
	sub sp, #8
	adds r4, r0, #0
	lsls r1, r1, #0x18
	lsrs r6, r1, #0x18
	ldr r0, _0800EB30 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	adds r5, r4, #0
	adds r5, #0x51
	ldrb r0, [r5]
	cmp r0, #9
	bne _0800EB48
	bl rand
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x18
	adds r1, r0, #0
	cmp r0, #0x56
	bhi _0800EB34
	movs r0, #1
	b _0800EB46
	.align 2, 0
_0800EB30: .4byte gUnknown_030012BC
_0800EB34:
	cmp r0, #0xd3
	bhi _0800EB3C
	movs r0, #4
	b _0800EB46
_0800EB3C:
	cmp r1, #0xec
	bhi _0800EB44
	movs r0, #7
	b _0800EB46
_0800EB44:
	movs r0, #8
_0800EB46:
	strb r0, [r5]
_0800EB48:
	adds r0, r4, #0
	adds r0, #0x51
	ldrb r0, [r0]
	subs r0, #1
	cmp r0, #9
	bls _0800EB56
	b _0800ECDE
_0800EB56:
	lsls r0, r0, #2
	ldr r1, _0800EB60 @ =_0800EB64
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800EB60: .4byte _0800EB64
_0800EB64: @ jump table
	.4byte _0800ECDE @ case 0
	.4byte _0800ECC0 @ case 1
	.4byte _0800ECA2 @ case 2
	.4byte _0800EC84 @ case 3
	.4byte _0800EC66 @ case 4
	.4byte _0800EC48 @ case 5
	.4byte _0800EC0C @ case 6
	.4byte _0800EBB0 @ case 7
	.4byte _0800ECDE @ case 8
	.4byte _0800EB8C @ case 9
_0800EB8C:
	ldr r1, [r4]
	asrs r1, r1, #8
	ldr r2, [r4, #4]
	asrs r2, r2, #8
	ldr r0, _0800EBAC @ =gUnknown_030012E4
	ldr r0, [r0]
	movs r3, #0xff
	str r3, [sp]
	add r4, sp, #4
	movs r3, #0
	strb r3, [r4]
	movs r3, #0
	bl sub_8025CA4
	b _0800ECFC
	.align 2, 0
_0800EBAC: .4byte gUnknown_030012E4
_0800EBB0:
	ldr r0, _0800EBFC @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #3
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	ldrh r1, [r4, #8]
	ldr r0, _0800EC00 @ =0x0000FFFF
	cmp r1, r0
	beq _0800EBDC
	ldr r5, _0800EC04 @ =gUnknown_030012B4
	ldr r0, [r5]
	bl sub_802599C
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0800EBDC
	ldr r0, [r5]
	ldrh r1, [r4, #8]
	bl sub_80259D4
_0800EBDC:
	ldr r1, [r4]
	asrs r1, r1, #8
	ldr r2, [r4, #4]
	asrs r2, r2, #8
	adds r2, #3
	ldr r0, _0800EC08 @ =gUnknown_030012E4
	ldr r0, [r0]
	movs r3, #3
	str r3, [sp]
	add r3, sp, #4
	strb r6, [r3]
	movs r3, #0
	bl sub_8025A64
	b _0800ECFC
	.align 2, 0
_0800EBFC: .4byte gUnknown_030012BC
_0800EC00: .4byte 0x0000FFFF
_0800EC04: .4byte gUnknown_030012B4
_0800EC08: .4byte gUnknown_030012E4
_0800EC0C:
	ldr r0, _0800EC40 @ =gUnknown_030012D8
	ldr r2, [r0]
	ldrb r1, [r2, #0xc]
	lsrs r0, r1, #7
	cmp r0, #0
	beq _0800ECFC
	ldr r1, [r2, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0x1a
	movs r3, #0
	bl sub_803AD88
	ldr r0, _0800EC44 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #1
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	b _0800ECFC
	.align 2, 0
_0800EC40: .4byte gUnknown_030012D8
_0800EC44: .4byte gUnknown_030012BC
_0800EC48:
	ldr r1, [r4]
	asrs r1, r1, #8
	subs r1, #1
	ldr r2, [r4, #4]
	asrs r2, r2, #8
	adds r2, #3
	ldr r0, _0800ED04 @ =gUnknown_030012E4
	ldr r0, [r0]
	movs r3, #3
	str r3, [sp]
	add r3, sp, #4
	strb r6, [r3]
	movs r3, #1
	bl sub_8025CA4
_0800EC66:
	ldr r1, [r4]
	asrs r1, r1, #8
	adds r1, #1
	ldr r2, [r4, #4]
	asrs r2, r2, #8
	adds r2, #1
	ldr r0, _0800ED04 @ =gUnknown_030012E4
	ldr r0, [r0]
	movs r3, #3
	str r3, [sp]
	add r3, sp, #4
	strb r6, [r3]
	movs r3, #0
	bl sub_8025CA4
_0800EC84:
	ldr r1, [r4]
	asrs r1, r1, #8
	subs r1, #3
	ldr r2, [r4, #4]
	asrs r2, r2, #8
	adds r2, #3
	ldr r0, _0800ED04 @ =gUnknown_030012E4
	ldr r0, [r0]
	movs r3, #1
	str r3, [sp]
	add r3, sp, #4
	strb r6, [r3]
	movs r3, #1
	bl sub_8025CA4
_0800ECA2:
	ldr r1, [r4]
	asrs r1, r1, #8
	adds r1, #3
	ldr r2, [r4, #4]
	asrs r2, r2, #8
	adds r2, #2
	ldr r0, _0800ED04 @ =gUnknown_030012E4
	ldr r0, [r0]
	movs r3, #1
	str r3, [sp]
	add r3, sp, #4
	strb r6, [r3]
	movs r3, #0
	bl sub_8025CA4
_0800ECC0:
	ldr r1, [r4]
	asrs r1, r1, #8
	adds r1, #5
	ldr r2, [r4, #4]
	asrs r2, r2, #8
	adds r2, #2
	ldr r0, _0800ED04 @ =gUnknown_030012E4
	ldr r0, [r0]
	movs r3, #2
	str r3, [sp]
	add r3, sp, #4
	strb r6, [r3]
	movs r3, #0
	bl sub_8025CA4
_0800ECDE:
	ldr r1, [r4]
	asrs r1, r1, #8
	subs r1, #5
	ldr r2, [r4, #4]
	asrs r2, r2, #8
	adds r2, #3
	ldr r0, _0800ED04 @ =gUnknown_030012E4
	ldr r0, [r0]
	movs r3, #2
	str r3, [sp]
	add r3, sp, #4
	strb r6, [r3]
	movs r3, #1
	bl sub_8025CA4
_0800ECFC:
	add sp, #8
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0800ED04: .4byte gUnknown_030012E4

	thumb_func_start sub_800ED08
sub_800ED08: @ 0x0800ED08
	push {r4, r5, r6, r7, lr}
	sub sp, #8
	adds r4, r0, #0
	lsls r1, r1, #0x18
	lsrs r7, r1, #0x18
	ldr r5, _0800ED3C @ =gUnknown_030012BC
	ldr r0, [r5]
	movs r6, #0x80
	lsls r6, r6, #1
	movs r1, #3
	adds r2, r6, #0
	bl PlaySfx
	ldr r1, [r4, #0x48]
	movs r0, #7
	ands r1, r0
	cmp r1, #1
	beq _0800ED40
	cmp r1, #1
	ble _0800EDB2
	cmp r1, #2
	beq _0800ED94
	cmp r1, #3
	beq _0800ED9E
	b _0800EDB2
	.align 2, 0
_0800ED3C: .4byte gUnknown_030012BC
_0800ED40:
	ldr r0, [r5]
	movs r1, #3
	adds r2, r6, #0
	bl PlaySfx
	ldrh r1, [r4, #8]
	ldr r0, _0800ED88 @ =0x0000FFFF
	cmp r1, r0
	beq _0800ED68
	ldr r5, _0800ED8C @ =gUnknown_030012B4
	ldr r0, [r5]
	bl sub_802599C
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0800ED68
	ldr r0, [r5]
	ldrh r1, [r4, #8]
	bl sub_80259D4
_0800ED68:
	ldr r1, [r4]
	asrs r1, r1, #8
	ldr r2, [r4, #4]
	asrs r2, r2, #8
	adds r2, #3
	ldr r0, _0800ED90 @ =gUnknown_030012E4
	ldr r0, [r0]
	movs r3, #3
	str r3, [sp]
	add r3, sp, #4
	strb r7, [r3]
	movs r3, #0
	bl sub_8025A64
	b _0800EDB2
	.align 2, 0
_0800ED88: .4byte 0x0000FFFF
_0800ED8C: .4byte gUnknown_030012B4
_0800ED90: .4byte gUnknown_030012E4
_0800ED94:
	adds r0, r4, #0
	adds r1, r7, #0
	bl sub_800EAFC
	b _0800EDB2
_0800ED9E:
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x80
	ldrb r2, [r1]
	ands r0, r2
	strb r0, [r1]
	adds r0, r4, #0
	movs r1, #1
	bl sub_800EEF0
_0800EDB2:
	add sp, #8
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_800EDBC
sub_800EDBC: @ 0x0800EDBC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov r8, r0
	movs r0, #0xfe
	str r0, [sp]
	mov r1, r8
	ldr r0, [r1, #0x20]
	mov r2, r8
	adds r2, #0x2d
	ldrb r3, [r2]
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	ldr r0, [r0]
	adds r0, r0, r1
	ldrb r0, [r0, #9]
	adds r0, #1
	lsls r0, r0, #8
	mov sl, r0
	mov r0, r8
	bl sub_801070C
	adds r5, r0, #0
	mov r1, r8
	ldr r0, [r1, #0x44]
	cmp r0, #0
	beq _0800EDFE
	movs r2, #0xfc
	str r2, [sp]
_0800EDFE:
	cmp r5, #0
	beq _0800EEE0
	mov r3, r8
	cmp r3, #0
	beq _0800EEE0
	cmp r0, #0
	beq _0800EE10
	ldr r0, [r3, #0x40]
	b _0800EE14
_0800EE10:
	mov r1, r8
	ldr r0, [r1, #4]
_0800EE14:
	str r0, [r5, #0x40]
	ldr r7, [r5, #0x40]
	ldr r0, [r5, #4]
	subs r7, r7, r0
	cmp r7, #0
	bge _0800EE22
	movs r7, #0
_0800EE22:
	movs r2, #0
	mov sb, r2
_0800EE26:
	cmp r5, #0
	beq _0800EEE0
	ldr r0, [r5, #0x44]
	cmp r0, #0
	beq _0800EE3C
	mov r3, sb
	adds r0, r7, r3
	str r0, [r5, #0x44]
	ldr r0, [r5, #0x40]
	add r0, sb
	b _0800EE42
_0800EE3C:
	str r7, [r5, #0x44]
	ldr r0, [r5, #4]
	add r0, sl
_0800EE42:
	str r0, [r5, #0x40]
	adds r2, r5, #0
	adds r2, #0x4c
	movs r1, #0
	ldrsb r1, [r2, r1]
	cmp r1, #0
	ble _0800EE52
	movs r1, #0
_0800EE52:
	ldr r3, [sp]
	lsls r0, r3, #0x18
	asrs r0, r0, #0x18
	adds r0, r1, r0
	strb r0, [r2]
	movs r0, #0x10
	ldrb r1, [r5, #0xc]
	orrs r0, r1
	strb r0, [r5, #0xc]
	ldr r0, _0800EED8 @ =gUnknown_0300130C
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8009150
	adds r6, r5, #0
	adds r6, #0x4e
	ldrb r2, [r6]
	ldr r3, _0800EEDC @ =gStaticData_0816BBC4
	adds r0, r2, r3
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800EEC6
	mov r1, r8
	ldr r0, [r1, #0x48]
	cmp r0, #0
	bne _0800EEC6
	ldr r1, [r5, #0x44]
	movs r0, #0xb0
	lsls r0, r0, #5
	cmp r1, r0
	ble _0800EEC6
	adds r0, r5, #0
	bl sub_801070C
	adds r4, r0, #0
	adds r0, r5, #0
	bl sub_8010708
	cmp r4, #0
	bne _0800EEBA
	cmp r0, #0
	beq _0800EEBA
	ldrb r6, [r6]
	cmp r6, #0xa
	bne _0800EEC6
	adds r1, r5, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800EEC6
_0800EEBA:
	adds r1, r5, #0
	adds r1, #0x4f
	movs r0, #0
	strb r0, [r1]
	movs r0, #1
	str r0, [r5, #0x48]
_0800EEC6:
	adds r0, r5, #0
	bl sub_801070C
	adds r5, r0, #0
	mov r2, sb
	cmp r2, #0
	bne _0800EE26
	mov sb, sl
	b _0800EE26
	.align 2, 0
_0800EED8: .4byte gUnknown_0300130C
_0800EEDC: .4byte gStaticData_0816BBC4
_0800EEE0:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_800EEF0
sub_800EEF0: @ 0x0800EEF0
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	lsls r1, r1, #0x18
	lsrs r7, r1, #0x18
	adds r5, r4, #0
	adds r5, #0x4d
	movs r2, #0x7f
	adds r0, r2, #0
	ldrb r1, [r5]
	ands r0, r1
	cmp r0, #1
	bne _0800EF0A
	b _0800F04C
_0800EF0A:
	adds r0, r4, #0
	adds r0, #0x4f
	movs r1, #0
	strb r1, [r0]
	adds r0, r2, #0
	ldrb r2, [r5]
	ands r0, r2
	strb r0, [r5]
	ldr r0, _0800EF68 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x80
	strb r1, [r0]
	movs r0, #0x10
	ldrb r3, [r4, #0xc]
	orrs r0, r3
	strb r0, [r4, #0xc]
	ldr r0, _0800EF6C @ =gUnknown_0300130C
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8009150
	movs r1, #1
	movs r0, #0x80
	ldrb r2, [r5]
	ands r0, r2
	orrs r0, r1
	strb r0, [r5]
	adds r0, r4, #0
	adds r0, #0x4e
	adds r6, r0, #0
	ldrb r3, [r6]
	cmp r3, #0xa
	bne _0800EF70
	subs r0, #0x21
	strb r1, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	b _0800EF8C
	.align 2, 0
_0800EF68: .4byte gUnknown_030012D8
_0800EF6C: .4byte gUnknown_0300130C
_0800EF70:
	movs r0, #0x21
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
_0800EF8C:
	ldr r0, _0800F054 @ =gStaticData_0816BB98
	ldrb r5, [r6]
	adds r0, r5, r0
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800EFA0
	ldr r0, _0800F058 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022FEC
_0800EFA0:
	ldrh r3, [r4, #8]
	ldr r0, _0800F05C @ =gUnknown_030012B4
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
	ldr r0, _0800F060 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	adds r0, r4, #0
	bl sub_800EDBC
	ldr r0, _0800F064 @ =gUnknown_030012D8
	ldr r3, [r0]
	ldrb r1, [r3, #0xc]
	lsrs r0, r1, #6
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _0800F042
	movs r5, #0
	adds r0, r3, #0
	adds r0, #0x8c
	ldr r1, _0800F068 @ =gUnknown_0300082C
	ldr r2, [r0]
	ldr r0, [r1]
	cmp r2, r0
	bls _0800EFF8
	movs r5, #1
_0800EFF8:
	cmp r5, #0
	bne _0800F042
	ldr r0, [r3]
	asrs r0, r0, #8
	ldr r1, [r4]
	asrs r1, r1, #8
	subs r0, r0, r1
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	cmp r0, #0x1d
	bgt _0800F024
	ldr r0, [r3, #4]
	asrs r0, r0, #8
	ldr r1, [r4, #4]
	asrs r1, r1, #8
	subs r0, r0, r1
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	cmp r0, #0x1d
	ble _0800F028
_0800F024:
	cmp r7, #0
	beq _0800F042
_0800F028:
	ldr r0, _0800F064 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #4
	movs r3, #0
	bl sub_803AD88
_0800F042:
	ldrb r5, [r6]
	cmp r5, #0xa
	beq _0800F04C
	movs r0, #0x13
	strb r0, [r6]
_0800F04C:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800F054: .4byte gStaticData_0816BB98
_0800F058: .4byte gUnknown_030012C0
_0800F05C: .4byte gUnknown_030012B4
_0800F060: .4byte gUnknown_030012BC
_0800F064: .4byte gUnknown_030012D8
_0800F068: .4byte gUnknown_0300082C

	thumb_func_start sub_800F06C
sub_800F06C: @ 0x0800F06C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	mov r8, r1
	movs r5, #0
	ldr r0, _0800F0F0 @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r0, [r0]
	cmp r5, r0
	bge _0800F136
	ldr r7, _0800F0F4 @ =gStaticData_0816BBC4
_0800F084:
	ldr r0, _0800F0F0 @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r1, [r0, #8]
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
	cmp r0, #3
	bne _0800F12A
	ldr r2, [r4]
	asrs r2, r2, #8
	ldr r0, [r6]
	asrs r0, r0, #8
	subs r2, r2, r0
	asrs r0, r2, #0x1f
	eors r2, r0
	subs r2, r2, r0
	ldr r1, [r4, #4]
	asrs r1, r1, #8
	ldr r0, [r6, #4]
	asrs r0, r0, #8
	subs r1, r1, r0
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	adds r2, r2, r1
	cmp r2, r8
	bgt _0800F12A
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800F12A
	adds r0, r4, #0
	adds r0, #0x4e
	ldrb r1, [r0]
	adds r0, r1, r7
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800F0F8
	adds r0, r4, #0
	movs r1, #0
	bl sub_800EEF0
	b _0800F12A
	.align 2, 0
_0800F0F0: .4byte gUnknown_0300130C
_0800F0F4: .4byte gStaticData_0816BBC4
_0800F0F8:
	ldr r0, _0800F110 @ =gStaticData_0816BBAE
	adds r0, r1, r0
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800F114
	adds r0, r4, #0
	movs r1, #1
	movs r2, #0
	movs r3, #0
	bl sub_800E7A8
	b _0800F12A
	.align 2, 0
_0800F110: .4byte gStaticData_0816BBAE
_0800F114:
	cmp r1, #3
	bne _0800F120
	adds r0, r4, #0
	bl sub_800F368
	b _0800F12A
_0800F120:
	cmp r1, #6
	bne _0800F12A
	adds r0, r4, #0
	bl sub_800F2BC
_0800F12A:
	adds r5, #1
	ldr r0, _0800F1B0 @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r0, [r0]
	cmp r5, r0
	blt _0800F084
_0800F136:
	movs r5, #0
	ldr r1, _0800F1B4 @ =gUnknown_030012EC
	ldr r0, [r1]
	ldr r0, [r0, #4]
	cmp r5, r0
	bge _0800F1A2
	adds r7, r1, #0
_0800F144:
	ldr r0, [r7]
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
	cmp r0, #2
	bne _0800F198
	ldr r2, [r4]
	asrs r2, r2, #8
	ldr r0, [r6]
	asrs r0, r0, #8
	subs r2, r2, r0
	asrs r0, r2, #0x1f
	eors r2, r0
	subs r2, r2, r0
	ldr r1, [r4, #4]
	asrs r1, r1, #8
	ldr r0, [r6, #4]
	asrs r0, r0, #8
	subs r1, r1, r0
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	adds r2, r2, r1
	cmp r2, r8
	bgt _0800F198
	adds r0, r4, #0
	movs r1, #1
	bl sub_8011448
	movs r0, #0x10
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
_0800F198:
	adds r5, #1
	ldr r0, [r7]
	ldr r0, [r0, #4]
	cmp r5, r0
	blt _0800F144
_0800F1A2:
	movs r0, #0xff
	str r0, [r6, #0x48]
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800F1B0: .4byte gUnknown_0300130C
_0800F1B4: .4byte gUnknown_030012EC

	thumb_func_start sub_800F1B8
sub_800F1B8: @ 0x0800F1B8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	bl sub_800F258
	ldr r0, _0800F224 @ =gUnknown_030012B0
	mov r8, r0
_0800F1C6:
	movs r0, #0
	mov r1, r8
	strb r0, [r1]
	movs r5, #0
	ldr r7, _0800F228 @ =gUnknown_0300130C
	ldr r0, [r7]
	ldr r0, [r0]
	cmp r5, r0
	bge _0800F244
	adds r6, r7, #0
_0800F1DA:
	ldr r0, [r6]
	ldr r1, [r0, #8]
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
	cmp r0, #3
	bne _0800F23A
	movs r0, #1
	ldrb r1, [r4, #0xc]
	ands r0, r1
	cmp r0, #0
	beq _0800F22C
	ldr r0, [r6]
	adds r1, r5, #0
	bl sub_8009AA0
	cmp r4, #0
	beq _0800F220
	ldr r1, [r4, #0x18]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0800F220:
	subs r5, #1
	b _0800F23A
	.align 2, 0
_0800F224: .4byte gUnknown_030012B0
_0800F228: .4byte gUnknown_0300130C
_0800F22C:
	ldr r1, [r4, #0x18]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #0x1c]
	bl sub_803AD7C
_0800F23A:
	adds r5, #1
	ldr r0, [r7]
	ldr r0, [r0]
	cmp r5, r0
	blt _0800F1DA
_0800F244:
	mov r1, r8
	ldrb r0, [r1]
	cmp r0, #0
	bne _0800F1C6
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_800F258
sub_800F258: @ 0x0800F258
	push {r4, r5, r6, lr}
	movs r5, #0
	ldr r1, _0800F2B8 @ =gUnknown_0300130C
	ldr r0, [r1]
	ldr r0, [r0]
	cmp r5, r0
	bge _0800F2B0
	adds r6, r1, #0
_0800F268:
	ldr r0, [r6]
	ldr r1, [r0, #8]
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
	cmp r0, #3
	bne _0800F2A6
	adds r0, r4, #0
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #0xa
	bne _0800F2A6
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800F2A6
	adds r0, r4, #0
	movs r1, #0
	bl sub_800EEF0
_0800F2A6:
	adds r5, #1
	ldr r0, [r6]
	ldr r0, [r0]
	cmp r5, r0
	blt _0800F268
_0800F2B0:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0800F2B8: .4byte gUnknown_0300130C

	thumb_func_start sub_800F2BC
sub_800F2BC: @ 0x0800F2BC
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	ldr r0, [r6, #0x48]
	cmp r0, #0
	bne _0800F34C
	adds r1, r6, #0
	adds r1, #0x4d
	movs r0, #0x80
	ldrb r2, [r1]
	orrs r0, r2
	strb r0, [r1]
	ldr r0, _0800F354 @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r4, #1
	adds r0, #0x80
	strb r4, [r0]
	movs r0, #0x23
	adds r5, r6, #0
	adds r5, #0x2d
	strb r0, [r5]
	adds r0, r6, #0
	bl sub_80087C0
	adds r0, r6, #0
	bl sub_80087B4
	adds r0, r6, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r6, #0x20]
	ldr r1, [r0]
	ldrb r3, [r5]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	adds r1, r1, r0
	ldr r0, _0800F358 @ =gUnknown_030012B8
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
	bl sub_800F258
	ldr r0, _0800F35C @ =gUnknown_03001318
	ldr r0, [r0]
	bl sub_8028474
	ldr r0, _0800F360 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	str r4, [r6, #0x48]
	ldr r0, _0800F364 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_802306C
_0800F34C:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0800F354: .4byte gUnknown_030012D8
_0800F358: .4byte gUnknown_030012B8
_0800F35C: .4byte gUnknown_03001318
_0800F360: .4byte gUnknown_030012BC
_0800F364: .4byte gUnknown_030012C0

	thumb_func_start sub_800F368
sub_800F368: @ 0x0800F368
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x80
	adds r6, r0, #0
	movs r7, #0
	ldr r1, [r6, #0x48]
	movs r0, #1
	rsbs r0, r0, #0
	cmp r1, r0
	bne _0800F384
	b _0800F4E4
_0800F384:
	cmp r1, #0
	beq _0800F38A
	b _0800F4E4
_0800F38A:
	movs r0, #0x10
	ldrb r1, [r6, #0xc]
	orrs r0, r1
	strb r0, [r6, #0xc]
	ldr r4, _0800F4C0 @ =gUnknown_0300130C
	ldr r0, [r4]
	adds r1, r6, #0
	bl sub_8009150
	adds r1, r6, #0
	adds r1, #0x4d
	movs r0, #0x80
	ldrb r2, [r1]
	orrs r0, r2
	strb r0, [r1]
	ldr r0, _0800F4C4 @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r1, #1
	adds r0, #0x80
	strb r1, [r0]
	movs r0, #0x22
	adds r5, r6, #0
	adds r5, #0x2d
	strb r0, [r5]
	adds r0, r6, #0
	bl sub_80087C0
	adds r0, r6, #0
	bl sub_80087B4
	adds r0, r6, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r6, #0x20]
	ldr r1, [r0]
	ldrb r3, [r5]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	adds r1, r1, r0
	ldr r0, _0800F4C8 @ =gUnknown_030012B8
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
	ldr r0, _0800F4CC @ =gUnknown_030012B4
	ldr r0, [r0]
	ldrh r1, [r6, #8]
	bl sub_8025A0C
	movs r5, #0
	ldr r0, [r4]
	ldr r0, [r0]
	movs r1, #0x50
	adds r1, r1, r6
	mov r8, r1
	movs r2, #0x4c
	adds r2, r2, r6
	mov sb, r2
	movs r3, #0x4f
	adds r3, r3, r6
	mov sl, r3
	cmp r7, r0
	bge _0800F48C
_0800F424:
	ldr r0, _0800F4C0 @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r1, [r0, #8]
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
	cmp r0, #3
	bne _0800F480
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800F480
	adds r0, r4, #0
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #5
	bne _0800F480
	adds r0, r4, #0
	adds r0, #0x50
	ldrb r0, [r0]
	mov r3, r8
	ldrb r3, [r3]
	cmp r0, r3
	bne _0800F480
	lsls r0, r7, #2
	add r0, sp
	str r4, [r0]
	adds r7, #1
	movs r0, #0x1f
	ands r7, r0
	ldr r0, _0800F4CC @ =gUnknown_030012B4
	ldr r0, [r0]
	ldrh r1, [r4, #8]
	bl sub_8025A0C
_0800F480:
	adds r5, #1
	ldr r0, _0800F4C0 @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r0, [r0]
	cmp r5, r0
	blt _0800F424
_0800F48C:
	cmp r7, #0
	beq _0800F4D0
	adds r0, r7, #1
	lsls r0, r0, #2
	bl sub_8026EC0
	adds r2, r0, #0
	adds r1, r6, #0
	adds r1, #0x59
	movs r0, #1
	strb r0, [r1]
	str r7, [r2]
	cmp r7, #0
	ble _0800F4BA
	mov r3, sp
	adds r1, r2, #0
	adds r5, r7, #0
_0800F4AE:
	ldm r3!, {r0}
	str r0, [r1, #4]
	adds r1, #4
	subs r5, #1
	cmp r5, #0
	bne _0800F4AE
_0800F4BA:
	str r2, [r6, #0x48]
	b _0800F4D6
	.align 2, 0
_0800F4C0: .4byte gUnknown_0300130C
_0800F4C4: .4byte gUnknown_030012D8
_0800F4C8: .4byte gUnknown_030012B8
_0800F4CC: .4byte gUnknown_030012B4
_0800F4D0:
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r6, #0x48]
_0800F4D6:
	movs r0, #0
	mov r1, r8
	strb r0, [r1]
	mov r2, sb
	ldrb r0, [r2]
	mov r3, sl
	strb r0, [r3]
_0800F4E4:
	add sp, #0x80
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_800F4F4
sub_800F4F4: @ 0x0800F4F4
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r4, r0, #0
	adds r3, r4, #0
	adds r3, #0x4f
	ldrb r5, [r3]
	cmp r5, #0
	bne _0800F5A6
	adds r1, r4, #0
	adds r1, #0x50
	ldrb r0, [r1]
	adds r0, #1
	strb r0, [r1]
	adds r2, r4, #0
	adds r2, #0x51
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	adds r7, r1, #0
	ldrb r2, [r2]
	cmp r0, r2
	blo _0800F54A
	ldr r1, [r4, #0x48]
	adds r0, r1, #1
	cmp r0, #1
	bls _0800F53A
	cmp r1, #0
	beq _0800F534
	adds r0, r1, #0
	bl sub_8026EB4
_0800F534:
	adds r0, r4, #0
	adds r0, #0x59
	strb r5, [r0]
_0800F53A:
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r4, #0x48]
	movs r1, #7
	adds r0, r4, #0
	adds r0, #0x4e
	strb r1, [r0]
	b _0800F5A6
_0800F54A:
	ldr r1, [r4, #0x48]
	adds r0, r1, #1
	mov sb, r3
	adds r4, #0x4c
	mov r8, r4
	cmp r0, #1
	bls _0800F59E
	ldm r1!, {r2}
	adds r0, r1, #0
	movs r6, #0
	cmp r2, #0
	ble _0800F59E
	adds r4, r0, #0
	adds r5, r2, #0
_0800F566:
	ldr r0, [r4]
	adds r1, r0, #0
	adds r1, #0x4e
	ldrb r1, [r1]
	cmp r1, #5
	bne _0800F596
	adds r1, r0, #0
	adds r1, #0x51
	ldrb r2, [r7]
	ldrb r1, [r1]
	cmp r2, r1
	blo _0800F596
	bl sub_800F5B8
	cmp r6, #0
	bne _0800F596
	ldr r0, _0800F5B4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0xf
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	movs r6, #1
_0800F596:
	adds r4, #4
	subs r5, #1
	cmp r5, #0
	bne _0800F566
_0800F59E:
	mov r1, r8
	ldrb r0, [r1]
	mov r2, sb
	strb r0, [r2]
_0800F5A6:
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800F5B4: .4byte gUnknown_030012BC

	thumb_func_start sub_800F5B8
sub_800F5B8: @ 0x0800F5B8
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x48]
	subs r0, #0x15
	adds r1, r4, #0
	adds r1, #0x4e
	strb r0, [r1]
	ldrb r0, [r1]
	cmp r0, #0x12
	bhi _0800F698
	lsls r0, r0, #2
	ldr r1, _0800F5D8 @ =_0800F5DC
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800F5D8: .4byte _0800F5DC
_0800F5DC: @ jump table
	.4byte _0800F628 @ case 0
	.4byte _0800F62C @ case 1
	.4byte _0800F630 @ case 2
	.4byte _0800F698 @ case 3
	.4byte _0800F634 @ case 4
	.4byte _0800F698 @ case 5
	.4byte _0800F638 @ case 6
	.4byte _0800F63C @ case 7
	.4byte _0800F640 @ case 8
	.4byte _0800F698 @ case 9
	.4byte _0800F644 @ case 10
	.4byte _0800F698 @ case 11
	.4byte _0800F648 @ case 12
	.4byte _0800F652 @ case 13
	.4byte _0800F656 @ case 14
	.4byte _0800F698 @ case 15
	.4byte _0800F65A @ case 16
	.4byte _0800F65E @ case 17
	.4byte _0800F67C @ case 18
_0800F628:
	movs r0, #0x1f
	b _0800F660
_0800F62C:
	movs r0, #0x1a
	b _0800F660
_0800F630:
	movs r0, #0x17
	b _0800F660
_0800F634:
	movs r0, #0x18
	b _0800F660
_0800F638:
	movs r0, #4
	b _0800F660
_0800F63C:
	movs r0, #0x20
	b _0800F660
_0800F640:
	movs r0, #2
	b _0800F660
_0800F644:
	movs r0, #5
	b _0800F660
_0800F648:
	movs r0, #0x2a
	rsbs r0, r0, #0
	str r0, [r4, #0x48]
	movs r0, #0x19
	b _0800F660
_0800F652:
	movs r0, #6
	b _0800F660
_0800F656:
	movs r0, #0x11
	b _0800F660
_0800F65A:
	movs r0, #0xe
	b _0800F660
_0800F65E:
	movs r0, #0xf
_0800F660:
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
	b _0800F698
_0800F67C:
	movs r0, #0x10
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
_0800F698:
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
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_800F6B8
sub_800F6B8: @ 0x0800F6B8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	mov sb, r0
	mov r8, r1
	adds r7, r2, #0
	adds r6, r3, #0
	movs r5, #0
	ldr r0, _0800F748 @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r0, [r0]
	cmp r5, r0
	bge _0800F784
	ldr r0, _0800F74C @ =gStaticData_0816BBC4
	mov sl, r0
_0800F6DA:
	ldr r0, _0800F748 @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r1, [r0, #8]
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
	cmp r0, #3
	bne _0800F778
	ldr r1, [r4]
	asrs r1, r1, #8
	mov r0, sb
	subs r1, r1, r0
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	ldr r0, [r4, #4]
	asrs r0, r0, #8
	mov r2, r8
	subs r0, r0, r2
	asrs r2, r0, #0x1f
	eors r0, r2
	subs r0, r0, r2
	adds r1, r1, r0
	cmp r1, r7
	bgt _0800F778
	cmp r0, r6
	bge _0800F778
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800F778
	adds r0, r4, #0
	adds r0, #0x4e
	ldrb r1, [r0]
	mov r2, sl
	adds r0, r1, r2
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800F750
	adds r0, r4, #0
	movs r1, #0
	bl sub_800EEF0
	b _0800F778
	.align 2, 0
_0800F748: .4byte gUnknown_0300130C
_0800F74C: .4byte gStaticData_0816BBC4
_0800F750:
	ldr r0, _0800F768 @ =gStaticData_0816BBAE
	adds r0, r1, r0
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800F778
	cmp r1, #1
	bne _0800F76C
	adds r0, r4, #0
	bl sub_800E6B0
	b _0800F778
	.align 2, 0
_0800F768: .4byte gStaticData_0816BBAE
_0800F76C:
	adds r0, r4, #0
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_800E7A8
_0800F778:
	adds r5, #1
	ldr r0, _0800F794 @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r0, [r0]
	cmp r5, r0
	blt _0800F6DA
_0800F784:
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800F794: .4byte gUnknown_0300130C

	thumb_func_start sub_800F798
sub_800F798: @ 0x0800F798
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	ldr r0, _0800F7C4 @ =gStaticData_0816BBC4
	adds r1, r6, #0
	adds r1, #0x4e
	ldrb r1, [r1]
	adds r0, r1, r0
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800F7D4
	ldr r0, [r6, #0x34]
	cmp r0, #0
	bne _0800F7D4
	ldr r0, [r6, #0x30]
	cmp r0, #3
	bne _0800F7C8
	adds r0, r6, #0
	movs r1, #0x14
	bl sub_800F06C
	b _0800F7D4
	.align 2, 0
_0800F7C4: .4byte gStaticData_0816BBC4
_0800F7C8:
	cmp r0, #6
	bne _0800F7D4
	adds r0, r6, #0
	movs r1, #0x28
	bl sub_800F06C
_0800F7D4:
	adds r0, r6, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800F8C4
	adds r0, r6, #0
	bl sub_8010708
	adds r5, r0, #0
	adds r0, r6, #0
	bl sub_801070C
	adds r4, r0, #0
	cmp r5, #0
	beq _0800F806
	cmp r4, #0
	beq _0800F814
	adds r1, r5, #0
	bl sub_8010710
	adds r0, r5, #0
	adds r1, r4, #0
	bl sub_8010714
	b _0800F820
_0800F806:
	cmp r4, #0
	beq _0800F814
	adds r0, r4, #0
	movs r1, #0
	bl sub_8010710
	b _0800F820
_0800F814:
	cmp r5, #0
	beq _0800F820
	adds r0, r5, #0
	movs r1, #0
	bl sub_8010714
_0800F820:
	adds r0, r6, #0
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #1
	beq _0800F8D4
	ldr r0, _0800F898 @ =gUnknown_030012B0
	movs r1, #1
	strb r1, [r0]
	ldrb r0, [r6, #0xc]
	orrs r1, r0
	strb r1, [r6, #0xc]
	ldr r0, _0800F89C @ =0x0000FFFF
	ldrh r1, [r6, #8]
	cmp r1, r0
	beq _0800F860
	ldrh r3, [r6, #8]
	ldr r0, _0800F8A0 @ =gUnknown_030012B4
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
_0800F860:
	movs r3, #0
	ldr r1, _0800F8A4 @ =gUnknown_030012D8
	ldr r0, [r1]
	adds r0, #0x94
	ldrb r0, [r0]
	cmp r3, r0
	bge _0800F8D4
	adds r4, r1, #0
_0800F870:
	ldr r2, [r4]
	adds r0, r2, #0
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #0
	bne _0800F8A8
	cmp r3, #4
	ble _0800F88A
	adds r0, r2, #0
	adds r0, #0x94
	ldrb r0, [r0]
	cmp r3, r0
	bge _0800F8A8
_0800F88A:
	lsls r1, r3, #2
	adds r0, r2, #0
	adds r0, #0x98
	adds r0, r0, r1
	ldr r0, [r0]
	b _0800F8AA
	.align 2, 0
_0800F898: .4byte gUnknown_030012B0
_0800F89C: .4byte 0x0000FFFF
_0800F8A0: .4byte gUnknown_030012B4
_0800F8A4: .4byte gUnknown_030012D8
_0800F8A8:
	movs r0, #0
_0800F8AA:
	cmp r0, r6
	bne _0800F8B6
	ldr r0, [r4]
	adds r0, #0x94
	movs r1, #0
	strb r1, [r0]
_0800F8B6:
	adds r3, #1
	ldr r0, [r4]
	adds r0, #0x94
	ldrb r0, [r0]
	cmp r3, r0
	blt _0800F870
	b _0800F8D4
_0800F8C4:
	adds r0, r6, #0
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #1
	beq _0800F8D4
	ldr r1, _0800F8DC @ =gUnknown_030012B0
	movs r0, #1
	strb r0, [r1]
_0800F8D4:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0800F8DC: .4byte gUnknown_030012B0

	thumb_func_start sub_800F8E0
sub_800F8E0: @ 0x0800F8E0
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r6, r4, #0
	adds r6, #0x4f
	ldrb r0, [r6]
	cmp r0, #0
	bne _0800F98A
	adds r5, r4, #0
	adds r5, #0x4e
	ldrb r0, [r5]
	adds r1, r0, #0
	cmp r0, #0x14
	beq _0800F93C
	cmp r0, #0x14
	bgt _0800F904
	cmp r0, #0x13
	beq _0800F974
	b _0800F98A
_0800F904:
	cmp r1, #0x15
	bne _0800F98A
	movs r0, #0x13
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
	ldr r0, _0800F938 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x11
	bl PlaySfx
	movs r0, #0x14
	b _0800F968
	.align 2, 0
_0800F938: .4byte gUnknown_030012BC
_0800F93C:
	movs r0, #0x12
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
	ldr r0, _0800F970 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x11
	bl PlaySfx
	movs r0, #0x13
_0800F968:
	strb r0, [r5]
	movs r0, #0x3c
	strb r0, [r6]
	b _0800F98A
	.align 2, 0
_0800F970: .4byte gUnknown_030012BC
_0800F974:
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800F98A
	adds r0, r4, #0
	movs r1, #0
	bl sub_800EEF0
_0800F98A:
	pop {r4, r5, r6}
	pop {r0}
	bx r0

	thumb_func_start sub_800F990
sub_800F990: @ 0x0800F990
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r4, r0, #0
	ldr r2, [r4, #0x48]
	movs r0, #0xc0
	ands r0, r2
	cmp r0, #0
	bne _0800F9E2
	ldr r0, _0800FA3C @ =gUnknown_030012D8
	ldr r3, [r0]
	ldr r0, [r3]
	asrs r1, r0, #8
	ldr r0, [r4]
	asrs r0, r0, #8
	subs r1, r1, r0
	cmp r1, #0
	bge _0800F9B8
	rsbs r1, r1, #0
_0800F9B8:
	cmp r1, #0x4f
	bgt _0800F9E2
	ldr r0, [r3, #4]
	asrs r1, r0, #8
	ldr r0, [r4, #4]
	asrs r0, r0, #8
	subs r1, r1, r0
	cmp r1, #0
	bge _0800F9CC
	rsbs r1, r1, #0
_0800F9CC:
	cmp r1, #0x3f
	bgt _0800F9E2
	movs r0, #0x3f
	ands r2, r0
	movs r0, #0x40
	orrs r2, r0
	movs r0, #0xc7
	ands r2, r0
	movs r0, #0x10
	orrs r2, r0
	str r2, [r4, #0x48]
_0800F9E2:
	adds r0, r4, #0
	adds r0, #0x4f
	ldrb r1, [r0]
	mov sb, r0
	cmp r1, #0
	beq _0800F9F0
	b _0800FC5C
_0800F9F0:
	ldr r0, [r4, #0x48]
	movs r1, #7
	ands r1, r0
	movs r2, #4
	ands r1, r2
	adds r2, r0, #0
	adds r5, r4, #0
	adds r5, #0x2d
	cmp r1, #0
	bne _0800FA06
	b _0800FBA0
_0800FA06:
	ldrb r0, [r5]
	cmp r0, #8
	beq _0800FA0E
	b _0800FBA0
_0800FA0E:
	movs r6, #0
	movs r1, #0x29
	adds r1, r1, r4
	mov r8, r1
	movs r7, #7
_0800FA18:
	ldr r1, [r4, #0x48]
	adds r0, r1, #0
	ands r0, r7
	adds r2, r0, #1
	movs r0, #3
	ands r2, r0
	adds r3, r2, #0
	movs r0, #0xf8
	ands r1, r0
	orrs r1, r2
	str r1, [r4, #0x48]
	cmp r2, #1
	beq _0800FAEC
	cmp r2, #1
	bgt _0800FA40
	cmp r2, #0
	beq _0800FA4A
	b _0800FB4E
	.align 2, 0
_0800FA3C: .4byte gUnknown_030012D8
_0800FA40:
	cmp r3, #2
	beq _0800FAFE
	cmp r3, #3
	beq _0800FB26
	b _0800FB4E
_0800FA4A:
	strb r7, [r5]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r4, #0x48]
	movs r6, #0xc0
	ands r0, r6
	cmp r0, #0
	beq _0800FB54
	adds r0, r4, #0
	bl sub_8010A50
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #0
	beq _0800FA88
	subs r2, r0, #1
	lsls r2, r2, #0x18
	ldr r1, [r4, #0x48]
	movs r0, #0xc7
	ands r1, r0
	lsrs r2, r2, #0x15
	orrs r1, r2
	str r1, [r4, #0x48]
_0800FA88:
	ldr r2, [r4, #0x48]
	movs r0, #0x38
	ands r0, r2
	cmp r0, #0
	bne _0800FB54
	movs r1, #0xc7
	ands r1, r2
	movs r0, #0x10
	orrs r1, r0
	str r1, [r4, #0x48]
	adds r0, r1, #0
	ands r0, r6
	lsrs r0, r0, #6
	cmp r0, #2
	beq _0800FAC2
	cmp r0, #2
	bgt _0800FAB0
	cmp r0, #1
	beq _0800FAB6
	b _0800FB54
_0800FAB0:
	cmp r0, #3
	beq _0800FACC
	b _0800FB54
_0800FAB6:
	movs r0, #0x3f
	ands r1, r0
	movs r0, #0x80
	orrs r1, r0
	str r1, [r4, #0x48]
	b _0800FB54
_0800FAC2:
	movs r0, #0x3f
	ands r1, r0
	orrs r1, r6
	str r1, [r4, #0x48]
	b _0800FB54
_0800FACC:
	movs r0, #0x20
	strb r0, [r5]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	adds r0, #0x4e
	strb r7, [r0]
	b _0800FB54
_0800FAEC:
	adds r1, r4, #0
	adds r1, #0x50
	movs r0, #2
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _0800FB4E
	movs r0, #9
	b _0800FB0E
_0800FAFE:
	adds r1, r4, #0
	adds r1, #0x50
	movs r0, #1
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _0800FB4E
	movs r0, #0xb
_0800FB0E:
	strb r0, [r5]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	b _0800FB54
_0800FB26:
	adds r1, r4, #0
	adds r1, #0x50
	movs r0, #4
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _0800FB4E
	movs r0, #0xd
	strb r0, [r5]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	movs r6, #1
_0800FB4E:
	cmp r6, #0
	bne _0800FB54
	b _0800FA18
_0800FB54:
	ldr r0, [r4, #0x20]
	ldr r1, [r0]
	ldrb r2, [r5]
	lsls r0, r2, #3
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r1, r1, r0
	ldr r0, _0800FB98 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldrb r1, [r1, #0x14]
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	mov r3, r8
	ldrb r3, [r3]
	ands r1, r3
	orrs r1, r0
	mov r0, r8
	strb r1, [r0]
	ldr r0, [r4, #0x48]
	movs r1, #0xc0
	ands r0, r1
	lsrs r0, r0, #6
	ldr r1, _0800FB9C @ =gStaticData_0816BB94
	adds r0, r0, r1
	ldrb r0, [r0]
	mov r1, sb
	strb r0, [r1]
	b _0800FC5C
	.align 2, 0
_0800FB98: .4byte gUnknown_030012B8
_0800FB9C: .4byte gStaticData_0816BB94
_0800FBA0:
	movs r1, #7
	ands r1, r2
	movs r0, #4
	orrs r1, r0
	movs r0, #0xf8
	ands r0, r2
	orrs r1, r0
	str r1, [r4, #0x48]
	movs r0, #1
	mov r2, sb
	strb r0, [r2]
	ldrb r0, [r5]
	cmp r0, #0xc
	beq _0800FBE2
	cmp r0, #0xa
	bne _0800FBC4
	movs r0, #8
	b _0800FBE4
_0800FBC4:
	ldr r0, [r4, #0x48]
	movs r1, #0xc0
	ands r0, r1
	lsrs r0, r0, #6
	cmp r0, #2
	beq _0800FBE2
	cmp r0, #2
	ble _0800FBDA
	cmp r0, #3
	beq _0800FBFC
	b _0800FC14
_0800FBDA:
	cmp r0, #0
	blt _0800FC14
	movs r0, #0xc
	b _0800FBE4
_0800FBE2:
	movs r0, #0xa
_0800FBE4:
	strb r0, [r5]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	b _0800FC14
_0800FBFC:
	movs r0, #8
	strb r0, [r5]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
_0800FC14:
	ldr r0, [r4, #0x20]
	ldr r1, [r0]
	ldrb r3, [r5]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	adds r1, r1, r0
	ldr r0, _0800FC68 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldrb r1, [r1, #0x14]
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
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
	ldr r0, [r4, #0x48]
	movs r1, #0xc0
	ands r0, r1
	cmp r0, #0
	beq _0800FC5C
	ldr r0, _0800FC6C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x10
	bl PlaySfx
_0800FC5C:
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800FC68: .4byte gUnknown_030012B8
_0800FC6C: .4byte gUnknown_030012BC

	thumb_func_start sub_800FC70
sub_800FC70: @ 0x0800FC70
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	ldr r7, [r5, #0x44]
	cmp r7, #0
	bne _0800FC82
	b _0800FDBA
_0800FC82:
	ldr r0, _0800FCB4 @ =gUnknown_030012B0
	movs r1, #1
	strb r1, [r0]
	adds r0, r5, #0
	adds r0, #0x4c
	movs r6, #0
	ldrsb r6, [r0, r6]
	mov sb, r0
	cmp r6, #0
	bne _0800FC98
	movs r6, #1
_0800FC98:
	movs r0, #0
	mov r8, r0
	ldr r1, [r5]
	cmp r6, #0
	bge _0800FCBC
	ldr r2, _0800FCB8 @ =0xFFFFFF00
	movs r0, #0x80
	lsls r0, r0, #1
_0800FCA8:
	add r8, r2
	adds r7, r7, r0
	adds r6, #1
	cmp r6, #0
	bne _0800FCA8
	b _0800FD86
	.align 2, 0
_0800FCB4: .4byte gUnknown_030012B0
_0800FCB8: .4byte 0xFFFFFF00
_0800FCBC:
	cmp r7, #0
	ble _0800FCF0
	ldr r0, _0800FCD8 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #1
	bne _0800FCDC
	movs r1, #0x40
	add r8, r1
	subs r7, #0x40
	subs r6, #1
	b _0800FD80
	.align 2, 0
_0800FCD8: .4byte gUnknown_030012D8
_0800FCDC:
	movs r0, #0x80
	lsls r0, r0, #1
	add r8, r0
	ldr r1, _0800FCEC @ =0xFFFFFF00
	adds r7, r7, r1
	subs r6, #1
	b _0800FD80
	.align 2, 0
_0800FCEC: .4byte 0xFFFFFF00
_0800FCF0:
	movs r6, #1
	ldr r1, [r5, #0x40]
	str r1, [r5, #4]
	movs r0, #0
	mov r8, r0
	str r7, [r5, #0x44]
	ldr r0, _0800FD30 @ =gStaticData_0816BBC4
	adds r1, r5, #0
	adds r1, #0x4e
	ldrb r1, [r1]
	adds r0, r1, r0
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800FD54
	ldr r0, [r5, #0x48]
	cmp r0, #0
	bne _0800FD16
	cmp r1, #0xa
	bne _0800FD34
_0800FD16:
	adds r1, r5, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800FD54
	adds r0, r5, #0
	movs r1, #0
	bl sub_800EEF0
	b _0800FD54
	.align 2, 0
_0800FD30: .4byte gStaticData_0816BBC4
_0800FD34:
	cmp r1, #0xe
	bne _0800FD54
	adds r0, r5, #0
	bl sub_801070C
	adds r4, r0, #0
	adds r0, r5, #0
	bl sub_8010708
	cmp r4, #0
	bne _0800FD4E
	cmp r0, #0
	bne _0800FD54
_0800FD4E:
	adds r0, r5, #0
	bl sub_800E620
_0800FD54:
	adds r0, r5, #0
	bl sub_8010708
	adds r4, r0, #0
	subs r6, #1
	cmp r4, #0
	beq _0800FD80
	b _0800FD76
_0800FD64:
	adds r0, r4, #0
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #0xe
	bne _0800FD74
	adds r0, r4, #0
	bl sub_800E620
_0800FD74:
	adds r0, r4, #0
_0800FD76:
	bl sub_8010708
	adds r4, r0, #0
	cmp r4, #0
	bne _0800FD64
_0800FD80:
	ldr r1, [r5]
	cmp r6, #0
	bne _0800FCBC
_0800FD86:
	ldr r0, [r5, #4]
	add r0, r8
	str r1, [r5]
	str r0, [r5, #4]
	str r7, [r5, #0x44]
	cmp r7, #0
	bne _0800FD9A
	mov r1, sb
	strb r7, [r1]
	b _0800FDBA
_0800FD9A:
	mov r2, sb
	ldrb r1, [r2]
	adds r1, #1
	strb r1, [r2]
	lsls r0, r1, #0x18
	cmp r0, #0
	bne _0800FDAC
	adds r0, r1, #1
	strb r0, [r2]
_0800FDAC:
	mov r1, sb
	movs r0, #0
	ldrsb r0, [r1, r0]
	cmp r0, #5
	ble _0800FDBA
	movs r0, #5
	strb r0, [r1]
_0800FDBA:
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_800FDC8
sub_800FDC8: @ 0x0800FDC8
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r7, [sp, #0x14]
	cmp r1, r3
	ble _0800FDDE
	adds r0, r1, #0
	adds r1, r3, #0
	adds r3, r0, #0
	adds r0, r4, #0
	adds r4, r2, #0
	adds r2, r0, #0
_0800FDDE:
	subs r2, r2, r4
	subs r3, r3, r1
	cmp r2, #0
	ble _0800FE46
	cmp r2, r3
	ble _0800FE1A
	lsls r3, r3, #1
	lsls r0, r2, #1
	subs r6, r3, r0
	subs r0, r3, r2
	subs r2, #1
	movs r5, #1
	rsbs r5, r5, #0
	cmp r2, r5
	beq _0800FEA6
_0800FDFC:
	cmp r0, #0
	blt _0800FE06
	adds r1, #1
	adds r0, r0, r6
	b _0800FE08
_0800FE06:
	adds r0, r0, r3
_0800FE08:
	adds r4, #1
	cmp r4, r7
	bge _0800FE16
	subs r2, #1
	cmp r2, r5
	bne _0800FDFC
	b _0800FEA6
_0800FE16:
	adds r0, r1, #0
	b _0800FEAA
_0800FE1A:
	lsls r2, r2, #1
	lsls r0, r3, #1
	subs r6, r2, r0
	subs r0, r2, r3
	subs r3, #1
	movs r5, #1
	rsbs r5, r5, #0
	cmp r3, r5
	beq _0800FEA6
_0800FE2C:
	cmp r0, #0
	blt _0800FE3A
	adds r4, #1
	cmp r4, r7
	bge _0800FE78
	adds r0, r0, r6
	b _0800FE3C
_0800FE3A:
	adds r0, r0, r2
_0800FE3C:
	adds r1, #1
	subs r3, #1
	cmp r3, r5
	bne _0800FE2C
	b _0800FEA6
_0800FE46:
	rsbs r2, r2, #0
	cmp r2, r3
	ble _0800FE7C
	lsls r3, r3, #1
	lsls r0, r2, #1
	subs r6, r3, r0
	subs r0, r3, r2
	subs r2, #1
	movs r5, #1
	rsbs r5, r5, #0
	cmp r2, r5
	beq _0800FEA6
_0800FE5E:
	cmp r0, #0
	blt _0800FE68
	adds r1, #1
	adds r0, r0, r6
	b _0800FE6A
_0800FE68:
	adds r0, r0, r3
_0800FE6A:
	subs r4, #1
	cmp r4, r7
	bge _0800FE78
	subs r2, #1
	cmp r2, r5
	bne _0800FE5E
	b _0800FEA6
_0800FE78:
	adds r0, r1, #0
	b _0800FEAA
_0800FE7C:
	lsls r2, r2, #1
	lsls r0, r3, #1
	subs r6, r2, r0
	subs r0, r2, r3
	subs r3, #1
	movs r5, #1
	rsbs r5, r5, #0
	cmp r3, r5
	beq _0800FEA6
_0800FE8E:
	cmp r0, #0
	blt _0800FE9C
	subs r4, #1
	cmp r4, r7
	bge _0800FE78
	adds r0, r0, r6
	b _0800FE9E
_0800FE9C:
	adds r0, r0, r2
_0800FE9E:
	adds r1, #1
	subs r3, #1
	cmp r3, r5
	bne _0800FE8E
_0800FEA6:
	movs r0, #1
	rsbs r0, r0, #0
_0800FEAA:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_800FEB0
sub_800FEB0: @ 0x0800FEB0
	push {r4, lr}
	adds r2, r0, #0
	movs r0, #4
	ldrb r1, [r2, #0xc]
	orrs r0, r1
	movs r1, #0x40
	orrs r0, r1
	strb r0, [r2, #0xc]
	adds r3, r2, #0
	adds r3, #0x4d
	movs r0, #0x7f
	ldrb r4, [r3]
	ands r0, r4
	movs r1, #0
	strb r0, [r3]
	ldr r0, _0800FF08 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x80
	strb r1, [r0]
	movs r0, #0x80
	ldrb r4, [r3]
	ands r0, r4
	strb r0, [r3]
	str r1, [r2, #0x44]
	adds r0, r2, #0
	adds r0, #0x4c
	strb r1, [r0]
	str r1, [r2, #0x48]
	adds r0, #3
	strb r1, [r0]
	adds r0, #1
	strb r1, [r0]
	adds r0, #1
	strb r1, [r0]
	adds r0, #7
	strb r1, [r0]
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r2, #0x54]
	str r1, [r2, #0x5c]
	str r1, [r2, #0x60]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0800FF08: .4byte gUnknown_030012D8

	thumb_func_start sub_800FF0C
sub_800FF0C: @ 0x0800FF0C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	ldr r4, [sp, #0x28]
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	mov sb, r0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	str r1, [sp]
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	str r2, [sp, #4]
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r4, r4, #0x18
	lsrs r7, r4, #0x18
	movs r0, #0x64
	bl sub_8026EDC
	adds r4, r0, #0
	bl sub_80084A4
	ldr r0, _0800FFC8 @ =gStaticData_087E4074
	str r0, [r4, #0x18]
	adds r1, r4, #0
	adds r1, #0x59
	movs r0, #0
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_800FEB0
	adds r5, r4, #0
	mov r0, sb
	strh r0, [r5, #8]
	cmp r7, #9
	bne _0800FF76
	ldr r0, _0800FFCC @ =0x0000FFFF
	cmp sb, r0
	beq _0800FF76
	ldr r0, _0800FFD0 @ =gUnknown_030012B4
	ldr r0, [r0]
	mov r1, sb
	bl sub_802599C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0800FF76
	movs r7, #0
_0800FF76:
	ldr r6, _0800FFD4 @ =gUnknown_030012C0
	ldr r1, [r6]
	adds r0, r1, #0
	adds r0, #0x8c
	ldrb r0, [r0]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	cmp r0, #0
	bne _08010018
	adds r0, r1, #0
	bl sub_80232E0
	adds r4, r0, #0
	ldr r0, [r6]
	bl sub_8023128
	cmp r4, r0
	blt _08010018
	cmp r7, #0xb
	bne _0800FFD8
	ldr r0, _0800FFD0 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	add r1, r8
	ldr r0, [r0, #0xc]
	ldrh r1, [r1]
	adds r0, r1, r0
	adds r2, r0, #0
	ldrb r1, [r2]
	movs r0, #0x40
	ands r0, r1
	cmp r0, #0
	bne _0800FFF6
	movs r0, #0x80
	ands r0, r1
	cmp r0, #0
	bne _08010008
	b _0801000C
	.align 2, 0
_0800FFC8: .4byte gStaticData_087E4074
_0800FFCC: .4byte 0x0000FFFF
_0800FFD0: .4byte gUnknown_030012B4
_0800FFD4: .4byte gUnknown_030012C0
_0800FFD8:
	cmp r7, #0xf
	bne _08010018
	ldr r0, _0800FFFC @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r0, r8
	ldr r1, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r1
	ldrb r1, [r2]
	movs r0, #0x40
	ands r0, r1
	cmp r0, #0
	beq _08010000
_0800FFF6:
	movs r7, #2
	b _08010018
	.align 2, 0
_0800FFFC: .4byte gUnknown_030012B4
_08010000:
	movs r0, #0x80
	ands r0, r1
	cmp r0, #0
	beq _0801000C
_08010008:
	movs r7, #1
	b _08010018
_0801000C:
	movs r0, #1
	ldrb r2, [r2, #1]
	ands r0, r2
	cmp r0, #0
	beq _08010018
	movs r7, #9
_08010018:
	movs r4, #0
	ldr r0, _0801003C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r3, #0xba
	lsls r3, r3, #1
	adds r0, r0, r3
	str r0, [r5, #0x20]
	subs r0, r7, #1
	cmp r0, #0xe
	bhi _08010096
	lsls r0, r0, #2
	ldr r1, _08010040 @ =_08010044
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801003C: .4byte gUnknown_030012D0
_08010040: .4byte _08010044
_08010044: @ jump table
	.4byte _08010080 @ case 0
	.4byte _08010096 @ case 1
	.4byte _08010084 @ case 2
	.4byte _08010096 @ case 3
	.4byte _08010096 @ case 4
	.4byte _08010096 @ case 5
	.4byte _08010096 @ case 6
	.4byte _08010096 @ case 7
	.4byte _08010080 @ case 8
	.4byte _08010096 @ case 9
	.4byte _08010080 @ case 10
	.4byte _08010080 @ case 11
	.4byte _08010096 @ case 12
	.4byte _08010096 @ case 13
	.4byte _08010080 @ case 14
_08010080:
	movs r4, #1
	b _08010096
_08010084:
	ldr r0, _080100C4 @ =gUnknown_030012B4
	ldr r0, [r0]
	mov r1, sb
	bl sub_802599C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08010096
	movs r7, #7
_08010096:
	movs r6, #0
	ldr r2, _080100C4 @ =gUnknown_030012B4
	ldr r0, [r2]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r0, r8
	ldr r1, [r1, #0xc]
	ldrh r0, [r0]
	adds r1, r0, r1
	adds r3, r1, #0
	cmp r4, #0
	bne _080100B8
	movs r0, #0x20
	ldrb r4, [r1]
	ands r0, r4
	cmp r0, #0
	beq _080100E2
_080100B8:
	movs r6, #1
	ldrh r1, [r1, #4]
	cmp r1, #0x1b
	bne _080100C8
	movs r0, #0x15
	b _080100CC
	.align 2, 0
_080100C4: .4byte gUnknown_030012B4
_080100C8:
	movs r1, #4
	ldrsh r0, [r3, r1]
_080100CC:
	str r0, [r5, #0x54]
	ldr r0, _080100F4 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _080100E2
	ldr r0, [r5, #0x54]
	subs r0, #0x15
	lsls r0, r0, #0x18
	lsrs r7, r0, #0x18
_080100E2:
	cmp r7, #0x12
	bls _080100E8
	b _08010372
_080100E8:
	lsls r0, r7, #2
	ldr r1, _080100F8 @ =_080100FC
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080100F4: .4byte gUnknown_030012C0
_080100F8: .4byte _080100FC
_080100FC: @ jump table
	.4byte _08010148 @ case 0
	.4byte _0801014C @ case 1
	.4byte _0801016C @ case 2
	.4byte _08010170 @ case 3
	.4byte _080101B4 @ case 4
	.4byte _080101B8 @ case 5
	.4byte _080101FA @ case 6
	.4byte _080101FE @ case 7
	.4byte _08010202 @ case 8
	.4byte _08010206 @ case 9
	.4byte _08010244 @ case 10
	.4byte _08010248 @ case 11
	.4byte _0801027C @ case 12
	.4byte _08010286 @ case 13
	.4byte _0801028A @ case 14
	.4byte _0801028E @ case 15
	.4byte _08010334 @ case 16
	.4byte _08010338 @ case 17
	.4byte _08010356 @ case 18
_08010148:
	movs r0, #0x1f
	b _0801033A
_0801014C:
	ldr r0, [r2]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	add r1, r8
	ldr r0, [r0, #0xc]
	ldrh r1, [r1]
	adds r0, r1, r0
	ldrb r0, [r0]
	lsls r0, r0, #0x19
	lsrs r0, r0, #0x1f
	adds r1, r5, #0
	adds r1, #0x50
	strb r0, [r1]
	movs r0, #0x1a
	subs r1, #0x23
	b _0801033E
_0801016C:
	movs r0, #0x17
	b _0801033A
_08010170:
	ldr r0, [r2]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r0, r8
	ldr r4, [r1, #0xc]
	ldrh r0, [r0]
	adds r4, r0, r4
	movs r0, #3
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
	ldrb r1, [r4, #6]
	adds r0, r5, #0
	adds r0, #0x50
	strb r1, [r0]
	ldrb r0, [r4, #7]
	adds r1, r5, #0
	adds r1, #0x51
	strb r0, [r1]
	ldrb r1, [r4, #8]
	adds r0, r5, #0
	adds r0, #0x4c
	strb r1, [r0]
	b _08010372
_080101B4:
	movs r0, #0x18
	b _0801033A
_080101B8:
	ldr r0, [r2]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r0, r8
	ldr r4, [r1, #0xc]
	ldrh r0, [r0]
	adds r4, r0, r4
	movs r0, #0x15
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
	ldrb r1, [r4, #6]
	adds r0, r5, #0
	adds r0, #0x50
	strb r1, [r0]
	ldrb r0, [r4, #7]
	adds r1, r5, #0
	adds r1, #0x51
	strb r0, [r1]
	movs r2, #8
	ldrsh r0, [r4, r2]
	str r0, [r5, #0x48]
	b _08010372
_080101FA:
	movs r0, #4
	b _0801033A
_080101FE:
	movs r0, #0x20
	b _0801033A
_08010202:
	movs r0, #2
	b _0801033A
_08010206:
	movs r0, #0x1c
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
	cmp r6, #0
	beq _08010228
	b _08010372
_08010228:
	movs r0, #0x15
	str r0, [r5, #0x54]
	ldr r0, _08010240 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0801023A
	b _08010372
_0801023A:
	movs r7, #0
	b _08010372
	.align 2, 0
_08010240: .4byte gUnknown_030012C0
_08010244:
	movs r0, #5
	b _0801033A
_08010248:
	ldr r0, [r2]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r0, r8
	ldr r4, [r1, #0xc]
	ldrh r0, [r0]
	adds r4, r0, r4
	movs r0, #0
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
	ldrb r0, [r4, #6]
	adds r1, r5, #0
	adds r1, #0x51
	strb r0, [r1]
	b _08010372
_0801027C:
	movs r0, #0x2a
	rsbs r0, r0, #0
	str r0, [r5, #0x48]
	movs r0, #0x19
	b _0801033A
_08010286:
	movs r0, #6
	b _0801033A
_0801028A:
	movs r0, #0x11
	b _0801033A
_0801028E:
	ldr r0, [r2]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r0, r8
	ldr r1, [r1, #0xc]
	ldrh r0, [r0]
	adds r6, r0, r1
	mov sl, r6
	ldr r0, [r5, #0x20]
	ldr r1, [r0]
	adds r1, #0xe0
	ldr r0, _0801032C @ =gUnknown_030012B8
	ldr r0, [r0]
	ldrb r1, [r1, #0x14]
	bl sub_8006DF8
	ldr r0, [r5, #0x48]
	movs r1, #0x3f
	ands r0, r1
	movs r1, #0xf8
	ands r0, r1
	str r0, [r5, #0x48]
	movs r0, #7
	adds r1, r5, #0
	adds r1, #0x2d
	movs r4, #0
	strb r0, [r1]
	adds r0, r5, #0
	bl sub_80087C0
	adds r0, r5, #0
	bl sub_80087B4
	adds r0, r5, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r5, #0x48]
	movs r1, #0x38
	ands r0, r1
	lsrs r0, r0, #3
	ldr r1, _08010330 @ =gStaticData_0816BB94
	adds r0, r0, r1
	ldrb r0, [r0]
	adds r1, r5, #0
	adds r1, #0x4f
	strb r0, [r1]
	ldrb r0, [r6, #6]
	adds r1, #2
	strb r0, [r1]
	subs r1, #1
	strb r4, [r1]
	movs r0, #2
	ldrb r3, [r6, #1]
	ands r0, r3
	cmp r0, #0
	beq _08010304
	movs r0, #1
	strb r0, [r1]
_08010304:
	movs r0, #4
	ldrb r6, [r6, #1]
	ands r0, r6
	cmp r0, #0
	beq _08010316
	movs r0, #2
	ldrb r4, [r1]
	orrs r0, r4
	strb r0, [r1]
_08010316:
	movs r0, #8
	mov r2, sl
	ldrb r2, [r2, #1]
	ands r0, r2
	cmp r0, #0
	beq _08010372
	movs r0, #4
	ldrb r3, [r1]
	orrs r0, r3
	strb r0, [r1]
	b _08010372
	.align 2, 0
_0801032C: .4byte gUnknown_030012B8
_08010330: .4byte gStaticData_0816BB94
_08010334:
	movs r0, #0xe
	b _0801033A
_08010338:
	movs r0, #0xf
_0801033A:
	adds r1, r5, #0
	adds r1, #0x2d
_0801033E:
	strb r0, [r1]
	adds r0, r5, #0
	bl sub_80087C0
	adds r0, r5, #0
	bl sub_80087B4
	adds r0, r5, #0
	movs r1, #0
	bl sub_800872C
	b _08010372
_08010356:
	movs r0, #0x10
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
_08010372:
	adds r2, r5, #0
	adds r2, #0x28
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r4, [r2]
	ands r0, r4
	movs r1, #0x21
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r2]
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
	ldr r4, [sp]
	lsls r0, r4, #8
	str r0, [r5]
	ldr r1, [sp, #4]
	lsls r0, r1, #8
	str r0, [r5, #4]
	ldr r4, _08010474 @ =gUnknown_030012B4
	ldr r0, [r4]
	mov r1, sb
	bl sub_802599C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _080103DE
	cmp r7, #0xb
	beq _080103C4
	cmp r7, #0xf
	bne _080103DE
_080103C4:
	ldr r0, [r4]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r0, r8
	ldr r1, [r1, #0xc]
	ldrh r0, [r0]
	adds r1, r0, r1
	movs r0, #0x80
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _080103DE
	movs r7, #1
_080103DE:
	cmp r7, #1
	bne _08010436
	ldr r0, _08010478 @ =0x0000FFFF
	cmp sb, r0
	beq _08010436
	ldr r0, _08010474 @ =gUnknown_030012B4
	ldr r0, [r0]
	mov r1, sb
	bl sub_802599C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08010436
	movs r0, #0x1b
	adds r4, r5, #0
	adds r4, #0x2d
	strb r0, [r4]
	adds r0, r5, #0
	bl sub_80087C0
	adds r0, r5, #0
	bl sub_80087B4
	adds r0, r5, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r5, #0x20]
	ldr r1, [r0]
	ldrb r2, [r4]
	lsls r0, r2, #3
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	subs r0, #1
	str r0, [r5, #0x30]
	adds r1, r5, #0
	adds r1, #0x4d
	movs r0, #0x80
	ldrb r3, [r1]
	ands r0, r3
	orrs r0, r7
	strb r0, [r1]
_08010436:
	adds r0, r5, #0
	adds r0, #0x4e
	strb r7, [r0]
	cmp r7, #5
	bne _08010456
	ldr r0, _08010474 @ =gUnknown_030012B4
	ldr r0, [r0]
	mov r1, sb
	bl sub_802599C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08010456
	adds r0, r5, #0
	bl sub_800F5B8
_08010456:
	ldr r0, _0801047C @ =gUnknown_0300130C
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8009B70
	adds r0, r5, #0
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08010474: .4byte gUnknown_030012B4
_08010478: .4byte 0x0000FFFF
_0801047C: .4byte gUnknown_0300130C

	thumb_func_start sub_8010480
sub_8010480: @ 0x08010480
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r0, #0x4d
	ldrb r2, [r0]
	movs r0, #0x80
	ands r0, r2
	cmp r0, #0
	bne _080104BC
	movs r1, #0x7f
	ands r1, r2
	cmp r1, #0
	bne _080104BC
	adds r0, r4, #0
	adds r0, #0x38
	strb r1, [r0]
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
	blt _080104BA
	subs r3, r0, #1
_080104BA:
	str r3, [r4, #0x30]
_080104BC:
	ldr r0, _080104E0 @ =gUnknown_030012CC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8007A84
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _080104DA
	movs r0, #9
	rsbs r0, r0, #0
	ldrb r1, [r4, #0xc]
	ands r0, r1
	strb r0, [r4, #0xc]
_080104DA:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080104E0: .4byte gUnknown_030012CC

	thumb_func_start sub_80104E4
sub_80104E4: @ 0x080104E4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r4, r0, #0
	adds r1, r4, #0
	adds r1, #0x4f
	ldrb r0, [r1]
	cmp r0, #0
	beq _0801055C
	subs r0, #1
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x4e
	ldrb r1, [r0]
	adds r6, r0, #0
	cmp r1, #0x12
	ble _0801051C
	cmp r1, #0x15
	bgt _0801051C
	adds r0, r4, #0
	bl sub_800F8E0
	ldr r1, _08010518 @ =gUnknown_030012B0
	movs r0, #1
	strb r0, [r1]
	b _0801055C
	.align 2, 0
_08010518: .4byte gUnknown_030012B0
_0801051C:
	ldrb r6, [r6]
	cmp r6, #0xf
	bne _08010538
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _08010538
	adds r0, r4, #0
	bl sub_800F990
	b _0801055C
_08010538:
	adds r0, r4, #0
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #0xc
	bne _08010552
	adds r0, r4, #0
	adds r0, #0x4f
	ldrb r1, [r0]
	cmp r1, #0
	bne _0801055C
	adds r0, #1
	strb r1, [r0]
	b _0801055C
_08010552:
	cmp r0, #3
	bne _0801055C
	adds r0, r4, #0
	bl sub_800F4F4
_0801055C:
	adds r0, r4, #0
	adds r0, #0x4e
	adds r6, r0, #0
	ldrb r0, [r6]
	cmp r0, #0xc
	bne _08010572
	ldr r0, [r4, #0x48]
	cmp r0, #0
	ble _08010572
	subs r0, #1
	str r0, [r4, #0x48]
_08010572:
	adds r0, r4, #0
	bl sub_800FC70
	adds r2, r4, #0
	adds r2, #0x4d
	ldrb r1, [r2]
	movs r0, #0x80
	ands r0, r1
	cmp r0, #0
	beq _08010646
	movs r1, #0x38
	adds r1, r1, r4
	mov r8, r1
	ldrb r0, [r1]
	cmp r0, #0
	beq _08010654
	movs r3, #0
	mov ip, r3
	ldr r0, [r4, #0x20]
	adds r5, r4, #0
	adds r5, #0x2d
	ldr r1, [r0]
	ldrb r7, [r5]
	lsls r0, r7, #3
	subs r0, r0, r7
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _080105B2
	subs r0, #1
	mov ip, r0
_080105B2:
	mov r0, ip
	str r0, [r4, #0x30]
	mov r1, r8
	strb r3, [r1]
	movs r0, #0x7f
	ldrb r7, [r2]
	ands r0, r7
	strb r0, [r2]
	ldr r0, _08010620 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x80
	strb r3, [r0]
	ldrb r0, [r6]
	cmp r0, #6
	bne _08010628
	movs r0, #7
	strb r0, [r6]
	movs r0, #0x20
	strb r0, [r5]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r4, #0x20]
	ldr r1, [r0]
	ldrb r2, [r5]
	lsls r0, r2, #3
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r1, r1, r0
	ldr r0, _08010624 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldrb r1, [r1, #0x14]
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
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
	b _08010654
	.align 2, 0
_08010620: .4byte gUnknown_030012D8
_08010624: .4byte gUnknown_030012B8
_08010628:
	cmp r0, #3
	bne _08010654
	movs r0, #0x20
	strb r0, [r5]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	b _08010654
_08010646:
	movs r0, #0x7f
	ands r0, r1
	cmp r0, #1
	bne _08010654
	adds r0, r4, #0
	bl sub_800F798
_08010654:
	adds r0, r4, #0
	bl sub_8008044
	ldr r1, [r4, #0x18]
	adds r1, #0x60
	movs r7, #0
	ldrsh r0, [r1, r7]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8010674
sub_8010674: @ 0x08010674
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	ldrb r0, [r5, #0xc]
	lsrs r1, r0, #4
	movs r0, #1
	ands r1, r0
	ldr r0, [r5, #0x44]
	cmp r0, #0
	beq _0801068A
	movs r1, #1
_0801068A:
	cmp r1, #0
	bne _080106D2
	ldr r1, [r5, #0x18]
	movs r2, #0x10
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r1, [r1, #0x14]
	bl sub_803AD7C
	ldrb r1, [r0, #4]
	lsls r2, r1, #7
	ldrb r0, [r0, #5]
	lsls r3, r0, #7
	ldr r1, [r5]
	subs r4, r1, r2
	ldr r0, [r5, #4]
	subs r5, r0, r3
	adds r1, r1, r2
	adds r3, r0, r3
	movs r7, #0
	ldr r2, [r6]
	cmp r4, r2
	ble _080106D0
	ldr r0, [r6, #8]
	adds r0, r2, r0
	cmp r1, r0
	bge _080106D0
	ldr r2, [r6, #4]
	cmp r5, r2
	ble _080106D0
	ldr r0, [r6, #0xc]
	adds r0, r2, r0
	cmp r3, r0
	bge _080106D0
	movs r7, #1
_080106D0:
	adds r1, r7, #0
_080106D2:
	adds r0, r1, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_80106DC
sub_80106DC: @ 0x080106DC
	push {r4, lr}
	ldr r4, _08010704 @ =gUnknown_030012D8
	ldr r0, [r4]
	movs r1, #0x84
	lsls r1, r1, #1
	adds r0, r0, r1
	bl sub_8010B6C
	ldr r0, [r4]
	adds r1, r0, #0
	adds r1, #0x92
	ldrb r0, [r1]
	cmp r0, #0
	beq _080106FC
	adds r0, #1
	strb r0, [r1]
_080106FC:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08010704: .4byte gUnknown_030012D8

	thumb_func_start sub_8010708
sub_8010708: @ 0x08010708
	ldr r0, [r0, #0x60]
	bx lr

	thumb_func_start sub_801070C
sub_801070C: @ 0x0801070C
	ldr r0, [r0, #0x5c]
	bx lr

	thumb_func_start sub_8010710
sub_8010710: @ 0x08010710
	str r1, [r0, #0x60]
	bx lr

	thumb_func_start sub_8010714
sub_8010714: @ 0x08010714
	str r1, [r0, #0x5c]
	bx lr

	thumb_func_start sub_8010718
sub_8010718: @ 0x08010718
	movs r0, #3
	bx lr

	thumb_func_start sub_801071C
sub_801071C: @ 0x0801071C
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, _08010758 @ =gStaticData_087E4074
	str r0, [r4, #0x18]
	adds r0, r4, #0
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #3
	bne _0801074A
	ldr r1, [r4, #0x48]
	adds r0, r1, #1
	cmp r0, #1
	bls _0801074A
	cmp r1, #0
	beq _08010742
	adds r0, r1, #0
	bl sub_8026EB4
_08010742:
	adds r1, r4, #0
	adds r1, #0x59
	movs r0, #0
	strb r0, [r1]
_0801074A:
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_8008484
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08010758: .4byte gStaticData_087E4074

	thumb_func_start sub_801075C
sub_801075C: @ 0x0801075C
	push {r4, lr}
	adds r4, r0, #0
	bl sub_80084A4
	ldr r0, _08010780 @ =gStaticData_087E4074
	str r0, [r4, #0x18]
	adds r1, r4, #0
	adds r1, #0x59
	movs r0, #0
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_800FEB0
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08010780: .4byte gStaticData_087E4074

	thumb_func_start sub_8010784
sub_8010784: @ 0x08010784
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	lsls r2, r2, #1
	lsls r0, r3, #1
	subs r6, r2, r0
	subs r0, r2, r3
	subs r3, #1
	movs r5, #1
	rsbs r5, r5, #0
	cmp r3, r5
	beq _080107BA
_0801079A:
	cmp r0, #0
	blt _080107B0
	ldr r7, [sp, #0x14]
	adds r4, r4, r7
	ldr r7, [sp, #0x18]
	cmp r4, r7
	blt _080107AC
	adds r0, r1, #0
	b _080107BE
_080107AC:
	adds r0, r0, r6
	b _080107B2
_080107B0:
	adds r0, r0, r2
_080107B2:
	adds r1, #1
	subs r3, #1
	cmp r3, r5
	bne _0801079A
_080107BA:
	movs r0, #1
	rsbs r0, r0, #0
_080107BE:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_80107C4
sub_80107C4: @ 0x080107C4
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	lsls r3, r3, #1
	lsls r0, r2, #1
	subs r6, r3, r0
	subs r0, r3, r2
	subs r2, #1
	movs r5, #1
	rsbs r5, r5, #0
	cmp r2, r5
	beq _080107FA
_080107DA:
	cmp r0, #0
	blt _080107E4
	adds r1, #1
	adds r0, r0, r6
	b _080107E6
_080107E4:
	adds r0, r0, r3
_080107E6:
	ldr r7, [sp, #0x14]
	adds r4, r4, r7
	ldr r7, [sp, #0x18]
	cmp r4, r7
	blt _080107F4
	adds r0, r1, #0
	b _080107FE
_080107F4:
	subs r2, #1
	cmp r2, r5
	bne _080107DA
_080107FA:
	movs r0, #1
	rsbs r0, r0, #0
_080107FE:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_8010804
sub_8010804: @ 0x08010804
	push {r4, r5, r6, lr}
	movs r5, #0
	ldr r1, _08010858 @ =gUnknown_0300130C
	ldr r0, [r1]
	ldr r0, [r0]
	cmp r5, r0
	bge _08010852
	adds r6, r1, #0
_08010814:
	ldr r0, [r6]
	ldr r1, [r0, #8]
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
	cmp r0, #3
	bne _08010848
	ldr r1, [r4, #0x54]
	movs r0, #1
	rsbs r0, r0, #0
	cmp r1, r0
	beq _08010848
	lsls r0, r1, #0x18
	lsrs r0, r0, #0x18
	str r0, [r4, #0x48]
	adds r0, r4, #0
	bl sub_800F5B8
_08010848:
	adds r5, #1
	ldr r0, [r6]
	ldr r0, [r0]
	cmp r5, r0
	blt _08010814
_08010852:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08010858: .4byte gUnknown_0300130C

	thumb_func_start sub_801085C
sub_801085C: @ 0x0801085C
	push {r4, lr}
	ldr r0, _08010894 @ =gUnknown_030012D8
	ldr r2, [r0]
	ldrb r1, [r2, #0xc]
	lsrs r0, r1, #7
	cmp r0, #0
	beq _0801088E
	ldr r1, [r2, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0x1a
	movs r3, #0
	bl sub_803AD88
	ldr r0, _08010898 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #1
	bl PlaySfx
_0801088E:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08010894: .4byte gUnknown_030012D8
_08010898: .4byte gUnknown_030012BC

	thumb_func_start sub_801089C
sub_801089C: @ 0x0801089C
	push {r4, r5, r6, lr}
	sub sp, #8
	adds r4, r0, #0
	lsls r1, r1, #0x18
	lsrs r6, r1, #0x18
	ldr r0, _080108F8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	ldrh r1, [r4, #8]
	ldr r0, _080108FC @ =0x0000FFFF
	cmp r1, r0
	beq _080108D2
	ldr r5, _08010900 @ =gUnknown_030012B4
	ldr r0, [r5]
	bl sub_802599C
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _080108D2
	ldr r0, [r5]
	ldrh r1, [r4, #8]
	bl sub_80259D4
_080108D2:
	ldr r1, [r4]
	asrs r1, r1, #8
	ldr r2, [r4, #4]
	asrs r2, r2, #8
	adds r2, #3
	ldr r0, _08010904 @ =gUnknown_030012E4
	ldr r0, [r0]
	movs r3, #3
	str r3, [sp]
	add r3, sp, #4
	strb r6, [r3]
	movs r3, #0
	bl sub_8025A64
	add sp, #8
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_080108F8: .4byte gUnknown_030012BC
_080108FC: .4byte 0x0000FFFF
_08010900: .4byte gUnknown_030012B4
_08010904: .4byte gUnknown_030012E4

	thumb_func_start sub_8010908
sub_8010908: @ 0x08010908
	ldr r0, _08010910 @ =gStaticData_0816BBAE
	adds r1, r1, r0
	ldrb r0, [r1]
	bx lr
	.align 2, 0
_08010910: .4byte gStaticData_0816BBAE

	thumb_func_start sub_8010914
sub_8010914: @ 0x08010914
	push {r4, r5, lr}
	adds r5, r0, #0
	bl sub_801070C
	adds r4, r0, #0
	cmp r4, #0
	beq _08010930
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #1
	bne _08010938
_08010930:
	adds r0, r5, #0
	b _08010956
_08010934:
	adds r0, r4, #0
	b _08010956
_08010938:
	adds r0, r4, #0
	bl sub_801070C
	adds r2, r0, #0
	cmp r2, #0
	beq _08010934
	adds r1, r2, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #1
	beq _08010934
	adds r4, r2, #0
	b _08010938
_08010956:
	pop {r4, r5}
	pop {r1}
	bx r1

	thumb_func_start sub_801095C
sub_801095C: @ 0x0801095C
	push {r4, r5, lr}
	adds r5, r0, #0
	bl sub_8010708
	adds r4, r0, #0
	cmp r4, #0
	beq _08010978
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #1
	bne _08010980
_08010978:
	adds r0, r5, #0
	b _0801099E
_0801097C:
	adds r0, r4, #0
	b _0801099E
_08010980:
	adds r0, r4, #0
	bl sub_8010708
	adds r2, r0, #0
	cmp r2, #0
	beq _0801097C
	adds r1, r2, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #1
	beq _0801097C
	adds r4, r2, #0
	b _08010980
_0801099E:
	pop {r4, r5}
	pop {r1}
	bx r1

	thumb_func_start sub_80109A4
sub_80109A4: @ 0x080109A4
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	movs r0, #0x4d
	adds r0, r0, r4
	mov ip, r0
	movs r0, #0x7f
	mov r5, ip
	ldrb r5, [r5]
	ands r0, r5
	cmp r0, #1
	beq _080109E8
	ldr r0, [r4]
	subs r0, r0, r2
	cmp r0, #0
	bge _080109C4
	rsbs r0, r0, #0
_080109C4:
	ldr r2, _080109FC @ =0x00003FFF
	cmp r0, r2
	bgt _080109E8
	ldr r0, [r4, #4]
	subs r0, r0, r3
	cmp r0, #0
	bge _080109D4
	rsbs r0, r0, #0
_080109D4:
	cmp r0, r2
	bgt _080109E8
	adds r0, r4, #0
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #5
	beq _080109E8
	adds r0, r4, #0
	bl sub_0800D18C
_080109E8:
	movs r0, #9
	rsbs r0, r0, #0
	ldrb r6, [r4, #0xc]
	ands r0, r6
	strb r0, [r4, #0xc]
	movs r0, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_080109FC: .4byte 0x00003FFF

	thumb_func_start sub_8010A00
sub_8010A00: @ 0x08010A00
	ldr r0, [r0, #0x48]
	movs r1, #0xc0
	ands r0, r1
	lsrs r0, r0, #6
	bx lr
	.align 2, 0

	thumb_func_start sub_8010A0C
sub_8010A0C: @ 0x08010A0C
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8010A00
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #0
	beq _08010A2C
	subs r0, #1
	lsls r0, r0, #0x18
	ldr r1, [r4, #0x48]
	movs r2, #0x3f
	ands r1, r2
	lsrs r0, r0, #0x12
	orrs r1, r0
	str r1, [r4, #0x48]
_08010A2C:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8010A34
sub_8010A34: @ 0x08010A34
	lsls r1, r1, #0x18
	ldr r2, [r0, #0x48]
	movs r3, #0x3f
	ands r2, r3
	lsrs r1, r1, #0x12
	orrs r2, r1
	str r2, [r0, #0x48]
	bx lr

	thumb_func_start sub_8010A44
sub_8010A44: @ 0x08010A44
	ldr r1, [r0, #0x48]
	movs r2, #0x3f
	ands r1, r2
	str r1, [r0, #0x48]
	bx lr
	.align 2, 0

	thumb_func_start sub_8010A50
sub_8010A50: @ 0x08010A50
	ldr r0, [r0, #0x48]
	movs r1, #0x38
	ands r0, r1
	lsrs r0, r0, #3
	bx lr
	.align 2, 0

	thumb_func_start sub_8010A5C
sub_8010A5C: @ 0x08010A5C
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8010A50
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #0
	beq _08010A7C
	subs r0, #1
	lsls r0, r0, #0x18
	ldr r1, [r4, #0x48]
	movs r2, #0xc7
	ands r1, r2
	lsrs r0, r0, #0x15
	orrs r1, r0
	str r1, [r4, #0x48]
_08010A7C:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8010A84
sub_8010A84: @ 0x08010A84
	lsls r1, r1, #0x18
	ldr r2, [r0, #0x48]
	movs r3, #0xc7
	ands r2, r3
	lsrs r1, r1, #0x15
	orrs r2, r1
	str r2, [r0, #0x48]
	bx lr

	thumb_func_start sub_8010A94
sub_8010A94: @ 0x08010A94
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	ldr r2, [r0, #0x48]
	movs r3, #0xf8
	ands r2, r3
	orrs r2, r1
	str r2, [r0, #0x48]
	bx lr

	thumb_func_start sub_8010AA4
sub_8010AA4: @ 0x08010AA4
	ldr r0, [r0, #0x48]
	movs r1, #7
	ands r0, r1
	bx lr

	thumb_func_start sub_8010AAC
sub_8010AAC: @ 0x08010AAC
	adds r0, #0x4e
	strb r1, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8010AB4
sub_8010AB4: @ 0x08010AB4
	adds r0, #0x4e
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8010ABC
sub_8010ABC: @ 0x08010ABC
	str r1, [r0, #0x44]
	bx lr

	thumb_func_start sub_8010AC0
sub_8010AC0: @ 0x08010AC0
	ldr r0, [r0, #0x44]
	bx lr

	thumb_func_start sub_8010AC4
sub_8010AC4: @ 0x08010AC4
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	adds r0, #0x4d
	movs r2, #0x80
	ldrb r3, [r0]
	ands r2, r3
	orrs r1, r2
	strb r1, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8010AD8
sub_8010AD8: @ 0x08010AD8
	adds r1, r0, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	bx lr

	thumb_func_start sub_8010AE4
sub_8010AE4: @ 0x08010AE4
	adds r0, #0x4c
	strb r1, [r0]
	bx lr
	.align 2, 0
	thumb_func_start sub_8010AEC
sub_8010AEC: @ 0x08010AEC
	adds	r0, #76	@ 0x4c
	ldrb	r0, [r0, #0]
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	bx	lr
	movs	r0, r0
	adds	r0, #77	@ 0x4d
	movs	r1, #128	@ 0x80
	ldrb	r0, [r0, #0]
	ands	r1, r0
	cmp	r1, #0
	bne _08010B08
	movs	r0, #0
	b _08010B0A
_08010B08:
	movs	r0, #1
_08010B0A:
	bx	lr

	thumb_func_start sub_8010B0C
sub_8010B0C: @ 0x08010B0C
	adds r0, #0x4d
	movs r1, #0x80
	ldrb r2, [r0]
	orrs r1, r2
	strb r1, [r0]
	ldr r0, _08010B24 @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r1, #1
	adds r0, #0x80
	strb r1, [r0]
	bx lr
	.align 2, 0
_08010B24: .4byte gUnknown_030012D8

	thumb_func_start sub_8010B28
sub_8010B28: @ 0x08010B28
	adds r0, #0x4d
	movs r1, #0x7f
	ldrb r2, [r0]
	ands r1, r2
	movs r2, #0
	strb r1, [r0]
	ldr r0, _08010B40 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x80
	strb r2, [r0]
	bx lr
	.align 2, 0
_08010B40: .4byte gUnknown_030012D8

	thumb_func_start sub_8010B44
sub_8010B44: @ 0x08010B44
	adds r0, #0x58
	strb r1, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8010B4C
sub_8010B4C: @ 0x08010B4C
	adds r0, #0x51
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8010B54
sub_8010B54: @ 0x08010B54
	adds r0, #0x50
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8010B5C
sub_8010B5C: @ 0x08010B5C
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	str r1, [r0, #0x48]
	bx lr

	thumb_func_start sub_8010B64
sub_8010B64: @ 0x08010B64
	str r1, [r0, #0x54]
	bx lr

	thumb_func_start sub_8010B68
sub_8010B68: @ 0x08010B68
	ldr r0, [r0, #0x54]
	bx lr

	thumb_func_start sub_8010B6C
sub_8010B6C: @ 0x08010B6C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x68
	adds r7, r0, #0
	ldr r1, [r7]
	cmp r1, #0
	bne _08010B82
	b _08010D42
_08010B82:
	ldr r0, _08010C94 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r2, [r0]
	str r2, [sp, #0x24]
	ldr r0, [r0, #4]
	str r0, [sp, #0x28]
	movs r3, #0
	str r3, [sp, #0x1c]
	ldr r0, [r7, #8]
	ldr r4, [r0]
	ldr r0, [r0, #4]
	mov sb, r0
	subs r4, r4, r2
	str r4, [sp, #0x20]
	cmp r4, #0
	bge _08010BA6
	rsbs r4, r4, #0
	str r4, [sp, #0x20]
_08010BA6:
	mov r5, sb
	ldr r0, [sp, #0x28]
	subs r5, r5, r0
	mov sb, r5
	cmp r5, #0
	bge _08010BB6
	rsbs r5, r5, #0
	mov sb, r5
_08010BB6:
	movs r2, #0
	mov ip, r2
	movs r3, #1
	str r3, [sp, #0x2c]
	adds r4, r7, #0
	adds r4, #8
	str r4, [sp, #0x34]
	adds r5, r7, #0
	adds r5, #0x14
	str r5, [sp, #0x40]
	adds r0, r7, #0
	adds r0, #0x18
	str r0, [sp, #0x44]
	adds r2, r7, #0
	adds r2, #0x1c
	str r2, [sp, #0x48]
	adds r3, r7, #0
	adds r3, #0x20
	str r3, [sp, #0x4c]
	adds r4, #0x1c
	str r4, [sp, #0x50]
	mov r5, sp
	adds r5, #0x10
	str r5, [sp, #0x30]
	mov r0, sp
	adds r0, #0x14
	str r0, [sp, #0x38]
	mov r2, sp
	adds r2, #0x18
	str r2, [sp, #0x3c]
	ldr r3, [sp, #0x2c]
	cmp r3, r1
	bge _08010CDE
	adds r4, #0x24
	str r4, [sp, #0x54]
	movs r5, #0x30
	adds r5, r5, r7
	mov r8, r5
	adds r0, r7, #0
	adds r0, #0x44
	str r0, [sp, #0x58]
	adds r1, r7, #0
	adds r1, #0x40
	str r1, [sp, #0x5c]
	adds r2, r7, #0
	adds r2, #0x3c
	str r2, [sp, #0x60]
	movs r3, #0x38
	adds r3, r3, r7
	mov sl, r3
	subs r4, #0x1c
	str r4, [sp, #0x64]
_08010C1E:
	ldr r5, [sp, #0x64]
	ldr r6, [r5]
	ldr r2, [r6]
	ldr r1, [r6, #4]
	ldr r0, [sp, #0x24]
	subs r2, r2, r0
	cmp r2, #0
	bge _08010C30
	rsbs r2, r2, #0
_08010C30:
	ldr r3, [sp, #0x28]
	subs r1, r1, r3
	cmp r1, #0
	bge _08010C3A
	rsbs r1, r1, #0
_08010C3A:
	mov r4, sb
	subs r0, r1, r4
	cmp r0, #0
	bge _08010C44
	rsbs r0, r0, #0
_08010C44:
	cmp r0, #8
	bgt _08010C50
	mov r5, sl
	ldr r0, [r5]
	cmp r0, #4
	bne _08010C98
_08010C50:
	mov r0, sl
	ldr r1, [r0]
	ldr r3, [sp, #0x60]
	ldr r2, [r3]
	ldr r4, [sp, #0x5c]
	ldr r3, [r4]
	ldr r5, [sp, #0x58]
	ldr r0, [r5]
	str r0, [sp]
	mov r0, r8
	ldr r4, [r0]
	ldr r5, [r0, #4]
	str r4, [sp, #4]
	str r5, [sp, #8]
	ldr r4, [sp, #0x54]
	ldr r0, [r4]
	str r0, [sp, #0xc]
	mov r5, r8
	ldrb r0, [r5, #0x1c]
	ldr r4, [sp, #0x30]
	strb r0, [r4]
	ldrb r0, [r5, #0x1d]
	ldr r5, [sp, #0x38]
	strb r0, [r5]
	movs r0, #0
	ldr r4, [sp, #0x3c]
	strb r0, [r4]
	adds r0, r6, #0
	bl sub_800E08C
	movs r5, #1
	mov ip, r5
	b _08010CAE
	.align 2, 0
_08010C94: .4byte gUnknown_030012D8
_08010C98:
	cmp r1, sb
	blt _08010CA6
	cmp r1, sb
	bne _08010CAE
	ldr r0, [sp, #0x20]
	cmp r2, r0
	bge _08010CAE
_08010CA6:
	str r2, [sp, #0x20]
	mov sb, r1
	ldr r1, [sp, #0x2c]
	str r1, [sp, #0x1c]
_08010CAE:
	ldr r2, [sp, #0x54]
	adds r2, #0x24
	str r2, [sp, #0x54]
	movs r3, #0x24
	add r8, r3
	ldr r4, [sp, #0x58]
	adds r4, #0x24
	str r4, [sp, #0x58]
	ldr r5, [sp, #0x5c]
	adds r5, #0x24
	str r5, [sp, #0x5c]
	ldr r0, [sp, #0x60]
	adds r0, #0x24
	str r0, [sp, #0x60]
	add sl, r3
	ldr r1, [sp, #0x64]
	adds r1, #0x24
	str r1, [sp, #0x64]
	ldr r2, [sp, #0x2c]
	adds r2, #1
	str r2, [sp, #0x2c]
	ldr r0, [r7]
	cmp r2, r0
	blt _08010C1E
_08010CDE:
	ldr r3, [sp, #0x1c]
	lsls r6, r3, #3
	adds r6, r6, r3
	lsls r6, r6, #2
	ldr r4, [sp, #0x34]
	adds r0, r4, r6
	ldr r0, [r0]
	ldr r5, [sp, #0x40]
	adds r1, r5, r6
	ldr r1, [r1]
	ldr r3, [sp, #0x44]
	adds r2, r3, r6
	ldr r2, [r2]
	ldr r4, [sp, #0x48]
	adds r3, r4, r6
	ldr r3, [r3]
	ldr r5, [sp, #0x4c]
	adds r4, r5, r6
	ldr r4, [r4]
	str r4, [sp]
	adds r4, r6, r7
	mov r8, r4
	mov r5, r8
	ldr r4, [r5, #0xc]
	ldr r5, [r5, #0x10]
	str r4, [sp, #4]
	str r5, [sp, #8]
	ldr r4, [sp, #0x50]
	adds r6, r4, r6
	ldr r4, [r6]
	str r4, [sp, #0xc]
	mov r4, r8
	adds r4, #0x28
	ldrb r4, [r4]
	ldr r5, [sp, #0x30]
	strb r4, [r5]
	movs r4, #0x29
	add r8, r4
	mov r5, r8
	ldrb r4, [r5]
	ldr r5, [sp, #0x38]
	strb r4, [r5]
	mov r5, ip
	ldr r4, [sp, #0x3c]
	strb r5, [r4]
	bl sub_800E08C
	movs r0, #0
	str r0, [r7]
	strb r0, [r7, #4]
_08010D42:
	add sp, #0x68
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8010D54
sub_8010D54: @ 0x08010D54
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov ip, r0
	mov sb, r1
	ldr r7, [sp, #0x1c]
	ldr r6, [sp, #0x2c]
	add r0, sp, #0x30
	add r4, sp, #0x34
	ldrb r0, [r0]
	mov r8, r0
	ldrb r5, [r4]
	mov r1, ip
	ldr r0, [r1]
	lsls r4, r0, #3
	adds r4, r4, r0
	lsls r4, r4, #2
	mov r0, ip
	adds r0, #8
	adds r0, r0, r4
	mov r1, sb
	str r1, [r0]
	mov r0, ip
	adds r0, #0x14
	adds r0, r0, r4
	str r2, [r0]
	mov r2, ip
	ldr r0, [r2]
	lsls r1, r0, #3
	adds r1, r1, r0
	lsls r1, r1, #2
	mov r0, ip
	adds r0, #0x18
	adds r0, r0, r1
	str r3, [r0]
	ldr r1, [r2]
	lsls r0, r1, #3
	adds r0, r0, r1
	lsls r0, r0, #2
	add r0, ip
	adds r0, #0x29
	strb r5, [r0]
	ldr r0, [r2]
	lsls r1, r0, #3
	adds r1, r1, r0
	lsls r1, r1, #2
	mov r0, ip
	adds r0, #0x24
	adds r0, r0, r1
	str r6, [r0]
	ldr r0, [r2]
	lsls r1, r0, #3
	adds r1, r1, r0
	lsls r1, r1, #2
	mov r0, ip
	adds r0, #0x1c
	adds r0, r0, r1
	str r7, [r0]
	ldr r1, [r2]
	lsls r0, r1, #3
	adds r0, r0, r1
	lsls r0, r0, #2
	add r0, ip
	adds r0, #0x28
	mov r1, r8
	strb r1, [r0]
	ldr r1, [r2]
	lsls r0, r1, #3
	adds r0, r0, r1
	lsls r0, r0, #2
	add r0, ip
	ldr r1, [sp, #0x24]
	ldr r2, [sp, #0x28]
	str r1, [r0, #0xc]
	str r2, [r0, #0x10]
	mov r2, ip
	ldr r0, [r2]
	lsls r1, r0, #3
	adds r1, r1, r0
	lsls r1, r1, #2
	mov r0, ip
	adds r0, #0x20
	adds r0, r0, r1
	ldr r1, [sp, #0x20]
	str r1, [r0]
	ldr r0, [r2]
	adds r0, #1
	str r0, [r2]
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8010E14
sub_8010E14: @ 0x08010E14
	push {lr}
	adds r2, r0, #0
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08010E26
	adds r0, r2, #0
	bl sub_8026ED0
_08010E26:
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8010E2C
sub_8010E2C: @ 0x08010E2C
	movs r1, #0
	str r1, [r0]
	strb r1, [r0, #4]
	bx lr

	thumb_func_start sub_8010E34
sub_8010E34: @ 0x08010E34
	push {r4, r5, lr}
	sub sp, #0x20
	adds r5, r0, #0
	adds r0, #0x4a
	ldrb r0, [r0]
	cmp r0, #0
	beq _08010E58
	adds r0, r5, #0
	adds r0, #0x4b
	ldrb r0, [r0]
	cmp r0, #0x16
	bhi _08010E58
	ldr r0, _08010EA8 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #3
	bne _08010EA0
_08010E58:
	ldrb r0, [r5, #0xc]
	lsls r1, r0, #0x18
	lsrs r0, r1, #0x1b
	movs r2, #1
	ands r0, r2
	cmp r0, #0
	bne _08010EA0
	lsrs r0, r1, #0x1a
	ands r0, r2
	cmp r0, #0
	beq _08010EA0
	mov r0, sp
	adds r1, r5, #0
	bl sub_8007B98
	ldr r0, _08010EA8 @ =gUnknown_030012D8
	ldr r1, [r0]
	add r4, sp, #0x10
	adds r0, r4, #0
	bl sub_8007B98
	adds r0, r4, #0
	mov r1, sp
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08010EA0
	movs r0, #8
	ldrb r1, [r5, #0xc]
	orrs r0, r1
	strb r0, [r5, #0xc]
	adds r0, r5, #0
	movs r1, #0
	bl sub_8010EAC
_08010EA0:
	add sp, #0x20
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08010EA8: .4byte gUnknown_030012D8

	thumb_func_start sub_8010EAC
sub_8010EAC: @ 0x08010EAC
	push {r4, r5, r6, lr}
	sub sp, #0xc
	adds r5, r0, #0
	lsls r4, r1, #0x18
	lsrs r4, r4, #0x18
	ldr r0, _08010EF4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #7
	bl PlaySfx
	movs r0, #0xa0
	strh r0, [r5, #0x3c]
	cmp r4, #0
	beq _08010F1C
	bl rand
	lsls r0, r0, #0x10
	lsrs r2, r0, #0x10
	movs r1, #1
	adds r0, r2, #0
	ands r0, r1
	adds r1, r5, #0
	adds r1, #0x49
	strb r0, [r1]
	cmp r0, #0
	beq _08010F02
	movs r0, #2
	ands r0, r2
	cmp r0, #0
	beq _08010EF8
	movs r0, #0x3f
	ands r0, r2
	adds r0, #5
	b _08010F08
	.align 2, 0
_08010EF4: .4byte gUnknown_030012BC
_08010EF8:
	movs r1, #0x3f
	ands r1, r2
	movs r0, #0xeb
	subs r0, r0, r1
	b _08010F08
_08010F02:
	movs r0, #0x7f
	ands r0, r2
	adds r0, #0x24
_08010F08:
	lsls r4, r0, #8
	movs r0, #0x1f
	ands r0, r2
	adds r0, #0x10
	lsls r6, r0, #8
	adds r1, r5, #0
	adds r1, #0x48
	movs r0, #2
	strb r0, [r1]
	b _08010F34
_08010F1C:
	movs r4, #0xb4
	lsls r4, r4, #8
	movs r6, #0xc0
	lsls r6, r6, #4
	adds r1, r5, #0
	adds r1, #0x48
	movs r0, #1
	strb r0, [r1]
	ldr r0, _08010F88 @ =gUnknown_03001318
	ldr r0, [r0]
	bl sub_80284A4
_08010F34:
	movs r0, #0x10
	ldrb r1, [r5, #0xc]
	orrs r0, r1
	strb r0, [r5, #0xc]
	movs r0, #1
	adds r1, r5, #0
	adds r1, #0x25
	strb r0, [r1]
	ldr r1, [r5]
	asrs r1, r1, #8
	ldr r2, [r5, #4]
	asrs r2, r2, #8
	add r0, sp, #8
	str r0, [sp]
	adds r0, r5, #0
	add r3, sp, #4
	bl sub_8007174
	ldr r0, [sp, #4]
	lsls r0, r0, #8
	str r0, [r5]
	subs r0, r0, r4
	movs r4, #0xa0
	lsls r4, r4, #5
	adds r1, r4, #0
	bl sub_80008F0
	rsbs r0, r0, #0
	str r0, [r5, #0x40]
	ldr r0, [sp, #8]
	lsls r0, r0, #8
	str r0, [r5, #4]
	subs r0, r0, r6
	adds r1, r4, #0
	bl sub_80008F0
	rsbs r0, r0, #0
	str r0, [r5, #0x44]
	add sp, #0xc
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08010F88: .4byte gUnknown_03001318

	thumb_func_start sub_8010F8C
sub_8010F8C: @ 0x08010F8C
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r0, #0x48
	ldrb r5, [r0]
	cmp r5, #1
	bne _08011014
	ldr r1, [r4]
	ldr r3, [r4, #0x40]
	adds r0, r1, r3
	str r0, [r4]
	ldr r2, [r4, #4]
	ldr r0, [r4, #0x44]
	adds r0, r2, r0
	str r0, [r4, #4]
	adds r1, r1, r3
	asrs r1, r1, #8
	cmp r1, #0xb4
	ble _08010FB2
	b _080110C0
_08010FB2:
	asrs r0, r0, #8
	cmp r0, #0xc
	ble _08010FBA
	b _080110C0
_08010FBA:
	ldr r0, _08011004 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xe
	bl PlaySfx
	ldr r0, _08011008 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023464
	movs r0, #1
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _0801100C @ =0x0000FFFF
	ldrh r2, [r4, #8]
	cmp r2, r0
	beq _080110C0
	ldrh r3, [r4, #8]
	ldr r0, _08011010 @ =gUnknown_030012B4
	ldr r1, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r2, r0, #2
	movs r6, #0x84
	lsls r6, r6, #1
	adds r1, r1, r6
	adds r1, r1, r2
	lsls r0, r0, #5
	subs r0, r3, r0
	lsls r5, r0
	ldr r0, [r1]
	orrs r0, r5
	str r0, [r1]
	b _080110C0
	.align 2, 0
_08011004: .4byte gUnknown_030012BC
_08011008: .4byte gUnknown_030012C0
_0801100C: .4byte 0x0000FFFF
_08011010: .4byte gUnknown_030012B4
_08011014:
	cmp r5, #2
	bne _08011094
	ldr r1, [r4]
	ldr r0, [r4, #0x40]
	adds r1, r1, r0
	str r1, [r4]
	ldr r1, [r4, #4]
	ldr r0, [r4, #0x44]
	adds r1, r1, r0
	str r1, [r4, #4]
	movs r1, #0
	adds r0, r4, #0
	adds r0, #0x49
	ldrb r0, [r0]
	cmp r0, #0
	bne _08011040
	ldrh r0, [r4, #0x3c]
	subs r0, #4
	strh r0, [r4, #0x3c]
	cmp r0, #0x3f
	bgt _08011052
	b _08011056
_08011040:
	ldrh r0, [r4, #0x3c]
	adds r0, #0xc
	strh r0, [r4, #0x3c]
	movs r0, #0xd8
	lsls r0, r0, #1
	ldrh r2, [r4, #0x3c]
	cmp r2, r0
	ble _08011052
	movs r1, #1
_08011052:
	cmp r1, #0
	beq _080110C0
_08011056:
	movs r0, #1
	ldrb r5, [r4, #0xc]
	orrs r0, r5
	strb r0, [r4, #0xc]
	ldr r0, _0801108C @ =0x0000FFFF
	ldrh r6, [r4, #8]
	cmp r6, r0
	beq _080110C0
	ldrh r3, [r4, #8]
	ldr r0, _08011090 @ =gUnknown_030012B4
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
	b _080110C0
	.align 2, 0
_0801108C: .4byte 0x0000FFFF
_08011090: .4byte gUnknown_030012B4
_08011094:
	adds r2, r4, #0
	adds r2, #0x4a
	ldrb r0, [r2]
	cmp r0, #0
	bne _080110AA
	adds r1, r4, #0
	adds r1, #0x49
	ldrb r0, [r1]
	adds r0, #1
	strb r0, [r1]
	b _080110C0
_080110AA:
	adds r1, r4, #0
	adds r1, #0x4b
	ldrb r0, [r1]
	adds r0, #1
	strb r0, [r1]
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #0x1f
	bls _080110C0
	movs r0, #0
	strb r0, [r2]
_080110C0:
	adds r0, r4, #0
	adds r0, #0x48
	ldrb r0, [r0]
	cmp r0, #0
	bne _08011106
	adds r0, r4, #0
	adds r0, #0x4a
	ldrb r0, [r0]
	cmp r0, #0
	bne _08011100
	ldr r1, _080110FC @ =gStaticData_0816A820
	adds r2, r4, #0
	adds r2, #0x49
	movs r0, #0x7f
	ldrb r2, [r2]
	ands r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	movs r6, #0
	ldrsh r2, [r0, r6]
	movs r1, #0xa0
	lsls r1, r1, #2
	adds r0, r2, #0
	bl sub_80008FC
	adds r2, r0, #0
	ldr r0, [r4, #0x50]
	adds r0, r0, r2
	str r0, [r4, #4]
	b _08011106
	.align 2, 0
_080110FC: .4byte gStaticData_0816A820
_08011100:
	adds r0, r4, #0
	bl sub_8011248
_08011106:
	adds r0, r4, #0
	bl sub_8008364
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8011114
sub_8011114: @ 0x08011114
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	adds r4, r1, #0
	adds r5, r2, #0
	lsls r6, r6, #0x10
	lsrs r6, r6, #0x10
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	movs r0, #0x54
	bl sub_8026EDC
	adds r7, r0, #0
	bl sub_80084A4
	ldr r0, _080111B0 @ =gStaticData_087E40DC
	str r0, [r7, #0x18]
	adds r0, r7, #0
	bl sub_8011308
	movs r0, #0
	mov r8, r0
	strh r6, [r7, #8]
	lsls r4, r4, #8
	str r4, [r7]
	lsls r5, r5, #8
	str r5, [r7, #4]
	ldr r0, [r7]
	ldr r1, [r7, #4]
	str r0, [r7, #0x4c]
	str r1, [r7, #0x50]
	ldr r0, _080111B4 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r7, #0
	bl sub_8008E94
	movs r3, #0
	ldr r0, [r7, #0x20]
	adds r2, r7, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r4, [r2]
	lsls r0, r4, #3
	subs r0, r0, r4
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _0801117E
	subs r3, r0, #1
_0801117E:
	str r3, [r7, #0x30]
	adds r0, r7, #0
	adds r0, #0x28
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r2, [r0]
	ands r1, r2
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	strb r1, [r0]
	adds r0, #0x21
	mov r4, r8
	strb r4, [r0]
	adds r0, #1
	strb r4, [r0]
	adds r0, #1
	strb r4, [r0]
	adds r0, r7, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080111B0: .4byte gStaticData_087E40DC
_080111B4: .4byte gUnknown_030012EC

	thumb_func_start sub_80111B8
sub_80111B8: @ 0x080111B8
	push {r4, r5, lr}
	sub sp, #0xc
	adds r5, r0, #0
	ldr r0, _08011238 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #7
	bl PlaySfx
	adds r0, r5, #0
	adds r0, #0x48
	movs r2, #1
	strb r2, [r0]
	adds r0, #2
	ldrb r0, [r0]
	lsls r1, r0, #8
	ldr r0, [r5]
	subs r0, r0, r1
	str r0, [r5]
	adds r0, r5, #0
	adds r0, #0x25
	strb r2, [r0]
	ldr r1, [r5]
	asrs r1, r1, #8
	ldr r2, [r5, #4]
	asrs r2, r2, #8
	add r0, sp, #8
	str r0, [sp]
	adds r0, r5, #0
	add r3, sp, #4
	bl sub_8007174
	ldr r0, [sp, #4]
	lsls r0, r0, #8
	str r0, [r5]
	ldr r1, _0801123C @ =0xFFFF4C00
	adds r0, r0, r1
	movs r4, #0xa0
	lsls r4, r4, #5
	adds r1, r4, #0
	bl sub_80008F0
	rsbs r0, r0, #0
	str r0, [r5, #0x40]
	ldr r0, [sp, #8]
	lsls r0, r0, #8
	str r0, [r5, #4]
	ldr r1, _08011240 @ =0xFFFFF400
	adds r0, r0, r1
	adds r1, r4, #0
	bl sub_80008F0
	rsbs r0, r0, #0
	str r0, [r5, #0x44]
	ldr r0, _08011244 @ =gUnknown_03001318
	ldr r0, [r0]
	bl sub_80284A4
	add sp, #0xc
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08011238: .4byte gUnknown_030012BC
_0801123C: .4byte 0xFFFF4C00
_08011240: .4byte 0xFFFFF400
_08011244: .4byte gUnknown_03001318

	thumb_func_start sub_8011248
sub_8011248: @ 0x08011248
	push {r4, r5, r6, lr}
	sub sp, #0xc
	adds r5, r0, #0
	mov r1, sp
	ldr r0, _080112A4 @ =gStaticData_0816BF08
	ldm r0!, {r2, r3, r4}
	stm r1!, {r2, r3, r4}
	ldr r4, _080112A8 @ =gStaticData_0816A820
	adds r6, r5, #0
	adds r6, #0x4b
	ldrb r1, [r6]
	lsls r0, r1, #3
	adds r0, r0, r4
	movs r3, #0
	ldrsh r2, [r0, r3]
	movs r1, #0x80
	lsls r1, r1, #4
	adds r0, r2, #0
	bl sub_80008FC
	ldr r1, [r5, #0x50]
	subs r1, r1, r0
	str r1, [r5, #4]
	ldrb r6, [r6]
	lsls r0, r6, #2
	adds r0, r0, r4
	movs r4, #0
	ldrsh r2, [r0, r4]
	adds r4, r5, #0
	adds r4, #0x4a
	ldrb r0, [r4]
	subs r0, #1
	lsls r0, r0, #2
	add r0, sp
	ldr r1, [r0]
	adds r0, r2, #0
	bl sub_80008FC
	adds r2, r0, #0
	ldrb r0, [r4]
	cmp r0, #1
	bne _080112AC
	ldr r0, [r5, #0x4c]
	subs r0, r0, r2
	b _080112B8
	.align 2, 0
_080112A4: .4byte gStaticData_0816BF08
_080112A8: .4byte gStaticData_0816A820
_080112AC:
	cmp r0, #2
	bne _080112B6
	ldr r0, [r5, #0x4c]
	adds r0, r0, r2
	b _080112B8
_080112B6:
	ldr r0, [r5, #0x4c]
_080112B8:
	str r0, [r5]
	add sp, #0xc
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80112C4
sub_80112C4: @ 0x080112C4
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, _080112EC @ =gUnknown_030012CC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8007A84
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _080112E6
	movs r0, #9
	rsbs r0, r0, #0
	ldrb r1, [r4, #0xc]
	ands r0, r1
	strb r0, [r4, #0xc]
_080112E6:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080112EC: .4byte gUnknown_030012CC

	thumb_func_start sub_80112F0
sub_80112F0: @ 0x080112F0
	movs r0, #2
	bx lr

	thumb_func_start sub_80112F4
sub_80112F4: @ 0x080112F4
	push {lr}
	ldr r2, _08011304 @ =gStaticData_087E40DC
	str r2, [r0, #0x18]
	bl sub_8008484
	pop {r0}
	bx r0
	.align 2, 0
_08011304: .4byte gStaticData_087E40DC

	thumb_func_start sub_8011308
sub_8011308: @ 0x08011308
	adds r0, #0x48
	movs r1, #0
	strb r1, [r0]
	bx lr

	thumb_func_start sub_8011310
sub_8011310: @ 0x08011310
	push {r4, lr}
	adds r4, r0, #0
	bl sub_80084A4
	ldr r0, _0801132C @ =gStaticData_087E40DC
	str r0, [r4, #0x18]
	adds r0, r4, #0
	bl sub_8011308
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801132C: .4byte gStaticData_087E40DC

	thumb_func_start sub_8011330
sub_8011330: @ 0x08011330
	push {lr}
	adds r2, r0, #0
	adds r0, #0x48
	ldrb r0, [r0]
	cmp r0, #0
	bne _08011358
	ldr r0, _08011360 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldrb r0, [r0, #0xc]
	lsrs r0, r0, #7
	cmp r0, #0
	beq _08011358
	ldr r1, [r2, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
_08011358:
	movs r0, #0
	pop {r1}
	bx r1
	.align 2, 0
_08011360: .4byte gUnknown_030012D8

	thumb_func_start sub_8011364
sub_8011364: @ 0x08011364
	lsls r1, r1, #8
	str r1, [r0]
	lsls r2, r2, #8
	str r2, [r0, #4]
	ldr r1, [r0]
	ldr r2, [r0, #4]
	str r1, [r0, #0x4c]
	str r2, [r0, #0x50]
	bx lr
	.align 2, 0

	thumb_func_start sub_8011378
sub_8011378: @ 0x08011378
	adds r3, r0, #0
	adds r3, #0x4a
	movs r2, #0
	strb r1, [r3]
	adds r0, #0x4b
	strb r2, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8011388
sub_8011388: @ 0x08011388
	adds r0, #0x49
	strb r1, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8011390
sub_8011390: @ 0x08011390
	push {r4, r5, lr}
	sub sp, #0x20
	adds r5, r0, #0
	adds r0, #0x4a
	ldrb r0, [r0]
	cmp r0, #0
	beq _080113B4
	adds r0, r5, #0
	adds r0, #0x4b
	ldrb r0, [r0]
	cmp r0, #0x16
	bhi _080113B4
	ldr r0, _08011410 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #3
	bne _0801143E
_080113B4:
	ldrb r0, [r5, #0xc]
	lsls r1, r0, #0x18
	lsrs r0, r1, #0x1b
	movs r2, #1
	ands r0, r2
	cmp r0, #0
	bne _0801143E
	lsrs r0, r1, #0x1a
	ands r0, r2
	cmp r0, #0
	beq _0801143E
	mov r0, sp
	adds r1, r5, #0
	bl sub_8007B98
	ldr r0, _08011410 @ =gUnknown_030012D8
	ldr r1, [r0]
	ldrb r0, [r1, #0xa]
	cmp r0, #0x13
	bne _08011418
	add r4, sp, #0x10
	adds r0, r4, #0
	bl sub_8007C30
	adds r0, r4, #0
	mov r1, sp
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801143E
	movs r0, #8
	ldrb r1, [r5, #0xc]
	orrs r0, r1
	strb r0, [r5, #0xc]
	adds r0, r5, #0
	movs r1, #1
	bl sub_8011448
	ldr r0, _08011414 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #6
	movs r2, #0x80
	bl PlaySfx
	b _0801143E
	.align 2, 0
_08011410: .4byte gUnknown_030012D8
_08011414: .4byte gUnknown_030012BC
_08011418:
	add r4, sp, #0x10
	adds r0, r4, #0
	bl sub_8007B98
	adds r0, r4, #0
	mov r1, sp
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801143E
	movs r0, #8
	ldrb r1, [r5, #0xc]
	orrs r0, r1
	strb r0, [r5, #0xc]
	adds r0, r5, #0
	movs r1, #0
	bl sub_8011448
_0801143E:
	add sp, #0x20
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8011448
sub_8011448: @ 0x08011448
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	adds r5, r0, #0
	lsls r4, r1, #0x18
	lsrs r4, r4, #0x18
	ldr r0, _0801148C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #8
	bl PlaySfx
	cmp r4, #0
	beq _080114B4
	bl rand
	lsls r0, r0, #0x10
	lsrs r2, r0, #0x10
	movs r1, #1
	adds r0, r2, #0
	ands r0, r1
	adds r1, r5, #0
	adds r1, #0x49
	strb r0, [r1]
	cmp r0, #0
	beq _0801149A
	movs r0, #2
	ands r0, r2
	cmp r0, #0
	beq _08011490
	movs r0, #0x3f
	ands r0, r2
	adds r0, #5
	b _080114A0
	.align 2, 0
_0801148C: .4byte gUnknown_030012BC
_08011490:
	movs r1, #0x3f
	ands r1, r2
	movs r0, #0xeb
	subs r0, r0, r1
	b _080114A0
_0801149A:
	movs r0, #0x7f
	ands r0, r2
	adds r0, #0x24
_080114A0:
	lsls r4, r0, #8
	movs r0, #0x1f
	ands r0, r2
	adds r0, #0x10
	lsls r6, r0, #8
	adds r1, r5, #0
	adds r1, #0x48
	movs r0, #2
	strb r0, [r1]
	b _080114CA
_080114B4:
	movs r6, #0x80
	lsls r6, r6, #5
	adds r4, r6, #0
	adds r1, r5, #0
	adds r1, #0x48
	movs r0, #1
	strb r0, [r1]
	ldr r0, _08011544 @ =gUnknown_03001318
	ldr r0, [r0]
	bl sub_80284D4
_080114CA:
	movs r0, #0xa0
	strh r0, [r5, #0x3c]
	movs r3, #0
	ldr r0, [r5, #0x20]
	adds r2, r5, #0
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
	blt _080114EC
	subs r3, r0, #1
_080114EC:
	str r3, [r5, #0x30]
	movs r0, #1
	adds r1, r5, #0
	adds r1, #0x25
	strb r0, [r1]
	movs r0, #0x10
	ldrb r1, [r5, #0xc]
	orrs r0, r1
	strb r0, [r5, #0xc]
	ldr r1, [r5]
	asrs r1, r1, #8
	ldr r2, [r5, #4]
	asrs r2, r2, #8
	add r0, sp, #8
	str r0, [sp]
	adds r0, r5, #0
	add r3, sp, #4
	bl sub_8007174
	ldr r0, [sp, #4]
	lsls r0, r0, #8
	str r0, [r5]
	subs r0, r0, r4
	movs r4, #0xa0
	lsls r4, r4, #5
	adds r1, r4, #0
	bl sub_80008F0
	rsbs r0, r0, #0
	str r0, [r5, #0x40]
	ldr r0, [sp, #8]
	lsls r0, r0, #8
	str r0, [r5, #4]
	subs r0, r0, r6
	adds r1, r4, #0
	bl sub_80008F0
	rsbs r0, r0, #0
	str r0, [r5, #0x44]
	add sp, #0xc
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08011544: .4byte gUnknown_03001318

	thumb_func_start sub_8011548
sub_8011548: @ 0x08011548
	push {r4, r5, lr}
	sub sp, #8
	adds r4, r0, #0
	adds r0, #0x48
	ldrb r0, [r0]
	cmp r0, #1
	bne _080115C8
	ldr r1, [r4]
	ldr r0, [r4, #0x40]
	adds r1, r1, r0
	str r1, [r4]
	ldr r1, [r4, #4]
	ldr r0, [r4, #0x44]
	adds r1, r1, r0
	str r1, [r4, #4]
	ldrh r0, [r4, #0x3c]
	cmp r0, #0
	beq _0801157E
	adds r0, #4
	strh r0, [r4, #0x3c]
	movs r0, #0x80
	lsls r0, r0, #1
	ldrh r1, [r4, #0x3c]
	cmp r1, r0
	ble _0801157E
	movs r0, #0
	strh r0, [r4, #0x3c]
_0801157E:
	ldr r0, [r4]
	asrs r0, r0, #8
	cmp r0, #0x10
	ble _08011588
	b _080116C0
_08011588:
	ldr r0, [r4, #4]
	asrs r0, r0, #8
	cmp r0, #0x10
	ble _08011592
	b _080116C0
_08011592:
	ldr r0, _080115BC @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xe
	bl PlaySfx
	ldr r0, _080115C0 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023430
	movs r0, #1
	ldrb r2, [r4, #0xc]
	orrs r0, r2
	strb r0, [r4, #0xc]
	ldr r0, _080115C4 @ =0x0000FFFF
	ldrh r5, [r4, #8]
	cmp r5, r0
	bne _080115BA
	b _080116C0
_080115BA:
	b _08011664
	.align 2, 0
_080115BC: .4byte gUnknown_030012BC
_080115C0: .4byte gUnknown_030012C0
_080115C4: .4byte 0x0000FFFF
_080115C8:
	cmp r0, #2
	bne _08011610
	ldr r1, [r4]
	ldr r0, [r4, #0x40]
	adds r1, r1, r0
	str r1, [r4]
	ldr r1, [r4, #4]
	ldr r0, [r4, #0x44]
	adds r1, r1, r0
	str r1, [r4, #4]
	movs r1, #0
	adds r0, r4, #0
	adds r0, #0x49
	ldrb r0, [r0]
	cmp r0, #0
	bne _080115F4
	ldrh r0, [r4, #0x3c]
	subs r0, #4
	strh r0, [r4, #0x3c]
	cmp r0, #0x3f
	bgt _08011606
	b _0801160A
_080115F4:
	ldrh r0, [r4, #0x3c]
	adds r0, #0xc
	strh r0, [r4, #0x3c]
	movs r0, #0xd8
	lsls r0, r0, #1
	ldrh r2, [r4, #0x3c]
	cmp r2, r0
	ble _08011606
	movs r1, #1
_08011606:
	cmp r1, #0
	beq _080116C0
_0801160A:
	movs r0, #1
	ldrb r5, [r4, #0xc]
	b _08011658
_08011610:
	cmp r0, #3
	bne _08011694
	adds r1, r4, #0
	adds r1, #0x49
	ldrb r0, [r1]
	adds r0, #1
	movs r3, #0
	strb r0, [r1]
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #0xa
	bls _080116C0
	strb r3, [r1]
	ldr r1, [r4]
	asrs r1, r1, #8
	ldr r2, [r4, #4]
	asrs r2, r2, #8
	ldr r0, _08011688 @ =gUnknown_030012E4
	ldr r0, [r0]
	str r3, [sp]
	add r3, sp, #4
	movs r5, #1
	strb r5, [r3]
	movs r3, #0
	bl sub_8025CA4
	adds r1, r4, #0
	adds r1, #0x4b
	ldrb r0, [r1]
	adds r0, #1
	strb r0, [r1]
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #9
	bls _080116C0
	ldrb r0, [r4, #0xc]
_08011658:
	orrs r0, r5
	strb r0, [r4, #0xc]
	ldr r0, _0801168C @ =0x0000FFFF
	ldrh r1, [r4, #8]
	cmp r1, r0
	beq _080116C0
_08011664:
	ldrh r3, [r4, #8]
	ldr r0, _08011690 @ =gUnknown_030012B4
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
	b _080116C0
	.align 2, 0
_08011688: .4byte gUnknown_030012E4
_0801168C: .4byte 0x0000FFFF
_08011690: .4byte gUnknown_030012B4
_08011694:
	adds r2, r4, #0
	adds r2, #0x4a
	ldrb r0, [r2]
	cmp r0, #0
	bne _080116AA
	adds r1, r4, #0
	adds r1, #0x49
	ldrb r0, [r1]
	adds r0, #1
	strb r0, [r1]
	b _080116C0
_080116AA:
	adds r1, r4, #0
	adds r1, #0x4b
	ldrb r0, [r1]
	adds r0, #1
	strb r0, [r1]
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #0x1f
	bls _080116C0
	movs r0, #0
	strb r0, [r2]
_080116C0:
	adds r0, r4, #0
	adds r0, #0x48
	ldrb r0, [r0]
	cmp r0, #0
	bne _08011708
	adds r0, r4, #0
	adds r0, #0x4a
	ldrb r0, [r0]
	cmp r0, #0
	bne _08011700
	ldr r1, _080116FC @ =gStaticData_0816A820
	adds r2, r4, #0
	adds r2, #0x49
	movs r0, #0x7f
	ldrb r2, [r2]
	ands r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	movs r1, #0
	ldrsh r2, [r0, r1]
	movs r1, #0xa0
	lsls r1, r1, #2
	adds r0, r2, #0
	bl sub_80008FC
	adds r2, r0, #0
	ldr r0, [r4, #0x50]
	adds r0, r0, r2
	str r0, [r4, #4]
	b _08011720
	.align 2, 0
_080116FC: .4byte gStaticData_0816A820
_08011700:
	adds r0, r4, #0
	bl sub_801192C
	b _08011720
_08011708:
	cmp r0, #3
	bne _08011720
	ldr r0, _08011730 @ =gUnknown_030012D8
	ldr r1, [r0]
	ldr r0, [r1]
	ldr r1, [r1, #4]
	ldr r2, _08011734 @ =0xFFFFFC00
	adds r0, r0, r2
	ldr r5, _08011738 @ =0xFFFFF200
	adds r1, r1, r5
	str r0, [r4]
	str r1, [r4, #4]
_08011720:
	adds r0, r4, #0
	bl sub_8008364
	add sp, #8
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08011730: .4byte gUnknown_030012D8
_08011734: .4byte 0xFFFFFC00
_08011738: .4byte 0xFFFFF200

	thumb_func_start sub_801173C
sub_801173C: @ 0x0801173C
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r0
	adds r5, r1, #0
	adds r6, r2, #0
	mov sb, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	mov r8, r0
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	lsls r6, r6, #0x10
	lsrs r6, r6, #0x10
	mov r1, sb
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	mov sb, r1
	movs r7, #0
	movs r0, #0x54
	bl sub_8026EDC
	adds r4, r0, #0
	bl sub_80084A4
	ldr r0, _080117A0 @ =gStaticData_087E414C
	str r0, [r4, #0x18]
	adds r0, r4, #0
	bl sub_80119EC
	mov r3, r8
	strh r3, [r4, #8]
	lsls r5, r5, #8
	str r5, [r4]
	lsls r6, r6, #8
	str r6, [r4, #4]
	ldr r0, [r4]
	ldr r1, [r4, #4]
	str r0, [r4, #0x4c]
	str r1, [r4, #0x50]
	ldr r0, _080117A4 @ =0x0000FFFF
	cmp sb, r0
	bne _080117AC
	ldr r0, _080117A8 @ =gUnknown_030012F4
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	b _080117B6
	.align 2, 0
_080117A0: .4byte gStaticData_087E414C
_080117A4: .4byte 0x0000FFFF
_080117A8: .4byte gUnknown_030012F4
_080117AC:
	ldr r0, _08011864 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
_080117B6:
	ldr r0, _08011868 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xd2
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	movs r0, #1
	adds r5, r4, #0
	adds r5, #0x2d
	movs r6, #0
	strb r0, [r5]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	movs r2, #0
	ldr r0, [r4, #0x20]
	ldr r1, [r0]
	ldrb r3, [r5]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r6, r0
	blt _080117FC
	subs r2, r0, #1
_080117FC:
	str r2, [r4, #0x30]
	adds r2, r4, #0
	adds r2, #0x28
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r1, [r2]
	ands r0, r1
	movs r1, #0x21
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r2]
	adds r1, r4, #0
	adds r1, #0x49
	movs r0, #0
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x4a
	strb r7, [r0]
	adds r0, #1
	strb r6, [r0]
	cmp r7, #0xff
	bne _0801182E
	adds r0, r4, #0
	bl sub_801191C
_0801182E:
	ldr r0, _0801186C @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r1, [r4, #0x20]
	ldr r1, [r1]
	ldrb r1, [r1, #0x14]
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
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
	adds r0, r4, #0
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08011864: .4byte gUnknown_030012EC
_08011868: .4byte gUnknown_030012D0
_0801186C: .4byte gUnknown_030012B8

	thumb_func_start sub_8011870
sub_8011870: @ 0x08011870
	push {r4, r5, r6, lr}
	sub sp, #0xc
	adds r5, r0, #0
	ldr r0, _08011910 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #8
	bl PlaySfx
	adds r0, r5, #0
	adds r0, #0x48
	movs r4, #1
	strb r4, [r0]
	adds r0, #2
	ldrb r0, [r0]
	lsls r1, r0, #8
	ldr r0, [r5]
	subs r0, r0, r1
	str r0, [r5]
	movs r0, #0xa0
	strh r0, [r5, #0x3c]
	movs r3, #0
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
	blt _080118B8
	subs r3, r0, #1
_080118B8:
	str r3, [r5, #0x30]
	adds r0, r5, #0
	adds r0, #0x25
	strb r4, [r0]
	ldr r1, [r5]
	asrs r1, r1, #8
	ldr r2, [r5, #4]
	asrs r2, r2, #8
	add r0, sp, #8
	str r0, [sp]
	adds r0, r5, #0
	add r3, sp, #4
	bl sub_8007174
	ldr r0, [sp, #4]
	lsls r0, r0, #8
	str r0, [r5]
	ldr r1, _08011914 @ =0xFFFFF000
	adds r0, r0, r1
	movs r4, #0xa0
	lsls r4, r4, #5
	adds r1, r4, #0
	bl sub_80008F0
	rsbs r0, r0, #0
	str r0, [r5, #0x40]
	ldr r0, [sp, #8]
	lsls r0, r0, #8
	str r0, [r5, #4]
	ldr r6, _08011914 @ =0xFFFFF000
	adds r0, r0, r6
	adds r1, r4, #0
	bl sub_80008F0
	rsbs r0, r0, #0
	str r0, [r5, #0x44]
	ldr r0, _08011918 @ =gUnknown_03001318
	ldr r0, [r0]
	bl sub_80284D4
	add sp, #0xc
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08011910: .4byte gUnknown_030012BC
_08011914: .4byte 0xFFFFF000
_08011918: .4byte gUnknown_03001318

	thumb_func_start sub_801191C
sub_801191C: @ 0x0801191C
	adds r2, r0, #0
	adds r2, #0x48
	movs r1, #3
	strb r1, [r2]
	adds r0, #0x49
	movs r1, #0xa
	strb r1, [r0]
	bx lr

	thumb_func_start sub_801192C
sub_801192C: @ 0x0801192C
	push {r4, r5, r6, lr}
	sub sp, #0xc
	adds r5, r0, #0
	mov r1, sp
	ldr r0, _08011988 @ =gStaticData_0816BF14
	ldm r0!, {r2, r3, r4}
	stm r1!, {r2, r3, r4}
	ldr r4, _0801198C @ =gStaticData_0816A820
	adds r6, r5, #0
	adds r6, #0x4b
	ldrb r1, [r6]
	lsls r0, r1, #3
	adds r0, r0, r4
	movs r3, #0
	ldrsh r2, [r0, r3]
	movs r1, #0xc0
	lsls r1, r1, #6
	adds r0, r2, #0
	bl sub_80008FC
	ldr r1, [r5, #0x50]
	subs r1, r1, r0
	str r1, [r5, #4]
	ldrb r6, [r6]
	lsls r0, r6, #2
	adds r0, r0, r4
	movs r4, #0
	ldrsh r2, [r0, r4]
	adds r4, r5, #0
	adds r4, #0x4a
	ldrb r0, [r4]
	subs r0, #1
	lsls r0, r0, #2
	add r0, sp
	ldr r1, [r0]
	adds r0, r2, #0
	bl sub_80008FC
	adds r2, r0, #0
	ldrb r0, [r4]
	cmp r0, #1
	bne _08011990
	ldr r0, [r5, #0x4c]
	subs r0, r0, r2
	b _0801199C
	.align 2, 0
_08011988: .4byte gStaticData_0816BF14
_0801198C: .4byte gStaticData_0816A820
_08011990:
	cmp r0, #2
	bne _0801199A
	ldr r0, [r5, #0x4c]
	adds r0, r0, r2
	b _0801199C
_0801199A:
	ldr r0, [r5, #0x4c]
_0801199C:
	str r0, [r5]
	add sp, #0xc
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80119A8
sub_80119A8: @ 0x080119A8
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, _080119D0 @ =gUnknown_030012CC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8007A84
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _080119CA
	movs r0, #9
	rsbs r0, r0, #0
	ldrb r1, [r4, #0xc]
	ands r0, r1
	strb r0, [r4, #0xc]
_080119CA:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080119D0: .4byte gUnknown_030012CC

	thumb_func_start sub_80119D4
sub_80119D4: @ 0x080119D4
	movs r0, #2
	bx lr

	thumb_func_start sub_80119D8
sub_80119D8: @ 0x080119D8
	push {lr}
	ldr r2, _080119E8 @ =gStaticData_087E414C
	str r2, [r0, #0x18]
	bl sub_8008484
	pop {r0}
	bx r0
	.align 2, 0
_080119E8: .4byte gStaticData_087E414C

	thumb_func_start sub_80119EC
sub_80119EC: @ 0x080119EC
	movs r1, #0x40
	ldrb r2, [r0, #0xc]
	orrs r1, r2
	strb r1, [r0, #0xc]
	adds r0, #0x48
	movs r1, #0
	strb r1, [r0]
	bx lr

	thumb_func_start sub_80119FC
sub_80119FC: @ 0x080119FC
	push {r4, lr}
	adds r4, r0, #0
	bl sub_80084A4
	ldr r0, _08011A18 @ =gStaticData_087E414C
	str r0, [r4, #0x18]
	adds r0, r4, #0
	bl sub_80119EC
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08011A18: .4byte gStaticData_087E414C

	thumb_func_start sub_8011A1C
sub_8011A1C: @ 0x08011A1C
	push {lr}
	adds r2, r0, #0
	adds r0, #0x48
	ldrb r0, [r0]
	cmp r0, #0
	bne _08011A44
	ldr r0, _08011A4C @ =gUnknown_030012D8
	ldr r0, [r0]
	ldrb r0, [r0, #0xc]
	lsrs r0, r0, #7
	cmp r0, #0
	beq _08011A44
	ldr r1, [r2, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
_08011A44:
	movs r0, #0
	pop {r1}
	bx r1
	.align 2, 0
_08011A4C: .4byte gUnknown_030012D8

	thumb_func_start sub_8011A50
sub_8011A50: @ 0x08011A50
	lsls r1, r1, #8
	str r1, [r0]
	lsls r2, r2, #8
	str r2, [r0, #4]
	ldr r1, [r0]
	ldr r2, [r0, #4]
	str r1, [r0, #0x4c]
	str r2, [r0, #0x50]
	bx lr
	.align 2, 0

	thumb_func_start sub_8011A64
sub_8011A64: @ 0x08011A64
	push {lr}
	adds r3, r0, #0
	adds r2, r3, #0
	adds r2, #0x4a
	movs r0, #0
	strb r1, [r2]
	adds r2, #1
	strb r0, [r2]
	cmp r1, #0xff
	bne _08011A7E
	adds r0, r3, #0
	bl sub_801191C
_08011A7E:
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8011A84
sub_8011A84: @ 0x08011A84
	adds r0, #0x49
	strb r1, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8011A8C
sub_8011A8C: @ 0x08011A8C
	push {r4, lr}
	adds r2, r0, #0
	ldr r0, _08011AF4 @ =gUnknown_030012D8
	ldr r3, [r0]
	ldr r0, [r3]
	asrs r1, r0, #8
	ldr r0, [r2]
	asrs r0, r0, #8
	subs r1, r1, r0
	cmp r1, #0
	bge _08011AA4
	rsbs r1, r1, #0
_08011AA4:
	movs r4, #0xc0
	lsls r4, r4, #1
	cmp r1, r4
	bgt _08011AC0
	ldr r0, [r3, #4]
	asrs r1, r0, #8
	ldr r0, [r2, #4]
	asrs r0, r0, #8
	subs r1, r1, r0
	cmp r1, #0
	bge _08011ABC
	rsbs r1, r1, #0
_08011ABC:
	cmp r1, r4
	ble _08011B00
_08011AC0:
	movs r0, #1
	ldrb r1, [r2, #0xc]
	orrs r0, r1
	strb r0, [r2, #0xc]
	ldr r0, _08011AF8 @ =0x0000FFFF
	ldrh r4, [r2, #8]
	cmp r4, r0
	beq _08011B06
	ldrh r3, [r2, #8]
	ldr r0, _08011AFC @ =gUnknown_030012B4
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
	b _08011B06
	.align 2, 0
_08011AF4: .4byte gUnknown_030012D8
_08011AF8: .4byte 0x0000FFFF
_08011AFC: .4byte gUnknown_030012B4
_08011B00:
	adds r0, r2, #0
	bl sub_8008364
_08011B06:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8011B0C
sub_8011B0C: @ 0x08011B0C
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	mov r8, r0
	adds r5, r1, #0
	adds r6, r2, #0
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	mov r8, r0
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	lsls r6, r6, #0x10
	lsrs r6, r6, #0x10
	movs r0, #0x40
	bl sub_8026EDC
	adds r4, r0, #0
	bl sub_80084A4
	ldr r0, _08011B54 @ =gStaticData_087E41BC
	str r0, [r4, #0x18]
	adds r0, r4, #0
	bl nullsub_16
	mov r0, r8
	strh r0, [r4, #8]
	lsls r5, r5, #8
	str r5, [r4]
	lsls r6, r6, #8
	str r6, [r4, #4]
	adds r0, r4, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08011B54: .4byte gStaticData_087E41BC

	thumb_func_start nullsub_16
nullsub_16: @ 0x08011B58
	bx lr
	.align 2, 0

	thumb_func_start sub_8011B5C
sub_8011B5C: @ 0x08011B5C
	push {lr}
	ldr r2, _08011B6C @ =gStaticData_087E41BC
	str r2, [r0, #0x18]
	bl sub_8008484
	pop {r0}
	bx r0
	.align 2, 0
_08011B6C: .4byte gStaticData_087E41BC

	thumb_func_start sub_8011B70
sub_8011B70: @ 0x08011B70
	push {r4, lr}
	adds r4, r0, #0
	bl sub_80084A4
	ldr r0, _08011B8C @ =gStaticData_087E41BC
	str r0, [r4, #0x18]
	adds r0, r4, #0
	bl nullsub_16
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08011B8C: .4byte gStaticData_087E41BC

	thumb_func_start sub_8011B90
sub_8011B90: @ 0x08011B90
	adds r3, r0, #0
	adds r1, r3, #0
	adds r1, #0x29
	movs r0, #0
	strb r0, [r1]
	str r0, [r3, #8]
	adds r1, #3
	strb r0, [r1]
	subs r1, #5
	strb r0, [r1]
	adds r1, #1
	strb r0, [r1]
	adds r1, #7
	movs r2, #1
	strb r2, [r1]
	adds r1, #1
	strb r2, [r1]
	str r0, [r3, #0x14]
	str r0, [r3, #0x10]
	subs r1, #0xa
	strb r0, [r1]
	adds r1, #4
	strb r0, [r1]
	subs r1, #5
	strb r0, [r1]
	adds r1, #6
	strb r0, [r1]
	str r0, [r3, #0x18]
	str r0, [r3, #0x1c]
	adds r1, #8
	strb r0, [r1]
	adds r1, #1
	strb r0, [r1]
	bx lr

	thumb_func_start sub_8011BD4
sub_8011BD4: @ 0x08011BD4
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r6, r0, #0
	ldr r0, [r6, #8]
	cmp r0, #0x1d
	bne _08011BE2
	b _08012154
_08011BE2:
	subs r0, r2, #1
	cmp r0, #0x18
	bls _08011BEA
	b _08012154
_08011BEA:
	lsls r0, r0, #2
	ldr r1, _08011BF4 @ =_08011BF8
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08011BF4: .4byte _08011BF8
_08011BF8: @ jump table
	.4byte _080120CE @ case 0
	.4byte _0801209C @ case 1
	.4byte _080120A6 @ case 2
	.4byte _080120CE @ case 3
	.4byte _08012154 @ case 4
	.4byte _080120CE @ case 5
	.4byte _080120B0 @ case 6
	.4byte _080120BA @ case 7
	.4byte _080120C4 @ case 8
	.4byte _0801210C @ case 9
	.4byte _08012116 @ case 10
	.4byte _08011D80 @ case 11
	.4byte _08011E7E @ case 12
	.4byte _08011EF8 @ case 13
	.4byte _08012012 @ case 14
	.4byte _0801201C @ case 15
	.4byte _0801201C @ case 16
	.4byte _08012154 @ case 17
	.4byte _08012154 @ case 18
	.4byte _08012154 @ case 19
	.4byte _08012154 @ case 20
	.4byte _08012154 @ case 21
	.4byte _08011C5C @ case 22
	.4byte _08011D48 @ case 23
	.4byte _08011F70 @ case 24
_08011C5C:
	ldr r0, [r6, #0x10]
	bl sub_80083B8
	adds r2, r0, #0
	ldr r0, [r2, #4]
	ldrb r0, [r0]
	lsrs r0, r0, #4
	cmp r0, #6
	bhi _08011CA4
	lsls r0, r0, #2
	ldr r1, _08011C78 @ =_08011C7C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08011C78: .4byte _08011C7C
_08011C7C: @ jump table
	.4byte _08011C98 @ case 0
	.4byte _08011CA4 @ case 1
	.4byte _08011CA4 @ case 2
	.4byte _08011CA4 @ case 3
	.4byte _08011CA4 @ case 4
	.4byte _08011CA4 @ case 5
	.4byte _08011C9E @ case 6
_08011C98:
	adds r7, r2, #0
	adds r7, #0x24
	b _08011CA6
_08011C9E:
	adds r7, r2, #0
	adds r7, #0x14
	b _08011CA6
_08011CA4:
	ldr r7, _08011CF8 @ =gStaticData_0816B300
_08011CA6:
	ldr r0, [r6, #0x10]
	movs r5, #1
	ldr r1, _08011CFC @ =0x00000101
	adds r0, r0, r1
	movs r4, #0
	strb r5, [r0]
	str r4, [sp]
	adds r0, r6, #0
	movs r1, #0x1f
	movs r2, #0x1d
	movs r3, #0
	bl sub_8015780
	adds r0, r6, #0
	adds r0, #0x31
	strb r4, [r0]
	subs r0, #2
	strb r5, [r0]
	subs r0, #8
	strb r4, [r0]
	adds r0, #0xb
	strb r4, [r0]
	subs r0, #2
	strb r5, [r0]
	subs r0, #8
	strb r4, [r0]
	ldr r0, [r6, #0x10]
	bl sub_80083B8
	adds r2, r0, #0
	ldr r0, [r2, #4]
	ldrb r0, [r0]
	lsrs r0, r0, #4
	cmp r0, #6
	bhi _08011D2C
	lsls r0, r0, #2
	ldr r1, _08011D00 @ =_08011D04
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08011CF8: .4byte gStaticData_0816B300
_08011CFC: .4byte 0x00000101
_08011D00: .4byte _08011D04
_08011D04: @ jump table
	.4byte _08011D20 @ case 0
	.4byte _08011D2C @ case 1
	.4byte _08011D2C @ case 2
	.4byte _08011D2C @ case 3
	.4byte _08011D2C @ case 4
	.4byte _08011D2C @ case 5
	.4byte _08011D26 @ case 6
_08011D20:
	adds r0, r2, #0
	adds r0, #0x24
	b _08011D2E
_08011D26:
	adds r0, r2, #0
	adds r0, #0x14
	b _08011D2E
_08011D2C:
	ldr r0, _08011D44 @ =gStaticData_0816B300
_08011D2E:
	movs r2, #2
	ldrsh r1, [r0, r2]
	movs r3, #2
	ldrsh r0, [r7, r3]
	subs r1, r1, r0
	ldr r2, [r6, #0x10]
	ldr r0, [r2, #4]
	lsls r1, r1, #8
	subs r0, r0, r1
	str r0, [r2, #4]
	b _08012154
	.align 2, 0
_08011D44: .4byte gStaticData_0816B300
_08011D48:
	ldr r0, [r6, #0x10]
	ldr r5, _08011D78 @ =0x00000101
	adds r0, r0, r5
	movs r4, #0
	strb r4, [r0]
	ldr r3, _08011D7C @ =0x7FFFFFFF
	str r3, [sp]
	adds r0, r6, #0
	movs r1, #0x1a
	movs r2, #0x1b
	bl sub_8015780
	movs r2, #4
	adds r0, r6, #0
	adds r0, #0x32
	strb r4, [r0]
	adds r1, r6, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	adds r0, r6, #0
	adds r0, #0x28
	strb r2, [r0]
	b _08012154
	.align 2, 0
_08011D78: .4byte 0x00000101
_08011D7C: .4byte 0x7FFFFFFF
_08011D80:
	ldr r2, [r6, #8]
	cmp r2, #0
	bne _08011DAE
	adds r0, r6, #0
	adds r0, #0x31
	strb r2, [r0]
	adds r1, r6, #0
	adds r1, #0x2f
	movs r0, #1
	strb r0, [r1]
	adds r0, r6, #0
	adds r0, #0x27
	strb r2, [r0]
	adds r0, #5
	strb r2, [r0]
	ldr r0, [r6, #0x10]
	adds r0, #0x68
	ldrb r1, [r0]
	orrs r3, r1
	strb r3, [r0]
	ldr r0, [r6, #0x10]
	str r2, [r0, #0x60]
	b _08012154
_08011DAE:
	movs r2, #3
	ands r2, r3
	cmp r2, #2
	bne _08011DEC
	adds r4, r6, #0
	adds r4, #0x27
	ldrb r2, [r4]
	cmp r2, #0
	beq _08011E1E
	ldr r0, _08011DE8 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _08011E1E
	adds r0, r6, #0
	adds r0, #0x2c
	movs r1, #0
	strb r2, [r0]
	adds r0, #5
	strb r1, [r0]
	adds r2, r6, #0
	adds r2, #0x2f
	movs r0, #1
	strb r0, [r2]
	strb r1, [r4]
	b _08011E1A
	.align 2, 0
_08011DE8: .4byte gUnknown_030012D8
_08011DEC:
	cmp r2, #1
	bne _08011E30
	adds r5, r6, #0
	adds r5, #0x27
	ldrb r4, [r5]
	cmp r4, #0
	beq _08011E1E
	ldr r0, _08011E68 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	lsrs r1, r0, #0x1f
	cmp r1, #0
	bne _08011E1E
	adds r0, r6, #0
	adds r0, #0x2c
	strb r4, [r0]
	adds r0, #5
	strb r1, [r0]
	subs r0, #2
	strb r2, [r0]
	strb r1, [r5]
_08011E1A:
	ldr r0, [r6, #0x10]
	str r1, [r0, #0x60]
_08011E1E:
	ldr r0, [r6, #8]
	adds r1, r6, #0
	adds r1, #0x2b
	movs r0, #3
	strb r0, [r1]
	ldr r0, [r6, #0x10]
	movs r1, #1
	adds r0, #0x90
	strb r1, [r0]
_08011E30:
	ldr r0, [r6, #8]
	cmp r0, #0xc
	bne _08011E6C
	ldr r0, _08011E68 @ =gUnknown_030012D8
	ldr r4, [r0]
	ldr r0, [r4, #0x30]
	cmp r0, #0
	beq _08011E6C
	ldr r0, [r6, #0x1c]
	str r0, [r6, #0x18]
	adds r1, r6, #0
	adds r1, #0x2b
	movs r0, #0
	strb r0, [r1]
	ldr r0, [r4, #0x20]
	adds r2, r4, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r3, [r2]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	subs r0, #1
	str r0, [r4, #0x30]
	b _08012154
	.align 2, 0
_08011E68: .4byte gUnknown_030012D8
_08011E6C:
	ldr r0, [r6, #0x10]
	adds r0, #0x68
	ldrb r5, [r0]
	orrs r3, r5
	movs r1, #0
	strb r3, [r0]
	ldr r0, [r6, #0x10]
	str r1, [r0, #0x60]
	b _08012154
_08011E7E:
	ldr r0, _08011EC8 @ =gUnknown_030007E0
	ldr r0, [r0]
	adds r2, r0, #0
	movs r0, #0x80
	lsls r0, r0, #1
	ands r0, r2
	cmp r0, #0
	beq _08011E96
	adds r1, r6, #0
	adds r1, #0x34
	movs r0, #1
	strb r0, [r1]
_08011E96:
	movs r5, #1
	movs r4, #1
	ands r4, r2
	cmp r4, #0
	beq _08011ED0
	ldr r3, _08011ECC @ =0x7FFFFFFF
	str r3, [sp]
	adds r0, r6, #0
	movs r1, #5
	movs r2, #0x13
	bl sub_8015780
	ldr r0, [r6, #0x10]
	movs r1, #0
	str r1, [r0, #0x64]
	movs r2, #0x10
	adds r0, r6, #0
	adds r0, #0x32
	strb r1, [r0]
	subs r0, #2
	strb r5, [r0]
	subs r0, #8
	strb r2, [r0]
	b _0801200C
	.align 2, 0
_08011EC8: .4byte gUnknown_030007E0
_08011ECC: .4byte 0x7FFFFFFF
_08011ED0:
	ldr r3, _08011EF4 @ =0x7FFFFFFF
	str r3, [sp]
	adds r0, r6, #0
	movs r1, #5
	movs r2, #0x13
	bl sub_8015780
	ldr r0, [r6, #0x10]
	str r4, [r0, #0x64]
	movs r1, #0xf
	adds r0, r6, #0
	adds r0, #0x32
	strb r4, [r0]
	subs r0, #2
	strb r5, [r0]
	subs r0, #8
	strb r1, [r0]
	b _0801200C
	.align 2, 0
_08011EF4: .4byte 0x7FFFFFFF
_08011EF8:
	ldr r0, _08011F40 @ =gUnknown_030007E0
	ldr r0, [r0]
	adds r2, r0, #0
	movs r0, #0x80
	lsls r0, r0, #1
	ands r0, r2
	cmp r0, #0
	beq _08011F10
	adds r1, r6, #0
	adds r1, #0x34
	movs r0, #1
	strb r0, [r1]
_08011F10:
	movs r5, #1
	movs r4, #1
	ands r4, r2
	cmp r4, #0
	beq _08011F48
	ldr r3, _08011F44 @ =0x7FFFFFFF
	str r3, [sp]
	adds r0, r6, #0
	movs r1, #5
	movs r2, #0x13
	bl sub_8015780
	ldr r0, [r6, #0x10]
	movs r1, #0
	str r1, [r0, #0x64]
	movs r2, #0x12
	adds r0, r6, #0
	adds r0, #0x32
	strb r1, [r0]
	subs r0, #2
	strb r5, [r0]
	subs r0, #8
	strb r2, [r0]
	b _0801200C
	.align 2, 0
_08011F40: .4byte gUnknown_030007E0
_08011F44: .4byte 0x7FFFFFFF
_08011F48:
	ldr r3, _08011F6C @ =0x7FFFFFFF
	str r3, [sp]
	adds r0, r6, #0
	movs r1, #5
	movs r2, #0x13
	bl sub_8015780
	ldr r0, [r6, #0x10]
	str r4, [r0, #0x64]
	movs r1, #0x11
	adds r0, r6, #0
	adds r0, #0x32
	strb r4, [r0]
	subs r0, #2
	strb r5, [r0]
	subs r0, #8
	strb r1, [r0]
	b _0801200C
	.align 2, 0
_08011F6C: .4byte 0x7FFFFFFF
_08011F70:
	ldr r0, _08011FC4 @ =gUnknown_030007E0
	ldr r4, [r0]
	ldr r0, _08011FC8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xa
	bl PlaySfx
	movs r5, #1
	movs r0, #1
	ands r4, r0
	cmp r4, #0
	beq _08011FCC
	movs r0, #0x18
	str r0, [sp]
	adds r0, r6, #0
	movs r1, #0xe
	movs r2, #0x10
	movs r3, #0
	bl sub_8015780
	adds r0, r6, #0
	adds r0, #0x22
	movs r2, #0
	strb r2, [r0]
	adds r0, #1
	strb r2, [r0]
	adds r0, #1
	strb r2, [r0]
	adds r1, r6, #0
	adds r1, #0x20
	movs r0, #3
	strb r0, [r1]
	ldr r0, [r6, #0x10]
	str r2, [r0, #0x64]
	movs r1, #0x14
	adds r0, r6, #0
	adds r0, #0x32
	strb r2, [r0]
	b _08011FFE
	.align 2, 0
_08011FC4: .4byte gUnknown_030007E0
_08011FC8: .4byte gUnknown_030012BC
_08011FCC:
	movs r0, #0x18
	str r0, [sp]
	adds r0, r6, #0
	movs r1, #0xe
	movs r2, #0x10
	movs r3, #0
	bl sub_8015780
	adds r0, r6, #0
	adds r0, #0x22
	strb r4, [r0]
	adds r0, #1
	strb r4, [r0]
	adds r0, #1
	strb r4, [r0]
	adds r1, r6, #0
	adds r1, #0x20
	movs r0, #3
	strb r0, [r1]
	ldr r0, [r6, #0x10]
	str r4, [r0, #0x64]
	movs r1, #0x13
	adds r0, r6, #0
	adds r0, #0x32
	strb r4, [r0]
_08011FFE:
	subs r0, #2
	strb r5, [r0]
	subs r0, #8
	strb r1, [r0]
	adds r0, r6, #0
	bl sub_8012AF4
_0801200C:
	movs r0, #0
	str r0, [r6, #0x18]
	b _08012154
_08012012:
	ldr r0, _0801208C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8001AC4
_0801201C:
	ldr r0, _0801208C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x2c
	bl PlaySfx
	ldr r0, _08012090 @ =gUnknown_030012D8
	ldr r1, [r0]
	movs r0, #0x7f
	ldrb r2, [r1, #0xc]
	ands r0, r2
	strb r0, [r1, #0xc]
	ldr r3, _08012094 @ =0x7FFFFFFF
	str r3, [sp]
	adds r0, r6, #0
	movs r1, #0x1e
	movs r2, #0x24
	bl sub_8015780
	ldr r0, _08012098 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r3, [r6, #0x10]
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
	movs r1, #0
	adds r0, r6, #0
	adds r0, #0x31
	strb r1, [r0]
	subs r0, #2
	movs r2, #1
	strb r2, [r0]
	subs r0, #8
	strb r1, [r0]
	adds r0, #0xb
	strb r1, [r0]
	subs r0, #2
	strb r2, [r0]
	subs r0, #8
	strb r1, [r0]
	b _08012154
	.align 2, 0
_0801208C: .4byte gUnknown_030012BC
_08012090: .4byte gUnknown_030012D8
_08012094: .4byte 0x7FFFFFFF
_08012098: .4byte gUnknown_030012B8
_0801209C:
	adds r0, r6, #0
	movs r1, #0x2e
	bl sub_8012160
	b _08012154
_080120A6:
	adds r0, r6, #0
	movs r1, #0x2c
	bl sub_8012160
	b _08012154
_080120B0:
	adds r0, r6, #0
	movs r1, #0x2b
	bl sub_8012160
	b _08012154
_080120BA:
	adds r0, r6, #0
	movs r1, #0x2f
	bl sub_8012160
	b _08012154
_080120C4:
	adds r0, r6, #0
	movs r1, #0x2d
	bl sub_8012160
	b _08012154
_080120CE:
	adds r0, r6, #0
	movs r1, #0x1c
	bl sub_8012160
	ldr r1, [r6, #0x10]
	movs r2, #0
	movs r3, #0x80
	lsls r3, r3, #1
	adds r0, r1, r3
	ldrb r0, [r0]
	cmp r0, #0
	bne _080120E8
	str r2, [r1, #0x60]
_080120E8:
	str r2, [r1, #0x48]
	str r2, [r1, #0x4c]
	str r2, [r1, #0x50]
	ldr r0, [r6, #0x10]
	ldr r1, _08012104 @ =0xFFFFFF00
	str r1, [r0, #0x64]
	str r1, [r0, #0x54]
	str r2, [r0, #0x58]
	str r1, [r0, #0x5c]
	ldr r0, _08012108 @ =gUnknown_030012D4
	ldr r1, [r0]
	movs r0, #3
	str r0, [r1, #0x14]
	b _08012154
	.align 2, 0
_08012104: .4byte 0xFFFFFF00
_08012108: .4byte gUnknown_030012D4
_0801210C:
	adds r0, r6, #0
	movs r1, #0x2a
	bl sub_8012160
	b _08012154
_08012116:
	ldr r0, _0801215C @ =gUnknown_030012D8
	ldr r1, [r0]
	adds r1, #0x68
	movs r0, #8
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _08012154
	ldr r0, [r6, #0x10]
	movs r1, #0xb
	bl sub_800AAEC
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #1
	bne _08012154
	ldr r1, [r6, #0x10]
	movs r0, #2
	rsbs r0, r0, #0
	ldrb r5, [r1, #0xd]
	ands r0, r5
	strb r0, [r1, #0xd]
	ldr r1, [r6, #0x10]
	movs r0, #3
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xd]
	ands r0, r2
	strb r0, [r1, #0xd]
	adds r0, r6, #0
	bl sub_8015558
_08012154:
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801215C: .4byte gUnknown_030012D8

	thumb_func_start sub_8012160
sub_8012160: @ 0x08012160
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	adds r4, r1, #0
	ldr r0, _0801222C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x1b
	bl PlaySfx
	ldr r2, [r6, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r6, r0
	ldr r1, [r6, #0x10]
	ldr r3, [r2, #4]
	adds r2, r4, #0
	bl sub_803AD84
	ldr r1, [r6, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x1d
	bl sub_803AD80
	movs r4, #0
	adds r0, r6, #0
	adds r0, #0x31
	strb r4, [r0]
	subs r0, #2
	movs r5, #1
	strb r5, [r0]
	subs r0, #8
	strb r4, [r0]
	adds r0, #0xb
	strb r4, [r0]
	subs r0, #2
	strb r5, [r0]
	subs r0, #8
	strb r4, [r0]
	adds r0, r6, #0
	bl sub_8012AF4
	ldr r0, [r6, #0x10]
	movs r1, #0x80
	lsls r1, r1, #1
	adds r0, r0, r1
	strb r4, [r0]
	ldr r0, [r6, #0x10]
	movs r2, #0x81
	lsls r2, r2, #1
	adds r0, r0, r2
	strb r4, [r0]
	ldr r0, [r6, #0x10]
	adds r1, #3
	adds r0, r0, r1
	strb r4, [r0]
	ldr r1, [r6, #0x10]
	movs r0, #0x7f
	ldrb r2, [r1, #0xc]
	ands r0, r2
	strb r0, [r1, #0xc]
	ldr r1, [r6, #0x10]
	movs r0, #0x41
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xc]
	ands r0, r2
	strb r0, [r1, #0xc]
	ldr r0, [r6, #0x10]
	movs r1, #0x82
	lsls r1, r1, #1
	adds r0, r0, r1
	strb r5, [r0]
	ldr r0, _08012230 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023234
	ldr r0, _08012234 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r3, [r6, #0x10]
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
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801222C: .4byte gUnknown_030012BC
_08012230: .4byte gUnknown_030012C0
_08012234: .4byte gUnknown_030012B8

	thumb_func_start sub_8012238
sub_8012238: @ 0x08012238
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r6, r0, #0
	ldr r1, _08012264 @ =gUnknown_030012D8
	ldr r4, [r1]
	movs r2, #0x80
	lsls r2, r2, #1
	adds r0, r4, r2
	ldrb r5, [r0]
	cmp r5, #0
	beq _0801229A
	adds r3, r4, #0
	adds r3, #0x2d
	ldrb r0, [r3]
	adds r2, r0, #0
	cmp r0, #0x12
	beq _0801226E
	cmp r0, #0x12
	bgt _08012268
	cmp r0, #0xd
	beq _0801227A
	b _080122C0
	.align 2, 0
_08012264: .4byte gUnknown_030012D8
_08012268:
	cmp r2, #0x18
	beq _0801227A
	b _080122C0
_0801226E:
	ldr r0, [r4, #0x60]
	cmp r0, #0
	beq _080122C0
	movs r0, #0x25
	strb r0, [r3]
	b _08012284
_0801227A:
	ldr r4, [r1]
	movs r0, #0x26
	adds r1, r4, #0
	adds r1, #0x2d
	strb r0, [r1]
_08012284:
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	b _080122C0
_0801229A:
	adds r0, r4, #0
	adds r0, #0x2d
	ldrb r0, [r0]
	cmp r0, #0x25
	beq _080122A8
	cmp r0, #0x26
	bne _080122C0
_080122A8:
	ldr r0, _080122C8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x36
	bl sub_80019A8
	str r5, [sp]
	adds r0, r6, #0
	movs r1, #0
	movs r2, #0x12
	movs r3, #0
	bl sub_8015780
_080122C0:
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_080122C8: .4byte gUnknown_030012BC

	thumb_func_start sub_80122CC
sub_80122CC: @ 0x080122CC
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r0, _080122F0 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r3, r0, #0x18
	movs r2, #0
	ldr r0, [r4, #8]
	cmp r0, #0x26
	bls _080122E6
	b _08012416
_080122E6:
	lsls r0, r0, #2
	ldr r1, _080122F4 @ =_080122F8
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080122F0: .4byte gUnknown_03001304
_080122F4: .4byte _080122F8
_080122F8: @ jump table
	.4byte _08012394 @ case 0
	.4byte _08012416 @ case 1
	.4byte _08012416 @ case 2
	.4byte _08012394 @ case 3
	.4byte _08012394 @ case 4
	.4byte _08012394 @ case 5
	.4byte _08012416 @ case 6
	.4byte _08012394 @ case 7
	.4byte _08012416 @ case 8
	.4byte _08012394 @ case 9
	.4byte _08012416 @ case 10
	.4byte _08012394 @ case 11
	.4byte _08012416 @ case 12
	.4byte _08012394 @ case 13
	.4byte _08012394 @ case 14
	.4byte _08012394 @ case 15
	.4byte _08012416 @ case 16
	.4byte _08012416 @ case 17
	.4byte _08012416 @ case 18
	.4byte _08012416 @ case 19
	.4byte _08012394 @ case 20
	.4byte _08012416 @ case 21
	.4byte _08012416 @ case 22
	.4byte _08012416 @ case 23
	.4byte _08012416 @ case 24
	.4byte _08012416 @ case 25
	.4byte _08012394 @ case 26
	.4byte _08012416 @ case 27
	.4byte _08012416 @ case 28
	.4byte _08012416 @ case 29
	.4byte _08012416 @ case 30
	.4byte _08012416 @ case 31
	.4byte _08012394 @ case 32
	.4byte _08012394 @ case 33
	.4byte _08012416 @ case 34
	.4byte _08012416 @ case 35
	.4byte _08012416 @ case 36
	.4byte _08012394 @ case 37
	.4byte _08012394 @ case 38
_08012394:
	ldr r1, [r4, #0x10]
	adds r1, #0x28
	movs r0, #0x21
	rsbs r0, r0, #0
	ldrb r5, [r1]
	ands r0, r5
	strb r0, [r1]
	ldr r0, [r4, #0x10]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _080123DA
	cmp r3, #4
	beq _080123BA
	cmp r3, #6
	beq _080123BA
	cmp r3, #8
	bne _080123DA
_080123BA:
	ldr r1, [r4, #0x10]
	movs r2, #0
	adds r1, #0x28
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r3, [r1]
	ands r0, r3
	strb r0, [r1]
	adds r1, r4, #0
	adds r1, #0x2f
	movs r0, #1
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x29
	strb r2, [r0]
	b _08012414
_080123DA:
	ldr r0, [r4, #0x10]
	adds r1, r0, #0
	adds r1, #0x28
	ldrb r1, [r1]
	lsls r1, r1, #0x1b
	cmp r1, #0
	blt _08012416
	cmp r3, #3
	beq _080123F4
	cmp r3, #5
	beq _080123F4
	cmp r3, #7
	bne _08012416
_080123F4:
	movs r3, #1
	adds r2, r0, #0
	adds r2, #0x28
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r5, [r2]
	ands r0, r5
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r2]
	adds r0, r4, #0
	adds r0, #0x2f
	movs r1, #0
	strb r3, [r0]
	subs r0, #6
	strb r1, [r0]
_08012414:
	movs r2, #1
_08012416:
	adds r0, r2, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8012420
sub_8012420: @ 0x08012420
	push {r4, r5, r6, r7, lr}
	sub sp, #8
	adds r5, r0, #0
	ldr r0, _08012564 @ =gUnknown_030007E0
	ldr r1, [r0]
	adds r2, r5, #0
	adds r2, #0x34
	ldrb r0, [r2]
	cmp r0, #0
	beq _08012444
	movs r0, #0x80
	lsls r0, r0, #1
	ands r1, r0
	lsls r0, r1, #0x10
	lsrs r0, r0, #0x10
	cmp r0, #0
	bne _08012444
	strb r0, [r2]
_08012444:
	adds r4, r5, #0
	adds r4, #0x2e
	ldr r0, [r5, #0x10]
	movs r1, #0x80
	lsls r1, r1, #1
	adds r0, r0, r1
	ldrb r2, [r4]
	ldrb r0, [r0]
	cmp r2, r0
	beq _0801245E
	adds r0, r5, #0
	bl sub_8012238
_0801245E:
	ldr r0, [r5, #0x10]
	movs r3, #0x80
	lsls r3, r3, #1
	adds r0, r0, r3
	ldrb r0, [r0]
	movs r3, #0
	strb r0, [r4]
	ldr r2, [r5, #0x10]
	ldr r1, [r2, #4]
	ldr r4, _08012568 @ =gUnknown_03001308
	ldr r0, [r4]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x14]
	lsls r0, r0, #8
	ldr r6, _0801256C @ =0xFFFFEC00
	adds r0, r0, r6
	cmp r1, r0
	ble _080124DA
	movs r0, #0x7f
	ldrb r1, [r2, #0xc]
	ands r0, r1
	strb r0, [r2, #0xc]
	ldr r1, [r5, #0x10]
	movs r2, #0x80
	lsls r2, r2, #1
	adds r0, r1, r2
	ldrb r0, [r0]
	cmp r0, #0
	bne _0801249A
	str r3, [r1, #0x60]
_0801249A:
	str r3, [r1, #0x48]
	str r3, [r1, #0x4c]
	str r3, [r1, #0x50]
	ldr r2, [r5, #0x10]
	ldr r1, [r2, #4]
	ldr r0, [r4]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x14]
	lsls r0, r0, #8
	movs r4, #0xa0
	lsls r4, r4, #5
	adds r0, r0, r4
	cmp r1, r0
	ble _080124DA
	adds r0, r2, #0
	adds r0, #0x8c
	str r3, [r0]
	ldr r0, _08012570 @ =gUnknown_030012C0
	ldr r0, [r0]
	movs r1, #0
	bl sub_80231EC
	ldr r1, [r5, #0xc]
	movs r6, #0x10
	ldrsh r0, [r1, r6]
	adds r0, r5, r0
	ldr r4, [r1, #0x14]
	movs r1, #0
	movs r2, #1
	movs r3, #0
	bl sub_803AD88
_080124DA:
	adds r1, r5, #0
	adds r1, #0x26
	ldrb r0, [r1]
	cmp r0, #0
	beq _080124E8
	subs r0, #1
	strb r0, [r1]
_080124E8:
	adds r2, r5, #0
	adds r2, #0x2b
	ldrb r1, [r2]
	cmp r1, #0
	beq _08012536
	ldr r0, [r5, #0x10]
	adds r0, #0x94
	ldrb r0, [r0]
	cmp r0, #1
	bgt _08012536
	subs r0, r1, #1
	strb r0, [r2]
	lsls r0, r0, #0x18
	lsrs r4, r0, #0x18
	cmp r4, #0
	bne _08012536
	movs r0, #0x27
	adds r0, r0, r5
	mov ip, r0
	ldrb r0, [r0]
	cmp r0, #0
	bne _0801252E
	adds r0, r5, #0
	adds r0, #0x2c
	ldrb r2, [r0]
	adds r1, r5, #0
	adds r1, #0x31
	strb r4, [r1]
	adds r3, r5, #0
	adds r3, #0x2f
	movs r1, #1
	strb r1, [r3]
	mov r1, ip
	strb r2, [r1]
	strb r4, [r0]
_0801252E:
	ldr r0, _08012574 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x90
	strb r4, [r0]
_08012536:
	ldr r1, _08012578 @ =gStaticData_0816BF20
	ldr r0, [r5, #8]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r4, #2
	ldrsh r2, [r0, r4]
	adds r4, r1, #0
	cmp r2, #0
	ble _0801257C
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
	str r0, [sp]
	str r1, [sp, #4]
	ldr r3, [sp, #4]
	b _08012582
	.align 2, 0
_08012564: .4byte gUnknown_030007E0
_08012568: .4byte gUnknown_03001308
_0801256C: .4byte 0xFFFFEC00
_08012570: .4byte gUnknown_030012C0
_08012574: .4byte gUnknown_030012D8
_08012578: .4byte gStaticData_0816BF20
_0801257C:
	adds r0, r4, #4
	adds r0, r3, r0
	ldr r3, [r0]
_08012582:
	ldr r0, [r5, #8]
	lsls r0, r0, #3
	adds r0, r0, r4
	movs r4, #0
	ldrsh r1, [r0, r4]
	cmp r2, #0
	ble _0801259A
	ldr r6, [sp]
	lsls r0, r6, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _0801259C
_0801259A:
	adds r0, r1, #0
_0801259C:
	adds r0, r5, r0
	bl sub_803AD84
	ldr r1, [r5, #0x10]
	adds r1, #0x68
	movs r0, #8
	ldrb r2, [r1]
	ands r0, r2
	movs r3, #0
	strb r0, [r1]
	ldr r0, [r5, #0x10]
	adds r0, #0x68
	ldrb r0, [r0]
	cmp r0, #8
	bne _080125DA
	adds r2, r5, #0
	adds r2, #0x28
	ldrb r0, [r2]
	subs r0, #4
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #1
	bhi _080125DA
	adds r0, r5, #0
	adds r0, #0x32
	strb r3, [r0]
	adds r1, r5, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	strb r3, [r2]
_080125DA:
	adds r0, r5, #0
	bl sub_8012AF4
	ldr r2, [r5, #0x10]
	ldrb r3, [r2, #0xc]
	lsrs r1, r3, #7
	cmp r1, #0
	bne _080125FE
	movs r4, #0x80
	lsls r4, r4, #1
	adds r0, r2, r4
	ldrb r0, [r0]
	cmp r0, #0
	bne _080125F8
	str r1, [r2, #0x60]
_080125F8:
	str r1, [r2, #0x48]
	str r1, [r2, #0x4c]
	str r1, [r2, #0x50]
_080125FE:
	ldr r0, [r5, #8]
	subs r0, #0xc
	cmp r0, #0x15
	bhi _08012684
	lsls r0, r0, #2
	ldr r1, _08012610 @ =_08012614
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08012610: .4byte _08012614
_08012614: @ jump table
	.4byte _08012672 @ case 0
	.4byte _0801266C @ case 1
	.4byte _0801266C @ case 2
	.4byte _0801266C @ case 3
	.4byte _08012684 @ case 4
	.4byte _08012684 @ case 5
	.4byte _08012684 @ case 6
	.4byte _08012684 @ case 7
	.4byte _08012684 @ case 8
	.4byte _08012684 @ case 9
	.4byte _08012684 @ case 10
	.4byte _08012684 @ case 11
	.4byte _08012678 @ case 12
	.4byte _0801267E @ case 13
	.4byte _08012684 @ case 14
	.4byte _08012684 @ case 15
	.4byte _08012684 @ case 16
	.4byte _08012684 @ case 17
	.4byte _08012684 @ case 18
	.4byte _08012684 @ case 19
	.4byte _08012684 @ case 20
	.4byte _0801266C @ case 21
_0801266C:
	ldr r1, [r5, #0x10]
	movs r0, #0x13
	b _08012688
_08012672:
	ldr r1, [r5, #0x10]
	movs r0, #0x14
	b _08012688
_08012678:
	ldr r1, [r5, #0x10]
	movs r0, #0x15
	b _08012688
_0801267E:
	ldr r1, [r5, #0x10]
	movs r0, #0x16
	b _08012688
_08012684:
	ldr r1, [r5, #0x10]
	movs r0, #1
_08012688:
	strb r0, [r1, #0xa]
	add sp, #8
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8012694
sub_8012694: @ 0x08012694
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _0801274C @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp]
	ldr r0, [r4, #8]
	cmp r0, #0xe
	bne _080126A8
	b _08012830
_080126A8:
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r6, #1
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	bne _080126B8
	b _08012830
_080126B8:
	ldr r5, [r4, #0x18]
	cmp r5, #0
	beq _080126C0
	b _08012830
_080126C0:
	ldr r0, _08012750 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231CC
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _080126D0
	b _08012830
_080126D0:
	ldr r0, [r4, #0x10]
	adds r1, r0, #0
	adds r1, #0x2d
	adds r2, r0, #0
	ldrb r1, [r1]
	cmp r1, #6
	bne _08012758
	ldr r0, [r2, #0x30]
	cmp r0, #0
	blt _08012758
	ldr r0, [r4, #0x18]
	adds r0, #1
	str r0, [r4, #0x18]
	ldr r0, _08012754 @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r1, #0x80
	lsls r1, r1, #1
	adds r0, r0, r1
	strb r5, [r0]
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x12
	bl sub_803AD84
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #9
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #6
	bl sub_803AD84
	movs r1, #0xd
	adds r0, r4, #0
	adds r0, #0x31
	strb r5, [r0]
	subs r0, #2
	strb r6, [r0]
	subs r0, #8
	strb r1, [r0]
	adds r0, #0xb
	strb r5, [r0]
	subs r0, #2
	strb r6, [r0]
	subs r0, #8
	strb r1, [r0]
	b _0801281A
	.align 2, 0
_0801274C: .4byte gUnknown_030007E0
_08012750: .4byte gUnknown_030012C0
_08012754: .4byte gUnknown_030012D8
_08012758:
	adds r0, r2, #0
	adds r0, #0x2d
	ldrb r0, [r0]
	cmp r0, #0xb
	bne _080127C4
	ldr r0, [r2, #0x30]
	cmp r0, #0
	blt _080127C4
	ldr r0, [r4, #0x18]
	adds r0, #1
	str r0, [r4, #0x18]
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
	movs r2, #0xa
	bl sub_803AD84
	movs r2, #0xe
	adds r1, r4, #0
	adds r1, #0x31
	movs r0, #0
	strb r0, [r1]
	adds r3, r4, #0
	adds r3, #0x2f
	movs r1, #1
	strb r1, [r3]
	subs r3, #8
	strb r2, [r3]
	adds r3, #0xb
	strb r0, [r3]
	adds r0, r4, #0
	adds r0, #0x30
	strb r1, [r0]
	subs r0, #8
	strb r2, [r0]
	ldr r0, _080127C0 @ =gUnknown_030012BC
	ldr r0, [r0]
	adds r2, #0xf2
	b _08012822
	.align 2, 0
_080127C0: .4byte gUnknown_030012BC
_080127C4:
	adds r0, r2, #0
	adds r0, #0x2d
	ldrb r5, [r0]
	cmp r5, #0xc
	bne _08012830
	ldr r0, [r4, #0x18]
	adds r0, #1
	str r0, [r4, #0x18]
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
	movs r2, #0xa
	bl sub_803AD84
	adds r1, r4, #0
	adds r1, #0x31
	movs r0, #0
	strb r0, [r1]
	adds r2, r4, #0
	adds r2, #0x2f
	movs r1, #1
	strb r1, [r2]
	subs r2, #8
	strb r5, [r2]
	adds r2, #0xb
	strb r0, [r2]
	adds r0, r4, #0
	adds r0, #0x30
	strb r1, [r0]
	subs r0, #8
	strb r5, [r0]
_0801281A:
	ldr r0, _0801282C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
_08012822:
	movs r1, #0xc
	bl PlaySfx
	movs r0, #1
	b _08012832
	.align 2, 0
_0801282C: .4byte gUnknown_030012BC
_08012830:
	movs r0, #0
_08012832:
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_801283C
sub_801283C: @ 0x0801283C
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	movs r6, #0
	ldr r0, [r4, #0x10]
	ldr r1, [r0, #0x64]
	ldr r0, _0801287C @ =0x0000027F
	cmp r1, r0
	bgt _0801285C
	movs r6, #1
	adds r0, r4, #0
	bl sub_8012694
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801285C
	b _08012A72
_0801285C:
	ldr r0, _08012880 @ =gUnknown_030007E0
	ldr r5, [r0]
	ldr r0, [r4, #8]
	cmp r0, #7
	bne _08012888
	ldr r1, [r4, #0x10]
	ldr r0, [r1, #0x64]
	rsbs r2, r0, #0
	ldr r0, _08012884 @ =0x000001BF
	cmp r2, r0
	bgt _080128FA
	subs r0, #0x40
	cmp r2, r0
	bgt _080128FA
	b _080128B6
	.align 2, 0
_0801287C: .4byte 0x0000027F
_08012880: .4byte gUnknown_030007E0
_08012884: .4byte 0x000001BF
_08012888:
	cmp r0, #9
	bne _080128A4
	ldr r1, [r4, #0x10]
	ldr r0, [r1, #0x64]
	rsbs r2, r0, #0
	ldr r0, _080128A0 @ =0x000001BF
	cmp r2, r0
	bgt _080128FA
	cmp r2, #0x7f
	bgt _080128FA
	b _080128B6
	.align 2, 0
_080128A0: .4byte 0x000001BF
_080128A4:
	cmp r0, #0xb
	bne _080128D0
	ldr r1, [r4, #0x10]
	ldr r0, [r1, #0x64]
	rsbs r0, r0, #0
	cmp r0, #0xff
	bgt _080128FA
	cmp r0, #0x1f
	bgt _080128FA
_080128B6:
	movs r0, #1
	ldrb r2, [r1, #0xd]
	orrs r0, r2
	strb r0, [r1, #0xd]
	ldr r1, [r4, #0xc]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x1a
	bl sub_803AD80
	b _080128FA
_080128D0:
	cmp r0, #0xe
	bne _08012900
	adds r0, r4, #0
	adds r0, #0x22
	ldrb r0, [r0]
	cmp r0, #0
	beq _080128FA
	ldr r1, [r4, #0x10]
	ldr r2, [r1, #0x64]
	rsbs r0, r2, #0
	cmp r0, #0x7f
	bgt _080128F0
	movs r0, #1
	ldrb r3, [r1, #0xd]
	orrs r0, r3
	strb r0, [r1, #0xd]
_080128F0:
	cmp r2, #0
	ble _080128FA
	adds r0, r4, #0
	bl sub_80151C8
_080128FA:
	ldr r0, [r4, #8]
	cmp r0, #0xe
	beq _080129B8
_08012900:
	cmp r0, #0xb
	beq _080129B8
	cmp r6, #0
	beq _080129B8
	movs r0, #0x80
	lsls r0, r0, #1
	ands r5, r0
	cmp r5, #0
	beq _080129B8
	adds r0, r4, #0
	adds r0, #0x34
	ldrb r5, [r0]
	cmp r5, #0
	bne _080129B8
	ldr r1, [r4, #0x10]
	movs r0, #1
	ldrb r2, [r1, #0xd]
	orrs r0, r2
	strb r0, [r1, #0xd]
	adds r0, r4, #0
	adds r0, #0x2d
	ldrb r1, [r0]
	cmp r1, #9
	beq _08012936
	ldr r0, [r4, #8]
	cmp r0, #9
	bne _08012972
_08012936:
	ldr r1, [r4, #0xc]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xa
	bl sub_803AD80
	adds r0, r4, #0
	adds r0, #0x31
	strb r5, [r0]
	subs r0, #2
	movs r1, #1
	strb r1, [r0]
	subs r0, #8
	strb r5, [r0]
	movs r2, #0x16
	adds r0, #0xb
	strb r5, [r0]
	subs r0, #2
	strb r1, [r0]
	subs r0, #8
	strb r2, [r0]
	adds r0, r4, #0
	bl sub_80138E8
	adds r0, r4, #0
	adds r0, #0x2a
	strb r5, [r0]
	b _08012A72
_08012972:
	cmp r1, #7
	bne _080129B8
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #8
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r3, #0
	ldrsh r0, [r2, r3]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x19
	bl sub_803AD84
	adds r0, r4, #0
	adds r0, #0x31
	strb r5, [r0]
	subs r0, #2
	movs r1, #1
	strb r1, [r0]
	subs r0, #8
	strb r5, [r0]
	movs r2, #0x15
	adds r0, #0xb
	strb r5, [r0]
	subs r0, #2
	strb r1, [r0]
	subs r0, #8
	strb r2, [r0]
_080129B8:
	ldr r0, [r4, #8]
	cmp r0, #7
	beq _080129CE
	cmp r0, #9
	beq _080129CE
	cmp r0, #0xb
	beq _080129CE
	cmp r0, #0xe
	beq _080129CE
	cmp r0, #0x1a
	bne _08012A72
_080129CE:
	ldr r0, _080129F8 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #2
	bhi _080129FC
	movs r1, #0
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
	b _08012A5A
	.align 2, 0
_080129F8: .4byte gUnknown_03001304
_080129FC:
	adds r3, r4, #0
	adds r3, #0x27
	ldrb r1, [r3]
	adds r0, r1, #0
	subs r0, #0x1b
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #1
	bhi _08012A1C
	movs r1, #0x1c
	adds r2, r4, #0
	adds r2, #0x31
	movs r0, #1
	strb r0, [r2]
	subs r2, #2
	b _08012A56
_08012A1C:
	ldr r0, [r4, #8]
	cmp r0, #0xe
	bne _08012A26
	movs r0, #7
	b _08012A36
_08012A26:
	ldr r2, [r4, #0x18]
	cmp r2, #0
	beq _08012A48
	lsls r0, r1, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #0xd
	beq _08012A5A
	movs r0, #0xd
_08012A36:
	adds r2, r4, #0
	adds r2, #0x31
	movs r1, #0
	strb r1, [r2]
	subs r2, #2
	movs r1, #1
	strb r1, [r2]
	strb r0, [r3]
	b _08012A5A
_08012A48:
	movs r1, #7
	adds r0, r4, #0
	adds r0, #0x31
	strb r2, [r0]
	adds r2, r4, #0
	adds r2, #0x2f
	movs r0, #1
_08012A56:
	strb r0, [r2]
	strb r1, [r3]
_08012A5A:
	ldr r0, _08012A78 @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r1, #0x80
	lsls r1, r1, #1
	adds r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0
	beq _08012A72
	adds r1, r4, #0
	adds r1, #0x31
	movs r0, #1
	strb r0, [r1]
_08012A72:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08012A78: .4byte gUnknown_030012D8

	thumb_func_start sub_8012A7C
sub_8012A7C: @ 0x08012A7C
	push {r4, lr}
	adds r4, r0, #0
	ldr r2, [r4, #0x10]
	adds r1, r2, #0
	adds r1, #0x68
	movs r0, #8
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _08012AEA
	adds r0, r2, #0
	adds r0, #0x69
	ldrb r0, [r0]
	cmp r0, #2
	bls _08012AC0
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
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
	b _08012AD0
_08012AC0:
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x1c
	bl sub_803AD80
_08012AD0:
	movs r2, #4
	adds r1, r4, #0
	adds r1, #0x32
	movs r0, #0
	strb r0, [r1]
	subs r1, #2
	movs r0, #1
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x28
	strb r2, [r0]
	movs r0, #1
	b _08012AEC
_08012AEA:
	movs r0, #0
_08012AEC:
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8012AF4
sub_8012AF4: @ 0x08012AF4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0xc
	adds r4, r0, #0
	ldr r0, _08012B30 @ =gUnknown_030012D8
	ldr r2, [r0]
	movs r3, #0x81
	lsls r3, r3, #1
	adds r1, r2, r3
	ldrb r1, [r1]
	adds r3, r0, #0
	cmp r1, #0
	bne _08012B1A
	ldr r5, _08012B34 @ =0x00000103
	adds r0, r2, r5
	ldrb r0, [r0]
	cmp r0, #0
	beq _08012B5A
_08012B1A:
	ldr r0, [r4, #0x10]
	adds r0, #0x68
	ldrb r0, [r0]
	cmp r0, #8
	bne _08012B5A
	cmp r1, #0
	beq _08012B3C
	ldr r0, [r2]
	ldr r6, _08012B38 @ =0xFFFFFF00
	adds r0, r0, r6
	b _08012B4E
	.align 2, 0
_08012B30: .4byte gUnknown_030012D8
_08012B34: .4byte 0x00000103
_08012B38: .4byte 0xFFFFFF00
_08012B3C:
	ldr r1, _08012C88 @ =0x00000103
	adds r0, r2, r1
	ldrb r0, [r0]
	cmp r0, #0
	beq _08012B50
	ldr r0, [r2]
	movs r5, #0x80
	lsls r5, r5, #1
	adds r0, r0, r5
_08012B4E:
	str r0, [r2]
_08012B50:
	ldr r0, [r3]
	ldr r1, [r0]
	ldr r2, [r0, #4]
	bl sub_8009EA8
_08012B5A:
	ldr r0, [r4, #8]
	cmp r0, #0
	bne _08012BA0
	ldr r5, _08012C8C @ =gUnknown_030012D8
	ldr r1, [r5]
	ldr r0, [r1, #0x60]
	cmp r0, #0
	bne _08012B9A
	ldr r0, [r1, #0x20]
	ldrh r0, [r0, #0xa]
	cmp r0, #0x12
	beq _08012B9A
	adds r0, r4, #0
	adds r0, #0x33
	ldrb r0, [r0]
	cmp r0, #0
	bne _08012B9A
	ldr r0, _08012C90 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x36
	bl sub_80019A8
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r6, #0
	ldrsh r0, [r2, r6]
	adds r0, r4, r0
	ldr r1, [r5]
	ldr r3, [r2, #4]
	movs r2, #0x12
	bl sub_803AD84
_08012B9A:
	ldr r0, [r4, #8]
	cmp r0, #0
	beq _08012BA8
_08012BA0:
	adds r7, r4, #0
	adds r7, #0x2f
	cmp r0, #0x11
	bne _08012BD2
_08012BA8:
	ldr r0, _08012C8C @ =gUnknown_030012D8
	ldr r1, [r0]
	ldr r0, [r1, #0x60]
	adds r7, r4, #0
	adds r7, #0x2f
	cmp r0, #0
	beq _08012BD2
	movs r2, #0x80
	lsls r2, r2, #1
	adds r0, r1, r2
	ldrb r1, [r0]
	cmp r1, #0
	bne _08012BD2
	adds r0, r4, #0
	adds r0, #0x31
	strb r1, [r0]
	movs r0, #1
	strb r0, [r7]
	adds r0, r4, #0
	adds r0, #0x27
	strb r1, [r0]
_08012BD2:
	ldrb r3, [r7]
	mov ip, r3
	cmp r3, #1
	bne _08012CAE
	ldr r0, [r4, #4]
	adds r3, r4, #0
	adds r3, #0x27
	ldr r1, [r0]
	ldrb r5, [r3]
	lsls r0, r5, #3
	adds r0, r0, r1
	ldr r1, [r0]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r2, _08012C94 @ =gStaticData_0816B304
	mov r1, sp
	adds r0, r0, r2
	ldm r0!, {r2, r5, r6}
	stm r1!, {r2, r5, r6}
	ldr r0, _08012C8C @ =gUnknown_030012D8
	ldr r1, [r0]
	movs r6, #0x80
	lsls r6, r6, #1
	adds r0, r1, r6
	ldrb r0, [r0]
	adds r5, r4, #0
	adds r5, #0x31
	mov r8, r3
	cmp r0, #0
	beq _08012C3A
	ldr r0, [r4, #0x10]
	adds r0, #0x68
	ldrb r0, [r0]
	cmp r0, #8
	bne _08012C3A
	ldr r0, [r1, #0x60]
	cmp r0, #0
	beq _08012C3A
	mov r0, ip
	strb r0, [r5]
	ldr r0, [sp, #8]
	movs r1, #0xc0
	lsls r1, r1, #1
	bl sub_80008FC
	str r0, [sp, #8]
	ldr r0, [sp, #4]
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	str r0, [sp, #4]
_08012C3A:
	mov r1, r8
	ldrb r1, [r1]
	cmp r1, #0x1e
	bne _08012C6E
	movs r0, #0
	strb r0, [r5]
	ldr r0, _08012C8C @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	adds r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #0
	beq _08012C6E
	ldr r0, [sp, #4]
	movs r1, #0x80
	lsls r1, r1, #2
	bl sub_80008FC
	str r0, [sp, #4]
	ldr r0, [sp]
	movs r1, #0xc0
	lsls r1, r1, #1
	bl sub_80008FC
	str r0, [sp]
_08012C6E:
	ldrb r0, [r5]
	cmp r0, #0
	beq _08012C98
	ldr r2, [r4, #0xc]
	movs r3, #0x38
	ldrsh r0, [r2, r3]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #0x3c]
	mov r2, sp
	bl sub_803AD84
	b _08012CAA
	.align 2, 0
_08012C88: .4byte 0x00000103
_08012C8C: .4byte gUnknown_030012D8
_08012C90: .4byte gUnknown_030012BC
_08012C94: .4byte gStaticData_0816B304
_08012C98:
	ldr r2, [r4, #0xc]
	movs r5, #0x28
	ldrsh r0, [r2, r5]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #0x2c]
	mov r2, sp
	bl sub_803AD84
_08012CAA:
	movs r0, #0
	strb r0, [r7]
_08012CAE:
	adds r0, r4, #0
	adds r0, #0x30
	adds r5, r0, #0
	ldrb r6, [r5]
	cmp r6, #1
	bne _08012D16
	ldr r0, [r4, #4]
	adds r1, r4, #0
	adds r1, #0x28
	ldr r2, [r0]
	ldrb r1, [r1]
	lsls r0, r1, #3
	adds r0, r0, r2
	ldr r1, [r0, #4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r2, _08012CFC @ =gStaticData_0816B304
	mov r1, sp
	adds r0, r0, r2
	ldm r0!, {r2, r3, r6}
	stm r1!, {r2, r3, r6}
	adds r0, r4, #0
	adds r0, #0x32
	ldrb r0, [r0]
	cmp r0, #0
	beq _08012D00
	ldr r2, [r4, #0xc]
	adds r2, #0x40
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	mov r2, sp
	bl sub_803AD84
	b _08012D12
	.align 2, 0
_08012CFC: .4byte gStaticData_0816B304
_08012D00:
	ldr r2, [r4, #0xc]
	movs r3, #0x30
	ldrsh r0, [r2, r3]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #0x34]
	mov r2, sp
	bl sub_803AD84
_08012D12:
	movs r0, #0
	strb r0, [r5]
_08012D16:
	add sp, #0xc
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8012D24
sub_8012D24: @ 0x08012D24
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _08012DB4 @ =gUnknown_03001304
	ldr r0, [r0]
	ldr r1, _08012DB8 @ =gUnknown_030007E0
	ldr r1, [r1]
	str r1, [sp]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r6, r0, #0x18
	adds r1, r4, #0
	adds r1, #0x25
	ldrb r0, [r1]
	cmp r0, #0
	beq _08012D54
	subs r0, #1
	strb r0, [r1]
	cmp r6, #0
	bne _08012D54
	strb r6, [r1]
_08012D54:
	ldr r2, [r4, #0x10]
	adds r0, r2, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08012D7C
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r2, #0
	movs r2, #0x12
	bl sub_803AD84
	adds r1, r4, #0
	adds r1, #0x33
	movs r0, #0
	strb r0, [r1]
_08012D7C:
	ldr r1, [r4, #0x1c]
	adds r3, r1, #1
	str r3, [r4, #0x1c]
	ldr r2, [r4, #0x10]
	adds r0, r2, #0
	adds r0, #0x2d
	ldrb r0, [r0]
	cmp r0, #0x12
	bne _08012E0E
	ldr r5, [r2, #0x30]
	cmp r5, #0
	bne _08012E0E
	movs r0, #0xe1
	lsls r0, r0, #3
	cmp r3, r0
	ble _08012DBC
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r2, #0
	movs r2, #0x1a
	bl sub_803AD84
	str r5, [r4, #0x1c]
	b _08012E06
	.align 2, 0
_08012DB4: .4byte gUnknown_03001304
_08012DB8: .4byte gUnknown_030007E0
_08012DBC:
	ldr r3, _08012DDC @ =0xFFFFFB64
	adds r0, r1, r3
	cmp r0, #0x26
	bhi _08012DE4
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r2, #0
	movs r2, #5
	bl sub_803AD84
	ldr r0, _08012DE0 @ =0x000004C4
	b _08012E04
	.align 2, 0
_08012DDC: .4byte 0xFFFFFB64
_08012DE0: .4byte 0x000004C4
_08012DE4:
	ldr r3, _08012E78 @ =0xFFFFFE20
	adds r0, r1, r3
	cmp r0, #0x26
	bhi _08012E0E
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r2, #0
	movs r2, #0xe
	bl sub_803AD84
	movs r0, #0x82
	lsls r0, r0, #2
_08012E04:
	str r0, [r4, #0x1c]
_08012E06:
	adds r1, r4, #0
	adds r1, #0x33
	movs r0, #1
	strb r0, [r1]
_08012E0E:
	adds r0, r4, #0
	bl sub_8012A7C
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	cmp r5, #0
	beq _08012E1E
	b _08012FB0
_08012E1E:
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #1
	mov r8, r0
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08012E80
	ldr r0, _08012E7C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xd
	bl PlaySfx
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #5
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r3, #0
	ldrsh r0, [r2, r3]
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
	mov r2, r8
	strb r2, [r0]
	subs r0, #8
	strb r1, [r0]
	b _08012ECC
	.align 2, 0
_08012E78: .4byte 0xFFFFFE20
_08012E7C: .4byte gUnknown_030012BC
_08012E80:
	movs r0, #2
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r5, r0, #0x10
	cmp r5, #0
	beq _08012E94
	adds r0, r4, #0
	bl sub_8015398
	b _08012ECC
_08012E94:
	mov r0, sp
	ldrh r1, [r0]
	movs r0, #0x80
	lsls r0, r0, #1
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	cmp r0, #0
	beq _08012ED4
	ldr r1, [r4, #0xc]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x10
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #3
	bl sub_803AD84
	str r5, [r4, #0x1c]
_08012ECC:
	adds r0, r4, #0
	bl sub_80122CC
	b _08012FB0
_08012ED4:
	adds r7, r4, #0
	adds r7, #0x29
	strb r0, [r7]
	cmp r6, #0
	bne _08012F0E
	ldr r1, [r4, #0x10]
	movs r2, #0x80
	lsls r2, r2, #1
	adds r0, r1, r2
	ldrb r0, [r0]
	cmp r0, #0
	beq _08012FAA
	adds r2, r4, #0
	adds r2, #0x27
	ldrb r3, [r2]
	cmp r3, #0x1f
	beq _08012FAA
	ldr r0, [r1, #0x60]
	cmp r0, #0
	beq _08012FAA
	movs r0, #0x1f
	adds r1, r4, #0
	adds r1, #0x31
	mov r3, r8
	strb r3, [r1]
	subs r1, #2
	strb r3, [r1]
	strb r0, [r2]
	b _08012FAA
_08012F0E:
	adds r0, r4, #0
	adds r0, #0x25
	ldrb r5, [r0]
	cmp r5, #0
	bne _08012FAA
	cmp r6, #2
	beq _08012F84
	cmp r6, #2
	blt _08012FAA
	cmp r6, #8
	bgt _08012FAA
	movs r0, #0x80
	lsls r0, r0, #2
	ands r0, r1
	cmp r0, #0
	beq _08012F7C
	ldr r0, _08012F78 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231C4
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08012F7C
	mov r0, r8
	strb r0, [r7]
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #4
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r3, #0
	ldrsh r0, [r2, r3]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x18
	bl sub_803AD84
	movs r0, #0x1b
	adds r1, r4, #0
	adds r1, #0x31
	strb r5, [r1]
	subs r1, #2
	mov r2, r8
	strb r2, [r1]
	subs r1, #8
	strb r0, [r1]
	b _08012FAA
	.align 2, 0
_08012F78: .4byte gUnknown_030012C0
_08012F7C:
	adds r0, r4, #0
	bl sub_8015460
	b _08012FAA
_08012F84:
	ldr r1, [r4, #0xc]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x10
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #3
	bl sub_803AD84
	str r5, [r4, #0x1c]
_08012FAA:
	adds r0, r4, #0
	bl sub_80122CC
_08012FB0:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8012FBC
sub_8012FBC: @ 0x08012FBC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x10
	adds r6, r0, #0
	ldr r0, _08013034 @ =gUnknown_03001304
	mov r8, r0
	ldr r0, _08013038 @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp, #0xc]
	adds r0, r6, #0
	bl sub_8012A7C
	lsls r0, r0, #0x18
	lsrs r4, r0, #0x18
	cmp r4, #0
	beq _08012FE0
	b _0801321A
_08012FE0:
	add r0, sp, #0xc
	ldrh r1, [r0, #2]
	movs r7, #1
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08013040
	ldr r0, _0801303C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xd
	bl PlaySfx
	ldr r1, [r6, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x24]
	movs r1, #5
	bl sub_803AD80
	ldr r2, [r6, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r6, r0
	ldr r1, [r6, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x13
	bl sub_803AD84
	str r4, [r6, #0x18]
	movs r1, #7
	adds r0, r6, #0
	adds r0, #0x32
	strb r4, [r0]
	subs r0, #2
	strb r7, [r0]
	subs r0, #8
	strb r1, [r0]
	b _0801321A
	.align 2, 0
_08013034: .4byte gUnknown_03001304
_08013038: .4byte gUnknown_030007E0
_0801303C: .4byte gUnknown_030012BC
_08013040:
	movs r0, #2
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r5, r0, #0x10
	cmp r5, #0
	beq _08013054
	adds r0, r6, #0
	bl sub_8015398
	b _0801321A
_08013054:
	movs r2, #0x80
	lsls r2, r2, #1
	adds r0, r2, #0
	ands r0, r1
	cmp r0, #0
	beq _080130E4
	ldr r0, _08013100 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x1a
	bl PlaySfx
	movs r4, #0x10
	ldr r1, [r6, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xc
	bl sub_803AD80
	ldr r2, [r6, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r6, r0
	ldr r1, [r6, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0xf
	bl sub_803AD84
	str r5, [r6, #0x18]
	str r4, [r6, #0x1c]
	movs r0, #0x1e
	adds r1, r6, #0
	adds r1, #0x31
	strb r5, [r1]
	subs r1, #2
	movs r4, #1
	strb r7, [r1]
	subs r1, #8
	strb r0, [r1]
	ldr r2, _08013104 @ =gUnknown_030012D8
	ldr r0, [r2]
	adds r0, #0x94
	strb r5, [r0]
	ldr r0, [r2]
	adds r0, #0x94
	strb r5, [r0]
	ldr r0, _08013108 @ =gUnknown_030012E4
	ldr r0, [r0]
	movs r1, #0xa
	str r1, [sp]
	str r5, [sp, #4]
	ldr r1, [r2]
	str r1, [sp, #8]
	movs r1, #0x29
	movs r2, #1
	movs r3, #0
	bl sub_8025B0C
	movs r1, #5
	rsbs r1, r1, #0
	ldrb r2, [r0, #0xc]
	ands r1, r2
	strb r1, [r0, #0xc]
	adds r0, #0x28
	movs r1, #4
	rsbs r1, r1, #0
	ldrb r2, [r0]
	ands r1, r2
	orrs r1, r4
	strb r1, [r0]
_080130E4:
	mov r1, r8
	ldr r0, [r1]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r4, r0, #0x18
	cmp r4, #2
	beq _0801314E
	cmp r4, #2
	bgt _0801310C
	cmp r4, #0
	beq _08013116
	b _0801318C
	.align 2, 0
_08013100: .4byte gUnknown_030012BC
_08013104: .4byte gUnknown_030012D8
_08013108: .4byte gUnknown_030012E4
_0801310C:
	cmp r4, #8
	bgt _0801318C
	cmp r4, #7
	blt _0801318C
	b _0801314E
_08013116:
	str r4, [sp]
	adds r0, r6, #0
	movs r1, #0
	movs r2, #0x12
	movs r3, #0
	bl sub_8015780
	adds r3, r6, #0
	adds r3, #0x31
	strb r4, [r3]
	adds r2, r6, #0
	adds r2, #0x2f
	strb r7, [r2]
	adds r1, r6, #0
	adds r1, #0x27
	strb r4, [r1]
	adds r0, r6, #0
	adds r0, #0x32
	strb r4, [r0]
	subs r0, #2
	strb r7, [r0]
	subs r0, #8
	strb r4, [r0]
	movs r0, #0x1d
	strb r4, [r3]
	strb r7, [r2]
	strb r0, [r1]
	b _0801318C
_0801314E:
	movs r4, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x10
	bl sub_803AD80
	ldr r2, [r6, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r6, r0
	ldr r1, [r6, #0x10]
	ldr r3, [r2, #4]
	movs r2, #3
	bl sub_803AD84
	str r4, [r6, #0x1c]
	movs r1, #0x1d
	adds r0, r6, #0
	adds r0, #0x31
	strb r4, [r0]
	adds r2, r6, #0
	adds r2, #0x2f
	movs r0, #1
	strb r0, [r2]
	adds r0, r6, #0
	adds r0, #0x27
	strb r1, [r0]
_0801318C:
	add r1, sp, #0xc
	movs r0, #0x80
	lsls r0, r0, #2
	ldrh r1, [r1]
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r1, r0, #0x10
	cmp r1, #0
	beq _080131F8
	ldr r0, [r6, #8]
	cmp r0, #3
	bne _08013214
	ldr r0, _080131F4 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231C4
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08013214
	adds r0, r6, #0
	adds r0, #0x29
	movs r5, #0
	movs r4, #1
	strb r4, [r0]
	ldr r1, [r6, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x24]
	movs r1, #4
	bl sub_803AD80
	ldr r2, [r6, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r6, r0
	ldr r1, [r6, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x18
	bl sub_803AD84
	movs r0, #0x1b
	adds r1, r6, #0
	adds r1, #0x31
	strb r5, [r1]
	subs r1, #2
	strb r4, [r1]
	subs r1, #8
	strb r0, [r1]
	b _08013214
	.align 2, 0
_080131F4: .4byte gUnknown_030012C0
_080131F8:
	ldr r0, [r6, #8]
	cmp r0, #4
	bne _0801320C
	adds r0, r6, #0
	adds r0, #0x29
	strb r1, [r0]
	adds r0, r6, #0
	bl sub_8015460
	b _08013214
_0801320C:
	ldr r0, [r6, #0x18]
	cmp r0, #0
	beq _08013214
	str r1, [r6, #0x18]
_08013214:
	adds r0, r6, #0
	bl sub_80122CC
_0801321A:
	add sp, #0x10
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8013228
sub_8013228: @ 0x08013228
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	ldr r1, [r5, #0x10]
	movs r0, #2
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xd]
	ands r0, r2
	strb r0, [r1, #0xd]
	ldr r1, [r5, #0x10]
	movs r0, #3
	rsbs r0, r0, #0
	ldrb r6, [r1, #0xd]
	ands r0, r6
	strb r0, [r1, #0xd]
	ldr r1, [r5, #0x10]
	adds r1, #0x68
	movs r0, #4
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _080132A8
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x1a
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r6, #0
	ldrsh r0, [r2, r6]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x15
	bl sub_803AD84
	ldr r3, [r5, #0x10]
	movs r4, #2
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
	blt _08013296
	subs r4, r0, #1
_08013296:
	str r4, [r3, #0x30]
	adds r0, r3, #0
	bl sub_800B334
	ldr r0, [r5, #0x10]
	movs r1, #0
	adds r0, #0x68
	strb r1, [r0]
	b _080134AA
_080132A8:
	ldr r1, _08013320 @ =gUnknown_030007E0
	ldr r0, [r1]
	str r0, [sp]
	adds r0, r5, #0
	adds r0, #0x26
	ldrb r6, [r0]
	adds r2, r1, #0
	cmp r6, #0
	bne _0801332C
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _0801332C
	ldr r0, _08013324 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xa
	bl PlaySfx
	movs r4, #0x18
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xe
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x10
	bl sub_803AD84
	str r6, [r5, #0x18]
	str r4, [r5, #0x1c]
	adds r0, r5, #0
	adds r0, #0x21
	strb r6, [r0]
	subs r0, #1
	strb r6, [r0]
	adds r0, #2
	strb r6, [r0]
	adds r0, #1
	strb r6, [r0]
	adds r0, #1
	strb r6, [r0]
	ldr r0, _08013328 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x92
	strb r6, [r0]
	b _080134AA
	.align 2, 0
_08013320: .4byte gUnknown_030007E0
_08013324: .4byte gUnknown_030012BC
_08013328: .4byte gUnknown_030012D8
_0801332C:
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08013402
	ldr r4, [r2]
	movs r0, #1
	ands r0, r4
	cmp r0, #0
	beq _080133A8
	movs r0, #0x30
	ands r0, r4
	cmp r0, #0
	beq _080133A8
	adds r0, r1, #0
	adds r0, #0x2d
	ldrb r0, [r0]
	cmp r0, #6
	bne _08013366
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #9
	bl sub_803AD80
	b _0801338A
_08013366:
	ldr r1, [r5, #0xc]
	movs r6, #0x20
	ldrsh r0, [r1, r6]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #9
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #6
	bl sub_803AD84
_0801338A:
	adds r3, r5, #0
	adds r3, #0x28
	ldrb r2, [r3]
	cmp r2, #7
	bne _08013402
	movs r2, #0xa
	adds r1, r5, #0
	adds r1, #0x32
	movs r0, #0
	strb r0, [r1]
	subs r1, #2
	movs r0, #1
	strb r0, [r1]
	strb r2, [r3]
	b _08013402
_080133A8:
	ldr r1, [r5, #0xc]
	movs r6, #0x20
	ldrsh r0, [r1, r6]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #7
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0xc
	bl sub_803AD84
	adds r3, r5, #0
	adds r3, #0x28
	ldrb r2, [r3]
	cmp r2, #7
	bne _08013402
	movs r6, #1
	movs r2, #1
	ands r2, r4
	cmp r2, #0
	beq _080133F4
	movs r0, #9
	adds r2, r5, #0
	adds r2, #0x32
	movs r1, #0
	strb r1, [r2]
	adds r1, r5, #0
	adds r1, #0x30
	strb r6, [r1]
	strb r0, [r3]
	b _08013402
_080133F4:
	movs r1, #8
	adds r0, r5, #0
	adds r0, #0x32
	strb r2, [r0]
	subs r0, #2
	strb r6, [r0]
	strb r1, [r3]
_08013402:
	ldr r0, _08013438 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #2
	bhi _08013440
	ldr r0, _0801343C @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r6, #0x80
	lsls r6, r6, #1
	adds r0, r0, r6
	ldrb r1, [r0]
	cmp r1, #0
	bne _080134A4
	adds r0, r5, #0
	adds r0, #0x31
	strb r1, [r0]
	adds r2, r5, #0
	adds r2, #0x2f
	movs r0, #1
	strb r0, [r2]
	adds r0, r5, #0
	adds r0, #0x27
	strb r1, [r0]
	b _080134A4
	.align 2, 0
_08013438: .4byte gUnknown_03001304
_0801343C: .4byte gUnknown_030012D8
_08013440:
	adds r3, r5, #0
	adds r3, #0x27
	ldrb r1, [r3]
	adds r0, r1, #0
	subs r0, #0x1b
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #1
	bhi _08013460
	movs r1, #0x1c
	adds r2, r5, #0
	adds r2, #0x31
	movs r0, #1
	strb r0, [r2]
	subs r2, #2
	b _080134A0
_08013460:
	ldr r0, [r5, #0x18]
	cmp r0, #0
	beq _08013482
	lsls r0, r1, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #0xd
	beq _080134A4
	movs r0, #0xd
	adds r2, r5, #0
	adds r2, #0x31
	movs r1, #0
	strb r1, [r2]
	subs r2, #2
	movs r1, #1
	strb r1, [r2]
	strb r0, [r3]
	b _080134A4
_08013482:
	ldr r0, _080134B4 @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r1, #0x80
	lsls r1, r1, #1
	adds r0, r0, r1
	ldrb r2, [r0]
	cmp r2, #0
	bne _080134A4
	movs r1, #7
	adds r0, r5, #0
	adds r0, #0x31
	strb r2, [r0]
	adds r2, r5, #0
	adds r2, #0x2f
	movs r0, #1
_080134A0:
	strb r0, [r2]
	strb r1, [r3]
_080134A4:
	adds r0, r5, #0
	bl sub_80122CC
_080134AA:
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_080134B4: .4byte gUnknown_030012D8

	thumb_func_start sub_80134B8
sub_80134B8: @ 0x080134B8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xc
	adds r5, r0, #0
	ldr r0, _08013580 @ =gUnknown_03001304
	ldr r0, [r0]
	ldr r1, _08013584 @ =gUnknown_030007E0
	ldr r1, [r1]
	str r1, [sp, #8]
	ldr r1, [r5, #0x10]
	adds r1, #0x68
	ldrb r7, [r1]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	mov r8, r0
	ldr r2, [r5, #8]
	cmp r2, #0xe
	beq _0801355A
	adds r0, r5, #0
	adds r0, #0x26
	ldrb r6, [r0]
	cmp r6, #0
	bne _0801355A
	add r0, sp, #8
	ldrh r1, [r0, #2]
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _0801355A
	adds r0, r2, #0
	subs r0, #0x18
	cmp r0, #1
	bls _0801355A
	ldr r0, _08013588 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xa
	bl PlaySfx
	movs r4, #0x18
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xe
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x10
	bl sub_803AD84
	str r6, [r5, #0x18]
	str r4, [r5, #0x1c]
	adds r0, r5, #0
	adds r0, #0x21
	strb r6, [r0]
	subs r0, #1
	strb r6, [r0]
	adds r0, #2
	strb r6, [r0]
	adds r0, #1
	strb r6, [r0]
	adds r0, #1
	strb r6, [r0]
	ldr r0, _0801358C @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x92
	strb r6, [r0]
_0801355A:
	cmp r7, #0
	bne _080135BE
	adds r0, r5, #0
	bl sub_80122CC
	adds r1, r5, #0
	adds r1, #0x25
	ldrb r0, [r1]
	cmp r0, #0
	beq _08013590
	subs r0, #1
	strb r0, [r1]
	mov r2, r8
	cmp r2, #0
	beq _0801357A
	b _080138D6
_0801357A:
	strb r7, [r1]
	b _080138D6
	.align 2, 0
_08013580: .4byte gUnknown_03001304
_08013584: .4byte gUnknown_030007E0
_08013588: .4byte gUnknown_030012BC
_0801358C: .4byte gUnknown_030012D8
_08013590:
	adds r0, r5, #0
	bl sub_801283C
	ldr r0, [r5, #8]
	cmp r0, #0x1a
	beq _0801359E
	b _080138D6
_0801359E:
	adds r3, r5, #0
	adds r3, #0x28
	ldrb r0, [r3]
	cmp r0, #0
	beq _080135AA
	b _080138D6
_080135AA:
	movs r2, #4
	adds r0, r5, #0
	adds r0, #0x32
	strb r7, [r0]
	adds r1, r5, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	strb r2, [r3]
	b _080138D6
_080135BE:
	movs r0, #4
	ands r0, r7
	lsls r0, r0, #0x18
	lsrs r6, r0, #0x18
	cmp r6, #0
	beq _08013648
	ldr r0, [r5, #8]
	cmp r0, #0x1a
	beq _08013632
	ldr r1, [r5, #0x10]
	movs r0, #1
	ldrb r6, [r1, #0xd]
	orrs r0, r6
	strb r0, [r1, #0xd]
	adds r1, r5, #0
	adds r1, #0x34
	movs r0, #0
	strb r0, [r1]
	ldr r0, [r5, #8]
	cmp r0, #0xe
	beq _0801362C
	ldr r1, [r5, #0xc]
	movs r7, #0x20
	ldrsh r0, [r1, r7]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x1a
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x15
	bl sub_803AD84
	ldr r3, [r5, #0x10]
	movs r4, #2
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
	blt _0801362A
	subs r4, r0, #1
_0801362A:
	str r4, [r3, #0x30]
_0801362C:
	ldr r0, [r5, #0x10]
	bl sub_800B334
_08013632:
	adds r0, r5, #0
	bl sub_80122CC
	adds r0, r5, #0
	bl sub_801283C
	ldr r0, [r5, #0x10]
	movs r1, #0
	adds r0, #0x68
	strb r1, [r0]
	b _080138D6
_08013648:
	subs r0, r7, #1
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #1
	bhi _08013666
	adds r0, r5, #0
	bl sub_80122CC
	adds r0, r5, #0
	bl sub_801283C
	ldr r0, [r5, #0x10]
	adds r0, #0x68
	strb r6, [r0]
	b _080138D6
_08013666:
	movs r0, #8
	ands r7, r0
	cmp r7, #0
	bne _08013670
	b _080138C0
_08013670:
	ldr r1, [r5, #0x10]
	ldr r0, [r1, #0x64]
	cmp r0, #0
	bge _0801367A
	b _080138C0
_0801367A:
	ldrb r0, [r1, #0xd]
	movs r7, #1
	orrs r0, r7
	strb r0, [r1, #0xd]
	adds r0, r5, #0
	adds r0, #0x34
	strb r6, [r0]
	ldr r1, [r5, #8]
	adds r0, r1, #0
	subs r0, #0x18
	cmp r0, #1
	bls _08013694
	b _080137C8
_08013694:
	ldr r1, _080137BC @ =gUnknown_030012D8
	ldr r0, [r1]
	ldr r3, [r0]
	asrs r3, r3, #8
	adds r3, #0x14
	ldr r1, [r0, #4]
	asrs r1, r1, #8
	adds r1, #0xc
	ldr r2, _080137C0 @ =gUnknown_030012E4
	ldr r0, [r2]
	str r1, [sp]
	str r7, [sp, #4]
	movs r1, #0x29
	movs r2, #1
	bl sub_8025BAC
	adds r3, r0, #0
	adds r2, r3, #0
	adds r2, #0x28
	movs r0, #4
	rsbs r0, r0, #0
	mov sl, r0
	ldrb r1, [r2]
	ands r0, r1
	orrs r0, r7
	strb r0, [r2]
	movs r0, #5
	rsbs r0, r0, #0
	mov sb, r0
	ldrb r1, [r3, #0xc]
	ands r0, r1
	strb r0, [r3, #0xc]
	subs r7, #0x12
	mov r8, r7
	mov r0, r8
	ldrb r1, [r2]
	ands r0, r1
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r2]
	movs r4, #3
	ldr r0, [r3, #0x20]
	adds r2, #5
	ldr r1, [r0]
	ldrb r7, [r2]
	lsls r0, r7, #3
	adds r2, r7, #0
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _08013700
	subs r4, r0, #1
_08013700:
	str r4, [r3, #0x30]
	ldr r1, _080137BC @ =gUnknown_030012D8
	ldr r0, [r1]
	ldr r3, [r0]
	asrs r3, r3, #8
	subs r3, #0x14
	ldr r1, [r0, #4]
	asrs r1, r1, #8
	adds r1, #0xc
	ldr r2, _080137C0 @ =gUnknown_030012E4
	ldr r0, [r2]
	str r1, [sp]
	str r6, [sp, #4]
	movs r1, #0x29
	movs r2, #1
	bl sub_8025BAC
	adds r3, r0, #0
	adds r1, r3, #0
	adds r1, #0x28
	mov r0, sl
	ldrb r7, [r1]
	ands r0, r7
	movs r2, #1
	orrs r0, r2
	strb r0, [r1]
	mov r0, sb
	ldrb r7, [r3, #0xc]
	ands r0, r7
	strb r0, [r3, #0xc]
	mov r0, r8
	ldrb r2, [r1]
	ands r0, r2
	strb r0, [r1]
	movs r4, #3
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
	blt _08013762
	subs r4, r0, #1
_08013762:
	str r4, [r3, #0x30]
	ldr r0, [r5, #8]
	cmp r0, #0x19
	bne _08013770
	adds r0, r5, #0
	bl sub_8014F8C
_08013770:
	ldr r0, [r5, #8]
	cmp r0, #0x1d
	bne _08013778
	b _080138D6
_08013778:
	ldr r0, _080137C4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x19
	bl PlaySfx
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x16
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r7, #0
	ldrsh r0, [r2, r7]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x11
	bl sub_803AD84
	adds r0, r5, #0
	adds r0, #0x32
	strb r6, [r0]
	subs r0, #2
	movs r1, #1
	strb r1, [r0]
	subs r0, #8
	strb r6, [r0]
	b _080138D6
	.align 2, 0
_080137BC: .4byte gUnknown_030012D8
_080137C0: .4byte gUnknown_030012E4
_080137C4: .4byte gUnknown_030012BC
_080137C8:
	cmp r1, #0xe
	bne _08013840
	add r0, sp, #8
	movs r2, #0x30
	ldrh r0, [r0]
	ands r2, r0
	cmp r2, #0
	beq _080137EA
	movs r0, #1
	adds r1, r5, #0
	adds r1, #0x31
	strb r6, [r1]
	subs r1, #2
	strb r0, [r1]
	subs r1, #8
	strb r0, [r1]
	b _080137FE
_080137EA:
	adds r0, r5, #0
	adds r0, #0x31
	strb r2, [r0]
	adds r1, r5, #0
	adds r1, #0x2f
	movs r0, #1
	strb r0, [r1]
	adds r0, r5, #0
	adds r0, #0x27
	strb r2, [r0]
_080137FE:
	movs r2, #0
	adds r0, r5, #0
	adds r0, #0x32
	strb r2, [r0]
	adds r1, r5, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	adds r0, r5, #0
	adds r0, #0x28
	strb r2, [r0]
	subs r0, #6
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801382E
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xf
	bl sub_803AD80
	b _080138D6
_0801382E:
	ldr r1, [r5, #0xc]
	movs r6, #0x20
	ldrsh r0, [r1, r6]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xd
	bl sub_803AD80
	b _080138D6
_08013840:
	ldr r0, _08013878 @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r7, #0x80
	lsls r7, r7, #1
	adds r0, r0, r7
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801387C
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x17
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r6, #0
	ldrsh r0, [r2, r6]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x16
	bl sub_803AD84
	b _080138A0
	.align 2, 0
_08013878: .4byte gUnknown_030012D8
_0801387C:
	ldr r1, [r5, #0xc]
	movs r7, #0x20
	ldrsh r0, [r1, r7]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x17
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x16
	bl sub_803AD84
_080138A0:
	movs r1, #0
	adds r0, r5, #0
	adds r0, #0x31
	strb r1, [r0]
	subs r0, #2
	movs r2, #1
	strb r2, [r0]
	subs r0, #8
	strb r1, [r0]
	adds r0, #0xb
	strb r1, [r0]
	subs r0, #2
	strb r2, [r0]
	subs r0, #8
	strb r1, [r0]
	b _080138D6
_080138C0:
	movs r0, #0
	adds r1, r5, #0
	adds r1, #0x31
	strb r0, [r1]
	adds r2, r5, #0
	adds r2, #0x2f
	movs r1, #1
	strb r1, [r2]
	adds r1, r5, #0
	adds r1, #0x27
	strb r0, [r1]
_080138D6:
	add sp, #0xc
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80138E8
sub_80138E8: @ 0x080138E8
	push {r4, lr}
	adds r4, r0, #0
	ldr r2, [r4, #0x10]
	adds r0, r2, #0
	adds r0, #0x2d
	ldrb r0, [r0]
	cmp r0, #6
	bne _08013938
	ldr r0, [r2, #0x30]
	cmp r0, #3
	bne _08013914
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r2, #0
	movs r2, #9
	bl sub_803AD84
	b _0801398C
_08013914:
	cmp r0, #3
	bgt _08013922
	adds r0, r2, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801398C
_08013922:
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r2, #0
	movs r2, #8
	bl sub_803AD84
	b _0801398C
_08013938:
	adds r0, r2, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801398C
	ldr r0, _08013978 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231BC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801397C
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x19
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r3, #0
	ldrsh r0, [r2, r3]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #7
	bl sub_803AD84
	b _0801398C
	.align 2, 0
_08013978: .4byte gUnknown_030012C0
_0801397C:
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x18
	bl sub_803AD80
_0801398C:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8013994
sub_8013994: @ 0x08013994
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	ldr r0, _080139C4 @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp]
	ldr r2, [r5, #0x10]
	adds r0, r2, #0
	adds r0, #0x68
	ldrb r1, [r0]
	cmp r1, #0
	bne _080139C8
	movs r2, #5
	adds r0, r5, #0
	adds r0, #0x32
	strb r1, [r0]
	adds r1, r5, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	adds r0, r5, #0
	adds r0, #0x28
	strb r2, [r0]
	b _08013A4A
	.align 2, 0
_080139C4: .4byte gUnknown_030007E0
_080139C8:
	mov r0, sp
	movs r6, #1
	movs r4, #1
	ldrh r0, [r0]
	ands r4, r0
	cmp r4, #0
	beq _08013A18
	adds r0, r2, #0
	movs r1, #0xb
	bl sub_800AAEC
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #1
	bne _08013A4A
	ldr r0, _08013A14 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xc
	bl PlaySfx
	ldr r1, [r5, #0x10]
	movs r0, #2
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xd]
	ands r0, r2
	strb r0, [r1, #0xd]
	ldr r1, [r5, #0x10]
	movs r0, #3
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xd]
	ands r0, r2
	strb r0, [r1, #0xd]
	adds r0, r5, #0
	bl sub_8015508
	b _08013C58
	.align 2, 0
_08013A14: .4byte gUnknown_030012BC
_08013A18:
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _08013A4A
	adds r0, r2, #0
	movs r1, #0x10
	bl sub_800AAEC
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #1
	bne _08013A4A
	adds r0, r5, #0
	bl sub_8015398
	adds r0, r5, #0
	adds r0, #0x31
	strb r4, [r0]
	subs r0, #2
	strb r6, [r0]
	subs r0, #8
	strb r6, [r0]
	b _08013C58
_08013A4A:
	ldr r0, [r5, #0x18]
	adds r0, #1
	str r0, [r5, #0x18]
	ldr r1, [r5, #0x1c]
	cmp r0, r1
	bge _08013A7C
	ldr r3, [r5, #0x10]
	movs r0, #0
	str r0, [r3, #0x34]
	movs r4, #3
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
	blt _08013A78
	subs r4, r0, #1
_08013A78:
	str r4, [r3, #0x30]
	b _08013C58
_08013A7C:
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	bne _08013A8A
	b _08013C58
_08013A8A:
	adds r0, r1, #0
	adds r0, #0x68
	ldrb r4, [r0]
	cmp r4, #0
	bne _08013AD0
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x1a
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x1b
	bl sub_803AD84
	movs r2, #4
	adds r0, r5, #0
	adds r0, #0x32
	strb r4, [r0]
	adds r1, r5, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	adds r0, r5, #0
	adds r0, #0x28
	strb r2, [r0]
	b _08013C58
_08013AD0:
	mov r1, sp
	movs r0, #0x80
	lsls r0, r0, #1
	ldrh r1, [r1]
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r6, r0, #0x10
	cmp r6, #0
	beq _08013B28
	movs r4, #0
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x14
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0
	bl sub_803AD84
	str r4, [r5, #0x1c]
	movs r1, #3
	adds r0, r5, #0
	adds r0, #0x31
	strb r4, [r0]
	adds r2, r5, #0
	adds r2, #0x2f
	movs r0, #1
	strb r0, [r2]
	adds r0, r5, #0
	adds r0, #0x27
	strb r1, [r0]
	adds r0, r5, #0
	bl sub_801434C
	b _08013C58
_08013B28:
	ldr r0, _08013BA8 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r4, r0, #0x18
	cmp r4, #0
	beq _08013BDA
	ldr r0, [r5, #0x10]
	movs r1, #2
	bl sub_800AAEC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08013BDA
	cmp r4, #4
	bgt _08013BB8
	cmp r4, #3
	blt _08013BB8
	mov r1, sp
	movs r0, #0x80
	lsls r0, r0, #2
	ldrh r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _08013BB0
	ldr r0, _08013BAC @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231C4
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08013BB0
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
	adds r0, r5, #0
	adds r0, #0x31
	strb r6, [r0]
	subs r0, #2
	strb r4, [r0]
	subs r0, #8
	b _08013C56
	.align 2, 0
_08013BA8: .4byte gUnknown_03001304
_08013BAC: .4byte gUnknown_030012C0
_08013BB0:
	adds r0, r5, #0
	bl sub_8015460
	b _08013C58
_08013BB8:
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x12
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #2
	b _08013C3E
_08013BDA:
	ldr r0, [r5, #0x10]
	movs r1, #2
	bl sub_800AAEC
	lsls r0, r0, #0x18
	lsrs r4, r0, #0x18
	cmp r4, #1
	bne _08013C1E
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x12
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #2
	bl sub_803AD84
	movs r1, #0
	adds r0, r5, #0
	adds r0, #0x31
	strb r1, [r0]
	subs r0, #2
	strb r4, [r0]
	subs r0, #8
	b _08013C56
_08013C1E:
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x11
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #4
_08013C3E:
	bl sub_803AD84
	movs r1, #0
	adds r0, r5, #0
	adds r0, #0x31
	strb r1, [r0]
	adds r2, r5, #0
	adds r2, #0x2f
	movs r0, #1
	strb r0, [r2]
	adds r0, r5, #0
	adds r0, #0x27
_08013C56:
	strb r1, [r0]
_08013C58:
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0

	thumb_func_start sub_8013C60
sub_8013C60: @ 0x08013C60
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _08013D04 @ =gUnknown_03001304
	ldr r0, [r0]
	ldr r1, _08013D08 @ =gUnknown_030007E0
	ldr r1, [r1]
	str r1, [sp]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	ldr r0, [r4, #0x10]
	adds r0, #0x68
	ldrb r1, [r0]
	cmp r1, #0
	bne _08013C98
	movs r2, #5
	adds r0, r4, #0
	adds r0, #0x32
	strb r1, [r0]
	adds r1, r4, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x28
	strb r2, [r0]
_08013C98:
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08013D0C
	ldr r2, [r4, #0x10]
	adds r1, r2, #0
	adds r1, #0x68
	movs r0, #8
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _08013D0C
	movs r0, #2
	rsbs r0, r0, #0
	ldrb r1, [r2, #0xd]
	ands r0, r1
	strb r0, [r2, #0xd]
	ldr r1, [r4, #0x10]
	movs r0, #3
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xd]
	ands r0, r2
	strb r0, [r1, #0xd]
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xe
	bl sub_803AD80
	movs r3, #7
	adds r0, r4, #0
	adds r0, #0x32
	movs r2, #0
	strb r2, [r0]
	adds r1, r4, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x28
	strb r3, [r0]
	subs r0, #6
	strb r2, [r0]
	adds r0, #1
	strb r2, [r0]
	ldr r0, [r4, #0x10]
	adds r0, #0x68
	strb r2, [r0]
	b _08013D8A
	.align 2, 0
_08013D04: .4byte gUnknown_03001304
_08013D08: .4byte gUnknown_030007E0
_08013D0C:
	ldr r0, _08013D7C @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231B4
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08013D46
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _08013D46
	adds r0, r4, #0
	adds r0, #0x26
	ldrb r0, [r0]
	cmp r0, #0
	bne _08013D46
	adds r1, r4, #0
	adds r1, #0x20
	ldrb r0, [r1]
	adds r0, #1
	strb r0, [r1]
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #3
	bls _08013D46
	movs r0, #3
	strb r0, [r1]
_08013D46:
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_80152F0
	ldr r0, [r4, #0x18]
	adds r0, #1
	str r0, [r4, #0x18]
	ldr r1, [r4, #0x1c]
	cmp r0, r1
	bge _08013D64
	ldr r0, [r4, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08013D8A
_08013D64:
	adds r0, r4, #0
	adds r0, #0x20
	ldrb r0, [r0]
	cmp r0, #0
	beq _08013D80
	adds r0, r4, #0
	movs r1, #0xf
	movs r2, #0xd
	bl sub_8015038
	b _08013D8A
	.align 2, 0
_08013D7C: .4byte gUnknown_030012C0
_08013D80:
	ldr r2, [sp]
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_8015238
_08013D8A:
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8013D94
sub_8013D94: @ 0x08013D94
	push {r4, r5, lr}
	sub sp, #4
	adds r5, r0, #0
	ldr r0, _08013DF4 @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp]
	ldr r2, [r5, #0x10]
	adds r1, r2, #0
	adds r1, #0x68
	movs r0, #8
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _08013DF8
	ldr r0, [r2, #0x64]
	cmp r0, #0
	ble _08013DF8
	movs r0, #1
	ldrb r1, [r2, #0xd]
	orrs r0, r1
	strb r0, [r2, #0xd]
	adds r0, r5, #0
	adds r0, #0x34
	movs r4, #0
	strb r4, [r0]
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xd
	bl sub_803AD80
	adds r0, r5, #0
	adds r0, #0x32
	strb r4, [r0]
	adds r1, r5, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	adds r0, r5, #0
	adds r0, #0x28
	strb r4, [r0]
	adds r0, r5, #0
	bl sub_8013C60
	b _08013EA2
	.align 2, 0
_08013DF4: .4byte gUnknown_030007E0
_08013DF8:
	ldr r0, _08013E68 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231B4
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08013E32
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _08013E32
	adds r0, r5, #0
	adds r0, #0x26
	ldrb r0, [r0]
	cmp r0, #0
	bne _08013E32
	adds r1, r5, #0
	adds r1, #0x20
	ldrb r0, [r1]
	adds r0, #1
	strb r0, [r1]
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #3
	bls _08013E32
	movs r0, #3
	strb r0, [r1]
_08013E32:
	ldr r0, [r5, #0x18]
	adds r0, #1
	str r0, [r5, #0x18]
	ldr r1, [r5, #0x1c]
	ldr r2, [r5, #0x10]
	cmp r0, r1
	bge _08013E4A
	adds r0, r2, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08013E9C
_08013E4A:
	movs r0, #1
	ldrb r1, [r2, #0xd]
	orrs r0, r1
	strb r0, [r2, #0xd]
	adds r0, r5, #0
	adds r0, #0x20
	ldrb r4, [r0]
	cmp r4, #0
	beq _08013E6C
	adds r0, r5, #0
	movs r1, #0xe
	movs r2, #0xe
	bl sub_8015038
	b _08013E9C
	.align 2, 0
_08013E68: .4byte gUnknown_030012C0
_08013E6C:
	adds r1, r5, #0
	adds r1, #0x26
	movs r0, #0xc
	strb r0, [r1]
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x1a
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x15
	bl sub_803AD84
	str r4, [r5, #0x18]
	str r4, [r5, #0x1c]
_08013E9C:
	adds r0, r5, #0
	bl sub_80134B8
_08013EA2:
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8013EAC
sub_8013EAC: @ 0x08013EAC
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _08013EE0 @ =gUnknown_03001304
	ldr r0, [r0]
	ldr r1, _08013EE4 @ =gUnknown_030007E0
	ldr r1, [r1]
	str r1, [sp]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	ldr r0, [r4, #0x10]
	adds r0, #0x68
	ldrb r0, [r0]
	cmp r0, #0
	bne _08013EFE
	adds r0, r4, #0
	adds r0, #0x22
	ldrb r1, [r0]
	cmp r1, #0
	beq _08013EE8
	adds r0, r4, #0
	bl sub_80151C8
	b _08013EFE
	.align 2, 0
_08013EE0: .4byte gUnknown_03001304
_08013EE4: .4byte gUnknown_030007E0
_08013EE8:
	movs r2, #5
	adds r0, r4, #0
	adds r0, #0x32
	strb r1, [r0]
	adds r1, r4, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x28
	strb r2, [r0]
_08013EFE:
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08013F64
	ldr r2, [r4, #0x10]
	adds r1, r2, #0
	adds r1, #0x68
	movs r0, #8
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _08013F64
	movs r0, #2
	rsbs r0, r0, #0
	ldrb r1, [r2, #0xd]
	ands r0, r1
	strb r0, [r2, #0xd]
	ldr r1, [r4, #0x10]
	movs r0, #3
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xd]
	ands r0, r2
	strb r0, [r1, #0xd]
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0xe
	bl sub_803AD80
	movs r3, #7
	adds r0, r4, #0
	adds r0, #0x32
	movs r2, #0
	strb r2, [r0]
	adds r1, r4, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x28
	strb r3, [r0]
	subs r0, #5
	strb r2, [r0]
	ldr r0, [r4, #0x10]
	adds r0, #0x68
	strb r2, [r0]
	b _08013FC6
_08013F64:
	ldr r0, _08013FD0 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231B4
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08013F9E
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _08013F9E
	adds r0, r4, #0
	adds r0, #0x26
	ldrb r0, [r0]
	cmp r0, #0
	bne _08013F9E
	adds r1, r4, #0
	adds r1, #0x20
	ldrb r0, [r1]
	adds r0, #1
	strb r0, [r1]
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #3
	bls _08013F9E
	movs r0, #3
	strb r0, [r1]
_08013F9E:
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_80152F0
	ldr r0, [r4, #0x18]
	adds r0, #1
	str r0, [r4, #0x18]
	ldr r1, [r4, #0x1c]
	cmp r0, r1
	bge _08013FBC
	ldr r0, [r4, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08013FC6
_08013FBC:
	adds r0, r4, #0
	movs r1, #0xf
	movs r2, #0xd
	bl sub_8015038
_08013FC6:
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08013FD0: .4byte gUnknown_030012C0

	thumb_func_start sub_8013FD4
sub_8013FD4: @ 0x08013FD4
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _0801401C @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp]
	mov r0, sp
	ldrh r0, [r0, #2]
	movs r2, #1
	ands r2, r0
	cmp r2, #0
	beq _08014024
	ldr r0, _08014020 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xc
	bl PlaySfx
	ldr r1, [r4, #0x10]
	movs r0, #2
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xd]
	ands r0, r2
	strb r0, [r1, #0xd]
	ldr r1, [r4, #0x10]
	movs r0, #3
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xd]
	ands r0, r2
	strb r0, [r1, #0xd]
	adds r0, r4, #0
	bl sub_8015508
	b _0801407C
	.align 2, 0
_0801401C: .4byte gUnknown_030007E0
_08014020: .4byte gUnknown_030012BC
_08014024:
	ldr r0, [r4, #0x10]
	adds r0, #0x68
	ldrb r0, [r0]
	cmp r0, #8
	bne _0801404E
	adds r3, r4, #0
	adds r3, #0x28
	ldrb r0, [r3]
	subs r0, #4
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #1
	bhi _0801404E
	adds r0, r4, #0
	adds r0, #0x32
	strb r2, [r0]
	adds r1, r4, #0
	adds r1, #0x30
	movs r0, #1
	strb r0, [r1]
	strb r2, [r3]
_0801404E:
	ldr r0, [r4, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801407C
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
_0801407C:
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8014084
sub_8014084: @ 0x08014084
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _080140E8 @ =gUnknown_03001304
	ldr r0, [r0]
	ldr r1, _080140EC @ =gUnknown_030007E0
	ldr r1, [r1]
	str r1, [sp]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _080140F4
	ldr r0, [r4, #0x10]
	movs r1, #0xb
	bl sub_800AAEC
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #1
	bne _080140F4
	ldr r0, _080140F0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xc
	bl PlaySfx
	ldr r1, [r4, #0x10]
	movs r0, #2
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xd]
	ands r0, r2
	strb r0, [r1, #0xd]
	ldr r1, [r4, #0x10]
	movs r0, #3
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xd]
	ands r0, r2
	strb r0, [r1, #0xd]
	adds r0, r4, #0
	bl sub_8015508
	b _08014264
	.align 2, 0
_080140E8: .4byte gUnknown_03001304
_080140EC: .4byte gUnknown_030007E0
_080140F0: .4byte gUnknown_030012BC
_080140F4:
	adds r0, r4, #0
	bl sub_8012A7C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08014102
	b _08014264
_08014102:
	movs r3, #0
	ldr r0, [r4, #0x10]
	adds r1, r0, #0
	adds r1, #0x28
	ldrb r1, [r1]
	lsls r1, r1, #0x1b
	adds r2, r0, #0
	cmp r1, #0
	bge _0801413A
	cmp r5, #4
	beq _08014120
	cmp r5, #6
	beq _08014120
	cmp r5, #8
	bne _0801413A
_08014120:
	adds r0, r2, #0
	adds r0, #0x28
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r2, [r0]
	ands r1, r2
	strb r1, [r0]
	adds r1, r4, #0
	adds r1, #0x2f
	movs r0, #1
	strb r0, [r1]
	movs r3, #1
	b _0801416A
_0801413A:
	adds r0, r2, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	blt _0801416A
	cmp r5, #3
	beq _08014152
	cmp r5, #5
	beq _08014152
	cmp r5, #7
	bne _0801416A
_08014152:
	movs r3, #1
	adds r2, #0x28
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r1, [r2]
	ands r0, r1
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r2]
	adds r0, r4, #0
	adds r0, #0x2f
	strb r3, [r0]
_0801416A:
	movs r6, #0
	cmp r3, #0
	bne _080141C8
	ldr r0, _0801421C @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #3
	blt _080141C8
	cmp r0, #4
	ble _0801418C
	cmp r0, #8
	bgt _080141C8
	cmp r0, #7
	blt _080141C8
_0801418C:
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x13
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x14
	bl sub_803AD84
	movs r1, #3
	adds r0, r4, #0
	adds r0, #0x31
	strb r6, [r0]
	adds r2, r4, #0
	adds r2, #0x2f
	movs r0, #1
	strb r0, [r2]
	adds r0, r4, #0
	adds r0, #0x27
	strb r1, [r0]
	movs r6, #1
_080141C8:
	mov r0, sp
	movs r5, #0xc0
	lsls r5, r5, #1
	ldrh r0, [r0]
	ands r5, r0
	cmp r5, #0
	bne _08014264
	ldr r0, [r4, #0x10]
	movs r1, #2
	bl sub_800AAEC
	lsls r0, r0, #0x18
	lsrs r7, r0, #0x18
	cmp r7, #1
	bne _08014220
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x12
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #2
	bl sub_803AD84
	adds r0, r4, #0
	adds r0, #0x31
	strb r5, [r0]
	subs r0, #2
	strb r7, [r0]
	subs r0, #8
	strb r5, [r0]
	b _08014264
	.align 2, 0
_0801421C: .4byte gUnknown_03001304
_08014220:
	cmp r6, #0
	bne _08014264
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
	adds r0, r4, #0
	adds r0, #0x31
	strb r6, [r0]
	subs r0, #2
	movs r1, #1
	strb r1, [r0]
	subs r0, #8
	strb r6, [r0]
	adds r0, #0xb
	strb r6, [r0]
	subs r0, #2
	strb r1, [r0]
	subs r0, #8
	strb r6, [r0]
_08014264:
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

