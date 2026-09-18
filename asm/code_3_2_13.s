.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8008F20 is reconstructed (semantics fully understood, but not
@ yet byte-matching) as C in src/graphics/actor_part11.c, guarded by
@ #if NON_MATCHING - see docs/matching.md for the exact remaining gap.
.if NON_MATCHING == 0
	thumb_func_start sub_8008F20
sub_8008F20: @ 0x08008F20
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	adds r5, r0, #0
	adds r0, r1, #0
	movs r1, #0
	str r1, [r5]
	str r0, [r5, #4]
	lsls r0, r0, #2
	bl sub_8026EC0
	str r0, [r5, #8]
	ldr r1, [r5, #4]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	bl sub_8026EC0
	str r0, [r5, #0xc]
	movs r0, #0x81
	lsls r0, r0, #4
	adds r4, r5, r0
	ldr r0, [r5, #4]
	lsls r0, r0, #3
	bl sub_8026EC0
	str r0, [r4]
	ldr r0, [r5, #4]
	cmp r0, #0
	ble _08008F6C
	movs r2, #0
	ldr r1, [r5, #8]
_08008F64:
	stm r1!, {r2}
	subs r0, #1
	cmp r0, #0
	bne _08008F64
_08008F6C:
	ldr r3, [r5, #4]
	movs r1, #0x81
	lsls r1, r1, #4
	adds r1, r1, r5
	mov sb, r1
	ldr r2, _08008FD8 @ =0x00000814
	adds r2, r2, r5
	mov sl, r2
	movs r0, #0
	movs r1, #0x82
	lsls r1, r1, #3
	adds r2, r5, r1
	adds r1, r5, #0
	adds r1, #0x10
	movs r4, #0xff
_08008F8A:
	stm r1!, {r0}
	stm r2!, {r0}
	subs r4, #1
	cmp r4, #0
	bge _08008F8A
	movs r4, #0
	cmp r4, r3
	bge _08008FF0
	mov ip, sb
	movs r7, #0
	movs r2, #8
	mov r8, r2
	movs r6, #0
_08008FA4:
	mov r0, ip
	ldr r1, [r0]
	lsls r2, r4, #3
	adds r1, r2, r1
	ldr r0, [r5, #0xc]
	adds r0, r0, r6
	str r0, [r1]
	ldr r0, [r5, #0xc]
	adds r0, r6, r0
	str r7, [r0]
	str r7, [r0, #4]
	str r7, [r0, #0xc]
	strb r7, [r0, #0x10]
	ldr r0, [r5, #0xc]
	adds r0, r6, r0
	mov r1, ip
	ldr r3, [r1]
	adds r1, r3, r2
	str r1, [r0, #8]
	ldr r0, [r5, #4]
	subs r0, #1
	cmp r4, r0
	bne _08008FDC
	str r7, [r1, #4]
	b _08008FE2
	.align 2, 0
_08008FD8: .4byte 0x00000814
_08008FDC:
	mov r2, r8
	adds r0, r3, r2
	str r0, [r1, #4]
_08008FE2:
	movs r0, #8
	add r8, r0
	adds r6, #0x14
	adds r4, #1
	ldr r0, [r5, #4]
	cmp r4, r0
	blt _08008FA4
_08008FF0:
	mov r1, sb
	ldr r0, [r1]
	mov r2, sl
	str r0, [r2]
	adds r0, r5, #0
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
.endif
