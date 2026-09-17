.include "asm/macros.inc"

.syntax unified
.arm

@ PlaySfx is reconstructed (semantics fully understood, but not yet
@ byte-matching) as C in src/audio/sfx_ambient.c, guarded by
@ #if NON_MATCHING - see docs/matching.md for the exact remaining gap
@ (a prologue register-save scheduling difference this compiler won't
@ reproduce).
.if NON_MATCHING == 0
	thumb_func_start PlaySfx
PlaySfx: @ 0x08001854
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov sb, r0
	mov sl, r1
	str r2, [sp]
	movs r1, #0
	ldr r0, [r0, #4]
	cmp r0, #1
	bne _08001870
	movs r1, #1
_08001870:
	cmp r1, #0
	beq _080018F4
	mov r1, sl
	lsls r0, r1, #1
	add r0, sl
	lsls r5, r0, #2
	ldr r2, _08001904 @ =gStaticData_0816AA6C
	adds r0, r5, r2
	ldr r4, [r0]
	cmp r4, #0
	beq _080018F4
	ldr r0, _08001908 @ =gUnknown_030007FC
	mov r8, r0
	ldr r1, [r0]
	adds r0, r2, #0
	adds r0, #4
	adds r0, r5, r0
	ldr r7, [r0]
	movs r6, #1
	rsbs r6, r6, #0
	adds r0, r4, #0
	adds r2, r7, #0
	adds r3, r6, #0
	bl sub_8038E74
	adds r3, r0, #0
	cmp r3, r6
	bne _080018C0
	mov r2, r8
	ldr r1, [r2]
	movs r0, #1
	eors r1, r0
	str r1, [r2]
	adds r0, r4, #0
	adds r2, r7, #0
	bl sub_8038E74
	adds r3, r0, #0
	cmp r3, r6
	beq _080018F4
_080018C0:
	ldr r0, _08001904 @ =gStaticData_0816AA6C
	adds r0, #8
	adds r0, r5, r0
	ldr r1, [r0]
	mov r2, sb
	ldr r0, [r2, #0x2c]
	muls r0, r1, r0
	ldr r2, [sp]
	adds r1, r0, #0
	muls r1, r2, r1
	lsrs r1, r1, #0x10
	adds r0, r3, #0
	bl sub_80390F8
	mov r0, r8
	ldr r1, [r0]
	lsls r2, r1, #2
	mov r0, sb
	adds r0, #0x10
	adds r0, r0, r2
	mov r2, sl
	str r2, [r0]
	movs r0, #1
	eors r1, r0
	mov r0, r8
	str r1, [r0]
_080018F4:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08001904: .4byte gStaticData_0816AA6C
_08001908: .4byte gUnknown_030007FC
.endif
