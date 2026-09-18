.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8006124/sub_800619C/sub_80061E8 are reconstructed (but not yet
@ byte-matching) as C in src/graphics/settings_menu9.c, guarded by
@ #if NON_MATCHING - this raw version is only assembled for the default
@ (matching) build. See docs/matching/issue-8-0x080060ac-overlay-ui.md.
.if NON_MATCHING == 0
	thumb_func_start sub_8006124
sub_8006124: @ 0x08006124
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	adds r0, #0x6c
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800613E
	adds r0, r6, #0
	adds r0, #0xbc
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
_0800613E:
	ldr r5, _08006194 @ =gUnknown_030012DC
	ldr r0, [r5]
	movs r4, #0x98
	lsls r4, r4, #1
	adds r1, r0, r4
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	adds r6, #0x7c
	ldr r2, [r2, #0x14]
	adds r1, r6, #0
	bl sub_803AD80
	ldr r1, _08006198 @ =gStaticData_0816B27C
	lsrs r0, r0, #1
	ldr r2, [r1]
	subs r2, r2, r0
	ldr r0, [r5]
	subs r2, #2
	ldr r3, [r1, #4]
	subs r3, #0x23
	movs r5, #0x88
	lsls r5, r5, #1
	adds r1, r0, r5
	str r2, [r1]
	movs r2, #0x8a
	lsls r2, r2, #1
	adds r1, r0, r2
	str r3, [r1]
	adds r4, r0, r4
	ldr r2, [r4]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	adds r1, r6, #0
	bl sub_803AD80
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08006194: .4byte gUnknown_030012DC
_08006198: .4byte gStaticData_0816B27C

	thumb_func_start sub_800619C
sub_800619C: @ 0x0800619C
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r0, #0x88
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	ldr r2, _080061E0 @ =gStaticData_0816B1E4
	ldr r0, _080061E4 @ =gUnknown_030012DC
	ldr r3, [r0]
	ldr r1, [r2]
	subs r1, #0x2c
	ldr r2, [r2, #4]
	subs r2, #8
	movs r5, #0x88
	lsls r5, r5, #1
	adds r0, r3, r5
	str r1, [r0]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r3, r1
	str r2, [r0]
	adds r1, r4, #0
	adds r1, #0x2c
	adds r2, r4, #0
	adds r2, #0x46
	adds r0, r4, #0
	bl sub_8005E5C
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080061E0: .4byte gStaticData_0816B1E4
_080061E4: .4byte gUnknown_030012DC

	thumb_func_start sub_80061E8
sub_80061E8: @ 0x080061E8
	push {r4, r5, r6, lr}
	ldr r1, _08006248 @ =gStaticData_0816B1D0
	ldr r0, [r0, #0x24]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	bl sub_8026F38
	adds r6, r0, #0
	ldr r5, _0800624C @ =gUnknown_030012DC
	ldr r0, [r5]
	movs r4, #0x98
	lsls r4, r4, #1
	adds r1, r0, r4
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	adds r1, r6, #0
	bl sub_803AD80
	lsrs r0, r0, #1
	movs r2, #0xc2
	subs r2, r2, r0
	ldr r0, [r5]
	movs r3, #0x2c
	movs r5, #0x88
	lsls r5, r5, #1
	adds r1, r0, r5
	str r2, [r1]
	movs r2, #0x8a
	lsls r2, r2, #1
	adds r1, r0, r2
	str r3, [r1]
	adds r4, r0, r4
	ldr r2, [r4]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	adds r1, r6, #0
	bl sub_803AD80
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08006248: .4byte gStaticData_0816B1D0
_0800624C: .4byte gUnknown_030012DC
.endif
