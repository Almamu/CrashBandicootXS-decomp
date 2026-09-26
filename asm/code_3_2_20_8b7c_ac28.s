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

