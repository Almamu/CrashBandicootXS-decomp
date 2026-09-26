.include "asm/macros.inc"
.syntax unified
.arm

	thumb_func_start sub_8031A6C
sub_8031A6C: @ 0x08031A6C
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r1, _08031A98 @ =gStaticData_0817C42C
	ldr r0, [r4, #0x28]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r7, #2
	ldrsh r2, [r0, r7]
	adds r7, r1, #0
	cmp r2, #0
	ble _08031A9C
	movs r1, #4
	ldrsh r0, [r0, r1]
	adds r0, r4, r0
	ldr r1, [r0]
	lsls r0, r2, #3
	adds r0, r0, r1
	subs r0, #8
	ldr r5, [r0]
	ldr r6, [r0, #4]
	adds r3, r6, #0
	b _08031AA2
	.align 2, 0
_08031A98: .4byte gStaticData_0817C42C
_08031A9C:
	adds r0, r7, #4
	adds r0, r3, r0
	ldr r3, [r0]
_08031AA2:
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _08031AB8
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _08031ABA
_08031AB8:
	adds r0, r1, #0
_08031ABA:
	adds r0, r4, r0
	bl sub_803AD84
	ldr r0, [r4, #0x28]
	cmp r0, #1
	bne _08031ADC
	ldr r1, [r4, #0x20]
	movs r0, #0xe1
	lsls r0, r0, #8
	cmp r1, r0
	ble _08031ADC
	cmp r4, #0
	beq _08031B04
	ldr r1, [r4, #0x50]
	movs r2, #8
	ldrsh r0, [r1, r2]
	b _08031AF2
_08031ADC:
	ldr r0, [r4, #0x28]
	cmp r0, #2
	bne _08031AFE
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _08031AFE
	cmp r4, #0
	beq _08031B04
	ldr r1, [r4, #0x50]
	movs r7, #8
	ldrsh r0, [r1, r7]
_08031AF2:
	adds r0, r4, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
	b _08031B04
_08031AFE:
	adds r0, r4, #0
	bl sub_802A7B8
_08031B04:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8031B0C
sub_8031B0C: @ 0x08031B0C
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r5, [r4, #0xc]
	cmp r5, #0
	bne _08031BF8
	bl sub_802A6EC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08031BF8
	movs r0, #2
	movs r1, #1
	str r0, [r4, #0x28]
	str r5, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
	ldr r0, [r4, #0x30]
	ldrb r0, [r0]
	cmp r0, #0x15
	beq _08031B74
	cmp r0, #0x15
	bgt _08031B48
	cmp r0, #0x14
	beq _08031B52
	b _08031BD8
_08031B48:
	cmp r0, #0x16
	beq _08031B98
	cmp r0, #0x17
	beq _08031BBC
	b _08031BD8
_08031B52:
	ldr r0, _08031B6C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	ldr r0, _08031B70 @ =gUnknown_03000884
	ldr r0, [r0]
	movs r1, #1
	bl sub_802F540
	b _08031BD8
	.align 2, 0
_08031B6C: .4byte gUnknown_030012BC
_08031B70: .4byte gUnknown_03000884
_08031B74:
	ldr r0, _08031B90 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	ldr r0, _08031B94 @ =gUnknown_03000884
	ldr r0, [r0]
	movs r1, #3
	bl sub_802F540
	b _08031BD8
	.align 2, 0
_08031B90: .4byte gUnknown_030012BC
_08031B94: .4byte gUnknown_03000884
_08031B98:
	ldr r0, _08031BB4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	ldr r0, _08031BB8 @ =gUnknown_03000884
	ldr r0, [r0]
	movs r1, #5
	bl sub_802F540
	b _08031BD8
	.align 2, 0
_08031BB4: .4byte gUnknown_030012BC
_08031BB8: .4byte gUnknown_03000884
_08031BBC:
	ldr r0, _08031C04 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #7
	bl PlaySfx
	ldr r0, [r4, #0x70]
	bl sub_802AAB4
	ldr r0, _08031C08 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023464
_08031BD8:
	ldr r0, [r4, #0x58]
	cmp r0, #0
	beq _08031BF0
	ldr r0, _08031C08 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022FEC
	ldr r0, [r4, #0x58]
	bl sub_80318B4
	movs r0, #0
	str r0, [r4, #0x58]
_08031BF0:
	adds r1, r4, #0
	adds r1, #0x5c
	movs r0, #1
	strb r0, [r1]
_08031BF8:
	adds r0, r4, #0
	bl sub_8031A6C
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08031C04: .4byte gUnknown_030012BC
_08031C08: .4byte gUnknown_030012C0

	thumb_func_start sub_8031C0C
sub_8031C0C: @ 0x08031C0C
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x54]
	subs r0, r0, r1
	str r0, [r4, #0x54]
	cmp r0, #0
	bgt _08031CF4
	movs r0, #2
	movs r1, #1
	str r0, [r4, #0x28]
	movs r2, #0
	str r2, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	ldr r0, [r4, #0x30]
	ldrb r0, [r0]
	cmp r0, #0x15
	beq _08031C70
	cmp r0, #0x15
	bgt _08031C44
	cmp r0, #0x14
	beq _08031C4E
	b _08031CD4
_08031C44:
	cmp r0, #0x16
	beq _08031C94
	cmp r0, #0x17
	beq _08031CB8
	b _08031CD4
_08031C4E:
	ldr r0, _08031C68 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	ldr r0, _08031C6C @ =gUnknown_03000884
	ldr r0, [r0]
	movs r1, #1
	bl sub_802F540
	b _08031CD4
	.align 2, 0
_08031C68: .4byte gUnknown_030012BC
_08031C6C: .4byte gUnknown_03000884
_08031C70:
	ldr r0, _08031C8C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	ldr r0, _08031C90 @ =gUnknown_03000884
	ldr r0, [r0]
	movs r1, #3
	bl sub_802F540
	b _08031CD4
	.align 2, 0
_08031C8C: .4byte gUnknown_030012BC
_08031C90: .4byte gUnknown_03000884
_08031C94:
	ldr r0, _08031CB0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	ldr r0, _08031CB4 @ =gUnknown_03000884
	ldr r0, [r0]
	movs r1, #5
	bl sub_802F540
	b _08031CD4
	.align 2, 0
_08031CB0: .4byte gUnknown_030012BC
_08031CB4: .4byte gUnknown_03000884
_08031CB8:
	ldr r0, _08031CFC @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #7
	bl PlaySfx
	ldr r0, [r4, #0x70]
	bl sub_802AAB4
	ldr r0, _08031D00 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023464
_08031CD4:
	ldr r0, [r4, #0x58]
	cmp r0, #0
	beq _08031CEC
	ldr r0, _08031D00 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022FEC
	ldr r0, [r4, #0x58]
	bl sub_80318B4
	movs r0, #0
	str r0, [r4, #0x58]
_08031CEC:
	adds r1, r4, #0
	adds r1, #0x5c
	movs r0, #1
	strb r0, [r1]
_08031CF4:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08031CFC: .4byte gUnknown_030012BC
_08031D00: .4byte gUnknown_030012C0

	thumb_func_start sub_8031D04
sub_8031D04: @ 0x08031D04
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	ldr r5, [r4, #0xc]
	cmp r5, #0
	bne _08031D62
	bl sub_802A6EC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08031D62
	movs r0, #2
	movs r6, #1
	str r0, [r4, #0x28]
	str r5, [r4, #0x44]
	str r6, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
	ldr r0, _08031D70 @ =gUnknown_03000884
	ldr r0, [r0]
	movs r1, #0x14
	bl sub_802F50C
	ldr r0, _08031D74 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	ldr r0, [r4, #0x58]
	cmp r0, #0
	beq _08031D5C
	ldr r0, _08031D78 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022FEC
	ldr r0, [r4, #0x58]
	bl sub_80318B4
	str r5, [r4, #0x58]
_08031D5C:
	adds r0, r4, #0
	adds r0, #0x5c
	strb r6, [r0]
_08031D62:
	adds r0, r4, #0
	bl sub_8031A6C
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08031D70: .4byte gUnknown_03000884
_08031D74: .4byte gUnknown_030012BC
_08031D78: .4byte gUnknown_030012C0

	thumb_func_start sub_8031D7C
sub_8031D7C: @ 0x08031D7C
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r5, [r4, #0xc]
	cmp r5, #0
	bne _08031E6A
	bl sub_802A6EC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08031E6A
	movs r0, #2
	movs r1, #1
	str r0, [r4, #0x28]
	str r5, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
	ldr r0, [r4, #0x30]
	ldrb r0, [r0]
	cmp r0, #0x19
	beq _08031DE4
	cmp r0, #0x19
	bgt _08031DB8
	cmp r0, #0x18
	beq _08031DC2
	b _08031E42
_08031DB8:
	cmp r0, #0x1a
	beq _08031E08
	cmp r0, #0x1d
	beq _08031E2C
	b _08031E42
_08031DC2:
	ldr r0, _08031DDC @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	ldr r0, _08031DE0 @ =gUnknown_030012C0
	ldr r0, [r0]
	movs r1, #1
	bl sub_8022EA8
	b _08031E42
	.align 2, 0
_08031DDC: .4byte gUnknown_030012BC
_08031DE0: .4byte gUnknown_030012C0
_08031DE4:
	ldr r0, _08031E00 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	ldr r0, _08031E04 @ =gUnknown_030012C0
	ldr r0, [r0]
	movs r1, #2
	bl sub_8022EA8
	b _08031E42
	.align 2, 0
_08031E00: .4byte gUnknown_030012BC
_08031E04: .4byte gUnknown_030012C0
_08031E08:
	ldr r0, _08031E24 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	ldr r0, _08031E28 @ =gUnknown_030012C0
	ldr r0, [r0]
	movs r1, #3
	bl sub_8022EA8
	b _08031E42
	.align 2, 0
_08031E24: .4byte gUnknown_030012BC
_08031E28: .4byte gUnknown_030012C0
_08031E2C:
	ldr r0, _08031E78 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x18
	bl PlaySfx
	ldr r0, _08031E7C @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022D50
_08031E42:
	ldr r0, [r4, #0x58]
	cmp r0, #0
	beq _08031E62
	ldr r0, [r4, #0x30]
	ldrb r0, [r0]
	cmp r0, #0x1d
	beq _08031E58
	ldr r0, _08031E7C @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022FEC
_08031E58:
	ldr r0, [r4, #0x58]
	bl sub_80318B4
	movs r0, #0
	str r0, [r4, #0x58]
_08031E62:
	adds r1, r4, #0
	adds r1, #0x5c
	movs r0, #1
	strb r0, [r1]
_08031E6A:
	adds r0, r4, #0
	bl sub_8031A6C
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08031E78: .4byte gUnknown_030012BC
_08031E7C: .4byte gUnknown_030012C0

	thumb_func_start sub_8031E80
sub_8031E80: @ 0x08031E80
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x54]
	subs r0, r0, r1
	str r0, [r4, #0x54]
	cmp r0, #0
	bgt _08031F6A
	movs r0, #2
	movs r1, #1
	str r0, [r4, #0x28]
	movs r2, #0
	str r2, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	ldr r0, [r4, #0x30]
	ldrb r0, [r0]
	cmp r0, #0x19
	beq _08031EE4
	cmp r0, #0x19
	bgt _08031EB8
	cmp r0, #0x18
	beq _08031EC2
	b _08031F42
_08031EB8:
	cmp r0, #0x1a
	beq _08031F08
	cmp r0, #0x1d
	beq _08031F2C
	b _08031F42
_08031EC2:
	ldr r0, _08031EDC @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	ldr r0, _08031EE0 @ =gUnknown_030012C0
	ldr r0, [r0]
	movs r1, #1
	bl sub_8022EA8
	b _08031F42
	.align 2, 0
_08031EDC: .4byte gUnknown_030012BC
_08031EE0: .4byte gUnknown_030012C0
_08031EE4:
	ldr r0, _08031F00 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	ldr r0, _08031F04 @ =gUnknown_030012C0
	ldr r0, [r0]
	movs r1, #2
	bl sub_8022EA8
	b _08031F42
	.align 2, 0
_08031F00: .4byte gUnknown_030012BC
_08031F04: .4byte gUnknown_030012C0
_08031F08:
	ldr r0, _08031F24 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	ldr r0, _08031F28 @ =gUnknown_030012C0
	ldr r0, [r0]
	movs r1, #3
	bl sub_8022EA8
	b _08031F42
	.align 2, 0
_08031F24: .4byte gUnknown_030012BC
_08031F28: .4byte gUnknown_030012C0
_08031F2C:
	ldr r0, _08031F70 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x18
	bl PlaySfx
	ldr r0, _08031F74 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022D50
_08031F42:
	ldr r0, [r4, #0x58]
	cmp r0, #0
	beq _08031F62
	ldr r0, [r4, #0x30]
	ldrb r0, [r0]
	cmp r0, #0x1d
	beq _08031F58
	ldr r0, _08031F74 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022FEC
_08031F58:
	ldr r0, [r4, #0x58]
	bl sub_80318B4
	movs r0, #0
	str r0, [r4, #0x58]
_08031F62:
	adds r1, r4, #0
	adds r1, #0x5c
	movs r0, #1
	strb r0, [r1]
_08031F6A:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08031F70: .4byte gUnknown_030012BC
_08031F74: .4byte gUnknown_030012C0

	thumb_func_start sub_8031F78
sub_8031F78: @ 0x08031F78
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r4, r0, #0
	adds r6, r2, #0
	adds r5, r3, #0
	ldr r7, [sp, #0x1c]
	movs r0, #2
	mov r8, r0
	str r7, [sp]
	adds r0, r4, #0
	bl InitActorPart
	mov r0, r8
	str r0, [r4, #0x54]
	ldr r0, _08031FDC @ =gStaticData_087E538C
	str r0, [r4, #0x50]
	adds r1, r4, #0
	adds r1, #0x5c
	movs r0, #0
	strb r0, [r1]
	str r6, [r4, #0x60]
	str r5, [r4, #0x64]
	movs r0, #0xff
	bl sub_8000E1C
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	str r0, [r4, #0x68]
	ldr r0, _08031FE0 @ =0xFFFFC24A
	adds r5, r5, r0
	str r4, [sp]
	movs r0, #0x28
	adds r1, r6, #0
	adds r2, r5, #0
	adds r3, r7, #0
	bl sub_802E4B8
	str r0, [r4, #0x58]
	ldr r0, _08031FE4 @ =gStaticData_087E530C
	str r0, [r4, #0x50]
	adds r0, r4, #0
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08031FDC: .4byte gStaticData_087E538C
_08031FE0: .4byte 0xFFFFC24A
_08031FE4: .4byte gStaticData_087E530C

	thumb_func_start sub_8031FE8
sub_8031FE8: @ 0x08031FE8
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x54]
	subs r0, r0, r1
	str r0, [r4, #0x54]
	cmp r0, #0
	bgt _08032042
	movs r0, #2
	movs r6, #1
	str r0, [r4, #0x28]
	movs r5, #0
	str r5, [r4, #0x44]
	str r6, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
	ldr r0, _08032048 @ =gUnknown_03000884
	ldr r0, [r0]
	movs r1, #0x14
	bl sub_802F50C
	ldr r0, _0803204C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	ldr r0, [r4, #0x58]
	cmp r0, #0
	beq _0803203C
	ldr r0, _08032050 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022FEC
	ldr r0, [r4, #0x58]
	bl sub_80318B4
	str r5, [r4, #0x58]
_0803203C:
	adds r0, r4, #0
	adds r0, #0x5c
	strb r6, [r0]
_08032042:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08032048: .4byte gUnknown_03000884
_0803204C: .4byte gUnknown_030012BC
_08032050: .4byte gUnknown_030012C0

	thumb_func_start sub_8032054
sub_8032054: @ 0x08032054
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r4, r0, #0
	adds r6, r2, #0
	adds r5, r3, #0
	ldr r7, [sp, #0x1c]
	movs r0, #2
	mov r8, r0
	str r7, [sp]
	adds r0, r4, #0
	bl InitActorPart
	mov r0, r8
	str r0, [r4, #0x54]
	ldr r0, _080320B8 @ =gStaticData_087E538C
	str r0, [r4, #0x50]
	adds r1, r4, #0
	adds r1, #0x5c
	movs r0, #0
	strb r0, [r1]
	str r6, [r4, #0x60]
	str r5, [r4, #0x64]
	movs r0, #0xff
	bl sub_8000E1C
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	str r0, [r4, #0x68]
	ldr r0, _080320BC @ =0xFFFFC24A
	adds r5, r5, r0
	str r4, [sp]
	movs r0, #0x2a
	adds r1, r6, #0
	adds r2, r5, #0
	adds r3, r7, #0
	bl sub_802E4B8
	str r0, [r4, #0x58]
	ldr r0, _080320C0 @ =gStaticData_087E52CC
	str r0, [r4, #0x50]
	adds r0, r4, #0
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080320B8: .4byte gStaticData_087E538C
_080320BC: .4byte 0xFFFFC24A
_080320C0: .4byte gStaticData_087E52CC

	thumb_func_start sub_80320C4
sub_80320C4: @ 0x080320C4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r4, r0, #0
	adds r6, r2, #0
	adds r5, r3, #0
	ldr r7, [sp, #0x1c]
	movs r0, #2
	mov r8, r0
	str r7, [sp]
	adds r0, r4, #0
	bl InitActorPart
	mov r0, r8
	str r0, [r4, #0x54]
	ldr r0, _0803212C @ =gStaticData_087E538C
	str r0, [r4, #0x50]
	adds r1, r4, #0
	adds r1, #0x5c
	movs r0, #0
	strb r0, [r1]
	str r6, [r4, #0x60]
	str r5, [r4, #0x64]
	movs r0, #0xff
	bl sub_8000E1C
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	str r0, [r4, #0x68]
	ldr r0, _08032130 @ =0xFFFFC24A
	adds r5, r5, r0
	str r4, [sp]
	movs r0, #0x29
	adds r1, r6, #0
	adds r2, r5, #0
	adds r3, r7, #0
	bl sub_802E4B8
	str r0, [r4, #0x58]
	ldr r0, _08032134 @ =gStaticData_087E534C
	str r0, [r4, #0x50]
	ldr r0, [sp, #0x20]
	str r0, [r4, #0x70]
	adds r0, r4, #0
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0803212C: .4byte gStaticData_087E538C
_08032130: .4byte 0xFFFFC24A
_08032134: .4byte gStaticData_087E534C

	thumb_func_start sub_8032138
sub_8032138: @ 0x08032138
	movs r1, #0
	str r1, [r0, #0x58]
	bx lr
	.align 2, 0

	thumb_func_start sub_8032140
sub_8032140: @ 0x08032140
	push {r4, r5, lr}
	adds r4, r0, #0
	movs r5, #0
	str r5, [r4, #0x6c]
	movs r0, #1
	str r0, [r4, #0x28]
	str r5, [r4, #0x44]
	str r5, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
	ldr r0, _0803216C @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022FEC
	str r5, [r4, #0x58]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0803216C: .4byte gUnknown_030012C0

	thumb_func_start sub_8032170
sub_8032170: @ 0x08032170
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x54]
	subs r0, r0, r1
	str r0, [r4, #0x54]
	cmp r0, #0
	bgt _080321C0
	movs r0, #2
	movs r6, #1
	str r0, [r4, #0x28]
	movs r5, #0
	str r5, [r4, #0x44]
	str r6, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
	ldr r0, [r4, #0x58]
	cmp r0, #0
	beq _080321BA
	ldr r0, _080321C8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	ldr r0, _080321CC @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022FEC
	ldr r0, [r4, #0x58]
	bl sub_80318B4
	str r5, [r4, #0x58]
_080321BA:
	adds r0, r4, #0
	adds r0, #0x5c
	strb r6, [r0]
_080321C0:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_080321C8: .4byte gUnknown_030012BC
_080321CC: .4byte gUnknown_030012C0

	thumb_func_start sub_80321D0
sub_80321D0: @ 0x080321D0
	push {lr}
	adds r3, r0, #0
	ldr r0, _080321F8 @ =gStaticData_087E4DF4
	str r0, [r3, #0x50]
	ldr r2, [r3, #0x4c]
	ldr r0, [r3, #0x48]
	str r0, [r2, #0x48]
	ldr r2, [r3, #0x48]
	ldr r0, [r3, #0x4c]
	str r0, [r2, #0x4c]
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _080321F2
	adds r0, r3, #0
	bl mem_free
_080321F2:
	pop {r0}
	bx r0
	.align 2, 0
_080321F8: .4byte gStaticData_087E4DF4

	thumb_func_start sub_80321FC
sub_80321FC: @ 0x080321FC
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	adds r4, r0, #0
	mov sb, r2
	adds r6, r3, #0
	ldr r7, [sp, #0x20]
	ldr r5, [sp, #0x24]
	lsls r5, r5, #0x18
	lsrs r5, r5, #0x18
	movs r0, #2
	mov r8, r0
	str r7, [sp]
	adds r0, r4, #0
	bl InitActorPart
	mov r0, r8
	str r0, [r4, #0x54]
	ldr r0, _08032268 @ =gStaticData_087E538C
	str r0, [r4, #0x50]
	adds r1, r4, #0
	adds r1, #0x5c
	movs r0, #0
	strb r0, [r1]
	mov r0, sb
	str r0, [r4, #0x60]
	str r6, [r4, #0x64]
	movs r0, #0xff
	bl sub_8000E1C
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	str r0, [r4, #0x68]
	ldr r0, _0803226C @ =0xFFFFC24A
	adds r6, r6, r0
	str r4, [sp]
	adds r0, r5, #0
	mov r1, sb
	adds r2, r6, #0
	adds r3, r7, #0
	bl sub_802E4B8
	str r0, [r4, #0x58]
	adds r0, r4, #0
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08032268: .4byte gStaticData_087E538C
_0803226C: .4byte 0xFFFFC24A

	thumb_func_start nullsub_33
nullsub_33: @ 0x08032270
	bx lr
	.align 2, 0

	thumb_func_start sub_8032274
sub_8032274: @ 0x08032274
	adds r2, r0, #0
	ldr r0, [r2, #0x20]
	ldr r1, [r2, #0x6c]
	adds r0, r0, r1
	str r0, [r2, #0x20]
	adds r1, #0x12
	str r1, [r2, #0x6c]
	movs r0, #0x98
	lsls r0, r0, #3
	cmp r1, r0
	ble _0803228C
	str r0, [r2, #0x6c]
_0803228C:
	bx lr
	.align 2, 0

	thumb_func_start sub_8032290
sub_8032290: @ 0x08032290
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	ldr r4, _080322EC @ =gStaticData_0816A820
	ldr r1, [r5, #0x68]
	ldr r0, [r5, #0x44]
	adds r1, r1, r0
	lsls r0, r1, #2
	adds r0, r0, r1
	asrs r0, r0, #4
	movs r3, #0xff
	ands r0, r3
	lsls r0, r0, #1
	adds r0, r0, r4
	movs r6, #0
	ldrsh r2, [r0, r6]
	lsls r0, r2, #4
	adds r0, r0, r2
	ldr r2, [r5, #0x60]
	adds r6, r2, r0
	str r6, [r5, #0x1c]
	lsls r1, r1, #3
	asrs r1, r1, #4
	ands r1, r3
	lsls r1, r1, #1
	adds r1, r1, r4
	movs r0, #0
	ldrsh r1, [r1, r0]
	lsls r0, r1, #4
	subs r0, r0, r1
	lsls r0, r0, #1
	ldr r1, [r5, #0x64]
	adds r1, r1, r0
	str r1, [r5, #0x20]
	ldr r0, [r5, #0x58]
	cmp r0, #0
	beq _080322E4
	ldr r3, _080322F0 @ =0xFFFFC24A
	adds r2, r1, r3
	ldr r3, [r5, #0x24]
	adds r1, r6, #0
	bl sub_80318D0
_080322E4:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_080322EC: .4byte gStaticData_0816A820
_080322F0: .4byte 0xFFFFC24A

	thumb_func_start sub_80322F4
sub_80322F4: @ 0x080322F4
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r1, _08032320 @ =gStaticData_0817C42C
	ldr r0, [r4, #0x28]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r7, #2
	ldrsh r2, [r0, r7]
	adds r7, r1, #0
	cmp r2, #0
	ble _08032324
	movs r1, #4
	ldrsh r0, [r0, r1]
	adds r0, r4, r0
	ldr r1, [r0]
	lsls r0, r2, #3
	adds r0, r0, r1
	subs r0, #8
	ldr r5, [r0]
	ldr r6, [r0, #4]
	adds r3, r6, #0
	b _0803232A
	.align 2, 0
_08032320: .4byte gStaticData_0817C42C
_08032324:
	adds r0, r7, #4
	adds r0, r3, r0
	ldr r3, [r0]
_0803232A:
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _08032340
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _08032342
_08032340:
	adds r0, r1, #0
_08032342:
	adds r0, r4, r0
	bl sub_803AD84
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8032350
sub_8032350: @ 0x08032350
	adds r0, #0x5c
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8032358
sub_8032358: @ 0x08032358
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0xc]
	cmp r0, #1
	bne _0803237E
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _080323DC
	cmp r4, #0
	beq _080323E2
	ldr r1, [r4, #0x50]
	movs r2, #8
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
	b _080323E2
_0803237E:
	adds r0, r4, #0
	bl sub_802A6EC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _080323CC
	ldr r0, _080323E8 @ =gUnknown_03000884
	ldr r0, [r0]
	ldr r2, [r0, #0x50]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #0x14
	bl sub_803AD80
	ldr r0, _080323EC @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022FEC
	ldr r0, _080323F0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	movs r3, #1
	str r3, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	movs r2, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	adds r0, r4, #0
	adds r0, #0x58
	strb r3, [r0]
_080323CC:
	ldr r1, [r4, #0x20]
	ldr r0, [r4, #0x5c]
	cmp r1, r0
	bge _080323DC
	movs r2, #0xa0
	lsls r2, r2, #1
	adds r0, r1, r2
	str r0, [r4, #0x20]
_080323DC:
	adds r0, r4, #0
	bl sub_802A7B8
_080323E2:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080323E8: .4byte gUnknown_03000884
_080323EC: .4byte gUnknown_030012C0
_080323F0: .4byte gUnknown_030012BC

	thumb_func_start sub_80323F4
sub_80323F4: @ 0x080323F4
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	ldr r0, [r6, #0x54]
	subs r0, r0, r1
	str r0, [r6, #0x54]
	cmp r0, #0
	bgt _08032430
	adds r0, r6, #0
	adds r0, #0x58
	movs r5, #0
	movs r4, #1
	strb r4, [r0]
	ldr r0, _08032438 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	str r4, [r6, #0xc]
	ldr r0, [r6]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r6, #0x10]
	strb r1, [r6, #0x12]
	str r5, [r6, #8]
	ldr r0, _0803243C @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022FEC
_08032430:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08032438: .4byte gUnknown_030012BC
_0803243C: .4byte gUnknown_030012C0

	thumb_func_start sub_8032440
sub_8032440: @ 0x08032440
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r4, r0, #0
	adds r6, r3, #0
	ldr r0, [sp, #0x14]
	movs r5, #2
	str r0, [sp]
	adds r0, r4, #0
	ldr r3, _08032470 @ =0xFFFF0600
	bl InitActorPart
	str r5, [r4, #0x54]
	ldr r0, _08032474 @ =gStaticData_087E53CC
	str r0, [r4, #0x50]
	str r6, [r4, #0x5c]
	adds r1, r4, #0
	adds r1, #0x58
	movs r0, #0
	strb r0, [r1]
	adds r0, r4, #0
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08032470: .4byte 0xFFFF0600
_08032474: .4byte gStaticData_087E53CC

	thumb_func_start sub_8032478
sub_8032478: @ 0x08032478
	adds r0, #0x58
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8032480
sub_8032480: @ 0x08032480
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0xc]
	cmp r0, #0
	bne _08032510
	adds r0, r4, #0
	bl sub_802A6EC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _080324B8
	ldr r0, _080324F0 @ =gUnknown_03000884
	ldr r0, [r0]
	ldr r2, [r0, #0x50]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #0xe
	bl sub_803AD80
	adds r1, r4, #0
	adds r1, #0x65
	movs r0, #1
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_803256C
_080324B8:
	ldr r0, [r4, #0xc]
	cmp r0, #0
	bne _08032510
	ldr r2, _080324F4 @ =gStaticData_0816A820
	ldr r0, [r4, #0x44]
	lsls r0, r0, #6
	asrs r0, r0, #4
	movs r1, #0xff
	ands r0, r1
	adds r0, #0x40
	ands r0, r1
	lsls r0, r0, #1
	adds r0, r0, r2
	movs r5, #0
	ldrsh r1, [r0, r5]
	lsls r1, r1, #4
	ldr r0, [r4, #0x58]
	adds r0, r0, r1
	str r0, [r4, #0x1c]
	ldr r1, [r4, #0x20]
	ldr r0, [r4, #0x5c]
	cmp r1, r0
	ble _080324F8
	ldr r0, [r4, #0x60]
	adds r0, r1, r0
	str r0, [r4, #0x20]
	b _0803255A
	.align 2, 0
_080324F0: .4byte gUnknown_03000884
_080324F4: .4byte gStaticData_0816A820
_080324F8:
	adds r1, r4, #0
	adds r1, #0x38
	ldr r0, _0803250C @ =gStaticData_0817C444
	ldm r0!, {r2, r3, r5}
	stm r1!, {r2, r3, r5}
	adds r0, r4, #0
	bl sub_803256C
	b _0803255A
	.align 2, 0
_0803250C: .4byte gStaticData_0817C444
_08032510:
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _0803252C
	cmp r4, #0
	beq _08032560
	ldr r1, [r4, #0x50]
	movs r2, #8
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
	b _08032560
_0803252C:
	adds r5, r4, #0
	adds r5, #0x65
	ldrb r0, [r5]
	cmp r0, #0
	bne _0803255A
	adds r0, r4, #0
	bl sub_802A6EC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0803255A
	ldr r0, _08032568 @ =gUnknown_03000884
	ldr r0, [r0]
	ldr r2, [r0, #0x50]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #0xe
	bl sub_803AD80
	movs r0, #1
	strb r0, [r5]
_0803255A:
	adds r0, r4, #0
	bl sub_802A7B8
_08032560:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08032568: .4byte gUnknown_03000884

	thumb_func_start sub_803256C
sub_803256C: @ 0x0803256C
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r0, #0x64
	movs r6, #0
	movs r5, #1
	strb r5, [r0]
	ldr r0, _080325A0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	movs r0, #7
	str r0, [r4, #0x18]
	str r5, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r6, [r4, #8]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_080325A0: .4byte gUnknown_030012BC

	thumb_func_start sub_80325A4
sub_80325A4: @ 0x080325A4
	push {r4, r5, lr}
	adds r5, r0, #0
	ldr r0, [r5, #0x54]
	subs r0, r0, r1
	str r0, [r5, #0x54]
	cmp r0, #0
	bgt _080325E2
	adds r1, r5, #0
	adds r1, #0x64
	movs r4, #0
	movs r0, #1
	strb r0, [r1]
	adds r1, #1
	strb r0, [r1]
	ldr r0, _080325E8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	movs r0, #4
	str r0, [r5, #0x18]
	movs r0, #2
	str r0, [r5, #0xc]
	ldr r0, [r5]
	ldrh r0, [r0, #0x18]
	movs r1, #0
	strh r0, [r5, #0x10]
	strb r1, [r5, #0x12]
	str r4, [r5, #8]
_080325E2:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080325E8: .4byte gUnknown_030012BC

	thumb_func_start sub_80325EC
sub_80325EC: @ 0x080325EC
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r6, r3, #0
	ldr r0, [sp, #0x14]
	movs r4, #1
	str r0, [sp]
	adds r0, r5, #0
	movs r3, #0xfa
	lsls r3, r3, #8
	bl InitActorPart
	str r4, [r5, #0x54]
	ldr r0, _0803266C @ =gStaticData_087E5404
	str r0, [r5, #0x50]
	movs r0, #0xfc
	lsls r0, r0, #6
	cmp r6, r0
	ble _08032614
	adds r6, r0, #0
_08032614:
	ldr r0, _08032670 @ =0xFFFFC100
	cmp r6, r0
	bge _0803261C
	adds r6, r0, #0
_0803261C:
	str r6, [r5, #0x5c]
	ldr r0, [r5, #0x1c]
	movs r1, #0x80
	lsls r1, r1, #8
	cmp r0, r1
	ble _0803262A
	str r1, [r5, #0x1c]
_0803262A:
	ldr r0, [r5, #0x1c]
	ldr r1, _08032674 @ =0xFFFF8000
	cmp r0, r1
	bge _08032634
	str r1, [r5, #0x1c]
_08032634:
	ldr r0, [r5, #0x1c]
	str r0, [r5, #0x58]
	ldr r0, [r5, #0x5c]
	ldr r1, _08032678 @ =0xFFFF0600
	adds r0, r0, r1
	movs r1, #0xc6
	bl sub_803ADB4
	str r0, [r5, #0x60]
	adds r0, r5, #0
	adds r0, #0x65
	movs r1, #0
	strb r1, [r0]
	subs r0, #1
	strb r1, [r0]
	ldr r0, _0803267C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x2d
	bl PlaySfx
	adds r0, r5, #0
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0803266C: .4byte gStaticData_087E5404
_08032670: .4byte 0xFFFFC100
_08032674: .4byte 0xFFFF8000
_08032678: .4byte 0xFFFF0600
_0803267C: .4byte gUnknown_030012BC

	thumb_func_start sub_8032680
sub_8032680: @ 0x08032680
	adds r0, #0x64
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8032688
sub_8032688: @ 0x08032688
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x30]
	ldrb r0, [r0]
	cmp r0, #0x1f
	bne _080326CE
	adds r0, r4, #0
	bl sub_802A6EC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _080326CE
	ldr r0, _080326DC @ =gUnknown_03000884
	ldr r0, [r0]
	ldr r2, [r4, #0x30]
	ldr r1, [r4, #0x1c]
	ldr r2, [r2, #0x20]
	subs r1, r1, r2
	ldr r2, [r4, #0x20]
	bl sub_802F164
	adds r1, r4, #0
	adds r1, #0x58
	ldrb r0, [r1]
	cmp r0, #0
	bne _080326CE
	movs r0, #1
	strb r0, [r1]
	ldr r0, _080326E0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x3e
	bl PlaySfx
_080326CE:
	adds r0, r4, #0
	bl sub_802A7B8
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080326DC: .4byte gUnknown_03000884
_080326E0: .4byte gUnknown_030012BC

	thumb_func_start sub_80326E4
sub_80326E4: @ 0x080326E4
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, [sp, #0x10]
	movs r5, #1
	str r0, [sp]
	adds r0, r4, #0
	bl InitActorPart
	str r5, [r4, #0x54]
	ldr r0, _08032710 @ =gStaticData_087E543C
	str r0, [r4, #0x50]
	adds r1, r4, #0
	adds r1, #0x58
	movs r0, #0
	strb r0, [r1]
	adds r0, r4, #0
	add sp, #4
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08032710: .4byte gStaticData_087E543C

	thumb_func_start sub_8032714
sub_8032714: @ 0x08032714
	movs r0, #1
	bx lr

	thumb_func_start sub_8032718
sub_8032718: @ 0x08032718
	push {r4, r5, lr}
	adds r4, r0, #0
	movs r5, #1
	str r5, [r4, #0x14]
	ldr r1, [r4, #0x1c]
	ldr r0, [r4, #0x58]
	adds r1, r1, r0
	str r1, [r4, #0x1c]
	ldr r2, [r4, #0x20]
	ldr r0, [r4, #0x5c]
	adds r2, r2, r0
	str r2, [r4, #0x20]
	movs r0, #0x80
	lsls r0, r0, #5
	cmp r1, r0
	ble _0803273C
	cmp r2, r0
	bgt _08032764
_0803273C:
	ldr r0, _08032760 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xe
	bl PlaySfx
	cmp r4, #0
	beq _0803279C
	ldr r1, [r4, #0x50]
	movs r2, #8
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
	b _0803279C
	.align 2, 0
_08032760: .4byte gUnknown_030012BC
_08032764:
	movs r3, #0x10
	ldrsh r1, [r4, r3]
	ldr r0, [r4, #8]
	adds r0, r0, r1
	str r0, [r4, #8]
	movs r0, #0
	strb r0, [r4, #0x12]
	adds r0, r4, #0
	bl GetAnimFrameBaseOffset
	ldr r2, [r4, #0xc]
	ldr r3, [r4]
	lsls r1, r2, #1
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r1, r1, r3
	movs r3, #4
	ldrsh r2, [r1, r3]
	cmp r0, r2
	blt _0803279C
	movs r0, #6
	ldrsh r1, [r1, r0]
	subs r1, r2, r1
	lsls r1, r1, #8
	ldr r0, [r4, #8]
	subs r0, r0, r1
	str r0, [r4, #8]
	strb r5, [r4, #0x12]
_0803279C:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80327A4
sub_80327A4: @ 0x080327A4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	ldr r0, [r6, #0x1c]
	ldr r1, [r6, #0x20]
	asrs r4, r0, #8
	asrs r5, r1, #8
	adds r0, r6, #0
	bl GetAnimFrameData
	adds r7, r0, #0
	movs r0, #0
	mov r8, r0
	ldrb r0, [r7]
	lsls r2, r0, #2
	ldrb r1, [r7, #1]
	lsls r0, r1, #2
	subs r4, r4, r2
	subs r5, r5, r0
	cmp r5, #0x9f
	bgt _08032830
	lsls r0, r1, #3
	adds r0, r5, r0
	cmp r0, #0
	blt _08032830
	cmp r4, #0xef
	bgt _08032830
	lsls r0, r2, #1
	adds r0, r4, r0
	cmp r0, #0
	blt _08032830
	movs r0, #0x80
	lsls r0, r0, #1
	mov r8, r0
	adds r0, r6, #0
	bl sub_803B060
	movs r3, #0xff
	ands r3, r5
	ldr r1, _0803281C @ =0x000001FF
	ands r4, r1
	lsls r1, r4, #0x10
	orrs r3, r1
	orrs r3, r0
	mov r0, r8
	orrs r3, r0
	ldr r4, [r6, #0x18]
	lsls r2, r4, #0xc
	ldr r0, [r6, #0x14]
	movs r1, #0x80
	lsls r1, r1, #8
	ands r0, r1
	cmp r0, #0
	beq _08032820
	movs r0, #0x80
	lsls r0, r0, #4
	orrs r2, r0
	lsls r0, r2, #0x10
	b _08032822
	.align 2, 0
_0803281C: .4byte 0x000001FF
_08032820:
	lsls r0, r4, #0x1c
_08032822:
	lsrs r2, r0, #0x10
	adds r0, r7, #0
	adds r1, r3, #0
	movs r3, #0xc0
	lsls r3, r3, #1
	bl SetupSpriteFrameOam
_08032830:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_803283C
sub_803283C: @ 0x0803283C
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	adds r7, r1, #0
	ldr r0, _08032884 @ =gStaticData_087E5474
	str r0, [r4, #0x50]
	movs r5, #0
	ldr r0, [r4, #0x60]
	cmp r5, r0
	bge _0803285E
	ldr r6, _08032888 @ =gUnknown_030012C0
_08032850:
	ldr r0, [r6]
	bl sub_8023430
	adds r5, #1
	ldr r0, [r4, #0x60]
	cmp r5, r0
	blt _08032850
_0803285E:
	ldr r0, _0803288C @ =gStaticData_087E4DF4
	str r0, [r4, #0x50]
	ldr r1, [r4, #0x4c]
	ldr r0, [r4, #0x48]
	str r0, [r1, #0x48]
	ldr r1, [r4, #0x48]
	ldr r0, [r4, #0x4c]
	str r0, [r1, #0x4c]
	movs r0, #1
	ands r0, r7
	cmp r0, #0
	beq _0803287C
	adds r0, r4, #0
	bl mem_free
_0803287C:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08032884: .4byte gStaticData_087E5474
_08032888: .4byte gUnknown_030012C0
_0803288C: .4byte gStaticData_087E4DF4

	thumb_func_start sub_8032890
sub_8032890: @ 0x08032890
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r6, r0, #0
	ldr r5, [sp, #0x18]
	movs r4, #1
	str r4, [sp]
	bl InitActorPart
	str r4, [r6, #0x54]
	ldr r0, _08032900 @ =gStaticData_087E5474
	str r0, [r6, #0x50]
	str r5, [r6, #0x60]
	bl sub_8029E98
	ldr r1, [r6, #0x20]
	adds r1, r1, r0
	str r1, [r6, #0x20]
	bl sub_8029EB4
	ldr r1, [r6, #0x1c]
	adds r3, r1, r0
	str r3, [r6, #0x1c]
	ldr r0, _08032904 @ =0xFFFFF000
	adds r1, r3, r0
	asrs r2, r1, #0x1f
	eors r1, r2
	subs r1, r1, r2
	ldr r7, [r6, #0x20]
	adds r0, r7, r0
	asrs r2, r0, #0x1f
	eors r0, r2
	subs r0, r0, r2
	adds r1, r1, r0
	cmp r1, #0
	bge _080328DA
	ldr r0, _08032908 @ =0x000007FF
	adds r1, r1, r0
_080328DA:
	asrs r5, r1, #0xb
	movs r4, #0x80
	lsls r4, r4, #5
	subs r0, r4, r3
	adds r1, r5, #0
	bl sub_803ADB4
	str r0, [r6, #0x58]
	subs r4, r4, r7
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_803ADB4
	str r0, [r6, #0x5c]
	adds r0, r6, #0
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08032900: .4byte gStaticData_087E5474
_08032904: .4byte 0xFFFFF000
_08032908: .4byte 0x000007FF

	thumb_func_start sub_803290C
sub_803290C: @ 0x0803290C
	movs r0, #1
	bx lr

	thumb_func_start sub_8032910
sub_8032910: @ 0x08032910
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x54]
	subs r0, r0, r1
	str r0, [r4, #0x54]
	cmp r0, #0
	bgt _08032946
	movs r0, #4
	str r0, [r4, #0x18]
	ldr r0, _0803294C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	movs r0, #1
	str r0, [r4, #0x28]
	movs r2, #0
	str r2, [r4, #0x44]
	str r0, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
_08032946:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0803294C: .4byte gUnknown_030012BC

	thumb_func_start sub_8032950
sub_8032950: @ 0x08032950
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r1, _0803297C @ =gStaticData_0817C450
	ldr r0, [r4, #0x28]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r7, #2
	ldrsh r2, [r0, r7]
	adds r7, r1, #0
	cmp r2, #0
	ble _08032980
	movs r1, #4
	ldrsh r0, [r0, r1]
	adds r0, r4, r0
	ldr r1, [r0]
	lsls r0, r2, #3
	adds r0, r0, r1
	subs r0, #8
	ldr r5, [r0]
	ldr r6, [r0, #4]
	adds r3, r6, #0
	b _08032986
	.align 2, 0
_0803297C: .4byte gStaticData_0817C450
_08032980:
	adds r0, r7, #4
	adds r0, r3, r0
	ldr r3, [r0]
_08032986:
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _0803299C
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _0803299E
_0803299C:
	adds r0, r1, #0
_0803299E:
	adds r0, r4, r0
	bl sub_803AD84
	ldr r0, [r4, #0x28]
	cmp r0, #1
	bne _080329C6
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _080329C6
	cmp r4, #0
	beq _080329CC
	ldr r1, [r4, #0x50]
	movs r2, #8
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
	b _080329CC
_080329C6:
	adds r0, r4, #0
	bl sub_802A7B8
_080329CC:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80329D4
sub_80329D4: @ 0x080329D4
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	sub sp, #4
	adds r4, r0, #0
	adds r6, r2, #0
	mov r8, r3
	ldr r0, [sp, #0x18]
	movs r5, #2
	str r0, [sp]
	adds r0, r4, #0
	bl InitActorPart
	str r5, [r4, #0x54]
	ldr r0, _08032A18 @ =gStaticData_087E54AC
	str r0, [r4, #0x50]
	str r6, [r4, #0x58]
	mov r0, r8
	str r0, [r4, #0x5c]
	movs r1, #0
	str r1, [r4, #0x64]
	movs r0, #0x95
	str r0, [r4, #0x60]
	adds r0, r4, #0
	adds r0, #0x68
	strb r1, [r0]
	adds r0, r4, #0
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08032A18: .4byte gStaticData_087E54AC

	thumb_func_start sub_8032A1C
sub_8032A1C: @ 0x08032A1C
	adds r0, #0x68
	movs r1, #1
	strb r1, [r0]
	bx lr

	thumb_func_start sub_8032A24
sub_8032A24: @ 0x08032A24
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x24]
	ldr r1, [r4, #0x60]
	adds r0, r0, r1
	str r0, [r4, #0x24]
	subs r1, #5
	str r1, [r4, #0x60]
	cmp r1, #0x13
	bgt _08032A3C
	movs r0, #0x14
	str r0, [r4, #0x60]
_08032A3C:
	adds r0, r4, #0
	bl sub_802A6EC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08032A84
	ldr r0, _08032A8C @ =gUnknown_03000884
	ldr r0, [r0]
	ldr r2, [r0, #0x50]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #6
	bl sub_803AD80
	movs r0, #4
	str r0, [r4, #0x18]
	ldr r0, _08032A90 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	movs r0, #1
	str r0, [r4, #0x28]
	movs r2, #0
	str r2, [r4, #0x44]
	str r0, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
_08032A84:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08032A8C: .4byte gUnknown_03000884
_08032A90: .4byte gUnknown_030012BC

	thumb_func_start sub_8032A94
sub_8032A94: @ 0x08032A94
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r1, _08032AC0 @ =gStaticData_0817C450
	ldr r0, [r4, #0x28]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r7, #2
	ldrsh r2, [r0, r7]
	adds r7, r1, #0
	cmp r2, #0
	ble _08032AC4
	movs r1, #4
	ldrsh r0, [r0, r1]
	adds r0, r4, r0
	ldr r1, [r0]
	lsls r0, r2, #3
	adds r0, r0, r1
	subs r0, #8
	ldr r5, [r0]
	ldr r6, [r0, #4]
	adds r3, r6, #0
	b _08032ACA
	.align 2, 0
_08032AC0: .4byte gStaticData_0817C450
_08032AC4:
	adds r0, r7, #4
	adds r0, r3, r0
	ldr r3, [r0]
_08032ACA:
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _08032AE0
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _08032AE2
_08032AE0:
	adds r0, r1, #0
_08032AE2:
	adds r0, r4, r0
	bl sub_803AD84
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8032AF0
sub_8032AF0: @ 0x08032AF0
	adds r0, #0x68
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8032AF8
sub_8032AF8: @ 0x08032AF8
	push {r4, r5, lr}
	ldr r2, _08032B44 @ =gUnknown_030015FC
	ldrh r1, [r2]
	movs r3, #0
	ldrsh r0, [r2, r3]
	cmp r0, #0
	beq _08032B64
	adds r0, r1, #1
	strh r0, [r2]
	movs r0, #3
	ldrh r1, [r2]
	ands r0, r1
	cmp r0, #0
	bne _08032B1E
	ldr r1, _08032B48 @ =gUnknown_030015FE
	movs r0, #1
	ldrb r3, [r1]
	eors r0, r3
	strb r0, [r1]
_08032B1E:
	movs r1, #0
	ldrsh r0, [r2, r1]
	cmp r0, #0xb
	ble _08032B2A
	movs r0, #0
	strh r0, [r2]
_08032B2A:
	ldr r5, _08032B48 @ =gUnknown_030015FE
	ldr r3, _08032B4C @ =0x00007FFF
	adds r4, r3, #0
	ldr r2, _08032B50 @ =gStaticData_08169AE8
	ldr r1, _08032B54 @ =0x05000020
	adds r3, r1, #0
	adds r3, #0x1e
_08032B38:
	ldrb r0, [r5]
	cmp r0, #0
	beq _08032B58
	strh r4, [r1]
	b _08032B5C
	.align 2, 0
_08032B44: .4byte gUnknown_030015FC
_08032B48: .4byte gUnknown_030015FE
_08032B4C: .4byte 0x00007FFF
_08032B50: .4byte gStaticData_08169AE8
_08032B54: .4byte 0x05000020
_08032B58:
	ldrh r0, [r2]
	strh r0, [r1]
_08032B5C:
	adds r2, #2
	adds r1, #2
	cmp r1, r3
	ble _08032B38
_08032B64:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8032B6C
sub_8032B6C: @ 0x08032B6C
	push {lr}
	ldr r1, _08032B9C @ =gUnknown_030015F4
	ldr r0, [r1]
	adds r2, r0, #1
	str r2, [r1]
	movs r0, #0xf
	ands r0, r2
	cmp r0, #0
	bne _08032BB0
	ldr r2, _08032BA0 @ =gUnknown_03001594
	ldr r0, [r2]
	ldr r3, _08032BA4 @ =gUnknown_030008B4
	cmp r0, #0
	bne _08032B94
	ldr r1, _08032BA8 @ =gUnknown_03001590
	ldr r0, [r3]
	ldrh r0, [r0, #0x1e]
	strh r0, [r1]
	movs r0, #1
	str r0, [r2]
_08032B94:
	ldr r0, [r3]
	ldr r2, _08032BAC @ =0x00007FFF
	adds r1, r2, #0
	b _08032BD2
	.align 2, 0
_08032B9C: .4byte gUnknown_030015F4
_08032BA0: .4byte gUnknown_03001594
_08032BA4: .4byte gUnknown_030008B4
_08032BA8: .4byte gUnknown_03001590
_08032BAC: .4byte 0x00007FFF
_08032BB0:
	movs r0, #7
	ands r2, r0
	cmp r2, #0
	bne _08032BDA
	ldr r2, _08032BF4 @ =gUnknown_03001594
	ldr r0, [r2]
	ldr r1, _08032BF8 @ =gUnknown_03001590
	ldr r3, _08032BFC @ =gUnknown_030008B4
	cmp r0, #0
	bne _08032BCE
	ldr r0, [r3]
	ldrh r0, [r0, #0x1e]
	strh r0, [r1]
	movs r0, #1
	str r0, [r2]
_08032BCE:
	ldr r0, [r3]
	ldrh r1, [r1]
_08032BD2:
	strh r1, [r0, #0x1e]
	ldr r0, _08032C00 @ =gUnknown_030008B8
	ldr r0, [r0]
	strh r1, [r0, #0x1e]
_08032BDA:
	bl sub_8032AF8
	ldr r1, _08032C04 @ =gStaticData_0817C4C8
	ldr r0, _08032C08 @ =gUnknown_030015B0
	ldr r0, [r0]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	bl sub_803AD78
	pop {r0}
	bx r0
	.align 2, 0
_08032BF4: .4byte gUnknown_03001594
_08032BF8: .4byte gUnknown_03001590
_08032BFC: .4byte gUnknown_030008B4
_08032C00: .4byte gUnknown_030008B8
_08032C04: .4byte gStaticData_0817C4C8
_08032C08: .4byte gUnknown_030015B0

	thumb_func_start sub_8032C0C
sub_8032C0C: @ 0x08032C0C
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	ldr r0, _08032C34 @ =gUnknown_030015BC
	ldr r3, _08032C38 @ =gUnknown_030015D4
	ldr r1, [r0]
	ldr r2, [r3]
	adds r1, r1, r2
	str r1, [r0]
	ldr r0, _08032C3C @ =gUnknown_030015EC
	ldr r1, [r0]
	adds r4, r0, #0
	cmp r1, #0
	bne _08032C40
	cmp r2, #0x98
	bgt _08032C5E
	adds r0, r2, #1
	b _08032C60
	.align 2, 0
_08032C34: .4byte gUnknown_030015BC
_08032C38: .4byte gUnknown_030015D4
_08032C3C: .4byte gUnknown_030015EC
_08032C40:
	cmp r1, #1
	bne _08032C52
	cmp r2, #0x3f
	bgt _08032C4C
	adds r0, r2, #1
	b _08032C60
_08032C4C:
	cmp r2, #0x40
	ble _08032C62
	b _08032C5E
_08032C52:
	cmp r2, #0x69
	bgt _08032C5A
	adds r0, r2, #1
	b _08032C60
_08032C5A:
	cmp r2, #0x6a
	ble _08032C62
_08032C5E:
	subs r0, r2, #1
_08032C60:
	str r0, [r3]
_08032C62:
	ldr r0, [r4]
	cmp r0, #0
	beq _08032C6A
	b _08032D6C
_08032C6A:
	ldr r1, _08032D38 @ =gUnknown_030015B4
	ldr r0, _08032D3C @ =gUnknown_030015CC
	mov ip, r0
	ldr r0, [r1]
	mov r2, ip
	ldr r5, [r2]
	adds r0, r0, r5
	str r0, [r1]
	ldr r1, _08032D40 @ =gUnknown_030015B8
	ldr r3, _08032D44 @ =gUnknown_030015D0
	mov sb, r3
	ldr r0, [r1]
	ldr r4, [r3]
	mov r8, r4
	add r0, r8
	str r0, [r1]
	ldr r0, _08032D48 @ =gUnknown_03000884
	ldr r6, [r0]
	ldr r2, [r6, #0x1c]
	ldr r0, _08032D4C @ =gUnknown_030015C0
	ldr r7, [r0]
	ldr r1, _08032D50 @ =0xFFFFEE00
	adds r0, r7, r1
	subs r2, r2, r0
	ldr r4, _08032D54 @ =gStaticData_0817C4B0
	movs r0, #0
	ldrsh r3, [r4, r0]
	movs r1, #6
	ldrsh r0, [r4, r1]
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	adds r3, r3, r0
	subs r2, r2, r3
	asrs r2, r2, #0xc
	subs r5, r5, r2
	mov r2, ip
	str r5, [r2]
	ldr r2, [r6, #0x20]
	ldr r0, _08032D58 @ =gUnknown_030015C4
	ldr r6, [r0]
	movs r3, #0xc0
	lsls r3, r3, #5
	adds r0, r6, r3
	subs r2, r2, r0
	movs r0, #2
	ldrsh r3, [r4, r0]
	movs r1, #8
	ldrsh r0, [r4, r1]
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	adds r3, r3, r0
	subs r2, r2, r3
	asrs r2, r2, #0xc
	mov r3, r8
	subs r0, r3, r2
	mov r4, sb
	str r0, [r4]
	movs r2, #0x80
	lsls r2, r2, #2
	cmp r5, r2
	ble _08032CEA
	adds r5, r2, #0
_08032CEA:
	mov r1, ip
	str r5, [r1]
	ldr r1, _08032D5C @ =0xFFFFFE00
	cmp r5, r1
	bge _08032CF6
	adds r5, r1, #0
_08032CF6:
	mov r3, ip
	str r5, [r3]
	cmp r0, r2
	ble _08032D00
	adds r0, r2, #0
_08032D00:
	mov r4, sb
	str r0, [r4]
	cmp r0, r1
	bge _08032D0A
	adds r0, r1, #0
_08032D0A:
	mov r3, sb
	str r0, [r3]
	cmp r7, #0
	bgt _08032D16
	mov r4, ip
	str r2, [r4]
_08032D16:
	ldr r0, _08032D60 @ =0x000063FF
	cmp r7, r0
	ble _08032D20
	mov r0, ip
	str r1, [r0]
_08032D20:
	ldr r0, _08032D64 @ =0xFFFFC400
	cmp r6, r0
	bgt _08032D2A
	mov r3, sb
	str r2, [r3]
_08032D2A:
	ldr r0, _08032D68 @ =0x00002BFF
	cmp r6, r0
	ble _08032E04
	mov r4, sb
	str r1, [r4]
	b _08032E04
	.align 2, 0
_08032D38: .4byte gUnknown_030015B4
_08032D3C: .4byte gUnknown_030015CC
_08032D40: .4byte gUnknown_030015B8
_08032D44: .4byte gUnknown_030015D0
_08032D48: .4byte gUnknown_03000884
_08032D4C: .4byte gUnknown_030015C0
_08032D50: .4byte 0xFFFFEE00
_08032D54: .4byte gStaticData_0817C4B0
_08032D58: .4byte gUnknown_030015C4
_08032D5C: .4byte 0xFFFFFE00
_08032D60: .4byte 0x000063FF
_08032D64: .4byte 0xFFFFC400
_08032D68: .4byte 0x00002BFF
_08032D6C:
	cmp r0, #1
	bne _08032DB4
	ldr r1, _08032D8C @ =gUnknown_030015B4
	ldr r3, _08032D90 @ =gUnknown_030015CC
	ldr r0, [r1]
	ldr r4, [r3]
	adds r0, r0, r4
	str r0, [r1]
	ldr r2, _08032D94 @ =0x0000FFFF
	cmp r0, r2
	ble _08032D9C
	cmp r4, #0
	ble _08032D9C
	ldr r0, _08032D98 @ =0xFFFFFC00
	b _08032E02
	.align 2, 0
_08032D8C: .4byte gUnknown_030015B4
_08032D90: .4byte gUnknown_030015CC
_08032D94: .4byte 0x0000FFFF
_08032D98: .4byte 0xFFFFFC00
_08032D9C:
	ldr r1, [r1]
	ldr r0, _08032DB0 @ =0xFFFF0000
	cmp r1, r0
	bgt _08032E04
	ldr r0, [r3]
	cmp r0, #0
	bge _08032E04
	movs r0, #0x80
	lsls r0, r0, #3
	b _08032E02
	.align 2, 0
_08032DB0: .4byte 0xFFFF0000
_08032DB4:
	ldr r5, _08032E68 @ =gUnknown_030015F0
	ldr r0, [r5]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r5]
	movs r1, #0x80
	lsls r1, r1, #8
	cmp r0, r1
	ble _08032DCA
	str r1, [r5]
_08032DCA:
	ldr r3, _08032E6C @ =gUnknown_030015B4
	ldr r4, _08032E70 @ =gStaticData_0816A820
	ldr r0, _08032E74 @ =gUnknown_030015F4
	ldr r0, [r0]
	lsls r1, r0, #4
	subs r1, r1, r0
	lsls r1, r1, #1
	asrs r1, r1, #4
	movs r2, #0xff
	ands r1, r2
	adds r0, r1, #0
	adds r0, #0x40
	ands r0, r2
	lsls r0, r0, #1
	adds r0, r0, r4
	movs r2, #0
	ldrsh r0, [r0, r2]
	ldr r2, [r5]
	muls r0, r2, r0
	asrs r0, r0, #8
	str r0, [r3]
	ldr r3, _08032E78 @ =gUnknown_030015B8
	lsls r1, r1, #1
	adds r1, r1, r4
	movs r4, #0
	ldrsh r0, [r1, r4]
	muls r0, r2, r0
	asrs r0, r0, #8
_08032E02:
	str r0, [r3]
_08032E04:
	ldr r0, _08032E7C @ =gUnknown_030015C8
	ldr r1, [r0]
	ldr r0, _08032E80 @ =0x000027FF
	cmp r1, r0
	bgt _08032E5A
	ldr r1, _08032E84 @ =gUnknown_030015E4
	ldr r0, _08032E88 @ =gUnknown_030015DC
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	str r0, [r1]
	ldr r0, _08032E8C @ =gUnknown_030015E8
	movs r5, #0
	str r5, [r0]
	movs r1, #3
	ldr r0, _08032E90 @ =gUnknown_030015B0
	str r1, [r0]
	ldr r0, _08032E94 @ =gUnknown_030015AC
	ldr r4, [r0]
	str r5, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	adds r0, r4, #0
	bl GetAnimFrameBaseOffset
	ldr r2, [r4, #0xc]
	ldr r3, [r4]
	lsls r1, r2, #1
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r1, r1, r3
	movs r2, #4
	ldrsh r1, [r1, r2]
	cmp r0, r1
	blt _08032E50
	str r5, [r4, #8]
_08032E50:
	ldr r0, _08032E98 @ =gUnknown_030015EC
	str r5, [r0]
	ldr r1, _08032E9C @ =gUnknown_030015D4
	movs r0, #0xae
	str r0, [r1]
_08032E5A:
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08032E68: .4byte gUnknown_030015F0
_08032E6C: .4byte gUnknown_030015B4
_08032E70: .4byte gStaticData_0816A820
_08032E74: .4byte gUnknown_030015F4
_08032E78: .4byte gUnknown_030015B8
_08032E7C: .4byte gUnknown_030015C8
_08032E80: .4byte 0x000027FF
_08032E84: .4byte gUnknown_030015E4
_08032E88: .4byte gUnknown_030015DC
_08032E8C: .4byte gUnknown_030015E8
_08032E90: .4byte gUnknown_030015B0
_08032E94: .4byte gUnknown_030015AC
_08032E98: .4byte gUnknown_030015EC
_08032E9C: .4byte gUnknown_030015D4

	thumb_func_start sub_8032EA0
sub_8032EA0: @ 0x08032EA0
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, _08032ED4 @ =gUnknown_030015B4
	ldr r5, _08032ED8 @ =gUnknown_030015CC
	ldr r0, [r3]
	ldr r1, [r5]
	adds r0, r0, r1
	str r0, [r3]
	ldr r2, _08032EDC @ =gUnknown_030015B8
	ldr r7, _08032EE0 @ =gUnknown_030015D0
	ldr r1, [r2]
	ldr r0, [r7]
	adds r6, r1, r0
	str r6, [r2]
	ldr r2, _08032EE4 @ =gUnknown_030015BC
	ldr r4, _08032EE8 @ =gUnknown_030015D4
	ldr r0, [r2]
	ldr r1, [r4]
	adds r0, r0, r1
	str r0, [r2]
	cmp r6, #0
	ble _08032EF0
	ldr r0, _08032EEC @ =0xFFFFFF00
	b _08032EFC
	.align 2, 0
_08032ED4: .4byte gUnknown_030015B4
_08032ED8: .4byte gUnknown_030015CC
_08032EDC: .4byte gUnknown_030015B8
_08032EE0: .4byte gUnknown_030015D0
_08032EE4: .4byte gUnknown_030015BC
_08032EE8: .4byte gUnknown_030015D4
_08032EEC: .4byte 0xFFFFFF00
_08032EF0:
	cmp r6, #0
	bge _08032EFA
	movs r0, #0x80
	lsls r0, r0, #1
	b _08032EFC
_08032EFA:
	movs r0, #0
_08032EFC:
	str r0, [r7]
	ldr r0, _08032F14 @ =gUnknown_030015EC
	ldr r1, [r0]
	adds r6, r0, #0
	cmp r1, #3
	bgt _08032F5C
	ldr r0, [r4]
	cmp r0, #0xad
	bgt _08032F18
	adds r0, #1
	b _08032F1E
	.align 2, 0
_08032F14: .4byte gUnknown_030015EC
_08032F18:
	cmp r0, #0xae
	ble _08032F20
	subs r0, #1
_08032F1E:
	str r0, [r4]
_08032F20:
	ldr r1, [r3]
	ldr r0, _08032F34 @ =0x00007FFF
	cmp r1, r0
	ble _08032F3C
	ldr r0, [r5]
	cmp r0, #0
	ble _08032F3C
	ldr r0, _08032F38 @ =0xFFFFFE00
	b _08032F4E
	.align 2, 0
_08032F34: .4byte 0x00007FFF
_08032F38: .4byte 0xFFFFFE00
_08032F3C:
	ldr r1, [r3]
	ldr r0, _08032F58 @ =0xFFFF8000
	cmp r1, r0
	bgt _08032F8A
	ldr r0, [r5]
	cmp r0, #0
	bge _08032F8A
	movs r0, #0x80
	lsls r0, r0, #2
_08032F4E:
	str r0, [r5]
	ldr r0, [r6]
	adds r0, #1
	str r0, [r6]
	b _08032F8A
	.align 2, 0
_08032F58: .4byte 0xFFFF8000
_08032F5C:
	ldr r1, [r4]
	movs r0, #0xea
	lsls r0, r0, #1
	cmp r1, r0
	bgt _08032F6A
	adds r0, r1, #1
	b _08032F6C
_08032F6A:
	subs r0, r1, #1
_08032F6C:
	str r0, [r4]
	ldr r1, [r3]
	cmp r1, #0
	ble _08032F7C
	ldr r0, _08032F78 @ =0xFFFFFE00
	b _08032F88
	.align 2, 0
_08032F78: .4byte 0xFFFFFE00
_08032F7C:
	cmp r1, #0
	bge _08032F86
	movs r0, #0x80
	lsls r0, r0, #2
	b _08032F88
_08032F86:
	movs r0, #0
_08032F88:
	str r0, [r5]
_08032F8A:
	mov r8, r6
	ldr r0, [r6]
	cmp r0, #3
	ble _08033032
	ldr r0, _08032FFC @ =gUnknown_030015C8
	ldr r1, [r0]
	movs r0, #0x80
	lsls r0, r0, #8
	cmp r1, r0
	ble _08033032
	ldr r0, _08033000 @ =gUnknown_030015F4
	movs r5, #0
	str r5, [r0]
	ldr r0, _08033004 @ =gUnknown_030015F0
	str r5, [r0]
	ldr r1, _08033008 @ =gUnknown_030015E4
	ldr r0, _0803300C @ =gUnknown_030015DC
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	str r0, [r1]
	ldr r0, _08033010 @ =gUnknown_030015E8
	str r5, [r0]
	movs r7, #2
	ldr r0, _08033014 @ =gUnknown_030015B0
	str r7, [r0]
	ldr r0, _08033018 @ =gUnknown_030015AC
	ldr r4, [r0]
	str r5, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	adds r0, r4, #0
	bl GetAnimFrameBaseOffset
	ldr r2, [r4, #0xc]
	ldr r3, [r4]
	lsls r1, r2, #1
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r1, r1, r3
	movs r2, #4
	ldrsh r1, [r1, r2]
	cmp r0, r1
	blt _08032FE8
	str r5, [r4, #8]
_08032FE8:
	ldr r0, _0803301C @ =gUnknown_030015F8
	ldr r0, [r0]
	cmp r0, #2
	ble _08033024
	movs r0, #1
	mov r1, r8
	str r0, [r1]
	ldr r1, _08033020 @ =gUnknown_030015D4
	movs r0, #0x40
	b _0803302A
	.align 2, 0
_08032FFC: .4byte gUnknown_030015C8
_08033000: .4byte gUnknown_030015F4
_08033004: .4byte gUnknown_030015F0
_08033008: .4byte gUnknown_030015E4
_0803300C: .4byte gUnknown_030015DC
_08033010: .4byte gUnknown_030015E8
_08033014: .4byte gUnknown_030015B0
_08033018: .4byte gUnknown_030015AC
_0803301C: .4byte gUnknown_030015F8
_08033020: .4byte gUnknown_030015D4
_08033024:
	str r7, [r6]
	ldr r1, _0803303C @ =gUnknown_030015D4
	movs r0, #0x6a
_0803302A:
	str r0, [r1]
	ldr r1, _08033040 @ =gUnknown_030015CC
	ldr r0, _08033044 @ =0xFFFFF600
	str r0, [r1]
_08033032:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0803303C: .4byte gUnknown_030015D4
_08033040: .4byte gUnknown_030015CC
_08033044: .4byte 0xFFFFF600

	thumb_func_start sub_8033048
sub_8033048: @ 0x08033048
	push {r4, lr}
	ldr r0, _08033060 @ =gUnknown_030015FF
	ldrb r0, [r0]
	cmp r0, #0
	bne _080330BA
	ldr r0, _08033064 @ =gUnknown_030015D4
	ldr r2, [r0]
	adds r1, r0, #0
	cmp r2, #0x98
	bgt _08033068
	adds r0, r2, #1
	b _0803306E
	.align 2, 0
_08033060: .4byte gUnknown_030015FF
_08033064: .4byte gUnknown_030015D4
_08033068:
	cmp r2, #0x99
	ble _08033070
	subs r0, r2, #1
_0803306E:
	str r0, [r1]
_08033070:
	ldr r0, _08033080 @ =gUnknown_030015D0
	ldr r2, [r0]
	adds r3, r0, #0
	cmp r2, #0xff
	bgt _08033084
	movs r4, #0x80
	lsls r4, r4, #1
	b _0803308E
	.align 2, 0
_08033080: .4byte gUnknown_030015D0
_08033084:
	movs r0, #0x80
	lsls r0, r0, #1
	cmp r2, r0
	ble _08033092
	ldr r4, _080330DC @ =0xFFFFFF00
_0803308E:
	adds r0, r2, r4
	str r0, [r3]
_08033092:
	ldr r2, _080330E0 @ =gUnknown_030015BC
	ldr r0, [r2]
	ldr r1, [r1]
	adds r0, r0, r1
	str r0, [r2]
	ldr r2, _080330E4 @ =gUnknown_030015B8
	ldr r0, [r2]
	ldr r1, [r3]
	adds r0, r0, r1
	str r0, [r2]
	movs r1, #0x96
	lsls r1, r1, #7
	cmp r0, r1
	ble _080330BA
	bl sub_802A4EC
	ldr r0, _080330E8 @ =gUnknown_03000884
	ldr r0, [r0]
	bl sub_802F0DC
_080330BA:
	ldr r0, _080330EC @ =gUnknown_030015C8
	ldr r1, [r0]
	ldr r0, _080330F0 @ =0x000014FF
	cmp r1, r0
	bgt _080330D6
	movs r2, #0x80
	lsls r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _080330F4 @ =0x0000FBFF
	ands r0, r1
	strh r0, [r2]
	ldr r1, _080330F8 @ =gUnknown_030015FF
	movs r0, #1
	strb r0, [r1]
_080330D6:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080330DC: .4byte 0xFFFFFF00
_080330E0: .4byte gUnknown_030015BC
_080330E4: .4byte gUnknown_030015B8
_080330E8: .4byte gUnknown_03000884
_080330EC: .4byte gUnknown_030015C8
_080330F0: .4byte 0x000014FF
_080330F4: .4byte 0x0000FBFF
_080330F8: .4byte gUnknown_030015FF

	thumb_func_start sub_80330FC
sub_80330FC: @ 0x080330FC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	adds r5, r0, #0
	ldr r0, _080331AC @ =gUnknown_03001598
	ldr r0, [r0]
	adds r0, #0x18
	lsls r2, r0, #0xb
	ldr r7, _080331B0 @ =gUnknown_030015A0
	ldr r0, [r7]
	movs r6, #0x20
	subs r1, r6, r0
	cmp r1, #0
	bge _0803311E
	adds r1, #3
_0803311E:
	asrs r1, r1, #2
	lsls r1, r1, #1
	movs r0, #0xc0
	lsls r0, r0, #0x13
	adds r1, r1, r0
	adds r1, r2, r1
	ldr r3, _080331B4 @ =gUnknown_030015A4
	ldr r4, [r3]
	subs r0, r6, r4
	lsrs r2, r0, #0x1f
	adds r0, r0, r2
	asrs r0, r0, #1
	lsls r0, r0, #5
	adds r0, #2
	adds r6, r1, r0
	movs r2, #0
	mov sl, r3
	cmp r2, r4
	bge _0803319E
	mov r8, r7
	ldr r1, _080331B8 @ =gUnknown_030015A8
	mov sb, r1
_0803314A:
	movs r4, #0
	mov r0, r8
	ldr r1, [r0]
	lsrs r0, r1, #0x1f
	adds r1, r1, r0
	asrs r1, r1, #1
	movs r0, #0x20
	adds r0, r0, r6
	mov ip, r0
	adds r7, r2, #1
	cmp r4, r1
	bge _08033192
	mov r3, sb
	adds r2, r6, #0
_08033166:
	ldrb r0, [r3]
	ldrh r6, [r5]
	adds r1, r6, r0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	adds r5, #2
	ldrh r6, [r5]
	adds r0, r6, r0
	lsls r0, r0, #0x10
	adds r5, #2
	lsrs r0, r0, #8
	orrs r1, r0
	strh r1, [r2]
	adds r2, #2
	adds r4, #1
	mov r1, r8
	ldr r0, [r1]
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	cmp r4, r0
	blt _08033166
_08033192:
	mov r6, ip
	adds r2, r7, #0
	mov r1, sl
	ldr r0, [r1]
	cmp r2, r0
	blt _0803314A
_0803319E:
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080331AC: .4byte gUnknown_03001598
_080331B0: .4byte gUnknown_030015A0
_080331B4: .4byte gUnknown_030015A4
_080331B8: .4byte gUnknown_030015A8

	thumb_func_start sub_80331BC
sub_80331BC: @ 0x080331BC
	push {r4, r5, r6, lr}
	ldr r1, _0803323C @ =gUnknown_030015D8
	str r0, [r1]
	ldr r1, _08033240 @ =gUnknown_030015A0
	ldr r2, _08033244 @ =gStaticData_08169CE8
	movs r3, #0
	ldrsh r0, [r2, r3]
	str r0, [r1]
	ldr r1, _08033248 @ =gUnknown_030015A4
	movs r3, #2
	ldrsh r0, [r2, r3]
	str r0, [r1]
	ldr r4, _0803324C @ =gUnknown_030015AC
	movs r0, #0x1c
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r5, r0, #0
	ldr r0, _08033250 @ =gStaticData_0817C4BC
	ldr r1, _08033254 @ =gUnknown_03001600
	movs r2, #1
	str r0, [r5]
	str r1, [r5, #4]
	str r2, [r5, #0x18]
	adds r0, r5, #0
	movs r1, #0
	bl sub_803B0A8
	str r5, [r4]
	movs r4, #0
	ldr r0, _08033258 @ =gUnknown_030015B0
	str r4, [r0]
	str r4, [r5, #0xc]
	ldr r0, [r5]
	ldrh r0, [r0]
	movs r6, #0
	strh r0, [r5, #0x10]
	strb r6, [r5, #0x12]
	adds r0, r5, #0
	bl GetAnimFrameBaseOffset
	ldr r2, [r5, #0xc]
	ldr r3, [r5]
	lsls r1, r2, #1
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r1, r1, r3
	movs r2, #4
	ldrsh r1, [r1, r2]
	cmp r0, r1
	blt _08033226
	str r4, [r5, #8]
_08033226:
	bl sub_8033604
	ldr r0, _0803325C @ =gUnknown_0300159C
	strb r6, [r0]
	ldr r1, _08033260 @ =gUnknown_030015F8
	movs r0, #4
	str r0, [r1]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0803323C: .4byte gUnknown_030015D8
_08033240: .4byte gUnknown_030015A0
_08033244: .4byte gStaticData_08169CE8
_08033248: .4byte gUnknown_030015A4
_0803324C: .4byte gUnknown_030015AC
_08033250: .4byte gStaticData_0817C4BC
_08033254: .4byte gUnknown_03001600
_08033258: .4byte gUnknown_030015B0
_0803325C: .4byte gUnknown_0300159C
_08033260: .4byte gUnknown_030015F8

	thumb_func_start sub_8033264
sub_8033264: @ 0x08033264
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	mov sb, r0
	adds r5, r1, #0
	adds r6, r2, #0
	mov r8, r3
	ldr r1, _08033408 @ =gUnknown_030015D4
	movs r0, #0x66
	str r0, [r1]
	movs r7, #0
	ldr r0, _0803340C @ =gUnknown_030015B0
	movs r1, #1
	str r1, [r0]
	ldr r2, _08033410 @ =gUnknown_030015AC
	ldr r4, [r2]
	str r7, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0]
	strh r0, [r4, #0x10]
	movs r3, #0
	strb r3, [r4, #0x12]
	adds r0, r4, #0
	bl GetAnimFrameBaseOffset
	ldr r2, [r4, #0xc]
	ldr r3, [r4]
	lsls r1, r2, #1
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r1, r1, r3
	movs r2, #4
	ldrsh r1, [r1, r2]
	cmp r0, r1
	blt _080332B0
	str r7, [r4, #8]
_080332B0:
	lsls r0, r5, #2
	adds r0, r0, r5
	ldr r3, _08033414 @ =gUnknown_030015B4
	str r0, [r3]
	ldr r0, _08033418 @ =gUnknown_030015B8
	mov sl, r0
	lsls r0, r6, #1
	adds r0, r0, r6
	mov r1, sl
	str r0, [r1]
	ldr r5, _0803341C @ =gUnknown_030015BC
	mov r2, r8
	str r2, [r5]
	ldr r3, _08033420 @ =gUnknown_030015DC
	ldr r0, _08033424 @ =gUnknown_030015D8
	ldr r0, [r0]
	lsls r1, r0, #2
	adds r1, r1, r0
	lsls r1, r1, #3
	mov r2, sb
	lsls r0, r2, #2
	add r0, sb
	lsls r0, r0, #3
	ldr r2, _08033428 @ =gStaticData_0817C460
	adds r0, r0, r2
	adds r1, r1, r0
	str r1, [r3]
	ldr r2, _0803342C @ =gUnknown_030015E4
	ldr r0, [r1, #0xc]
	str r0, [r2]
	ldr r2, _08033430 @ =gUnknown_030015E0
	ldr r0, [r1]
	str r0, [r2]
	ldr r0, _08033434 @ =gUnknown_030015E8
	str r7, [r0]
	movs r2, #0x80
	lsls r2, r2, #0x13
	ldrh r0, [r2]
	movs r3, #0x80
	lsls r3, r3, #3
	adds r1, r3, #0
	orrs r0, r1
	strh r0, [r2]
	ldr r0, _08033438 @ =gUnknown_0300159C
	movs r1, #1
	strb r1, [r0]
	ldr r0, _0803343C @ =gUnknown_03001598
	str r7, [r0]
	ldr r0, _08033440 @ =gUnknown_030015EC
	str r7, [r0]
	ldr r0, _08033444 @ =gUnknown_030015F4
	str r7, [r0]
	ldr r0, _08033448 @ =gUnknown_030015F0
	str r7, [r0]
	ldr r1, _0803344C @ =gUnknown_030015F8
	movs r0, #4
	str r0, [r1]
	ldr r0, _08033450 @ =gUnknown_030015FC
	strh r7, [r0]
	ldr r0, _08033454 @ =gUnknown_030015FE
	movs r2, #0
	strb r2, [r0]
	ldr r4, _08033458 @ =gUnknown_030015C8
	bl sub_8029B2C
	lsls r0, r0, #8
	ldr r1, [r5]
	subs r1, r1, r0
	str r1, [r4]
	movs r0, #0xe0
	lsls r0, r0, #0x11
	bl sub_803ADB4
	ldr r2, _0803345C @ =gUnknown_030015C0
	ldr r3, _08033414 @ =gUnknown_030015B4
	ldr r1, [r3]
	muls r1, r0, r1
	asrs r1, r1, #0xc
	str r1, [r2]
	ldr r2, _08033460 @ =gUnknown_030015C4
	mov r3, sl
	ldr r1, [r3]
	muls r0, r1, r0
	asrs r0, r0, #0xc
	str r0, [r2]
	ldr r0, [r4]
	bl sub_8029E34
	ldr r0, _08033410 @ =gUnknown_030015AC
	ldr r2, [r0]
	ldr r3, [r2, #8]
	asrs r3, r3, #8
	ldr r1, [r2, #0xc]
	ldr r4, [r2]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r4
	movs r1, #2
	ldrsh r0, [r0, r1]
	adds r0, r0, r3
	ldr r1, [r2, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	bl sub_80330FC
	ldr r2, _08033414 @ =gUnknown_030015B4
	ldr r0, [r2]
	movs r3, #0x80
	lsls r3, r3, #6
	adds r0, r0, r3
	mov r2, sl
	ldr r1, [r2]
	movs r3, #0xc0
	lsls r3, r3, #6
	adds r1, r1, r3
	ldr r2, [r5]
	ldr r4, _08033464 @ =0xFFFFFF00
	adds r2, r2, r4
	bl sub_802E5B0
	ldr r1, _08033414 @ =gUnknown_030015B4
	ldr r0, [r1]
	movs r2, #0xf0
	lsls r2, r2, #5
	adds r0, r0, r2
	mov r3, sl
	ldr r1, [r3]
	ldr r2, _08033468 @ =0xFFFFD000
	adds r1, r1, r2
	ldr r2, [r5]
	adds r2, r2, r4
	bl sub_802E57C
	ldr r3, _08033414 @ =gUnknown_030015B4
	ldr r0, [r3]
	ldr r1, _0803346C @ =0xFFFFBF00
	adds r0, r0, r1
	mov r2, sl
	ldr r1, [r2]
	movs r4, #0xa0
	lsls r4, r4, #4
	adds r1, r1, r4
	ldr r2, [r5]
	subs r2, #1
	movs r3, #1
	bl sub_802E538
	ldr r3, _08033414 @ =gUnknown_030015B4
	ldr r0, [r3]
	movs r1, #0x84
	lsls r1, r1, #8
	adds r0, r0, r1
	mov r2, sl
	ldr r1, [r2]
	adds r1, r1, r4
	ldr r2, [r5]
	subs r2, #1
	movs r3, #0
	bl sub_802E538
	bl sub_802A4F8
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08033408: .4byte gUnknown_030015D4
_0803340C: .4byte gUnknown_030015B0
_08033410: .4byte gUnknown_030015AC
_08033414: .4byte gUnknown_030015B4
_08033418: .4byte gUnknown_030015B8
_0803341C: .4byte gUnknown_030015BC
_08033420: .4byte gUnknown_030015DC
_08033424: .4byte gUnknown_030015D8
_08033428: .4byte gStaticData_0817C460
_0803342C: .4byte gUnknown_030015E4
_08033430: .4byte gUnknown_030015E0
_08033434: .4byte gUnknown_030015E8
_08033438: .4byte gUnknown_0300159C
_0803343C: .4byte gUnknown_03001598
_08033440: .4byte gUnknown_030015EC
_08033444: .4byte gUnknown_030015F4
_08033448: .4byte gUnknown_030015F0
_0803344C: .4byte gUnknown_030015F8
_08033450: .4byte gUnknown_030015FC
_08033454: .4byte gUnknown_030015FE
_08033458: .4byte gUnknown_030015C8
_0803345C: .4byte gUnknown_030015C0
_08033460: .4byte gUnknown_030015C4
_08033464: .4byte 0xFFFFFF00
_08033468: .4byte 0xFFFFD000
_0803346C: .4byte 0xFFFFBF00

	thumb_func_start sub_8033470
sub_8033470: @ 0x08033470
	push {r4, r5, r6, lr}
	ldr r5, _0803352C @ =gUnknown_030015AC
	ldr r0, [r5]
	ldr r0, [r0, #8]
	asrs r6, r0, #8
	bl sub_8032B6C
	ldr r0, _08033530 @ =gUnknown_030015B0
	ldr r0, [r0]
	cmp r0, #0
	beq _08033526
	ldr r4, [r5]
	movs r0, #0x10
	ldrsh r1, [r4, r0]
	ldr r0, [r4, #8]
	adds r0, r0, r1
	str r0, [r4, #8]
	movs r0, #0
	strb r0, [r4, #0x12]
	adds r0, r4, #0
	bl GetAnimFrameBaseOffset
	ldr r2, [r4, #0xc]
	ldr r3, [r4]
	lsls r1, r2, #1
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r1, r1, r3
	movs r3, #4
	ldrsh r2, [r1, r3]
	cmp r0, r2
	blt _080334C2
	movs r3, #6
	ldrsh r0, [r1, r3]
	subs r0, r2, r0
	lsls r0, r0, #8
	ldr r1, [r4, #8]
	subs r1, r1, r0
	str r1, [r4, #8]
	movs r0, #1
	strb r0, [r4, #0x12]
_080334C2:
	ldr r4, _08033534 @ =gUnknown_030015C8
	bl sub_8029B2C
	ldr r1, _08033538 @ =gUnknown_030015BC
	lsls r0, r0, #8
	ldr r1, [r1]
	subs r1, r1, r0
	str r1, [r4]
	movs r0, #0xe0
	lsls r0, r0, #0x11
	bl sub_803ADB4
	ldr r2, _0803353C @ =gUnknown_030015C0
	ldr r1, _08033540 @ =gUnknown_030015B4
	ldr r1, [r1]
	muls r1, r0, r1
	asrs r1, r1, #0xc
	str r1, [r2]
	ldr r2, _08033544 @ =gUnknown_030015C4
	ldr r1, _08033548 @ =gUnknown_030015B8
	ldr r1, [r1]
	muls r0, r1, r0
	asrs r0, r0, #0xc
	str r0, [r2]
	ldr r0, [r4]
	bl sub_8029E34
	ldr r3, [r5]
	ldr r0, [r3, #8]
	asrs r4, r0, #8
	cmp r6, r4
	beq _08033526
	ldr r1, [r3, #0xc]
	ldr r2, [r3]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	movs r1, #2
	ldrsh r0, [r0, r1]
	adds r0, r0, r4
	ldr r1, [r3, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	bl sub_80330FC
	ldr r1, _0803354C @ =gUnknown_0300159C
	movs r0, #1
	strb r0, [r1]
_08033526:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0803352C: .4byte gUnknown_030015AC
_08033530: .4byte gUnknown_030015B0
_08033534: .4byte gUnknown_030015C8
_08033538: .4byte gUnknown_030015BC
_0803353C: .4byte gUnknown_030015C0
_08033540: .4byte gUnknown_030015B4
_08033544: .4byte gUnknown_030015C4
_08033548: .4byte gUnknown_030015B8
_0803354C: .4byte gUnknown_0300159C

	thumb_func_start sub_8033550
sub_8033550: @ 0x08033550
	push {r4, r5, lr}
	ldr r0, _0803356C @ =gUnknown_0300159C
	ldrb r1, [r0]
	adds r3, r0, #0
	cmp r1, #0
	beq _08033590
	ldr r0, _08033570 @ =gUnknown_03001598
	ldr r1, [r0]
	adds r2, r0, #0
	cmp r1, #0
	bne _0803357C
	ldr r1, _08033574 @ =0x0400000C
	ldr r4, _08033578 @ =0x00005809
	b _08033580
	.align 2, 0
_0803356C: .4byte gUnknown_0300159C
_08033570: .4byte gUnknown_03001598
_08033574: .4byte 0x0400000C
_08033578: .4byte 0x00005809
_0803357C:
	ldr r1, _080335E8 @ =0x0400000C
	ldr r4, _080335EC @ =0x00005909
_08033580:
	adds r0, r4, #0
	strh r0, [r1]
	movs r0, #0
	strb r0, [r3]
	ldr r0, [r2]
	movs r1, #1
	eors r0, r1
	str r0, [r2]
_08033590:
	ldr r0, _080335F0 @ =gUnknown_030015C8
	ldr r0, [r0]
	lsls r0, r0, #8
	movs r1, #0xf0
	lsls r1, r1, #6
	bl sub_803ADB4
	adds r5, r0, #0
	bl sub_8029EB4
	ldr r1, _080335F4 @ =gUnknown_030015C0
	ldr r4, [r1]
	adds r4, r4, r0
	bl sub_8029E98
	ldr r1, _080335F8 @ =gUnknown_030015C4
	ldr r2, [r1]
	adds r2, r2, r0
	ldr r3, _080335FC @ =0x04000028
	adds r0, r4, #0
	muls r0, r5, r0
	asrs r0, r0, #8
	movs r1, #0x80
	lsls r1, r1, #8
	subs r0, r1, r0
	str r0, [r3]
	adds r3, #4
	adds r0, r2, #0
	muls r0, r5, r0
	asrs r0, r0, #8
	subs r1, r1, r0
	str r1, [r3]
	ldr r0, _08033600 @ =0x04000020
	strh r5, [r0]
	adds r0, #2
	movs r1, #0
	strh r1, [r0]
	adds r0, #2
	strh r1, [r0]
	adds r0, #2
	strh r5, [r0]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080335E8: .4byte 0x0400000C
_080335EC: .4byte 0x00005909
_080335F0: .4byte gUnknown_030015C8
_080335F4: .4byte gUnknown_030015C0
_080335F8: .4byte gUnknown_030015C4
_080335FC: .4byte 0x04000028
_08033600: .4byte 0x04000020

	thumb_func_start sub_8033604
sub_8033604: @ 0x08033604
	push {r4, lr}
	sub sp, #4
	ldr r1, _0803369C @ =0x040000D4
	ldr r0, _080336A0 @ =gStaticData_08169AE8
	str r0, [r1]
	ldr r0, _080336A4 @ =0x05000020
	str r0, [r1, #4]
	ldr r0, _080336A8 @ =0x80000010
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r1, _080336AC @ =0x0600BFC0
	movs r2, #0
	adds r0, r1, #0
	adds r0, #0x3c
_08033620:
	str r2, [r0]
	subs r0, #4
	cmp r0, r1
	bge _08033620
	mov r1, sp
	ldr r2, _080336B0 @ =0x0000FFFF
	adds r0, r2, #0
	strh r0, [r1]
	ldr r1, _0803369C @ =0x040000D4
	mov r3, sp
	str r3, [r1]
	ldr r0, _080336B4 @ =0x0600C000
	str r0, [r1, #4]
	ldr r0, _080336B8 @ =0x81000800
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	bl sub_80336CC
	ldr r0, _080336BC @ =gUnknown_030015B0
	ldr r0, [r0]
	cmp r0, #0
	beq _08033692
	ldr r1, _080336C0 @ =gUnknown_0300159C
	movs r0, #1
	strb r0, [r1]
	ldr r1, _080336C4 @ =gUnknown_03001598
	movs r0, #0
	str r0, [r1]
	ldr r0, _080336C8 @ =gUnknown_030015AC
	ldr r2, [r0]
	ldr r3, [r2, #8]
	asrs r3, r3, #8
	ldr r1, [r2, #0xc]
	ldr r4, [r2]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r4
	movs r1, #2
	ldrsh r0, [r0, r1]
	adds r0, r0, r3
	ldr r1, [r2, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	bl sub_80330FC
	movs r2, #0x80
	lsls r2, r2, #0x13
	ldrh r0, [r2]
	movs r3, #0x80
	lsls r3, r3, #3
	adds r1, r3, #0
	orrs r0, r1
	strh r0, [r2]
	bl sub_8033550
_08033692:
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0803369C: .4byte 0x040000D4
_080336A0: .4byte gStaticData_08169AE8
_080336A4: .4byte 0x05000020
_080336A8: .4byte 0x80000010
_080336AC: .4byte 0x0600BFC0
_080336B0: .4byte 0x0000FFFF
_080336B4: .4byte 0x0600C000
_080336B8: .4byte 0x81000800
_080336BC: .4byte gUnknown_030015B0
_080336C0: .4byte gUnknown_0300159C
_080336C4: .4byte gUnknown_03001598
_080336C8: .4byte gUnknown_030015AC

	thumb_func_start sub_80336CC
sub_80336CC: @ 0x080336CC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	movs r5, #0
	movs r3, #0x81
	lsls r3, r3, #2
	ldr r0, _080337CC @ =gUnknown_030015A0
	ldr r1, _080337D0 @ =gUnknown_030015A4
	ldr r2, [r0]
	ldr r0, [r1]
	muls r0, r2, r0
	adds r0, #1
	lsrs r0, r0, #1
	lsls r0, r0, #2
	str r0, [sp, #4]
	ldr r7, _080337D4 @ =gUnknown_030015A8
	ldr r4, _080337D8 @ =gStaticData_08169AE8
	mov r1, sp
	ldr r6, _080337DC @ =gUnknown_03001600
	adds r2, r5, #0
_080336FA:
	adds r0, r3, r4
	ldr r0, [r0]
	str r0, [r1]
	adds r5, r5, r0
	adds r3, #4
	adds r0, r3, r4
	stm r6!, {r0}
	ldr r0, [sp, #4]
	adds r3, r3, r0
	ldm r1!, {r0}
	lsls r0, r0, #5
	adds r3, r3, r0
	subs r2, #1
	cmp r2, #0
	bge _080336FA
	movs r0, #0xff
	subs r0, r0, r5
	str r0, [r7]
	lsls r0, r0, #6
	ldr r1, _080337E0 @ =0x06008000
	adds r3, r0, r1
	movs r2, #0
_08033726:
	lsls r1, r2, #2
	ldr r4, _080337DC @ =gUnknown_03001600
	adds r0, r1, r4
	ldr r0, [r0]
	add r1, sp
	mov sb, r3
	ldr r3, [sp, #4]
	adds r5, r0, r3
	ldr r1, [r1]
	mov r8, r1
	movs r4, #0
	mov ip, r4
	lsls r0, r1, #4
	adds r2, #1
	mov sl, r2
	cmp ip, r0
	bge _080337B4
	movs r6, #0xf
	movs r7, #0x10
_0803374C:
	ldrb r0, [r5]
	adds r4, r6, #0
	ands r4, r0
	movs r1, #0
	cmp r4, #0
	beq _0803375C
	adds r1, r7, #0
	orrs r1, r4
_0803375C:
	adds r4, r1, #0
	lsrs r0, r0, #4
	ands r0, r6
	adds r5, #1
	movs r1, #0
	cmp r0, #0
	beq _0803376E
	adds r1, r7, #0
	orrs r1, r0
_0803376E:
	adds r0, r1, #0
	ldrb r3, [r5]
	adds r1, r6, #0
	ands r1, r3
	movs r2, #0
	cmp r1, #0
	beq _08033780
	adds r2, r7, #0
	orrs r2, r1
_08033780:
	adds r1, r2, #0
	lsrs r3, r3, #4
	ands r3, r6
	adds r5, #1
	movs r2, #0
	cmp r3, #0
	beq _08033792
	adds r2, r7, #0
	orrs r2, r3
_08033792:
	lsls r0, r0, #8
	orrs r0, r4
	lsls r1, r1, #0x10
	orrs r1, r0
	lsls r0, r2, #0x18
	orrs r0, r1
	mov r1, sb
	adds r1, #4
	mov sb, r1
	subs r1, #4
	stm r1!, {r0}
	movs r3, #1
	add ip, r3
	mov r4, r8
	lsls r0, r4, #4
	cmp ip, r0
	blt _0803374C
_080337B4:
	mov r3, sb
	mov r2, sl
	cmp r2, #0
	ble _08033726
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080337CC: .4byte gUnknown_030015A0
_080337D0: .4byte gUnknown_030015A4
_080337D4: .4byte gUnknown_030015A8
_080337D8: .4byte gStaticData_08169AE8
_080337DC: .4byte gUnknown_03001600
_080337E0: .4byte 0x06008000

	thumb_func_start sub_80337E4
sub_80337E4: @ 0x080337E4
	push {lr}
	ldr r0, _080337F4 @ =gUnknown_030015AC
	ldr r0, [r0]
	bl mem_free
	pop {r0}
	bx r0
	.align 2, 0
_080337F4: .4byte gUnknown_030015AC

	thumb_func_start nullsub_34
nullsub_34: @ 0x080337F8
	bx lr
	.align 2, 0

	thumb_func_start sub_80337FC
sub_80337FC: @ 0x080337FC
	movs r0, #0
	bx lr

	thumb_func_start nullsub_35
nullsub_35: @ 0x08033800
	bx lr
	.align 2, 0

