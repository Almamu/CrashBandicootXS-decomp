.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8010B6C is reconstructed (but not yet byte-matching) as C in
@ src/system/game_loop28.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-14-0x08010a0c-graphics.md.
.if NON_MATCHING == 0
	thumb_func_start sub_8010B6C
sub_8010B6C: @ 0x08010B6C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x68
	adds r7, r0, #0
	ldr r1, [r7]
	cmp r1, #0
	bne _08010B82
	b _08010D42
_08010B82:
	ldr r0, _08010C94 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r2, [r0]
	str r2, [sp, #0x24]
	ldr r0, [r0, #4]
	str r0, [sp, #0x28]
	movs r3, #0
	str r3, [sp, #0x1c]
	ldr r0, [r7, #8]
	ldr r4, [r0]
	ldr r0, [r0, #4]
	mov sb, r0
	subs r4, r4, r2
	str r4, [sp, #0x20]
	cmp r4, #0
	bge _08010BA6
	rsbs r4, r4, #0
	str r4, [sp, #0x20]
_08010BA6:
	mov r5, sb
	ldr r0, [sp, #0x28]
	subs r5, r5, r0
	mov sb, r5
	cmp r5, #0
	bge _08010BB6
	rsbs r5, r5, #0
	mov sb, r5
_08010BB6:
	movs r2, #0
	mov ip, r2
	movs r3, #1
	str r3, [sp, #0x2c]
	adds r4, r7, #0
	adds r4, #8
	str r4, [sp, #0x34]
	adds r5, r7, #0
	adds r5, #0x14
	str r5, [sp, #0x40]
	adds r0, r7, #0
	adds r0, #0x18
	str r0, [sp, #0x44]
	adds r2, r7, #0
	adds r2, #0x1c
	str r2, [sp, #0x48]
	adds r3, r7, #0
	adds r3, #0x20
	str r3, [sp, #0x4c]
	adds r4, #0x1c
	str r4, [sp, #0x50]
	mov r5, sp
	adds r5, #0x10
	str r5, [sp, #0x30]
	mov r0, sp
	adds r0, #0x14
	str r0, [sp, #0x38]
	mov r2, sp
	adds r2, #0x18
	str r2, [sp, #0x3c]
	ldr r3, [sp, #0x2c]
	cmp r3, r1
	bge _08010CDE
	adds r4, #0x24
	str r4, [sp, #0x54]
	movs r5, #0x30
	adds r5, r5, r7
	mov r8, r5
	adds r0, r7, #0
	adds r0, #0x44
	str r0, [sp, #0x58]
	adds r1, r7, #0
	adds r1, #0x40
	str r1, [sp, #0x5c]
	adds r2, r7, #0
	adds r2, #0x3c
	str r2, [sp, #0x60]
	movs r3, #0x38
	adds r3, r3, r7
	mov sl, r3
	subs r4, #0x1c
	str r4, [sp, #0x64]
_08010C1E:
	ldr r5, [sp, #0x64]
	ldr r6, [r5]
	ldr r2, [r6]
	ldr r1, [r6, #4]
	ldr r0, [sp, #0x24]
	subs r2, r2, r0
	cmp r2, #0
	bge _08010C30
	rsbs r2, r2, #0
_08010C30:
	ldr r3, [sp, #0x28]
	subs r1, r1, r3
	cmp r1, #0
	bge _08010C3A
	rsbs r1, r1, #0
_08010C3A:
	mov r4, sb
	subs r0, r1, r4
	cmp r0, #0
	bge _08010C44
	rsbs r0, r0, #0
_08010C44:
	cmp r0, #8
	bgt _08010C50
	mov r5, sl
	ldr r0, [r5]
	cmp r0, #4
	bne _08010C98
_08010C50:
	mov r0, sl
	ldr r1, [r0]
	ldr r3, [sp, #0x60]
	ldr r2, [r3]
	ldr r4, [sp, #0x5c]
	ldr r3, [r4]
	ldr r5, [sp, #0x58]
	ldr r0, [r5]
	str r0, [sp]
	mov r0, r8
	ldr r4, [r0]
	ldr r5, [r0, #4]
	str r4, [sp, #4]
	str r5, [sp, #8]
	ldr r4, [sp, #0x54]
	ldr r0, [r4]
	str r0, [sp, #0xc]
	mov r5, r8
	ldrb r0, [r5, #0x1c]
	ldr r4, [sp, #0x30]
	strb r0, [r4]
	ldrb r0, [r5, #0x1d]
	ldr r5, [sp, #0x38]
	strb r0, [r5]
	movs r0, #0
	ldr r4, [sp, #0x3c]
	strb r0, [r4]
	adds r0, r6, #0
	bl sub_800E08C
	movs r5, #1
	mov ip, r5
	b _08010CAE
	.align 2, 0
_08010C94: .4byte gUnknown_030012D8
_08010C98:
	cmp r1, sb
	blt _08010CA6
	cmp r1, sb
	bne _08010CAE
	ldr r0, [sp, #0x20]
	cmp r2, r0
	bge _08010CAE
_08010CA6:
	str r2, [sp, #0x20]
	mov sb, r1
	ldr r1, [sp, #0x2c]
	str r1, [sp, #0x1c]
_08010CAE:
	ldr r2, [sp, #0x54]
	adds r2, #0x24
	str r2, [sp, #0x54]
	movs r3, #0x24
	add r8, r3
	ldr r4, [sp, #0x58]
	adds r4, #0x24
	str r4, [sp, #0x58]
	ldr r5, [sp, #0x5c]
	adds r5, #0x24
	str r5, [sp, #0x5c]
	ldr r0, [sp, #0x60]
	adds r0, #0x24
	str r0, [sp, #0x60]
	add sl, r3
	ldr r1, [sp, #0x64]
	adds r1, #0x24
	str r1, [sp, #0x64]
	ldr r2, [sp, #0x2c]
	adds r2, #1
	str r2, [sp, #0x2c]
	ldr r0, [r7]
	cmp r2, r0
	blt _08010C1E
_08010CDE:
	ldr r3, [sp, #0x1c]
	lsls r6, r3, #3
	adds r6, r6, r3
	lsls r6, r6, #2
	ldr r4, [sp, #0x34]
	adds r0, r4, r6
	ldr r0, [r0]
	ldr r5, [sp, #0x40]
	adds r1, r5, r6
	ldr r1, [r1]
	ldr r3, [sp, #0x44]
	adds r2, r3, r6
	ldr r2, [r2]
	ldr r4, [sp, #0x48]
	adds r3, r4, r6
	ldr r3, [r3]
	ldr r5, [sp, #0x4c]
	adds r4, r5, r6
	ldr r4, [r4]
	str r4, [sp]
	adds r4, r6, r7
	mov r8, r4
	mov r5, r8
	ldr r4, [r5, #0xc]
	ldr r5, [r5, #0x10]
	str r4, [sp, #4]
	str r5, [sp, #8]
	ldr r4, [sp, #0x50]
	adds r6, r4, r6
	ldr r4, [r6]
	str r4, [sp, #0xc]
	mov r4, r8
	adds r4, #0x28
	ldrb r4, [r4]
	ldr r5, [sp, #0x30]
	strb r4, [r5]
	movs r4, #0x29
	add r8, r4
	mov r5, r8
	ldrb r4, [r5]
	ldr r5, [sp, #0x38]
	strb r4, [r5]
	mov r5, ip
	ldr r4, [sp, #0x3c]
	strb r5, [r4]
	bl sub_800E08C
	movs r0, #0
	str r0, [r7]
	strb r0, [r7, #4]
_08010D42:
	add sp, #0x68
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
.endif
