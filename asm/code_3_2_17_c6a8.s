.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_800C6A8
sub_800C6A8: @ 0x0800C6A8
	push {r4, r5, lr}
	adds r5, r0, #0
	str r1, [r5, #0x74]
	subs r0, r1, #1
	cmp r0, #0x11
	bls _0800C6B6
	b _0800C84E
_0800C6B6:
	lsls r0, r0, #2
	ldr r1, _0800C6C0 @ =_0800C6C4
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800C6C0: .4byte _0800C6C4
_0800C6C4: @ jump table
	.4byte _0800C7D4 @ case 0
	.4byte _0800C742 @ case 1
	.4byte _0800C7D4 @ case 2
	.4byte _0800C76C @ case 3
	.4byte _0800C70C @ case 4
	.4byte _0800C734 @ case 5
	.4byte _0800C81C @ case 6
	.4byte _0800C7E2 @ case 7
	.4byte _0800C734 @ case 8
	.4byte _0800C734 @ case 9
	.4byte _0800C734 @ case 10
	.4byte _0800C84E @ case 11
	.4byte _0800C75E @ case 12
	.4byte _0800C76C @ case 13
	.4byte _0800C742 @ case 14
	.4byte _0800C76C @ case 15
	.4byte _0800C7D4 @ case 16
	.4byte _0800C75E @ case 17
_0800C70C:
	ldr r1, [r5, #0x70]
	ldr r0, _0800C730 @ =0xFFFFFE80
	movs r2, #0
	str r0, [r1, #0x60]
	str r0, [r1, #0x48]
	str r2, [r1, #0x4c]
	str r0, [r1, #0x50]
	movs r0, #0x80
	lsls r0, r0, #3
	str r0, [r1, #0x64]
	str r0, [r1, #0x54]
	str r2, [r1, #0x58]
	str r0, [r1, #0x5c]
	movs r0, #0x80
	ldrb r2, [r1, #0xc]
	orrs r0, r2
	strb r0, [r1, #0xc]
	b _0800C84E
	.align 2, 0
_0800C730: .4byte 0xFFFFFE80
_0800C734:
	movs r0, #0
	str r0, [r5, #0x68]
	ldr r3, [r5, #0xc]
	adds r3, #0x50
	movs r4, #0
	ldrsh r0, [r3, r4]
	b _0800C808
_0800C742:
	movs r0, #1
	str r0, [r5, #0x78]
	ldr r1, [r5, #0x70]
	adds r0, r5, #0
	movs r2, #1
	bl sub_800B838
	movs r0, #0
	str r0, [r5, #0x68]
	ldr r3, [r5, #0xc]
	adds r3, #0x50
	movs r1, #0
	ldrsh r0, [r3, r1]
	b _0800C808
_0800C75E:
	movs r0, #1
	str r0, [r5, #0x78]
	ldr r1, [r5, #0x70]
	adds r0, r5, #0
	movs r2, #1
	bl sub_800B838
_0800C76C:
	ldr r1, [r5, #0x38]
	ldr r0, [r5, #0x30]
	cmp r1, r0
	blt _0800C794
	movs r0, #4
	str r0, [r5, #0x68]
	ldr r3, [r5, #0xc]
	adds r3, #0x50
	movs r2, #0
	ldrsh r0, [r3, r2]
	adds r0, r5, r0
	ldr r1, [r5, #0x70]
	adds r2, r5, #0
	adds r2, #0x84
	ldr r2, [r2]
	ldr r2, [r2, #0x10]
	ldr r3, [r3, #4]
	bl sub_803AD84
	b _0800C7B8
_0800C794:
	movs r0, #0
	str r0, [r5, #0x68]
	ldr r3, [r5, #0xc]
	adds r3, #0x50
	movs r1, #0
	ldrsh r0, [r3, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x70]
	adds r2, r5, #0
	adds r2, #0x84
	ldr r2, [r2]
	ldr r2, [r2]
	ldr r3, [r3, #4]
	bl sub_803AD84
	ldr r0, [r5, #0x6c]
	cmp r0, #0x1b
	bne _0800C84E
_0800C7B8:
	ldr r1, [r5, #0x70]
	ldr r0, [r1, #0x20]
	adds r3, r1, #0
	adds r3, #0x2d
	ldr r2, [r0]
	ldrb r4, [r3]
	lsls r0, r4, #3
	subs r0, r0, r4
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #0x16]
	subs r0, #1
	str r0, [r1, #0x30]
	b _0800C84E
_0800C7D4:
	movs r0, #0
	str r0, [r5, #0x68]
	ldr r3, [r5, #0xc]
	adds r3, #0x50
	movs r1, #0
	ldrsh r0, [r3, r1]
	b _0800C808
_0800C7E2:
	movs r4, #3
	str r4, [r5, #0x78]
	ldr r1, [r5, #0x70]
	adds r0, r5, #0
	movs r2, #3
	bl sub_800B838
	str r4, [r5, #0x7c]
	ldr r1, [r5, #0x70]
	adds r0, r5, #0
	movs r2, #3
	bl sub_800B704
	movs r0, #0
	str r0, [r5, #0x68]
	ldr r3, [r5, #0xc]
	adds r3, #0x50
	movs r2, #0
	ldrsh r0, [r3, r2]
_0800C808:
	adds r0, r5, r0
	ldr r1, [r5, #0x70]
	adds r2, r5, #0
	adds r2, #0x84
	ldr r2, [r2]
	ldr r2, [r2]
	ldr r3, [r3, #4]
	bl sub_803AD84
	b _0800C84E
_0800C81C:
	movs r0, #2
	str r0, [r5, #0x78]
	ldr r1, [r5, #0x70]
	adds r0, r5, #0
	movs r2, #2
	bl sub_800B838
	movs r4, #0
	str r4, [r5, #0x68]
	ldr r3, [r5, #0xc]
	adds r3, #0x50
	movs r1, #0
	ldrsh r0, [r3, r1]
	adds r0, r5, r0
	ldr r1, [r5, #0x70]
	adds r2, r5, #0
	adds r2, #0x84
	ldr r2, [r2]
	ldr r2, [r2]
	ldr r3, [r3, #4]
	bl sub_803AD84
	adds r0, r5, #0
	adds r0, #0x80
	str r4, [r0]
_0800C84E:
	ldr r0, [r5, #0x70]
	ldr r1, [r0]
	str r1, [r5, #0x60]
	ldr r0, [r0, #4]
	str r0, [r5, #0x64]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_800C860
sub_800C860: @ 0x0800C860
	push {r4, r5, lr}
	ldr r5, [r0, #0x70]
	ldr r4, [r5]
	lsls r1, r1, #8
	subs r4, r4, r1
	str r4, [r0, #0x10]
	ldr r4, [r5]
	adds r4, r4, r1
	str r4, [r0, #0x14]
	str r3, [r0, #0x5c]
	str r2, [r0, #0x58]
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_800C87C
sub_800C87C: @ 0x0800C87C
	push {r4, r5, lr}
	ldr r5, [r0, #0x70]
	ldr r4, [r5, #4]
	lsls r1, r1, #8
	subs r4, r4, r1
	str r4, [r0, #0x1c]
	ldr r4, [r5, #4]
	adds r4, r4, r1
	str r4, [r0, #0x18]
	str r3, [r0, #0x5c]
	str r2, [r0, #0x58]
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_800C898
sub_800C898: @ 0x0800C898
	ldr r3, [r0, #0x70]
	ldr r2, [r3]
	lsls r1, r1, #8
	subs r2, r2, r1
	str r2, [r0, #0x10]
	ldr r2, [r3]
	adds r2, r2, r1
	str r2, [r0, #0x14]
	bx lr
	.align 2, 0
