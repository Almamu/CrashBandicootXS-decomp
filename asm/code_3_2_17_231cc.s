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

	thumb_func_start sub_8024E68
sub_8024E68: @ 0x08024E68
	push {r4, lr}
	sub sp, #8
	adds r4, r0, #0
	ldr r0, [r1]
	ldr r1, [r1, #4]
	str r0, [sp]
	str r1, [sp, #4]
	adds r0, r4, #0
	mov r1, sp
	bl sub_8024DFC
	adds r0, r4, #0
	mov r1, sp
	bl sub_8024E24
	add sp, #8
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8024E90
sub_8024E90: @ 0x08024E90
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r1]
	ldr r1, [r1, #4]
	str r0, [r4]
	str r1, [r4, #4]
	adds r0, r4, #0
	adds r1, r4, #0
	bl sub_8024DFC
	ldr r0, [r4, #0x2c]
	adds r1, r4, #0
	bl sub_8024C64
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8024EB4
sub_8024EB4: @ 0x08024EB4
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r2, r1, #0
	adds r5, r4, #0
	adds r5, #0x28
	movs r3, #0
	strb r3, [r5]
	cmp r2, #0
	beq _08024EFE
	ldrh r0, [r2, #0x1a]
	str r0, [r4, #0x18]
	ldrh r1, [r2, #0x1c]
	str r1, [r4, #0x1c]
	lsls r0, r0, #3
	str r0, [r4, #0x10]
	lsls r1, r1, #3
	str r1, [r4, #0x14]
	subs r0, #0xf0
	str r0, [r4, #8]
	subs r1, #0xa0
	str r1, [r4, #0xc]
	ldr r0, [r2, #0xc]
	str r0, [r4, #0x20]
	ldr r0, [r2, #0x10]
	str r0, [r4, #0x24]
	str r3, [r4]
	str r3, [r4, #4]
	ldr r0, [r4, #0x2c]
	adds r1, r2, #0
	bl sub_8024CF0
	ldr r0, [r4, #0x2c]
	adds r1, r4, #0
	bl sub_8024C64
	movs r0, #1
	strb r0, [r5]
_08024EFE:
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_8024F04
sub_8024F04: @ 0x08024F04
	adds r0, #0x28
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8024F0C
sub_8024F0C: @ 0x08024F0C
	ldr r0, [r0, #4]
	bx lr

	thumb_func_start sub_8024F10
sub_8024F10: @ 0x08024F10
	ldr r0, [r0]
	bx lr

	thumb_func_start sub_8024F14
sub_8024F14: @ 0x08024F14
	ldr r0, [r0, #0x1c]
	bx lr

	thumb_func_start sub_8024F18
sub_8024F18: @ 0x08024F18
	ldr r0, [r0, #0x18]
	bx lr

	thumb_func_start sub_8024F1C
sub_8024F1C: @ 0x08024F1C
	ldr r0, [r0, #0x14]
	bx lr

	thumb_func_start sub_8024F20
sub_8024F20: @ 0x08024F20
	ldr r0, [r0, #0x10]
	bx lr

	thumb_func_start sub_8024F24
sub_8024F24: @ 0x08024F24
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r7, r0, #0
	adds r3, r1, #0
	movs r1, #0x81
	lsls r1, r1, #5
	adds r0, r7, r1
	ldr r0, [r0]
	cmp r3, r0
	bne _08024F42
	adds r0, r7, #0
	adds r0, #0x20
	b _080250AE
_08024F42:
	ldr r1, _08024F54 @ =0x00001024
	adds r0, r7, r1
	ldr r0, [r0]
	cmp r3, r0
	bne _08024F58
	movs r1, #0x90
	lsls r1, r1, #1
	b _080250AC
	.align 2, 0
_08024F54: .4byte 0x00001024
_08024F58:
	ldr r1, _08024F68 @ =0x00001028
	adds r0, r7, r1
	ldr r0, [r0]
	cmp r3, r0
	bne _08024F6C
	movs r1, #0x88
	lsls r1, r1, #2
	b _080250AC
	.align 2, 0
_08024F68: .4byte 0x00001028
_08024F6C:
	ldr r1, _08024F7C @ =0x0000102C
	adds r0, r7, r1
	ldr r0, [r0]
	cmp r3, r0
	bne _08024F80
	movs r1, #0xc8
	lsls r1, r1, #2
	b _080250AC
	.align 2, 0
_08024F7C: .4byte 0x0000102C
_08024F80:
	ldr r1, _08024F90 @ =0x00001030
	adds r0, r7, r1
	ldr r0, [r0]
	cmp r3, r0
	bne _08024F94
	movs r1, #0x84
	lsls r1, r1, #3
	b _080250AC
	.align 2, 0
_08024F90: .4byte 0x00001030
_08024F94:
	ldr r1, _08024FA4 @ =0x00001034
	adds r0, r7, r1
	ldr r0, [r0]
	cmp r3, r0
	bne _08024FA8
	movs r1, #0xa4
	lsls r1, r1, #3
	b _080250AC
	.align 2, 0
_08024FA4: .4byte 0x00001034
_08024FA8:
	ldr r1, _08024FB8 @ =0x00001038
	adds r0, r7, r1
	ldr r0, [r0]
	cmp r3, r0
	bne _08024FBC
	movs r1, #0xc4
	lsls r1, r1, #3
	b _080250AC
	.align 2, 0
_08024FB8: .4byte 0x00001038
_08024FBC:
	ldr r1, _08024FCC @ =0x0000103C
	adds r0, r7, r1
	ldr r0, [r0]
	cmp r3, r0
	bne _08024FD0
	movs r1, #0xe4
	lsls r1, r1, #3
	b _080250AC
	.align 2, 0
_08024FCC: .4byte 0x0000103C
_08024FD0:
	movs r1, #0x82
	lsls r1, r1, #5
	adds r0, r7, r1
	ldr r0, [r0]
	cmp r3, r0
	bne _08024FE2
	movs r1, #0x82
	lsls r1, r1, #4
	b _080250AC
_08024FE2:
	ldr r1, _08024FF4 @ =0x00001044
	adds r0, r7, r1
	ldr r0, [r0]
	cmp r3, r0
	bne _08024FF8
	movs r1, #0x92
	lsls r1, r1, #4
	b _080250AC
	.align 2, 0
_08024FF4: .4byte 0x00001044
_08024FF8:
	ldr r1, _08025008 @ =0x00001048
	adds r0, r7, r1
	ldr r0, [r0]
	cmp r3, r0
	bne _0802500C
	movs r1, #0xa2
	lsls r1, r1, #4
	b _080250AC
	.align 2, 0
_08025008: .4byte 0x00001048
_0802500C:
	ldr r1, _0802501C @ =0x0000104C
	adds r0, r7, r1
	ldr r0, [r0]
	cmp r3, r0
	bne _08025020
	movs r1, #0xb2
	lsls r1, r1, #4
	b _080250AC
	.align 2, 0
_0802501C: .4byte 0x0000104C
_08025020:
	ldr r1, _08025030 @ =0x00001050
	adds r0, r7, r1
	ldr r0, [r0]
	cmp r3, r0
	bne _08025034
	movs r1, #0xc2
	lsls r1, r1, #4
	b _080250AC
	.align 2, 0
_08025030: .4byte 0x00001050
_08025034:
	ldr r1, _08025044 @ =0x00001054
	adds r0, r7, r1
	ldr r0, [r0]
	cmp r3, r0
	bne _08025048
	movs r1, #0xd2
	lsls r1, r1, #4
	b _080250AC
	.align 2, 0
_08025044: .4byte 0x00001054
_08025048:
	ldr r1, _08025058 @ =0x00001058
	adds r0, r7, r1
	ldr r0, [r0]
	cmp r3, r0
	bne _0802505C
	movs r1, #0xe2
	lsls r1, r1, #4
	b _080250AC
	.align 2, 0
_08025058: .4byte 0x00001058
_0802505C:
	ldr r1, _080250A4 @ =0x0000105C
	adds r0, r7, r1
	ldr r0, [r0]
	cmp r3, r0
	beq _080250A8
	movs r0, #0x83
	lsls r0, r0, #5
	adds r6, r7, r0
	ldr r4, [r6]
	adds r4, #0xf
	movs r1, #0xf
	mov r8, r1
	ands r4, r1
	lsls r5, r4, #8
	adds r5, #0x20
	adds r5, r7, r5
	adds r0, r7, #0
	adds r1, r3, #0
	adds r2, r5, #0
	str r3, [sp]
	bl sub_8025334
	lsls r4, r4, #2
	movs r1, #0x81
	lsls r1, r1, #5
	adds r0, r7, r1
	adds r0, r0, r4
	ldr r3, [sp]
	str r3, [r0]
	ldr r0, [r6]
	adds r0, #1
	mov r1, r8
	ands r0, r1
	str r0, [r6]
	adds r0, r5, #0
	b _080250AE
	.align 2, 0
_080250A4: .4byte 0x0000105C
_080250A8:
	movs r1, #0xf2
	lsls r1, r1, #4
_080250AC:
	adds r0, r7, r1
_080250AE:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_80250BC
sub_80250BC: @ 0x080250BC
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r4, r1, #0
	adds r6, r2, #0
	mov r7, sp
	cmp r4, #0
	blt _08025116
	cmp r6, #0
	blt _08025116
	asrs r3, r4, #4
	asrs r1, r6, #3
	ldr r2, [r5]
	ldr r0, [r5, #0x18]
	muls r0, r1, r0
	adds r0, r0, r3
	ldr r1, [r2]
	lsls r0, r0, #1
	adds r0, r0, r1
	ldrh r1, [r0]
	adds r0, r5, #0
	bl sub_8024F24
	movs r1, #7
	ands r1, r6
	movs r2, #0xf
	ands r4, r2
	lsls r1, r1, #4
	adds r1, r1, r4
	lsls r1, r1, #1
	adds r1, r1, r0
	ldrh r1, [r1]
	lsls r0, r1, #0x10
	lsrs r3, r0, #0x10
	lsrs r0, r0, #0x18
	ands r0, r2
	cmp r0, #0
	beq _0802510A
	strb r0, [r7]
_0802510A:
	movs r1, #0xff
	ands r1, r3
	cmp r1, #0
	beq _08025116
	cmp r1, #0x23
	ble _0802511A
_08025116:
	movs r0, #0
	b _08025124
_0802511A:
	lsls r0, r1, #3
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0802512C @ =gStaticData_081725AC
	adds r0, r0, r1
_08025124:
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0802512C: .4byte gStaticData_081725AC

	thumb_func_start sub_8025130
sub_8025130: @ 0x08025130
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	adds r4, r1, #0
	adds r6, r2, #0
	adds r7, r3, #0
	movs r0, #0
	mov r8, r0
	movs r3, #0
	cmp r4, #0
	blt _0802514C
	cmp r6, #0
	bge _08025150
_0802514C:
	movs r1, #0
	b _08025192
_08025150:
	asrs r3, r4, #4
	asrs r1, r6, #3
	ldr r2, [r5]
	ldr r0, [r5, #0x18]
	muls r0, r1, r0
	adds r0, r0, r3
	ldr r1, [r2]
	lsls r0, r0, #1
	adds r0, r0, r1
	ldrh r1, [r0]
	adds r0, r5, #0
	bl sub_8024F24
	movs r1, #7
	ands r1, r6
	movs r2, #0xf
	ands r4, r2
	lsls r1, r1, #4
	adds r1, r1, r4
	lsls r1, r1, #1
	adds r1, r1, r0
	ldrh r1, [r1]
	lsls r0, r1, #0x10
	lsrs r4, r0, #0x10
	lsrs r3, r0, #0x1c
	lsrs r1, r0, #0x18
	ands r1, r2
	cmp r1, #0
	beq _0802518E
	ldr r0, [sp, #0x18]
	strb r1, [r0]
_0802518E:
	movs r1, #0xff
	ands r1, r4
_08025192:
	cmp r1, #0x23
	bgt _0802519A
	movs r0, #0
	b _0802521A
_0802519A:
	cmp r7, #1
	beq _080251CC
	cmp r7, #1
	bgt _080251A8
	cmp r7, #0
	beq _080251B2
	b _08025218
_080251A8:
	cmp r7, #2
	beq _080251E4
	cmp r7, #3
	beq _08025200
	b _08025218
_080251B2:
	movs r0, #4
	ands r3, r0
	cmp r3, #0
	beq _080251BE
	movs r0, #0
	b _08025216
_080251BE:
	lsls r0, r1, #3
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _080251C8 @ =gStaticData_081725AC
	b _08025214
	.align 2, 0
_080251C8: .4byte gStaticData_081725AC
_080251CC:
	ands r3, r7
	cmp r3, #0
	beq _080251D6
	movs r0, #0
	b _08025216
_080251D6:
	lsls r0, r1, #3
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _080251E0 @ =gStaticData_081725B4
	b _08025214
	.align 2, 0
_080251E0: .4byte gStaticData_081725B4
_080251E4:
	movs r0, #8
	ands r3, r0
	cmp r3, #0
	beq _080251F0
	movs r0, #0
	b _08025216
_080251F0:
	lsls r0, r1, #3
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _080251FC @ =gStaticData_081725BC
	b _08025214
	.align 2, 0
_080251FC: .4byte gStaticData_081725BC
_08025200:
	movs r0, #2
	ands r3, r0
	cmp r3, #0
	beq _0802520C
	movs r0, #0
	b _08025216
_0802520C:
	lsls r0, r1, #3
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _08025224 @ =gStaticData_081725C4
_08025214:
	adds r0, r0, r1
_08025216:
	mov r8, r0
_08025218:
	mov r0, r8
_0802521A:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08025224: .4byte gStaticData_081725C4

	thumb_func_start sub_8025228
sub_8025228: @ 0x08025228
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	adds r4, r1, #0
	adds r6, r2, #0
	adds r7, r3, #0
	movs r0, #0
	mov r8, r0
	movs r3, #0
	cmp r4, #0
	blt _08025244
	cmp r6, #0
	bge _08025248
_08025244:
	movs r2, #0
	b _0802528A
_08025248:
	asrs r3, r4, #4
	asrs r1, r6, #3
	ldr r2, [r5]
	ldr r0, [r5, #0x18]
	muls r0, r1, r0
	adds r0, r0, r3
	ldr r1, [r2]
	lsls r0, r0, #1
	adds r0, r0, r1
	ldrh r1, [r0]
	adds r0, r5, #0
	bl sub_8024F24
	movs r1, #7
	ands r1, r6
	movs r2, #0xf
	ands r4, r2
	lsls r1, r1, #4
	adds r1, r1, r4
	lsls r1, r1, #1
	adds r1, r1, r0
	ldrh r1, [r1]
	lsls r0, r1, #0x10
	lsrs r4, r0, #0x10
	lsrs r3, r0, #0x1c
	lsrs r1, r0, #0x18
	ands r1, r2
	cmp r1, #0
	beq _08025286
	ldr r0, [sp, #0x18]
	strb r1, [r0]
_08025286:
	movs r2, #0xff
	ands r2, r4
_0802528A:
	cmp r2, #0x23
	bgt _08025294
	movs r0, #1
	rsbs r0, r0, #0
	b _08025324
_08025294:
	cmp r7, #1
	beq _080252C8
	cmp r7, #1
	bgt _080252A2
	cmp r7, #0
	beq _080252AC
	b _0802531E
_080252A2:
	cmp r7, #2
	beq _080252E4
	cmp r7, #3
	beq _08025304
	b _0802531E
_080252AC:
	movs r0, #4
	ands r3, r0
	cmp r3, #0
	bne _080252EC
	ldr r1, _080252C4 @ =gStaticData_081725A8
	lsls r0, r2, #3
	adds r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0]
	b _0802531C
	.align 2, 0
_080252C4: .4byte gStaticData_081725A8
_080252C8:
	ands r3, r7
	cmp r3, #0
	beq _080252D2
	movs r0, #0
	b _0802531C
_080252D2:
	ldr r1, _080252E0 @ =gStaticData_081725A8
	lsls r0, r2, #3
	adds r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #1]
	b _0802531C
	.align 2, 0
_080252E0: .4byte gStaticData_081725A8
_080252E4:
	movs r0, #8
	ands r3, r0
	cmp r3, #0
	beq _080252F2
_080252EC:
	movs r1, #0
	mov r8, r1
	b _0802531E
_080252F2:
	ldr r1, _08025300 @ =gStaticData_081725A8
	lsls r0, r2, #3
	adds r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #2]
	b _0802531C
	.align 2, 0
_08025300: .4byte gStaticData_081725A8
_08025304:
	movs r0, #2
	ands r3, r0
	cmp r3, #0
	beq _08025310
	movs r0, #0
	b _0802531C
_08025310:
	ldr r1, _08025330 @ =gStaticData_081725A8
	lsls r0, r2, #3
	adds r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #3]
_0802531C:
	mov r8, r0
_0802531E:
	mov r1, r8
	lsls r0, r1, #0x18
	asrs r0, r0, #0x18
_08025324:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08025330: .4byte gStaticData_081725A8

	thumb_func_start sub_8025334
sub_8025334: @ 0x08025334
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r2
	ldr r6, [r0, #4]
	lsls r1, r1, #1
	adds r1, r1, r6
	ldrh r1, [r1]
	lsls r0, r1, #2
	adds r6, r6, r0
	movs r0, #0x7f
	mov sb, r0
	movs r1, #0
	mov ip, r1
	mov r7, r8
_08025354:
	ldrh r1, [r6]
	ldrb r3, [r6]
	adds r6, #2
	movs r0, #0x80
	lsls r0, r0, #8
	ands r0, r1
	cmp r0, #0
	beq _0802538C
	ldrh r2, [r6]
	adds r6, #2
	mov r4, sb
	subs r4, r4, r3
	mov sb, r4
	mov r1, ip
	lsls r0, r1, #1
	mov r4, r8
	adds r1, r0, r4
_08025376:
	strh r2, [r1]
	adds r1, #2
	adds r7, #2
	movs r0, #1
	add ip, r0
	subs r0, r3, #1
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	cmp r3, #0
	bne _08025376
	b _08025430
_0802538C:
	movs r0, #0x80
	lsls r0, r0, #7
	ands r1, r0
	cmp r1, #0
	beq _0802540A
	mov r1, sb
	subs r1, r1, r3
	mov sb, r1
	ldrh r4, [r6]
	adds r6, #2
	strh r4, [r7]
	subs r0, r3, #1
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	adds r7, #2
	movs r2, #1
	add ip, r2
	mov r1, ip
	lsls r0, r1, #1
	mov r2, r8
	adds r5, r0, r2
_080253B6:
	ldrh r2, [r6]
	adds r6, #2
	lsls r1, r2, #0x18
	lsls r0, r4, #0x10
	asrs r0, r0, #0x10
	asrs r1, r1, #0x18
	adds r0, r0, r1
	lsls r0, r0, #0x10
	lsrs r4, r0, #0x10
	strh r4, [r5]
	adds r5, #2
	lsls r2, r2, #0x10
	lsls r0, r4, #0x10
	asrs r0, r0, #0x10
	asrs r2, r2, #0x18
	adds r0, r0, r2
	lsls r0, r0, #0x10
	lsrs r4, r0, #0x10
	strh r4, [r5]
	adds r5, #2
	adds r7, #4
	movs r0, #2
	add ip, r0
	subs r0, r3, #2
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	cmp r3, #1
	bhi _080253B6
	cmp r3, #0
	beq _08025430
	ldrh r1, [r6]
	adds r6, #2
	lsls r1, r1, #0x18
	lsls r0, r4, #0x10
	asrs r0, r0, #0x10
	asrs r1, r1, #0x18
	adds r0, r0, r1
	strh r0, [r7]
	adds r7, #2
	movs r1, #1
	add ip, r1
	b _08025430
_0802540A:
	mov r2, sb
	subs r2, r2, r3
	mov sb, r2
	mov r4, ip
	lsls r0, r4, #1
	mov r2, r8
	adds r1, r0, r2
_08025418:
	ldrh r0, [r6]
	strh r0, [r1]
	adds r6, #2
	adds r1, #2
	adds r7, #2
	movs r4, #1
	add ip, r4
	subs r0, r3, #1
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	cmp r3, #0
	bne _08025418
_08025430:
	mov r0, sb
	cmp r0, #0
	bge _08025354
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8025444
sub_8025444: @ 0x08025444
	push {lr}
	adds r2, r0, #0
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08025456
	adds r0, r2, #0
	bl sub_8026ED0
_08025456:
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start nullsub_4
nullsub_4: @ 0x0802545C
	bx lr
	.align 2, 0

	thumb_func_start sub_8025460
sub_8025460: @ 0x08025460
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	adds r4, r1, #0
	adds r6, r2, #0
	adds r7, r3, #0
	cmp r4, #0
	blt _08025472
	cmp r6, #0
	bge _08025476
_08025472:
	movs r0, #0
	b _080254BA
_08025476:
	asrs r3, r4, #4
	asrs r1, r6, #3
	ldr r2, [r5]
	ldr r0, [r5, #0x18]
	muls r0, r1, r0
	adds r0, r0, r3
	ldr r1, [r2]
	lsls r0, r0, #1
	adds r0, r0, r1
	ldrh r1, [r0]
	adds r0, r5, #0
	bl sub_8024F24
	movs r1, #7
	ands r1, r6
	movs r3, #0xf
	ands r4, r3
	lsls r1, r1, #4
	adds r1, r1, r4
	lsls r1, r1, #1
	adds r1, r1, r0
	ldrh r1, [r1]
	lsls r1, r1, #0x10
	lsrs r4, r1, #0x10
	lsrs r2, r1, #0x1c
	ldr r0, [sp, #0x14]
	str r2, [r0]
	lsrs r1, r1, #0x18
	ands r1, r3
	cmp r1, #0
	beq _080254B6
	strb r1, [r7]
_080254B6:
	movs r0, #0xff
	ands r0, r4
_080254BA:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_80254C0
sub_80254C0: @ 0x080254C0
	push {r4, r5, r6, lr}
	adds r6, r1, #0
	adds r5, r2, #0
	asrs r4, r6, #4
	asrs r2, r5, #3
	ldr r3, [r0]
	ldr r1, [r0, #0x18]
	muls r1, r2, r1
	adds r1, r1, r4
	ldr r2, [r3]
	lsls r1, r1, #1
	adds r1, r1, r2
	ldrh r1, [r1]
	bl sub_8024F24
	movs r1, #7
	ands r1, r5
	movs r2, #0xf
	ands r2, r6
	lsls r1, r1, #4
	adds r1, r1, r2
	lsls r1, r1, #1
	adds r1, r1, r0
	ldrh r0, [r1]
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_80254F8
sub_80254F8: @ 0x080254F8
	push {r4, lr}
	adds r3, r0, #0
	adds r2, r1, #0
	cmp r2, #0
	beq _08025546
	str r2, [r3]
	ldr r0, _0802554C @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x24]
	ldr r1, [r2, #4]
	adds r0, r0, r1
	str r0, [r3, #4]
	ldrh r0, [r2, #0x1a]
	str r0, [r3, #0x10]
	ldrh r1, [r2, #0x1c]
	str r1, [r3, #0x14]
	lsls r0, r0, #3
	str r0, [r3, #8]
	lsls r1, r1, #3
	str r1, [r3, #0xc]
	ldrh r0, [r2, #0x16]
	str r0, [r3, #0x18]
	ldrh r0, [r2, #0x18]
	str r0, [r3, #0x1c]
	movs r2, #1
	rsbs r2, r2, #0
	movs r1, #0xf
	ldr r4, _08025550 @ =0x0000105C
	adds r0, r3, r4
_08025532:
	str r2, [r0]
	subs r0, #4
	subs r1, #1
	cmp r1, #0
	bge _08025532
	movs r1, #0
	movs r2, #0x83
	lsls r2, r2, #5
	adds r0, r3, r2
	str r1, [r0]
_08025546:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0802554C: .4byte gUnknown_03001308
_08025550: .4byte 0x0000105C

	thumb_func_start sub_8025554
sub_8025554: @ 0x08025554
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r2, r1, #0
	movs r5, #0
	cmp r2, #0
	bge _08025562
	adds r1, #0x1f
_08025562:
	asrs r1, r1, #5
	lsls r0, r1, #5
	subs r0, r2, r0
	movs r3, #1
	lsls r3, r0
	lsls r1, r1, #2
	adds r1, r4, r1
	ldr r2, [r1]
	adds r0, r2, #0
	ands r0, r3
	cmp r0, #0
	bne _08025580
	orrs r2, r3
	str r2, [r1]
	movs r5, #1
_08025580:
	adds r0, r5, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_start sub_8025588
sub_8025588: @ 0x08025588
	adds	r3, r0, #0
	adds	r2, r1, #0
	cmp	r2, #0
	bge _08025592
	adds	r1, #31
_08025592:
	asrs	r1, r1, #5
	lsls	r0, r1, #5
	subs	r0, r2, r0
	movs	r2, #1
	lsls	r2, r0
	lsls	r1, r1, #2
	adds	r1, r3, r1
	ldr	r0, [r1, #0]
	bics	r0, r2
	str	r0, [r1, #0]
	bx	lr

	thumb_func_start sub_80255A8
sub_80255A8: @ 0x080255A8
	push {lr}
	sub sp, #4
	adds r1, r0, #0
	movs r0, #0
	str r0, [sp]
	ldr r2, _080255C0 @ =0x05000020
	mov r0, sp
	bl sub_803A94C
	add sp, #4
	pop {r0}
	bx r0
	.align 2, 0
_080255C0: .4byte 0x05000020

	thumb_func_start sub_80255C4
sub_80255C4: @ 0x080255C4
	push {r4, lr}
	adds r4, r0, #0
	bl sub_80255A8
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1

	thumb_func_start sub_80255D4
sub_80255D4: @ 0x080255D4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x14
	adds r6, r0, #0
	mov sb, r2
	adds r5, r3, #0
	movs r3, #0
	ldr r0, [r6]
	cmp r1, r0
	beq _08025616
	str r1, [r6]
	str r3, [sp]
	ldr r0, _080256DC @ =0x040000D4
	mov r1, sp
	str r1, [r0]
	adds r1, r6, #0
	adds r1, #8
	str r1, [r0, #4]
	ldr r2, _080256E0 @ =0x85000010
	str r2, [r0, #8]
	ldr r1, [r0, #8]
	str r3, [sp]
	mov r3, sp
	str r3, [r0]
	movs r4, #0x82
	lsls r4, r4, #2
	adds r1, r6, r4
	str r1, [r0, #4]
	str r2, [r0, #8]
	ldr r0, [r0, #8]
_08025616:
	adds r0, r6, #0
	adds r0, #8
	movs r2, #0x84
	lsls r2, r2, #1
	adds r1, r6, r2
	ldr r4, _080256E4 @ =0x04000040
	adds r2, r4, #0
	bl sub_803A94C
	movs r3, #0x82
	lsls r3, r3, #2
	adds r0, r6, r3
	movs r2, #0xc2
	lsls r2, r2, #2
	adds r1, r6, r2
	adds r2, r4, #0
	bl sub_803A94C
	asrs r0, r5, #8
	str r0, [r6, #4]
	movs r7, #0
	ldr r0, [r6]
	ldrh r2, [r0, #2]
	subs r2, #1
	cmp r2, #0
	blt _0802568C
_0802564A:
	ldr r0, [r6]
	lsls r1, r2, #3
	ldr r0, [r0, #4]
	adds r5, r0, r1
	movs r4, #0
	subs r2, #1
	mov r8, r2
	ldrh r3, [r5, #2]
	cmp r4, r3
	bge _08025686
_0802565E:
	adds r0, r6, #0
	adds r1, r7, #0
	bl sub_8025968
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0802567C
	ldr r0, _080256E8 @ =gUnknown_030012E4
	ldr r0, [r0]
	lsls r1, r4, #3
	ldr r2, [r5, #4]
	adds r2, r2, r1
	adds r1, r7, #0
	bl sub_8025D28
_0802567C:
	adds r7, #1
	adds r4, #1
	ldrh r0, [r5, #2]
	cmp r4, r0
	blt _0802565E
_08025686:
	mov r2, r8
	cmp r2, #0
	bge _0802564A
_0802568C:
	mov r1, sb
	cmp r1, #0
	bne _08025694
	b _08025884
_08025694:
	adds r1, #4
	mov sb, r1
	subs r1, #4
	ldm r1!, {r2}
	mov r8, r2
	ldr r0, _080256EC @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r0, [r0]
	subs r2, r0, #1
	cmp r2, #0
	blt _08025762
_080256AA:
	ldr r0, _080256EC @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r2, #2
	adds r0, r0, r1
	ldr r7, [r0]
	ldrh r4, [r7, #8]
	movs r3, #0
	subs r2, #1
	str r2, [sp, #0x10]
	cmp r3, r8
	bge _0802575C
	mov r1, sb
_080256C4:
	ldr r0, [r1]
	cmp r4, r0
	bne _08025754
	movs r6, #0
	ldr r5, [r1, #4]
	ldr r3, _080256EC @ =gUnknown_0300130C
	mov sl, r3
_080256D2:
	ldr r0, _080256EC @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r0, [r0]
	subs r2, r0, #1
	b _080256F2
	.align 2, 0
_080256DC: .4byte 0x040000D4
_080256E0: .4byte 0x85000010
_080256E4: .4byte 0x04000040
_080256E8: .4byte gUnknown_030012E4
_080256EC: .4byte gUnknown_0300130C
_080256F0:
	subs r2, #1
_080256F2:
	cmp r2, #0
	blt _0802571A
	mov r4, sl
	ldr r0, [r4]
	ldr r1, [r0, #8]
	lsls r0, r2, #2
	adds r0, r0, r1
	ldr r4, [r0]
	ldrh r0, [r4, #8]
	cmp r5, r0
	bne _080256F0
	adds r0, r7, #0
	adds r1, r4, #0
	bl sub_8010714
	adds r0, r4, #0
	adds r1, r7, #0
	bl sub_8010710
	movs r6, #1
_0802571A:
	cmp r6, #0
	bne _0802575C
	movs r3, #1
	movs r2, #0
	cmp r6, r8
	bge _08025748
	mov r1, sb
	ldr r0, [r1]
	cmp r5, r0
	bne _08025732
	ldr r5, [r1, #4]
	b _0802574E
_08025732:
	adds r2, #1
	cmp r2, r8
	bge _08025748
	lsls r0, r2, #3
	mov r4, sb
	adds r1, r0, r4
	ldr r0, [r1]
	cmp r5, r0
	bne _08025732
	ldr r5, [r1, #4]
	movs r3, #0
_08025748:
	cmp r3, #0
	beq _0802574E
	movs r6, #1
_0802574E:
	cmp r6, #0
	beq _080256D2
	b _0802575C
_08025754:
	adds r1, #8
	adds r3, #1
	cmp r3, r8
	blt _080256C4
_0802575C:
	ldr r2, [sp, #0x10]
	cmp r2, #0
	bge _080256AA
_08025762:
	movs r3, #0
	cmp r3, r8
	blt _0802576A
	b _08025884
_0802576A:
	lsls r0, r3, #3
	mov r2, sb
	adds r1, r0, r2
	ldrh r5, [r1]
	movs r7, #0
	movs r4, #0
	ldr r1, _080257AC @ =gUnknown_0300130C
	ldr r1, [r1]
	ldr r2, [r1]
	adds r6, r0, #0
	adds r3, #1
	str r3, [sp, #0xc]
	cmp r7, r2
	bge _08025798
	ldr r1, [r1, #8]
_08025788:
	ldr r0, [r1]
	ldrh r0, [r0, #8]
	cmp r0, r5
	beq _0802587C
	adds r1, #4
	adds r4, #1
	cmp r4, r2
	blt _08025788
_08025798:
	cmp r7, #0
	bne _0802587C
	mov r3, sb
	adds r0, r6, r3
	ldrh r5, [r0, #4]
	movs r6, #0
	ldr r4, _080257AC @ =gUnknown_0300130C
	mov sl, r4
	b _080257B2
	.align 2, 0
_080257AC: .4byte gUnknown_0300130C
_080257B0:
	mov r5, ip
_080257B2:
	movs r7, #1
	movs r0, #0
	mov ip, r0
	movs r2, #0
	mov r1, r8
	cmp r1, #0
	ble _08025800
	mov r1, sb
_080257C2:
	ldr r0, [r1]
	cmp r5, r0
	bne _080257F8
	movs r7, #0
	ldrh r1, [r1, #4]
	mov ip, r1
	movs r3, #0
	ldr r2, _080257F4 @ =gUnknown_0300130C
	ldr r0, [r2]
	ldr r0, [r0]
	cmp r7, r0
	bge _08025800
	mov r4, sl
	ldr r0, [r4]
	ldr r2, [r0]
	ldr r0, [r0, #8]
_080257E2:
	ldr r6, [r0]
	ldrh r1, [r6, #8]
	cmp r1, r5
	beq _08025844
	adds r0, #4
	adds r3, #1
	cmp r3, r2
	blt _080257E2
	b _08025800
	.align 2, 0
_080257F4: .4byte gUnknown_0300130C
_080257F8:
	adds r1, #8
	adds r2, #1
	cmp r2, r8
	blt _080257C2
_08025800:
	movs r0, #0
	cmp r0, #0
	bne _08025844
	cmp r7, #0
	beq _08025838
	movs r4, #0
	ldr r1, _0802582C @ =gUnknown_0300130C
	ldr r0, [r1]
	ldr r0, [r0]
	cmp r4, r0
	bge _08025838
	mov r2, sl
	ldr r0, [r2]
	ldr r3, [r0]
	ldr r2, [r0, #8]
_0802581E:
	ldr r1, [r2]
	ldrh r0, [r1, #8]
	cmp r0, r5
	bne _08025830
	adds r6, r1, #0
	b _08025844
	.align 2, 0
_0802582C: .4byte gUnknown_0300130C
_08025830:
	adds r2, #4
	adds r4, #1
	cmp r4, r3
	blt _0802581E
_08025838:
	movs r3, #0
	cmp r3, #0
	bne _08025844
	cmp r7, #0
	beq _080257B0
	b _0802587C
_08025844:
	cmp r6, #0
	beq _0802587C
	ldr r1, [r6, #0x18]
	movs r4, #0x10
	ldrsh r0, [r1, r4]
	adds r0, r6, r0
	ldr r1, [r1, #0x14]
	bl sub_803AD7C
	ldrb r0, [r0, #5]
	adds r0, #1
	lsls r5, r0, #8
	add r4, sp, #4
_0802585E:
	ldr r0, [r6]
	str r0, [sp, #4]
	ldr r2, [r6, #4]
	adds r2, r2, r5
	str r2, [r4, #4]
	ldr r1, [sp, #4]
	adds r0, r6, #0
	bl sub_8007398
	adds r0, r6, #0
	bl sub_801070C
	adds r6, r0, #0
	cmp r6, #0
	bne _0802585E
_0802587C:
	ldr r3, [sp, #0xc]
	cmp r3, r8
	bge _08025884
	b _0802576A
_08025884:
	add sp, #0x14
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8025894
sub_8025894: @ 0x08025894
	push {r4, r5, r6, lr}
	adds r5, r1, #0
	movs r6, #0
	ldrh r2, [r5, #2]
	subs r2, #1
	cmp r2, #0
	blt _0802593A
_080258A2:
	lsls r1, r2, #3
	ldr r0, [r5, #4]
	adds r4, r0, r1
	movs r3, #0
	subs r2, #1
	b _08025930
_080258AE:
	lsls r1, r3, #3
	ldr r0, [r4, #4]
	adds r1, r0, r1
	ldrh r0, [r1]
	cmp r0, #0x1a
	bne _080258CC
	ldr r0, [r5, #8]
	ldrh r1, [r1, #6]
	lsls r1, r1, #1
	adds r1, r1, r0
	ldr r0, [r5, #0xc]
	ldrh r1, [r1]
	adds r0, r1, r0
	movs r1, #8
	ldrsh r0, [r0, r1]
_080258CC:
	subs r0, #0x15
	cmp r0, #0x12
	bhi _0802592E
	lsls r0, r0, #2
	ldr r1, _080258DC @ =_080258E0
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080258DC: .4byte _080258E0
_080258E0: @ jump table
	.4byte _0802592C @ case 0
	.4byte _0802592C @ case 1
	.4byte _0802592C @ case 2
	.4byte _0802592E @ case 3
	.4byte _0802592C @ case 4
	.4byte _0802592E @ case 5
	.4byte _0802592E @ case 6
	.4byte _0802592E @ case 7
	.4byte _0802592E @ case 8
	.4byte _0802592C @ case 9
	.4byte _0802592C @ case 10
	.4byte _0802592C @ case 11
	.4byte _0802592C @ case 12
	.4byte _0802592C @ case 13
	.4byte _0802592C @ case 14
	.4byte _0802592C @ case 15
	.4byte _0802592C @ case 16
	.4byte _0802592C @ case 17
	.4byte _0802592C @ case 18
_0802592C:
	adds r6, #1
_0802592E:
	adds r3, #1
_08025930:
	ldrh r0, [r4, #2]
	cmp r3, r0
	blt _080258AE
	cmp r2, #0
	bge _080258A2
_0802593A:
	adds r0, r6, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
	thumb_func_start sub_8025944
sub_8025944: @ 0x08025944
	adds	r2, r0, #0
	adds	r3, r1, #0
	adds	r0, r3, #0
	cmp	r3, #0
	bge _08025950
	adds	r0, #31
_08025950:
	asrs	r0, r0, #5
	lsls	r1, r0, #2
	adds	r2, #8
	adds	r2, r2, r1
	lsls	r0, r0, #5
	subs	r0, r3, r0
	movs	r1, #1
	lsls	r1, r0
	ldr	r0, [r2, #0]
	orrs	r0, r1
	str	r0, [r2, #0]
	bx	lr

	thumb_func_start sub_8025968
sub_8025968: @ 0x08025968
	push {r4, lr}
	adds r2, r0, #0
	adds r3, r1, #0
	movs r4, #0
	adds r0, r3, #0
	cmp r3, #0
	bge _08025978
	adds r0, #0x1f
_08025978:
	asrs r0, r0, #5
	lsls r1, r0, #2
	adds r2, #8
	adds r2, r2, r1
	lsls r0, r0, #5
	subs r0, r3, r0
	movs r1, #1
	lsls r1, r0
	ldr r0, [r2]
	ands r0, r1
	cmp r0, #0
	beq _08025992
	movs r4, #1
_08025992:
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_802599C
sub_802599C: @ 0x0802599C
	push {r4, r5, lr}
	adds r2, r0, #0
	adds r3, r1, #0
	movs r4, #0
	adds r0, r3, #0
	cmp r3, #0
	bge _080259AC
	adds r0, #0x1f
_080259AC:
	asrs r0, r0, #5
	lsls r1, r0, #2
	movs r5, #0x82
	lsls r5, r5, #2
	adds r2, r2, r5
	adds r2, r2, r1
	lsls r0, r0, #5
	subs r0, r3, r0
	movs r1, #1
	lsls r1, r0
	ldr r0, [r2]
	ands r0, r1
	cmp r0, #0
	beq _080259CA
	movs r4, #1
_080259CA:
	adds r0, r4, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_80259D4
sub_80259D4: @ 0x080259D4
	mov ip, r0
	adds r2, r1, #0
	adds r0, r2, #0
	cmp r2, #0
	bge _080259E0
	adds r0, #0x1f
_080259E0:
	asrs r0, r0, #5
	lsls r3, r0, #2
	movs r1, #0x82
	lsls r1, r1, #2
	add r1, ip
	adds r1, r1, r3
	lsls r0, r0, #5
	subs r0, r2, r0
	movs r2, #1
	lsls r2, r0
	ldr r0, [r1]
	orrs r0, r2
	str r0, [r1]
	movs r1, #0xc2
	lsls r1, r1, #2
	add r1, ip
	adds r1, r1, r3
	ldr r0, [r1]
	orrs r0, r2
	str r0, [r1]
	bx lr
	.align 2, 0

	thumb_func_start sub_8025A0C
sub_8025A0C: @ 0x08025A0C
	push {r4, lr}
	adds r2, r0, #0
	adds r3, r1, #0
	adds r0, r3, #0
	cmp r3, #0
	bge _08025A1A
	adds r0, #0x1f
_08025A1A:
	asrs r0, r0, #5
	lsls r1, r0, #2
	movs r4, #0xc2
	lsls r4, r4, #2
	adds r2, r2, r4
	adds r2, r2, r1
	lsls r0, r0, #5
	subs r0, r3, r0
	movs r1, #1
	lsls r1, r0
	ldr r0, [r2]
	orrs r0, r1
	str r0, [r2]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
	thumb_func_start sub_8025A3C
sub_8025A3C: @ 0x08025A3C
	asrs r1, r1, #8
	str r1, [r0, #4]
	bx lr

	thumb_func_start sub_8025A44
sub_8025A44: @ 0x08025A44
	push {lr}
	adds r2, r0, #0
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08025A56
	adds r0, r2, #0
	bl sub_8026ED0
_08025A56:
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8025A5C
sub_8025A5C: @ 0x08025A5C
	movs r1, #0
	str r1, [r0]
	str r1, [r0, #4]
	bx lr

	thumb_func_start sub_8025A64
sub_8025A64: @ 0x08025A64
	push {r4, r5, r6, r7, lr}
	adds r6, r3, #0
	add r0, sp, #0x18
	ldrb r7, [r0]
	movs r4, #0
	ldr r0, _08025B00 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r0, #0x8c
	ldrb r5, [r0]
	cmp r5, #0
	bne _08025AF6
	ldr r0, _08025B04 @ =0x0000FFFF
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	movs r3, #0
	bl sub_8011114
	adds r4, r0, #0
	movs r0, #0x10
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	adds r0, r4, #0
	adds r0, #0x49
	strb r6, [r0]
	adds r1, r4, #0
	adds r1, #0x4a
	ldr r0, [sp, #0x14]
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x4b
	strb r5, [r0]
	ldr r0, _08025B08 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r3, #0x8d
	lsls r3, r3, #2
	adds r0, r0, r3
	str r0, [r4, #0x20]
	movs r0, #0xa
	subs r1, #0x1d
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
	cmp r7, #0
	beq _08025AF6
	adds r0, r4, #0
	bl sub_80111B8
_08025AF6:
	adds r0, r4, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08025B00: .4byte gUnknown_030012C0
_08025B04: .4byte 0x0000FFFF
_08025B08: .4byte gUnknown_030012D0

	thumb_func_start sub_8025B0C
sub_8025B0C: @ 0x08025B0C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x28
	mov r8, r3
	ldr r7, [sp, #0x44]
	ldr r6, [sp, #0x48]
	ldr r3, [r6]
	asrs r3, r3, #8
	ldr r5, [r6, #4]
	asrs r5, r5, #8
	adds r4, r6, #0
	adds r4, #0x28
	ldrb r4, [r4]
	lsls r4, r4, #0x1b
	lsrs r4, r4, #0x1f
	str r5, [sp]
	str r4, [sp, #4]
	bl sub_8025BAC
	adds r5, r0, #0
	add r0, sp, #8
	adds r1, r5, #0
	bl sub_8007B98
	ldr r4, [sp, #0x10]
	add r0, sp, #0x18
	adds r1, r6, #0
	bl sub_8007B98
	ldr r0, [sp, #0x20]
	lsrs r1, r4, #0x1f
	adds r4, r4, r1
	asrs r4, r4, #1
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	adds r4, r4, r0
	add r4, r8
	ldr r0, [r5]
	asrs r1, r0, #8
	adds r3, r5, #0
	adds r3, #0x28
	ldrb r2, [r3]
	lsls r0, r2, #0x1b
	adds r2, r1, r4
	cmp r0, #0
	bge _08025B6E
	subs r2, r1, r4
_08025B6E:
	ldr r0, [r5, #4]
	asrs r0, r0, #8
	ldr r1, [sp, #0x40]
	adds r0, r0, r1
	lsls r1, r2, #8
	str r1, [r5]
	lsls r0, r0, #8
	str r0, [r5, #4]
	ldrb r3, [r3]
	lsls r0, r3, #0x1b
	cmp r0, #0
	bge _08025B94
	rsbs r0, r7, #0
	movs r1, #0x40
	str r0, [r5, #0x60]
	str r0, [r5, #0x48]
	str r1, [r5, #0x4c]
	str r0, [r5, #0x50]
	b _08025B9E
_08025B94:
	movs r0, #0x40
	str r7, [r5, #0x60]
	str r7, [r5, #0x48]
	str r0, [r5, #0x4c]
	str r7, [r5, #0x50]
_08025B9E:
	adds r0, r5, #0
	add sp, #0x28
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_8025BAC
sub_8025BAC: @ 0x08025BAC
	push {r4, r5, r6, r7, lr}
	adds r5, r1, #0
	adds r7, r2, #0
	ldr r2, [sp, #0x14]
	ldr r6, [sp, #0x18]
	cmp r3, #0
	bge _08025BBC
	movs r3, #0
_08025BBC:
	ldr r0, _08025C94 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r1, [r0, #0x10]
	ldr r0, [r1, #0x10]
	lsls r4, r0, #8
	asrs r0, r4, #8
	cmp r3, r0
	blt _08025BD0
	lsrs r0, r4, #8
	subs r3, r0, #1
_08025BD0:
	cmp r2, #0
	bge _08025BD6
	movs r2, #0
_08025BD6:
	ldr r0, [r1, #0x14]
	lsls r4, r0, #8
	asrs r0, r4, #8
	cmp r2, r0
	blt _08025BE4
	lsrs r0, r4, #8
	subs r2, r0, #1
_08025BE4:
	ldr r0, _08025C98 @ =0x0000FFFF
	lsls r1, r3, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	movs r3, #0
	bl sub_8009ED0
	adds r4, r0, #0
	rsbs r1, r6, #0
	orrs r1, r6
	adds r2, r4, #0
	adds r2, #0x28
	lsrs r1, r1, #0x1f
	lsls r1, r1, #4
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r3, [r2]
	ands r0, r3
	orrs r0, r1
	strb r0, [r2]
	ldr r0, _08025C9C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	lsls r1, r5, #1
	adds r1, r1, r5
	lsls r1, r1, #2
	ldr r0, [r0]
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	strb r7, [r0]
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
	movs r0, #0x10
	bl sub_8026EDC
	bl sub_800CCE0
	str r0, [r4, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r1, [r4, #0xc]
	ands r0, r1
	movs r1, #3
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _08025CA0 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	adds r0, r4, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08025C94: .4byte gUnknown_03001308
_08025C98: .4byte 0x0000FFFF
_08025C9C: .4byte gUnknown_030012D0
_08025CA0: .4byte gUnknown_030012F0

	thumb_func_start sub_8025CA4
sub_8025CA4: @ 0x08025CA4
	push {r4, r5, r6, r7, lr}
	adds r7, r3, #0
	ldr r5, [sp, #0x14]
	add r0, sp, #0x18
	ldrb r6, [r0]
	movs r4, #0
	ldr r0, _08025CD4 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _08025D1A
	cmp r6, #0
	bne _08025CC4
	cmp r5, #0xff
	bne _08025CDC
_08025CC4:
	ldr r3, _08025CD8 @ =0x0000FFFF
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	adds r0, r3, #0
	b _08025CE8
	.align 2, 0
_08025CD4: .4byte gUnknown_030012C0
_08025CD8: .4byte 0x0000FFFF
_08025CDC:
	ldr r0, _08025D24 @ =0x0000FFFF
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	movs r3, #0
_08025CE8:
	bl sub_801173C
	adds r4, r0, #0
	movs r0, #0x10
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	adds r1, r4, #0
	adds r1, #0x49
	movs r0, #0
	strb r7, [r1]
	adds r1, #1
	strb r5, [r1]
	adds r1, #1
	strb r0, [r1]
	cmp r5, #0xff
	bne _08025D10
	adds r0, r4, #0
	bl sub_801191C
_08025D10:
	cmp r6, #0
	beq _08025D1A
	adds r0, r4, #0
	bl sub_8011870
_08025D1A:
	adds r0, r4, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08025D24: .4byte 0x0000FFFF

	thumb_func_start sub_8025D28
sub_8025D28: @ 0x08025D28
	push {r4, r5, r6, lr}
	adds r6, r1, #0
	ldr r1, [r0]
	ldrh r3, [r2]
	lsls r0, r3, #2
	adds r0, r0, r1
	ldrh r1, [r2, #2]
	ldrh r4, [r2, #4]
	ldrh r3, [r2, #6]
	ldr r5, [r0]
	adds r0, r6, #0
	adds r2, r4, #0
	bl sub_803AD8C
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8025D4C
sub_8025D4C: @ 0x08025D4C
	str r2, [r0, #4]
	str r1, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8025D54
sub_8025D54: @ 0x08025D54
	push {lr}
	adds r2, r0, #0
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08025D66
	adds r0, r2, #0
	bl sub_8026ED0
_08025D66:
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8025D6C
sub_8025D6C: @ 0x08025D6C
	movs r1, #0
	str r1, [r0]
	str r1, [r0, #4]
	bx lr

	thumb_func_start sub_8025D74
sub_8025D74: @ 0x08025D74
	push {r4, r5, lr}
	adds r5, r0, #0
	adds r4, r1, #0
	bl sub_8024DAC
	ldr r0, _08025DDC @ =gStaticData_087E4C14
	str r0, [r5, #0x30]
	adds r1, r4, #0
	adds r1, #0x1c
	lsls r0, r1, #0xb
	movs r2, #0xc0
	lsls r2, r2, #0x13
	adds r0, r0, r2
	str r0, [r5, #0x4c]
	lsls r0, r4, #1
	ldr r3, _08025DE0 @ =0x04000008
	adds r0, r0, r3
	str r0, [r5, #0x38]
	lsls r4, r4, #2
	ldr r0, _08025DE4 @ =0x04000010
	adds r4, r4, r0
	str r4, [r5, #0x58]
	movs r0, #0
	strh r0, [r5, #0x34]
	adds r2, r5, #0
	adds r2, #0x34
	movs r0, #0x7f
	ldrb r3, [r2]
	ands r0, r3
	strb r0, [r2]
	adds r3, r5, #0
	adds r3, #0x35
	movs r0, #0x1f
	ands r1, r0
	movs r0, #0x20
	rsbs r0, r0, #0
	ldrb r4, [r3]
	ands r0, r4
	orrs r0, r1
	strb r0, [r3]
	movs r0, #0xd
	rsbs r0, r0, #0
	ldrb r1, [r2]
	ands r0, r1
	movs r1, #8
	orrs r0, r1
	strb r0, [r2]
	adds r0, r5, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08025DDC: .4byte gStaticData_087E4C14
_08025DE0: .4byte 0x04000008
_08025DE4: .4byte 0x04000010

	thumb_func_start sub_8025DE8
sub_8025DE8: @ 0x08025DE8
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	b _08025E04
_08025DF2:
	subs r1, #1
	str r1, [r4, #0x3c]
	ldr r2, [r4, #0x30]
	movs r3, #0x30
	ldrsh r0, [r2, r3]
	adds r0, r4, r0
	ldr r2, [r2, #0x34]
	bl sub_803AD80
_08025E04:
	ldr r1, [r4, #0x3c]
	cmp r1, r5
	bgt _08025DF2
	b _08025E1E
_08025E0C:
	adds r1, #1
	str r1, [r4, #0x40]
	ldr r2, [r4, #0x30]
	movs r3, #0x30
	ldrsh r0, [r2, r3]
	adds r0, r4, r0
	ldr r2, [r2, #0x34]
	bl sub_803AD80
_08025E1E:
	ldr r1, [r4, #0x40]
	cmp r1, r6
	blt _08025E0C
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8025E2C
sub_8025E2C: @ 0x08025E2C
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	b _08025E48
_08025E36:
	subs r1, #1
	str r1, [r4, #0x44]
	ldr r2, [r4, #0x30]
	movs r3, #0x38
	ldrsh r0, [r2, r3]
	adds r0, r4, r0
	ldr r2, [r2, #0x3c]
	bl sub_803AD80
_08025E48:
	ldr r1, [r4, #0x44]
	cmp r1, r5
	bgt _08025E36
	b _08025E62
_08025E50:
	adds r1, #1
	str r1, [r4, #0x48]
	ldr r2, [r4, #0x30]
	movs r3, #0x38
	ldrsh r0, [r2, r3]
	adds r0, r4, r0
	ldr r2, [r2, #0x3c]
	bl sub_803AD80
_08025E62:
	ldr r1, [r4, #0x48]
	cmp r1, r6
	blt _08025E50
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8025E70
sub_8025E70: @ 0x08025E70
	adds r3, r0, #0
	ldr r0, [r3, #0x44]
	cmp r0, r1
	bge _08025E7A
	str r1, [r3, #0x44]
_08025E7A:
	ldr r0, [r3, #0x48]
	cmp r0, r2
	ble _08025E82
	str r2, [r3, #0x48]
_08025E82:
	bx lr

	thumb_func_start sub_8025E84
sub_8025E84: @ 0x08025E84
	adds r3, r0, #0
	ldr r0, [r3, #0x3c]
	cmp r0, r1
	bge _08025E8E
	str r1, [r3, #0x3c]
_08025E8E:
	ldr r0, [r3, #0x40]
	cmp r0, r2
	ble _08025E96
	str r2, [r3, #0x40]
_08025E96:
	bx lr

	thumb_func_start sub_8025E98
sub_8025E98: @ 0x08025E98
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	bl sub_8024E68
	ldr r0, [r5]
	adds r1, r0, #0
	cmp r0, #0
	bge _08025EAE
	adds r1, r0, #7
_08025EAE:
	asrs r1, r1, #3
	mov r8, r1
	adds r2, r0, #0
	adds r2, #0xef
	cmp r2, #0
	bge _08025EBC
	adds r2, #7
_08025EBC:
	asrs r7, r2, #3
	ldr r0, [r5, #4]
	adds r1, r0, #0
	cmp r0, #0
	bge _08025EC8
	adds r1, r0, #7
_08025EC8:
	asrs r6, r1, #3
	adds r4, r0, #0
	adds r4, #0x9f
	cmp r4, #0
	bge _08025ED4
	adds r4, #7
_08025ED4:
	asrs r4, r4, #3
	ldr r1, [r5, #0x30]
	adds r1, #0x48
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	adds r2, r4, #0
	bl sub_803AD84
	ldr r1, [r5, #0x30]
	adds r1, #0x40
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	mov r1, r8
	adds r2, r7, #0
	bl sub_803AD84
	ldr r0, [r5, #0x2c]
	adds r1, r5, #0
	bl sub_8024AA0
	adds r0, r5, #0
	mov r1, r8
	adds r2, r7, #0
	bl sub_8025E2C
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r4, #0
	bl sub_8025DE8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8025F24
sub_8025F24: @ 0x08025F24
	adds r2, r0, #0
	ldr r1, [r2]
	adds r0, #0x54
	strh r1, [r0]
	ldr r0, [r2, #4]
	adds r1, r2, #0
	adds r1, #0x56
	strh r0, [r1]
	ldr r1, [r2, #0x58]
	ldr r0, [r2, #0x54]
	str r0, [r1]
	bx lr

	thumb_func_start sub_8025F3C
sub_8025F3C: @ 0x08025F3C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r5, r0, #0
	adds r4, r1, #0
	ldr r0, [r5, #0x2c]
	ldr r2, [r5, #0x3c]
	mov r3, sp
	bl sub_8024B18
	mov ip, r0
	ldr r2, [r5, #0x3c]
	adds r0, r2, #0
	cmp r2, #0
	bge _08025F5E
	adds r0, #0x1f
_08025F5E:
	asrs r0, r0, #5
	lsls r0, r0, #5
	subs r3, r2, r0
	adds r0, r4, #0
	cmp r4, #0
	bge _08025F6C
	adds r0, #0x1f
_08025F6C:
	asrs r1, r0, #5
	lsls r0, r1, #5
	subs r1, r4, r0
	lsls r0, r3, #5
	adds r3, r0, r1
	adds r4, r2, #0
	ldr r0, [r5, #0x40]
	cmp r4, r0
	bgt _08025FB6
	ldr r6, [r5, #0x4c]
	ldr r2, [sp]
	movs r1, #0x1f
	mov r8, r1
	adds r5, r0, #0
_08025F88:
	lsls r0, r3, #1
	adds r0, r0, r6
	lsls r1, r2, #7
	add r1, ip
	ldrh r1, [r1]
	strh r1, [r0]
	adds r2, #1
	mov r7, r8
	ands r2, r7
	adds r1, r3, #0
	adds r1, #0x20
	adds r0, r1, #0
	cmp r1, #0
	bge _08025FA8
	ldr r7, _08025FC4 @ =0x0000041F
	adds r0, r3, r7
_08025FA8:
	asrs r3, r0, #0xa
	lsls r0, r3, #0xa
	subs r3, r1, r0
	adds r4, #1
	cmp r4, r5
	ble _08025F88
	str r2, [sp]
_08025FB6:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08025FC4: .4byte 0x0000041F

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

	thumb_func_start sub_8026418
sub_8026418: @ 0x08026418
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, _08026440 @ =gStaticData_087E4C64
	str r0, [r4, #0x30]
	ldr r0, [r4, #0x5c]
	cmp r0, #0
	beq _0802642C
	bl sub_8026ED0
_0802642C:
	ldr r0, _08026444 @ =gStaticData_087E4C14
	str r0, [r4, #0x30]
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_8024D74
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08026440: .4byte gStaticData_087E4C64
_08026444: .4byte gStaticData_087E4C14

	thumb_func_start sub_8026448
sub_8026448: @ 0x08026448
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8025D74
	ldr r0, _08026478 @ =gStaticData_087E4C64
	str r0, [r4, #0x30]
	adds r2, r4, #0
	adds r2, #0x34
	movs r1, #0x80
	movs r0, #0x7f
	ldrb r3, [r2]
	ands r0, r3
	orrs r0, r1
	subs r1, #0x8d
	ands r0, r1
	strb r0, [r2]
	ldr r0, _0802647C @ =0x0000480C
	bl sub_8026EDC
	str r0, [r4, #0x5c]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08026478: .4byte gStaticData_087E4C64
_0802647C: .4byte 0x0000480C

	thumb_func_start sub_8026480
sub_8026480: @ 0x08026480
	adds r0, #0x34
	ldrb r0, [r0]
	lsls r0, r0, #0x1e
	lsrs r0, r0, #0x1e
	bx lr
	.align 2, 0

	thumb_func_start sub_802648C
sub_802648C: @ 0x0802648C
	push {r4, r5, lr}
	adds r3, r0, #0
	ldr r0, _080264EC @ =0x00004808
	adds r1, r3, r0
	movs r0, #0x80
	lsls r0, r0, #2
	str r0, [r1]
	movs r1, #0
	ldr r5, _080264F0 @ =0x000001FF
	ldr r0, _080264EC @ =0x00004808
	adds r2, r3, r0
	ldr r0, _080264F4 @ =0x00004408
	adds r4, r3, r0
_080264A6:
	ldr r0, [r2]
	subs r0, #1
	str r0, [r2]
	lsls r0, r0, #1
	adds r0, r4, r0
	strh r1, [r0]
	adds r1, #1
	cmp r1, r5
	ble _080264A6
	adds r4, r3, #0
	adds r4, #8
	movs r2, #0x80
	lsls r2, r2, #2
	movs r1, #0x81
	lsls r1, r1, #3
	adds r0, r3, r1
	movs r1, #0x80
	lsls r1, r1, #6
_080264CA:
	strh r2, [r0]
	adds r0, #2
	subs r1, #1
	cmp r1, #0
	bne _080264CA
	movs r2, #0
	adds r0, r4, #0
	movs r1, #0x80
	lsls r1, r1, #2
_080264DC:
	strh r2, [r0]
	adds r0, #2
	subs r1, #1
	cmp r1, #0
	bne _080264DC
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080264EC: .4byte 0x00004808
_080264F0: .4byte 0x000001FF
_080264F4: .4byte 0x00004408

	thumb_func_start sub_80264F8
sub_80264F8: @ 0x080264F8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	ldr r0, _08026530 @ =0xFFFF0000
	mov r2, r8
	ands r2, r0
	orrs r2, r1
	mov r8, r2
	lsls r0, r2, #0x12
	lsrs r0, r0, #0x12
	mov ip, r0
	lsls r7, r0, #1
	movs r4, #0x81
	lsls r4, r4, #3
	adds r0, r5, r4
	adds r0, r0, r7
	movs r1, #0x80
	lsls r1, r1, #2
	ldrh r0, [r0]
	cmp r0, r1
	beq _08026534
	adds r0, r5, r4
	adds r0, r0, r7
	ldrh r4, [r0]
	b _0802655E
	.align 2, 0
_08026530: .4byte 0xFFFF0000
_08026534:
	ldr r0, _08026590 @ =0x00004808
	adds r3, r5, r0
	ldr r1, [r3]
	lsls r2, r1, #1
	ldr r4, _08026594 @ =0x00004408
	adds r0, r5, r4
	adds r0, r0, r2
	ldrh r0, [r0]
	adds r1, #1
	str r1, [r3]
	adds r4, r0, #0
	movs r1, #0x81
	lsls r1, r1, #3
	adds r0, r5, r1
	adds r0, r0, r7
	strh r4, [r0]
	adds r0, r5, #0
	mov r1, ip
	adds r2, r4, #0
	bl sub_80265FC
_0802655E:
	lsls r1, r4, #1
	adds r0, r5, #0
	adds r0, #8
	adds r0, r0, r1
	ldrh r1, [r0]
	adds r1, #1
	strh r1, [r0]
	ldr r0, _08026598 @ =0xFFFF0000
	ands r6, r0
	orrs r6, r4
	mov r2, r8
	lsls r0, r2, #0x10
	lsrs r0, r0, #0x1e
	lsls r0, r0, #0xa
	ldr r1, _0802659C @ =0xFFFFF3FF
	ands r6, r1
	orrs r6, r0
	lsls r0, r6, #0x10
	lsrs r0, r0, #0x10
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08026590: .4byte 0x00004808
_08026594: .4byte 0x00004408
_08026598: .4byte 0xFFFF0000
_0802659C: .4byte 0xFFFFF3FF

	thumb_func_start sub_80265A0
sub_80265A0: @ 0x080265A0
	push {r4, r5, lr}
	adds r2, r0, #0
	ldr r0, _080265F0 @ =0x00003FFF
	ands r0, r1
	lsls r4, r0, #1
	movs r1, #0x81
	lsls r1, r1, #3
	adds r0, r2, r1
	adds r0, r0, r4
	ldrh r3, [r0]
	lsls r1, r3, #1
	adds r0, r2, #0
	adds r0, #8
	adds r0, r0, r1
	ldrh r1, [r0]
	subs r1, #1
	strh r1, [r0]
	lsls r1, r1, #0x10
	cmp r1, #0
	bne _080265EA
	ldr r5, _080265F4 @ =0x00004808
	adds r0, r2, r5
	ldr r1, [r0]
	subs r1, #1
	str r1, [r0]
	lsls r1, r1, #1
	ldr r5, _080265F8 @ =0x00004408
	adds r0, r2, r5
	adds r0, r0, r1
	strh r3, [r0]
	movs r1, #0x81
	lsls r1, r1, #3
	adds r0, r2, r1
	adds r0, r0, r4
	movs r1, #0x80
	lsls r1, r1, #2
	strh r1, [r0]
_080265EA:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080265F0: .4byte 0x00003FFF
_080265F4: .4byte 0x00004808
_080265F8: .4byte 0x00004408

	thumb_func_start sub_80265FC
sub_80265FC: @ 0x080265FC
	push {lr}
	adds r3, r0, #0
	lsls r1, r1, #6
	ldr r0, [r3, #4]
	adds r0, r0, r1
	lsls r2, r2, #6
	ldr r1, [r3]
	adds r1, r1, r2
	movs r2, #0x40
	movs r3, #0x10
	bl QueueVramDmaTransfer
	pop {r0}
	bx r0

	thumb_func_start sub_8026618
sub_8026618: @ 0x08026618
	lsls r1, r1, #0xe
	movs r3, #0xc0
	lsls r3, r3, #0x13
	adds r1, r1, r3
	str r1, [r0]
	str r2, [r0, #4]
	bx lr
	.align 2, 0

	thumb_func_start sub_8026628
sub_8026628: @ 0x08026628
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r4, r2, #0
	adds r6, r3, #0
	ldr r3, [sp, #0x18]
	movs r7, #0
	cmp r1, #2
	beq _0802664E
	cmp r1, #2
	bgt _08026644
	cmp r1, #1
	beq _08026668
	b _080266B0
_08026644:
	cmp r1, #4
	beq _08026686
	cmp r1, #8
	beq _0802668C
	b _080266B0
_0802664E:
	ldr r0, [r4]
	cmp r0, #0
	bge _08026658
	str r7, [r3]
	b _080266AE
_08026658:
	movs r0, #3
	str r0, [sp]
	adds r0, r5, #0
	adds r1, r4, #0
	adds r2, r6, #0
	bl sub_8026AE8
	b _080266A8
_08026668:
	ldr r0, [r5, #0x10]
	ldr r2, [r0, #0x10]
	ldr r0, [r4]
	cmp r0, r2
	ble _08026678
	lsls r0, r2, #8
	str r0, [r3]
	b _080266AE
_08026678:
	str r1, [sp]
	adds r0, r5, #0
	adds r1, r4, #0
	adds r2, r6, #0
	bl sub_8026AE8
	b _080266A8
_08026686:
	movs r0, #2
	str r0, [sp]
	b _0802669E
_0802668C:
	ldr r0, [r5, #0x10]
	ldr r1, [r0, #0x14]
	ldr r0, [r4, #4]
	cmp r0, r1
	ble _0802669C
	lsls r0, r1, #8
	str r0, [r3]
	b _080266AE
_0802669C:
	str r7, [sp]
_0802669E:
	adds r0, r5, #0
	adds r1, r4, #0
	adds r2, r6, #0
	bl sub_8026A18
_080266A8:
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _080266B0
_080266AE:
	movs r7, #1
_080266B0:
	adds r0, r7, #0
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_80266BC
sub_80266BC: @ 0x080266BC
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, [r5, #4]
	ldrb r1, [r0, #0x18]
	cmp r1, #0
	bne _080266D8
	ldr r0, [r0, #0x14]
	str r0, [r4, #0x24]
	adds r0, r4, #0
	adds r0, #0x28
	strb r1, [r0]
	b _080266F6
_080266D8:
	ldr r0, [r0, #0x14]
	ldr r0, [r0]
	lsrs r0, r0, #8
	bl sub_8026EC0
	adds r1, r0, #0
	str r1, [r4, #0x24]
	ldr r0, [r5, #4]
	ldr r0, [r0, #0x14]
	bl LoadTaggedAsset
	adds r1, r4, #0
	adds r1, #0x28
	movs r0, #1
	strb r0, [r1]
_080266F6:
	ldr r0, [r4, #0x10]
	ldr r1, [r5, #4]
	ldr r1, [r1, #0xc]
	bl sub_80260D4
	ldr r0, [r4, #0x20]
	ldr r1, [r5, #4]
	ldr r1, [r1, #0x10]
	bl sub_80254F8
	ldr r1, [r4, #0x10]
	ldr r0, [r1, #0x10]
	subs r0, #0xf0
	str r0, [r4]
	ldr r0, [r1, #0x14]
	subs r0, #0xa0
	str r0, [r4, #4]
	bl sub_80015D0
	ldr r0, [r4, #0x14]
	ldr r1, [r5, #4]
	ldr r1, [r1]
	bl sub_80260D4
	ldr r0, [r4, #0x14]
	adds r0, #0x28
	ldrb r0, [r0]
	cmp r0, #0
	beq _08026734
	bl sub_80015C0
_08026734:
	ldr r0, [r4, #0x18]
	ldr r1, [r5, #4]
	ldr r1, [r1, #4]
	bl sub_80260D4
	ldr r0, [r4, #0x18]
	adds r0, #0x28
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802674C
	bl sub_80015B0
_0802674C:
	ldr r0, [r4, #0x1c]
	ldr r1, [r5, #4]
	ldr r1, [r1, #8]
	bl sub_80260D4
	ldr r0, [r4, #0x1c]
	adds r0, #0x28
	ldrb r0, [r0]
	cmp r0, #0
	beq _08026764
	bl sub_80015A0
_08026764:
	ldr r0, _08026794 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r2, [r5, #4]
	ldr r1, [r2, #0x1c]
	ldr r2, [r2, #0x20]
	movs r4, #0
	str r4, [sp]
	movs r3, #0
	bl sub_80255D4
	ldr r1, _08026798 @ =0x040000D4
	ldr r0, [r5]
	str r0, [r1]
	movs r2, #0xa0
	lsls r2, r2, #0x13
	str r2, [r1, #4]
	ldr r0, _0802679C @ =0x80000100
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	strh r4, [r2]
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08026794: .4byte gUnknown_030012B4
_08026798: .4byte 0x040000D4
_0802679C: .4byte 0x80000100

	thumb_func_start sub_80267A0
sub_80267A0: @ 0x080267A0
	push {r4, lr}
	adds r4, r0, #0
	movs r0, #0x60
	bl sub_8026EDC
	movs r1, #0
	bl sub_8026448
	str r0, [r4, #0x10]
	ldr r0, _08026808 @ =0x00001064
	bl sub_8026EDC
	bl nullsub_4
	str r0, [r4, #0x20]
	movs r0, #0x5c
	bl sub_8026EDC
	movs r1, #1
	bl sub_8025D74
	str r0, [r4, #0x14]
	movs r0, #0x5c
	bl sub_8026EDC
	movs r1, #2
	bl sub_8025D74
	str r0, [r4, #0x18]
	movs r0, #0x5c
	bl sub_8026EDC
	movs r1, #3
	bl sub_8025D74
	str r0, [r4, #0x1c]
	adds r0, r4, #0
	adds r0, #0x29
	movs r1, #0
	strb r1, [r0]
	str r1, [r4, #0x24]
	subs r0, #1
	strb r1, [r0]
	adds r0, #3
	strb r1, [r0]
	subs r0, #1
	strb r1, [r0]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08026808: .4byte 0x00001064

	thumb_func_start sub_802680C
sub_802680C: @ 0x0802680C
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	cmp r0, #0
	bne _08026820
	ldr r0, [r4, #0x24]
	cmp r0, #0
	beq _0802682A
_08026820:
	ldr r0, [r4, #0x24]
	cmp r0, #0
	beq _0802682A
	bl sub_8026EB4
_0802682A:
	ldr r2, [r4, #0x10]
	cmp r2, #0
	beq _08026840
	ldr r1, [r2, #0x30]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_08026840:
	ldr r0, [r4, #0x20]
	cmp r0, #0
	beq _0802684C
	movs r1, #3
	bl sub_8025444
_0802684C:
	ldr r2, [r4, #0x14]
	cmp r2, #0
	beq _08026862
	ldr r1, [r2, #0x30]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_08026862:
	ldr r2, [r4, #0x18]
	cmp r2, #0
	beq _08026878
	ldr r1, [r2, #0x30]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_08026878:
	ldr r2, [r4, #0x1c]
	cmp r2, #0
	beq _0802688E
	ldr r1, [r2, #0x30]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_0802688E:
	ldr r1, _080268A8 @ =gUnknown_0300084C
	movs r0, #0
	str r0, [r1]
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _080268A2
	adds r0, r4, #0
	bl sub_8026ED0
_080268A2:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080268A8: .4byte gUnknown_0300084C

	thumb_func_start sub_80268AC
sub_80268AC: @ 0x080268AC
	push {r4, lr}
	ldr r4, _080268CC @ =gUnknown_0300084C
	ldr r0, [r4]
	cmp r0, #0
	bne _080268C2
	movs r0, #0x2c
	bl sub_8026EDC
	bl sub_80267A0
	str r0, [r4]
_080268C2:
	ldr r0, [r4]
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_080268CC: .4byte gUnknown_0300084C

	thumb_func_start sub_80268D0
sub_80268D0: @ 0x080268D0
	cmp r1, #0
	bge _080268D6
	movs r1, #0
_080268D6:
	cmp r2, #0
	bge _080268DC
	movs r2, #0
_080268DC:
	asrs r1, r1, #8
	asrs r2, r2, #8
	ldr r3, [r0]
	cmp r3, r1
	ble _080268E8
	adds r3, r1, #0
_080268E8:
	str r3, [r0, #8]
	ldr r1, [r0, #4]
	cmp r1, r2
	ble _080268F2
	adds r1, r2, #0
_080268F2:
	str r1, [r0, #0xc]
	bx lr
	.align 2, 0

	thumb_func_start sub_80268F8
sub_80268F8: @ 0x080268F8
	push {r4, r5, lr}
	adds r5, r0, #0
	ldr r0, [r5, #0x10]
	bl sub_8025F24
	movs r4, #0
_08026904:
	lsls r1, r4, #2
	adds r0, r5, #0
	adds r0, #0x14
	adds r0, r0, r1
	ldr r1, [r0]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802691E
	adds r0, r1, #0
	bl sub_8025F24
_0802691E:
	adds r4, #1
	cmp r4, #2
	ble _08026904
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_802692C
sub_802692C: @ 0x0802692C
	push {r4, r5, lr}
	sub sp, #8
	adds r5, r0, #0
	ldr r0, [r5, #0x10]
	ldr r2, [r0, #0x30]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	adds r1, r5, #0
	adds r1, #8
	ldr r2, [r2, #0x1c]
	bl sub_803AD80
	ldr r1, [r5, #0x10]
	ldr r0, [r1]
	str r0, [sp]
	ldr r0, [r1, #4]
	str r0, [sp, #4]
	movs r4, #0
_08026952:
	lsls r1, r4, #2
	adds r0, r5, #0
	adds r0, #0x14
	adds r0, r0, r1
	ldr r2, [r0]
	adds r0, r2, #0
	adds r0, #0x28
	ldrb r0, [r0]
	cmp r0, #0
	beq _08026976
	ldr r1, [r2, #0x30]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0x1c]
	mov r1, sp
	bl sub_803AD80
_08026976:
	adds r4, #1
	cmp r4, #2
	ble _08026952
	add sp, #8
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_8026984
sub_8026984: @ 0x08026984
	push {r4, r5, lr}
	sub sp, #8
	adds r5, r0, #0
	ldr r0, [r5, #0x10]
	ldr r2, [r0, #0x30]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	adds r1, r5, #0
	adds r1, #8
	ldr r2, [r2, #0x14]
	bl sub_803AD80
	ldr r1, [r5, #0x10]
	ldr r0, [r1]
	str r0, [sp]
	ldr r0, [r1, #4]
	str r0, [sp, #4]
	movs r4, #0
_080269AA:
	lsls r1, r4, #2
	adds r0, r5, #0
	adds r0, #0x14
	adds r0, r0, r1
	ldr r2, [r0]
	adds r0, r2, #0
	adds r0, #0x28
	ldrb r0, [r0]
	cmp r0, #0
	beq _080269CE
	ldr r1, [r2, #0x30]
	movs r3, #0x10
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0x14]
	mov r1, sp
	bl sub_803AD80
_080269CE:
	adds r4, #1
	cmp r4, #2
	ble _080269AA
	add sp, #8
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_80269DC
sub_80269DC: @ 0x080269DC
	push {r4, lr}
	movs r4, #1
	cmp r1, #0
	beq _080269F0
	ldr r0, [r2]
	cmp r0, #0
	beq _080269F0
	cmp r3, #0
	beq _080269F0
	movs r4, #0
_080269F0:
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1

	thumb_func_start sub_80269F8
sub_80269F8: @ 0x080269F8
	push {r4, lr}
	movs r4, #1
	cmp r1, #0
	beq _08026A0C
	ldr r0, [r2]
	cmp r0, #0
	beq _08026A0C
	cmp r3, #0
	beq _08026A0C
	movs r4, #0
_08026A0C:
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1

	thumb_func_start sub_8026A14
sub_8026A14: @ 0x08026A14
	movs r0, #0
	bx lr

	thumb_func_start sub_8026A18
sub_8026A18: @ 0x08026A18
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xc
	mov sl, r0
	adds r7, r1, #0
	mov r8, r3
	movs r0, #0
	mov sb, r0
	add r0, sp, #4
	mov r1, sb
	strb r1, [r0]
	ldr r6, [r7, #4]
	ldr r4, [r7]
	adds r2, r4, r2
	subs r5, r2, #1
	asrs r4, r4, #3
	asrs r6, r6, #3
	asrs r5, r5, #3
	movs r0, #1
	rsbs r0, r0, #0
	cmp r4, r0
	bne _08026A4C
	movs r4, #0
_08026A4C:
	mov r2, sl
	ldr r0, [r2, #0x20]
	ldr r0, [r0, #0x10]
	cmp r5, r0
	bne _08026A58
	subs r5, #1
_08026A58:
	mov r0, sl
	adds r0, #0x2a
	str r0, [sp, #8]
	cmp r4, r5
	bgt _08026A88
_08026A62:
	mov r1, sl
	ldr r0, [r1, #0x20]
	add r2, sp, #4
	str r2, [sp]
	adds r1, r4, #0
	adds r2, r6, #0
	ldr r3, [sp, #0x2c]
	bl sub_8025130
	cmp r0, #0
	beq _08026A7C
	movs r0, #1
	mov sb, r0
_08026A7C:
	adds r4, #1
	cmp r4, r5
	bgt _08026A88
	mov r1, sb
	cmp r1, #0
	beq _08026A62
_08026A88:
	mov r2, sb
	cmp r2, #0
	beq _08026ABE
	ldr r0, [sp, #0x2c]
	cmp r0, #0
	beq _08026AAE
	cmp r0, #2
	bne _08026ABE
	ldr r0, [r7, #4]
	movs r1, #7
	ands r0, r1
	movs r1, #8
	subs r1, r1, r0
	lsls r1, r1, #8
	mov r2, r8
	ldr r0, [r2]
	adds r0, r0, r1
	str r0, [r2]
	b _08026ABE
_08026AAE:
	ldr r0, [r7, #4]
	movs r1, #7
	ands r0, r1
	lsls r0, r0, #8
	mov r2, r8
	ldr r1, [r2]
	subs r1, r1, r0
	str r1, [r2]
_08026ABE:
	ldr r1, [sp, #8]
	ldrb r0, [r1]
	cmp r0, #0
	beq _08026AD4
	add r0, sp, #4
	ldrb r1, [r0]
	cmp r1, #0
	beq _08026AD4
	mov r0, sl
	adds r0, #0x29
	strb r1, [r0]
_08026AD4:
	mov r0, sb
	add sp, #0xc
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8026AE8
sub_8026AE8: @ 0x08026AE8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xc
	mov sl, r0
	adds r7, r1, #0
	mov r8, r3
	movs r0, #0
	mov sb, r0
	add r0, sp, #4
	mov r1, sb
	strb r1, [r0]
	ldr r4, [r7, #4]
	ldr r6, [r7]
	adds r2, r4, r2
	subs r5, r2, #1
	asrs r6, r6, #3
	asrs r4, r4, #3
	asrs r5, r5, #3
	movs r0, #1
	rsbs r0, r0, #0
	cmp r4, r0
	bne _08026B1C
	movs r4, #0
_08026B1C:
	mov r1, sl
	ldr r0, [r1, #0x20]
	ldr r0, [r0, #0x14]
	cmp r5, r0
	bne _08026B28
	subs r5, #1
_08026B28:
	mov r0, sl
	adds r0, #0x2a
	str r0, [sp, #8]
	cmp r4, r5
	bgt _08026B58
_08026B32:
	mov r1, sl
	ldr r0, [r1, #0x20]
	add r1, sp, #4
	str r1, [sp]
	adds r1, r6, #0
	adds r2, r4, #0
	ldr r3, [sp, #0x2c]
	bl sub_8025130
	cmp r0, #0
	beq _08026B4C
	movs r0, #1
	mov sb, r0
_08026B4C:
	adds r4, #1
	cmp r4, r5
	bgt _08026B58
	mov r1, sb
	cmp r1, #0
	beq _08026B32
_08026B58:
	mov r0, sb
	cmp r0, #0
	beq _08026B96
	ldr r1, [sp, #0x2c]
	cmp r1, #1
	beq _08026B82
	cmp r1, #3
	bne _08026B96
	mov r0, r8
	ldr r2, [r0]
	adds r2, #1
	ldr r1, [r7]
	movs r0, #7
	ands r1, r0
	movs r0, #8
	subs r0, r0, r1
	lsls r0, r0, #8
	adds r2, r2, r0
	mov r1, r8
	str r2, [r1]
	b _08026B96
_08026B82:
	mov r1, r8
	ldr r0, [r1]
	subs r0, #1
	ldr r1, [r7]
	movs r2, #7
	ands r1, r2
	lsls r1, r1, #8
	subs r0, r0, r1
	mov r1, r8
	str r0, [r1]
_08026B96:
	ldr r1, [sp, #8]
	ldrb r0, [r1]
	cmp r0, #0
	beq _08026BAC
	add r0, sp, #4
	ldrb r1, [r0]
	cmp r1, #0
	beq _08026BAC
	mov r0, sl
	adds r0, #0x29
	strb r1, [r0]
_08026BAC:
	mov r0, sb
	add sp, #0xc
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8026BC0
sub_8026BC0: @ 0x08026BC0
	push {r4, lr}
	sub sp, #0xc
	adds r4, r0, #0
	movs r3, #0
	add r0, sp, #4
	strb r3, [r0]
	str r3, [sp, #8]
	asrs r3, r1, #3
	asrs r2, r2, #3
	cmp r3, #0
	bge _08026BD8
	movs r3, #0
_08026BD8:
	cmp r2, #0
	bge _08026BDE
	movs r2, #0
_08026BDE:
	ldr r0, [r4, #0x20]
	add r1, sp, #8
	str r1, [sp]
	adds r1, r3, #0
	add r3, sp, #4
	bl sub_8025460
	add r0, sp, #4
	ldrb r0, [r0]
	add sp, #0xc
	pop {r4}
	pop {r1}
	bx r1

	thumb_func_start sub_8026BF8
sub_8026BF8: @ 0x08026BF8
	push {r4, r5, r6, lr}
	adds r4, r1, #0
	adds r6, r2, #0
	ldr r1, [r4]
	asrs r1, r1, #3
	ldr r2, [r4, #4]
	asrs r5, r2, #3
	ldr r0, [r0, #0x20]
	adds r2, r5, #0
	bl sub_80250BC
	adds r3, r0, #0
	cmp r3, #0
	bne _08026C18
	movs r0, #0
	b _08026C36
_08026C18:
	ldr r2, [r4, #4]
	ldr r0, [r4]
	movs r1, #7
	ands r0, r1
	adds r0, r3, r0
	movs r1, #0
	ldrsb r1, [r0, r1]
	lsls r0, r5, #3
	adds r0, r0, r1
	subs r0, r0, r2
	lsls r0, r0, #8
	ldr r1, [r6]
	adds r1, r1, r0
	str r1, [r6]
	movs r0, #1
_08026C36:
	pop {r4, r5, r6}
	pop {r1}
	bx r1

	thumb_func_start sub_8026C3C
sub_8026C3C: @ 0x08026C3C
	push {r4, r5, r6, lr}
	sub sp, #8
	adds r4, r1, #0
	adds r6, r2, #0
	ldr r1, [r4]
	asrs r1, r1, #3
	ldr r2, [r4, #4]
	asrs r5, r2, #3
	ldr r0, [r0, #0x20]
	add r2, sp, #4
	str r2, [sp]
	adds r2, r5, #0
	movs r3, #0
	bl sub_8025228
	lsls r0, r0, #0x18
	asrs r2, r0, #0x18
	cmp r2, #0
	bge _08026C66
	movs r0, #0
	b _08026C78
_08026C66:
	ldr r0, [r4, #4]
	lsls r1, r5, #3
	adds r1, r1, r2
	subs r1, r1, r0
	lsls r1, r1, #8
	ldr r0, [r6]
	adds r0, r0, r1
	str r0, [r6]
	movs r0, #1
_08026C78:
	add sp, #8
	pop {r4, r5, r6}
	pop {r1}
	bx r1

	thumb_func_start sub_8026C80
sub_8026C80: @ 0x08026C80
	cmp r1, #0
	beq _08026C86
	ldr r0, [r2]
_08026C86:
	movs r0, #0
	bx lr
	.align 2, 0

	thumb_func_start sub_8026C8C
sub_8026C8C: @ 0x08026C8C
	movs r0, #0
	bx lr

	thumb_func_start sub_8026C90
sub_8026C90: @ 0x08026C90
	push {r4, r5, r6, lr}
	adds r1, r0, #0
	ldr r0, [r1, #0x10]
	ldr r2, [r0]
	ldr r3, [r0, #4]
	adds r0, #0x24
	ldrb r5, [r0]
	cmp r5, #0
	beq _08026D5C
	movs r0, #4
	ands r0, r5
	cmp r0, #0
	beq _08026CC0
	ldr r4, [r1, #0xc]
	ldr r0, _08026CB8 @ =0xFFFFE556
	cmp r4, r0
	ble _08026CC0
	ldr r6, _08026CBC @ =0xFFFFFF00
	b _08026CD4
	.align 2, 0
_08026CB8: .4byte 0xFFFFE556
_08026CBC: .4byte 0xFFFFFF00
_08026CC0:
	movs r0, #8
	ands r0, r5
	cmp r0, #0
	beq _08026CD8
	ldr r4, [r1, #0xc]
	ldr r0, _08026CEC @ =0x00001AA9
	cmp r4, r0
	bgt _08026CD8
	movs r6, #0x80
	lsls r6, r6, #1
_08026CD4:
	adds r0, r4, r6
	str r0, [r1, #0xc]
_08026CD8:
	movs r0, #2
	ands r0, r5
	cmp r0, #0
	beq _08026CF8
	ldr r4, [r1, #8]
	ldr r0, _08026CF0 @ =0xFFFFD800
	cmp r4, r0
	ble _08026CF8
	ldr r6, _08026CF4 @ =0xFFFFFF00
	b _08026D0C
	.align 2, 0
_08026CEC: .4byte 0x00001AA9
_08026CF0: .4byte 0xFFFFD800
_08026CF4: .4byte 0xFFFFFF00
_08026CF8:
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _08026D10
	ldr r4, [r1, #8]
	ldr r0, _08026D24 @ =0x000027FF
	cmp r4, r0
	bgt _08026D10
	movs r6, #0x80
	lsls r6, r6, #1
_08026D0C:
	adds r0, r4, r6
	str r0, [r1, #8]
_08026D10:
	movs r0, #3
	ands r0, r5
	cmp r0, #0
	bne _08026D38
	ldr r0, [r1, #8]
	cmp r0, #0
	ble _08026D2C
	ldr r4, _08026D28 @ =0xFFFFFF00
	adds r0, r0, r4
	b _08026D36
	.align 2, 0
_08026D24: .4byte 0x000027FF
_08026D28: .4byte 0xFFFFFF00
_08026D2C:
	cmp r0, #0
	bge _08026D38
	movs r6, #0x80
	lsls r6, r6, #1
	adds r0, r0, r6
_08026D36:
	str r0, [r1, #8]
_08026D38:
	movs r0, #0xc
	ands r5, r0
	cmp r5, #0
	bne _08026D5C
	ldr r0, [r1, #0xc]
	cmp r0, #0
	ble _08026D50
	ldr r4, _08026D4C @ =0xFFFFFF00
	adds r0, r0, r4
	b _08026D5A
	.align 2, 0
_08026D4C: .4byte 0xFFFFFF00
_08026D50:
	cmp r0, #0
	bge _08026D5C
	movs r6, #0x80
	lsls r6, r6, #1
	adds r0, r0, r6
_08026D5A:
	str r0, [r1, #0xc]
_08026D5C:
	ldr r0, [r1, #8]
	adds r2, r2, r0
	ldr r0, [r1, #0xc]
	adds r3, r3, r0
	ldr r4, [r1]
	subs r0, r2, r4
	cmp r0, #0
	bge _08026D6E
	adds r0, #3
_08026D6E:
	asrs r0, r0, #2
	adds r0, r4, r0
	str r0, [r1]
	ldr r4, [r1, #4]
	subs r0, r3, r4
	cmp r0, #0
	bge _08026D7E
	adds r0, #3
_08026D7E:
	asrs r0, r0, #2
	adds r0, r4, r0
	str r0, [r1, #4]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8026D8C
sub_8026D8C: @ 0x08026D8C
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x10]
	ldr r2, [r0]
	ldr r3, [r0, #4]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _08026DB4
	ldr r1, [r4, #8]
	ldr r0, _08026DAC @ =0xFFFFED8A
	cmp r1, r0
	ble _08026DC4
	ldr r5, _08026DB0 @ =0xFFFFFF00
	b _08026DC0
	.align 2, 0
_08026DAC: .4byte 0xFFFFED8A
_08026DB0: .4byte 0xFFFFFF00
_08026DB4:
	ldr r1, [r4, #8]
	ldr r0, _08026DF4 @ =0x00001275
	cmp r1, r0
	bgt _08026DC4
	movs r5, #0x80
	lsls r5, r5, #1
_08026DC0:
	adds r0, r1, r5
	str r0, [r4, #8]
_08026DC4:
	ldr r1, _08026DF8 @ =0xFFFFF000
	str r1, [r4, #0xc]
	ldr r0, [r4, #8]
	adds r2, r2, r0
	adds r3, r3, r1
	ldr r1, [r4]
	subs r0, r2, r1
	cmp r0, #0
	bge _08026DD8
	adds r0, #3
_08026DD8:
	asrs r0, r0, #2
	adds r0, r1, r0
	str r0, [r4]
	ldr r1, [r4, #4]
	subs r0, r3, r1
	cmp r0, #0
	bge _08026DE8
	adds r0, #3
_08026DE8:
	asrs r0, r0, #2
	adds r0, r1, r0
	str r0, [r4, #4]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08026DF4: .4byte 0x00001275
_08026DF8: .4byte 0xFFFFF000

	thumb_func_start sub_8026DFC
sub_8026DFC: @ 0x08026DFC
	push {lr}
	adds r3, r0, #0
	ldr r1, [r3, #0x10]
	ldr r0, [r1]
	str r0, [r3]
	ldr r0, [r1, #4]
	str r0, [r3, #4]
	ldr r0, [r3, #0x14]
	cmp r0, #1
	bne _08026E34
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _08026E24
	ldr r0, _08026E20 @ =0xFFFFED8A
	b _08026E26
	.align 2, 0
_08026E20: .4byte 0xFFFFED8A
_08026E24:
	ldr r0, _08026E2C @ =0x00001276
_08026E26:
	str r0, [r3, #8]
	ldr r0, _08026E30 @ =0xFFFFF000
	b _08026E38
	.align 2, 0
_08026E2C: .4byte 0x00001276
_08026E30: .4byte 0xFFFFF000
_08026E34:
	movs r0, #0
	str r0, [r3, #8]
_08026E38:
	str r0, [r3, #0xc]
	ldr r1, [r3]
	ldr r0, [r3, #8]
	adds r1, r1, r0
	str r1, [r3]
	ldr r2, [r3, #4]
	ldr r0, [r3, #0xc]
	adds r2, r2, r0
	str r2, [r3, #4]
	ldr r0, _08026E60 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r3, _08026E64 @ =0xFFFF8800
	adds r1, r1, r3
	ldr r3, _08026E68 @ =0xFFFFB000
	adds r2, r2, r3
	bl sub_80268D0
	pop {r0}
	bx r0
	.align 2, 0
_08026E60: .4byte gUnknown_03001308
_08026E64: .4byte 0xFFFF8800
_08026E68: .4byte 0xFFFFB000

	thumb_func_start sub_8026E6C
sub_8026E6C: @ 0x08026E6C
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x14]
	cmp r0, #2
	beq _08026E86
	cmp r0, #2
	bgt _08026E8C
	cmp r0, #1
	bne _08026E8C
	adds r0, r4, #0
	bl sub_8026D8C
	b _08026E8C
_08026E86:
	adds r0, r4, #0
	bl sub_8026C90
_08026E8C:
	ldr r0, _08026EA8 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r1, [r4]
	ldr r2, _08026EAC @ =0xFFFF8800
	adds r1, r1, r2
	ldr r2, [r4, #4]
	ldr r3, _08026EB0 @ =0xFFFFB000
	adds r2, r2, r3
	bl sub_80268D0
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08026EA8: .4byte gUnknown_03001308
_08026EAC: .4byte 0xFFFF8800
_08026EB0: .4byte 0xFFFFB000

	thumb_func_start sub_8026EB4
sub_8026EB4: @ 0x08026EB4
	push {lr}
	bl mem_free
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8026EC0
sub_8026EC0: @ 0x08026EC0
	push {lr}
	movs r1, #0x80
	lsls r1, r1, #0x17
	bl mem_alloc
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8026ED0
sub_8026ED0: @ 0x08026ED0
	push {lr}
	bl mem_free
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8026EDC
sub_8026EDC: @ 0x08026EDC
	push {lr}
	movs r1, #0x80
	lsls r1, r1, #0x17
	bl mem_alloc
	pop {r1}
	bx r1
	.align 2, 0

