.include "asm/macros.inc"

.syntax unified
.arm


	thumb_func_start sub_8021280
sub_8021280: @ 0x08021280
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #0xc
	adds r6, r0, #0
	lsls r1, r1, #0x10
	lsrs r7, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	mov r8, r2
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov sb, r3
	ldr r5, _08021300 @ =gUnknown_030012C0
	ldr r0, [r5]
	bl sub_8023290
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0802130C
	ldr r0, [r5]
	bl sub_80232B8
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0802130C
	ldr r0, [r5]
	bl sub_8023324
	cmp r0, #0
	bne _0802130C
	ldr r4, _08021304 @ =gStaticData_0816C86C
	ldr r0, [r5]
	bl sub_802332C
	lsls r1, r0, #3
	adds r1, r1, r0
	lsls r1, r1, #2
	adds r4, #4
	adds r1, r1, r4
	ldr r0, [r1]
	cmp r0, #0
	bne _0802130C
	lsls r0, r6, #0x10
	lsrs r0, r0, #0x10
	adds r1, r7, #0
	mov r2, r8
	mov r3, sb
	bl sub_80071E4
	adds r4, r0, #0
	movs r1, #0x64
	movs r2, #0x64
	bl sub_80070EC
	movs r0, #0x12
	strb r0, [r4, #0xa]
	ldr r0, _08021308 @ =gUnknown_030012E8
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	b _08021376
	.align 2, 0
_08021300: .4byte gUnknown_030012C0
_08021304: .4byte gStaticData_0816C86C
_08021308: .4byte gUnknown_030012E8
_0802130C:
	ldr r0, _08021348 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #0
	bne _08021350
	lsls r0, r6, #0x10
	lsrs r0, r0, #0x10
	movs r1, #4
	str r1, [sp]
	adds r1, r7, #0
	mov r2, r8
	mov r3, sb
	bl sub_801A878
	ldr r1, [r0]
	asrs r1, r1, #8
	subs r2, r1, #2
	ldr r0, [r0, #4]
	asrs r0, r0, #8
	adds r3, r0, #0
	subs r3, #0x1e
	str r2, [sp, #4]
	str r3, [sp, #8]
	ldr r0, _0802134C @ =gUnknown_030012C0
	ldr r0, [r0]
	add r1, sp, #4
	bl sub_8023500
	b _08021376
	.align 2, 0
_08021348: .4byte gUnknown_030012D8
_0802134C: .4byte gUnknown_030012C0
_08021350:
	lsls r0, r6, #0x10
	lsrs r0, r0, #0x10
	adds r1, r7, #0
	mov r2, r8
	mov r3, sb
	bl sub_80071E4
	adds r4, r0, #0
	movs r1, #0x28
	movs r2, #0x28
	bl sub_80070EC
	movs r0, #0x12
	strb r0, [r4, #0xa]
	ldr r0, _08021384 @ =gUnknown_030012E8
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
_08021376:
	add sp, #0xc
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08021384: .4byte gUnknown_030012E8

	thumb_func_start sub_8021388
sub_8021388: @ 0x08021388
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	mov r8, r1
	mov sb, r2
	adds r4, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	mov r8, r1
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	mov sb, r2
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r4, #0
	bl sub_8009ED0
	adds r6, r0, #0
	ldr r0, _08021470 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r3, #0xa2
	lsls r3, r3, #2
	adds r0, r0, r3
	str r0, [r6, #0x20]
	adds r0, r6, #0
	bl sub_800815C
	adds r2, r6, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x10
	ldrb r1, [r6, #0xc]
	orrs r0, r1
	strb r0, [r6, #0xc]
	movs r0, #1
	movs r5, #1
	strb r0, [r6, #0xa]
	ldr r0, _08021474 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r4, r4, #1
	adds r4, r4, r0
	ldr r2, [r1, #0xc]
	ldrh r4, [r4]
	adds r2, r4, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r5
	ands r0, r5
	adds r3, r6, #0
	adds r3, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r4, [r3]
	ands r1, r4
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r5
	ands r0, r5
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _08021478 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r6, #0
	bl sub_8008E94
	movs r0, #0x30
	bl sub_8026EDC
	mov r1, r8
	mov r2, sb
	bl sub_801A838
	adds r4, r0, #0
	str r4, [r6, #0x44]
	ldr r1, [r4, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x1c]
	adds r1, r6, #0
	bl sub_803AD80
	ldr r0, _0802147C @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8023318
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08021470: .4byte gUnknown_030012D0
_08021474: .4byte gUnknown_030012B4
_08021478: .4byte gUnknown_030012F0
_0802147C: .4byte gUnknown_030012C0

	thumb_func_start sub_8021480
sub_8021480: @ 0x08021480
	push {r4, r5, r6, r7, lr}
	adds r4, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r4, #0
	bl sub_8009ED0
	adds r5, r0, #0
	ldr r0, _0802154C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xa5
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r5, #0x20]
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x4c
	bl sub_8026EDC
	bl sub_80189EC
	adds r6, r0, #0
	str r6, [r5, #0x44]
	ldr r1, [r6, #0xc]
	movs r7, #0x18
	ldrsh r0, [r1, r7]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	ldr r0, _08021550 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r4, r4, #1
	adds r4, r4, r0
	ldr r3, [r1, #0xc]
	ldrh r4, [r4]
	adds r3, r4, r3
	ldrb r1, [r3]
	lsrs r0, r1, #1
	movs r2, #1
	eors r0, r2
	ands r0, r2
	adds r4, r5, #0
	adds r4, #0x28
	ands r0, r2
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r4]
	ands r1, r7
	orrs r1, r0
	strb r1, [r4]
	ldrb r3, [r3]
	lsrs r0, r3, #2
	ands r0, r2
	ands r0, r2
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r4]
	movs r0, #0x10
	ldrb r1, [r5, #0xc]
	orrs r0, r1
	strb r0, [r5, #0xc]
	ldr r0, _08021554 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	ldr r0, _08021558 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r1, r6, #0
	bl sub_8023318
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802154C: .4byte gUnknown_030012D0
_08021550: .4byte gUnknown_030012B4
_08021554: .4byte gUnknown_030012F0
_08021558: .4byte gUnknown_030012C0

	thumb_func_start sub_802155C
sub_802155C: @ 0x0802155C
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	adds r5, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r5, #0
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _08021658 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x9f
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x20]
	movs r0, #1
	adds r1, r4, #0
	adds r1, #0x2d
	movs r2, #0
	mov sb, r2
	movs r6, #1
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x24
	bl sub_8026EDC
	bl sub_80197DC
	mov r8, r0
	str r0, [r4, #0x44]
	ldr r1, [r0, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	add r0, r8
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	ldr r0, _0802165C @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r5, r5, #1
	adds r5, r5, r0
	ldr r2, [r1, #0xc]
	ldrh r5, [r5]
	adds r2, r5, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r6
	ands r0, r6
	adds r3, r4, #0
	adds r3, #0x28
	ands r0, r6
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r5, [r3]
	ands r1, r5
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r6
	ands r0, r6
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	movs r0, #0x10
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _08021660 @ =gUnknown_030012F4
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	adds r4, #0x2c
	mov r2, sb
	strb r2, [r4]
	ldr r0, _08021664 @ =gUnknown_030012C0
	ldr r0, [r0]
	mov r1, r8
	bl sub_8023318
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08021658: .4byte gUnknown_030012D0
_0802165C: .4byte gUnknown_030012B4
_08021660: .4byte gUnknown_030012F4
_08021664: .4byte gUnknown_030012C0
