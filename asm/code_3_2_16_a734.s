.include "asm/macros.inc"

.syntax unified
.arm

@ sub_800A734 is reconstructed (but not yet byte-matching) as C in
@ src/graphics/actor_part48.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-9-0x08007634-actor.md. (sub_800A810, right after
@ it in ROM order, matched and moved to actor_part48.c outright.)
.if NON_MATCHING == 0
	thumb_func_start sub_800A734
sub_800A734: @ 0x0800A734
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	movs r0, #0x80
	ldrb r1, [r6, #0xc]
	orrs r0, r1
	movs r1, #0x40
	orrs r0, r1
	movs r1, #2
	orrs r0, r1
	movs r1, #5
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r6, #0xc]
	movs r0, #1
	ldrb r2, [r6, #0xd]
	orrs r0, r2
	strb r0, [r6, #0xd]
	movs r4, #0
	str r4, [r6, #0x60]
	str r4, [r6, #0x64]
	str r4, [r6, #0x48]
	str r4, [r6, #0x4c]
	str r4, [r6, #0x50]
	str r4, [r6, #0x54]
	str r4, [r6, #0x58]
	str r4, [r6, #0x5c]
	adds r1, r6, #0
	adds r1, #0x28
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r3, [r1]
	ands r0, r3
	strb r0, [r1]
	adds r1, #0x40
	movs r0, #8
	strb r0, [r1]
	adds r0, r6, #0
	adds r0, #0x24
	strb r4, [r0]
	str r4, [r6, #0x44]
	str r4, [r6, #0x78]
	str r4, [r6, #0x1c]
	adds r0, #0x6c
	strb r4, [r0]
	adds r0, #0x1c
	str r4, [r0]
	subs r0, #0x2c
	strb r4, [r0]
	adds r0, #8
	strb r4, [r0]
	adds r1, #0x24
	ldr r0, _0800A808 @ =gUnknown_0300082C
	ldr r0, [r0]
	str r0, [r1]
	adds r0, r6, #0
	adds r0, #0x91
	strb r4, [r0]
	adds r0, #3
	strb r4, [r0]
	subs r0, #2
	strb r4, [r0]
	movs r0, #1
	strb r0, [r6, #0xa]
	adds r5, r6, #0
	adds r5, #0xb0
	ldr r0, [r5]
	bl sub_800815C
	ldr r2, [r5]
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r1, #0x80
	lsls r1, r1, #1
	adds r0, r6, r1
	strb r4, [r0]
	ldr r2, _0800A80C @ =0x00000101
	adds r0, r6, r2
	strb r4, [r0]
	movs r3, #0x81
	lsls r3, r3, #1
	adds r0, r6, r3
	strb r4, [r0]
	adds r1, #3
	adds r0, r6, r1
	strb r4, [r0]
	adds r2, #3
	adds r0, r6, r2
	strb r4, [r0]
	movs r0, #2
	rsbs r0, r0, #0
	ldrb r3, [r6, #0xc]
	ands r0, r3
	strb r0, [r6, #0xc]
	adds r1, #2
	adds r0, r6, r1
	strb r4, [r0]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0800A808: .4byte gUnknown_0300082C
_0800A80C: .4byte 0x00000101
.endif
