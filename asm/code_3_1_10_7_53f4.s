.include "asm/macros.inc"

.syntax unified
.arm

.if NON_MATCHING == 0
	thumb_func_start sub_80053F4
sub_80053F4: @ 0x080053F4
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	ldr r0, _080054F8 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006A90
	ldr r0, _080054FC @ =gUnknown_030012FC
	ldr r0, [r0]
	bl sub_8006C28
	ldr r6, _08005500 @ =gUnknown_030012E0
	ldr r0, [r6]
	movs r7, #0x98
	lsls r7, r7, #1
	adds r1, r0, r7
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r1, [r5, #0x70]
	ldr r2, [r2, #0x14]
	bl sub_803AD80
	movs r1, #0xf0
	subs r1, r1, r0
	lsrs r3, r1, #1
	ldr r0, [r6]
	movs r2, #0xe
	movs r4, #0x88
	lsls r4, r4, #1
	adds r1, r0, r4
	str r3, [r1]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r1, r0, r3
	str r2, [r1]
	adds r1, r0, r7
	ldr r2, [r1]
	movs r4, #0x20
	ldrsh r1, [r2, r4]
	adds r0, r0, r1
	ldr r1, [r5, #0x70]
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r4, [r5, #0x74]
	cmp r4, #0
	beq _08005498
	ldr r3, [r6]
	movs r1, #0x20
	movs r0, #0x26
	mov ip, r0
	movs r2, #0x88
	lsls r2, r2, #1
	adds r0, r3, r2
	str r1, [r0]
	adds r1, #0xf4
	adds r0, r3, r1
	mov r2, ip
	str r2, [r0]
	adds r1, r7, #0
	adds r0, r3, r1
	ldr r1, [r0]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r3, r0
	ldr r2, [r1, #0x24]
	adds r1, r4, #0
	bl sub_803AD80
	ldr r0, [r6]
	adds r3, r7, #0
	adds r1, r0, r3
	ldr r2, [r1]
	movs r4, #0x20
	ldrsh r1, [r2, r4]
	adds r0, r0, r1
	adds r1, r5, #0
	adds r1, #0x78
	ldr r2, [r2, #0x24]
	bl sub_803AD80
_08005498:
	ldr r0, [r6]
	adds r1, r0, r7
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	adds r4, r5, #0
	adds r4, #0x41
	ldr r2, [r2, #0x14]
	adds r1, r4, #0
	bl sub_803AD80
	movs r1, #0x8c
	subs r3, r1, r0
	ldr r0, [r6]
	movs r2, #0x88
	movs r6, #0x88
	lsls r6, r6, #1
	adds r1, r0, r6
	str r3, [r1]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r1, r0, r3
	str r2, [r1]
	adds r1, r0, r7
	ldr r2, [r1]
	movs r6, #0x20
	ldrsh r1, [r2, r6]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	adds r1, r4, #0
	bl sub_803AD80
	adds r0, r5, #0
	bl sub_800556C
	adds r0, r5, #0
	bl sub_80061E8
	ldr r0, [r5, #0x24]
	cmp r0, #4
	bhi _08005542
	lsls r0, r0, #2
	ldr r1, _08005504 @ =_08005508
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080054F8: .4byte gUnknown_03001300
_080054FC: .4byte gUnknown_030012FC
_08005500: .4byte gUnknown_030012E0
_08005504: .4byte _08005508
_08005508: @ jump table
	.4byte _0800551C @ case 0
	.4byte _08005524 @ case 1
	.4byte _0800552C @ case 2
	.4byte _08005534 @ case 3
	.4byte _0800553C @ case 4
_0800551C:
	adds r0, r5, #0
	bl sub_800619C
	b _08005542
_08005524:
	adds r0, r5, #0
	bl sub_800570C
	b _08005542
_0800552C:
	adds r0, r5, #0
	bl sub_80057E0
	b _08005542
_08005534:
	adds r0, r5, #0
	bl sub_80058C0
	b _08005542
_0800553C:
	adds r0, r5, #0
	bl sub_8006124
_08005542:
	adds r0, r5, #0
	adds r0, #0xc4
	ldr r0, [r0]
	cmp r0, #0
	bne _0800555A
	adds r0, r5, #0
	adds r0, #0xc0
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
_0800555A:
	ldr r0, _08005568 @ =gUnknown_03001300
	ldr r0, [r0]
	bl sub_8006A48
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08005568: .4byte gUnknown_03001300
.endif

.if NON_MATCHING == 0
	thumb_func_start sub_800556C
sub_800556C: @ 0x0800556C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xc
	mov sb, r0
	movs r0, #0x4a
	str r0, [sp]
	movs r1, #0
	mov sl, r1
	mov r2, sb
	ldr r0, [r2, #0x1c]
	cmp sl, r0
	blt _0800558C
	b _080056F0
_0800558C:
	adds r2, #0x57
	str r2, [sp, #4]
	mov r3, sb
	adds r3, #0x4f
	str r3, [sp, #8]
_08005596:
	mov r1, sb
	ldr r0, [r1, #0x18]
	cmp sl, r0
	bne _080055B0
	ldr r0, _080055AC @ =gUnknown_030012DC
	ldr r0, [r0]
	movs r1, #0xf
	bl sub_8028A30
	b _080055B8
	.align 2, 0
_080055AC: .4byte gUnknown_030012DC
_080055B0:
	ldr r0, _08005624 @ =gUnknown_030012DC
	ldr r0, [r0]
	bl sub_8028A40
_080055B8:
	mov r2, sb
	ldr r0, [r2, #0x14]
	mov r3, sl
	lsls r4, r3, #3
	adds r0, r4, r0
	ldr r0, [r0]
	bl sub_8026F38
	adds r7, r0, #0
	ldr r6, _08005624 @ =gUnknown_030012DC
	ldr r0, [r6]
	movs r1, #0x98
	lsls r1, r1, #1
	mov r8, r1
	adds r1, r0, r1
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	adds r1, r7, #0
	bl sub_803AD80
	lsrs r0, r0, #1
	movs r1, #0x32
	subs r5, r1, r0
	mov r1, sb
	ldr r0, [r1, #0x14]
	adds r4, r4, r0
	ldr r0, [r4, #4]
	cmp r0, #4
	beq _08005628
	cmp r0, #5
	beq _08005682
	ldr r0, [r6]
	movs r2, #0x88
	lsls r2, r2, #1
	adds r1, r0, r2
	str r5, [r1]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r1, r0, r3
	ldr r2, [sp]
	str r2, [r1]
	mov r3, r8
	adds r1, r0, r3
	ldr r2, [r1]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	adds r1, r7, #0
	b _080056D6
	.align 2, 0
_08005624: .4byte gUnknown_030012DC
_08005628:
	ldr r0, [r6]
	movs r2, #0x98
	lsls r2, r2, #1
	adds r1, r0, r2
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	ldr r1, [sp, #4]
	bl sub_803AD80
	lsrs r0, r0, #1
	subs r5, r5, r0
	ldr r2, [r6]
	movs r1, #0x88
	lsls r1, r1, #1
	adds r0, r2, r1
	str r5, [r0]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r0, r2, r3
	ldr r1, [sp]
	str r1, [r0]
	adds r3, #0x1c
	adds r0, r2, r3
	ldr r1, [r0]
	movs r3, #0x20
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0x24]
	adds r1, r7, #0
	bl sub_803AD80
	ldr r0, [r6]
	movs r2, #0x98
	lsls r2, r2, #1
	adds r1, r0, r2
	ldr r2, [r1]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	ldr r1, [sp, #4]
	b _080056D6
_08005682:
	ldr r0, [r6]
	mov r2, r8
	adds r1, r0, r2
	ldr r2, [r1]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x14]
	ldr r1, [sp, #8]
	bl sub_803AD80
	lsrs r0, r0, #1
	subs r5, r5, r0
	ldr r0, [r6]
	movs r2, #0x88
	lsls r2, r2, #1
	adds r1, r0, r2
	str r5, [r1]
	movs r3, #0x8a
	lsls r3, r3, #1
	adds r1, r0, r3
	ldr r2, [sp]
	str r2, [r1]
	mov r3, r8
	adds r1, r0, r3
	ldr r2, [r1]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	adds r1, r7, #0
	bl sub_803AD80
	ldr r0, [r6]
	mov r2, r8
	adds r1, r0, r2
	ldr r2, [r1]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x24]
	ldr r1, [sp, #8]
_080056D6:
	bl sub_803AD80
	mov r1, sb
	ldr r0, [r1, #0x20]
	ldr r2, [sp]
	adds r2, r2, r0
	str r2, [sp]
	movs r3, #1
	add sl, r3
	ldr r0, [r1, #0x1c]
	cmp sl, r0
	bge _080056F0
	b _08005596
_080056F0:
	ldr r0, _08005708 @ =gUnknown_030012DC
	ldr r0, [r0]
	bl sub_8028A40
	add sp, #0xc
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08005708: .4byte gUnknown_030012DC
.endif
