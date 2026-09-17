.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_80255D4
sub_80255D4: @ 0x080255D4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x14
	adds r6, r0, #0
	mov sb, r2
	adds r5, r3, #0
	movs r3, #0
	ldr r0, [r6]
	cmp r1, r0
	beq _08025616
	str r1, [r6]
	str r3, [sp]
	ldr r0, _080256DC @ =0x040000D4
	mov r1, sp
	str r1, [r0]
	adds r1, r6, #0
	adds r1, #8
	str r1, [r0, #4]
	ldr r2, _080256E0 @ =0x85000010
	str r2, [r0, #8]
	ldr r1, [r0, #8]
	str r3, [sp]
	mov r3, sp
	str r3, [r0]
	movs r4, #0x82
	lsls r4, r4, #2
	adds r1, r6, r4
	str r1, [r0, #4]
	str r2, [r0, #8]
	ldr r0, [r0, #8]
_08025616:
	adds r0, r6, #0
	adds r0, #8
	movs r2, #0x84
	lsls r2, r2, #1
	adds r1, r6, r2
	ldr r4, _080256E4 @ =0x04000040
	adds r2, r4, #0
	bl sub_803A94C
	movs r3, #0x82
	lsls r3, r3, #2
	adds r0, r6, r3
	movs r2, #0xc2
	lsls r2, r2, #2
	adds r1, r6, r2
	adds r2, r4, #0
	bl sub_803A94C
	asrs r0, r5, #8
	str r0, [r6, #4]
	movs r7, #0
	ldr r0, [r6]
	ldrh r2, [r0, #2]
	subs r2, #1
	cmp r2, #0
	blt _0802568C
_0802564A:
	ldr r0, [r6]
	lsls r1, r2, #3
	ldr r0, [r0, #4]
	adds r5, r0, r1
	movs r4, #0
	subs r2, #1
	mov r8, r2
	ldrh r3, [r5, #2]
	cmp r4, r3
	bge _08025686
_0802565E:
	adds r0, r6, #0
	adds r1, r7, #0
	bl sub_8025968
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0802567C
	ldr r0, _080256E8 @ =gUnknown_030012E4
	ldr r0, [r0]
	lsls r1, r4, #3
	ldr r2, [r5, #4]
	adds r2, r2, r1
	adds r1, r7, #0
	bl sub_8025D28
_0802567C:
	adds r7, #1
	adds r4, #1
	ldrh r0, [r5, #2]
	cmp r4, r0
	blt _0802565E
_08025686:
	mov r2, r8
	cmp r2, #0
	bge _0802564A
_0802568C:
	mov r1, sb
	cmp r1, #0
	bne _08025694
	b _08025884
_08025694:
	adds r1, #4
	mov sb, r1
	subs r1, #4
	ldm r1!, {r2}
	mov r8, r2
	ldr r0, _080256EC @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r0, [r0]
	subs r2, r0, #1
	cmp r2, #0
	blt _08025762
_080256AA:
	ldr r0, _080256EC @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r2, #2
	adds r0, r0, r1
	ldr r7, [r0]
	ldrh r4, [r7, #8]
	movs r3, #0
	subs r2, #1
	str r2, [sp, #0x10]
	cmp r3, r8
	bge _0802575C
	mov r1, sb
_080256C4:
	ldr r0, [r1]
	cmp r4, r0
	bne _08025754
	movs r6, #0
	ldr r5, [r1, #4]
	ldr r3, _080256EC @ =gUnknown_0300130C
	mov sl, r3
_080256D2:
	ldr r0, _080256EC @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r0, [r0]
	subs r2, r0, #1
	b _080256F2
	.align 2, 0
_080256DC: .4byte 0x040000D4
_080256E0: .4byte 0x85000010
_080256E4: .4byte 0x04000040
_080256E8: .4byte gUnknown_030012E4
_080256EC: .4byte gUnknown_0300130C
_080256F0:
	subs r2, #1
_080256F2:
	cmp r2, #0
	blt _0802571A
	mov r4, sl
	ldr r0, [r4]
	ldr r1, [r0, #8]
	lsls r0, r2, #2
	adds r0, r0, r1
	ldr r4, [r0]
	ldrh r0, [r4, #8]
	cmp r5, r0
	bne _080256F0
	adds r0, r7, #0
	adds r1, r4, #0
	bl sub_8010714
	adds r0, r4, #0
	adds r1, r7, #0
	bl sub_8010710
	movs r6, #1
_0802571A:
	cmp r6, #0
	bne _0802575C
	movs r3, #1
	movs r2, #0
	cmp r6, r8
	bge _08025748
	mov r1, sb
	ldr r0, [r1]
	cmp r5, r0
	bne _08025732
	ldr r5, [r1, #4]
	b _0802574E
_08025732:
	adds r2, #1
	cmp r2, r8
	bge _08025748
	lsls r0, r2, #3
	mov r4, sb
	adds r1, r0, r4
	ldr r0, [r1]
	cmp r5, r0
	bne _08025732
	ldr r5, [r1, #4]
	movs r3, #0
_08025748:
	cmp r3, #0
	beq _0802574E
	movs r6, #1
_0802574E:
	cmp r6, #0
	beq _080256D2
	b _0802575C
_08025754:
	adds r1, #8
	adds r3, #1
	cmp r3, r8
	blt _080256C4
_0802575C:
	ldr r2, [sp, #0x10]
	cmp r2, #0
	bge _080256AA
_08025762:
	movs r3, #0
	cmp r3, r8
	blt _0802576A
	b _08025884
_0802576A:
	lsls r0, r3, #3
	mov r2, sb
	adds r1, r0, r2
	ldrh r5, [r1]
	movs r7, #0
	movs r4, #0
	ldr r1, _080257AC @ =gUnknown_0300130C
	ldr r1, [r1]
	ldr r2, [r1]
	adds r6, r0, #0
	adds r3, #1
	str r3, [sp, #0xc]
	cmp r7, r2
	bge _08025798
	ldr r1, [r1, #8]
_08025788:
	ldr r0, [r1]
	ldrh r0, [r0, #8]
	cmp r0, r5
	beq _0802587C
	adds r1, #4
	adds r4, #1
	cmp r4, r2
	blt _08025788
_08025798:
	cmp r7, #0
	bne _0802587C
	mov r3, sb
	adds r0, r6, r3
	ldrh r5, [r0, #4]
	movs r6, #0
	ldr r4, _080257AC @ =gUnknown_0300130C
	mov sl, r4
	b _080257B2
	.align 2, 0
_080257AC: .4byte gUnknown_0300130C
_080257B0:
	mov r5, ip
_080257B2:
	movs r7, #1
	movs r0, #0
	mov ip, r0
	movs r2, #0
	mov r1, r8
	cmp r1, #0
	ble _08025800
	mov r1, sb
_080257C2:
	ldr r0, [r1]
	cmp r5, r0
	bne _080257F8
	movs r7, #0
	ldrh r1, [r1, #4]
	mov ip, r1
	movs r3, #0
	ldr r2, _080257F4 @ =gUnknown_0300130C
	ldr r0, [r2]
	ldr r0, [r0]
	cmp r7, r0
	bge _08025800
	mov r4, sl
	ldr r0, [r4]
	ldr r2, [r0]
	ldr r0, [r0, #8]
_080257E2:
	ldr r6, [r0]
	ldrh r1, [r6, #8]
	cmp r1, r5
	beq _08025844
	adds r0, #4
	adds r3, #1
	cmp r3, r2
	blt _080257E2
	b _08025800
	.align 2, 0
_080257F4: .4byte gUnknown_0300130C
_080257F8:
	adds r1, #8
	adds r2, #1
	cmp r2, r8
	blt _080257C2
_08025800:
	movs r0, #0
	cmp r0, #0
	bne _08025844
	cmp r7, #0
	beq _08025838
	movs r4, #0
	ldr r1, _0802582C @ =gUnknown_0300130C
	ldr r0, [r1]
	ldr r0, [r0]
	cmp r4, r0
	bge _08025838
	mov r2, sl
	ldr r0, [r2]
	ldr r3, [r0]
	ldr r2, [r0, #8]
_0802581E:
	ldr r1, [r2]
	ldrh r0, [r1, #8]
	cmp r0, r5
	bne _08025830
	adds r6, r1, #0
	b _08025844
	.align 2, 0
_0802582C: .4byte gUnknown_0300130C
_08025830:
	adds r2, #4
	adds r4, #1
	cmp r4, r3
	blt _0802581E
_08025838:
	movs r3, #0
	cmp r3, #0
	bne _08025844
	cmp r7, #0
	beq _080257B0
	b _0802587C
_08025844:
	cmp r6, #0
	beq _0802587C
	ldr r1, [r6, #0x18]
	movs r4, #0x10
	ldrsh r0, [r1, r4]
	adds r0, r6, r0
	ldr r1, [r1, #0x14]
	bl sub_803AD7C
	ldrb r0, [r0, #5]
	adds r0, #1
	lsls r5, r0, #8
	add r4, sp, #4
_0802585E:
	ldr r0, [r6]
	str r0, [sp, #4]
	ldr r2, [r6, #4]
	adds r2, r2, r5
	str r2, [r4, #4]
	ldr r1, [sp, #4]
	adds r0, r6, #0
	bl sub_8007398
	adds r0, r6, #0
	bl sub_801070C
	adds r6, r0, #0
	cmp r6, #0
	bne _0802585E
_0802587C:
	ldr r3, [sp, #0xc]
	cmp r3, r8
	bge _08025884
	b _0802576A
_08025884:
	add sp, #0x14
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8025894
sub_8025894: @ 0x08025894
	push {r4, r5, r6, lr}
	adds r5, r1, #0
	movs r6, #0
	ldrh r2, [r5, #2]
	subs r2, #1
	cmp r2, #0
	blt _0802593A
_080258A2:
	lsls r1, r2, #3
	ldr r0, [r5, #4]
	adds r4, r0, r1
	movs r3, #0
	subs r2, #1
	b _08025930
_080258AE:
	lsls r1, r3, #3
	ldr r0, [r4, #4]
	adds r1, r0, r1
	ldrh r0, [r1]
	cmp r0, #0x1a
	bne _080258CC
	ldr r0, [r5, #8]
	ldrh r1, [r1, #6]
	lsls r1, r1, #1
	adds r1, r1, r0
	ldr r0, [r5, #0xc]
	ldrh r1, [r1]
	adds r0, r1, r0
	movs r1, #8
	ldrsh r0, [r0, r1]
_080258CC:
	subs r0, #0x15
	cmp r0, #0x12
	bhi _0802592E
	lsls r0, r0, #2
	ldr r1, _080258DC @ =_080258E0
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080258DC: .4byte _080258E0
_080258E0: @ jump table
	.4byte _0802592C @ case 0
	.4byte _0802592C @ case 1
	.4byte _0802592C @ case 2
	.4byte _0802592E @ case 3
	.4byte _0802592C @ case 4
	.4byte _0802592E @ case 5
	.4byte _0802592E @ case 6
	.4byte _0802592E @ case 7
	.4byte _0802592E @ case 8
	.4byte _0802592C @ case 9
	.4byte _0802592C @ case 10
	.4byte _0802592C @ case 11
	.4byte _0802592C @ case 12
	.4byte _0802592C @ case 13
	.4byte _0802592C @ case 14
	.4byte _0802592C @ case 15
	.4byte _0802592C @ case 16
	.4byte _0802592C @ case 17
	.4byte _0802592C @ case 18
_0802592C:
	adds r6, #1
_0802592E:
	adds r3, #1
_08025930:
	ldrh r0, [r4, #2]
	cmp r3, r0
	blt _080258AE
	cmp r2, #0
	bge _080258A2
_0802593A:
	adds r0, r6, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
	thumb_func_start sub_8025944
sub_8025944: @ 0x08025944
	adds	r2, r0, #0
	adds	r3, r1, #0
	adds	r0, r3, #0
	cmp	r3, #0
	bge _08025950
	adds	r0, #31
_08025950:
	asrs	r0, r0, #5
	lsls	r1, r0, #2
	adds	r2, #8
	adds	r2, r2, r1
	lsls	r0, r0, #5
	subs	r0, r3, r0
	movs	r1, #1
	lsls	r1, r0
	ldr	r0, [r2, #0]
	orrs	r0, r1
	str	r0, [r2, #0]
	bx	lr

	thumb_func_start sub_8025968
sub_8025968: @ 0x08025968
	push {r4, lr}
	adds r2, r0, #0
	adds r3, r1, #0
	movs r4, #0
	adds r0, r3, #0
	cmp r3, #0
	bge _08025978
	adds r0, #0x1f
_08025978:
	asrs r0, r0, #5
	lsls r1, r0, #2
	adds r2, #8
	adds r2, r2, r1
	lsls r0, r0, #5
	subs r0, r3, r0
	movs r1, #1
	lsls r1, r0
	ldr r0, [r2]
	ands r0, r1
	cmp r0, #0
	beq _08025992
	movs r4, #1
_08025992:
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_802599C
sub_802599C: @ 0x0802599C
	push {r4, r5, lr}
	adds r2, r0, #0
	adds r3, r1, #0
	movs r4, #0
	adds r0, r3, #0
	cmp r3, #0
	bge _080259AC
	adds r0, #0x1f
_080259AC:
	asrs r0, r0, #5
	lsls r1, r0, #2
	movs r5, #0x82
	lsls r5, r5, #2
	adds r2, r2, r5
	adds r2, r2, r1
	lsls r0, r0, #5
	subs r0, r3, r0
	movs r1, #1
	lsls r1, r0
	ldr r0, [r2]
	ands r0, r1
	cmp r0, #0
	beq _080259CA
	movs r4, #1
_080259CA:
	adds r0, r4, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_80259D4
sub_80259D4: @ 0x080259D4
	mov ip, r0
	adds r2, r1, #0
	adds r0, r2, #0
	cmp r2, #0
	bge _080259E0
	adds r0, #0x1f
_080259E0:
	asrs r0, r0, #5
	lsls r3, r0, #2
	movs r1, #0x82
	lsls r1, r1, #2
	add r1, ip
	adds r1, r1, r3
	lsls r0, r0, #5
	subs r0, r2, r0
	movs r2, #1
	lsls r2, r0
	ldr r0, [r1]
	orrs r0, r2
	str r0, [r1]
	movs r1, #0xc2
	lsls r1, r1, #2
	add r1, ip
	adds r1, r1, r3
	ldr r0, [r1]
	orrs r0, r2
	str r0, [r1]
	bx lr
	.align 2, 0

	thumb_func_start sub_8025A0C
sub_8025A0C: @ 0x08025A0C
	push {r4, lr}
	adds r2, r0, #0
	adds r3, r1, #0
	adds r0, r3, #0
	cmp r3, #0
	bge _08025A1A
	adds r0, #0x1f
_08025A1A:
	asrs r0, r0, #5
	lsls r1, r0, #2
	movs r4, #0xc2
	lsls r4, r4, #2
	adds r2, r2, r4
	adds r2, r2, r1
	lsls r0, r0, #5
	subs r0, r3, r0
	movs r1, #1
	lsls r1, r0
	ldr r0, [r2]
	orrs r0, r1
	str r0, [r2]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
	thumb_func_start sub_8025A3C
sub_8025A3C: @ 0x08025A3C
	asrs r1, r1, #8
	str r1, [r0, #4]
	bx lr

	thumb_func_start sub_8025A44
sub_8025A44: @ 0x08025A44
	push {lr}
	adds r2, r0, #0
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08025A56
	adds r0, r2, #0
	bl sub_8026ED0
_08025A56:
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8025A5C
sub_8025A5C: @ 0x08025A5C
	movs r1, #0
	str r1, [r0]
	str r1, [r0, #4]
	bx lr

	thumb_func_start sub_8025A64
sub_8025A64: @ 0x08025A64
	push {r4, r5, r6, r7, lr}
	adds r6, r3, #0
	add r0, sp, #0x18
	ldrb r7, [r0]
	movs r4, #0
	ldr r0, _08025B00 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r0, #0x8c
	ldrb r5, [r0]
	cmp r5, #0
	bne _08025AF6
	ldr r0, _08025B04 @ =0x0000FFFF
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	movs r3, #0
	bl sub_8011114
	adds r4, r0, #0
	movs r0, #0x10
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	adds r0, r4, #0
	adds r0, #0x49
	strb r6, [r0]
	adds r1, r4, #0
	adds r1, #0x4a
	ldr r0, [sp, #0x14]
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x4b
	strb r5, [r0]
	ldr r0, _08025B08 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r3, #0x8d
	lsls r3, r3, #2
	adds r0, r0, r3
	str r0, [r4, #0x20]
	movs r0, #0xa
	subs r1, #0x1d
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	cmp r7, #0
	beq _08025AF6
	adds r0, r4, #0
	bl sub_80111B8
_08025AF6:
	adds r0, r4, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08025B00: .4byte gUnknown_030012C0
_08025B04: .4byte 0x0000FFFF
_08025B08: .4byte gUnknown_030012D0

	thumb_func_start sub_8025B0C
sub_8025B0C: @ 0x08025B0C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x28
	mov r8, r3
	ldr r7, [sp, #0x44]
	ldr r6, [sp, #0x48]
	ldr r3, [r6]
	asrs r3, r3, #8
	ldr r5, [r6, #4]
	asrs r5, r5, #8
	adds r4, r6, #0
	adds r4, #0x28
	ldrb r4, [r4]
	lsls r4, r4, #0x1b
	lsrs r4, r4, #0x1f
	str r5, [sp]
	str r4, [sp, #4]
	bl sub_8025BAC
	adds r5, r0, #0
	add r0, sp, #8
	adds r1, r5, #0
	bl sub_8007B98
	ldr r4, [sp, #0x10]
	add r0, sp, #0x18
	adds r1, r6, #0
	bl sub_8007B98
	ldr r0, [sp, #0x20]
	lsrs r1, r4, #0x1f
	adds r4, r4, r1
	asrs r4, r4, #1
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	adds r4, r4, r0
	add r4, r8
	ldr r0, [r5]
	asrs r1, r0, #8
	adds r3, r5, #0
	adds r3, #0x28
	ldrb r2, [r3]
	lsls r0, r2, #0x1b
	adds r2, r1, r4
	cmp r0, #0
	bge _08025B6E
	subs r2, r1, r4
_08025B6E:
	ldr r0, [r5, #4]
	asrs r0, r0, #8
	ldr r1, [sp, #0x40]
	adds r0, r0, r1
	lsls r1, r2, #8
	str r1, [r5]
	lsls r0, r0, #8
	str r0, [r5, #4]
	ldrb r3, [r3]
	lsls r0, r3, #0x1b
	cmp r0, #0
	bge _08025B94
	rsbs r0, r7, #0
	movs r1, #0x40
	str r0, [r5, #0x60]
	str r0, [r5, #0x48]
	str r1, [r5, #0x4c]
	str r0, [r5, #0x50]
	b _08025B9E
_08025B94:
	movs r0, #0x40
	str r7, [r5, #0x60]
	str r7, [r5, #0x48]
	str r0, [r5, #0x4c]
	str r7, [r5, #0x50]
_08025B9E:
	adds r0, r5, #0
	add sp, #0x28
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_8025BAC
sub_8025BAC: @ 0x08025BAC
	push {r4, r5, r6, r7, lr}
	adds r5, r1, #0
	adds r7, r2, #0
	ldr r2, [sp, #0x14]
	ldr r6, [sp, #0x18]
	cmp r3, #0
	bge _08025BBC
	movs r3, #0
_08025BBC:
	ldr r0, _08025C94 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r1, [r0, #0x10]
	ldr r0, [r1, #0x10]
	lsls r4, r0, #8
	asrs r0, r4, #8
	cmp r3, r0
	blt _08025BD0
	lsrs r0, r4, #8
	subs r3, r0, #1
_08025BD0:
	cmp r2, #0
	bge _08025BD6
	movs r2, #0
_08025BD6:
	ldr r0, [r1, #0x14]
	lsls r4, r0, #8
	asrs r0, r4, #8
	cmp r2, r0
	blt _08025BE4
	lsrs r0, r4, #8
	subs r2, r0, #1
_08025BE4:
	ldr r0, _08025C98 @ =0x0000FFFF
	lsls r1, r3, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	movs r3, #0
	bl sub_8009ED0
	adds r4, r0, #0
	rsbs r1, r6, #0
	orrs r1, r6
	adds r2, r4, #0
	adds r2, #0x28
	lsrs r1, r1, #0x1f
	lsls r1, r1, #4
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r3, [r2]
	ands r0, r3
	orrs r0, r1
	strb r0, [r2]
	ldr r0, _08025C9C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	lsls r1, r5, #1
	adds r1, r1, r5
	lsls r1, r1, #2
	ldr r0, [r0]
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	strb r7, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x10
	bl sub_8026EDC
	bl sub_800CCE0
	str r0, [r4, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r1, [r4, #0xc]
	ands r0, r1
	movs r1, #3
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _08025CA0 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	adds r0, r4, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08025C94: .4byte gUnknown_03001308
_08025C98: .4byte 0x0000FFFF
_08025C9C: .4byte gUnknown_030012D0
_08025CA0: .4byte gUnknown_030012F0

	thumb_func_start sub_8025CA4
sub_8025CA4: @ 0x08025CA4
	push {r4, r5, r6, r7, lr}
	adds r7, r3, #0
	ldr r5, [sp, #0x14]
	add r0, sp, #0x18
	ldrb r6, [r0]
	movs r4, #0
	ldr r0, _08025CD4 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _08025D1A
	cmp r6, #0
	bne _08025CC4
	cmp r5, #0xff
	bne _08025CDC
_08025CC4:
	ldr r3, _08025CD8 @ =0x0000FFFF
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	adds r0, r3, #0
	b _08025CE8
	.align 2, 0
_08025CD4: .4byte gUnknown_030012C0
_08025CD8: .4byte 0x0000FFFF
_08025CDC:
	ldr r0, _08025D24 @ =0x0000FFFF
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	movs r3, #0
_08025CE8:
	bl sub_801173C
	adds r4, r0, #0
	movs r0, #0x10
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	adds r1, r4, #0
	adds r1, #0x49
	movs r0, #0
	strb r7, [r1]
	adds r1, #1
	strb r5, [r1]
	adds r1, #1
	strb r0, [r1]
	cmp r5, #0xff
	bne _08025D10
	adds r0, r4, #0
	bl sub_801191C
_08025D10:
	cmp r6, #0
	beq _08025D1A
	adds r0, r4, #0
	bl sub_8011870
_08025D1A:
	adds r0, r4, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08025D24: .4byte 0x0000FFFF

	thumb_func_start sub_8025D28
sub_8025D28: @ 0x08025D28
	push {r4, r5, r6, lr}
	adds r6, r1, #0
	ldr r1, [r0]
	ldrh r3, [r2]
	lsls r0, r3, #2
	adds r0, r0, r1
	ldrh r1, [r2, #2]
	ldrh r4, [r2, #4]
	ldrh r3, [r2, #6]
	ldr r5, [r0]
	adds r0, r6, #0
	adds r2, r4, #0
	bl sub_803AD8C
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8025D4C
sub_8025D4C: @ 0x08025D4C
	str r2, [r0, #4]
	str r1, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8025D54
sub_8025D54: @ 0x08025D54
	push {lr}
	adds r2, r0, #0
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08025D66
	adds r0, r2, #0
	bl sub_8026ED0
_08025D66:
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8025D6C
sub_8025D6C: @ 0x08025D6C
	movs r1, #0
	str r1, [r0]
	str r1, [r0, #4]
	bx lr

	thumb_func_start sub_8025D74
sub_8025D74: @ 0x08025D74
	push {r4, r5, lr}
	adds r5, r0, #0
	adds r4, r1, #0
	bl sub_8024DAC
	ldr r0, _08025DDC @ =gStaticData_087E4C14
	str r0, [r5, #0x30]
	adds r1, r4, #0
	adds r1, #0x1c
	lsls r0, r1, #0xb
	movs r2, #0xc0
	lsls r2, r2, #0x13
	adds r0, r0, r2
	str r0, [r5, #0x4c]
	lsls r0, r4, #1
	ldr r3, _08025DE0 @ =0x04000008
	adds r0, r0, r3
	str r0, [r5, #0x38]
	lsls r4, r4, #2
	ldr r0, _08025DE4 @ =0x04000010
	adds r4, r4, r0
	str r4, [r5, #0x58]
	movs r0, #0
	strh r0, [r5, #0x34]
	adds r2, r5, #0
	adds r2, #0x34
	movs r0, #0x7f
	ldrb r3, [r2]
	ands r0, r3
	strb r0, [r2]
	adds r3, r5, #0
	adds r3, #0x35
	movs r0, #0x1f
	ands r1, r0
	movs r0, #0x20
	rsbs r0, r0, #0
	ldrb r4, [r3]
	ands r0, r4
	orrs r0, r1
	strb r0, [r3]
	movs r0, #0xd
	rsbs r0, r0, #0
	ldrb r1, [r2]
	ands r0, r1
	movs r1, #8
	orrs r0, r1
	strb r0, [r2]
	adds r0, r5, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08025DDC: .4byte gStaticData_087E4C14
_08025DE0: .4byte 0x04000008
_08025DE4: .4byte 0x04000010

	thumb_func_start sub_8025DE8
sub_8025DE8: @ 0x08025DE8
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	b _08025E04
_08025DF2:
	subs r1, #1
	str r1, [r4, #0x3c]
	ldr r2, [r4, #0x30]
	movs r3, #0x30
	ldrsh r0, [r2, r3]
	adds r0, r4, r0
	ldr r2, [r2, #0x34]
	bl sub_803AD80
_08025E04:
	ldr r1, [r4, #0x3c]
	cmp r1, r5
	bgt _08025DF2
	b _08025E1E
_08025E0C:
	adds r1, #1
	str r1, [r4, #0x40]
	ldr r2, [r4, #0x30]
	movs r3, #0x30
	ldrsh r0, [r2, r3]
	adds r0, r4, r0
	ldr r2, [r2, #0x34]
	bl sub_803AD80
_08025E1E:
	ldr r1, [r4, #0x40]
	cmp r1, r6
	blt _08025E0C
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8025E2C
sub_8025E2C: @ 0x08025E2C
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	b _08025E48
_08025E36:
	subs r1, #1
	str r1, [r4, #0x44]
	ldr r2, [r4, #0x30]
	movs r3, #0x38
	ldrsh r0, [r2, r3]
	adds r0, r4, r0
	ldr r2, [r2, #0x3c]
	bl sub_803AD80
_08025E48:
	ldr r1, [r4, #0x44]
	cmp r1, r5
	bgt _08025E36
	b _08025E62
_08025E50:
	adds r1, #1
	str r1, [r4, #0x48]
	ldr r2, [r4, #0x30]
	movs r3, #0x38
	ldrsh r0, [r2, r3]
	adds r0, r4, r0
	ldr r2, [r2, #0x3c]
	bl sub_803AD80
_08025E62:
	ldr r1, [r4, #0x48]
	cmp r1, r6
	blt _08025E50
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8025E70
sub_8025E70: @ 0x08025E70
	adds r3, r0, #0
	ldr r0, [r3, #0x44]
	cmp r0, r1
	bge _08025E7A
	str r1, [r3, #0x44]
_08025E7A:
	ldr r0, [r3, #0x48]
	cmp r0, r2
	ble _08025E82
	str r2, [r3, #0x48]
_08025E82:
	bx lr

	thumb_func_start sub_8025E84
sub_8025E84: @ 0x08025E84
	adds r3, r0, #0
	ldr r0, [r3, #0x3c]
	cmp r0, r1
	bge _08025E8E
	str r1, [r3, #0x3c]
_08025E8E:
	ldr r0, [r3, #0x40]
	cmp r0, r2
	ble _08025E96
	str r2, [r3, #0x40]
_08025E96:
	bx lr

	thumb_func_start sub_8025E98
sub_8025E98: @ 0x08025E98
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	bl sub_8024E68
	ldr r0, [r5]
	adds r1, r0, #0
	cmp r0, #0
	bge _08025EAE
	adds r1, r0, #7
_08025EAE:
	asrs r1, r1, #3
	mov r8, r1
	adds r2, r0, #0
	adds r2, #0xef
	cmp r2, #0
	bge _08025EBC
	adds r2, #7
_08025EBC:
	asrs r7, r2, #3
	ldr r0, [r5, #4]
	adds r1, r0, #0
	cmp r0, #0
	bge _08025EC8
	adds r1, r0, #7
_08025EC8:
	asrs r6, r1, #3
	adds r4, r0, #0
	adds r4, #0x9f
	cmp r4, #0
	bge _08025ED4
	adds r4, #7
_08025ED4:
	asrs r4, r4, #3
	ldr r1, [r5, #0x30]
	adds r1, #0x48
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	adds r2, r4, #0
	bl sub_803AD84
	ldr r1, [r5, #0x30]
	adds r1, #0x40
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	mov r1, r8
	adds r2, r7, #0
	bl sub_803AD84
	ldr r0, [r5, #0x2c]
	adds r1, r5, #0
	bl sub_8024AA0
	adds r0, r5, #0
	mov r1, r8
	adds r2, r7, #0
	bl sub_8025E2C
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r4, #0
	bl sub_8025DE8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8025F24
sub_8025F24: @ 0x08025F24
	adds r2, r0, #0
	ldr r1, [r2]
	adds r0, #0x54
	strh r1, [r0]
	ldr r0, [r2, #4]
	adds r1, r2, #0
	adds r1, #0x56
	strh r0, [r1]
	ldr r1, [r2, #0x58]
	ldr r0, [r2, #0x54]
	str r0, [r1]
	bx lr

	thumb_func_start sub_8025F3C
sub_8025F3C: @ 0x08025F3C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r5, r0, #0
	adds r4, r1, #0
	ldr r0, [r5, #0x2c]
	ldr r2, [r5, #0x3c]
	mov r3, sp
	bl sub_8024B18
	mov ip, r0
	ldr r2, [r5, #0x3c]
	adds r0, r2, #0
	cmp r2, #0
	bge _08025F5E
	adds r0, #0x1f
_08025F5E:
	asrs r0, r0, #5
	lsls r0, r0, #5
	subs r3, r2, r0
	adds r0, r4, #0
	cmp r4, #0
	bge _08025F6C
	adds r0, #0x1f
_08025F6C:
	asrs r1, r0, #5
	lsls r0, r1, #5
	subs r1, r4, r0
	lsls r0, r3, #5
	adds r3, r0, r1
	adds r4, r2, #0
	ldr r0, [r5, #0x40]
	cmp r4, r0
	bgt _08025FB6
	ldr r6, [r5, #0x4c]
	ldr r2, [sp]
	movs r1, #0x1f
	mov r8, r1
	adds r5, r0, #0
_08025F88:
	lsls r0, r3, #1
	adds r0, r0, r6
	lsls r1, r2, #7
	add r1, ip
	ldrh r1, [r1]
	strh r1, [r0]
	adds r2, #1
	mov r7, r8
	ands r2, r7
	adds r1, r3, #0
	adds r1, #0x20
	adds r0, r1, #0
	cmp r1, #0
	bge _08025FA8
	ldr r7, _08025FC4 @ =0x0000041F
	adds r0, r3, r7
_08025FA8:
	asrs r3, r0, #0xa
	lsls r0, r3, #0xa
	subs r3, r1, r0
	adds r4, #1
	cmp r4, r5
	ble _08025F88
	str r2, [sp]
_08025FB6:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08025FC4: .4byte 0x0000041F

	thumb_func_start sub_8025FC8
sub_8025FC8: @ 0x08025FC8
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r4, r0, #0
	adds r2, r1, #0
	adds r0, r2, #0
	cmp r2, #0
	bge _08025FD8
	adds r0, #0x1f
_08025FD8:
	asrs r0, r0, #5
	lsls r0, r0, #5
	subs r0, r2, r0
	lsls r1, r0, #6
	ldr r0, [r4, #0x4c]
	adds r6, r0, r1
	ldr r0, [r4, #0x2c]
	ldr r1, [r4, #0x44]
	mov r3, sp
	bl sub_8024B48
	adds r5, r0, #0
	ldr r3, [r4, #0x44]
	ldr r0, [r4, #0x48]
	cmp r3, r0
	bgt _08026024
	movs r7, #0x3f
	adds r4, r0, #0
_08025FFC:
	adds r0, r3, #0
	cmp r3, #0
	bge _08026004
	adds r0, #0x1f
_08026004:
	asrs r0, r0, #5
	lsls r0, r0, #5
	subs r0, r3, r0
	lsls r2, r0, #1
	adds r2, r2, r6
	ldr r1, [sp]
	lsls r0, r1, #1
	adds r0, r0, r5
	ldrh r0, [r0]
	strh r0, [r2]
	adds r1, #1
	ands r1, r7
	str r1, [sp]
	adds r3, #1
	cmp r3, r4
	ble _08025FFC
_08026024:
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_802602C
sub_802602C: @ 0x0802602C
	push {r4, r5, lr}
	adds r5, r0, #0
	ldr r1, [r5]
	adds r0, r1, #0
	cmp r1, #0
	bge _0802603A
	adds r0, r1, #7
_0802603A:
	asrs r0, r0, #3
	str r0, [r5, #0x44]
	adds r0, r1, #0
	adds r0, #0xef
	cmp r0, #0
	bge _08026048
	adds r0, #7
_08026048:
	asrs r0, r0, #3
	str r0, [r5, #0x48]
	ldr r2, [r5, #4]
	adds r0, r2, #0
	cmp r2, #0
	bge _08026056
	adds r0, r2, #7
_08026056:
	asrs r1, r0, #3
	str r1, [r5, #0x3c]
	adds r0, r2, #0
	adds r0, #0x9f
	cmp r0, #0
	bge _08026064
	adds r0, #7
_08026064:
	asrs r0, r0, #3
	str r0, [r5, #0x40]
	adds r4, r1, #0
	cmp r4, r0
	bgt _08026086
_0802606E:
	ldr r1, [r5, #0x30]
	movs r2, #0x30
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x34]
	adds r1, r4, #0
	bl sub_803AD80
	adds r4, #1
	ldr r0, [r5, #0x40]
	cmp r4, r0
	ble _0802606E
_08026086:
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_802608C
sub_802608C: @ 0x0802608C
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8024E90
	ldr r1, [r4, #0x30]
	movs r2, #0x28
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #0x2c]
	bl sub_803AD7C
	adds r0, r4, #0
	bl sub_802602C
	ldr r1, [r4, #0x38]
	ldrh r0, [r4, #0x34]
	strh r0, [r1]
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_80260B4
sub_80260B4: @ 0x080260B4
	push {lr}
	ldr r2, [r0, #0x50]
	adds r0, #0x34
	ldrb r0, [r0]
	lsls r1, r0, #0x1c
	lsrs r1, r1, #0x1e
	lsls r1, r1, #0xe
	movs r0, #0xc0
	lsls r0, r0, #0x13
	adds r1, r1, r0
	adds r0, r2, #0
	bl LoadTaggedAsset
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80260D4
sub_80260D4: @ 0x080260D4
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	bl sub_8024EB4
	adds r0, r4, #0
	adds r0, #0x28
	ldrb r0, [r0]
	cmp r0, #0
	beq _08026102
	ldr r0, [r5, #8]
	str r0, [r4, #0x50]
	adds r2, r4, #0
	adds r2, #0x34
	movs r1, #3
	ldrh r5, [r5, #0x14]
	ands r1, r5
	movs r0, #4
	rsbs r0, r0, #0
	ldrb r3, [r2]
	ands r0, r3
	orrs r0, r1
	strb r0, [r2]
_08026102:
	pop {r4, r5}
	pop {r0}
	bx r0
	thumb_func_start sub_8026108
sub_8026108: @ 0x08026108
	adds	r0, r2, #0
	cmp	r2, #0
	bge _08026110
	adds	r0, #31
_08026110:
	asrs	r0, r0, #5
	lsls	r0, r0, #5
	subs	r3, r2, r0
	adds	r0, r1, #0
	cmp	r1, #0
	bge _0802611E
	adds	r0, #31
_0802611E:
	asrs	r2, r0, #5
	lsls	r0, r2, #5
	subs	r2, r1, r0
	lsls	r0, r3, #5
	adds	r0, r0, r2
	bx	lr
	movs	r0, r0
	adds	r0, r1, #0
	cmp	r1, #0
	bge _08026134
	adds	r0, #31
_08026134:
	asrs	r0, r0, #5
	lsls	r0, r0, #5
	subs	r0, r1, r0
	bx	lr
	adds	r0, r1, #0

	non_word_aligned_thumb_func_start sub_802613E
sub_802613E: @ 0x0802613E
	cmp r1, #0
	bge _08026144
	adds r0, #0x1f
_08026144:
	asrs r0, r0, #5
	lsls r0, r0, #5
	subs r0, r1, r0
	bx lr

	thumb_func_start sub_802614C
sub_802614C: @ 0x0802614C
	adds r0, #0x35
	movs r2, #0x1f
	ands r1, r2
	movs r2, #0x20
	rsbs r2, r2, #0
	ldrb r3, [r0]
	ands r2, r3
	orrs r2, r1
	strb r2, [r0]
	bx lr

	thumb_func_start sub_8026160
sub_8026160: @ 0x08026160
	adds r0, #0x34
	movs r2, #3
	ands r1, r2
	movs r2, #4
	rsbs r2, r2, #0
	ldrb r3, [r0]
	ands r2, r3
	orrs r2, r1
	strb r2, [r0]
	bx lr

	thumb_func_start sub_8026174
sub_8026174: @ 0x08026174
	adds r0, #0x34
	lsls r1, r1, #7
	movs r2, #0x7f
	ldrb r3, [r0]
	ands r2, r3
	orrs r2, r1
	strb r2, [r0]
	bx lr

	thumb_func_start sub_8026184
sub_8026184: @ 0x08026184
	adds r0, #0x34
	ldrb r0, [r0]
	lsls r0, r0, #0x1c
	lsrs r0, r0, #0x1e
	bx lr
	.align 2, 0

	thumb_func_start sub_8026190
sub_8026190: @ 0x08026190
	adds r0, #0x34
	movs r2, #3
	ands r1, r2
	lsls r1, r1, #2
	movs r2, #0xd
	rsbs r2, r2, #0
	ldrb r3, [r0]
	ands r2, r3
	orrs r2, r1
	strb r2, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_80261A8
sub_80261A8: @ 0x080261A8
	ldr r1, [r0, #0x58]
	ldr r0, [r0, #0x54]
	str r0, [r1]
	bx lr

	thumb_func_start sub_80261B0
sub_80261B0: @ 0x080261B0
	ldr r1, [r0, #0x38]
	ldrh r0, [r0, #0x34]
	strh r0, [r1]
	bx lr

	thumb_func_start sub_80261B8
sub_80261B8: @ 0x080261B8
	push {lr}
	ldr r2, _080261C8 @ =gStaticData_087E4C14
	str r2, [r0, #0x30]
	bl sub_8024D74
	pop {r0}
	bx r0
	.align 2, 0
_080261C8: .4byte gStaticData_087E4C14

	thumb_func_start sub_80261CC
sub_80261CC: @ 0x080261CC
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r6, r0, #0
	adds r4, r1, #0
	ldr r0, [r6, #0x2c]
	ldr r2, [r6, #0x3c]
	mov r3, sp
	bl sub_8024B18
	adds r7, r0, #0
	ldr r2, [r6, #0x3c]
	adds r0, r2, #0
	cmp r2, #0
	bge _080261EA
	adds r0, #0x1f
_080261EA:
	asrs r0, r0, #5
	lsls r0, r0, #5
	subs r3, r2, r0
	adds r0, r4, #0
	cmp r4, #0
	bge _080261F8
	adds r0, #0x1f
_080261F8:
	asrs r1, r0, #5
	lsls r0, r1, #5
	subs r1, r4, r0
	lsls r0, r3, #5
	adds r4, r0, r1
	adds r5, r2, #0
	b _0802623C
_08026206:
	ldr r0, [r6, #0x5c]
	ldr r1, [sp]
	lsls r1, r1, #7
	adds r1, r1, r7
	ldrh r1, [r1]
	bl sub_80264F8
	ldr r2, [r6, #0x4c]
	lsls r1, r4, #1
	adds r1, r1, r2
	strh r0, [r1]
	ldr r0, [sp]
	adds r0, #1
	movs r1, #0x1f
	ands r0, r1
	str r0, [sp]
	adds r1, r4, #0
	adds r1, #0x20
	adds r0, r1, #0
	cmp r1, #0
	bge _08026234
	ldr r2, _0802624C @ =0x0000041F
	adds r0, r4, r2
_08026234:
	asrs r4, r0, #0xa
	lsls r0, r4, #0xa
	subs r4, r1, r0
	adds r5, #1
_0802623C:
	ldr r0, [r6, #0x40]
	cmp r5, r0
	ble _08026206
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802624C: .4byte 0x0000041F

	thumb_func_start sub_8026250
sub_8026250: @ 0x08026250
	adds r0, r1, #0
	movs r1, #8
	rsbs r1, r1, #0
	cmp r0, r1
	bge _0802625C
	adds r0, r1, #0
_0802625C:
	cmp r0, #8
	ble _08026262
	movs r0, #8
_08026262:
	bx lr

	thumb_func_start sub_8026264
sub_8026264: @ 0x08026264
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	ldr r0, [r5, #0x2c]
	ldr r2, [r5, #0x3c]
	mov r3, sp
	bl sub_8024B18
	adds r6, r0, #0
	ldr r4, [r5, #0x3c]
	b _08026294
_0802627A:
	ldr r0, [r5, #0x5c]
	ldr r1, [sp]
	lsls r1, r1, #7
	adds r1, r1, r6
	ldrh r1, [r1]
	bl sub_80265A0
	ldr r0, [sp]
	adds r0, #1
	movs r1, #0x1f
	ands r0, r1
	str r0, [sp]
	adds r4, #1
_08026294:
	ldr r0, [r5, #0x40]
	cmp r4, r0
	ble _0802627A
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80262A4
sub_80262A4: @ 0x080262A4
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r2, r1, #0
	ldr r0, [r5, #0x2c]
	ldr r1, [r5, #0x44]
	mov r3, sp
	bl sub_8024B48
	adds r6, r0, #0
	ldr r4, [r5, #0x44]
	b _080262D8
_080262BC:
	ldr r0, [r5, #0x5c]
	ldr r2, [sp]
	lsls r1, r2, #1
	adds r1, r1, r6
	ldrh r1, [r1]
	adds r2, #1
	str r2, [sp]
	bl sub_80265A0
	ldr r0, [sp]
	movs r1, #0x3f
	ands r0, r1
	str r0, [sp]
	adds r4, #1
_080262D8:
	ldr r0, [r5, #0x48]
	cmp r4, r0
	ble _080262BC
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80262E8
sub_80262E8: @ 0x080262E8
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	ldr r1, [r4, #0x44]
	cmp r1, r5
	bge _08026308
_080262F6:
	adds r0, r4, #0
	bl sub_8026264
	ldr r0, [r4, #0x44]
	adds r0, #1
	str r0, [r4, #0x44]
	adds r1, r0, #0
	cmp r1, r5
	blt _080262F6
_08026308:
	ldr r1, [r4, #0x48]
	cmp r1, r6
	ble _08026320
_0802630E:
	adds r0, r4, #0
	bl sub_8026264
	ldr r0, [r4, #0x48]
	subs r0, #1
	str r0, [r4, #0x48]
	adds r1, r0, #0
	cmp r1, r6
	bgt _0802630E
_08026320:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8026328
sub_8026328: @ 0x08026328
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	ldr r1, [r4, #0x3c]
	cmp r1, r5
	bge _08026348
_08026336:
	adds r0, r4, #0
	bl sub_80262A4
	ldr r0, [r4, #0x3c]
	adds r0, #1
	str r0, [r4, #0x3c]
	adds r1, r0, #0
	cmp r1, r5
	blt _08026336
_08026348:
	ldr r1, [r4, #0x40]
	cmp r1, r6
	ble _08026360
_0802634E:
	adds r0, r4, #0
	bl sub_80262A4
	ldr r0, [r4, #0x40]
	subs r0, #1
	str r0, [r4, #0x40]
	adds r1, r0, #0
	cmp r1, r6
	bgt _0802634E
_08026360:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8026368
sub_8026368: @ 0x08026368
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r6, r0, #0
	adds r2, r1, #0
	adds r0, r2, #0
	cmp r2, #0
	bge _0802637C
	adds r0, #0x1f
_0802637C:
	asrs r0, r0, #5
	lsls r0, r0, #5
	subs r0, r2, r0
	lsls r1, r0, #6
	ldr r0, [r6, #0x4c]
	adds r0, r0, r1
	mov r8, r0
	ldr r0, [r6, #0x2c]
	ldr r1, [r6, #0x44]
	mov r3, sp
	bl sub_8024B48
	adds r7, r0, #0
	ldr r4, [r6, #0x44]
	b _080263CA
_0802639A:
	adds r0, r4, #0
	cmp r4, #0
	bge _080263A2
	adds r0, #0x1f
_080263A2:
	asrs r5, r0, #5
	lsls r0, r5, #5
	subs r5, r4, r0
	ldr r0, [r6, #0x5c]
	ldr r2, [sp]
	lsls r1, r2, #1
	adds r1, r1, r7
	ldrh r1, [r1]
	adds r2, #1
	str r2, [sp]
	bl sub_80264F8
	lsls r1, r5, #1
	add r1, r8
	strh r0, [r1]
	ldr r0, [sp]
	movs r1, #0x3f
	ands r0, r1
	str r0, [sp]
	adds r4, #1
_080263CA:
	ldr r0, [r6, #0x48]
	cmp r4, r0
	ble _0802639A
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_80263DC
sub_80263DC: @ 0x080263DC
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, [r4, #0x5c]
	bl sub_802648C
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_802608C
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80263F8
sub_80263F8: @ 0x080263F8
	push {lr}
	ldr r2, [r0, #0x50]
	ldr r3, [r0, #0x5c]
	adds r0, #0x34
	ldrb r0, [r0]
	lsls r1, r0, #0x1c
	lsrs r1, r1, #0x1e
	adds r2, #4
	adds r0, r3, #0
	bl sub_8026618
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start nullsub_26
nullsub_26: @ 0x08026414
	bx lr
	.align 2, 0

	thumb_func_start sub_8026418
sub_8026418: @ 0x08026418
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, _08026440 @ =gStaticData_087E4C64
	str r0, [r4, #0x30]
	ldr r0, [r4, #0x5c]
	cmp r0, #0
	beq _0802642C
	bl sub_8026ED0
_0802642C:
	ldr r0, _08026444 @ =gStaticData_087E4C14
	str r0, [r4, #0x30]
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_8024D74
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08026440: .4byte gStaticData_087E4C64
_08026444: .4byte gStaticData_087E4C14

	thumb_func_start sub_8026448
sub_8026448: @ 0x08026448
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8025D74
	ldr r0, _08026478 @ =gStaticData_087E4C64
	str r0, [r4, #0x30]
	adds r2, r4, #0
	adds r2, #0x34
	movs r1, #0x80
	movs r0, #0x7f
	ldrb r3, [r2]
	ands r0, r3
	orrs r0, r1
	subs r1, #0x8d
	ands r0, r1
	strb r0, [r2]
	ldr r0, _0802647C @ =0x0000480C
	bl sub_8026EDC
	str r0, [r4, #0x5c]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08026478: .4byte gStaticData_087E4C64
_0802647C: .4byte 0x0000480C

	thumb_func_start sub_8026480
sub_8026480: @ 0x08026480
	adds r0, #0x34
	ldrb r0, [r0]
	lsls r0, r0, #0x1e
	lsrs r0, r0, #0x1e
	bx lr
	.align 2, 0

	thumb_func_start sub_802648C
sub_802648C: @ 0x0802648C
	push {r4, r5, lr}
	adds r3, r0, #0
	ldr r0, _080264EC @ =0x00004808
	adds r1, r3, r0
	movs r0, #0x80
	lsls r0, r0, #2
	str r0, [r1]
	movs r1, #0
	ldr r5, _080264F0 @ =0x000001FF
	ldr r0, _080264EC @ =0x00004808
	adds r2, r3, r0
	ldr r0, _080264F4 @ =0x00004408
	adds r4, r3, r0
_080264A6:
	ldr r0, [r2]
	subs r0, #1
	str r0, [r2]
	lsls r0, r0, #1
	adds r0, r4, r0
	strh r1, [r0]
	adds r1, #1
	cmp r1, r5
	ble _080264A6
	adds r4, r3, #0
	adds r4, #8
	movs r2, #0x80
	lsls r2, r2, #2
	movs r1, #0x81
	lsls r1, r1, #3
	adds r0, r3, r1
	movs r1, #0x80
	lsls r1, r1, #6
_080264CA:
	strh r2, [r0]
	adds r0, #2
	subs r1, #1
	cmp r1, #0
	bne _080264CA
	movs r2, #0
	adds r0, r4, #0
	movs r1, #0x80
	lsls r1, r1, #2
_080264DC:
	strh r2, [r0]
	adds r0, #2
	subs r1, #1
	cmp r1, #0
	bne _080264DC
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080264EC: .4byte 0x00004808
_080264F0: .4byte 0x000001FF
_080264F4: .4byte 0x00004408

	thumb_func_start sub_80264F8
sub_80264F8: @ 0x080264F8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	ldr r0, _08026530 @ =0xFFFF0000
	mov r2, r8
	ands r2, r0
	orrs r2, r1
	mov r8, r2
	lsls r0, r2, #0x12
	lsrs r0, r0, #0x12
	mov ip, r0
	lsls r7, r0, #1
	movs r4, #0x81
	lsls r4, r4, #3
	adds r0, r5, r4
	adds r0, r0, r7
	movs r1, #0x80
	lsls r1, r1, #2
	ldrh r0, [r0]
	cmp r0, r1
	beq _08026534
	adds r0, r5, r4
	adds r0, r0, r7
	ldrh r4, [r0]
	b _0802655E
	.align 2, 0
_08026530: .4byte 0xFFFF0000
_08026534:
	ldr r0, _08026590 @ =0x00004808
	adds r3, r5, r0
	ldr r1, [r3]
	lsls r2, r1, #1
	ldr r4, _08026594 @ =0x00004408
	adds r0, r5, r4
	adds r0, r0, r2
	ldrh r0, [r0]
	adds r1, #1
	str r1, [r3]
	adds r4, r0, #0
	movs r1, #0x81
	lsls r1, r1, #3
	adds r0, r5, r1
	adds r0, r0, r7
	strh r4, [r0]
	adds r0, r5, #0
	mov r1, ip
	adds r2, r4, #0
	bl sub_80265FC
_0802655E:
	lsls r1, r4, #1
	adds r0, r5, #0
	adds r0, #8
	adds r0, r0, r1
	ldrh r1, [r0]
	adds r1, #1
	strh r1, [r0]
	ldr r0, _08026598 @ =0xFFFF0000
	ands r6, r0
	orrs r6, r4
	mov r2, r8
	lsls r0, r2, #0x10
	lsrs r0, r0, #0x1e
	lsls r0, r0, #0xa
	ldr r1, _0802659C @ =0xFFFFF3FF
	ands r6, r1
	orrs r6, r0
	lsls r0, r6, #0x10
	lsrs r0, r0, #0x10
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08026590: .4byte 0x00004808
_08026594: .4byte 0x00004408
_08026598: .4byte 0xFFFF0000
_0802659C: .4byte 0xFFFFF3FF

	thumb_func_start sub_80265A0
sub_80265A0: @ 0x080265A0
	push {r4, r5, lr}
	adds r2, r0, #0
	ldr r0, _080265F0 @ =0x00003FFF
	ands r0, r1
	lsls r4, r0, #1
	movs r1, #0x81
	lsls r1, r1, #3
	adds r0, r2, r1
	adds r0, r0, r4
	ldrh r3, [r0]
	lsls r1, r3, #1
	adds r0, r2, #0
	adds r0, #8
	adds r0, r0, r1
	ldrh r1, [r0]
	subs r1, #1
	strh r1, [r0]
	lsls r1, r1, #0x10
	cmp r1, #0
	bne _080265EA
	ldr r5, _080265F4 @ =0x00004808
	adds r0, r2, r5
	ldr r1, [r0]
	subs r1, #1
	str r1, [r0]
	lsls r1, r1, #1
	ldr r5, _080265F8 @ =0x00004408
	adds r0, r2, r5
	adds r0, r0, r1
	strh r3, [r0]
	movs r1, #0x81
	lsls r1, r1, #3
	adds r0, r2, r1
	adds r0, r0, r4
	movs r1, #0x80
	lsls r1, r1, #2
	strh r1, [r0]
_080265EA:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080265F0: .4byte 0x00003FFF
_080265F4: .4byte 0x00004808
_080265F8: .4byte 0x00004408

	thumb_func_start sub_80265FC
sub_80265FC: @ 0x080265FC
	push {lr}
	adds r3, r0, #0
	lsls r1, r1, #6
	ldr r0, [r3, #4]
	adds r0, r0, r1
	lsls r2, r2, #6
	ldr r1, [r3]
	adds r1, r1, r2
	movs r2, #0x40
	movs r3, #0x10
	bl QueueVramDmaTransfer
	pop {r0}
	bx r0

	thumb_func_start sub_8026618
sub_8026618: @ 0x08026618
	lsls r1, r1, #0xe
	movs r3, #0xc0
	lsls r3, r3, #0x13
	adds r1, r1, r3
	str r1, [r0]
	str r2, [r0, #4]
	bx lr
	.align 2, 0

	thumb_func_start sub_8026628
sub_8026628: @ 0x08026628
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r4, r2, #0
	adds r6, r3, #0
	ldr r3, [sp, #0x18]
	movs r7, #0
	cmp r1, #2
	beq _0802664E
	cmp r1, #2
	bgt _08026644
	cmp r1, #1
	beq _08026668
	b _080266B0
_08026644:
	cmp r1, #4
	beq _08026686
	cmp r1, #8
	beq _0802668C
	b _080266B0
_0802664E:
	ldr r0, [r4]
	cmp r0, #0
	bge _08026658
	str r7, [r3]
	b _080266AE
_08026658:
	movs r0, #3
	str r0, [sp]
	adds r0, r5, #0
	adds r1, r4, #0
	adds r2, r6, #0
	bl sub_8026AE8
	b _080266A8
_08026668:
	ldr r0, [r5, #0x10]
	ldr r2, [r0, #0x10]
	ldr r0, [r4]
	cmp r0, r2
	ble _08026678
	lsls r0, r2, #8
	str r0, [r3]
	b _080266AE
_08026678:
	str r1, [sp]
	adds r0, r5, #0
	adds r1, r4, #0
	adds r2, r6, #0
	bl sub_8026AE8
	b _080266A8
_08026686:
	movs r0, #2
	str r0, [sp]
	b _0802669E
_0802668C:
	ldr r0, [r5, #0x10]
	ldr r1, [r0, #0x14]
	ldr r0, [r4, #4]
	cmp r0, r1
	ble _0802669C
	lsls r0, r1, #8
	str r0, [r3]
	b _080266AE
_0802669C:
	str r7, [sp]
_0802669E:
	adds r0, r5, #0
	adds r1, r4, #0
	adds r2, r6, #0
	bl sub_8026A18
_080266A8:
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _080266B0
_080266AE:
	movs r7, #1
_080266B0:
	adds r0, r7, #0
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_80266BC
sub_80266BC: @ 0x080266BC
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, [r5, #4]
	ldrb r1, [r0, #0x18]
	cmp r1, #0
	bne _080266D8
	ldr r0, [r0, #0x14]
	str r0, [r4, #0x24]
	adds r0, r4, #0
	adds r0, #0x28
	strb r1, [r0]
	b _080266F6
_080266D8:
	ldr r0, [r0, #0x14]
	ldr r0, [r0]
	lsrs r0, r0, #8
	bl sub_8026EC0
	adds r1, r0, #0
	str r1, [r4, #0x24]
	ldr r0, [r5, #4]
	ldr r0, [r0, #0x14]
	bl LoadTaggedAsset
	adds r1, r4, #0
	adds r1, #0x28
	movs r0, #1
	strb r0, [r1]
_080266F6:
	ldr r0, [r4, #0x10]
	ldr r1, [r5, #4]
	ldr r1, [r1, #0xc]
	bl sub_80260D4
	ldr r0, [r4, #0x20]
	ldr r1, [r5, #4]
	ldr r1, [r1, #0x10]
	bl sub_80254F8
	ldr r1, [r4, #0x10]
	ldr r0, [r1, #0x10]
	subs r0, #0xf0
	str r0, [r4]
	ldr r0, [r1, #0x14]
	subs r0, #0xa0
	str r0, [r4, #4]
	bl sub_80015D0
	ldr r0, [r4, #0x14]
	ldr r1, [r5, #4]
	ldr r1, [r1]
	bl sub_80260D4
	ldr r0, [r4, #0x14]
	adds r0, #0x28
	ldrb r0, [r0]
	cmp r0, #0
	beq _08026734
	bl sub_80015C0
_08026734:
	ldr r0, [r4, #0x18]
	ldr r1, [r5, #4]
	ldr r1, [r1, #4]
	bl sub_80260D4
	ldr r0, [r4, #0x18]
	adds r0, #0x28
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802674C
	bl sub_80015B0
_0802674C:
	ldr r0, [r4, #0x1c]
	ldr r1, [r5, #4]
	ldr r1, [r1, #8]
	bl sub_80260D4
	ldr r0, [r4, #0x1c]
	adds r0, #0x28
	ldrb r0, [r0]
	cmp r0, #0
	beq _08026764
	bl sub_80015A0
_08026764:
	ldr r0, _08026794 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r2, [r5, #4]
	ldr r1, [r2, #0x1c]
	ldr r2, [r2, #0x20]
	movs r4, #0
	str r4, [sp]
	movs r3, #0
	bl sub_80255D4
	ldr r1, _08026798 @ =0x040000D4
	ldr r0, [r5]
	str r0, [r1]
	movs r2, #0xa0
	lsls r2, r2, #0x13
	str r2, [r1, #4]
	ldr r0, _0802679C @ =0x80000100
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	strh r4, [r2]
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08026794: .4byte gUnknown_030012B4
_08026798: .4byte 0x040000D4
_0802679C: .4byte 0x80000100

	thumb_func_start sub_80267A0
sub_80267A0: @ 0x080267A0
	push {r4, lr}
	adds r4, r0, #0
	movs r0, #0x60
	bl sub_8026EDC
	movs r1, #0
	bl sub_8026448
	str r0, [r4, #0x10]
	ldr r0, _08026808 @ =0x00001064
	bl sub_8026EDC
	bl nullsub_4
	str r0, [r4, #0x20]
	movs r0, #0x5c
	bl sub_8026EDC
	movs r1, #1
	bl sub_8025D74
	str r0, [r4, #0x14]
	movs r0, #0x5c
	bl sub_8026EDC
	movs r1, #2
	bl sub_8025D74
	str r0, [r4, #0x18]
	movs r0, #0x5c
	bl sub_8026EDC
	movs r1, #3
	bl sub_8025D74
	str r0, [r4, #0x1c]
	adds r0, r4, #0
	adds r0, #0x29
	movs r1, #0
	strb r1, [r0]
	str r1, [r4, #0x24]
	subs r0, #1
	strb r1, [r0]
	adds r0, #3
	strb r1, [r0]
	subs r0, #1
	strb r1, [r0]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08026808: .4byte 0x00001064

	thumb_func_start sub_802680C
sub_802680C: @ 0x0802680C
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	cmp r0, #0
	bne _08026820
	ldr r0, [r4, #0x24]
	cmp r0, #0
	beq _0802682A
_08026820:
	ldr r0, [r4, #0x24]
	cmp r0, #0
	beq _0802682A
	bl sub_8026EB4
_0802682A:
	ldr r2, [r4, #0x10]
	cmp r2, #0
	beq _08026840
	ldr r1, [r2, #0x30]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_08026840:
	ldr r0, [r4, #0x20]
	cmp r0, #0
	beq _0802684C
	movs r1, #3
	bl sub_8025444
_0802684C:
	ldr r2, [r4, #0x14]
	cmp r2, #0
	beq _08026862
	ldr r1, [r2, #0x30]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_08026862:
	ldr r2, [r4, #0x18]
	cmp r2, #0
	beq _08026878
	ldr r1, [r2, #0x30]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_08026878:
	ldr r2, [r4, #0x1c]
	cmp r2, #0
	beq _0802688E
	ldr r1, [r2, #0x30]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_0802688E:
	ldr r1, _080268A8 @ =gUnknown_0300084C
	movs r0, #0
	str r0, [r1]
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _080268A2
	adds r0, r4, #0
	bl sub_8026ED0
_080268A2:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080268A8: .4byte gUnknown_0300084C

	thumb_func_start sub_80268AC
sub_80268AC: @ 0x080268AC
	push {r4, lr}
	ldr r4, _080268CC @ =gUnknown_0300084C
	ldr r0, [r4]
	cmp r0, #0
	bne _080268C2
	movs r0, #0x2c
	bl sub_8026EDC
	bl sub_80267A0
	str r0, [r4]
_080268C2:
	ldr r0, [r4]
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_080268CC: .4byte gUnknown_0300084C

	thumb_func_start sub_80268D0
sub_80268D0: @ 0x080268D0
	cmp r1, #0
	bge _080268D6
	movs r1, #0
_080268D6:
	cmp r2, #0
	bge _080268DC
	movs r2, #0
_080268DC:
	asrs r1, r1, #8
	asrs r2, r2, #8
	ldr r3, [r0]
	cmp r3, r1
	ble _080268E8
	adds r3, r1, #0
_080268E8:
	str r3, [r0, #8]
	ldr r1, [r0, #4]
	cmp r1, r2
	ble _080268F2
	adds r1, r2, #0
_080268F2:
	str r1, [r0, #0xc]
	bx lr
	.align 2, 0

	thumb_func_start sub_80268F8
sub_80268F8: @ 0x080268F8
	push {r4, r5, lr}
	adds r5, r0, #0
	ldr r0, [r5, #0x10]
	bl sub_8025F24
	movs r4, #0
_08026904:
	lsls r1, r4, #2
	adds r0, r5, #0
	adds r0, #0x14
	adds r0, r0, r1
	ldr r1, [r0]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802691E
	adds r0, r1, #0
	bl sub_8025F24
_0802691E:
	adds r4, #1
	cmp r4, #2
	ble _08026904
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_802692C
sub_802692C: @ 0x0802692C
	push {r4, r5, lr}
	sub sp, #8
	adds r5, r0, #0
	ldr r0, [r5, #0x10]
	ldr r2, [r0, #0x30]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	adds r1, r5, #0
	adds r1, #8
	ldr r2, [r2, #0x1c]
	bl sub_803AD80
	ldr r1, [r5, #0x10]
	ldr r0, [r1]
	str r0, [sp]
	ldr r0, [r1, #4]
	str r0, [sp, #4]
	movs r4, #0
_08026952:
	lsls r1, r4, #2
	adds r0, r5, #0
	adds r0, #0x14
	adds r0, r0, r1
	ldr r2, [r0]
	adds r0, r2, #0
	adds r0, #0x28
	ldrb r0, [r0]
	cmp r0, #0
	beq _08026976
	ldr r1, [r2, #0x30]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0x1c]
	mov r1, sp
	bl sub_803AD80
_08026976:
	adds r4, #1
	cmp r4, #2
	ble _08026952
	add sp, #8
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_8026984
sub_8026984: @ 0x08026984
	push {r4, r5, lr}
	sub sp, #8
	adds r5, r0, #0
	ldr r0, [r5, #0x10]
	ldr r2, [r0, #0x30]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	adds r1, r5, #0
	adds r1, #8
	ldr r2, [r2, #0x14]
	bl sub_803AD80
	ldr r1, [r5, #0x10]
	ldr r0, [r1]
	str r0, [sp]
	ldr r0, [r1, #4]
	str r0, [sp, #4]
	movs r4, #0
_080269AA:
	lsls r1, r4, #2
	adds r0, r5, #0
	adds r0, #0x14
	adds r0, r0, r1
	ldr r2, [r0]
	adds r0, r2, #0
	adds r0, #0x28
	ldrb r0, [r0]
	cmp r0, #0
	beq _080269CE
	ldr r1, [r2, #0x30]
	movs r3, #0x10
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0x14]
	mov r1, sp
	bl sub_803AD80
_080269CE:
	adds r4, #1
	cmp r4, #2
	ble _080269AA
	add sp, #8
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_80269DC
sub_80269DC: @ 0x080269DC
	push {r4, lr}
	movs r4, #1
	cmp r1, #0
	beq _080269F0
	ldr r0, [r2]
	cmp r0, #0
	beq _080269F0
	cmp r3, #0
	beq _080269F0
	movs r4, #0
_080269F0:
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1

	thumb_func_start sub_80269F8
sub_80269F8: @ 0x080269F8
	push {r4, lr}
	movs r4, #1
	cmp r1, #0
	beq _08026A0C
	ldr r0, [r2]
	cmp r0, #0
	beq _08026A0C
	cmp r3, #0
	beq _08026A0C
	movs r4, #0
_08026A0C:
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1

	thumb_func_start sub_8026A14
sub_8026A14: @ 0x08026A14
	movs r0, #0
	bx lr

	thumb_func_start sub_8026A18
sub_8026A18: @ 0x08026A18
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xc
	mov sl, r0
	adds r7, r1, #0
	mov r8, r3
	movs r0, #0
	mov sb, r0
	add r0, sp, #4
	mov r1, sb
	strb r1, [r0]
	ldr r6, [r7, #4]
	ldr r4, [r7]
	adds r2, r4, r2
	subs r5, r2, #1
	asrs r4, r4, #3
	asrs r6, r6, #3
	asrs r5, r5, #3
	movs r0, #1
	rsbs r0, r0, #0
	cmp r4, r0
	bne _08026A4C
	movs r4, #0
_08026A4C:
	mov r2, sl
	ldr r0, [r2, #0x20]
	ldr r0, [r0, #0x10]
	cmp r5, r0
	bne _08026A58
	subs r5, #1
_08026A58:
	mov r0, sl
	adds r0, #0x2a
	str r0, [sp, #8]
	cmp r4, r5
	bgt _08026A88
_08026A62:
	mov r1, sl
	ldr r0, [r1, #0x20]
	add r2, sp, #4
	str r2, [sp]
	adds r1, r4, #0
	adds r2, r6, #0
	ldr r3, [sp, #0x2c]
	bl sub_8025130
	cmp r0, #0
	beq _08026A7C
	movs r0, #1
	mov sb, r0
_08026A7C:
	adds r4, #1
	cmp r4, r5
	bgt _08026A88
	mov r1, sb
	cmp r1, #0
	beq _08026A62
_08026A88:
	mov r2, sb
	cmp r2, #0
	beq _08026ABE
	ldr r0, [sp, #0x2c]
	cmp r0, #0
	beq _08026AAE
	cmp r0, #2
	bne _08026ABE
	ldr r0, [r7, #4]
	movs r1, #7
	ands r0, r1
	movs r1, #8
	subs r1, r1, r0
	lsls r1, r1, #8
	mov r2, r8
	ldr r0, [r2]
	adds r0, r0, r1
	str r0, [r2]
	b _08026ABE
_08026AAE:
	ldr r0, [r7, #4]
	movs r1, #7
	ands r0, r1
	lsls r0, r0, #8
	mov r2, r8
	ldr r1, [r2]
	subs r1, r1, r0
	str r1, [r2]
_08026ABE:
	ldr r1, [sp, #8]
	ldrb r0, [r1]
	cmp r0, #0
	beq _08026AD4
	add r0, sp, #4
	ldrb r1, [r0]
	cmp r1, #0
	beq _08026AD4
	mov r0, sl
	adds r0, #0x29
	strb r1, [r0]
_08026AD4:
	mov r0, sb
	add sp, #0xc
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8026AE8
sub_8026AE8: @ 0x08026AE8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xc
	mov sl, r0
	adds r7, r1, #0
	mov r8, r3
	movs r0, #0
	mov sb, r0
	add r0, sp, #4
	mov r1, sb
	strb r1, [r0]
	ldr r4, [r7, #4]
	ldr r6, [r7]
	adds r2, r4, r2
	subs r5, r2, #1
	asrs r6, r6, #3
	asrs r4, r4, #3
	asrs r5, r5, #3
	movs r0, #1
	rsbs r0, r0, #0
	cmp r4, r0
	bne _08026B1C
	movs r4, #0
_08026B1C:
	mov r1, sl
	ldr r0, [r1, #0x20]
	ldr r0, [r0, #0x14]
	cmp r5, r0
	bne _08026B28
	subs r5, #1
_08026B28:
	mov r0, sl
	adds r0, #0x2a
	str r0, [sp, #8]
	cmp r4, r5
	bgt _08026B58
_08026B32:
	mov r1, sl
	ldr r0, [r1, #0x20]
	add r1, sp, #4
	str r1, [sp]
	adds r1, r6, #0
	adds r2, r4, #0
	ldr r3, [sp, #0x2c]
	bl sub_8025130
	cmp r0, #0
	beq _08026B4C
	movs r0, #1
	mov sb, r0
_08026B4C:
	adds r4, #1
	cmp r4, r5
	bgt _08026B58
	mov r1, sb
	cmp r1, #0
	beq _08026B32
_08026B58:
	mov r0, sb
	cmp r0, #0
	beq _08026B96
	ldr r1, [sp, #0x2c]
	cmp r1, #1
	beq _08026B82
	cmp r1, #3
	bne _08026B96
	mov r0, r8
	ldr r2, [r0]
	adds r2, #1
	ldr r1, [r7]
	movs r0, #7
	ands r1, r0
	movs r0, #8
	subs r0, r0, r1
	lsls r0, r0, #8
	adds r2, r2, r0
	mov r1, r8
	str r2, [r1]
	b _08026B96
_08026B82:
	mov r1, r8
	ldr r0, [r1]
	subs r0, #1
	ldr r1, [r7]
	movs r2, #7
	ands r1, r2
	lsls r1, r1, #8
	subs r0, r0, r1
	mov r1, r8
	str r0, [r1]
_08026B96:
	ldr r1, [sp, #8]
	ldrb r0, [r1]
	cmp r0, #0
	beq _08026BAC
	add r0, sp, #4
	ldrb r1, [r0]
	cmp r1, #0
	beq _08026BAC
	mov r0, sl
	adds r0, #0x29
	strb r1, [r0]
_08026BAC:
	mov r0, sb
	add sp, #0xc
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8026BC0
sub_8026BC0: @ 0x08026BC0
	push {r4, lr}
	sub sp, #0xc
	adds r4, r0, #0
	movs r3, #0
	add r0, sp, #4
	strb r3, [r0]
	str r3, [sp, #8]
	asrs r3, r1, #3
	asrs r2, r2, #3
	cmp r3, #0
	bge _08026BD8
	movs r3, #0
_08026BD8:
	cmp r2, #0
	bge _08026BDE
	movs r2, #0
_08026BDE:
	ldr r0, [r4, #0x20]
	add r1, sp, #8
	str r1, [sp]
	adds r1, r3, #0
	add r3, sp, #4
	bl sub_8025460
	add r0, sp, #4
	ldrb r0, [r0]
	add sp, #0xc
	pop {r4}
	pop {r1}
	bx r1

	thumb_func_start sub_8026BF8
sub_8026BF8: @ 0x08026BF8
	push {r4, r5, r6, lr}
	adds r4, r1, #0
	adds r6, r2, #0
	ldr r1, [r4]
	asrs r1, r1, #3
	ldr r2, [r4, #4]
	asrs r5, r2, #3
	ldr r0, [r0, #0x20]
	adds r2, r5, #0
	bl sub_80250BC
	adds r3, r0, #0
	cmp r3, #0
	bne _08026C18
	movs r0, #0
	b _08026C36
_08026C18:
	ldr r2, [r4, #4]
	ldr r0, [r4]
	movs r1, #7
	ands r0, r1
	adds r0, r3, r0
	movs r1, #0
	ldrsb r1, [r0, r1]
	lsls r0, r5, #3
	adds r0, r0, r1
	subs r0, r0, r2
	lsls r0, r0, #8
	ldr r1, [r6]
	adds r1, r1, r0
	str r1, [r6]
	movs r0, #1
_08026C36:
	pop {r4, r5, r6}
	pop {r1}
	bx r1

	thumb_func_start sub_8026C3C
sub_8026C3C: @ 0x08026C3C
	push {r4, r5, r6, lr}
	sub sp, #8
	adds r4, r1, #0
	adds r6, r2, #0
	ldr r1, [r4]
	asrs r1, r1, #3
	ldr r2, [r4, #4]
	asrs r5, r2, #3
	ldr r0, [r0, #0x20]
	add r2, sp, #4
	str r2, [sp]
	adds r2, r5, #0
	movs r3, #0
	bl sub_8025228
	lsls r0, r0, #0x18
	asrs r2, r0, #0x18
	cmp r2, #0
	bge _08026C66
	movs r0, #0
	b _08026C78
_08026C66:
	ldr r0, [r4, #4]
	lsls r1, r5, #3
	adds r1, r1, r2
	subs r1, r1, r0
	lsls r1, r1, #8
	ldr r0, [r6]
	adds r0, r0, r1
	str r0, [r6]
	movs r0, #1
_08026C78:
	add sp, #8
	pop {r4, r5, r6}
	pop {r1}
	bx r1

	thumb_func_start sub_8026C80
sub_8026C80: @ 0x08026C80
	cmp r1, #0
	beq _08026C86
	ldr r0, [r2]
_08026C86:
	movs r0, #0
	bx lr
	.align 2, 0

	thumb_func_start sub_8026C8C
sub_8026C8C: @ 0x08026C8C
	movs r0, #0
	bx lr

	thumb_func_start sub_8026C90
sub_8026C90: @ 0x08026C90
	push {r4, r5, r6, lr}
	adds r1, r0, #0
	ldr r0, [r1, #0x10]
	ldr r2, [r0]
	ldr r3, [r0, #4]
	adds r0, #0x24
	ldrb r5, [r0]
	cmp r5, #0
	beq _08026D5C
	movs r0, #4
	ands r0, r5
	cmp r0, #0
	beq _08026CC0
	ldr r4, [r1, #0xc]
	ldr r0, _08026CB8 @ =0xFFFFE556
	cmp r4, r0
	ble _08026CC0
	ldr r6, _08026CBC @ =0xFFFFFF00
	b _08026CD4
	.align 2, 0
_08026CB8: .4byte 0xFFFFE556
_08026CBC: .4byte 0xFFFFFF00
_08026CC0:
	movs r0, #8
	ands r0, r5
	cmp r0, #0
	beq _08026CD8
	ldr r4, [r1, #0xc]
	ldr r0, _08026CEC @ =0x00001AA9
	cmp r4, r0
	bgt _08026CD8
	movs r6, #0x80
	lsls r6, r6, #1
_08026CD4:
	adds r0, r4, r6
	str r0, [r1, #0xc]
_08026CD8:
	movs r0, #2
	ands r0, r5
	cmp r0, #0
	beq _08026CF8
	ldr r4, [r1, #8]
	ldr r0, _08026CF0 @ =0xFFFFD800
	cmp r4, r0
	ble _08026CF8
	ldr r6, _08026CF4 @ =0xFFFFFF00
	b _08026D0C
	.align 2, 0
_08026CEC: .4byte 0x00001AA9
_08026CF0: .4byte 0xFFFFD800
_08026CF4: .4byte 0xFFFFFF00
_08026CF8:
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _08026D10
	ldr r4, [r1, #8]
	ldr r0, _08026D24 @ =0x000027FF
	cmp r4, r0
	bgt _08026D10
	movs r6, #0x80
	lsls r6, r6, #1
_08026D0C:
	adds r0, r4, r6
	str r0, [r1, #8]
_08026D10:
	movs r0, #3
	ands r0, r5
	cmp r0, #0
	bne _08026D38
	ldr r0, [r1, #8]
	cmp r0, #0
	ble _08026D2C
	ldr r4, _08026D28 @ =0xFFFFFF00
	adds r0, r0, r4
	b _08026D36
	.align 2, 0
_08026D24: .4byte 0x000027FF
_08026D28: .4byte 0xFFFFFF00
_08026D2C:
	cmp r0, #0
	bge _08026D38
	movs r6, #0x80
	lsls r6, r6, #1
	adds r0, r0, r6
_08026D36:
	str r0, [r1, #8]
_08026D38:
	movs r0, #0xc
	ands r5, r0
	cmp r5, #0
	bne _08026D5C
	ldr r0, [r1, #0xc]
	cmp r0, #0
	ble _08026D50
	ldr r4, _08026D4C @ =0xFFFFFF00
	adds r0, r0, r4
	b _08026D5A
	.align 2, 0
_08026D4C: .4byte 0xFFFFFF00
_08026D50:
	cmp r0, #0
	bge _08026D5C
	movs r6, #0x80
	lsls r6, r6, #1
	adds r0, r0, r6
_08026D5A:
	str r0, [r1, #0xc]
_08026D5C:
	ldr r0, [r1, #8]
	adds r2, r2, r0
	ldr r0, [r1, #0xc]
	adds r3, r3, r0
	ldr r4, [r1]
	subs r0, r2, r4
	cmp r0, #0
	bge _08026D6E
	adds r0, #3
_08026D6E:
	asrs r0, r0, #2
	adds r0, r4, r0
	str r0, [r1]
	ldr r4, [r1, #4]
	subs r0, r3, r4
	cmp r0, #0
	bge _08026D7E
	adds r0, #3
_08026D7E:
	asrs r0, r0, #2
	adds r0, r4, r0
	str r0, [r1, #4]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8026D8C
sub_8026D8C: @ 0x08026D8C
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x10]
	ldr r2, [r0]
	ldr r3, [r0, #4]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _08026DB4
	ldr r1, [r4, #8]
	ldr r0, _08026DAC @ =0xFFFFED8A
	cmp r1, r0
	ble _08026DC4
	ldr r5, _08026DB0 @ =0xFFFFFF00
	b _08026DC0
	.align 2, 0
_08026DAC: .4byte 0xFFFFED8A
_08026DB0: .4byte 0xFFFFFF00
_08026DB4:
	ldr r1, [r4, #8]
	ldr r0, _08026DF4 @ =0x00001275
	cmp r1, r0
	bgt _08026DC4
	movs r5, #0x80
	lsls r5, r5, #1
_08026DC0:
	adds r0, r1, r5
	str r0, [r4, #8]
_08026DC4:
	ldr r1, _08026DF8 @ =0xFFFFF000
	str r1, [r4, #0xc]
	ldr r0, [r4, #8]
	adds r2, r2, r0
	adds r3, r3, r1
	ldr r1, [r4]
	subs r0, r2, r1
	cmp r0, #0
	bge _08026DD8
	adds r0, #3
_08026DD8:
	asrs r0, r0, #2
	adds r0, r1, r0
	str r0, [r4]
	ldr r1, [r4, #4]
	subs r0, r3, r1
	cmp r0, #0
	bge _08026DE8
	adds r0, #3
_08026DE8:
	asrs r0, r0, #2
	adds r0, r1, r0
	str r0, [r4, #4]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08026DF4: .4byte 0x00001275
_08026DF8: .4byte 0xFFFFF000

	thumb_func_start sub_8026DFC
sub_8026DFC: @ 0x08026DFC
	push {lr}
	adds r3, r0, #0
	ldr r1, [r3, #0x10]
	ldr r0, [r1]
	str r0, [r3]
	ldr r0, [r1, #4]
	str r0, [r3, #4]
	ldr r0, [r3, #0x14]
	cmp r0, #1
	bne _08026E34
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _08026E24
	ldr r0, _08026E20 @ =0xFFFFED8A
	b _08026E26
	.align 2, 0
_08026E20: .4byte 0xFFFFED8A
_08026E24:
	ldr r0, _08026E2C @ =0x00001276
_08026E26:
	str r0, [r3, #8]
	ldr r0, _08026E30 @ =0xFFFFF000
	b _08026E38
	.align 2, 0
_08026E2C: .4byte 0x00001276
_08026E30: .4byte 0xFFFFF000
_08026E34:
	movs r0, #0
	str r0, [r3, #8]
_08026E38:
	str r0, [r3, #0xc]
	ldr r1, [r3]
	ldr r0, [r3, #8]
	adds r1, r1, r0
	str r1, [r3]
	ldr r2, [r3, #4]
	ldr r0, [r3, #0xc]
	adds r2, r2, r0
	str r2, [r3, #4]
	ldr r0, _08026E60 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r3, _08026E64 @ =0xFFFF8800
	adds r1, r1, r3
	ldr r3, _08026E68 @ =0xFFFFB000
	adds r2, r2, r3
	bl sub_80268D0
	pop {r0}
	bx r0
	.align 2, 0
_08026E60: .4byte gUnknown_03001308
_08026E64: .4byte 0xFFFF8800
_08026E68: .4byte 0xFFFFB000

	thumb_func_start sub_8026E6C
sub_8026E6C: @ 0x08026E6C
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x14]
	cmp r0, #2
	beq _08026E86
	cmp r0, #2
	bgt _08026E8C
	cmp r0, #1
	bne _08026E8C
	adds r0, r4, #0
	bl sub_8026D8C
	b _08026E8C
_08026E86:
	adds r0, r4, #0
	bl sub_8026C90
_08026E8C:
	ldr r0, _08026EA8 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r1, [r4]
	ldr r2, _08026EAC @ =0xFFFF8800
	adds r1, r1, r2
	ldr r2, [r4, #4]
	ldr r3, _08026EB0 @ =0xFFFFB000
	adds r2, r2, r3
	bl sub_80268D0
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08026EA8: .4byte gUnknown_03001308
_08026EAC: .4byte 0xFFFF8800
_08026EB0: .4byte 0xFFFFB000

	thumb_func_start sub_8026EB4
sub_8026EB4: @ 0x08026EB4
	push {lr}
	bl mem_free
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8026EC0
sub_8026EC0: @ 0x08026EC0
	push {lr}
	movs r1, #0x80
	lsls r1, r1, #0x17
	bl mem_alloc
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8026ED0
sub_8026ED0: @ 0x08026ED0
	push {lr}
	bl mem_free
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8026EDC
sub_8026EDC: @ 0x08026EDC
	push {lr}
	movs r1, #0x80
	lsls r1, r1, #0x17
	bl mem_alloc
	pop {r1}
	bx r1
	.align 2, 0

