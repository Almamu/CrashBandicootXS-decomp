.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8021D04
sub_8021D04: @ 0x08021D04
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r3, #0
	str r3, [sp]
	adds r3, r4, #0
	bl sub_800FF0C
	adds r5, r0, #0
	ldr r0, _08021D7C @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r4, r4, #1
	adds r4, r4, r0
	ldr r0, [r1, #0xc]
	ldrh r4, [r4]
	adds r0, r4, r0
	adds r3, r0, #0
	movs r0, #2
	ldrb r1, [r3]
	ands r0, r1
	cmp r0, #0
	beq _08021D56
	adds r0, r5, #0
	adds r0, #0x28
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r2, [r0]
	ands r1, r2
	movs r2, #0x10
	orrs r1, r2
	strb r1, [r0]
_08021D56:
	movs r0, #4
	ldrb r3, [r3]
	ands r0, r3
	cmp r0, #0
	beq _08021D72
	adds r0, r5, #0
	adds r0, #0x28
	movs r1, #0x21
	rsbs r1, r1, #0
	ldrb r2, [r0]
	ands r1, r2
	movs r2, #0x20
	orrs r1, r2
	strb r1, [r0]
_08021D72:
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08021D7C: .4byte gUnknown_030012B4

