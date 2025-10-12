.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_80001CC
sub_80001CC: @ 0x080001CC
	push {r4, r5, lr}
	sub sp, #8
	ldr r3, _08000254 @ =0x03001638
	ldr r4, _08000258 @ =0x03007E00
	subs r4, r4, r3
	subs r4, r4, r0
	movs r5, #0
	str r5, [sp]
	ldr r2, _0800025C @ =0x040000D4
	mov r0, sp
	str r0, [r2]
	str r3, [r2, #4]
	lsrs r0, r4, #2
	movs r1, #0x85
	lsls r1, r1, #0x18
	orrs r0, r1
	str r0, [r2, #8]
	ldr r0, [r2, #8]
	add r0, sp, #4
	strh r5, [r0]
	str r0, [r2]
	movs r1, #0x80
	lsls r1, r1, #0x12
	str r1, [r2, #4]
	ldr r0, _08000260 @ =0x81020000
	str r0, [r2, #8]
	ldr r0, [r2, #8]
	ldr r0, _08000264 @ =0x030007CC
	str r3, [r0]
	adds r0, r3, #0
	adds r0, #0x14
	str r0, [r3, #8]
	str r0, [r3, #0xc]
	movs r2, #1
	str r2, [r3, #4]
	str r5, [r3]
	str r0, [r3, #0x10]
	str r5, [r0, #4]
	str r3, [r0, #0xc]
	str r3, [r0, #8]
	subs r4, #0x14
	str r4, [r3, #0x14]
	ldr r0, _08000268 @ =0x030007D0
	str r1, [r0]
	adds r0, r1, #0
	adds r0, #0x14
	str r0, [r1, #8]
	str r0, [r1, #0xc]
	str r2, [r1, #4]
	str r5, [r1]
	str r0, [r1, #0x10]
	str r5, [r0, #4]
	str r1, [r0, #0xc]
	str r1, [r0, #8]
	ldr r0, _0800026C @ =0x0003FFEC
	str r0, [r1, #0x14]
	movs r0, #0xc0
	lsls r0, r0, #0x18
	bl sub_80003DC
	ldr r1, _08000270 @ =0x030007D4
	str r0, [r1]
	movs r0, #0
	add sp, #8
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08000254: .4byte 0x03001638
_08000258: .4byte 0x03007E00
_0800025C: .4byte 0x040000D4
_08000260: .4byte 0x81020000
_08000264: .4byte 0x030007CC
_08000268: .4byte 0x030007D0
_0800026C: .4byte 0x0003FFEC
_08000270: .4byte 0x030007D4

	thumb_func_start sub_8000274
sub_8000274: @ 0x08000274
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	cmp r7, #0
	bge _08000304
	ldr r0, _080002AC @ =0x030007CC
	ldr r5, [r0]
	ldr r1, [r5, #8]
	mov r8, r0
	cmp r1, r5
	beq _08000304
	ldr r0, _080002B0 @ =0x030007D0
	mov ip, r0
_08000290:
	ldr r6, [r1, #8]
	ldr r0, [r1, #4]
	cmp r0, #2
	bne _080002FE
	adds r0, r1, #0
	adds r0, #0x10
	cmp r0, #0
	beq _080002FE
	mov r1, r8
	ldr r2, [r1]
	cmp r0, r2
	blo _080002B4
	adds r4, r2, #0
	b _080002B8
	.align 2, 0
_080002AC: .4byte 0x030007CC
_080002B0: .4byte 0x030007D0
_080002B4:
	mov r1, ip
	ldr r4, [r1]
_080002B8:
	adds r3, r0, #0
	subs r3, #0x10
	movs r0, #0
	str r0, [r3, #4]
	ldr r2, [r3, #0xc]
	ldr r0, [r2, #4]
	cmp r0, #0
	bne _080002E0
	ldr r0, [r2]
	ldr r1, [r3]
	adds r0, r0, r1
	str r0, [r2]
	ldr r0, [r3, #8]
	str r0, [r2, #8]
	str r2, [r0, #0xc]
	ldr r0, [r4, #0x10]
	cmp r3, r0
	bne _080002DE
	str r2, [r4, #0x10]
_080002DE:
	adds r3, r2, #0
_080002E0:
	ldr r2, [r3, #8]
	ldr r0, [r2, #4]
	cmp r0, #0
	bne _080002FE
	ldr r0, [r3]
	ldr r1, [r2]
	adds r0, r0, r1
	str r0, [r3]
	ldr r0, [r2, #8]
	str r0, [r3, #8]
	str r3, [r0, #0xc]
	ldr r0, [r4, #0x10]
	cmp r2, r0
	bne _080002FE
	str r3, [r4, #0x10]
_080002FE:
	adds r1, r6, #0
	cmp r1, r5
	bne _08000290
_08000304:
	movs r0, #0x80
	lsls r0, r0, #0x17
	ands r0, r7
	cmp r0, #0
	beq _08000390
	ldr r0, _08000338 @ =0x030007D0
	ldr r5, [r0]
	ldr r1, [r5, #8]
	mov r8, r0
	cmp r1, r5
	beq _08000390
	ldr r7, _0800033C @ =0x030007CC
_0800031C:
	ldr r6, [r1, #8]
	ldr r0, [r1, #4]
	cmp r0, #2
	bne _0800038A
	adds r0, r1, #0
	adds r0, #0x10
	cmp r0, #0
	beq _0800038A
	ldr r2, [r7]
	cmp r0, r2
	blo _08000340
	adds r4, r2, #0
	b _08000344
	.align 2, 0
_08000338: .4byte 0x030007D0
_0800033C: .4byte 0x030007CC
_08000340:
	mov r1, r8
	ldr r4, [r1]
_08000344:
	adds r3, r0, #0
	subs r3, #0x10
	movs r0, #0
	str r0, [r3, #4]
	ldr r2, [r3, #0xc]
	ldr r0, [r2, #4]
	cmp r0, #0
	bne _0800036C
	ldr r0, [r2]
	ldr r1, [r3]
	adds r0, r0, r1
	str r0, [r2]
	ldr r0, [r3, #8]
	str r0, [r2, #8]
	str r2, [r0, #0xc]
	ldr r0, [r4, #0x10]
	cmp r3, r0
	bne _0800036A
	str r2, [r4, #0x10]
_0800036A:
	adds r3, r2, #0
_0800036C:
	ldr r2, [r3, #8]
	ldr r0, [r2, #4]
	cmp r0, #0
	bne _0800038A
	ldr r0, [r3]
	ldr r1, [r2]
	adds r0, r0, r1
	str r0, [r3]
	ldr r0, [r2, #8]
	str r0, [r3, #8]
	str r3, [r0, #0xc]
	ldr r0, [r4, #0x10]
	cmp r2, r0
	bne _0800038A
	str r3, [r4, #0x10]
_0800038A:
	adds r1, r6, #0
	cmp r1, r5
	bne _0800031C
_08000390:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800039C:
	.byte 0x03, 0x1C, 0x00, 0x2B
	.byte 0x0A, 0xDA, 0x02, 0x48, 0x02, 0x68, 0x91, 0x68, 0x03, 0xE0, 0x00, 0x00, 0xCC, 0x07, 0x00, 0x03
	.byte 0x89, 0x68, 0x88, 0x68, 0x90, 0x42, 0xFB, 0xD1, 0x80, 0x20, 0xC0, 0x05, 0x18, 0x40, 0x00, 0x28
	.byte 0x0A, 0xD0, 0x02, 0x48, 0x02, 0x68, 0x91, 0x68, 0x03, 0xE0, 0x00, 0x00, 0xD0, 0x07, 0x00, 0x03
	.byte 0x89, 0x68, 0x88, 0x68, 0x90, 0x42, 0xFB, 0xD1, 0x70, 0x47, 0x00, 0x00

	thumb_func_start sub_80003DC
sub_80003DC: @ 0x080003DC
	push {r4, r5, lr}
	adds r5, r0, #0
	movs r4, #0
	cmp r5, #0
	bge _08000406
	ldr r0, _08000438 @ =0x030007CC
	ldr r3, [r0]
	movs r1, #0
	ldr r2, [r3, #8]
	cmp r2, r3
	beq _08000404
_080003F2:
	ldr r0, [r2, #4]
	cmp r0, #0
	bne _080003FE
	subs r1, #0x10
	ldr r0, [r2]
	adds r1, r1, r0
_080003FE:
	ldr r2, [r2, #8]
	cmp r2, r3
	bne _080003F2
_08000404:
	adds r4, r4, r1
_08000406:
	movs r0, #0x80
	lsls r0, r0, #0x17
	ands r0, r5
	cmp r0, #0
	beq _08000430
	ldr r0, _0800043C @ =0x030007D0
	ldr r3, [r0]
	movs r1, #0
	ldr r2, [r3, #8]
	cmp r2, r3
	beq _0800042E
_0800041C:
	ldr r0, [r2, #4]
	cmp r0, #0
	bne _08000428
	subs r1, #0x10
	ldr r0, [r2]
	adds r1, r1, r0
_08000428:
	ldr r2, [r2, #8]
	cmp r2, r3
	bne _0800041C
_0800042E:
	adds r4, r4, r1
_08000430:
	adds r0, r4, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08000438: .4byte 0x030007CC
_0800043C: .4byte 0x030007D0

	thumb_func_start sub_8000440
sub_8000440: @ 0x08000440
	push {r4, lr}
	adds r3, r0, #0
	cmp r1, #0
	bge _08000454
	ldr r0, _0800044C @ =0x030007CC
	b _08000456
	.align 2, 0
_0800044C: .4byte 0x030007CC
_08000450:
	movs r0, #0
	b _080004A4
_08000454:
	ldr r0, _08000468 @ =0x030007D0
_08000456:
	ldr r4, [r0]
	adds r3, #0x13
	movs r0, #4
	rsbs r0, r0, #0
	ands r3, r0
	ldr r2, [r4, #0x10]
	ldr r1, [r2, #0xc]
	b _08000472
	.align 2, 0
_08000468: .4byte 0x030007D0
_0800046C:
	cmp r2, r1
	beq _08000450
	ldr r2, [r2, #8]
_08000472:
	ldr r0, [r2, #4]
	cmp r0, #0
	bne _0800046C
	ldr r0, [r2]
	cmp r0, r3
	blo _0800046C
	subs r1, r0, r3
	cmp r1, #0x40
	bls _08000498
	adds r0, r2, r3
	str r1, [r0]
	movs r1, #0
	str r1, [r0, #4]
	str r2, [r0, #0xc]
	ldr r1, [r2, #8]
	str r1, [r0, #8]
	str r0, [r1, #0xc]
	str r0, [r2, #8]
	str r3, [r2]
_08000498:
	movs r0, #2
	str r0, [r2, #4]
	ldr r0, [r2, #8]
	str r0, [r4, #0x10]
	adds r0, r2, #0
	adds r0, #0x10
_080004A4:
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_80004AC
sub_80004AC: @ 0x080004AC
	push {r4, lr}
	adds r1, r0, #0
	cmp r1, #0
	beq _0800050E
	ldr r0, _080004C0 @ =0x030007CC
	ldr r0, [r0]
	cmp r1, r0
	blo _080004C4
	adds r4, r0, #0
	b _080004C8
	.align 2, 0
_080004C0: .4byte 0x030007CC
_080004C4:
	ldr r0, _08000514 @ =0x030007D0
	ldr r4, [r0]
_080004C8:
	adds r3, r1, #0
	subs r3, #0x10
	movs r0, #0
	str r0, [r3, #4]
	ldr r2, [r3, #0xc]
	ldr r0, [r2, #4]
	cmp r0, #0
	bne _080004F0
	ldr r0, [r2]
	ldr r1, [r3]
	adds r0, r0, r1
	str r0, [r2]
	ldr r0, [r3, #8]
	str r0, [r2, #8]
	str r2, [r0, #0xc]
	ldr r0, [r4, #0x10]
	cmp r3, r0
	bne _080004EE
	str r2, [r4, #0x10]
_080004EE:
	adds r3, r2, #0
_080004F0:
	ldr r2, [r3, #8]
	ldr r0, [r2, #4]
	cmp r0, #0
	bne _0800050E
	ldr r0, [r3]
	ldr r1, [r2]
	adds r0, r0, r1
	str r0, [r3]
	ldr r0, [r2, #8]
	str r0, [r3, #8]
	str r3, [r0, #0xc]
	ldr r0, [r4, #0x10]
	cmp r2, r0
	bne _0800050E
	str r3, [r4, #0x10]
_0800050E:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08000514: .4byte 0x030007D0

	thumb_func_start sub_8000518
sub_8000518: @ 0x08000518
	push {r4, lr}
	movs r4, #0xc0
	lsls r4, r4, #0x18
	adds r0, r4, #0
	bl sub_80003DC
	ldr r1, _08000540 @ =0x030007D4
	ldr r1, [r1]
	cmp r1, r0
	beq _08000538
	adds r0, r4, #0
	bl sub_8000274
	adds r0, r4, #0
	bl sub_80003DC
_08000538:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08000540: .4byte 0x030007D4

	thumb_func_start sub_8000544
sub_8000544: @ 0x08000544
	ldr r1, _08000550 @ =0x030009E8
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r1, _08000554 @ =nullsub_10
	str r1, [r0]
	bx lr
	.align 2, 0
_08000550: .4byte 0x030009E8
_08000554: .4byte nullsub_10

	thumb_func_start sub_8000558
sub_8000558: @ 0x08000558
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	ldr r0, _0800058C @ =0x030009E8
	lsls r2, r5, #2
	adds r0, r2, r0
	ldr r1, _08000590 @ =0x03000A20
	adds r6, r2, r1
	ldr r1, [r6]
	str r1, [r0]
	cmp r1, #0
	bne _08000582
	ldr r3, _08000594 @ =0x04000208
	ldrh r2, [r3]
	strh r1, [r3]
	ldr r4, _08000598 @ =0x04000200
	movs r1, #1
	lsls r1, r5
	ldrh r0, [r4]
	bics r0, r1
	strh r0, [r4]
	strh r2, [r3]
_08000582:
	ldr r0, _0800059C @ =nullsub_10
	str r0, [r6]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0800058C: .4byte 0x030009E8
_08000590: .4byte 0x03000A20
_08000594: .4byte 0x04000208
_08000598: .4byte 0x04000200
_0800059C: .4byte nullsub_10

	thumb_func_start sub_80005A0
sub_80005A0: @ 0x080005A0
	push {r4, lr}
	ldr r4, _080005C4 @ =0x03000A20
	lsls r2, r0, #2
	adds r4, r2, r4
	ldr r3, _080005C8 @ =0x030009E8
	adds r2, r2, r3
	ldr r3, [r2]
	str r3, [r4]
	str r1, [r2]
	ldr r2, _080005CC @ =0x04000200
	movs r1, #1
	lsls r1, r0
	ldrh r0, [r2]
	orrs r1, r0
	strh r1, [r2]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080005C4: .4byte 0x03000A20
_080005C8: .4byte 0x030009E8
_080005CC: .4byte 0x04000200

	thumb_func_start sub_80005D0
sub_80005D0: @ 0x080005D0
	ldr r1, _080005D8 @ =0x04000208
	movs r0, #0
	strh r0, [r1]
	bx lr
	.align 2, 0
_080005D8: .4byte 0x04000208
