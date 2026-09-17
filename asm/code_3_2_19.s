.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_800B3F0
sub_800B3F0: @ 0x0800B3F0
	push {r4, r5, r6, lr}
	mov r6, sl
	mov r5, sb
	mov r4, r8
	push {r4, r5, r6}
	adds r5, r0, #0
	mov sb, r1
	adds r6, r2, #0
	mov r8, r3
	mov r0, sb
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	mov sb, r0
	lsls r6, r6, #0x10
	lsrs r6, r6, #0x10
	mov r1, r8
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	mov r8, r1
	adds r0, r5, #0
	bl sub_800A6A4
	ldr r0, _0800B49C @ =gStaticData_087E3E04
	str r0, [r5, #0x18]
	movs r1, #0x84
	lsls r1, r1, #1
	adds r0, r5, r1
	bl sub_8010E2C
	movs r0, #0
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_8008434
	adds r4, r0, #0
	adds r0, r5, #0
	adds r0, #0xb0
	str r4, [r0]
	ldr r0, _0800B4A0 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xcc
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	movs r0, #0
	mov sl, r0
	adds r0, r4, #0
	adds r0, #0x2d
	mov r1, sl
	strb r1, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r5, #0
	adds r0, #0xb4
	mov r1, sl
	str r1, [r0]
	adds r0, r5, #0
	bl sub_800A734
	mov r0, sb
	strh r0, [r5, #8]
	lsls r6, r6, #8
	str r6, [r5]
	mov r1, r8
	lsls r1, r1, #8
	str r1, [r5, #4]
	adds r0, r5, #0
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0800B49C: .4byte gStaticData_087E3E04
_0800B4A0: .4byte gUnknown_030012D0
