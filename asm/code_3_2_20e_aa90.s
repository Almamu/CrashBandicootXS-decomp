.include "asm/macros.inc"

.syntax unified
.arm

@ sub_803AAD4 is reconstructed (but not yet byte-matching) as C in
@ src/system/timer_util.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. Split out
@ of asm/code_3_2_20e_aa08.s once sub_803AA90 (the function that used
@ to sit between this and sub_803AA08) matched as plain C in its own
@ file - see docs/workflow.md step 4. See docs/matching/issue-69-*.md,
@ GitHub issue #69's entry.
.if NON_MATCHING == 0
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
