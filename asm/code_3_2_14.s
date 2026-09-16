.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8009BE0
sub_8009BE0: @ 0x08009BE0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	adds r5, r0, #0
	mov sb, r1
	ldrb r0, [r2, #4]
	mov sl, r0
	ldr r0, [r5, #4]
	str r0, [sp, #0xc]
	ldr r0, [r5]
	ldr r1, [r5, #4]
	str r0, [sp, #4]
	str r1, [sp, #8]
	add r0, sp, #4
	mov r1, sb
	bl sub_8008278
	ldr r0, [sp, #4]
	asrs r0, r0, #8
	str r0, [sp, #4]
	ldr r0, [sp, #8]
	asrs r0, r0, #8
	str r0, [sp, #8]
	adds r6, r5, #0
	adds r6, #0x69
	movs r0, #0
	strb r0, [r6]
	ldr r1, _08009C40 @ =gUnknown_03001308
	mov r8, r1
	ldr r0, [r1]
	add r4, sp, #0xc
	str r4, [sp]
	mov r1, sb
	add r2, sp, #4
	mov r3, sl
	bl sub_8026628
	lsls r0, r0, #0x18
	lsrs r1, r0, #0x18
	cmp r1, #0
	beq _08009C44
	ldr r0, [sp, #0xc]
	str r0, [r5, #4]
	movs r0, #1
	b _08009C8C
	.align 2, 0
_08009C40: .4byte gUnknown_03001308
_08009C44:
	mov r2, r8
	ldr r0, [r2]
	adds r0, #0x2a
	ldrb r7, [r0]
	strb r1, [r0]
	adds r5, r6, #0
	add r4, sp, #4
_08009C52:
	ldrb r0, [r5]
	adds r0, #1
	strb r0, [r5]
	ldr r0, [r4, #4]
	adds r0, #8
	str r0, [r4, #4]
	mov r1, r8
	ldr r0, [r1]
	add r2, sp, #0xc
	str r2, [sp]
	mov r1, sb
	add r2, sp, #4
	mov r3, sl
	bl sub_8026628
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08009C82
	ldrb r0, [r6]
	cmp r0, #2
	bls _08009C52
	mov r1, r8
	ldr r0, [r1]
	b _08009C86
_08009C82:
	ldr r0, _08009C9C @ =gUnknown_03001308
	ldr r0, [r0]
_08009C86:
	adds r0, #0x2a
	strb r7, [r0]
	movs r0, #0
_08009C8C:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08009C9C: .4byte gUnknown_03001308

	thumb_func_start sub_8009CA0
sub_8009CA0: @ 0x08009CA0
	push {r4, r5, lr}
	sub sp, #0x20
	adds r4, r0, #0
	ldrb r1, [r4, #0xc]
	lsrs r0, r1, #2
	movs r5, #1
	ands r0, r5
	cmp r0, #0
	beq _08009CDE
	ldr r0, _08009D1C @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r3, #0
	adds r0, #0x8c
	ldr r1, _08009D20 @ =gUnknown_0300082C
	ldr r2, [r0]
	ldr r0, [r1]
	cmp r2, r0
	bls _08009CC6
	movs r3, #1
_08009CC6:
	cmp r3, #0
	beq _08009CD4
	ldr r0, _08009D24 @ =gUnknown_030012C0
	ldr r0, [r0]
	ldr r0, [r0, #0x78]
	cmp r0, #3
	bne _08009CDE
_08009CD4:
	ldrb r1, [r4, #0xd]
	lsrs r0, r1, #3
	ands r0, r5
	cmp r0, #0
	beq _08009CF4
_08009CDE:
	ldrb r1, [r4, #0xd]
	lsrs r0, r1, #3
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _08009D4E
	ldr r0, _08009D24 @ =gUnknown_030012C0
	ldr r0, [r0]
	ldr r0, [r0, #0x78]
	cmp r0, #3
	bne _08009D4E
_08009CF4:
	mov r0, sp
	adds r1, r4, #0
	bl sub_8007C30
	ldr r0, [sp, #8]
	cmp r0, #0
	beq _08009D28
	ldr r0, _08009D1C @ =gUnknown_030012D8
	ldr r0, [r0]
	mov r1, sp
	bl sub_800B37C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08009D28
	adds r0, r4, #0
	bl sub_8009D5C
	b _08009D4E
	.align 2, 0
_08009D1C: .4byte gUnknown_030012D8
_08009D20: .4byte gUnknown_0300082C
_08009D24: .4byte gUnknown_030012C0
_08009D28:
	add r5, sp, #0x10
	adds r0, r5, #0
	adds r1, r4, #0
	bl sub_8007CF8
	ldr r0, [sp, #0x18]
	cmp r0, #0
	beq _08009D4E
	ldr r0, _08009D58 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_800B37C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08009D4E
	adds r0, r4, #0
	bl sub_8009D5C
_08009D4E:
	add sp, #0x20
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08009D58: .4byte gUnknown_030012D8

	thumb_func_start sub_8009D5C
sub_8009D5C: @ 0x08009D5C
	push {r4, r5, lr}
	adds r5, r0, #0
	movs r0, #8
	ldrb r1, [r5, #0xc]
	orrs r0, r1
	strb r0, [r5, #0xc]
	ldr r0, _08009D7C @ =gUnknown_030012C0
	ldr r0, [r0]
	ldr r0, [r0, #0x78]
	cmp r0, #2
	bgt _08009D80
	cmp r0, #1
	bge _08009DA0
	cmp r0, #0
	beq _08009D86
	b _08009DEE
	.align 2, 0
_08009D7C: .4byte gUnknown_030012C0
_08009D80:
	cmp r0, #3
	beq _08009DD8
	b _08009DEE
_08009D86:
	ldr r0, _08009D9C @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldrb r2, [r5, #0xa]
	ldr r4, [r1, #4]
	movs r1, #0
	b _08009DCA
	.align 2, 0
_08009D9C: .4byte gUnknown_030012D8
_08009DA0:
	ldr r0, _08009DD4 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldrb r2, [r5, #0xa]
	ldr r4, [r1, #4]
	movs r1, #0
	movs r3, #0
	bl sub_803AD88
	ldr r1, [r5, #0x18]
	adds r1, #0x68
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r4, [r1, #4]
	movs r1, #1
	movs r2, #1
_08009DCA:
	movs r3, #0
	bl sub_803AD88
	b _08009DEE
	.align 2, 0
_08009DD4: .4byte gUnknown_030012D8
_08009DD8:
	ldr r1, [r5, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r4, [r1, #4]
	movs r1, #1
	movs r2, #1
	movs r3, #0
	bl sub_803AD88
_08009DEE:
	pop {r4, r5}
	pop {r0}
	bx r0

