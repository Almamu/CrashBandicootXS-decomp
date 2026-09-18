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

	thumb_func_start sub_8021668
sub_8021668: @ 0x08021668
	push {r4, r5, r6, lr}
	adds r4, r1, #0
	adds r5, r2, #0
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_8009ED0
	adds r6, r0, #0
	ldr r0, _0802173C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xb4
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r6, #0x20]
	lsls r4, r4, #8
	str r4, [r6]
	lsls r5, r5, #8
	str r5, [r6, #4]
	movs r0, #0
	adds r1, r6, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r6, #0
	bl sub_80087C0
	adds r0, r6, #0
	bl sub_80087B4
	adds r0, r6, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, _08021740 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r1, [r6, #0x20]
	ldr r1, [r1]
	ldrb r1, [r1, #0x14]
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
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
	subs r2, #1
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r1, [r2]
	ands r0, r1
	movs r1, #0x21
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r2]
	movs r0, #0x24
	bl sub_8026EDC
	bl sub_8017FE8
	str r0, [r6, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x1c]
	adds r1, r6, #0
	bl sub_803AD80
	movs r0, #1
	strb r0, [r6, #0xa]
	movs r0, #0x7f
	ldrb r1, [r6, #0xc]
	ands r0, r1
	movs r1, #5
	rsbs r1, r1, #0
	ands r0, r1
	subs r1, #0x3c
	ands r0, r1
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r6, #0xc]
	ldr r0, _08021744 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r6, #0
	bl sub_8008E94
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802173C: .4byte gUnknown_030012D0
_08021740: .4byte gUnknown_030012B8
_08021744: .4byte gUnknown_030012F0

	thumb_func_start sub_8021748
sub_8021748: @ 0x08021748
	push {r4, r5, lr}
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _080217C8 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x87
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x20]
	movs r5, #0
	adds r0, r4, #0
	adds r0, #0x2d
	strb r5, [r0]
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
	movs r0, #0x7f
	ldrb r1, [r4, #0xc]
	ands r0, r1
	movs r1, #5
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r4, #0xc]
	strb r5, [r4, #0xa]
	ldr r0, _080217CC @ =gUnknown_030012F8
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080217C8: .4byte gUnknown_030012D0
_080217CC: .4byte gUnknown_030012F8

	thumb_func_start sub_80217D0
sub_80217D0: @ 0x080217D0
	push {r4, lr}
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _08021834 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x87
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x20]
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
	movs r0, #0x7f
	ldrb r1, [r4, #0xc]
	ands r0, r1
	movs r1, #5
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r4, #0xc]
	movs r0, #0
	strb r0, [r4, #0xa]
	ldr r0, _08021838 @ =gUnknown_030012F8
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08021834: .4byte gUnknown_030012D0
_08021838: .4byte gUnknown_030012F8

	thumb_func_start sub_802183C
sub_802183C: @ 0x0802183C
	push {r4, r5, lr}
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _080218BC @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x84
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x20]
	movs r5, #0
	adds r0, r4, #0
	adds r0, #0x2d
	strb r5, [r0]
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
	movs r0, #0x7f
	ldrb r1, [r4, #0xc]
	ands r0, r1
	movs r1, #5
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r4, #0xc]
	strb r5, [r4, #0xa]
	ldr r0, _080218C0 @ =gUnknown_030012F8
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080218BC: .4byte gUnknown_030012D0
_080218C0: .4byte gUnknown_030012F8

	thumb_func_start sub_80218C4
sub_80218C4: @ 0x080218C4
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #8
	str r4, [sp]
	bl sub_801A878
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_80218E8
sub_80218E8: @ 0x080218E8
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #6
	str r4, [sp]
	bl sub_801A878
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_802190C
sub_802190C: @ 0x0802190C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r5, r0, #0
	lsls r1, r1, #0x10
	lsrs r6, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r7, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	ldr r4, _08021944 @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_80232A0
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0802193C
	ldr r0, [r4]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _08021948
_0802193C:
	lsls r0, r5, #0x10
	lsrs r0, r0, #0x10
	movs r1, #7
	b _0802194E
	.align 2, 0
_08021944: .4byte gUnknown_030012C0
_08021948:
	lsls r0, r5, #0x10
	lsrs r0, r0, #0x10
	movs r1, #5
_0802194E:
	str r1, [sp]
	adds r1, r6, #0
	adds r2, r7, #0
	mov r3, r8
	bl sub_801A878
	adds r1, r0, #0
	ldr r0, _08021970 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80234F4
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08021970: .4byte gUnknown_030012C0

	thumb_func_start sub_8021974
sub_8021974: @ 0x08021974
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #2
	str r4, [sp]
	bl sub_801A878
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021998
sub_8021998: @ 0x08021998
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #1
	str r4, [sp]
	bl sub_801A878
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_80219BC
sub_80219BC: @ 0x080219BC
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0
	str r4, [sp]
	bl sub_801A878
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_80219E0
sub_80219E0: @ 0x080219E0
	push {lr}
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_801B984
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start nullsub_21
nullsub_21: @ 0x080219FC
	bx lr
	.align 2, 0

	thumb_func_start sub_8021A00
sub_8021A00: @ 0x08021A00
	push {r4, r5, lr}
	adds r4, r1, #0
	adds r5, r2, #0
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	movs r0, #0x28
	bl sub_8026EDC
	bl sub_800CB40
	adds r1, r0, #0
	movs r2, #0
	ldr r0, _08021A44 @ =sub_801F680
	str r0, [r1, #0x1c]
	movs r0, #0x78
	str r0, [r1, #0x20]
	str r2, [r1, #0x24]
	lsls r4, r4, #8
	str r4, [r1]
	lsls r5, r5, #8
	str r5, [r1, #4]
	movs r0, #0x10
	ldrb r2, [r1, #0xc]
	orrs r0, r2
	strb r0, [r1, #0xc]
	ldr r0, _08021A48 @ =gUnknown_030012E8
	ldr r0, [r0]
	bl sub_8008E94
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08021A44: .4byte sub_801F680
_08021A48: .4byte gUnknown_030012E8

	thumb_func_start sub_8021A4C
sub_8021A4C: @ 0x08021A4C
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0x12
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021A70
sub_8021A70: @ 0x08021A70
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0x11
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021A94
sub_8021A94: @ 0x08021A94
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0x10
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021AB8
sub_8021AB8: @ 0x08021AB8
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0xf
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021ADC
sub_8021ADC: @ 0x08021ADC
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0xe
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021B00
sub_8021B00: @ 0x08021B00
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0xd
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021B24
sub_8021B24: @ 0x08021B24
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0xc
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021B48
sub_8021B48: @ 0x08021B48
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0xb
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021B6C
sub_8021B6C: @ 0x08021B6C
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0xa
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021B90
sub_8021B90: @ 0x08021B90
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #9
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021BB4
sub_8021BB4: @ 0x08021BB4
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #8
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021BD8
sub_8021BD8: @ 0x08021BD8
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #7
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

