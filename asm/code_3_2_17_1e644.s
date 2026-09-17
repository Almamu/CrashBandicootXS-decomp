.include "asm/macros.inc"

.syntax unified
.arm

@ sub_801E644: not yet matched (NON_MATCHING) - see
@ src/graphics/graphics_package_1e640.c, guarded by #if NON_MATCHING - this
@ raw version is only assembled for the default (matching) build. See
@ docs/matching/issue-30-graphics-loading.md.
.if NON_MATCHING == 0
	thumb_func_start sub_801E644
sub_801E644: @ 0x0801E644
	push {r4, r5, r6, r7, lr}
	ldr r5, [sp, #0x14]
	movs r4, #0
	strh r4, [r0, #0xc]
	movs r6, #3
	ands r5, r6
	subs r4, #4
	ldrb r7, [r0, #0xc]
	ands r4, r7
	orrs r4, r5
	str r1, [r0]
	ands r1, r6
	lsls r1, r1, #2
	movs r5, #0xd
	rsbs r5, r5, #0
	ands r4, r5
	orrs r4, r1
	strb r4, [r0, #0xc]
	movs r1, #0x3f
	ldrb r4, [r0, #0xd]
	ands r1, r4
	str r2, [r0, #4]
	movs r4, #0x1f
	ands r2, r4
	movs r4, #0x20
	rsbs r4, r4, #0
	ands r1, r4
	orrs r1, r2
	strb r1, [r0, #0xd]
	str r3, [r0, #8]
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
.endif
	.align 2, 0

	thumb_func_start sub_801E688
sub_801E688: @ 0x0801E688
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	adds r6, r0, #0
	adds r7, r1, #0
	mov r8, r2
	str r7, [r6, #8]
	str r2, [r6, #0xc]
	movs r0, #0x80
	lsls r0, r0, #5
	mov ip, r0
	movs r3, #0
	ldr r1, _0801E748 @ =gStaticData_0816C644
	mov sb, r1
	ldr r2, _0801E74C @ =gStaticData_0816C674
	mov sl, r2
	mov r5, sl
	mov r4, sb
_0801E6B0:
	ldr r2, [r4]
	lsls r0, r2, #1
	cmp r7, r0
	bgt _0801E6CC
	ldr r1, [r5]
	lsls r0, r1, #1
	cmp r8, r0
	bgt _0801E6CC
	adds r0, r2, #0
	muls r0, r1, r0
	cmp r0, ip
	bge _0801E6CC
	mov ip, r0
	str r3, [r6, #0x18]
_0801E6CC:
	adds r5, #4
	adds r4, #4
	adds r3, #1
	cmp r3, #0xb
	ble _0801E6B0
	ldr r4, [r6, #0x18]
	lsls r1, r4, #6
	movs r2, #0x3f
	adds r0, r2, #0
	ldrb r3, [r6, #0x13]
	ands r0, r3
	orrs r0, r1
	strb r0, [r6, #0x13]
	asrs r0, r4, #2
	lsls r0, r0, #6
	adds r5, r2, #0
	ldrb r1, [r6, #0x11]
	ands r5, r1
	orrs r5, r0
	strb r5, [r6, #0x11]
	mov r0, ip
	cmp r0, #0
	bge _0801E6FC
	adds r0, #0x1f
_0801E6FC:
	asrs r0, r0, #5
	movs r2, #0x80
	lsls r2, r2, #3
	adds r1, r2, #0
	subs r1, r1, r0
	ldr r3, _0801E750 @ =0x000003FF
	adds r0, r3, #0
	ands r1, r0
	ldr r0, _0801E754 @ =0xFFFFFC00
	ldrh r2, [r6, #0x14]
	ands r0, r2
	orrs r0, r1
	strh r0, [r6, #0x14]
	lsls r4, r4, #2
	mov r3, sb
	adds r0, r4, r3
	ldr r0, [r0]
	lsls r0, r0, #8
	adds r1, r7, #0
	bl sub_803ADB4
	adds r7, r0, #0
	str r7, [r6, #0x20]
	add r4, sl
	ldr r0, [r4]
	lsls r0, r0, #8
	mov r1, r8
	bl sub_803ADB4
	str r0, [r6, #0x24]
	cmp r7, #0xff
	ble _0801E740
	cmp r0, #0xff
	bgt _0801E758
_0801E740:
	movs r0, #3
	orrs r5, r0
	b _0801E776
	.align 2, 0
_0801E748: .4byte gStaticData_0816C644
_0801E74C: .4byte gStaticData_0816C674
_0801E750: .4byte 0x000003FF
_0801E754: .4byte 0xFFFFFC00
_0801E758:
	movs r1, #0x80
	lsls r1, r1, #1
	cmp r7, r1
	bgt _0801E764
	cmp r0, r1
	ble _0801E770
_0801E764:
	movs r0, #4
	rsbs r0, r0, #0
	ands r5, r0
	movs r0, #1
	orrs r5, r0
	b _0801E776
_0801E770:
	movs r0, #4
	rsbs r0, r0, #0
	ands r5, r0
_0801E776:
	strb r5, [r6, #0x11]
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

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

