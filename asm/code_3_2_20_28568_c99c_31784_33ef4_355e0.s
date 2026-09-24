.include "asm/macros.inc"

.syntax unified
.arm

@ LoadObjSpriteTiles: not yet matched (NON_MATCHING) - see
@ src/graphics/level_graphics.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. It hasn't had
@ a full register-tuning pass yet. LoadBg2Background (the other half of this
@ pair) is now matched, as a NAKED transcription, directly in
@ src/graphics/level_graphics.c, and no longer has raw bytes here. See
@ docs/matching/issue-65-graphics-loading.md.
.if NON_MATCHING == 0
	thumb_func_start LoadObjSpriteTiles
LoadObjSpriteTiles: @ 0x08035684
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	ldr r7, _0803576C @ =gUnknown_030008BC
	ldr r0, _08035770 @ =0x06010000
	mov sl, r0
	ldr r1, _08035774 @ =0x05000200
	str r1, [sp]
	movs r4, #0
	mov sb, r4
_0803569E:
	ldr r0, [r7]
	ldr r0, [r0, #8]
	ldr r0, [r0]
	lsrs r0, r0, #8
	bl sub_8026EC0
	adds r4, r0, #0
	ldr r0, [r7]
	ldr r0, [r0, #8]
	adds r1, r4, #0
	bl LoadTaggedAsset
	ldr r0, _08035778 @ =0x040000D4
	str r4, [r0]
	ldr r1, [sp]
	str r1, [r0, #4]
	ldr r1, _0803577C @ =0x80000010
	str r1, [r0, #8]
	ldr r0, [r0, #8]
	ldr r0, [sp]
	adds r0, #0x20
	str r0, [sp]
	cmp r4, #0
	beq _080356D4
	adds r0, r4, #0
	bl sub_8026EB4
_080356D4:
	ldr r0, [r7]
	ldr r0, [r0, #0xc]
	ldr r0, [r0]
	lsrs r0, r0, #8
	bl sub_8026EC0
	adds r6, r0, #0
	ldr r0, [r7]
	ldr r0, [r0, #0xc]
	adds r1, r6, #0
	bl LoadTaggedAsset
	ldr r0, [r7]
	ldr r1, [r0, #4]
	ldr r0, [r0]
	adds r4, r1, #0
	muls r4, r0, r4
	lsls r0, r4, #1
	bl sub_8026EC0
	adds r5, r0, #0
	ldm r7!, {r0}
	ldr r0, [r0, #0x10]
	adds r1, r5, #0
	bl LoadTaggedAsset
	mov r8, r7
	movs r1, #1
	add sb, r1
	cmp r4, #0
	ble _0803573E
	ldr r3, _08035778 @ =0x040000D4
	movs r0, #0xff
	mov ip, r0
	ldr r7, _0803577C @ =0x80000010
	adds r2, r5, #0
	adds r1, r4, #0
_0803571E:
	mov r0, ip
	ldrh r4, [r2]
	ands r0, r4
	lsls r0, r0, #5
	adds r0, r6, r0
	str r0, [r3]
	mov r0, sl
	str r0, [r3, #4]
	str r7, [r3, #8]
	ldr r0, [r3, #8]
	movs r4, #0x20
	add sl, r4
	adds r2, #2
	subs r1, #1
	cmp r1, #0
	bne _0803571E
_0803573E:
	cmp r5, #0
	beq _08035748
	adds r0, r5, #0
	bl sub_8026EB4
_08035748:
	cmp r6, #0
	beq _08035752
	adds r0, r6, #0
	bl sub_8026EB4
_08035752:
	mov r7, r8
	mov r0, sb
	cmp r0, #3
	ble _0803569E
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0803576C: .4byte gUnknown_030008BC
_08035770: .4byte 0x06010000
_08035774: .4byte 0x05000200
_08035778: .4byte 0x040000D4
_0803577C: .4byte 0x80000010
.endif

	thumb_func_start sub_8035780
sub_8035780: @ 0x08035780
	push {r4, r5, lr}
	mov ip, r0
	movs r5, #0
_08035786:
	movs r0, #0x34
	adds r3, r5, #0
	muls r3, r0, r3
	mov r0, ip
	adds r0, #0x14
	adds r4, r0, r3
	ldr r0, [r4]
	cmp r0, #0
	bne _0803579A
	b _0803589A
_0803579A:
	subs r0, #1
	str r0, [r4]
	cmp r0, #0
	bne _08035836
	mov r1, ip
	adds r1, #0x40
	adds r1, r1, r3
	ldr r0, [r1]
	adds r2, r0, #0
	adds r0, #0x20
	str r0, [r1]
	mov r0, ip
	adds r1, r0, r3
	movs r0, #1
	strb r0, [r1, #0x10]
	movs r1, #0
	ldrsh r0, [r2, r1]
	str r0, [r4]
	cmp r0, #0
	beq _0803589A
	mov r0, ip
	adds r0, #0x20
	adds r0, r0, r3
	ldrh r4, [r2, #6]
	lsls r1, r4, #0x10
	str r1, [r0]
	mov r0, ip
	adds r0, #0x34
	adds r0, r0, r3
	ldr r1, [r2, #0x14]
	str r1, [r0]
	mov r1, ip
	adds r1, #0x24
	adds r1, r1, r3
	movs r4, #8
	ldrsh r0, [r2, r4]
	lsls r0, r0, #8
	str r0, [r1]
	mov r0, ip
	adds r0, #0x38
	adds r0, r0, r3
	ldr r1, [r2, #0x18]
	str r1, [r0]
	mov r1, ip
	adds r1, #0x28
	adds r1, r1, r3
	movs r4, #0xa
	ldrsh r0, [r2, r4]
	lsls r0, r0, #8
	str r0, [r1]
	mov r0, ip
	adds r0, #0x3c
	adds r0, r0, r3
	ldr r1, [r2, #0x1c]
	str r1, [r0]
	mov r0, ip
	adds r0, #0x18
	adds r0, r0, r3
	ldrh r4, [r2, #2]
	lsls r1, r4, #0x10
	str r1, [r0]
	mov r0, ip
	adds r0, #0x2c
	adds r0, r0, r3
	ldr r1, [r2, #0xc]
	str r1, [r0]
	mov r0, ip
	adds r0, #0x1c
	adds r0, r0, r3
	ldrh r4, [r2, #4]
	lsls r1, r4, #0x10
	str r1, [r0]
	mov r0, ip
	adds r0, #0x30
	adds r0, r0, r3
	ldr r1, [r2, #0x10]
	str r1, [r0]
	b _0803589A
_08035836:
	mov r2, ip
	adds r2, #0x20
	adds r2, r2, r3
	mov r0, ip
	adds r0, #0x34
	adds r0, r0, r3
	ldr r1, [r2]
	ldr r0, [r0]
	adds r1, r1, r0
	str r1, [r2]
	mov r2, ip
	adds r2, #0x24
	adds r2, r2, r3
	mov r0, ip
	adds r0, #0x38
	adds r0, r0, r3
	ldr r1, [r2]
	ldr r0, [r0]
	adds r1, r1, r0
	str r1, [r2]
	mov r2, ip
	adds r2, #0x28
	adds r2, r2, r3
	mov r0, ip
	adds r0, #0x3c
	adds r0, r0, r3
	ldr r1, [r2]
	ldr r0, [r0]
	adds r1, r1, r0
	str r1, [r2]
	mov r2, ip
	adds r2, #0x18
	adds r2, r2, r3
	mov r0, ip
	adds r0, #0x2c
	adds r0, r0, r3
	ldr r1, [r2]
	ldr r0, [r0]
	adds r1, r1, r0
	str r1, [r2]
	mov r2, ip
	adds r2, #0x1c
	adds r2, r2, r3
	mov r0, ip
	adds r0, #0x30
	adds r0, r0, r3
	ldr r1, [r2]
	ldr r0, [r0]
	adds r1, r1, r0
	str r1, [r2]
_0803589A:
	adds r5, #1
	cmp r5, #8
	bgt _080358A2
	b _08035786
_080358A2:
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_80358A8
sub_80358A8: @ 0x080358A8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x2c
	mov sl, r0
	movs r0, #0
	mov sb, r0
	movs r7, #0xd8
	lsls r7, r7, #1
	add r7, sl
	ldrb r0, [r7]
	cmp r0, #0
	bne _080358C8
	b _080359DE
_080358C8:
	ldr r1, [r7, #0x14]
	movs r0, #0x80
	lsls r0, r0, #0x11
	bl sub_803ADB4
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	ldr r1, _08035A98 @ =gUnknown_03001300
	ldr r1, [r1]
	mov r8, r1
	strh r0, [r1, #0x12]
	strh r0, [r1, #0x2a]
	mov r0, r8
	adds r0, #8
	mov r2, sb
	strh r2, [r0, #0x12]
	adds r0, #8
	strh r2, [r0, #0x12]
	mov r0, sp
	strh r2, [r0]
	ldr r1, _08035A9C @ =0x040000D4
	str r0, [r1]
	add r4, sp, #4
	str r4, [r1, #4]
	ldr r0, _08035AA0 @ =0x81000004
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldrb r3, [r4, #1]
	movs r0, #3
	orrs r3, r0
	ldrb r0, [r4, #3]
	movs r1, #0xf
	rsbs r1, r1, #0
	ands r1, r0
	movs r0, #0xf
	ldrb r5, [r4, #5]
	ands r0, r5
	movs r2, #0x30
	orrs r0, r2
	strb r0, [r4, #5]
	movs r2, #0x3f
	ands r1, r2
	movs r0, #0x80
	orrs r1, r0
	strb r1, [r4, #3]
	ands r3, r2
	movs r0, #0x40
	orrs r3, r0
	strb r3, [r4, #1]
	movs r1, #0xe
	ldrsh r0, [r7, r1]
	subs r0, #0x10
	strb r0, [r4]
	ldr r6, _08035AA4 @ =0xFFFFFC00
	adds r0, r6, #0
	ldrh r2, [r4, #4]
	ands r0, r2
	movs r3, #0xe0
	lsls r3, r3, #1
	adds r1, r3, #0
	orrs r0, r1
	strh r0, [r4, #4]
	movs r5, #0xa
	ldrsh r1, [r7, r5]
	ldr r0, [r7, #0x14]
	lsls r0, r0, #5
	asrs r0, r0, #0x10
	adds r0, #0x20
	subs r1, r1, r0
	ldr r0, _08035AA8 @ =0x000001FF
	mov sb, r0
	mov r2, sb
	ands r1, r2
	ldrh r2, [r4, #2]
	ldr r5, _08035AAC @ =0xFFFFFE00
	adds r0, r5, #0
	ands r0, r2
	orrs r0, r1
	strh r0, [r4, #2]
	mov r0, r8
	adds r1, r4, #0
	bl sub_8006AC8
	ldrh r2, [r4, #4]
	lsls r1, r2, #0x16
	lsrs r1, r1, #0x16
	adds r1, #8
	ldr r3, _08035AB0 @ =0x000003FF
	mov r8, r3
	mov r0, r8
	ands r1, r0
	adds r0, r6, #0
	ands r0, r2
	orrs r0, r1
	strh r0, [r4, #4]
	movs r2, #0xa
	ldrsh r1, [r7, r2]
	subs r1, #0x20
	mov r3, sb
	ands r1, r3
	ldrh r2, [r4, #2]
	adds r0, r5, #0
	ands r0, r2
	orrs r0, r1
	strh r0, [r4, #2]
	ldr r1, _08035A98 @ =gUnknown_03001300
	ldr r0, [r1]
	adds r1, r4, #0
	bl sub_8006AC8
	ldrh r1, [r4, #4]
	lsls r0, r1, #0x16
	lsrs r0, r0, #0x16
	adds r0, #8
	mov r2, r8
	ands r0, r2
	ands r6, r1
	orrs r6, r0
	strh r6, [r4, #4]
	movs r3, #0xa
	ldrsh r1, [r7, r3]
	ldr r0, [r7, #0x14]
	lsls r0, r0, #5
	asrs r0, r0, #0x10
	subs r0, #0x20
	adds r1, r1, r0
	mov r7, sb
	ands r1, r7
	ldrh r0, [r4, #2]
	ands r5, r0
	orrs r5, r1
	strh r5, [r4, #2]
	ldr r1, _08035A98 @ =gUnknown_03001300
	ldr r0, [r1]
	adds r1, r4, #0
	bl sub_8006AC8
	movs r2, #2
	mov sb, r2
_080359DE:
	movs r7, #0
	mov r3, sp
	adds r3, #0x14
	str r3, [sp, #0x24]
	mov r5, sl
	adds r5, #0x78
	str r5, [sp, #0x20]
	movs r0, #0
	mov r8, r0
_080359F0:
	movs r0, #0x34
	muls r0, r7, r0
	ldr r1, _08035AB4 @ =0xFFFFFE84
	adds r0, r0, r1
	mov r2, sl
	subs r6, r2, r0
	ldrb r0, [r6]
	cmp r0, #0
	bne _08035A04
	b _08035B38
_08035A04:
	movs r0, #7
	subs r0, r0, r7
	lsls r0, r0, #2
	movs r1, #0xf2
	lsls r1, r1, #1
	add r1, sl
	adds r1, r1, r0
	ldr r2, [r1]
	cmp r2, #0
	beq _08035A3C
	movs r0, #1
	rsbs r0, r0, #0
	cmp r2, r0
	bne _08035A24
	movs r0, #0xa
	str r0, [r1]
_08035A24:
	ldr r0, [r1]
	subs r0, #1
	str r0, [r1]
	cmp r0, #0
	bne _08035A3C
	ldr r0, _08035AB8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x4a
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
_08035A3C:
	ldr r1, [r6, #0x14]
	movs r0, #0x80
	lsls r0, r0, #0x11
	bl sub_803ADB4
	lsls r3, r0, #0x10
	lsrs r3, r3, #0x10
	ldr r5, _08035A98 @ =gUnknown_03001300
	mov ip, r5
	ldr r4, [r5]
	mov r1, sb
	lsls r2, r1, #2
	lsls r1, r1, #5
	adds r1, r4, r1
	strh r3, [r1, #0x12]
	adds r1, r2, #3
	lsls r1, r1, #3
	adds r1, r4, r1
	strh r3, [r1, #0x12]
	adds r1, r2, #1
	lsls r1, r1, #3
	adds r1, r4, r1
	mov r3, r8
	strh r3, [r1, #0x12]
	adds r2, #2
	lsls r2, r2, #3
	adds r4, r4, r2
	strh r3, [r4, #0x12]
	mov r1, sp
	strh r3, [r1]
	ldr r2, _08035A9C @ =0x040000D4
	str r1, [r2]
	add r3, sp, #0xc
	str r3, [r2, #4]
	ldr r1, _08035AA0 @ =0x81000004
	str r1, [r2, #8]
	ldr r1, [r2, #8]
	movs r4, #0
	movs r1, #0x80
	lsls r1, r1, #1
	cmp r0, r1
	beq _08035ABC
	subs r4, #0x20
	ldrb r0, [r3, #1]
	movs r1, #3
	b _08035AC8
	.align 2, 0
_08035A98: .4byte gUnknown_03001300
_08035A9C: .4byte 0x040000D4
_08035AA0: .4byte 0x81000004
_08035AA4: .4byte 0xFFFFFC00
_08035AA8: .4byte 0x000001FF
_08035AAC: .4byte 0xFFFFFE00
_08035AB0: .4byte 0x000003FF
_08035AB4: .4byte 0xFFFFFE84
_08035AB8: .4byte gUnknown_030012BC
_08035ABC:
	ldrb r0, [r3, #1]
	movs r5, #4
	rsbs r5, r5, #0
	adds r1, r5, #0
	ands r0, r1
	movs r1, #1
_08035AC8:
	orrs r0, r1
	strb r0, [r3, #1]
	movs r0, #7
	mov r1, sb
	ands r1, r0
	lsls r1, r1, #1
	ldrb r2, [r3, #3]
	movs r5, #0xf
	rsbs r5, r5, #0
	adds r0, r5, #0
	ands r2, r0
	orrs r2, r1
	movs r0, #1
	add sb, r0
	movs r0, #0xf
	ldrb r1, [r3, #5]
	ands r0, r1
	strb r0, [r3, #5]
	lsls r1, r7, #6
	ldr r5, _08035B88 @ =0x000003FF
	adds r0, r5, #0
	ands r1, r0
	ldr r5, _08035B8C @ =0xFFFFFC00
	adds r0, r5, #0
	ldrh r5, [r3, #4]
	ands r0, r5
	orrs r0, r1
	strh r0, [r3, #4]
	movs r0, #0xc0
	orrs r2, r0
	strb r2, [r3, #3]
	movs r0, #0xa
	ldrsh r2, [r6, r0]
	adds r0, r4, #0
	subs r0, #0x20
	adds r2, r2, r0
	ldr r1, _08035B90 @ =0x000001FF
	adds r0, r1, #0
	ands r2, r0
	ldrh r0, [r3, #2]
	ldr r5, _08035B94 @ =0xFFFFFE00
	adds r1, r5, #0
	ands r0, r1
	orrs r0, r2
	strh r0, [r3, #2]
	movs r1, #0xe
	ldrsh r0, [r6, r1]
	subs r0, #0x20
	adds r0, r0, r4
	add r1, sp, #0xc
	strb r0, [r1]
	mov r2, ip
	ldr r0, [r2]
	adds r1, r3, #0
	bl sub_8006AC8
_08035B38:
	adds r7, #1
	cmp r7, #4
	bgt _08035B40
	b _080359F0
_08035B40:
	ldr r3, _08035B98 @ =gStaticData_0817CFF4
	str r3, [sp, #0x1c]
	movs r4, #0
	ldr r5, _08035B9C @ =0x040000D4
	mov sb, r5
	ldr r5, [sp, #0x24]
_08035B4C:
	movs r0, #0x34
	muls r0, r4, r0
	adds r0, #0x10
	add r0, sl
	str r0, [sp, #0x28]
	ldrb r0, [r0]
	cmp r0, #0
	beq _08035BB6
	lsls r1, r4, #2
	movs r0, #0xf2
	lsls r0, r0, #1
	add r0, sl
	adds r2, r0, r1
	ldr r1, [r2]
	cmp r1, #0
	beq _08035BB6
	movs r0, #1
	rsbs r0, r0, #0
	cmp r1, r0
	bne _08035BA4
	movs r0, #8
	str r0, [r2]
	ldr r0, _08035BA0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x3d
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	b _08035BB6
	.align 2, 0
_08035B88: .4byte 0x000003FF
_08035B8C: .4byte 0xFFFFFC00
_08035B90: .4byte 0x000001FF
_08035B94: .4byte 0xFFFFFE00
_08035B98: .4byte gStaticData_0817CFF4
_08035B9C: .4byte 0x040000D4
_08035BA0: .4byte gUnknown_030012BC
_08035BA4:
	subs r0, r1, #1
	str r0, [r2]
	cmp r0, #0
	bne _08035BB6
	movs r1, #0x83
	lsls r1, r1, #2
	add r1, sl
	movs r0, #0x1e
	str r0, [r1]
_08035BB6:
	mov r1, sp
	movs r0, #0
	strh r0, [r1]
	mov r7, sb
	str r1, [r7]
	str r5, [r7, #4]
	ldr r0, _08035D04 @ =0x81000004
	str r0, [r7, #8]
	ldr r0, [r7, #8]
	adds r2, r4, #1
	lsls r1, r2, #4
	movs r0, #0xf
	ldrb r3, [r5, #5]
	ands r0, r3
	orrs r0, r1
	strb r0, [r5, #5]
	lsls r1, r4, #6
	movs r7, #0xa0
	lsls r7, r7, #1
	adds r1, r1, r7
	ldr r3, _08035D08 @ =0x000003FF
	adds r0, r3, #0
	ands r1, r0
	ldr r7, _08035D0C @ =0xFFFFFC00
	adds r0, r7, #0
	ldrh r3, [r5, #4]
	ands r0, r3
	orrs r0, r1
	strh r0, [r5, #4]
	ldrb r1, [r5, #3]
	movs r0, #0x3f
	ands r0, r1
	movs r1, #0x80
	orrs r0, r1
	strb r0, [r5, #3]
	movs r7, #0xd
	rsbs r7, r7, #0
	adds r0, r7, #0
	ldrb r1, [r5, #5]
	ands r0, r1
	movs r1, #8
	orrs r0, r1
	strb r0, [r5, #5]
	mov r8, r2
	ldr r4, [sp, #0x24]
	movs r6, #3
_08035C12:
	ldr r2, [sp, #0x28]
	movs r3, #0xa
	ldrsh r1, [r2, r3]
	ldr r7, [sp, #0x1c]
	ldm r7!, {r0}
	adds r2, r1, r0
	ldr r0, [sp, #0x28]
	movs r3, #0xe
	ldrsh r1, [r0, r3]
	ldm r7!, {r0}
	str r7, [sp, #0x1c]
	adds r3, r1, r0
	cmp r3, #0x8b
	bgt _08035C56
	ldr r1, _08035D10 @ =0x000001FF
	adds r0, r1, #0
	ands r2, r0
	ldrh r0, [r4, #2]
	ldr r7, _08035D14 @ =0xFFFFFE00
	adds r1, r7, #0
	ands r0, r1
	orrs r0, r2
	strh r0, [r4, #2]
	add r0, sp, #0x14
	strb r3, [r0]
	ldr r1, [sp, #0x28]
	ldrb r0, [r1]
	cmp r0, #0
	beq _08035C56
	ldr r0, _08035D18 @ =gUnknown_03001300
	ldr r0, [r0]
	ldr r1, [sp, #0x24]
	bl sub_8006AC8
_08035C56:
	ldrh r2, [r4, #4]
	lsls r0, r2, #0x16
	lsrs r0, r0, #0x16
	adds r0, #0x10
	ldr r3, _08035D08 @ =0x000003FF
	adds r1, r3, #0
	ands r0, r1
	ldr r7, _08035D0C @ =0xFFFFFC00
	adds r1, r7, #0
	ands r2, r1
	orrs r2, r0
	strh r2, [r4, #4]
	subs r6, #1
	cmp r6, #0
	bge _08035C12
	mov r4, r8
	cmp r4, #1
	bgt _08035C7C
	b _08035B4C
_08035C7C:
	ldr r4, [sp, #0x20]
	ldrb r0, [r4]
	cmp r0, #0
	beq _08035CF2
	bl sub_80015B0
	ldr r5, [r4, #8]
	ldr r6, [r4, #0xc]
	movs r1, #0x83
	lsls r1, r1, #2
	add r1, sl
	ldr r0, [r1]
	cmp r0, #0
	beq _08035CB8
	subs r0, #1
	str r0, [r1]
	movs r0, #0xa
	bl sub_8000E1C
	subs r1, r5, #5
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r5, r1, r0
	movs r0, #0xa
	bl sub_8000E1C
	subs r1, r6, #5
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r6, r1, r0
_08035CB8:
	movs r4, #0x87
	lsls r4, r4, #2
	add r4, sl
	ldr r0, [sp, #0x20]
	ldr r1, [r0, #0x14]
	movs r0, #0x80
	lsls r0, r0, #0x11
	bl sub_803ADB4
	str r0, [r4]
	movs r3, #0x85
	lsls r3, r3, #2
	add r3, sl
	rsbs r1, r5, #0
	asrs r1, r1, #0x10
	muls r0, r1, r0
	movs r2, #0x80
	lsls r2, r2, #7
	adds r0, r0, r2
	str r0, [r3]
	movs r3, #0x86
	lsls r3, r3, #2
	add r3, sl
	rsbs r0, r6, #0
	asrs r0, r0, #0x10
	ldr r1, [r4]
	muls r0, r1, r0
	adds r0, r0, r2
	str r0, [r3]
_08035CF2:
	add sp, #0x2c
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08035D04: .4byte 0x81000004
_08035D08: .4byte 0x000003FF
_08035D0C: .4byte 0xFFFFFC00
_08035D10: .4byte 0x000001FF
_08035D14: .4byte 0xFFFFFE00
_08035D18: .4byte gUnknown_03001300

	thumb_func_start sub_8035D1C
sub_8035D1C: @ 0x08035D1C
	push {r4, lr}
	adds r3, r0, #0
	adds r2, r1, #0
	ldr r0, _08035D44 @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r4, #0x80
	lsls r4, r4, #1
	adds r1, r4, #0
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r1, r0, #0x10
	cmp r1, #0
	bne _08035D48
	movs r4, #0x84
	lsls r4, r4, #2
	adds r0, r3, r4
	str r1, [r0]
	adds r0, r2, #0
	b _08035E02
	.align 2, 0
_08035D44: .4byte gUnknown_030007E0
_08035D48:
	movs r0, #0x20
	ands r0, r2
	cmp r0, #0
	beq _08035D58
	ldr r1, _08035D54 @ =0x12345678
	b _08035DCA
	.align 2, 0
_08035D54: .4byte 0x12345678
_08035D58:
	movs r0, #0x10
	ands r0, r2
	cmp r0, #0
	beq _08035D70
	ldr r1, _08035D6C @ =0x31415926
	movs r4, #0x84
	lsls r4, r4, #2
	adds r2, r3, r4
	b _08035DD0
	.align 2, 0
_08035D6C: .4byte 0x31415926
_08035D70:
	movs r0, #0x40
	ands r0, r2
	cmp r0, #0
	beq _08035D80
	ldr r1, _08035D7C @ =0xC0DEBA1D
	b _08035DCA
	.align 2, 0
_08035D7C: .4byte 0xC0DEBA1D
_08035D80:
	movs r0, #0x80
	ands r0, r2
	cmp r0, #0
	beq _08035D98
	ldr r1, _08035D94 @ =0xDEADBEEF
	movs r4, #0x84
	lsls r4, r4, #2
	adds r2, r3, r4
	b _08035DD0
	.align 2, 0
_08035D94: .4byte 0xDEADBEEF
_08035D98:
	movs r0, #2
	ands r0, r2
	cmp r0, #0
	beq _08035DA8
	ldr r1, _08035DA4 @ =0xB1E4B1E4
	b _08035DCA
	.align 2, 0
_08035DA4: .4byte 0xB1E4B1E4
_08035DA8:
	movs r0, #1
	ands r0, r2
	cmp r0, #0
	beq _08035DC0
	ldr r1, _08035DBC @ =0x71839406
	movs r4, #0x84
	lsls r4, r4, #2
	adds r2, r3, r4
	b _08035DD0
	.align 2, 0
_08035DBC: .4byte 0x71839406
_08035DC0:
	movs r0, #8
	ands r0, r2
	cmp r0, #0
	beq _08035DE4
	ldr r1, _08035E08 @ =0x828A048B
_08035DCA:
	movs r0, #0x84
	lsls r0, r0, #2
	adds r2, r3, r0
_08035DD0:
	ldr r0, [r2]
	eors r0, r1
	lsls r1, r0, #1
	lsrs r0, r0, #0x1f
	orrs r1, r0
	lsls r0, r1, #6
	adds r0, r0, r1
	lsls r0, r0, #3
	adds r0, r0, r1
	str r0, [r2]
_08035DE4:
	movs r0, #0x84
	lsls r0, r0, #2
	adds r4, r3, r0
	ldr r1, [r4]
	ldr r0, _08035E0C @ =0x3034AF3B
	cmp r1, r0
	bne _08035E00
	ldr r0, _08035E10 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0xc
	bl sub_8001B54
	movs r0, #0
	str r0, [r4]
_08035E00:
	movs r0, #0
_08035E02:
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08035E08: .4byte 0x828A048B
_08035E0C: .4byte 0x3034AF3B
_08035E10: .4byte gUnknown_030012BC

	thumb_func_start sub_8035E14
sub_8035E14: @ 0x08035E14
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r4, r0, #0
	movs r3, #0
	ldr r7, _08035EF8 @ =gStaticData_0817CFA4
	mov r8, r7
	adds r5, r4, #0
	movs r6, #0
_08035E26:
	movs r0, #0
	mov ip, r0
	mov r1, ip
	strb r1, [r5, #0x10]
	adds r2, r4, #0
	adds r2, #0x14
	adds r2, r2, r6
	lsls r0, r3, #3
	mov r1, r8
	adds r1, #4
	adds r0, r0, r1
	ldr r0, [r0]
	adds r0, #1
	str r0, [r2]
	adds r0, r4, #0
	adds r0, #0x40
	adds r0, r0, r6
	ldr r1, [r7]
	str r1, [r0]
	lsls r1, r3, #2
	movs r2, #0xf2
	lsls r2, r2, #1
	adds r0, r4, r2
	adds r0, r0, r1
	movs r1, #1
	rsbs r1, r1, #0
	str r1, [r0]
	adds r7, #8
	adds r5, #0x34
	adds r6, #0x34
	adds r3, #1
	cmp r3, #8
	ble _08035E26
	movs r1, #0x83
	lsls r1, r1, #2
	adds r0, r4, r1
	mov r2, ip
	str r2, [r0]
	strb r2, [r4, #8]
	ldr r0, [r4, #0x14]
	cmp r0, #0
	beq _08035EA8
	ldr r5, _08035EFC @ =0x04000050
_08035E7C:
	adds r0, r4, #0
	bl sub_8035780
	adds r0, r4, #0
	bl sub_8036068
	movs r1, #0x82
	lsls r1, r1, #2
	adds r0, r4, r1
	ldr r0, [r0]
	bl sub_8034688
	bl sub_80006A8
	movs r0, #0
	str r0, [r5]
	adds r0, r4, #0
	bl sub_8035F9C
	ldr r0, [r4, #0x14]
	cmp r0, #0
	bne _08035E7C
_08035EA8:
	movs r0, #0
	movs r1, #1
	strb r1, [r4, #8]
	movs r2, #0x84
	lsls r2, r2, #2
	adds r1, r4, r2
	str r0, [r1]
	ldr r6, _08035F00 @ =gUnknown_030012BC
_08035EB8:
	adds r0, r4, #0
	bl sub_8036068
	movs r1, #0x82
	lsls r1, r1, #2
	adds r0, r4, r1
	ldr r0, [r0]
	bl sub_8034688
	ldr r0, _08035F04 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r0, _08035F08 @ =gUnknown_030007E0
	ldrh r5, [r0, #2]
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_8035D1C
	adds r5, r0, #0
	movs r0, #9
	ands r0, r5
	cmp r0, #0
	beq _08035F0C
	ldr r0, [r6]
	movs r1, #0x49
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	movs r5, #0
	b _08035F5C
	.align 2, 0
_08035EF8: .4byte gStaticData_0817CFA4
_08035EFC: .4byte 0x04000050
_08035F00: .4byte gUnknown_030012BC
_08035F04: .4byte gUnknown_03001304
_08035F08: .4byte gUnknown_030007E0
_08035F0C:
	movs r0, #0x40
	ands r0, r5
	cmp r0, #0
	beq _08035F2E
	ldr r0, [r6]
	movs r1, #0x46
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	ldr r0, [r4]
	cmp r0, #0
	beq _08035F2A
	subs r0, #1
	b _08035F2C
_08035F2A:
	movs r0, #2
_08035F2C:
	str r0, [r4]
_08035F2E:
	movs r0, #0x80
	ands r0, r5
	cmp r0, #0
	beq _08035F50
	ldr r0, [r6]
	movs r1, #0x46
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	ldr r0, [r4]
	adds r0, #1
	str r0, [r4]
	movs r1, #3
	bl sub_803AE4C
	str r0, [r4]
_08035F50:
	bl sub_80006A8
	adds r0, r4, #0
	bl sub_8035F9C
	b _08035EB8
_08035F5C:
	adds r0, r4, #0
	bl sub_8036068
	movs r2, #0x82
	lsls r2, r2, #2
	adds r0, r4, r2
	ldr r0, [r0]
	bl sub_8034688
	bl sub_80006A8
	ldr r0, _08035F94 @ =0x04000054
	strh r5, [r0]
	ldr r1, _08035F98 @ =0x04000050
	movs r0, #0xff
	strh r0, [r1]
	adds r0, r4, #0
	bl sub_8035F9C
	adds r5, #1
	cmp r5, #0x10
	ble _08035F5C
	ldr r0, [r4]
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08035F94: .4byte 0x04000054
_08035F98: .4byte 0x04000050

	thumb_func_start sub_8035F9C
sub_8035F9C: @ 0x08035F9C
	push {lr}
	adds r2, r0, #0
	ldr r1, _08035FE0 @ =0x04000028
	movs r3, #0x85
	lsls r3, r3, #2
	adds r0, r2, r3
	ldr r0, [r0]
	str r0, [r1]
	adds r1, #4
	adds r3, #4
	adds r0, r2, r3
	ldr r0, [r0]
	str r0, [r1]
	subs r1, #0xc
	adds r3, #4
	adds r0, r2, r3
	ldr r2, [r0]
	strh r2, [r1]
	ldr r0, _08035FE4 @ =0x04000022
	movs r1, #0
	strh r1, [r0]
	adds r0, #2
	strh r1, [r0]
	adds r0, #2
	strh r2, [r0]
	bl sub_8001614
	ldr r0, _08035FE8 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	pop {r0}
	bx r0
	.align 2, 0
_08035FE0: .4byte 0x04000028
_08035FE4: .4byte 0x04000022
_08035FE8: .4byte gUnknown_03001300

	thumb_func_start sub_8035FEC
sub_8035FEC: @ 0x08035FEC
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	adds r7, r1, #0
	adds r6, r2, #0
	ldr r0, [r5]
	cmp r6, r0
	bne _08036010
	ldr r1, [r5, #4]
	adds r1, #1
	str r1, [r5, #4]
	ldr r0, [r5, #0xc]
	asrs r1, r1, #2
	movs r2, #1
	ands r1, r2
	adds r1, #0xe
	bl sub_8028A30
	b _08036018
_08036010:
	ldr r0, [r5, #0xc]
	movs r1, #0xd
	bl sub_8028A30
_08036018:
	ldr r0, [r5, #0xc]
	movs r4, #0x98
	lsls r4, r4, #1
	adds r1, r0, r4
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	adds r1, r7, #0
	bl sub_803AD80
	movs r3, #0xf0
	subs r3, r3, r0
	asrs r3, r3, #1
	ldr r0, [r5, #0xc]
	lsls r1, r6, #2
	adds r1, r1, r6
	lsls r1, r1, #1
	adds r1, #0x80
	movs r5, #0x88
	lsls r5, r5, #1
	adds r2, r0, r5
	str r3, [r2]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r2, r0, r3
	str r1, [r2]
	adds r4, r0, r4
	ldr r2, [r4]
	movs r5, #0x20
	ldrsh r1, [r2, r5]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	adds r1, r7, #0
	bl sub_803AD80
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8036068
sub_8036068: @ 0x08036068
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r5, _080360BC @ =gUnknown_03001300
	ldr r0, [r5]
	bl sub_8006A90
	adds r0, r4, #0
	bl sub_80358A8
	ldrb r0, [r4, #8]
	cmp r0, #0
	beq _080360B0
	movs r0, #0x1a
	bl sub_8026F38
	adds r1, r0, #0
	adds r0, r4, #0
	movs r2, #0
	bl sub_8035FEC
	movs r0, #0x1b
	bl sub_8026F38
	adds r1, r0, #0
	adds r0, r4, #0
	movs r2, #1
	bl sub_8035FEC
	movs r0, #0x3b
	bl sub_8026F38
	adds r1, r0, #0
	adds r0, r4, #0
	movs r2, #2
	bl sub_8035FEC
_080360B0:
	ldr r0, [r5]
	bl sub_8006A48
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080360BC: .4byte gUnknown_03001300
	thumb_func_start sub_80360C0
sub_80360C0: @ 0x080360C0
	movs	r2, #132	@ 0x84
	lsls	r2, r2, #2
	adds	r0, r0, r2
	ldr	r2, [r0, #0]
	eors	r2, r1
	lsls	r3, r2, #1
	lsrs	r2, r2, #31
	orrs	r3, r2
	lsls	r1, r3, #6
	adds	r1, r1, r3
	lsls	r1, r1, #3
	adds	r1, r1, r3
	str	r1, [r0, #0]
	bx	lr

	thumb_func_start sub_80360DC
sub_80360DC: @ 0x080360DC
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r3, r0, #0
	movs r6, #0
	ldr r0, _08036150 @ =gStaticData_0817CFA4
	mov r8, r0
	movs r1, #0xf2
	lsls r1, r1, #1
	adds r1, r1, r3
	mov sb, r1
	mov r7, r8
	adds r5, r3, #0
	movs r4, #0
_080360FA:
	movs r0, #0
	mov ip, r0
	mov r1, ip
	strb r1, [r5, #0x10]
	adds r2, r3, #0
	adds r2, #0x14
	adds r2, r2, r4
	lsls r0, r6, #3
	mov r1, r8
	adds r1, #4
	adds r0, r0, r1
	ldr r0, [r0]
	adds r0, #1
	str r0, [r2]
	adds r0, r3, #0
	adds r0, #0x40
	adds r0, r0, r4
	ldr r1, [r7]
	str r1, [r0]
	movs r0, #1
	rsbs r0, r0, #0
	mov r1, sb
	adds r1, #4
	mov sb, r1
	subs r1, #4
	stm r1!, {r0}
	adds r7, #8
	adds r5, #0x34
	adds r4, #0x34
	adds r6, #1
	cmp r6, #8
	ble _080360FA
	movs r1, #0x83
	lsls r1, r1, #2
	adds r0, r3, r1
	mov r1, ip
	str r1, [r0]
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08036150: .4byte gStaticData_0817CFA4

	thumb_func_start sub_8036154
sub_8036154: @ 0x08036154
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	movs r1, #0x82
	lsls r1, r1, #2
	adds r0, r4, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _0803616C
	movs r1, #3
	bl sub_80346FC
_0803616C:
	ldr r1, _080361A8 @ =gUnknown_03001288
	movs r0, #0
	strh r0, [r1]
	bl sub_8001614
	movs r2, #0
	movs r1, #0xa0
	lsls r1, r1, #0x13
	movs r0, #0xff
_0803617E:
	strh r2, [r1]
	adds r1, #2
	subs r0, #1
	cmp r0, #0
	bge _0803617E
	ldr r1, _080361AC @ =0x04000050
	movs r0, #0xff
	strh r0, [r1]
	adds r1, #4
	movs r0, #0x10
	strh r0, [r1]
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _080361A2
	adds r0, r4, #0
	bl sub_8026ED0
_080361A2:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080361A8: .4byte gUnknown_03001288
_080361AC: .4byte 0x04000050

	thumb_func_start sub_80361B0
sub_80361B0: @ 0x080361B0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	adds r6, r0, #0
	ldr r0, _08036220 @ =0x06010000
	bl InitObjTileFreeList
	bl InitSpriteFrameOamQueue
	bl InitSpriteFrameCache
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _08036224 @ =gStaticData_0817D698
	bl sub_8036E20
	mov r8, r0
	ldr r1, _08036228 @ =0x040000D4
	ldr r0, _0803622C @ =gStaticData_08178F80
	str r0, [r1]
	ldr r0, _08036230 @ =0x05000200
	str r0, [r1, #4]
	ldr r0, _08036234 @ =0x80000100
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	adds r0, r6, #0
	bl sub_8036528
	adds r0, r6, #0
	bl sub_8036600
	movs r0, #0x14
	bl sub_8026EDC
	bl sub_8034374
	mov sb, r0
	adds r0, r6, #0
	bl sub_8036CF4
	movs r4, #0
	ldr r5, _08036238 @ =0x04000050
	ldr r7, _0803623C @ =0x04000054
_08036210:
	cmp r4, #0x10
	bgt _08036240
	movs r0, #0xff
	strh r0, [r5]
	movs r0, #0x10
	subs r0, r0, r4
	strh r0, [r7]
	b _08036244
	.align 2, 0
_08036220: .4byte 0x06010000
_08036224: .4byte gStaticData_0817D698
_08036228: .4byte 0x040000D4
_0803622C: .4byte gStaticData_08178F80
_08036230: .4byte 0x05000200
_08036234: .4byte 0x80000100
_08036238: .4byte 0x04000050
_0803623C: .4byte 0x04000054
_08036240:
	movs r0, #0
	str r0, [r5]
_08036244:
	bl sub_80006A8
	mov r0, sb
	bl sub_8034688
	adds r4, #1
	cmp r4, #0x3b
	ble _08036210
	ldr r0, _08036308 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x4b
	bl PlaySfx
	movs r5, #0x80
	lsls r5, r5, #6
	ldr r0, _0803630C @ =0x00000444
	adds r1, r6, r0
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r1]
_08036270:
	ldr r0, _08036310 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r1, _08036314 @ =gUnknown_030007E0
	movs r0, #9
	ldrh r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _08036292
	ldr r2, _0803630C @ =0x00000444
	adds r1, r6, r2
	ldr r0, [r1]
	cmp r0, #0x40
	ble _08036292
	movs r0, #0x40
	str r0, [r1]
_08036292:
	bl sub_80006A8
	bl sub_8001614
	ldr r3, _0803630C @ =0x00000444
	adds r4, r6, r3
	ldr r0, [r4]
	movs r7, #1
	rsbs r7, r7, #0
	mov sl, r7
	cmp r0, sl
	beq _080362E0
	cmp r0, #0x40
	bne _080362BC
	ldr r0, _08036308 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x4c
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
_080362BC:
	ldr r3, [r4]
	cmp r3, #0x40
	bgt _080362D8
	asrs r2, r3, #2
	ldr r1, _08036318 @ =0x04000050
	ldr r7, _0803631C @ =0x00003F7F
	adds r0, r7, #0
	strh r0, [r1]
	adds r1, #2
	movs r0, #0x10
	subs r0, r0, r2
	lsls r0, r0, #8
	orrs r2, r0
	strh r2, [r1]
_080362D8:
	subs r2, r3, #1
	str r2, [r4]
	cmp r2, sl
	bne _08036324
_080362E0:
	ldr r0, _08036320 @ =0x0000FFFF
	cmp r5, r0
	bgt _080362F0
	movs r1, #0xc0
	lsls r1, r1, #3
	adds r5, r5, r1
	cmp r5, r0
	ble _08036334
_080362F0:
	movs r5, #0x80
	lsls r5, r5, #9
	ldr r3, _0803630C @ =0x00000444
	adds r2, r6, r3
	ldr r1, [r2]
	movs r0, #1
	rsbs r0, r0, #0
	cmp r1, r0
	bne _08036334
	movs r0, #0xf4
	str r0, [r2]
	b _08036334
	.align 2, 0
_08036308: .4byte gUnknown_030012BC
_0803630C: .4byte 0x00000444
_08036310: .4byte gUnknown_03001304
_08036314: .4byte gUnknown_030007E0
_08036318: .4byte 0x04000050
_0803631C: .4byte 0x00003F7F
_08036320: .4byte 0x0000FFFF
_08036324:
	cmp r2, #0x40
	bgt _08036334
	lsls r0, r5, #3
	adds r0, r0, r5
	lsls r0, r0, #2
	subs r0, r0, r5
	lsls r0, r0, #3
	asrs r5, r0, #8
_08036334:
	movs r0, #0x80
	lsls r0, r0, #0x11
	adds r1, r5, #0
	bl sub_803ADB4
	lsls r3, r0, #4
	subs r3, r3, r0
	lsls r3, r3, #3
	rsbs r3, r3, #0
	movs r7, #0xf0
	lsls r7, r7, #7
	adds r3, r3, r7
	lsls r1, r0, #2
	adds r1, r1, r0
	lsls r1, r1, #4
	rsbs r1, r1, #0
	movs r2, #0xa0
	lsls r2, r2, #7
	adds r1, r1, r2
	ldr r2, _08036458 @ =0x04000028
	str r3, [r2]
	adds r2, #4
	str r1, [r2]
	ldr r1, _0803645C @ =0x04000020
	strh r0, [r1]
	adds r1, #6
	strh r0, [r1]
	ldr r0, _08036460 @ =0x04000022
	movs r1, #0
	strh r1, [r0]
	adds r0, #2
	strh r1, [r0]
	mov r0, sb
	bl sub_8034688
	ldr r3, _08036464 @ =0x00000444
	adds r0, r6, r3
	ldr r4, [r0]
	cmp r4, #0
	beq _08036386
	b _08036270
_08036386:
	ldr r2, _08036468 @ =gUnknown_03001288
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r7, [r2, #1]
	ands r0, r7
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r2, #1]
	movs r0, #0x40
	ldrb r1, [r2]
	orrs r0, r1
	strb r0, [r2]
	bl sub_8001614
	ldr r0, _0803646C @ =0x04000050
	str r4, [r0]
	ldr r3, _08036464 @ =0x00000444
	adds r2, r6, r3
	movs r1, #1
	rsbs r1, r1, #0
	str r1, [r2]
	movs r7, #0x89
	lsls r7, r7, #3
	adds r0, r6, r7
	str r1, [r0]
	ldr r0, [r2]
	cmp r0, #0
	beq _080364B4
_080363BE:
	ldr r0, _08036470 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r1, _08036474 @ =gUnknown_030007E0
	movs r0, #9
	ldrh r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _080363E2
	movs r0, #0x89
	lsls r0, r0, #3
	adds r1, r6, r0
	ldr r0, [r1]
	cmp r0, #0
	ble _080363E2
	movs r0, #1
	str r0, [r1]
_080363E2:
	mov r2, r8
	ldr r1, [r2, #0x50]
	movs r3, #0x10
	ldrsh r0, [r1, r3]
	add r0, r8
	ldr r1, [r1, #0x14]
	bl sub_803AD7C
	mov r7, r8
	ldr r1, [r7, #0x50]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	add r0, r8
	ldr r1, [r1, #0x1c]
	bl sub_803AD7C
	ldr r0, _08036478 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006A78
	bl FlushSpriteFrameOamQueue
	adds r0, r6, #0
	bl sub_8036668
	adds r0, r6, #0
	bl sub_803686C
	mov r0, sb
	bl sub_8034688
	bl sub_80006A8
	ldr r3, _08036464 @ =0x00000444
	adds r4, r6, r3
	ldr r1, [r4]
	cmp r1, #0x10
	ble _08036484
	subs r3, r1, #1
	str r3, [r4]
	subs r1, #0x12
	ldr r5, _0803646C @ =0x04000050
	ldr r7, _0803647C @ =0x00003F7F
	adds r0, r7, #0
	strh r0, [r5]
	ldr r2, _08036480 @ =0x04000052
	movs r0, #0x10
	subs r0, r0, r1
	lsls r1, r1, #8
	orrs r0, r1
	strh r0, [r2]
	cmp r3, #0x11
	bne _0803649A
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r4]
	movs r0, #0
	str r0, [r5]
	b _0803649A
	.align 2, 0
_08036458: .4byte 0x04000028
_0803645C: .4byte 0x04000020
_08036460: .4byte 0x04000022
_08036464: .4byte 0x00000444
_08036468: .4byte gUnknown_03001288
_0803646C: .4byte 0x04000050
_08036470: .4byte gUnknown_03001304
_08036474: .4byte gUnknown_030007E0
_08036478: .4byte gUnknown_03001300
_0803647C: .4byte 0x00003F7F
_08036480: .4byte 0x04000052
_08036484:
	cmp r1, #0
	blt _0803649A
	subs r1, #1
	str r1, [r4]
	ldr r2, _08036514 @ =0x04000054
	movs r0, #0x10
	subs r0, r0, r1
	strh r0, [r2]
	ldr r1, _08036518 @ =0x04000050
	movs r0, #0xff
	strh r0, [r1]
_0803649A:
	ldr r0, _0803651C @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	bl AgeSpriteFrameCache
	ldr r1, _08036520 @ =0x00000444
	adds r0, r6, r1
	ldr r0, [r0]
	cmp r0, #0
	bne _080363BE
_080364B4:
	mov r2, r8
	cmp r2, #0
	beq _080364CA
	ldr r1, [r2, #0x50]
	movs r3, #8
	ldrsh r0, [r1, r3]
	add r0, r8
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_080364CA:
	mov r7, sb
	cmp r7, #0
	beq _080364D8
	mov r0, sb
	movs r1, #3
	bl sub_80346FC
_080364D8:
	ldr r1, _08036524 @ =0x00000434
	adds r0, r6, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _080364E6
	bl sub_8026EB4
_080364E6:
	movs r2, #0x86
	lsls r2, r2, #3
	adds r0, r6, r2
	ldr r0, [r0]
	cmp r0, #0
	beq _080364F6
	bl sub_8026EB4
_080364F6:
	bl FreeSpriteFrameCache
	bl FreeSpriteFrameOamQueue
	bl FreeObjTileFreeList
	bl FreeCategorySpriteSheet
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08036514: .4byte 0x04000054
_08036518: .4byte 0x04000050
_0803651C: .4byte gUnknown_03001300
_08036520: .4byte 0x00000444
_08036524: .4byte 0x00000434

	thumb_func_start sub_8036528
sub_8036528: @ 0x08036528
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	adds r4, r0, #0
	movs r0, #0x90
	lsls r0, r0, #5
	bl AllocVramTileBlock
	ldr r1, _080365DC @ =0x00000424
	adds r1, r1, r4
	mov sb, r1
	str r0, [r1]
	movs r0, #0x80
	lsls r0, r0, #3
	bl AllocVramTileBlock
	movs r2, #0x85
	lsls r2, r2, #3
	adds r2, r2, r4
	mov sl, r2
	str r0, [r2]
	movs r7, #0x80
	lsls r7, r7, #5
	adds r0, r7, #0
	bl AllocVramTileBlock
	ldr r2, _080365E0 @ =0x0000042C
	adds r1, r4, r2
	str r0, [r1]
	ldr r0, _080365E4 @ =gStaticData_0817D768
	mov r8, r0
	ldr r1, [r0, #8]
	ldr r2, _080365E8 @ =0x050003E0
	adds r0, r4, #0
	bl sub_8037110
	ldr r6, _080365EC @ =gStaticData_0817D77C
	ldr r1, [r6, #8]
	ldr r2, _080365F0 @ =0x050003C0
	adds r0, r4, #0
	bl sub_8037110
	ldr r5, _080365F4 @ =gStaticData_0817D790
	ldr r1, [r5, #8]
	ldr r2, _080365F8 @ =0x050003A0
	adds r0, r4, #0
	bl sub_8037110
	ldr r1, [r6, #0xc]
	mov r0, sb
	ldr r2, [r0]
	adds r0, r4, #0
	bl sub_8037110
	ldr r1, [r5, #0xc]
	mov r0, sl
	ldr r2, [r0]
	adds r0, r4, #0
	bl sub_8037110
	mov r1, r8
	ldr r0, [r1, #0xc]
	ldr r0, [r0]
	lsrs r0, r0, #8
	movs r2, #0x86
	lsls r2, r2, #3
	adds r5, r4, r2
	bl sub_8026EC0
	adds r1, r0, #0
	str r1, [r5]
	mov r2, r8
	ldr r0, [r2, #0xc]
	bl LoadTaggedAsset
	ldr r0, _080365FC @ =0x00000434
	adds r4, r4, r0
	adds r0, r7, #0
	bl sub_8026EC0
	str r0, [r4]
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080365DC: .4byte 0x00000424
_080365E0: .4byte 0x0000042C
_080365E4: .4byte gStaticData_0817D768
_080365E8: .4byte 0x050003E0
_080365EC: .4byte gStaticData_0817D77C
_080365F0: .4byte 0x050003C0
_080365F4: .4byte gStaticData_0817D790
_080365F8: .4byte 0x050003A0
_080365FC: .4byte 0x00000434

	thumb_func_start sub_8036600
sub_8036600: @ 0x08036600
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	movs r6, #0
	movs r7, #0
	ldr r3, _0803665C @ =gStaticData_0817D6C0
	adds r2, r5, #0
	adds r4, r3, #4
	adds r1, r5, #4
_08036610:
	strb r7, [r2]
	ldr r0, [r4]
	adds r0, #1
	str r0, [r1]
	ldr r0, [r3]
	str r0, [r1, #0x2c]
	adds r3, #8
	adds r2, #0x34
	adds r1, #0x34
	adds r4, #8
	adds r6, #1
	cmp r6, #0x13
	ble _08036610
	movs r2, #1
	movs r1, #0x11
	ldr r3, _08036660 @ =0x00000421
	adds r0, r5, r3
_08036632:
	strb r2, [r0]
	subs r0, #1
	subs r1, #1
	cmp r1, #0
	bge _08036632
	movs r1, #0x87
	lsls r1, r1, #3
	adds r0, r5, r1
	movs r1, #0
	str r1, [r0]
	ldr r2, _08036664 @ =0x0000043C
	adds r0, r5, r2
	str r1, [r0]
	movs r3, #0x88
	lsls r3, r3, #3
	adds r0, r5, r3
	str r1, [r0]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0803665C: .4byte gStaticData_0817D6C0
_08036660: .4byte 0x00000421
_08036664: .4byte 0x0000043C

	thumb_func_start sub_8036668
sub_8036668: @ 0x08036668
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	movs r0, #0x89
	lsls r0, r0, #3
	adds r2, r4, r0
	ldr r1, [r2]
	movs r0, #1
	rsbs r0, r0, #0
	cmp r1, r0
	beq _0803667E
	b _080367DC
_0803667E:
	movs r6, #0
	adds r1, r4, #4
	mov ip, r1
	adds r7, r2, #0
_08036686:
	movs r0, #0x34
	adds r3, r6, #0
	muls r3, r0, r3
	mov r0, ip
	adds r5, r0, r3
	ldr r0, [r5]
	cmp r0, #0
	bne _08036698
	b _08036798
_08036698:
	subs r0, #1
	str r0, [r5]
	cmp r0, #0
	bne _08036732
	adds r1, r4, #0
	adds r1, #0x30
	adds r1, r1, r3
	ldr r0, [r1]
	adds r2, r0, #0
	adds r0, #0x20
	str r0, [r1]
	adds r1, r4, r3
	movs r0, #1
	strb r0, [r1]
	movs r1, #0
	ldrsh r0, [r2, r1]
	str r0, [r5]
	cmp r0, #0
	beq _0803679E
	adds r0, r4, #0
	adds r0, #0x10
	adds r0, r0, r3
	ldrh r5, [r2, #6]
	lsls r1, r5, #0x10
	str r1, [r0]
	adds r0, r4, #0
	adds r0, #0x24
	adds r0, r0, r3
	ldr r1, [r2, #0x14]
	str r1, [r0]
	adds r1, r4, #0
	adds r1, #0x14
	adds r1, r1, r3
	movs r5, #8
	ldrsh r0, [r2, r5]
	lsls r0, r0, #8
	str r0, [r1]
	adds r0, r4, #0
	adds r0, #0x28
	adds r0, r0, r3
	ldr r1, [r2, #0x18]
	str r1, [r0]
	adds r1, r4, #0
	adds r1, #0x18
	adds r1, r1, r3
	movs r5, #0xa
	ldrsh r0, [r2, r5]
	lsls r0, r0, #8
	str r0, [r1]
	adds r0, r4, #0
	adds r0, #0x2c
	adds r0, r0, r3
	ldr r1, [r2, #0x1c]
	str r1, [r0]
	adds r0, r4, #0
	adds r0, #8
	adds r0, r0, r3
	ldrh r5, [r2, #2]
	lsls r1, r5, #0x10
	str r1, [r0]
	adds r0, r4, #0
	adds r0, #0x1c
	adds r0, r0, r3
	ldr r1, [r2, #0xc]
	str r1, [r0]
	adds r0, r4, #0
	adds r0, #0xc
	adds r0, r0, r3
	ldrh r5, [r2, #4]
	lsls r1, r5, #0x10
	str r1, [r0]
	adds r0, r4, #0
	adds r0, #0x20
	adds r0, r0, r3
	ldr r1, [r2, #0x10]
	str r1, [r0]
	b _0803679E
_08036732:
	adds r2, r4, #0
	adds r2, #0x10
	adds r2, r2, r3
	adds r0, r4, #0
	adds r0, #0x24
	adds r0, r0, r3
	ldr r1, [r2]
	ldr r0, [r0]
	adds r1, r1, r0
	str r1, [r2]
	adds r2, r4, #0
	adds r2, #0x14
	adds r2, r2, r3
	adds r0, r4, #0
	adds r0, #0x28
	adds r0, r0, r3
	ldr r1, [r2]
	ldr r0, [r0]
	adds r1, r1, r0
	str r1, [r2]
	adds r2, r4, #0
	adds r2, #0x18
	adds r2, r2, r3
	adds r0, r4, #0
	adds r0, #0x2c
	adds r0, r0, r3
	ldr r1, [r2]
	ldr r0, [r0]
	adds r1, r1, r0
	str r1, [r2]
	adds r2, r4, #0
	adds r2, #8
	adds r2, r2, r3
	adds r0, r4, #0
	adds r0, #0x1c
	adds r0, r0, r3
	ldr r1, [r2]
	ldr r0, [r0]
	adds r1, r1, r0
	str r1, [r2]
	adds r2, r4, #0
	adds r2, #0xc
	adds r2, r2, r3
	adds r0, r4, #0
	adds r0, #0x20
	adds r0, r0, r3
	ldr r1, [r2]
	ldr r0, [r0]
	adds r1, r1, r0
	str r1, [r2]
	b _0803679E
_08036798:
	movs r0, #2
	rsbs r0, r0, #0
	str r0, [r7]
_0803679E:
	adds r6, #1
	cmp r6, #0x13
	bgt _080367A6
	b _08036686
_080367A6:
	ldr r0, _08036858 @ =0x0000043C
	adds r2, r4, r0
	ldr r0, [r2]
	cmp r0, #1
	bgt _080367DC
	movs r5, #0x88
	lsls r5, r5, #3
	adds r1, r4, r5
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
	cmp r0, #3
	ble _080367DC
	movs r3, #0
	str r3, [r1]
	movs r0, #0x87
	lsls r0, r0, #3
	adds r1, r4, r0
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
	cmp r0, #9
	ble _080367DC
	str r3, [r1]
	ldr r0, [r2]
	adds r0, #1
	str r0, [r2]
_080367DC:
	movs r1, #0x89
	lsls r1, r1, #3
	adds r5, r4, r1
	ldr r1, [r5]
	movs r0, #2
	rsbs r0, r0, #0
	cmp r1, r0
	bne _080367F0
	movs r0, #0xf0
	str r0, [r5]
_080367F0:
	ldr r0, [r5]
	cmp r0, #0
	ble _0803680C
	subs r0, #1
	str r0, [r5]
	cmp r0, #0
	bne _08036850
	ldr r0, _0803685C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x50
	bl PlaySfx
_0803680C:
	ldr r0, [r5]
	cmp r0, #0
	bne _08036850
	movs r2, #1
	adds r1, r4, #0
	movs r5, #0xf7
	lsls r5, r5, #2
	adds r3, r4, r5
	ldr r6, _08036860 @ =0xFFF80000
	ldr r5, _08036864 @ =0xFF810000
_08036820:
	ldrb r0, [r1]
	cmp r0, #0
	beq _08036834
	movs r2, #0
	ldr r0, [r1, #8]
	adds r0, r0, r6
	str r0, [r1, #8]
	cmp r0, r5
	bge _08036834
	strb r2, [r1]
_08036834:
	adds r1, #0x34
	cmp r1, r3
	ble _08036820
	cmp r2, #0
	beq _08036850
	ldr r0, _08036868 @ =0x00000444
	adds r1, r4, r0
	movs r0, #0x10
	str r0, [r1]
	movs r5, #0x89
	lsls r5, r5, #3
	adds r1, r4, r5
	subs r0, #0x13
	str r0, [r1]
_08036850:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08036858: .4byte 0x0000043C
_0803685C: .4byte gUnknown_030012BC
_08036860: .4byte 0xFFF80000
_08036864: .4byte 0xFF810000
_08036868: .4byte 0x00000444

	thumb_func_start sub_803686C
sub_803686C: @ 0x0803686C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x28
	mov sb, r0
	movs r0, #1
	str r0, [sp, #0x20]
	movs r5, #0xf7
	lsls r5, r5, #2
	add r5, sb
	ldrb r0, [r5]
	cmp r0, #0
	beq _08036938
	mov r1, sp
	movs r0, #0
	strh r0, [r1]
	ldr r1, _08036BFC @ =0x040000D4
	mov r2, sp
	str r2, [r1]
	add r3, sp, #8
	str r3, [r1, #4]
	ldr r0, _08036C00 @ =0x81000004
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	movs r0, #0x85
	lsls r0, r0, #3
	add r0, sb
	ldr r4, [r0]
	movs r0, #0xf
	ldrb r7, [r3, #5]
	ands r0, r7
	movs r1, #0xd0
	orrs r0, r1
	strb r0, [r3, #5]
	ldrb r2, [r3, #3]
	movs r1, #0x3f
	adds r0, r1, #0
	ands r0, r2
	movs r2, #0x80
	orrs r0, r2
	strb r0, [r3, #3]
	ldrb r0, [r3, #1]
	ands r1, r0
	movs r0, #0x40
	orrs r1, r0
	strb r1, [r3, #1]
	movs r1, #0xe
	ldrsh r0, [r5, r1]
	strb r0, [r3]
	movs r2, #0xa
	ldrsh r1, [r5, r2]
	ldr r7, _08036C04 @ =0x000001FF
	adds r0, r7, #0
	ands r1, r0
	ldrh r2, [r3, #2]
	ldr r0, _08036C08 @ =0xFFFFFE00
	ands r0, r2
	orrs r0, r1
	strh r0, [r3, #2]
	lsls r4, r4, #0x11
	lsrs r4, r4, #0x16
	ldr r0, _08036C0C @ =0xFFFFFC00
	ldrh r1, [r3, #4]
	ands r0, r1
	orrs r0, r4
	strh r0, [r3, #4]
	adds r4, r3, #0
	movs r5, #3
_080368F8:
	ldr r0, _08036C10 @ =gUnknown_03001300
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8006AC8
	ldrh r2, [r4, #4]
	lsls r0, r2, #0x16
	lsrs r0, r0, #0x16
	adds r0, #8
	ldr r3, _08036C14 @ =0x000003FF
	adds r1, r3, #0
	ands r0, r1
	ldr r7, _08036C0C @ =0xFFFFFC00
	adds r1, r7, #0
	ands r2, r1
	orrs r2, r0
	strh r2, [r4, #4]
	ldrh r2, [r4, #2]
	lsls r0, r2, #0x17
	lsrs r0, r0, #0x17
	adds r0, #0x20
	ldr r3, _08036C04 @ =0x000001FF
	adds r1, r3, #0
	ands r0, r1
	ldr r7, _08036C08 @ =0xFFFFFE00
	adds r1, r7, #0
	ands r2, r1
	orrs r2, r0
	strh r2, [r4, #2]
	subs r5, #1
	cmp r5, #0
	bge _080368F8
_08036938:
	mov r7, sb
	adds r7, #0x34
	ldr r0, _08036C18 @ =0x00000424
	add r0, sb
	ldr r0, [r0]
	ldr r1, _08036C1C @ =0xF9FF0000
	adds r0, r0, r1
	lsrs r0, r0, #5
	mov r8, r0
	movs r2, #0
	mov sl, r2
_0803694E:
	ldrb r0, [r7]
	cmp r0, #0
	bne _08036956
	b _08036A70
_08036956:
	mov r1, sp
	movs r0, #0
	strh r0, [r1]
	ldr r1, _08036BFC @ =0x040000D4
	mov r3, sp
	str r3, [r1]
	add r5, sp, #0x10
	str r5, [r1, #4]
	ldr r0, _08036C00 @ =0x81000004
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r1, [r7, #0x14]
	movs r4, #0x80
	lsls r4, r4, #0x11
	adds r0, r4, #0
	bl sub_803ADB4
	adds r6, r0, #0
	ldr r1, [r7, #0x18]
	adds r0, r4, #0
	bl sub_803ADB4
	adds r4, r0, #0
	movs r1, #0
	movs r0, #0x80
	lsls r0, r0, #1
	cmp r6, r0
	bne _08036992
	cmp r4, r6
	beq _08036994
_08036992:
	movs r1, #1
_08036994:
	cmp r1, #0
	beq _08036A0E
	movs r0, #0x82
	lsls r0, r0, #3
	add r0, sb
	mov r2, sl
	adds r1, r0, r2
	ldrb r0, [r1]
	cmp r0, #0
	beq _080369BA
	movs r3, #0
	strb r3, [r1]
	ldr r0, _08036C20 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x4e
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
_080369BA:
	ldr r0, _08036C10 @ =gUnknown_03001300
	ldr r2, [r0]
	ldr r0, [sp, #0x20]
	lsls r1, r0, #2
	lsls r0, r0, #5
	adds r0, r2, r0
	strh r6, [r0, #0x12]
	adds r0, r1, #3
	lsls r0, r0, #3
	adds r0, r2, r0
	strh r4, [r0, #0x12]
	adds r0, r1, #1
	lsls r0, r0, #3
	adds r0, r2, r0
	movs r3, #0
	strh r3, [r0, #0x12]
	adds r1, #2
	lsls r1, r1, #3
	adds r2, r2, r1
	strh r3, [r2, #0x12]
	ldrb r1, [r5, #1]
	movs r2, #4
	rsbs r2, r2, #0
	adds r0, r2, #0
	ands r1, r0
	movs r0, #1
	orrs r1, r0
	strb r1, [r5, #1]
	movs r0, #7
	ldr r2, [sp, #0x20]
	ands r2, r0
	lsls r2, r2, #1
	ldrb r0, [r5, #3]
	movs r3, #0xf
	rsbs r3, r3, #0
	adds r1, r3, #0
	ands r0, r1
	orrs r0, r2
	strb r0, [r5, #3]
	ldr r0, [sp, #0x20]
	adds r0, #1
	str r0, [sp, #0x20]
_08036A0E:
	movs r0, #0xf
	ldrb r1, [r5, #5]
	ands r0, r1
	movs r1, #0xe0
	orrs r0, r1
	strb r0, [r5, #5]
	ldrb r2, [r5, #3]
	movs r1, #0x3f
	adds r0, r1, #0
	ands r0, r2
	movs r2, #0x80
	orrs r0, r2
	strb r0, [r5, #3]
	ldrb r0, [r5, #1]
	ands r1, r0
	orrs r1, r2
	strb r1, [r5, #1]
	movs r2, #0xe
	ldrsh r0, [r7, r2]
	subs r0, #0x10
	add r1, sp, #0x10
	strb r0, [r1]
	movs r3, #0xa
	ldrsh r2, [r7, r3]
	subs r2, #8
	ldr r1, _08036C04 @ =0x000001FF
	adds r0, r1, #0
	ands r2, r0
	ldrh r0, [r5, #2]
	ldr r3, _08036C08 @ =0xFFFFFE00
	adds r1, r3, #0
	ands r0, r1
	orrs r0, r2
	strh r0, [r5, #2]
	ldr r1, _08036C14 @ =0x000003FF
	adds r0, r1, #0
	mov r1, r8
	ands r1, r0
	ldr r2, _08036C0C @ =0xFFFFFC00
	adds r0, r2, #0
	ldrh r3, [r5, #4]
	ands r0, r3
	orrs r0, r1
	strh r0, [r5, #4]
	ldr r0, _08036C10 @ =gUnknown_03001300
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8006AC8
_08036A70:
	movs r0, #8
	add r8, r0
	adds r7, #0x34
	movs r1, #1
	add sl, r1
	mov r2, sl
	cmp r2, #0x11
	bgt _08036A82
	b _0803694E
_08036A82:
	mov r3, sb
	ldrb r0, [r3]
	cmp r0, #0
	bne _08036A8C
	b _08036CD0
_08036A8C:
	movs r4, #0x82
	lsls r4, r4, #3
	add r4, sb
	ldrb r0, [r4]
	cmp r0, #0
	beq _08036AAA
	ldr r0, _08036C20 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x4d
	bl PlaySfx
	movs r0, #0
	strb r0, [r4]
_08036AAA:
	mov r8, sb
	ldr r0, _08036C24 @ =0x0000042C
	add r0, r8
	ldr r0, [r0]
	ldr r7, _08036C1C @ =0xF9FF0000
	adds r0, r0, r7
	lsrs r0, r0, #5
	str r0, [sp, #0x24]
	movs r0, #0
	str r0, [sp, #4]
	ldr r2, _08036BFC @ =0x040000D4
	add r0, sp, #4
	str r0, [r2]
	ldr r0, _08036C28 @ =0x00000434
	add r0, r8
	ldr r4, [r0]
	str r4, [r2, #4]
	ldr r0, _08036C2C @ =0x85000400
	str r0, [r2, #8]
	ldr r0, [r2, #8]
	movs r3, #0x86
	lsls r3, r3, #3
	add r3, r8
	movs r0, #0x87
	lsls r0, r0, #3
	add r0, r8
	ldr r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #9
	ldr r1, [r3]
	adds r1, r1, r0
	add r5, sp, #0x18
	ldr r0, _08036C30 @ =0x80000050
	mov ip, r0
	adds r3, r4, #0
	adds r3, #0x60
	movs r7, #0x80
	lsls r7, r7, #4
	mov sl, r7
	movs r6, #7
_08036AFC:
	str r1, [r2]
	str r3, [r2, #4]
	mov r0, ip
	str r0, [r2, #8]
	ldr r0, [r2, #8]
	adds r1, #0xa0
	str r1, [r2]
	mov r7, sl
	adds r0, r4, r7
	str r0, [r2, #4]
	mov r0, ip
	str r0, [r2, #8]
	ldr r0, [r2, #8]
	adds r1, #0xa0
	movs r7, #0x80
	lsls r7, r7, #1
	adds r4, r4, r7
	adds r3, r3, r7
	subs r6, #1
	cmp r6, #0
	bge _08036AFC
	ldr r0, _08036C28 @ =0x00000434
	add r0, sb
	ldr r0, [r0]
	ldr r1, _08036C24 @ =0x0000042C
	add r1, sb
	ldr r1, [r1]
	movs r2, #0x80
	lsls r2, r2, #5
	movs r3, #0x10
	bl QueueVramDmaTransfer
	mov r1, sp
	movs r0, #0
	strh r0, [r1]
	ldr r0, _08036BFC @ =0x040000D4
	str r1, [r0]
	str r5, [r0, #4]
	ldr r1, _08036C00 @ =0x81000004
	str r1, [r0, #8]
	ldr r0, [r0, #8]
	mov r0, r8
	ldr r1, [r0, #0x14]
	movs r4, #0x80
	lsls r4, r4, #0x11
	adds r0, r4, #0
	bl sub_803ADB4
	adds r6, r0, #0
	mov r2, r8
	ldr r1, [r2, #0x18]
	adds r0, r4, #0
	bl sub_803ADB4
	adds r4, r0, #0
	movs r1, #0
	adds r0, r7, #0
	cmp r6, r0
	bne _08036B76
	cmp r4, r6
	beq _08036B78
_08036B76:
	movs r1, #1
_08036B78:
	adds r7, r1, #0
	cmp r7, #0
	beq _08036BC2
	ldr r0, _08036C10 @ =gUnknown_03001300
	ldr r2, [r0]
	ldr r3, [sp, #0x20]
	lsls r1, r3, #2
	lsls r0, r3, #5
	adds r0, r2, r0
	movs r3, #0
	strh r6, [r0, #0x12]
	adds r0, r1, #3
	lsls r0, r0, #3
	adds r0, r2, r0
	strh r4, [r0, #0x12]
	adds r0, r1, #1
	lsls r0, r0, #3
	adds r0, r2, r0
	strh r3, [r0, #0x12]
	adds r1, #2
	lsls r1, r1, #3
	adds r2, r2, r1
	strh r3, [r2, #0x12]
	ldrb r0, [r5, #1]
	movs r1, #3
	orrs r0, r1
	strb r0, [r5, #1]
	movs r0, #7
	ldr r1, [sp, #0x20]
	ands r1, r0
	lsls r2, r1, #1
	ldrb r1, [r5, #3]
	movs r0, #0xf
	rsbs r0, r0, #0
	ands r0, r1
	orrs r0, r2
	strb r0, [r5, #3]
_08036BC2:
	movs r0, #0xf0
	ldrb r2, [r5, #5]
	orrs r0, r2
	strb r0, [r5, #5]
	ldrb r0, [r5, #3]
	movs r1, #0xc0
	orrs r0, r1
	strb r0, [r5, #3]
	ldrb r1, [r5, #1]
	movs r0, #0x3f
	ands r0, r1
	strb r0, [r5, #1]
	cmp r7, #0
	beq _08036C34
	mov r3, r8
	movs r1, #0xe
	ldrsh r0, [r3, r1]
	subs r0, #0x40
	strb r0, [r5]
	movs r2, #0xa
	ldrsh r1, [r3, r2]
	ldr r0, [r3, #0x14]
	lsls r0, r0, #5
	asrs r0, r0, #0x10
	subs r1, r1, r0
	subs r1, #0x40
	ldr r3, _08036C04 @ =0x000001FF
	adds r0, r3, #0
	b _08036C48
	.align 2, 0
_08036BFC: .4byte 0x040000D4
_08036C00: .4byte 0x81000004
_08036C04: .4byte 0x000001FF
_08036C08: .4byte 0xFFFFFE00
_08036C0C: .4byte 0xFFFFFC00
_08036C10: .4byte gUnknown_03001300
_08036C14: .4byte 0x000003FF
_08036C18: .4byte 0x00000424
_08036C1C: .4byte 0xF9FF0000
_08036C20: .4byte gUnknown_030012BC
_08036C24: .4byte 0x0000042C
_08036C28: .4byte 0x00000434
_08036C2C: .4byte 0x85000400
_08036C30: .4byte 0x80000050
_08036C34:
	mov r1, r8
	movs r2, #0xe
	ldrsh r0, [r1, r2]
	subs r0, #0x20
	strb r0, [r5]
	movs r3, #0xa
	ldrsh r1, [r1, r3]
	subs r1, #0x40
	ldr r2, _08036C88 @ =0x000001FF
	adds r0, r2, #0
_08036C48:
	ands r1, r0
	ldrh r2, [r5, #2]
	ldr r0, _08036C8C @ =0xFFFFFE00
	ands r0, r2
	orrs r0, r1
	strh r0, [r5, #2]
	ldr r3, _08036C90 @ =0x000003FF
	adds r0, r3, #0
	ldr r1, [sp, #0x24]
	ands r1, r0
	ldr r0, _08036C94 @ =0xFFFFFC00
	ldrh r2, [r5, #4]
	ands r0, r2
	orrs r0, r1
	strh r0, [r5, #4]
	ldr r0, _08036C98 @ =gUnknown_03001300
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8006AC8
	cmp r7, #0
	beq _08036C9C
	mov r3, r8
	movs r7, #0xa
	ldrsh r0, [r3, r7]
	ldr r1, [r3, #0x14]
	lsls r1, r1, #5
	asrs r1, r1, #0x10
	adds r1, r1, r0
	subs r1, #0x40
	b _08036CA2
	.align 2, 0
_08036C88: .4byte 0x000001FF
_08036C8C: .4byte 0xFFFFFE00
_08036C90: .4byte 0x000003FF
_08036C94: .4byte 0xFFFFFC00
_08036C98: .4byte gUnknown_03001300
_08036C9C:
	mov r3, r8
	movs r7, #0xa
	ldrsh r1, [r3, r7]
_08036CA2:
	ldr r2, _08036CE0 @ =0x000001FF
	adds r0, r2, #0
	ands r1, r0
	ldrh r2, [r5, #2]
	ldr r0, _08036CE4 @ =0xFFFFFE00
	ands r0, r2
	orrs r0, r1
	strh r0, [r5, #2]
	ldr r1, [sp, #0x24]
	adds r1, #0x40
	ldr r3, _08036CE8 @ =0x000003FF
	adds r0, r3, #0
	ands r1, r0
	ldr r0, _08036CEC @ =0xFFFFFC00
	ldrh r7, [r5, #4]
	ands r0, r7
	orrs r0, r1
	strh r0, [r5, #4]
	ldr r0, _08036CF0 @ =gUnknown_03001300
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8006AC8
_08036CD0:
	add sp, #0x28
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08036CE0: .4byte 0x000001FF
_08036CE4: .4byte 0xFFFFFE00
_08036CE8: .4byte 0x000003FF
_08036CEC: .4byte 0xFFFFFC00
_08036CF0: .4byte gUnknown_03001300

	thumb_func_start sub_8036CF4
sub_8036CF4: @ 0x08036CF4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	ldr r0, _08036D8C @ =gStaticData_0817D7A4
	mov sb, r0
	movs r0, #0x80
	lsls r0, r0, #2
	bl sub_8026EC0
	adds r4, r0, #0
	mov r1, sb
	ldr r0, [r1, #8]
	adds r1, r4, #0
	bl LoadTaggedAsset
	ldr r1, _08036D90 @ =0x040000D4
	adds r0, r4, #2
	str r0, [r1]
	ldr r0, _08036D94 @ =0x05000002
	str r0, [r1, #4]
	ldr r0, _08036D98 @ =0x80000040
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	cmp r4, #0
	beq _08036D30
	adds r0, r4, #0
	bl sub_8026EB4
_08036D30:
	mov r2, sb
	ldr r0, [r2, #0xc]
	ldr r1, _08036D9C @ =0x06008000
	bl LoadTaggedAsset
	mov r0, sb
	ldr r1, [r0, #4]
	ldr r0, [r0]
	muls r0, r1, r0
	lsls r0, r0, #1
	bl sub_8026EC0
	mov sl, r0
	mov r1, sb
	ldr r0, [r1, #0x10]
	mov r1, sl
	bl LoadTaggedAsset
	ldr r4, _08036DA0 @ =0x0600F000
	movs r5, #0
	mov r2, sb
	ldr r2, [r2, #4]
	mov ip, r2
	movs r0, #0xff
	mov r8, r0
_08036D62:
	movs r3, #0
	adds r7, r5, #1
_08036D66:
	cmp r5, ip
	bge _08036DA4
	mov r1, sb
	ldr r0, [r1]
	cmp r3, r0
	bge _08036DA4
	muls r0, r5, r0
	adds r0, r0, r3
	lsls r0, r0, #1
	add r0, sl
	mov r2, r8
	ldrh r1, [r0]
	ands r2, r1
	mov r1, r8
	ldrh r0, [r0, #2]
	ands r1, r0
	lsls r1, r1, #8
	orrs r2, r1
	b _08036DA6
	.align 2, 0
_08036D8C: .4byte gStaticData_0817D7A4
_08036D90: .4byte 0x040000D4
_08036D94: .4byte 0x05000002
_08036D98: .4byte 0x80000040
_08036D9C: .4byte 0x06008000
_08036DA0: .4byte 0x0600F000
_08036DA4:
	movs r2, #0
_08036DA6:
	strh r2, [r4]
	adds r4, #2
	adds r3, #2
	cmp r3, #0x1f
	ble _08036D66
	adds r5, r7, #0
	cmp r5, #0x1f
	ble _08036D62
	ldr r0, _08036E10 @ =0xFFFF0000
	ands r6, r0
	movs r0, #8
	orrs r6, r0
	movs r0, #0xf0
	lsls r0, r0, #5
	orrs r6, r0
	movs r0, #0x80
	orrs r6, r0
	movs r0, #1
	orrs r6, r0
	ldr r0, _08036E14 @ =0xFFFF3FFF
	ands r6, r0
	movs r0, #0x80
	lsls r0, r0, #7
	orrs r6, r0
	ldr r0, _08036E18 @ =0x0400000C
	strh r6, [r0]
	movs r0, #4
	ldr r1, _08036E1C @ =gUnknown_03001288
	ldrb r1, [r1, #1]
	orrs r0, r1
	ldr r2, _08036E1C @ =gUnknown_03001288
	strb r0, [r2, #1]
	movs r0, #8
	rsbs r0, r0, #0
	ldrb r1, [r2]
	ands r0, r1
	movs r1, #1
	orrs r0, r1
	strb r0, [r2]
	mov r2, sl
	cmp r2, #0
	beq _08036E00
	mov r0, sl
	bl sub_8026EB4
_08036E00:
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08036E10: .4byte 0xFFFF0000
_08036E14: .4byte 0xFFFF3FFF
_08036E18: .4byte 0x0400000C
_08036E1C: .4byte gUnknown_03001288

	thumb_func_start sub_8036E20
sub_8036E20: @ 0x08036E20
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r0, #0
	movs r0, #0x80
	lsls r0, r0, #1
	str r0, [sp]
	adds r0, r4, #0
	movs r2, #0
	movs r3, #0
	bl InitActorPart
	ldr r0, _08036EB4 @ =gStaticData_087E55C4
	str r0, [r4, #0x50]
	ldr r2, [r4, #8]
	asrs r2, r2, #8
	ldr r1, [r4, #0xc]
	ldr r3, [r4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r3
	movs r1, #2
	ldrsh r0, [r0, r1]
	adds r0, r0, r2
	ldr r1, [r4, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldrb r3, [r0]
	ldrb r1, [r0, #1]
	adds r2, r3, #0
	muls r2, r1, r2
	adds r0, r2, #0
	lsls r0, r0, #5
	bl AllocVramTileBlock
	ldr r5, _08036EB8 @ =gUnknown_0300160C
	str r0, [r5]
	ldr r2, [r4, #8]
	asrs r2, r2, #8
	ldr r1, [r4, #0xc]
	ldr r3, [r4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r3
	movs r3, #2
	ldrsh r0, [r0, r3]
	adds r0, r0, r2
	ldr r1, [r4, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldrb r2, [r0]
	ldrb r3, [r0, #1]
	adds r1, r2, #0
	muls r1, r3, r1
	adds r0, r1, #0
	lsls r0, r0, #5
	bl AllocVramTileBlock
	str r0, [r5, #4]
	ldr r1, _08036EBC @ =gUnknown_03001604
	movs r0, #1
	str r0, [r1]
	ldr r1, _08036EC0 @ =gUnknown_03001608
	movs r0, #0
	str r0, [r1]
	adds r0, r4, #0
	add sp, #4
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08036EB4: .4byte gStaticData_087E55C4
_08036EB8: .4byte gUnknown_0300160C
_08036EBC: .4byte gUnknown_03001604
_08036EC0: .4byte gUnknown_03001608

	thumb_func_start sub_8036EC4
sub_8036EC4: @ 0x08036EC4
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x44]
	adds r1, r0, #1
	str r1, [r4, #0x44]
	ldr r0, [r4, #0x28]
	cmp r0, #1
	beq _08036F00
	cmp r0, #1
	blo _08036EE2
	cmp r0, #2
	beq _08036F38
	cmp r0, #3
	beq _08036F60
	b _08036F76
_08036EE2:
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _08036F76
	movs r0, #1
	str r0, [r4, #0x28]
	movs r2, #0
	str r2, [r4, #0x44]
	str r0, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	b _08036F76
_08036F00:
	ldr r0, [r4, #8]
	asrs r0, r0, #8
	cmp r0, #0x12
	bne _08036F76
	movs r0, #2
	movs r1, #7
	str r0, [r4, #0x28]
	movs r2, #0
	str r2, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	adds r0, #0x54
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	ldr r0, _08036F34 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x4f
	bl PlaySfx
	b _08036F76
	.align 2, 0
_08036F34: .4byte gUnknown_030012BC
_08036F38:
	ldr r0, [r4, #8]
	asrs r0, r0, #8
	cmp r0, #7
	bne _08036F76
	movs r1, #0
	strh r1, [r4, #0x10]
	movs r0, #3
	str r0, [r4, #0x28]
	str r1, [r4, #0x44]
	ldr r0, _08036F5C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x1b
	bl PlaySfx
	b _08036F76
	.align 2, 0
_08036F5C: .4byte gUnknown_030012BC
_08036F60:
	ldr r0, [r4, #0x24]
	subs r0, #0xe
	str r0, [r4, #0x24]
	ldr r0, [r4, #0x20]
	ldr r2, _08036FB8 @ =0xFFFFFF00
	adds r0, r0, r2
	str r0, [r4, #0x20]
	cmp r1, #0xf
	ble _08036F76
	movs r0, #4
	str r0, [r4, #0x28]
_08036F76:
	movs r3, #0x10
	ldrsh r1, [r4, r3]
	ldr r0, [r4, #8]
	adds r0, r0, r1
	str r0, [r4, #8]
	movs r0, #0
	strb r0, [r4, #0x12]
	adds r0, r4, #0
	bl GetAnimFrameBaseOffset
	ldr r2, [r4, #0xc]
	ldr r3, [r4]
	lsls r1, r2, #1
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r1, r1, r3
	movs r3, #4
	ldrsh r2, [r1, r3]
	cmp r0, r2
	blt _08036FB0
	movs r3, #6
	ldrsh r0, [r1, r3]
	subs r0, r2, r0
	lsls r0, r0, #8
	ldr r1, [r4, #8]
	subs r1, r1, r0
	str r1, [r4, #8]
	movs r0, #1
	strb r0, [r4, #0x12]
_08036FB0:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08036FB8: .4byte 0xFFFFFF00

	thumb_func_start sub_8036FBC
sub_8036FBC: @ 0x08036FBC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x14
	adds r7, r0, #0
	ldr r0, [r7, #0x28]
	cmp r0, #4
	bne _08036FD2
	b _080370E8
_08036FD2:
	ldr r2, [r7, #8]
	asrs r2, r2, #8
	ldr r1, [r7, #0xc]
	ldr r3, [r7]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r3
	str r0, [sp, #8]
	movs r1, #2
	ldrsh r0, [r0, r1]
	adds r0, r0, r2
	ldr r1, [r7, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	mov sl, r0
	ldrb r2, [r0]
	str r2, [sp, #0xc]
	lsls r2, r2, #2
	mov sb, r2
	ldrb r3, [r0, #1]
	str r3, [sp, #0x10]
	lsls r3, r3, #2
	mov r8, r3
	ldr r4, [r7, #0x24]
	lsls r0, r4, #8
	ldr r1, [r7, #0x30]
	ldr r1, [r1, #0x10]
	bl sub_803ADB4
	str r0, [sp]
	movs r0, #0x80
	lsls r0, r0, #0xd
	adds r1, r4, #0
	bl sub_803ADB4
	ldr r1, [r7, #0x20]
	muls r1, r0, r1
	asrs r1, r1, #0xc
	movs r2, #0xa0
	lsls r2, r2, #7
	adds r1, r1, r2
	asrs r5, r1, #8
	ldr r1, [r7, #0x1c]
	muls r0, r1, r0
	asrs r0, r0, #0xc
	movs r3, #0xf0
	lsls r3, r3, #7
	adds r0, r0, r3
	asrs r6, r0, #8
	movs r0, #0x80
	lsls r0, r0, #1
	str r0, [sp, #4]
	ldr r1, [sp]
	cmp r1, #0xff
	bgt _0803705A
	movs r0, #0x80
	lsls r0, r0, #2
	ldr r2, [sp, #4]
	orrs r2, r0
	str r2, [sp, #4]
	ldr r3, [sp, #0xc]
	lsls r3, r3, #3
	mov sb, r3
	ldr r0, [sp, #0x10]
	lsls r0, r0, #3
	mov r8, r0
_0803705A:
	mov r1, sb
	subs r6, r6, r1
	mov r2, r8
	subs r5, r5, r2
	cmp r5, #0x9f
	bgt _080370E8
	lsls r0, r2, #1
	adds r0, r5, r0
	cmp r0, #0
	blt _080370E8
	cmp r6, #0xef
	bgt _080370E8
	lsls r0, r1, #1
	adds r0, r6, r0
	cmp r0, #0
	blt _080370E8
	ldr r3, [sp, #8]
	ldrh r3, [r3, #8]
	lsls r4, r3, #0x10
	mov r0, sl
	bl GetSpriteShapeSizeBits
	movs r1, #0xff
	ands r5, r1
	ldr r1, _080370F8 @ =0x000001FF
	ands r6, r1
	lsls r1, r6, #0x10
	orrs r5, r1
	orrs r5, r4
	orrs r5, r0
	ldr r0, [sp, #4]
	orrs r0, r5
	str r0, [sp, #4]
	ldr r4, _080370FC @ =gUnknown_03001608
	ldr r0, [r4]
	cmp sl, r0
	beq _080370C4
	ldr r2, _08037100 @ =gUnknown_03001604
	ldr r0, [r2]
	movs r1, #1
	eors r0, r1
	str r0, [r2]
	ldr r2, _08037104 @ =gUnknown_03000874
	ldr r1, _08037108 @ =gUnknown_0300160C
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r2, [r2]
	mov r1, sl
	bl sub_803AD80
	mov r1, sl
	str r1, [r4]
_080370C4:
	ldr r1, _08037108 @ =gUnknown_0300160C
	ldr r0, _08037100 @ =gUnknown_03001604
	ldr r0, [r0]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r2, _0803710C @ =0xF9FF0000
	adds r0, r0, r2
	lsrs r0, r0, #5
	ldr r1, [r7, #0x18]
	lsls r1, r1, #0xc
	orrs r1, r0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	ldr r0, [sp, #4]
	ldr r2, [sp]
	bl QueueSpriteFrameOam
_080370E8:
	add sp, #0x14
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080370F8: .4byte 0x000001FF
_080370FC: .4byte gUnknown_03001608
_08037100: .4byte gUnknown_03001604
_08037104: .4byte gUnknown_03000874
_08037108: .4byte gUnknown_0300160C
_0803710C: .4byte 0xF9FF0000

