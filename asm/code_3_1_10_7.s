.include "asm/macros.inc"

.syntax unified
.arm

.if NON_MATCHING == 0
	thumb_func_start sub_8004D74
sub_8004D74: @ 0x08004D74
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #0xc0
	lsls r0, r0, #0x18
	mov sl, r0
	bl mem_free_bytes
	ldr r0, _08004EA4 @ =gUnknown_030012BC
	ldr r0, [r0]
	bl sub_80019E8
	bl sub_80006A8
	movs r0, #0xa0
	lsls r0, r0, #0x13
	movs r1, #0
	strh r1, [r0]
	movs r0, #0x80
	lsls r0, r0, #0x13
	strh r1, [r0]
	ldr r6, _08004EA8 @ =gUnknown_030012B8
	ldr r1, [r6]
	mov sb, r1
	movs r0, #0x8c
	lsls r0, r0, #2
	bl sub_8026EDC
	bl sub_8006FB4
	str r0, [r6]
	ldr r2, _08004EAC @ =gStaticData_084A5600
	ldrh r1, [r2, #0xe]
	ldr r2, [r2, #8]
	bl sub_8006EF0
	ldr r0, [r6]
	movs r1, #0xf
	bl sub_8006D50
	ldr r1, [r6]
	ldr r0, _08004EB0 @ =gStaticData_0816B2C0
	movs r2, #0x83
	lsls r2, r2, #2
	adds r1, r1, r2
	movs r2, #0x10
	bl sub_803A94C
	ldr r4, _08004EB4 @ =gUnknown_030012DC
	ldr r0, [r4]
	bl sub_8028A40
	ldr r5, _08004EB8 @ =gUnknown_030012E0
	ldr r0, [r5]
	bl sub_8028A40
	ldr r0, [r4]
	movs r3, #0
	mov r8, r3
	movs r2, #0x84
	lsls r2, r2, #1
	adds r1, r0, r2
	str r3, [r1]
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
	ldr r0, [r4]
	movs r1, #0x96
	lsls r1, r1, #1
	adds r0, r0, r1
	ldr r2, [r0]
	ldr r0, [r5]
	movs r3, #0x84
	lsls r3, r3, #1
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
	ldr r0, [r4]
	movs r1, #0x96
	lsls r1, r1, #1
	adds r0, r0, r1
	ldr r1, [r0]
	ldr r0, [r5]
	movs r2, #0x96
	lsls r2, r2, #1
	adds r0, r0, r2
	ldr r2, [r0]
	ldr r7, _08004EBC @ =gUnknown_030012FC
	ldr r0, [r7]
	adds r1, r1, r2
	str r1, [r0, #8]
	bl sub_8006C4C
	movs r0, #0xd4
	bl sub_8026EDC
	bl sub_8004EC0
	adds r4, r0, #0
	bl sub_8005100
	adds r5, r0, #0
	cmp r4, #0
	beq _08004E74
	adds r0, r4, #0
	movs r1, #3
	bl sub_8005004
_08004E74:
	ldr r0, [r7]
	mov r3, r8
	str r3, [r0, #8]
	bl sub_8006C4C
	ldr r0, [r6]
	cmp r0, #0
	beq _08004E8A
	movs r1, #3
	bl sub_8006F94
_08004E8A:
	mov r0, sb
	str r0, [r6]
	mov r0, sl
	bl mem_free_bytes
	adds r0, r5, #0
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08004EA4: .4byte gUnknown_030012BC
_08004EA8: .4byte gUnknown_030012B8
_08004EAC: .4byte gStaticData_084A5600
_08004EB0: .4byte gStaticData_0816B2C0
_08004EB4: .4byte gUnknown_030012DC
_08004EB8: .4byte gUnknown_030012E0
_08004EBC: .4byte gUnknown_030012FC
.endif
