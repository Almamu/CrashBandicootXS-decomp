.include "asm/macros.inc"

.syntax unified
.arm

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
