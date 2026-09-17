.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8022D50 - a level-start/reset routine (clears self+0x8c/0x90..0xa0,
@ tears down two actor slots at self+0x1bc/0x1c0 via sub_80087C0/
@ sub_80087B4/sub_800872C when non-null, then walks gUnknown_030012EC's
@ array firing sub_803AD7C table trampolines and setting collision-map
@ bits in gUnknown_030012B4). Left fully raw: several callees
@ (sub_80231EC, sub_80087C0/B4/2C, sub_8010804, sub_803AD7C,
@ sub_8011448) aren't characterized precisely enough in this session to
@ commit a byte-exact-attempt reconstruction with confidence - see
@ docs/matching.md's entry for GitHub issue #34.
	thumb_func_start sub_8022D50
sub_8022D50: @ 0x08022D50
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	movs r1, #0
	bl sub_80231EC
	adds r2, r5, #0
	adds r2, #0x8c
	movs r1, #0
	movs r0, #1
	strb r0, [r2]
	adds r0, r5, #0
	adds r0, #0x90
	str r1, [r0]
	adds r0, #4
	str r1, [r0]
	adds r0, #4
	str r1, [r0]
	adds r0, #4
	str r1, [r0]
	adds r0, #4
	str r1, [r0]
	adds r0, #0x3c
	ldr r0, [r0]
	ldr r0, [r0, #8]
	cmp r0, #3
	bne _08022D86
	b _08022E98
_08022D86:
	movs r1, #0xdc
	lsls r1, r1, #1
	adds r0, r5, r1
	ldr r4, [r0]
	cmp r4, #0
	beq _08022DAE
	movs r0, #7
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
_08022DAE:
	movs r2, #0xde
	lsls r2, r2, #1
	adds r5, r5, r2
	ldr r4, [r5]
	cmp r4, #0
	beq _08022DFC
	movs r0, #0xc
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
	ldr r0, _08022E50 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r3, [r5]
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
_08022DFC:
	bl sub_8010804
	movs r6, #0
	ldr r0, _08022E54 @ =gUnknown_030012EC
	ldr r0, [r0]
	ldr r0, [r0, #4]
	cmp r6, r0
	bge _08022E98
	ldr r7, _08022E58 @ =0x0000FFFF
_08022E0E:
	ldr r0, _08022E54 @ =gUnknown_030012EC
	ldr r0, [r0]
	ldr r1, [r0, #0xc]
	lsls r0, r6, #2
	adds r0, r0, r1
	ldr r4, [r0]
	adds r5, r4, #0
	ldr r1, [r4, #0x18]
	adds r1, #0x48
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	cmp r0, #2
	bne _08022E8C
	ldr r1, [r4, #0x18]
	movs r2, #0x28
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #0x2c]
	bl sub_803AD7C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08022E5C
	adds r0, r4, #0
	movs r1, #1
	bl sub_8011448
	b _08022E8C
	.align 2, 0
_08022E50: .4byte gUnknown_030012B8
_08022E54: .4byte gUnknown_030012EC
_08022E58: .4byte 0x0000FFFF
_08022E5C:
	movs r0, #1
	ldrb r4, [r5, #0xc]
	orrs r0, r4
	strb r0, [r5, #0xc]
	ldrh r0, [r5, #8]
	cmp r0, r7
	beq _08022E8C
	ldrh r3, [r5, #8]
	ldr r0, _08022EA0 @ =gUnknown_030012B4
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
_08022E8C:
	adds r6, #1
	ldr r0, _08022EA4 @ =gUnknown_030012EC
	ldr r0, [r0]
	ldr r0, [r0, #4]
	cmp r6, r0
	blt _08022E0E
_08022E98:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08022EA0: .4byte gUnknown_030012B4
_08022EA4: .4byte gUnknown_030012EC

