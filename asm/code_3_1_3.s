.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8000EE4 is reconstructed (but not yet byte-matching) as C in
@ src/text_layout.c, guarded by #if NON_MATCHING - this raw version is
@ only assembled for the default (matching) build. See docs/graphics.md,
@ "Parked, not matched: sub_8000EE4".
.if NON_MATCHING == 0
	thumb_func_start sub_8000EE4
sub_8000EE4: @ 0x08000EE4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x1c
	mov sl, r0
	mov r8, r1
	str r2, [sp]
	str r3, [sp, #4]
	movs r0, #0
	str r0, [sp, #0x10]
	ldr r1, [sp, #0x3c]
	cmp r1, #0
	beq _08000F10
	ldr r4, _08000F68 @ =gUnknown_03001300
	ldr r0, [r4]
	bl sub_8006A90
	ldr r0, [r4]
	bl sub_8006A48
_08000F10:
	ldr r2, [sp]
	ldr r0, [r2]
	ldr r2, [r2, #4]
	movs r3, #0x88
	lsls r3, r3, #1
	add r3, r8
	str r0, [r3]
	movs r1, #0x8a
	lsls r1, r1, #1
	add r1, r8
	str r2, [r1]
	movs r0, #0
	str r0, [sp, #8]
	movs r2, #0
	str r2, [sp, #0xc]
	mov r2, sl
	ldrb r0, [r2]
	str r1, [sp, #0x18]
	cmp r0, #0
	bne _08000F3A
	b _08001044
_08000F3A:
	ldr r0, [sp, #0x10]
	ldr r1, [sp, #4]
	cmp r0, r1
	blt _08000F44
	b _08001044
_08000F44:
	str r3, [sp, #0x14]
_08000F46:
	mov r0, sl
	bl GetWordLength
	adds r7, r0, #0
	mov r6, sl
	adds r2, r6, r7
	mov sl, r2
	ldrb r0, [r6]
	cmp r0, #0x2f
	bne _08000FA6
	adds r6, #1
	ldrb r0, [r6]
	cmp r0, #0x62
	beq _08000F6C
	cmp r0, #0x6e
	beq _08000F7E
	b _08000F9E
	.align 2, 0
_08000F68: .4byte gUnknown_03001300
_08000F6C:
	ldr r2, [sp, #0x14]
	ldr r1, [r2]
	ldr r2, [sp, #0x18]
	ldr r0, [r2]
	adds r0, #4
	ldr r2, [sp, #0x14]
	str r1, [r2]
	ldr r1, [sp, #0x18]
	str r0, [r1]
_08000F7E:
	movs r0, #0x98
	lsls r0, r0, #1
	add r0, r8
	ldr r1, [r0]
	movs r2, #0x38
	ldrsh r0, [r1, r2]
	add r0, r8
	ldr r2, [r1, #0x3c]
	movs r1, #0xa
	bl sub_803AD80
	movs r0, #0
	str r0, [sp, #8]
	ldr r1, [sp, #0xc]
	adds r1, #1
	str r1, [sp, #0xc]
_08000F9E:
	ldr r2, [sp, #0x10]
	adds r2, r2, r7
	str r2, [sp, #0x10]
	b _08001034
_08000FA6:
	movs r4, #0x98
	lsls r4, r4, #1
	add r4, r8
	ldr r1, [r4]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	add r0, r8
	ldr r3, [r1, #0x1c]
	adds r1, r6, #0
	adds r2, r7, #0
	bl sub_803AD84
	mov sb, r0
	ldr r5, [sp, #8]
	add r5, sb
	ldr r1, [sp]
	ldr r0, [r1, #8]
	cmp r5, r0
	bgt _08000FE8
	ldr r1, [r4]
	movs r2, #0x28
	ldrsh r0, [r1, r2]
	add r0, r8
	ldr r3, [r1, #0x2c]
	adds r1, r6, #0
	adds r2, r7, #0
	bl sub_803AD84
	str r5, [sp, #8]
	ldr r0, [sp, #0x3c]
	cmp r0, #1
	bne _0800102E
	b _08001022
_08000FE8:
	ldr r2, [sp, #0xc]
	adds r2, #1
	str r2, [sp, #0xc]
	ldr r0, [sp, #4]
	cmp r2, r0
	bge _08001034
	ldr r1, [r4]
	movs r2, #0x38
	ldrsh r0, [r1, r2]
	add r0, r8
	ldr r2, [r1, #0x3c]
	movs r1, #0xa
	bl sub_803AD80
	ldr r1, [r4]
	movs r2, #0x28
	ldrsh r0, [r1, r2]
	add r0, r8
	ldr r3, [r1, #0x2c]
	adds r1, r6, #0
	adds r2, r7, #0
	bl sub_803AD84
	mov r0, sb
	str r0, [sp, #8]
	ldr r0, [sp, #0x3c]
	subs r0, #1
	cmp r0, #1
	bhi _0800102E
_08001022:
	bl sub_80006A8
	ldr r0, _08001068 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
_0800102E:
	ldr r1, [sp, #0x10]
	adds r1, r1, r7
	str r1, [sp, #0x10]
_08001034:
	ldrb r0, [r6]
	cmp r0, #0
	beq _08001044
	ldr r2, [sp, #0xc]
	ldr r0, [sp, #4]
	cmp r2, r0
	bge _08001044
	b _08000F46
_08001044:
	ldr r1, [sp, #0x3c]
	cmp r1, #0
	beq _08001056
	bl sub_80006A8
	ldr r0, _08001068 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
_08001056:
	ldr r0, [sp, #0x10]
	add sp, #0x1c
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08001068: .4byte gUnknown_03001300
.endif
