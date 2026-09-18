.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8010914
sub_8010914: @ 0x08010914
	push {r4, r5, lr}
	adds r5, r0, #0
	bl sub_801070C
	adds r4, r0, #0
	cmp r4, #0
	beq _08010930
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #1
	bne _08010938
_08010930:
	adds r0, r5, #0
	b _08010956
_08010934:
	adds r0, r4, #0
	b _08010956
_08010938:
	adds r0, r4, #0
	bl sub_801070C
	adds r2, r0, #0
	cmp r2, #0
	beq _08010934
	adds r1, r2, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #1
	beq _08010934
	adds r4, r2, #0
	b _08010938
_08010956:
	pop {r4, r5}
	pop {r1}
	bx r1

	thumb_func_start sub_801095C
sub_801095C: @ 0x0801095C
	push {r4, r5, lr}
	adds r5, r0, #0
	bl sub_8010708
	adds r4, r0, #0
	cmp r4, #0
	beq _08010978
	adds r1, r4, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #1
	bne _08010980
_08010978:
	adds r0, r5, #0
	b _0801099E
_0801097C:
	adds r0, r4, #0
	b _0801099E
_08010980:
	adds r0, r4, #0
	bl sub_8010708
	adds r2, r0, #0
	cmp r2, #0
	beq _0801097C
	adds r1, r2, #0
	adds r1, #0x4d
	movs r0, #0x7f
	ldrb r1, [r1]
	ands r0, r1
	cmp r0, #1
	beq _0801097C
	adds r4, r2, #0
	b _08010980
_0801099E:
	pop {r4, r5}
	pop {r1}
	bx r1

	thumb_func_start sub_80109A4
sub_80109A4: @ 0x080109A4
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	movs r0, #0x4d
	adds r0, r0, r4
	mov ip, r0
	movs r0, #0x7f
	mov r5, ip
	ldrb r5, [r5]
	ands r0, r5
	cmp r0, #1
	beq _080109E8
	ldr r0, [r4]
	subs r0, r0, r2
	cmp r0, #0
	bge _080109C4
	rsbs r0, r0, #0
_080109C4:
	ldr r2, _080109FC @ =0x00003FFF
	cmp r0, r2
	bgt _080109E8
	ldr r0, [r4, #4]
	subs r0, r0, r3
	cmp r0, #0
	bge _080109D4
	rsbs r0, r0, #0
_080109D4:
	cmp r0, r2
	bgt _080109E8
	adds r0, r4, #0
	adds r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #5
	beq _080109E8
	adds r0, r4, #0
	bl sub_0800D18C
_080109E8:
	movs r0, #9
	rsbs r0, r0, #0
	ldrb r6, [r4, #0xc]
	ands r0, r6
	strb r0, [r4, #0xc]
	movs r0, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_080109FC: .4byte 0x00003FFF

