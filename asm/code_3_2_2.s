.include "asm/macros.inc"

.syntax unified
.arm
@ sub_8007B00 is reconstructed (extremely close, but not yet
@ byte-matching) as C in src/graphics/actor_part.c, guarded by
@ #if NON_MATCHING - this raw version is used only for the real
@ byte-matching build. See docs/matching.md, "Parked, not matched:
@ sub_8007B00".
.if NON_MATCHING == 0
	thumb_func_start sub_8007B00
sub_8007B00: @ 0x08007B00
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x10
	mov r8, r0
	adds r7, r1, #0
	ldr r1, [r7, #0x20]
	adds r2, r7, #0
	adds r2, #0x2d
	ldrb r3, [r2]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	ldr r1, [r1]
	adds r1, r1, r0
	adds r3, r1, #0
	adds r3, #0xc
	ldr r4, [r7]
	asrs r4, r4, #8
	movs r5, #0xc
	ldrsh r1, [r1, r5]
	ldr r0, [r7, #4]
	asrs r0, r0, #8
	movs r5, #2
	ldrsh r2, [r3, r5]
	ldrb r5, [r3, #4]
	ldrb r6, [r3, #5]
	adds r1, r1, r4
	adds r2, r2, r0
	mov r0, sp
	bl sub_803AFE4
	mov r0, sp
	adds r1, r5, #0
	adds r2, r6, #0
	bl sub_803AFDC
	adds r3, r7, #0
	adds r3, #0x28
	ldrb r1, [r3]
	lsls r0, r1, #0x1b
	cmp r0, #0
	bge _08007B66
	ldr r0, [r7]
	asrs r0, r0, #8
	lsls r0, r0, #1
	ldr r1, [sp]
	ldr r2, [sp, #8]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp]
_08007B66:
	ldrb r3, [r3]
	lsls r0, r3, #0x1a
	cmp r0, #0
	bge _08007B7E
	ldr r0, [r7, #4]
	asrs r0, r0, #8
	lsls r0, r0, #1
	ldr r1, [sp, #4]
	ldr r2, [sp, #0xc]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #4]
_08007B7E:
	mov r0, r8
	mov r1, sp
	ldm r1!, {r2, r3, r4}
	stm r0!, {r2, r3, r4}
	ldr r1, [r1]
	str r1, [r0]
	mov r0, r8
	add sp, #0x10
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

.endif

@ sub_8007B98 is reconstructed (extremely close, but not yet
@ byte-matching) as C in src/graphics/actor_part.c, guarded by
@ #if NON_MATCHING - this raw version is used only for the real
@ byte-matching build. See docs/matching.md, "Parked, not matched:
@ sub_8007B98".
.if NON_MATCHING == 0
	thumb_func_start sub_8007B98
sub_8007B98: @ 0x08007B98
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x10
	mov r8, r0
	adds r7, r1, #0
	ldr r1, [r7, #0x20]
	adds r2, r7, #0
	adds r2, #0x2d
	ldrb r3, [r2]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	ldr r1, [r1]
	adds r1, r1, r0
	adds r3, r1, #4
	ldr r4, [r7]
	asrs r4, r4, #8
	movs r5, #4
	ldrsh r1, [r1, r5]
	ldr r0, [r7, #4]
	asrs r0, r0, #8
	movs r5, #2
	ldrsh r2, [r3, r5]
	ldrb r5, [r3, #4]
	ldrb r6, [r3, #5]
	adds r1, r1, r4
	adds r2, r2, r0
	mov r0, sp
	bl sub_803AFE4
	mov r0, sp
	adds r1, r5, #0
	adds r2, r6, #0
	bl sub_803AFDC
	adds r3, r7, #0
	adds r3, #0x28
	ldrb r1, [r3]
	lsls r0, r1, #0x1b
	cmp r0, #0
	bge _08007BFC
	ldr r0, [r7]
	asrs r0, r0, #8
	lsls r0, r0, #1
	ldr r1, [sp]
	ldr r2, [sp, #8]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp]
_08007BFC:
	ldrb r3, [r3]
	lsls r0, r3, #0x1a
	cmp r0, #0
	bge _08007C14
	ldr r0, [r7, #4]
	asrs r0, r0, #8
	lsls r0, r0, #1
	ldr r1, [sp, #4]
	ldr r2, [sp, #0xc]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #4]
_08007C14:
	mov r0, r8
	mov r1, sp
	ldm r1!, {r2, r3, r4}
	stm r0!, {r2, r3, r4}
	ldr r1, [r1]
	str r1, [r0]
	mov r0, r8
	add sp, #0x10
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

.endif

