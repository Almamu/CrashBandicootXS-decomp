.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_800FF0C
sub_800FF0C: @ 0x0800FF0C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	ldr r4, [sp, #0x28]
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	mov sb, r0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	str r1, [sp]
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	str r2, [sp, #4]
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r4, r4, #0x18
	lsrs r7, r4, #0x18
	movs r0, #0x64
	bl sub_8026EDC
	adds r4, r0, #0
	bl sub_80084A4
	ldr r0, _0800FFC8 @ =gStaticData_087E4074
	str r0, [r4, #0x18]
	adds r1, r4, #0
	adds r1, #0x59
	movs r0, #0
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_800FEB0
	adds r5, r4, #0
	mov r0, sb
	strh r0, [r5, #8]
	cmp r7, #9
	bne _0800FF76
	ldr r0, _0800FFCC @ =0x0000FFFF
	cmp sb, r0
	beq _0800FF76
	ldr r0, _0800FFD0 @ =gUnknown_030012B4
	ldr r0, [r0]
	mov r1, sb
	bl sub_802599C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0800FF76
	movs r7, #0
_0800FF76:
	ldr r6, _0800FFD4 @ =gUnknown_030012C0
	ldr r1, [r6]
	adds r0, r1, #0
	adds r0, #0x8c
	ldrb r0, [r0]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	cmp r0, #0
	bne _08010018
	adds r0, r1, #0
	bl sub_80232E0
	adds r4, r0, #0
	ldr r0, [r6]
	bl sub_8023128
	cmp r4, r0
	blt _08010018
	cmp r7, #0xb
	bne _0800FFD8
	ldr r0, _0800FFD0 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	add r1, r8
	ldr r0, [r0, #0xc]
	ldrh r1, [r1]
	adds r0, r1, r0
	adds r2, r0, #0
	ldrb r1, [r2]
	movs r0, #0x40
	ands r0, r1
	cmp r0, #0
	bne _0800FFF6
	movs r0, #0x80
	ands r0, r1
	cmp r0, #0
	bne _08010008
	b _0801000C
	.align 2, 0
_0800FFC8: .4byte gStaticData_087E4074
_0800FFCC: .4byte 0x0000FFFF
_0800FFD0: .4byte gUnknown_030012B4
_0800FFD4: .4byte gUnknown_030012C0
_0800FFD8:
	cmp r7, #0xf
	bne _08010018
	ldr r0, _0800FFFC @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r0, r8
	ldr r1, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r1
	ldrb r1, [r2]
	movs r0, #0x40
	ands r0, r1
	cmp r0, #0
	beq _08010000
_0800FFF6:
	movs r7, #2
	b _08010018
	.align 2, 0
_0800FFFC: .4byte gUnknown_030012B4
_08010000:
	movs r0, #0x80
	ands r0, r1
	cmp r0, #0
	beq _0801000C
_08010008:
	movs r7, #1
	b _08010018
_0801000C:
	movs r0, #1
	ldrb r2, [r2, #1]
	ands r0, r2
	cmp r0, #0
	beq _08010018
	movs r7, #9
_08010018:
	movs r4, #0
	ldr r0, _0801003C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r3, #0xba
	lsls r3, r3, #1
	adds r0, r0, r3
	str r0, [r5, #0x20]
	subs r0, r7, #1
	cmp r0, #0xe
	bhi _08010096
	lsls r0, r0, #2
	ldr r1, _08010040 @ =_08010044
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801003C: .4byte gUnknown_030012D0
_08010040: .4byte _08010044
_08010044: @ jump table
	.4byte _08010080 @ case 0
	.4byte _08010096 @ case 1
	.4byte _08010084 @ case 2
	.4byte _08010096 @ case 3
	.4byte _08010096 @ case 4
	.4byte _08010096 @ case 5
	.4byte _08010096 @ case 6
	.4byte _08010096 @ case 7
	.4byte _08010080 @ case 8
	.4byte _08010096 @ case 9
	.4byte _08010080 @ case 10
	.4byte _08010080 @ case 11
	.4byte _08010096 @ case 12
	.4byte _08010096 @ case 13
	.4byte _08010080 @ case 14
_08010080:
	movs r4, #1
	b _08010096
_08010084:
	ldr r0, _080100C4 @ =gUnknown_030012B4
	ldr r0, [r0]
	mov r1, sb
	bl sub_802599C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08010096
	movs r7, #7
_08010096:
	movs r6, #0
	ldr r2, _080100C4 @ =gUnknown_030012B4
	ldr r0, [r2]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r0, r8
	ldr r1, [r1, #0xc]
	ldrh r0, [r0]
	adds r1, r0, r1
	adds r3, r1, #0
	cmp r4, #0
	bne _080100B8
	movs r0, #0x20
	ldrb r4, [r1]
	ands r0, r4
	cmp r0, #0
	beq _080100E2
_080100B8:
	movs r6, #1
	ldrh r1, [r1, #4]
	cmp r1, #0x1b
	bne _080100C8
	movs r0, #0x15
	b _080100CC
	.align 2, 0
_080100C4: .4byte gUnknown_030012B4
_080100C8:
	movs r1, #4
	ldrsh r0, [r3, r1]
_080100CC:
	str r0, [r5, #0x54]
	ldr r0, _080100F4 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _080100E2
	ldr r0, [r5, #0x54]
	subs r0, #0x15
	lsls r0, r0, #0x18
	lsrs r7, r0, #0x18
_080100E2:
	cmp r7, #0x12
	bls _080100E8
	b _08010372
_080100E8:
	lsls r0, r7, #2
	ldr r1, _080100F8 @ =_080100FC
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080100F4: .4byte gUnknown_030012C0
_080100F8: .4byte _080100FC
_080100FC: @ jump table
	.4byte _08010148 @ case 0
	.4byte _0801014C @ case 1
	.4byte _0801016C @ case 2
	.4byte _08010170 @ case 3
	.4byte _080101B4 @ case 4
	.4byte _080101B8 @ case 5
	.4byte _080101FA @ case 6
	.4byte _080101FE @ case 7
	.4byte _08010202 @ case 8
	.4byte _08010206 @ case 9
	.4byte _08010244 @ case 10
	.4byte _08010248 @ case 11
	.4byte _0801027C @ case 12
	.4byte _08010286 @ case 13
	.4byte _0801028A @ case 14
	.4byte _0801028E @ case 15
	.4byte _08010334 @ case 16
	.4byte _08010338 @ case 17
	.4byte _08010356 @ case 18
_08010148:
	movs r0, #0x1f
	b _0801033A
_0801014C:
	ldr r0, [r2]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	add r1, r8
	ldr r0, [r0, #0xc]
	ldrh r1, [r1]
	adds r0, r1, r0
	ldrb r0, [r0]
	lsls r0, r0, #0x19
	lsrs r0, r0, #0x1f
	adds r1, r5, #0
	adds r1, #0x50
	strb r0, [r1]
	movs r0, #0x1a
	subs r1, #0x23
	b _0801033E
_0801016C:
	movs r0, #0x17
	b _0801033A
_08010170:
	ldr r0, [r2]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r0, r8
	ldr r4, [r1, #0xc]
	ldrh r0, [r0]
	adds r4, r0, r4
	movs r0, #3
	adds r1, r5, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r5, #0
	bl sub_80087C0
	adds r0, r5, #0
	bl sub_80087B4
	adds r0, r5, #0
	movs r1, #0
	bl sub_800872C
	ldrb r1, [r4, #6]
	adds r0, r5, #0
	adds r0, #0x50
	strb r1, [r0]
	ldrb r0, [r4, #7]
	adds r1, r5, #0
	adds r1, #0x51
	strb r0, [r1]
	ldrb r1, [r4, #8]
	adds r0, r5, #0
	adds r0, #0x4c
	strb r1, [r0]
	b _08010372
_080101B4:
	movs r0, #0x18
	b _0801033A
_080101B8:
	ldr r0, [r2]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r0, r8
	ldr r4, [r1, #0xc]
	ldrh r0, [r0]
	adds r4, r0, r4
	movs r0, #0x15
	adds r1, r5, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r5, #0
	bl sub_80087C0
	adds r0, r5, #0
	bl sub_80087B4
	adds r0, r5, #0
	movs r1, #0
	bl sub_800872C
	ldrb r1, [r4, #6]
	adds r0, r5, #0
	adds r0, #0x50
	strb r1, [r0]
	ldrb r0, [r4, #7]
	adds r1, r5, #0
	adds r1, #0x51
	strb r0, [r1]
	movs r2, #8
	ldrsh r0, [r4, r2]
	str r0, [r5, #0x48]
	b _08010372
_080101FA:
	movs r0, #4
	b _0801033A
_080101FE:
	movs r0, #0x20
	b _0801033A
_08010202:
	movs r0, #2
	b _0801033A
_08010206:
	movs r0, #0x1c
	adds r1, r5, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r5, #0
	bl sub_80087C0
	adds r0, r5, #0
	bl sub_80087B4
	adds r0, r5, #0
	movs r1, #0
	bl sub_800872C
	cmp r6, #0
	beq _08010228
	b _08010372
_08010228:
	movs r0, #0x15
	str r0, [r5, #0x54]
	ldr r0, _08010240 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0801023A
	b _08010372
_0801023A:
	movs r7, #0
	b _08010372
	.align 2, 0
_08010240: .4byte gUnknown_030012C0
_08010244:
	movs r0, #5
	b _0801033A
_08010248:
	ldr r0, [r2]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r0, r8
	ldr r4, [r1, #0xc]
	ldrh r0, [r0]
	adds r4, r0, r4
	movs r0, #0
	adds r1, r5, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r5, #0
	bl sub_80087C0
	adds r0, r5, #0
	bl sub_80087B4
	adds r0, r5, #0
	movs r1, #0
	bl sub_800872C
	ldrb r0, [r4, #6]
	adds r1, r5, #0
	adds r1, #0x51
	strb r0, [r1]
	b _08010372
_0801027C:
	movs r0, #0x2a
	rsbs r0, r0, #0
	str r0, [r5, #0x48]
	movs r0, #0x19
	b _0801033A
_08010286:
	movs r0, #6
	b _0801033A
_0801028A:
	movs r0, #0x11
	b _0801033A
_0801028E:
	ldr r0, [r2]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r0, r8
	ldr r1, [r1, #0xc]
	ldrh r0, [r0]
	adds r6, r0, r1
	mov sl, r6
	ldr r0, [r5, #0x20]
	ldr r1, [r0]
	adds r1, #0xe0
	ldr r0, _0801032C @ =gUnknown_030012B8
	ldr r0, [r0]
	ldrb r1, [r1, #0x14]
	bl sub_8006DF8
	ldr r0, [r5, #0x48]
	movs r1, #0x3f
	ands r0, r1
	movs r1, #0xf8
	ands r0, r1
	str r0, [r5, #0x48]
	movs r0, #7
	adds r1, r5, #0
	adds r1, #0x2d
	movs r4, #0
	strb r0, [r1]
	adds r0, r5, #0
	bl sub_80087C0
	adds r0, r5, #0
	bl sub_80087B4
	adds r0, r5, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r5, #0x48]
	movs r1, #0x38
	ands r0, r1
	lsrs r0, r0, #3
	ldr r1, _08010330 @ =gStaticData_0816BB94
	adds r0, r0, r1
	ldrb r0, [r0]
	adds r1, r5, #0
	adds r1, #0x4f
	strb r0, [r1]
	ldrb r0, [r6, #6]
	adds r1, #2
	strb r0, [r1]
	subs r1, #1
	strb r4, [r1]
	movs r0, #2
	ldrb r3, [r6, #1]
	ands r0, r3
	cmp r0, #0
	beq _08010304
	movs r0, #1
	strb r0, [r1]
_08010304:
	movs r0, #4
	ldrb r6, [r6, #1]
	ands r0, r6
	cmp r0, #0
	beq _08010316
	movs r0, #2
	ldrb r4, [r1]
	orrs r0, r4
	strb r0, [r1]
_08010316:
	movs r0, #8
	mov r2, sl
	ldrb r2, [r2, #1]
	ands r0, r2
	cmp r0, #0
	beq _08010372
	movs r0, #4
	ldrb r3, [r1]
	orrs r0, r3
	strb r0, [r1]
	b _08010372
	.align 2, 0
_0801032C: .4byte gUnknown_030012B8
_08010330: .4byte gStaticData_0816BB94
_08010334:
	movs r0, #0xe
	b _0801033A
_08010338:
	movs r0, #0xf
_0801033A:
	adds r1, r5, #0
	adds r1, #0x2d
_0801033E:
	strb r0, [r1]
	adds r0, r5, #0
	bl sub_80087C0
	adds r0, r5, #0
	bl sub_80087B4
	adds r0, r5, #0
	movs r1, #0
	bl sub_800872C
	b _08010372
_08010356:
	movs r0, #0x10
	adds r1, r5, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r5, #0
	bl sub_80087C0
	adds r0, r5, #0
	bl sub_80087B4
	adds r0, r5, #0
	movs r1, #0
	bl sub_800872C
_08010372:
	adds r2, r5, #0
	adds r2, #0x28
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r4, [r2]
	ands r0, r4
	movs r1, #0x21
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r2]
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
	ldr r4, [sp]
	lsls r0, r4, #8
	str r0, [r5]
	ldr r1, [sp, #4]
	lsls r0, r1, #8
	str r0, [r5, #4]
	ldr r4, _08010474 @ =gUnknown_030012B4
	ldr r0, [r4]
	mov r1, sb
	bl sub_802599C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _080103DE
	cmp r7, #0xb
	beq _080103C4
	cmp r7, #0xf
	bne _080103DE
_080103C4:
	ldr r0, [r4]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r0, r8
	ldr r1, [r1, #0xc]
	ldrh r0, [r0]
	adds r1, r0, r1
	movs r0, #0x80
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _080103DE
	movs r7, #1
_080103DE:
	cmp r7, #1
	bne _08010436
	ldr r0, _08010478 @ =0x0000FFFF
	cmp sb, r0
	beq _08010436
	ldr r0, _08010474 @ =gUnknown_030012B4
	ldr r0, [r0]
	mov r1, sb
	bl sub_802599C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08010436
	movs r0, #0x1b
	adds r4, r5, #0
	adds r4, #0x2d
	strb r0, [r4]
	adds r0, r5, #0
	bl sub_80087C0
	adds r0, r5, #0
	bl sub_80087B4
	adds r0, r5, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r5, #0x20]
	ldr r1, [r0]
	ldrb r2, [r4]
	lsls r0, r2, #3
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	subs r0, #1
	str r0, [r5, #0x30]
	adds r1, r5, #0
	adds r1, #0x4d
	movs r0, #0x80
	ldrb r3, [r1]
	ands r0, r3
	orrs r0, r7
	strb r0, [r1]
_08010436:
	adds r0, r5, #0
	adds r0, #0x4e
	strb r7, [r0]
	cmp r7, #5
	bne _08010456
	ldr r0, _08010474 @ =gUnknown_030012B4
	ldr r0, [r0]
	mov r1, sb
	bl sub_802599C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08010456
	adds r0, r5, #0
	bl sub_800F5B8
_08010456:
	ldr r0, _0801047C @ =gUnknown_0300130C
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8009B70
	adds r0, r5, #0
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08010474: .4byte gUnknown_030012B4
_08010478: .4byte 0x0000FFFF
_0801047C: .4byte gUnknown_0300130C
