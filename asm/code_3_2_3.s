.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8007DBC is reconstructed (extremely close, but not yet
@ byte-matching) as C in src/graphics/actor_part2.c, guarded by
@ #if NON_MATCHING - this raw version is used only for the real
@ byte-matching build. See docs/matching.md, "Parked, not matched:
@ sub_8007DBC".
.if NON_MATCHING == 0
	thumb_func_start sub_8007DBC
sub_8007DBC: @ 0x08007DBC
	push {r4, r5, r6, r7, lr}
	sub sp, #0x28
	adds r5, r0, #0
	ldrb r0, [r5, #0xc]
	lsls r1, r0, #0x18
	lsrs r0, r1, #0x1b
	movs r6, #1
	ands r0, r6
	cmp r0, #0
	beq _08007DD2
	b _08007F68
_08007DD2:
	lsrs r0, r1, #0x1a
	ands r0, r6
	cmp r0, #0
	bne _08007DDC
	b _08007F68
_08007DDC:
	add r0, sp, #8
	adds r1, r5, #0
	bl sub_8007B98
	ldr r7, _08007E70 @ =gUnknown_030012D8
	ldr r1, [r7]
	ldrb r2, [r1, #0xc]
	lsrs r0, r2, #7
	cmp r0, #0
	bne _08007DF2
	b _08007F68
_08007DF2:
	add r4, sp, #0x18
	adds r0, r4, #0
	bl sub_8007B98
	adds r0, r4, #0
	add r1, sp, #8
	bl sub_8001688
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08007E0A
	b _08007F68
_08007E0A:
	movs r0, #8
	ldrb r3, [r5, #0xc]
	orrs r0, r3
	strb r0, [r5, #0xc]
	ldr r0, [r7]
	ldr r1, [r0, #0x18]
	adds r1, #0x68
	movs r4, #0
	ldrsh r2, [r1, r4]
	adds r0, r0, r2
	ldrb r2, [r5, #0xa]
	ldr r4, [r1, #4]
	movs r1, #0
	movs r3, #0
	bl sub_803AD88
	ldrb r0, [r5, #0xc]
	orrs r0, r6
	strb r0, [r5, #0xc]
	ldr r0, _08007E74 @ =0x0000FFFF
	ldrh r1, [r5, #8]
	cmp r1, r0
	beq _08007E5A
	ldrh r3, [r5, #8]
	ldr r0, _08007E78 @ =gUnknown_030012B4
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
_08007E5A:
	movs r3, #0
	ldrb r0, [r5, #0xa]
	subs r0, #0x1b
	cmp r0, #7
	bhi _08007F48
	lsls r0, r0, #2
	ldr r1, _08007E7C @ =_08007E80
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08007E70: .4byte gUnknown_030012D8
_08007E74: .4byte 0x0000FFFF
_08007E78: .4byte gUnknown_030012B4
_08007E7C: .4byte _08007E80
_08007E80: @ jump table
	.4byte _08007F2C @ case 0
	.4byte _08007F48 @ case 1
	.4byte _08007EA0 @ case 2
	.4byte _08007EA0 @ case 3
	.4byte _08007ED8 @ case 4
	.4byte _08007F10 @ case 5
	.4byte _08007EBC @ case 6
	.4byte _08007EF4 @ case 7
_08007EA0:
	ldr r3, [r5]
	asrs r3, r3, #8
	ldr r1, [r5, #4]
	asrs r1, r1, #8
	ldr r0, _08007EB8 @ =gUnknown_030012E4
	ldr r0, [r0]
	str r1, [sp]
	movs r1, #0
	str r1, [sp, #4]
	movs r1, #0x2b
	movs r2, #1
	b _08007F42
	.align 2, 0
_08007EB8: .4byte gUnknown_030012E4
_08007EBC:
	ldr r3, [r5]
	asrs r3, r3, #8
	ldr r1, [r5, #4]
	asrs r1, r1, #8
	ldr r0, _08007ED4 @ =gUnknown_030012E4
	ldr r0, [r0]
	str r1, [sp]
	movs r1, #0
	str r1, [sp, #4]
	movs r1, #0x2b
	movs r2, #6
	b _08007F42
	.align 2, 0
_08007ED4: .4byte gUnknown_030012E4
_08007ED8:
	ldr r3, [r5]
	asrs r3, r3, #8
	ldr r1, [r5, #4]
	asrs r1, r1, #8
	ldr r0, _08007EF0 @ =gUnknown_030012E4
	ldr r0, [r0]
	str r1, [sp]
	movs r1, #0
	str r1, [sp, #4]
	movs r1, #0x2b
	movs r2, #5
	b _08007F42
	.align 2, 0
_08007EF0: .4byte gUnknown_030012E4
_08007EF4:
	ldr r3, [r5]
	asrs r3, r3, #8
	ldr r1, [r5, #4]
	asrs r1, r1, #8
	ldr r0, _08007F0C @ =gUnknown_030012E4
	ldr r0, [r0]
	str r1, [sp]
	movs r1, #0
	str r1, [sp, #4]
	movs r1, #0x2b
	movs r2, #0
	b _08007F42
	.align 2, 0
_08007F0C: .4byte gUnknown_030012E4
_08007F10:
	ldr r3, [r5]
	asrs r3, r3, #8
	ldr r1, [r5, #4]
	asrs r1, r1, #8
	ldr r0, _08007F28 @ =gUnknown_030012E4
	ldr r0, [r0]
	str r1, [sp]
	movs r1, #0
	str r1, [sp, #4]
	movs r1, #0x2b
	movs r2, #3
	b _08007F42
	.align 2, 0
_08007F28: .4byte gUnknown_030012E4
_08007F2C:
	ldr r3, [r5]
	asrs r3, r3, #8
	ldr r1, [r5, #4]
	asrs r1, r1, #8
	ldr r0, _08007F74 @ =gUnknown_030012E4
	ldr r0, [r0]
	str r1, [sp]
	movs r1, #0
	str r1, [sp, #4]
	movs r1, #0x2b
	movs r2, #4
_08007F42:
	bl sub_8025BAC
	adds r3, r0, #0
_08007F48:
	cmp r3, #0
	beq _08007F68
	adds r2, r3, #0
	adds r2, #0x28
	movs r1, #1
	movs r0, #4
	rsbs r0, r0, #0
	ldrb r4, [r2]
	ands r0, r4
	orrs r0, r1
	strb r0, [r2]
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r1, [r3, #0xc]
	ands r0, r1
	strb r0, [r3, #0xc]
_08007F68:
	movs r0, #0
	add sp, #0x28
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08007F74: .4byte gUnknown_030012E4

.endif

