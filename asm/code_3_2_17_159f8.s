.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_80159F8
sub_80159F8: @ 0x080159F8
	push {r4, r5, lr}
	sub sp, #0x24
	adds r5, r0, #0
	ldr r0, _08015A38 @ =gUnknown_0300082C
	ldr r0, [r0]
	adds r0, #0x10
	str r0, [r5, #0x28]
	ldr r0, [r5, #8]
	cmp r0, #4
	bne _08015A52
	add r1, sp, #4
	ldr r0, _08015A3C @ =gStaticData_0816C090
	ldm r0!, {r2, r3, r4}
	stm r1!, {r2, r3, r4}
	ldm r0!, {r2, r3, r4}
	stm r1!, {r2, r3, r4}
	ldm r0!, {r2, r3}
	stm r1!, {r2, r3}
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	cmp r0, #6
	bne _08015A40
	ldr r1, [r5, #0x10]
	ldr r0, [r1, #0x30]
	lsls r0, r0, #2
	add r0, sp
	adds r0, #4
	ldr r0, [r0]
	str r0, [r1, #0x60]
	b _08015C64
	.align 2, 0
_08015A38: .4byte gUnknown_0300082C
_08015A3C: .4byte gStaticData_0816C090
_08015A40:
	ldr r1, [r5, #0x10]
	ldr r0, [r1, #0x30]
	lsls r0, r0, #2
	add r0, sp
	adds r0, #4
	ldr r0, [r0]
	rsbs r0, r0, #0
	str r0, [r1, #0x60]
	b _08015C64
_08015A52:
	ldr r4, [r5, #0x10]
	adds r1, r4, #0
	adds r1, #0x2d
	ldrb r0, [r1]
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r3, _08015A98 @ =0x7FFFFFFF
	str r3, [sp]
	adds r0, r5, #0
	movs r1, #2
	movs r2, #2
	bl sub_8017264
	adds r0, r5, #0
	adds r0, #0x21
	ldrb r1, [r0]
	adds r3, r0, #0
	cmp r1, #0xc
	bls _08015A8C
	b _08015C64
_08015A8C:
	lsls r0, r1, #2
	ldr r1, _08015A9C @ =_08015AA0
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08015A98: .4byte 0x7FFFFFFF
_08015A9C: .4byte _08015AA0
_08015AA0: @ jump table
	.4byte _08015C30 @ case 0
	.4byte _08015AD4 @ case 1
	.4byte _08015AF8 @ case 2
	.4byte _08015C10 @ case 3
	.4byte _08015B1C @ case 4
	.4byte _08015B44 @ case 5
	.4byte _08015BF2 @ case 6
	.4byte _08015B64 @ case 7
	.4byte _08015B88 @ case 8
	.4byte _08015C3C @ case 9
	.4byte _08015BA8 @ case 10
	.4byte _08015BD0 @ case 11
	.4byte _08015C5C @ case 12
_08015AD4:
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	movs r2, #0xb0
	cmp r0, #0
	bge _08015AE8
	movs r2, #0xb0
	rsbs r2, r2, #0
_08015AE8:
	str r2, [r1, #0x60]
	ldr r0, _08015AF4 @ =0xFFFFFD40
	str r0, [r1, #0x64]
	movs r0, #0
	strb r0, [r3]
	b _08015C64
	.align 2, 0
_08015AF4: .4byte 0xFFFFFD40
_08015AF8:
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	movs r2, #0xb0
	lsls r2, r2, #1
	cmp r0, #0
	bge _08015B0C
	ldr r2, _08015B14 @ =0xFFFFFEA0
_08015B0C:
	str r2, [r1, #0x60]
	ldr r0, _08015B18 @ =0xFFFFFD40
	b _08015B34
	.align 2, 0
_08015B14: .4byte 0xFFFFFEA0
_08015B18: .4byte 0xFFFFFD40
_08015B1C:
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	movs r2, #0xb0
	lsls r2, r2, #2
	cmp r0, #0
	bge _08015B30
	ldr r2, _08015B3C @ =0xFFFFFD40
_08015B30:
	str r2, [r1, #0x60]
	ldr r0, _08015B40 @ =0xFFFFFEA0
_08015B34:
	str r0, [r1, #0x64]
	movs r0, #3
	strb r0, [r3]
	b _08015C64
	.align 2, 0
_08015B3C: .4byte 0xFFFFFD40
_08015B40: .4byte 0xFFFFFEA0
_08015B44:
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	movs r2, #0xb0
	lsls r2, r2, #2
	cmp r0, #0
	bge _08015B58
	ldr r2, _08015B60 @ =0xFFFFFD40
_08015B58:
	str r2, [r1, #0x60]
	movs r0, #0xb0
	rsbs r0, r0, #0
	b _08015B7C
	.align 2, 0
_08015B60: .4byte 0xFFFFFD40
_08015B64:
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	movs r2, #0xb0
	lsls r2, r2, #2
	cmp r0, #0
	bge _08015B78
	ldr r2, _08015B84 @ =0xFFFFFD40
_08015B78:
	str r2, [r1, #0x60]
	movs r0, #0xb0
_08015B7C:
	str r0, [r1, #0x64]
	movs r0, #6
	strb r0, [r3]
	b _08015C64
	.align 2, 0
_08015B84: .4byte 0xFFFFFD40
_08015B88:
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	movs r2, #0xb0
	lsls r2, r2, #2
	cmp r0, #0
	bge _08015B9C
	ldr r2, _08015BA4 @ =0xFFFFFD40
_08015B9C:
	str r2, [r1, #0x60]
	movs r0, #0xb0
	lsls r0, r0, #1
	b _08015BC2
	.align 2, 0
_08015BA4: .4byte 0xFFFFFD40
_08015BA8:
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	movs r2, #0xb0
	lsls r2, r2, #1
	cmp r0, #0
	bge _08015BBC
	ldr r2, _08015BCC @ =0xFFFFFEA0
_08015BBC:
	str r2, [r1, #0x60]
	movs r0, #0xb0
	lsls r0, r0, #2
_08015BC2:
	str r0, [r1, #0x64]
	movs r0, #9
	strb r0, [r3]
	b _08015C64
	.align 2, 0
_08015BCC: .4byte 0xFFFFFEA0
_08015BD0:
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	movs r2, #0xb0
	cmp r0, #0
	bge _08015BE4
	movs r2, #0xb0
	rsbs r2, r2, #0
_08015BE4:
	str r2, [r1, #0x60]
	movs r0, #0xb0
	lsls r0, r0, #2
	str r0, [r1, #0x64]
	movs r0, #0xc
	strb r0, [r3]
	b _08015C64
_08015BF2:
	ldr r2, [r5, #0x10]
	adds r0, r2, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	movs r1, #0xb0
	lsls r1, r1, #2
	cmp r0, #0
	bge _08015C06
	ldr r1, _08015C0C @ =0xFFFFFD40
_08015C06:
	str r1, [r2, #0x60]
	b _08015C64
	.align 2, 0
_08015C0C: .4byte 0xFFFFFD40
_08015C10:
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	movs r2, #0x84
	lsls r2, r2, #2
	cmp r0, #0
	bge _08015C24
	ldr r2, _08015C2C @ =0xFFFFFDF0
_08015C24:
	str r2, [r1, #0x60]
	ldr r0, _08015C2C @ =0xFFFFFDF0
	b _08015C62
	.align 2, 0
_08015C2C: .4byte 0xFFFFFDF0
_08015C30:
	ldr r1, [r5, #0x10]
	ldr r0, _08015C38 @ =0xFFFFFD40
	b _08015C62
	.align 2, 0
_08015C38: .4byte 0xFFFFFD40
_08015C3C:
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	movs r2, #0x84
	lsls r2, r2, #2
	cmp r0, #0
	bge _08015C50
	ldr r2, _08015C58 @ =0xFFFFFDF0
_08015C50:
	str r2, [r1, #0x60]
	movs r0, #0x84
	lsls r0, r0, #2
	b _08015C62
	.align 2, 0
_08015C58: .4byte 0xFFFFFDF0
_08015C5C:
	ldr r1, [r5, #0x10]
	movs r0, #0xb0
	lsls r0, r0, #2
_08015C62:
	str r0, [r1, #0x64]
_08015C64:
	add sp, #0x24
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_8015C6C
sub_8015C6C: @ 0x08015C6C
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r4, r0, #0
	adds r0, #0x23
	ldrb r0, [r0]
	cmp r0, #0
	beq _08015C7C
	b _08015DF0
_08015C7C:
	movs r5, #0xf0
	lsls r5, r5, #2
	movs r6, #0
	ldr r0, _08015CD0 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #9
	bl PlaySfx
	ldr r1, [r4, #0x10]
	adds r1, #0x68
	movs r0, #3
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #0
	beq _08015CB0
	ldr r0, _08015CD4 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #2
	bhi _08015CB0
	movs r6, #1
_08015CB0:
	ldr r0, [r4, #8]
	cmp r0, #4
	bne _08015CD8
	cmp r6, #0
	bne _08015D52
	adds r1, r5, #0
	adds r0, r4, #0
	adds r0, #0x22
	ldrb r0, [r0]
	cmp r0, #7
	bne _08015CC8
	rsbs r1, r5, #0
_08015CC8:
	ldr r0, [r4, #0x10]
	str r1, [r0, #0x60]
	b _08015DF0
	.align 2, 0
_08015CD0: .4byte gUnknown_030012BC
_08015CD4: .4byte gUnknown_03001304
_08015CD8:
	movs r0, #0x18
	str r0, [sp]
	adds r0, r4, #0
	movs r1, #3
	movs r2, #3
	movs r3, #0
	bl sub_8017264
	adds r0, r4, #0
	adds r0, #0x21
	ldrb r0, [r0]
	cmp r0, #0xc
	bls _08015CF4
	b _08015DF0
_08015CF4:
	lsls r0, r0, #2
	ldr r1, _08015D00 @ =_08015D04
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08015D00: .4byte _08015D04
_08015D04: @ jump table
	.4byte _08015DA2 @ case 0
	.4byte _08015DF0 @ case 1
	.4byte _08015DF0 @ case 2
	.4byte _08015D5A @ case 3
	.4byte _08015DF0 @ case 4
	.4byte _08015DF0 @ case 5
	.4byte _08015D38 @ case 6
	.4byte _08015DF0 @ case 7
	.4byte _08015DF0 @ case 8
	.4byte _08015DAA @ case 9
	.4byte _08015DF0 @ case 10
	.4byte _08015DF0 @ case 11
	.4byte _08015DEC @ case 12
_08015D38:
	cmp r6, #0
	bne _08015D52
	ldr r2, [r4, #0x10]
	adds r0, r2, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	adds r1, r5, #0
	cmp r0, #0
	bge _08015D4E
	rsbs r1, r5, #0
_08015D4E:
	str r1, [r2, #0x60]
	b _08015DF0
_08015D52:
	ldr r1, [r4, #0x10]
	movs r0, #0
	str r0, [r1, #0x60]
	b _08015DF0
_08015D5A:
	cmp r6, #0
	bne _08015D8C
	ldr r0, [r4, #0x10]
	adds r1, r0, #0
	adds r1, #0x28
	ldrb r1, [r1]
	lsls r1, r1, #0x1b
	adds r3, r0, #0
	cmp r1, #0
	bge _08015D80
	rsbs r0, r5, #0
	lsls r1, r0, #1
	adds r1, r1, r0
	adds r2, r0, #0
	cmp r1, #0
	bge _08015D7C
	adds r1, #3
_08015D7C:
	asrs r0, r1, #2
	b _08015D88
_08015D80:
	lsls r0, r5, #1
	adds r0, r0, r5
	asrs r0, r0, #2
	rsbs r2, r5, #0
_08015D88:
	str r0, [r3, #0x60]
	b _08015D96
_08015D8C:
	ldr r1, [r4, #0x10]
	movs r0, #0
	str r0, [r1, #0x60]
	adds r3, r1, #0
	rsbs r2, r5, #0
_08015D96:
	lsls r0, r2, #1
	adds r0, r0, r2
	cmp r0, #0
	bge _08015DE6
	adds r0, #3
	b _08015DE6
_08015DA2:
	rsbs r1, r5, #0
	ldr r0, [r4, #0x10]
	str r1, [r0, #0x64]
	b _08015DF0
_08015DAA:
	cmp r6, #0
	bne _08015DDA
	ldr r0, [r4, #0x10]
	adds r1, r0, #0
	adds r1, #0x28
	ldrb r1, [r1]
	lsls r1, r1, #0x1b
	adds r3, r0, #0
	cmp r1, #0
	bge _08015DD0
	rsbs r1, r5, #0
	lsls r0, r1, #1
	adds r0, r0, r1
	cmp r0, #0
	bge _08015DCA
	adds r0, #3
_08015DCA:
	asrs r1, r0, #2
	lsls r0, r5, #1
	b _08015DD6
_08015DD0:
	lsls r0, r5, #1
	adds r1, r0, r5
	asrs r1, r1, #2
_08015DD6:
	str r1, [r3, #0x60]
	b _08015DE4
_08015DDA:
	ldr r1, [r4, #0x10]
	movs r0, #0
	str r0, [r1, #0x60]
	adds r3, r1, #0
	lsls r0, r5, #1
_08015DE4:
	adds r0, r0, r5
_08015DE6:
	asrs r0, r0, #2
	str r0, [r3, #0x64]
	b _08015DF0
_08015DEC:
	ldr r0, [r4, #0x10]
	str r5, [r0, #0x64]
_08015DF0:
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0

	thumb_func_start sub_8015DF8
sub_8015DF8: @ 0x08015DF8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #8
	adds r6, r0, #0
	ldr r0, [r6, #8]
	cmp r0, #2
	bne _08015E14
	movs r0, #0x96
	lsls r0, r0, #1
	mov r8, r0
	movs r5, #0x14
	movs r4, #0x1e
	b _08015E2E
_08015E14:
	cmp r0, #3
	bne _08015E24
	movs r1, #0x96
	lsls r1, r1, #1
	mov r8, r1
	movs r5, #0x14
	movs r4, #0x20
	b _08015E2E
_08015E24:
	movs r2, #0x96
	lsls r2, r2, #1
	mov r8, r2
	movs r5, #0xf
	movs r4, #5
_08015E2E:
	ldr r0, _08015E98 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r7, r0, #0x18
	ldr r0, _08015E9C @ =gUnknown_0300082C
	ldr r0, [r0]
	movs r1, #0x7f
	ands r0, r1
	cmp r0, #0
	bne _08015E86
	movs r0, #2
	bl sub_8000E1C
	lsls r0, r0, #0x10
	cmp r0, #0
	bne _08015E86
	ldr r0, [r6, #0x10]
	ldr r3, [r0]
	asrs r3, r3, #8
	ldr r2, [r0, #4]
	asrs r2, r2, #8
	subs r2, #0x14
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r1, r0, #0x1b
	lsrs r1, r1, #0x1f
	ldr r0, _08015EA0 @ =gUnknown_030012E4
	ldr r0, [r0]
	str r2, [sp]
	str r1, [sp, #4]
	movs r1, #0x28
	movs r2, #4
	bl sub_8025BAC
	adds r1, r0, #0
	cmp r1, #0
	beq _08015E86
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r2, [r1, #0xc]
	ands r0, r2
	strb r0, [r1, #0xc]
_08015E86:
	cmp r7, #8
	bls _08015E8C
	b _08015FD0
_08015E8C:
	lsls r0, r7, #2
	ldr r1, _08015EA4 @ =_08015EA8
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08015E98: .4byte gUnknown_03001304
_08015E9C: .4byte gUnknown_0300082C
_08015EA0: .4byte gUnknown_030012E4
_08015EA4: .4byte _08015EA8
_08015EA8: @ jump table
	.4byte _08015F6E @ case 0
	.4byte _08015FA4 @ case 1
	.4byte _08015FBC @ case 2
	.4byte _08015F84 @ case 3
	.4byte _08015F8E @ case 4
	.4byte _08015ECC @ case 5
	.4byte _08015EF2 @ case 6
	.4byte _08015F44 @ case 7
	.4byte _08015F1C @ case 8
_08015ECC:
	lsls r0, r5, #1
	adds r5, r0, r5
	adds r1, r5, #0
	asrs r6, r1, #2
	mov r0, r8
	rsbs r1, r0, #0
	lsls r0, r1, #1
	adds r4, r0, r1
	adds r2, r4, #0
	cmp r4, #0
	bge _08015EE4
	adds r2, r4, #3
_08015EE4:
	asrs r5, r2, #2
	movs r0, #0
	adds r1, r6, #0
	adds r2, r5, #0
	bl sub_80172D0
	b _08015F38
_08015EF2:
	lsls r0, r5, #1
	adds r4, r0, r5
	adds r1, r4, #0
	asrs r5, r1, #2
	mov r1, r8
	lsls r0, r1, #1
	add r0, r8
	asrs r2, r0, #2
	movs r0, #0
	adds r1, r5, #0
	bl sub_80172D0
	mov r2, r8
	rsbs r1, r2, #0
	lsls r0, r1, #1
	adds r0, r0, r1
	cmp r0, #0
	bge _08015F18
	adds r0, #3
_08015F18:
	asrs r2, r0, #2
	b _08015FB2
_08015F1C:
	lsls r0, r5, #1
	adds r5, r0, r5
	adds r1, r5, #0
	asrs r6, r1, #2
	mov r1, r8
	lsls r0, r1, #1
	adds r4, r0, r1
	adds r2, r4, #0
	asrs r5, r2, #2
	movs r0, #0
	adds r1, r6, #0
	adds r2, r5, #0
	bl sub_80172D0
_08015F38:
	movs r0, #0
	adds r1, r6, #0
	adds r2, r5, #0
	bl sub_8015FDC
	b _08015FD0
_08015F44:
	lsls r0, r5, #1
	adds r4, r0, r5
	adds r1, r4, #0
	asrs r5, r1, #2
	mov r2, r8
	rsbs r1, r2, #0
	lsls r0, r1, #1
	adds r0, r0, r1
	cmp r0, #0
	bge _08015F5A
	adds r0, #3
_08015F5A:
	asrs r2, r0, #2
	movs r0, #0
	adds r1, r5, #0
	bl sub_80172D0
	mov r1, r8
	lsls r0, r1, #1
	add r0, r8
	asrs r2, r0, #2
	b _08015FB2
_08015F6E:
	movs r0, #0
	adds r1, r4, #0
	movs r2, #0
	bl sub_8015FDC
	movs r0, #0
	adds r1, r4, #0
	movs r2, #0
	bl sub_80172D0
	b _08015FD0
_08015F84:
	mov r0, r8
	rsbs r2, r0, #0
	movs r0, #0
	adds r1, r5, #0
	b _08015F94
_08015F8E:
	movs r0, #0
	adds r1, r5, #0
	mov r2, r8
_08015F94:
	bl sub_80172D0
	movs r0, #0
	adds r1, r4, #0
	movs r2, #0
	bl sub_8015FDC
	b _08015FD0
_08015FA4:
	movs r0, #0
	adds r1, r4, #0
	movs r2, #0
	bl sub_80172D0
	mov r1, r8
	rsbs r2, r1, #0
_08015FB2:
	movs r0, #0
	adds r1, r5, #0
	bl sub_8015FDC
	b _08015FD0
_08015FBC:
	movs r0, #0
	adds r1, r4, #0
	movs r2, #0
	bl sub_80172D0
	movs r0, #0
	adds r1, r5, #0
	mov r2, r8
	bl sub_8015FDC
_08015FD0:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
