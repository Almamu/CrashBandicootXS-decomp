.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8003B40 is reconstructed (semantics understood) but NOT YET
@ byte-matching - parked as C in src/graphics/settings_menu.c, guarded
@ by #if NON_MATCHING. See docs/matching.md for this chunk's write-up.
.if NON_MATCHING == 0
	thumb_func_start sub_8003B40
sub_8003B40: @ 0x08003B40
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	movs r0, #0x88
	lsls r0, r0, #2
	bl sub_8026EDC
	adds r5, r0, #0
	adds r0, r6, #0
	adds r0, #0x8c
	ldr r1, [r0]
	adds r0, r5, #0
	bl sub_8002FCC
	adds r0, r5, #0
	bl sub_8002FD8
_08003B60:
	bl sub_80006A8
	ldr r0, _08003B80 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r0, _08003B84 @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #2
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r1, r0, #0x10
	cmp r1, #0
	beq _08003B88
	movs r4, #3
	b _08003BA8
	.align 2, 0
_08003B80: .4byte gUnknown_03001304
_08003B84: .4byte gUnknown_030007E0
_08003B88:
	ldr r2, _08003BD4 @ =gUnknown_03000800
	ldrb r0, [r2]
	cmp r0, #0
	beq _08003B98
	strb r1, [r2]
	adds r0, r5, #0
	bl sub_8002FD8
_08003B98:
	ldr r0, _08003BD8 @ =gUnknown_03000804
	ldr r0, [r0]
	bl sub_8001F50
	adds r0, r5, #0
	bl sub_8002EFC
	adds r4, r0, #0
_08003BA8:
	cmp r4, #1
	beq _08003B60
	cmp r4, #0
	bne _08003BC6
	adds r0, r5, #0
	bl sub_8002FD4
	adds r1, r0, #0
	adds r0, r6, #0
	adds r0, #0x90
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #2
	bl sub_800014C
_08003BC6:
	adds r0, r5, #0
	bl sub_8026ED0
	adds r0, r4, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08003BD4: .4byte gUnknown_03000800
_08003BD8: .4byte gUnknown_03000804
.endif

@ sub_8003BDC is reconstructed (semantics understood) but NOT YET
@ byte-matching - parked as C in src/graphics/settings_menu.c, guarded
@ by #if NON_MATCHING. See docs/matching.md for this chunk's write-up.
.if NON_MATCHING == 0
	thumb_func_start sub_8003BDC
sub_8003BDC: @ 0x08003BDC
	push {r4, r5, r6, r7, lr}
	adds r5, r1, #0
	adds r6, r2, #0
	ldr r7, _08003C8C @ =gUnknown_030012DC
	ldr r0, [r7]
	movs r1, #0
	bl sub_8028A30
	cmp r5, #0
	beq _08003C3A
	ldr r0, [r7]
	movs r4, #0x98
	lsls r4, r4, #1
	adds r1, r0, r4
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	adds r1, r5, #0
	bl sub_803AD80
	adds r1, r0, #0
	movs r0, #0xf0
	subs r0, r0, r1
	asrs r3, r0, #1
	ldr r0, [r7]
	movs r1, #0x87
	mov ip, r1
	movs r2, #0x88
	lsls r2, r2, #1
	adds r1, r0, r2
	str r3, [r1]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r1, r0, r3
	mov r2, ip
	str r2, [r1]
	adds r4, r0, r4
	ldr r2, [r4]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	adds r1, r5, #0
	bl sub_803AD80
_08003C3A:
	cmp r6, #0
	beq _08003C84
	ldr r0, [r7]
	movs r4, #0x98
	lsls r4, r4, #1
	adds r1, r0, r4
	ldr r2, [r1]
	movs r5, #0x10
	ldrsh r1, [r2, r5]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	adds r1, r6, #0
	bl sub_803AD80
	adds r1, r0, #0
	movs r0, #0xf0
	subs r0, r0, r1
	asrs r3, r0, #1
	ldr r0, [r7]
	movs r2, #0x91
	movs r5, #0x88
	lsls r5, r5, #1
	adds r1, r0, r5
	str r3, [r1]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r1, r0, r3
	str r2, [r1]
	adds r4, r0, r4
	ldr r2, [r4]
	movs r5, #0x20
	ldrsh r1, [r2, r5]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	adds r1, r6, #0
	bl sub_803AD80
_08003C84:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08003C8C: .4byte gUnknown_030012DC
.endif

@ sub_8003C90 is reconstructed (semantics understood) but NOT YET
@ byte-matching - parked as C in src/graphics/settings_menu.c, guarded
@ by #if NON_MATCHING. See docs/matching.md for this chunk's write-up.
.if NON_MATCHING == 0
	thumb_func_start sub_8003C90
sub_8003C90: @ 0x08003C90
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r2, r0, #0
	lsls r1, r1, #0x18
	cmp r1, #0
	beq _08003CC0
	ldr r0, _08003CBC @ =gUnknown_030012DC
	ldr r3, [r0]
	ldr r0, [r2, #4]
	asrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	movs r1, #2
	cmp r0, #0
	beq _08003CB2
	movs r1, #1
_08003CB2:
	adds r0, r3, #0
	bl sub_8028A30
	b _08003CCA
	.align 2, 0
_08003CBC: .4byte gUnknown_030012DC
_08003CC0:
	ldr r0, _08003D38 @ =gUnknown_030012DC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8028A30
_08003CCA:
	ldr r0, _08003D38 @ =gUnknown_030012DC
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
	movs r0, #0x23
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r6, #4]
	adds r0, r4, #0
	bl sub_803AD80
	movs r1, #0xf0
	subs r1, r1, r0
	asrs r1, r1, #1
	mov r3, r8
	ldr r4, [r3]
	movs r2, #0x87
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
	movs r0, #0x23
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08003D38: .4byte gUnknown_030012DC

.endif
