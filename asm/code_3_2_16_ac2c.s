.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_800AC2C
sub_800AC2C: @ 0x0800AC2C
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	adds r6, r0, #0
	adds r7, r1, #0
	adds r5, r2, #0
	mov r8, r3
	subs r0, r5, #1
	cmp r0, #0x25
	bls _0800AC46
	b _0800AFE6
_0800AC46:
	lsls r0, r0, #2
	ldr r1, _0800AC50 @ =_0800AC54
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800AC50: .4byte _0800AC54
_0800AC54: @ jump table
	.4byte _0800AEF0 @ case 0
	.4byte _0800AEF0 @ case 1
	.4byte _0800AEF0 @ case 2
	.4byte _0800AEF0 @ case 3
	.4byte _0800AEF0 @ case 4
	.4byte _0800AEF0 @ case 5
	.4byte _0800AEF0 @ case 6
	.4byte _0800AEF0 @ case 7
	.4byte _0800AEF0 @ case 8
	.4byte _0800AEF0 @ case 9
	.4byte _0800AFE6 @ case 10
	.4byte _0800AFB0 @ case 11
	.4byte _0800AFC8 @ case 12
	.4byte _0800AFC8 @ case 13
	.4byte _0800AD58 @ case 14
	.4byte _0800AD68 @ case 15
	.4byte _0800AD1A @ case 16
	.4byte _0800AD14 @ case 17
	.4byte _0800AFE6 @ case 18
	.4byte _0800AFE6 @ case 19
	.4byte _0800AFE6 @ case 20
	.4byte _0800AFE6 @ case 21
	.4byte _0800AFA8 @ case 22
	.4byte _0800AFA8 @ case 23
	.4byte _0800AFC8 @ case 24
	.4byte _0800AE86 @ case 25
	.4byte _0800ACEC @ case 26
	.4byte _0800AD78 @ case 27
	.4byte _0800ADA8 @ case 28
	.4byte _0800ADD0 @ case 29
	.4byte _0800AE38 @ case 30
	.4byte _0800AE18 @ case 31
	.4byte _0800AE5C @ case 32
	.4byte _0800ADF8 @ case 33
	.4byte _0800AE80 @ case 34
	.4byte _0800AE80 @ case 35
	.4byte _0800AE80 @ case 36
	.4byte _0800AE80 @ case 37
_0800ACEC:
	ldr r0, _0800AD0C @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023404
	movs r1, #1
	ldrb r2, [r0]
	orrs r1, r2
	strb r1, [r0]
	ldr r0, _0800AD10 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x1c
	bl PlaySfx
	b _0800AFE6
	.align 2, 0
_0800AD0C: .4byte gUnknown_030012C0
_0800AD10: .4byte gUnknown_030012BC
_0800AD14:
	bl sub_80241A4
	b _0800AD46
_0800AD1A:
	ldr r0, _0800AD50 @ =gUnknown_030012C0
	ldr r1, [r0]
	adds r0, r1, #0
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800AD30
	adds r0, r1, #0
	movs r1, #0x64
	bl sub_8022EA8
_0800AD30:
	ldr r0, [r6, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r4, [r2, #0x14]
	adds r1, r7, #0
	adds r2, r5, #0
	mov r3, r8
	bl sub_803AD88
_0800AD46:
	ldr r0, _0800AD54 @ =gUnknown_03001318
	ldr r0, [r0]
	bl sub_8028504
	b _0800AFE6
	.align 2, 0
_0800AD50: .4byte gUnknown_030012C0
_0800AD54: .4byte gUnknown_03001318
_0800AD58:
	ldr r0, _0800AD64 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_802352C
	b _0800AFB0
	.align 2, 0
_0800AD64: .4byte gUnknown_030012C0
_0800AD68:
	ldr r0, _0800AD74 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023510
	b _0800AFB0
	.align 2, 0
_0800AD74: .4byte gUnknown_030012C0
_0800AD78:
	ldr r4, _0800ADA0 @ =gUnknown_030012C0
	ldr r0, [r4]
	ldr r0, [r0, #0x78]
	cmp r0, #3
	bne _0800AD8A
	adds r1, r6, #0
	adds r1, #0x8c
	movs r0, #0
	str r0, [r1]
_0800AD8A:
	ldr r0, _0800ADA4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x18
	bl PlaySfx
	ldr r0, [r4]
	bl sub_8022D50
	b _0800AFE6
	.align 2, 0
_0800ADA0: .4byte gUnknown_030012C0
_0800ADA4: .4byte gUnknown_030012BC
_0800ADA8:
	ldr r0, _0800ADC8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x1f
	bl PlaySfx
	ldr r0, _0800ADCC @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023404
	movs r1, #2
	ldrb r2, [r0]
	orrs r1, r2
	strb r1, [r0]
	b _0800AFE6
	.align 2, 0
_0800ADC8: .4byte gUnknown_030012BC
_0800ADCC: .4byte gUnknown_030012C0
_0800ADD0:
	ldr r0, _0800ADF0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x1f
	bl PlaySfx
	ldr r0, _0800ADF4 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023404
	movs r1, #4
	ldrb r3, [r0]
	orrs r1, r3
	strb r1, [r0]
	b _0800AFE6
	.align 2, 0
_0800ADF0: .4byte gUnknown_030012BC
_0800ADF4: .4byte gUnknown_030012C0
_0800ADF8:
	ldr r0, _0800AE10 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x1f
	bl PlaySfx
	ldr r0, _0800AE14 @ =gUnknown_030012C0
	ldr r1, [r0]
	movs r0, #2
	b _0800AE4C
	.align 2, 0
_0800AE10: .4byte gUnknown_030012BC
_0800AE14: .4byte gUnknown_030012C0
_0800AE18:
	ldr r0, _0800AE30 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x1f
	bl PlaySfx
	ldr r0, _0800AE34 @ =gUnknown_030012C0
	ldr r1, [r0]
	movs r0, #4
	b _0800AE70
	.align 2, 0
_0800AE30: .4byte gUnknown_030012BC
_0800AE34: .4byte gUnknown_030012C0
_0800AE38:
	ldr r0, _0800AE54 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x1f
	bl PlaySfx
	ldr r0, _0800AE58 @ =gUnknown_030012C0
	ldr r1, [r0]
	movs r0, #1
_0800AE4C:
	ldrb r2, [r1, #2]
	orrs r0, r2
	strb r0, [r1, #2]
	b _0800AFE6
	.align 2, 0
_0800AE54: .4byte gUnknown_030012BC
_0800AE58: .4byte gUnknown_030012C0
_0800AE5C:
	ldr r0, _0800AE78 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x1f
	bl PlaySfx
	ldr r0, _0800AE7C @ =gUnknown_030012C0
	ldr r1, [r0]
	movs r0, #8
_0800AE70:
	ldrb r3, [r1, #2]
	orrs r0, r3
	strb r0, [r1, #2]
	b _0800AFE6
	.align 2, 0
_0800AE78: .4byte gUnknown_030012BC
_0800AE7C: .4byte gUnknown_030012C0
_0800AE80:
	bl sub_80241A4
	b _0800AFE6
_0800AE86:
	ldr r0, _0800AEE4 @ =gUnknown_030012C0
	ldr r1, [r0]
	ldr r1, [r1, #0x78]
	adds r4, r0, #0
	cmp r1, #0
	bne _0800AEA4
	adds r3, r6, #0
	adds r3, #0xb8
	movs r2, #7
_0800AE98:
	ldr r0, [r6]
	ldr r1, [r6, #4]
	stm r3!, {r0, r1}
	subs r2, #1
	cmp r2, #0
	bge _0800AE98
_0800AEA4:
	ldr r0, [r4]
	ldr r1, [r0, #0x78]
	cmp r1, #2
	bgt _0800AEB8
	ldr r0, _0800AEE8 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #1
	bne _0800AEBC
_0800AEB8:
	cmp r1, #1
	bgt _0800AEC4
_0800AEBC:
	ldr r0, _0800AEE4 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023224
_0800AEC4:
	ldr r0, _0800AEE4 @ =gUnknown_030012C0
	ldr r0, [r0]
	ldr r0, [r0, #0x78]
	cmp r0, #3
	beq _0800AED0
	b _0800AFE6
_0800AED0:
	adds r1, r6, #0
	adds r1, #0x8c
	ldr r0, _0800AEEC @ =gUnknown_0300082C
	ldr r0, [r0]
	movs r2, #0x96
	lsls r2, r2, #3
	adds r0, r0, r2
	str r0, [r1]
	b _0800AFE6
	.align 2, 0
_0800AEE4: .4byte gUnknown_030012C0
_0800AEE8: .4byte gUnknown_030012D8
_0800AEEC: .4byte gUnknown_0300082C
_0800AEF0:
	ldrb r3, [r6, #0xc]
	lsrs r0, r3, #6
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _0800AFE6
	movs r2, #0
	adds r3, r6, #0
	adds r3, #0x8c
	ldr r0, _0800AF90 @ =gUnknown_0300082C
	ldr r1, [r3]
	ldr r0, [r0]
	cmp r1, r0
	bls _0800AF0E
	movs r2, #1
_0800AF0E:
	cmp r2, #0
	bne _0800AFE6
	ldr r1, _0800AF94 @ =gUnknown_030012C0
	mov sb, r1
	ldr r2, [r1]
	ldr r1, [r2, #0x78]
	cmp r1, #0
	beq _0800AFA0
	cmp r1, #2
	bgt _0800AFE6
	adds r0, #0x5a
	str r0, [r3]
	ldr r1, [r2, #0x78]
	subs r1, #1
	adds r0, r2, #0
	bl sub_80231EC
	ldr r4, _0800AF98 @ =gUnknown_030012BC
	ldr r0, [r4]
	movs r5, #0x80
	lsls r5, r5, #1
	movs r1, #0
	adds r2, r5, #0
	bl PlaySfx
	ldr r0, [r4]
	movs r1, #0x1b
	adds r2, r5, #0
	bl PlaySfx
	ldr r0, [r6, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r4, [r2, #0x14]
	adds r1, r7, #0
	movs r2, #0xb
	mov r3, r8
	bl sub_803AD88
	mov r1, sb
	ldr r0, [r1]
	ldr r0, [r0, #0x78]
	adds r0, r6, #0
	adds r0, #0xb0
	ldr r0, [r0]
	ldr r3, [r0]
	asrs r3, r3, #8
	ldr r2, [r0, #4]
	asrs r2, r2, #8
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r1, r0, #0x1b
	lsrs r1, r1, #0x1f
	ldr r0, _0800AF9C @ =gUnknown_030012E4
	ldr r0, [r0]
	str r2, [sp]
	str r1, [sp, #4]
	movs r1, #0x22
	movs r2, #3
	bl sub_8025BAC
	b _0800AFE6
	.align 2, 0
_0800AF90: .4byte gUnknown_0300082C
_0800AF94: .4byte gUnknown_030012C0
_0800AF98: .4byte gUnknown_030012BC
_0800AF9C: .4byte gUnknown_030012E4
_0800AFA0:
	adds r0, r2, #0
	bl sub_80232E4
	b _0800AFB0
_0800AFA8:
	movs r0, #0
	str r0, [r6, #0x54]
	str r0, [r6, #0x58]
	str r0, [r6, #0x5c]
_0800AFB0:
	ldr r0, [r6, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r4, [r2, #0x14]
	adds r1, r7, #0
	adds r2, r5, #0
	mov r3, r8
	bl sub_803AD88
	b _0800AFE6
_0800AFC8:
	movs r0, #0
	str r0, [r6, #0x54]
	str r0, [r6, #0x58]
	str r0, [r6, #0x5c]
	ldr r0, [r6, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r4, [r2, #0x14]
	adds r1, r7, #0
	adds r2, r5, #0
	mov r3, r8
	bl sub_803AD88
_0800AFE6:
	add sp, #8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_800AFF4
sub_800AFF4: @ 0x0800AFF4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	adds r7, r0, #0
	ldr r0, _0800B07C @ =gUnknown_030012C0
	ldr r0, [r0]
	ldr r0, [r0, #0x78]
	cmp r0, #3
	bne _0800B0D8
	ldr r0, _0800B080 @ =gUnknown_0300082C
	ldr r0, [r0]
	movs r1, #7
	ands r0, r1
	movs r1, #0x28
	adds r1, r1, r7
	mov r8, r1
	cmp r0, #0
	bne _0800B03A
	ldr r4, _0800B084 @ =gUnknown_03000818
	movs r0, #2
	bl sub_8000E1C
	mov r2, r8
	ldrb r2, [r2]
	lsls r1, r2, #0x1b
	lsrs r1, r1, #0x1f
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r0, #2
	lsls r1, r1, #1
	subs r0, r0, r1
	str r0, [r4]
_0800B03A:
	ldr r0, _0800B084 @ =gUnknown_03000818
	adds r2, r7, #0
	adds r2, #0xb0
	ldr r5, [r2]
	ldr r4, [r0]
	ldr r0, [r5, #0x20]
	adds r3, r5, #0
	adds r3, #0x2d
	ldr r1, [r0]
	ldrb r6, [r3]
	lsls r0, r6, #3
	subs r0, r0, r6
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	adds r3, r2, #0
	cmp r4, r0
	blt _0800B060
	subs r4, r0, #1
_0800B060:
	str r4, [r5, #0x30]
	mov r1, r8
	ldrb r1, [r1]
	lsls r0, r1, #0x1b
	cmp r0, #0
	bge _0800B090
	ldr r0, [r7]
	ldr r1, [r7, #4]
	ldr r2, [r3]
	ldr r4, _0800B088 @ =0xFFFFFA00
	adds r0, r0, r4
	ldr r5, _0800B08C @ =0xFFFFED00
	adds r1, r1, r5
	b _0800B0A0
	.align 2, 0
_0800B07C: .4byte gUnknown_030012C0
_0800B080: .4byte gUnknown_0300082C
_0800B084: .4byte gUnknown_03000818
_0800B088: .4byte 0xFFFFFA00
_0800B08C: .4byte 0xFFFFED00
_0800B090:
	ldr r0, [r7]
	ldr r1, [r7, #4]
	ldr r2, [r3]
	movs r6, #0xc0
	lsls r6, r6, #3
	adds r0, r0, r6
	ldr r4, _0800B0B8 @ =0xFFFFED00
	adds r1, r1, r4
_0800B0A0:
	str r0, [r2]
	str r1, [r2, #4]
	ldr r0, _0800B0BC @ =gUnknown_0300082C
	ldr r0, [r0]
	movs r1, #4
	ands r0, r1
	cmp r0, #0
	beq _0800B0C0
	ldr r0, [r3]
	movs r1, #1
	b _0800B0C4
	.align 2, 0
_0800B0B8: .4byte 0xFFFFED00
_0800B0BC: .4byte gUnknown_0300082C
_0800B0C0:
	ldr r0, [r3]
	movs r1, #2
_0800B0C4:
	adds r0, #0x2d
	strb r1, [r0]
	ldr r0, [r3]
	ldr r2, [r0, #0x18]
	movs r5, #0x20
	ldrsh r1, [r2, r5]
	adds r0, r0, r1
	ldr r1, [r2, #0x24]
	bl sub_803AD7C
_0800B0D8:
	ldr r0, _0800B258 @ =gUnknown_030012C0
	ldr r0, [r0]
	ldr r0, [r0, #0x78]
	cmp r0, #3
	beq _0800B100
	movs r2, #0
	adds r0, r7, #0
	adds r0, #0x8c
	ldr r1, _0800B25C @ =gUnknown_0300082C
	ldr r0, [r0]
	ldr r1, [r1]
	cmp r0, r1
	bls _0800B0F4
	movs r2, #1
_0800B0F4:
	cmp r2, #0
	beq _0800B100
	movs r0, #4
	ands r1, r0
	cmp r1, #0
	beq _0800B10A
_0800B100:
	ldr r0, _0800B260 @ =gUnknown_030012CC
	ldr r0, [r0]
	adds r1, r7, #0
	bl sub_8007A84
_0800B10A:
	ldr r0, _0800B258 @ =gUnknown_030012C0
	ldr r3, [r0]
	ldr r0, [r3, #0x78]
	cmp r0, #3
	bne _0800B132
	movs r4, #0
	adds r0, r7, #0
	adds r0, #0x8c
	ldr r1, _0800B25C @ =gUnknown_0300082C
	ldr r2, [r0]
	ldr r0, [r1]
	cmp r2, r0
	bls _0800B126
	movs r4, #1
_0800B126:
	cmp r4, #0
	bne _0800B132
	adds r0, r3, #0
	movs r1, #2
	bl sub_80231EC
_0800B132:
	ldr r2, [r7]
	adds r1, r7, #0
	adds r1, #0xb4
	ldr r0, [r1]
	lsls r0, r0, #3
	adds r4, r7, #0
	adds r4, #0xb8
	adds r0, r4, r0
	str r2, [r0]
	ldr r2, [r7, #4]
	ldr r0, [r1]
	lsls r0, r0, #3
	adds r3, r7, #0
	adds r3, #0xbc
	adds r0, r3, r0
	str r2, [r0]
	ldr r5, [r1]
	adds r2, r5, #1
	adds r0, r2, #0
	str r1, [sp]
	mov sb, r4
	mov sl, r3
	cmp r2, #0
	bge _0800B166
	adds r0, r5, #0
	adds r0, #8
_0800B166:
	asrs r0, r0, #3
	lsls r0, r0, #3
	subs r0, r2, r0
	ldr r6, [sp]
	str r0, [r6]
	ldr r0, _0800B258 @ =gUnknown_030012C0
	ldr r0, [r0]
	ldr r0, [r0, #0x78]
	mov r8, r0
	subs r0, #1
	cmp r0, #1
	bhi _0800B232
	ldr r0, _0800B25C @ =gUnknown_0300082C
	ldr r1, [r0]
	movs r2, #7
	ands r1, r2
	adds r4, r0, #0
	cmp r1, #0
	bne _0800B1AE
	ldr r5, _0800B264 @ =gUnknown_0300081C
	movs r0, #3
	bl sub_8000E1C
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	ldr r1, [r5]
	adds r1, r1, r0
	subs r1, #1
	str r1, [r5]
	cmp r1, #3
	ble _0800B1A6
	movs r1, #3
_0800B1A6:
	cmp r1, #0
	bge _0800B1AC
	movs r1, #0
_0800B1AC:
	str r1, [r5]
_0800B1AE:
	ldr r0, _0800B264 @ =gUnknown_0300081C
	movs r1, #0xb0
	adds r1, r1, r7
	mov ip, r1
	ldr r5, [r1]
	ldr r3, [r0]
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
	blt _0800B1D4
	subs r3, r0, #1
_0800B1D4:
	str r3, [r5, #0x30]
	ldr r0, [sp]
	ldr r3, [r0]
	lsls r3, r3, #3
	add sb, r3
	ldr r5, _0800B268 @ =gStaticData_0816A820
	ldr r1, [r4]
	movs r4, #0xff
	adds r0, r1, #0
	ands r0, r4
	lsls r0, r0, #1
	adds r0, r0, r5
	movs r6, #0
	ldrsh r2, [r0, r6]
	add r3, sl
	lsrs r1, r1, #1
	ands r1, r4
	lsls r1, r1, #1
	adds r1, r1, r5
	movs r4, #0
	ldrsh r0, [r1, r4]
	mov r5, ip
	ldr r4, [r5]
	lsls r2, r2, #4
	mov r6, sb
	ldr r1, [r6]
	adds r2, r2, r1
	lsls r0, r0, #3
	ldr r1, [r3]
	adds r0, r0, r1
	ldr r1, _0800B26C @ =0xFFFFE800
	adds r0, r0, r1
	str r2, [r4]
	str r0, [r4, #4]
	ldr r0, [r5]
	mov r1, r8
	subs r1, #1
	adds r0, #0x2d
	strb r1, [r0]
	ldr r0, [r5]
	ldr r2, [r0, #0x18]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r2, #0x24]
	bl sub_803AD7C
_0800B232:
	adds r0, r7, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800B246
	movs r0, #9
	rsbs r0, r0, #0
	ldrb r4, [r7, #0xc]
	ands r0, r4
	strb r0, [r7, #0xc]
_0800B246:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800B258: .4byte gUnknown_030012C0
_0800B25C: .4byte gUnknown_0300082C
_0800B260: .4byte gUnknown_030012CC
_0800B264: .4byte gUnknown_0300081C
_0800B268: .4byte gStaticData_0816A820
_0800B26C: .4byte 0xFFFFE800
