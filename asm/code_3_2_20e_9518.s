.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8039518
sub_8039518: @ 0x08039518
	push {r4, r5, lr}
	adds r5, r0, #0
	movs r2, #0
	str r2, [r5, #0x44]
	strb r2, [r5, #0x10]
	str r2, [r5, #0x3c]
	movs r1, #0
	ldr r0, _08039570 @ =0x00008AD0
	strh r0, [r5, #0x2a]
	movs r3, #1
	strb r3, [r5, #0x11]
	movs r0, #0xff
	strb r0, [r5, #0x15]
	movs r0, #1
	rsbs r0, r0, #0
	strb r0, [r5, #0x18]
	strb r1, [r5, #0xc]
	strb r1, [r5, #0x12]
	strb r1, [r5, #0xd]
	strb r1, [r5, #0xf]
	strb r1, [r5, #0xe]
	adds r0, r5, #0
	adds r0, #0x24
	strb r1, [r0]
	adds r0, #1
	strb r1, [r0]
	strh r2, [r5, #0x34]
	strh r2, [r5, #0x32]
	strh r2, [r5, #0x30]
	adds r0, #0x2d
	strb r3, [r0]
	ldr r4, _08039574 @ =gUnknown_03001618
	ldr r0, [r5, #4]
	ldrh r2, [r0, #2]
	movs r3, #0
	ldr r0, _08039578 @ =0x00000000
	ldr r1, _0803957C @ =0x00000001
	bl sub_8037A7C
	str r0, [r4]
	str r1, [r4, #4]
	movs r4, #0
	b _08039592
	.align 2, 0
_08039570: .4byte 0x00008AD0
_08039574: .4byte gUnknown_03001618
_08039578: .4byte 0x00000000
_0803957C: .4byte 0x00000001
_08039580:
	ldr r1, [r5, #8]
	lsls r0, r4, #2
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r1, [r1]
	bl sub_803AD7C
	adds r4, #1
_08039592:
	ldr r0, [r5]
	ldr r0, [r0, #0xc]
	cmp r4, r0
	blo _08039580
	pop {r4, r5}
	pop {r0}
	bx r0

