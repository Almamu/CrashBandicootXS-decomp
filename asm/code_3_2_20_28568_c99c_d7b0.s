.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_802D7B0
sub_802D7B0: @ 0x0802D7B0
	push {r4, r5, r6, r7, lr}
	sub sp, #0x24
	ldr r0, _0802D934 @ =gUnknown_030014D0
	ldr r0, [r0]
	cmp r0, #3
	beq _0802D7D4
	ldr r2, _0802D938 @ =gUnknown_030014C4
	ldr r0, _0802D93C @ =gUnknown_03000884
	ldr r0, [r0]
	ldr r0, [r0, #0x1c]
	ldr r1, [r2]
	subs r0, r0, r1
	cmp r0, #0
	bge _0802D7CE
	adds r0, #0x1f
_0802D7CE:
	asrs r0, r0, #5
	adds r0, r1, r0
	str r0, [r2]
_0802D7D4:
	ldr r5, _0802D940 @ =gUnknown_030014BC
	ldr r4, [r5]
	ldr r0, [r4, #8]
	asrs r6, r0, #8
	movs r2, #0x10
	ldrsh r1, [r4, r2]
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
	blt _0802D814
	movs r3, #6
	ldrsh r0, [r1, r3]
	subs r0, r2, r0
	lsls r0, r0, #8
	ldr r1, [r4, #8]
	subs r1, r1, r0
	str r1, [r4, #8]
	movs r0, #1
	strb r0, [r4, #0x12]
_0802D814:
	ldr r1, _0802D944 @ =gStaticData_0817A840
	ldr r7, _0802D934 @ =gUnknown_030014D0
	ldr r0, [r7]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	bl sub_803AD78
	ldr r4, [r5]
	ldr r0, [r4, #8]
	asrs r5, r0, #8
	cmp r6, r5
	beq _0802D85C
	ldr r3, _0802D948 @ =gUnknown_03000898
	ldr r1, [r4, #0xc]
	ldr r2, [r4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	movs r1, #2
	ldrsh r0, [r0, r1]
	adds r0, r0, r5
	ldr r1, [r4, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	adds r0, #4
	ldr r1, _0802D94C @ =gUnknown_030014C0
	ldrb r1, [r1]
	ldr r2, [r3]
	bl sub_803AD80
	ldr r1, _0802D950 @ =gUnknown_030014C1
	movs r0, #1
	strb r0, [r1]
_0802D85C:
	ldr r0, _0802D954 @ =gUnknown_030014CC
	ldr r0, [r0]
	bl sub_8029E34
	bl sub_802D9A8
	mov r1, sp
	ldr r0, _0802D958 @ =gStaticData_0817AA98
	ldm r0!, {r2, r3, r4}
	stm r1!, {r2, r3, r4}
	ldr r0, _0802D938 @ =gUnknown_030014C4
	ldr r1, [r0]
	asrs r1, r1, #8
	ldr r0, _0802D95C @ =gUnknown_030014C8
	ldr r2, [r0]
	asrs r2, r2, #8
	mov r0, sp
	ldrh r5, [r0]
	adds r1, r5, r1
	strh r1, [r0]
	ldrh r1, [r0, #4]
	adds r2, r1, r2
	strh r2, [r0, #4]
	ldr r0, [r7]
	cmp r0, #1
	bls _0802D892
	b _0802D992
_0802D892:
	ldr r2, _0802D93C @ =gUnknown_03000884
	ldr r0, _0802D960 @ =gUnknown_030014A0
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802D992
	ldr r2, [r2]
	add r1, sp, #0x18
	adds r0, r2, #0
	adds r0, #0x38
	ldm r0!, {r3, r4, r5}
	stm r1!, {r3, r4, r5}
	ldr r0, [r2, #0x1c]
	asrs r0, r0, #8
	ldr r3, [r2, #0x20]
	asrs r3, r3, #8
	ldr r2, [r2, #0x24]
	asrs r2, r2, #8
	add r1, sp, #0x18
	ldrh r4, [r1]
	adds r0, r4, r0
	strh r0, [r1]
	ldrh r0, [r1, #2]
	adds r0, r0, r3
	strh r0, [r1, #2]
	ldrh r5, [r1, #4]
	adds r2, r5, r2
	strh r2, [r1, #4]
	add r0, sp, #0xc
	ldm r1!, {r2, r3, r4}
	stm r0!, {r2, r3, r4}
	add r4, sp, #0xc
	adds r0, r4, #0
	adds r1, r4, #0
	movs r2, #0xc
	bl sub_800014C
	mov r1, sp
	movs r5, #4
	ldrsh r2, [r4, r5]
	movs r0, #4
	ldrsh r3, [r1, r0]
	movs r5, #0xa
	ldrsh r0, [r1, r5]
	adds r0, r3, r0
	cmp r2, r0
	bge _0802D930
	movs r5, #0xa
	ldrsh r0, [r4, r5]
	adds r0, r2, r0
	cmp r0, r3
	ble _0802D930
	movs r0, #2
	ldrsh r2, [r4, r0]
	movs r5, #2
	ldrsh r3, [r1, r5]
	movs r5, #8
	ldrsh r0, [r1, r5]
	adds r0, r3, r0
	cmp r2, r0
	bge _0802D930
	movs r5, #8
	ldrsh r0, [r4, r5]
	adds r0, r2, r0
	cmp r0, r3
	ble _0802D930
	movs r0, #0
	ldrsh r2, [r4, r0]
	movs r5, #0
	ldrsh r3, [r1, r5]
	movs r5, #6
	ldrsh r0, [r1, r5]
	adds r0, r3, r0
	cmp r2, r0
	bge _0802D930
	movs r1, #6
	ldrsh r0, [r4, r1]
	adds r0, r2, r0
	cmp r0, r3
	bgt _0802D964
_0802D930:
	movs r0, #0
	b _0802D966
	.align 2, 0
_0802D934: .4byte gUnknown_030014D0
_0802D938: .4byte gUnknown_030014C4
_0802D93C: .4byte gUnknown_03000884
_0802D940: .4byte gUnknown_030014BC
_0802D944: .4byte gStaticData_0817A840
_0802D948: .4byte gUnknown_03000898
_0802D94C: .4byte gUnknown_030014C0
_0802D950: .4byte gUnknown_030014C1
_0802D954: .4byte gUnknown_030014CC
_0802D958: .4byte gStaticData_0817AA98
_0802D95C: .4byte gUnknown_030014C8
_0802D960: .4byte gUnknown_030014A0
_0802D964:
	movs r0, #1
_0802D966:
	cmp r0, #0
	beq _0802D992
	ldr r0, _0802D99C @ =gUnknown_030014D0
	movs r2, #2
	str r2, [r0]
	ldr r0, _0802D9A0 @ =gUnknown_030014BC
	ldr r1, [r0]
	str r2, [r1, #0xc]
	ldr r0, [r1]
	ldrh r0, [r0, #0x18]
	movs r2, #0
	movs r3, #0
	strh r0, [r1, #0x10]
	strb r2, [r1, #0x12]
	str r3, [r1, #8]
	ldr r0, _0802D9A4 @ =gUnknown_03000884
	ldr r0, [r0]
	bl sub_802C018
	movs r0, #0
	bl sub_8029BAC
_0802D992:
	add sp, #0x24
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802D99C: .4byte gUnknown_030014D0
_0802D9A0: .4byte gUnknown_030014BC
_0802D9A4: .4byte gUnknown_03000884

	thumb_func_start sub_802D9A8
sub_802D9A8: @ 0x0802D9A8
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	ldr r0, _0802D9C4 @ =gUnknown_030014CC
	ldr r1, [r0]
	ldr r0, _0802D9C8 @ =0x00004FFF
	cmp r1, r0
	bgt _0802D9DC
	ldr r1, _0802D9CC @ =0x040000D4
	ldr r0, _0802D9D0 @ =gStaticData_0817AA6C
	str r0, [r1]
	ldr r0, _0802D9D4 @ =0x050001E0
	str r0, [r1, #4]
	ldr r0, _0802D9D8 @ =0x80000010
	b _0802D9F4
	.align 2, 0
_0802D9C4: .4byte gUnknown_030014CC
_0802D9C8: .4byte 0x00004FFF
_0802D9CC: .4byte 0x040000D4
_0802D9D0: .4byte gStaticData_0817AA6C
_0802D9D4: .4byte 0x050001E0
_0802D9D8: .4byte 0x80000010
_0802D9DC:
	ldr r0, _0802D9FC @ =0x0000BDFF
	cmp r1, r0
	ble _0802DA0C
	mov r1, sp
	movs r0, #0
	strh r0, [r1]
	ldr r1, _0802DA00 @ =0x040000D4
	mov r0, sp
	str r0, [r1]
	ldr r0, _0802DA04 @ =0x050001E0
	str r0, [r1, #4]
	ldr r0, _0802DA08 @ =0x81000010
_0802D9F4:
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	b _0802DA56
	.align 2, 0
_0802D9FC: .4byte 0x0000BDFF
_0802DA00: .4byte 0x040000D4
_0802DA04: .4byte 0x050001E0
_0802DA08: .4byte 0x81000010
_0802DA0C:
	movs r0, #0xbe
	lsls r0, r0, #8
	subs r0, r0, r1
	lsls r0, r0, #8
	movs r1, #0xdc
	lsls r1, r1, #7
	bl sub_803ADB4
	adds r3, r0, #0
	movs r0, #0x1f
	mov ip, r0
	ldr r6, _0802DA60 @ =0x050001E0
	ldr r5, _0802DA64 @ =gStaticData_0817AA6C
	movs r7, #0x1f
	movs r4, #0xf
_0802DA2A:
	ldrh r1, [r5]
	adds r0, r7, #0
	ands r0, r1
	adds r2, r0, #0
	muls r2, r3, r2
	asrs r2, r2, #8
	lsrs r1, r1, #5
	mov r0, ip
	ands r1, r0
	adds r0, r1, #0
	muls r0, r3, r0
	asrs r0, r0, #8
	lsls r1, r0, #5
	orrs r2, r1
	lsls r0, r0, #0xa
	orrs r2, r0
	strh r2, [r6]
	adds r6, #2
	adds r5, #2
	subs r4, #1
	cmp r4, #0
	bge _0802DA2A
_0802DA56:
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802DA60: .4byte 0x050001E0
_0802DA64: .4byte gStaticData_0817AA6C

	thumb_func_start sub_802DA68
sub_802DA68: @ 0x0802DA68
	push {r4, r5, r6, lr}
	ldr r0, _0802DA84 @ =gUnknown_030014C1
	ldrb r1, [r0]
	adds r3, r0, #0
	cmp r1, #0
	beq _0802DAA8
	ldr r0, _0802DA88 @ =gUnknown_030014C0
	ldrb r1, [r0]
	adds r2, r0, #0
	cmp r1, #0
	beq _0802DA94
	ldr r1, _0802DA8C @ =0x0400000C
	ldr r4, _0802DA90 @ =0x00001A09
	b _0802DA98
	.align 2, 0
_0802DA84: .4byte gUnknown_030014C1
_0802DA88: .4byte gUnknown_030014C0
_0802DA8C: .4byte 0x0400000C
_0802DA90: .4byte 0x00001A09
_0802DA94:
	ldr r1, _0802DB10 @ =0x0400000C
	ldr r4, _0802DB14 @ =0x00001B09
_0802DA98:
	adds r0, r4, #0
	strh r0, [r1]
	movs r0, #0
	strb r0, [r3]
	movs r0, #1
	ldrb r1, [r2]
	eors r0, r1
	strb r0, [r2]
_0802DAA8:
	ldr r5, _0802DB18 @ =gUnknown_030014CC
	ldr r0, [r5]
	lsls r0, r0, #8
	movs r1, #0xaa
	lsls r1, r1, #7
	bl sub_803ADB4
	adds r4, r0, #0
	bl sub_8029EB4
	adds r6, r0, #0
	ldr r0, _0802DB1C @ =gUnknown_030014C4
	ldr r1, [r0]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #4
	subs r0, r0, r1
	lsls r0, r0, #8
	ldr r1, [r5]
	bl sub_803ADB4
	adds r0, r0, r6
	ldr r2, _0802DB20 @ =0x04000028
	adds r1, r0, #0
	muls r1, r4, r1
	asrs r1, r1, #8
	movs r0, #0x80
	lsls r0, r0, #7
	subs r0, r0, r1
	str r0, [r2]
	bl sub_8029E98
	ldr r2, _0802DB24 @ =0x0400002C
	adds r1, r0, #0
	muls r1, r4, r1
	asrs r1, r1, #8
	movs r0, #0x88
	lsls r0, r0, #7
	subs r0, r0, r1
	str r0, [r2]
	ldr r0, _0802DB28 @ =0x04000020
	strh r4, [r0]
	adds r0, #2
	movs r1, #0
	strh r1, [r0]
	adds r0, #2
	strh r1, [r0]
	adds r0, #2
	strh r4, [r0]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802DB10: .4byte 0x0400000C
_0802DB14: .4byte 0x00001B09
_0802DB18: .4byte gUnknown_030014CC
_0802DB1C: .4byte gUnknown_030014C4
_0802DB20: .4byte 0x04000028
_0802DB24: .4byte 0x0400002C
_0802DB28: .4byte 0x04000020

