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

