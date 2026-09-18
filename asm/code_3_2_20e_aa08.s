.include "asm/macros.inc"

.syntax unified
.arm

@ sub_803AA08 is reconstructed (but not yet byte-matching) as C in
@ src/system/timer_util.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build.
@ sub_803AA90 (formerly here too, now matches as plain C in its own
@ src/system/timer_util_aa90.c - docs/workflow.md step 4) and
@ sub_803AAD4 (asm/code_3_2_20e_aa90.s) have been cut out. See
@ docs/matching/issue-69-*.md, GitHub issue #69's entry.
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

.endif
