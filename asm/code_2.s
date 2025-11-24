.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8000558
sub_8000558: @ 0x08000558
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	ldr r0, _0800058C @ =gUnknown_030009E8
	lsls r2, r5, #2
	adds r0, r2, r0
	ldr r1, _08000590 @ =gUnknown_03000A20
	adds r6, r2, r1
	ldr r1, [r6]
	str r1, [r0]
	cmp r1, #0
	bne _08000582
	ldr r3, _08000594 @ =0x04000208
	ldrh r2, [r3]
	strh r1, [r3]
	ldr r4, _08000598 @ =0x04000200
	movs r1, #1
	lsls r1, r5
	ldrh r0, [r4]
	bics r0, r1
	strh r0, [r4]
	strh r2, [r3]
_08000582:
	ldr r0, _0800059C @ =nullsub_10
	str r0, [r6]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0800058C: .4byte gUnknown_030009E8
_08000590: .4byte gUnknown_03000A20
_08000594: .4byte 0x04000208
_08000598: .4byte 0x04000200
_0800059C: .4byte nullsub_10

	thumb_func_start sub_80005A0
sub_80005A0: @ 0x080005A0
	push {r4, lr}
	ldr r4, _080005C4 @ =gUnknown_03000A20
	lsls r2, r0, #2
	adds r4, r2, r4
	ldr r3, _080005C8 @ =gUnknown_030009E8
	adds r2, r2, r3
	ldr r3, [r2]
	str r3, [r4]
	str r1, [r2]
	ldr r2, _080005CC @ =0x04000200
	movs r1, #1
	lsls r1, r0
	ldrh r0, [r2]
	orrs r1, r0
	strh r1, [r2]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080005C4: .4byte gUnknown_03000A20
_080005C8: .4byte gUnknown_030009E8
_080005CC: .4byte 0x04000200

	thumb_func_start sub_80005D0
sub_80005D0: @ 0x080005D0
	ldr r1, _080005D8 @ =0x04000208
	movs r0, #0
	strh r0, [r1]
	bx lr
	.align 2, 0
_080005D8: .4byte 0x04000208
