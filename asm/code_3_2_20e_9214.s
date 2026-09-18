.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8039214
sub_8039214: @ 0x08039214
	push {r4, r5, r6, r7, lr}
	adds r5, r2, #0
	movs r3, #0xc0
	lsls r3, r3, #0x13
	adds r2, r0, r3
	adds r0, r0, r2
	lsls r1, r1, #6
	adds r3, r0, r1
	ldrb r2, [r5]
	cmp r2, #0
	beq _080392BC
	movs r0, #0x40
	rsbs r0, r0, #0
	mov ip, r0
_08039230:
	movs r4, #0
	movs r0, #0x3f
	ands r0, r3
	lsrs r1, r0, #1
	ldrb r6, [r5]
	adds r7, r5, #1
	cmp r1, #0x1f
	bgt _08039268
	adds r0, r2, #0
	b _08039250
_08039244:
	adds r1, #1
	adds r4, #1
	cmp r1, #0x1f
	bgt _08039268
	adds r0, r5, r4
	ldrb r0, [r0]
_08039250:
	cmp r0, #0
	beq _08039268
	cmp r0, #0x20
	beq _08039268
	cmp r0, #0xa
	beq _08039268
	cmp r1, #0x1d
	ble _08039244
	mov r0, ip
	ands r0, r3
	adds r3, r0, #0
	adds r3, #0x40
_08039268:
	adds r1, r6, #0
	adds r5, r7, #0
	cmp r1, #0x5f
	bne _08039272
	movs r1, #0x5d
_08039272:
	cmp r1, #0x3a
	bne _08039278
	movs r1, #0x5c
_08039278:
	cmp r1, #0x2e
	bne _0803927E
	movs r1, #0x5b
_0803927E:
	cmp r1, #0xa
	bne _0803928A
	mov r0, ip
	ands r0, r3
	adds r3, r0, #0
	adds r3, #0x3f
_0803928A:
	cmp r1, #0x20
	bne _08039292
	movs r1, #0
	b _080392B2
_08039292:
	cmp r1, #0x40
	bhi _0803929C
	adds r0, r1, #0
	subs r0, #0x2f
	b _080392AE
_0803929C:
	cmp r1, #0x60
	bls _080392A6
	adds r0, r1, #0
	subs r0, #0x56
	b _080392AE
_080392A6:
	cmp r1, #0x40
	bls _080392B2
	adds r0, r1, #0
	subs r0, #0x36
_080392AE:
	lsls r0, r0, #0x18
	lsrs r1, r0, #0x18
_080392B2:
	strh r1, [r3]
	adds r3, #2
	ldrb r2, [r5]
	cmp r2, #0
	bne _08039230
_080392BC:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
