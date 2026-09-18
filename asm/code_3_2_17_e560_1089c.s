.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_801089C
sub_801089C: @ 0x0801089C
	push {r4, r5, r6, lr}
	sub sp, #8
	adds r4, r0, #0
	lsls r1, r1, #0x18
	lsrs r6, r1, #0x18
	ldr r0, _080108F8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	ldrh r1, [r4, #8]
	ldr r0, _080108FC @ =0x0000FFFF
	cmp r1, r0
	beq _080108D2
	ldr r5, _08010900 @ =gUnknown_030012B4
	ldr r0, [r5]
	bl sub_802599C
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _080108D2
	ldr r0, [r5]
	ldrh r1, [r4, #8]
	bl sub_80259D4
_080108D2:
	ldr r1, [r4]
	asrs r1, r1, #8
	ldr r2, [r4, #4]
	asrs r2, r2, #8
	adds r2, #3
	ldr r0, _08010904 @ =gUnknown_030012E4
	ldr r0, [r0]
	movs r3, #3
	str r3, [sp]
	add r3, sp, #4
	strb r6, [r3]
	movs r3, #0
	bl sub_8025A64
	add sp, #8
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_080108F8: .4byte gUnknown_030012BC
_080108FC: .4byte 0x0000FFFF
_08010900: .4byte gUnknown_030012B4
_08010904: .4byte gUnknown_030012E4

