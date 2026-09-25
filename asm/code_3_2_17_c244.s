.include "asm/macros.inc"

.syntax unified
.arm

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
