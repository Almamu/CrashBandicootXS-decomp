.include "asm/macros.inc"

.syntax unified
.arm

@ sub_803AB54/sub_803AC04/sub_803ACE0/sub_803AD38 are left untouched -
@ no C reconstruction attempted. Working theory (from issue #69, not
@ confirmed enough to commit even a NON_MATCHING reconstruction): these
@ build/verify a bit-serial transmission for the GBA EEPROM save chip
@ whose config table `sub_803A968` selects (see the header comment on
@ that function and on `EepromConfig` in src/system/timer_util.c) -
@ sub_803AB54 packs a start-bit pair, the chip's address bits, and 64
@ transposed data bits into a stack buffer and DMAs it out twice via
@ sub_803AAD4 (once for a `addrBitCount+3`-halfword send, once for a
@ 0x44-halfword follow-up whose purpose isn't pinned down), sub_803AC04
@ builds a similar buffer and additionally arms the timer via
@ sub_803AA08/sub_803AA90 with an IRQ-flag busy-wait, sub_803ACE0 calls
@ sub_803AB54 and compares the result against the caller's buffer, and
@ sub_803AD38 retries sub_803AC04 then sub_803ACE0 up to 3 times. The
@ overall shape (chip-size-keyed bit count, DMA to 0x0D000000 inside
@ sub_803AAD4, a 3-attempt retry wrapper) is consistent with a
@ read+verify EEPROM access routine, but the exact bit-count arithmetic
@ (why sub_803AB54's second DMA sends 0x44 halfwords, what the "+3" is
@ made of) isn't confidently worked out yet - left for whoever picks up
@ the rest of this chunk.
	thumb_func_start sub_803AB54
sub_803AB54: @ 0x0803AB54
	push {r4, r5, r6, lr}
	sub sp, #0x88
	adds r5, r1, #0
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	ldr r0, _0803AB6C @ =gUnknown_03001634
	ldr r0, [r0]
	ldrh r0, [r0, #4]
	cmp r3, r0
	blo _0803AB74
	ldr r0, _0803AB70 @ =0x000080FF
	b _0803ABF6
	.align 2, 0
_0803AB6C: .4byte gUnknown_03001634
_0803AB70: .4byte 0x000080FF
_0803AB74:
	ldr r0, _0803AC00 @ =gUnknown_03001634
	adds r6, r0, #0
	ldr r0, [r0]
	ldrb r1, [r0, #8]
	lsls r0, r1, #1
	mov r4, sp
	adds r2, r0, r4
	adds r2, #2
	movs r4, #0
	cmp r4, r1
	bhs _0803AB9E
_0803AB8A:
	strh r3, [r2]
	subs r2, #2
	lsrs r3, r3, #1
	adds r0, r4, #1
	lsls r0, r0, #0x18
	lsrs r4, r0, #0x18
	ldr r0, [r6]
	ldrb r0, [r0, #8]
	cmp r4, r0
	blo _0803AB8A
_0803AB9E:
	movs r0, #1
	strh r0, [r2]
	subs r2, #2
	strh r0, [r2]
	movs r4, #0xd0
	lsls r4, r4, #0x14
	ldr r0, _0803AC00 @ =gUnknown_03001634
	ldr r0, [r0]
	ldrb r2, [r0, #8]
	adds r2, #3
	mov r0, sp
	adds r1, r4, #0
	bl sub_803AAD4
	adds r0, r4, #0
	mov r1, sp
	movs r2, #0x44
	bl sub_803AAD4
	add r2, sp, #8
	adds r5, #6
	movs r4, #0
	movs r6, #1
_0803ABCC:
	movs r1, #0
	movs r3, #0
_0803ABD0:
	lsls r1, r1, #0x11
	ldrh r0, [r2]
	ands r0, r6
	lsrs r1, r1, #0x10
	orrs r1, r0
	adds r2, #2
	adds r0, r3, #1
	lsls r0, r0, #0x18
	lsrs r3, r0, #0x18
	cmp r3, #0xf
	bls _0803ABD0
	strh r1, [r5]
	subs r5, #2
	adds r0, r4, #1
	lsls r0, r0, #0x18
	lsrs r4, r0, #0x18
	cmp r4, #3
	bls _0803ABCC
	movs r0, #0
_0803ABF6:
	add sp, #0x88
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0803AC00: .4byte gUnknown_03001634

	thumb_func_start sub_803AC04
sub_803AC04: @ 0x0803AC04
	push {r4, r5, lr}
	sub sp, #0xa4
	adds r5, r1, #0
	lsls r0, r0, #0x10
	lsrs r4, r0, #0x10
	ldr r0, _0803AC1C @ =gUnknown_03001634
	ldr r0, [r0]
	ldrh r0, [r0, #4]
	cmp r4, r0
	blo _0803AC24
	ldr r0, _0803AC20 @ =0x000080FF
	b _0803ACC8
	.align 2, 0
_0803AC1C: .4byte gUnknown_03001634
_0803AC20: .4byte 0x000080FF
_0803AC24:
	ldr r0, _0803AC64 @ =gUnknown_03001634
	ldr r0, [r0]
	ldrb r0, [r0, #8]
	lsls r0, r0, #1
	mov r1, sp
	adds r3, r0, r1
	adds r3, #0x84
	movs r0, #0
	strh r0, [r3]
	subs r3, #2
	movs r1, #0
_0803AC3A:
	ldrh r2, [r5]
	adds r5, #2
	movs r0, #0
_0803AC40:
	strh r2, [r3]
	subs r3, #2
	lsrs r2, r2, #1
	adds r0, #1
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #0xf
	bls _0803AC40
	adds r0, r1, #1
	lsls r0, r0, #0x18
	lsrs r1, r0, #0x18
	cmp r1, #3
	bls _0803AC3A
	movs r1, #0
	ldr r0, _0803AC64 @ =gUnknown_03001634
	adds r2, r0, #0
	ldr r0, [r0]
	b _0803AC76
	.align 2, 0
_0803AC64: .4byte gUnknown_03001634
_0803AC68:
	strh r4, [r3]
	subs r3, #2
	lsrs r4, r4, #1
	adds r0, r1, #1
	lsls r0, r0, #0x18
	lsrs r1, r0, #0x18
	ldr r0, [r2]
_0803AC76:
	ldrb r0, [r0, #8]
	cmp r1, r0
	blo _0803AC68
	movs r0, #0
	strh r0, [r3]
	subs r3, #2
	movs r0, #1
	strh r0, [r3]
	movs r1, #0xd0
	lsls r1, r1, #0x14
	ldr r0, _0803ACD0 @ =gUnknown_03001634
	ldr r0, [r0]
	ldrb r2, [r0, #8]
	adds r2, #0x43
	mov r0, sp
	bl sub_803AAD4
	ldr r0, _0803ACD4 @ =gStaticData_085A9F10
	bl sub_803AA08
	movs r4, #0
	movs r1, #0xd0
	lsls r1, r1, #0x14
	movs r3, #1
	ldr r2, _0803ACD8 @ =gUnknown_03001624
_0803ACA8:
	ldrh r0, [r1]
	ands r0, r3
	cmp r0, #0
	bne _0803ACC2
	ldrb r0, [r2]
	cmp r0, #0
	beq _0803ACA8
	ldrh r0, [r1]
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	bne _0803ACC2
	ldr r4, _0803ACDC @ =0x0000C001
_0803ACC2:
	bl sub_803AA90
	adds r0, r4, #0
_0803ACC8:
	add sp, #0xa4
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0803ACD0: .4byte gUnknown_03001634
_0803ACD4: .4byte gStaticData_085A9F10
_0803ACD8: .4byte gUnknown_03001624
_0803ACDC: .4byte 0x0000C001

	thumb_func_start sub_803ACE0
sub_803ACE0: @ 0x0803ACE0
	push {r4, r5, lr}
	sub sp, #8
	adds r4, r1, #0
	lsls r0, r0, #0x10
	lsrs r1, r0, #0x10
	movs r5, #0
	ldr r0, _0803ACFC @ =gUnknown_03001634
	ldr r0, [r0]
	ldrh r0, [r0, #4]
	cmp r1, r0
	blo _0803AD04
	ldr r0, _0803AD00 @ =0x000080FF
	b _0803AD2E
	.align 2, 0
_0803ACFC: .4byte gUnknown_03001634
_0803AD00: .4byte 0x000080FF
_0803AD04:
	adds r0, r1, #0
	mov r1, sp
	bl sub_803AB54
	mov r2, sp
	movs r3, #0
	b _0803AD1C
_0803AD12:
	adds r0, r3, #1
	lsls r0, r0, #0x18
	lsrs r3, r0, #0x18
	cmp r3, #3
	bhi _0803AD2C
_0803AD1C:
	ldrh r1, [r4]
	ldrh r0, [r2]
	adds r2, #2
	adds r4, #2
	cmp r1, r0
	beq _0803AD12
	movs r5, #0x80
	lsls r5, r5, #8
_0803AD2C:
	adds r0, r5, #0
_0803AD2E:
	add sp, #8
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_803AD38
sub_803AD38: @ 0x0803AD38
	push {r4, r5, r6, lr}
	adds r5, r1, #0
	lsls r0, r0, #0x10
	lsrs r4, r0, #0x10
	movs r6, #0
	b _0803AD4A
_0803AD44:
	adds r0, r6, #1
	lsls r0, r0, #0x18
	lsrs r6, r0, #0x18
_0803AD4A:
	cmp r6, #2
	bhi _0803AD6E
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_803AC04
	lsls r0, r0, #0x10
	lsrs r2, r0, #0x10
	cmp r2, #0
	bne _0803AD44
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_803ACE0
	lsls r0, r0, #0x10
	lsrs r2, r0, #0x10
	cmp r2, #0
	bne _0803AD44
_0803AD6E:
	adds r0, r2, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
