.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8025FC8
sub_8025FC8: @ 0x08025FC8
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r4, r0, #0
	adds r2, r1, #0
	adds r0, r2, #0
	cmp r2, #0
	bge _08025FD8
	adds r0, #0x1f
_08025FD8:
	asrs r0, r0, #5
	lsls r0, r0, #5
	subs r0, r2, r0
	lsls r1, r0, #6
	ldr r0, [r4, #0x4c]
	adds r6, r0, r1
	ldr r0, [r4, #0x2c]
	ldr r1, [r4, #0x44]
	mov r3, sp
	bl sub_8024B48
	adds r5, r0, #0
	ldr r3, [r4, #0x44]
	ldr r0, [r4, #0x48]
	cmp r3, r0
	bgt _08026024
	movs r7, #0x3f
	adds r4, r0, #0
_08025FFC:
	adds r0, r3, #0
	cmp r3, #0
	bge _08026004
	adds r0, #0x1f
_08026004:
	asrs r0, r0, #5
	lsls r0, r0, #5
	subs r0, r3, r0
	lsls r2, r0, #1
	adds r2, r2, r6
	ldr r1, [sp]
	lsls r0, r1, #1
	adds r0, r0, r5
	ldrh r0, [r0]
	strh r0, [r2]
	adds r1, #1
	ands r1, r7
	str r1, [sp]
	adds r3, #1
	cmp r3, r4
	ble _08025FFC
_08026024:
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_802602C
sub_802602C: @ 0x0802602C
	push {r4, r5, lr}
	adds r5, r0, #0
	ldr r1, [r5]
	adds r0, r1, #0
	cmp r1, #0
	bge _0802603A
	adds r0, r1, #7
_0802603A:
	asrs r0, r0, #3
	str r0, [r5, #0x44]
	adds r0, r1, #0
	adds r0, #0xef
	cmp r0, #0
	bge _08026048
	adds r0, #7
_08026048:
	asrs r0, r0, #3
	str r0, [r5, #0x48]
	ldr r2, [r5, #4]
	adds r0, r2, #0
	cmp r2, #0
	bge _08026056
	adds r0, r2, #7
_08026056:
	asrs r1, r0, #3
	str r1, [r5, #0x3c]
	adds r0, r2, #0
	adds r0, #0x9f
	cmp r0, #0
	bge _08026064
	adds r0, #7
_08026064:
	asrs r0, r0, #3
	str r0, [r5, #0x40]
	adds r4, r1, #0
	cmp r4, r0
	bgt _08026086
_0802606E:
	ldr r1, [r5, #0x30]
	movs r2, #0x30
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x34]
	adds r1, r4, #0
	bl sub_803AD80
	adds r4, #1
	ldr r0, [r5, #0x40]
	cmp r4, r0
	ble _0802606E
_08026086:
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_802608C
sub_802608C: @ 0x0802608C
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8024E90
	ldr r1, [r4, #0x30]
	movs r2, #0x28
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #0x2c]
	bl sub_803AD7C
	adds r0, r4, #0
	bl sub_802602C
	ldr r1, [r4, #0x38]
	ldrh r0, [r4, #0x34]
	strh r0, [r1]
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_80260B4
sub_80260B4: @ 0x080260B4
	push {lr}
	ldr r2, [r0, #0x50]
	adds r0, #0x34
	ldrb r0, [r0]
	lsls r1, r0, #0x1c
	lsrs r1, r1, #0x1e
	lsls r1, r1, #0xe
	movs r0, #0xc0
	lsls r0, r0, #0x13
	adds r1, r1, r0
	adds r0, r2, #0
	bl LoadTaggedAsset
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80260D4
sub_80260D4: @ 0x080260D4
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	bl sub_8024EB4
	adds r0, r4, #0
	adds r0, #0x28
	ldrb r0, [r0]
	cmp r0, #0
	beq _08026102
	ldr r0, [r5, #8]
	str r0, [r4, #0x50]
	adds r2, r4, #0
	adds r2, #0x34
	movs r1, #3
	ldrh r5, [r5, #0x14]
	ands r1, r5
	movs r0, #4
	rsbs r0, r0, #0
	ldrb r3, [r2]
	ands r0, r3
	orrs r0, r1
	strb r0, [r2]
_08026102:
	pop {r4, r5}
	pop {r0}
	bx r0
	thumb_func_start sub_8026108
sub_8026108: @ 0x08026108
	adds	r0, r2, #0
	cmp	r2, #0
	bge _08026110
	adds	r0, #31
_08026110:
	asrs	r0, r0, #5
	lsls	r0, r0, #5
	subs	r3, r2, r0
	adds	r0, r1, #0
	cmp	r1, #0
	bge _0802611E
	adds	r0, #31
_0802611E:
	asrs	r2, r0, #5
	lsls	r0, r2, #5
	subs	r2, r1, r0
	lsls	r0, r3, #5
	adds	r0, r0, r2
	bx	lr
	movs	r0, r0
	adds	r0, r1, #0
	cmp	r1, #0
	bge _08026134
	adds	r0, #31
_08026134:
	asrs	r0, r0, #5
	lsls	r0, r0, #5
	subs	r0, r1, r0
	bx	lr
	adds	r0, r1, #0

	non_word_aligned_thumb_func_start sub_802613E
sub_802613E: @ 0x0802613E
	cmp r1, #0
	bge _08026144
	adds r0, #0x1f
_08026144:
	asrs r0, r0, #5
	lsls r0, r0, #5
	subs r0, r1, r0
	bx lr

	thumb_func_start sub_802614C
sub_802614C: @ 0x0802614C
	adds r0, #0x35
	movs r2, #0x1f
	ands r1, r2
	movs r2, #0x20
	rsbs r2, r2, #0
	ldrb r3, [r0]
	ands r2, r3
	orrs r2, r1
	strb r2, [r0]
	bx lr

	thumb_func_start sub_8026160
sub_8026160: @ 0x08026160
	adds r0, #0x34
	movs r2, #3
	ands r1, r2
	movs r2, #4
	rsbs r2, r2, #0
	ldrb r3, [r0]
	ands r2, r3
	orrs r2, r1
	strb r2, [r0]
	bx lr

	thumb_func_start sub_8026174
sub_8026174: @ 0x08026174
	adds r0, #0x34
	lsls r1, r1, #7
	movs r2, #0x7f
	ldrb r3, [r0]
	ands r2, r3
	orrs r2, r1
	strb r2, [r0]
	bx lr

	thumb_func_start sub_8026184
sub_8026184: @ 0x08026184
	adds r0, #0x34
	ldrb r0, [r0]
	lsls r0, r0, #0x1c
	lsrs r0, r0, #0x1e
	bx lr
	.align 2, 0

	thumb_func_start sub_8026190
sub_8026190: @ 0x08026190
	adds r0, #0x34
	movs r2, #3
	ands r1, r2
	lsls r1, r1, #2
	movs r2, #0xd
	rsbs r2, r2, #0
	ldrb r3, [r0]
	ands r2, r3
	orrs r2, r1
	strb r2, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_80261A8
sub_80261A8: @ 0x080261A8
	ldr r1, [r0, #0x58]
	ldr r0, [r0, #0x54]
	str r0, [r1]
	bx lr

	thumb_func_start sub_80261B0
sub_80261B0: @ 0x080261B0
	ldr r1, [r0, #0x38]
	ldrh r0, [r0, #0x34]
	strh r0, [r1]
	bx lr

	thumb_func_start sub_80261B8
sub_80261B8: @ 0x080261B8
	push {lr}
	ldr r2, _080261C8 @ =gStaticData_087E4C14
	str r2, [r0, #0x30]
	bl sub_8024D74
	pop {r0}
	bx r0
	.align 2, 0
_080261C8: .4byte gStaticData_087E4C14

	thumb_func_start sub_80261CC
sub_80261CC: @ 0x080261CC
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r6, r0, #0
	adds r4, r1, #0
	ldr r0, [r6, #0x2c]
	ldr r2, [r6, #0x3c]
	mov r3, sp
	bl sub_8024B18
	adds r7, r0, #0
	ldr r2, [r6, #0x3c]
	adds r0, r2, #0
	cmp r2, #0
	bge _080261EA
	adds r0, #0x1f
_080261EA:
	asrs r0, r0, #5
	lsls r0, r0, #5
	subs r3, r2, r0
	adds r0, r4, #0
	cmp r4, #0
	bge _080261F8
	adds r0, #0x1f
_080261F8:
	asrs r1, r0, #5
	lsls r0, r1, #5
	subs r1, r4, r0
	lsls r0, r3, #5
	adds r4, r0, r1
	adds r5, r2, #0
	b _0802623C
_08026206:
	ldr r0, [r6, #0x5c]
	ldr r1, [sp]
	lsls r1, r1, #7
	adds r1, r1, r7
	ldrh r1, [r1]
	bl sub_80264F8
	ldr r2, [r6, #0x4c]
	lsls r1, r4, #1
	adds r1, r1, r2
	strh r0, [r1]
	ldr r0, [sp]
	adds r0, #1
	movs r1, #0x1f
	ands r0, r1
	str r0, [sp]
	adds r1, r4, #0
	adds r1, #0x20
	adds r0, r1, #0
	cmp r1, #0
	bge _08026234
	ldr r2, _0802624C @ =0x0000041F
	adds r0, r4, r2
_08026234:
	asrs r4, r0, #0xa
	lsls r0, r4, #0xa
	subs r4, r1, r0
	adds r5, #1
_0802623C:
	ldr r0, [r6, #0x40]
	cmp r5, r0
	ble _08026206
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802624C: .4byte 0x0000041F

	thumb_func_start sub_8026250
sub_8026250: @ 0x08026250
	adds r0, r1, #0
	movs r1, #8
	rsbs r1, r1, #0
	cmp r0, r1
	bge _0802625C
	adds r0, r1, #0
_0802625C:
	cmp r0, #8
	ble _08026262
	movs r0, #8
_08026262:
	bx lr

	thumb_func_start sub_8026264
sub_8026264: @ 0x08026264
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	ldr r0, [r5, #0x2c]
	ldr r2, [r5, #0x3c]
	mov r3, sp
	bl sub_8024B18
	adds r6, r0, #0
	ldr r4, [r5, #0x3c]
	b _08026294
_0802627A:
	ldr r0, [r5, #0x5c]
	ldr r1, [sp]
	lsls r1, r1, #7
	adds r1, r1, r6
	ldrh r1, [r1]
	bl sub_80265A0
	ldr r0, [sp]
	adds r0, #1
	movs r1, #0x1f
	ands r0, r1
	str r0, [sp]
	adds r4, #1
_08026294:
	ldr r0, [r5, #0x40]
	cmp r4, r0
	ble _0802627A
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80262A4
sub_80262A4: @ 0x080262A4
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r2, r1, #0
	ldr r0, [r5, #0x2c]
	ldr r1, [r5, #0x44]
	mov r3, sp
	bl sub_8024B48
	adds r6, r0, #0
	ldr r4, [r5, #0x44]
	b _080262D8
_080262BC:
	ldr r0, [r5, #0x5c]
	ldr r2, [sp]
	lsls r1, r2, #1
	adds r1, r1, r6
	ldrh r1, [r1]
	adds r2, #1
	str r2, [sp]
	bl sub_80265A0
	ldr r0, [sp]
	movs r1, #0x3f
	ands r0, r1
	str r0, [sp]
	adds r4, #1
_080262D8:
	ldr r0, [r5, #0x48]
	cmp r4, r0
	ble _080262BC
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80262E8
sub_80262E8: @ 0x080262E8
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	ldr r1, [r4, #0x44]
	cmp r1, r5
	bge _08026308
_080262F6:
	adds r0, r4, #0
	bl sub_8026264
	ldr r0, [r4, #0x44]
	adds r0, #1
	str r0, [r4, #0x44]
	adds r1, r0, #0
	cmp r1, r5
	blt _080262F6
_08026308:
	ldr r1, [r4, #0x48]
	cmp r1, r6
	ble _08026320
_0802630E:
	adds r0, r4, #0
	bl sub_8026264
	ldr r0, [r4, #0x48]
	subs r0, #1
	str r0, [r4, #0x48]
	adds r1, r0, #0
	cmp r1, r6
	bgt _0802630E
_08026320:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8026328
sub_8026328: @ 0x08026328
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	ldr r1, [r4, #0x3c]
	cmp r1, r5
	bge _08026348
_08026336:
	adds r0, r4, #0
	bl sub_80262A4
	ldr r0, [r4, #0x3c]
	adds r0, #1
	str r0, [r4, #0x3c]
	adds r1, r0, #0
	cmp r1, r5
	blt _08026336
_08026348:
	ldr r1, [r4, #0x40]
	cmp r1, r6
	ble _08026360
_0802634E:
	adds r0, r4, #0
	bl sub_80262A4
	ldr r0, [r4, #0x40]
	subs r0, #1
	str r0, [r4, #0x40]
	adds r1, r0, #0
	cmp r1, r6
	bgt _0802634E
_08026360:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8026368
sub_8026368: @ 0x08026368
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r6, r0, #0
	adds r2, r1, #0
	adds r0, r2, #0
	cmp r2, #0
	bge _0802637C
	adds r0, #0x1f
_0802637C:
	asrs r0, r0, #5
	lsls r0, r0, #5
	subs r0, r2, r0
	lsls r1, r0, #6
	ldr r0, [r6, #0x4c]
	adds r0, r0, r1
	mov r8, r0
	ldr r0, [r6, #0x2c]
	ldr r1, [r6, #0x44]
	mov r3, sp
	bl sub_8024B48
	adds r7, r0, #0
	ldr r4, [r6, #0x44]
	b _080263CA
_0802639A:
	adds r0, r4, #0
	cmp r4, #0
	bge _080263A2
	adds r0, #0x1f
_080263A2:
	asrs r5, r0, #5
	lsls r0, r5, #5
	subs r5, r4, r0
	ldr r0, [r6, #0x5c]
	ldr r2, [sp]
	lsls r1, r2, #1
	adds r1, r1, r7
	ldrh r1, [r1]
	adds r2, #1
	str r2, [sp]
	bl sub_80264F8
	lsls r1, r5, #1
	add r1, r8
	strh r0, [r1]
	ldr r0, [sp]
	movs r1, #0x3f
	ands r0, r1
	str r0, [sp]
	adds r4, #1
_080263CA:
	ldr r0, [r6, #0x48]
	cmp r4, r0
	ble _0802639A
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_80263DC
sub_80263DC: @ 0x080263DC
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, [r4, #0x5c]
	bl sub_802648C
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_802608C
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80263F8
sub_80263F8: @ 0x080263F8
	push {lr}
	ldr r2, [r0, #0x50]
	ldr r3, [r0, #0x5c]
	adds r0, #0x34
	ldrb r0, [r0]
	lsls r1, r0, #0x1c
	lsrs r1, r1, #0x1e
	adds r2, #4
	adds r0, r3, #0
	bl sub_8026618
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start nullsub_26
nullsub_26: @ 0x08026414
	bx lr
	.align 2, 0
