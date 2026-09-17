.include "asm/macros.inc"

.syntax unified
.arm

@ sub_801434C is reconstructed (but not yet byte-matching) as C in
@ src/graphics/actor_part18.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching.md, "Parked, not matched: sub_801434C".
.if NON_MATCHING == 0
	thumb_func_start sub_801434C
sub_801434C: @ 0x0801434C
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _080143A4 @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp]
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _080143AC
	ldr r0, [r4, #0x10]
	movs r1, #0xb
	bl sub_800AAEC
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #1
	bne _080143AC
	ldr r0, _080143A8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xc
	bl PlaySfx
	ldr r1, [r4, #0x10]
	movs r0, #2
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xd]
	ands r0, r2
	strb r0, [r1, #0xd]
	ldr r1, [r4, #0x10]
	movs r0, #3
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xd]
	ands r0, r2
	strb r0, [r1, #0xd]
	adds r0, r4, #0
	bl sub_8015508
	b _080144D6
	.align 2, 0
_080143A4: .4byte gUnknown_030007E0
_080143A8: .4byte gUnknown_030012BC
_080143AC:
	adds r0, r4, #0
	bl sub_8012A7C
	lsls r0, r0, #0x18
	lsrs r6, r0, #0x18
	cmp r6, #0
	beq _080143BC
	b _080144D6
_080143BC:
	ldr r0, _080143D8 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	cmp r5, #1
	beq _0801441C
	cmp r5, #1
	bgt _080143DC
	cmp r5, #0
	beq _080143E0
	b _08014480
	.align 2, 0
_080143D8: .4byte gUnknown_03001304
_080143DC:
	cmp r5, #2
	bne _08014480
_080143E0:
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x1b
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #1
	bl sub_803AD84
	movs r1, #0
	adds r0, r4, #0
	adds r0, #0x31
	strb r1, [r0]
	adds r2, r4, #0
	adds r2, #0x2f
	movs r0, #1
	strb r0, [r2]
	adds r0, r4, #0
	adds r0, #0x27
	strb r1, [r0]
	b _08014480
_0801441C:
	ldr r0, [r4, #0x10]
	movs r1, #2
	bl sub_800AAEC
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #1
	bne _0801444E
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x15
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #2
	b _0801446E
_0801444E:
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x11
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #4
_0801446E:
	bl sub_803AD84
	adds r0, r4, #0
	adds r0, #0x31
	strb r6, [r0]
	subs r0, #2
	strb r5, [r0]
	subs r0, #8
	strb r6, [r0]
_08014480:
	mov r0, sp
	movs r5, #0xc0
	lsls r5, r5, #1
	ldrh r0, [r0]
	ands r5, r0
	cmp r5, #0
	bne _080144D0
	ldr r0, [r4, #0x10]
	movs r1, #2
	bl sub_800AAEC
	lsls r0, r0, #0x18
	lsrs r6, r0, #0x18
	cmp r6, #1
	bne _080144D0
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #0x12
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #2
	bl sub_803AD84
	adds r0, r4, #0
	adds r0, #0x31
	strb r5, [r0]
	subs r0, #2
	strb r6, [r0]
	subs r0, #8
	strb r5, [r0]
_080144D0:
	adds r0, r4, #0
	bl sub_80122CC
_080144D6:
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

.endif
