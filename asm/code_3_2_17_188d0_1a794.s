.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_801A794
sub_801A794: @ 0x0801A794
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8017A8C
	ldr r0, _0801A7A8 @ =gStaticData_087E490C
	str r0, [r4, #0xc]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801A7A8: .4byte gStaticData_087E490C

	thumb_func_start sub_801A7AC
sub_801A7AC: @ 0x0801A7AC
	push {r4, r5, r6, r7, lr}
	adds r3, r1, #0
	adds r5, r2, #0
	ldr r2, _0801A7E8 @ =gStaticData_0816C418
	lsls r0, r5, #3
	adds r0, r0, r2
	ldr r1, [r0]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801A7EC @ =gStaticData_0816C3B8
	adds r4, r0, r1
	adds r0, r3, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	adds r6, r2, #0
	adds r7, r1, #0
	cmp r0, #0
	bge _0801A7F0
	ldr r0, [r4]
	rsbs r0, r0, #0
	ldr r1, [r4, #8]
	rsbs r1, r1, #0
	ldr r2, [r4, #4]
	str r0, [r3, #0x60]
	str r0, [r3, #0x48]
	str r2, [r3, #0x4c]
	str r1, [r3, #0x50]
	b _0801A7FE
	.align 2, 0
_0801A7E8: .4byte gStaticData_0816C418
_0801A7EC: .4byte gStaticData_0816C3B8
_0801A7F0:
	ldr r0, [r4]
	ldr r1, [r4, #4]
	ldr r2, [r4, #8]
	str r0, [r3, #0x60]
	str r0, [r3, #0x48]
	str r1, [r3, #0x4c]
	str r2, [r3, #0x50]
_0801A7FE:
	lsls r0, r5, #3
	adds r1, r6, #4
	adds r0, r0, r1
	ldr r1, [r0]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r7
	ldr r1, [r0]
	ldr r2, [r0, #4]
	ldr r0, [r0, #8]
	str r1, [r3, #0x64]
	str r1, [r3, #0x54]
	str r2, [r3, #0x58]
	str r0, [r3, #0x5c]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_801A824
sub_801A824: @ 0x0801A824
	push {lr}
	ldr r2, _0801A834 @ =gStaticData_087E4974
	str r2, [r0, #0xc]
	bl sub_8017A78
	pop {r0}
	bx r0
	.align 2, 0
_0801A834: .4byte gStaticData_087E4974

	thumb_func_start sub_801A838
sub_801A838: @ 0x0801A838
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r6, r0, #0
	adds r4, r1, #0
	adds r5, r2, #0
	bl sub_8017A8C
	ldr r0, _0801A86C @ =gStaticData_087E4974
	str r0, [r6, #0xc]
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	movs r0, #0
	str r0, [sp]
	adds r0, r6, #0
	movs r1, #0
	adds r2, r4, #0
	adds r3, r5, #0
	bl sub_8019EBC
	adds r0, r6, #0
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0801A86C: .4byte gStaticData_087E4974

	thumb_func_start sub_801A870
sub_801A870: @ 0x0801A870
	str r1, [r0, #0x1c]
	bx lr

	thumb_func_start sub_801A874
sub_801A874: @ 0x0801A874
	str r1, [r0, #0x24]
	bx lr

	thumb_func_start sub_801A878
sub_801A878: @ 0x0801A878
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	mov sb, r0
	adds r5, r1, #0
	adds r6, r2, #0
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	mov sb, r0
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	lsls r6, r6, #0x10
	lsrs r6, r6, #0x10
	mov r1, r8
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	mov r8, r1
	movs r0, #0x80
	bl sub_8026EDC
	adds r4, r0, #0
	bl sub_8009F90
	ldr r0, _0801A8F4 @ =gStaticData_087E49DC
	str r0, [r4, #0x18]
	adds r0, r4, #0
	bl sub_801B2D8
	adds r7, r4, #0
	mov r2, sb
	strh r2, [r7, #8]
	lsls r5, r5, #8
	str r5, [r7]
	lsls r6, r6, #8
	str r6, [r7, #4]
	ldr r0, _0801A8F8 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r3, r8
	lsls r3, r3, #1
	mov r8, r3
	add r8, r0
	ldr r0, [r1, #0xc]
	mov r4, r8
	ldrh r5, [r4]
	adds r5, r5, r0
	mov r8, r5
	ldr r2, [r5, #4]
	ldr r0, [sp, #0x24]
	subs r0, #3
	cmp r0, #9
	bhi _0801A93A
	lsls r0, r0, #2
	ldr r1, _0801A8FC @ =_0801A900
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801A8F4: .4byte gStaticData_087E49DC
_0801A8F8: .4byte gUnknown_030012B4
_0801A8FC: .4byte _0801A900
_0801A900: @ jump table
	.4byte _0801A934 @ case 0
	.4byte _0801A928 @ case 1
	.4byte _0801A92C @ case 2
	.4byte _0801A938 @ case 3
	.4byte _0801A93A @ case 4
	.4byte _0801A930 @ case 5
	.4byte _0801A934 @ case 6
	.4byte _0801A934 @ case 7
	.4byte _0801A934 @ case 8
	.4byte _0801A934 @ case 9
_0801A928:
	movs r2, #2
	b _0801A93A
_0801A92C:
	movs r2, #3
	b _0801A93A
_0801A930:
	movs r2, #7
	b _0801A93A
_0801A934:
	movs r2, #4
	b _0801A93A
_0801A938:
	movs r2, #6
_0801A93A:
	cmp r2, #7
	bls _0801A940
	b _0801AA9C
_0801A940:
	lsls r0, r2, #2
	ldr r1, _0801A94C @ =_0801A950
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801A94C: .4byte _0801A950
_0801A950: @ jump table
	.4byte _0801A970 @ case 0
	.4byte _0801A976 @ case 1
	.4byte _0801A9D4 @ case 2
	.4byte _0801A9DA @ case 3
	.4byte _0801A9EE @ case 4
	.4byte _0801A9F4 @ case 5
	.4byte _0801AA1C @ case 6
	.4byte _0801AA6C @ case 7
_0801A970:
	movs r0, #0
	str r0, [r7, #0x78]
	b _0801AA9C
_0801A976:
	movs r6, #1
	str r6, [r7, #0x78]
	movs r0, #0x38
	bl sub_8026EDC
	mov r2, r8
	ldr r1, [r2, #8]
	ldr r2, [r2, #0xc]
	mov r3, r8
	movs r5, #0x10
	ldrsh r4, [r3, r5]
	rsbs r3, r4, #0
	orrs r3, r4
	lsrs r3, r3, #0x1f
	mov sb, r3
	mov r3, r8
	movs r4, #0x12
	ldrsh r5, [r3, r4]
	rsbs r4, r5, #0
	orrs r4, r5
	lsrs r4, r4, #0x1f
	mov r5, sp
	strb r4, [r5]
	str r6, [sp, #4]
	mov r3, sb
	bl sub_801B7D8
	adds r2, r0, #0
	str r2, [r7, #0x44]
	ldr r1, [r2, #0xc]
	movs r5, #0x18
	ldrsh r0, [r1, r5]
	adds r0, r2, r0
	ldr r2, [r1, #0x1c]
	adds r1, r7, #0
	bl sub_803AD80
	mov r1, r8
	movs r2, #0x14
	ldrsh r0, [r1, r2]
	cmp r0, #0
	beq _0801AA9C
	movs r0, #0x10
	ldrb r3, [r7, #0xc]
	orrs r0, r3
	strb r0, [r7, #0xc]
	b _0801AA9C
_0801A9D4:
	movs r0, #2
	str r0, [r7, #0x78]
	b _0801AA9C
_0801A9DA:
	movs r0, #3
	str r0, [r7, #0x78]
	movs r1, #1
	mov r4, r8
	ldrb r4, [r4]
	ands r1, r4
	adds r0, r7, #0
	bl sub_801B2A8
	b _0801AA9C
_0801A9EE:
	movs r0, #4
	str r0, [r7, #0x78]
	b _0801AA9C
_0801A9F4:
	movs r4, #5
	str r4, [r7, #0x78]
	movs r0, #0x38
	bl sub_8026EDC
	mov r2, sp
	movs r1, #0
	strb r1, [r2]
	str r4, [sp, #4]
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_801B7D8
	adds r2, r0, #0
	str r2, [r7, #0x44]
	ldr r1, [r2, #0xc]
	movs r5, #0x18
	ldrsh r0, [r1, r5]
	b _0801AA60
_0801AA1C:
	movs r4, #6
	str r4, [r7, #0x78]
	ldr r0, _0801AA48 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80233B4
	cmp r0, #1
	beq _0801AA4C
	movs r0, #0x38
	bl sub_8026EDC
	mov r2, sp
	movs r1, #0
	strb r1, [r2]
	str r4, [sp, #4]
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_801B7D8
	b _0801AA56
	.align 2, 0
_0801AA48: .4byte gUnknown_030012C0
_0801AA4C:
	movs r0, #0x38
	bl sub_8026EDC
	bl sub_801961C
_0801AA56:
	adds r2, r0, #0
	str r2, [r7, #0x44]
	ldr r1, [r2, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
_0801AA60:
	adds r0, r2, r0
	ldr r2, [r1, #0x1c]
	adds r1, r7, #0
	bl sub_803AD80
	b _0801AA9C
_0801AA6C:
	movs r4, #7
	str r4, [r7, #0x78]
	movs r0, #0x38
	bl sub_8026EDC
	mov r2, sp
	movs r1, #0
	strb r1, [r2]
	str r4, [sp, #4]
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_801B7D8
	adds r2, r0, #0
	str r2, [r7, #0x44]
	ldr r1, [r2, #0xc]
	movs r4, #0x18
	ldrsh r0, [r1, r4]
	adds r0, r2, r0
	ldr r2, [r1, #0x1c]
	adds r1, r7, #0
	bl sub_803AD80
_0801AA9C:
	ldr r0, _0801AB28 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r7, #0
	bl sub_8008E94
	ldr r0, _0801AB2C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r5, #0xea
	lsls r5, r5, #1
	adds r0, r0, r5
	str r0, [r7, #0x20]
	adds r4, r7, #0
	adds r4, #0x2d
	add r0, sp, #0x24
	ldrb r0, [r0]
	strb r0, [r4]
	adds r0, r7, #0
	bl sub_80087C0
	adds r0, r7, #0
	bl sub_80087B4
	adds r0, r7, #0
	movs r1, #0
	bl sub_800872C
	adds r2, r7, #0
	adds r2, #0x28
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r1, [r2]
	ands r0, r1
	movs r1, #0x21
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r2]
	ldr r0, [r7, #0x20]
	ldr r1, [r0]
	ldrb r2, [r4]
	lsls r0, r2, #3
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r1, r1, r0
	ldr r0, _0801AB30 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldrb r1, [r1, #0x14]
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	adds r2, r7, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	adds r0, r7, #0
	add sp, #8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0801AB28: .4byte gUnknown_030012EC
_0801AB2C: .4byte gUnknown_030012D0
_0801AB30: .4byte gUnknown_030012B8

	thumb_func_start sub_801AB34
sub_801AB34: @ 0x0801AB34
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x78]
	cmp r0, #6
	bne _0801AB44
	ldr r0, [r4, #0x30]
	cmp r0, #0x12
	bgt _0801AB88
_0801AB44:
	ldr r0, _0801AB90 @ =gUnknown_030012D8
	ldr r3, [r0]
	ldr r0, [r3, #0x44]
	ldr r5, [r0, #8]
	ldrb r1, [r3, #0xc]
	lsrs r0, r1, #7
	cmp r0, #0
	beq _0801AB7E
	ldr r2, [r4]
	ldr r0, [r3]
	subs r2, r2, r0
	cmp r2, #0
	bge _0801AB60
	rsbs r2, r2, #0
_0801AB60:
	ldr r1, _0801AB94 @ =0x00007FFF
	cmp r2, r1
	bgt _0801AB7E
	ldr r2, [r4, #4]
	ldr r0, [r3, #4]
	subs r2, r2, r0
	cmp r2, #0
	bge _0801AB72
	rsbs r2, r2, #0
_0801AB72:
	cmp r2, r1
	bgt _0801AB7E
	adds r0, r4, #0
	adds r1, r5, #0
	bl sub_801AB98
_0801AB7E:
	movs r0, #9
	rsbs r0, r0, #0
	ldrb r1, [r4, #0xc]
	ands r0, r1
	strb r0, [r4, #0xc]
_0801AB88:
	movs r0, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0801AB90: .4byte gUnknown_030012D8
_0801AB94: .4byte 0x00007FFF

	thumb_func_start sub_801AB98
sub_801AB98: @ 0x0801AB98
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x44
	mov sb, r0
	add r0, sp, #4
	mov r1, sb
	bl sub_8007B98
	ldr r0, _0801AC3C @ =gUnknown_030012D8
	ldr r1, [r0]
	ldr r0, [r1]
	asrs r5, r0, #8
	ldr r0, [r1, #4]
	asrs r0, r0, #8
	mov sl, r0
	add r4, sp, #0x14
	adds r0, r4, #0
	bl sub_8007B98
	ldr r1, _0801AC3C @ =gUnknown_030012D8
	ldr r0, [r1]
	ldr r2, [r0, #0x20]
	adds r0, #0x2d
	ldrb r3, [r0]
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	ldr r0, [r2]
	adds r0, r0, r1
	adds r7, r0, #4
	add r0, sp, #4
	adds r1, r4, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801ABEA
	b _0801B078
_0801ABEA:
	movs r0, #0
	mov r8, r0
	movs r1, #0
	str r1, [sp, #0x40]
	ldr r1, [sp, #0x18]
	ldr r0, [sp, #8]
	cmp r1, r0
	bge _0801ABFE
	movs r2, #1
	str r2, [sp, #0x40]
_0801ABFE:
	ldr r3, _0801AC3C @ =gUnknown_030012D8
	ldr r0, [r3]
	bl sub_8009EC4
	adds r4, r0, #0
	ldr r1, _0801AC3C @ =gUnknown_030012D8
	ldr r0, [r1]
	bl sub_8009EBC
	adds r6, r0, #0
	movs r2, #2
	str r2, [sp, #0x3c]
	cmp r5, r4
	ble _0801AC1E
	movs r3, #1
	str r3, [sp, #0x3c]
_0801AC1E:
	ldr r1, _0801AC3C @ =gUnknown_030012D8
	ldr r0, [r1]
	ldr r1, [r0]
	asrs r1, r1, #8
	mov r2, sb
	ldr r0, [r2]
	asrs r0, r0, #8
	cmp r1, r0
	bge _0801AC40
	movs r3, #1
	str r3, [sp, #0x34]
	ldr r0, [sp, #0x14]
	ldr r1, [sp, #0x1c]
	ldr r2, [sp, #4]
	b _0801AC4A
	.align 2, 0
_0801AC3C: .4byte gUnknown_030012D8
_0801AC40:
	movs r0, #2
	str r0, [sp, #0x34]
	ldr r0, [sp, #4]
	ldr r1, [sp, #0xc]
	ldr r2, [sp, #0x14]
_0801AC4A:
	adds r0, r0, r1
	subs r0, r0, r2
	adds r0, #1
	str r0, [sp, #0x2c]
	ldr r0, _0801AC70 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r1, [r0, #4]
	asrs r1, r1, #8
	mov r2, sb
	ldr r0, [r2, #4]
	asrs r0, r0, #8
	cmp r1, r0
	ble _0801AC74
	movs r3, #4
	str r3, [sp, #0x38]
	ldr r0, [sp, #8]
	ldr r1, [sp, #0x10]
	ldr r2, [sp, #0x18]
	b _0801AC7E
	.align 2, 0
_0801AC70: .4byte gUnknown_030012D8
_0801AC74:
	movs r0, #8
	str r0, [sp, #0x38]
	ldr r0, [sp, #0x18]
	ldr r1, [sp, #0x20]
	ldr r2, [sp, #8]
_0801AC7E:
	adds r0, r0, r1
	subs r0, r0, r2
	str r0, [sp, #0x30]
	mov r1, sb
	ldr r0, [r1, #0x78]
	cmp r0, #1
	beq _0801ACE0
	cmp r0, #5
	beq _0801ACE0
	cmp r0, #6
	beq _0801ACE0
	cmp r6, sl
	bne _0801ACC8
	cmp r4, r5
	bne _0801ACAC
	ldr r2, [sp, #0x34]
	mov r8, r2
	ldr r3, [sp, #0x30]
	cmp r3, #2
	bgt _0801ACE0
	movs r5, #8
	mov r8, r5
	b _0801ACE0
_0801ACAC:
	mov r0, sb
	bl sub_8009EBC
	mov r2, sb
	ldr r1, [r2, #4]
	asrs r1, r1, #8
	cmp r0, r1
	bne _0801ACC8
	ldr r3, [sp, #0x30]
	cmp r3, #2
	ble _0801ACC8
	ldr r5, [sp, #0x34]
	mov r8, r5
	b _0801ACE0
_0801ACC8:
	cmp r4, r5
	bne _0801ACE0
	mov r0, sb
	bl sub_8009EC4
	mov r2, sb
	ldr r1, [r2]
	asrs r1, r1, #8
	cmp r0, r1
	bne _0801ACE0
	ldr r3, [sp, #0x38]
	mov r8, r3
_0801ACE0:
	cmp r6, sl
	bgt _0801ADA8
	ldr r5, [sp, #0x40]
	cmp r5, #0
	beq _0801ADA8
	mov r0, r8
	cmp r0, #0
	beq _0801ACF2
	b _0801AE60
_0801ACF2:
	movs r1, #2
	ldrsh r0, [r7, r1]
	ldrb r2, [r7, #5]
	adds r0, r0, r2
	adds r6, r6, r0
	ldr r0, [sp, #8]
	ldr r1, [sp, #0x10]
	adds r0, r0, r1
	adds r3, r2, #0
	cmp r6, r0
	ble _0801AD0A
	b _0801AE5C
_0801AD0A:
	ldr r2, [sp, #0x3c]
	cmp r2, #1
	bne _0801AD2A
	ldr r1, [sp, #0x14]
	ldr r0, [sp, #0x1c]
	adds r0, r1, r0
	adds r2, r1, #0
	ldr r1, [sp, #4]
	cmp r0, r1
	blt _0801AD40
	ldr r5, [sp, #0x2c]
	cmp r5, #2
	ble _0801AD40
	movs r0, #8
	mov r8, r0
	b _0801AE60
_0801AD2A:
	ldr r1, [sp, #4]
	ldr r0, [sp, #0xc]
	adds r0, r1, r0
	ldr r2, [sp, #0x14]
	cmp r2, r0
	bgt _0801AD40
	ldr r5, [sp, #0x2c]
	cmp r5, #2
	ble _0801AD40
	movs r0, #8
	mov r8, r0
_0801AD40:
	mov r5, r8
	cmp r5, #0
	beq _0801AD48
	b _0801AE60
_0801AD48:
	movs r5, #2
	ldrsh r0, [r7, r5]
	adds r0, r0, r3
	add sl, r0
	ldr r0, [sp, #0x34]
	cmp r0, #1
	bne _0801AD6E
	movs r3, #0
	ldrsh r0, [r7, r3]
	ldrb r7, [r7, #4]
	adds r0, r7, r0
	adds r4, r4, r0
	ldr r0, [sp, #0x1c]
	adds r5, r2, r0
	str r1, [sp]
	adds r0, r4, #0
	adds r1, r6, #0
	adds r2, r5, #0
	b _0801AD80
_0801AD6E:
	movs r5, #0
	ldrsh r0, [r7, r5]
	adds r4, r4, r0
	adds r5, r2, #0
	ldr r0, [sp, #0xc]
	adds r0, r1, r0
	str r0, [sp]
	adds r0, r4, #0
	adds r1, r6, #0
_0801AD80:
	mov r3, sl
	bl sub_800FDC8
	adds r2, r0, #0
	cmp r2, #0
	bge _0801AD98
	ldr r0, [sp, #0x40]
	cmp r0, #0
	beq _0801AD98
	ldr r1, [sp, #0x30]
	cmp r1, #1
	ble _0801ADA2
_0801AD98:
	cmp r2, #0
	ble _0801AE56
	ldr r0, [sp, #8]
	cmp r2, r0
	bgt _0801AE56
_0801ADA2:
	movs r2, #8
	mov r8, r2
	b _0801AE60
_0801ADA8:
	mov r0, r8
	cmp r0, #0
	bne _0801AE60
	movs r1, #2
	ldrsh r0, [r7, r1]
	adds r6, r6, r0
	ldr r0, [sp, #8]
	cmp r6, r0
	blt _0801AE5C
	ldr r2, [sp, #0x3c]
	cmp r2, #1
	bne _0801ADD8
	ldr r1, [sp, #0x14]
	ldr r0, [sp, #0x1c]
	adds r0, r1, r0
	adds r2, r1, #0
	ldr r1, [sp, #4]
	cmp r0, r1
	blt _0801ADEE
	ldr r3, [sp, #0x2c]
	cmp r3, #3
	ble _0801ADEE
	movs r5, #4
	b _0801AE5E
_0801ADD8:
	ldr r1, [sp, #4]
	ldr r0, [sp, #0xc]
	adds r0, r1, r0
	ldr r2, [sp, #0x14]
	cmp r2, r0
	bgt _0801ADEE
	ldr r0, [sp, #0x2c]
	cmp r0, #3
	ble _0801ADEE
	movs r3, #4
	mov r8, r3
_0801ADEE:
	mov r5, r8
	cmp r5, #0
	bne _0801AE60
	movs r3, #2
	ldrsh r0, [r7, r3]
	add sl, r0
	ldr r5, [sp, #0x34]
	cmp r5, #1
	bne _0801AE18
	movs r3, #0
	ldrsh r0, [r7, r3]
	ldrb r7, [r7, #4]
	adds r0, r7, r0
	adds r4, r4, r0
	ldr r0, [sp, #0x1c]
	adds r5, r2, r0
	str r1, [sp]
	adds r0, r4, #0
	adds r1, r6, #0
	adds r2, r5, #0
	b _0801AE2A
_0801AE18:
	movs r5, #0
	ldrsh r0, [r7, r5]
	adds r4, r4, r0
	adds r5, r2, #0
	ldr r0, [sp, #0xc]
	adds r0, r1, r0
	str r0, [sp]
	adds r0, r4, #0
	adds r1, r6, #0
_0801AE2A:
	mov r3, sl
	bl sub_800FDC8
	adds r2, r0, #0
	cmp r2, #0
	bge _0801AE42
	ldr r0, [sp, #0x40]
	cmp r0, #0
	bne _0801AE42
	ldr r1, [sp, #0x2c]
	cmp r1, #3
	bgt _0801AE50
_0801AE42:
	cmp r2, #0
	ble _0801AE56
	ldr r0, [sp, #8]
	ldr r1, [sp, #0x10]
	adds r0, r0, r1
	cmp r2, r0
	blt _0801AE56
_0801AE50:
	movs r2, #4
	mov r8, r2
	b _0801AE60
_0801AE56:
	ldr r3, [sp, #0x34]
	mov r8, r3
	b _0801AE60
_0801AE5C:
	ldr r5, [sp, #0x34]
_0801AE5E:
	mov r8, r5
_0801AE60:
	ldr r2, _0801AE98 @ =gUnknown_030012D8
	ldr r1, [r2]
	ldr r0, [r1]
	str r0, [sp, #0x24]
	ldr r0, [r1, #4]
	add r1, sp, #0x24
	str r0, [r1, #4]
	mov sl, r2
	adds r7, r1, #0
	ldr r0, [sp, #0x2c]
	cmp r0, #0
	bge _0801AE7C
	movs r1, #0
	str r1, [sp, #0x2c]
_0801AE7C:
	ldr r2, [sp, #0x30]
	cmp r2, #0
	bge _0801AE86
	movs r3, #0
	str r3, [sp, #0x30]
_0801AE86:
	movs r5, #0
	mov r0, r8
	cmp r0, #8
	bhi _0801AF3C
	lsls r0, r0, #2
	ldr r1, _0801AE9C @ =_0801AEA0
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801AE98: .4byte gUnknown_030012D8
_0801AE9C: .4byte _0801AEA0
_0801AEA0: @ jump table
	.4byte _0801AF32 @ case 0
	.4byte _0801AF14 @ case 1
	.4byte _0801AF14 @ case 2
	.4byte _0801AF32 @ case 3
	.4byte _0801AEC4 @ case 4
	.4byte _0801AF32 @ case 5
	.4byte _0801AF32 @ case 6
	.4byte _0801AF32 @ case 7
	.4byte _0801AEFC @ case 8
_0801AEC4:
	ldr r0, _0801AEF8 @ =gUnknown_030012D8
	ldr r2, [r0]
	adds r1, r2, #0
	adds r1, #0x68
	movs r0, #8
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0801AF32
	ldr r1, [r2, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xc
	movs r3, #4
	bl sub_803AD88
	ldr r1, [sp, #0x30]
	lsls r0, r1, #8
	ldr r1, [r7, #4]
	adds r0, r0, r1
	str r0, [r7, #4]
	b _0801AF32
	.align 2, 0
_0801AEF8: .4byte gUnknown_030012D8
_0801AEFC:
	ldr r0, [sp, #0x30]
	subs r0, #1
	lsls r0, r0, #8
	ldr r1, [r7, #4]
	subs r1, r1, r0
	ldr r0, _0801AF10 @ =0xFFFFFF00
	ands r1, r0
	str r1, [r7, #4]
	b _0801AF32
	.align 2, 0
_0801AF10: .4byte 0xFFFFFF00
_0801AF14:
	ldr r5, [sp, #0x34]
	cmp r5, #2
	bne _0801AF24
	ldr r2, [sp, #0x2c]
	lsls r0, r2, #8
	ldr r1, [sp, #0x24]
	adds r0, r0, r1
	b _0801AF30
_0801AF24:
	cmp r5, #1
	bne _0801AF32
	ldr r3, [sp, #0x2c]
	lsls r1, r3, #8
	ldr r0, [sp, #0x24]
	subs r0, r0, r1
_0801AF30:
	str r0, [sp, #0x24]
_0801AF32:
	ldr r0, _0801AFD0 @ =gUnknown_030012D8
	mov sl, r0
	mov r1, r8
	cmp r1, #8
	beq _0801AF42
_0801AF3C:
	ldr r2, [sp, #0x30]
	cmp r2, #1
	bgt _0801AF7C
_0801AF42:
	mov r3, sl
	ldr r2, [r3]
	adds r1, r2, #0
	adds r1, #0x24
	movs r0, #4
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	bne _0801AF7C
	ldr r0, [sp, #0x40]
	cmp r0, #0
	beq _0801AF7C
	adds r0, r2, #0
	adds r0, #0xac
	mov r1, sb
	str r1, [r0]
	movs r1, #8
	subs r0, #0x44
	strb r1, [r0]
	ldr r2, [r3]
	ldr r1, [r2, #4]
	ldr r0, [sp, #0x30]
	subs r0, #1
	lsls r0, r0, #8
	subs r1, r1, r0
	str r1, [r7, #4]
	movs r5, #0
	ldr r0, [r2]
	str r0, [sp, #0x24]
_0801AF7C:
	mov r6, sl
	ldr r0, [r6]
	ldr r1, [sp, #0x24]
	ldr r2, [r7, #4]
	bl sub_8007398
	cmp r5, #0
	beq _0801AFAC
	ldr r0, [r6]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xc
	adds r3, r5, #0
	bl sub_803AD88
	ldr r1, [r6]
	ldr r0, [r1, #0x74]
	orrs r0, r5
	str r0, [r1, #0x74]
_0801AFAC:
	mov r5, r8
	cmp r5, #8
	beq _0801AFB4
	b _0801B1F8
_0801AFB4:
	mov r0, sb
	ldr r2, [r0, #0x78]
	cmp r2, #1
	beq _0801AFC4
	cmp r2, #5
	beq _0801AFC4
	cmp r2, #6
	bne _0801AFD4
_0801AFC4:
	mov r1, sb
	ldr r0, [r1, #0x44]
	adds r0, #0x32
	movs r1, #1
	b _0801B1F6
	.align 2, 0
_0801AFD0: .4byte gUnknown_030012D8
_0801AFD4:
	mov r3, sb
	ldr r0, [r3]
	asrs r0, r0, #8
	ldr r3, [r6]
	ldr r1, [r3]
	asrs r1, r1, #8
	subs r0, r0, r1
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	cmp r0, #7
	ble _0801AFEE
	b _0801B1F8
_0801AFEE:
	cmp r2, #3
	beq _0801B014
	cmp r2, #3
	bgt _0801AFFC
	cmp r2, #2
	beq _0801B002
	b _0801B1F8
_0801AFFC:
	cmp r2, #4
	beq _0801B048
	b _0801B1F8
_0801B002:
	ldr r1, [r3, #0x18]
	adds r1, #0x68
	movs r5, #0
	ldrsh r0, [r1, r5]
	adds r0, r3, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0x11
	b _0801B1D0
_0801B014:
	ldr r4, _0801B044 @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_80232A0
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801B024
	b _0801B1F8
_0801B024:
	ldr r0, [r4]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801B030
	b _0801B1F8
_0801B030:
	ldr r0, [r6]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xf
	b _0801B1D0
	.align 2, 0
_0801B044: .4byte gUnknown_030012C0
_0801B048:
	ldr r4, _0801B074 @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_8023278
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801B058
	b _0801B1F8
_0801B058:
	ldr r0, [r4]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801B064
	b _0801B1F8
_0801B064:
	mov r5, sl
	ldr r0, [r5]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	b _0801B1CA
	.align 2, 0
_0801B074: .4byte gUnknown_030012C0
_0801B078:
	ldr r0, [sp, #8]
	subs r0, #4
	str r0, [sp, #8]
	ldr r0, [sp, #0x10]
	adds r0, #4
	str r0, [sp, #0x10]
	mov r5, sb
	ldr r0, [r5, #0x78]
	cmp r0, #7
	bls _0801B08E
	b _0801B1F8
_0801B08E:
	lsls r0, r0, #2
	ldr r1, _0801B098 @ =_0801B09C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801B098: .4byte _0801B09C
_0801B09C: @ jump table
	.4byte _0801B0BC @ case 0
	.4byte _0801B1E0 @ case 1
	.4byte _0801B0E4 @ case 2
	.4byte _0801B124 @ case 3
	.4byte _0801B180 @ case 4
	.4byte _0801B1E0 @ case 5
	.4byte _0801B1E0 @ case 6
	.4byte _0801B0BC @ case 7
_0801B0BC:
	add r0, sp, #4
	adds r1, r4, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801B0CC
	b _0801B1F8
_0801B0CC:
	ldr r0, _0801B0E0 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r1, r0, #0
	adds r1, #0xac
	mov r2, sb
	str r2, [r1]
	movs r1, #8
	adds r0, #0x68
	b _0801B1F6
	.align 2, 0
_0801B0E0: .4byte gUnknown_030012D8
_0801B0E4:
	add r0, sp, #4
	adds r1, r4, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801B0F4
	b _0801B1F8
_0801B0F4:
	mov r3, sb
	ldr r1, [r3]
	asrs r1, r1, #8
	ldr r0, _0801B120 @ =gUnknown_030012D8
	ldr r2, [r0]
	ldr r0, [r2]
	asrs r0, r0, #8
	subs r1, r1, r0
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	cmp r1, #7
	bgt _0801B1F8
	ldr r1, [r2, #0x18]
	adds r1, #0x68
	movs r5, #0
	ldrsh r0, [r1, r5]
	adds r0, r2, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0x11
	b _0801B1D0
	.align 2, 0
_0801B120: .4byte gUnknown_030012D8
_0801B124:
	ldr r5, _0801B178 @ =gUnknown_030012C0
	ldr r0, [r5]
	bl sub_80232A0
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801B1F8
	ldr r0, [r5]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0801B1F8
	add r0, sp, #4
	adds r1, r4, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801B1F8
	mov r0, sb
	ldr r1, [r0]
	asrs r1, r1, #8
	ldr r0, _0801B17C @ =gUnknown_030012D8
	ldr r2, [r0]
	ldr r0, [r2]
	asrs r0, r0, #8
	subs r1, r1, r0
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	cmp r1, #7
	bgt _0801B1F8
	ldr r1, [r2, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0xf
	b _0801B1D0
	.align 2, 0
_0801B178: .4byte gUnknown_030012C0
_0801B17C: .4byte gUnknown_030012D8
_0801B180:
	ldr r5, _0801B1D8 @ =gUnknown_030012C0
	ldr r0, [r5]
	bl sub_8023278
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0801B1F8
	ldr r0, [r5]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0801B1F8
	add r0, sp, #4
	adds r1, r4, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801B1F8
	mov r5, sb
	ldr r1, [r5]
	asrs r1, r1, #8
	ldr r0, _0801B1DC @ =gUnknown_030012D8
	ldr r2, [r0]
	ldr r0, [r2]
	asrs r0, r0, #8
	subs r1, r1, r0
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	cmp r1, #7
	bgt _0801B1F8
	ldr r1, [r2, #0x18]
	adds r1, #0x68
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
_0801B1CA:
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0x10
_0801B1D0:
	movs r3, #0
	bl sub_803AD88
	b _0801B1F8
	.align 2, 0
_0801B1D8: .4byte gUnknown_030012C0
_0801B1DC: .4byte gUnknown_030012D8
_0801B1E0:
	add r0, sp, #4
	adds r1, r4, #0
	bl sub_8001688
	lsls r0, r0, #0x18
	lsrs r1, r0, #0x18
	cmp r1, #0
	bne _0801B1F8
	mov r5, sb
	ldr r0, [r5, #0x44]
	adds r0, #0x32
_0801B1F6:
	strb r1, [r0]
_0801B1F8:
	add sp, #0x44
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_801B208
sub_801B208: @ 0x0801B208
	push {r4, lr}
	adds r4, r0, #0
	ldr r1, [r4, #0x18]
	movs r2, #0x38
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #0x3c]
	bl sub_803AD7C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801B270
	adds r0, r4, #0
	bl sub_8008044
	ldr r1, [r4, #0x18]
	adds r1, #0x60
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	ldr r0, [r4, #0x78]
	cmp r0, #6
	bne _0801B254
	ldr r0, [r4, #0x30]
	cmp r0, #0x12
	ble _0801B254
	ldr r0, _0801B26C @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r1, r0, #0
	adds r1, #0xac
	ldr r0, [r1]
	cmp r0, r4
	bne _0801B254
	movs r0, #0
	str r0, [r1]
_0801B254:
	ldr r2, [r4, #0x44]
	cmp r2, #0
	beq _0801B296
	ldr r1, [r2, #0xc]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	adds r1, r4, #0
	bl sub_803AD80
	b _0801B296
	.align 2, 0
_0801B26C: .4byte gUnknown_030012D8
_0801B270:
	ldr r1, [r4, #0x18]
	adds r1, #0x60
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	ldr r2, [r4, #0x44]
	cmp r2, #0
	beq _0801B296
	ldr r1, [r2, #0xc]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	adds r1, r4, #0
	bl sub_803AD80
_0801B296:
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_801B29C
sub_801B29C: @ 0x0801B29C
	ldrb r0, [r0, #0xd]
	lsrs r0, r0, #4
	movs r1, #1
	ands r0, r1
	bx lr
	.align 2, 0

	thumb_func_start sub_801B2A8
sub_801B2A8: @ 0x0801B2A8
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	movs r2, #1
	ands r1, r2
	lsls r1, r1, #4
	movs r2, #0x11
	rsbs r2, r2, #0
	ldrb r3, [r0, #0xd]
	ands r2, r3
	orrs r2, r1
	strb r2, [r0, #0xd]
	bx lr

	thumb_func_start sub_801B2C0
sub_801B2C0: @ 0x0801B2C0
	movs r0, #4
	bx lr

	thumb_func_start sub_801B2C4
sub_801B2C4: @ 0x0801B2C4
	push {lr}
	ldr r2, _0801B2D4 @ =gStaticData_087E49DC
	str r2, [r0, #0x18]
	bl sub_8009F1C
	pop {r0}
	bx r0
	.align 2, 0
_0801B2D4: .4byte gStaticData_087E49DC

	thumb_func_start sub_801B2D8
sub_801B2D8: @ 0x0801B2D8
	movs r1, #0x41
	rsbs r1, r1, #0
	ldrb r2, [r0, #0xc]
	ands r1, r2
	strb r1, [r0, #0xc]
	bx lr

	thumb_func_start sub_801B2E4
sub_801B2E4: @ 0x0801B2E4
	push {r4, lr}
	adds r4, r0, #0
	bl sub_8009F90
	ldr r0, _0801B300 @ =gStaticData_087E49DC
	str r0, [r4, #0x18]
	adds r0, r4, #0
	bl sub_801B2D8
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801B300: .4byte gStaticData_087E49DC

	thumb_func_start sub_801B304
sub_801B304: @ 0x0801B304
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	adds r4, r1, #0
	ldr r0, [r6, #0x20]
	cmp r0, #0
	bne _0801B35E
	ldr r0, [r6, #0x28]
	cmp r0, #0
	ble _0801B35E
	ldr r0, [r4]
	asrs r0, r0, #8
	str r0, [r6, #0x20]
	ldr r0, [r6, #4]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801B348 @ =gStaticData_0816C460
	adds r3, r0, r1
	adds r0, r6, #0
	adds r0, #0x30
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801B34C
	ldr r0, [r3]
	ldr r1, [r3, #4]
	ldr r2, [r3, #8]
	str r0, [r4, #0x60]
	str r0, [r4, #0x48]
	str r1, [r4, #0x4c]
	str r2, [r4, #0x50]
	b _0801B35E
	.align 2, 0
_0801B348: .4byte gStaticData_0816C460
_0801B34C:
	ldr r0, [r3]
	rsbs r0, r0, #0
	ldr r1, [r3, #8]
	rsbs r1, r1, #0
	ldr r2, [r3, #4]
	str r0, [r4, #0x60]
	str r0, [r4, #0x48]
	str r2, [r4, #0x4c]
	str r1, [r4, #0x50]
_0801B35E:
	ldr r0, [r6, #0x24]
	cmp r0, #0
	bne _0801B3B2
	ldr r0, [r6, #0x2c]
	cmp r0, #0
	ble _0801B3B2
	ldr r0, [r4, #4]
	asrs r0, r0, #8
	str r0, [r6, #0x24]
	ldr r0, [r6, #4]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801B39C @ =gStaticData_0816C460
	adds r3, r0, r1
	adds r0, r6, #0
	adds r0, #0x31
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801B3A0
	ldr r0, [r3]
	ldr r1, [r3, #4]
	ldr r2, [r3, #8]
	str r0, [r4, #0x64]
	str r0, [r4, #0x54]
	str r1, [r4, #0x58]
	str r2, [r4, #0x5c]
	b _0801B3B2
	.align 2, 0
_0801B39C: .4byte gStaticData_0816C460
_0801B3A0:
	ldr r0, [r3]
	rsbs r0, r0, #0
	ldr r1, [r3, #8]
	rsbs r1, r1, #0
	ldr r2, [r3, #4]
	str r0, [r4, #0x64]
	str r0, [r4, #0x54]
	str r2, [r4, #0x58]
	str r1, [r4, #0x5c]
_0801B3B2:
	ldr r2, [r6, #0x18]
	movs r0, #1
	rsbs r0, r0, #0
	cmp r2, r0
	beq _0801B3CE
	ldr r0, [r4]
	asrs r0, r0, #8
	ldr r1, [r6, #0x20]
	subs r0, r0, r1
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	adds r0, r2, r0
	b _0801B3F0
_0801B3CE:
	ldr r2, [r4, #0x60]
	asrs r0, r2, #0x1f
	eors r2, r0
	subs r2, r2, r0
	ldr r3, _0801B410 @ =gStaticData_0816C460
	ldr r0, [r6, #4]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r3, #8
	adds r0, r0, r3
	ldr r0, [r0]
	cmp r2, r0
	blt _0801B3F2
	movs r0, #0
_0801B3F0:
	str r0, [r6, #0x18]
_0801B3F2:
	ldr r2, [r6, #0x1c]
	movs r0, #1
	rsbs r0, r0, #0
	cmp r2, r0
	beq _0801B414
	ldr r0, [r4, #4]
	asrs r0, r0, #8
	ldr r1, [r6, #0x24]
	subs r0, r0, r1
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	adds r0, r2, r0
	b _0801B436
	.align 2, 0
_0801B410: .4byte gStaticData_0816C460
_0801B414:
	ldr r2, [r4, #0x64]
	asrs r0, r2, #0x1f
	eors r2, r0
	subs r2, r2, r0
	ldr r3, _0801B474 @ =gStaticData_0816C460
	ldr r0, [r6, #4]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r3, #8
	adds r0, r0, r3
	ldr r0, [r0]
	cmp r2, r0
	blt _0801B438
	movs r0, #0
_0801B436:
	str r0, [r6, #0x1c]
_0801B438:
	ldr r0, [r6, #0x18]
	ldr r1, [r6, #0x28]
	cmp r0, r1
	ble _0801B48E
	cmp r1, #0
	beq _0801B48E
	adds r0, r6, #0
	adds r0, #0x30
	movs r2, #1
	ldrb r1, [r0]
	eors r2, r1
	strb r2, [r0]
	ldr r0, [r6, #4]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801B474 @ =gStaticData_0816C460
	adds r3, r0, r1
	cmp r2, #0
	beq _0801B478
	ldr r0, [r3]
	ldr r1, [r3, #4]
	ldr r2, [r3, #8]
	str r0, [r4, #0x48]
	str r1, [r4, #0x4c]
	str r2, [r4, #0x50]
	b _0801B488
	.align 2, 0
_0801B474: .4byte gStaticData_0816C460
_0801B478:
	ldr r0, [r3]
	rsbs r0, r0, #0
	ldr r1, [r3, #8]
	rsbs r1, r1, #0
	ldr r2, [r3, #4]
	str r0, [r4, #0x48]
	str r2, [r4, #0x4c]
	str r1, [r4, #0x50]
_0801B488:
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r6, #0x18]
_0801B48E:
	ldr r0, [r6, #0x1c]
	ldr r1, [r6, #0x2c]
	cmp r0, r1
	ble _0801B4E2
	cmp r1, #0
	beq _0801B4E2
	adds r0, r6, #0
	adds r0, #0x31
	movs r2, #1
	ldrb r5, [r0]
	eors r2, r5
	strb r2, [r0]
	ldr r0, [r6, #4]
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801B4C8 @ =gStaticData_0816C460
	adds r3, r0, r1
	cmp r2, #0
	beq _0801B4CC
	ldr r0, [r3]
	ldr r1, [r3, #4]
	ldr r2, [r3, #8]
	str r0, [r4, #0x54]
	str r1, [r4, #0x58]
	str r2, [r4, #0x5c]
	b _0801B4DC
	.align 2, 0
_0801B4C8: .4byte gStaticData_0816C460
_0801B4CC:
	ldr r0, [r3]
	rsbs r0, r0, #0
	ldr r1, [r3, #8]
	rsbs r1, r1, #0
	ldr r2, [r3, #4]
	str r0, [r4, #0x54]
	str r2, [r4, #0x58]
	str r1, [r4, #0x5c]
_0801B4DC:
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r6, #0x1c]
_0801B4E2:
	ldr r7, [r6, #0x10]
	cmp r7, #5
	bne _0801B558
	ldr r1, [r6, #0x14]
	cmp r1, #0
	ble _0801B518
	ldr r0, _0801B514 @ =gUnknown_0300082C
	ldr r0, [r0]
	subs r0, r0, r1
	cmp r0, #0x3c
	bne _0801B518
	ldr r1, [r6, #0xc]
	adds r1, #0x60
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r3, [r1, #4]
	adds r1, r4, #0
	movs r2, #3
	bl sub_803AD84
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r6, #0x14]
	b _0801B604
	.align 2, 0
_0801B514: .4byte gUnknown_0300082C
_0801B518:
	cmp r7, #5
	bne _0801B558
	cmp r1, #0
	ble _0801B558
	ldr r0, _0801B544 @ =gUnknown_0300082C
	ldr r5, [r0]
	subs r0, r5, r1
	movs r1, #0x1e
	bl sub_803AF1C
	cmp r0, #4
	bhi _0801B558
	movs r0, #1
	ands r5, r0
	cmp r5, #0
	bne _0801B54C
	ldr r0, [r4, #4]
	ldr r5, _0801B548 @ =0xFFFFFD00
	adds r0, r0, r5
	str r0, [r4, #4]
	b _0801B604
	.align 2, 0
_0801B544: .4byte gUnknown_0300082C
_0801B548: .4byte 0xFFFFFD00
_0801B54C:
	ldr r0, [r4, #4]
	movs r2, #0xc0
	lsls r2, r2, #2
	adds r0, r0, r2
	str r0, [r4, #4]
	b _0801B604
_0801B558:
	cmp r7, #7
	bne _0801B56C
	ldr r0, [r4, #0x30]
	cmp r0, #1
	bgt _0801B56C
	adds r0, r6, #0
	adds r0, #0x32
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801B5E6
_0801B56C:
	cmp r7, #6
	bne _0801B580
	ldr r0, [r4, #0x30]
	cmp r0, #1
	bgt _0801B580
	ldr r0, _0801B5C4 @ =gUnknown_0300082C
	ldr r1, [r0]
	ldr r0, [r6, #0x34]
	cmp r1, r0
	blo _0801B5E6
_0801B580:
	cmp r7, #7
	bne _0801B5D0
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801B5D0
	movs r0, #1
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _0801B5C8 @ =0x0000FFFF
	ldrh r2, [r4, #8]
	cmp r2, r0
	beq _0801B604
	ldrh r3, [r4, #8]
	ldr r0, _0801B5CC @ =gUnknown_030012B4
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
	b _0801B604
	.align 2, 0
_0801B5C4: .4byte gUnknown_0300082C
_0801B5C8: .4byte 0x0000FFFF
_0801B5CC: .4byte gUnknown_030012B4
_0801B5D0:
	cmp r7, #6
	bne _0801B604
	adds r0, r4, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801B604
	ldr r0, _0801B620 @ =gUnknown_0300082C
	ldr r0, [r0]
	adds r0, #0x78
	str r0, [r6, #0x34]
_0801B5E6:
	movs r3, #0
	ldr r0, [r4, #0x20]
	adds r2, r4, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r5, [r2]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _0801B602
	subs r3, r0, #1
_0801B602:
	str r3, [r4, #0x30]
_0801B604:
	adds r0, r6, #0
	adds r1, r4, #0
	bl sub_801B624
	ldr r0, [r4]
	asrs r0, r0, #8
	str r0, [r6, #0x20]
	ldr r0, [r4, #4]
	asrs r0, r0, #8
	str r0, [r6, #0x24]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801B620: .4byte gUnknown_0300082C

	thumb_func_start sub_801B624
sub_801B624: @ 0x0801B624
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	adds r7, r1, #0
	adds r0, #0x32
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801B6D8
	ldr r0, [r6, #0x10]
	cmp r0, #6
	beq _0801B6D8
	ldr r0, _0801B698 @ =gUnknown_030012D8
	mov r8, r0
	ldr r2, [r0]
	ldrb r1, [r2, #0xc]
	lsrs r0, r1, #7
	cmp r0, #0
	beq _0801B6D8
	adds r0, r2, #0
	adds r0, #0xac
	str r7, [r0]
	movs r1, #8
	subs r0, #0x44
	strb r1, [r0]
	mov r1, r8
	ldr r0, [r1]
	ldr r1, [r0]
	asrs r1, r1, #8
	ldr r5, [r7]
	asrs r5, r5, #8
	ldr r2, [r6, #0x20]
	subs r5, r5, r2
	ldr r2, [r0, #4]
	asrs r2, r2, #8
	ldr r3, [r7, #4]
	asrs r3, r3, #8
	ldr r4, [r6, #0x24]
	subs r3, r3, r4
	adds r1, r1, r5
	adds r2, r2, r3
	lsls r1, r1, #8
	str r1, [r0]
	lsls r2, r2, #8
	str r2, [r0, #4]
	bl sub_8009EA8
	mov r1, r8
	ldr r0, [r1]
	adds r0, #0x24
	ldrb r2, [r0]
	ldr r0, [r7, #0x60]
	cmp r0, #0
	ble _0801B69C
	movs r0, #1
	orrs r2, r0
	b _0801B6A8
	.align 2, 0
_0801B698: .4byte gUnknown_030012D8
_0801B69C:
	cmp r0, #0
	bge _0801B6A8
	movs r0, #2
	orrs r2, r0
	lsls r0, r2, #0x18
	lsrs r2, r0, #0x18
_0801B6A8:
	ldr r1, [r7, #0x64]
	cmp r1, #0
	ble _0801B6B2
	movs r0, #8
	b _0801B6B8
_0801B6B2:
	cmp r1, #0
	bge _0801B6BE
	movs r0, #4
_0801B6B8:
	orrs r2, r0
	lsls r0, r2, #0x18
	lsrs r2, r0, #0x18
_0801B6BE:
	ldr r0, _0801B6E4 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x24
	strb r2, [r0]
	ldr r0, [r6, #0x10]
	cmp r0, #5
	bne _0801B6D8
	ldr r0, [r6, #0x14]
	cmp r0, #0
	bne _0801B6D8
	ldr r0, _0801B6E8 @ =gUnknown_0300082C
	ldr r0, [r0]
	str r0, [r6, #0x14]
_0801B6D8:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801B6E4: .4byte gUnknown_030012D8
_0801B6E8: .4byte gUnknown_0300082C
	thumb_func_start sub_801B6EC
sub_801B6EC: @ 0x0801B6EC
	adds r3, r1, #0
	ldr r0, [r0, #4]
	ldr r0, [r0, #0]
	lsls r2, r2, #3
	adds r2, r2, r0
	ldr r1, [r2, #4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801B720 @ =gStaticData_0816C460
	adds r2, r0, r1
	adds r0, r3, #0
	adds r0, #0x28
	ldrb r0, [r0, #0]
	lsls r0, r0, #26
	cmp r0, #0
	bge _0801B724
	ldr r0, [r2, #0]
	negs r0, r0
	ldr r1, [r2, #8]
	negs r1, r1
	ldr r2, [r2, #4]
	str r0, [r3, #0x54]
	str r2, [r3, #0x58]
	str r1, [r3, #0x5c]
	b _0801B730
	.align 2, 0
_0801B720: .4byte gStaticData_0816C460
_0801B724:
	ldr r0, [r2, #0]
	ldr r1, [r2, #4]
	ldr r2, [r2, #8]
	str r0, [r3, #0x54]
	str r1, [r3, #0x58]
	str r2, [r3, #0x5c]
_0801B730:
	bx lr
	.align 2, 0

	thumb_func_start sub_801B734
sub_801B734: @ 0x0801B734
	adds r3, r1, #0
	ldr r0, [r0, #4]
	ldr r0, [r0, #0]
	lsls r2, r2, #3
	adds r2, r2, r0
	ldr r1, [r2, #0]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0801B768 @ =gStaticData_0816C460
	adds r2, r0, r1
	adds r0, r3, #0
	adds r0, #0x28
	ldrb r0, [r0, #0]
	lsls r0, r0, #27
	cmp r0, #0
	bge _0801B76C
	ldr r0, [r2, #0]
	negs r0, r0
	ldr r1, [r2, #8]
	negs r1, r1
	ldr r2, [r2, #4]
	str r0, [r3, #0x48]
	str r2, [r3, #0x4c]
	str r1, [r3, #0x50]
	b _0801B778
	.align 2, 0
_0801B768: .4byte gStaticData_0816C460
_0801B76C:
	ldr r0, [r2, #0]
	ldr r1, [r2, #4]
	ldr r2, [r2, #8]
	str r0, [r3, #0x48]
	str r1, [r3, #0x4c]
	str r2, [r3, #0x50]
_0801B778:
	bx lr

	thumb_func_start sub_801B77C
sub_801B77C: @ 0x0801B77C
	push {lr}
	ldr r3, [r0, #4]
	ldr r3, [r3]
	lsls r2, r2, #3
	adds r2, r2, r3
	ldr r3, [r2, #4]
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r2, r2, #2
	ldr r3, _0801B79C @ =gStaticData_0816C460
	adds r2, r2, r3
	bl sub_800B6D0
	pop {r0}
	bx r0
	.align 2, 0
_0801B79C: .4byte gStaticData_0816C460

	thumb_func_start sub_801B7A0
sub_801B7A0: @ 0x0801B7A0
	push {lr}
	ldr r3, [r0, #4]
	ldr r3, [r3]
	lsls r2, r2, #3
	adds r2, r2, r3
	ldr r3, [r2]
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r2, r2, #2
	ldr r3, _0801B7C0 @ =gStaticData_0816C460
	adds r2, r2, r3
	bl sub_800B7B0
	pop {r0}
	bx r0
	.align 2, 0
_0801B7C0: .4byte gStaticData_0816C460

	thumb_func_start sub_801B7C4
sub_801B7C4: @ 0x0801B7C4
	push {lr}
	ldr r2, _0801B7D4 @ =gStaticData_087E4A54
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_0801B7D4: .4byte gStaticData_087E4A54

	thumb_func_start sub_801B7D8
sub_801B7D8: @ 0x0801B7D8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	add r0, sp, #0x18
	lsls r3, r3, #0x18
	lsrs r3, r3, #0x18
	mov r8, r3
	ldrb r7, [r0]
	adds r0, r4, #0
	bl sub_800B8C8
	ldr r0, _0801B848 @ =gStaticData_087E4A54
	str r0, [r4, #0xc]
	ldr r0, [sp, #0x1c]
	subs r0, #6
	cmp r0, #1
	bhi _0801B804
	movs r6, #0
	movs r5, #0
_0801B804:
	movs r1, #0
	str r1, [r4, #0x20]
	str r5, [r4, #0x18]
	str r1, [r4, #0x24]
	str r6, [r4, #0x1c]
	ldr r0, _0801B84C @ =gStaticData_0816C458
	str r0, [r4, #4]
	adds r0, r4, #0
	adds r0, #0x32
	strb r1, [r0]
	ldr r0, [sp, #0x1c]
	str r0, [r4, #0x10]
	str r1, [r4, #0x14]
	lsls r0, r5, #1
	str r0, [r4, #0x28]
	lsls r0, r6, #1
	str r0, [r4, #0x2c]
	adds r0, r4, #0
	adds r0, #0x30
	mov r1, r8
	strb r1, [r0]
	adds r0, #1
	strb r7, [r0]
	ldr r0, _0801B850 @ =gUnknown_0300082C
	ldr r0, [r0]
	adds r0, #0x78
	str r0, [r4, #0x34]
	adds r0, r4, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0801B848: .4byte gStaticData_087E4A54
_0801B84C: .4byte gStaticData_0816C458
_0801B850: .4byte gUnknown_0300082C

	thumb_func_start sub_801B854
sub_801B854: @ 0x0801B854
	adds r0, #0x32
	movs r1, #0
	strb r1, [r0]
	bx lr
