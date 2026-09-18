.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_800E560
sub_800E560: @ 0x0800E560
	push {r4, r5, r6, r7, lr}
	sub sp, #8
	adds r7, r0, #0
	ldr r1, [r7, #0x48]
	movs r0, #0x2a
	rsbs r0, r0, #0
	cmp r1, r0
	bne _0800E57E
	movs r0, #0xb4
	lsls r0, r0, #1
	str r0, [r7, #0x48]
	adds r1, r7, #0
	adds r1, #0x51
	movs r0, #0
	strb r0, [r1]
_0800E57E:
	ldr r0, [r7, #0x48]
	cmp r0, #0
	ble _0800E60C
	adds r3, r7, #0
	adds r3, #0x50
	ldrb r0, [r3]
	cmp r0, #0
	bne _0800E618
	adds r1, r7, #0
	adds r1, #0x51
	ldrb r0, [r1]
	adds r0, #1
	strb r0, [r1]
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #4
	bls _0800E5AE
	adds r0, r7, #0
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_800E7A8
	b _0800E5CE
_0800E5AE:
	adds r1, r7, #0
	adds r1, #0x4d
	movs r0, #0x80
	ldrb r2, [r1]
	orrs r0, r2
	strb r0, [r1]
	ldr r0, _0800E604 @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r1, #1
	adds r0, #0x80
	strb r1, [r0]
	adds r2, r7, #0
	adds r2, #0x4f
	movs r0, #6
	strb r0, [r2]
	strb r1, [r3]
_0800E5CE:
	ldr r1, [r7]
	asrs r1, r1, #8
	ldr r2, [r7, #4]
	asrs r2, r2, #8
	subs r2, #6
	ldr r6, _0800E608 @ =gUnknown_030012E4
	ldr r0, [r6]
	movs r3, #0xe
	str r3, [sp]
	add r5, sp, #4
	movs r4, #1
	strb r4, [r5]
	movs r3, #0
	bl sub_8025CA4
	ldr r1, [r7]
	asrs r1, r1, #8
	adds r1, #3
	ldr r2, [r7, #4]
	asrs r2, r2, #8
	ldr r0, [r6]
	movs r3, #0
	str r3, [sp]
	strb r4, [r5]
	bl sub_8025CA4
	b _0800E618
	.align 2, 0
_0800E604: .4byte gUnknown_030012D8
_0800E608: .4byte gUnknown_030012E4
_0800E60C:
	adds r0, r7, #0
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_800E7A8
_0800E618:
	add sp, #8
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_800E620
sub_800E620: @ 0x0800E620
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r1, r4, #0
	adds r1, #0x4e
	movs r0, #0x15
	strb r0, [r1]
	movs r0, #0x14
	adds r5, r4, #0
	adds r5, #0x2d
	strb r0, [r5]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	movs r0, #0x10
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _0800E6A4 @ =gUnknown_0300130C
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8009150
	ldr r0, [r4, #0x20]
	ldr r1, [r0]
	ldrb r2, [r5]
	lsls r0, r2, #3
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r1, r1, r0
	ldr r0, _0800E6A8 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldrb r1, [r1, #0x14]
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldr r0, _0800E6AC @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x11
	bl PlaySfx
	adds r4, #0x4f
	movs r0, #0x3c
	strb r0, [r4]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0800E6A4: .4byte gUnknown_0300130C
_0800E6A8: .4byte gUnknown_030012B8
_0800E6AC: .4byte gUnknown_030012BC

	thumb_func_start sub_800E6B0
sub_800E6B0: @ 0x0800E6B0
	push {r4, r5, lr}
	sub sp, #8
	adds r4, r0, #0
	ldr r3, [r4]
	asrs r3, r3, #8
	subs r3, #0xa
	ldr r1, [r4, #4]
	asrs r1, r1, #8
	ldr r0, _0800E788 @ =gUnknown_030012E4
	ldr r0, [r0]
	str r1, [sp]
	movs r5, #0
	str r5, [sp, #4]
	movs r1, #0x2a
	movs r2, #0
	bl sub_8025BAC
	movs r1, #5
	rsbs r1, r1, #0
	ldrb r2, [r0, #0xc]
	ands r1, r2
	strb r1, [r0, #0xc]
	adds r2, r0, #0
	adds r2, #0x28
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	strb r1, [r2]
	ldr r1, _0800E78C @ =0xFFFFFE80
	movs r2, #8
	movs r3, #0x10
	rsbs r3, r3, #0
	str r1, [r0, #0x64]
	str r1, [r0, #0x54]
	str r2, [r0, #0x58]
	str r3, [r0, #0x5c]
	movs r0, #0x1b
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
	ldr r0, _0800E790 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x17
	bl PlaySfx
	ldrh r1, [r4, #8]
	ldr r0, _0800E794 @ =0x0000FFFF
	cmp r1, r0
	beq _0800E734
	ldr r0, _0800E798 @ =gUnknown_030012B4
	ldr r0, [r0]
	bl sub_80259D4
_0800E734:
	ldr r0, _0800E79C @ =gStaticData_0816BB98
	adds r1, r4, #0
	adds r1, #0x4e
	ldrb r1, [r1]
	adds r0, r1, r0
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800E74C
	ldr r0, _0800E7A0 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022FEC
_0800E74C:
	ldr r0, _0800E7A0 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r2, r4, #0
	adds r2, #0x50
	ldrb r3, [r2]
	rsbs r1, r3, #0
	orrs r1, r3
	lsrs r1, r1, #0x1f
	bl sub_8022CA0
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r2, [r1]
	ands r0, r2
	strb r0, [r1]
	ldr r0, _0800E7A4 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x80
	strb r5, [r0]
	movs r2, #1
	movs r0, #0x80
	ldrb r3, [r1]
	ands r0, r3
	orrs r0, r2
	strb r0, [r1]
	add sp, #8
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0800E788: .4byte gUnknown_030012E4
_0800E78C: .4byte 0xFFFFFE80
_0800E790: .4byte gUnknown_030012BC
_0800E794: .4byte 0x0000FFFF
_0800E798: .4byte gUnknown_030012B4
_0800E79C: .4byte gStaticData_0816BB98
_0800E7A0: .4byte gUnknown_030012C0
_0800E7A4: .4byte gUnknown_030012D8

	thumb_func_start sub_800E7A8
sub_800E7A8: @ 0x0800E7A8
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	lsls r1, r1, #0x18
	lsrs r6, r1, #0x18
	lsls r2, r2, #0x18
	cmp r2, #0
	beq _0800E7D2
	ldr r2, _0800E810 @ =gUnknown_030012D8
	ldr r0, [r2]
	adds r1, r0, #0
	adds r1, #0x91
	ldrb r0, [r1]
	cmp r0, #0
	bne _0800E880
	movs r0, #1
	strb r0, [r1]
	ldr r0, [r2]
	adds r0, #0x91
	ldrb r1, [r0]
	adds r1, #1
	strb r1, [r0]
_0800E7D2:
	cmp r3, #4
	bne _0800E814
	adds r0, r5, #0
	bl sub_8010708
	adds r4, r0, #0
	cmp r4, #0
	beq _0800E832
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #1
	beq _0800E832
_0800E7F0:
	adds r0, r4, #0
	bl sub_8010708
	adds r2, r0, #0
	cmp r2, #0
	beq _0800E836
	adds r1, r2, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #1
	beq _0800E836
	adds r4, r2, #0
	b _0800E7F0
	.align 2, 0
_0800E810: .4byte gUnknown_030012D8
_0800E814:
	cmp r3, #8
	bne _0800E878
	adds r0, r5, #0
	bl sub_801070C
	adds r4, r0, #0
	cmp r4, #0
	beq _0800E832
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #1
	bne _0800E83A
_0800E832:
	adds r2, r5, #0
	b _0800E858
_0800E836:
	adds r2, r4, #0
	b _0800E858
_0800E83A:
	adds r0, r4, #0
	bl sub_801070C
	adds r2, r0, #0
	cmp r2, #0
	beq _0800E836
	adds r1, r2, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #1
	beq _0800E836
	adds r4, r2, #0
	b _0800E83A
_0800E858:
	ldr r0, _0800E874 @ =gStaticData_0816BBDA
	adds r1, r2, #0
	adds r1, #0x4e
	ldrb r1, [r1]
	adds r0, r1, r0
	ldrb r0, [r0]
	cmp r0, #0
	bne _0800E880
	adds r0, r2, #0
	adds r1, r6, #0
	bl sub_800E888
	b _0800E880
	.align 2, 0
_0800E874: .4byte gStaticData_0816BBDA
_0800E878:
	adds r0, r5, #0
	adds r1, r6, #0
	bl sub_800E888
_0800E880:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_800E888
sub_800E888: @ 0x0800E888
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	adds r4, r0, #0
	lsls r1, r1, #0x18
	lsrs r6, r1, #0x18
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #1
	bne _0800E8A8
	b _0800EAE6
_0800E8A8:
	movs r7, #0
	adds r0, r4, #0
	bl sub_801070C
	cmp r0, #0
	beq _0800E8BA
	cmp r6, #0
	bne _0800E8BA
	movs r7, #1
_0800E8BA:
	movs r0, #0x10
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _0800E9AC @ =gUnknown_0300130C
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8009150
	adds r2, r4, #0
	adds r2, #0x4d
	movs r0, #0x7f
	ldrb r3, [r2]
	ands r0, r3
	movs r1, #0
	strb r0, [r2]
	ldr r0, _0800E9B0 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x80
	strb r1, [r0]
	movs r5, #1
	mov r8, r5
	movs r0, #0x80
	ldrb r1, [r2]
	ands r0, r1
	orrs r0, r5
	strb r0, [r2]
	movs r0, #0x1d
	adds r5, r4, #0
	adds r5, #0x2d
	strb r0, [r5]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r4, #0x20]
	ldr r1, [r0]
	ldrb r2, [r5]
	lsls r0, r2, #3
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r1, r1, r0
	ldr r0, _0800E9B4 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldrb r1, [r1, #0x14]
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r2, #3
	ldr r0, [r4, #0x20]
	ldr r1, [r0]
	ldrb r3, [r5]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r2, r0
	blt _0800E954
	subs r2, r0, #1
_0800E954:
	str r2, [r4, #0x30]
	ldr r0, _0800E9B8 @ =gStaticData_0816BB98
	movs r5, #0x4e
	adds r5, r5, r4
	mov sb, r5
	ldrb r1, [r5]
	adds r0, r1, r0
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800E970
	ldr r0, _0800E9BC @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022FEC
_0800E970:
	ldrh r3, [r4, #8]
	ldr r0, _0800E9C0 @ =gUnknown_030012B4
	ldr r2, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r1, r0, #2
	movs r5, #0x84
	lsls r5, r5, #1
	adds r2, r2, r5
	adds r2, r2, r1
	lsls r0, r0, #5
	subs r0, r3, r0
	mov r1, r8
	lsls r1, r0
	ldr r0, [r2]
	orrs r0, r1
	str r0, [r2]
	adds r0, r4, #0
	bl sub_800EDBC
	mov r1, sb
	ldrb r0, [r1]
	cmp r0, #0x16
	bls _0800E9A2
	b _0800EAE6
_0800E9A2:
	lsls r0, r0, #2
	ldr r1, _0800E9C4 @ =_0800E9C8
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800E9AC: .4byte gUnknown_0300130C
_0800E9B0: .4byte gUnknown_030012D8
_0800E9B4: .4byte gUnknown_030012B8
_0800E9B8: .4byte gStaticData_0816BB98
_0800E9BC: .4byte gUnknown_030012C0
_0800E9C0: .4byte gUnknown_030012B4
_0800E9C4: .4byte _0800E9C8
_0800E9C8: @ jump table
	.4byte _0800EAB8 @ case 0
	.4byte _0800EAE6 @ case 1
	.4byte _0800EA24 @ case 2
	.4byte _0800EA3E @ case 3
	.4byte _0800EA5C @ case 4
	.4byte _0800EAE6 @ case 5
	.4byte _0800EA46 @ case 6
	.4byte _0800EAE6 @ case 7
	.4byte _0800EAE6 @ case 8
	.4byte _0800EA30 @ case 9
	.4byte _0800EA70 @ case 10
	.4byte _0800EA4E @ case 11
	.4byte _0800EA5C @ case 12
	.4byte _0800EA5C @ case 13
	.4byte _0800EA70 @ case 14
	.4byte _0800EA7A @ case 15
	.4byte _0800EA88 @ case 16
	.4byte _0800EA98 @ case 17
	.4byte _0800EAA8 @ case 18
	.4byte _0800EA70 @ case 19
	.4byte _0800EA70 @ case 20
	.4byte _0800EA70 @ case 21
	.4byte _0800EAE6 @ case 22
_0800EA24:
	cmp r6, #0
	bne _0800EAE6
	adds r0, r4, #0
	bl sub_801085C
	b _0800EAE6
_0800EA30:
	cmp r6, #0
	bne _0800EAE6
	adds r0, r4, #0
	adds r1, r7, #0
	bl sub_801089C
	b _0800EAE6
_0800EA3E:
	adds r0, r4, #0
	bl sub_800F368
	b _0800EAE6
_0800EA46:
	adds r0, r4, #0
	bl sub_800F2BC
	b _0800EAE6
_0800EA4E:
	cmp r6, #0
	bne _0800EAE6
	adds r0, r4, #0
	adds r1, r7, #0
	bl sub_800EAFC
	b _0800EAE6
_0800EA5C:
	ldr r0, _0800EA6C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	b _0800EAE6
	.align 2, 0
_0800EA6C: .4byte gUnknown_030012BC
_0800EA70:
	adds r0, r4, #0
	movs r1, #0
	bl sub_800EEF0
	b _0800EAE6
_0800EA7A:
	cmp r6, #0
	bne _0800EAE6
	adds r0, r4, #0
	adds r1, r7, #0
	bl sub_800ED08
	b _0800EAE6
_0800EA88:
	ldr r0, _0800EA94 @ =gUnknown_030012C0
	ldr r0, [r0]
	movs r1, #1
	bl sub_8022EA8
	b _0800EAE6
	.align 2, 0
_0800EA94: .4byte gUnknown_030012C0
_0800EA98:
	ldr r0, _0800EAA4 @ =gUnknown_030012C0
	ldr r0, [r0]
	movs r1, #2
	bl sub_8022EA8
	b _0800EAE6
	.align 2, 0
_0800EAA4: .4byte gUnknown_030012C0
_0800EAA8:
	ldr r0, _0800EAB4 @ =gUnknown_030012C0
	ldr r0, [r0]
	movs r1, #3
	bl sub_8022EA8
	b _0800EAE6
	.align 2, 0
_0800EAB4: .4byte gUnknown_030012C0
_0800EAB8:
	ldr r1, [r4]
	asrs r1, r1, #8
	ldr r2, [r4, #4]
	asrs r2, r2, #8
	adds r2, #3
	ldr r0, _0800EAF4 @ =gUnknown_030012E4
	ldr r0, [r0]
	movs r3, #3
	str r3, [sp]
	add r3, sp, #4
	strb r7, [r3]
	movs r3, #0
	bl sub_8025CA4
	cmp r6, #0
	bne _0800EAE6
	ldr r0, _0800EAF8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
_0800EAE6:
	add sp, #8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800EAF4: .4byte gUnknown_030012E4
_0800EAF8: .4byte gUnknown_030012BC

	thumb_func_start sub_800EAFC
sub_800EAFC: @ 0x0800EAFC
	push {r4, r5, r6, lr}
	sub sp, #8
	adds r4, r0, #0
	lsls r1, r1, #0x18
	lsrs r6, r1, #0x18
	ldr r0, _0800EB30 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #3
	bl PlaySfx
	adds r5, r4, #0
	adds r5, #0x51
	ldrb r0, [r5]
	cmp r0, #9
	bne _0800EB48
	bl rand
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x18
	adds r1, r0, #0
	cmp r0, #0x56
	bhi _0800EB34
	movs r0, #1
	b _0800EB46
	.align 2, 0
_0800EB30: .4byte gUnknown_030012BC
_0800EB34:
	cmp r0, #0xd3
	bhi _0800EB3C
	movs r0, #4
	b _0800EB46
_0800EB3C:
	cmp r1, #0xec
	bhi _0800EB44
	movs r0, #7
	b _0800EB46
_0800EB44:
	movs r0, #8
_0800EB46:
	strb r0, [r5]
_0800EB48:
	adds r0, r4, #0
	adds r0, #0x51
	ldrb r0, [r0]
	subs r0, #1
	cmp r0, #9
	bls _0800EB56
	b _0800ECDE
_0800EB56:
	lsls r0, r0, #2
	ldr r1, _0800EB60 @ =_0800EB64
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800EB60: .4byte _0800EB64
_0800EB64: @ jump table
	.4byte _0800ECDE @ case 0
	.4byte _0800ECC0 @ case 1
	.4byte _0800ECA2 @ case 2
	.4byte _0800EC84 @ case 3
	.4byte _0800EC66 @ case 4
	.4byte _0800EC48 @ case 5
	.4byte _0800EC0C @ case 6
	.4byte _0800EBB0 @ case 7
	.4byte _0800ECDE @ case 8
	.4byte _0800EB8C @ case 9
_0800EB8C:
	ldr r1, [r4]
	asrs r1, r1, #8
	ldr r2, [r4, #4]
	asrs r2, r2, #8
	ldr r0, _0800EBAC @ =gUnknown_030012E4
	ldr r0, [r0]
	movs r3, #0xff
	str r3, [sp]
	add r4, sp, #4
	movs r3, #0
	strb r3, [r4]
	movs r3, #0
	bl sub_8025CA4
	b _0800ECFC
	.align 2, 0
_0800EBAC: .4byte gUnknown_030012E4
_0800EBB0:
	ldr r0, _0800EBFC @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #3
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	ldrh r1, [r4, #8]
	ldr r0, _0800EC00 @ =0x0000FFFF
	cmp r1, r0
	beq _0800EBDC
	ldr r5, _0800EC04 @ =gUnknown_030012B4
	ldr r0, [r5]
	bl sub_802599C
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0800EBDC
	ldr r0, [r5]
	ldrh r1, [r4, #8]
	bl sub_80259D4
_0800EBDC:
	ldr r1, [r4]
	asrs r1, r1, #8
	ldr r2, [r4, #4]
	asrs r2, r2, #8
	adds r2, #3
	ldr r0, _0800EC08 @ =gUnknown_030012E4
	ldr r0, [r0]
	movs r3, #3
	str r3, [sp]
	add r3, sp, #4
	strb r6, [r3]
	movs r3, #0
	bl sub_8025A64
	b _0800ECFC
	.align 2, 0
_0800EBFC: .4byte gUnknown_030012BC
_0800EC00: .4byte 0x0000FFFF
_0800EC04: .4byte gUnknown_030012B4
_0800EC08: .4byte gUnknown_030012E4
_0800EC0C:
	ldr r0, _0800EC40 @ =gUnknown_030012D8
	ldr r2, [r0]
	ldrb r1, [r2, #0xc]
	lsrs r0, r1, #7
	cmp r0, #0
	beq _0800ECFC
	ldr r1, [r2, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0x1a
	movs r3, #0
	bl sub_803AD88
	ldr r0, _0800EC44 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #1
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	b _0800ECFC
	.align 2, 0
_0800EC40: .4byte gUnknown_030012D8
_0800EC44: .4byte gUnknown_030012BC
_0800EC48:
	ldr r1, [r4]
	asrs r1, r1, #8
	subs r1, #1
	ldr r2, [r4, #4]
	asrs r2, r2, #8
	adds r2, #3
	ldr r0, _0800ED04 @ =gUnknown_030012E4
	ldr r0, [r0]
	movs r3, #3
	str r3, [sp]
	add r3, sp, #4
	strb r6, [r3]
	movs r3, #1
	bl sub_8025CA4
_0800EC66:
	ldr r1, [r4]
	asrs r1, r1, #8
	adds r1, #1
	ldr r2, [r4, #4]
	asrs r2, r2, #8
	adds r2, #1
	ldr r0, _0800ED04 @ =gUnknown_030012E4
	ldr r0, [r0]
	movs r3, #3
	str r3, [sp]
	add r3, sp, #4
	strb r6, [r3]
	movs r3, #0
	bl sub_8025CA4
_0800EC84:
	ldr r1, [r4]
	asrs r1, r1, #8
	subs r1, #3
	ldr r2, [r4, #4]
	asrs r2, r2, #8
	adds r2, #3
	ldr r0, _0800ED04 @ =gUnknown_030012E4
	ldr r0, [r0]
	movs r3, #1
	str r3, [sp]
	add r3, sp, #4
	strb r6, [r3]
	movs r3, #1
	bl sub_8025CA4
_0800ECA2:
	ldr r1, [r4]
	asrs r1, r1, #8
	adds r1, #3
	ldr r2, [r4, #4]
	asrs r2, r2, #8
	adds r2, #2
	ldr r0, _0800ED04 @ =gUnknown_030012E4
	ldr r0, [r0]
	movs r3, #1
	str r3, [sp]
	add r3, sp, #4
	strb r6, [r3]
	movs r3, #0
	bl sub_8025CA4
_0800ECC0:
	ldr r1, [r4]
	asrs r1, r1, #8
	adds r1, #5
	ldr r2, [r4, #4]
	asrs r2, r2, #8
	adds r2, #2
	ldr r0, _0800ED04 @ =gUnknown_030012E4
	ldr r0, [r0]
	movs r3, #2
	str r3, [sp]
	add r3, sp, #4
	strb r6, [r3]
	movs r3, #0
	bl sub_8025CA4
_0800ECDE:
	ldr r1, [r4]
	asrs r1, r1, #8
	subs r1, #5
	ldr r2, [r4, #4]
	asrs r2, r2, #8
	adds r2, #3
	ldr r0, _0800ED04 @ =gUnknown_030012E4
	ldr r0, [r0]
	movs r3, #2
	str r3, [sp]
	add r3, sp, #4
	strb r6, [r3]
	movs r3, #1
	bl sub_8025CA4
_0800ECFC:
	add sp, #8
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0800ED04: .4byte gUnknown_030012E4

	thumb_func_start sub_800ED08
sub_800ED08: @ 0x0800ED08
	push {r4, r5, r6, r7, lr}
	sub sp, #8
	adds r4, r0, #0
	lsls r1, r1, #0x18
	lsrs r7, r1, #0x18
	ldr r5, _0800ED3C @ =gUnknown_030012BC
	ldr r0, [r5]
	movs r6, #0x80
	lsls r6, r6, #1
	movs r1, #3
	adds r2, r6, #0
	bl PlaySfx
	ldr r1, [r4, #0x48]
	movs r0, #7
	ands r1, r0
	cmp r1, #1
	beq _0800ED40
	cmp r1, #1
	ble _0800EDB2
	cmp r1, #2
	beq _0800ED94
	cmp r1, #3
	beq _0800ED9E
	b _0800EDB2
	.align 2, 0
_0800ED3C: .4byte gUnknown_030012BC
_0800ED40:
	ldr r0, [r5]
	movs r1, #3
	adds r2, r6, #0
	bl PlaySfx
	ldrh r1, [r4, #8]
	ldr r0, _0800ED88 @ =0x0000FFFF
	cmp r1, r0
	beq _0800ED68
	ldr r5, _0800ED8C @ =gUnknown_030012B4
	ldr r0, [r5]
	bl sub_802599C
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0800ED68
	ldr r0, [r5]
	ldrh r1, [r4, #8]
	bl sub_80259D4
_0800ED68:
	ldr r1, [r4]
	asrs r1, r1, #8
	ldr r2, [r4, #4]
	asrs r2, r2, #8
	adds r2, #3
	ldr r0, _0800ED90 @ =gUnknown_030012E4
	ldr r0, [r0]
	movs r3, #3
	str r3, [sp]
	add r3, sp, #4
	strb r7, [r3]
	movs r3, #0
	bl sub_8025A64
	b _0800EDB2
	.align 2, 0
_0800ED88: .4byte 0x0000FFFF
_0800ED8C: .4byte gUnknown_030012B4
_0800ED90: .4byte gUnknown_030012E4
_0800ED94:
	adds r0, r4, #0
	adds r1, r7, #0
	bl sub_800EAFC
	b _0800EDB2
_0800ED9E:
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x80
	ldrb r2, [r1]
	ands r0, r2
	strb r0, [r1]
	adds r0, r4, #0
	movs r1, #1
	bl sub_800EEF0
_0800EDB2:
	add sp, #8
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_800EDBC
sub_800EDBC: @ 0x0800EDBC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov r8, r0
	movs r0, #0xfe
	str r0, [sp]
	mov r1, r8
	ldr r0, [r1, #0x20]
	mov r2, r8
	adds r2, #0x2d
	ldrb r3, [r2]
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	ldr r0, [r0]
	adds r0, r0, r1
	ldrb r0, [r0, #9]
	adds r0, #1
	lsls r0, r0, #8
	mov sl, r0
	mov r0, r8
	bl sub_801070C
	adds r5, r0, #0
	mov r1, r8
	ldr r0, [r1, #0x44]
	cmp r0, #0
	beq _0800EDFE
	movs r2, #0xfc
	str r2, [sp]
_0800EDFE:
	cmp r5, #0
	beq _0800EEE0
	mov r3, r8
	cmp r3, #0
	beq _0800EEE0
	cmp r0, #0
	beq _0800EE10
	ldr r0, [r3, #0x40]
	b _0800EE14
_0800EE10:
	mov r1, r8
	ldr r0, [r1, #4]
_0800EE14:
	str r0, [r5, #0x40]
	ldr r7, [r5, #0x40]
	ldr r0, [r5, #4]
	subs r7, r7, r0
	cmp r7, #0
	bge _0800EE22
	movs r7, #0
_0800EE22:
	movs r2, #0
	mov sb, r2
_0800EE26:
	cmp r5, #0
	beq _0800EEE0
	ldr r0, [r5, #0x44]
	cmp r0, #0
	beq _0800EE3C
	mov r3, sb
	adds r0, r7, r3
	str r0, [r5, #0x44]
	ldr r0, [r5, #0x40]
	add r0, sb
	b _0800EE42
_0800EE3C:
	str r7, [r5, #0x44]
	ldr r0, [r5, #4]
	add r0, sl
_0800EE42:
	str r0, [r5, #0x40]
	adds r2, r5, #0
	adds r2, #0x4c
	movs r1, #0
	ldrsb r1, [r2, r1]
	cmp r1, #0
	ble _0800EE52
	movs r1, #0
_0800EE52:
	ldr r3, [sp]
	lsls r0, r3, #0x18
	asrs r0, r0, #0x18
	adds r0, r1, r0
	strb r0, [r2]
	movs r0, #0x10
	ldrb r1, [r5, #0xc]
	orrs r0, r1
	strb r0, [r5, #0xc]
	ldr r0, _0800EED8 @ =gUnknown_0300130C
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8009150
	adds r6, r5, #0
	adds r6, #0x4e
	ldrb r2, [r6]
	ldr r3, _0800EEDC @ =gStaticData_0816BBC4
	adds r0, r2, r3
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800EEC6
	mov r1, r8
	ldr r0, [r1, #0x48]
	cmp r0, #0
	bne _0800EEC6
	ldr r1, [r5, #0x44]
	movs r0, #0xb0
	lsls r0, r0, #5
	cmp r1, r0
	ble _0800EEC6
	adds r0, r5, #0
	bl sub_801070C
	adds r4, r0, #0
	adds r0, r5, #0
	bl sub_8010708
	cmp r4, #0
	bne _0800EEBA
	cmp r0, #0
	beq _0800EEBA
	ldrb r6, [r6]
	cmp r6, #0xa
	bne _0800EEC6
	adds r1, r5, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800EEC6
_0800EEBA:
	adds r1, r5, #0
	adds r1, #0x4f
	movs r0, #0
	strb r0, [r1]
	movs r0, #1
	str r0, [r5, #0x48]
_0800EEC6:
	adds r0, r5, #0
	bl sub_801070C
	adds r5, r0, #0
	mov r2, sb
	cmp r2, #0
	bne _0800EE26
	mov sb, sl
	b _0800EE26
	.align 2, 0
_0800EED8: .4byte gUnknown_0300130C
_0800EEDC: .4byte gStaticData_0816BBC4
_0800EEE0:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_800EEF0
sub_800EEF0: @ 0x0800EEF0
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	lsls r1, r1, #0x18
	lsrs r7, r1, #0x18
	adds r5, r4, #0
	adds r5, #0x4d
	movs r2, #0x7f
	adds r0, r2, #0
	ldrb r1, [r5]
	ands r0, r1
	cmp r0, #1
	bne _0800EF0A
	b _0800F04C
_0800EF0A:
	adds r0, r4, #0
	adds r0, #0x4f
	movs r1, #0
	strb r1, [r0]
	adds r0, r2, #0
	ldrb r2, [r5]
	ands r0, r2
	strb r0, [r5]
	ldr r0, _0800EF68 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x80
	strb r1, [r0]
	movs r0, #0x10
	ldrb r3, [r4, #0xc]
	orrs r0, r3
	strb r0, [r4, #0xc]
	ldr r0, _0800EF6C @ =gUnknown_0300130C
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8009150
	movs r1, #1
	movs r0, #0x80
	ldrb r2, [r5]
	ands r0, r2
	orrs r0, r1
	strb r0, [r5]
	adds r0, r4, #0
	adds r0, #0x4e
	adds r6, r0, #0
	ldrb r3, [r6]
	cmp r3, #0xa
	bne _0800EF70
	subs r0, #0x21
	strb r1, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	b _0800EF8C
	.align 2, 0
_0800EF68: .4byte gUnknown_030012D8
_0800EF6C: .4byte gUnknown_0300130C
_0800EF70:
	movs r0, #0x21
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
_0800EF8C:
	ldr r0, _0800F054 @ =gStaticData_0816BB98
	ldrb r5, [r6]
	adds r0, r5, r0
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800EFA0
	ldr r0, _0800F058 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8022FEC
_0800EFA0:
	ldrh r3, [r4, #8]
	ldr r0, _0800F05C @ =gUnknown_030012B4
	ldr r2, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r1, r0, #2
	movs r5, #0x84
	lsls r5, r5, #1
	adds r2, r2, r5
	adds r2, r2, r1
	lsls r0, r0, #5
	subs r0, r3, r0
	movs r1, #1
	lsls r1, r0
	ldr r0, [r2]
	orrs r0, r1
	str r0, [r2]
	ldr r0, _0800F060 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	adds r0, r4, #0
	bl sub_800EDBC
	ldr r0, _0800F064 @ =gUnknown_030012D8
	ldr r3, [r0]
	ldrb r1, [r3, #0xc]
	lsrs r0, r1, #6
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _0800F042
	movs r5, #0
	adds r0, r3, #0
	adds r0, #0x8c
	ldr r1, _0800F068 @ =gUnknown_0300082C
	ldr r2, [r0]
	ldr r0, [r1]
	cmp r2, r0
	bls _0800EFF8
	movs r5, #1
_0800EFF8:
	cmp r5, #0
	bne _0800F042
	ldr r0, [r3]
	asrs r0, r0, #8
	ldr r1, [r4]
	asrs r1, r1, #8
	subs r0, r0, r1
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	cmp r0, #0x1d
	bgt _0800F024
	ldr r0, [r3, #4]
	asrs r0, r0, #8
	ldr r1, [r4, #4]
	asrs r1, r1, #8
	subs r0, r0, r1
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	cmp r0, #0x1d
	ble _0800F028
_0800F024:
	cmp r7, #0
	beq _0800F042
_0800F028:
	ldr r0, _0800F064 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #4
	movs r3, #0
	bl sub_803AD88
_0800F042:
	ldrb r5, [r6]
	cmp r5, #0xa
	beq _0800F04C
	movs r0, #0x13
	strb r0, [r6]
_0800F04C:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800F054: .4byte gStaticData_0816BB98
_0800F058: .4byte gUnknown_030012C0
_0800F05C: .4byte gUnknown_030012B4
_0800F060: .4byte gUnknown_030012BC
_0800F064: .4byte gUnknown_030012D8
_0800F068: .4byte gUnknown_0300082C

	thumb_func_start sub_800F06C
sub_800F06C: @ 0x0800F06C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	mov r8, r1
	movs r5, #0
	ldr r0, _0800F0F0 @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r0, [r0]
	cmp r5, r0
	bge _0800F136
	ldr r7, _0800F0F4 @ =gStaticData_0816BBC4
_0800F084:
	ldr r0, _0800F0F0 @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r5, #2
	adds r0, r0, r1
	ldr r4, [r0]
	ldr r1, [r4, #0x18]
	adds r1, #0x48
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	cmp r0, #3
	bne _0800F12A
	ldr r2, [r4]
	asrs r2, r2, #8
	ldr r0, [r6]
	asrs r0, r0, #8
	subs r2, r2, r0
	asrs r0, r2, #0x1f
	eors r2, r0
	subs r2, r2, r0
	ldr r1, [r4, #4]
	asrs r1, r1, #8
	ldr r0, [r6, #4]
	asrs r0, r0, #8
	subs r1, r1, r0
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	adds r2, r2, r1
	cmp r2, r8
	bgt _0800F12A
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800F12A
	adds r0, r4, #0
	adds r0, #0x4e
	ldrb r1, [r0]
	adds r0, r1, r7
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800F0F8
	adds r0, r4, #0
	movs r1, #0
	bl sub_800EEF0
	b _0800F12A
	.align 2, 0
_0800F0F0: .4byte gUnknown_0300130C
_0800F0F4: .4byte gStaticData_0816BBC4
_0800F0F8:
	ldr r0, _0800F110 @ =gStaticData_0816BBAE
	adds r0, r1, r0
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800F114
	adds r0, r4, #0
	movs r1, #1
	movs r2, #0
	movs r3, #0
	bl sub_800E7A8
	b _0800F12A
	.align 2, 0
_0800F110: .4byte gStaticData_0816BBAE
_0800F114:
	cmp r1, #3
	bne _0800F120
	adds r0, r4, #0
	bl sub_800F368
	b _0800F12A
_0800F120:
	cmp r1, #6
	bne _0800F12A
	adds r0, r4, #0
	bl sub_800F2BC
_0800F12A:
	adds r5, #1
	ldr r0, _0800F1B0 @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r0, [r0]
	cmp r5, r0
	blt _0800F084
_0800F136:
	movs r5, #0
	ldr r1, _0800F1B4 @ =gUnknown_030012EC
	ldr r0, [r1]
	ldr r0, [r0, #4]
	cmp r5, r0
	bge _0800F1A2
	adds r7, r1, #0
_0800F144:
	ldr r0, [r7]
	ldr r1, [r0, #0xc]
	lsls r0, r5, #2
	adds r0, r0, r1
	ldr r4, [r0]
	ldr r1, [r4, #0x18]
	adds r1, #0x48
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	cmp r0, #2
	bne _0800F198
	ldr r2, [r4]
	asrs r2, r2, #8
	ldr r0, [r6]
	asrs r0, r0, #8
	subs r2, r2, r0
	asrs r0, r2, #0x1f
	eors r2, r0
	subs r2, r2, r0
	ldr r1, [r4, #4]
	asrs r1, r1, #8
	ldr r0, [r6, #4]
	asrs r0, r0, #8
	subs r1, r1, r0
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	adds r2, r2, r1
	cmp r2, r8
	bgt _0800F198
	adds r0, r4, #0
	movs r1, #1
	bl sub_8011448
	movs r0, #0x10
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
_0800F198:
	adds r5, #1
	ldr r0, [r7]
	ldr r0, [r0, #4]
	cmp r5, r0
	blt _0800F144
_0800F1A2:
	movs r0, #0xff
	str r0, [r6, #0x48]
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800F1B0: .4byte gUnknown_0300130C
_0800F1B4: .4byte gUnknown_030012EC

	thumb_func_start sub_800F1B8
sub_800F1B8: @ 0x0800F1B8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	bl sub_800F258
	ldr r0, _0800F224 @ =gUnknown_030012B0
	mov r8, r0
_0800F1C6:
	movs r0, #0
	mov r1, r8
	strb r0, [r1]
	movs r5, #0
	ldr r7, _0800F228 @ =gUnknown_0300130C
	ldr r0, [r7]
	ldr r0, [r0]
	cmp r5, r0
	bge _0800F244
	adds r6, r7, #0
_0800F1DA:
	ldr r0, [r6]
	ldr r1, [r0, #8]
	lsls r0, r5, #2
	adds r0, r0, r1
	ldr r4, [r0]
	ldr r1, [r4, #0x18]
	adds r1, #0x48
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	cmp r0, #3
	bne _0800F23A
	movs r0, #1
	ldrb r1, [r4, #0xc]
	ands r0, r1
	cmp r0, #0
	beq _0800F22C
	ldr r0, [r6]
	adds r1, r5, #0
	bl sub_8009AA0
	cmp r4, #0
	beq _0800F220
	ldr r1, [r4, #0x18]
	adds r1, #0x50
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #4]
	movs r1, #3
	bl sub_803AD80
_0800F220:
	subs r5, #1
	b _0800F23A
	.align 2, 0
_0800F224: .4byte gUnknown_030012B0
_0800F228: .4byte gUnknown_0300130C
_0800F22C:
	ldr r1, [r4, #0x18]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #0x1c]
	bl sub_803AD7C
_0800F23A:
	adds r5, #1
	ldr r0, [r7]
	ldr r0, [r0]
	cmp r5, r0
	blt _0800F1DA
_0800F244:
	mov r1, r8
	ldrb r0, [r1]
	cmp r0, #0
	bne _0800F1C6
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_800F258
sub_800F258: @ 0x0800F258
	push {r4, r5, r6, lr}
	movs r5, #0
	ldr r1, _0800F2B8 @ =gUnknown_0300130C
	ldr r0, [r1]
	ldr r0, [r0]
	cmp r5, r0
	bge _0800F2B0
	adds r6, r1, #0
_0800F268:
	ldr r0, [r6]
	ldr r1, [r0, #8]
	lsls r0, r5, #2
	adds r0, r0, r1
	ldr r4, [r0]
	ldr r1, [r4, #0x18]
	adds r1, #0x48
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	cmp r0, #3
	bne _0800F2A6
	adds r0, r4, #0
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #0xa
	bne _0800F2A6
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800F2A6
	adds r0, r4, #0
	movs r1, #0
	bl sub_800EEF0
_0800F2A6:
	adds r5, #1
	ldr r0, [r6]
	ldr r0, [r0]
	cmp r5, r0
	blt _0800F268
_0800F2B0:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0800F2B8: .4byte gUnknown_0300130C

	thumb_func_start sub_800F2BC
sub_800F2BC: @ 0x0800F2BC
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	ldr r0, [r6, #0x48]
	cmp r0, #0
	bne _0800F34C
	adds r1, r6, #0
	adds r1, #0x4d
	movs r0, #0x80
	ldrb r2, [r1]
	orrs r0, r2
	strb r0, [r1]
	ldr r0, _0800F354 @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r4, #1
	adds r0, #0x80
	strb r4, [r0]
	movs r0, #0x23
	adds r5, r6, #0
	adds r5, #0x2d
	strb r0, [r5]
	adds r0, r6, #0
	bl sub_80087C0
	adds r0, r6, #0
	bl sub_80087B4
	adds r0, r6, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r6, #0x20]
	ldr r1, [r0]
	ldrb r3, [r5]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	adds r1, r1, r0
	ldr r0, _0800F358 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldrb r1, [r1, #0x14]
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	adds r2, r6, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	bl sub_800F258
	ldr r0, _0800F35C @ =gUnknown_03001318
	ldr r0, [r0]
	bl sub_8028474
	ldr r0, _0800F360 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	str r4, [r6, #0x48]
	ldr r0, _0800F364 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_802306C
_0800F34C:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0800F354: .4byte gUnknown_030012D8
_0800F358: .4byte gUnknown_030012B8
_0800F35C: .4byte gUnknown_03001318
_0800F360: .4byte gUnknown_030012BC
_0800F364: .4byte gUnknown_030012C0

	thumb_func_start sub_800F368
sub_800F368: @ 0x0800F368
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x80
	adds r6, r0, #0
	movs r7, #0
	ldr r1, [r6, #0x48]
	movs r0, #1
	rsbs r0, r0, #0
	cmp r1, r0
	bne _0800F384
	b _0800F4E4
_0800F384:
	cmp r1, #0
	beq _0800F38A
	b _0800F4E4
_0800F38A:
	movs r0, #0x10
	ldrb r1, [r6, #0xc]
	orrs r0, r1
	strb r0, [r6, #0xc]
	ldr r4, _0800F4C0 @ =gUnknown_0300130C
	ldr r0, [r4]
	adds r1, r6, #0
	bl sub_8009150
	adds r1, r6, #0
	adds r1, #0x4d
	movs r0, #0x80
	ldrb r2, [r1]
	orrs r0, r2
	strb r0, [r1]
	ldr r0, _0800F4C4 @ =gUnknown_030012D8
	ldr r0, [r0]
	movs r1, #1
	adds r0, #0x80
	strb r1, [r0]
	movs r0, #0x22
	adds r5, r6, #0
	adds r5, #0x2d
	strb r0, [r5]
	adds r0, r6, #0
	bl sub_80087C0
	adds r0, r6, #0
	bl sub_80087B4
	adds r0, r6, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r6, #0x20]
	ldr r1, [r0]
	ldrb r3, [r5]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	adds r1, r1, r0
	ldr r0, _0800F4C8 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldrb r1, [r1, #0x14]
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	adds r2, r6, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldr r0, _0800F4CC @ =gUnknown_030012B4
	ldr r0, [r0]
	ldrh r1, [r6, #8]
	bl sub_8025A0C
	movs r5, #0
	ldr r0, [r4]
	ldr r0, [r0]
	movs r1, #0x50
	adds r1, r1, r6
	mov r8, r1
	movs r2, #0x4c
	adds r2, r2, r6
	mov sb, r2
	movs r3, #0x4f
	adds r3, r3, r6
	mov sl, r3
	cmp r7, r0
	bge _0800F48C
_0800F424:
	ldr r0, _0800F4C0 @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r5, #2
	adds r0, r0, r1
	ldr r4, [r0]
	ldr r1, [r4, #0x18]
	adds r1, #0x48
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	cmp r0, #3
	bne _0800F480
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800F480
	adds r0, r4, #0
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #5
	bne _0800F480
	adds r0, r4, #0
	adds r0, #0x50
	ldrb r0, [r0]
	mov r3, r8
	ldrb r3, [r3]
	cmp r0, r3
	bne _0800F480
	lsls r0, r7, #2
	add r0, sp
	str r4, [r0]
	adds r7, #1
	movs r0, #0x1f
	ands r7, r0
	ldr r0, _0800F4CC @ =gUnknown_030012B4
	ldr r0, [r0]
	ldrh r1, [r4, #8]
	bl sub_8025A0C
_0800F480:
	adds r5, #1
	ldr r0, _0800F4C0 @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r0, [r0]
	cmp r5, r0
	blt _0800F424
_0800F48C:
	cmp r7, #0
	beq _0800F4D0
	adds r0, r7, #1
	lsls r0, r0, #2
	bl sub_8026EC0
	adds r2, r0, #0
	adds r1, r6, #0
	adds r1, #0x59
	movs r0, #1
	strb r0, [r1]
	str r7, [r2]
	cmp r7, #0
	ble _0800F4BA
	mov r3, sp
	adds r1, r2, #0
	adds r5, r7, #0
_0800F4AE:
	ldm r3!, {r0}
	str r0, [r1, #4]
	adds r1, #4
	subs r5, #1
	cmp r5, #0
	bne _0800F4AE
_0800F4BA:
	str r2, [r6, #0x48]
	b _0800F4D6
	.align 2, 0
_0800F4C0: .4byte gUnknown_0300130C
_0800F4C4: .4byte gUnknown_030012D8
_0800F4C8: .4byte gUnknown_030012B8
_0800F4CC: .4byte gUnknown_030012B4
_0800F4D0:
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r6, #0x48]
_0800F4D6:
	movs r0, #0
	mov r1, r8
	strb r0, [r1]
	mov r2, sb
	ldrb r0, [r2]
	mov r3, sl
	strb r0, [r3]
_0800F4E4:
	add sp, #0x80
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_800F4F4
sub_800F4F4: @ 0x0800F4F4
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r4, r0, #0
	adds r3, r4, #0
	adds r3, #0x4f
	ldrb r5, [r3]
	cmp r5, #0
	bne _0800F5A6
	adds r1, r4, #0
	adds r1, #0x50
	ldrb r0, [r1]
	adds r0, #1
	strb r0, [r1]
	adds r2, r4, #0
	adds r2, #0x51
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	adds r7, r1, #0
	ldrb r2, [r2]
	cmp r0, r2
	blo _0800F54A
	ldr r1, [r4, #0x48]
	adds r0, r1, #1
	cmp r0, #1
	bls _0800F53A
	cmp r1, #0
	beq _0800F534
	adds r0, r1, #0
	bl sub_8026EB4
_0800F534:
	adds r0, r4, #0
	adds r0, #0x59
	strb r5, [r0]
_0800F53A:
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r4, #0x48]
	movs r1, #7
	adds r0, r4, #0
	adds r0, #0x4e
	strb r1, [r0]
	b _0800F5A6
_0800F54A:
	ldr r1, [r4, #0x48]
	adds r0, r1, #1
	mov sb, r3
	adds r4, #0x4c
	mov r8, r4
	cmp r0, #1
	bls _0800F59E
	ldm r1!, {r2}
	adds r0, r1, #0
	movs r6, #0
	cmp r2, #0
	ble _0800F59E
	adds r4, r0, #0
	adds r5, r2, #0
_0800F566:
	ldr r0, [r4]
	adds r1, r0, #0
	adds r1, #0x4e
	ldrb r1, [r1]
	cmp r1, #5
	bne _0800F596
	adds r1, r0, #0
	adds r1, #0x51
	ldrb r2, [r7]
	ldrb r1, [r1]
	cmp r2, r1
	blo _0800F596
	bl sub_800F5B8
	cmp r6, #0
	bne _0800F596
	ldr r0, _0800F5B4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0xf
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	movs r6, #1
_0800F596:
	adds r4, #4
	subs r5, #1
	cmp r5, #0
	bne _0800F566
_0800F59E:
	mov r1, r8
	ldrb r0, [r1]
	mov r2, sb
	strb r0, [r2]
_0800F5A6:
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800F5B4: .4byte gUnknown_030012BC

	thumb_func_start sub_800F5B8
sub_800F5B8: @ 0x0800F5B8
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x48]
	subs r0, #0x15
	adds r1, r4, #0
	adds r1, #0x4e
	strb r0, [r1]
	ldrb r0, [r1]
	cmp r0, #0x12
	bhi _0800F698
	lsls r0, r0, #2
	ldr r1, _0800F5D8 @ =_0800F5DC
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800F5D8: .4byte _0800F5DC
_0800F5DC: @ jump table
	.4byte _0800F628 @ case 0
	.4byte _0800F62C @ case 1
	.4byte _0800F630 @ case 2
	.4byte _0800F698 @ case 3
	.4byte _0800F634 @ case 4
	.4byte _0800F698 @ case 5
	.4byte _0800F638 @ case 6
	.4byte _0800F63C @ case 7
	.4byte _0800F640 @ case 8
	.4byte _0800F698 @ case 9
	.4byte _0800F644 @ case 10
	.4byte _0800F698 @ case 11
	.4byte _0800F648 @ case 12
	.4byte _0800F652 @ case 13
	.4byte _0800F656 @ case 14
	.4byte _0800F698 @ case 15
	.4byte _0800F65A @ case 16
	.4byte _0800F65E @ case 17
	.4byte _0800F67C @ case 18
_0800F628:
	movs r0, #0x1f
	b _0800F660
_0800F62C:
	movs r0, #0x1a
	b _0800F660
_0800F630:
	movs r0, #0x17
	b _0800F660
_0800F634:
	movs r0, #0x18
	b _0800F660
_0800F638:
	movs r0, #4
	b _0800F660
_0800F63C:
	movs r0, #0x20
	b _0800F660
_0800F640:
	movs r0, #2
	b _0800F660
_0800F644:
	movs r0, #5
	b _0800F660
_0800F648:
	movs r0, #0x2a
	rsbs r0, r0, #0
	str r0, [r4, #0x48]
	movs r0, #0x19
	b _0800F660
_0800F652:
	movs r0, #6
	b _0800F660
_0800F656:
	movs r0, #0x11
	b _0800F660
_0800F65A:
	movs r0, #0xe
	b _0800F660
_0800F65E:
	movs r0, #0xf
_0800F660:
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
	b _0800F698
_0800F67C:
	movs r0, #0x10
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
_0800F698:
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_800F6B8
sub_800F6B8: @ 0x0800F6B8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	mov sb, r0
	mov r8, r1
	adds r7, r2, #0
	adds r6, r3, #0
	movs r5, #0
	ldr r0, _0800F748 @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r0, [r0]
	cmp r5, r0
	bge _0800F784
	ldr r0, _0800F74C @ =gStaticData_0816BBC4
	mov sl, r0
_0800F6DA:
	ldr r0, _0800F748 @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r5, #2
	adds r0, r0, r1
	ldr r4, [r0]
	ldr r1, [r4, #0x18]
	adds r1, #0x48
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	cmp r0, #3
	bne _0800F778
	ldr r1, [r4]
	asrs r1, r1, #8
	mov r0, sb
	subs r1, r1, r0
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	ldr r0, [r4, #4]
	asrs r0, r0, #8
	mov r2, r8
	subs r0, r0, r2
	asrs r2, r0, #0x1f
	eors r0, r2
	subs r0, r0, r2
	adds r1, r1, r0
	cmp r1, r7
	bgt _0800F778
	cmp r0, r6
	bge _0800F778
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800F778
	adds r0, r4, #0
	adds r0, #0x4e
	ldrb r1, [r0]
	mov r2, sl
	adds r0, r1, r2
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800F750
	adds r0, r4, #0
	movs r1, #0
	bl sub_800EEF0
	b _0800F778
	.align 2, 0
_0800F748: .4byte gUnknown_0300130C
_0800F74C: .4byte gStaticData_0816BBC4
_0800F750:
	ldr r0, _0800F768 @ =gStaticData_0816BBAE
	adds r0, r1, r0
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800F778
	cmp r1, #1
	bne _0800F76C
	adds r0, r4, #0
	bl sub_800E6B0
	b _0800F778
	.align 2, 0
_0800F768: .4byte gStaticData_0816BBAE
_0800F76C:
	adds r0, r4, #0
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_800E7A8
_0800F778:
	adds r5, #1
	ldr r0, _0800F794 @ =gUnknown_0300130C
	ldr r0, [r0]
	ldr r0, [r0]
	cmp r5, r0
	blt _0800F6DA
_0800F784:
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800F794: .4byte gUnknown_0300130C

	thumb_func_start sub_800F798
sub_800F798: @ 0x0800F798
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	ldr r0, _0800F7C4 @ =gStaticData_0816BBC4
	adds r1, r6, #0
	adds r1, #0x4e
	ldrb r1, [r1]
	adds r0, r1, r0
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800F7D4
	ldr r0, [r6, #0x34]
	cmp r0, #0
	bne _0800F7D4
	ldr r0, [r6, #0x30]
	cmp r0, #3
	bne _0800F7C8
	adds r0, r6, #0
	movs r1, #0x14
	bl sub_800F06C
	b _0800F7D4
	.align 2, 0
_0800F7C4: .4byte gStaticData_0816BBC4
_0800F7C8:
	cmp r0, #6
	bne _0800F7D4
	adds r0, r6, #0
	movs r1, #0x28
	bl sub_800F06C
_0800F7D4:
	adds r0, r6, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800F8C4
	adds r0, r6, #0
	bl sub_8010708
	adds r5, r0, #0
	adds r0, r6, #0
	bl sub_801070C
	adds r4, r0, #0
	cmp r5, #0
	beq _0800F806
	cmp r4, #0
	beq _0800F814
	adds r1, r5, #0
	bl sub_8010710
	adds r0, r5, #0
	adds r1, r4, #0
	bl sub_8010714
	b _0800F820
_0800F806:
	cmp r4, #0
	beq _0800F814
	adds r0, r4, #0
	movs r1, #0
	bl sub_8010710
	b _0800F820
_0800F814:
	cmp r5, #0
	beq _0800F820
	adds r0, r5, #0
	movs r1, #0
	bl sub_8010714
_0800F820:
	adds r0, r6, #0
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #1
	beq _0800F8D4
	ldr r0, _0800F898 @ =gUnknown_030012B0
	movs r1, #1
	strb r1, [r0]
	ldrb r0, [r6, #0xc]
	orrs r1, r0
	strb r1, [r6, #0xc]
	ldr r0, _0800F89C @ =0x0000FFFF
	ldrh r1, [r6, #8]
	cmp r1, r0
	beq _0800F860
	ldrh r3, [r6, #8]
	ldr r0, _0800F8A0 @ =gUnknown_030012B4
	ldr r2, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r1, r0, #2
	movs r4, #0x84
	lsls r4, r4, #1
	adds r2, r2, r4
	adds r2, r2, r1
	lsls r0, r0, #5
	subs r0, r3, r0
	movs r1, #1
	lsls r1, r0
	ldr r0, [r2]
	orrs r0, r1
	str r0, [r2]
_0800F860:
	movs r3, #0
	ldr r1, _0800F8A4 @ =gUnknown_030012D8
	ldr r0, [r1]
	adds r0, #0x94
	ldrb r0, [r0]
	cmp r3, r0
	bge _0800F8D4
	adds r4, r1, #0
_0800F870:
	ldr r2, [r4]
	adds r0, r2, #0
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #0
	bne _0800F8A8
	cmp r3, #4
	ble _0800F88A
	adds r0, r2, #0
	adds r0, #0x94
	ldrb r0, [r0]
	cmp r3, r0
	bge _0800F8A8
_0800F88A:
	lsls r1, r3, #2
	adds r0, r2, #0
	adds r0, #0x98
	adds r0, r0, r1
	ldr r0, [r0]
	b _0800F8AA
	.align 2, 0
_0800F898: .4byte gUnknown_030012B0
_0800F89C: .4byte 0x0000FFFF
_0800F8A0: .4byte gUnknown_030012B4
_0800F8A4: .4byte gUnknown_030012D8
_0800F8A8:
	movs r0, #0
_0800F8AA:
	cmp r0, r6
	bne _0800F8B6
	ldr r0, [r4]
	adds r0, #0x94
	movs r1, #0
	strb r1, [r0]
_0800F8B6:
	adds r3, #1
	ldr r0, [r4]
	adds r0, #0x94
	ldrb r0, [r0]
	cmp r3, r0
	blt _0800F870
	b _0800F8D4
_0800F8C4:
	adds r0, r6, #0
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #1
	beq _0800F8D4
	ldr r1, _0800F8DC @ =gUnknown_030012B0
	movs r0, #1
	strb r0, [r1]
_0800F8D4:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0800F8DC: .4byte gUnknown_030012B0

	thumb_func_start sub_800F8E0
sub_800F8E0: @ 0x0800F8E0
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r6, r4, #0
	adds r6, #0x4f
	ldrb r0, [r6]
	cmp r0, #0
	bne _0800F98A
	adds r5, r4, #0
	adds r5, #0x4e
	ldrb r0, [r5]
	adds r1, r0, #0
	cmp r0, #0x14
	beq _0800F93C
	cmp r0, #0x14
	bgt _0800F904
	cmp r0, #0x13
	beq _0800F974
	b _0800F98A
_0800F904:
	cmp r1, #0x15
	bne _0800F98A
	movs r0, #0x13
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
	ldr r0, _0800F938 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x11
	bl PlaySfx
	movs r0, #0x14
	b _0800F968
	.align 2, 0
_0800F938: .4byte gUnknown_030012BC
_0800F93C:
	movs r0, #0x12
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
	ldr r0, _0800F970 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x11
	bl PlaySfx
	movs r0, #0x13
_0800F968:
	strb r0, [r5]
	movs r0, #0x3c
	strb r0, [r6]
	b _0800F98A
	.align 2, 0
_0800F970: .4byte gUnknown_030012BC
_0800F974:
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0800F98A
	adds r0, r4, #0
	movs r1, #0
	bl sub_800EEF0
_0800F98A:
	pop {r4, r5, r6}
	pop {r0}
	bx r0

	thumb_func_start sub_800F990
sub_800F990: @ 0x0800F990
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r4, r0, #0
	ldr r2, [r4, #0x48]
	movs r0, #0xc0
	ands r0, r2
	cmp r0, #0
	bne _0800F9E2
	ldr r0, _0800FA3C @ =gUnknown_030012D8
	ldr r3, [r0]
	ldr r0, [r3]
	asrs r1, r0, #8
	ldr r0, [r4]
	asrs r0, r0, #8
	subs r1, r1, r0
	cmp r1, #0
	bge _0800F9B8
	rsbs r1, r1, #0
_0800F9B8:
	cmp r1, #0x4f
	bgt _0800F9E2
	ldr r0, [r3, #4]
	asrs r1, r0, #8
	ldr r0, [r4, #4]
	asrs r0, r0, #8
	subs r1, r1, r0
	cmp r1, #0
	bge _0800F9CC
	rsbs r1, r1, #0
_0800F9CC:
	cmp r1, #0x3f
	bgt _0800F9E2
	movs r0, #0x3f
	ands r2, r0
	movs r0, #0x40
	orrs r2, r0
	movs r0, #0xc7
	ands r2, r0
	movs r0, #0x10
	orrs r2, r0
	str r2, [r4, #0x48]
_0800F9E2:
	adds r0, r4, #0
	adds r0, #0x4f
	ldrb r1, [r0]
	mov sb, r0
	cmp r1, #0
	beq _0800F9F0
	b _0800FC5C
_0800F9F0:
	ldr r0, [r4, #0x48]
	movs r1, #7
	ands r1, r0
	movs r2, #4
	ands r1, r2
	adds r2, r0, #0
	adds r5, r4, #0
	adds r5, #0x2d
	cmp r1, #0
	bne _0800FA06
	b _0800FBA0
_0800FA06:
	ldrb r0, [r5]
	cmp r0, #8
	beq _0800FA0E
	b _0800FBA0
_0800FA0E:
	movs r6, #0
	movs r1, #0x29
	adds r1, r1, r4
	mov r8, r1
	movs r7, #7
_0800FA18:
	ldr r1, [r4, #0x48]
	adds r0, r1, #0
	ands r0, r7
	adds r2, r0, #1
	movs r0, #3
	ands r2, r0
	adds r3, r2, #0
	movs r0, #0xf8
	ands r1, r0
	orrs r1, r2
	str r1, [r4, #0x48]
	cmp r2, #1
	beq _0800FAEC
	cmp r2, #1
	bgt _0800FA40
	cmp r2, #0
	beq _0800FA4A
	b _0800FB4E
	.align 2, 0
_0800FA3C: .4byte gUnknown_030012D8
_0800FA40:
	cmp r3, #2
	beq _0800FAFE
	cmp r3, #3
	beq _0800FB26
	b _0800FB4E
_0800FA4A:
	strb r7, [r5]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r4, #0x48]
	movs r6, #0xc0
	ands r0, r6
	cmp r0, #0
	beq _0800FB54
	adds r0, r4, #0
	bl sub_8010A50
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #0
	beq _0800FA88
	subs r2, r0, #1
	lsls r2, r2, #0x18
	ldr r1, [r4, #0x48]
	movs r0, #0xc7
	ands r1, r0
	lsrs r2, r2, #0x15
	orrs r1, r2
	str r1, [r4, #0x48]
_0800FA88:
	ldr r2, [r4, #0x48]
	movs r0, #0x38
	ands r0, r2
	cmp r0, #0
	bne _0800FB54
	movs r1, #0xc7
	ands r1, r2
	movs r0, #0x10
	orrs r1, r0
	str r1, [r4, #0x48]
	adds r0, r1, #0
	ands r0, r6
	lsrs r0, r0, #6
	cmp r0, #2
	beq _0800FAC2
	cmp r0, #2
	bgt _0800FAB0
	cmp r0, #1
	beq _0800FAB6
	b _0800FB54
_0800FAB0:
	cmp r0, #3
	beq _0800FACC
	b _0800FB54
_0800FAB6:
	movs r0, #0x3f
	ands r1, r0
	movs r0, #0x80
	orrs r1, r0
	str r1, [r4, #0x48]
	b _0800FB54
_0800FAC2:
	movs r0, #0x3f
	ands r1, r0
	orrs r1, r6
	str r1, [r4, #0x48]
	b _0800FB54
_0800FACC:
	movs r0, #0x20
	strb r0, [r5]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	adds r0, #0x4e
	strb r7, [r0]
	b _0800FB54
_0800FAEC:
	adds r1, r4, #0
	adds r1, #0x50
	movs r0, #2
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _0800FB4E
	movs r0, #9
	b _0800FB0E
_0800FAFE:
	adds r1, r4, #0
	adds r1, #0x50
	movs r0, #1
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _0800FB4E
	movs r0, #0xb
_0800FB0E:
	strb r0, [r5]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	b _0800FB54
_0800FB26:
	adds r1, r4, #0
	adds r1, #0x50
	movs r0, #4
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _0800FB4E
	movs r0, #0xd
	strb r0, [r5]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	movs r6, #1
_0800FB4E:
	cmp r6, #0
	bne _0800FB54
	b _0800FA18
_0800FB54:
	ldr r0, [r4, #0x20]
	ldr r1, [r0]
	ldrb r2, [r5]
	lsls r0, r2, #3
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r1, r1, r0
	ldr r0, _0800FB98 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldrb r1, [r1, #0x14]
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	mov r3, r8
	ldrb r3, [r3]
	ands r1, r3
	orrs r1, r0
	mov r0, r8
	strb r1, [r0]
	ldr r0, [r4, #0x48]
	movs r1, #0xc0
	ands r0, r1
	lsrs r0, r0, #6
	ldr r1, _0800FB9C @ =gStaticData_0816BB94
	adds r0, r0, r1
	ldrb r0, [r0]
	mov r1, sb
	strb r0, [r1]
	b _0800FC5C
	.align 2, 0
_0800FB98: .4byte gUnknown_030012B8
_0800FB9C: .4byte gStaticData_0816BB94
_0800FBA0:
	movs r1, #7
	ands r1, r2
	movs r0, #4
	orrs r1, r0
	movs r0, #0xf8
	ands r0, r2
	orrs r1, r0
	str r1, [r4, #0x48]
	movs r0, #1
	mov r2, sb
	strb r0, [r2]
	ldrb r0, [r5]
	cmp r0, #0xc
	beq _0800FBE2
	cmp r0, #0xa
	bne _0800FBC4
	movs r0, #8
	b _0800FBE4
_0800FBC4:
	ldr r0, [r4, #0x48]
	movs r1, #0xc0
	ands r0, r1
	lsrs r0, r0, #6
	cmp r0, #2
	beq _0800FBE2
	cmp r0, #2
	ble _0800FBDA
	cmp r0, #3
	beq _0800FBFC
	b _0800FC14
_0800FBDA:
	cmp r0, #0
	blt _0800FC14
	movs r0, #0xc
	b _0800FBE4
_0800FBE2:
	movs r0, #0xa
_0800FBE4:
	strb r0, [r5]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	b _0800FC14
_0800FBFC:
	movs r0, #8
	strb r0, [r5]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
_0800FC14:
	ldr r0, [r4, #0x20]
	ldr r1, [r0]
	ldrb r3, [r5]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	adds r1, r1, r0
	ldr r0, _0800FC68 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldrb r1, [r1, #0x14]
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldr r0, [r4, #0x48]
	movs r1, #0xc0
	ands r0, r1
	cmp r0, #0
	beq _0800FC5C
	ldr r0, _0800FC6C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x10
	bl PlaySfx
_0800FC5C:
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800FC68: .4byte gUnknown_030012B8
_0800FC6C: .4byte gUnknown_030012BC

