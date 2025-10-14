.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8000440
sub_8000440: @ 0x08000440
	push {r4, lr}
	adds r3, r0, #0
	cmp r1, #0
	bge _08000454
	ldr r0, _0800044C @ =mem_iwram_heap
	b _08000456
	.align 2, 0
_0800044C: .4byte mem_iwram_heap
_08000450:
	movs r0, #0
	b _080004A4
_08000454:
	ldr r0, _08000468 @ =mem_ewram_heap
_08000456:
	ldr r4, [r0]
	adds r3, #0x13
	movs r0, #4
	rsbs r0, r0, #0
	ands r3, r0
	ldr r2, [r4, #0x10]
	ldr r1, [r2, #0xc]
	b _08000472
	.align 2, 0
_08000468: .4byte mem_ewram_heap
_0800046C:
	cmp r2, r1
	beq _08000450
	ldr r2, [r2, #8]
_08000472:
	ldr r0, [r2, #4]
	cmp r0, #0
	bne _0800046C
	ldr r0, [r2]
	cmp r0, r3
	blo _0800046C
	subs r1, r0, r3
	cmp r1, #0x40
	bls _08000498
	adds r0, r2, r3
	str r1, [r0]
	movs r1, #0
	str r1, [r0, #4]
	str r2, [r0, #0xc]
	ldr r1, [r2, #8]
	str r1, [r0, #8]
	str r0, [r1, #0xc]
	str r0, [r2, #8]
	str r3, [r2]
_08000498:
	movs r0, #2
	str r0, [r2, #4]
	ldr r0, [r2, #8]
	str r0, [r4, #0x10]
	adds r0, r2, #0
	adds r0, #0x10
_080004A4:
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_80004AC
sub_80004AC: @ 0x080004AC
	push {r4, lr}
	adds r1, r0, #0
	cmp r1, #0
	beq _0800050E
	ldr r0, _080004C0 @ =mem_iwram_heap
	ldr r0, [r0]
	cmp r1, r0
	blo _080004C4
	adds r4, r0, #0
	b _080004C8
	.align 2, 0
_080004C0: .4byte mem_iwram_heap
_080004C4:
	ldr r0, _08000514 @ =mem_ewram_heap
	ldr r4, [r0]
_080004C8:
	adds r3, r1, #0
	subs r3, #0x10
	movs r0, #0
	str r0, [r3, #4]
	ldr r2, [r3, #0xc]
	ldr r0, [r2, #4]
	cmp r0, #0
	bne _080004F0
	ldr r0, [r2]
	ldr r1, [r3]
	adds r0, r0, r1
	str r0, [r2]
	ldr r0, [r3, #8]
	str r0, [r2, #8]
	str r2, [r0, #0xc]
	ldr r0, [r4, #0x10]
	cmp r3, r0
	bne _080004EE
	str r2, [r4, #0x10]
_080004EE:
	adds r3, r2, #0
_080004F0:
	ldr r2, [r3, #8]
	ldr r0, [r2, #4]
	cmp r0, #0
	bne _0800050E
	ldr r0, [r3]
	ldr r1, [r2]
	adds r0, r0, r1
	str r0, [r3]
	ldr r0, [r2, #8]
	str r0, [r3, #8]
	str r3, [r0, #0xc]
	ldr r0, [r4, #0x10]
	cmp r2, r0
	bne _0800050E
	str r3, [r4, #0x10]
_0800050E:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08000514: .4byte mem_ewram_heap

	thumb_func_start sub_8000518
sub_8000518: @ 0x08000518
	push {r4, lr}
	movs r4, #0xc0
	lsls r4, r4, #0x18
	adds r0, r4, #0
	bl mem_free_bytes
	ldr r1, _08000540 @ =gUnknown_030007D4
	ldr r1, [r1]
	cmp r1, r0
	beq _08000538
	adds r0, r4, #0
	bl mem_collect
	adds r0, r4, #0
	bl mem_free_bytes
_08000538:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08000540: .4byte gUnknown_030007D4

	thumb_func_start sub_8000544
sub_8000544: @ 0x08000544
	ldr r1, _08000550 @ =gUnknown_030009E8
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r1, _08000554 @ =nullsub_10
	str r1, [r0]
	bx lr
	.align 2, 0
_08000550: .4byte gUnknown_030009E8
_08000554: .4byte nullsub_10

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
