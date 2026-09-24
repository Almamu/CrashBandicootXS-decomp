.include "asm/macros.inc"

.syntax unified
.arm

@ sub_800944C is reconstructed (semantics fully understood, but not
@ yet byte-matching) as C in src/graphics/actor_part11.c, guarded by
@ #if NON_MATCHING - see docs/matching.md for the exact remaining gap.
.if NON_MATCHING == 0
	thumb_func_start sub_800944C
sub_800944C: @ 0x0800944C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x10
	adds r3, r0, #0
	ldr r0, _08009508 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r2, [r0, #0x10]
	ldr r1, [r2]
	lsls r1, r1, #8
	ldr r0, [r2, #4]
	lsls r0, r0, #8
	str r1, [sp]
	str r0, [sp, #4]
	movs r0, #0xf0
	lsls r0, r0, #8
	movs r1, #0xa0
	lsls r1, r1, #8
	str r0, [sp, #8]
	str r1, [sp, #0xc]
	ldr r5, [r2]
	asrs r5, r5, #8
	cmp r5, #0
	bge _0800947E
	movs r5, #0
_0800947E:
	adds r1, r5, #2
	adds r7, r3, #0
	adds r7, #0x10
	ldr r0, _0800950C @ =0x0000040C
	adds r0, r0, r3
	mov r8, r0
_0800948A:
	lsls r0, r1, #2
	adds r0, r7, r0
	ldr r4, [r0]
	subs r6, r1, #1
	cmp r4, #0
	beq _080094C8
_08009496:
	ldr r0, [r4]
	ldr r2, [r0, #0x18]
	movs r3, #0x30
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x34]
	mov r1, sp
	bl sub_803AD80
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _080094C2
	ldr r0, [r4]
	ldr r2, [r0, #0x18]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r2, #0x24]
	bl sub_803AD7C
	movs r0, #1
	strb r0, [r4, #0x11]
_080094C2:
	ldr r4, [r4, #4]
	cmp r4, #0
	bne _08009496
_080094C8:
	adds r1, r6, #0
	cmp r1, r5
	bge _0800948A
	mov r0, r8
	ldr r4, [r0]
	cmp r4, #0
	beq _0800951A
_080094D6:
	ldr r1, [r4, #0xc]
	ldrb r0, [r1, #0x11]
	cmp r0, #0
	bne _08009510
	ldr r0, [r4]
	ldr r2, [r0, #0x18]
	movs r3, #0x30
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x34]
	mov r1, sp
	bl sub_803AD80
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08009514
	ldr r0, [r4]
	ldr r2, [r0, #0x18]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r2, #0x24]
	bl sub_803AD7C
	b _08009514
	.align 2, 0
_08009508: .4byte gUnknown_03001308
_0800950C: .4byte 0x0000040C
_08009510:
	movs r0, #0
	strb r0, [r1, #0x11]
_08009514:
	ldr r4, [r4, #4]
	cmp r4, #0
	bne _080094D6
_0800951A:
	add sp, #0x10
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
.endif
	.align 2, 0
