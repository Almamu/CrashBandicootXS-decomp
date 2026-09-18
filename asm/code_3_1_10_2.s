.include "asm/macros.inc"

.syntax unified
.arm

@ sub_80019F8 is reconstructed (semantics fully understood, but not yet
@ byte-matching) as C in src/audio/audio_context.c, guarded by
@ #if NON_MATCHING - see docs/matching/issue-3-overlay-ui-audio-wrapper.md
@ for the remaining gap (a base-volume field-address CSE this compiler
@ won't reproduce; the earlier u8 stack-parameter byte-load gap was
@ closed this pass).
.if NON_MATCHING == 0
	thumb_func_start sub_80019F8
sub_80019F8: @ 0x080019F8
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	adds r6, r1, #0
	adds r7, r2, #0
	add r0, sp, #0x14
	ldrb r0, [r0]
	mov ip, r0
	ldr r5, _08001A58 @ =gStaticData_0816AA6C
	lsls r0, r6, #1
	adds r0, r0, r6
	lsls r1, r0, #2
	adds r0, r1, r5
	ldr r2, [r0]
	cmp r2, #0
	beq _08001AAE
	cmp r3, #0
	ble _08001AAE
	adds r0, r5, #0
	adds r0, #8
	adds r0, r1, r0
	ldr r0, [r0]
	adds r1, r0, #0
	muls r1, r3, r1
	ldr r0, [r4, #0x2c]
	muls r0, r1, r0
	lsrs r5, r0, #0x10
	ldr r1, [r4, #0x38]
	cmp r1, #0x63
	bne _08001A60
	movs r3, #1
	rsbs r3, r3, #0
	adds r0, r2, #0
	movs r1, #2
	movs r2, #0
	bl sub_8038E74
	ldr r1, [r4, #0x34]
	movs r0, #2
	bl sub_80390F8
	str r6, [r4, #0x38]
	ldr r0, _08001A5C @ =gUnknown_0300082C
	ldr r0, [r0]
	adds r0, r0, r7
	str r0, [r4, #0x3c]
	str r5, [r4, #0x40]
	b _08001AAE
	.align 2, 0
_08001A58: .4byte gStaticData_0816AA6C
_08001A5C: .4byte gUnknown_0300082C
_08001A60:
	ldr r0, [r4, #0x40]
	cmp r5, r0
	blt _08001AAE
	cmp r1, r6
	bne _08001AA0
	ldr r0, _08001A9C @ =gUnknown_0300082C
	ldr r0, [r0]
	adds r0, r0, r7
	str r0, [r4, #0x3c]
	str r5, [r4, #0x40]
	mov r0, ip
	cmp r0, #0
	beq _08001A90
	movs r3, #1
	rsbs r3, r3, #0
	adds r0, r2, #0
	movs r1, #2
	movs r2, #0
	bl sub_8038E74
	ldr r1, [r4, #0x34]
	movs r0, #2
	bl sub_80390F8
_08001A90:
	movs r0, #0x63
	str r0, [r4, #0x44]
	movs r0, #0
	str r0, [r4, #0x4c]
	b _08001AAE
	.align 2, 0
_08001A9C: .4byte gUnknown_0300082C
_08001AA0:
	str r6, [r4, #0x44]
	ldr r0, _08001AB4 @ =gUnknown_0300082C
	ldr r1, [r0]
	adds r0, r1, r7
	str r0, [r4, #0x48]
	str r5, [r4, #0x4c]
	str r1, [r4, #0x3c]
_08001AAE:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08001AB4: .4byte gUnknown_0300082C
.endif
