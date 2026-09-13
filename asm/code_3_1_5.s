.include "asm/macros.inc"

.syntax unified
.arm

@ sub_80010E0 is reconstructed (but not yet byte-matching) as C in
@ src/input_util.c, guarded by #if NON_MATCHING - this raw version is
@ only assembled for the default (matching) build. See docs/graphics.md,
@ "Parked, not matched: sub_80010E0".
.if NON_MATCHING == 0
	thumb_func_start sub_80010E0
sub_80010E0: @ 0x080010E0
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	adds r7, r2, #0
	lsls r1, r1, #0x18
	lsrs r4, r1, #0x18
	movs r0, #1
	mov r8, r0
	cmp r6, #0
	beq _08001134
	movs r5, #0
	b _08001104
_080010FA:
	movs r0, #8
	ands r1, r0
	cmp r1, #0
	bne _0800115C
_08001102:
	adds r5, #1
_08001104:
	cmp r5, r6
	bge _08001160
	bl sub_80006A8
	ldr r0, _0800112C @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r0, _08001130 @ =gUnknown_030007E0
	adds r1, r7, #0
	ldrh r0, [r0, #2]
	ands r1, r0
	cmp r4, #0
	beq _08001102
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _080010FA
	b _08001160
	.align 2, 0
_0800112C: .4byte gUnknown_03001304
_08001130: .4byte gUnknown_030007E0
_08001134:
	cmp r4, #0
	beq _08001160
_08001138:
	bl sub_80006A8
	ldr r0, _0800116C @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r0, _08001170 @ =gUnknown_030007E0
	adds r1, r7, #0
	ldrh r0, [r0, #2]
	ands r1, r0
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	bne _08001160
	movs r0, #8
	ands r1, r0
	cmp r1, #0
	beq _08001138
_0800115C:
	movs r0, #0
	mov r8, r0
_08001160:
	mov r0, r8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0800116C: .4byte gUnknown_03001304
_08001170: .4byte gUnknown_030007E0
.endif
