.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8025D74
sub_8025D74: @ 0x08025D74
	push {r4, r5, lr}
	adds r5, r0, #0
	adds r4, r1, #0
	bl sub_8024DAC
	ldr r0, _08025DDC @ =gStaticData_087E4C14
	str r0, [r5, #0x30]
	adds r1, r4, #0
	adds r1, #0x1c
	lsls r0, r1, #0xb
	movs r2, #0xc0
	lsls r2, r2, #0x13
	adds r0, r0, r2
	str r0, [r5, #0x4c]
	lsls r0, r4, #1
	ldr r3, _08025DE0 @ =0x04000008
	adds r0, r0, r3
	str r0, [r5, #0x38]
	lsls r4, r4, #2
	ldr r0, _08025DE4 @ =0x04000010
	adds r4, r4, r0
	str r4, [r5, #0x58]
	movs r0, #0
	strh r0, [r5, #0x34]
	adds r2, r5, #0
	adds r2, #0x34
	movs r0, #0x7f
	ldrb r3, [r2]
	ands r0, r3
	strb r0, [r2]
	adds r3, r5, #0
	adds r3, #0x35
	movs r0, #0x1f
	ands r1, r0
	movs r0, #0x20
	rsbs r0, r0, #0
	ldrb r4, [r3]
	ands r0, r4
	orrs r0, r1
	strb r0, [r3]
	movs r0, #0xd
	rsbs r0, r0, #0
	ldrb r1, [r2]
	ands r0, r1
	movs r1, #8
	orrs r0, r1
	strb r0, [r2]
	adds r0, r5, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08025DDC: .4byte gStaticData_087E4C14
_08025DE0: .4byte 0x04000008
_08025DE4: .4byte 0x04000010

