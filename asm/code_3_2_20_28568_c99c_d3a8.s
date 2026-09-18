.include "asm/macros.inc"

.syntax unified
.arm

@ sub_802D3A8 is reconstructed (but not yet byte-matching) as C in
@ src/graphics/actor_part61.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-54-actor-d3a8.md.
.if NON_MATCHING == 0
	thumb_func_start sub_802D3A8
sub_802D3A8: @ 0x0802D3A8
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	mov ip, r2
	adds r7, r3, #0
	ldr r0, [r5, #0x28]
	cmp r0, #0
	bne _0802D41C
	ldr r4, _0802D40C @ =gStaticData_0816A820
	ldr r1, [r5, #0x44]
	lsls r0, r1, #2
	movs r3, #0xff
	ands r0, r3
	lsls r0, r0, #1
	adds r0, r0, r4
	movs r2, #0
	ldrsh r0, [r0, r2]
	lsls r2, r0, #1
	adds r2, r2, r0
	lsls r2, r2, #3
	ldr r0, _0802D410 @ =0xFFFFF000
	adds r2, r2, r0
	adds r2, r6, r2
	lsls r1, r1, #1
	ands r1, r3
	lsls r1, r1, #1
	adds r1, r1, r4
	movs r0, #0
	ldrsh r1, [r1, r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #1
	ldr r1, _0802D414 @ =0xFFFFE200
	adds r0, r0, r1
	mov r1, ip
	adds r4, r1, r0
	ldr r0, _0802D418 @ =0xFFFFFE00
	adds r3, r7, r0
	ldr r1, [r5, #0x1c]
	subs r0, r2, r1
	cmp r0, #0
	bge _0802D3FE
	adds r0, #0xf
_0802D3FE:
	asrs r0, r0, #4
	adds r0, r1, r0
	str r0, [r5, #0x1c]
	ldr r1, [r5, #0x20]
	subs r0, r4, r1
	b _0802D46C
	.align 2, 0
_0802D40C: .4byte gStaticData_0816A820
_0802D410: .4byte 0xFFFFF000
_0802D414: .4byte 0xFFFFE200
_0802D418: .4byte 0xFFFFFE00
_0802D41C:
	cmp r0, #1
	bne _0802D450
	str r6, [r5, #0x1c]
	ldr r2, _0802D448 @ =gStaticData_0816A820
	ldr r1, [r5, #0x44]
	lsls r0, r1, #3
	adds r0, r0, r1
	movs r1, #0xff
	ands r0, r1
	lsls r0, r0, #1
	adds r0, r0, r2
	movs r1, #0
	ldrsh r0, [r0, r1]
	lsls r0, r0, #2
	ldr r2, _0802D44C @ =0xFFFFF600
	adds r0, r0, r2
	add r0, ip
	str r0, [r5, #0x20]
	movs r1, #0x80
	lsls r1, r1, #2
	adds r0, r7, r1
	b _0802D486
	.align 2, 0
_0802D448: .4byte gStaticData_0816A820
_0802D44C: .4byte 0xFFFFF600
_0802D450:
	movs r2, #0x80
	lsls r2, r2, #2
	adds r3, r7, r2
	ldr r1, [r5, #0x1c]
	subs r0, r6, r1
	cmp r0, #0
	bge _0802D460
	adds r0, #0xf
_0802D460:
	asrs r0, r0, #4
	adds r0, r1, r0
	str r0, [r5, #0x1c]
	ldr r1, [r5, #0x20]
	mov r2, ip
	subs r0, r2, r1
_0802D46C:
	cmp r0, #0
	bge _0802D472
	adds r0, #0xf
_0802D472:
	asrs r0, r0, #4
	adds r0, r1, r0
	str r0, [r5, #0x20]
	ldr r1, [r5, #0x24]
	subs r0, r3, r1
	cmp r0, #0
	bge _0802D482
	adds r0, #3
_0802D482:
	asrs r0, r0, #2
	adds r0, r1, r0
_0802D486:
	str r0, [r5, #0x24]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
.endif

