.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start InitObjTileFreeList
InitObjTileFreeList: @ 0x08028BA0
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r7, r0, #0
	ldr r6, _08028C2C @ =gUnknown_03001320
	movs r0, #0x80
	lsls r0, r0, #4
	movs r4, #0x80
	lsls r4, r4, #0x17
	adds r1, r4, #0
	bl mem_alloc
	str r0, [r6]
	ldr r5, _08028C30 @ =gUnknown_03001340
	movs r0, #0x80
	lsls r0, r0, #3
	adds r1, r4, #0
	bl mem_alloc
	str r0, [r5]
	ldr r0, _08028C34 @ =0x06018000
	subs r4, r0, r7
	mov r1, sp
	movs r0, #0
	strh r0, [r1]
	ldr r2, _08028C38 @ =0x040000D4
	str r1, [r2]
	str r7, [r2, #4]
	lsrs r0, r4, #0x1f
	adds r0, r4, r0
	asrs r0, r0, #1
	movs r1, #0x81
	lsls r1, r1, #0x18
	orrs r0, r1
	str r0, [r2, #8]
	ldr r0, [r2, #8]
	ldr r1, _08028C3C @ =gUnknown_0300133C
	ldr r0, [r6]
	str r0, [r1]
	adds r3, r0, #0
	movs r1, #0x7d
_08028BF0:
	adds r0, r3, #0
	adds r0, #0x10
	str r0, [r3, #8]
	adds r3, r0, #0
	subs r1, #1
	cmp r1, #0
	bge _08028BF0
	movs r2, #0
	str r2, [r3, #8]
	adds r3, #0x10
	ldr r0, _08028C40 @ =gUnknown_03001328
	str r3, [r0, #8]
	str r3, [r0, #0xc]
	movs r1, #1
	strh r1, [r0, #6]
	strh r2, [r0, #4]
	ldr r1, _08028C34 @ =0x06018000
	str r1, [r0]
	strh r2, [r3, #6]
	str r0, [r3, #0xc]
	str r0, [r3, #8]
	strh r4, [r3, #4]
	str r7, [r3]
	ldr r0, _08028C44 @ =gUnknown_03001338
	str r3, [r0]
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08028C2C: .4byte gUnknown_03001320
_08028C30: .4byte gUnknown_03001340
_08028C34: .4byte 0x06018000
_08028C38: .4byte 0x040000D4
_08028C3C: .4byte gUnknown_0300133C
_08028C40: .4byte gUnknown_03001328
_08028C44: .4byte gUnknown_03001338

	thumb_func_start sub_8028C48
sub_8028C48: @ 0x08028C48
	push {r4, lr}
	cmp r0, #0
	beq _08028CBA
	ldr r2, _08028CC0 @ =0xF9FF0000
	adds r1, r0, r2
	lsrs r1, r1, #5
	ldr r2, _08028CC4 @ =gUnknown_03001320
	ldr r0, _08028CC8 @ =gUnknown_03001340
	ldr r0, [r0]
	adds r0, r0, r1
	ldrb r0, [r0]
	lsls r1, r0, #4
	ldr r0, [r2]
	adds r2, r0, r1
	movs r0, #0
	strh r0, [r2, #6]
	ldr r3, [r2, #0xc]
	ldrh r0, [r3, #6]
	cmp r0, #0
	bne _08028C92
	ldrh r4, [r3, #4]
	ldrh r1, [r2, #4]
	adds r0, r4, r1
	strh r0, [r3, #4]
	ldr r0, [r2, #8]
	str r0, [r3, #8]
	str r3, [r0, #0xc]
	ldr r1, _08028CCC @ =gUnknown_03001338
	ldr r0, [r1]
	cmp r2, r0
	bne _08028C88
	str r3, [r1]
_08028C88:
	ldr r1, _08028CD0 @ =gUnknown_0300133C
	ldr r0, [r1]
	str r0, [r2, #8]
	str r2, [r1]
	adds r2, r3, #0
_08028C92:
	ldr r3, [r2, #8]
	ldrh r0, [r3, #6]
	cmp r0, #0
	bne _08028CBA
	ldrh r4, [r2, #4]
	ldrh r1, [r3, #4]
	adds r0, r4, r1
	strh r0, [r2, #4]
	ldr r0, [r3, #8]
	str r0, [r2, #8]
	str r2, [r0, #0xc]
	ldr r1, _08028CCC @ =gUnknown_03001338
	ldr r0, [r1]
	cmp r3, r0
	bne _08028CB2
	str r2, [r1]
_08028CB2:
	ldr r1, _08028CD0 @ =gUnknown_0300133C
	ldr r0, [r1]
	str r0, [r3, #8]
	str r3, [r1]
_08028CBA:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08028CC0: .4byte 0xF9FF0000
_08028CC4: .4byte gUnknown_03001320
_08028CC8: .4byte gUnknown_03001340
_08028CCC: .4byte gUnknown_03001338
_08028CD0: .4byte gUnknown_0300133C

	thumb_func_start sub_8028CD4
sub_8028CD4: @ 0x08028CD4
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	ldr r0, _08028D14 @ =gUnknown_03001338
	ldr r1, [r0]
	ldr r2, [r1, #0xc]
	adds r3, r1, #0
	ldrh r1, [r3, #6]
	adds r6, r0, #0
	cmp r1, #0
	bne _08028CEE
	ldrh r0, [r3, #4]
	cmp r0, r4
	bge _08028D00
_08028CEE:
	cmp r3, r2
	beq _08028D10
	ldr r3, [r3, #8]
	ldrh r0, [r3, #6]
	cmp r0, #0
	bne _08028CEE
	ldrh r1, [r3, #4]
	cmp r1, r4
	blt _08028CEE
_08028D00:
	ldrh r0, [r3, #4]
	subs r5, r0, r4
	cmp r5, #0
	beq _08028D38
	ldr r1, _08028D18 @ =gUnknown_0300133C
	ldr r2, [r1]
	cmp r2, #0
	bne _08028D1C
_08028D10:
	movs r0, #0
	b _08028D5A
	.align 2, 0
_08028D14: .4byte gUnknown_03001338
_08028D18: .4byte gUnknown_0300133C
_08028D1C:
	ldr r0, [r2, #8]
	str r0, [r1]
	movs r1, #0
	strh r5, [r2, #4]
	ldr r0, [r3]
	adds r0, r0, r4
	str r0, [r2]
	strh r1, [r2, #6]
	str r3, [r2, #0xc]
	ldr r0, [r3, #8]
	str r0, [r2, #8]
	str r2, [r0, #0xc]
	str r2, [r3, #8]
	strh r4, [r3, #4]
_08028D38:
	movs r0, #2
	strh r0, [r3, #6]
	ldr r0, [r3, #8]
	str r0, [r6]
	ldr r0, [r3]
	ldr r1, _08028D60 @ =0xF9FF0000
	adds r0, r0, r1
	lsrs r0, r0, #5
	ldr r1, _08028D64 @ =gUnknown_03001340
	ldr r1, [r1]
	adds r1, r1, r0
	ldr r0, _08028D68 @ =gUnknown_03001320
	ldr r0, [r0]
	subs r0, r3, r0
	asrs r0, r0, #4
	strb r0, [r1]
	ldr r0, [r3]
_08028D5A:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08028D60: .4byte 0xF9FF0000
_08028D64: .4byte gUnknown_03001340
_08028D68: .4byte gUnknown_03001320
	thumb_func_start sub_8028D6C
sub_8028D6C: @ 0x08028D6C
	ldr r0, _08028D8C
	ldr	r0, [r0, #0]
	ldr r1, _08028D90
	cmp	r0, #0
	beq _08028D7C
_08028D76:
	ldr	r0, [r0, #8]
	cmp	r0, #0
	bne _08028D76
_08028D7C:
	ldr	r0, [r1, #8]
	cmp	r0, r1
	beq _08028D88
_08028D82:
	ldr	r0, [r0, #8]
	cmp	r0, r1
	bne _08028D82
_08028D88:
	bx	lr
	movs	r0, r0
_08028D8C: .4byte gUnknown_0300133C
_08028D90: .4byte gUnknown_03001328
	movs	r2, #0
	ldr r0, _08028DB4
	ldr	r1, [r0, #8]
	cmp	r1, r0
	beq _08028DB0
	adds	r3, r0, #0
_08028DA0:
	ldrh	r0, [r1, #6]
	cmp	r0, #0
	bne _08028DAA
	ldrh	r0, [r1, #4]
	adds	r2, r0, r2
_08028DAA:
	ldr	r1, [r1, #8]
	cmp	r1, r3
	bne _08028DA0
_08028DB0:
	adds	r0, r2, #0
	bx	lr
_08028DB4: .4byte gUnknown_03001328

	thumb_func_start sub_8028DB8
sub_8028DB8: @ 0x08028DB8
	push {lr}
	ldr r0, _08028DD0 @ =gUnknown_03001340
	ldr r0, [r0]
	bl mem_free
	ldr r0, _08028DD4 @ =gUnknown_03001320
	ldr r0, [r0]
	bl mem_free
	pop {r0}
	bx r0
	.align 2, 0
_08028DD0: .4byte gUnknown_03001340
_08028DD4: .4byte gUnknown_03001320

	thumb_func_start sub_8028DD8
sub_8028DD8: @ 0x08028DD8
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	lsls r1, r1, #0x10
	lsrs r7, r1, #0x10
	movs r0, #0x80
	lsls r0, r0, #1
	ands r0, r4
	cmp r0, #0
	beq _08028E54
	movs r0, #0x80
	lsls r0, r0, #0x15
	ands r0, r4
	cmp r0, #0
	beq _08028DF8
	rsbs r0, r2, #0
	b _08028DFA
_08028DF8:
	adds r0, r2, #0
_08028DFA:
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	ldr r1, _08028E14 @ =0xFFFF0000
	ands r3, r1
	orrs r3, r0
	movs r0, #0x80
	lsls r0, r0, #0x16
	ands r0, r4
	cmp r0, #0
	beq _08028E18
	rsbs r1, r2, #0
	b _08028E1A
	.align 2, 0
_08028E14: .4byte 0xFFFF0000
_08028E18:
	adds r1, r2, #0
_08028E1A:
	lsls r1, r1, #0x10
	ldr r0, _08028E70 @ =0x0000FFFF
	ands r3, r0
	orrs r3, r1
	ldr r0, _08028E74 @ =0xCFFFFFFF
	ands r4, r0
	ldr r0, _08028E78 @ =gUnknown_03001350
	ldr r2, [r0]
	adds r6, r0, #0
	ldr r5, _08028E7C @ =gUnknown_03001348
	cmp r2, #0
	beq _08028E40
	ldr r0, [r5]
	lsls r1, r2, #2
	adds r1, r1, r0
	subs r1, #4
	ldr r0, [r1]
	cmp r3, r0
	beq _08028E4C
_08028E40:
	ldr r0, [r5]
	lsls r1, r2, #2
	adds r1, r1, r0
	str r3, [r1]
	adds r0, r2, #1
	str r0, [r6]
_08028E4C:
	ldr r0, [r6]
	subs r0, #1
	lsls r0, r0, #0x19
	orrs r4, r0
_08028E54:
	ldr r3, _08028E80 @ =gUnknown_0300134C
	ldr r1, [r3]
	ldr r0, _08028E84 @ =gUnknown_03001344
	ldr r2, [r0]
	lsls r0, r1, #3
	adds r0, r0, r2
	str r4, [r0]
	strh r7, [r0, #4]
	adds r1, #1
	str r1, [r3]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08028E70: .4byte 0x0000FFFF
_08028E74: .4byte 0xCFFFFFFF
_08028E78: .4byte gUnknown_03001350
_08028E7C: .4byte gUnknown_03001348
_08028E80: .4byte gUnknown_0300134C
_08028E84: .4byte gUnknown_03001344

	thumb_func_start sub_8028E88
sub_8028E88: @ 0x08028E88
	push {lr}
	ldr r0, _08028EA0 @ =gUnknown_03001348
	ldr r0, [r0]
	bl mem_free
	ldr r0, _08028EA4 @ =gUnknown_03001344
	ldr r0, [r0]
	bl mem_free
	pop {r0}
	bx r0
	.align 2, 0
_08028EA0: .4byte gUnknown_03001348
_08028EA4: .4byte gUnknown_03001344

	thumb_func_start sub_8028EA8
sub_8028EA8: @ 0x08028EA8
	push {r4, r5, lr}
	ldr r4, _08028EDC @ =gUnknown_03001300
	ldr r0, [r4]
	ldr r1, _08028EE0 @ =gUnknown_03001344
	ldr r1, [r1]
	ldr r5, _08028EE4 @ =gUnknown_0300134C
	ldr r2, [r5]
	bl sub_8006A14
	ldr r0, [r4]
	bl sub_8006A48
	ldr r0, [r4]
	ldr r1, _08028EE8 @ =gUnknown_03001348
	ldr r1, [r1]
	ldr r4, _08028EEC @ =gUnknown_03001350
	ldr r2, [r4]
	bl sub_80069E8
	movs r0, #0
	str r0, [r5]
	str r0, [r4]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08028EDC: .4byte gUnknown_03001300
_08028EE0: .4byte gUnknown_03001344
_08028EE4: .4byte gUnknown_0300134C
_08028EE8: .4byte gUnknown_03001348
_08028EEC: .4byte gUnknown_03001350

	thumb_func_start sub_8028EF0
sub_8028EF0: @ 0x08028EF0
	push {r4, r5, lr}
	sub sp, #4
	ldr r4, _08028F40 @ =gUnknown_03001344
	movs r0, #0x80
	lsls r0, r0, #3
	movs r5, #0x80
	lsls r5, r5, #0x17
	adds r1, r5, #0
	bl mem_alloc
	str r0, [r4]
	ldr r4, _08028F44 @ =gUnknown_03001348
	movs r0, #0x80
	adds r1, r5, #0
	bl mem_alloc
	str r0, [r4]
	ldr r0, _08028F48 @ =gUnknown_0300134C
	movs r1, #0
	str r1, [r0]
	ldr r0, _08028F4C @ =gUnknown_03001350
	str r1, [r0]
	mov r1, sp
	movs r2, #0x80
	lsls r2, r2, #2
	adds r0, r2, #0
	strh r0, [r1]
	ldr r1, _08028F50 @ =0x040000D4
	mov r0, sp
	str r0, [r1]
	movs r0, #0xe0
	lsls r0, r0, #0x13
	str r0, [r1, #4]
	ldr r0, _08028F54 @ =0x81000200
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08028F40: .4byte gUnknown_03001344
_08028F44: .4byte gUnknown_03001348
_08028F48: .4byte gUnknown_0300134C
_08028F4C: .4byte gUnknown_03001350
_08028F50: .4byte 0x040000D4
_08028F54: .4byte 0x81000200

	thumb_func_start LoadSpriteFrameTiles
LoadSpriteFrameTiles: @ 0x08028F58
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r0, _08028F74 @ =gUnknown_03000870
	ldr r1, [r0]
	adds r0, r4, #0
	bl sub_803AD7C
	adds r1, r0, #0
	movs r0, #1
	rsbs r0, r0, #0
	cmp r1, r0
	beq _08028F78
	adds r0, r1, #0
	b _08028FE8
	.align 2, 0
_08028F74: .4byte gUnknown_03000870
_08028F78:
	ldr r2, _08028FA0 @ =gUnknown_03001374
	ldr r5, [r2]
	ldr r0, [r5]
	str r0, [r2]
	str r4, [r5, #8]
	ldr r1, _08028FA4 @ =gUnknown_03001354
	str r1, [r5, #4]
	ldr r0, [r1]
	str r0, [r5]
	ldr r0, [r1]
	str r5, [r0, #4]
	str r5, [r1]
	ldrb r1, [r4]
	ldrb r3, [r4, #1]
	adds r0, r1, #0
	muls r0, r3, r0
	lsls r6, r0, #5
	adds r7, r2, #0
	b _08028FC4
	.align 2, 0
_08028FA0: .4byte gUnknown_03001374
_08028FA4: .4byte gUnknown_03001354
_08028FA8:
	ldr r0, _08028FF0 @ =gUnknown_03001364
	ldr r4, [r0, #4]
	ldr r0, [r4, #0xc]
	bl sub_8028C48
	ldr r1, [r4, #4]
	ldr r0, [r4]
	str r0, [r1]
	ldr r1, [r4]
	ldr r0, [r4, #4]
	str r0, [r1, #4]
	ldr r0, [r7]
	str r0, [r4]
	str r4, [r7]
_08028FC4:
	adds r0, r6, #0
	bl sub_8028CD4
	adds r1, r0, #0
	str r1, [r5, #0xc]
	cmp r1, #0
	beq _08028FA8
	ldr r0, [r5, #8]
	adds r0, #4
	lsls r2, r6, #0x10
	lsrs r2, r2, #0x10
	movs r3, #0x10
	bl QueueVramDmaTransfer
	ldr r0, [r5, #0xc]
	ldr r1, _08028FF4 @ =0xF9FF0000
	adds r0, r0, r1
	lsrs r0, r0, #5
_08028FE8:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08028FF0: .4byte gUnknown_03001364
_08028FF4: .4byte 0xF9FF0000

	thumb_func_start SetupSpriteFrameOam
SetupSpriteFrameOam: @ 0x08028FF8
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	adds r7, r3, #0
	lsls r2, r2, #0x10
	lsrs r4, r2, #0x10
	ldrb r1, [r5]
	ldrb r0, [r5, #1]
	cmp r1, r0
	bne _0802902C
	movs r2, #0
	cmp r1, #8
	bne _08029018
	movs r2, #0xc0
	lsls r2, r2, #0x18
	b _0802905E
_08029018:
	cmp r1, #4
	bne _08029022
	movs r2, #0x80
	lsls r2, r2, #0x18
	b _0802905E
_08029022:
	cmp r1, #2
	bne _0802905E
	movs r2, #0x80
	lsls r2, r2, #0x17
	b _0802905E
_0802902C:
	movs r2, #0x80
	lsls r2, r2, #7
	cmp r1, r0
	bge _08029038
	movs r2, #0x80
	lsls r2, r2, #8
_08029038:
	subs r0, r1, r0
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	cmp r0, #4
	bne _0802904A
	movs r0, #0xc0
	lsls r0, r0, #0x18
	b _0802905C
_0802904A:
	cmp r0, #2
	bne _08029054
	movs r0, #0x80
	lsls r0, r0, #0x18
	b _0802905C
_08029054:
	cmp r0, #3
	bne _0802905E
	movs r0, #0x80
	lsls r0, r0, #0x17
_0802905C:
	orrs r2, r0
_0802905E:
	orrs r6, r2
	adds r0, r5, #0
	bl LoadSpriteFrameTiles
	orrs r4, r0
	lsls r0, r4, #0x10
	lsrs r4, r0, #0x10
	adds r0, r6, #0
	adds r1, r4, #0
	adds r2, r7, #0
	bl sub_8028DD8
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_802907C
sub_802907C: @ 0x0802907C
	push {lr}
	ldr r0, _0802908C @ =gUnknown_03001378
	ldr r0, [r0]
	bl mem_free
	pop {r0}
	bx r0
	.align 2, 0
_0802908C: .4byte gUnknown_03001378

	thumb_func_start sub_8029090
sub_8029090: @ 0x08029090
	ldr r3, _080290B4 @ =gUnknown_03001354
	ldr r0, [r3]
	cmp r0, r3
	beq _080290B0
	ldr r1, _080290B8 @ =gUnknown_03001364
	str r1, [r0, #4]
	ldr r2, [r3, #4]
	ldr r0, [r1]
	str r0, [r2]
	ldr r2, [r1]
	ldr r0, [r3, #4]
	str r0, [r2, #4]
	ldr r0, [r3]
	str r0, [r1]
	str r3, [r3, #4]
	str r3, [r3]
_080290B0:
	bx lr
	.align 2, 0
_080290B4: .4byte gUnknown_03001354
_080290B8: .4byte gUnknown_03001364

	thumb_func_start sub_80290BC
sub_80290BC: @ 0x080290BC
	push {r4, lr}
	ldr r4, _080290F8 @ =gUnknown_03001378
	movs r0, #0x80
	lsls r0, r0, #4
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	str r0, [r4]
	ldr r1, _080290FC @ =gUnknown_03001354
	str r1, [r1, #4]
	str r1, [r1]
	ldr r1, _08029100 @ =gUnknown_03001364
	str r1, [r1, #4]
	str r1, [r1]
	ldr r1, _08029104 @ =gUnknown_03001374
	str r0, [r1]
	movs r2, #0x7e
_080290E0:
	adds r1, r0, #0
	adds r1, #0x10
	str r1, [r0]
	adds r0, r1, #0
	subs r2, #1
	cmp r2, #0
	bge _080290E0
	movs r0, #0
	str r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080290F8: .4byte gUnknown_03001378
_080290FC: .4byte gUnknown_03001354
_08029100: .4byte gUnknown_03001364
_08029104: .4byte gUnknown_03001374

	thumb_func_start sub_8029108
sub_8029108: @ 0x08029108
	ldrb r1, [r0]
	ldrb r0, [r0, #1]
	cmp r1, r0
	bne _08029130
	movs r2, #0
	cmp r1, #8
	bne _0802911C
	movs r2, #0xc0
	lsls r2, r2, #0x18
	b _08029162
_0802911C:
	cmp r1, #4
	bne _08029126
	movs r2, #0x80
	lsls r2, r2, #0x18
	b _08029162
_08029126:
	cmp r1, #2
	bne _08029162
	movs r2, #0x80
	lsls r2, r2, #0x17
	b _08029162
_08029130:
	movs r2, #0x80
	lsls r2, r2, #7
	cmp r1, r0
	bge _0802913C
	movs r2, #0x80
	lsls r2, r2, #8
_0802913C:
	subs r0, r1, r0
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	cmp r0, #4
	bne _0802914E
	movs r0, #0xc0
	lsls r0, r0, #0x18
	b _08029160
_0802914E:
	cmp r0, #2
	bne _08029158
	movs r0, #0x80
	lsls r0, r0, #0x18
	b _08029160
_08029158:
	cmp r0, #3
	bne _08029162
	movs r0, #0x80
	lsls r0, r0, #0x17
_08029160:
	orrs r2, r0
_08029162:
	adds r0, r2, #0
	bx lr
	.align 2, 0

	thumb_func_start sub_8029168
sub_8029168: @ 0x08029168
	push {lr}
	ldr r0, _08029178 @ =gUnknown_0300137C
	ldr r0, [r0]
	bl mem_free
	pop {r0}
	bx r0
	.align 2, 0
_08029178: .4byte gUnknown_0300137C

	thumb_func_start DecompressCategorySpriteSheet
DecompressCategorySpriteSheet: @ 0x0802917C
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4]
	lsrs r0, r0, #8
	movs r1, #0x80
	lsls r1, r1, #0x17
	bl mem_alloc
	adds r1, r0, #0
	ldr r0, _080291A0 @ =gUnknown_0300137C
	str r1, [r0]
	adds r0, r4, #0
	bl LoadTaggedAsset
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080291A0: .4byte gUnknown_0300137C

