.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_0800D18C
sub_0800D18C: @ 0x0800D18C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xa4
	mov sl, r0
	adds r6, r1, #0
	ldr r1, [r0, #0x20]
	mov r2, sl
	adds r2, #0x2d
	ldrb r3, [r2]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	ldr r1, [r1]
	adds r1, r1, r0
	adds r3, r1, #4
	mov r4, sl
	ldr r0, [r4]
	asrs r0, r0, #8
	str r0, [sp, #0x70]
	ldr r0, [r4, #4]
	asrs r0, r0, #8
	str r0, [sp, #0x74]
	movs r5, #4
	ldrsh r1, [r1, r5]
	movs r0, #2
	ldrsh r2, [r3, r0]
	ldrb r4, [r3, #4]
	ldrb r5, [r3, #5]
	ldr r3, [sp, #0x70]
	adds r1, r1, r3
	ldr r0, [sp, #0x74]
	adds r2, r2, r0
	add r0, sp, #0x1c
	bl sub_803AFE4
	add r0, sp, #0x1c
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_803AFDC
	mov r3, sl
	adds r3, #0x28
	ldrb r1, [r3]
	lsls r0, r1, #0x1b
	cmp r0, #0
	bge _0800D1FC
	ldr r2, [sp, #0x70]
	lsls r0, r2, #1
	ldr r1, [sp, #0x1c]
	ldr r2, [sp, #0x24]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #0x1c]
_0800D1FC:
	ldrb r3, [r3]
	lsls r0, r3, #0x1a
	cmp r0, #0
	bge _0800D212
	ldr r3, [sp, #0x74]
	lsls r0, r3, #1
	ldr r1, [sp, #0x20]
	ldr r2, [sp, #0x28]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #0x20]
_0800D212:
	ldr r1, _0800D234 @ =gUnknown_030012D8
	ldr r3, [r1]
	ldr r0, [r3]
	asrs r0, r0, #8
	str r0, [sp, #0x70]
	ldr r0, [r3, #4]
	asrs r0, r0, #8
	str r0, [sp, #0x74]
	ldr r0, _0800D238 @ =gUnknown_030012C0
	ldr r0, [r0]
	ldr r0, [r0, #0x78]
	cmp r0, #3
	bne _0800D23C
	movs r4, #6
	str r4, [sp, #0x78]
	b _0800D264
	.align 2, 0
_0800D234: .4byte gUnknown_030012D8
_0800D238: .4byte gUnknown_030012C0
_0800D23C:
	ldr r1, _0800D2A0 @ =gStaticData_0816BBF0
	lsls r0, r6, #2
	adds r0, r0, r1
	ldr r0, [r0]
	str r0, [sp, #0x78]
	mov r0, sl
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #0xd
	bne _0800D264
	ldr r5, [sp, #0x78]
	cmp r5, #5
	bne _0800D264
	adds r0, r3, #0
	adds r0, #0x24
	ldrb r0, [r0]
	cmp r0, #4
	bne _0800D264
	movs r6, #2
	str r6, [sp, #0x78]
_0800D264:
	mov r1, sl
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r2, [r1]
	ands r0, r2
	str r1, [sp, #0x94]
	cmp r0, #1
	bne _0800D276
	b _0800D6AC
_0800D276:
	mov r3, sl
	ldr r0, [r3, #0x44]
	cmp r0, #0
	beq _0800D280
	b _0800D6AC
_0800D280:
	ldr r4, _0800D2A4 @ =gUnknown_030012D8
	ldr r0, [r4]
	bl sub_80083B8
	adds r2, r0, #0
	ldr r0, [r2, #4]
	ldrb r0, [r0]
	lsrs r0, r0, #4
	cmp r0, #6
	bhi _0800D2CE
	lsls r0, r0, #2
	ldr r1, _0800D2A8 @ =_0800D2AC
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800D2A0: .4byte gStaticData_0816BBF0
_0800D2A4: .4byte gUnknown_030012D8
_0800D2A8: .4byte _0800D2AC
_0800D2AC: @ jump table
	.4byte _0800D2C8 @ case 0
	.4byte _0800D2CE @ case 1
	.4byte _0800D2CE @ case 2
	.4byte _0800D2CE @ case 3
	.4byte _0800D2C8 @ case 4
	.4byte _0800D2CE @ case 5
	.4byte _0800D2CE @ case 6
_0800D2C8:
	adds r3, r2, #0
	adds r3, #0x1c
	b _0800D2D0
_0800D2CE:
	ldr r3, _0800D36C @ =gStaticData_0816B2F8
_0800D2D0:
	movs r6, #0
	ldrb r0, [r3, #4]
	cmp r0, #0
	bne _0800D2E0
	ldrb r0, [r3, #5]
	cmp r0, #0
	bne _0800D2E0
	movs r6, #1
_0800D2E0:
	cmp r6, #0
	beq _0800D2E6
	b _0800D6AC
_0800D2E6:
	movs r5, #0
	ldrsh r1, [r3, r5]
	movs r0, #2
	ldrsh r2, [r3, r0]
	ldrb r4, [r3, #4]
	ldrb r5, [r3, #5]
	ldr r3, [sp, #0x70]
	adds r1, r1, r3
	ldr r0, [sp, #0x74]
	adds r2, r2, r0
	add r0, sp, #0x3c
	bl sub_803AFE4
	add r0, sp, #0x3c
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_803AFDC
	ldr r3, _0800D370 @ =gUnknown_030012D8
	ldr r0, [r3]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _0800D326
	ldr r1, [sp, #0x70]
	lsls r0, r1, #1
	ldr r1, [sp, #0x3c]
	ldr r2, [sp, #0x44]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #0x3c]
_0800D326:
	ldr r0, [r3]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1a
	cmp r0, #0
	bge _0800D340
	ldr r2, [sp, #0x74]
	lsls r0, r2, #1
	ldr r1, [sp, #0x40]
	ldr r2, [sp, #0x48]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #0x40]
_0800D340:
	add r4, sp, #0x3c
	add r0, sp, #0x1c
	adds r1, r4, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0800D352
	b _0800D6AC
_0800D352:
	add r0, sp, #0x4c
	strb r6, [r0]
	ldr r3, [sp, #0x78]
	cmp r3, #4
	bgt _0800D374
	mov r0, sl
	adds r1, r4, #0
	add r2, sp, #0x4c
	bl sub_800CF70
	mov r8, r0
	b _0800D376
	.align 2, 0
_0800D36C: .4byte gStaticData_0816B2F8
_0800D370: .4byte gUnknown_030012D8
_0800D374:
	mov r8, sl
_0800D376:
	ldr r2, _0800D3E4 @ =gStaticData_0816BC98
	mov r1, r8
	adds r1, #0x4e
	ldr r4, [sp, #0x78]
	lsls r3, r4, #2
	ldrb r5, [r1]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r3, r0
	adds r0, r0, r2
	ldr r6, [r0]
	ldr r0, _0800D3E8 @ =gUnknown_030012D8
	ldr r2, [r0]
	adds r4, r2, #0
	adds r4, #0x92
	mov sb, r1
	str r3, [sp, #0x98]
	ldrb r4, [r4]
	cmp r4, #4
	ble _0800D3A2
	movs r6, #0
_0800D3A2:
	add r1, sp, #0x4c
	ldrb r0, [r1]
	cmp r0, #0
	beq _0800D43A
	adds r0, r2, #0
	adds r0, #0x94
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800D43A
	movs r3, #0
	cmp r3, r0
	bge _0800D43A
_0800D3BA:
	ldr r4, _0800D3E8 @ =gUnknown_030012D8
	ldr r2, [r4]
	adds r0, r2, #0
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #0
	bne _0800D3EC
	cmp r3, #4
	ble _0800D3D6
	adds r0, r2, #0
	adds r0, #0x94
	ldrb r0, [r0]
	cmp r3, r0
	bge _0800D3EC
_0800D3D6:
	lsls r1, r3, #2
	adds r0, r2, #0
	adds r0, #0x98
	adds r0, r0, r1
	ldr r5, [r0]
	b _0800D3EE
	.align 2, 0
_0800D3E4: .4byte gStaticData_0816BC98
_0800D3E8: .4byte gUnknown_030012D8
_0800D3EC:
	movs r5, #0
_0800D3EE:
	adds r7, r3, #1
	cmp r5, #0
	beq _0800D42C
	adds r0, r5, #0
	bl sub_8010708
	adds r4, r0, #0
	cmp r4, #0
	beq _0800D416
	b _0800D40A
_0800D402:
	adds r0, r4, #0
	bl sub_8010708
	adds r4, r0, #0
_0800D40A:
	adds r0, r4, #0
	bl sub_8010708
	cmp r0, #0
	bne _0800D402
	b _0800D422
_0800D416:
	adds r4, r5, #0
	b _0800D422
_0800D41A:
	adds r0, r4, #0
	bl sub_801070C
	adds r4, r0, #0
_0800D422:
	cmp r4, #0
	beq _0800D42C
	cmp sl, r4
	bne _0800D41A
	movs r6, #0
_0800D42C:
	adds r3, r7, #0
	ldr r1, _0800D44C @ =gUnknown_030012D8
	ldr r0, [r1]
	adds r0, #0x94
	ldrb r0, [r0]
	cmp r3, r0
	blt _0800D3BA
_0800D43A:
	movs r7, #0
	cmp r6, #5
	bls _0800D442
	b _0800D6AC
_0800D442:
	lsls r0, r6, #2
	ldr r1, _0800D450 @ =_0800D454
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800D44C: .4byte gUnknown_030012D8
_0800D450: .4byte _0800D454
_0800D454: @ jump table
	.4byte _0800D46C @ case 0
	.4byte _0800D46C @ case 1
	.4byte _0800D48A @ case 2
	.4byte _0800D4A8 @ case 3
	.4byte _0800D684 @ case 4
	.4byte _0800D6A2 @ case 5
_0800D46C:
	mov r5, sb
	ldrb r1, [r5]
	cmp r1, #6
	bne _0800D47C
	mov r0, r8
	bl sub_800F2BC
	b _0800D6AC
_0800D47C:
	cmp r1, #3
	beq _0800D482
	b _0800D6AC
_0800D482:
	mov r0, r8
	bl sub_800F368
	b _0800D6AC
_0800D48A:
	mov r1, r8
	adds r1, #0x4d
	movs r0, #0x80
	ldrb r6, [r1]
	orrs r0, r6
	strb r0, [r1]
	ldr r1, _0800D4A4 @ =gUnknown_030012D8
	ldr r0, [r1]
	movs r1, #1
	adds r0, #0x80
	strb r1, [r0]
	b _0800D6AC
	.align 2, 0
_0800D4A4: .4byte gUnknown_030012D8
_0800D4A8:
	mov r0, r8
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_800E7A8
	ldr r2, _0800D534 @ =gUnknown_030012D8
	mov sb, r2
	ldr r1, [r2]
	adds r0, r1, #0
	adds r0, #0x94
	ldrb r0, [r0]
	cmp r0, #0
	bne _0800D56E
	ldr r2, [r1, #0x20]
	adds r1, #0x2d
	ldrb r3, [r1]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	ldr r1, [r2]
	adds r0, r1, r0
	ldr r4, [sp, #0x78]
	cmp r4, #3
	beq _0800D560
	adds r1, r0, #4
	ldr r5, [sp, #0x74]
	str r5, [sp]
	mov r0, sl
	add r2, sp, #0x1c
	ldr r3, [sp, #0x70]
	bl sub_800CEAC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0800D560
	mov r0, r8
	bl sub_801070C
	adds r4, r0, #0
	cmp r4, #0
	beq _0800D56E
	adds r3, r4, #0
	adds r3, #0x4d
	ldrb r5, [r3]
	movs r0, #0x7f
	ands r0, r5
	cmp r0, #1
	beq _0800D56E
	ldr r1, _0800D538 @ =gStaticData_0816BC98
	adds r2, r4, #0
	adds r2, #0x4e
	ldrb r6, [r2]
	lsls r0, r6, #3
	subs r0, r0, r6
	lsls r0, r0, #2
	ldr r2, [sp, #0x98]
	adds r0, r2, r0
	adds r0, r0, r1
	ldr r0, [r0]
	cmp r0, #3
	bne _0800D53C
	adds r0, r4, #0
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_800E7A8
	b _0800D56E
	.align 2, 0
_0800D534: .4byte gUnknown_030012D8
_0800D538: .4byte gStaticData_0816BC98
_0800D53C:
	cmp r0, #2
	bne _0800D552
	movs r0, #0x80
	orrs r0, r5
	strb r0, [r3]
	mov r3, sb
	ldr r0, [r3]
	movs r1, #1
	adds r0, #0x80
	strb r1, [r0]
	b _0800D56E
_0800D552:
	cmp r0, #4
	bne _0800D56E
	adds r0, r4, #0
	movs r1, #1
	bl sub_800EEF0
	b _0800D56E
_0800D560:
	ldr r0, _0800D5BC @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x24
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800D56E
	adds r7, #2
_0800D56E:
	add r4, sp, #0x4c
	ldrb r0, [r4]
	cmp r0, #0
	bne _0800D57A
	bl _0800E074
_0800D57A:
	ldr r5, _0800D5BC @ =gUnknown_030012D8
	ldr r0, [r5]
	ldr r1, [r0]
	asrs r1, r1, #8
	mov r6, sl
	ldr r0, [r6]
	asrs r0, r0, #8
	cmp r1, r0
	bge _0800D5C0
	ldr r0, [sp, #0x78]
	cmp r0, #3
	bne _0800D59C
	mov r0, r8
	bl sub_801070C
	cmp r0, #0
	beq _0800D5F2
_0800D59C:
	ldr r0, [r5]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xc
	movs r3, #1
	bl sub_803AD88
	ldr r2, [r5]
	movs r1, #1
	b _0800D5EC
	.align 2, 0
_0800D5BC: .4byte gUnknown_030012D8
_0800D5C0:
	ldr r4, [sp, #0x78]
	cmp r4, #3
	bne _0800D5D0
	mov r0, r8
	bl sub_801070C
	cmp r0, #0
	beq _0800D5F2
_0800D5D0:
	ldr r0, [r5]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r6, #0
	ldrsh r2, [r1, r6]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xc
	movs r3, #2
	bl sub_803AD88
	ldr r2, [r5]
	movs r1, #2
_0800D5EC:
	ldr r0, [r2, #0x74]
	orrs r0, r1
	str r0, [r2, #0x74]
_0800D5F2:
	ldr r0, _0800D680 @ =gUnknown_030012D8
	ldr r3, [r0]
	adds r1, r3, #0
	adds r1, #0x88
	ldrb r1, [r1]
	cmp r1, #0
	bne _0800D614
	adds r0, r3, #0
	adds r0, #0x94
	ldrb r1, [r0]
	cmp r1, #4
	bhi _0800D614
	lsls r1, r1, #2
	adds r0, #4
	adds r0, r0, r1
	mov r2, r8
	str r2, [r0]
_0800D614:
	ldr r3, _0800D680 @ =gUnknown_030012D8
	ldr r1, [r3]
	adds r0, r1, #0
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #0
	bne _0800D62A
	adds r1, #0x94
	ldrb r0, [r1]
	adds r0, #1
	strb r0, [r1]
_0800D62A:
	ldr r4, _0800D680 @ =gUnknown_030012D8
	ldr r0, [r4]
	adds r1, r0, #0
	adds r1, #0x92
	ldrb r0, [r1]
	cmp r0, #0
	bne _0800D63C
	movs r0, #1
	strb r0, [r1]
_0800D63C:
	cmp r7, #0
	bne _0800D644
	bl _0800E074
_0800D644:
	ldr r2, _0800D680 @ =gUnknown_030012D8
_0800D646:
	ldr r3, [r2]
	adds r0, r3, #0
	adds r0, #0x88
	ldrb r4, [r0]
	cmp r4, #0
	bne _0800D662
	adds r0, #0xc
	ldrb r5, [r0]
	cmp r5, #4
	bhi _0800D662
	lsls r1, r5, #2
	adds r0, #4
	adds r0, r0, r1
	str r4, [r0]
_0800D662:
	ldr r1, [r2]
	adds r0, r1, #0
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #0
	bne _0800D676
	adds r1, #0x94
	ldrb r0, [r1]
	adds r0, #1
	strb r0, [r1]
_0800D676:
	subs r7, #1
	cmp r7, #0
	bne _0800D646
	bl _0800E074
	.align 2, 0
_0800D680: .4byte gUnknown_030012D8
_0800D684:
	mov r1, r8
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _0800D696
	bl _0800E074
_0800D696:
	mov r0, r8
	movs r1, #1
	bl sub_800EEF0
	bl _0800E074
_0800D6A2:
	mov r0, r8
	bl sub_800E6B0
	bl _0800E074
_0800D6AC:
	ldr r6, [sp, #0x78]
	cmp r6, #5
	bne _0800D6C8
	ldr r0, _0800D6C4 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x24
	ldrb r0, [r0]
	cmp r0, #4
	bne _0800D6D2
	movs r0, #2
	str r0, [sp, #0x78]
	b _0800D6D2
	.align 2, 0
_0800D6C4: .4byte gUnknown_030012D8
_0800D6C8:
	ldr r1, [sp, #0x78]
	cmp r1, #3
	bne _0800D6D2
	movs r2, #1
	str r2, [sp, #0x78]
_0800D6D2:
	mov r1, sl
	adds r1, #0x58
	ldrb r0, [r1]
	cmp r0, #0
	beq _0800D6EC
	movs r0, #0
	strb r0, [r1]
	mov r3, sl
	ldr r0, [r3, #0x44]
	cmp r0, #0
	bne _0800D6EC
	bl _0800E074
_0800D6EC:
	add r2, sp, #0x2c
	adds r1, r2, #0
	add r0, sp, #0x1c
	ldm r0!, {r4, r5, r6}
	stm r1!, {r4, r5, r6}
	ldr r0, [r0]
	str r0, [r1]
	ldr r0, [sp, #0x78]
	cmp r0, #6
	beq _0800D708
	mov r0, sl
	add r1, sp, #0x1c
	bl sub_800E4E4
_0800D708:
	ldr r6, _0800D7F8 @ =gUnknown_030012D8
	ldr r0, [r6]
	ldr r2, [r0, #0x20]
	adds r0, #0x2d
	ldrb r3, [r0]
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	ldr r0, [r2]
	adds r0, r0, r1
	adds r4, r0, #4
	mov r8, r4
	movs r5, #4
	ldrsh r1, [r0, r5]
	movs r0, #2
	ldrsh r2, [r4, r0]
	ldrb r4, [r4, #4]
	mov r3, r8
	ldrb r5, [r3, #5]
	ldr r0, [sp, #0x70]
	adds r1, r1, r0
	ldr r3, [sp, #0x74]
	adds r2, r2, r3
	add r0, sp, #0x3c
	bl sub_803AFE4
	add r0, sp, #0x3c
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_803AFDC
	ldr r0, [r6]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _0800D760
	ldr r4, [sp, #0x70]
	lsls r0, r4, #1
	ldr r1, [sp, #0x3c]
	ldr r2, [sp, #0x44]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #0x3c]
_0800D760:
	ldr r0, [r6]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1a
	cmp r0, #0
	bge _0800D77A
	ldr r5, [sp, #0x74]
	lsls r0, r5, #1
	ldr r1, [sp, #0x40]
	ldr r2, [sp, #0x48]
	adds r1, r1, r2
	subs r0, r0, r1
	str r0, [sp, #0x40]
_0800D77A:
	add r4, sp, #0x3c
	add r0, sp, #0x1c
	adds r1, r4, #0
	bl sub_8001640
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0800D78E
	bl _0800E074
_0800D78E:
	movs r0, #0
	str r0, [sp, #0x7c]
	movs r1, #0
	str r1, [sp, #0x88]
	ldr r1, [sp, #0x40]
	ldr r0, [sp, #0x20]
	cmp r1, r0
	bge _0800D7A2
	movs r2, #1
	str r2, [sp, #0x88]
_0800D7A2:
	mov r3, sl
	ldr r0, [r3, #0x44]
	cmp r0, #0
	bne _0800D7AC
	b _0800DA84
_0800D7AC:
	add r0, sp, #0x2c
	add r1, sp, #0x3c
	bl sub_8001640
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0800D7BE
	bl _0800E074
_0800D7BE:
	ldr r4, _0800D7FC @ =gStaticData_0816BBDA
	mov r0, sl
	adds r0, #0x4e
	ldrb r5, [r0]
	adds r1, r5, r4
	ldrb r1, [r1]
	adds r7, r0, #0
	cmp r1, #0
	bne _0800D7D2
	b _0800DA34
_0800D7D2:
	ldr r0, [r6]
	bl sub_8009EC4
	ldr r1, [sp, #0x70]
	ldr r0, [r6]
	ldr r1, [r0]
	asrs r1, r1, #8
	mov r2, sl
	ldr r0, [r2]
	asrs r0, r0, #8
	cmp r1, r0
	bge _0800D800
	movs r3, #1
	str r3, [sp, #0x80]
	ldr r0, [sp, #0x3c]
	ldr r1, [sp, #0x44]
	ldr r2, [sp, #0x2c]
	b _0800D80A
	.align 2, 0
_0800D7F8: .4byte gUnknown_030012D8
_0800D7FC: .4byte gStaticData_0816BBDA
_0800D800:
	movs r4, #2
	str r4, [sp, #0x80]
	ldr r0, [sp, #0x2c]
	ldr r1, [sp, #0x34]
	ldr r2, [sp, #0x3c]
_0800D80A:
	adds r0, r0, r1
	subs r0, r0, r2
	adds r5, r0, #1
	ldr r2, _0800D830 @ =gUnknown_030012D8
	ldr r0, [r2]
	ldr r1, [r0, #4]
	asrs r1, r1, #8
	mov r6, sl
	ldr r0, [r6, #4]
	asrs r0, r0, #8
	cmp r1, r0
	ble _0800D834
	movs r0, #4
	str r0, [sp, #0x84]
	ldr r0, [sp, #0x30]
	ldr r1, [sp, #0x38]
	ldr r2, [sp, #0x40]
	b _0800D83E
	.align 2, 0
_0800D830: .4byte gUnknown_030012D8
_0800D834:
	movs r1, #8
	str r1, [sp, #0x84]
	ldr r0, [sp, #0x40]
	ldr r1, [sp, #0x48]
	ldr r2, [sp, #0x30]
_0800D83E:
	adds r0, r0, r1
	subs r0, r0, r2
	mov sb, r0
	cmp r5, #5
	ble _0800D8E8
	ldr r2, [sp, #0x88]
	cmp r2, #0
	bne _0800D8E8
	ldr r0, _0800D8B0 @ =gUnknown_030012C0
	ldr r1, [r0]
	ldr r1, [r1, #0x78]
	adds r4, r0, #0
	cmp r1, #0
	bne _0800D880
	ldr r3, _0800D8B4 @ =gUnknown_030012D8
	ldr r2, [r3]
	ldrb r5, [r2, #0xc]
	lsrs r0, r5, #6
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _0800D880
	movs r3, #0
	adds r0, r2, #0
	adds r0, #0x8c
	ldr r1, _0800D8B8 @ =gUnknown_0300082C
	ldr r2, [r0]
	ldr r0, [r1]
	cmp r2, r0
	bls _0800D87C
	movs r3, #1
_0800D87C:
	cmp r3, #0
	beq _0800D886
_0800D880:
	ldrb r7, [r7]
	cmp r7, #0xd
	beq _0800D8BC
_0800D886:
	ldr r6, _0800D8B4 @ =gUnknown_030012D8
	ldr r1, [r6]
	movs r0, #0x40
	ldrb r2, [r1, #0xc]
	orrs r0, r2
	strb r0, [r1, #0xc]
	ldr r0, [r4]
	movs r1, #0
	bl sub_80231EC
	ldr r0, [r6]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xa
	b _0800D8DC
	.align 2, 0
_0800D8B0: .4byte gUnknown_030012C0
_0800D8B4: .4byte gUnknown_030012D8
_0800D8B8: .4byte gUnknown_0300082C
_0800D8BC:
	mov r0, sl
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_800E7A8
	ldr r0, _0800D8E4 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r4, #0
	ldrsh r2, [r1, r4]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #1
_0800D8DC:
	movs r3, #0
	bl sub_803AD88
	b _0800E074
	.align 2, 0
_0800D8E4: .4byte gUnknown_030012D8
_0800D8E8:
	cmp r5, #6
	ble _0800D940
	mov r6, sb
	cmp r6, #1
	ble _0800D93C
	ldr r0, [sp, #0x88]
	cmp r0, #0
	beq _0800D93C
	ldr r2, _0800D938 @ =gUnknown_030012D8
	ldr r1, [r2]
	ldr r0, [r1]
	str r0, [sp, #0x50]
	ldr r1, [r1, #4]
	add r2, sp, #0x50
	mov r0, sb
	subs r0, #1
	lsls r0, r0, #8
	subs r1, r1, r0
	str r1, [r2, #4]
	ldr r3, _0800D938 @ =gUnknown_030012D8
	ldr r0, [r3]
	movs r1, #0
	str r1, [r0, #0x64]
	ldr r1, [sp, #0x50]
	ldr r2, [r2, #4]
	bl sub_8007398
	ldr r4, _0800D938 @ =gUnknown_030012D8
	ldr r0, [r4]
	movs r5, #0x84
	lsls r5, r5, #1
	adds r0, r0, r5
	movs r1, #1
	strb r1, [r0, #4]
	ldr r1, [r4]
	ldr r0, [r1, #0x74]
	ldr r6, [sp, #0x84]
	orrs r0, r6
	str r0, [r1, #0x74]
	b _0800E074
	.align 2, 0
_0800D938: .4byte gUnknown_030012D8
_0800D93C:
	cmp r5, #6
	bgt _0800D9B4
_0800D940:
	mov r0, sb
	cmp r0, #2
	ble _0800D9B4
	ldr r2, _0800D964 @ =gUnknown_030012D8
	ldr r1, [r2]
	ldr r0, [r1]
	str r0, [sp, #0x58]
	ldr r1, [r1, #4]
	add r0, sp, #0x58
	str r1, [r0, #4]
	adds r2, r0, #0
	ldr r3, [sp, #0x80]
	cmp r3, #2
	bne _0800D968
	lsls r0, r5, #8
	ldr r1, [sp, #0x58]
	adds r0, r0, r1
	b _0800D974
	.align 2, 0
_0800D964: .4byte gUnknown_030012D8
_0800D968:
	ldr r4, [sp, #0x80]
	cmp r4, #1
	bne _0800D976
	lsls r1, r5, #8
	ldr r0, [sp, #0x58]
	subs r0, r0, r1
_0800D974:
	str r0, [sp, #0x58]
_0800D976:
	ldr r5, _0800D9B0 @ =gUnknown_030012D8
	ldr r0, [r5]
	ldr r1, [sp, #0x58]
	ldr r2, [r2, #4]
	bl sub_8007398
	ldr r0, [r5]
	movs r6, #0x84
	lsls r6, r6, #1
	adds r0, r0, r6
	movs r1, #1
	strb r1, [r0, #4]
	ldr r0, [r5]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xc
	ldr r3, [sp, #0x80]
	bl sub_803AD88
	ldr r1, [r5]
	ldr r0, [r1, #0x74]
	ldr r4, [sp, #0x80]
	b _0800DA28
	.align 2, 0
_0800D9B0: .4byte gUnknown_030012D8
_0800D9B4:
	ldr r5, _0800D9E0 @ =gUnknown_030012D8
	ldr r1, [r5]
	adds r0, r1, #0
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #1
	beq _0800D9C4
	b _0800E074
_0800D9C4:
	ldr r0, [r1]
	str r0, [sp, #0x60]
	ldr r1, [r1, #4]
	add r0, sp, #0x60
	str r1, [r0, #4]
	adds r2, r0, #0
	ldr r6, [sp, #0x84]
	cmp r6, #4
	bne _0800D9E4
	mov r3, sb
	lsls r0, r3, #8
	adds r0, r0, r1
	b _0800D9F0
	.align 2, 0
_0800D9E0: .4byte gUnknown_030012D8
_0800D9E4:
	ldr r4, [sp, #0x80]
	cmp r4, #8
	bne _0800D9F2
	mov r5, sb
	lsls r0, r5, #8
	subs r0, r1, r0
_0800D9F0:
	str r0, [r2, #4]
_0800D9F2:
	ldr r6, _0800DA30 @ =gUnknown_030012D8
	ldr r0, [r6]
	ldr r1, [sp, #0x60]
	ldr r2, [r2, #4]
	bl sub_8007398
	ldr r0, [r6]
	movs r1, #0x84
	lsls r1, r1, #1
	adds r0, r0, r1
	movs r1, #1
	strb r1, [r0, #4]
	ldr r0, [r6]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xc
	ldr r3, [sp, #0x84]
	bl sub_803AD88
	ldr r1, [r6]
	ldr r0, [r1, #0x74]
	ldr r4, [sp, #0x84]
_0800DA28:
	orrs r0, r4
	str r0, [r1, #0x74]
	b _0800E074
	.align 2, 0
_0800DA30: .4byte gUnknown_030012D8
_0800DA34:
	mov r0, sl
	b _0800DA58
_0800DA38:
	adds r0, r2, #0
	adds r0, #0x4e
	ldrb r0, [r0]
	adds r0, r0, r4
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800DA56
	adds r1, r2, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800DA56
	b _0800E074
_0800DA56:
	adds r0, r2, #0
_0800DA58:
	bl sub_8010708
	adds r2, r0, #0
	cmp r2, #0
	bne _0800DA38
	ldr r1, _0800DA80 @ =gStaticData_0816BC98
	ldrb r5, [r7]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r1, #0x14
	adds r0, r0, r1
	ldr r6, [r0]
	movs r5, #0
	mov sb, r5
	movs r0, #0
	str r0, [sp, #0x80]
	movs r1, #0
	str r1, [sp, #0x84]
	b _0800DD20
	.align 2, 0
_0800DA80: .4byte gStaticData_0816BC98
_0800DA84:
	ldr r0, [r6]
	bl sub_8009EC4
	adds r4, r0, #0
	ldr r0, [r6]
	bl sub_8009EBC
	adds r7, r0, #0
	movs r2, #2
	mov ip, r2
	ldr r3, [sp, #0x70]
	cmp r3, r4
	ble _0800DAA2
	movs r5, #1
	mov ip, r5
_0800DAA2:
	ldr r0, [r6]
	ldr r1, [r0]
	asrs r1, r1, #8
	mov r6, sl
	ldr r0, [r6]
	asrs r0, r0, #8
	cmp r1, r0
	bge _0800DAC8
	movs r0, #1
	str r0, [sp, #0x80]
	ldr r0, [sp, #0x3c]
	ldr r1, [sp, #0x44]
	ldr r2, [sp, #0x1c]
	adds r1, r0, r1
	subs r1, r1, r2
	adds r5, r1, #1
	adds r6, r2, #0
	adds r3, r0, #0
	b _0800DADC
_0800DAC8:
	movs r1, #2
	str r1, [sp, #0x80]
	ldr r0, [sp, #0x1c]
	ldr r1, [sp, #0x24]
	ldr r2, [sp, #0x3c]
	adds r1, r0, r1
	subs r1, r1, r2
	adds r5, r1, #1
	adds r6, r0, #0
	adds r3, r2, #0
_0800DADC:
	ldr r0, _0800DB04 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r1, [r0, #4]
	asrs r1, r1, #8
	mov r2, sl
	ldr r0, [r2, #4]
	asrs r0, r0, #8
	cmp r1, r0
	ble _0800DB08
	movs r0, #4
	str r0, [sp, #0x84]
	ldr r0, [sp, #0x20]
	ldr r1, [sp, #0x28]
	ldr r2, [sp, #0x40]
	adds r1, r0, r1
	subs r1, r1, r2
	mov sb, r1
	adds r2, r0, #0
	b _0800DB18
	.align 2, 0
_0800DB04: .4byte gUnknown_030012D8
_0800DB08:
	movs r1, #8
	str r1, [sp, #0x84]
	ldr r0, [sp, #0x40]
	ldr r1, [sp, #0x48]
	ldr r2, [sp, #0x20]
	adds r0, r0, r1
	subs r0, r0, r2
	mov sb, r0
_0800DB18:
	ldr r0, [sp, #0x74]
	cmp r7, r0
	bne _0800DB6C
	ldr r1, [sp, #0x70]
	cmp r4, r1
	bne _0800DB3E
	mov r2, sb
	cmp r2, #2
	ble _0800DB2C
	b _0800DD1A
_0800DB2C:
	movs r3, #4
	str r3, [sp, #0x7c]
	ldr r4, [sp, #0x88]
	cmp r4, #0
	bne _0800DB38
	b _0800DD1E
_0800DB38:
	movs r6, #8
	str r6, [sp, #0x7c]
	b _0800DD1E
_0800DB3E:
	mov r1, sb
	cmp r1, #2
	ble _0800DB46
	b _0800DD14
_0800DB46:
	cmp r5, #3
	bgt _0800DB5A
	ldr r0, _0800DB68 @ =gUnknown_030012D8
	ldr r0, [r0]
	bl sub_800B324
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0800DB5A
	b _0800DD14
_0800DB5A:
	movs r2, #4
	str r2, [sp, #0x7c]
	ldr r3, [sp, #0x88]
	cmp r3, #0
	bne _0800DB66
	b _0800DD1E
_0800DB66:
	b _0800DC54
	.align 2, 0
_0800DB68: .4byte gUnknown_030012D8
_0800DB6C:
	ldr r0, [sp, #0x70]
	cmp r4, r0
	bne _0800DB92
	ldr r1, [sp, #0x84]
	str r1, [sp, #0x7c]
	mov r2, sb
	cmp r2, #2
	bgt _0800DB7E
	b _0800DD1E
_0800DB7E:
	ldr r3, [sp, #0x80]
	str r3, [sp, #0x7c]
	cmp r2, #7
	ble _0800DB88
	b _0800DD1E
_0800DB88:
	cmp r5, #3
	bgt _0800DB8E
	b _0800DD1E
_0800DB8E:
	str r1, [sp, #0x7c]
	b _0800DD1E
_0800DB92:
	ldr r0, [sp, #0x74]
	cmp r7, r0
	bgt _0800DC5A
	ldr r1, [sp, #0x88]
	cmp r1, #0
	beq _0800DC5A
	mov r0, r8
	movs r1, #2
	ldrsh r0, [r0, r1]
	str r0, [sp, #0x9c]
	mov r0, r8
	ldrb r1, [r0, #5]
	ldr r0, [sp, #0x9c]
	adds r0, r0, r1
	str r0, [sp, #0x9c]
	adds r7, r7, r0
	ldr r0, [sp, #0x28]
	adds r0, r2, r0
	str r1, [sp, #0xa0]
	cmp r7, r0
	ble _0800DBBE
	b _0800DD1A
_0800DBBE:
	movs r1, #0
	str r1, [sp, #0x7c]
	mov r2, ip
	cmp r2, #1
	bne _0800DBDA
	ldr r0, [sp, #0x44]
	adds r0, r3, r0
	cmp r0, r6
	blt _0800DBEA
	cmp r5, #4
	ble _0800DBEA
	movs r3, #8
	str r3, [sp, #0x7c]
	b _0800DD1E
_0800DBDA:
	ldr r0, [sp, #0x24]
	adds r0, r6, r0
	cmp r3, r0
	bgt _0800DBEA
	cmp r5, #4
	ble _0800DBEA
	movs r0, #8
	str r0, [sp, #0x7c]
_0800DBEA:
	ldr r1, [sp, #0x7c]
	cmp r1, #0
	beq _0800DBF2
	b _0800DD1E
_0800DBF2:
	mov r2, r8
	movs r1, #2
	ldrsh r0, [r2, r1]
	ldr r2, [sp, #0xa0]
	adds r0, r0, r2
	ldr r1, [sp, #0x74]
	adds r1, r1, r0
	str r1, [sp, #0x74]
	ldr r2, [sp, #0x80]
	cmp r2, #1
	bne _0800DC1E
	mov r1, r8
	movs r2, #0
	ldrsh r0, [r1, r2]
	ldrb r1, [r1, #4]
	adds r0, r1, r0
	adds r4, r4, r0
	ldr r0, [sp, #0x44]
	adds r3, r3, r0
	str r3, [sp, #0x70]
	str r6, [sp]
	b _0800DC2E
_0800DC1E:
	mov r2, r8
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r4, r4, r0
	str r3, [sp, #0x70]
	ldr r0, [sp, #0x24]
	adds r0, r6, r0
	str r0, [sp]
_0800DC2E:
	adds r0, r4, #0
	adds r1, r7, #0
	adds r2, r3, #0
	ldr r3, [sp, #0x74]
	bl sub_800FDC8
	adds r2, r0, #0
	cmp r2, #0
	bge _0800DC4A
	ldr r3, [sp, #0x88]
	cmp r3, #0
	beq _0800DC4A
	cmp r5, #4
	bgt _0800DC54
_0800DC4A:
	cmp r2, #0
	ble _0800DD14
	ldr r0, [sp, #0x20]
	cmp r2, r0
	bgt _0800DD14
_0800DC54:
	movs r4, #8
	str r4, [sp, #0x7c]
	b _0800DD1E
_0800DC5A:
	mov r1, r8
	movs r0, #2
	ldrsh r1, [r1, r0]
	adds r7, r7, r1
	cmp r7, r2
	blt _0800DD1A
	movs r1, #0
	str r1, [sp, #0x7c]
	mov r2, ip
	cmp r2, #1
	bne _0800DC82
	ldr r0, [sp, #0x44]
	adds r0, r3, r0
	cmp r0, r6
	blt _0800DC92
	cmp r5, #5
	ble _0800DC92
	movs r3, #4
	str r3, [sp, #0x7c]
	b _0800DD1E
_0800DC82:
	ldr r0, [sp, #0x24]
	adds r0, r6, r0
	cmp r3, r0
	bgt _0800DC92
	cmp r5, #5
	ble _0800DC92
	movs r0, #4
	str r0, [sp, #0x7c]
_0800DC92:
	ldr r1, [sp, #0x7c]
	cmp r1, #0
	bne _0800DD1E
	mov r2, r8
	movs r1, #2
	ldrsh r0, [r2, r1]
	ldr r2, [sp, #0x74]
	adds r2, r2, r0
	str r2, [sp, #0x74]
	ldr r0, [sp, #0x80]
	cmp r0, #1
	bne _0800DCC0
	mov r1, r8
	movs r2, #0
	ldrsh r0, [r1, r2]
	ldrb r1, [r1, #4]
	adds r0, r1, r0
	adds r4, r4, r0
	ldr r0, [sp, #0x44]
	adds r3, r3, r0
	str r3, [sp, #0x70]
	str r6, [sp]
	b _0800DCD0
_0800DCC0:
	mov r2, r8
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r4, r4, r0
	str r3, [sp, #0x70]
	ldr r0, [sp, #0x24]
	adds r0, r6, r0
	str r0, [sp]
_0800DCD0:
	adds r0, r4, #0
	adds r1, r7, #0
	adds r2, r3, #0
	ldr r3, [sp, #0x74]
	bl sub_800FDC8
	adds r2, r0, #0
	ldr r0, _0800DD10 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #1
	bne _0800DCEC
	adds r2, #2
_0800DCEC:
	cmp r2, #0
	bge _0800DCFA
	ldr r3, [sp, #0x88]
	cmp r3, #0
	bne _0800DCFA
	cmp r5, #5
	bgt _0800DD08
_0800DCFA:
	cmp r2, #0
	ble _0800DD14
	ldr r0, [sp, #0x20]
	ldr r1, [sp, #0x28]
	adds r0, r0, r1
	cmp r2, r0
	blt _0800DD14
_0800DD08:
	movs r4, #4
	str r4, [sp, #0x7c]
	b _0800DD1E
	.align 2, 0
_0800DD10: .4byte gUnknown_030012D8
_0800DD14:
	ldr r6, [sp, #0x80]
	str r6, [sp, #0x7c]
	b _0800DD1E
_0800DD1A:
	ldr r0, [sp, #0x80]
	str r0, [sp, #0x7c]
_0800DD1E:
	movs r6, #0
_0800DD20:
	ldr r0, _0800DD64 @ =gUnknown_030012D8
	ldr r1, [r0]
	ldr r0, [r1]
	str r0, [sp, #0x68]
	ldr r1, [r1, #4]
	add r0, sp, #0x68
	str r1, [r0, #4]
	adds r7, r0, #0
	cmp r5, #0
	bge _0800DD36
	movs r5, #0
_0800DD36:
	mov r1, sb
	cmp r1, #0
	bge _0800DD40
	movs r2, #0
	mov sb, r2
_0800DD40:
	movs r3, #0
	str r3, [sp, #0x8c]
	ldr r0, _0800DD68 @ =gStaticData_0816BF00
	ldr r4, [sp, #0x78]
	adds r0, r4, r0
	ldrb r0, [r0]
	str r0, [sp, #0x90]
	mov r8, sl
	ldr r0, [sp, #0x7c]
	cmp r0, #8
	bls _0800DD58
	b _0800E00C
_0800DD58:
	lsls r0, r0, #2
	ldr r1, _0800DD6C @ =_0800DD70
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800DD64: .4byte gUnknown_030012D8
_0800DD68: .4byte gStaticData_0816BF00
_0800DD6C: .4byte _0800DD70
_0800DD70: @ jump table
	.4byte _0800E00C @ case 0
	.4byte _0800DEAC @ case 1
	.4byte _0800DEAC @ case 2
	.4byte _0800E00C @ case 3
	.4byte _0800DD94 @ case 4
	.4byte _0800E00C @ case 5
	.4byte _0800E00C @ case 6
	.4byte _0800E00C @ case 7
	.4byte _0800DE14 @ case 8
_0800DD94:
	mov r0, sl
	bl sub_801095C
	mov r8, r0
	ldr r2, _0800DE0C @ =gStaticData_0816BC98
	adds r0, #0x4e
	ldrb r1, [r0]
	lsls r0, r1, #3
	subs r0, r0, r1
	ldr r3, [sp, #0x78]
	adds r0, r0, r3
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r6, [r0]
	cmp r1, #4
	bne _0800DDBA
	cmp r3, #2
	bne _0800DDBA
	movs r6, #3
_0800DDBA:
	ldr r4, [sp, #0x78]
	cmp r4, #3
	ble _0800DDD0
	cmp r4, #6
	beq _0800DDD0
	cmp r4, #4
	beq _0800DDCA
	b _0800E00C
_0800DDCA:
	cmp r6, #2
	ble _0800DDD0
	b _0800E00C
_0800DDD0:
	ldr r5, _0800DE10 @ =gUnknown_030012D8
	ldr r0, [r5]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xc
	movs r3, #4
	bl sub_803AD88
	ldr r1, [r5]
	movs r2, #4
	ldr r0, [r1, #0x74]
	orrs r0, r2
	str r0, [r1, #0x74]
	adds r1, #0x68
	ldrb r1, [r1]
	cmp r1, #8
	bne _0800DDFE
	b _0800E00C
_0800DDFE:
	mov r4, sb
	lsls r0, r4, #8
	ldr r1, [r7, #4]
	adds r0, r0, r1
	str r0, [r7, #4]
	b _0800E00C
	.align 2, 0
_0800DE0C: .4byte gStaticData_0816BC98
_0800DE10: .4byte gUnknown_030012D8
_0800DE14:
	mov r0, sl
	bl sub_8010914
	mov r8, r0
	adds r0, #0x4e
	ldrb r2, [r0]
	ldr r1, _0800DE80 @ =gStaticData_0816BC98
	lsls r0, r2, #3
	subs r0, r0, r2
	ldr r5, [sp, #0x78]
	adds r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r6, [r0]
	cmp r5, #4
	bne _0800DE52
	cmp r2, #0xa
	beq _0800DE52
	ldr r0, _0800DE84 @ =gUnknown_030012D8
	ldr r1, [r0]
	adds r0, r1, #0
	adds r0, #0x94
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800DE52
	movs r0, #0
	str r0, [r1, #0x64]
	str r0, [r1, #0x54]
	str r0, [r1, #0x58]
	str r0, [r1, #0x5c]
	movs r6, #1
_0800DE52:
	subs r0, r6, #1
	cmp r0, #1
	bhi _0800DE8C
	mov r0, sb
	subs r0, #1
	lsls r0, r0, #8
	ldr r2, [r7, #4]
	subs r2, r2, r0
	ldr r0, _0800DE88 @ =0xFFFFFF00
	ands r2, r0
	str r2, [r7, #4]
	ldr r4, _0800DE84 @ =gUnknown_030012D8
	ldr r0, [r4]
	ldr r1, [sp, #0x68]
	bl sub_8007398
	ldr r0, [r4]
	movs r1, #0x84
	lsls r1, r1, #1
	adds r0, r0, r1
	movs r1, #1
	strb r1, [r0, #4]
	b _0800DE9E
	.align 2, 0
_0800DE80: .4byte gStaticData_0816BC98
_0800DE84: .4byte gUnknown_030012D8
_0800DE88: .4byte 0xFFFFFF00
_0800DE8C:
	cmp r6, #0
	beq _0800DE94
	cmp r6, #2
	bne _0800DE9E
_0800DE94:
	mov r2, sb
	lsls r1, r2, #8
	ldr r0, [r7, #4]
	subs r0, r0, r1
	str r0, [r7, #4]
_0800DE9E:
	ldr r0, [r7, #4]
	ldr r1, _0800DEA8 @ =0xFFFFFF00
	ands r0, r1
	str r0, [r7, #4]
	b _0800E00C
	.align 2, 0
_0800DEA8: .4byte 0xFFFFFF00
_0800DEAC:
	ldr r3, [sp, #0x80]
	str r3, [sp, #0x8c]
	add r0, sp, #0x2c
	add r1, sp, #0x3c
	bl sub_8001640
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0800DECC
	movs r6, #0
	mov r0, sl
	bl sub_800E494
	movs r4, #0
	str r4, [sp, #0x8c]
	b _0800DF9A
_0800DECC:
	movs r0, #0x7f
	ldr r1, [sp, #0x94]
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800DEF4
	ldr r2, [sp, #0x8c]
	cmp r2, #2
	bne _0800DEE6
	lsls r1, r5, #8
	ldr r0, [sp, #0x68]
	adds r0, r0, r1
	b _0800DEF2
_0800DEE6:
	ldr r3, [sp, #0x8c]
	cmp r3, #1
	bne _0800DEF4
	lsls r1, r5, #8
	ldr r0, [sp, #0x68]
	subs r0, r0, r1
_0800DEF2:
	str r0, [sp, #0x68]
_0800DEF4:
	ldr r4, [sp, #0x78]
	cmp r4, #2
	ble _0800DF34
	ldr r1, _0800DF2C @ =gStaticData_0816BC98
	mov r2, sl
	adds r2, #0x4e
	ldrb r5, [r2]
	lsls r0, r5, #3
	subs r0, r0, r5
	adds r0, r0, r4
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r6, [r0]
	cmp r4, #4
	bne _0800DF18
	cmp r6, #2
	bne _0800DF18
	movs r6, #0
_0800DF18:
	ldr r0, [sp, #0x78]
	cmp r0, #5
	bne _0800DF9A
	cmp r6, #3
	bne _0800DF9A
	ldr r0, _0800DF30 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0]
	str r0, [sp, #0x68]
	b _0800DF9A
	.align 2, 0
_0800DF2C: .4byte gStaticData_0816BC98
_0800DF30: .4byte gUnknown_030012D8
_0800DF34:
	mov r1, sb
	cmp r1, #4
	bgt _0800DF68
	cmp r5, #3
	ble _0800DF68
	ldr r2, [sp, #0x88]
	cmp r2, #0
	beq _0800DF68
	ldr r1, _0800DF64 @ =gStaticData_0816BC98
	mov r2, sl
	adds r2, #0x4e
	ldrb r3, [r2]
	lsls r0, r3, #3
	subs r0, r0, r3
	ldr r4, [sp, #0x78]
	adds r0, r0, r4
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r6, [r0]
	cmp r6, #1
	ble _0800DF9A
	movs r6, #0
	b _0800DF9A
	.align 2, 0
_0800DF64: .4byte gStaticData_0816BC98
_0800DF68:
	ldr r3, _0800E084 @ =gStaticData_0816BC98
	ldr r5, [sp, #0x78]
	lsls r2, r5, #2
	mov r1, sl
	adds r1, #0x4e
	ldrb r4, [r1]
	lsls r0, r4, #3
	subs r0, r0, r4
	lsls r0, r0, #2
	adds r0, r2, r0
	adds r0, r0, r3
	ldr r0, [r0]
	cmp r0, #4
	bne _0800DF9A
	ldr r0, _0800E088 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0]
	str r0, [sp, #0x68]
	ldrb r5, [r1]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r2, r0
	adds r0, r0, r3
	ldr r6, [r0]
_0800DF9A:
	cmp r6, #1
	beq _0800E00C
	ldr r0, [sp, #0x8c]
	cmp r0, #0
	beq _0800E00C
	movs r5, #1
	mov r0, sl
	bl sub_801070C
	adds r4, r0, #0
	mov r0, sl
	bl sub_8010708
	adds r2, r0, #0
	ldr r0, _0800E088 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0, #0x64]
	asrs r1, r0, #8
	ldr r3, [sp, #0x84]
	cmp r3, #8
	bne _0800DFD6
	cmp r4, #0
	bne _0800DFD6
	mov r0, sb
	subs r0, #1
	cmp r1, r0
	bge _0800E00C
	mov r4, sb
	cmp r4, #2
	ble _0800E00C
_0800DFD6:
	ldr r0, [sp, #0x84]
	cmp r0, #4
	bne _0800DFF0
	cmp r2, #0
	bne _0800DFF0
	mov r0, sb
	subs r0, #1
	cmp r1, r0
	bge _0800DFEE
	mov r1, sb
	cmp r1, #2
	bgt _0800DFF0
_0800DFEE:
	movs r5, #0
_0800DFF0:
	cmp r5, #0
	beq _0800E00C
	ldr r4, _0800E088 @ =gUnknown_030012D8
	ldr r0, [r4]
	ldr r1, [sp, #0x68]
	ldr r2, [r7, #4]
	bl sub_8007398
	ldr r0, [r4]
	movs r2, #0x84
	lsls r2, r2, #1
	adds r0, r0, r2
	movs r1, #1
	strb r1, [r0, #4]
_0800E00C:
	ldr r0, _0800E088 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #1
	bne _0800E03C
	mov r0, sl
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #0xe
	bne _0800E03C
	cmp r6, #1
	bgt _0800E03C
	add r0, sp, #0x2c
	add r1, sp, #0x3c
	bl sub_8001640
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #1
	bne _0800E03C
	mov r0, r8
	bl sub_800E620
_0800E03C:
	ldr r0, _0800E088 @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r3, #0x84
	lsls r3, r3, #1
	adds r0, r0, r3
	ldr r4, [sp, #0x7c]
	str r4, [sp]
	mov r5, sb
	str r5, [sp, #4]
	ldr r1, [sp, #0x68]
	ldr r2, [sp, #0x6c]
	str r1, [sp, #8]
	str r2, [sp, #0xc]
	ldr r1, [sp, #0x8c]
	str r1, [sp, #0x10]
	add r1, sp, #0x14
	add r2, sp, #0x90
	ldrb r2, [r2]
	strb r2, [r1]
	add r1, sp, #0x18
	add r3, sp, #0x88
	ldrb r3, [r3]
	strb r3, [r1]
	mov r1, r8
	ldr r2, [sp, #0x78]
	adds r3, r6, #0
	bl sub_8010D54
_0800E074:
	add sp, #0xa4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800E084: .4byte gStaticData_0816BC98
_0800E088: .4byte gUnknown_030012D8

	thumb_func_start sub_800E08C
sub_800E08C: @ 0x0800E08C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xc
	adds r6, r0, #0
	mov r8, r1
	adds r7, r2, #0
	mov sb, r3
	add r0, sp, #0x3c
	add r1, sp, #0x40
	add r2, sp, #0x44
	ldrb r0, [r0]
	str r0, [sp]
	ldrb r1, [r1]
	str r1, [sp, #4]
	ldrb r2, [r2]
	str r2, [sp, #8]
	adds r1, r6, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _0800E0C2
	b _0800E43C
_0800E0C2:
	ldr r0, _0800E13C @ =gUnknown_030012D8
	ldr r2, [r0]
	adds r1, r2, #0
	adds r1, #0x88
	mov sl, r0
	ldrb r1, [r1]
	cmp r1, #1
	bne _0800E0DE
	cmp r7, #2
	ble _0800E0DE
	ldr r0, [r2]
	str r0, [sp, #0x30]
	ldr r0, [r2, #4]
	str r0, [sp, #0x34]
_0800E0DE:
	subs r0, r7, #2
	cmp r0, #1
	bls _0800E0E8
	cmp r7, #5
	bne _0800E1D2
_0800E0E8:
	adds r0, r6, #0
	adds r0, #0x4e
	ldrb r1, [r0]
	mov r0, sl
	ldr r2, [r0]
	adds r0, r2, #0
	adds r0, #0x24
	movs r5, #4
	ldrb r0, [r0]
	ands r5, r0
	cmp r5, #0
	bne _0800E1D2
	cmp r1, #0xd
	beq _0800E1B8
	mov r3, r8
	cmp r3, #2
	bne _0800E170
	cmp r1, #4
	beq _0800E112
	cmp r1, #8
	bne _0800E144
_0800E112:
	ldr r0, _0800E140 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #2
	bl PlaySfx
	mov r5, sl
	ldr r0, [r5]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xe
	movs r3, #8
	bl sub_803AD88
	b _0800E15A
	.align 2, 0
_0800E13C: .4byte gUnknown_030012D8
_0800E140: .4byte gUnknown_030012BC
_0800E144:
	ldr r1, [r2, #0x18]
	adds r1, #0x68
	movs r5, #0
	ldrsh r0, [r1, r5]
	adds r0, r2, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xd
	movs r3, #8
	bl sub_803AD88
_0800E15A:
	ldr r0, _0800E16C @ =gUnknown_030012D8
	ldr r1, [r0]
	movs r0, #0
	str r0, [r1, #0x64]
	str r0, [r1, #0x54]
	str r0, [r1, #0x58]
	str r0, [r1, #0x5c]
	b _0800E1D2
	.align 2, 0
_0800E16C: .4byte gUnknown_030012D8
_0800E170:
	mov r0, r8
	subs r0, #5
	cmp r0, #1
	bhi _0800E1D2
	cmp r1, #8
	bne _0800E1D2
	ldr r0, _0800E1B4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #2
	bl PlaySfx
	mov r1, sl
	ldr r0, [r1]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xe
	movs r3, #8
	bl sub_803AD88
	mov r1, sl
	ldr r0, [r1]
	str r5, [r0, #0x64]
	str r5, [r0, #0x54]
	str r5, [r0, #0x58]
	str r5, [r0, #0x5c]
	b _0800E1D2
	.align 2, 0
_0800E1B4: .4byte gUnknown_030012BC
_0800E1B8:
	cmp r7, #2
	bne _0800E1D2
	adds r1, r6, #0
	adds r1, #0x4d
	movs r0, #0x80
	ldrb r3, [r1]
	orrs r0, r3
	strb r0, [r1]
	movs r1, #1
	adds r0, r2, #0
	adds r0, #0x80
	strb r1, [r0]
	movs r7, #1
_0800E1D2:
	cmp r7, #3
	bne _0800E202
	adds r2, r6, #0
	adds r2, #0x4e
	ldrb r5, [r2]
	cmp r5, #0xf
	bne _0800E202
	ldr r0, [r6, #0x48]
	movs r1, #7
	ands r0, r1
	cmp r0, #3
	bne _0800E202
	movs r0, #0xe
	movs r1, #0
	strb r0, [r2]
	str r1, [r6, #0x48]
	ldr r1, _0800E270 @ =gStaticData_0816BC98
	ldrb r3, [r2]
	lsls r0, r3, #3
	subs r0, r0, r3
	add r0, r8
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r7, [r0]
_0800E202:
	ldr r2, [sp, #8]
	cmp r7, #1
	bne _0800E260
	mov r5, r8
	cmp r5, #4
	bne _0800E260
	ldr r3, _0800E274 @ =gUnknown_030012D8
	ldr r1, [r3]
	adds r4, r1, #0
	adds r4, #0x92
	ldrb r0, [r4]
	cmp r0, #1
	bne _0800E260
	adds r1, #0x24
	movs r0, #0xc
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800E260
	ldr r1, _0800E270 @ =gStaticData_0816BC98
	adds r2, r6, #0
	adds r2, #0x4e
	ldrb r5, [r2]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, #0x10
	adds r0, r0, r1
	ldr r7, [r0]
	movs r0, #2
	strb r0, [r4]
	ldr r0, [r3]
	adds r0, #0x92
	ldrb r1, [r0]
	adds r1, #1
	strb r1, [r0]
	ldr r0, [r3]
	adds r0, #0x92
	ldrb r1, [r0]
	adds r1, #1
	strb r1, [r0]
	ldr r0, [r3]
	adds r0, #0x92
	ldrb r1, [r0]
	adds r1, #1
	strb r1, [r0]
	movs r2, #1
_0800E260:
	cmp r7, #5
	bls _0800E266
	b _0800E43C
_0800E266:
	lsls r0, r7, #2
	ldr r1, _0800E278 @ =_0800E27C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800E270: .4byte gStaticData_0816BC98
_0800E274: .4byte gUnknown_030012D8
_0800E278: .4byte _0800E27C
_0800E27C: @ jump table
	.4byte _0800E294 @ case 0
	.4byte _0800E294 @ case 1
	.4byte _0800E342 @ case 2
	.4byte _0800E37C @ case 3
	.4byte _0800E41C @ case 4
	.4byte _0800E434 @ case 5
_0800E294:
	ldr r0, _0800E330 @ =gUnknown_030012D8
	ldr r1, [r0]
	adds r1, #0x24
	movs r2, #4
	ldrb r1, [r1]
	ands r2, r1
	mov sl, r0
	cmp r2, #0
	bne _0800E2FE
	ldr r0, [sp, #4]
	cmp r0, #0
	beq _0800E2FE
	mov r1, sb
	cmp r1, #8
	bne _0800E2B8
	ldr r2, [sp, #0x2c]
	cmp r2, #1
	ble _0800E2D4
_0800E2B8:
	ldr r3, [sp, #0x2c]
	cmp r3, #1
	bgt _0800E2C2
	cmp r7, #1
	beq _0800E2D4
_0800E2C2:
	ldr r5, [sp, #0x2c]
	cmp r5, #7
	bgt _0800E2FE
	cmp r7, #1
	beq _0800E2CE
	b _0800E43C
_0800E2CE:
	mov r0, sb
	cmp r0, #8
	bne _0800E2FE
_0800E2D4:
	mov r1, sl
	ldr r0, [r1]
	adds r1, r0, #0
	adds r1, #0xac
	str r6, [r1]
	movs r1, #8
	adds r0, #0x68
	strb r1, [r0]
	mov r2, sl
	ldr r0, [r2]
	ldr r1, [r0, #4]
	ldr r0, [sp, #0x2c]
	subs r0, #1
	lsls r0, r0, #8
	subs r1, r1, r0
	str r1, [sp, #0x34]
	movs r3, #0
	str r3, [sp, #0x38]
	ldr r0, [r2]
	ldr r0, [r0]
	str r0, [sp, #0x30]
_0800E2FE:
	cmp r7, #1
	beq _0800E304
	b _0800E43C
_0800E304:
	mov r0, sb
	subs r0, #1
	cmp r0, #1
	bhi _0800E31E
	mov r5, r8
	cmp r5, #1
	bgt _0800E31E
	movs r0, #0
	str r0, [sp, #0x38]
	ldr r0, _0800E330 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0]
	str r0, [sp, #0x30]
_0800E31E:
	adds r0, r6, #0
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #6
	bne _0800E334
	adds r0, r6, #0
	bl sub_800F2BC
	b _0800E43C
	.align 2, 0
_0800E330: .4byte gUnknown_030012D8
_0800E334:
	cmp r0, #3
	beq _0800E33A
	b _0800E43C
_0800E33A:
	adds r0, r6, #0
	bl sub_800F368
	b _0800E43C
_0800E342:
	adds r0, r6, #0
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #0xe
	bne _0800E354
	adds r0, r6, #0
	bl sub_800E620
	b _0800E43C
_0800E354:
	cmp r0, #0xc
	bne _0800E360
	adds r0, r6, #0
	bl sub_800E560
	b _0800E43C
_0800E360:
	adds r1, r6, #0
	adds r1, #0x4d
	movs r0, #0x80
	ldrb r2, [r1]
	orrs r0, r2
	strb r0, [r1]
	ldr r0, _0800E378 @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r1, #1
	adds r0, #0x80
	strb r1, [r0]
	b _0800E43C
	.align 2, 0
_0800E378: .4byte gUnknown_030012D8
_0800E37C:
	mov r0, r8
	subs r0, #5
	cmp r0, #1
	bhi _0800E392
	adds r0, r6, #0
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_800E7A8
	b _0800E47E
_0800E392:
	ldr r0, [r6, #0x44]
	cmp r0, #0
	beq _0800E3A6
	adds r0, r6, #0
	movs r1, #0
	movs r2, #0
	movs r3, #4
	bl sub_800E7A8
	b _0800E47E
_0800E3A6:
	mov r3, r8
	cmp r3, #2
	bne _0800E3BC
	adds r0, r6, #0
	movs r1, #0
	mov r5, sp
	ldrb r2, [r5]
	mov r3, sb
	bl sub_800E7A8
	b _0800E47E
_0800E3BC:
	ldr r4, _0800E418 @ =gUnknown_030012D8
	ldr r0, [r4]
	adds r0, #0x94
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800E3CC
	cmp r2, #0
	beq _0800E47E
_0800E3CC:
	mov r0, sb
	cmp r0, #8
	beq _0800E3D6
	cmp r0, #4
	bne _0800E47E
_0800E3D6:
	adds r0, r6, #0
	movs r1, #0
	movs r2, #0
	mov r3, sb
	bl sub_800E7A8
	ldr r2, [r4]
	adds r0, r2, #0
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #0
	bne _0800E400
	adds r0, r2, #0
	adds r0, #0x94
	ldrb r1, [r0]
	cmp r1, #4
	bhi _0800E400
	lsls r1, r1, #2
	adds r0, #4
	adds r0, r0, r1
	str r6, [r0]
_0800E400:
	ldr r0, _0800E418 @ =gUnknown_030012D8
	ldr r1, [r0]
	adds r0, r1, #0
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #0
	bne _0800E47E
	adds r1, #0x94
	ldrb r0, [r1]
	adds r0, #1
	strb r0, [r1]
	b _0800E47E
	.align 2, 0
_0800E418: .4byte gUnknown_030012D8
_0800E41C:
	adds r1, r6, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800E47E
	adds r0, r6, #0
	movs r1, #1
	bl sub_800EEF0
	b _0800E47E
_0800E434:
	adds r0, r6, #0
	bl sub_800E6B0
	b _0800E47E
_0800E43C:
	ldr r5, _0800E490 @ =gUnknown_030012D8
	ldr r3, [r5]
	movs r2, #0x84
	lsls r2, r2, #1
	adds r0, r3, r2
	ldrb r0, [r0, #4]
	cmp r0, #0
	bne _0800E456
	ldr r1, [sp, #0x30]
	ldr r2, [sp, #0x34]
	adds r0, r3, #0
	bl sub_8007398
_0800E456:
	ldr r3, [sp, #0x38]
	cmp r3, #0
	beq _0800E47E
	ldr r0, [r5]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xc
	ldr r3, [sp, #0x38]
	bl sub_803AD88
	ldr r1, [r5]
	ldr r0, [r1, #0x74]
	ldr r5, [sp, #0x38]
	orrs r0, r5
	str r0, [r1, #0x74]
_0800E47E:
	add sp, #0xc
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800E490: .4byte gUnknown_030012D8
