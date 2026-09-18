.include "asm/macros.inc"

.syntax unified
.arm

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
