.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_802F164
sub_802F164: @ 0x0802F164
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	adds r3, r1, #0
	adds r4, r2, #0
	ldr r0, [r5, #0x28]
	cmp r0, #1
	beq _0802F180
	cmp r0, #6
	beq _0802F180
	cmp r0, #2
	beq _0802F180
	cmp r0, #3
	beq _0802F180
	b _0802F322
_0802F180:
	ldr r0, [r5, #0xc]
	cmp r0, #5
	beq _0802F198
	movs r0, #5
	str r0, [r5, #0xc]
	ldr r0, [r5]
	ldrh r0, [r0, #0x3c]
	movs r1, #0
	movs r2, #0
	strh r0, [r5, #0x10]
	strb r1, [r5, #0x12]
	str r2, [r5, #8]
_0802F198:
	str r3, [r5, #0x1c]
	str r4, [r5, #0x20]
	ldr r0, [r5, #0x28]
	cmp r0, #6
	beq _0802F1A8
	movs r0, #0x50
	bl sub_8029BAC
_0802F1A8:
	movs r0, #6
	str r0, [r5, #0x28]
	ldr r2, _0802F1FC @ =gUnknown_03001508
	ldr r1, _0802F200 @ =gUnknown_0300150C
	movs r0, #0
	str r0, [r1]
	str r0, [r2]
	str r0, [r5, #0x44]
	ldr r0, _0802F204 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r0, #0x8c
	ldrb r6, [r0]
	cmp r6, #0
	beq _0802F1C6
	b _0802F322
_0802F1C6:
	bl sub_802A4D4
	ldr r4, _0802F208 @ =gUnknown_030014EC
	ldr r1, [r4]
	subs r0, r0, r1
	cmp r0, #0x14
	bgt _0802F1D6
	b _0802F322
_0802F1D6:
	bl sub_802A4D4
	ldr r1, [r4]
	subs r0, r0, r1
	cmp r0, #0xbe
	ble _0802F1E6
	ldr r0, _0802F20C @ =gUnknown_030014F0
	str r6, [r0]
_0802F1E6:
	ldr r0, _0802F20C @ =gUnknown_030014F0
	ldr r0, [r0]
	cmp r0, #4
	bls _0802F1F0
	b _0802F30A
_0802F1F0:
	lsls r0, r0, #2
	ldr r1, _0802F210 @ =_0802F214
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0802F1FC: .4byte gUnknown_03001508
_0802F200: .4byte gUnknown_0300150C
_0802F204: .4byte gUnknown_030012C0
_0802F208: .4byte gUnknown_030014EC
_0802F20C: .4byte gUnknown_030014F0
_0802F210: .4byte _0802F214
_0802F214: @ jump table
	.4byte _0802F228 @ case 0
	.4byte _0802F258 @ case 1
	.4byte _0802F288 @ case 2
	.4byte _0802F2B8 @ case 3
	.4byte _0802F2E8 @ case 4
_0802F228:
	ldr r0, _0802F24C @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802F30A
	ldr r2, _0802F250 @ =gUnknown_030014FC
	ldr r0, [r2]
	cmp r0, #0
	bne _0802F242
	ldr r1, _0802F254 @ =gUnknown_030014F8
	movs r0, #0xf
	str r0, [r1]
_0802F242:
	ldr r0, [r2]
	adds r0, #1
	str r0, [r2]
	b _0802F30A
	.align 2, 0
_0802F24C: .4byte gUnknown_030012C0
_0802F250: .4byte gUnknown_030014FC
_0802F254: .4byte gUnknown_030014F8
_0802F258:
	ldr r0, _0802F27C @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802F30A
	ldr r2, _0802F280 @ =gUnknown_030014FC
	ldr r0, [r2]
	cmp r0, #0
	bne _0802F272
	ldr r1, _0802F284 @ =gUnknown_030014F8
	movs r0, #0xf
	str r0, [r1]
_0802F272:
	ldr r0, [r2]
	adds r0, #5
	str r0, [r2]
	b _0802F30A
	.align 2, 0
_0802F27C: .4byte gUnknown_030012C0
_0802F280: .4byte gUnknown_030014FC
_0802F284: .4byte gUnknown_030014F8
_0802F288:
	ldr r0, _0802F2AC @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802F30A
	ldr r2, _0802F2B0 @ =gUnknown_030014FC
	ldr r0, [r2]
	cmp r0, #0
	bne _0802F2A2
	ldr r1, _0802F2B4 @ =gUnknown_030014F8
	movs r0, #0xf
	str r0, [r1]
_0802F2A2:
	ldr r0, [r2]
	adds r0, #0x14
	str r0, [r2]
	b _0802F30A
	.align 2, 0
_0802F2AC: .4byte gUnknown_030012C0
_0802F2B0: .4byte gUnknown_030014FC
_0802F2B4: .4byte gUnknown_030014F8
_0802F2B8:
	ldr r0, _0802F2E0 @ =gUnknown_03001506
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802F30A
	ldr r4, _0802F2E4 @ =gUnknown_030014E4
	ldr r1, [r4]
	movs r0, #0x14
	muls r0, r1, r0
	movs r1, #0x64
	bl sub_803ADB4
	ldr r1, [r5, #0x54]
	adds r1, r1, r0
	str r1, [r5, #0x54]
	ldr r4, [r4]
	cmp r1, r4
	ble _0802F30A
	str r4, [r5, #0x54]
	b _0802F30A
	.align 2, 0
_0802F2E0: .4byte gUnknown_03001506
_0802F2E4: .4byte gUnknown_030014E4
_0802F2E8:
	ldr r0, _0802F328 @ =gUnknown_030012C0
	ldr r1, [r0]
	adds r0, r1, #0
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802F30A
	adds r0, r1, #0
	bl sub_8023464
	ldr r0, _0802F32C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #7
	bl PlaySfx
_0802F30A:
	bl sub_802A4D4
	ldr r1, _0802F330 @ =gUnknown_030014EC
	str r0, [r1]
	ldr r1, _0802F334 @ =gUnknown_030014F0
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
	cmp r0, #5
	bne _0802F322
	movs r0, #0
	str r0, [r1]
_0802F322:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802F328: .4byte gUnknown_030012C0
_0802F32C: .4byte gUnknown_030012BC
_0802F330: .4byte gUnknown_030014EC
_0802F334: .4byte gUnknown_030014F0

