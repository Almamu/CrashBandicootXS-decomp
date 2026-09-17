.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_80231CC
sub_80231CC: @ 0x080231CC
	ldrb r0, [r0, #2]
	lsrs r0, r0, #7
	bx lr
	.align 2, 0

	thumb_func_start sub_80231D4
sub_80231D4: @ 0x080231D4
	movs r1, #0
	str r1, [r0, #0x70]
	bx lr
	.align 2, 0

	thumb_func_start sub_80231DC
sub_80231DC: @ 0x080231DC
	movs r1, #0
	str r1, [r0, #0x6c]
	bx lr
	.align 2, 0

	thumb_func_start sub_80231E4
sub_80231E4: @ 0x080231E4
	movs r1, #5
	str r1, [r0, #0x74]
	bx lr
	.align 2, 0

	thumb_func_start sub_80231EC
sub_80231EC: @ 0x080231EC
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	cmp r5, #3
	bne _08023208
	ldr r0, _08023204 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x12
	bl sub_80017BC
	b _08023218
	.align 2, 0
_08023204: .4byte gUnknown_030012BC
_08023208:
	ldr r0, [r4, #0x78]
	cmp r0, #3
	bne _08023218
	str r5, [r4, #0x78]
	adds r0, r4, #0
	adds r0, #0xc4
	bl sub_8024498
_08023218:
	str r5, [r4, #0x78]
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_8023220
sub_8023220: @ 0x08023220
	str r1, [r0, #0x74]
	bx lr

	thumb_func_start sub_8023224
sub_8023224: @ 0x08023224
	push {lr}
	ldr r1, [r0, #0x78]
	adds r1, #1
	bl sub_80231EC
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8023234
sub_8023234: @ 0x08023234
	push {lr}
	adds r1, r0, #0
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _08023252
	ldr r0, [r1, #0x74]
	subs r0, #1
	str r0, [r1, #0x74]
	cmp r0, #0
	blt _08023252
	ldr r0, _08023258 @ =gUnknown_03001318
	ldr r0, [r0]
	bl sub_80284A4
_08023252:
	pop {r0}
	bx r0
	.align 2, 0
_08023258: .4byte gUnknown_03001318

	thumb_func_start sub_802325C
sub_802325C: @ 0x0802325C
	ldr r0, [r0, #0x6c]
	bx lr

	thumb_func_start sub_8023260
sub_8023260: @ 0x08023260
	adds r0, #0x98
	ldr r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8023268
sub_8023268: @ 0x08023268
	adds r0, #0x94
	ldr r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8023270
sub_8023270: @ 0x08023270
	adds r0, #0x90
	ldr r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8023278
sub_8023278: @ 0x08023278
	adds r0, #0xa7
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8023280
sub_8023280: @ 0x08023280
	adds r0, #0xa7
	movs r1, #0
	strb r1, [r0]
	bx lr

	thumb_func_start sub_8023288
sub_8023288: @ 0x08023288
	adds r0, #0xa7
	movs r1, #1
	strb r1, [r0]
	bx lr

	thumb_func_start sub_8023290
sub_8023290: @ 0x08023290
	adds r0, #0xa6
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8023298
sub_8023298: @ 0x08023298
	adds r0, #0xa6
	movs r1, #0
	strb r1, [r0]
	bx lr

	thumb_func_start sub_80232A0
sub_80232A0: @ 0x080232A0
	adds r0, #0xa5
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_80232A8
sub_80232A8: @ 0x080232A8
	adds r0, #0xa5
	movs r1, #0
	strb r1, [r0]
	bx lr

	thumb_func_start sub_80232B0
sub_80232B0: @ 0x080232B0
	adds r0, #0xa5
	movs r1, #1
	strb r1, [r0]
	bx lr

	thumb_func_start sub_80232B8
sub_80232B8: @ 0x080232B8
	adds r0, #0xa4
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_80232C0
sub_80232C0: @ 0x080232C0
	adds r0, #0xa4
	movs r1, #0
	strb r1, [r0]
	bx lr

	thumb_func_start sub_80232C8
sub_80232C8: @ 0x080232C8
	adds r0, #0xa9
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_80232D0
sub_80232D0: @ 0x080232D0
	adds r0, #0xa9
	movs r1, #0
	strb r1, [r0]
	bx lr

	thumb_func_start sub_80232D8
sub_80232D8: @ 0x080232D8
	adds r0, #0x8c
	movs r1, #0
	strb r1, [r0]
	bx lr

	thumb_func_start sub_80232E0
sub_80232E0: @ 0x080232E0
	ldr r0, [r0, #0x7c]
	bx lr

	thumb_func_start sub_80232E4
sub_80232E4: @ 0x080232E4
	ldr r1, [r0, #0x7c]
	adds r1, #1
	str r1, [r0, #0x7c]
	bx lr

	thumb_func_start sub_80232EC
sub_80232EC: @ 0x080232EC
	movs r1, #0
	str r1, [r0, #0x7c]
	bx lr
	.align 2, 0

	thumb_func_start sub_80232F4
sub_80232F4: @ 0x080232F4
	adds r0, #0xa8
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_80232FC
sub_80232FC: @ 0x080232FC
	adds r0, #0xa8
	movs r1, #0
	strb r1, [r0]
	bx lr

	thumb_func_start sub_8023304
sub_8023304: @ 0x08023304
	push {r4, lr}
	adds r4, r0, #0
	bl sub_80232EC
	adds r4, #0xa8
	movs r0, #1
	strb r0, [r4]
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8023318
sub_8023318: @ 0x08023318
	movs r2, #0xe4
	lsls r2, r2, #1
	adds r0, r0, r2
	str r1, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8023324
sub_8023324: @ 0x08023324
	adds r0, #0xc8
	ldr r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_802332C
sub_802332C: @ 0x0802332C
	adds r0, #0xc4
	ldr r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8023334
sub_8023334: @ 0x08023334
	adds r0, #0xc4
	str r1, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_802333C
sub_802333C: @ 0x0802333C
	push {lr}
	adds r0, r1, #0
	bl sub_8024428
	pop {r1}
	bx r1

	thumb_func_start sub_8023348
sub_8023348: @ 0x08023348
	push {lr}
	adds r0, r1, #0
	bl sub_8024434
	pop {r1}
	bx r1

	thumb_func_start sub_8023354
sub_8023354: @ 0x08023354
	push {lr}
	adds r0, r1, #0
	bl sub_8024440
	pop {r1}
	bx r1

	thumb_func_start sub_8023360
sub_8023360: @ 0x08023360
	push {lr}
	adds r0, r1, #0
	bl sub_802444C
	pop {r1}
	bx r1

	thumb_func_start sub_802336C
sub_802336C: @ 0x0802336C
	push {lr}
	adds r0, r1, #0
	bl sub_8024458
	pop {r1}
	bx r1

	thumb_func_start sub_8023378
sub_8023378: @ 0x08023378
	push {lr}
	adds r1, r0, #0
	adds r0, #0xc4
	ldr r0, [r0]
	cmp r0, #0x15
	beq _08023398
	cmp r0, #0x15
	bgt _0802338E
	cmp r0, #0x14
	beq _0802339E
	b _080233AE
_0802338E:
	cmp r0, #0x16
	beq _0802339E
	cmp r0, #0x17
	beq _0802339E
	b _080233AE
_08023398:
	bl sub_8033880
	b _080233B0
_0802339E:
	movs r2, #0xe4
	lsls r2, r2, #1
	adds r0, r1, r2
	ldr r0, [r0]
	ldr r1, [r0, #0x10]
	movs r0, #3
	subs r0, r0, r1
	b _080233B0
_080233AE:
	movs r0, #0
_080233B0:
	pop {r1}
	bx r1

	thumb_func_start sub_80233B4
sub_80233B4: @ 0x080233B4
	adds r0, #0xc4
	ldr r0, [r0]
	subs r0, #0x14
	cmp r0, #4
	bhi _080233F0
	lsls r0, r0, #2
	ldr r1, _080233C8 @ =_080233CC
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080233C8: .4byte _080233CC
_080233CC: @ jump table
	.4byte _080233E0 @ case 0
	.4byte _080233E4 @ case 1
	.4byte _080233E8 @ case 2
	.4byte _080233EC @ case 3
	.4byte _080233F0 @ case 4
_080233E0:
	movs r0, #9
	b _080233F6
_080233E4:
	movs r0, #8
	b _080233F6
_080233E8:
	movs r0, #6
	b _080233F6
_080233EC:
	movs r0, #7
	b _080233F6
_080233F0:
	movs r0, #1
	rsbs r0, r0, #0
	b _080233F8
_080233F6:
	subs r0, #6
_080233F8:
	bx lr
	.align 2, 0

	thumb_func_start sub_80233FC
sub_80233FC: @ 0x080233FC
	lsls r1, r1, #2
	adds r1, #4
	adds r0, r0, r1
	bx lr

	thumb_func_start sub_8023404
sub_8023404: @ 0x08023404
	push {lr}
	adds r1, r0, #0
	adds r1, #0xc4
	ldr r1, [r1]
	bl sub_80233FC
	pop {r1}
	bx r1

	thumb_func_start sub_8023414
sub_8023414: @ 0x08023414
	ldr r0, [r0, #0x70]
	bx lr

	thumb_func_start sub_8023418
sub_8023418: @ 0x08023418
	adds r1, r0, #0
	adds r1, #0xc4
	ldr r1, [r1]
	lsls r1, r1, #2
	adds r0, r0, r1
	movs r1, #0xa8
	lsls r1, r1, #1
	adds r0, r0, r1
	ldrb r0, [r0]
	lsls r0, r0, #0x1f
	lsrs r0, r0, #0x1f
	bx lr

	thumb_func_start sub_8023430
sub_8023430: @ 0x08023430
	push {lr}
	adds r1, r0, #0
	ldr r0, [r1, #0x6c]
	adds r0, #1
	str r0, [r1, #0x6c]
	cmp r0, #0x63
	ble _08023454
	movs r0, #0
	str r0, [r1, #0x6c]
	ldr r0, [r1, #0x74]
	cmp r0, #0x62
	bgt _0802344C
	adds r0, #1
	str r0, [r1, #0x74]
_0802344C:
	ldr r0, _08023460 @ =gUnknown_03001318
	ldr r0, [r0]
	bl sub_80284A4
_08023454:
	ldr r0, _08023460 @ =gUnknown_03001318
	ldr r0, [r0]
	bl sub_80284D4
	pop {r0}
	bx r0
	.align 2, 0
_08023460: .4byte gUnknown_03001318

	thumb_func_start sub_8023464
sub_8023464: @ 0x08023464
	push {lr}
	adds r1, r0, #0
	ldr r0, [r1, #0x74]
	cmp r0, #0x62
	bgt _08023472
	adds r0, #1
	str r0, [r1, #0x74]
_08023472:
	ldr r0, _08023480 @ =gUnknown_03001318
	ldr r0, [r0]
	bl sub_80284A4
	pop {r0}
	bx r0
	.align 2, 0
_08023480: .4byte gUnknown_03001318

	thumb_func_start sub_8023484
sub_8023484: @ 0x08023484
	push {r4, lr}
	adds r4, r0, #0
	adds r0, #0xbc
	ldr r1, [r4, #0x70]
	ldr r0, [r0]
	cmp r1, r0
	bne _080234DC
	adds r0, r4, #0
	bl sub_80232B8
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _080234DC
	adds r0, r4, #0
	bl sub_8023290
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _080234DC
	adds r0, r4, #0
	adds r0, #0xdc
	ldr r0, [r0]
	ldr r0, [r0, #8]
	cmp r0, #3
	bne _080234C6
	adds r0, r4, #0
	bl sub_8023404
	movs r1, #2
	ldrb r2, [r0]
	orrs r1, r2
	strb r1, [r0]
	b _080234DC
_080234C6:
	ldr r0, _080234E4 @ =0x0000FFFF
	movs r3, #0xe0
	lsls r3, r3, #1
	adds r1, r4, r3
	ldrh r1, [r1]
	adds r3, #4
	adds r2, r4, r3
	ldrh r2, [r2]
	movs r3, #0
	bl sub_801EB04
_080234DC:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080234E4: .4byte 0x0000FFFF

	thumb_func_start sub_80234E8
sub_80234E8: @ 0x080234E8
	movs r2, #0xde
	lsls r2, r2, #1
	adds r0, r0, r2
	str r1, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_80234F4
sub_80234F4: @ 0x080234F4
	movs r2, #0xdc
	lsls r2, r2, #1
	adds r0, r0, r2
	str r1, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8023500
sub_8023500: @ 0x08023500
	movs r2, #0xe0
	lsls r2, r2, #1
	adds r0, r0, r2
	ldr r2, [r1, #4]
	ldr r1, [r1]
	str r1, [r0]
	str r2, [r0, #4]
	bx lr

	thumb_func_start sub_8023510
sub_8023510: @ 0x08023510
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8023290
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08023526
	adds r1, r4, #0
	adds r1, #0xa6
	movs r0, #1
	strb r0, [r1]
_08023526:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_802352C
sub_802352C: @ 0x0802352C
	push {r4, lr}
	adds r4, r0, #0
	bl sub_80232B8
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08023542
	adds r1, r4, #0
	adds r1, #0xa4
	movs r0, #1
	strb r0, [r1]
_08023542:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8023548
sub_8023548: @ 0x08023548
	push {lr}
	adds r2, r0, #0
	adds r0, #0xcc
	ldr r0, [r0]
	str r0, [r2, #0x70]
	adds r0, r2, #0
	adds r0, #0xd0
	ldrb r0, [r0]
	adds r1, r2, #0
	adds r1, #0xa9
	strb r0, [r1]
	adds r1, #0x3b
	adds r0, r2, #0
	movs r2, #0x68
	bl sub_800014C
	pop {r0}
	bx r0

	thumb_func_start sub_802356C
sub_802356C: @ 0x0802356C
	push {r4, r5, lr}
	adds r5, r0, #0
	adds r4, r2, #0
	adds r0, #0xe0
	strb r1, [r0]
	adds r0, r5, #0
	bl sub_8023414
	adds r1, r5, #0
	adds r1, #0xcc
	str r0, [r1]
	adds r0, r5, #0
	adds r0, #0xa9
	ldrb r1, [r0]
	adds r0, #0x27
	strb r1, [r0]
	adds r0, r5, #0
	bl sub_80232FC
	adds r0, r5, #0
	bl sub_80232EC
	adds r2, r5, #0
	adds r2, #0xd4
	ldr r0, [r4]
	ldr r1, [r4, #4]
	str r0, [r2]
	str r1, [r2, #4]
	ldr r0, _080235DC @ =gUnknown_030012B4
	ldr r4, [r0]
	movs r1, #0x84
	lsls r1, r1, #1
	adds r0, r4, r1
	adds r1, r4, #0
	adds r1, #8
	ldr r2, _080235E0 @ =0x04000040
	bl sub_803A94C
	movs r2, #0xc2
	lsls r2, r2, #2
	adds r0, r4, r2
	movs r2, #0x82
	lsls r2, r2, #2
	adds r1, r4, r2
	ldr r2, _080235E0 @ =0x04000040
	bl sub_803A94C
	adds r0, r5, #0
	adds r0, #0xe4
	adds r1, r5, #0
	movs r2, #0x68
	bl sub_800014C
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080235DC: .4byte gUnknown_030012B4
_080235E0: .4byte 0x04000040

	thumb_func_start sub_80235E4
sub_80235E4: @ 0x080235E4
	push {r4, lr}
	adds r4, r0, #0
	lsls r1, r1, #0x18
	cmp r1, #0
	beq _08023640
	adds r1, r4, #0
	adds r1, #0xb4
	ldr r0, [r4, #0x70]
	ldr r1, [r1]
	adds r0, r0, r1
	str r0, [r4, #0x70]
	adds r0, r4, #0
	bl sub_8023298
	adds r0, r4, #0
	bl sub_80232FC
	adds r0, r4, #0
	bl sub_8023288
	ldr r0, _08023638 @ =gUnknown_03001318
	ldr r0, [r0]
	adds r1, r4, #0
	adds r1, #0xbc
	ldr r1, [r1]
	bl sub_8028568
	ldr r0, _0802363C @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r2, r4, #0
	adds r2, #0xd4
	ldr r1, [r2]
	ldr r2, [r2, #4]
	bl sub_8007398
	adds r0, r4, #0
	adds r0, #0xe0
	ldrb r1, [r0]
	adds r0, r4, #0
	bl sub_8022CA0
	b _08023646
	.align 2, 0
_08023638: .4byte gUnknown_03001318
_0802363C: .4byte gUnknown_030012D8
_08023640:
	adds r0, r4, #0
	bl sub_80231D4
_08023646:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_802364C
sub_802364C: @ 0x0802364C
	push {lr}
	movs r1, #2
	bl sub_8022468
	pop {r0}
	bx r0

	thumb_func_start sub_8023658
sub_8023658: @ 0x08023658
	push {lr}
	movs r1, #1
	bl sub_8022468
	ldr r0, _08023670 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x5d
	bl sub_80019A8
	pop {r0}
	bx r0
	.align 2, 0
_08023670: .4byte gUnknown_030012BC

	thumb_func_start sub_8023674
sub_8023674: @ 0x08023674
	push {r4, lr}
	ldr r0, _08023698 @ =0x0000044C
	bl sub_8026EDC
	bl nullsub_7
	adds r4, r0, #0
	bl sub_80361B0
	cmp r4, #0
	beq _08023692
	adds r0, r4, #0
	movs r1, #3
	bl sub_8037154
_08023692:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08023698: .4byte 0x0000044C

	thumb_func_start sub_802369C
sub_802369C: @ 0x0802369C
	push {lr}
	movs r1, #0
	bl sub_8022468
	pop {r0}
	bx r0

	thumb_func_start nullsub_24
nullsub_24: @ 0x080236A8
	bx lr
	.align 2, 0

	thumb_func_start sub_80236AC
sub_80236AC: @ 0x080236AC
	push {r4, r5, lr}
	adds r4, r0, #0
	movs r0, #0xa6
	lsls r0, r0, #1
	adds r5, r4, r0
	adds r0, r4, #0
	movs r2, #0x68
	bl sub_800014C
	adds r0, r5, #0
	adds r1, r4, #0
	movs r2, #0x68
	bl sub_800014C
	ldrb r1, [r5]
	lsls r0, r1, #0x19
	lsrs r0, r0, #0x19
	str r0, [r4, #0x74]
	ldr r1, _080236E8 @ =0x0000014D
	adds r0, r4, r1
	ldrb r0, [r0]
	lsrs r0, r0, #1
	str r0, [r4, #0x6c]
	ldrh r5, [r5]
	lsls r0, r5, #0x17
	lsrs r0, r0, #0x1e
	str r0, [r4, #0x78]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080236E8: .4byte 0x0000014D

	thumb_func_start sub_80236EC
sub_80236EC: @ 0x080236EC
	push {r4, r5, lr}
	adds r3, r0, #0
	ldr r2, [r3, #0x74]
	movs r1, #0xa6
	lsls r1, r1, #1
	adds r0, r3, r1
	movs r1, #0x7f
	ands r2, r1
	movs r1, #0x80
	rsbs r1, r1, #0
	ldrb r4, [r0]
	ands r1, r4
	orrs r1, r2
	strb r1, [r0]
	ldr r2, [r3, #0x6c]
	ldr r5, _08023730 @ =0x0000014D
	lsls r2, r2, #1
	movs r1, #1
	ldrb r4, [r5, r3]
	ands r1, r4
	orrs r1, r2
	strb r1, [r5, r3]
	ldr r2, [r3, #0x78]
	movs r1, #3
	ands r2, r1
	lsls r2, r2, #7
	ldr r1, _08023734 @ =0xFFFFFE7F
	ldrh r5, [r0]
	ands r1, r5
	orrs r1, r2
	strh r1, [r0]
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08023730: .4byte 0x0000014D
_08023734: .4byte 0xFFFFFE7F

	thumb_func_start sub_8023738
sub_8023738: @ 0x08023738
	push {r4, lr}
	ldr r4, _08023758 @ =gUnknown_03000828
	ldr r0, [r4]
	cmp r0, #0
	bne _08023750
	movs r0, #0xe6
	lsls r0, r0, #1
	bl sub_8026EDC
	bl sub_8022230
	str r0, [r4]
_08023750:
	ldr r0, [r4]
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08023758: .4byte gUnknown_03000828

	thumb_func_start sub_802375C
sub_802375C: @ 0x0802375C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	mov r8, r0
	bl sub_8022208
	bl sub_8024198
	ldr r4, _0802383C @ =gUnknown_030012E8
	movs r0, #0x14
	bl sub_8026EDC
	movs r1, #0x20
	bl sub_8008EE4
	str r0, [r4]
	ldr r4, _08023840 @ =gUnknown_030012EC
	movs r0, #0x14
	bl sub_8026EDC
	movs r1, #0xc0
	bl sub_8008EE4
	str r0, [r4]
	ldr r4, _08023844 @ =gUnknown_0300130C
	ldr r0, _08023848 @ =0x00000818
	bl sub_8026EDC
	movs r1, #0xc0
	bl sub_8008F20
	str r0, [r4]
	ldr r4, _0802384C @ =gUnknown_030012F0
	movs r0, #0x14
	bl sub_8026EDC
	movs r1, #0x80
	bl sub_8008EE4
	str r0, [r4]
	ldr r4, _08023850 @ =gUnknown_030012F8
	movs r0, #0x14
	bl sub_8026EDC
	movs r1, #0x40
	bl sub_8008EE4
	str r0, [r4]
	ldr r4, _08023854 @ =gUnknown_030012F4
	movs r0, #0x14
	bl sub_8026EDC
	movs r1, #0x40
	bl sub_8008EE4
	str r0, [r4]
	ldr r4, _08023858 @ =gUnknown_030012D4
	movs r0, #0x18
	bl sub_8026EDC
	str r0, [r4]
	bl sub_80268AC
	ldr r1, _0802385C @ =gUnknown_03001308
	str r0, [r1]
	ldr r7, _08023860 @ =gUnknown_030012D8
	movs r0, #0xd4
	lsls r0, r0, #2
	bl sub_8026EDC
	ldr r1, _08023864 @ =0x0000FFFF
	movs r2, #0
	str r2, [sp]
	movs r3, #0
	bl sub_800B3F0
	str r0, [r7]
	mov r2, r8
	ldr r1, [r2, #0x10]
	ldr r2, [r2, #0x14]
	bl sub_8007398
	ldr r1, [r7]
	movs r0, #0x10
	ldrb r3, [r1, #0xc]
	orrs r0, r3
	strb r0, [r1, #0xc]
	ldr r2, [r7]
	adds r2, #0x28
	movs r1, #1
	mov r4, r8
	ldrb r4, [r4, #0x1c]
	ands r1, r4
	lsls r1, r1, #4
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r3, [r2]
	ands r0, r3
	orrs r0, r1
	strb r0, [r2]
	mov r4, r8
	ldr r0, [r4, #0x18]
	ldr r6, [r0, #8]
	cmp r6, #1
	beq _080238B0
	cmp r6, #1
	bgt _08023868
	cmp r6, #0
	beq _0802386E
	b _08023952
	.align 2, 0
_0802383C: .4byte gUnknown_030012E8
_08023840: .4byte gUnknown_030012EC
_08023844: .4byte gUnknown_0300130C
_08023848: .4byte 0x00000818
_0802384C: .4byte gUnknown_030012F0
_08023850: .4byte gUnknown_030012F8
_08023854: .4byte gUnknown_030012F4
_08023858: .4byte gUnknown_030012D4
_0802385C: .4byte gUnknown_03001308
_08023860: .4byte gUnknown_030012D8
_08023864: .4byte 0x0000FFFF
_08023868:
	cmp r6, #2
	beq _08023918
	b _08023952
_0802386E:
	movs r0, #0x38
	bl sub_8026EDC
	bl sub_801588C
	adds r4, r0, #0
	ldr r1, _080238A8 @ =gStaticData_0816B92C
	bl sub_800B69C
	ldr r0, [r7]
	adds r0, #0x88
	strb r6, [r0]
	ldr r0, _080238AC @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r1, [r7]
	str r0, [r1, #0x20]
	str r4, [r1, #0x44]
	ldr r2, [r4, #0xc]
	movs r3, #0x18
	ldrsh r0, [r2, r3]
	adds r4, r4, r0
	ldr r2, [r2, #0x1c]
	adds r0, r4, #0
	bl sub_803AD80
	b _08023952
	.align 2, 0
_080238A8: .4byte gStaticData_0816B92C
_080238AC: .4byte gUnknown_030012D0
_080238B0:
	ldr r5, _0802390C @ =gUnknown_03001310
	movs r0, #0x30
	bl sub_8026EDC
	bl sub_80174EC
	str r0, [r5]
	ldr r1, _08023910 @ =gStaticData_0816B934
	bl sub_800B69C
	ldr r0, [r7]
	adds r0, #0x88
	strb r6, [r0]
	ldr r0, _08023914 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0xc
	ldr r4, [r7]
	str r0, [r4, #0x20]
	movs r0, #0x1f
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
	ldr r1, [r7]
	ldr r0, [r5]
	str r0, [r1, #0x44]
	ldr r3, [r0, #0xc]
	movs r4, #0x18
	ldrsh r2, [r3, r4]
	adds r0, r0, r2
	ldr r2, [r3, #0x1c]
	bl sub_803AD80
	b _08023952
	.align 2, 0
_0802390C: .4byte gUnknown_03001310
_08023910: .4byte gStaticData_0816B934
_08023914: .4byte gUnknown_030012D0
_08023918:
	movs r0, #0x28
	bl sub_8026EDC
	bl sub_8017A00
	adds r4, r0, #0
	ldr r1, _080239F0 @ =gStaticData_0816B93C
	bl sub_800B69C
	ldr r0, [r7]
	movs r1, #3
	adds r0, #0x88
	strb r1, [r0]
	ldr r0, _080239F4 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x18
	ldr r1, [r7]
	str r0, [r1, #0x20]
	str r4, [r1, #0x44]
	ldr r2, [r4, #0xc]
	movs r3, #0x18
	ldrsh r0, [r2, r3]
	adds r4, r4, r0
	ldr r2, [r2, #0x1c]
	adds r0, r4, #0
	bl sub_803AD80
_08023952:
	mov r0, r8
	bl sub_8023A1C
	adds r4, r0, #0
	ldr r0, _080239F8 @ =gUnknown_03001308
	ldr r0, [r0]
	cmp r0, #0
	beq _08023968
	movs r1, #3
	bl sub_802680C
_08023968:
	ldr r0, _080239FC @ =gUnknown_030012D4
	ldr r0, [r0]
	bl sub_8026ED0
	ldr r0, _08023A00 @ =gUnknown_030012D8
	ldr r2, [r0]
	cmp r2, #0
	beq _0802398A
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0802398A:
	ldr r0, _08023A04 @ =gUnknown_030012F4
	ldr r0, [r0]
	cmp r0, #0
	beq _08023998
	movs r1, #3
	bl sub_8008EB4
_08023998:
	ldr r0, _08023A08 @ =gUnknown_030012F8
	ldr r0, [r0]
	cmp r0, #0
	beq _080239A6
	movs r1, #3
	bl sub_8008EB4
_080239A6:
	ldr r0, _08023A0C @ =gUnknown_030012F0
	ldr r0, [r0]
	cmp r0, #0
	beq _080239B4
	movs r1, #3
	bl sub_8008EB4
_080239B4:
	ldr r0, _08023A10 @ =gUnknown_0300130C
	ldr r0, [r0]
	cmp r0, #0
	beq _080239C2
	movs r1, #3
	bl sub_8009B9C
_080239C2:
	ldr r0, _08023A14 @ =gUnknown_030012EC
	ldr r0, [r0]
	cmp r0, #0
	beq _080239D0
	movs r1, #3
	bl sub_8008EB4
_080239D0:
	ldr r0, _08023A18 @ =gUnknown_030012E8
	ldr r0, [r0]
	cmp r0, #0
	beq _080239DE
	movs r1, #3
	bl sub_8008EB4
_080239DE:
	bl sub_80221F0
	adds r0, r4, #0
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080239F0: .4byte gStaticData_0816B93C
_080239F4: .4byte gUnknown_030012D0
_080239F8: .4byte gUnknown_03001308
_080239FC: .4byte gUnknown_030012D4
_08023A00: .4byte gUnknown_030012D8
_08023A04: .4byte gUnknown_030012F4
_08023A08: .4byte gUnknown_030012F8
_08023A0C: .4byte gUnknown_030012F0
_08023A10: .4byte gUnknown_0300130C
_08023A14: .4byte gUnknown_030012EC
_08023A18: .4byte gUnknown_030012E8

	thumb_func_start sub_8023A1C
sub_8023A1C: @ 0x08023A1C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x18
	adds r7, r0, #0
	movs r0, #1
	mov sl, r0
	ldr r4, _08023AC4 @ =gUnknown_030012D8
	ldr r0, [r4]
	bl sub_800A810
	ldr r0, _08023AC8 @ =gUnknown_030012D4
	ldr r1, [r0]
	ldr r0, [r4]
	str r0, [r1, #0x10]
	mov r2, sl
	str r2, [r1, #0x14]
	ldr r0, _08023ACC @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r1, [r7, #0x18]
	bl sub_80266BC
	ldr r5, _08023AD0 @ =gStaticData_0816C86C
	ldr r1, [r7]
	lsls r0, r1, #3
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r5
	ldrb r0, [r0, #0x1c]
	cmp r0, #0
	bne _08023A66
	ldr r0, _08023AD4 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023484
_08023A66:
	ldr r4, _08023AD4 @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_80232C8
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08023A78
	bl sub_800F1B8
_08023A78:
	ldr r0, [r4]
	ldr r2, [r7]
	lsls r1, r2, #3
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r2, r5, #0
	adds r2, #0x14
	adds r1, r1, r2
	ldr r1, [r1]
	bl sub_8023118
	ldr r0, [r4]
	ldr r2, [r7]
	lsls r1, r2, #3
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r2, r5, #0
	adds r2, #0x18
	adds r1, r1, r2
	ldr r1, [r1]
	bl sub_8023110
	ldr r1, [r7]
	lsls r0, r1, #3
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r1, r5, #4
	adds r0, r0, r1
	ldr r0, [r0]
	subs r0, #1
	cmp r0, #5
	bls _08023ABA
	b _08023BC4
_08023ABA:
	lsls r0, r0, #2
	ldr r1, _08023AD8 @ =_08023ADC
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08023AC4: .4byte gUnknown_030012D8
_08023AC8: .4byte gUnknown_030012D4
_08023ACC: .4byte gUnknown_03001308
_08023AD0: .4byte gStaticData_0816C86C
_08023AD4: .4byte gUnknown_030012C0
_08023AD8: .4byte _08023ADC
_08023ADC: @ jump table
	.4byte _08023B20 @ case 0
	.4byte _08023AF4 @ case 1
	.4byte _08023B60 @ case 2
	.4byte _08023BC4 @ case 3
	.4byte _08023B8C @ case 4
	.4byte _08023B20 @ case 5
_08023AF4:
	ldr r4, _08023B18 @ =gUnknown_030012C8
	ldr r0, [r4]
	bl sub_8027088
	ldr r0, [r4]
	movs r1, #0xa0
	lsls r1, r1, #0x13
	ldr r2, _08023B1C @ =gStaticData_0816C814
	movs r3, #5
	str r3, [sp]
	add r4, sp, #4
	movs r3, #1
	strb r3, [r4]
	movs r3, #6
	bl sub_8027018
	b _08023BCC
	.align 2, 0
_08023B18: .4byte gUnknown_030012C8
_08023B1C: .4byte gStaticData_0816C814
_08023B20:
	ldr r4, _08023B54 @ =gUnknown_030012C8
	ldr r0, [r4]
	bl sub_8027088
	ldr r0, [r4]
	movs r3, #0xa0
	lsls r3, r3, #0x13
	mov sb, r3
	ldr r2, _08023B58 @ =gStaticData_0816C81E
	movs r5, #9
	mov r8, r5
	str r5, [sp]
	add r6, sp, #4
	movs r5, #0
	strb r5, [r6]
	mov r1, sb
	movs r3, #0x10
	bl sub_8027018
	ldr r0, [r4]
	ldr r2, _08023B5C @ =gStaticData_0816C830
	mov r1, r8
	str r1, [sp]
	strb r5, [r6]
	mov r1, sb
	b _08023BA6
	.align 2, 0
_08023B54: .4byte gUnknown_030012C8
_08023B58: .4byte gStaticData_0816C81E
_08023B5C: .4byte gStaticData_0816C830
_08023B60:
	ldr r4, _08023B84 @ =gUnknown_030012C8
	ldr r0, [r4]
	bl sub_8027088
	ldr r0, [r4]
	movs r1, #0xa0
	lsls r1, r1, #0x13
	ldr r2, _08023B88 @ =gStaticData_0816C842
	movs r3, #0x10
	str r3, [sp]
	add r4, sp, #4
	movs r3, #0
	strb r3, [r4]
	movs r3, #0xa
	bl sub_8027018
	b _08023BCC
	.align 2, 0
_08023B84: .4byte gUnknown_030012C8
_08023B88: .4byte gStaticData_0816C842
_08023B8C:
	ldr r4, _08023BB0 @ =gUnknown_030012C8
	ldr r0, [r4]
	bl sub_8027088
	ldr r0, [r4]
	movs r1, #0xa0
	lsls r1, r1, #0x13
	ldr r2, _08023BB4 @ =gStaticData_0816C862
	movs r3, #5
	str r3, [sp]
	add r4, sp, #4
	movs r3, #0
	strb r3, [r4]
_08023BA6:
	movs r3, #0x14
	bl sub_8027018
	b _08023BCC
	.align 2, 0
_08023BB0: .4byte gUnknown_030012C8
_08023BB4: .4byte gStaticData_0816C862
_08023BB8:
	movs r2, #1
	mov sl, r2
	b _08023E72
_08023BBE:
	movs r3, #2
	mov sl, r3
	b _08023E72
_08023BC4:
	ldr r0, _08023D4C @ =gUnknown_030012C8
	ldr r1, [r0]
	movs r0, #0
	strb r0, [r1]
_08023BCC:
	adds r0, r7, #0
	bl sub_80240E4
	bl sub_802423C
	ldr r0, [r7, #0x18]
	ldr r0, [r0, #8]
	cmp r0, #1
	bne _08023C06
	ldr r0, _08023D50 @ =gUnknown_030012D8
	ldr r4, [r0]
	movs r0, #0x1f
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
	ldr r0, _08023D54 @ =gUnknown_030012D4
	ldr r1, [r0]
	movs r0, #2
	str r0, [r1, #0x14]
_08023C06:
	ldr r4, _08023D50 @ =gUnknown_030012D8
	ldr r0, [r4]
	bl sub_800815C
	ldr r2, [r4]
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r5, [r2]
	ands r1, r5
	orrs r1, r0
	strb r1, [r2]
	ldr r0, _08023D58 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r3, [r4]
	adds r1, r3, #0
	adds r1, #0x29
	ldrb r1, [r1]
	lsls r1, r1, #0x1c
	lsrs r1, r1, #0x1c
	ldr r2, [r3, #0x20]
	adds r3, #0x2d
	ldr r4, [r2]
	ldrb r5, [r3]
	lsls r2, r5, #3
	subs r2, r2, r5
	lsls r2, r2, #2
	adds r2, r2, r4
	ldrb r2, [r2, #0x14]
	bl sub_8006D08
	ldr r0, _08023D54 @ =gUnknown_030012D4
	ldr r0, [r0]
	bl sub_8026DFC
	ldr r0, _08023D5C @ =gUnknown_03001308
	ldr r0, [r0]
	bl sub_8026984
	ldr r0, [r7, #0x18]
	ldr r0, [r0, #8]
	cmp r0, #0
	bne _08023D0C
	ldr r4, _08023D60 @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_80232B8
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08023C7A
	adds r0, r7, #0
	bl sub_8024404
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08023C92
_08023C7A:
	ldr r0, [r4]
	bl sub_8023290
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08023D0C
	adds r0, r7, #0
	bl sub_80243E0
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08023D0C
_08023C92:
	ldr r5, _08023D50 @ =gUnknown_030012D8
	ldr r1, [r5]
	movs r0, #0x7f
	ldrb r2, [r1, #0xc]
	ands r0, r2
	strb r0, [r1, #0xc]
	ldr r4, [r5]
	movs r0, #0x29
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
	ldr r0, _08023D64 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x2c
	bl PlaySfx
	ldr r0, [r5]
	ldr r0, [r0, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #0x29
	bl sub_803AD80
	ldr r0, _08023D58 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r3, [r5]
	adds r1, r3, #0
	adds r1, #0x29
	ldrb r1, [r1]
	lsls r1, r1, #0x1c
	lsrs r1, r1, #0x1c
	ldr r2, [r3, #0x20]
	adds r3, #0x2d
	ldr r4, [r2]
	ldrb r5, [r3]
	lsls r2, r5, #3
	subs r2, r2, r5
	lsls r2, r2, #2
	adds r2, r2, r4
	ldrb r2, [r2, #0x14]
	bl sub_8006D08
	ldr r0, _08023D68 @ =gUnknown_03001318
	ldr r0, [r0]
	bl sub_8028504
_08023D0C:
	ldr r0, _08023D6C @ =gUnknown_030012F4
	ldr r0, [r0]
	bl sub_8008C80
	ldr r0, _08023D70 @ =gUnknown_030012EC
	ldr r0, [r0]
	bl sub_8008C80
	ldr r0, _08023D74 @ =gUnknown_030012F0
	ldr r0, [r0]
	bl sub_8008C80
	ldr r0, _08023D78 @ =gUnknown_030012F8
	ldr r0, [r0]
	bl sub_8008C80
	adds r0, r7, #0
	bl sub_802400C
	movs r0, #0
	bl sub_8001524
	bl sub_8001604
	bl sub_80015E0
	bl sub_8001614
	bl sub_8001624
	b _08023E5A
	.align 2, 0
_08023D4C: .4byte gUnknown_030012C8
_08023D50: .4byte gUnknown_030012D8
_08023D54: .4byte gUnknown_030012D4
_08023D58: .4byte gUnknown_030012B8
_08023D5C: .4byte gUnknown_03001308
_08023D60: .4byte gUnknown_030012C0
_08023D64: .4byte gUnknown_030012BC
_08023D68: .4byte gUnknown_03001318
_08023D6C: .4byte gUnknown_030012F4
_08023D70: .4byte gUnknown_030012EC
_08023D74: .4byte gUnknown_030012F0
_08023D78: .4byte gUnknown_030012F8
_08023D7C:
	bl sub_802423C
	adds r0, r7, #0
	bl sub_802400C
	ldr r5, _08023ED4 @ =gUnknown_03001304
	ldr r0, [r5]
	bl sub_80007AC
	ldr r0, [r4]
	movs r1, #0x82
	lsls r1, r1, #1
	adds r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0
	bne _08023DCA
	ldr r1, _08023ED8 @ =gUnknown_030007E0
	movs r0, #8
	ldrh r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _08023DCA
	bl sub_8004D74
	adds r4, r0, #0
	cmp r4, #0
	bne _08023DBE
	adds r0, r7, #0
	bl sub_80241BC
	ldr r0, [r5]
	bl sub_80007AC
_08023DBE:
	cmp r4, #1
	bne _08023DC4
	b _08023BB8
_08023DC4:
	cmp r4, #2
	bne _08023DCA
	b _08023BBE
_08023DCA:
	ldr r0, _08023ED8 @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #4
	ands r0, r1
	cmp r0, #0
	beq _08023DDE
	ldr r0, _08023EDC @ =gUnknown_03001318
	ldr r0, [r0]
	bl sub_8028504
_08023DDE:
	ldr r0, _08023EE0 @ =gUnknown_030012F4
	ldr r0, [r0]
	bl sub_800891C
	ldr r0, _08023EE4 @ =gUnknown_030012E8
	ldr r0, [r0]
	bl sub_800891C
	ldr r4, _08023EE8 @ =gUnknown_030012D8
	ldr r0, [r4]
	ldr r2, [r0, #0x18]
	movs r3, #0x38
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r2, #0x3c]
	bl sub_803AD7C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08023E16
	ldr r0, [r4]
	ldr r2, [r0, #0x18]
	movs r5, #0x18
	ldrsh r1, [r2, r5]
	adds r0, r0, r1
	ldr r1, [r2, #0x1c]
	bl sub_803AD7C
_08023E16:
	ldr r0, _08023EEC @ =gUnknown_0300130C
	ldr r0, [r0]
	bl sub_80091D4
	ldr r0, _08023EF0 @ =gUnknown_030012EC
	ldr r0, [r0]
	bl sub_800891C
	ldr r0, _08023EF4 @ =gUnknown_030012F0
	ldr r0, [r0]
	bl sub_800891C
	ldr r0, _08023EF8 @ =gUnknown_030012F8
	ldr r0, [r0]
	bl sub_800891C
	ldr r0, _08023EDC @ =gUnknown_03001318
	ldr r0, [r0]
	bl sub_8028400
	ldr r0, _08023EFC @ =gUnknown_030012C0
	ldr r1, [r0]
	adds r0, r1, #0
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _08023E52
	adds r0, r1, #0
	bl sub_8022F2C
_08023E52:
	ldr r1, _08023F00 @ =gUnknown_0300082C
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
_08023E5A:
	bl sub_80241B0
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08023E72
	ldr r4, _08023EE8 @ =gUnknown_030012D8
	ldr r1, [r4]
	movs r0, #1
	ldrb r1, [r1, #0xc]
	ands r0, r1
	cmp r0, #0
	beq _08023D7C
_08023E72:
	bl sub_80014A4
	bl sub_80241B0
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08023E82
	b _08023F92
_08023E82:
	movs r0, #0
	mov sl, r0
	adds r0, r7, #0
	bl sub_8024404
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08023F08
	ldr r6, _08023EFC @ =gUnknown_030012C0
	ldr r0, [r6]
	bl sub_80232B8
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08023F08
	ldr r0, [r6]
	bl sub_8023104
	ldr r0, [r0]
	ldr r1, _08023F04 @ =0xFFFFE200
	adds r4, r0, r1
	ldr r0, _08023EE8 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0, #4]
	movs r2, #0x90
	lsls r2, r2, #5
	adds r5, r0, r2
	str r4, [sp, #8]
	str r5, [sp, #0xc]
	ldr r4, [r6]
	adds r0, r4, #0
	bl sub_8023104
	bl sub_801B29C
	adds r1, r0, #0
	adds r0, r4, #0
	add r2, sp, #8
	bl sub_802356C
	b _08023F92
	.align 2, 0
_08023ED4: .4byte gUnknown_03001304
_08023ED8: .4byte gUnknown_030007E0
_08023EDC: .4byte gUnknown_03001318
_08023EE0: .4byte gUnknown_030012F4
_08023EE4: .4byte gUnknown_030012E8
_08023EE8: .4byte gUnknown_030012D8
_08023EEC: .4byte gUnknown_0300130C
_08023EF0: .4byte gUnknown_030012EC
_08023EF4: .4byte gUnknown_030012F0
_08023EF8: .4byte gUnknown_030012F8
_08023EFC: .4byte gUnknown_030012C0
_08023F00: .4byte gUnknown_0300082C
_08023F04: .4byte 0xFFFFE200
_08023F08:
	adds r0, r7, #0
	bl sub_80243E0
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08023F44
	ldr r4, _08023F3C @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_8023290
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08023F44
	ldr r2, _08023F40 @ =gUnknown_030012D8
	ldr r2, [r2]
	ldr r0, [r2]
	ldr r1, [r2, #4]
	str r0, [sp, #0x10]
	str r1, [sp, #0x14]
	ldr r0, [r4]
	add r2, sp, #0x10
	movs r1, #0
	bl sub_802356C
	b _08023F92
	.align 2, 0
_08023F3C: .4byte gUnknown_030012C0
_08023F40: .4byte gUnknown_030012D8
_08023F44:
	movs r7, #0
	movs r5, #0
	ldr r1, _08023FF0 @ =gUnknown_0300130C
	ldr r0, [r1]
	ldr r0, [r0]
	cmp r7, r0
	bge _08023F88
	adds r6, r1, #0
_08023F54:
	ldr r0, [r6]
	ldr r1, [r0, #8]
	lsls r0, r5, #2
	adds r0, r0, r1
	ldr r4, [r0]
	ldr r1, [r4, #0x18]
	adds r1, #0x48
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	cmp r0, #3
	bne _08023F7E
	adds r0, r4, #0
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #0xa
	bne _08023F7E
	adds r7, #1
_08023F7E:
	adds r5, #1
	ldr r0, [r6]
	ldr r0, [r0]
	cmp r5, r0
	blt _08023F54
_08023F88:
	ldr r0, _08023FF4 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r1, r7, #0
	bl sub_8023140
_08023F92:
	ldr r0, _08023FF8 @ =gUnknown_030012E8
	ldr r0, [r0]
	bl sub_8008CEC
	ldr r0, _08023FF0 @ =gUnknown_0300130C
	ldr r0, [r0]
	bl sub_8009914
	ldr r0, _08023FFC @ =gUnknown_030012EC
	ldr r0, [r0]
	bl sub_8008CEC
	ldr r0, _08024000 @ =gUnknown_030012F0
	ldr r0, [r0]
	bl sub_8008CEC
	ldr r0, _08024004 @ =gUnknown_030012F8
	ldr r0, [r0]
	bl sub_8008CEC
	ldr r0, _08024008 @ =gUnknown_030012F4
	ldr r0, [r0]
	bl sub_8008CEC
	bl sub_8001578
	bl sub_8001564
	bl sub_8001550
	bl sub_800153C
	bl sub_800158C
	bl sub_80006A8
	bl sub_8001614
	mov r0, sl
	add sp, #0x18
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08023FF0: .4byte gUnknown_0300130C
_08023FF4: .4byte gUnknown_030012C0
_08023FF8: .4byte gUnknown_030012E8
_08023FFC: .4byte gUnknown_030012EC
_08024000: .4byte gUnknown_030012F0
_08024004: .4byte gUnknown_030012F8
_08024008: .4byte gUnknown_030012F4

	thumb_func_start sub_802400C
sub_802400C: @ 0x0802400C
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r0, _080240B4 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _080240B8 @ =gUnknown_030012D4
	ldr r0, [r0]
	bl sub_8026E6C
	ldr r5, _080240BC @ =gUnknown_03001308
	ldr r0, [r5]
	bl sub_802692C
	ldr r0, _080240C0 @ =gUnknown_030012C8
	ldr r0, [r0]
	bl sub_8026F54
	ldr r1, [r4]
	movs r0, #0x80
	lsls r0, r0, #5
	cmp r1, r0
	bgt _080240AE
	ldr r0, _080240C4 @ =gUnknown_03001318
	ldr r0, [r0]
	bl sub_80274EC
	ldr r0, _080240C8 @ =gUnknown_030012F4
	ldr r0, [r0]
	bl sub_8008DC0
	ldr r4, _080240CC @ =gUnknown_030012D8
	ldr r0, [r4]
	ldr r2, [r0, #0x18]
	movs r3, #0x28
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r2, #0x2c]
	bl sub_803AD7C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08024072
	ldr r0, [r4]
	ldr r2, [r0, #0x18]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r2, #0x24]
	bl sub_803AD7C
_08024072:
	ldr r0, _080240D0 @ =gUnknown_030012F0
	ldr r0, [r0]
	bl sub_8008DC0
	ldr r0, _080240D4 @ =gUnknown_030012EC
	ldr r0, [r0]
	bl sub_8008DC0
	ldr r0, _080240D8 @ =gUnknown_0300130C
	ldr r0, [r0]
	bl sub_800944C
	ldr r0, _080240DC @ =gUnknown_030012F8
	ldr r0, [r0]
	bl sub_8008DC0
	ldr r4, _080240E0 @ =gUnknown_03001300
	ldr r0, [r4]
	bl sub_8006A48
	bl sub_80006A8
	ldr r0, [r4]
	bl sub_8006AAC
	ldr r0, [r5]
	bl sub_80268F8
	bl FlushVramDmaQueue
_080240AE:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080240B4: .4byte gUnknown_030012B8
_080240B8: .4byte gUnknown_030012D4
_080240BC: .4byte gUnknown_03001308
_080240C0: .4byte gUnknown_030012C8
_080240C4: .4byte gUnknown_03001318
_080240C8: .4byte gUnknown_030012F4
_080240CC: .4byte gUnknown_030012D8
_080240D0: .4byte gUnknown_030012F0
_080240D4: .4byte gUnknown_030012EC
_080240D8: .4byte gUnknown_0300130C
_080240DC: .4byte gUnknown_030012F8
_080240E0: .4byte gUnknown_03001300

	thumb_func_start sub_80240E4
sub_80240E4: @ 0x080240E4
	push {r4, r5, r6, r7, lr}
	adds r2, r0, #0
	ldr r6, _08024154 @ =gUnknown_03001280
	movs r0, #0
	str r0, [r6]
	ldr r3, _08024158 @ =gUnknown_03001308
	ldr r1, [r3]
	adds r1, #0x2b
	strb r0, [r1]
	ldr r1, [r2, #0x18]
	ldrh r0, [r1, #0x10]
	cmp r0, #0
	beq _0802415C
	ldr r1, [r1, #8]
	cmp r1, #1
	bne _0802410A
	ldr r0, [r3]
	adds r0, #0x2b
	strb r1, [r0]
_0802410A:
	ldr r4, [r2, #0x18]
	ldrb r1, [r4, #0x10]
	lsls r0, r1, #6
	movs r2, #0x3f
	ldrb r3, [r6]
	ands r2, r3
	orrs r2, r0
	strb r2, [r6]
	movs r3, #0x1f
	ldrb r5, [r4, #0x12]
	ands r5, r3
	movs r1, #0x20
	rsbs r1, r1, #0
	adds r0, r1, #0
	ldrb r7, [r6, #2]
	ands r0, r7
	orrs r0, r5
	strb r0, [r6, #2]
	ldrb r4, [r4, #0x13]
	ands r3, r4
	ldrb r0, [r6, #3]
	ands r1, r0
	orrs r1, r3
	strb r1, [r6, #3]
	movs r0, #8
	orrs r2, r0
	strb r2, [r6]
	movs r0, #1
	ldrb r1, [r6, #1]
	orrs r0, r1
	movs r1, #2
	orrs r0, r1
	movs r1, #4
	orrs r0, r1
	movs r1, #0x10
	orrs r0, r1
	b _08024190
	.align 2, 0
_08024154: .4byte gUnknown_03001280
_08024158: .4byte gUnknown_03001308
_0802415C:
	movs r2, #0x3f
	ldrb r3, [r6]
	ands r2, r3
	movs r0, #0x20
	rsbs r0, r0, #0
	adds r1, r0, #0
	ldrb r7, [r6, #2]
	ands r1, r7
	movs r3, #0x10
	orrs r1, r3
	strb r1, [r6, #2]
	ldrb r1, [r6, #3]
	ands r0, r1
	orrs r0, r3
	strb r0, [r6, #3]
	movs r0, #8
	orrs r2, r0
	strb r2, [r6]
	movs r0, #1
	ldrb r7, [r6, #1]
	orrs r0, r7
	movs r1, #2
	orrs r0, r1
	movs r1, #4
	orrs r0, r1
	orrs r0, r3
_08024190:
	strb r0, [r6, #1]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8024198
sub_8024198: @ 0x08024198
	ldr r1, _080241A0 @ =gUnknown_03000830
	movs r0, #0
	strb r0, [r1]
	bx lr
	.align 2, 0
_080241A0: .4byte gUnknown_03000830

	thumb_func_start sub_80241A4
sub_80241A4: @ 0x080241A4
	ldr r1, _080241AC @ =gUnknown_03000830
	movs r0, #1
	strb r0, [r1]
	bx lr
	.align 2, 0
_080241AC: .4byte gUnknown_03000830

	thumb_func_start sub_80241B0
sub_80241B0: @ 0x080241B0
	ldr r0, _080241B8 @ =gUnknown_03000830
	ldrb r0, [r0]
	bx lr
	.align 2, 0
_080241B8: .4byte gUnknown_03000830

	thumb_func_start sub_80241BC
sub_80241BC: @ 0x080241BC
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, _08024220 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r1, _08024224 @ =0x040000D4
	ldr r0, [r4, #0x18]
	ldr r0, [r0]
	str r0, [r1]
	movs r2, #0xa0
	lsls r2, r2, #0x13
	str r2, [r1, #4]
	ldr r0, _08024228 @ =0x80000100
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	movs r0, #0
	strh r0, [r2]
	ldr r0, _0802422C @ =gUnknown_030012FC
	ldr r0, [r0]
	bl sub_8006C4C
	ldr r0, _08024230 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006A90
	ldr r0, _08024234 @ =gUnknown_030012D4
	ldr r0, [r0]
	bl sub_8026DFC
	ldr r0, _08024238 @ =gUnknown_03001308
	ldr r0, [r0]
	bl sub_8026984
	adds r0, r4, #0
	bl sub_802400C
	movs r0, #0
	bl sub_8001524
	bl sub_80015E0
	bl sub_8001614
	bl sub_8001624
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08024220: .4byte gUnknown_030012B8
_08024224: .4byte 0x040000D4
_08024228: .4byte 0x80000100
_0802422C: .4byte gUnknown_030012FC
_08024230: .4byte gUnknown_03001300
_08024234: .4byte gUnknown_030012D4
_08024238: .4byte gUnknown_03001308

	thumb_func_start sub_802423C
sub_802423C: @ 0x0802423C
	push {lr}
	ldr r0, _08024254 @ =gUnknown_030012FC
	ldr r0, [r0]
	bl sub_8006C4C
	ldr r0, _08024258 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006A90
	pop {r0}
	bx r0
	.align 2, 0
_08024254: .4byte gUnknown_030012FC
_08024258: .4byte gUnknown_03001300

	thumb_func_start sub_802425C
sub_802425C: @ 0x0802425C
	push {lr}
	adds r2, r0, #0
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _0802426E
	adds r0, r2, #0
	bl sub_8026ED0
_0802426E:
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start nullsub_25
nullsub_25: @ 0x08024274
	bx lr
	.align 2, 0

	thumb_func_start sub_8024278
sub_8024278: @ 0x08024278
	push {r4, r5, r6, lr}
	movs r6, #0
	ldr r2, _080242AC @ =gStaticData_0816C86C
	lsls r1, r0, #3
	adds r1, r1, r0
	lsls r1, r1, #2
	adds r2, #0x20
	adds r1, r1, r2
	ldr r4, [r1]
	movs r5, #0
	ldr r0, [r4]
	cmp r6, r0
	bge _080242D4
_08024292:
	ldr r1, [r4, #4]
	lsls r0, r5, #2
	adds r0, r0, r1
	ldr r2, [r0]
	movs r0, #0
	ldr r1, [r2, #8]
	cmp r1, #0
	blt _080242CA
	cmp r1, #2
	ble _080242B0
	cmp r1, #3
	beq _080242C4
	b _080242CA
	.align 2, 0
_080242AC: .4byte gStaticData_0816C86C
_080242B0:
	ldr r0, _080242C0 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r2, #4]
	ldr r1, [r1, #0x1c]
	bl sub_8025894
	b _080242CA
	.align 2, 0
_080242C0: .4byte gUnknown_030012B4
_080242C4:
	ldrh r0, [r2, #0x10]
	bl sub_802968C
_080242CA:
	adds r6, r6, r0
	adds r5, #1
	ldr r0, [r4]
	cmp r5, r0
	blt _08024292
_080242D4:
	ldr r2, [r4, #8]
	cmp r2, #0
	beq _08024308
	movs r0, #0
	ldr r1, [r2, #8]
	cmp r1, #0
	blt _08024306
	cmp r1, #2
	ble _080242EC
	cmp r1, #3
	beq _08024300
	b _08024306
_080242EC:
	ldr r0, _080242FC @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r2, #4]
	ldr r1, [r1, #0x1c]
	bl sub_8025894
	b _08024306
	.align 2, 0
_080242FC: .4byte gUnknown_030012B4
_08024300:
	ldrh r0, [r2, #0x10]
	bl sub_802968C
_08024306:
	adds r6, r6, r0
_08024308:
	ldr r1, [r4, #0xc]
	cmp r1, #0
	beq _0802433C
	movs r0, #0
	ldr r2, [r1, #8]
	cmp r2, #0
	blt _0802433A
	cmp r2, #2
	ble _08024320
	cmp r2, #3
	beq _08024334
	b _0802433A
_08024320:
	ldr r0, _08024330 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r1, #4]
	ldr r1, [r1, #0x1c]
	bl sub_8025894
	b _0802433A
	.align 2, 0
_08024330: .4byte gUnknown_030012B4
_08024334:
	ldrh r0, [r1, #0x10]
	bl sub_802968C
_0802433A:
	adds r6, r6, r0
_0802433C:
	adds r0, r6, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1

	thumb_func_start sub_8024344
sub_8024344: @ 0x08024344
	push {r4, r5, r6, r7, lr}
	mov ip, r1
	movs r3, #0
	ldr r2, _080243DC @ =gStaticData_0816C86C
	lsls r1, r0, #3
	adds r1, r1, r0
	lsls r1, r1, #2
	adds r2, #0x20
	adds r1, r1, r2
	ldr r5, [r1]
	movs r4, #0
	ldr r0, [r5]
	cmp r3, r0
	bge _0802438C
	ldr r2, [r5, #4]
	mov r1, ip
	lsls r7, r1, #1
	adds r6, r0, #0
_08024368:
	ldr r1, [r2]
	ldr r0, [r1, #8]
	cmp r0, #3
	beq _08024380
	ldr r0, [r1, #4]
	ldr r0, [r0, #0x1c]
	ldr r1, [r0, #0x10]
	adds r1, r7, r1
	ldrh r3, [r1]
	rsbs r0, r3, #0
	orrs r0, r3
	lsrs r3, r0, #0x1f
_08024380:
	adds r2, #4
	adds r4, #1
	cmp r4, r6
	bge _0802438C
	cmp r3, #0
	beq _08024368
_0802438C:
	cmp r3, #0
	bne _080243D4
	ldr r1, [r5, #8]
	cmp r1, #0
	beq _080243B0
	ldr r0, [r1, #8]
	cmp r0, #3
	beq _080243B0
	ldr r0, [r1, #4]
	ldr r0, [r0, #0x1c]
	ldr r0, [r0, #0x10]
	mov r2, ip
	lsls r1, r2, #1
	adds r1, r1, r0
	ldrh r3, [r1]
	rsbs r0, r3, #0
	orrs r0, r3
	lsrs r3, r0, #0x1f
_080243B0:
	cmp r3, #0
	bne _080243D4
	ldr r1, [r5, #0xc]
	cmp r1, #0
	beq _080243D4
	ldr r0, [r1, #8]
	cmp r0, #3
	beq _080243D4
	ldr r0, [r1, #4]
	ldr r0, [r0, #0x1c]
	ldr r0, [r0, #0x10]
	mov r2, ip
	lsls r1, r2, #1
	adds r1, r1, r0
	ldrh r3, [r1]
	rsbs r0, r3, #0
	orrs r0, r3
	lsrs r3, r0, #0x1f
_080243D4:
	adds r0, r3, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080243DC: .4byte gStaticData_0816C86C

	thumb_func_start sub_80243E0
sub_80243E0: @ 0x080243E0
	ldr r3, _08024400 @ =gStaticData_0816C86C
	ldr r2, [r0]
	lsls r1, r2, #3
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r3, #0x20
	adds r1, r1, r3
	ldr r2, [r1]
	movs r3, #0
	ldr r1, [r0, #0x18]
	ldr r0, [r2, #0xc]
	cmp r1, r0
	bne _080243FC
	movs r3, #1
_080243FC:
	adds r0, r3, #0
	bx lr
	.align 2, 0
_08024400: .4byte gStaticData_0816C86C

	thumb_func_start sub_8024404
sub_8024404: @ 0x08024404
	ldr r3, _08024424 @ =gStaticData_0816C86C
	ldr r2, [r0]
	lsls r1, r2, #3
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r3, #0x20
	adds r1, r1, r3
	ldr r2, [r1]
	movs r3, #0
	ldr r1, [r0, #0x18]
	ldr r0, [r2, #8]
	cmp r1, r0
	bne _08024420
	movs r3, #1
_08024420:
	adds r0, r3, #0
	bx lr
	.align 2, 0
_08024424: .4byte gStaticData_0816C86C

	thumb_func_start sub_8024428
sub_8024428: @ 0x08024428
	push {lr}
	movs r1, #0xc
	bl sub_8024344
	pop {r1}
	bx r1

	thumb_func_start sub_8024434
sub_8024434: @ 0x08024434
	push {lr}
	movs r1, #9
	bl sub_8024344
	pop {r1}
	bx r1

	thumb_func_start sub_8024440
sub_8024440: @ 0x08024440
	push {lr}
	movs r1, #0xb
	bl sub_8024344
	pop {r1}
	bx r1

	thumb_func_start sub_802444C
sub_802444C: @ 0x0802444C
	push {lr}
	movs r1, #0xa
	bl sub_8024344
	pop {r1}
	bx r1

	thumb_func_start sub_8024458
sub_8024458: @ 0x08024458
	push {lr}
	movs r1, #8
	bl sub_8024344
	pop {r1}
	bx r1

	thumb_func_start sub_8024464
sub_8024464: @ 0x08024464
	push {lr}
	adds r2, r0, #0
	movs r0, #0
	ldr r1, [r2, #8]
	cmp r1, #0
	blt _08024492
	cmp r1, #2
	ble _0802447A
	cmp r1, #3
	beq _0802448C
	b _08024492
_0802447A:
	ldr r0, _08024488 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r2, #4]
	ldr r1, [r1, #0x1c]
	bl sub_8025894
	b _08024492
	.align 2, 0
_08024488: .4byte gUnknown_030012B4
_0802448C:
	ldrh r0, [r2, #0x10]
	bl sub_802968C
_08024492:
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8024498
sub_8024498: @ 0x08024498
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, _080244AC @ =gUnknown_030012C0
	ldr r0, [r0]
	ldr r0, [r0, #0x78]
	cmp r0, #3
	bne _080244B0
	movs r1, #0x12
	b _080244D6
	.align 2, 0
_080244AC: .4byte gUnknown_030012C0
_080244B0:
	adds r0, r4, #0
	bl sub_8024404
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _080244C0
	movs r1, #6
	b _080244D6
_080244C0:
	ldr r2, _080244E4 @ =gStaticData_0816C86C
	ldr r1, [r4]
	lsls r0, r1, #3
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r2, #4
	adds r0, r0, r2
	ldr r0, [r0]
	ldr r1, _080244E8 @ =gStaticData_0816CD80
	adds r0, r0, r1
	ldrb r1, [r0]
_080244D6:
	ldr r0, _080244EC @ =gUnknown_030012BC
	ldr r0, [r0]
	bl sub_8001B54
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080244E4: .4byte gStaticData_0816C86C
_080244E8: .4byte gStaticData_0816CD80
_080244EC: .4byte gUnknown_030012BC

	thumb_func_start sub_80244F0
sub_80244F0: @ 0x080244F0
	push {r4, lr}
	adds r3, r0, #0
	movs r4, #0
	ldr r2, _08024520 @ =gStaticData_0816C86C
	ldr r1, [r3]
	lsls r0, r1, #3
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r2, #0x20
	adds r0, r0, r2
	ldr r0, [r0]
	ldr r0, [r0]
	subs r0, #1
	ldr r1, [r3, #4]
	cmp r1, r0
	bge _08024516
	adds r0, r1, #1
	str r0, [r3, #4]
	movs r4, #1
_08024516:
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08024520: .4byte gStaticData_0816C86C

	thumb_func_start sub_8024524
sub_8024524: @ 0x08024524
	ldr r3, _0802453C @ =gStaticData_0816C86C
	ldr r2, [r0]
	lsls r1, r2, #3
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r3, #0x20
	adds r1, r1, r3
	ldr r1, [r1]
	ldr r1, [r1, #0xc]
	str r1, [r0, #0x18]
	bx lr
	.align 2, 0
_0802453C: .4byte gStaticData_0816C86C

	thumb_func_start sub_8024540
sub_8024540: @ 0x08024540
	ldr r3, _08024558 @ =gStaticData_0816C86C
	ldr r2, [r0]
	lsls r1, r2, #3
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r3, #0x20
	adds r1, r1, r3
	ldr r1, [r1]
	ldr r1, [r1, #8]
	str r1, [r0, #0x18]
	bx lr
	.align 2, 0
_08024558: .4byte gStaticData_0816C86C

	thumb_func_start sub_802455C
sub_802455C: @ 0x0802455C
	adds r3, r0, #0
	ldr r2, _0802458C @ =gStaticData_0816C86C
	ldr r1, [r3]
	lsls r0, r1, #3
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r2, #0x20
	adds r0, r0, r2
	ldr r2, [r0]
	ldr r0, [r2]
	cmp r0, #0
	beq _08024580
	ldr r0, [r3, #4]
	ldr r1, [r2, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	str r0, [r3, #0x18]
_08024580:
	ldr r1, [r2]
	rsbs r0, r1, #0
	orrs r0, r1
	lsrs r0, r0, #0x1f
	bx lr
	.align 2, 0
_0802458C: .4byte gStaticData_0816C86C

	thumb_func_start sub_8024590
sub_8024590: @ 0x08024590
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	ldr r6, _080245E8 @ =gUnknown_030012BC
	ldr r0, [r6]
	ldr r2, [r5]
	lsls r4, r1, #2
	adds r2, r4, r2
	ldr r1, [r2]
	ldr r1, [r1, #0x14]
	bl sub_8001B54
	ldr r0, [r6]
	bl sub_8001AB8
	ldr r1, [r5]
	adds r1, r4, r1
	ldr r2, [r1]
	ldr r1, [r2, #0x14]
	cmp r0, r1
	bne _080245EC
	ldr r1, [r2, #0x18]
	cmp r1, #0x63
	beq _080245C8
	ldr r0, [r6]
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
_080245C8:
	ldr r0, [r5]
	adds r0, r4, r0
	ldr r0, [r0]
	ldr r0, [r0, #8]
	movs r2, #0x80
	rsbs r2, r2, #0
	adds r1, r2, #0
	orrs r0, r1
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	movs r1, #1
	movs r2, #0
	bl sub_800132C
	b _08024636
	.align 2, 0
_080245E8: .4byte gUnknown_030012BC
_080245EC:
	ldr r0, [r2, #8]
	movs r2, #0x80
	rsbs r2, r2, #0
	adds r1, r2, #0
	orrs r0, r1
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	movs r1, #1
	movs r2, #0
	bl sub_800132C
	ldr r0, [r5]
	adds r0, r4, r0
	ldr r0, [r0]
	ldr r0, [r0, #0x18]
	cmp r0, #0x63
	beq _08024636
	adds r7, r6, #0
	adds r6, r4, #0
_08024612:
	ldr r0, [r7]
	bl sub_8001AB8
	ldr r2, [r5]
	adds r1, r4, r2
	ldr r1, [r1]
	ldr r1, [r1, #0x14]
	cmp r0, r1
	bne _08024612
	ldr r0, _0802463C @ =gUnknown_030012BC
	ldr r0, [r0]
	adds r1, r6, r2
	ldr r1, [r1]
	ldr r1, [r1, #0x18]
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
_08024636:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802463C: .4byte gUnknown_030012BC

	thumb_func_start sub_8024640
sub_8024640: @ 0x08024640
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	movs r6, #0
	b _080246C8
_08024648:
	adds r0, r4, #0
	adds r1, r6, #0
	bl sub_8024708
	adds r0, r4, #0
	adds r1, r6, #0
	bl sub_8024590
	ldr r0, [r4]
	lsls r5, r6, #2
	adds r0, r5, r0
	ldr r1, [r0]
	ldr r0, [r1, #4]
	ldrb r1, [r1, #0x10]
	movs r2, #8
	bl sub_80010E0
	lsls r0, r0, #0x18
	lsrs r7, r0, #0x18
	ldr r0, [r4]
	adds r0, r5, r0
	ldr r0, [r0]
	ldrb r0, [r0, #0x11]
	cmp r0, #0
	beq _08024684
	ldr r0, _080246D4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8001AC4
_08024684:
	ldr r0, [r4]
	adds r0, r5, r0
	ldr r0, [r0]
	ldr r1, [r0, #0xc]
	movs r0, #1
	rsbs r0, r0, #0
	cmp r1, r0
	beq _080246A0
	lsls r0, r1, #0x18
	lsrs r0, r0, #0x18
	movs r1, #1
	movs r2, #0
	bl sub_800132C
_080246A0:
	ldr r0, [r4]
	adds r0, r5, r0
	ldr r1, [r0]
	ldrb r0, [r1, #0x12]
	cmp r0, #0
	beq _080246BA
	ldr r1, [r1, #0x18]
	cmp r1, #0x63
	beq _080246BA
	ldr r0, _080246D4 @ =gUnknown_030012BC
	ldr r0, [r0]
	bl sub_80019A8
_080246BA:
	adds r0, r4, #0
	adds r1, r6, #0
	adds r2, r7, #0
	bl sub_80246D8
	adds r6, r0, #0
	adds r6, #1
_080246C8:
	ldr r0, [r4, #4]
	cmp r6, r0
	blt _08024648
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080246D4: .4byte gUnknown_030012BC

	thumb_func_start sub_80246D8
sub_80246D8: @ 0x080246D8
	push {r4, lr}
	lsls r2, r2, #0x18
	cmp r2, #0
	bne _08024700
	adds r2, r1, #1
	ldr r4, [r0, #4]
	cmp r2, r4
	bge _08024700
	ldr r3, [r0]
	b _080246F4
_080246EC:
	adds r1, r2, #0
	adds r2, r1, #1
	cmp r2, r4
	bge _08024700
_080246F4:
	lsls r0, r1, #2
	adds r0, r0, r3
	ldr r0, [r0, #4]
	ldrb r0, [r0, #0x10]
	cmp r0, #1
	beq _080246EC
_08024700:
	adds r0, r1, #0
	pop {r4}
	pop {r1}
	bx r1

	thumb_func_start sub_8024708
sub_8024708: @ 0x08024708
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	ldr r0, [r5]
	lsls r1, r1, #2
	adds r1, r1, r0
	ldr r0, [r1]
	ldr r6, [r0]
	ldr r0, [r5, #0xc]
	movs r1, #1
	eors r0, r1
	str r0, [r5, #0xc]
	cmp r0, #0
	bne _08024732
	movs r1, #0x80
	lsls r1, r1, #2
	adds r0, r6, r1
	movs r1, #0xc0
	lsls r1, r1, #0x13
	bl LoadTaggedAsset
	b _0802473E
_08024732:
	movs r2, #0x80
	lsls r2, r2, #2
	adds r0, r6, r2
	ldr r1, _08024774 @ =0x0600A000
	bl LoadTaggedAsset
_0802473E:
	ldr r4, _08024778 @ =gUnknown_03001314
	movs r1, #1
	ldrb r5, [r5, #0xc]
	ands r1, r5
	lsls r1, r1, #4
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r2, [r4]
	ands r0, r2
	orrs r0, r1
	strb r0, [r4]
	bl sub_80006A8
	ldr r1, _0802477C @ =0x040000D4
	str r6, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	str r0, [r1, #4]
	ldr r0, _08024780 @ =0x80000100
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	subs r1, #0xd4
	ldrh r0, [r4]
	strh r0, [r1]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08024774: .4byte 0x0600A000
_08024778: .4byte gUnknown_03001314
_0802477C: .4byte 0x040000D4
_08024780: .4byte 0x80000100

	thumb_func_start sub_8024784
sub_8024784: @ 0x08024784
	ldr r1, _0802478C @ =gUnknown_03001314
	str r0, [r1]
	bx lr
	.align 2, 0
_0802478C: .4byte gUnknown_03001314

	thumb_func_start sub_8024790
sub_8024790: @ 0x08024790
	push {r4, r5, lr}
	adds r5, r0, #0
	ldr r0, [r5]
	lsls r4, r1, #2
	adds r0, r4, r0
	ldr r0, [r0]
	ldrb r0, [r0, #0x11]
	cmp r0, #0
	beq _080247AC
	ldr r0, _080247E8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8001AC4
_080247AC:
	ldr r0, [r5]
	adds r0, r4, r0
	ldr r0, [r0]
	ldr r1, [r0, #0xc]
	movs r0, #1
	rsbs r0, r0, #0
	cmp r1, r0
	beq _080247C8
	lsls r0, r1, #0x18
	lsrs r0, r0, #0x18
	movs r1, #1
	movs r2, #0
	bl sub_800132C
_080247C8:
	ldr r0, [r5]
	adds r0, r4, r0
	ldr r1, [r0]
	ldrb r0, [r1, #0x12]
	cmp r0, #0
	beq _080247E2
	ldr r1, [r1, #0x18]
	cmp r1, #0x63
	beq _080247E2
	ldr r0, _080247E8 @ =gUnknown_030012BC
	ldr r0, [r0]
	bl sub_80019A8
_080247E2:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080247E8: .4byte gUnknown_030012BC

	thumb_func_start sub_80247EC
sub_80247EC: @ 0x080247EC
	push {lr}
	adds r2, r0, #0
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _080247FE
	adds r0, r2, #0
	bl sub_8026ED0
_080247FE:
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8024804
sub_8024804: @ 0x08024804
	movs r1, #0
	str r1, [r0]
	str r1, [r0, #4]
	movs r1, #1
	str r1, [r0, #0xc]
	bx lr

	thumb_func_start sub_8024810
sub_8024810: @ 0x08024810
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8024804
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1

	thumb_func_start sub_8024820
sub_8024820: @ 0x08024820
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	adds r4, r0, #0
	ldr r1, [r4, #0x18]
	ldr r2, [r4, #0x14]
	movs r3, #0x8c
	lsls r3, r3, #1
	adds r0, r2, r3
	str r1, [r0]
	ldr r0, [r4, #0x24]
	adds r3, #4
	adds r1, r2, r3
	ldr r1, [r1]
	bl sub_8037E54
	str r0, [sp, #4]
	movs r0, #0
	mov r8, r0
	b _08024924
_0802484E:
	movs r7, #1
	adds r0, r4, #0
	mov r1, r8
	bl sub_8024708
	ldr r1, _080248A4 @ =gUnknown_03001300
	ldr r0, [r1]
	bl sub_8006A90
	ldr r2, _080248A4 @ =gUnknown_03001300
	ldr r0, [r2]
	bl sub_8006A48
	bl sub_80006A8
	ldr r3, _080248A4 @ =gUnknown_03001300
	ldr r0, [r3]
	bl sub_8006AAC
	adds r0, r4, #0
	mov r1, r8
	bl sub_8024590
	ldr r0, [r4, #0x10]
	mov r2, r8
	lsls r1, r2, #3
	adds r0, r1, r0
	ldr r0, [r0, #4]
	mov sb, r1
	cmp r0, #0
	bne _080248A8
	ldr r1, [r4]
	lsls r0, r2, #2
	adds r0, r0, r1
	ldr r1, [r0]
	ldr r0, [r1, #4]
	ldrb r1, [r1, #0x10]
	movs r2, #9
	bl sub_80010E0
	lsls r0, r0, #0x18
	lsrs r7, r0, #0x18
	b _0802490C
	.align 2, 0
_080248A4: .4byte gUnknown_03001300
_080248A8:
	movs r2, #0
	cmp r2, r0
	bge _0802490C
_080248AE:
	ldr r0, [r4, #0x10]
	add r0, sb
	ldr r1, [r0]
	lsls r0, r2, #2
	adds r0, r0, r1
	ldr r5, [r0]
	movs r6, #0
	ldrb r0, [r5]
	adds r2, #1
	mov sl, r2
	b _080248F4
_080248C4:
	adds r0, r5, r6
	ldr r1, [r4, #0x14]
	movs r2, #1
	str r2, [sp]
	adds r2, r4, #0
	adds r2, #0x18
	ldr r3, [sp, #4]
	bl sub_8000EE4
	adds r6, r6, r0
	ldr r1, [r4]
	mov r3, r8
	lsls r0, r3, #2
	adds r0, r0, r1
	ldr r1, [r0]
	ldr r0, [r1, #4]
	ldrb r1, [r1, #0x10]
	movs r2, #9
	bl sub_80010E0
	lsls r0, r0, #0x18
	lsrs r7, r0, #0x18
	adds r0, r5, r6
	ldrb r0, [r0]
_080248F4:
	cmp r0, #0
	beq _080248FC
	cmp r7, #1
	beq _080248C4
_080248FC:
	mov r2, sl
	ldr r0, [r4, #0x10]
	add r0, sb
	ldr r0, [r0, #4]
	cmp r2, r0
	bge _0802490C
	cmp r7, #1
	beq _080248AE
_0802490C:
	adds r0, r4, #0
	mov r1, r8
	bl sub_8024790
	adds r0, r4, #0
	mov r1, r8
	adds r2, r7, #0
	bl sub_80246D8
	mov r8, r0
	movs r0, #1
	add r8, r0
_08024924:
	ldr r0, [r4, #4]
	cmp r8, r0
	blt _0802484E
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_802493C
sub_802493C: @ 0x0802493C
	push {lr}
	bl sub_80247EC
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8024948
sub_8024948: @ 0x08024948
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8024810
	movs r0, #0
	str r0, [r4, #0x10]
	str r0, [r4, #0x14]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8024960
sub_8024960: @ 0x08024960
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r7, r2, #0
	ldr r4, [r0, #4]
	lsls r1, r1, #1
	adds r1, r1, r4
	ldrh r1, [r1]
	lsls r0, r1, #2
	adds r4, r4, r0
	movs r0, #0x7f
	mov r8, r0
	movs r6, #0
_0802497C:
	ldrh r1, [r4]
	ldrb r3, [r4]
	adds r4, #2
	movs r0, #0x80
	lsls r0, r0, #8
	ands r0, r1
	cmp r0, #0
	beq _080249B6
	ldrh r2, [r4]
	adds r4, #2
	mov r0, r8
	subs r0, r0, r3
	mov r8, r0
	movs r5, #0xf
_08024998:
	asrs r0, r6, #4
	adds r1, r6, #0
	ands r1, r5
	lsls r0, r0, #6
	adds r0, r0, r1
	lsls r0, r0, #1
	adds r0, r0, r7
	strh r2, [r0]
	adds r6, #1
	subs r0, r3, #1
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	cmp r3, #0
	bne _08024998
	b _08024A8C
_080249B6:
	movs r0, #0x80
	lsls r0, r0, #7
	ands r1, r0
	cmp r1, #0
	beq _08024A64
	mov r2, r8
	subs r2, r2, r3
	mov r8, r2
	ldrh r5, [r4]
	adds r4, #2
	asrs r0, r6, #4
	movs r1, #0xf
	ands r1, r6
	lsls r0, r0, #6
	adds r0, r0, r1
	lsls r0, r0, #1
	adds r0, r0, r7
	strh r5, [r0]
	subs r0, r3, #1
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	adds r6, #1
	movs r0, #0xf
	mov ip, r0
_080249E6:
	ldrh r2, [r4]
	adds r4, #2
	lsls r1, r2, #0x18
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	asrs r1, r1, #0x18
	adds r0, r0, r1
	lsls r0, r0, #0x10
	lsrs r5, r0, #0x10
	asrs r0, r6, #4
	mov sb, r0
	adds r1, r6, #0
	mov r0, ip
	ands r1, r0
	mov r0, sb
	lsls r0, r0, #6
	mov sb, r0
	add r1, sb
	lsls r0, r1, #1
	adds r0, r0, r7
	strh r5, [r0]
	adds r6, #1
	lsls r2, r2, #0x10
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	asrs r2, r2, #0x18
	adds r0, r0, r2
	lsls r0, r0, #0x10
	lsrs r5, r0, #0x10
	asrs r0, r6, #4
	adds r1, r6, #0
	mov r2, ip
	ands r1, r2
	lsls r0, r0, #6
	adds r0, r0, r1
	lsls r0, r0, #1
	adds r0, r0, r7
	strh r5, [r0]
	adds r6, #1
	subs r0, r3, #2
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	cmp r3, #1
	bhi _080249E6
	cmp r3, #0
	beq _08024A8C
	ldrh r0, [r4]
	adds r4, #2
	lsls r0, r0, #0x18
	lsls r2, r5, #0x10
	asrs r2, r2, #0x10
	asrs r0, r0, #0x18
	adds r2, r2, r0
	asrs r0, r6, #4
	movs r1, #0xf
	ands r1, r6
	lsls r0, r0, #6
	adds r0, r0, r1
	lsls r0, r0, #1
	adds r0, r0, r7
	strh r2, [r0]
	adds r6, #1
	b _08024A8C
_08024A64:
	mov r0, r8
	subs r0, r0, r3
	mov r8, r0
	movs r2, #0xf
_08024A6C:
	asrs r0, r6, #4
	lsls r1, r0, #6
	adds r0, r6, #0
	ands r0, r2
	adds r0, r1, r0
	lsls r0, r0, #1
	adds r0, r0, r7
	ldrh r1, [r4]
	strh r1, [r0]
	adds r4, #2
	adds r6, #1
	subs r0, r3, #1
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	cmp r3, #0
	bne _08024A6C
_08024A8C:
	mov r2, r8
	cmp r2, #0
	blt _08024A94
	b _0802497C
_08024A94:
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8024AA0
sub_8024AA0: @ 0x08024AA0
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r0, [r1]
	asrs r6, r0, #7
	ldr r0, [r1, #4]
	asrs r7, r0, #6
	ldr r2, [r4, #0xc]
	ldr r5, [r4, #0x10]
	cmp r6, r2
	ble _08024AC8
	adds r1, r2, #4
	adds r0, r4, #0
	bl sub_8024C08
	ldrb r0, [r4, #0x14]
	adds r0, #1
	movs r1, #3
	ands r0, r1
	strb r0, [r4, #0x14]
	b _08024ADE
_08024AC8:
	cmp r6, r2
	bge _08024ADE
	ldrb r0, [r4, #0x14]
	subs r0, #1
	movs r1, #3
	ands r0, r1
	strb r0, [r4, #0x14]
	subs r1, r2, #1
	adds r0, r4, #0
	bl sub_8024C08
_08024ADE:
	str r6, [r4, #0xc]
	cmp r7, r5
	ble _08024AF8
	adds r1, r5, #4
	adds r0, r4, #0
	bl sub_8024BAC
	ldrb r0, [r4, #0x15]
	adds r0, #1
	movs r1, #3
	ands r0, r1
	strb r0, [r4, #0x15]
	b _08024B0E
_08024AF8:
	cmp r7, r5
	bge _08024B0E
	ldrb r0, [r4, #0x15]
	subs r0, #1
	movs r1, #3
	ands r0, r1
	strb r0, [r4, #0x15]
	subs r1, r5, #1
	adds r0, r4, #0
	bl sub_8024BAC
_08024B0E:
	str r7, [r4, #0x10]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8024B18
sub_8024B18: @ 0x08024B18
	push {r4, r5, lr}
	ldr r4, [r0, #0xc]
	lsls r4, r4, #4
	subs r1, r1, r4
	ldr r4, [r0, #0x10]
	lsls r4, r4, #3
	subs r2, r2, r4
	ldrb r5, [r0, #0x14]
	lsls r4, r5, #4
	adds r1, r1, r4
	ldrb r5, [r0, #0x15]
	lsls r4, r5, #3
	adds r2, r2, r4
	movs r4, #0x3f
	ands r1, r4
	movs r4, #0x1f
	ands r2, r4
	ldr r0, [r0, #8]
	lsls r1, r1, #1
	adds r0, r0, r1
	str r2, [r3]
	pop {r4, r5}
	pop {r1}
	bx r1

	thumb_func_start sub_8024B48
sub_8024B48: @ 0x08024B48
	push {r4, r5, lr}
	ldr r4, [r0, #0xc]
	lsls r4, r4, #4
	subs r1, r1, r4
	ldr r4, [r0, #0x10]
	lsls r4, r4, #3
	subs r2, r2, r4
	ldrb r5, [r0, #0x14]
	lsls r4, r5, #4
	adds r1, r1, r4
	ldrb r5, [r0, #0x15]
	lsls r4, r5, #3
	adds r2, r2, r4
	movs r4, #0x3f
	ands r1, r4
	movs r4, #0x1f
	ands r2, r4
	ldr r0, [r0, #8]
	lsls r2, r2, #7
	adds r0, r0, r2
	str r1, [r3]
	pop {r4, r5}
	pop {r1}
	bx r1

	thumb_func_start sub_8024B78
sub_8024B78: @ 0x08024B78
	push {r4, lr}
	ldr r3, [r0, #0xc]
	lsls r3, r3, #4
	subs r1, r1, r3
	ldr r3, [r0, #0x10]
	lsls r3, r3, #3
	subs r2, r2, r3
	ldrb r4, [r0, #0x14]
	lsls r3, r4, #4
	adds r1, r1, r3
	ldrb r4, [r0, #0x15]
	lsls r3, r4, #3
	adds r2, r2, r3
	movs r3, #0x3f
	ands r1, r3
	movs r3, #0x1f
	ands r2, r3
	ldr r0, [r0, #8]
	lsls r2, r2, #6
	adds r2, r2, r1
	lsls r2, r2, #1
	adds r0, r0, r2
	ldrh r0, [r0]
	pop {r4}
	pop {r1}
	bx r1

	thumb_func_start sub_8024BAC
sub_8024BAC: @ 0x08024BAC
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r0, [r4]
	ldrh r2, [r0, #0x18]
	cmp r1, r2
	bge _08024C00
	ldrh r0, [r0, #0x16]
	adds r5, r0, #0
	muls r5, r1, r5
	ldr r0, [r4, #0xc]
	adds r5, r5, r0
	movs r6, #0
	movs r7, #3
_08024BC6:
	ldr r0, [r4, #0xc]
	adds r0, r6, r0
	ldr r1, [r4]
	ldrh r3, [r1, #0x16]
	cmp r0, r3
	bge _08024BFA
	ldr r2, [r4, #8]
	ldrb r0, [r4, #0x15]
	adds r0, #4
	ands r0, r7
	lsls r0, r0, #0xa
	adds r2, r2, r0
	ldrb r3, [r4, #0x14]
	adds r0, r3, r6
	adds r0, #4
	ands r0, r7
	lsls r0, r0, #5
	adds r2, r2, r0
	ldr r1, [r1]
	lsls r0, r5, #1
	adds r0, r0, r1
	ldrh r1, [r0]
	adds r5, #1
	adds r0, r4, #0
	bl sub_8024960
_08024BFA:
	adds r6, #1
	cmp r6, #3
	ble _08024BC6
_08024C00:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8024C08
sub_8024C08: @ 0x08024C08
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r0, [r4]
	ldrh r2, [r0, #0x16]
	cmp r1, r2
	bge _08024C5C
	ldr r0, [r4, #0x10]
	adds r5, r0, #0
	muls r5, r2, r5
	adds r5, r5, r1
	movs r6, #0
	movs r7, #3
_08024C20:
	ldr r0, [r4, #0x10]
	adds r0, r6, r0
	ldr r3, [r4]
	ldrh r1, [r3, #0x18]
	cmp r0, r1
	bge _08024C56
	ldr r2, [r4, #8]
	ldrb r1, [r4, #0x15]
	adds r0, r1, r6
	adds r0, #4
	ands r0, r7
	lsls r0, r0, #0xa
	adds r2, r2, r0
	ldrb r0, [r4, #0x14]
	adds r0, #4
	ands r0, r7
	lsls r0, r0, #5
	adds r2, r2, r0
	ldr r1, [r3]
	lsls r0, r5, #1
	adds r0, r0, r1
	ldrh r1, [r0]
	ldrh r3, [r3, #0x16]
	adds r5, r3, r5
	adds r0, r4, #0
	bl sub_8024960
_08024C56:
	adds r6, #1
	cmp r6, #3
	ble _08024C20
_08024C5C:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8024C64
sub_8024C64: @ 0x08024C64
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	adds r4, r0, #0
	movs r0, #0x40
	mov sl, r0
	movs r2, #0x20
	mov sb, r2
	movs r0, #0
	strb r0, [r4, #0x14]
	strb r0, [r4, #0x15]
	ldr r0, [r1]
	asrs r0, r0, #7
	str r0, [r4, #0xc]
	ldr r0, [r1, #4]
	asrs r0, r0, #6
	str r0, [r4, #0x10]
	movs r6, #0
_08024C8C:
	ldr r0, [r4, #0x10]
	adds r0, r6, r0
	ldr r1, [r4]
	adds r3, r6, #1
	mov r8, r3
	ldrh r1, [r1, #0x18]
	cmp r0, r1
	bge _08024CDA
	movs r5, #0
	lsls r0, r6, #3
	mov r7, sl
	muls r7, r0, r7
_08024CA4:
	ldr r0, [r4, #0xc]
	adds r3, r5, r0
	ldr r1, [r4]
	ldrh r2, [r1, #0x16]
	cmp r3, r2
	bge _08024CD4
	ldr r0, [r4, #0x10]
	adds r0, r0, r6
	muls r0, r2, r0
	adds r0, r0, r3
	ldr r1, [r1]
	lsls r0, r0, #1
	adds r0, r0, r1
	ldrh r1, [r0]
	ldr r2, [r4, #8]
	mov r3, sb
	asrs r0, r3, #1
	muls r0, r5, r0
	adds r0, r7, r0
	lsls r0, r0, #1
	adds r2, r2, r0
	adds r0, r4, #0
	bl sub_8024960
_08024CD4:
	adds r5, #1
	cmp r5, #3
	ble _08024CA4
_08024CDA:
	mov r6, r8
	cmp r6, #3
	ble _08024C8C
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8024CF0
sub_8024CF0: @ 0x08024CF0
	str r1, [r0]
	ldrh r2, [r1, #0x1a]
	ldrh r3, [r1, #0x1c]
	str r2, [r0, #0x18]
	str r3, [r0, #0x1c]
	ldr r2, _08024D08 @ =gUnknown_03001308
	ldr r2, [r2]
	ldr r2, [r2, #0x24]
	ldr r1, [r1, #4]
	adds r2, r2, r1
	str r2, [r0, #4]
	bx lr
	.align 2, 0
_08024D08: .4byte gUnknown_03001308

	thumb_func_start sub_8024D0C
sub_8024D0C: @ 0x08024D0C
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, _08024D34 @ =gStaticData_087E4BDC
	str r0, [r4, #0x20]
	ldr r0, [r4, #8]
	cmp r0, #0
	beq _08024D20
	bl sub_8026EB4
_08024D20:
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _08024D2E
	adds r0, r4, #0
	bl sub_8026ED0
_08024D2E:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08024D34: .4byte gStaticData_087E4BDC

	thumb_func_start sub_8024D38
sub_8024D38: @ 0x08024D38
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, _08024D54 @ =gStaticData_087E4BDC
	str r0, [r4, #0x20]
	movs r0, #0x80
	lsls r0, r0, #5
	bl sub_8026EC0
	str r0, [r4, #8]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08024D54: .4byte gStaticData_087E4BDC

	thumb_func_start sub_8024D58
sub_8024D58: @ 0x08024D58
	ldr r0, [r0, #0x1c]
	bx lr

	thumb_func_start sub_8024D5C
sub_8024D5C: @ 0x08024D5C
	ldr r0, [r0, #0x18]
	bx lr

	thumb_func_start sub_8024D60
sub_8024D60: @ 0x08024D60
	ldr r2, [r1, #4]
	ldr r1, [r1]
	str r1, [r0, #0x18]
	str r2, [r0, #0x1c]
	bx lr
	.align 2, 0

	thumb_func_start sub_8024D6C
sub_8024D6C: @ 0x08024D6C
	str r1, [r0, #0x18]
	str r2, [r0, #0x1c]
	bx lr
	.align 2, 0

	thumb_func_start sub_8024D74
sub_8024D74: @ 0x08024D74
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, _08024DA8 @ =gStaticData_087E4BEC
	str r0, [r4, #0x30]
	ldr r2, [r4, #0x2c]
	cmp r2, #0
	beq _08024D94
	ldr r1, [r2, #0x20]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_08024D94:
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _08024DA2
	adds r0, r4, #0
	bl sub_8026ED0
_08024DA2:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08024DA8: .4byte gStaticData_087E4BEC

	thumb_func_start sub_8024DAC
sub_8024DAC: @ 0x08024DAC
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, _08024DC8 @ =gStaticData_087E4BEC
	str r0, [r4, #0x30]
	movs r0, #0x24
	bl sub_8026EDC
	bl sub_8024D38
	str r0, [r4, #0x2c]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08024DC8: .4byte gStaticData_087E4BEC

	thumb_func_start sub_8024DCC
sub_8024DCC: @ 0x08024DCC
	adds r0, r1, #0
	movs r1, #0x10
	rsbs r1, r1, #0
	cmp r0, r1
	bge _08024DD8
	adds r0, r1, #0
_08024DD8:
	cmp r0, #0x10
	ble _08024DDE
	movs r0, #0x10
_08024DDE:
	bx lr

	thumb_func_start sub_8024DE0
sub_8024DE0: @ 0x08024DE0
	ldr r2, [r0, #8]
	ldr r3, [r1]
	cmp r2, r3
	ble _08024DEA
	adds r2, r3, #0
_08024DEA:
	str r2, [r1]
	ldr r0, [r0, #0xc]
	ldr r2, [r1, #4]
	cmp r0, r2
	ble _08024DF6
	adds r0, r2, #0
_08024DF6:
	str r0, [r1, #4]
	bx lr
	.align 2, 0

	thumb_func_start sub_8024DFC
sub_8024DFC: @ 0x08024DFC
	adds r3, r0, #0
	adds r2, r1, #0
	ldr r1, [r2]
	ldr r0, [r3, #0x20]
	muls r0, r1, r0
	cmp r0, #0
	bge _08024E0C
	adds r0, #0xff
_08024E0C:
	asrs r0, r0, #8
	str r0, [r2]
	ldr r1, [r2, #4]
	ldr r0, [r3, #0x24]
	muls r0, r1, r0
	cmp r0, #0
	bge _08024E1C
	adds r0, #0xff
_08024E1C:
	asrs r0, r0, #8
	str r0, [r2, #4]
	bx lr
	.align 2, 0

	thumb_func_start sub_8024E24
sub_8024E24: @ 0x08024E24
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r3, [r4, #0x30]
	movs r1, #0x20
	ldrsh r0, [r3, r1]
	adds r0, r4, r0
	ldr r1, [r5]
	ldr r2, [r4]
	subs r1, r1, r2
	ldr r2, [r3, #0x24]
	bl sub_803AD80
	adds r6, r0, #0
	ldr r3, [r4, #0x30]
	movs r1, #0x20
	ldrsh r0, [r3, r1]
	adds r0, r4, r0
	ldr r1, [r5, #4]
	ldr r2, [r4, #4]
	subs r1, r1, r2
	ldr r2, [r3, #0x24]
	bl sub_803AD80
	ldr r1, [r4]
	adds r1, r1, r6
	str r1, [r4]
	ldr r1, [r4, #4]
	adds r1, r1, r0
	str r1, [r4, #4]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

