.include "asm/macros.inc"

.syntax unified
.arm

@ sub_802C2FC is reconstructed (but not yet byte-matching) as C in
@ src/graphics/actor_part19b.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching.md, "Parked, not matched: sub_802C2FC".
.if NON_MATCHING == 0
	thumb_func_start sub_802C2FC
sub_802C2FC: @ 0x0802C2FC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	ldr r0, [r6, #0x1c]
	ldr r1, [r6, #0x20]
	asrs r4, r0, #8
	asrs r5, r1, #8
	adds r0, r6, #0
	bl GetAnimFrameData
	adds r7, r0, #0
	movs r0, #0
	mov r8, r0
	ldrb r0, [r7]
	lsls r2, r0, #2
	ldrb r1, [r7, #1]
	lsls r0, r1, #2
	subs r4, r4, r2
	subs r5, r5, r0
	cmp r5, #0x9f
	bgt _0802C388
	lsls r0, r1, #3
	adds r0, r5, r0
	cmp r0, #0
	blt _0802C388
	cmp r4, #0xef
	bgt _0802C388
	lsls r0, r2, #1
	adds r0, r4, r0
	cmp r0, #0
	blt _0802C388
	movs r0, #0x80
	lsls r0, r0, #1
	mov r8, r0
	adds r0, r6, #0
	bl sub_803B060
	movs r3, #0xff
	ands r3, r5
	ldr r1, _0802C374 @ =0x000001FF
	ands r4, r1
	lsls r1, r4, #0x10
	orrs r3, r1
	orrs r3, r0
	mov r0, r8
	orrs r3, r0
	ldr r4, [r6, #0x18]
	lsls r2, r4, #0xc
	ldr r0, [r6, #0x14]
	movs r1, #0x80
	lsls r1, r1, #8
	ands r0, r1
	cmp r0, #0
	beq _0802C378
	movs r0, #0x80
	lsls r0, r0, #4
	orrs r2, r0
	lsls r0, r2, #0x10
	b _0802C37A
	.align 2, 0
_0802C374: .4byte 0x000001FF
_0802C378:
	lsls r0, r4, #0x1c
_0802C37A:
	lsrs r2, r0, #0x10
	adds r0, r7, #0
	adds r1, r3, #0
	movs r3, #0xa0
	lsls r3, r3, #1
	bl SetupSpriteFrameOam
_0802C388:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

.endif

