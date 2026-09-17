.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8038C88
sub_8038C88: @ 0x08038C88
	push {r4, r5, r6, lr}
	ldr r6, _08038DBC @ =gUnknown_03001630
	ldr r1, [r6]
	ldr r0, [r1, #0x30]
	cmp r0, #0
	bne _08038C96
	b _08038DB4
_08038C96:
	ldr r0, [r1, #4]
	ldr r0, [r0, #0x34]
	cmp r0, #0
	beq _08038CA4
	movs r1, #0x40
	bl sub_8037F3C
_08038CA4:
	ldr r0, [r6]
	ldr r1, [r0, #4]
	ldrh r0, [r1, #0x10]
	cmp r0, #0xff
	bls _08038CB2
	movs r0, #0xff
	strh r0, [r1, #0x10]
_08038CB2:
	ldr r2, [r6]
	ldr r1, [r2, #0x10]
	lsls r1, r1, #2
	adds r0, r2, #0
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0, #4]
	ldr r0, [r2, #4]
	ldrh r0, [r0, #0x10]
	strb r0, [r1, #0x1f]
	ldr r0, [r6]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r2, r0, r1
	ldr r1, [r0, #4]
	ldrh r1, [r1, #0xa]
	str r1, [r2]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r0, [r0, #4]
	movs r4, #1
	strb r4, [r0, #0x1a]
	ldr r3, [r6]
	ldr r1, [r3, #0x10]
	lsls r1, r1, #2
	adds r0, r3, #0
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r1, [r0, #4]
	ldrh r2, [r1, #4]
	ldr r1, [r3, #0x2c]
	muls r2, r1, r2
	ldr r1, [r3, #0x18]
	adds r1, r1, r2
	bl sub_803A5A8
	ldr r1, [r6]
	ldr r0, [r1, #0x2c]
	eors r0, r4
	str r0, [r1, #0x2c]
	ldr r2, [r1, #4]
	ldr r0, [r1, #0x10]
	lsls r0, r0, #2
	adds r1, #8
	adds r1, r1, r0
	ldr r0, [r1]
	ldr r0, [r0, #4]
	adds r0, #0x21
	ldrb r0, [r0]
	adds r2, #0x39
	strb r0, [r2]
	ldr r1, [r6]
	ldr r3, [r1, #0x10]
	cmp r3, #1
	bne _08038DAA
	ldr r2, [r1, #4]
	adds r0, r2, #0
	adds r0, #0x39
	ldrb r0, [r0]
	cmp r0, #0
	beq _08038DAA
	movs r0, #0
	str r0, [r1, #0x10]
	adds r0, r2, #0
	adds r0, #0x3a
	strb r3, [r0]
	ldr r0, [r6]
	ldr r1, [r0, #4]
	ldr r0, [r1, #0x2c]
	cmp r0, #0
	beq _08038DAA
	movs r5, #0
	ldrh r1, [r1, #0xe]
	cmp r5, r1
	bge _08038DAA
_08038D54:
	ldr r1, [r6]
	ldr r0, [r1, #0x10]
	lsls r0, r0, #2
	adds r2, r1, #0
	adds r2, #8
	adds r2, r2, r0
	ldr r0, [r1, #4]
	ldr r0, [r0, #0x30]
	ldr r0, [r0]
	adds r0, r0, r5
	ldr r2, [r2]
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	ldr r1, [r0, #8]
	ldr r0, [r2, #4]
	str r0, [r1]
	ldr r3, [r6]
	ldr r1, [r3, #0x10]
	lsls r1, r1, #2
	adds r0, r3, #0
	adds r0, #8
	adds r0, r0, r1
	ldr r4, [r0]
	ldr r2, [r4]
	ldr r0, [r2]
	ldr r1, [r0, #0xc]
	adds r1, r1, r5
	ldr r0, [r2, #8]
	lsls r1, r1, #2
	adds r1, r1, r0
	ldr r2, [r3, #4]
	ldr r0, [r2, #0x30]
	ldr r0, [r0]
	adds r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r4
	ldr r0, [r0]
	str r0, [r1]
	adds r5, #1
	ldrh r2, [r2, #0xe]
	cmp r5, r2
	blt _08038D54
_08038DAA:
	ldr r0, _08038DBC @ =gUnknown_03001630
	ldr r0, [r0]
	adds r0, #0x43
	movs r1, #1
	strb r1, [r0]
_08038DB4:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08038DBC: .4byte gUnknown_03001630

	thumb_func_start sub_8038DC0
sub_8038DC0: @ 0x08038DC0
	push {r4, r5, r6, r7, lr}
	adds r7, r0, #0
	ldr r4, _08038E6C @ =0x7FFFFFFF
	movs r3, #0
	ldr r2, _08038E70 @ =gUnknown_03001630
	ldr r0, [r2]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #0x14]
	adds r6, r2, #0
	cmp r3, r0
	bhs _08038E00
	adds r2, r0, #0
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r1, r0, r1
_08038DEC:
	ldr r0, [r1]
	ldr r0, [r0, #0x4c]
	cmp r0, r4
	bgt _08038DF8
	adds r5, r3, #0
	adds r4, r0, #0
_08038DF8:
	adds r1, #4
	adds r3, #1
	cmp r3, r2
	blo _08038DEC
_08038E00:
	ldr r0, [r6]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	adds r0, r0, r5
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	adds r0, #0x24
	movs r2, #0
	movs r1, #8
	strb r1, [r0]
	ldr r0, [r6]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	adds r0, r0, r5
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	adds r0, #0x25
	strb r7, [r0]
	ldr r0, [r6]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	adds r0, r0, r5
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	str r2, [r0, #0x4c]
	adds r0, r5, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08038E6C: .4byte 0x7FFFFFFF
_08038E70: .4byte gUnknown_03001630

	thumb_func_start sub_8038E74
sub_8038E74: @ 0x08038E74
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r0
	adds r6, r2, #0
	adds r7, r3, #0
	movs r5, #1
	rsbs r5, r5, #0
	adds r4, r6, #0
	adds r3, r5, #0
	cmp r1, r5
	bne _08038ECC
	movs r3, #0
	ldr r2, _08038EC8 @ =gUnknown_03001630
	ldr r0, [r2]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #0x14]
	mov ip, r2
	cmp r3, r0
	bhs _08038F02
	adds r2, r0, #0
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r1, r0, r1
_08038EB2:
	ldr r0, [r1]
	ldr r0, [r0, #0x4c]
	cmp r0, r4
	bgt _08038EBE
	adds r5, r3, #0
	adds r4, r0, #0
_08038EBE:
	adds r1, #4
	adds r3, #1
	cmp r3, r2
	blo _08038EB2
	b _08038F02
	.align 2, 0
_08038EC8: .4byte gUnknown_03001630
_08038ECC:
	adds r5, r1, #0
	ldr r2, _08038F34 @ =gUnknown_03001630
	ldr r0, [r2]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #0x14]
	mov ip, r2
	cmp r5, r0
	blo _08038EE8
	adds r5, r3, #0
_08038EE8:
	cmp r5, r3
	beq _08038F02
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	adds r0, r0, r5
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r0, [r0, #0x4c]
	cmp r6, r0
	bge _08038F02
	adds r5, r3, #0
_08038F02:
	ldr r0, _08038F38 @ =0x0FFFFFFF
	cmp r5, r0
	beq _08038F88
	mov r1, ip
	ldr r0, [r1]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	adds r0, r0, r5
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r4, [r0]
	movs r0, #1
	rsbs r0, r0, #0
	cmp r7, r0
	beq _08038F3C
	asrs r0, r7, #5
	adds r1, r0, #2
	b _08038F3E
	.align 2, 0
_08038F34: .4byte gUnknown_03001630
_08038F38: .4byte 0x0FFFFFFF
_08038F3C:
	movs r1, #8
_08038F3E:
	adds r0, r4, #0
	adds r0, #0x24
	strb r1, [r0]
	mov r1, ip
	ldr r0, [r1]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	adds r0, r0, r5
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	adds r0, #0x25
	mov r1, r8
	strb r1, [r0]
	mov r1, ip
	ldr r0, [r1]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1]
	ldr r0, [r0, #0xc]
	adds r0, r0, r5
	ldr r1, [r1, #8]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	str r6, [r0, #0x4c]
_08038F88:
	adds r0, r5, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
