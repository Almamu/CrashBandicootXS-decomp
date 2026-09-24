.include "asm/macros.inc"

.syntax unified
.arm

	.align 2, 0

.if NON_MATCHING == 0
	thumb_func_start sub_801E788
sub_801E788: @ 0x0801E788
	push {r4, r5, r6, r7, lr}
	adds r7, r0, #0
	ldrb r1, [r7, #0x11]
	lsls r0, r1, #0x1e
	lsrs r0, r0, #0x1e
	cmp r0, #1
	beq _0801E7C0
	cmp r0, #1
	blo _0801E7A0
	cmp r0, #3
	beq _0801E810
	b _0801E84E
_0801E7A0:
	ldr r1, [r7]
	ldr r2, _0801E7B8 @ =0x000001FF
	adds r0, r2, #0
	ands r1, r0
	ldr r0, _0801E7BC @ =0xFFFFFE00
	ldrh r3, [r7, #0x12]
	ands r0, r3
	orrs r0, r1
	strh r0, [r7, #0x12]
	ldr r0, [r7, #4]
	strb r0, [r7, #0x10]
	b _0801E84E
	.align 2, 0
_0801E7B8: .4byte 0x000001FF
_0801E7BC: .4byte 0xFFFFFE00
_0801E7C0:
	ldr r0, _0801E800 @ =gStaticData_0816C644
	ldr r2, [r7, #0x18]
	lsls r2, r2, #2
	adds r0, r2, r0
	ldr r0, [r0]
	ldr r1, [r7, #8]
	subs r0, r0, r1
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	ldr r1, [r7]
	subs r1, r1, r0
	ldr r3, _0801E804 @ =0x000001FF
	adds r0, r3, #0
	ands r1, r0
	ldr r0, _0801E808 @ =0xFFFFFE00
	ldrh r3, [r7, #0x12]
	ands r0, r3
	orrs r0, r1
	strh r0, [r7, #0x12]
	ldr r0, _0801E80C @ =gStaticData_0816C674
	adds r2, r2, r0
	ldr r0, [r2]
	ldr r1, [r7, #0xc]
	subs r0, r0, r1
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	ldrb r1, [r7, #4]
	subs r0, r1, r0
	strb r0, [r7, #0x10]
	b _0801E84E
	.align 2, 0
_0801E800: .4byte gStaticData_0816C644
_0801E804: .4byte 0x000001FF
_0801E808: .4byte 0xFFFFFE00
_0801E80C: .4byte gStaticData_0816C674
_0801E810:
	ldr r0, [r7, #8]
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	ldr r1, [r7]
	adds r1, r1, r0
	ldr r0, _0801E86C @ =gStaticData_0816C644
	ldr r3, [r7, #0x18]
	lsls r3, r3, #2
	adds r0, r3, r0
	ldr r0, [r0]
	subs r1, r1, r0
	ldr r2, _0801E870 @ =0x000001FF
	adds r0, r2, #0
	ands r1, r0
	ldr r0, _0801E874 @ =0xFFFFFE00
	ldrh r2, [r7, #0x12]
	ands r0, r2
	orrs r0, r1
	strh r0, [r7, #0x12]
	ldr r2, [r7, #4]
	ldr r0, [r7, #0xc]
	lsrs r1, r0, #0x1f
	adds r0, r0, r1
	asrs r0, r0, #1
	adds r2, r2, r0
	ldr r0, _0801E878 @ =gStaticData_0816C674
	adds r3, r3, r0
	ldrb r3, [r3]
	subs r2, r2, r3
	strb r2, [r7, #0x10]
_0801E84E:
	movs r0, #3
	ldrb r3, [r7, #0x11]
	ands r0, r3
	cmp r0, #0
	bne _0801E880
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r1, [r7, #0x13]
	ands r0, r1
	movs r1, #0x21
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r7, #0x13]
	ldr r6, _0801E87C @ =gUnknown_03001300
	b _0801E8E2
	.align 2, 0
_0801E86C: .4byte gStaticData_0816C644
_0801E870: .4byte 0x000001FF
_0801E874: .4byte 0xFFFFFE00
_0801E878: .4byte gStaticData_0816C674
_0801E87C: .4byte gUnknown_03001300
_0801E880:
	ldr r6, _0801E8F4 @ =gUnknown_03001300
	ldr r4, [r6]
	ldr r0, [r4, #8]
	adds r3, r0, #0
	adds r0, #1
	str r0, [r4, #8]
	movs r0, #7
	adds r1, r3, #0
	ands r1, r0
	lsls r1, r1, #1
	movs r0, #0xf
	rsbs r0, r0, #0
	ldrb r2, [r7, #0x13]
	ands r0, r2
	orrs r0, r1
	asrs r1, r3, #3
	movs r5, #1
	ands r1, r5
	lsls r1, r1, #4
	movs r2, #0x11
	rsbs r2, r2, #0
	ands r0, r2
	orrs r0, r1
	asrs r1, r3, #4
	ands r1, r5
	lsls r1, r1, #5
	subs r2, #0x10
	ands r0, r2
	orrs r0, r1
	strb r0, [r7, #0x13]
	ldrh r0, [r7, #0x20]
	lsls r1, r3, #2
	lsls r3, r3, #5
	adds r3, r4, r3
	movs r2, #0
	strh r0, [r3, #0x12]
	adds r0, r1, #1
	lsls r0, r0, #3
	adds r0, r4, r0
	strh r2, [r0, #0x12]
	adds r0, r1, #2
	lsls r0, r0, #3
	adds r0, r4, r0
	strh r2, [r0, #0x12]
	ldrh r0, [r7, #0x24]
	adds r1, #3
	lsls r1, r1, #3
	adds r4, r4, r1
	strh r0, [r4, #0x12]
_0801E8E2:
	ldr r0, [r6]
	adds r1, r7, #0
	adds r1, #0x10
	bl sub_8006AC8
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0801E8F4: .4byte gUnknown_03001300
.endif

