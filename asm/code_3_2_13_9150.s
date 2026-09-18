.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8009150 is reconstructed (semantics fully understood, but not
@ yet byte-matching) as C in src/graphics/actor_part11.c, guarded by
@ #if NON_MATCHING - see docs/matching.md for the exact remaining gap.
.if NON_MATCHING == 0
	thumb_func_start sub_8009150
sub_8009150: @ 0x08009150
	push {r4, r5, r6, r7, lr}
	adds r7, r0, #0
	adds r3, r1, #0
	movs r2, #0xfe
_08009158:
	lsls r1, r2, #2
	adds r0, r7, #0
	adds r0, #0x10
	adds r0, r0, r1
	ldr r4, [r0]
	cmp r4, #0
	beq _080091C6
	ldr r0, _080091B4 @ =0x00000814
	adds r6, r7, r0
_0800916A:
	ldr r5, [r4]
	cmp r5, r3
	bne _080091C0
	ldrb r1, [r5, #0xc]
	lsrs r0, r1, #4
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _080091CC
	ldr r3, [r4, #0xc]
	cmp r3, #0
	bne _080091CC
	ldr r0, [r6]
	ldr r2, [r0]
	ldr r1, [r0, #4]
	str r1, [r6]
	str r3, [r0, #4]
	str r5, [r2]
	str r3, [r2, #4]
	str r4, [r2, #0xc]
	strb r3, [r2, #0x10]
	strb r3, [r2, #0x11]
	ldr r0, _080091B8 @ =0x0000040C
	adds r1, r7, r0
	ldr r0, [r1]
	cmp r0, #0
	bne _080091A2
	str r2, [r1]
_080091A2:
	ldr r1, _080091BC @ =0x0000080C
	adds r0, r7, r1
	ldr r1, [r0]
	cmp r1, #0
	beq _080091AE
	str r2, [r1, #4]
_080091AE:
	str r2, [r0]
	str r2, [r4, #0xc]
	b _080091CC
	.align 2, 0
_080091B4: .4byte 0x00000814
_080091B8: .4byte 0x0000040C
_080091BC: .4byte 0x0000080C
_080091C0:
	ldr r4, [r4, #4]
	cmp r4, #0
	bne _0800916A
_080091C6:
	subs r2, #1
	cmp r2, #0
	bge _08009158
_080091CC:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
.endif
	.align 2, 0
