.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8015238 and sub_80152F0 are reconstructed (but not yet
@ byte-matching) as C in src/graphics/actor_part28b.c, guarded by
@ #if NON_MATCHING - this raw version is only assembled for the
@ default (matching) build. See docs/matching/issue-18-0x08014f8c-
@ actor.md, "Parked, not matched: sub_8015238" and "Parked, not
@ matched: sub_80152F0".
.if NON_MATCHING == 0
	thumb_func_start sub_8015238
sub_8015238: @ 0x08015238
	push {r4, r5, lr}
	sub sp, #4
	adds r5, r0, #0
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	adds r3, r5, #0
	adds r3, #0x26
	movs r0, #0xc
	strb r0, [r3]
	cmp r1, #4
	bgt _080152BC
	cmp r1, #3
	blt _080152BC
	movs r1, #0x80
	lsls r1, r1, #2
	adds r0, r1, #0
	ands r2, r0
	cmp r2, #0
	beq _080152B4
	ldr r0, _080152B0 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80231C4
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _080152B4
	adds r0, r5, #0
	adds r0, #0x29
	movs r4, #1
	strb r4, [r0]
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #4
	bl sub_803AD80
	ldr r2, [r5, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x18
	bl sub_803AD84
	movs r1, #0x1b
	adds r2, r5, #0
	adds r2, #0x31
	movs r0, #0
	strb r0, [r2]
	adds r0, r5, #0
	adds r0, #0x2f
	strb r4, [r0]
	subs r0, #8
	strb r1, [r0]
	b _080152E8
	.align 2, 0
_080152B0: .4byte gUnknown_030012C0
_080152B4:
	adds r0, r5, #0
	bl sub_8015460
	b _080152E8
_080152BC:
	movs r4, #0
	str r4, [sp]
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0x12
	movs r3, #0
	bl sub_8015780
	adds r0, r5, #0
	adds r0, #0x31
	strb r4, [r0]
	subs r0, #2
	movs r1, #1
	strb r1, [r0]
	subs r0, #8
	strb r4, [r0]
	adds r0, #0xb
	strb r4, [r0]
	subs r0, #2
	strb r1, [r0]
	subs r0, #8
	strb r4, [r0]
_080152E8:
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_80152F0
sub_80152F0: @ 0x080152F0
	push {r4, lr}
	adds r2, r0, #0
	lsls r1, r1, #0x18
	lsrs r4, r1, #0x18
	movs r0, #0x27
	adds r0, r0, r2
	mov ip, r0
	ldrb r0, [r0]
	cmp r0, #0
	bne _08015328
	adds r0, r2, #0
	adds r0, #0x2b
	ldrb r3, [r0]
	cmp r3, #0
	bne _08015328
	cmp r4, #4
	bgt _08015328
	cmp r4, #3
	blt _08015328
	movs r1, #0x17
	adds r0, #6
	strb r3, [r0]
	adds r3, r2, #0
	adds r3, #0x2f
	movs r0, #1
	strb r0, [r3]
	mov r0, ip
	strb r1, [r0]
_08015328:
	cmp r4, #2
	bhi _08015342
	movs r1, #0
	adds r0, r2, #0
	adds r0, #0x31
	strb r1, [r0]
	adds r3, r2, #0
	adds r3, #0x2f
	movs r0, #1
	strb r0, [r3]
	adds r0, r2, #0
	adds r0, #0x27
	strb r1, [r0]
_08015342:
	adds r0, r2, #0
	bl sub_80122CC
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

.endif
