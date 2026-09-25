.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_800BFA8
sub_800BFA8: @ 0x0800BFA8
	push {r4, lr}
	sub sp, #8
	adds r4, r0, #0
	ldr r0, _0800BFD0 @ =gUnknown_0300082C
	ldr r0, [r0]
	ldr r1, [r4, #0x48]
	adds r0, r0, r1
	ldr r2, [r4, #0x4c]
	subs r0, r0, r2
	bl sub_803AE4C
	cmp r0, #0
	bne _0800BFE8
	ldr r0, [r4, #0x68]
	cmp r0, #0
	beq _0800BFD4
	cmp r0, #4
	beq _0800BFDE
	b _0800C06C
	.align 2, 0
_0800BFD0: .4byte gUnknown_0300082C
_0800BFD4:
	adds r0, r4, #0
	movs r1, #2
	bl sub_800C8CC
	b _0800C06C
_0800BFDE:
	adds r0, r4, #0
	movs r1, #7
	bl sub_800C8CC
	b _0800C06C
_0800BFE8:
	ldr r1, [r4, #0x70]
	adds r0, r1, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800C014
	ldr r0, [r4, #0x68]
	cmp r0, #2
	beq _0800C000
	cmp r0, #7
	beq _0800C00A
	b _0800C06C
_0800C000:
	adds r0, r4, #0
	movs r1, #0
	bl sub_800C8CC
	b _0800C06C
_0800C00A:
	adds r0, r4, #0
	movs r1, #4
	bl sub_800C8CC
	b _0800C06C
_0800C014:
	movs r2, #0
	ldr r0, [r4, #0x68]
	cmp r0, #2
	beq _0800C022
	cmp r0, #7
	beq _0800C042
	b _0800C064
_0800C022:
	ldr r0, [r1, #0x30]
	cmp r0, #0xa
	bne _0800C064
	ldr r0, [r1, #0x34]
	cmp r0, #0
	bne _0800C064
	movs r3, #0xa
	rsbs r3, r3, #0
	movs r0, #0x80
	lsls r0, r0, #3
	str r0, [sp]
	str r1, [sp, #4]
	movs r0, #0xc
	movs r1, #6
	movs r2, #0
	b _0800C05E
_0800C042:
	ldr r0, [r1, #0x30]
	cmp r0, #8
	bne _0800C064
	ldr r0, [r1, #0x34]
	cmp r0, #0
	bne _0800C064
	movs r0, #0x80
	lsls r0, r0, #3
	str r0, [sp]
	str r1, [sp, #4]
	movs r0, #0xc
	movs r1, #6
	movs r2, #0
	movs r3, #8
_0800C05E:
	bl sub_800C9C8
	adds r2, r0, #0
_0800C064:
	cmp r2, #0
	beq _0800C06C
	movs r0, #8
	strb r0, [r2, #0xa]
_0800C06C:
	add sp, #8
	pop {r4}
	pop {r0}
	bx r0
