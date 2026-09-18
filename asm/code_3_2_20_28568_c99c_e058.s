.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_802E058
sub_802E058: @ 0x0802E058
	push {r4, r5, r6, r7, lr}
	mov ip, r0
	lsls r1, r1, #0x18
	lsrs r5, r1, #0x18
	movs r4, #0
	movs r7, #0xff
_0802E064:
	movs r3, #0
	lsls r0, r4, #4
	adds r6, r4, #1
	mov r1, ip
	adds r2, r0, r1
_0802E06E:
	subs r0, r3, #3
	cmp r0, #9
	bhi _0802E07C
	cmp r4, #2
	ble _0802E07C
	cmp r4, #0xc
	ble _0802E080
_0802E07C:
	strb r7, [r2]
	b _0802E08A
_0802E080:
	adds r1, r5, #0
	adds r0, r1, #1
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	strb r1, [r2]
_0802E08A:
	adds r2, #1
	adds r3, #1
	cmp r3, #0xf
	ble _0802E06E
	adds r4, r6, #0
	cmp r4, #0xf
	ble _0802E064
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

