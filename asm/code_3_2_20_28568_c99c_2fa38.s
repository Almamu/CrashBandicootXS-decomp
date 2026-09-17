.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_802FA38
sub_802FA38: @ 0x0802FA38
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	adds r4, r0, #0
	ldr r1, [r4, #0x34]
	movs r0, #0xd8
	lsls r0, r0, #5
	cmp r1, r0
	ble _0802FA56
	adds r1, r4, #0
	adds r1, #0x2c
	movs r0, #1
	b _0802FA5C
_0802FA56:
	adds r1, r4, #0
	adds r1, #0x2c
	movs r0, #0
_0802FA5C:
	strb r0, [r1]
	ldr r1, [r4, #0x60]
	asrs r1, r1, #4
	ldr r0, [r4, #0x1c]
	adds r0, r0, r1
	str r0, [r4, #0x1c]
	ldr r1, [r4, #0x64]
	asrs r1, r1, #4
	ldr r0, [r4, #0x20]
	adds r0, r0, r1
	str r0, [r4, #0x20]
	ldr r1, [r4, #0x68]
	asrs r1, r1, #4
	ldr r0, [r4, #0x24]
	adds r0, r0, r1
	str r0, [r4, #0x24]
	ldr r1, _0802FAA4 @ =gStaticData_0817C260
	ldr r0, [r4, #0x28]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r7, #2
	ldrsh r2, [r0, r7]
	adds r7, r1, #0
	cmp r2, #0
	ble _0802FAA8
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
	b _0802FAAE
	.align 2, 0
_0802FAA4: .4byte gStaticData_0817C260
_0802FAA8:
	adds r0, r7, #4
	adds r0, r3, r0
	ldr r3, [r0]
_0802FAAE:
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _0802FAC4
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _0802FAC6
_0802FAC4:
	adds r0, r1, #0
_0802FAC6:
	adds r0, r4, r0
	bl sub_803AD84
	ldr r0, [r4, #0xc]
	cmp r0, #3
	bne _0802FB76
	ldr r0, [r4, #0x58]
	mov r8, r0
	cmp r0, #0
	bne _0802FB70
	ldr r0, _0802FB5C @ =gUnknown_03000884
	ldr r5, [r0]
	ldr r0, [r5, #0x24]
	adds r0, #0xa
	ldr r1, [r4, #0x24]
	mov sb, r1
	subs r0, r0, r1
	ldr r1, _0802FB60 @ =0xFFFFFE56
	bl sub_803ADB4
	adds r2, r0, #0
	cmp r2, #0
	ble _0802FB76
	ldr r1, [r4, #0x34]
	ldr r0, _0802FB64 @ =0x00008BFF
	cmp r1, r0
	bgt _0802FB76
	movs r0, #0x80
	lsls r0, r0, #5
	adds r1, r2, #0
	bl sub_803ADB4
	ldr r1, [r5, #0x1c]
	ldr r7, [r4, #0x1c]
	subs r1, r1, r7
	adds r3, r1, #0
	muls r3, r0, r3
	asrs r2, r3, #0xc
	mov ip, r2
	ldr r1, [r5, #0x20]
	ldr r6, [r4, #0x20]
	subs r1, r1, r6
	adds r2, r1, #0
	muls r2, r0, r2
	asrs r5, r2, #0xc
	asrs r3, r3, #0x1f
	mov r1, ip
	eors r1, r3
	subs r1, r1, r3
	asrs r2, r2, #0x1f
	adds r0, r5, #0
	eors r0, r2
	subs r0, r0, r2
	adds r1, r1, r0
	ldr r0, _0802FB68 @ =0x000005FF
	cmp r1, r0
	bgt _0802FB76
	mov r2, sb
	subs r2, #0xa
	str r5, [sp]
	adds r0, r7, #0
	adds r1, r6, #0
	mov r3, ip
	bl sub_802E674
	ldr r0, [r4, #0x5c]
	adds r0, #1
	str r0, [r4, #0x5c]
	cmp r0, #3
	bne _0802FB6C
	mov r3, r8
	str r3, [r4, #0x5c]
	movs r0, #0x3c
	b _0802FB74
	.align 2, 0
_0802FB5C: .4byte gUnknown_03000884
_0802FB60: .4byte 0xFFFFFE56
_0802FB64: .4byte 0x00008BFF
_0802FB68: .4byte 0x000005FF
_0802FB6C:
	movs r0, #0x14
	b _0802FB74
_0802FB70:
	mov r0, r8
	subs r0, #1
_0802FB74:
	str r0, [r4, #0x58]
_0802FB76:
	adds r0, r4, #0
	adds r0, #0x7c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802FBB0
	adds r0, r4, #0
	bl sub_802A6EC
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802FBB0
	ldr r0, _0802FBD8 @ =gUnknown_03000884
	ldr r0, [r0]
	ldr r2, [r0, #0x50]
	movs r7, #0x20
	ldrsh r1, [r2, r7]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	movs r1, #6
	bl sub_803AD80
	ldr r1, [r4, #0x50]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #4
	bl sub_803AD80
_0802FBB0:
	ldr r0, [r4, #0x28]
	cmp r0, #3
	bne _0802FBDC
	ldr r1, [r4, #0x20]
	movs r0, #0xe1
	lsls r0, r0, #8
	cmp r1, r0
	ble _0802FBDC
	cmp r4, #0
	beq _0802FBE2
	ldr r1, [r4, #0x50]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
	b _0802FBE2
	.align 2, 0
_0802FBD8: .4byte gUnknown_03000884
_0802FBDC:
	adds r0, r4, #0
	bl sub_802A7B8
_0802FBE2:
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

