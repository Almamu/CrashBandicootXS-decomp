.include "asm/macros.inc"

.syntax unified
.arm

@ sub_800944C is reconstructed (semantics fully understood, but not
@ yet byte-matching) as C in src/graphics/actor_part11.c, guarded by
@ #if NON_MATCHING - see docs/matching.md for the exact remaining gap.
.if NON_MATCHING == 0
	thumb_func_start sub_800944C
sub_800944C: @ 0x0800944C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x10
	adds r3, r0, #0
	ldr r0, _08009508 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r2, [r0, #0x10]
	ldr r1, [r2]
	lsls r1, r1, #8
	ldr r0, [r2, #4]
	lsls r0, r0, #8
	str r1, [sp]
	str r0, [sp, #4]
	movs r0, #0xf0
	lsls r0, r0, #8
	movs r1, #0xa0
	lsls r1, r1, #8
	str r0, [sp, #8]
	str r1, [sp, #0xc]
	ldr r5, [r2]
	asrs r5, r5, #8
	cmp r5, #0
	bge _0800947E
	movs r5, #0
_0800947E:
	adds r1, r5, #2
	adds r7, r3, #0
	adds r7, #0x10
	ldr r0, _0800950C @ =0x0000040C
	adds r0, r0, r3
	mov r8, r0
_0800948A:
	lsls r0, r1, #2
	adds r0, r7, r0
	ldr r4, [r0]
	subs r6, r1, #1
	cmp r4, #0
	beq _080094C8
_08009496:
	ldr r0, [r4]
	ldr r2, [r0, #0x18]
	movs r3, #0x30
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x34]
	mov r1, sp
	bl sub_803AD80
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _080094C2
	ldr r0, [r4]
	ldr r2, [r0, #0x18]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r2, #0x24]
	bl sub_803AD7C
	movs r0, #1
	strb r0, [r4, #0x11]
_080094C2:
	ldr r4, [r4, #4]
	cmp r4, #0
	bne _08009496
_080094C8:
	adds r1, r6, #0
	cmp r1, r5
	bge _0800948A
	mov r0, r8
	ldr r4, [r0]
	cmp r4, #0
	beq _0800951A
_080094D6:
	ldr r1, [r4, #0xc]
	ldrb r0, [r1, #0x11]
	cmp r0, #0
	bne _08009510
	ldr r0, [r4]
	ldr r2, [r0, #0x18]
	movs r3, #0x30
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x34]
	mov r1, sp
	bl sub_803AD80
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08009514
	ldr r0, [r4]
	ldr r2, [r0, #0x18]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r2, #0x24]
	bl sub_803AD7C
	b _08009514
	.align 2, 0
_08009508: .4byte gUnknown_03001308
_0800950C: .4byte 0x0000040C
_08009510:
	movs r0, #0
	strb r0, [r1, #0x11]
_08009514:
	ldr r4, [r4, #4]
	cmp r4, #0
	bne _080094D6
_0800951A:
	add sp, #0x10
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
.endif
	.align 2, 0

@ sub_8009528 is reconstructed (semantics fully understood, but not
@ yet byte-matching) as C in src/graphics/actor_part11.c, guarded by
@ #if NON_MATCHING - see docs/matching.md for the exact remaining gap.
.if NON_MATCHING == 0
	thumb_func_start sub_8009528
sub_8009528: @ 0x08009528
	sub sp, #0xc
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x30
	adds r7, r0, #0
	str r1, [sp, #0x50]
	str r2, [sp, #0x54]
	str r3, [sp, #0x58]
	ldr r0, [sp, #0x64]
	mov sb, r0
	ldr r0, _080095E4 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r2, [r0, #0x10]
	ldr r1, [r2]
	lsls r1, r1, #8
	ldr r0, [r2, #4]
	lsls r0, r0, #8
	str r1, [sp, #0xc]
	str r0, [sp, #0x10]
	movs r0, #0xf0
	lsls r0, r0, #8
	movs r1, #0xa0
	lsls r1, r1, #8
	str r0, [sp, #0x14]
	str r1, [sp, #0x18]
	ldr r6, [r2]
	asrs r6, r6, #8
	cmp r6, #0
	bge _0800956A
	movs r6, #0
_0800956A:
	adds r1, r6, #2
	movs r2, #0x10
	adds r2, r2, r7
	mov sl, r2
	ldr r0, _080095E8 @ =0x0000040C
	adds r0, r7, r0
	str r0, [sp, #0x2c]
_08009578:
	lsls r0, r1, #2
	add r0, sl
	ldr r5, [r0]
	subs r1, #1
	mov r8, r1
	cmp r5, #0
	beq _08009616
_08009586:
	ldr r4, [r5]
	ldr r1, [r4, #0x18]
	movs r2, #0x30
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x34]
	add r1, sp, #0xc
	bl sub_803AD80
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08009610
	ldrb r1, [r4, #0xc]
	lsrs r0, r1, #2
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _08009610
	ldr r1, [r4, #0x18]
	adds r1, #0x48
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	cmp r0, #4
	ble _08009610
	ldr r0, _080095EC @ =gUnknown_030012D8
	ldr r0, [r0]
	cmp sb, r0
	bne _080095F0
	add r0, sp, #0x1c
	add r1, sp, #0x50
	movs r2, #0x10
	bl sub_800014C
	str r4, [sp, #4]
	ldr r0, [sp, #0x28]
	str r0, [sp]
	ldr r1, [sp, #0x1c]
	ldr r2, [sp, #0x20]
	ldr r3, [sp, #0x24]
	adds r0, r7, #0
	bl sub_80096C0
	b _08009610
	.align 2, 0
_080095E4: .4byte gUnknown_03001308
_080095E8: .4byte 0x0000040C
_080095EC: .4byte gUnknown_030012D8
_080095F0:
	add r0, sp, #0x1c
	add r1, sp, #0x50
	movs r2, #0x10
	bl sub_800014C
	str r4, [sp, #4]
	mov r0, sb
	str r0, [sp, #8]
	ldr r0, [sp, #0x28]
	str r0, [sp]
	ldr r1, [sp, #0x1c]
	ldr r2, [sp, #0x20]
	ldr r3, [sp, #0x24]
	adds r0, r7, #0
	bl sub_80099F0
_08009610:
	ldr r5, [r5, #4]
	cmp r5, #0
	bne _08009586
_08009616:
	mov r1, r8
	cmp r1, r6
	bge _08009578
	ldr r1, [sp, #0x2c]
	ldr r5, [r1]
	cmp r5, #0
	beq _080096AE
_08009624:
	ldr r4, [r5]
	ldr r1, [r4, #0x18]
	movs r2, #0x30
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x34]
	add r1, sp, #0xc
	bl sub_803AD80
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _080096A8
	ldrb r1, [r4, #0xc]
	lsrs r0, r1, #2
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _080096A8
	ldr r1, [r4, #0x18]
	adds r1, #0x48
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	cmp r0, #4
	ble _080096A8
	ldr r0, _08009684 @ =gUnknown_030012D8
	ldr r0, [r0]
	cmp sb, r0
	bne _08009688
	add r0, sp, #0x1c
	add r1, sp, #0x50
	movs r2, #0x10
	bl sub_800014C
	str r4, [sp, #4]
	ldr r0, [sp, #0x28]
	str r0, [sp]
	ldr r1, [sp, #0x1c]
	ldr r2, [sp, #0x20]
	ldr r3, [sp, #0x24]
	adds r0, r7, #0
	bl sub_80096C0
	b _080096A8
	.align 2, 0
_08009684: .4byte gUnknown_030012D8
_08009688:
	add r0, sp, #0x1c
	add r1, sp, #0x50
	movs r2, #0x10
	bl sub_800014C
	str r4, [sp, #4]
	mov r0, sb
	str r0, [sp, #8]
	ldr r0, [sp, #0x28]
	str r0, [sp]
	ldr r1, [sp, #0x1c]
	ldr r2, [sp, #0x20]
	ldr r3, [sp, #0x24]
	adds r0, r7, #0
	bl sub_80099F0
_080096A8:
	ldr r5, [r5, #4]
	cmp r5, #0
	bne _08009624
_080096AE:
	add sp, #0x30
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r3}
	add sp, #0xc
	bx r3
.endif
