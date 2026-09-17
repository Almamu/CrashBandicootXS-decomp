.include "asm/macros.inc"

.syntax unified
.arm

@ MeasureText is reconstructed (semantics fully understood, but not yet
@ byte-matching) as C in src/graphics/hud_icon_widget_8994.c, guarded by
@ #if NON_MATCHING - see docs/matching/issue-46-hud-icon-widget.md for
@ the exact remaining gap (needs r8 register pinning and an if/else-if
@ block-layout shape this compiler won't reproduce with plain C, the
@ same class of issue already documented for sub_8006600/sub_8037388
@ elsewhere in this codebase).
.if NON_MATCHING == 0
	thumb_func_start MeasureText
MeasureText: @ 0x08028994
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov ip, r0
	adds r4, r1, #0
	movs r5, #0
	movs r3, #0
	ldrb r0, [r4]
	cmp r0, #0
	beq _080289EE
	movs r1, #0x90
	lsls r1, r1, #1
	add r1, ip
	mov r8, r1
	movs r6, #0x86
	lsls r6, r6, #1
	add r6, ip
_080289B6:
	cmp r0, #0xa
	beq _080289C4
	cmp r0, #0x20
	bne _080289CE
	mov r7, r8
	ldr r0, [r7]
	b _080289E4
_080289C4:
	cmp r3, r5
	bls _080289CA
	adds r5, r3, #0
_080289CA:
	movs r3, #0
	b _080289E6
_080289CE:
	mov r1, ip
	adds r1, #8
	adds r1, r1, r0
	ldr r2, [r6]
	ldrb r7, [r1]
	lsls r0, r7, #1
	adds r1, r7, #0
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
_080289E4:
	adds r3, r3, r0
_080289E6:
	adds r4, #1
	ldrb r0, [r4]
	cmp r0, #0
	bne _080289B6
_080289EE:
	adds r0, r3, #0
	cmp r0, r5
	bhs _080289F6
	adds r0, r5, #0
_080289F6:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
.endif
