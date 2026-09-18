.include "asm/macros.inc"

.syntax unified
.arm

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

