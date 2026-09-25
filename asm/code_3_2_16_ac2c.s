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
