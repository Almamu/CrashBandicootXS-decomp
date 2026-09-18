.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8024590
sub_8024590: @ 0x08024590
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	ldr r6, _080245E8 @ =gUnknown_030012BC
	ldr r0, [r6]
	ldr r2, [r5]
	lsls r4, r1, #2
	adds r2, r4, r2
	ldr r1, [r2]
	ldr r1, [r1, #0x14]
	bl sub_8001B54
	ldr r0, [r6]
	bl sub_8001AB8
	ldr r1, [r5]
	adds r1, r4, r1
	ldr r2, [r1]
	ldr r1, [r2, #0x14]
	cmp r0, r1
	bne _080245EC
	ldr r1, [r2, #0x18]
	cmp r1, #0x63
	beq _080245C8
	ldr r0, [r6]
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
_080245C8:
	ldr r0, [r5]
	adds r0, r4, r0
	ldr r0, [r0]
	ldr r0, [r0, #8]
	movs r2, #0x80
	rsbs r2, r2, #0
	adds r1, r2, #0
	orrs r0, r1
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	movs r1, #1
	movs r2, #0
	bl sub_800132C
	b _08024636
	.align 2, 0
_080245E8: .4byte gUnknown_030012BC
_080245EC:
	ldr r0, [r2, #8]
	movs r2, #0x80
	rsbs r2, r2, #0
	adds r1, r2, #0
	orrs r0, r1
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	movs r1, #1
	movs r2, #0
	bl sub_800132C
	ldr r0, [r5]
	adds r0, r4, r0
	ldr r0, [r0]
	ldr r0, [r0, #0x18]
	cmp r0, #0x63
	beq _08024636
	adds r7, r6, #0
	adds r6, r4, #0
_08024612:
	ldr r0, [r7]
	bl sub_8001AB8
	ldr r2, [r5]
	adds r1, r4, r2
	ldr r1, [r1]
	ldr r1, [r1, #0x14]
	cmp r0, r1
	bne _08024612
	ldr r0, _0802463C @ =gUnknown_030012BC
	ldr r0, [r0]
	adds r1, r6, r2
	ldr r1, [r1]
	ldr r1, [r1, #0x18]
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
_08024636:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802463C: .4byte gUnknown_030012BC

	thumb_func_start sub_8024640
sub_8024640: @ 0x08024640
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	movs r6, #0
	b _080246C8
_08024648:
	adds r0, r4, #0
	adds r1, r6, #0
	bl sub_8024708
	adds r0, r4, #0
	adds r1, r6, #0
	bl sub_8024590
	ldr r0, [r4]
	lsls r5, r6, #2
	adds r0, r5, r0
	ldr r1, [r0]
	ldr r0, [r1, #4]
	ldrb r1, [r1, #0x10]
	movs r2, #8
	bl sub_80010E0
	lsls r0, r0, #0x18
	lsrs r7, r0, #0x18
	ldr r0, [r4]
	adds r0, r5, r0
	ldr r0, [r0]
	ldrb r0, [r0, #0x11]
	cmp r0, #0
	beq _08024684
	ldr r0, _080246D4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8001AC4
_08024684:
	ldr r0, [r4]
	adds r0, r5, r0
	ldr r0, [r0]
	ldr r1, [r0, #0xc]
	movs r0, #1
	rsbs r0, r0, #0
	cmp r1, r0
	beq _080246A0
	lsls r0, r1, #0x18
	lsrs r0, r0, #0x18
	movs r1, #1
	movs r2, #0
	bl sub_800132C
_080246A0:
	ldr r0, [r4]
	adds r0, r5, r0
	ldr r1, [r0]
	ldrb r0, [r1, #0x12]
	cmp r0, #0
	beq _080246BA
	ldr r1, [r1, #0x18]
	cmp r1, #0x63
	beq _080246BA
	ldr r0, _080246D4 @ =gUnknown_030012BC
	ldr r0, [r0]
	bl sub_80019A8
_080246BA:
	adds r0, r4, #0
	adds r1, r6, #0
	adds r2, r7, #0
	bl sub_80246D8
	adds r6, r0, #0
	adds r6, #1
_080246C8:
	ldr r0, [r4, #4]
	cmp r6, r0
	blt _08024648
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080246D4: .4byte gUnknown_030012BC

	thumb_func_start sub_80246D8
sub_80246D8: @ 0x080246D8
	push {r4, lr}
	lsls r2, r2, #0x18
	cmp r2, #0
	bne _08024700
	adds r2, r1, #1
	ldr r4, [r0, #4]
	cmp r2, r4
	bge _08024700
	ldr r3, [r0]
	b _080246F4
_080246EC:
	adds r1, r2, #0
	adds r2, r1, #1
	cmp r2, r4
	bge _08024700
_080246F4:
	lsls r0, r1, #2
	adds r0, r0, r3
	ldr r0, [r0, #4]
	ldrb r0, [r0, #0x10]
	cmp r0, #1
	beq _080246EC
_08024700:
	adds r0, r1, #0
	pop {r4}
	pop {r1}
	bx r1

	thumb_func_start sub_8024708
sub_8024708: @ 0x08024708
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	ldr r0, [r5]
	lsls r1, r1, #2
	adds r1, r1, r0
	ldr r0, [r1]
	ldr r6, [r0]
	ldr r0, [r5, #0xc]
	movs r1, #1
	eors r0, r1
	str r0, [r5, #0xc]
	cmp r0, #0
	bne _08024732
	movs r1, #0x80
	lsls r1, r1, #2
	adds r0, r6, r1
	movs r1, #0xc0
	lsls r1, r1, #0x13
	bl LoadTaggedAsset
	b _0802473E
_08024732:
	movs r2, #0x80
	lsls r2, r2, #2
	adds r0, r6, r2
	ldr r1, _08024774 @ =0x0600A000
	bl LoadTaggedAsset
_0802473E:
	ldr r4, _08024778 @ =gUnknown_03001314
	movs r1, #1
	ldrb r5, [r5, #0xc]
	ands r1, r5
	lsls r1, r1, #4
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r2, [r4]
	ands r0, r2
	orrs r0, r1
	strb r0, [r4]
	bl sub_80006A8
	ldr r1, _0802477C @ =0x040000D4
	str r6, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	str r0, [r1, #4]
	ldr r0, _08024780 @ =0x80000100
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	subs r1, #0xd4
	ldrh r0, [r4]
	strh r0, [r1]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08024774: .4byte 0x0600A000
_08024778: .4byte gUnknown_03001314
_0802477C: .4byte 0x040000D4
_08024780: .4byte 0x80000100

