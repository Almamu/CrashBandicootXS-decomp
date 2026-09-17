.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8009D5C is reconstructed (semantics fully understood, but not
@ yet byte-matching) as C in src/graphics/actor_part13.c, guarded by
@ #if NON_MATCHING - see docs/matching.md for the exact remaining gap.
.if NON_MATCHING == 0
	thumb_func_start sub_8009D5C
sub_8009D5C: @ 0x08009D5C
	push {r4, r5, lr}
	adds r5, r0, #0
	movs r0, #8
	ldrb r1, [r5, #0xc]
	orrs r0, r1
	strb r0, [r5, #0xc]
	ldr r0, _08009D7C @ =gUnknown_030012C0
	ldr r0, [r0]
	ldr r0, [r0, #0x78]
	cmp r0, #2
	bgt _08009D80
	cmp r0, #1
	bge _08009DA0
	cmp r0, #0
	beq _08009D86
	b _08009DEE
	.align 2, 0
_08009D7C: .4byte gUnknown_030012C0
_08009D80:
	cmp r0, #3
	beq _08009DD8
	b _08009DEE
_08009D86:
	ldr r0, _08009D9C @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldrb r2, [r5, #0xa]
	ldr r4, [r1, #4]
	movs r1, #0
	b _08009DCA
	.align 2, 0
_08009D9C: .4byte gUnknown_030012D8
_08009DA0:
	ldr r0, _08009DD4 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldrb r2, [r5, #0xa]
	ldr r4, [r1, #4]
	movs r1, #0
	movs r3, #0
	bl sub_803AD88
	ldr r1, [r5, #0x18]
	adds r1, #0x68
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r4, [r1, #4]
	movs r1, #1
	movs r2, #1
_08009DCA:
	movs r3, #0
	bl sub_803AD88
	b _08009DEE
	.align 2, 0
_08009DD4: .4byte gUnknown_030012D8
_08009DD8:
	ldr r1, [r5, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r4, [r1, #4]
	movs r1, #1
	movs r2, #1
	movs r3, #0
	bl sub_803AD88
_08009DEE:
	pop {r4, r5}
	pop {r0}
	bx r0
.endif
