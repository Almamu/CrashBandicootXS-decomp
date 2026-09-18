.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8024790
sub_8024790: @ 0x08024790
	push {r4, r5, lr}
	adds r5, r0, #0
	ldr r0, [r5]
	lsls r4, r1, #2
	adds r0, r4, r0
	ldr r0, [r0]
	ldrb r0, [r0, #0x11]
	cmp r0, #0
	beq _080247AC
	ldr r0, _080247E8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8001AC4
_080247AC:
	ldr r0, [r5]
	adds r0, r4, r0
	ldr r0, [r0]
	ldr r1, [r0, #0xc]
	movs r0, #1
	rsbs r0, r0, #0
	cmp r1, r0
	beq _080247C8
	lsls r0, r1, #0x18
	lsrs r0, r0, #0x18
	movs r1, #1
	movs r2, #0
	bl sub_800132C
_080247C8:
	ldr r0, [r5]
	adds r0, r4, r0
	ldr r1, [r0]
	ldrb r0, [r1, #0x12]
	cmp r0, #0
	beq _080247E2
	ldr r1, [r1, #0x18]
	cmp r1, #0x63
	beq _080247E2
	ldr r0, _080247E8 @ =gUnknown_030012BC
	ldr r0, [r0]
	bl sub_80019A8
_080247E2:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080247E8: .4byte gUnknown_030012BC

