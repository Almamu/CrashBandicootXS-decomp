.include "asm/macros.inc"

.syntax unified
.arm

@ sub_800D040 is reconstructed (but not yet byte-matching) as C in
@ src/system/game_loop6.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-12-physics-collision.md.
.if NON_MATCHING == 0
	thumb_func_start sub_800D040
sub_800D040: @ 0x0800D040
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #0x20
	adds r6, r0, #0
	adds r1, r6, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #1
	bne _0800D05C
	b _0800D17C
_0800D05C:
	ldr r1, [r6, #0x20]
	adds r2, r6, #0
	adds r2, #0x2d
	ldrb r3, [r2]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	ldr r1, [r1]
	adds r1, r1, r0
	adds r3, r1, #4
	ldr r0, [r6]
	asrs r7, r0, #8
	ldr r0, [r6, #4]
	asrs r0, r0, #8
	mov r8, r0
	movs r0, #4
	ldrsh r1, [r1, r0]
	movs r0, #2
	ldrsh r2, [r3, r0]
	ldrb r4, [r3, #4]
	ldrb r5, [r3, #5]
	adds r1, r1, r7
	add r2, r8
	mov r0, sp
	bl sub_803AFE4
	mov r0, sp
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_803AFDC
	adds r3, r6, #0
	adds r3, #0x28
	ldrb r1, [r3]
	lsls r0, r1, #0x1b
	cmp r0, #0
	bge _0800D0B2
	lsls r0, r7, #1
	ldr r1, [sp]
	ldr r2, [sp, #8]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp]
_0800D0B2:
	ldrb r3, [r3]
	lsls r0, r3, #0x1a
	cmp r0, #0
	bge _0800D0C8
	mov r2, r8
	lsls r0, r2, #1
	ldr r1, [sp, #4]
	ldr r2, [sp, #0xc]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #4]
_0800D0C8:
	ldr r3, _0800D168 @ =gUnknown_030012D8
	mov sb, r3
	ldr r0, [r3]
	ldr r1, [r0]
	asrs r7, r1, #8
	ldr r1, [r0, #4]
	asrs r1, r1, #8
	mov r8, r1
	ldr r2, [r0, #0x20]
	adds r0, #0x2d
	ldrb r3, [r0]
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	ldr r0, [r2]
	adds r0, r0, r1
	adds r3, r0, #4
	movs r2, #4
	ldrsh r1, [r0, r2]
	movs r0, #2
	ldrsh r2, [r3, r0]
	ldrb r4, [r3, #4]
	ldrb r5, [r3, #5]
	adds r1, r1, r7
	add r2, r8
	add r0, sp, #0x10
	bl sub_803AFE4
	add r0, sp, #0x10
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_803AFDC
	mov r1, sb
	ldr r0, [r1]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _0800D124
	lsls r0, r7, #1
	ldr r1, [sp, #0x10]
	ldr r2, [sp, #0x18]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #0x10]
_0800D124:
	mov r2, sb
	ldr r0, [r2]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1a
	cmp r0, #0
	bge _0800D140
	mov r3, r8
	lsls r0, r3, #1
	ldr r1, [sp, #0x14]
	ldr r2, [sp, #0x1c]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #0x14]
_0800D140:
	add r1, sp, #0x10
	mov r0, sp
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0800D17C
	ldr r0, _0800D16C @ =gStaticData_0816BBC4
	adds r1, r6, #0
	adds r1, #0x4e
	ldrb r1, [r1]
	adds r0, r1, r0
	ldrb r0, [r0]
	cmp r0, #1
	bne _0800D170
	adds r0, r6, #0
	movs r1, #1
	bl sub_800EEF0
	b _0800D17C
	.align 2, 0
_0800D168: .4byte gUnknown_030012D8
_0800D16C: .4byte gStaticData_0816BBC4
_0800D170:
	adds r0, r6, #0
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_800E7A8
_0800D17C:
	add sp, #0x20
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
.endif
