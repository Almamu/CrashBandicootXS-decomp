.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8001254
sub_8001254: @ 0x08001254
	adds r2, r0, #0
	adds r0, #0x24
	ldrb r0, [r0]
	cmp r0, #0
	beq _08001284
	ldr r1, [r2, #0x10]
	cmp r1, #0
	bgt _0800126C
	ldr r0, [r2, #0x14]
	adds r0, r1, r0
	str r0, [r2, #0x10]
	b _0800127A
_0800126C:
	ldr r0, [r2, #0x18]
	adds r0, r1, r0
	str r0, [r2, #0x10]
	ldr r0, [r2, #4]
	ldr r1, [r2, #0x20]
	adds r0, r0, r1
	str r0, [r2, #4]
_0800127A:
	ldr r0, [r2]
	ldr r1, [r2, #0x1c]
	adds r0, r0, r1
	str r0, [r2]
	b _080012A8
_08001284:
	ldr r1, [r2, #0x10]
	cmp r1, #0
	bgt _08001292
	ldr r0, [r2, #0x14]
	adds r0, r1, r0
	str r0, [r2, #0x10]
	b _080012A0
_08001292:
	ldr r0, [r2, #0x18]
	adds r0, r1, r0
	str r0, [r2, #0x10]
	ldr r0, [r2]
	ldr r1, [r2, #0x1c]
	adds r0, r0, r1
	str r0, [r2]
_080012A0:
	ldr r0, [r2, #4]
	ldr r1, [r2, #0x20]
	adds r0, r0, r1
	str r0, [r2, #4]
_080012A8:
	bx lr
	.align 2, 0

	thumb_func_start sub_80012AC
sub_80012AC: @ 0x080012AC
	push {r4, r5, lr}
	ldr r1, _080012E0 @ =gUnknown_030007F8
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
	ldr r2, _080012E4 @ =gUnknown_030007E8
	ldr r3, [r2]
	adds r4, r1, #0
	adds r5, r2, #0
	cmp r0, r3
	bne _0800131A
	movs r0, #0
	str r0, [r4]
	movs r0, #0x80
	ldrb r1, [r5, #8]
	ands r0, r1
	cmp r0, #0
	beq _080012F0
	ldr r3, _080012E8 @ =0x04000054
	ldr r2, _080012EC @ =gUnknown_030007F4
	ldr r1, [r2]
	movs r0, #0x10
	subs r0, r0, r1
	strh r0, [r3]
	adds r1, r2, #0
	b _080012F8
	.align 2, 0
_080012E0: .4byte gUnknown_030007F8
_080012E4: .4byte gUnknown_030007E8
_080012E8: .4byte 0x04000054
_080012EC: .4byte gUnknown_030007F4
_080012F0:
	ldr r2, _08001320 @ =0x04000054
	ldr r1, _08001324 @ =gUnknown_030007F4
	ldr r0, [r1]
	strh r0, [r2]
_080012F8:
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
	cmp r0, #0x11
	bne _0800131A
	movs r0, #0
	str r0, [r1]
	str r0, [r4]
	ldr r4, _08001328 @ =0x04000208
	strh r0, [r4]
	subs r0, #1
	str r0, [r5]
	ldr r0, [r5, #4]
	bl sub_8000670
	movs r0, #1
	strh r0, [r4]
_0800131A:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08001320: .4byte 0x04000054
_08001324: .4byte gUnknown_030007F4
_08001328: .4byte 0x04000208

	thumb_func_start sub_800132C
sub_800132C: @ 0x0800132C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r1, #0
	lsls r0, r0, #0x18
	lsrs r3, r0, #0x18
	lsls r2, r2, #0x18
	lsrs r4, r2, #0x18
	ldr r2, _08001360 @ =gUnknown_030007E8
	ldr r0, [r2]
	mvns r0, r0
	rsbs r1, r0, #0
	orrs r1, r0
	adds r5, r2, #0
	cmp r1, #0
	blt _080013F2
	cmp r6, #0
	bgt _08001352
	movs r6, #1
_08001352:
	movs r0, #1
	ands r0, r3
	cmp r0, #0
	beq _08001368
	ldr r1, _08001364 @ =0x04000050
	movs r0, #0xbf
	b _0800136C
	.align 2, 0
_08001360: .4byte gUnknown_030007E8
_08001364: .4byte 0x04000050
_08001368:
	ldr r1, _08001388 @ =0x04000050
	movs r0, #0xff
_0800136C:
	strh r0, [r1]
	cmp r4, #0
	beq _080013B8
	movs r0, #0x80
	ands r0, r3
	lsls r0, r0, #0x18
	lsrs r1, r0, #0x18
	cmp r1, #0
	beq _08001390
	ldr r1, _0800138C @ =0x04000054
	movs r0, #0x10
	strh r0, [r1]
	b _08001394
	.align 2, 0
_08001388: .4byte 0x04000050
_0800138C: .4byte 0x04000054
_08001390:
	ldr r0, _080013AC @ =0x04000054
	strh r1, [r0]
_08001394:
	ldr r4, _080013B0 @ =0x04000208
	movs r0, #0
	strh r0, [r4]
	strb r3, [r5, #8]
	str r6, [r5]
	ldr r0, _080013B4 @ =sub_80012AC
	bl sub_8000680
	str r0, [r5, #4]
	movs r0, #1
	strh r0, [r4]
	b _080013F2
	.align 2, 0
_080013AC: .4byte 0x04000054
_080013B0: .4byte 0x04000208
_080013B4: .4byte sub_80012AC
_080013B8:
	movs r1, #0
	movs r0, #0x80
	ands r0, r3
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	mov r8, r0
	ldr r7, _080013D4 @ =0x04000054
_080013C6:
	mov r0, r8
	cmp r0, #0
	beq _080013D8
	movs r0, #0x10
	subs r0, r0, r1
	strh r0, [r7]
	b _080013DA
	.align 2, 0
_080013D4: .4byte 0x04000054
_080013D8:
	strh r1, [r7]
_080013DA:
	adds r5, r1, #1
	cmp r6, #0
	ble _080013EC
	adds r4, r6, #0
_080013E2:
	bl sub_80006A8
	subs r4, #1
	cmp r4, #0
	bne _080013E2
_080013EC:
	adds r1, r5, #0
	cmp r1, #0x10
	ble _080013C6
_080013F2:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_80013FC
sub_80013FC: @ 0x080013FC
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	movs r6, #0
	ldr r0, _0800148C @ =gUnknown_03000A80
	mov ip, r0
	ldr r7, _08001490 @ =gUnknown_03000E80
_08001408:
	lsls r5, r6, #1
	mov r0, ip
	adds r1, r5, r0
	ldr r0, _08001494 @ =0xFFFF0000
	ands r2, r0
	ldrh r1, [r1]
	orrs r2, r1
	lsls r0, r2, #0x1b
	lsrs r1, r0, #0x1b
	adds r0, r1, #0
	muls r0, r4, r0
	cmp r0, #0
	bge _08001424
	adds r0, #0xf
_08001424:
	asrs r0, r0, #4
	subs r0, r1, r0
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r3, #0x1f
	ands r0, r3
	movs r1, #0x20
	rsbs r1, r1, #0
	ands r2, r1
	orrs r2, r0
	lsls r0, r2, #0x16
	lsrs r1, r0, #0x1b
	adds r0, r1, #0
	muls r0, r4, r0
	cmp r0, #0
	bge _08001446
	adds r0, #0xf
_08001446:
	asrs r0, r0, #4
	subs r0, r1, r0
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	ands r0, r3
	lsls r0, r0, #5
	ldr r1, _08001498 @ =0xFFFFFC1F
	ands r2, r1
	orrs r2, r0
	lsls r0, r2, #0x11
	lsrs r1, r0, #0x1b
	adds r0, r1, #0
	muls r0, r4, r0
	cmp r0, #0
	bge _08001466
	adds r0, #0xf
_08001466:
	asrs r0, r0, #4
	subs r0, r1, r0
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	ands r0, r3
	lsls r0, r0, #0xa
	ldr r1, _0800149C @ =0xFFFF83FF
	ands r2, r1
	orrs r2, r0
	adds r0, r5, r7
	strh r2, [r0]
	adds r6, #1
	ldr r0, _080014A0 @ =0x000001FF
	cmp r6, r0
	ble _08001408
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800148C: .4byte gUnknown_03000A80
_08001490: .4byte gUnknown_03000E80
_08001494: .4byte 0xFFFF0000
_08001498: .4byte 0xFFFFFC1F
_0800149C: .4byte 0xFFFF83FF
_080014A0: .4byte 0x000001FF

	thumb_func_start sub_80014A4
sub_80014A4: @ 0x080014A4
	push {r4, r5, r6, lr}
	ldr r1, _080014FC @ =0x040000D4
	movs r0, #0xa0
	lsls r0, r0, #0x13
	str r0, [r1]
	ldr r0, _08001500 @ =gUnknown_03000A80
	str r0, [r1, #4]
	ldr r0, _08001504 @ =0x80000200
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	movs r5, #0
	adds r4, r1, #0
	ldr r6, _08001508 @ =gUnknown_03000E80
_080014BE:
	adds r0, r5, #0
	bl sub_80013FC
	bl sub_80006A8
	str r6, [r4]
	movs r3, #0xa0
	lsls r3, r3, #0x13
	str r3, [r4, #4]
	ldr r2, _08001504 @ =0x80000200
	str r2, [r4, #8]
	ldr r0, [r4, #8]
	adds r5, #2
	cmp r5, #0x10
	ble _080014BE
	ldr r1, _0800150C @ =0x04000050
	movs r0, #0xff
	strh r0, [r1]
	adds r1, #4
	movs r0, #0x10
	strh r0, [r1]
	ldr r0, _080014FC @ =0x040000D4
	ldr r1, _08001500 @ =gUnknown_03000A80
	str r1, [r0]
	str r3, [r0, #4]
	str r2, [r0, #8]
	ldr r0, [r0, #8]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_080014FC: .4byte 0x040000D4
_08001500: .4byte gUnknown_03000A80
_08001504: .4byte 0x80000200
_08001508: .4byte gUnknown_03000E80
_0800150C: .4byte 0x04000050

	thumb_func_start sub_8001510
sub_8001510: @ 0x08001510
	ldr r0, _08001520 @ =gUnknown_030007E8
	ldr r1, [r0]
	mvns r1, r1
	rsbs r0, r1, #0
	orrs r0, r1
	lsrs r0, r0, #0x1f
	bx lr
	.align 2, 0
_08001520: .4byte gUnknown_030007E8

	thumb_func_start sub_8001524
sub_8001524: @ 0x08001524
	ldr r2, _08001538 @ =gUnknown_03001288
	movs r1, #7
	ands r0, r1
	movs r1, #8
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	bx lr
	.align 2, 0
_08001538: .4byte gUnknown_03001288

	thumb_func_start sub_800153C
sub_800153C: @ 0x0800153C
	ldr r1, _0800154C @ =gUnknown_03001288
	movs r0, #9
	rsbs r0, r0, #0
	ldrb r2, [r1, #1]
	ands r0, r2
	strb r0, [r1, #1]
	bx lr
	.align 2, 0
_0800154C: .4byte gUnknown_03001288

	thumb_func_start sub_8001550
sub_8001550: @ 0x08001550
	ldr r1, _08001560 @ =gUnknown_03001288
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r2, [r1, #1]
	ands r0, r2
	strb r0, [r1, #1]
	bx lr
	.align 2, 0
_08001560: .4byte gUnknown_03001288

	thumb_func_start sub_8001564
sub_8001564: @ 0x08001564
	ldr r1, _08001574 @ =gUnknown_03001288
	movs r0, #3
	rsbs r0, r0, #0
	ldrb r2, [r1, #1]
	ands r0, r2
	strb r0, [r1, #1]
	bx lr
	.align 2, 0
_08001574: .4byte gUnknown_03001288

	thumb_func_start sub_8001578
sub_8001578: @ 0x08001578
	ldr r1, _08001588 @ =gUnknown_03001288
	movs r0, #2
	rsbs r0, r0, #0
	ldrb r2, [r1, #1]
	ands r0, r2
	strb r0, [r1, #1]
	bx lr
	.align 2, 0
_08001588: .4byte gUnknown_03001288

	thumb_func_start sub_800158C
sub_800158C: @ 0x0800158C
	ldr r1, _0800159C @ =gUnknown_03001288
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r2, [r1, #1]
	ands r0, r2
	strb r0, [r1, #1]
	bx lr
	.align 2, 0
_0800159C: .4byte gUnknown_03001288

	thumb_func_start sub_80015A0
sub_80015A0: @ 0x080015A0
	ldr r1, _080015AC @ =gUnknown_03001288
	movs r0, #8
	ldrb r2, [r1, #1]
	orrs r0, r2
	strb r0, [r1, #1]
	bx lr
	.align 2, 0
_080015AC: .4byte gUnknown_03001288

	thumb_func_start sub_80015B0
sub_80015B0: @ 0x080015B0
	ldr r1, _080015BC @ =gUnknown_03001288
	movs r0, #4
	ldrb r2, [r1, #1]
	orrs r0, r2
	strb r0, [r1, #1]
	bx lr
	.align 2, 0
_080015BC: .4byte gUnknown_03001288

	thumb_func_start sub_80015C0
sub_80015C0: @ 0x080015C0
	ldr r1, _080015CC @ =gUnknown_03001288
	movs r0, #2
	ldrb r2, [r1, #1]
	orrs r0, r2
	strb r0, [r1, #1]
	bx lr
	.align 2, 0
_080015CC: .4byte gUnknown_03001288

	thumb_func_start sub_80015D0
sub_80015D0: @ 0x080015D0
	ldr r1, _080015DC @ =gUnknown_03001288
	movs r0, #1
	ldrb r2, [r1, #1]
	orrs r0, r2
	strb r0, [r1, #1]
	bx lr
	.align 2, 0
_080015DC: .4byte gUnknown_03001288

	thumb_func_start sub_80015E0
sub_80015E0: @ 0x080015E0
	ldr r1, _080015EC @ =gUnknown_03001288
	movs r0, #0x10
	ldrb r2, [r1, #1]
	orrs r0, r2
	strb r0, [r1, #1]
	bx lr
	.align 2, 0
_080015EC: .4byte gUnknown_03001288

	thumb_func_start sub_80015F0
sub_80015F0: @ 0x080015F0
	ldr r1, _08001600 @ =gUnknown_03001288
	movs r0, #0x41
	rsbs r0, r0, #0
	ldrb r2, [r1]
	ands r0, r2
	strb r0, [r1]
	bx lr
	.align 2, 0
_08001600: .4byte gUnknown_03001288

	thumb_func_start sub_8001604
sub_8001604: @ 0x08001604
	ldr r1, _08001610 @ =gUnknown_03001288
	movs r0, #0x40
	ldrb r2, [r1]
	orrs r0, r2
	strb r0, [r1]
	bx lr
	.align 2, 0
_08001610: .4byte gUnknown_03001288

	thumb_func_start sub_8001614
sub_8001614: @ 0x08001614
	movs r0, #0x80
	lsls r0, r0, #0x13
	ldr r1, _08001620 @ =gUnknown_03001288
	ldrh r1, [r1]
	strh r1, [r0]
	bx lr
	.align 2, 0
_08001620: .4byte gUnknown_03001288

	thumb_func_start sub_8001624
sub_8001624: @ 0x08001624
	ldr r2, _08001638 @ =0x04000050
	ldr r1, _0800163C @ =gUnknown_03001280
	ldr r0, [r1]
	str r0, [r2]
	adds r2, #4
	ldrb r1, [r1, #4]
	lsls r0, r1, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r2]
	bx lr
	.align 2, 0
_08001638: .4byte 0x04000050
_0800163C: .4byte gUnknown_03001280

	thumb_func_start sub_8001640
sub_8001640: @ 0x08001640
	push {r4, r5, r6, lr}
	adds r3, r0, #0
	adds r4, r1, #0
	movs r6, #0
	ldr r2, [r3, #8]
	cmp r2, #0
	ble _0800167E
	ldr r0, [r4, #8]
	cmp r0, #0
	ble _0800167E
	ldr r1, [r3]
	adds r5, r1, r2
	ldr r2, [r4]
	adds r0, r2, r0
	cmp r1, r0
	bgt _0800167E
	cmp r2, r5
	bgt _0800167E
	ldr r1, [r3, #4]
	ldr r0, [r3, #0xc]
	adds r3, r1, r0
	ldr r2, [r4, #4]
	ldr r0, [r4, #0xc]
	adds r0, r2, r0
	movs r4, #0
	cmp r1, r0
	bge _0800167C
	cmp r2, r3
	bge _0800167C
	movs r4, #1
_0800167C:
	adds r6, r4, #0
_0800167E:
	adds r0, r6, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8001688
sub_8001688: @ 0x08001688
	push {r4, r5, r6, lr}
	adds r3, r0, #0
	adds r4, r1, #0
	movs r6, #0
	ldr r2, [r3, #8]
	cmp r2, #0
	ble _080016C6
	ldr r0, [r4, #8]
	cmp r0, #0
	ble _080016C6
	ldr r1, [r3]
	adds r5, r1, r2
	ldr r2, [r4]
	adds r0, r2, r0
	cmp r1, r0
	bge _080016C6
	cmp r2, r5
	bge _080016C6
	ldr r1, [r3, #4]
	ldr r0, [r3, #0xc]
	adds r3, r1, r0
	ldr r2, [r4, #4]
	ldr r0, [r4, #0xc]
	adds r0, r2, r0
	movs r4, #0
	cmp r1, r0
	bge _080016C4
	cmp r2, r3
	bge _080016C4
	movs r4, #1
_080016C4:
	adds r6, r4, #0
_080016C6:
	adds r0, r6, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_80016D0
sub_80016D0: @ 0x080016D0
	push {lr}
	bl mem_free
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80016DC
sub_80016DC: @ 0x080016DC
	push {lr}
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_80016EC
sub_80016EC: @ 0x080016EC
	push {r4, lr}
	adds r4, r0, #0
	movs r1, #0
	ldr r0, [r4, #4]
	cmp r0, #1
	bne _080016FA
	movs r1, #1
_080016FA:
	cmp r1, #0
	beq _080017B4
	adds r2, r4, #0
	adds r2, #0x50
	ldrb r0, [r2]
	cmp r0, #0
	beq _08001724
	ldr r0, [r4, #0x18]
	ldr r1, [r4, #0x1c]
	cmp r0, r1
	blt _08001718
	str r1, [r4, #0x18]
	movs r0, #0
	strb r0, [r2]
	b _0800171C
_08001718:
	adds r0, #0x10
	str r0, [r4, #0x18]
_0800171C:
	ldr r1, [r4, #0x18]
	adds r0, r4, #0
	adds r0, #0x68
	strh r1, [r0]
_08001724:
	adds r2, r4, #0
	adds r2, #0x51
	ldrb r0, [r2]
	cmp r0, #0
	beq _0800174A
	ldr r0, [r4, #0x18]
	ldr r1, [r4, #0x1c]
	cmp r0, r1
	bgt _0800173E
	str r1, [r4, #0x18]
	movs r0, #0
	strb r0, [r2]
	b _08001742
_0800173E:
	subs r0, #0x10
	str r0, [r4, #0x18]
_08001742:
	ldr r1, [r4, #0x18]
	adds r0, r4, #0
	adds r0, #0x68
	strh r1, [r0]
_0800174A:
	adds r0, r4, #0
	bl sub_800190C
	adds r2, r4, #0
	adds r2, #0x52
	ldrb r0, [r2]
	cmp r0, #0
	beq _08001778
	ldr r0, [r4, #0x24]
	ldr r1, [r4, #0x28]
	cmp r0, r1
	blt _0800176A
	str r1, [r4, #0x24]
	movs r0, #0
	strb r0, [r2]
	b _0800176E
_0800176A:
	adds r0, #0x10
	str r0, [r4, #0x24]
_0800176E:
	movs r0, #1
	rsbs r0, r0, #0
	ldr r1, [r4, #0x24]
	bl sub_8039064
_08001778:
	adds r2, r4, #0
	adds r2, #0x53
	ldrb r0, [r2]
	cmp r0, #0
	beq _080017B0
	ldr r0, [r4, #0x24]
	ldr r1, [r4, #0x28]
	cmp r0, r1
	bgt _080017A2
	str r1, [r4, #0x24]
	movs r0, #0
	strb r0, [r2]
	ldr r1, [r4, #0xc]
	cmp r1, #0x13
	beq _080017A6
	adds r0, r4, #0
	bl sub_80017BC
	movs r0, #0x13
	str r0, [r4, #0xc]
	b _080017A6
_080017A2:
	subs r0, #0x10
	str r0, [r4, #0x24]
_080017A6:
	movs r0, #1
	rsbs r0, r0, #0
	ldr r1, [r4, #0x24]
	bl sub_8039064
_080017B0:
	bl sub_8038C88
_080017B4:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80017BC
sub_80017BC: @ 0x080017BC
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	adds r7, r1, #0
	movs r1, #0
	ldr r0, [r5, #4]
	cmp r0, #0
	bne _080017CC
	movs r1, #1
_080017CC:
	adds r4, r1, #0
	cmp r4, #0
	bne _080017E2
	movs r0, #0x13
	str r0, [r5, #0xc]
	str r0, [r5, #8]
	str r4, [r5, #4]
	bl sub_8039198
	ldr r0, _08001848 @ =gUnknown_030007DD
	strb r4, [r0]
_080017E2:
	adds r4, r5, #0
	adds r4, #0x58
	adds r0, r4, #0
	bl sub_80381FC
	adds r0, r5, #0
	adds r0, #0x94
	str r0, [r5, #0x58]
	movs r0, #0x80
	lsls r0, r0, #6
	str r0, [r5, #0x5c]
	adds r2, r5, #0
	adds r2, #0x88
	ldr r1, _0800184C @ =gStaticData_0816AA20
	lsls r0, r7, #2
	adds r0, r0, r1
	ldr r0, [r0]
	str r0, [r2]
	adds r1, r5, #0
	adds r1, #0x66
	movs r6, #0
	movs r0, #3
	strh r0, [r1]
	adds r1, #0x1e
	ldr r0, _08001850 @ =gStaticData_0855BCB4
	str r0, [r1]
	adds r0, r5, #0
	adds r0, #0x90
	strb r6, [r0]
	adds r0, r4, #0
	bl sub_8038538
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08001840
	str r7, [r5, #8]
	adds r0, r5, #0
	adds r0, #0x54
	strb r6, [r0]
	adds r0, r5, #0
	bl sub_8001AD8
	ldr r1, _08001848 @ =gUnknown_030007DD
	movs r0, #1
	strb r0, [r1]
	movs r0, #1
	str r0, [r5, #4]
_08001840:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08001848: .4byte gUnknown_030007DD
_0800184C: .4byte gStaticData_0816AA20
_08001850: .4byte gStaticData_0855BCB4

	thumb_func_start PlaySfx
PlaySfx: @ 0x08001854
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov sb, r0
	mov sl, r1
	str r2, [sp]
	movs r1, #0
	ldr r0, [r0, #4]
	cmp r0, #1
	bne _08001870
	movs r1, #1
_08001870:
	cmp r1, #0
	beq _080018F4
	mov r1, sl
	lsls r0, r1, #1
	add r0, sl
	lsls r5, r0, #2
	ldr r2, _08001904 @ =gStaticData_0816AA6C
	adds r0, r5, r2
	ldr r4, [r0]
	cmp r4, #0
	beq _080018F4
	ldr r0, _08001908 @ =gUnknown_030007FC
	mov r8, r0
	ldr r1, [r0]
	adds r0, r2, #0
	adds r0, #4
	adds r0, r5, r0
	ldr r7, [r0]
	movs r6, #1
	rsbs r6, r6, #0
	adds r0, r4, #0
	adds r2, r7, #0
	adds r3, r6, #0
	bl sub_8038E74
	adds r3, r0, #0
	cmp r3, r6
	bne _080018C0
	mov r2, r8
	ldr r1, [r2]
	movs r0, #1
	eors r1, r0
	str r1, [r2]
	adds r0, r4, #0
	adds r2, r7, #0
	bl sub_8038E74
	adds r3, r0, #0
	cmp r3, r6
	beq _080018F4
_080018C0:
	ldr r0, _08001904 @ =gStaticData_0816AA6C
	adds r0, #8
	adds r0, r5, r0
	ldr r1, [r0]
	mov r2, sb
	ldr r0, [r2, #0x2c]
	muls r0, r1, r0
	ldr r2, [sp]
	adds r1, r0, #0
	muls r1, r2, r1
	lsrs r1, r1, #0x10
	adds r0, r3, #0
	bl sub_80390F8
	mov r0, r8
	ldr r1, [r0]
	lsls r2, r1, #2
	mov r0, sb
	adds r0, #0x10
	adds r0, r0, r2
	mov r2, sl
	str r2, [r0]
	movs r0, #1
	eors r1, r0
	mov r0, r8
	str r1, [r0]
_080018F4:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08001904: .4byte gStaticData_0816AA6C
_08001908: .4byte gUnknown_030007FC

	thumb_func_start sub_800190C
sub_800190C: @ 0x0800190C
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x38]
	cmp r0, #0x63
	beq _080019A2
	ldr r0, _08001968 @ =gUnknown_0300082C
	ldr r1, [r0]
	ldr r0, [r4, #0x3c]
	cmp r1, r0
	blo _08001970
	movs r1, #0
	str r1, [r4, #0x40]
	ldr r0, [r4, #0x34]
	subs r0, #0x10
	str r0, [r4, #0x34]
	cmp r0, #0
	bgt _08001982
	str r1, [r4, #0x34]
	movs r0, #2
	bl sub_8038FD0
	adds r1, r4, #0
	adds r1, #0x38
	adds r0, r4, #0
	adds r0, #0x44
	ldm r0!, {r2, r3, r5}
	stm r1!, {r2, r3, r5}
	ldr r2, [r4, #0x38]
	cmp r2, #0x63
	beq _08001982
	movs r0, #0x63
	str r0, [r4, #0x44]
	ldr r1, _0800196C @ =gStaticData_0816AA6C
	lsls r0, r2, #1
	adds r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	movs r3, #1
	rsbs r3, r3, #0
	movs r1, #2
	movs r2, #0
	bl sub_8038E74
	b _08001982
	.align 2, 0
_08001968: .4byte gUnknown_0300082C
_0800196C: .4byte gStaticData_0816AA6C
_08001970:
	ldr r0, [r4, #0x34]
	ldr r1, [r4, #0x40]
	cmp r0, r1
	bge _0800198C
	adds r0, #0x10
	str r0, [r4, #0x34]
	cmp r0, r1
	ble _08001982
	str r1, [r4, #0x34]
_08001982:
	ldr r1, [r4, #0x34]
	movs r0, #2
	bl sub_80390F8
	b _080019A2
_0800198C:
	cmp r0, r1
	ble _080019A2
	subs r0, #0x10
	str r0, [r4, #0x34]
	cmp r0, r1
	bge _0800199A
	str r1, [r4, #0x34]
_0800199A:
	ldr r1, [r4, #0x34]
	movs r0, #2
	bl sub_80390F8
_080019A2:
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_80019A8
sub_80019A8: @ 0x080019A8
	push {r4, r5, r6, lr}
	adds r6, r1, #0
	movs r5, #0
	adds r4, r0, #0
	adds r4, #0x10
_080019B2:
	ldr r0, [r4]
	cmp r0, r6
	bne _080019BE
	adds r0, r5, #0
	bl sub_8038FD0
_080019BE:
	adds r4, #4
	adds r5, #1
	cmp r5, #1
	ble _080019B2
	pop {r4, r5, r6}
	pop {r0}
	bx r0

	thumb_func_start sub_80019CC
sub_80019CC: @ 0x080019CC
	push {lr}
	movs r1, #0x63
	str r1, [r0, #0x44]
	str r1, [r0, #0x38]
	movs r1, #0
	str r1, [r0, #0x34]
	str r1, [r0, #0x4c]
	str r1, [r0, #0x40]
	movs r0, #2
	bl sub_8038FD0
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80019E8
sub_80019E8: @ 0x080019E8
	movs r1, #0x63
	str r1, [r0, #0x44]
	ldr r1, _080019F4 @ =gUnknown_0300082C
	ldr r1, [r1]
	str r1, [r0, #0x3c]
	bx lr
	.align 2, 0
_080019F4: .4byte gUnknown_0300082C

	thumb_func_start sub_80019F8
sub_80019F8: @ 0x080019F8
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	adds r6, r1, #0
	adds r7, r2, #0
	add r0, sp, #0x14
	ldrb r0, [r0]
	mov ip, r0
	ldr r5, _08001A58 @ =gStaticData_0816AA6C
	lsls r0, r6, #1
	adds r0, r0, r6
	lsls r1, r0, #2
	adds r0, r1, r5
	ldr r2, [r0]
	cmp r2, #0
	beq _08001AAE
	cmp r3, #0
	ble _08001AAE
	adds r0, r5, #0
	adds r0, #8
	adds r0, r1, r0
	ldr r0, [r0]
	adds r1, r0, #0
	muls r1, r3, r1
	ldr r0, [r4, #0x2c]
	muls r0, r1, r0
	lsrs r5, r0, #0x10
	ldr r1, [r4, #0x38]
	cmp r1, #0x63
	bne _08001A60
	movs r3, #1
	rsbs r3, r3, #0
	adds r0, r2, #0
	movs r1, #2
	movs r2, #0
	bl sub_8038E74
	ldr r1, [r4, #0x34]
	movs r0, #2
	bl sub_80390F8
	str r6, [r4, #0x38]
	ldr r0, _08001A5C @ =gUnknown_0300082C
	ldr r0, [r0]
	adds r0, r0, r7
	str r0, [r4, #0x3c]
	str r5, [r4, #0x40]
	b _08001AAE
	.align 2, 0
_08001A58: .4byte gStaticData_0816AA6C
_08001A5C: .4byte gUnknown_0300082C
_08001A60:
	ldr r0, [r4, #0x40]
	cmp r5, r0
	blt _08001AAE
	cmp r1, r6
	bne _08001AA0
	ldr r0, _08001A9C @ =gUnknown_0300082C
	ldr r0, [r0]
	adds r0, r0, r7
	str r0, [r4, #0x3c]
	str r5, [r4, #0x40]
	mov r0, ip
	cmp r0, #0
	beq _08001A90
	movs r3, #1
	rsbs r3, r3, #0
	adds r0, r2, #0
	movs r1, #2
	movs r2, #0
	bl sub_8038E74
	ldr r1, [r4, #0x34]
	movs r0, #2
	bl sub_80390F8
_08001A90:
	movs r0, #0x63
	str r0, [r4, #0x44]
	movs r0, #0
	str r0, [r4, #0x4c]
	b _08001AAE
	.align 2, 0
_08001A9C: .4byte gUnknown_0300082C
_08001AA0:
	str r6, [r4, #0x44]
	ldr r0, _08001AB4 @ =gUnknown_0300082C
	ldr r1, [r0]
	adds r0, r1, r7
	str r0, [r4, #0x48]
	str r5, [r4, #0x4c]
	str r1, [r4, #0x3c]
_08001AAE:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08001AB4: .4byte gUnknown_0300082C

	thumb_func_start sub_8001AB8
sub_8001AB8: @ 0x08001AB8
	ldr r0, [r0, #8]
	bx lr

	thumb_func_start sub_8001ABC
sub_8001ABC: @ 0x08001ABC
	ldr r0, [r0, #0x2c]
	bx lr

	thumb_func_start sub_8001AC0
sub_8001AC0: @ 0x08001AC0
	ldr r0, [r0, #0x20]
	bx lr

	thumb_func_start sub_8001AC4
sub_8001AC4: @ 0x08001AC4
	str r1, [r0, #0x28]
	adds r2, r0, #0
	adds r2, #0x52
	movs r1, #0
	strb r1, [r2]
	adds r0, #0x53
	movs r1, #1
	strb r1, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8001AD8
sub_8001AD8: @ 0x08001AD8
	ldr r1, [r0, #0x20]
	str r1, [r0, #0x28]
	adds r3, r0, #0
	adds r3, #0x52
	movs r2, #0
	movs r1, #1
	strb r1, [r3]
	adds r0, #0x53
	strb r2, [r0]
	bx lr

	thumb_func_start sub_8001AEC
sub_8001AEC: @ 0x08001AEC
	str r1, [r0, #0x1c]
	adds r2, r0, #0
	adds r2, #0x50
	movs r1, #0
	strb r1, [r2]
	adds r0, #0x51
	movs r1, #1
	strb r1, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8001B00
sub_8001B00: @ 0x08001B00
	str r1, [r0, #0x1c]
	adds r2, r0, #0
	adds r2, #0x50
	movs r3, #0
	movs r1, #1
	strb r1, [r2]
	adds r0, #0x51
	strb r3, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8001B14
sub_8001B14: @ 0x08001B14
	adds r2, r0, #0
	str r1, [r2, #0x30]
	movs r3, #0
	ldr r0, [r2, #4]
	cmp r0, #1
	bne _08001B22
	movs r3, #1
_08001B22:
	cmp r3, #0
	beq _08001B2C
	adds r0, r2, #0
	adds r0, #0x62
	strh r1, [r0]
_08001B2C:
	bx lr
	.align 2, 0

	thumb_func_start sub_8001B30
sub_8001B30: @ 0x08001B30
	push {lr}
	str r1, [r0, #0x24]
	str r1, [r0, #0x20]
	movs r2, #0
	ldr r0, [r0, #4]
	cmp r0, #1
	bne _08001B40
	movs r2, #1
_08001B40:
	cmp r2, #0
	beq _08001B4C
	movs r0, #1
	rsbs r0, r0, #0
	bl sub_8039064
_08001B4C:
	pop {r0}
	bx r0

	thumb_func_start sub_8001B50
sub_8001B50: @ 0x08001B50
	str r1, [r0, #0x2c]
	bx lr

	thumb_func_start sub_8001B54
sub_8001B54: @ 0x08001B54
	push {lr}
	adds r2, r0, #0
	movs r3, #0
	ldr r0, [r2, #4]
	cmp r0, #1
	bne _08001B62
	movs r3, #1
_08001B62:
	cmp r3, #0
	bne _08001B6E
	adds r0, r2, #0
	bl sub_80017BC
	b _08001B84
_08001B6E:
	ldr r0, [r2, #8]
	cmp r1, r0
	beq _08001B84
	ldr r0, [r2, #0xc]
	cmp r1, r0
	beq _08001B84
	str r1, [r2, #0xc]
	adds r0, r2, #0
	movs r1, #0
	bl sub_8001AC4
_08001B84:
	pop {r0}
	bx r0

	thumb_func_start sub_8001B88
sub_8001B88: @ 0x08001B88
	push {r4, lr}
	adds r4, r0, #0
	movs r1, #0
	ldr r0, [r4, #4]
	cmp r0, #2
	bne _08001B96
	movs r1, #1
_08001B96:
	cmp r1, #0
	beq _08001BA6
	bl sub_80006A8
	bl sub_8038C50
	movs r0, #1
	str r0, [r4, #4]
_08001BA6:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8001BAC
sub_8001BAC: @ 0x08001BAC
	push {lr}
	adds r1, r0, #0
	movs r2, #0
	ldr r0, [r1, #4]
	cmp r0, #1
	bne _08001BBA
	movs r2, #1
_08001BBA:
	cmp r2, #0
	beq _08001BCE
	movs r0, #2
	str r0, [r1, #4]
	bl sub_80006A8
	bl sub_8038C88
	bl sub_8038C28
_08001BCE:
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8001BD4
sub_8001BD4: @ 0x08001BD4
	push {r4, lr}
	adds r1, r0, #0
	movs r2, #0
	ldr r0, [r1, #4]
	cmp r0, #0
	bne _08001BE2
	movs r2, #1
_08001BE2:
	adds r4, r2, #0
	cmp r4, #0
	bne _08001BF8
	movs r0, #0x13
	str r0, [r1, #0xc]
	str r0, [r1, #8]
	str r4, [r1, #4]
	bl sub_8039198
	ldr r0, _08001C00 @ =gUnknown_030007DD
	strb r4, [r0]
_08001BF8:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08001C00: .4byte gUnknown_030007DD

	thumb_func_start sub_8001C04
sub_8001C04: @ 0x08001C04
	push {r4, r5, lr}
	adds r5, r0, #0
	adds r4, r1, #0
	bl sub_8001BD4
	ldr r1, _08001C28 @ =gUnknown_030007DD
	movs r0, #0
	strb r0, [r1]
	movs r0, #1
	ands r0, r4
	cmp r0, #0
	beq _08001C22
	adds r0, r5, #0
	bl sub_80016D0
_08001C22:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08001C28: .4byte gUnknown_030007DD

	thumb_func_start sub_8001C2C
sub_8001C2C: @ 0x08001C2C
	adds r2, r0, #0
	movs r1, #0
	str r1, [r2, #4]
	movs r0, #0x13
	str r0, [r2, #0xc]
	str r0, [r2, #8]
	adds r0, #0xed
	str r0, [r2, #0x18]
	str r0, [r2, #0x24]
	str r0, [r2, #0x20]
	str r0, [r2, #0x2c]
	str r1, [r2, #0x34]
	movs r0, #0x63
	str r0, [r2, #0x38]
	adds r0, r2, #0
	adds r0, #0x51
	strb r1, [r0]
	subs r0, #1
	strb r1, [r0]
	adds r0, #3
	strb r1, [r0]
	subs r0, #1
	strb r1, [r0]
	adds r0, #2
	strb r1, [r0]
	str r1, [r2, #0x30]
	adds r0, r2, #0
	bx lr

	thumb_func_start sub_8001C64
sub_8001C64: @ 0x08001C64
	push {lr}
	ldr r1, _08001C7C @ =0x04000004
	movs r0, #0x21
	rsbs r0, r0, #0
	ldrb r2, [r1]
	ands r0, r2
	strb r0, [r1]
	movs r0, #2
	bl sub_8000558
	pop {r0}
	bx r0
	.align 2, 0
_08001C7C: .4byte 0x04000004

	thumb_func_start sub_8001C80
sub_8001C80: @ 0x08001C80
	push {lr}
	ldr r1, _08001C9C @ =sub_8001CA4
	movs r0, #2
	bl sub_80005A0
	ldr r1, _08001CA0 @ =0x04000004
	movs r0, #0x35
	strb r0, [r1, #1]
	movs r0, #0x20
	ldrb r2, [r1]
	orrs r0, r2
	strb r0, [r1]
	pop {r0}
	bx r0
	.align 2, 0
_08001C9C: .4byte sub_8001CA4
_08001CA0: .4byte 0x04000004

	thumb_func_start sub_8001CA4
sub_8001CA4: @ 0x08001CA4
	push {lr}
	ldr r0, _08001CB4 @ =gUnknown_030012BC
	ldr r0, [r0]
	bl sub_80016EC
	pop {r0}
	bx r0
	.align 2, 0
_08001CB4: .4byte gUnknown_030012BC

	thumb_func_start sub_8001CB8
sub_8001CB8: @ 0x08001CB8
	push {r4, r5, r6, r7, lr}
	adds r3, r0, #0
	movs r1, #0xec
	adds r0, r3, #7
_08001CC0:
	strb r1, [r0]
	subs r0, #1
	cmp r0, r3
	bge _08001CC0
	movs r0, #0xf
	ldrb r1, [r3]
	ands r0, r1
	strb r0, [r3]
	movs r0, #0
	strb r0, [r3, #1]
	ldr r1, _08001D28 @ =0x00001234
	adds r2, r3, #1
	movs r4, #4
	ldr r7, _08001D2C @ =gStaticData_0816AF10
	mov ip, r7
	movs r6, #0xff
	movs r5, #1
	rsbs r5, r5, #0
_08001CE4:
	lsrs r0, r1, #8
	ldrb r7, [r2]
	eors r0, r7
	ands r0, r6
	lsls r0, r0, #1
	add r0, ip
	lsls r1, r1, #8
	ldrh r0, [r0]
	eors r1, r0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	adds r2, #1
	subs r4, #1
	cmp r4, r5
	bne _08001CE4
	strb r1, [r3, #6]
	lsrs r0, r1, #8
	strb r0, [r3, #7]
	ldrb r2, [r3]
	lsrs r1, r2, #4
	lsls r0, r0, #8
	ldrb r4, [r3, #6]
	orrs r0, r4
	adds r1, r1, r0
	movs r0, #0xf
	ands r1, r0
	subs r0, #0x1f
	ands r0, r2
	orrs r0, r1
	strb r0, [r3]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08001D28: .4byte 0x00001234
_08001D2C: .4byte gStaticData_0816AF10

	thumb_func_start sub_8001D30
sub_8001D30: @ 0x08001D30
	push {r4, r5, lr}
	ldr r4, _08001D94 @ =0x04000208
	movs r5, #0
	strh r5, [r4]
	ldrh r1, [r4]
	strh r5, [r4]
	ldr r3, _08001D98 @ =0x04000200
	ldrh r2, [r3]
	ldr r0, _08001D9C @ =0x0000FF7F
	ands r0, r2
	strh r0, [r3]
	strh r1, [r4]
	ldrh r1, [r4]
	strh r5, [r4]
	ldrh r2, [r3]
	ldr r0, _08001DA0 @ =0x0000FFBF
	ands r0, r2
	strh r0, [r3]
	strh r1, [r4]
	movs r0, #7
	bl sub_8000544
	movs r0, #6
	bl sub_8000544
	movs r0, #1
	strh r0, [r4]
	ldr r0, _08001DA4 @ =0x04000134
	strh r5, [r0]
	ldr r1, _08001DA8 @ =0x04000128
	movs r2, #0xc0
	lsls r2, r2, #6
	adds r0, r2, #0
	strh r0, [r1]
	subs r1, #0x1c
	ldr r0, _08001DAC @ =0x0000BBBC
	str r0, [r1]
	ldr r2, _08001DB0 @ =0x04000202
	ldrh r0, [r2]
	movs r1, #0x80
	orrs r0, r1
	strh r0, [r2]
	ldrh r0, [r2]
	movs r1, #0x40
	orrs r0, r1
	strh r0, [r2]
	movs r0, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08001D94: .4byte 0x04000208
_08001D98: .4byte 0x04000200
_08001D9C: .4byte 0x0000FF7F
_08001DA0: .4byte 0x0000FFBF
_08001DA4: .4byte 0x04000134
_08001DA8: .4byte 0x04000128
_08001DAC: .4byte 0x0000BBBC
_08001DB0: .4byte 0x04000202

	thumb_func_start sub_8001DB4
sub_8001DB4: @ 0x08001DB4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	adds r5, r0, #0
	movs r2, #0
	strb r2, [r5, #6]
	strb r2, [r5, #8]
	strb r2, [r5, #7]
	ldr r1, _08001F40 @ =gUnknown_03000800
	movs r0, #1
	strb r0, [r1]
	strb r2, [r5, #4]
	movs r1, #1
	rsbs r1, r1, #0
	str r1, [r5, #0x1c]
	movs r3, #0xff
	lsls r3, r3, #2
	adds r0, r5, r3
	str r1, [r0]
	adds r0, r5, #0
	adds r0, #0xc4
	str r2, [r0]
	adds r0, #4
	str r2, [r0]
	adds r1, r5, #0
	adds r1, #0xcc
	movs r0, #0x7f
	str r0, [r1]
	adds r4, r5, #0
	adds r4, #0x30
	adds r0, r4, #0
	bl sub_8001CB8
	adds r2, r5, #0
	adds r2, #0x28
	movs r6, #0xff
	movs r3, #3
_08001E04:
	ldrb r7, [r2, #9]
	lsls r1, r7, #8
	ldrb r0, [r2, #8]
	orrs r1, r0
	adds r0, r1, #0
	ands r0, r6
	strb r0, [r2]
	lsrs r1, r1, #8
	strb r1, [r2, #1]
	adds r2, #2
	subs r3, #1
	cmp r3, #0
	bge _08001E04
	movs r0, #0
	str r0, [r5, #0xc]
	str r0, [r5, #0x24]
	movs r6, #0
	adds r1, r5, #0
	adds r1, #0xfc
	str r1, [sp, #8]
	adds r2, r5, #0
	adds r2, #0x20
	str r2, [sp, #0xc]
	movs r3, #0xc8
	mov sl, r3
	movs r7, #0x80
	lsls r7, r7, #1
	adds r7, r5, r7
	str r7, [sp]
	str r4, [sp, #4]
_08001E40:
	mov r0, sl
	muls r0, r6, r0
	adds r0, r0, r5
	movs r1, #0x84
	lsls r1, r1, #1
	adds r0, r0, r1
	adds r1, r0, #0
	adds r1, #0x84
	movs r2, #0
	str r2, [r1]
	adds r1, #4
	str r2, [r1]
	adds r0, #0x8c
	movs r1, #0x7f
	str r1, [r0]
	mov r0, sl
	muls r0, r6, r0
	ldr r3, [sp]
	adds r0, r3, r0
	str r2, [r0]
	movs r4, #0
	adds r7, r6, #1
	mov r8, r7
	movs r0, #0xc8
	adds r1, r6, #0
	muls r1, r0, r1
	adds r0, r5, #0
	adds r0, #0xd0
	adds r2, r1, r0
	ldr r3, [sp, #4]
_08001E7C:
	ldrb r0, [r3, #1]
	lsls r1, r0, #8
	ldrb r7, [r3]
	orrs r1, r7
	adds r0, r1, #0
	movs r7, #0xff
	ands r0, r7
	strb r0, [r2]
	lsrs r1, r1, #8
	strb r1, [r2, #1]
	adds r2, #2
	adds r3, #2
	adds r4, #1
	cmp r4, #3
	ble _08001E7C
	mov r2, sl
	muls r2, r6, r2
	adds r0, r5, r2
	mov ip, r0
	mov r4, ip
	adds r4, #0xd1
	ldrb r3, [r4]
	lsls r1, r3, #0x1c
	lsrs r1, r1, #0x1c
	subs r1, #1
	movs r0, #0xf
	ands r1, r0
	movs r7, #0x10
	rsbs r7, r7, #0
	mov sb, r7
	mov r0, sb
	ands r0, r3
	orrs r0, r1
	strb r0, [r4]
	movs r1, #0x82
	lsls r1, r1, #1
	adds r0, r5, r1
	adds r0, r0, r2
	movs r3, #0
	str r3, [r0]
	ldr r7, [sp, #8]
	adds r2, r7, r2
	str r3, [r2]
	mov r0, ip
	adds r0, #0xd6
	ldr r1, _08001F44 @ =0x00001234
	strh r1, [r0]
	adds r0, #2
	adds r2, r1, #0
	strh r2, [r0]
	mov r6, r8
	cmp r6, #3
	ble _08001E40
	movs r3, #0xfc
	lsls r3, r3, #2
	adds r0, r5, r3
	movs r1, #0
	str r1, [r0]
	movs r7, #0xfd
	lsls r7, r7, #2
	adds r0, r5, r7
	str r1, [r0]
	movs r2, #0xfe
	lsls r2, r2, #2
	adds r0, r5, r2
	str r1, [r0]
	str r1, [r5, #0x38]
	str r1, [r5, #0x3c]
	movs r0, #0xf
	ldrh r3, [r5, #0x20]
	ands r0, r3
	ldr r7, _08001F48 @ =0x0000F0B0
	adds r1, r7, #0
	orrs r0, r1
	strh r0, [r5, #0x20]
	mov r0, sb
	ldr r1, [sp, #0xc]
	ldrb r1, [r1]
	ands r0, r1
	ldr r2, [sp, #0xc]
	strb r0, [r2]
	movs r3, #0x80
	lsls r3, r3, #3
	adds r1, r5, r3
	ldrh r0, [r5, #0x20]
	strh r0, [r1]
	ldrh r1, [r1]
	ldr r0, _08001F4C @ =0x0400012A
	strh r1, [r0]
	movs r0, #0
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08001F40: .4byte gUnknown_03000800
_08001F44: .4byte 0x00001234
_08001F48: .4byte 0x0000F0B0
_08001F4C: .4byte 0x0400012A

	thumb_func_start sub_8001F50
sub_8001F50: @ 0x08001F50
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	ldrb r0, [r5, #5]
	cmp r0, #0
	beq _08001FB6
	ldrb r1, [r5, #6]
	cmp r1, #0
	bne _08001F82
	ldr r0, _08001FBC @ =0x04000134
	strh r1, [r0]
	ldr r2, _08001FC0 @ =0x04000128
	movs r1, #0x80
	lsls r1, r1, #6
	adds r0, r1, #0
	strh r0, [r2]
	ldrh r0, [r2]
	ldr r3, _08001FC4 @ =0x00004003
	adds r1, r3, #0
	orrs r0, r1
	strh r0, [r2]
	movs r0, #1
	strb r0, [r5, #6]
_08001F82:
	ldrb r7, [r5, #8]
	cmp r7, #0
	bne _0800204A
	ldr r4, _08001FC0 @ =0x04000128
	ldrh r0, [r4]
	lsrs r0, r0, #3
	movs r1, #1
	movs r2, #1
	mov sb, r2
	ands r0, r2
	cmp r0, #0
	bne _08001FC8
	adds r0, r5, #0
	bl sub_8001D30
	ldr r0, _08001FBC @ =0x04000134
	strh r7, [r0]
	movs r3, #0x80
	lsls r3, r3, #6
	adds r0, r3, #0
	strh r0, [r4]
	ldrh r0, [r4]
	ldr r2, _08001FC4 @ =0x00004003
	adds r1, r2, #0
	orrs r0, r1
	strh r0, [r4]
_08001FB6:
	movs r0, #0
	b _08002106
	.align 2, 0
_08001FBC: .4byte 0x04000134
_08001FC0: .4byte 0x04000128
_08001FC4: .4byte 0x00004003
_08001FC8:
	mov r3, sb
	strb r3, [r5, #8]
	ldrh r4, [r4]
	lsrs r4, r4, #2
	eors r4, r1
	ands r4, r1
	ldr r0, _08002080 @ =0x04000208
	mov r8, r0
	strh r7, [r0]
	ldrh r1, [r0]
	strh r7, [r0]
	ldr r6, _08002084 @ =0x04000200
	ldrh r2, [r6]
	ldr r0, _08002088 @ =0x0000FF7F
	ands r0, r2
	strh r0, [r6]
	mov r2, r8
	strh r1, [r2]
	ldrh r1, [r2]
	strh r7, [r2]
	ldrh r2, [r6]
	ldr r0, _0800208C @ =0x0000FFBF
	ands r0, r2
	strh r0, [r6]
	mov r3, r8
	strh r1, [r3]
	movs r0, #6
	bl sub_8000544
	ldr r1, _08002090 @ =sub_8002830
	movs r0, #7
	bl sub_80005A0
	ldrh r0, [r6]
	movs r1, #0x80
	orrs r0, r1
	strh r0, [r6]
	cmp r4, #0
	beq _0800202C
	ldr r1, _08002094 @ =sub_8002848
	movs r0, #6
	bl sub_80005A0
	ldrh r0, [r6]
	movs r1, #0x40
	orrs r0, r1
	strh r0, [r6]
	ldr r1, _08002098 @ =0x0400010C
	ldr r0, _0800209C @ =0x00C0BBBC
	str r0, [r1]
_0800202C:
	mov r1, sb
	mov r0, r8
	strh r1, [r0]
	movs r2, #0xff
	lsls r2, r2, #2
	adds r1, r5, r2
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r1]
	str r7, [r5, #0x14]
	ldr r3, _080020A0 @ =0x00000404
	adds r0, r5, r3
	str r7, [r0]
	movs r0, #0
	strb r0, [r5, #0x18]
_0800204A:
	ldr r0, _080020A0 @ =0x00000404
	adds r1, r5, r0
	ldr r0, [r1]
	cmp r0, #0xf
	ble _08002060
	movs r0, #0xf
	rsbs r0, r0, #0
	str r0, [r5, #0xc]
	movs r0, #0xe1
	lsls r0, r0, #3
	str r0, [r5, #0x14]
_08002060:
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
	ldrb r0, [r5, #7]
	cmp r0, #0
	bne _080020D2
	movs r1, #0xff
	lsls r1, r1, #2
	adds r0, r5, r1
	ldr r0, [r0]
	cmp r0, #0
	bge _080020A4
	ldr r0, [r5, #0xc]
	subs r0, #1
	b _080020A8
	.align 2, 0
_08002080: .4byte 0x04000208
_08002084: .4byte 0x04000200
_08002088: .4byte 0x0000FF7F
_0800208C: .4byte 0x0000FFBF
_08002090: .4byte sub_8002830
_08002094: .4byte sub_8002848
_08002098: .4byte 0x0400010C
_0800209C: .4byte 0x00C0BBBC
_080020A0: .4byte 0x00000404
_080020A4:
	ldr r0, [r5, #0xc]
	adds r0, #1
_080020A8:
	str r0, [r5, #0xc]
	ldr r1, [r5, #0xc]
	cmp r1, #0xe
	ble _080020BC
	movs r1, #0
	movs r0, #1
	strb r0, [r5, #7]
	str r1, [r5, #0x14]
	str r1, [r5, #0x10]
	b _080020D2
_080020BC:
	movs r0, #0xf
	rsbs r0, r0, #0
	cmp r1, r0
	ble _080020C6
	b _08001FB6
_080020C6:
	adds r0, r5, #0
	bl sub_8001D30
	adds r0, r5, #0
	bl sub_8001DB4
_080020D2:
	ldr r0, [r5, #0x10]
	ldr r1, [r5, #0x14]
	cmp r0, r1
	bge _080020DC
	adds r0, r1, #0
_080020DC:
	str r0, [r5, #0x10]
	ldrb r0, [r5, #0x18]
	adds r1, #1
	cmp r0, #0
	beq _080020E8
	movs r1, #0
_080020E8:
	str r1, [r5, #0x14]
	movs r0, #0
	strb r0, [r5, #0x18]
	cmp r1, #0x1d
	ble _080020FE
	adds r0, r5, #0
	bl sub_8001D30
	adds r0, r5, #0
	bl sub_8001DB4
_080020FE:
	ldr r0, [r5, #0xc]
	adds r0, #1
	str r0, [r5, #0xc]
	movs r0, #1
_08002106:
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8002114
sub_8002114: @ 0x08002114
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x2c
	mov sl, r0
	str r1, [sp, #0x10]
	ldr r0, _08002144 @ =0x00000404
	add r0, sl
	movs r1, #0
	str r1, [r0]
	mov r1, sl
	ldrb r0, [r1, #4]
	cmp r0, #0
	beq _0800214C
	movs r0, #0x80
	lsls r0, r0, #3
	add r0, sl
	ldrh r1, [r0]
	ldr r0, _08002148 @ =0x0400012A
	strh r1, [r0]
	b _080026CE
	.align 2, 0
_08002144: .4byte 0x00000404
_08002148: .4byte 0x0400012A
_0800214C:
	movs r2, #1
	mov r3, sl
	strb r2, [r3, #4]
	ldr r0, _08002250 @ =0x04000128
	ldrh r0, [r0]
	lsls r3, r0, #0x10
	movs r4, #0
	str r4, [sp, #0x18]
	mov r1, sl
	adds r1, #0x31
	ldrb r5, [r1]
	lsls r0, r5, #0x1c
	lsrs r0, r0, #0x1c
	adds r0, #1
	str r0, [sp, #0x1c]
	movs r0, #0xf
	ldr r7, [sp, #0x1c]
	ands r7, r0
	str r7, [sp, #0x1c]
	lsrs r0, r3, #0x16
	ands r0, r2
	str r1, [sp, #0x28]
	cmp r0, #0
	beq _0800217E
	b _080026BC
_0800217E:
	mov r1, sl
	ldrb r0, [r1, #7]
	cmp r0, #0
	bne _0800225C
	lsrs r0, r3, #0x13
	ands r0, r2
	cmp r0, #0
	bne _08002190
	b _080026BC
_08002190:
	movs r5, #0
	movs r2, #0x20
	add r2, sl
	mov r8, r2
	ldr r3, _08002254 @ =0x00000F0B
	mov sb, r3
	ldr r6, _08002258 @ =0x0000FFFF
	ldr r1, [sp, #0x10]
	mov r2, sp
	adds r3, r1, #0
	movs r7, #3
	str r7, [sp, #0x14]
_080021A8:
	ldr r0, [r1]
	str r0, [r2]
	ldrh r7, [r2]
	lsrs r0, r7, #4
	cmp r0, sb
	bne _080021B6
	adds r4, #1
_080021B6:
	ldrh r0, [r3]
	cmp r0, r6
	bne _080021BE
	adds r5, #1
_080021BE:
	adds r1, #2
	adds r2, #4
	adds r3, #2
	ldr r0, [sp, #0x14]
	subs r0, #1
	str r0, [sp, #0x14]
	cmp r0, #0
	bge _080021A8
	movs r2, #1
	adds r3, r5, r4
	cmp r4, #0
	ble _080021F2
	mov r1, sp
	str r4, [sp, #0x14]
_080021DA:
	ldrb r5, [r1]
	lsls r0, r5, #0x1c
	lsrs r0, r0, #0x1c
	cmp r0, r4
	beq _080021E6
	movs r2, #0
_080021E6:
	adds r1, #4
	ldr r7, [sp, #0x14]
	subs r7, #1
	str r7, [sp, #0x14]
	cmp r7, #0
	bne _080021DA
_080021F2:
	cmp r3, #4
	bne _0800222E
	cmp r2, #0
	beq _0800222E
	cmp r4, #1
	ble _0800222E
	mov r0, sl
	str r4, [r0, #0x1c]
	ldr r0, _08002250 @ =0x04000128
	ldrh r1, [r0]
	movs r0, #0x30
	ands r0, r1
	lsrs r0, r0, #4
	movs r2, #0xff
	lsls r2, r2, #2
	add r2, sl
	str r0, [r2]
	movs r3, #0xfe
	lsls r3, r3, #2
	add r3, sl
	movs r1, #1
	rsbs r1, r1, #0
	lsls r1, r4
	mvns r1, r1
	str r1, [r3]
	ldr r2, [r2]
	movs r0, #1
	lsls r0, r2
	bics r1, r0
	str r1, [r3]
_0800222E:
	movs r0, #0xf
	ands r4, r0
	movs r0, #0x10
	rsbs r0, r0, #0
	mov r1, r8
	ldrb r1, [r1]
	ands r0, r1
	orrs r0, r4
	mov r2, r8
	strb r0, [r2]
	movs r1, #0x80
	lsls r1, r1, #3
	add r1, sl
	mov r3, sl
	ldrh r0, [r3, #0x20]
	strh r0, [r1]
	b _080026BC
	.align 2, 0
_08002250: .4byte 0x04000128
_08002254: .4byte 0x00000F0B
_08002258: .4byte 0x0000FFFF
_0800225C:
	movs r4, #0
	str r4, [sp, #0x14]
	mov r5, sl
	ldr r0, [r5, #0x1c]
	ldr r7, [sp, #0x18]
	cmp r7, r0
	blt _0800226C
	b _080024C2
_0800226C:
	movs r0, #0xfd
	lsls r0, r0, #2
	add r0, sl
	str r0, [sp, #0x20]
_08002274:
	movs r0, #0xff
	lsls r0, r0, #2
	add r0, sl
	ldr r0, [r0]
	ldr r1, [sp, #0x14]
	adds r1, #1
	str r1, [sp, #0x24]
	ldr r2, [sp, #0x14]
	cmp r2, r0
	bne _0800228A
	b _080024B2
_0800228A:
	movs r0, #0xc8
	muls r0, r2, r0
	adds r0, #0xd0
	add r0, sl
	mov ip, r0
	ldr r0, [r0, #0x2c]
	lsls r0, r0, #1
	mov r1, ip
	adds r1, #0xa
	adds r1, r1, r0
	lsls r0, r2, #1
	ldr r3, [sp, #0x10]
	adds r0, r0, r3
	ldrh r0, [r0]
	strh r0, [r1]
	mov r4, ip
	ldr r0, [r4, #0x2c]
	adds r0, #1
	movs r6, #0xf
	ands r0, r6
	str r0, [r4, #0x2c]
	cmp r0, #3
	bgt _080022BA
	b _080024B2
_080022BA:
	lsls r0, r0, #1
	adds r0, #2
	add r0, ip
	mov sb, r0
	movs r7, #0
	ldrb r5, [r0, #1]
	lsls r3, r5, #0x1c
	ldrb r2, [r0]
	lsls r4, r2, #0x18
	lsrs r1, r3, #0x1c
	lsrs r0, r4, #0x1c
	cmp r1, r0
	beq _080022DC
	subs r0, #1
	ands r0, r6
	cmp r1, r0
	bne _080022FE
_080022DC:
	lsrs r0, r5, #4
	cmp r0, #4
	bhi _080022FE
	lsls r2, r2, #0x1c
	lsrs r1, r4, #0x1c
	mov r5, sb
	ldrb r5, [r5, #7]
	lsls r0, r5, #8
	mov r3, sb
	ldrb r3, [r3, #6]
	orrs r0, r3
	adds r1, r1, r0
	ands r1, r6
	lsrs r2, r2, #0x1c
	cmp r2, r1
	bne _080022FE
	movs r7, #1
_080022FE:
	ldr r4, [sp, #0x14]
	adds r4, #1
	str r4, [sp, #0x24]
	cmp r7, #0
	bne _0800230A
	b _080024B2
_0800230A:
	mov r5, sb
	ldrb r3, [r5, #1]
	movs r0, #0xf
	mov r7, ip
	ldrb r2, [r7, #1]
	adds r1, r0, #0
	ands r1, r3
	ands r0, r2
	mov r8, r2
	cmp r1, r0
	bne _08002364
	ldrb r0, [r5, #7]
	lsls r3, r0, #8
	ldrb r1, [r5, #6]
	orrs r3, r1
	ldrh r0, [r7, #8]
	mov r2, sb
	adds r2, #1
	movs r4, #4
	ldr r5, _08002360 @ =gStaticData_0816AF10
	mov r8, r5
	movs r6, #0xff
	movs r5, #1
	rsbs r5, r5, #0
_0800233A:
	lsrs r1, r0, #8
	ldrb r7, [r2]
	eors r1, r7
	ands r1, r6
	lsls r1, r1, #1
	add r1, r8
	lsls r0, r0, #8
	ldrh r1, [r1]
	eors r0, r1
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r2, #1
	subs r4, #1
	cmp r4, r5
	bne _0800233A
	cmp r3, r0
	bne _0800235E
	b _08002466
_0800235E:
	b _080024B2
	.align 2, 0
_08002360: .4byte gStaticData_0816AF10
_08002364:
	lsls r0, r3, #0x1c
	lsrs r0, r0, #0x1c
	mov r2, ip
	ldr r1, [r2, #0x30]
	cmp r0, r1
	beq _08002372
	b _080024B2
_08002372:
	mov r3, sb
	ldrb r3, [r3, #7]
	lsls r4, r3, #8
	mov r5, sb
	ldrb r5, [r5, #6]
	orrs r4, r5
	ldrh r0, [r2, #6]
	mov r3, sb
	adds r3, #1
	movs r5, #4
	ldr r2, _08002400 @ =gStaticData_0816AF10
	movs r6, #1
	rsbs r6, r6, #0
_0800238C:
	lsrs r1, r0, #8
	ldrb r7, [r3]
	eors r1, r7
	movs r7, #0xff
	ands r1, r7
	lsls r1, r1, #1
	adds r1, r1, r2
	lsls r0, r0, #8
	ldrh r1, [r1]
	eors r0, r1
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, #1
	subs r5, #1
	cmp r5, r6
	bne _0800238C
	cmp r4, r0
	beq _080023B2
	b _080024B2
_080023B2:
	mov r0, r8
	lsrs r7, r0, #4
	mov r4, ip
	adds r4, #2
	mov r0, ip
	adds r0, #0xc4
	movs r1, #0x80
	subs r1, r1, r7
	ldr r0, [r0]
	cmp r0, r1
	bge _08002404
	subs r3, r7, #1
	movs r1, #1
	rsbs r1, r1, #0
	cmp r3, r1
	beq _08002442
	mov r2, ip
	adds r2, #0xc4
	mov r5, ip
	adds r5, #0xbc
	mov r6, ip
	adds r6, #0x3c
_080023DE:
	ldr r0, [r2]
	adds r0, #1
	str r0, [r2]
	ldr r0, [r5]
	adds r0, #1
	str r0, [r5]
	ldr r0, [r2]
	adds r0, r6, r0
	ldrb r1, [r4]
	strb r1, [r0]
	adds r4, #1
	subs r3, #1
	movs r0, #1
	rsbs r0, r0, #0
	cmp r3, r0
	bne _080023DE
	b _08002442
	.align 2, 0
_08002400: .4byte gStaticData_0816AF10
_08002404:
	subs r3, r7, #1
	movs r1, #1
	rsbs r1, r1, #0
	cmp r3, r1
	beq _08002442
	mov r2, ip
	adds r2, #0xc4
	mov r5, ip
	adds r5, #0xbc
	movs r0, #0x3c
	add r0, ip
	mov r8, r0
_0800241C:
	ldrb r6, [r4]
	adds r4, #1
	ldr r0, [r2]
	movs r1, #0
	cmp r0, #0x7f
	beq _0800242A
	adds r1, r0, #1
_0800242A:
	str r1, [r2]
	ldr r0, [r5]
	adds r0, #1
	str r0, [r5]
	ldr r0, [r2]
	add r0, r8
	strb r6, [r0]
	subs r3, #1
	movs r1, #1
	rsbs r1, r1, #0
	cmp r3, r1
	bne _0800241C
_08002442:
	mov r2, ip
	ldr r0, [r2, #0x34]
	adds r0, r0, r7
	str r0, [r2, #0x34]
	ldrh r0, [r2, #6]
	strh r0, [r2, #8]
	ldr r0, [r2, #0x30]
	adds r0, #1
	movs r1, #0xf
	ands r0, r1
	str r0, [r2, #0x30]
	movs r1, #1
	ldr r3, [sp, #0x14]
	lsls r1, r3
	ldr r4, [sp, #0x20]
	ldr r0, [r4]
	orrs r0, r1
	str r0, [r4]
_08002466:
	movs r5, #0xff
	mov r3, ip
	mov r2, sb
	movs r4, #3
_0800246E:
	ldrb r7, [r2, #1]
	lsls r1, r7, #8
	ldrb r0, [r2]
	orrs r1, r0
	adds r0, r1, #0
	ands r0, r5
	strb r0, [r3]
	lsrs r1, r1, #8
	strb r1, [r3, #1]
	adds r3, #2
	adds r2, #2
	subs r4, #1
	cmp r4, #0
	bge _0800246E
	mov r1, ip
	ldrb r1, [r1]
	lsrs r0, r1, #4
	ldr r2, [sp, #0x1c]
	cmp r0, r2
	bne _080024A8
	movs r2, #0xfc
	lsls r2, r2, #2
	add r2, sl
	movs r1, #1
	ldr r3, [sp, #0x14]
	lsls r1, r3
	ldr r0, [r2]
	orrs r0, r1
	str r0, [r2]
_080024A8:
	movs r0, #0
	mov r4, ip
	str r0, [r4, #0x2c]
	movs r5, #1
	str r5, [sp, #0x18]
_080024B2:
	ldr r7, [sp, #0x24]
	str r7, [sp, #0x14]
	mov r1, sl
	ldr r0, [r1, #0x1c]
	adds r2, r7, #0
	cmp r2, r0
	bge _080024C2
	b _08002274
_080024C2:
	ldr r3, [sp, #0x18]
	cmp r3, #0
	bne _080024CA
	b _08002668
_080024CA:
	movs r4, #0
	str r4, [sp, #0x18]
	movs r2, #0xfc
	lsls r2, r2, #2
	add r2, sl
	movs r0, #0xfe
	lsls r0, r0, #2
	add r0, sl
	ldr r1, [r2]
	ldr r0, [r0]
	cmp r1, r0
	beq _080024E4
	b _08002606
_080024E4:
	str r4, [r2]
	mov r5, sl
	adds r5, #0x30
	mov r7, sl
	adds r7, #0x40
	movs r0, #0xc4
	add r0, sl
	mov r8, r0
	movs r1, #0x32
	add r1, sl
	mov sb, r1
	movs r2, #0xc8
	add r2, sl
	mov ip, r2
	mov r3, sl
	adds r3, #0x28
	adds r2, r5, #0
	movs r6, #0xff
	movs r4, #3
_0800250A:
	ldrb r0, [r2, #1]
	lsls r1, r0, #8
	ldrb r0, [r2]
	orrs r1, r0
	adds r0, r1, #0
	ands r0, r6
	strb r0, [r3]
	lsrs r1, r1, #8
	strb r1, [r3, #1]
	adds r3, #2
	adds r2, #2
	subs r4, #1
	cmp r4, #0
	bge _0800250A
	adds r4, r7, #0
	mov r1, r8
	ldr r6, [r1]
	cmp r6, #4
	ble _08002532
	movs r6, #4
_08002532:
	lsls r1, r6, #4
	movs r0, #0xf
	ldrb r2, [r5, #1]
	ands r0, r2
	orrs r0, r1
	strb r0, [r5, #1]
	mov r3, sb
	movs r0, #0x80
	subs r0, r0, r6
	mov r5, ip
	ldr r1, [r5]
	cmp r1, r0
	bge _0800257E
	subs r2, r6, #1
	movs r0, #1
	rsbs r0, r0, #0
	cmp r2, r0
	beq _080025B4
	adds r1, r4, #0
	adds r1, #0x88
	adds r5, r4, #4
	adds r4, #0x84
	adds r7, r0, #0
_08002560:
	ldr r0, [r1]
	adds r0, r5, r0
	ldrb r0, [r0]
	strb r0, [r3]
	adds r3, #1
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
	ldr r0, [r4]
	subs r0, #1
	str r0, [r4]
	subs r2, #1
	cmp r2, r7
	bne _08002560
	b _080025B4
_0800257E:
	subs r2, r6, #1
	movs r0, #1
	rsbs r0, r0, #0
	cmp r2, r0
	beq _080025B4
	adds r5, r7, #0
	adds r5, #0x88
	adds r4, r7, #0
	adds r4, #0x84
	adds r7, #4
	mov r8, r0
_08002594:
	ldr r1, [r5]
	movs r0, #0
	cmp r1, #0x7f
	beq _0800259E
	adds r0, r1, #1
_0800259E:
	str r0, [r5]
	ldr r0, [r4]
	subs r0, #1
	str r0, [r4]
	adds r0, r7, r1
	ldrb r0, [r0]
	strb r0, [r3]
	adds r3, #1
	subs r2, #1
	cmp r2, r8
	bne _08002594
_080025B4:
	mov r7, sl
	ldr r0, [r7, #0x24]
	adds r0, r0, r6
	str r0, [r7, #0x24]
	movs r0, #0x10
	rsbs r0, r0, #0
	ldr r1, [sp, #0x28]
	ldrb r1, [r1]
	ands r0, r1
	ldr r2, [sp, #0x1c]
	orrs r0, r2
	ldr r3, [sp, #0x28]
	strb r0, [r3]
	ldrh r1, [r7, #0x36]
	ldr r2, [sp, #0x28]
	movs r3, #4
	ldr r6, _08002684 @ =gStaticData_0816AF10
	movs r5, #0xff
	movs r4, #1
	rsbs r4, r4, #0
_080025DC:
	lsrs r0, r1, #8
	ldrb r7, [r2]
	eors r0, r7
	ands r0, r5
	lsls r0, r0, #1
	adds r0, r0, r6
	lsls r1, r1, #8
	ldrh r0, [r0]
	eors r1, r0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	adds r2, #1
	subs r3, #1
	cmp r3, r4
	bne _080025DC
	movs r0, #0
	mov r2, sl
	strh r1, [r2, #0x36]
	str r0, [r2, #0x3c]
	movs r3, #1
	str r3, [sp, #0x18]
_08002606:
	movs r2, #0xfd
	lsls r2, r2, #2
	add r2, sl
	movs r0, #0xfe
	lsls r0, r0, #2
	add r0, sl
	ldr r1, [r2]
	ldr r0, [r0]
	cmp r1, r0
	bne _0800263A
	movs r0, #0
	str r0, [r2]
	mov r3, sl
	adds r3, #0x30
	ldrb r2, [r3]
	lsrs r1, r2, #4
	adds r1, #1
	movs r0, #0xf
	ands r1, r0
	lsls r1, r1, #4
	movs r0, #0xf
	ands r0, r2
	orrs r0, r1
	strb r0, [r3]
	movs r4, #1
	str r4, [sp, #0x18]
_0800263A:
	ldr r5, [sp, #0x18]
	cmp r5, #0
	beq _08002668
	movs r0, #0
	mov r7, sl
	str r0, [r7, #0x38]
	movs r0, #1
	strb r0, [r7, #0x18]
	mov r2, sl
	adds r2, #0x30
	ldrb r3, [r2]
	lsrs r1, r3, #4
	ldrb r4, [r2, #7]
	lsls r0, r4, #8
	ldrb r5, [r2, #6]
	orrs r0, r5
	adds r1, r1, r0
	movs r0, #0xf
	ands r1, r0
	subs r0, #0x1f
	ands r0, r3
	orrs r0, r1
	strb r0, [r2]
_08002668:
	mov r7, sl
	ldr r0, [r7, #0x3c]
	movs r1, #3
	ands r0, r1
	cmp r0, #3
	bne _08002688
	ldr r0, [r7, #0x38]
	lsls r0, r0, #1
	add r0, sl
	adds r1, r0, #0
	adds r1, #0x28
	adds r0, #0x29
	b _08002696
	.align 2, 0
_08002684: .4byte gStaticData_0816AF10
_08002688:
	mov r1, sl
	ldr r0, [r1, #0x38]
	lsls r0, r0, #1
	add r0, sl
	adds r1, r0, #0
	adds r1, #0x30
	adds r0, #0x31
_08002696:
	ldrb r0, [r0]
	lsls r0, r0, #8
	ldrb r1, [r1]
	orrs r0, r1
	movs r1, #0x80
	lsls r1, r1, #3
	add r1, sl
	strh r0, [r1]
	mov r2, sl
	ldr r0, [r2, #0x38]
	adds r0, #1
	str r0, [r2, #0x38]
	cmp r0, #4
	bne _080026BC
	movs r0, #0
	str r0, [r2, #0x38]
	ldr r0, [r2, #0x3c]
	adds r0, #1
	str r0, [r2, #0x3c]
_080026BC:
	movs r0, #0x80
	lsls r0, r0, #3
	add r0, sl
	ldrh r1, [r0]
	ldr r0, _080026E0 @ =0x0400012A
	strh r1, [r0]
	movs r0, #0
	mov r3, sl
	strb r0, [r3, #4]
_080026CE:
	add sp, #0x2c
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080026E0: .4byte 0x0400012A

	thumb_func_start sub_80026E4
sub_80026E4: @ 0x080026E4
	push {r4, r5, r6, lr}
	lsls r4, r1, #0x18
	lsrs r4, r4, #0x18
	ldr r6, _0800274C @ =0x04000208
	movs r3, #0
	strh r3, [r6]
	ldrh r1, [r6]
	strh r3, [r6]
	ldr r5, _08002750 @ =0x04000200
	ldrh r2, [r5]
	ldr r0, _08002754 @ =0x0000FF7F
	ands r0, r2
	strh r0, [r5]
	strh r1, [r6]
	ldrh r1, [r6]
	strh r3, [r6]
	ldrh r2, [r5]
	ldr r0, _08002758 @ =0x0000FFBF
	ands r0, r2
	strh r0, [r5]
	strh r1, [r6]
	movs r0, #6
	bl sub_8000544
	ldr r1, _0800275C @ =sub_8002830
	movs r0, #7
	bl sub_80005A0
	ldrh r0, [r5]
	movs r1, #0x80
	orrs r0, r1
	strh r0, [r5]
	cmp r4, #0
	beq _0800273E
	ldr r1, _08002760 @ =sub_8002848
	movs r0, #6
	bl sub_80005A0
	ldrh r0, [r5]
	movs r1, #0x40
	orrs r0, r1
	strh r0, [r5]
	ldr r1, _08002764 @ =0x0400010C
	ldr r0, _08002768 @ =0x00C0BBBC
	str r0, [r1]
_0800273E:
	movs r0, #1
	strh r0, [r6]
	movs r0, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0800274C: .4byte 0x04000208
_08002750: .4byte 0x04000200
_08002754: .4byte 0x0000FF7F
_08002758: .4byte 0x0000FFBF
_0800275C: .4byte sub_8002830
_08002760: .4byte sub_8002848
_08002764: .4byte 0x0400010C
_08002768: .4byte 0x00C0BBBC

	thumb_func_start sub_800276C
sub_800276C: @ 0x0800276C
	ldr r1, _0800278C @ =0x04000134
	movs r0, #0
	strh r0, [r1]
	ldr r2, _08002790 @ =0x04000128
	movs r1, #0x80
	lsls r1, r1, #6
	adds r0, r1, #0
	strh r0, [r2]
	ldrh r0, [r2]
	ldr r3, _08002794 @ =0x00004003
	adds r1, r3, #0
	orrs r0, r1
	strh r0, [r2]
	movs r0, #0
	bx lr
	.align 2, 0
_0800278C: .4byte 0x04000134
_08002790: .4byte 0x04000128
_08002794: .4byte 0x00004003

	thumb_func_start sub_8002798
sub_8002798: @ 0x08002798
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8001D30
	adds r0, r4, #0
	bl sub_8001DB4
	movs r0, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_80027B0
sub_80027B0: @ 0x080027B0
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	bl sub_8002798
	adds r1, r4, #0
	adds r1, #0xd0
	cmp r1, #0
	beq _080027D2
	movs r2, #0xfc
	lsls r2, r2, #2
	adds r0, r4, r2
	cmp r1, r0
	beq _080027D2
_080027CC:
	subs r0, #0xc8
	cmp r1, r0
	bne _080027CC
_080027D2:
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _080027E0
	adds r0, r4, #0
	bl sub_80016D0
_080027E0:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80027E8
sub_80027E8: @ 0x080027E8
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r0, #0xc4
	movs r1, #0
	str r1, [r0]
	adds r0, #4
	str r1, [r0]
	adds r1, r4, #0
	adds r1, #0xcc
	movs r0, #0x7f
	str r0, [r1]
	movs r1, #3
	movs r2, #0
	movs r5, #0x7f
	movs r3, #1
	rsbs r3, r3, #0
	movs r6, #0xc6
	lsls r6, r6, #1
	adds r0, r4, r6
_0800280E:
	str r2, [r0]
	str r2, [r0, #4]
	str r5, [r0, #8]
	adds r0, #0xc8
	subs r1, #1
	cmp r1, r3
	bne _0800280E
	adds r0, r4, #0
	bl sub_8002798
	movs r0, #0
	strb r0, [r4, #5]
	adds r0, r4, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8002830
sub_8002830: @ 0x08002830
	push {lr}
	ldr r0, _08002840 @ =gUnknown_03000804
	ldr r0, [r0]
	ldr r1, _08002844 @ =0x04000120
	bl sub_8002114
	pop {r0}
	bx r0
	.align 2, 0
_08002840: .4byte gUnknown_03000804
_08002844: .4byte 0x04000120

	thumb_func_start sub_8002848
sub_8002848: @ 0x08002848
	ldr r3, _08002860 @ =0x0400010E
	movs r0, #0
	strh r0, [r3]
	ldr r2, _08002864 @ =0x04000128
	ldrh r0, [r2]
	movs r1, #0x80
	orrs r0, r1
	strh r0, [r2]
	movs r0, #0xc0
	strh r0, [r3]
	bx lr
	.align 2, 0
_08002860: .4byte 0x0400010E
_08002864: .4byte 0x04000128

	thumb_func_start sub_8002868
sub_8002868: @ 0x08002868
	push {r4, r5, r6, r7, lr}
	ldr r4, _080028A0 @ =0xFFFFFE00
	add sp, r4
	adds r6, r0, #0
	adds r7, r1, #0
	ldr r4, _080028A4 @ =gUnknown_03000808
	ldrb r0, [r4]
	cmp r0, #0
	beq _0800288A
	movs r0, #4
	bl sub_803A968
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	cmp r0, #0
	bne _0800291C
	strb r0, [r4]
_0800288A:
	ldr r1, _080028A8 @ =0x04000208
	movs r0, #0
	strh r0, [r1]
	ldr r1, _080028AC @ =gUnknown_030009FC
	movs r0, #2
	bl sub_803A9D0
	mov r5, sp
	movs r4, #0
	b _080028C4
	.align 2, 0
_080028A0: .4byte 0xFFFFFE00
_080028A4: .4byte gUnknown_03000808
_080028A8: .4byte 0x04000208
_080028AC: .4byte gUnknown_030009FC
_080028B0:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	adds r1, r5, #0
	bl sub_803AB54
	lsls r0, r0, #0x10
	cmp r0, #0
	bne _08002904
	adds r5, #8
	adds r4, #1
_080028C4:
	ldr r0, _080028F4 @ =gUnknown_03001634
	ldr r0, [r0]
	ldrh r0, [r0, #4]
	cmp r4, r0
	blt _080028B0
	ldr r3, _080028F8 @ =0x04000208
	ldrh r2, [r3]
	movs r0, #0
	strh r0, [r3]
	ldr r4, _080028FC @ =0x04000200
	ldrh r1, [r4]
	ldr r0, _08002900 @ =0x0000FFDF
	ands r0, r1
	strh r0, [r4]
	strh r2, [r3]
	movs r0, #1
	strh r0, [r3]
	adds r0, r6, #0
	mov r1, sp
	adds r2, r7, #0
	bl sub_800014C
	movs r0, #0
	b _08002920
	.align 2, 0
_080028F4: .4byte gUnknown_03001634
_080028F8: .4byte 0x04000208
_080028FC: .4byte 0x04000200
_08002900: .4byte 0x0000FFDF
_08002904:
	ldr r3, _0800292C @ =0x04000208
	ldrh r2, [r3]
	movs r0, #0
	strh r0, [r3]
	ldr r4, _08002930 @ =0x04000200
	ldrh r1, [r4]
	ldr r0, _08002934 @ =0x0000FFDF
	ands r0, r1
	strh r0, [r4]
	strh r2, [r3]
	movs r0, #1
	strh r0, [r3]
_0800291C:
	movs r0, #1
	rsbs r0, r0, #0
_08002920:
	movs r3, #0x80
	lsls r3, r3, #2
	add sp, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0800292C: .4byte 0x04000208
_08002930: .4byte 0x04000200
_08002934: .4byte 0x0000FFDF

	thumb_func_start sub_8002938
sub_8002938: @ 0x08002938
	push {r4, r5, r6, lr}
	ldr r4, _08002978 @ =0xFFFFFE00
	add sp, r4
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r6, _0800297C @ =gUnknown_03000808
	ldrb r0, [r6]
	cmp r0, #0
	beq _0800295A
	movs r0, #4
	bl sub_803A968
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	cmp r0, #0
	bne _080029EC
	strb r0, [r6]
_0800295A:
	mov r0, sp
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_800014C
	ldr r1, _08002980 @ =0x04000208
	movs r0, #0
	strh r0, [r1]
	ldr r1, _08002984 @ =gUnknown_030009FC
	movs r0, #2
	bl sub_803A9D0
	mov r5, sp
	movs r4, #0
	b _0800299C
	.align 2, 0
_08002978: .4byte 0xFFFFFE00
_0800297C: .4byte gUnknown_03000808
_08002980: .4byte 0x04000208
_08002984: .4byte gUnknown_030009FC
_08002988:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	adds r1, r5, #0
	bl sub_803AD38
	lsls r0, r0, #0x10
	cmp r0, #0
	bne _080029D4
	adds r5, #8
	adds r4, #1
_0800299C:
	ldr r0, _080029C4 @ =gUnknown_03001634
	ldr r0, [r0]
	ldrh r0, [r0, #4]
	cmp r4, r0
	blt _08002988
	ldr r3, _080029C8 @ =0x04000208
	ldrh r2, [r3]
	movs r0, #0
	strh r0, [r3]
	ldr r4, _080029CC @ =0x04000200
	ldrh r1, [r4]
	ldr r0, _080029D0 @ =0x0000FFDF
	ands r0, r1
	strh r0, [r4]
	strh r2, [r3]
	movs r0, #1
	strh r0, [r3]
	movs r0, #0
	b _080029F0
	.align 2, 0
_080029C4: .4byte gUnknown_03001634
_080029C8: .4byte 0x04000208
_080029CC: .4byte 0x04000200
_080029D0: .4byte 0x0000FFDF
_080029D4:
	ldr r3, _080029FC @ =0x04000208
	ldrh r2, [r3]
	movs r0, #0
	strh r0, [r3]
	ldr r4, _08002A00 @ =0x04000200
	ldrh r1, [r4]
	ldr r0, _08002A04 @ =0x0000FFDF
	ands r0, r1
	strh r0, [r4]
	strh r2, [r3]
	movs r0, #1
	strh r0, [r3]
_080029EC:
	movs r0, #1
	rsbs r0, r0, #0
_080029F0:
	movs r3, #0x80
	lsls r3, r3, #2
	add sp, r3
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_080029FC: .4byte 0x04000208
_08002A00: .4byte 0x04000200
_08002A04: .4byte 0x0000FFDF

	thumb_func_start sub_8002A08
sub_8002A08: @ 0x08002A08
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	ldr r4, _08002A60 @ =gUnknown_030012BC
	ldr r1, [r4]
	movs r2, #0
	ldr r0, [r1, #4]
	cmp r0, #1
	bne _08002A1E
	movs r2, #1
_08002A1E:
	adds r7, r2, #0
	adds r0, r1, #0
	bl sub_8001AB8
	mov r8, r0
	cmp r7, #0
	beq _08002A32
	ldr r0, [r4]
	bl sub_8001BD4
_08002A32:
	movs r5, #0
_08002A34:
	adds r0, r6, #0
	movs r1, #0x80
	lsls r1, r1, #2
	bl sub_8002868
	adds r4, r0, #0
	adds r5, #1
	cmp r5, #2
	bgt _08002A4A
	cmp r4, #0
	bne _08002A34
_08002A4A:
	cmp r7, #0
	beq _08002A58
	ldr r0, _08002A60 @ =gUnknown_030012BC
	ldr r0, [r0]
	mov r1, r8
	bl sub_8001B54
_08002A58:
	cmp r4, #0
	beq _08002A64
	movs r0, #4
	b _08002A9A
	.align 2, 0
_08002A60: .4byte gUnknown_030012BC
_08002A64:
	movs r1, #0xfc
	lsls r1, r1, #1
	adds r0, r6, r1
	ldrb r0, [r0]
	cmp r0, #0x43
	beq _08002A74
	movs r0, #2
	b _08002A9A
_08002A74:
	ldr r1, _08002A84 @ =0x000001F9
	adds r0, r6, r1
	ldrb r0, [r0]
	cmp r0, #0x12
	beq _08002A88
	movs r0, #1
	b _08002A9A
	.align 2, 0
_08002A84: .4byte 0x000001F9
_08002A88:
	adds r0, r6, #0
	bl sub_8002B44
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08002A98
	movs r0, #0
	b _08002A9A
_08002A98:
	movs r0, #3
_08002A9A:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_8002AA4
sub_8002AA4: @ 0x08002AA4
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	adds r5, r0, #0
	adds r3, r5, #0
	movs r2, #0
	movs r1, #0x7e
_08002AB6:
	ldm r3!, {r0}
	adds r2, r2, r0
	subs r1, #1
	cmp r1, #0
	bge _08002AB6
	movs r1, #0
	movs r3, #0xfe
	lsls r3, r3, #1
	adds r0, r5, r3
	ldr r0, [r0]
	cmp r2, r0
	bne _08002AD0
	movs r1, #1
_08002AD0:
	cmp r1, #0
	bne _08002B26
	mov r0, sp
	strh r1, [r0]
	ldr r0, _08002B34 @ =0x040000D4
	mov r1, sp
	str r1, [r0]
	str r5, [r0, #4]
	ldr r1, _08002B38 @ =0x81000100
	str r1, [r0, #8]
	ldr r0, [r0, #8]
	movs r4, #0
	movs r2, #0xfc
	lsls r2, r2, #1
	adds r6, r5, r2
	ldr r3, _08002B3C @ =0x000001F9
	adds r3, r3, r5
	mov sb, r3
	movs r0, #0xfd
	lsls r0, r0, #1
	adds r7, r5, r0
	ldr r1, _08002B40 @ =0x000001FB
	adds r1, r1, r5
	mov r8, r1
_08002B00:
	adds r0, r5, #0
	adds r1, r4, #0
	bl sub_8002C6C
	adds r4, #1
	cmp r4, #3
	ble _08002B00
	movs r0, #0
	movs r1, #0x43
	strb r1, [r6]
	movs r1, #0x12
	mov r2, sb
	strb r1, [r2]
	strb r0, [r7]
	mov r3, r8
	strb r0, [r3]
	adds r0, r5, #0
	bl sub_8002B70
_08002B26:
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08002B34: .4byte 0x040000D4
_08002B38: .4byte 0x81000100
_08002B3C: .4byte 0x000001F9
_08002B40: .4byte 0x000001FB

	thumb_func_start sub_8002B44
sub_8002B44: @ 0x08002B44
	push {r4, lr}
	adds r4, r0, #0
	adds r3, r4, #0
	movs r2, #0
	movs r1, #0x7e
_08002B4E:
	ldm r3!, {r0}
	adds r2, r2, r0
	subs r1, #1
	cmp r1, #0
	bge _08002B4E
	movs r1, #0
	movs r3, #0xfe
	lsls r3, r3, #1
	adds r0, r4, r3
	ldr r0, [r0]
	cmp r2, r0
	bne _08002B68
	movs r1, #1
_08002B68:
	adds r0, r1, #0
	pop {r4}
	pop {r1}
	bx r1

	thumb_func_start sub_8002B70
sub_8002B70: @ 0x08002B70
	push {r4, lr}
	adds r4, r0, #0
	adds r3, r4, #0
	movs r2, #0
	movs r1, #0x7e
_08002B7A:
	ldm r3!, {r0}
	adds r2, r2, r0
	subs r1, #1
	cmp r1, #0
	bge _08002B7A
	movs r1, #0xfe
	lsls r1, r1, #1
	adds r0, r4, r1
	str r2, [r0]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8002B94
sub_8002B94: @ 0x08002B94
	ldr r1, _08002BA0 @ =0x000001F9
	adds r0, r0, r1
	ldrb r0, [r0]
	lsrs r0, r0, #4
	bx lr
	.align 2, 0
_08002BA0: .4byte 0x000001F9

	thumb_func_start sub_8002BA4
sub_8002BA4: @ 0x08002BA4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	ldr r4, _08002C04 @ =gUnknown_030012BC
	ldr r1, [r4]
	movs r2, #0
	ldr r0, [r1, #4]
	cmp r0, #1
	bne _08002BBA
	movs r2, #1
_08002BBA:
	adds r7, r2, #0
	adds r0, r1, #0
	bl sub_8001AB8
	mov r8, r0
	adds r0, r6, #0
	bl sub_8002B70
	cmp r7, #0
	beq _08002BD4
	ldr r0, [r4]
	bl sub_8001BD4
_08002BD4:
	movs r5, #0
_08002BD6:
	adds r0, r6, #0
	movs r1, #0x80
	lsls r1, r1, #2
	bl sub_8002938
	adds r4, r0, #0
	adds r5, #1
	cmp r5, #4
	bgt _08002BEC
	cmp r4, #0
	bne _08002BD6
_08002BEC:
	cmp r7, #0
	beq _08002BFA
	ldr r0, _08002C04 @ =gUnknown_030012BC
	ldr r0, [r0]
	mov r1, r8
	bl sub_8001B54
_08002BFA:
	cmp r4, #0
	bne _08002C08
	movs r0, #0
	b _08002C0A
	.align 2, 0
_08002C04: .4byte gUnknown_030012BC
_08002C08:
	movs r0, #4
_08002C0A:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_8002C14
sub_8002C14: @ 0x08002C14
	push {r4, lr}
	adds r4, r0, #0
	adds r3, r1, #0
	movs r1, #0xfa
	lsls r1, r1, #1
	adds r0, r4, r1
	adds r0, r0, r3
	ldrb r0, [r0]
	cmp r0, #0
	bne _08002C38
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #4
	adds r1, r1, r4
	adds r0, r2, #0
	movs r2, #0x70
	bl sub_800014C
_08002C38:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8002C40
sub_8002C40: @ 0x08002C40
	push {r4, lr}
	adds r4, r0, #0
	movs r3, #0xfa
	lsls r3, r3, #1
	adds r0, r4, r3
	adds r0, r0, r1
	movs r3, #0
	strb r3, [r0]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r0, r0, #4
	adds r0, r0, r4
	adds r1, r2, #0
	movs r2, #0x70
	bl sub_800014C
	adds r0, r4, #0
	bl sub_8002B70
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8002C6C
sub_8002C6C: @ 0x08002C6C
	push {lr}
	movs r3, #0xfa
	lsls r3, r3, #1
	adds r2, r0, r3
	adds r2, r2, r1
	movs r1, #1
	strb r1, [r2]
	bl sub_8002B70
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8002C84
sub_8002C84: @ 0x08002C84
	push {r4, r5, lr}
	sub sp, #4
	adds r5, r0, #0
	mov r1, sp
	movs r0, #0
	strh r0, [r1]
	ldr r0, _08002CDC @ =0x040000D4
	str r1, [r0]
	str r5, [r0, #4]
	ldr r1, _08002CE0 @ =0x81000100
	str r1, [r0, #8]
	ldr r0, [r0, #8]
	movs r4, #0
_08002C9E:
	adds r0, r5, #0
	adds r1, r4, #0
	bl sub_8002C6C
	adds r4, #1
	cmp r4, #3
	ble _08002C9E
	movs r0, #0xfc
	lsls r0, r0, #1
	adds r1, r5, r0
	movs r2, #0
	movs r0, #0x43
	strb r0, [r1]
	ldr r0, _08002CE4 @ =0x000001F9
	adds r1, r5, r0
	movs r0, #0x12
	strb r0, [r1]
	movs r1, #0xfd
	lsls r1, r1, #1
	adds r0, r5, r1
	strb r2, [r0]
	adds r1, #1
	adds r0, r5, r1
	strb r2, [r0]
	adds r0, r5, #0
	bl sub_8002B70
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08002CDC: .4byte 0x040000D4
_08002CE0: .4byte 0x81000100
_08002CE4: .4byte 0x000001F9

	thumb_func_start sub_8002CE8
sub_8002CE8: @ 0x08002CE8
	movs r2, #0xfa
	lsls r2, r2, #1
	adds r0, r0, r2
	adds r0, r0, r1
	ldrb r0, [r0]
	bx lr

	thumb_func_start sub_8002CF4
sub_8002CF4: @ 0x08002CF4
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	movs r2, #0xfd
	lsls r2, r2, #1
	adds r0, r0, r2
	ldrb r0, [r0]
	ands r1, r0
	adds r0, r1, #0
	cmp r1, #0
	beq _08002D0A
	movs r0, #1
_08002D0A:
	bx lr

	thumb_func_start sub_8002D0C
sub_8002D0C: @ 0x08002D0C
	push {lr}
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	movs r3, #0xfd
	lsls r3, r3, #1
	adds r2, r0, r3
	ldrb r3, [r2]
	bics r3, r1
	adds r1, r3, #0
	strb r1, [r2]
	bl sub_8002B70
	pop {r0}
	bx r0

	thumb_func_start sub_8002D28
sub_8002D28: @ 0x08002D28
	push {lr}
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	movs r3, #0xfd
	lsls r3, r3, #1
	adds r2, r0, r3
	ldrb r3, [r2]
	orrs r1, r3
	strb r1, [r2]
	bl sub_8002B70
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8002D44
sub_8002D44: @ 0x08002D44
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r0
	ldr r1, [r0]
	cmp r1, #0
	beq _08002DFA
	ldr r0, _08002DB0 @ =gUnknown_03000804
	ldr r0, [r0]
	mov ip, r0
	adds r0, #0xc4
	ldr r0, [r0]
	cmp r0, #0
	bne _08002E10
	adds r7, r1, #0
	cmp r7, #0x60
	ble _08002D6A
	movs r7, #0x60
_08002D6A:
	mov r0, r8
	ldr r4, [r0, #0xc]
	mov r3, ip
	adds r3, #0xcc
	movs r0, #0x80
	subs r0, r0, r7
	ldr r1, [r3]
	cmp r1, r0
	bge _08002DB4
	subs r2, r7, #1
	movs r0, #1
	rsbs r0, r0, #0
	cmp r2, r0
	beq _08002DEC
	mov r5, ip
	adds r5, #0xc4
	mov r6, ip
	adds r6, #0x44
	mov ip, r0
_08002D90:
	ldr r0, [r3]
	adds r0, #1
	str r0, [r3]
	ldr r0, [r5]
	adds r0, #1
	str r0, [r5]
	ldr r0, [r3]
	adds r0, r6, r0
	ldrb r1, [r4]
	strb r1, [r0]
	adds r4, #1
	subs r2, #1
	cmp r2, ip
	bne _08002D90
	b _08002DEC
	.align 2, 0
_08002DB0: .4byte gUnknown_03000804
_08002DB4:
	subs r2, r7, #1
	movs r0, #1
	rsbs r0, r0, #0
	cmp r2, r0
	beq _08002DEC
	adds r1, r3, #0
	mov r6, ip
	adds r6, #0xc4
	movs r3, #0x44
	add ip, r3
	mov sb, r0
_08002DCA:
	ldrb r5, [r4]
	adds r4, #1
	ldr r0, [r1]
	movs r3, #0
	cmp r0, #0x7f
	beq _08002DD8
	adds r3, r0, #1
_08002DD8:
	str r3, [r1]
	ldr r0, [r6]
	adds r0, #1
	str r0, [r6]
	ldr r0, [r1]
	add r0, ip
	strb r5, [r0]
	subs r2, #1
	cmp r2, sb
	bne _08002DCA
_08002DEC:
	mov r1, r8
	ldr r0, [r1, #0xc]
	adds r0, r0, r7
	str r0, [r1, #0xc]
	ldr r0, [r1]
	subs r0, r0, r7
	b _08002E0E
_08002DFA:
	ldr r0, _08002E1C @ =gUnknown_03000804
	ldr r0, [r0]
	adds r0, #0xc4
	ldr r0, [r0]
	cmp r0, #0
	bne _08002E10
	movs r1, #0x85
	lsls r1, r1, #2
	add r1, r8
	movs r0, #1
_08002E0E:
	str r0, [r1]
_08002E10:
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08002E1C: .4byte gUnknown_03000804

	thumb_func_start sub_8002E20
sub_8002E20: @ 0x08002E20
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov ip, r0
	ldr r0, _08002E90 @ =gUnknown_03000804
	ldr r3, [r0]
	movs r2, #0xc8
	adds r0, r1, #0
	muls r0, r2, r0
	adds r0, r0, r3
	movs r4, #0xc6
	lsls r4, r4, #1
	adds r0, r0, r4
	ldr r6, [r0]
	cmp r6, #0
	beq _08002EDC
	movs r0, #0x84
	lsls r0, r0, #2
	add r0, ip
	muls r2, r1, r2
	adds r2, r2, r3
	movs r1, #0x84
	lsls r1, r1, #1
	adds r2, r2, r1
	ldr r4, [r0]
	adds r5, r2, #0
	adds r5, #0x88
	movs r0, #0x80
	subs r0, r0, r6
	ldr r1, [r5]
	cmp r1, r0
	bge _08002E94
	subs r3, r6, #1
	movs r0, #1
	rsbs r0, r0, #0
	cmp r3, r0
	beq _08002EC6
	adds r1, r5, #0
	adds r5, r2, #4
	adds r2, #0x84
	adds r7, r0, #0
_08002E72:
	ldr r0, [r1]
	adds r0, r5, r0
	ldrb r0, [r0]
	strb r0, [r4]
	adds r4, #1
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
	ldr r0, [r2]
	subs r0, #1
	str r0, [r2]
	subs r3, #1
	cmp r3, r7
	bne _08002E72
	b _08002EC6
	.align 2, 0
_08002E90: .4byte gUnknown_03000804
_08002E94:
	subs r3, r6, #1
	movs r0, #1
	rsbs r0, r0, #0
	cmp r3, r0
	beq _08002EC6
	adds r1, r2, #0
	adds r1, #0x84
	adds r7, r2, #4
	mov r8, r0
_08002EA6:
	ldr r2, [r5]
	movs r0, #0
	cmp r2, #0x7f
	beq _08002EB0
	adds r0, r2, #1
_08002EB0:
	str r0, [r5]
	ldr r0, [r1]
	subs r0, #1
	str r0, [r1]
	adds r0, r7, r2
	ldrb r0, [r0]
	strb r0, [r4]
	adds r4, #1
	subs r3, #1
	cmp r3, r8
	bne _08002EA6
_08002EC6:
	movs r0, #0x84
	lsls r0, r0, #2
	add r0, ip
	ldr r1, [r0]
	adds r1, r1, r6
	str r1, [r0]
	mov r4, ip
	ldr r0, [r4, #4]
	adds r0, r0, r6
	str r0, [r4, #4]
	b _08002EF2
_08002EDC:
	mov r0, ip
	ldr r1, [r0, #4]
	movs r0, #0x80
	lsls r0, r0, #2
	cmp r1, r0
	bne _08002EF2
	movs r1, #0x86
	lsls r1, r1, #2
	add r1, ip
	movs r0, #1
	str r0, [r1]
_08002EF2:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8002EFC
sub_8002EFC: @ 0x08002EFC
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	ldr r0, _08002F54 @ =gUnknown_03000804
	ldr r1, [r0]
	ldrb r0, [r1, #7]
	cmp r0, #0
	bne _08002F58
	movs r1, #0x86
	lsls r1, r1, #2
	adds r0, r4, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08002F20
	subs r1, #4
	adds r0, r4, r1
	ldr r0, [r0]
	cmp r0, #0
	bne _08002FC2
_08002F20:
	movs r0, #0x80
	lsls r0, r0, #2
	str r0, [r4]
	movs r2, #0
	str r2, [r4, #4]
	ldr r0, [r4, #8]
	str r0, [r4, #0xc]
	movs r0, #0x84
	lsls r0, r0, #2
	adds r1, r4, r0
	adds r0, r4, #0
	adds r0, #0x10
	str r0, [r1]
	movs r1, #0x85
	lsls r1, r1, #2
	adds r0, r4, r1
	str r2, [r0]
	adds r1, #4
	adds r0, r4, r1
	str r2, [r0]
	adds r1, #4
	adds r0, r4, r1
	str r2, [r0]
	movs r0, #1
	b _08002FC4
	.align 2, 0
_08002F54: .4byte gUnknown_03000804
_08002F58:
	movs r0, #0xff
	lsls r0, r0, #2
	adds r1, r1, r0
	ldr r0, [r1]
	cmp r0, #0
	bne _08002F68
	movs r1, #1
	b _08002F74
_08002F68:
	ldr r0, [r1]
	cmp r0, #1
	beq _08002F72
	movs r0, #2
	b _08002FC4
_08002F72:
	movs r1, #0
_08002F74:
	movs r0, #0x86
	lsls r0, r0, #2
	adds r6, r4, r0
	ldr r0, [r6]
	cmp r0, #0
	bne _08002F86
	adds r0, r4, #0
	bl sub_8002E20
_08002F86:
	movs r1, #0x85
	lsls r1, r1, #2
	adds r5, r4, r1
	ldr r0, [r5]
	cmp r0, #0
	bne _08002F98
	adds r0, r4, #0
	bl sub_8002D44
_08002F98:
	movs r3, #0
	ldr r0, [r6]
	cmp r0, #0
	beq _08002FBA
	ldr r0, [r5]
	cmp r0, #0
	beq _08002FBA
	movs r1, #0x87
	lsls r1, r1, #2
	adds r0, r4, r1
	ldr r1, [r0]
	adds r2, r1, #0
	adds r1, #1
	str r1, [r0]
	cmp r2, #0x1e
	ble _08002FBA
	movs r3, #1
_08002FBA:
	cmp r3, #0
	bne _08002FC2
	movs r0, #1
	b _08002FC4
_08002FC2:
	movs r0, #0
_08002FC4:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8002FCC
sub_8002FCC: @ 0x08002FCC
	str r1, [r0, #8]
	str r1, [r0, #0xc]
	bx lr
	.align 2, 0

	thumb_func_start sub_8002FD4
sub_8002FD4: @ 0x08002FD4
	adds r0, #0x10
	bx lr

	thumb_func_start sub_8002FD8
sub_8002FD8: @ 0x08002FD8
	adds r3, r0, #0
	movs r0, #0x80
	lsls r0, r0, #2
	str r0, [r3]
	movs r2, #0
	str r2, [r3, #4]
	ldr r0, [r3, #8]
	str r0, [r3, #0xc]
	movs r0, #0x84
	lsls r0, r0, #2
	adds r1, r3, r0
	adds r0, r3, #0
	adds r0, #0x10
	str r0, [r1]
	movs r1, #0x85
	lsls r1, r1, #2
	adds r0, r3, r1
	str r2, [r0]
	adds r1, #4
	adds r0, r3, r1
	str r2, [r0]
	adds r1, #4
	adds r0, r3, r1
	str r2, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_800300C
sub_800300C: @ 0x0800300C
	push {r4, lr}
	ldr r4, _08003024 @ =gUnknown_0300080C
	ldr r2, [r4]
	str r0, [r2, #0xc]
	str r1, [r2, #0x10]
	movs r1, #0
	str r1, [r2, #4]
	strb r1, [r2, #8]
	ldr r0, [r4]
	adds r0, #0x20
	strb r1, [r0]
	b _0800303A
	.align 2, 0
_08003024: .4byte gUnknown_0300080C
_08003028:
	ldr r0, _08003060 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r0, _08003064 @ =gUnknown_030007E0
	ldrh r1, [r0, #2]
	ldr r0, [r4]
	bl sub_80031E4
_0800303A:
	ldr r0, [r4]
	bl sub_8004BD0
	bl sub_80006A8
	ldr r0, [r4]
	bl sub_8004CE8
	ldr r0, [r4]
	ldrb r0, [r0, #8]
	cmp r0, #0
	beq _08003028
	ldr r0, _08003068 @ =gUnknown_0300080C
	ldr r0, [r0]
	adds r0, #0x20
	ldrb r0, [r0]
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08003060: .4byte gUnknown_03001304
_08003064: .4byte gUnknown_030007E0
_08003068: .4byte gUnknown_0300080C

	thumb_func_start sub_800306C
sub_800306C: @ 0x0800306C
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	adds r5, r0, #0
	movs r0, #0x8c
	adds r0, r0, r5
	mov sb, r0
	movs r6, #0x80
	lsls r6, r6, #2
	adds r0, r6, #0
	bl sub_8026EDC
	adds r4, r0, #0
	bl sub_8002C84
	mov r0, sb
	str r4, [r0]
	movs r0, #0x90
	adds r0, r0, r5
	mov r8, r0
	adds r0, r6, #0
	bl sub_8026EDC
	adds r4, r0, #0
	bl sub_8002C84
	mov r0, r8
	str r4, [r0]
	ldr r0, _0800311C @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006EA8
	adds r0, r5, #0
	bl sub_800450C
	adds r0, r5, #0
	bl sub_80047F8
	ldr r0, _08003120 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x10
	bl sub_8001B54
	adds r0, r5, #0
	bl sub_80048BC
	adds r4, r5, #0
	adds r4, #0x28
	ldr r0, _08003124 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80236EC
	adds r2, r0, #0
	adds r0, r5, #0
	adds r1, r4, #0
	bl sub_80048E0
	mov r0, sb
	ldr r1, [r0]
	adds r0, r5, #0
	bl sub_8004860
	ldr r4, _08003128 @ =gUnknown_03000804
	movs r0, #0x81
	lsls r0, r0, #3
	bl sub_80016DC
	bl sub_80027E8
	str r0, [r4]
	movs r0, #0x80
	movs r1, #1
	movs r2, #0
	bl sub_800132C
	adds r1, r5, #0
	adds r1, #0x20
	movs r0, #0
	strb r0, [r1]
	adds r0, r5, #0
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0800311C: .4byte gUnknown_030012B8
_08003120: .4byte gUnknown_030012BC
_08003124: .4byte gUnknown_030012C0
_08003128: .4byte gUnknown_03000804

	thumb_func_start sub_800312C
sub_800312C: @ 0x0800312C
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r7, r0, #0
	mov sb, r1
	ldr r0, _080031E0 @ =gUnknown_03000804
	ldr r0, [r0]
	cmp r0, #0
	beq _08003146
	movs r1, #3
	bl sub_80027B0
_08003146:
	adds r0, r7, #0
	adds r0, #0x90
	ldr r0, [r0]
	bl sub_8026ED0
	adds r0, r7, #0
	adds r0, #0x8c
	ldr r0, [r0]
	bl sub_8026ED0
	adds r6, r7, #0
	adds r6, #0xd0
	adds r5, r7, #0
	adds r5, #0xbc
	adds r4, r7, #0
	adds r4, #0xa8
	movs r0, #4
	mov r8, r0
_0800316A:
	ldr r2, [r4]
	cmp r2, #0
	beq _08003182
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_08003182:
	ldr r2, [r5]
	cmp r2, #0
	beq _0800319A
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0800319A:
	ldr r2, [r6]
	cmp r2, #0
	beq _080031B2
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_080031B2:
	adds r6, #4
	adds r5, #4
	adds r4, #4
	movs r0, #1
	rsbs r0, r0, #0
	add r8, r0
	mov r1, r8
	cmp r1, #0
	bge _0800316A
	movs r0, #1
	mov r3, sb
	ands r0, r3
	cmp r0, #0
	beq _080031D4
	adds r0, r7, #0
	bl sub_8026ED0
_080031D4:
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080031E0: .4byte gUnknown_03000804

	thumb_func_start sub_80031E4
sub_80031E4: @ 0x080031E4
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	movs r7, #0
_080031EC:
	lsls r4, r7, #2
	adds r0, r5, #0
	adds r0, #0xa8
	adds r0, r0, r4
	ldr r0, [r0]
	ldr r2, [r0, #0x18]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r2, #0x1c]
	bl sub_803AD7C
	adds r0, r5, #0
	adds r0, #0xbc
	adds r0, r0, r4
	ldr r0, [r0]
	ldr r2, [r0, #0x18]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r2, #0x1c]
	bl sub_803AD7C
	adds r0, r5, #0
	adds r0, #0xd0
	adds r0, r0, r4
	ldr r0, [r0]
	ldr r2, [r0, #0x18]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r2, #0x1c]
	bl sub_803AD7C
	adds r7, #1
	cmp r7, #4
	ble _080031EC
	ldr r0, [r5, #0xc]
	cmp r0, #0xa
	bhi _080032D0
	lsls r0, r0, #2
	ldr r1, _08003248 @ =_0800324C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08003248: .4byte _0800324C
_0800324C: @ jump table
	.4byte _08003278 @ case 0
	.4byte _08003282 @ case 1
	.4byte _08003288 @ case 2
	.4byte _08003298 @ case 3
	.4byte _080032A0 @ case 4
	.4byte _080032AA @ case 5
	.4byte _080032B4 @ case 6
	.4byte _080032C8 @ case 7
	.4byte _080032D0 @ case 8
	.4byte _080032BE @ case 9
	.4byte _080032D0 @ case 10
_08003278:
	adds r0, r5, #0
	adds r1, r6, #0
	bl sub_80032E8
	b _080032D0
_08003282:
	adds r0, r5, #0
	adds r0, #0x8c
	b _0800328C
_08003288:
	adds r0, r5, #0
	adds r0, #0x90
_0800328C:
	ldr r2, [r0]
	adds r0, r5, #0
	adds r1, r6, #0
	bl sub_80034BC
	b _080032D0
_08003298:
	adds r0, r5, #0
	bl sub_80035C0
	b _080032D0
_080032A0:
	adds r0, r5, #0
	adds r1, r6, #0
	bl sub_8004CB4
	b _080032D0
_080032AA:
	adds r0, r5, #0
	adds r1, r6, #0
	bl sub_8003824
	b _080032D0
_080032B4:
	adds r0, r5, #0
	adds r1, r6, #0
	bl sub_80038D0
	b _080032D0
_080032BE:
	adds r0, r5, #0
	adds r1, r6, #0
	bl sub_800376C
	b _080032D0
_080032C8:
	adds r0, r5, #0
	adds r1, r6, #0
	bl sub_800397C
_080032D0:
	ldr r0, [r5, #4]
	adds r0, #1
	movs r1, #0xff
	ands r0, r1
	str r0, [r5, #4]
	ldr r0, [r5]
	adds r0, #1
	str r0, [r5]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80032E8
sub_80032E8: @ 0x080032E8
	push {r4, r5, lr}
	adds r4, r0, #0
	movs r0, #0xa
	ands r0, r1
	cmp r0, #0
	beq _08003308
	ldr r0, _08003304 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x47
	bl PlaySfx
	b _0800338A
	.align 2, 0
_08003304: .4byte gUnknown_030012BC
_08003308:
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08003390
	ldr r0, _08003330 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
	ldr r0, [r4, #0x10]
	cmp r0, #4
	bhi _080033DE
	lsls r0, r0, #2
	ldr r1, _08003334 @ =_08003338
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08003330: .4byte gUnknown_030012BC
_08003334: .4byte _08003338
_08003338: @ jump table
	.4byte _0800334C @ case 0
	.4byte _08003350 @ case 1
	.4byte _08003370 @ case 2
	.4byte _08003374 @ case 3
	.4byte _0800338A @ case 4
_0800334C:
	movs r0, #1
	b _08003376
_08003350:
	movs r0, #3
	str r0, [r4, #0xc]
	movs r0, #0
	str r0, [r4, #0x10]
	movs r0, #0x2b
	bl sub_8026F38
	str r0, [r4, #0x14]
	movs r0, #0x2d
	bl sub_8026F38
	str r0, [r4, #0x18]
	adds r0, r4, #0
	bl sub_8004A80
	b _080033DE
_08003370:
	movs r0, #5
	b _08003376
_08003374:
	movs r0, #6
_08003376:
	str r0, [r4, #0xc]
	movs r0, #0
	str r0, [r4, #0x10]
	adds r0, r4, #0
	adds r0, #0x8c
	ldr r1, [r0]
	adds r0, r4, #0
	bl sub_8004860
	b _080033DE
_0800338A:
	movs r0, #1
	strb r0, [r4, #8]
	b _080033DE
_08003390:
	movs r5, #0x40
	ands r5, r1
	cmp r5, #0
	beq _080033BC
	ldr r0, _080033B8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x46
	bl PlaySfx
	ldr r0, [r4, #0x10]
	subs r0, #1
	str r0, [r4, #0x10]
	cmp r0, #0
	bge _080033DE
	movs r0, #4
	str r0, [r4, #0x10]
	b _080033DE
	.align 2, 0
_080033B8: .4byte gUnknown_030012BC
_080033BC:
	movs r0, #0x80
	ands r0, r1
	cmp r0, #0
	beq _080033DE
	ldr r0, _080033E4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x46
	bl PlaySfx
	ldr r0, [r4, #0x10]
	adds r0, #1
	str r0, [r4, #0x10]
	cmp r0, #4
	ble _080033DE
	str r5, [r4, #0x10]
_080033DE:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080033E4: .4byte gUnknown_030012BC

	thumb_func_start sub_80033E8
sub_80033E8: @ 0x080033E8
	push {r4, lr}
	adds r4, r0, #0
	movs r0, #0x40
	ands r0, r1
	cmp r0, #0
	beq _0800343E
	ldr r0, _08003414 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x46
	bl PlaySfx
	ldr r0, [r4, #0x10]
	cmp r0, #4
	bhi _080034B2
	lsls r0, r0, #2
	ldr r1, _08003418 @ =_0800341C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08003414: .4byte gUnknown_030012BC
_08003418: .4byte _0800341C
_0800341C: @ jump table
	.4byte _08003430 @ case 0
	.4byte _08003434 @ case 1
	.4byte _08003430 @ case 2
	.4byte _08003434 @ case 3
	.4byte _0800343A @ case 4
_08003430:
	movs r0, #4
	b _080034B0
_08003434:
	ldr r0, [r4, #0x10]
	subs r0, #1
	b _080034B0
_0800343A:
	movs r0, #1
	b _080034B0
_0800343E:
	movs r0, #0x80
	ands r0, r1
	cmp r0, #0
	beq _0800348E
	ldr r0, _08003464 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x46
	bl PlaySfx
	ldr r0, [r4, #0x10]
	cmp r0, #4
	bhi _080034B2
	lsls r0, r0, #2
	ldr r1, _08003468 @ =_0800346C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08003464: .4byte gUnknown_030012BC
_08003468: .4byte _0800346C
_0800346C: @ jump table
	.4byte _08003480 @ case 0
	.4byte _08003486 @ case 1
	.4byte _08003480 @ case 2
	.4byte _08003486 @ case 3
	.4byte _0800348A @ case 4
_08003480:
	ldr r0, [r4, #0x10]
	adds r0, #1
	b _080034B0
_08003486:
	movs r0, #4
	b _080034B0
_0800348A:
	movs r0, #0
	b _080034B0
_0800348E:
	movs r0, #0x30
	ands r0, r1
	cmp r0, #0
	beq _080034B2
	ldr r0, [r4, #0x10]
	cmp r0, #4
	beq _080034B2
	ldr r0, _080034B8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x46
	bl PlaySfx
	ldr r0, [r4, #0x10]
	movs r1, #2
	eors r0, r1
_080034B0:
	str r0, [r4, #0x10]
_080034B2:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080034B8: .4byte gUnknown_030012BC

	thumb_func_start sub_80034BC
sub_80034BC: @ 0x080034BC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x70
	adds r6, r0, #0
	adds r7, r2, #0
	movs r0, #1
	mov r8, r0
	adds r0, r1, #0
	mov r2, r8
	ands r0, r2
	cmp r0, #0
	bne _080034DE
	movs r4, #8
	ands r4, r1
	cmp r4, #0
	beq _0800358C
_080034DE:
	ldr r1, [r6, #0x10]
	cmp r1, #4
	bne _08003500
	ldr r0, _080034FC @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
	movs r0, #0
	str r0, [r6, #0xc]
	str r0, [r6, #0x10]
	b _080035B2
	.align 2, 0
_080034FC: .4byte gUnknown_030012BC
_08003500:
	adds r0, r7, #0
	bl sub_8002CE8
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08003520
	ldr r0, _0800351C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x48
	bl PlaySfx
	b _080035B2
	.align 2, 0
_0800351C: .4byte gUnknown_030012BC
_08003520:
	ldr r5, _08003584 @ =gUnknown_030012BC
	ldr r0, [r5]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
	ldr r1, [r6, #0x10]
	adds r0, r7, #0
	mov r2, sp
	bl sub_8002C14
	ldr r4, _08003588 @ =gUnknown_030012C0
	ldr r0, [r4]
	mov r1, sp
	bl sub_80236AC
	ldr r0, [r4]
	add r1, sp, #0x68
	ldrb r1, [r1]
	bl sub_8023334
	ldr r0, [r5]
	mov r1, sp
	adds r1, #0x6a
	ldrh r1, [r1]
	bl sub_8001B50
	ldr r0, [r5]
	add r1, sp, #0x6c
	ldrh r1, [r1]
	bl sub_8001B30
	adds r5, r6, #0
	adds r5, #0x28
	ldr r0, [r4]
	bl sub_80236EC
	adds r2, r0, #0
	adds r0, r6, #0
	adds r1, r5, #0
	bl sub_80048E0
	adds r0, r6, #0
	adds r0, #0x20
	mov r1, r8
	strb r1, [r0]
	strb r1, [r6, #8]
	b _080035B2
	.align 2, 0
_08003584: .4byte gUnknown_030012BC
_08003588: .4byte gUnknown_030012C0
_0800358C:
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _080035AC
	ldr r0, _080035A8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x47
	bl PlaySfx
	str r4, [r6, #0xc]
	str r4, [r6, #0x10]
	b _080035B2
	.align 2, 0
_080035A8: .4byte gUnknown_030012BC
_080035AC:
	adds r0, r6, #0
	bl sub_80033E8
_080035B2:
	add sp, #0x70
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80035C0
sub_80035C0: @ 0x080035C0
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	bl sub_8003B40
	adds r4, r0, #0
	adds r0, r5, #0
	bl sub_8004A64
	cmp r4, #3
	bne _080035F0
	movs r0, #0
	str r0, [r5, #0xc]
	movs r0, #1
	str r0, [r5, #0x10]
	ldr r0, _080035EC @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x47
	bl PlaySfx
	b _0800368C
	.align 2, 0
_080035EC: .4byte gUnknown_030012BC
_080035F0:
	cmp r4, #2
	beq _08003604
	adds r7, r5, #0
	adds r7, #0x90
	ldr r0, [r7]
	bl sub_8002B44
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08003610
_08003604:
	movs r0, #4
	str r0, [r5, #0xc]
	movs r0, #0x2c
	bl sub_8026F38
	b _08003682
_08003610:
	adds r6, r5, #0
	adds r6, #0x8c
	ldr r0, [r6]
	bl sub_8002B94
	adds r4, r0, #0
	ldr r0, [r7]
	bl sub_8002B94
	cmp r4, r0
	bne _08003638
	movs r0, #2
	str r0, [r5, #0xc]
	movs r0, #0
	str r0, [r5, #0x10]
	ldr r1, [r7]
	adds r0, r5, #0
	bl sub_8004860
	b _0800368C
_08003638:
	ldr r0, [r7]
	bl sub_8002B94
	cmp r0, #2
	beq _08003650
	cmp r0, #3
	beq _0800366C
	movs r0, #0
	str r0, [r5, #0xc]
	movs r0, #1
	str r0, [r5, #0x10]
	b _0800368C
_08003650:
	ldr r0, [r6]
	movs r1, #2
	bl sub_8002D28
	ldr r0, [r6]
	bl sub_8002BA4
	movs r0, #4
	str r0, [r5, #0xc]
	ldr r0, _08003668 @ =gUnknown_03000810
	b _08003680
	.align 2, 0
_08003668: .4byte gUnknown_03000810
_0800366C:
	ldr r0, [r6]
	movs r1, #4
	bl sub_8002D28
	ldr r0, [r6]
	bl sub_8002BA4
	movs r0, #4
	str r0, [r5, #0xc]
	ldr r0, _08003694 @ =gUnknown_03000814
_08003680:
	ldr r0, [r0]
_08003682:
	str r0, [r5, #0x14]
	movs r0, #0x2e
	bl sub_8026F38
	str r0, [r5, #0x18]
_0800368C:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08003694: .4byte gUnknown_03000814

	thumb_func_start sub_8003698
sub_8003698: @ 0x08003698
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #0xe0
	adds r7, r0, #0
	adds r6, r1, #0
	adds r4, r7, #0
	adds r4, #0x8c
	ldr r0, [r4]
	bl sub_8002CE8
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _080036C6
	ldr r0, [r4]
	adds r1, r6, #0
	mov r2, sp
	bl sub_8002C14
	movs r0, #0
	mov sb, r0
	b _080036CA
_080036C6:
	movs r1, #1
	mov sb, r1
_080036CA:
	ldr r0, _0800372C @ =gUnknown_030012C0
	mov r8, r0
	ldr r0, [r0]
	bl sub_80236EC
	adds r1, r0, #0
	add r5, sp, #0x70
	adds r0, r5, #0
	movs r2, #0x68
	bl sub_800014C
	mov r1, r8
	ldr r0, [r1]
	bl sub_802332C
	add r1, sp, #0xd8
	strb r0, [r1]
	ldr r4, _08003730 @ =gUnknown_030012BC
	ldr r0, [r4]
	bl sub_8001ABC
	mov r1, sp
	adds r1, #0xda
	strh r0, [r1]
	ldr r0, [r4]
	bl sub_8001AC0
	add r1, sp, #0xdc
	strh r0, [r1]
	adds r4, r7, #0
	adds r4, #0x8c
	ldr r0, [r4]
	adds r1, r6, #0
	adds r2, r5, #0
	bl sub_8002C40
	ldr r0, [r4]
	bl sub_8002BA4
	cmp r0, #0
	beq _08003740
	mov r0, sb
	cmp r0, #0
	beq _08003734
	ldr r0, [r4]
	adds r1, r6, #0
	bl sub_8002C6C
	b _0800375C
	.align 2, 0
_0800372C: .4byte gUnknown_030012C0
_08003730: .4byte gUnknown_030012BC
_08003734:
	ldr r0, [r4]
	adds r1, r6, #0
	mov r2, sp
	bl sub_8002C40
	b _0800375C
_08003740:
	lsls r4, r6, #2
	adds r4, r4, r6
	lsls r4, r4, #2
	adds r4, #0x3c
	adds r4, r7, r4
	mov r1, r8
	ldr r0, [r1]
	bl sub_80236EC
	adds r2, r0, #0
	adds r0, r7, #0
	adds r1, r4, #0
	bl sub_80048E0
_0800375C:
	add sp, #0xe0
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_800376C
sub_800376C: @ 0x0800376C
	push {r4, r5, lr}
	adds r4, r0, #0
	movs r3, #1
	adds r0, r1, #0
	ands r0, r3
	cmp r0, #0
	bne _08003782
	movs r0, #8
	ands r0, r1
	cmp r0, #0
	beq _080037B4
_08003782:
	ldr r5, [r4, #0x10]
	cmp r5, #0
	bne _08003798
	ldr r1, [r4, #0x24]
	adds r0, r4, #0
	bl sub_8003698
	str r5, [r4, #0xc]
	movs r0, #4
	str r0, [r4, #0x10]
	b _0800381A
_08003798:
	movs r0, #5
	str r0, [r4, #0xc]
	ldr r0, [r4, #0x24]
	str r0, [r4, #0x10]
	ldr r0, _080037B0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
	b _0800381A
	.align 2, 0
_080037B0: .4byte gUnknown_030012BC
_080037B4:
	movs r2, #2
	ands r2, r1
	cmp r2, #0
	beq _080037D8
	movs r0, #5
	str r0, [r4, #0xc]
	ldr r0, [r4, #0x24]
	str r0, [r4, #0x10]
	ldr r0, _080037D4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x47
	bl PlaySfx
	b _0800381A
	.align 2, 0
_080037D4: .4byte gUnknown_030012BC
_080037D8:
	movs r0, #0x40
	ands r0, r1
	cmp r0, #0
	beq _080037FC
	ldr r0, [r4, #0x10]
	cmp r0, #1
	bne _0800381A
	str r2, [r4, #0x10]
	ldr r0, _080037F8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x46
	bl PlaySfx
	b _0800381A
	.align 2, 0
_080037F8: .4byte gUnknown_030012BC
_080037FC:
	movs r0, #0x80
	ands r0, r1
	cmp r0, #0
	beq _0800381A
	ldr r0, [r4, #0x10]
	cmp r0, #0
	bne _0800381A
	str r3, [r4, #0x10]
	ldr r0, _08003820 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x46
	bl PlaySfx
_0800381A:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08003820: .4byte gUnknown_030012BC

	thumb_func_start sub_8003824
sub_8003824: @ 0x08003824
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	bne _08003838
	movs r5, #8
	ands r5, r1
	cmp r5, #0
	beq _080038A2
_08003838:
	ldr r0, [r4, #0x10]
	cmp r0, #4
	bne _0800385C
	ldr r0, _08003858 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
	movs r0, #0
	str r0, [r4, #0xc]
	movs r0, #2
	str r0, [r4, #0x10]
	b _080038CA
	.align 2, 0
_08003858: .4byte gUnknown_030012BC
_0800385C:
	ldr r0, _0800388C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
	adds r0, r4, #0
	adds r0, #0x8c
	ldr r0, [r0]
	ldr r1, [r4, #0x10]
	bl sub_8002CE8
	lsls r0, r0, #0x18
	lsrs r1, r0, #0x18
	cmp r1, #0
	bne _08003890
	movs r0, #9
	str r0, [r4, #0xc]
	ldr r0, [r4, #0x10]
	str r0, [r4, #0x24]
	str r1, [r4, #0x10]
	b _080038CA
	.align 2, 0
_0800388C: .4byte gUnknown_030012BC
_08003890:
	ldr r1, [r4, #0x10]
	adds r0, r4, #0
	bl sub_8003698
	movs r0, #0
	str r0, [r4, #0xc]
	movs r0, #4
	str r0, [r4, #0x10]
	b _080038CA
_080038A2:
	movs r6, #2
	adds r0, r1, #0
	ands r0, r6
	cmp r0, #0
	beq _080038C4
	ldr r0, _080038C0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x47
	bl PlaySfx
	str r5, [r4, #0xc]
	str r6, [r4, #0x10]
	b _080038CA
	.align 2, 0
_080038C0: .4byte gUnknown_030012BC
_080038C4:
	adds r0, r4, #0
	bl sub_80033E8
_080038CA:
	pop {r4, r5, r6}
	pop {r0}
	bx r0

	thumb_func_start sub_80038D0
sub_80038D0: @ 0x080038D0
	push {r4, r5, lr}
	adds r4, r0, #0
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	bne _080038E4
	movs r5, #8
	ands r5, r1
	cmp r5, #0
	beq _0800394C
_080038E4:
	ldr r1, [r4, #0x10]
	cmp r1, #4
	bne _08003904
	ldr r0, _08003900 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
	movs r0, #0
	str r0, [r4, #0xc]
	b _08003964
	.align 2, 0
_08003900: .4byte gUnknown_030012BC
_08003904:
	adds r0, r4, #0
	adds r0, #0x8c
	ldr r0, [r0]
	bl sub_8002CE8
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	cmp r5, #0
	beq _0800392C
	ldr r0, _08003928 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x48
	bl PlaySfx
	b _08003976
	.align 2, 0
_08003928: .4byte gUnknown_030012BC
_0800392C:
	ldr r0, _08003948 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
	movs r0, #7
	str r0, [r4, #0xc]
	ldr r0, [r4, #0x10]
	str r0, [r4, #0x24]
	str r5, [r4, #0x10]
	b _08003976
	.align 2, 0
_08003948: .4byte gUnknown_030012BC
_0800394C:
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _08003970
	ldr r0, _0800396C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x47
	bl PlaySfx
	str r5, [r4, #0xc]
_08003964:
	movs r0, #3
	str r0, [r4, #0x10]
	b _08003976
	.align 2, 0
_0800396C: .4byte gUnknown_030012BC
_08003970:
	adds r0, r4, #0
	bl sub_80033E8
_08003976:
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_800397C
sub_800397C: @ 0x0800397C
	push {r4, r5, r6, r7, lr}
	sub sp, #0x70
	adds r4, r0, #0
	movs r3, #1
	adds r0, r1, #0
	ands r0, r3
	cmp r0, #0
	bne _08003994
	movs r0, #8
	ands r0, r1
	cmp r0, #0
	beq _080039EC
_08003994:
	ldr r7, [r4, #0x10]
	cmp r7, #0
	bne _080039CE
	ldr r6, [r4, #0x24]
	adds r5, r4, #0
	adds r5, #0x8c
	ldr r0, [r5]
	adds r1, r6, #0
	mov r2, sp
	bl sub_8002C14
	ldr r0, [r5]
	adds r1, r6, #0
	bl sub_8002C6C
	ldr r0, [r5]
	bl sub_8002BA4
	cmp r0, #0
	beq _080039C6
	ldr r0, [r5]
	adds r1, r6, #0
	mov r2, sp
	bl sub_8002C40
_080039C6:
	str r7, [r4, #0xc]
	movs r0, #4
	str r0, [r4, #0x10]
	b _08003A52
_080039CE:
	movs r0, #6
	str r0, [r4, #0xc]
	ldr r0, [r4, #0x24]
	str r0, [r4, #0x10]
	ldr r0, _080039E8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
	b _08003A52
	.align 2, 0
_080039E8: .4byte gUnknown_030012BC
_080039EC:
	movs r2, #2
	ands r2, r1
	cmp r2, #0
	beq _08003A10
	movs r0, #6
	str r0, [r4, #0xc]
	ldr r0, [r4, #0x24]
	str r0, [r4, #0x10]
	ldr r0, _08003A0C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x47
	bl PlaySfx
	b _08003A52
	.align 2, 0
_08003A0C: .4byte gUnknown_030012BC
_08003A10:
	movs r0, #0x40
	ands r0, r1
	cmp r0, #0
	beq _08003A34
	ldr r0, [r4, #0x10]
	cmp r0, #1
	bne _08003A52
	str r2, [r4, #0x10]
	ldr r0, _08003A30 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x46
	bl PlaySfx
	b _08003A52
	.align 2, 0
_08003A30: .4byte gUnknown_030012BC
_08003A34:
	movs r0, #0x80
	ands r0, r1
	cmp r0, #0
	beq _08003A52
	ldr r0, [r4, #0x10]
	cmp r0, #0
	bne _08003A52
	str r3, [r4, #0x10]
	ldr r0, _08003A5C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x46
	bl PlaySfx
_08003A52:
	add sp, #0x70
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08003A5C: .4byte gUnknown_030012BC

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

	thumb_func_start sub_8003B40
sub_8003B40: @ 0x08003B40
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	movs r0, #0x88
	lsls r0, r0, #2
	bl sub_8026EDC
	adds r5, r0, #0
	adds r0, r6, #0
	adds r0, #0x8c
	ldr r1, [r0]
	adds r0, r5, #0
	bl sub_8002FCC
	adds r0, r5, #0
	bl sub_8002FD8
_08003B60:
	bl sub_80006A8
	ldr r0, _08003B80 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r0, _08003B84 @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #2
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r1, r0, #0x10
	cmp r1, #0
	beq _08003B88
	movs r4, #3
	b _08003BA8
	.align 2, 0
_08003B80: .4byte gUnknown_03001304
_08003B84: .4byte gUnknown_030007E0
_08003B88:
	ldr r2, _08003BD4 @ =gUnknown_03000800
	ldrb r0, [r2]
	cmp r0, #0
	beq _08003B98
	strb r1, [r2]
	adds r0, r5, #0
	bl sub_8002FD8
_08003B98:
	ldr r0, _08003BD8 @ =gUnknown_03000804
	ldr r0, [r0]
	bl sub_8001F50
	adds r0, r5, #0
	bl sub_8002EFC
	adds r4, r0, #0
_08003BA8:
	cmp r4, #1
	beq _08003B60
	cmp r4, #0
	bne _08003BC6
	adds r0, r5, #0
	bl sub_8002FD4
	adds r1, r0, #0
	adds r0, r6, #0
	adds r0, #0x90
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #2
	bl sub_800014C
_08003BC6:
	adds r0, r5, #0
	bl sub_8026ED0
	adds r0, r4, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08003BD4: .4byte gUnknown_03000800
_08003BD8: .4byte gUnknown_03000804

	thumb_func_start sub_8003BDC
sub_8003BDC: @ 0x08003BDC
	push {r4, r5, r6, r7, lr}
	adds r5, r1, #0
	adds r6, r2, #0
	ldr r7, _08003C8C @ =gUnknown_030012DC
	ldr r0, [r7]
	movs r1, #0
	bl sub_8028A30
	cmp r5, #0
	beq _08003C3A
	ldr r0, [r7]
	movs r4, #0x98
	lsls r4, r4, #1
	adds r1, r0, r4
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	adds r1, r5, #0
	bl sub_803AD80
	adds r1, r0, #0
	movs r0, #0xf0
	subs r0, r0, r1
	asrs r3, r0, #1
	ldr r0, [r7]
	movs r1, #0x87
	mov ip, r1
	movs r2, #0x88
	lsls r2, r2, #1
	adds r1, r0, r2
	str r3, [r1]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r1, r0, r3
	mov r2, ip
	str r2, [r1]
	adds r4, r0, r4
	ldr r2, [r4]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	adds r1, r5, #0
	bl sub_803AD80
_08003C3A:
	cmp r6, #0
	beq _08003C84
	ldr r0, [r7]
	movs r4, #0x98
	lsls r4, r4, #1
	adds r1, r0, r4
	ldr r2, [r1]
	movs r5, #0x10
	ldrsh r1, [r2, r5]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	adds r1, r6, #0
	bl sub_803AD80
	adds r1, r0, #0
	movs r0, #0xf0
	subs r0, r0, r1
	asrs r3, r0, #1
	ldr r0, [r7]
	movs r2, #0x91
	movs r5, #0x88
	lsls r5, r5, #1
	adds r1, r0, r5
	str r3, [r1]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r1, r0, r3
	str r2, [r1]
	adds r4, r0, r4
	ldr r2, [r4]
	movs r5, #0x20
	ldrsh r1, [r2, r5]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	adds r1, r6, #0
	bl sub_803AD80
_08003C84:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08003C8C: .4byte gUnknown_030012DC

	thumb_func_start sub_8003C90
sub_8003C90: @ 0x08003C90
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r2, r0, #0
	lsls r1, r1, #0x18
	cmp r1, #0
	beq _08003CC0
	ldr r0, _08003CBC @ =gUnknown_030012DC
	ldr r3, [r0]
	ldr r0, [r2, #4]
	asrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	movs r1, #2
	cmp r0, #0
	beq _08003CB2
	movs r1, #1
_08003CB2:
	adds r0, r3, #0
	bl sub_8028A30
	b _08003CCA
	.align 2, 0
_08003CBC: .4byte gUnknown_030012DC
_08003CC0:
	ldr r0, _08003D38 @ =gUnknown_030012DC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8028A30
_08003CCA:
	ldr r0, _08003D38 @ =gUnknown_030012DC
	mov r8, r0
	ldr r4, [r0]
	movs r5, #0x98
	lsls r5, r5, #1
	adds r0, r4, r5
	ldr r0, [r0]
	adds r6, r0, #0
	adds r6, #0x10
	movs r1, #0x10
	ldrsh r0, [r0, r1]
	adds r4, r4, r0
	movs r0, #0x23
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r6, #4]
	adds r0, r4, #0
	bl sub_803AD80
	movs r1, #0xf0
	subs r1, r1, r0
	asrs r1, r1, #1
	mov r3, r8
	ldr r4, [r3]
	movs r2, #0x87
	movs r3, #0x88
	lsls r3, r3, #1
	adds r0, r4, r3
	str r1, [r0]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r4, r1
	str r2, [r0]
	adds r5, r4, r5
	ldr r0, [r5]
	adds r5, r0, #0
	adds r5, #0x20
	movs r3, #0x20
	ldrsh r0, [r0, r3]
	adds r4, r4, r0
	movs r0, #0x23
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08003D38: .4byte gUnknown_030012DC

	thumb_func_start sub_8003D3C
sub_8003D3C: @ 0x08003D3C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	mov sl, r0
	adds r6, r1, #0
	ldr r7, _08003E38 @ =gUnknown_030012DC
	ldr r0, [r7]
	movs r1, #0
	bl sub_8028A30
	ldr r4, [r7]
	movs r0, #0x98
	lsls r0, r0, #1
	mov r8, r0
	adds r0, r4, r0
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x10
	movs r1, #0x10
	ldrsh r0, [r0, r1]
	adds r4, r4, r0
	adds r0, r6, #0
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	movs r1, #0xa0
	subs r1, r1, r0
	ldr r4, [r7]
	movs r2, #0x87
	mov sb, r2
	movs r3, #0x88
	lsls r3, r3, #1
	adds r0, r4, r3
	str r1, [r0]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r4, r1
	str r2, [r0]
	mov r2, r8
	adds r0, r4, r2
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r3, #0x20
	ldrsh r0, [r0, r3]
	adds r4, r4, r0
	adds r0, r6, #0
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	ldr r2, [r7]
	mov r4, sl
	ldr r0, [r4, #4]
	asrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	movs r1, #2
	cmp r0, #0
	beq _08003DC8
	movs r1, #1
_08003DC8:
	adds r0, r2, #0
	bl sub_8028A30
	mov r1, sl
	ldr r0, [r1, #0x10]
	cmp r0, #0
	bne _08003E40
	ldr r0, [r7]
	movs r2, #0xa8
	movs r3, #0x88
	lsls r3, r3, #1
	adds r1, r0, r3
	str r2, [r1]
	movs r4, #0x8a
	lsls r4, r4, #1
	adds r1, r0, r4
	mov r2, sb
	str r2, [r1]
	mov r3, r8
	adds r1, r0, r3
	ldr r2, [r1]
	movs r4, #0x20
	ldrsh r1, [r2, r4]
	adds r0, r0, r1
	ldr r1, _08003E3C @ =gStaticData_0816B138
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r4, [r7]
	movs r1, #0xb0
	movs r2, #0x88
	lsls r2, r2, #1
	adds r0, r4, r2
	str r1, [r0]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r0, r4, r3
	mov r1, sb
	str r1, [r0]
	mov r2, r8
	adds r0, r4, r2
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r3, #0x20
	ldrsh r0, [r0, r3]
	adds r4, r4, r0
	movs r0, #0x29
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	b _08003E9C
	.align 2, 0
_08003E38: .4byte gUnknown_030012DC
_08003E3C: .4byte gStaticData_0816B138
_08003E40:
	ldr r0, [r7]
	movs r2, #0xa8
	movs r5, #0x91
	movs r4, #0x88
	lsls r4, r4, #1
	adds r1, r0, r4
	str r2, [r1]
	adds r2, #0x6c
	adds r1, r0, r2
	str r5, [r1]
	mov r3, r8
	adds r1, r0, r3
	ldr r2, [r1]
	movs r4, #0x20
	ldrsh r1, [r2, r4]
	adds r0, r0, r1
	ldr r1, _08003EE4 @ =gStaticData_0816B138
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r4, [r7]
	movs r1, #0xb0
	movs r2, #0x88
	lsls r2, r2, #1
	adds r0, r4, r2
	str r1, [r0]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r0, r4, r3
	str r5, [r0]
	mov r1, r8
	adds r0, r4, r1
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r2, #0x20
	ldrsh r0, [r0, r2]
	adds r4, r4, r0
	movs r0, #0x2a
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
_08003E9C:
	ldr r4, _08003EE8 @ =gUnknown_030012DC
	ldr r0, [r4]
	movs r1, #0
	bl sub_8028A30
	mov r3, sl
	ldr r0, [r3, #0x10]
	cmp r0, #0
	bne _08003EEC
	ldr r3, [r4]
	movs r1, #0xb0
	movs r2, #0x91
	movs r4, #0x88
	lsls r4, r4, #1
	adds r0, r3, r4
	str r1, [r0]
	adds r1, #0x64
	adds r0, r3, r1
	str r2, [r0]
	adds r2, #0x9f
	adds r0, r3, r2
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r1, #0x20
	ldrsh r4, [r0, r1]
	adds r4, r3, r4
	movs r0, #0x2a
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	b _08003F20
	.align 2, 0
_08003EE4: .4byte gStaticData_0816B138
_08003EE8: .4byte gUnknown_030012DC
_08003EEC:
	ldr r3, [r4]
	movs r1, #0xb0
	movs r2, #0x87
	movs r4, #0x88
	lsls r4, r4, #1
	adds r0, r3, r4
	str r1, [r0]
	adds r1, #0x64
	adds r0, r3, r1
	str r2, [r0]
	adds r2, #0xa9
	adds r0, r3, r2
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r1, #0x20
	ldrsh r4, [r0, r1]
	adds r4, r3, r4
	movs r0, #0x29
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
_08003F20:
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8003F30
sub_8003F30: @ 0x08003F30
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x1c
	adds r7, r0, #0
	mov r8, r1
	mov sl, r2
	adds r4, r3, #0
	add r0, sp, #0x3c
	ldrb r0, [r0]
	str r0, [sp, #8]
	lsls r1, r4, #2
	adds r0, r1, r4
	lsls r0, r0, #2
	adds r0, #0x28
	adds r0, r7, r0
	str r0, [sp, #0xc]
	mov r5, r8
	adds r5, #0x2b
	mov r6, sl
	adds r6, #5
	adds r0, r7, #0
	adds r0, #0xa8
	adds r0, r0, r1
	ldr r0, [r0]
	lsls r1, r5, #8
	str r1, [r0]
	lsls r1, r6, #8
	str r1, [r0, #4]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	adds r5, #0xd
	mov r6, sl
	ldr r1, [sp, #0xc]
	ldr r0, [r1, #4]
	mov r1, sp
	movs r2, #0xa
	bl sub_800094C
	ldr r2, [sp, #8]
	cmp r2, #0
	beq _08003FAC
	ldr r0, _08003FA8 @ =gUnknown_030012DC
	ldr r2, [r0]
	ldr r0, [r7, #4]
	asrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	movs r1, #2
	cmp r0, #0
	beq _08003FA0
	movs r1, #1
_08003FA0:
	adds r0, r2, #0
	bl sub_8028A30
	b _08003FB6
	.align 2, 0
_08003FA8: .4byte gUnknown_030012DC
_08003FAC:
	ldr r0, _08004040 @ =gUnknown_030012DC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8028A30
_08003FB6:
	ldr r3, _08004040 @ =gUnknown_030012DC
	mov sb, r3
	ldr r2, [r3]
	movs r1, #0x88
	lsls r1, r1, #1
	adds r0, r2, r1
	str r5, [r0]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r0, r2, r3
	str r6, [r0]
	adds r1, #0x20
	adds r0, r2, r1
	ldr r1, [r0]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0x24]
	mov r1, sp
	bl sub_803AD80
	mov r5, r8
	adds r5, #7
	mov r6, sl
	adds r6, #0x1e
	lsls r4, r4, #2
	adds r0, r7, #0
	adds r0, #0xd0
	adds r0, r0, r4
	ldr r0, [r0]
	lsls r1, r5, #8
	str r1, [r0]
	lsls r1, r6, #8
	str r1, [r0, #4]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	adds r5, #9
	subs r6, #7
	ldr r1, [sp, #0xc]
	ldr r0, [r1, #0x10]
	mov r1, sp
	movs r2, #0xa
	bl sub_800094C
	str r4, [sp, #0x10]
	mov r2, sl
	adds r2, #0x1e
	str r2, [sp, #0x18]
	str r6, [sp, #0x14]
	ldr r3, [sp, #8]
	cmp r3, #0
	beq _08004044
	mov r0, sb
	ldr r2, [r0]
	ldr r0, [r7, #4]
	asrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	movs r1, #2
	cmp r0, #0
	beq _08004036
	movs r1, #1
_08004036:
	adds r0, r2, #0
	bl sub_8028A30
	b _0800404E
	.align 2, 0
_08004040: .4byte gUnknown_030012DC
_08004044:
	mov r1, sb
	ldr r0, [r1]
	movs r1, #0
	bl sub_8028A30
_0800404E:
	ldr r4, _080040C8 @ =gUnknown_030012DC
	ldr r2, [r4]
	movs r3, #0x88
	lsls r3, r3, #1
	adds r0, r2, r3
	str r5, [r0]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r2, r1
	str r6, [r0]
	adds r3, #0x20
	adds r0, r2, r3
	ldr r1, [r0]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0x24]
	mov r1, sp
	bl sub_803AD80
	mov r5, r8
	adds r5, #0x2b
	adds r0, r7, #0
	adds r0, #0xbc
	ldr r1, [sp, #0x10]
	adds r0, r0, r1
	ldr r0, [r0]
	lsls r1, r5, #8
	str r1, [r0]
	ldr r2, [sp, #0x18]
	lsls r1, r2, #8
	str r1, [r0, #4]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	adds r5, #0xd
	ldr r6, [sp, #0x14]
	ldr r3, [sp, #0xc]
	ldr r0, [r3, #8]
	mov r1, sp
	movs r2, #0xa
	bl sub_800094C
	ldr r0, [sp, #8]
	cmp r0, #0
	beq _080040D8
	ldr r2, [r4]
	ldr r0, [r7, #4]
	asrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	movs r1, #2
	cmp r0, #0
	beq _080040BE
	movs r1, #1
_080040BE:
	adds r0, r2, #0
	bl sub_8028A30
	b _080040E0
	.align 2, 0
_080040C8: .4byte gUnknown_030012DC
_080040CC:
	movs r0, #0x25
	strb r0, [r1]
	adds r0, r3, #1
	add r0, sp
	strb r2, [r0]
	b _0800412A
_080040D8:
	ldr r0, [r4]
	movs r1, #0
	bl sub_8028A30
_080040E0:
	ldr r0, _08004168 @ =gUnknown_030012DC
	ldr r2, [r0]
	movs r1, #0x88
	lsls r1, r1, #1
	adds r0, r2, r1
	str r5, [r0]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r0, r2, r3
	str r6, [r0]
	adds r1, #0x20
	adds r0, r2, r1
	ldr r1, [r0]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0x24]
	mov r1, sp
	bl sub_803AD80
	ldr r1, [sp, #0xc]
	ldr r0, [r1]
	mov r1, sp
	movs r2, #0xa
	bl sub_800094C
	movs r3, #0
	mov r6, sl
	subs r6, #2
_0800411A:
	mov r2, sp
	adds r1, r2, r3
	ldrb r2, [r1]
	cmp r2, #0
	beq _080040CC
	adds r3, #1
	cmp r3, #6
	ble _0800411A
_0800412A:
	ldr r4, _0800416C @ =gUnknown_030012E0
	ldr r0, [r4]
	movs r3, #0x98
	lsls r3, r3, #1
	adds r1, r0, r3
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	mov r1, sp
	bl sub_803AD80
	adds r5, r0, #0
	ldr r0, [sp, #8]
	cmp r0, #0
	beq _08004170
	ldr r2, [r4]
	ldr r0, [r7, #4]
	asrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	movs r1, #2
	cmp r0, #0
	beq _0800415E
	movs r1, #1
_0800415E:
	adds r0, r2, #0
	bl sub_8028A30
	b _08004178
	.align 2, 0
_08004168: .4byte gUnknown_030012DC
_0800416C: .4byte gUnknown_030012E0
_08004170:
	ldr r0, [r4]
	movs r1, #0
	bl sub_8028A30
_08004178:
	mov r2, r8
	subs r1, r2, r5
	ldr r0, _080041B8 @ =gUnknown_030012E0
	ldr r2, [r0]
	adds r1, #0x1f
	movs r3, #0x88
	lsls r3, r3, #1
	adds r0, r2, r3
	str r1, [r0]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r2, r1
	str r6, [r0]
	adds r3, #0x20
	adds r0, r2, r3
	ldr r1, [r0]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0x24]
	mov r1, sp
	bl sub_803AD80
	add sp, #0x1c
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080041B8: .4byte gUnknown_030012E0

	thumb_func_start sub_80041BC
sub_80041BC: @ 0x080041BC
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	mov r8, r0
	mov sb, r1
	adds r7, r2, #0
	mov r0, sb
	movs r1, #0
	bl sub_8002CE8
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08004280
	movs r0, #0
	cmp r7, #0
	bne _080041E2
	movs r0, #1
_080041E2:
	cmp r0, #0
	beq _08004208
	ldr r0, _08004204 @ =gUnknown_030012DC
	ldr r2, [r0]
	mov r1, r8
	ldr r0, [r1, #4]
	asrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	movs r1, #2
	cmp r0, #0
	beq _080041FC
	movs r1, #1
_080041FC:
	adds r0, r2, #0
	bl sub_8028A30
	b _08004212
	.align 2, 0
_08004204: .4byte gUnknown_030012DC
_08004208:
	ldr r0, _0800427C @ =gUnknown_030012DC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8028A30
_08004212:
	ldr r6, _0800427C @ =gUnknown_030012DC
	ldr r4, [r6]
	movs r3, #0x98
	lsls r3, r3, #1
	adds r0, r4, r3
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x10
	movs r1, #0x10
	ldrsh r0, [r0, r1]
	adds r4, r4, r0
	movs r0, #0x25
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	movs r1, #0x43
	subs r1, r1, r0
	ldr r2, [r6]
	movs r3, #0x88
	lsls r3, r3, #1
	adds r0, r2, r3
	str r1, [r0]
	movs r0, #0x8a
	lsls r0, r0, #1
	adds r1, r2, r0
	movs r0, #0x2d
	str r0, [r1]
	movs r1, #0x98
	lsls r1, r1, #1
	adds r0, r2, r1
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r3, #0x20
	ldrsh r4, [r0, r3]
	adds r4, r2, r4
	movs r0, #0x25
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	b _08004298
	.align 2, 0
_0800427C: .4byte gUnknown_030012DC
_08004280:
	movs r1, #0
	cmp r7, #0
	bne _08004288
	movs r1, #1
_08004288:
	mov r0, sp
	strb r1, [r0]
	mov r0, r8
	movs r1, #0x26
	movs r2, #0x21
	movs r3, #1
	bl sub_8003F30
_08004298:
	mov r0, sb
	movs r1, #1
	bl sub_8002CE8
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0800434C
	movs r0, #0
	cmp r7, #1
	bne _080042AE
	movs r0, #1
_080042AE:
	cmp r0, #0
	beq _080042D4
	ldr r0, _080042D0 @ =gUnknown_030012DC
	ldr r2, [r0]
	mov r1, r8
	ldr r0, [r1, #4]
	asrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	movs r1, #2
	cmp r0, #0
	beq _080042C8
	movs r1, #1
_080042C8:
	adds r0, r2, #0
	bl sub_8028A30
	b _080042DE
	.align 2, 0
_080042D0: .4byte gUnknown_030012DC
_080042D4:
	ldr r0, _08004348 @ =gUnknown_030012DC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8028A30
_080042DE:
	ldr r6, _08004348 @ =gUnknown_030012DC
	ldr r4, [r6]
	movs r3, #0x98
	lsls r3, r3, #1
	adds r0, r4, r3
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x10
	movs r1, #0x10
	ldrsh r0, [r0, r1]
	adds r4, r4, r0
	movs r0, #0x25
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	movs r1, #0x43
	subs r1, r1, r0
	ldr r2, [r6]
	movs r3, #0x88
	lsls r3, r3, #1
	adds r0, r2, r3
	str r1, [r0]
	movs r0, #0x8a
	lsls r0, r0, #1
	adds r1, r2, r0
	movs r0, #0x5f
	str r0, [r1]
	movs r1, #0x98
	lsls r1, r1, #1
	adds r0, r2, r1
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r3, #0x20
	ldrsh r4, [r0, r3]
	adds r4, r2, r4
	movs r0, #0x25
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	b _08004364
	.align 2, 0
_08004348: .4byte gUnknown_030012DC
_0800434C:
	movs r1, #0
	cmp r7, #1
	bne _08004354
	movs r1, #1
_08004354:
	mov r0, sp
	strb r1, [r0]
	mov r0, r8
	movs r1, #0x26
	movs r2, #0x53
	movs r3, #2
	bl sub_8003F30
_08004364:
	mov r0, sb
	movs r1, #2
	bl sub_8002CE8
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08004418
	movs r0, #0
	cmp r7, #2
	bne _0800437A
	movs r0, #1
_0800437A:
	cmp r0, #0
	beq _080043A0
	ldr r0, _0800439C @ =gUnknown_030012DC
	ldr r2, [r0]
	mov r1, r8
	ldr r0, [r1, #4]
	asrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	movs r1, #2
	cmp r0, #0
	beq _08004394
	movs r1, #1
_08004394:
	adds r0, r2, #0
	bl sub_8028A30
	b _080043AA
	.align 2, 0
_0800439C: .4byte gUnknown_030012DC
_080043A0:
	ldr r0, _08004414 @ =gUnknown_030012DC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8028A30
_080043AA:
	ldr r6, _08004414 @ =gUnknown_030012DC
	ldr r4, [r6]
	movs r3, #0x98
	lsls r3, r3, #1
	adds r0, r4, r3
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x10
	movs r1, #0x10
	ldrsh r0, [r0, r1]
	adds r4, r4, r0
	movs r0, #0x25
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	movs r1, #0xa3
	subs r1, r1, r0
	ldr r2, [r6]
	movs r3, #0x88
	lsls r3, r3, #1
	adds r0, r2, r3
	str r1, [r0]
	movs r0, #0x8a
	lsls r0, r0, #1
	adds r1, r2, r0
	movs r0, #0x2d
	str r0, [r1]
	movs r1, #0x98
	lsls r1, r1, #1
	adds r0, r2, r1
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r3, #0x20
	ldrsh r4, [r0, r3]
	adds r4, r2, r4
	movs r0, #0x25
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	b _08004430
	.align 2, 0
_08004414: .4byte gUnknown_030012DC
_08004418:
	movs r1, #0
	cmp r7, #2
	bne _08004420
	movs r1, #1
_08004420:
	mov r0, sp
	strb r1, [r0]
	mov r0, r8
	movs r1, #0x86
	movs r2, #0x21
	movs r3, #3
	bl sub_8003F30
_08004430:
	mov r0, sb
	movs r1, #3
	bl sub_8002CE8
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _080044E4
	movs r0, #0
	cmp r7, #3
	bne _08004446
	movs r0, #1
_08004446:
	cmp r0, #0
	beq _0800446C
	ldr r0, _08004468 @ =gUnknown_030012DC
	ldr r2, [r0]
	mov r1, r8
	ldr r0, [r1, #4]
	asrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	movs r1, #2
	cmp r0, #0
	beq _08004460
	movs r1, #1
_08004460:
	adds r0, r2, #0
	bl sub_8028A30
	b _08004476
	.align 2, 0
_08004468: .4byte gUnknown_030012DC
_0800446C:
	ldr r0, _080044E0 @ =gUnknown_030012DC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8028A30
_08004476:
	ldr r6, _080044E0 @ =gUnknown_030012DC
	ldr r4, [r6]
	movs r3, #0x98
	lsls r3, r3, #1
	adds r0, r4, r3
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x10
	movs r1, #0x10
	ldrsh r0, [r0, r1]
	adds r4, r4, r0
	movs r0, #0x25
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	movs r1, #0xa3
	subs r1, r1, r0
	ldr r2, [r6]
	movs r3, #0x88
	lsls r3, r3, #1
	adds r0, r2, r3
	str r1, [r0]
	movs r0, #0x8a
	lsls r0, r0, #1
	adds r1, r2, r0
	movs r0, #0x5f
	str r0, [r1]
	movs r1, #0x98
	lsls r1, r1, #1
	adds r0, r2, r1
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r3, #0x20
	ldrsh r4, [r0, r3]
	adds r4, r2, r4
	movs r0, #0x25
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	b _080044FC
	.align 2, 0
_080044E0: .4byte gUnknown_030012DC
_080044E4:
	movs r1, #0
	cmp r7, #3
	bne _080044EC
	movs r1, #1
_080044EC:
	mov r0, sp
	strb r1, [r0]
	mov r0, r8
	movs r1, #0x86
	movs r2, #0x53
	movs r3, #4
	bl sub_8003F30
_080044FC:
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

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

	thumb_func_start sub_80047F8
sub_80047F8: @ 0x080047F8
	push {r4, r5, lr}
	sub sp, #0x14
	adds r4, r0, #0
	movs r5, #0
	strh r5, [r4, #0x1c]
	movs r0, #0x40
	ldrb r1, [r4, #0x1c]
	orrs r0, r1
	movs r1, #8
	rsbs r1, r1, #0
	ands r0, r1
	movs r1, #1
	orrs r0, r1
	strb r0, [r4, #0x1c]
	ldrb r0, [r4, #0x1d]
	orrs r1, r0
	movs r0, #3
	rsbs r0, r0, #0
	ands r1, r0
	movs r0, #0x10
	orrs r1, r0
	strb r1, [r4, #0x1d]
	movs r0, #3
	str r0, [sp]
	add r0, sp, #4
	movs r1, #2
	movs r2, #0x1e
	movs r3, #1
	bl sub_801E644
	ldr r1, _08004854 @ =gStaticData_0816C484
	add r0, sp, #4
	bl LoadGraphicsPackage
	str r5, [r4]
	add r0, sp, #4
	bl sub_801E640
	ldr r1, _08004858 @ =0x04000008
	strh r0, [r1]
	ldr r0, _0800485C @ =0x04000010
	str r5, [r0]
	add sp, #0x14
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08004854: .4byte gStaticData_0816C484
_08004858: .4byte 0x04000008
_0800485C: .4byte 0x04000010

	thumb_func_start sub_8004860
sub_8004860: @ 0x08004860
	push {r4, r5, r6, lr}
	sub sp, #0x70
	adds r6, r1, #0
	movs r5, #0
	adds r4, r0, #0
	adds r4, #0x3c
_0800486C:
	adds r0, r6, #0
	adds r1, r5, #0
	bl sub_8002CE8
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _080048AC
	adds r0, r6, #0
	adds r1, r5, #0
	mov r2, sp
	bl sub_8002C14
	mov r0, sp
	bl sub_8006920
	str r0, [r4, #4]
	mov r0, sp
	bl sub_80068A8
	str r0, [r4, #8]
	mov r0, sp
	bl sub_80067E4
	str r0, [r4, #0xc]
	mov r0, sp
	bl sub_800695C
	str r0, [r4, #0x10]
	mov r0, sp
	bl sub_800697C
	str r0, [r4]
_080048AC:
	adds r4, #0x14
	adds r5, #1
	cmp r5, #3
	ble _0800486C
	add sp, #0x70
	pop {r4, r5, r6}
	pop {r0}
	bx r0

	thumb_func_start sub_80048BC
sub_80048BC: @ 0x080048BC
	push {r4, lr}
	adds r4, r0, #0
	adds r4, #0x8c
	ldr r0, [r4]
	bl sub_8002A08
	subs r0, #1
	cmp r0, #3
	bhi _080048DA
	ldr r0, [r4]
	bl sub_8002C84
	ldr r0, [r4]
	bl sub_8002BA4
_080048DA:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_80048E0
sub_80048E0: @ 0x080048E0
	push {r4, r5, lr}
	adds r5, r1, #0
	adds r4, r2, #0
	adds r0, r4, #0
	bl sub_8006920
	str r0, [r5, #4]
	adds r0, r4, #0
	bl sub_80068A8
	str r0, [r5, #8]
	adds r0, r4, #0
	bl sub_80067E4
	str r0, [r5, #0xc]
	adds r0, r4, #0
	bl sub_800695C
	str r0, [r5, #0x10]
	adds r0, r4, #0
	bl sub_800697C
	str r0, [r5]
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_8004914
sub_8004914: @ 0x08004914
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r4, r0, #0
	lsls r3, r3, #0x18
	adds r7, r1, #0
	adds r7, #0x1d
	adds r2, #0xc
	mov sb, r2
	cmp r3, #0
	beq _0800494C
	ldr r0, _08004948 @ =gUnknown_030012DC
	ldr r2, [r0]
	ldr r0, [r4, #4]
	asrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	movs r1, #2
	cmp r0, #0
	beq _08004940
	movs r1, #1
_08004940:
	adds r0, r2, #0
	bl sub_8028A30
	b _08004956
	.align 2, 0
_08004948: .4byte gUnknown_030012DC
_0800494C:
	ldr r0, _080049C8 @ =gUnknown_030012DC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8028A30
_08004956:
	ldr r0, _080049C8 @ =gUnknown_030012DC
	mov r8, r0
	ldr r4, [r0]
	movs r5, #0x98
	lsls r5, r5, #1
	adds r0, r4, r5
	ldr r0, [r0]
	adds r6, r0, #0
	adds r6, #0x10
	movs r1, #0x10
	ldrsh r0, [r0, r1]
	adds r4, r4, r0
	movs r0, #0x25
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r6, #4]
	adds r0, r4, #0
	bl sub_803AD80
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	subs r0, r7, r0
	mov r2, r8
	ldr r4, [r2]
	movs r2, #0x88
	lsls r2, r2, #1
	adds r1, r4, r2
	str r0, [r1]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r4, r1
	mov r2, sb
	str r2, [r0]
	adds r5, r4, r5
	ldr r0, [r5]
	adds r5, r0, #0
	adds r5, #0x20
	movs r1, #0x20
	ldrsh r0, [r0, r1]
	adds r4, r4, r0
	movs r0, #0x25
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080049C8: .4byte gUnknown_030012DC

	thumb_func_start sub_80049CC
sub_80049CC: @ 0x080049CC
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	mov sb, r1
	ldr r6, _08004A4C @ =gUnknown_030012E0
	ldr r0, [r6]
	movs r1, #0
	bl sub_8028A30
	ldr r4, [r6]
	movs r5, #0x98
	lsls r5, r5, #1
	adds r0, r4, r5
	ldr r0, [r0]
	movs r1, #0x10
	adds r1, r1, r0
	mov r8, r1
	movs r3, #0x10
	ldrsh r0, [r0, r3]
	adds r4, r4, r0
	mov r0, sb
	bl sub_8026F38
	adds r1, r0, #0
	mov r0, r8
	ldr r2, [r0, #4]
	adds r0, r4, #0
	bl sub_803AD80
	movs r1, #0xf0
	subs r1, r1, r0
	asrs r1, r1, #1
	ldr r4, [r6]
	movs r2, #6
	movs r3, #0x88
	lsls r3, r3, #1
	adds r0, r4, r3
	str r1, [r0]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r4, r1
	str r2, [r0]
	adds r5, r4, r5
	ldr r0, [r5]
	adds r5, r0, #0
	adds r5, #0x20
	movs r3, #0x20
	ldrsh r0, [r0, r3]
	adds r4, r4, r0
	mov r0, sb
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08004A4C: .4byte gUnknown_030012E0

	thumb_func_start sub_8004A50
sub_8004A50: @ 0x08004A50
	ldr r0, [r0, #4]
	asrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	bne _08004A60
	movs r0, #2
	b _08004A62
_08004A60:
	movs r0, #1
_08004A62:
	bx lr

	thumb_func_start sub_8004A64
sub_8004A64: @ 0x08004A64
	push {r4, lr}
	ldr r0, _08004A7C @ =gUnknown_03000804
	ldr r4, [r0]
	adds r0, r4, #0
	bl sub_8002798
	movs r0, #0
	strb r0, [r4, #5]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08004A7C: .4byte gUnknown_03000804

	thumb_func_start sub_8004A80
sub_8004A80: @ 0x08004A80
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r5, _08004AA0 @ =gUnknown_03000804
	ldr r0, [r5]
	bl sub_8002798
	ldr r1, [r5]
	movs r0, #1
	strb r0, [r1, #5]
	adds r4, #0x90
	ldr r0, [r4]
	bl sub_8002C84
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08004AA0: .4byte gUnknown_03000804

	thumb_func_start sub_8004AA4
sub_8004AA4: @ 0x08004AA4
	push {r4, lr}
	adds r4, r0, #0
	movs r1, #0x1d
	bl sub_80049CC
	adds r0, r4, #0
	adds r0, #0x8c
	ldr r1, [r0]
	ldr r2, [r4, #0x24]
	adds r0, r4, #0
	bl sub_80041BC
	adds r0, r4, #0
	movs r1, #0x26
	bl sub_8003D3C
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8004ACC
sub_8004ACC: @ 0x08004ACC
	push {r4, lr}
	adds r4, r0, #0
	movs r1, #0x1d
	bl sub_80049CC
	adds r0, r4, #0
	adds r0, #0x8c
	ldr r1, [r0]
	ldr r2, [r4, #0x10]
	adds r0, r4, #0
	bl sub_80041BC
	movs r1, #0
	ldr r0, [r4, #0x10]
	cmp r0, #4
	bne _08004AEE
	movs r1, #1
_08004AEE:
	adds r0, r4, #0
	bl sub_8003C90
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8004AFC
sub_8004AFC: @ 0x08004AFC
	push {r4, lr}
	adds r4, r0, #0
	movs r1, #0x1e
	bl sub_80049CC
	adds r0, r4, #0
	adds r0, #0x8c
	ldr r1, [r0]
	ldr r2, [r4, #0x24]
	adds r0, r4, #0
	bl sub_80041BC
	adds r0, r4, #0
	movs r1, #0x27
	bl sub_8003D3C
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8004B24
sub_8004B24: @ 0x08004B24
	push {r4, lr}
	adds r4, r0, #0
	movs r1, #0x1e
	bl sub_80049CC
	adds r0, r4, #0
	adds r0, #0x8c
	ldr r1, [r0]
	ldr r2, [r4, #0x10]
	adds r0, r4, #0
	bl sub_80041BC
	movs r1, #0
	ldr r0, [r4, #0x10]
	cmp r0, #4
	bne _08004B46
	movs r1, #1
_08004B46:
	adds r0, r4, #0
	bl sub_8003C90
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8004B54
sub_8004B54: @ 0x08004B54
	push {r4, lr}
	adds r4, r0, #0
	movs r1, #0x1c
	bl sub_80049CC
	ldr r1, [r4, #0x14]
	ldr r2, [r4, #0x18]
	adds r0, r4, #0
	bl sub_8003BDC
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8004B70
sub_8004B70: @ 0x08004B70
	push {r4, lr}
	adds r4, r0, #0
	movs r1, #0x1c
	bl sub_80049CC
	adds r0, r4, #0
	adds r0, #0x90
	ldr r1, [r0]
	ldr r2, [r4, #0x10]
	adds r0, r4, #0
	bl sub_80041BC
	movs r1, #0
	ldr r0, [r4, #0x10]
	cmp r0, #4
	bne _08004B92
	movs r1, #1
_08004B92:
	adds r0, r4, #0
	bl sub_8003C90
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8004BA0
sub_8004BA0: @ 0x08004BA0
	push {r4, lr}
	adds r4, r0, #0
	movs r1, #0x1b
	bl sub_80049CC
	adds r0, r4, #0
	adds r0, #0x8c
	ldr r1, [r0]
	ldr r2, [r4, #0x10]
	adds r0, r4, #0
	bl sub_80041BC
	movs r1, #0
	ldr r0, [r4, #0x10]
	cmp r0, #4
	bne _08004BC2
	movs r1, #1
_08004BC2:
	adds r0, r4, #0
	bl sub_8003C90
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8004BD0
sub_8004BD0: @ 0x08004BD0
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, _08004BF4 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006A90
	ldr r0, _08004BF8 @ =gUnknown_030012FC
	ldr r0, [r0]
	bl sub_8006C28
	ldr r0, [r4, #0xc]
	cmp r0, #0xa
	bhi _08004C6A
	lsls r0, r0, #2
	ldr r1, _08004BFC @ =_08004C00
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08004BF4: .4byte gUnknown_03001300
_08004BF8: .4byte gUnknown_030012FC
_08004BFC: .4byte _08004C00
_08004C00: @ jump table
	.4byte _08004C2C @ case 0
	.4byte _08004C34 @ case 1
	.4byte _08004C3C @ case 2
	.4byte _08004C44 @ case 3
	.4byte _08004C44 @ case 4
	.4byte _08004C4C @ case 5
	.4byte _08004C54 @ case 6
	.4byte _08004C64 @ case 7
	.4byte _08004C6A @ case 8
	.4byte _08004C5C @ case 9
	.4byte _08004C6A @ case 10
_08004C2C:
	adds r0, r4, #0
	bl sub_8003A60
	b _08004C6A
_08004C34:
	adds r0, r4, #0
	bl sub_8004BA0
	b _08004C6A
_08004C3C:
	adds r0, r4, #0
	bl sub_8004B70
	b _08004C6A
_08004C44:
	adds r0, r4, #0
	bl sub_8004B54
	b _08004C6A
_08004C4C:
	adds r0, r4, #0
	bl sub_8004B24
	b _08004C6A
_08004C54:
	adds r0, r4, #0
	bl sub_8004ACC
	b _08004C6A
_08004C5C:
	adds r0, r4, #0
	bl sub_8004AFC
	b _08004C6A
_08004C64:
	adds r0, r4, #0
	bl sub_8004AA4
_08004C6A:
	ldr r0, _08004C78 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006A48
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08004C78: .4byte gUnknown_03001300

	thumb_func_start sub_8004C7C
sub_8004C7C: @ 0x08004C7C
	push {r4, r5, lr}
	sub sp, #0x70
	adds r5, r1, #0
	adds r4, r0, #0
	adds r4, #0x8c
	ldr r0, [r4]
	mov r2, sp
	bl sub_8002C14
	ldr r0, [r4]
	adds r1, r5, #0
	bl sub_8002C6C
	ldr r0, [r4]
	bl sub_8002BA4
	cmp r0, #0
	beq _08004CAA
	ldr r0, [r4]
	adds r1, r5, #0
	mov r2, sp
	bl sub_8002C40
_08004CAA:
	add sp, #0x70
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8004CB4
sub_8004CB4: @ 0x08004CB4
	push {r4, r5, lr}
	adds r4, r0, #0
	movs r5, #1
	adds r0, r1, #0
	ands r0, r5
	cmp r0, #0
	bne _08004CCA
	movs r0, #8
	ands r0, r1
	cmp r0, #0
	beq _08004CDE
_08004CCA:
	ldr r0, _08004CE4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
	movs r0, #0
	str r0, [r4, #0xc]
	str r5, [r4, #0x10]
_08004CDE:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08004CE4: .4byte gUnknown_030012BC

	thumb_func_start sub_8004CE8
sub_8004CE8: @ 0x08004CE8
	push {lr}
	movs r2, #0x80
	lsls r2, r2, #0x13
	ldrh r1, [r0, #0x1c]
	strh r1, [r2]
	ldr r1, _08004D14 @ =0x04000010
	ldr r0, [r0]
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, _08004D18 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _08004D1C @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	pop {r0}
	bx r0
	.align 2, 0
_08004D14: .4byte 0x04000010
_08004D18: .4byte gUnknown_030012B8
_08004D1C: .4byte gUnknown_03001300

	thumb_func_start sub_8004D20
sub_8004D20: @ 0x08004D20
	push {r4, lr}
	ldr r4, _08004D44 @ =gUnknown_0300080C
	ldr r0, [r4]
	cmp r0, #0
	beq _08004D30
	movs r1, #3
	bl sub_800312C
_08004D30:
	movs r0, #0
	str r0, [r4]
	ldr r0, _08004D48 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006EA8
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08004D44: .4byte gUnknown_0300080C
_08004D48: .4byte gUnknown_030012B8

	thumb_func_start sub_8004D4C
sub_8004D4C: @ 0x08004D4C
	push {r4, lr}
	ldr r0, _08004D6C @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006EA8
	ldr r4, _08004D70 @ =gUnknown_0300080C
	movs r0, #0xe4
	bl sub_8026EDC
	bl sub_800306C
	str r0, [r4]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08004D6C: .4byte gUnknown_030012B8
_08004D70: .4byte gUnknown_0300080C

	thumb_func_start sub_8004D74
sub_8004D74: @ 0x08004D74
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #0xc0
	lsls r0, r0, #0x18
	mov sl, r0
	bl mem_free_bytes
	ldr r0, _08004EA4 @ =gUnknown_030012BC
	ldr r0, [r0]
	bl sub_80019E8
	bl sub_80006A8
	movs r0, #0xa0
	lsls r0, r0, #0x13
	movs r1, #0
	strh r1, [r0]
	movs r0, #0x80
	lsls r0, r0, #0x13
	strh r1, [r0]
	ldr r6, _08004EA8 @ =gUnknown_030012B8
	ldr r1, [r6]
	mov sb, r1
	movs r0, #0x8c
	lsls r0, r0, #2
	bl sub_8026EDC
	bl sub_8006FB4
	str r0, [r6]
	ldr r2, _08004EAC @ =gStaticData_084A5600
	ldrh r1, [r2, #0xe]
	ldr r2, [r2, #8]
	bl sub_8006EF0
	ldr r0, [r6]
	movs r1, #0xf
	bl sub_8006D50
	ldr r1, [r6]
	ldr r0, _08004EB0 @ =gStaticData_0816B2C0
	movs r2, #0x83
	lsls r2, r2, #2
	adds r1, r1, r2
	movs r2, #0x10
	bl sub_803A94C
	ldr r4, _08004EB4 @ =gUnknown_030012DC
	ldr r0, [r4]
	bl sub_8028A40
	ldr r5, _08004EB8 @ =gUnknown_030012E0
	ldr r0, [r5]
	bl sub_8028A40
	ldr r0, [r4]
	movs r3, #0
	mov r8, r3
	movs r2, #0x84
	lsls r2, r2, #1
	adds r1, r0, r2
	str r3, [r1]
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
	ldr r0, [r4]
	movs r1, #0x96
	lsls r1, r1, #1
	adds r0, r0, r1
	ldr r2, [r0]
	ldr r0, [r5]
	movs r3, #0x84
	lsls r3, r3, #1
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
	ldr r0, [r4]
	movs r1, #0x96
	lsls r1, r1, #1
	adds r0, r0, r1
	ldr r1, [r0]
	ldr r0, [r5]
	movs r2, #0x96
	lsls r2, r2, #1
	adds r0, r0, r2
	ldr r2, [r0]
	ldr r7, _08004EBC @ =gUnknown_030012FC
	ldr r0, [r7]
	adds r1, r1, r2
	str r1, [r0, #8]
	bl sub_8006C4C
	movs r0, #0xd4
	bl sub_8026EDC
	bl sub_8004EC0
	adds r4, r0, #0
	bl sub_8005100
	adds r5, r0, #0
	cmp r4, #0
	beq _08004E74
	adds r0, r4, #0
	movs r1, #3
	bl sub_8005004
_08004E74:
	ldr r0, [r7]
	mov r3, r8
	str r3, [r0, #8]
	bl sub_8006C4C
	ldr r0, [r6]
	cmp r0, #0
	beq _08004E8A
	movs r1, #3
	bl sub_8006F94
_08004E8A:
	mov r0, sb
	str r0, [r6]
	mov r0, sl
	bl mem_free_bytes
	adds r0, r5, #0
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08004EA4: .4byte gUnknown_030012BC
_08004EA8: .4byte gUnknown_030012B8
_08004EAC: .4byte gStaticData_084A5600
_08004EB0: .4byte gStaticData_0816B2C0
_08004EB4: .4byte gUnknown_030012DC
_08004EB8: .4byte gUnknown_030012E0
_08004EBC: .4byte gUnknown_030012FC

	thumb_func_start sub_8004EC0
sub_8004EC0: @ 0x08004EC0
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r7, r0, #0
	movs r0, #3
	str r0, [sp]
	adds r0, r7, #0
	movs r1, #0
	movs r2, #0x1f
	movs r3, #0
	bl sub_801E644
	adds r4, r7, #0
	adds r4, #0xc8
	movs r6, #0
	str r6, [r4]
	movs r0, #0xc0
	ldrb r1, [r4]
	orrs r0, r1
	movs r1, #0x20
	orrs r0, r1
	movs r3, #1
	orrs r0, r3
	movs r1, #2
	orrs r0, r1
	movs r1, #4
	orrs r0, r1
	movs r1, #8
	orrs r0, r1
	movs r5, #0x10
	orrs r0, r5
	strb r0, [r4]
	adds r2, r7, #0
	adds r2, #0xcc
	movs r0, #0x20
	rsbs r0, r0, #0
	ldrb r1, [r2]
	ands r0, r1
	orrs r0, r5
	strb r0, [r2]
	ldr r1, _08004FC0 @ =0x04000050
	ldr r0, [r4]
	str r0, [r1]
	adds r1, #4
	ldrb r2, [r2]
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	adds r2, r7, #0
	adds r2, #0xd0
	strh r6, [r2]
	movs r0, #0x40
	ldrb r1, [r2]
	orrs r0, r1
	movs r1, #8
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r2]
	adds r0, r7, #0
	adds r0, #0xd1
	ldrb r2, [r0]
	orrs r3, r2
	orrs r3, r5
	strb r3, [r0]
	ldr r1, _08004FC4 @ =gStaticData_0816B284
	adds r0, r7, #0
	bl LoadGraphicsPackage
	ldr r5, _08004FC8 @ =gUnknown_030012C0
	ldr r0, [r5]
	bl sub_80236EC
	str r0, [r7, #0x10]
	adds r0, r7, #0
	bl sub_800599C
	subs r4, #8
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	str r0, [r4]
	ldr r1, _08004FCC @ =gUnknown_030012D0
	ldr r1, [r1]
	ldr r1, [r1]
	ldr r1, [r1]
	movs r3, #0x8a
	lsls r3, r3, #2
	adds r1, r1, r3
	str r1, [r0, #0x20]
	bl sub_800815C
	ldr r2, [r4]
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldr r1, [r4]
	movs r0, #0xee
	lsls r0, r0, #7
	str r0, [r1]
	movs r0, #0xbc
	lsls r0, r0, #7
	str r0, [r1, #4]
	movs r0, #0x78
	bl sub_8000E1C
	adds r1, r7, #0
	adds r1, #0xc4
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r0, #0x78
	str r0, [r1]
	ldr r0, _08004FD0 @ =gStaticData_0816B298
	str r0, [r7, #0x14]
	str r6, [r7, #0x18]
	ldr r0, [r5]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _08004FD4
	movs r0, #5
	b _08004FD6
	.align 2, 0
_08004FC0: .4byte 0x04000050
_08004FC4: .4byte gStaticData_0816B284
_08004FC8: .4byte gUnknown_030012C0
_08004FCC: .4byte gUnknown_030012D0
_08004FD0: .4byte gStaticData_0816B298
_08004FD4:
	movs r0, #4
_08004FD6:
	str r0, [r7, #0x1c]
	movs r0, #0x10
	str r0, [r7, #0x20]
	movs r4, #0
	str r4, [r7, #0x24]
	movs r0, #0xb4
	str r0, [r7, #0x28]
	adds r0, r7, #0
	bl sub_801E640
	ldr r1, _08004FFC @ =0x04000008
	strh r0, [r1]
	ldr r0, _08005000 @ =0x04000010
	str r4, [r0]
	adds r0, r7, #0
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08004FFC: .4byte 0x04000008
_08005000: .4byte 0x04000010

	thumb_func_start sub_8005004
sub_8005004: @ 0x08005004
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	adds r6, r0, #0
	mov sl, r1
	adds r0, #0xc0
	ldr r2, [r0]
	cmp r2, #0
	beq _0800502C
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0800502C:
	adds r0, r6, #0
	adds r0, #0xbc
	ldr r2, [r0]
	cmp r2, #0
	beq _08005048
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_08005048:
	adds r7, r6, #0
	adds r7, #0x9c
	movs r0, #0x8c
	adds r0, r0, r6
	mov r8, r0
	movs r1, #0x88
	adds r1, r1, r6
	mov sb, r1
	adds r4, r6, #0
	adds r4, #0xb0
	movs r5, #2
_0800505E:
	ldr r2, [r4]
	cmp r2, #0
	beq _08005076
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_08005076:
	adds r4, #4
	subs r5, #1
	cmp r5, #0
	bge _0800505E
	adds r4, r7, #0
	movs r5, #4
_08005082:
	ldr r2, [r4]
	cmp r2, #0
	beq _0800509A
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0800509A:
	adds r4, #4
	subs r5, #1
	cmp r5, #0
	bge _08005082
	mov r4, r8
	movs r5, #3
_080050A6:
	ldr r2, [r4]
	cmp r2, #0
	beq _080050BE
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_080050BE:
	adds r4, #4
	subs r5, #1
	cmp r5, #0
	bge _080050A6
	mov r0, sb
	ldr r2, [r0]
	cmp r2, #0
	beq _080050E0
	ldr r1, [r2, #0x18]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_080050E0:
	movs r0, #1
	mov r1, sl
	ands r0, r1
	cmp r0, #0
	beq _080050F0
	adds r0, r6, #0
	bl sub_8026ED0
_080050F0:
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8005100
sub_8005100: @ 0x08005100
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #0xcc
	movs r0, #0x1f
	ldrb r1, [r6]
	ands r0, r1
	cmp r0, #0
	beq _0800514E
	movs r2, #0x20
	rsbs r2, r2, #0
	adds r7, r2, #0
_0800511E:
	adds r4, r6, #0
	ldrb r2, [r6]
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1b
	subs r0, #1
	movs r1, #0x1f
	ands r0, r1
	ands r2, r7
	orrs r2, r0
	strb r2, [r6]
	adds r0, r5, #0
	bl sub_80053F4
	adds r0, r5, #0
	bl sub_8006250
	adds r0, r5, #0
	bl sub_8005304
	movs r0, #0x1f
	ldrb r4, [r4]
	ands r0, r4
	cmp r0, #0
	bne _0800511E
_0800514E:
	adds r4, r5, #0
	adds r4, #0xcc
	movs r0, #0xd0
	adds r0, r0, r5
	mov sb, r0
	b _08005180
_0800515A:
	ldr r1, _08005178 @ =gUnknown_030007E0
	movs r0, #8
	ldrh r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _08005180
	ldr r0, _0800517C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
	movs r7, #0
	b _0800529A
	.align 2, 0
_08005178: .4byte gUnknown_030007E0
_0800517C: .4byte gUnknown_030012BC
_08005180:
	adds r0, r5, #0
	bl sub_80053F4
	adds r0, r5, #0
	bl sub_8006250
	adds r0, r5, #0
	bl sub_8005304
	ldr r0, _080051F8 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r6, _080051FC @ =gUnknown_030007E0
	movs r0, #0x40
	ldrh r1, [r6, #2]
	ands r0, r1
	cmp r0, #0
	beq _080051BE
	adds r0, r5, #0
	bl sub_800609C
	movs r0, #0x1e
	str r0, [r5, #0x68]
	ldr r0, _08005200 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x46
	bl PlaySfx
_080051BE:
	movs r0, #0x80
	ldrh r2, [r6, #2]
	ands r0, r2
	cmp r0, #0
	beq _080051E0
	adds r0, r5, #0
	bl sub_8006084
	movs r0, #0x1e
	str r0, [r5, #0x68]
	ldr r0, _08005200 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x46
	bl PlaySfx
_080051E0:
	ldr r2, [r6]
	lsrs r1, r2, #0x10
	movs r3, #0x20
	movs r0, #0x20
	ands r0, r1
	cmp r0, #0
	beq _08005204
	adds r0, r5, #0
	bl sub_8005EF4
	movs r0, #0x1e
	b _0800521C
	.align 2, 0
_080051F8: .4byte gUnknown_03001304
_080051FC: .4byte gUnknown_030007E0
_08005200: .4byte gUnknown_030012BC
_08005204:
	ands r2, r3
	cmp r2, #0
	beq _0800521E
	ldr r0, [r5, #0x68]
	cmp r0, #0
	bne _0800521A
	adds r0, r5, #0
	bl sub_8005EF4
	movs r0, #5
	b _0800521C
_0800521A:
	subs r0, #1
_0800521C:
	str r0, [r5, #0x68]
_0800521E:
	ldr r0, _08005238 @ =gUnknown_030007E0
	ldr r2, [r0]
	lsrs r1, r2, #0x10
	movs r3, #0x10
	movs r0, #0x10
	ands r0, r1
	cmp r0, #0
	beq _0800523C
	adds r0, r5, #0
	bl sub_8005FBC
	movs r0, #0x1e
	b _08005254
	.align 2, 0
_08005238: .4byte gUnknown_030007E0
_0800523C:
	ands r2, r3
	cmp r2, #0
	beq _08005256
	ldr r0, [r5, #0x68]
	cmp r0, #0
	bne _08005252
	adds r0, r5, #0
	bl sub_8005FBC
	movs r0, #5
	b _08005254
_08005252:
	subs r0, #1
_08005254:
	str r0, [r5, #0x68]
_08005256:
	ldr r1, _08005284 @ =gUnknown_030007E0
	movs r0, #1
	ldrh r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	bne _08005264
	b _0800515A
_08005264:
	ldr r0, [r5, #0x18]
	ldr r1, [r5, #0x14]
	lsls r0, r0, #3
	adds r0, r0, r1
	ldr r7, [r0, #4]
	subs r0, r7, #4
	cmp r0, #1
	bhi _0800528C
	ldr r0, _08005288 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x48
	bl PlaySfx
	b _0800515A
	.align 2, 0
_08005284: .4byte gUnknown_030007E0
_08005288: .4byte gUnknown_030012BC
_0800528C:
	ldr r0, _08005300 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
_0800529A:
	adds r6, r4, #0
	movs r0, #0x1f
	ldrb r1, [r6]
	ands r0, r1
	cmp r0, #0x10
	beq _080052DE
	movs r2, #0x20
	rsbs r2, r2, #0
	mov r8, r2
_080052AC:
	adds r4, r6, #0
	ldrb r2, [r6]
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1b
	adds r0, #1
	movs r1, #0x1f
	ands r0, r1
	mov r1, r8
	ands r2, r1
	orrs r2, r0
	strb r2, [r6]
	adds r0, r5, #0
	bl sub_80053F4
	adds r0, r5, #0
	bl sub_8006250
	adds r0, r5, #0
	bl sub_8005304
	movs r0, #0x1f
	ldrb r4, [r4]
	ands r0, r4
	cmp r0, #0x10
	bne _080052AC
_080052DE:
	movs r0, #0
	mov r2, sb
	strh r0, [r2]
	movs r0, #0x40
	ldrb r1, [r2]
	orrs r0, r1
	strb r0, [r2]
	adds r0, r5, #0
	bl sub_8006250
	adds r0, r7, #0
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08005300: .4byte gUnknown_030012BC

	thumb_func_start sub_8005304
sub_8005304: @ 0x08005304
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	ldr r0, [r5, #0x24]
	cmp r0, #4
	bhi _08005382
	lsls r0, r0, #2
	ldr r1, _08005318 @ =_0800531C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08005318: .4byte _0800531C
_0800531C: @ jump table
	.4byte _08005330 @ case 0
	.4byte _0800533C @ case 1
	.4byte _08005350 @ case 2
	.4byte _08005364 @ case 3
	.4byte _08005378 @ case 4
_08005330:
	adds r0, r5, #0
	adds r0, #0x88
	ldr r0, [r0]
	bl sub_8008044
	b _08005382
_0800533C:
	adds r6, r5, #0
	adds r6, #0x8c
	movs r4, #3
_08005342:
	ldm r6!, {r0}
	bl sub_8008044
	subs r4, #1
	cmp r4, #0
	bge _08005342
	b _08005382
_08005350:
	adds r6, r5, #0
	adds r6, #0x9c
	movs r4, #4
_08005356:
	ldm r6!, {r0}
	bl sub_8008044
	subs r4, #1
	cmp r4, #0
	bge _08005356
	b _08005382
_08005364:
	adds r6, r5, #0
	adds r6, #0xb0
	movs r4, #2
_0800536A:
	ldm r6!, {r0}
	bl sub_8008044
	subs r4, #1
	cmp r4, #0
	bge _0800536A
	b _08005382
_08005378:
	adds r0, r5, #0
	adds r0, #0xbc
	ldr r0, [r0]
	bl sub_8008044
_08005382:
	ldr r0, [r5, #0x28]
	subs r0, #1
	str r0, [r5, #0x28]
	cmp r0, #0
	bne _0800539E
	ldr r0, [r5, #0x24]
	adds r0, #1
	str r0, [r5, #0x24]
	movs r1, #5
	bl sub_803AE4C
	str r0, [r5, #0x24]
	movs r0, #0xb4
	str r0, [r5, #0x28]
_0800539E:
	adds r6, r5, #0
	adds r6, #0xc4
	ldr r1, [r6]
	cmp r1, #0
	bne _080053E8
	adds r0, r5, #0
	adds r0, #0xc0
	ldr r4, [r0]
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _080053E0
	adds r0, r4, #0
	adds r0, #0x2d
	strb r1, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	movs r0, #0x78
	bl sub_8000E1C
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r0, #0x78
	b _080053EA
_080053E0:
	adds r0, r4, #0
	bl sub_8008044
	b _080053EC
_080053E8:
	subs r0, r1, #1
_080053EA:
	str r0, [r6]
_080053EC:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80053F4
sub_80053F4: @ 0x080053F4
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	ldr r0, _080054F8 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006A90
	ldr r0, _080054FC @ =gUnknown_030012FC
	ldr r0, [r0]
	bl sub_8006C28
	ldr r6, _08005500 @ =gUnknown_030012E0
	ldr r0, [r6]
	movs r7, #0x98
	lsls r7, r7, #1
	adds r1, r0, r7
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r5, #0x70]
	ldr r2, [r2, #0x14]
	bl sub_803AD80
	movs r1, #0xf0
	subs r1, r1, r0
	lsrs r3, r1, #1
	ldr r0, [r6]
	movs r2, #0xe
	movs r4, #0x88
	lsls r4, r4, #1
	adds r1, r0, r4
	str r3, [r1]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r1, r0, r3
	str r2, [r1]
	adds r1, r0, r7
	ldr r2, [r1]
	movs r4, #0x20
	ldrsh r1, [r2, r4]
	adds r0, r0, r1
	ldr r1, [r5, #0x70]
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r4, [r5, #0x74]
	cmp r4, #0
	beq _08005498
	ldr r3, [r6]
	movs r1, #0x20
	movs r0, #0x26
	mov ip, r0
	movs r2, #0x88
	lsls r2, r2, #1
	adds r0, r3, r2
	str r1, [r0]
	adds r1, #0xf4
	adds r0, r3, r1
	mov r2, ip
	str r2, [r0]
	adds r1, r7, #0
	adds r0, r3, r1
	ldr r1, [r0]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r3, r0
	ldr r2, [r1, #0x24]
	adds r1, r4, #0
	bl sub_803AD80
	ldr r0, [r6]
	adds r3, r7, #0
	adds r1, r0, r3
	ldr r2, [r1]
	movs r4, #0x20
	ldrsh r1, [r2, r4]
	adds r0, r0, r1
	adds r1, r5, #0
	adds r1, #0x78
	ldr r2, [r2, #0x24]
	bl sub_803AD80
_08005498:
	ldr r0, [r6]
	adds r1, r0, r7
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	adds r4, r5, #0
	adds r4, #0x41
	ldr r2, [r2, #0x14]
	adds r1, r4, #0
	bl sub_803AD80
	movs r1, #0x8c
	subs r3, r1, r0
	ldr r0, [r6]
	movs r2, #0x88
	movs r6, #0x88
	lsls r6, r6, #1
	adds r1, r0, r6
	str r3, [r1]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r1, r0, r3
	str r2, [r1]
	adds r1, r0, r7
	ldr r2, [r1]
	movs r6, #0x20
	ldrsh r1, [r2, r6]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	adds r1, r4, #0
	bl sub_803AD80
	adds r0, r5, #0
	bl sub_800556C
	adds r0, r5, #0
	bl sub_80061E8
	ldr r0, [r5, #0x24]
	cmp r0, #4
	bhi _08005542
	lsls r0, r0, #2
	ldr r1, _08005504 @ =_08005508
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080054F8: .4byte gUnknown_03001300
_080054FC: .4byte gUnknown_030012FC
_08005500: .4byte gUnknown_030012E0
_08005504: .4byte _08005508
_08005508: @ jump table
	.4byte _0800551C @ case 0
	.4byte _08005524 @ case 1
	.4byte _0800552C @ case 2
	.4byte _08005534 @ case 3
	.4byte _0800553C @ case 4
_0800551C:
	adds r0, r5, #0
	bl sub_800619C
	b _08005542
_08005524:
	adds r0, r5, #0
	bl sub_800570C
	b _08005542
_0800552C:
	adds r0, r5, #0
	bl sub_80057E0
	b _08005542
_08005534:
	adds r0, r5, #0
	bl sub_80058C0
	b _08005542
_0800553C:
	adds r0, r5, #0
	bl sub_8006124
_08005542:
	adds r0, r5, #0
	adds r0, #0xc4
	ldr r0, [r0]
	cmp r0, #0
	bne _0800555A
	adds r0, r5, #0
	adds r0, #0xc0
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
_0800555A:
	ldr r0, _08005568 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006A48
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08005568: .4byte gUnknown_03001300

	thumb_func_start sub_800556C
sub_800556C: @ 0x0800556C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xc
	mov sb, r0
	movs r0, #0x4a
	str r0, [sp]
	movs r1, #0
	mov sl, r1
	mov r2, sb
	ldr r0, [r2, #0x1c]
	cmp sl, r0
	blt _0800558C
	b _080056F0
_0800558C:
	adds r2, #0x57
	str r2, [sp, #4]
	mov r3, sb
	adds r3, #0x4f
	str r3, [sp, #8]
_08005596:
	mov r1, sb
	ldr r0, [r1, #0x18]
	cmp sl, r0
	bne _080055B0
	ldr r0, _080055AC @ =gUnknown_030012DC
	ldr r0, [r0]
	movs r1, #0xf
	bl sub_8028A30
	b _080055B8
	.align 2, 0
_080055AC: .4byte gUnknown_030012DC
_080055B0:
	ldr r0, _08005624 @ =gUnknown_030012DC
	ldr r0, [r0]
	bl sub_8028A40
_080055B8:
	mov r2, sb
	ldr r0, [r2, #0x14]
	mov r3, sl
	lsls r4, r3, #3
	adds r0, r4, r0
	ldr r0, [r0]
	bl sub_8026F38
	adds r7, r0, #0
	ldr r6, _08005624 @ =gUnknown_030012DC
	ldr r0, [r6]
	movs r1, #0x98
	lsls r1, r1, #1
	mov r8, r1
	adds r1, r0, r1
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	adds r1, r7, #0
	bl sub_803AD80
	lsrs r0, r0, #1
	movs r1, #0x32
	subs r5, r1, r0
	mov r1, sb
	ldr r0, [r1, #0x14]
	adds r4, r4, r0
	ldr r0, [r4, #4]
	cmp r0, #4
	beq _08005628
	cmp r0, #5
	beq _08005682
	ldr r0, [r6]
	movs r2, #0x88
	lsls r2, r2, #1
	adds r1, r0, r2
	str r5, [r1]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r1, r0, r3
	ldr r2, [sp]
	str r2, [r1]
	mov r3, r8
	adds r1, r0, r3
	ldr r2, [r1]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	adds r1, r7, #0
	b _080056D6
	.align 2, 0
_08005624: .4byte gUnknown_030012DC
_08005628:
	ldr r0, [r6]
	movs r2, #0x98
	lsls r2, r2, #1
	adds r1, r0, r2
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	ldr r1, [sp, #4]
	bl sub_803AD80
	lsrs r0, r0, #1
	subs r5, r5, r0
	ldr r2, [r6]
	movs r1, #0x88
	lsls r1, r1, #1
	adds r0, r2, r1
	str r5, [r0]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r0, r2, r3
	ldr r1, [sp]
	str r1, [r0]
	adds r3, #0x1c
	adds r0, r2, r3
	ldr r1, [r0]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0x24]
	adds r1, r7, #0
	bl sub_803AD80
	ldr r0, [r6]
	movs r2, #0x98
	lsls r2, r2, #1
	adds r1, r0, r2
	ldr r2, [r1]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	ldr r1, [sp, #4]
	b _080056D6
_08005682:
	ldr r0, [r6]
	mov r2, r8
	adds r1, r0, r2
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	ldr r1, [sp, #8]
	bl sub_803AD80
	lsrs r0, r0, #1
	subs r5, r5, r0
	ldr r0, [r6]
	movs r2, #0x88
	lsls r2, r2, #1
	adds r1, r0, r2
	str r5, [r1]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r1, r0, r3
	ldr r2, [sp]
	str r2, [r1]
	mov r3, r8
	adds r1, r0, r3
	ldr r2, [r1]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	adds r1, r7, #0
	bl sub_803AD80
	ldr r0, [r6]
	mov r2, r8
	adds r1, r0, r2
	ldr r2, [r1]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	ldr r1, [sp, #8]
_080056D6:
	bl sub_803AD80
	mov r1, sb
	ldr r0, [r1, #0x20]
	ldr r2, [sp]
	adds r2, r2, r0
	str r2, [sp]
	movs r3, #1
	add sl, r3
	ldr r0, [r1, #0x1c]
	cmp sl, r0
	bge _080056F0
	b _08005596
_080056F0:
	ldr r0, _08005708 @ =gUnknown_030012DC
	ldr r0, [r0]
	bl sub_8028A40
	add sp, #0xc
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08005708: .4byte gUnknown_030012DC

	thumb_func_start sub_800570C
sub_800570C: @ 0x0800570C
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	movs r2, #1
	ldr r1, [r4, #0x10]
	movs r0, #0x20
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _0800572E
	adds r0, r4, #0
	adds r0, #0x8c
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	movs r2, #0
_0800572E:
	ldr r1, [r4, #0x10]
	movs r0, #0x80
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _0800574A
	adds r0, r4, #0
	adds r0, #0x90
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	movs r2, #0
_0800574A:
	ldr r1, [r4, #0x10]
	movs r0, #0x40
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _08005766
	adds r0, r4, #0
	adds r0, #0x94
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	movs r2, #0
_08005766:
	ldr r1, [r4, #0x10]
	movs r0, #0x10
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _08005782
	adds r0, r4, #0
	adds r0, #0x98
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	movs r2, #0
_08005782:
	cmp r2, #0
	beq _080057D4
	movs r0, #0x3a
	bl sub_8026F38
	adds r6, r0, #0
	ldr r5, _080057DC @ =gUnknown_030012DC
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
	movs r3, #0x64
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
_080057D4:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_080057DC: .4byte gUnknown_030012DC

	thumb_func_start sub_80057E0
sub_80057E0: @ 0x080057E0
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	ldr r1, [r5, #0x10]
	movs r0, #1
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _080057FE
	adds r0, r5, #0
	adds r0, #0xa0
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
_080057FE:
	ldr r1, [r5, #0x10]
	movs r0, #4
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _08005818
	adds r0, r5, #0
	adds r0, #0xa4
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
_08005818:
	ldr r1, [r5, #0x10]
	movs r0, #8
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _08005832
	adds r0, r5, #0
	adds r0, #0xa8
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
_08005832:
	ldr r1, [r5, #0x10]
	movs r0, #2
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _0800584C
	adds r0, r5, #0
	adds r0, #0xac
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
_0800584C:
	adds r0, r5, #0
	adds r0, #0x9c
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	ldr r0, _080058B8 @ =gStaticData_0816B21C
	ldr r4, _080058BC @ =gUnknown_030012DC
	ldr r3, [r4]
	ldr r1, [r0]
	subs r1, #0x14
	ldr r2, [r0, #4]
	subs r2, #4
	movs r6, #0x88
	lsls r6, r6, #1
	adds r0, r3, r6
	str r1, [r0]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r3, r1
	str r2, [r0]
	adds r6, #0x20
	adds r0, r3, r6
	ldr r2, [r0]
	movs r1, #0x20
	ldrsh r0, [r2, r1]
	adds r0, r3, r0
	adds r1, r5, #0
	adds r1, #0x2f
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r3, [r4]
	movs r1, #0xb4
	movs r2, #0x80
	movs r4, #0x88
	lsls r4, r4, #1
	adds r0, r3, r4
	str r1, [r0]
	subs r6, #0x1c
	adds r0, r3, r6
	str r2, [r0]
	adds r1, r5, #0
	adds r1, #0x32
	adds r2, r5, #0
	adds r2, #0x49
	adds r0, r5, #0
	bl sub_8005E5C
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_080058B8: .4byte gStaticData_0816B21C
_080058BC: .4byte gUnknown_030012DC

	thumb_func_start sub_80058C0
sub_80058C0: @ 0x080058C0
	push {r4, r5, r6, r7, lr}
	adds r7, r0, #0
	adds r5, r7, #0
	adds r5, #0xb0
	movs r4, #2
_080058CA:
	ldm r5!, {r0}
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	subs r4, #1
	cmp r4, #0
	bge _080058CA
	ldr r4, _08005994 @ =gStaticData_0816B258
	ldr r6, _08005998 @ =gUnknown_030012DC
	ldr r0, [r6]
	ldr r2, [r4, #0x10]
	subs r2, #4
	ldr r3, [r4, #0x14]
	adds r3, #0xe
	movs r5, #0x88
	lsls r5, r5, #1
	adds r1, r0, r5
	str r2, [r1]
	movs r2, #0x8a
	lsls r2, r2, #1
	adds r1, r0, r2
	str r3, [r1]
	adds r5, #0x20
	adds r1, r0, r5
	ldr r2, [r1]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	adds r1, r7, #0
	adds r1, #0x38
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r0, [r6]
	ldr r2, [r4, #8]
	subs r2, #4
	ldr r3, [r4, #0xc]
	adds r3, #0xe
	mov ip, r3
	movs r3, #0x88
	lsls r3, r3, #1
	adds r1, r0, r3
	str r2, [r1]
	movs r2, #0x8a
	lsls r2, r2, #1
	adds r1, r0, r2
	mov r3, ip
	str r3, [r1]
	adds r1, r0, r5
	ldr r2, [r1]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	adds r1, r7, #0
	adds r1, #0x3b
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r0, [r6]
	ldr r2, [r4]
	subs r2, #4
	ldr r3, [r4, #4]
	adds r3, #0xe
	movs r4, #0x88
	lsls r4, r4, #1
	adds r1, r0, r4
	str r2, [r1]
	movs r2, #0x8a
	lsls r2, r2, #1
	adds r1, r0, r2
	str r3, [r1]
	adds r5, r0, r5
	ldr r2, [r5]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	adds r1, r7, #0
	adds r1, #0x3e
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r3, [r6]
	movs r1, #0xb4
	movs r2, #0x80
	adds r0, r3, r4
	str r1, [r0]
	adds r4, #4
	adds r0, r3, r4
	str r2, [r0]
	adds r1, r7, #0
	adds r1, #0x35
	adds r2, r7, #0
	adds r2, #0x4c
	adds r0, r7, #0
	bl sub_8005E5C
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08005994: .4byte gStaticData_0816B258
_08005998: .4byte gUnknown_030012DC

	thumb_func_start sub_800599C
sub_800599C: @ 0x0800599C
	push {r4, r5, lr}
	adds r5, r0, #0
	ldr r0, _080059DC @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_802332C
	adds r4, r0, #0
	lsls r0, r4, #3
	adds r0, r0, r4
	lsls r0, r0, #2
	ldr r1, _080059E0 @ =gStaticData_0816C86C
	adds r0, r0, r1
	ldr r0, [r0]
	bl sub_8026F38
	str r0, [r5, #0x70]
	cmp r4, #0x13
	bgt _080059E4
	movs r0, #0
	bl sub_8026F38
	str r0, [r5, #0x74]
	adds r1, r5, #0
	adds r1, #0x78
	movs r0, #0x20
	strb r0, [r1]
	adds r0, r4, #1
	adds r1, #1
	bl sub_80060AC
	b _080059E8
	.align 2, 0
_080059DC: .4byte gUnknown_030012C0
_080059E0: .4byte gStaticData_0816C86C
_080059E4:
	movs r0, #0
	str r0, [r5, #0x74]
_080059E8:
	ldr r0, [r5, #0x10]
	bl sub_800697C
	adds r4, r5, #0
	adds r4, #0x41
	adds r1, r4, #0
	bl sub_80060AC
	adds r2, r4, r0
	movs r3, #0
	movs r1, #0x25
	strb r1, [r2]
	adds r0, #1
	adds r4, r4, r0
	strb r3, [r4]
	ldr r4, _08005A74 @ =gUnknown_030012BC
	ldr r0, [r4]
	bl sub_8001AC0
	adds r0, #0xc
	lsls r1, r0, #2
	adds r1, r1, r0
	lsls r0, r1, #2
	cmp r0, #0
	bge _08005A1C
	adds r0, #0xff
_08005A1C:
	asrs r0, r0, #8
	str r0, [r5, #0x60]
	ldr r0, [r4]
	bl sub_8001ABC
	adds r0, #0xc
	lsls r1, r0, #2
	adds r1, r1, r0
	lsls r0, r1, #2
	cmp r0, #0
	bge _08005A34
	adds r0, #0xff
_08005A34:
	asrs r0, r0, #8
	str r0, [r5, #0x64]
	ldr r1, [r5, #0x60]
	adds r2, r5, #0
	adds r2, #0x57
	adds r0, r5, #0
	bl sub_80060F8
	ldr r1, [r5, #0x64]
	adds r2, r5, #0
	adds r2, #0x4f
	adds r0, r5, #0
	bl sub_80060F8
	adds r0, r5, #0
	bl sub_8005A78
	adds r0, r5, #0
	bl sub_8005AE8
	adds r0, r5, #0
	bl sub_8005B80
	adds r0, r5, #0
	bl sub_8005C58
	adds r0, r5, #0
	bl sub_8005D44
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08005A74: .4byte gUnknown_030012BC

	thumb_func_start sub_8005A78
sub_8005A78: @ 0x08005A78
	push {r4, r5, lr}
	adds r5, r0, #0
	adds r4, r5, #0
	adds r4, #0x88
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	str r0, [r4]
	ldr r1, _08005AE0 @ =gUnknown_030012D0
	ldr r1, [r1]
	ldr r1, [r1]
	ldr r1, [r1]
	movs r2, #0xde
	lsls r2, r2, #1
	adds r1, r1, r2
	str r1, [r0, #0x20]
	ldr r2, _08005AE4 @ =gStaticData_0816B1E4
	ldr r1, [r2]
	ldr r2, [r2, #4]
	bl sub_800737C
	ldr r0, [r4]
	bl sub_800815C
	ldr r2, [r4]
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldr r0, [r5, #0x10]
	bl sub_800695C
	adds r1, r5, #0
	adds r1, #0x2c
	bl sub_80060AC
	adds r5, #0x46
	movs r0, #0x14
	adds r1, r5, #0
	bl sub_80060AC
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08005AE0: .4byte gUnknown_030012D0
_08005AE4: .4byte gStaticData_0816B1E4

	thumb_func_start sub_8005AE8
sub_8005AE8: @ 0x08005AE8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r0
	movs r7, #0
_08005AF2:
	lsls r5, r7, #2
	mov r6, r8
	adds r6, #0x8c
	adds r6, r6, r5
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	adds r4, r0, #0
	str r4, [r6]
	ldr r0, _08005B74 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xe4
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	ldr r0, _08005B78 @ =gStaticData_0816B20C
	adds r5, r5, r0
	ldr r0, [r5]
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
	lsls r2, r7, #3
	ldr r1, _08005B7C @ =gStaticData_0816B1EC
	adds r2, r2, r1
	ldr r1, [r2]
	ldr r2, [r2, #4]
	bl sub_800737C
	ldr r0, [r6]
	bl sub_800815C
	ldr r2, [r6]
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r3, #0x10
	rsbs r3, r3, #0
	adds r1, r3, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	adds r7, #1
	cmp r7, #3
	ble _08005AF2
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08005B74: .4byte gUnknown_030012D0
_08005B78: .4byte gStaticData_0816B20C
_08005B7C: .4byte gStaticData_0816B1EC

	thumb_func_start sub_8005B80
sub_8005B80: @ 0x08005B80
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	movs r0, #0
	mov r8, r0
_08005B8C:
	mov r1, r8
	lsls r5, r1, #2
	adds r6, r7, #0
	adds r6, #0x9c
	adds r6, r6, r5
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	adds r4, r0, #0
	str r4, [r6]
	ldr r0, _08005C4C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r3, #0xc0
	lsls r3, r3, #1
	adds r0, r0, r3
	str r0, [r4, #0x20]
	ldr r0, _08005C50 @ =gStaticData_0816B244
	adds r5, r5, r0
	ldr r0, [r5]
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
	mov r1, r8
	lsls r2, r1, #3
	ldr r1, _08005C54 @ =gStaticData_0816B21C
	adds r2, r2, r1
	ldr r1, [r2]
	ldr r2, [r2, #4]
	bl sub_800737C
	ldr r0, [r6]
	bl sub_800815C
	ldr r2, [r6]
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r3, #0x10
	rsbs r3, r3, #0
	adds r1, r3, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldr r1, [r6]
	movs r0, #0x80
	strh r0, [r1, #0x3c]
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #4
	ble _08005B8C
	ldr r0, [r7, #0x10]
	bl sub_8006920
	adds r4, r0, #0
	ldr r0, [r7, #0x10]
	bl sub_80068CC
	adds r5, r0, #0
	adds r1, r7, #0
	adds r1, #0x2f
	adds r0, r4, #0
	bl sub_80060AC
	adds r1, r7, #0
	adds r1, #0x32
	adds r0, r5, #0
	bl sub_80060AC
	adds r1, r7, #0
	adds r1, #0x49
	movs r0, #0x1c
	bl sub_80060AC
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08005C4C: .4byte gUnknown_030012D0
_08005C50: .4byte gStaticData_0816B244
_08005C54: .4byte gStaticData_0816B21C

	thumb_func_start sub_8005C58
sub_8005C58: @ 0x08005C58
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	movs r0, #0
	mov r8, r0
_08005C64:
	mov r1, r8
	lsls r5, r1, #2
	adds r6, r7, #0
	adds r6, #0xb0
	adds r6, r6, r5
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	adds r4, r0, #0
	str r4, [r6]
	ldr r0, _08005D38 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r3, #0xc6
	lsls r3, r3, #1
	adds r0, r0, r3
	str r0, [r4, #0x20]
	ldr r0, _08005D3C @ =gStaticData_0816B270
	adds r5, r5, r0
	ldr r0, [r5]
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
	mov r1, r8
	lsls r2, r1, #3
	ldr r1, _08005D40 @ =gStaticData_0816B258
	adds r2, r2, r1
	ldr r1, [r2]
	ldr r2, [r2, #4]
	bl sub_800737C
	ldr r0, [r6]
	bl sub_800815C
	ldr r2, [r6]
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r3, #0x10
	rsbs r3, r3, #0
	adds r1, r3, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldr r1, [r6]
	movs r0, #0x80
	strh r0, [r1, #0x3c]
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #2
	ble _08005C64
	ldr r0, [r7, #0x10]
	bl sub_8006864
	adds r1, r7, #0
	adds r1, #0x38
	bl sub_80060AC
	ldr r0, [r7, #0x10]
	bl sub_8006820
	adds r1, r7, #0
	adds r1, #0x3b
	bl sub_80060AC
	ldr r0, [r7, #0x10]
	bl sub_80067EC
	adds r1, r7, #0
	adds r1, #0x3e
	bl sub_80060AC
	ldr r0, [r7, #0x10]
	bl sub_80068A8
	adds r1, r7, #0
	adds r1, #0x35
	bl sub_80060AC
	adds r1, r7, #0
	adds r1, #0x4c
	movs r0, #0x14
	bl sub_80060AC
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08005D38: .4byte gUnknown_030012D0
_08005D3C: .4byte gStaticData_0816B270
_08005D40: .4byte gStaticData_0816B258

	thumb_func_start sub_8005D44
sub_8005D44: @ 0x08005D44
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r0, _08005E48 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_802332C
	adds r4, r0, #0
	lsls r1, r4, #2
	adds r1, #4
	ldr r0, [r6, #0x10]
	adds r0, r0, r1
	ldr r0, [r0]
	lsls r0, r0, #0x10
	lsrs r5, r0, #0x13
	adds r1, r6, #0
	adds r1, #0x7c
	adds r0, r5, #0
	bl sub_800106C
	lsls r0, r4, #3
	adds r0, r0, r4
	lsls r0, r0, #2
	ldr r1, _08005E4C @ =gStaticData_0816C86C
	adds r7, r0, r1
	movs r1, #0
	cmp r5, #0
	beq _08005D82
	ldr r0, [r7, #8]
	cmp r5, r0
	bhi _08005D82
	movs r1, #1
_08005D82:
	adds r0, r6, #0
	adds r0, #0x6c
	strb r1, [r0]
	adds r6, #0xbc
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	str r0, [r6]
	ldr r1, _08005E50 @ =gUnknown_030012D0
	ldr r1, [r1]
	ldr r1, [r1]
	ldr r1, [r1]
	movs r2, #0xc6
	lsls r2, r2, #1
	adds r1, r1, r2
	str r1, [r0, #0x20]
	ldr r2, _08005E54 @ =gStaticData_0816B27C
	ldr r1, [r2]
	ldr r2, [r2, #4]
	bl sub_800737C
	cmp r5, #0
	beq _08005E40
	ldr r0, [r7, #8]
	cmp r5, r0
	bhi _08005DDA
	ldr r0, _08005E58 @ =gStaticData_0816B270
	ldr r4, [r6]
	ldr r0, [r0, #8]
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
_08005DDA:
	ldr r0, [r7, #0xc]
	cmp r5, r0
	bhi _08005E00
	ldr r0, _08005E58 @ =gStaticData_0816B270
	ldr r4, [r6]
	ldr r0, [r0, #4]
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
_08005E00:
	ldr r0, [r7, #0x10]
	cmp r5, r0
	bhi _08005E26
	ldr r0, _08005E58 @ =gStaticData_0816B270
	ldr r4, [r6]
	ldr r0, [r0]
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
_08005E26:
	ldr r0, [r6]
	bl sub_800815C
	ldr r2, [r6]
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
_08005E40:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08005E48: .4byte gUnknown_030012C0
_08005E4C: .4byte gStaticData_0816C86C
_08005E50: .4byte gUnknown_030012D0
_08005E54: .4byte gStaticData_0816B27C
_08005E58: .4byte gStaticData_0816B270

	thumb_func_start sub_8005E5C
sub_8005E5C: @ 0x08005E5C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r2
	ldr r5, _08005EEC @ =gUnknown_030012DC
	ldr r0, [r5]
	movs r4, #0x98
	lsls r4, r4, #1
	adds r2, r0, r4
	ldr r3, [r2]
	movs r6, #0x20
	ldrsh r2, [r3, r6]
	adds r0, r0, r2
	ldr r2, [r3, #0x24]
	bl sub_803AD80
	ldr r1, [r5]
	movs r7, #0x88
	lsls r7, r7, #1
	adds r0, r1, r7
	ldr r2, [r0]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r0, r1, r3
	ldr r3, [r0]
	ldr r6, _08005EF0 @ =gUnknown_030012E0
	ldr r0, [r6]
	subs r2, #2
	adds r1, r0, r7
	str r2, [r1]
	movs r2, #0x8a
	lsls r2, r2, #1
	adds r1, r0, r2
	str r3, [r1]
	adds r1, r0, r4
	ldr r2, [r1]
	movs r3, #0x30
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x34]
	movs r1, #0x2f
	bl sub_803AD80
	ldr r1, [r6]
	adds r6, r7, #0
	adds r0, r1, r6
	ldr r2, [r0]
	adds r7, #4
	adds r0, r1, r7
	ldr r3, [r0]
	ldr r0, [r5]
	subs r2, #5
	adds r3, #8
	adds r1, r0, r6
	str r2, [r1]
	adds r2, r7, #0
	adds r1, r0, r2
	str r3, [r1]
	adds r4, r0, r4
	ldr r2, [r4]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	mov r1, r8
	bl sub_803AD80
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08005EEC: .4byte gUnknown_030012DC
_08005EF0: .4byte gUnknown_030012E0

	thumb_func_start sub_8005EF4
sub_8005EF4: @ 0x08005EF4
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	ldr r0, [r6, #0x18]
	ldr r1, [r6, #0x14]
	lsls r0, r0, #3
	adds r0, r0, r1
	ldr r0, [r0, #4]
	cmp r0, #4
	beq _08005F0C
	cmp r0, #5
	beq _08005F5C
	b _08005FB0
_08005F0C:
	ldr r1, [r6, #0x60]
	cmp r1, #0
	beq _08005FB0
	subs r1, #1
	str r1, [r6, #0x60]
	adds r4, r6, #0
	adds r4, #0x57
	lsls r0, r1, #2
	adds r0, r0, r1
	movs r1, #0x20
	strb r1, [r4]
	movs r1, #0x3c
	strb r1, [r4, #1]
	adds r1, r6, #0
	adds r1, #0x59
	bl sub_80060AC
	adds r0, r0, r4
	movs r1, #0x25
	strb r1, [r0, #2]
	movs r1, #0x3e
	strb r1, [r0, #3]
	movs r1, #0
	strb r1, [r0, #4]
	ldr r0, _08005F58 @ =gUnknown_030012BC
	ldr r4, [r0]
	ldr r0, [r6, #0x60]
	lsls r0, r0, #8
	adds r0, #1
	movs r1, #0x14
	bl sub_803ADB4
	adds r1, r0, #0
	adds r0, r4, #0
	bl sub_8001B30
	b _08005FB0
	.align 2, 0
_08005F58: .4byte gUnknown_030012BC
_08005F5C:
	ldr r1, [r6, #0x64]
	cmp r1, #0
	beq _08005FB0
	subs r1, #1
	str r1, [r6, #0x64]
	adds r4, r6, #0
	adds r4, #0x4f
	lsls r0, r1, #2
	adds r0, r0, r1
	movs r1, #0x20
	strb r1, [r4]
	movs r1, #0x3c
	strb r1, [r4, #1]
	adds r1, r6, #0
	adds r1, #0x51
	bl sub_80060AC
	adds r0, r0, r4
	movs r1, #0x25
	strb r1, [r0, #2]
	movs r1, #0x3e
	strb r1, [r0, #3]
	movs r1, #0
	strb r1, [r0, #4]
	ldr r4, _08005FB8 @ =gUnknown_030012BC
	ldr r5, [r4]
	ldr r0, [r6, #0x64]
	lsls r0, r0, #8
	adds r0, #1
	movs r1, #0x14
	bl sub_803ADB4
	adds r1, r0, #0
	adds r0, r5, #0
	bl sub_8001B50
	ldr r0, [r4]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xe
	bl PlaySfx
_08005FB0:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08005FB8: .4byte gUnknown_030012BC

	thumb_func_start sub_8005FBC
sub_8005FBC: @ 0x08005FBC
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	ldr r0, [r6, #0x18]
	ldr r1, [r6, #0x14]
	lsls r0, r0, #3
	adds r0, r0, r1
	ldr r0, [r0, #4]
	cmp r0, #4
	beq _08005FD4
	cmp r0, #5
	beq _08006024
	b _08006078
_08005FD4:
	ldr r1, [r6, #0x60]
	cmp r1, #0x13
	bgt _08006078
	adds r1, #1
	str r1, [r6, #0x60]
	adds r4, r6, #0
	adds r4, #0x57
	lsls r0, r1, #2
	adds r0, r0, r1
	movs r1, #0x20
	strb r1, [r4]
	movs r1, #0x3c
	strb r1, [r4, #1]
	adds r1, r6, #0
	adds r1, #0x59
	bl sub_80060AC
	adds r0, r0, r4
	movs r1, #0x25
	strb r1, [r0, #2]
	movs r1, #0x3e
	strb r1, [r0, #3]
	movs r1, #0
	strb r1, [r0, #4]
	ldr r0, _08006020 @ =gUnknown_030012BC
	ldr r4, [r0]
	ldr r0, [r6, #0x60]
	lsls r0, r0, #8
	adds r0, #1
	movs r1, #0x14
	bl sub_803ADB4
	adds r1, r0, #0
	adds r0, r4, #0
	bl sub_8001B30
	b _08006078
	.align 2, 0
_08006020: .4byte gUnknown_030012BC
_08006024:
	ldr r1, [r6, #0x64]
	cmp r1, #0x13
	bgt _08006078
	adds r1, #1
	str r1, [r6, #0x64]
	adds r4, r6, #0
	adds r4, #0x4f
	lsls r0, r1, #2
	adds r0, r0, r1
	movs r1, #0x20
	strb r1, [r4]
	movs r1, #0x3c
	strb r1, [r4, #1]
	adds r1, r6, #0
	adds r1, #0x51
	bl sub_80060AC
	adds r0, r0, r4
	movs r1, #0x25
	strb r1, [r0, #2]
	movs r1, #0x3e
	strb r1, [r0, #3]
	movs r1, #0
	strb r1, [r0, #4]
	ldr r4, _08006080 @ =gUnknown_030012BC
	ldr r5, [r4]
	ldr r0, [r6, #0x64]
	lsls r0, r0, #8
	adds r0, #1
	movs r1, #0x14
	bl sub_803ADB4
	adds r1, r0, #0
	adds r0, r5, #0
	bl sub_8001B50
	ldr r0, [r4]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xe
	bl PlaySfx
_08006078:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08006080: .4byte gUnknown_030012BC

	thumb_func_start sub_8006084
sub_8006084: @ 0x08006084
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x18]
	adds r0, #1
	str r0, [r4, #0x18]
	ldr r1, [r4, #0x1c]
	bl sub_803AE4C
	str r0, [r4, #0x18]
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_800609C
sub_800609C: @ 0x0800609C
	adds r1, r0, #0
	ldr r0, [r1, #0x18]
	cmp r0, #0
	bne _080060A6
	ldr r0, [r1, #0x1c]
_080060A6:
	subs r0, #1
	str r0, [r1, #0x18]
	bx lr

	thumb_func_start sub_80060AC
sub_80060AC: @ 0x080060AC
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	adds r7, r1, #0
	adds r5, r0, #0
	movs r6, #0
_080060B6:
	mov r0, sp
	adds r4, r0, r6
	adds r0, r5, #0
	movs r1, #0xa
	bl sub_803AE4C
	adds r0, #0x30
	strb r0, [r4]
	adds r0, r5, #0
	movs r1, #0xa
	bl sub_803ADB4
	adds r5, r0, #0
	adds r6, #1
	cmp r5, #0
	bne _080060B6
	movs r2, #0
_080060D8:
	subs r6, #1
	adds r0, r7, r6
	mov r3, sp
	adds r1, r3, r2
	ldrb r1, [r1]
	strb r1, [r0]
	adds r2, #1
	cmp r6, #0
	bne _080060D8
	adds r0, r7, r2
	strb r6, [r0]
	adds r0, r2, #0
	add sp, #0xc
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_80060F8
sub_80060F8: @ 0x080060F8
	push {r4, lr}
	adds r4, r2, #0
	lsls r0, r1, #2
	adds r0, r0, r1
	movs r1, #0x20
	strb r1, [r4]
	movs r1, #0x3c
	strb r1, [r4, #1]
	adds r1, r4, #2
	bl sub_80060AC
	adds r0, r0, r4
	movs r1, #0x25
	strb r1, [r0, #2]
	movs r1, #0x3e
	strb r1, [r0, #3]
	movs r1, #0
	strb r1, [r0, #4]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

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

	thumb_func_start sub_8006250
sub_8006250: @ 0x08006250
	push {r4, lr}
	adds r4, r0, #0
	bl sub_80006A8
	ldr r0, _0800629C @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _080062A0 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	movs r1, #0xa0
	lsls r1, r1, #0x13
	movs r0, #0
	strh r0, [r1]
	ldr r1, _080062A4 @ =0x04000050
	adds r0, r4, #0
	adds r0, #0xc8
	ldr r0, [r0]
	str r0, [r1]
	adds r1, #4
	adds r0, r4, #0
	adds r0, #0xcc
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	adds r0, r4, #0
	adds r0, #0xd0
	ldrh r0, [r0]
	strh r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0800629C: .4byte gUnknown_030012B8
_080062A0: .4byte gUnknown_03001300
_080062A4: .4byte 0x04000050

	thumb_func_start sub_80062A8
sub_80062A8: @ 0x080062A8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	mov sb, r0
	mov sl, r1
	adds r7, r2, #0
	movs r0, #0xc0
	lsls r0, r0, #0x18
	bl mem_free_bytes
	bl sub_80006A8
	movs r0, #0xa0
	lsls r0, r0, #0x13
	movs r1, #0
	strh r1, [r0]
	movs r0, #0x80
	lsls r0, r0, #0x13
	strh r1, [r0]
	ldr r1, _080063C8 @ =gUnknown_030012B8
	ldr r0, [r1]
	bl sub_8006EA8
	ldr r5, _080063CC @ =gUnknown_030012DC
	ldr r0, [r5]
	bl sub_8028A40
	ldr r2, _080063D0 @ =gUnknown_030012E0
	mov r8, r2
	ldr r0, [r2]
	bl sub_8028A40
	ldr r6, _080063D4 @ =gUnknown_030012FC
	ldr r0, [r6]
	movs r4, #0
	str r4, [r0, #8]
	bl sub_8006C4C
	ldr r0, [r6]
	bl sub_8006C4C
	ldr r0, [r5]
	movs r3, #0x84
	lsls r3, r3, #1
	adds r1, r0, r3
	str r4, [r1]
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
	movs r0, #0x2c
	bl sub_8026EDC
	adds r5, r0, #0
	mov r0, sb
	bl sub_8026F38
	adds r4, r0, #0
	mov r0, sl
	bl sub_8026F38
	adds r2, r0, #0
	adds r0, r5, #0
	adds r1, r4, #0
	adds r3, r7, #0
	bl sub_80063D8
	adds r4, r0, #0
	bl sub_8006518
	cmp r4, #0
	beq _080063A8
	adds r0, r4, #0
	movs r1, #3
	bl sub_8006770
_080063A8:
	ldr r1, _080063C8 @ =gUnknown_030012B8
	ldr r0, [r1]
	bl sub_8006EA8
	movs r0, #0xc0
	lsls r0, r0, #0x18
	bl mem_free_bytes
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080063C8: .4byte gUnknown_030012B8
_080063CC: .4byte gUnknown_030012DC
_080063D0: .4byte gUnknown_030012E0
_080063D4: .4byte gUnknown_030012FC

	thumb_func_start sub_80063D8
sub_80063D8: @ 0x080063D8
	push {r4, r5, r6, lr}
	mov r6, sl
	mov r5, sb
	mov r4, r8
	push {r4, r5, r6}
	sub sp, #4
	adds r5, r0, #0
	mov r8, r1
	mov sb, r2
	mov sl, r3
	movs r0, #3
	str r0, [sp]
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0x1f
	movs r3, #0
	bl sub_801E644
	movs r6, #0
	str r6, [r5, #0x20]
	adds r2, r5, #0
	adds r2, #0x20
	movs r0, #0xc0
	ldrb r1, [r2]
	orrs r0, r1
	movs r1, #0x20
	orrs r0, r1
	movs r3, #1
	orrs r0, r3
	movs r1, #2
	orrs r0, r1
	movs r1, #4
	orrs r0, r1
	movs r1, #8
	orrs r0, r1
	movs r4, #0x10
	orrs r0, r4
	strb r0, [r2]
	adds r2, #4
	movs r0, #0x20
	rsbs r0, r0, #0
	ldrb r1, [r2]
	ands r0, r1
	orrs r0, r4
	strb r0, [r2]
	ldr r1, _08006500 @ =0x04000050
	ldr r0, [r5, #0x20]
	str r0, [r1]
	adds r1, #4
	ldrb r2, [r2]
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	strh r6, [r5, #0x28]
	adds r2, r5, #0
	adds r2, #0x28
	movs r0, #0x40
	ldrb r1, [r2]
	orrs r0, r1
	movs r1, #8
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r2]
	adds r0, r5, #0
	adds r0, #0x29
	ldrb r1, [r0]
	orrs r3, r1
	orrs r3, r4
	strb r3, [r0]
	mov r3, r8
	str r3, [r5, #0x10]
	mov r0, sb
	str r0, [r5, #0x14]
	ldr r1, _08006504 @ =gStaticData_0816C484
	adds r0, r5, #0
	bl LoadGraphicsPackage
	str r6, [r5, #0x1c]
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	adds r4, r0, #0
	str r4, [r5, #0x18]
	ldr r0, _08006508 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xe4
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r3, sl
	strb r3, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r5, #0x18]
	movs r1, #0xf0
	lsls r1, r1, #7
	str r1, [r0]
	movs r1, #0xa0
	lsls r1, r1, #7
	str r1, [r0, #4]
	bl sub_800815C
	ldr r2, [r5, #0x18]
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	adds r0, r5, #0
	bl sub_801E640
	ldr r1, _0800650C @ =0x04000008
	strh r0, [r1]
	ldr r0, _08006510 @ =0x04000010
	str r6, [r0]
	ldr r0, _08006514 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0xf
	bl sub_8001B54
	adds r0, r5, #0
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08006500: .4byte 0x04000050
_08006504: .4byte gStaticData_0816C484
_08006508: .4byte gUnknown_030012D0
_0800650C: .4byte 0x04000008
_08006510: .4byte 0x04000010
_08006514: .4byte gUnknown_030012BC

	thumb_func_start sub_8006518
sub_8006518: @ 0x08006518
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #0x24
	movs r0, #0x1f
	ldrb r1, [r6]
	ands r0, r1
	cmp r0, #0
	beq _08006564
	movs r2, #0x20
	rsbs r2, r2, #0
	adds r7, r2, #0
_08006534:
	adds r4, r6, #0
	ldrb r2, [r6]
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1b
	subs r0, #1
	movs r1, #0x1f
	ands r0, r1
	ands r2, r7
	orrs r2, r0
	strb r2, [r6]
	adds r0, r5, #0
	bl sub_8006600
	adds r0, r5, #0
	bl sub_8006714
	adds r0, r5, #0
	bl sub_8006700
	movs r0, #0x1f
	ldrb r4, [r4]
	ands r0, r4
	cmp r0, #0
	bne _08006534
_08006564:
	adds r4, r5, #0
	adds r4, #0x24
	movs r0, #0x28
	adds r0, r0, r5
	mov r8, r0
_0800656E:
	adds r0, r5, #0
	bl sub_8006600
	adds r0, r5, #0
	bl sub_8006714
	adds r0, r5, #0
	bl sub_8006700
	ldr r0, _080065F8 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r1, _080065FC @ =gUnknown_030007E0
	movs r0, #8
	ldrh r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _0800656E
	adds r6, r4, #0
	movs r0, #0x1f
	ldrb r1, [r6]
	ands r0, r1
	cmp r0, #0x10
	beq _080065D6
	movs r2, #0x20
	rsbs r2, r2, #0
	adds r7, r2, #0
_080065A6:
	adds r4, r6, #0
	ldrb r2, [r6]
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1b
	adds r0, #1
	movs r1, #0x1f
	ands r0, r1
	ands r2, r7
	orrs r2, r0
	strb r2, [r6]
	adds r0, r5, #0
	bl sub_8006600
	adds r0, r5, #0
	bl sub_8006714
	adds r0, r5, #0
	bl sub_8006700
	movs r0, #0x1f
	ldrb r4, [r4]
	ands r0, r4
	cmp r0, #0x10
	bne _080065A6
_080065D6:
	movs r0, #0
	strh r0, [r5, #0x28]
	movs r0, #0x40
	mov r1, r8
	ldrb r1, [r1]
	orrs r0, r1
	mov r2, r8
	strb r0, [r2]
	adds r0, r5, #0
	bl sub_8006714
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080065F8: .4byte gUnknown_03001304
_080065FC: .4byte gUnknown_030007E0

@ sub_8006600 is reconstructed (but not yet byte-matching) as C in
@ src/oam_count.c, guarded by #if NON_MATCHING - this raw version is
@ only assembled for the default (matching) build. See docs/graphics.md,
@ "Parked, not matched: sub_8006600".
.if NON_MATCHING == 0
	thumb_func_start sub_8006600
sub_8006600: @ 0x08006600
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #0x10
	adds r4, r0, #0
	ldr r0, _080066F0 @ =gUnknown_03001300
	mov sb, r0
	ldr r0, [r0]
	bl sub_8006A90
	ldr r0, _080066F4 @ =gUnknown_030012FC
	ldr r0, [r0]
	bl sub_8006C28
	ldr r0, [r4, #0x18]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	ldr r1, _080066F8 @ =gUnknown_030012E0
	mov r8, r1
	ldr r0, [r1]
	movs r5, #0x98
	lsls r5, r5, #1
	adds r1, r0, r5
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r4, #0x10]
	ldr r2, [r2, #0x14]
	bl sub_803AD80
	movs r6, #0xf0
	subs r0, r6, r0
	lsrs r3, r0, #1
	mov r7, r8
	ldr r0, [r7]
	movs r2, #0x2d
	movs r7, #0x88
	lsls r7, r7, #1
	adds r1, r0, r7
	str r3, [r1]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r1, r0, r3
	str r2, [r1]
	adds r1, r0, r5
	ldr r2, [r1]
	movs r7, #0x20
	ldrsh r1, [r2, r7]
	adds r0, r0, r1
	ldr r1, [r4, #0x10]
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	mov r0, sp
	movs r1, #0x10
	movs r2, #0x6a
	bl sub_803AFE4
	mov r0, sp
	movs r1, #0xd0
	movs r2, #0x35
	bl sub_803AFDC
	ldr r0, [r4, #0x14]
	ldr r4, _080066FC @ =gUnknown_030012DC
	ldr r1, [r4]
	mov r2, sp
	movs r3, #0
	bl sub_8001214
	movs r0, #0x2e
	bl sub_8026F38
	mov r8, r0
	ldr r0, [r4]
	adds r1, r0, r5
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	mov r1, r8
	bl sub_803AD80
	subs r6, r6, r0
	lsrs r3, r6, #1
	ldr r0, [r4]
	movs r2, #0x90
	movs r4, #0x88
	lsls r4, r4, #1
	adds r1, r0, r4
	str r3, [r1]
	movs r7, #0x8a
	lsls r7, r7, #1
	adds r1, r0, r7
	str r2, [r1]
	adds r5, r0, r5
	ldr r2, [r5]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	mov r1, r8
	bl sub_803AD80
	mov r4, sb
	ldr r0, [r4]
	bl sub_8006A48
	add sp, #0x10
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080066F0: .4byte gUnknown_03001300
_080066F4: .4byte gUnknown_030012FC
_080066F8: .4byte gUnknown_030012E0
_080066FC: .4byte gUnknown_030012DC
.endif
