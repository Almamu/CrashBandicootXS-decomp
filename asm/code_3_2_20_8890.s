.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8028890/sub_8028900 are reconstructed (semantics fully understood,
@ but not yet byte-matching) as C in src/graphics/hud_icon_widget_8890.c,
@ guarded by #if NON_MATCHING - see
@ docs/matching/issue-46-hud-icon-widget.md for the exact remaining gap
@ (sub_8028900 needs r8/sb register pinning this compiler won't
@ reproduce with plain C, the same class of issue already documented for
@ sub_8006600/sub_8037388 elsewhere in this codebase; sub_8028890 is an
@ if/else-if block-layout mismatch).
.if NON_MATCHING == 0
	thumb_func_start sub_8028890
sub_8028890: @ 0x08028890
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldrb r3, [r5]
	cmp r3, #0
	beq _080288F8
	movs r0, #0x88
	lsls r0, r0, #1
	adds r6, r4, r0
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r7, r4, r1
_080288A8:
	cmp r3, #0xa
	beq _080288C0
	cmp r3, #0x20
	bne _080288DA
	movs r2, #0x90
	lsls r2, r2, #1
	adds r0, r4, r2
	ldr r1, [r6]
	ldr r0, [r0]
	adds r1, r1, r0
	str r1, [r6]
	b _080288F0
_080288C0:
	movs r1, #0x8c
	lsls r1, r1, #1
	adds r0, r4, r1
	ldr r0, [r0]
	str r0, [r6]
	movs r2, #0x8e
	lsls r2, r2, #1
	adds r1, r4, r2
	ldr r0, [r7]
	ldr r1, [r1]
	adds r0, r0, r1
	str r0, [r7]
	b _080288F0
_080288DA:
	movs r1, #0x98
	lsls r1, r1, #1
	adds r0, r4, r1
	ldr r1, [r0]
	movs r2, #0x30
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x34]
	adds r1, r3, #0
	bl sub_803AD80
_080288F0:
	adds r5, #1
	ldrb r3, [r5]
	cmp r3, #0
	bne _080288A8
_080288F8:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8028900
sub_8028900: @ 0x08028900
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r1
	adds r5, r2, #0
	movs r3, #0
	movs r4, #0
	cmp r3, r5
	bge _08028958
	movs r1, #0x90
	lsls r1, r1, #1
	adds r1, r1, r0
	mov ip, r1
	movs r7, #0x86
	lsls r7, r7, #1
	adds r7, r7, r0
	mov sb, r7
	adds r6, r0, #0
	adds r6, #8
_08028928:
	mov r1, r8
	adds r0, r1, r4
	ldrb r0, [r0]
	cmp r0, #0xa
	beq _08028952
	cmp r0, #0x20
	bne _0802893C
	mov r7, ip
	ldr r0, [r7]
	b _08028950
_0802893C:
	adds r1, r6, r0
	mov r0, sb
	ldr r2, [r0]
	ldrb r7, [r1]
	lsls r0, r7, #1
	adds r1, r7, #0
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
_08028950:
	adds r3, r3, r0
_08028952:
	adds r4, #1
	cmp r4, r5
	blt _08028928
_08028958:
	adds r0, r3, #0
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
.endif
