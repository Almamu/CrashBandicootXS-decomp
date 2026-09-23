.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8025894
sub_8025894: @ 0x08025894
	push {r4, r5, r6, lr}
	adds r5, r1, #0
	movs r6, #0
	ldrh r2, [r5, #2]
	subs r2, #1
	cmp r2, #0
	blt _0802593A
_080258A2:
	lsls r1, r2, #3
	ldr r0, [r5, #4]
	adds r4, r0, r1
	movs r3, #0
	subs r2, #1
	b _08025930
_080258AE:
	lsls r1, r3, #3
	ldr r0, [r4, #4]
	adds r1, r0, r1
	ldrh r0, [r1]
	cmp r0, #0x1a
	bne _080258CC
	ldr r0, [r5, #8]
	ldrh r1, [r1, #6]
	lsls r1, r1, #1
	adds r1, r1, r0
	ldr r0, [r5, #0xc]
	ldrh r1, [r1]
	adds r0, r1, r0
	movs r1, #8
	ldrsh r0, [r0, r1]
_080258CC:
	subs r0, #0x15
	cmp r0, #0x12
	bhi _0802592E
	lsls r0, r0, #2
	ldr r1, _080258DC @ =_080258E0
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080258DC: .4byte _080258E0
_080258E0: @ jump table
	.4byte _0802592C @ case 0
	.4byte _0802592C @ case 1
	.4byte _0802592C @ case 2
	.4byte _0802592E @ case 3
	.4byte _0802592C @ case 4
	.4byte _0802592E @ case 5
	.4byte _0802592E @ case 6
	.4byte _0802592E @ case 7
	.4byte _0802592E @ case 8
	.4byte _0802592C @ case 9
	.4byte _0802592C @ case 10
	.4byte _0802592C @ case 11
	.4byte _0802592C @ case 12
	.4byte _0802592C @ case 13
	.4byte _0802592C @ case 14
	.4byte _0802592C @ case 15
	.4byte _0802592C @ case 16
	.4byte _0802592C @ case 17
	.4byte _0802592C @ case 18
_0802592C:
	adds r6, #1
_0802592E:
	adds r3, #1
_08025930:
	ldrh r0, [r4, #2]
	cmp r3, r0
	blt _080258AE
	cmp r2, #0
	bge _080258A2
_0802593A:
	adds r0, r6, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
