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

