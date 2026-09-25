.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_800A0FC
sub_800A0FC: @ 0x0800A0FC
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r6, r4, #0
	adds r6, #0x68
	ldrb r5, [r6]
	ldrb r1, [r4, #0xc]
	lsrs r0, r1, #7
	cmp r0, #0
	beq _0800A16C
	adds r0, r4, #0
	bl sub_800A178
	orrs r5, r0
	strb r5, [r6]
	adds r0, r4, #0
	bl sub_800A050
	movs r0, #8
	ldrb r2, [r6]
	ands r0, r2
	cmp r0, #0
	beq _0800A16C
	movs r0, #0x21
	rsbs r0, r0, #0
	ldrb r1, [r4, #0xc]
	ands r0, r1
	strb r0, [r4, #0xc]
	ldrb r2, [r4, #0xd]
	lsrs r0, r2, #1
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	bne _0800A16C
	ldr r1, [r4, #0x18]
	movs r2, #0x10
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #0x14]
	bl sub_803AD7C
	adds r2, r0, #0
	adds r0, r4, #0
	movs r1, #8
	bl sub_8009BE0
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0800A16C
	movs r0, #0x20
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	movs r0, #7
	ldrb r2, [r6]
	ands r0, r2
	strb r0, [r6]
_0800A16C:
	adds r0, r4, #0
	adds r0, #0x68
	ldrb r0, [r0]
	pop {r4, r5, r6}
	pop {r1}
	bx r1
