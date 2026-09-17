.include "asm/macros.inc"

.syntax unified
.arm

.if NON_MATCHING == 0
	thumb_func_start sub_8005AE8
sub_8005AE8: @ 0x08005AE8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r0
	movs r7, #0
_08005AF2:
	lsls r5, r7, #2
	mov r6, r8
	adds r6, #0x8c
	adds r6, r6, r5
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	adds r4, r0, #0
	str r4, [r6]
	ldr r0, _08005B74 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xe4
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	ldr r0, _08005B78 @ =gStaticData_0816B20C
	adds r5, r5, r0
	ldr r0, [r5]
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
	ldr r0, [r6]
	lsls r2, r7, #3
	ldr r1, _08005B7C @ =gStaticData_0816B1EC
	adds r2, r2, r1
	ldr r1, [r2]
	ldr r2, [r2, #4]
	bl sub_800737C
	ldr r0, [r6]
	bl sub_800815C
	ldr r2, [r6]
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
	adds r7, #1
	cmp r7, #3
	ble _08005AF2
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08005B74: .4byte gUnknown_030012D0
_08005B78: .4byte gStaticData_0816B20C
_08005B7C: .4byte gStaticData_0816B1EC
.endif

.if NON_MATCHING == 0
	thumb_func_start sub_8005B80
sub_8005B80: @ 0x08005B80
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	movs r0, #0
	mov r8, r0
_08005B8C:
	mov r1, r8
	lsls r5, r1, #2
	adds r6, r7, #0
	adds r6, #0x9c
	adds r6, r6, r5
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	adds r4, r0, #0
	str r4, [r6]
	ldr r0, _08005C4C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r3, #0xc0
	lsls r3, r3, #1
	adds r0, r0, r3
	str r0, [r4, #0x20]
	ldr r0, _08005C50 @ =gStaticData_0816B244
	adds r5, r5, r0
	ldr r0, [r5]
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
	ldr r0, [r6]
	mov r1, r8
	lsls r2, r1, #3
	ldr r1, _08005C54 @ =gStaticData_0816B21C
	adds r2, r2, r1
	ldr r1, [r2]
	ldr r2, [r2, #4]
	bl sub_800737C
	ldr r0, [r6]
	bl sub_800815C
	ldr r2, [r6]
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
	ldr r1, [r6]
	movs r0, #0x80
	strh r0, [r1, #0x3c]
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #4
	ble _08005B8C
	ldr r0, [r7, #0x10]
	bl sub_8006920
	adds r4, r0, #0
	ldr r0, [r7, #0x10]
	bl sub_80068CC
	adds r5, r0, #0
	adds r1, r7, #0
	adds r1, #0x2f
	adds r0, r4, #0
	bl sub_80060AC
	adds r1, r7, #0
	adds r1, #0x32
	adds r0, r5, #0
	bl sub_80060AC
	adds r1, r7, #0
	adds r1, #0x49
	movs r0, #0x1c
	bl sub_80060AC
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08005C4C: .4byte gUnknown_030012D0
_08005C50: .4byte gStaticData_0816B244
_08005C54: .4byte gStaticData_0816B21C
.endif

.if NON_MATCHING == 0
	thumb_func_start sub_8005C58
sub_8005C58: @ 0x08005C58
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	movs r0, #0
	mov r8, r0
_08005C64:
	mov r1, r8
	lsls r5, r1, #2
	adds r6, r7, #0
	adds r6, #0xb0
	adds r6, r6, r5
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	adds r4, r0, #0
	str r4, [r6]
	ldr r0, _08005D38 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r3, #0xc6
	lsls r3, r3, #1
	adds r0, r0, r3
	str r0, [r4, #0x20]
	ldr r0, _08005D3C @ =gStaticData_0816B270
	adds r5, r5, r0
	ldr r0, [r5]
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
	ldr r0, [r6]
	mov r1, r8
	lsls r2, r1, #3
	ldr r1, _08005D40 @ =gStaticData_0816B258
	adds r2, r2, r1
	ldr r1, [r2]
	ldr r2, [r2, #4]
	bl sub_800737C
	ldr r0, [r6]
	bl sub_800815C
	ldr r2, [r6]
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
	ldr r1, [r6]
	movs r0, #0x80
	strh r0, [r1, #0x3c]
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #2
	ble _08005C64
	ldr r0, [r7, #0x10]
	bl sub_8006864
	adds r1, r7, #0
	adds r1, #0x38
	bl sub_80060AC
	ldr r0, [r7, #0x10]
	bl sub_8006820
	adds r1, r7, #0
	adds r1, #0x3b
	bl sub_80060AC
	ldr r0, [r7, #0x10]
	bl sub_80067EC
	adds r1, r7, #0
	adds r1, #0x3e
	bl sub_80060AC
	ldr r0, [r7, #0x10]
	bl sub_80068A8
	adds r1, r7, #0
	adds r1, #0x35
	bl sub_80060AC
	adds r1, r7, #0
	adds r1, #0x4c
	movs r0, #0x14
	bl sub_80060AC
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08005D38: .4byte gUnknown_030012D0
_08005D3C: .4byte gStaticData_0816B270
_08005D40: .4byte gStaticData_0816B258
.endif

.if NON_MATCHING == 0
	thumb_func_start sub_8005D44
sub_8005D44: @ 0x08005D44
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r0, _08005E48 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_802332C
	adds r4, r0, #0
	lsls r1, r4, #2
	adds r1, #4
	ldr r0, [r6, #0x10]
	adds r0, r0, r1
	ldr r0, [r0]
	lsls r0, r0, #0x10
	lsrs r5, r0, #0x13
	adds r1, r6, #0
	adds r1, #0x7c
	adds r0, r5, #0
	bl FormatCentiseconds
	lsls r0, r4, #3
	adds r0, r0, r4
	lsls r0, r0, #2
	ldr r1, _08005E4C @ =gStaticData_0816C86C
	adds r7, r0, r1
	movs r1, #0
	cmp r5, #0
	beq _08005D82
	ldr r0, [r7, #8]
	cmp r5, r0
	bhi _08005D82
	movs r1, #1
_08005D82:
	adds r0, r6, #0
	adds r0, #0x6c
	strb r1, [r0]
	adds r6, #0xbc
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	str r0, [r6]
	ldr r1, _08005E50 @ =gUnknown_030012D0
	ldr r1, [r1]
	ldr r1, [r1]
	ldr r1, [r1]
	movs r2, #0xc6
	lsls r2, r2, #1
	adds r1, r1, r2
	str r1, [r0, #0x20]
	ldr r2, _08005E54 @ =gStaticData_0816B27C
	ldr r1, [r2]
	ldr r2, [r2, #4]
	bl sub_800737C
	cmp r5, #0
	beq _08005E40
	ldr r0, [r7, #8]
	cmp r5, r0
	bhi _08005DDA
	ldr r0, _08005E58 @ =gStaticData_0816B270
	ldr r4, [r6]
	ldr r0, [r0, #8]
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
_08005DDA:
	ldr r0, [r7, #0xc]
	cmp r5, r0
	bhi _08005E00
	ldr r0, _08005E58 @ =gStaticData_0816B270
	ldr r4, [r6]
	ldr r0, [r0, #4]
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
_08005E00:
	ldr r0, [r7, #0x10]
	cmp r5, r0
	bhi _08005E26
	ldr r0, _08005E58 @ =gStaticData_0816B270
	ldr r4, [r6]
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
_08005E26:
	ldr r0, [r6]
	bl sub_800815C
	ldr r2, [r6]
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
_08005E40:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08005E48: .4byte gUnknown_030012C0
_08005E4C: .4byte gStaticData_0816C86C
_08005E50: .4byte gUnknown_030012D0
_08005E54: .4byte gStaticData_0816B27C
_08005E58: .4byte gStaticData_0816B270
.endif
