.include "asm/macros.inc"

.syntax unified
.arm
	thumb_func_start sub_8030574
sub_8030574: @ 0x08030574
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r1, _080305A0 @ =gStaticData_0817C2B8
	ldr r0, [r4, #0x28]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r7, #2
	ldrsh r2, [r0, r7]
	adds r7, r1, #0
	cmp r2, #0
	ble _080305A4
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
	b _080305AA
	.align 2, 0
_080305A0: .4byte gStaticData_0817C2B8
_080305A4:
	adds r0, r7, #4
	adds r0, r3, r0
	ldr r3, [r0]
_080305AA:
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _080305C0
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _080305C2
_080305C0:
	adds r0, r1, #0
_080305C2:
	adds r0, r4, r0
	bl sub_803AD84
	ldr r0, [r4, #0x28]
	cmp r0, #2
	bne _080305EA
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _080305EA
	cmp r4, #0
	beq _080305F0
	ldr r1, [r4, #0x50]
	movs r2, #8
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
	b _080305F0
_080305EA:
	adds r0, r4, #0
	bl sub_802A7B8
_080305F0:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80305F8
sub_80305F8: @ 0x080305F8
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	sub sp, #4
	adds r4, r0, #0
	adds r6, r2, #0
	mov r8, r3
	ldr r0, [sp, #0x18]
	movs r5, #2
	str r0, [sp]
	adds r0, r4, #0
	bl InitActorPart
	str r5, [r4, #0x54]
	ldr r0, _0803063C @ =gStaticData_087E525C
	str r0, [r4, #0x50]
	str r6, [r4, #0x58]
	mov r0, r8
	str r0, [r4, #0x5c]
	movs r1, #0
	str r1, [r4, #0x64]
	movs r0, #0x95
	str r0, [r4, #0x60]
	adds r0, r4, #0
	adds r0, #0x68
	strb r1, [r0]
	adds r0, r4, #0
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0803063C: .4byte gStaticData_087E525C

