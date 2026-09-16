.include "asm/macros.inc"

.syntax unified
.arm
	thumb_func_start sub_8007FD8
sub_8007FD8: @ 0x08007FD8
	push {r4, r5, r6, r7, lr}
	sub sp, #0x10
	adds r2, r0, #0
	adds r4, r1, #0
	movs r3, #0
	adds r0, #0x25
	ldrb r0, [r0]
	cmp r0, #1
	bne _08007FEE
	movs r0, #1
	b _0800803A
_08007FEE:
	ldrb r1, [r2, #0xd]
	lsrs r0, r1, #2
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	bne _08008038
	mov r0, sp
	adds r1, r2, #0
	bl sub_8007B00
	ldr r0, [sp]
	lsls r3, r0, #8
	ldr r0, [sp, #4]
	lsls r6, r0, #8
	ldr r1, [sp, #8]
	lsls r1, r1, #8
	adds r1, r3, r1
	ldr r0, [sp, #0xc]
	lsls r0, r0, #8
	adds r5, r6, r0
	movs r7, #0
	ldr r2, [r4]
	cmp r1, r2
	ble _08008036
	ldr r0, [r4, #8]
	adds r0, r2, r0
	cmp r3, r0
	bge _08008036
	ldr r1, [r4, #4]
	cmp r5, r1
	ble _08008036
	ldr r0, [r4, #0xc]
	adds r0, r1, r0
	cmp r6, r0
	bge _08008036
	movs r7, #1
_08008036:
	adds r3, r7, #0
_08008038:
	adds r0, r3, #0
_0800803A:
	add sp, #0x10
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8008044
sub_8008044: @ 0x08008044
	push {r4, r5, lr}
	mov ip, r0
	adds r0, #0x2c
	ldrb r0, [r0]
	cmp r0, #0
	beq _080080BA
	mov r0, ip
	ldr r4, [r0, #0x34]
	ldr r3, [r0, #0x20]
	mov r1, ip
	adds r1, #0x2d
	ldr r2, [r3]
	ldrb r5, [r1]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r2
	adds r2, r1, #0
	ldrb r0, [r0, #0x15]
	cmp r4, r0
	bge _08008076
	adds r0, r4, #1
	mov r1, ip
	str r0, [r1, #0x34]
	b _08008082
_08008076:
	movs r0, #0
	mov r4, ip
	str r0, [r4, #0x34]
	ldr r0, [r4, #0x30]
	adds r0, #1
	str r0, [r4, #0x30]
_08008082:
	mov r5, ip
	ldr r1, [r5, #0x30]
	ldr r3, [r3]
	ldrb r4, [r2]
	lsls r0, r4, #3
	subs r0, r0, r4
	lsls r0, r0, #2
	adds r0, r0, r3
	ldrb r0, [r0, #0x16]
	cmp r1, r0
	blt _080080BA
	movs r0, #0
	str r0, [r5, #0x30]
	str r0, [r5, #0x34]
	ldrb r5, [r2]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r3
	movs r1, #2
	ldrb r0, [r0, #0x17]
	ands r1, r0
	cmp r1, #0
	bne _080080BA
	movs r1, #1
	mov r0, ip
	adds r0, #0x38
	strb r1, [r0]
_080080BA:
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_80080C0
sub_80080C0: @ 0x080080C0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	mov sl, r1
	adds r1, r0, #0
	adds r1, #0x28
	ldrb r1, [r1]
	lsls r4, r1, #0x1b
	lsrs r4, r4, #0x1f
	lsls r1, r1, #0x1a
	lsrs r1, r1, #0x1f
	mov sb, r1
	ldr r1, [r0]
	asrs r7, r1, #8
	ldr r1, [r0, #4]
	asrs r1, r1, #8
	mov r8, r1
	ldr r2, [r0, #0x20]
	adds r0, #0x2d
	ldrb r3, [r0]
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	ldr r0, [r2]
	adds r0, r0, r1
	adds r3, r0, #4
	movs r2, #4
	ldrsh r1, [r0, r2]
	movs r0, #2
	ldrsh r2, [r3, r0]
	ldrb r5, [r3, #4]
	ldrb r6, [r3, #5]
	adds r1, r1, r7
	add r2, r8
	mov r0, sp
	bl sub_803AFE4
	mov r0, sp
	adds r1, r5, #0
	adds r2, r6, #0
	bl sub_803AFDC
	cmp r4, #0
	beq _0800812A
	lsls r0, r7, #1
	ldr r1, [sp]
	ldr r2, [sp, #8]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp]
_0800812A:
	mov r1, sb
	cmp r1, #0
	beq _0800813E
	mov r2, r8
	lsls r0, r2, #1
	ldr r1, [sp, #4]
	ldr r2, [sp, #0xc]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #4]
_0800813E:
	mov r0, sp
	mov r1, sl
	bl sub_8001688
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_800815C
sub_800815C: @ 0x0800815C
	push {r4, lr}
	ldr r1, _08008184 @ =gUnknown_030012B8
	ldr r3, [r1]
	ldr r1, [r0, #0x20]
	adds r0, #0x2d
	ldr r2, [r1]
	ldrb r4, [r0]
	lsls r1, r4, #3
	subs r1, r1, r4
	lsls r1, r1, #2
	adds r1, r1, r2
	ldrb r1, [r1, #0x14]
	adds r0, r3, #0
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08008184: .4byte gUnknown_030012B8
	thumb_func_start sub_8008188
sub_8008188: @ 0x08008188
	adds r3, r0, #0
	subs r0, r1, #1
	cmp r0, #0xb
	bhi _080081FE
	lsls r0, r0, #2
	ldr r1, _0800819C @ =_080081A0
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800819C: .4byte _080081A0
_080081A0: @ jump table
	.4byte _080081DC @ case 0
	.4byte _080081D0 @ case 1
	.4byte _080081FE @ case 2
	.4byte _080081E8 @ case 3
	.4byte _080081FE @ case 4
	.4byte _080081FE @ case 5
	.4byte _080081FE @ case 6
	.4byte _080081EE @ case 7
	.4byte _080081FE @ case 8
	.4byte _080081FE @ case 9
	.4byte _080081FE @ case 10
	.4byte _080081EE @ case 11
_080081D0:
	ldrb r2, [r2, #4]
	lsls r1, r2, #7
	ldr r0, [r3]
	adds r0, r0, r1
	str r0, [r3]
	b _080081FE
_080081DC:
	ldrb r2, [r2, #4]
	lsls r1, r2, #7
	ldr r0, [r3]
	subs r0, r0, r1
	str r0, [r3]
	b _080081FE
_080081E8:
	movs r0, #2
	ldrsh r1, [r2, r0]
	b _080081F6
_080081EE:
	movs r0, #2
	ldrsh r1, [r2, r0]
	ldrb r2, [r2, #5]
	adds r1, r2, r1
_080081F6:
	lsls r1, r1, #8
	ldr r0, [r3, #4]
	subs r0, r0, r1
	str r0, [r3, #4]
_080081FE:
	bx lr

	thumb_func_start sub_8008200
sub_8008200: @ 0x08008200
	adds r3, r0, #0
	subs r0, r1, #1
	cmp r0, #0xb
	bhi _08008276
	lsls r0, r0, #2
	ldr r1, _08008214 @ =_08008218
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08008214: .4byte _08008218
_08008218: @ jump table
	.4byte _08008254 @ case 0
	.4byte _08008248 @ case 1
	.4byte _08008276 @ case 2
	.4byte _08008260 @ case 3
	.4byte _08008276 @ case 4
	.4byte _08008276 @ case 5
	.4byte _08008276 @ case 6
	.4byte _08008266 @ case 7
	.4byte _08008276 @ case 8
	.4byte _08008276 @ case 9
	.4byte _08008276 @ case 10
	.4byte _08008266 @ case 11
_08008248:
	ldrb r2, [r2, #4]
	lsls r1, r2, #7
	ldr r0, [r3]
	subs r0, r0, r1
	str r0, [r3]
	b _08008276
_08008254:
	ldrb r2, [r2, #4]
	lsls r1, r2, #7
	ldr r0, [r3]
	adds r0, r0, r1
	str r0, [r3]
	b _08008276
_08008260:
	movs r0, #2
	ldrsh r1, [r2, r0]
	b _0800826E
_08008266:
	movs r0, #2
	ldrsh r1, [r2, r0]
	ldrb r2, [r2, #5]
	adds r1, r2, r1
_0800826E:
	lsls r1, r1, #8
	ldr r0, [r3, #4]
	adds r0, r0, r1
	str r0, [r3, #4]
_08008276:
	bx lr

	thumb_func_start sub_8008278
sub_8008278: @ 0x08008278
	adds r3, r0, #0
	subs r0, r1, #1
	cmp r0, #0xb
	bhi _08008302
	lsls r0, r0, #2
	ldr r1, _0800828C @ =_08008290
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800828C: .4byte _08008290
_08008290: @ jump table
	.4byte _080082CA @ case 0
	.4byte _080082C0 @ case 1
	.4byte _08008302 @ case 2
	.4byte _080082E2 @ case 3
	.4byte _08008302 @ case 4
	.4byte _08008302 @ case 5
	.4byte _08008302 @ case 6
	.4byte _080082E8 @ case 7
	.4byte _08008302 @ case 8
	.4byte _08008302 @ case 9
	.4byte _08008302 @ case 10
	.4byte _080082E8 @ case 11
_080082C0:
	ldrb r0, [r2, #4]
	lsls r1, r0, #7
	ldr r0, [r3]
	subs r0, r0, r1
	b _080082D2
_080082CA:
	ldrb r0, [r2, #4]
	lsls r1, r0, #7
	ldr r0, [r3]
	adds r0, r0, r1
_080082D2:
	str r0, [r3]
	movs r0, #2
	ldrsh r1, [r2, r0]
	lsls r1, r1, #8
	ldr r0, [r3, #4]
	adds r0, r0, r1
	str r0, [r3, #4]
	b _08008302
_080082E2:
	movs r0, #2
	ldrsh r1, [r2, r0]
	b _080082F0
_080082E8:
	movs r0, #2
	ldrsh r1, [r2, r0]
	ldrb r0, [r2, #5]
	adds r1, r0, r1
_080082F0:
	lsls r1, r1, #8
	ldr r0, [r3, #4]
	adds r0, r0, r1
	str r0, [r3, #4]
	ldrb r2, [r2, #4]
	lsls r1, r2, #7
	ldr r0, [r3]
	subs r0, r0, r1
	str r0, [r3]
_08008302:
	bx lr

	thumb_func_start sub_8008304
sub_8008304: @ 0x08008304
	push {r4, lr}
	movs r4, #0
	adds r2, r0, #0
	adds r2, #0x25
	ldrb r2, [r2]
	cmp r2, #1
	beq _0800831C
	bl sub_8007114
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0800831E
_0800831C:
	movs r4, #1
_0800831E:
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8008328
sub_8008328: @ 0x08008328
	push {r4, lr}
	movs r4, #0
	adds r1, r0, #0
	adds r1, #0x25
	ldrb r1, [r1]
	cmp r1, #1
	beq _08008340
	bl sub_8006FE4
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08008342
_08008340:
	movs r4, #1
_08008342:
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_800834C
sub_800834C: @ 0x0800834C
	movs r0, #1
	bx lr

	thumb_func_start sub_8008350
sub_8008350: @ 0x08008350
	push {lr}
	adds r1, r0, #0
	ldr r0, _08008360 @ =gUnknown_030012CC
	ldr r0, [r0]
	bl sub_8007A84
	pop {r0}
	bx r0
	.align 2, 0
_08008360: .4byte gUnknown_030012CC

	thumb_func_start sub_8008364
sub_8008364: @ 0x08008364
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8008044
	ldr r1, [r4, #0x18]
	adds r1, #0x60
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	ldr r1, [r4, #0x18]
	movs r2, #8
	ldrsh r0, [r1, r2]
	adds r4, r4, r0
	ldr r1, [r1, #0xc]
	adds r0, r4, #0
	bl sub_803AD7C
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8008394
sub_8008394: @ 0x08008394
	ldr r2, [r0, #0x20]
	adds r0, #0x2d
	ldrb r3, [r0]
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	ldr r0, [r2]
	adds r0, r0, r1
	adds r0, #4
	bx lr

	thumb_func_start sub_80083A8
sub_80083A8: @ 0x080083A8
	ldr r0, _080083B4 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0, #4]
	bx lr
	.align 2, 0
_080083B4: .4byte gUnknown_030012D0

	thumb_func_start sub_80083B8
sub_80083B8: @ 0x080083B8
	push {r4, lr}
	adds r3, r0, #0
	ldr r1, [r3, #0x20]
	adds r2, r3, #0
	adds r2, #0x2d
	ldrb r4, [r2]
	lsls r0, r4, #3
	subs r0, r0, r4
	lsls r0, r0, #2
	ldr r1, [r1]
	adds r1, r1, r0
	adds r0, r3, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _080083EC
	movs r0, #2
	ldrb r2, [r1, #0x17]
	ands r0, r2
	cmp r0, #0
	bne _080083EC
	ldrb r0, [r1, #0x16]
	subs r0, #1
	str r0, [r3, #0x30]
	ldrb r0, [r1, #0x15]
	str r0, [r3, #0x34]
_080083EC:
	ldr r2, [r3, #0x20]
	ldr r0, [r3, #0x30]
	ldr r1, [r1]
	lsls r0, r0, #1
	adds r0, r0, r1
	ldr r1, [r2, #4]
	ldrh r0, [r0]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8008408
sub_8008408: @ 0x08008408
	ldr r0, _08008424 @ =gUnknown_03001308
	ldr r1, [r0]
	adds r0, r1, #0
	adds r0, #0x2b
	ldrb r0, [r0]
	cmp r0, #0
	beq _08008428
	ldr r0, [r1, #0x10]
	adds r0, #0x34
	ldrb r0, [r0]
	lsls r0, r0, #0x1e
	lsrs r0, r0, #0x1e
	subs r0, #1
	b _08008432
	.align 2, 0
_08008424: .4byte gUnknown_03001308
_08008428:
	ldr r0, [r1, #0x10]
	adds r0, #0x34
	ldrb r0, [r0]
	lsls r0, r0, #0x1e
	lsrs r0, r0, #0x1e
_08008432:
	bx lr

	thumb_func_start sub_8008434
sub_8008434: @ 0x08008434
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	mov r8, r0
	adds r5, r1, #0
	adds r6, r2, #0
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	mov r8, r0
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	lsls r6, r6, #0x10
	lsrs r6, r6, #0x10
	movs r0, #0x40
	bl sub_8026EDC
	adds r4, r0, #0
	bl sub_800725C
	ldr r0, _0800847C @ =gStaticData_087E3C44
	str r0, [r4, #0x18]
	adds r0, r4, #0
	bl sub_8007AB4
	mov r0, r8
	strh r0, [r4, #8]
	lsls r5, r5, #8
	str r5, [r4]
	lsls r6, r6, #8
	str r6, [r4, #4]
	adds r0, r4, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0800847C: .4byte gStaticData_087E3C44

	thumb_func_start sub_8008480
sub_8008480: @ 0x08008480
	movs r0, #1
	bx lr

	thumb_func_start sub_8008484
sub_8008484: @ 0x08008484
	push {lr}
	adds r2, r0, #0
	ldr r0, _080084A0 @ =gStaticData_087E3BEC
	str r0, [r2, #0x18]
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _0800849A
	adds r0, r2, #0
	bl sub_8026ED0
_0800849A:
	pop {r0}
	bx r0
	.align 2, 0
_080084A0: .4byte gStaticData_087E3BEC

	thumb_func_start sub_80084A4
sub_80084A4: @ 0x080084A4
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800725C
	ldr r0, _080084C0 @ =gStaticData_087E3C44
	str r0, [r4, #0x18]
	adds r0, r4, #0
	bl sub_8007AB4
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_080084C0: .4byte gStaticData_087E3C44

	thumb_func_start sub_80084C4
sub_80084C4: @ 0x080084C4
	push {lr}
	bl sub_80083B8
	adds r2, r0, #0
	ldr r0, [r2, #4]
	ldrb r0, [r0]
	lsrs r0, r0, #4
	cmp r0, #6
	bhi _0800850C
	lsls r0, r0, #2
	ldr r1, _080084E0 @ =_080084E4
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080084E0: .4byte _080084E4
_080084E4: @ jump table
	.4byte _08008500 @ case 0
	.4byte _0800850C @ case 1
	.4byte _0800850C @ case 2
	.4byte _0800850C @ case 3
	.4byte _0800850C @ case 4
	.4byte _0800850C @ case 5
	.4byte _08008506 @ case 6
_08008500:
	adds r0, r2, #0
	adds r0, #0x24
	b _0800850E
_08008506:
	adds r0, r2, #0
	adds r0, #0x14
	b _0800850E
_0800850C:
	ldr r0, _08008514 @ =gStaticData_0816B300
_0800850E:
	pop {r1}
	bx r1
	.align 2, 0
_08008514: .4byte gStaticData_0816B300

	thumb_func_start sub_8008518
sub_8008518: @ 0x08008518
	push {lr}
	bl sub_80083B8
	adds r2, r0, #0
	ldr r0, [r2, #4]
	ldrb r0, [r0]
	lsrs r0, r0, #4
	cmp r0, #6
	bhi _0800855A
	lsls r0, r0, #2
	ldr r1, _08008534 @ =_08008538
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08008534: .4byte _08008538
_08008538: @ jump table
	.4byte _08008554 @ case 0
	.4byte _0800855A @ case 1
	.4byte _0800855A @ case 2
	.4byte _0800855A @ case 3
	.4byte _08008554 @ case 4
	.4byte _0800855A @ case 5
	.4byte _0800855A @ case 6
_08008554:
	adds r0, r2, #0
	adds r0, #0x1c
	b _0800855C
_0800855A:
	ldr r0, _08008560 @ =gStaticData_0816B2F8
_0800855C:
	pop {r1}
	bx r1
	.align 2, 0
_08008560: .4byte gStaticData_0816B2F8

	thumb_func_start sub_8008564
sub_8008564: @ 0x08008564
	push {lr}
	bl sub_80083B8
	adds r2, r0, #0
	ldr r0, [r2, #4]
	ldrb r0, [r0]
	lsrs r0, r0, #4
	cmp r0, #6
	bhi _080085AC
	lsls r0, r0, #2
	ldr r1, _08008580 @ =_08008584
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08008580: .4byte _08008584
_08008584: @ jump table
	.4byte _080085A0 @ case 0
	.4byte _080085AC @ case 1
	.4byte _080085AC @ case 2
	.4byte _080085A0 @ case 3
	.4byte _080085A0 @ case 4
	.4byte _080085A6 @ case 5
	.4byte _080085AC @ case 6
_080085A0:
	adds r0, r2, #0
	adds r0, #0x14
	b _080085AE
_080085A6:
	adds r0, r2, #0
	adds r0, #0xc
	b _080085AE
_080085AC:
	ldr r0, _080085B4 @ =gStaticData_0816B2F8
_080085AE:
	pop {r1}
	bx r1
	.align 2, 0
_080085B4: .4byte gStaticData_0816B2F8

	thumb_func_start sub_80085B8
sub_80085B8: @ 0x080085B8
	push {lr}
	bl sub_80083B8
	adds r2, r0, #0
	ldr r0, [r2, #4]
	ldrb r0, [r0]
	lsrs r0, r0, #4
	cmp r0, #6
	bhi _080085FA
	lsls r0, r0, #2
	ldr r1, _080085D4 @ =_080085D8
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080085D4: .4byte _080085D8
_080085D8: @ jump table
	.4byte _080085F4 @ case 0
	.4byte _080085FA @ case 1
	.4byte _080085F4 @ case 2
	.4byte _080085F4 @ case 3
	.4byte _080085F4 @ case 4
	.4byte _080085FA @ case 5
	.4byte _080085F4 @ case 6
_080085F4:
	adds r0, r2, #0
	adds r0, #0xc
	b _080085FC
_080085FA:
	ldr r0, _08008600 @ =gStaticData_0816B2F8
_080085FC:
	pop {r1}
	bx r1
	.align 2, 0
_08008600: .4byte gStaticData_0816B2F8

	thumb_func_start sub_8008604
sub_8008604: @ 0x08008604
	ldr r2, [r0, #0x20]
	adds r0, #0x2d
	ldrb r3, [r0]
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	ldr r0, [r2]
	adds r0, r0, r1
	bx lr
	.align 2, 0

