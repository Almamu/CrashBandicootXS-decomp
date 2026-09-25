.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8026C90
sub_8026C90: @ 0x08026C90
	push {r4, r5, r6, lr}
	adds r1, r0, #0
	ldr r0, [r1, #0x10]
	ldr r2, [r0]
	ldr r3, [r0, #4]
	adds r0, #0x24
	ldrb r5, [r0]
	cmp r5, #0
	beq _08026D5C
	movs r0, #4
	ands r0, r5
	cmp r0, #0
	beq _08026CC0
	ldr r4, [r1, #0xc]
	ldr r0, _08026CB8 @ =0xFFFFE556
	cmp r4, r0
	ble _08026CC0
	ldr r6, _08026CBC @ =0xFFFFFF00
	b _08026CD4
	.align 2, 0
_08026CB8: .4byte 0xFFFFE556
_08026CBC: .4byte 0xFFFFFF00
_08026CC0:
	movs r0, #8
	ands r0, r5
	cmp r0, #0
	beq _08026CD8
	ldr r4, [r1, #0xc]
	ldr r0, _08026CEC @ =0x00001AA9
	cmp r4, r0
	bgt _08026CD8
	movs r6, #0x80
	lsls r6, r6, #1
_08026CD4:
	adds r0, r4, r6
	str r0, [r1, #0xc]
_08026CD8:
	movs r0, #2
	ands r0, r5
	cmp r0, #0
	beq _08026CF8
	ldr r4, [r1, #8]
	ldr r0, _08026CF0 @ =0xFFFFD800
	cmp r4, r0
	ble _08026CF8
	ldr r6, _08026CF4 @ =0xFFFFFF00
	b _08026D0C
	.align 2, 0
_08026CEC: .4byte 0x00001AA9
_08026CF0: .4byte 0xFFFFD800
_08026CF4: .4byte 0xFFFFFF00
_08026CF8:
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _08026D10
	ldr r4, [r1, #8]
	ldr r0, _08026D24 @ =0x000027FF
	cmp r4, r0
	bgt _08026D10
	movs r6, #0x80
	lsls r6, r6, #1
_08026D0C:
	adds r0, r4, r6
	str r0, [r1, #8]
_08026D10:
	movs r0, #3
	ands r0, r5
	cmp r0, #0
	bne _08026D38
	ldr r0, [r1, #8]
	cmp r0, #0
	ble _08026D2C
	ldr r4, _08026D28 @ =0xFFFFFF00
	adds r0, r0, r4
	b _08026D36
	.align 2, 0
_08026D24: .4byte 0x000027FF
_08026D28: .4byte 0xFFFFFF00
_08026D2C:
	cmp r0, #0
	bge _08026D38
	movs r6, #0x80
	lsls r6, r6, #1
	adds r0, r0, r6
_08026D36:
	str r0, [r1, #8]
_08026D38:
	movs r0, #0xc
	ands r5, r0
	cmp r5, #0
	bne _08026D5C
	ldr r0, [r1, #0xc]
	cmp r0, #0
	ble _08026D50
	ldr r4, _08026D4C @ =0xFFFFFF00
	adds r0, r0, r4
	b _08026D5A
	.align 2, 0
_08026D4C: .4byte 0xFFFFFF00
_08026D50:
	cmp r0, #0
	bge _08026D5C
	movs r6, #0x80
	lsls r6, r6, #1
	adds r0, r0, r6
_08026D5A:
	str r0, [r1, #0xc]
_08026D5C:
	ldr r0, [r1, #8]
	adds r2, r2, r0
	ldr r0, [r1, #0xc]
	adds r3, r3, r0
	ldr r4, [r1]
	subs r0, r2, r4
	cmp r0, #0
	bge _08026D6E
	adds r0, #3
_08026D6E:
	asrs r0, r0, #2
	adds r0, r4, r0
	str r0, [r1]
	ldr r4, [r1, #4]
	subs r0, r3, r4
	cmp r0, #0
	bge _08026D7E
	adds r0, #3
_08026D7E:
	asrs r0, r0, #2
	adds r0, r4, r0
	str r0, [r1, #4]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8026D8C
sub_8026D8C: @ 0x08026D8C
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x10]
	ldr r2, [r0]
	ldr r3, [r0, #4]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _08026DB4
	ldr r1, [r4, #8]
	ldr r0, _08026DAC @ =0xFFFFED8A
	cmp r1, r0
	ble _08026DC4
	ldr r5, _08026DB0 @ =0xFFFFFF00
	b _08026DC0
	.align 2, 0
_08026DAC: .4byte 0xFFFFED8A
_08026DB0: .4byte 0xFFFFFF00
_08026DB4:
	ldr r1, [r4, #8]
	ldr r0, _08026DF4 @ =0x00001275
	cmp r1, r0
	bgt _08026DC4
	movs r5, #0x80
	lsls r5, r5, #1
_08026DC0:
	adds r0, r1, r5
	str r0, [r4, #8]
_08026DC4:
	ldr r1, _08026DF8 @ =0xFFFFF000
	str r1, [r4, #0xc]
	ldr r0, [r4, #8]
	adds r2, r2, r0
	adds r3, r3, r1
	ldr r1, [r4]
	subs r0, r2, r1
	cmp r0, #0
	bge _08026DD8
	adds r0, #3
_08026DD8:
	asrs r0, r0, #2
	adds r0, r1, r0
	str r0, [r4]
	ldr r1, [r4, #4]
	subs r0, r3, r1
	cmp r0, #0
	bge _08026DE8
	adds r0, #3
_08026DE8:
	asrs r0, r0, #2
	adds r0, r1, r0
	str r0, [r4, #4]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08026DF4: .4byte 0x00001275
_08026DF8: .4byte 0xFFFFF000

	thumb_func_start sub_8026DFC
sub_8026DFC: @ 0x08026DFC
	push {lr}
	adds r3, r0, #0
	ldr r1, [r3, #0x10]
	ldr r0, [r1]
	str r0, [r3]
	ldr r0, [r1, #4]
	str r0, [r3, #4]
	ldr r0, [r3, #0x14]
	cmp r0, #1
	bne _08026E34
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _08026E24
	ldr r0, _08026E20 @ =0xFFFFED8A
	b _08026E26
	.align 2, 0
_08026E20: .4byte 0xFFFFED8A
_08026E24:
	ldr r0, _08026E2C @ =0x00001276
_08026E26:
	str r0, [r3, #8]
	ldr r0, _08026E30 @ =0xFFFFF000
	b _08026E38
	.align 2, 0
_08026E2C: .4byte 0x00001276
_08026E30: .4byte 0xFFFFF000
_08026E34:
	movs r0, #0
	str r0, [r3, #8]
_08026E38:
	str r0, [r3, #0xc]
	ldr r1, [r3]
	ldr r0, [r3, #8]
	adds r1, r1, r0
	str r1, [r3]
	ldr r2, [r3, #4]
	ldr r0, [r3, #0xc]
	adds r2, r2, r0
	str r2, [r3, #4]
	ldr r0, _08026E60 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r3, _08026E64 @ =0xFFFF8800
	adds r1, r1, r3
	ldr r3, _08026E68 @ =0xFFFFB000
	adds r2, r2, r3
	bl sub_80268D0
	pop {r0}
	bx r0
	.align 2, 0
_08026E60: .4byte gUnknown_03001308
_08026E64: .4byte 0xFFFF8800
_08026E68: .4byte 0xFFFFB000

	thumb_func_start sub_8026E6C
sub_8026E6C: @ 0x08026E6C
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x14]
	cmp r0, #2
	beq _08026E86
	cmp r0, #2
	bgt _08026E8C
	cmp r0, #1
	bne _08026E8C
	adds r0, r4, #0
	bl sub_8026D8C
	b _08026E8C
_08026E86:
	adds r0, r4, #0
	bl sub_8026C90
_08026E8C:
	ldr r0, _08026EA8 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r1, [r4]
	ldr r2, _08026EAC @ =0xFFFF8800
	adds r1, r1, r2
	ldr r2, [r4, #4]
	ldr r3, _08026EB0 @ =0xFFFFB000
	adds r2, r2, r3
	bl sub_80268D0
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08026EA8: .4byte gUnknown_03001308
_08026EAC: .4byte 0xFFFF8800
_08026EB0: .4byte 0xFFFFB000

	thumb_func_start sub_8026EB4
sub_8026EB4: @ 0x08026EB4
	push {lr}
	bl mem_free
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8026EC0
sub_8026EC0: @ 0x08026EC0
	push {lr}
	movs r1, #0x80
	lsls r1, r1, #0x17
	bl mem_alloc
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8026ED0
sub_8026ED0: @ 0x08026ED0
	push {lr}
	bl mem_free
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8026EDC
sub_8026EDC: @ 0x08026EDC
	push {lr}
	movs r1, #0x80
	lsls r1, r1, #0x17
	bl mem_alloc
	pop {r1}
	bx r1
	.align 2, 0

