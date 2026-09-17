.include "asm/macros.inc"

.syntax unified
.arm
	thumb_func_start sub_8030648
sub_8030648: @ 0x08030648
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r1, _08030674 @ =gStaticData_0817C2B8
	ldr r0, [r4, #0x28]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r7, #2
	ldrsh r2, [r0, r7]
	adds r7, r1, #0
	cmp r2, #0
	ble _08030678
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
	b _0803067E
	.align 2, 0
_08030674: .4byte gStaticData_0817C2B8
_08030678:
	adds r0, r7, #4
	adds r0, r3, r0
	ldr r3, [r0]
_0803067E:
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _08030694
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _08030696
_08030694:
	adds r0, r1, #0
_08030696:
	adds r0, r4, r0
	bl sub_803AD84
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

