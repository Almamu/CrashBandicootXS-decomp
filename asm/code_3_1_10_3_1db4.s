.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8001DB4 is reconstructed (semantics fully understood) but NOT YET
@ byte-matching - parked as C in src/system/link_cable.c, guarded by
@ #if NON_MATCHING. See docs/matching/issue-4-sio-settings-sync.md.
.if NON_MATCHING == 0
	thumb_func_start sub_8001DB4
sub_8001DB4: @ 0x08001DB4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	adds r5, r0, #0
	movs r2, #0
	strb r2, [r5, #6]
	strb r2, [r5, #8]
	strb r2, [r5, #7]
	ldr r1, _08001F40 @ =gUnknown_03000800
	movs r0, #1
	strb r0, [r1]
	strb r2, [r5, #4]
	movs r1, #1
	rsbs r1, r1, #0
	str r1, [r5, #0x1c]
	movs r3, #0xff
	lsls r3, r3, #2
	adds r0, r5, r3
	str r1, [r0]
	adds r0, r5, #0
	adds r0, #0xc4
	str r2, [r0]
	adds r0, #4
	str r2, [r0]
	adds r1, r5, #0
	adds r1, #0xcc
	movs r0, #0x7f
	str r0, [r1]
	adds r4, r5, #0
	adds r4, #0x30
	adds r0, r4, #0
	bl sub_8001CB8
	adds r2, r5, #0
	adds r2, #0x28
	movs r6, #0xff
	movs r3, #3
_08001E04:
	ldrb r7, [r2, #9]
	lsls r1, r7, #8
	ldrb r0, [r2, #8]
	orrs r1, r0
	adds r0, r1, #0
	ands r0, r6
	strb r0, [r2]
	lsrs r1, r1, #8
	strb r1, [r2, #1]
	adds r2, #2
	subs r3, #1
	cmp r3, #0
	bge _08001E04
	movs r0, #0
	str r0, [r5, #0xc]
	str r0, [r5, #0x24]
	movs r6, #0
	adds r1, r5, #0
	adds r1, #0xfc
	str r1, [sp, #8]
	adds r2, r5, #0
	adds r2, #0x20
	str r2, [sp, #0xc]
	movs r3, #0xc8
	mov sl, r3
	movs r7, #0x80
	lsls r7, r7, #1
	adds r7, r5, r7
	str r7, [sp]
	str r4, [sp, #4]
_08001E40:
	mov r0, sl
	muls r0, r6, r0
	adds r0, r0, r5
	movs r1, #0x84
	lsls r1, r1, #1
	adds r0, r0, r1
	adds r1, r0, #0
	adds r1, #0x84
	movs r2, #0
	str r2, [r1]
	adds r1, #4
	str r2, [r1]
	adds r0, #0x8c
	movs r1, #0x7f
	str r1, [r0]
	mov r0, sl
	muls r0, r6, r0
	ldr r3, [sp]
	adds r0, r3, r0
	str r2, [r0]
	movs r4, #0
	adds r7, r6, #1
	mov r8, r7
	movs r0, #0xc8
	adds r1, r6, #0
	muls r1, r0, r1
	adds r0, r5, #0
	adds r0, #0xd0
	adds r2, r1, r0
	ldr r3, [sp, #4]
_08001E7C:
	ldrb r0, [r3, #1]
	lsls r1, r0, #8
	ldrb r7, [r3]
	orrs r1, r7
	adds r0, r1, #0
	movs r7, #0xff
	ands r0, r7
	strb r0, [r2]
	lsrs r1, r1, #8
	strb r1, [r2, #1]
	adds r2, #2
	adds r3, #2
	adds r4, #1
	cmp r4, #3
	ble _08001E7C
	mov r2, sl
	muls r2, r6, r2
	adds r0, r5, r2
	mov ip, r0
	mov r4, ip
	adds r4, #0xd1
	ldrb r3, [r4]
	lsls r1, r3, #0x1c
	lsrs r1, r1, #0x1c
	subs r1, #1
	movs r0, #0xf
	ands r1, r0
	movs r7, #0x10
	rsbs r7, r7, #0
	mov sb, r7
	mov r0, sb
	ands r0, r3
	orrs r0, r1
	strb r0, [r4]
	movs r1, #0x82
	lsls r1, r1, #1
	adds r0, r5, r1
	adds r0, r0, r2
	movs r3, #0
	str r3, [r0]
	ldr r7, [sp, #8]
	adds r2, r7, r2
	str r3, [r2]
	mov r0, ip
	adds r0, #0xd6
	ldr r1, _08001F44 @ =0x00001234
	strh r1, [r0]
	adds r0, #2
	adds r2, r1, #0
	strh r2, [r0]
	mov r6, r8
	cmp r6, #3
	ble _08001E40
	movs r3, #0xfc
	lsls r3, r3, #2
	adds r0, r5, r3
	movs r1, #0
	str r1, [r0]
	movs r7, #0xfd
	lsls r7, r7, #2
	adds r0, r5, r7
	str r1, [r0]
	movs r2, #0xfe
	lsls r2, r2, #2
	adds r0, r5, r2
	str r1, [r0]
	str r1, [r5, #0x38]
	str r1, [r5, #0x3c]
	movs r0, #0xf
	ldrh r3, [r5, #0x20]
	ands r0, r3
	ldr r7, _08001F48 @ =0x0000F0B0
	adds r1, r7, #0
	orrs r0, r1
	strh r0, [r5, #0x20]
	mov r0, sb
	ldr r1, [sp, #0xc]
	ldrb r1, [r1]
	ands r0, r1
	ldr r2, [sp, #0xc]
	strb r0, [r2]
	movs r3, #0x80
	lsls r3, r3, #3
	adds r1, r5, r3
	ldrh r0, [r5, #0x20]
	strh r0, [r1]
	ldrh r1, [r1]
	ldr r0, _08001F4C @ =0x0400012A
	strh r1, [r0]
	movs r0, #0
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08001F40: .4byte gUnknown_03000800
_08001F44: .4byte 0x00001234
_08001F48: .4byte 0x0000F0B0
_08001F4C: .4byte 0x0400012A
.endif
