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

