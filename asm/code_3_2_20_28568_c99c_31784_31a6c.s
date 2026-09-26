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

