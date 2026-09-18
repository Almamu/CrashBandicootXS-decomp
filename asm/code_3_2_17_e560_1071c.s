.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_801071C
sub_801071C: @ 0x0801071C
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, _08010758 @ =gStaticData_087E4074
	str r0, [r4, #0x18]
	adds r0, r4, #0
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #3
	bne _0801074A
	ldr r1, [r4, #0x48]
	adds r0, r1, #1
	cmp r0, #1
	bls _0801074A
	cmp r1, #0
	beq _08010742
	adds r0, r1, #0
	bl sub_8026EB4
_08010742:
	adds r1, r4, #0
	adds r1, #0x59
	movs r0, #0
	strb r0, [r1]
_0801074A:
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_8008484
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08010758: .4byte gStaticData_087E4074

	thumb_func_start sub_801075C
sub_801075C: @ 0x0801075C
	push {r4, lr}
	adds r4, r0, #0
	bl sub_80084A4
	ldr r0, _08010780 @ =gStaticData_087E4074
	str r0, [r4, #0x18]
	adds r1, r4, #0
	adds r1, #0x59
	movs r0, #0
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_800FEB0
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08010780: .4byte gStaticData_087E4074

	thumb_func_start sub_8010784
sub_8010784: @ 0x08010784
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	lsls r2, r2, #1
	lsls r0, r3, #1
	subs r6, r2, r0
	subs r0, r2, r3
	subs r3, #1
	movs r5, #1
	rsbs r5, r5, #0
	cmp r3, r5
	beq _080107BA
_0801079A:
	cmp r0, #0
	blt _080107B0
	ldr r7, [sp, #0x14]
	adds r4, r4, r7
	ldr r7, [sp, #0x18]
	cmp r4, r7
	blt _080107AC
	adds r0, r1, #0
	b _080107BE
_080107AC:
	adds r0, r0, r6
	b _080107B2
_080107B0:
	adds r0, r0, r2
_080107B2:
	adds r1, #1
	subs r3, #1
	cmp r3, r5
	bne _0801079A
_080107BA:
	movs r0, #1
	rsbs r0, r0, #0
_080107BE:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_80107C4
sub_80107C4: @ 0x080107C4
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	lsls r3, r3, #1
	lsls r0, r2, #1
	subs r6, r3, r0
	subs r0, r3, r2
	subs r2, #1
	movs r5, #1
	rsbs r5, r5, #0
	cmp r2, r5
	beq _080107FA
_080107DA:
	cmp r0, #0
	blt _080107E4
	adds r1, #1
	adds r0, r0, r6
	b _080107E6
_080107E4:
	adds r0, r0, r3
_080107E6:
	ldr r7, [sp, #0x14]
	adds r4, r4, r7
	ldr r7, [sp, #0x18]
	cmp r4, r7
	blt _080107F4
	adds r0, r1, #0
	b _080107FE
_080107F4:
	subs r2, #1
	cmp r2, r5
	bne _080107DA
_080107FA:
	movs r0, #1
	rsbs r0, r0, #0
_080107FE:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

