.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8003D3C is reconstructed (semantics understood) but NOT YET
@ byte-matching - parked as C in src/graphics/settings_menu.c, guarded
@ by #if NON_MATCHING. See docs/matching.md for this chunk's write-up.
.if NON_MATCHING == 0
	thumb_func_start sub_8003D3C
sub_8003D3C: @ 0x08003D3C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	mov sl, r0
	adds r6, r1, #0
	ldr r7, _08003E38 @ =gUnknown_030012DC
	ldr r0, [r7]
	movs r1, #0
	bl sub_8028A30
	ldr r4, [r7]
	movs r0, #0x98
	lsls r0, r0, #1
	mov r8, r0
	adds r0, r4, r0
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x10
	movs r1, #0x10
	ldrsh r0, [r0, r1]
	adds r4, r4, r0
	adds r0, r6, #0
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	movs r1, #0xa0
	subs r1, r1, r0
	ldr r4, [r7]
	movs r2, #0x87
	mov sb, r2
	movs r3, #0x88
	lsls r3, r3, #1
	adds r0, r4, r3
	str r1, [r0]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r4, r1
	str r2, [r0]
	mov r2, r8
	adds r0, r4, r2
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r3, #0x20
	ldrsh r0, [r0, r3]
	adds r4, r4, r0
	adds r0, r6, #0
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	ldr r2, [r7]
	mov r4, sl
	ldr r0, [r4, #4]
	asrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	movs r1, #2
	cmp r0, #0
	beq _08003DC8
	movs r1, #1
_08003DC8:
	adds r0, r2, #0
	bl sub_8028A30
	mov r1, sl
	ldr r0, [r1, #0x10]
	cmp r0, #0
	bne _08003E40
	ldr r0, [r7]
	movs r2, #0xa8
	movs r3, #0x88
	lsls r3, r3, #1
	adds r1, r0, r3
	str r2, [r1]
	movs r4, #0x8a
	lsls r4, r4, #1
	adds r1, r0, r4
	mov r2, sb
	str r2, [r1]
	mov r3, r8
	adds r1, r0, r3
	ldr r2, [r1]
	movs r4, #0x20
	ldrsh r1, [r2, r4]
	adds r0, r0, r1
	ldr r1, _08003E3C @ =gStaticData_0816B138
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r4, [r7]
	movs r1, #0xb0
	movs r2, #0x88
	lsls r2, r2, #1
	adds r0, r4, r2
	str r1, [r0]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r0, r4, r3
	mov r1, sb
	str r1, [r0]
	mov r2, r8
	adds r0, r4, r2
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r3, #0x20
	ldrsh r0, [r0, r3]
	adds r4, r4, r0
	movs r0, #0x29
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	b _08003E9C
	.align 2, 0
_08003E38: .4byte gUnknown_030012DC
_08003E3C: .4byte gStaticData_0816B138
_08003E40:
	ldr r0, [r7]
	movs r2, #0xa8
	movs r5, #0x91
	movs r4, #0x88
	lsls r4, r4, #1
	adds r1, r0, r4
	str r2, [r1]
	adds r2, #0x6c
	adds r1, r0, r2
	str r5, [r1]
	mov r3, r8
	adds r1, r0, r3
	ldr r2, [r1]
	movs r4, #0x20
	ldrsh r1, [r2, r4]
	adds r0, r0, r1
	ldr r1, _08003EE4 @ =gStaticData_0816B138
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r4, [r7]
	movs r1, #0xb0
	movs r2, #0x88
	lsls r2, r2, #1
	adds r0, r4, r2
	str r1, [r0]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r0, r4, r3
	str r5, [r0]
	mov r1, r8
	adds r0, r4, r1
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r2, #0x20
	ldrsh r0, [r0, r2]
	adds r4, r4, r0
	movs r0, #0x2a
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
_08003E9C:
	ldr r4, _08003EE8 @ =gUnknown_030012DC
	ldr r0, [r4]
	movs r1, #0
	bl sub_8028A30
	mov r3, sl
	ldr r0, [r3, #0x10]
	cmp r0, #0
	bne _08003EEC
	ldr r3, [r4]
	movs r1, #0xb0
	movs r2, #0x91
	movs r4, #0x88
	lsls r4, r4, #1
	adds r0, r3, r4
	str r1, [r0]
	adds r1, #0x64
	adds r0, r3, r1
	str r2, [r0]
	adds r2, #0x9f
	adds r0, r3, r2
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r1, #0x20
	ldrsh r4, [r0, r1]
	adds r4, r3, r4
	movs r0, #0x2a
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	b _08003F20
	.align 2, 0
_08003EE4: .4byte gStaticData_0816B138
_08003EE8: .4byte gUnknown_030012DC
_08003EEC:
	ldr r3, [r4]
	movs r1, #0xb0
	movs r2, #0x87
	movs r4, #0x88
	lsls r4, r4, #1
	adds r0, r3, r4
	str r1, [r0]
	adds r1, #0x64
	adds r0, r3, r1
	str r2, [r0]
	adds r2, #0xa9
	adds r0, r3, r2
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r1, #0x20
	ldrsh r4, [r0, r1]
	adds r4, r3, r4
	movs r0, #0x29
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
_08003F20:
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
.endif

@ sub_8003F30's semantics aren't confidently understood yet (a
@ complex per-digit itoa-rendering routine across three parallel
@ object arrays) - left fully untouched rather than force a
@ low-confidence reconstruction. See the PR/issue for this chunk.
	thumb_func_start sub_8003F30
sub_8003F30: @ 0x08003F30
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x1c
	adds r7, r0, #0
	mov r8, r1
	mov sl, r2
	adds r4, r3, #0
	add r0, sp, #0x3c
	ldrb r0, [r0]
	str r0, [sp, #8]
	lsls r1, r4, #2
	adds r0, r1, r4
	lsls r0, r0, #2
	adds r0, #0x28
	adds r0, r7, r0
	str r0, [sp, #0xc]
	mov r5, r8
	adds r5, #0x2b
	mov r6, sl
	adds r6, #5
	adds r0, r7, #0
	adds r0, #0xa8
	adds r0, r0, r1
	ldr r0, [r0]
	lsls r1, r5, #8
	str r1, [r0]
	lsls r1, r6, #8
	str r1, [r0, #4]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	adds r5, #0xd
	mov r6, sl
	ldr r1, [sp, #0xc]
	ldr r0, [r1, #4]
	mov r1, sp
	movs r2, #0xa
	bl itoa
	ldr r2, [sp, #8]
	cmp r2, #0
	beq _08003FAC
	ldr r0, _08003FA8 @ =gUnknown_030012DC
	ldr r2, [r0]
	ldr r0, [r7, #4]
	asrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	movs r1, #2
	cmp r0, #0
	beq _08003FA0
	movs r1, #1
_08003FA0:
	adds r0, r2, #0
	bl sub_8028A30
	b _08003FB6
	.align 2, 0
_08003FA8: .4byte gUnknown_030012DC
_08003FAC:
	ldr r0, _08004040 @ =gUnknown_030012DC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8028A30
_08003FB6:
	ldr r3, _08004040 @ =gUnknown_030012DC
	mov sb, r3
	ldr r2, [r3]
	movs r1, #0x88
	lsls r1, r1, #1
	adds r0, r2, r1
	str r5, [r0]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r0, r2, r3
	str r6, [r0]
	adds r1, #0x20
	adds r0, r2, r1
	ldr r1, [r0]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0x24]
	mov r1, sp
	bl sub_803AD80
	mov r5, r8
	adds r5, #7
	mov r6, sl
	adds r6, #0x1e
	lsls r4, r4, #2
	adds r0, r7, #0
	adds r0, #0xd0
	adds r0, r0, r4
	ldr r0, [r0]
	lsls r1, r5, #8
	str r1, [r0]
	lsls r1, r6, #8
	str r1, [r0, #4]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	adds r5, #9
	subs r6, #7
	ldr r1, [sp, #0xc]
	ldr r0, [r1, #0x10]
	mov r1, sp
	movs r2, #0xa
	bl itoa
	str r4, [sp, #0x10]
	mov r2, sl
	adds r2, #0x1e
	str r2, [sp, #0x18]
	str r6, [sp, #0x14]
	ldr r3, [sp, #8]
	cmp r3, #0
	beq _08004044
	mov r0, sb
	ldr r2, [r0]
	ldr r0, [r7, #4]
	asrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	movs r1, #2
	cmp r0, #0
	beq _08004036
	movs r1, #1
_08004036:
	adds r0, r2, #0
	bl sub_8028A30
	b _0800404E
	.align 2, 0
_08004040: .4byte gUnknown_030012DC
_08004044:
	mov r1, sb
	ldr r0, [r1]
	movs r1, #0
	bl sub_8028A30
_0800404E:
	ldr r4, _080040C8 @ =gUnknown_030012DC
	ldr r2, [r4]
	movs r3, #0x88
	lsls r3, r3, #1
	adds r0, r2, r3
	str r5, [r0]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r2, r1
	str r6, [r0]
	adds r3, #0x20
	adds r0, r2, r3
	ldr r1, [r0]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0x24]
	mov r1, sp
	bl sub_803AD80
	mov r5, r8
	adds r5, #0x2b
	adds r0, r7, #0
	adds r0, #0xbc
	ldr r1, [sp, #0x10]
	adds r0, r0, r1
	ldr r0, [r0]
	lsls r1, r5, #8
	str r1, [r0]
	ldr r2, [sp, #0x18]
	lsls r1, r2, #8
	str r1, [r0, #4]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	adds r5, #0xd
	ldr r6, [sp, #0x14]
	ldr r3, [sp, #0xc]
	ldr r0, [r3, #8]
	mov r1, sp
	movs r2, #0xa
	bl itoa
	ldr r0, [sp, #8]
	cmp r0, #0
	beq _080040D8
	ldr r2, [r4]
	ldr r0, [r7, #4]
	asrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	movs r1, #2
	cmp r0, #0
	beq _080040BE
	movs r1, #1
_080040BE:
	adds r0, r2, #0
	bl sub_8028A30
	b _080040E0
	.align 2, 0
_080040C8: .4byte gUnknown_030012DC
_080040CC:
	movs r0, #0x25
	strb r0, [r1]
	adds r0, r3, #1
	add r0, sp
	strb r2, [r0]
	b _0800412A
_080040D8:
	ldr r0, [r4]
	movs r1, #0
	bl sub_8028A30
_080040E0:
	ldr r0, _08004168 @ =gUnknown_030012DC
	ldr r2, [r0]
	movs r1, #0x88
	lsls r1, r1, #1
	adds r0, r2, r1
	str r5, [r0]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r0, r2, r3
	str r6, [r0]
	adds r1, #0x20
	adds r0, r2, r1
	ldr r1, [r0]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0x24]
	mov r1, sp
	bl sub_803AD80
	ldr r1, [sp, #0xc]
	ldr r0, [r1]
	mov r1, sp
	movs r2, #0xa
	bl itoa
	movs r3, #0
	mov r6, sl
	subs r6, #2
_0800411A:
	mov r2, sp
	adds r1, r2, r3
	ldrb r2, [r1]
	cmp r2, #0
	beq _080040CC
	adds r3, #1
	cmp r3, #6
	ble _0800411A
_0800412A:
	ldr r4, _0800416C @ =gUnknown_030012E0
	ldr r0, [r4]
	movs r3, #0x98
	lsls r3, r3, #1
	adds r1, r0, r3
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	mov r1, sp
	bl sub_803AD80
	adds r5, r0, #0
	ldr r0, [sp, #8]
	cmp r0, #0
	beq _08004170
	ldr r2, [r4]
	ldr r0, [r7, #4]
	asrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	movs r1, #2
	cmp r0, #0
	beq _0800415E
	movs r1, #1
_0800415E:
	adds r0, r2, #0
	bl sub_8028A30
	b _08004178
	.align 2, 0
_08004168: .4byte gUnknown_030012DC
_0800416C: .4byte gUnknown_030012E0
_08004170:
	ldr r0, [r4]
	movs r1, #0
	bl sub_8028A30
_08004178:
	mov r2, r8
	subs r1, r2, r5
	ldr r0, _080041B8 @ =gUnknown_030012E0
	ldr r2, [r0]
	adds r1, #0x1f
	movs r3, #0x88
	lsls r3, r3, #1
	adds r0, r2, r3
	str r1, [r0]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r2, r1
	str r6, [r0]
	adds r3, #0x20
	adds r0, r2, r3
	ldr r1, [r0]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0x24]
	mov r1, sp
	bl sub_803AD80
	add sp, #0x1c
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080041B8: .4byte gUnknown_030012E0

@ sub_80041BC is reconstructed (semantics understood) but NOT YET
@ byte-matching - parked as C in src/graphics/settings_menu.c, guarded
@ by #if NON_MATCHING. See docs/matching.md for this chunk's write-up.
.if NON_MATCHING == 0
	thumb_func_start sub_80041BC
sub_80041BC: @ 0x080041BC
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	mov r8, r0
	mov sb, r1
	adds r7, r2, #0
	mov r0, sb
	movs r1, #0
	bl sub_8002CE8
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08004280
	movs r0, #0
	cmp r7, #0
	bne _080041E2
	movs r0, #1
_080041E2:
	cmp r0, #0
	beq _08004208
	ldr r0, _08004204 @ =gUnknown_030012DC
	ldr r2, [r0]
	mov r1, r8
	ldr r0, [r1, #4]
	asrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	movs r1, #2
	cmp r0, #0
	beq _080041FC
	movs r1, #1
_080041FC:
	adds r0, r2, #0
	bl sub_8028A30
	b _08004212
	.align 2, 0
_08004204: .4byte gUnknown_030012DC
_08004208:
	ldr r0, _0800427C @ =gUnknown_030012DC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8028A30
_08004212:
	ldr r6, _0800427C @ =gUnknown_030012DC
	ldr r4, [r6]
	movs r3, #0x98
	lsls r3, r3, #1
	adds r0, r4, r3
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x10
	movs r1, #0x10
	ldrsh r0, [r0, r1]
	adds r4, r4, r0
	movs r0, #0x25
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	movs r1, #0x43
	subs r1, r1, r0
	ldr r2, [r6]
	movs r3, #0x88
	lsls r3, r3, #1
	adds r0, r2, r3
	str r1, [r0]
	movs r0, #0x8a
	lsls r0, r0, #1
	adds r1, r2, r0
	movs r0, #0x2d
	str r0, [r1]
	movs r1, #0x98
	lsls r1, r1, #1
	adds r0, r2, r1
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r3, #0x20
	ldrsh r4, [r0, r3]
	adds r4, r2, r4
	movs r0, #0x25
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	b _08004298
	.align 2, 0
_0800427C: .4byte gUnknown_030012DC
_08004280:
	movs r1, #0
	cmp r7, #0
	bne _08004288
	movs r1, #1
_08004288:
	mov r0, sp
	strb r1, [r0]
	mov r0, r8
	movs r1, #0x26
	movs r2, #0x21
	movs r3, #1
	bl sub_8003F30
_08004298:
	mov r0, sb
	movs r1, #1
	bl sub_8002CE8
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0800434C
	movs r0, #0
	cmp r7, #1
	bne _080042AE
	movs r0, #1
_080042AE:
	cmp r0, #0
	beq _080042D4
	ldr r0, _080042D0 @ =gUnknown_030012DC
	ldr r2, [r0]
	mov r1, r8
	ldr r0, [r1, #4]
	asrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	movs r1, #2
	cmp r0, #0
	beq _080042C8
	movs r1, #1
_080042C8:
	adds r0, r2, #0
	bl sub_8028A30
	b _080042DE
	.align 2, 0
_080042D0: .4byte gUnknown_030012DC
_080042D4:
	ldr r0, _08004348 @ =gUnknown_030012DC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8028A30
_080042DE:
	ldr r6, _08004348 @ =gUnknown_030012DC
	ldr r4, [r6]
	movs r3, #0x98
	lsls r3, r3, #1
	adds r0, r4, r3
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x10
	movs r1, #0x10
	ldrsh r0, [r0, r1]
	adds r4, r4, r0
	movs r0, #0x25
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	movs r1, #0x43
	subs r1, r1, r0
	ldr r2, [r6]
	movs r3, #0x88
	lsls r3, r3, #1
	adds r0, r2, r3
	str r1, [r0]
	movs r0, #0x8a
	lsls r0, r0, #1
	adds r1, r2, r0
	movs r0, #0x5f
	str r0, [r1]
	movs r1, #0x98
	lsls r1, r1, #1
	adds r0, r2, r1
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r3, #0x20
	ldrsh r4, [r0, r3]
	adds r4, r2, r4
	movs r0, #0x25
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	b _08004364
	.align 2, 0
_08004348: .4byte gUnknown_030012DC
_0800434C:
	movs r1, #0
	cmp r7, #1
	bne _08004354
	movs r1, #1
_08004354:
	mov r0, sp
	strb r1, [r0]
	mov r0, r8
	movs r1, #0x26
	movs r2, #0x53
	movs r3, #2
	bl sub_8003F30
_08004364:
	mov r0, sb
	movs r1, #2
	bl sub_8002CE8
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08004418
	movs r0, #0
	cmp r7, #2
	bne _0800437A
	movs r0, #1
_0800437A:
	cmp r0, #0
	beq _080043A0
	ldr r0, _0800439C @ =gUnknown_030012DC
	ldr r2, [r0]
	mov r1, r8
	ldr r0, [r1, #4]
	asrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	movs r1, #2
	cmp r0, #0
	beq _08004394
	movs r1, #1
_08004394:
	adds r0, r2, #0
	bl sub_8028A30
	b _080043AA
	.align 2, 0
_0800439C: .4byte gUnknown_030012DC
_080043A0:
	ldr r0, _08004414 @ =gUnknown_030012DC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8028A30
_080043AA:
	ldr r6, _08004414 @ =gUnknown_030012DC
	ldr r4, [r6]
	movs r3, #0x98
	lsls r3, r3, #1
	adds r0, r4, r3
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x10
	movs r1, #0x10
	ldrsh r0, [r0, r1]
	adds r4, r4, r0
	movs r0, #0x25
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	movs r1, #0xa3
	subs r1, r1, r0
	ldr r2, [r6]
	movs r3, #0x88
	lsls r3, r3, #1
	adds r0, r2, r3
	str r1, [r0]
	movs r0, #0x8a
	lsls r0, r0, #1
	adds r1, r2, r0
	movs r0, #0x2d
	str r0, [r1]
	movs r1, #0x98
	lsls r1, r1, #1
	adds r0, r2, r1
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r3, #0x20
	ldrsh r4, [r0, r3]
	adds r4, r2, r4
	movs r0, #0x25
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	b _08004430
	.align 2, 0
_08004414: .4byte gUnknown_030012DC
_08004418:
	movs r1, #0
	cmp r7, #2
	bne _08004420
	movs r1, #1
_08004420:
	mov r0, sp
	strb r1, [r0]
	mov r0, r8
	movs r1, #0x86
	movs r2, #0x21
	movs r3, #3
	bl sub_8003F30
_08004430:
	mov r0, sb
	movs r1, #3
	bl sub_8002CE8
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _080044E4
	movs r0, #0
	cmp r7, #3
	bne _08004446
	movs r0, #1
_08004446:
	cmp r0, #0
	beq _0800446C
	ldr r0, _08004468 @ =gUnknown_030012DC
	ldr r2, [r0]
	mov r1, r8
	ldr r0, [r1, #4]
	asrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	movs r1, #2
	cmp r0, #0
	beq _08004460
	movs r1, #1
_08004460:
	adds r0, r2, #0
	bl sub_8028A30
	b _08004476
	.align 2, 0
_08004468: .4byte gUnknown_030012DC
_0800446C:
	ldr r0, _080044E0 @ =gUnknown_030012DC
	ldr r0, [r0]
	movs r1, #0
	bl sub_8028A30
_08004476:
	ldr r6, _080044E0 @ =gUnknown_030012DC
	ldr r4, [r6]
	movs r3, #0x98
	lsls r3, r3, #1
	adds r0, r4, r3
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x10
	movs r1, #0x10
	ldrsh r0, [r0, r1]
	adds r4, r4, r0
	movs r0, #0x25
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	movs r1, #0xa3
	subs r1, r1, r0
	ldr r2, [r6]
	movs r3, #0x88
	lsls r3, r3, #1
	adds r0, r2, r3
	str r1, [r0]
	movs r0, #0x8a
	lsls r0, r0, #1
	adds r1, r2, r0
	movs r0, #0x5f
	str r0, [r1]
	movs r1, #0x98
	lsls r1, r1, #1
	adds r0, r2, r1
	ldr r0, [r0]
	adds r5, r0, #0
	adds r5, #0x20
	movs r3, #0x20
	ldrsh r4, [r0, r3]
	adds r4, r2, r4
	movs r0, #0x25
	bl sub_8026F38
	adds r1, r0, #0
	ldr r2, [r5, #4]
	adds r0, r4, #0
	bl sub_803AD80
	b _080044FC
	.align 2, 0
_080044E0: .4byte gUnknown_030012DC
_080044E4:
	movs r1, #0
	cmp r7, #3
	bne _080044EC
	movs r1, #1
_080044EC:
	mov r0, sp
	strb r1, [r0]
	mov r0, r8
	movs r1, #0x86
	movs r2, #0x53
	movs r3, #4
	bl sub_8003F30
_080044FC:
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
.endif

@ sub_800450C's semantics aren't confidently understood yet in
@ full (a large screen-init routine touching several still-raw
@ helpers and an unconfirmed triple-pointer-dereference table) -
@ left fully untouched rather than force a low-confidence
@ reconstruction. See the PR/issue for this chunk.
	thumb_func_start sub_800450C
sub_800450C: @ 0x0800450C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x14
	adds r7, r0, #0
	ldr r4, _080047D0 @ =gUnknown_03001300
	ldr r0, [r4]
	bl sub_8006A90
	ldr r0, [r4]
	bl sub_8006A48
	bl sub_80006A8
	ldr r0, [r4]
	bl sub_8006AAC
	ldr r4, _080047D4 @ =gUnknown_030012B8
	ldr r0, [r4]
	bl sub_8006EA8
	ldr r0, [r4]
	movs r1, #0
	bl sub_8006D50
	ldr r0, [r4]
	movs r1, #1
	bl sub_8006D50
	ldr r0, [r4]
	movs r1, #2
	bl sub_8006D50
	ldr r0, [r4]
	movs r1, #3
	bl sub_8006D50
	ldr r0, [r4]
	movs r1, #0
	mov r8, r1
	adds r2, r0, #0
	adds r2, #0x6c
	ldr r6, _080047D8 @ =gStaticData_0816B15A
	adds r1, r0, #0
	adds r1, #0x2c
	ldr r5, _080047DC @ =gStaticData_0816B13A
	ldr r4, _080047E0 @ =gStaticData_0816B19A
	ldr r3, _080047E4 @ =gStaticData_0816B17A
_08004570:
	ldrh r0, [r5]
	strh r0, [r1]
	ldrh r0, [r6]
	strh r0, [r1, #0x20]
	ldrh r0, [r3]
	strh r0, [r2]
	ldrh r0, [r4]
	strh r0, [r2, #0x20]
	adds r2, #2
	adds r6, #2
	adds r1, #2
	adds r5, #2
	adds r4, #2
	adds r3, #2
	movs r0, #1
	add r8, r0
	mov r0, r8
	cmp r0, #0xf
	ble _08004570
	ldr r5, _080047E8 @ =gUnknown_030012DC
	ldr r0, [r5]
	movs r1, #0
	bl sub_8028A30
	ldr r1, _080047EC @ =gUnknown_030012E0
	mov r8, r1
	ldr r0, [r1]
	movs r1, #0
	bl sub_8028A30
	ldr r6, _080047F0 @ =gUnknown_030012FC
	ldr r0, [r6]
	movs r4, #0
	str r4, [r0, #8]
	bl sub_8006C4C
	ldr r0, [r6]
	bl sub_8006C4C
	ldr r0, [r5]
	movs r2, #0x84
	lsls r2, r2, #1
	adds r1, r0, r2
	str r4, [r1]
	movs r3, #0x98
	lsls r3, r3, #1
	adds r1, r0, r3
	ldr r1, [r1]
	adds r1, #0x40
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r1, [r1, #4]
	bl sub_803AD7C
	ldr r0, [r6]
	ldr r1, [r5]
	movs r2, #0x96
	lsls r2, r2, #1
	adds r1, r1, r2
	ldr r1, [r1]
	lsls r1, r1, #5
	bl sub_8006C58
	ldr r0, [r5]
	movs r3, #0x96
	lsls r3, r3, #1
	adds r0, r0, r3
	ldr r2, [r0]
	mov r1, r8
	ldr r0, [r1]
	subs r3, #0x24
	adds r1, r0, r3
	str r2, [r1]
	movs r2, #0x98
	lsls r2, r2, #1
	adds r1, r0, r2
	ldr r1, [r1]
	adds r1, #0x40
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r1, [r1, #4]
	bl sub_803AD7C
	ldr r0, [r6]
	mov r2, r8
	ldr r1, [r2]
	movs r3, #0x96
	lsls r3, r3, #1
	adds r1, r1, r3
	ldr r1, [r1]
	lsls r1, r1, #5
	bl sub_8006C58
	ldr r0, [r6]
	bl sub_8006C30
	adds r0, r7, #0
	adds r0, #0xa8
	str r0, [sp]
	adds r1, r7, #0
	adds r1, #0xbc
	str r1, [sp, #4]
	adds r2, r7, #0
	adds r2, #0xd0
	str r2, [sp, #0x10]
	adds r3, r7, #0
	adds r3, #0xc0
	str r3, [sp, #8]
	adds r7, #0xc4
	str r7, [sp, #0xc]
	movs r0, #0xf
	mov sl, r0
	movs r1, #0x80
	mov sb, r1
	adds r7, r2, #0
	ldr r6, [sp, #4]
	ldr r5, [sp]
	movs r2, #4
	mov r8, r2
_08004662:
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	adds r4, r0, #0
	str r4, [r5]
	ldr r3, _080047F4 @ =gUnknown_030012D0
	ldr r0, [r3]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	movs r0, #1
	adds r1, r4, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r5]
	bl sub_800815C
	ldr r2, [r5]
	adds r2, #0x29
	mov r3, sl
	ands r0, r3
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldm r5!, {r0}
	mov r1, sb
	strh r1, [r0, #0x3c]
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	adds r4, r0, #0
	str r4, [r6]
	ldr r2, _080047F4 @ =gUnknown_030012D0
	ldr r0, [r2]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r3, #0xc6
	lsls r3, r3, #1
	adds r0, r0, r3
	str r0, [r4, #0x20]
	movs r0, #2
	adds r1, r4, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r6]
	bl sub_800815C
	ldr r2, [r6]
	adds r2, #0x29
	mov r1, sl
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldm r6!, {r0}
	mov r1, sb
	strh r1, [r0, #0x3c]
	movs r0, #0x40
	bl sub_8026EDC
	bl sub_8008904
	adds r4, r0, #0
	str r4, [r7]
	ldr r2, _080047F4 @ =gUnknown_030012D0
	ldr r0, [r2]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r3, #0xde
	lsls r3, r3, #1
	adds r0, r0, r3
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	movs r1, #0
	strb r1, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, [r7]
	bl sub_800815C
	ldr r2, [r7]
	adds r2, #0x29
	mov r3, sl
	ands r0, r3
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	ldm r7!, {r0}
	mov r1, sb
	strh r1, [r0, #0x3c]
	movs r2, #1
	rsbs r2, r2, #0
	add r8, r2
	mov r3, r8
	cmp r3, #0
	blt _0800477E
	b _08004662
_0800477E:
	ldr r0, [sp]
	ldr r1, [r0]
	movs r2, #0xa0
	lsls r2, r2, #5
	str r2, [r1]
	movs r0, #0xa0
	lsls r0, r0, #6
	str r0, [r1, #4]
	ldr r1, [sp, #4]
	ldr r0, [r1]
	str r2, [r0]
	movs r3, #0xa0
	lsls r3, r3, #7
	str r3, [r0, #4]
	ldr r0, [sp, #8]
	ldr r1, [r0]
	str r2, [r1]
	movs r0, #0xf0
	lsls r0, r0, #6
	str r0, [r1, #4]
	ldr r0, [sp, #0xc]
	ldr r1, [r0]
	str r2, [r1]
	movs r0, #0x96
	lsls r0, r0, #7
	str r0, [r1, #4]
	ldr r2, [sp, #0x10]
	ldr r1, [r2]
	movs r0, #0xf0
	lsls r0, r0, #7
	str r0, [r1]
	str r3, [r1, #4]
	add sp, #0x14
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080047D0: .4byte gUnknown_03001300
_080047D4: .4byte gUnknown_030012B8
_080047D8: .4byte gStaticData_0816B15A
_080047DC: .4byte gStaticData_0816B13A
_080047E0: .4byte gStaticData_0816B19A
_080047E4: .4byte gStaticData_0816B17A
_080047E8: .4byte gUnknown_030012DC
_080047EC: .4byte gUnknown_030012E0
_080047F0: .4byte gUnknown_030012FC
_080047F4: .4byte gUnknown_030012D0
