.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8037FC0
sub_8037FC0: @ 0x08037FC0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x1c
	str r0, [sp]
	movs r7, #0
	movs r0, #0
	str r0, [sp, #8]
	ldr r1, [sp]
	ldr r0, [r1, #0x30]
	cmp r0, #0
	bne _08037FE0
	ldr r0, _08037FF4 @ =gStaticData_085A4C5C
	str r0, [r1, #0x30]
_08037FE0:
	ldr r2, [sp]
	ldrh r1, [r2, #8]
	ldr r0, _08037FF8 @ =0x0000FFFF
	cmp r1, r0
	bne _08037FFC
	ldr r0, [r2, #0x30]
	ldr r0, [r0, #8]
	ldr r0, [r0, #0x18]
	ldrh r2, [r0, #0x18]
	b _08038000
	.align 2, 0
_08037FF4: .4byte gStaticData_085A4C5C
_08037FF8: .4byte 0x0000FFFF
_08037FFC:
	ldr r4, [sp]
	ldrh r2, [r4, #8]
_08038000:
	ldr r6, [sp]
	ldrh r1, [r6, #0xe]
	ldr r0, _08038014 @ =0x0000FFFF
	cmp r1, r0
	bne _08038018
	ldr r0, [r6, #0x30]
	ldr r0, [r0, #8]
	ldr r0, [r0, #0x18]
	ldrb r0, [r0, #0x1a]
	b _0803801C
	.align 2, 0
_08038014: .4byte 0x0000FFFF
_08038018:
	ldr r0, [sp]
	ldrh r0, [r0, #0xe]
_0803801C:
	mov r8, r0
	ldr r1, [sp]
	ldr r0, [r1, #0x2c]
	cmp r0, #0
	bne _0803802E
	movs r4, #0
	mov r8, r4
	mov r6, r8
	strh r6, [r1, #0xe]
_0803802E:
	ldr r4, _080381C0 @ =gStaticData_085A6150
	adds r0, r2, #0
	bl sub_8037FA0
	lsls r0, r0, #3
	adds r0, r0, r4
	ldr r0, [r0]
	str r0, [sp, #4]
	ldr r0, [sp]
	ldrh r3, [r0, #0xc]
	movs r0, #4
	ands r0, r3
	cmp r0, #0
	beq _0803804C
	adds r7, #0xf0
_0803804C:
	movs r1, #0xc6
	lsls r1, r1, #1
	adds r7, r7, r1
	ldr r4, [sp]
	ldr r2, [r4, #0x30]
	ldr r1, [r2]
	mov r6, r8
	adds r0, r1, r6
	lsls r0, r0, #2
	adds r7, r7, r0
	adds r7, #8
	movs r4, #0
	str r2, [sp, #0x10]
	mov r0, sp
	strh r3, [r0, #0x18]
	ldr r6, [sp, #4]
	lsls r6, r6, #5
	str r6, [sp, #0x14]
	cmp r4, r1
	bhs _080380A0
	mov r0, r8
	lsls r3, r0, #2
_08038078:
	lsls r0, r4, #2
	adds r0, r0, r2
	ldr r2, [r0, #4]
	cmp r4, #2
	beq _08038096
	adds r1, r7, #0
	adds r1, #0xc
	ldr r0, [r2, #0x14]
	adds r7, r1, r0
	ldr r0, [r2, #0xc]
	lsls r0, r0, #2
	adds r7, r7, r0
	cmp r4, #0
	bne _08038096
	adds r7, r7, r3
_08038096:
	adds r4, #1
	ldr r2, [sp, #0x10]
	ldr r0, [r2]
	cmp r4, r0
	blo _08038078
_080380A0:
	movs r0, #0x10
	mov r1, sp
	ldrh r1, [r1, #0x18]
	ands r0, r1
	cmp r0, #0
	bne _08038114
	ldr r2, [sp, #0x10]
	ldr r2, [r2, #0xc]
	mov sb, r2
	cmp r2, #0
	beq _08038114
	movs r4, #0
	str r4, [sp, #0xc]
	movs r5, #0
	ldr r6, [r2]
	mov sl, r6
	cmp r4, sl
	bge _08038110
_080380C4:
	lsls r0, r5, #2
	add r0, sb
	ldr r3, [r0, #4]
	ldr r2, [r3]
	mov r1, r8
	adds r0, r2, r1
	lsls r1, r0, #2
	movs r4, #0
	adds r5, #1
	mov ip, r5
	cmp r4, r2
	bhs _08038102
	mov r0, r8
	lsls r6, r0, #2
	adds r5, r2, #0
_080380E2:
	ldr r2, [r3, #4]
	cmp r4, #2
	beq _080380FA
	adds r1, #0xc
	ldr r0, [r2, #0x14]
	adds r1, r1, r0
	ldr r0, [r2, #0xc]
	lsls r0, r0, #2
	adds r1, r1, r0
	cmp r4, #0
	bne _080380FA
	adds r1, r1, r6
_080380FA:
	adds r3, #4
	adds r4, #1
	cmp r4, r5
	blo _080380E2
_08038102:
	ldr r2, [sp, #0xc]
	cmp r1, r2
	ble _0803810A
	str r1, [sp, #0xc]
_0803810A:
	mov r5, ip
	cmp r5, sl
	blt _080380C4
_08038110:
	ldr r4, [sp, #0xc]
	adds r7, r7, r4
_08038114:
	movs r4, #0
	cmp r4, r8
	bhs _08038122
_0803811A:
	adds r7, #0x58
	adds r4, #1
	cmp r4, r8
	blo _0803811A
_08038122:
	movs r4, #0
	ldr r6, [sp, #0x10]
	ldr r0, [r6, #4]
	ldr r1, [r0, #0x18]
_0803812A:
	ldr r0, [r1, #4]
	ldr r2, [sp, #8]
	cmp r0, r2
	bls _08038134
	str r0, [sp, #8]
_08038134:
	adds r1, #8
	adds r4, #1
	cmp r4, #2
	bls _0803812A
	movs r0, #0x10
	mov r4, sp
	ldrh r4, [r4, #0x18]
	ands r0, r4
	cmp r0, #0
	bne _08038182
	ldr r6, [sp, #0x10]
	ldr r0, [r6, #0xc]
	cmp r0, #0
	beq _08038182
	adds r3, r0, #0
	movs r1, #0
	ldr r0, [r3]
	cmp r1, r0
	bge _08038182
	adds r5, r0, #0
_0803815C:
	lsls r0, r1, #2
	adds r0, r0, r3
	ldr r0, [r0, #4]
	movs r4, #0
	adds r2, r1, #1
	ldr r0, [r0, #4]
	ldr r1, [r0, #0x18]
_0803816A:
	ldr r0, [r1, #4]
	ldr r6, [sp, #8]
	cmp r0, r6
	bls _08038174
	str r0, [sp, #8]
_08038174:
	adds r1, #8
	adds r4, #1
	cmp r4, #2
	bls _0803816A
	adds r1, r2, #0
	cmp r1, r5
	blt _0803815C
_08038182:
	ldr r0, [sp, #8]
	cmp r0, #0
	beq _0803819E
	adds r1, r0, #0
	ldr r2, [sp, #4]
	adds r0, r1, #0
	muls r0, r2, r0
	movs r1, #0xfa
	lsls r1, r1, #2
	bl sub_8037E54
	lsls r0, r0, #1
	adds r7, r7, r0
	adds r7, #0x18
_0803819E:
	ldr r4, [sp, #0x10]
	ldr r0, [r4, #8]
	ldr r0, [r0, #0x18]
	ldrb r0, [r0, #0x1b]
	cmp r0, #0
	bne _080381B6
	movs r0, #0x20
	mov r6, sp
	ldrh r6, [r6, #0x18]
	ands r0, r6
	cmp r0, #0
	beq _080381C4
_080381B6:
	movs r0, #0x98
	lsls r0, r0, #1
	adds r7, r7, r0
	b _080381C6
	.align 2, 0
_080381C0: .4byte gStaticData_085A6150
_080381C4:
	adds r7, #0xdc
_080381C6:
	ldr r1, [sp, #0x14]
	ldr r2, [sp, #4]
	subs r0, r1, r2
	lsls r0, r0, #2
	adds r0, r0, r2
	lsls r0, r0, #3
	ldr r1, _080381F8 @ =0x0000E94F
	bl sub_8037E54
	lsls r0, r0, #1
	adds r7, r7, r0
	adds r7, r7, r0
	adds r0, r7, #0
	adds r0, #0x20
	ldr r4, [sp]
	str r0, [r4, #4]
	add sp, #0x1c
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080381F8: .4byte 0x0000E94F

