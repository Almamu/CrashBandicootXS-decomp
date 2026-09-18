.include "asm/macros.inc"

.syntax unified
.arm

.if NON_MATCHING == 0
	thumb_func_start sub_8005E5C
sub_8005E5C: @ 0x08005E5C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r2
	ldr r5, _08005EEC @ =gUnknown_030012DC
	ldr r0, [r5]
	movs r4, #0x98
	lsls r4, r4, #1
	adds r2, r0, r4
	ldr r3, [r2]
	movs r6, #0x20
	ldrsh r2, [r3, r6]
	adds r0, r0, r2
	ldr r2, [r3, #0x24]
	bl sub_803AD80
	ldr r1, [r5]
	movs r7, #0x88
	lsls r7, r7, #1
	adds r0, r1, r7
	ldr r2, [r0]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r0, r1, r3
	ldr r3, [r0]
	ldr r6, _08005EF0 @ =gUnknown_030012E0
	ldr r0, [r6]
	subs r2, #2
	adds r1, r0, r7
	str r2, [r1]
	movs r2, #0x8a
	lsls r2, r2, #1
	adds r1, r0, r2
	str r3, [r1]
	adds r1, r0, r4
	ldr r2, [r1]
	movs r3, #0x30
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x34]
	movs r1, #0x2f
	bl sub_803AD80
	ldr r1, [r6]
	adds r6, r7, #0
	adds r0, r1, r6
	ldr r2, [r0]
	adds r7, #4
	adds r0, r1, r7
	ldr r3, [r0]
	ldr r0, [r5]
	subs r2, #5
	adds r3, #8
	adds r1, r0, r6
	str r2, [r1]
	adds r2, r7, #0
	adds r1, r0, r2
	str r3, [r1]
	adds r4, r0, r4
	ldr r2, [r4]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	mov r1, r8
	bl sub_803AD80
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08005EEC: .4byte gUnknown_030012DC
_08005EF0: .4byte gUnknown_030012E0
.endif
