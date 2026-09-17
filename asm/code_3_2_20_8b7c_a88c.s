.include "asm/macros.inc"

.syntax unified
.arm

@ UpdateAnimatedActorPart is reconstructed (but not yet byte-matching) as
@ C in src/graphics/actor_part44.c, guarded by #if NON_MATCHING - this
@ raw version is only assembled for the default (matching) build. See
@ docs/matching/issue-50-actor-2a69c.md.
.if NON_MATCHING == 0
	thumb_func_start UpdateAnimatedActorPart
UpdateAnimatedActorPart: @ 0x0802A88C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r6, r0, #0
	ldr r4, [r6, #0x34]
	lsls r0, r4, #8
	ldr r1, [r6, #0x30]
	ldr r1, [r1, #0x10]
	bl sub_803ADB4
	mov r8, r0
	ldr r0, _0802A8F0 @ =gUnknown_030013C8
	ldr r0, [r0]
	lsls r0, r0, #0xc
	adds r1, r4, #0
	bl sub_803ADB4
	adds r5, r0, #0
	bl sub_8029E98
	ldr r1, [r6, #0x20]
	adds r4, r1, #0
	muls r4, r5, r4
	asrs r4, r4, #0xc
	adds r4, r4, r0
	asrs r4, r4, #8
	bl sub_8029EB4
	ldr r1, [r6, #0x1c]
	muls r1, r5, r1
	asrs r1, r1, #0xc
	adds r1, r1, r0
	asrs r5, r1, #8
	adds r0, r6, #0
	bl GetAnimFrameData
	adds r7, r0, #0
	movs r2, #0
	mov r1, r8
	cmp r1, #0xff
	bgt _0802A8E4
	movs r2, #0x80
	lsls r2, r2, #2
_0802A8E4:
	cmp r2, #0
	beq _0802A8F4
	ldrb r3, [r7]
	lsls r1, r3, #3
	b _0802A8F8
	.align 2, 0
_0802A8F0: .4byte gUnknown_030013C8
_0802A8F4:
	ldrb r3, [r7]
	lsls r1, r3, #2
_0802A8F8:
	cmp r2, #0
	beq _0802A902
	ldrb r0, [r0, #1]
	lsls r0, r0, #3
	b _0802A906
_0802A902:
	ldrb r0, [r0, #1]
	lsls r0, r0, #2
_0802A906:
	subs r5, r5, r1
	subs r4, r4, r0
	cmp r4, #0x9f
	bgt _0802A972
	lsls r0, r0, #1
	adds r0, r4, r0
	cmp r0, #0
	blt _0802A972
	cmp r5, #0xef
	bgt _0802A972
	lsls r0, r1, #1
	adds r0, r5, r0
	cmp r0, #0
	blt _0802A972
	movs r0, #0x80
	lsls r0, r0, #1
	cmp r8, r0
	beq _0802A92C
	orrs r2, r0
_0802A92C:
	adds r0, r6, #0
	str r2, [sp]
	bl sub_803B060
	movs r3, #0xff
	ands r3, r4
	ldr r1, _0802A960 @ =0x000001FF
	ands r5, r1
	lsls r1, r5, #0x10
	orrs r3, r1
	orrs r3, r0
	ldr r2, [sp]
	orrs r3, r2
	ldr r4, [r6, #0x18]
	lsls r2, r4, #0xc
	ldr r0, [r6, #0x14]
	movs r1, #0x80
	lsls r1, r1, #8
	ands r0, r1
	cmp r0, #0
	beq _0802A964
	movs r0, #0x80
	lsls r0, r0, #4
	orrs r2, r0
	lsls r0, r2, #0x10
	b _0802A966
	.align 2, 0
_0802A960: .4byte 0x000001FF
_0802A964:
	lsls r0, r4, #0x1c
_0802A966:
	lsrs r2, r0, #0x10
	adds r0, r7, #0
	adds r1, r3, #0
	mov r3, r8
	bl SetupSpriteFrameOam
_0802A972:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
.endif
