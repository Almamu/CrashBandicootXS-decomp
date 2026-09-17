.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8015840
sub_8015840: @ 0x08015840
	push {r4, r5, lr}
	sub sp, #4
	adds r5, r0, #0
	movs r4, #0
	str r4, [sp]
	movs r1, #0
	movs r2, #0x12
	movs r3, #0
	bl sub_8015780
	adds r0, r5, #0
	adds r0, #0x31
	strb r4, [r0]
	subs r0, #2
	movs r1, #1
	strb r1, [r0]
	subs r0, #8
	strb r4, [r0]
	adds r0, #0xb
	strb r4, [r0]
	subs r0, #2
	strb r1, [r0]
	subs r0, #8
	strb r4, [r0]
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_8015878
sub_8015878: @ 0x08015878
	push {lr}
	ldr r2, _08015888 @ =gStaticData_087E4224
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_08015888: .4byte gStaticData_087E4224

	thumb_func_start sub_801588C
sub_801588C: @ 0x0801588C
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800B8C8
	ldr r0, _080158A8 @ =gStaticData_087E4224
	str r0, [r4, #0xc]
	adds r0, r4, #0
	bl sub_8011B90
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_080158A8: .4byte gStaticData_087E4224

	thumb_func_start sub_80158AC
sub_80158AC: @ 0x080158AC
	movs r1, #0
	str r1, [r0, #0x14]
	bx lr
	.align 2, 0

	thumb_func_start sub_80158B4
sub_80158B4: @ 0x080158B4
	adds r0, #0x32
	movs r1, #1
	strb r1, [r0]
	bx lr

	thumb_func_start sub_80158BC
sub_80158BC: @ 0x080158BC
	adds r0, #0x31
	movs r1, #1
	strb r1, [r0]
	bx lr

	thumb_func_start sub_80158C4
sub_80158C4: @ 0x080158C4
	adds r0, #0x30
	movs r1, #1
	strb r1, [r0]
	bx lr

	thumb_func_start sub_80158CC
sub_80158CC: @ 0x080158CC
	adds r0, #0x2f
	movs r1, #1
	strb r1, [r0]
	bx lr

	thumb_func_start sub_80158D4
sub_80158D4: @ 0x080158D4
	adds r0, #0x30
	movs r1, #0
	strb r1, [r0]
	bx lr

	thumb_func_start sub_80158DC
sub_80158DC: @ 0x080158DC
	adds r0, #0x2f
	movs r1, #0
	strb r1, [r0]
	bx lr

	thumb_func_start sub_80158E4
sub_80158E4: @ 0x080158E4
	adds r0, #0x30
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_80158EC
sub_80158EC: @ 0x080158EC
	adds r0, #0x2f
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_80158F4
sub_80158F4: @ 0x080158F4
	adds r3, r0, #0
	adds r0, #0x32
	movs r2, #1
	strb r2, [r0]
	subs r0, #2
	strb r2, [r0]
	subs r0, #8
	strb r1, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8015908
sub_8015908: @ 0x08015908
	adds r2, r0, #0
	adds r3, r2, #0
	adds r3, #0x31
	movs r0, #1
	strb r0, [r3]
	subs r3, #2
	strb r0, [r3]
	adds r0, r2, #0
	adds r0, #0x27
	strb r1, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8015920
sub_8015920: @ 0x08015920
	adds r3, r0, #0
	adds r2, r3, #0
	adds r2, #0x32
	movs r0, #0
	strb r0, [r2]
	subs r2, #2
	movs r0, #1
	strb r0, [r2]
	adds r0, r3, #0
	adds r0, #0x28
	strb r1, [r0]
	bx lr

	thumb_func_start sub_8015938
sub_8015938: @ 0x08015938
	adds r2, r0, #0
	adds r3, r2, #0
	adds r3, #0x31
	movs r0, #0
	strb r0, [r3]
	subs r3, #2
	movs r0, #1
	strb r0, [r3]
	adds r0, r2, #0
	adds r0, #0x27
	strb r1, [r0]
	bx lr

	thumb_func_start sub_8015950
sub_8015950: @ 0x08015950
	adds r0, #0x2d
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8015958
sub_8015958: @ 0x08015958
	adds r3, r0, #0
	adds r0, #0x26
	movs r1, #0
	strb r1, [r0]
	str r1, [r3, #8]
	subs r0, #2
	strb r1, [r0]
	adds r0, #1
	strb r1, [r0]
	adds r0, #7
	movs r2, #1
	strb r2, [r0]
	adds r0, #1
	strb r2, [r0]
	str r1, [r3, #0x14]
	str r1, [r3, #0x10]
	subs r0, #0xb
	strb r1, [r0]
	adds r0, #1
	strb r1, [r0]
	adds r0, #4
	strb r1, [r0]
	subs r0, #7
	strb r1, [r0]
	adds r2, r3, #0
	adds r2, #0x21
	movs r0, #6
	strb r0, [r2]
	str r1, [r3, #0x18]
	str r1, [r3, #0x1c]
	ldr r0, _080159A0 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x92
	strb r1, [r0]
	bx lr
	.align 2, 0
_080159A0: .4byte gUnknown_030012D8

	thumb_func_start sub_80159A4
sub_80159A4: @ 0x080159A4
	push {r4, r5, lr}
	sub sp, #4
	adds r5, r0, #0
	movs r4, #0
	str r4, [sp]
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_8017264
	adds r0, r5, #0
	adds r0, #0x27
	strb r4, [r0]
	subs r0, #7
	strb r4, [r0]
	adds r1, r5, #0
	adds r1, #0x21
	movs r0, #6
	strb r0, [r1]
	adds r0, r5, #0
	adds r0, #0x22
	strb r4, [r0]
	ldr r0, _080159F4 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x92
	strb r4, [r0]
	adds r0, r5, #0
	adds r0, #0x2c
	movs r1, #1
	strb r1, [r0]
	subs r0, #8
	strb r4, [r0]
	adds r0, #9
	strb r1, [r0]
	subs r0, #8
	strb r4, [r0]
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080159F4: .4byte gUnknown_030012D8

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

	thumb_func_start sub_8015FDC
sub_8015FDC: @ 0x08015FDC
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	mov ip, r1
	adds r5, r2, #0
	ldr r0, _08016020 @ =gUnknown_030012D8
	ldr r3, [r0]
	ldr r4, [r3, #0x64]
	adds r1, r4, #0
	muls r1, r4, r1
	cmp r1, #0
	bge _08015FF6
	ldr r0, _08016024 @ =0x00003FFF
	adds r1, r1, r0
_08015FF6:
	asrs r1, r1, #0xe
	adds r1, #4
	lsls r0, r1, #1
	adds r0, r0, r1
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r7, r0, #1
	asrs r0, r4, #0x1f
	adds r1, r4, #0
	eors r1, r0
	subs r1, r1, r0
	asrs r2, r5, #0x1f
	adds r0, r5, #0
	eors r0, r2
	subs r0, r0, r2
	cmp r1, r0
	ble _08016028
	str r6, [r3, #0x54]
	str r7, [r3, #0x58]
	b _0801603E
	.align 2, 0
_08016020: .4byte gUnknown_030012D8
_08016024: .4byte 0x00003FFF
_08016028:
	adds r0, r4, #0
	muls r0, r5, r0
	cmp r0, #0
	bge _08016038
	mov r1, ip
	adds r0, r7, r1
	str r6, [r3, #0x54]
	b _0801603C
_08016038:
	str r6, [r3, #0x54]
	mov r0, ip
_0801603C:
	str r0, [r3, #0x58]
_0801603E:
	str r5, [r3, #0x5c]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	non_word_aligned_thumb_func_start sub_8016046
sub_8016046: @ 0x08016046
	movs r0, r0

	thumb_func_start sub_8016048
sub_8016048: @ 0x08016048
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _080160B4 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r2, r0, #0x18
	ldr r0, [r4, #8]
	cmp r0, #0
	blt _0801611A
	cmp r0, #3
	ble _0801606C
	cmp r0, #6
	bgt _0801611A
	cmp r0, #5
	blt _0801611A
_0801606C:
	ldr r1, [r4, #0x10]
	adds r1, #0x28
	movs r0, #0x21
	rsbs r0, r0, #0
	ldrb r3, [r1]
	ands r0, r3
	strb r0, [r1]
	ldr r0, [r4, #0x10]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _080160CC
	cmp r2, #4
	beq _08016092
	cmp r2, #6
	beq _08016092
	cmp r2, #8
	bne _080160CC
_08016092:
	ldr r0, [r4, #0x10]
	adds r0, #0x28
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r2, [r0]
	ands r1, r2
	strb r1, [r0]
	ldr r0, [r4, #8]
	cmp r0, #3
	beq _080160BC
	ldr r3, _080160B8 @ =0x7FFFFFFF
	str r3, [sp]
	adds r0, r4, #0
	movs r1, #4
	movs r2, #4
	b _080160F4
	.align 2, 0
_080160B4: .4byte gUnknown_03001304
_080160B8: .4byte 0x7FFFFFFF
_080160BC:
	ldr r3, _080160C8 @ =0x7FFFFFFF
	str r3, [sp]
	adds r0, r4, #0
	movs r1, #4
	movs r2, #5
	b _0801610E
	.align 2, 0
_080160C8: .4byte 0x7FFFFFFF
_080160CC:
	ldr r0, [r4, #0x10]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	blt _0801611A
	cmp r2, #3
	beq _080160E4
	cmp r2, #5
	beq _080160E4
	cmp r2, #7
	bne _0801611A
_080160E4:
	ldr r0, [r4, #8]
	cmp r0, #3
	beq _08016104
	ldr r3, _08016100 @ =0x7FFFFFFF
	str r3, [sp]
	adds r0, r4, #0
	movs r1, #4
	movs r2, #6
_080160F4:
	bl sub_8017264
	movs r0, #0
	str r0, [r4, #0x1c]
	b _08016112
	.align 2, 0
_08016100: .4byte 0x7FFFFFFF
_08016104:
	ldr r3, _08016124 @ =0x7FFFFFFF
	str r3, [sp]
	adds r0, r4, #0
	movs r1, #4
	movs r2, #7
_0801610E:
	bl sub_8017264
_08016112:
	adds r1, r4, #0
	adds r1, #0x26
	movs r0, #0
	strb r0, [r1]
_0801611A:
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08016124: .4byte 0x7FFFFFFF

	thumb_func_start sub_8016128
sub_8016128: @ 0x08016128
	push {r4, lr}
	adds r4, r0, #0
	subs r0, r2, #1
	cmp r0, #0xc
	bhi _080161E4
	lsls r0, r0, #2
	ldr r1, _0801613C @ =_08016140
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801613C: .4byte _08016140
_08016140: @ jump table
	.4byte _080161DC @ case 0
	.4byte _080161E4 @ case 1
	.4byte _080161C8 @ case 2
	.4byte _080161D2 @ case 3
	.4byte _080161BE @ case 4
	.4byte _080161DC @ case 5
	.4byte _080161E4 @ case 6
	.4byte _080161E4 @ case 7
	.4byte _080161E4 @ case 8
	.4byte _080161DC @ case 9
	.4byte _080161E4 @ case 10
	.4byte _08016174 @ case 11
	.4byte _080161E4 @ case 12
_08016174:
	movs r2, #3
	ands r2, r3
	cmp r2, #2
	bne _0801619A
	ldr r0, [r4, #0x10]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _080161B6
	movs r2, #0
	adds r1, r4, #0
	adds r1, #0x2c
	movs r0, #1
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x24
	strb r2, [r0]
	b _080161B6
_0801619A:
	cmp r2, #1
	bne _080161E4
	ldr r0, [r4, #0x10]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	lsrs r1, r0, #0x1f
	cmp r1, #0
	bne _080161B6
	adds r0, r4, #0
	adds r0, #0x2c
	strb r2, [r0]
	subs r0, #8
	strb r1, [r0]
_080161B6:
	ldr r1, [r4, #0x10]
	movs r0, #0
	str r0, [r1, #0x60]
	b _080161E4
_080161BE:
	adds r0, r4, #0
	movs r1, #0x2d
	bl sub_80161EC
	b _080161E4
_080161C8:
	adds r0, r4, #0
	movs r1, #0x2b
	bl sub_80161EC
	b _080161E4
_080161D2:
	adds r0, r4, #0
	movs r1, #0x2c
	bl sub_80161EC
	b _080161E4
_080161DC:
	adds r0, r4, #0
	movs r1, #0x2e
	bl sub_80161EC
_080161E4:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80161EC
sub_80161EC: @ 0x080161EC
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, _0801627C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x1b
	bl PlaySfx
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #7
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	adds r2, r5, #0
	bl sub_803AD84
	ldr r1, [r4, #0x10]
	movs r0, #0x7f
	ldrb r2, [r1, #0xc]
	ands r0, r2
	strb r0, [r1, #0xc]
	ldr r1, [r4, #0x10]
	movs r0, #0x41
	rsbs r0, r0, #0
	ldrb r5, [r1, #0xc]
	ands r0, r5
	strb r0, [r1, #0xc]
	ldr r0, [r4, #0x10]
	movs r1, #0x82
	lsls r1, r1, #1
	adds r0, r0, r1
	movs r1, #1
	strb r1, [r0]
	ldr r0, _08016280 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023234
	ldr r0, _08016284 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r3, [r4, #0x10]
	adds r1, r3, #0
	adds r1, #0x29
	ldrb r1, [r1]
	lsls r1, r1, #0x1c
	lsrs r1, r1, #0x1c
	ldr r2, [r3, #0x20]
	adds r3, #0x2d
	ldr r4, [r2]
	ldrb r5, [r3]
	lsls r2, r5, #3
	subs r2, r2, r5
	lsls r2, r2, #2
	adds r2, r2, r4
	ldrb r2, [r2, #0x14]
	bl sub_8006D08
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0801627C: .4byte gUnknown_030012BC
_08016280: .4byte gUnknown_030012C0
_08016284: .4byte gUnknown_030012B8

	thumb_func_start sub_8016288
sub_8016288: @ 0x08016288
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	adds r5, r0, #0
	ldr r0, [r5, #8]
	cmp r0, #7
	bne _080162E2
	ldr r0, _080162BC @ =gStaticData_0816C250
	adds r1, r0, #0
	adds r1, #0x38
	movs r6, #2
	ldrsh r2, [r1, r6]
	adds r7, r0, #0
	cmp r2, #0
	ble _080162C0
	movs r3, #4
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r1, [r0]
	lsls r0, r2, #3
	adds r0, r0, r1
	subs r0, #8
	ldr r3, [r0]
	ldr r4, [r0, #4]
	adds r6, r4, #0
	b _080162C2
	.align 2, 0
_080162BC: .4byte gStaticData_0816C250
_080162C0:
	ldr r6, [r7, #0x3c]
_080162C2:
	ldr r0, [r5, #8]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _080162D8
	lsls r0, r3, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _080162DA
_080162D8:
	adds r0, r1, #0
_080162DA:
	adds r0, r5, r0
	bl sub_803AD90
	b _08016A88
_080162E2:
	adds r1, r5, #0
	adds r1, #0x23
	ldrb r0, [r1]
	cmp r0, #0
	beq _080162F0
	subs r0, #1
	strb r0, [r1]
_080162F0:
	ldr r0, _0801632C @ =gUnknown_03001304
	ldr r0, [r0]
	ldr r1, _08016330 @ =gUnknown_030007E0
	ldr r4, [r1]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r1, r0, #0x18
	movs r0, #0xc0
	ands r0, r4
	cmp r0, #0
	beq _0801630A
	b _08016428
_0801630A:
	ldr r0, [r5, #8]
	cmp r0, #2
	bne _08016312
	b _0801642E
_08016312:
	adds r0, r5, #0
	adds r0, #0x20
	ldrb r1, [r0]
	adds r3, r0, #0
	cmp r1, #0
	beq _08016334
	adds r0, #1
	ldrb r0, [r0]
	cmp r0, #6
	beq _08016334
	subs r0, r1, #1
	b _08016336
	.align 2, 0
_0801632C: .4byte gUnknown_03001304
_08016330: .4byte gUnknown_030007E0
_08016334:
	movs r0, #3
_08016336:
	strb r0, [r3]
	ldrb r0, [r3]
	cmp r0, #0
	beq _08016340
	b _080169FC
_08016340:
	adds r2, r5, #0
	adds r2, #0x21
	ldrb r0, [r2]
	adds r1, r0, #0
	cmp r1, #6
	bne _0801634E
	b _080169FC
_0801634E:
	cmp r1, #5
	bhi _08016356
	adds r0, #1
	strb r0, [r2]
_08016356:
	ldrb r0, [r2]
	cmp r0, #6
	bls _08016360
	subs r0, #1
	strb r0, [r2]
_08016360:
	ldrb r0, [r2]
	cmp r0, #6
	beq _0801636A
	movs r0, #3
	strb r0, [r3]
_0801636A:
	ldr r6, [r5, #0x10]
	adds r3, r6, #0
	adds r3, #0x2d
	ldrb r0, [r3]
	cmp r0, #0x20
	beq _08016380
	cmp r0, #0x1d
	beq _08016380
	cmp r0, #0x1f
	beq _08016380
	b _080168AC
_08016380:
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	subs r0, #7
	cmp r0, #0x22
	bls _0801638E
	b _08016870
_0801638E:
	lsls r0, r0, #2
	ldr r1, _08016398 @ =_0801639C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08016398: .4byte _0801639C
_0801639C: @ jump table
	.4byte _080169AC @ case 0
	.4byte _08016870 @ case 1
	.4byte _08016870 @ case 2
	.4byte _08016870 @ case 3
	.4byte _08016870 @ case 4
	.4byte _08016870 @ case 5
	.4byte _08016870 @ case 6
	.4byte _080169AC @ case 7
	.4byte _080169AC @ case 8
	.4byte _080169AC @ case 9
	.4byte _080169AC @ case 10
	.4byte _080169AC @ case 11
	.4byte _08016870 @ case 12
	.4byte _08016870 @ case 13
	.4byte _08016870 @ case 14
	.4byte _08016870 @ case 15
	.4byte _080169AC @ case 16
	.4byte _08016870 @ case 17
	.4byte _08016870 @ case 18
	.4byte _08016870 @ case 19
	.4byte _08016870 @ case 20
	.4byte _08016870 @ case 21
	.4byte _08016870 @ case 22
	.4byte _08016870 @ case 23
	.4byte _08016870 @ case 24
	.4byte _08016870 @ case 25
	.4byte _080169AC @ case 26
	.4byte _080169AC @ case 27
	.4byte _080169AC @ case 28
	.4byte _080169AC @ case 29
	.4byte _080169AC @ case 30
	.4byte _08016870 @ case 31
	.4byte _08016870 @ case 32
	.4byte _08016870 @ case 33
	.4byte _080169AC @ case 34
_08016428:
	ldr r0, [r5, #8]
	cmp r0, #2
	bne _08016434
_0801642E:
	cmp r1, #0
	bne _08016434
	b _080169FC
_08016434:
	adds r1, r5, #0
	adds r1, #0x20
	ldrb r0, [r1]
	cmp r0, #0
	beq _0801644A
	subs r0, #1
	strb r0, [r1]
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0801644A
	b _080169FC
_0801644A:
	movs r0, #3
	strb r0, [r1]
	movs r0, #0x40
	ands r0, r4
	cmp r0, #0
	bne _08016458
	b _080166C8
_08016458:
	movs r0, #0x30
	ands r4, r0
	cmp r4, #0
	bne _08016462
	b _080165F8
_08016462:
	adds r2, r5, #0
	adds r2, #0x21
	ldrb r0, [r2]
	adds r1, r0, #0
	cmp r1, #3
	bls _08016530
	subs r0, #1
	strb r0, [r2]
	ldr r6, [r5, #0x10]
	adds r3, r6, #0
	adds r3, #0x2d
	ldrb r0, [r3]
	cmp r0, #0x20
	beq _08016488
	cmp r0, #0x1d
	beq _08016488
	cmp r0, #0x1f
	beq _08016488
	b _080168AC
_08016488:
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	subs r0, #7
	cmp r0, #0x22
	bls _08016496
	b _08016870
_08016496:
	lsls r0, r0, #2
	ldr r1, _080164A0 @ =_080164A4
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080164A0: .4byte _080164A4
_080164A4: @ jump table
	.4byte _080169AC @ case 0
	.4byte _08016870 @ case 1
	.4byte _08016870 @ case 2
	.4byte _08016870 @ case 3
	.4byte _08016870 @ case 4
	.4byte _08016870 @ case 5
	.4byte _08016870 @ case 6
	.4byte _080169AC @ case 7
	.4byte _080169AC @ case 8
	.4byte _080169AC @ case 9
	.4byte _080169AC @ case 10
	.4byte _080169AC @ case 11
	.4byte _08016870 @ case 12
	.4byte _08016870 @ case 13
	.4byte _08016870 @ case 14
	.4byte _08016870 @ case 15
	.4byte _080169AC @ case 16
	.4byte _08016870 @ case 17
	.4byte _08016870 @ case 18
	.4byte _08016870 @ case 19
	.4byte _08016870 @ case 20
	.4byte _08016870 @ case 21
	.4byte _08016870 @ case 22
	.4byte _08016870 @ case 23
	.4byte _08016870 @ case 24
	.4byte _08016870 @ case 25
	.4byte _080169AC @ case 26
	.4byte _080169AC @ case 27
	.4byte _080169AC @ case 28
	.4byte _080169AC @ case 29
	.4byte _080169AC @ case 30
	.4byte _08016870 @ case 31
	.4byte _08016870 @ case 32
	.4byte _08016870 @ case 33
	.4byte _080169AC @ case 34
_08016530:
	cmp r1, #2
	bls _08016536
	b _080169FC
_08016536:
	adds r0, #1
	strb r0, [r2]
	ldr r6, [r5, #0x10]
	adds r3, r6, #0
	adds r3, #0x2d
	ldrb r0, [r3]
	cmp r0, #0x20
	beq _08016550
	cmp r0, #0x1d
	beq _08016550
	cmp r0, #0x1f
	beq _08016550
	b _080168AC
_08016550:
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	subs r0, #7
	cmp r0, #0x22
	bls _0801655E
	b _08016870
_0801655E:
	lsls r0, r0, #2
	ldr r1, _08016568 @ =_0801656C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08016568: .4byte _0801656C
_0801656C: @ jump table
	.4byte _080169AC @ case 0
	.4byte _08016870 @ case 1
	.4byte _08016870 @ case 2
	.4byte _08016870 @ case 3
	.4byte _08016870 @ case 4
	.4byte _08016870 @ case 5
	.4byte _08016870 @ case 6
	.4byte _080169AC @ case 7
	.4byte _080169AC @ case 8
	.4byte _080169AC @ case 9
	.4byte _080169AC @ case 10
	.4byte _080169AC @ case 11
	.4byte _08016870 @ case 12
	.4byte _08016870 @ case 13
	.4byte _08016870 @ case 14
	.4byte _08016870 @ case 15
	.4byte _080169AC @ case 16
	.4byte _08016870 @ case 17
	.4byte _08016870 @ case 18
	.4byte _08016870 @ case 19
	.4byte _08016870 @ case 20
	.4byte _08016870 @ case 21
	.4byte _08016870 @ case 22
	.4byte _08016870 @ case 23
	.4byte _08016870 @ case 24
	.4byte _08016870 @ case 25
	.4byte _080169AC @ case 26
	.4byte _080169AC @ case 27
	.4byte _080169AC @ case 28
	.4byte _080169AC @ case 29
	.4byte _080169AC @ case 30
	.4byte _08016870 @ case 31
	.4byte _08016870 @ case 32
	.4byte _08016870 @ case 33
	.4byte _080169AC @ case 34
_080165F8:
	adds r2, r5, #0
	adds r2, #0x21
	ldrb r0, [r2]
	cmp r0, #0
	bne _08016604
	b _080169FC
_08016604:
	subs r0, #1
	strb r0, [r2]
	ldr r6, [r5, #0x10]
	adds r3, r6, #0
	adds r3, #0x2d
	ldrb r0, [r3]
	cmp r0, #0x20
	beq _0801661E
	cmp r0, #0x1d
	beq _0801661E
	cmp r0, #0x1f
	beq _0801661E
	b _080168AC
_0801661E:
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	subs r0, #7
	cmp r0, #0x22
	bls _0801662C
	b _08016870
_0801662C:
	lsls r0, r0, #2
	ldr r1, _08016638 @ =_0801663C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08016638: .4byte _0801663C
_0801663C: @ jump table
	.4byte _080169AC @ case 0
	.4byte _08016870 @ case 1
	.4byte _08016870 @ case 2
	.4byte _08016870 @ case 3
	.4byte _08016870 @ case 4
	.4byte _08016870 @ case 5
	.4byte _08016870 @ case 6
	.4byte _080169AC @ case 7
	.4byte _080169AC @ case 8
	.4byte _080169AC @ case 9
	.4byte _080169AC @ case 10
	.4byte _080169AC @ case 11
	.4byte _08016870 @ case 12
	.4byte _08016870 @ case 13
	.4byte _08016870 @ case 14
	.4byte _08016870 @ case 15
	.4byte _080169AC @ case 16
	.4byte _08016870 @ case 17
	.4byte _08016870 @ case 18
	.4byte _08016870 @ case 19
	.4byte _08016870 @ case 20
	.4byte _08016870 @ case 21
	.4byte _08016870 @ case 22
	.4byte _08016870 @ case 23
	.4byte _08016870 @ case 24
	.4byte _08016870 @ case 25
	.4byte _080169AC @ case 26
	.4byte _080169AC @ case 27
	.4byte _080169AC @ case 28
	.4byte _080169AC @ case 29
	.4byte _080169AC @ case 30
	.4byte _08016870 @ case 31
	.4byte _08016870 @ case 32
	.4byte _08016870 @ case 33
	.4byte _080169AC @ case 34
_080166C8:
	movs r0, #0x80
	ands r0, r4
	cmp r0, #0
	bne _080166D2
	b _080169FC
_080166D2:
	movs r0, #0x30
	ands r4, r0
	cmp r4, #0
	bne _080166DC
	b _08016888
_080166DC:
	adds r2, r5, #0
	adds r2, #0x21
	ldrb r0, [r2]
	adds r1, r0, #0
	cmp r1, #8
	bhi _080167AC
	adds r0, #1
	strb r0, [r2]
	ldr r6, [r5, #0x10]
	adds r3, r6, #0
	adds r3, #0x2d
	ldrb r0, [r3]
	cmp r0, #0x20
	beq _08016702
	cmp r0, #0x1d
	beq _08016702
	cmp r0, #0x1f
	beq _08016702
	b _080168AC
_08016702:
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	subs r0, #7
	cmp r0, #0x22
	bls _08016710
	b _08016870
_08016710:
	lsls r0, r0, #2
	ldr r1, _0801671C @ =_08016720
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801671C: .4byte _08016720
_08016720: @ jump table
	.4byte _080169AC @ case 0
	.4byte _08016870 @ case 1
	.4byte _08016870 @ case 2
	.4byte _08016870 @ case 3
	.4byte _08016870 @ case 4
	.4byte _08016870 @ case 5
	.4byte _08016870 @ case 6
	.4byte _080169AC @ case 7
	.4byte _080169AC @ case 8
	.4byte _080169AC @ case 9
	.4byte _080169AC @ case 10
	.4byte _080169AC @ case 11
	.4byte _08016870 @ case 12
	.4byte _08016870 @ case 13
	.4byte _08016870 @ case 14
	.4byte _08016870 @ case 15
	.4byte _080169AC @ case 16
	.4byte _08016870 @ case 17
	.4byte _08016870 @ case 18
	.4byte _08016870 @ case 19
	.4byte _08016870 @ case 20
	.4byte _08016870 @ case 21
	.4byte _08016870 @ case 22
	.4byte _08016870 @ case 23
	.4byte _08016870 @ case 24
	.4byte _08016870 @ case 25
	.4byte _080169AC @ case 26
	.4byte _080169AC @ case 27
	.4byte _080169AC @ case 28
	.4byte _080169AC @ case 29
	.4byte _080169AC @ case 30
	.4byte _08016870 @ case 31
	.4byte _08016870 @ case 32
	.4byte _08016870 @ case 33
	.4byte _080169AC @ case 34
_080167AC:
	cmp r1, #9
	bhi _080167B2
	b _080169FC
_080167B2:
	subs r0, #1
	strb r0, [r2]
	ldr r6, [r5, #0x10]
	adds r3, r6, #0
	adds r3, #0x2d
	ldrb r0, [r3]
	cmp r0, #0x20
	beq _080167CA
	cmp r0, #0x1d
	beq _080167CA
	cmp r0, #0x1f
	bne _080168AC
_080167CA:
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	subs r0, #7
	cmp r0, #0x22
	bhi _08016870
	lsls r0, r0, #2
	ldr r1, _080167E0 @ =_080167E4
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080167E0: .4byte _080167E4
_080167E4: @ jump table
	.4byte _080169AC @ case 0
	.4byte _08016870 @ case 1
	.4byte _08016870 @ case 2
	.4byte _08016870 @ case 3
	.4byte _08016870 @ case 4
	.4byte _08016870 @ case 5
	.4byte _08016870 @ case 6
	.4byte _080169AC @ case 7
	.4byte _080169AC @ case 8
	.4byte _080169AC @ case 9
	.4byte _080169AC @ case 10
	.4byte _080169AC @ case 11
	.4byte _08016870 @ case 12
	.4byte _08016870 @ case 13
	.4byte _08016870 @ case 14
	.4byte _08016870 @ case 15
	.4byte _080169AC @ case 16
	.4byte _08016870 @ case 17
	.4byte _08016870 @ case 18
	.4byte _08016870 @ case 19
	.4byte _08016870 @ case 20
	.4byte _08016870 @ case 21
	.4byte _08016870 @ case 22
	.4byte _08016870 @ case 23
	.4byte _08016870 @ case 24
	.4byte _08016870 @ case 25
	.4byte _080169AC @ case 26
	.4byte _080169AC @ case 27
	.4byte _080169AC @ case 28
	.4byte _080169AC @ case 29
	.4byte _080169AC @ case 30
	.4byte _08016870 @ case 31
	.4byte _08016870 @ case 32
	.4byte _08016870 @ case 33
	.4byte _080169AC @ case 34
_08016870:
	movs r0, #0
	str r0, [sp]
	adds r0, r5, #0
	movs r1, #1
	movs r2, #1
	ldr r3, _08016884 @ =0x7FFFFFFF
	bl sub_8017264
	b _080169FC
	.align 2, 0
_08016884: .4byte 0x7FFFFFFF
_08016888:
	adds r2, r5, #0
	adds r2, #0x21
	ldrb r0, [r2]
	cmp r0, #0xb
	bls _08016894
	b _080169FC
_08016894:
	adds r0, #1
	strb r0, [r2]
	ldr r6, [r5, #0x10]
	adds r3, r6, #0
	adds r3, #0x2d
	ldrb r0, [r3]
	cmp r0, #0x20
	beq _08016904
	cmp r0, #0x1d
	beq _08016904
	cmp r0, #0x1f
	beq _08016904
_080168AC:
	ldr r4, [r6, #0x30]
	ldr r7, [r6, #0x34]
	ldr r1, _08016900 @ =gStaticData_0816C070
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r1, [r0]
	ldrb r2, [r2]
	lsls r0, r2, #2
	adds r0, r0, r1
	ldrb r0, [r0]
	strb r0, [r3]
	adds r0, r6, #0
	bl sub_80087C0
	adds r0, r6, #0
	bl sub_80087B4
	adds r0, r6, #0
	movs r1, #0
	bl sub_800872C
	ldr r3, [r5, #0x10]
	ldr r0, [r3, #0x20]
	adds r2, r3, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r6, [r2]
	lsls r0, r6, #3
	subs r0, r0, r6
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _080168F8
	subs r4, r0, #1
_080168F8:
	str r4, [r3, #0x30]
	str r7, [r3, #0x34]
	b _080169FC
	.align 2, 0
_08016900: .4byte gStaticData_0816C070
_08016904:
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	subs r0, #7
	cmp r0, #0x22
	bhi _080169EC
	lsls r0, r0, #2
	ldr r1, _0801691C @ =_08016920
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801691C: .4byte _08016920
_08016920: @ jump table
	.4byte _080169AC @ case 0
	.4byte _080169EC @ case 1
	.4byte _080169EC @ case 2
	.4byte _080169EC @ case 3
	.4byte _080169EC @ case 4
	.4byte _080169EC @ case 5
	.4byte _080169EC @ case 6
	.4byte _080169AC @ case 7
	.4byte _080169AC @ case 8
	.4byte _080169AC @ case 9
	.4byte _080169AC @ case 10
	.4byte _080169AC @ case 11
	.4byte _080169EC @ case 12
	.4byte _080169EC @ case 13
	.4byte _080169EC @ case 14
	.4byte _080169EC @ case 15
	.4byte _080169AC @ case 16
	.4byte _080169EC @ case 17
	.4byte _080169EC @ case 18
	.4byte _080169EC @ case 19
	.4byte _080169EC @ case 20
	.4byte _080169EC @ case 21
	.4byte _080169EC @ case 22
	.4byte _080169EC @ case 23
	.4byte _080169EC @ case 24
	.4byte _080169EC @ case 25
	.4byte _080169AC @ case 26
	.4byte _080169AC @ case 27
	.4byte _080169AC @ case 28
	.4byte _080169AC @ case 29
	.4byte _080169AC @ case 30
	.4byte _080169EC @ case 31
	.4byte _080169EC @ case 32
	.4byte _080169EC @ case 33
	.4byte _080169AC @ case 34
_080169AC:
	ldr r0, [r5, #0x10]
	ldr r4, [r0, #0x30]
	ldr r6, [r0, #0x34]
	movs r0, #0
	str r0, [sp]
	adds r0, r5, #0
	movs r1, #1
	movs r2, #1
	ldr r3, _080169E8 @ =0x7FFFFFFF
	bl sub_8017264
	ldr r3, [r5, #0x10]
	ldr r0, [r3, #0x20]
	adds r2, r3, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r7, [r2]
	lsls r0, r7, #3
	adds r2, r7, #0
	subs r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _080169E0
	subs r4, r0, #1
_080169E0:
	str r4, [r3, #0x30]
	str r6, [r3, #0x34]
	b _080169FC
	.align 2, 0
_080169E8: .4byte 0x7FFFFFFF
_080169EC:
	movs r0, #0
	str r0, [sp]
	adds r0, r5, #0
	movs r1, #1
	movs r2, #1
	ldr r3, _08016A2C @ =0x7FFFFFFF
	bl sub_8017264
_080169FC:
	ldr r1, _08016A30 @ =gStaticData_0816C250
	ldr r0, [r5, #8]
	lsls r3, r0, #3
	adds r0, r3, r1
	movs r4, #2
	ldrsh r2, [r0, r4]
	adds r7, r1, #0
	cmp r2, #0
	ble _08016A34
	movs r6, #4
	ldrsh r0, [r0, r6]
	adds r0, r5, r0
	ldr r1, [r0]
	lsls r0, r2, #3
	adds r0, r0, r1
	adds r3, r0, #0
	subs r3, #8
	ldr r0, [r3]
	ldr r1, [r3, #4]
	str r0, [sp, #4]
	str r1, [sp, #8]
	ldr r3, [sp, #8]
	b _08016A3A
	.align 2, 0
_08016A2C: .4byte 0x7FFFFFFF
_08016A30: .4byte gStaticData_0816C250
_08016A34:
	adds r0, r7, #4
	adds r0, r3, r0
	ldr r3, [r0]
_08016A3A:
	ldr r0, [r5, #8]
	lsls r0, r0, #3
	adds r0, r0, r7
	movs r4, #0
	ldrsh r1, [r0, r4]
	cmp r2, #0
	ble _08016A52
	ldr r6, [sp, #4]
	lsls r0, r6, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _08016A54
_08016A52:
	adds r0, r1, #0
_08016A54:
	adds r0, r5, r0
	bl sub_803AD84
	ldr r2, [r5, #0x10]
	ldr r0, [r2, #0x74]
	movs r1, #3
	ands r0, r1
	cmp r0, #0
	beq _08016A6A
	movs r0, #0
	str r0, [r2, #0x60]
_08016A6A:
	ldr r2, [r5, #0x10]
	ldr r0, [r2, #0x74]
	movs r1, #0xc
	ands r0, r1
	cmp r0, #0
	beq _08016A7A
	movs r0, #0
	str r0, [r2, #0x64]
_08016A7A:
	ldr r0, [r5, #0x10]
	movs r1, #0
	adds r0, #0x68
	strb r1, [r0]
	adds r0, r5, #0
	bl sub_8015DF8
_08016A88:
	ldr r0, [r5, #8]
	cmp r0, #3
	beq _08016A9C
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	cmp r0, #5
	beq _08016A9C
	cmp r0, #7
	bne _08016AA2
_08016A9C:
	ldr r1, [r5, #0x10]
	movs r0, #0x13
	b _08016AA6
_08016AA2:
	ldr r1, [r5, #0x10]
	movs r0, #1
_08016AA6:
	strb r0, [r1, #0xa]
	add sp, #0xc
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	thumb_func_start sub_8016AB0
sub_8016AB0: @ 0x08016AB0
	push {r4, lr}
	adds r4, r0, #0
	adds r1, r4, #0
	adds r1, #0x2c
	ldrb r0, [r1]
	cmp r0, #1
	bne _08016AE2
	movs r0, #0
	strb r0, [r1]
	ldr r0, [r4, #4]
	subs r1, #8
	ldr r2, [r0]
	ldrb r1, [r1]
	lsls r0, r1, #3
	adds r0, r0, r2
	ldr r1, [r0]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _08016B18 @ =gStaticData_0816B61C
	adds r2, r0, r1
	ldr r1, [r4, #0x10]
	adds r0, r4, #0
	bl sub_800B7B0
_08016AE2:
	adds r1, r4, #0
	adds r1, #0x2d
	ldrb r0, [r1]
	cmp r0, #1
	bne _08016B12
	movs r0, #0
	strb r0, [r1]
	ldr r0, [r4, #4]
	adds r2, r4, #0
	adds r2, #0x25
	ldr r1, [r0]
	ldrb r2, [r2]
	lsls r0, r2, #3
	adds r0, r0, r1
	ldr r1, [r0, #4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _08016B18 @ =gStaticData_0816B61C
	adds r2, r0, r1
	ldr r1, [r4, #0x10]
	adds r0, r4, #0
	bl sub_800B6D0
_08016B12:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08016B18: .4byte gStaticData_0816B61C

	thumb_func_start sub_8016B1C
sub_8016B1C: @ 0x08016B1C
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _08016B54 @ =gUnknown_03001304
	ldr r0, [r0]
	ldr r1, _08016B58 @ =gUnknown_030007E0
	ldr r1, [r1]
	str r1, [sp]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	adds r3, r4, #0
	adds r3, #0x27
	ldrb r0, [r3]
	adds r0, #1
	strb r0, [r3]
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #0x1e
	bne _08016B5C
	movs r0, #1
	adds r1, r4, #0
	adds r1, #0x2d
	strb r0, [r1]
	subs r1, #8
	strb r0, [r1]
	b _08016B74
	.align 2, 0
_08016B54: .4byte gUnknown_03001304
_08016B58: .4byte gUnknown_030007E0
_08016B5C:
	cmp r0, #0x3b
	bls _08016B74
	movs r0, #2
	adds r2, r4, #0
	adds r2, #0x2d
	movs r1, #1
	strb r1, [r2]
	adds r1, r4, #0
	adds r1, #0x25
	strb r0, [r1]
	movs r0, #0
	strb r0, [r3]
_08016B74:
	ldr r2, [r4, #0x10]
	adds r0, r2, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08016B94
	ldr r1, [r4, #0xc]
	adds r1, #0x50
	movs r3, #0
	ldrsh r0, [r1, r3]
	adds r0, r4, r0
	ldr r3, [r1, #4]
	adds r1, r2, #0
	movs r2, #0x1f
	bl sub_803AD84
_08016B94:
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08016BA8
	adds r0, r4, #0
	bl sub_80159F8
	b _08016BF2
_08016BA8:
	movs r0, #0x81
	lsls r0, r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08016BBA
	adds r0, r4, #0
	bl sub_8015C6C
	b _08016BF2
_08016BBA:
	cmp r5, #0
	beq _08016BF2
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #6
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r3, #0
	ldrsh r0, [r2, r3]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x1d
	bl sub_803AD84
	movs r2, #0xc
	adds r1, r4, #0
	adds r1, #0x2c
	movs r0, #1
	strb r0, [r1]
	adds r0, r4, #0
	adds r0, #0x24
	strb r2, [r0]
_08016BF2:
	adds r1, r4, #0
	adds r1, #0x26
	movs r0, #0
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_8016048
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_8016C08
sub_8016C08: @ 0x08016C08
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _08016C34 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	ldr r0, _08016C38 @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp]
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08016C3C
	adds r0, r4, #0
	bl sub_80159F8
	b _08016C8A
	.align 2, 0
_08016C34: .4byte gUnknown_03001304
_08016C38: .4byte gUnknown_030007E0
_08016C3C:
	movs r0, #0x81
	lsls r0, r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08016C4C
	adds r0, r4, #0
	bl sub_8015C6C
_08016C4C:
	cmp r5, #0
	bne _08016C84
	adds r0, r4, #0
	adds r0, #0x21
	ldrb r0, [r0]
	cmp r0, #6
	bne _08016C84
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #5
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x20
	bl sub_803AD84
	adds r0, r4, #0
	adds r0, #0x22
	strb r5, [r0]
_08016C84:
	adds r0, r4, #0
	bl sub_8016048
_08016C8A:
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8016C94
sub_8016C94: @ 0x08016C94
	push {r4, r5, lr}
	sub sp, #8
	adds r4, r0, #0
	ldr r0, _08016CB8 @ =gUnknown_03001304
	ldr r2, [r0]
	ldr r0, _08016CBC @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp, #4]
	add r5, sp, #4
	ldrh r1, [r5, #2]
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _08016CC0
	adds r0, r4, #0
	bl sub_8015C6C
	b _08016D50
	.align 2, 0
_08016CB8: .4byte gUnknown_03001304
_08016CBC: .4byte gUnknown_030007E0
_08016CC0:
	ldr r0, [r4, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	bne _08016CD4
	ldr r0, _08016CF0 @ =gUnknown_0300082C
	ldr r1, [r0]
	ldr r0, [r4, #0x28]
	cmp r1, r0
	bls _08016D4A
_08016CD4:
	adds r0, r2, #0
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r2, r0, #0x18
	ldrh r0, [r5, #2]
	movs r1, #1
	ands r1, r0
	cmp r1, #0
	beq _08016CF4
	adds r0, r4, #0
	bl sub_80159F8
	b _08016D4A
	.align 2, 0
_08016CF0: .4byte gUnknown_0300082C
_08016CF4:
	cmp r2, #0
	beq _08016D0C
	str r1, [sp]
	adds r0, r4, #0
	movs r1, #1
	movs r2, #1
	ldr r3, _08016D08 @ =0x7FFFFFFF
	bl sub_8017264
	b _08016D4A
	.align 2, 0
_08016D08: .4byte 0x7FFFFFFF
_08016D0C:
	adds r0, r4, #0
	adds r0, #0x21
	ldrb r0, [r0]
	cmp r0, #6
	bne _08016D3C
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #5
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x20
	bl sub_803AD84
	b _08016D4A
_08016D3C:
	str r2, [sp]
	adds r0, r4, #0
	movs r1, #1
	movs r2, #1
	ldr r3, _08016D58 @ =0x7FFFFFFF
	bl sub_8017264
_08016D4A:
	adds r0, r4, #0
	bl sub_8016048
_08016D50:
	add sp, #8
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08016D58: .4byte 0x7FFFFFFF

	thumb_func_start sub_8016D5C
sub_8016D5C: @ 0x08016D5C
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, _08016DB0 @ =gUnknown_03001304
	ldr r0, [r0]
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r3, r0, #0x18
	ldr r0, [r4, #0x18]
	adds r0, #1
	str r0, [r4, #0x18]
	ldr r1, [r4, #0x1c]
	cmp r0, r1
	bge _08016D84
	ldr r0, [r4, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08016DCA
_08016D84:
	ldr r0, _08016DB4 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x92
	movs r2, #0
	strb r2, [r0]
	adds r1, r4, #0
	adds r1, #0x23
	movs r0, #0xc
	strb r0, [r1]
	cmp r3, #0
	bne _08016DBC
	str r2, [sp]
	adds r0, r4, #0
	movs r1, #1
	movs r2, #1
	ldr r3, _08016DB8 @ =0x7FFFFFFF
	bl sub_8017264
	movs r0, #1
	str r0, [r4, #0x1c]
	b _08016DCA
	.align 2, 0
_08016DB0: .4byte gUnknown_03001304
_08016DB4: .4byte gUnknown_030012D8
_08016DB8: .4byte 0x7FFFFFFF
_08016DBC:
	str r2, [sp]
	adds r0, r4, #0
	movs r1, #1
	movs r2, #1
	ldr r3, _08016DD8 @ =0x7FFFFFFF
	bl sub_8017264
_08016DCA:
	adds r0, r4, #0
	bl sub_8016048
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08016DD8: .4byte 0x7FFFFFFF

	thumb_func_start sub_8016DDC
sub_8016DDC: @ 0x08016DDC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #8
	adds r5, r0, #0
	ldr r0, _08016E34 @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp, #4]
	ldr r1, [r5, #0x1c]
	cmp r1, #0
	beq _08016E86
	ldr r0, [r5, #0x18]
	adds r0, #1
	str r0, [r5, #0x18]
	cmp r0, r1
	blt _08016E86
	adds r2, r5, #0
	adds r2, #0x23
	movs r1, #0
	movs r0, #0xc
	strb r0, [r2]
	str r1, [r5, #0x1c]
	adds r1, r5, #0
	adds r1, #0x22
	ldrb r0, [r1]
	cmp r0, #5
	beq _08016E3C
	cmp r0, #7
	bne _08016E86
	ldr r0, [r5, #0x10]
	ldr r6, [r0, #0x30]
	movs r0, #6
	strb r0, [r1]
	ldr r4, [r5, #0xc]
	adds r4, #0x50
	movs r1, #0
	ldrsh r0, [r4, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r2, _08016E38 @ =gStaticData_0816C070
	movs r3, #0x21
	ldr r2, [r2, #0x18]
	b _08016E56
	.align 2, 0
_08016E34: .4byte gUnknown_030007E0
_08016E38: .4byte gStaticData_0816C070
_08016E3C:
	ldr r0, [r5, #0x10]
	ldr r6, [r0, #0x30]
	movs r0, #4
	strb r0, [r1]
	ldr r4, [r5, #0xc]
	adds r4, #0x50
	movs r7, #0
	ldrsh r0, [r4, r7]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r2, _08016EC4 @ =gStaticData_0816C070
	movs r3, #0x21
	ldr r2, [r2, #0x10]
_08016E56:
	mov r8, r2
	ldrb r7, [r3, r5]
	lsls r2, r7, #2
	add r2, r8
	ldrb r2, [r2]
	ldr r3, [r4, #4]
	bl sub_803AD84
	ldr r4, [r5, #0x10]
	adds r3, r6, #0
	ldr r0, [r4, #0x20]
	adds r2, r4, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r6, [r2]
	lsls r0, r6, #3
	subs r0, r0, r6
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r3, r0
	blt _08016E84
	subs r3, r0, #1
_08016E84:
	str r3, [r4, #0x30]
_08016E86:
	add r0, sp, #4
	ldrh r1, [r0, #2]
	movs r4, #0x81
	lsls r4, r4, #1
	ands r4, r1
	cmp r4, #0
	beq _08016F20
	adds r0, r5, #0
	adds r0, #0x23
	ldrb r0, [r0]
	cmp r0, #0
	beq _08016EA0
	b _08017034
_08016EA0:
	ldr r0, [r5, #0x1c]
	cmp r0, #0
	beq _08016EA8
	b _08017034
_08016EA8:
	str r0, [r5, #0x18]
	movs r0, #0x18
	str r0, [r5, #0x1c]
	ldr r0, [r5, #0x10]
	ldr r6, [r0, #0x30]
	adds r0, r5, #0
	adds r0, #0x22
	adds r2, r0, #0
	ldrb r7, [r2]
	cmp r7, #4
	bne _08016EC8
	movs r0, #5
	b _08016ECA
	.align 2, 0
_08016EC4: .4byte gStaticData_0816C070
_08016EC8:
	movs r0, #7
_08016ECA:
	strb r0, [r2]
	ldr r4, [r5, #0xc]
	adds r4, #0x50
	movs r1, #0
	ldrsh r0, [r4, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, _08016F1C @ =gStaticData_0816C070
	ldrb r2, [r2]
	lsls r2, r2, #2
	adds r2, r2, r3
	movs r3, #0x21
	ldr r2, [r2]
	mov r8, r2
	ldrb r7, [r3, r5]
	lsls r2, r7, #2
	add r2, r8
	ldrb r2, [r2]
	ldr r3, [r4, #4]
	bl sub_803AD84
	adds r0, r5, #0
	bl sub_8015C6C
	ldr r4, [r5, #0x10]
	adds r3, r6, #0
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
	blt _08016F18
	subs r3, r0, #1
_08016F18:
	str r3, [r4, #0x30]
	b _08017034
	.align 2, 0
_08016F1C: .4byte gStaticData_0816C070
_08016F20:
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08016F2E
	adds r0, r5, #0
	bl sub_80159F8
_08016F2E:
	ldr r2, [r5, #0x10]
	adds r0, r2, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08017034
	adds r6, r5, #0
	adds r6, #0x22
	ldrb r0, [r6]
	cmp r0, #5
	beq _08016F94
	cmp r0, #5
	bgt _08016F4E
	cmp r0, #4
	beq _08016F58
	b _0801702C
_08016F4E:
	cmp r0, #6
	beq _08016F6C
	cmp r0, #7
	beq _08016FE8
	b _0801702C
_08016F58:
	adds r2, #0x28
	ldrb r1, [r2]
	lsls r0, r1, #0x1b
	cmp r0, #0
	bge _08016F7E
	movs r0, #0x11
	rsbs r0, r0, #0
	ands r0, r1
	strb r0, [r2]
	b _08016F7E
_08016F6C:
	adds r0, r2, #0
	adds r0, #0x28
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r6, [r0]
	ands r1, r6
	movs r2, #0x10
	orrs r1, r2
	strb r1, [r0]
_08016F7E:
	str r4, [sp]
	adds r0, r5, #0
	movs r1, #1
	movs r2, #1
	ldr r3, _08016F90 @ =0x7FFFFFFF
	bl sub_8017264
	b _0801702C
	.align 2, 0
_08016F90: .4byte 0x7FFFFFFF
_08016F94:
	adds r2, #0x28
	ldrb r1, [r2]
	lsls r0, r1, #0x1b
	cmp r0, #0
	bge _08016FA6
	movs r0, #0x11
	rsbs r0, r0, #0
	ands r0, r1
	strb r0, [r2]
_08016FA6:
	movs r4, #3
	ldr r1, [r5, #0xc]
	movs r7, #0x20
	ldrsh r0, [r1, r7]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #3
	bl sub_803AD80
	strb r4, [r6]
	ldr r4, [r5, #0xc]
	adds r4, #0x50
	movs r1, #0
	ldrsh r0, [r4, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r2, _08016FE4 @ =gStaticData_0816C070
	movs r3, #0x21
	adds r3, r3, r5
	mov ip, r3
	ldr r3, [r2, #0xc]
	mov r6, ip
	ldrb r6, [r6]
	lsls r2, r6, #2
	adds r2, r2, r3
	ldrb r2, [r2]
	ldr r3, [r4, #4]
	bl sub_803AD84
	b _0801702C
	.align 2, 0
_08016FE4: .4byte gStaticData_0816C070
_08016FE8:
	adds r2, #0x28
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r7, [r2]
	ands r0, r7
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r2]
	movs r4, #3
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #3
	bl sub_803AD80
	strb r4, [r6]
	ldr r4, [r5, #0xc]
	adds r4, #0x50
	movs r3, #0
	ldrsh r0, [r4, r3]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r2, _08017040 @ =gStaticData_0816C070
	movs r6, #0x21
	ldr r3, [r2, #0xc]
	ldrb r7, [r6, r5]
	lsls r2, r7, #2
	adds r2, r2, r3
	ldrb r2, [r2]
	ldr r3, [r4, #4]
	bl sub_803AD84
_0801702C:
	adds r1, r5, #0
	adds r1, #0x2c
	movs r0, #1
	strb r0, [r1]
_08017034:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08017040: .4byte gStaticData_0816C070

	thumb_func_start sub_8017044
sub_8017044: @ 0x08017044
	push {r4, r5, lr}
	sub sp, #8
	adds r4, r0, #0
	ldr r0, _08017068 @ =gUnknown_03001304
	ldr r2, [r0]
	ldr r0, _0801706C @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp, #4]
	add r5, sp, #4
	ldrh r1, [r5, #2]
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _08017070
	adds r0, r4, #0
	bl sub_8015C6C
	b _080170E4
	.align 2, 0
_08017068: .4byte gUnknown_03001304
_0801706C: .4byte gUnknown_030007E0
_08017070:
	ldr r0, [r4, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _080170DE
	adds r0, r2, #0
	bl sub_8000760
	lsls r0, r0, #0x18
	lsrs r2, r0, #0x18
	ldrh r0, [r5, #2]
	movs r1, #1
	ands r1, r0
	cmp r1, #0
	beq _08017096
	adds r0, r4, #0
	bl sub_80159F8
	b _080170DE
_08017096:
	cmp r2, #0
	beq _080170B0
	str r1, [sp]
	adds r0, r4, #0
	movs r1, #1
	movs r2, #1
	ldr r3, _080170AC @ =0x7FFFFFFF
	bl sub_8017264
	b _080170DE
	.align 2, 0
_080170AC: .4byte 0x7FFFFFFF
_080170B0:
	adds r0, r4, #0
	adds r0, #0x21
	ldrb r0, [r0]
	cmp r0, #6
	bne _080170DE
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #5
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x20
	bl sub_803AD84
_080170DE:
	adds r0, r4, #0
	bl sub_8016048
_080170E4:
	add sp, #8
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_80170EC
sub_80170EC: @ 0x080170EC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r5, r0, #0
	ldr r0, _08017110 @ =gUnknown_030007E0
	ldr r0, [r0]
	str r0, [sp]
	mov r0, sp
	ldrh r1, [r0, #2]
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08017114
	adds r0, r5, #0
	bl sub_80159F8
	b _08017174
	.align 2, 0
_08017110: .4byte gUnknown_030007E0
_08017114:
	movs r6, #0x81
	lsls r6, r6, #1
	ands r6, r1
	cmp r6, #0
	beq _08017126
	adds r0, r5, #0
	bl sub_8015C6C
	b _08017174
_08017126:
	ldr r0, [r5, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801716E
	adds r4, r5, #0
	adds r4, #0x22
	strb r6, [r4]
	ldr r1, [r5, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x24]
	movs r1, #0
	bl sub_803AD80
	strb r6, [r4]
	ldr r4, [r5, #0xc]
	adds r4, #0x50
	movs r3, #0
	ldrsh r0, [r4, r3]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r2, _08017180 @ =gStaticData_0816C070
	movs r7, #0x21
	ldr r2, [r2]
	mov r8, r2
	ldrb r3, [r7, r5]
	lsls r2, r3, #2
	add r2, r8
	ldrb r2, [r2]
	ldr r3, [r4, #4]
	bl sub_803AD84
	str r6, [r5, #0x18]
	str r6, [r5, #0x1c]
_0801716E:
	adds r0, r5, #0
	bl sub_8016048
_08017174:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08017180: .4byte gStaticData_0816C070

	thumb_func_start sub_8017184
sub_8017184: @ 0x08017184
	push {r4, r5, lr}
	adds r5, r0, #0
	movs r0, #0
	movs r1, #5
	movs r2, #0
	bl sub_8015FDC
	movs r4, #0
	ldr r0, _080171BC @ =gUnknown_030012D8
	ldr r2, [r0]
	ldr r1, [r2, #0x60]
	adds r0, r1, #0
	muls r0, r1, r0
	cmp r0, #0
	bge _080171A6
	ldr r3, _080171C0 @ =0x00003FFF
	adds r0, r0, r3
_080171A6:
	asrs r0, r0, #0xe
	adds r3, r0, #4
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r0, r1, r0
	cmp r0, #0
	ble _080171C4
	str r4, [r2, #0x48]
	str r3, [r2, #0x4c]
	b _080171CA
	.align 2, 0
_080171BC: .4byte gUnknown_030012D8
_080171C0: .4byte 0x00003FFF
_080171C4:
	str r4, [r2, #0x48]
	movs r0, #5
	str r0, [r2, #0x4c]
_080171CA:
	str r4, [r2, #0x50]
	ldr r1, [r5, #0x10]
	adds r0, r1, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801720A
	movs r0, #1
	ldrb r4, [r1, #0xc]
	orrs r0, r4
	strb r0, [r1, #0xc]
	ldr r0, _08017210 @ =0x0000FFFF
	ldrh r2, [r1, #8]
	cmp r2, r0
	beq _0801720A
	ldrh r3, [r1, #8]
	ldr r0, _08017214 @ =gUnknown_030012B4
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
_0801720A:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08017210: .4byte 0x0000FFFF
_08017214: .4byte gUnknown_030012B4

	thumb_func_start sub_8017218
sub_8017218: @ 0x08017218
	str r1, [r0, #0x10]
	bx lr

	thumb_func_start sub_801721C
sub_801721C: @ 0x0801721C
	push {lr}
	ldr r3, [r0, #4]
	ldr r3, [r3]
	lsls r2, r2, #3
	adds r2, r2, r3
	ldr r3, [r2, #4]
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r2, r2, #2
	ldr r3, _0801723C @ =gStaticData_0816B61C
	adds r2, r2, r3
	bl sub_800B6D0
	pop {r0}
	bx r0
	.align 2, 0
_0801723C: .4byte gStaticData_0816B61C

	thumb_func_start sub_8017240
sub_8017240: @ 0x08017240
	push {lr}
	ldr r3, [r0, #4]
	ldr r3, [r3]
	lsls r2, r2, #3
	adds r2, r2, r3
	ldr r3, [r2]
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r2, r2, #2
	ldr r3, _08017260 @ =gStaticData_0816B61C
	adds r2, r2, r3
	bl sub_800B7B0
	pop {r0}
	bx r0
	.align 2, 0
_08017260: .4byte gStaticData_0816B61C

	thumb_func_start sub_8017264
sub_8017264: @ 0x08017264
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	adds r4, r2, #0
	adds r6, r3, #0
	ldr r2, [r5, #0xc]
	movs r3, #0x20
	ldrsh r0, [r2, r3]
	adds r0, r5, r0
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	adds r2, r5, #0
	adds r2, #0x22
	strb r4, [r2]
	ldr r4, [r5, #0xc]
	adds r4, #0x50
	movs r7, #0
	ldrsh r0, [r4, r7]
	adds r0, r5, r0
	ldr r1, [r5, #0x10]
	ldr r3, _080172C8 @ =gStaticData_0816C070
	ldrb r2, [r2]
	lsls r2, r2, #2
	adds r2, r2, r3
	movs r3, #0x21
	ldr r2, [r2]
	mov r8, r2
	ldrb r7, [r3, r5]
	lsls r2, r7, #2
	add r2, r8
	ldrb r2, [r2]
	ldr r3, [r4, #4]
	bl sub_803AD84
	ldr r0, _080172CC @ =0x7FFFFFFF
	cmp r6, r0
	beq _080172B4
	str r6, [r5, #0x18]
_080172B4:
	ldr r1, [sp, #0x18]
	cmp r1, r0
	beq _080172BC
	str r1, [r5, #0x1c]
_080172BC:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080172C8: .4byte gStaticData_0816C070
_080172CC: .4byte 0x7FFFFFFF

	thumb_func_start sub_80172D0
sub_80172D0: @ 0x080172D0
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	mov ip, r1
	adds r5, r2, #0
	ldr r0, _08017308 @ =gUnknown_030012D8
	ldr r3, [r0]
	ldr r4, [r3, #0x60]
	adds r0, r4, #0
	muls r0, r4, r0
	cmp r0, #0
	bge _080172EA
	ldr r1, _0801730C @ =0x00003FFF
	adds r0, r0, r1
_080172EA:
	asrs r0, r0, #0xe
	adds r7, r0, #4
	asrs r0, r4, #0x1f
	adds r1, r4, #0
	eors r1, r0
	subs r1, r1, r0
	asrs r2, r5, #0x1f
	adds r0, r5, #0
	eors r0, r2
	subs r0, r0, r2
	cmp r1, r0
	ble _08017310
	str r6, [r3, #0x48]
	str r7, [r3, #0x4c]
	b _08017326
	.align 2, 0
_08017308: .4byte gUnknown_030012D8
_0801730C: .4byte 0x00003FFF
_08017310:
	adds r0, r4, #0
	muls r0, r5, r0
	cmp r0, #0
	bge _08017320
	mov r1, ip
	adds r0, r7, r1
	str r6, [r3, #0x48]
	b _08017324
_08017320:
	str r6, [r3, #0x48]
	mov r0, ip
_08017324:
	str r0, [r3, #0x4c]
_08017326:
	str r5, [r3, #0x50]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
	thumb_func_start sub_8017330
sub_8017330: @ 0x08017330
	adds	r1, r0, #0
	muls	r1, r0
	adds	r0, r1, #0
	cmp	r0, #0
	bge _0801733E
	ldr r1, _08017344
	adds	r0, r0, r1
_0801733E:
	asrs	r0, r0, #14
	adds	r0, #4
	bx	lr
_08017344: .4byte 0x3fff

	thumb_func_start sub_8017348
sub_8017348: @ 0x08017348
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r5, r0, #0
	ldr r6, [r5, #0x10]
	adds r3, r6, #0
	adds r3, #0x2d
	ldrb r0, [r3]
	cmp r0, #0x20
	beq _080173BC
	cmp r0, #0x1d
	beq _080173BC
	cmp r0, #0x1f
	beq _080173BC
	ldr r4, [r6, #0x30]
	ldr r7, [r6, #0x34]
	ldr r1, _080173B8 @ =gStaticData_0816C070
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	lsls r0, r0, #2
	adds r0, r0, r1
	adds r2, r5, #0
	adds r2, #0x21
	ldr r1, [r0]
	ldrb r2, [r2]
	lsls r0, r2, #2
	adds r0, r0, r1
	ldrb r0, [r0]
	strb r0, [r3]
	adds r0, r6, #0
	bl sub_80087C0
	adds r0, r6, #0
	bl sub_80087B4
	adds r0, r6, #0
	movs r1, #0
	bl sub_800872C
	ldr r3, [r5, #0x10]
	ldr r0, [r3, #0x20]
	adds r2, r3, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r5, [r2]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _080173B2
	subs r4, r0, #1
_080173B2:
	str r4, [r3, #0x30]
	str r7, [r3, #0x34]
	b _080174B0
	.align 2, 0
_080173B8: .4byte gStaticData_0816C070
_080173BC:
	adds r0, r5, #0
	adds r0, #0x22
	ldrb r0, [r0]
	subs r0, #7
	cmp r0, #0x22
	bhi _080174A0
	lsls r0, r0, #2
	ldr r1, _080173D4 @ =_080173D8
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080173D4: .4byte _080173D8
_080173D8: @ jump table
	.4byte _08017464 @ case 0
	.4byte _080174A0 @ case 1
	.4byte _080174A0 @ case 2
	.4byte _080174A0 @ case 3
	.4byte _080174A0 @ case 4
	.4byte _080174A0 @ case 5
	.4byte _080174A0 @ case 6
	.4byte _08017464 @ case 7
	.4byte _08017464 @ case 8
	.4byte _08017464 @ case 9
	.4byte _08017464 @ case 10
	.4byte _08017464 @ case 11
	.4byte _080174A0 @ case 12
	.4byte _080174A0 @ case 13
	.4byte _080174A0 @ case 14
	.4byte _080174A0 @ case 15
	.4byte _08017464 @ case 16
	.4byte _080174A0 @ case 17
	.4byte _080174A0 @ case 18
	.4byte _080174A0 @ case 19
	.4byte _080174A0 @ case 20
	.4byte _080174A0 @ case 21
	.4byte _080174A0 @ case 22
	.4byte _080174A0 @ case 23
	.4byte _080174A0 @ case 24
	.4byte _080174A0 @ case 25
	.4byte _08017464 @ case 26
	.4byte _08017464 @ case 27
	.4byte _08017464 @ case 28
	.4byte _08017464 @ case 29
	.4byte _08017464 @ case 30
	.4byte _080174A0 @ case 31
	.4byte _080174A0 @ case 32
	.4byte _080174A0 @ case 33
	.4byte _08017464 @ case 34
_08017464:
	ldr r0, [r5, #0x10]
	ldr r4, [r0, #0x30]
	ldr r6, [r0, #0x34]
	movs r0, #0
	str r0, [sp]
	adds r0, r5, #0
	movs r1, #1
	movs r2, #1
	ldr r3, _0801749C @ =0x7FFFFFFF
	bl sub_8017264
	ldr r3, [r5, #0x10]
	ldr r0, [r3, #0x20]
	adds r2, r3, #0
	adds r2, #0x2d
	ldr r1, [r0]
	ldrb r5, [r2]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #0x16]
	cmp r4, r0
	blt _08017496
	subs r4, r0, #1
_08017496:
	str r4, [r3, #0x30]
	str r6, [r3, #0x34]
	b _080174B0
	.align 2, 0
_0801749C: .4byte 0x7FFFFFFF
_080174A0:
	movs r0, #0
	str r0, [sp]
	adds r0, r5, #0
	movs r1, #1
	movs r2, #1
	ldr r3, _080174B8 @ =0x7FFFFFFF
	bl sub_8017264
_080174B0:
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080174B8: .4byte 0x7FFFFFFF

	thumb_func_start sub_80174BC
sub_80174BC: @ 0x080174BC
	push {lr}
	sub sp, #4
	ldr r3, _080174D4 @ =0x7FFFFFFF
	movs r1, #0
	str r1, [sp]
	movs r1, #1
	movs r2, #1
	bl sub_8017264
	add sp, #4
	pop {r0}
	bx r0
	.align 2, 0
_080174D4: .4byte 0x7FFFFFFF

	thumb_func_start sub_80174D8
sub_80174D8: @ 0x080174D8
	push {lr}
	ldr r2, _080174E8 @ =gStaticData_087E428C
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_080174E8: .4byte gStaticData_087E428C

	thumb_func_start sub_80174EC
sub_80174EC: @ 0x080174EC
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800B8C8
	ldr r0, _08017508 @ =gStaticData_087E428C
	str r0, [r4, #0xc]
	adds r0, r4, #0
	bl sub_8015958
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08017508: .4byte gStaticData_087E428C

	thumb_func_start sub_801750C
sub_801750C: @ 0x0801750C
	movs r1, #0
	str r1, [r0, #0x14]
	bx lr
	.align 2, 0

	thumb_func_start sub_8017514
sub_8017514: @ 0x08017514
	adds r0, #0x2d
	movs r1, #1
	strb r1, [r0]
	bx lr

	thumb_func_start sub_801751C
sub_801751C: @ 0x0801751C
	adds r0, #0x2c
	movs r1, #1
	strb r1, [r0]
	bx lr

	thumb_func_start sub_8017524
sub_8017524: @ 0x08017524
	adds r0, #0x2d
	movs r1, #0
	strb r1, [r0]
	bx lr

	thumb_func_start sub_801752C
sub_801752C: @ 0x0801752C
	adds r0, #0x2c
	movs r1, #0
	strb r1, [r0]
	bx lr

	thumb_func_start sub_8017534
sub_8017534: @ 0x08017534
	adds r0, #0x2d
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_801753C
sub_801753C: @ 0x0801753C
	adds r0, #0x2c
	ldrb r0, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8017544
sub_8017544: @ 0x08017544
	adds r3, r0, #0
	adds r3, #0x2d
	movs r2, #1
	strb r2, [r3]
	adds r0, #0x25
	strb r1, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8017554
sub_8017554: @ 0x08017554
	adds r3, r0, #0
	adds r3, #0x2c
	movs r2, #1
	strb r2, [r3]
	adds r0, #0x24
	strb r1, [r0]
	bx lr
	.align 2, 0

	thumb_func_start sub_8017564
sub_8017564: @ 0x08017564
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, _080175F4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x1b
	bl PlaySfx
	ldr r1, [r4, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x24]
	movs r1, #3
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	adds r2, r5, #0
	bl sub_803AD84
	ldr r1, [r4, #0x10]
	movs r0, #0x7f
	ldrb r2, [r1, #0xc]
	ands r0, r2
	strb r0, [r1, #0xc]
	ldr r1, [r4, #0x10]
	movs r0, #0x41
	rsbs r0, r0, #0
	ldrb r5, [r1, #0xc]
	ands r0, r5
	strb r0, [r1, #0xc]
	ldr r0, [r4, #0x10]
	movs r1, #0x82
	lsls r1, r1, #1
	adds r0, r0, r1
	movs r1, #1
	strb r1, [r0]
	ldr r0, _080175F8 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_8023234
	ldr r0, _080175FC @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r3, [r4, #0x10]
	adds r1, r3, #0
	adds r1, #0x29
	ldrb r1, [r1]
	lsls r1, r1, #0x1c
	lsrs r1, r1, #0x1c
	ldr r2, [r3, #0x20]
	adds r3, #0x2d
	ldr r4, [r2]
	ldrb r5, [r3]
	lsls r2, r5, #3
	subs r2, r2, r5
	lsls r2, r2, #2
	adds r2, r2, r4
	ldrb r2, [r2, #0x14]
	bl sub_8006D08
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080175F4: .4byte gUnknown_030012BC
_080175F8: .4byte gUnknown_030012C0
_080175FC: .4byte gUnknown_030012B8

	thumb_func_start sub_8017600
sub_8017600: @ 0x08017600
	push {r4, r5, lr}
	sub sp, #4
	adds r5, r0, #0
	movs r4, #0
	str r4, [sp]
	movs r1, #1
	movs r2, #0
	movs r3, #0
	bl sub_80178BC
	movs r0, #1
	strb r0, [r5, #0x17]
	strb r0, [r5, #0x14]
	strb r0, [r5, #0x18]
	strb r4, [r5, #0x15]
	strb r4, [r5, #0x16]
	ldr r0, [r5, #0x1c]
	cmp r0, #0
	bne _0801763C
	movs r0, #0x80
	bl sub_8026EDC
	bl sub_801B940
	adds r1, r0, #0
	str r1, [r5, #0x1c]
	ldr r0, _0801764C @ =gUnknown_030012F0
	ldr r0, [r0]
	bl sub_8008E94
_0801763C:
	ldr r0, [r5, #0x1c]
	bl sub_801B864
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0801764C: .4byte gUnknown_030012F0

	thumb_func_start sub_8017650
sub_8017650: @ 0x08017650
	push {r4, r5, r6, r7, lr}
	sub sp, #8
	adds r4, r0, #0
	ldr r0, [r4, #8]
	cmp r0, #3
	bne _0801765E
	b _080177A2
_0801765E:
	ldr r0, [r4, #0x10]
	ldr r1, [r0]
	ldr r0, _080176D0 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	ldr r0, [r0, #0x10]
	lsls r0, r0, #8
	ldr r2, _080176D4 @ =0xFFFFF600
	adds r0, r0, r2
	cmp r1, r0
	ble _080176B0
	ldr r1, [r4, #0x1c]
	movs r0, #1
	ldrb r3, [r1, #0xc]
	orrs r0, r3
	strb r0, [r1, #0xc]
	ldr r0, _080176D8 @ =0x0000FFFF
	ldrh r5, [r1, #8]
	cmp r5, r0
	beq _080176A8
	ldrh r3, [r1, #8]
	ldr r0, _080176DC @ =gUnknown_030012B4
	ldr r2, [r0]
	adds r0, r3, #0
	asrs r0, r0, #5
	lsls r1, r0, #2
	movs r6, #0x84
	lsls r6, r6, #1
	adds r2, r2, r6
	adds r2, r2, r1
	lsls r0, r0, #5
	subs r0, r3, r0
	movs r1, #1
	lsls r1, r0
	ldr r0, [r2]
	orrs r0, r1
	str r0, [r2]
_080176A8:
	movs r0, #0
	str r0, [r4, #0x1c]
	bl sub_80241A4
_080176B0:
	ldr r0, _080176E0 @ =gUnknown_030007E0
	ldr r2, [r0]
	movs r0, #0x40
	ands r0, r2
	cmp r0, #0
	beq _080176E4
	ldrb r0, [r4, #0x16]
	cmp r0, #1
	beq _080176E4
	movs r1, #3
	movs r0, #1
	strb r0, [r4, #0x18]
	strb r1, [r4, #0x15]
	strb r0, [r4, #0x16]
	b _08017710
	.align 2, 0
_080176D0: .4byte gUnknown_03001308
_080176D4: .4byte 0xFFFFF600
_080176D8: .4byte 0x0000FFFF
_080176DC: .4byte gUnknown_030012B4
_080176E0: .4byte gUnknown_030007E0
_080176E4:
	movs r0, #0x80
	ands r0, r2
	cmp r0, #0
	beq _08017700
	ldrb r1, [r4, #0x16]
	cmp r1, #2
	beq _08017700
	movs r0, #5
	movs r1, #1
	strb r1, [r4, #0x18]
	strb r0, [r4, #0x15]
	movs r0, #2
	strb r0, [r4, #0x16]
	b _08017710
_08017700:
	movs r1, #0xc0
	ands r1, r2
	cmp r1, #0
	bne _08017710
	movs r0, #1
	strb r0, [r4, #0x18]
	strb r1, [r4, #0x15]
	strb r1, [r4, #0x16]
_08017710:
	movs r0, #0x20
	ands r0, r2
	adds r3, r4, #0
	adds r3, #0x20
	cmp r0, #0
	beq _08017746
	ldrb r0, [r3]
	cmp r0, #0
	beq _08017746
	movs r1, #7
	movs r0, #1
	strb r0, [r4, #0x17]
	strb r1, [r4, #0x14]
	movs r1, #0xc8
	lsls r1, r1, #6
	ldr r0, [r4, #0x1c]
	str r1, [r0, #0x78]
	ldr r0, [r4, #0x24]
	adds r0, #1
	str r0, [r4, #0x24]
	cmp r0, #0x1e
	ble _08017784
	movs r0, #0
	strb r0, [r3]
	movs r0, #0xa
	str r0, [r4, #0x24]
	b _08017784
_08017746:
	movs r0, #0x10
	ands r0, r2
	cmp r0, #0
	beq _08017760
	movs r1, #8
	movs r0, #1
	strb r0, [r4, #0x17]
	strb r1, [r4, #0x14]
	movs r1, #0xa0
	lsls r1, r1, #4
	ldr r0, [r4, #0x1c]
	str r1, [r0, #0x78]
	b _08017784
_08017760:
	movs r0, #0x30
	ands r0, r2
	cmp r0, #0
	beq _08017776
	movs r0, #0x20
	ands r0, r2
	cmp r0, #0
	beq _08017784
	ldrb r0, [r3]
	cmp r0, #0
	bne _080177A2
_08017776:
	movs r0, #0xf0
	lsls r0, r0, #5
	ldr r1, [r4, #0x1c]
	str r0, [r1, #0x78]
	movs r0, #1
	strb r0, [r4, #0x17]
	strb r0, [r4, #0x14]
_08017784:
	ldrb r1, [r3]
	cmp r1, #0
	bne _080177A2
	ldr r0, [r4, #0x24]
	subs r0, #1
	str r0, [r4, #0x24]
	cmp r0, #0
	bge _080177A2
	str r1, [r4, #0x24]
	movs r0, #0x20
	ands r2, r0
	cmp r2, #0
	bne _080177A2
	movs r0, #1
	strb r0, [r3]
_080177A2:
	ldr r1, _080177D0 @ =gStaticData_0816C290
	ldr r0, [r4, #8]
	lsls r5, r0, #3
	adds r2, r5, r1
	movs r6, #2
	ldrsh r3, [r2, r6]
	adds r6, r0, #0
	adds r7, r1, #0
	cmp r3, #0
	ble _080177D4
	movs r1, #4
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r0]
	lsls r0, r3, #3
	adds r0, r0, r1
	subs r0, #8
	ldr r1, [r0]
	ldr r2, [r0, #4]
	str r1, [sp]
	str r2, [sp, #4]
	ldr r2, [sp, #4]
	b _080177DA
	.align 2, 0
_080177D0: .4byte gStaticData_0816C290
_080177D4:
	adds r0, r7, #4
	adds r0, r5, r0
	ldr r2, [r0]
_080177DA:
	lsls r0, r6, #3
	adds r0, r0, r7
	movs r5, #0
	ldrsh r1, [r0, r5]
	cmp r3, #0
	ble _080177F0
	ldr r6, [sp]
	lsls r0, r6, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _080177F2
_080177F0:
	adds r0, r1, #0
_080177F2:
	adds r0, r4, r0
	bl sub_803AD80
	adds r0, r4, #0
	bl sub_8017808
	add sp, #8
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8017808
sub_8017808: @ 0x08017808
	push {r4, r5, lr}
	adds r4, r0, #0
	ldrb r0, [r4, #0x17]
	cmp r0, #1
	bne _08017860
	ldr r0, [r4, #4]
	ldr r1, [r0]
	ldrb r2, [r4, #0x14]
	lsls r0, r2, #3
	adds r0, r0, r1
	ldr r1, [r0]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _08017844 @ =gStaticData_0816B8C0
	adds r5, r0, r1
	ldrb r0, [r4, #0x19]
	cmp r0, #0
	beq _08017848
	ldr r2, [r4, #0xc]
	movs r1, #0x38
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #0x3c]
	adds r2, r5, #0
	bl sub_803AD84
	b _0801785A
	.align 2, 0
_08017844: .4byte gStaticData_0816B8C0
_08017848:
	ldr r2, [r4, #0xc]
	movs r1, #0x28
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #0x2c]
	adds r2, r5, #0
	bl sub_803AD84
_0801785A:
	movs r0, #0
	strb r0, [r4, #0x17]
	strb r0, [r4, #0x19]
_08017860:
	ldrb r2, [r4, #0x18]
	cmp r2, #1
	bne _080178B4
	ldr r0, [r4, #4]
	ldr r1, [r0]
	ldrb r2, [r4, #0x15]
	lsls r0, r2, #3
	adds r0, r0, r1
	ldr r1, [r0, #4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _08017898 @ =gStaticData_0816B8C0
	adds r5, r0, r1
	ldrb r0, [r4, #0x1a]
	cmp r0, #0
	beq _0801789C
	ldr r2, [r4, #0xc]
	adds r2, #0x40
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	adds r2, r5, #0
	bl sub_803AD84
	b _080178AE
	.align 2, 0
_08017898: .4byte gStaticData_0816B8C0
_0801789C:
	ldr r2, [r4, #0xc]
	movs r1, #0x30
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #0x34]
	adds r2, r5, #0
	bl sub_803AD84
_080178AE:
	movs r0, #0
	strb r0, [r4, #0x18]
	strb r0, [r4, #0x1a]
_080178B4:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80178BC
sub_80178BC: @ 0x080178BC
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r2, #0
	ldr r2, [r4, #0xc]
	movs r3, #0x20
	ldrsh r0, [r2, r3]
	adds r0, r4, r0
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r2, [r4, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r4, r0
	ldr r1, [r4, #0x10]
	ldr r3, [r2, #4]
	adds r2, r5, #0
	bl sub_803AD84
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80178EC
sub_80178EC: @ 0x080178EC
	push {r4, lr}
	ldr r1, [r0, #0x10]
	adds r0, r1, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801792C
	movs r0, #1
	ldrb r2, [r1, #0xc]
	orrs r0, r2
	strb r0, [r1, #0xc]
	ldr r0, _08017934 @ =0x0000FFFF
	ldrh r4, [r1, #8]
	cmp r4, r0
	beq _0801792C
	ldrh r3, [r1, #8]
	ldr r0, _08017938 @ =gUnknown_030012B4
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
_0801792C:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08017934: .4byte 0x0000FFFF
_08017938: .4byte gUnknown_030012B4

	thumb_func_start sub_801793C
sub_801793C: @ 0x0801793C
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r0, [r4, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _08017964
	movs r0, #0
	str r0, [sp]
	adds r0, r4, #0
	movs r1, #1
	movs r2, #0
	movs r3, #0
	bl sub_80178BC
	movs r1, #2
	movs r0, #1
	strb r0, [r4, #0x17]
	strb r1, [r4, #0x14]
_08017964:
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_801796C
sub_801796C: @ 0x0801796C
	push {lr}
	sub sp, #4
	adds r1, r0, #0
	ldr r0, [r1, #0x10]
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801798C
	movs r0, #0
	str r0, [sp]
	adds r0, r1, #0
	movs r1, #2
	movs r2, #0
	movs r3, #0
	bl sub_80178BC
_0801798C:
	add sp, #4
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8017994
sub_8017994: @ 0x08017994
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r0, #0
	movs r5, #0
	str r5, [sp]
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl sub_80178BC
	movs r0, #1
	strb r0, [r4, #0x17]
	strb r5, [r4, #0x14]
	strb r0, [r4, #0x18]
	strb r5, [r4, #0x15]
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_80179BC
sub_80179BC: @ 0x080179BC
	movs r2, #1
	str r2, [r0, #8]
	movs r1, #0
	strb r1, [r0, #0x14]
	strb r1, [r0, #0x15]
	strb r2, [r0, #0x17]
	strb r2, [r0, #0x18]
	str r1, [r0, #0x10]
	str r1, [r0, #0x1c]
	adds r0, #0x20
	strb r1, [r0]
	bx lr

	thumb_func_start sub_80179D4
sub_80179D4: @ 0x080179D4
	push {lr}
	cmp r2, #1
	blt _080179E4
	cmp r2, #4
	bgt _080179E4
	movs r1, #1
	bl sub_8017564
_080179E4:
	pop {r0}
	bx r0

	thumb_func_start sub_80179E8
sub_80179E8: @ 0x080179E8
	str r1, [r0, #0x10]
	bx lr

	thumb_func_start sub_80179EC
sub_80179EC: @ 0x080179EC
	push {lr}
	ldr r2, _080179FC @ =gStaticData_087E42F4
	str r2, [r0, #0xc]
	bl sub_800B8A8
	pop {r0}
	bx r0
	.align 2, 0
_080179FC: .4byte gStaticData_087E42F4

	thumb_func_start sub_8017A00
sub_8017A00: @ 0x08017A00
	push {r4, lr}
	adds r4, r0, #0
	bl sub_800B8C8
	ldr r0, _08017A1C @ =gStaticData_087E42F4
	str r0, [r4, #0xc]
	adds r0, r4, #0
	bl sub_80179BC
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08017A1C: .4byte gStaticData_087E42F4

	thumb_func_start sub_8017A20
sub_8017A20: @ 0x08017A20
	movs r1, #1
	strb r1, [r0, #0x18]
	bx lr
	.align 2, 0

	thumb_func_start sub_8017A28
sub_8017A28: @ 0x08017A28
	movs r1, #1
	strb r1, [r0, #0x17]
	bx lr
	.align 2, 0

	thumb_func_start sub_8017A30
sub_8017A30: @ 0x08017A30
	movs r1, #0
	strb r1, [r0, #0x18]
	strb r1, [r0, #0x1a]
	bx lr

	thumb_func_start sub_8017A38
sub_8017A38: @ 0x08017A38
	movs r1, #0
	strb r1, [r0, #0x17]
	strb r1, [r0, #0x19]
	bx lr

	thumb_func_start sub_8017A40
sub_8017A40: @ 0x08017A40
	ldrb r0, [r0, #0x18]
	bx lr

