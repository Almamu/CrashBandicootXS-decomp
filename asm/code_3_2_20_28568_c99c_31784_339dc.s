.include "asm/macros.inc"

.syntax unified
.arm

@ sub_80339DC is reconstructed (but not yet byte-matching) as C in
@ src/graphics/actor_part29.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-62-0x08033804-actor.md, "Parked, not matched:
@ sub_80339DC".
.if NON_MATCHING == 0
	thumb_func_start sub_80339DC
sub_80339DC: @ 0x080339DC
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	adds r5, r0, #0
	bl sub_8033900
	movs r1, #0x80
	lsls r1, r1, #6
	adds r0, r0, r1
	str r0, [r5, #0x1c]
	bl sub_80338F4
	movs r2, #0xc0
	lsls r2, r2, #6
	adds r0, r0, r2
	str r0, [r5, #0x20]
	bl sub_80338E8
	ldr r1, _08033A98 @ =0xFFFFFF00
	adds r6, r0, r1
	str r6, [r5, #0x24]
	ldr r2, [r5, #0x64]
	mov sb, r2
	cmp r2, #0
	bne _08033AAC
	ldr r0, _08033A9C @ =gUnknown_03000884
	ldr r4, [r0]
	ldr r0, [r4, #0x24]
	subs r0, r0, r6
	subs r1, #0xaa
	bl sub_803ADB4
	adds r1, r0, #0
	cmp r1, #0
	ble _08033AB2
	movs r0, #0x80
	lsls r0, r0, #5
	bl sub_803ADB4
	ldr r1, [r4, #0x1c]
	ldr r2, [r5, #0x1c]
	mov ip, r2
	subs r1, r1, r2
	adds r3, r1, #0
	muls r3, r0, r3
	asrs r1, r3, #0xc
	mov r8, r1
	ldr r1, [r4, #0x20]
	ldr r7, [r5, #0x20]
	subs r1, r1, r7
	adds r2, r1, #0
	muls r2, r0, r2
	asrs r4, r2, #0xc
	asrs r3, r3, #0x1f
	mov r1, r8
	eors r1, r3
	subs r1, r1, r3
	asrs r2, r2, #0x1f
	adds r0, r4, #0
	eors r0, r2
	subs r0, r0, r2
	adds r1, r1, r0
	ldr r0, _08033AA0 @ =0x00000FFF
	cmp r1, r0
	bgt _08033AB2
	str r4, [sp]
	mov r0, ip
	adds r1, r7, #0
	adds r2, r6, #0
	mov r3, r8
	bl sub_802E674
	ldr r0, [r5, #0x1c]
	ldr r1, [r5, #0x20]
	ldr r2, [r5, #0x24]
	bl sub_802E504
	ldr r4, [r5, #0x68]
	adds r4, #1
	str r4, [r5, #0x68]
	bl sub_80338C4
	ldr r0, [r0, #0x14]
	cmp r4, r0
	bne _08033AA4
	mov r2, sb
	str r2, [r5, #0x68]
	bl sub_80338C4
	ldr r0, [r0, #0x18]
	b _08033AB0
	.align 2, 0
_08033A98: .4byte 0xFFFFFF00
_08033A9C: .4byte gUnknown_03000884
_08033AA0: .4byte 0x00000FFF
_08033AA4:
	bl sub_80338C4
	ldr r0, [r0, #0x10]
	b _08033AB0
_08033AAC:
	mov r0, sb
	subs r0, #1
_08033AB0:
	str r0, [r5, #0x64]
_08033AB2:
	ldr r1, [r5, #0x34]
	movs r0, #0x96
	lsls r0, r0, #7
	cmp r1, r0
	ble _08033AD0
	movs r1, #0
	str r1, [r5, #0x28]
	str r1, [r5, #0x44]
	str r1, [r5, #0xc]
	ldr r0, [r5]
	ldrh r0, [r0]
	movs r2, #0
	strh r0, [r5, #0x10]
	strb r2, [r5, #0x12]
	str r1, [r5, #8]
_08033AD0:
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

.endif
