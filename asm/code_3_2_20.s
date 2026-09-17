.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8027940
sub_8027940: @ 0x08027940
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	ldr r0, [r5, #0x10]
	cmp r0, #0
	bne _08027950
	b _08027D3E
_08027950:
	ldr r0, _08027970 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023414
	str r0, [r5, #0x24]
	ldr r0, [r5, #0x10]
	cmp r0, #1
	beq _08027964
	cmp r0, #3
	bne _08027978
_08027964:
	ldr r1, _08027974 @ =gUnknown_0300086C
	ldr r0, [r5, #0x14]
	lsls r0, r0, #1
	subs r0, #0x28
	b _0802797C
	.align 2, 0
_08027970: .4byte gUnknown_030012C0
_08027974: .4byte gUnknown_0300086C
_08027978:
	ldr r1, _08027A00 @ =gUnknown_0300086C
	movs r0, #0
_0802797C:
	str r0, [r1]
	ldr r1, [r5, #0x24]
	ldr r0, [r5, #0x48]
	cmp r1, r0
	bne _08027988
	b _08027AF2
_08027988:
	cmp r1, #0x63
	ble _08027A08
	adds r0, r1, #0
	movs r1, #0x64
	bl sub_803ADB4
	ldr r4, [r5, #0x64]
	adds r6, r4, #0
	adds r6, #0xc0
	adds r3, r0, #0
	ldr r0, [r6, #0x20]
	adds r2, r4, #0
	adds r2, #0xed
	ldr r1, [r0]
	ldrb r7, [r2]
	lsls r0, r7, #3
	adds r2, r7, #0
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _080279B8
	subs r3, r0, #1
_080279B8:
	str r3, [r6, #0x30]
	ldr r0, [r5, #0x24]
	movs r1, #0xa
	bl sub_803ADB4
	movs r1, #0xa
	bl sub_803AE4C
	movs r1, #0x80
	lsls r1, r1, #1
	adds r6, r4, r1
	adds r3, r0, #0
	ldr r0, [r6, #0x20]
	ldr r7, _08027A04 @ =0x0000012D
	adds r2, r4, r7
	ldr r1, [r0]
	ldrb r7, [r2]
	lsls r0, r7, #3
	adds r2, r7, #0
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _080279EC
	subs r3, r0, #1
_080279EC:
	str r3, [r6, #0x30]
	ldr r0, [r5, #0x24]
	movs r1, #0xa
	bl sub_803AE4C
	movs r1, #0xa0
	lsls r1, r1, #1
	adds r6, r4, r1
	adds r3, r0, #0
	b _08027AD6
	.align 2, 0
_08027A00: .4byte gUnknown_0300086C
_08027A04: .4byte 0x0000012D
_08027A08:
	cmp r1, #9
	ble _08027A7C
	adds r0, r1, #0
	movs r1, #0xa
	bl sub_803ADB4
	ldr r4, [r5, #0x64]
	adds r6, r4, #0
	adds r6, #0xc0
	adds r3, r0, #0
	ldr r0, [r6, #0x20]
	adds r2, r4, #0
	adds r2, #0xed
	ldr r1, [r0]
	ldrb r7, [r2]
	lsls r0, r7, #3
	adds r2, r7, #0
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08027A38
	subs r3, r0, #1
_08027A38:
	str r3, [r6, #0x30]
	ldr r0, [r5, #0x24]
	movs r1, #0xa
	bl sub_803AE4C
	movs r1, #0x80
	lsls r1, r1, #1
	adds r6, r4, r1
	adds r3, r0, #0
	ldr r0, [r6, #0x20]
	ldr r7, _08027A78 @ =0x0000012D
	adds r2, r4, r7
	ldr r1, [r0]
	ldrb r7, [r2]
	lsls r0, r7, #3
	adds r2, r7, #0
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08027A66
	subs r3, r0, #1
_08027A66:
	str r3, [r6, #0x30]
	movs r0, #0xa0
	lsls r0, r0, #1
	adds r3, r4, r0
	movs r1, #1
	rsbs r1, r1, #0
	str r1, [r3, #0x30]
	b _08027AF2
	.align 2, 0
_08027A78: .4byte 0x0000012D
_08027A7C:
	ldr r4, [r5, #0x64]
	adds r6, r4, #0
	adds r6, #0xc0
	adds r3, r1, #0
	ldr r0, [r6, #0x20]
	adds r2, r4, #0
	adds r2, #0xed
	ldr r1, [r0]
	ldrb r7, [r2]
	lsls r0, r7, #3
	adds r2, r7, #0
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08027AA0
	subs r3, r0, #1
_08027AA0:
	str r3, [r6, #0x30]
	movs r3, #1
	rsbs r3, r3, #0
	movs r0, #0x80
	lsls r0, r0, #1
	adds r6, r4, r0
	mov r8, r3
	ldr r0, [r6, #0x20]
	ldr r1, _08027BB8 @ =0x0000012D
	adds r2, r4, r1
	ldr r1, [r0]
	ldrb r7, [r2]
	lsls r0, r7, #3
	adds r2, r7, #0
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08027ACC
	subs r0, #1
	mov r8, r0
_08027ACC:
	mov r0, r8
	str r0, [r6, #0x30]
	movs r1, #0xa0
	lsls r1, r1, #1
	adds r6, r4, r1
_08027AD6:
	ldr r0, [r6, #0x20]
	ldr r7, _08027BBC @ =0x0000016D
	adds r2, r4, r7
	ldr r1, [r0]
	ldrb r4, [r2]
	lsls r0, r4, #3
	subs r0, r0, r4
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08027AF0
	subs r3, r0, #1
_08027AF0:
	str r3, [r6, #0x30]
_08027AF2:
	ldr r0, [r5, #0x64]
	adds r0, #0xc0
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r5, #0x64]
	movs r6, #0x80
	lsls r6, r6, #1
	adds r0, r0, r6
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r5, #0x64]
	movs r7, #0xa0
	lsls r7, r7, #1
	adds r0, r0, r7
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r5, #0x24]
	movs r1, #2
	cmp r0, #0x63
	bgt _08027B2E
	movs r1, #0
	cmp r0, #9
	ble _08027B2E
	movs r1, #1
_08027B2E:
	lsls r0, r1, #4
	subs r0, r0, r1
	mov r8, r0
	ldr r1, [r5, #0x28]
	ldr r0, [r5, #0x4c]
	ldr r4, [r5, #0x64]
	cmp r1, r0
	bne _08027B40
	b _08027CB8
_08027B40:
	cmp r1, #0x63
	ble _08027BC8
	adds r0, r1, #0
	movs r1, #0x64
	bl sub_803ADB4
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r6, r4, r1
	adds r3, r0, #0
	ldr r0, [r6, #0x20]
	ldr r7, _08027BC0 @ =0x000001AD
	adds r2, r4, r7
	ldr r1, [r0]
	ldrb r7, [r2]
	lsls r0, r7, #3
	adds r2, r7, #0
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08027B70
	subs r3, r0, #1
_08027B70:
	str r3, [r6, #0x30]
	ldr r0, [r5, #0x28]
	movs r1, #0xa
	bl sub_803ADB4
	movs r1, #0xa
	bl sub_803AE4C
	movs r1, #0xe0
	lsls r1, r1, #1
	adds r6, r4, r1
	adds r3, r0, #0
	ldr r0, [r6, #0x20]
	ldr r7, _08027BC4 @ =0x000001ED
	adds r2, r4, r7
	ldr r1, [r0]
	ldrb r7, [r2]
	lsls r0, r7, #3
	adds r2, r7, #0
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08027BA4
	subs r3, r0, #1
_08027BA4:
	str r3, [r6, #0x30]
	ldr r0, [r5, #0x28]
	movs r1, #0xa
	bl sub_803AE4C
	movs r1, #0x80
	lsls r1, r1, #2
	adds r6, r4, r1
	adds r3, r0, #0
	b _08027C9A
	.align 2, 0
_08027BB8: .4byte 0x0000012D
_08027BBC: .4byte 0x0000016D
_08027BC0: .4byte 0x000001AD
_08027BC4: .4byte 0x000001ED
_08027BC8:
	cmp r1, #9
	ble _08027C40
	adds r0, r1, #0
	movs r1, #0xa
	bl sub_803ADB4
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r6, r4, r1
	adds r3, r0, #0
	ldr r0, [r6, #0x20]
	ldr r7, _08027C38 @ =0x000001AD
	adds r2, r4, r7
	ldr r1, [r0]
	ldrb r7, [r2]
	lsls r0, r7, #3
	adds r2, r7, #0
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08027BF8
	subs r3, r0, #1
_08027BF8:
	str r3, [r6, #0x30]
	ldr r0, [r5, #0x28]
	movs r1, #0xa
	bl sub_803AE4C
	movs r1, #0xe0
	lsls r1, r1, #1
	adds r6, r4, r1
	adds r3, r0, #0
	ldr r0, [r6, #0x20]
	ldr r7, _08027C3C @ =0x000001ED
	adds r2, r4, r7
	ldr r1, [r0]
	ldrb r7, [r2]
	lsls r0, r7, #3
	adds r2, r7, #0
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08027C26
	subs r3, r0, #1
_08027C26:
	str r3, [r6, #0x30]
	movs r0, #0x80
	lsls r0, r0, #2
	adds r3, r4, r0
	movs r1, #1
	rsbs r1, r1, #0
	str r1, [r3, #0x30]
	b _08027CB8
	.align 2, 0
_08027C38: .4byte 0x000001AD
_08027C3C: .4byte 0x000001ED
_08027C40:
	movs r7, #0xc0
	lsls r7, r7, #1
	adds r6, r4, r7
	adds r3, r1, #0
	ldr r0, [r6, #0x20]
	ldr r1, _08027D48 @ =0x000001AD
	adds r2, r4, r1
	ldr r1, [r0]
	ldrb r7, [r2]
	lsls r0, r7, #3
	adds r2, r7, #0
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08027C64
	subs r3, r0, #1
_08027C64:
	str r3, [r6, #0x30]
	movs r3, #1
	rsbs r3, r3, #0
	movs r0, #0xe0
	lsls r0, r0, #1
	adds r6, r4, r0
	mov ip, r3
	ldr r0, [r6, #0x20]
	ldr r1, _08027D4C @ =0x000001ED
	adds r2, r4, r1
	ldr r1, [r0]
	ldrb r7, [r2]
	lsls r0, r7, #3
	adds r2, r7, #0
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08027C90
	subs r0, #1
	mov ip, r0
_08027C90:
	mov r0, ip
	str r0, [r6, #0x30]
	movs r1, #0x80
	lsls r1, r1, #2
	adds r6, r4, r1
_08027C9A:
	ldr r0, [r6, #0x20]
	ldr r2, _08027D50 @ =0x0000022D
	adds r1, r4, r2
	ldr r2, [r0]
	ldrb r7, [r1]
	lsls r0, r7, #3
	adds r1, r7, #0
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08027CB6
	subs r3, r0, #1
_08027CB6:
	str r3, [r6, #0x30]
_08027CB8:
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r0, r4, r1
	mov r1, r8
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r5, #0x64]
	movs r2, #0xe0
	lsls r2, r2, #1
	adds r0, r0, r2
	mov r1, r8
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r5, #0x64]
	movs r4, #0x80
	lsls r4, r4, #2
	adds r0, r0, r4
	mov r1, r8
	movs r2, #0
	bl sub_80270E0
	ldr r1, _08027D54 @ =gStaticData_08174C6C
	ldr r2, [r5, #0x64]
	movs r6, #0xa0
	lsls r6, r6, #2
	adds r3, r2, r6
	ldr r0, [r1, #0x50]
	add r0, r8
	ldr r1, [r1, #0x54]
	lsls r0, r0, #8
	str r0, [r3]
	lsls r1, r1, #8
	str r1, [r3, #4]
	movs r4, #0
	ldr r0, [r3, #0x20]
	ldr r7, _08027D58 @ =0x000002AD
	adds r2, r2, r7
	ldr r1, [r0]
	ldrb r6, [r2]
	lsls r0, r6, #3
	subs r0, r0, r6
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _08027D1A
	subs r4, r0, #1
_08027D1A:
	str r4, [r3, #0x30]
	adds r0, r3, #0
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r5, #0x64]
	movs r7, #0x90
	lsls r7, r7, #2
	adds r0, r0, r7
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r5, #0x28]
	str r0, [r5, #0x4c]
	ldr r0, [r5, #0x24]
	str r0, [r5, #0x48]
_08027D3E:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08027D48: .4byte 0x000001AD
_08027D4C: .4byte 0x000001ED
_08027D50: .4byte 0x0000022D
_08027D54: .4byte gStaticData_08174C6C
_08027D58: .4byte 0x000002AD

	thumb_func_start sub_8027D5C
sub_8027D5C: @ 0x08027D5C
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	ldr r0, [r5, #8]
	cmp r0, #0
	bne _08027D68
	b _08027E7C
_08027D68:
	cmp r0, #1
	beq _08027D70
	cmp r0, #3
	bne _08027D80
_08027D70:
	ldr r1, _08027D7C @ =gUnknown_0300086C
	ldr r0, [r5, #0xc]
	lsls r0, r0, #1
	subs r0, #0x28
	b _08027D84
	.align 2, 0
_08027D7C: .4byte gUnknown_0300086C
_08027D80:
	ldr r1, _08027E14 @ =gUnknown_0300086C
	movs r0, #0
_08027D84:
	str r0, [r1]
	ldr r0, _08027E18 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_802325C
	str r0, [r5, #0x20]
	ldr r0, [r5, #0x64]
	movs r4, #0xd0
	lsls r4, r4, #2
	adds r0, r0, r4
	bl sub_8008044
	ldr r0, [r5, #0x64]
	adds r0, r0, r4
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r1, [r5, #0x20]
	ldr r0, [r5, #0x44]
	cmp r1, r0
	beq _08027E58
	cmp r1, #9
	ble _08027E24
	adds r0, r1, #0
	movs r1, #0xa
	bl sub_803ADB4
	ldr r4, [r5, #0x64]
	movs r1, #0xb0
	lsls r1, r1, #2
	adds r6, r4, r1
	adds r3, r0, #0
	ldr r0, [r6, #0x20]
	ldr r2, _08027E1C @ =0x000002ED
	adds r1, r4, r2
	ldr r2, [r0]
	ldrb r7, [r1]
	lsls r0, r7, #3
	adds r1, r7, #0
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08027DE2
	subs r3, r0, #1
_08027DE2:
	str r3, [r6, #0x30]
	ldr r0, [r5, #0x20]
	movs r1, #0xa
	bl sub_803AE4C
	movs r1, #0xc0
	lsls r1, r1, #2
	adds r6, r4, r1
	adds r3, r0, #0
	ldr r0, [r6, #0x20]
	ldr r2, _08027E20 @ =0x0000032D
	adds r1, r4, r2
	ldr r2, [r0]
	ldrb r4, [r1]
	lsls r0, r4, #3
	subs r0, r0, r4
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r1, [r0, #0x16]
	cmp r3, r1
	blt _08027E0E
	subs r3, r1, #1
_08027E0E:
	str r3, [r6, #0x30]
	b _08027E58
	.align 2, 0
_08027E14: .4byte gUnknown_0300086C
_08027E18: .4byte gUnknown_030012C0
_08027E1C: .4byte 0x000002ED
_08027E20: .4byte 0x0000032D
_08027E24:
	ldr r4, [r5, #0x64]
	movs r7, #0xb0
	lsls r7, r7, #2
	adds r6, r4, r7
	adds r3, r1, #0
	ldr r0, [r6, #0x20]
	ldr r2, _08027E84 @ =0x000002ED
	adds r1, r4, r2
	ldr r2, [r0]
	ldrb r7, [r1]
	lsls r0, r7, #3
	adds r1, r7, #0
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08027E4A
	subs r3, r0, #1
_08027E4A:
	str r3, [r6, #0x30]
	movs r0, #0xc0
	lsls r0, r0, #2
	adds r3, r4, r0
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r3, #0x30]
_08027E58:
	ldr r0, [r5, #0x64]
	movs r7, #0xb0
	lsls r7, r7, #2
	adds r0, r0, r7
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r5, #0x64]
	movs r1, #0xc0
	lsls r1, r1, #2
	adds r0, r0, r1
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r5, #0x20]
	str r0, [r5, #0x44]
_08027E7C:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08027E84: .4byte 0x000002ED

	thumb_func_start sub_8027E88
sub_8027E88: @ 0x08027E88
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r1, _08027F94 @ =gUnknown_0300086C
	movs r0, #0
	str r0, [r1]
	ldr r1, _08027F98 @ =gStaticData_08174C6C
	ldr r2, [r6, #0x64]
	movs r0, #0xc0
	lsls r0, r0, #3
	adds r3, r2, r0
	adds r0, r1, #0
	adds r0, #0xc0
	ldr r0, [r0]
	adds r1, #0xc4
	ldr r1, [r1]
	lsls r0, r0, #8
	str r0, [r3]
	lsls r1, r1, #8
	str r1, [r3, #4]
	movs r4, #0
	ldr r0, [r3, #0x20]
	ldr r1, _08027F9C @ =0x0000062D
	adds r2, r2, r1
	ldr r1, [r0]
	ldrb r5, [r2]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _08027ECA
	subs r4, r0, #1
_08027ECA:
	str r4, [r3, #0x30]
	adds r0, r3, #0
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, _08027FA0 @ =gUnknown_03000884
	ldr r0, [r0]
	ldr r2, [r0, #0x50]
	movs r7, #0x30
	ldrsh r1, [r2, r7]
	adds r0, r0, r1
	ldr r1, [r2, #0x34]
	bl sub_803AD7C
	adds r1, r0, #0
	str r1, [r6, #0x38]
	ldr r0, [r6, #0x5c]
	cmp r1, r0
	bne _08027EF4
	b _080280F6
_08027EF4:
	cmp r1, #0x64
	bne _08027FB4
	ldr r3, [r6, #0x64]
	movs r0, #0xc8
	lsls r0, r0, #3
	adds r5, r3, r0
	movs r4, #1
	ldr r0, [r5, #0x20]
	ldr r2, _08027FA4 @ =0x0000066D
	adds r1, r3, r2
	ldr r2, [r0]
	ldrb r7, [r1]
	lsls r0, r7, #3
	adds r1, r7, #0
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _08027F1E
	subs r4, r0, #1
_08027F1E:
	str r4, [r5, #0x30]
	movs r0, #0xd0
	lsls r0, r0, #3
	adds r5, r3, r0
	movs r4, #0
	ldr r0, [r5, #0x20]
	ldr r2, _08027FA8 @ =0x000006AD
	adds r1, r3, r2
	ldr r2, [r0]
	ldrb r7, [r1]
	lsls r0, r7, #3
	adds r1, r7, #0
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _08027F44
	subs r4, r0, #1
_08027F44:
	str r4, [r5, #0x30]
	movs r0, #0xd8
	lsls r0, r0, #3
	adds r5, r3, r0
	movs r4, #0
	ldr r0, [r5, #0x20]
	ldr r2, _08027FAC @ =0x000006ED
	adds r1, r3, r2
	ldr r2, [r0]
	ldrb r7, [r1]
	lsls r0, r7, #3
	adds r1, r7, #0
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _08027F6A
	subs r4, r0, #1
_08027F6A:
	str r4, [r5, #0x30]
	movs r0, #0xe0
	lsls r0, r0, #3
	adds r5, r3, r0
	movs r4, #0xa
	ldr r0, [r5, #0x20]
	ldr r2, _08027FB0 @ =0x0000072D
	adds r1, r3, r2
	ldr r2, [r0]
	ldrb r3, [r1]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r1, [r0, #0x16]
	cmp r4, r1
	blt _08027F8E
	subs r4, r1, #1
_08027F8E:
	str r4, [r5, #0x30]
	b _080280F6
	.align 2, 0
_08027F94: .4byte gUnknown_0300086C
_08027F98: .4byte gStaticData_08174C6C
_08027F9C: .4byte 0x0000062D
_08027FA0: .4byte gUnknown_03000884
_08027FA4: .4byte 0x0000066D
_08027FA8: .4byte 0x000006AD
_08027FAC: .4byte 0x000006ED
_08027FB0: .4byte 0x0000072D
_08027FB4:
	cmp r1, #9
	ble _08028058
	adds r0, r1, #0
	movs r1, #0xa
	bl sub_803ADB4
	ldr r4, [r6, #0x64]
	movs r7, #0xc8
	lsls r7, r7, #3
	adds r5, r4, r7
	adds r3, r0, #0
	ldr r0, [r5, #0x20]
	ldr r2, _0802804C @ =0x0000066D
	adds r1, r4, r2
	ldr r2, [r0]
	ldrb r7, [r1]
	lsls r0, r7, #3
	adds r1, r7, #0
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08027FE6
	subs r3, r0, #1
_08027FE6:
	str r3, [r5, #0x30]
	ldr r0, [r6, #0x38]
	movs r1, #0xa
	bl sub_803AE4C
	movs r1, #0xd0
	lsls r1, r1, #3
	adds r5, r4, r1
	adds r3, r0, #0
	ldr r0, [r5, #0x20]
	ldr r2, _08028050 @ =0x000006AD
	adds r1, r4, r2
	ldr r2, [r0]
	ldrb r7, [r1]
	lsls r0, r7, #3
	adds r1, r7, #0
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08028014
	subs r3, r0, #1
_08028014:
	str r3, [r5, #0x30]
	movs r0, #0xd8
	lsls r0, r0, #3
	adds r5, r4, r0
	movs r3, #0xa
	ldr r0, [r5, #0x20]
	ldr r2, _08028054 @ =0x000006ED
	adds r1, r4, r2
	ldr r2, [r0]
	ldrb r7, [r1]
	lsls r0, r7, #3
	adds r1, r7, #0
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _0802803A
	subs r3, r0, #1
_0802803A:
	str r3, [r5, #0x30]
	movs r0, #0xe0
	lsls r0, r0, #3
	adds r3, r4, r0
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r3, #0x30]
	b _080280F6
	.align 2, 0
_0802804C: .4byte 0x0000066D
_08028050: .4byte 0x000006AD
_08028054: .4byte 0x000006ED
_08028058:
	ldr r4, [r6, #0x64]
	movs r7, #0xc8
	lsls r7, r7, #3
	adds r5, r4, r7
	adds r3, r1, #0
	ldr r0, [r5, #0x20]
	ldr r2, _08028248 @ =0x0000066D
	adds r1, r4, r2
	ldr r2, [r0]
	ldrb r7, [r1]
	lsls r0, r7, #3
	adds r1, r7, #0
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _0802807E
	subs r3, r0, #1
_0802807E:
	str r3, [r5, #0x30]
	movs r0, #0xd0
	lsls r0, r0, #3
	adds r5, r4, r0
	movs r3, #0xa
	ldr r0, [r5, #0x20]
	ldr r2, _0802824C @ =0x000006AD
	adds r1, r4, r2
	ldr r2, [r0]
	ldrb r7, [r1]
	lsls r0, r7, #3
	adds r1, r7, #0
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _080280A4
	subs r3, r0, #1
_080280A4:
	str r3, [r5, #0x30]
	movs r3, #1
	rsbs r3, r3, #0
	movs r0, #0xd8
	lsls r0, r0, #3
	adds r5, r4, r0
	mov ip, r3
	ldr r0, [r5, #0x20]
	ldr r2, _08028250 @ =0x000006ED
	adds r1, r4, r2
	ldr r2, [r0]
	ldrb r7, [r1]
	lsls r0, r7, #3
	adds r1, r7, #0
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _080280D0
	subs r0, #1
	mov ip, r0
_080280D0:
	mov r0, ip
	str r0, [r5, #0x30]
	movs r1, #0xe0
	lsls r1, r1, #3
	adds r5, r4, r1
	ldr r0, [r5, #0x20]
	ldr r2, _08028254 @ =0x0000072D
	adds r1, r4, r2
	ldr r2, [r0]
	ldrb r4, [r1]
	lsls r0, r4, #3
	subs r0, r0, r4
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r1, [r0, #0x16]
	cmp r3, r1
	blt _080280F4
	subs r3, r1, #1
_080280F4:
	str r3, [r5, #0x30]
_080280F6:
	ldr r0, [r6, #0x64]
	movs r5, #0xc8
	lsls r5, r5, #3
	adds r0, r0, r5
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r6, #0x64]
	movs r7, #0xd0
	lsls r7, r7, #3
	adds r0, r0, r7
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r6, #0x64]
	movs r1, #0xd8
	lsls r1, r1, #3
	adds r0, r0, r1
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r6, #0x64]
	movs r2, #0xe0
	lsls r2, r2, #3
	adds r0, r0, r2
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r6, #0x38]
	str r0, [r6, #0x5c]
	ldr r0, _08028258 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80233B4
	adds r7, r0, #0
	movs r0, #1
	rsbs r0, r0, #0
	cmp r7, r0
	beq _0802814E
	b _080283E8
_0802814E:
	bl sub_8031784
	str r0, [r6, #0x3c]
	cmp r0, r7
	bne _0802815A
	b _080283E8
_0802815A:
	ldr r1, _0802825C @ =gStaticData_08174C6C
	ldr r2, [r6, #0x64]
	movs r4, #0xe8
	lsls r4, r4, #3
	adds r3, r2, r4
	adds r0, r1, #0
	adds r0, #0xe8
	ldr r0, [r0]
	adds r1, #0xec
	ldr r1, [r1]
	lsls r0, r0, #8
	str r0, [r3]
	lsls r1, r1, #8
	str r1, [r3, #4]
	movs r4, #0
	ldr r0, [r3, #0x20]
	ldr r5, _08028260 @ =0x0000076D
	adds r2, r2, r5
	ldr r1, [r0]
	ldrb r5, [r2]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _08028192
	subs r4, r0, #1
_08028192:
	str r4, [r3, #0x30]
	adds r0, r3, #0
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r1, [r6, #0x3c]
	ldr r0, [r6, #0x60]
	cmp r1, r0
	bne _080281A8
	b _080283A4
_080281A8:
	cmp r1, #0x64
	bne _08028274
	ldr r3, [r6, #0x64]
	movs r7, #0xf0
	lsls r7, r7, #3
	adds r5, r3, r7
	movs r4, #1
	ldr r0, [r5, #0x20]
	ldr r2, _08028264 @ =0x000007AD
	adds r1, r3, r2
	ldr r2, [r0]
	ldrb r7, [r1]
	lsls r0, r7, #3
	adds r1, r7, #0
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _080281D2
	subs r4, r0, #1
_080281D2:
	str r4, [r5, #0x30]
	movs r0, #0xf8
	lsls r0, r0, #3
	adds r5, r3, r0
	movs r4, #0
	ldr r0, [r5, #0x20]
	ldr r2, _08028268 @ =0x000007ED
	adds r1, r3, r2
	ldr r2, [r0]
	ldrb r7, [r1]
	lsls r0, r7, #3
	adds r1, r7, #0
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _080281F8
	subs r4, r0, #1
_080281F8:
	str r4, [r5, #0x30]
	movs r0, #0x80
	lsls r0, r0, #4
	adds r5, r3, r0
	movs r4, #0
	ldr r0, [r5, #0x20]
	ldr r2, _0802826C @ =0x0000082D
	adds r1, r3, r2
	ldr r2, [r0]
	ldrb r7, [r1]
	lsls r0, r7, #3
	adds r1, r7, #0
	subs r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _0802821E
	subs r4, r0, #1
_0802821E:
	str r4, [r5, #0x30]
	movs r0, #0x84
	lsls r0, r0, #4
	adds r5, r3, r0
	movs r4, #0xa
	ldr r0, [r5, #0x20]
	ldr r2, _08028270 @ =0x0000086D
	adds r1, r3, r2
	ldr r2, [r0]
	ldrb r3, [r1]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r1, [r0, #0x16]
	cmp r4, r1
	blt _08028242
	subs r4, r1, #1
_08028242:
	str r4, [r5, #0x30]
	b _080283A4
	.align 2, 0
_08028248: .4byte 0x0000066D
_0802824C: .4byte 0x000006AD
_08028250: .4byte 0x000006ED
_08028254: .4byte 0x0000072D
_08028258: .4byte gUnknown_030012C0
_0802825C: .4byte gStaticData_08174C6C
_08028260: .4byte 0x0000076D
_08028264: .4byte 0x000007AD
_08028268: .4byte 0x000007ED
_0802826C: .4byte 0x0000082D
_08028270: .4byte 0x0000086D
_08028274:
	cmp r1, #9
	ble _0802830C
	adds r0, r1, #0
	movs r1, #0xa
	bl sub_803ADB4
	ldr r4, [r6, #0x64]
	movs r1, #0xf0
	lsls r1, r1, #3
	adds r5, r4, r1
	adds r3, r0, #0
	ldr r0, [r5, #0x20]
	ldr r2, _08028300 @ =0x000007AD
	adds r1, r4, r2
	ldr r0, [r0]
	mov ip, r0
	ldrb r2, [r1]
	lsls r0, r2, #3
	subs r0, r0, r2
	lsls r0, r0, #2
	add r0, ip
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _080282A6
	subs r3, r0, #1
_080282A6:
	str r3, [r5, #0x30]
	ldr r0, [r6, #0x3c]
	movs r1, #0xa
	bl sub_803AE4C
	movs r3, #0xf8
	lsls r3, r3, #3
	adds r5, r4, r3
	adds r3, r0, #0
	ldr r0, [r5, #0x20]
	ldr r2, _08028304 @ =0x000007ED
	adds r1, r4, r2
	ldr r0, [r0]
	mov ip, r0
	ldrb r2, [r1]
	lsls r0, r2, #3
	subs r0, r0, r2
	lsls r0, r0, #2
	add r0, ip
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _080282D4
	subs r3, r0, #1
_080282D4:
	str r3, [r5, #0x30]
	movs r3, #0x80
	lsls r3, r3, #4
	adds r5, r4, r3
	movs r3, #0xa
	ldr r0, [r5, #0x20]
	ldr r2, _08028308 @ =0x0000082D
	adds r1, r4, r2
	ldr r0, [r0]
	mov ip, r0
	ldrb r2, [r1]
	lsls r0, r2, #3
	subs r0, r0, r2
	lsls r0, r0, #2
	add r0, ip
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _080282FA
	subs r3, r0, #1
_080282FA:
	str r3, [r5, #0x30]
	b _08028380
	.align 2, 0
_08028300: .4byte 0x000007AD
_08028304: .4byte 0x000007ED
_08028308: .4byte 0x0000082D
_0802830C:
	ldr r4, [r6, #0x64]
	movs r0, #0xf0
	lsls r0, r0, #3
	adds r5, r4, r0
	adds r3, r1, #0
	ldr r0, [r5, #0x20]
	ldr r2, _080283F0 @ =0x000007AD
	adds r1, r4, r2
	ldr r0, [r0]
	mov ip, r0
	ldrb r2, [r1]
	lsls r0, r2, #3
	subs r0, r0, r2
	lsls r0, r0, #2
	add r0, ip
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08028332
	subs r3, r0, #1
_08028332:
	str r3, [r5, #0x30]
	movs r3, #0xf8
	lsls r3, r3, #3
	adds r5, r4, r3
	movs r3, #0xa
	ldr r0, [r5, #0x20]
	ldr r2, _080283F4 @ =0x000007ED
	adds r1, r4, r2
	ldr r0, [r0]
	mov ip, r0
	ldrb r2, [r1]
	lsls r0, r2, #3
	subs r0, r0, r2
	lsls r0, r0, #2
	add r0, ip
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08028358
	subs r3, r0, #1
_08028358:
	str r3, [r5, #0x30]
	movs r5, #0x80
	lsls r5, r5, #4
	adds r3, r4, r5
	adds r5, r7, #0
	ldr r0, [r3, #0x20]
	ldr r2, _080283F8 @ =0x0000082D
	adds r1, r4, r2
	ldr r0, [r0]
	mov ip, r0
	ldrb r2, [r1]
	lsls r0, r2, #3
	subs r0, r0, r2
	lsls r0, r0, #2
	add r0, ip
	ldrb r0, [r0, #0x16]
	cmp r7, r0
	blt _0802837E
	subs r5, r0, #1
_0802837E:
	str r5, [r3, #0x30]
_08028380:
	movs r3, #0x84
	lsls r3, r3, #4
	adds r5, r4, r3
	adds r3, r7, #0
	ldr r0, [r5, #0x20]
	ldr r7, _080283FC @ =0x0000086D
	adds r1, r4, r7
	ldr r2, [r0]
	ldrb r4, [r1]
	lsls r0, r4, #3
	subs r0, r0, r4
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r1, [r0, #0x16]
	cmp r3, r1
	blt _080283A2
	subs r3, r1, #1
_080283A2:
	str r3, [r5, #0x30]
_080283A4:
	ldr r0, [r6, #0x64]
	movs r5, #0xf0
	lsls r5, r5, #3
	adds r0, r0, r5
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r6, #0x64]
	movs r7, #0xf8
	lsls r7, r7, #3
	adds r0, r0, r7
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r6, #0x64]
	movs r1, #0x80
	lsls r1, r1, #4
	adds r0, r0, r1
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r6, #0x64]
	movs r2, #0x84
	lsls r2, r2, #4
	adds r0, r0, r2
	movs r1, #0
	movs r2, #0
	bl sub_80270E0
	ldr r0, [r6, #0x3c]
	str r0, [r6, #0x60]
_080283E8:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080283F0: .4byte 0x000007AD
_080283F4: .4byte 0x000007ED
_080283F8: .4byte 0x0000082D
_080283FC: .4byte 0x0000086D

