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
