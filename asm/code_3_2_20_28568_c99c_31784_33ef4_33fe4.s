.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8033FE4
sub_8033FE4: @ 0x08033FE4
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r1, _08034010 @ =gStaticData_0817C4F8
	ldr r0, [r4, #0x28]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r7, #2
	ldrsh r2, [r0, r7]
	adds r7, r1, #0
	cmp r2, #0
	ble _08034014
	movs r1, #4
	ldrsh r0, [r0, r1]
	adds r0, r4, r0
	ldr r1, [r0]
	lsls r0, r2, #3
	adds r0, r0, r1
	subs r0, #8
	ldr r5, [r0]
	ldr r6, [r0, #4]
	adds r3, r6, #0
	b _0803401A
	.align 2, 0
_08034010: .4byte gStaticData_0817C4F8
_08034014:
	adds r0, r7, #4
	adds r0, r3, r0
	ldr r3, [r0]
_0803401A:
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _08034030
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _08034032
_08034030:
	adds r0, r1, #0
_08034032:
	adds r0, r4, r0
	bl sub_803AD84
	ldr r0, [r4, #0x28]
	cmp r0, #2
	bne _08034048
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _08034048
	movs r0, #0
	b _0803404A
_08034048:
	movs r0, #1
_0803404A:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
