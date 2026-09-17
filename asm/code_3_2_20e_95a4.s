.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_80395A4
sub_80395A4: @ 0x080395A4
	push {r4, r5, r6, r7, lr}
	sub sp, #8
	adds r4, r0, #0
	adds r6, r1, #0
	adds r7, r2, #0
	ldr r0, [r4, #8]
	ldr r5, [r0]
	ldr r0, [r5]
	ldr r3, [r0, #8]
	adds r0, r5, #0
	bl sub_803AD84
	ldrb r0, [r5, #0x1b]
	cmp r0, #0
	beq _080395C6
	movs r0, #0
	str r0, [r4, #0x3c]
_080395C6:
	ldrb r0, [r5, #0x1a]
	cmp r0, #0
	beq _08039600
	ldrh r1, [r4, #0x34]
	movs r2, #0x34
	ldrsh r0, [r4, r2]
	cmp r0, #0
	beq _080395EA
	subs r0, r1, #1
	strh r0, [r4, #0x34]
	lsls r0, r0, #0x10
	cmp r0, #0
	bne _080395EA
	adds r0, r4, #0
	adds r1, r5, #0
	movs r2, #1
	bl sub_8039658
_080395EA:
	ldrh r0, [r5, #0x18]
	cmp r0, #0
	beq _08039600
	ldrb r0, [r5, #0x1d]
	cmp r0, #0
	beq _08039600
	adds r0, r4, #0
	adds r1, r5, #0
	movs r2, #0
	bl sub_8039658
_08039600:
	ldr r0, [r4, #0x3c]
	ldrb r1, [r4, #0x1f]
	cmp r0, #0
	beq _08039620
	cmp r1, #0
	bne _08039620
	ldrb r0, [r4, #0x1e]
	cmp r0, #0
	beq _08039624
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_80398DC
	ldrb r0, [r4, #0x1e]
	subs r0, #1
	b _08039622
_08039620:
	subs r0, r1, #1
_08039622:
	strb r0, [r4, #0x1f]
_08039624:
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_8039AA4
	ldrb r1, [r4, #0xc]
	cmp r1, #0
	bne _0803964C
	ldr r0, [r5]
	ldr r0, [r0, #0x18]
	str r0, [sp]
	str r1, [sp, #4]
	adds r0, r4, #0
	adds r1, r5, #0
	adds r2, r6, #0
	adds r3, r7, #0
	bl sub_8039B44
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	b _0803964E
_0803964C:
	movs r0, #0
_0803964E:
	add sp, #8
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8039658
sub_8039658: @ 0x08039658
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r4, r0, #0
	adds r7, r1, #0
	lsls r2, r2, #0x18
	movs r0, #0
	strh r0, [r4, #0x1a]
	strh r0, [r4, #0x28]
	strh r0, [r4, #0x34]
	cmp r2, #0
	bne _0803972C
	ldrb r0, [r7, #0x1e]
	cmp r0, #0
	beq _0803969C
	ldr r0, [r7]
	ldr r2, [r0, #0x18]
	ldr r1, [r4]
	movs r3, #0x14
	ldrsh r0, [r7, r3]
	ldr r1, [r1, #0x18]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrh r1, [r0]
	ldr r0, [r2, #0xc]
	adds r0, r0, r1
	str r0, [r4, #0x40]
	movs r1, #0
	strb r1, [r4, #0xf]
	ldrb r1, [r0]
	strb r1, [r4, #0xe]
	adds r0, #1
	str r0, [r4, #0x40]
_0803969C:
	ldrb r0, [r4, #0xe]
	cmp r0, #0
	beq _080396A4
	b _0803980A
_080396A4:
	ldrb r0, [r4, #0xf]
	cmp r0, #0
	beq _080396B0
	subs r0, #1
	strb r0, [r4, #0xf]
	b _0803980A
_080396B0:
	ldr r2, [r4, #0x40]
	ldrb r3, [r2]
	adds r0, r3, #0
	cmp r0, #0xff
	bne _080396C6
	ldrb r0, [r2, #1]
	subs r0, #1
	strb r0, [r4, #0xf]
	adds r0, r2, #2
	str r0, [r4, #0x40]
	b _0803980A
_080396C6:
	movs r0, #0x80
	ands r0, r3
	cmp r0, #0
	beq _080396FC
	movs r1, #0x7f
	ands r1, r3
	cmp r1, #0
	bne _080396DC
	adds r0, r2, #1
	str r0, [r4, #0x40]
	b _0803980A
_080396DC:
	cmp r1, #0x79
	bhi _080396EC
	mov r8, r1
	ldrb r6, [r2, #1]
	movs r5, #0
	mov sb, r5
	adds r0, r2, #2
	b _0803970A
_080396EC:
	movs r0, #0
	mov r8, r0
	movs r6, #0
	ldrb r1, [r2, #1]
	mov sb, r1
	ldrb r5, [r2, #2]
	adds r0, r2, #3
	b _0803970A
_080396FC:
	ldrb r3, [r2]
	mov r8, r3
	ldrb r6, [r2, #1]
	ldrb r0, [r2, #2]
	mov sb, r0
	ldrb r5, [r2, #3]
	adds r0, r2, #4
_0803970A:
	str r0, [r4, #0x40]
	mov r1, sb
	cmp r1, #0xe
	bne _0803973E
	lsrs r0, r5, #4
	cmp r0, #0xd
	bne _0803973E
	movs r0, #0xf
	ands r5, r0
	strh r5, [r4, #0x34]
	adds r0, r4, #0
	adds r0, #0x50
	mov r2, r8
	strb r2, [r0]
	adds r0, #1
	strb r6, [r0]
	b _0803980A
_0803972C:
	adds r0, r4, #0
	adds r0, #0x50
	ldrb r0, [r0]
	mov r8, r0
	adds r0, r4, #0
	adds r0, #0x51
	ldrb r6, [r0]
	movs r5, #0
	mov sb, r5
_0803973E:
	mov r3, sb
	cmp r3, #3
	beq _0803974C
	adds r0, r4, #0
	mov r1, r8
	bl sub_8039818
_0803974C:
	ldr r0, [r7]
	ldr r3, [r0, #0x18]
	adds r0, r4, #0
	adds r1, r7, #0
	adds r2, r6, #0
	bl sub_803985C
	mov r0, sb
	subs r0, #1
	cmp r0, #0xe
	bhi _0803980A
	lsls r0, r0, #2
	ldr r1, _0803976C @ =_08039770
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0803976C: .4byte _08039770
_08039770: @ jump table
	.4byte _080397AC @ case 0
	.4byte _080397B0 @ case 1
	.4byte _080397B6 @ case 2
	.4byte _0803980A @ case 3
	.4byte _0803980A @ case 4
	.4byte _0803980A @ case 5
	.4byte _080397D6 @ case 6
	.4byte _0803980A @ case 7
	.4byte _0803980A @ case 8
	.4byte _080397EA @ case 9
	.4byte _080397EE @ case 10
	.4byte _080397F4 @ case 11
	.4byte _080397F8 @ case 12
	.4byte _0803980A @ case 13
	.4byte _08039804 @ case 14
_080397AC:
	strh r5, [r4, #0x28]
	b _0803980A
_080397B0:
	rsbs r0, r5, #0
	strh r0, [r4, #0x28]
	b _0803980A
_080397B6:
	cmp r5, #0
	beq _0803980A
	mov r0, r8
	subs r0, #2
	lsls r0, r0, #5
	strh r0, [r4, #0x30]
	movs r1, #0x30
	ldrsh r0, [r4, r1]
	movs r2, #0x26
	ldrsh r1, [r4, r2]
	subs r0, r0, r1
	adds r1, r5, #0
	bl sub_803ADB4
	strh r0, [r4, #0x32]
	b _0803980A
_080397D6:
	lsrs r0, r5, #4
	lsls r1, r5, #8
	movs r3, #0xf0
	lsls r3, r3, #4
	adds r2, r3, #0
	ands r1, r2
	orrs r0, r1
	strh r0, [r7, #0x18]
	subs r0, #1
	b _08039808
_080397EA:
	strh r5, [r4, #0x1a]
	b _0803980A
_080397EE:
	rsbs r0, r5, #0
	strh r0, [r4, #0x1a]
	b _0803980A
_080397F4:
	strb r5, [r4, #0x15]
	b _0803980A
_080397F8:
	adds r1, r7, #0
	adds r1, #0x22
	movs r0, #1
	strb r0, [r1]
	strh r5, [r7, #0x24]
	b _0803980A
_08039804:
	strh r5, [r7, #0x18]
	subs r0, r5, #1
_08039808:
	strb r0, [r7, #0x1c]
_0803980A:
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8039818
sub_8039818: @ 0x08039818
	adds r2, r0, #0
	adds r3, r1, #0
	cmp r3, #1
	bne _08039844
	ldr r0, [r2, #0x3c]
	cmp r0, #0
	beq _0803983C
	ldr r0, [r0, #0x7c]
	ldrb r0, [r0, #1]
	cmp r0, #0xff
	bne _0803983C
	movs r0, #0
	ldr r1, _08039858 @ =0x00008AD0
	strh r1, [r2, #0x2a]
	strh r0, [r2, #0x2c]
	movs r0, #0x80
	lsls r0, r0, #0x18
	str r0, [r2, #0x4c]
_0803983C:
	adds r1, r2, #0
	adds r1, #0x22
	movs r0, #1
	strb r0, [r1]
_08039844:
	cmp r3, #1
	bls _08039856
	subs r0, r3, #2
	lsls r0, r0, #5
	movs r1, #0
	strh r0, [r2, #0x26]
	adds r0, r2, #0
	adds r0, #0x22
	strb r1, [r0]
_08039856:
	bx lr
	.align 2, 0
_08039858: .4byte 0x00008AD0

	thumb_func_start sub_803985C
sub_803985C: @ 0x0803985C
	push {r4, lr}
	mov ip, r0
	adds r4, r2, #0
	cmp r4, #0
	beq _080398D2
	ldr r1, [r3, #0x10]
	lsls r0, r4, #2
	adds r0, r0, r1
	ldr r0, [r0]
	mov r1, ip
	str r0, [r1, #0x3c]
	movs r1, #0
	movs r2, #0
	mov r3, ip
	strh r2, [r3, #0x38]
	mov r0, ip
	adds r0, #0x22
	strb r1, [r0]
	strh r2, [r3, #0x3a]
	ldr r0, [r3, #0x3c]
	ldrb r0, [r0, #8]
	adds r3, #0x23
	strb r0, [r3]
	mov r0, ip
	strh r2, [r0, #0x36]
	strb r1, [r0, #0x1f]
	adds r0, #0x20
	strb r1, [r0]
	movs r0, #0xff
	mov r1, ip
	strb r0, [r1, #0x15]
	ldr r1, [r1, #0x3c]
	adds r0, r1, #0
	adds r0, #0x84
	ldrb r0, [r0]
	mov r3, ip
	strb r0, [r3, #0x1e]
	strh r2, [r3, #0x32]
	strh r2, [r3, #0x30]
	ldrb r0, [r1]
	cmp r0, #0
	beq _080398B2
	str r2, [r3, #0x3c]
_080398B2:
	mov r1, ip
	ldr r0, [r1, #0x3c]
	cmp r0, #0
	beq _080398D2
	ldr r0, _080398D8 @ =gUnknown_03001630
	ldr r0, [r0]
	ldr r0, [r0, #4]
	ldr r1, [r0, #0x34]
	cmp r1, #0
	beq _080398D2
	mov r0, ip
	adds r0, #0x53
	ldrb r0, [r0]
	lsls r0, r0, #2
	adds r0, r0, r1
	strb r4, [r0]
_080398D2:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080398D8: .4byte gUnknown_03001630

	thumb_func_start sub_80398DC
sub_80398DC: @ 0x080398DC
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r3, r0, #0
	ldr r1, [r3, #0x3c]
	adds r0, r1, #0
	adds r0, #0x88
	ldrh r4, [r3, #0x36]
	lsls r2, r4, #3
	ldr r0, [r0]
	adds r0, r0, r2
	mov ip, r0
	adds r1, #0x85
	ldrb r1, [r1]
	cmp r4, r1
	blo _08039904
	movs r0, #0
	strb r0, [r3, #0x1e]
	b _08039A98
_08039904:
	adds r0, r4, #1
	strh r0, [r3, #0x36]
	mov r1, ip
	ldrb r0, [r1]
	cmp r0, #0
	beq _080399E2
	subs r0, #2
	lsls r0, r0, #5
	strh r0, [r3, #0x2a]
	ldrb r0, [r1, #1]
	adds r1, r3, #0
	adds r1, #0x21
	strb r0, [r1]
	mov r2, ip
	ldrb r0, [r2, #2]
	cmp r0, #0
	beq _080399E2
	subs r0, #1
	strb r0, [r3, #0x10]
	movs r0, #0
	str r0, [r3, #0x44]
	movs r0, #1
	mov r8, r0
	mov r1, r8
	strb r1, [r3, #0x11]
	movs r0, #0xff
	strb r0, [r3, #0x17]
	movs r0, #0
	strb r0, [r3, #0x12]
	ldr r2, [r3, #0x3c]
	ldrb r1, [r3, #0x10]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r5, r0, #2
	adds r6, r2, r5
	ldrb r0, [r6, #0xc]
	adds r4, r2, #0
	cmp r0, #0
	beq _080399CE
	adds r0, r4, #0
	adds r0, #0x14
	adds r0, r0, r5
	movs r2, #0x18
	adds r2, r2, r4
	mov sb, r2
	adds r1, r2, r5
	ldr r2, [r0]
	ldr r0, [r1]
	cmp r2, r0
	bge _080399CE
	adds r7, r4, #0
	adds r7, #0x1c
	adds r0, r7, r5
	ldr r0, [r0]
	cmp r0, #0
	ble _080399CE
	ldrh r0, [r6, #0x24]
	cmp r0, #0
	beq _080399CE
	adds r0, r4, #0
	adds r0, #0x20
	adds r0, r0, r5
	ldr r0, [r0]
	cmp r0, #0
	ble _080399CE
	mov r0, r8
	strb r0, [r3, #0x12]
	ldrb r0, [r3, #0x10]
	lsls r1, r0, #3
	subs r1, r1, r0
	lsls r1, r1, #2
	adds r0, r4, #0
	adds r0, #0x10
	adds r0, r0, r1
	ldr r2, [r0]
	str r2, [r3, #0x48]
	lsls r0, r2, #0xb
	str r0, [r3, #0x44]
	ldrb r1, [r3, #0x10]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r0, r4, r0
	ldrh r0, [r0, #0x24]
	strb r0, [r3, #0x14]
	mov r1, r8
	strb r1, [r3, #0x13]
	ldrb r1, [r3, #0x10]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r1, r7, r0
	ldr r1, [r1]
	adds r2, r2, r1
	add r0, sb
	ldr r0, [r0]
	cmp r2, r0
	ble _080399E2
	movs r0, #0xff
	strb r0, [r3, #0x13]
	b _080399E2
_080399CE:
	ldrb r0, [r3, #0x10]
	lsls r1, r0, #3
	subs r1, r1, r0
	lsls r1, r1, #2
	adds r0, r4, #0
	adds r0, #0x10
	adds r0, r0, r1
	ldr r0, [r0]
	lsls r0, r0, #0xb
	str r0, [r3, #0x44]
_080399E2:
	movs r0, #0
	strh r0, [r3, #0x1c]
	strh r0, [r3, #0x2c]
	movs r4, #0
_080399EA:
	lsls r0, r4, #1
	add r0, ip
	ldrh r0, [r0, #4]
	lsrs r1, r0, #8
	movs r2, #0xff
	ands r2, r0
	subs r0, r1, #1
	cmp r0, #0xe
	bhi _08039A92
	lsls r0, r0, #2
	ldr r1, _08039A08 @ =_08039A0C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08039A08: .4byte _08039A0C
_08039A0C: @ jump table
	.4byte _08039A48 @ case 0
	.4byte _08039A4C @ case 1
	.4byte _08039A92 @ case 2
	.4byte _08039A92 @ case 3
	.4byte _08039A52 @ case 4
	.4byte _08039A6A @ case 5
	.4byte _08039A92 @ case 6
	.4byte _08039A92 @ case 7
	.4byte _08039A92 @ case 8
	.4byte _08039A82 @ case 9
	.4byte _08039A86 @ case 10
	.4byte _08039A8C @ case 11
	.4byte _08039A92 @ case 12
	.4byte _08039A92 @ case 13
	.4byte _08039A90 @ case 14
_08039A48:
	strh r2, [r3, #0x2c]
	b _08039A92
_08039A4C:
	rsbs r0, r2, #0
	strh r0, [r3, #0x2c]
	b _08039A92
_08039A52:
	adds r1, r3, #0
	adds r1, #0x20
	ldrb r0, [r1]
	cmp r0, #0
	beq _08039A66
	subs r0, #1
	strb r0, [r1]
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08039A92
_08039A66:
	strh r2, [r3, #0x36]
	b _08039A92
_08039A6A:
	adds r0, r3, #0
	adds r0, #0x20
	ldrb r1, [r0]
	cmp r1, #0
	bne _08039A92
	cmp r2, #0
	beq _08039A7C
	adds r1, r2, #1
	b _08039A7E
_08039A7C:
	movs r1, #0
_08039A7E:
	strb r1, [r0]
	b _08039A92
_08039A82:
	strh r2, [r3, #0x1c]
	b _08039A92
_08039A86:
	rsbs r0, r2, #0
	strh r0, [r3, #0x1c]
	b _08039A92
_08039A8C:
	strb r2, [r3, #0x17]
	b _08039A92
_08039A90:
	strb r2, [r3, #0x1e]
_08039A92:
	adds r4, #1
	cmp r4, #1
	bls _080399EA
_08039A98:
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8039AA4
sub_8039AA4: @ 0x08039AA4
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x3c]
	cmp r0, #0
	beq _08039AC8
	ldr r1, [r0, #0x7c]
	adds r2, r4, #0
	adds r2, #0x38
	adds r0, r4, #0
	bl sub_8039F30
	strb r0, [r4, #0x16]
	adds r0, r4, #0
	bl sub_8039FFC
	adds r0, r4, #0
	bl sub_803A03C
_08039AC8:
	movs r1, #0x1a
	ldrsh r0, [r4, r1]
	ldrb r2, [r4, #0x15]
	adds r0, r0, r2
	cmp r0, #0xff
	ble _08039AD6
	movs r0, #0xff
_08039AD6:
	cmp r0, #0
	bge _08039ADC
	movs r0, #0
_08039ADC:
	movs r6, #0
	strb r0, [r4, #0x15]
	movs r3, #0x1c
	ldrsh r0, [r4, r3]
	ldrb r5, [r4, #0x17]
	adds r0, r0, r5
	cmp r0, #0xff
	ble _08039AEE
	movs r0, #0xff
_08039AEE:
	cmp r0, #0
	bge _08039AF4
	movs r0, #0
_08039AF4:
	strb r0, [r4, #0x17]
	ldrh r0, [r4, #0x28]
	ldrh r1, [r4, #0x26]
	adds r5, r0, r1
	strh r5, [r4, #0x26]
	ldrh r0, [r4, #0x2c]
	ldrh r2, [r4, #0x2a]
	adds r0, r0, r2
	strh r0, [r4, #0x2a]
	ldrh r1, [r4, #0x32]
	movs r3, #0x32
	ldrsh r0, [r4, r3]
	cmp r0, #0
	beq _08039B3C
	movs r0, #0x30
	ldrsh r2, [r4, r0]
	movs r3, #0x26
	ldrsh r0, [r4, r3]
	subs r2, r2, r0
	movs r3, #0x80
	lsls r3, r3, #0x18
	ands r2, r3
	adds r0, r5, r1
	strh r0, [r4, #0x26]
	movs r5, #0x30
	ldrsh r0, [r4, r5]
	movs r5, #0x26
	ldrsh r1, [r4, r5]
	subs r0, r0, r1
	ands r0, r3
	cmp r2, r0
	beq _08039B3C
	strh r6, [r4, #0x32]
	ldrh r0, [r4, #0x30]
	strh r0, [r4, #0x26]
	strh r6, [r4, #0x30]
_08039B3C:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8039B44
sub_8039B44: @ 0x08039B44
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x5c
	adds r6, r0, #0
	adds r4, r1, #0
	str r2, [sp, #0x50]
	ldr r0, [sp, #0x80]
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	ldr r3, [r6, #0x3c]
	cmp r3, #0
	beq _08039B8C
	movs r0, #0x2a
	ldrsh r1, [r6, r0]
	ldr r0, _08039B90 @ =0xFFFF8AD0
	cmp r1, r0
	beq _08039B8C
	ldrb r0, [r6, #0x10]
	cmp r0, #3
	bhi _08039B8C
	ldrb r2, [r6, #0x10]
	adds r0, r3, #1
	adds r0, r0, r2
	ldrb r0, [r0]
	lsls r0, r0, #3
	ldr r1, [sp, #0x7c]
	ldr r1, [r1, #0x14]
	adds r1, r1, r0
	str r1, [sp, #0x54]
	ldr r0, [r1]
	mov sb, r2
	cmp r0, #0
	bne _08039B94
_08039B8C:
	movs r0, #0
	b _08039F1E
	.align 2, 0
_08039B90: .4byte 0xFFFF8AD0
_08039B94:
	movs r1, #0x2a
	ldrsh r0, [r6, r1]
	movs r2, #0x2e
	ldrsh r1, [r6, r2]
	adds r2, r0, r1
	adds r0, r6, #0
	adds r0, #0x21
	ldrb r0, [r0]
	cmp r0, #0
	bne _08039BC8
	movs r3, #0x26
	ldrsh r0, [r6, r3]
	adds r2, r2, r0
	cmp r5, #0
	bne _08039BC8
	ldr r1, [r6]
	movs r3, #0x14
	ldrsh r0, [r4, r3]
	ldr r1, [r1, #0x18]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #2]
	lsls r0, r0, #0x18
	asrs r0, r0, #0x18
	lsls r0, r0, #5
	adds r2, r2, r0
_08039BC8:
	ldr r0, [r6, #0x3c]
	mov r3, sb
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	adds r1, r0, r1
	movs r3, #0x26
	ldrsh r1, [r1, r3]
	ldr r3, _08039CB8 @ =gStaticData_085A62DC
	adds r1, r2, r1
	ldr r2, _08039CBC @ =0x00000EF3
	mov r8, r0
	cmp r1, r2
	bls _08039BE6
	adds r1, r2, #0
_08039BE6:
	lsls r0, r1, #2
	adds r0, r0, r3
	ldr r1, [r0]
	ldrb r0, [r6, #0x16]
	movs r7, #0x80
	lsls r7, r7, #1
	cmp r0, #0xff
	beq _08039BF8
	adds r7, r0, #0
_08039BF8:
	ldrb r0, [r6, #0x17]
	cmp r0, #0xff
	beq _08039C02
	muls r0, r7, r0
	lsrs r7, r0, #8
_08039C02:
	ldrb r0, [r6, #0x15]
	cmp r0, #0xff
	beq _08039C0C
	muls r0, r7, r0
	lsrs r7, r0, #8
_08039C0C:
	ldrb r0, [r6, #0x18]
	cmp r0, #0xff
	beq _08039C16
	muls r0, r7, r0
	lsrs r7, r0, #8
_08039C16:
	ldrb r0, [r4, #0x1f]
	cmp r0, #0xff
	beq _08039C20
	muls r0, r7, r0
	lsrs r7, r0, #8
_08039C20:
	cmp r5, #0
	bne _08039C2E
	ldr r0, [r4]
	ldr r0, [r0, #0x18]
	ldrh r0, [r0, #8]
	muls r0, r7, r0
	lsrs r7, r0, #8
_08039C2E:
	adds r4, r1, #0
	asrs r5, r1, #0x1f
	ldr r0, _08039CC0 @ =gUnknown_03001618
	ldr r2, [r0]
	ldr r3, [r0, #4]
	adds r1, r5, #0
	adds r0, r4, #0
	bl sub_8037ECC
	adds r4, r1, #0
	mov sl, r4
	ldr r4, [sp, #0x54]
	ldr r3, [r4, #4]
	movs r0, #0
	str r0, [sp, #0x58]
	mov r1, sb
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r2, r0, #2
	mov r4, r8
	adds r0, r4, r2
	ldrb r0, [r0, #0xc]
	cmp r0, #0
	bne _08039C76
	mov r1, r8
	adds r1, #0x14
	adds r1, r1, r2
	mov r0, r8
	adds r0, #0x18
	adds r0, r0, r2
	ldr r1, [r1]
	ldr r0, [r0]
	cmp r1, r0
	bge _08039C76
	movs r0, #1
	str r0, [sp, #0x58]
_08039C76:
	ldr r0, [r6, #4]
	ldrh r2, [r0, #4]
	ldr r1, [sp, #0x54]
	ldr r0, [r1]
	ldr r1, [r6, #0x44]
	mov r4, sp
	str r0, [sp, #0x28]
	ldr r0, [sp, #0x50]
	str r0, [sp, #0x2c]
	str r1, [sp, #0x30]
	lsls r0, r3, #0xb
	str r0, [sp, #0x34]
	str r2, [sp, #0x38]
	movs r0, #0
	str r0, [sp, #0x3c]
	str r7, [sp, #0x40]
	mov r1, sl
	str r1, [sp, #0x44]
	str r0, [sp, #0x48]
	ldrb r0, [r6, #0x12]
	cmp r0, #0
	beq _08039CC4
	ldr r1, [r6, #0x3c]
	ldrb r2, [r6, #0x10]
	lsls r0, r2, #3
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r1, #0x1c
	adds r1, r1, r0
	ldr r0, [r1]
	lsls r0, r0, #0xb
	b _08039CC6
	.align 2, 0
_08039CB8: .4byte gStaticData_085A62DC
_08039CBC: .4byte 0x00000EF3
_08039CC0: .4byte gUnknown_03001618
_08039CC4:
	movs r0, #0
_08039CC6:
	str r0, [sp, #0x4c]
	add r1, sp, #0x28
	adds r0, r4, #0
	movs r2, #0x28
	bl sub_800014C
	ldr r1, [r6, #4]
	ldr r0, [sp, #0x14]
	ldrh r1, [r1, #4]
	cmp r0, r1
	blo _08039CDE
	b _08039F18
_08039CDE:
	ldr r0, _08039D08 @ =gStaticData_0803A874
	ldr r7, _08039D0C @ =gStaticData_0803A818
	subs r0, r0, r7
	adds r5, r0, #2
_08039CE6:
	movs r0, #0x11
	ldrsb r0, [r6, r0]
	cmp r0, #0
	ble _08039DC4
	ldr r2, [sp, #0x58]
	cmp r2, #0
	beq _08039D10
	ldr r2, [r6, #0x3c]
	ldrb r1, [r6, #0x10]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r2, #0x18
	adds r2, r2, r0
	ldr r0, [r2]
	b _08039D30
	.align 2, 0
_08039D08: .4byte gStaticData_0803A874
_08039D0C: .4byte gStaticData_0803A818
_08039D10:
	ldrb r0, [r6, #0x12]
	cmp r0, #0
	beq _08039D2C
	ldr r2, [r6, #0x3c]
	ldrb r1, [r6, #0x10]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r2, #0x1c
	adds r2, r2, r0
	ldr r0, [r6, #0x48]
	ldr r1, [r2]
	adds r0, r0, r1
	b _08039D30
_08039D2C:
	ldr r3, [sp, #0x54]
	ldr r0, [r3, #4]
_08039D30:
	lsls r0, r0, #0xb
	str r0, [sp, #0xc]
	ldrb r0, [r6, #0xd]
	cmp r0, #0
	beq _08039D78
	movs r0, #0
	str r0, [sp, #0x20]
	ldr r3, _08039D68 @ =gUnknown_03001630
	ldr r1, [r3]
	lsrs r0, r5, #0x1f
	adds r0, r5, r0
	asrs r0, r0, #1
	ldr r2, [r1, #0x44]
	lsls r0, r0, #1
	adds r0, r0, r2
	ldr r4, _08039D6C @ =0x0000E082
	adds r1, r4, #0
	strh r1, [r0]
	ldr r0, _08039D70 @ =gStaticData_0803A884
	subs r0, r0, r7
	adds r0, #2
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	lsls r0, r0, #1
	adds r0, r0, r2
	ldr r2, _08039D74 @ =0x0000BAFF
	b _08039E3A
	.align 2, 0
_08039D68: .4byte gUnknown_03001630
_08039D6C: .4byte 0x0000E082
_08039D70: .4byte gStaticData_0803A884
_08039D74: .4byte 0x0000BAFF
_08039D78:
	adds r0, r6, #0
	adds r0, #0x52
	ldrb r0, [r0]
	str r0, [sp, #0x20]
	ldr r3, _08039DB0 @ =gUnknown_03001630
	ldr r2, [r3]
	ldr r0, _08039DB4 @ =gStaticData_0803A8B4
	subs r0, r0, r7
	adds r0, #2
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	ldr r2, [r2, #0x44]
	lsls r0, r0, #1
	adds r0, r0, r2
	ldr r4, _08039DB8 @ =0x0000E082
	adds r1, r4, #0
	strh r1, [r0]
	ldr r0, _08039DBC @ =gStaticData_0803A8C4
	subs r0, r0, r7
	adds r0, #2
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	lsls r0, r0, #1
	adds r0, r0, r2
	ldr r2, _08039DC0 @ =0x0000BAFF
	b _08039E3A
	.align 2, 0
_08039DB0: .4byte gUnknown_03001630
_08039DB4: .4byte gStaticData_0803A8B4
_08039DB8: .4byte 0x0000E082
_08039DBC: .4byte gStaticData_0803A8C4
_08039DC0: .4byte 0x0000BAFF
_08039DC4:
	ldr r2, [r6, #0x3c]
	ldrb r1, [r6, #0x10]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r2, #0x14
	adds r2, r2, r0
	ldr r3, [r2]
	lsls r0, r3, #0xb
	str r0, [sp, #0xc]
	ldrb r0, [r6, #0xd]
	cmp r0, #0
	beq _08039E08
	movs r0, #0
	str r0, [sp, #0x20]
	ldr r3, _08039DFC @ =gUnknown_03001630
	ldr r1, [r3]
	lsrs r0, r5, #0x1f
	adds r0, r5, r0
	asrs r0, r0, #1
	ldr r2, [r1, #0x44]
	lsls r0, r0, #1
	adds r0, r0, r2
	ldr r4, _08039E00 @ =0x0000E042
	adds r1, r4, #0
	strh r1, [r0]
	ldr r0, _08039E04 @ =gStaticData_0803A884
	b _08039E2A
	.align 2, 0
_08039DFC: .4byte gUnknown_03001630
_08039E00: .4byte 0x0000E042
_08039E04: .4byte gStaticData_0803A884
_08039E08:
	movs r0, #1
	str r0, [sp, #0x20]
	ldr r3, _08039E88 @ =gUnknown_03001630
	ldr r2, [r3]
	ldr r0, _08039E8C @ =gStaticData_0803A8B4
	subs r0, r0, r7
	adds r0, #2
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	ldr r2, [r2, #0x44]
	lsls r0, r0, #1
	adds r0, r0, r2
	ldr r4, _08039E90 @ =0x0000E042
	adds r1, r4, #0
	strh r1, [r0]
	ldr r0, _08039E94 @ =gStaticData_0803A8C4
_08039E2A:
	subs r0, r0, r7
	adds r0, #2
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	lsls r0, r0, #1
	adds r0, r0, r2
	ldr r2, _08039E98 @ =0x0000CAFF
_08039E3A:
	adds r1, r2, #0
	strh r1, [r0]
	mov r4, sp
	ldr r3, [r3]
	ldr r3, [r3, #0x44]
	adds r1, r3, #0
	adds r0, r4, #0
	mov r2, pc
	adds r2, #5
	mov lr, r2
	bx r1
	thumb_func_start sub_8039E50
sub_8039E50: @ 0x08039E50
	nop			@ (mov r8, r8)
	ldr	r0, [r6, #4]
	ldrh	r2, [r0, #4]
	ldr	r0, [sp, #20]
	cmp	r0, r2
	beq _08039F18
	ldr	r3, [sp, #88]	@ 0x58
	cmp	r3, #0
	beq _08039EC0
	ldr	r3, [r6, #60]	@ 0x3c
	ldrb	r1, [r6, #16]
	lsls	r0, r1, #3
	subs	r0, r0, r1
	lsls	r2, r0, #2
	adds	r0, r3, r2
	ldrb	r0, [r0, #13]
	cmp	r0, #0
	beq _08039EAC
	movs	r0, #17
	ldrsb	r0, [r6, r0]
	ldrb	r2, [r6, #17]
	cmp	r0, #0
	ble _08039E9C
	mov	r4, sl
	lsls	r1, r4, #1
	ldr	r0, [sp, #8]
	subs	r0, r0, r1
	b _08039EA4
_08039E88: .4byte gUnknown_03001630
_08039E8C: .4byte gStaticData_0803A8B4
_08039E90: .4byte 0x0000E042
_08039E94: .4byte gStaticData_0803A8C4
_08039E98: .4byte 0x0000CAFF
_08039E9C:
	mov	r0, sl
	lsls	r1, r0, #1
	ldr	r0, [sp, #8]
	adds	r0, r0, r1
_08039EA4:
	str	r0, [sp, #8]
	mvns	r0, r2
	strb	r0, [r6, #17]
	b _08039F0C
_08039EAC:
	adds	r1, r3, #0
	adds	r1, #24
	adds	r1, r1, r2
	adds	r0, r3, #0
	adds	r0, #20
	adds	r0, r0, r2
	ldr	r1, [r1, #0]
	ldr	r0, [r0, #0]
	subs	r1, r1, r0
	b _08039ED6
_08039EC0:
	ldrb	r4, [r6, #18]
	cmp	r4, #0
	beq _08039EE0
	ldr	r2, [r6, #60]	@ 0x3c
	ldrb	r1, [r6, #16]
	lsls	r0, r1, #3
	subs	r0, r0, r1
	lsls	r0, r0, #2
	adds	r2, #28
	adds	r2, r2, r0
	ldr	r1, [r2, #0]
_08039ED6:
	lsls	r1, r1, #11
	ldr	r0, [sp, #8]
	subs	r0, r0, r1
	str	r0, [sp, #8]
	b _08039F0C
_08039EE0:
	ldrb	r0, [r6, #13]
	cmp	r0, #0
	beq _08039EFA
	ldr	r0, [sp, #20]
	lsls	r0, r0, #1
	ldr	r1, [sp, #80]	@ 0x50
	adds	r0, r1, r0
	ldr	r1, [sp, #20]
	subs	r1, r2, r1
	adds	r1, #1
	lsls	r1, r1, #1
	bl sub_8037F3C
_08039EFA:
	ldr r0, _08039F08
	strh	r0, [r6, #42]	@ 0x2a
	strh	r4, [r6, #44]	@ 0x2c
	movs	r0, #128	@ 0x80
	lsls	r0, r0, #24
	str	r0, [r6, #76]	@ 0x4c
	b _08039F18
_08039F08: .4byte 0x8ad0
_08039F0C:
	ldr	r1, [r6, #4]
	ldr	r0, [sp, #20]
	ldrh	r1, [r1, #4]
	cmp	r0, r1
	bcs _08039F18
	b _08039CE6
_08039F18:
	ldr r0, [sp, #8]
	str r0, [r6, #0x44]
	movs r0, #1
_08039F1E:
	add sp, #0x5c
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8039F30
sub_8039F30: @ 0x08039F30
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	adds r3, r1, #0
	adds r6, r2, #0
	ldrh r0, [r6]
	adds r1, r0, #1
	strh r1, [r6]
	lsls r0, r0, #0x10
	lsrs r4, r0, #0x10
	ldrb r0, [r3, #1]
	cmp r0, #0xff
	beq _08039F60
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	cmp r0, #0
	bne _08039F60
	ldrb r0, [r3, #1]
	lsls r0, r0, #3
	adds r0, r3, r0
	ldrh r0, [r0, #4]
	cmp r4, r0
	bne _08039F60
	strh r4, [r6]
_08039F60:
	ldrb r0, [r3]
	subs r2, r0, #1
	lsls r0, r2, #3
	adds r0, r3, r0
	ldrh r1, [r0, #4]
	cmp r4, r1
	blo _08039F8C
	ldrb r1, [r0, #8]
	cmp r1, #0
	bne _08039F8A
	ldrb r0, [r3, #3]
	cmp r0, #0xff
	beq _08039F7E
	cmp r0, r2
	bge _08039F8A
_08039F7E:
	ldr r0, _08039FF0 @ =0x00008AD0
	strh r0, [r5, #0x2a]
	strh r1, [r5, #0x2c]
	movs r0, #0x80
	lsls r0, r0, #0x18
	str r0, [r5, #0x4c]
_08039F8A:
	strh r4, [r6]
_08039F8C:
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	cmp r0, #0
	bne _08039FB6
	ldrb r0, [r3, #2]
	cmp r0, #0xff
	beq _08039FB6
	ldrb r0, [r3, #3]
	cmp r0, #0xff
	beq _08039FB6
	lsls r0, r0, #3
	adds r0, r3, r0
	ldrh r0, [r0, #4]
	cmp r4, r0
	bne _08039FB6
	ldrb r0, [r3, #2]
	lsls r0, r0, #3
	adds r0, r3, r0
	ldrh r0, [r0, #4]
	strh r0, [r6]
_08039FB6:
	movs r1, #0
	ldrh r0, [r3, #4]
	cmp r0, r4
	bhs _08039FCA
	adds r2, r3, #0
_08039FC0:
	adds r2, #8
	adds r1, #1
	ldrh r0, [r2, #4]
	cmp r0, r4
	blo _08039FC0
_08039FCA:
	lsls r0, r1, #3
	adds r0, r3, r0
	ldrh r2, [r0, #4]
	cmp r4, r2
	beq _08039FF4
	movs r5, #6
	ldrsh r2, [r0, r5]
	subs r1, #1
	lsls r1, r1, #3
	adds r1, r3, r1
	ldrh r0, [r1, #4]
	subs r0, r4, r0
	muls r0, r2, r0
	asrs r0, r0, #8
	ldrb r1, [r1, #8]
	adds r0, r0, r1
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	b _08039FF6
	.align 2, 0
_08039FF0: .4byte 0x00008AD0
_08039FF4:
	ldrb r0, [r0, #8]
_08039FF6:
	pop {r4, r5, r6}
	pop {r1}
	bx r1

	thumb_func_start sub_8039FFC
sub_8039FFC: @ 0x08039FFC
	adds r2, r0, #0
	ldr r3, [r2, #0x3c]
	ldrb r0, [r3, #9]
	cmp r0, #0
	beq _0803A034
	adds r1, r2, #0
	adds r1, #0x23
	ldrb r0, [r1]
	cmp r0, #0
	bne _0803A01E
	ldrb r0, [r3, #0xa]
	ldrh r1, [r2, #0x3a]
	adds r1, r1, r0
	movs r0, #0x3f
	ands r1, r0
	strh r1, [r2, #0x3a]
	b _0803A022
_0803A01E:
	subs r0, #1
	strb r0, [r1]
_0803A022:
	ldr r1, _0803A038 @ =gStaticData_085A9EAC
	ldrh r0, [r2, #0x3a]
	adds r0, r0, r1
	movs r1, #0
	ldrsb r1, [r0, r1]
	ldr r0, [r2, #0x3c]
	ldrb r0, [r0, #9]
	muls r0, r1, r0
	asrs r0, r0, #8
_0803A034:
	strh r0, [r2, #0x2e]
	bx lr
	.align 2, 0
_0803A038: .4byte gStaticData_085A9EAC

	thumb_func_start sub_803A03C
sub_803A03C: @ 0x0803A03C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r2, r0, #0
	ldrb r0, [r2, #0x12]
	cmp r0, #0
	beq _0803A0F8
	ldrb r0, [r2, #0x14]
	subs r0, #1
	strb r0, [r2, #0x14]
	movs r1, #0xff
	mov r8, r1
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0803A0F8
	ldr r6, [r2, #0x48]
	ldr r0, [r2, #0x3c]
	mov ip, r0
	ldrb r1, [r2, #0x10]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r0, r0, #2
	add r0, ip
	ldrh r0, [r0, #0x24]
	strb r0, [r2, #0x14]
	ldrb r7, [r2, #0x13]
	movs r0, #0x13
	ldrsb r0, [r2, r0]
	cmp r0, #0
	ble _0803A0B6
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r0, r0, #2
	mov r5, ip
	adds r5, #0x20
	adds r0, r5, r0
	ldr r0, [r0]
	adds r4, r6, r0
	str r4, [r2, #0x48]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r3, r0, #2
	mov r0, ip
	adds r0, #0x1c
	adds r0, r0, r3
	ldr r1, [r0]
	adds r1, r4, r1
	mov r0, ip
	adds r0, #0x18
	adds r0, r0, r3
	ldr r0, [r0]
	cmp r1, r0
	ble _0803A0EA
	adds r0, r5, r3
	ldr r0, [r0]
	lsls r0, r0, #1
	subs r0, r4, r0
	str r0, [r2, #0x48]
	mov r0, r8
	orrs r0, r7
	b _0803A0E8
_0803A0B6:
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r0, r0, #2
	mov r4, ip
	adds r4, #0x20
	adds r0, r4, r0
	ldr r0, [r0]
	subs r3, r6, r0
	str r3, [r2, #0x48]
	ldrb r1, [r2, #0x10]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r1, r0, #2
	mov r0, ip
	adds r0, #0x14
	adds r0, r0, r1
	ldr r0, [r0]
	cmp r3, r0
	bge _0803A0EA
	adds r0, r4, r1
	ldr r0, [r0]
	lsls r0, r0, #1
	adds r0, r3, r0
	str r0, [r2, #0x48]
	movs r0, #1
_0803A0E8:
	strb r0, [r2, #0x13]
_0803A0EA:
	lsls r0, r6, #0xb
	ldr r1, [r2, #0x44]
	subs r1, r1, r0
	ldr r0, [r2, #0x48]
	lsls r0, r0, #0xb
	adds r1, r1, r0
	str r1, [r2, #0x44]
_0803A0F8:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_803A104
sub_803A104: @ 0x0803A104
	adds r2, r0, #0
	movs r0, #0
	str r0, [r2, #0x44]
	strb r0, [r2, #0x10]
	str r0, [r2, #0x3c]
	movs r1, #0
	ldr r0, _0803A150 @ =0x00008AD0
	strh r0, [r2, #0x2a]
	movs r0, #1
	strb r0, [r2, #0x11]
	movs r0, #0xff
	strb r0, [r2, #0x15]
	movs r0, #1
	rsbs r0, r0, #0
	strb r0, [r2, #0x18]
	strb r1, [r2, #0xc]
	strb r1, [r2, #0x12]
	strb r1, [r2, #0xd]
	adds r0, r2, #0
	adds r0, #0x24
	strb r1, [r0]
	adds r0, #1
	strb r1, [r0]
	movs r0, #0x80
	lsls r0, r0, #0x18
	str r0, [r2, #0x4c]
	ldr r0, _0803A154 @ =gUnknown_03001630
	ldr r0, [r0]
	adds r0, #0x42
	ldrb r0, [r0]
	movs r1, #1
	cmp r0, #0
	beq _0803A148
	movs r1, #2
_0803A148:
	adds r0, r2, #0
	adds r0, #0x52
	strb r1, [r0]
	bx lr
	.align 2, 0
_0803A150: .4byte 0x00008AD0
_0803A154: .4byte gUnknown_03001630

	thumb_func_start sub_803A158
sub_803A158: @ 0x0803A158
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	adds r4, r0, #0
	mov sb, r1
	mov sl, r2
	ldr r0, [r4, #8]
	ldr r6, [r0]
	ldrb r0, [r6, #0x1b]
	cmp r0, #0
	beq _0803A178
	movs r0, #0
	str r0, [r4, #0x3c]
_0803A178:
	ldrb r0, [r6, #0x1a]
	cmp r0, #0
	beq _0803A1C6
	movs r0, #0x24
	adds r0, r0, r4
	mov r8, r0
	ldrb r0, [r0]
	cmp r0, #1
	beq _0803A198
	cmp r0, #0
	beq _0803A1C6
	adds r0, r4, #0
	adds r0, #0x25
	ldrb r0, [r0]
	cmp r0, #0
	beq _0803A1C6
_0803A198:
	ldrb r5, [r6, #0x1b]
	cmp r5, #0
	bne _0803A1C6
	mov r0, r8
	ldrb r1, [r0]
	adds r0, r4, #0
	bl sub_8039818
	adds r7, r4, #0
	adds r7, #0x25
	ldrb r2, [r7]
	ldr r0, [r4]
	ldr r3, [r0, #0x18]
	adds r0, r4, #0
	adds r1, r6, #0
	bl sub_803985C
	movs r0, #0
	strh r5, [r4, #0x1a]
	strh r5, [r4, #0x28]
	strb r0, [r7]
	mov r1, r8
	strb r0, [r1]
_0803A1C6:
	ldr r0, [r4, #0x3c]
	ldrb r1, [r4, #0x1f]
	cmp r0, #0
	beq _0803A1E6
	cmp r1, #0
	bne _0803A1E6
	ldrb r0, [r4, #0x1e]
	cmp r0, #0
	beq _0803A1EA
	adds r0, r4, #0
	adds r1, r6, #0
	bl sub_80398DC
	ldrb r0, [r4, #0x1e]
	subs r0, #1
	b _0803A1E8
_0803A1E6:
	subs r0, r1, #1
_0803A1E8:
	strb r0, [r4, #0x1f]
_0803A1EA:
	adds r0, r4, #0
	adds r1, r6, #0
	bl sub_8039AA4
	ldrb r0, [r4, #0xc]
	cmp r0, #0
	bne _0803A214
	ldr r0, [r4]
	ldr r0, [r0, #0x18]
	str r0, [sp]
	movs r0, #1
	str r0, [sp, #4]
	adds r0, r4, #0
	adds r1, r6, #0
	mov r2, sb
	mov r3, sl
	bl sub_8039B44
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	b _0803A216
_0803A214:
	movs r0, #0
_0803A216:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start nullsub_41
nullsub_41: @ 0x0803A228
	bx lr
	.align 2, 0

	thumb_func_start sub_803A22C
sub_803A22C: @ 0x0803A22C
	push {r4, r5, lr}
	adds r4, r0, #0
	movs r0, #1
	str r0, [r4, #0xc]
	movs r5, #0
	b _0803A24A
_0803A238:
	ldr r1, [r4, #8]
	lsls r0, r5, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r1, [r1]
	bl sub_803AD7C
	adds r5, #1
_0803A24A:
	ldr r0, _0803A260 @ =gUnknown_03001630
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	cmp r0, #0
	bne _0803A264
	ldr r0, [r4]
	ldr r0, [r0, #0xc]
	ldr r1, [r4, #0x14]
	adds r0, r0, r1
	b _0803A268
	.align 2, 0
_0803A260: .4byte gUnknown_03001630
_0803A264:
	ldr r0, [r4]
	ldr r0, [r0, #0xc]
_0803A268:
	cmp r5, r0
	blo _0803A238
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start nullsub_42
nullsub_42: @ 0x0803A274
	bx lr
	.align 2, 0

	thumb_func_start sub_803A278
sub_803A278: @ 0x0803A278
	push {r4, lr}
	sub sp, #0x2c
	ldr r0, [r0, #4]
	ldrh r2, [r0, #4]
	ldrb r0, [r0, #1]
	muls r0, r2, r0
	lsls r0, r0, #1
	ldr r4, _0803A2C4 @ =gUnknown_03001630
	ldr r2, [r4]
	ldr r3, [r2, #0x20]
	str r0, [sp, #0x14]
	str r1, [sp, #0x18]
	str r3, [sp, #0x1c]
	ldr r0, [r2, #0x28]
	str r0, [sp, #0x20]
	ldr r0, [r2, #0x24]
	str r0, [sp, #0x24]
	add r1, sp, #0x14
	mov r0, sp
	movs r2, #0x14
	bl sub_800014C
	mov r0, sp
	str r0, [sp, #0x28]
	ldr r3, [r4]
	adds r3, #0x9c
	adds r1, r3, #0
	ldr r0, [sp, #0x28]
	mov r2, pc
	adds r2, #5
	mov lr, r2
	bx r1
_0803A2B8:
	nop
	add sp, #0x2c
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0803A2C4: .4byte gUnknown_03001630

	thumb_func_start sub_803A2C8
sub_803A2C8: @ 0x0803A2C8
	push {r4, r5, r6, lr}
	sub sp, #0x3c
	ldr r0, [r0, #4]
	ldrh r4, [r0, #4]
	ldrb r0, [r0, #1]
	muls r0, r4, r0
	lsls r0, r0, #1
	ldr r6, _0803A320 @ =gUnknown_03001630
	ldr r4, [r6]
	ldr r5, [r4, #0x20]
	str r1, [sp, #0x1c]
	str r0, [sp, #0x20]
	movs r0, #0x55
	subs r0, r0, r2
	str r0, [sp, #0x24]
	str r3, [sp, #0x28]
	str r5, [sp, #0x2c]
	ldr r0, [r4, #0x28]
	str r0, [sp, #0x30]
	ldr r0, [r4, #0x24]
	str r0, [sp, #0x34]
	add r1, sp, #0x1c
	mov r0, sp
	movs r2, #0x1c
	bl sub_800014C
	mov r0, sp
	str r0, [sp, #0x38]
	ldr r3, [r6]
	movs r0, #0xbe
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r3, [r3]
	adds r1, r3, #0
	ldr r0, [sp, #0x38]
	mov r2, pc
	adds r2, #5
	mov lr, r2
	bx r1
	nop
	thumb_func_start sub_803A318
sub_803A318: @ 0x0803A318
	add	sp, #60	@ 0x3c
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
_0803A320: .4byte gUnknown_03001630

	thumb_func_start sub_803A324
sub_803A324: @ 0x0803A324
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	mov r8, r1
	mov sb, r2
	movs r6, #0
	ldr r0, _0803A494 @ =gUnknown_03001630
	ldr r0, [r0]
	adds r0, #0x41
	ldrb r0, [r0]
	cmp r0, #0
	bne _0803A382
	movs r4, #0
	ldr r0, [r5]
	ldr r0, [r0, #0x18]
	ldr r0, [r0]
	cmp r6, r0
	bhs _0803A382
_0803A34C:
	ldr r0, [r5, #8]
	lsls r1, r4, #2
	adds r0, r1, r0
	ldr r0, [r0]
	movs r2, #0
	cmp r6, #0
	bne _0803A35C
	movs r2, #1
_0803A35C:
	strb r2, [r0, #0xd]
	ldr r0, [r5, #8]
	adds r0, r1, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r3, [r1, #8]
	mov r1, r8
	mov r2, sb
	bl sub_803AD84
	orrs r6, r0
	lsls r0, r6, #0x18
	lsrs r6, r0, #0x18
	adds r4, #1
	ldr r0, [r5]
	ldr r0, [r0, #0x18]
	ldr r0, [r0]
	cmp r4, r0
	blo _0803A34C
_0803A382:
	ldr r0, _0803A494 @ =gUnknown_03001630
	ldr r1, [r0]
	adds r0, r1, #0
	adds r0, #0x40
	ldrb r0, [r0]
	cmp r0, #0
	beq _0803A410
	ldr r3, [r5]
	ldr r7, [r5, #0x14]
	cmp r6, #0
	bne _0803A3D0
	ldr r1, [r1, #4]
	ldr r0, [r1, #0x30]
	ldr r0, [r0, #8]
	ldr r0, [r0, #0x18]
	ldrb r0, [r0, #0x1b]
	cmp r0, #0
	bne _0803A3B0
	ldrh r1, [r1, #0xc]
	movs r0, #0x20
	ands r0, r1
	cmp r0, #0
	beq _0803A3D0
_0803A3B0:
	ldr r0, [r5, #4]
	ldrh r1, [r0, #4]
	ldrb r0, [r0, #1]
	muls r0, r1, r0
	lsls r0, r0, #1
	mov r2, r8
	movs r6, #1
	ldr r3, [r5]
	ldr r7, [r5, #0x14]
	cmp r0, #0
	ble _0803A3D0
	movs r1, #0
_0803A3C8:
	stm r2!, {r1}
	subs r0, #4
	cmp r0, #0
	bgt _0803A3C8
_0803A3D0:
	ldr r4, [r3, #0xc]
	adds r0, r4, r7
	cmp r4, r0
	bhs _0803A410
_0803A3D8:
	ldr r0, [r5, #8]
	lsls r1, r4, #2
	adds r0, r1, r0
	ldr r0, [r0]
	movs r2, #0
	cmp r6, #0
	bne _0803A3E8
	movs r2, #1
_0803A3E8:
	strb r2, [r0, #0xd]
	ldr r0, [r5, #8]
	adds r0, r1, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r3, [r1, #8]
	mov r1, r8
	mov r2, sb
	bl sub_803AD84
	orrs r6, r0
	lsls r0, r6, #0x18
	lsrs r6, r0, #0x18
	adds r4, #1
	ldr r0, [r5]
	ldr r0, [r0, #0xc]
	ldr r1, [r5, #0x14]
	adds r0, r0, r1
	cmp r4, r0
	blo _0803A3D8
_0803A410:
	ldr r0, [r5]
	ldr r1, [r0, #0x18]
	ldr r1, [r1, #4]
	adds r3, r0, #0
	cmp r1, #0
	beq _0803A480
	ldr r4, _0803A494 @ =gUnknown_03001630
	cmp r6, #0
	bne _0803A43E
	ldr r0, [r5, #4]
	ldrh r1, [r0, #4]
	ldrb r0, [r0, #1]
	muls r0, r1, r0
	lsls r0, r0, #1
	mov r1, r8
	movs r6, #1
	cmp r0, #0
	ble _0803A43E
	movs r2, #0
_0803A436:
	stm r1!, {r2}
	subs r0, #4
	cmp r0, #0
	bgt _0803A436
_0803A43E:
	movs r7, #1
	ldr r0, [r4]
	ldr r1, [r0, #0x10]
	lsls r1, r1, #2
	adds r0, #8
	adds r0, r0, r1
	ldr r1, [r0]
	ldr r0, [r1, #4]
	ldrb r0, [r0, #0x1f]
	cmp r0, #0
	beq _0803A474
	movs r4, #0
	ldr r0, [r3, #0xc]
	cmp r4, r0
	bhs _0803A474
	adds r2, r0, #0
_0803A45E:
	ldr r0, [r1, #0xc]
	ldrb r0, [r0, #0x18]
	cmp r0, #0
	beq _0803A468
	movs r7, #0
_0803A468:
	adds r1, #4
	adds r4, #1
	cmp r4, r2
	bhs _0803A474
	cmp r7, #0
	bne _0803A45E
_0803A474:
	cmp r7, #0
	bne _0803A480
	adds r0, r5, #0
	mov r1, r8
	bl sub_803A278
_0803A480:
	ldr r0, _0803A494 @ =gUnknown_03001630
	ldr r0, [r0]
	adds r0, #0x41
	ldrb r0, [r0]
	cmp r0, #0
	bne _0803A4CC
	ldr r0, [r5]
	ldr r1, [r0, #0x18]
	ldr r4, [r1]
	b _0803A4C6
	.align 2, 0
_0803A494: .4byte gUnknown_03001630
_0803A498:
	ldr r0, [r5, #8]
	lsls r1, r4, #2
	adds r0, r1, r0
	ldr r0, [r0]
	movs r2, #0
	cmp r6, #0
	bne _0803A4A8
	movs r2, #1
_0803A4A8:
	strb r2, [r0, #0xd]
	ldr r0, [r5, #8]
	adds r0, r1, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r3, [r1, #8]
	mov r1, r8
	mov r2, sb
	bl sub_803AD84
	orrs r6, r0
	lsls r0, r6, #0x18
	lsrs r6, r0, #0x18
	adds r4, #1
	ldr r0, [r5]
_0803A4C6:
	ldr r0, [r0, #0xc]
	cmp r4, r0
	blo _0803A498
_0803A4CC:
	ldr r0, _0803A5A4 @ =gUnknown_03001630
	ldr r1, [r0]
	movs r2, #0xc0
	lsls r2, r2, #1
	adds r0, r1, r2
	ldr r2, [r0]
	cmp r2, #0x55
	bls _0803A4DE
	movs r2, #0x55
_0803A4DE:
	str r2, [r0]
	movs r3, #0xbe
	lsls r3, r3, #1
	adds r0, r1, r3
	ldr r0, [r0]
	cmp r0, #0
	beq _0803A4FC
	cmp r2, #0
	beq _0803A4FC
	ldr r0, [r5]
	ldr r3, [r0, #0xc]
	adds r0, r5, #0
	mov r1, r8
	bl sub_803A2C8
_0803A4FC:
	ldr r0, _0803A5A4 @ =gUnknown_03001630
	ldr r1, [r0]
	adds r0, r1, #0
	adds r0, #0x40
	ldrb r0, [r0]
	cmp r0, #0
	bne _0803A58A
	ldr r3, [r5]
	ldr r7, [r5, #0x14]
	cmp r6, #0
	bne _0803A54A
	ldr r1, [r1, #4]
	ldr r0, [r1, #0x30]
	ldr r0, [r0, #8]
	ldr r0, [r0, #0x18]
	ldrb r0, [r0, #0x1b]
	cmp r0, #0
	bne _0803A52A
	ldrh r1, [r1, #0xc]
	movs r0, #0x20
	ands r0, r1
	cmp r0, #0
	beq _0803A54A
_0803A52A:
	ldr r0, [r5, #4]
	ldrh r1, [r0, #4]
	ldrb r0, [r0, #1]
	muls r0, r1, r0
	lsls r0, r0, #1
	mov r2, r8
	movs r6, #1
	ldr r3, [r5]
	ldr r7, [r5, #0x14]
	cmp r0, #0
	ble _0803A54A
	movs r1, #0
_0803A542:
	stm r2!, {r1}
	subs r0, #4
	cmp r0, #0
	bgt _0803A542
_0803A54A:
	ldr r4, [r3, #0xc]
	adds r0, r4, r7
	cmp r4, r0
	bhs _0803A58A
_0803A552:
	ldr r0, [r5, #8]
	lsls r1, r4, #2
	adds r0, r1, r0
	ldr r0, [r0]
	movs r2, #0
	cmp r6, #0
	bne _0803A562
	movs r2, #1
_0803A562:
	strb r2, [r0, #0xd]
	ldr r0, [r5, #8]
	adds r0, r1, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r3, [r1, #8]
	mov r1, r8
	mov r2, sb
	bl sub_803AD84
	orrs r6, r0
	lsls r0, r6, #0x18
	lsrs r6, r0, #0x18
	adds r4, #1
	ldr r0, [r5]
	ldr r0, [r0, #0xc]
	ldr r1, [r5, #0x14]
	adds r0, r0, r1
	cmp r4, r0
	blo _0803A552
_0803A58A:
	ldr r0, _0803A5A4 @ =gUnknown_03001630
	ldr r0, [r0]
	adds r0, #0x41
	movs r1, #0
	strb r1, [r0]
	adds r0, r6, #0
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0803A5A4: .4byte gUnknown_03001630

	thumb_func_start sub_803A5A8
sub_803A5A8: @ 0x0803A5A8
	push {r4, r5, r6, lr}
	sub sp, #0x14
	adds r4, r0, #0
	adds r6, r1, #0
	ldr r0, [r4]
	ldr r1, [r0, #0xc]
	ldr r0, [r4, #0x14]
	adds r0, r1, r0
	cmp r0, #1
	beq _0803A5C2
	adds r0, #8
	lsls r3, r0, #6
	b _0803A5C6
_0803A5C2:
	movs r3, #0x80
	lsls r3, r3, #3
_0803A5C6:
	ldr r1, [r4, #0x10]
	ldr r0, [r4, #4]
	ldrh r2, [r0, #4]
	ldrb r0, [r0, #1]
	adds r5, r2, #0
	muls r5, r0, r5
	str r1, [sp]
	str r6, [sp, #4]
	str r5, [sp, #8]
	str r3, [sp, #0xc]
	ldr r3, [r4]
	ldr r0, [r4, #0xc]
	adds r2, r0, #0
	adds r0, #1
	str r0, [r4, #0xc]
	ldr r3, [r3, #8]
	adds r0, r4, #0
	bl sub_803AD84
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0803A610
	mov r0, sp
	str r0, [sp, #0x10]
	ldr r0, _0803A60C @ =gUnknown_03001630
	ldr r3, [r0]
	adds r3, #0x48
	adds r1, r3, #0
	ldr r0, [sp, #0x10]
	mov r2, pc
	adds r2, #5
	mov lr, r2
	bx r1
	thumb_func_start sub_803A608
sub_803A608: @ 0x0803A608
	nop			@ (mov r8, r8)
	b _0803A61E
_0803A60C: .4byte gUnknown_03001630
_0803A610:
	cmp r5, #0
	ble _0803A61E
	movs r0, #0
_0803A616:
	stm r6!, {r0}
	subs r5, #4
	cmp r5, #0
	bgt _0803A616
_0803A61E:
	add sp, #0x14
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0803A628:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
.global gStaticData_0803A630
gStaticData_0803A630:
	.byte 0x60, 0x00, 0x2D, 0xE9, 0x0C, 0x50, 0x90, 0xE5, 0x08, 0x20, 0x90, 0xE5, 0x04, 0x10, 0x90, 0xE5
	.byte 0x00, 0x00, 0x90, 0xE5, 0x01, 0x20, 0x82, 0xE0, 0xF2, 0x60, 0xD0, 0xE0, 0x95, 0x06, 0x06, 0xE0
	.byte 0x46, 0x65, 0xA0, 0xE1, 0x80, 0x00, 0x76, 0xE3, 0x7F, 0x60, 0xE0, 0xB3, 0x7F, 0x00, 0x56, 0xE3
	.byte 0x7F, 0x60, 0xA0, 0xC3, 0x01, 0x60, 0xC1, 0xE4, 0x02, 0x00, 0x51, 0xE1, 0xF5, 0xFF, 0xFF, 0xBA
	.byte 0x60, 0x00, 0xBD, 0xE8, 0x0E, 0x00, 0xA0, 0xE1, 0x10, 0xFF, 0x2F, 0xE1
.global gStaticData_0803A67C
gStaticData_0803A67C:
	.byte 0xF0, 0x0F, 0x2D, 0xE9, 0x0C, 0x10, 0x90, 0xE5, 0x08, 0x70, 0x90, 0xE5, 0x04, 0x20, 0x90, 0xE5
	.byte 0x00, 0x00, 0x90, 0xE5, 0x98, 0x40, 0x8F, 0xE2, 0x04, 0x50, 0x94, 0xE5, 0x00, 0x40, 0x94, 0xE5
	.byte 0xCD, 0x8F, 0xA0, 0xE3, 0x98, 0x07, 0x07, 0xE0, 0x47, 0x74, 0xA0, 0xE1, 0x7F, 0x90, 0xA0, 0xE3
	.byte 0x09, 0x94, 0xA0, 0xE1, 0x91, 0x09, 0x09, 0xE0, 0xF0, 0x80, 0xD0, 0xE1, 0x08, 0x84, 0xA0, 0xE1
	.byte 0x05, 0x60, 0x48, 0xE0, 0x04, 0x60, 0x46, 0xE0, 0x09, 0x00, 0x76, 0xE1, 0x09, 0x60, 0xE0, 0xB1
	.byte 0x09, 0x00, 0x56, 0xE1, 0x09, 0x60, 0xA0, 0xC1, 0x96, 0x07, 0x0A, 0xE0, 0x4A, 0x54, 0x85, 0xE0
	.byte 0x09, 0x00, 0x75, 0xE1, 0x09, 0x50, 0xE0, 0xB1, 0x09, 0x00, 0x55, 0xE1, 0x09, 0x50, 0xA0, 0xC1
	.byte 0x95, 0x07, 0x0A, 0xE0, 0x4A, 0x44, 0x84, 0xE0, 0x09, 0x00, 0x74, 0xE1, 0x09, 0x40, 0xE0, 0xB1
	.byte 0x09, 0x00, 0x54, 0xE1, 0x09, 0x40, 0xA0, 0xC1, 0x44, 0xA4, 0xA0, 0xE1, 0xB0, 0xA0, 0xC0, 0xE1
	.byte 0x02, 0x00, 0x80, 0xE2, 0x02, 0x20, 0x52, 0xE2, 0x00, 0x00, 0x52, 0xE3, 0xE5, 0xFF, 0xFF, 0xCA
	.byte 0x0C, 0x00, 0x8F, 0xE2, 0x00, 0x40, 0x80, 0xE5, 0x04, 0x50, 0x80, 0xE5, 0xF0, 0x0F, 0xBD, 0xE8
	.byte 0x1E, 0xFF, 0x2F, 0xE1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x49, 0x4C, 0x54
.global gStaticData_0803A73C
gStaticData_0803A73C:
	.byte 0xF0, 0x01, 0x2D, 0xE9
	.byte 0xC8, 0x50, 0x9F, 0xE5, 0x00, 0x40, 0x90, 0xE5, 0x0C, 0x30, 0x90, 0xE5, 0x10, 0x20, 0x90, 0xE5
	.byte 0x08, 0x10, 0x90, 0xE5, 0x04, 0x00, 0x90, 0xE5, 0x00, 0x40, 0x84, 0xE0, 0x03, 0x00, 0x55, 0xE1
	.byte 0x00, 0x50, 0xA0, 0x03, 0x03, 0x70, 0x85, 0xE0, 0x00, 0x80, 0x92, 0xE5, 0x08, 0x70, 0x47, 0xE0
	.byte 0x03, 0x00, 0x57, 0xE1, 0x03, 0x70, 0x47, 0xA0, 0xF7, 0x70, 0x91, 0xE1, 0x04, 0x80, 0x92, 0xE5
	.byte 0x98, 0x07, 0x17, 0xE0, 0x47, 0x62, 0xA0, 0xE1, 0x08, 0x80, 0x92, 0xE5, 0x00, 0x00, 0x58, 0xE3
	.byte 0x11, 0x00, 0x00, 0x0A, 0x03, 0x70, 0x85, 0xE0, 0x08, 0x70, 0x47, 0xE0, 0x03, 0x00, 0x57, 0xE1
	.byte 0x03, 0x70, 0x47, 0xA0, 0xF7, 0x70, 0x91, 0xE1, 0x0C, 0x80, 0x92, 0xE5, 0x98, 0x07, 0x17, 0xE0
	.byte 0x47, 0x62, 0x86, 0xE0, 0x10, 0x80, 0x92, 0xE5, 0x00, 0x00, 0x58, 0xE3, 0x06, 0x00, 0x00, 0x0A
	.byte 0x03, 0x70, 0x85, 0xE0, 0x08, 0x70, 0x47, 0xE0, 0x03, 0x00, 0x57, 0xE1, 0x03, 0x70, 0x47, 0xA0
	.byte 0xF7, 0x70, 0x91, 0xE1, 0x14, 0x80, 0x92, 0xE5, 0x47, 0x62, 0x86, 0xE0, 0xF0, 0x80, 0xD0, 0xE1
	.byte 0x06, 0x80, 0x88, 0xE0, 0xB0, 0x80, 0xC0, 0xE1, 0xB5, 0x80, 0x81, 0xE1, 0x02, 0x00, 0x80, 0xE2
	.byte 0x02, 0x50, 0x85, 0xE2, 0x04, 0x00, 0x50, 0xE1, 0xD7, 0xFF, 0xFF, 0xBA, 0x0C, 0x00, 0x8F, 0xE2
	.byte 0x00, 0x50, 0x80, 0xE5, 0xF0, 0x01, 0xBD, 0xE8, 0x1E, 0xFF, 0x2F, 0xE1, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x42, 0x41, 0x52, 0x54
.global gStaticData_0803A818
gStaticData_0803A818:
	.byte 0xF1, 0x0F, 0x2D, 0xE9, 0x18, 0x70, 0x90, 0xE5
	.byte 0x1C, 0x80, 0x90, 0xE5, 0x04, 0x10, 0x90, 0xE5, 0x08, 0x20, 0x90, 0xE5, 0x0C, 0x30, 0x90, 0xE5
	.byte 0x10, 0x40, 0x90, 0xE5, 0x84, 0x40, 0x81, 0xE0, 0x14, 0x50, 0x90, 0xE5, 0x85, 0x10, 0x81, 0xE0
	.byte 0x20, 0xA0, 0x90, 0xE5, 0x24, 0xB0, 0x90, 0xE5, 0x00, 0x00, 0x90, 0xE5, 0x0A, 0xA1, 0x9F, 0xE7
	.byte 0x0A, 0xF0, 0x8F, 0xE0, 0x20, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xC4, 0x00, 0x00, 0x00
	.byte 0xC2, 0x65, 0xA0, 0xE1, 0xD6, 0x60, 0x90, 0xE1, 0x97, 0x06, 0x06, 0xE0, 0x46, 0x64, 0xA0, 0xE1
	.byte 0xB2, 0x60, 0xC1, 0xE0
gStaticData_0803A874:
	.byte 0x08, 0x20, 0x82, 0xE0, 0x04, 0x00, 0x51, 0xE1, 0x14, 0x00, 0x00, 0xAA, 0x03, 0x00, 0x52, 0xE1
gStaticData_0803A884:
	.byte 0xF5, 0xFF, 0xFF, 0xBA, 0x00, 0x00, 0x5B, 0xE3, 0x10, 0x00, 0x00, 0x0A, 0x0B, 0x20, 0x42, 0xE0
	.byte 0xF1, 0xFF, 0xFF, 0xEA, 0xC2, 0x65, 0xA0, 0xE1, 0xD6, 0x60, 0x90, 0xE1, 0x97, 0x06, 0x06, 0xE0
	.byte 0x46, 0x64, 0xA0, 0xE1, 0xF0, 0x90, 0xD1, 0xE1, 0x09, 0x60, 0x86, 0xE0, 0xB2, 0x60, 0xC1, 0xE0
gStaticData_0803A8B4:
	.byte 0x08, 0x20, 0x82, 0xE0, 0x04, 0x00, 0x51, 0xE1, 0x04, 0x00, 0x00, 0xAA, 0x03, 0x00, 0x52, 0xE1
gStaticData_0803A8C4:
	.byte 0xF3, 0xFF, 0xFF, 0xBA, 0x00, 0x00, 0x5B, 0xE3, 0x0B, 0x20, 0x42, 0x10, 0xF0, 0xFF, 0xFF, 0x1A
	.byte 0xF1, 0x0F, 0xBD, 0xE8, 0x04, 0x30, 0x90, 0xE5, 0x03, 0x30, 0x41, 0xE0, 0xA3, 0x30, 0xA0, 0xE1
	.byte 0x08, 0x20, 0x80, 0xE5, 0x14, 0x30, 0x80, 0xE5, 0x0E, 0x00, 0xA0, 0xE1
	.byte 0x10, 0xFF, 0x2F, 0xE1, 0xC2, 0x65, 0xA0, 0xE1, 0xD6, 0x60, 0x90, 0xE1, 0x97, 0x06, 0x06, 0xE0
	.byte 0x46, 0x64, 0xA0, 0xE1, 0x06, 0x68, 0xA0, 0xE1, 0x26, 0x68, 0x86, 0xE1, 0x00, 0x90, 0x91, 0xE5
	.byte 0x09, 0x90, 0x86, 0xE0, 0x04, 0x90, 0x81, 0xE4, 0x88, 0x20, 0x82, 0xE0, 0x03, 0x00, 0x52, 0xE1
	.byte 0x04, 0x00, 0x51, 0xB1, 0xF2, 0xFF, 0xFF, 0xBA, 0x03, 0x00, 0x52, 0xE1, 0x01, 0x00, 0x00, 0xCA
	.byte 0x04, 0x00, 0x51, 0xE1, 0xE6, 0xFF, 0xFF, 0xDA, 0x02, 0x10, 0x41, 0xE2, 0x08, 0x20, 0x42, 0xE0
	.byte 0xE3, 0xFF, 0xFF, 0xEA

