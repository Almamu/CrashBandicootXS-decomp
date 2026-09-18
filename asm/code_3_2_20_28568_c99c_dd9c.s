.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_802DD9C
sub_802DD9C: @ 0x0802DD9C
	push {r4, r5, lr}
	sub sp, #0x24
	mov r2, sp
	ldr r1, _0802DE58 @ =gStaticData_0817AA8C
	ldm r1!, {r3, r4, r5}
	stm r2!, {r3, r4, r5}
	ldr r1, _0802DE5C @ =gUnknown_030014C4
	ldr r2, [r1]
	asrs r2, r2, #8
	ldr r1, _0802DE60 @ =gUnknown_030014C8
	ldr r3, [r1]
	asrs r3, r3, #8
	mov r1, sp
	ldrh r4, [r1]
	adds r2, r4, r2
	strh r2, [r1]
	ldrh r5, [r1, #4]
	adds r3, r5, r3
	strh r3, [r1, #4]
	add r2, sp, #0x18
	adds r1, r0, #0
	adds r1, #0x38
	ldm r1!, {r3, r4, r5}
	stm r2!, {r3, r4, r5}
	ldr r2, [r0, #0x1c]
	asrs r2, r2, #8
	ldr r4, [r0, #0x20]
	asrs r4, r4, #8
	ldr r3, [r0, #0x24]
	asrs r3, r3, #8
	add r1, sp, #0x18
	ldrh r0, [r1]
	adds r2, r0, r2
	strh r2, [r1]
	ldrh r0, [r1, #2]
	adds r0, r0, r4
	strh r0, [r1, #2]
	ldrh r2, [r1, #4]
	adds r3, r2, r3
	strh r3, [r1, #4]
	add r0, sp, #0xc
	ldm r1!, {r3, r4, r5}
	stm r0!, {r3, r4, r5}
	add r4, sp, #0xc
	adds r0, r4, #0
	adds r1, r4, #0
	movs r2, #0xc
	bl sub_800014C
	mov r1, sp
	movs r0, #4
	ldrsh r2, [r1, r0]
	movs r5, #4
	ldrsh r3, [r4, r5]
	movs r5, #0xa
	ldrsh r0, [r4, r5]
	adds r0, r3, r0
	cmp r2, r0
	bge _0802DE54
	movs r5, #0xa
	ldrsh r0, [r1, r5]
	adds r0, r2, r0
	cmp r0, r3
	ble _0802DE54
	movs r0, #2
	ldrsh r2, [r1, r0]
	movs r5, #2
	ldrsh r3, [r4, r5]
	movs r5, #8
	ldrsh r0, [r4, r5]
	adds r0, r3, r0
	cmp r2, r0
	bge _0802DE54
	movs r5, #8
	ldrsh r0, [r1, r5]
	adds r0, r2, r0
	cmp r0, r3
	ble _0802DE54
	movs r0, #0
	ldrsh r2, [r1, r0]
	movs r5, #0
	ldrsh r3, [r4, r5]
	movs r5, #6
	ldrsh r0, [r4, r5]
	adds r0, r3, r0
	cmp r2, r0
	bge _0802DE54
	movs r4, #6
	ldrsh r0, [r1, r4]
	adds r0, r2, r0
	cmp r0, r3
	bgt _0802DE64
_0802DE54:
	movs r0, #0
	b _0802DE66
	.align 2, 0
_0802DE58: .4byte gStaticData_0817AA8C
_0802DE5C: .4byte gUnknown_030014C4
_0802DE60: .4byte gUnknown_030014C8
_0802DE64:
	movs r0, #1
_0802DE66:
	add sp, #0x24
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_802DE70
sub_802DE70: @ 0x0802DE70
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x100
	movs r0, #0x80
	lsls r0, r0, #0x13
	ldrh r1, [r0]
	movs r3, #0x80
	lsls r3, r3, #3
	adds r2, r3, #0
	orrs r1, r2
	strh r1, [r0]
	mov ip, sp
	movs r6, #0
	movs r5, #0
	ldr r0, _0802DEBC @ =gUnknown_030014C0
	mov sb, r0
	ldr r1, _0802DEC0 @ =gUnknown_03000898
	mov sl, r1
	ldr r3, _0802DEC4 @ =gUnknown_030014BC
	mov r8, r3
	movs r7, #0xff
_0802DEA0:
	movs r4, #0
	lsls r0, r5, #4
	adds r2, r5, #1
	mov r1, ip
	adds r3, r0, r1
_0802DEAA:
	subs r0, r4, #3
	cmp r0, #9
	bhi _0802DEB8
	cmp r5, #2
	ble _0802DEB8
	cmp r5, #0xc
	ble _0802DEC8
_0802DEB8:
	strb r7, [r3]
	b _0802DED2
	.align 2, 0
_0802DEBC: .4byte gUnknown_030014C0
_0802DEC0: .4byte gUnknown_03000898
_0802DEC4: .4byte gUnknown_030014BC
_0802DEC8:
	adds r1, r6, #0
	adds r0, r1, #1
	lsls r0, r0, #0x18
	lsrs r6, r0, #0x18
	strb r1, [r3]
_0802DED2:
	adds r3, #1
	adds r4, #1
	cmp r4, #0xf
	ble _0802DEAA
	adds r5, r2, #0
	cmp r5, #0xf
	ble _0802DEA0
	ldr r1, _0802DF14 @ =0x040000D4
	mov r3, sp
	str r3, [r1]
	ldr r0, _0802DF18 @ =0x0600D000
	str r0, [r1, #4]
	ldr r0, _0802DF1C @ =0x80000080
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	mov ip, sp
	movs r6, #0x80
	movs r5, #0
	movs r7, #0xff
_0802DEF8:
	movs r4, #0
	lsls r0, r5, #4
	adds r2, r5, #1
	mov r1, ip
	adds r3, r0, r1
_0802DF02:
	subs r0, r4, #3
	cmp r0, #9
	bhi _0802DF10
	cmp r5, #2
	ble _0802DF10
	cmp r5, #0xc
	ble _0802DF20
_0802DF10:
	strb r7, [r3]
	b _0802DF2A
	.align 2, 0
_0802DF14: .4byte 0x040000D4
_0802DF18: .4byte 0x0600D000
_0802DF1C: .4byte 0x80000080
_0802DF20:
	adds r1, r6, #0
	adds r0, r1, #1
	lsls r0, r0, #0x18
	lsrs r6, r0, #0x18
	strb r1, [r3]
_0802DF2A:
	adds r3, #1
	adds r4, #1
	cmp r4, #0xf
	ble _0802DF02
	adds r5, r2, #0
	cmp r5, #0xf
	ble _0802DEF8
	ldr r1, _0802DFA8 @ =0x040000D4
	mov r3, sp
	str r3, [r1]
	ldr r0, _0802DFAC @ =0x0600D800
	str r0, [r1, #4]
	ldr r0, _0802DFB0 @ =0x80000080
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r1, _0802DFB4 @ =0x0600BFC0
	movs r3, #0
	adds r0, r1, #0
	adds r0, #0x3c
_0802DF50:
	str r3, [r0]
	subs r0, #4
	cmp r0, r1
	bge _0802DF50
	movs r5, #1
	mov r0, sb
	strb r5, [r0]
	mov r1, r8
	ldr r2, [r1]
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
	adds r0, #4
	mov r3, sl
	ldr r2, [r3]
	movs r1, #1
	bl sub_803AD80
	ldr r0, _0802DFB8 @ =gUnknown_030014C1
	strb r5, [r0]
	bl sub_802DA68
	bl sub_802D9A8
	add sp, #0x100
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802DFA8: .4byte 0x040000D4
_0802DFAC: .4byte 0x0600D800
_0802DFB0: .4byte 0x80000080
_0802DFB4: .4byte 0x0600BFC0
_0802DFB8: .4byte gUnknown_030014C1

