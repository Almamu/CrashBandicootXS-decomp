.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_801CEE0
sub_801CEE0: @ 0x0801CEE0
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r4, r0, #0
	b _0801D036
_0801CEEC:
	adds r0, r4, #0
	bl sub_801C104
	bl sub_80006A8
	ldr r0, _0801CFD4 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801CFD8 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r4, #0x20]
	bl sub_801DCBC
	ldr r0, [r4, #0x7c]
	adds r0, #1
	str r0, [r4, #0x7c]
	ldr r1, _0801CFDC @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801CFE0 @ =0x04000014
	str r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801E640
	ldr r1, _0801CFE4 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r4, #0x20]
	bl sub_801DE24
	ldr r1, _0801CFE8 @ =0x0400000C
	strh r0, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	strh r5, [r0]
	adds r1, #0x44
	adds r0, r4, #0
	adds r0, #0xa0
	ldr r0, [r0]
	str r0, [r1]
	adds r1, #4
	adds r0, r4, #0
	adds r0, #0xa4
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	adds r0, r4, #0
	adds r0, #0xa8
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801D7AC
	ldr r0, [r4, #0x3c]
	bl sub_801E190
	ldr r0, [r4, #0x1c]
	bl sub_801D77C
	movs r1, #0xff
	ands r1, r0
	cmp r1, #0xa0
	bne _0801D036
	movs r5, #0
	adds r7, r4, #0
	adds r7, #0x24
	movs r0, #0x9c
	adds r0, r0, r4
	mov r8, r0
	adds r6, r7, #0
_0801CF8A:
	ldm r6!, {r0}
	ldr r2, [r0, #0x10]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r4, #0xc]
	ldr r3, [r2, #0x14]
	adds r2, r5, #0
	bl sub_803AD84
	adds r5, #1
	cmp r5, #5
	ble _0801CF8A
	movs r3, #0
	movs r2, #0
	ldr r0, [r4, #0xc]
	lsls r1, r0, #2
	adds r1, r1, r0
	mov r5, r8
	ldr r0, [r5]
	lsls r1, r1, #2
	adds r1, r1, r0
_0801CFB6:
	ldrb r5, [r1, #4]
	lsls r0, r5, #0x1f
	lsrs r0, r0, #0x1f
	adds r3, r3, r0
	adds r1, #4
	adds r2, #1
	cmp r2, #4
	ble _0801CFB6
	cmp r3, #5
	bne _0801CFF0
	ldr r0, _0801CFEC @ =gStaticData_0816C508
	str r0, [r4, #0x18]
	str r3, [r4, #4]
	b _0801CFF8
	.align 2, 0
_0801CFD4: .4byte gUnknown_030012B8
_0801CFD8: .4byte gUnknown_03001300
_0801CFDC: .4byte 0x04000010
_0801CFE0: .4byte 0x04000014
_0801CFE4: .4byte 0x0400000A
_0801CFE8: .4byte 0x0400000C
_0801CFEC: .4byte gStaticData_0816C508
_0801CFF0:
	ldr r0, _0801D054 @ =gStaticData_0816C4D8
	str r0, [r4, #0x18]
	movs r0, #4
	str r0, [r4, #4]
_0801CFF8:
	movs r5, #0
	adds r6, r7, #0
_0801CFFC:
	ldm r6!, {r0}
	ldr r3, [r0, #0x10]
	movs r2, #0x18
	ldrsh r1, [r3, r2]
	adds r0, r0, r1
	lsls r2, r5, #3
	ldr r1, [r4, #0x18]
	adds r1, r1, r2
	ldr r2, [r3, #0x1c]
	bl sub_803AD80
	adds r5, #1
	cmp r5, #5
	ble _0801CFFC
	ldr r2, _0801D058 @ =gStaticData_0816C538
	adds r6, r7, #0
	movs r5, #5
_0801D01E:
	ldm r6!, {r0}
	ldr r1, [r4, #0xc]
	lsls r1, r1, #2
	adds r1, r1, r2
	ldr r1, [r1]
	str r2, [sp]
	bl sub_801DF0C
	subs r5, #1
	ldr r2, [sp]
	cmp r5, #0
	bge _0801D01E
_0801D036:
	ldr r0, [r4, #0x1c]
	bl sub_801D780
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	cmp r5, #0
	bne _0801D046
	b _0801CEEC
_0801D046:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801D054: .4byte gStaticData_0816C4D8
_0801D058: .4byte gStaticData_0816C538

	thumb_func_start sub_801D05C
sub_801D05C: @ 0x0801D05C
	push {r4, r5, lr}
	adds r4, r0, #0
	b _0801D0E4
_0801D062:
	adds r0, r4, #0
	bl sub_801C104
	bl sub_80006A8
	ldr r0, _0801D0F8 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801D0FC @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r4, #0x20]
	bl sub_801DCBC
	ldr r0, [r4, #0x7c]
	adds r0, #1
	str r0, [r4, #0x7c]
	ldr r1, _0801D100 @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801D104 @ =0x04000014
	str r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801E640
	ldr r1, _0801D108 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r4, #0x20]
	bl sub_801DE24
	ldr r1, _0801D10C @ =0x0400000C
	strh r0, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	strh r5, [r0]
	adds r1, #0x44
	adds r0, r4, #0
	adds r0, #0xa0
	ldr r0, [r0]
	str r0, [r1]
	adds r1, #4
	adds r0, r4, #0
	adds r0, #0xa4
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	adds r0, r4, #0
	adds r0, #0xa8
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, [r4, #0x3c]
	bl sub_801E190
	ldr r0, [r4, #0x20]
	bl sub_801DAD8
_0801D0E4:
	ldr r0, [r4, #0x3c]
	bl sub_801E464
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	cmp r5, #0
	beq _0801D062
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0801D0F8: .4byte gUnknown_030012B8
_0801D0FC: .4byte gUnknown_03001300
_0801D100: .4byte 0x04000010
_0801D104: .4byte 0x04000014
_0801D108: .4byte 0x0400000A
_0801D10C: .4byte 0x0400000C

	thumb_func_start sub_801D110
sub_801D110: @ 0x0801D110
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	ldr r0, _0801D150 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x52
	bl PlaySfx
	ldr r1, [r6, #8]
	lsls r1, r1, #2
	adds r0, r6, #0
	adds r0, #0x24
	adds r0, r0, r1
	ldr r0, [r0]
	movs r1, #0
	bl sub_801DEA0
	ldr r0, [r6, #0x3c]
	movs r1, #0x78
	movs r2, #0x35
	bl sub_801E480
	ldr r0, [r6, #0x3c]
	bl sub_801E3F4
	adds r0, r6, #0
	bl sub_801D05C
	b _0801D1D6
	.align 2, 0
_0801D150: .4byte gUnknown_030012BC
_0801D154:
	adds r0, r6, #0
	bl sub_801C104
	bl sub_80006A8
	ldr r0, _0801D224 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801D228 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r6, #0x20]
	bl sub_801DCBC
	ldr r0, [r6, #0x7c]
	adds r0, #1
	str r0, [r6, #0x7c]
	ldr r1, _0801D22C @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r6, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801D230 @ =0x04000014
	str r0, [r1]
	ldr r0, [r6, #0x1c]
	bl sub_801E640
	ldr r1, _0801D234 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r6, #0x20]
	bl sub_801DE24
	ldr r1, _0801D238 @ =0x0400000C
	strh r0, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	strh r4, [r0]
	adds r1, #0x44
	adds r0, r6, #0
	adds r0, #0xa0
	ldr r0, [r0]
	str r0, [r1]
	adds r1, #4
	adds r0, r6, #0
	adds r0, #0xa4
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	adds r0, r6, #0
	adds r0, #0xa8
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, [r6, #0x3c]
	bl sub_801E190
	ldr r0, [r6, #0x20]
	bl sub_801DAD8
_0801D1D6:
	ldr r0, [r6, #0x20]
	bl sub_801DD18
	lsls r0, r0, #0x18
	lsrs r4, r0, #0x18
	cmp r4, #0
	beq _0801D154
	adds r5, r6, #0
	adds r5, #0xa0
	movs r0, #0xc0
	ldrb r1, [r5]
	orrs r0, r1
	movs r1, #0x20
	orrs r0, r1
	movs r1, #1
	orrs r0, r1
	movs r1, #2
	orrs r0, r1
	movs r1, #4
	orrs r0, r1
	movs r1, #8
	orrs r0, r1
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r5]
	adds r4, r6, #0
	adds r4, #0xa4
	movs r0, #0x20
	rsbs r0, r0, #0
	ldrb r2, [r4]
	ands r0, r2
	strb r0, [r4]
	movs r7, #0
	ldr r0, [r6, #0x20]
	bl sub_801DD48
	mov r8, r5
	b _0801D2D0
	.align 2, 0
_0801D224: .4byte gUnknown_030012B8
_0801D228: .4byte gUnknown_03001300
_0801D22C: .4byte 0x04000010
_0801D230: .4byte 0x04000014
_0801D234: .4byte 0x0400000A
_0801D238: .4byte 0x0400000C
_0801D23C:
	adds r0, r6, #0
	bl sub_801C104
	bl sub_80006A8
	ldr r0, _0801D2E8 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801D2EC @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r6, #0x20]
	bl sub_801DCBC
	ldr r0, [r6, #0x7c]
	adds r0, #1
	str r0, [r6, #0x7c]
	ldr r1, _0801D2F0 @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r6, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801D2F4 @ =0x04000014
	str r0, [r1]
	ldr r0, [r6, #0x1c]
	bl sub_801E640
	ldr r1, _0801D2F8 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r6, #0x20]
	bl sub_801DE24
	ldr r1, _0801D2FC @ =0x0400000C
	strh r0, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	strh r5, [r0]
	adds r1, #0x44
	mov r2, r8
	ldr r0, [r2]
	str r0, [r1]
	adds r1, #4
	ldrb r2, [r4]
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	adds r0, r6, #0
	adds r0, #0xa8
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, [r6, #0x3c]
	bl sub_801E190
	ldr r0, [r6, #0x20]
	bl sub_801DAD8
	adds r7, #1
	lsrs r1, r7, #0x1f
	adds r1, r7, r1
	asrs r1, r1, #1
	movs r0, #0x1f
	ands r1, r0
	movs r0, #0x20
	rsbs r0, r0, #0
	ldrb r2, [r4]
	ands r0, r2
	orrs r0, r1
	strb r0, [r4]
_0801D2D0:
	ldr r0, [r6, #0x20]
	bl sub_801DD08
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	cmp r5, #0
	beq _0801D23C
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801D2E8: .4byte gUnknown_030012B8
_0801D2EC: .4byte gUnknown_03001300
_0801D2F0: .4byte 0x04000010
_0801D2F4: .4byte 0x04000014
_0801D2F8: .4byte 0x0400000A
_0801D2FC: .4byte 0x0400000C

	thumb_func_start sub_801D300
sub_801D300: @ 0x0801D300
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	ldr r0, _0801D350 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x49
	bl PlaySfx
	adds r2, r5, #0
	adds r2, #0xa0
	movs r0, #0xc0
	ldrb r1, [r2]
	orrs r0, r1
	movs r1, #0x20
	orrs r0, r1
	movs r1, #1
	orrs r0, r1
	movs r1, #2
	orrs r0, r1
	movs r1, #4
	orrs r0, r1
	movs r1, #8
	orrs r0, r1
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r2]
	adds r4, r5, #0
	adds r4, #0xa4
	movs r0, #0x20
	rsbs r0, r0, #0
	ldrb r2, [r4]
	ands r0, r2
	strb r0, [r4]
	movs r7, #0
	ldr r0, [r5, #0x20]
	bl sub_801DD5C
	adds r6, r4, #0
	b _0801D3EC
	.align 2, 0
_0801D350: .4byte gUnknown_030012BC
_0801D354:
	adds r0, r5, #0
	bl sub_801C104
	bl sub_80006A8
	ldr r0, _0801D404 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801D408 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r5, #0x20]
	bl sub_801DCBC
	ldr r0, [r5, #0x7c]
	adds r0, #1
	str r0, [r5, #0x7c]
	ldr r1, _0801D40C @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r5, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801D410 @ =0x04000014
	str r0, [r1]
	ldr r0, [r5, #0x1c]
	bl sub_801E640
	ldr r1, _0801D414 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r5, #0x20]
	bl sub_801DE24
	ldr r1, _0801D418 @ =0x0400000C
	strh r0, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	strh r4, [r0]
	adds r1, #0x44
	adds r0, r5, #0
	adds r0, #0xa0
	ldr r0, [r0]
	str r0, [r1]
	adds r1, #4
	ldrb r2, [r6]
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	adds r0, r5, #0
	adds r0, #0xa8
	ldrh r0, [r0]
	strh r0, [r1]
	ldr r0, [r5, #0x3c]
	bl sub_801E190
	ldr r0, [r5, #0x20]
	bl sub_801DAD8
	adds r7, #1
	lsrs r1, r7, #0x1f
	adds r1, r7, r1
	asrs r1, r1, #1
	movs r0, #0x1f
	ands r1, r0
	movs r2, #0x20
	rsbs r2, r2, #0
	adds r0, r2, #0
	ldrb r2, [r6]
	ands r0, r2
	orrs r0, r1
	strb r0, [r6]
_0801D3EC:
	ldr r0, [r5, #0x20]
	bl sub_801DD28
	lsls r0, r0, #0x18
	lsrs r4, r0, #0x18
	cmp r4, #0
	beq _0801D354
	movs r0, #1
	strb r0, [r5]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801D404: .4byte gUnknown_030012B8
_0801D408: .4byte gUnknown_03001300
_0801D40C: .4byte 0x04000010
_0801D410: .4byte 0x04000014
_0801D414: .4byte 0x0400000A
_0801D418: .4byte 0x0400000C

	thumb_func_start sub_801D41C
sub_801D41C: @ 0x0801D41C
	ldr r1, _0801D424 @ =gUnknown_03000824
	movs r0, #1
	strb r0, [r1]
	bx lr
	.align 2, 0
_0801D424: .4byte gUnknown_03000824

	thumb_func_start sub_801D428
sub_801D428: @ 0x0801D428
	ldr r1, [r0, #0xc]
	rsbs r0, r1, #0
	orrs r0, r1
	lsrs r0, r0, #0x1f
	bx lr
	.align 2, 0

	thumb_func_start sub_801D434
sub_801D434: @ 0x0801D434
	movs r2, #0
	ldr r1, [r0, #0xc]
	cmp r1, #1
	beq _0801D456
	cmp r1, #1
	bgt _0801D446
	cmp r1, #0
	beq _0801D44C
	b _0801D46C
_0801D446:
	cmp r1, #2
	beq _0801D460
	b _0801D46C
_0801D44C:
	adds r0, #0x9c
	ldr r0, [r0]
	ldrb r0, [r0, #2]
	lsrs r2, r0, #5
	b _0801D468
_0801D456:
	adds r0, #0x9c
	ldr r0, [r0]
	ldrb r0, [r0, #2]
	lsrs r2, r0, #7
	b _0801D46C
_0801D460:
	adds r0, #0x9c
	ldr r0, [r0]
	ldrb r0, [r0, #2]
	lsrs r2, r0, #6
_0801D468:
	movs r0, #1
	ands r2, r0
_0801D46C:
	adds r0, r2, #0
	bx lr

	thumb_func_start sub_801D470
sub_801D470: @ 0x0801D470
	push {r4, r5, lr}
	adds r5, r0, #0
	ldr r1, _0801D4C0 @ =gStaticData_0816C548
	ldr r0, [r5, #0xc]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r4, [r5, #0x40]
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
	ldr r0, [r5, #8]
	ldr r1, [r5, #4]
	cmp r0, r1
	ble _0801D4A4
	str r1, [r5, #8]
_0801D4A4:
	ldr r0, [r5, #8]
	lsls r0, r0, #3
	ldr r2, [r5, #0x18]
	adds r2, r2, r0
	ldr r0, [r5, #0x3c]
	ldr r1, [r2]
	ldr r2, [r2, #4]
	subs r2, #0x18
	bl sub_801E480
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0801D4C0: .4byte gStaticData_0816C548

	thumb_func_start sub_801D4C4
sub_801D4C4: @ 0x0801D4C4
	push {r4, lr}
	adds r4, r0, #0
	bl sub_801D428
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801D530
	adds r0, r4, #0
	bl sub_801CCF8
	ldr r0, _0801D4E8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x56
	bl PlaySfx
	b _0801D512
	.align 2, 0
_0801D4E8: .4byte gUnknown_030012BC
_0801D4EC:
	ldr r0, [r4, #0xc]
	subs r0, #1
	str r0, [r4, #0xc]
	ldr r0, [r4, #0x1c]
	bl sub_801D790
	adds r0, r4, #0
	bl sub_801CEE0
	ldr r0, _0801D528 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r0, _0801D52C @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #0x80
	ands r0, r1
	cmp r0, #0
	beq _0801D51E
_0801D512:
	adds r0, r4, #0
	bl sub_801D428
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801D4EC
_0801D51E:
	adds r0, r4, #0
	bl sub_801D470
	b _0801D53E
	.align 2, 0
_0801D528: .4byte gUnknown_03001304
_0801D52C: .4byte gUnknown_030007E0
_0801D530:
	ldr r0, _0801D544 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x48
	bl PlaySfx
_0801D53E:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0801D544: .4byte gUnknown_030012BC

	thumb_func_start sub_801D548
sub_801D548: @ 0x0801D548
	push {r4, lr}
	adds r4, r0, #0
	bl sub_801D434
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801D5B4
	adds r0, r4, #0
	bl sub_801CCF8
	ldr r0, _0801D56C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x55
	bl PlaySfx
	b _0801D596
	.align 2, 0
_0801D56C: .4byte gUnknown_030012BC
_0801D570:
	ldr r0, [r4, #0xc]
	adds r0, #1
	str r0, [r4, #0xc]
	ldr r0, [r4, #0x1c]
	bl sub_801D79C
	adds r0, r4, #0
	bl sub_801CEE0
	ldr r0, _0801D5AC @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r0, _0801D5B0 @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #0x40
	ands r0, r1
	cmp r0, #0
	beq _0801D5A2
_0801D596:
	adds r0, r4, #0
	bl sub_801D434
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801D570
_0801D5A2:
	adds r0, r4, #0
	bl sub_801D470
	b _0801D5C2
	.align 2, 0
_0801D5AC: .4byte gUnknown_03001304
_0801D5B0: .4byte gUnknown_030007E0
_0801D5B4:
	ldr r0, _0801D5C8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x48
	bl PlaySfx
_0801D5C2:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0801D5C8: .4byte gUnknown_030012BC

	thumb_func_start sub_801D5CC
sub_801D5CC: @ 0x0801D5CC
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	movs r3, #0
	movs r2, #0
	ldr r1, [r5, #0xc]
	lsls r0, r1, #2
	adds r0, r0, r1
	adds r1, r5, #0
	adds r1, #0x9c
	ldr r1, [r1]
	lsls r0, r0, #2
	adds r1, r0, r1
_0801D5E4:
	ldrb r4, [r1, #4]
	lsls r0, r4, #0x1f
	lsrs r0, r0, #0x1f
	adds r3, r3, r0
	adds r1, #4
	adds r2, #1
	cmp r2, #4
	ble _0801D5E4
	cmp r3, #5
	bne _0801D604
	ldr r0, _0801D600 @ =gStaticData_0816C508
	str r0, [r5, #0x18]
	str r3, [r5, #4]
	b _0801D60C
	.align 2, 0
_0801D600: .4byte gStaticData_0816C508
_0801D604:
	ldr r0, _0801D634 @ =gStaticData_0816C4D8
	str r0, [r5, #0x18]
	movs r0, #4
	str r0, [r5, #4]
_0801D60C:
	movs r4, #0
	adds r6, r5, #0
	adds r6, #0x24
_0801D612:
	ldm r6!, {r0}
	ldr r3, [r0, #0x10]
	movs r2, #0x18
	ldrsh r1, [r3, r2]
	adds r0, r0, r1
	lsls r2, r4, #3
	ldr r1, [r5, #0x18]
	adds r1, r1, r2
	ldr r2, [r3, #0x1c]
	bl sub_803AD80
	adds r4, #1
	cmp r4, #5
	ble _0801D612
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801D634: .4byte gStaticData_0816C4D8

	thumb_func_start sub_801D638
sub_801D638: @ 0x0801D638
	push {r4, r5, lr}
	adds r5, r0, #0
	movs r4, #0
_0801D63E:
	lsls r1, r4, #2
	adds r0, r5, #0
	adds r0, #0x24
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r2, [r0, #0x10]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r5, #0xc]
	ldr r3, [r2, #0x14]
	adds r2, r4, #0
	bl sub_803AD84
	adds r4, #1
	cmp r4, #5
	ble _0801D63E
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801D668
sub_801D668: @ 0x0801D668
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	movs r4, #0
	ldr r6, _0801D694 @ =gStaticData_0816C538
_0801D670:
	lsls r1, r4, #2
	adds r0, r5, #0
	adds r0, #0x24
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r5, #0xc]
	lsls r1, r1, #2
	adds r1, r1, r6
	ldr r1, [r1]
	bl sub_801DF0C
	adds r4, #1
	cmp r4, #5
	ble _0801D670
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0801D694: .4byte gStaticData_0816C538

	thumb_func_start sub_801D698
sub_801D698: @ 0x0801D698
	push {r4, lr}
	adds r4, r0, #0
	bl sub_80006A8
	ldr r0, _0801D714 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _0801D718 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	ldr r0, [r4, #0x20]
	bl sub_801DCBC
	ldr r0, [r4, #0x7c]
	adds r0, #1
	str r0, [r4, #0x7c]
	ldr r1, _0801D71C @ =0x04000010
	lsrs r0, r0, #3
	strh r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801D7D0
	ldr r1, _0801D720 @ =0x04000014
	str r0, [r1]
	ldr r0, [r4, #0x1c]
	bl sub_801E640
	ldr r1, _0801D724 @ =0x0400000A
	strh r0, [r1]
	ldr r0, [r4, #0x20]
	bl sub_801DE24
	ldr r1, _0801D728 @ =0x0400000C
	strh r0, [r1]
	movs r1, #0xa0
	lsls r1, r1, #0x13
	movs r0, #0
	strh r0, [r1]
	ldr r1, _0801D72C @ =0x04000050
	adds r0, r4, #0
	adds r0, #0xa0
	ldr r0, [r0]
	str r0, [r1]
	adds r1, #4
	adds r0, r4, #0
	adds r0, #0xa4
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	lsrs r0, r0, #0x1b
	strh r0, [r1]
	subs r1, #0x54
	adds r0, r4, #0
	adds r0, #0xa8
	ldrh r0, [r0]
	strh r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0801D714: .4byte gUnknown_030012B8
_0801D718: .4byte gUnknown_03001300
_0801D71C: .4byte 0x04000010
_0801D720: .4byte 0x04000014
_0801D724: .4byte 0x0400000A
_0801D728: .4byte 0x0400000C
_0801D72C: .4byte 0x04000050

	thumb_func_start sub_801D730
sub_801D730: @ 0x0801D730
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r0, _0801D778 @ =gUnknown_030012B8
	ldr r0, [r0]
	movs r1, #0xf
	bl sub_8006D50
	movs r5, #0
	movs r0, #0x10
	rsbs r0, r0, #0
	adds r7, r0, #0
_0801D746:
	lsls r0, r5, #2
	adds r4, r6, #0
	adds r4, #0x40
	adds r4, r4, r0
	ldr r0, [r4]
	bl sub_800815C
	ldr r2, [r4]
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	ldrb r1, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	adds r5, #1
	cmp r5, #7
	ble _0801D746
	adds r0, r6, #0
	bl sub_801D668
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801D778: .4byte gUnknown_030012B8

	thumb_func_start sub_801D77C
sub_801D77C: @ 0x0801D77C
	ldr r0, [r0, #0x10]
	bx lr

	thumb_func_start sub_801D780
sub_801D780: @ 0x0801D780
	movs r2, #0
	ldr r1, [r0, #0x10]
	ldr r0, [r0, #0x14]
	cmp r1, r0
	bne _0801D78C
	movs r2, #1
_0801D78C:
	adds r0, r2, #0
	bx lr

	thumb_func_start sub_801D790
sub_801D790: @ 0x0801D790
	ldr r1, [r0, #0x14]
	movs r2, #0x80
	lsls r2, r2, #1
	adds r1, r1, r2
	str r1, [r0, #0x14]
	bx lr

	thumb_func_start sub_801D79C
sub_801D79C: @ 0x0801D79C
	ldr r1, [r0, #0x14]
	ldr r2, _0801D7A8 @ =0xFFFFFF00
	adds r1, r1, r2
	str r1, [r0, #0x14]
	bx lr
	.align 2, 0
_0801D7A8: .4byte 0xFFFFFF00

	thumb_func_start sub_801D7AC
sub_801D7AC: @ 0x0801D7AC
	adds r1, r0, #0
	ldr r2, [r1, #0x10]
	ldr r0, [r1, #0x14]
	cmp r2, r0
	bge _0801D7BC
	adds r0, r2, #0
	adds r0, #8
	str r0, [r1, #0x10]
_0801D7BC:
	ldr r2, [r1, #0x10]
	ldr r0, [r1, #0x14]
	cmp r2, r0
	ble _0801D7CA
	adds r0, r2, #0
	subs r0, #8
	str r0, [r1, #0x10]
_0801D7CA:
	ldr r0, [r1, #0x10]
	strh r0, [r1, #0x26]
	bx lr

	thumb_func_start sub_801D7D0
sub_801D7D0: @ 0x0801D7D0
	ldr r0, [r0, #0x24]
	bx lr

	thumb_func_start sub_801D7D4
sub_801D7D4: @ 0x0801D7D4
	movs r1, #8
	strh r1, [r0, #0x24]
	ldrh r1, [r0, #0x10]
	adds r1, #0x30
	strh r1, [r0, #0x26]
	bx lr

	thumb_func_start sub_801D7E0
sub_801D7E0: @ 0x0801D7E0
	push {lr}
	adds r2, r0, #0
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _0801D7F2
	adds r0, r2, #0
	bl sub_8026ED0
_0801D7F2:
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801D7F8
sub_801D7F8: @ 0x0801D7F8
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	movs r0, #2
	str r0, [sp]
	adds r0, r4, #0
	movs r3, #0
	bl sub_801E644
	movs r0, #0xc0
	lsls r0, r0, #2
	str r0, [r4, #0x14]
	str r0, [r4, #0x10]
	ldr r1, _0801D824 @ =gStaticData_0816C58C
	adds r0, r4, #0
	bl LoadGraphicsPackage
	adds r0, r4, #0
	add sp, #4
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801D824: .4byte gStaticData_0816C58C

	thumb_func_start sub_801D828
sub_801D828: @ 0x0801D828
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xc
	adds r6, r0, #0
	movs r0, #0x78
	str r0, [r6, #0x20]
	movs r0, #0x35
	str r0, [r6, #0x24]
	movs r5, #0
	strh r5, [r6, #0x34]
	adds r4, r6, #0
	adds r4, #0x34
	subs r0, #0x39
	ldrb r3, [r4]
	ands r0, r3
	movs r3, #1
	orrs r0, r3
	strb r0, [r4]
	str r1, [r6, #0x18]
	movs r0, #3
	ands r1, r0
	lsls r1, r1, #2
	movs r0, #0xd
	rsbs r0, r0, #0
	ldrb r3, [r4]
	ands r0, r3
	orrs r0, r1
	strb r0, [r4]
	adds r1, r6, #0
	adds r1, #0x35
	movs r0, #0x3f
	ldrb r3, [r1]
	ands r0, r3
	strb r0, [r1]
	str r2, [r6, #0x1c]
	movs r0, #0x1f
	ands r2, r0
	movs r0, #0x20
	rsbs r0, r0, #0
	ldrb r3, [r1]
	ands r0, r3
	orrs r0, r2
	strb r0, [r1]
	movs r0, #0x80
	ldrb r1, [r4]
	orrs r0, r1
	strb r0, [r4]
	ldr r0, [r6, #0x1c]
	lsls r0, r0, #0xb
	movs r2, #0xc0
	lsls r2, r2, #0x13
	adds r3, r0, r2
	mov r0, sp
	strh r5, [r0]
	ldr r0, _0801DA20 @ =0x040000D4
	mov r4, sp
	str r4, [r0]
	str r3, [r0, #4]
	ldr r1, _0801DA24 @ =0x81000080
	str r1, [r0, #8]
	ldr r0, [r0, #8]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r0, #0
	ldr r1, [r6, #0x20]
	mov sl, r1
	movs r4, #0x40
	adds r4, r4, r6
	mov r8, r4
	movs r1, #0x42
	adds r1, r1, r6
	mov ip, r1
	movs r4, #0x48
	adds r4, r4, r6
	mov sb, r4
	adds r1, r6, #0
	adds r1, #0x64
	str r1, [sp, #4]
	adds r4, r6, #0
	adds r4, #0x88
	str r4, [sp, #8]
	ldr r7, _0801DA28 @ =0x00000202
_0801D8D2:
	adds r5, r3, #0
	adds r5, #0x10
	adds r4, r0, #1
	adds r0, r3, #0
	movs r1, #3
_0801D8DC:
	strh r2, [r0]
	adds r2, r2, r7
	adds r0, #2
	subs r1, #1
	cmp r1, #0
	bge _0801D8DC
	adds r3, r5, #0
	adds r0, r4, #0
	cmp r0, #7
	ble _0801D8D2
	movs r0, #0xb
	str r0, [r6, #0x10]
	movs r0, #2
	str r0, [r6, #0xc]
	movs r0, #8
	str r0, [r6, #0x14]
	movs r1, #0
	str r1, [r6, #0x30]
	str r1, [r6, #0x2c]
	str r1, [r6, #0x28]
	movs r0, #0x80
	lsls r0, r0, #6
	str r0, [r6, #0x38]
	str r0, [r6, #0x3c]
	mov r2, sl
	mov r0, r8
	strh r2, [r0]
	ldr r0, [r6, #0x24]
	mov r3, ip
	strh r0, [r3]
	mov r4, sb
	strh r1, [r4]
	adds r5, r6, #0
	adds r5, #0x5c
	ldr r4, [sp, #4]
	ldr r7, _0801DA2C @ =gStaticData_0816C5F0
	movs r0, #1
	mov sb, r0
	movs r1, #3
	mov r8, r1
_0801D92C:
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	str r0, [r4]
	ldr r1, _0801DA30 @ =gUnknown_030012D0
	ldr r1, [r1]
	ldr r1, [r1]
	ldr r1, [r1]
	movs r2, #0x96
	lsls r2, r2, #2
	adds r1, r1, r2
	str r1, [r0, #0x20]
	adds r0, #0x28
	movs r3, #4
	rsbs r3, r3, #0
	adds r1, r3, #0
	ldrb r2, [r0]
	ands r1, r2
	mov r3, sb
	orrs r1, r3
	strb r1, [r0]
	ldr r0, [r4]
	ldr r3, [r6, #0x20]
	ldr r1, [r7]
	adds r3, r3, r1
	ldr r2, [r6, #0x24]
	ldr r1, [r7, #4]
	adds r2, r2, r1
	lsls r3, r3, #8
	str r3, [r0]
	lsls r2, r2, #8
	str r2, [r0, #4]
	movs r1, #1
	bl sub_80088D8
	ldr r0, [r6, #0x64]
	bl sub_800815C
	ldr r2, [r4]
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
	adds r0, r6, #0
	adds r1, r5, #0
	bl sub_801DDB4
	adds r5, #0xc
	adds r4, #0xc
	adds r7, #8
	movs r0, #1
	rsbs r0, r0, #0
	add r8, r0
	mov r1, r8
	cmp r1, #0
	bge _0801D92C
	ldr r0, _0801DA34 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r2, [r6, #0x64]
	ldr r1, [r2, #0x20]
	adds r2, #0x2d
	ldr r3, [r1]
	ldrb r4, [r2]
	lsls r1, r4, #3
	subs r1, r1, r4
	lsls r1, r1, #2
	adds r1, r1, r3
	ldrb r1, [r1, #0x14]
	bl sub_8006D84
	ldr r1, [r6, #0x70]
	adds r1, #0x28
	movs r3, #0x11
	rsbs r3, r3, #0
	adds r0, r3, #0
	ldrb r2, [r1]
	ands r0, r2
	movs r5, #0x10
	orrs r0, r5
	strb r0, [r1]
	ldr r2, [r6, #0x7c]
	adds r2, #0x28
	movs r1, #0x21
	rsbs r1, r1, #0
	adds r0, r1, #0
	ldrb r4, [r2]
	ands r0, r4
	movs r4, #0x20
	orrs r0, r4
	strb r0, [r2]
	ldr r2, [sp, #8]
	ldr r0, [r2]
	adds r0, #0x28
	ldrb r2, [r0]
	ands r3, r2
	orrs r3, r5
	strb r3, [r0]
	ldr r3, [sp, #8]
	ldr r0, [r3]
	adds r0, #0x28
	ldrb r2, [r0]
	ands r1, r2
	orrs r1, r4
	strb r1, [r0]
	adds r0, r6, #0
	add sp, #0xc
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0801DA20: .4byte 0x040000D4
_0801DA24: .4byte 0x81000080
_0801DA28: .4byte 0x00000202
_0801DA2C: .4byte gStaticData_0816C5F0
_0801DA30: .4byte gUnknown_030012D0
_0801DA34: .4byte gUnknown_030012B8
