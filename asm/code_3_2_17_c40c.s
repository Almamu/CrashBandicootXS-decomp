.include "asm/macros.inc"

.syntax unified
.arm

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

