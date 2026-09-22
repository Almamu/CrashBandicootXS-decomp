.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_802AC28
sub_802AC28: @ 0x0802AC28
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	adds r7, r1, #0
	mov r8, r2
	adds r6, r3, #0
	ldr r1, [sp, #0x24]
	mov sb, r1
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	ldr r0, _0802AC68 @ =gUnknown_0300147C
	ldr r1, [r0]
	lsls r0, r5, #2
	adds r0, r0, r5
	lsls r0, r0, #3
	adds r0, r0, r1
	ldr r1, [r0, #0x20]
	adds r7, r7, r1
	ldr r0, [r0, #0x24]
	add r8, r0
	subs r0, r5, #1
	cmp r0, #0x26
	bls _0802AC5C
	b _0802B11C
_0802AC5C:
	lsls r0, r0, #2
	ldr r1, _0802AC6C @ =_0802AC70
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0802AC68: .4byte gUnknown_0300147C
_0802AC6C: .4byte _0802AC70
_0802AC70: @ jump table
	.4byte _0802AD94 @ case 0
	.4byte _0802B0E0 @ case 1
	.4byte _0802AD3C @ case 2
	.4byte _0802ADC4 @ case 3
	.4byte _0802AD64 @ case 4
	.4byte _0802AD64 @ case 5
	.4byte _0802AD64 @ case 6
	.4byte _0802AF14 @ case 7
	.4byte _0802AD0C @ case 8
	.4byte _0802AF88 @ case 9
	.4byte _0802AFB8 @ case 10
	.4byte _0802AE1C @ case 11
	.4byte _0802B08C @ case 12
	.4byte _0802B11C @ case 13
	.4byte _0802B11C @ case 14
	.4byte _0802B010 @ case 15
	.4byte _0802B11C @ case 16
	.4byte _0802B010 @ case 17
	.4byte _0802B11C @ case 18
	.4byte _0802B010 @ case 19
	.4byte _0802B11C @ case 20
	.4byte _0802ADF4 @ case 21
	.4byte _0802AFE8 @ case 22
	.4byte _0802AE44 @ case 23
	.4byte _0802B038 @ case 24
	.4byte _0802B11C @ case 25
	.4byte _0802B11C @ case 26
	.4byte _0802AE70 @ case 27
	.4byte _0802AE70 @ case 28
	.4byte _0802AE70 @ case 29
	.4byte _0802AE70 @ case 30
	.4byte _0802B11C @ case 31
	.4byte _0802B11C @ case 32
	.4byte _0802B11C @ case 33
	.4byte _0802AEA0 @ case 34
	.4byte _0802B114 @ case 35
	.4byte _0802B114 @ case 36
	.4byte _0802B114 @ case 37
	.4byte _0802B114 @ case 38
_0802AD0C:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	lsls r1, r5, #2
	adds r1, r1, r5
	lsls r1, r1, #3
	ldr r0, _0802AD34 @ =gUnknown_0300147C
	ldr r0, [r0]
	adds r1, r1, r0
	str r6, [sp]
	adds r0, r4, #0
	adds r2, r7, #0
	mov r3, r8
	bl sub_802CB34
	ldr r0, _0802AD38 @ =gStaticData_087E4EF4
	b _0802B106
	.align 2, 0
_0802AD34: .4byte gUnknown_0300147C
_0802AD38: .4byte gStaticData_087E4EF4
_0802AD3C:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802AD60 @ =gUnknown_0300147C
	lsls r2, r5, #2
	adds r2, r2, r5
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	ldr r2, [r1, #0x20]
	str r6, [sp]
	mov r3, r8
	bl sub_802D764
	b _0802B11E
	.align 2, 0
_0802AD60: .4byte gUnknown_0300147C
_0802AD64:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	lsls r1, r5, #2
	adds r1, r1, r5
	lsls r1, r1, #3
	ldr r0, _0802AD8C @ =gUnknown_0300147C
	ldr r0, [r0]
	adds r1, r1, r0
	str r6, [sp]
	adds r0, r4, #0
	adds r2, r7, #0
	mov r3, r8
	bl sub_802CB34
	ldr r0, _0802AD90 @ =gStaticData_087E4EB4
	b _0802B106
	.align 2, 0
_0802AD8C: .4byte gUnknown_0300147C
_0802AD90: .4byte gStaticData_087E4EB4
_0802AD94:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	lsls r1, r5, #2
	adds r1, r1, r5
	lsls r1, r1, #3
	ldr r0, _0802ADBC @ =gUnknown_0300147C
	ldr r0, [r0]
	adds r1, r1, r0
	str r6, [sp]
	adds r0, r4, #0
	adds r2, r7, #0
	mov r3, r8
	bl sub_802CB34
	ldr r0, _0802ADC0 @ =gStaticData_087E4F74
	b _0802B106
	.align 2, 0
_0802ADBC: .4byte gUnknown_0300147C
_0802ADC0: .4byte gStaticData_087E4F74
_0802ADC4:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	lsls r1, r5, #2
	adds r1, r1, r5
	lsls r1, r1, #3
	ldr r0, _0802ADEC @ =gUnknown_0300147C
	ldr r0, [r0]
	adds r1, r1, r0
	str r6, [sp]
	adds r0, r4, #0
	adds r2, r7, #0
	mov r3, r8
	bl sub_802CB34
	ldr r0, _0802ADF0 @ =gStaticData_087E4F14
	b _0802B106
	.align 2, 0
_0802ADEC: .4byte gUnknown_0300147C
_0802ADF0: .4byte gStaticData_087E4F14
_0802ADF4:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802AE18 @ =gUnknown_0300147C
	lsls r2, r5, #2
	adds r2, r2, r5
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	str r6, [sp]
	adds r2, r7, #0
	mov r3, r8
	bl sub_802CF0C
	b _0802B11E
	.align 2, 0
_0802AE18: .4byte gUnknown_0300147C
_0802AE1C:
	movs r0, #0x58
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802AE40 @ =gUnknown_0300147C
	lsls r2, r5, #2
	adds r2, r2, r5
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	str r6, [sp]
	adds r2, r7, #0
	mov r3, r8
	bl sub_802D648
	b _0802B11E
	.align 2, 0
_0802AE40: .4byte gUnknown_0300147C
_0802AE44:
	movs r0, #0x68
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802AE6C @ =gUnknown_0300147C
	lsls r2, r5, #2
	adds r2, r2, r5
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	str r6, [sp]
	mov r2, sb
	str r2, [sp, #4]
	adds r2, r7, #0
	mov r3, r8
	bl sub_802D0C8
	b _0802B11E
	.align 2, 0
_0802AE6C: .4byte gUnknown_0300147C
_0802AE70:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	lsls r1, r5, #2
	adds r1, r1, r5
	lsls r1, r1, #3
	ldr r0, _0802AE98 @ =gUnknown_0300147C
	ldr r0, [r0]
	adds r1, r1, r0
	str r6, [sp]
	adds r0, r4, #0
	adds r2, r7, #0
	mov r3, r8
	bl sub_802CB34
	ldr r0, _0802AE9C @ =gStaticData_087E4ED4
	b _0802B106
	.align 2, 0
_0802AE98: .4byte gUnknown_0300147C
_0802AE9C: .4byte gStaticData_087E4ED4
_0802AEA0:
	mov r0, sb
	bl sub_802AA80
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802AEDC
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	ldr r0, _0802AED4 @ =gUnknown_0300147C
	ldr r1, [r0]
	movs r0, #0x8c
	lsls r0, r0, #3
	adds r1, r1, r0
	str r6, [sp]
	adds r0, r4, #0
	adds r2, r7, #0
	mov r3, r8
	bl sub_802CB34
	ldr r0, _0802AED8 @ =gStaticData_087E4ED4
	b _0802B106
	.align 2, 0
_0802AED4: .4byte gUnknown_0300147C
_0802AED8: .4byte gStaticData_087E4ED4
_0802AEDC:
	movs r0, #0x58
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	lsls r1, r5, #2
	adds r1, r1, r5
	lsls r1, r1, #3
	ldr r0, _0802AF0C @ =gUnknown_0300147C
	ldr r0, [r0]
	adds r1, r1, r0
	str r6, [sp]
	adds r0, r4, #0
	adds r2, r7, #0
	mov r3, r8
	bl sub_802CB34
	ldr r0, _0802AF10 @ =gStaticData_087E4F34
	str r0, [r4, #0x50]
	mov r1, sb
	str r1, [r4, #0x54]
	adds r0, r4, #0
	b _0802B11E
	.align 2, 0
_0802AF0C: .4byte gUnknown_0300147C
_0802AF10: .4byte gStaticData_087E4F34
_0802AF14:
	mov r0, sb
	bl sub_802AA80
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802AF50
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	ldr r0, _0802AF48 @ =gUnknown_0300147C
	ldr r1, [r0]
	movs r2, #0x8c
	lsls r2, r2, #3
	adds r1, r1, r2
	str r6, [sp]
	adds r0, r4, #0
	adds r2, r7, #0
	mov r3, r8
	bl sub_802CB34
	ldr r0, _0802AF4C @ =gStaticData_087E4ED4
	b _0802B106
	.align 2, 0
_0802AF48: .4byte gUnknown_0300147C
_0802AF4C: .4byte gStaticData_087E4ED4
_0802AF50:
	movs r0, #0x58
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	lsls r1, r5, #2
	adds r1, r1, r5
	lsls r1, r1, #3
	ldr r0, _0802AF80 @ =gUnknown_0300147C
	ldr r0, [r0]
	adds r1, r1, r0
	str r6, [sp]
	adds r0, r4, #0
	adds r2, r7, #0
	mov r3, r8
	bl sub_802CB34
	ldr r0, _0802AF84 @ =gStaticData_087E4F34
	str r0, [r4, #0x50]
	mov r0, sb
	str r0, [r4, #0x54]
	adds r0, r4, #0
	b _0802B11E
	.align 2, 0
_0802AF80: .4byte gUnknown_0300147C
_0802AF84: .4byte gStaticData_087E4F34
_0802AF88:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	lsls r1, r5, #2
	adds r1, r1, r5
	lsls r1, r1, #3
	ldr r0, _0802AFB0 @ =gUnknown_0300147C
	ldr r0, [r0]
	adds r1, r1, r0
	str r6, [sp]
	adds r0, r4, #0
	adds r2, r7, #0
	mov r3, r8
	bl sub_802CB34
	ldr r0, _0802AFB4 @ =gStaticData_087E4F54
	b _0802B106
	.align 2, 0
_0802AFB0: .4byte gUnknown_0300147C
_0802AFB4: .4byte gStaticData_087E4F54
_0802AFB8:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	lsls r1, r5, #2
	adds r1, r1, r5
	lsls r1, r1, #3
	ldr r0, _0802AFE0 @ =gUnknown_0300147C
	ldr r0, [r0]
	adds r1, r1, r0
	str r6, [sp]
	adds r0, r4, #0
	adds r2, r7, #0
	mov r3, r8
	bl InitActorPart
	ldr r0, _0802AFE4 @ =gStaticData_087E4E94
	b _0802B106
	.align 2, 0
_0802AFE0: .4byte gUnknown_0300147C
_0802AFE4: .4byte gStaticData_087E4E94
_0802AFE8:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802B00C @ =gUnknown_0300147C
	lsls r2, r5, #2
	adds r2, r2, r5
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	str r6, [sp]
	adds r2, r7, #0
	mov r3, r8
	bl sub_802CDE4
	b _0802B11E
	.align 2, 0
_0802B00C: .4byte gUnknown_0300147C
_0802B010:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802B034 @ =gUnknown_0300147C
	lsls r2, r5, #2
	adds r2, r2, r5
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	str r6, [sp]
	adds r2, r7, #0
	mov r3, r8
	bl sub_802D1B8
	b _0802B11E
	.align 2, 0
_0802B034: .4byte gUnknown_0300147C
_0802B038:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r4, _0802B088 @ =gUnknown_0300147C
	ldr r1, [r4]
	movs r2, #0x82
	lsls r2, r2, #3
	adds r1, r1, r2
	ldr r2, [r1, #0x20]
	str r6, [sp]
	mov r3, r8
	bl sub_802D5D4
	movs r1, #1
	str r1, [r0, #0xc]
	ldr r1, [r0]
	ldrh r1, [r1, #0xc]
	movs r2, #0
	movs r3, #0
	strh r1, [r0, #0x10]
	strb r2, [r0, #0x12]
	str r3, [r0, #8]
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	lsls r2, r5, #2
	adds r2, r2, r5
	lsls r2, r2, #3
	ldr r1, [r4]
	adds r1, r1, r2
	ldr r2, [r1, #0x20]
	str r6, [sp]
	mov r3, r8
	bl sub_802D5D4
	b _0802B11E
	.align 2, 0
_0802B088: .4byte gUnknown_0300147C
_0802B08C:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r4, _0802B0DC @ =gUnknown_0300147C
	ldr r1, [r4]
	movs r2, #0x8c
	lsls r2, r2, #2
	adds r1, r1, r2
	ldr r2, [r1, #0x20]
	str r6, [sp]
	mov r3, r8
	bl sub_802CE38
	movs r1, #1
	str r1, [r0, #0xc]
	ldr r1, [r0]
	ldrh r1, [r1, #0xc]
	movs r2, #0
	movs r3, #0
	strh r1, [r0, #0x10]
	strb r2, [r0, #0x12]
	str r3, [r0, #8]
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	lsls r2, r5, #2
	adds r2, r2, r5
	lsls r2, r2, #3
	ldr r1, [r4]
	adds r1, r1, r2
	ldr r2, [r1, #0x20]
	str r6, [sp]
	mov r3, r8
	bl sub_802CE38
	b _0802B11E
	.align 2, 0
_0802B0DC: .4byte gUnknown_0300147C
_0802B0E0:
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	lsls r1, r5, #2
	adds r1, r1, r5
	lsls r1, r1, #3
	ldr r0, _0802B10C @ =gUnknown_0300147C
	ldr r0, [r0]
	adds r1, r1, r0
	str r6, [sp]
	adds r0, r4, #0
	adds r2, r7, #0
	mov r3, r8
	bl InitActorPart
	ldr r0, _0802B110 @ =gStaticData_087E4E14
_0802B106:
	str r0, [r4, #0x50]
	adds r0, r4, #0
	b _0802B11E
	.align 2, 0
_0802B10C: .4byte gUnknown_0300147C
_0802B110: .4byte gStaticData_087E4E14
_0802B114:
	adds r0, r5, #0
	subs r0, #0x24
	bl sub_802ABC8
_0802B11C:
	movs r0, #0
_0802B11E:
	add sp, #8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_802B12C
sub_802B12C: @ 0x0802B12C
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	sub sp, #4
	adds r6, r0, #0
	mov r8, r1
	adds r5, r2, #0
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	ldr r0, _0802B16C @ =gUnknown_0300147C
	ldr r1, [r0]
	movs r0, #0xc8
	lsls r0, r0, #3
	adds r1, r1, r0
	str r5, [sp]
	adds r0, r4, #0
	adds r2, r6, #0
	mov r3, r8
	bl InitActorPart
	ldr r0, _0802B170 @ =gStaticData_087E4E34
	str r0, [r4, #0x50]
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802B16C: .4byte gUnknown_0300147C
_0802B170: .4byte gStaticData_087E4E34

	thumb_func_start sub_802B174
sub_802B174: @ 0x0802B174
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r6, r1, #0
	adds r4, r2, #0
	movs r0, #0x60
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802B1A4 @ =gUnknown_0300147C
	ldr r1, [r1]
	movs r2, #0xdc
	lsls r2, r2, #1
	adds r1, r1, r2
	str r4, [sp]
	adds r2, r5, #0
	adds r3, r6, #0
	bl sub_802C3E8
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802B1A4: .4byte gUnknown_0300147C

	thumb_func_start sub_802B1A8
sub_802B1A8: @ 0x0802B1A8
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	sub sp, #8
	adds r6, r0, #0
	mov r8, r1
	adds r4, r2, #0
	adds r5, r3, #0
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802B1E4 @ =gUnknown_0300147C
	ldr r1, [r1]
	movs r2, #0x87
	lsls r2, r2, #3
	adds r1, r1, r2
	str r4, [sp]
	str r5, [sp, #4]
	adds r2, r6, #0
	mov r3, r8
	bl sub_802D528
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0802B1E4: .4byte gUnknown_0300147C

	thumb_func_start ConstructAnimTableState
ConstructAnimTableState: @ 0x0802B1E8
	push {r4, r5, r6, lr}
	adds r6, r1, #0
	ldr r4, _0802B210 @ =gUnknown_0300147C
	str r0, [r4]
	ldr r5, _0802B214 @ =gUnknown_03000884
	movs r0, #0
	str r0, [r5]
	movs r0, #0x54
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, [r4]
	adds r2, r6, #0
	bl ConstructActorPart
	str r0, [r5]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802B210: .4byte gUnknown_0300147C
_0802B214: .4byte gUnknown_03000884

	thumb_func_start sub_802B218
sub_802B218: @ 0x0802B218
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r6, r2, #0
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	ldrb r4, [r5]
	ldr r0, _0802B258 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802B25C
	ldrb r4, [r5, #1]
	cmp r4, #0xb
	beq _0802B276
	cmp r4, #3
	beq _0802B254
	cmp r4, #8
	beq _0802B254
	cmp r4, #0x1c
	beq _0802B254
	cmp r4, #0x1d
	beq _0802B254
	cmp r4, #0x1e
	beq _0802B254
	cmp r4, #0x1f
	beq _0802B254
	cmp r4, #0x23
	bne _0802B262
_0802B254:
	movs r4, #1
	b _0802B266
	.align 2, 0
_0802B258: .4byte gUnknown_030012C0
_0802B25C:
	cmp r1, #0
	beq _0802B262
	ldrb r4, [r5, #2]
_0802B262:
	cmp r4, #0
	beq _0802B276
_0802B266:
	cmp r4, #0x20
	beq _0802B276
	cmp r4, #0x21
	beq _0802B276
	cmp r4, #0x22
	beq _0802B276
	cmp r4, #0x3e
	bne _0802B27A
_0802B276:
	movs r0, #0
	b _0802B290
_0802B27A:
	ldr r1, [r5, #4]
	lsls r1, r1, #8
	ldr r2, [r5, #8]
	lsls r2, r2, #8
	ldr r3, [r5, #0xc]
	lsls r3, r3, #8
	adds r3, r3, r6
	str r5, [sp]
	adds r0, r4, #0
	bl sub_802AC28
_0802B290:
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1

	thumb_func_start ConstructActorPart
ConstructActorPart: @ 0x0802B298
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r3, _0802B2D4 @ =0xFFFFB000
	cmp r2, #0
	beq _0802B2A8
	movs r3, #0xa0
	lsls r3, r3, #6
_0802B2A8:
	str r2, [sp]
	adds r0, r4, #0
	movs r2, #0
	bl InitActorPart
	ldr r0, _0802B2D8 @ =gStaticData_087E4E54
	str r0, [r4, #0x50]
	adds r0, r4, #0
	bl sub_802B864
	ldr r2, [r4, #0x24]
	cmp r2, #0
	beq _0802B2DC
	movs r0, #0xd
	movs r1, #0xc
	str r0, [r4, #0x28]
	movs r2, #0
	str r2, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	adds r0, #0x90
	b _0802B2E4
	.align 2, 0
_0802B2D4: .4byte 0xFFFFB000
_0802B2D8: .4byte gStaticData_087E4E54
_0802B2DC:
	movs r0, #8
	str r0, [r4, #0xc]
	ldr r0, [r4]
	adds r0, #0x60
_0802B2E4:
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	ldr r0, _0802B330 @ =gUnknown_030014A4
	movs r1, #0
	str r1, [r0]
	ldr r0, _0802B334 @ =gUnknown_03001490
	str r1, [r0]
	ldr r0, _0802B338 @ =gUnknown_03001494
	str r1, [r0]
	ldr r0, _0802B33C @ =gUnknown_030014A3
	strb r1, [r0]
	ldr r0, _0802B340 @ =gUnknown_0300149C
	str r1, [r0]
	ldr r0, _0802B344 @ =gUnknown_03001498
	str r1, [r0]
	ldr r0, _0802B348 @ =gUnknown_0300148C
	str r1, [r0]
	ldr r2, _0802B34C @ =gUnknown_030014A0
	movs r0, #1
	strb r0, [r2]
	ldr r0, _0802B350 @ =gUnknown_030014A2
	strb r1, [r0]
	ldr r0, _0802B354 @ =gUnknown_030014A1
	strb r1, [r0]
	ldr r0, _0802B358 @ =gUnknown_03001488
	str r1, [r0]
	ldr r0, _0802B35C @ =gUnknown_03001484
	str r1, [r0]
	ldr r0, _0802B360 @ =gUnknown_03001480
	strb r1, [r0]
	adds r0, r4, #0
	add sp, #4
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0802B330: .4byte gUnknown_030014A4
_0802B334: .4byte gUnknown_03001490
_0802B338: .4byte gUnknown_03001494
_0802B33C: .4byte gUnknown_030014A3
_0802B340: .4byte gUnknown_0300149C
_0802B344: .4byte gUnknown_03001498
_0802B348: .4byte gUnknown_0300148C
_0802B34C: .4byte gUnknown_030014A0
_0802B350: .4byte gUnknown_030014A2
_0802B354: .4byte gUnknown_030014A1
_0802B358: .4byte gUnknown_03001488
_0802B35C: .4byte gUnknown_03001484
_0802B360: .4byte gUnknown_03001480

	thumb_func_start sub_802B364
sub_802B364: @ 0x0802B364
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	bl sub_802BC68
	ldr r1, _0802B3CC @ =gUnknown_0300148C
	ldr r0, [r1]
	cmp r0, #0
	beq _0802B39E
	subs r5, r0, #1
	str r5, [r1]
	cmp r5, #0
	bne _0802B39E
	ldr r1, _0802B3D0 @ =gUnknown_030014A1
	movs r0, #1
	strb r0, [r1]
	movs r0, #0
	bl sub_8029BAC
	movs r0, #0xa
	str r0, [r4, #0x28]
	str r5, [r4, #0x44]
	str r0, [r4, #0xc]
	ldr r0, [r4]
	adds r0, #0x78
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
_0802B39E:
	ldr r2, _0802B3D4 @ =gUnknown_0300149C
	ldr r0, [r2]
	cmp r0, #0
	beq _0802B3E0
	subs r1, r0, #1
	str r1, [r2]
	cmp r1, #0
	beq _0802B3E0
	ldr r0, _0802B3D8 @ =gUnknown_030014A0
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802B3E0
	ldr r0, _0802B3DC @ =gUnknown_030012C0
	ldr r0, [r0]
	ldr r0, [r0, #0x78]
	cmp r0, #3
	beq _0802B3E0
	lsrs r0, r1, #2
	movs r1, #1
	ands r0, r1
	adds r1, r4, #0
	adds r1, #0x2c
	b _0802B3E6
	.align 2, 0
_0802B3CC: .4byte gUnknown_0300148C
_0802B3D0: .4byte gUnknown_030014A1
_0802B3D4: .4byte gUnknown_0300149C
_0802B3D8: .4byte gUnknown_030014A0
_0802B3DC: .4byte gUnknown_030012C0
_0802B3E0:
	adds r1, r4, #0
	adds r1, #0x2c
	movs r0, #1
_0802B3E6:
	strb r0, [r1]
	ldr r0, _0802B498 @ =gUnknown_030014A1
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802B402
	movs r0, #0xbc
	lsls r0, r0, #6
	str r0, [r4, #0x34]
	bl sub_8029B2C
	lsls r0, r0, #8
	ldr r1, [r4, #0x34]
	subs r0, r0, r1
	str r0, [r4, #0x24]
_0802B402:
	ldr r3, [r4, #0x34]
	asrs r3, r3, #1
	movs r0, #0xff
	lsls r0, r0, #7
	ands r3, r0
	ldr r1, [r4, #0x20]
	asrs r0, r1, #0x1f
	eors r1, r0
	subs r1, r1, r0
	ldr r0, [r4, #0x1c]
	asrs r2, r0, #0x1f
	eors r0, r2
	subs r0, r0, r2
	adds r1, r1, r0
	asrs r1, r1, #0xb
	movs r0, #0x7f
	ands r1, r0
	orrs r3, r1
	str r3, [r4, #0x14]
	ldr r0, [r4, #0x44]
	adds r0, #1
	str r0, [r4, #0x44]
	movs r0, #0x10
	ldrsh r1, [r4, r0]
	ldr r0, [r4, #8]
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
	blt _0802B468
	movs r5, #6
	ldrsh r0, [r1, r5]
	subs r0, r2, r0
	lsls r0, r0, #8
	ldr r1, [r4, #8]
	subs r1, r1, r0
	str r1, [r4, #8]
	movs r0, #1
	strb r0, [r4, #0x12]
_0802B468:
	ldr r0, [r4, #0x1c]
	ldr r1, [r4, #0x20]
	bl sub_8029D8C
	ldr r3, _0802B49C @ =gStaticData_0817A6B8
	ldr r0, [r4, #0x28]
	lsls r1, r0, #3
	adds r0, r1, r3
	movs r5, #2
	ldrsh r2, [r0, r5]
	cmp r2, #0
	ble _0802B4A0
	movs r1, #4
	ldrsh r0, [r0, r1]
	adds r0, r4, r0
	ldr r1, [r0]
	lsls r0, r2, #3
	adds r0, r0, r1
	subs r0, #8
	ldr r6, [r0]
	ldr r7, [r0, #4]
	adds r3, r7, #0
	b _0802B4A6
	.align 2, 0
_0802B498: .4byte gUnknown_030014A1
_0802B49C: .4byte gStaticData_0817A6B8
_0802B4A0:
	adds r0, r3, #4
	adds r0, r1, r0
	ldr r3, [r0]
_0802B4A6:
	ldr r1, _0802B4C0 @ =gStaticData_0817A6B8
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r1
	movs r5, #0
	ldrsh r1, [r0, r5]
	cmp r2, #0
	ble _0802B4C4
	lsls r0, r6, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _0802B4C6
	.align 2, 0
_0802B4C0: .4byte gStaticData_0817A6B8
_0802B4C4:
	adds r0, r1, #0
_0802B4C6:
	adds r0, r4, r0
	bl sub_803AD84
	ldr r0, _0802B4F8 @ =gUnknown_030014A3
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802B56C
	ldr r0, _0802B4FC @ =gUnknown_030007E0
	ldr r2, [r0]
	movs r1, #0x20
	adds r0, r2, #0
	ands r0, r1
	cmp r0, #0
	beq _0802B524
	ldr r0, _0802B500 @ =gUnknown_03001498
	ldr r1, [r0]
	adds r2, r1, #0
	adds r1, #1
	str r1, [r0]
	cmp r2, #0xc
	ble _0802B508
	ldr r0, [r4, #0x1c]
	ldr r1, _0802B504 @ =0xFFFFFC80
	adds r0, r0, r1
	b _0802B50E
	.align 2, 0
_0802B4F8: .4byte gUnknown_030014A3
_0802B4FC: .4byte gUnknown_030007E0
_0802B500: .4byte gUnknown_03001498
_0802B504: .4byte 0xFFFFFC80
_0802B508:
	ldr r0, [r4, #0x1c]
	ldr r3, _0802B51C @ =0xFFFFFD33
	adds r0, r0, r3
_0802B50E:
	str r0, [r4, #0x1c]
	ldr r0, [r4, #0x1c]
	ldr r1, _0802B520 @ =0xFFFFCE00
	cmp r0, r1
	bge _0802B56C
	str r1, [r4, #0x1c]
	b _0802B56C
	.align 2, 0
_0802B51C: .4byte 0xFFFFFD33
_0802B520: .4byte 0xFFFFCE00
_0802B524:
	movs r0, #0x10
	ands r2, r0
	lsls r0, r2, #0x10
	lsrs r1, r0, #0x10
	cmp r1, #0
	beq _0802B568
	ldr r0, _0802B548 @ =gUnknown_03001498
	ldr r1, [r0]
	adds r2, r1, #0
	adds r1, #1
	str r1, [r0]
	cmp r2, #0xc
	ble _0802B54C
	ldr r0, [r4, #0x1c]
	movs r5, #0xe0
	lsls r5, r5, #2
	adds r0, r0, r5
	b _0802B552
	.align 2, 0
_0802B548: .4byte gUnknown_03001498
_0802B54C:
	ldr r0, [r4, #0x1c]
	ldr r1, _0802B564 @ =0x000002CD
	adds r0, r0, r1
_0802B552:
	str r0, [r4, #0x1c]
	ldr r0, [r4, #0x1c]
	movs r1, #0xc8
	lsls r1, r1, #6
	cmp r0, r1
	ble _0802B56C
	str r1, [r4, #0x1c]
	b _0802B56C
	.align 2, 0
_0802B564: .4byte 0x000002CD
_0802B568:
	ldr r0, _0802B580 @ =gUnknown_03001498
	str r1, [r0]
_0802B56C:
	ldr r5, _0802B584 @ =gUnknown_03001494
	ldr r0, [r5]
	cmp r0, #0
	beq _0802B588
	ldr r1, [r4, #0x1c]
	ldr r2, [r4, #0x20]
	ldr r3, [r4, #0x24]
	bl sub_802D3A8
	b _0802B5AA
	.align 2, 0
_0802B580: .4byte gUnknown_03001498
_0802B584: .4byte gUnknown_03001494
_0802B588:
	ldr r0, _0802B5B0 @ =gUnknown_030012C0
	ldr r0, [r0]
	ldr r3, [r0, #0x78]
	ldr r0, [r4, #0x1c]
	ldr r1, [r4, #0x20]
	ldr r2, [r4, #0x24]
	bl sub_802B1A8
	str r0, [r5]
	bl sub_8029794
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802B5AA
	ldr r0, [r5]
	bl sub_802D4EC
_0802B5AA:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802B5B0: .4byte gUnknown_030012C0

	thumb_func_start sub_802B5B4
sub_802B5B4: @ 0x0802B5B4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	adds r7, r0, #0
	movs r0, #0
	str r0, [sp, #4]
	ldr r2, [r7, #8]
	asrs r2, r2, #8
	ldr r1, [r7, #0xc]
	ldr r3, [r7]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r3
	movs r1, #2
	ldrsh r0, [r0, r1]
	adds r0, r0, r2
	ldr r1, [r7, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	mov sl, r0
	ldrb r0, [r0]
	str r0, [sp, #8]
	lsls r0, r0, #2
	mov sb, r0
	mov r1, sl
	ldrb r1, [r1, #1]
	str r1, [sp, #0xc]
	lsls r1, r1, #2
	mov r8, r1
	ldr r0, [r7, #0x30]
	ldr r4, [r7, #0x34]
	ldr r1, [r0, #0x10]
	cmp r4, r1
	bne _0802B61E
	movs r0, #0x80
	lsls r0, r0, #1
	str r0, [sp]
	bl sub_8029E98
	ldr r1, [r7, #0x20]
	adds r1, r1, r0
	asrs r5, r1, #8
	bl sub_8029EB4
	ldr r1, [r7, #0x1c]
	adds r1, r1, r0
	asrs r6, r1, #8
	b _0802B66E
_0802B61E:
	lsls r0, r4, #8
	bl sub_803ADB4
	str r0, [sp]
	movs r0, #0xbc
	lsls r0, r0, #0x12
	adds r1, r4, #0
	bl sub_803ADB4
	adds r4, r0, #0
	bl sub_8029E98
	ldr r1, [r7, #0x20]
	muls r1, r4, r1
	asrs r1, r1, #0xc
	adds r1, r1, r0
	asrs r5, r1, #8
	bl sub_8029EB4
	ldr r1, [r7, #0x1c]
	muls r1, r4, r1
	asrs r1, r1, #0xc
	adds r1, r1, r0
	asrs r6, r1, #8
	movs r1, #0x80
	lsls r1, r1, #1
	str r1, [sp, #4]
	ldr r0, [sp]
	cmp r0, #0xff
	bgt _0802B66E
	movs r0, #0x80
	lsls r0, r0, #2
	orrs r1, r0
	str r1, [sp, #4]
	ldr r1, [sp, #8]
	lsls r1, r1, #3
	mov sb, r1
	ldr r0, [sp, #0xc]
	lsls r0, r0, #3
	mov r8, r0
_0802B66E:
	mov r1, sb
	subs r6, r6, r1
	mov r0, r8
	subs r5, r5, r0
	cmp r5, #0x9f
	bgt _0802B706
	lsls r0, r0, #1
	adds r0, r5, r0
	cmp r0, #0
	blt _0802B706
	cmp r6, #0xef
	bgt _0802B706
	lsls r0, r1, #1
	adds r0, r6, r0
	cmp r0, #0
	blt _0802B706
	ldr r1, [r7, #0xc]
	ldr r2, [r7]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrh r0, [r0, #8]
	lsls r4, r0, #0x10
	mov r0, sl
	bl GetSpriteShapeSizeBits
	movs r1, #0xff
	ands r5, r1
	ldr r1, _0802B718 @ =0x000001FF
	ands r6, r1
	lsls r1, r6, #0x10
	orrs r5, r1
	orrs r5, r4
	orrs r5, r0
	ldr r1, [sp, #4]
	orrs r1, r5
	str r1, [sp, #4]
	ldr r4, _0802B71C @ =gUnknown_030014AC
	ldr r0, [r4]
	cmp sl, r0
	beq _0802B6E2
	ldr r2, _0802B720 @ =gUnknown_030014A8
	ldr r0, [r2]
	movs r1, #1
	eors r0, r1
	str r0, [r2]
	ldr r2, _0802B724 @ =gUnknown_03000874
	ldr r1, _0802B728 @ =gUnknown_030014B0
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r2, [r2]
	mov r1, sl
	bl sub_803AD80
	mov r0, sl
	str r0, [r4]
_0802B6E2:
	ldr r1, _0802B728 @ =gUnknown_030014B0
	ldr r0, _0802B720 @ =gUnknown_030014A8
	ldr r0, [r0]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, _0802B72C @ =0xF9FF0000
	adds r0, r0, r1
	lsrs r0, r0, #5
	ldr r1, [r7, #0x18]
	lsls r1, r1, #0xc
	orrs r1, r0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	ldr r0, [sp, #4]
	ldr r2, [sp]
	bl QueueSpriteFrameOam
_0802B706:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802B718: .4byte 0x000001FF
_0802B71C: .4byte gUnknown_030014AC
_0802B720: .4byte gUnknown_030014A8
_0802B724: .4byte gUnknown_03000874
_0802B728: .4byte gUnknown_030014B0
_0802B72C: .4byte 0xF9FF0000

	thumb_func_start sub_802B730
sub_802B730: @ 0x0802B730
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r1, _0802B740 @ =gUnknown_0300149C
	ldr r0, [r1]
	cmp r0, #0
	beq _0802B744
	movs r0, #1
	b _0802B7D8
	.align 2, 0
_0802B740: .4byte gUnknown_0300149C
_0802B744:
	ldr r2, _0802B7AC @ =gUnknown_03001494
	ldr r7, _0802B7B0 @ =gUnknown_030012C0
	ldr r0, [r7]
	ldr r5, [r0, #0x78]
	cmp r5, #0
	bne _0802B7CC
	ldr r0, _0802B7B4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x1b
	bl PlaySfx
	ldr r0, _0802B7B8 @ =gStaticData_0817A728
	ldr r1, _0802B7BC @ =0x05000200
	movs r2, #0x20
	movs r3, #0x10
	bl QueueVramDmaTransfer
	movs r0, #6
	movs r1, #5
	str r0, [r4, #0x28]
	str r5, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0x3c]
	movs r6, #0
	strh r0, [r4, #0x10]
	strb r6, [r4, #0x12]
	str r5, [r4, #8]
	ldr r0, _0802B7C0 @ =gUnknown_03001480
	movs r4, #1
	strb r4, [r0]
	ldr r1, [r7]
	adds r0, r1, #0
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802B798
	adds r0, r1, #0
	bl sub_8023234
_0802B798:
	ldr r0, _0802B7C4 @ =gUnknown_030014A3
	strb r6, [r0]
	ldr r0, _0802B7C8 @ =gUnknown_030014A0
	strb r4, [r0]
	movs r0, #0
	bl sub_8029BAC
	bl sub_802DFBC
	b _0802B7D6
	.align 2, 0
_0802B7AC: .4byte gUnknown_03001494
_0802B7B0: .4byte gUnknown_030012C0
_0802B7B4: .4byte gUnknown_030012BC
_0802B7B8: .4byte gStaticData_0817A728
_0802B7BC: .4byte 0x05000200
_0802B7C0: .4byte gUnknown_03001480
_0802B7C4: .4byte gUnknown_030014A3
_0802B7C8: .4byte gUnknown_030014A0
_0802B7CC:
	movs r0, #0x4b
	str r0, [r1]
	ldr r0, [r2]
	bl sub_802D4B0
_0802B7D6:
	movs r0, #0
_0802B7D8:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_802B7E0
sub_802B7E0: @ 0x0802B7E0
	push {r4, lr}
	adds r2, r0, #0
	ldr r1, _0802B7F0 @ =gUnknown_0300149C
	ldr r0, [r1]
	cmp r0, #0
	beq _0802B7F4
	movs r0, #1
	b _0802B85C
	.align 2, 0
_0802B7F0: .4byte gUnknown_0300149C
_0802B7F4:
	ldr r4, _0802B83C @ =gUnknown_03001494
	ldr r0, _0802B840 @ =gUnknown_030012C0
	ldr r0, [r0]
	ldr r3, [r0, #0x78]
	cmp r3, #0
	bne _0802B850
	movs r0, #0xc
	movs r1, #0xb
	str r0, [r2, #0x28]
	str r3, [r2, #0x44]
	str r1, [r2, #0xc]
	ldr r0, [r2]
	adds r0, #0x84
	ldrh r0, [r0]
	movs r4, #0
	strh r0, [r2, #0x10]
	strb r4, [r2, #0x12]
	str r3, [r2, #8]
	ldr r0, _0802B844 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x33
	bl PlaySfx
	ldr r0, _0802B848 @ =gUnknown_030014A3
	strb r4, [r0]
	ldr r1, _0802B84C @ =gUnknown_030014A0
	movs r0, #1
	strb r0, [r1]
	movs r0, #0
	bl sub_8029BAC
	bl sub_802DFBC
	b _0802B85A
	.align 2, 0
_0802B83C: .4byte gUnknown_03001494
_0802B840: .4byte gUnknown_030012C0
_0802B844: .4byte gUnknown_030012BC
_0802B848: .4byte gUnknown_030014A3
_0802B84C: .4byte gUnknown_030014A0
_0802B850:
	movs r0, #0x4b
	str r0, [r1]
	ldr r0, [r4]
	bl sub_802D4B0
_0802B85A:
	movs r0, #0
_0802B85C:
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_802B864
sub_802B864: @ 0x0802B864
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r2, [r4, #8]
	asrs r2, r2, #8
	ldr r1, [r4, #0xc]
	ldr r3, [r4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r3
	movs r1, #2
	ldrsh r0, [r0, r1]
	adds r0, r0, r2
	ldr r1, [r4, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldrb r3, [r0]
	ldrb r1, [r0, #1]
	adds r2, r3, #0
	muls r2, r1, r2
	adds r0, r2, #0
	lsls r0, r0, #5
	bl AllocVramTileBlock
	ldr r5, _0802B8DC @ =gUnknown_030014B0
	str r0, [r5]
	ldr r2, [r4, #8]
	asrs r2, r2, #8
	ldr r1, [r4, #0xc]
	ldr r3, [r4]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r3
	movs r3, #2
	ldrsh r0, [r0, r3]
	adds r0, r0, r2
	ldr r1, [r4, #4]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldrb r2, [r0]
	ldrb r3, [r0, #1]
	adds r1, r2, #0
	muls r1, r3, r1
	adds r0, r1, #0
	lsls r0, r0, #5
	bl AllocVramTileBlock
	str r0, [r5, #4]
	ldr r1, _0802B8E0 @ =gUnknown_030014A8
	movs r0, #1
	str r0, [r1]
	ldr r1, _0802B8E4 @ =gUnknown_030014AC
	movs r0, #0
	str r0, [r1]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802B8DC: .4byte gUnknown_030014B0
_0802B8E0: .4byte gUnknown_030014A8
_0802B8E4: .4byte gUnknown_030014AC

	thumb_func_start sub_802B8E8
sub_802B8E8: @ 0x0802B8E8
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r4, r0, #0
	ldr r6, _0802B980 @ =gUnknown_03001490
	ldr r5, [r6]
	cmp r5, #0
	bne _0802B918
	ldr r1, [r4, #0x1c]
	movs r2, #0xa0
	lsls r2, r2, #6
	ldr r3, [r4, #0x24]
	str r5, [sp]
	movs r0, #2
	bl sub_802AC28
	str r0, [r6]
	movs r1, #1
	str r1, [r0, #0xc]
	ldr r1, [r0]
	ldrh r1, [r1, #0xc]
	movs r2, #0
	strh r1, [r0, #0x10]
	strb r2, [r0, #0x12]
	str r5, [r0, #8]
_0802B918:
	ldr r2, _0802B984 @ =gUnknown_030014A4
	ldr r1, [r4, #0x20]
	ldr r0, [r2]
	adds r1, r1, r0
	str r1, [r4, #0x20]
	adds r0, #0x2d
	str r0, [r2]
	movs r5, #0xa0
	lsls r5, r5, #6
	cmp r1, r5
	ble _0802B978
	ldr r0, _0802B988 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x35
	bl PlaySfx
	str r5, [r4, #0x20]
	movs r0, #9
	str r0, [r4, #0x28]
	movs r5, #0
	str r5, [r4, #0x44]
	str r0, [r4, #0xc]
	ldr r0, [r4]
	adds r0, #0x6c
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
	ldr r0, _0802B98C @ =gUnknown_030014A0
	strb r1, [r0]
	ldr r2, [r6]
	cmp r2, #0
	beq _0802B970
	ldr r1, [r2, #0x50]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_0802B970:
	str r5, [r6]
	movs r0, #0x19
	bl sub_8029BAC
_0802B978:
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802B980: .4byte gUnknown_03001490
_0802B984: .4byte gUnknown_030014A4
_0802B988: .4byte gUnknown_030012BC
_0802B98C: .4byte gUnknown_030014A0

	thumb_func_start sub_802B990
sub_802B990: @ 0x0802B990
	push {r4, r5, lr}
	adds r4, r0, #0
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _0802B9DC
	movs r0, #0x24
	bl sub_8029BAC
	ldr r5, [r4, #0xc]
	cmp r5, #0
	beq _0802B9B8
	movs r2, #0
	str r2, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	b _0802B9DC
_0802B9B8:
	movs r0, #3
	bl sub_8000E1C
	lsls r0, r0, #0x10
	cmp r0, #0
	bne _0802B9CE
	movs r0, #1
	str r0, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	b _0802B9D4
_0802B9CE:
	str r5, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0]
_0802B9D4:
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
_0802B9DC:
	ldr r0, _0802BA48 @ =gUnknown_030014A3
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802BA42
	ldr r5, _0802BA4C @ =gUnknown_030007E0
	movs r0, #1
	ldrh r1, [r5, #2]
	ands r0, r1
	cmp r0, #0
	beq _0802BA1C
	movs r0, #4
	movs r1, #3
	str r0, [r4, #0x28]
	movs r2, #0
	str r2, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0x24]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	ldr r0, _0802BA50 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xd
	bl PlaySfx
	ldr r1, _0802BA54 @ =gUnknown_030014A4
	ldr r0, _0802BA58 @ =0xFFFFF880
	str r0, [r1]
_0802BA1C:
	ldr r0, [r5]
	movs r1, #2
	ands r0, r1
	cmp r0, #0
	beq _0802BA42
	movs r0, #2
	str r0, [r4, #0x28]
	movs r2, #0
	str r2, [r4, #0x44]
	str r0, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0x18]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	movs r0, #0x38
	bl sub_8029BAC
_0802BA42:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802BA48: .4byte gUnknown_030014A3
_0802BA4C: .4byte gUnknown_030007E0
_0802BA50: .4byte gUnknown_030012BC
_0802BA54: .4byte gUnknown_030014A4
_0802BA58: .4byte 0xFFFFF880

	thumb_func_start sub_802BA5C
sub_802BA5C: @ 0x0802BA5C
	push {lr}
	adds r3, r0, #0
	ldr r2, _0802BAC4 @ =gUnknown_030014A4
	ldr r0, [r3, #0x20]
	ldr r1, [r2]
	adds r0, r0, r1
	str r0, [r3, #0x20]
	adds r1, #0x60
	str r1, [r2]
	movs r0, #0xf0
	lsls r0, r0, #3
	cmp r1, r0
	ble _0802BA78
	str r0, [r2]
_0802BA78:
	ldr r0, [r3, #0x44]
	cmp r0, #0xa
	bgt _0802BA94
	ldr r0, _0802BAC8 @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	bne _0802BA94
	ldr r0, [r2]
	ldr r1, _0802BACC @ =0xFFFFFC00
	cmp r0, r1
	bge _0802BA94
	str r1, [r2]
_0802BA94:
	ldr r0, [r3, #0x20]
	movs r1, #0xa0
	lsls r1, r1, #6
	cmp r0, r1
	ble _0802BABE
	str r1, [r3, #0x20]
	movs r0, #1
	movs r1, #4
	str r0, [r3, #0x28]
	movs r2, #0
	str r2, [r3, #0x44]
	str r1, [r3, #0xc]
	ldr r0, [r3]
	ldrh r0, [r0, #0x30]
	movs r1, #0
	strh r0, [r3, #0x10]
	strb r1, [r3, #0x12]
	str r2, [r3, #8]
	movs r0, #0x24
	bl sub_8029BAC
_0802BABE:
	pop {r0}
	bx r0
	.align 2, 0
_0802BAC4: .4byte gUnknown_030014A4
_0802BAC8: .4byte gUnknown_030007E0
_0802BACC: .4byte 0xFFFFFC00

	thumb_func_start sub_802BAD0
sub_802BAD0: @ 0x0802BAD0
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r5, _0802BB3C @ =gUnknown_030007E0
	ldr r0, [r5]
	movs r1, #2
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r2, r0, #0x10
	cmp r2, #0
	bne _0802BAFE
	movs r0, #1
	str r0, [r4, #0x28]
	str r2, [r4, #0x44]
	str r2, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	movs r0, #0x24
	bl sub_8029BAC
_0802BAFE:
	movs r0, #1
	ldrh r5, [r5, #2]
	ands r0, r5
	cmp r0, #0
	beq _0802BB34
	movs r0, #4
	movs r1, #3
	str r0, [r4, #0x28]
	movs r2, #0
	str r2, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0x24]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	ldr r0, _0802BB40 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xd
	bl PlaySfx
	ldr r1, _0802BB44 @ =gUnknown_030014A4
	ldr r0, _0802BB48 @ =0xFFFFF880
	str r0, [r1]
_0802BB34:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802BB3C: .4byte gUnknown_030007E0
_0802BB40: .4byte gUnknown_030012BC
_0802BB44: .4byte gUnknown_030014A4
_0802BB48: .4byte 0xFFFFF880

	thumb_func_start sub_802BB4C
sub_802BB4C: @ 0x0802BB4C
	push {r4, lr}
	adds r4, r0, #0
	ldr r1, [r4, #0x44]
	cmp r1, #0x2c
	ble _0802BBA8
	ldr r0, _0802BB98 @ =gStaticData_0817A728
	ldr r1, _0802BB9C @ =0x05000200
	movs r2, #0x20
	movs r3, #0x10
	bl QueueVramDmaTransfer
	ldr r1, _0802BBA0 @ =gUnknown_03001480
	movs r0, #1
	strb r0, [r1]
	movs r0, #6
	movs r1, #5
	str r0, [r4, #0x28]
	movs r2, #0
	str r2, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0x3c]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	ldr r0, _0802BBA4 @ =gUnknown_030012C0
	ldr r1, [r0]
	adds r0, r1, #0
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802BBD4
	adds r0, r1, #0
	bl sub_8023234
	b _0802BBD4
	.align 2, 0
_0802BB98: .4byte gStaticData_0817A728
_0802BB9C: .4byte 0x05000200
_0802BBA0: .4byte gUnknown_03001480
_0802BBA4: .4byte gUnknown_030012C0
_0802BBA8:
	movs r0, #4
	ands r1, r0
	cmp r1, #0
	beq _0802BBC8
	ldr r0, _0802BBC0 @ =gStaticData_0817A728
	ldr r1, _0802BBC4 @ =0x05000200
	movs r2, #0x20
	movs r3, #0x10
	bl QueueVramDmaTransfer
	b _0802BBD4
	.align 2, 0
_0802BBC0: .4byte gStaticData_0817A728
_0802BBC4: .4byte 0x05000200
_0802BBC8:
	ldr r0, _0802BBDC @ =gStaticData_0817A748
	ldr r1, _0802BBE0 @ =0x05000200
	movs r2, #0x20
	movs r3, #0x10
	bl QueueVramDmaTransfer
_0802BBD4:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0802BBDC: .4byte gStaticData_0817A748
_0802BBE0: .4byte 0x05000200

	thumb_func_start sub_802BBE4
sub_802BBE4: @ 0x0802BBE4
	push {r4, lr}
	sub sp, #4
	adds r4, r0, #0
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _0802BC4A
	ldr r1, _0802BC54 @ =gUnknown_030014A4
	ldr r0, _0802BC58 @ =0xFFFFF980
	str r0, [r1]
	ldr r1, [r4, #0x20]
	movs r0, #0x80
	lsls r0, r0, #6
	cmp r1, r0
	ble _0802BC16
	ldr r1, [r4, #0x1c]
	movs r2, #0xa0
	lsls r2, r2, #6
	ldr r3, [r4, #0x24]
	movs r0, #0
	str r0, [sp]
	movs r0, #2
	bl sub_802AC28
	ldr r1, _0802BC5C @ =gUnknown_03001490
	str r0, [r1]
_0802BC16:
	movs r0, #8
	movs r1, #7
	str r0, [r4, #0x28]
	movs r2, #0
	str r2, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	adds r0, #0x54
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	ldr r1, _0802BC60 @ =gUnknown_03001480
	movs r0, #1
	strb r0, [r1]
	ldr r0, _0802BC64 @ =gUnknown_030012C0
	ldr r1, [r0]
	adds r0, r1, #0
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802BC4A
	adds r0, r1, #0
	bl sub_8023234
_0802BC4A:
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0802BC54: .4byte gUnknown_030014A4
_0802BC58: .4byte 0xFFFFF980
_0802BC5C: .4byte gUnknown_03001490
_0802BC60: .4byte gUnknown_03001480
_0802BC64: .4byte gUnknown_030012C0

	thumb_func_start sub_802BC68
sub_802BC68: @ 0x0802BC68
	push {r4, r5, lr}
	adds r1, r0, #0
	ldr r4, _0802BC90 @ =gUnknown_03001488
	ldr r2, [r4]
	cmp r2, #0
	beq _0802BD0E
	ldr r0, _0802BC94 @ =gUnknown_030014A0
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802BC9C
	ldr r5, _0802BC98 @ =gUnknown_030012C0
_0802BC7E:
	ldr r0, [r5]
	bl sub_8023430
	ldr r0, [r4]
	subs r0, #1
	str r0, [r4]
	cmp r0, #0
	bne _0802BC7E
	b _0802BD0E
	.align 2, 0
_0802BC90: .4byte gUnknown_03001488
_0802BC94: .4byte gUnknown_030014A0
_0802BC98: .4byte gUnknown_030012C0
_0802BC9C:
	ldr r3, _0802BCAC @ =gUnknown_03001484
	ldr r0, [r3]
	cmp r0, #0
	beq _0802BCB0
	subs r0, #1
	str r0, [r3]
	b _0802BD0E
	.align 2, 0
_0802BCAC: .4byte gUnknown_03001484
_0802BCB0:
	movs r0, #0xf
	str r0, [r3]
	cmp r2, #9
	bgt _0802BCC8
	ldr r0, [r1, #0x1c]
	ldr r1, [r1, #0x20]
	movs r2, #1
	bl sub_802B174
	ldr r0, [r4]
	subs r0, #1
	b _0802BCFE
_0802BCC8:
	cmp r2, #0x13
	bgt _0802BCDC
	ldr r0, [r1, #0x1c]
	ldr r1, [r1, #0x20]
	movs r2, #2
	bl sub_802B174
	ldr r0, [r4]
	subs r0, #2
	b _0802BCFE
_0802BCDC:
	cmp r2, #0x27
	bgt _0802BCF0
	ldr r0, [r1, #0x1c]
	ldr r1, [r1, #0x20]
	movs r2, #4
	bl sub_802B174
	ldr r0, [r4]
	subs r0, #4
	b _0802BCFE
_0802BCF0:
	ldr r0, [r1, #0x1c]
	ldr r1, [r1, #0x20]
	movs r2, #8
	bl sub_802B174
	ldr r0, [r4]
	subs r0, #8
_0802BCFE:
	str r0, [r4]
	ldr r0, _0802BD14 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #8
	bl PlaySfx
_0802BD0E:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802BD14: .4byte gUnknown_030012BC

	thumb_func_start sub_802BD18
sub_802BD18: @ 0x0802BD18
	ldr r0, _0802BD20 @ =gUnknown_03001480
	ldrb r0, [r0]
	bx lr
	.align 2, 0
_0802BD20: .4byte gUnknown_03001480

	thumb_func_start sub_802BD24
sub_802BD24: @ 0x0802BD24
	push {lr}
	adds r3, r0, #0
	ldr r0, [r3, #0x44]
	cmp r0, #0x13
	ble _0802BD56
	ldr r1, _0802BD5C @ =gUnknown_030014A3
	movs r0, #1
	strb r0, [r1]
	ldr r1, _0802BD60 @ =gUnknown_030014A0
	movs r0, #0
	strb r0, [r1]
	movs r0, #1
	movs r2, #0
	str r0, [r3, #0x28]
	str r2, [r3, #0x44]
	str r2, [r3, #0xc]
	ldr r0, [r3]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r3, #0x10]
	strb r1, [r3, #0x12]
	str r2, [r3, #8]
	movs r0, #0x24
	bl sub_8029BAC
_0802BD56:
	pop {r0}
	bx r0
	.align 2, 0
_0802BD5C: .4byte gUnknown_030014A3
_0802BD60: .4byte gUnknown_030014A0

	thumb_func_start sub_802BD64
sub_802BD64: @ 0x0802BD64
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r2, _0802BDBC @ =gUnknown_030014A4
	ldr r0, [r4, #0x20]
	ldr r1, [r2]
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r1, #0x2d
	str r1, [r2]
	ldr r0, [r4, #0x24]
	adds r0, #0x3c
	str r0, [r4, #0x24]
	bl sub_8029B2C
	lsls r0, r0, #8
	ldr r1, [r4, #0x24]
	subs r1, r0, r1
	str r1, [r4, #0x34]
	ldr r5, _0802BDC0 @ =gUnknown_030014A2
	ldrb r0, [r5]
	cmp r0, #0
	bne _0802BDA8
	ldr r0, _0802BDC4 @ =0x000016FF
	cmp r1, r0
	bgt _0802BDA8
	movs r0, #0
	movs r1, #2
	movs r2, #1
	bl sub_800132C
	ldr r1, _0802BDC8 @ =gUnknown_03001480
	movs r0, #1
	strb r0, [r1]
	strb r0, [r5]
_0802BDA8:
	ldr r1, [r4, #0x34]
	ldr r0, _0802BDCC @ =0x000003FF
	cmp r1, r0
	bgt _0802BDB6
	movs r0, #1
	bl sub_802A668
_0802BDB6:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802BDBC: .4byte gUnknown_030014A4
_0802BDC0: .4byte gUnknown_030014A2
_0802BDC4: .4byte 0x000016FF
_0802BDC8: .4byte gUnknown_03001480
_0802BDCC: .4byte 0x000003FF

	thumb_func_start sub_802BDD0
sub_802BDD0: @ 0x0802BDD0
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r2, _0802BE24 @ =gUnknown_030014A4
	ldr r0, [r4, #0x20]
	ldr r1, [r2]
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r1, #0x2d
	str r1, [r2]
	ldr r0, [r4, #0x24]
	adds r0, #0x3c
	str r0, [r4, #0x24]
	bl sub_8029B2C
	lsls r0, r0, #8
	ldr r1, [r4, #0x24]
	subs r1, r0, r1
	str r1, [r4, #0x34]
	ldr r5, _0802BE28 @ =gUnknown_030014A2
	ldrb r0, [r5]
	cmp r0, #0
	bne _0802BE10
	ldr r0, _0802BE2C @ =0x000016FF
	cmp r1, r0
	bgt _0802BE10
	movs r0, #0
	movs r1, #2
	movs r2, #1
	bl sub_800132C
	movs r0, #1
	strb r0, [r5]
_0802BE10:
	ldr r1, [r4, #0x34]
	ldr r0, _0802BE30 @ =0x000003FF
	cmp r1, r0
	bgt _0802BE1E
	movs r0, #2
	bl sub_802A668
_0802BE1E:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802BE24: .4byte gUnknown_030014A4
_0802BE28: .4byte gUnknown_030014A2
_0802BE2C: .4byte 0x000016FF
_0802BE30: .4byte 0x000003FF

	thumb_func_start sub_802BE34
sub_802BE34: @ 0x0802BE34
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x20]
	ldr r2, _0802BE70 @ =0xFFFFFF00
	adds r1, r0, r2
	str r1, [r4, #0x20]
	ldr r5, _0802BE74 @ =gUnknown_030014A2
	ldrb r0, [r5]
	cmp r0, #0
	bne _0802BE5C
	ldr r0, _0802BE78 @ =0xFFFFC000
	cmp r1, r0
	bge _0802BE5C
	movs r0, #0
	movs r1, #2
	movs r2, #1
	bl sub_800132C
	movs r0, #1
	strb r0, [r5]
_0802BE5C:
	ldr r1, [r4, #0x20]
	ldr r0, _0802BE7C @ =0xFFFF8E00
	cmp r1, r0
	bge _0802BE6A
	movs r0, #3
	bl sub_802A668
_0802BE6A:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802BE70: .4byte 0xFFFFFF00
_0802BE74: .4byte gUnknown_030014A2
_0802BE78: .4byte 0xFFFFC000
_0802BE7C: .4byte 0xFFFF8E00

	thumb_func_start sub_802BE80
sub_802BE80: @ 0x0802BE80
	push {lr}
	adds r2, r0, #0
	ldr r0, [r2, #0x44]
	cmp r0, #0x1e
	bne _0802BED2
	ldr r1, _0802BEBC @ =gUnknown_030014A3
	movs r0, #1
	strb r0, [r1]
	ldr r0, _0802BEC0 @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #2
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	cmp r3, #0
	bne _0802BEC4
	movs r0, #1
	str r0, [r2, #0x28]
	str r3, [r2, #0x44]
	str r3, [r2, #0xc]
	ldr r0, [r2]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r2, #0x10]
	strb r1, [r2, #0x12]
	str r3, [r2, #8]
	movs r0, #0x24
	bl sub_8029BAC
	b _0802BED2
	.align 2, 0
_0802BEBC: .4byte gUnknown_030014A3
_0802BEC0: .4byte gUnknown_030007E0
_0802BEC4:
	movs r0, #2
	str r0, [r2, #0x28]
	movs r0, #0
	str r0, [r2, #0x44]
	movs r0, #0x38
	bl sub_8029BAC
_0802BED2:
	pop {r0}
	bx r0
	.align 2, 0

