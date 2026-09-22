.include "asm/macros.inc"

.syntax unified
.arm

@ sub_803472C: not yet matched (NON_MATCHING) - see
@ src/graphics/actor_part87.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. Fully
@ semantically understood (every field/call/register choice checked
@ against the ROM) and extremely close (register-pinned + inline-asm
@ anchors reproduce the vast majority of the body byte-for-byte), but a
@ handful of individual accumulator/temp-register choices in the BLDCNT/
@ BLDALPHA byte-packing sequence (e.g. which of two freshly-available low
@ registers holds a reloaded byte vs. an already-live constant) never
@ converged after extensive iteration - see docs/matching/issue-63-0x08033ef4-actor.md.
.if NON_MATCHING == 0
	thumb_func_start sub_803472C
sub_803472C: @ 0x0803472C
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	sub sp, #4
	adds r5, r0, #0
	movs r0, #0x10
	bl sub_8026EDC
	movs r1, #3
	str r1, [sp]
	movs r1, #0
	movs r2, #0x1f
	movs r3, #0
	bl sub_801E644
	str r0, [r5, #4]
	movs r0, #0x10
	bl sub_8026EDC
	movs r1, #1
	str r1, [sp]
	movs r1, #3
	movs r2, #0x1e
	movs r3, #0
	bl sub_801E644
	str r0, [r5]
	movs r0, #0x10
	bl sub_8026EDC
	movs r1, #2
	str r1, [sp]
	movs r2, #0x1d
	movs r3, #1
	bl sub_801E644
	str r0, [r5, #8]
	ldr r0, [r5]
	ldr r1, _08034854 @ =gStaticData_0817C5BC
	bl LoadGraphicsPackage
	ldr r0, [r5, #4]
	ldr r1, _08034858 @ =gStaticData_0817C594
	bl LoadGraphicsPackage
	ldr r0, [r5, #8]
	ldr r1, _0803485C @ =gStaticData_0817C5A8
	bl LoadGraphicsPackage
	movs r1, #0xa0
	lsls r1, r1, #0x13
	movs r0, #0
	strh r0, [r1]
	movs r0, #0
	mov r8, r0
	mov r1, r8
	strh r1, [r5, #0xc]
	movs r2, #0x40
	mov sb, r2
	mov r0, sb
	ldrb r1, [r5, #0xc]
	orrs r0, r1
	movs r1, #8
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r5, #0xc]
	movs r6, #1
	ldrb r0, [r5, #0xd]
	orrs r0, r6
	movs r1, #2
	orrs r0, r1
	movs r4, #4
	orrs r0, r4
	strb r0, [r5, #0xd]
	adds r0, r5, #0
	bl sub_803487C
	mov r2, r8
	str r2, [r5, #0x10]
	ldrb r0, [r5, #0x10]
	orrs r4, r0
	ldrb r1, [r5, #0x11]
	orrs r6, r1
	strb r6, [r5, #0x11]
	movs r1, #0x20
	rsbs r1, r1, #0
	adds r0, r1, #0
	ldrb r2, [r5, #0x12]
	ands r0, r2
	movs r2, #8
	orrs r0, r2
	strb r0, [r5, #0x12]
	ldrb r0, [r5, #0x13]
	ands r1, r0
	movs r0, #0x10
	orrs r1, r0
	strb r1, [r5, #0x13]
	movs r0, #0x3f
	ands r4, r0
	mov r1, sb
	orrs r4, r1
	strb r4, [r5, #0x10]
	ldr r0, [r5, #4]
	bl sub_801E640
	ldr r1, _08034860 @ =0x04000008
	strh r0, [r1]
	ldr r0, _08034864 @ =0x04000010
	mov r2, r8
	str r2, [r0]
	ldr r0, [r5]
	bl sub_801E640
	ldr r1, _08034868 @ =0x0400000A
	strh r0, [r1]
	ldr r0, _0803486C @ =0x04000014
	mov r1, r8
	str r1, [r0]
	ldr r0, [r5, #8]
	bl sub_801E640
	ldr r1, _08034870 @ =0x0400000C
	strh r0, [r1]
	ldr r0, _08034874 @ =0x04000018
	mov r2, r8
	str r2, [r0]
	subs r1, #0xc
	ldrh r0, [r5, #0xc]
	strh r0, [r1]
	adds r1, #0x50
	ldr r0, [r5, #0x10]
	str r0, [r1]
	str r2, [r5, #0x1c]
	str r2, [r5, #0x20]
	ldr r0, _08034878 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8001AC4
	adds r0, r5, #0
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08034854: .4byte gStaticData_0817C5BC
_08034858: .4byte gStaticData_0817C594
_0803485C: .4byte gStaticData_0817C5A8
_08034860: .4byte 0x04000008
_08034864: .4byte 0x04000010
_08034868: .4byte 0x0400000A
_0803486C: .4byte 0x04000014
_08034870: .4byte 0x0400000C
_08034874: .4byte 0x04000018
_08034878: .4byte gUnknown_030012BC

.endif
