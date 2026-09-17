.include "asm/macros.inc"

.syntax unified
.arm

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
