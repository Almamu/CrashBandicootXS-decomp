.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_803943C
sub_803943C: @ 0x0803943C
	push {r4, r5, r6, lr}
	adds r3, r0, #0
	adds r6, r2, #0
	ldr r0, [r3, #0xc]
	cmp r0, r6
	beq _08039510
	ldrb r0, [r3, #0x1a]
	cmp r0, #0
	beq _08039510
	ldrb r0, [r3, #0x18]
	ldrh r4, [r3, #0x18]
	ldrb r1, [r3, #0x1c]
	cmp r0, #0
	beq _080394E4
	adds r2, r1, #0
	cmp r2, #0
	bne _080394E4
	adds r1, r3, #0
	adds r1, #0x22
	ldrb r0, [r1]
	cmp r0, #0
	beq _08039472
	strb r2, [r1]
	ldr r0, [r3]
	ldr r0, [r0, #0x18]
	ldrh r0, [r0, #2]
	b _08039476
_08039472:
	ldrh r0, [r3, #0x16]
	adds r0, #1
_08039476:
	strh r0, [r3, #0x16]
	ldrh r2, [r3, #0x18]
	lsrs r1, r2, #8
	cmp r1, #0
	beq _0803948A
	movs r0, #0xff
	ands r0, r2
	lsls r0, r0, #8
	orrs r1, r0
	strh r1, [r3, #0x18]
_0803948A:
	ldrb r0, [r3, #0x18]
	subs r0, #1
	movs r4, #0
	strb r0, [r3, #0x1c]
	movs r0, #0x16
	ldrsh r1, [r3, r0]
	ldr r2, [r3]
	ldr r0, [r2, #0x18]
	ldrh r0, [r0, #2]
	cmp r1, r0
	blt _080394DA
	strh r4, [r3, #0x16]
	strh r4, [r3, #0x24]
	movs r5, #1
	strb r5, [r3, #0x1e]
	ldrh r0, [r3, #0x14]
	adds r0, #1
	strh r0, [r3, #0x14]
	movs r0, #0x14
	ldrsh r1, [r3, r0]
	ldr r0, [r2, #0x18]
	ldrh r0, [r0, #4]
	cmp r1, r0
	blt _080394DC
	adds r0, r3, #0
	adds r0, #0x20
	ldrb r0, [r0]
	cmp r0, #0
	beq _080394CA
	movs r0, #0
	strb r0, [r3, #0x1a]
	strh r4, [r3, #0x18]
_080394CA:
	adds r0, r3, #0
	adds r0, #0x21
	strb r5, [r0]
	ldr r0, [r3]
	ldr r0, [r0, #0x18]
	ldrh r0, [r0, #6]
	strh r0, [r3, #0x14]
	b _080394DC
_080394DA:
	strb r4, [r3, #0x1e]
_080394DC:
	movs r0, #1
	strb r0, [r3, #0x1d]
	ldrh r4, [r3, #0x18]
	b _080394EC
_080394E4:
	subs r0, r1, #1
	movs r1, #0
	strb r0, [r3, #0x1c]
	strb r1, [r3, #0x1d]
_080394EC:
	movs r0, #0xff
	ands r0, r4
	cmp r0, #0
	bne _080394FC
	adds r1, r3, #0
	adds r1, #0x21
	movs r0, #1
	strb r0, [r1]
_080394FC:
	str r6, [r3, #0xc]
	ldr r0, [r3, #0x10]
	cmp r0, #0
	bne _08039506
	str r6, [r3, #0x10]
_08039506:
	ldrb r0, [r3, #0x1b]
	cmp r0, #0
	beq _08039510
	subs r0, #1
	strb r0, [r3, #0x1b]
_08039510:
	movs r0, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1

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

