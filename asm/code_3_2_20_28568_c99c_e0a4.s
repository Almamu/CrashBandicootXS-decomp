.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_802E0A4
sub_802E0A4: @ 0x0802E0A4
	ldr r0, _0802E0C8 @ =gUnknown_030014BC
	ldr r3, [r0]
	ldr r0, [r3, #0xc]
	cmp r0, #3
	beq _0802E0C6
	ldrb r0, [r3, #0x12]
	cmp r0, #0
	beq _0802E0C6
	movs r0, #3
	str r0, [r3, #0xc]
	ldr r0, [r3]
	ldrh r0, [r0, #0x24]
	movs r1, #0
	movs r2, #0
	strh r0, [r3, #0x10]
	strb r1, [r3, #0x12]
	str r2, [r3, #8]
_0802E0C6:
	bx lr
	.align 2, 0
_0802E0C8: .4byte gUnknown_030014BC

	thumb_func_start sub_802E0CC
sub_802E0CC: @ 0x0802E0CC
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r6, r2, #0
	lsls r1, r1, #0x18
	lsrs r2, r1, #0x18
	ldrb r4, [r5]
	ldr r1, _0802E0F0 @ =gUnknown_030012C0
	ldr r0, [r1]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802E0F4
	ldrb r4, [r5, #1]
	cmp r4, #0x17
	bne _0802E0FA
	movs r4, #0x14
	b _0802E0FA
	.align 2, 0
_0802E0F0: .4byte gUnknown_030012C0
_0802E0F4:
	cmp r2, #0
	beq _0802E0FA
	ldrb r4, [r5, #2]
_0802E0FA:
	cmp r4, #0x1d
	bne _0802E10A
	ldr r0, [r1]
	bl sub_8023418
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802E166
_0802E10A:
	cmp r4, #0
	beq _0802E166
	cmp r4, #0x3e
	beq _0802E166
	adds r0, r4, #0
	subs r0, #0x20
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #5
	bls _0802E166
	ldr r0, [r5, #4]
	lsls r2, r0, #8
	ldr r0, [r5, #8]
	lsls r3, r0, #8
	ldr r0, [r5, #0xc]
	lsls r0, r0, #8
	adds r6, r0, r6
	adds r1, r4, #0
	subs r1, #0x10
	lsls r0, r1, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #2
	bhi _0802E146
	adds r0, r1, #0
	adds r1, r2, #0
	adds r2, r3, #0
	adds r3, r6, #0
	bl sub_8031040
	b _0802E166
_0802E146:
	cmp r4, #0xa
	beq _0802E15A
	str r5, [sp]
	adds r0, r4, #0
	adds r1, r2, #0
	adds r2, r3, #0
	adds r3, r6, #0
	bl sub_802E170
	b _0802E168
_0802E15A:
	movs r0, #0
	adds r1, r2, #0
	adds r2, r3, #0
	adds r3, r6, #0
	bl sub_8033264
_0802E166:
	movs r0, #0
_0802E168:
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1

	thumb_func_start sub_802E170
sub_802E170: @ 0x0802E170
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	adds r5, r1, #0
	mov r8, r2
	adds r7, r3, #0
	ldr r4, [sp, #0x24]
	lsls r0, r0, #0x18
	lsrs r6, r0, #0x18
	ldr r0, _0802E1AC @ =gUnknown_030014D8
	ldr r1, [r0]
	lsls r0, r6, #2
	adds r0, r0, r6
	lsls r0, r0, #3
	adds r0, r0, r1
	ldr r1, [r0, #0x20]
	adds r5, r5, r1
	ldr r0, [r0, #0x24]
	add r8, r0
	subs r0, r6, #1
	cmp r0, #0x1e
	bls _0802E1A2
	b _0802E3BC
_0802E1A2:
	lsls r0, r0, #2
	ldr r1, _0802E1B0 @ =_0802E1B4
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0802E1AC: .4byte gUnknown_030014D8
_0802E1B0: .4byte _0802E1B4
_0802E1B4: @ jump table
	.4byte _0802E230 @ case 0
	.4byte _0802E3BC @ case 1
	.4byte _0802E3BC @ case 2
	.4byte _0802E258 @ case 3
	.4byte _0802E258 @ case 4
	.4byte _0802E258 @ case 5
	.4byte _0802E258 @ case 6
	.4byte _0802E258 @ case 7
	.4byte _0802E258 @ case 8
	.4byte _0802E3BC @ case 9
	.4byte _0802E3BC @ case 10
	.4byte _0802E3BC @ case 11
	.4byte _0802E3BC @ case 12
	.4byte _0802E3BC @ case 13
	.4byte _0802E3BC @ case 14
	.4byte _0802E3BC @ case 15
	.4byte _0802E3BC @ case 16
	.4byte _0802E3BC @ case 17
	.4byte _0802E280 @ case 18
	.4byte _0802E2CC @ case 19
	.4byte _0802E2CC @ case 20
	.4byte _0802E2CC @ case 21
	.4byte _0802E2A8 @ case 22
	.4byte _0802E2F4 @ case 23
	.4byte _0802E2F4 @ case 24
	.4byte _0802E2F4 @ case 25
	.4byte _0802E31C @ case 26
	.4byte _0802E344 @ case 27
	.4byte _0802E2F4 @ case 28
	.4byte _0802E3BC @ case 29
	.4byte _0802E36C @ case 30
_0802E230:
	movs r0, #0x80
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E254 @ =gUnknown_030014D8
	lsls r2, r6, #2
	adds r2, r2, r6
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	str r7, [sp]
	str r4, [sp, #4]
	adds r2, r5, #0
	mov r3, r8
	bl sub_802FD8C
	b _0802E3BE
	.align 2, 0
_0802E254: .4byte gUnknown_030014D8
_0802E258:
	movs r0, #0x64
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E27C @ =gUnknown_030014D8
	lsls r2, r6, #2
	adds r2, r2, r6
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	str r7, [sp]
	adds r2, r5, #0
	mov r3, r8
	bl sub_802FF08
	b _0802E3BE
	.align 2, 0
_0802E27C: .4byte gUnknown_030014D8
_0802E280:
	movs r0, #0x70
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E2A4 @ =gUnknown_030014D8
	lsls r2, r6, #2
	adds r2, r2, r6
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	str r7, [sp]
	adds r2, r5, #0
	mov r3, r8
	bl sub_8032054
	b _0802E3BE
	.align 2, 0
_0802E2A4: .4byte gUnknown_030014D8
_0802E2A8:
	adds r0, r4, #0
	bl sub_802AA80
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802E2CC
	movs r0, #0x74
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E2C8 @ =gUnknown_030014D8
	ldr r1, [r1]
	movs r2, #0xc8
	lsls r2, r2, #2
	b _0802E2E0
	.align 2, 0
_0802E2C8: .4byte gUnknown_030014D8
_0802E2CC:
	movs r0, #0x74
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E2F0 @ =gUnknown_030014D8
	lsls r2, r6, #2
	adds r2, r2, r6
	lsls r2, r2, #3
	ldr r1, [r1]
_0802E2E0:
	adds r1, r1, r2
	str r7, [sp]
	str r4, [sp, #4]
	adds r2, r5, #0
	mov r3, r8
	bl sub_80320C4
	b _0802E3BE
	.align 2, 0
_0802E2F0: .4byte gUnknown_030014D8
_0802E2F4:
	movs r0, #0x70
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E318 @ =gUnknown_030014D8
	lsls r2, r6, #2
	adds r2, r2, r6
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	str r7, [sp]
	adds r2, r5, #0
	mov r3, r8
	bl sub_8031F78
	b _0802E3BE
	.align 2, 0
_0802E318: .4byte gUnknown_030014D8
_0802E31C:
	movs r0, #0x60
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E340 @ =gUnknown_030014D8
	lsls r2, r6, #2
	adds r2, r2, r6
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	str r7, [sp]
	adds r2, r5, #0
	mov r3, r8
	bl sub_8032440
	b _0802E3BE
	.align 2, 0
_0802E340: .4byte gUnknown_030014D8
_0802E344:
	movs r0, #0x68
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E368 @ =gUnknown_030014D8
	lsls r2, r6, #2
	adds r2, r2, r6
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	str r7, [sp]
	adds r2, r5, #0
	mov r3, r8
	bl sub_80325EC
	b _0802E3BE
	.align 2, 0
_0802E368: .4byte gUnknown_030014D8
_0802E36C:
	movs r0, #0x5c
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r3, _0802E3B8 @ =gUnknown_030014D8
	mov sb, r3
	ldr r2, [r3]
	movs r3, #0xd7
	lsls r3, r3, #3
	adds r1, r2, r3
	lsls r4, r6, #2
	adds r4, r4, r6
	lsls r4, r4, #3
	adds r2, r4, r2
	ldr r2, [r2, #0x20]
	subs r2, r5, r2
	ldr r3, [r1, #0x20]
	adds r2, r2, r3
	str r7, [sp]
	mov r3, r8
	bl sub_80326E4
	movs r0, #0x5c
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	mov r2, sb
	ldr r1, [r2]
	adds r1, r1, r4
	str r7, [sp]
	adds r2, r5, #0
	mov r3, r8
	bl sub_80326E4
	b _0802E3BE
	.align 2, 0
_0802E3B8: .4byte gUnknown_030014D8
_0802E3BC:
	movs r0, #0
_0802E3BE:
	add sp, #8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	thumb_func_start sub_802E3CC
sub_802E3CC: @ 0x0802E3CC
	push {r4, r5, lr}
	sub sp, #4
	ldr r0, _0802E414 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x17
	bl PlaySfx
	movs r0, #0x58
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	ldr r0, _0802E418 @ =gUnknown_030014D8
	ldr r1, [r0]
	movs r0, #0xe6
	lsls r0, r0, #3
	adds r1, r1, r0
	movs r0, #0
	movs r5, #1
	str r0, [sp]
	adds r0, r4, #0
	movs r2, #0
	movs r3, #0
	bl InitActorPart
	str r5, [r4, #0x54]
	ldr r0, _0802E41C @ =gStaticData_087E50D4
	str r0, [r4, #0x50]
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802E414: .4byte gUnknown_030012BC
_0802E418: .4byte gUnknown_030014D8
_0802E41C: .4byte gStaticData_087E50D4

	thumb_func_start sub_802E420
sub_802E420: @ 0x0802E420
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	sub sp, #4
	mov r8, r0
	mov sb, r1
	adds r6, r2, #0
	ldr r0, _0802E478 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #4
	bl PlaySfx
	movs r0, #0x58
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	adds r4, r0, #0
	ldr r0, _0802E47C @ =gUnknown_030014D8
	ldr r1, [r0]
	movs r0, #0xe1
	lsls r0, r0, #3
	adds r1, r1, r0
	movs r5, #1
	str r6, [sp]
	adds r0, r4, #0
	mov r2, r8
	mov r3, sb
	bl InitActorPart
	str r5, [r4, #0x54]
	ldr r0, _0802E480 @ =gStaticData_087E510C
	str r0, [r4, #0x50]
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802E478: .4byte gUnknown_030012BC
_0802E47C: .4byte gUnknown_030014D8
_0802E480: .4byte gStaticData_087E510C

	thumb_func_start sub_802E484
sub_802E484: @ 0x0802E484
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r6, r1, #0
	adds r4, r2, #0
	movs r0, #0x64
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E4B4 @ =gUnknown_030014D8
	ldr r1, [r1]
	movs r2, #0xdc
	lsls r2, r2, #3
	adds r1, r1, r2
	str r4, [sp]
	adds r2, r5, #0
	adds r3, r6, #0
	bl sub_8032890
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802E4B4: .4byte gUnknown_030014D8

	thumb_func_start sub_802E4B8
sub_802E4B8: @ 0x0802E4B8
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	sub sp, #8
	adds r4, r0, #0
	mov r8, r1
	mov sb, r2
	adds r5, r3, #0
	ldr r6, [sp, #0x20]
	lsls r4, r4, #0x18
	lsrs r4, r4, #0x18
	movs r0, #0x64
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E500 @ =gUnknown_030014D8
	lsls r2, r4, #2
	adds r2, r2, r4
	lsls r2, r2, #3
	ldr r1, [r1]
	adds r1, r1, r2
	str r5, [sp]
	str r6, [sp, #4]
	mov r2, r8
	mov r3, sb
	bl sub_8031920
	add sp, #8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0802E500: .4byte gUnknown_030014D8

	thumb_func_start sub_802E504
sub_802E504: @ 0x0802E504
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r6, r1, #0
	adds r4, r2, #0
	movs r0, #0x5c
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E534 @ =gUnknown_030014D8
	ldr r1, [r1]
	movs r2, #0x8c
	lsls r2, r2, #2
	adds r1, r1, r2
	str r4, [sp]
	adds r2, r5, #0
	adds r3, r6, #0
	bl sub_80342D4
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802E534: .4byte gUnknown_030014D8

	thumb_func_start sub_802E538
sub_802E538: @ 0x0802E538
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	sub sp, #8
	adds r6, r0, #0
	mov r8, r1
	adds r5, r2, #0
	lsls r4, r3, #0x18
	lsrs r4, r4, #0x18
	movs r0, #0x70
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E578 @ =gUnknown_030014D8
	ldr r1, [r1]
	movs r2, #0x82
	lsls r2, r2, #2
	adds r1, r1, r2
	str r5, [sp]
	add r2, sp, #4
	strb r4, [r2]
	adds r2, r6, #0
	mov r3, r8
	bl sub_8034058
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802E578: .4byte gUnknown_030014D8

	thumb_func_start sub_802E57C
sub_802E57C: @ 0x0802E57C
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r6, r1, #0
	adds r4, r2, #0
	movs r0, #0x70
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E5AC @ =gUnknown_030014D8
	ldr r1, [r1]
	movs r2, #0xf0
	lsls r2, r2, #1
	adds r1, r1, r2
	str r4, [sp]
	adds r2, r5, #0
	adds r3, r6, #0
	bl sub_8033EF4
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802E5AC: .4byte gUnknown_030014D8

	thumb_func_start sub_802E5B0
sub_802E5B0: @ 0x0802E5B0
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r6, r1, #0
	adds r4, r2, #0
	movs r0, #0x70
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E5E0 @ =gUnknown_030014D8
	ldr r1, [r1]
	movs r2, #0xdc
	lsls r2, r2, #1
	adds r1, r1, r2
	str r4, [sp]
	adds r2, r5, #0
	adds r3, r6, #0
	bl sub_8033BB8
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802E5E0: .4byte gUnknown_030014D8

	thumb_func_start sub_802E5E4
sub_802E5E4: @ 0x0802E5E4
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r6, r1, #0
	adds r4, r2, #0
	ldr r0, _0802E624 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x38
	bl PlaySfx
	movs r0, #0x6c
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E628 @ =gUnknown_030014D8
	ldr r1, [r1]
	movs r2, #0xc3
	lsls r2, r2, #3
	adds r1, r1, r2
	str r4, [sp]
	adds r2, r5, #0
	adds r3, r6, #0
	bl sub_80329D4
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802E624: .4byte gUnknown_030012BC
_0802E628: .4byte gUnknown_030014D8

	thumb_func_start sub_802E62C
sub_802E62C: @ 0x0802E62C
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r6, r1, #0
	adds r4, r2, #0
	ldr r0, _0802E66C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x38
	bl PlaySfx
	movs r0, #0x6c
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E670 @ =gUnknown_030014D8
	ldr r1, [r1]
	movs r2, #0xbe
	lsls r2, r2, #3
	adds r1, r1, r2
	str r4, [sp]
	adds r2, r5, #0
	adds r3, r6, #0
	bl sub_80305F8
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802E66C: .4byte gUnknown_030012BC
_0802E670: .4byte gUnknown_030014D8

	thumb_func_start sub_802E674
sub_802E674: @ 0x0802E674
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	sub sp, #0xc
	mov r8, r0
	mov sb, r1
	adds r4, r2, #0
	adds r5, r3, #0
	ldr r6, [sp, #0x24]
	ldr r0, _0802E6C4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x30
	bl PlaySfx
	movs r0, #0x60
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E6C8 @ =gUnknown_030014D8
	ldr r1, [r1]
	adds r1, #0x78
	str r4, [sp]
	str r5, [sp, #4]
	str r6, [sp, #8]
	mov r2, r8
	mov r3, sb
	bl sub_8030300
	add sp, #0xc
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802E6C4: .4byte gUnknown_030012BC
_0802E6C8: .4byte gUnknown_030014D8

	thumb_func_start sub_802E6CC
sub_802E6CC: @ 0x0802E6CC
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	sub sp, #0xc
	mov r8, r0
	mov sb, r1
	adds r4, r2, #0
	adds r5, r3, #0
	ldr r6, [sp, #0x24]
	movs r0, #0x60
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, _0802E70C @ =gUnknown_030014D8
	ldr r1, [r1]
	adds r1, #0x50
	str r4, [sp]
	str r5, [sp, #4]
	str r6, [sp, #8]
	mov r2, r8
	mov r3, sb
	bl sub_802FA04
	add sp, #0xc
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802E70C: .4byte gUnknown_030014D8

	thumb_func_start sub_802E710
sub_802E710: @ 0x0802E710
	push {r4, r5, r6, lr}
	adds r5, r1, #0
	ldr r4, _0802E738 @ =gUnknown_030014D8
	str r0, [r4]
	ldr r6, _0802E73C @ =gUnknown_03000884
	movs r0, #0x58
	movs r1, #0x80
	lsls r1, r1, #0x18
	bl mem_alloc
	ldr r1, [r4]
	adds r2, r5, #0
	bl sub_802E740
	str r0, [r6]
	str r0, [r0, #0x48]
	str r0, [r0, #0x4c]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802E738: .4byte gUnknown_030014D8
_0802E73C: .4byte gUnknown_03000884

	thumb_func_start sub_802E740
sub_802E740: @ 0x0802E740
	push {r4, r5, lr}
	sub sp, #4
	adds r5, r0, #0
	movs r3, #0
	cmp r2, #0
	bne _0802E74E
	ldr r3, _0802E794 @ =0xFFFF6A00
_0802E74E:
	movs r4, #0x64
	str r2, [sp]
	adds r0, r5, #0
	movs r2, #0
	bl InitActorPart
	str r4, [r5, #0x54]
	ldr r0, _0802E798 @ =gStaticData_087E5144
	str r0, [r5, #0x50]
	adds r0, r5, #0
	bl sub_802F338
	ldr r0, _0802E79C @ =gUnknown_0300150C
	movs r2, #0
	str r2, [r0]
	ldr r0, [r5, #0x24]
	cmp r0, #0
	beq _0802E7A4
	ldr r0, _0802E7A0 @ =gUnknown_03001508
	str r2, [r0]
	movs r0, #7
	str r0, [r5, #0x28]
	str r2, [r5, #0x44]
	str r2, [r5, #0xc]
	ldr r0, [r5]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r5, #0x10]
	strb r1, [r5, #0x12]
	str r2, [r5, #8]
	movs r0, #0x28
	bl sub_8029BAC
	b _0802E7B2
	.align 2, 0
_0802E794: .4byte 0xFFFF6A00
_0802E798: .4byte gStaticData_087E5144
_0802E79C: .4byte gUnknown_0300150C
_0802E7A0: .4byte gUnknown_03001508
_0802E7A4:
	movs r0, #0x1e
	bl sub_8029BAC
	ldr r1, _0802E810 @ =gUnknown_03001508
	movs r0, #0xc0
	lsls r0, r0, #1
	str r0, [r1]
_0802E7B2:
	ldr r1, _0802E814 @ =gUnknown_03001507
	movs r0, #0
	strb r0, [r1]
	ldr r1, _0802E818 @ =gUnknown_03001506
	movs r0, #1
	strb r0, [r1]
	ldr r0, _0802E81C @ =gUnknown_03001500
	movs r4, #0
	str r4, [r0]
	ldr r0, _0802E820 @ =gUnknown_03001505
	strb r4, [r0]
	ldr r0, _0802E824 @ =gUnknown_03001504
	strb r4, [r0]
	ldr r0, _0802E828 @ =gUnknown_030014FC
	str r4, [r0]
	ldr r0, _0802E82C @ =gUnknown_030014F8
	str r4, [r0]
	ldr r0, _0802E830 @ =gUnknown_030014F4
	str r4, [r0]
	ldr r1, _0802E834 @ =gUnknown_030014EC
	movs r0, #0xbe
	rsbs r0, r0, #0
	str r0, [r1]
	ldr r0, _0802E838 @ =gUnknown_030014F0
	str r4, [r0]
	ldr r0, _0802E83C @ =gUnknown_030014E8
	strb r4, [r0]
	bl sub_8029794
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0802E7F6
	movs r0, #0x78
	str r0, [r5, #0x54]
_0802E7F6:
	ldr r1, _0802E840 @ =gUnknown_030014E4
	ldr r0, [r5, #0x54]
	str r0, [r1]
	ldr r0, _0802E844 @ =gUnknown_030014E0
	str r4, [r0]
	ldr r0, _0802E848 @ =gUnknown_030014DC
	str r4, [r0]
	adds r0, r5, #0
	add sp, #4
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0802E810: .4byte gUnknown_03001508
_0802E814: .4byte gUnknown_03001507
_0802E818: .4byte gUnknown_03001506
_0802E81C: .4byte gUnknown_03001500
_0802E820: .4byte gUnknown_03001505
_0802E824: .4byte gUnknown_03001504
_0802E828: .4byte gUnknown_030014FC
_0802E82C: .4byte gUnknown_030014F8
_0802E830: .4byte gUnknown_030014F4
_0802E834: .4byte gUnknown_030014EC
_0802E838: .4byte gUnknown_030014F0
_0802E83C: .4byte gUnknown_030014E8
_0802E840: .4byte gUnknown_030014E4
_0802E844: .4byte gUnknown_030014E0
_0802E848: .4byte gUnknown_030014DC

	thumb_func_start sub_802E84C
sub_802E84C: @ 0x0802E84C
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r0, _0802E99C @ =gUnknown_030014E0
	ldr r2, [r0]
	cmp r2, #0
	beq _0802E88A
	ldr r3, _0802E9A0 @ =gUnknown_030014DC
	ldr r0, [r3]
	adds r1, r0, #0
	subs r0, #1
	str r0, [r3]
	cmp r1, #0
	bgt _0802E884
	movs r0, #0x16
	str r0, [r3]
	lsls r0, r2, #1
	adds r0, r0, r2
	lsls r2, r0, #4
	movs r0, #0x80
	lsls r0, r0, #1
	cmp r2, r0
	ble _0802E87A
	adds r2, r0, #0
_0802E87A:
	ldr r0, _0802E9A4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x37
	bl PlaySfx
_0802E884:
	ldr r1, _0802E99C @ =gUnknown_030014E0
	movs r0, #0
	str r0, [r1]
_0802E88A:
	ldr r1, _0802E9A8 @ =gUnknown_03001500
	ldr r0, [r1]
	cmp r0, #0
	beq _0802E896
	subs r0, #1
	str r0, [r1]
_0802E896:
	adds r0, r4, #0
	bl sub_802F4CC
	adds r0, r4, #0
	bl sub_802F3BC
	ldr r0, _0802E9AC @ =gUnknown_0300150C
	ldr r1, [r4, #0x1c]
	ldr r0, [r0]
	adds r2, r1, r0
	str r2, [r4, #0x1c]
	ldr r0, _0802E9B0 @ =gUnknown_03001508
	ldr r1, [r4, #0x20]
	ldr r0, [r0]
	adds r3, r1, r0
	str r3, [r4, #0x20]
	ldr r0, _0802E9B4 @ =gUnknown_03001506
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802E8EE
	adds r1, r2, #0
	ldr r0, _0802E9B8 @ =0xFFFF8000
	cmp r1, r0
	bge _0802E8C8
	adds r1, r0, #0
_0802E8C8:
	str r1, [r4, #0x1c]
	movs r0, #0x80
	lsls r0, r0, #8
	cmp r1, r0
	ble _0802E8D4
	adds r1, r0, #0
_0802E8D4:
	str r1, [r4, #0x1c]
	adds r1, r3, #0
	ldr r0, _0802E9BC @ =0xFFFFB500
	cmp r1, r0
	bge _0802E8E0
	adds r1, r0, #0
_0802E8E0:
	str r1, [r4, #0x20]
	movs r0, #0x96
	lsls r0, r0, #7
	cmp r1, r0
	ble _0802E8EC
	adds r1, r0, #0
_0802E8EC:
	str r1, [r4, #0x20]
_0802E8EE:
	ldr r0, _0802E9C0 @ =gUnknown_03001504
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802E908
	movs r0, #0xe0
	lsls r0, r0, #5
	str r0, [r4, #0x34]
	bl sub_8029B2C
	lsls r0, r0, #8
	ldr r1, [r4, #0x34]
	adds r0, r0, r1
	str r0, [r4, #0x24]
_0802E908:
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
	blt _0802E96E
	movs r7, #6
	ldrsh r0, [r1, r7]
	subs r0, r2, r0
	lsls r0, r0, #8
	ldr r1, [r4, #8]
	subs r1, r1, r0
	str r1, [r4, #8]
	movs r0, #1
	strb r0, [r4, #0x12]
_0802E96E:
	ldr r0, [r4, #0x1c]
	ldr r1, [r4, #0x20]
	bl sub_8029D8C
	ldr r3, _0802E9C4 @ =gStaticData_0817C1C0
	ldr r0, [r4, #0x28]
	lsls r1, r0, #3
	adds r0, r1, r3
	movs r7, #2
	ldrsh r2, [r0, r7]
	cmp r2, #0
	ble _0802E9C8
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
	b _0802E9CE
	.align 2, 0
_0802E99C: .4byte gUnknown_030014E0
_0802E9A0: .4byte gUnknown_030014DC
_0802E9A4: .4byte gUnknown_030012BC
_0802E9A8: .4byte gUnknown_03001500
_0802E9AC: .4byte gUnknown_0300150C
_0802E9B0: .4byte gUnknown_03001508
_0802E9B4: .4byte gUnknown_03001506
_0802E9B8: .4byte 0xFFFF8000
_0802E9BC: .4byte 0xFFFFB500
_0802E9C0: .4byte gUnknown_03001504
_0802E9C4: .4byte gStaticData_0817C1C0
_0802E9C8:
	adds r0, r3, #4
	adds r0, r1, r0
	ldr r3, [r0]
_0802E9CE:
	ldr r1, _0802E9E8 @ =gStaticData_0817C1C0
	ldr r0, [r4, #0x28]
	lsls r0, r0, #3
	adds r0, r0, r1
	movs r7, #0
	ldrsh r1, [r0, r7]
	cmp r2, #0
	ble _0802E9EC
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	adds r0, r0, r1
	b _0802E9EE
	.align 2, 0
_0802E9E8: .4byte gStaticData_0817C1C0
_0802E9EC:
	adds r0, r1, #0
_0802E9EE:
	adds r0, r4, r0
	bl sub_803AD84
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_802E9FC
sub_802E9FC: @ 0x0802E9FC
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
	bne _0802EA66
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
	b _0802EAB6
_0802EA66:
	lsls r0, r4, #8
	bl sub_803ADB4
	str r0, [sp]
	movs r0, #0xe0
	lsls r0, r0, #0x11
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
	bgt _0802EAB6
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
_0802EAB6:
	mov r1, sb
	subs r6, r6, r1
	mov r0, r8
	subs r5, r5, r0
	cmp r5, #0x9f
	bgt _0802EB4E
	lsls r0, r0, #1
	adds r0, r5, r0
	cmp r0, #0
	blt _0802EB4E
	cmp r6, #0xef
	bgt _0802EB4E
	lsls r0, r1, #1
	adds r0, r6, r0
	cmp r0, #0
	blt _0802EB4E
	ldr r1, [r7, #0xc]
	ldr r2, [r7]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrh r0, [r0, #8]
	lsls r4, r0, #0x10
	mov r0, sl
	bl sub_8029108
	movs r1, #0xff
	ands r5, r1
	ldr r1, _0802EB60 @ =0x000001FF
	ands r6, r1
	lsls r1, r6, #0x10
	orrs r5, r1
	orrs r5, r4
	orrs r5, r0
	ldr r1, [sp, #4]
	orrs r1, r5
	str r1, [sp, #4]
	ldr r4, _0802EB64 @ =gUnknown_03001514
	ldr r0, [r4]
	cmp sl, r0
	beq _0802EB2A
	ldr r2, _0802EB68 @ =gUnknown_03001510
	ldr r0, [r2]
	movs r1, #1
	eors r0, r1
	str r0, [r2]
	ldr r2, _0802EB6C @ =gUnknown_03000874
	ldr r1, _0802EB70 @ =gUnknown_03001518
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r2, [r2]
	mov r1, sl
	bl sub_803AD80
	mov r0, sl
	str r0, [r4]
_0802EB2A:
	ldr r1, _0802EB70 @ =gUnknown_03001518
	ldr r0, _0802EB68 @ =gUnknown_03001510
	ldr r0, [r0]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, _0802EB74 @ =0xF9FF0000
	adds r0, r0, r1
	lsrs r0, r0, #5
	ldr r1, [r7, #0x18]
	lsls r1, r1, #0xc
	orrs r1, r0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	ldr r0, [sp, #4]
	ldr r2, [sp]
	bl sub_8028DD8
_0802EB4E:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802EB60: .4byte 0x000001FF
_0802EB64: .4byte gUnknown_03001514
_0802EB68: .4byte gUnknown_03001510
_0802EB6C: .4byte gUnknown_03000874
_0802EB70: .4byte gUnknown_03001518
_0802EB74: .4byte 0xF9FF0000

	thumb_func_start sub_802EB78
sub_802EB78: @ 0x0802EB78
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x28]
	subs r0, #2
	cmp r0, #1
	bhi _0802EB8A
	ldr r0, [r4, #0x44]
	cmp r0, #0x10
	ble _0802EC5A
_0802EB8A:
	ldr r0, [r4, #0x54]
	subs r0, r0, r1
	str r0, [r4, #0x54]
	ldr r2, _0802EC10 @ =gUnknown_030014F4
	movs r1, #0x12
	str r1, [r2]
	cmp r0, #0
	bgt _0802EC4C
	movs r5, #0
	str r5, [r4, #0x54]
	ldr r0, _0802EC14 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x3a
	bl PlaySfx
	movs r0, #4
	movs r1, #3
	str r0, [r4, #0x28]
	str r5, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0x24]
	movs r6, #0
	strh r0, [r4, #0x10]
	strb r6, [r4, #0x12]
	str r5, [r4, #8]
	ldr r0, _0802EC18 @ =gUnknown_030012C0
	ldr r1, [r0]
	adds r0, r1, #0
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802EBD6
	adds r0, r1, #0
	bl sub_8023234
_0802EBD6:
	ldr r0, _0802EC1C @ =gUnknown_03001507
	strb r6, [r0]
	ldr r0, _0802EC20 @ =gUnknown_030014E8
	movs r1, #1
	strb r1, [r0]
	ldr r0, _0802EC24 @ =gUnknown_03001506
	strb r1, [r0]
	movs r0, #0x1e
	bl sub_8029BAC
	ldr r0, _0802EC28 @ =gUnknown_03001508
	str r5, [r0]
	ldr r3, _0802EC2C @ =gUnknown_0300150C
	ldr r2, [r3]
	asrs r1, r2, #0x1f
	adds r0, r2, #0
	eors r0, r1
	subs r0, r0, r1
	movs r1, #0x90
	lsls r1, r1, #2
	cmp r0, r1
	ble _0802EC34
	cmp r2, #0
	blt _0802EC30
	movs r0, #0
	cmp r2, #0
	beq _0802EC32
	adds r0, r1, #0
	b _0802EC32
	.align 2, 0
_0802EC10: .4byte gUnknown_030014F4
_0802EC14: .4byte gUnknown_030012BC
_0802EC18: .4byte gUnknown_030012C0
_0802EC1C: .4byte gUnknown_03001507
_0802EC20: .4byte gUnknown_030014E8
_0802EC24: .4byte gUnknown_03001506
_0802EC28: .4byte gUnknown_03001508
_0802EC2C: .4byte gUnknown_0300150C
_0802EC30:
	ldr r0, _0802EC44 @ =0xFFFFFDC0
_0802EC32:
	str r0, [r3]
_0802EC34:
	ldr r0, _0802EC48 @ =gUnknown_0300150C
	ldr r1, [r0]
	lsrs r2, r1, #0x1f
	adds r1, r1, r2
	asrs r1, r1, #1
	str r1, [r0]
	b _0802EC5A
	.align 2, 0
_0802EC44: .4byte 0xFFFFFDC0
_0802EC48: .4byte gUnknown_0300150C
_0802EC4C:
	ldr r0, _0802EC60 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x42
	bl PlaySfx
_0802EC5A:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802EC60: .4byte gUnknown_030012BC

	thumb_func_start sub_802EC64
sub_802EC64: @ 0x0802EC64
	ldr r0, _0802EC84 @ =gUnknown_03001507
	ldrb r1, [r0]
	adds r2, r0, #0
	cmp r1, #0
	beq _0802EC90
	ldr r0, _0802EC88 @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #0x40
	ands r0, r1
	cmp r0, #0
	beq _0802EC90
	ldr r1, _0802EC8C @ =gUnknown_03001508
	ldr r0, [r1]
	subs r0, #0x40
	b _0802ECA8
	.align 2, 0
_0802EC84: .4byte gUnknown_03001507
_0802EC88: .4byte gUnknown_030007E0
_0802EC8C: .4byte gUnknown_03001508
_0802EC90:
	ldrb r0, [r2]
	cmp r0, #0
	beq _0802ECB8
	ldr r0, _0802ECB0 @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #0x80
	ands r0, r1
	cmp r0, #0
	beq _0802ECB8
	ldr r1, _0802ECB4 @ =gUnknown_03001508
	ldr r0, [r1]
	adds r0, #0x40
_0802ECA8:
	str r0, [r1]
	adds r3, r1, #0
	b _0802ECE4
	.align 2, 0
_0802ECB0: .4byte gUnknown_030007E0
_0802ECB4: .4byte gUnknown_03001508
_0802ECB8:
	ldr r1, _0802ECCC @ =gUnknown_03001508
	ldr r0, [r1]
	adds r3, r1, #0
	cmp r0, #0
	blt _0802ECD0
	cmp r0, #0
	beq _0802ECD2
	subs r0, #0x40
	b _0802ECD2
	.align 2, 0
_0802ECCC: .4byte gUnknown_03001508
_0802ECD0:
	adds r0, #0x40
_0802ECD2:
	str r0, [r1]
	ldr r0, [r3]
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	cmp r0, #0x40
	bgt _0802ECE4
	movs r0, #0
	str r0, [r3]
_0802ECE4:
	ldr r2, [r3]
	asrs r1, r2, #0x1f
	adds r0, r2, #0
	eors r0, r1
	subs r0, r0, r1
	movs r1, #0x90
	lsls r1, r1, #2
	cmp r0, r1
	ble _0802ED0A
	adds r0, r3, #0
	cmp r2, #0
	blt _0802ED06
	movs r3, #0
	cmp r2, #0
	beq _0802ED08
	adds r3, r1, #0
	b _0802ED08
_0802ED06:
	ldr r3, _0802ED0C @ =0xFFFFFDC0
_0802ED08:
	str r3, [r0]
_0802ED0A:
	bx lr
	.align 2, 0
_0802ED0C: .4byte 0xFFFFFDC0

	thumb_func_start sub_802ED10
sub_802ED10: @ 0x0802ED10
	ldr r0, _0802ED30 @ =gUnknown_03001507
	ldrb r1, [r0]
	adds r2, r0, #0
	cmp r1, #0
	beq _0802ED3C
	ldr r0, _0802ED34 @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #0x20
	ands r0, r1
	cmp r0, #0
	beq _0802ED3C
	ldr r1, _0802ED38 @ =gUnknown_0300150C
	ldr r0, [r1]
	subs r0, #0x40
	b _0802ED54
	.align 2, 0
_0802ED30: .4byte gUnknown_03001507
_0802ED34: .4byte gUnknown_030007E0
_0802ED38: .4byte gUnknown_0300150C
_0802ED3C:
	ldrb r0, [r2]
	cmp r0, #0
	beq _0802ED64
	ldr r0, _0802ED5C @ =gUnknown_030007E0
	ldr r0, [r0]
	movs r1, #0x10
	ands r0, r1
	cmp r0, #0
	beq _0802ED64
	ldr r1, _0802ED60 @ =gUnknown_0300150C
	ldr r0, [r1]
	adds r0, #0x40
_0802ED54:
	str r0, [r1]
	adds r3, r1, #0
	b _0802ED90
	.align 2, 0
_0802ED5C: .4byte gUnknown_030007E0
_0802ED60: .4byte gUnknown_0300150C
_0802ED64:
	ldr r1, _0802ED78 @ =gUnknown_0300150C
	ldr r0, [r1]
	adds r3, r1, #0
	cmp r0, #0
	blt _0802ED7C
	cmp r0, #0
	beq _0802ED7E
	subs r0, #0x40
	b _0802ED7E
	.align 2, 0
_0802ED78: .4byte gUnknown_0300150C
_0802ED7C:
	adds r0, #0x40
_0802ED7E:
	str r0, [r1]
	ldr r0, [r3]
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	cmp r0, #0x40
	bgt _0802ED90
	movs r0, #0
	str r0, [r3]
_0802ED90:
	ldr r2, [r3]
	asrs r1, r2, #0x1f
	adds r0, r2, #0
	eors r0, r1
	subs r0, r0, r1
	movs r1, #0x90
	lsls r1, r1, #2
	cmp r0, r1
	ble _0802EDB6
	adds r0, r3, #0
	cmp r2, #0
	blt _0802EDB2
	movs r3, #0
	cmp r2, #0
	beq _0802EDB4
	adds r3, r1, #0
	b _0802EDB4
_0802EDB2:
	ldr r3, _0802EDB8 @ =0xFFFFFDC0
_0802EDB4:
	str r3, [r0]
_0802EDB6:
	bx lr
	.align 2, 0
_0802EDB8: .4byte 0xFFFFFDC0

	thumb_func_start sub_802EDBC
sub_802EDBC: @ 0x0802EDBC
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r0, #0
	bl sub_802EC64
	adds r0, r4, #0
	bl sub_802ED10
	ldr r0, _0802EE18 @ =gUnknown_03001507
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802EEB8
	ldr r0, _0802EE1C @ =gUnknown_030007E0
	ldr r2, [r0]
	movs r0, #0x80
	lsls r0, r0, #2
	adds r1, r0, #0
	adds r0, r2, #0
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r5, r0, #0x10
	cmp r5, #0
	beq _0802EE28
	ldr r1, _0802EE20 @ =gUnknown_030014F4
	movs r0, #0x12
	str r0, [r1]
	ldr r0, _0802EE24 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xa
	bl PlaySfx
	movs r0, #2
	movs r1, #1
	str r0, [r4, #0x28]
	movs r2, #0
	str r2, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	b _0802EEB8
	.align 2, 0
_0802EE18: .4byte gUnknown_03001507
_0802EE1C: .4byte gUnknown_030007E0
_0802EE20: .4byte gUnknown_030014F4
_0802EE24: .4byte gUnknown_030012BC
_0802EE28:
	movs r0, #0x80
	lsls r0, r0, #1
	adds r1, r0, #0
	adds r0, r2, #0
	ands r0, r1
	cmp r0, #0
	beq _0802EE6C
	ldr r1, _0802EE64 @ =gUnknown_030014F4
	movs r0, #0x12
	str r0, [r1]
	ldr r0, _0802EE68 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xa
	bl PlaySfx
	movs r0, #3
	movs r1, #2
	str r0, [r4, #0x28]
	str r5, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0x18]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
	b _0802EEB8
	.align 2, 0
_0802EE64: .4byte gUnknown_030014F4
_0802EE68: .4byte gUnknown_030012BC
_0802EE6C:
	ldr r1, _0802EEC0 @ =gUnknown_03001500
	ldr r0, [r1]
	cmp r0, #0
	bne _0802EEB8
	movs r3, #1
	ands r2, r3
	cmp r2, #0
	beq _0802EEB8
	movs r0, #0x12
	str r0, [r1]
	ldr r0, _0802EEC4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0xfa
	lsls r2, r2, #2
	mov r1, sp
	strb r3, [r1]
	movs r1, #0x24
	movs r3, #0xa0
	bl sub_80019F8
	ldr r0, [r4, #0x1c]
	movs r1, #0x90
	lsls r1, r1, #5
	adds r0, r0, r1
	ldr r1, [r4, #0x20]
	ldr r2, _0802EEC8 @ =0xFFFFE800
	adds r1, r1, r2
	ldr r2, [r4, #0x24]
	adds r2, #0xa
	ldr r4, _0802EECC @ =0x00000199
	adds r3, r0, #0
	muls r3, r4, r3
	asrs r3, r3, #0xc
	muls r4, r1, r4
	asrs r4, r4, #0xc
	str r4, [sp]
	bl sub_802E6CC
_0802EEB8:
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802EEC0: .4byte gUnknown_03001500
_0802EEC4: .4byte gUnknown_030012BC
_0802EEC8: .4byte 0xFFFFE800
_0802EECC: .4byte 0x00000199

	thumb_func_start sub_802EED0
sub_802EED0: @ 0x0802EED0
	push {r4, r5, lr}
	adds r4, r0, #0
	bl sub_802EC64
	ldr r0, [r4, #0x44]
	cmp r0, #5
	bgt _0802EF00
	ldr r1, _0802EEF4 @ =gUnknown_0300150C
	ldr r0, [r1]
	ldr r2, _0802EEF8 @ =0xFFFFFF00
	adds r0, r0, r2
	str r0, [r1]
	ldr r2, _0802EEFC @ =0xFFFFFB00
	cmp r0, r2
	bge _0802EF16
	str r2, [r1]
	b _0802EF16
	.align 2, 0
_0802EEF4: .4byte gUnknown_0300150C
_0802EEF8: .4byte 0xFFFFFF00
_0802EEFC: .4byte 0xFFFFFB00
_0802EF00:
	ldr r2, _0802EF68 @ =gUnknown_0300150C
	ldr r0, [r2]
	adds r0, #0x2d
	str r0, [r2]
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	cmp r0, #0x2d
	bgt _0802EF16
	movs r0, #0
	str r0, [r2]
_0802EF16:
	ldr r0, [r4, #0x44]
	cmp r0, #0x21
	ble _0802EFAE
	adds r0, r4, #0
	bl sub_802ED10
	ldr r0, _0802EF6C @ =gUnknown_030007E0
	ldr r2, [r0]
	movs r0, #0x80
	lsls r0, r0, #2
	adds r1, r0, #0
	adds r0, r2, #0
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r5, r0, #0x10
	cmp r5, #0
	beq _0802EF78
	ldr r1, _0802EF70 @ =gUnknown_030014F4
	movs r0, #0x12
	str r0, [r1]
	ldr r0, _0802EF74 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xa
	bl PlaySfx
	movs r0, #2
	movs r1, #1
	str r0, [r4, #0x28]
	movs r2, #0
	str r2, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	b _0802EFAE
	.align 2, 0
_0802EF68: .4byte gUnknown_0300150C
_0802EF6C: .4byte gUnknown_030007E0
_0802EF70: .4byte gUnknown_030014F4
_0802EF74: .4byte gUnknown_030012BC
_0802EF78:
	movs r1, #0x80
	lsls r1, r1, #1
	adds r0, r1, #0
	ands r2, r0
	cmp r2, #0
	beq _0802EFAE
	ldr r1, _0802EFD0 @ =gUnknown_030014F4
	movs r0, #0x12
	str r0, [r1]
	ldr r0, _0802EFD4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xa
	bl PlaySfx
	movs r0, #3
	movs r1, #2
	str r0, [r4, #0x28]
	str r5, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0x18]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
_0802EFAE:
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _0802EFCA
	movs r0, #1
	movs r2, #0
	str r0, [r4, #0x28]
	str r2, [r4, #0x44]
	str r2, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
_0802EFCA:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802EFD0: .4byte gUnknown_030014F4
_0802EFD4: .4byte gUnknown_030012BC

	thumb_func_start sub_802EFD8
sub_802EFD8: @ 0x0802EFD8
	push {r4, r5, lr}
	adds r4, r0, #0
	bl sub_802EC64
	ldr r0, [r4, #0x44]
	cmp r0, #5
	bgt _0802F004
	ldr r1, _0802F000 @ =gUnknown_0300150C
	ldr r0, [r1]
	movs r2, #0x80
	lsls r2, r2, #1
	adds r0, r0, r2
	str r0, [r1]
	movs r2, #0xa0
	lsls r2, r2, #3
	cmp r0, r2
	ble _0802F01A
	str r2, [r1]
	b _0802F01A
	.align 2, 0
_0802F000: .4byte gUnknown_0300150C
_0802F004:
	ldr r2, _0802F06C @ =gUnknown_0300150C
	ldr r0, [r2]
	subs r0, #0x2d
	str r0, [r2]
	asrs r1, r0, #0x1f
	eors r0, r1
	subs r0, r0, r1
	cmp r0, #0x2d
	bgt _0802F01A
	movs r0, #0
	str r0, [r2]
_0802F01A:
	ldr r0, [r4, #0x44]
	cmp r0, #0x21
	ble _0802F0B2
	adds r0, r4, #0
	bl sub_802ED10
	ldr r0, _0802F070 @ =gUnknown_030007E0
	ldr r2, [r0]
	movs r0, #0x80
	lsls r0, r0, #2
	adds r1, r0, #0
	adds r0, r2, #0
	ands r0, r1
	lsls r0, r0, #0x10
	lsrs r5, r0, #0x10
	cmp r5, #0
	beq _0802F07C
	ldr r1, _0802F074 @ =gUnknown_030014F4
	movs r0, #0x12
	str r0, [r1]
	ldr r0, _0802F078 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xa
	bl PlaySfx
	movs r0, #2
	movs r1, #1
	str r0, [r4, #0x28]
	movs r2, #0
	str r2, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0xc]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
	b _0802F0B2
	.align 2, 0
_0802F06C: .4byte gUnknown_0300150C
_0802F070: .4byte gUnknown_030007E0
_0802F074: .4byte gUnknown_030014F4
_0802F078: .4byte gUnknown_030012BC
_0802F07C:
	movs r1, #0x80
	lsls r1, r1, #1
	adds r0, r1, #0
	ands r2, r0
	cmp r2, #0
	beq _0802F0B2
	ldr r1, _0802F0D4 @ =gUnknown_030014F4
	movs r0, #0x12
	str r0, [r1]
	ldr r0, _0802F0D8 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xa
	bl PlaySfx
	movs r0, #3
	movs r1, #2
	str r0, [r4, #0x28]
	str r5, [r4, #0x44]
	str r1, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0, #0x18]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r5, [r4, #8]
_0802F0B2:
	ldrb r0, [r4, #0x12]
	cmp r0, #0
	beq _0802F0CE
	movs r0, #1
	movs r2, #0
	str r0, [r4, #0x28]
	str r2, [r4, #0x44]
	str r2, [r4, #0xc]
	ldr r0, [r4]
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r4, #0x10]
	strb r1, [r4, #0x12]
	str r2, [r4, #8]
_0802F0CE:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802F0D4: .4byte gUnknown_030014F4
_0802F0D8: .4byte gUnknown_030012BC

