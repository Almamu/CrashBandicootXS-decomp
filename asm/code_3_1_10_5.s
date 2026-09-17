.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8004914 is reconstructed (semantics understood) but NOT YET
@ byte-matching - parked as C in src/graphics/settings_menu.c, guarded
@ by #if NON_MATCHING. See docs/matching.md for this chunk's write-up.
.if NON_MATCHING == 0
	thumb_func_start sub_8004914
sub_8004914: @ 0x08004914
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r4, r0, #0
	lsls r3, r3, #0x18
	adds r7, r1, #0
	adds r7, #0x1d
	adds r2, #0xc
	mov sb, r2
	cmp r3, #0
	beq _0800494C
	ldr r0, _08004948 @ =gUnknown_030012DC
	ldr r2, [r0]
	ldr r0, [r4, #4]
	asrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	movs r1, #2
	cmp r0, #0
	beq _08004940
	movs r1, #1
_08004940:
	adds r0, r2, #0
	bl sub_8028A30
	b _08004956
	.align 2, 0
_08004948: .4byte gUnknown_030012DC
_0800494C:
	ldr r0, _080049C8 @ =gUnknown_030012DC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8028A30
_08004956:
	ldr r0, _080049C8 @ =gUnknown_030012DC
	mov r8, r0
	ldr r4, [r0]
	movs r5, #0x98
	lsls r5, r5, #1
	adds r0, r4, r5
	ldr r0, [r0]
	adds r6, r0, #0
	adds r6, #0x10
	movs r1, #0x10
	ldrsh r0, [r0, r1]
	adds r4, r4, r0
	movs r0, #0x25
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r6, #4]
	adds r0, r4, #0
	bl sub_803AD80
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	subs r0, r7, r0
	mov r2, r8
	ldr r4, [r2]
	movs r2, #0x88
	lsls r2, r2, #1
	adds r1, r4, r2
	str r0, [r1]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r4, r1
	mov r2, sb
	str r2, [r0]
	adds r5, r4, r5
	ldr r0, [r5]
	adds r5, r0, #0
	adds r5, #0x20
	movs r1, #0x20
	ldrsh r0, [r0, r1]
	adds r4, r4, r0
	movs r0, #0x25
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080049C8: .4byte gUnknown_030012DC
.endif

@ sub_80049CC is reconstructed (semantics understood) but NOT YET
@ byte-matching - parked as C in src/graphics/settings_menu.c, guarded
@ by #if NON_MATCHING. See docs/matching.md for this chunk's write-up.
.if NON_MATCHING == 0
	thumb_func_start sub_80049CC
sub_80049CC: @ 0x080049CC
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	mov sb, r1
	ldr r6, _08004A4C @ =gUnknown_030012E0
	ldr r0, [r6]
	movs r1, #0
	bl sub_8028A30
	ldr r4, [r6]
	movs r5, #0x98
	lsls r5, r5, #1
	adds r0, r4, r5
	ldr r0, [r0]
	movs r1, #0x10
	adds r1, r1, r0
	mov r8, r1
	movs r3, #0x10
	ldrsh r0, [r0, r3]
	adds r4, r4, r0
	mov r0, sb
	bl sub_8026F38
	adds r1, r0, #0
	mov r0, r8
	ldr r2, [r0, #4]
	adds r0, r4, #0
	bl sub_803AD80
	movs r1, #0xf0
	subs r1, r1, r0
	asrs r1, r1, #1
	ldr r4, [r6]
	movs r2, #6
	movs r3, #0x88
	lsls r3, r3, #1
	adds r0, r4, r3
	str r1, [r0]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r4, r1
	str r2, [r0]
	adds r5, r4, r5
	ldr r0, [r5]
	adds r5, r0, #0
	adds r5, #0x20
	movs r3, #0x20
	ldrsh r0, [r0, r3]
	adds r4, r4, r0
	mov r0, sb
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08004A4C: .4byte gUnknown_030012E0
.endif
