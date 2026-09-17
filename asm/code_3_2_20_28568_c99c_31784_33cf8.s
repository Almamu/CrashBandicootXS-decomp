.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8033CF8 is reconstructed (but not yet byte-matching) as C in
@ src/graphics/actor_part35.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-62-0x08033804-actor.md, "Parked, not matched:
@ sub_8033CF8".
.if NON_MATCHING == 0
	thumb_func_start sub_8033CF8
sub_8033CF8: @ 0x08033CF8
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r5, r0, #0
	bl sub_8033900
	movs r1, #0xf0
	lsls r1, r1, #5
	adds r0, r0, r1
	str r0, [r5, #0x1c]
	bl sub_80338F4
	ldr r2, _08033D88 @ =0xFFFFD000
	adds r0, r0, r2
	str r0, [r5, #0x20]
	bl sub_80338E8
	ldr r2, _08033D8C @ =0xFFFFFF00
	adds r1, r0, r2
	str r1, [r5, #0x24]
	ldr r7, [r5, #0x64]
	cmp r7, #0
	bne _08033DE2
	ldr r0, _08033D90 @ =gUnknown_03000884
	ldr r6, [r0]
	ldr r0, [r6, #0x24]
	subs r0, r0, r1
	ldr r1, _08033D94 @ =0xFFFFFE56
	bl sub_803ADB4
	adds r1, r0, #0
	cmp r1, #0
	ble _08033DE6
	movs r0, #0x80
	lsls r0, r0, #5
	bl sub_803ADB4
	ldr r1, [r6, #0x1c]
	ldr r2, [r5, #0x1c]
	subs r1, r1, r2
	adds r4, r1, #0
	muls r4, r0, r4
	asrs r3, r4, #0xc
	ldr r1, [r6, #0x20]
	ldr r2, [r5, #0x20]
	subs r1, r1, r2
	muls r0, r1, r0
	asrs r1, r0, #0xc
	asrs r4, r4, #0x1f
	eors r3, r4
	subs r3, r3, r4
	asrs r0, r0, #0x1f
	eors r1, r0
	subs r1, r1, r0
	adds r3, r3, r1
	ldr r0, _08033D98 @ =0x00000FFF
	cmp r3, r0
	bgt _08033DE6
	movs r0, #3
	bl sub_8000E1C
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	cmp r0, #0
	bne _08033D9C
	ldr r1, [r5, #0x1c]
	ldr r2, [r5, #0x20]
	ldr r3, [r5, #0x24]
	str r7, [sp]
	movs r0, #5
	bl sub_802E170
	b _08033DBE
	.align 2, 0
_08033D88: .4byte 0xFFFFD000
_08033D8C: .4byte 0xFFFFFF00
_08033D90: .4byte gUnknown_03000884
_08033D94: .4byte 0xFFFFFE56
_08033D98: .4byte 0x00000FFF
_08033D9C:
	cmp r0, #1
	bne _08033DB0
	ldr r1, [r5, #0x1c]
	ldr r2, [r5, #0x20]
	ldr r3, [r5, #0x24]
	str r7, [sp]
	movs r0, #6
	bl sub_802E170
	b _08033DBE
_08033DB0:
	ldr r1, [r5, #0x1c]
	ldr r2, [r5, #0x20]
	ldr r3, [r5, #0x24]
	str r7, [sp]
	movs r0, #8
	bl sub_802E170
_08033DBE:
	ldr r4, [r5, #0x68]
	adds r4, #1
	str r4, [r5, #0x68]
	bl sub_80338C4
	ldr r0, [r0, #0x20]
	cmp r4, r0
	bne _08033DDA
	movs r0, #0
	str r0, [r5, #0x68]
	bl sub_80338C4
	ldr r0, [r0, #0x24]
	b _08033DE4
_08033DDA:
	bl sub_80338C4
	ldr r0, [r0, #0x1c]
	b _08033DE4
_08033DE2:
	subs r0, r7, #1
_08033DE4:
	str r0, [r5, #0x64]
_08033DE6:
	ldr r1, [r5, #0x34]
	movs r0, #0x96
	lsls r0, r0, #7
	cmp r1, r0
	ble _08033E0E
	bl sub_80338D0
	cmp r0, #3
	bne _08033E0E
	movs r2, #0
	movs r0, #2
	str r2, [r5, #0x28]
	str r2, [r5, #0x44]
	str r0, [r5, #0xc]
	ldr r0, [r5]
	ldrh r0, [r0, #0x18]
	movs r1, #0
	strh r0, [r5, #0x10]
	strb r1, [r5, #0x12]
	str r2, [r5, #8]
_08033E0E:
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

.endif
