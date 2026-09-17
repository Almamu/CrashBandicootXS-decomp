.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8003A60 is reconstructed (semantics fully understood) but NOT YET
@ byte-matching - parked as C in src/graphics/settings_menu8c.c, guarded
@ by #if NON_MATCHING. See docs/matching/issue-5-overlay-ui-sync.md.
.if NON_MATCHING == 0
	thumb_func_start sub_8003A60
sub_8003A60: @ 0x08003A60
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov sb, r0
	movs r0, #0x64
	mov sl, r0
	movs r7, #0
	ldr r1, _08003A98 @ =gUnknown_030012DC
	mov r8, r1
_08003A78:
	mov r2, sb
	ldr r0, [r2, #0x10]
	cmp r7, r0
	bne _08003A9C
	mov r0, r8
	ldr r4, [r0]
	mov r0, sb
	bl sub_8004A50
	adds r1, r0, #0
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	adds r0, r4, #0
	bl sub_8028A30
	b _08003AA6
	.align 2, 0
_08003A98: .4byte gUnknown_030012DC
_08003A9C:
	mov r1, r8
	ldr r0, [r1]
	movs r1, #0
	bl sub_8028A30
_08003AA6:
	mov r2, r8
	ldr r4, [r2]
	movs r1, #0x98
	lsls r1, r1, #1
	adds r0, r4, r1
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x10
	movs r2, #0x10
	ldrsh r0, [r0, r2]
	adds r4, r4, r0
	ldr r1, _08003B3C @ =gStaticData_0816B1BC
	lsls r0, r7, #2
	adds r0, r0, r1
	ldr r6, [r0]
	adds r0, r6, #0
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	movs r1, #0xf0
	subs r1, r1, r0
	asrs r1, r1, #1
	mov r0, r8
	ldr r4, [r0]
	movs r2, #0x88
	lsls r2, r2, #1
	adds r0, r4, r2
	str r1, [r0]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r4, r1
	mov r2, sl
	str r2, [r0]
	adds r1, #0x1c
	adds r0, r4, r1
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r2, #0x20
	ldrsh r0, [r0, r2]
	adds r4, r4, r0
	adds r0, r6, #0
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	movs r0, #0xa
	add sl, r0
	adds r7, #1
	cmp r7, #4
	ble _08003A78
	mov r1, sp
	movs r0, #0
	strb r0, [r1]
	mov r0, sb
	movs r1, #0x5a
	movs r2, #0x21
	movs r3, #0
	bl sub_8003F30
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08003B3C: .4byte gStaticData_0816B1BC
.endif
