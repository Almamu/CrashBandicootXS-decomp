.include "asm/macros.inc"

.syntax unified
.arm

@ sub_803AA08/sub_803AA90/sub_803AAD4 are reconstructed (but not yet
@ byte-matching) as C in src/system/timer_util.c, guarded by
@ #if NON_MATCHING - this raw version is only assembled for the default
@ (matching) build. See docs/matching.md, GitHub issue #69's entry.
.if NON_MATCHING == 0
	thumb_func_start sub_803AA08
sub_803AA08: @ 0x0803AA08
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	ldr r2, _0803AA74 @ =gUnknown_0300162C
	ldr r1, _0803AA78 @ =0x04000208
	mov sb, r1
	ldrh r1, [r1]
	strh r1, [r2]
	movs r6, #0
	mov r2, sb
	strh r6, [r2]
	ldr r3, _0803AA7C @ =gUnknown_03001628
	mov r8, r3
	ldr r5, [r3]
	strh r6, [r5, #2]
	ldr r3, _0803AA80 @ =0x04000202
	ldr r4, _0803AA84 @ =gUnknown_03001620
	ldrb r1, [r4]
	movs r2, #8
	adds r7, r2, #0
	lsls r7, r1
	adds r1, r7, #0
	strh r1, [r3]
	subs r3, #2
	ldrb r1, [r4]
	lsls r2, r1
	ldrh r1, [r3]
	orrs r1, r2
	strh r1, [r3]
	ldr r1, _0803AA88 @ =gUnknown_03001624
	strb r6, [r1]
	ldr r2, _0803AA8C @ =gUnknown_03001622
	ldrh r1, [r0]
	strh r1, [r2]
	adds r0, #2
	ldrh r1, [r0]
	strh r1, [r5]
	adds r1, r5, #2
	mov r2, r8
	str r1, [r2]
	ldrh r0, [r0, #2]
	strh r0, [r5, #2]
	str r5, [r2]
	movs r0, #1
	mov r3, sb
	strh r0, [r3]
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0803AA74: .4byte gUnknown_0300162C
_0803AA78: .4byte 0x04000208
_0803AA7C: .4byte gUnknown_03001628
_0803AA80: .4byte 0x04000202
_0803AA84: .4byte gUnknown_03001620
_0803AA88: .4byte gUnknown_03001624
_0803AA8C: .4byte gUnknown_03001622

	thumb_func_start sub_803AA90
sub_803AA90: @ 0x0803AA90
	ldr r3, _0803AAC0 @ =0x04000208
	movs r1, #0
	strh r1, [r3]
	ldr r2, _0803AAC4 @ =gUnknown_03001628
	ldr r0, [r2]
	strh r1, [r0]
	adds r0, #2
	str r0, [r2]
	strh r1, [r0]
	subs r0, #2
	str r0, [r2]
	ldr r2, _0803AAC8 @ =0x04000200
	ldr r0, _0803AACC @ =gUnknown_03001620
	ldrb r0, [r0]
	movs r1, #8
	lsls r1, r0
	ldrh r0, [r2]
	bics r0, r1
	strh r0, [r2]
	ldr r0, _0803AAD0 @ =gUnknown_0300162C
	ldrh r0, [r0]
	strh r0, [r3]
	bx lr
	.align 2, 0
_0803AAC0: .4byte 0x04000208
_0803AAC4: .4byte gUnknown_03001628
_0803AAC8: .4byte 0x04000200
_0803AACC: .4byte gUnknown_03001620
_0803AAD0: .4byte gUnknown_0300162C

	thumb_func_start sub_803AAD4
sub_803AAD4: @ 0x0803AAD4
	push {r4, r5, r6, lr}
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	ldr r4, _0803AB34 @ =0x04000208
	ldrh r3, [r4]
	adds r6, r3, #0
	movs r3, #0
	strh r3, [r4]
	ldr r5, _0803AB38 @ =0x04000204
	ldrh r4, [r5]
	ldr r3, _0803AB3C @ =0x0000F8FF
	ands r4, r3
	ldr r3, _0803AB40 @ =gUnknown_03001634
	ldr r3, [r3]
	ldrh r3, [r3, #6]
	orrs r4, r3
	strh r4, [r5]
	ldr r3, _0803AB44 @ =0x040000D4
	str r0, [r3]
	ldr r0, _0803AB48 @ =0x040000D8
	str r1, [r0]
	ldr r1, _0803AB4C @ =0x040000DC
	movs r0, #0x80
	lsls r0, r0, #0x18
	orrs r2, r0
	str r2, [r1]
	adds r1, #2
	movs r2, #0x80
	lsls r2, r2, #8
	adds r0, r2, #0
	ldrh r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _0803AB28
	ldr r2, _0803AB50 @ =0x040000DE
	movs r0, #0x80
	lsls r0, r0, #8
	adds r1, r0, #0
_0803AB20:
	ldrh r0, [r2]
	ands r0, r1
	cmp r0, #0
	bne _0803AB20
_0803AB28:
	ldr r0, _0803AB34 @ =0x04000208
	strh r6, [r0]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0803AB34: .4byte 0x04000208
_0803AB38: .4byte 0x04000204
_0803AB3C: .4byte 0x0000F8FF
_0803AB40: .4byte gUnknown_03001634
_0803AB44: .4byte 0x040000D4
_0803AB48: .4byte 0x040000D8
_0803AB4C: .4byte 0x040000DC
_0803AB50: .4byte 0x040000DE

.endif
