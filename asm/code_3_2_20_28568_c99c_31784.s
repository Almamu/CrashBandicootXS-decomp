.include "asm/macros.inc"

.syntax unified
.arm
	thumb_func_start sub_8031784
sub_8031784: @ 0x08031784
	push {r4, lr}
	ldr r0, _08031794 @ =gUnknown_03001538
	ldr r0, [r0]
	cmp r0, #0
	bne _08031798
	movs r0, #1
	rsbs r0, r0, #0
	b _080317B4
	.align 2, 0
_08031794: .4byte gUnknown_03001538
_08031798:
	ldr r0, _080317BC @ =gUnknown_0300156C
	ldr r4, [r0]
	movs r0, #0x64
	muls r0, r4, r0
	ldr r1, _080317C0 @ =gUnknown_03001568
	ldr r1, [r1]
	ldr r1, [r1]
	bl sub_803ADB4
	cmp r0, #0
	bne _080317B4
	cmp r4, #0
	ble _080317B4
	movs r0, #1
_080317B4:
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_080317BC: .4byte gUnknown_0300156C
_080317C0: .4byte gUnknown_03001568

	thumb_func_start sub_80317C4
sub_80317C4: @ 0x080317C4
	push {lr}
	ldr r0, _080317D4 @ =gUnknown_03001534
	ldr r0, [r0]
	bl mem_free
	pop {r0}
	bx r0
	.align 2, 0
_080317D4: .4byte gUnknown_03001534

	thumb_func_start nullsub_30
nullsub_30: @ 0x080317D8
	bx lr
	.align 2, 0

	thumb_func_start nullsub_31
nullsub_31: @ 0x080317DC
	bx lr
	.align 2, 0

	thumb_func_start sub_80317E0
sub_80317E0: @ 0x080317E0
	push {r4, lr}
	adds r4, r0, #0
	bl sub_802A980
	ldr r0, _08031808 @ =gUnknown_030013C0
	ldr r0, [r0]
	ldr r1, _0803180C @ =0xFFFFFE00
	adds r0, r0, r1
	ldr r1, [r4, #0x34]
	cmp r1, r0
	bge _08031810
	ldr r0, [r4, #0x58]
	cmp r0, #0
	beq _08031828
	bl sub_8032138
	movs r0, #0
	str r0, [r4, #0x58]
	b _08031828
	.align 2, 0
_08031808: .4byte gUnknown_030013C0
_0803180C: .4byte 0xFFFFFE00
_08031810:
	ldr r1, [r4, #0x28]
	cmp r1, #2
	bne _0803181C
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	bne _08031828
_0803181C:
	cmp r1, #1
	bne _08031844
	ldr r1, [r4, #0x20]
	ldr r0, _08031840 @ =0xFFFF1F00
	cmp r1, r0
	bge _08031844
_08031828:
	cmp r4, #0
	beq _0803184A
	ldr r1, [r4, #0x50]
	movs r2, #8
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
	b _0803184A
	.align 2, 0
_08031840: .4byte 0xFFFF1F00
_08031844:
	adds r0, r4, #0
	bl sub_8031A08
_0803184A:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8031850
sub_8031850: @ 0x08031850
	movs r1, #0
	str r1, [r0, #0x58]
	bx lr
	.align 2, 0

	thumb_func_start sub_8031858
sub_8031858: @ 0x08031858
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x54]
	subs r0, r0, r1
	str r0, [r4, #0x54]
	cmp r0, #0
	bgt _080318A8
	adds r0, r4, #0
	adds r0, #0x5c
	movs r5, #0
	movs r6, #1
	strb r6, [r0]
	ldr r2, [r4, #0x58]
	cmp r2, #0
	beq _08031886
	ldr r1, [r2, #0x50]
	movs r3, #0x38
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r1, [r1, #0x3c]
	bl sub_803AD7C
	str r5, [r4, #0x58]
_08031886:
	ldr r0, _080318B0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x2e
	bl PlaySfx
	movs r0, #2
	str r0, [r4, #0x28]
	str r5, [r4, #0x44]
	str r6, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
_080318A8:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_080318B0: .4byte gUnknown_030012BC

	thumb_func_start sub_80318B4
sub_80318B4: @ 0x080318B4
	movs r2, #0
	str r2, [r0, #0x58]
	str r2, [r0, #0x60]
	movs r1, #1
	str r1, [r0, #0x28]
	str r2, [r0, #0x44]
	str r2, [r0, #0xc]
	ldr r1, [r0]
	ldrh r1, [r1]
	movs r3, #0
	strh r1, [r0, #0x10]
	strb r3, [r0, #0x12]
	str r2, [r0, #8]
	bx lr

	thumb_func_start sub_80318D0
sub_80318D0: @ 0x080318D0
	push {r4, lr}
	adds r4, r0, #0
	str r1, [r4, #0x1c]
	str r2, [r4, #0x20]
	str r3, [r4, #0x24]
	ldr r0, [r4, #0x44]
	adds r0, #1
	str r0, [r4, #0x44]
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
	blt _0803191A
	movs r3, #6
	ldrsh r0, [r1, r3]
	subs r0, r2, r0
	lsls r0, r0, #8
	ldr r1, [r4, #8]
	subs r1, r1, r0
	str r1, [r4, #8]
	movs r0, #1
	strb r0, [r4, #0x12]
_0803191A:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8031920
sub_8031920: @ 0x08031920
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, [sp, #0x14]
	ldr r6, [sp, #0x18]
	movs r5, #2
	str r0, [sp]
	adds r0, r4, #0
	bl InitActorPart
	str r5, [r4, #0x54]
	ldr r0, _08031950 @ =gStaticData_087E5294
	str r0, [r4, #0x50]
	str r6, [r4, #0x58]
	adds r1, r4, #0
	adds r1, #0x5c
	movs r0, #0
	strb r0, [r1]
	adds r0, r4, #0
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08031950: .4byte gStaticData_087E5294

	thumb_func_start sub_8031954
sub_8031954: @ 0x08031954
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x44]
	adds r0, #1
	str r0, [r4, #0x44]
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
	blt _08031998
	movs r3, #6
	ldrsh r0, [r1, r3]
	subs r0, r2, r0
	lsls r0, r0, #8
	ldr r1, [r4, #8]
	subs r1, r1, r0
	str r1, [r4, #8]
	movs r0, #1
	strb r0, [r4, #0x12]
_08031998:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80319A0
sub_80319A0: @ 0x080319A0
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x20]
	ldr r1, [r4, #0x60]
	adds r0, r0, r1
	str r0, [r4, #0x20]
	subs r1, #6
	str r1, [r4, #0x60]
	ldr r0, _08031A00 @ =0xFFFFFED4
	cmp r1, r0
	ble _080319B8
	str r0, [r4, #0x60]
_080319B8:
	ldr r0, [r4, #0x44]
	adds r0, #1
	str r0, [r4, #0x44]
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
	blt _080319F8
	movs r3, #6
	ldrsh r0, [r1, r3]
	subs r0, r2, r0
	lsls r0, r0, #8
	ldr r1, [r4, #8]
	subs r1, r1, r0
	str r1, [r4, #8]
	movs r0, #1
	strb r0, [r4, #0x12]
_080319F8:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08031A00: .4byte 0xFFFFFED4

	thumb_func_start nullsub_32
nullsub_32: @ 0x08031A04
	bx lr
	.align 2, 0

	thumb_func_start sub_8031A08
sub_8031A08: @ 0x08031A08
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r1, _08031A34 @ =gStaticData_0817C414
	ldr r0, [r4, #0x28]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r7, #2
	ldrsh r2, [r0, r7]
	adds r7, r1, #0
	cmp r2, #0
	ble _08031A38
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
	b _08031A3E
	.align 2, 0
_08031A34: .4byte gStaticData_0817C414
_08031A38:
	adds r0, r7, #4
	adds r0, r3, r0
	ldr r3, [r0]
_08031A3E:
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _08031A54
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _08031A56
_08031A54:
	adds r0, r1, #0
_08031A56:
	adds r0, r4, r0
	bl sub_803AD84
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8031A64
sub_8031A64: @ 0x08031A64
	adds r0, #0x5c
	ldrb r0, [r0]
	bx lr
	.align 2, 0

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

	thumb_func_start sub_8033804
sub_8033804: @ 0x08033804
	ldr r2, _08033820 @ =gUnknown_030015FE
	ldrb r0, [r2]
	cmp r0, #0
	bne _0803381C
	ldr r1, _08033824 @ =gUnknown_030015FC
	movs r3, #0
	ldrsh r0, [r1, r3]
	cmp r0, #0
	bne _0803381C
	movs r0, #1
	strh r0, [r1]
	strb r0, [r2]
_0803381C:
	bx lr
	.align 2, 0
_08033820: .4byte gUnknown_030015FE
_08033824: .4byte gUnknown_030015FC

	thumb_func_start sub_8033828
sub_8033828: @ 0x08033828
	push {r4, lr}
	lsls r0, r0, #0x18
	lsrs r3, r0, #0x18
	ldr r4, _08033854 @ =gUnknown_03001594
	ldr r0, [r4]
	ldr r2, _08033858 @ =gUnknown_030008B4
	cmp r0, #0
	bne _08033844
	ldr r1, _0803385C @ =gUnknown_03001590
	ldr r0, [r2]
	ldrh r0, [r0, #0x1e]
	strh r0, [r1]
	movs r0, #1
	str r0, [r4]
_08033844:
	cmp r3, #0
	beq _08033864
	ldr r0, [r2]
	ldr r2, _08033860 @ =0x00007FFF
	adds r1, r2, #0
	strh r1, [r0, #0x1e]
	b _0803386C
	.align 2, 0
_08033854: .4byte gUnknown_03001594
_08033858: .4byte gUnknown_030008B4
_0803385C: .4byte gUnknown_03001590
_08033860: .4byte 0x00007FFF
_08033864:
	ldr r2, [r2]
	ldr r0, _08033878 @ =gUnknown_03001590
	ldrh r1, [r0]
	strh r1, [r2, #0x1e]
_0803386C:
	ldr r0, _0803387C @ =gUnknown_030008B8
	ldr r0, [r0]
	strh r1, [r0, #0x1e]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08033878: .4byte gUnknown_03001590
_0803387C: .4byte gUnknown_030008B8

	thumb_func_start sub_8033880
sub_8033880: @ 0x08033880
	ldr r0, _08033888 @ =gUnknown_030015F8
	ldr r0, [r0]
	bx lr
	.align 2, 0
_08033888: .4byte gUnknown_030015F8

	thumb_func_start sub_803388C
sub_803388C: @ 0x0803388C
	push {lr}
	ldr r0, _080338B8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	ldr r1, _080338BC @ =gUnknown_030015F8
	ldr r0, [r1]
	subs r2, r0, #1
	str r2, [r1]
	cmp r2, #0
	bne _080338B4
	ldr r0, _080338C0 @ =gUnknown_030015FF
	strb r2, [r0]
	movs r0, #5
	movs r1, #0
	bl sub_803390C
_080338B4:
	pop {r0}
	bx r0
	.align 2, 0
_080338B8: .4byte gUnknown_030012BC
_080338BC: .4byte gUnknown_030015F8
_080338C0: .4byte gUnknown_030015FF

	thumb_func_start sub_80338C4
sub_80338C4: @ 0x080338C4
	ldr r0, _080338CC @ =gUnknown_030015DC
	ldr r0, [r0]
	bx lr
	.align 2, 0
_080338CC: .4byte gUnknown_030015DC

	thumb_func_start sub_80338D0
sub_80338D0: @ 0x080338D0
	ldr r0, _080338D8 @ =gUnknown_030015B0
	ldr r0, [r0]
	bx lr
	.align 2, 0
_080338D8: .4byte gUnknown_030015B0

	thumb_func_start sub_80338DC
sub_80338DC: @ 0x080338DC
	ldr r0, _080338E4 @ =gUnknown_030015D8
	ldr r0, [r0]
	bx lr
	.align 2, 0
_080338E4: .4byte gUnknown_030015D8

	thumb_func_start sub_80338E8
sub_80338E8: @ 0x080338E8
	ldr r0, _080338F0 @ =gUnknown_030015BC
	ldr r0, [r0]
	bx lr
	.align 2, 0
_080338F0: .4byte gUnknown_030015BC

	thumb_func_start sub_80338F4
sub_80338F4: @ 0x080338F4
	ldr r0, _080338FC @ =gUnknown_030015B8
	ldr r0, [r0]
	bx lr
	.align 2, 0
_080338FC: .4byte gUnknown_030015B8

	thumb_func_start sub_8033900
sub_8033900: @ 0x08033900
	ldr r0, _08033908 @ =gUnknown_030015B4
	ldr r0, [r0]
	bx lr
	.align 2, 0
_08033908: .4byte gUnknown_030015B4

	thumb_func_start sub_803390C
sub_803390C: @ 0x0803390C
	push {r4, lr}
	ldr r2, _08033950 @ =gUnknown_030015B0
	str r0, [r2]
	ldr r0, _08033954 @ =gUnknown_030015AC
	ldr r4, [r0]
	str r1, [r4, #0xc]
	ldr r2, [r4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
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
	blt _08033948
	movs r0, #0
	str r0, [r4, #8]
_08033948:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08033950: .4byte gUnknown_030015B0
_08033954: .4byte gUnknown_030015AC

	thumb_func_start nullsub_36
nullsub_36: @ 0x08033958
	bx lr
	.align 2, 0

	thumb_func_start sub_803395C
sub_803395C: @ 0x0803395C
	push {r4, r5, lr}
	ldr r2, _080339B8 @ =gUnknown_030015BC
	ldr r1, _080339BC @ =gUnknown_030015D4
	ldr r0, [r2]
	ldr r1, [r1]
	adds r0, r0, r1
	str r0, [r2]
	ldr r0, _080339C0 @ =gUnknown_030015C8
	ldr r1, [r0]
	ldr r0, _080339C4 @ =0x000081FF
	cmp r1, r0
	bgt _080339B0
	ldr r1, _080339C8 @ =gUnknown_030015CC
	ldr r0, _080339CC @ =gUnknown_030015D0
	movs r5, #0
	str r5, [r0]
	str r5, [r1]
	movs r1, #2
	ldr r0, _080339D0 @ =gUnknown_030015B0
	str r1, [r0]
	ldr r0, _080339D4 @ =gUnknown_030015AC
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
	blt _080339B0
	str r5, [r4, #8]
_080339B0:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080339B8: .4byte gUnknown_030015BC
_080339BC: .4byte gUnknown_030015D4
_080339C0: .4byte gUnknown_030015C8
_080339C4: .4byte 0x000081FF
_080339C8: .4byte gUnknown_030015CC
_080339CC: .4byte gUnknown_030015D0
_080339D0: .4byte gUnknown_030015B0
_080339D4: .4byte gUnknown_030015AC

	thumb_func_start nullsub_37
nullsub_37: @ 0x080339D8
	bx lr
	.align 2, 0

	thumb_func_start sub_80339DC
sub_80339DC: @ 0x080339DC
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	adds r5, r0, #0
	bl sub_8033900
	movs r1, #0x80
	lsls r1, r1, #6
	adds r0, r0, r1
	str r0, [r5, #0x1c]
	bl sub_80338F4
	movs r2, #0xc0
	lsls r2, r2, #6
	adds r0, r0, r2
	str r0, [r5, #0x20]
	bl sub_80338E8
	ldr r1, _08033A98 @ =0xFFFFFF00
	adds r6, r0, r1
	str r6, [r5, #0x24]
	ldr r2, [r5, #0x64]
	mov sb, r2
	cmp r2, #0
	bne _08033AAC
	ldr r0, _08033A9C @ =gUnknown_03000884
	ldr r4, [r0]
	ldr r0, [r4, #0x24]
	subs r0, r0, r6
	subs r1, #0xaa
	bl sub_803ADB4
	adds r1, r0, #0
	cmp r1, #0
	ble _08033AB2
	movs r0, #0x80
	lsls r0, r0, #5
	bl sub_803ADB4
	ldr r1, [r4, #0x1c]
	ldr r2, [r5, #0x1c]
	mov ip, r2
	subs r1, r1, r2
	adds r3, r1, #0
	muls r3, r0, r3
	asrs r1, r3, #0xc
	mov r8, r1
	ldr r1, [r4, #0x20]
	ldr r7, [r5, #0x20]
	subs r1, r1, r7
	adds r2, r1, #0
	muls r2, r0, r2
	asrs r4, r2, #0xc
	asrs r3, r3, #0x1f
	mov r1, r8
	eors r1, r3
	subs r1, r1, r3
	asrs r2, r2, #0x1f
	adds r0, r4, #0
	eors r0, r2
	subs r0, r0, r2
	adds r1, r1, r0
	ldr r0, _08033AA0 @ =0x00000FFF
	cmp r1, r0
	bgt _08033AB2
	str r4, [sp]
	mov r0, ip
	adds r1, r7, #0
	adds r2, r6, #0
	mov r3, r8
	bl sub_802E674
	ldr r0, [r5, #0x1c]
	ldr r1, [r5, #0x20]
	ldr r2, [r5, #0x24]
	bl sub_802E504
	ldr r4, [r5, #0x68]
	adds r4, #1
	str r4, [r5, #0x68]
	bl sub_80338C4
	ldr r0, [r0, #0x14]
	cmp r4, r0
	bne _08033AA4
	mov r2, sb
	str r2, [r5, #0x68]
	bl sub_80338C4
	ldr r0, [r0, #0x18]
	b _08033AB0
	.align 2, 0
_08033A98: .4byte 0xFFFFFF00
_08033A9C: .4byte gUnknown_03000884
_08033AA0: .4byte 0x00000FFF
_08033AA4:
	bl sub_80338C4
	ldr r0, [r0, #0x10]
	b _08033AB0
_08033AAC:
	mov r0, sb
	subs r0, #1
_08033AB0:
	str r0, [r5, #0x64]
_08033AB2:
	ldr r1, [r5, #0x34]
	movs r0, #0x96
	lsls r0, r0, #7
	cmp r1, r0
	ble _08033AD0
	movs r1, #0
	str r1, [r5, #0x28]
	str r1, [r5, #0x44]
	str r1, [r5, #0xc]
	ldr r0, [r5]
	ldrh r0, [r0]
	movs r2, #0
	strh r0, [r5, #0x10]
	strb r2, [r5, #0x12]
	str r1, [r5, #8]
_08033AD0:
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8033AE0
sub_8033AE0: @ 0x08033AE0
	push {r4, r5, lr}
	adds r5, r0, #0
	adds r4, r1, #0
	bl sub_8033804
	ldr r0, [r5, #0x54]
	subs r0, r0, r4
	str r0, [r5, #0x54]
	cmp r0, #0
	bgt _08033B2C
	adds r1, r5, #0
	adds r1, #0x6c
	movs r4, #0
	movs r0, #1
	strb r0, [r1]
	bl sub_803388C
	movs r0, #2
	str r0, [r5, #0x28]
	str r4, [r5, #0x44]
	str r0, [r5, #0xc]
	ldr r0, [r5]
	ldrh r0, [r0, #0x18]
	movs r1, #0
	strh r0, [r5, #0x10]
	strb r1, [r5, #0x12]
	str r4, [r5, #8]
	ldr r0, _08033B28 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	b _08033B3A
	.align 2, 0
_08033B28: .4byte gUnknown_030012BC
_08033B2C:
	ldr r0, _08033B40 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x45
	bl PlaySfx
_08033B3A:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08033B40: .4byte gUnknown_030012BC

	thumb_func_start sub_8033B44
sub_8033B44: @ 0x08033B44
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r1, _08033B70 @ =gStaticData_0817C4E0
	ldr r0, [r4, #0x28]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r7, #2
	ldrsh r2, [r0, r7]
	adds r7, r1, #0
	cmp r2, #0
	ble _08033B74
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
	b _08033B7A
	.align 2, 0
_08033B70: .4byte gStaticData_0817C4E0
_08033B74:
	adds r0, r7, #4
	adds r0, r3, r0
	ldr r3, [r0]
_08033B7A:
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _08033B90
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _08033B92
_08033B90:
	adds r0, r1, #0
_08033B92:
	adds r0, r4, r0
	bl sub_803AD84
	ldr r0, [r4, #0x28]
	movs r1, #1
	cmp r0, #2
	bne _08033BA8
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _08033BA8
	movs r1, #0
_08033BA8:
	cmp r1, #0
	beq _08033BB2
	adds r0, r4, #0
	bl sub_802A7B8
_08033BB2:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8033BB8
sub_8033BB8: @ 0x08033BB8
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	sub sp, #4
	adds r4, r0, #0
	adds r6, r2, #0
	mov r8, r3
	ldr r0, [sp, #0x18]
	movs r5, #0xf
	str r0, [sp]
	adds r0, r4, #0
	bl InitActorPart
	str r5, [r4, #0x54]
	ldr r0, _08033BF8 @ =gStaticData_087E54E4
	str r0, [r4, #0x50]
	str r6, [r4, #0x58]
	mov r0, r8
	str r0, [r4, #0x5c]
	movs r1, #0
	str r1, [r4, #0x28]
	adds r0, r4, #0
	adds r0, #0x6c
	strb r1, [r0]
	adds r0, r4, #0
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08033BF8: .4byte gStaticData_087E54E4

	thumb_func_start sub_8033BFC
sub_8033BFC: @ 0x08033BFC
	push {r4, lr}
	adds r4, r0, #0
	movs r0, #0x80
	lsls r0, r0, #3
	bl sub_8029E28
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _08033C22
	cmp r4, #0
	beq _08033C22
	ldr r1, [r4, #0x50]
	movs r2, #8
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_08033C22:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8033C28
sub_8033C28: @ 0x08033C28
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8033900
	movs r1, #0x80
	lsls r1, r1, #6
	adds r0, r0, r1
	str r0, [r4, #0x1c]
	bl sub_80338F4
	movs r1, #0xc0
	lsls r1, r1, #6
	adds r0, r0, r1
	str r0, [r4, #0x20]
	bl sub_80338E8
	ldr r1, _08033C7C @ =0xFFFFFF00
	adds r0, r0, r1
	str r0, [r4, #0x24]
	ldr r1, [r4, #0x34]
	ldr r0, _08033C80 @ =0x00004AFF
	cmp r1, r0
	bgt _08033C76
	bl sub_80338C4
	ldr r0, [r0, #0x10]
	str r0, [r4, #0x64]
	movs r2, #0
	str r2, [r4, #0x68]
	movs r0, #1
	str r0, [r4, #0x28]
	str r2, [r4, #0x44]
	str r0, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
_08033C76:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08033C7C: .4byte 0xFFFFFF00
_08033C80: .4byte 0x00004AFF

	thumb_func_start sub_8033C84
sub_8033C84: @ 0x08033C84
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r1, _08033CB0 @ =gStaticData_0817C4E0
	ldr r0, [r4, #0x28]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r7, #2
	ldrsh r2, [r0, r7]
	adds r7, r1, #0
	cmp r2, #0
	ble _08033CB4
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
	b _08033CBA
	.align 2, 0
_08033CB0: .4byte gStaticData_0817C4E0
_08033CB4:
	adds r0, r7, #4
	adds r0, r3, r0
	ldr r3, [r0]
_08033CBA:
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _08033CD0
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _08033CD2
_08033CD0:
	adds r0, r1, #0
_08033CD2:
	adds r0, r4, r0
	bl sub_803AD84
	ldr r0, [r4, #0x28]
	cmp r0, #2
	bne _08033CE8
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _08033CE8
	movs r0, #0
	b _08033CEA
_08033CE8:
	movs r0, #1
_08033CEA:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_8033CF0
sub_8033CF0: @ 0x08033CF0
	adds r0, #0x6c
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8033CF8
sub_8033CF8: @ 0x08033CF8
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r5, r0, #0
	bl sub_8033900
	movs r1, #0xf0
	lsls r1, r1, #5
	adds r0, r0, r1
	str r0, [r5, #0x1c]
	bl sub_80338F4
	ldr r2, _08033D88 @ =0xFFFFD000
	adds r0, r0, r2
	str r0, [r5, #0x20]
	bl sub_80338E8
	ldr r2, _08033D8C @ =0xFFFFFF00
	adds r1, r0, r2
	str r1, [r5, #0x24]
	ldr r7, [r5, #0x64]
	cmp r7, #0
	bne _08033DE2
	ldr r0, _08033D90 @ =gUnknown_03000884
	ldr r6, [r0]
	ldr r0, [r6, #0x24]
	subs r0, r0, r1
	ldr r1, _08033D94 @ =0xFFFFFE56
	bl sub_803ADB4
	adds r1, r0, #0
	cmp r1, #0
	ble _08033DE6
	movs r0, #0x80
	lsls r0, r0, #5
	bl sub_803ADB4
	ldr r1, [r6, #0x1c]
	ldr r2, [r5, #0x1c]
	subs r1, r1, r2
	adds r4, r1, #0
	muls r4, r0, r4
	asrs r3, r4, #0xc
	ldr r1, [r6, #0x20]
	ldr r2, [r5, #0x20]
	subs r1, r1, r2
	muls r0, r1, r0
	asrs r1, r0, #0xc
	asrs r4, r4, #0x1f
	eors r3, r4
	subs r3, r3, r4
	asrs r0, r0, #0x1f
	eors r1, r0
	subs r1, r1, r0
	adds r3, r3, r1
	ldr r0, _08033D98 @ =0x00000FFF
	cmp r3, r0
	bgt _08033DE6
	movs r0, #3
	bl sub_8000E1C
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	cmp r0, #0
	bne _08033D9C
	ldr r1, [r5, #0x1c]
	ldr r2, [r5, #0x20]
	ldr r3, [r5, #0x24]
	str r7, [sp]
	movs r0, #5
	bl sub_802E170
	b _08033DBE
	.align 2, 0
_08033D88: .4byte 0xFFFFD000
_08033D8C: .4byte 0xFFFFFF00
_08033D90: .4byte gUnknown_03000884
_08033D94: .4byte 0xFFFFFE56
_08033D98: .4byte 0x00000FFF
_08033D9C:
	cmp r0, #1
	bne _08033DB0
	ldr r1, [r5, #0x1c]
	ldr r2, [r5, #0x20]
	ldr r3, [r5, #0x24]
	str r7, [sp]
	movs r0, #6
	bl sub_802E170
	b _08033DBE
_08033DB0:
	ldr r1, [r5, #0x1c]
	ldr r2, [r5, #0x20]
	ldr r3, [r5, #0x24]
	str r7, [sp]
	movs r0, #8
	bl sub_802E170
_08033DBE:
	ldr r4, [r5, #0x68]
	adds r4, #1
	str r4, [r5, #0x68]
	bl sub_80338C4
	ldr r0, [r0, #0x20]
	cmp r4, r0
	bne _08033DDA
	movs r0, #0
	str r0, [r5, #0x68]
	bl sub_80338C4
	ldr r0, [r0, #0x24]
	b _08033DE4
_08033DDA:
	bl sub_80338C4
	ldr r0, [r0, #0x1c]
	b _08033DE4
_08033DE2:
	subs r0, r7, #1
_08033DE4:
	str r0, [r5, #0x64]
_08033DE6:
	ldr r1, [r5, #0x34]
	movs r0, #0x96
	lsls r0, r0, #7
	cmp r1, r0
	ble _08033E0E
	bl sub_80338D0
	cmp r0, #3
	bne _08033E0E
	movs r2, #0
	movs r0, #2
	str r2, [r5, #0x28]
	str r2, [r5, #0x44]
	str r0, [r5, #0xc]
	ldr r0, [r5]
	ldrh r0, [r0, #0x18]
	movs r1, #0
	strh r0, [r5, #0x10]
	strb r1, [r5, #0x12]
	str r2, [r5, #8]
_08033E0E:
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8033E18
sub_8033E18: @ 0x08033E18
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	adds r4, r1, #0
	ldr r6, [r5, #0x28]
	cmp r6, #1
	bne _08033E76
	bl sub_8033804
	ldr r0, [r5, #0x54]
	subs r0, r0, r4
	str r0, [r5, #0x54]
	cmp r0, #0
	bgt _08033E68
	adds r0, r5, #0
	adds r0, #0x6c
	movs r4, #0
	strb r6, [r0]
	bl sub_803388C
	movs r0, #2
	movs r1, #3
	str r0, [r5, #0x28]
	str r4, [r5, #0x44]
	str r1, [r5, #0xc]
	ldr r0, [r5]
	ldrh r0, [r0, #0x24]
	movs r1, #0
	strh r0, [r5, #0x10]
	strb r1, [r5, #0x12]
	str r4, [r5, #8]
	ldr r0, _08033E64 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	b _08033E76
	.align 2, 0
_08033E64: .4byte gUnknown_030012BC
_08033E68:
	ldr r0, _08033E7C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x45
	bl PlaySfx
_08033E76:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08033E7C: .4byte gUnknown_030012BC

	thumb_func_start sub_8033E80
sub_8033E80: @ 0x08033E80
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r1, _08033EAC @ =gStaticData_0817C4F8
	ldr r0, [r4, #0x28]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r7, #2
	ldrsh r2, [r0, r7]
	adds r7, r1, #0
	cmp r2, #0
	ble _08033EB0
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
	b _08033EB6
	.align 2, 0
_08033EAC: .4byte gStaticData_0817C4F8
_08033EB0:
	adds r0, r7, #4
	adds r0, r3, r0
	ldr r3, [r0]
_08033EB6:
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _08033ECC
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _08033ECE
_08033ECC:
	adds r0, r1, #0
_08033ECE:
	adds r0, r4, r0
	bl sub_803AD84
	ldr r0, [r4, #0x28]
	movs r1, #1
	cmp r0, #2
	bne _08033EE4
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _08033EE4
	movs r1, #0
_08033EE4:
	cmp r1, #0
	beq _08033EEE
	adds r0, r4, #0
	bl sub_802A7B8
_08033EEE:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8033EF4
sub_8033EF4: @ 0x08033EF4
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	sub sp, #4
	adds r4, r0, #0
	adds r6, r2, #0
	mov r8, r3
	ldr r0, [sp, #0x18]
	movs r5, #0x19
	str r0, [sp]
	adds r0, r4, #0
	bl InitActorPart
	str r5, [r4, #0x54]
	ldr r0, _08033F44 @ =gStaticData_087E551C
	str r0, [r4, #0x50]
	str r6, [r4, #0x58]
	mov r0, r8
	str r0, [r4, #0x5c]
	movs r1, #0
	str r1, [r4, #0x28]
	str r1, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0]
	movs r2, #0
	strh r0, [r4, #0x10]
	strb r2, [r4, #0x12]
	str r1, [r4, #8]
	adds r0, r4, #0
	adds r0, #0x6c
	strb r2, [r0]
	adds r0, r4, #0
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08033F44: .4byte gStaticData_087E551C

	thumb_func_start sub_8033F48
sub_8033F48: @ 0x08033F48
	push {r4, lr}
	adds r4, r0, #0
	movs r0, #0x80
	lsls r0, r0, #3
	bl sub_8029E28
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _08033F6E
	cmp r4, #0
	beq _08033F6E
	ldr r1, [r4, #0x50]
	movs r2, #8
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_08033F6E:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8033F74
sub_8033F74: @ 0x08033F74
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8033900
	movs r1, #0xf0
	lsls r1, r1, #5
	adds r0, r0, r1
	str r0, [r4, #0x1c]
	bl sub_80338F4
	ldr r1, _08033FD8 @ =0xFFFFD000
	adds r0, r0, r1
	str r0, [r4, #0x20]
	bl sub_80338E8
	ldr r1, _08033FDC @ =0xFFFFFF00
	adds r0, r0, r1
	str r0, [r4, #0x24]
	bl sub_8033880
	cmp r0, #2
	bgt _08033FD2
	bl sub_80338D0
	cmp r0, #2
	beq _08033FB8
	bl sub_80338D0
	cmp r0, #3
	bne _08033FD2
	ldr r1, [r4, #0x34]
	ldr r0, _08033FE0 @ =0x00004AFF
	cmp r1, r0
	bgt _08033FD2
_08033FB8:
	movs r2, #0
	str r2, [r4, #0x64]
	str r2, [r4, #0x68]
	movs r0, #1
	str r0, [r4, #0x28]
	str r2, [r4, #0x44]
	str r0, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
_08033FD2:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08033FD8: .4byte 0xFFFFD000
_08033FDC: .4byte 0xFFFFFF00
_08033FE0: .4byte 0x00004AFF

	thumb_func_start sub_8033FE4
sub_8033FE4: @ 0x08033FE4
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r1, _08034010 @ =gStaticData_0817C4F8
	ldr r0, [r4, #0x28]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r7, #2
	ldrsh r2, [r0, r7]
	adds r7, r1, #0
	cmp r2, #0
	ble _08034014
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
	b _0803401A
	.align 2, 0
_08034010: .4byte gStaticData_0817C4F8
_08034014:
	adds r0, r7, #4
	adds r0, r3, r0
	ldr r3, [r0]
_0803401A:
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _08034030
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _08034032
_08034030:
	adds r0, r1, #0
_08034032:
	adds r0, r4, r0
	bl sub_803AD84
	ldr r0, [r4, #0x28]
	cmp r0, #2
	bne _08034048
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _08034048
	movs r0, #0
	b _0803404A
_08034048:
	movs r0, #1
_0803404A:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_8034050
sub_8034050: @ 0x08034050
	adds r0, #0x6c
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8034058
sub_8034058: @ 0x08034058
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	adds r5, r0, #0
	adds r6, r1, #0
	adds r7, r2, #0
	mov r8, r3
	add r0, sp, #0x24
	ldrb r0, [r0]
	mov sb, r0
	bl sub_80338DC
	movs r4, #0x10
	cmp r0, #0
	bne _0803407C
	movs r4, #0x18
_0803407C:
	ldr r0, [sp, #0x20]
	str r0, [sp]
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r7, #0
	mov r3, r8
	bl InitActorPart
	str r4, [r5, #0x54]
	ldr r0, _080340D4 @ =gStaticData_087E5554
	str r0, [r5, #0x50]
	adds r3, r5, #0
	adds r3, #0x59
	movs r4, #0
	mov r0, sb
	strb r0, [r3]
	str r4, [r5, #0x28]
	ldrb r0, [r3]
	movs r2, #1
	cmp r0, #0
	beq _080340A8
	movs r2, #0
_080340A8:
	str r4, [r5, #0x28]
	str r4, [r5, #0x44]
	str r2, [r5, #0xc]
	ldr r1, [r5]
	lsls r0, r2, #1
	adds r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r5, #0x10]
	strb r1, [r5, #0x12]
	str r4, [r5, #8]
	adds r0, r5, #0
	adds r0, #0x58
	strb r1, [r0]
	ldrb r0, [r3]
	cmp r0, #0
	beq _080340DC
	ldr r0, _080340D8 @ =0xFFFFBF00
	b _080340E0
	.align 2, 0
_080340D4: .4byte gStaticData_087E5554
_080340D8: .4byte 0xFFFFBF00
_080340DC:
	movs r0, #0x84
	lsls r0, r0, #8
_080340E0:
	str r0, [r5, #0x5c]
	movs r0, #0xa0
	lsls r0, r0, #4
	str r0, [r5, #0x60]
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r5, #0x64]
	adds r0, r5, #0
	adds r0, #0x2c
	movs r4, #0
	strb r4, [r0]
	bl sub_80338C4
	ldr r0, [r0, #4]
	str r0, [r5, #0x68]
	str r4, [r5, #0x6c]
	adds r0, r5, #0
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_8034110
sub_8034110: @ 0x08034110
	push {r4, r5, lr}
	adds r5, r0, #0
	adds r4, r1, #0
	bl sub_8033804
	ldr r0, [r5, #0x54]
	subs r0, r0, r4
	str r0, [r5, #0x54]
	cmp r0, #0
	bgt _08034170
	bl sub_803388C
	adds r0, r5, #0
	adds r0, #0x58
	movs r3, #0
	movs r1, #1
	strb r1, [r0]
	subs r0, #0x2c
	strb r1, [r0]
	adds r0, #0x2d
	ldrb r0, [r0]
	movs r2, #1
	cmp r0, #0
	beq _08034142
	movs r2, #0
_08034142:
	str r1, [r5, #0x28]
	str r3, [r5, #0x44]
	str r2, [r5, #0xc]
	ldr r1, [r5]
	lsls r0, r2, #1
	adds r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r5, #0x10]
	strb r1, [r5, #0x12]
	str r3, [r5, #8]
	ldr r0, _0803416C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	b _0803417E
	.align 2, 0
_0803416C: .4byte gUnknown_030012BC
_08034170:
	ldr r0, _08034184 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x45
	bl PlaySfx
_0803417E:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08034184: .4byte gUnknown_030012BC

	thumb_func_start sub_8034188
sub_8034188: @ 0x08034188
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	bl sub_802A7B8
	bl sub_8033900
	ldr r1, [r5, #0x5c]
	adds r0, r0, r1
	str r0, [r5, #0x1c]
	bl sub_80338F4
	ldr r1, [r5, #0x60]
	adds r0, r0, r1
	str r0, [r5, #0x20]
	bl sub_80338E8
	ldr r1, [r5, #0x64]
	adds r2, r0, r1
	str r2, [r5, #0x24]
	ldr r0, [r5, #0x28]
	cmp r0, #0
	bne _080341F2
	ldr r1, [r5, #0x34]
	movs r0, #0xa0
	lsls r0, r0, #6
	cmp r1, r0
	ble _080341F2
	ldr r6, [r5, #0x68]
	cmp r6, #0
	bne _080341EE
	ldr r0, [r5, #0x1c]
	ldr r1, [r5, #0x20]
	bl sub_802E5E4
	ldr r4, [r5, #0x6c]
	adds r4, #1
	str r4, [r5, #0x6c]
	bl sub_80338C4
	ldr r0, [r0, #8]
	cmp r4, r0
	bne _080341E6
	str r6, [r5, #0x6c]
	bl sub_80338C4
	ldr r0, [r0, #0xc]
	b _080341F0
_080341E6:
	bl sub_80338C4
	ldr r0, [r0, #4]
	b _080341F0
_080341EE:
	subs r0, r6, #1
_080341F0:
	str r0, [r5, #0x68]
_080341F2:
	pop {r4, r5, r6}
	pop {r0}
	bx r0

	thumb_func_start sub_80341F8
sub_80341F8: @ 0x080341F8
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	bl sub_8033900
	ldr r1, [r5, #0x5c]
	adds r0, r0, r1
	str r0, [r5, #0x1c]
	bl sub_80338F4
	ldr r1, [r5, #0x60]
	adds r0, r0, r1
	str r0, [r5, #0x20]
	bl sub_80338E8
	ldr r1, [r5, #0x64]
	adds r2, r0, r1
	str r2, [r5, #0x24]
	ldr r0, [r5, #0x28]
	cmp r0, #0
	bne _0803425E
	ldr r1, [r5, #0x34]
	movs r0, #0xa0
	lsls r0, r0, #6
	cmp r1, r0
	ble _0803425E
	ldr r6, [r5, #0x68]
	cmp r6, #0
	bne _0803425A
	ldr r0, [r5, #0x1c]
	ldr r1, [r5, #0x20]
	bl sub_802E5E4
	ldr r4, [r5, #0x6c]
	adds r4, #1
	str r4, [r5, #0x6c]
	bl sub_80338C4
	ldr r0, [r0, #8]
	cmp r4, r0
	bne _08034252
	str r6, [r5, #0x6c]
	bl sub_80338C4
	ldr r0, [r0, #0xc]
	b _0803425C
_08034252:
	bl sub_80338C4
	ldr r0, [r0, #4]
	b _0803425C
_0803425A:
	subs r0, r6, #1
_0803425C:
	str r0, [r5, #0x68]
_0803425E:
	pop {r4, r5, r6}
	pop {r0}
	bx r0

	thumb_func_start sub_8034264
sub_8034264: @ 0x08034264
	adds r0, #0x58
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start nullsub_38
nullsub_38: @ 0x0803426C
	bx lr
	.align 2, 0

	thumb_func_start sub_8034270
sub_8034270: @ 0x08034270
	push {r4, lr}
	adds r4, r0, #0
	bl sub_80338E8
	ldr r1, _080342BC @ =0xFFFFFE00
	adds r0, r0, r1
	str r0, [r4, #0x24]
	bl sub_8033900
	movs r2, #0x80
	lsls r2, r2, #6
	adds r0, r0, r2
	str r0, [r4, #0x1c]
	bl sub_80338F4
	movs r1, #0xc0
	lsls r1, r1, #6
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r1, r4, #0
	adds r1, #0x58
	movs r0, #1
	strb r0, [r1]
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _080342C0
	cmp r4, #0
	beq _080342B8
	ldr r1, [r4, #0x50]
	movs r2, #8
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_080342B8:
	movs r0, #0
	b _080342C2
	.align 2, 0
_080342BC: .4byte 0xFFFFFE00
_080342C0:
	movs r0, #1
_080342C2:
	cmp r0, #0
	beq _080342CC
	adds r0, r4, #0
	bl sub_802A7B8
_080342CC:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80342D4
sub_80342D4: @ 0x080342D4
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, [sp, #0x10]
	movs r5, #1
	str r0, [sp]
	adds r0, r4, #0
	bl InitActorPart
	str r5, [r4, #0x54]
	ldr r0, _08034310 @ =gStaticData_087E558C
	str r0, [r4, #0x50]
	movs r1, #0
	str r1, [r4, #0x28]
	str r1, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0]
	movs r2, #0
	strh r0, [r4, #0x10]
	strb r2, [r4, #0x12]
	str r1, [r4, #8]
	adds r0, r4, #0
	adds r0, #0x58
	strb r5, [r0]
	adds r0, r4, #0
	add sp, #4
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08034310: .4byte gStaticData_087E558C

	thumb_func_start sub_8034314
sub_8034314: @ 0x08034314
	push {r4, lr}
	adds r4, r0, #0
	bl sub_80338E8
	ldr r1, _08034360 @ =0xFFFFFE00
	adds r0, r0, r1
	str r0, [r4, #0x24]
	bl sub_8033900
	movs r2, #0x80
	lsls r2, r2, #6
	adds r0, r0, r2
	str r0, [r4, #0x1c]
	bl sub_80338F4
	movs r1, #0xc0
	lsls r1, r1, #6
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r1, r4, #0
	adds r1, #0x58
	movs r0, #1
	strb r0, [r1]
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _08034364
	cmp r4, #0
	beq _0803435C
	ldr r1, [r4, #0x50]
	movs r2, #8
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_0803435C:
	movs r0, #0
	b _08034366
	.align 2, 0
_08034360: .4byte 0xFFFFFE00
_08034364:
	movs r0, #1
_08034366:
	pop {r4}
	pop {r1}
	bx r1

	thumb_func_start sub_803436C
sub_803436C: @ 0x0803436C
	adds r0, #0x58
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8034374
sub_8034374: @ 0x08034374
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #8
	adds r6, r0, #0
	movs r0, #0x80
	lsls r0, r0, #4
	bl sub_8026EC0
	str r0, [r6, #8]
	ldr r1, _08034454 @ =gUnknown_03001288
	movs r4, #0
	movs r0, #0x40
	strh r0, [r1]
	movs r0, #0
	bl sub_8001524
	ldr r0, _08034458 @ =0x04000010
	str r4, [r0]
	ldr r0, _0803445C @ =0xFFFF0000
	ands r5, r0
	movs r0, #3
	orrs r5, r0
	movs r0, #0xf8
	lsls r0, r0, #5
	orrs r5, r0
	ldr r0, _08034460 @ =0x04000008
	strh r5, [r0]
	movs r0, #0xc0
	lsls r0, r0, #0x13
	str r0, [r6]
	ldr r0, _08034464 @ =0x0600F800
	str r0, [r6, #4]
	bl sub_80015D0
	movs r0, #0xa0
	lsls r0, r0, #0x13
	strh r4, [r0]
	ldr r4, _08034468 @ =0x050001E0
	add r0, sp, #4
	mov r8, r0
	movs r2, #0
	movs r3, #2
_080343CA:
	lsrs r0, r2, #0x1f
	adds r0, r2, r0
	asrs r0, r0, #1
	lsls r1, r0, #5
	orrs r1, r0
	lsls r0, r0, #0xa
	orrs r1, r0
	strh r1, [r4]
	adds r4, #2
	adds r2, #0x1f
	subs r3, #1
	cmp r3, #0
	bge _080343CA
	movs r3, #0
	movs r0, #0
	ldr r1, [r6]
	mov ip, r1
	ldr r7, [r6, #4]
	ldr r1, _0803446C @ =0xFFFFF000
	adds r5, r1, #0
_080343F2:
	adds r4, r0, #1
	lsls r0, r0, #6
	adds r1, r0, r7
	movs r2, #0x1d
_080343FA:
	adds r0, r3, #0
	orrs r0, r5
	strh r0, [r1]
	adds r3, #1
	adds r1, #2
	subs r2, #1
	cmp r2, #0
	bge _080343FA
	adds r0, r4, #0
	cmp r0, #0x13
	ble _080343F2
	movs r5, #0
	mov r0, sp
	strh r5, [r0]
	ldr r4, _08034470 @ =0x040000D4
	str r0, [r4]
	mov r0, ip
	str r0, [r4, #4]
	ldr r0, _08034474 @ =0x81002580
	str r0, [r4, #8]
	ldr r0, [r4, #8]
	ldr r0, _08034478 @ =0x04000050
	strh r5, [r0]
	bl sub_8001614
	str r5, [r6, #0xc]
	movs r0, #0x96
	lsls r0, r0, #7
	bl sub_8026EC0
	str r0, [r6, #0x10]
	str r5, [sp, #4]
	mov r1, r8
	str r1, [r4]
	str r0, [r4, #4]
	ldr r0, _0803447C @ =0x850012C0
	str r0, [r4, #8]
	ldr r0, [r4, #8]
	adds r0, r6, #0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08034454: .4byte gUnknown_03001288
_08034458: .4byte 0x04000010
_0803445C: .4byte 0xFFFF0000
_08034460: .4byte 0x04000008
_08034464: .4byte 0x0600F800
_08034468: .4byte 0x050001E0
_0803446C: .4byte 0xFFFFF000
_08034470: .4byte 0x040000D4
_08034474: .4byte 0x81002580
_08034478: .4byte 0x04000050
_0803447C: .4byte 0x850012C0

	thumb_func_start sub_8034480
sub_8034480: @ 0x08034480
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	adds r6, r0, #0
	ldr r1, _0803459C @ =0x040000D4
	ldr r2, [r6, #0x10]
	str r2, [r1]
	ldr r0, [r6]
	str r0, [r1, #4]
	ldr r0, _080345A0 @ =0x840012C0
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	movs r0, #0
	str r0, [sp]
	mov r0, sp
	str r0, [r1]
	str r2, [r1, #4]
	ldr r0, _080345A4 @ =0x850012C0
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r5, [r6, #8]
	movs r7, #0
	ldr r0, [r6, #0xc]
	cmp r7, r0
	bge _0803458C
	movs r0, #1
	mov sb, r0
	movs r0, #7
	mov r8, r0
_080344BE:
	ldr r1, [r5]
	asrs r4, r1, #8
	ldr r2, [r5, #4]
	asrs r3, r2, #8
	cmp r4, #0xef
	bhi _0803450A
	cmp r3, #0
	blt _0803450A
	cmp r3, #0x9f
	bgt _0803450A
	asrs r1, r1, #0xb
	lsls r1, r1, #6
	asrs r2, r2, #0xb
	lsls r0, r2, #4
	subs r0, r0, r2
	lsls r0, r0, #7
	adds r1, r1, r0
	mov r0, r8
	ands r4, r0
	adds r1, r1, r4
	ands r3, r0
	lsls r0, r3, #3
	adds r1, r1, r0
	asrs r0, r1, #2
	lsls r0, r0, #1
	ldr r3, [r6, #0x10]
	adds r3, r3, r0
	movs r0, #3
	ands r1, r0
	lsls r1, r1, #2
	movs r0, #0xf
	lsls r0, r1
	ldrh r2, [r3]
	bics r2, r0
	mov r0, sb
	lsls r0, r1
	orrs r2, r0
	strh r2, [r3]
_0803450A:
	ldr r2, [r5]
	ldr r0, [r5, #8]
	adds r2, r2, r0
	str r2, [r5]
	ldr r1, [r5, #4]
	ldr r0, [r5, #0xc]
	adds r1, r1, r0
	str r1, [r5, #4]
	ldr r0, _080345A8 @ =0x0000EFFF
	cmp r2, r0
	bhi _0803452A
	cmp r1, #0
	blt _0803452A
	ldr r0, _080345AC @ =0x00009FFF
	cmp r1, r0
	ble _08034532
_0803452A:
	adds r0, r6, #0
	adds r1, r7, #0
	bl sub_80345B0
_08034532:
	ldr r1, [r5]
	asrs r4, r1, #8
	ldr r2, [r5, #4]
	asrs r3, r2, #8
	movs r0, #2
	mov ip, r0
	cmp r4, #0xef
	bhi _08034582
	cmp r3, #0
	blt _08034582
	cmp r3, #0x9f
	bgt _08034582
	asrs r1, r1, #0xb
	lsls r1, r1, #6
	asrs r2, r2, #0xb
	lsls r0, r2, #4
	subs r0, r0, r2
	lsls r0, r0, #7
	adds r1, r1, r0
	mov r0, r8
	ands r4, r0
	adds r1, r1, r4
	ands r3, r0
	lsls r0, r3, #3
	adds r1, r1, r0
	asrs r0, r1, #2
	lsls r0, r0, #1
	ldr r3, [r6, #0x10]
	adds r3, r3, r0
	movs r0, #3
	ands r1, r0
	lsls r1, r1, #2
	movs r0, #0xf
	lsls r0, r1
	ldrh r2, [r3]
	bics r2, r0
	mov r0, ip
	lsls r0, r1
	orrs r2, r0
	strh r2, [r3]
_08034582:
	adds r5, #0x10
	adds r7, #1
	ldr r0, [r6, #0xc]
	cmp r7, r0
	blt _080344BE
_0803458C:
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0803459C: .4byte 0x040000D4
_080345A0: .4byte 0x840012C0
_080345A4: .4byte 0x850012C0
_080345A8: .4byte 0x0000EFFF
_080345AC: .4byte 0x00009FFF

	thumb_func_start sub_80345B0
sub_80345B0: @ 0x080345B0
	push {r4, r5, r6, lr}
	adds r2, r0, #0
	cmp r1, #0x7f
	ble _080345BA
_080345B8:
	b _080345B8
_080345BA:
	lsls r0, r1, #4
	ldr r5, [r2, #8]
	adds r5, r5, r0
	movs r0, #0xf0
	lsls r0, r0, #7
	str r0, [r5]
	movs r0, #0xa0
	lsls r0, r0, #7
	str r0, [r5, #4]
	movs r6, #0x80
	lsls r6, r6, #1
	adds r0, r6, #0
	bl sub_8000E1C
	adds r4, r0, #0
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	movs r0, #0x80
	lsls r0, r0, #2
	bl sub_8000E1C
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r0, r0, r6
	ldr r6, _08034630 @ =gStaticData_0816A820
	adds r1, r4, #0
	adds r1, #0x40
	movs r2, #0xff
	ands r1, r2
	lsls r1, r1, #1
	adds r1, r1, r6
	movs r3, #0
	ldrsh r1, [r1, r3]
	adds r3, r1, #0
	muls r3, r0, r3
	asrs r3, r3, #8
	str r3, [r5, #8]
	ands r4, r2
	lsls r4, r4, #1
	adds r4, r4, r6
	movs r2, #0
	ldrsh r1, [r4, r2]
	adds r2, r1, #0
	muls r2, r0, r2
	asrs r2, r2, #8
	str r2, [r5, #0xc]
	lsls r1, r3, #2
	adds r1, r1, r3
	ldr r0, [r5]
	adds r0, r0, r1
	str r0, [r5]
	lsls r1, r2, #2
	adds r1, r1, r2
	ldr r0, [r5, #4]
	adds r0, r0, r1
	str r0, [r5, #4]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08034630: .4byte gStaticData_0816A820

	thumb_func_start sub_8034634
sub_8034634: @ 0x08034634
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	adds r5, r1, #0
	adds r4, r2, #0
	cmp r5, #0xef
	bhi _08034680
	cmp r4, #0
	blt _08034680
	cmp r4, #0x9f
	bgt _08034680
	asrs r1, r5, #3
	lsls r1, r1, #6
	asrs r2, r4, #3
	lsls r0, r2, #4
	subs r0, r0, r2
	lsls r0, r0, #7
	adds r1, r1, r0
	movs r0, #7
	ands r5, r0
	adds r1, r1, r5
	ands r4, r0
	lsls r0, r4, #3
	adds r1, r1, r0
	asrs r0, r1, #2
	lsls r0, r0, #1
	ldr r2, [r6, #0x10]
	adds r2, r2, r0
	movs r0, #3
	ands r1, r0
	lsls r1, r1, #2
	movs r0, #0xf
	lsls r0, r1
	ldrh r4, [r2]
	bics r4, r0
	adds r0, r4, #0
	lsls r3, r1
	orrs r0, r3
	strh r0, [r2]
_08034680:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8034688
sub_8034688: @ 0x08034688
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	bl sub_8034480
	ldr r1, [r5, #0xc]
	cmp r1, #0x7f
	bgt _080346C0
	movs r0, #0x80
	subs r4, r0, r1
	cmp r4, #8
	ble _080346A0
	movs r4, #8
_080346A0:
	subs r4, #1
	movs r0, #1
	rsbs r0, r0, #0
	cmp r4, r0
	beq _080346C0
	adds r6, r0, #0
_080346AC:
	ldr r0, [r5, #0xc]
	adds r1, r0, #0
	adds r0, #1
	str r0, [r5, #0xc]
	adds r0, r5, #0
	bl sub_80345B0
	subs r4, #1
	cmp r4, r6
	bne _080346AC
_080346C0:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80346C8
sub_80346C8: @ 0x080346C8
	push {r4, lr}
	adds r4, r0, #0
	b _080346D8
_080346CE:
	bl sub_80006A8
	adds r0, r4, #0
	bl sub_8034688
_080346D8:
	ldr r0, _080346F4 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r1, _080346F8 @ =gUnknown_030007E0
	movs r0, #9
	ldrh r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _080346CE
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080346F4: .4byte gUnknown_03001304
_080346F8: .4byte gUnknown_030007E0

	thumb_func_start sub_80346FC
sub_80346FC: @ 0x080346FC
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, [r4, #0x10]
	cmp r0, #0
	beq _0803470C
	bl sub_8026EB4
_0803470C:
	ldr r0, [r4, #8]
	cmp r0, #0
	beq _08034716
	bl sub_8026EB4
_08034716:
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _08034724
	adds r0, r4, #0
	bl sub_8026ED0
_08034724:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_803472C
sub_803472C: @ 0x0803472C
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	sub sp, #4
	adds r5, r0, #0
	movs r0, #0x10
	bl sub_8026EDC
	movs r1, #3
	str r1, [sp]
	movs r1, #0
	movs r2, #0x1f
	movs r3, #0
	bl sub_801E644
	str r0, [r5, #4]
	movs r0, #0x10
	bl sub_8026EDC
	movs r1, #1
	str r1, [sp]
	movs r1, #3
	movs r2, #0x1e
	movs r3, #0
	bl sub_801E644
	str r0, [r5]
	movs r0, #0x10
	bl sub_8026EDC
	movs r1, #2
	str r1, [sp]
	movs r2, #0x1d
	movs r3, #1
	bl sub_801E644
	str r0, [r5, #8]
	ldr r0, [r5]
	ldr r1, _08034854 @ =gStaticData_0817C5BC
	bl LoadGraphicsPackage
	ldr r0, [r5, #4]
	ldr r1, _08034858 @ =gStaticData_0817C594
	bl LoadGraphicsPackage
	ldr r0, [r5, #8]
	ldr r1, _0803485C @ =gStaticData_0817C5A8
	bl LoadGraphicsPackage
	movs r1, #0xa0
	lsls r1, r1, #0x13
	movs r0, #0
	strh r0, [r1]
	movs r0, #0
	mov r8, r0
	mov r1, r8
	strh r1, [r5, #0xc]
	movs r2, #0x40
	mov sb, r2
	mov r0, sb
	ldrb r1, [r5, #0xc]
	orrs r0, r1
	movs r1, #8
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r5, #0xc]
	movs r6, #1
	ldrb r0, [r5, #0xd]
	orrs r0, r6
	movs r1, #2
	orrs r0, r1
	movs r4, #4
	orrs r0, r4
	strb r0, [r5, #0xd]
	adds r0, r5, #0
	bl sub_803487C
	mov r2, r8
	str r2, [r5, #0x10]
	ldrb r0, [r5, #0x10]
	orrs r4, r0
	ldrb r1, [r5, #0x11]
	orrs r6, r1
	strb r6, [r5, #0x11]
	movs r1, #0x20
	rsbs r1, r1, #0
	adds r0, r1, #0
	ldrb r2, [r5, #0x12]
	ands r0, r2
	movs r2, #8
	orrs r0, r2
	strb r0, [r5, #0x12]
	ldrb r0, [r5, #0x13]
	ands r1, r0
	movs r0, #0x10
	orrs r1, r0
	strb r1, [r5, #0x13]
	movs r0, #0x3f
	ands r4, r0
	mov r1, sb
	orrs r4, r1
	strb r4, [r5, #0x10]
	ldr r0, [r5, #4]
	bl sub_801E640
	ldr r1, _08034860 @ =0x04000008
	strh r0, [r1]
	ldr r0, _08034864 @ =0x04000010
	mov r2, r8
	str r2, [r0]
	ldr r0, [r5]
	bl sub_801E640
	ldr r1, _08034868 @ =0x0400000A
	strh r0, [r1]
	ldr r0, _0803486C @ =0x04000014
	mov r1, r8
	str r1, [r0]
	ldr r0, [r5, #8]
	bl sub_801E640
	ldr r1, _08034870 @ =0x0400000C
	strh r0, [r1]
	ldr r0, _08034874 @ =0x04000018
	mov r2, r8
	str r2, [r0]
	subs r1, #0xc
	ldrh r0, [r5, #0xc]
	strh r0, [r1]
	adds r1, #0x50
	ldr r0, [r5, #0x10]
	str r0, [r1]
	str r2, [r5, #0x1c]
	str r2, [r5, #0x20]
	ldr r0, _08034878 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8001AC4
	adds r0, r5, #0
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08034854: .4byte gStaticData_0817C5BC
_08034858: .4byte gStaticData_0817C594
_0803485C: .4byte gStaticData_0817C5A8
_08034860: .4byte 0x04000008
_08034864: .4byte 0x04000010
_08034868: .4byte 0x0400000A
_0803486C: .4byte 0x04000014
_08034870: .4byte 0x0400000C
_08034874: .4byte 0x04000018
_08034878: .4byte gUnknown_030012BC

	thumb_func_start sub_803487C
sub_803487C: @ 0x0803487C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r0
	ldr r5, _08034974 @ =gUnknown_030012FC
	ldr r0, [r5]
	movs r4, #0
	str r4, [r0, #8]
	bl sub_8006C4C
	ldr r0, [r5]
	bl sub_8006C4C
	ldr r0, _08034978 @ =gUnknown_030012DC
	ldr r0, [r0]
	mov r1, r8
	str r0, [r1, #0x18]
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
	mov r0, r8
	ldr r1, [r0, #0x18]
	movs r2, #0x8c
	lsls r2, r2, #1
	adds r0, r1, r2
	str r4, [r0]
	ldr r0, [r5]
	movs r3, #0x96
	lsls r3, r3, #1
	adds r1, r1, r3
	ldr r1, [r1]
	lsls r1, r1, #5
	bl sub_8006C58
	ldr r0, [r5]
	bl sub_8006C30
	ldr r4, _0803497C @ =gUnknown_030012B8
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
	movs r7, #0
	adds r2, r0, #0
	adds r2, #0x6c
	ldr r6, _08034980 @ =gStaticData_0817C532
	adds r1, r0, #0
	adds r1, #0x2c
	ldr r5, _08034984 @ =gStaticData_0817C512
	ldr r4, _08034988 @ =gStaticData_0817C572
	ldr r3, _0803498C @ =gStaticData_0817C552
_0803491A:
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
	adds r7, #1
	cmp r7, #0xf
	ble _0803491A
	ldr r0, _0803497C @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	movs r0, #0x10
	mov r1, r8
	ldrb r1, [r1, #0xd]
	orrs r0, r1
	mov r2, r8
	strb r0, [r2, #0xd]
	ldr r4, _08034990 @ =gUnknown_03001300
	ldr r0, [r4]
	bl sub_8006A90
	ldr r0, [r4]
	bl sub_8006A48
	bl sub_80006A8
	ldr r0, [r4]
	bl sub_8006AAC
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08034974: .4byte gUnknown_030012FC
_08034978: .4byte gUnknown_030012DC
_0803497C: .4byte gUnknown_030012B8
_08034980: .4byte gStaticData_0817C532
_08034984: .4byte gStaticData_0817C512
_08034988: .4byte gStaticData_0817C572
_0803498C: .4byte gStaticData_0817C552
_08034990: .4byte gUnknown_03001300

	thumb_func_start sub_8034994
sub_8034994: @ 0x08034994
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	adds r4, r0, #0
	movs r0, #1
	mov sb, r0
	movs r1, #0
	mov r8, r1
	ldrb r2, [r4, #0x12]
	lsls r0, r2, #0x1b
	lsrs r6, r0, #0x1b
	ldr r0, _080349E8 @ =gUnknown_030007E0
	mov sl, r0
	ldr r7, _080349EC @ =gUnknown_030012BC
_080349B4:
	ldr r0, _080349F0 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	mov r1, sl
	ldr r2, [r1]
	lsrs r1, r2, #0x10
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	bne _080349D8
	lsrs r1, r2, #0x10
	movs r0, #8
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r5, r0, #0x10
	cmp r5, #0
	beq _080349F4
_080349D8:
	ldr r0, [r7]
	movs r1, #0x49
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	b _08034A88
	.align 2, 0
_080349E8: .4byte gUnknown_030007E0
_080349EC: .4byte gUnknown_030012BC
_080349F0: .4byte gUnknown_03001304
_080349F4:
	lsrs r1, r2, #0x10
	movs r0, #0x40
	ands r0, r1
	cmp r0, #0
	beq _08034A12
	ldr r0, [r4, #0x20]
	cmp r0, #1
	bne _08034A12
	ldr r0, [r7]
	movs r1, #0x46
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	str r5, [r4, #0x20]
_08034A12:
	movs r0, #0x80
	mov r2, sl
	ldrh r2, [r2, #2]
	ands r0, r2
	cmp r0, #0
	beq _08034A34
	ldr r0, [r4, #0x20]
	cmp r0, #0
	bne _08034A34
	ldr r0, [r7]
	movs r1, #0x46
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	movs r0, #1
	str r0, [r4, #0x20]
_08034A34:
	adds r0, r4, #0
	bl sub_8034AA4
	adds r0, r4, #0
	bl sub_8034C5C
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #1
	ble _080349B4
	movs r2, #0
	mov r8, r2
	mov r0, sb
	cmp r0, #0
	beq _08034A5E
	subs r6, #1
	cmp r6, #0
	bgt _08034A68
	mov sb, r2
	b _08034A68
_08034A5E:
	adds r6, #1
	cmp r6, #0xf
	ble _08034A68
	movs r1, #1
	mov sb, r1
_08034A68:
	movs r0, #0x1f
	adds r1, r6, #0
	ands r1, r0
	movs r2, #0x20
	rsbs r2, r2, #0
	adds r0, r2, #0
	ldrb r2, [r4, #0x12]
	ands r0, r2
	orrs r0, r1
	strb r0, [r4, #0x12]
	ldr r1, _08034A84 @ =0x04000050
	ldr r0, [r4, #0x10]
	str r0, [r1]
	b _080349B4
	.align 2, 0
_08034A84: .4byte 0x04000050
_08034A88:
	movs r1, #0
	ldr r0, [r4, #0x20]
	cmp r0, #0
	bne _08034A92
	movs r1, #1
_08034A92:
	adds r0, r1, #0
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8034AA4
sub_8034AA4: @ 0x08034AA4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	adds r6, r0, #0
	ldr r0, _08034C34 @ =gUnknown_03001300
	mov sl, r0
	ldr r0, [r0]
	bl sub_8006A90
	ldr r0, _08034C38 @ =gUnknown_030012FC
	ldr r0, [r0]
	bl sub_8006C28
	ldr r4, [r6, #0x18]
	movs r1, #0x98
	lsls r1, r1, #1
	mov r8, r1
	adds r0, r4, r1
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x10
	movs r2, #0x10
	ldrsh r0, [r0, r2]
	adds r4, r4, r0
	movs r0, #0x28
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	adds r4, r0, #0
	ldr r0, [r6, #0x18]
	movs r1, #0
	bl sub_8028A30
	movs r1, #0x88
	subs r1, r1, r4
	ldr r4, [r6, #0x18]
	movs r7, #0x87
	movs r3, #0x88
	lsls r3, r3, #1
	adds r0, r4, r3
	str r1, [r0]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r4, r1
	str r7, [r0]
	mov r2, r8
	adds r0, r4, r2
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r3, #0x20
	ldrsh r0, [r0, r3]
	adds r4, r4, r0
	movs r0, #0x28
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	ldr r4, [r6, #0x18]
	adds r0, r6, #0
	movs r1, #0
	bl sub_8034C40
	adds r1, r0, #0
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	adds r0, r4, #0
	bl sub_8028A30
	ldr r0, [r6, #0x20]
	cmp r0, #0
	bne _08034B6E
	ldr r3, [r6, #0x18]
	movs r1, #0x90
	movs r4, #0x88
	lsls r4, r4, #1
	adds r0, r3, r4
	str r1, [r0]
	adds r1, #0x84
	adds r0, r3, r1
	str r7, [r0]
	movs r2, #0x98
	lsls r2, r2, #1
	adds r0, r3, r2
	ldr r2, [r0]
	movs r4, #0x20
	ldrsh r0, [r2, r4]
	adds r0, r3, r0
	ldr r1, _08034C3C @ =gStaticData_0817C510
	ldr r2, [r2, #0x24]
	bl sub_803AD80
_08034B6E:
	ldr r4, [r6, #0x18]
	movs r0, #0x98
	mov sb, r0
	movs r1, #0x88
	lsls r1, r1, #1
	adds r0, r4, r1
	mov r2, sb
	str r2, [r0]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r0, r4, r3
	str r7, [r0]
	mov r1, r8
	adds r0, r4, r1
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r2, #0x20
	ldrsh r0, [r0, r2]
	adds r4, r4, r0
	movs r0, #0x29
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	ldr r4, [r6, #0x18]
	adds r0, r6, #0
	movs r1, #1
	bl sub_8034C40
	adds r1, r0, #0
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	adds r0, r4, #0
	bl sub_8028A30
	ldr r0, [r6, #0x20]
	cmp r0, #1
	bne _08034BEA
	ldr r0, [r6, #0x18]
	movs r2, #0x90
	movs r3, #0x91
	movs r4, #0x88
	lsls r4, r4, #1
	adds r1, r0, r4
	str r2, [r1]
	adds r2, #0x84
	adds r1, r0, r2
	str r3, [r1]
	mov r3, r8
	adds r1, r0, r3
	ldr r2, [r1]
	movs r4, #0x20
	ldrsh r1, [r2, r4]
	adds r0, r0, r1
	ldr r1, _08034C3C @ =gStaticData_0817C510
	ldr r2, [r2, #0x24]
	bl sub_803AD80
_08034BEA:
	ldr r4, [r6, #0x18]
	movs r1, #0x91
	movs r2, #0x88
	lsls r2, r2, #1
	adds r0, r4, r2
	mov r3, sb
	str r3, [r0]
	adds r2, #4
	adds r0, r4, r2
	str r1, [r0]
	mov r3, r8
	adds r0, r4, r3
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r1, #0x20
	ldrsh r0, [r0, r1]
	adds r4, r4, r0
	movs r0, #0x2a
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	mov r2, sl
	ldr r0, [r2]
	bl sub_8006A48
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08034C34: .4byte gUnknown_03001300
_08034C38: .4byte gUnknown_030012FC
_08034C3C: .4byte gStaticData_0817C510

	thumb_func_start sub_8034C40
sub_8034C40: @ 0x08034C40
	adds r3, r0, #0
	ldr r0, [r3, #0x20]
	cmp r1, r0
	beq _08034C4C
	movs r0, #1
	b _08034C58
_08034C4C:
	ldr r1, [r3, #0x1c]
	asrs r0, r1, #1
	movs r2, #2
	ands r0, r2
	adds r1, #1
	str r1, [r3, #0x1c]
_08034C58:
	bx lr
	.align 2, 0

	thumb_func_start sub_8034C5C
sub_8034C5C: @ 0x08034C5C
	push {r4, lr}
	adds r4, r0, #0
	bl sub_80006A8
	ldr r0, _08034C80 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	movs r1, #0x80
	lsls r1, r1, #0x13
	ldrh r0, [r4, #0xc]
	strh r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08034C80: .4byte gUnknown_03001300

	thumb_func_start sub_8034C84
sub_8034C84: @ 0x08034C84
	push {r4, r5, lr}
	adds r5, r0, #0
	adds r4, r1, #0
	ldr r0, [r5, #4]
	bl sub_8026ED0
	ldr r0, [r5]
	bl sub_8026ED0
	ldr r0, [r5, #8]
	bl sub_8026ED0
	movs r0, #1
	ands r0, r4
	cmp r0, #0
	beq _08034CAA
	adds r0, r5, #0
	bl sub_8026ED0
_08034CAA:
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_8034CB0
sub_8034CB0: @ 0x08034CB0
	push {r4, r5, r6, lr}
	movs r6, #0xc0
	lsls r6, r6, #0x18
	adds r0, r6, #0
	bl mem_free_bytes
	movs r0, #0x24
	bl sub_8026EDC
	bl sub_803472C
	adds r4, r0, #0
	bl sub_8034994
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	cmp r4, #0
	beq _08034CDC
	adds r0, r4, #0
	movs r1, #3
	bl sub_8034C84
_08034CDC:
	adds r0, r6, #0
	bl mem_free_bytes
	adds r0, r5, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8034CEC
sub_8034CEC: @ 0x08034CEC
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	adds r5, r0, #0
	movs r0, #0x14
	bl sub_8026EDC
	bl sub_8034374
	str r0, [r5, #0xc]
	ldr r4, _08034E0C @ =gUnknown_03001300
	ldr r0, [r4]
	bl sub_8006A90
	ldr r0, [r4]
	bl sub_8006A48
	bl sub_80006A8
	ldr r0, [r4]
	bl sub_8006AAC
	ldr r4, _08034E10 @ =gUnknown_030012B8
	ldr r0, [r4]
	bl sub_8006EA8
	ldr r6, _08034E14 @ =gUnknown_030012DC
	ldr r0, [r6]
	bl sub_8028A40
	ldr r0, _08034E18 @ =gUnknown_030012E0
	mov sb, r0
	ldr r0, [r0]
	movs r1, #0
	bl sub_8028A30
	adds r0, r5, #0
	bl sub_80352AC
	ldr r0, [r4]
	bl sub_8006DC8
	ldr r4, _08034E1C @ =gUnknown_030012FC
	ldr r0, [r4]
	movs r1, #0
	mov r8, r1
	str r1, [r0, #8]
	bl sub_8006C4C
	ldr r0, [r4]
	bl sub_8006C4C
	ldr r0, [r6]
	movs r2, #0x84
	lsls r2, r2, #1
	adds r1, r0, r2
	mov r3, r8
	str r3, [r1]
	adds r2, #0x28
	adds r1, r0, r2
	ldr r1, [r1]
	adds r1, #0x40
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r1, [r1, #4]
	bl sub_803AD7C
	ldr r0, [r4]
	ldr r1, [r6]
	movs r2, #0x96
	lsls r2, r2, #1
	adds r1, r1, r2
	ldr r1, [r1]
	lsls r1, r1, #5
	bl sub_8006C58
	ldr r0, [r6]
	movs r3, #0x96
	lsls r3, r3, #1
	adds r0, r0, r3
	ldr r2, [r0]
	mov r1, sb
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
	ldr r0, [r4]
	mov r2, sb
	ldr r1, [r2]
	movs r3, #0x96
	lsls r3, r3, #1
	adds r1, r1, r3
	ldr r1, [r1]
	lsls r1, r1, #5
	bl sub_8006C58
	ldr r0, [r4]
	bl sub_8006C30
	mov r0, r8
	str r0, [r5]
	str r0, [r5, #0x10]
	ldr r0, _08034E20 @ =gStaticData_0817C5D0
	str r0, [r5, #4]
	str r0, [r5, #8]
	mov r1, r8
	str r1, [r5, #0x14]
	ldr r1, _08034E24 @ =gUnknown_03001288
	movs r0, #0x10
	ldrb r2, [r1, #1]
	orrs r0, r2
	strb r0, [r1, #1]
	bl sub_8001614
	adds r0, r5, #0
	adds r0, #0x94
	mov r3, r8
	str r3, [r0]
	ldr r0, _08034E28 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x11
	bl sub_8001B54
	adds r0, r5, #0
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08034E0C: .4byte gUnknown_03001300
_08034E10: .4byte gUnknown_030012B8
_08034E14: .4byte gUnknown_030012DC
_08034E18: .4byte gUnknown_030012E0
_08034E1C: .4byte gUnknown_030012FC
_08034E20: .4byte gStaticData_0817C5D0
_08034E24: .4byte gUnknown_03001288
_08034E28: .4byte gUnknown_030012BC

	thumb_func_start sub_8034E2C
sub_8034E2C: @ 0x08034E2C
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	adds r4, r5, #0
	adds r4, #0x94
	b _08034E60
_08034E36:
	ldr r0, [r4]
	adds r0, #1
	movs r1, #1
	ands r0, r1
	str r0, [r4]
	cmp r0, #0
	beq _08034E4A
	adds r0, r5, #0
	bl sub_80350A4
_08034E4A:
	adds r0, r5, #0
	bl sub_8034EF0
	bl sub_80006A8
	adds r0, r5, #0
	bl sub_803544C
	ldr r0, [r5, #0xc]
	bl sub_8034688
_08034E60:
	ldr r0, _08034EDC @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r1, _08034EE0 @ =gUnknown_030007E0
	movs r0, #9
	ldrh r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _08034E36
	ldr r0, _08034EE4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8001AC4
	movs r4, #0
	adds r6, r5, #0
	adds r6, #0x94
	ldr r7, _08034EE8 @ =0x04000050
_08034E86:
	ldr r0, [r6]
	adds r0, #1
	movs r1, #1
	ands r0, r1
	str r0, [r6]
	cmp r0, #0
	beq _08034E9A
	adds r0, r5, #0
	bl sub_80350A4
_08034E9A:
	adds r0, r5, #0
	bl sub_8034EF0
	bl sub_80006A8
	ldr r0, _08034EEC @ =0x04000054
	strh r4, [r0]
	movs r0, #0xff
	strh r0, [r7]
	adds r0, r5, #0
	bl sub_803544C
	ldr r0, [r5, #0xc]
	bl sub_8034688
	adds r4, #1
	cmp r4, #0x10
	ble _08034E86
	ldr r0, [r5]
	cmp r0, #0
	beq _08034ED0
_08034EC4:
	ldr r4, [r0]
	bl sub_8026ED0
	adds r0, r4, #0
	cmp r0, #0
	bne _08034EC4
_08034ED0:
	movs r0, #0
	str r0, [r5]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08034EDC: .4byte gUnknown_03001304
_08034EE0: .4byte gUnknown_030007E0
_08034EE4: .4byte gUnknown_030012BC
_08034EE8: .4byte 0x04000050
_08034EEC: .4byte 0x04000054

	thumb_func_start sub_8034EF0
sub_8034EF0: @ 0x08034EF0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x24
	str r0, [sp, #0xc]
	ldr r0, _08034F34 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006A90
	ldr r0, _08034F38 @ =gUnknown_030012FC
	ldr r0, [r0]
	bl sub_8006C28
	ldr r0, [sp, #0xc]
	ldr r0, [r0]
	mov sb, r0
	cmp r0, #0
	bne _08034F1A
	b _0803506E
_08034F1A:
	mov r1, sp
	adds r1, #4
	str r1, [sp, #0x10]
_08034F20:
	mov r2, sb
	ldr r0, [r2, #0x10]
	cmp r0, #1
	beq _08034F4C
	cmp r0, #1
	bgt _08034F3C
	cmp r0, #0
	beq _08034F42
	b _08035062
	.align 2, 0
_08034F34: .4byte gUnknown_03001300
_08034F38: .4byte gUnknown_030012FC
_08034F3C:
	cmp r0, #2
	beq _08034F84
	b _08035062
_08034F42:
	ldr r0, _08034F48 @ =gUnknown_030012DC
	b _08034F4E
	.align 2, 0
_08034F48: .4byte gUnknown_030012DC
_08034F4C:
	ldr r0, _08034F80 @ =gUnknown_030012E0
_08034F4E:
	ldr r3, [r0]
	mov r4, sb
	ldr r1, [r4, #4]
	ldr r2, [r4, #8]
	movs r4, #0x88
	lsls r4, r4, #1
	adds r0, r3, r4
	str r1, [r0]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r3, r1
	str r2, [r0]
	movs r2, #0x98
	lsls r2, r2, #1
	adds r0, r3, r2
	ldr r2, [r0]
	movs r4, #0x30
	ldrsh r0, [r2, r4]
	adds r0, r3, r0
	mov r3, sb
	ldrb r1, [r3, #0x14]
	ldr r2, [r2, #0x34]
	bl sub_803AD80
	b _08035062
	.align 2, 0
_08034F80: .4byte gUnknown_030012E0
_08034F84:
	mov r4, sb
	ldrb r4, [r4, #0x14]
	lsls r0, r4, #1
	mov r1, sb
	ldrb r1, [r1, #0x14]
	adds r0, r0, r1
	lsls r0, r0, #3
	adds r0, #0x1c
	ldr r2, [sp, #0xc]
	adds r6, r2, r0
	ldr r4, _08035088 @ =gUnknown_030012FC
	ldr r0, [r4]
	bl sub_8006C44
	mov sl, r0
	ldr r0, [r4]
	ldr r1, [r6, #0x14]
	ldr r3, [r6, #4]
	ldr r2, [r6]
	muls r2, r3, r2
	lsls r2, r2, #9
	bl sub_8006C84
	movs r0, #0
	str r0, [sp]
	mov r0, sp
	ldr r1, [sp, #0x10]
	ldr r2, _0803508C @ =0x05000002
	bl sub_803A94C
	ldr r3, [sp, #0x10]
	ldrb r1, [r3, #3]
	movs r0, #0x3f
	ands r0, r1
	movs r1, #0x80
	orrs r0, r1
	strb r0, [r3, #3]
	ldrb r4, [r6, #0x10]
	lsls r1, r4, #4
	movs r0, #0xf
	ldrb r2, [r3, #5]
	ands r0, r2
	orrs r0, r1
	strb r0, [r3, #5]
	mov r3, sb
	ldr r3, [r3, #8]
	mov r8, r3
	movs r1, #0
	ldr r0, [r6, #4]
	mov r4, sp
	adds r4, #4
	str r4, [sp, #0x20]
	cmp r1, r0
	bge _08035062
_08034FF0:
	mov r2, r8
	ldr r0, [sp, #0x20]
	strb r2, [r0]
	mov r3, sb
	ldr r5, [r3, #4]
	movs r7, #0
	ldr r0, [r6]
	mov r4, r8
	adds r4, #0x20
	str r4, [sp, #0x18]
	adds r1, #1
	str r1, [sp, #0x14]
	cmp r7, r0
	bge _08035056
	ldr r4, [sp, #0x20]
_0803500E:
	mov r0, r8
	adds r0, #0x1f
	cmp r0, #0xbe
	bhi _08035048
	ldr r1, _08035090 @ =0x000001FF
	adds r0, r1, #0
	adds r2, r5, #0
	ands r2, r0
	ldrh r0, [r4, #2]
	ldr r3, _08035094 @ =0xFFFFFE00
	adds r1, r3, #0
	ands r0, r1
	orrs r0, r2
	strh r0, [r4, #2]
	ldr r1, _08035098 @ =0x000003FF
	adds r0, r1, #0
	mov r1, sl
	ands r1, r0
	ldr r2, _0803509C @ =0xFFFFFC00
	adds r0, r2, #0
	ldrh r3, [r4, #4]
	ands r0, r3
	orrs r0, r1
	strh r0, [r4, #4]
	ldr r0, _080350A0 @ =gUnknown_03001300
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8006AC8
_08035048:
	movs r0, #0x10
	add sl, r0
	adds r5, #0x20
	adds r7, #1
	ldr r0, [r6]
	cmp r7, r0
	blt _0803500E
_08035056:
	ldr r1, [sp, #0x18]
	mov r8, r1
	ldr r1, [sp, #0x14]
	ldr r0, [r6, #4]
	cmp r1, r0
	blt _08034FF0
_08035062:
	mov r2, sb
	ldr r2, [r2]
	mov sb, r2
	cmp r2, #0
	beq _0803506E
	b _08034F20
_0803506E:
	ldr r0, _080350A0 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006A48
	add sp, #0x24
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08035088: .4byte gUnknown_030012FC
_0803508C: .4byte 0x05000002
_08035090: .4byte 0x000001FF
_08035094: .4byte 0xFFFFFE00
_08035098: .4byte 0x000003FF
_0803509C: .4byte 0xFFFFFC00
_080350A0: .4byte gUnknown_03001300

	thumb_func_start sub_80350A4
sub_80350A4: @ 0x080350A4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	adds r5, r0, #0
	adds r4, r5, #0
	ldr r0, [r5]
	cmp r0, #0
	beq _080350DE
_080350BA:
	ldr r2, [r4]
	ldr r0, [r2, #8]
	subs r0, #1
	str r0, [r2, #8]
	ldr r1, [r2, #0xc]
	adds r0, r0, r1
	cmp r0, #0
	bgt _080350D6
	ldr r0, [r2]
	str r0, [r4]
	adds r0, r2, #0
	bl sub_8026ED0
	b _080350D8
_080350D6:
	adds r4, r2, #0
_080350D8:
	ldr r0, [r4]
	cmp r0, #0
	bne _080350BA
_080350DE:
	ldr r0, [r5, #0x14]
	cmp r0, #0
	beq _080350EA
	subs r0, #1
	str r0, [r5, #0x14]
	b _08035298
_080350EA:
	mov r8, r5
	ldr r0, [r5]
	ldr r1, _08035154 @ =gUnknown_030012DC
	ldr r4, _08035158 @ =gStaticData_0817CF3C
	cmp r0, #0
	beq _08035102
_080350F6:
	mov r0, r8
	ldr r0, [r0]
	mov r8, r0
	ldr r0, [r0]
	cmp r0, #0
	bne _080350F6
_08035102:
	mov r7, r8
	ldr r0, [r1]
	adds r1, r4, #0
	bl sub_8028968
	str r0, [sp]
	ldr r0, _0803515C @ =gUnknown_030012E0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8028968
	str r0, [sp, #4]
	ldr r1, [sp]
	mov sb, r1
	movs r2, #0
	mov sl, r2
	ldr r0, [r5, #8]
	ldrb r0, [r0]
	cmp r0, #0
	bne _0803512E
	ldr r0, [r5, #4]
	str r0, [r5, #8]
_0803512E:
	ldr r4, [r5, #8]
	ldrb r0, [r4]
	cmp r0, #0xa
	bne _08035138
	b _08035254
_08035138:
	cmp r0, #0
	bne _0803513E
	b _0803524C
_0803513E:
	adds r3, r5, #0
	adds r3, #0x24
	str r3, [sp, #8]
_08035144:
	movs r2, #0
	movs r6, #0
	ldrb r0, [r4]
	cmp r0, #2
	bne _08035160
	str r6, [r5, #0x10]
	b _08035230
	.align 2, 0
_08035154: .4byte gUnknown_030012DC
_08035158: .4byte gStaticData_0817CF3C
_0803515C: .4byte gUnknown_030012E0
_08035160:
	cmp r0, #3
	bne _0803516A
	movs r0, #1
	str r0, [r5, #0x10]
	b _08035230
_0803516A:
	cmp r0, #1
	bne _080351B4
	adds r0, r4, #1
	str r0, [r5, #8]
	movs r0, #0x18
	bl sub_8026EDC
	str r0, [r7]
	movs r1, #2
	str r1, [r0, #0x10]
	str r6, [r0, #8]
	mov r1, sl
	str r1, [r0, #4]
	ldr r1, [r5, #8]
	ldrb r1, [r1]
	strb r1, [r0, #0x14]
	lsls r1, r1, #1
	ldrb r2, [r0, #0x14]
	adds r1, r1, r2
	lsls r1, r1, #3
	ldr r3, [sp, #8]
	adds r1, r3, r1
	ldr r1, [r1]
	str r1, [r0, #0xc]
	str r6, [r0]
	adds r7, r0, #0
	ldrb r0, [r7, #0x14]
	lsls r1, r0, #1
	adds r1, r1, r0
	lsls r1, r1, #3
	adds r0, r3, r1
	ldr r6, [r0]
	adds r0, r5, #0
	adds r0, #0x28
	adds r0, r0, r1
	ldr r2, [r0]
	b _08035230
_080351B4:
	ldr r0, [r5, #0x10]
	cmp r0, #0
	bne _080351E0
	ldr r0, _080351DC @ =gUnknown_030012DC
	ldr r0, [r0]
	movs r2, #0x98
	lsls r2, r2, #1
	adds r1, r0, r2
	ldr r2, [r1]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r3, [r2, #0x1c]
	adds r1, r4, #0
	movs r2, #1
	bl sub_803AD84
	adds r2, r0, #0
	ldr r6, [sp]
	b _08035200
	.align 2, 0
_080351DC: .4byte gUnknown_030012DC
_080351E0:
	ldr r0, _080352A8 @ =gUnknown_030012E0
	ldr r0, [r0]
	movs r2, #0x98
	lsls r2, r2, #1
	adds r1, r0, r2
	ldr r2, [r1]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r3, [r2, #0x1c]
	adds r1, r4, #0
	movs r2, #1
	bl sub_803AD84
	adds r2, r0, #0
	ldr r6, [sp, #4]
_08035200:
	ldr r0, [r5, #8]
	ldrb r0, [r0]
	cmp r0, #0x20
	beq _08035230
	movs r0, #0x18
	str r2, [sp, #0xc]
	bl sub_8026EDC
	str r0, [r7]
	ldr r1, [r5, #0x10]
	str r1, [r0, #0x10]
	movs r1, #0
	str r1, [r0, #8]
	mov r3, sl
	str r3, [r0, #4]
	ldr r1, [r5, #8]
	ldrb r1, [r1]
	strb r1, [r0, #0x14]
	ldr r0, [r7]
	str r6, [r0, #0xc]
	adds r7, r0, #0
	movs r0, #0
	str r0, [r7]
	ldr r2, [sp, #0xc]
_08035230:
	add sl, r2
	cmp sb, r6
	bge _08035238
	mov sb, r6
_08035238:
	ldr r0, [r5, #8]
	adds r1, r0, #1
	str r1, [r5, #8]
	ldrb r0, [r0, #1]
	cmp r0, #0xa
	beq _08035254
	adds r4, r1, #0
	cmp r0, #0
	beq _0803524C
	b _08035144
_0803524C:
	ldr r0, [r5, #8]
	ldrb r0, [r0]
	cmp r0, #0xa
	bne _0803525A
_08035254:
	ldr r0, [r5, #8]
	adds r0, #1
	str r0, [r5, #8]
_0803525A:
	mov r1, r8
	ldr r3, [r1]
	mov r6, sb
	adds r6, #6
	cmp r3, #0
	beq _08035296
	movs r0, #0xf0
	mov r2, sl
	subs r0, r0, r2
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r4, r0, #1
_08035272:
	ldr r0, [r3, #8]
	adds r2, r0, #0
	adds r2, #0xa0
	ldr r1, [r3, #0xc]
	adds r0, r0, r1
	mov r1, sb
	subs r0, r1, r0
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	adds r2, r2, r0
	str r2, [r3, #8]
	ldr r0, [r3, #4]
	adds r0, r0, r4
	str r0, [r3, #4]
	ldr r3, [r3]
	cmp r3, #0
	bne _08035272
_08035296:
	str r6, [r5, #0x14]
_08035298:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080352A8: .4byte gUnknown_030012E0

	thumb_func_start sub_80352AC
sub_80352AC: @ 0x080352AC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x24
	str r0, [sp, #4]
	ldr r0, _0803543C @ =gUnknown_030012B8
	ldr r0, [r0]
	adds r0, #0x2c
	str r0, [sp, #8]
	movs r0, #1
	str r0, [sp, #0xc]
	movs r5, #0
_080352C8:
	lsls r0, r5, #2
	adds r0, r0, r5
	lsls r0, r0, #2
	ldr r1, _08035440 @ =gStaticData_0817CF40
	adds r7, r0, r1
	lsls r0, r5, #1
	adds r0, r0, r5
	lsls r0, r0, #3
	adds r0, #0x1c
	ldr r1, [sp, #4]
	adds r1, r1, r0
	mov r8, r1
	ldr r1, [r7]
	ldr r2, [r7, #4]
	lsls r0, r2, #3
	mov r3, r8
	str r0, [r3, #8]
	lsls r0, r1, #3
	str r0, [r3, #0xc]
	adds r0, r1, #3
	cmp r0, #0
	bge _080352F6
	adds r0, r1, #6
_080352F6:
	asrs r0, r0, #2
	mov r1, r8
	str r0, [r1]
	adds r0, r2, #3
	cmp r0, #0
	bge _08035304
	adds r0, r2, #6
_08035304:
	asrs r0, r0, #2
	mov r2, r8
	str r0, [r2, #4]
	ldr r0, [r7, #0xc]
	ldr r0, [r0]
	lsrs r0, r0, #8
	bl sub_8026EC0
	str r0, [sp, #0x10]
	ldr r0, [r7, #0xc]
	ldr r1, [sp, #0x10]
	bl LoadTaggedAsset
	mov r3, r8
	ldr r1, [r3]
	ldr r0, [r3, #4]
	adds r4, r1, #0
	muls r4, r0, r4
	lsls r4, r4, #9
	adds r0, r4, #0
	bl sub_8026EC0
	mov r1, r8
	str r0, [r1, #0x14]
	movs r1, #0
	str r1, [sp]
	ldr r2, _08035444 @ =0x040000D4
	mov r3, sp
	str r3, [r2]
	str r0, [r2, #4]
	cmp r4, #0
	bge _08035346
	adds r4, #3
_08035346:
	asrs r0, r4, #2
	movs r1, #0x85
	lsls r1, r1, #0x18
	orrs r0, r1
	str r0, [r2, #8]
	ldr r0, [r2, #8]
	movs r6, #0
	ldr r0, [r7, #4]
	ldr r1, [sp, #0xc]
	adds r1, #1
	str r1, [sp, #0x18]
	adds r5, #1
	str r5, [sp, #0x1c]
	cmp r6, r0
	bge _080353CC
_08035364:
	movs r4, #0
	ldr r3, [r7]
	adds r2, r6, #1
	str r2, [sp, #0x20]
	cmp r4, r3
	bge _080353C4
	asrs r0, r6, #2
	str r0, [sp, #0x14]
	movs r1, #3
	mov sl, r1
	adds r0, r6, #0
	ands r0, r1
	lsls r0, r0, #2
	mov sb, r0
	ldr r5, _08035444 @ =0x040000D4
	mov r2, r8
	ldr r2, [r2, #0x14]
	mov ip, r2
_08035388:
	mov r1, r8
	ldr r0, [r1]
	ldr r2, [sp, #0x14]
	adds r1, r2, #0
	muls r1, r0, r1
	asrs r0, r4, #2
	adds r1, r1, r0
	adds r2, r4, #0
	mov r0, sl
	ands r2, r0
	add r2, sb
	adds r0, r6, #0
	muls r0, r3, r0
	adds r0, r0, r4
	lsls r0, r0, #5
	ldr r3, [sp, #0x10]
	adds r0, r3, r0
	str r0, [r5]
	lsls r1, r1, #4
	adds r1, r1, r2
	lsls r1, r1, #5
	add r1, ip
	str r1, [r5, #4]
	ldr r0, _08035448 @ =0x84000008
	str r0, [r5, #8]
	ldr r0, [r5, #8]
	adds r4, #1
	ldr r3, [r7]
	cmp r4, r3
	blt _08035388
_080353C4:
	ldr r6, [sp, #0x20]
	ldr r0, [r7, #4]
	cmp r6, r0
	blt _08035364
_080353CC:
	ldr r0, [sp, #0x10]
	cmp r0, #0
	beq _080353D6
	bl sub_8026EB4
_080353D6:
	ldr r0, [r7, #8]
	ldr r0, [r0]
	lsrs r0, r0, #9
	lsls r0, r0, #1
	bl sub_8026EC0
	adds r4, r0, #0
	ldr r0, [r7, #8]
	adds r1, r4, #0
	bl LoadTaggedAsset
	adds r2, r4, #0
	ldr r1, [sp, #0xc]
	lsls r0, r1, #5
	ldr r3, [sp, #8]
	adds r1, r0, r3
	movs r3, #0xf
_080353F8:
	ldrh r0, [r2]
	strh r0, [r1]
	adds r2, #2
	adds r1, #2
	subs r3, #1
	cmp r3, #0
	bge _080353F8
	cmp r4, #0
	beq _08035410
	adds r0, r4, #0
	bl sub_8026EB4
_08035410:
	ldr r0, _0803543C @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r1, [sp, #0xc]
	bl sub_8006D50
	ldr r0, [sp, #0xc]
	mov r1, r8
	str r0, [r1, #0x10]
	ldr r2, [sp, #0x18]
	str r2, [sp, #0xc]
	ldr r5, [sp, #0x1c]
	cmp r5, #4
	bgt _0803542C
	b _080352C8
_0803542C:
	add sp, #0x24
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0803543C: .4byte gUnknown_030012B8
_08035440: .4byte gStaticData_0817CF40
_08035444: .4byte 0x040000D4
_08035448: .4byte 0x84000008

	thumb_func_start sub_803544C
sub_803544C: @ 0x0803544C
	push {lr}
	bl sub_8001614
	ldr r1, _08035470 @ =0x04000010
	movs r0, #0
	str r0, [r1]
	ldr r0, _08035474 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	ldr r0, _08035478 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	pop {r0}
	bx r0
	.align 2, 0
_08035470: .4byte 0x04000010
_08035474: .4byte gUnknown_030012B8
_08035478: .4byte gUnknown_03001300

	thumb_func_start sub_803547C
sub_803547C: @ 0x0803547C
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	adds r7, r1, #0
	ldr r0, [r6, #0xc]
	cmp r0, #0
	beq _0803548E
	movs r1, #3
	bl sub_80346FC
_0803548E:
	adds r4, r6, #0
	adds r4, #0x30
	movs r5, #4
_08035494:
	ldr r0, [r4]
	cmp r0, #0
	beq _0803549E
	bl sub_8026EB4
_0803549E:
	adds r4, #0x18
	subs r5, #1
	cmp r5, #0
	bge _08035494
	movs r0, #1
	ands r0, r7
	cmp r0, #0
	beq _080354B4
	adds r0, r6, #0
	bl sub_8026ED0
_080354B4:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80354BC
sub_80354BC: @ 0x080354BC
	push {r4, lr}
	movs r0, #0x98
	bl sub_8026EDC
	bl sub_8034CEC
	adds r4, r0, #0
	bl sub_8034E2C
	cmp r4, #0
	beq _080354DA
	adds r0, r4, #0
	movs r1, #3
	bl sub_803547C
_080354DA:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start LoadLevelGraphics
LoadLevelGraphics: @ 0x080354E0
	push {r4, r5, lr}
	adds r5, r0, #0
	ldr r0, _080355B0 @ =gUnknown_030012DC
	ldr r0, [r0]
	str r0, [r5, #0xc]
	ldr r4, _080355B4 @ =gUnknown_03001300
	ldr r0, [r4]
	bl sub_8006A90
	ldr r0, [r4]
	bl sub_8006A48
	bl sub_80006A8
	ldr r0, [r4]
	bl sub_8006AAC
	ldr r1, _080355B8 @ =0x04000050
	movs r0, #0xff
	str r0, [r1]
	adds r1, #4
	movs r0, #0x10
	strh r0, [r1]
	subs r1, #0x54
	movs r0, #0
	strh r0, [r1]
	ldr r0, [r5, #0xc]
	movs r1, #0xe
	bl sub_8028A30
	ldr r0, [r5, #0xc]
	movs r2, #0x80
	lsls r2, r2, #2
	movs r3, #0x84
	lsls r3, r3, #1
	adds r1, r0, r3
	str r2, [r1]
	subs r2, #0xd0
	adds r1, r0, r2
	ldr r1, [r1]
	adds r1, #0x40
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r1, [r1, #4]
	bl sub_803AD7C
	ldr r0, _080355BC @ =0x040000D4
	ldr r1, _080355C0 @ =gStaticData_0817D034
	str r1, [r0]
	ldr r1, _080355C4 @ =0x050003A0
	str r1, [r0, #4]
	ldr r2, _080355C8 @ =0x80000010
	str r2, [r0, #8]
	ldr r1, [r0, #8]
	ldr r1, _080355CC @ =gStaticData_0817D054
	str r1, [r0]
	ldr r1, _080355D0 @ =0x050003C0
	str r1, [r0, #4]
	str r2, [r0, #8]
	ldr r1, [r0, #8]
	ldr r1, _080355D4 @ =gStaticData_0817D074
	str r1, [r0]
	ldr r1, _080355D8 @ =0x050003E0
	str r1, [r0, #4]
	str r2, [r0, #8]
	ldr r0, [r0, #8]
	adds r0, r5, #0
	bl LoadBg2Background
	adds r0, r5, #0
	bl LoadObjSpriteTiles
	movs r0, #0x82
	lsls r0, r0, #2
	adds r4, r5, r0
	movs r0, #0x14
	bl sub_8026EDC
	bl sub_8034374
	str r0, [r4]
	bl sub_8001604
	bl sub_80015E0
	movs r0, #1
	bl sub_8001524
	bl sub_8001614
	movs r0, #0
	str r0, [r5]
	str r0, [r5, #4]
	ldr r0, _080355DC @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0xb
	bl sub_80017BC
	adds r0, r5, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_080355B0: .4byte gUnknown_030012DC
_080355B4: .4byte gUnknown_03001300
_080355B8: .4byte 0x04000050
_080355BC: .4byte 0x040000D4
_080355C0: .4byte gStaticData_0817D034
_080355C4: .4byte 0x050003A0
_080355C8: .4byte 0x80000010
_080355CC: .4byte gStaticData_0817D054
_080355D0: .4byte 0x050003C0
_080355D4: .4byte gStaticData_0817D074
_080355D8: .4byte 0x050003E0
_080355DC: .4byte gUnknown_030012BC

	thumb_func_start LoadBg2Background
LoadBg2Background: @ 0x080355E0
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r4, _08035670 @ =gStaticData_0817D0E4
	ldr r0, [r4, #8]
	movs r1, #0xa0
	lsls r1, r1, #0x13
	bl LoadTaggedAsset
	ldr r0, [r4, #0xc]
	ldr r1, _08035674 @ =0x06008000
	bl LoadTaggedAsset
	ldr r1, [r4, #4]
	ldr r0, [r4]
	muls r0, r1, r0
	lsls r0, r0, #1
	bl sub_8026EC0
	mov r8, r0
	ldr r0, [r4, #0x10]
	mov r1, r8
	bl LoadTaggedAsset
	ldr r6, _08035678 @ =0x0600F000
	movs r3, #0
	ldr r1, [r4, #4]
	ldr r0, [r4]
	muls r1, r0, r1
	cmp r3, r1
	bge _08035640
	movs r4, #0xff
	mov ip, r1
	mov r2, r8
_08035624:
	adds r1, r4, #0
	ldrh r0, [r2]
	ands r1, r0
	adds r0, r4, #0
	ldrh r7, [r2, #2]
	ands r0, r7
	lsls r0, r0, #8
	orrs r1, r0
	strh r1, [r6]
	adds r6, #2
	adds r2, #4
	adds r3, #2
	cmp r3, ip
	blt _08035624
_08035640:
	ldr r0, _0803567C @ =0xFFFF0000
	ands r5, r0
	movs r0, #8
	orrs r5, r0
	movs r0, #0xf0
	lsls r0, r0, #5
	orrs r5, r0
	movs r0, #0x80
	orrs r5, r0
	movs r0, #1
	orrs r5, r0
	ldr r0, _08035680 @ =0x0400000C
	strh r5, [r0]
	mov r0, r8
	cmp r0, #0
	beq _08035664
	bl sub_8026EB4
_08035664:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08035670: .4byte gStaticData_0817D0E4
_08035674: .4byte 0x06008000
_08035678: .4byte 0x0600F000
_0803567C: .4byte 0xFFFF0000
_08035680: .4byte 0x0400000C

	thumb_func_start LoadObjSpriteTiles
LoadObjSpriteTiles: @ 0x08035684
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	ldr r7, _0803576C @ =gUnknown_030008BC
	ldr r0, _08035770 @ =0x06010000
	mov sl, r0
	ldr r1, _08035774 @ =0x05000200
	str r1, [sp]
	movs r4, #0
	mov sb, r4
_0803569E:
	ldr r0, [r7]
	ldr r0, [r0, #8]
	ldr r0, [r0]
	lsrs r0, r0, #8
	bl sub_8026EC0
	adds r4, r0, #0
	ldr r0, [r7]
	ldr r0, [r0, #8]
	adds r1, r4, #0
	bl LoadTaggedAsset
	ldr r0, _08035778 @ =0x040000D4
	str r4, [r0]
	ldr r1, [sp]
	str r1, [r0, #4]
	ldr r1, _0803577C @ =0x80000010
	str r1, [r0, #8]
	ldr r0, [r0, #8]
	ldr r0, [sp]
	adds r0, #0x20
	str r0, [sp]
	cmp r4, #0
	beq _080356D4
	adds r0, r4, #0
	bl sub_8026EB4
_080356D4:
	ldr r0, [r7]
	ldr r0, [r0, #0xc]
	ldr r0, [r0]
	lsrs r0, r0, #8
	bl sub_8026EC0
	adds r6, r0, #0
	ldr r0, [r7]
	ldr r0, [r0, #0xc]
	adds r1, r6, #0
	bl LoadTaggedAsset
	ldr r0, [r7]
	ldr r1, [r0, #4]
	ldr r0, [r0]
	adds r4, r1, #0
	muls r4, r0, r4
	lsls r0, r4, #1
	bl sub_8026EC0
	adds r5, r0, #0
	ldm r7!, {r0}
	ldr r0, [r0, #0x10]
	adds r1, r5, #0
	bl LoadTaggedAsset
	mov r8, r7
	movs r1, #1
	add sb, r1
	cmp r4, #0
	ble _0803573E
	ldr r3, _08035778 @ =0x040000D4
	movs r0, #0xff
	mov ip, r0
	ldr r7, _0803577C @ =0x80000010
	adds r2, r5, #0
	adds r1, r4, #0
_0803571E:
	mov r0, ip
	ldrh r4, [r2]
	ands r0, r4
	lsls r0, r0, #5
	adds r0, r6, r0
	str r0, [r3]
	mov r0, sl
	str r0, [r3, #4]
	str r7, [r3, #8]
	ldr r0, [r3, #8]
	movs r4, #0x20
	add sl, r4
	adds r2, #2
	subs r1, #1
	cmp r1, #0
	bne _0803571E
_0803573E:
	cmp r5, #0
	beq _08035748
	adds r0, r5, #0
	bl sub_8026EB4
_08035748:
	cmp r6, #0
	beq _08035752
	adds r0, r6, #0
	bl sub_8026EB4
_08035752:
	mov r7, r8
	mov r0, sb
	cmp r0, #3
	ble _0803569E
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0803576C: .4byte gUnknown_030008BC
_08035770: .4byte 0x06010000
_08035774: .4byte 0x05000200
_08035778: .4byte 0x040000D4
_0803577C: .4byte 0x80000010

	thumb_func_start sub_8035780
sub_8035780: @ 0x08035780
	push {r4, r5, lr}
	mov ip, r0
	movs r5, #0
_08035786:
	movs r0, #0x34
	adds r3, r5, #0
	muls r3, r0, r3
	mov r0, ip
	adds r0, #0x14
	adds r4, r0, r3
	ldr r0, [r4]
	cmp r0, #0
	bne _0803579A
	b _0803589A
_0803579A:
	subs r0, #1
	str r0, [r4]
	cmp r0, #0
	bne _08035836
	mov r1, ip
	adds r1, #0x40
	adds r1, r1, r3
	ldr r0, [r1]
	adds r2, r0, #0
	adds r0, #0x20
	str r0, [r1]
	mov r0, ip
	adds r1, r0, r3
	movs r0, #1
	strb r0, [r1, #0x10]
	movs r1, #0
	ldrsh r0, [r2, r1]
	str r0, [r4]
	cmp r0, #0
	beq _0803589A
	mov r0, ip
	adds r0, #0x20
	adds r0, r0, r3
	ldrh r4, [r2, #6]
	lsls r1, r4, #0x10
	str r1, [r0]
	mov r0, ip
	adds r0, #0x34
	adds r0, r0, r3
	ldr r1, [r2, #0x14]
	str r1, [r0]
	mov r1, ip
	adds r1, #0x24
	adds r1, r1, r3
	movs r4, #8
	ldrsh r0, [r2, r4]
	lsls r0, r0, #8
	str r0, [r1]
	mov r0, ip
	adds r0, #0x38
	adds r0, r0, r3
	ldr r1, [r2, #0x18]
	str r1, [r0]
	mov r1, ip
	adds r1, #0x28
	adds r1, r1, r3
	movs r4, #0xa
	ldrsh r0, [r2, r4]
	lsls r0, r0, #8
	str r0, [r1]
	mov r0, ip
	adds r0, #0x3c
	adds r0, r0, r3
	ldr r1, [r2, #0x1c]
	str r1, [r0]
	mov r0, ip
	adds r0, #0x18
	adds r0, r0, r3
	ldrh r4, [r2, #2]
	lsls r1, r4, #0x10
	str r1, [r0]
	mov r0, ip
	adds r0, #0x2c
	adds r0, r0, r3
	ldr r1, [r2, #0xc]
	str r1, [r0]
	mov r0, ip
	adds r0, #0x1c
	adds r0, r0, r3
	ldrh r4, [r2, #4]
	lsls r1, r4, #0x10
	str r1, [r0]
	mov r0, ip
	adds r0, #0x30
	adds r0, r0, r3
	ldr r1, [r2, #0x10]
	str r1, [r0]
	b _0803589A
_08035836:
	mov r2, ip
	adds r2, #0x20
	adds r2, r2, r3
	mov r0, ip
	adds r0, #0x34
	adds r0, r0, r3
	ldr r1, [r2]
	ldr r0, [r0]
	adds r1, r1, r0
	str r1, [r2]
	mov r2, ip
	adds r2, #0x24
	adds r2, r2, r3
	mov r0, ip
	adds r0, #0x38
	adds r0, r0, r3
	ldr r1, [r2]
	ldr r0, [r0]
	adds r1, r1, r0
	str r1, [r2]
	mov r2, ip
	adds r2, #0x28
	adds r2, r2, r3
	mov r0, ip
	adds r0, #0x3c
	adds r0, r0, r3
	ldr r1, [r2]
	ldr r0, [r0]
	adds r1, r1, r0
	str r1, [r2]
	mov r2, ip
	adds r2, #0x18
	adds r2, r2, r3
	mov r0, ip
	adds r0, #0x2c
	adds r0, r0, r3
	ldr r1, [r2]
	ldr r0, [r0]
	adds r1, r1, r0
	str r1, [r2]
	mov r2, ip
	adds r2, #0x1c
	adds r2, r2, r3
	mov r0, ip
	adds r0, #0x30
	adds r0, r0, r3
	ldr r1, [r2]
	ldr r0, [r0]
	adds r1, r1, r0
	str r1, [r2]
_0803589A:
	adds r5, #1
	cmp r5, #8
	bgt _080358A2
	b _08035786
_080358A2:
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_80358A8
sub_80358A8: @ 0x080358A8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x2c
	mov sl, r0
	movs r0, #0
	mov sb, r0
	movs r7, #0xd8
	lsls r7, r7, #1
	add r7, sl
	ldrb r0, [r7]
	cmp r0, #0
	bne _080358C8
	b _080359DE
_080358C8:
	ldr r1, [r7, #0x14]
	movs r0, #0x80
	lsls r0, r0, #0x11
	bl sub_803ADB4
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	ldr r1, _08035A98 @ =gUnknown_03001300
	ldr r1, [r1]
	mov r8, r1
	strh r0, [r1, #0x12]
	strh r0, [r1, #0x2a]
	mov r0, r8
	adds r0, #8
	mov r2, sb
	strh r2, [r0, #0x12]
	adds r0, #8
	strh r2, [r0, #0x12]
	mov r0, sp
	strh r2, [r0]
	ldr r1, _08035A9C @ =0x040000D4
	str r0, [r1]
	add r4, sp, #4
	str r4, [r1, #4]
	ldr r0, _08035AA0 @ =0x81000004
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldrb r3, [r4, #1]
	movs r0, #3
	orrs r3, r0
	ldrb r0, [r4, #3]
	movs r1, #0xf
	rsbs r1, r1, #0
	ands r1, r0
	movs r0, #0xf
	ldrb r5, [r4, #5]
	ands r0, r5
	movs r2, #0x30
	orrs r0, r2
	strb r0, [r4, #5]
	movs r2, #0x3f
	ands r1, r2
	movs r0, #0x80
	orrs r1, r0
	strb r1, [r4, #3]
	ands r3, r2
	movs r0, #0x40
	orrs r3, r0
	strb r3, [r4, #1]
	movs r1, #0xe
	ldrsh r0, [r7, r1]
	subs r0, #0x10
	strb r0, [r4]
	ldr r6, _08035AA4 @ =0xFFFFFC00
	adds r0, r6, #0
	ldrh r2, [r4, #4]
	ands r0, r2
	movs r3, #0xe0
	lsls r3, r3, #1
	adds r1, r3, #0
	orrs r0, r1
	strh r0, [r4, #4]
	movs r5, #0xa
	ldrsh r1, [r7, r5]
	ldr r0, [r7, #0x14]
	lsls r0, r0, #5
	asrs r0, r0, #0x10
	adds r0, #0x20
	subs r1, r1, r0
	ldr r0, _08035AA8 @ =0x000001FF
	mov sb, r0
	mov r2, sb
	ands r1, r2
	ldrh r2, [r4, #2]
	ldr r5, _08035AAC @ =0xFFFFFE00
	adds r0, r5, #0
	ands r0, r2
	orrs r0, r1
	strh r0, [r4, #2]
	mov r0, r8
	adds r1, r4, #0
	bl sub_8006AC8
	ldrh r2, [r4, #4]
	lsls r1, r2, #0x16
	lsrs r1, r1, #0x16
	adds r1, #8
	ldr r3, _08035AB0 @ =0x000003FF
	mov r8, r3
	mov r0, r8
	ands r1, r0
	adds r0, r6, #0
	ands r0, r2
	orrs r0, r1
	strh r0, [r4, #4]
	movs r2, #0xa
	ldrsh r1, [r7, r2]
	subs r1, #0x20
	mov r3, sb
	ands r1, r3
	ldrh r2, [r4, #2]
	adds r0, r5, #0
	ands r0, r2
	orrs r0, r1
	strh r0, [r4, #2]
	ldr r1, _08035A98 @ =gUnknown_03001300
	ldr r0, [r1]
	adds r1, r4, #0
	bl sub_8006AC8
	ldrh r1, [r4, #4]
	lsls r0, r1, #0x16
	lsrs r0, r0, #0x16
	adds r0, #8
	mov r2, r8
	ands r0, r2
	ands r6, r1
	orrs r6, r0
	strh r6, [r4, #4]
	movs r3, #0xa
	ldrsh r1, [r7, r3]
	ldr r0, [r7, #0x14]
	lsls r0, r0, #5
	asrs r0, r0, #0x10
	subs r0, #0x20
	adds r1, r1, r0
	mov r7, sb
	ands r1, r7
	ldrh r0, [r4, #2]
	ands r5, r0
	orrs r5, r1
	strh r5, [r4, #2]
	ldr r1, _08035A98 @ =gUnknown_03001300
	ldr r0, [r1]
	adds r1, r4, #0
	bl sub_8006AC8
	movs r2, #2
	mov sb, r2
_080359DE:
	movs r7, #0
	mov r3, sp
	adds r3, #0x14
	str r3, [sp, #0x24]
	mov r5, sl
	adds r5, #0x78
	str r5, [sp, #0x20]
	movs r0, #0
	mov r8, r0
_080359F0:
	movs r0, #0x34
	muls r0, r7, r0
	ldr r1, _08035AB4 @ =0xFFFFFE84
	adds r0, r0, r1
	mov r2, sl
	subs r6, r2, r0
	ldrb r0, [r6]
	cmp r0, #0
	bne _08035A04
	b _08035B38
_08035A04:
	movs r0, #7
	subs r0, r0, r7
	lsls r0, r0, #2
	movs r1, #0xf2
	lsls r1, r1, #1
	add r1, sl
	adds r1, r1, r0
	ldr r2, [r1]
	cmp r2, #0
	beq _08035A3C
	movs r0, #1
	rsbs r0, r0, #0
	cmp r2, r0
	bne _08035A24
	movs r0, #0xa
	str r0, [r1]
_08035A24:
	ldr r0, [r1]
	subs r0, #1
	str r0, [r1]
	cmp r0, #0
	bne _08035A3C
	ldr r0, _08035AB8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x4a
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
_08035A3C:
	ldr r1, [r6, #0x14]
	movs r0, #0x80
	lsls r0, r0, #0x11
	bl sub_803ADB4
	lsls r3, r0, #0x10
	lsrs r3, r3, #0x10
	ldr r5, _08035A98 @ =gUnknown_03001300
	mov ip, r5
	ldr r4, [r5]
	mov r1, sb
	lsls r2, r1, #2
	lsls r1, r1, #5
	adds r1, r4, r1
	strh r3, [r1, #0x12]
	adds r1, r2, #3
	lsls r1, r1, #3
	adds r1, r4, r1
	strh r3, [r1, #0x12]
	adds r1, r2, #1
	lsls r1, r1, #3
	adds r1, r4, r1
	mov r3, r8
	strh r3, [r1, #0x12]
	adds r2, #2
	lsls r2, r2, #3
	adds r4, r4, r2
	strh r3, [r4, #0x12]
	mov r1, sp
	strh r3, [r1]
	ldr r2, _08035A9C @ =0x040000D4
	str r1, [r2]
	add r3, sp, #0xc
	str r3, [r2, #4]
	ldr r1, _08035AA0 @ =0x81000004
	str r1, [r2, #8]
	ldr r1, [r2, #8]
	movs r4, #0
	movs r1, #0x80
	lsls r1, r1, #1
	cmp r0, r1
	beq _08035ABC
	subs r4, #0x20
	ldrb r0, [r3, #1]
	movs r1, #3
	b _08035AC8
	.align 2, 0
_08035A98: .4byte gUnknown_03001300
_08035A9C: .4byte 0x040000D4
_08035AA0: .4byte 0x81000004
_08035AA4: .4byte 0xFFFFFC00
_08035AA8: .4byte 0x000001FF
_08035AAC: .4byte 0xFFFFFE00
_08035AB0: .4byte 0x000003FF
_08035AB4: .4byte 0xFFFFFE84
_08035AB8: .4byte gUnknown_030012BC
_08035ABC:
	ldrb r0, [r3, #1]
	movs r5, #4
	rsbs r5, r5, #0
	adds r1, r5, #0
	ands r0, r1
	movs r1, #1
_08035AC8:
	orrs r0, r1
	strb r0, [r3, #1]
	movs r0, #7
	mov r1, sb
	ands r1, r0
	lsls r1, r1, #1
	ldrb r2, [r3, #3]
	movs r5, #0xf
	rsbs r5, r5, #0
	adds r0, r5, #0
	ands r2, r0
	orrs r2, r1
	movs r0, #1
	add sb, r0
	movs r0, #0xf
	ldrb r1, [r3, #5]
	ands r0, r1
	strb r0, [r3, #5]
	lsls r1, r7, #6
	ldr r5, _08035B88 @ =0x000003FF
	adds r0, r5, #0
	ands r1, r0
	ldr r5, _08035B8C @ =0xFFFFFC00
	adds r0, r5, #0
	ldrh r5, [r3, #4]
	ands r0, r5
	orrs r0, r1
	strh r0, [r3, #4]
	movs r0, #0xc0
	orrs r2, r0
	strb r2, [r3, #3]
	movs r0, #0xa
	ldrsh r2, [r6, r0]
	adds r0, r4, #0
	subs r0, #0x20
	adds r2, r2, r0
	ldr r1, _08035B90 @ =0x000001FF
	adds r0, r1, #0
	ands r2, r0
	ldrh r0, [r3, #2]
	ldr r5, _08035B94 @ =0xFFFFFE00
	adds r1, r5, #0
	ands r0, r1
	orrs r0, r2
	strh r0, [r3, #2]
	movs r1, #0xe
	ldrsh r0, [r6, r1]
	subs r0, #0x20
	adds r0, r0, r4
	add r1, sp, #0xc
	strb r0, [r1]
	mov r2, ip
	ldr r0, [r2]
	adds r1, r3, #0
	bl sub_8006AC8
_08035B38:
	adds r7, #1
	cmp r7, #4
	bgt _08035B40
	b _080359F0
_08035B40:
	ldr r3, _08035B98 @ =gStaticData_0817CFF4
	str r3, [sp, #0x1c]
	movs r4, #0
	ldr r5, _08035B9C @ =0x040000D4
	mov sb, r5
	ldr r5, [sp, #0x24]
_08035B4C:
	movs r0, #0x34
	muls r0, r4, r0
	adds r0, #0x10
	add r0, sl
	str r0, [sp, #0x28]
	ldrb r0, [r0]
	cmp r0, #0
	beq _08035BB6
	lsls r1, r4, #2
	movs r0, #0xf2
	lsls r0, r0, #1
	add r0, sl
	adds r2, r0, r1
	ldr r1, [r2]
	cmp r1, #0
	beq _08035BB6
	movs r0, #1
	rsbs r0, r0, #0
	cmp r1, r0
	bne _08035BA4
	movs r0, #8
	str r0, [r2]
	ldr r0, _08035BA0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x3d
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	b _08035BB6
	.align 2, 0
_08035B88: .4byte 0x000003FF
_08035B8C: .4byte 0xFFFFFC00
_08035B90: .4byte 0x000001FF
_08035B94: .4byte 0xFFFFFE00
_08035B98: .4byte gStaticData_0817CFF4
_08035B9C: .4byte 0x040000D4
_08035BA0: .4byte gUnknown_030012BC
_08035BA4:
	subs r0, r1, #1
	str r0, [r2]
	cmp r0, #0
	bne _08035BB6
	movs r1, #0x83
	lsls r1, r1, #2
	add r1, sl
	movs r0, #0x1e
	str r0, [r1]
_08035BB6:
	mov r1, sp
	movs r0, #0
	strh r0, [r1]
	mov r7, sb
	str r1, [r7]
	str r5, [r7, #4]
	ldr r0, _08035D04 @ =0x81000004
	str r0, [r7, #8]
	ldr r0, [r7, #8]
	adds r2, r4, #1
	lsls r1, r2, #4
	movs r0, #0xf
	ldrb r3, [r5, #5]
	ands r0, r3
	orrs r0, r1
	strb r0, [r5, #5]
	lsls r1, r4, #6
	movs r7, #0xa0
	lsls r7, r7, #1
	adds r1, r1, r7
	ldr r3, _08035D08 @ =0x000003FF
	adds r0, r3, #0
	ands r1, r0
	ldr r7, _08035D0C @ =0xFFFFFC00
	adds r0, r7, #0
	ldrh r3, [r5, #4]
	ands r0, r3
	orrs r0, r1
	strh r0, [r5, #4]
	ldrb r1, [r5, #3]
	movs r0, #0x3f
	ands r0, r1
	movs r1, #0x80
	orrs r0, r1
	strb r0, [r5, #3]
	movs r7, #0xd
	rsbs r7, r7, #0
	adds r0, r7, #0
	ldrb r1, [r5, #5]
	ands r0, r1
	movs r1, #8
	orrs r0, r1
	strb r0, [r5, #5]
	mov r8, r2
	ldr r4, [sp, #0x24]
	movs r6, #3
_08035C12:
	ldr r2, [sp, #0x28]
	movs r3, #0xa
	ldrsh r1, [r2, r3]
	ldr r7, [sp, #0x1c]
	ldm r7!, {r0}
	adds r2, r1, r0
	ldr r0, [sp, #0x28]
	movs r3, #0xe
	ldrsh r1, [r0, r3]
	ldm r7!, {r0}
	str r7, [sp, #0x1c]
	adds r3, r1, r0
	cmp r3, #0x8b
	bgt _08035C56
	ldr r1, _08035D10 @ =0x000001FF
	adds r0, r1, #0
	ands r2, r0
	ldrh r0, [r4, #2]
	ldr r7, _08035D14 @ =0xFFFFFE00
	adds r1, r7, #0
	ands r0, r1
	orrs r0, r2
	strh r0, [r4, #2]
	add r0, sp, #0x14
	strb r3, [r0]
	ldr r1, [sp, #0x28]
	ldrb r0, [r1]
	cmp r0, #0
	beq _08035C56
	ldr r0, _08035D18 @ =gUnknown_03001300
	ldr r0, [r0]
	ldr r1, [sp, #0x24]
	bl sub_8006AC8
_08035C56:
	ldrh r2, [r4, #4]
	lsls r0, r2, #0x16
	lsrs r0, r0, #0x16
	adds r0, #0x10
	ldr r3, _08035D08 @ =0x000003FF
	adds r1, r3, #0
	ands r0, r1
	ldr r7, _08035D0C @ =0xFFFFFC00
	adds r1, r7, #0
	ands r2, r1
	orrs r2, r0
	strh r2, [r4, #4]
	subs r6, #1
	cmp r6, #0
	bge _08035C12
	mov r4, r8
	cmp r4, #1
	bgt _08035C7C
	b _08035B4C
_08035C7C:
	ldr r4, [sp, #0x20]
	ldrb r0, [r4]
	cmp r0, #0
	beq _08035CF2
	bl sub_80015B0
	ldr r5, [r4, #8]
	ldr r6, [r4, #0xc]
	movs r1, #0x83
	lsls r1, r1, #2
	add r1, sl
	ldr r0, [r1]
	cmp r0, #0
	beq _08035CB8
	subs r0, #1
	str r0, [r1]
	movs r0, #0xa
	bl sub_8000E1C
	subs r1, r5, #5
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r5, r1, r0
	movs r0, #0xa
	bl sub_8000E1C
	subs r1, r6, #5
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r6, r1, r0
_08035CB8:
	movs r4, #0x87
	lsls r4, r4, #2
	add r4, sl
	ldr r0, [sp, #0x20]
	ldr r1, [r0, #0x14]
	movs r0, #0x80
	lsls r0, r0, #0x11
	bl sub_803ADB4
	str r0, [r4]
	movs r3, #0x85
	lsls r3, r3, #2
	add r3, sl
	rsbs r1, r5, #0
	asrs r1, r1, #0x10
	muls r0, r1, r0
	movs r2, #0x80
	lsls r2, r2, #7
	adds r0, r0, r2
	str r0, [r3]
	movs r3, #0x86
	lsls r3, r3, #2
	add r3, sl
	rsbs r0, r6, #0
	asrs r0, r0, #0x10
	ldr r1, [r4]
	muls r0, r1, r0
	adds r0, r0, r2
	str r0, [r3]
_08035CF2:
	add sp, #0x2c
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08035D04: .4byte 0x81000004
_08035D08: .4byte 0x000003FF
_08035D0C: .4byte 0xFFFFFC00
_08035D10: .4byte 0x000001FF
_08035D14: .4byte 0xFFFFFE00
_08035D18: .4byte gUnknown_03001300

	thumb_func_start sub_8035D1C
sub_8035D1C: @ 0x08035D1C
	push {r4, lr}
	adds r3, r0, #0
	adds r2, r1, #0
	ldr r0, _08035D44 @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r4, #0x80
	lsls r4, r4, #1
	adds r1, r4, #0
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r1, r0, #0x10
	cmp r1, #0
	bne _08035D48
	movs r4, #0x84
	lsls r4, r4, #2
	adds r0, r3, r4
	str r1, [r0]
	adds r0, r2, #0
	b _08035E02
	.align 2, 0
_08035D44: .4byte gUnknown_030007E0
_08035D48:
	movs r0, #0x20
	ands r0, r2
	cmp r0, #0
	beq _08035D58
	ldr r1, _08035D54 @ =0x12345678
	b _08035DCA
	.align 2, 0
_08035D54: .4byte 0x12345678
_08035D58:
	movs r0, #0x10
	ands r0, r2
	cmp r0, #0
	beq _08035D70
	ldr r1, _08035D6C @ =0x31415926
	movs r4, #0x84
	lsls r4, r4, #2
	adds r2, r3, r4
	b _08035DD0
	.align 2, 0
_08035D6C: .4byte 0x31415926
_08035D70:
	movs r0, #0x40
	ands r0, r2
	cmp r0, #0
	beq _08035D80
	ldr r1, _08035D7C @ =0xC0DEBA1D
	b _08035DCA
	.align 2, 0
_08035D7C: .4byte 0xC0DEBA1D
_08035D80:
	movs r0, #0x80
	ands r0, r2
	cmp r0, #0
	beq _08035D98
	ldr r1, _08035D94 @ =0xDEADBEEF
	movs r4, #0x84
	lsls r4, r4, #2
	adds r2, r3, r4
	b _08035DD0
	.align 2, 0
_08035D94: .4byte 0xDEADBEEF
_08035D98:
	movs r0, #2
	ands r0, r2
	cmp r0, #0
	beq _08035DA8
	ldr r1, _08035DA4 @ =0xB1E4B1E4
	b _08035DCA
	.align 2, 0
_08035DA4: .4byte 0xB1E4B1E4
_08035DA8:
	movs r0, #1
	ands r0, r2
	cmp r0, #0
	beq _08035DC0
	ldr r1, _08035DBC @ =0x71839406
	movs r4, #0x84
	lsls r4, r4, #2
	adds r2, r3, r4
	b _08035DD0
	.align 2, 0
_08035DBC: .4byte 0x71839406
_08035DC0:
	movs r0, #8
	ands r0, r2
	cmp r0, #0
	beq _08035DE4
	ldr r1, _08035E08 @ =0x828A048B
_08035DCA:
	movs r0, #0x84
	lsls r0, r0, #2
	adds r2, r3, r0
_08035DD0:
	ldr r0, [r2]
	eors r0, r1
	lsls r1, r0, #1
	lsrs r0, r0, #0x1f
	orrs r1, r0
	lsls r0, r1, #6
	adds r0, r0, r1
	lsls r0, r0, #3
	adds r0, r0, r1
	str r0, [r2]
_08035DE4:
	movs r0, #0x84
	lsls r0, r0, #2
	adds r4, r3, r0
	ldr r1, [r4]
	ldr r0, _08035E0C @ =0x3034AF3B
	cmp r1, r0
	bne _08035E00
	ldr r0, _08035E10 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0xc
	bl sub_8001B54
	movs r0, #0
	str r0, [r4]
_08035E00:
	movs r0, #0
_08035E02:
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08035E08: .4byte 0x828A048B
_08035E0C: .4byte 0x3034AF3B
_08035E10: .4byte gUnknown_030012BC

	thumb_func_start sub_8035E14
sub_8035E14: @ 0x08035E14
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r4, r0, #0
	movs r3, #0
	ldr r7, _08035EF8 @ =gStaticData_0817CFA4
	mov r8, r7
	adds r5, r4, #0
	movs r6, #0
_08035E26:
	movs r0, #0
	mov ip, r0
	mov r1, ip
	strb r1, [r5, #0x10]
	adds r2, r4, #0
	adds r2, #0x14
	adds r2, r2, r6
	lsls r0, r3, #3
	mov r1, r8
	adds r1, #4
	adds r0, r0, r1
	ldr r0, [r0]
	adds r0, #1
	str r0, [r2]
	adds r0, r4, #0
	adds r0, #0x40
	adds r0, r0, r6
	ldr r1, [r7]
	str r1, [r0]
	lsls r1, r3, #2
	movs r2, #0xf2
	lsls r2, r2, #1
	adds r0, r4, r2
	adds r0, r0, r1
	movs r1, #1
	rsbs r1, r1, #0
	str r1, [r0]
	adds r7, #8
	adds r5, #0x34
	adds r6, #0x34
	adds r3, #1
	cmp r3, #8
	ble _08035E26
	movs r1, #0x83
	lsls r1, r1, #2
	adds r0, r4, r1
	mov r2, ip
	str r2, [r0]
	strb r2, [r4, #8]
	ldr r0, [r4, #0x14]
	cmp r0, #0
	beq _08035EA8
	ldr r5, _08035EFC @ =0x04000050
_08035E7C:
	adds r0, r4, #0
	bl sub_8035780
	adds r0, r4, #0
	bl sub_8036068
	movs r1, #0x82
	lsls r1, r1, #2
	adds r0, r4, r1
	ldr r0, [r0]
	bl sub_8034688
	bl sub_80006A8
	movs r0, #0
	str r0, [r5]
	adds r0, r4, #0
	bl sub_8035F9C
	ldr r0, [r4, #0x14]
	cmp r0, #0
	bne _08035E7C
_08035EA8:
	movs r0, #0
	movs r1, #1
	strb r1, [r4, #8]
	movs r2, #0x84
	lsls r2, r2, #2
	adds r1, r4, r2
	str r0, [r1]
	ldr r6, _08035F00 @ =gUnknown_030012BC
_08035EB8:
	adds r0, r4, #0
	bl sub_8036068
	movs r1, #0x82
	lsls r1, r1, #2
	adds r0, r4, r1
	ldr r0, [r0]
	bl sub_8034688
	ldr r0, _08035F04 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r0, _08035F08 @ =gUnknown_030007E0
	ldrh r5, [r0, #2]
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_8035D1C
	adds r5, r0, #0
	movs r0, #9
	ands r0, r5
	cmp r0, #0
	beq _08035F0C
	ldr r0, [r6]
	movs r1, #0x49
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	movs r5, #0
	b _08035F5C
	.align 2, 0
_08035EF8: .4byte gStaticData_0817CFA4
_08035EFC: .4byte 0x04000050
_08035F00: .4byte gUnknown_030012BC
_08035F04: .4byte gUnknown_03001304
_08035F08: .4byte gUnknown_030007E0
_08035F0C:
	movs r0, #0x40
	ands r0, r5
	cmp r0, #0
	beq _08035F2E
	ldr r0, [r6]
	movs r1, #0x46
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	ldr r0, [r4]
	cmp r0, #0
	beq _08035F2A
	subs r0, #1
	b _08035F2C
_08035F2A:
	movs r0, #2
_08035F2C:
	str r0, [r4]
_08035F2E:
	movs r0, #0x80
	ands r0, r5
	cmp r0, #0
	beq _08035F50
	ldr r0, [r6]
	movs r1, #0x46
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	ldr r0, [r4]
	adds r0, #1
	str r0, [r4]
	movs r1, #3
	bl sub_803AE4C
	str r0, [r4]
_08035F50:
	bl sub_80006A8
	adds r0, r4, #0
	bl sub_8035F9C
	b _08035EB8
_08035F5C:
	adds r0, r4, #0
	bl sub_8036068
	movs r2, #0x82
	lsls r2, r2, #2
	adds r0, r4, r2
	ldr r0, [r0]
	bl sub_8034688
	bl sub_80006A8
	ldr r0, _08035F94 @ =0x04000054
	strh r5, [r0]
	ldr r1, _08035F98 @ =0x04000050
	movs r0, #0xff
	strh r0, [r1]
	adds r0, r4, #0
	bl sub_8035F9C
	adds r5, #1
	cmp r5, #0x10
	ble _08035F5C
	ldr r0, [r4]
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08035F94: .4byte 0x04000054
_08035F98: .4byte 0x04000050

	thumb_func_start sub_8035F9C
sub_8035F9C: @ 0x08035F9C
	push {lr}
	adds r2, r0, #0
	ldr r1, _08035FE0 @ =0x04000028
	movs r3, #0x85
	lsls r3, r3, #2
	adds r0, r2, r3
	ldr r0, [r0]
	str r0, [r1]
	adds r1, #4
	adds r3, #4
	adds r0, r2, r3
	ldr r0, [r0]
	str r0, [r1]
	subs r1, #0xc
	adds r3, #4
	adds r0, r2, r3
	ldr r2, [r0]
	strh r2, [r1]
	ldr r0, _08035FE4 @ =0x04000022
	movs r1, #0
	strh r1, [r0]
	adds r0, #2
	strh r1, [r0]
	adds r0, #2
	strh r2, [r0]
	bl sub_8001614
	ldr r0, _08035FE8 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	pop {r0}
	bx r0
	.align 2, 0
_08035FE0: .4byte 0x04000028
_08035FE4: .4byte 0x04000022
_08035FE8: .4byte gUnknown_03001300

	thumb_func_start sub_8035FEC
sub_8035FEC: @ 0x08035FEC
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	adds r7, r1, #0
	adds r6, r2, #0
	ldr r0, [r5]
	cmp r6, r0
	bne _08036010
	ldr r1, [r5, #4]
	adds r1, #1
	str r1, [r5, #4]
	ldr r0, [r5, #0xc]
	asrs r1, r1, #2
	movs r2, #1
	ands r1, r2
	adds r1, #0xe
	bl sub_8028A30
	b _08036018
_08036010:
	ldr r0, [r5, #0xc]
	movs r1, #0xd
	bl sub_8028A30
_08036018:
	ldr r0, [r5, #0xc]
	movs r4, #0x98
	lsls r4, r4, #1
	adds r1, r0, r4
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	adds r1, r7, #0
	bl sub_803AD80
	movs r3, #0xf0
	subs r3, r3, r0
	asrs r3, r3, #1
	ldr r0, [r5, #0xc]
	lsls r1, r6, #2
	adds r1, r1, r6
	lsls r1, r1, #1
	adds r1, #0x80
	movs r5, #0x88
	lsls r5, r5, #1
	adds r2, r0, r5
	str r3, [r2]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r2, r0, r3
	str r1, [r2]
	adds r4, r0, r4
	ldr r2, [r4]
	movs r5, #0x20
	ldrsh r1, [r2, r5]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	adds r1, r7, #0
	bl sub_803AD80
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8036068
sub_8036068: @ 0x08036068
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r5, _080360BC @ =gUnknown_03001300
	ldr r0, [r5]
	bl sub_8006A90
	adds r0, r4, #0
	bl sub_80358A8
	ldrb r0, [r4, #8]
	cmp r0, #0
	beq _080360B0
	movs r0, #0x1a
	bl sub_8026F38
	adds r1, r0, #0
	adds r0, r4, #0
	movs r2, #0
	bl sub_8035FEC
	movs r0, #0x1b
	bl sub_8026F38
	adds r1, r0, #0
	adds r0, r4, #0
	movs r2, #1
	bl sub_8035FEC
	movs r0, #0x3b
	bl sub_8026F38
	adds r1, r0, #0
	adds r0, r4, #0
	movs r2, #2
	bl sub_8035FEC
_080360B0:
	ldr r0, [r5]
	bl sub_8006A48
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080360BC: .4byte gUnknown_03001300
	thumb_func_start sub_80360C0
sub_80360C0: @ 0x080360C0
	movs	r2, #132	@ 0x84
	lsls	r2, r2, #2
	adds	r0, r0, r2
	ldr	r2, [r0, #0]
	eors	r2, r1
	lsls	r3, r2, #1
	lsrs	r2, r2, #31
	orrs	r3, r2
	lsls	r1, r3, #6
	adds	r1, r1, r3
	lsls	r1, r1, #3
	adds	r1, r1, r3
	str	r1, [r0, #0]
	bx	lr

	thumb_func_start sub_80360DC
sub_80360DC: @ 0x080360DC
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r3, r0, #0
	movs r6, #0
	ldr r0, _08036150 @ =gStaticData_0817CFA4
	mov r8, r0
	movs r1, #0xf2
	lsls r1, r1, #1
	adds r1, r1, r3
	mov sb, r1
	mov r7, r8
	adds r5, r3, #0
	movs r4, #0
_080360FA:
	movs r0, #0
	mov ip, r0
	mov r1, ip
	strb r1, [r5, #0x10]
	adds r2, r3, #0
	adds r2, #0x14
	adds r2, r2, r4
	lsls r0, r6, #3
	mov r1, r8
	adds r1, #4
	adds r0, r0, r1
	ldr r0, [r0]
	adds r0, #1
	str r0, [r2]
	adds r0, r3, #0
	adds r0, #0x40
	adds r0, r0, r4
	ldr r1, [r7]
	str r1, [r0]
	movs r0, #1
	rsbs r0, r0, #0
	mov r1, sb
	adds r1, #4
	mov sb, r1
	subs r1, #4
	stm r1!, {r0}
	adds r7, #8
	adds r5, #0x34
	adds r4, #0x34
	adds r6, #1
	cmp r6, #8
	ble _080360FA
	movs r1, #0x83
	lsls r1, r1, #2
	adds r0, r3, r1
	mov r1, ip
	str r1, [r0]
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08036150: .4byte gStaticData_0817CFA4

	thumb_func_start sub_8036154
sub_8036154: @ 0x08036154
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	movs r1, #0x82
	lsls r1, r1, #2
	adds r0, r4, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _0803616C
	movs r1, #3
	bl sub_80346FC
_0803616C:
	ldr r1, _080361A8 @ =gUnknown_03001288
	movs r0, #0
	strh r0, [r1]
	bl sub_8001614
	movs r2, #0
	movs r1, #0xa0
	lsls r1, r1, #0x13
	movs r0, #0xff
_0803617E:
	strh r2, [r1]
	adds r1, #2
	subs r0, #1
	cmp r0, #0
	bge _0803617E
	ldr r1, _080361AC @ =0x04000050
	movs r0, #0xff
	strh r0, [r1]
	adds r1, #4
	movs r0, #0x10
	strh r0, [r1]
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _080361A2
	adds r0, r4, #0
	bl sub_8026ED0
_080361A2:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080361A8: .4byte gUnknown_03001288
_080361AC: .4byte 0x04000050

	thumb_func_start sub_80361B0
sub_80361B0: @ 0x080361B0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	adds r6, r0, #0
	ldr r0, _08036220 @ =0x06010000
	bl InitObjTileFreeList
	bl sub_8028EF0
	bl sub_80290BC
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _08036224 @ =gStaticData_0817D698
	bl sub_8036E20
	mov r8, r0
	ldr r1, _08036228 @ =0x040000D4
	ldr r0, _0803622C @ =gStaticData_08178F80
	str r0, [r1]
	ldr r0, _08036230 @ =0x05000200
	str r0, [r1, #4]
	ldr r0, _08036234 @ =0x80000100
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	adds r0, r6, #0
	bl sub_8036528
	adds r0, r6, #0
	bl sub_8036600
	movs r0, #0x14
	bl sub_8026EDC
	bl sub_8034374
	mov sb, r0
	adds r0, r6, #0
	bl sub_8036CF4
	movs r4, #0
	ldr r5, _08036238 @ =0x04000050
	ldr r7, _0803623C @ =0x04000054
_08036210:
	cmp r4, #0x10
	bgt _08036240
	movs r0, #0xff
	strh r0, [r5]
	movs r0, #0x10
	subs r0, r0, r4
	strh r0, [r7]
	b _08036244
	.align 2, 0
_08036220: .4byte 0x06010000
_08036224: .4byte gStaticData_0817D698
_08036228: .4byte 0x040000D4
_0803622C: .4byte gStaticData_08178F80
_08036230: .4byte 0x05000200
_08036234: .4byte 0x80000100
_08036238: .4byte 0x04000050
_0803623C: .4byte 0x04000054
_08036240:
	movs r0, #0
	str r0, [r5]
_08036244:
	bl sub_80006A8
	mov r0, sb
	bl sub_8034688
	adds r4, #1
	cmp r4, #0x3b
	ble _08036210
	ldr r0, _08036308 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x4b
	bl PlaySfx
	movs r5, #0x80
	lsls r5, r5, #6
	ldr r0, _0803630C @ =0x00000444
	adds r1, r6, r0
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r1]
_08036270:
	ldr r0, _08036310 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r1, _08036314 @ =gUnknown_030007E0
	movs r0, #9
	ldrh r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _08036292
	ldr r2, _0803630C @ =0x00000444
	adds r1, r6, r2
	ldr r0, [r1]
	cmp r0, #0x40
	ble _08036292
	movs r0, #0x40
	str r0, [r1]
_08036292:
	bl sub_80006A8
	bl sub_8001614
	ldr r3, _0803630C @ =0x00000444
	adds r4, r6, r3
	ldr r0, [r4]
	movs r7, #1
	rsbs r7, r7, #0
	mov sl, r7
	cmp r0, sl
	beq _080362E0
	cmp r0, #0x40
	bne _080362BC
	ldr r0, _08036308 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x4c
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
_080362BC:
	ldr r3, [r4]
	cmp r3, #0x40
	bgt _080362D8
	asrs r2, r3, #2
	ldr r1, _08036318 @ =0x04000050
	ldr r7, _0803631C @ =0x00003F7F
	adds r0, r7, #0
	strh r0, [r1]
	adds r1, #2
	movs r0, #0x10
	subs r0, r0, r2
	lsls r0, r0, #8
	orrs r2, r0
	strh r2, [r1]
_080362D8:
	subs r2, r3, #1
	str r2, [r4]
	cmp r2, sl
	bne _08036324
_080362E0:
	ldr r0, _08036320 @ =0x0000FFFF
	cmp r5, r0
	bgt _080362F0
	movs r1, #0xc0
	lsls r1, r1, #3
	adds r5, r5, r1
	cmp r5, r0
	ble _08036334
_080362F0:
	movs r5, #0x80
	lsls r5, r5, #9
	ldr r3, _0803630C @ =0x00000444
	adds r2, r6, r3
	ldr r1, [r2]
	movs r0, #1
	rsbs r0, r0, #0
	cmp r1, r0
	bne _08036334
	movs r0, #0xf4
	str r0, [r2]
	b _08036334
	.align 2, 0
_08036308: .4byte gUnknown_030012BC
_0803630C: .4byte 0x00000444
_08036310: .4byte gUnknown_03001304
_08036314: .4byte gUnknown_030007E0
_08036318: .4byte 0x04000050
_0803631C: .4byte 0x00003F7F
_08036320: .4byte 0x0000FFFF
_08036324:
	cmp r2, #0x40
	bgt _08036334
	lsls r0, r5, #3
	adds r0, r0, r5
	lsls r0, r0, #2
	subs r0, r0, r5
	lsls r0, r0, #3
	asrs r5, r0, #8
_08036334:
	movs r0, #0x80
	lsls r0, r0, #0x11
	adds r1, r5, #0
	bl sub_803ADB4
	lsls r3, r0, #4
	subs r3, r3, r0
	lsls r3, r3, #3
	rsbs r3, r3, #0
	movs r7, #0xf0
	lsls r7, r7, #7
	adds r3, r3, r7
	lsls r1, r0, #2
	adds r1, r1, r0
	lsls r1, r1, #4
	rsbs r1, r1, #0
	movs r2, #0xa0
	lsls r2, r2, #7
	adds r1, r1, r2
	ldr r2, _08036458 @ =0x04000028
	str r3, [r2]
	adds r2, #4
	str r1, [r2]
	ldr r1, _0803645C @ =0x04000020
	strh r0, [r1]
	adds r1, #6
	strh r0, [r1]
	ldr r0, _08036460 @ =0x04000022
	movs r1, #0
	strh r1, [r0]
	adds r0, #2
	strh r1, [r0]
	mov r0, sb
	bl sub_8034688
	ldr r3, _08036464 @ =0x00000444
	adds r0, r6, r3
	ldr r4, [r0]
	cmp r4, #0
	beq _08036386
	b _08036270
_08036386:
	ldr r2, _08036468 @ =gUnknown_03001288
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r7, [r2, #1]
	ands r0, r7
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r2, #1]
	movs r0, #0x40
	ldrb r1, [r2]
	orrs r0, r1
	strb r0, [r2]
	bl sub_8001614
	ldr r0, _0803646C @ =0x04000050
	str r4, [r0]
	ldr r3, _08036464 @ =0x00000444
	adds r2, r6, r3
	movs r1, #1
	rsbs r1, r1, #0
	str r1, [r2]
	movs r7, #0x89
	lsls r7, r7, #3
	adds r0, r6, r7
	str r1, [r0]
	ldr r0, [r2]
	cmp r0, #0
	beq _080364B4
_080363BE:
	ldr r0, _08036470 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_80007AC
	ldr r1, _08036474 @ =gUnknown_030007E0
	movs r0, #9
	ldrh r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _080363E2
	movs r0, #0x89
	lsls r0, r0, #3
	adds r1, r6, r0
	ldr r0, [r1]
	cmp r0, #0
	ble _080363E2
	movs r0, #1
	str r0, [r1]
_080363E2:
	mov r2, r8
	ldr r1, [r2, #0x50]
	movs r3, #0x10
	ldrsh r0, [r1, r3]
	add r0, r8
	ldr r1, [r1, #0x14]
	bl sub_803AD7C
	mov r7, r8
	ldr r1, [r7, #0x50]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	add r0, r8
	ldr r1, [r1, #0x1c]
	bl sub_803AD7C
	ldr r0, _08036478 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006A78
	bl sub_8028EA8
	adds r0, r6, #0
	bl sub_8036668
	adds r0, r6, #0
	bl sub_803686C
	mov r0, sb
	bl sub_8034688
	bl sub_80006A8
	ldr r3, _08036464 @ =0x00000444
	adds r4, r6, r3
	ldr r1, [r4]
	cmp r1, #0x10
	ble _08036484
	subs r3, r1, #1
	str r3, [r4]
	subs r1, #0x12
	ldr r5, _0803646C @ =0x04000050
	ldr r7, _0803647C @ =0x00003F7F
	adds r0, r7, #0
	strh r0, [r5]
	ldr r2, _08036480 @ =0x04000052
	movs r0, #0x10
	subs r0, r0, r1
	lsls r1, r1, #8
	orrs r0, r1
	strh r0, [r2]
	cmp r3, #0x11
	bne _0803649A
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r4]
	movs r0, #0
	str r0, [r5]
	b _0803649A
	.align 2, 0
_08036458: .4byte 0x04000028
_0803645C: .4byte 0x04000020
_08036460: .4byte 0x04000022
_08036464: .4byte 0x00000444
_08036468: .4byte gUnknown_03001288
_0803646C: .4byte 0x04000050
_08036470: .4byte gUnknown_03001304
_08036474: .4byte gUnknown_030007E0
_08036478: .4byte gUnknown_03001300
_0803647C: .4byte 0x00003F7F
_08036480: .4byte 0x04000052
_08036484:
	cmp r1, #0
	blt _0803649A
	subs r1, #1
	str r1, [r4]
	ldr r2, _08036514 @ =0x04000054
	movs r0, #0x10
	subs r0, r0, r1
	strh r0, [r2]
	ldr r1, _08036518 @ =0x04000050
	movs r0, #0xff
	strh r0, [r1]
_0803649A:
	ldr r0, _0803651C @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006AAC
	bl FlushVramDmaQueue
	bl sub_8029090
	ldr r1, _08036520 @ =0x00000444
	adds r0, r6, r1
	ldr r0, [r0]
	cmp r0, #0
	bne _080363BE
_080364B4:
	mov r2, r8
	cmp r2, #0
	beq _080364CA
	ldr r1, [r2, #0x50]
	movs r3, #8
	ldrsh r0, [r1, r3]
	add r0, r8
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_080364CA:
	mov r7, sb
	cmp r7, #0
	beq _080364D8
	mov r0, sb
	movs r1, #3
	bl sub_80346FC
_080364D8:
	ldr r1, _08036524 @ =0x00000434
	adds r0, r6, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _080364E6
	bl sub_8026EB4
_080364E6:
	movs r2, #0x86
	lsls r2, r2, #3
	adds r0, r6, r2
	ldr r0, [r0]
	cmp r0, #0
	beq _080364F6
	bl sub_8026EB4
_080364F6:
	bl sub_802907C
	bl sub_8028E88
	bl sub_8028DB8
	bl sub_8029168
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08036514: .4byte 0x04000054
_08036518: .4byte 0x04000050
_0803651C: .4byte gUnknown_03001300
_08036520: .4byte 0x00000444
_08036524: .4byte 0x00000434

	thumb_func_start sub_8036528
sub_8036528: @ 0x08036528
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	adds r4, r0, #0
	movs r0, #0x90
	lsls r0, r0, #5
	bl sub_8028CD4
	ldr r1, _080365DC @ =0x00000424
	adds r1, r1, r4
	mov sb, r1
	str r0, [r1]
	movs r0, #0x80
	lsls r0, r0, #3
	bl sub_8028CD4
	movs r2, #0x85
	lsls r2, r2, #3
	adds r2, r2, r4
	mov sl, r2
	str r0, [r2]
	movs r7, #0x80
	lsls r7, r7, #5
	adds r0, r7, #0
	bl sub_8028CD4
	ldr r2, _080365E0 @ =0x0000042C
	adds r1, r4, r2
	str r0, [r1]
	ldr r0, _080365E4 @ =gStaticData_0817D768
	mov r8, r0
	ldr r1, [r0, #8]
	ldr r2, _080365E8 @ =0x050003E0
	adds r0, r4, #0
	bl sub_8037110
	ldr r6, _080365EC @ =gStaticData_0817D77C
	ldr r1, [r6, #8]
	ldr r2, _080365F0 @ =0x050003C0
	adds r0, r4, #0
	bl sub_8037110
	ldr r5, _080365F4 @ =gStaticData_0817D790
	ldr r1, [r5, #8]
	ldr r2, _080365F8 @ =0x050003A0
	adds r0, r4, #0
	bl sub_8037110
	ldr r1, [r6, #0xc]
	mov r0, sb
	ldr r2, [r0]
	adds r0, r4, #0
	bl sub_8037110
	ldr r1, [r5, #0xc]
	mov r0, sl
	ldr r2, [r0]
	adds r0, r4, #0
	bl sub_8037110
	mov r1, r8
	ldr r0, [r1, #0xc]
	ldr r0, [r0]
	lsrs r0, r0, #8
	movs r2, #0x86
	lsls r2, r2, #3
	adds r5, r4, r2
	bl sub_8026EC0
	adds r1, r0, #0
	str r1, [r5]
	mov r2, r8
	ldr r0, [r2, #0xc]
	bl LoadTaggedAsset
	ldr r0, _080365FC @ =0x00000434
	adds r4, r4, r0
	adds r0, r7, #0
	bl sub_8026EC0
	str r0, [r4]
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080365DC: .4byte 0x00000424
_080365E0: .4byte 0x0000042C
_080365E4: .4byte gStaticData_0817D768
_080365E8: .4byte 0x050003E0
_080365EC: .4byte gStaticData_0817D77C
_080365F0: .4byte 0x050003C0
_080365F4: .4byte gStaticData_0817D790
_080365F8: .4byte 0x050003A0
_080365FC: .4byte 0x00000434

	thumb_func_start sub_8036600
sub_8036600: @ 0x08036600
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	movs r6, #0
	movs r7, #0
	ldr r3, _0803665C @ =gStaticData_0817D6C0
	adds r2, r5, #0
	adds r4, r3, #4
	adds r1, r5, #4
_08036610:
	strb r7, [r2]
	ldr r0, [r4]
	adds r0, #1
	str r0, [r1]
	ldr r0, [r3]
	str r0, [r1, #0x2c]
	adds r3, #8
	adds r2, #0x34
	adds r1, #0x34
	adds r4, #8
	adds r6, #1
	cmp r6, #0x13
	ble _08036610
	movs r2, #1
	movs r1, #0x11
	ldr r3, _08036660 @ =0x00000421
	adds r0, r5, r3
_08036632:
	strb r2, [r0]
	subs r0, #1
	subs r1, #1
	cmp r1, #0
	bge _08036632
	movs r1, #0x87
	lsls r1, r1, #3
	adds r0, r5, r1
	movs r1, #0
	str r1, [r0]
	ldr r2, _08036664 @ =0x0000043C
	adds r0, r5, r2
	str r1, [r0]
	movs r3, #0x88
	lsls r3, r3, #3
	adds r0, r5, r3
	str r1, [r0]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0803665C: .4byte gStaticData_0817D6C0
_08036660: .4byte 0x00000421
_08036664: .4byte 0x0000043C

	thumb_func_start sub_8036668
sub_8036668: @ 0x08036668
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	movs r0, #0x89
	lsls r0, r0, #3
	adds r2, r4, r0
	ldr r1, [r2]
	movs r0, #1
	rsbs r0, r0, #0
	cmp r1, r0
	beq _0803667E
	b _080367DC
_0803667E:
	movs r6, #0
	adds r1, r4, #4
	mov ip, r1
	adds r7, r2, #0
_08036686:
	movs r0, #0x34
	adds r3, r6, #0
	muls r3, r0, r3
	mov r0, ip
	adds r5, r0, r3
	ldr r0, [r5]
	cmp r0, #0
	bne _08036698
	b _08036798
_08036698:
	subs r0, #1
	str r0, [r5]
	cmp r0, #0
	bne _08036732
	adds r1, r4, #0
	adds r1, #0x30
	adds r1, r1, r3
	ldr r0, [r1]
	adds r2, r0, #0
	adds r0, #0x20
	str r0, [r1]
	adds r1, r4, r3
	movs r0, #1
	strb r0, [r1]
	movs r1, #0
	ldrsh r0, [r2, r1]
	str r0, [r5]
	cmp r0, #0
	beq _0803679E
	adds r0, r4, #0
	adds r0, #0x10
	adds r0, r0, r3
	ldrh r5, [r2, #6]
	lsls r1, r5, #0x10
	str r1, [r0]
	adds r0, r4, #0
	adds r0, #0x24
	adds r0, r0, r3
	ldr r1, [r2, #0x14]
	str r1, [r0]
	adds r1, r4, #0
	adds r1, #0x14
	adds r1, r1, r3
	movs r5, #8
	ldrsh r0, [r2, r5]
	lsls r0, r0, #8
	str r0, [r1]
	adds r0, r4, #0
	adds r0, #0x28
	adds r0, r0, r3
	ldr r1, [r2, #0x18]
	str r1, [r0]
	adds r1, r4, #0
	adds r1, #0x18
	adds r1, r1, r3
	movs r5, #0xa
	ldrsh r0, [r2, r5]
	lsls r0, r0, #8
	str r0, [r1]
	adds r0, r4, #0
	adds r0, #0x2c
	adds r0, r0, r3
	ldr r1, [r2, #0x1c]
	str r1, [r0]
	adds r0, r4, #0
	adds r0, #8
	adds r0, r0, r3
	ldrh r5, [r2, #2]
	lsls r1, r5, #0x10
	str r1, [r0]
	adds r0, r4, #0
	adds r0, #0x1c
	adds r0, r0, r3
	ldr r1, [r2, #0xc]
	str r1, [r0]
	adds r0, r4, #0
	adds r0, #0xc
	adds r0, r0, r3
	ldrh r5, [r2, #4]
	lsls r1, r5, #0x10
	str r1, [r0]
	adds r0, r4, #0
	adds r0, #0x20
	adds r0, r0, r3
	ldr r1, [r2, #0x10]
	str r1, [r0]
	b _0803679E
_08036732:
	adds r2, r4, #0
	adds r2, #0x10
	adds r2, r2, r3
	adds r0, r4, #0
	adds r0, #0x24
	adds r0, r0, r3
	ldr r1, [r2]
	ldr r0, [r0]
	adds r1, r1, r0
	str r1, [r2]
	adds r2, r4, #0
	adds r2, #0x14
	adds r2, r2, r3
	adds r0, r4, #0
	adds r0, #0x28
	adds r0, r0, r3
	ldr r1, [r2]
	ldr r0, [r0]
	adds r1, r1, r0
	str r1, [r2]
	adds r2, r4, #0
	adds r2, #0x18
	adds r2, r2, r3
	adds r0, r4, #0
	adds r0, #0x2c
	adds r0, r0, r3
	ldr r1, [r2]
	ldr r0, [r0]
	adds r1, r1, r0
	str r1, [r2]
	adds r2, r4, #0
	adds r2, #8
	adds r2, r2, r3
	adds r0, r4, #0
	adds r0, #0x1c
	adds r0, r0, r3
	ldr r1, [r2]
	ldr r0, [r0]
	adds r1, r1, r0
	str r1, [r2]
	adds r2, r4, #0
	adds r2, #0xc
	adds r2, r2, r3
	adds r0, r4, #0
	adds r0, #0x20
	adds r0, r0, r3
	ldr r1, [r2]
	ldr r0, [r0]
	adds r1, r1, r0
	str r1, [r2]
	b _0803679E
_08036798:
	movs r0, #2
	rsbs r0, r0, #0
	str r0, [r7]
_0803679E:
	adds r6, #1
	cmp r6, #0x13
	bgt _080367A6
	b _08036686
_080367A6:
	ldr r0, _08036858 @ =0x0000043C
	adds r2, r4, r0
	ldr r0, [r2]
	cmp r0, #1
	bgt _080367DC
	movs r5, #0x88
	lsls r5, r5, #3
	adds r1, r4, r5
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
	cmp r0, #3
	ble _080367DC
	movs r3, #0
	str r3, [r1]
	movs r0, #0x87
	lsls r0, r0, #3
	adds r1, r4, r0
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
	cmp r0, #9
	ble _080367DC
	str r3, [r1]
	ldr r0, [r2]
	adds r0, #1
	str r0, [r2]
_080367DC:
	movs r1, #0x89
	lsls r1, r1, #3
	adds r5, r4, r1
	ldr r1, [r5]
	movs r0, #2
	rsbs r0, r0, #0
	cmp r1, r0
	bne _080367F0
	movs r0, #0xf0
	str r0, [r5]
_080367F0:
	ldr r0, [r5]
	cmp r0, #0
	ble _0803680C
	subs r0, #1
	str r0, [r5]
	cmp r0, #0
	bne _08036850
	ldr r0, _0803685C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x50
	bl PlaySfx
_0803680C:
	ldr r0, [r5]
	cmp r0, #0
	bne _08036850
	movs r2, #1
	adds r1, r4, #0
	movs r5, #0xf7
	lsls r5, r5, #2
	adds r3, r4, r5
	ldr r6, _08036860 @ =0xFFF80000
	ldr r5, _08036864 @ =0xFF810000
_08036820:
	ldrb r0, [r1]
	cmp r0, #0
	beq _08036834
	movs r2, #0
	ldr r0, [r1, #8]
	adds r0, r0, r6
	str r0, [r1, #8]
	cmp r0, r5
	bge _08036834
	strb r2, [r1]
_08036834:
	adds r1, #0x34
	cmp r1, r3
	ble _08036820
	cmp r2, #0
	beq _08036850
	ldr r0, _08036868 @ =0x00000444
	adds r1, r4, r0
	movs r0, #0x10
	str r0, [r1]
	movs r5, #0x89
	lsls r5, r5, #3
	adds r1, r4, r5
	subs r0, #0x13
	str r0, [r1]
_08036850:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08036858: .4byte 0x0000043C
_0803685C: .4byte gUnknown_030012BC
_08036860: .4byte 0xFFF80000
_08036864: .4byte 0xFF810000
_08036868: .4byte 0x00000444

	thumb_func_start sub_803686C
sub_803686C: @ 0x0803686C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x28
	mov sb, r0
	movs r0, #1
	str r0, [sp, #0x20]
	movs r5, #0xf7
	lsls r5, r5, #2
	add r5, sb
	ldrb r0, [r5]
	cmp r0, #0
	beq _08036938
	mov r1, sp
	movs r0, #0
	strh r0, [r1]
	ldr r1, _08036BFC @ =0x040000D4
	mov r2, sp
	str r2, [r1]
	add r3, sp, #8
	str r3, [r1, #4]
	ldr r0, _08036C00 @ =0x81000004
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	movs r0, #0x85
	lsls r0, r0, #3
	add r0, sb
	ldr r4, [r0]
	movs r0, #0xf
	ldrb r7, [r3, #5]
	ands r0, r7
	movs r1, #0xd0
	orrs r0, r1
	strb r0, [r3, #5]
	ldrb r2, [r3, #3]
	movs r1, #0x3f
	adds r0, r1, #0
	ands r0, r2
	movs r2, #0x80
	orrs r0, r2
	strb r0, [r3, #3]
	ldrb r0, [r3, #1]
	ands r1, r0
	movs r0, #0x40
	orrs r1, r0
	strb r1, [r3, #1]
	movs r1, #0xe
	ldrsh r0, [r5, r1]
	strb r0, [r3]
	movs r2, #0xa
	ldrsh r1, [r5, r2]
	ldr r7, _08036C04 @ =0x000001FF
	adds r0, r7, #0
	ands r1, r0
	ldrh r2, [r3, #2]
	ldr r0, _08036C08 @ =0xFFFFFE00
	ands r0, r2
	orrs r0, r1
	strh r0, [r3, #2]
	lsls r4, r4, #0x11
	lsrs r4, r4, #0x16
	ldr r0, _08036C0C @ =0xFFFFFC00
	ldrh r1, [r3, #4]
	ands r0, r1
	orrs r0, r4
	strh r0, [r3, #4]
	adds r4, r3, #0
	movs r5, #3
_080368F8:
	ldr r0, _08036C10 @ =gUnknown_03001300
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8006AC8
	ldrh r2, [r4, #4]
	lsls r0, r2, #0x16
	lsrs r0, r0, #0x16
	adds r0, #8
	ldr r3, _08036C14 @ =0x000003FF
	adds r1, r3, #0
	ands r0, r1
	ldr r7, _08036C0C @ =0xFFFFFC00
	adds r1, r7, #0
	ands r2, r1
	orrs r2, r0
	strh r2, [r4, #4]
	ldrh r2, [r4, #2]
	lsls r0, r2, #0x17
	lsrs r0, r0, #0x17
	adds r0, #0x20
	ldr r3, _08036C04 @ =0x000001FF
	adds r1, r3, #0
	ands r0, r1
	ldr r7, _08036C08 @ =0xFFFFFE00
	adds r1, r7, #0
	ands r2, r1
	orrs r2, r0
	strh r2, [r4, #2]
	subs r5, #1
	cmp r5, #0
	bge _080368F8
_08036938:
	mov r7, sb
	adds r7, #0x34
	ldr r0, _08036C18 @ =0x00000424
	add r0, sb
	ldr r0, [r0]
	ldr r1, _08036C1C @ =0xF9FF0000
	adds r0, r0, r1
	lsrs r0, r0, #5
	mov r8, r0
	movs r2, #0
	mov sl, r2
_0803694E:
	ldrb r0, [r7]
	cmp r0, #0
	bne _08036956
	b _08036A70
_08036956:
	mov r1, sp
	movs r0, #0
	strh r0, [r1]
	ldr r1, _08036BFC @ =0x040000D4
	mov r3, sp
	str r3, [r1]
	add r5, sp, #0x10
	str r5, [r1, #4]
	ldr r0, _08036C00 @ =0x81000004
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r1, [r7, #0x14]
	movs r4, #0x80
	lsls r4, r4, #0x11
	adds r0, r4, #0
	bl sub_803ADB4
	adds r6, r0, #0
	ldr r1, [r7, #0x18]
	adds r0, r4, #0
	bl sub_803ADB4
	adds r4, r0, #0
	movs r1, #0
	movs r0, #0x80
	lsls r0, r0, #1
	cmp r6, r0
	bne _08036992
	cmp r4, r6
	beq _08036994
_08036992:
	movs r1, #1
_08036994:
	cmp r1, #0
	beq _08036A0E
	movs r0, #0x82
	lsls r0, r0, #3
	add r0, sb
	mov r2, sl
	adds r1, r0, r2
	ldrb r0, [r1]
	cmp r0, #0
	beq _080369BA
	movs r3, #0
	strb r3, [r1]
	ldr r0, _08036C20 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x4e
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
_080369BA:
	ldr r0, _08036C10 @ =gUnknown_03001300
	ldr r2, [r0]
	ldr r0, [sp, #0x20]
	lsls r1, r0, #2
	lsls r0, r0, #5
	adds r0, r2, r0
	strh r6, [r0, #0x12]
	adds r0, r1, #3
	lsls r0, r0, #3
	adds r0, r2, r0
	strh r4, [r0, #0x12]
	adds r0, r1, #1
	lsls r0, r0, #3
	adds r0, r2, r0
	movs r3, #0
	strh r3, [r0, #0x12]
	adds r1, #2
	lsls r1, r1, #3
	adds r2, r2, r1
	strh r3, [r2, #0x12]
	ldrb r1, [r5, #1]
	movs r2, #4
	rsbs r2, r2, #0
	adds r0, r2, #0
	ands r1, r0
	movs r0, #1
	orrs r1, r0
	strb r1, [r5, #1]
	movs r0, #7
	ldr r2, [sp, #0x20]
	ands r2, r0
	lsls r2, r2, #1
	ldrb r0, [r5, #3]
	movs r3, #0xf
	rsbs r3, r3, #0
	adds r1, r3, #0
	ands r0, r1
	orrs r0, r2
	strb r0, [r5, #3]
	ldr r0, [sp, #0x20]
	adds r0, #1
	str r0, [sp, #0x20]
_08036A0E:
	movs r0, #0xf
	ldrb r1, [r5, #5]
	ands r0, r1
	movs r1, #0xe0
	orrs r0, r1
	strb r0, [r5, #5]
	ldrb r2, [r5, #3]
	movs r1, #0x3f
	adds r0, r1, #0
	ands r0, r2
	movs r2, #0x80
	orrs r0, r2
	strb r0, [r5, #3]
	ldrb r0, [r5, #1]
	ands r1, r0
	orrs r1, r2
	strb r1, [r5, #1]
	movs r2, #0xe
	ldrsh r0, [r7, r2]
	subs r0, #0x10
	add r1, sp, #0x10
	strb r0, [r1]
	movs r3, #0xa
	ldrsh r2, [r7, r3]
	subs r2, #8
	ldr r1, _08036C04 @ =0x000001FF
	adds r0, r1, #0
	ands r2, r0
	ldrh r0, [r5, #2]
	ldr r3, _08036C08 @ =0xFFFFFE00
	adds r1, r3, #0
	ands r0, r1
	orrs r0, r2
	strh r0, [r5, #2]
	ldr r1, _08036C14 @ =0x000003FF
	adds r0, r1, #0
	mov r1, r8
	ands r1, r0
	ldr r2, _08036C0C @ =0xFFFFFC00
	adds r0, r2, #0
	ldrh r3, [r5, #4]
	ands r0, r3
	orrs r0, r1
	strh r0, [r5, #4]
	ldr r0, _08036C10 @ =gUnknown_03001300
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8006AC8
_08036A70:
	movs r0, #8
	add r8, r0
	adds r7, #0x34
	movs r1, #1
	add sl, r1
	mov r2, sl
	cmp r2, #0x11
	bgt _08036A82
	b _0803694E
_08036A82:
	mov r3, sb
	ldrb r0, [r3]
	cmp r0, #0
	bne _08036A8C
	b _08036CD0
_08036A8C:
	movs r4, #0x82
	lsls r4, r4, #3
	add r4, sb
	ldrb r0, [r4]
	cmp r0, #0
	beq _08036AAA
	ldr r0, _08036C20 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x4d
	bl PlaySfx
	movs r0, #0
	strb r0, [r4]
_08036AAA:
	mov r8, sb
	ldr r0, _08036C24 @ =0x0000042C
	add r0, r8
	ldr r0, [r0]
	ldr r7, _08036C1C @ =0xF9FF0000
	adds r0, r0, r7
	lsrs r0, r0, #5
	str r0, [sp, #0x24]
	movs r0, #0
	str r0, [sp, #4]
	ldr r2, _08036BFC @ =0x040000D4
	add r0, sp, #4
	str r0, [r2]
	ldr r0, _08036C28 @ =0x00000434
	add r0, r8
	ldr r4, [r0]
	str r4, [r2, #4]
	ldr r0, _08036C2C @ =0x85000400
	str r0, [r2, #8]
	ldr r0, [r2, #8]
	movs r3, #0x86
	lsls r3, r3, #3
	add r3, r8
	movs r0, #0x87
	lsls r0, r0, #3
	add r0, r8
	ldr r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #9
	ldr r1, [r3]
	adds r1, r1, r0
	add r5, sp, #0x18
	ldr r0, _08036C30 @ =0x80000050
	mov ip, r0
	adds r3, r4, #0
	adds r3, #0x60
	movs r7, #0x80
	lsls r7, r7, #4
	mov sl, r7
	movs r6, #7
_08036AFC:
	str r1, [r2]
	str r3, [r2, #4]
	mov r0, ip
	str r0, [r2, #8]
	ldr r0, [r2, #8]
	adds r1, #0xa0
	str r1, [r2]
	mov r7, sl
	adds r0, r4, r7
	str r0, [r2, #4]
	mov r0, ip
	str r0, [r2, #8]
	ldr r0, [r2, #8]
	adds r1, #0xa0
	movs r7, #0x80
	lsls r7, r7, #1
	adds r4, r4, r7
	adds r3, r3, r7
	subs r6, #1
	cmp r6, #0
	bge _08036AFC
	ldr r0, _08036C28 @ =0x00000434
	add r0, sb
	ldr r0, [r0]
	ldr r1, _08036C24 @ =0x0000042C
	add r1, sb
	ldr r1, [r1]
	movs r2, #0x80
	lsls r2, r2, #5
	movs r3, #0x10
	bl QueueVramDmaTransfer
	mov r1, sp
	movs r0, #0
	strh r0, [r1]
	ldr r0, _08036BFC @ =0x040000D4
	str r1, [r0]
	str r5, [r0, #4]
	ldr r1, _08036C00 @ =0x81000004
	str r1, [r0, #8]
	ldr r0, [r0, #8]
	mov r0, r8
	ldr r1, [r0, #0x14]
	movs r4, #0x80
	lsls r4, r4, #0x11
	adds r0, r4, #0
	bl sub_803ADB4
	adds r6, r0, #0
	mov r2, r8
	ldr r1, [r2, #0x18]
	adds r0, r4, #0
	bl sub_803ADB4
	adds r4, r0, #0
	movs r1, #0
	adds r0, r7, #0
	cmp r6, r0
	bne _08036B76
	cmp r4, r6
	beq _08036B78
_08036B76:
	movs r1, #1
_08036B78:
	adds r7, r1, #0
	cmp r7, #0
	beq _08036BC2
	ldr r0, _08036C10 @ =gUnknown_03001300
	ldr r2, [r0]
	ldr r3, [sp, #0x20]
	lsls r1, r3, #2
	lsls r0, r3, #5
	adds r0, r2, r0
	movs r3, #0
	strh r6, [r0, #0x12]
	adds r0, r1, #3
	lsls r0, r0, #3
	adds r0, r2, r0
	strh r4, [r0, #0x12]
	adds r0, r1, #1
	lsls r0, r0, #3
	adds r0, r2, r0
	strh r3, [r0, #0x12]
	adds r1, #2
	lsls r1, r1, #3
	adds r2, r2, r1
	strh r3, [r2, #0x12]
	ldrb r0, [r5, #1]
	movs r1, #3
	orrs r0, r1
	strb r0, [r5, #1]
	movs r0, #7
	ldr r1, [sp, #0x20]
	ands r1, r0
	lsls r2, r1, #1
	ldrb r1, [r5, #3]
	movs r0, #0xf
	rsbs r0, r0, #0
	ands r0, r1
	orrs r0, r2
	strb r0, [r5, #3]
_08036BC2:
	movs r0, #0xf0
	ldrb r2, [r5, #5]
	orrs r0, r2
	strb r0, [r5, #5]
	ldrb r0, [r5, #3]
	movs r1, #0xc0
	orrs r0, r1
	strb r0, [r5, #3]
	ldrb r1, [r5, #1]
	movs r0, #0x3f
	ands r0, r1
	strb r0, [r5, #1]
	cmp r7, #0
	beq _08036C34
	mov r3, r8
	movs r1, #0xe
	ldrsh r0, [r3, r1]
	subs r0, #0x40
	strb r0, [r5]
	movs r2, #0xa
	ldrsh r1, [r3, r2]
	ldr r0, [r3, #0x14]
	lsls r0, r0, #5
	asrs r0, r0, #0x10
	subs r1, r1, r0
	subs r1, #0x40
	ldr r3, _08036C04 @ =0x000001FF
	adds r0, r3, #0
	b _08036C48
	.align 2, 0
_08036BFC: .4byte 0x040000D4
_08036C00: .4byte 0x81000004
_08036C04: .4byte 0x000001FF
_08036C08: .4byte 0xFFFFFE00
_08036C0C: .4byte 0xFFFFFC00
_08036C10: .4byte gUnknown_03001300
_08036C14: .4byte 0x000003FF
_08036C18: .4byte 0x00000424
_08036C1C: .4byte 0xF9FF0000
_08036C20: .4byte gUnknown_030012BC
_08036C24: .4byte 0x0000042C
_08036C28: .4byte 0x00000434
_08036C2C: .4byte 0x85000400
_08036C30: .4byte 0x80000050
_08036C34:
	mov r1, r8
	movs r2, #0xe
	ldrsh r0, [r1, r2]
	subs r0, #0x20
	strb r0, [r5]
	movs r3, #0xa
	ldrsh r1, [r1, r3]
	subs r1, #0x40
	ldr r2, _08036C88 @ =0x000001FF
	adds r0, r2, #0
_08036C48:
	ands r1, r0
	ldrh r2, [r5, #2]
	ldr r0, _08036C8C @ =0xFFFFFE00
	ands r0, r2
	orrs r0, r1
	strh r0, [r5, #2]
	ldr r3, _08036C90 @ =0x000003FF
	adds r0, r3, #0
	ldr r1, [sp, #0x24]
	ands r1, r0
	ldr r0, _08036C94 @ =0xFFFFFC00
	ldrh r2, [r5, #4]
	ands r0, r2
	orrs r0, r1
	strh r0, [r5, #4]
	ldr r0, _08036C98 @ =gUnknown_03001300
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8006AC8
	cmp r7, #0
	beq _08036C9C
	mov r3, r8
	movs r7, #0xa
	ldrsh r0, [r3, r7]
	ldr r1, [r3, #0x14]
	lsls r1, r1, #5
	asrs r1, r1, #0x10
	adds r1, r1, r0
	subs r1, #0x40
	b _08036CA2
	.align 2, 0
_08036C88: .4byte 0x000001FF
_08036C8C: .4byte 0xFFFFFE00
_08036C90: .4byte 0x000003FF
_08036C94: .4byte 0xFFFFFC00
_08036C98: .4byte gUnknown_03001300
_08036C9C:
	mov r3, r8
	movs r7, #0xa
	ldrsh r1, [r3, r7]
_08036CA2:
	ldr r2, _08036CE0 @ =0x000001FF
	adds r0, r2, #0
	ands r1, r0
	ldrh r2, [r5, #2]
	ldr r0, _08036CE4 @ =0xFFFFFE00
	ands r0, r2
	orrs r0, r1
	strh r0, [r5, #2]
	ldr r1, [sp, #0x24]
	adds r1, #0x40
	ldr r3, _08036CE8 @ =0x000003FF
	adds r0, r3, #0
	ands r1, r0
	ldr r0, _08036CEC @ =0xFFFFFC00
	ldrh r7, [r5, #4]
	ands r0, r7
	orrs r0, r1
	strh r0, [r5, #4]
	ldr r0, _08036CF0 @ =gUnknown_03001300
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8006AC8
_08036CD0:
	add sp, #0x28
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08036CE0: .4byte 0x000001FF
_08036CE4: .4byte 0xFFFFFE00
_08036CE8: .4byte 0x000003FF
_08036CEC: .4byte 0xFFFFFC00
_08036CF0: .4byte gUnknown_03001300

	thumb_func_start sub_8036CF4
sub_8036CF4: @ 0x08036CF4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	ldr r0, _08036D8C @ =gStaticData_0817D7A4
	mov sb, r0
	movs r0, #0x80
	lsls r0, r0, #2
	bl sub_8026EC0
	adds r4, r0, #0
	mov r1, sb
	ldr r0, [r1, #8]
	adds r1, r4, #0
	bl LoadTaggedAsset
	ldr r1, _08036D90 @ =0x040000D4
	adds r0, r4, #2
	str r0, [r1]
	ldr r0, _08036D94 @ =0x05000002
	str r0, [r1, #4]
	ldr r0, _08036D98 @ =0x80000040
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	cmp r4, #0
	beq _08036D30
	adds r0, r4, #0
	bl sub_8026EB4
_08036D30:
	mov r2, sb
	ldr r0, [r2, #0xc]
	ldr r1, _08036D9C @ =0x06008000
	bl LoadTaggedAsset
	mov r0, sb
	ldr r1, [r0, #4]
	ldr r0, [r0]
	muls r0, r1, r0
	lsls r0, r0, #1
	bl sub_8026EC0
	mov sl, r0
	mov r1, sb
	ldr r0, [r1, #0x10]
	mov r1, sl
	bl LoadTaggedAsset
	ldr r4, _08036DA0 @ =0x0600F000
	movs r5, #0
	mov r2, sb
	ldr r2, [r2, #4]
	mov ip, r2
	movs r0, #0xff
	mov r8, r0
_08036D62:
	movs r3, #0
	adds r7, r5, #1
_08036D66:
	cmp r5, ip
	bge _08036DA4
	mov r1, sb
	ldr r0, [r1]
	cmp r3, r0
	bge _08036DA4
	muls r0, r5, r0
	adds r0, r0, r3
	lsls r0, r0, #1
	add r0, sl
	mov r2, r8
	ldrh r1, [r0]
	ands r2, r1
	mov r1, r8
	ldrh r0, [r0, #2]
	ands r1, r0
	lsls r1, r1, #8
	orrs r2, r1
	b _08036DA6
	.align 2, 0
_08036D8C: .4byte gStaticData_0817D7A4
_08036D90: .4byte 0x040000D4
_08036D94: .4byte 0x05000002
_08036D98: .4byte 0x80000040
_08036D9C: .4byte 0x06008000
_08036DA0: .4byte 0x0600F000
_08036DA4:
	movs r2, #0
_08036DA6:
	strh r2, [r4]
	adds r4, #2
	adds r3, #2
	cmp r3, #0x1f
	ble _08036D66
	adds r5, r7, #0
	cmp r5, #0x1f
	ble _08036D62
	ldr r0, _08036E10 @ =0xFFFF0000
	ands r6, r0
	movs r0, #8
	orrs r6, r0
	movs r0, #0xf0
	lsls r0, r0, #5
	orrs r6, r0
	movs r0, #0x80
	orrs r6, r0
	movs r0, #1
	orrs r6, r0
	ldr r0, _08036E14 @ =0xFFFF3FFF
	ands r6, r0
	movs r0, #0x80
	lsls r0, r0, #7
	orrs r6, r0
	ldr r0, _08036E18 @ =0x0400000C
	strh r6, [r0]
	movs r0, #4
	ldr r1, _08036E1C @ =gUnknown_03001288
	ldrb r1, [r1, #1]
	orrs r0, r1
	ldr r2, _08036E1C @ =gUnknown_03001288
	strb r0, [r2, #1]
	movs r0, #8
	rsbs r0, r0, #0
	ldrb r1, [r2]
	ands r0, r1
	movs r1, #1
	orrs r0, r1
	strb r0, [r2]
	mov r2, sl
	cmp r2, #0
	beq _08036E00
	mov r0, sl
	bl sub_8026EB4
_08036E00:
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08036E10: .4byte 0xFFFF0000
_08036E14: .4byte 0xFFFF3FFF
_08036E18: .4byte 0x0400000C
_08036E1C: .4byte gUnknown_03001288

	thumb_func_start sub_8036E20
sub_8036E20: @ 0x08036E20
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r0, #0
	movs r0, #0x80
	lsls r0, r0, #1
	str r0, [sp]
	adds r0, r4, #0
	movs r2, #0
	movs r3, #0
	bl InitActorPart
	ldr r0, _08036EB4 @ =gStaticData_087E55C4
	str r0, [r4, #0x50]
	ldr r2, [r4, #8]
	asrs r2, r2, #8
	ldr r1, [r4, #0xc]
	ldr r3, [r4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r3
	movs r1, #2
	ldrsh r0, [r0, r1]
	adds r0, r0, r2
	ldr r1, [r4, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldrb r3, [r0]
	ldrb r1, [r0, #1]
	adds r2, r3, #0
	muls r2, r1, r2
	adds r0, r2, #0
	lsls r0, r0, #5
	bl sub_8028CD4
	ldr r5, _08036EB8 @ =gUnknown_0300160C
	str r0, [r5]
	ldr r2, [r4, #8]
	asrs r2, r2, #8
	ldr r1, [r4, #0xc]
	ldr r3, [r4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r3
	movs r3, #2
	ldrsh r0, [r0, r3]
	adds r0, r0, r2
	ldr r1, [r4, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldrb r2, [r0]
	ldrb r3, [r0, #1]
	adds r1, r2, #0
	muls r1, r3, r1
	adds r0, r1, #0
	lsls r0, r0, #5
	bl sub_8028CD4
	str r0, [r5, #4]
	ldr r1, _08036EBC @ =gUnknown_03001604
	movs r0, #1
	str r0, [r1]
	ldr r1, _08036EC0 @ =gUnknown_03001608
	movs r0, #0
	str r0, [r1]
	adds r0, r4, #0
	add sp, #4
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08036EB4: .4byte gStaticData_087E55C4
_08036EB8: .4byte gUnknown_0300160C
_08036EBC: .4byte gUnknown_03001604
_08036EC0: .4byte gUnknown_03001608

	thumb_func_start sub_8036EC4
sub_8036EC4: @ 0x08036EC4
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x44]
	adds r1, r0, #1
	str r1, [r4, #0x44]
	ldr r0, [r4, #0x28]
	cmp r0, #1
	beq _08036F00
	cmp r0, #1
	blo _08036EE2
	cmp r0, #2
	beq _08036F38
	cmp r0, #3
	beq _08036F60
	b _08036F76
_08036EE2:
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _08036F76
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
	b _08036F76
_08036F00:
	ldr r0, [r4, #8]
	asrs r0, r0, #8
	cmp r0, #0x12
	bne _08036F76
	movs r0, #2
	movs r1, #7
	str r0, [r4, #0x28]
	movs r2, #0
	str r2, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	adds r0, #0x54
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	ldr r0, _08036F34 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x4f
	bl PlaySfx
	b _08036F76
	.align 2, 0
_08036F34: .4byte gUnknown_030012BC
_08036F38:
	ldr r0, [r4, #8]
	asrs r0, r0, #8
	cmp r0, #7
	bne _08036F76
	movs r1, #0
	strh r1, [r4, #0x10]
	movs r0, #3
	str r0, [r4, #0x28]
	str r1, [r4, #0x44]
	ldr r0, _08036F5C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x1b
	bl PlaySfx
	b _08036F76
	.align 2, 0
_08036F5C: .4byte gUnknown_030012BC
_08036F60:
	ldr r0, [r4, #0x24]
	subs r0, #0xe
	str r0, [r4, #0x24]
	ldr r0, [r4, #0x20]
	ldr r2, _08036FB8 @ =0xFFFFFF00
	adds r0, r0, r2
	str r0, [r4, #0x20]
	cmp r1, #0xf
	ble _08036F76
	movs r0, #4
	str r0, [r4, #0x28]
_08036F76:
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
	blt _08036FB0
	movs r3, #6
	ldrsh r0, [r1, r3]
	subs r0, r2, r0
	lsls r0, r0, #8
	ldr r1, [r4, #8]
	subs r1, r1, r0
	str r1, [r4, #8]
	movs r0, #1
	strb r0, [r4, #0x12]
_08036FB0:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08036FB8: .4byte 0xFFFFFF00

	thumb_func_start sub_8036FBC
sub_8036FBC: @ 0x08036FBC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x14
	adds r7, r0, #0
	ldr r0, [r7, #0x28]
	cmp r0, #4
	bne _08036FD2
	b _080370E8
_08036FD2:
	ldr r2, [r7, #8]
	asrs r2, r2, #8
	ldr r1, [r7, #0xc]
	ldr r3, [r7]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r3
	str r0, [sp, #8]
	movs r1, #2
	ldrsh r0, [r0, r1]
	adds r0, r0, r2
	ldr r1, [r7, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	mov sl, r0
	ldrb r2, [r0]
	str r2, [sp, #0xc]
	lsls r2, r2, #2
	mov sb, r2
	ldrb r3, [r0, #1]
	str r3, [sp, #0x10]
	lsls r3, r3, #2
	mov r8, r3
	ldr r4, [r7, #0x24]
	lsls r0, r4, #8
	ldr r1, [r7, #0x30]
	ldr r1, [r1, #0x10]
	bl sub_803ADB4
	str r0, [sp]
	movs r0, #0x80
	lsls r0, r0, #0xd
	adds r1, r4, #0
	bl sub_803ADB4
	ldr r1, [r7, #0x20]
	muls r1, r0, r1
	asrs r1, r1, #0xc
	movs r2, #0xa0
	lsls r2, r2, #7
	adds r1, r1, r2
	asrs r5, r1, #8
	ldr r1, [r7, #0x1c]
	muls r0, r1, r0
	asrs r0, r0, #0xc
	movs r3, #0xf0
	lsls r3, r3, #7
	adds r0, r0, r3
	asrs r6, r0, #8
	movs r0, #0x80
	lsls r0, r0, #1
	str r0, [sp, #4]
	ldr r1, [sp]
	cmp r1, #0xff
	bgt _0803705A
	movs r0, #0x80
	lsls r0, r0, #2
	ldr r2, [sp, #4]
	orrs r2, r0
	str r2, [sp, #4]
	ldr r3, [sp, #0xc]
	lsls r3, r3, #3
	mov sb, r3
	ldr r0, [sp, #0x10]
	lsls r0, r0, #3
	mov r8, r0
_0803705A:
	mov r1, sb
	subs r6, r6, r1
	mov r2, r8
	subs r5, r5, r2
	cmp r5, #0x9f
	bgt _080370E8
	lsls r0, r2, #1
	adds r0, r5, r0
	cmp r0, #0
	blt _080370E8
	cmp r6, #0xef
	bgt _080370E8
	lsls r0, r1, #1
	adds r0, r6, r0
	cmp r0, #0
	blt _080370E8
	ldr r3, [sp, #8]
	ldrh r3, [r3, #8]
	lsls r4, r3, #0x10
	mov r0, sl
	bl sub_8029108
	movs r1, #0xff
	ands r5, r1
	ldr r1, _080370F8 @ =0x000001FF
	ands r6, r1
	lsls r1, r6, #0x10
	orrs r5, r1
	orrs r5, r4
	orrs r5, r0
	ldr r0, [sp, #4]
	orrs r0, r5
	str r0, [sp, #4]
	ldr r4, _080370FC @ =gUnknown_03001608
	ldr r0, [r4]
	cmp sl, r0
	beq _080370C4
	ldr r2, _08037100 @ =gUnknown_03001604
	ldr r0, [r2]
	movs r1, #1
	eors r0, r1
	str r0, [r2]
	ldr r2, _08037104 @ =gUnknown_03000874
	ldr r1, _08037108 @ =gUnknown_0300160C
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r2, [r2]
	mov r1, sl
	bl sub_803AD80
	mov r1, sl
	str r1, [r4]
_080370C4:
	ldr r1, _08037108 @ =gUnknown_0300160C
	ldr r0, _08037100 @ =gUnknown_03001604
	ldr r0, [r0]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r2, _0803710C @ =0xF9FF0000
	adds r0, r0, r2
	lsrs r0, r0, #5
	ldr r1, [r7, #0x18]
	lsls r1, r1, #0xc
	orrs r1, r0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	ldr r0, [sp, #4]
	ldr r2, [sp]
	bl sub_8028DD8
_080370E8:
	add sp, #0x14
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080370F8: .4byte 0x000001FF
_080370FC: .4byte gUnknown_03001608
_08037100: .4byte gUnknown_03001604
_08037104: .4byte gUnknown_03000874
_08037108: .4byte gUnknown_0300160C
_0803710C: .4byte 0xF9FF0000

