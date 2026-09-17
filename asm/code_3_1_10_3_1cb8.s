.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8001CB8 is reconstructed (semantics fully understood) but NOT YET
@ byte-matching - parked as C in src/system/link_cable.c, guarded by
@ #if NON_MATCHING. See docs/matching/issue-4-sio-settings-sync.md.
.if NON_MATCHING == 0
	thumb_func_start sub_8001CB8
sub_8001CB8: @ 0x08001CB8
	push {r4, r5, r6, r7, lr}
	adds r3, r0, #0
	movs r1, #0xec
	adds r0, r3, #7
_08001CC0:
	strb r1, [r0]
	subs r0, #1
	cmp r0, r3
	bge _08001CC0
	movs r0, #0xf
	ldrb r1, [r3]
	ands r0, r1
	strb r0, [r3]
	movs r0, #0
	strb r0, [r3, #1]
	ldr r1, _08001D28 @ =0x00001234
	adds r2, r3, #1
	movs r4, #4
	ldr r7, _08001D2C @ =gStaticData_0816AF10
	mov ip, r7
	movs r6, #0xff
	movs r5, #1
	rsbs r5, r5, #0
_08001CE4:
	lsrs r0, r1, #8
	ldrb r7, [r2]
	eors r0, r7
	ands r0, r6
	lsls r0, r0, #1
	add r0, ip
	lsls r1, r1, #8
	ldrh r0, [r0]
	eors r1, r0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	adds r2, #1
	subs r4, #1
	cmp r4, r5
	bne _08001CE4
	strb r1, [r3, #6]
	lsrs r0, r1, #8
	strb r0, [r3, #7]
	ldrb r2, [r3]
	lsrs r1, r2, #4
	lsls r0, r0, #8
	ldrb r4, [r3, #6]
	orrs r0, r4
	adds r1, r1, r0
	movs r0, #0xf
	ands r1, r0
	subs r0, #0x1f
	ands r0, r2
	orrs r0, r1
	strb r0, [r3]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08001D28: .4byte 0x00001234
_08001D2C: .4byte gStaticData_0816AF10
.endif
