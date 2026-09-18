.include "asm/macros.inc"

.syntax unified
.arm

@ sub_800450C's semantics aren't confidently understood yet in
@ full (a large screen-init routine touching several still-raw
@ helpers and an unconfirmed triple-pointer-dereference table) -
@ left fully untouched rather than force a low-confidence
@ reconstruction. See the PR/issue for this chunk.
	thumb_func_start sub_800450C
sub_800450C: @ 0x0800450C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x14
	adds r7, r0, #0
	ldr r4, _080047D0 @ =gUnknown_03001300
	ldr r0, [r4]
	bl sub_8006A90
	ldr r0, [r4]
	bl sub_8006A48
	bl sub_80006A8
	ldr r0, [r4]
	bl sub_8006AAC
	ldr r4, _080047D4 @ =gUnknown_030012B8
	ldr r0, [r4]
	bl sub_8006EA8
	ldr r0, [r4]
	movs r1, #0
	bl sub_8006D50
	ldr r0, [r4]
	movs r1, #1
	bl sub_8006D50
	ldr r0, [r4]
	movs r1, #2
	bl sub_8006D50
	ldr r0, [r4]
	movs r1, #3
	bl sub_8006D50
	ldr r0, [r4]
	movs r1, #0
	mov r8, r1
	adds r2, r0, #0
	adds r2, #0x6c
	ldr r6, _080047D8 @ =gStaticData_0816B15A
	adds r1, r0, #0
	adds r1, #0x2c
	ldr r5, _080047DC @ =gStaticData_0816B13A
	ldr r4, _080047E0 @ =gStaticData_0816B19A
	ldr r3, _080047E4 @ =gStaticData_0816B17A
_08004570:
	ldrh r0, [r5]
	strh r0, [r1]
	ldrh r0, [r6]
	strh r0, [r1, #0x20]
	ldrh r0, [r3]
	strh r0, [r2]
	ldrh r0, [r4]
	strh r0, [r2, #0x20]
	adds r2, #2
	adds r6, #2
	adds r1, #2
	adds r5, #2
	adds r4, #2
	adds r3, #2
	movs r0, #1
	add r8, r0
	mov r0, r8
	cmp r0, #0xf
	ble _08004570
	ldr r5, _080047E8 @ =gUnknown_030012DC
	ldr r0, [r5]
	movs r1, #0
	bl sub_8028A30
	ldr r1, _080047EC @ =gUnknown_030012E0
	mov r8, r1
	ldr r0, [r1]
	movs r1, #0
	bl sub_8028A30
	ldr r6, _080047F0 @ =gUnknown_030012FC
	ldr r0, [r6]
	movs r4, #0
	str r4, [r0, #8]
	bl sub_8006C4C
	ldr r0, [r6]
	bl sub_8006C4C
	ldr r0, [r5]
	movs r2, #0x84
	lsls r2, r2, #1
	adds r1, r0, r2
	str r4, [r1]
	movs r3, #0x98
	lsls r3, r3, #1
	adds r1, r0, r3
	ldr r1, [r1]
	adds r1, #0x40
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r1, [r1, #4]
	bl sub_803AD7C
	ldr r0, [r6]
	ldr r1, [r5]
	movs r2, #0x96
	lsls r2, r2, #1
	adds r1, r1, r2
	ldr r1, [r1]
	lsls r1, r1, #5
	bl sub_8006C58
	ldr r0, [r5]
	movs r3, #0x96
	lsls r3, r3, #1
	adds r0, r0, r3
	ldr r2, [r0]
	mov r1, r8
	ldr r0, [r1]
	subs r3, #0x24
	adds r1, r0, r3
	str r2, [r1]
	movs r2, #0x98
	lsls r2, r2, #1
	adds r1, r0, r2
	ldr r1, [r1]
	adds r1, #0x40
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r1, [r1, #4]
	bl sub_803AD7C
	ldr r0, [r6]
	mov r2, r8
	ldr r1, [r2]
	movs r3, #0x96
	lsls r3, r3, #1
	adds r1, r1, r3
	ldr r1, [r1]
	lsls r1, r1, #5
	bl sub_8006C58
	ldr r0, [r6]
	bl sub_8006C30
	adds r0, r7, #0
	adds r0, #0xa8
	str r0, [sp]
	adds r1, r7, #0
	adds r1, #0xbc
	str r1, [sp, #4]
	adds r2, r7, #0
	adds r2, #0xd0
	str r2, [sp, #0x10]
	adds r3, r7, #0
	adds r3, #0xc0
	str r3, [sp, #8]
	adds r7, #0xc4
	str r7, [sp, #0xc]
	movs r0, #0xf
	mov sl, r0
	movs r1, #0x80
	mov sb, r1
	adds r7, r2, #0
	ldr r6, [sp, #4]
	ldr r5, [sp]
	movs r2, #4
	mov r8, r2
_08004662:
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	adds r4, r0, #0
	str r4, [r5]
	ldr r3, _080047F4 @ =gUnknown_030012D0
	ldr r0, [r3]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	movs r0, #1
	adds r1, r4, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r5]
	bl sub_800815C
	ldr r2, [r5]
	adds r2, #0x29
	mov r3, sl
	ands r0, r3
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldm r5!, {r0}
	mov r1, sb
	strh r1, [r0, #0x3c]
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	adds r4, r0, #0
	str r4, [r6]
	ldr r2, _080047F4 @ =gUnknown_030012D0
	ldr r0, [r2]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r3, #0xc6
	lsls r3, r3, #1
	adds r0, r0, r3
	str r0, [r4, #0x20]
	movs r0, #2
	adds r1, r4, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r6]
	bl sub_800815C
	ldr r2, [r6]
	adds r2, #0x29
	mov r1, sl
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldm r6!, {r0}
	mov r1, sb
	strh r1, [r0, #0x3c]
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	adds r4, r0, #0
	str r4, [r7]
	ldr r2, _080047F4 @ =gUnknown_030012D0
	ldr r0, [r2]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r3, #0xde
	lsls r3, r3, #1
	adds r0, r0, r3
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	movs r1, #0
	strb r1, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r7]
	bl sub_800815C
	ldr r2, [r7]
	adds r2, #0x29
	mov r3, sl
	ands r0, r3
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldm r7!, {r0}
	mov r1, sb
	strh r1, [r0, #0x3c]
	movs r2, #1
	rsbs r2, r2, #0
	add r8, r2
	mov r3, r8
	cmp r3, #0
	blt _0800477E
	b _08004662
_0800477E:
	ldr r0, [sp]
	ldr r1, [r0]
	movs r2, #0xa0
	lsls r2, r2, #5
	str r2, [r1]
	movs r0, #0xa0
	lsls r0, r0, #6
	str r0, [r1, #4]
	ldr r1, [sp, #4]
	ldr r0, [r1]
	str r2, [r0]
	movs r3, #0xa0
	lsls r3, r3, #7
	str r3, [r0, #4]
	ldr r0, [sp, #8]
	ldr r1, [r0]
	str r2, [r1]
	movs r0, #0xf0
	lsls r0, r0, #6
	str r0, [r1, #4]
	ldr r0, [sp, #0xc]
	ldr r1, [r0]
	str r2, [r1]
	movs r0, #0x96
	lsls r0, r0, #7
	str r0, [r1, #4]
	ldr r2, [sp, #0x10]
	ldr r1, [r2]
	movs r0, #0xf0
	lsls r0, r0, #7
	str r0, [r1]
	str r3, [r1, #4]
	add sp, #0x14
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080047D0: .4byte gUnknown_03001300
_080047D4: .4byte gUnknown_030012B8
_080047D8: .4byte gStaticData_0816B15A
_080047DC: .4byte gStaticData_0816B13A
_080047E0: .4byte gStaticData_0816B19A
_080047E4: .4byte gStaticData_0816B17A
_080047E8: .4byte gUnknown_030012DC
_080047EC: .4byte gUnknown_030012E0
_080047F0: .4byte gUnknown_030012FC
_080047F4: .4byte gUnknown_030012D0
