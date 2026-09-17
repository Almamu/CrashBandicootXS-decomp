.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_80392E0
sub_80392E0: @ 0x080392E0
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	ldr r1, _0803939C @ =0x04000208
	movs r0, #0
	strh r0, [r1]
	movs r0, #0
	bl sub_80391E8
	movs r0, #1
	bl sub_80391E8
	movs r0, #2
	bl sub_80391E8
	movs r0, #3
	bl sub_80391E8
	movs r0, #0xc0
	lsls r0, r0, #0x13
	movs r1, #0x80
	lsls r1, r1, #9
	bl sub_8037F3C
	ldr r0, _080393A0 @ =gUnknown_030008D0
	ldr r4, _080393A4 @ =0x06004000
	adds r1, r4, #0
	bl sub_80392C4
	movs r1, #0
	movs r0, #0xf
_0803931E:
	strh r1, [r4]
	adds r4, #2
	subs r0, #1
	cmp r0, #0
	bge _0803931E
	ldr r1, _080393A8 @ =0x060044B8
	movs r0, #0x80
	lsls r0, r0, #5
	str r0, [r1]
	ldr r0, _080393AC @ =0x060044C8
	movs r1, #0x80
	lsls r1, r1, #9
	str r1, [r0]
	adds r0, #0xc
	str r1, [r0]
	ldr r1, _080393B0 @ =0x060044FC
	ldr r0, _080393B4 @ =0x01111110
	str r0, [r1]
	ldr r0, _080393B8 @ =gStaticData_085A62C8
	ldr r2, [r0]
	movs r0, #0
	movs r1, #0
	bl sub_8039214
	ldr r2, _080393BC @ =gStaticData_085A62CC
	movs r0, #0
	movs r1, #5
	bl sub_8039214
	movs r0, #0xf
	movs r1, #5
	adds r2, r5, #0
	bl sub_8039214
	movs r0, #0
	movs r1, #7
	adds r2, r6, #0
	bl sub_8039214
	ldr r0, _080393C0 @ =0x05000002
	movs r2, #0
	strh r2, [r0]
	movs r1, #0xa0
	lsls r1, r1, #0x13
	ldr r3, _080393C4 @ =0x00007FFF
	adds r0, r3, #0
	strh r0, [r1]
	ldr r1, _080393C8 @ =0x04000008
	movs r0, #4
	strh r0, [r1]
	subs r1, #8
	movs r3, #0x80
	lsls r3, r3, #1
	adds r0, r3, #0
	strh r0, [r1]
	ldr r0, _080393CC @ =0x04000050
	strh r2, [r0]
	adds r0, #2
	strh r2, [r0]
	adds r0, #2
	strh r2, [r0]
_08039398:
	b _08039398
	.align 2, 0
_0803939C: .4byte 0x04000208
_080393A0: .4byte gUnknown_030008D0
_080393A4: .4byte 0x06004000
_080393A8: .4byte 0x060044B8
_080393AC: .4byte 0x060044C8
_080393B0: .4byte 0x060044FC
_080393B4: .4byte 0x01111110
_080393B8: .4byte gStaticData_085A62C8
_080393BC: .4byte gStaticData_085A62CC
_080393C0: .4byte 0x05000002
_080393C4: .4byte 0x00007FFF
_080393C8: .4byte 0x04000008
_080393CC: .4byte 0x04000050

