.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8009914 is reconstructed (semantics fully understood, but not
@ yet byte-matching) as C in src/graphics/actor_part11.c, guarded by
@ #if NON_MATCHING - see docs/matching.md for the exact remaining gap.
.if NON_MATCHING == 0
	thumb_func_start sub_8009914
sub_8009914: @ 0x08009914
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	adds r4, r0, #0
	movs r6, #0
	b _0800994C
_08009924:
	ldr r0, [r4, #8]
	lsls r5, r6, #2
	adds r0, r5, r0
	ldr r2, [r0]
	cmp r2, #0
	beq _08009942
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_08009942:
	ldr r0, [r4, #8]
	adds r0, r5, r0
	movs r1, #0
	str r1, [r0]
	adds r6, #1
_0800994C:
	ldr r0, [r4]
	cmp r6, r0
	blt _08009924
	movs r0, #0
	str r0, [r4]
	ldr r3, [r4, #4]
	movs r0, #0x81
	lsls r0, r0, #4
	adds r0, r0, r4
	mov sb, r0
	ldr r1, _080099C0 @ =0x00000814
	adds r1, r1, r4
	mov sl, r1
	movs r0, #0
	movs r1, #0x82
	lsls r1, r1, #3
	adds r2, r4, r1
	adds r1, r4, #0
	adds r1, #0x10
	movs r5, #0xff
_08009974:
	stm r1!, {r0}
	stm r2!, {r0}
	subs r5, #1
	cmp r5, #0
	bge _08009974
	movs r5, #0
	cmp r5, r3
	bge _080099D8
	mov ip, sb
	movs r7, #0
	movs r2, #8
	mov r8, r2
	movs r6, #0
_0800998E:
	mov r3, ip
	ldr r1, [r3]
	lsls r2, r5, #3
	adds r1, r2, r1
	ldr r0, [r4, #0xc]
	adds r0, r0, r6
	str r0, [r1]
	ldr r0, [r4, #0xc]
	adds r0, r6, r0
	str r7, [r0]
	str r7, [r0, #4]
	str r7, [r0, #0xc]
	strb r7, [r0, #0x10]
	ldr r0, [r4, #0xc]
	adds r0, r6, r0
	ldr r3, [r3]
	adds r1, r3, r2
	str r1, [r0, #8]
	ldr r0, [r4, #4]
	subs r0, #1
	cmp r5, r0
	bne _080099C4
	str r7, [r1, #4]
	b _080099CA
	.align 2, 0
_080099C0: .4byte 0x00000814
_080099C4:
	mov r2, r8
	adds r0, r3, r2
	str r0, [r1, #4]
_080099CA:
	movs r3, #8
	add r8, r3
	adds r6, #0x14
	adds r5, #1
	ldr r0, [r4, #4]
	cmp r5, r0
	blt _0800998E
_080099D8:
	mov r1, sb
	ldr r0, [r1]
	mov r2, sl
	str r0, [r2]
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
.endif
	.align 2, 0


@ sub_80099F0 is reconstructed (semantics fully understood, but not
@ yet byte-matching) as C in src/graphics/actor_part12.c, guarded by
@ #if NON_MATCHING - see docs/matching.md for the exact remaining gap.
.if NON_MATCHING == 0
	thumb_func_start sub_80099F0
sub_80099F0: @ 0x080099F0
	sub sp, #0xc
	push {r4, r5, lr}
	str r1, [sp, #0xc]
	str r2, [sp, #0x10]
	str r3, [sp, #0x14]
	ldr r4, [sp, #0x1c]
	ldr r5, [sp, #0x20]
	adds r0, r4, #0
	add r1, sp, #0xc
	bl sub_8009FF4
	cmp r0, #0
	beq _08009A28
	ldr r1, [r4, #0x18]
	adds r1, #0x68
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldrb r2, [r5, #0xa]
	ldr r4, [r1, #4]
	movs r1, #1
	movs r3, #0
	bl sub_803AD88
	movs r0, #8
	ldrb r1, [r5, #0xc]
	orrs r0, r1
	strb r0, [r5, #0xc]
_08009A28:
	pop {r4, r5}
	pop {r3}
	add sp, #0xc
	bx r3
.endif
