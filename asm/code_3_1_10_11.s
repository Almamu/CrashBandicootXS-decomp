.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8006600 is reconstructed (but not yet byte-matching) as C in
@ src/oam_count.c, guarded by #if NON_MATCHING - this raw version is
@ only assembled for the default (matching) build. See docs/graphics.md,
@ "Parked, not matched: sub_8006600".
.if NON_MATCHING == 0
	thumb_func_start sub_8006600
sub_8006600: @ 0x08006600
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #0x10
	adds r4, r0, #0
	ldr r0, _080066F0 @ =gUnknown_03001300
	mov sb, r0
	ldr r0, [r0]
	bl sub_8006A90
	ldr r0, _080066F4 @ =gUnknown_030012FC
	ldr r0, [r0]
	bl sub_8006C28
	ldr r0, [r4, #0x18]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	ldr r1, _080066F8 @ =gUnknown_030012E0
	mov r8, r1
	ldr r0, [r1]
	movs r5, #0x98
	lsls r5, r5, #1
	adds r1, r0, r5
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r4, #0x10]
	ldr r2, [r2, #0x14]
	bl sub_803AD80
	movs r6, #0xf0
	subs r0, r6, r0
	lsrs r3, r0, #1
	mov r7, r8
	ldr r0, [r7]
	movs r2, #0x2d
	movs r7, #0x88
	lsls r7, r7, #1
	adds r1, r0, r7
	str r3, [r1]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r1, r0, r3
	str r2, [r1]
	adds r1, r0, r5
	ldr r2, [r1]
	movs r7, #0x20
	ldrsh r1, [r2, r7]
	adds r0, r0, r1
	ldr r1, [r4, #0x10]
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	mov r0, sp
	movs r1, #0x10
	movs r2, #0x6a
	bl sub_803AFE4
	mov r0, sp
	movs r1, #0xd0
	movs r2, #0x35
	bl sub_803AFDC
	ldr r0, [r4, #0x14]
	ldr r4, _080066FC @ =gUnknown_030012DC
	ldr r1, [r4]
	mov r2, sp
	movs r3, #0
	bl sub_8001214
	movs r0, #0x2e
	bl sub_8026F38
	mov r8, r0
	ldr r0, [r4]
	adds r1, r0, r5
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	mov r1, r8
	bl sub_803AD80
	subs r6, r6, r0
	lsrs r3, r6, #1
	ldr r0, [r4]
	movs r2, #0x90
	movs r4, #0x88
	lsls r4, r4, #1
	adds r1, r0, r4
	str r3, [r1]
	movs r7, #0x8a
	lsls r7, r7, #1
	adds r1, r0, r7
	str r2, [r1]
	adds r5, r0, r5
	ldr r2, [r5]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	mov r1, r8
	bl sub_803AD80
	mov r4, sb
	ldr r0, [r4]
	bl sub_8006A48
	add sp, #0x10
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080066F0: .4byte gUnknown_03001300
_080066F4: .4byte gUnknown_030012FC
_080066F8: .4byte gUnknown_030012E0
_080066FC: .4byte gUnknown_030012DC
.endif
