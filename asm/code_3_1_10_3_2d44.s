.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8002D44/sub_8002E20 are reconstructed (semantics fully
@ understood) but NOT YET byte-matching - both need r7 as genuine
@ scratch, and this agbcc build never includes r7 in a function's
@ automatic callee-save push/pop (a categorical, reproduced limitation
@ - see the doc comment in src/graphics/settings_menu8a2.c right above
@ their parked C). sub_8002EFC (the third function of this trio, which
@ doesn't touch r7) has already been matched and moved into
@ src/graphics/settings_menu8a2.c unconditionally. See
@ docs/matching/issue-5-overlay-ui-sync.md for the write-up.
.if NON_MATCHING == 0
	thumb_func_start sub_8002D44
sub_8002D44: @ 0x08002D44
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r0
	ldr r1, [r0]
	cmp r1, #0
	beq _08002DFA
	ldr r0, _08002DB0 @ =gUnknown_03000804
	ldr r0, [r0]
	mov ip, r0
	adds r0, #0xc4
	ldr r0, [r0]
	cmp r0, #0
	bne _08002E10
	adds r7, r1, #0
	cmp r7, #0x60
	ble _08002D6A
	movs r7, #0x60
_08002D6A:
	mov r0, r8
	ldr r4, [r0, #0xc]
	mov r3, ip
	adds r3, #0xcc
	movs r0, #0x80
	subs r0, r0, r7
	ldr r1, [r3]
	cmp r1, r0
	bge _08002DB4
	subs r2, r7, #1
	movs r0, #1
	rsbs r0, r0, #0
	cmp r2, r0
	beq _08002DEC
	mov r5, ip
	adds r5, #0xc4
	mov r6, ip
	adds r6, #0x44
	mov ip, r0
_08002D90:
	ldr r0, [r3]
	adds r0, #1
	str r0, [r3]
	ldr r0, [r5]
	adds r0, #1
	str r0, [r5]
	ldr r0, [r3]
	adds r0, r6, r0
	ldrb r1, [r4]
	strb r1, [r0]
	adds r4, #1
	subs r2, #1
	cmp r2, ip
	bne _08002D90
	b _08002DEC
	.align 2, 0
_08002DB0: .4byte gUnknown_03000804
_08002DB4:
	subs r2, r7, #1
	movs r0, #1
	rsbs r0, r0, #0
	cmp r2, r0
	beq _08002DEC
	adds r1, r3, #0
	mov r6, ip
	adds r6, #0xc4
	movs r3, #0x44
	add ip, r3
	mov sb, r0
_08002DCA:
	ldrb r5, [r4]
	adds r4, #1
	ldr r0, [r1]
	movs r3, #0
	cmp r0, #0x7f
	beq _08002DD8
	adds r3, r0, #1
_08002DD8:
	str r3, [r1]
	ldr r0, [r6]
	adds r0, #1
	str r0, [r6]
	ldr r0, [r1]
	add r0, ip
	strb r5, [r0]
	subs r2, #1
	cmp r2, sb
	bne _08002DCA
_08002DEC:
	mov r1, r8
	ldr r0, [r1, #0xc]
	adds r0, r0, r7
	str r0, [r1, #0xc]
	ldr r0, [r1, #0]
	subs r0, r0, r7
	b _08002E0E
_08002DFA:
	ldr r0, _08002E1C @ =gUnknown_03000804
	ldr r0, [r0]
	adds r0, #0xc4
	ldr r0, [r0]
	cmp r0, #0
	bne _08002E10
	movs r1, #0x85
	lsls r1, r1, #2
	add r1, r8
	movs r0, #1
_08002E0E:
	str r0, [r1]
_08002E10:
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08002E1C: .4byte gUnknown_03000804

	thumb_func_start sub_8002E20
sub_8002E20: @ 0x08002E20
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov ip, r0
	ldr r0, _08002E90 @ =gUnknown_03000804
	ldr r3, [r0]
	movs r2, #0xc8
	adds r0, r1, #0
	muls r0, r2, r0
	adds r0, r0, r3
	movs r4, #0xc6
	lsls r4, r4, #1
	adds r0, r0, r4
	ldr r6, [r0]
	cmp r6, #0
	beq _08002EDC
	movs r0, #0x84
	lsls r0, r0, #2
	add r0, ip
	muls r2, r1, r2
	adds r2, r2, r3
	movs r1, #0x84
	lsls r1, r1, #1
	adds r2, r2, r1
	ldr r4, [r0]
	adds r5, r2, #0
	adds r5, #0x88
	movs r0, #0x80
	subs r0, r0, r6
	ldr r1, [r5]
	cmp r1, r0
	bge _08002E94
	subs r3, r6, #1
	movs r0, #1
	rsbs r0, r0, #0
	cmp r3, r0
	beq _08002EC6
	adds r1, r5, #0
	adds r5, r2, #4
	adds r2, #0x84
	adds r7, r0, #0
_08002E72:
	ldr r0, [r1]
	adds r0, r5, r0
	ldrb r0, [r0]
	strb r0, [r4]
	adds r4, #1
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
	ldr r0, [r2]
	subs r0, #1
	str r0, [r2]
	subs r3, #1
	cmp r3, r7
	bne _08002E72
	b _08002EC6
	.align 2, 0
_08002E90: .4byte gUnknown_03000804
_08002E94:
	subs r3, r6, #1
	movs r0, #1
	rsbs r0, r0, #0
	cmp r3, r0
	beq _08002EC6
	adds r1, r2, #0
	adds r1, #0x84
	adds r7, r2, #4
	mov r8, r0
_08002EA6:
	ldr r2, [r5]
	movs r0, #0
	cmp r2, #0x7f
	beq _08002EB0
	adds r0, r2, #1
_08002EB0:
	str r0, [r5]
	ldr r0, [r1]
	subs r0, #1
	str r0, [r1]
	adds r0, r7, r2
	ldrb r0, [r0]
	strb r0, [r4]
	adds r4, #1
	subs r3, #1
	cmp r3, r8
	bne _08002EA6
_08002EC6:
	movs r0, #0x84
	lsls r0, r0, #2
	add r0, ip
	ldr r1, [r0]
	adds r1, r1, r6
	str r1, [r0]
	mov r4, ip
	ldr r0, [r4, #4]
	adds r0, r0, r6
	str r0, [r4, #4]
	b _08002EF2
_08002EDC:
	mov r0, ip
	ldr r1, [r0, #4]
	movs r0, #0x80
	lsls r0, r0, #2
	cmp r1, r0
	bne _08002EF2
	movs r1, #0x86
	lsls r1, r1, #2
	add r1, ip
	movs r0, #1
	str r0, [r1]
_08002EF2:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

.endif
